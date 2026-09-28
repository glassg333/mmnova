/*
  Reference classic-filter bridge for Monomachine Nova.

  Adapted from the user-supplied JUCE/for import sources at:
  glassg333/mmnova commit 3791a32187b5dcffb0efb6832156a688ab93ece7

    TB303.{h,cpp}  -- "Based off Saikes Yutani bass filters 303"
    MS20.{h,cpp}   -- "Based off Saikes Yutani bass filters MS-20"
    Moog.{h,cpp}   -- adapted JUCE Ladder filter

  The supplied files depend on an absent Filter.h/Lerp/tanhLUT host layer.
  This header keeps their filter equations and mode laws, replaces only those
  host utilities with allocation-free C++17 equivalents, and adds finite
  guards appropriate for a real-time stereo plug-in. It does not claim to be
  an exact model of the named hardware.
*/
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace nova::hybrid_private::reference_filters {

inline constexpr double kPi = 3.1415926535897932384626433832795;
inline constexpr double kMinCutoffHz = 20.0;

inline bool finite(double value) noexcept { return std::isfinite(value); }
inline double safeFinite(double value, double fallback = 0.0) noexcept
{
    return finite(value) ? value : fallback;
}

inline double clampCutoff(double cutoff, double sampleRate) noexcept
{
    const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
    return std::clamp(finite(cutoff) ? cutoff : kMinCutoffHz,
                      kMinCutoffHz, rate * 0.45);
}

inline double clampResonance(double resonance) noexcept
{
    // The supplied source treats q as a normalized 0..1 control.  Keep a tiny
    // margin below one in the reference bridges so nonlinear state equations
    // remain finite at an automated endpoint.
    return std::clamp(finite(resonance) ? resonance : 0.0, 0.0, 0.985);
}

inline double softFinite(double value) noexcept
{
    if (! finite(value)) return 0.0;
    constexpr double knee = 12.0;
    const double magnitude = std::abs(value);
    if (magnitude <= knee) return value;
    return std::copysign(knee + (magnitude - knee) / (1.0 + magnitude - knee), value);
}

inline double safeTanh(double value) noexcept
{
    // std::tanh itself is finite for all finite doubles. Clamping first avoids
    // preserving an accidental infinity from a hostile input/control state.
    return std::tanh(std::clamp(safeFinite(value), -32.0, 32.0));
}

enum class Response : int { LowPass, BandPass, HighPass };
enum class MoogPoles : int { Twelve, TwentyFour };

// -----------------------------------------------------------------------------
// R 303 — direct adaptation of the supplied TB303 core.  The source's
// coeffLUT() is a pre-warp lookup; std::tan is used here as the exact analytic
// replacement.  Control smoothing is supplied by ReferenceStereo (16 samples)
// rather than the source's unavailable Lerp helper.
// -----------------------------------------------------------------------------
class TB303Core final {
public:
    void configure(double sampleRate, double cutoff, double resonance, Response response) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        response_ = response;
        const double frequency = clampCutoff(cutoff, sampleRate_);
        // TB303.cpp feeds coeffLUT(0.45*pi*f/fs*0.5). The LUT's host-side
        // scaling is absent from the supplied package; its equivalent
        // pre-warp must be five times that input for the named cutoff to stay
        // at the physical BASE/WDTH edge rather than about an octave-and-a-
        // half below it. Keep the source's nonlinear state equations intact.
        const double angle = std::clamp(1.125 * kPi * frequency / sampleRate_, 0.0, 1.35);
        wc1_ = std::max(1.0e-8, std::tan(angle));
        if (! finite(wc1_)) wc1_ = 0.1;
        wc2_ = wc1_ * wc1_;
        wc3_ = wc2_ * wc1_;
        wc4_ = wc3_ * wc1_;
        b_ = 1.0 / std::max(1.0e-12, 1.0 + 8.0 * wc1_ + 20.0 * wc2_
                                           + 16.0 * wc3_ + 2.0 * wc4_);
        g_ = 2.0 * wc4_ * b_;
        k_ = 16.95 * clampResonance(resonance);
        aGain_ = 1.0 + 0.5 * k_;

        const double dwc = 2.0 * wc1_;
        const double dwc2 = 2.0 * wc2_;
        const double qwc2 = 4.0 * wc2_;
        const double dwc3 = 2.0 * wc3_;
        const double qwc3 = 4.0 * wc3_;

        b0_ = dwc + 12.0 * wc2_ + 20.0 * wc3_ + 8.0 * wc4_;
        a0_ = 1.0 + 6.0 * wc1_ + 10.0 * wc2_ + qwc3;
        a1_ = dwc + 8.0 * wc2_ + 6.0 * wc3_;
        a2_ = dwc2 + wc3_;
        a3_ = dwc3;

        b10_ = dwc2 + 8.0 * wc3_ + 6.0 * wc4_;
        a10_ = wc1_ + 4.0 * wc2_ + 3.0 * wc3_;
        a11_ = 1.0 + 6.0 * wc1_ + 11.0 * wc2_ + 6.0 * wc3_;
        a12_ = wc1_ + qwc2 + qwc3;
        a13_ = wc2_ + dwc3;

        b20_ = dwc3 + 4.0 * wc4_;
        a20_ = a13_;
        a21_ = wc1_ + qwc2 + 4.0 * wc3_;
        a22_ = 1.0 + 6.0 * wc1_ + 10.0 * wc2_ + qwc3;
        a23_ = wc1_ + qwc2 + dwc3;
        c2_ = a21_ - a3_;
        c3_ = 1.0 + 6.0 * wc1_ + 9.0 * wc2_ + dwc3;
    }

    void reset(double sample = 0.0) noexcept
    {
        z0_ = z1_ = z2_ = z3_ = 0.0;
        y1_ = y2_ = y3_ = y4_ = safeFinite(sample);
    }

    double process(double sample) noexcept
    {
        sample = std::clamp(safeFinite(sample), -32.0, 32.0);
        const double denominator = 1.0 + g_ * k_;
        if (! finite(denominator) || std::abs(denominator) < 1.0e-12) {
            reset();
            return 0.0;
        }

        const double s = (z0_ * wc3_ + z1_ * a20_ + z2_ * c2_ + z3_ * c3_) * b_;
        y4_ = (g_ * sample + s) / denominator;
        const double feedback = sample - k_ * y4_;
        const double y0 = std::clamp(feedback, -1.0, 1.0);

        y1_ = b_ * (y0 * b0_ + z0_ * a0_ + z1_ * a1_ + z2_ * a2_ + z3_ * a3_);
        y2_ = b_ * (y0 * b10_ + z0_ * a10_ + z1_ * a11_ + z2_ * a12_ + z3_ * a13_);
        y3_ = b_ * (y0 * b20_ + z0_ * a20_ + z1_ * a21_ + z2_ * a22_ + z3_ * a23_);
        y4_ = g_ * y0 + s;

        z0_ += 4.0 * wc1_ * (y0 - y1_ + y2_);
        z1_ += 2.0 * wc1_ * (y1_ - 2.0 * y2_ + y3_);
        z2_ += 2.0 * wc1_ * (y2_ - 2.0 * y3_ + y4_);
        z3_ += 2.0 * wc1_ * (y3_ - 2.0 * y4_);

        if (! finite(z0_) || ! finite(z1_) || ! finite(z2_) || ! finite(z3_)
            || ! finite(y1_) || ! finite(y2_) || ! finite(y3_) || ! finite(y4_)) {
            reset();
            return 0.0;
        }

        double output = 0.0;
        switch (response_) {
            case Response::LowPass:  output = aGain_ * y4_; break;
            case Response::BandPass: output = y4_ + y2_ - y1_; break;
            case Response::HighPass: output = -0.5 * (y0 - y4_); break;
        }
        return softFinite(output);
    }

private:
    double sampleRate_ = 44100.0;
    Response response_ = Response::LowPass;
    double wc1_ = 0.1, wc2_ = 0.01, wc3_ = 0.001, wc4_ = 0.0001;
    double aGain_ = 1.0, b_ = 1.0, g_ = 0.0, k_ = 0.0;
    double z0_ = 0.0, z1_ = 0.0, z2_ = 0.0, z3_ = 0.0;
    double y1_ = 0.0, y2_ = 0.0, y3_ = 0.0, y4_ = 0.0;
    double b0_ = 0.0, a0_ = 0.0, a1_ = 0.0, a2_ = 0.0, a3_ = 0.0;
    double b10_ = 0.0, a10_ = 0.0, a11_ = 0.0, a12_ = 0.0, a13_ = 0.0;
    double b20_ = 0.0, a20_ = 0.0, a21_ = 0.0, a22_ = 0.0, a23_ = 0.0;
    double c2_ = 0.0, c3_ = 0.0;
};

