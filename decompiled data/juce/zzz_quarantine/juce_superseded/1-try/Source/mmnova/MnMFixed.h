// ============================================================================
//  MnMFixed.h — арифметика DSP56300 (24-бит дробные, 56-бит аккумулятор)
//  Monomachine SFX-60 OS 1.32B port. Семантика проверена по листингам
//  dsp1_pmem (dis56300, 22836/22836 инструкций).
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <array>
#include <cstdlib>
#include <algorithm>

namespace mmnova {

using s24 = int32_t;   // знаковое 24-битное слово в младших битах int32
using s56 = int64_t;   // аккумулятор A/B

static constexpr s24 kOne24  = 0x7FFFFF;   // +1.0 - 1 LSB
static constexpr s24 kMin24  = -0x800000;  // -1.0
static constexpr double kInv24 = 1.0 / 8388608.0;

// --- знаковое расширение 24 -> 32 -------------------------------------------
inline s24 sext24 (int32_t v) noexcept
{
    v &= 0xFFFFFF;
    return (v & 0x800000) ? (v | ~0xFFFFFF) : v;
}

// --- сатурация аккумулятора (эквивалент bset #$14,sr : SR.SM) ---------------
inline s24 sat24 (s56 a) noexcept
{
    if (a >  kOne24) return kOne24;
    if (a <  kMin24) return kMin24;
    return (s24) a;
}

// --- mpy x,y,a : a(56) = 2 * x * y ------------------------------------------
inline s56 mpy (s24 x, s24 y) noexcept { return ((s56) x * (s56) y) << 1; }

// --- mac : a += 2*x*y --------------------------------------------------------
inline void mac (s56& a, s24 x, s24 y) noexcept { a += mpy (x, y); }

// --- A1 : верхнее 24-битное слово 48-битного произведения -------------------
inline s24 hi24 (s56 a) noexcept { return sat24 (a >> 24); }

// --- дробное умножение слово×слово -> слово (самый частый паттерн) ----------
inline s24 fmul (s24 x, s24 y) noexcept { return sat24 ((((s56) x * (s56) y) >> 23)); }

// --- mpysu / macsu : беззнаковая младшая часть (интерполяция таблиц) --------
inline s56 mpysu (s24 sx, uint32_t uy) noexcept { return ((s56) sx * (s56) uy) << 1; }

// --- конвертация -------------------------------------------------------------
inline float  toFloat (s24 v)  noexcept { return (float) (v * kInv24); }
inline s24    fromFloat (double f) noexcept
{
    double s = f * 8388608.0;
    if (s >  8388607.0) s =  8388607.0;
    if (s < -8388608.0) s = -8388608.0;
    return (s24) std::lrint (s);
}

// --- параметр патча 0..127 в том виде, как его кладёт ColdFire: param << 16 --
inline s24 paramWord (int v7)      noexcept { return (s24) ((v7 & 0x7F) << 16); }
// обратная операция asr #$10 — индексация таблиц параметром
inline int paramIndex (s24 word)   noexcept { return (int) ((word >> 16) & 0x7F); }
inline double paramNorm (s24 word) noexcept { return paramIndex (word) / 127.0; }

// --- линейная интерполяция таблицы по 48-битной фазе ------------------------
//  Паттерн прошивки: idx = phase >> (48-13) & $1FFF ; frac = следующие 16 бит.
template <int kBits>
inline s24 lerpTable (const s24* tab, uint64_t phase48) noexcept
{
    constexpr uint32_t kMask = (1u << kBits) - 1u;
    const uint32_t idx  = (uint32_t) ((phase48 >> (48 - kBits)) & kMask);
    const uint32_t frac = (uint32_t) ((phase48 >> (48 - kBits - 16)) & 0xFFFF);
    const s24 a = tab[idx];
    const s24 b = tab[(idx + 1) & kMask];
    return (s24) (a + (((s56) (b - a) * (s56) frac) >> 16));
}

// --- однополюсный LP по коэффициенту из таблицы $144AC7 ---------------------
struct OnePoleLP
{
    s56 z { 0 };
    inline s24 process (s24 in, s24 coeff) noexcept
    {
        // y += coeff * (x - y)   (в прошивке: mpy + add с сатурацией)
        const s24 y = sat24 (z >> 24);
        z += mpy (coeff, sat24 (in - y));
        return sat24 (z >> 24);
    }
    inline void reset() noexcept { z = 0; }
};

} // namespace mmnova
