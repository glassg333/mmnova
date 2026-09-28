#pragma once
// DSP mode registry — native rollback after the withdrawn 1.9.0 experimental
// imports.  Every selectable DSP block starts with its native "mnm" path;
// "old" is retained only as the pre-existing reserve implementation.

#include <cstring>
#include <string>

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
        case DspSynt:    return "mode_synt"; // legacy shared id, migrated per machine
        case DspAmp:     return "mode_amp";
        case DspFilter:  return "mode_filt";
        case DspDist:    return "mode_dist";
        case DspDelay:   return "mode_dly";
        case DspRouting: return "mode_route";
        case DspChorus:  return "mode_cho";
        default:         return "mode_unknown";
    }
}

inline std::string dspMachineModeParamId(int machineId) {
    return std::string("mode_synt_m") + std::to_string(machineId);
}

inline const char* dspSectionLabel(int section) {
    switch (section) {
        case DspSynt:    return "SYNT";
        case DspAmp:     return "AMP";
        case DspFilter:  return "FILT";
        case DspDist:    return "DIST";
        case DspDelay:   return "DLY";
        case DspRouting: return "ROUTE";
        case DspChorus:  return "CHOR";
        default:         return "?";
    }
}

inline constexpr int dspModeMnm = 0;
inline constexpr int dspModeOld = 1;
// NEW remains the original isolated exact-core candidate at index two.  The
// two later FIX values are appended, never reusing mnm/old/new automation IDs.
// They are legal only for the three FM+ machine-local SYNT parameters.
inline constexpr int dspModeNew = 2;
inline constexpr int dspModeMnmFix = 3;
inline constexpr int dspModeNewFix = 4;
inline constexpr int dspModeCount = 5;

// Schema 19 additionally reserves separate experimental MNM FIX while
// preserving schema-18 MODE S values and physical MODE L/H ordering.
// Schema 20 adds neutral opt-in filter extras and MOD ENV parameters.
// Schema 23 appends the opt-in NEW Track Delay plus neutral feedback-Q fields.
// 24 appends R 303/R MS20/R MOOG hybrid filter choices after stable IDs 0..8.
// 25 appends independent R Import 2 choices after the complete 1.9.9 0..20 range.
// 28 appends six derived HP complements after the existing MODE L/H IDs 0..51.
// 29 permits NEW=2 only for FM+ STAT/PAR/DYN's own SYNT parameters; pre-29
// values of two still migrate to MNM.  Schema 30 appends MNM FIX=3 and NEW
// FIX=4 for those same three FM+ machines; pre-30 values never acquire them.
inline constexpr int kDspModeSchemaVersion = 30; // + opt-in measured FM+ control-law FIX modes

inline const char* dspModeChoices() { return "mnm|old|new|mnm fix|new fix"; }
inline const char* dspModeName(int mode) {
    switch (mode) {
        case dspModeMnm:    return "mnm";
        case dspModeOld:    return "old";
        case dspModeNew:    return "new";
        case dspModeMnmFix: return "mnm fix";
        case dspModeNewFix: return "new fix";
        default:            return "?";
    }
}
inline const char* dspModeLabel(int mode) { return dspModeName(mode); }

inline int dspModeIndexByName(const char* name) {
    if (name == nullptr) return -1;
    for (int mode = 0; mode < dspModeCount; ++mode)
        if (std::strcmp(dspModeName(mode), name) == 0) return mode;
    return -1;
}

inline int dspModeLegacyIndexToCurrent(int legacy) noexcept {
    return legacy == dspModeOld ? dspModeOld : dspModeMnm;
}

inline constexpr bool kShowBackupModes = true;

inline const char* dspSectionModeChoices(int section) {
    switch (section) {
        // Generic SYNT callers intentionally see only its stable mnm|old
        // contract.  The per-machine helpers below add NEW solely to m8..m10.
        case DspSynt:   return "mnm|old";
        case DspAmp:    return "mnm"; // AMP owns its separate old|mnm|vital selector
        case DspFilter: return "mnm|old";
        case DspDist:   return "mnm|old";
        case DspDelay:  return "mnm|old|new"; // NEW is appended; mnm/old IDs remain stable
        case DspRouting:return "mnm";
        case DspChorus: return "mnm";
        default:        return "mnm";
    }
}

// MODE SYNT is stored once per machine.  Keep this allow-list local and
// explicit: arbitrary machines cannot pick the exact FM import or its measured
// control-law FIX profiles by raw value.
inline constexpr bool dspSyntSupportsNewForMachine(int machineId) noexcept {
    return machineId == 8 || machineId == 9 || machineId == 10;
}
inline constexpr bool dspSyntSupportsFixForMachine(int machineId) noexcept {
    return dspSyntSupportsNewForMachine(machineId);
}
inline const char* dspSyntModeChoicesForMachine(int machineId) {
    return dspSyntSupportsFixForMachine(machineId)
        ? "mnm|old|new|mnm fix|new fix" : "mnm|old";
}
inline int dspSyntModeCountForMachine(int machineId) noexcept {
    return dspSyntSupportsFixForMachine(machineId) ? 5 : 2;
}
// Only the two appended FIX selections may show/use the measured profile.
// Plain mnm, old, and new deliberately retain their own historical laws and
// faceplate readouts for A/B comparison.
inline constexpr bool dspSyntModeUsesMeasuredFix(int mode) noexcept {
    return mode == dspModeMnmFix || mode == dspModeNewFix;
}
inline bool dspSyntModeAllowedForMachine(int machineId, int mode) noexcept {
    return mode == dspModeMnm || mode == dspModeOld
        || ((mode == dspModeNew || mode == dspModeMnmFix || mode == dspModeNewFix)
            && dspSyntSupportsFixForMachine(machineId));
}

