# Monomachine Nova — FILT / AMP / DIST evidence audit

**Date:** 2026-09-28  
**Scope:** source-forensics only. No production source was edited and no archive was made.

## Authority order for the next fix

1. Monomachine OS 1.32 manual and the user's observed hardware behaviour.
2. Existing mined material only where it identifies an operation / table / control path and does not conflict with (1).
3. Current Nova source, as the thing to compare and repair — not as evidence that its approximation is correct.

Do **not** use a generic smoother, hidden native clipper, a new global chain order, or an unverified `exact` header as a solution.

## Manual facts that directly match the reported failure

| Manual fact | Consequence for Nova |
|---|---|
| The filter is a resonant 24 dB low/high/band-pass system. `BASE` is HP cutoff when `WDTH` is max; `WDTH` is LP cutoff when `BASE` is minimum. | BASE/WDTH must remain the two physical edges; this is not a conventional one-cutoff filter. |
| `HPQ` and `LPQ` independently boost level around their respective cutoffs. | A generic shared Q law is not a suitable claim of hardware behaviour. |
| FILT `ATK`/`DEC` are an envelope triggered by a **FILTER-trig**. `BOFS` is added to BASE and `WOFS` is added to WDTH under that envelope. | BOFS/WOFS cannot be reclassified as unrelated delay controls or discarded. The reported `BASE=0, BOFS≈+30, DEC≈30` kick/laser case is a primary acceptance probe. |
| Normally the filter tracks note pitch: with BASE=0 the HP edge is two octaves below the note; both BASE and WDTH move one octave per eight steps. HPF/LPF tracking can be independently disabled. The manual explicitly recommends disabling HPF tracking when using HPQ as a low-frequency “loudness” boost. | The current tracking formula is directionally correct, but test state (HPF keytrack) matters greatly for the low-Q/hum comparison. |

Manual pages: 30–31 / online pages 38–39.

## What the current native code actually does

Relevant mirrored files are byte-identical in Synth and FX at the start of this audit.

### `Source/dsp/mnm/MnmRealFilter.hpp`

* FILTER page call order is `BASE, WDTH, HPQ, LPQ, ATK, DEC, BOFS, WOFS`.
* UI and conversion code make BOFS/WOFS bipolar: stored `0..127`, neutral `64`, so an LCD/display value `+30` is raw `94`.
* Current control law is linear:

  ```text
  BASE' = clamp(BASE + env * signed_BOFS, 0, 127)
  WDTH' = clamp(WDTH + env * signed_WOFS, 0, 127)
  ```

* Current normal tracking uses `noteHz / 4 * 2^(control / 8)`, which agrees with the manual's base/octave semantics.
* With tracking disabled, it switches to an existing native table-to-Hz approximation.
* Current native Q is explicitly a placeholder: `Q = 0.5 + 15.5 * (raw / 127)^2`; its resonant stage is disabled completely for raw Q `<= 0.5`.
* The native high-Q stage is an extra generic TPT/SVF **before** the native HP table stage; the native LP Q stage is **after** the native LP table stage. That is neither an established Monomachine Q-table law nor an evidenced recurrence/routing claim.
* Imported filters are extension/diagnostic modes only, not a Monomachine-manual target and not an explanation for the native defect. Their shared control entry can reveal a common routing problem, but they must not be used as a reference response or as a reason to defer the native repair.
* There is no generic native output soft-clip in this path. The output still passes the existing 8 Hz DC blocker, which must not be casually blamed for or used to solve the resonance issue.

### `Source/NovaDSP.h` / `Source/dsp/mnm/MnmKernel.hpp`

* Current AMP “MNM” path is still a custom `AmpEnvelope` integration: it adds a host-time hold approximation, supplies a fake zero sustain, and does not implement a proven original AHDR state/timing path as a single unit.
* Current DIST `Saturator` is a generic positive drive + hard clamp / output division approximation. The retained `OLD`, `MNM+OLD`, `MNMv2`, and `OLDv2` comparisons remain untouched. No conclusion about global routing follows from this audit.

## Already-mined material: usable evidence vs. exclusions

