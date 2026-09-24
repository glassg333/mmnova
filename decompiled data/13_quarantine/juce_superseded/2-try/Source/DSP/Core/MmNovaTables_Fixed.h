#pragma once
#include "Dsp56300Core.h"
#include <array>
#include <cmath>
#include <algorithm>

namespace mmnova::tables {

// P:$141A80 — 24 FM ratios (0x004000..0x400000)
constexpr std::array<float,24> kFmRatioTable_P141A80 = {
  0.03125f,0.0625f,0.125f,0.1875f,0.25f,0.3125f,0.375f,0.5f,0.625f,0.75f,0.875f,1.0f,
  1.25f,1.5f,1.75f,2.0f,2.5f,3.0f,3.5f,4.0f,5.0f,6.0f,7.0f,8.0f };

// P:$144AC7 — shared 1-pole LP coeff (258 words), idx = param>>16
inline float lookupLpCoeff_P144AC7(int p7) noexcept {
    const int i = std::clamp(p7,0,127);
    const float n = (float)i/127.0f;
    return 0.00390625f * std::pow(254.0f, n);
}

// X:$140000 — exp pitch (wt[i]=0.5*2^(i/2048)) scaled by $1D22A, 11 octaves
inline double computeKernelPhaseIncrement(int32_t acc) noexcept {
    const int32_t c = std::clamp(acc,0,0x5800);
    const int oct = c >> 11, fr = c & 0x7FF;
    const double wt = 0.5 * std::exp2((double)fr/2048.0);
    constexpr double k = (double)0x1D22A / 8388608.0;
    return std::ldexp(wt*k, oct-3);
}

// Y:$141800 — shared LFO SPD + AMP rate (128 words)
inline float lookupEnvOrLfoRate_Y141800(int i7) noexcept {
    const int i = std::clamp(i7,0,127);
    return 0.000012f * std::exp2((float)i * 0.128f);
}

// SWAVE unison detune $101BFB/$101CFB/$101B7B/$101C7B (restored)
constexpr std::array<float,4> kSwaveUnisonRatios_P101BFB = { -0.0185f, +0.0192f, -0.0394f, +0.0411f };

} // namespace
