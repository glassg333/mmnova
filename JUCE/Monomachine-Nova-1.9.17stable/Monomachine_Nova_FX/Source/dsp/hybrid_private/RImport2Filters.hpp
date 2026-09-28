/*
  R Import 2 -- self-contained independent filter-family equivalents.

  Audit source: glassg333/mmnova commit a08334d586ac972c9ac932461ef3631924d31af9,
  JUCE/for import/2.  That directory mixes usable model descriptions with
  unavailable host framework types (Filter.h, OnePole.h, TptFilter.h, JUCE
  helpers) and mixed / incomplete licensing provenance.  The user selected
  independent equivalents rather than direct incorporation of that source.

  Consequently this header does NOT include, copy, or require that source
  directory.  It provides allocation-free C++17 implementations that retain
  the named topology families and physical LP/BP/HP meanings, but it makes no
  bit-identical or plug-in-interoperability claim for the upstream code.

  Named conceptual references:
  - Analog / Linear / RBJ / TPT: independently implemented Sallen-Key/SVF,
    serial-SVF, cookbook-biquad and topology-preserving SVF families.
  - Huovilainen, Hyperion, Krajeski, Microtracker, MusicDSP, Oberheim and
    D'Angelo-Valimaki: independently bounded ladder-family equivalents based
    on the descriptions and equations supplied in the audited folder.

  Real-time rules: no allocation, no exceptions, finite guards on every
  stateful family, and prepare/reset only under the owning stereo adapter.
*/
#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace nova::hybrid_private::reference_import2 {

inline constexpr double kPi = 3.141592653589793238462643383279502884;
inline constexpr double kMinCutoff = 20.0;
inline constexpr double kMaxCutoff = 20000.0;

inline bool finite(double value) noexcept { return std::isfinite(value); }
inline double finiteOr(double value, double fallback = 0.0) noexcept
{
    return finite(value) ? value : fallback;
}
inline double clampInput(double value) noexcept
{
    return std::clamp(finiteOr(value), -32.0, 32.0);
}
inline double clampCutoff(double frequency, double sampleRate) noexcept
{
    const double safeRate = sampleRate > 1000.0 ? sampleRate : 44100.0;
    const double upper = std::min(kMaxCutoff, safeRate * 0.45);
    return std::clamp(finiteOr(frequency, 1000.0), kMinCutoff, std::max(kMinCutoff, upper));
}
inline double safeTanh(double value) noexcept
{
    return std::tanh(std::clamp(finiteOr(value), -20.0, 20.0));
}
inline double safeOutput(double value) noexcept
{
    // The surrounding Monomachine FilterCore applies its own final limiter.
    // This bound protects an imported-family state from contaminating it first.
    return finite(value) ? std::clamp(value, -32.0, 32.0) : 0.0;
}
inline double qFromNorm(double resonance, double low = 0.55, double high = 12.0) noexcept
{
    return std::clamp(low + std::clamp(finiteOr(resonance), 0.0, 1.0) * high, 0.05, 24.0);
}

enum class Response : int { LowPass, BandPass, HighPass, Notch };

struct SvfOutputs {
    double low = 0.0;
    double band = 0.0;
    double high = 0.0;
};

inline double responseValue(const SvfOutputs& values, Response response) noexcept
{
    switch (response) {
        case Response::BandPass: return values.band;
        case Response::HighPass: return values.high;
        case Response::Notch:    return values.low + values.high;
        case Response::LowPass:
        default:                 return values.low;
    }
}

// A bounded, topology-preserving two-integrator state-variable stage.  The
// equations are independently written from the standard TPT/SVF derivation.
class TptSvfStage final {
public:
    void configure(double sampleRate, double cutoff, double q) noexcept
    {
        const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
        const double frequency = clampCutoff(cutoff, rate);
        const double angle = std::clamp(kPi * frequency / rate, 0.0, 1.40);
        g_ = std::clamp(std::tan(angle), 1.0e-8, 8.0);
        damping_ = 1.0 / std::max(0.05, finiteOr(q, 0.707));
        const double denom = std::max(1.0e-12, 1.0 + g_ * (g_ + damping_));
        a1_ = 1.0 / denom;
        a2_ = g_ * a1_;
        a3_ = g_ * a2_;
    }

    void reset(double seed = 0.0) noexcept
    {
        ic1_ = ic2_ = finiteOr(seed);
    }

