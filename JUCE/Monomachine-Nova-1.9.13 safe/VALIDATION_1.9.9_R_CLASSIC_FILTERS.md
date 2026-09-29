# Validation record — Monomachine Nova 1.9.9 R classic filters

Validation date: **2026-09-27**.

## Completed in this source-delivery environment

### Static release checks

Ran from the package root:

```text
python3 verify_dsp_mode_patch.py
```

Result: **PASS for both `Monomachine_Nova_Synth` and
`Monomachine_Nova_FX`**.

The checker confirms, among the existing retained DSP contracts:

- schema 24 and the appended `9..20` range;
- unchanged IDs `0..8` and side-specific physical mapping tables;
- registration of `R 303`, `R MS20`, and `R MOOG` labels;
- attributed self-contained TB303/MS20/Moog adaptation and bridge routing;
- no-op behavior for unchanged sample rates;
- grouped, screen-safe MODE L/H UI path;
- regression-source coverage for mapping, state migration, R response,
  stability, stereo state, control ramps, and host snapshot cadence;
- retained Synth/FX product identities and CMake version 1.9.9.

### Standalone C++17 DSP suite

For **each** product, the following were compiled with
`g++ -std=c++17 -O2 -Wall -Wextra -pedantic -I Source` and executed:

```text
MnmCoreTests
DelayFeedbackDspTests
MnmRealFilterTests
HybridDspTests
FilterExtrasDspTests
ModEnvMatrixTests
ArpWindowTests
ModeRollbackTests
```

Result: **8/8 PASS for Synth; 8/8 PASS for FX**.

Relevant new 1.9.9 coverage includes:

- all 21×21 selectable lower/upper combinations through `FilterCore`;
- exact legacy mapping plus appended R mapping;
- LP/BP/HP response-semantic checks for R 303, R MS20, R MOOG 12-pole and
  R MOOG 24-pole variants;
- high-control, multi-rate finite/bounded-output stress;
- independent stereo-state checks;
- 16-sample control-ramp boundary checks;
- unchanged 48 kHz setup bit-exact continuity for every imported mode;
- repeated same-mode host snapshot bit-exact continuity;
- finite 48 kHz → 44.1 kHz reprepare followed by repeated 44.1 kHz no-op
  checks;
- schema-23 malformed-value protection and schema-24 R-family state round
  trips.

### Sanitizer pass

The Synth copies of `HybridDspTests` and `MnmRealFilterTests` also passed with
AddressSanitizer and UndefinedBehaviorSanitizer:

```text
g++ -std=c++17 -O1 -g -fno-omit-frame-pointer \
    -fsanitize=address,undefined -I Source ...
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 ...
```

No sanitizer diagnostic was emitted.

## Not claimed

A complete JUCE module checkout/full plug-in build and DAW-host runtime session
were not available in this delivery environment. Therefore this record does
**not** claim VST3 build, Projucer regeneration, host automation UI rendering,
or subjective audio audition in a DAW. The source package retains the matching
CMake/.jucer/generated version metadata and a `Check-Build.ps1` hash/build
marker audit for those environments.

## Package integrity

Completed for the delivered source package:

- regenerated each product's `SOURCE_BUILD.json` and verified all 150 listed
  SHA-256 entries per product;
- created `Monomachine-Nova-1.9.9.zip` with a versioned archive root;
- ran `unzip -t`, then extracted into a fresh directory;
- re-ran the root static checker, manifest verification, `HybridDspTests`, and
  `MnmRealFilterTests` for both products from that fresh extraction;
- wrote the matching archive SHA-256 sidecar
  `Monomachine-Nova-1.9.9.sha256`.
