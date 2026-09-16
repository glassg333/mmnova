// Nova: deterministic default kit, external pitch bend, prepared-slot swap, short-sample retriggers.
#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstdint>
#include <utility>

namespace monomachine {

/**
 * @brief Elektron Monomachine DPRO-BBOX (Machine 7) & Extended Sampler Engine.
 * 
 * Hardware Architecture:
 * - DPRO-BBOX is the drum synthesizer/beatbox machine of the Monomachine.
 * - Hardware parameters:
 *   - PTCH (Pitch): Sample playback pitch / rate (-64 .. +63 semitone/fine ratio)
 *   - STRT (Start): Sample playback start offset (0 .. 127 -> 0% .. 100%)
 *   - RTRG (Retrig Count): Number of rapid retrigger repeats (0 = off, 1 .. 127)
 *   - RTIM (Retrig Time): Time between retriggers (tempo synchronized / milliseconds)
 * 
 * Extension for Modern DAW / VST3:
 * - 10 Sample Slots with dynamic loading.
 * - Sample randomization mode (random slot selection and/or random start position).
 * - Linear / Hermite sample interpolation with pitch tracking.
 */
class MonomachineBBox {
public:
    static constexpr size_t kMaxSlots = 10;

    struct SampleSlot {
        std::string name;
        std::vector<float> data;
        double originalSampleRate = 44100.0;
        bool loaded = false;
    };

    MonomachineBBox() {
        m_slots.resize(kMaxSlots);
        m_rng.seed(1337);
        generateDefaultDrumKit();
    }

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_playbackPos = 0.0;
        m_isPlaying = false;
        m_retrigCounter = 0;
        m_retrigSamplesRemaining = 0;
        m_playingOsc = false; m_oscAge = 0.0; m_oscPhase = m_oscPhase2 = m_oscPhase3 = m_oscPhase4 = 0.0; m_oscNoise = 0.0f;
    }

    /**
     * @brief Load a raw mono sample into one of the 10 slots (0..9).
     */
    bool loadSample(size_t slotIndex, const std::string& name, const float* samples, size_t numSamples, double sampleRate = 44100.0) {
        if (slotIndex >= kMaxSlots || samples == nullptr || numSamples == 0) return false;
        m_slots[slotIndex].name = name;
        m_slots[slotIndex].data.assign(samples, samples + numSamples);
        m_slots[slotIndex].originalSampleRate = sampleRate;
        m_slots[slotIndex].loaded = true;
        return true;
    }

    /**
     * @brief Monomachine BBox hardware parameter setter.
     * oscMode switches the engine from sample-slot playback to the internal
     * drum oscillator bank; slotSelect then picks the timbre (0..9) instead.
     */
    void setParameters(uint8_t ptch, uint8_t strt, uint8_t rtrg, uint8_t rtim,
                       uint8_t slotSelect = 0, bool randomSlot = false, bool randomStrt = false,
                       bool oscMode = false) {
        m_paramPtch = ptch;
        m_paramStrt = strt;
        m_paramRtrg = rtrg;
        m_paramRtim = rtim;
        m_activeSlot = slotSelect % kMaxSlots;
        m_oscMode = oscMode;
        m_oscKind = m_activeSlot;
        m_randomSlot = randomSlot;
        m_randomStart = randomStrt;

        // Pitch multiplier: 0 -> -2 octaves (0.25x), 64 -> 1.0x (normal), 127 -> +2 octaves (4.0x)
        float semitones = (static_cast<float>(ptch) - 64.0f) / 64.0f * 24.0f;
        m_pitchRatio = std::pow(2.0f, semitones / 12.0f);

        // Retrig interval in samples
        float retrigMs = 5.0f + (static_cast<float>(rtim) / 127.0f) * 250.0f;
        m_retrigIntervalSamples = static_cast<size_t>((retrigMs / 1000.0f) * m_sampleRate);
    }