    SvfOutputs process(double input) noexcept
    {
        input = clampInput(input);
        const double v3 = input - ic2_;
        const double v1 = a1_ * ic1_ + a2_ * v3;
        const double v2 = ic2_ + a2_ * ic1_ + a3_ * v3;
        const double next1 = 2.0 * v1 - ic1_;
        const double next2 = 2.0 * v2 - ic2_;
        if (!finite(v1) || !finite(v2) || !finite(next1) || !finite(next2)) {
            reset();
            return {};
        }
        ic1_ = std::clamp(next1, -64.0, 64.0);
        ic2_ = std::clamp(next2, -64.0, 64.0);
        return { safeOutput(v2), safeOutput(v1), safeOutput(v3 - damping_ * v1) };
    }

private:
    double g_ = 0.1, damping_ = 1.0;
    double a1_ = 1.0, a2_ = 0.0, a3_ = 0.0;
    double ic1_ = 0.0, ic2_ = 0.0;
};

// R ANALOG: nonlinear Sallen-Key / serial-SVF family equivalent.  A soft
// nonlinear input stage and resonant feedback distinguish it from R LINEAR.
class AnalogCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance, Response response, bool fourPole) noexcept
    {
        response_ = response;
        fourPole_ = fourPole;
        const double q = qFromNorm(resonance, 0.62, fourPole ? 2.6 : 4.0);
        first_.configure(sampleRate, cutoff, q);
        second_.configure(sampleRate, cutoff, q);
        feedback_ = std::clamp(finiteOr(resonance), 0.0, 1.0) * (fourPole ? 0.015 : 0.010);
        drive_ = 1.0 + 2.4 * std::clamp(finiteOr(resonance), 0.0, 1.0);
    }

    void reset() noexcept
    {
        first_.reset(); second_.reset(); feedbackState_ = 0.0;
    }

    double process(double input) noexcept
    {
        const double excitation = safeTanh(drive_ * (clampInput(input) - feedback_ * feedbackState_));
        const auto first = first_.process(excitation);
        SvfOutputs selected = first;
        if (fourPole_) selected = second_.process(responseValue(first, response_));
        feedbackState_ = selected.low;
        const double output = responseValue(selected, response_);
        if (!finite(output) || !finite(feedbackState_)) { reset(); return 0.0; }
        // This family is deliberately nonlinear; a bounded output stage keeps
        // high-Q automation from presenting an unbounded discontinuity to the
        // physical FilterCore while retaining the small-signal response.
        return safeTanh(output);
    }

private:
    TptSvfStage first_, second_;
    Response response_ = Response::LowPass;
    bool fourPole_ = false;
    double feedback_ = 0.0, drive_ = 1.0, feedbackState_ = 0.0;
};

// R LINEAR: serial, linear TPT-SVF family equivalent.  Unlike AnalogCore it
// uses no nonlinear drive stage; its resonance remains explicitly bounded.
class LinearCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance, Response response, bool fourPole) noexcept
    {
        response_ = response;
        fourPole_ = fourPole;
        const double q = qFromNorm(resonance, 0.70, fourPole ? 2.8 : 4.0);
        first_.configure(sampleRate, cutoff, q);
        second_.configure(sampleRate, cutoff, q);
        feedback_ = std::clamp(finiteOr(resonance), 0.0, 1.0) * (fourPole ? 0.06 : 0.04);
    }

    void reset() noexcept
    {
        first_.reset(); second_.reset(); feedbackState_ = 0.0;
    }

    double process(double input) noexcept
    {
        const auto first = first_.process(clampInput(input) - feedback_ * feedbackState_);
        SvfOutputs selected = first;
        if (fourPole_) selected = second_.process(responseValue(first, response_));
        feedbackState_ = selected.low;
        const double output = responseValue(selected, response_);
        if (!finite(output) || !finite(feedbackState_)) { reset(); return 0.0; }
        return safeOutput(output);
    }

private:
    TptSvfStage first_, second_;
    Response response_ = Response::LowPass;
    bool fourPole_ = false;
    double feedback_ = 0.0, feedbackState_ = 0.0;
};

