# Monomachine Nova 1.9.16 source package

This is a **source-only** package containing both maintained variants:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

## Included change

This package closes the requested MODE L/H extension-list omission without changing the native Monomachine filter:

- Existing MODE L/H IDs `0..51` remain unchanged.
- Schema `28` appends six derived HP complements at IDs `52..57`:
  `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`,
  `R OBERHEIM HP4`, and `R DVAL HP4`.
- Each new response is derived from its previously LP-only core. It is an extension feature, **not** a claim that the original Monomachine contains these imported filters.
- The DVAL complement is polarity-aware (`dry + LP`) because that recovered LP core has inverted output polarity; the other derived complements use `dry - LP`.
- MODE L/H popup grouping derives from actual response metadata, so the appended modes are shown as HP/low-cut responses rather than merely renamed.
- Synth and FX contain matching source and tests.

The native FILT/Q/BOFS behaviour reported in listening tests is **not claimed fixed by 1.9.16**. `FILTER_SOURCE_EVIDENCE_2026-09-28.md` is included as the bounded source-forensics record for that separate repair.

## Validation performed

For each variant, JUCE-free C++17 builds and runs passed for:

- `Import2FiltersTests`
- `HybridDspTests`
- `FilterRouteTests`

The checks cover response labels, appended-ID stability, finite/stereo behaviour, route exclusivity, and unchanged snapshot continuity. No complete JUCE/VST3/DAW build or listening validation was performed in this environment.

## Build

A JUCE 8 checkout is required to build either plug-in. No JUCE checkout, binary, CMake output, cache, or DAW artefact is included.

```sh
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/path/to/JUCE
cmake --build build-synth --config Release
```

Use the analogous `Monomachine_Nova_FX` command for the FX variant.
