# Private hybrid DSP test

This directory contains the active private DSP test module used by Nova's
separate `MODE L`, `MODE H`, and `MODE S` controls.

## Test controls

| Control | Physical scope | Choices |
|---|---|---|
| `MODE L` | lower / `BASE` filter edge | `NATIVE`, `K35 LP`, `K35 HP`, `MOOG LP24`, `MOOG LP12`, `MOOG BP24`, `MOOG BP12`, `MOOG HP24`, `MOOG HP12` |
| `MODE H` | upper / `BASE+WDTH` filter edge | `NATIVE`, `K35 LP`, `K35 HP`, `MOOG LP24`, `MOOG LP12`, `MOOG BP24`, `MOOG BP12`, `MOOG HP24`, `MOOG HP12` |
| `MODE S` | existing post-filter `DIST` block | `NATIVE`, `FOLD`, `ZERO`, `CLAMP` |

P1 and P2 store independent values. The three visible controls always attach
to the currently selected P1/P2 view. All defaults are `NATIVE`.

The private choices do not add or move a stage: L/H replace only the selected
physical filter side, and S changes only the algorithm inside the existing
post-filter DIST stage. `BASE`, `WDTH`, filter-envelope offsets, manual
routing, the native P2 machine list, and the retained `old` reference remain
separate. The hybrid choices run only with the underlying `mnm` path, so `old`
continues to be an untouched A/B reference.

## Test coverage

`tests/HybridDspTests.cpp` checks native equivalence for L/H defaults, all 9×9
L/H combinations through the actual FilterCore bridge, independent stereo
state for K35 and every ladder response, and finite Fold/Zero/Clamp rendering. It is exposed as `HybridDspTests` when
`NOVA_BUILD_TESTS=ON` and also compiles standalone.

The `upstream/` subdirectory is preserved as an unmodified source-reference
bundle. A future stereo-safe reverb experiment remains separate from this
filter and saturation test.