// R RBJ: independently calculated cookbook-style biquad with LP/BP/HP
// responses.  Coefficients are normalized up front and state is direct form I.
class RbjCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance, Response response) noexcept
    {
        const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
        const double frequency = clampCutoff(cutoff, rate);
        const double omega = std::clamp(2.0 * kPi * frequency / rate, 0.0, 2.82);
        const double q = qFromNorm(resonance, 0.48, 13.0);
        const double alpha = std::sin(omega) / std::max(0.10, 2.0 * q);
        const double cosine = std::cos(omega);
        const double a0 = std::max(1.0e-12, 1.0 + alpha);
        double nb0 = 1.0, nb1 = 0.0, nb2 = 0.0;
        switch (response) {
            case Response::HighPass:
                nb0 = (1.0 + cosine) * 0.5;
                nb1 = -(1.0 + cosine);
                nb2 = nb0;
                break;
            case Response::BandPass:
                nb0 = alpha;
                nb1 = 0.0;
                nb2 = -alpha;
                break;
            case Response::Notch:
                nb0 = 1.0; nb1 = -2.0 * cosine; nb2 = 1.0;
                break;
            case Response::LowPass:
            default:
                nb0 = (1.0 - cosine) * 0.5;
                nb1 = 1.0 - cosine;
                nb2 = nb0;
                break;
        }
        b0_ = nb0 / a0; b1_ = nb1 / a0; b2_ = nb2 / a0;
        a1_ = -2.0 * cosine / a0; a2_ = (1.0 - alpha) / a0;
    }

    void reset() noexcept { x1_ = x2_ = y1_ = y2_ = 0.0; }

    double process(double input) noexcept
    {
        const double x = clampInput(input);
        const double y = b0_ * x + b1_ * x1_ + b2_ * x2_ - a1_ * y1_ - a2_ * y2_;
        if (!finite(y)) { reset(); return 0.0; }
        x2_ = x1_; x1_ = x;
        y2_ = y1_; y1_ = std::clamp(y, -64.0, 64.0);
        return safeOutput(y);
    }

private:
    double b0_ = 1.0, b1_ = 0.0, b2_ = 0.0, a1_ = 0.0, a2_ = 0.0;
    double x1_ = 0.0, x2_ = 0.0, y1_ = 0.0, y2_ = 0.0;
};

// R TPT: self-contained single-stage topology-preserving SVF.  The audited
// wrapper depended on an absent TptFilter.h, so this is intentionally a
// generic TPT equivalent rather than a claim about its unavailable model ID.
class TptCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance, Response response) noexcept
    {
        response_ = response;
        stage_.configure(sampleRate, cutoff, qFromNorm(resonance, 0.65, 10.5));
    }
    void reset() noexcept { stage_.reset(); }
    double process(double input) noexcept { return safeOutput(responseValue(stage_.process(input), response_)); }
private:
    TptSvfStage stage_;
    Response response_ = Response::LowPass;
};

// R HUV: bounded Huovilainen-flavoured nonlinear four-pole ladder
// equivalent. The audited source did not include its support layer, so this
// implementation keeps the named topology family without claiming exact model
// equivalence. A normalized TPT cascade replaces the earlier approximation,
// whose pass band could rise by more than +20 dB and sound broken at open cutoff.
class HuovilainenCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        const double frequency = clampCutoff(cutoff, sampleRate_);
        const double angle = std::clamp(kPi * frequency / sampleRate_, 0.0, 1.35);
        const double g = std::clamp(std::tan(angle), 1.0e-8, 6.0);
        stageGain_ = g / (1.0 + g);
        feedback_ = 3.60 * std::clamp(finiteOr(resonance), 0.0, 1.0);
        drive_ = 1.0 + 0.75 * std::clamp(finiteOr(resonance), 0.0, 1.0);
    }

    void reset() noexcept { state_.fill(0.0); }

    double process(double sample) noexcept
    {
        double stageInput = safeTanh(drive_ * (clampInput(sample) - feedback_ * state_[3]));
        for (int index = 0; index < 4; ++index) {
            const auto slot = static_cast<size_t>(index);
            const double v = (stageInput - state_[slot]) * stageGain_;
            const double output = v + state_[slot];
            const double next = output + v;
            if (!finite(output) || !finite(next)) { reset(); return 0.0; }
            state_[slot] = std::clamp(next, -32.0, 32.0);
            stageInput = safeTanh(output);
        }
        return safeOutput(stageInput);
    }

private:
    double sampleRate_ = 44100.0, stageGain_ = 0.1, feedback_ = 0.0, drive_ = 1.0;
    std::array<double, 4> state_{};
};

// Shared independently implemented four-pole nonlinear TPT ladder.  It gives
// Hyperion's multi-output response forms without taking the unavailable host
// implementation or asserting a bit-identical Newton/ADAA implementation.
class HyperionCore final {
public:
    enum class Mode : int { Lp2, Lp4, Bp2, Bp4, Hp2, Hp4, Notch };

