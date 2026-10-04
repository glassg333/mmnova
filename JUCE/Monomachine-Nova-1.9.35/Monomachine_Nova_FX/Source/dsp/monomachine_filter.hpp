// Nova integration fixes: reset coefficients; envelope times expressed in seconds.
#pragma once

#include <cmath>
#include <algorithm>
#include <cstdint>

namespace monomachine {

/**
 * @brief Elektron Monomachine Base-Width Multi-Mode Resonant Filter.
 * 
 * Hardware Architecture:
 * - BASE defines High-Pass Cutoff (0..127).
 * - WIDTH defines Low-Pass Cutoff span relative to BASE (LP Cutoff = BASE + WIDTH).
 * - HPQ / LPQ set the resonance/peak damping of the respective filters.
 * - Modulations from Filter Envelope (ATK, DEC, BOFS, WOFS) offset Base and Width dynamically.
 * - Formed by a 2-pole resonant High-Pass filter in series with a 2-pole resonant Low-Pass filter.
 */
class MonomachineFilter {
public:
    MonomachineFilter() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_hpL.reset();
        m_hpR.reset();
        m_lpL.reset();
        m_lpR.reset();
        m_envStage = EnvStage::Idle;
        m_envValue = 0.0f;
        m_envFrameCounter = 0;
        updateCoefficients();
    }

    void setParameters(float base, float width, float hpq, float lpq,
                       float attack, float decay, float bofs, float wofs) {
        // The host renders TrackChain in one-sample slices. Parameters are
        // refreshed on a control cadence, not a reason to recalculate four
        // trigonometric biquad coefficient sets on every audio sample.
        constexpr float epsilon = 1.0e-6f;
        if (std::abs(base - m_baseParam) <= epsilon && std::abs(width - m_widthParam) <= epsilon
            && std::abs(hpq - m_hpqParam) <= epsilon && std::abs(lpq - m_lpqParam) <= epsilon
            && std::abs(attack - m_attackParam) <= epsilon && std::abs(decay - m_decayParam) <= epsilon
            && std::abs(bofs - m_bofsParam) <= epsilon && std::abs(wofs - m_wofsParam) <= epsilon)
            return;
        m_baseParam = base;
        m_widthParam = width;
        m_hpqParam = hpq;
        m_lpqParam = lpq;
        m_attackParam = attack;
        m_decayParam = decay;
        m_bofsParam = bofs;
        m_wofsParam = wofs;

        updateCoefficients();
    }

    // Explicit opt-in VEL/KT and FIL ENV path. Defaults are zero, retaining
    // the legacy OLD filter response bit-for-bit until a new control is used.
    void setExternalFilterModifiers(float lowerSemitones, float upperSemitones,
                                    float baseOffset, float widthOffset,
                                    bool hpfKeyTracking = false, bool lpfKeyTracking = false,
                                    int midiNote = 60) {
        lowerSemitones = std::clamp(lowerSemitones, -128.0f, 128.0f);
        upperSemitones = std::clamp(upperSemitones, -128.0f, 128.0f);
        baseOffset = std::clamp(baseOffset, -127.0f, 127.0f);
        widthOffset = std::clamp(widthOffset, -127.0f, 127.0f);
        midiNote = std::clamp(midiNote, 0, 127);
        if (std::abs(lowerSemitones - m_externalLowerSemitones) < 1.0e-6f
            && std::abs(upperSemitones - m_externalUpperSemitones) < 1.0e-6f
            && std::abs(baseOffset - m_externalBaseOffset) < 1.0e-6f
            && std::abs(widthOffset - m_externalWidthOffset) < 1.0e-6f
            && hpfKeyTracking == m_hpfKeyTracking
            && lpfKeyTracking == m_lpfKeyTracking
            && midiNote == m_keyTrackingMidiNote) return;
        m_externalLowerSemitones = lowerSemitones;
        m_externalUpperSemitones = upperSemitones;
        m_externalBaseOffset = baseOffset;
        m_externalWidthOffset = widthOffset;
        m_hpfKeyTracking = hpfKeyTracking;
        m_lpfKeyTracking = lpfKeyTracking;
        m_keyTrackingMidiNote = midiNote;
        updateCoefficients();
    }

