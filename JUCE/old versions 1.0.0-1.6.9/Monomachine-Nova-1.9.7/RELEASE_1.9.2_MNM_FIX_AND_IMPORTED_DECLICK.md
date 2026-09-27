# Monomachine Nova 1.9.2 — experimental MNM FIX and imported-side de-click

Date: 2026-09-26

This revision responds to two separate audible reports while keeping existing
reference modes available for A/B comparison.

## MODE S now includes MNM FIX

| Value | Selection | Behaviour |
|---:|---|---|
| 0 | `MNM` | Unchanged retained local MNM DIST reference. |
| 1 | `OLD` | Unchanged untouched bipolar reference. |
| 2 | `MNM FIX` | Explicit experimental 3.5 kHz notch-compensation candidate. |
| 3 | `FOLD` | Imported character under MNM-compatible DIST control. |
| 4 | `ZERO` | Imported character under MNM-compatible DIST control. |
| 5 | `CLAMP` | Imported character under MNM-compatible DIST control. |

`MNM FIX` is intentionally separate from MNM and OLD. It applies a +3 dB,
Q=1.0 peaking correction centred at 3.5 kHz **after** the retained local MNM
DIST result. The correction blends from zero above DIST=64, so DIST values at
or below 64 are exactly the MNM result. This is an A/B testing candidate for
the reported level notch — it is **not** represented as a recovered firmware
handler.

The standalone test measures a 1.4061 amplitude ratio at 3.5 kHz and DIST=127
(the expected near-3 dB starting correction), and verifies exact neutral output
at DIST=64.

Schema 19 preserves projects saved by schema 18:

```text
schema 18: MNM | OLD | FOLD | ZERO | CLAMP
schema 19: MNM | OLD | MNM FIX | FOLD | ZERO | CLAMP
                  ^ inserted without changing existing sounds
```

Thus former FOLD/ZERO/CLAMP values 2/3/4 migrate to 3/4/5. Earlier schema
migration rules for legacy private selections and OLD precedence remain intact.

## Imported filter coefficient de-click

K35 and ladder side adapters now ramp cutoff and resonance targets over one
16-sample DSP frame. The change is restricted to imported MODE L/H responses;
the native FilterCore and OLD filter path are unchanged.

The direct high-Q step test moves only the chosen physical side and measures
the adjacent-sample jump exactly at the control boundary:

| Response | Before 16-sample ramp | After 16-sample ramp |
|---|---:|---:|
| lower K35 LP | 0.024643 | 0.001143 |
| lower K35 HP | 0.056228 | 0.001366 |
| upper ladder HP24 | 1.539230 | 0.305369 |
| upper ladder HP12 | 4.147610 | 0.180665 |

This fixes the immediate coefficient discontinuity that produces the control
boundary click. The test also prints the later maximum high-Q excursion. That
later resonant settling is intentionally retained as a separate measurement:
the 16-sample ramp does not claim to eliminate every high-Q ring/fart, and no
hidden limiter or global filter darkening was added.

## Validation

For both Synth and FX source projects:

- `HybridDspTests` compiled and passed.
- `ModeRollbackTests` compiled and passed.
- State/topology tests passed syntax-only compilation with JUCE 8.0.4.
- `PluginProcessor.cpp` and `PluginEditor.cpp` passed syntax-only compilation.
- `verify_dsp_mode_patch.py` passed.

Only pre-existing JUCE Font constructor deprecation warnings appeared in editor
syntax checks. This remains a source-only delivery; no full plugin link or host
render is claimed.
