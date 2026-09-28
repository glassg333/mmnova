#!/usr/bin/env python3
"""Static clean-room boundary checks for schema-32 MODE SYNT=oracle.

Usage:
    python3 oracle/fmplus/verify_oracle_isolation.py /path/to/product-project

This reads source text only. It verifies that Oracle's parameter namespace,
tables, DSP objects, lifecycle, UI route and test target are private, and that
no firmware payload is involved.
"""

from __future__ import annotations

import argparse
import hashlib
import sys
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("project", type=Path, help="Monomachine_Nova_Synth or _FX project directory")
    args = parser.parse_args()
    project = args.project.resolve()
    source = project / "Source"
    failures: list[str] = []

    def load(relative: str) -> str:
        path = source / relative
        if not path.is_file():
            failures.append(f"missing Source/{relative}")
            return ""
        return path.read_text(encoding="utf-8", errors="replace")

    def check(condition: bool, message: str) -> None:
        print(("OK   " if condition else "FAIL ") + message)
        if not condition:
            failures.append(message)

    modes = load("models/DspModes.hpp")
    data = load("NovaData.h")
    processor_h = load("PluginProcessor.h")
    processor = load("PluginProcessor.cpp")
    editor = load("PluginEditor.cpp")
    dsp = load("NovaDSP.h")
    tables = load("dsp/fm_oracle/OracleFmTables.hpp")
    oracle = load("dsp/fm_oracle/OracleFm.hpp")
    cmake = (project / "CMakeLists.txt").read_text(encoding="utf-8", errors="replace") if (project / "CMakeLists.txt").is_file() else ""
    tests = (project / "tests/OracleModeTests.cpp").read_text(encoding="utf-8", errors="replace") if (project / "tests/OracleModeTests.cpp").is_file() else ""

    check("dspModeOracle = 6" in modes and "kDspModeSchemaVersion = 32" in modes
          and '"mnm|old|new|mnm fix|new fix|old fix|oracle"' in modes,
          "append-only registry keeps IDs 0..5 and exposes oracle=6")
    check("oracleFmParam" in data and '"oracle_m"' in data and "isOracleFmMachine" in data
          and "factoryRawForMachine" in data and "machineLabelForMachine" in data and "labelForMachine" in data
          and "oracleMachineDefinitions" not in data,
          "APVTS declares the independent oracle_m8/m9/m10 parameter bank with private factories/labels")
    check("oracleRaw" in processor_h and "oracleRaw" in processor and "nova::oracleFmParam" in processor
          and "schema<32" in processor and "dspModeOracle" in processor
          and "oracleSyntActive" in processor and "!(oracleSyntActive&&t<8)" in processor
          and "if(oracleSyntActive&&target<8)continue" in processor,
          "processor snapshots/migrates the private bank and blocks shared SYN matrix/P-LOCK/LFO aliases")
    check("usesOracleSyntBank" in editor and "usesOracleFmReadout" in editor
          and "applyOracleFactoryDefaults" in editor and "kOracleFactoryMenuBase" in editor
          and "monomachine::fm_oracle::labelForMachine" in editor
          and "monomachine::fm_oracle::statParRatioLabelForRaw" in editor,
          "editor binds private Oracle IDs/labels/readouts/factory action only while mode 6 is selected")
    check('#include "dsp/fm_oracle/OracleFm.hpp"' in dsp
          and "OracleStatCore oracleStat" in dsp and "OracleParallelCore oraclePar" in dsp and "OracleDynamicCore oracleDyn" in dsp
          and "oracleStat.noteOn" in dsp and "oraclePar.noteOn" in dsp and "oracleDyn.noteOn" in dsp
          and "oracleStat.setPitchBend" in dsp and "oraclePar.setPitchBend" in dsp and "oracleDyn.setPitchBend" in dsp
          and "if(oracle){" in dsp and "oracleStat.processStereo" in dsp and "oraclePar.processStereo" in dsp and "oracleDyn.processStereo" in dsp,
          "NovaDSP owns separate Oracle reset/note/pitch/render lifecycle objects")

    forbidden = ("#include \"dsp/fm_fix/", "#include \"dsp/fm_new/", "#include \"dsp/fm_new_fix/",
                 "#include \"dsp/mnm/", "MnmFixCore", "OldFix", "FmExact", "MnmFm")
    check(all(token not in tables and token not in oracle for token in forbidden),
          "Oracle table/core headers contain no prior-mode table/core include or alias")
    check("class OracleStatCore" in oracle and "class OracleParallelCore" in oracle and "class OracleDynamicCore" in oracle
          and "factoryRawForMachine" in tables and "labelsForMachine" in tables and "machineLabelForMachine" in tables
          and "statParRatioForRaw" in tables and "dynRatio1ForRaw" in tables,
          "Oracle namespace owns STAT/PAR/DYN cores plus raw/display mapping laws")
    check("OracleModeTests" in cmake and "OracleNativeRenderer" in cmake
          and "private STAT/PAR/DYN cores have distinct native topology/state outputs" in tests
          and '#include "dsp/fm_oracle/OracleFm.hpp"' in tests,
          "CMake/test suite contains standalone isolation and corpus-renderer targets")

    root = project.parent
    corpus = root / "oracle" / "fmplus"
    cases = corpus / "fmplus_oracle_cases.json"
    baseline = corpus / "fmplus_oracle_baseline_1.32B.json"
    compare = corpus / "compare_native_oracle.py"
    expected_cases = "368681fb2d297721341b921d4196571204ff76bc2b198d7b763283658ce2819f"
    expected_baseline = "145e0e14cae6fb0ea5d187d1f6a83e6b9d09f5785a81eb330b207815ea39c3d7"
    check(cases.is_file() and baseline.is_file() and compare.is_file()
          and hashlib.sha256(cases.read_bytes()).hexdigest() == expected_cases
          and hashlib.sha256(baseline.read_bytes()).hexdigest() == expected_baseline,
          "87-case firmware-free corpus and native comparison harness retain verified fingerprints")

    if failures:
        print(f"\nORACLE ISOLATION FAIL: {len(failures)} check(s)")
        return 1
    print("\nORACLE ISOLATION PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
