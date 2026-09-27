# DSP evidence audit — current decision

Date: **2026-09-26**.

This short release-level note is superseded in detail by
[`HYBRID_FILTER_AUDIT_2026-09-26.md`](HYBRID_FILTER_AUDIT_2026-09-26.md).

## Decision retained in 1.9.1

- The manual P1/P2 audio block order stays `EQ → FILT → DIST → ENV → VOL/PAN →
  SRR → DELAY`.
- P2 retains its native `MachineEngine` and original FX-slot machine list.
- No universal “original” DIST curve, Q makeup gain, filter darkening, or
  imported-filter smoothing has been asserted from incomplete reconstruction
  evidence.
- Current MODE S `MNM` and `OLD` remain distinct references; imported FOLD,
  ZERO, and CLAMP are integration characters using a MNM-compatible control
  progression.
- A generic `MNM FIX` is intentionally deferred because the recovered evidence
  proves a per-machine handler slot and drive envelope, while the sample-level
  handler transfer remains unresolved.

The new direct high-Q transition test records material discontinuities in some
imported paths. It is a measurement baseline, not authorization for an
unverified smoothing algorithm.
