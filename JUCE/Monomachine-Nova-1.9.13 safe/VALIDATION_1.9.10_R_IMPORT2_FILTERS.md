# Validation record — Monomachine Nova 1.9.10 R Import 2 filters

Validation date: **2026-09-27**.

This is a **source-only** validation record for both `Monomachine_Nova_Synth`
and `Monomachine_Nova_FX`. The requested `JUCE/for import/2` material was used
as an audit/topology reference only. Its files were not copied, compiled, or
claimed to be bit-identical; the delivered `RImport2Filters.hpp` is the
self-contained independent-equivalent implementation described in
[`R_IMPORT2_AUDIT_2026-09-27.md`](R_IMPORT2_AUDIT_2026-09-27.md).

## Executed static release check

From the package root, the following was run after the final source, test,
metadata, and UI review:

```sh
python3 verify_dsp_mode_patch.py
```

Result: **PASS** for both products. The root check covers, among the retained
contracts:

- release version `1.9.10`, version code `0x1090A`, and both product identities;
- append-only hybrid IDs: retained `0..8`, R-classic `9..20`, and R Import 2
  `21..51`;
- schema `25` and the `<24`, schema-24, and schema-25 migration boundaries;
- 52-item physical MODE L/MODE H maps and response metadata;
- lower-side HP/low-cut and upper-side LP/high-cut semantics;
- the nested compact `R CLASSIC` / `R IMPORT 2` UI menu path;
- allocation-free Import 2 adapter state, control-ramp, and sample-rate
  continuity guards; and
- registration of the standalone `Import2FiltersTests` CMake target along with
  the retained state, delay, hybrid, real-filter, FILT-extra, MOD ENV, and ARP
  regressions.

## Executed standalone C++17 suite

For **each** product, these sources were compiled with
`g++ -std=c++17 -O2 -Wall -Wextra -pedantic -I Source` and executed:

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

The R Import 2-specific coverage includes:

- all `52 × 52` independently selectable MODE L/MODE H combinations through
  the physical `FilterCore` path;
- exact 52-item lower/upper mapping arrays, plus explicit physical-response
  metadata assertions for each newly appended selection `21..51`;
- measured LP/BP/HP response checks for the multimode Analog, Linear, RBJ,
  TPT, and Hyperion-style families;
- hostile-control finite/bounded-output stress for every appended mode at
  22.05, 44.1, 48, and 96 kHz;
- independent left/right state checks, impulse rendering, and common ±4 output
  safety bounds;
- bit-exact equivalence under repeated unchanged host snapshots;
- 16-sample cutoff/control-ramp boundary checks;
- real sample-rate transition and unchanged-rate no-reset checks; and
- retained delay `mnm|old|new` compatibility and schema-25 rollback coverage.

## Executed sanitizer coverage

For both Synth and FX, the R Import 2-specific tests below were built and run
with AddressSanitizer and UndefinedBehaviorSanitizer:

```sh
g++ -std=c++17 -O1 -g -fno-omit-frame-pointer \
    -fsanitize=address,undefined -I Source tests/Import2FiltersTests.cpp ...
g++ -std=c++17 -O1 -g -fno-omit-frame-pointer \
    -fsanitize=address,undefined -I Source tests/HybridDspTests.cpp ...
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 ...
```

Result: **PASS** for all four executions; no AddressSanitizer or
UndefinedBehaviorSanitizer diagnostic was emitted.

## Manual source review completed

The final source review confirmed that both products expose the same 52 stable
hybrid choices. The MODE popup root is deliberately short (`K35`, `MOOG`,
`R CLASSIC`, `R IMPORT 2`); the Import 2 families cascade below it rather than
using an off-screen flat menu. Within each response-capable family, MODE L
labels actual HP/low-cut before BP/LP and MODE H labels actual LP/high-cut
before BP/HP. The UI item value remains the canonical serialized index.

Both product copies of the shared Import 2 implementation, bridge, data/schema
mapping, and corresponding tests were byte-compared where they are intended to
match. Product-specific Synth/FX metadata remains separate.

## Not claimed in this environment

A usable JUCE 8 source checkout and DAW host were not present in this
source-delivery workspace. Therefore this record does **not** claim:

- a full CMake/JUCE plug-in or VST3 build;
- execution of JUCE-linked integration/UI/state targets;
- Projucer regeneration;
- DAW automation, host save/load, editor rendering, or subjective audio
  audition.

The source package does retain matching CMake, `.jucer`, generated version
metadata, `Check-Build.ps1`, and standalone target registration. A binary
release should additionally run a clean JUCE build and DAW smoke test for P1
and P2, both physical filter sides, state loading from schemas below 24, schema
24, and schema 25, and lower-row menu placement.

## Package integrity

For the delivered source archive:

- both `SOURCE_BUILD.json` manifests were regenerated from the final product
  trees; each contains **153** non-backup source-package entries, including
  `README_1.9.10.md`, `RImport2Filters.hpp`, and `Import2FiltersTests.cpp`;
- every manifest SHA-256 entry was recalculated and verified against disk, with
  no missing, extra, or mismatched entry;
- the archive is created with the versioned root
  `Monomachine-Nova-1.9.10/`, then checked with `unzip -t` and extracted into a
  fresh directory;
- the fresh extraction is checked again with the root static verifier, both
  source manifests, and the focused `HybridDspTests` and
  `Import2FiltersTests` standalone tests for Synth and FX; and
- the matching SHA-256 sidecar is delivered as
  `Monomachine-Nova-1.9.10.sha256` beside the archive.
