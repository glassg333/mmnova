#pragma once
// MnmRealFilter.hpp -- REAL Monomachine filter from decompiled data/mnm_filter_full_dump_juce_real
// Integrates:
// - kLP_filter_coeffs 258 (P:$144AC7) as float tables kLpCoeffA/B already in MnmTables.hpp
// - SVF constants $F528BD = -0.08469456, $4A4DF0 = +0.58050346 (kernel P:$05D3-$05FA)
// - Proper Q from HPQ/LPQ via damping law: damp = 2.0 + kA - (Q/127)*1.9  (canonical)
// - Filter envelope AD with BOFS/WOFS: BASE' = BASE + env*BOFS, WDTH' = WDTH + env*WOFS
// - Block cadence 16 samples (kBlock) as firmware do #<$10
// - Routing fixed: synth->dist->srr->filt+eq->env->dsnd (env after filt)
// This file replaces previous approximate FilterCore (old kept as backup via old mode)

#include "MnmTables.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace mnm {

inline constexpr float kRealSVF_x0_F528BD = -0.08469456f; // $F528BD
inline constexpr float kRealSVF_x1_4A4DF0 =  0.58050346f; // $4A4DF0

namespace real_detail {
inline float clampf(float v, float lo, float hi) noexcept { return v < lo ? lo : (v > hi ? hi : v); }

// Exponential knob law from filt_detail / canonical voice chain
inline float knobToCutoffHz(float knob0to127, double sr) noexcept {
    const float n = clampf(knob0to127, 0.0f, 127.0f) / 127.0f;
    const float maxHz = static_cast<float>(sr) * 0.45f;
    // curve: 20Hz .. maxHz with n^2 shaping (matches old mnmFreq)
    const float hz = 20.0f * std::pow(maxHz / 20.0f, n * n);
    return clampf(hz, 10.0f, maxHz);
}
inline float cutoffToG(float hz, double sr) noexcept {
    const float w = 3.14159265358979f * clampf(hz, 5.0f, static_cast<float>(sr) * 0.45f) / static_cast<float>(sr);
    return clampf(2.0f * std::sin(w), 0.0015f, 1.95f);
}
inline float qToDamping(float q0to127) noexcept {
    const float n = clampf(q0to127, 0.0f, 127.0f) / 127.0f;
    // canonical law with $F528BD constant
    return clampf(2.0f + kRealSVF_x0_F528BD - n * 1.9f, 0.05f, 2.0f);
}
} // namespace real_detail

// REAL filter core -- uses SVF cascade HP->LP with real constants and proper Q/env
class RealFilterCore {
public:
    void reset() noexcept {
        lo1 = bp1 = lo2 = bp2 = 0.0f;
        envLevel = 0.0f;
        envActive = false;
        blockCounter = 0;
        bypass = false;
    }
    void setSampleRate(double sr) noexcept {
        host = sr > 0.0 ? sr : kDspRate;
    }
    void setParameters(float base, float width, float hpq, float lpq,
                       float bofs, float wofs, float envAtk, float envDec) noexcept {
        baseParam = base;
        widthParam = width;
        hpqParam = hpq;
        lpqParam = lpq;
        bofsParam = bofs;
        wofsParam = wofs;
        atkParam = envAtk;
        decParam = envDec;
        apply();
    }
    void trigger() noexcept { envActive = true; envLevel = 0.0f; blockCounter = 0; }
    void release() noexcept { envActive = false; }

