// ============================================================================
// mnm_resonator397.h — func_000397: СКАНЕР ДРОБНЫХ ТАПОВ (резонансная секция
//                       каскада 2, HPQ/LPQ) — порт 1:1 из верифицированного
//                       соответствия svf_fit.py
//                       Monomachine OS 1.32B, ядро DSP1
// ----------------------------------------------------------------------------
// ИСТОЧНИК: mmnova decompiled data/juce/filter/MnmFilterExact.hpp + svf_recursion_proven.md
// (итерация 14: 8/8 вызовов воспроизведены бит-в-бит — инструкции, аккумуляторы
// 56 бит, память $00-$FF).
//
// Структура (вызов резонансной секции — P:$0938; ТРИ ДРУГИХ вызова $0978/$0A5A/
// $0B13 обслуживают track delay — НЕ ПУТАТЬ, см. DECOMPILATION_NOTES.md):
//   * L:$80-$90 — 17 пар (целая позиция тапа, дробь), арифметическая прогрессия
//     с 48-битным шагом (P:$092B-092D);
//   * рекурсия на сэмпл, 8 тапов × 2 шины (a/b), дробная интерполяция:
//         out = x_mid·(1+y0) − y0·x_prev + округление (macr)
//             = x_mid + y0·(x_mid − x_prev)
//   * HPQ → damping = kHpqDamp[hpq>>1]   ($1448C6: 0.500→0.0315)
//   * HPQ → fbGain  = kHpqFb[hpq>>1]     ($144946: 0.0078→0.1204)
//   * LPQ → fbGainLP= kLpqFb[lpq]        ($144BC9: 0.0039→0.4844, шаг 1/256)
//
// РЕКОНСТРУКЦИЯ (помечено): разбег тапов — крайние точки бит-точны
// (HPQ=0 → шаг 1.0; HPQ=127 → 15.8828), промежуток — линейная калибровка пака.
// ============================================================================
#pragma once
#include <array>
#include <cmath>
#include <algorithm>
#include "mnm_fixed.h"
#include "mnm_tables_data.h"

namespace mnm {

class Resonator397
{
public:
    void reset() noexcept
    {
        for (int c = 0; c < 2; ++c)
        {
            hist_[c].fill(0.0f);
            histPos_[c] = 0;
            tapPos_[c] = 0.0f;
            fbState_[c] = 0.0f;
        }
        tapStep_ = 1.0f;
    }

    // hpq/lpq: СЛОВА страниц (knob<<16). Вызывается раз в кадр (L-канал).
    void setQWords(uint32_t hpqWord, uint32_t lpqWord) noexcept
    {
        const int hpq = (int)std::min<uint64_t>(((uint64_t)hpqWord * 128) >> 23, 127);
        const int lpq = (int)std::min<uint64_t>(((uint64_t)lpqWord * 128) >> 23, 127);
        hpq_ = hpq; lpq_ = lpq;
        damp_ = q23ToF(kHpqDamp[(size_t)hpq >> 1]);    // индексация >>1 ($1448C6)
        fbHP_ = q23ToF(kHpqFb[(size_t)hpq >> 1]);      // ($144946)
        fbLP_ = q23ToF(kLpqFb[(size_t)lpq]);           // ($144BC9)

        // Разбег тапов: крайние точки бит-точны (HPQ=0 → 1.0; HPQ=127 → 15.8828),
        // промежуток — линейная калибровка [РЕКОНСТРУКЦИЯ, pack].
        const float stepTarget = 1.0f + 14.8828f * ((float)hpq / 127.0f);
        tapStep_ += 0.25f * (stepTarget - tapStep_);
    }

    // Один сэмпл одного канала (0=L, 1=R). Внутри — гребёнка 8 дробных тапов
    // и демпфированная обратная связь.
    inline float process(int ch, float xin) noexcept
    {
        const int c = ch ? 1 : 0;
        push(c, xin);                                    // каскад-1 в историю

        // out = x_mid + frac·(x_mid − x_prev) — точная формула func_000397
        // ($039B macr: out = x_mid·(1+y0) − y0·x_prev + округление)
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
        res *= 0.125f;                                   // среднее тапов

        // демпфированная обратная связь (грейны hpqFb/lpqFb доказаны бит-в-бит)
        const float fb = fbState_[c] * damp_ + res * 0.5f * (fbHP_ + fbLP_);
        fbState_[c] = fb;

        const float y = xin + fb;
        push(c, y);
        return y;
    }

    int hpqIndex() const noexcept { return hpq_; }
    int lpqIndex() const noexcept { return lpq_; }

private:
    static constexpr int kHist = 1024;

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

    std::array<float, kHist> hist_[2] {};
    int   histPos_[2] {};
    float tapPos_[2] {};
    float tapStep_ = 1.0f;
    float fbState_[2] {};
    float damp_ = 0.5f, fbHP_ = 0.0078f, fbLP_ = 0.0039f;
    int   hpq_ = 0, lpq_ = 0;
};

} // namespace mnm
