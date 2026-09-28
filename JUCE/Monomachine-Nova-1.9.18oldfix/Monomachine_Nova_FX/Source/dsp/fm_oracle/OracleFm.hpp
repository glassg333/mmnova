// OracleFm.hpp -- self-contained native FM+ cores for MODE SYNT = oracle.
//
// The oracle mode is intentionally isolated from prior MNM/NEW/OLD/FIX cores.
// Its tables, raw parameter bank, lifecycle and state are private to the
// fm_oracle namespace. The local Monomodule renderer is used only as a test
// oracle; no OS/DSP binary is needed or included at runtime.
#pragma once

#include "OracleFmTables.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace monomachine {
namespace fm_oracle {
namespace detail {

inline constexpr float kPi = 3.14159265358979323846f;
inline constexpr float kTwoPi = 2.0f * kPi;

inline float unit(uint8_t raw) noexcept { return static_cast<float>(raw) * (1.0f / 127.0f); }
inline float bipolar(uint8_t raw) noexcept { return static_cast<float>(raw) * (1.0f / 63.5f) - 1.0f; }
inline float wrapPhase(float phase) noexcept
{
    phase -= kTwoPi * std::floor(phase / kTwoPi);
    return phase;
}
inline float noteHz(float note) noexcept
{
    return 440.0f * std::pow(2.0f, (note - 69.0f) * (1.0f / 12.0f));
}
inline float onePoleCoefficient(float tone, double sampleRate) noexcept
{
    const float cutoff = 120.0f + 18800.0f * tone * tone;
    return 1.0f - std::exp(-kTwoPi * cutoff / static_cast<float>(sampleRate));
}
inline float decayCoefficient(float seconds, double sampleRate) noexcept
{
    return std::exp(-1.0f / (std::max(0.001f, seconds) * static_cast<float>(sampleRate)));
}
inline float applyTone(float input, float coefficient, float& state) noexcept
{
    state += coefficient * (input - state);
    return state;
}

} // namespace detail

// Native independent interpretation of FM+ STAT. Its topology is intentionally
// local to oracle mode: a feedbacked primary modulator plus the independently
// levelled second modulator feed the carrier; TONE is an internal one-pole path.
class OracleStatCore {
public:
    void reset(double sampleRate = 44100.0) noexcept
    {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = m_phaseOne = m_phaseTwo = 0.0f;
        m_feedbackSample = m_toneState = 0.0f;
        m_gateEnvelope = 0.0f;
    }

    void setParameters(uint8_t frq1, uint8_t fin1, uint8_t env1, uint8_t fb1,
                       uint8_t frq2, uint8_t vol2, uint8_t tone, uint8_t tune) noexcept
    {
        m_ratioOne = statParRatioForRaw(frq1);
        m_fineOne = detail::bipolar(fin1) * 0.125f;
        m_indexOne = detail::unit(env1) * 5.0f;
        m_feedback = detail::unit(fb1) * 1.35f;
        m_ratioTwo = statParRatioForRaw(frq2);
        m_indexTwo = detail::unit(vol2) * 4.0f;
        m_toneCoefficient = detail::onePoleCoefficient(detail::unit(tone), m_sampleRate);
        m_tune = tuneSemitonesForRaw(tune);
        updateCarrier();
    }

    void noteOn(uint8_t note, uint8_t velocity = 127) noexcept
    {
        m_note = note;
        m_velocity = detail::unit(velocity);
        m_gateEnvelope = 1.0f;
        updateCarrier();
    }
    void noteOff() noexcept {}
    void setPitchBend(float semitones) noexcept { m_pitchBend = semitones; updateCarrier(); }

