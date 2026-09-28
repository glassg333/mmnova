/*
  PRIVATE HYBRID DSP TEST

  K35 LP/HP, six Moog-style ladder responses, and Fold/Zero/Clamp support
  for Nova's separate physical filter and DIST selections. The source-reference bundle in upstream/ is retained
  unchanged; the active module below is isolated, allocation-free, and uses
  safe zero-modulation defaults.
*/
#pragma once

#include <algorithm>
#include <array>
#include <cmath>

#include "RClassicFilters.hpp"
#include "RImport2Filters.hpp"

namespace nova::hybrid_private {

inline constexpr double kPi = 3.14159265358979323846;
inline constexpr double kFilterFcMin = 20.0;
inline constexpr double kFilterFcMax = 20000.0;
inline constexpr double kFilterFcDefault = 10000.0;
inline constexpr double kFilterQDefault = 0.707;
inline constexpr float kThresholdMinimum = 0.05f;
inline constexpr float kThresholdSmoothing = 0.998f;

// A cutoff or resonance target can arrive at a host-block boundary while a
// resonant integrator is carrying energy.  Smooth the physical families for a
// fixed 10 ms in time, rather than a fixed 16 samples, so 44.1/48/96 kHz all
// receive comparable de-clicking without a callback/state reset.
inline int physicalControlRampSamples(double sampleRate) noexcept {
    constexpr double kPhysicalControlRampSeconds = 0.010;
    const double rate = sampleRate > 1000.0 ? sampleRate : 44100.0;
    return std::clamp(static_cast<int>(std::lround(rate * kPhysicalControlRampSeconds)), 64, 2048);
}

// Canonical algorithm IDs used by FilterCore. IDs 0..8 are the 1.9.8
// serialization contract, IDs 9..20 are the 1.9.9 R-classic contract, IDs
// 21..51 are R Import 2, and 52..57 are appended HP complements. No existing
// ID is inserted, reordered, or repurposed.
//
// UI choice order is deliberately side-specific: BASE / MODE L presents a
// lower-edge (high-pass) response before non-edge alternatives, while WDTH /
// MODE H presents an upper-edge (low-pass) response first. The response text
// and processing mode therefore never falsely call an LP a lower-side HP, or
// an HP an upper-side LP.
enum HybridFilterAlgorithm : int {
    hybridNative = 0,
    hybridK35Lp = 1,
    hybridK35Hp = 2,
    hybridMoogLp24 = 3,
    hybridMoogLp12 = 4,
    hybridMoogBp24 = 5,
    hybridMoogBp12 = 6,
    hybridMoogHp24 = 7,
    hybridMoogHp12 = 8,

    hybridR303Lp = 9,
    hybridR303Bp = 10,
    hybridR303Hp = 11,
    hybridRMs20Lp = 12,
    hybridRMs20Bp = 13,
    hybridRMs20Hp = 14,
    hybridRMoogLp24 = 15,
    hybridRMoogLp12 = 16,
    hybridRMoogBp24 = 17,
    hybridRMoogBp12 = 18,
    hybridRMoogHp24 = 19,
    hybridRMoogHp12 = 20,

    hybridRAnalogLp12 = 21,
    hybridRAnalogLp24 = 22,
    hybridRAnalogBp12 = 23,
    hybridRAnalogBp24 = 24,
    hybridRAnalogHp12 = 25,
    hybridRAnalogHp24 = 26,
    hybridRLinearLp12 = 27,
    hybridRLinearLp24 = 28,
    hybridRLinearBp12 = 29,
    hybridRLinearBp24 = 30,
    hybridRLinearHp12 = 31,
    hybridRLinearHp24 = 32,
    hybridRRbjLp = 33,
    hybridRRbjBp = 34,
    hybridRRbjHp = 35,
    hybridRTptLp = 36,
    hybridRTptBp = 37,
    hybridRTptHp = 38,
    hybridRHuovilainenLp4 = 39,
    hybridRHyperionLp2 = 40,
    hybridRHyperionLp4 = 41,
    hybridRHyperionBp2 = 42,
    hybridRHyperionBp4 = 43,
    hybridRHyperionHp2 = 44,
    hybridRHyperionHp4 = 45,
    hybridRHyperionNotch = 46,
    hybridRKrajeskiLp4 = 47,
    hybridRMicrotrackerLp4 = 48,
    hybridRMusicLp4 = 49,
    hybridROberheimLp4 = 50,
    hybridRDvalLp4 = 51,

