# Monomachine Nova 1.9.2 — hybrid side/DIST source package

Date: 2026-09-26

This package contains both source projects:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

It retains manual P1/P2 block order, every physical FX-slot point,
`MachineEngine`, the native P2 machine list, `BASE`/`WDTH`, and independent
MNM/OLD references.

## MODE L / MODE H

The two filter-side selectors remain separate per P1/P2 page and follow
physical side order:

- **MODE L / BASE**: `NATIVE`, HP/low-cut, BP, LP/high-cut.
- **MODE H / WDTH**: `NATIVE`, LP/high-cut, BP, HP/low-cut.

Internal canonical FilterCore IDs remain unchanged. K35 and ladder responses
now ramp their imported cutoff/resonance targets over 16 samples when a control
changes. Native and OLD paths are not changed.

## MODE S

MODE S is the one audible selector for the existing post-filter DIST block:

```text
MNM | OLD | MNM FIX | FOLD | ZERO | CLAMP
```

- `MNM` and `OLD` are unchanged references.
- `MNM FIX` is a clearly marked experimental +3 dB/Q=1.0 correction near
  3.5 kHz, blended in only above DIST=64. It is a comparative answer to the
  reported notch, not an original-firmware claim.
- FOLD/ZERO/CLAMP retain the MNM-compatible DIST knob progression and output
  behaviour introduced in 1.9.1.

Schema 19 inserts MNM FIX without changing a schema-18 project's existing
MNM/OLD/FOLD/ZERO/CLAMP sound selection. Earlier OLD precedence is retained.

## Classic blocks

Every P1/P2 classic block (`EQ`, `FILT`, `DIST`, `ENV`, `VOL/PAN`, `SRR`, and
`DELAY`) has an independent persisted ON/OFF bypass. Disabling a block skips
only that call; order and physical FX-slot points remain unchanged.

## Measurements and validation

`HYBRID_FILTER_AUDIT_2026-09-26.md` records the original reconstruction limits,
the 3.5 kHz candidate definition, and direct high-Q transition measurements.
The 16-sample ramp sharply reduces the immediate control-boundary jump, while
later high-Q resonant settling remains measured rather than hidden.

For both source projects:

- standalone `HybridDspTests` passed;
- standalone `ModeRollbackTests` passed;
- `verify_dsp_mode_patch.py` passed;
- processor/editor and modified test sources passed JUCE 8.0.4 syntax-only
  checks (only existing Font deprecation warnings appeared).

The exact isolated ladder references remain under each product's
`Source/dsp/hybrid_private/upstream/` directory:

- `LadderFilter.h`: `b8cf6709e5bcc56b47ff8e846467332b11d335c143473996db701aa725321053`
- `LadderFilter.cpp`: `0d4aeba4fe0d5a0642414182c873df0beec1c3148c812bd294dd6708ab5c0dfc`

## Delivery status

This is a **source-only package**, not a built plugin binary. Full JUCE plugin
linking/host rendering is not claimed in this environment. No generated build
artefacts are included.
