# Build guide — 1.9.0 native DSP rollback

This archive intentionally does **not** bundle a JUCE checkout or generated build
artifacts. Supply a JUCE 8 checkout outside the source tree.

## CMake

Run separately for `Monomachine_Nova_Synth` and `Monomachine_Nova_FX`:

```sh
cmake -S Monomachine_Nova_Synth -B build-synth \
  -DJUCE_DIR=/path/to/JUCE \
  -DNOVA_BUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-synth --config Release
ctest --test-dir build-synth --output-on-failure -C Release
```

Substitute `Monomachine_Nova_FX` / `build-fx` for the second product. The CMake
files retain `/utf-8` for MSVC, so Russian source/UI strings use the correct
source and execution charset.

## Windows/MSVC

Each product contains `Check-Build.ps1` and its `.jucer` project. Keep the
projects separate and point their JUCE path at an external JUCE 8 checkout.
Do not copy a JUCE checkout into the release source tree.

## Release checks

Before packaging, run:

```sh
python3 verify_dsp_mode_patch.py
```

from the archive root. It dispatches the native-rollback static check in both
products. `ModeRollbackTests` and `RollbackStateTests` provide regression
coverage for selector removal and serialized-state migration.

See [`VALIDATION_1.9.0_NATIVE_ROLLBACK.md`](VALIDATION_1.9.0_NATIVE_ROLLBACK.md)
for the validation performed for this source archive and the sandbox-specific
full-build limitation.