    // Appended, derived complements for LP-only R Import 2 cores.  They are
    // intentionally separate IDs: old saved LP selections remain byte-for-byte
    // the same sound and MODE L/H can classify the derived response honestly.
    hybridRHuovilainenHp4 = 52,
    hybridRKrajeskiHp4 = 53,
    hybridRMicrotrackerHp4 = 54,
    hybridRMusicHp4 = 55,
    hybridROberheimHp4 = 56,
    hybridRDvalHp4 = 57
};
inline constexpr int kHybridLegacyAlgorithmCount = 9;
inline constexpr int kHybridRClassicFirstAlgorithm = hybridR303Lp;
inline constexpr int kHybridRClassicLastAlgorithm = hybridRMoogHp12;
inline constexpr int kHybridRImport2FirstAlgorithm = hybridRAnalogLp12;
inline constexpr int kHybridRImport2LastAlgorithm = hybridRDvalHp4;
inline constexpr int kHybridFilterAlgorithmCount = 58;
inline constexpr int kHybridFilterLastAlgorithm = kHybridFilterAlgorithmCount - 1;
inline constexpr bool hybridUsesKorg35(int mode) noexcept
{
    return mode == hybridK35Lp || mode == hybridK35Hp;
}
inline constexpr bool hybridUsesOdinLadder(int mode) noexcept
{
    return mode >= hybridMoogLp24 && mode <= hybridMoogHp12;
}
inline constexpr bool hybridUsesReferenceClassic(int mode) noexcept
{
    return mode >= kHybridRClassicFirstAlgorithm && mode <= kHybridRClassicLastAlgorithm;
}
inline constexpr bool hybridUsesReferenceImport2(int mode) noexcept
{
    return mode >= kHybridRImport2FirstAlgorithm && mode <= kHybridRImport2LastAlgorithm;
}

// These six experimental IDs synthesize an HP-looking response by mixing the
// input (dry) with an LP-only core output. They are deliberately not normal
// user-facing MODE choices: removing the input term would make them LP/inverted
// LP, not an honest high-pass implementation.
inline constexpr bool hybridIsDryDerivedComplement(int mode) noexcept
{
    return mode >= hybridRHuovilainenHp4 && mode <= hybridRDvalHp4;
}

// Keep the deliberately extreme Hyperion family at the end of each visible
// response folder, rather than between ordinary filter families.
inline constexpr bool hybridIsHyperionFamily(int mode) noexcept
{
    return mode >= hybridRHyperionLp2 && mode <= hybridRHyperionNotch;
}

// FILT DSP names only the two retained native renderers. MODE L/H names the
// independently selected physical filter family. Once either physical selector
// is non-NATIVE, neither the OLD renderer nor the MNM native renderer is part
// of that audio path. This deliberately avoids an OLD+K35/R or MNM+K35/R
// serial topology while retaining the historic side-specific MODE L/H law.
enum class FilterRenderRoute : unsigned char {
    LegacyOld,
    NativeMnm,
    IndependentPhysical
};

enum class PhysicalSideRenderer : unsigned char {
    Native,
    SelectedFamily
};

inline constexpr bool hasIndependentPhysicalSelection(int lowerMode, int upperMode) noexcept
{
    return lowerMode != hybridNative || upperMode != hybridNative;
}

// This planner is shared by the real side renderer and the standalone route
// regression. A selected K35/ladder/R side replaces the native stage on that
// same physical side exactly once. A NATIVE result can occur only because that
// *other* MODE side is explicitly NATIVE; it is not inherited from FILT DSP.
inline constexpr PhysicalSideRenderer physicalSideRendererFor(int mode) noexcept
{
    return mode == hybridNative ? PhysicalSideRenderer::Native
                                : PhysicalSideRenderer::SelectedFamily;
}

inline constexpr FilterRenderRoute selectFilterRenderRoute(bool mnmModeSelected,
                                                            int lowerMode,
                                                            int upperMode) noexcept
{
    if (hasIndependentPhysicalSelection(lowerMode, upperMode))
        return FilterRenderRoute::IndependentPhysical;
    return mnmModeSelected ? FilterRenderRoute::NativeMnm
                           : FilterRenderRoute::LegacyOld;
}

// Keep the decision and invocation together, so a regression can prove that a
// selected physical family invokes only its dedicated renderer. The TrackChain
// calls this helper directly; it is not merely a test model of the route.
template <typename LegacyOldFn, typename NativeMnmFn, typename IndependentFn>
inline void dispatchFilterRenderRoute(bool mnmModeSelected,
                                      int lowerMode,
                                      int upperMode,
                                      LegacyOldFn&& legacyOld,
                                      NativeMnmFn&& nativeMnm,
                                      IndependentFn&& independentPhysical)
{
    switch (selectFilterRenderRoute(mnmModeSelected, lowerMode, upperMode)) {
        case FilterRenderRoute::LegacyOld:
            legacyOld();
            break;
        case FilterRenderRoute::NativeMnm:
            nativeMnm();
            break;
        case FilterRenderRoute::IndependentPhysical:
            independentPhysical();
            break;
    }
}

enum class HybridResponse : int { Native, LowPass, BandPass, HighPass, Notch };
inline constexpr HybridResponse hybridResponseForAlgorithm(int mode) noexcept
{
    switch (mode) {
        case hybridK35Lp: case hybridMoogLp24: case hybridMoogLp12:
        case hybridR303Lp: case hybridRMs20Lp: case hybridRMoogLp24: case hybridRMoogLp12:
        case hybridRAnalogLp12: case hybridRAnalogLp24: case hybridRLinearLp12: case hybridRLinearLp24:
        case hybridRRbjLp: case hybridRTptLp: case hybridRHuovilainenLp4:
        case hybridRHyperionLp2: case hybridRHyperionLp4: case hybridRKrajeskiLp4:
        case hybridRMicrotrackerLp4: case hybridRMusicLp4: case hybridROberheimLp4: case hybridRDvalLp4:
            return HybridResponse::LowPass;
        case hybridMoogBp24: case hybridMoogBp12:
        case hybridR303Bp: case hybridRMs20Bp: case hybridRMoogBp24: case hybridRMoogBp12:
        case hybridRAnalogBp12: case hybridRAnalogBp24: case hybridRLinearBp12: case hybridRLinearBp24:
        case hybridRRbjBp: case hybridRTptBp: case hybridRHyperionBp2: case hybridRHyperionBp4:
            return HybridResponse::BandPass;
        case hybridK35Hp: case hybridMoogHp24: case hybridMoogHp12:
        case hybridR303Hp: case hybridRMs20Hp: case hybridRMoogHp24: case hybridRMoogHp12:
        case hybridRAnalogHp12: case hybridRAnalogHp24: case hybridRLinearHp12: case hybridRLinearHp24:
        case hybridRRbjHp: case hybridRTptHp: case hybridRHyperionHp2: case hybridRHyperionHp4:
        case hybridRHuovilainenHp4: case hybridRKrajeskiHp4: case hybridRMicrotrackerHp4:
        case hybridRMusicHp4: case hybridROberheimHp4: case hybridRDvalHp4:
            return HybridResponse::HighPass;
        case hybridRHyperionNotch:
            return HybridResponse::Notch;
        case hybridNative:
        default:
            return HybridResponse::Native;
    }
}

inline constexpr std::array<int, kHybridFilterAlgorithmCount> kModeLChoiceToAlgorithm{
    // Lower physical side: true HP/low-cut edge forms before BP/LP choices.
    hybridNative, hybridK35Hp, hybridMoogHp24, hybridMoogHp12,
    hybridMoogBp24, hybridMoogBp12, hybridK35Lp, hybridMoogLp24, hybridMoogLp12,
    hybridR303Hp, hybridRMs20Hp, hybridRMoogHp24, hybridRMoogHp12,
    hybridR303Bp, hybridRMs20Bp, hybridRMoogBp24, hybridRMoogBp12,
    hybridR303Lp, hybridRMs20Lp, hybridRMoogLp24, hybridRMoogLp12,
    hybridRAnalogHp24, hybridRAnalogHp12, hybridRAnalogBp24, hybridRAnalogBp12, hybridRAnalogLp24, hybridRAnalogLp12,
    hybridRLinearHp24, hybridRLinearHp12, hybridRLinearBp24, hybridRLinearBp12, hybridRLinearLp24, hybridRLinearLp12,
    hybridRRbjHp, hybridRRbjBp, hybridRRbjLp,
    hybridRTptHp, hybridRTptBp, hybridRTptLp,
    hybridRHuovilainenLp4,
    hybridRHyperionHp4, hybridRHyperionHp2, hybridRHyperionBp4, hybridRHyperionBp2, hybridRHyperionNotch, hybridRHyperionLp4, hybridRHyperionLp2,
    hybridRKrajeskiLp4, hybridRMicrotrackerLp4, hybridRMusicLp4, hybridROberheimLp4, hybridRDvalLp4,
    hybridRHuovilainenHp4, hybridRKrajeskiHp4, hybridRMicrotrackerHp4, hybridRMusicHp4, hybridROberheimHp4, hybridRDvalHp4
};
inline constexpr std::array<int, kHybridFilterAlgorithmCount> kModeHChoiceToAlgorithm{
    // Upper physical side: true LP/high-cut edge forms before BP/HP choices.
    hybridNative, hybridK35Lp, hybridMoogLp24, hybridMoogLp12,
    hybridMoogBp24, hybridMoogBp12, hybridK35Hp, hybridMoogHp24, hybridMoogHp12,
    hybridR303Lp, hybridRMs20Lp, hybridRMoogLp24, hybridRMoogLp12,
    hybridR303Bp, hybridRMs20Bp, hybridRMoogBp24, hybridRMoogBp12,
    hybridR303Hp, hybridRMs20Hp, hybridRMoogHp24, hybridRMoogHp12,
    hybridRAnalogLp24, hybridRAnalogLp12, hybridRAnalogBp24, hybridRAnalogBp12, hybridRAnalogHp24, hybridRAnalogHp12,
    hybridRLinearLp24, hybridRLinearLp12, hybridRLinearBp24, hybridRLinearBp12, hybridRLinearHp24, hybridRLinearHp12,
    hybridRRbjLp, hybridRRbjBp, hybridRRbjHp,
    hybridRTptLp, hybridRTptBp, hybridRTptHp,
    hybridRHuovilainenLp4,
    hybridRHyperionLp4, hybridRHyperionLp2, hybridRHyperionBp4, hybridRHyperionBp2, hybridRHyperionNotch, hybridRHyperionHp4, hybridRHyperionHp2,
    hybridRKrajeskiLp4, hybridRMicrotrackerLp4, hybridRMusicLp4, hybridROberheimLp4, hybridRDvalLp4,
    hybridRHuovilainenHp4, hybridRKrajeskiHp4, hybridRMicrotrackerHp4, hybridRMusicHp4, hybridROberheimHp4, hybridRDvalHp4
};
inline int modeLChoiceToAlgorithm(int choice) noexcept {
    return kModeLChoiceToAlgorithm[static_cast<size_t>(std::clamp(choice, 0, kHybridFilterLastAlgorithm))];
}
inline int modeHChoiceToAlgorithm(int choice) noexcept {
    return kModeHChoiceToAlgorithm[static_cast<size_t>(std::clamp(choice, 0, kHybridFilterLastAlgorithm))];
}

// MODE S is the one authority for the physical DIST block. These are user
// selection IDs, distinct from OversamplingDistortionStereo's internal
// character IDs (FOLD=1, ZERO=2, CLAMP=3).
enum HybridSaturationSelection : int {
    hybridDistMnm = 0,
    hybridDistOld = 1,
    // Explicit A/B candidate for the reported 3.5 kHz level notch. It is
    // intentionally marked experimental rather than firmware-identical.
    hybridDistMnmFix = 2,
    hybridDistFold = 3,
    hybridDistZero = 4,
    hybridDistClamp = 5,
    // Appended 1.9.15 A/B candidates. Existing 0..5 values stay serialized.
    hybridDistMnmOld = 6,
    hybridDistMnmV2 = 7,
    hybridDistOldV2 = 8
};

// Shared control and modulation base for hybrid filter modules.
class HybridFilterBase {
public:
    virtual ~HybridFilterBase() = default;

