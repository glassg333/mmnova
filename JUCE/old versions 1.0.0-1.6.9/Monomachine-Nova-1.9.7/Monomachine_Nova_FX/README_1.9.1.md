# Monomachine Nova FX 1.9.1

See the package-level release notes at:

- `../RELEASE_1.9.1_MODE_S_AND_BLOCK_BYPASS.md`
- `../HYBRID_FILTER_AUDIT_2026-09-26.md`

FX-specific scope is identical to the paired Synth source project for MODE
L/H/S, classic-block bypass persistence, and schema-18 migration.

Key user-facing points:

- MODE S is the one audible DIST selection: `MNM`, `OLD`, `FOLD`, `ZERO`,
  `CLAMP`.
- MODE L and MODE H are ordered by lower/upper physical filter-side semantics.
- Every P1/P2 classic block has independent ON/OFF bypass without moving any
  physical FX-slot point.
- `MNM FIX` is deliberately deferred until the unresolved per-machine firmware
  handler transfer is bounded by evidence.

This project is source-only. The revision passed standalone and syntax checks;
it was not fully linked into a plugin binary in this environment.
