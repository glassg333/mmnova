#!/usr/bin/env python3
"""Compare the firmware-free native ORACLE renderer against the 87-case report.

The checker invokes a locally built native renderer, fingerprints temporary WAVs,
and compares non-reversible measurements with the checked-in Monomodule baseline.
It never accepts a firmware argument, downloads nothing, and deletes all rendered
WAVs before exit. The baseline hashes are diagnostic; use --require-pcm-hash only
when bit identity is the explicit target.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
from capture_monomodule_oracle import expand_cases, fingerprint_wav  # noqa: E402


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def raw_csv(values: list[int]) -> str:
    return ",".join(str(int(value)) for value in values)


def numeric_match(actual: float, expected: float, absolute: float, relative: float) -> tuple[bool, float]:
    difference = abs(actual - expected)
    scale = max(abs(expected), 1.0e-12)
    return difference <= absolute + relative * scale, difference


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--native-renderer", required=True, type=Path,
                        help="firmware-free OracleNativeRenderer executable built from this source")
    parser.add_argument("--cases", type=Path, default=HERE / "fmplus_oracle_cases.json")
    parser.add_argument("--baseline", type=Path, default=HERE / "fmplus_oracle_baseline_1.32B.json")
    parser.add_argument("--out", type=Path, default=Path("/tmp/oracle_native_comparison.json"),
                        help="JSON measurement report (never contains WAV/firmware data)")
    parser.add_argument("--peak-abs", type=float, default=0.005)
    parser.add_argument("--rms-abs", type=float, default=0.005)
    parser.add_argument("--mean-abs", type=float, default=0.002)
    parser.add_argument("--frequency-abs", type=float, default=8.0)
    parser.add_argument("--frequency-rel", type=float, default=0.02)
    parser.add_argument("--require-pcm-hash", action="store_true",
                        help="also require exact 24-bit PCM SHA-256 identity")
    parser.add_argument("--allow-mismatch", action="store_true",
                        help="write/report all comparisons but exit zero even when a case misses tolerance")
    args = parser.parse_args()

    if not args.native_renderer.is_file():
        parser.error(f"native renderer does not exist: {args.native_renderer}")
    case_data = json.loads(args.cases.read_text(encoding="utf-8"))
    baseline = json.loads(args.baseline.read_text(encoding="utf-8"))
    if case_data.get("schema") != 1 or baseline.get("schema") != 1:
        parser.error("unsupported oracle corpus schema")
    expected_cases = expand_cases(case_data)
    expected_baseline_hash = sha256(args.cases)
    if baseline.get("case_matrix_sha256") != expected_baseline_hash:
        parser.error("baseline case-matrix hash does not match supplied case matrix")
    baseline_by_id = {entry["id"]: entry for entry in baseline.get("cases", [])}
    if len(expected_cases) != 87 or len(baseline_by_id) != len(expected_cases):
        parser.error("expected exactly 87 one-to-one baseline cases")

    metric_groups = {
        "peak": ("left_peak", "right_peak", args.peak_abs, 0.0),
        "rms": ("left_rms", "right_rms", args.rms_abs, 0.0),
        "early_rms": ("left_rms_early", "right_rms_early", args.rms_abs, 0.0),
        "late_rms": ("left_rms_late", "right_rms_late", args.rms_abs, 0.0),
        "mean": ("left_mean", "right_mean", args.mean_abs, 0.0),
        "frequency": ("left_estimated_frequency_hz", None, args.frequency_abs, args.frequency_rel),
    }
    reports: list[dict[str, Any]] = []
    mismatch_count = 0
    with tempfile.TemporaryDirectory(prefix="native_oracle_fmplus_") as temp_dir_text:
        temp_dir = Path(temp_dir_text)
        for index, case in enumerate(expected_cases, start=1):
            baseline_case = baseline_by_id.get(case.case_id)
            if baseline_case is None:
                raise RuntimeError(f"baseline missing case {case.case_id}")
            wav = temp_dir / f"{case.case_id}.wav"
            command = [
                str(args.native_renderer), "--machine", str(case.machine["id"]),
                "--note", str(case.note), "--dur", repr(case.duration), "--tail", repr(case.tail),
                "--syn", raw_csv(case.syn), "--out", str(wav),
            ]
            completed = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
            if completed.returncode != 0:
                raise RuntimeError(f"native renderer failed for {case.case_id} (exit {completed.returncode})\n"
                                   f"COMMAND: {' '.join(command)}\nSTDOUT:\n{completed.stdout}\nSTDERR:\n{completed.stderr}")
            observed = fingerprint_wav(wav)
            expected_signal = baseline_case["signal"]
            checks: dict[str, Any] = {
                "format": observed["channels"] == expected_signal["channels"]
                    and observed["sample_width_bits"] == expected_signal["sample_width_bits"]
                    and observed["sample_rate"] == expected_signal["sample_rate"]
                    and observed["frames"] == expected_signal["frames"],
                "pcm_sha256_equal": observed["pcm_sha256"] == expected_signal["pcm_sha256"],
            }
            deltas: dict[str, float] = {}
            for group, (left_key, right_key, absolute, relative) in metric_groups.items():
                left_ok, left_delta = numeric_match(float(observed[left_key]), float(expected_signal[left_key]), absolute, relative)
                deltas[left_key] = left_delta
                if right_key is None:
                    checks[group] = left_ok
                    continue
                right_ok, right_delta = numeric_match(float(observed[right_key]), float(expected_signal[right_key]), absolute, relative)
                deltas[right_key] = right_delta
                checks[group] = left_ok and right_ok
            passed = all(value for key, value in checks.items() if key != "pcm_sha256_equal")
            if args.require_pcm_hash:
                passed = passed and checks["pcm_sha256_equal"]
            if not passed:
                mismatch_count += 1
            reports.append({
                "id": case.case_id,
                "machine": case.machine["id"],
                "machine_index": int(case.machine["machine_index"]),
                "note": case.note,
                "syn": case.syn,
                "pass": passed,
                "checks": checks,
                "absolute_deltas": deltas,
                "expected_signal": expected_signal,
                "native_signal": observed,
            })
            print(f"[{index:02d}/87] {'PASS' if passed else 'MISS'} {case.case_id}")

    report = {
        "schema": 1,
        "firmware_embedded": False,
        "golden_audio_embedded": False,
        "case_count": len(reports),
        "case_matrix_sha256": expected_baseline_hash,
        "baseline_sha256": sha256(args.baseline),
        "native_renderer_sha256": sha256(args.native_renderer),
        "tolerances": {
            "peak_abs": args.peak_abs, "rms_abs": args.rms_abs, "mean_abs": args.mean_abs,
            "frequency_abs": args.frequency_abs, "frequency_rel": args.frequency_rel,
            "require_pcm_hash": args.require_pcm_hash,
        },
        "passed_cases": len(reports) - mismatch_count,
        "mismatched_cases": mismatch_count,
        "cases": reports,
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(f"Wrote {args.out}; {len(reports) - mismatch_count}/{len(reports)} cases within tolerance; temporary WAVs removed")
    if mismatch_count and not args.allow_mismatch:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
