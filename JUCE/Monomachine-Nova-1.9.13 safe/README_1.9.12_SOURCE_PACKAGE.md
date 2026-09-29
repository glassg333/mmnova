> **Superseded in this working tree by 1.9.13:** the original 1.9.12 archive remains unchanged; this note is retained as its historical record. See [`README_1.9.13_SOURCE_PACKAGE.md`](README_1.9.13_SOURCE_PACKAGE.md).

# Monomachine Nova 1.9.12 source package

This is a **source-only** correction package for Monomachine Nova Synth and
Monomachine Nova FX. It is a new package, separate from the presented
`Monomachine-Nova-1.9.11.zip`; the old archive is not overwritten.

## What this package fixes

A non-`NATIVE` MODE L/H choice is now an **independent physical-filter
selection**, not an instruction to run the MNM renderer. Therefore:

| DSP FILT | MODE L/H | Active renderer |
|---|---|---|
| `old` | both `NATIVE` | retained OLD native filter |
| `mnm` | both `NATIVE` | retained MNM native filter |
| `old` or `mnm` | either side non-`NATIVE` | dedicated independent physical filter |

The selected K35/Moog/ladder/R side executes once. Neither OLD nor the MNM
native renderer is executed in that route. The remaining MODE side retains its
historic explicit selection; an opposite `NATIVE` is only that separately
selected physical side, not an OLD/MNM outer serial stage.

## Retained work

- append-only MODE L/H values `0..51`, schema 25, and side-specific BASE/WDTH
  semantics;
- R HUV bounded normalized core and R DVAL open-cutoff repair;
- one compact, reachable `R CLASSIC` menu branch for all R families;
- DLY `mnm|old|new`, DBAS/DWID feedback filters, and optional right-click Q;
- Synth/FX mirror layout and binary identity metadata.

## Evidence and validation boundary

The historical 1.9.8/1.9.9 source trace is recorded in
[`HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md`](HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md).
Both products include `FilterRouteTests.cpp` and
`TrackFilterRouteIntegrationTests.cpp`, direct standalone regressions for K35
and R family route exclusivity; the latter compiles the production
`TrackFILT.inl` body itself.

Run from the archive root:

```sh
python3 verify_dsp_mode_patch.py
```

Then build the standalone targets in each product, especially
`FilterRouteTests`, `HybridDspTests`, and `Import2FiltersTests`.

No VST3/DAW host build or subjective audition is claimed in this workspace;
see [`VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md).