    void processStereo(float* outL, float* outR, size_t frames) noexcept
    {
        const float inverseRate = 1.0f / static_cast<float>(m_sampleRate);
        const float release = detail::decayCoefficient(0.48f, m_sampleRate);
        for (size_t i = 0; i < frames; ++i) {
            m_gateEnvelope *= release;
            const float phaseOneStep = detail::kTwoPi * m_carrier * std::max(0.0f, m_ratioOne + m_fineOne) * inverseRate;
            const float phaseTwoStep = detail::kTwoPi * m_carrier * m_ratioTwo * inverseRate;
            const float one = std::sin(m_phaseOne + m_feedbackSample * m_feedback);
            m_feedbackSample = one;
            const float two = std::sin(m_phaseTwo);
            m_phaseOne = detail::wrapPhase(m_phaseOne + phaseOneStep);
            m_phaseTwo = detail::wrapPhase(m_phaseTwo + phaseTwoStep);
            const float phase = m_phaseCarrier + one * m_indexOne * (0.42f + 0.58f * m_gateEnvelope)
                + two * m_indexTwo;
            m_phaseCarrier = detail::wrapPhase(m_phaseCarrier + detail::kTwoPi * m_carrier * inverseRate);
            const float raw = std::tanh(std::sin(phase) * 1.16f) * (0.34f + 0.10f * m_velocity);
            const float sample = detail::applyTone(raw, m_toneCoefficient, m_toneState);
            outL[i] = sample;
            outR[i] = sample;
        }
    }

private:
    void updateCarrier() noexcept { m_carrier = detail::noteHz(static_cast<float>(m_note) + m_pitchBend + m_tune); }
    double m_sampleRate = 44100.0;
    uint8_t m_note = 60;
    float m_velocity = 1.0f, m_pitchBend = 0.0f, m_tune = 0.0f, m_carrier = 261.6256f;
    float m_ratioOne = 1.0f, m_fineOne = 0.0f, m_indexOne = 0.0f, m_feedback = 0.0f;
    float m_ratioTwo = 1.0f, m_indexTwo = 0.0f, m_toneCoefficient = 1.0f;
    float m_phaseCarrier = 0.0f, m_phaseOne = 0.0f, m_phaseTwo = 0.0f;
    float m_feedbackSample = 0.0f, m_gateEnvelope = 0.0f, m_toneState = 0.0f;
};

// Native independent interpretation of FM+ PAR. Three independently decaying
// modulators are summed in parallel at the carrier phase input.
class OracleParallelCore {
public:
    void reset(double sampleRate = 44100.0) noexcept
    {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = m_phaseOne = m_phaseTwo = m_phaseThree = 0.0f;
        m_envOne = m_envTwo = m_envThree = 0.0f;
        m_toneState = 0.0f;
    }

    void setParameters(uint8_t frq1, uint8_t env1, uint8_t frq2, uint8_t env2,
                       uint8_t frq3, uint8_t env3, uint8_t tone, uint8_t tune) noexcept
    {
        m_ratioOne = statParRatioForRaw(frq1);
        m_ratioTwo = statParRatioForRaw(frq2);
        m_ratioThree = statParRatioForRaw(frq3);
        m_depthOne = detail::unit(env1) * 4.25f;
        m_depthTwo = detail::unit(env2) * 4.25f;
        m_depthThree = detail::unit(env3) * 4.25f;
        m_toneCoefficient = detail::onePoleCoefficient(detail::unit(tone), m_sampleRate);
        m_tune = tuneSemitonesForRaw(tune);
        updateCarrier();
    }

    void noteOn(uint8_t note, uint8_t velocity = 127) noexcept
    {
        m_note = note;
        m_velocity = detail::unit(velocity);
        m_envOne = m_envTwo = m_envThree = 1.0f;
        updateCarrier();
    }
    void noteOff() noexcept {}
    void setPitchBend(float semitones) noexcept { m_pitchBend = semitones; updateCarrier(); }

    void processStereo(float* outL, float* outR, size_t frames) noexcept
    {
        const float inverseRate = 1.0f / static_cast<float>(m_sampleRate);
        const float decayOne = detail::decayCoefficient(0.30f, m_sampleRate);
        const float decayTwo = detail::decayCoefficient(0.47f, m_sampleRate);
        const float decayThree = detail::decayCoefficient(0.68f, m_sampleRate);
        for (size_t i = 0; i < frames; ++i) {
            m_envOne *= decayOne;
            m_envTwo *= decayTwo;
            m_envThree *= decayThree;
            const float one = std::sin(m_phaseOne) * m_depthOne * (0.35f + 0.65f * m_envOne);
            const float two = std::sin(m_phaseTwo) * m_depthTwo * (0.35f + 0.65f * m_envTwo);
            const float three = std::sin(m_phaseThree) * m_depthThree * (0.35f + 0.65f * m_envThree);
            m_phaseOne = detail::wrapPhase(m_phaseOne + detail::kTwoPi * m_carrier * m_ratioOne * inverseRate);
            m_phaseTwo = detail::wrapPhase(m_phaseTwo + detail::kTwoPi * m_carrier * m_ratioTwo * inverseRate);
            m_phaseThree = detail::wrapPhase(m_phaseThree + detail::kTwoPi * m_carrier * m_ratioThree * inverseRate);
            const float raw = std::tanh(std::sin(m_phaseCarrier + one + two + three) * 1.08f) * (0.35f + 0.09f * m_velocity);
            m_phaseCarrier = detail::wrapPhase(m_phaseCarrier + detail::kTwoPi * m_carrier * inverseRate);
            const float sample = detail::applyTone(raw, m_toneCoefficient, m_toneState);
            outL[i] = sample;
            outR[i] = sample;
        }
    }

private:
    void updateCarrier() noexcept { m_carrier = detail::noteHz(static_cast<float>(m_note) + m_pitchBend + m_tune); }
    double m_sampleRate = 44100.0;
    uint8_t m_note = 60;
    float m_velocity = 1.0f, m_pitchBend = 0.0f, m_tune = 0.0f, m_carrier = 261.6256f;
    float m_ratioOne = 1.0f, m_ratioTwo = 1.0f, m_ratioThree = 1.0f;
    float m_depthOne = 0.0f, m_depthTwo = 0.0f, m_depthThree = 0.0f, m_toneCoefficient = 1.0f;
    float m_phaseCarrier = 0.0f, m_phaseOne = 0.0f, m_phaseTwo = 0.0f, m_phaseThree = 0.0f;
    float m_envOne = 0.0f, m_envTwo = 0.0f, m_envThree = 0.0f, m_toneState = 0.0f;
};

// Native independent interpretation of FM+ DYN. The second modulator feeds a
// feedback path into the first one; the first modulator then drives the carrier.
class OracleDynamicCore {
public:
    void reset(double sampleRate = 44100.0) noexcept
    {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = m_phaseOne = m_phaseTwo = 0.0f;
        m_feedbackSample = m_envFrequency = m_envVolume = m_envSecond = 0.0f;
        m_toneState = 0.0f;
    }

