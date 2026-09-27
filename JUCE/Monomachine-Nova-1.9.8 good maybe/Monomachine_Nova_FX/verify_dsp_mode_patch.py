#!/usr/bin/env python3
"""Static integrity checks for the 1.9.8 source package and retained native DSP.

Usage:
    python3 verify_dsp_mode_patch.py Source

The argument is the project's Source directory.  This check intentionally does
not replace a compiler/test run; it guards the release inventory against a
withdrawn selector or imported DSP branch being reintroduced accidentally.
"""

from __future__ import annotations

from pathlib import Path
import re
import sys


def main() -> int:
    source = Path(sys.argv[1] if len(sys.argv) > 1 else "Source").resolve()
    project = source.parent
    failures: list[str] = []

    def check(condition: bool, message: str) -> None:
        print(("OK   " if condition else "FAIL ") + message)
        if not condition:
            failures.append(message)

    def text(relative: str) -> str:
        path = source / relative
        check(path.is_file(), f"present: Source/{relative}")
        return path.read_text(encoding="utf-8", errors="replace") if path.is_file() else ""

    modes = text("models/DspModes.hpp")
    data = text("NovaData.h")
    dsp = text("NovaDSP.h")
    processor = text("PluginProcessor.cpp")
    editor = text("PluginEditor.cpp")
    dist = text("dsp/TrackDIST.inl")
    filt = text("dsp/TrackFILT.inl")
    srr = text("dsp/TrackSRR.inl")
    delay = text("dsp/TrackDelay.inl")
    cmake = (project / "CMakeLists.txt").read_text(encoding="utf-8", errors="replace")

    check('dspModeCount = 3' in modes and 'dspModeNew = 2' in modes,
          "DSP registry appends stable NEW=2 after mnm|old")
    check('kDspModeSchemaVersion = 23' in modes,
          "DSP schema 23 records appended DLY NEW and feedback-Q migration")
    for section in ("DspSynt", "DspFilter", "DspDist"):
        check(re.search(rf'case {section}:\s*return "mnm\|old";', modes) is not None,
              f"{section} retains only mnm|old")
    check(re.search(r'case DspDelay:\s*return "mnm\|old\|new";', modes) is not None,
          "DspDelay alone exposes appended mnm|old|new")
    for forbidden in ("dspModeFma", "dspModeSyntNew"):
        check(forbidden not in modes, f"registry has no {forbidden}")

    check('"old|mnm|vital"' in data, "AMP retains old|mnm|vital")
    check('defaultMode=(section==monomachine::DspFilter||section==monomachine::DspDelay)' in data
          and '"p2_mode_filt"' in data and '"p2_mode_dly"' in data
          and 'section == DspFilter || section == DspDelay' in modes,
          "new P1/P2 FILT and DLY instances default to OLD without changing mode IDs")
    check('"MNM|OLD|MNM FIX|FOLD|ZERO|CLAMP"' in data,
          "MODE S exposes separate experimental MNM FIX plus retained references")
    check('hybridDistMnmFix' in dist and 'processMnmPreDriven' in dist,
          "MNM FIX and direct pre-driven imported DIST paths are active")
    check('p2_fx_mode' not in data, "P2 experimental selector is absent from parameter layout")
    check('p2FxModeRaw' not in processor, "P2 experimental selector has no runtime field")
    check('state.removeChild(p2FxMode,nullptr)' in processor,
          "legacy p2_fx_mode is discarded while loading state")
    check('nativeMode("p2_mode_dist")' in processor and '"p2_amp_mode"' in processor,
          "P1/P2 withdrawn values are explicitly migrated")
    check('schema<20' in processor and 'filterDefaults[]{0,0,0,0,0,0,0,127,127,0,0,0}' in processor,
          "schema-20 filter-extra migration inserts neutral values")
    check('schema<21' in processor and '"arp_step_start"' in processor
          and '"arp_step_end"' in processor and 'arp_step_page_limit' in processor,
          "schema-21 migrates legacy ARP pages into the live 1..64 window")
    check('schema<22' in processor and '"plock_sync"' in processor
          and '"plock_step_start"' in processor and '"plock_step_end"' in processor,
          "schema-22 preserves legacy P-LOCK sync while adding its independent window")
    check('schema<23' in processor and 'migrateDelayMode("mode_dly")' in processor
          and 'migrateDelayMode("p2_mode_dly")' in processor
          and '"dly_dbas_q"' in processor and '"p2_dly_dwid_q"' in processor,
          "schema-23 collapses legacy DLY value 2 to MNM and adds neutral P1/P2 feedback-Q")
    check('"dly_dbas_q"' in data and '"dly_dwid_q"' in data
          and '"p2_dly_dbas_q"' in data and '"p2_dly_dwid_q"' in data,
          "DBAS/DWID optional Q parameters are registered for P1 and P2")
    check('"filt_"' in data and '"p2_filt_"' in data and '"ENV FIL"' in data,
          "P1/P2 opt-in FILT extra parameters are registered")
    check('MOD ENV1' in data and '0,35,0,1' in data and '0,36,0,1' in data,
          "four MOD ENV source IDs and AUX encoding are registered")
    check('lfoDirectTarget' in data and 'lfoPageForDirectTarget' in data
          and 'kLfoPageChoiceCount = 21' in data,
          "canonical six-LFO P1/P2 PAGE/DEST mapping is registered")
    matrix = text("models/modulation_matrix.hpp")
    check('ModEnv1 = 32' in matrix and 'ModEnv4' in matrix and 'setModEnvOutputs' in matrix,
          "matrix exposes appended MOD ENV1..4 sources")
    check('kTargetCount = 248' in matrix and 'ARP START' in matrix and 'P-LOCK END' in matrix
          and 'arpWindowDests' in matrix and 'plockWindowDests' in matrix
          and '0,248,0,1,targets' in data and 'jlimit(0,248' in processor,
          "matrix exposes ARP/P-LOCK START/END destinations 244..247 end-to-end")
    check('applyArpAndPlockWindowModulation' in processor and 'plockStepForArpEvent' in processor
          and 'plockStep(plockStepForArpEvent(arp.stepEcho))' in processor,
          "DSP applies independent P-LOCK cursor/window timing on ARP note events")
    check('FilterExtraPanel' in editor and 'FilterEnvelopePage' in editor and 'ModEnvelopePage' in editor,
          "FILT RMB panel and AMP/FIL/MOD ENV overlay pages are present")
    check('setFilterExtras(filterExtrasP1Raw,chain);setFilterExtras(filterExtrasP2Raw,chain2);' in processor,
          "P1/P2 FILT extras feed independent chains")
    hybrid = text("dsp/hybrid_private/HybridDSP.hpp")
    check('processMnmPreDriven' in hybrid and 'processMnmControlled' not in hybrid
          and 'postLevel' not in hybrid and 'return retainedMnmOutput + amount' not in hybrid,
          "imported MODE S retains lower MNM half but has no synthetic blend/post-level")
    real_filter = text("dsp/mnm/MnmRealFilter.hpp")
    check('sampleRateConfigured_' in real_filter
          and 'std::abs(nextHost - host_) < 1.0e-9' in real_filter
          and 'sampleRateConfigured_ = true;' in real_filter,
          "unchanged sample rate is a no-op for stateful imported filter adapters")
    set_modes_match = re.search(
        r'void setModes\(int filterModeIn,int distModeIn,int delayModeIn,int routingModeIn\)\s*\{(.*?)\n    \}',
        dsp, re.S)
    check(set_modes_match is not None and 'mnmFilter.setSampleRate' not in set_modes_match.group(1),
          "per-block DSP-mode snapshots do not re-prepare imported filter state")

    for forbidden in ("MnmNewAdapters", "MnmAmpDistNew", "newdsp::", "FmCoreExact",
                      "EnvExactCore", "mnmFilterExact", "newSrr",
                      "softFmSat", "softFmDist", "fmaActive"):
        check(forbidden not in dsp, f"NovaDSP has no active {forbidden}")
    check('bbox.setFirmwareStart(false);bbox.setFirmwarePitch(false);' in dsp,
          "BBOX uses retained native laws")
    check('dspModeNew' not in dist and 'softFmDist' not in dist,
          "DIST has no FMA/source-derived branch")
    check('dspModeNew' not in filt and 'mnmFilterExact' not in filt,
          "FILT has no source-derived branch")
    check('dspModeNew' not in srr and 'newSrr' not in srr,
          "SRR has no source-derived branch")
    check('dspModeNew' in delay and 'newDelay.process' in delay
          and 'delayFilter.processStereo' in delay
          and 'delayBaseQ' in delay and 'delayWidthQ' in delay,
          "DLY dispatches the opt-in NEW candidate and routes DBAS/DWID/Q to all delay paths")
    feedback_filter = text("dsp/mnm/MnmDelayFeedbackFilter.hpp")
    new_delay = text("dsp/mnm/MnmTrackDelayNew.hpp")
    check('Korg35Stereo' in feedback_filter and 'active()' in feedback_filter
          and 'processStereo(float& left, float& right)' in feedback_filter,
          "shared Korg feedback bridge has neutral bypass and independent stereo state")
    check('kControlFrame = 16' in new_delay and 'DelayFeedbackFilter' in new_delay,
          "NEW delay is an independent 16-frame Track Delay candidate")
    check('MnmTrackDelayNew.hpp' in dsp and 'setDelayFeedbackQ' in dsp
          and 'newDelay' in dsp and 'setFeedbackFilterParameters' in dsp,
          "NovaDSP owns NEW delay and delivers independent feedback-Q snapshots")

    check('syntModeCombo.addItem("fma"' not in editor,
          "FMA is absent from the user-facing selector")
    check('DelayFeedbackFilterPanel' in editor and 'showDelayFeedbackQ' in editor
          and 'openDelayFeedbackQ' in editor and 'DBAS Q' in editor and 'DWID Q' in editor,
          "right-click DBAS/DWID exposes the optional matching feedback-Q panel")
    check('P2 uses the native FX-slot path.' in editor,
          "P2 UI documents the retained native FX-slot path")
    check('lane.setBounds(21, 97, 1129, 197)' in editor
          and 'plck.setBounds(21, 341, 1129, 96)' in editor
          and 'aimBtn.setBounds(399, 312, 40, 26)' in editor
          and 'slotBtn.setBounds(439, 312, 159, 26)' in editor
          and 'polBtn.setBounds(598, 312, 74, 26)' in editor,
          "ARP/P-LOCK default geometry keeps a compact P-LOCK header above its lane")
    check('plockSync' in editor and 'plockStartBox' in editor and 'plockEndBox' in editor
          and 'resetNormalWindow' in editor and 'showBlackValueEntry' in editor,
          "ARP controls implement sync, independent bounds, paired reset and black RMB entry")
    check('shiftNormalSteps' in editor and 'shiftPlockSteps' in editor
          and 'plockActiveStart(),plockActiveEnd()' in editor
          and 'normalShiftLeft.setBounds(68,295,18,22)' in editor
          and 'plockShiftLeft.setBounds(68,314,18,22)' in editor
          and 'normalShiftLeft.getBounds()' in editor and 'plockStartBox.getBounds()' in editor,
          "ARP page has compact independent shift rows with labels that follow saved layout positions")
    check('effxDlyCombo.setBounds(759,335,99,28)' in editor,
          "MODE DLY is positioned above DTIM rather than SRR")
    check('outlineLfoDestination' in editor and 'flashDirectLfoOutline' in editor
          and 'clearDirectLfoOutline' in editor and 'setDirectLfoOutline' not in editor
          and 'outlineLfoDestination(target,0)' in editor
          and 'revealLfoDestination' not in editor and 'p2Button.flashFor(180)' in editor,
          "direct LFO feedback fades once, clears on a patchcord, and P1/P2 flash/navigation is cord-drag-only")
    check('compactDisplayedValue' in editor and 'drawLfoSideBadge' in editor
          and 'full.substring(3)' in editor,
          "P1/P2 names stay canonical in lists while PAGE/DEST faceplates use a compact badge")
    check('overLfoNavigationRegion' in editor and 'updateLfoDragPageNavigation' in editor
          and 'lfoDragPageCandidate' in editor and '<170u' in editor,
          "patchcord LFO-page navigation requires a small inner-region dwell")
    check('alignmentTrack' in editor and 'normalStepTrack' in editor
          and 'x0=190' not in editor,
          "P-LOCK steps derive their width and columns from the normal ARP step track")

    for relative in ("dsp/mnm_new", "dsp/mnm/new", "dsp/mnm/MnmAmpDistNew.hpp",
                     "models/exact_firmware_fm_dispatch.hpp"):
        check(not (source / relative).exists(), f"removed: Source/{relative}")
    for relative in ("tests/DistNewModeSchemaTests.cpp", "tests/DistNewStateTests.cpp",
                     "tests/ImportedNewModesTests.cpp", "tests/MnmAmpDistNewTests.cpp",
                     "tests/TrackDistNewIntegrationTests.cpp", "tests/MnmHeaderOdrLinkTests.cpp"):
        check(not (project / relative).exists(), f"removed: {relative}")
    for forbidden in ("DistNew", "ImportedNew", "MnmAmpDistNew", "MnmHeaderOdr"):
        check(forbidden not in cmake, f"CMake has no {forbidden} target")
    check("ModeRollbackTests" in cmake and "NovaRollbackStateTests" in cmake
          and "DelayFeedbackDspTests" in cmake,
          "CMake registers rollback/state and delay-feedback DSP regressions")
    check("FilterExtrasDspTests" in cmake and "ModEnvMatrixTests" in cmake
          and "MnmRealFilterTests" in cmake,
          "CMake registers FILT-extra, real-filter continuity, and MOD ENV matrix regressions")
    check("ArpWindowTests" in cmake and "LfoMappingTests" in cmake,
          "CMake registers ARP-window and direct-LFO mapping regressions")
    if project.name.endswith("_FX"):
        check("project(MonomachineNovaUnifiedFX" in cmake
              and "BUNDLE_ID \"com.monomachinenova.unified.fx\"" in cmake
              and "PLUGIN_CODE NvFx" in cmake and "IS_SYNTH FALSE" in cmake
              and "NEEDS_MIDI_OUTPUT TRUE" in cmake and "VST3_CATEGORIES Fx" in cmake,
              "FX CMake metadata retains FX target, MIDI-out and VST3 identity")
    else:
        check("project(MonomachineNovaUnifiedSynth" in cmake
              and "BUNDLE_ID \"com.monomachinenova.unified.synth\"" in cmake
              and "PLUGIN_CODE NvSy" in cmake and "IS_SYNTH TRUE" in cmake
              and "NEEDS_MIDI_OUTPUT FALSE" in cmake and "VST3_CATEGORIES Instrument Synth" in cmake,
              "Synth CMake metadata retains Synth target and VST3 identity")

    if failures:
        print(f"\n{len(failures)} static integrity checks failed")
        return 1
    print("\nDSP/static integrity checks PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
