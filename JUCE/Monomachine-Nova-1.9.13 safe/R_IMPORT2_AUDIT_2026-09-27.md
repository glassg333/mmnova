# R Import 2 audit — 2026-09-27

## Scope

This audit covers the requested source set at:

- supplied repository location: `JUCE/for import/2`;
- pinned review commit: `a08334d586ac972c9ac932461ef3631924d31af9`;
- release result: Monomachine Nova **1.9.10**.

It is a new append-only patch after the released 1.9.9 R-classic work. The
1.9.9 ZIP and canonical IDs `0..20` are not modified or reinterpreted.

## Source and dependency findings

The folder is not a drop-in C++ component. `just filt.h` expects unavailable
`Globals.h`, `Utils.h`, JUCE lookup helpers, and a `Filter` base interface.
`Analog` also expects an absent `OnePole.h`; `TptWrapper` expects an absent
`../TptFilter.h`; the wrapper's constructor accepts an otherwise unspecified
model ID. Therefore direct inclusion would not build in Nova and could not
truthfully preserve the source framework's behaviour.

The audit also found mixed or incomplete licensing/provenance signals in the
supplied collection. In particular, the Analog/Linear/TPT-style grouping has
an apparent FILT-R/host-framework lineage, while the absent TPT dependency
has an external project lineage. Other headers contain varied source notices:
Unlicense/public-domain indications, commercial-use permission, unclear
MusicDSP attribution, or no explicit license statement. The supplied folder
itself carries no consolidated license or dependency lock.

### Project decision

The requester selected **independent equivalents**, rather than importing or
copying the supplied sources under GPL/AGPL or unclear terms. Consequently:

- no file from `JUCE/for import/2` is compiled or vendored into this package;
- `RImport2Filters.hpp` is self-contained C++17 and has no JUCE dependency;
- no unavailable framework type, `new`, runtime allocation, or external
  `TptFilter` implementation is required;
- topology/family labels are retained as conceptual references only; this is
  **not** a bit-identical port or a promise of plug-in-state compatibility with
  an upstream host project.

This is an engineering provenance record, not legal advice. Anyone redistributing
an independently modified package should still review the upstream material
and their intended distribution terms.

## Visible 1.9.10 inventory

All new choices are appended after frozen IDs `0..20`. `R` remains the visible
prefix policy. The MODE L list always presents an actual high-pass/low-cut
form before BP and LP where the family provides all responses; MODE H presents
actual low-pass/high-cut first.

| Canonical IDs | Family | Responses |
|---:|---|---|
| 21–26 | `R ANALOG` | LP12, LP24, BP12, BP24, HP12, HP24 |
| 27–32 | `R LINEAR` | LP12, LP24, BP12, BP24, HP12, HP24 |
| 33–35 | `R RBJ` | LP, BP, HP |
| 36–38 | `R TPT` | LP, BP, HP |
| 39 | `R HUV` | LP4 |
| 40–46 | `R HYPER` | LP2, LP4, BP2, BP4, HP2, HP4, NOTCH |
| 47 | `R KRAJ` | LP4 |
| 48 | `R MICRO` | LP4 |
| 49 | `R MUSIC` | LP4 |
| 50 | `R OBER` | LP4 |
| 51 | `R DVAL` | LP4 |

The GUI keeps the root popup short: `K35`, `MOOG`, `R CLASSIC`, and `R IMPORT
2`. The latter contains `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`, and a compact
`R LADDERS` group. This avoids a lower-edge inaccessible flat popup.

## Compatibility contract

- Canonical hybrid IDs `0..8` remain the 1.9.8 contract.
- IDs `9..20` remain the complete 1.9.9 R-classic contract.
- IDs `21..51` are new in 1.9.10 only.
- Schema is bumped from 24 to **25**.
- A state with schema `<24` accepts only historical `0..8` values.
- A schema-24 state accepts only `0..20`; a malformed 21–51 value normalizes
  to `NATIVE`, rather than being reinterpreted as a newer sound.
- A schema-25 state round-trips all valid `0..51` values.
- DLY `mnm|old|new`, DBAS/DWID feedback filters, optional Q controls, MODE S,
  and all 1.9.8/1.9.9 mappings remain unchanged.

## Runtime safety design

`RImport2Filters.hpp` owns independent state per channel through
`ReferenceImport2Stereo` and applies a common bounded output guard. It uses:

- no dynamic allocation in the processing path;
- finite checks and per-family reset-on-invalid-state guards;
- a shared 16-sample cutoff/resonance control ramp;
- reset only on a genuine mode change;
- host-rate preparation only when `RealFilterCore::setSampleRate()` sees a
  real rate change, not on repeated host snapshots;
- the retained outer physical-filter DC blocker and output limiter.

## Validation boundary

Standalone C++17 tests cover physical LP/BP/HP response checks for the
multimode families, all MODE L/H combinations through `FilterCore`, high-
control multi-rate finite stress, stereo isolation, repeated control/mode
snapshots, cutoff-ramp boundaries, sample-rate-change cadence, and schema
migration. The full JUCE module checkout/DAW runtime build is not available in
this source-only workspace, so host/plugin integration remains explicitly
outside this validation boundary.