    /**
     * @brief Trigger note on / drum hit.
     */
    void noteOn(uint8_t midiNote = 60, uint8_t velocity = 127) {
        if (m_oscMode) {
            m_playingOsc = true;
            m_oscAge = 0.0;
            m_oscPhase = 0.0; m_oscPhase2 = 0.0; m_oscPhase3 = 0.0; m_oscPhase4 = 0.0;
            m_oscNote = static_cast<float>(midiNote);
            m_oscEnv = velocity == 0 ? 0.0f : static_cast<float>(velocity) / 127.0f;
            m_retrigTotal = m_paramRtrg; m_retrigCounter = 0; m_retrigSamplesRemaining = m_retrigIntervalSamples;
            return;
        }
        (void)velocity;
        size_t targetSlot = m_activeSlot;

        if (m_randomSlot) {
            std::uniform_int_distribution<size_t> dist(0, kMaxSlots - 1);
            targetSlot = dist(m_rng);
        }

        m_currentPlayingSlot = targetSlot;
        auto& slot = m_slots[m_currentPlayingSlot];
        if (!slot.loaded || slot.data.empty()) {
            m_isPlaying = false;
            return;
        }

        float startFrac = static_cast<float>(m_paramStrt) / 127.0f;
        if (m_randomStart) {
            std::uniform_real_distribution<float> dist(0.0f, 0.85f);
            startFrac = dist(m_rng);
        }

        m_playbackPos = startFrac * static_cast<float>(slot.data.size());
        m_isPlaying = true;

        // Retrigger setup
        m_retrigTotal = m_paramRtrg;
        m_retrigCounter = 0;
        m_retrigSamplesRemaining = m_retrigIntervalSamples;

        // Key tracking pitch offset (Middle C / 60 is neutral)
        float noteSemitones = static_cast<float>(midiNote) - 60.0f;
        m_keyTrackRatio = std::pow(2.0f, noteSemitones / 12.0f);
    }

    /**
     * @brief Process a block of stereo samples.
     */
    void processStereo(float* outL, float* outR, size_t numFrames) {
        if (m_oscMode) { processOsc(outL, outR, numFrames); return; }
        if (!m_isPlaying || m_currentPlayingSlot >= kMaxSlots) {
            for (size_t i = 0; i < numFrames; ++i) {
                outL[i] = 0.0f;
                outR[i] = 0.0f;
            }
            return;
        }

        const auto& slot = m_slots[m_currentPlayingSlot];
        const auto& data = slot.data;
        const size_t dataSize = data.size();
        const double rateFactor = (slot.originalSampleRate / m_sampleRate) * m_pitchRatio * m_keyTrackRatio * m_bendRatio;

        for (size_t i = 0; i < numFrames; ++i) {
            if (!m_isPlaying) {
                outL[i] = 0.0f;
                outR[i] = 0.0f;
                continue;
            }

            // Check Retrigger
            if (m_retrigTotal > 0 && m_retrigCounter < m_retrigTotal) {
                if (m_retrigSamplesRemaining == 0) {
                    float startFrac = static_cast<float>(m_paramStrt) / 127.0f;
                    m_playbackPos = startFrac * static_cast<float>(dataSize);
                    m_retrigSamplesRemaining = m_retrigIntervalSamples;
                    ++m_retrigCounter;
                } else {
                    --m_retrigSamplesRemaining;
                }
            }

            // Interpolated sample read
            size_t idx0 = static_cast<size_t>(m_playbackPos);
            if (idx0 >= dataSize) {
                if (m_retrigCounter >= m_retrigTotal) m_isPlaying = false;
                outL[i] = 0.0f;
                outR[i] = 0.0f;
                continue;
            }

            size_t idx1 = std::min(idx0 + 1, dataSize - 1);
            float frac = static_cast<float>(m_playbackPos - idx0);
            float sample = data[idx0] + frac * (data[idx1] - data[idx0]);

            outL[i] = sample;
            outR[i] = sample;

            m_playbackPos += rateFactor;
        }
    }