// -----------------------------------------------------------------------------
// R MS20 — supplied nonlinear two-stage equations, with a bounded Newton
// solve. getCoeff()/tanhLUT() from the missing host layer become a pre-warped
// TPT coefficient and std::tanh.  A singular/non-finite solve clears only this
// filter instance rather than contaminating an audio buffer with NaN.
// -----------------------------------------------------------------------------
class MS20Core final {
public:
    void configure(double sampleRate, double cutoff, double resonance, Response response) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        response_ = response;
        const double frequency = clampCutoff(cutoff, sampleRate_);
        const double angle = std::clamp(kPi * frequency / sampleRate_, 0.0, 1.325);
        h_ = std::clamp(std::tan(angle), 1.0e-8, 4.0);
        k_ = 2.0 * clampResonance(resonance) * 0.98;
    }

    void reset(double sample = 0.0) noexcept
    {
        const double seed = safeFinite(sample);
        y1_ = y2_ = d1_ = d2_ = seed;
    }

    double process(double sample) noexcept
    {
        sample = std::clamp(safeFinite(sample), -16.0, 16.0);
        double output = 0.0;
        switch (response_) {
            case Response::LowPass:  output = evalLowPass(sample); break;
            case Response::BandPass: output = evalBandPass(sample); break;
            case Response::HighPass: output = evalHighPass(sample); break;
        }
        if (! finite(output)) {
            reset();
            return 0.0;
        }
        return softFinite(output);
    }

