#pragma once
// =============================================================================
// MnmFilterModes.hpp -- selectable FILT algorithms for Monomachine Nova
// Fixed for MSVC compatibility: fully qualified Word/Acc types, no functional cast ambiguities
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
// approx knob law helper - explicit approximate mapping


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

inline float knobToCutoffHz(float knob0to127, double sr) noexcept {
    const float n = clampf(knob0to127, 0.0f, 127.0f) / 127.0f;
    const float maxHz = static_cast<float>(sr) * 0.45f;
    const float hz = 15.0f * std::pow(maxHz / 15.0f, n * n);
    return clampf(hz, 10.0f, maxHz);
}
inline float cutoffToG(float hz, double sr) noexcept {
    const float w = 3.14159265358979f * clampf(hz, 5.0f, static_cast<float>(sr) * 0.45f) /
                    static_cast<float>(sr);
    return clampf(2.0f * std::sin(w), 0.0015f, 1.95f);
}
inline float qToDamping(float q0to127) noexcept {
    const float n = clampf(q0to127, 0.0f, 127.0f) / 127.0f;
    return clampf(2.0f - 1.94f * n, 0.06f, 2.0f);
}

}  // namespace filt_detail

// -----------------------------------------------------------------------------
// Mode 2 -- SvfCascade  ("cascade")
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

    void setParameters(float base, float width, float q) noexcept {
        const float fc = filt_detail::knobToCutoffHz(base, sr_);
        const float bw = filt_detail::clampf(width, 0.0f, 127.0f) / 127.0f;
        const float fc2 = filt_detail::clampf(fc * (0.5f + bw), 10.0f,
                                              static_cast<float>(sr_) * 0.45f);
        g_[0] = filt_detail::cutoffToG(fc, sr_);
        g_[1] = filt_detail::cutoffToG(fc2, sr_);
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
// Mode 4 -- TruthfulRaw  ("raw") - MSVC fixed
// -----------------------------------------------------------------------------
class TruthfulRawFilter {
public:
    enum class Output { LowPass = 0, BandPass, HighPass };

    void reset() noexcept { g_ = target_ = k_ = lo_ = bp_ = 0; }
    void setOutput(Output out) noexcept { out_ = out; }

    void setRaw(filt_detail::Word targetG, filt_detail::Word rawDamping) noexcept {
        target_ = filt_detail::sat24(targetG);
        k_ = filt_detail::sat24(rawDamping);
    }
    void beginBlock() noexcept {
        filt_detail::Acc d = static_cast<filt_detail::Acc>(target_) - static_cast<filt_detail::Acc>(g_);
        const filt_detail::Acc mask = static_cast<filt_detail::Acc>(0xFF);
        const filt_detail::Acc add = static_cast<filt_detail::Acc>(0x80);
        filt_detail::Acc rounded;
        if (d >= 0) {
            rounded = (d + add) & (~mask);
        } else {
            rounded = -(((-d) + add) & (~mask));
        }
        g_ = filt_detail::sat24(static_cast<filt_detail::Acc>(g_) + rounded);
    }
    filt_detail::Word processSample(filt_detail::Word input) noexcept {
        const filt_detail::Word hi = filt_detail::sat24(
            static_cast<filt_detail::Acc>(input) - static_cast<filt_detail::Acc>(lo_) -
            static_cast<filt_detail::Acc>(filt_detail::mulQ23(k_, bp_)));
        bp_ = filt_detail::sat24(static_cast<filt_detail::Acc>(bp_) + static_cast<filt_detail::Acc>(filt_detail::mulQ23(g_, hi)));
        lo_ = filt_detail::sat24(static_cast<filt_detail::Acc>(lo_) + static_cast<filt_detail::Acc>(filt_detail::mulQ23(g_, bp_)));
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
    filt_detail::Word g_ = 0;
    filt_detail::Word target_ = 0;
    filt_detail::Word k_ = 0;
    filt_detail::Word lo_ = 0;
    filt_detail::Word bp_ = 0;
    Output out_ = Output::LowPass;
};

}  // namespace mnm
}  // namespace monomachine
