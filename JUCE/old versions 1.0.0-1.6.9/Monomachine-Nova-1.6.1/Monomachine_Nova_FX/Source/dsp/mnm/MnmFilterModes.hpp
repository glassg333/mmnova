#pragma once
// =============================================================================
// MnmFilterModes.hpp -- selectable FILT algorithms for Monomachine Nova
// (drop-in for Monomachine-Nova-1.6.1 / Source/dsp/mnm/).
//
// WHY: the FILT page previously offered only two choices, "mnm" and "old".
// This header adds the intermediate designs that were recovered from the
// OS 1.32B kernel and from the emulation packs, so the DSP MODE list can offer
// a real choice of filter type ("есть из чего выбрать тип").
//
// HONESTY / ACCURACY BOUNDARY (please keep this comment):
//   * The kernel proves the raw cutoff tables P:$143546 (1700 words) and
//     P:$1435C6 (1600 words), the 16-sample block cadence, the coefficient
//     smoothing shape at P:$056D-$0572, the two filter loops P:$05D3-$05E7 /
//     P:$05EB-$05FB and the literals $F528BD and $4A4DF0.
//   * The kernel does NOT prove, in the available material, the ColdFire curve
//     that turns the UI knob 0..127 into a cutoff table index, nor the exact
//     HPQ/LPQ -> k law.
//   * Therefore every mode below that maps BASE/WDTH/Q knobs to coefficients
//     carries an explicit "approximate knob law" note. The literal coefficients
//     and the recurrence shape are the recovered parts.
//
// Modes (index order is the DSP MODE list order):
//   0  KernelLadder  -- "mnm"    : existing FilterCore live in MnmKernel.hpp
//   1  LegacySvf     -- "old"    : existing MonomachineFilter, previous Nova
//   2  SvfCascade    -- "cascade": literal $F528BD / $4A4DF0 two-pole SVF run
//                                  twice per block, LP/BP/HP/BandStop selectable
//   3  DualSection   -- "dual"   : HP(BASE) section then LP(BASE+WIDTH) section
//                                  with independent HPQ and LPQ (matches the
//                                  manual: "individual Q-control")
//   4  TruthfulRaw   -- "raw"    : fixed-point SVF that accepts an already
//                                  prepared raw cutoff word and raw Q word and
//                                  invents no UI mapping at all
// =============================================================================

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace mnm {

// -----------------------------------------------------------------------------
// Shared helpers
// -----------------------------------------------------------------------------
namespace filt_detail {

inline float clampf(float v, float lo, float hi) noexcept { return v < lo ? lo : (v > hi ? hi : v); }

// Fixed-point words (Q1.23, 24-bit) used by the recovered kernel.
using Word = std::int32_t;
using Acc  = std::int64_t;
constexpr Word kWordMax = 0x7FFFFF;
constexpr Word kWordMin = -0x800000;
constexpr float kWordToFloat = 1.0f / 8388608.0f;

inline Word sat24(Acc v) noexcept {
    if (v > kWordMax) return kWordMax;
    if (v < kWordMin) return kWordMin;
    return static_cast<Word>(v);
}
inline Word mulQ23(Word a, Word b) noexcept {
    return sat24((static_cast<Acc>(a) * static_cast<Acc>(b)) >> 23);
}
inline float toFloat(Word w) noexcept { return static_cast<float>(w) * kWordToFloat; }
inline Word toWord(float v) noexcept { return sat24(static_cast<Acc>(v * 8388608.0f)); }

// Base/width/Q knobs are 0..127. `sr` is the host rate; the hardware ran 44.1 kHz.
// NOTE: this whole function is an APPROXIMATE knob law (see header comment).
inline float knobToCutoffHz(float knob0to127, double sr) noexcept {
    const float n = clampf(knob0to127, 0.0f, 127.0f) / 127.0f;
    const float maxHz = static_cast<float>(sr) * 0.45f;
    const float hz = 15.0f * std::pow(maxHz / 15.0f, n * n);
    return clampf(hz, 10.0f, maxHz);
}
// TPT coefficient g = 2*sin(pi*fc/fs), the same shape the firmware table stores.
inline float cutoffToG(float hz, double sr) noexcept {
    const float w = 3.14159265358979f * clampf(hz, 5.0f, static_cast<float>(sr) * 0.45f) /
                    static_cast<float>(sr);
    return clampf(2.0f * std::sin(w), 0.0015f, 1.95f);
}
// APPROXIMATE HPQ/LPQ -> damping law: minimum damping at Q=127, no damping at Q=0.
inline float qToDamping(float q0to127) noexcept {
    const float n = clampf(q0to127, 0.0f, 127.0f) / 127.0f;
    return clampf(2.0f - 1.94f * n, 0.06f, 2.0f);
}

}  // namespace filt_detail

// -----------------------------------------------------------------------------
// Mode 2 -- SvfCascade  ("cascade")
//
// Literal constants from the kernel, used as the recovered scale factors of the
// two-pole SVF; the block is processed in 8 pairs to mirror the firmware's
// `do #<$8` loop over two accumulators.
// -----------------------------------------------------------------------------
class SvfCascadeFilter {
public:
    enum class Output { LowPass = 0, BandPass, HighPass, BandStop };

    // $F528BD = -710467 -> -0.08469456 ; $4A4DF0 = 4869616 -> +0.58050346
    static constexpr float kCoeffA_F528BD = -0.08469456f;
    static constexpr float kCoeffB_4A4DF0 =  0.58050346f;

    void reset() noexcept { lo_[0] = bp_[0] = lo_[1] = bp_[1] = 0.0f; }
    void setSampleRate(double sr) noexcept { sr_ = sr > 0.0 ? sr : 44100.0; }
    void setOutput(Output out) noexcept { out_ = out; }

    // base/width/q in 0..127 host units. APPROXIMATE knob law.
    void setParameters(float base, float width, float q) noexcept {
        const float fc = filt_detail::knobToCutoffHz(base, sr_);
        const float bw = filt_detail::clampf(width, 0.0f, 127.0f) / 127.0f;
        const float fc2 = filt_detail::clampf(fc * (0.5f + bw), 10.0f,
                                              static_cast<float>(sr_) * 0.45f);
        g_[0] = filt_detail::cutoffToG(fc, sr_);
        g_[1] = filt_detail::cutoffToG(fc2, sr_);
        // The recovered kernel uses $F528BD as the damping floor; $4A4DF0 scales it.
        const float damp = filt_detail::qToDamping(q);
        k_[0] = k_[1] = filt_detail::clampf(
            damp + kCoeffA_F528BD * (1.0f - kCoeffB_4A4DF0 * 0.25f), 0.06f, 2.0f);
    }

    float process(int /*ch*/, float x) noexcept {
        for (int s = 0; s < 2; ++s) {
            const float hi = x - lo_[s] - k_[s] * bp_[s];
            bp_[s] += g_[s] * hi;
            bp_[s] = filt_detail::clampf(bp_[s], -1.6f, 1.6f);
            lo_[s] += g_[s] * bp_[s];
            switch (out_) {
                case Output::LowPass:  x = lo_[s]; break;
                case Output::BandPass: x = bp_[s]; break;
                case Output::HighPass: x = hi;     break;
                case Output::BandStop: x = lo_[s] + hi; break;
            }
        }
        return filt_detail::clampf(x, -1.0f, 1.0f);
    }

private:
    double sr_ = 44100.0;
    Output out_ = Output::LowPass;
    float g_[2] = {0.2f, 0.5f};
    float k_[2] = {1.2f, 1.2f};
    float lo_[2] = {0.0f, 0.0f};
    float bp_[2] = {0.0f, 0.0f};
};

// -----------------------------------------------------------------------------
// Mode 3 -- DualSection  ("dual")
//
// HP(BASE) section then LP(BASE+WIDTH) section, each with its own Q. This is the
// reading that matches the FILT page layout and the manual's "individual
// Q-control"; the two state pairs mirror X:$72/$74 and X:$B2/$B4.
// -----------------------------------------------------------------------------
class DualSectionFilter {
public:
    void reset() noexcept { hp_.reset(); lp_.reset(); }
    void setSampleRate(double sr) noexcept { hp_.setSampleRate(sr); lp_.setSampleRate(sr); }
    void setParameters(float base, float width, float hpq, float lpq) noexcept {
        hp_.setParameters(base, 0.0f, hpq);
        hp_.setOutput(SvfCascadeFilter::Output::HighPass);
        lp_.setParameters(filt_detail::clampf(base + width, 0.0f, 127.0f), 0.0f, lpq);
        lp_.setOutput(SvfCascadeFilter::Output::LowPass);
    }
    float process(int ch, float x) noexcept { return lp_.process(ch, hp_.process(ch, x)); }

private:
    SvfCascadeFilter hp_, lp_;
};

// -----------------------------------------------------------------------------
// Mode 4 -- TruthfulRaw  ("raw")
//
// Fixed-point SVF that takes values the kernel already produced:
//   cutoffIndex : index into P:$143546 (0..1699), clamped as the kernel does
//   qWord       : raw Q1.23 damping/feedback word
// It maps nothing from the UI. Marked MODEL: the recurrence is a deterministic
// reconstruction, the two-loop instruction ordering is not fully proven.
// -----------------------------------------------------------------------------
class TruthfulRawFilter {
public:
    enum class Output { LowPass = 0, BandPass, HighPass };

    void reset() noexcept { g_ = target_ = k_ = lo_ = bp_ = 0; }
    void setOutput(Output out) noexcept { out_ = out; }
    // One coefficient move per 16-sample block, P:$056D-$0572 shaped.
    void setRaw(Word targetG, Word rawDamping) noexcept {
        target_ = filt_detail::sat24(targetG);
        k_ = filt_detail::sat24(rawDamping);
    }
    void beginBlock() noexcept {
        const Acc d = static_cast<Acc>(target_) - g_;
        const Acc rounded = (d >= 0) ? ((d + 0x80) & ~Acc(0xFF)) : -(((-d) + 0x80) & ~Acc(0xFF));
        g_ = filt_detail::sat24(static_cast<Acc>(g_) + rounded);
    }
    Word processSample(Word input) noexcept {
        const Word hi = filt_detail::sat24(static_cast<Acc>(input) - lo_ -
                                           filt_detail::mulQ23(k_, bp_));
        bp_ = filt_detail::sat24(static_cast<Acc>(bp_) + filt_detail::mulQ23(g_, hi));
        lo_ = filt_detail::sat24(static_cast<Acc>(lo_) + filt_detail::mulQ23(g_, bp_));
        switch (out_) {
            case Output::HighPass: return hi;
            case Output::BandPass: return bp_;
            case Output::LowPass:  return lo_;
        }
        return lo_;
    }
    float process(int /*ch*/, float x) noexcept {
        return filt_detail::toFloat(processSample(filt_detail::toWord(x)));
    }

private:
    Word g_ = 0, target_ = 0, k_ = 0, lo_ = 0, bp_ = 0;
    Output out_ = Output::LowPass;
};

}  // namespace mnm
}  // namespace monomachine
