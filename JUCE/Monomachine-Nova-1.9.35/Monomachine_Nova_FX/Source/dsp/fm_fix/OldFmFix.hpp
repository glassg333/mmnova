// OldFmFix.hpp — isolated OLD topology with measured FM+ FIX controls.
//
// Retained monomachine_fm_par.hpp and monomachine_fm_dynamic.hpp remain
// untouched. These copies preserve their old audio topology/envelopes and
// replace only frequency/TUNE control laws for MODE SYNT = old fix.
#pragma once

#include "FmFixTables.hpp"
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace fm_fix {

// m8 restores the archived OLD STATIC topology (separate from MNM) and
// feeds it only the measured FIX frequency/TUNE laws.  The retained `old`
// route remains unchanged; this class exists exclusively for `old fix`.
class OldFixStaticCore {
public:
    OldFixStaticCore() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = 0.0f;
        m_phaseMod1 = 0.0f;
        m_phaseMod2 = 0.0f;
        m_mod1Feedback = 0.0f;
        m_envMod1 = 0.0f;
    }

    void setParameters(uint8_t frq1, uint8_t fin1, uint8_t env1, uint8_t fb1,
                       uint8_t frq2, uint8_t vol2, uint8_t tone, uint8_t tune) {
        // The archived OLD STATIC renderer used listed ratios.  FIX replaces
        // just the two ratio fetches and master TUNE law with the measured
        // profile; its serial two-modulator topology is otherwise retained.
        m_ratioMod1 = statParRatioForRaw(frq1);
        m_fineDetune1 = (static_cast<float>(fin1) - 64.0f) * (0.05f / 64.0f);
        m_mod1Index = (static_cast<float>(env1) / 127.0f) * 6.0f;
        m_feedback1 = (static_cast<float>(fb1) / 127.0f) * 1.2f;
        m_ratioMod2 = statParRatioForRaw(frq2);
        m_mod2Index = (static_cast<float>(vol2) / 127.0f) * 5.0f;
        m_toneParam = tone; // OLD STATIC stored this word; tonal integration is outside the core.
        m_tuneSemitones = tuneSemitonesForRaw(tune);
        updateFrequencies();
    }

    void noteOn(uint8_t note, uint8_t velocity = 127) {
        (void)velocity;
        m_midiNote = note;
        m_envMod1 = 1.0f;
        updateFrequencies();
    }

    void noteOff() {}

    void processStereo(float* outL, float* outR, size_t numFrames) {
        constexpr float twoPi = 6.28318530717958647692f;
        const float sampleRateInv = 1.0f / static_cast<float>(m_sampleRate);
        const float envDecay = std::exp(-1.0f / (0.025f * static_cast<float>(m_sampleRate)));

        for (size_t i = 0; i < numFrames; ++i) {
            m_envMod1 *= envDecay;

            const float mod2PhaseInc = twoPi * (m_carrierFreq * m_ratioMod2) * sampleRateInv;
            const float mod2Out = std::sin(m_phaseMod2);
            m_phaseMod2 += mod2PhaseInc;
            m_phaseMod2 -= twoPi * std::floor(m_phaseMod2 / twoPi);

            const float mod1RatioEffective = m_ratioMod1 * (1.0f + m_fineDetune1);
            const float mod1PhaseInc = twoPi * (m_carrierFreq * mod1RatioEffective) * sampleRateInv;
            const float mod1In = m_phaseMod1 + mod2Out * m_mod2Index + m_mod1Feedback * m_feedback1;
            const float mod1Out = std::sin(mod1In);
            m_mod1Feedback = mod1Out;
            m_phaseMod1 += mod1PhaseInc;
            m_phaseMod1 -= twoPi * std::floor(m_phaseMod1 / twoPi);

            const float carrierPhaseInc = twoPi * m_carrierFreq * sampleRateInv;
            const float carrierOut = std::sin(m_phaseCarrier + mod1Out * m_mod1Index * m_envMod1);
            m_phaseCarrier += carrierPhaseInc;
            m_phaseCarrier -= twoPi * std::floor(m_phaseCarrier / twoPi);

            const float output = std::tanh(carrierOut);
            outL[i] = output;
            outR[i] = output;
        }
    }

    void setPitchBend(float semitones) { m_pitchBend = semitones; updateFrequencies(); }

private:
    void updateFrequencies() {
        const float effectivePitch = static_cast<float>(m_midiNote) + m_tuneSemitones + m_pitchBend;
        m_carrierFreq = 440.0f * std::pow(2.0f, (effectivePitch - 69.0f) / 12.0f);
    }

    float m_pitchBend = 0.0f;
    double m_sampleRate = 44100.0;
    uint8_t m_midiNote = 60;
    float m_carrierFreq = 261.63f;
    float m_tuneSemitones = 0.0f;
    float m_ratioMod1 = 1.0f;
    float m_fineDetune1 = 0.0f;
    float m_mod1Index = 2.0f;
    float m_feedback1 = 0.0f;
    float m_ratioMod2 = 2.0f;
    float m_mod2Index = 1.0f;
    uint8_t m_toneParam = 64;
    float m_phaseCarrier = 0.0f;
    float m_phaseMod1 = 0.0f;
    float m_phaseMod2 = 0.0f;
    float m_mod1Feedback = 0.0f;
    float m_envMod1 = 0.0f;
};

