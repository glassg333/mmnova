#!/usr/bin/env python3
"""Static integrity checks for the 1.9.0 native-DSP rollback.

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

    check('dspModeCount = 2' in modes, "DSP registry has exactly mnm|old")
    check('kDspModeSchemaVersion = 21' in modes,
          "DSP schema includes the schema-21 live ARP-window migration")
    for section in ("DspSynt", "DspFilter", "DspDist", "DspDelay"):
        check(re.search(rf'case {section}:\s*return "mnm\|old";', modes) is not None,
              f"{section} exposes only mnm|old")
    for forbidden in ("dspModeNew", "dspModeFma", "dspModeSyntNew"):
        check(forbidden not in modes, f"registry has no {forbidden}")

    check('"old|mnm|vital"' in data, "AMP retains old|mnm|vital")
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
    check('FilterExtraPanel' in editor and 'FilterEnvelopePage' in editor and 'ModEnvelopePage' in editor,
          "FILT RMB panel and AMP/FIL/MOD ENV overlay pages are present")
    check('setFilterExtras(filterExtrasP1Raw,chain);setFilterExtras(filterExtrasP2Raw,chain2);' in processor,
          "P1/P2 FILT extras feed independent chains")
    hybrid = text("dsp/hybrid_private/HybridDSP.hpp")
    check('processMnmPreDriven' in hybrid and 'processMnmControlled' not in hybrid
          and 'postLevel' not in hybrid and 'return retainedMnmOutput + amount' not in hybrid,
          "imported MODE S retains lower MNM half but has no synthetic blend/post-level")

    for forbidden in ("MnmNewAdapters", "MnmAmpDistNew", "newdsp::", "FmCoreExact",
                      "EnvExactCore", "mnmFilterExact", "newDelay", "newSrr",
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
    check('dspModeNew' not in delay and 'newDelay' not in delay,
          "DLY has no source-derived branch")

    check('syntModeCombo.addItem("fma"' not in editor,
          "FMA is absent from the user-facing selector")
    check('addItem("new"' not in editor, "new is absent from user-facing selectors")
    check('P2 uses the native FX-slot path.' in editor,
          "P2 UI documents the retained native FX-slot path")

    for relative in ("dsp/mnm_new", "dsp/mnm/new", "dsp/mnm/MnmAmpDistNew.hpp",
                     "models/exact_firmware_fm_dispatch.hpp"):
        check(not (source / relative).exists(), f"removed: Source/{relative}")
    for relative in ("tests/DistNewModeSchemaTests.cpp", "tests/DistNewStateTests.cpp",
                     "tests/ImportedNewModesTests.cpp", "tests/MnmAmpDistNewTests.cpp",
                     "tests/TrackDistNewIntegrationTests.cpp", "tests/MnmHeaderOdrLinkTests.cpp"):
        check(not (project / relative).exists(), f"removed: {relative}")
    for forbidden in ("DistNew", "ImportedNew", "MnmAmpDistNew", "MnmHeaderOdr"):
        check(forbidden not in cmake, f"CMake has no {forbidden} target")
    check("ModeRollbackTests" in cmake and "NovaRollbackStateTests" in cmake,
          "CMake registers rollback registry and state tests")
    check("FilterExtrasDspTests" in cmake and "ModEnvMatrixTests" in cmake,
          "CMake registers FILT-extra and MOD ENV matrix regressions")
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
        print(f"\n{len(failures)} rollback checks failed")
        return 1
    print("\nNative DSP rollback static checks PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
