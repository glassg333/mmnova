# MODE SYNT `oracle=6` — implementation status (2026-09-28)

## Scope completed in this source worktree

`oracle` is appended as raw SYNT mode ID **6**. IDs `0..5` remain unchanged,
including `new fix=4` and `old fix=5`; the serialization schema is **32**.
Only FM+ STAT/PAR/DYN (`m8`, `m9`, `m10`) may select it.

The implementation is clean-room and firmware-free:

- `Source/dsp/fm_oracle/OracleFmTables.hpp` owns Oracle labels, factory raw
  words, STAT/PAR list words/readouts, DYN direct laws, and tune law.
- `Source/dsp/fm_oracle/OracleFm.hpp` owns separate native
  `OracleStatCore`, `OracleParallelCore`, and `OracleDynamicCore` classes.
  They include no prior FM core/table headers and do not alias prior objects.
- APVTS registers the private serialized namespace
  `oracle_m8_0..oracle_m8_7`, `oracle_m9_0..oracle_m9_7`, and
  `oracle_m10_0..oracle_m10_7`. Their factory profiles are independent of
  `m8_*`, `m9_*`, and `m10_*`.
- Pre-schema-32 state loading seeds absent Oracle controls from those private
  factory words and refuses to reinterpret raw values as Oracle before schema
  32.
- The editor binds Oracle cells, labels, readouts, and factory action only
  while the selected machine's SYNT mode is `oracle`.
- `NovaDSP` has separate Oracle reset, note, pitch, and render branches.
  Oracle has no NEW/NEW-FIX `+10 dB` output bridge.
- Existing shared SYN matrix/P-LOCK/direct-LFO targets `0..7` are explicitly
  blocked while Oracle is selected. This avoids a retained mode's implicit
  automation/lock namespace altering Oracle's private raw bank.

Both `Monomachine_Nova_Synth` and `Monomachine_Nova_FX` contain the matching
source, test, and CMake integration. The FX target retains its original product
identity; its complete Oracle bank is serialized even though its FX machine
surface does not expose FM+ machines.

## Validation completed

Firmware-free static and standalone checks passed for both product trees:

- `verify_dsp_mode_patch.py Source`
- `oracle/fmplus/verify_oracle_isolation.py <project>`
- `oracle/fmplus/verify_no_firmware_payload.py .`
- standalone C++17 builds/runs of `ModeRollbackTests`, `FmNewModeTests`,
  `FmFixModeTests`, and `OracleModeTests`
- standalone C++17 build of `OracleNativeRenderer`

The Oracle test confirms serial-ID stability, private table laws, independent
STAT/PAR/DYN reset-note-pitch rendering, deterministic finite stereo output,
and distinct topology fingerprints. It does **not** prove hardware audio
fidelity.

## 87-case audio-comparison result

The firmware-free native renderer was run through all 87 declared oracle cases.
Its temporary WAVs were deleted after fingerprinting. The retained metrics-only
audit is:

- `oracle/fmplus/oracle_native_comparison_2026-09-28.json`
- SHA-256:
  `e75000e97309e45e3d3582121d3b390dcc8e9c1951cd568eb7c9bfc687640b5c`
- result at the documented default tolerances: **0 / 87 cases passed**.

This is intentionally recorded as a failure-to-match calibration result, not a
claim of oracle fidelity. The native cores are structurally independent and the
comparison gate is operational, but their numerical FM topology, envelope/host
chain behaviour, level calibration, and pitch/frequency response still require
iterative oracle-guided work.

## Not claimed

No JUCE plugin build, VST3 load, DAW run, editor interaction, or listening test
is claimed here. No Monomachine OS, DSP binary/program, emulator payload, PCM,
or WAV golden asset is present in this worktree or its comparison audit.

A completed delivery archive should be produced only after the 87-case native
comparison gate passes and any available JUCE-level integration validation is
actually run.
