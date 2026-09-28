// OracleFmTables.hpp -- private raw/control tables for MODE SYNT = oracle.
//
// This namespace deliberately owns its own factory words, ratio words, labels
// and control laws. It neither includes nor aliases MNM FIX, NEW FIX, or OLD
// FIX data. The values are documented against the local Monomodule/OS 1.32B
// behavioural oracle in ../../../../oracle/fmplus/.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace monomachine {
namespace fm_oracle {

inline constexpr std::array<uint32_t, 24> kOracleStatParRatioWord = {
    0x002000u, 0x004000u, 0x008000u, 0x00c000u,
    0x010000u, 0x018000u, 0x020000u, 0x028000u,
    0x030000u, 0x014000u, 0x038000u, 0x040000u,
    0x050000u, 0x060000u, 0x070000u, 0x080000u,
    0x0a0000u, 0x0c0000u, 0x0e0000u, 0x100000u,
    0x140000u, 0x180000u, 0x1c0000u, 0x200000u,
};

inline constexpr std::array<const char*, 24> kOracleStatParRatioLabel = {
    "1/64", "1/32", "1/16", "3/32", "1/8", "3/16",
    "1/4", "5/16", "3/8", "5/32", "7/16", "1/2",
    "5/8", "3/4", "7/8", "1", "1.25", "1.5", "1.75",
    "2", "2.5", "3", "3.5", "4",
};

// Own copies of the visible control labels, kept out of the generic machine
// metadata so an Oracle UI route never consults a retained mode's list.
inline constexpr std::array<const char*, 8> kOracleStatLabel = {"1FRQ", "1FIN", "1ENV", "1FB", "2FRQ", "2VOL", "TONE", "TUNE"};
inline constexpr std::array<const char*, 8> kOracleParLabel  = {"1FRQ", "1ENV", "2FRQ", "2ENV", "3FRQ", "3ENV", "TONE", "TUNE"};
inline constexpr std::array<const char*, 8> kOracleDynLabel  = {"1FRQ", "1FEN", "1VOL", "1VEN", "2FRQ", "2ENV", "2FB", "TUNE"};
inline constexpr std::array<const char*, 3> kOracleMachineLabel = {"FM+ STAT", "FM+ PAR", "FM+ DYN"};

inline constexpr std::array<uint8_t, 8> kOracleStatFactoryRaw = {60, 64, 80, 30, 80, 64, 98, 64};
inline constexpr std::array<uint8_t, 8> kOracleParFactoryRaw  = {60, 64, 80, 64, 102, 80, 98, 64};
inline constexpr std::array<uint8_t, 8> kOracleDynFactoryRaw  = {64, 64, 64, 64, 74, 80, 30, 64};

inline constexpr bool supportsMachine(int machineId) noexcept
{
    return machineId == 8 || machineId == 9 || machineId == 10;
}

inline constexpr const char* machineLabelForMachine(int machineId) noexcept
{
    return supportsMachine(machineId) ? kOracleMachineLabel[static_cast<size_t>(machineId - 8)] : "ORACLE";
}

inline constexpr const std::array<uint8_t, 8>* factoryRawForMachine(int machineId) noexcept
{
    switch (machineId) {
        case 8: return &kOracleStatFactoryRaw;
        case 9: return &kOracleParFactoryRaw;
        case 10: return &kOracleDynFactoryRaw;
        default: return nullptr;
    }
}

inline constexpr const std::array<const char*, 8>* labelsForMachine(int machineId) noexcept
{
    switch (machineId) {
        case 8: return &kOracleStatLabel;
        case 9: return &kOracleParLabel;
        case 10: return &kOracleDynLabel;
        default: return nullptr;
    }
}

inline constexpr const char* labelForMachine(int machineId, int knob) noexcept
{
    const auto* labels = labelsForMachine(machineId);
    return labels != nullptr && knob >= 0 && knob < 8 ? (*labels)[static_cast<size_t>(knob)] : "---";
}

inline constexpr int clampRaw(int raw) noexcept
{
    return raw < 0 ? 0 : (raw > 127 ? 127 : raw);
}

// Monomodule's FM+ UI list index: ((2 * raw + 1) * 24) >> 8.
inline constexpr int statParListIndexForRaw(int raw) noexcept
{
    return ((2 * clampRaw(raw) + 1) * 24) >> 8;
}

inline constexpr uint32_t statParRatioWordForRaw(int raw) noexcept
{
    return kOracleStatParRatioWord[static_cast<size_t>(statParListIndexForRaw(raw))];
}

inline constexpr float statParRatioForRaw(int raw) noexcept
{
    return static_cast<float>(statParRatioWordForRaw(raw)) * (1.0f / 524288.0f);
}

inline constexpr const char* statParRatioLabelForRaw(int raw) noexcept
{
    return kOracleStatParRatioLabel[static_cast<size_t>(statParListIndexForRaw(raw))];
}

// The oracle's dynamic controls are independent direct laws, not list values.
inline constexpr float dynRatio1ForRaw(int raw) noexcept
{
    return static_cast<float>(clampRaw(raw)) * (1.0f / 64.0f);
}

inline constexpr float dynRatio2ForRaw(int raw) noexcept
{
    const float first = dynRatio1ForRaw(raw);
    return first * first;
}

inline constexpr float tuneSemitonesForRaw(int raw) noexcept
{
    return static_cast<float>(clampRaw(raw) - 64) * (1.0f / 32.0f);
}

} // namespace fm_oracle
} // namespace monomachine