// m9 uses the pre-existing OLD parallel renderer/topology.
class OldFixParallelCore {
public:
    OldFixParallelCore() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = 0.0f;
        m_phaseMod1 = 0.0f;
        m_phaseMod2 = 0.0f;
        m_phaseMod3 = 0.0f;
        m_env1 = 0.0f;
        m_env2 = 0.0f;
        m_env3 = 0.0f;
        m_indexSmoothingPrimed = false;
    }

    void setParameters(uint8_t frq1, uint8_t env1, uint8_t frq2, uint8_t env2,
                       uint8_t frq3, uint8_t env3, uint8_t tone, uint8_t tune) {
        // Retain OLD parallel topology/envelopes while feeding the measured
        // FIX frequency/tune laws directly rather than translating to OLD raw.
        m_ratioMod1 = statParRatioForRaw(frq1);
        m_index1Target = (static_cast<float>(env1) / 127.0f) * 4.0f;

        m_ratioMod2 = statParRatioForRaw(frq2);
        m_index2Target = (static_cast<float>(env2) / 127.0f) * 4.0f;

        m_ratioMod3 = statParRatioForRaw(frq3);
        m_index3Target = (static_cast<float>(env3) / 127.0f) * 4.0f;

        // Prime only the initial post-reset value. Subsequent 1ENV/2ENV/3ENV
        // writes are smoothed in processStereo, so an FM phase offset cannot
        // jump at a control-block boundary and make a click/crackle.
        if (!m_indexSmoothingPrimed) {
            m_index1 = m_index1Target;
            m_index2 = m_index2Target;
            m_index3 = m_index3Target;
            m_indexSmoothingPrimed = true;
        }

        m_toneParam = tone;
        m_tuneSemitones = tuneSemitonesForRaw(tune);
        updateFrequencies();
    }

    void noteOn(uint8_t note, uint8_t velocity = 127) {
        (void)velocity;
        m_midiNote = note;
        m_env1 = 1.0f;
        m_env2 = 1.0f;
        m_env3 = 1.0f;
        updateFrequencies();
    }

    void noteOff() {}

    void processStereo(float* outL, float* outR, size_t numFrames) {
        constexpr float twoPi = 2.0f * 3.14159265358979323846f;
        const float sampleRateInv = 1.0f / static_cast<float>(m_sampleRate);

        // 1.6.5: медленные огибающие (0.25/0.45/0.65 с) вместо 20/40/60 мс.
        const float decay1 = std::exp(-1.0f / (0.25f * static_cast<float>(m_sampleRate)));
        const float decay2 = std::exp(-1.0f / (0.45f * static_cast<float>(m_sampleRate)));
        const float decay3 = std::exp(-1.0f / (0.65f * static_cast<float>(m_sampleRate)));
        // A 12 ms one-pole slew makes the three modulation-index controls
        // continuous while retaining their raw 0..127 endpoints and OLD PAR
        // envelope topology. The coefficient is sample-rate invariant.
        const float indexSlew = 1.0f - std::exp(-1.0f / (0.012f * static_cast<float>(m_sampleRate)));

        for (size_t i = 0; i < numFrames; ++i) {
            m_index1 += (m_index1Target - m_index1) * indexSlew;
            m_index2 += (m_index2Target - m_index2) * indexSlew;
            m_index3 += (m_index3Target - m_index3) * indexSlew;

            m_env1 *= decay1;
            m_env2 *= decay2;
            m_env3 *= decay3;

            // Устойчивый пол 45% + атакующий плик до 100%: модуляция слышна
            // всю ноту, поэтому 1FRQ/2FRQ/3FRQ меняют тембр, а не только атаку.
            const float depth1 = m_index1 * (kSustain + (1.0f - kSustain) * m_env1);
            const float depth2 = m_index2 * (kSustain + (1.0f - kSustain) * m_env2);
            const float depth3 = m_index3 * (kSustain + (1.0f - kSustain) * m_env3);

            const float mod1Out = std::sin(m_phaseMod1) * depth1;
            const float mod2Out = std::sin(m_phaseMod2) * depth2;
            const float mod3Out = std::sin(m_phaseMod3) * depth3;

            m_phaseMod1 += twoPi * (m_carrierFreq * m_ratioMod1) * sampleRateInv;
            m_phaseMod2 += twoPi * (m_carrierFreq * m_ratioMod2) * sampleRateInv;
            m_phaseMod3 += twoPi * (m_carrierFreq * m_ratioMod3) * sampleRateInv;

            m_phaseMod1 -= twoPi * std::floor(m_phaseMod1 / twoPi);
            m_phaseMod2 -= twoPi * std::floor(m_phaseMod2 / twoPi);
            m_phaseMod3 -= twoPi * std::floor(m_phaseMod3 / twoPi);

            const float totalMod = mod1Out + mod2Out + mod3Out;
            const float carrierOut = std::sin(m_phaseCarrier + totalMod);

            m_phaseCarrier += twoPi * m_carrierFreq * sampleRateInv;
            m_phaseCarrier -= twoPi * std::floor(m_phaseCarrier / twoPi);

            const float outSample = std::tanh(carrierOut);
            outL[i] = outSample;
            outR[i] = outSample;
        }
    }

    void setPitchBend(float semitones) { m_pitchBend = semitones; updateFrequencies(); }

