# FM+ `MODE SYNT = new` / `* fix` audit — 2026-09-28

This is a working-tree audit for the opt-in FM+ implementations and measured
control-law candidates. It does **not** replace the already delivered 1.9.17
source archive or claim a JUCE/VST3/DAW/listening validation.

## Selector contract

The three FM+ machines each own their own `mode_synt_m<ID>` value:

| ID | machine | allowed SYNT choices |
| --- | --- | --- |
| 8 | FM+ STAT | `mnm`, `old`, `new`, `mnm fix`, `new fix` |
| 9 | FM+ PAR | `mnm`, `old`, `new`, `mnm fix`, `new fix` |
| 10 | FM+ DYN | `mnm`, `old`, `new`, `mnm fix`, `new fix` |

All other machines and generic SYNT callers retain exactly `mnm|old`.
DLY independently retains its old `mnm|old|new` contract; no FIX value is
accepted there.

The stable serialized IDs are:

| value | name | meaning |
| ---: | --- | --- |
| 0 | `mnm` | retained local MNM route |
| 1 | `old` | retained prior implementation where applicable |
| 2 | `new` | preserved imported exact-core candidate |
| 3 | `mnm fix` | separate measured `MnmFixCore` control/profile candidate |
| 4 | `new fix` | separately namespaced imported exact-core/profile candidate |

Schema 30 appends values 3 and 4 without renumbering 0–2.  A schema before 29
cannot activate raw `2`; a schema before 30 cannot activate raw `3` or `4`.
Such values migrate to `mnm`. Non-FM machines always reject 2–4.

The faceplate `MODE SYNT` selector now uses the same per-machine choice list as
`DSP MODE`. This fixes the prior UI defect where DSP MODE could select raw
`new=2` but the face selector only knew `mnm|old` and could display a misleading
state.

## Measured FIX control profile

`Source/dsp/fm_fix/FmFixTables.hpp` is the sole local record for this profile.
Its Q23 `kStatParRatioWord` array is the **one canonical numerical table**:
MNM FIX derives float ratios from those exact words, NEW FIX receives the same
word pointer, and the separate strings are display text only. It is selected
only by `mnm fix` or `new fix`; retained `mnm`, `old`, and `new` are not
reinterpreted.

### STAT/PAR frequency controls

The literal user-confirmed 24-entry order is used for every `1FRQ`/`2FRQ`/`3FRQ`
control in the two FIX modes:

```text
1/64, 1/32, 1/16, 3/32, 1/8, 3/16, 1/4, 5/16,
3/8, 5/32, 7/16, 1/2, 5/8, 3/4, 7/8, 1,
1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4
```

The unusual literal `3/8 → 5/32 → 7/16` order is intentionally **not**
normalised. The index law is `floor((K + 0.5) * 3 / 16)`, `K=0..127`. Thus raw
factory STAT values `1FRQ=60` and `2FRQ=80` resolve to `1/2` and `1`.

### Faceplate/UI isolation

The faceplate now reads the selected machine-local SYNT mode before formatting
an FM value. Only raw SYNT `3` (`mnm fix`) and `4` (`new fix`) display this
measured STAT/PAR list, the measured DYN values, and semitone TUNE values
(`-2.000`, `0`, `+1.969` at raw `0`, `64`, `127`). Raw SYNT `0`/`1`/`2`
(`mnm`/`old`/`new`) retain their original comparison readouts and are **not**
relabeled with FIX values: m8/m9 retain `getFmListedRatio(raw/4)`; m10 retains
`v/16` and `pow(2,(v-32)/24)`; TUNE remains `raw-64`. This corrects both the
earlier UI-only FIX-label leak and a later erroneous raw-`0..127` fallback.
`FM_RETAINED_READOUT_ROUTES_2026-09-28.md` carries the literal baseline extract.

### DYN frequency controls

The FIX profile uses the direct laws evidenced by the supplied DYN sweep:

```text
1FRQ = K / 64
2FRQ = (K / 64)^2
```

Only the two FIX modes display the supplied useful aliases such as `1/64`,
`1/16`, `1/4`, `1/2`, `1`, and the nominal endpoint `4.0`; retained modes
keep their own comparison readouts.
The supplied pasted `1FRQ` sweep contained a repeated `0..0.656` section before
resuming at `1.34`; that would require a discontinuous reset at a mid-knob raw
value and conflicts with both the direct DSP multiplication and its later
anchors. It is therefore **not** silently invented as an audio reset in a FIX
mode. A raw-index capture proving such a reset can be added as a separate,
explicit profile if needed.

