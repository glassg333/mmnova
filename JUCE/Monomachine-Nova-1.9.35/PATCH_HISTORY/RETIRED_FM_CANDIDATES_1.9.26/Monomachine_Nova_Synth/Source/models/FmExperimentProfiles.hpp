// FmExperimentProfiles.hpp -- UI/default calibration isolated from FM DSP laws.
//
// FM+ PAR has three historical raw default words (16, 32, 48).  Their legacy,
// FIX, and package-4 audio mappings are intentionally different, so one raw
// value cannot honestly mean the same physical ratio on every route.  The UI
// profile gives the three reset positions a common visible "1" reference
// without touching any retained FM renderer or control-law table.
#pragma once

#include <array>

namespace monomachine {
namespace fm_experiments {

inline constexpr std::array<int, 3> kParFrequencyKnobs = {0, 2, 4};
inline constexpr std::array<int, 3> kParVisibleUnityDefaultRaw = {16, 32, 48};

inline constexpr int parFrequencySlot(int knob) noexcept
{
    for (int i = 0; i < static_cast<int>(kParFrequencyKnobs.size()); ++i)
        if (kParFrequencyKnobs[static_cast<size_t>(i)] == knob) return i;
    return -1;
}

inline constexpr bool isParFrequencyKnob(int knob) noexcept
{
    return parFrequencySlot(knob) >= 0;
}

inline constexpr bool parDefaultReadsUnity(int knob, int raw) noexcept
{
    const int slot = parFrequencySlot(knob);
    return slot >= 0 && raw == kParVisibleUnityDefaultRaw[static_cast<size_t>(slot)];
}


// The m6 import owns a real raw-zero ratio-table endpoint. A first explicit
// selection may therefore initialise only the affected FRQ words to zero,
// but it must not overwrite a user/programmed non-default FM state.
inline constexpr bool mnmFrqEnvFixIsFrequencyKnob(int machineId, int knob) noexcept
{
    return (machineId == 8 && (knob == 0 || knob == 4))
        || (machineId == 9 && (knob == 0 || knob == 2 || knob == 4))
        || (machineId == 10 && (knob == 0 || knob == 4));
}

inline constexpr bool mnmFrqEnvFixNeedsRawZeroInitialisation(
    int machineId, const std::array<int, 8>& raw) noexcept
{
    // These are the retained shared defaults from machine_definitions.hpp.
    // Require every FRQ word to match before treating the state as fresh: a
    // deliberate retained/FM+ program is never rewritten merely by selecting m6.
    if (machineId == 8) return raw[0] == 16 && raw[4] == 32;
    if (machineId == 9) return raw[0] == 16 && raw[2] == 32 && raw[4] == 48;
    if (machineId == 10) return raw[0] == 16 && raw[4] == 32;
    return false;
}

} // namespace fm_experiments
} // namespace monomachine
