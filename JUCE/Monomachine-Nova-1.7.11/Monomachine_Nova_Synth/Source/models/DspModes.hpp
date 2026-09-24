#pragma once
// DSP mode registry -- Monomachine Nova 1.6.8 (revision v8)
//
// WHAT CHANGED IN v8 (1.6.8 user requests):
//   * All menu / tooltip texts are ENGLISH now. The old \\xNN UTF-8 escapes went
//     through juce::String(const char*) which copies bytes as Latin-1 -- that is
//     exactly why the DSP menu showed mojibake. The pixel font has no Cyrillic
//     glyphs anyway. One deliberate ENCODING TEST line (the word "proverka" in
//     the old broken encoding) is added to the DSP MODE menu so the broken
//     pipeline can still be detected if it ever comes back.
//   * The SYNT section remembers its mode PER MACHINE: mode_synt_m<id> (one
//     choice parameter per machine). Flipping through the machines keeps one
//     machine on "old" and another on "mnm". The legacy shared "mode_synt"
//     parameter only survives inside old states and is migrated (schema 11).
//   * Removing "old" or "mnm" for a whole section is still a one-line edit:
//     dspSectionModeChoices() below is the single source of truth.
//
// v7 history: ROUTE removed from the menu (one verified chain), FM+STAT old
// engine removed, texts grouped by section. v6: dist2/fm2/bbox2 engines deleted.
// -----------------------------------------------------------------------------
// MAIN RULE: every section has its own short mode list, always starting with
// the primary "mnm".
//
// Mode numbers:
// 0 mnm -- PRIMARY: real firmware filter (FILT), firmware cores for AMP/DLY/SYNT
// 1 old -- RESERVE: previous Nova approximations (biquad filter, tanh dist, linear delay...)

#include <array>
#include <cstdint>
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
        case DspSynt:   return "mode_synt";   // legacy shared id, migrated per machine (schema 11)
        case DspAmp:    return "mode_amp";
        case DspFilter: return "mode_filt";
        case DspDist:   return "mode_dist";
        case DspDelay:  return "mode_dly";
        case DspRouting:return "mode_route";
        case DspChorus: return "mode_cho";
        default:        return "mode_unknown";
    }
}

