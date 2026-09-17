#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace mmnova {

using s24 = int32_t;
using s56 = int64_t;

static constexpr s24 kOne24 =  0x7FFFFF;
static constexpr s24 kMin24 = -0x800000;
static constexpr double kInv24 = 1.0 / 8388608.0;

inline s24 sext24 (int32_t v) noexcept {
    v &= 0xFFFFFF;
    return (v & 0x800000) ? (v | ~0xFFFFFF) : v;
}

// bset #$14,sr -> SR.SM. Жёсткий клип 24-битного аккумулятора.
// Это единственный источник искажения в Monomachine: отдельной
// машины дисторшна в OS 1.32B нет.
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

inline s24    paramWord  (int v7) noexcept { return (s24) (std::clamp (v7,0,127) << 16); }
inline int    paramIndex (s24 w)  noexcept { return (int) ((w >> 16) & 0x7F); }
inline double paramNorm  (s24 w)  noexcept { return paramIndex (w) / 127.0; }

} // namespace mmnova
