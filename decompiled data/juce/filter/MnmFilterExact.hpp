// ============================================================================
// MnmFilterExact.hpp — ФИЛЬТР трека Monomachine, режим "ex" (exact)
// Бит-точный порт машинного кода DSP56300 (OS 1.32B, ядро DSP1).
// ============================================================================
// ИСТОЧНИК: pack filter_phaser_pack (decompiled data), итерации 14–16:
//   - кольцо коэффициентов P:$0537–$05A1 (DIV-путь eps верифицирован 9/9
//     конфигураций бит-в-бит, scripts/probe_eps.py);
//   - каскад 1 func_000340 — доказан бит-в-бит (iter.14, svf_fit340.py,
//     совпадение с эмулятором ±2 LSB ~ -130 дБ);
//   - резонанс func_000397 — гребёнка 8 дробных тапов, доказан бит-в-бит
//     (iter.14, svf_recursion_proven.md); крайние точки разбега тапов
//     (HPQ=0 -> шаг 1.0; HPQ=127 -> 15.8828) измерены бит-точно.
//
// ГЛАВНОЕ ОТЛИЧИЕ от текущего режима "mnm" (MnmRealFilter.hpp):
//   1. Резонанс — НЕ TPT-SVF с выдуманным Q=0.5..16, а родная гребёнка
//      дробных тапов с таблицами демпфирования/фидбека ПРОШИВКИ
//      ($1448C6/$144946/$144BC9).
//   2. У фильтра НЕТ собственного энвелопа — доказано бит-точной эмуляцией
//      (iter.15, 08_env_verdict/). Параметры P+$14–$17 страницы голоса — это
//      delay-модуляция, НЕ ATK/DEC/BOFS/WOFS. Срез на реальной машине двигает
//      CPU (LFO/мод-матрица) между кадрами. Поэтому в режиме "ex" ручки
//      ATK/DEC/BOFS/WOFS страницы FILT ПРИНИМАЮТСЯ, НО ИГНОРИРУЮТСЯ (как в
//      железе), а для движения среза используйте мод-матрицу плагина —
//      setParameters() можно звать каждый сэмпл.
//   3. Срез идёт через кольцо div1/div2/width1/width2 + coeff2 с точным
//      DIV-путём eps — не через 258-словную таблицу P:$144AC7 (та таблица в
//      прошивке обслуживает скорости рамп delay-модуляции и aux-стадию).
//
// СЕТКА: кадр = 16 сэмплов (родная сетка машины). process() буферизует сам.
//
// ПОРТ-СОГЛАШЕНИЕ (не из прошивки): bypass при ручках по умолчанию
// (BASE=0, WDTH=127, HPQ=LPQ=0) — чтобы переключение режимов не меняло
// громкость на дефолтном состоянии, как принято в этом плагине.
//
// Зависимости: только MnmFilterExactTables.hpp (тот же каталог).
// ============================================================================
#pragma once
#include <array>
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "MnmFilterExactTables.hpp"