private:
    static constexpr float kSustain = 0.45f;
    float m_pitchBend = 0.0f;
    void updateFrequencies() {
        const float effectivePitch = static_cast<float>(m_midiNote) + m_tuneSemitones + m_pitchBend;
        m_carrierFreq = 440.0f * std::pow(2.0f, (effectivePitch - 69.0f) / 12.0f);
    }

    double m_sampleRate = 44100.0;
    uint8_t m_midiNote = 60;
    float m_carrierFreq = 261.63f;
    float m_tuneSemitones = 0.0f;

    float m_ratioMod1 = 1.0f;
    float m_index1 = 1.0f;
    float m_index1Target = 1.0f;
    float m_ratioMod2 = 2.0f;
    float m_index2 = 1.0f;
    float m_index2Target = 1.0f;
    float m_ratioMod3 = 3.0f;
    float m_index3 = 1.0f;
    float m_index3Target = 1.0f;
    bool m_indexSmoothingPrimed = false;
    uint8_t m_toneParam = 64;

    float m_phaseCarrier = 0.0f;
    float m_phaseMod1 = 0.0f;
    float m_phaseMod2 = 0.0f;
    float m_phaseMod3 = 0.0f;
    float m_env1 = 0.0f;
    float m_env2 = 0.0f;
    float m_env3 = 0.0f;
};

