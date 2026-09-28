# Monomachine Nova Synth 1.9.10

Source-only patch release. This Synth package retains 1.9.8's DLY `mnm|old|new`
feedback path and 1.9.9's R-classic filters, then appends R Import 2 hybrid
families without moving existing IDs.

## Hybrid filter additions

`MODE L` / `MODE H` now contain compact `R IMPORT 2` groups for:

- `R ANALOG`, `R LINEAR` (LP/BP/HP at 12/24 poles);
- `R RBJ`, `R TPT` (LP/BP/HP);
- `R HUV`, `R HYPER`, `R KRAJ`, `R MICRO`, `R MUSIC`, `R OBER`, `R DVAL`.

MODE L keeps physical lower-side HP/low-cut ordering; MODE H keeps upper-side
LP/high-cut ordering. The nested groups keep the menu reachable at the lower
edge of the plug-in window.

## Saved state

Hybrid IDs `0..20` are frozen. IDs `21..51` are new and schema 25 records the
append. Pre-25 state never reinterprets malformed later values as a new sound.

## Source boundary

The requested `for import/2` collection is handled via independent,
self-contained equivalents because its framework dependencies and licensing
provenance are incomplete. See [`../R_IMPORT2_AUDIT_2026-09-27.md`](../R_IMPORT2_AUDIT_2026-09-27.md)
and [`../RELEASE_1.9.10_R_IMPORT2_FILTERS.md`](../RELEASE_1.9.10_R_IMPORT2_FILTERS.md).
