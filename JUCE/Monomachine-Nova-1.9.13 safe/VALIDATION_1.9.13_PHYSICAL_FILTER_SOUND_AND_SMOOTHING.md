# Validation record — Monomachine Nova 1.9.13 physical-filter sound/control correction

Validation date: **2026-09-27**. This is a source-only record for both Synth
and FX. No usable JUCE module checkout or DAW host is available here, so it
makes no VST3 build, DAW automation, listening, or subjective sound claim.

## Static integrity

The root command below passed for both products after the sound/control change,
metadata update, and regression updates:

```sh
python3 verify_dsp_mode_patch.py
```

It verifies the 1.9.13 metadata, independent OLD/MNM/physical route, separate
physical lifecycle, explicit no-generic-MNM-clip entry point, sample-rate-aware
10 ms control ramp, and the new/strengthened route regressions.

## Executed strict standalone regressions

For **each** product, all 11 available JUCE-free targets were built with:

```sh
c++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -I Source tests/<target>.cpp
```

and then run successfully:

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

Important direct results:

- `FilterRouteTests` drives a selected K35 physical core above the prior
  native soft-clip knee, proves the independent output stays finite and follows
  no generic MNM clip law, and retains native carrier safety only for the
  native core under test.
- `TrackFilterRouteIntegrationTests` includes the production `TrackFILT.inl`
  unchanged and confirms a selected K35/R invocation calls neither OLD nor
  native MNM.
- `HybridDspTests` verifies 10 ms control-ramp counts of **441**, **480**, and
  **960** samples at 44.1/48/96 kHz; repeated K35/ladder target snapshots are
  bit-identical to an uninterrupted ramp; and its high-Q cutoff-step scenario
  reported maximum per-sample deltas of K35 LP **0.701813**, K35 HP
  **0.972394**, ladder HP24 **0.128453**, and ladder HP12 **0.256409**.
- Native `MnmRealFilterTests`, schema rollback, R Import 2, and filter-extra
  regressions still passed unchanged.

## Executed focused sanitizers

For **each** product, these five targets were independently built/run with
`-fsanitize=address,undefined`,
`ASAN_OPTIONS=detect_leaks=1:halt_on_error=1`, and
`UBSAN_OPTIONS=halt_on_error=1`:

```text
MnmRealFilterTests
HybridDspTests
FilterRouteTests
TrackFilterRouteIntegrationTests
Import2FiltersTests
```

All **5/5 Synth** and **5/5 FX** sanitizer runs passed with no sanitizer
diagnostic.

## Boundary

`LfoMappingTests` was attempted for both products but was not built because it
includes `NovaData.h`, which needs an unavailable JUCE-generated `JuceHeader.h`.
This source-only environment also cannot validate a VST3/DAW host or judge
audible character.

## Final package integrity

The final clean source archive contains one `Monomachine-Nova-1.9.13/` root,
**364 files** / **388 ZIP entries**, and no `.bak*` or editor-backup file.
Each product `SOURCE_BUILD.json` records **158** non-backup files. The completed
release check fresh-extracts the archive, verifies every manifest hash, runs
`unzip -t`, runs the root static verifier, and reruns the focused route/physical
filter tests for Synth and FX. Its SHA-256 is supplied beside the archive in a
standard `Monomachine-Nova-1.9.13.zip.sha256` sidecar rather than embedded in
this self-referential source document.