    inline float pitchShiftMultiplier(float semitones) const noexcept {
        return static_cast<float>(std::exp(0.05776226504 * static_cast<double>(semitones)));
    }
    inline float fasttanh(float input, float factor) const noexcept {
        return std::tanh(factor * input);
    }
    virtual void update() noexcept {
        const float kbdModded = m_kbd_mod_amount + *m_kbd_mod_mod < 0 ? 0 : m_kbd_mod_amount + *m_kbd_mod_mod;
        const float velModded = m_vel_mod_amount + *m_vel_mod_mod < 0 ? 0 : m_vel_mod_amount + *m_vel_mod_mod;
        m_freq_modded = m_freq_base;
        if (*m_freq_mod + kbdModded + m_env_mod_amount + *m_env_mod_mod + velModded) {
            m_freq_modded *= pitchShiftMultiplier(
                *m_freq_mod * 64.0f + kbdModded * static_cast<float>(m_MIDI_note) +
                (m_env_value * (m_env_mod_amount + *m_env_mod_mod) + velModded * static_cast<float>(m_MIDI_velocity) / 127.0f) * 64.0f);
        }
        m_freq_modded = std::clamp(m_freq_modded, kFilterFcMin, kFilterFcMax);
    }
    inline void applyOverdrive(double& input, float tanhFactor = 3.5f) noexcept {
        float overdriveModded = m_overdrive + 2.0f * (*m_saturation_mod);
        overdriveModded = std::max(0.0f, overdriveModded);
        if (overdriveModded > 0.01f && overdriveModded < 1.0f) {
            input = input * (1.0 - overdriveModded) + overdriveModded * fasttanh(static_cast<float>(input), tanhFactor);
        } else if (overdriveModded >= 1.0f) {
            input = fasttanh(overdriveModded * static_cast<float>(input), tanhFactor);
        }
    }

    void setFreqModPointer(float* pointer) noexcept { m_freq_mod = pointer ? pointer : &m_mod_dummy_zero; }
    void setResModPointer(float* pointer) noexcept { m_res_mod = pointer ? pointer : &m_mod_dummy_zero; }
    void setVelModPointer(float* pointer) noexcept { m_vel_mod_mod = pointer ? pointer : &m_mod_dummy_zero; }
    void setKbdModPointer(float* pointer) noexcept { m_kbd_mod_mod = pointer ? pointer : &m_mod_dummy_zero; }
    void setSaturationModPointer(float* pointer) noexcept { m_saturation_mod = pointer ? pointer : &m_mod_dummy_zero; }
    void setEnvModPointer(float* pointer) noexcept { m_env_mod_mod = pointer ? pointer : &m_mod_dummy_zero; }
    virtual void setResControl(double) noexcept {}
    virtual void setSampleRate(double rate) noexcept {
        m_samplerate = rate;
        m_one_over_samplerate = rate > 0.0 ? 1.0 / rate : 0.0;
    }
    virtual double doFilter(double input) noexcept = 0;
    virtual void reset() noexcept {}

    double m_freq_base = kFilterFcDefault;
    double m_res_base = 1.0;
    int m_MIDI_note = 0;
    int m_MIDI_velocity = 0;
    float m_kbd_mod_amount = 0.0f;
    float m_vel_mod_amount = 0.0f;
    float m_env_mod_amount = 0.0f;
    float m_env_value = 0.0f;
    float m_overdrive = 0.0f;
    double m_mod_frequency = 0.0;

protected:
    float m_mod_dummy_zero = 0.0f;
    float* m_res_mod = &m_mod_dummy_zero;
    float* m_freq_mod = &m_mod_dummy_zero;
    float* m_saturation_mod = &m_mod_dummy_zero;
    float* m_env_mod_mod = &m_mod_dummy_zero;
    float* m_vel_mod_mod = &m_mod_dummy_zero;
    float* m_kbd_mod_mod = &m_mod_dummy_zero;
    double m_samplerate = -1.0;
    double m_one_over_samplerate = 0.0;
    double m_freq_modded = kFilterFcDefault;
    double m_res_modded = kFilterQDefault;
};

// K35 one-pole support state.
class VAOnePoleFilter final : public HybridFilterBase {
public:
    VAOnePoleFilter() noexcept { reset(); }