// 1.6.8: per-machine SYNT mode parameter. Keyed by the stable machine id, so the
// selected machine keeps its own mnm/old choice while cycling through machines.
inline std::string dspMachineModeParamId(int machineId) {
    return std::string("mode_synt_m") + std::to_string(machineId);
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

// Full mode list. Needed by the AudioParameterChoice parameter (value = index),
// so the order here = the numbering above and must not change.
inline const char* dspModeChoices() { return "mnm|old|fma"; }
inline constexpr int dspModeMnm = 0;
inline constexpr int dspModeOld = 1;
inline constexpr int dspModeFma = 2; // 1.6.14: FM + правильный DIST (мягкая сатурация вместо жёсткого фолда)
inline constexpr int dspModeCount = 3;

// State schema version: 10 -> 11 because of the per-machine SYNT mode parameters.
inline constexpr int kDspModeSchemaVersion = 13; // 1.7.8: rate 0..1 + RMODE/KEY/LPOINT; 1.7.7: MSEG-режимы + матрица 64 ON

inline const char* dspModeName(int mode) {
    switch (mode) {
        case 0: return "mnm";
        case 1: return "old";
        case 2: return "fma";
        default: return "?";
    }
}
inline const char* dspModeLabel(int mode) { return dspModeName(mode); }

inline int dspModeIndexByName(const char* name) {
    if (name == nullptr) return -1;
    for (int m = 0; m < dspModeCount; ++m)
        if (std::strcmp(dspModeName(m), name) == 0) return m;
    return -1;
}

// Mapping of mode indices from older state schemas (8 and below):
//   0 mnm -> mnm, 1 old -> old,
//   2/3/4 (cascade/dual/raw from the old patch) and 5/6/7 (dist2/fm2/bbox2) -> mnm.
// So no saved state is left with a non-existing mode.
inline int dspModeLegacyIndexToCurrent(int legacy) noexcept {
    switch (legacy) {
        case 0: return dspModeMnm;
        case 1: return dspModeOld;
        default: return dspModeMnm;
    }
}

// false -> the menu keeps only the primary (mnm) modes, the reserve is fully hidden.
inline constexpr bool kShowBackupModes = true;

// -----------------------------------------------------------------------------
// Mode lists PER SECTION. "mnm" (the section primary) is always first.
// Only these lists decide what can be enabled in a section: both the menu and
// the audio stream read them. To remove "old" from a section completely, delete
// it from its list here (and vice versa).
// -----------------------------------------------------------------------------
inline const char* dspSectionModeChoices(int section) {
    switch (section) {
        case DspSynt:    return "mnm|old|fma";   // synt: firmware FmCore, reserve = previous Nova FM; fma = 1.6.14 proper DIST for FM
        case DspAmp:     return "mnm";       // 1.6.14: amplitude -- только firmware ADSR (old удалён как бесполезный)
        case DspFilter:  return "mnm|old";   // filter: REAL firmware dump, reserve = previous approximation
        case DspDist:    return "mnm|old";   // dist: firmware ALU saturation, reserve = previous bipolarDist
        case DspDelay:   return "mnm|old";   // delay: m32 FX-DLY
        case DspRouting: return "mnm";       // the chain is verified and it is the only one
        case DspChorus:  return "mnm";       // chorus: single implementation (OS 1.32B core)
        default:         return "mnm";
    }
}

inline int dspSectionModeCount(int section) {
    const char* list = dspSectionModeChoices(section);
    int n = 0;
    for (const char* p = list; *p != 0; ++p) if (*p == '|') ++n;
    return n + 1;
}

// i-th mode of a section (0 = primary mnm). Falls back to mnm for a bad index.
inline int dspSectionModeAt(int section, int index) {
    const char* list = dspSectionModeChoices(section);
    int i = 0;
    const char* start = list;
    for (const char* p = list; ; ++p) {
        if (*p == '|' || *p == 0) {
            if (i == index) {
                std::array<char, 16> token{};
                const std::size_t len = static_cast<std::size_t>(p - start);
                const std::size_t n = len < token.size() - 1 ? len : token.size() - 1;
                for (std::size_t k = 0; k < n; ++k) token[k] = start[k];
                const int mode = dspModeIndexByName(token.data());
                return mode < 0 ? dspModeMnm : mode;
            }
            ++i;
            if (*p == 0) break;
            start = p + 1;
        }
    }
    return dspModeMnm;
}

// The single admissibility rule: a mode must be in its own section list.
inline bool dspModeAllowedForSection(int section, int mode) {
    const int n = dspSectionModeCount(section);
    for (int i = 0; i < n; ++i) if (dspSectionModeAt(section, i) == mode) return true;
    return false;
}

// The primary mode of a section is always mnm and always available.
inline constexpr int dspModePrimaryForSection(int) { return dspModeMnm; }

// Compatibility with the previous code.
inline constexpr int dspModeFilter = DspFilter;

// -----------------------------------------------------------------------------
// DSP MODE menu categories (folders): they show which machines/knobs each
// section serves. English text only -- the pixel font has no Cyrillic glyphs.
// -----------------------------------------------------------------------------
inline const char* dspSectionCategory(int section) {
    switch (section) {
        case DspSynt:    return "MACHINES (OSC)";   // FM+ STAT/PAR/DYN and the other oscillators
        case DspAmp:     return "AMPLITUDE";        // DIST/VOL knob of the AMP page + envelope
        case DspFilter:  return "FILTER";           // FILT page: BASE/WDTH/HPQ/LPQ/ATK/DEC/BOFS/WOFS
        case DspDist:    return "DISTORTION/SRR";   // DIST knob (AMP page) + SRR (EFFX page)
        case DspDelay:   return "DELAY";            // EFFX page: DTIM/DSND/DFB/DBAS/DWID
        case DspRouting: return "CHAIN";            // the block order (one, verified)
        case DspChorus:  return "CHORUS";           // FX-CHORUS machine (15)
        default:         return "";
    }
}

// 1.6.5: ROUTE is not selectable in the menu -- the chain is single and verified.
// The section stays in the state (mode_route) only for preset compatibility.
// 1.6.19: AMP больше не режим DSP-секции -- выбор огибающей (old/mnm) живёт в
// самом редакторе огибающей, поэтому из меню DSP MODE раздел AMP убран.
inline bool dspSectionVisibleInMenu(int section) { return section != DspRouting && section != DspAmp; }

// -----------------------------------------------------------------------------
// Texts (menu/tooltips). Every line starts with the name of its OWN section so
// the categories can never get mixed up again. Resonance constants are the real
// firmware ones: $F528BD = -0.084747577, $4A4DF0 = +0.581161499.
// -----------------------------------------------------------------------------
inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "SYNT: mnm = FM+ core from firmware OS 1.32B (P:$4A8-$4F5, LP tone P:$144AC7). old = previous Nova FM (FM+STAT has no reserve, machine 8 is always on mnm)";
        case DspAmp:    return "AMP: mnm = firmware envelope automat, tables Y:$141800 (attack) and Y:$141880 (decay/release). old = previous Nova approximation with curves";
        case DspFilter: return "FILT: mnm = the real filter from the firmware dump (258 coeffs P:$144AC7, SVF $F528BD/$4A4DF0, BOFS/WOFS envelope, transparent at defaults). old = previous biquad";
        case DspDist:   return "DIST: mnm = firmware saturation (hard clip +-1.0, 64 = transparent). old = previous tanh/bipolar";
        case DspDelay:  return "DLY: mnm = m32 FX-DLY line (buffer X:$114000, time table P:$144C49), wet as a separate signal, DSND=0 transparent. old = linear interpolation, DSND left = ping-pong, right = stereo";
        case DspRouting:return "ROUTE: one verified chain: synth->dist->srr->filt+eq->env->dsnd. The switch was removed from the menu in 1.6.5";
        case DspChorus: return "CHOR: mnm = native OS 1.32B core (24-bit), linear INP, dry->wet mix by MIX";
        default:        return "";
    }
}

