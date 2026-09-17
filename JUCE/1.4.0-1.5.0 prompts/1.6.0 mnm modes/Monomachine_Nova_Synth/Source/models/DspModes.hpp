#pragma once
// DSP mode registry -- Monomachine Nova 1.6.0
//
// User requirement (prompt "1.5.2 agent read first actual prompts here and ex for check.txt"):
//   "новые режимы дсп добавь как новые открывай их по умолчанию в список [mode: [mnm]] ...
//    и старые алгоритмы что бы можно было с mnm на old переключить"
//   "рядом с synth page 1 секцией сделать выбор нового дсп mode"
//   "[mode: [mnm]] (не надо повторять этот убогий интерфейс скобок я пишу так что бы было
//    понятно как сделать список)"
//
// Rules that this registry encodes:
//   * every section that owns DSP has its own mode list parameter id ("mode_filt", ...);
//   * entry 0 of every list is "mnm" -- the recovered OS 1.32B firmware algorithm, and it is
//     the default for a fresh instance;
//   * entry 1 is "old" -- the previous Nova implementation, kept for A/B and rollback;
//   * a state saved before these parameters existed must NOT silently switch to the new DSP:
//     PluginProcessor::setStateInformation() rewrites every mode parameter to "old" (index 1)
//     when the incoming state carries no "mode_*" parameters.
#pragma once

#include <array>
#include <cstdint>

namespace monomachine {

enum DspSection : int {
    DspSynt = 0,   // SYNT page: oscillator / FM engine of the selected machine
    DspAmp,        // AMP page: ATK/HOLD/DEC/REL envelope
    DspFilter,     // FILT page: BASE/WDTH/HPQ/LPQ/BOFS/WOFS
    DspDist,       // EFFX page: DIST (saturation stage)
    DspDelay,      // EFFX page: DTIM/DSND/DFB/DBAS/DWID
    DspRouting,    // chain order: where the AMP envelope sits relative to filter and delay
    DspChorus,     // FX-CHO machine
    DspSectionCount
};

// Parameter id of each mode list ("mode_synt" ... "mode_cho").
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

// Short label used by the UI list and by the DSP-mode page.
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

// What the "mnm" entry currently covers. Sections listed as partial fall back to the old
// algorithm for the machine types that are not ported yet (see АУДИТ_И_ПЛАН.md).
inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "FM+STAT / FM+PAR / FM+DYN: recovered kernels; other machines still old";
        case DspAmp:    return "kernel P:$4A8-$4F5 envelope FSM with tables Y:$141800 / Y:$141880";
        case DspFilter: return "kernel P:$4A8-$5FB 2-pole SVF with P:$144AC7 coefficient tables";
        case DspDist:   return "ALU saturation stage (bset #$14,sr)";
        case DspDelay:  return "m32 FX-DLY: X:$114000 buffer + P:$144C49 reciprocal time table";
        case DspRouting:return "firmware chain order: osc -> AMP env -> dist -> SVF -> delay send";
        case DspChorus: return "OS 1.32B chorus core (already shipped unchanged)";
        default:        return "";
    }
}

// The list itself. Both entries always exist; add new algorithms as extra entries here and the
// parameter, the UI list and the state serialisation follow automatically.
inline const char* dspModeChoices() { return "mnm|old"; }
inline constexpr int dspModeMnm = 0;
inline constexpr int dspModeOld = 1;
inline constexpr int dspModeCount = 2;

}  // namespace monomachine
