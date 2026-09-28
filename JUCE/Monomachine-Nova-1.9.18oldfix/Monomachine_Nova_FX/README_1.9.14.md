# Monomachine Nova FX 1.9.14

Source-only follow-up: documented independent filter keyboard tracking and
compact envelope access. The same source changes are mirrored in the FX tree.

## Independent HPF / LPF key tracking

Two new P1 parameters and their P2 counterparts are binary switches:

- `filt_track_hpf` / `filt_track_lpf`
- `p2_filt_track_hpf` / `p2_filt_track_lpf`

They default to **ON**, matching normal Monomachine tracking. Each physical edge
can be disconnected independently.

With a switch on, the manual-derived target is:

```text
cutoff = played-note frequency / 4 * 2^(control / 8)
```

Thus HPF `BASE=0` begins two octaves below the played note and every `+8` BASE
units is an octave. LPF retains the existing Monomachine physical topology
`BASE + WDTH`: every `+8` BASE or `+8` WDTH units adds an octave to its edge.
The native MNM table core selects the closest retained table cutoff; the legacy
core calculates the same Hz law through its own coefficient path. Existing
VEL/KT amount parameters remain additive legacy modifiers; they are not these
new switches.

Schema 26 migrates states without these controls to both switches ON, preserving
the documented normal-tracking default.

## UI

- RMB on the FILT-page `BASE` or `WDTH` control opens `FILT / EXTRA` for the
  current P1/P2 faceplate.
- That compact panel contains visible `HPF KEYTRACK` and `LPF KEYTRACK`
  checkboxes. The older continuous `KT-L` / `KT-H` parameters remain serialized
  for old projects but are no longer presented as the normal tracking controls.
- RMB on the `FILT` category header no longer opens the extras panel.
- The main surface now reads `AMP`, `FIL ENV`, `MOD ENV`, then `MODE AMP`.
  Left-click `FIL ENV` or `MOD ENV` opens a compact duplicate that binds the
  same parameters as the retained full tab. RMB on its header button opens the
  retained full page; RMB on empty compact/full-page space switches the other
  direction, matching AMP-envelope navigation. The compact MOD ENV view keeps
  its selected source and exposes its `ROUTE` action.

## Validation performed on source

Standalone source tests passed for the real MNM filter (including the new
manual-law checks), existing filter-extra DSP behavior, and the production
`TrackFILT.inl` route integration. A legacy-core octave-equivalence smoke check
also passed. No JUCE plug-in build, DAW load, or interactive GUI test was run
for this source-only delivery.
