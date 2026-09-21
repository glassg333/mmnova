// Nova modulation matrix. Route depth keeps the 1.5 raw -64..+63 range for
// state/automation compatibility; the editor presents the same range as 0..100%.
#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <array>
#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace monomachine {

enum class ModSource : uint8_t {
    KeyTracking = 0,
    Velocity,
    ParamX,          // legacy Macro X / CC1, centred
    ParamY,          // legacy Macro Y / CC11
    LFO1,
    LFO2,
    LFO3,
    PitchWheel,
    ModWheel,
    Aftertouch,
    MSEG,
    MSEG2 = 11,   // 1.6.25: gapless (11..17) -- выбор старших источников в списках больше не падает в дыру 11..15
    MSEG3 = 12,
    StepVelocity,
    StepTranspose,
    ArpGate,
    ArpRate,
    Random
};

enum class ModPolarity : uint8_t {
    Auto = 0,
    Unipolar = 1,
    Bipolar = 2
};

struct ModRouting {
    bool enabled = false;
    ModSource source = ModSource::Velocity;
    uint8_t targetParamId = 16;
    int8_t depth = 0;                 // -64..+63, shown as signed percent
    ModPolarity polarity = ModPolarity::Auto;
    uint8_t auxSource = 0;            // 1.6.24: 0 = OFF, иначе (ModSource+1) -- второй источник масштабирует Amount
};

class ModulationMatrix {
public:
    static constexpr size_t kMaxRoutings = 16;
    static constexpr size_t kTargetCount = 56; // 8 synth + 8 AMP + 8 FILT + 8 FX + 24 LFO

    ModulationMatrix() : m_routings(kMaxRoutings) {
        // Keep the former defaults for old tests and old projects. New routes are off.
        m_routings[0] = { true, ModSource::KeyTracking, 16, 32, ModPolarity::Auto };
        m_routings[1] = { true, ModSource::Velocity, 13, 32, ModPolarity::Auto };
        m_routings[2] = { true, ModSource::ParamX, 17, 30, ModPolarity::Auto };
        m_routings[3] = { true, ModSource::ParamY, 2, 40, ModPolarity::Auto };
    }

    void reset() {
        m_keyNote = 60;
        m_velocity = 127;
        m_paramX = 64;
        m_paramY = 0;
        m_pitchWheel = 0.0f;
        m_modWheel = 0.0f;
        m_aftertouch = 0.0f;
        m_mseg = 0.0f;
        m_stepVelocity = 0.0f;
        m_stepTranspose = 0.0f;
        m_arpGate = 0.0f;
        m_arpRate = 0.0f;
        m_random = 0.0f;
        m_lfoOutputs.fill(0.0f);
    }

    void setNoteAndVelocity(uint8_t note, uint8_t velocity) noexcept { m_keyNote = note; m_velocity = velocity; }
    void setParamX(uint8_t x) noexcept { m_paramX = x; }
    void setParamY(uint8_t y) noexcept { m_paramY = y; }
    void setPitchWheel(float value) noexcept { m_pitchWheel = std::clamp(value, -1.0f, 1.0f); }
    void setModWheel(float value) noexcept { m_modWheel = std::clamp(value, 0.0f, 1.0f); }
    void setAftertouch(float value) noexcept { m_aftertouch = std::clamp(value, 0.0f, 1.0f); }
    void setMsegOutput(float value) noexcept { m_mseg = std::clamp(value, 0.0f, 1.0f); }
    void setMsegOutputs(float v1, float v2, float v3) noexcept { // 1.6.21: three curves
        m_mseg = std::clamp(v1, 0.0f, 1.0f); m_mseg2 = std::clamp(v2, 0.0f, 1.0f); m_mseg3 = std::clamp(v3, 0.0f, 1.0f); }
    void setStepValues(float velocity, float transpose, float gate) noexcept {
        m_stepVelocity = std::clamp(velocity, 0.0f, 1.0f);
        m_stepTranspose = std::clamp(transpose, -1.0f, 1.0f);
        m_arpGate = std::clamp(gate, 0.0f, 1.0f);
    }
    void setArpRate(float value) noexcept { m_arpRate = std::clamp(value, 0.0f, 1.0f); }
    void setRandom(float value) noexcept { m_random = std::clamp(value, -1.0f, 1.0f); }
    void setLfoOutputs(float lfo1, float lfo2, float lfo3) noexcept { m_lfoOutputs = { lfo1, lfo2, lfo3 }; }

    // Five-argument overload is retained for 1.5 state/tests.
    void configureRouting(size_t slot, bool enabled, ModSource src, uint8_t targetId, int8_t depth) {
        configureRouting(slot, enabled, src, targetId, depth, ModPolarity::Auto);
    }
    void configureRouting(size_t slot, bool enabled, ModSource src, uint8_t targetId, int8_t depth, ModPolarity polarity) {
        configureRouting(slot, enabled, src, targetId, depth, polarity, 0);
    }
    void configureRouting(size_t slot, bool enabled, ModSource src, uint8_t targetId, int8_t depth, ModPolarity polarity, uint8_t aux) {
        if (slot < kMaxRoutings) m_routings[slot] = { enabled, src, targetId, depth, polarity, aux }; // 1.6.24: + aux
    }