### TUNE and factory defaults

For all three FM+ FIX modes, `TUNE` is now:

```text
raw 0 (-64)  = -2.00000 semitones
raw 64       =  0
raw 127 (+63)= +1.96875 semitones
```

This preserves the existing local MNM endpoint law and corrects only NEW FIX's
previous approximately ±1-semitone host bridge. The retained `new` route is
left unchanged for A/B comparison.

The recovered FIX factory raw profiles are retained as explicit data only:

```text
STAT: 3C 40 50 1E 50 40 62 40
PAR:  3C 40 50 40 66 50 62 40
DYN:  40 40 40 40 4A 50 1E 40
```

They are **not** global APVTS/machine defaults. Global defaults were restored to
the pre-FIX baseline so fresh/reset `mnm`, `old`, and `new` instances cannot
change sound or knob positions. When `mnm fix` or `new fix` is selected, the
DSP MODE SYNT menu exposes the explicit user action `LOAD FIX FACTORY RAW
VALUES`; only that deliberate action writes the profile. Existing saved raw
parameter values are never rewritten on load.

## What is and is not changed

- `old` remains preserved. m8 OLD still aliases its historical MNM route;
  m9/m10 OLD still use their retained legacy endpoints.
- `mnm` remains preserved; its pre-FIX fingerprints remain in the regression
  suite.
- `new` retains the imported table and the supplied STAT vector anchor.
- `mnm fix` uses a separate `MnmFixCore`; the retained `MnmFm.hpp` source and
  state object are restored byte-for-byte to the pre-FIX baseline. It is a
  measured mapping candidate, not a claim that the old approximate
  MNM operator/envelope topology is now firmware-exact. In particular, that
  approximate DYN renderer calculates a `mod2` path but does not mix the
  generated `mod2` signal into its rendered output. Its DYN `2FRQ` mapping is
  therefore a helper/mapping proof only, **not** an audible DYN-topology fix.
- `new fix` owns a separate imported-core state object and changes its
  STAT/PAR table view and host TUNE mapping. The default imported exact table
  and state remain untouched for `new`. `new fix` is the meaningful candidate
  for a corrected-DYN-`2FRQ` listening comparison; this remains a candidate
  rather than a full hardware proof.
- Native FM TONE was **not** altered in this patch. The user-reported tonal
  filtering mismatch remains a separate measured task; no generic filter or
  smoothing substitute was added.

## Imported exact-core provenance and validation boundary

The reviewed upstream package remains:

`glassg333/mmnova` →
`decompiled data/juce/fm/!fm fix patch/1_NEW_code_FM_2026-09-28/dsp/mnm/`

Its installer was not run because it overwrites active `Source/dsp/mnm/`
files. The five retained raw cores remain isolated under `Source/dsp/fm_new/`,
and are restored byte-for-byte to the candidate baseline; the wrapped default
`new` core does not include local MNM/OLD headers and has no FIX table API.
A separate five-header copy under `Source/dsp/fm_new_fix/`, in namespace
`fmnewfix`, owns the measured STAT/PAR pointer and all NEW FIX register state.
Thus `new fix` cannot retain a table pointer or DSP state in default `new`.

The supplied STAT vector corpus was reproduced locally for the retained default
import:

```text
FM-STAT: 400 blocks, 52800 words, mismatches = 0  [100% BIT-EXACT]
```

That result validates the imported/default table path, not an end-to-end
hardware proof for the user-measured FIX table or host pitch-word bridge.
PAR/DYN supplied vector corpora were not shipped by the upstream package.

## Regression and delivery boundary

For both Synth and FX, the static verifier and a fresh 16-target JUCE-free
suite passed, including `NovaChorusTests`, `FmNewModeTests`, the new
`FmFixModeTests`, `ModeRollbackTests`, `DelayFeedbackDspTests`, all existing
core/filter/route regressions, and the unchanged MNM fingerprints.

No full JUCE/plugin/editor/DAW build, no listening comparison, and no claim of
universal hardware identity is made because compatible JUCE modules are absent.

The separate source-only FM FIX candidate archive packages this worktree. The
verified original `Monomachine-Nova-1.9.17.zip` and prior FM NEW candidate are
not replaced.
