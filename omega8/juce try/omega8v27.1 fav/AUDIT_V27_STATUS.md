# v27.1 audit: what is implemented, corrected, and still unverified

**Source revision:** `r27.1-20261004`  
**Scope:** compare the v20–v27 change claims with the current code in `Source/` and rerun focused regression checks. This is a source/DSP audit, not a DAW or hardware certification.

## Corrections made in this pass

| Area | Finding in the code before this pass | Current result |
|---|---|---|
| A/B morph / DK2 | `ENV1/2/3_DK2` was numerically lerped. `0` and `127` are both disabled sentinels in `Env::setParams()`, while `1..126` enables a second decay. A001 has ENV2_DK2 `127`; B045 has `0`. Their interpolation therefore created a real decay and collapsed the held note. There was also a 50% tie mismatch: `Patch::lerp()` selected B at exactly 50%, while `editSlot()` selected A. | DK2 now stays discrete whenever either endpoint is disabled; active times interpolate only when both endpoints are active. `UNK36` is also discrete. `editSlot()` now selects B at 50%, matching `Patch::lerp()`. All three DK2 offsets and the held-note sweep are covered by a temporary headless regression test. |
| MW=BLEND global switch | It was a plain processor bool, separate from the patches, but was not a host/APVTS parameter. Matrix `sync()` was read-only for it; there was no source for A/B to own this flag. | It is now one global APVTS bool, default OFF, saved in the host state (with the old XML attribute retained for compatibility). Matrix sync still only reads/displays it; only the main button or an explicit matrix-cell click writes it. Preset selection and morph do not change it. ModWheel smoothing is atomic for UI/audio-thread reads. |
| MW source matrix vs. MW=BLEND | These are different controls. `MODW_D1/D2` are patch routing destinations; they are not the global `MW=BLEND` switch. | The two mechanisms remain separate. In the **embedded factory** A001/B045 pair used for the sound test, the ModWheel routes are actually nonzero: A001 has `FILT` (amount 74) and `LF2R` (amount 4); B045 has `LF1D` (40) and `LF1R` (105). So those factory routes are not OFF, even though the global MW=BLEND parameter defaults OFF. If the report refers to different/custom A/B patches, inspect those patch bytes separately. When both patch endpoints set a ModWheel destination to OFF, the enum morph keeps it OFF throughout. |
| CONT1/CONT2 source selection | `evalContSrc()` selected TRAK/DYNA/MODW/PRES and then the caller also added CC16/CC17. This contaminated every non-PEDL selection. | Removed the extra addition. The selected source is exclusive; CC16/17 are used only for PEDL/fallback. |
| Accu-Tune side effect | `accuTune()` wrote MASTER_TUNE=64 but also rewrote OCTAVE, dropping its low nibble. That was unrelated and destructive to stored patch data. | Accu-Tune now changes only MASTER_TUNE; all other patch bytes are preserved. |
| Preset UI / GLOBAL | Duplicate A/B ComboBoxes were present; GLOBAL was above ARP. | ComboBoxes and their callbacks/layout were removed. A/B name rows open 128-preset menus; left/right arrows remain. GLOBAL is laid out below the ARP controls. UI source compiles, but visual interaction still needs a plugin run. |

## Checks actually run

- Loaded the embedded bank: 128 patches.
- Verified factory patch bytes: A001 `REVELATION 1-8`, ENV2_DK2=127; B045 `PIGALLE PARTY`, ENV2_DK2=0.
- Held one note while sweeping A001→B045 through all 101 integer morph positions in `Engine`. After the DK2 fix, every 640-sample window remained above the regression floor; minimum RMS was `0.03713810` at 40%. The prior test had RMS `0` through much of 60–90%.
- Rendered B045 directly for two seconds: RMS `0.13539766`, peak `0.29814318`.
- Checked all three DK2 offsets for disabled-sentinel behavior and active-to-active interpolation; checked that a ModWheel destination stays zero when both endpoints are zero.
- Ran GCC 14.2 syntax-only compilation of `PluginProcessor.cpp` (which includes `PluginEditor.h`) against JUCE 8.0.12 with `-Wall -Wextra`; no diagnostics/errors.

## Claims that are present in source but are not certified here

- Direct v20-style morph is now explicit: `morphSm`/`morphSide` state and the old block smoothing/hysteresis implementation have been removed. `Patch::lerp()` handles discrete enums at the 50% boundary. The held-note regression establishes that the PIGALLE dropout is gone in the headless Engine test; RMS is **not** a click detector.
- Filter-state reset-on-real-`FILT_TYPE` change, per-sample pitch smoothing, tuning, octave decode, the envelope mappings, AUX2 card selection, TS-808/WAH controls, LFO/PAN sync, and the other previously listed matrix routes have implementation paths in source. They were not all independently compared against a physical Omega 8 in this pass.
- MASTER_TUNE currently uses 64 as neutral, lower values for deterministic per-voice spread (up to ±20 cents), and higher values for positive cent trim. The source documents this as the manual-based interpretation; physical calibration/A-B against hardware is not available here.

## Still required before calling the release verified

1. Build the Windows x64 VST3 with the project's MSBuild/Projucer setup and confirm the host/About build marker is `r27.1-20261004`.
2. In the target DAW, test A/B selection from the clickable name rows, GLOBAL below ARP, and that morph/preset changes leave global MW=BLEND unchanged. Also test host save/reload and automation of `mwBlendOn`.
3. Audition a held-note A001→B045 morph and B045 at 0%, 50%, 60–90%, and 100% in the VST. The Engine test confirms no silent gap, but this pass does **not** claim that all VST/DAW transition clicks are eliminated.
4. If MW routing is intended to be OFF, verify the actual A/B patch matrix destinations (`MODW_D1/D2`); the embedded A001/B045 factory pair has active routes as listed above.
