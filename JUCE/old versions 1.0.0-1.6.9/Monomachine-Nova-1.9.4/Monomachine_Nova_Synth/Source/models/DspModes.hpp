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
inline constexpr int dspModeCount = 2;

// Schema 19 additionally reserves separate experimental MNM FIX while
// preserving schema-18 MODE S values and physical MODE L/H ordering.
// Schema 20 adds neutral opt-in filter extras and MOD ENV parameters.
inline constexpr int kDspModeSchemaVersion = 21; // 1..64 ARP window + committed PAGE RND

inline const char* dspModeChoices() { return "mnm|old"; }
inline const char* dspModeName(int mode) {
    switch (mode) {
        case dspModeMnm: return "mnm";
        case dspModeOld: return "old";
        default:         return "?";
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
        case DspSynt:   return "mnm|old";
        case DspAmp:    return "mnm"; // AMP owns its separate old|mnm|vital selector
        case DspFilter: return "mnm|old";
        case DspDist:   return "mnm|old";
        case DspDelay:  return "mnm|old";
        case DspRouting:return "mnm";
        case DspChorus: return "mnm";
        default:        return "mnm";
    }
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

inline constexpr int dspModePrimaryForSection(int) { return dspModeMnm; }
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
        case DspSynt:   return "SYNT: mnm is the native/default path; old is the retained reserve where applicable";
        case DspAmp:    return "AMP: selector is on AMP page: old|mnm|vital; native/default remains mnm";
        case DspFilter: return "FILT: mnm is the native/default filter; old is the retained reserve biquad";
        case DspDist:   return "DIST: migration-only mnm|old state; user selection lives in MODE S";
        case DspDelay:  return "DLY: mnm is the native/default delay; old is the retained reserve delay";
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
            case DspFilter: return "FILT mnm (primary): native filter";
            case DspDist:   return "DIST mnm (migration state): MODE S selects the audible block";
            case DspDelay:  return "DLY mnm (primary): native delay";
            case DspRouting:return "ROUTE (single): EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY";
            case DspChorus: return "CHOR mnm (primary): native core";
            default:        return "mnm (primary)";
        }
    }
    if (mode == dspModeOld) {
        switch (section) {
            case DspSynt:   return "SYNT old (reserve): previous Nova implementation where applicable";
            case DspFilter: return "FILT old (reserve): previous Nova biquad";
            case DspDist:   return "DIST old (migration state): preserved as explicit MODE S OLD";
            case DspDelay:  return "DLY old (reserve): previous linear delay";
            default:        return "old (reserve)";
        }
    }
    return "?";
}

inline const char* dspRouteDescription() {
    return
        "ROUTE (documented track path):\n"
        "1) SYNT or a native FX slot supplies the signal.\n"
        "2) EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY.\n"
        "3) DIST is post-filter. There is one audible DIST block, selected only by MODE S.\n"
        "4) MODE S exposes MNM, OLD, experimental MNM FIX, FOLD, ZERO and CLAMP; old remains an explicit reference.";
}

} // namespace monomachine
