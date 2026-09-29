> **Superseded in this working tree by 1.9.13:** the original 1.9.12 archive remains unchanged; this note is retained as its historical record. See [`README_1.9.13_SOURCE_PACKAGE.md`](README_1.9.13_SOURCE_PACKAGE.md).

# Monomachine Nova 1.9.12 — independent physical-filter route

This is a new **source-only** patch release. It does not overwrite or reissue
the presented 1.9.11 archive. The prior 1.9.11 auto-hybrid route is retained
only as a superseded historical record; it is not the accepted topology.

## Plain-language behaviour

- If **both** `MODE L` and `MODE H` are `NATIVE`, `DSP FILT` works normally:
  select `old` for the retained OLD filter or `mnm` for the retained MNM
  native filter.
- If either MODE control is K35, MOOG/ladder, `R 303`, `R MS20`, `R MOOG`, or
  an appended R Import 2 family, that selected physical filter is rendered by
  its **own independent path**. It works with the ordinary `DSP FILT=old`
  default; no manual switch to `mnm` is needed.
- In that independent path, the OLD renderer is not run and the MNM native
  renderer is not run. Therefore there is no `old + K35/R` or `mnm + K35/R`
  serial/double-filter route.

## Historical physical-side law retained

Cloud source 1.9.8 and 1.9.9 was audited before this correction; see
[`HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md`](HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md).
MODE L remains the lower/BASE physical side and MODE H remains the
upper/BASE+WDTH physical side. A non-NATIVE choice replaces the native stage
on its own side exactly once.

If the other MODE control remains `NATIVE`, that is the separately selected
historic physical-side companion only. It is not an OLD/MNM outer renderer,
and it is transparent at the normal open physical edge. Both sides can also be
selected deliberately, in which case their two explicitly selected physical
responses run in lower-then-upper order as before.

## Implementation boundary

- `FilterRenderRoute` has exactly three choices: `LegacyOld`, `NativeMnm`, and
  `IndependentPhysical`.
- `TrackFILT.inl` calls the single shared `dispatchFilterRenderRoute()` helper.
- `IndependentPhysicalFilterCore` has separate state from `mnmFilter`; it is
  prepared, cleared, triggered, released, and sample-rate guarded separately.
- Repeated unchanged MODE snapshots and unchanged sample rates do not reset its
  state. The existing 16-sample adapter control ramps remain intact.
- This release keeps the 1.9.10/1.9.11 R HUV and R DVAL repairs, unified
  screen-safe `R CLASSIC` menu, schema 25, choice IDs `0..51`, physical menu
  mappings, and 1.9.8 delay `NEW`/DBAS/DWID/Q work.

## Direct regression

`tests/FilterRouteTests.cpp` and `tests/TrackFilterRouteIntegrationTests.cpp`
exist in both products and are registered in CMake. The first tests the shared
route planner plus actual independent K35/R cores; the second includes the
production `TrackFILT.inl` body with counted renderer stand-ins. Together they
test K35, R 303, R HUV, and R DVAL with both FILT DSP values. For each selected
side they assert:

```text
OLD calls = 0
MNM-native calls = 0
independent selected-family calls = 1
```

It also checks the historic explicitly-NATIVE opposite-side rule and repeated
selected-family snapshot continuity. See
[`VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md).

## Compatibility

No parameter ID, MODE L/H value, schema number, menu choice order, or physical
BASE/WDTH role is changed. Existing projects retain their saved values; only
the renderer chosen for a non-NATIVE family is corrected so it no longer
implicitly becomes the MNM native renderer.
