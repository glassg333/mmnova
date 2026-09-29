> **SUPERSEDED / NOT AN ACCEPTED ROUTE:** This 1.9.11 document describes the
> rejected automatic `mnmFilter` carrier policy. It remains only as a historical
> record for the already-presented archive. Do **not** use it as the current
> architecture or reissue that archive. See the independent 1.9.12 correction:
> [`RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md).

# Monomachine Nova 1.9.11 source package

This archive is a **source-only** repair release for both Monomachine Nova
Synth and Monomachine Nova FX. It supersedes the 1.9.10 source package without
modifying that earlier package.

## Repair scope

- A non-`NATIVE` `MODE L` or `MODE H` selection now activates the
  hybrid-capable filter path even when **DSP FILT remains at its normal `old`
  default**. No manual `old` → `mnm` switch is required to hear an explicitly
  selected R/K35/Moog filter.
- `old` with both `MODE L` and `MODE H` at `NATIVE` remains on the retained
  legacy filter path. Selecting DSP FILT `mnm` still explicitly selects the
  reconstructed path, as before.
- The independent `R HUV` equivalent was replaced with a bounded,
  pass-band-normalized TPT-style four-pole ladder. This removes the former
  large open-cutoff gain that could clip and sound broken.
- `R DVAL` no longer reverses its integration sign at the normal open cutoff;
  that defect could rail the core and become quiet/garbled after the outer
  protection stages.
- `MODE L/H` menu presentation now has one compact **`R CLASSIC`** branch:
  `R 303`, `R MS20`, `R MOOG`, `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`, and
  `R LADDERS` all live together. The old separate `R IMPORT 2` top-level
  branch is removed.

## Compatibility

- All 52 serialized MODE L/H choice values remain unchanged: retained IDs
  `0..8`, R-classic IDs `9..20`, and R Import 2 IDs `21..51`.
- The state schema remains **25**. No parameter layout, automation ID, or
  saved-choice migration is needed for this repair.
- Physical side semantics remain deliberate: MODE L / BASE is the lower edge
  (HP/low-cut choices first); MODE H / BASE+WDTH is the upper edge (LP/high-cut
  choices first). A deliberately selected LP on the lower edge at a very low
  BASE, or HP on the upper edge at a very high BASE+WDTH, can still attenuate
  audio by design; it is not a routing failure.
- The retained `mnm|old|new` DLY modes, DBAS/DWID feedback filtering, optional
  feedback Q, MODE S, and all previous R-family entries remain available.

## Validation boundary

The package includes standalone C++17 regression sources and a static
integrity verifier. It does not claim a VST3/DAW build in this workspace,
because a usable JUCE module checkout and DAW host are not included.

Run from the archive root:

```sh
python3 verify_dsp_mode_patch.py
```

Then compile the standalone tests in each product's `tests/` directory,
especially `HybridDspTests.cpp`, `Import2FiltersTests.cpp`, and
`MnmRealFilterTests.cpp`.

See [`RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md`](RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md)
and [`VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md`](VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md)
for the implementation and validation record.
