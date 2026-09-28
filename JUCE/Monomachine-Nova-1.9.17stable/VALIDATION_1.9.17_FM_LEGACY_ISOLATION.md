# 1.9.17 FM+ legacy-isolation repair validation — 2026-09-28

## Goal

Repair the FM+ FIX UI/readout path without changing retained `mnm`, `old`, or
`new` default knobs, source core, state, or sound route. This document is an
evidence record for the accompanying source package; it is not a claim of a
full JUCE/VST3/DAW/listening validation.

## Retained source-byte evidence

The following source files are restored from
`Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` in **both** Synth and FX:

| retained route | file | SHA-256 |
| --- | --- | --- |
| `mnm` | `Source/dsp/mnm/MnmFm.hpp` | `2addde732ce2d1550018e1450ebd370cd5a3c0290dfb9604e3c3c80cb859a23d` |
| `new` wrapper | `Source/dsp/fm_new/FmExactNew.hpp` | `9271249e7433c85961d32e28e4b4ee6b457dc1e8d50d0268d8246dcc8adafa3b` |
| `new` raw DSP | `Source/dsp/fm_new/FmExactDsp.hpp` | `64e2adbbd2ac0f3d6d859514ea98599de24757f92ca5531e150fa051d2cab0d2` |

The static verifier hashes these retained files, as well as the pre-existing
legacy FM source headers, in both product trees.

## Default/raw profile isolation

The global descriptor defaults are restored to the retained baseline:

```text
m8 STAT: {16,64,0,0,32,64,64,64}
m9 PAR:  {16,64,32,64,48,64,64,64}
m10 DYN: {16,0,64,0,32,80,30,64}
```

The previously recovered FIX profiles remain in
`Source/dsp/fm_fix/FmFixTables.hpp` only:

```text
m8 STAT: {60,64,80,30,80,64,98,64}
m9 PAR:  {60,64,80,64,102,80,98,64}
m10 DYN: {64,64,64,64,74,80,30,64}
```

They are never APVTS/machine defaults, migration writes, or mode-switch writes.
The only write path is the intentionally named UI command
`LOAD FIX FACTORY RAW VALUES (explicit)`, exposed only while `mnm fix` or
`new fix` is selected for m8/m9/m10.

## Separate core/state topology

| SYNT choice | core/state object | retained route shared? |
| --- | --- | --- |
| `mnm` | `monomachine::mnm::FmCore mnmFm` | no FIX API/table in retained source |
| `mnm fix` | `monomachine::fm_fix::MnmFixCore mnmFmFix` | no |
| `new` | `monomachine::fm_new::FmExactCore fmNew` | no FIX API/table in retained source |
| `new fix` | `monomachine::fm_new::FmExactFixCore fmNewFix` | no; raw copy is in `fmnewfix` namespace |

`NovaDSP.h` resets, notes, bends, and renders each object separately. Selecting
a FIX mode therefore cannot leave a control-law/table pointer or envelope state
inside a retained core.

## UI/readout isolation

`DspModes.hpp` gates `dspSyntModeUsesMeasuredFix()` to raw SYNT IDs 3 and 4
only. `PluginEditor.cpp` checks that selected machine-local mode before
formatting measured STAT/PAR/DYN/TUNE output. Retained raw IDs 0/1/2 (`mnm`,
`old`, `new`) retain their original `getFmListedRatio(raw/4)`, `v/16`,
`pow(2,(v-32)/24)`, and `raw-64` display paths; they do not fall through to a
new generic raw-`0..127` display.

## Completed checks

For **both Synth and FX**:

1. `verify_dsp_mode_patch.py` passed, including retained source-hash checks,
   core/state separation checks, FIX-only UI gating, original retained
   formatter ordering, and explicit-profile checks.
2. The literal original `PluginEditor.cpp` FM formatter fragments were compared
   against `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` and are present in
   both products behind the FIX gate.
3. All 16 JUCE-free C++17 CMake targets compiled and passed:
   `NovaChorusTests`, `MnmCoreTests`, `DelayFeedbackDspTests`,
   `DistVariantTests`, `TrackDistVariantIntegrationTests`,
   `MnmRealFilterTests`, `HybridDspTests`, `FilterRouteTests`,
   `TrackFilterRouteIntegrationTests`, `Import2FiltersTests`,
   `FilterExtrasDspTests`, `ModEnvMatrixTests`, `ArpWindowTests`,
   `FmNewModeTests`, `FmFixModeTests`, and `ModeRollbackTests`.
4. Product-equivalent changed source/test/verifier files were compared
   byte-for-byte between Synth and FX.
5. Both `SOURCE_BUILD.json` manifests were regenerated with 180 files and
   independently rehashed successfully.

The C++17 command emitted pre-existing Chorus unused-variable warnings; no
validation command failed.

## Not completed / deliberately not claimed

- No full JUCE/editor/VST3 build: a compatible JUCE checkout is not present.
- No host/DAW state load, automated plug-in render, or subjective listening
  comparison.
- No universal hardware-identical claim for the measured FIX candidate,
  particularly the approximate MNM DYN topology.