    void setFeedback(double feedback) noexcept { m_feedback = feedback; }
    double getFeedbackOutput() const noexcept { return m_beta * (m_z_1 + m_feedback * m_delta); }
    void reset() noexcept override { m_z_1 = 0.0; m_feedback = 0.0; }
    void update() noexcept override {
        HybridFilterBase::update();
        const double wd = 2.0 * kPi * m_freq_modded;
        const double t = 1.0 / m_samplerate;
        const double wa = (2.0 / t) * std::tan(wd * t * 0.5);
        const double g = wa * t * 0.5;
        m_alpha = g / (1.0 + g);
    }
    double doFilter(double input) noexcept override {
        input = input * m_gamma + m_feedback + m_epsilon * getFeedbackOutput();
        const double vn = (m_a_0 * input - m_z_1) * m_alpha;
        const double lpf = vn + m_z_1;
        m_z_1 = vn + lpf;
        return m_is_lowpass ? lpf : input - lpf;
    }
    void setLP() noexcept { m_is_lowpass = true; }
    void setHP() noexcept { m_is_lowpass = false; }

    double m_alpha = 1.0;
    double m_beta = 0.0;
    double m_gamma = 1.0;
    double m_delta = 0.0;
    double m_epsilon = 0.0;
    double m_a_0 = 1.0;
    double m_feedback = 0.0;

private:
    bool m_is_lowpass = true;
    double m_z_1 = 0.0;
};

// K35 LP/HP filter state.
class Korg35Filter final : public HybridFilterBase {
public:
    Korg35Filter() noexcept {
        m_LPF1.setLP(); m_LPF2.setLP(); m_HPF1.setHP(); m_HPF2.setHP();
        reset();
    }
    void reset() noexcept override {
        m_LPF1.reset(); m_LPF2.reset(); m_HPF1.reset(); m_HPF2.reset();
    }
    void update() noexcept override {
        HybridFilterBase::update();
        if (m_freq_modded == m_last_freq_modded && !(*m_res_mod)) return;
        m_last_freq_modded = m_freq_modded;
        const double wd = 2.0 * 3.141592653 * m_freq_modded;
        const double wa = (2.0 * m_samplerate) * std::tan(wd * m_one_over_samplerate * 0.5);
        const double g = wa * m_one_over_samplerate * 0.5;
        const double G = g / (1.0 + g);
        m_LPF1.m_alpha = G; m_LPF2.m_alpha = G; m_HPF1.m_alpha = G; m_HPF2.m_alpha = G;
        m_k_modded = std::clamp(m_k + (*m_res_mod) * 2.0, 0.01, 1.96);
        m_alpha = 1.0 / (1.0 - m_k_modded * G + m_k_modded * G * G);
        if (m_is_lowpass) {
            m_LPF2.m_beta = (m_k_modded - m_k_modded * G) / (1.0 + g);
            m_HPF1.m_beta = -1.0 / (1.0 + g);
        } else {
            m_HPF2.m_beta = -1.0 * G / (1.0 + g);
            m_LPF1.m_beta = 1.0 / (1.0 + g);
        }
    }
    double doFilter(double input) noexcept override {
        double y = 0.0;
        if (m_is_lowpass) {
            const double y1 = m_LPF1.doFilter(input);
            const double s35 = m_LPF2.getFeedbackOutput() + m_HPF1.getFeedbackOutput();
            const double u = m_alpha * (y1 + s35);
            y = m_k_modded * m_LPF2.doFilter(u);
            m_HPF1.doFilter(y);
        } else {
            const double y1 = m_HPF1.doFilter(input);
            const double s35 = m_HPF2.getFeedbackOutput() + m_LPF1.getFeedbackOutput();
            const double u = m_alpha * (y1 + s35);
            y = m_k_modded * u;
            m_LPF1.doFilter(m_HPF2.doFilter(y));
        }
        y /= m_k_modded;
        applyOverdrive(y, 3.0f);
        return y;
    }
    void setResControl(double resonance) noexcept override {
        m_k = resonance * 1.95 + 0.01;
        m_last_freq_modded = -1.0;
    }
    void setFilterType(bool isLowpass) noexcept {
        m_is_lowpass = isLowpass;
        m_last_freq_modded = -1.0;
    }
    void setSampleRate(double rate) noexcept override {
        HybridFilterBase::setSampleRate(rate);
        m_LPF1.setSampleRate(rate); m_LPF2.setSampleRate(rate);
        m_HPF1.setSampleRate(rate); m_HPF2.setSampleRate(rate);
        m_last_freq_modded = -1.0;
    }

private:
    double m_last_freq_modded = -1.0;
    double m_k = 0.01;
    double m_k_modded = 0.01;
    double m_alpha = 0.0;
    VAOnePoleFilter m_LPF1, m_LPF2, m_HPF1, m_HPF2;
    bool m_is_lowpass = true;
};

// Four-pole ladder family. Modes correspond to the imported LP4, LP2,
// BP4, BP2, HP4 and HP2 response types.
class LadderFilter final : public HybridFilterBase {
public:
    enum class FilterType { LP4 = 0, LP2 = 1, BP4 = 2, BP2 = 3, HP4 = 4, HP2 = 5 };

