# 1.9.15 — P2 filter CPU repair, native no-auto-clip, and MODE S comparison set

## P2 HPF/LPF keytracking CPU repair

The P2 route can reach filter setup in one-sample render slices. In the prior
source, an unchanged `setParameters()` snapshot still called `apply()`, and an
unchanged `setHybridTestModes()` snapshot did so too. With HPF and/or LPF
keytracking enabled, each unnecessary `apply()` also repeated tracking cutoff
lookup/coefficient work.

The controls remain independent and manual-derived. This revision only skips
rebuilding state when its input snapshot is unchanged. A real parameter, played
note, tracking-switch, or MODE L/H change still rebuilds the appropriate
filter state. It is therefore a removal of redundant control work, not a
smoothing ramp or a change to the filter law.

A deliberately pathological standalone hot-path measurement that called the
same parameter and MODE L/H snapshot for each of 44,100 samples changed from
1937.781 ms in 1.9.14 to 2.115 ms in this source revision. This demonstrates
that the redundant work was removed; it is not a DAW CPU-percentage claim.

## Native filter output

The former native `RealFilterCore` output safety transfer was a generic soft
clip with a nominal ceiling of 8 and an onset near absolute output 2. It was
not an intentional Monomachine saturation control. It is removed from the
native filter route.

The selected K35/ladder/R physical route was already dispatched to its
separate `IndependentPhysicalFilterCore` and did not receive that generic clip
in the normal production route. Thus removal can change native high-Q output,
but does not by itself explain or solve a complaint shared by all independent
filter choices. The selected physical route retains its own mathematical core
behavior; explicit `FILT SAT` and post-filter `TRACK DIST` remain separate.

## MODE S comparison choices

Existing serialized IDs retain their meaning:

| ID | Choice | Status |
|---:|---|---|
| 0 | `MNM` | retained reference |
| 1 | `OLD` | retained reference |
| 2–5 | `MNM FIX`, `FOLD`, `ZERO`, `CLAMP` | retained comparisons |
| 6 | `MNM+OLD` | appended 1.9.15 candidate |
| 7 | `MNM V2` | appended 1.9.15 candidate |
| 8 | `OLD V2` | appended 1.9.15 candidate |

All modes are exact unity at LCD `DIST=0`.

- `MNM+OLD`: exact current MNM negative/centre behavior and exact MNM positive
  output/headroom compensation, with OLD's normalized soft positive curve.
- `MNM V2`: exact current MNM negative/centre behavior and hard positive
  pre-drive, with square-root rather than current MNM output compensation.
- `OLD V2`: retained OLD negative/centre behavior, stronger soft positive drive,
  and moderated positive output trim.

These are explicit A/B choices, not a claim that a candidate equals firmware.
Neither `MNM` nor `OLD` should be removed before a listening decision.

## State and UI

Schema 27 appends IDs 6–8. A pre-schema-27 state preserves valid IDs 0–5; an
impossible future candidate ID in such an old state normalizes to `MNM` rather
than being repurposed as a new sound. Schema-27 states preserve P1/P2 choices
independently. The MODE S selector visibly exposes all nine choices in both
Synth and FX.

## Suggested listening matrix

Use a sustained harmonic-rich source and a transient source. Set `FILT SAT=0`
for the first pass, level-match after the block, and compare `MNM`, `OLD`,
`MNM+OLD`, `MNM V2`, and `OLD V2` at `DIST=0`, `+16`, `+32`, and `+63`.
Record onset grit, sustain compression, output level, and interaction with
filter resonance. This separates a MODE S transfer-law choice from filter
saturation or later gain staging.