namespace monomachine {
namespace mnm {

static constexpr int   kExactFrame = 16;                  // сэмплов на кадр
static constexpr float kExactInv23 = 1.0f / 8388608.0f;   // Q1.23 -> float

// --- точные целочисленные помощники (аккумулятор DSP56300) ------------------
inline int32_t exactS24(uint32_t w) noexcept
{
    w &= 0xFFFFFFu;
    return (w & 0x800000u) ? (int32_t)(w - 0x1000000u) : (int32_t)w;
}
inline int64_t exactSext56(int64_t v) noexcept
{
    v &= ((int64_t)1 << 56) - 1;
    return (v & ((int64_t)1 << 55)) ? v - ((int64_t)1 << 56) : v;
}

// Параметры = СЛОВА страницы голоса (ровно то, что CPU пишет в Y[P+$10..$13]).
// Конвенция рукояток плагина: knob 0..127 -> word = knob<<16 (как во всех
// бит-точных прогонах пака: BASE=64 -> idx=599, средний срез).
struct FilterExactParams
{
    uint32_t baseWord  = 0u << 16;    // FILT BASE  (Y[P+$10])
    uint32_t wdthWord  = 127u << 16;  // FILT WDTH  (Y[P+$11])
    uint32_t hpqWord   = 0u << 16;    // FILT HPQ   (Y[P+$12])
    uint32_t lpqWord   = 0u << 16;    // FILT LPQ   (Y[P+$13])
};

inline uint32_t exactWordFromKnob(int knob) noexcept
{
    return (uint32_t)(knob & 0xFFFF) << 16;
}

// ---------------------------------------------------------------------------
// Кольцо коэффициентов (P:$0537–$05A1). eps-путь — точная целочисленная
// репликация P:$057D..$059A (24 шага невосстанавливающего деления DSP56300),
// верифицирована 9/9 конфигураций бит-в-бит (pack probe_eps.py).
// ---------------------------------------------------------------------------
struct FilterExactRing { float c[4]; };

inline FilterExactRing computeExactRing(uint32_t baseWord, uint32_t wdthWord) noexcept
{
    const int idx  = (int)std::min<uint64_t>(((uint64_t)baseWord * 1199) >> 23, 1200);
    const int widx = (int)std::min<uint64_t>(((uint64_t)wdthWord * 128) >> 23, 127);

    // avg = (div1+div2)>>1 — СЫРЫЕ СЛОВА (не флоаты!), слово A1 аккумулятора
    const int32_t avgWord =
        ((exact_tables::filt::kDiv1W[(size_t)idx] + exact_tables::filt::kDiv2W[(size_t)widx]) >> 1) & 0xFFFFFF;

    // a = 1.0 (A1 = 1); 24 шага DIV x1,a
    const auto sext24 = [](uint32_t v) noexcept -> int32_t
    {
        v &= 0xFFFFFFu;
        return (v & 0x800000u) ? (int32_t)(v - 0x1000000u) : (int32_t)v;
    };
    uint64_t D = 1u << 24;                    // 48-битный акк. деления (dhi:dlo)
    for (int i = 0; i < 24; ++i)
    {
        uint32_t dhi = (D >> 24) & 0xFFFFFFu, dlo = D & 0xFFFFFFu;
        dhi = ((dhi << 1) | (dlo >> 23)) & 0xFFFFFFu;
        dlo = (dlo << 1) & 0xFFFFFFu;
        const int32_t ss = sext24(avgWord), ds = sext24(dhi);
        dhi = ((ds < 0) == (ss < 0)) ? (dhi - avgWord) & 0xFFFFFFu
                                     : (dhi + avgWord) & 0xFFFFFFu;
        const uint32_t qbit = ((sext24(dhi) < 0) == (ss < 0)) ? 1u : 0u;
        dlo |= qbit;
        D = ((uint64_t)dhi << 24) | dlo;
    }
    const int32_t q0   = (int32_t)(D & 0xFFFFFFu);           // a0 = частное
    const int32_t dDif = exact_tables::filt::kDiv2W[(size_t)widx] - exact_tables::filt::kDiv1W[(size_t)idx];
    int64_t b = ((int64_t)exactS24((uint32_t)q0) * (int64_t)dDif) << 1;  // mpy
    b = exactSext56(b << 6);                                 // asl #6
    const float eps = (float)exactS24((uint32_t)((b >> 24) & 0xFFFFFFu)) * kExactInv23;

    FilterExactRing r;
    const float c2 = exact_tables::filt::kCoeff2[(size_t)idx];
    r.c[0] = -c2 * (0.5f + eps);      // Y:$04 (минус из `mpy -y0,x0,a`)
    r.c[1] = eps;                     // Y:$05
    r.c[2] = exact_tables::filt::kWidth1[(size_t)widx];  // Y:$06
    r.c[3] = exact_tables::filt::kWidth2[(size_t)widx];  // Y:$07
    return r;
}

// ---------------------------------------------------------------------------
// ExactCascade — точный перевод func_000340 (перевод svf_fit340.py 1:1).
// Номера инструкций в комментариях = PC оригинала. Окно истории персистентно
// между кадрами — это живая обратная связь оригинала.
// ---------------------------------------------------------------------------
class ExactCascade
{
public:
    void reset() noexcept
    {
        hx_.fill(0.0f);
        hy_.fill(0.0f);
        ringInit_ = false;
    }