    LadderFilter() noexcept {
        m_LPF1.setLP(); m_LPF2.setLP(); m_LPF3.setLP(); m_LPF4.setLP();
        reset();
    }
    void reset() noexcept override {
        m_LPF1.reset(); m_LPF2.reset(); m_LPF3.reset(); m_LPF4.reset();
    }
    void setResControl(double resonance) noexcept override {
        m_k = 3.88 * resonance;
        m_last_freq_modded = -1.0;
    }
    void setSampleRate(double rate) noexcept override {
        HybridFilterBase::setSampleRate(rate);
        m_LPF1.setSampleRate(rate); m_LPF2.setSampleRate(rate);
        m_LPF3.setSampleRate(rate); m_LPF4.setSampleRate(rate);
        m_last_freq_modded = -1.0;
    }
    void update() noexcept override {
        HybridFilterBase::update();
        if (m_last_freq_modded == m_freq_modded && !(*m_res_mod)) return;
        m_last_freq_modded = m_freq_modded;

        m_k_modded = std::clamp(m_k + 4.0 * (*m_res_mod), 0.0, 3.88);
        const double wd = 2.0 * kPi * m_freq_modded;
        const double wa = (2.0 * m_samplerate) * std::tan(wd * m_one_over_samplerate * 0.5);
        const double g = wa * m_one_over_samplerate * 0.5;
        const double G = g / (1.0 + g);

        m_LPF1.m_alpha = G; m_LPF2.m_alpha = G; m_LPF3.m_alpha = G; m_LPF4.m_alpha = G;
        m_LPF1.m_beta = G * G * G / (1.0 + g);
        m_LPF2.m_beta = G * G / (1.0 + g);
        m_LPF3.m_beta = G / (1.0 + g);
        m_LPF4.m_beta = 1.0 / (1.0 + g);
        m_gamma = G * G * G * G;
        m_alpha_0 = 1.0 / (1.0 + m_k_modded * m_gamma);

        switch (m_filter_type) {
            case FilterType::LP4: m_a=0.0; m_b=0.0;  m_c=0.0;  m_d=0.0;  m_e=1.0; break;
            case FilterType::LP2: m_a=0.0; m_b=0.0;  m_c=1.0;  m_d=0.0;  m_e=0.0; break;
            case FilterType::BP4: m_a=0.0; m_b=0.0;  m_c=4.0;  m_d=-8.0; m_e=4.0; break;
            case FilterType::BP2: m_a=0.0; m_b=2.0;  m_c=-2.0; m_d=0.0;  m_e=0.0; break;
            case FilterType::HP4: m_a=1.0; m_b=-4.0; m_c=6.0;  m_d=-4.0; m_e=1.0; break;
            case FilterType::HP2: m_a=1.0; m_b=-2.0; m_c=1.0;  m_d=0.0;  m_e=0.0; break;
        }
    }
    double doFilter(double input) noexcept override {
        const double sigma = m_LPF1.getFeedbackOutput() + m_LPF2.getFeedbackOutput() +
                             m_LPF3.getFeedbackOutput() + m_LPF4.getFeedbackOutput();
        const double u = (input - m_k_modded * sigma) * m_alpha_0;
        const double lp1 = m_LPF1.doFilter(u);
        const double lp2 = m_LPF2.doFilter(lp1);
        const double lp3 = m_LPF3.doFilter(lp2);
        const double lp4 = m_LPF4.doFilter(lp3);
        double output = m_a * u + m_b * lp1 + m_c * lp2 + m_d * lp3 + m_e * lp4;
        applyOverdrive(output);
        return output;
    }
    void setFilterType(int type) noexcept {
        m_filter_type = static_cast<FilterType>(std::clamp(type, 0, 5));
        m_last_freq_modded = -1.0;
    }

private:
    VAOnePoleFilter m_LPF1, m_LPF2, m_LPF3, m_LPF4;
    FilterType m_filter_type = FilterType::LP4;
    double m_last_freq_modded = -1.0;
    double m_k = 0.0, m_k_modded = 0.0, m_gamma = 0.0, m_alpha_0 = 1.0;
    double m_a = 0.0, m_b = 0.0, m_c = 0.0, m_d = 0.0, m_e = 1.0;
};

// Explicit opt-in filter saturation using the same output law as the imported
// Odin Korg-35 filter: a dry-to-tanh blend below unity, then driven tanh.
// It is a single post-FILT stage so selecting imported algorithms on both
// physical sides does not apply it twice. SAT=0 is an exact bypass.
class OdinKorgFilterSaturationStereo final {
public:
    void reset() noexcept {}
    void setAmount(float rawAmount) noexcept {
        amount_ = std::clamp(rawAmount, 0.0f, 127.0f) * (2.0f / 127.0f);
    }
    bool active() const noexcept { return amount_ > 0.01f; }
    float process(int /*channel*/, float input) const noexcept {
        if (amount_ <= 0.01f) return input;
        const double x = static_cast<double>(input);
        if (amount_ < 1.0f)
            return static_cast<float>(x * (1.0 - amount_)
                                      + amount_ * std::tanh(3.0 * x));
        return static_cast<float>(std::tanh(static_cast<double>(amount_) * 3.0 * x));
    }
private:
    float amount_ = 0.0f;
};

// Oversampled Clamp/Fold/Zero saturation state.
class OversamplingDistortion final {
public:
    enum DistortionAlgorithm { Clamp = 1, Fold = 2, Zero = 3, Sine = 4, Cube = 5 };

    void setThreshold(float threshold) noexcept {
        threshold = 1.0f - threshold;
        m_threshold = threshold * threshold * threshold;
    }
    void setAlgorithm(int algorithm) noexcept { m_algorithm = static_cast<DistortionAlgorithm>(algorithm); }
    void setDryWet(float dryWet) noexcept { m_drywet = dryWet; }
    void setThresholdModPointer(float* pointer) noexcept { m_threshold_mod = pointer ? pointer : &m_dummy_zero; }
    void setDryWetModPointer(float* pointer) noexcept { m_drywet_mod = pointer ? pointer : &m_dummy_zero; }
    void reset() noexcept {
        xv.fill(0.0); yv.fill(0.0); m_last_input = 0.0; m_threshold_smooth = m_threshold;
    }

    double doDistortion(double input) noexcept {
        double up[3] = {0.66666666 * m_last_input + 0.33333333 * input,
                        0.33333333 * m_last_input + 0.66666666 * input,
                        input};
        m_last_input = input;
        m_threshold_smooth = m_threshold_smooth * kThresholdSmoothing + (1.0f - kThresholdSmoothing) * m_threshold;
        float threshold = (m_threshold_smooth - *m_threshold_mod) * (1.0f - kThresholdMinimum) + kThresholdMinimum;
        threshold = std::clamp(threshold, kThresholdMinimum, 1.0f);

        switch (m_algorithm) {
            case Clamp:
                for (double& sample : up) {
                    if (sample > m_bias && sample > m_bias + threshold) sample = m_bias + threshold;
                    else if (sample < m_bias && sample < m_bias - threshold) sample = m_bias - threshold;
                }
                break;
            case Zero: {
                const float zeroThreshold = 0.5f + threshold * 0.5f;
                for (double& sample : up) {
                    if (sample > m_bias && sample > m_bias + zeroThreshold) sample = 0.0;
                    else if (sample < m_bias && sample < m_bias - zeroThreshold) sample = 0.0;
                }
                threshold = zeroThreshold;
                break;
            }
            case Sine:
                for (double& sample : up) sample = std::sin(sample);
                [[fallthrough]];
            case Cube:
                for (double& sample : up) sample *= sample * sample;
                break;
            case Fold:
                for (double& sample : up) {
                    while (std::abs(sample) > threshold) {
                        sample = sample > threshold ? 2.0 * threshold - sample : -2.0 * threshold - sample;
                    }
                }
                break;
            default: break;
        }

        // The original applies this ninth-order 3x downsampling filter once to
        // each interpolated sample.  Coefficients are retained verbatim.
        for (double sample : up) downsample(sample);
        const float dryWet = std::clamp(m_drywet + *m_drywet_mod, 0.0f, 1.0f);
        switch (m_algorithm) {
            case Clamp:
            case Fold:
            case Zero:
                return yv[9] * dryWet / threshold + input * (1.0f - dryWet);
            case Sine:
            case Cube:
                return yv[9] * dryWet + input * (1.0f - dryWet);
            default:
                return input;
        }
    }

private:
    void downsample(double input) noexcept {
        for (int i = 0; i < 9; ++i) { xv[static_cast<size_t>(i)] = xv[static_cast<size_t>(i + 1)]; yv[static_cast<size_t>(i)] = yv[static_cast<size_t>(i + 1)]; }
        xv[9] = input * 0.019966841051093;
        yv[9] = (xv[0] + xv[9]) + 9.0 * (xv[1] + xv[8]) + 36.0 * (xv[2] + xv[7]) + 84.0 * (xv[3] + xv[6]) +
                126.0 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) +
                (-0.0476997403 * yv[2]) + (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) +
                (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) + (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);
    }

    float m_dummy_zero = 0.0f;
    float* m_threshold_mod = &m_dummy_zero;
    float* m_drywet_mod = &m_dummy_zero;
    DistortionAlgorithm m_algorithm = Clamp;
    double m_last_input = 0.0;
    float m_bias = 0.0f;
    float m_threshold = 0.343f;
    float m_threshold_smooth = 0.343f;
    float m_drywet = 1.0f;
    std::array<double, 10> xv{};
    std::array<double, 10> yv{};
};

