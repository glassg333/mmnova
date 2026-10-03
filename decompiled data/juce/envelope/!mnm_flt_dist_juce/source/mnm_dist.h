// ============================================================================
// mnm_dist.h — DIST (дисторшн/хедрум тракта), Monomachine OS 1.32B
// ----------------------------------------------------------------------------
// Декомпиляция P:$079F-$07D1 ядра DSP1 (per-voice func_0004A8). Выписка:
// reference/kernel_P079F-07DB_dist_and_coupling.txt. Измерено на эмуляторе
// (MnMAmpDistCORE_VERIFIED.h, "stop@P:$07D2", синус 3/16 ампл. 0.5):
// статическое усиление ступени = 4.000·k, жёсткий клип ±1.0 при 4k ≥ 2.
//
// ТОЧНЫЙ ЗАКОН (транскрипция P:$07A6-$07B7):
//   D  = Y:(V-$24)                 ; слово параметра DIST, Q1.23   [P:$07A6]
//   a  = 2·D                       ; P:$07A7 asl a
//   a += −1.0                      ; P:$07A8 add #>$800000 ($800000 = −1.0!)
//   x0 = max(0, a)                 ; P:$07AA clr a ifmi
//   idx = floor(x0·256)            ; P:$07AD mpyi #$100, P:$07B1 move b,r2
//   c   = kDrive[idx]              ; P:$07B7 x:(r2+$1447C6) — таблица 256 слов,
//                                  ;   1.0 → 0.3535534 (sqrt(2)/4) к idx 128, дальше flat
//   k   = x0²·$7DECCD + $21333     ; P:$07AF/07B3/07B5
//       = 0.9837891·x0² + 0.0162109;   k(0)=0.01621, k(1)=1.0000
//
// СТУПЕНЬ (P:$07BD-$07D1): 34 слова блока масштабируются k·y1 (y1 = частное
// деления 8/X:(V-$1D), P:$079F-$07A5), затем ←7 (умножение на $80=128,
// P:$07C8) ПОД ФЛАГОМ НАСЫЩЕНИЯ SR bit11 (P:$07C3 bset / P:$07D1 bclr) —
// жёсткий клип 24-бит слова. Суммарная измеренная отдача ступени = 4k.
// Семантика X:(V-$1D) (X-сторона окна голоса) — OPEN.
//
// СВЯЗЬ С ФИЛЬТРОМ (мануал: "distortion is applied as an integrated part of
// the filter... controls the headroom for the filter and the EQ"):
//   c (кривая привода) входит множителем в КОЭФФИЦИЕНТ СРЕЗА:
//   P:$07B9 mpy x1,y0,b (b = c·y1); P:$07D2-07D6 clamp idx 0..$6A3;
//   P:$07D9 x:(r0+$143546) (kCutoff); P:$07DB mpy y1,y0 → больше drive →
//   меньше c → темнее срез.
//
// ПОВЕДЕНИЕ РУЧКИ (подтверждено владельцем машины, диапазон −64..+63):
//   значения < 0 ОСЛАБЛЯЮТ вход в тракт фильтр→дист (фильтр слабее
//   перегружается), значения > 0 усиливают перегруз. Огибающих у дисторшна
//   НЕТ. Точная кривая ColdFire «ручка → слово D» и закон аттенюации
//   отрицательной половины — OPEN (сторона ColdFire не дизассемблирована);
//   здесь принят линейный маппинг setKnob(), изолированный в одном методе.
//
// ЧЕГО ДЕЛАТЬ НЕ НАДО (MnMAmpDistCORE_VERIFIED.h):
//   * НЕ моделировать дисторшн как tanh/фолд/"клип с drive 1..8";
//   * НЕ подавать D в диапазоне 0..0.5 как «активный» — там x0=0 (мёртвая
//     зона привода; k=0.01621);
//   * НЕ убирать промежуточную ступень насыщения (sat24 на каждом каскаде).
// ============================================================================
#pragma once
#include <cmath>
#include <algorithm>
#include "mnm_fixed.h"
#include "mnm_tables_data.h"

namespace mnm {

class MnmDist
{
public:
    void reset() noexcept {}

    // --- первичный вход: СЛОВО параметра DIST (Q1.23), как его пишет ядро ---
    void setDistWord(uint32_t dWord) noexcept
    {
        d_ = q23ToF(dWord & 0xFFFFFFu);
        recompute();
    }

    // --- порт-конвенция ручки −64..+63 (OPEN: точная кривая ColdFire) ------
    // Линейно: knob −64..+63 → D 0..1. Привод активен только выше центра
    // (x0 = max(0,2D−1)), что воспроизводит «больше 0 — дист усиливается».
    void setKnob(int knob) noexcept
    {
        knob = std::clamp(knob, -64, 63);
        const double d = (double)(knob + 64) / 127.0;
        setDistWord((uint32_t)(int32_t)(d * 8388608.0) & 0xFFFFFFu);
        // Аттенюация отрицательной половины (поведение машины; кривая OPEN).
        // ПРИМЕНЯЕТСЯ НА ВХОДЕ ЦЕПИ фильтр→дист (см. MnmVoiceFilterDist),
        // т.к. ослабляется именно ВХОД в тракт — фильтр слабее перегружается.
        inputGain_ = knob < 0 ? (float)(1.0 + (double)knob / 64.0) : 1.0f;
    }

    // x·4k с насыщением 24-бит слова (ступень P:$07BD-$07D1, измеренная
    // отдача 4k, клип $7FFFFF/$800000 — «bset #$14,sr»)
    inline float process(float x) noexcept
    {
        const int32_t xq = (int32_t)fToQ23(x);
        const int64_t gainQ = (int64_t)llround((double)gain_ * 4.0 * 8388608.0); // 4k в Q1.23
        const int64_t prod = (int64_t)xq * gainQ;                                // Q46
        return q23ToF((uint32_t)sat24(prod >> 23) & 0xFFFFFFu);
    }

    // кривая привода для связки со срезом (подать в cutoffCoefficient())
    float c() const noexcept { return c_; }
    float k()  const noexcept { return k_; }
    float x0() const noexcept { return x0_; }
    float inputGain() const noexcept { return inputGain_; }

private:
    void recompute() noexcept
    {
        // P:$07A7-07AA: x0 = max(0, 2D − 1)
        x0_ = std::clamp(2.0f * d_ - 1.0f, 0.0f, 1.0f);
        // P:$07AD-07B1: idx = floor(x0·256) (таблица 256 слов, flat за 128)
        const int idx = std::min(int(x0_ * 256.0f), 255);
        c_ = q23ToF(kDrive[(size_t)idx]);                    // P:$07B7
        // P:$07B3/07B5: k = x0²·$7DECCD + $21333
        k_ = 0.9837891f * x0_ * x0_ + 0.0162109f;
        gain_ = k_;                                          // ступень: 4·k (см. process)
    }

    float d_ = 0.5f, x0_ = 0.0f, k_ = 0.0162109f, c_ = 1.0f;
    float gain_ = 0.0162109f;    // ступень даёт 4·gain_ (=4k)
    float inputGain_ = 1.0f;
};

} // namespace mnm