    template<typename T, size_t N>
    void evaluate(const std::array<T, N>& baseParams, std::array<T, N>& modulatedParams) const noexcept {
        std::array<float, N> sums{};
        for (size_t i = 0; i < N; ++i) sums[i] = static_cast<float>(baseParams[i]);
        for (const auto& route : m_routings) {
            if (!route.enabled || route.targetParamId >= N || route.depth == 0) continue;
            float source = uniOf(route.source); // 1.6.25: источник нормализован к 0..1 независимо от его родного диапазона
            source = route.polarity == ModPolarity::Bipolar ? source * 2.0f - 1.0f : source; // Serum: UNI = одна сторона 0..depth, BI = по центру -depth..+depth
            if (route.auxSource > 0) source *= uniOf(static_cast<ModSource>(route.auxSource - 1)); // 1.6.24/25: AUX SOURCE масштабирует Amount
            sums[route.targetParamId] += source * static_cast<float>(route.depth);
        }
        for (size_t i = 0; i < N; ++i) {
            if constexpr (std::is_integral_v<T>) modulatedParams[i] = static_cast<T>(std::clamp(std::round(sums[i]), 0.0f, 127.0f));
            else modulatedParams[i] = std::clamp(sums[i], 0.0f, 127.0f);
        }
    }

    const std::vector<ModRouting>& getRoutings() const noexcept { return m_routings; }

    static std::string getDestinationName(uint8_t paramId) {
        if (paramId >= kTargetCount) return "NONE";
        if (paramId < 8) return "SYNTH_" + std::string(1, 'A' + paramId);
        if (paramId < 16) {
            const char* names[] = { "AMP_ATK", "AMP_HOLD", "AMP_DEC", "AMP_REL", "AMP_DIST", "AMP_VOL", "AMP_PAN", "AMP_PORT" };
            return names[paramId - 8];
        }
        if (paramId < 24) {
            const char* names[] = { "FLTR_BASE", "FLTR_WDTH", "FLTR_HPQ", "FLTR_LPQ", "FLTR_ATK", "FLTR_DEC", "FLTR_BOFS", "FLTR_WOFS" };
            return names[paramId - 16];
        }
        if (paramId < 32) {
            const char* names[] = { "FX_EQF", "FX_EQG", "FX_SRR", "FX_DTIM", "FX_DSND", "FX_DFB", "FX_DBAS", "FX_DWID" };
            return names[paramId - 24];
        }
        return "LFO" + std::to_string((paramId - 32) / 8 + 1) + " PARAM " + std::to_string((paramId - 32) % 8 + 1);
    }

private:
    float uniOf(ModSource source) const noexcept { // 1.6.25: центрированные источники (-1..1) -> 0..1, остальные уже 0..1
        switch (source) {
            case ModSource::KeyTracking: case ModSource::ParamX: case ModSource::LFO1: case ModSource::LFO2:
            case ModSource::LFO3: case ModSource::PitchWheel: case ModSource::StepTranspose: case ModSource::Random:
                return sourceValue(source) * 0.5f + 0.5f;
            default: return sourceValue(source);
        }
    }
    float sourceValue(ModSource source) const noexcept {
        switch (source) {
            case ModSource::KeyTracking:  return (static_cast<float>(m_keyNote) - 60.0f) / 60.0f;
            case ModSource::Velocity:     return static_cast<float>(m_velocity) / 127.0f;
            case ModSource::ParamX:       return (static_cast<float>(m_paramX) - 64.0f) / 64.0f;
            case ModSource::ParamY:       return static_cast<float>(m_paramY) / 127.0f;
            case ModSource::LFO1:          return m_lfoOutputs[0];
            case ModSource::LFO2:          return m_lfoOutputs[1];
            case ModSource::LFO3:          return m_lfoOutputs[2];
            case ModSource::PitchWheel:   return m_pitchWheel;
            case ModSource::ModWheel:     return m_modWheel;
            case ModSource::Aftertouch:   return m_aftertouch;
            case ModSource::MSEG:         return m_mseg;
            case ModSource::MSEG2:        return m_mseg2;
            case ModSource::MSEG3:        return m_mseg3;
            case ModSource::StepVelocity: return m_stepVelocity;
            case ModSource::StepTranspose:return m_stepTranspose;
            case ModSource::ArpGate:      return m_arpGate;
            case ModSource::ArpRate:      return m_arpRate;
            case ModSource::Random:       return m_random;
        }
        return 0.0f;
    }

    std::vector<ModRouting> m_routings;
    uint8_t m_keyNote = 60, m_velocity = 127, m_paramX = 64, m_paramY = 0;
    float m_pitchWheel = 0.0f, m_modWheel = 0.0f, m_aftertouch = 0.0f;
    float m_mseg = 0.0f, m_mseg2 = 0.0f, m_mseg3 = 0.0f, m_stepVelocity = 0.0f, m_stepTranspose = 0.0f;
    float m_arpGate = 0.0f, m_arpRate = 0.0f, m_random = 0.0f;
    std::array<float, 3> m_lfoOutputs{ 0.0f, 0.0f, 0.0f };
};

} // namespace monomachine
