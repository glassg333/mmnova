# Closed hybrid DSP evaluation

Both Synth and FX contain a closed hybrid evaluation path with three selectors
above FILT on each P1/P2 page:

- **MODE L** — lower `BASE` side, physically ordered HP/low-cut → BP → LP/high-cut.
- **MODE H** — upper `BASE+WDTH` side, physically ordered LP/high-cut → BP → HP/low-cut.
- **MODE S** — the only selector for the existing post-filter DIST block:
  `MNM`, `OLD`, `MNM FIX`, `FOLD`, `ZERO`, `CLAMP`.

MNM FIX is a separate, experimental +3 dB / Q=1.0 candidate at 3.5 kHz,
blended only above DIST=64. It is deliberately not described as an original
firmware handler. MNM and OLD remain untouched references.

Imported K35/ladder cutoff and resonance targets ramp over 16 samples to
reduce control-boundary clicks. This change applies only to imported sides;
native and OLD filter paths are unchanged. High-Q settling is still measured
rather than hidden by a limiter or broad filter darkening.

Manual P1/P2 block order, physical FX-slot points, `BASE`/`WDTH`, the native
P2 machine list, and `MachineEngine` remain intact.

See `RELEASE_1.9.2_MNM_FIX_AND_IMPORTED_DECLICK.md` and
`HYBRID_FILTER_AUDIT_2026-09-26.md` for exact measurements.
