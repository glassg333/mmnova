// FmFixTables.hpp -- measured FM+ control laws used only by MODE SYNT * FIX.
//
// The pre-existing mnm/old/new routes deliberately retain their own tables and
// output.  This header contains the opt-in correction profile requested after
// direct Monomachine/emulator comparison on 2026-09-28.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace monomachine {
namespace fm_fix {

// FM+ STAT/PAR, 24 entries addressed by the DSP's measured table-index law.
// Keep this order literal.  In particular index 9 is 5/32 after 3/8: it is an
// observed hardware/emulator sequence, not normalised into a monotonic list.
//
// This Q23-word array is the ONE canonical numerical FIX table. MNM FIX derives
// its float ratio from these same words and NEW FIX passes this exact pointer to
// its isolated core. There is no independently maintained float table that can
// drift from NEW FIX. ratio = word / 2^19.
inline constexpr std::array<uint32_t, 24> kStatParRatioWord = {
    0x002000u, 0x004000u, 0x008000u, 0x00c000u,
    0x010000u, 0x018000u, 0x020000u, 0x028000u,
    0x030000u, 0x014000u, 0x038000u, 0x040000u,
    0x050000u, 0x060000u, 0x070000u, 0x080000u,
    0x0a0000u, 0x0c0000u, 0x0e0000u, 0x100000u,
    0x140000u, 0x180000u, 0x1c0000u, 0x200000u,
};

// Display strings only: the words above remain the sole numerical source.
inline constexpr std::array<const char*, 24> kStatParRatioLabel = {
    "1/64", "1/32", "1/16", "3/32", "1/8", "3/16",
    "1/4", "5/16", "3/8", "5/32", "7/16", "1/2",
    "5/8", "3/4", "7/8", "1", "1.25", "1.5", "1.75",
    "2", "2.5", "3", "3.5", "4",
};

// Recovered factory raw profiles belong only to an explicit FIX-only user
// action. They are intentionally NOT APVTS/global parameter defaults: changing
// global defaults would alter fresh mnm/old/new comparison instances.
inline constexpr std::array<uint8_t, 8> kStatFactoryRaw = {60, 64, 80, 30, 80, 64, 98, 64};
inline constexpr std::array<uint8_t, 8> kParFactoryRaw  = {60, 64, 80, 64, 102, 80, 98, 64};
inline constexpr std::array<uint8_t, 8> kDynFactoryRaw  = {64, 64, 64, 64, 74, 80, 30, 64};

inline constexpr const std::array<uint8_t, 8>* factoryDefaultsForMachine(int machineId) noexcept
{
    switch (machineId) {
        case 8:  return &kStatFactoryRaw;
        case 9:  return &kParFactoryRaw;
        case 10: return &kDynFactoryRaw;
        default: return nullptr;
    }
}

inline constexpr int clampRaw(int raw) noexcept
{
    return raw < 0 ? 0 : (raw > 127 ? 127 : raw);
}

// DSP table fetch law: floor((K + 0.5) * 3 / 16), K = 0..127.
inline constexpr int statParRatioIndex(int raw) noexcept
{
    return (clampRaw(raw) * 6 + 3) / 32;
}

inline constexpr float statParRatioForRaw(int raw) noexcept
{
    // Keep the MNM float path numerically tied to NEW FIX's exact table words.
    return static_cast<float>(kStatParRatioWord[static_cast<size_t>(statParRatioIndex(raw))])
        * (1.0f / 524288.0f);
}

inline constexpr uint32_t statParRatioWordForRaw(int raw) noexcept
{
    return kStatParRatioWord[static_cast<size_t>(statParRatioIndex(raw))];
}

inline constexpr const char* statParRatioLabelForRaw(int raw) noexcept
{
    return kStatParRatioLabel[static_cast<size_t>(statParRatioIndex(raw))];
}

// FM+ DYN is not a STAT/PAR table.  The original DSP operations are a direct
// linear first ratio and a squared second ratio.  They match the supplied
// sweep anchors (1FRQ: 0, 1/64, 1/32 ... ~2; 2FRQ: 0, 1/64, 1/16, 1/4, 1, 4).
inline constexpr float dynRatio1ForRaw(int raw) noexcept
{
    return static_cast<float>(clampRaw(raw)) * (1.0f / 64.0f);
}

inline constexpr float dynRatio2ForRaw(int raw) noexcept
{
    const float x = dynRatio1ForRaw(raw);
    return x * x;
}

// NEW's DSP1 dynamic core takes a 0..127 word.  Its direct/squared arithmetic
// already implements the two laws above, so no remap is required for NEW FIX.
inline constexpr int dynRawForExactCore(int raw) noexcept
{
    return clampRaw(raw);
}

// Every FM+ TUNE word is centred at 64 and spans two semitones down to the
// lower endpoint and 63/32 semitones up to the upper endpoint.
inline constexpr float tuneSemitonesForRaw(int raw) noexcept
{
    return static_cast<float>(clampRaw(raw) - 64) * (1.0f / 32.0f);
}

} // namespace fm_fix
} // namespace monomachine
