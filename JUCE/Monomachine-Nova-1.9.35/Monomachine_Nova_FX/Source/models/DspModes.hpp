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
// later FIX values are appended, never reusing mnm/old/new automation IDs.
// They are legal only for the three FM+ machine-local SYNT parameters.
inline constexpr int dspModeNew = 2;
inline constexpr int dspModeMnmFix = 3;
inline constexpr int dspModeNewFix = 4;
// Appended in schema 31. Keep NEW FIX at its already serialized ID 4;
// OLD FIX therefore lives at 5 rather than being inserted into the list.
inline constexpr int dspModeOldFix = 5;
// IDs 6..8 occurred in historical saved states, but no longer name a
// renderer in shipping Source. Keep only generic migration identifiers so an
// old project is normalized safely to MNM FRQ rather than to OLD BPM.
inline constexpr int kRetiredSyntRawId6 = 6;
inline constexpr int kRetiredSyntRawId7 = 7;
inline constexpr int kRetiredSyntRawId8 = 8;
inline constexpr bool dspSyntRawIdIsRetired(int mode) noexcept {
    return mode >= kRetiredSyntRawId6 && mode <= kRetiredSyntRawId8;
}
// The selectable FM+ family deliberately ends at OLD BPM. Keep the next IDs
// free from the user-facing list until a future explicitly requested mode.
inline constexpr int dspModeCount = 6;

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
// 31 appends OLD FIX=5 without renumbering the already saved NEW FIX=4.
// 32..34 formerly appended three experimental FM import selections. Schema
// 36 retires their raw values; historical source is preserved in PATCH_HISTORY
// and no retired renderer remains in the shipping compilation graph.
// 35 restores page-local 16-step ARP/P-LOCK storage and adds SONG transport.
// 37 appends the CHORUS Core A/B choice at mode_cho=1; Native remains index 0.
// 38 appends global-default DFB dynamics / DSND feedback-polarity settings;
// all live in the DFB right-click panel and leave historic eight-knob IDs intact.
// 39 appends persistent DFB TUNE curve anchors, ceiling and live-governor
// timings. Defaults reproduce schema 38; no prior saved value is rewritten.
// 40 appended a temporary DLY CORE selector and DFB's strict RAW/64 BASE.
// 41 replaces the former raw-domain GUARD threshold with a level-domain
// START..END/PLATEAU window. 42 removes the non-working DLY CORE path again:
// serialized DLY value 3 from schema 40/41 migrates safely to NEW (2).
// 43 introduces the active RAW 64..127 safety segment through four @64/@127
// level anchors plus OFFSET. The current law bypasses GUARD and final clip at
// RAW<=63, then begins them at discrete RAW=64.
// 44 adds persisted BASE CURVE/HOLD @64: RAW=63 is exact unity, RAW 0..62
// bends upward, RAW=64 is the guard entry, and 65+ retains RAW/64. Pre-44
// states retain the neutral pair (1.00, 1.0000) as a safe lower entry.
inline constexpr int kDspModeSchemaVersion = 45;