// Stereo adapters only set the original classes' public controls and keep a
// separate state machine for each channel.  They do not create a replacement
// Nova filter or saturation approximation.
class Korg35Stereo final {
public:
    // mode: 1 = Korg-35 LP, 2 = Korg-35 HP. Zero is handled by the caller.
    // Imported coefficients are ramped for 10 ms at the actual host sample
    // rate. Native FilterCore and OLD are not touched by this de-click path.
    void prepare(double sampleRate) noexcept {
        controlRampSamples = physicalControlRampSamples(sampleRate);
        for (auto& filter : filters) {
            filter.setFreqModPointer(&zero);
            filter.setResModPointer(&zero);
            filter.setVelModPointer(&zero);
            filter.setKbdModPointer(&zero);
            filter.setSaturationModPointer(&zero);
            filter.setEnvModPointer(&zero);
            filter.setSampleRate(sampleRate);
            filter.reset();
        }
        configured = false; rampRemaining = 0;
    }
    void reset() noexcept { for (auto& filter : filters) filter.reset(); configured = false; rampRemaining = 0; }
    void configure(int mode, float cutoffHz, float resonance) noexcept {
        const int nextMode = mode == 1 ? 1 : 2;
        const float nextCutoff = std::clamp(cutoffHz, static_cast<float>(kFilterFcMin), static_cast<float>(kFilterFcMax));
        const float nextResonance = std::clamp(resonance, 0.0f, 1.0f);
        if (!configured || nextMode != targetMode) {
            if (configured && nextMode != targetMode)
                for (auto& filter : filters) filter.reset();
            targetMode = nextMode;
            targetCutoff = currentCutoff = nextCutoff;
            targetResonance = currentResonance = nextResonance;
            configured = true; rampRemaining = 0; applyControls();
        } else if (std::abs(nextCutoff - targetCutoff) > 1.0e-4f
                   || std::abs(nextResonance - targetResonance) > 1.0e-5f) {
            targetCutoff = nextCutoff;
            targetResonance = nextResonance;
            rampRemaining = controlRampSamples;
        }
    }
    float process(int channel, float input) noexcept {
        if ((channel & 1) == 0) advanceControls();
        return static_cast<float>(filters[static_cast<size_t>(channel & 1)].doFilter(input));
    }

private:
    void advanceControls() noexcept {
        if (rampRemaining <= 0) return;
        currentCutoff += (targetCutoff - currentCutoff) / static_cast<float>(rampRemaining);
        currentResonance += (targetResonance - currentResonance) / static_cast<float>(rampRemaining);
        --rampRemaining;
        applyControls();
    }
    void applyControls() noexcept {
        const bool lowpass = targetMode == 1;
        for (auto& filter : filters) {
            filter.setFilterType(lowpass);
            filter.m_freq_base = currentCutoff;
            filter.m_overdrive = 0.0f;
            filter.setResControl(currentResonance);
            filter.update();
        }
    }
    float zero = 0.0f;
    std::array<Korg35Filter, 2> filters{};
    int targetMode = 1, rampRemaining = 0;
    int controlRampSamples = physicalControlRampSamples(44100.0);
    float targetCutoff = 1000.0f, currentCutoff = 1000.0f;
    float targetResonance = 0.0f, currentResonance = 0.0f;
    bool configured = false;
};

class LadderStereo final {
public:
    // Hybrid mode 3..8 maps to LP24, LP12, BP24, BP12, HP24, HP12.
    // As for K35, only imported coefficients are ramped for 10 ms at the
    // actual host sample rate.
    void prepare(double sampleRate) noexcept {
        controlRampSamples = physicalControlRampSamples(sampleRate);
        for (auto& filter : filters) {
            filter.setFreqModPointer(&zero);
            filter.setResModPointer(&zero);
            filter.setVelModPointer(&zero);
            filter.setKbdModPointer(&zero);
            filter.setSaturationModPointer(&zero);
            filter.setEnvModPointer(&zero);
            filter.setSampleRate(sampleRate);
            filter.reset();
        }
        configured = false; rampRemaining = 0;
    }
    void reset() noexcept { for (auto& filter : filters) filter.reset(); configured = false; rampRemaining = 0; }
    void configure(int hybridMode, float cutoffHz, float resonance) noexcept {
        const int nextType = std::clamp(hybridMode - 3, 0, 5);
        const float nextCutoff = std::clamp(cutoffHz, static_cast<float>(kFilterFcMin), static_cast<float>(kFilterFcMax));
        const float nextResonance = std::clamp(resonance, 0.0f, 1.0f);
        if (!configured || nextType != targetType) {
            if (configured && nextType != targetType)
                for (auto& filter : filters) filter.reset();
            targetType = nextType;
            targetCutoff = currentCutoff = nextCutoff;
            targetResonance = currentResonance = nextResonance;
            configured = true; rampRemaining = 0; applyControls();
        } else if (std::abs(nextCutoff - targetCutoff) > 1.0e-4f
                   || std::abs(nextResonance - targetResonance) > 1.0e-5f) {
            targetCutoff = nextCutoff;
            targetResonance = nextResonance;
            rampRemaining = controlRampSamples;
        }
    }
    float process(int channel, float input) noexcept {
        if ((channel & 1) == 0) advanceControls();
        return static_cast<float>(filters[static_cast<size_t>(channel & 1)].doFilter(input));
    }

private:
    void advanceControls() noexcept {
        if (rampRemaining <= 0) return;
        currentCutoff += (targetCutoff - currentCutoff) / static_cast<float>(rampRemaining);
        currentResonance += (targetResonance - currentResonance) / static_cast<float>(rampRemaining);
        --rampRemaining;
        applyControls();
    }
    void applyControls() noexcept {
        for (auto& filter : filters) {
            filter.setFilterType(targetType);
            filter.m_freq_base = currentCutoff;
            filter.m_overdrive = 0.0f;
            filter.setResControl(currentResonance);
            filter.update();
        }
    }
    float zero = 0.0f;
    std::array<LadderFilter, 2> filters{};
    int targetType = 0, rampRemaining = 0;
    int controlRampSamples = physicalControlRampSamples(44100.0);
    float targetCutoff = 1000.0f, currentCutoff = 1000.0f;
    float targetResonance = 0.0f, currentResonance = 0.0f;
    bool configured = false;
};