inline const char* dspModeProvenanceForMode(int section, int mode) {
    if (mode == dspModeMnm) {
        switch (section) {
            case DspSynt:    return "SYNT mnm (primary): FM+ core from firmware OS 1.32B (P:$4A8-$4F5, LP tone P:$144AC7) -- remembered per machine";
            case DspAmp:     return "AMP mnm (primary): firmware envelope automat, tables Y:$141800/Y:$141880";
            case DspFilter:  return "FILT mnm (primary): real firmware dump, 258 coeffs + SVF $F528BD/$4A4DF0, Q 0.5..16, BOFS/WOFS envelope, transparent at defaults";
            case DspDist:    return "DIST mnm (primary): firmware saturation bset #$14,sr (hard clip +-1.0)";
            case DspDelay:   return "DLY mnm (primary): m32 FX-DLY line X:$114000 + P:$144C49, separate wet, transparent at DSND=0";
            case DspRouting: return "ROUTE (single): synth->dist->srr->filt+eq->env->dsnd (verified)";
            case DspChorus:  return "CHOR mnm (primary): native OS 1.32B 24-bit core, linear INP, dry->wet mix";
            default:         return "mnm (primary): restored firmware";
        }
    }
    if (mode == dspModeOld) {
        switch (section) {
            case DspSynt:    return "SYNT old (reserve): previous Nova FM. Warning: the FM+STAT reserve was removed in 1.6.5, machine 8 stays on mnm. Each machine remembers its own choice";
            case DspAmp:     return "AMP old (reserve): previous Nova envelope with ATK/DEC/REL curves";
            case DspFilter:  return "FILT old (reserve): previous Nova biquad filter";
            case DspDist:    return "DIST old (reserve): previous tanh/bipolar dist";
            case DspDelay:   return "DLY old (reserve): linear interpolation; DSND left = ping-pong, right = stereo";
            case DspChorus:  return "CHOR old (reserve): not used, the core is the only one";
            default:         return "old (reserve): previous Nova approximation";
        }
    }
    if (mode == dspModeFma) {
        switch (section) {
            case DspSynt:    return "SYNT fma (1.6.14): firmware FM+ core + PROPER DIST for FM machines: soft tanh saturation instead of the firmware hard fold (the fold sounded like an extra oscillator ratio). Remembered per machine";
            default:         return "fma (1.6.14): FM-only addition";
        }
    }
    return "?";
}

// -----------------------------------------------------------------------------
// 1.6.5: detailed chain description for hovering over the ROUTE line (tooltip of
// the DSP MODE button and the menu help). Explains HOW every block affects the
// sound and why at default knob values the FX chain does not change the source.
// -----------------------------------------------------------------------------
inline const char* dspRouteDescription() {
    return
        "CHAIN (one, verified, as in firmware OS 1.32B):\n"
        "1) SYNTH/machine -> the sound source (FM+, SWAVE, SID, FX-machines...).\n"
        "2) DIST: saturation. DIST=64 is fully transparent, below 64 tames the drive, above adds overdrive.\n"
        "3) SRR: sample rate reduction (SRR=0 off) -> grit as it rises.\n"
        "4) FILT+EQ: firmware filter (BASE/WDTH/HPQ/LPQ + BOFS/WOFS envelope), then peak EQ (EQF/EQG). Transparent at BASE=0/WDTH=127/Q=0 and EQG=0.\n"
        "5) ENV: amplitude envelope (ATK/HOLD/DEC/REL). In the FX build LATCH holds it open, the MIDI gate follows the notes.\n"
        "6) DSND: the send into the delay AFTER the filter and the envelope. DSND=0 send off, output = dry signal unchanged.\n"
        "Result: at DIST=64, SRR=0, default filter/EQ and DSND=0 the chain is unity, it does not change loudness or tone.\n"
        "Order matters: dist before the filter (the overdrive is tamed by the filter), the envelope after the filter (it does not modulate the cutoff), the delay at the end (repeats bypass the dist).";
}

}  // namespace monomachine
