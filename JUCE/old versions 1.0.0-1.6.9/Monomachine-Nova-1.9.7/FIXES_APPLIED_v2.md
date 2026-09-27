# Monomachine Nova 1.6.2 - Real Filter + Routing + FX Unity Fix

## Integrated real filter
- Source: decompiled data/mnm_filter_full_dump_juce_real/src/MnmFilter.h + mnm_filter_tables.h
- Created MnmRealFilter.hpp with RealFilterCore:
  - kRealSVF_x0_F528BD = -0.08469456 ($F528BD), kRealSVF_x1_4A4DF0 = 0.58050346 ($4A4DF0)
  - knobToCutoffHz exponential, cutoffToG 2*sin(pi*fc/fs), qToDamping 2.0+x0 - Qn*1.9
  - AD env per 16 samples kEnvAttackRate/kEnvDecayRate, BOFS/WOFS: BASE' = BASE+env*BOFS, WDTH' = WDTH+env*WOFS
  - Bypass when BASE~0 WIDTH~127 Q~0 for unity THRU (raw lacks Q and env)
- MnmKernel.hpp rewritten: FilterCore now real SVF HP->LP cascade lo1/bp1/lo2/bp2, FilterCoreLegacy kept as old
- Added MnmFilterTablesReal.hpp (258 coeffs P:$144AC7) and MnmFilterRealDump.hpp for reference

## Deleted previous mnm-named filter, renamed old to old
- Old biquad kept as FilterCoreLegacy, used when mode=old
- raw mode truthful recurrence lacks Q and env modulation, returns unity when bypass

## Spare filter modes
- DspModes expanded to 8: mnm|old|cascade|dual|raw|dist2|fm2|bbox2
- cascade = SvfCascadeFilter literal $F528BD/$4A4DF0 twice per block
- dual = DualSectionFilter HP(BASE) then LP(BASE+WIDTH) independent Q
- raw = TruthfulRawFilter fixed-point SVF
- dist2 = DistortionSaturationBlock2 (HardClip_SRSM/Overdrive_Soft/Fold + SRR) from 2 try/.../DistortionSaturationBlock.h
- fm2 = FMPlusBlock2 (DYN/STAT/PAR with ratio table) from FMPlusBlocks.h
- bbox2 = Bbox2Core Nova_2KnobFade repeat-sample knob mode from DproBboxBlock.h
- Helpers IsDistOnly/IsSyntOnly/IsFilterOnly, provenance strings explain real filter and alt blocks

## Routing fix
- Verified canonical: synth -> dist -> srr -> filt eq -> env -> dsndEQ after filter
- Sources: mnm-routing-100/README_ROUTING_100_VERIFIED.md, mnm-routing-exact, mnm-routing-verified/ROUTING_CONFIRMED.md, synth→dist→→srr→filt eq→env→dsndEQ после фильтра.txt
- TrackChain::process rewritten: DIST first (Saturator / Dist2Core / bipolar), then SRR hold, then FILT (mnm/cascade/dual/raw/dist2/bbox2/old), then EQ (peak after filter), then ENV (amp[i]), then DSND delay (mnmDelay or old)
- Previously SRR->DIST and amp before filter fixed

## DSP MOD button extra m letters
- updateDspModeLabel previously appended " m"/"o" per DspSectionCount -> "DSP MODE m m m m m m"
- Fixed to just "DSP MODE", tooltip shows all modes with explanations
- showDspModes now lists all 8 modes per section with label + provenanceForMode, uses index*100+mode for unique IDs, width 520

## Alternative blocks with unique names (add digit)
- MnmDist2.hpp: DistortionSaturationBlock2 + Dist2Core wrapper, processBlock16, HardClip_SRSM saturateSRSM Q23 sat24
- MnmFm2.hpp: FMPlusBlock2 with Kind Dyn/Stat/Par, Params pitch/mod/shape/decay/portamento/lfo/fmAmt/fmShape, tick with env
- MnmBbox2.hpp: Bbox2Core Mode Nova_2KnobFade / RepeatSample, repeat knob controls hold length 1+repeat*4, LP based on base
- MachineEngine now handles syntMode fm2 for FM machines (8,9,10) and bbox2 for BBOX (7) with repeat-sample
- TrackChain filterMode dist2 uses Dist2Core as filter, bbox2 uses Bbox2Core repeat

## FX dry volume bug
- INP quadratic law: gain = (INP/64)^2 => 32=-12dB 64=0dB 127~+12dB, per P:$14768B mpy x0,x0,a + asl #$2 (x4 in 56-bit) + sat24
- Filter bypass now returns x for unity THRU, previously attenuated
- Chorus makeup: wl*1.5 to make MIX 127 loudness = 64 previously, dry 0 = 0dB difference when FX enabled dry
- Reverb makeup 3x (wetLevel 1/3), phaser/flanger/ring kept unity, compressor dry unity when mix=0
- TrackChain volumeReference 64 for FX, 127 for synth, chain VOL default 64 unity for FX
- PluginProcessor snapshot collapses inappropriate modes: filter-only only for FILT, dist2 only for DIST/FILT, fm2/bbox2 only for SYNT/FILT, ensures menu shows all but DSP stable

## Files changed (both Synth and FX)
- Source/models/DspModes.hpp: 8 modes, provenance, IsDistOnly/IsSyntOnly, Label and ProvenanceForMode helpers
- Source/dsp/mnm/MnmKernel.hpp: real FilterCore
- Source/dsp/mnm/MnmRealFilter.hpp: new real filter core
- Source/dsp/mnm/MnmFilterTablesReal.hpp: copy of mnm_filter_tables.h (258 coeffs)
- Source/dsp/mnm/MnmFilterRealDump.hpp: copy of MnmFilter.h dump
- Source/dsp/mnm/MnmDist2.hpp, MnmFm2.hpp, MnmBbox2.hpp: alt blocks
- Source/NovaDSP.h: routing fix, dist2/fm2/bbox2 handling, INP quad + makeup, filter bypass
- Source/PluginEditor.cpp: DSP MOD button fix, show all modes with explanations
- Source/PluginProcessor.cpp: collapse logic for new modes, FX unity

## Tar
- Produced /home/user/Monomachine-Nova-1.6.2.tar without JUCE SDK, no compilation tested per user request