// The user-supplied 303/MS20/Moog sources require a Filter.h framework that is
// not present in the source package. RClassicFilters.hpp is the self-contained,
// attributed adaptation of their state equations. This bridge supplies a
// separate instance per stereo channel and ramps control targets rather than
// rewriting nonlinear coefficients abruptly at a host callback boundary.
inline reference_filters::ReferenceMode referenceModeForHybrid(int hybridMode) noexcept
{
    using reference_filters::ReferenceMode;
    switch (hybridMode) {
        case hybridR303Bp:     return ReferenceMode::TB303BP;
        case hybridR303Hp:     return ReferenceMode::TB303HP;
        case hybridRMs20Lp:    return ReferenceMode::MS20LP;
        case hybridRMs20Bp:    return ReferenceMode::MS20BP;
        case hybridRMs20Hp:    return ReferenceMode::MS20HP;
        case hybridRMoogLp24:  return ReferenceMode::MoogLP24;
        case hybridRMoogLp12:  return ReferenceMode::MoogLP12;
        case hybridRMoogBp24:  return ReferenceMode::MoogBP24;
        case hybridRMoogBp12:  return ReferenceMode::MoogBP12;
        case hybridRMoogHp24:  return ReferenceMode::MoogHP24;
        case hybridRMoogHp12:  return ReferenceMode::MoogHP12;
        case hybridR303Lp:
        default:               return ReferenceMode::TB303LP;
    }
}

class ReferenceClassicStereo final {
public:
    // setSampleRate() in RealFilterCore calls prepare only when the host rate
    // really changes. No callback-level reprepare/reset is hidden here.
    void prepare(double sampleRate) noexcept {
        controlRampSamples = physicalControlRampSamples(sampleRate);
        for (auto& filter : filters) filter.prepare(sampleRate);
        configured = false;
        rampRemaining = 0;
    }

    void reset() noexcept {
        for (auto& filter : filters) filter.reset();
        configured = false;
        rampRemaining = 0;
    }

    void configure(int hybridMode, float cutoffHz, float resonance) noexcept {
        const int mode = std::clamp(hybridMode, static_cast<int>(hybridR303Lp),
                                    static_cast<int>(hybridRMoogHp12));
        const float cutoff = std::clamp(cutoffHz, static_cast<float>(kFilterFcMin), static_cast<float>(kFilterFcMax));
        const float q = std::clamp(resonance, 0.0f, 1.0f);

        if (!configured || mode != currentMode) {
            // RealFilterCore has already reset on a mode change. Reset here as
            // well to make this adapter safe if it is used independently.
            if (configured && mode != currentMode)
                for (auto& filter : filters) filter.reset();
            currentMode = mode;
            targetCutoff = currentCutoff = cutoff;
            targetResonance = currentResonance = q;
            configured = true;
            rampRemaining = 0;
            applyControls();
            return;
        }

        // Compare with the previous target (not the still-ramping current
        // value). Repeating an unchanged host snapshot cannot restart a ramp.
        if (std::abs(cutoff - targetCutoff) > 1.0e-4f
            || std::abs(q - targetResonance) > 1.0e-5f) {
            targetCutoff = cutoff;
            targetResonance = q;
            rampRemaining = controlRampSamples;
        }
    }

    float process(int channel, float input) noexcept {
        if ((channel & 1) == 0) advanceControls();
        const float output = filters[static_cast<size_t>(channel & 1)].process(input);
        return std::isfinite(output) ? output : 0.0f;
    }

private:
    void advanceControls() noexcept {
        if (rampRemaining <= 0) return;
        currentCutoff += (targetCutoff - currentCutoff) / static_cast<float>(rampRemaining);
        currentResonance += (targetResonance - currentResonance) / static_cast<float>(rampRemaining);
        --rampRemaining;
        applyControls();
    }

    void applyControls() noexcept {
        const auto mode = referenceModeForHybrid(currentMode);
        for (auto& filter : filters)
            filter.configure(mode, currentCutoff, currentResonance);
    }

    std::array<reference_filters::ReferenceCore, 2> filters{};
    int currentMode = hybridR303Lp;
    int rampRemaining = 0;
    int controlRampSamples = physicalControlRampSamples(44100.0);
    float targetCutoff = 1000.0f, currentCutoff = 1000.0f;
    float targetResonance = 0.0f, currentResonance = 0.0f;
    bool configured = false;
};

// The separately audited `for import/2` collection had missing framework
// dependencies and mixed provenance.  RImport2Filters.hpp is the user-selected
// independent, self-contained equivalent bundle.  IDs 21..57 are append-only
// and are intentionally handled by a distinct adapter from 1.9.9 R-classic.
inline reference_import2::Import2Mode import2ModeForHybrid(int hybridMode) noexcept
{
    using reference_import2::Import2Mode;
    switch (hybridMode) {
        case hybridRAnalogLp24: return Import2Mode::AnalogLp24;
        case hybridRAnalogBp12: return Import2Mode::AnalogBp12;
        case hybridRAnalogBp24: return Import2Mode::AnalogBp24;
        case hybridRAnalogHp12: return Import2Mode::AnalogHp12;
        case hybridRAnalogHp24: return Import2Mode::AnalogHp24;
        case hybridRLinearLp12: return Import2Mode::LinearLp12;
        case hybridRLinearLp24: return Import2Mode::LinearLp24;
        case hybridRLinearBp12: return Import2Mode::LinearBp12;
        case hybridRLinearBp24: return Import2Mode::LinearBp24;
        case hybridRLinearHp12: return Import2Mode::LinearHp12;
        case hybridRLinearHp24: return Import2Mode::LinearHp24;
        case hybridRRbjLp: return Import2Mode::RbjLp;
        case hybridRRbjBp: return Import2Mode::RbjBp;
        case hybridRRbjHp: return Import2Mode::RbjHp;
        case hybridRTptLp: return Import2Mode::TptLp;
        case hybridRTptBp: return Import2Mode::TptBp;
        case hybridRTptHp: return Import2Mode::TptHp;
        case hybridRHuovilainenLp4: return Import2Mode::HuovilainenLp4;
        case hybridRHyperionLp2: return Import2Mode::HyperionLp2;
        case hybridRHyperionLp4: return Import2Mode::HyperionLp4;
        case hybridRHyperionBp2: return Import2Mode::HyperionBp2;
        case hybridRHyperionBp4: return Import2Mode::HyperionBp4;
        case hybridRHyperionHp2: return Import2Mode::HyperionHp2;
        case hybridRHyperionHp4: return Import2Mode::HyperionHp4;
        case hybridRHyperionNotch: return Import2Mode::HyperionNotch;
        case hybridRKrajeskiLp4: return Import2Mode::KrajeskiLp4;
        case hybridRMicrotrackerLp4: return Import2Mode::MicrotrackerLp4;
        case hybridRMusicLp4: return Import2Mode::MusicLp4;
        case hybridROberheimLp4: return Import2Mode::OberheimLp4;
        case hybridRDvalLp4: return Import2Mode::DvalLp4;
        case hybridRHuovilainenHp4: return Import2Mode::HuovilainenHp4;
        case hybridRKrajeskiHp4: return Import2Mode::KrajeskiHp4;
        case hybridRMicrotrackerHp4: return Import2Mode::MicrotrackerHp4;
        case hybridRMusicHp4: return Import2Mode::MusicHp4;
        case hybridROberheimHp4: return Import2Mode::OberheimHp4;
        case hybridRDvalHp4: return Import2Mode::DvalHp4;
        case hybridRAnalogLp12:
        default: return Import2Mode::AnalogLp12;
    }
}

