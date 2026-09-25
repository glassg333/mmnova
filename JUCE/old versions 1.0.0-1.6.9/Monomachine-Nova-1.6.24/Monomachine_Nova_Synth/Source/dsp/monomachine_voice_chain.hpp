// Nova integration: external ADSR, scalable delay, smoothed delay time.
#pragma once

#include "monomachine_filter.hpp"
#include "monomachine_chorus.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>

namespace monomachine {

/**
 * @brief Mode for FX triggering:
 * - FreeRunning: Gate is continuously held open (100% audio throughput, envelope bypassed).
 * - MidiTriggered: Envelopes strictly require MIDI Note-On to open gate and sweep filter.
 */
enum class FxGateMode : uint8_t {
    FreeRunning,   // Works continuously without MIDI (traditional insert FX)
    MidiTriggered  // Strictly gated & shaped by incoming MIDI notes (rhythmic/sidechain FX)
};

// Nova routing: Chorus -> Distortion -> HP/LP Filter -> Amp/Pan -> Delay.
// EQ and SRR are NOT implemented here. Separate L/R outputs are required.
class MonomachineVoiceChain {
public:
    MonomachineVoiceChain() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        kMaxDelaySamples = static_cast<size_t>(std::ceil(m_sampleRate * 0.75)) + 4;
        m_delaySlew = 1.0f - std::exp(-1.0f / static_cast<float>(0.03 * m_sampleRate));
        m_filter.reset(sampleRate);
        m_chorus.reset(sampleRate);
        m_delayBufferL.assign(kMaxDelaySamples, 0.0f);
        m_delayBufferR.assign(kMaxDelaySamples, 0.0f);
        m_delayWritePos = 0;
        m_ampEnvStage = AmpEnvStage::Idle;
        m_ampEnvVal = 0.0f;
        m_currentDelay = static_cast<float>(std::min(m_delayTimeSamples, kMaxDelaySamples - 2));
    }

    MonomachineFilter& filter() noexcept { return m_filter; }
    MonomachineChorus& chorus() noexcept { return m_chorus; }

    void setFxGateMode(FxGateMode mode) noexcept {
        m_fxGateMode = mode;
        if (m_fxGateMode == FxGateMode::FreeRunning) {
            m_ampEnvVal = 1.0f; // Gate held open continuously
        }
    }

    FxGateMode fxGateMode() const noexcept { return m_fxGateMode; }

    void noteOn(uint8_t note, uint8_t velocity = 127) {
        (void)note;
        (void)velocity;
        m_filter.triggerEnvelope();
        m_ampEnvStage = AmpEnvStage::Attack;
    }

    void noteOff() {
        m_filter.releaseEnvelope();
        m_ampEnvStage = AmpEnvStage::Release;
    }

    void setAmpEnvelope(uint8_t atk, uint8_t hold, uint8_t dec, uint8_t rel) {
        m_atkParam = atk;
        m_holdParam = hold;
        m_decParam = dec;
        m_relParam = rel;
    }

    void setDistortion(uint8_t dist) { m_distortion = dist; }
    void setVolume(uint8_t vol) { m_volume = vol / 127.0f; }
    void setPan(uint8_t pan) {
        m_panL = pan <= 64 ? 1.0f : (127.0f - pan) / 63.0f;
        m_panR = pan >= 64 ? 1.0f : pan / 64.0f;
    }

    void setDelay(uint8_t time, uint8_t send, uint8_t feedback) {
        m_delayTimeSamples = std::clamp(static_cast<size_t>((time / 127.0f) * static_cast<float>(m_sampleRate) * 0.75f),
                                        static_cast<size_t>(1), kMaxDelaySamples - 1);
        m_delaySend = send / 127.0f;
        m_delayFeedback = (feedback / 127.0f) * 0.95f;
    }

    void setDelayMilliseconds(float milliseconds, float wet, float feedback, bool snap = false) noexcept {
        m_delayTimeSamples = std::clamp(static_cast<size_t>(std::lround(milliseconds * 0.001 * m_sampleRate)),
                                        static_cast<size_t>(1), kMaxDelaySamples - 2);
        m_delaySend = std::clamp(wet, 0.0f, 1.0f);
        m_delayFeedback = std::clamp(feedback, 0.0f, 0.95f);
        if (snap) m_currentDelay = static_cast<float>(m_delayTimeSamples);
    }

