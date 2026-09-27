# Hybrid filter / DIST evidence audit — 2026-09-26

## Question

Before adding high-Q output compensation, filter darkening, cutoff smoothing,
or an `MNM FIX` saturation curve, determine what is measured in the local DSP
and what the recovered Monomachine evidence actually establishes.

## Recovered filter evidence

The reconstructed filter documents and `monomachine_track_filter.hpp` establish:

- `BASE`, `WDTH`, `HPQ`, and `LPQ` are filter controls; there is no recovered
  filter-envelope parameter in this part of the track page.
- `BASE` addresses the lower side and `BASE + WDTH` the upper side.
- HPQ selects a damping table and an HP feedback-gain table; LPQ selects an LP
  feedback-gain table.
- The recovered architecture includes two coefficient stages, a 2× halfband FIR
  step, and a fractional-tap resonator/comb stage. It is not evidence for a
  simple global Q makeup-gain law.
- Controls are written by the CPU between DSP frames. The evidence does not yet
  establish an interpolated imported-filter cutoff law suitable for a “click
  fix”.

Relevant recovered sources:

- `decompiled data/12_mining_archive/01_filter/README_filter_algorithm.md`
- `decompiled data/12_mining_archive/01_filter/README_manual_crosscheck.md`
- `decompiled data/12_mining_archive/01_filter/monomachine_track_filter.hpp`
- reconstructed DIST port: `09_dsp_models_verified/09_juce_port/Source/mnm_dist.h`

The local `FilterCore` is a retained approximation and does not reproduce the
verified fractional-tap resonator stage. Therefore it must not be described as
proving original high-Q loudness behaviour.

## Measured local native Q response

48 kHz, 500 Hz sine at amplitude 0.25, `BASE=0`, `WDTH=64`, `HPQ=0`:

| LPQ | Measured gain |
|---:|---:|
| 0 | -4.394 dB |
| 16 | -11.275 dB |
| 32 | -7.602 dB |
| 64 | -5.387 dB |
| 96 | -5.096 dB |
| 127 | -5.038 dB |

This is non-monotonic and parameter-dependent. It does **not** justify a
single Q-dependent output compensation curve.

## Direct imported-side cutoff-step regression measurement

`HybridDspTests` now directly steps `BASE` from 20 to 84 at sample 512 using a
48 kHz, 0.8-amplitude sine, `WDTH=40`, `HPQ=LPQ=127`, no filter-envelope
offset. The maximum adjacent-sample discontinuities measured in both products
were:

| Selected side / response | Max adjacent-sample jump |
|---|---:|
| lower K35 LP | 0.284921 |
| lower K35 HP | 5.35243 |
| upper ladder HP24 | 1.53906 |
| upper ladder HP12 | 4.14371 |

An earlier isolated high-resonance probe using a different cutoff transition
also found material jumps: lower K35 LP/HP 0.468089 / 0.712567 and upper
ladder HP24/HP12 0.234291 / 0.240101.

The two procedures use different control values, so the numbers should not be
merged into one threshold. They consistently establish the same finding:
coefficient/cutoff changes in several imported high-Q paths can create a
material waveform step. The test only guards against non-finite/runaway output
and records the baseline; it does not claim an acceptable click level.

## Current MNM DIST versus recovered Monomachine DIST

| Aspect | Current local `MnmKernel::Saturator` | Recovered firmware reconstruction |
|---|---|---|
| Transfer scope | One shared, memoryless local saturation law | Per-machine handler slot; slot zero is the clean path |
| DIST below/at 64 | Local attenuation below 64 and neutral at 64 | Verified 16-sample drive-envelope state machine, including a DISTINCT DIST=0 wrap/decay trajectory |
| DIST above 64 | Local increased pre-drive plus output division | Verified drive envelope/timbre-index preparation; handler output law remains handler-specific |
| Time structure | Audio-rate local de-zipper in the host integration | Drive updates once per 16-sample DSP frame; located SWAVE-ENS handler also has a 64-frame fade-in |
| Handler audio body | Shared local curve | Located SWAVE-ENS body uses rotating/banked/tap processing; not reconstructed sample-for-sample |

They are therefore not established as equivalent. The local MNM option remains
a reference for this project, but it cannot honestly be relabelled as a
universal original DIST implementation.

## DIST evidence boundary

The recovered DIST code verifies a 16-sample drive envelope and a per-machine
handler-slot dispatcher. It also documents a 64-frame handler fade-in. The
located SWAVE-ENS handler body has a rotating/banked, tap-based structure but
is not yet reconstructed sample-for-sample. Consequently:

- A shared universal replacement saturation curve cannot honestly be called
  original MNM DIST.
- An audio-shaping `MNM FIX` has not been added.
- Current `MNM` and `OLD` remain direct references for comparison.
- MODE S imported characters now use the retained MNM knob progression only as
  an integration/control convention, not as an original-hardware claim.

## User-directed experimental changes after the audit

The audit originally withheld both a generic MNM FIX and imported-side
smoothing. The user subsequently requested two explicit, separately selectable
test actions:

1. **MNM FIX** is now an experimental A/B candidate, not a firmware claim. It
   applies a +3 dB, Q=1.0 peak at the reported 3.5 kHz location after the
   retained MNM result, blended only above DIST=64. MNM and OLD are unchanged.
2. **Imported K35/ladder controls** now ramp cutoff and resonance over 16
   samples. Native FilterCore and OLD are untouched.

The direct test now isolates the selected physical side and shows the immediate
step was reduced from 0.024643/0.056228/1.539230/4.147610 to
0.001143/0.001366/0.305369/0.180665 for K35 LP, K35 HP, ladder HP24, and
ladder HP12 respectively. Later high-Q resonant excursions are still printed
by the test and are not described as solved; no limiter or general filter
darkening was added.

This keeps the evidence boundary explicit: `MNM FIX` is a user-directed
comparative correction, not a reconstructed per-machine handler.