// FM+ selector labels distinguish the retained frequency paths from the
// measured BPM paths without changing their numeric renderer IDs.
inline const char* dspModeChoices() { return "mnm frq|old frq|new frq|mnm bpm|new bpm|old bpm"; }
inline const char* dspModeName(int mode) {
    switch (mode) {
        case dspModeMnm:    return "mnm frq";
        case dspModeOld:    return "old frq";
        case dspModeNew:    return "new frq";
        case dspModeMnmFix: return "mnm bpm";
        case dspModeNewFix: return "new bpm";
        case dspModeOldFix: return "old bpm";
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
        case DspDelay:  return "mnm|old|new"; // CORE removed in schema 42; retained IDs stay stable
        case DspRouting:return "mnm";
        case DspChorus: return "native|core";
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
    if (!dspSyntSupportsFixForMachine(machineId)) return "mnm|old";
    return dspModeChoices();
}
inline int dspSyntModeCountForMachine(int machineId) noexcept {
    return dspSyntSupportsFixForMachine(machineId) ? dspModeCount : 2;
}
// The three retained BPM profiles keep their existing DSP implementations;
// only their selector labels change. Retired raw IDs have no renderer or UI
// path and are rejected by the allow-list below.
inline constexpr bool dspSyntModeUsesMeasuredFix(int mode) noexcept {
    return mode == dspModeMnmFix || mode == dspModeNewFix || mode == dspModeOldFix;
}
inline bool dspSyntModeAllowedForMachine(int machineId, int mode) noexcept {
    return mode == dspModeMnm || mode == dspModeOld
        || ((mode == dspModeNew || mode == dspModeMnmFix || mode == dspModeNewFix || mode == dspModeOldFix)
            && dspSyntSupportsFixForMachine(machineId));
}

// State migration gate: IDs 6..8 are intentionally absent. Old projects that
// contain a retired import normalize to MNM FRQ instead of falling into a
// neighbouring BPM renderer or an out-of-range APVTS value.
inline constexpr bool dspSyntModeAllowedForSchema(int machineId, int mode, int schema) noexcept {
    return mode == dspModeMnm || mode == dspModeOld
        || (schema >= 29 && mode == dspModeNew && dspSyntSupportsNewForMachine(machineId))
        || (schema >= 30 && (mode == dspModeMnmFix || mode == dspModeNewFix)
            && dspSyntSupportsFixForMachine(machineId))
        || (schema >= 31 && mode == dspModeOldFix && dspSyntSupportsFixForMachine(machineId));
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

inline const char* dspSectionModeName(int section, int mode) {
    if(section == DspSynt) return dspModeName(mode);
    if(section == DspChorus) return mode==dspModeMnm?"native":(mode==dspModeOld?"core":"?");
    // Non-SYNT sections retain their own historic MNM/OLD/NEW terminology.
    switch(mode){case dspModeMnm:return "mnm";case dspModeOld:return "old";case dspModeNew:return "new";default:return "?";}
}
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
        case DspSynt:   return "SYNT: MNM/OLD/NEW FRQ и MNM/NEW/OLD BPM — шесть сохранённых маршрутов FM+ STAT/PAR/DYN";
        case DspAmp:    return "AMP: selector is on AMP page: old|mnm|vital; native/default remains mnm";
        case DspFilter: return "FILT: old is the stable new-instance default; mnm remains the native A/B path";
        case DspDist:   return "DIST: migration-only mnm|old state; user selection lives in MODE S";
        case DspDelay:  return "DLY: old is the stable new-instance default; mnm/new are the remaining real A/B paths";
        case DspRouting:return "ROUTE: EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY";
        case DspChorus: return "CHOR: реальный A/B selector NativeChorusCore | ChorusCore через общий AudioAdapter; Native — default";
        default:        return "";
    }
}

inline const char* dspModeProvenanceForMode(int section, int mode) {
    if (mode == dspModeMnm) {
        switch (section) {
            case DspSynt:   return "SYNT mnm frq (основной): восстановленный нативный путь машины";
            case DspAmp:    return "AMP mnm (primary): kernel envelope";
            case DspFilter: return "FILT mnm (A/B): native filter";
            case DspDist:   return "DIST mnm (migration state): MODE S selects the audible block";
            case DspDelay:  return "DLY mnm (A/B): native delay";
            case DspRouting:return "ROUTE (single): EQ -> FILT -> DIST -> ENV -> VOL/PAN -> SRR -> DELAY";
            case DspChorus: return "CHOR native (default): NativeChorusCore через AudioAdapter";
            default:        return "mnm (primary)";
        }
    }
    if (mode == dspModeOld) {
        switch (section) {
            case DspSynt:   return "SYNT old frq (резерв): прежняя реализация Nova, где применимо";
            case DspFilter: return "FILT old (default): retained Nova biquad";
            case DspDist:   return "DIST old (migration state): preserved as explicit MODE S OLD";
            case DspDelay:  return "DLY old (default): retained Nova delay";
            case DspChorus: return "CHOR core (A/B): эталонный ChorusCore через тот же AudioAdapter; переключение сбрасывает tail";
            default:        return "old (reserve)";
        }
    }
    if (mode == dspModeNew && section == DspDelay)
        return "DLY NEW (opt-in): frame-scheduled Track Delay candidate; DBAS/DWID shape its Korg feedback loop";
    if (mode == dspModeNew && section == DspSynt)
        return "SYNT new frq (только FM+ STAT/PAR/DYN): изолированный проверенный candidate exact-core";
    if (mode == dspModeMnmFix && section == DspSynt)
        return "SYNT mnm bpm (только FM+ STAT/PAR/DYN): отдельный измеренный core FREQ/TUNE";
    if (mode == dspModeNewFix && section == DspSynt)
        return "SYNT new bpm (только FM+ STAT/PAR/DYN): exact core с измеренными законами FREQ/TUNE";
    if (mode == dspModeOldFix && section == DspSynt)
        return "SYNT old bpm (только FM+ STAT/PAR/DYN): отдельная топология/state OLD с измеренным FREQ/TUNE";
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
