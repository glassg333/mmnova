#!/usr/bin/env python3
"""Re-capture the FM+ oracle matrix and compare it to a checked-in hash/metric report.

The output report deliberately contains no .syx bytes, DSP program, PCM, or WAV
files. It only checks the local renderer against the local firmware supplied on
this invocation.
"""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any


def indexed(report: dict[str, Any]) -> dict[str, dict[str, Any]]:
    return {case["id"]: case for case in report["cases"]}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--renderer", required=True, type=Path)
    parser.add_argument("--firmware", required=True, type=Path)
    parser.add_argument("--baseline", type=Path, default=Path(__file__).with_name("fmplus_oracle_baseline_1.32B.json"))
    parser.add_argument("--capture", type=Path, default=Path(__file__).with_name("capture_monomodule_oracle.py"))
    args = parser.parse_args()

    baseline = json.loads(args.baseline.read_text(encoding="utf-8"))
    with tempfile.TemporaryDirectory(prefix="mnm_fmplus_oracle_verify_") as directory:
        observed_path = Path(directory) / "observed.json"
        command = [
            sys.executable, str(args.capture),
            "--renderer", str(args.renderer),
            "--firmware", str(args.firmware),
            "--out", str(observed_path),
        ]
        result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
        if result.returncode:
            sys.stderr.write(result.stdout + result.stderr)
            return result.returncode
        observed = json.loads(observed_path.read_text(encoding="utf-8"))

    failures: list[str] = []
    for key in ("schema", "case_matrix_sha256", "firmware_filename", "firmware_sha256", "case_count"):
        if observed.get(key) != baseline.get(key):
            failures.append(f"top-level {key}: expected {baseline.get(key)!r}, got {observed.get(key)!r}")

    expected_cases = indexed(baseline)
    observed_cases = indexed(observed)
    if set(expected_cases) != set(observed_cases):
        failures.append("case IDs differ")
    for case_id in sorted(set(expected_cases) & set(observed_cases)):
        expected = expected_cases[case_id]
        actual = observed_cases[case_id]
        for key in ("machine", "machine_index", "labels", "syn", "note", "amp", "filt", "efx", "duration_seconds", "tail_seconds", "signal"):
            if actual.get(key) != expected.get(key):
                failures.append(f"{case_id}: {key} differs")

    if failures:
        print("FM+ ORACLE VERIFY FAIL")
        for failure in failures[:50]:
            print("-", failure)
        if len(failures) > 50:
            print(f"- ... plus {len(failures) - 50} more")
        return 1
    print(f"FM+ ORACLE VERIFY PASS ({observed['case_count']} deterministic Monomodule cases; no firmware or PCM retained)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
