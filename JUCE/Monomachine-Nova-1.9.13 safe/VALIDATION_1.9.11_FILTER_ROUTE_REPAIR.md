> **SUPERSEDED / NOT AN ACCEPTED ROUTE:** This 1.9.11 document describes the
> rejected automatic `mnmFilter` carrier policy. It remains only as a historical
> record for the already-presented archive. Do **not** use it as the current
> architecture or reissue that archive. See the independent 1.9.12 correction:
> [`RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md).

# Validation record — Monomachine Nova 1.9.11 filter-route repair

Validation date: **2026-09-27**.

This is a **source-only** validation record for both `Monomachine_Nova_Synth`
and `Monomachine_Nova_FX`. No usable JUCE module checkout or DAW host is
present in this workspace, so a VST3 build and host audition are not claimed.

## Executed static release check

From the package root:

```sh
python3 verify_dsp_mode_patch.py
```

Result: **PASS** for both products.

The static check confirms, among the retained contracts:

- DSP FILT still defaults to `old`, with `old` + `NATIVE`/`NATIVE` retaining
  the legacy filter path;
- the standalone route policy makes any non-`NATIVE` MODE L/H choice use the
  hybrid-capable core without a user DSP FILT switch;
- all 52 serialized choice IDs, physical lower/upper maps, and schema 25 stay
  intact;
- R HUV uses the normalized bounded ladder path and R DVAL has no high-cutoff
  integration-sign reversal;
- both editor copies have one compact `R CLASSIC` branch containing all R
  families and `R LADDERS`, with no separate `R IMPORT 2` root branch;
- product metadata is consistently 1.9.11 / `0x1090B`; and
- Synth/FX retain their respective identities and matching standalone
  regression registration.

## Executed standalone C++17 suite

For **each** product, the following targets were built with:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -I Source tests/<target>.cpp
```

and then executed:

```text
MnmCoreTests
DelayFeedbackDspTests
MnmRealFilterTests
HybridDspTests
Import2FiltersTests
FilterExtrasDspTests
ModEnvMatrixTests
ArpWindowTests
ModeRollbackTests
```

Result: **9/9 PASS for Synth; 9/9 PASS for FX**.

The added/strengthened filter coverage includes:

- OLD/NATIVE versus OLD/non-NATIVE route-policy assertions for every selectable
  non-native MODE L/H value;
- all 52 × 52 FilterCore side combinations, exact choice-map checks, finite
  output, physical responses, stereo isolation, 16-sample control ramps, and
  unchanged-rate state continuity;
- explicit full-open, 1 kHz pass-band level checks for R HUV, R HYPER,
  R KRAJ, R MICRO, R MUSIC, R OBER, and R DVAL; and
- retained neutral/opt-in old/MNM filter-extra and DLY feedback regressions.

## Executed sanitizer coverage

The focused filter targets were independently built and run for both products
with AddressSanitizer and UndefinedBehaviorSanitizer:

```sh
g++ -std=c++17 -O1 -g -fno-omit-frame-pointer \
  -fsanitize=address,undefined -I Source tests/Import2FiltersTests.cpp
g++ -std=c++17 -O1 -g -fno-omit-frame-pointer \
  -fsanitize=address,undefined -I Source tests/HybridDspTests.cpp
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 ...
```

Result: **PASS** for all four executions. No AddressSanitizer or
UndefinedBehaviorSanitizer diagnostic was emitted.

## Metadata and package-source integrity

- Both `.jucer` files parsed as XML.
- Both `SOURCE_BUILD.json` files parsed as JSON and were regenerated from the
  final product trees.
- Each manifest contains **154** non-backup source-package entries; every
  listed SHA-256 matched disk, with no missing or extra entry.
- The released archive is checked with `unzip -t`, then freshly extracted and
  rechecked with the root static verifier, both product manifests, and the
  focused standalone tests. Its SHA-256 is supplied in the sidecar file next
  to the archive.

## Not claimed

This source validation does **not** claim a Projucer regeneration, full JUCE
CMake build, Windows/MSVC build, VST3 load, DAW automation/save-load, menu
rendering, or subjective audition. A binary release should run a clean build
and a DAW smoke test on P1 and P2, native and non-native MODE L/H selections,
full-open R HUV/R DVAL, and menu placement at the lower FILTER row.
