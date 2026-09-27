# Validation record — Monomachine Nova 1.9.8 delay work

Date: 2026-09-27

## Completed in this workspace

The following JUCE-free compiler/test commands were run with `c++ -std=c++17 -Wall -Wextra -pedantic -I Source` for **both** `Monomachine_Nova_Synth` and `Monomachine_Nova_FX`:

- `tests/MnmCoreTests.cpp` — passed (`MNM core checks: all OK`).
- `tests/DelayFeedbackDspTests.cpp` — passed all 11 checks (`DELAY FEEDBACK DSP PASS`): neutral MNM bypass, DBAS and DWID spectral effect, MNM/OLD audibility, redundant update continuity, high-Q NEW stability, state isolation, frame scheduling, and selector/schema compatibility.
- `tests/MnmRealFilterTests.cpp` — passed.
- `tests/HybridDspTests.cpp` — passed Korg/ladders/MNM FIX/Fold/Zero/Clamp checks.
- `tests/FilterExtrasDspTests.cpp` — passed neutral and opt-in MNM/OLD/imported paths.
- `tests/ModEnvMatrixTests.cpp` — passed.
- `tests/ArpWindowTests.cpp` — passed 1…64 window scheduling checks.
- `tests/ModeRollbackTests.cpp` — passed (`MODE_ROLLBACK PASS: DLY mnm|old|new with stable legacy indices`).

The root static verifier was run after mirroring the work to both products:

```sh
python3 verify_dsp_mode_patch.py
```

It passed for Synth and FX. It verifies DLY-only `NEW`, schema 23, P1/P2 Q registration/migration, NEW dispatch, feedback-filter/new-core source presence, Q-panel wiring, regression target registration, and preservation of product-specific metadata.

## Validation not performed here

The workspace did not include a usable JUCE module checkout or full configured plugin CMake build. Therefore the following are **not** represented as completed here:

- compiling the complete JUCE plugin projects;
- executing JUCE-linked state/UI tests such as `RollbackStateTests`;
- runtime validation of the editor right-click panel; and
- loading either VST3 in a DAW.

Run a clean JUCE/CMake build and DAW smoke test before a binary release. In particular, verify P1 and P2 independently, all three DLY modes, saved-state migration from schema 22 and schema 23, and right-click access to DBAS/DWID Q.