    void configure(double sampleRate, double cutoff, double resonance, Mode mode) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        mode_ = mode;
        const double frequency = clampCutoff(cutoff, sampleRate_);
        const double angle = std::clamp(kPi * frequency / sampleRate_, 0.0, 1.35);
        g_ = std::clamp(std::tan(angle), 1.0e-7, 6.0);
        gain_ = g_ / (1.0 + g_);
        feedback_ = 3.85 * std::clamp(finiteOr(resonance), 0.0, 1.0);
        drive_ = 1.0 + 0.65 * std::clamp(finiteOr(resonance), 0.0, 1.0);
    }

    void reset() noexcept { z_.fill(0.0); previousOut_ = 0.0; }

    double process(double input) noexcept
    {
        const double raw = clampInput(input);
        // Saturated input and per-stage saturation provide a bounded implicit-
        // style ladder surrogate; the previous output closes the feedback loop.
        double stageInput = safeTanh(drive_ * (raw - feedback_ * previousOut_));
        std::array<double, 4> y{};
        for (int index = 0; index < 4; ++index) {
            const double saturated = safeTanh(stageInput);
            const double next = gain_ * saturated + (1.0 - gain_) * z_[static_cast<size_t>(index)];
            const double state = 2.0 * next - z_[static_cast<size_t>(index)];
            if (!finite(next) || !finite(state)) { reset(); return 0.0; }
            z_[static_cast<size_t>(index)] = std::clamp(state, -64.0, 64.0);
            y[static_cast<size_t>(index)] = next;
            stageInput = next;
        }
        previousOut_ = y[3];
        const double u = safeTanh(drive_ * (raw - feedback_ * previousOut_));
        double output = 0.0;
        switch (mode_) {
            case Mode::Lp2:  output = y[1]; break;
            case Mode::Lp4:  output = y[3]; break;
            case Mode::Bp2:  output = 2.0 * (y[0] - y[1]); break;
            case Mode::Bp4:  output = 4.0 * (y[2] - y[3]); break;
            case Mode::Hp2:  output = u - 2.0 * y[0] + y[1]; break;
            case Mode::Hp4:  output = u - 4.0 * y[0] + 6.0 * y[1] - 4.0 * y[2] + y[3]; break;
            case Mode::Notch: output = u - 4.0 * y[0] + 6.0 * y[1] - 4.0 * y[2]; break;
        }
        if (!finite(output) || !finite(previousOut_)) { reset(); return 0.0; }
        return safeOutput(output);
    }

private:
    double sampleRate_ = 44100.0, g_ = 0.1, gain_ = 0.1, feedback_ = 0.0, drive_ = 1.0, previousOut_ = 0.0;
    Mode mode_ = Mode::Lp4;
    std::array<double, 4> z_{};
};

// R KRAJ: bounded Stilson/Krajeski-compromise style nonlinear ladder.
class KrajeskiCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        // The original polynomial is fitted for the useful lower portion of
        // the band; cap there rather than extrapolating it through Nyquist.
        const double frequency = std::min(clampCutoff(cutoff, sampleRate_), sampleRate_ * 0.18);
        const double wc = 2.0 * kPi * frequency / sampleRate_;
        const double wc2 = wc * wc, wc3 = wc2 * wc, wc4 = wc3 * wc;
        g_ = std::clamp(0.9892 * wc - 0.4342 * wc2 + 0.1381 * wc3 - 0.0202 * wc4, 0.0, 0.995);
        const double compensation = 1.0029 + 0.0526 * wc - 0.926 * wc2 + 0.0218 * wc3;
        gRes_ = std::clamp(std::clamp(finiteOr(resonance), 0.0, 1.0) * compensation, 0.0, 1.1);
    }

    void reset() noexcept { state_.fill(0.0); delay_.fill(0.0); }

    double process(double input) noexcept
    {
        input = clampInput(input);
        state_[0] = safeTanh(input - 4.0 * gRes_ * (state_[4] - input));
        for (int index = 0; index < 4; ++index) {
            const double prior = state_[static_cast<size_t>(index + 1)];
            const double next = g_ * ((0.3 / 1.3) * state_[static_cast<size_t>(index)]
                                      + (1.0 / 1.3) * delay_[static_cast<size_t>(index)] - prior) + prior;
            if (!finite(next)) { reset(); return 0.0; }
            state_[static_cast<size_t>(index + 1)] = std::clamp(next, -64.0, 64.0);
            delay_[static_cast<size_t>(index)] = state_[static_cast<size_t>(index)];
        }
        return safeOutput(state_[4]);
    }

private:
    double sampleRate_ = 44100.0, g_ = 0.1, gRes_ = 0.0;
    std::array<double, 5> state_{};
    std::array<double, 5> delay_{};
};

