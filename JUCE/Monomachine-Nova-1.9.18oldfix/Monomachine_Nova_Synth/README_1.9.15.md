# Monomachine Nova Synth 1.9.15 — source-only listening candidate

This tree is a **source-only candidate**. No ZIP archive, JUCE plug-in build,
DAW load, or interactive editor test has been claimed or produced yet. The
Synth and FX source changes are mirrored.

## P2 key tracking and native filter output

- Independent manual-derived `HPF KEYTRACK` and `LPF KEYTRACK` controls remain
  independent. Unchanged native parameter snapshots and unchanged MODE L/H
  selections now skip redundant coefficient rebuilds, which removes the
  pathological per-sample work reached by the one-sample P2 route.
- The native MNM filter no longer applies a hidden generic output soft clip.
  The DC blocker remains; intentional `FILT SAT` and `TRACK DIST` nonlinear
  paths remain separate. High-Q output is required to remain finite, but is
  allowed to exceed the former roughly ±2 knee.
- Filter-menu separator painting/height is repaired so dividers are visibly
  lines rather than blank selectable-looking gaps.

No broad filter-envelope or signal-order theory was introduced: the documented
route remains `headroom → EQ → FILTER → DISTORTION → AMP ENV → VOL/PAN → SRR →
DELAY → LEVEL`.

## MODE S A/B candidates

Existing serialized selections retain their IDs and behavior:

| ID | MODE S | Status |
|---:|---|---|
| 0 | `MNM` | retained reference |
| 1 | `OLD` | retained reference |
| 2–5 | `MNM FIX`, `FOLD`, `ZERO`, `CLAMP` | retained existing comparisons |
| 6 | `MNM+OLD` | appended candidate |
| 7 | `MNM V2` | appended candidate |
| 8 | `OLD V2` | appended candidate |

Schema 27 appends IDs 6–8 only. States written before schema 27 preserve valid
IDs 0–5; an impossible later value in an older state normalizes to `MNM` rather
than being interpreted as a new candidate. A schema-27 state retains each P1/P2
candidate independently.

All retained and appended transfer laws are exact unity at LCD `DIST=0` (raw
centre 64). Their positive side is deliberately different:

- **MNM+OLD** retains MNM's exact positive output/headroom compensation while
  substituting OLD's normalized `tanh` curve for the positive drive.
- **MNM V2** retains MNM's exact negative/centre path and hard pre-drive, but
  uses square-root positive output compensation. It isolates a level/headroom
  question from a soft-versus-hard curve question.
- **OLD V2** retains OLD's negative/centre path, then uses a stronger soft
  positive drive with a moderated positive output trim.

These are named comparison candidates, **not firmware-identity claims**. In
particular, the reported loud-but-under-distorted `OLD +63` behavior has not
been relabelled as solved. Compare references and candidates at matched output
levels before removing either `MNM` or `OLD`.

## Suggested listening matrix

For a sustained harmonic-rich source and a transient source, compare `MNM`,
`OLD`, `MNM+OLD`, `MNM V2`, and `OLD V2` at `DIST=0`, `+16`, `+32`, and `+63`.
Level-match after the block, then note (1) onset grit, (2) sustain compression,
(3) resonance interaction near the lower-filter region, and (4) output level.
Keep `FILT SAT` at zero for a first pass. This makes a `MODE S` conclusion
separate from intentional FILT saturation or downstream gain staging.

## Source validation performed

- `DistVariantTests`: exact zero identity, unchanged OLD reference law,
  negative headroom endpoints, candidate endpoint/curve distinction, and
  finite sweeps. `TrackDistVariantIntegrationTests` separately compiles the
  production `TrackDIST.inl` body and verifies exact dispatch of IDs 6–8.
- Existing standalone DSP targets passed in both trees, including `MnmCore`,
  `MnmRealFilter`, `HybridDsp`, `FilterRoute`,
  `TrackFilterRouteIntegration`, `Import2Filters`, `FilterExtras`,
  `DelayFeedback`, `ModEnvMatrix`, and `ArpWindow`.
- The project-local static verifier passed in both trees.

The JUCE modules required for a full plug-in/editor build were not available in
this environment. Therefore this document is not evidence of a compiled VST3,
DAW CPU percentage, GUI interaction test, or final sonic judgment.
