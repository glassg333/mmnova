#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace mmnova::dsp56k {

constexpr int32_t Q23_MAX =  0x7FFFFF;
constexpr int32_t Q23_MIN = -0x800000;
constexpr double  Q23_SCALE = 8388608.0;

inline int32_t floatToQ23(float x) noexcept {
    const double s = (double)x * Q23_SCALE;
    if (s >= (double)Q23_MAX) return Q23_MAX;
    if (s <= (double)Q23_MIN) return Q23_MIN;
    return (int32_t)std::llround(s);
}
inline float q23ToFloat(int32_t q) noexcept { return (float)((double)q / Q23_SCALE); }

// SR.SM saturation (bset #$14,sr): clip 56-bit accumulator to 24-bit signed
inline int32_t sat24(int64_t acc, bool srSM = true) noexcept {
    if (srSM) {
        if (acc > Q23_MAX) return Q23_MAX;
        if (acc < Q23_MIN) return Q23_MIN;
        return (int32_t)acc;
    }
    int32_t w = (int32_t)acc & 0xFFFFFF;
    if (w & 0x800000) w |= ~0xFFFFFF;
    return w;
}
inline int64_t mpy_q23(int32_t x, int32_t y) noexcept { return ((int64_t)x * (int64_t)y) >> 23; }
inline int64_t mac_q23(int64_t a, int32_t x, int32_t y) noexcept { return a + (((int64_t)x*(int64_t)y)>>23); }

// Linear table interp (mpysu -x1,y0,a / add / macsu / asr pattern)
inline float interpTableQ23(const float* t, int mask, uint32_t phase24) noexcept {
    const int i0 = (int)((phase24 >> 11) & (uint32_t)mask);
    const int i1 = (i0 + 1) & mask;
    const float f = (float)(phase24 & 0x7FFu) * (1.0f/2048.0f);
    return t[i0] + f * (t[i1] - t[i0]);
}

// FIX (audit 5.1): unified external SRAM, never subtract 0x100000 from flat P dump
constexpr uint32_t resolveUnifiedSramWordOffset(uint32_t a) noexcept { return a; }

} // namespace