    void setPitchBend(float semitones) { m_bendRatio = std::pow(2.0f, semitones / 12.0f); }
    void setOscMode(bool on) noexcept { m_oscMode = on; }
    bool oscMode() const noexcept { return m_oscMode; }

private:
    // Internal drum oscillator bank (OSC mode). Timbres follow the SLOT knob 0..9:
    // KICK SNARE HAT TOM CLAP RIM TONE NOIZ METAL ZAP.
    void processOsc(float* outL, float* outR, size_t numFrames) {
        if (!m_playingOsc) { std::fill_n(outL, numFrames, 0.0f); std::fill_n(outR, numFrames, 0.0f); return; }
        // PTCH shifts pitch, STRT stretches/shortens the decay, key tracking like the sample path.
        const float key = std::pow(2.0f, (m_oscNote - 60.0f + std::log2(std::max(0.0625f, m_pitchRatio))) / 12.0f);
        const float stretch = 0.5f + (static_cast<float>(m_paramStrt) / 127.0f) * 1.5f;
        const int kind = m_oscKind;
        float decayBase = 0.0f;
        switch (kind) {
            case 0: decayBase = 0.30f; break;  // kick
            case 1: decayBase = 0.18f; break;  // snare
            case 2: decayBase = 0.05f; break;  // hat
            case 3: decayBase = 0.45f; break;  // tom
            case 4: decayBase = 0.22f; break;  // clap
            case 5: decayBase = 0.035f; break; // rim
            case 6: decayBase = 0.80f; break;  // tone
            case 7: decayBase = 0.40f; break;  // noise
            case 8: decayBase = 0.65f; break;  // metal
            default: decayBase = 0.09f; break; // zap
        }
        const double decaySamples = std::max(24.0, decayBase * stretch * m_sampleRate);
        for (size_t i = 0; i < numFrames; ++i) {
            if (!m_playingOsc) { outL[i] = 0.0f; outR[i] = 0.0f; continue; }
            // Retrigger restarts the envelope like the sample path.
            if (m_retrigTotal > 0 && m_retrigCounter < m_retrigTotal) {
                if (m_retrigSamplesRemaining == 0) { m_oscAge = 0.0; m_oscEnv = 1.0f; ++m_retrigCounter; m_retrigSamplesRemaining = m_retrigIntervalSamples; }
                else --m_retrigSamplesRemaining;
            }
            const float t = static_cast<float>(m_oscAge);
            const float env = std::exp(-t / static_cast<float>(decaySamples));
            float s = 0.0f;
            const double fK = 1.0 / m_sampleRate;
            switch (kind) {
                case 0: { // KICK: pitch-dropping sine + click transient
                    double f = (46.0 + 170.0 * std::exp(-t / (0.006 * m_sampleRate))) * key;
                    m_oscPhase += 2.0 * 3.14159265 * f * fK;
                    s = std::sin(m_oscPhase) * env + (t < 20.0f ? m_rngFloat() * 0.4f : 0.0f);
                    break; }
                case 1: { // SNARE: 190 Hz body + bright noise
                    m_oscPhase += 2.0 * 3.14159265 * 190.0 * key * fK;
                    s = (std::sin(m_oscPhase) * 0.45f + m_rngFloat() * 0.55f) * env;
                    break; }
                case 2: { // HAT: six square modulators, short bright tail
                    float n = 0.0f;
                    for (int k = 0; k < 6; ++k) { m_oscPhases[static_cast<size_t>(k)] += 2.0 * 3.14159265 * (2600.0 + 1500.0 * k) * key * fK;
                        n += m_oscPhases[static_cast<size_t>(k)] - std::floor(0.5 + m_oscPhases[static_cast<size_t>(k)]) > 0.0f ? 0.17f : -0.17f; }
                    s = (n + m_rngFloat() * 0.3f) * env;
                    break; }
                case 3: { // TOM
                    double f = (95.0 + 130.0 * std::exp(-t / (0.02 * m_sampleRate))) * key;
                    m_oscPhase += 2.0 * 3.14159265 * f * fK;
                    s = std::sin(m_oscPhase) * env;
                    break; }
                case 4: { // CLAP: three noise bursts, then tail
                    float burst = (t < 90.0f) ? (std::sin(t * 0.35f) > 0.0f ? 1.0f : 0.0f) : 0.35f;
                    s = m_rngFloat() * burst * env;
                    break; }
                case 5: { // RIM: two pinged tones, very short
                    m_oscPhase += 2.0 * 3.14159265 * 820.0 * key * fK;
                    m_oscPhase2 += 2.0 * 3.14159265 * 1440.0 * key * fK;
                    s = (std::sin(m_oscPhase) * 0.6f + std::sin(m_oscPhase2) * 0.4f) * env;
                    break; }
                case 6: { // TONE: held sine
                    m_oscPhase += 2.0 * 3.14159265 * 110.0 * key * fK;
                    s = std::sin(m_oscPhase) * env;
                    break; }
                case 7: { // NOIZ: filtered noise (one-pole smoothed for body)
                    m_oscNoise = m_oscNoise * 0.35f + m_rngFloat() * 0.65f;
                    s = m_oscNoise * env;
                    break; }
                case 8: { // METAL: inharmonic square-ish partials
                    static const double ratio[5] = {1.0, 1.41, 1.79, 2.61, 3.44};
                    float acc = 0.0f;
                    for (int k = 0; k < 5; ++k) { m_oscPhases[static_cast<size_t>(k + 1) & 7] += 2.0 * 3.14159265 * 640.0 * ratio[k] * key * fK;
                        const double ph = m_oscPhases[static_cast<size_t>(k + 1) & 7]; acc += (ph - std::floor(ph) < 0.5 ? 0.2 : -0.2) / (1.0 + k); }
                    s = acc * env;
                    m_oscPhase = 0.0;
                    break; }
                default: { // ZAP: FM burst, fast pitch fall
                    m_oscPhase2 += 2.0 * 3.14159265 * 520.0 * std::exp(-t / (0.02 * m_sampleRate)) * key * fK;
                    m_oscPhase += 2.0 * 3.14159265 * 240.0 * key * fK + std::sin(m_oscPhase2) * 0.35f;
                    s = std::sin(m_oscPhase) * env;
                    break; }
            }
            s *= m_oscEnv;
            outL[i] = s; outR[i] = s;
            ++m_oscAge;
            if (env < 0.0004f && m_retrigCounter >= m_retrigTotal) m_playingOsc = false;
        }
    }

