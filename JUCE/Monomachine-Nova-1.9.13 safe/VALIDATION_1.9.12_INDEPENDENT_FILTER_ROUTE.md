> **Superseded in this working tree by 1.9.13:** the original 1.9.12 archive remains unchanged; this note is retained as its historical record. See [`README_1.9.13_SOURCE_PACKAGE.md`](README_1.9.13_SOURCE_PACKAGE.md).

# Validation record — Monomachine Nova 1.9.12 independent filter route

Validation date: **2026-09-27**. This is a source-only validation record for
both Synth and FX. No usable JUCE module checkout or DAW host is present here,
so no VST3 build, DAW load, automation, or subjective listening claim is made.

## Historical prerequisite completed

The cloud 1.9.8 and 1.9.9 sources were inspected before code changes. Their
byte-identical `TrackFILT.inl` files gated MODE L/H through `mnmFilter`; OLD
ignored a selected family. `MnmRealFilter` replaced a selected **physical
side**, rather than applying the native stage plus K35/R on that same side.
The detailed source trace is in
[`HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md`](HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md).

## Executed route regression

For each product, `tests/FilterRouteTests.cpp` and
`tests/TrackFilterRouteIntegrationTests.cpp` were compiled with:

```sh
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -I Source tests/<target>.cpp
```

and executed successfully.

`FilterRouteTests` calls the same `dispatchFilterRenderRoute()` helper used
by `TrackFILT.inl`; `TrackFilterRouteIntegrationTests` includes the production
`TrackFILT.inl` unchanged with counted renderer stand-ins. Together they confirm:

- `old` + `NATIVE`/`NATIVE` calls only OLD once;
- `mnm` + `NATIVE`/`NATIVE` calls only native MNM once;
- selected K35, R 303, R HUV, and R DVAL on either side calls the independent
  renderer exactly once for either FILT DSP value;
- OLD and MNM-native callback counts are zero in every selected-family case;
- an opposite NATIVE side is represented only by its explicit physical-side
  plan; and
- repeated selected-family snapshots at the same sample rate are bit-identical,
  so no callback-rate reset/dropout is introduced.

## Executed static and standalone validation

`python3 verify_dsp_mode_patch.py` passed for both products after the corrected
route, metadata, and CMake registration were in place.

The following **11 JUCE-free standalone C++17 targets per product** were built
with `-O2 -Wall -Wextra -pedantic` and passed:

```text
MnmCoreTests
DelayFeedbackDspTests
MnmRealFilterTests
HybridDspTests
FilterRouteTests
TrackFilterRouteIntegrationTests
Import2FiltersTests
FilterExtrasDspTests
ModEnvMatrixTests
ArpWindowTests
ModeRollbackTests
```

`HybridDspTests` retained its full mode-map, 52×52 physical-side,
finite/stereo, response, control-ramp, and R-family checks. The two new route
tests add the outer-renderer exclusivity proof and compilation of the actual
`TrackFILT.inl` body.

`LfoMappingTests` was not built in this workspace because it includes
`NovaData.h`, which requires an unavailable JUCE module checkout. This is not
represented as a successful JUCE or DAW build.

## Executed focused sanitizers

For **each** product, these four targets were independently built and run with
`-fsanitize=address,undefined`, `ASAN_OPTIONS=detect_leaks=1:halt_on_error=1`,
and `UBSAN_OPTIONS=halt_on_error=1`:

```text
FilterRouteTests
TrackFilterRouteIntegrationTests
HybridDspTests
Import2FiltersTests
```

All **4/4 Synth** and **4/4 FX** sanitizer executions passed with no sanitizer
diagnostic.

## Final package integrity

A clean source archive was built with a single `Monomachine-Nova-1.9.12/` root:

- **383 ZIP entries**; no `.bak*` or editor-backup file is included;
- both per-product `SOURCE_BUILD.json` manifests contain **157** non-backup
  files and every SHA-256 was verified after fresh extraction;
- `unzip -t` passed; and
- the fresh extraction passed the root static verifier plus `FilterRouteTests`,
  `TrackFilterRouteIntegrationTests`, `HybridDspTests`, and
  `Import2FiltersTests` for both Synth and FX.

The SHA-256 checksum is supplied beside the archive in the portable
`Monomachine-Nova-1.9.12.zip.sha256` sidecar rather than embedded in this
self-referential source document.

This document intentionally does not represent the source/standalone checks as
DAW validation.