| Existing mined artifact | What it provides | Status for integration |
|---|---|---|
| `filter/MnMFilter24CORE_VERIFIED.h` | A concrete 16-frame FILT-envelope candidate, cutoff-table path, separate HP/LP Q-table paths, and a base/width envelope curve. | Useful lead for recovering current missing dynamic behaviour. **Not a drop-in:** its BOFS/WOFS reconstruction uses a magnitude-squared / flag-dependent path, while Nova exposes signed `-64..+63` display controls. Sign and UI-to-DSP conversion must be resolved before copying its formula. |
| `filter/MNM_FILTER_PRACTICAL/FILTER_CLUES_CONSOLIDATED_RU.md` | Explicit evidence grading. It supports the manual page semantics, confirms that Q uses table/interpolation machinery rather than a simple textbook Q formula, and warns that full recurrence/routing is not completely closed. | Primary mined guide for bounded work. It prevents calling the current generic SVF or any header “exact.” |
| `filter/MnmFilterExact.hpp` | A different frame filter/resonator model that claims FILT AD/BOFS/WOFS are not filter-envelope controls. | **Excluded from parameter-ownership decisions.** It conflicts with the manual and with the user's hardware behaviour; no code from that claim may remove/re-route the FILT envelope. |
| `routing .../voice_page_map.md` | A later alternate offset attribution that treats some candidate words as delay-mod controls. | Context only, not sufficient to overrule the manual/UI mapping. It conflicts with the above filter evidence, so it must not drive a change by itself. |
| `distortion/MnMAmpDistCORE_VERIFIED.h` | A candidate fixed-point DIST transfer: hard 24-bit saturation, a derived drive curve, and a possible cutoff coupling. It also marks the ColdFire knob-to-DSP mapping unresolved. | Useful for a later, isolated DIST comparison. It does **not** authorize changing the documented Nova chain or calling the transfer exact at the UI knob. |
| `envelope/mnm_amp_env.h`, `MnmEnvExact.hpp` | 16-frame AHDR/state-machine candidates and timing tables. | Useful for AMP audit only; these artifacts disagree on retrigger details, so neither is a blind replacement. |

## Table-timing check (closed)

The current `MnmTables.hpp` FILT/AMP attack and decay tables match the locally mined `MnmEnvExactTables.hpp` source values (within the printed float precision), and the current native FILT envelope already advances once per 16 samples. Therefore the reported weak BOFS/DEC result is **not** evidence for replacing the two rate tables or adding generic smoothing.

The remaining uncertainty is the control-to-coefficient / resonance path after that correctly timed envelope tick.

## Supported discrepancies and priority

1. **Q:** Current `0.5..16` quadratic native law and per-import normalized laws are unambiguously approximations; mined evidence says hardware Q uses parameter-specific interpolation/tables. This is the strongest explanation for the gross mismatch between native and imported Q reactions.
2. **FILT envelope depth/control path:** Current linear raw-unit BOFS/WOFS offsets remain only a UI-level approximation of the mined indexed/coefficient path. However its 16-frame timing tables are already sourced correctly. This directs work to the signed control-to-coefficient/resonance connection, not to new envelope timing curves.
3. **Filter topology/recurrence:** Current extra SVFs placed around table HP/LP stages are not established by manual or mined evidence. Do not “fix” Q merely by enlarging their generic Q maximum.
4. **AMP:** Current `MNM` envelope is an approximation with host-time hold behaviour and needs a separate state/timing audit.
5. **DIST:** Current modes are comparison laws, not demonstrated hardware DIST. Preserve all named comparisons and defer change until the filter source/chain question is isolated.

## Boundaries for the next code change

* Preserve the user's authoritative chain: headroom → EQ → FILTER → DISTORTION → AMP ENV → VOL/PAN → SRR → DELAY → LEVEL.
* Do not roll back, add a generic limiter/soft clip, substitute P2 or a smoothing ramp, or overwrite OLD/MNM reference comparisons.
* First repair must be small, mirrored in Synth and FX, and test the three manual probes: BASE/WDTH edge roles, low-frequency HPQ with HPF tracking disabled, and filter-triggered BOFS/DEC movement.
* Before importing a mined BOFS curve, resolve its signed display control (`+30` = raw 94) against its flag/magnitude notation. The correct answer is not to silently treat a signed knob as an always-positive magnitude.