    // ring: текущее сглаженное кольцо коэффициентов (4 слова).
    // in16/out16: ровно kExactFrame сэмплов одного канала.
    void processFrame(const float* in16, float* out16, const float* ring) noexcept
    {
        if (!ringInit_)
        {
            for (int i = 0; i < 4; ++i) ringSmooth_[i] = ring[i];
            ringInit_ = true;
        }
        else
        {
            // CPU оригинала пишет слова раз в кадр; лёгкое сглаживание кольца
            // убирает ступеньки при быстрых модуляциях (квантование оригинала
            // грубее этого сглаживания — pack, mnm_dual_filter.h).
            for (int i = 0; i < 4; ++i)
                ringSmooth_[i] += 0.25f * (ring[i] - ringSmooth_[i]);
        }
        cascade1(in16, out16, ringSmooth_);
    }

private:
    static constexpr int kWin   = 64;
    static constexpr int kMask  = kWin - 1;
    static constexpr int kIn0   = 7;    // вход: X:$97-$90
    static constexpr int kAOut0 = 5;    // шина a_old: iter_i -> hx[5+i] (X:$95+)

    std::array<float, kWin> hx_{};  // X-сторона (вход + состояния a_old)
    std::array<float, kWin> hy_{};  // Y-сторона (состояния b_old)
    float ringSmooth_[4] {};
    bool  ringInit_ = false;

    // 24-битный wrap записи в память DSP (извлечение A1 заворачивает в ±1.0) —
    // родное поведение железа, стабилизирующее фильтр (НЕ ограничитель).
    static float wrap1(float v) noexcept
    {
        return v - 2.0f * std::round(v * 0.5f);
    }

    void cascade1(const float* in16, float* out16, const float* ring) noexcept
    {
        // 16 свежих сэмплов в окно + 4-сэмпловый хвост (аналог записи машины
        // в X:$97..$AC стадией $04F5 — регион шире кадра)
        for (int i = 0; i < kExactFrame + 4; ++i)
            hx_[(kIn0 + i) & kMask] = in16[std::min(i, kExactFrame - 1)];

        int r0 = 6;                       // $96-$90 (r0 = $96 у вызывающего)
        int r1 = 3;                       // $93-$90 (r1 = $93)
        int r4 = 0;                       // кольцо ring, mod 4 (M4 = $000003)
        // Аккумулятор B НЕ персистентен между кадрами: первый же mpy на $034B
        // сбрасывает его (вход b функции неважен — проверено по листингу).
        float b = 0.0f;

        float x0 = hx_[r0]; r0 = (r0 + 1) & kMask;               // 0340
        float a  = x0;                                           // 0342 tfr x0,a
        x0 = hx_[r0]; r0 = (r0 + 1) & kMask;                     // 0342 par x:(r0)+,x0
        float y0 = ring[r4 & 3]; ++r4;                           // 0342 par y:(r4)+,y0

        for (int it = 0; it < kExactFrame; ++it)
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
            hy_[r1] = wrap1(b); r1 = (r1 + 2) & kMask;           // 0349 par b,y:(r1)+n1 (wrap A1)
            a -= y0 * xs;                                        // 034A mac -y0,x0,a
            x0 = hx_[r0]; r0 = (r0 + 1) & kMask;                 // 034A par x:(r0)+,x0
            y0 = ring[r4 & 3]; ++r4;                             // 034A par y:(r4)+,y0
            b = x1 * y0;                                         // 034B mpy x1,y0,b (сброс B)
            const float y1b = ring[r4 & 3]; ++r4;                // 034B par y:(r4)+,y1
            const float aOld = wrap1(a);                         // 034C (a1-экстракция)
            a = x0;                                              // 034C tfr x0,a
            hx_[r1] = aOld; r1 = (r1 - 1) & kMask;               // 034C par a,x:(r1)-
            y0 = aOld;                                           // 034C par a,y0
            b += y1b * y0;                                       // 034D mac y1,y0,b
            x0 = hx_[r0]; r0 = (r0 + 1) & kMask;                 // 034D par x:(r0)+,x0
            y0 = ring[r4 & 3]; ++r4;                             // 034D par y:(r4)+,y0
            b *= 4.0f;                                           // 034E asl #2,b
        }
        // выход = шина a_old: iter_i пишет aOld_i в hx[5+i] (=$95-$90)
        for (int i = 0; i < kExactFrame; ++i)
            out16[i] = hx_[(kAOut0 + i) & kMask];
    }
};

// ---------------------------------------------------------------------------
// FilterExactCore — звуковой узел режима "ex" для плагина.
// каскад 1 (точный) + резонатор (гребёнка 8 дробных тапов, точные таблицы).
// API повторяет RealFilterCore (per-sample process(ch, x)), кадр 16 — внутри.
//
// ЛАТЕНТНОСТЬ: ровно 1 кадр = 16 сэмплов (~0.36 мс @44.1к) — родная блочная
// сетка прошивки: каскад считает выход кадра из ВСЕХ 16 входов кадра, поэтому
// стриминг возможен только с задержкой на кадр (выход n-го сэмпла вызова =
// n-й сэмпл ПРЕДЫДУЩЕГО кадра). Это не «задержка плагина», а сетка машины.
// ---------------------------------------------------------------------------
class FilterExactCore
{
public:
    void reset() noexcept
    {
        for (int c = 0; c < 2; ++c)
        {
            casc_[c].reset();
            hist_[c].fill(0.0f);
            histPos_[c] = 0;
            tapPos_[c] = 0.0f;
            fbState_[c] = 0.0f;
            cnt_[c] = 0;
            buf_[c].fill(0.0f);
            prevOut_[c].fill(0.0f);
            newOut_[c].fill(0.0f);
        }
        tapStep_ = 1.0f;
        ringCur_ = computeExactRing(params_.baseWord, params_.wdthWord);
        for (int i = 0; i < 4; ++i) ringTarget_[i] = ringCur_.c[i];
    }