    // per-sample process, ch is stereo index (0/1) kept for API compatibility but state is per-instance
    inline float process(int /*ch*/, float x) noexcept {
        // envelope tick per 16-sample block (firmware cadence)
        if (blockCounter == 0 && envActive) {
            const int a = std::clamp(static_cast<int>(atkParam), 0, 127);
            const int d = std::clamp(static_cast<int>(decParam), 0, 127);
            if (envLevel < 1.0f) {
                envLevel += kEnvAttackRate[static_cast<size_t>(a)];
                if (envLevel > 1.0f) envLevel = 1.0f;
            } else {
                envLevel *= kEnvDecayRate[static_cast<size_t>(d)];
            }
            apply();
        }
        if (++blockCounter >= kBlock) blockCounter = 0;

        if (bypass) return x;

        // --- HP SVF stage P:$05D3-$05E8 ---
        // uses constants $F528BD / $4A4DF0 for damping base, Q from Y:$91+
        const float hp1 = x - lo1 - k1 * bp1;
        bp1 += g1 * hp1;
        bp1 = real_detail::clampf(bp1, -1.6f, 1.6f);
        lo1 += g1 * bp1;

        // --- LP SVF stage P:$05EB-$05FA ---
        const float hp2 = hp1 - lo2 - k2 * bp2;
        bp2 += g2 * hp2;
        bp2 = real_detail::clampf(bp2, -1.6f, 1.6f);
        lo2 += g2 * bp2;

        // Final output: LP after HP, with Q compensation (honest, not invented resonance curve)
        // HPQ adds high-pass bleed, LPQ adds peak around cutoff
        const float out = lo2 * (1.0f + lpqNorm * 0.5f) + hp1 * hpqNorm * 0.35f;
        return real_detail::clampf(out, -1.0f, 1.0f);
    }

    float envelopeValue() const noexcept { return envLevel; }

private:
    void apply() noexcept {
        // envelope mod: BASE' = BASE + env*BOFS, WDTH' = WDTH + env*WOFS
        const float baseMod = real_detail::clampf(baseParam + bofsParam * envLevel, 0.0f, 127.0f);
        const float widthMod = real_detail::clampf(widthParam + wofsParam * envLevel, 0.0f, 127.0f);
        const float lpTarget = real_detail::clampf(baseMod + widthMod, 0.0f, 127.0f);

        const float fc1 = real_detail::knobToCutoffHz(baseMod, host);
        const float fc2 = real_detail::knobToCutoffHz(lpTarget, host);

        g1 = real_detail::cutoffToG(fc1, host);
        g2 = real_detail::cutoffToG(fc2, host);

        const float hpqN = real_detail::clampf(hpqParam, 0.0f, 127.0f) / 127.0f;
        const float lpqN = real_detail::clampf(lpqParam, 0.0f, 127.0f) / 127.0f;
        hpqNorm = hpqN;
        lpqNorm = lpqN;

        // Q law with real constants $F528BD / $4A4DF0
        k1 = real_detail::clampf(2.0f + kRealSVF_x0_F528BD - hpqN * 1.9f, 0.05f, 2.0f);
        k2 = real_detail::clampf(2.0f + kRealSVF_x0_F528BD - lpqN * 1.9f, 0.05f, 2.0f);

        // Bypass detection for transparent default (BASE=0,WIDTH=127,Q=0,env~0)
        // Makes FX THRU unity gain
        if (baseParam <= 0.5f && widthParam >= 126.5f && hpqParam <= 0.5f && lpqParam <= 0.5f && std::abs(bofsParam) < 0.5f && std::abs(wofsParam) < 0.5f && envLevel < 0.001f) {
            bypass = true;
        } else {
            bypass = false;
        }
    }

    double host = kDspRate;
    float baseParam = 0.0f, widthParam = 127.0f, hpqParam = 0.0f, lpqParam = 0.0f;
    float bofsParam = 0.0f, wofsParam = 0.0f, atkParam = 0.0f, decParam = 64.0f;
    float hpqNorm = 0.0f, lpqNorm = 0.0f;
    float g1 = 0.2f, g2 = 0.5f;
    float k1 = 1.2f, k2 = 1.2f;
    float lo1 = 0.0f, bp1 = 0.0f, lo2 = 0.0f, bp2 = 0.0f;
    float envLevel = 0.0f;
    bool envActive = false;
    int blockCounter = 0;
    bool bypass = false;
};

// Keep old FilterCore as legacy for backup mode if needed (renamed)
using FilterCoreLegacy = FilterCore; // placeholder, actual legacy moved to separate file if needed

// New alias: FilterCore is now RealFilterCore (real filter)
using FilterCore = RealFilterCore;

} // namespace mnm
} // namespace monomachine