    void processBlock(const float* inL, const float* inR,
                      float* outL, float* outR, size_t numFrames, const float* externalEnvelope = nullptr) {
        // 1. FX Stage (e.g. Chorus when active)
        m_chorus.processStereo(inL, inR, outL, outR, numFrames);

        // 2. Distortion / Saturation Stage
        if (m_distortion > 0) {
            float drive = 1.0f + (m_distortion / 127.0f) * 15.0f;
            for (size_t i = 0; i < numFrames; ++i) {
                outL[i] = std::tanh(outL[i] * drive);
                outR[i] = std::tanh(outR[i] * drive);
            }
        }

        // 3. Multi-Mode Filter Stage (Base-Width 24dB)
        m_filter.processStereo(outL, outR, outL, outR, numFrames);

        // 4. Amp Envelope & Pan Stage
        for (size_t i = 0; i < numFrames; ++i) {
            stepAmpEnvelope();
            float gain = m_volume * m_ampEnvVal * (externalEnvelope ? externalEnvelope[i] : 1.0f);
            outL[i] *= gain * m_panL;
            outR[i] *= gain * m_panR;
        }

        // Ring always advances, including when send is zero. No stale frozen echoes.
        for (size_t i = 0; i < numFrames; ++i) {
            m_currentDelay += m_delaySlew * (static_cast<float>(m_delayTimeSamples) - m_currentDelay);
            float pos = static_cast<float>(m_delayWritePos) - std::clamp(m_currentDelay, 1.0f, static_cast<float>(kMaxDelaySamples - 2));
            if (pos < 0.0f) pos += static_cast<float>(kMaxDelaySamples);
            const size_t p0 = static_cast<size_t>(pos) % kMaxDelaySamples;
            const size_t p1 = (p0 + 1) % kMaxDelaySamples;
            const float frac = pos - std::floor(pos);
            const float dL = m_delayBufferL[p0] + frac * (m_delayBufferL[p1] - m_delayBufferL[p0]);
            const float dR = m_delayBufferR[p0] + frac * (m_delayBufferR[p1] - m_delayBufferR[p0]);
            m_delayBufferL[m_delayWritePos] = outL[i] + dL * m_delayFeedback;
            m_delayBufferR[m_delayWritePos] = outR[i] + dR * m_delayFeedback;
            outL[i] += dL * m_delaySend;
            outR[i] += dR * m_delaySend;
            m_delayWritePos = (m_delayWritePos + 1) % kMaxDelaySamples;
        }
    }

    // Safe on the audio thread: clears allocated storage without resizing.
    void clearBuffers() noexcept {
        m_chorus.clearBuffers();
        m_filter.reset(m_sampleRate);
        std::fill(m_delayBufferL.begin(), m_delayBufferL.end(), 0.0f);
        std::fill(m_delayBufferR.begin(), m_delayBufferR.end(), 0.0f);
        m_delayWritePos = 0;
        m_ampEnvStage = AmpEnvStage::Idle;
        m_ampEnvVal = 0.0f;
    }

private:
    enum class AmpEnvStage { Idle, Attack, Hold, Decay, Release };

    void stepAmpEnvelope() {
        if (m_fxGateMode == FxGateMode::FreeRunning) {
            m_ampEnvVal = 1.0f; // Gate remains wide open, no MIDI required
            return;
        }

        // MidiTriggered envelope progression
        switch (m_ampEnvStage) {
            case AmpEnvStage::Idle:
                m_ampEnvVal = 0.0f;
                break;
            case AmpEnvStage::Attack: {
                float rate = 1.0f / (0.001f + (m_atkParam / 127.0f) * static_cast<float>(m_sampleRate) * 0.5f);
                m_ampEnvVal += rate;
                if (m_ampEnvVal >= 1.0f) {
                    m_ampEnvVal = 1.0f;
                    m_ampEnvStage = AmpEnvStage::Hold;
                    m_holdCounter = static_cast<size_t>((m_holdParam / 127.0f) * m_sampleRate * 0.5f);
                }
                break;
            }
            case AmpEnvStage::Hold:
                if (m_holdCounter > 0) {
                    --m_holdCounter;
                } else {
                    m_ampEnvStage = AmpEnvStage::Decay;
                }
                break;
            case AmpEnvStage::Decay: {
                float decayFactor = std::exp(-1.0f / (0.005f + (m_decParam / 127.0f) * static_cast<float>(m_sampleRate) * 1.5f));
                m_ampEnvVal *= decayFactor;
                if (m_ampEnvVal < 0.001f) {
                    m_ampEnvVal = 0.0f;
                    m_ampEnvStage = AmpEnvStage::Idle;
                }
                break;
            }
            case AmpEnvStage::Release: {
                float relFactor = std::exp(-1.0f / (0.005f + (m_relParam / 127.0f) * static_cast<float>(m_sampleRate) * 1.5f));
                m_ampEnvVal *= relFactor;
                if (m_ampEnvVal < 0.001f) {
                    m_ampEnvVal = 0.0f;
                    m_ampEnvStage = AmpEnvStage::Idle;
                }
                break;
            }
        }
    }

    size_t kMaxDelaySamples = 65536;
    float m_currentDelay = 22050.0f;
    float m_delaySlew = 0.001f;

    double m_sampleRate = 44100.0;
    MonomachineFilter m_filter;
    MonomachineChorus m_chorus;

    FxGateMode m_fxGateMode = FxGateMode::FreeRunning;
    AmpEnvStage m_ampEnvStage = AmpEnvStage::Idle;
    float m_ampEnvVal = 1.0f;
    size_t m_holdCounter = 0;

    uint8_t m_atkParam = 0;
    uint8_t m_holdParam = 64;
    uint8_t m_decParam = 64;
    uint8_t m_relParam = 64;

    uint8_t m_distortion = 0;
    float m_volume = 1.0f;
    float m_panL = 1.0f;
    float m_panR = 1.0f;

    std::vector<float> m_delayBufferL;
    std::vector<float> m_delayBufferR;
    size_t m_delayWritePos = 0;
    size_t m_delayTimeSamples = 22050;
    float m_delaySend = 0.0f;
    float m_delayFeedback = 0.0f;
};

} // namespace monomachine
