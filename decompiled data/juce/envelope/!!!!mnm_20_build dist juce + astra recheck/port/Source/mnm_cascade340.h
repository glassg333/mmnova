// ============================================================================
// mnm_cascade340.h — func_000340: ГИБРИД СИММЕТРИЧНОГО FIR + SVF-ОБРАТНОЙ
//                     СВЯЗИ (каскад 1 фильтра) — порт 1:1 из верифицированного
//                     соответствия svf_fit340.py
//                     Monomachine OS 1.32B, ядро DSP1
// ----------------------------------------------------------------------------
// ИСТОЧНИК: mmnova decompiled data/juce/filter/MnmFilterExact.hpp (класс
// ExactCascade), транслированный 1:1 из svf_fit340.py; доказательство бит-в-бит
// против эмулятора DSP56300 — 0 расхождений по инструкциям (163 контрольные
// точки A/B на кадр) и по памяти $00-$FF (svf_recursion_proven.md, итерация 14).
//
// Структура (вызов из P:$05A1/$05B5 — L/R фазы 2×oversample-конвейера):
//   * r0 = поток входных сэмплов, ходит +1/-1/+1 (симметричный FIR-доступ);
//   * r1 = пинг-понг состояний X:$93/$94(+95);
//   * r4 = Y:$04-$07, M4 = $000003 — МОДУЛЬ-4 КОЛЬЦО коэффициентов:
//          Y:$04 = f (срез), Y:$05-$07 = ширино-модулированные коэффициенты;
//   * константы halfband: x0 = $F528BD (−0.084694564), x1 = $4A4DF0 (+0.580503464)
//     [P:$05CF/05D1] — 4-tap halfband FIR [−a, b, b, −a], ноль на Найквисте;
//   * аккумулятор A между итерациями = следующий входной сэмпл ($34C tfr):
//     SVF-коррекция НЕ накапливается в A — уходит в сохраняемые a_old/b_old;
//   * B — «медленное» состояние: b = (x1·y0 + y1·a_prev) << 2, сбрасывается
//     каждый сэмпл mpy-ем;
//   * записи в память заворачиваются по 24 битам (wrap1) — родное поведение
//     экстракции A1, стабилизирующее фильтр (НЕ ограничитель).
//
// Арифметика порта — float (в исходном верифицированном порте то же самое);
// точность соответствия эмулятору ±2 LSB (~ −130 дБ).
//
// ЛАТЕНТНОСТЬ: ровно 1 кадр = 16 сэмплов (~0.36 мс @44.1к) — родная блочная
// сетка прошивки: каскад считает выход кадра из ВСЕХ 16 входов кадра.
// ============================================================================
#pragma once
#include <array>
#include <cmath>
#include <algorithm>
#include "mnm_dsp56300_math.h"
#include "mnm_filter_ring.h"

namespace mnm {

class Cascade340
{
public:
    void reset() noexcept
    {
        hx_.fill(0.0f);
        hy_.fill(0.0f);
    }

    // ring: текущее кольцо коэффициентов (4 слова Y:$04-$07, из computeRing).
    // in16/out16: ровно kFrame сэмплов одного канала.
    void processFrame(const float* in16, float* out16, const float* ring) noexcept
    {
        // mnm_15/16: сглаживание кольца УБРАНО — ядро пишет слова раз в кадр,
        // коэффициенты применяются как есть (вердикт, пункт 7 состава).
        cascade1(in16, out16, ring);
    }

private:
    static constexpr int kWin   = 64;
    static constexpr int kMask  = kWin - 1;
    static constexpr int kIn0   = 7;    // вход: X:$97-$90
    static constexpr int kAOut0 = 5;    // шина a_old: iter_i → hx[5+i] (X:$95+)

    std::array<float, kWin> hx_{};  // X-сторона (вход + состояния a_old)
    std::array<float, kWin> hy_{};  // Y-сторона (состояния b_old)

    void cascade1(const float* in16, float* out16, const float* ring) noexcept
    {
        // 16 свежих сэмплов в окно + 4-сэмпловый хвост (аналог записи машины
        // в X:$97..$AC стадией $04F5 — регион шире кадра)
        for (int i = 0; i < kFrame + 4; ++i)
            hx_[(kIn0 + i) & kMask] = in16[std::min(i, kFrame - 1)];

        int r0 = 6;                       // $96-$90 (r0 = $96 у вызывающего)
        int r1 = 3;                       // $93-$90 (r1 = $93)
        int r4 = 0;                       // кольцо ring, mod 4 (M4 = $000003)
        // Аккумулятор B не персистентен между кадрами: первый же mpy на $034B
        // сбрасывает его (вход b функции неважен — проверено по листингу).
        float b = 0.0f;

        float x0 = hx_[r0]; r0 = (r0 + 1) & kMask;               // 0340
        float a  = x0;                                           // 0342 tfr x0,a
        x0 = hx_[r0]; r0 = (r0 + 1) & kMask;                     // 0342 par x:(r0)+,x0
        float y0 = ring[r4 & 3]; ++r4;                           // 0342 par y:(r4)+,y0

        for (int it = 0; it < kFrame; ++it)
        {
            a += y0 * x0;                                        // 0345 mac y0,x0,a
            const float x1 = hx_[r0]; r0 = (r0 - 1) & kMask;     // 0345 par x:(r0)-,x1
            const float y1 = ring[r4 & 3]; ++r4;                 // 0345 par y:(r4)+,y1
            a += y0 * x0;                                        // 0346 mac y0,x0,a
            a += y1 * x1;                                        // 0347 mac y1,x1,a
            float xs = hx_[r1]; r1 = (r1 + 1) & kMask;           // 0347 par x:(r1)+,x0
            a -= xs * y1;                                        // 0348 mac -x0,y1,a
            xs = hx_[r1]; r1 = (r1 - 1) & kMask;                 // 0348 par x:(r1)-,x0
            a -= y0 * xs;                                        // 0349 mac -y0,x0,a
            hy_[r1] = wrap1(b); r1 = (r1 + 2) & kMask;           // 0349 par b,y:(r1)+n1
            a -= y0 * xs;                                        // 034A mac -y0,x0,a
            x0 = hx_[r0]; r0 = (r0 + 1) & kMask;                 // 034A par x:(r0)+,x0
            y0 = ring[r4 & 3]; ++r4;                             // 034A par y:(r4)+,y0
            b = x1 * y0;                                         // 034B mpy x1,y0,b (сброс B)
            const float y1b = ring[r4 & 3]; ++r4;                // 034B par y:(r4)+,y1
            const float aOld = wrap1(a);                         // 034C (экстракция A1)
            a = x0;                                              // 034C tfr x0,a
            hx_[r1] = aOld; r1 = (r1 - 1) & kMask;               // 034C par a,x:(r1)-
            y0 = aOld;                                           // 034C par a,y0
            b += y1b * y0;                                       // 034D mac y1,y0,b
            x0 = hx_[r0]; r0 = (r0 + 1) & kMask;                 // 034D par x:(r0)+,x0
            y0 = ring[r4 & 3]; ++r4;                             // 034D par y:(r4)+,y0
            b *= 4.0f;                                           // 034E asl #2,b
        }
        // выход = шина a_old: iter_i пишет aOld_i в hx[5+i] (=$95-$90)
        for (int i = 0; i < kFrame; ++i)
            out16[i] = hx_[(kAOut0 + i) & kMask];
    }
};

} // namespace mnm
