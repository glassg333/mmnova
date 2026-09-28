#!/usr/bin/env python3
"""Capture FM+ behavioral fingerprints from an external Monomodule renderer.

This utility intentionally does not download, copy, embed, or archive an OS .syx
file. Pass a locally supplied firmware path with --firmware. By default WAVs live
only in a temporary directory and only non-reversible measurements/hashes are
written to the JSON report.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import subprocess
import tempfile
import wave
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Iterable


@dataclass(frozen=True)
class ExpandedCase:
    case_id: str
    machine: dict[str, Any]
    syn: list[int]
    note: int
    amp: list[int]
    filt: list[int]
    efx: list[int]
    duration: float
    tail: float


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def csv(values: Iterable[int]) -> str:
    return ",".join(str(int(value)) for value in values)


def rms(values: list[float]) -> float:
    return math.sqrt(sum(value * value for value in values) / len(values)) if values else 0.0


def signed_24_le(raw: bytes) -> int:
    value = raw[0] | (raw[1] << 8) | (raw[2] << 16)
    return value - (1 << 24) if value & 0x800000 else value


def fingerprint_wav(path: Path) -> dict[str, Any]:
    with wave.open(str(path), "rb") as reader:
        channels = reader.getnchannels()
        sample_width = reader.getsampwidth()
        sample_rate = reader.getframerate()
        frames = reader.getnframes()
        raw = reader.readframes(frames)
    if channels != 2 or sample_width != 3 or sample_rate != 44100:
        raise RuntimeError(f"unexpected oracle WAV format at {path}: {channels}ch/{sample_width * 8}bit/{sample_rate}Hz")
    if len(raw) != frames * channels * sample_width:
        raise RuntimeError(f"truncated WAV payload at {path}")

    left: list[float] = []
    right: list[float] = []
    left_raw = bytearray()
    right_raw = bytearray()
    for offset in range(0, len(raw), 6):
        l = raw[offset : offset + 3]
        r = raw[offset + 3 : offset + 6]
        left_raw.extend(l)
        right_raw.extend(r)
        left.append(signed_24_le(l) / 8388608.0)
        right.append(signed_24_le(r) / 8388608.0)

    # Ignore the first 1,024 samples when estimating pitch: this avoids trigger
    # and filter-settling transients while preserving a deterministic feature.
    start = min(1024, max(0, frames // 4))
    crossings = [i for i in range(start + 1, frames) if left[i - 1] < 0.0 <= left[i]]
    frequency_hz = 0.0
    if len(crossings) >= 2:
        frequency_hz = sample_rate * (len(crossings) - 1) / (crossings[-1] - crossings[0])

    early = min(1024, frames)
    late_start = max(0, frames - early)
    def rounded(value: float) -> float:
        return round(value, 12)

    return {
        "pcm_sha256": hashlib.sha256(raw).hexdigest(),
        "left_pcm_sha256": hashlib.sha256(left_raw).hexdigest(),
        "right_pcm_sha256": hashlib.sha256(right_raw).hexdigest(),
        "channels": channels,
        "sample_width_bits": sample_width * 8,
        "sample_rate": sample_rate,
        "frames": frames,
        "left_peak": rounded(max((abs(value) for value in left), default=0.0)),
        "right_peak": rounded(max((abs(value) for value in right), default=0.0)),
        "left_rms": rounded(rms(left)),
        "right_rms": rounded(rms(right)),
        "left_rms_early": rounded(rms(left[:early])),
        "right_rms_early": rounded(rms(right[:early])),
        "left_rms_late": rounded(rms(left[late_start:])),
        "right_rms_late": rounded(rms(right[late_start:])),
        "left_mean": rounded(sum(left) / len(left) if left else 0.0),
        "right_mean": rounded(sum(right) / len(right) if right else 0.0),
        "left_positive_zero_crossings": len(crossings),
        "left_estimated_frequency_hz": rounded(frequency_hz),
    }


def profile(case_data: dict[str, Any], profile_name: str) -> tuple[list[int], float, float]:
    host = case_data["host"]
    if profile_name == "held":
        return list(host["held_amp"]), float(host["held_duration_seconds"]), float(host["held_tail_seconds"])
    if profile_name == "default":
        return list(host["default_amp"]), float(host["envelope_duration_seconds"]), float(host["envelope_tail_seconds"])
    raise ValueError(f"unknown amp profile {profile_name}")


def expand_cases(case_data: dict[str, Any]) -> list[ExpandedCase]:
    host = case_data["host"]
    out: list[ExpandedCase] = []
    for family in case_data["families"]:
        for machine in case_data["machines"]:
            factory = [int(value) for value in machine["factory_raw"]]
            amp, duration, tail = profile(case_data, family["amp_profile"])
            common = {
                "machine": machine,
                "amp": amp,
                "filt": list(host["default_filt"]),
                "efx": list(host["default_efx"]),
                "duration": duration,
                "tail": tail,
            }
            if family["kind"] == "factory":
                out.append(ExpandedCase(
                    case_id=f"{family['id']}__{machine['id']}",
                    syn=factory,
                    note=int(host["note"]),
                    **common,
                ))
            elif family["kind"] == "one_syn_knob":
                for knob in range(8):
                    for raw in family["raw_values"]:
                        syn = factory.copy()
                        syn[knob] = int(raw)
                        out.append(ExpandedCase(
                            case_id=f"{family['id']}__{machine['id']}__k{knob + 1}__raw{int(raw):03d}",
                            syn=syn,
                            note=int(host["note"]),
                            **common,
                        ))
            elif family["kind"] == "note":
                for note in family["notes"]:
                    out.append(ExpandedCase(
                        case_id=f"{family['id']}__{machine['id']}__note{int(note):03d}",
                        syn=factory,
                        note=int(note),
                        **common,
                    ))
            else:
                raise ValueError(f"unknown family kind {family['kind']}")
    ids = [case.case_id for case in out]
    if len(ids) != len(set(ids)):
        raise RuntimeError("duplicate generated oracle case IDs")
    return out


def run_case(renderer: Path, firmware: Path, level: int, wav_path: Path, case: ExpandedCase) -> None:
    command = [
        str(renderer),
        "--fw", str(firmware),
        "--machine", str(case.machine["renderer_name"]),
        "--note", str(case.note),
        "--dur", repr(case.duration),
        "--tail", repr(case.tail),
        "--level", str(level),
        "--syn", csv(case.syn),
        "--amp", csv(case.amp),
        "--filt", csv(case.filt),
        "--efx", csv(case.efx),
        "--out", str(wav_path),
    ]
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
    if result.returncode != 0:
        raise RuntimeError(
            f"oracle renderer failed for {case.case_id} with exit {result.returncode}\n"
            f"COMMAND: {' '.join(command)}\nSTDOUT:\n{result.stdout}\nSTDERR:\n{result.stderr}"
        )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--renderer", required=True, type=Path, help="built external mnm-render executable")
    parser.add_argument("--firmware", required=True, type=Path, help="local OS 1.32B .syx; never copied by this script")
    parser.add_argument("--cases", type=Path, default=Path(__file__).with_name("fmplus_oracle_cases.json"))
    parser.add_argument("--out", required=True, type=Path, help="JSON report to write")
    parser.add_argument("--keep-wav-dir", type=Path, help="optional external directory for diagnostic WAVs; do not commit it")
    args = parser.parse_args()

    if not args.renderer.is_file():
        parser.error(f"renderer does not exist: {args.renderer}")
    if not args.firmware.is_file():
        parser.error(f"firmware does not exist: {args.firmware}")
    case_data = json.loads(args.cases.read_text(encoding="utf-8"))
    if case_data.get("schema") != 1:
        parser.error("unsupported cases schema")

    target_dir_context: Any
    if args.keep_wav_dir:
        args.keep_wav_dir.mkdir(parents=True, exist_ok=True)
        target_dir_context = None
        wav_dir = args.keep_wav_dir
    else:
        target_dir_context = tempfile.TemporaryDirectory(prefix="mnm_fmplus_oracle_")
        wav_dir = Path(target_dir_context.name)

    try:
        report_cases: list[dict[str, Any]] = []
        for index, case in enumerate(expand_cases(case_data), start=1):
            wav_path = wav_dir / f"{case.case_id}.wav"
            run_case(args.renderer, args.firmware, int(case_data["host"]["level"]), wav_path, case)
            report_cases.append({
                "id": case.case_id,
                "machine": case.machine["id"],
                "machine_index": int(case.machine["machine_index"]),
                "labels": case.machine["labels"],
                "syn": case.syn,
                "note": case.note,
                "amp": case.amp,
                "filt": case.filt,
                "efx": case.efx,
                "duration_seconds": case.duration,
                "tail_seconds": case.tail,
                "signal": fingerprint_wav(wav_path),
            })
            print(f"[{index:02d}] {case.case_id}")
        report = {
            "schema": 1,
            "reference": case_data["reference"],
            "case_matrix_sha256": sha256_file(args.cases),
            "renderer_sha256": sha256_file(args.renderer),
            "firmware_filename": args.firmware.name,
            "firmware_sha256": sha256_file(args.firmware),
            "firmware_embedded": False,
            "case_count": len(report_cases),
            "cases": report_cases,
        }
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        print(f"Wrote {args.out} ({len(report_cases)} cases; no WAV retained={not bool(args.keep_wav_dir)})")
    finally:
        if target_dir_context is not None:
            target_dir_context.cleanup()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
