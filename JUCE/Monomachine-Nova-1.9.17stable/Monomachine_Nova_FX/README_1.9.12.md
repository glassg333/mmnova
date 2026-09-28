> **Superseded in this working tree by 1.9.13:** the original 1.9.12 archive remains unchanged. See [`README_1.9.13.md`](README_1.9.13.md).

# Monomachine Nova FX 1.9.12

Source-only independent physical-filter route correction.

## FILTER behaviour

`DSP FILT` is again limited to its two native alternatives while both `MODE L`
and `MODE H` are `NATIVE`:

- `old` runs the retained OLD native filter;
- `mnm` runs the retained MNM native filter.

When either MODE side selects K35, MOOG/ladder, R CLASSIC, or R Import 2, this
product renders the selected family through a separate
`IndependentPhysicalFilterCore`. The OLD renderer and MNM native renderer are
both excluded from that call, so there is no OLD+selected or MNM+selected
serial path. The selected side replaces its own native physical stage once;
the other side follows its own explicit MODE choice.

## Compatibility

Schema remains 25. MODE L/H IDs `0..51`, their BASE/WDTH physical direction,
the unified R CLASSIC popup, R HUV/R DVAL repairs, and DLY
`mnm|old|new`/DBAS/DWID/Q work are retained without parameter remapping.

See [`../HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md`](../HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md),
[`../RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](../RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md),
and [`../VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](../VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md).