// R MICRO: Microtracker-style four-stage nonlinear ladder equivalent.
class MicrotrackerCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance) noexcept
    {
        const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
        coefficient_ = std::clamp(2.0 * kPi * clampCutoff(cutoff, rate) / rate, 0.0, 0.95);
        feedback_ = 4.0 * std::clamp(finiteOr(resonance), 0.0, 1.0);
    }

    void reset() noexcept { p0_ = p1_ = p2_ = p3_ = p32_ = p33_ = p34_ = 0.0; }

    double process(double input) noexcept
    {
        const double output = p3_ * 0.360891 + p32_ * 0.417290 + p33_ * 0.177896 + p34_ * 0.0439725;
        p34_ = p33_; p33_ = p32_; p32_ = p3_;
        p0_ += (fastTanh(clampInput(input) - feedback_ * output) - fastTanh(p0_)) * coefficient_;
        p1_ += (fastTanh(p0_) - fastTanh(p1_)) * coefficient_;
        p2_ += (fastTanh(p1_) - fastTanh(p2_)) * coefficient_;
        p3_ += (fastTanh(p2_) - fastTanh(p3_)) * coefficient_;
        if (!finite(p0_) || !finite(p1_) || !finite(p2_) || !finite(p3_)) { reset(); return 0.0; }
        return safeOutput(output);
    }

private:
    static double fastTanh(double value) noexcept
    {
        value = std::clamp(finiteOr(value), -8.0, 8.0);
        const double square = value * value;
        return value * (27.0 + square) / (27.0 + 9.0 * square);
    }
    double coefficient_ = 0.1, feedback_ = 0.0;
    double p0_ = 0.0, p1_ = 0.0, p2_ = 0.0, p3_ = 0.0, p32_ = 0.0, p33_ = 0.0, p34_ = 0.0;
};

// R MUSIC: independently bounded classic four-pole ladder family equivalent.
class MusicCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance) noexcept
    {
        const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
        const double normalized = std::clamp(2.0 * clampCutoff(cutoff, rate) / rate, 0.0, 0.88);
        p_ = normalized * (1.8 - 0.8 * normalized);
        k_ = 2.0 * std::sin(normalized * kPi * 0.5) - 1.0;
        const double t1 = (1.0 - p_) * 1.386249;
        const double t2 = 12.0 + t1 * t1;
        const double denominator = std::max(0.25, std::abs(t2 - 6.0 * t1));
        resonance_ = std::clamp(finiteOr(resonance), 0.0, 1.0) * (t2 + 6.0 * t1) / denominator;
        resonance_ = std::clamp(resonance_, 0.0, 5.0);
    }

    void reset() noexcept { stage_.fill(0.0); delay_.fill(0.0); }

    double process(double input) noexcept
    {
        const double x = clampInput(input) - resonance_ * stage_[3];
        stage_[0] = x * p_ + delay_[0] * p_ - k_ * stage_[0];
        stage_[1] = stage_[0] * p_ + delay_[1] * p_ - k_ * stage_[1];
        stage_[2] = stage_[1] * p_ + delay_[2] * p_ - k_ * stage_[2];
        stage_[3] = stage_[2] * p_ + delay_[3] * p_ - k_ * stage_[3];
        stage_[3] -= (stage_[3] * stage_[3] * stage_[3]) / 6.0;
        if (!finite(stage_[0]) || !finite(stage_[1]) || !finite(stage_[2]) || !finite(stage_[3])) { reset(); return 0.0; }
        delay_[0] = x; delay_[1] = stage_[0]; delay_[2] = stage_[1]; delay_[3] = stage_[2];
        return safeOutput(stage_[3]);
    }

private:
    double p_ = 0.1, k_ = -0.5, resonance_ = 0.0;
    std::array<double, 4> stage_{};
    std::array<double, 4> delay_{};
};

class OberheimPole final {
public:
    void configure(double alpha, double beta) noexcept { alpha_ = alpha; beta_ = beta; }
    void reset() noexcept { z1_ = 0.0; }
    double feedbackOutput() const noexcept { return beta_ * z1_; }
    double tick(double input, double feedback) noexcept
    {
        const double v = (input + feedback - z1_) * alpha_;
        const double output = v + z1_;
        z1_ = v + output;
        return output;
    }
private:
    double alpha_ = 1.0, beta_ = 0.0, z1_ = 0.0;
};