    void setParameters(uint8_t frq1, uint8_t fen1, uint8_t vol1, uint8_t ven1,
                       uint8_t frq2, uint8_t env2, uint8_t fb2, uint8_t tune) noexcept
    {
        m_ratioOne = dynRatio1ForRaw(frq1);
        m_frequencyEnvelope = detail::bipolar(fen1);
        m_volumeOne = detail::unit(vol1) * 5.8f;
        m_volumeEnvelope = detail::bipolar(ven1);
        m_ratioTwo = dynRatio2ForRaw(frq2);
        m_volumeTwo = detail::unit(env2) * 5.0f;
        m_feedback = detail::unit(fb2) * 1.18f;
        m_tune = tuneSemitonesForRaw(tune);
        updateCarrier();
    }

    void noteOn(uint8_t note, uint8_t velocity = 127) noexcept
    {
        m_note = note;
        m_velocity = detail::unit(velocity);
        m_envFrequency = m_envVolume = m_envSecond = 1.0f;
        updateCarrier();
    }
    void noteOff() noexcept {}
    void setPitchBend(float semitones) noexcept { m_pitchBend = semitones; updateCarrier(); }

    void processStereo(float* outL, float* outR, size_t frames) noexcept
    {
        const float inverseRate = 1.0f / static_cast<float>(m_sampleRate);
        const float frequencyDecay = detail::decayCoefficient(0.013f, m_sampleRate);
        const float volumeDecay = detail::decayCoefficient(0.037f, m_sampleRate);
        const float secondDecay = detail::decayCoefficient(0.024f, m_sampleRate);
        for (size_t i = 0; i < frames; ++i) {
            m_envFrequency *= frequencyDecay;
            m_envVolume *= volumeDecay;
            m_envSecond *= secondDecay;
            const float two = std::sin(m_phaseTwo + m_feedbackSample * m_feedback);
            m_feedbackSample = two;
            m_phaseTwo = detail::wrapPhase(m_phaseTwo + detail::kTwoPi * m_carrier * m_ratioTwo * inverseRate);
            const float ratioOne = std::max(0.0f, m_ratioOne + m_frequencyEnvelope * m_envFrequency);
            const float one = std::sin(m_phaseOne + two * m_volumeTwo * m_envSecond);
            m_phaseOne = detail::wrapPhase(m_phaseOne + detail::kTwoPi * m_carrier * ratioOne * inverseRate);
            const float index = m_volumeOne * std::max(0.0f, 1.0f + m_volumeEnvelope * (m_envVolume - 1.0f));
            const float raw = std::tanh(std::sin(m_phaseCarrier + one * index) * 1.14f) * (0.34f + 0.10f * m_velocity);
            m_phaseCarrier = detail::wrapPhase(m_phaseCarrier + detail::kTwoPi * m_carrier * inverseRate);
            // DYN has no TONE word; keep its private output conditioning fixed.
            const float sample = detail::applyTone(raw, 0.92f, m_toneState);
            outL[i] = sample;
            outR[i] = sample;
        }
    }

private:
    void updateCarrier() noexcept { m_carrier = detail::noteHz(static_cast<float>(m_note) + m_pitchBend + m_tune); }
    double m_sampleRate = 44100.0;
    uint8_t m_note = 60;
    float m_velocity = 1.0f, m_pitchBend = 0.0f, m_tune = 0.0f, m_carrier = 261.6256f;
    float m_ratioOne = 0.0f, m_frequencyEnvelope = 0.0f, m_volumeOne = 0.0f, m_volumeEnvelope = 0.0f;
    float m_ratioTwo = 0.0f, m_volumeTwo = 0.0f, m_feedback = 0.0f;
    float m_phaseCarrier = 0.0f, m_phaseOne = 0.0f, m_phaseTwo = 0.0f;
    float m_feedbackSample = 0.0f, m_envFrequency = 0.0f, m_envVolume = 0.0f, m_envSecond = 0.0f, m_toneState = 0.0f;
};

} // namespace fm_oracle
} // namespace monomachine
