# Monomachine Nova 1.9.1 — MODE S authority and classic-block bypass

Date: 2026-09-26

## Scope

This revision keeps the existing manual P1/P2 block order and every physical
FX-slot point. It does not replace `MachineEngine`, the native P2 machine list,
`BASE`/`WDTH`, the retained `mnm` filter core, or the isolated `old` reference.

## MODE S is now the only audible DIST selector

Each P1/P2 page has its own MODE S selector above FILT:

| Value | Selection | Behaviour |
|---:|---|---|
| 0 | `MNM` | Retained current `MnmKernel::Saturator` result. |
| 1 | `OLD` | Untouched `bipolarDist` reference. |
| 2 | `FOLD` | Imported Fold character with MNM-compatible knob progression. |
| 3 | `ZERO` | Imported Zero character with MNM-compatible knob progression. |
| 4 | `CLAMP` | Imported Clamp character with MNM-compatible knob progression. |

The former DSP-menu/face-panel `mode_dist` choice is now migration-only state;
it is no longer a competing user-facing controller. DIST's MODE button on the
FX-slots page opens the same page's MODE S choices.

For `FOLD`, `ZERO`, and `CLAMP`:

- DIST values through 64 return the retained MNM result exactly, including its
  lower-half attenuation and neutral point.
- Above 64, the imported character receives the retained MNM positive-drive
  terms (`1 + 15 × amount` before the character and `1 / (1 + 2 × amount)`
  afterward), then blends continuously in from the retained MNM result.
- This removes the previous raw imported-threshold onset at DIST=64 without
  claiming that the local transfer is original hardware law.

`MNM FIX` is deliberately **not** exposed in this revision. The recovered
firmware evidence verifies a per-machine DIST slot and its 16-sample drive
envelope, but the sample-level body of the located SWAVE-ENS handler remains
unresolved. A generic audio-shaping “fix” would therefore be fabricated. The
current MNM and OLD references remain separately selectable for comparison.

## MODE L / MODE H physical ordering

The host values are now deliberately side-specific; they are translated to the
unchanged canonical IDs before `FilterCore` receives them.

| MODE L / BASE (lower side) | MODE H / WDTH (upper side) |
|---|---|
| `NATIVE` | `NATIVE` |
| `K35 HP` | `K35 LP` |
| `MOOG HP24` | `MOOG LP24` |
| `MOOG HP12` | `MOOG LP12` |
| `MOOG BP24` | `MOOG BP24` |
| `MOOG BP12` | `MOOG BP12` |
| `K35 LP` | `K35 HP` |
| `MOOG LP24` | `MOOG HP24` |
| `MOOG LP12` | `MOOG HP12` |

Thus MODE L begins with low-cut/high-pass choices and ends with high-cut/
low-pass choices; MODE H does the inverse. No side itself was moved.

## Saved-state migration (schema 18)

A schema-17 state is converted once:

- Existing MODE L/H canonical IDs map into the new physical-side host order.
- A saved legacy `mode_dist=old` always becomes explicit MODE S `OLD`: OLD
  already won over every private selection at render time, so this preserves
  its actual audible reference behaviour.
- Otherwise legacy `MODE S=NATIVE` becomes `MNM`, and legacy `FOLD`, `ZERO`,
  and `CLAMP` retain their character as MODE S values 2, 3, and 4.
- Missing legacy hybrid controls safely resolve to MNM / native side choices.

Regression coverage checks these mappings for P1 and P2.

## Independent classic-block bypass

Every one of the seven P1/P2 classic blocks now has its own `ON` / `OFF` button
on the FX-slots page:

`EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY`

The switch is serialized by stable block ID, not by its draggable position.
When OFF, only that block's audio call is skipped. The surrounding physical
FX-slot points still execute at their existing positions. User-controlled block
order is preserved. Older states without these flags load with every block ON;
resetting a P1 or P2 order also turns that page's blocks ON.

## Validation performed

For both Synth and FX source projects:

- `HybridDspTests` compiled and passed, including a defined maximum-DIST
  imported-character headroom probe below unity.
- `ModeRollbackTests` compiled and passed.
- `verify_dsp_mode_patch.py` passed.
- `PluginProcessor.cpp`, `PluginEditor.cpp`, and the modified state/topology
  test sources passed JUCE 8.0.4 syntax-only compilation.
- The only editor diagnostics were pre-existing JUCE 8 Font-constructor
  deprecation warnings.

This is source validation. It is not a successful full plugin binary link or
host-render claim.
