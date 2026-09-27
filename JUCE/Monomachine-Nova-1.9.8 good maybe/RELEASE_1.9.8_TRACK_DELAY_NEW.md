# Monomachine Nova 1.9.8 — usable delay feedback filters and `NEW` Track Delay

Date: 2026-09-27

## Purpose and scope

This source-only release repairs the practical use of the EFFX delay `DBAS` and `DWID` controls in both paired products:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

It also appends an opt-in delay algorithm named **`NEW`**. `NEW` is informed by the bounded recovered Track Delay material supplied under `glassg333/mmnova/.../DSP mm orig/trackdelay`: its 16-sample control-frame scheduling, reciprocal delay-time geometry, independent stereo feedback loops, optional cross-feedback, and a BASE/WIDTH-shaped feedback stage. The recovered material does **not** establish the complete original host parameter mapping or a complete bit-exact DSP replacement. Accordingly, `NEW` is a documented, practical candidate—not a claim of a firmware-exact port.

The work deliberately uses the existing imported Korg-related filter support for the feedback loop rather than attempting a long firmware-mining cycle.

## Delay modes and compatibility

`MODE DLY` now has these ordered choices:

| Saved / automation value | Mode | Status |
| --- | --- | --- |
| `0` | `mnm` | retained native delay |
| `1` | `old` | retained Nova delay |
| `2` | `new` | appended opt-in Track Delay candidate |

`mnm` and `old` remain available at their original values. Only the two delay selectors—`mode_dly` and `p2_mode_dly`—have the range `0…2`; no non-delay selector gains `NEW`.

The state schema is now **23**. On loading a state written by schema 22 or older, a DLY value of `2` is treated as a previously withdrawn/unknown choice and becomes `mnm`, rather than silently becoming `NEW`. Schema-23 states preserve intentional `new` values. Out-of-range delay values also fall back to `mnm`.

## DBAS, DWID, and optional Q controls

The two visible EFFX DLY knobs now control filters **inside the feedback recurrence**, so they shape subsequent repeats rather than acting as a cosmetic post-delay EQ:

- **DBAS** is the lower feedback edge: a Korg-35 high-pass cutoff. Raising it removes more low-frequency repeat energy.
- **DWID** is the upper feedback width: a Korg-35 low-pass cutoff at `DBAS + DWID`. Lowering it removes more high-frequency repeat energy.
- **DBAS Q** and **DWID Q** are optional resonance controls, defaulting to zero. They are intentionally hidden from the normal faceplate to preserve the existing layout.

Use the ordinary DBAS/DWID knobs normally. To reach their optional Q settings, **right-click DBAS or DWID** on either P1 or P2 EFFX DLY page. The compact panel controls the matching side’s `DBAS Q` and `DWID Q`; no extra main-page knobs are added.

The four host-automatable/state parameters are:

- P1: `dly_dbas_q`, `dly_dwid_q`
- P2: `p2_dly_dbas_q`, `p2_dly_dwid_q`

For `mnm` and `new`, `DBAS=0`, `DWID=127`, and both Q values at `0` is an exact feedback-filter bypass. Thus the untouched native MNM tap/write path remains unchanged. The retained `old` branch keeps its established BASE/WIDTH topology and now receives the optional Q values explicitly; its Q-zero route remains the retained response.

At high Q and feedback the new Korg bridge has a finite-only soft safety ceiling. It is inactive at ordinary levels and exists only to keep a resonant feedback line from retaining NaN/Inf or runaway values.

## Implementation outline

- `Source/dsp/mnm/MnmDelayFeedbackFilter.hpp` adds the shared stereo Korg-35 feedback bridge used by MNM and NEW.
- `Source/dsp/mnm/MnmTrackDelayNew.hpp` adds the independent 16-frame `TrackDelayNewCore`.
- `Source/dsp/mnm/MnmDelay.hpp` routes the feedback bridge through the retained MNM delay recurrence.
- `Source/dsp/TrackDelay.inl` dispatches `new` and makes OLD’s filtered tap/Q route explicit.
- `Source/NovaDSP.h`, `NovaData.h`, and the processor state path provide P1/P2 Q parameters, independent snapshots, migration, lifecycle/reset handling, and the appended DLY-only mode.
- `Source/PluginEditor.cpp` provides the right-click Q panel and the DBAS/DWID explanations.

The shared implementation and standalone regression sources are mirrored in the Synth and FX directories. Their product identities, CMake target metadata, and Projucer settings remain product-specific.

## Validation performed here

For both Synth and FX, the JUCE-free `DelayFeedbackDspTests` covers:

1. exact MNM neutral-bypass behavior;
2. DBAS low-repeat attenuation and DWID high-repeat attenuation;
3. audible MNM and OLD feedback-loop changes;
4. state continuity across redundant control snapshots;
5. finite NEW behavior at maximum Q / feedback;
6. isolation between MNM and NEW delay state;
7. NEW’s independent frame-scheduled response; and
8. appended mode/schema compatibility.

`ModeRollbackTests`, existing `MnmCoreTests`, and the updated static verifier were also run for both products. The root verifier checks that only DLY exposes `mnm|old|new`, schema 23 migration is present, the new feedback sources and UI wiring exist, and historical withdrawn non-delay branches remain absent.

This workspace does not contain a usable JUCE module checkout/CMake plugin environment. A full JUCE build, editor interaction test, and DAW runtime test must still be run in a configured JUCE environment before distributing a binary.
