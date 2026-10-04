# FM+ oracle-first clean-room handoff — 2026-09-28

## User-selected scope

This is the **oracle-first** stage for the current FM+ work, with the original
OS 1.32B used only locally as a behavioural oracle. It is not a firmware bundle
and not a claim that the native JUCE rewrite is complete.

The external reference used during this stage is Monomodule:

```text
repository: glassg333/mmnova
path:       decompiled data/monomod
commit:     38403947914a2e523e6162ddd608803a4cd9bb2c
```

Its source makes an important distinction: it does not provide a native FM+
C++ implementation to transplant. It feeds 52-word host blocks into an
emulated DSP56300 running the locally supplied OS program. It is therefore the
correct audio/control oracle, not source code to copy into this project.

## Established reference facts

The reference host/UI metadata establishes these hardware-facing values:

| hardware machine | index | factory SYN raw values |
| --- | ---: | --- |
| FM+ STAT | 8 | `{60,64,80,30,80,64,98,64}` |
| FM+ PAR | 9 | `{60,64,80,64,102,80,98,64}` |
| FM+ DYN | 10 | `{64,64,64,64,74,80,30,64}` |

It also confirms 24-entry raw-list handling for STAT/PAR frequency controls and
raw direct readouts for DYN frequency controls. These facts explain the
measured factory profile; they do not license sharing those tables with a new
native mode.

## Captured oracle corpus

`oracle/fmplus/` contains a firmware-free 87-case corpus:

- factory held-envelope render for STAT/PAR/DYN;
- each of the eight SYN controls at raw `0`, `64`, and `127` for each machine;
- note 36, 60, and 84 for each machine;
- factory render through the normal envelope path.

The report stores control inputs, SHA-256 fingerprints and signal measurements,
but **not** the OS, DSP memory, PCM, or WAV assets. It was captured locally
with a local OS file and verified by a complete second deterministic capture.

| artefact | SHA-256 |
| --- | --- |
| `oracle/fmplus/fmplus_oracle_cases.json` | `368681fb2d297721341b921d4196571204ff76bc2b198d7b763283658ce2819f` |
| `oracle/fmplus/fmplus_oracle_baseline_1.32B.json` | `145e0e14cae6fb0ea5d187d1f6a83e6b9d09f5785a81eb330b207815ea39c3d7` |

Run `oracle/fmplus/verify_monomodule_oracle.py` with an independently local
renderer and OS file to reproduce all 87 checks.

## Mandatory design rule for the native implementation

The next code stage must create a truly new appended mode, not reinterpret an
existing one. IDs 0–5—including serialized `new fix=4` and prior `old fix=5`—
remain immutable. The public name and appended ID for the native oracle-backed
mode must be selected before implementation.

That new mode must own all of the following:

1. A new ID and migration/state version.
2. A private knob/parameter bank and serialized raw values. Mode switches must
   not read, borrow, overwrite, or restore values owned by another mode.
3. A private table/namespace for every raw mapping, display list, default, and
   factory profile. It cannot include/alias `FmFixTables.hpp` or any MNM/NEW
   table.
4. Private DSP cores/state for STAT, PAR, and DYN, including lifecycle, note,
   pitch, reset, and sample processing.
5. Static isolation tests plus audio comparison tests against this oracle
   corpus.

The current earlier `old fix` candidate remains a historical baseline only: it
cannot be presented as the final solution under this stricter no-shared-knobs,
no-shared-tables rule.

## Firmware boundary verification

`oracle/fmplus/verify_no_firmware_payload.py` scans either a source directory
or a tar/tar.gz archive. It fails on `.syx`, `.srec`, `.s19`, `.hex`, or the
known OS filename. The oracle capture was built outside the delivery tree and
the test OS lives only in a temporary local location.

## Deliberate next decision

Before native code changes, choose the public label for the newly appended
oracle-backed mode (for example, `old native` or `old hw`) and confirm that it
will be appended after raw ID 5. That avoids silently changing the meaning of
any previously serialized mode.

## Implementation update — `oracle=6`

The public label was selected as `oracle`; it is now implemented append-only at
raw ID `6` with schema `32` in both product trees. The source has private
Oracle APVTS banks, table/readout/factory namespace, STAT/PAR/DYN core objects,
UI binding, migration, and a firmware-free standalone 87-case comparison tool.
Existing shared SYN matrix/P-LOCK/direct-LFO targets are explicitly suppressed
while Oracle is active rather than silently modulating Oracle through the
retained m8/m9/m10 target namespace.

This update is **not** a declaration of acoustic completion. The initial native
metrics-only comparison completed all 87 cases but passed `0/87` at the
published tolerances. See `ORACLE_MODE_IMPLEMENTATION_STATUS_2026-09-28.md`
and `oracle/fmplus/oracle_native_comparison_2026-09-28.json`; both record the
open calibration work without retaining firmware or audio payloads.
