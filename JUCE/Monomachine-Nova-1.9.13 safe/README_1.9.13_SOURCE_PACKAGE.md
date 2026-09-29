# Monomachine Nova 1.9.13 source package — physical-filter sound/control correction

This is a new **source-only** follow-up package. It does **not** overwrite or
reissue either `Monomachine-Nova-1.9.11.zip` or the separately retained
`Monomachine-Nova-1.9.12.zip`.

## What changed in plain language

The independent route introduced in 1.9.12 remains the route contract:

| DSP FILT | MODE L/H | Renderer |
|---|---|---|
| `old` | `NATIVE` / `NATIVE` | retained OLD native filter |
| `mnm` | `NATIVE` / `NATIVE` | retained MNM native filter |
| `old` or `mnm` | either MODE side is K35/MOOG/ladder/R | independent physical-filter renderer |

For the third row, the selected physical family no longer inherits the MNM
carrier's generic post-filter `softClip()` law. Its resonance is therefore not
silently turned into a second, non-user-controlled saturation stage. `FILT
SAT=0` remains an exact bypass; model-specific nonlinearities that belong to a
selected analogue model remain model-specific rather than a common hidden
clipper.

## Low BASE / live-control correction

The selected physical adapters now smooth cutoff and resonance targets for
**10 ms at the actual sample rate**: 441 samples at 44.1 kHz, 480 at 48 kHz,
and 960 at 96 kHz. This replaces the former fixed 16-sample transition.

K35 and ladder adapters also now compare a host snapshot to the prior
**target**, not the currently moving coefficient. Repeated unchanged snapshots
therefore cannot restart a live smoothing ramp every filter-envelope tick.
No sample-rate callback reset is added.

The retained native `old` and `mnm` NATIVE/NATIVE paths are intentionally not
retuned by this correction.

## Build/cache note for Windows

Use the supplied external builder with **Builds outside ZIP** and **sccache**
enabled. A 1.9.13 source tree deliberately gets its own raw MSBuild directory,
for example:

```text
E:\mm\build\Monomachine-Nova-1.9.13\Monomachine_Nova_Synth
E:\mm\build\Monomachine-Nova-1.9.13\Monomachine_Nova_FX
```

Do not point its `Builds` junction at the old 1.9.11 tree: generated VS/JUCE
artifacts can be stale after a version/source change. This does **not** discard
the shared compiler cache: `E:\mm\build\sccache` is reused. The builder hashes
source/build inputs and asks MSBuild for a correct rebuild when they differ;
sccache can still restore unchanged object compilations.

## Validation boundary

Run `python3 verify_dsp_mode_patch.py` at the archive root. Source-only
regressions cover the independent route, production `TrackFILT.inl`, no shared
MNM output clipping for a selected physical core, repeated snapshots, and the
10 ms control ramp. No JUCE module checkout, VST3 build, DAW load, or listening
claim is made in this workspace. See
[`VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md).
