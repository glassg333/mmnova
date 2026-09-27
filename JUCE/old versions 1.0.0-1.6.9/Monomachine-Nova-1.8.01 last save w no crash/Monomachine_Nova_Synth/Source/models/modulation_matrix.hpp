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
    Random,
    MSEG4 = 18,   // 1.6.29: страницы MSEG 4..8 (добавляются кнопкой «+» в редакторе)
    MSEG5,
    MSEG6,
    MSEG7,
    MSEG8,
    LFO4 = 23,    // 1.8.0: LFO страницы P2 (полноценные, свои страницы p6..p8)
    LFO5,
    LFO6
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
    int8_t auxDepth = 0;              // 1.6.32: вклад AUX -64..+63, 0 = не влияет (по умолчанию)
};

class ModulationMatrix {
public:
    static constexpr size_t kMaxRoutings = 64; // 1.7.7: было 16
    static constexpr size_t kTargetCount = 164; // 1.8.0: 132..163 = P2 (SYNT/AMP/FILT/EFFX второй страницы); 1.7.8: 131 = LFO FM // 1.7.8: 131 = LFO FM (октавы, до ~20 kHz); 1.7.7: 67..130 = ROUTE 1..64 (66) + ROUTE 1..16 (67..82) -- модуляция глубины соседних маршрутов

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
        m_msegAll.fill(0.0f);
        m_stepVelocity = 0.0f;
        m_stepTranspose = 0.0f;
        m_arpGate = 0.0f;
        m_arpRate = 0.0f;
        m_random = 0.0f;
        m_msegAll.fill(0.0f);
        m_lfoOutputs.fill(0.0f);
    }

    void setNoteAndVelocity(uint8_t note, uint8_t velocity) noexcept { m_keyNote = note; m_velocity = velocity; }
    void setParamX(uint8_t x) noexcept { m_paramX = x; }
    void setParamY(uint8_t y) noexcept { m_paramY = y; }
    void setPitchWheel(float value) noexcept { m_pitchWheel = std::clamp(value, -1.0f, 1.0f); }
    void setModWheel(float value) noexcept { m_modWheel = std::clamp(value, 0.0f, 1.0f); }
    void setAftertouch(float value) noexcept { m_aftertouch = std::clamp(value, 0.0f, 1.0f); }
    void setMsegOutput(float value) noexcept { m_msegAll[0] = std::clamp(value, 0.0f, 1.0f); }
    void setMsegOutputs(float v1, float v2, float v3) noexcept { // 1.6.21: three curves
        m_msegAll[0] = std::clamp(v1, 0.0f, 1.0f); m_msegAll[1] = std::clamp(v2, 0.0f, 1.0f); m_msegAll[2] = std::clamp(v3, 0.0f, 1.0f); }
    void setMsegOutputs(const float* values, int count) noexcept { // 1.6.29: до восьми страниц
        for (int i = 0; i < 8; ++i) m_msegAll[static_cast<size_t>(i)] = (values != nullptr && i < count) ? std::clamp(values[i], 0.0f, 1.0f) : 0.0f; }
    void setStepValues(float velocity, float transpose, float gate) noexcept {
        m_stepVelocity = std::clamp(velocity, 0.0f, 1.0f);
        m_stepTranspose = std::clamp(transpose, -1.0f, 1.0f);
        m_arpGate = std::clamp(gate, 0.0f, 1.0f);
    }
    void setArpRate(float value) noexcept { m_arpRate = std::clamp(value, 0.0f, 1.0f); }
    void setRandom(float value) noexcept { m_random = std::clamp(value, -1.0f, 1.0f); }
    void setLfoOutputs(float lfo1, float lfo2, float lfo3) noexcept { m_lfoOutputs = { lfo1, lfo2, lfo3 }; }
    void setLfoOutputs6(const float* v, int n) noexcept { for (int i = 0; i < n && i < 6; ++i) m_lfoOutputs[static_cast<size_t>(i)] = v[i]; } // 1.8.0

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
    void configureRouting(size_t slot, bool enabled, ModSource src, uint8_t targetId, int8_t depth, ModPolarity polarity, uint8_t aux, int8_t auxDepth) {
        if (slot < kMaxRoutings) m_routings[slot] = { enabled, src, targetId, depth, polarity, aux, auxDepth }; // 1.6.32: + AUX DEPTH
    }

    template<typename T, size_t N>
    void evaluate(const std::array<T, N>& baseParams, std::array<T, N>& modulatedParams, float* msegDests = nullptr, float* arpDests = nullptr, float* routeExtra = nullptr, float* lfoFm = nullptr, T* base2 = nullptr, T* modulated2 = nullptr, float* lfo2Dests = nullptr) const noexcept { // 1.8.0c: +lfo2Dests (LFO4-6, цели 164..187) // 1.8.0: +P2-цели 132..163 (base2/modulated2, 32 значения); 1.7.1: +arpDests; 1.7.8: +lfoFm; 1.7.5: +routeExtra
        std::array<float, N> sums{};
        for (size_t i = 0; i < N; ++i) sums[i] = static_cast<float>(baseParams[i]);
        std::array<float, 32> sums2{}; // 1.8.0: P2-страница (SYNT/AMP/FILT/EFFX по 8)
        if (base2 != nullptr) for (size_t i = 0; i < 32; ++i) sums2[i] = static_cast<float>(base2[i]);
        if (routeExtra != nullptr) // 1.7.5: pass 1 -- источники, целящиеся в маршруты 67..82, прибавляются к ГЛУБИНЕ той строки
            for (const auto& route : m_routings) {
                if (!route.enabled || route.depth == 0 || route.targetParamId < 67 || route.targetParamId >= 131) continue; // 1.7.8: ROUTE-цели 67..130; 131 = LFO FM (ниже)
                float source = uniOf(route.source);
                source = route.polarity == ModPolarity::Bipolar ? source * 2.0f - 1.0f : source;
                if (route.auxSource > 0) source *= uniOf(static_cast<ModSource>(route.auxSource - 1)) * (static_cast<float>(route.auxDepth) / (route.auxDepth < 0 ? 64.0f : 63.0f));
                routeExtra[route.targetParamId - 67] += source * static_cast<float>(route.depth); }
        for (const auto& route : m_routings) {
            if (route.targetParamId == 131) { // 1.7.8: LFO FM -- сумма в октавах; 1.8.0c FIX: было >=131 -- ветка ГЛОТАЛА все P2-маршруты (132..163 не работали, мусорили в частоту LFO)
                if (route.enabled && route.depth != 0 && lfoFm != nullptr) {
                    float src = uniOf(route.source);
                    src = route.polarity == ModPolarity::Bipolar ? src * 2.0f - 1.0f : src;
                    if (route.auxSource > 0) src *= uniOf(static_cast<ModSource>(route.auxSource - 1)) * (static_cast<float>(route.auxDepth) / (route.auxDepth < 0 ? 64.0f : 63.0f));
                    *lfoFm += src * static_cast<float>(route.depth); }
                continue; }
            if (!route.enabled || route.depth == 0 || (route.targetParamId >= 67 && route.targetParamId <= 130)) continue; // 1.8.0c: только 67..130 (>=131 -- P2/LFO4-6, они ниже) // 1.7.5 FIX: ROUTE-цели обработаны в pass 1 -- попадание в ветку arpDests[67-64] было записью ЗА ГРАНИЦЕЙ массива
            const float depth = route.targetParamId < 67 && routeExtra != nullptr && route.targetParamId < 64
                ? std::clamp(static_cast<float>(route.depth) + routeExtra[route.targetParamId], -64.0f, 63.0f)
                : static_cast<float>(route.depth); // 1.7.5: глубина маршрута модулируется целями MATRIX (0..15 = строки)
            if (depth == 0.0f) continue;
            float source = uniOf(route.source); // 1.6.25: источник нормализован к 0..1 независимо от его родного диапазона
            source = route.polarity == ModPolarity::Bipolar ? source * 2.0f - 1.0f : source; // Serum: UNI = одна сторона 0..depth, BI = по центру -depth..+depth
            if (route.auxSource > 0) source *= uniOf(static_cast<ModSource>(route.auxSource - 1)) * (static_cast<float>(route.auxDepth) / (route.auxDepth < 0 ? 64.0f : 63.0f)); // 1.6.24/25+32: AUX SOURCE масштабирует Amount, AUX DEPTH -- величина вклада (0 = выкл)
            if (route.targetParamId >= 164) { if (lfo2Dests != nullptr) lfo2Dests[route.targetParamId - 164] += source * depth; continue; } // 1.8.0c: LFO4-6 (P2) -- цели 164..187
            if (route.targetParamId >= 132) { if (base2 != nullptr) sums2[static_cast<size_t>(route.targetParamId - 132)] += source * depth; continue; } // 1.8.0: P2; 1.8.0c FIX: ветка была НИЖЕ арп-ветки и недостижима (>=131 глотал LFO FM)
            if (route.targetParamId >= 64) { if (arpDests != nullptr) arpDests[route.targetParamId - 64] += source * depth; continue; } // 1.7.1 RATE/GATE; 1.7.5: [2] = PITCH (полутона)
            if (route.targetParamId >= 56) { if (msegDests != nullptr) msegDests[route.targetParamId - 56] += source * depth; continue; } // 1.6.29: MSEG OUT
            if (route.targetParamId >= N) continue;
            sums[route.targetParamId] += source * depth;
        }
        for (size_t i = 0; i < N; ++i) {
            if constexpr (std::is_integral_v<T>) modulatedParams[i] = static_cast<T>(std::clamp(std::round(sums[i]), 0.0f, 127.0f));
            else modulatedParams[i] = std::clamp(sums[i], 0.0f, 127.0f);
        if (modulated2 != nullptr) for (size_t i = 0; i < 32; ++i) { // 1.8.0: P2-выход
            if constexpr (std::is_integral_v<T>) modulated2[i] = static_cast<T>(std::clamp(std::round(sums2[i]), 0.0f, 127.0f));
            else modulated2[i] = std::clamp(sums2[i], 0.0f, 127.0f); }
        }
    }

    const std::vector<ModRouting>& getRoutings() const noexcept { return m_routings; }

    static std::string getDestinationName(uint8_t paramId) {
        if (paramId >= 132) return "P2 " + getDestinationName(static_cast<uint8_t>(paramId - 132)); // 1.8.0
        if (paramId == 66) return "PITCH"; // 1.7.5
        if (paramId >= 131) return "LFO FM"; // 1.7.8: частота LFO в октавах
        if (paramId >= 67) return "ROUTE " + std::to_string(paramId - 66); // 1.7.5 (в UI имена динамические: номер строки + её цель)
        if (paramId >= 64) return paramId == 64 ? "ARP RATE" : "ARP GATE"; // 1.7.1: папка ARP
        if (paramId >= 56) return "MSEG" + std::to_string(paramId - 56 + 1) + " OUT"; // 1.6.29
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
            case ModSource::LFO3: case ModSource::LFO4: case ModSource::LFO5: case ModSource::LFO6: case ModSource::PitchWheel: case ModSource::StepTranspose: case ModSource::Random:
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
            case ModSource::LFO4:          return m_lfoOutputs[3]; // 1.8.0
            case ModSource::LFO5:          return m_lfoOutputs[4];
            case ModSource::LFO6:          return m_lfoOutputs[5];
            case ModSource::PitchWheel:   return m_pitchWheel;
            case ModSource::ModWheel:     return m_modWheel;
            case ModSource::Aftertouch:   return m_aftertouch;
            case ModSource::MSEG:         return m_msegAll[0];
            case ModSource::MSEG2:        return m_msegAll[1];
            case ModSource::MSEG3:        return m_msegAll[2];
            case ModSource::MSEG4:        return m_msegAll[3]; // 1.6.29
            case ModSource::MSEG5:        return m_msegAll[4];
            case ModSource::MSEG6:        return m_msegAll[5];
            case ModSource::MSEG7:        return m_msegAll[6];
            case ModSource::MSEG8:        return m_msegAll[7];
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
    std::array<float, 8> m_msegAll{}; // 1.6.29: выходы всех страниц MSEG
    float m_stepVelocity = 0.0f, m_stepTranspose = 0.0f;
    float m_arpGate = 0.0f, m_arpRate = 0.0f, m_random = 0.0f;
    std::array<float, 6> m_lfoOutputs{}; // 1.8.0: 6 LFO (3 = P1, 3 = P2)
};

} // namespace monomachine
