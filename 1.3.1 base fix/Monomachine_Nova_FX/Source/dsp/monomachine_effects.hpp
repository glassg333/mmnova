// Nova: scalable flanger ring and external cross-channel ring carrier.
#pragma once

#include "monomachine_chorus.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>

namespace monomachine {

/**
 * @brief Native Monomachine FX-PHASER (Machine 18)
 * 4-stage / 6-stage all-pass ladder modulated by quadrature LFO with stereo spread.
 * CNTR (Center), DEP (Depth), SPD (Speed), MIX (Wet/Dry), WID (Stereo Width Spread).
 */
class MonomachinePhaser {
public:
    MonomachinePhaser() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        for (int ch = 0; ch < 2; ++ch) {
            for (int stage = 0; stage < kStages; ++stage) {
                m_allpassState[ch][stage] = 0.0f;
            }
            m_feedback[ch] = 0.0f;
        }
        m_lfoPhase = 0.0f;
    }

    void setParameters(uint8_t cntr, uint8_t dep, uint8_t spd, uint8_t mix, uint8_t fb, uint8_t wid) {
        m_cntrParam = cntr;
        m_depParam = dep;
        m_spdParam = spd;
        m_mixParam = mix;
        m_fbParam = fb;
        m_widParam = wid;

        m_centerFreq = 100.0f * std::pow(30.0f, cntr / 127.0f); // 100 Hz to 3 kHz
        m_depth = (dep / 127.0f) * 1.5f;
        m_lfoFreq = 0.05f * std::pow(80.0f, spd / 127.0f); // 0.05 Hz to 4 Hz
        m_wetMix = mix / 127.0f;
        m_feedbackAmt = (fb / 127.0f) * 0.92f;
        m_stereoSpread = (wid / 127.0f) * 3.14159265358979323846f;
    }

    void processStereo(const float* inL, const float* inR,
                       float* outL, float* outR, size_t numFrames) {
        const float lfoPhaseInc = 2.0f * 3.14159265358979323846f * m_lfoFreq / static_cast<float>(m_sampleRate);

        for (size_t i = 0; i < numFrames; ++i) {
            float inL_sample = inL ? inL[i] : 0.0f;
            float inR_sample = inR ? inR[i] : 0.0f;

            float modL = std::sin(m_lfoPhase) * m_depth;
            float modR = std::sin(m_lfoPhase + m_stereoSpread) * m_depth;

            // Calculate all-pass break frequencies
            float fL = std::clamp(m_centerFreq * std::pow(2.0f, modL), 20.0f, static_cast<float>(m_sampleRate * 0.45));
            float fR = std::clamp(m_centerFreq * std::pow(2.0f, modR), 20.0f, static_cast<float>(m_sampleRate * 0.45));

            // Bilinear transform all-pass coefficient
            float tanL = std::tan(3.14159265358979323846f * fL / static_cast<float>(m_sampleRate));
            float aL = (1.0f - tanL) / (1.0f + tanL);

            float tanR = std::tan(3.14159265358979323846f * fR / static_cast<float>(m_sampleRate));
            float aR = (1.0f - tanR) / (1.0f + tanR);

            // Channel 0 (Left)
            float sL = inL_sample + m_feedback[0] * m_feedbackAmt;
            for (int st = 0; st < kStages; ++st) {
                float y = -aL * sL + m_allpassState[0][st];
                m_allpassState[0][st] = sL + aL * y;
                sL = y;
            }
            m_feedback[0] = sL;
            outL[i] = (1.0f - m_wetMix) * inL_sample + m_wetMix * sL;

            // Channel 1 (Right)
            float sR = inR_sample + m_feedback[1] * m_feedbackAmt;
            for (int st = 0; st < kStages; ++st) {
                float y = -aR * sR + m_allpassState[1][st];
                m_allpassState[1][st] = sR + aR * y;
                sR = y;
            }
            m_feedback[1] = sR;
            outR[i] = (1.0f - m_wetMix) * inR_sample + m_wetMix * sR;

            m_lfoPhase += lfoPhaseInc;
            if (m_lfoPhase >= 2.0f * 3.14159265358979323846f) {
                m_lfoPhase -= 2.0f * 3.14159265358979323846f;
            }
        }
    }

private:
    static constexpr int kStages = 6;
    double m_sampleRate = 44100.0;
    float m_allpassState[2][kStages]{};
    float m_feedback[2]{0.0f, 0.0f};

