# Monomachine Nova 1.9.14 source package

This archive is source-only and contains both maintained variants:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

Both trees include the same implementation changes for the 1.9.14 follow-up:
manual-law independent HPF/LPF filter key tracking, BASE/WDTH RMB access to the
compact filter-extra panel, and compact main-surface FIL ENV/MOD ENV access.
Each tree has its own `README_1.9.14.md` and a refreshed `SOURCE_BUILD.json`
manifest.

The package also retains the already-completed source corrections for MODE L/H
response grouping, the visible `R OBERHEIM LP4` name, and new matrix routes
appearing at the top.

## Building

No binaries, CMake build folders, JUCE checkout, object cache, or DAW artifacts
are included. To build, supply a JUCE 8 source checkout through `JUCE_DIR`, for
example:

```sh
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/path/to/JUCE
cmake --build build-synth --config Release
```

Use the analogous FX tree for the effects plug-in. The archive was delivered as
source because no full JUCE/VST3 or DAW runtime build was performed here.

See `RELEASE_1.9.14_KEYTRACKING_ENVELOPES.md` and
`VALIDATION_1.9.14_KEYTRACKING_ENVELOPES.md` for scope and source validation.
