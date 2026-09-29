# Monomachine Nova 1.9.10 — R Import 2 filters

Source-only patch release for both **Monomachine Nova Synth** and
**Monomachine Nova FX**.

## What changed

1.9.10 adds append-only `R IMPORT 2` hybrid filter families from the requested
`JUCE/for import/2` source set:

- `R ANALOG` — LP/BP/HP at 12 and 24 poles;
- `R LINEAR` — LP/BP/HP at 12 and 24 poles;
- `R RBJ` — LP/BP/HP;
- `R TPT` — LP/BP/HP;
- `R HUV`, `R HYPER`, `R KRAJ`, `R MICRO`, `R MUSIC`, `R OBER`, and `R DVAL`
  ladder-family responses.

The modes are offered through `MODE L`/`MODE H` in nested compact groups. The
physical filter law is retained: MODE L / BASE is the lower edge and lists
HP/low-cut forms before BP/LP; MODE H / BASE+WDTH is the upper edge and lists
LP/high-cut before BP/HP.

## Compatibility

- `0..8`: unchanged 1.9.8 hybrid IDs.
- `9..20`: unchanged 1.9.9 `R 303`, `R MS20`, `R MOOG` IDs.
- `21..51`: 1.9.10 appended R Import 2 IDs.
- DSP state schema: **25**.
- Schema-24 files retain valid `0..20` selections exactly and cannot accidentally
  reinterpret a malformed later value as an R Import 2 sound.
- Schema-25 files preserve every valid `0..51` selection independently for P1
  and P2, MODE L and MODE H.
- The 1.9.8 delay `NEW` mode, `mnm` and `old`, DBAS/DWID feedback filtering,
  optional feedback Q, MODE S, and all earlier parameters remain intact.

## Implementation boundary and provenance

The supplied directory is framework-incomplete (`Filter.h`, `OnePole.h`,
`TptFilter.h`, JUCE helpers) and has mixed/incomplete licensing provenance.
At the requester's selected policy, this release uses self-contained,
independent equivalents rather than copying or compiling the supplied files.
The selected families are topology references, not bit-identical ports.

See [`R_IMPORT2_AUDIT_2026-09-27.md`](R_IMPORT2_AUDIT_2026-09-27.md) for the
source audit, dependency boundary, visible inventory, and license/provenance
record.

## Real-time behaviour

The adapter is allocation-free, uses one stateful instance per stereo channel,
finite guards, a 16-sample control ramp, genuine-mode-change resets only, and
no reprepare when a host repeats an unchanged sample rate. The retained
physical filter output safety path remains active.

## Validation

The source package includes standalone tests for R Import 2 physical response,
finite/stereo safety, repeated snapshot continuity, control ramps, actual
sample-rate changes, all hybrid pair dispatches, and schema migrations. Full
JUCE/plugin/DAW runtime verification requires a separate environment with the
JUCE modules and a host.

See [`VALIDATION_1.9.10_R_IMPORT2_FILTERS.md`](VALIDATION_1.9.10_R_IMPORT2_FILTERS.md)
for the final executed validation log.
