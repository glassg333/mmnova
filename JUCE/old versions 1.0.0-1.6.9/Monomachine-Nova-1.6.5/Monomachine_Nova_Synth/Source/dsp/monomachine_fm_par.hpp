// Nova 1.6.5: FM+ PARALLEL (machine 9) выделен в отдельный файл, чтобы каждую
// FM-машину можно было править/удалять независимо (просьба пользователя:
// «раздели все машины… чтобы я мог удалять отдельно версию fm stat»).
//
// ИСПРАВЛЕНИЕ 1.6.5 (жалоба: «fm par OLD не работает, 1FRQ и 1ENV — маленький
// призвук при 1ENV в максимум, частота никак не меняется»):
//   * раньше индекс каждого модулятора умножался на огибающую с декаем ~20 мс,
//     поэтому через 20 мс после ноты модуляция исчезала и смена 1FRQ/2FRQ/3FRQ
//     была не слышна — оставался только короткий призвук в атаке;
//   * теперь у огибающей есть УСТОЙЧИВЫЙ ПОЛ (45% глубины ручки ENV) и медленный
//     декей 0.25/0.45/0.65 с: ручка ENV задаёт глубину модуляции, а ручка FRQ
//     слышимо меняет тембр в любой момент ноты; атака остаётся чуть ярче (плик).
#pragma once

#include <cmath>
#include <algorithm>
#include <cstdint>
#include <array>

namespace monomachine {

/** Listed fixed frequency ratios (FM+ STATIC / PARALLEL), disassembled from the
    OS 1.32B frequency lookup tables: 1/64 … 12/1. */
inline float getFmListedRatio(uint8_t index) {
    static constexpr std::array<float, 32> kListedRatios = {{
        0.015625f, // 1/64
        0.03125f,  // 1/32
        0.0625f,   // 1/16
        0.09375f,  // 3/32
        0.125f,    // 1/8
        0.1875f,   // 3/16
        0.25f,     // 1/4
        0.3125f,   // 5/16
        0.375f,    // 3/8
        0.4375f,   // 7/16
        0.5f,      // 1/2
        0.625f,    // 5/8
        0.75f,     // 3/4
        0.875f,    // 7/8
        1.0f,      // 1/1 (Fundamental)
        1.25f,     // 5/4
        1.333333f, // 4/3
        1.5f,      // 3/2
        1.666667f, // 5/3
        1.75f,     // 7/4
        2.0f,      // 2/1 (1 Octave up)
        2.25f,     // 9/4
        2.5f,      // 5/2
        2.666667f, // 8/3
        3.0f,      // 3/1
        3.5f,      // 7/2
        4.0f,      // 4/1 (2 Octaves up)
        5.0f,      // 5/1
        6.0f,      // 6/1
        7.0f,      // 7/1
        8.0f,      // 8/1 (3 Octaves up)
        12.0f      // 12/1
    }};
    const size_t idx = std::min<size_t>(index, kListedRatios.size() - 1);
    return kListedRatios[idx];
}

/** ELEKTRON MONOMACHINE FM+ PARALLEL (Machine 9 / 0x09), old ("old") engine.
    3 parallel modulators into one carrier.
    1FRQ/2FRQ/3FRQ: listed harmonic ratios; 1ENV/2ENV/3ENV: modulation depth
    (sustained floor + decaying pluck); TONE handled by the integration; TUNE fine. */
class MonomachineFmParallel {
public:
    MonomachineFmParallel() = default;

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_phaseCarrier = 0.0f;
        m_phaseMod1 = 0.0f;
        m_phaseMod2 = 0.0f;
        m_phaseMod3 = 0.0f;
        m_env1 = 0.0f;
        m_env2 = 0.0f;
        m_env3 = 0.0f;
    }

    void setParameters(uint8_t frq1, uint8_t env1, uint8_t frq2, uint8_t env2,
                       uint8_t frq3, uint8_t env3, uint8_t tone, uint8_t tune) {
        m_ratioMod1 = getFmListedRatio(static_cast<uint8_t>(frq1 / 4));
        m_index1 = (static_cast<float>(env1) / 127.0f) * 4.0f;

        m_ratioMod2 = getFmListedRatio(static_cast<uint8_t>(frq2 / 4));
        m_index2 = (static_cast<float>(env2) / 127.0f) * 4.0f;

        m_ratioMod3 = getFmListedRatio(static_cast<uint8_t>(frq3 / 4));
        m_index3 = (static_cast<float>(env3) / 127.0f) * 4.0f;

        m_toneParam = tone;
        m_tuneSemitones = (static_cast<float>(tune) - 64.0f) / 64.0f * 12.0f;
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

        for (size_t i = 0; i < numFrames; ++i) {
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
    float m_ratioMod2 = 2.0f;
    float m_index2 = 1.0f;
    float m_ratioMod3 = 3.0f;
    float m_index3 = 1.0f;
    uint8_t m_toneParam = 64;

    float m_phaseCarrier = 0.0f;
    float m_phaseMod1 = 0.0f;
    float m_phaseMod2 = 0.0f;
    float m_phaseMod3 = 0.0f;
    float m_env1 = 0.0f;
    float m_env2 = 0.0f;
    float m_env3 = 0.0f;
};

} // namespace monomachine