// m10 uses the pre-existing OLD dynamic renderer/topology.
class OldFixDynamicCore {
public:
    OldFixDynamicCore() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = 0.0f;
        m_phaseMod1 = 0.0f;
        m_phaseMod2 = 0.0f;
        m_feedbackMod2 = 0.0f;
        m_env1F = 0.0f;
        m_env1V = 0.0f;
        m_env2V = 0.0f;
        m_gate = false;
    }

    /**
     * @brief Configure parameters from Monomachine 0..127 knob values.
     */
    void setParameters(uint8_t frq1, uint8_t fen1, uint8_t vol1, uint8_t ven1,
                       uint8_t frq2, uint8_t env2, uint8_t fb2, uint8_t tune) {
        m_paramFrq1 = frq1;
        m_paramFen1 = fen1;
        m_paramVol1 = vol1;
        m_paramVen1 = ven1;
        m_paramFrq2 = frq2;
        m_paramEnv2 = env2;
        m_paramFb2  = fb2;
        m_paramTune = tune;

        // OLD DYN topology, measured FIX 1FRQ control law.
        m_ratioMod1Linear = dynRatio1ForRaw(frq1);

        // 1FEN: Bipolar envelope depth (-1.0 to +1.0)
        m_depthFen1 = (static_cast<float>(fen1) - 64.0f) / 64.0f;

        // 1VOL: Mod 1 volume / FM index (0.0 to ~6.0 radians of phase modulation)
        m_baseIndexMod1 = (static_cast<float>(vol1) / 127.0f) * 6.0f;

        // 1VEN: Mod 1 volume envelope depth
        m_depthVen1 = static_cast<float>(ven1) / 127.0f;

        // OLD DYN topology, measured FIX squared 2FRQ control law.
        m_ratioMod2Exp = dynRatio2ForRaw(frq2);

        // 2ENV: Mod 2 envelope & volume index
        m_baseIndexMod2 = (static_cast<float>(env2) / 127.0f) * 5.0f;

        // 2FB: Mod 2 feedback amount (0.0 to ~1.2)
        m_feedbackAmount = (static_cast<float>(fb2) / 127.0f) * 1.2f;

        // OLD DYN topology, measured FIX ±2-semitone bridge.
        m_tuneSemitones = tuneSemitonesForRaw(tune);

        updateFrequencies();
    }

    /**
     * @brief Trigger note on event with MIDI note (0..127)
     */
    void noteOn(uint8_t midiNote, uint8_t velocity = 127) {
        m_midiNote = midiNote;
        m_velocity = velocity;
        m_gate = true;

        // Reset envelopes on trigger
        m_env1F = 1.0f;
        m_env1V = 1.0f;
        m_env2V = 1.0f;

        updateFrequencies();
    }

    void noteOff() {
        m_gate = false;
    }

    // UI formatting is owned by PluginEditor FIX readouts.

    /**
     * @brief Process audio block of stereo samples.
     */
    void processStereo(float* outL, float* outR, size_t numFrames) {
        constexpr float twoPi = 2.0f * 3.14159265358979323846f;
        const float sampleRateInv = 1.0f / static_cast<float>(m_sampleRate);

        // Exponential decay envelope step rates
        const float decayRate1F = std::exp(-1.0f / (0.01f * static_cast<float>(m_sampleRate)));
        const float decayRate1V = std::exp(-1.0f / (0.03f * static_cast<float>(m_sampleRate)));
        const float decayRate2V = std::exp(-1.0f / (0.02f * static_cast<float>(m_sampleRate)));

        for (size_t i = 0; i < numFrames; ++i) {
            // Envelope steps
            m_env1F *= decayRate1F;
            m_env1V *= decayRate1V;
            m_env2V *= decayRate2V;

            // Modulator 2 (with feedback)
            float mod2PhaseInc = twoPi * (m_carrierFreq * m_ratioMod2Exp) * sampleRateInv;
            float mod2Sample = std::sin(m_phaseMod2 + m_feedbackMod2 * m_feedbackAmount);
            m_feedbackMod2 = mod2Sample;
            m_phaseMod2 += mod2PhaseInc;
            m_phaseMod2 -= twoPi * std::floor(m_phaseMod2 / twoPi);

            // Modulator 1 (with linear ratio + pitch envelope modulation + Mod 2 input)
            float mod1RatioEffective = std::max(0.01f, m_ratioMod1Linear + m_depthFen1 * m_env1F);
            float mod1PhaseInc = twoPi * (m_carrierFreq * mod1RatioEffective) * sampleRateInv;
            float mod2Modulation = mod2Sample * (m_baseIndexMod2 * m_env2V);
            float mod1Sample = std::sin(m_phaseMod1 + mod2Modulation);
            m_phaseMod1 += mod1PhaseInc;
            m_phaseMod1 -= twoPi * std::floor(m_phaseMod1 / twoPi);

            // Carrier (modulated by Modulator 1)
            float carrierPhaseInc = twoPi * m_carrierFreq * sampleRateInv;
            float mod1Modulation = mod1Sample * (m_baseIndexMod1 * (1.0f + m_depthVen1 * (m_env1V - 1.0f)));
            float carrierSample = std::sin(m_phaseCarrier + mod1Modulation);
            m_phaseCarrier += carrierPhaseInc;
            m_phaseCarrier -= twoPi * std::floor(m_phaseCarrier / twoPi);

            // Soft-saturation to emulate Monomachine 24-bit fixed point headroom
            float output = std::tanh(carrierSample);

            outL[i] = output;
            outR[i] = output;
        }
    }

    // Gate and velocity are applied by the plugin ADSR, allowing release tails.
    void setPitchBend(float semitones) { m_pitchBend = semitones; updateFrequencies(); }

