# Monomachine Nova 1.9.9 — full source package

This archive is a complete source package for both products:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

It is a new patch release after the separately preserved
`Monomachine-Nova-1.9.8` archive. Do not overwrite the older archive when
extracting this one.

## 1.9.9 contents

- appended hybrid physical-filter algorithms `9..20`:
  `R 303`, `R MS20`, and `R MOOG` LP/BP/HP forms;
- unchanged legacy hybrid IDs `0..8`, old project values, and 1.9.8 DLY `NEW`;
- schema-24 state migration that preserves valid historical values and never
  reinterprets malformed old values as an appended R algorithm;
- truthful lower/upper physical-side response ordering and compact, screen-safe
  MODE L/H popup groups;
- attributed self-contained source adaptation of the supplied incomplete
  TB303/MS20/Moog input package;
- regression coverage for mapping, response semantics, finite/stable output,
  stereo isolation, control ramp boundaries, unchanged sample-rate continuity,
  actual sample-rate changes, repeated mode snapshots, and schema migration.

Start with [`RELEASE_1.9.9_R_CLASSIC_FILTERS.md`](RELEASE_1.9.9_R_CLASSIC_FILTERS.md)
and [`VALIDATION_1.9.9_R_CLASSIC_FILTERS.md`](VALIDATION_1.9.9_R_CLASSIC_FILTERS.md).

## Build metadata

Both product projects use version `1.9.9` in:

- CMake and `.jucer` project metadata;
- generated JUCE project/version declarations;
- `Source/NovaConfig.h` and the visible editor build marker;
- `Check-Build.ps1` and per-product `SOURCE_BUILD.json` hash manifests.

The archive root is named `Monomachine-Nova-1.9.9`; the established product
directory names remain unchanged.
