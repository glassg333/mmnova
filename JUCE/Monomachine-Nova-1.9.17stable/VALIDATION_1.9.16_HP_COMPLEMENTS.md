# 1.9.16 validation — appended R HP complements

## Scope

This validation covers only the appended MODE L/H extension modes for the six R Import 2 cores that previously exposed LP output only. It does not claim a correction of the native Monomachine FILT/Q/BOFS implementation.

## Contract checked

- IDs `0..51` retain their prior MODE L/H mapping.
- IDs `52..57` append, without reordering, `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`, `R OBERHEIM HP4`, and `R DVAL HP4`.
- Schema `<28` rejects out-of-era `52..57` values rather than reinterpreting them; schema `28` permits them.
- HP output is the explicitly derived complement of the LP-only core. DVAL uses `dry + LP` because its LP output polarity is inverted; the remaining cores use `dry - LP`.
- Both Synth and FX source trees contain byte-identical implementation/test changes in the relevant files.

## Performed checks

The following JUCE-free C++17 targets were compiled and run separately in both source trees:

1. `Import2FiltersTests`
2. `HybridDspTests`
3. `FilterRouteTests`

All passed. Tests include observable low/high response checks for the six appended HP entries, selector/map stability, finite/stereo tests, repeat-snapshot continuity, and independent physical-route exclusivity.

## Not performed

- Full JUCE project/editor build.
- VST3/AU/DAW load.
- Subjective listening comparison.
- Any claim that imported extension models equal Monomachine firmware.
- Any resolution of the separate native FILT/Q/BOFS issue.
