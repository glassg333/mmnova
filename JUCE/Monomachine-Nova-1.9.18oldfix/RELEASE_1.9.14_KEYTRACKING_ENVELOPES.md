# 1.9.14 — filter key tracking and compact envelope access

## Implemented in both Synth and FX

### Manual-derived independent tracking

Four new state parameters are present:

- P1: `filt_track_hpf`, `filt_track_lpf`
- P2: `p2_filt_track_hpf`, `p2_filt_track_lpf`

They are binary, independently switchable HPF/LPF key-tracking controls and
default to ON. The target law is applied inside each filter core rather than by
forcing one raw-knob calibration onto both implementations:

```text
edge cutoff = played-note Hz / 4 × 2^(control / 8)
```

So HPF `BASE=0` is two octaves below the played note. `BASE +8` raises HPF by
one octave. LPF preserves the established physical `BASE + WDTH` topology, so
`+8` BASE or `+8` WDTH raises its edge one octave. HPF and LPF can be disabled
independently. Existing continuous `KT-L` / `KT-H` values remain legacy
additive modifiers, not replacements for these controls.

State schema is now 26. States that predate these parameters migrate both
switches to ON.

### Filter-extra interaction

- RMB on either visible FILT `BASE` or `WDTH` opens the compact `FILT / EXTRA`
  panel for the active P1/P2 faceplate.
- `HPF KEYTRACK` and `LPF KEYTRACK` are checkbox controls in that panel.
- RMB on the `FILT` category header no longer opens the panel.

### Envelope surface access

The main AMP header row now contains `AMP`, `FIL ENV`, `MOD ENV`, and `MODE`.

- Left-click `FIL ENV` or `MOD ENV` opens a compact view that binds the exact
  existing envelope parameters.
- RMB on the corresponding surface button opens the retained full tabbed page.
- RMB on empty compact or full envelope page space navigates between the two
  presentations, modeled after AMP.
- The compact MOD ENV view retains its source selection and includes its
  `ROUTE` action.

## Retained 1.9.14 source fixes

This package also includes the earlier source-only completed changes: stable
MODE L/H choice indices with response-led low-cut/high-pass and
high-cut/low-pass first folders, the visible `R OBERHEIM LP4` singleton label,
and promoted new patch-cord/matrix routes at GUI row zero.
