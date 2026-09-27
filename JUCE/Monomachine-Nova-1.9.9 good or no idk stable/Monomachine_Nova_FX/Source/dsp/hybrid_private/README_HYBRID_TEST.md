# Private hybrid DSP module

This directory contains the active private DSP module used by Nova's separate
`MODE L`, `MODE H`, and `MODE S` controls. P1 and P2 store independent values;
the visible control attaches to the selected P1/P2 view.

## Physical filter-side choices

The stored MODE L/H value is a **physical-side choice index**, translated in
`HybridDSP.hpp` to a canonical filter algorithm ID. IDs `0..8` are frozen for
1.9.8 compatibility. IDs `9..20` append the R-family responses in 1.9.9.

| Control | Physical edge | Within-family response order |
|---|---|---|
| `MODE L` / `BASE` | lower / low-cut edge | `HP → BP → LP` |
| `MODE H` / `BASE+WDTH` | upper / high-cut edge | `LP → BP → HP` |

Each side offers `NATIVE`, the existing K35 and MOOG-style ladder responses,
and these reference/adapted families:

- `R 303`: LP, BP, HP;
- `R MS20`: LP, BP, HP;
- `R MOOG`: LP12/LP24, BP12/BP24, HP12/HP24.

`MODE S` remains the separate existing post-filter DIST selector:
`MNM | OLD | MNM FIX | FOLD | ZERO | CLAMP`.

The private choices do not add or move a physical stage: L/H replace only the
selected side. `BASE`, `WDTH`, filter-envelope offsets, manual routing, the
native P2 machine list, and the retained `old` reference remain separate. The
hybrid choices run only with the underlying `mnm` path.

## R-family source boundary

`RClassicFilters.hpp` is a self-contained, attributed adaptation of the
user-supplied TB303/MS20/Moog sources at glassg333/mmnova commit
`3791a32187b5dcffb0efb6832156a688ab93ece7`. The supplied six files depend on an
absent `Filter.h` framework, so this directory does not claim to compile them
verbatim. The adaptation retains their nonlinear state/output equations and
replaces only unavailable host helpers with allocation-free finite C++17
utilities and the existing 16-sample control ramp.

## Test coverage

`tests/HybridDspTests.cpp` checks:

- immutable `0..8` mapping plus appended `9..20` mapping;
- LP/BP/HP response semantics for every R family;
- all 21×21 MODE L/H combinations through the actual `FilterCore` bridge;
- finite bounded output, multi-rate/control stress, and stereo independence;
- existing and R-family cutoff-ramp control boundaries.

`tests/MnmRealFilterTests.cpp` additionally checks unchanged-rate bit-exact
continuity for all imported modes, repeated same-mode snapshots, and a genuine
48 kHz → 44.1 kHz reprepare followed by identical-rate no-op calls.