    float m_rngFloat() { // deterministic xorshift noise for the OSC bank
        m_oscRng ^= m_oscRng << 13; m_oscRng ^= m_oscRng >> 17; m_oscRng ^= m_oscRng << 5;
        return (static_cast<float>(m_oscRng) / 4294967295.0f) * 2.0f - 1.0f;
    }

public:
    // Caller supplies an already decoded slot; allocation/destruction happen off the audio thread.
    void swapSample(size_t slot, SampleSlot& prepared) { std::swap(m_slots.at(slot), prepared); m_isPlaying = false; }
    const std::vector<SampleSlot>& getSlots() const noexcept { return m_slots; }

private:
    void generateDefaultDrumKit() {
        // Generate synthetic drum samples so all 10 slots work out-of-the-box
        for (size_t s = 0; s < kMaxSlots; ++s) {
            m_slots[s].loaded = true;
            m_slots[s].originalSampleRate = 44100.0;
            size_t len = 12000;
            m_slots[s].data.resize(len);

            if (s % 3 == 0) {
                // Kick drum (pitch drop sweep)
                m_slots[s].name = "Synthetic Kick " + std::to_string(s + 1);
                float phase = 0.0f;
                for (size_t n = 0; n < len; ++n) {
                    float env = std::exp(-static_cast<float>(n) / 2500.0f);
                    float freq = 50.0f + 180.0f * std::exp(-static_cast<float>(n) / 800.0f);
                    phase += 2.0f * 3.14159265f * freq / 44100.0f;
                    m_slots[s].data[n] = std::sin(phase) * env;
                }
            } else if (s % 3 == 1) {
                // Snare drum (noise burst + tone)
                m_slots[s].name = "Synthetic Snare " + std::to_string(s + 1);
                float phase = 0.0f;
                for (size_t n = 0; n < len; ++n) {
                    float env = std::exp(-static_cast<float>(n) / 1800.0f);
                    float noise = (static_cast<float>(m_rng()) / static_cast<float>(m_rng.max())) * 2.0f - 1.0f;
                    phase += 2.0f * 3.14159265f * 220.0f / 44100.0f;
                    m_slots[s].data[n] = (std::sin(phase) * 0.4f + noise * 0.6f) * env;
                }
            } else {
                // Hi-Hat / Percussion (short noise)
                m_slots[s].name = "Synthetic Hat " + std::to_string(s + 1);
                for (size_t n = 0; n < len; ++n) {
                    float env = std::exp(-static_cast<float>(n) / 600.0f);
                    float noise = (static_cast<float>(m_rng()) / static_cast<float>(m_rng.max())) * 2.0f - 1.0f;
                    m_slots[s].data[n] = noise * env;
                }
            }
        }
    }

    bool m_oscMode = false;
    bool m_playingOsc = false;
    int m_oscKind = 0;
    float m_oscNote = 60.0f, m_oscEnv = 1.0f;
    double m_oscAge = 0.0, m_oscPhase = 0.0, m_oscPhase2 = 0.0, m_oscPhase3 = 0.0, m_oscPhase4 = 0.0;
    double m_oscPhases[8] = {0,0,0,0,0,0,0,0};
    float m_oscNoise = 0.0f;
    uint32_t m_oscRng = 0x9e3779b9u;

    double m_sampleRate = 44100.0;
    std::vector<SampleSlot> m_slots;
    size_t m_activeSlot = 0;
    size_t m_currentPlayingSlot = 0;
    bool m_isPlaying = false;
    double m_playbackPos = 0.0;
    float m_bendRatio = 1.0f;
    float m_pitchRatio = 1.0f;
    float m_keyTrackRatio = 1.0f;

    uint8_t m_paramPtch = 64;
    uint8_t m_paramStrt = 0;
    uint8_t m_paramRtrg = 0;
    uint8_t m_paramRtim = 32;
    bool m_randomSlot = false;
    bool m_randomStart = false;

    uint8_t m_retrigTotal = 0;
    uint8_t m_retrigCounter = 0;
    size_t m_retrigIntervalSamples = 2000;
    size_t m_retrigSamplesRemaining = 0;

    std::mt19937 m_rng;
};

} // namespace monomachine