private:
    static constexpr int kMaxIterations = 6;
    static constexpr double kEpsilon = 1.0e-8;

    bool solve(double& next1, double& next2, double f1, double f2,
               double a, double b, double c, double d) noexcept
    {
        const double determinant = a * d - b * c;
        if (! finite(determinant) || std::abs(determinant) < 1.0e-12)
            return false;
        next1 -= (d * f1 - b * f2) / determinant;
        next2 -= (a * f2 - c * f1) / determinant;
        return finite(next1) && finite(next2);
    }

    double evalLowPass(double sample) noexcept
    {
        const double gd2k = std::clamp(d2_ * k_, -1.0, 1.0);
        const double tanhterm1 = safeTanh(-d1_ + sample - gd2k);
        const double tanhterm2 = safeTanh(d1_ - d2_ + gd2k);
        for (int iter = 0; iter < kMaxIterations; ++iter) {
            const double ky2 = k_ * y2_;
            const double gky2 = std::clamp(ky2, -1.0, 1.0);
            const double dgky2 = std::abs(ky2) > 1.0 ? 0.0 : 1.0;
            const double sig1 = sample - y1_ - gky2;
            const double thsig1 = safeTanh(sig1);
            const double sig2 = y1_ - y2_ + gky2;
            const double thsig2 = safeTanh(sig2);
            const double hs1 = h_ * (thsig1 * thsig1 - 1.0);
            const double hs2 = h_ * (thsig2 * thsig2 - 1.0);
            const double f1 = y1_ - d1_ - h_ * (tanhterm1 + thsig1);
            const double f2 = y2_ - d2_ - h_ * (tanhterm2 + thsig2);
            const double residual = std::abs(f1) + std::abs(f2);
            const double a = 1.0 - hs1;
            const double b = -k_ * hs1 * dgky2;
            const double c = hs2;
            const double d = (k_ * dgky2 - 1.0) * hs2 + 1.0;
            if (! solve(y1_, y2_, f1, f2, a, b, c, d)) return 0.0;
            if (residual <= kEpsilon) break;
        }
        d1_ = y1_; d2_ = y2_;
        return y2_;
    }

    double evalBandPass(double sample) noexcept
    {
        const double gd2k = std::clamp(d2_ * k_, -1.0, 1.0);
        const double tanhterm1 = safeTanh(-d1_ - sample - gd2k);
        const double tanhterm2 = safeTanh(d1_ - d2_ + sample + gd2k);
        for (int iter = 0; iter < kMaxIterations; ++iter) {
            const double ky2 = k_ * y2_;
            const double gky2 = std::clamp(ky2, -1.0, 1.0);
            const double dgky2 = std::abs(ky2) > 1.0 ? 0.0 : 1.0;
            const double sig1 = -sample - y1_ - gky2;
            const double thsig1 = safeTanh(sig1);
            const double sig2 = sample + y1_ - y2_ + gky2;
            const double thsig2 = safeTanh(sig2);
            const double hs1 = h_ * (thsig1 * thsig1 - 1.0);
            const double hs2 = h_ * (thsig2 * thsig2 - 1.0);
            const double f1 = y1_ - d1_ - h_ * (tanhterm1 + thsig1);
            const double f2 = y2_ - d2_ - h_ * (tanhterm2 + thsig2);
            const double residual = std::abs(f1) + std::abs(f2);
            const double a = 1.0 - hs1;
            const double b = -k_ * hs1 * dgky2;
            const double c = hs2;
            const double d = (k_ * dgky2 - 1.0) * hs2 + 1.0;
            if (! solve(y1_, y2_, f1, f2, a, b, c, d)) return 0.0;
            if (residual <= kEpsilon) break;
        }
        d1_ = y1_; d2_ = y2_;
        return y2_;
    }

    double evalHighPass(double sample) noexcept
    {
        const double kc = k_ * 0.9;
        const double gkd2px = std::clamp(kc * (d2_ + sample), -1.0, 1.0);
        const double tanhterm1 = safeTanh(-d1_ - gkd2px);
        const double tanhterm2 = safeTanh(d1_ - d2_ - sample + gkd2px);
        for (int iter = 0; iter < kMaxIterations; ++iter) {
            const double kxpy2 = kc * (sample + y2_);
            const double gkxpy2 = std::clamp(kxpy2, -1.0, 1.0);
            const double dgkxpy2 = std::abs(kxpy2) > 1.0 ? 0.0 : 1.0;
            const double sig1 = -y1_ - gkxpy2;
            const double thsig1 = safeTanh(sig1);
            const double sig2 = -sample + y1_ - y2_ + gkxpy2;
            const double thsig2 = safeTanh(sig2);
            const double hs1 = thsig1 * thsig1 - 1.0;
            const double hs2 = thsig2 * thsig2 - 1.0;
            const double f1 = y1_ - d1_ - h_ * (tanhterm1 + thsig1);
            const double f2 = y2_ - d2_ - h_ * (tanhterm2 + thsig2);
            const double residual = std::abs(f1) + std::abs(f2);
            const double a = 1.0 - hs1;
            const double b = -kc * hs1 * dgkxpy2;
            const double c = hs2;
            const double d = (kc * dgkxpy2 - 1.0) * hs2 + 1.0;
            if (! solve(y1_, y2_, f1, f2, a, b, c, d)) return 0.0;
            if (residual <= kEpsilon) break;
        }
        d1_ = y1_; d2_ = y2_;
        return y2_ + sample;
    }

    double sampleRate_ = 44100.0;
    Response response_ = Response::LowPass;
    double h_ = 0.1, k_ = 0.0;
    double y1_ = 0.0, y2_ = 0.0, d1_ = 0.0, d2_ = 0.0;
};