class ReferenceImport2Stereo final {
public:
    void prepare(double sampleRate) noexcept {
        controlRampSamples = physicalControlRampSamples(sampleRate);
        for (auto& filter : filters) filter.prepare(sampleRate);
        configured = false;
        rampRemaining = 0;
    }

    void reset() noexcept {
        for (auto& filter : filters) filter.reset();
        configured = false;
        rampRemaining = 0;
    }

    void configure(int hybridMode, float cutoffHz, float resonance) noexcept {
        const int mode = std::clamp(hybridMode, static_cast<int>(kHybridRImport2FirstAlgorithm),
                                    static_cast<int>(kHybridRImport2LastAlgorithm));
        const float cutoff = std::clamp(cutoffHz, static_cast<float>(kFilterFcMin), static_cast<float>(kFilterFcMax));
        const float q = std::clamp(resonance, 0.0f, 1.0f);
        if (!configured || mode != currentMode) {
            if (configured && mode != currentMode)
                for (auto& filter : filters) filter.reset();
            currentMode = mode;
            targetCutoff = currentCutoff = cutoff;
            targetResonance = currentResonance = q;
            configured = true;
            rampRemaining = 0;
            applyControls();
            return;
        }
        if (std::abs(cutoff - targetCutoff) > 1.0e-4f
            || std::abs(q - targetResonance) > 1.0e-5f) {
            targetCutoff = cutoff;
            targetResonance = q;
            rampRemaining = controlRampSamples;
        }
    }

    float process(int channel, float input) noexcept {
        if ((channel & 1) == 0) advanceControls();
        const float output = filters[static_cast<size_t>(channel & 1)].process(input);
        return std::isfinite(output) ? output : 0.0f;
    }

private:
    void advanceControls() noexcept {
        if (rampRemaining <= 0) return;
        currentCutoff += (targetCutoff - currentCutoff) / static_cast<float>(rampRemaining);
        currentResonance += (targetResonance - currentResonance) / static_cast<float>(rampRemaining);
        --rampRemaining;
        applyControls();
    }
    void applyControls() noexcept {
        const auto mode = import2ModeForHybrid(currentMode);
        for (auto& filter : filters) filter.configure(mode, currentCutoff, currentResonance);
    }
    std::array<reference_import2::Import2Core, 2> filters{};
    int currentMode = hybridRAnalogLp12;
    int rampRemaining = 0;
    int controlRampSamples = physicalControlRampSamples(44100.0);
    float targetCutoff = 1000.0f, currentCutoff = 1000.0f;
    float targetResonance = 0.0f, currentResonance = 0.0f;
    bool configured = false;
};

// A deliberately separate A/B candidate for the reported 3.5 kHz MNM-DIST
// notch. This is not a claim about the unresolved machine-handler transfer.
// It is blended only above DIST=64, retaining exact MNM below/at neutral.
class MnmFixNotchCompensatorStereo final {
public:
    static constexpr float kReportedNotchHz = 3500.0f;
    static constexpr float kStartingGainDb = 3.0f;
    static constexpr float kQ = 1.0f;
    void prepare(double sampleRate) noexcept {
        sampleRateHz = std::max(1000.0, sampleRate);
        for (auto& filter : filters) filter.setPeak(sampleRateHz, kReportedNotchHz, kQ, kStartingGainDb);
        reset();
    }
    void reset() noexcept { for (auto& filter : filters) filter.reset(); }
    float process(int channel, float input, float rawAmount) noexcept {
        const float amount = std::clamp((rawAmount - 64.0f) / 64.0f, 0.0f, 1.0f);
        const float corrected = filters[static_cast<size_t>(channel & 1)].process(input);
        return input + amount * (corrected - input);
    }
private:
    struct Peak {
        void reset() noexcept { z1 = z2 = 0.0; }
        void setPeak(double sampleRate, float frequency, float q, float gainDb) noexcept {
            const double w = 2.0 * kPi * std::clamp(static_cast<double>(frequency), kFilterFcMin, sampleRate * 0.45) / sampleRate;
            const double a = std::pow(10.0, static_cast<double>(gainDb) / 40.0);
            const double alpha = std::sin(w) / (2.0 * std::max(0.05, static_cast<double>(q)));
            const double a0 = 1.0 + alpha / a;
            b0 = (1.0 + alpha * a) / a0; b1 = -2.0 * std::cos(w) / a0; b2 = (1.0 - alpha * a) / a0;
            a1 = -2.0 * std::cos(w) / a0; a2 = (1.0 - alpha / a) / a0;
        }
        float process(float input) noexcept {
            const double output = b0 * input + z1;
            z1 = b1 * input - a1 * output + z2;
            z2 = b2 * input - a2 * output;
            return static_cast<float>(output);
        }
        double b0 = 1.0, b1 = 0.0, b2 = 0.0, a1 = 0.0, a2 = 0.0, z1 = 0.0, z2 = 0.0;
    };
    double sampleRateHz = 44100.0;
    std::array<Peak, 2> filters{};
};

class OversamplingDistortionStereo final {
public:
    void reset() noexcept { for (auto& distortion : distortions) distortion.reset(); }
    // mode: 1 = Fold, 2 = Zero, 3 = Clamp.
    void setMode(int mode) noexcept {
        const int algorithm = mode == 1 ? OversamplingDistortion::Fold : mode == 2 ? OversamplingDistortion::Zero : OversamplingDistortion::Clamp;
        for (auto& distortion : distortions) {
            distortion.setThresholdModPointer(&zero);
            distortion.setDryWetModPointer(&zero);
            distortion.setDryWet(1.0f);
            distortion.setAlgorithm(algorithm);
        }
    }
    float process(int channel, float input, float rawAmount) noexcept {
        // The threshold setter deliberately inverts the control: 0 = the
        // highest threshold, 127 = the most aggressive saturation.
        distortions[static_cast<size_t>(channel & 1)].setThreshold(std::clamp(rawAmount, 0.0f, 127.0f) / 127.0f);
        return static_cast<float>(distortions[static_cast<size_t>(channel & 1)].doDistortion(input));
    }
    // Preserve the native MNM lower half of the DIST knob exactly. Above the
    // neutral point, retain only its explicit pre-drive convention and feed
    // the imported algorithm directly. There is deliberately no synthetic
    // MNM crossfade and no common post-level law: Fold, Zero and Clamp have
    // different original output behaviour, so a shared compensation changes
    // their transfers rather than preserving them.
    float processMnmPreDriven(int channel, float input, float rawAmount, float retainedMnmOutput) noexcept {
        const float clamped = std::clamp(rawAmount, 0.0f, 127.0f);
        if (clamped <= 64.0f) {
            // Keep the oversampling state warm at its direct-unity endpoint,
            // while returning the exact retained MNM attenuation/unity sound.
            (void) process(channel, input, 0.0f);
            return retainedMnmOutput;
        }
        const float amount = (clamped - 64.0f) / 64.0f;
        const float driven = input * (1.0f + amount * 15.0f);
        return process(channel, driven, amount * 127.0f);
    }

private:
    float zero = 0.0f;
    std::array<OversamplingDistortion, 2> distortions{};
};

} // namespace nova::hybrid_private