inline const char* dspSectionParameterChoices(int section) { return dspSectionModeChoices(section); }

inline int dspSectionModeCount(int section) {
    const char* list = dspSectionModeChoices(section);
    int count = 1;
    for (const char* p = list; *p != 0; ++p) if (*p == '|') ++count;
    return count;
}

inline int dspSectionModeAt(int section, int index) {
    return index >= 0 && index < dspSectionModeCount(section) ? index : dspModeMnm;
}

inline const char* dspSectionModeName(int, int mode) { return dspModeName(mode); }
inline const char* dspSectionModeLabel(int section, int mode) { return dspSectionModeName(section, mode); }

inline bool dspModeAllowedForSection(int section, int mode) {
    for (int index = 0; index < dspSectionModeCount(section); ++index)
        if (dspSectionModeAt(section, index) == mode) return true;
    return false;
}

// New instances use the retained OLD implementations for FILT/DLY while the
// mnm values remain selectable at their stable automation index zero.
inline constexpr int dspModePrimaryForSection(int section) {
    return (section == DspFilter || section == DspDelay) ? dspModeOld : dspModeMnm;
}
inline constexpr int dspModeFilter = DspFilter;

inline const char* dspSectionCategory(int section) {
    switch (section) {
        case DspSynt:    return "MACHINES (OSC)";
        case DspAmp:     return "AMPLITUDE";
        case DspFilter:  return "FILTER";
        case DspDist:    return "DISTORTION/SRR";
        case DspDelay:   return "DELAY";
        case DspRouting: return "ROUTE";
        case DspChorus:  return "CHORUS";
        default:         return "";
    }
}

inline bool dspSectionVisibleInMenu(int section) { return section != DspRouting && section != DspAmp && section != DspDist; }

inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "SYNT: mnm/old/new stay preserved; MNM FIX and NEW FIX are measured FM+ STAT/PAR/DYN control-law candidates only";
        case DspAmp:    return "AMP: selector is on AMP page: old|mnm|vital; native/default remains mnm";
        case DspFilter: return "FILT: old is the stable new-instance default; mnm remains the native A/B path";
        case DspDist:   return "DIST: migration-only mnm|old state; user selection lives in MODE S";
        case DspDelay:  return "DLY: old is the stable new-instance default; mnm remains native A/B; NEW is an opt-in Track Delay candidate with DBAS/DWID feedback filters";
        case DspRouting:return "ROUTE: EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY";
        case DspChorus: return "CHOR: mnm is the native core";
        default:        return "";
    }
}

inline const char* dspModeProvenanceForMode(int section, int mode) {
    if (mode == dspModeMnm) {
        switch (section) {
            case DspSynt:   return "SYNT mnm (primary): native recovered machine path";
            case DspAmp:    return "AMP mnm (primary): kernel envelope";
            case DspFilter: return "FILT mnm (A/B): native filter";
            case DspDist:   return "DIST mnm (migration state): MODE S selects the audible block";
            case DspDelay:  return "DLY mnm (A/B): native delay";
            case DspRouting:return "ROUTE (single): EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY";
            case DspChorus: return "CHOR mnm (primary): native core";
            default:        return "mnm (primary)";
        }
    }
    if (mode == dspModeOld) {
        switch (section) {
            case DspSynt:   return "SYNT old (reserve): previous Nova implementation where applicable";
            case DspFilter: return "FILT old (default): retained Nova biquad";
            case DspDist:   return "DIST old (migration state): preserved as explicit MODE S OLD";
            case DspDelay:  return "DLY old (default): retained Nova delay";
            default:        return "old (reserve)";
        }
    }
    if (mode == dspModeNew && section == DspDelay)
        return "DLY NEW (opt-in): frame-scheduled Track Delay candidate; DBAS/DWID shape its Korg feedback loop";
    if (mode == dspModeNew && section == DspSynt)
        return "SYNT NEW (preserved, FM+ STAT/PAR/DYN only): isolated reviewed exact-core candidate";
    if (mode == dspModeMnmFix && section == DspSynt)
        return "SYNT MNM FIX (opt-in, FM+ STAT/PAR/DYN only): preserved MNM core with measured FREQ/TUNE control laws";
    if (mode == dspModeNewFix && section == DspSynt)
        return "SYNT NEW FIX (opt-in, FM+ STAT/PAR/DYN only): exact core with measured FREQ/TUNE control laws";
    return "?";
}

inline const char* dspRouteDescription() {
    return
        "ROUTE (documented track path):\n"
        "1) SYNT or a native FX slot supplies the signal.\n"
        "2) EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY.\n"
        "3) DIST is post-filter. There is one audible DIST block, selected only by MODE S.\n"
        "4) MODE S exposes MNM, OLD, MNM FIX, FOLD, ZERO, CLAMP and appended A/B candidates MNM+OLD, MNM V2, OLD V2; older IDs remain explicit references.";
}

} // namespace monomachine