    uint8_t m_cntrParam = 64;
    uint8_t m_depParam = 64;
    uint8_t m_spdParam = 48;
    uint8_t m_mixParam = 64;
    uint8_t m_fbParam = 0;
    uint8_t m_widParam = 64;

    float m_centerFreq = 500.0f;
    float m_feedbackAmt = 0.0f;
    float m_depth = 1.0f;
    float m_lfoFreq = 0.5f;
    float m_lfoPhase = 0.0f;
    float m_wetMix = 0.5f;
    float m_stereoSpread = 1.57f;
};

/**
 * @brief Native Monomachine FX-FLANGER (Machine 19)
 * Short comb delay (0.1ms to 10ms) with feedback resonance and quadrature LFO sweep.
 */
class MonomachineFlanger {
public:
    MonomachineFlanger() {
        m_bufL.resize(kMaxDelaySamples, 0.0f);
        m_bufR.resize(kMaxDelaySamples, 0.0f);
    }

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        kMaxDelaySamples = static_cast<size_t>(std::ceil(m_sampleRate * 0.012)) + 4;
        m_bufL.assign(kMaxDelaySamples, 0.0f);
        m_bufR.assign(kMaxDelaySamples, 0.0f);
        m_writePos = 0;
        m_lfoPhase = 0.0f;
        m_feedbackL = 0.0f;
        m_feedbackR = 0.0f;
    }

    void setParameters(uint8_t del, uint8_t dep, uint8_t spd, uint8_t mix, uint8_t fb, uint8_t wid) {
        m_delParam = del;
        m_depParam = dep;
        m_spdParam = spd;
        m_mixParam = mix;
        m_fbParam = fb;
        m_widParam = wid;

        // Base delay: 0.2ms to 8.0ms
        m_baseDelay = (0.0002f + (del / 127.0f) * 0.0078f) * static_cast<float>(m_sampleRate);
        m_modDepth = (dep / 127.0f) * 0.003f * static_cast<float>(m_sampleRate);
        m_lfoFreq = 0.05f * std::pow(60.0f, spd / 127.0f);
        m_wetMix = mix / 127.0f;
        m_feedbackGain = (fb / 127.0f) * 0.95f;
        m_stereoSpread = (wid / 127.0f) * 3.14159265358979323846f;
    }

    void processStereo(const float* inL, const float* inR,
                       float* outL, float* outR, size_t numFrames) {
        const float lfoPhaseInc = 2.0f * 3.14159265358979323846f * m_lfoFreq / static_cast<float>(m_sampleRate);

        for (size_t i = 0; i < numFrames; ++i) {
            float inSampleL = inL ? inL[i] : 0.0f;
            float inSampleR = inR ? inR[i] : 0.0f;

            m_bufL[m_writePos] = inSampleL + m_feedbackL * m_feedbackGain;
            m_bufR[m_writePos] = inSampleR + m_feedbackR * m_feedbackGain;

            float modL = 0.5f + 0.5f * std::sin(m_lfoPhase);
            float modR = 0.5f + 0.5f * std::sin(m_lfoPhase + m_stereoSpread);

            float dL = std::clamp(m_baseDelay + m_modDepth * modL, 1.0f, static_cast<float>(kMaxDelaySamples - 2));
            float dR = std::clamp(m_baseDelay + m_modDepth * modR, 1.0f, static_cast<float>(kMaxDelaySamples - 2));

            float delayedL = readInterpolated(m_bufL, static_cast<float>(m_writePos) - dL);
            float delayedR = readInterpolated(m_bufR, static_cast<float>(m_writePos) - dR);

            m_feedbackL = delayedL;
            m_feedbackR = delayedR;

            outL[i] = (1.0f - m_wetMix) * inSampleL + m_wetMix * delayedL;
            outR[i] = (1.0f - m_wetMix) * inSampleR + m_wetMix * delayedR;

            m_writePos = (m_writePos + 1) % kMaxDelaySamples;
            m_lfoPhase += lfoPhaseInc;
            if (m_lfoPhase >= 2.0f * 3.14159265358979323846f) {
                m_lfoPhase -= 2.0f * 3.14159265358979323846f;
            }
        }
    }

    void clearBuffers() noexcept {
        std::fill(m_bufL.begin(), m_bufL.end(), 0.0f);
        std::fill(m_bufR.begin(), m_bufR.end(), 0.0f);
        m_feedbackL = m_feedbackR = 0.0f; m_writePos = 0;
    }
