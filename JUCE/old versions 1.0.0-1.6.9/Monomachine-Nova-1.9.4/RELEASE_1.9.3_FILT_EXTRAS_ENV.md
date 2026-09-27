# Monomachine Nova 1.9.3 — FILT extras, FIL ENV and MOD ENV

Date: 2026-09-26

## Scope

This source-only revision adds opt-in filter controls without changing the
existing MNM or OLD references at their defaults. P1 and P2 remain independent.
The manual track order is unchanged:

```text
EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY
```

## One explicit filter SAT stage

`SAT` is intentionally **not** wired into each MODE L/MODE H algorithm.
Regardless of whether either or both physical sides use K35 or ladder choices,
the audio path is:

```text
FILT lower / BASE side → FILT upper / WIDTH side → SAT once → DIST / MODE S once
```

- `SAT = 0` is an exact bypass; no newly-added saturation is run and MNM/OLD
  retain their former output.
- `SAT > 0` runs exactly one stereo post-FILT Korg/Odin-style dry-to-tanh
  saturation stage.
- Its response is the extracted Odin/Korg overdrive law: a dry/wet blend into
  `tanh(3*x)` at partial drive and `tanh(6*x)` at the maximum control value.
- The imported K35/ladder classes' own overdrive controls remain at their
  neutral zero setting; `SAT` does not drive them individually.

`MODE S` remains a distinct, one-pass post-FILT **DIST** selector. It selects
MNM, OLD, MNM FIX, FOLD, ZERO or CLAMP for the existing track DIST block. It is
not the new filter-saturation control, and `SAT` does not cause MODE S to run a
second time.

For imported MODE S values only, the DIST control now follows this explicit
A/B rule:

```text
DIST 0..64   retained native MNM attenuation / unity exactly
DIST 65..127 retained MNM input pre-drive → direct imported FOLD/ZERO/CLAMP
```

There is no common post-level compensation and no blend back into MNM above
64. Those two shared operations changed Fold, Zero and Clamp by different
amounts; MNM, OLD and MNM FIX are unchanged.

## New FILT controls

Right-clicking the P1 or P2 `FILT` category opens that page's extra panel:

- `VEL-L`, `VEL-H`
- `KT-L`, `KT-H`
- `SAT`

The VEL/KT controls feed the lower and upper physical cutoff sides. Their zero
values are neutral for MNM, OLD, K35 and ladder routes.

## Separate FIL ENV

The existing FILT page controls `ATK`, `DEC`, `BOFS` and `WOFS` remain intact.
A separate additive `FIL ENV` page provides `ATK`, `HOLD`, `DEC`, `REL`,
`ENV FIL`, `BASE`, and `WIDTH`:

- `ENV FIL = 0` is sonically neutral.
- The supplemental envelope routes only through the existing `BASE/WIDTH`
  topology.
- It does not replace, reinterpret or hybridise the original filter envelope.

## Four MOD ENV sources

The AMP overlay exposes `AMP ENV | FIL ENV | MOD ENV`. MOD ENV has four
independent ADHR pages. They append stable matrix source IDs:

```text
MOD ENV1..4 = IDs 32..35
AUX encoding = IDs 33..36 (source + one; 0 is OFF)
```

They are triggered/released with the processor note gate and may route to P1 or
P2 targets through the existing matrix.

## Saved state

Schema 20 introduces the FILT-extra and MOD ENV parameters. A schema-19 state
that lacks them receives neutral defaults:

```text
VEL/KT/SAT = 0
FIL ENV MIX/BASE/WIDTH = 0
FIL ENV ATK/HOLD = 0; DEC/REL = 127
MOD ENV ATK/HOLD = 0; DEC/REL = 127
```

No old state is remapped to a different MNM/OLD or MODE S selection.

## Validation

Both Synth and FX source projects passed:

- `ModeRollbackTests`
- `HybridDspTests`
- `FilterExtrasDspTests` — native MNM, OLD and every imported K35/ladder side,
  including exact neutral bridges and the one-stage Korg/Odin SAT law
- `ModEnvMatrixTests` — source IDs, AUX encoding, clamp and reset neutrality
- syntax checks for `PluginProcessor.cpp`, `PluginEditor.cpp`, and
  `RollbackStateTests.cpp`
- generated Release object compilation for `PluginProcessor.cpp` and
  `PluginEditor.cpp`
- `verify_dsp_mode_patch.py`, including product-identity guards for Synth and FX

Only pre-existing JUCE Font constructor deprecation warnings were emitted.
This archive is source-only: it does not claim a completed full VST3 link or a
host-render test in this Linux environment.
