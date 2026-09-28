> **SUPERSEDED / NOT AN ACCEPTED ROUTE:** This product note documents the
> rejected 1.9.11 automatic MNM-carrier policy. The active correction is
> [`README_1.9.12.md`](README_1.9.12.md).

# Monomachine Nova Synth 1.9.11

Source-only filter-route repair release. It retains the 1.9.8 DLY
`mnm|old|new` feedback path, 1.9.9 R-classic choices, and 1.9.10 appended R
Import 2 IDs without moving any saved value.

## FILTER repair

`DSP FILT` may remain at its normal `old` default. Once either `MODE L` or
`MODE H` is non-`NATIVE`, Synth automatically renders through the
hybrid-capable filter core, so the selected R/K35/Moog response is audible.
With both selectors at `NATIVE`, `old` remains the retained legacy filter.

`R HUV` now uses a bounded normalized independent ladder equivalent and `R
DVAL` no longer reverses at open cutoff. These changes target the previously
quiet, clipped, or garbled imported-filter behavior while keeping the existing
physical BASE/WDTH law.

## Menu

All R choices now live in one `R CLASSIC` popup branch: `R 303`, `R MS20`, `R
MOOG`, `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`, and `R LADDERS`. The compact
cascade remains safe at the lower edge of the editor. Choice IDs stay exactly
as saved in 1.9.10.

## Saved state

The schema remains 25 and IDs `0..51` remain frozen. MODE L is the physical
lower / BASE edge; MODE H is the physical upper / BASE+WDTH edge. Intentional
mismatched responses can attenuate at extreme cutoff positions by design.

See [`../RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md`](../RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md)
and [`../VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md`](../VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md).