private:
    size_t kMaxDelaySamples = 2048;

    inline float readInterpolated(const std::vector<float>& buffer, float readIndex) {
        while (readIndex < 0.0f) readIndex += static_cast<float>(kMaxDelaySamples);
        size_t i0 = static_cast<size_t>(readIndex) % kMaxDelaySamples;
        size_t i1 = (i0 + 1) % kMaxDelaySamples;
        float frac = readIndex - std::floor(readIndex);
        return buffer[i0] + frac * (buffer[i1] - buffer[i0]);
    }

    double m_sampleRate = 44100.0;
    std::vector<float> m_bufL;
    std::vector<float> m_bufR;
    size_t m_writePos = 0;

    uint8_t m_delParam = 20;
    uint8_t m_depParam = 64;
    uint8_t m_spdParam = 40;
    uint8_t m_mixParam = 64;
    uint8_t m_fbParam = 95;
    uint8_t m_widParam = 64;
    float m_feedbackGain = 0.71f;

    float m_baseDelay = 88.0f;
    float m_modDepth = 60.0f;
    float m_lfoFreq = 0.5f;
    float m_lfoPhase = 0.0f;
    float m_wetMix = 0.5f;
    float m_stereoSpread = 1.57f;
    float m_feedbackL = 0.0f;
    float m_feedbackR = 0.0f;
};

/**
 * @brief Native Monomachine FX-RINGMOD (Machine 17)
 */
class MonomachineRingMod {
public:
    MonomachineRingMod() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_carrierPhase = 0.0f;
    }

    void setParameters(uint8_t wave, uint8_t ext, uint8_t mix, float tuneMidiPitch) {
        m_waveParam = wave;
        m_extParam = ext;
        m_wetMix = mix / 127.0f;

        // Carrier frequency from tune
        m_carrierFreq = 440.0f * std::pow(2.0f, (tuneMidiPitch - 69.0f) / 12.0f);
    }

    void processStereo(const float* inL, const float* inR,
                       float* outL, float* outR, size_t numFrames) {
        const float phaseInc = 2.0f * 3.14159265358979323846f * m_carrierFreq / static_cast<float>(m_sampleRate);

        for (size_t i = 0; i < numFrames; ++i) {
            float inSampleL = inL ? inL[i] : 0.0f;
            float inSampleR = inR ? inR[i] : 0.0f;

            float carrier = std::sin(m_carrierPhase);
            if (m_waveParam > 64) {
                // Approximate square / triangle carrier
                carrier = carrier >= 0.0f ? 1.0f : -1.0f;
            }

            // EXT crossfades the internal oscillator with the opposite input channel.
            const float ext = m_extParam / 127.0f;
            float modL = inSampleL * ((1.0f-ext)*carrier + ext*inSampleR);
            float modR = inSampleR * ((1.0f-ext)*carrier + ext*inSampleL);

            outL[i] = (1.0f - m_wetMix) * inSampleL + m_wetMix * modL;
            outR[i] = (1.0f - m_wetMix) * inSampleR + m_wetMix * modR;

            m_carrierPhase += phaseInc;
            if (m_carrierPhase >= 2.0f * 3.14159265358979323846f) {
                m_carrierPhase -= (2.0f * 3.14159265358979323846f) * std::floor(m_carrierPhase / (2.0f * 3.14159265358979323846f));
            }
        }
    }

private:
    double m_sampleRate = 44100.0;
    uint8_t m_waveParam = 0;
    uint8_t m_extParam = 0;
    float m_wetMix = 1.0f;
    float m_carrierFreq = 440.0f;
    float m_carrierPhase = 0.0f;
};

} // namespace monomachine