// R OBER: independently implemented Oberheim-variation four-pole ladder.
class OberheimCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance) noexcept
    {
        const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
        const double frequency = clampCutoff(cutoff, rate);
        const double g = std::tan(std::clamp(kPi * frequency / rate, 0.0, 1.38));
        const double G = g / (1.0 + g);
        const double inv = 1.0 / (1.0 + g);
        poles_[0].configure(G, G * G * G * inv);
        poles_[1].configure(G, G * G * inv);
        poles_[2].configure(G, G * inv);
        poles_[3].configure(G, inv);
        k_ = 4.0 * std::clamp(finiteOr(resonance), 0.0, 1.0);
        const double gamma = G * G * G * G;
        alpha0_ = 1.0 / std::max(1.0e-8, 1.0 + k_ * gamma);
    }

    void reset() noexcept { for (auto& pole : poles_) pole.reset(); }

    double process(double input) noexcept
    {
        const double sigma = poles_[0].feedbackOutput() + poles_[1].feedbackOutput()
                           + poles_[2].feedbackOutput() + poles_[3].feedbackOutput();
        const double u = safeTanh((clampInput(input) * (1.0 + k_) - k_ * sigma) * alpha0_);
        const double s1 = poles_[0].tick(u, 0.0);
        const double s2 = poles_[1].tick(s1, 0.0);
        const double s3 = poles_[2].tick(s2, 0.0);
        const double s4 = poles_[3].tick(s3, 0.0);
        if (!finite(s1) || !finite(s2) || !finite(s3) || !finite(s4)) { reset(); return 0.0; }
        return safeOutput(s4);
    }

private:
    std::array<OberheimPole, 4> poles_{};
    double k_ = 0.0, alpha0_ = 1.0;
};

// R DVAL: D'Angelo/Valimaki-style trapezoidal nonlinear ladder equivalent
// derived independently from the Python reference's documented model family.
class DvalCore final {
public:
    void configure(double sampleRate, double cutoff, double resonance) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        const double frequency = std::min(clampCutoff(cutoff, sampleRate_), sampleRate_ * 0.40);
        const double x = kPi * frequency / sampleRate_;
        // The former (1 - x) term reversed the integrator sign above fs/pi.
        // At the normal open 1.9.x cutoff this could latch DVAL to a rail,
        // yielding a near-silent or garbled result after the outer DC blocker.
        // Keep the finite bilinear damping denominator without the sign flip.
        g_ = 4.0 * kPi * thermalVoltage_ * frequency / std::max(0.10, 1.0 + x);
        resonance_ = 4.0 * std::clamp(finiteOr(resonance), 0.0, 1.0);
    }

    void reset() noexcept
    {
        voltage_.fill(0.0); derivative_.fill(0.0); tanhState_.fill(0.0);
    }

    double process(double input) noexcept
    {
        const double invThermal = 1.0 / (2.0 * thermalVoltage_);
        const double in = clampInput(input);
        std::array<double, 4> nextDerivative{};
        nextDerivative[0] = -g_ * (safeTanh((in + resonance_ * voltage_[3]) * invThermal) + tanhState_[0]);
        for (int index = 1; index < 4; ++index)
            nextDerivative[static_cast<size_t>(index)] = g_ * (tanhState_[static_cast<size_t>(index - 1)]
                                                               - tanhState_[static_cast<size_t>(index)]);
        for (int index = 0; index < 4; ++index) {
            voltage_[static_cast<size_t>(index)] += (nextDerivative[static_cast<size_t>(index)]
                                                    + derivative_[static_cast<size_t>(index)]) / (2.0 * sampleRate_);
            derivative_[static_cast<size_t>(index)] = nextDerivative[static_cast<size_t>(index)];
            if (!finite(voltage_[static_cast<size_t>(index)])) { reset(); return 0.0; }
            voltage_[static_cast<size_t>(index)] = std::clamp(voltage_[static_cast<size_t>(index)], -64.0, 64.0);
            tanhState_[static_cast<size_t>(index)] = safeTanh(voltage_[static_cast<size_t>(index)] * invThermal);
        }
        return safeOutput(voltage_[3]);
    }

private:
    static constexpr double thermalVoltage_ = 0.312;
    double sampleRate_ = 44100.0, g_ = 0.0, resonance_ = 0.0;
    std::array<double, 4> voltage_{}, derivative_{}, tanhState_{};
};

