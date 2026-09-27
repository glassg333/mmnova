# Monomachine Nova 1.9.8 — full source package

This archive is a **source-only** paired release. It contains the complete project directories:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

No VST3 or other plugin binary is included.

Start with [`RELEASE_1.9.8_TRACK_DELAY_NEW.md`](RELEASE_1.9.8_TRACK_DELAY_NEW.md). It documents the repaired delay feedback controls, the appended `NEW` mode, DBAS/DWID/Q behavior, mode/state compatibility, provenance, and the validation boundary.

## Build provenance and audit files

Each product directory retains its own:

- CMake project and Projucer (`.jucer`) metadata at version `1.9.8`;
- generated JUCE version defines/header and visible editor marker (`BUILD 1.9.8 / Synth` or `BUILD 1.9.8 / FX`);
- `SOURCE_BUILD.json` SHA-256 manifest;
- `Check-Build.ps1` read-only manifest and optional binary-marker checker; and
- `verify_dsp_mode_patch.py` static integrity verifier.

The root `verify_dsp_mode_patch.py` runs the product verifier for both directories. In a source checkout, run:

```sh
python3 verify_dsp_mode_patch.py
```

The delay-specific standalone tests can be compiled without JUCE from either product directory:

```sh
c++ -std=c++17 -Wall -Wextra -pedantic -I Source \
  tests/DelayFeedbackDspTests.cpp -o DelayFeedbackDspTests
./DelayFeedbackDspTests
```

Use your local JUCE/Projucer or CMake setup to generate and build a plugin. The archive intentionally preserves product-specific Synth/FX identity rather than copying one product’s build file over the other.

The release archive root is named `Monomachine-Nova-1.9.8`; the established underscore-containing product directory names are intentionally preserved.