    void triggerEnvelope() {
        m_envStage = EnvStage::Attack;
        m_envFrameCounter = 0;
    }

    void releaseEnvelope() {
        m_envStage = EnvStage::Decay;
        m_envFrameCounter = 0;
    }

    void processStereo(const float* inL, const float* inR,
                       float* outL, float* outR, size_t numFrames) {
        for (size_t i = 0; i < numFrames; ++i) {
            stepEnvelope();

            float left = inL ? inL[i] : 0.0f;
            float right = inR ? inR[i] : 0.0f;

            // Series routing: Input -> High-Pass (BASE) -> Low-Pass (BASE + WIDTH)
            left = m_hpL.process(left);
            right = m_hpR.process(right);

            left = m_lpL.process(left);
            right = m_lpR.process(right);

            outL[i] = left;
            outR[i] = right;
        }
    }

private:
    struct Biquad {
        float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
        float z1 = 0.0f, z2 = 0.0f;

        void reset() {
            z1 = 0.0f;
            z2 = 0.0f;
        }

        inline float process(float in) {
            float out = b0 * in + z1;
            z1 = b1 * in - a1 * out + z2;
            z2 = b2 * in - a2 * out;
            return out;
        }

        void setHighPass(float cutoffHz, float q, double sampleRate) {
            float omega = 2.0f * 3.14159265358979323846f * std::clamp(cutoffHz, 10.0f, static_cast<float>(sampleRate * 0.49));
            float w0 = omega / static_cast<float>(sampleRate);
            float cosw0 = std::cos(w0);
            float sinw0 = std::sin(w0);
            float alpha = sinw0 / (2.0f * std::max(q, 0.1f));

            float a0 = 1.0f + alpha;
            b0 = ((1.0f + cosw0) / 2.0f) / a0;
            b1 = (-(1.0f + cosw0)) / a0;
            b2 = ((1.0f + cosw0) / 2.0f) / a0;
            a1 = (-2.0f * cosw0) / a0;
            a2 = (1.0f - alpha) / a0;
        }

        void setLowPass(float cutoffHz, float q, double sampleRate) {
            float omega = 2.0f * 3.14159265358979323846f * std::clamp(cutoffHz, 10.0f, static_cast<float>(sampleRate * 0.49));
            float w0 = omega / static_cast<float>(sampleRate);
            float cosw0 = std::cos(w0);
            float sinw0 = std::sin(w0);
            float alpha = sinw0 / (2.0f * std::max(q, 0.1f));

            float a0 = 1.0f + alpha;
            b0 = ((1.0f - cosw0) / 2.0f) / a0;
            b1 = (1.0f - cosw0) / a0;
            b2 = ((1.0f - cosw0) / 2.0f) / a0;
            a1 = (-2.0f * cosw0) / a0;
            a2 = (1.0f - alpha) / a0;
        }
    };

    enum class EnvStage { Idle, Attack, Decay };

    void stepEnvelope() {
        // The firmware-facing filter control path is block based. Updating the
        // OLD comparison filter at audio rate was a P2 CPU regression (and was
        // needlessly recomputing sin/cos even when BOFS/WOFS were neutral).
        constexpr int kEnvelopeFrame = 16;
        if ((m_envFrameCounter++ % kEnvelopeFrame) != 0)
            return;
        const float frameSamples = static_cast<float>(kEnvelopeFrame);
        bool coefficientsChanged = false;
        if (m_envStage == EnvStage::Attack) {
            const float rate = frameSamples / ((0.001f + (static_cast<float>(m_attackParam) / 127.0f) * 0.5f)
                                               * static_cast<float>(m_sampleRate));
            m_envValue += rate;
            if (m_envValue >= 1.0f) {
                m_envValue = 1.0f;
                m_envStage = EnvStage::Decay;
            }
            coefficientsChanged = std::abs(m_bofsParam) > 1.0e-6f || std::abs(m_wofsParam) > 1.0e-6f;
        } else if (m_envStage == EnvStage::Decay) {
            const float decayFactor = std::exp(-frameSamples / ((0.005f + (static_cast<float>(m_decayParam) / 127.0f) * 1.5f)
                                                               * static_cast<float>(m_sampleRate)));
            m_envValue *= decayFactor;
            if (m_envValue < 0.0001f) {
                m_envValue = 0.0f;
                m_envStage = EnvStage::Idle;
            }
            coefficientsChanged = std::abs(m_bofsParam) > 1.0e-6f || std::abs(m_wofsParam) > 1.0e-6f;
        }
        if (coefficientsChanged)
            updateCoefficients();
    }

