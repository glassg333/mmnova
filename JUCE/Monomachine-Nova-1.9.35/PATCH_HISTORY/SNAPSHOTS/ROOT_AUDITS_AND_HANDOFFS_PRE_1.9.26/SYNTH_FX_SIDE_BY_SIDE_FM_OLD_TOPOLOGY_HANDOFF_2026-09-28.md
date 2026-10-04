# Synth / FX side-by-side FM OLD-topology handoff — 2026-09-28

> Исторический handoff до FM MODE cleanup. Он сохраняет source evidence, но не
> задаёт текущий six-slot MODE contract 1.9.26. Упомянутый ниже
> `verify_dsp_mode_patch.py` сохранён как historical snapshot в
> `PATCH_HISTORY/SNAPSHOTS/RETIRED_VERIFY_DSP_MODE_PATCH/`.

## Delivery intent

This handoff replaces the rejected `FM-LEVEL-OLD-FIX` interpretation.  The
appended `old fix` mode means OLD where an OLD renderer exists: it is **not** an
MNM-FIX renderer hidden behind an OLD label.

Both product trees preserve the serialized mode contract:

| raw SYNT ID | label | compatibility status |
| ---: | --- | --- |
| 0 | `mnm` | retained |
| 1 | `old` | retained |
| 2 | `new` | retained |
| 3 | `mnm fix` | retained appended FIX mode |
| 4 | `new fix` | retained serialized ID; not moved |
| 5 | `old fix` | newly appended in schema 31 |

## Exact source correspondence

The following shared patch paths are byte-identical between
`Monomachine_Nova_Synth` and `Monomachine_Nova_FX` (15 paths):

```text
Source/NovaDSP.h
Source/NovaData.h
Source/models/DspModes.hpp
Source/PluginEditor.cpp
Source/PluginProcessor.cpp
Source/PluginProcessor.h
Source/dsp/fm_fix/FmFixTables.hpp
Source/dsp/fm_fix/MnmFmFix.hpp
Source/dsp/fm_fix/OldFmFix.hpp
Source/dsp/fm_new/FmNewLevelBridge.hpp
Source/dsp/fm_new_fix/FmExactNewFix.hpp
tests/FmFixModeTests.cpp
tests/ModeRollbackTests.cpp
tests/RollbackStateTests.cpp
verify_dsp_mode_patch.py
```

The two `CMakeLists.txt` files deliberately retain normal product-name,
plug-in-ID, target, and Synth-versus-FX differences. Both register
`FmFixModeTests`.

## Route-by-route handoff

| concern | Synth (`Monomachine_Nova_Synth`) | FX (`Monomachine_Nova_FX`) | effect |
| --- | --- | --- | --- |
| NEW / NEW FIX output level | `Source/NovaDSP.h` applies `fm_new::kOutputBridgeGain` after each raw NEW route | same file and code | only `new` and `new fix` receive exactly +10.0 dB / ×3.1622776601683795 |
| OLD FIX PAR (m9) | `Source/dsp/fm_fix/OldFmFix.hpp`, `OldFixParallelCore` | byte-identical counterpart | copied OLD PAR topology/state, with measured FIX FREQ/TUNE control laws |
| OLD FIX DYN (m10) | `Source/dsp/fm_fix/OldFmFix.hpp`, `OldFixDynamicCore` | byte-identical counterpart | copied OLD DYN topology/state, with measured FIX 1FRQ/2FRQ/TUNE control laws |
| OLD FIX static (m8) | `oldFixStatAlias` has independent state | same | no historical OLD static core exists in this worktree; retained m8 `old` itself already aliases MNM, so this is a separate stateful alias only for that documented historical case |
| retained OLD m9/m10 | active retained `par` / `dyn` cores | same | untouched source headers and separate audio state; OLD FIX never renders through these retained objects |
| FIX UI/readouts | `DspModes.hpp` + `PluginEditor.cpp` | same | measured labels / explicit factory values only for raw IDs 3/4/5 on m8/m9/m10; retained `mnm`/`old`/`new` formatting stays original |
| serialization | `dspModeOldFix = 5`, schema 31 | same | keeps existing `new fix=4` project state intact |

`NovaDSP.h` separately resets, notes, pitch-bends, and renders the OLD FIX m8,
m9, and m10 state objects. The OLD FIX m9/m10 implementation does not reuse
an MNM-FIX audio renderer or mutate the retained OLD m9/m10 objects.

## Verified evidence

For both product trees:

1. `verify_dsp_mode_patch.py` passed after the OLD-topology correction. It
   hashes retained MNM/NEW/legacy sources, rejects the former `mnmFmOldFix`
   route, checks the new OLD m9/m10 core declarations, and checks the separate
   OLD FIX lifecycle/render paths.
2. All 16 JUCE-free C++17 targets compiled and passed, including the enhanced
   `FmFixModeTests` that compares OLD FIX PAR/DYN output against their retained
   OLD topologies under the same raw controls.
3. A syntax-only `NovaDSP.h` compile passed for both products with a minimal
   local JUCE API stub. This is a C++ route-integration check only.
4. Each `SOURCE_BUILD.json` manifest contains and rehashes 182 product files:
   - Synth manifest SHA-256: `26ac49e32f70eb89f7470be89db7b07d2694ceb26b0512ab71d24254e5a6239f`
   - FX manifest SHA-256: `a05ed6d709b655e65c5327248c6041c277e827e4c4711de69790f3d216178394`

See `PATCH_HISTORY/FIXES_APPLIED_1.9.21.md#validation-1-9-17-fm-legacy-isolation-md` for the full target list,
retained-source hashes, and limitations.

## Deliberate validation boundary

This package does **not** claim a full JUCE build, VST3 build, DAW state-load,
or listening test. It also does not invent an unavailable historical OLD m8
static renderer: the retained source explicitly records that its m8 OLD route
was already an MNM alias, and no named archive source was present in this
worktree to recover a separate static implementation.
