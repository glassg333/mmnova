# FM+ oracle-first harness

This directory established the executable behavioural oracle before production
FM changes. It now also contains the firmware-free comparison gate used by the
in-progress native schema-32 `oracle=6` implementation.

## Boundary

The reference project is Monomodule at commit
`38403947914a2e523e6162ddd608803a4cd9bb2c`. It runs the original DSP56300
program from a **locally supplied** Elektron OS 1.32B `.syx` file.

This handoff contains none of the following:

- no `.syx` firmware;
- no unpacked DSP program/data or disassembly;
- no PCM/WAV oracle output;
- no copied Monomodule source tree or emulator library.

The firmware is only a local, temporary input to the external renderer. The
checked-in baseline retains input settings plus SHA-256 fingerprints and
non-reversible signal measurements, so a later native implementation can be
regressed without redistributing the firmware or generated audio.

## What the reference establishes

The open host/UI metadata confirms the hardware-facing FM+ contract:

| machine | hardware index | factory SYN raw values |
| --- | ---: | --- |
| `FM+ STAT` | 8 | `{60,64,80,30,80,64,98,64}` |
| `FM+ PAR` | 9 | `{60,64,80,64,102,80,98,64}` |
| `FM+ DYN` | 10 | `{64,64,64,64,74,80,30,64}` |

It also establishes that STAT/PAR FREQ controls are 24-step raw lists and that
DYN FREQ controls are direct raw readouts. The oracle matrix records the
resulting behaviour for the factory setting, every SYN knob at raw 0/64/127,
three pitches, and the default envelope path: 87 cases total.

## Reproduce the baseline locally

Build the external Monomodule command-line renderer outside this tree. Its own
README documents the DSP56300 dependency and its `mnm-render` target. Then run:

```bash
python3 oracle/fmplus/capture_monomodule_oracle.py \
  --renderer /absolute/path/to/mnm-render \
  --firmware /absolute/path/to/Elektron_SFX6-60_OS1.32B.syx \
  --out /tmp/fmplus_oracle_observed.json

python3 oracle/fmplus/verify_monomodule_oracle.py \
  --renderer /absolute/path/to/mnm-render \
  --firmware /absolute/path/to/Elektron_SFX6-60_OS1.32B.syx
```

The capture command keeps WAVs in a temporary directory by default and removes
them at completion. `--keep-wav-dir` is diagnostic-only; point it outside the
source/archive tree and never commit it.

## Native JUCE implementation contract for the next stage

Every newly added mode must be genuinely separate from earlier modes:

1. Append a new serialized mode ID; do not reinterpret or renumber old IDs.
2. Give the mode a dedicated parameter/knob bank and dedicated serialized state.
   A mode switch must not borrow or overwrite another mode's raw knob values.
3. Put all mode-specific constants and raw/display mappings in a unique table
   header/namespace. A new mode must not include or alias another mode's table.
4. Give it dedicated DSP core objects and lifecycle (`prepare`, reset, note,
   pitch, process). No inheritance, alias, or state sharing with a prior mode.
5. Add static checks that fail on cross-mode table/core/parameter-bank symbols,
   and audio tests that compare native results with this oracle matrix.

In particular, a future native OLD-FIX mode must not use `MnmFixCore`,
`FmFixTables.hpp`, a MNM/NEW knob bank, or a shared FIX factory profile.

## Native `oracle=6` comparison gate

The schema-32 `oracle` mode is rendered by the firmware-free standalone
`OracleNativeRenderer` test target. It uses only `Source/dsp/fm_oracle/` and
writes temporary 24-bit WAVs solely for measurements. Build it with
`-DNOVA_BUILD_TESTS=ON`, then compare all 87 cases:

```bash
python3 oracle/fmplus/compare_native_oracle.py \
  --native-renderer /path/to/OracleNativeRenderer \
  --out /tmp/oracle_native_comparison.json
```

The command compares sample count/format, peak, RMS, early/late RMS, mean,
and zero-crossing frequency against the firmware-free baseline report. It
returns non-zero until every case meets the selected tolerances; use
`--allow-mismatch` only to inspect an intermediate calibration report. Neither
WAV data nor a firmware payload is written into this repository. Exact PCM hash
comparison is available explicitly with `--require-pcm-hash`.

The renderer currently isolates the native FM core so its mismatches are useful
calibration evidence, not a claim of DAW/plugin or complete host-chain fidelity.
Do not describe the mode as oracle-matched until this 87-case gate passes.

## Files

- `fmplus_oracle_cases.json` — declarative, firmware-free test matrix.
- `capture_monomodule_oracle.py` — runs an external renderer and creates a
  report from hashes/measurements only.
- `fmplus_oracle_baseline_1.32B.json` — captured report for 87 cases.
- `verify_monomodule_oracle.py` — re-captures and exactly checks the report.
- `compare_native_oracle.py` — invokes the firmware-free native renderer for all
  87 cases and compares temporary-audio measurements with the baseline.
- `oracle_native_comparison_2026-09-28.json` — metrics-only initial native
  comparison audit; it contains no WAV/PCM/firmware and documents an
  in-progress, non-passing calibration result.