    void updateCoefficients() {
        // Monomachine exponential frequency curve approximation for 0..127
        auto mnmFreq = [](float val) {
            float norm = std::clamp(val / 127.0f, 0.0f, 1.0f);
            return 20.0f * std::pow(1000.0f, norm); // ~20 Hz to 20 kHz
        };

        float effectiveBase = static_cast<float>(m_baseParam) + m_envValue * (m_bofsParam / 127.0f) * 64.0f + m_externalBaseOffset;
        float effectiveWidth = static_cast<float>(m_widthParam) + m_envValue * (m_wofsParam / 127.0f) * 64.0f + m_externalWidthOffset;
        effectiveBase = std::clamp(effectiveBase, 0.0f, 127.0f);
        effectiveWidth = std::clamp(effectiveWidth, 0.0f, 127.0f);

        // Manual-derived normal tracking: BASE=0 begins at played note / 4;
        // every +8 BASE or WDTH units raises the corresponding physical edge
        // by an octave. LP retains the documented BASE+WDTH topology.
        const auto trackedFreq=[this](float control){
            const float noteHz=440.0f*std::exp2((static_cast<float>(m_keyTrackingMidiNote)-69.0f)/12.0f);
            return noteHz*0.25f*std::exp2(control/8.0f);
        };
        const float lpControl = std::min(127.0f, effectiveBase + effectiveWidth);
        float hpFreq = m_hpfKeyTracking ? trackedFreq(effectiveBase) : mnmFreq(effectiveBase);
        // LP keytrack must use the same clamped BASE+WDTH position as the
        // non-tracked route. This keeps the physical upper-edge position
        // invariant when its keytrack switch is changed.
        float lpFreq = m_lpfKeyTracking ? trackedFreq(lpControl)
                                         : mnmFreq(lpControl);
        hpFreq = std::clamp(hpFreq * std::exp2(m_externalLowerSemitones / 12.0f), 10.0f, static_cast<float>(m_sampleRate * 0.49));
        lpFreq = std::clamp(lpFreq * std::exp2(m_externalUpperSemitones / 12.0f), 10.0f, static_cast<float>(m_sampleRate * 0.49));

        // Q mapping (0.5 to ~12.0)
        float hpQ = 0.707f + (static_cast<float>(m_hpqParam) / 127.0f) * 8.0f;
        float lpQ = 0.707f + (static_cast<float>(m_lpqParam) / 127.0f) * 8.0f;

        m_hpL.setHighPass(hpFreq, hpQ, m_sampleRate);
        m_hpR.setHighPass(hpFreq, hpQ, m_sampleRate);

        m_lpL.setLowPass(lpFreq, lpQ, m_sampleRate);
        m_lpR.setLowPass(lpFreq, lpQ, m_sampleRate);
    }

    double m_sampleRate = 44100.0;
    float m_baseParam = 0;
    float m_widthParam = 127;
    float m_hpqParam = 0;
    float m_lpqParam = 0;
    float m_attackParam = 0;
    float m_decayParam = 64;
    float m_bofsParam = 0;
    float m_wofsParam = 0;
    float m_externalLowerSemitones = 0;
    float m_externalUpperSemitones = 0;
    float m_externalBaseOffset = 0;
    float m_externalWidthOffset = 0;
    bool m_hpfKeyTracking = false, m_lpfKeyTracking = false;
    int m_keyTrackingMidiNote = 60;

    EnvStage m_envStage = EnvStage::Idle;
    float m_envValue = 0.0f;
    int m_envFrameCounter = 0;

    Biquad m_hpL, m_hpR;
    Biquad m_lpL, m_lpR;
};

} // namespace monomachine