    // Сетка кадров — 16 сэмплов (родная сетка машины). Частота хоста
    // запоминается для документации; временные константы фильтра табличные и
    // от частоты не зависят (таблицы прошивки = сетка 44.1 кГц).
    void setSampleRate(double sr) noexcept { (void)sr; }

    // Сигнатура 1:1 с RealFilterCore::setParameters для симметрии веток:
    //   base, width, hpq, lpq — FILT-ручки 0..127;
    //   envAtk, envDec, bofs, wofs — ПРИНИМАЮТСЯ И ИГНОРИРУЮТСЯ: у фильтра
    //   прошивки НЕТ энвелопа и BOFS/WOFS (доказано iter.15, 08_env_verdict/;
    //   P+$14–$17 = delay-модуляция). Движение среза — через мод-матрицу
    //   плагина (setParameters можно звать каждый сэмпл, слово сглаживается
    //   по кадрам как у CPU оригинала).
    void setParameters(float base, float width, float hpq, float lpq,
                       float envAtk = 0.0f, float envDec = 64.0f,
                       float bofs = 0.0f, float wofs = 0.0f) noexcept
    {
        (void)envAtk; (void)envDec; (void)bofs; (void)wofs;
        params_.baseWord = exactWordFromKnob((int)std::clamp(base + 0.5f, 0.0f, 127.0f));
        params_.wdthWord = exactWordFromKnob((int)std::clamp(width + 0.5f, 0.0f, 127.0f));
        params_.hpqWord  = exactWordFromKnob((int)std::clamp(hpq + 0.5f, 0.0f, 127.0f));
        params_.lpqWord  = exactWordFromKnob((int)std::clamp(lpq + 0.5f, 0.0f, 127.0f));
        recomputeBypass();
    }

    // Прямая модуляция словами (мод-матрица/LFO плагина — так «двигает»
    // фильтр оригинал): word = 0..0xFFFFFF, Q23.
    void setWords(uint32_t base, uint32_t wdth, uint32_t hpq, uint32_t lpq) noexcept
    {
        params_.baseWord = base; params_.wdthWord = wdth;
        params_.hpqWord = hpq;   params_.lpqWord = lpq;
        recomputeBypass();
    }

    // У фильтра прошивки нет энвелопа — триггеры пустые (оставлены для
    // симметрии вызовов TrackChain::trigger()/release()).
    void trigger() noexcept {}
    void release() noexcept {}

