#pragma once
// DSP mode registry -- Monomachine Nova 1.6.1 patched
//
// User requirement:
//   "новые режимы дсп добавь как новые открывай их по умолчанию в список [mode: [mnm]] ..."
//   Adds filter-only modes cascade/dual/raw while keeping mnm=0 old=1 stable for saved-state compatibility.

#include <array>
#include <cstdint>

namespace monomachine {

enum DspSection : int {
    DspSynt = 0,
    DspAmp,
    DspFilter,
    DspDist,
    DspDelay,
    DspRouting,
    DspChorus,
    DspSectionCount
};

inline const char* dspModeParamId(int section) {
    switch (section) {
        case DspSynt:   return "mode_synt";
        case DspAmp:    return "mode_amp";
        case DspFilter: return "mode_filt";
        case DspDist:   return "mode_dist";
        case DspDelay:  return "mode_dly";
        case DspRouting:return "mode_route";
        case DspChorus: return "mode_cho";
        default:        return "mode_unknown";
    }
}

inline const char* dspSectionLabel(int section) {
    switch (section) {
        case DspSynt:   return "SYNT";
        case DspAmp:    return "AMP";
        case DspFilter: return "FILT";
        case DspDist:   return "DIST";
        case DspDelay:  return "DLY";
        case DspRouting:return "ROUTE";
        case DspChorus: return "CHOR";
        default:        return "?";
    }
}

inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "FM+STAT / FM+PAR / FM+DYN: recovered kernels; other machines still old";
        case DspAmp:    return "kernel P:$4A8-$4F5 envelope FSM with tables Y:$141800 / Y:$141880";
        case DspFilter: return "kernel P:$4A8-$5FB 2-pole SVF with P:$144AC7 coefficient tables + cascade/dual/raw";
        case DspDist:   return "ALU saturation stage (bset #$14,sr)";
        case DspDelay:  return "m32 FX-DLY: X:$114000 buffer + P:$144C49 reciprocal time table";
        case DspRouting:return "firmware chain order: osc -> AMP env -> dist -> SVF -> delay send";
        case DspChorus: return "OS 1.32B chorus core (already shipped unchanged)";
        default:        return "";
    }
}

// The list itself. 0=mnm, 1=old stay stable. New filter-only modes appended.
inline const char* dspModeChoices() { return "mnm|old|cascade|dual|raw"; }
inline constexpr int dspModeMnm = 0;
inline constexpr int dspModeOld = 1;
inline constexpr int dspModeCascade = 2;
inline constexpr int dspModeDual = 3;
inline constexpr int dspModeRaw = 4;
inline constexpr int dspModeCount = 5;

// Helper for GUI / processor: filter-only modes must collapse for non-FILT sections.
inline constexpr int dspModeFilter = DspFilter;
inline bool dspModeIsFilterOnly(int mode) noexcept {
    return mode == dspModeCascade || mode == dspModeDual || mode == dspModeRaw;
}

inline const char* dspModeName(int mode) {
    switch (mode) {
        case 0: return "mnm";
        case 1: return "old";
        case 2: return "cascade";
        case 3: return "dual";
        case 4: return "raw";
        default: return "?";
    }
}

}  // namespace monomachine