// -----------------------------------------------------------------------------
// R MOOG — the supplied JUCE-ladder adaptation.  The original state/output
// equations and 12/24 dB response laws are retained; only jmap/tanhLUT and
// Lerp are replaced by direct, finite control values from ReferenceStereo.
// -----------------------------------------------------------------------------
class MoogCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance,
                   Response response, MoogPoles poles) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        response_ = response;
        poles_ = poles;
        const double frequency = clampCutoff(cutoff, sampleRate_);
        f0_ = std::exp(-2.0 * kPi * frequency / sampleRate_);
        if (! finite(f0_)) f0_ = 0.5;
        k_ = 0.1 + 0.9 * clampResonance(resonance);
        updateState();
    }

    void reset(double sample = 0.0) noexcept
    {
        states_.fill(safeFinite(sample));
    }

    double process(double sample) noexcept
    {
        sample = std::clamp(safeFinite(sample), -32.0, 32.0);
        const double f = f0_;
        const double g = 1.0 - f;
        const double b0 = g * 0.76923076923;
        const double b1 = g * 0.23076923076;
        const double dx = safeTanh(sample);
        const double a = dx - 4.0 * k_ * (safeTanh(states_[4]) - dx * compensation_);
        const double b = b1 * states_[0] + f * states_[1] + b0 * a;
        const double c = b1 * states_[1] + f * states_[2] + b0 * b;
        const double d = b1 * states_[2] + f * states_[3] + b0 * c;
        const double e = b1 * states_[3] + f * states_[4] + b0 * d;
        states_[0] = a; states_[1] = b; states_[2] = c; states_[3] = d; states_[4] = e;
        for (const auto state : states_) {
            if (! finite(state)) { reset(); return 0.0; }
        }
        double output = 0.0;
        if (response_ == Response::BandPass)
            output = poles_ == MoogPoles::Twelve ? b - c : c - 2.0 * d + e;
        else if (response_ == Response::HighPass)
            output = poles_ == MoogPoles::Twelve ? a - 2.0 * b + c
                                                 : a - 4.0 * b + 6.0 * c - 4.0 * d + e;
        else
            output = poles_ == MoogPoles::Twelve ? c : e;
        return softFinite(1.2 * output);
    }