private:
    void updateFrequencies() {
        // Base carrier frequency from MIDI note + Tune
        float effectivePitch = static_cast<float>(m_midiNote) + m_tuneSemitones + m_pitchBend;
        m_carrierFreq = 440.0f * std::pow(2.0f, (effectivePitch - 69.0f) / 12.0f);
    }

    float m_pitchBend = 0.0f;
    double m_sampleRate = 44100.0;
    uint8_t m_midiNote = 60;
    uint8_t m_velocity = 127;
    bool m_gate = false;

    // Parameters
    uint8_t m_paramFrq1 = 16;
    uint8_t m_paramFen1 = 64;
    uint8_t m_paramVol1 = 64;
    uint8_t m_paramVen1 = 64;
    uint8_t m_paramFrq2 = 32;
    uint8_t m_paramEnv2 = 80;
    uint8_t m_paramFb2  = 30;
    uint8_t m_paramTune = 64;

    float m_carrierFreq = 261.63f;
    float m_tuneSemitones = 0.0f;
    float m_ratioMod1Linear = 1.0f;
    float m_depthFen1 = 0.0f;
    float m_baseIndexMod1 = 2.0f;
    float m_depthVen1 = 0.5f;
    float m_ratioMod2Exp = 1.33f;
    float m_baseIndexMod2 = 3.0f;
    float m_feedbackAmount = 0.3f;

    // Phases & feedback state
    float m_phaseCarrier = 0.0f;
    float m_phaseMod1 = 0.0f;
    float m_phaseMod2 = 0.0f;
    float m_feedbackMod2 = 0.0f;

    // Envelopes
    float m_env1F = 0.0f;
    float m_env1V = 0.0f;
    float m_env2V = 0.0f;
};

} // namespace fm_fix
} // namespace monomachine