// This enum maps exactly one-to-one to appended HybridFilterAlgorithm values
// through import2ModeForHybrid() in HybridDSP.hpp.
enum class Import2Mode : int {
    AnalogLp12, AnalogLp24, AnalogBp12, AnalogBp24, AnalogHp12, AnalogHp24,
    LinearLp12, LinearLp24, LinearBp12, LinearBp24, LinearHp12, LinearHp24,
    RbjLp, RbjBp, RbjHp,
    TptLp, TptBp, TptHp,
    HuovilainenLp4,
    HyperionLp2, HyperionLp4, HyperionBp2, HyperionBp4, HyperionHp2, HyperionHp4, HyperionNotch,
    KrajeskiLp4, MicrotrackerLp4, MusicLp4, OberheimLp4, DvalLp4,
    // Diagnostic-only complements of the LP-only cores: HP = dry input minus
    // the core's LP output. DVAL's recovered LP exits with inverted polarity,
    // so its complement uses dry input plus LP. They remain available to old
    // state/diagnostics, but the MODE popup intentionally does not expose them:
    // without dry input they would not be honest HP filters.
    HuovilainenHp4, KrajeskiHp4, MicrotrackerHp4, MusicHp4, OberheimHp4, DvalHp4
};

class Import2Core final {
public:
    void prepare(double sampleRate) noexcept
    {
        sampleRate_ = sampleRate > 1000.0 ? sampleRate : 44100.0;
        reset();
    }

    void reset() noexcept
    {
        analog_.reset(); linear_.reset(); rbj_.reset(); tpt_.reset(); huovilainen_.reset();
        hyperion_.reset(); krajeski_.reset(); microtracker_.reset(); music_.reset(); oberheim_.reset(); dval_.reset();
        configured_ = false;
    }

