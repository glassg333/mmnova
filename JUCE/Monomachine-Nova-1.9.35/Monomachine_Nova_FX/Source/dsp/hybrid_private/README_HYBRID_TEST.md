# Private hybrid DSP module

This directory contains the active private DSP module used by Nova's separate
`MODE L`, `MODE H`, and `MODE S` controls. P1 and P2 store independent values;
the visible control attaches to the selected P1/P2 view.

## Physical filter-side choices

The stored MODE L/H value is a **physical-side choice index**, translated in
`HybridDSP.hpp` to a canonical filter algorithm ID:

- IDs `0..8` are frozen for 1.9.8 compatibility.
- IDs `9..20` are frozen R-classic responses appended in 1.9.9.
- IDs `21..51` are R Import 2 responses appended in 1.9.10.

| Control | Physical edge | Within-family response order |
|---|---|---|
| `MODE L` / `BASE` | lower / low-cut edge | `HP → BP → LP` |
| `MODE H` / `BASE+WDTH` | upper / high-cut edge | `LP → BP → HP` |

Each side offers `NATIVE`, the existing K35 and MOOG-style ladder responses,
plus these appended visible families:

- the one UI `R CLASSIC` branch: `R 303`, `R MS20`, `R MOOG`,
  `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`, and `R LADDERS`;
- `R LADDERS`: `R HUV`, `R HYPER`, `R KRAJ`, `R MICRO`, `R MUSIC`,
  `R OBER`, `R DVAL`.

`MODE S` remains the separate existing post-filter DIST selector:
`MNM | OLD | MNM FIX | FOLD | ZERO | CLAMP | MNM+OLD | MNM V2 | OLD V2`.

The first six names retain their existing IDs and reference behavior. The last
three are appended 1.9.15 listening candidates, not firmware claims: `MNM+OLD`
keeps MNM's positive headroom trim with a soft OLD-style curve; `MNM V2` keeps
the MNM hard shape with a gentler positive output trim; `OLD V2` keeps OLD's
negative/zero side and supplies more positive drive with moderated level. At
LCD `DIST=0`, every candidate is unity. Compare them at matched output levels
before removing either `MNM` or `OLD`.

The private choices do not add or move a physical stage: L/H replace only the
selected side. `BASE`, `WDTH`, filter-envelope offsets, manual routing, and the
native P2 machine list remain separate. With both L/H selectors `NATIVE`, DSP
FILT `old` keeps the retained legacy reference. A non-`NATIVE` L/H selection
explicitly requests the hybrid-capable FilterCore even when DSP FILT stays
`old`; DSP FILT `mnm` continues to choose that path directly.

## Source boundaries

`RClassicFilters.hpp` is a self-contained, attributed adaptation of the
user-supplied TB303/MS20/Moog sources at glassg333/mmnova commit
`3791a32187b5dcffb0efb6832156a688ab93ece7`.

`RImport2Filters.hpp` is deliberately different: it is a self-contained,
independent C++17 equivalent bundle for the audited `JUCE/for import/2` set at
commit `a08334d586ac972c9ac932461ef3631924d31af9`. The supplied directory has
unavailable framework dependencies and mixed/incomplete provenance, so no
source file from it is compiled or copied here. The labels identify topology
families, not bit-identical upstream ports. See
[`../../../R_IMPORT2_AUDIT_2026-09-27.md`](../../../R_IMPORT2_AUDIT_2026-09-27.md).

## Test coverage

`tests/HybridDspTests.cpp` checks:

- immutable `0..20` mapping plus appended `21..51` mapping;
- explicit OLD/NATIVE versus OLD/non-NATIVE hybrid-route policy;
- all 52×52 MODE L/H combinations through the actual `FilterCore` bridge;
- finite bounded output and physical-side dispatch;
- existing, R-classic, and appended-mode cutoff-control boundaries.

`tests/Import2FiltersTests.cpp` checks:

- small-signal physical LP/BP/HP responses for the multimode families;
- measured open-cutoff pass-band level for every single-output R ladder;
- multi-rate/high-control finite stress and the common output safety bound;
- independent stereo state;
- repeated same-mode host snapshots without reset;
- the sample-rate-aware 10 ms control-ramp boundary.

`tests/MnmRealFilterTests.cpp` additionally checks unchanged-rate bit-exact
continuity for all imported modes, repeated same-mode snapshots, and a genuine
48 kHz → 44.1 kHz reprepare followed by identical-rate no-op calls.
