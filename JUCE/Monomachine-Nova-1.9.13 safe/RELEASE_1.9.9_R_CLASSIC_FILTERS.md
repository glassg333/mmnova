# Monomachine Nova 1.9.9 — R 303, R MS20 and R MOOG hybrid physical filters

This is a **new source-only patch release** after 1.9.8. It retains the complete
1.9.8 delay work (`DLY mnm|old|new`, DBAS/DWID feedback filtering and optional
Q controls) and adds the requested reference-classic filter family to both
`Monomachine_Nova_Synth` and `Monomachine_Nova_FX`.

## Delivered scope

Both P1 and P2 physical filter chains now expose these appended response forms:

| Canonical hybrid algorithm ID | Response |
|---:|---|
| 9 | `R 303 LP` |
| 10 | `R 303 BP` |
| 11 | `R 303 HP` |
| 12 | `R MS20 LP` |
| 13 | `R MS20 BP` |
| 14 | `R MS20 HP` |
| 15 | `R MOOG LP24` |
| 16 | `R MOOG LP12` |
| 17 | `R MOOG BP24` |
| 18 | `R MOOG BP12` |
| 19 | `R MOOG HP24` |
| 20 | `R MOOG HP12` |

The user-facing prefix is intentionally `R` for every new family. `R` means
**reference/adapted source family**, not a claim of bit-exact hardware or
firmware emulation.

### Physical-side menu law

The UI stores a physical-side choice index, then translates it to the canonical
IDs above. It does not move either physical edge.

- **MODE L / BASE (lower edge):** NATIVE, existing HP choices, then R HP
  choices; BP alternatives follow; LP alternatives come last. Within each
  compact group this is `HP → BP → LP`.
- **MODE H / WDTH (upper edge):** NATIVE, existing LP choices, then R LP
  choices; BP alternatives follow; HP alternatives come last. Within each
  compact group this is `LP → BP → HP`.

Thus an item labelled HP actually renders the HP response on the lower/low-cut
side, and an item labelled LP actually renders the LP response on the
upper/high-cut side. The user may still deliberately choose any response on
either physical side; the label and DSP response remain truthful.

## Compatibility and saved state

- IDs **0..8 are unchanged** from 1.9.8:
  `NATIVE`, `K35 LP/HP`, and the existing six MOOG-style ladder responses.
- New IDs were appended after 8; none were inserted or repurposed.
- State schema advances from **23 to 24**.
- A state with schema 23 or earlier preserves valid MODE L/H values `0..8`.
  A malformed old value outside that historic range is made `NATIVE`, rather
  than being silently reinterpreted as an R-family sound.
- A schema-24 state preserves values `0..20` for P1 and P2 independently.

## UI accessibility

The old helper forced every ComboBox menu below the control. MODE L/H live near
the lower part of the front panel, so a 21-row flat list could run below the
plug-in window. In 1.9.9:

1. generic ComboBox popups use target-component, screen-aware placement rather
   than a forced below-control rectangle; and
2. MODE L/H use compact `K35`, `MOOG`, `R 303`, `R MS20`, and `R MOOG` submenus.

Every added choice remains reachable without requiring a taller plug-in window.
The selected item is still the same serializable choice index, and mouse drag,
wheel, host automation, and keyboard ComboBox semantics remain available.

## Imported-source provenance and adaptation boundary

The requested sources were supplied at:

- `https://github.com/glassg333/mmnova/tree/main/JUCE/for%20import`
- exact reviewed commit: `3791a32187b5dcffb0efb6832156a688ab93ece7`

Reviewed input files:

- `TB303.cpp` SHA-256 `af398fdfad281f1ad178d75d5366451ef56012ae8b5bd23048903ee17dd02155`
- `TB303.h` SHA-256 `6bc696814a096110a077a8f1b550fa8bd4a7779e04a2c0ec5b4c90e263111823`
- `MS20.cpp` SHA-256 `36d4c5c7d6fe5d9d9786893c7064eebdf3d7a2884617645761263c8eaf93eb85`
- `MS20.h` SHA-256 `eaa482bac680102ae97f23e318276e196d72660509eed2d4c65a603fcf4ac26e`
- `Moog.cpp` SHA-256 `e252b0d9e5b12849e22c17358cc02b96ea1496b47640a34b31b3eb57c63ff537`
- `Moog.h` SHA-256 `e645dd0708dec5a5a347d4d4fbfc4cffa131a89a14989e142a1f81676bf81b75`

Those six files include a missing external `Filter.h` framework and unavailable
helpers (`Filter`, `Lerp`, coefficient/tanh lookup helpers, drive/mode
infrastructure). They therefore cannot truthfully be copied as a compilable
Nova module. The active implementation is the self-contained, attributed
adaptation:

- `Source/dsp/hybrid_private/RClassicFilters.hpp`
- bridge/ID mapping: `Source/dsp/hybrid_private/HybridDSP.hpp`
- physical routing: `Source/dsp/mnm/MnmRealFilter.hpp`

It preserves the supplied TB303/MS20/Moog nonlinear state equations and their
LP/BP/HP or 12/24-pole output laws where applicable. The absent framework
utilities are replaced only with allocation-free C++17 equivalents:
pre-warp coefficient calculation, `std::tanh`, finite guards, safe endpoint
clamps, and the existing 16-sample control-target ramp. The TB303 coefficient
lookup has an explicit calibrated replacement so its physical cutoff follows
the named BASE/WDTH edge rather than an absent LUT's unobservable scaling.

No external raw source is represented as having compiled unchanged, and no
hardware-identical claim is made.

## De-click and sample-rate behavior

Each R adapter has independent left/right nonlinear state. Controls are targets
that ramp over 16 samples. Repeating the same host mode snapshot does not reset
the adapter. `RealFilterCore::setSampleRate()` remains a strict no-op when the
host rate is unchanged; only a real sample-rate change calls `prepare()` and
clears state.

## Validation

See [`VALIDATION_1.9.9_R_CLASSIC_FILTERS.md`](VALIDATION_1.9.9_R_CLASSIC_FILTERS.md).
The source package includes standalone DSP regression targets and state/UI
static checks. A full JUCE module checkout and DAW runtime session were not
available in the delivery environment; that boundary is stated rather than
being claimed as completed.
