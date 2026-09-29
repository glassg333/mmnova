> **SUPERSEDED / NOT AN ACCEPTED ROUTE:** This 1.9.11 document describes the
> rejected automatic `mnmFilter` carrier policy. It remains only as a historical
> record for the already-presented archive. Do **not** use it as the current
> architecture or reissue that archive. See the independent 1.9.12 correction:
> [`RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md).

# Monomachine Nova 1.9.11 — default-OLD filter-route repair

Source-only repair release for **Monomachine Nova Synth** and **Monomachine
Nova FX**.

## User-visible fixes

### Selected filters now work at the normal DSP FILT default

`DSP FILT = old` is still the normal default. Before 1.9.11, `MODE L/H` choices
were only stored in `mnmFilter`, while the `old` renderer exclusively processed
the legacy `MonomachineFilter`. As a result, an R/K35/Moog selection could be
visible in the menu but do nothing until the user manually switched DSP FILT to
`mnm`.

1.9.11 makes a non-`NATIVE` MODE L or MODE H choice an explicit route request:

- `old` + `MODE L=NATIVE` + `MODE H=NATIVE` → retained legacy filter;
- `mnm` + any MODE L/H values → retained reconstructed filter path;
- `old` + either MODE L/H non-NATIVE → hybrid-capable reconstructed path for
  the selected filter.

The routing decision is a small standalone-tested helper. It does not change
DSP FILT saved IDs or the saved MODE L/H values.

### Quiet / broken imported-filter rendering corrected

The reported bad `mnm` result included two objective defects in the independent
R Import 2 equivalents:

- **R HUV** could produce a very large open-pass-band gain. Its prior
  approximation has been replaced by a bounded, normalized nonlinear TPT
  four-pole ladder equivalent.
- **R DVAL** multiplied its integration coefficient by `(1 - x)`, which turns
  negative above `fs/pi`. The normal fully open Monomachine cutoff reaches this
  range. The coefficient now keeps a finite damping denominator without the
  sign reversal.

New regression coverage measures an open-cutoff 1 kHz pass-band level for every
single-output R ladder family and requires a sane audible range, in addition to
finite/stereo/control-ramp tests.

This does not make every filter mode unity at every control position. MODE L is
the physical lower BASE edge and MODE H is the physical upper BASE+WDTH edge:
a low-pass selected on a low lower-edge cutoff, or high-pass selected on a high
upper-edge cutoff, intentionally attenuates the signal.

### One R CLASSIC menu branch

The MODE L/H popup no longer splits `R CLASSIC` and `R IMPORT 2` into separate
top-level groups. `R CLASSIC` now contains all R families:

- `R 303`, `R MS20`, `R MOOG`;
- `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`;
- `R LADDERS` containing `R HUV`, `R HYPER`, `R KRAJ`, `R MICRO`, `R MUSIC`,
  `R OBER`, and `R DVAL`.

The popup remains compact and cascade-based, so it stays accessible from the
lower FILTER row. This is only a UI grouping change; item IDs and physical
MODE L/H ordering are unchanged.

## Compatibility and scope

- MODE L/H IDs remain `0..51`; no value is inserted, reordered, or repurposed.
- State schema stays **25**; older-state handling is unchanged.
- Both Synth and FX receive byte-identical shared filter, menu, helper, and
  standalone-test changes.
- This package retains 1.9.8 DLY `mnm|old|new`, DBAS/DWID/Q, 1.9.9 R-classic,
  and 1.9.10 independent R Import 2 source-boundary decisions.
- R Import 2 remains a set of self-contained independent equivalents, not a
  claim of a bit-identical incorporation of `JUCE/for import/2`.

## Version metadata

Both products are marked **1.9.11** in CMake, `.jucer`, generated JUCE
metadata, source build markers, and `Check-Build.ps1`. The generated version
code is `0x1090B`.