    // Один сэмпл одного канала. ch: 0 = L, 1 = R.
    // Возвращает сэмпл ПРЕДЫДУЩЕГО кадра (латентность 16 сэмплов, см. шапку).
    inline float process(int ch, float x) noexcept
    {
        if (bypass_) return x;                       // THRU (порт-соглашение)
        const int c = (ch == 0) ? 0 : 1;

        const float out = prevOut_[c][(size_t)cnt_[c]];   // выход пред. кадра
        buf_[c][(size_t)cnt_[c]] = x;                      // копим текущий кадр
        if (++cnt_[c] < kExactFrame) return out;

        // кадр заполнен — прогон 16 сэмплов этого канала
        cnt_[c] = 0;
        if (c == 0)
        {
            updateRing();                            // кольцо обновляется раз в кадр

            // Разбег тапов: крайние точки бит-точны (HPQ=0 -> 1.0; HPQ=127 ->
            // 15.8828), промежуток — линейная калибровка [РЕКОНСТР., pack].
            const int hpqS = (int)std::min<uint64_t>(((uint64_t)params_.hpqWord * 128) >> 23, 127);
            const float stepTarget = 1.0f + 14.8828f * ((float)hpqS / 127.0f);
            tapStep_ += 0.25f * (stepTarget - tapStep_);
        }

        casc_[c].processFrame(buf_[c].data(), frameTmp_[c].data(), ringCur_.c);

        // --- резонанс: гребёнка 8 дробных тапов (func_000397) --------------
        const int hpq = (int)std::min<uint64_t>(((uint64_t)params_.hpqWord * 128) >> 23, 127);
        const int lpq = (int)std::min<uint64_t>(((uint64_t)params_.lpqWord * 128) >> 23, 127);
        const float damp = exact_tables::filt::kHpqDamp[(size_t)hpq];
        const float fbHP = exact_tables::filt::kHpqFb[(size_t)hpq];
        const float fbLP = exact_tables::filt::kLpqFb[(size_t)lpq];

        for (int i = 0; i < kExactFrame; ++i)
        {
            const float xin = frameTmp_[c][i];
            push(c, xin);                            // каскад-1 в историю резонатора

            // out = x_mid + frac*(x_mid - x_prev) — точная формула func_000397
            float res = 0.0f;
            float pos = tapPos_[c];
            for (int t = 0; t < 8; ++t)
            {
                const float pf   = std::floor(pos);
                const float frac = pos - pf;
                const int   back = (int)pf;
                const float xPrev = peek(c, back);
                const float xMid  = peek(c, back + 1);
                res += xMid + frac * (xMid - xPrev);
                pos += tapStep_;
            }
            tapPos_[c] = std::fmod(pos, (float)kHist);
            res *= 0.125f;                           // среднее тапов

            // демпфированная обратная связь (грейны hpqFb/lpqFb доказаны)
            const float fb = fbState_[c] * damp + res * 0.5f * (fbHP + fbLP);
            fbState_[c] = fb;

            const float y = xin + fb;
            push(c, y);
            newOut_[c][(size_t)i] = y;
        }

        prevOut_[c].swap(newOut_[c]);
        return out;
    }

    // Диагностика/тесты
    bool bypassed() const noexcept { return bypass_; }
    const float* ringWords() const noexcept { return ringCur_.c; }

private:
    static constexpr int kHist = 1024;

    void updateRing() noexcept
    {
        const FilterExactRing target = computeExactRing(params_.baseWord, params_.wdthWord);
        for (int i = 0; i < 4; ++i) ringTarget_[i] = target.c[i];
        for (int i = 0; i < 4; ++i)
            ringCur_.c[i] += 0.25f * (ringTarget_[i] - ringCur_.c[i]);
    }

    void recomputeBypass() noexcept
    {
        // ПОРТ-СОГЛАШЕНИЕ (не прошивка): дефолт = прозрачный THRU, чтобы
        // переключение mnm/old/ex не меняло громкость на дефолтных ручках.
        const float base  = (float)(params_.baseWord >> 16);
        const float width = (float)(params_.wdthWord >> 16);
        const float hpq   = (float)(params_.hpqWord >> 16);
        const float lpq   = (float)(params_.lpqWord >> 16);
        bypass_ = (base <= 0.5f && width >= 126.5f && hpq <= 0.5f && lpq <= 0.5f);
    }

    void push(int c, float v) noexcept
    {
        hist_[c][(size_t)histPos_[c]] = v;
        histPos_[c] = (histPos_[c] + 1) % kHist;
    }
    float peek(int c, int back) const noexcept
    {
        int idx = histPos_[c] - 1 - back;
        int m = idx % kHist; if (m < 0) m += kHist;
        return hist_[c][(size_t)m];
    }

    FilterExactParams params_;
    ExactCascade casc_[2];
    std::array<float, kHist> hist_[2] {};
    int   histPos_[2] {};
    float tapPos_[2] {};
    float tapStep_ = 1.0f;
    float fbState_[2] {};
    std::array<float, kExactFrame> buf_[2] {};
    std::array<float, kExactFrame> frameTmp_[2] {};
    std::array<float, kExactFrame> prevOut_[2] {};
    std::array<float, kExactFrame> newOut_[2] {};
    int   cnt_[2] {};
    FilterExactRing ringCur_ {};
    float ringTarget_[4] {};
    bool  bypass_ = true;
};

} // namespace mnm
} // namespace monomachine