    void configure(Import2Mode mode, double cutoff, double resonance) noexcept
    {
        if (configured_ && mode != mode_) reset();
        mode_ = mode;
        cutoff_ = clampCutoff(cutoff, sampleRate_);
        resonance_ = std::clamp(finiteOr(resonance), 0.0, 1.0);
        configured_ = true;
        switch (mode_) {
            case Import2Mode::AnalogLp12: analog_.configure(sampleRate_, cutoff_, resonance_, Response::LowPass, false); break;
            case Import2Mode::AnalogLp24: analog_.configure(sampleRate_, cutoff_, resonance_, Response::LowPass, true); break;
            case Import2Mode::AnalogBp12: analog_.configure(sampleRate_, cutoff_, resonance_, Response::BandPass, false); break;
            case Import2Mode::AnalogBp24: analog_.configure(sampleRate_, cutoff_, resonance_, Response::BandPass, true); break;
            case Import2Mode::AnalogHp12: analog_.configure(sampleRate_, cutoff_, resonance_, Response::HighPass, false); break;
            case Import2Mode::AnalogHp24: analog_.configure(sampleRate_, cutoff_, resonance_, Response::HighPass, true); break;
            case Import2Mode::LinearLp12: linear_.configure(sampleRate_, cutoff_, resonance_, Response::LowPass, false); break;
            case Import2Mode::LinearLp24: linear_.configure(sampleRate_, cutoff_, resonance_, Response::LowPass, true); break;
            case Import2Mode::LinearBp12: linear_.configure(sampleRate_, cutoff_, resonance_, Response::BandPass, false); break;
            case Import2Mode::LinearBp24: linear_.configure(sampleRate_, cutoff_, resonance_, Response::BandPass, true); break;
            case Import2Mode::LinearHp12: linear_.configure(sampleRate_, cutoff_, resonance_, Response::HighPass, false); break;
            case Import2Mode::LinearHp24: linear_.configure(sampleRate_, cutoff_, resonance_, Response::HighPass, true); break;
            case Import2Mode::RbjLp: rbj_.configure(sampleRate_, cutoff_, resonance_, Response::LowPass); break;
            case Import2Mode::RbjBp: rbj_.configure(sampleRate_, cutoff_, resonance_, Response::BandPass); break;
            case Import2Mode::RbjHp: rbj_.configure(sampleRate_, cutoff_, resonance_, Response::HighPass); break;
            case Import2Mode::TptLp: tpt_.configure(sampleRate_, cutoff_, resonance_, Response::LowPass); break;
            case Import2Mode::TptBp: tpt_.configure(sampleRate_, cutoff_, resonance_, Response::BandPass); break;
            case Import2Mode::TptHp: tpt_.configure(sampleRate_, cutoff_, resonance_, Response::HighPass); break;
            case Import2Mode::HuovilainenLp4: case Import2Mode::HuovilainenHp4:
                huovilainen_.configure(sampleRate_, cutoff_, resonance_); break;
            case Import2Mode::HyperionLp2: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Lp2); break;
            case Import2Mode::HyperionLp4: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Lp4); break;
            case Import2Mode::HyperionBp2: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Bp2); break;
            case Import2Mode::HyperionBp4: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Bp4); break;
            case Import2Mode::HyperionHp2: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Hp2); break;
            case Import2Mode::HyperionHp4: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Hp4); break;
            case Import2Mode::HyperionNotch: hyperion_.configure(sampleRate_, cutoff_, resonance_, HyperionCore::Mode::Notch); break;
            case Import2Mode::KrajeskiLp4: case Import2Mode::KrajeskiHp4:
                krajeski_.configure(sampleRate_, cutoff_, resonance_); break;
            case Import2Mode::MicrotrackerLp4: case Import2Mode::MicrotrackerHp4:
                microtracker_.configure(sampleRate_, cutoff_, resonance_); break;
            case Import2Mode::MusicLp4: case Import2Mode::MusicHp4:
                music_.configure(sampleRate_, cutoff_, resonance_); break;
            case Import2Mode::OberheimLp4: case Import2Mode::OberheimHp4:
                oberheim_.configure(sampleRate_, cutoff_, resonance_); break;
            case Import2Mode::DvalLp4: case Import2Mode::DvalHp4:
                dval_.configure(sampleRate_, cutoff_, resonance_); break;
        }
    }

    float process(float input) noexcept
    {
        if (!configured_) return std::isfinite(input) ? input : 0.0f;
        const double x = static_cast<double>(input);
        double output = 0.0;
        switch (mode_) {
            case Import2Mode::AnalogLp12: case Import2Mode::AnalogLp24: case Import2Mode::AnalogBp12:
            case Import2Mode::AnalogBp24: case Import2Mode::AnalogHp12: case Import2Mode::AnalogHp24:
                output = analog_.process(x); break;
            case Import2Mode::LinearLp12: case Import2Mode::LinearLp24: case Import2Mode::LinearBp12:
            case Import2Mode::LinearBp24: case Import2Mode::LinearHp12: case Import2Mode::LinearHp24:
                output = linear_.process(x); break;
            case Import2Mode::RbjLp: case Import2Mode::RbjBp: case Import2Mode::RbjHp:
                output = rbj_.process(x); break;
            case Import2Mode::TptLp: case Import2Mode::TptBp: case Import2Mode::TptHp:
                output = tpt_.process(x); break;
            case Import2Mode::HuovilainenLp4: output = huovilainen_.process(x); break;
            case Import2Mode::HuovilainenHp4: output = x - huovilainen_.process(x); break;
            case Import2Mode::HyperionLp2: case Import2Mode::HyperionLp4: case Import2Mode::HyperionBp2:
            case Import2Mode::HyperionBp4: case Import2Mode::HyperionHp2: case Import2Mode::HyperionHp4:
            case Import2Mode::HyperionNotch: output = hyperion_.process(x); break;
            case Import2Mode::KrajeskiLp4: output = krajeski_.process(x); break;
            case Import2Mode::KrajeskiHp4: output = x - krajeski_.process(x); break;
            case Import2Mode::MicrotrackerLp4: output = microtracker_.process(x); break;
            case Import2Mode::MicrotrackerHp4: output = x - microtracker_.process(x); break;
            case Import2Mode::MusicLp4: output = music_.process(x); break;
            case Import2Mode::MusicHp4: output = x - music_.process(x); break;
            case Import2Mode::OberheimLp4: output = oberheim_.process(x); break;
            case Import2Mode::OberheimHp4: output = x - oberheim_.process(x); break;
            case Import2Mode::DvalLp4: output = dval_.process(x); break;
            case Import2Mode::DvalHp4: output = x + dval_.process(x); break;
        }
        // A final transparent-at-normal-level soft bound is shared by every
        // independently implemented family. It contains hostile host-control
        // combinations before the surrounding physical filter's own limiter.
        const double guarded = safeOutput(output);
        return static_cast<float>(4.0 * std::tanh(0.25 * guarded));
    }

private:
    double sampleRate_ = 44100.0, cutoff_ = 1000.0, resonance_ = 0.0;
    Import2Mode mode_ = Import2Mode::AnalogLp12;
    bool configured_ = false;
    AnalogCore analog_;
    LinearCore linear_;
    RbjCore rbj_;
    TptCore tpt_;
    HuovilainenCore huovilainen_;
    HyperionCore hyperion_;
    KrajeskiCore krajeski_;
    MicrotrackerCore microtracker_;
    MusicCore music_;
    OberheimCore oberheim_;
    DvalCore dval_;
};

} // namespace nova::hybrid_private::reference_import2
