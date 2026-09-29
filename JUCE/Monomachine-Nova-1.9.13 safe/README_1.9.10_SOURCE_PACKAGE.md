# Monomachine Nova 1.9.10 source package

This archive is a **source-only** patch package for Monomachine Nova Synth and
Monomachine Nova FX. It is versioned 1.9.10 and supersedes the delivered
1.9.9 package without changing that archive.

## Included patch scope

- retained 1.9.8 DLY `mnm|old|new`, DBAS/DWID feedback filters, and optional Q;
- retained 1.9.9 R-classic (`R 303`, `R MS20`, `R MOOG`) hybrid choices;
- new append-only `R IMPORT 2` hybrid choices at canonical IDs `21..51`;
- schema-25 migration preserving historical 0..20 state behaviour;
- compact, lower-edge-safe MODE L/H cascading menus;
- standalone C++17 regression sources and static package verifier;
- independent, allocation-free R Import 2 equivalents with no JUCE dependency.

## Important compatibility rule

A state predating schema 25 cannot legitimately contain IDs 21..51. The loader
therefore keeps schema-24 values only through 20 and normalizes malformed later
values to `NATIVE`. Schema-25 states retain the complete valid 0..51 range.

## Provenance and license boundary

`JUCE/for import/2` was audited but is **not** copied into this package. The
folder depends on absent framework files and has mixed/incomplete source
provenance. The project decision is self-contained independent equivalents;
labels identify topology families, not direct ports. Read
[`R_IMPORT2_AUDIT_2026-09-27.md`](R_IMPORT2_AUDIT_2026-09-27.md) before
redistributing or claiming upstream equivalence.

## Build/validation boundary

The included standalone tests compile with a normal C++17 compiler and do not
require JUCE. Full VST3/DAW validation is not claimed here because this source
workspace does not include a usable JUCE module checkout or host runtime.

Run the static inventory verifier from the archive root:

```sh
python3 verify_dsp_mode_patch.py
```

Then compile the listed standalone targets in each product's `tests/` folder,
especially `HybridDspTests.cpp`, `Import2FiltersTests.cpp`, and
`MnmRealFilterTests.cpp`. The executed source-package and archive verification
record is [`VALIDATION_1.9.10_R_IMPORT2_FILTERS.md`](VALIDATION_1.9.10_R_IMPORT2_FILTERS.md).
