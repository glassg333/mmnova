# Monomachine Nova 1.9.15 source package

This is a **source-only** package containing both maintained variants:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

Both trees carry the same 1.9.15 DSP, state-schema, UI-choice, test, and
source-audit changes. They identify themselves as version `1.9.15` in CMake,
Projucer metadata, generated JUCE metadata, UI build markers, and per-tree
source manifests.

## Scope

- P2 native HPF/LPF keytracking remains independently switchable, while
  unchanged filter snapshots and unchanged MODE L/H selections no longer
  rebuild coefficients for every one-sample P2 render slice.
- The hidden generic native-filter output soft clip was removed. The DC blocker
  remains; deliberate `FILT SAT` and post-filter `TRACK DIST` remain separate.
- Filter-choice divider drawing is repaired.
- MODE S retains all existing selections and appends comparison-only choices:
  `MNM+OLD` (ID 6), `MNM V2` (ID 7), and `OLD V2` (ID 8).
- Schema 27 protects states written before those appended IDs and preserves
  independent P1/P2 candidate selections in schema-27 states.

`MNM` and `OLD` remain listening references. The three new MODE S choices are
comparison candidates, not claims of firmware identity. See
`RELEASE_1.9.15_FILTER_CPU_DIST.md` for the exact intent and listening matrix.

## Build status

No binaries, JUCE checkout, object cache, CMake build tree, VST3, or DAW
artifacts are included. A full plug-in/editor build and DAW test were not
performed in this environment because the required JUCE modules were absent.

To build on a machine with JUCE 8 available:

```sh
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/path/to/JUCE
cmake --build build-synth --config Release
```

Use the analogous FX tree for the effect plug-in.

See `VALIDATION_1.9.15_FILTER_CPU_DIST.md` for the source-level checks that
were performed before packaging.
