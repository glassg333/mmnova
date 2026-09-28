# FM+ retained readout routes — exact baseline reconciliation (2026-09-28)

This is the route-by-route companion to `FM_FIX_UI_VALUE_MAP_2026-09-28.md`.
It records the original FM+ display behavior rather than treating a raw
`0..127` fallback as an acceptable substitute.

## Evidence source and scope

The following source was copied from the preserved
`Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` baseline:

```text
Monomachine_Nova_Synth/Source/PluginEditor.cpp
Monomachine_Nova_FX/Source/PluginEditor.cpp
```

The candidate's Synth and FX `PluginEditor.cpp` files are identical. The final
source package restores these retained branches verbatim in both products and
places only the `measuredFix` branches ahead of them.

## Original retained branch — literal source extract

The original `displayedValue()` route for FM+ was:

```cpp
if(machine==10&&knob==0)return juce::String(v/16,1);
if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);
if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))
    return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(raw/4)),2);
if(machine==2&&knob==3)return juce::String(raw-64);
if(label=="TUNE"&&machine!=17)return juce::String(raw-64);
```

The repaired source keeps the first three formula/list lines unchanged behind
`!measuredFix`. The following retained paths are therefore exact comparisons:

| machine | SYNT `mnm` / `old` / `new` | original displayed route |
| --- | --- | --- |
| m8 FM+ STAT | all retained values | `getFmListedRatio(raw / 4)`, two decimals for 1FRQ/2FRQ/3FRQ |
| m9 FM+ PAR | all retained values | `getFmListedRatio(raw / 4)`, two decimals for 1FRQ/2FRQ/3FRQ |
| m10 FM+ DYN 1FRQ | all retained values | `v / 16`, one decimal |
| m10 FM+ DYN 2FRQ | all retained values | `pow(2, (v - 32) / 24)`, two decimals |
| m8/m9/m10 TUNE | all retained values | `raw - 64` |

The other pre-existing generic/bipolar branches remain after these lines,
including m10 knob 1 (`raw - 64`).

## FIX-only insertion

Only raw SYNT `3` (`mnm fix`), `4` (`new fix`), and `5` (`old fix`) satisfy
`dspSyntModeUsesMeasuredFix()`. Ahead of the retained extract, they receive:

- m8/m9 STAT/PAR labels from
  `fm_fix::statParRatioLabelForRaw(raw)`;
- m10 DYN measured `K/64` and `(K/64)^2` text;
- m8/m9/m10 measured TUNE text using the ±2-semitone bridge.

`old fix` is schema-31 separate OLD m9/m10 topology/state with measured
controls; it does not change or share state with retained `old`. m8 has no
archived OLD static renderer, so retained m8 `old` and its separate OLD FIX
alias use the documented MNM implementation only for that historical case. No
`mnm`, `old`, or `new` route reaches the measured branches.

## Side-by-side source locations

| behavior | Synth | FX |
| --- | --- | --- |
| final faceplate condition | `Monomachine_Nova_Synth/Source/PluginEditor.cpp`, `MachinePanel::displayedValue()` | `Monomachine_Nova_FX/Source/PluginEditor.cpp`, same function/text |
| FIX format/control table | `Monomachine_Nova_Synth/Source/dsp/fm_fix/FmFixTables.hpp` | `Monomachine_Nova_FX/Source/dsp/fm_fix/FmFixTables.hpp` |
| retained MNM core | `Monomachine_Nova_Synth/Source/dsp/mnm/MnmFm.hpp` | `Monomachine_Nova_FX/Source/dsp/mnm/MnmFm.hpp` |
| retained NEW wrapper/DSP | `.../Source/dsp/fm_new/FmExactNew.hpp`, `FmExactDsp.hpp` | same relative paths |
| separate FIX cores | `.../Source/dsp/fm_fix/MnmFmFix.hpp`, `dsp/fm_new_fix/` | same relative paths |

All product-equivalent files above are byte-identical between Synth and FX.

## Delegated source extracts

The separately delivered
`Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28.zip`
remains the browseable exact-source handoff for the original FM `mnm`, `old`,
and `new` routes plus the labeled MNM/OLD DIST and FILT material, side by side
for Synth and FX. It intentionally predates this repair and must remain paired
with this final full source archive rather than being regenerated from a
post-FIX worktree.
