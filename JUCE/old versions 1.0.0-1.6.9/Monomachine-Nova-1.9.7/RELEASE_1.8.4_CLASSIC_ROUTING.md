# Monomachine Nova 1.8.4 — historical Classic Routing source snapshot

> **Superseded routing note (2026-09-26):** this document records the 1.8.4
> snapshot only. Its six-block order was later found to conflict with the
> Monomachine manual and reconstruction audit. Do **not** use it as a statement
> of the current or canonical signal path. See `DIST_NEW_MODE.md` for the
> corrected documented topology and validation status.

This is a source-only snapshot for the completed classic-routing increment. It contains both targets:

- `Monomachine_Nova_Synth`
- `Monomachine_Nova_FX`

## Historical 1.8.4 routing — not current

The archived snapshot used a six-block permutation:

`DIST → SRR → FILT → EQ → ENV → DSND`

That was the 1.8.4 reset order, not a verified Monomachine topology. It has
been superseded by the seven-block documented order. The remaining text in
this document describes historical 1.8.4 behaviour only.

Physical g-points remain fixed. FX-slots are attached to their physical g-point and never follow a moved classic block.

## P2 dry/wet boundary

The dry copy is taken after the FX-slots at `g7`. The P2 machine and the classic P2 block group form the wet path. `P2 MIX` crossfades that dry copy against the completed wet group; `g9` is after the mix. Thus moving ENV or DSND inside P2 never inserts it into the dry copy.

The FX page shows this dry side branch and exposes a real `P2 MIX` control next to it. Individual native FX slots retain their existing own parameters; this snapshot does not invent or substitute DSP algorithms.

## Verification performed

- Synth and FX source files for the processor and editor are mirrored.
- Full headless JUCE compile/link and regression suites passed for both targets.
- Canonical PPHASH values remained unchanged:
  - Synth: `ec101520ac42bb83`
  - FX: `f75309bcac42bb83`
- Routing regression covered independent P1/P2 order, ENV at the front, DSND before ENV, fixed FX-slot locations, state round-trip, malformed/old state fallback, and P2 dry/wet isolation.
- UI smoke covered all 15 physical `+ ADD SLOT` controls, 12 MODE controls, both order-reset controls, and the live `P2 MIX` control.

This archive intentionally contains no compiled artifacts and no JUCE source checkout. Use JUCE 8 externally, for example via `batches/setup_juce.sh` in the working environment.
