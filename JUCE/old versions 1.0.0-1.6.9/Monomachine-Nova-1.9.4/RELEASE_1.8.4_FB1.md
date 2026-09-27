# Monomachine Nova — 1.8.4 FB1 source snapshot

This is a verified source snapshot of the ongoing 1.8.4 work, not a claim that
all future 1.8.4 work is complete.

## Included

- Both project trees: `Monomachine_Nova_Synth` and `Monomachine_Nova_FX`.
- Up to 32 native FX-slots across all 15 P1/P2 chain gaps.
- Vertical FX page, drag ordering, slot state persistence and native P2 FX list.
- Classic DSP stage bodies split into `Source/dsp/Track*.inl` without changing
  their arithmetic or canonical ordering.
- One configurable FB SEND/RETURN route:
  - exact one-sample feedback timing while enabled;
  - SEND / FB / RETURN manual gains;
  - 0.1–10 Hz DC high-pass control;
  - explicit hard clip at ±1 and no automatic limiter;
  - state stored as `FEEDBACK`.

## Verification

- Synth default PPHASH: `ec101520ac42bb83`
- FX default PPHASH: `f75309bcac42bb83`
- Native slot-machine comparison, slot state/latency, FB timing/state and FX UI
  smoke tests pass in the workspace headless harness.

## Intentionally deferred

- A reusable FILTER block for a future FX-slot option. This does **not** mean
  adding a second FILTER to P1 or P2; each future filter instance will retain
  selectable legacy (`mnm`/`old`) and additional modes.
- Multiple FB routes, cross-feedback/waveguide matrix.
- TAP/duplicated outputs and standalone CPU-optimisation work.

See `RELEASE_1.8.4_FB1.md` and the workspace design notes for the exact current
scope. Build artefacts are intentionally not included in this source snapshot.
