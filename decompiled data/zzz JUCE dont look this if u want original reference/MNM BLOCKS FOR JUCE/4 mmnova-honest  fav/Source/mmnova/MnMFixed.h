#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

// Арифметика DSP56303. Семантика взята из опкодов, которые я видел
// (mpy/mac/mpysu/dmac/macsu/asr в chorus, GND-SIN, FM, SWAVE).
namespace mmnova {

using s24 = int32_t;   // 24-битное дробное слово Q23
using s56 = int64_t;   // аккумулятор A/B

static constexpr s24 kOne24 =  0x7FFFFF;
static constexpr s24 kMin24 = -0x800000;
static constexpr double kInv24 = 1.0 / 8388608.0;

inline s24 sext24 (int32_t v) noexcept {
    v &= 0xFFFFFF;
    return (v & 0x800000) ? (v | ~0xFFFFFF) : v;
}

// bset #$14,sr  -> SR.SM. Это и есть "дисторшн" Monomachine:
// отдельной машины дисторшна в OS 1.32B нет, клип делает ALU.
inline s24 sat24 (s56 a) noexcept {
    if (a > kOne24) return kOne24;
    if (a < kMin24) return kMin24;
    return (s24) a;
}

inline s56 mpy  (s24 x, s24 y) noexcept { return ((s56) x * (s56) y) << 1; }
inline s24 fmul (s24 x, s24 y) noexcept { return sat24 (((s56) x * (s56) y) >> 23); }

inline double toFloat (s24 x) noexcept { return (double) x * kInv24; }
inline s24 fromFloat (double x) noexcept {
    double s = x * 8388608.0;
    if (s >  8388607.0) s =  8388607.0;
    if (s < -8388608.0) s = -8388608.0;
    return (s24) std::llround (s);
}

// Параметры патча лежат в Y:(R6+$4..$B) как (val7 << 16)
inline s24    paramWord  (int v7)   noexcept { return (s24) (std::clamp (v7,0,127) << 16); }
inline int    paramIndex (s24 w)    noexcept { return (int) ((w >> 16) & 0x7F); }
inline double paramNorm  (s24 w)    noexcept { return paramIndex (w) / 127.0; }

// Паттерн интерполяции таблицы: mpysu -x1,y0,a / add x1,a / macsu x0,y0,a / asr a
template <int kBits = 13>
inline s24 lerpTable (const s24* t, uint64_t phase48) noexcept {
    constexpr uint32_t kMask = (1u << kBits) - 1u;
    const uint32_t i = (uint32_t) ((phase48 >> (48 - kBits)) & kMask);
    const uint32_t f = (uint32_t) ((phase48 >> (48 - kBits - 16)) & 0xFFFFu);
    const s24 a = t[i], b = t[(i + 1u) & kMask];
    return (s24) (a + (((s56)(b - a) * (s56) f) >> 16));
}

// Однополюсный LP по коэффициенту из P:$144AC7
struct OnePoleLP {
    s56 z { 0 };
    inline s24 process (s24 in, s24 coeff) noexcept {
        const s24 y = sat24 (z >> 24);
        z += mpy (coeff, sat24 ((s56) in - y));
        return sat24 (z >> 24);
    }
    inline void reset() noexcept { z = 0; }
};

} // namespace mmnova