private:
    void updateState() noexcept
    {
        if (poles_ == MoogPoles::Twelve) {
            compensation_ = response_ == Response::LowPass || response_ == Response::BandPass ? 0.5 : 0.0;
        } else {
            compensation_ = response_ == Response::LowPass || response_ == Response::BandPass ? 0.5 : 0.0;
        }
    }

    double sampleRate_ = 44100.0;
    Response response_ = Response::LowPass;
    MoogPoles poles_ = MoogPoles::TwentyFour;
    double f0_ = 0.5, k_ = 0.1, compensation_ = 0.0;
    std::array<double, 5> states_{};
};

enum class ReferenceMode : int {
    TB303LP, TB303BP, TB303HP,
    MS20LP, MS20BP, MS20HP,
    MoogLP24, MoogLP12, MoogBP24, MoogBP12, MoogHP24, MoogHP12
};

class ReferenceCore final {
public:
    void prepare(double sampleRate) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        reset();
    }

    void reset() noexcept
    {
        tb303_.reset();
        ms20_.reset();
        moog_.reset();
    }

    void configure(ReferenceMode mode, double cutoff, double resonance) noexcept
    {
        if (mode != mode_) {
            mode_ = mode;
            reset();
        }
        const auto response = responseFor(mode_);
        switch (mode_) {
            case ReferenceMode::TB303LP:
            case ReferenceMode::TB303BP:
            case ReferenceMode::TB303HP:
                tb303_.configure(sampleRate_, cutoff, resonance, response);
                break;
            case ReferenceMode::MS20LP:
            case ReferenceMode::MS20BP:
            case ReferenceMode::MS20HP:
                ms20_.configure(sampleRate_, cutoff, resonance, response);
                break;
            default:
                moog_.configure(sampleRate_, cutoff, resonance, response, polesFor(mode_));
                break;
        }
    }

    float process(float input) noexcept
    {
        const double x = static_cast<double>(input);
        double y = 0.0;
        switch (mode_) {
            case ReferenceMode::TB303LP:
            case ReferenceMode::TB303BP:
            case ReferenceMode::TB303HP: y = tb303_.process(x); break;
            case ReferenceMode::MS20LP:
            case ReferenceMode::MS20BP:
            case ReferenceMode::MS20HP:  y = ms20_.process(x); break;
            default: y = moog_.process(x); break;
        }
        return static_cast<float>(softFinite(y));
    }

private:
    static Response responseFor(ReferenceMode mode) noexcept
    {
        switch (mode) {
            case ReferenceMode::TB303BP:
            case ReferenceMode::MS20BP:
            case ReferenceMode::MoogBP24:
            case ReferenceMode::MoogBP12: return Response::BandPass;
            case ReferenceMode::TB303HP:
            case ReferenceMode::MS20HP:
            case ReferenceMode::MoogHP24:
            case ReferenceMode::MoogHP12: return Response::HighPass;
            default: return Response::LowPass;
        }
    }

    static MoogPoles polesFor(ReferenceMode mode) noexcept
    {
        switch (mode) {
            case ReferenceMode::MoogLP12:
            case ReferenceMode::MoogBP12:
            case ReferenceMode::MoogHP12: return MoogPoles::Twelve;
            default: return MoogPoles::TwentyFour;
        }
    }

    double sampleRate_ = 44100.0;
    ReferenceMode mode_ = ReferenceMode::TB303LP;
    TB303Core tb303_;
    MS20Core ms20_;
    MoogCore moog_;
};

} // namespace nova::hybrid_private::reference_filters
