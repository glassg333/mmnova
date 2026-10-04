#!/usr/bin/env python3
"""Static regression guard for the corrected 1.9.21 source delivery.

Run from either product directory. It validates retained IDs/routes, schema-34
migration, the independent Pack-7/Fix-5 import, its native AMP/VOL²/pan bridge,
the restored Track Delay defaults, and the source-only UI/DSP corrections.
"""
from __future__ import annotations

from hashlib import sha256
from pathlib import Path
import sys

root = Path(__file__).resolve().parent
failures = 0


def check(condition: bool, description: str) -> None:
    global failures
    print(f"{'OK  ' if condition else 'FAIL'} {description}")
    if not condition:
        failures += 1


def source(relative: str) -> str:
    path = root / relative
    if not path.is_file():
        check(False, f"present: {relative}")
        return ""
    return path.read_text(encoding="utf-8", errors="replace")


def digest(relative: str) -> str:
    path = root / relative
    if not path.is_file():
        return ""
    return sha256(path.read_bytes()).hexdigest()


modes = source("Source/models/DspModes.hpp")
processor = source("Source/PluginProcessor.cpp")
editor = source("Source/PluginEditor.cpp")
dsp = source("Source/NovaDSP.h")
track_volpan = source("Source/dsp/TrackVOLPAN.inl")
profiles = source("Source/models/FmExperimentProfiles.hpp")
old_fix = source("Source/dsp/fm_fix/OldFmFix.hpp")
bridge = source("Source/dsp/fm_new/FmNewLevelBridge.hpp")
try4 = source("Source/dsp/fm_try4_voice/Try4VoiceFm.hpp")
try4_dsp = source("Source/dsp/fm_try4_voice/Try4VoiceFmDsp.hpp")
try4_amp = source("Source/dsp/fm_try4_voice/Try4VoiceAmpEnv.hpp")
try4_page = source("Source/dsp/fm_try4_voice/Try4VoicePageMap.hpp")
frq = source("Source/dsp/fm_mnm_frq_env_fix/Import4Fm.hpp")
frq_dsp = source("Source/dsp/fm_mnm_frq_env_fix/Import4FmDsp.hpp")
track_env = source("Source/dsp/fm_mnm_frq_env_fix/TrackEnvExact.hpp")
fix5 = source("Source/dsp/fm_fix5/Fix5Fm.hpp")
fix5_amp = source("Source/dsp/fm_fix5/Fix5AmpEnv.hpp")
fix5_dsp = source("Source/dsp/fm_fix5/Fix5FmDsp.hpp")
fix5_vectors = source("tests/fixtures/fix5_plugin_level_vectors.txt")
fix5_vector_test = source("tests/Fix5PluginLevelTests.cpp")
fix5_host_test = source("tests/Fix5HostIntegrationTests.cpp")
experiment_tests = source("tests/FmExperimentsTests.cpp")
fix_tests = source("tests/FmFixModeTests.cpp")
cmake = source("CMakeLists.txt")
jucers = list(root.glob("*.jucer"))
jucer = jucers[0].read_text(encoding="utf-8", errors="replace") if len(jucers) == 1 else ""

# Retained source assets must remain byte-identical to the Package-4 delivery.
# Integration/UI/migration files intentionally move and are therefore excluded.
retained_hashes = {
    "Source/dsp/mnm/MnmFm.hpp": "2addde732ce2d1550018e1450ebd370cd5a3c0290dfb9604e3c3c80cb859a23d",
    "Source/dsp/fm_new/FmExactNew.hpp": "9271249e7433c85961d32e28e4b4ee6b457dc1e8d50d0268d8246dcc8adafa3b",
    "Source/dsp/fm_new/FmExactDsp.hpp": "64e2adbbd2ac0f3d6d859514ea98599de24757f92ca5531e150fa051d2cab0d2",
    "Source/dsp/fm_new/FmExactPar.hpp": "a6d61aa21b24d52d48a4f4885894db061dd1a56506135b6aea8d8401dd7d2f87",
    "Source/dsp/fm_new/FmExactDyn.hpp": "89e1f4f4c7b1d19f8cd938f9f0cc98a6ba82fbc4f1778a2efb3494402f1ed185",
    "Source/dsp/fm_new/FmExactStat.hpp": "d6b5d0ac16da4ea51d37e5b9ab741ef9fcb4577084f9f48d1db0cc445317a067",
    "Source/dsp/fm_new_fix/FmExactNewFix.hpp": "6f75f198216669ebee0b59cee3a0fd0435b49fe9be1db460c5a4ed49b46c741b",
    "Source/dsp/fm_fix/MnmFmFix.hpp": "91bc04e34e48318dc4717586d8f5cd6d002ac2ed265554f50e0c039dddd6a277",
    "Source/dsp/fm_fix/OldFmFix.hpp": "85e6afba7ebb9fee77218471734482752bab562f9513f08d5b477a83b3f67308",
    "Source/dsp/fm_fix/FmFixTables.hpp": "a904adf94c1a39d23ae53c9d24fa1650a23383433fab9358b56e1c99ff4ce7b9",
}
for relative, expected in retained_hashes.items():
    check(digest(relative) == expected, f"retained core byte fingerprint unchanged: {relative}")

check(all(token in modes for token in (
    "dspModeMnm = 0", "dspModeOld = 1", "dspModeNew = 2",
    "dspModeMnmFix = 3", "dspModeNewFix = 4", "dspModeOldFix = 5",
    "dspModeMnmFrqEnvFix = 6", "dspModeTry4 = 7",
    "dspModeMnmFix5Full = 8", "dspModeCount = 9",
    "kDspModeSchemaVersion = 34",
)), "retained IDs 0..7 stay fixed; Fix-5 appends ID 8/schema 34")
check('return "mnm FREQ|old FREQ|new FREQ|mnm fix BPM|new fix BPM|old fix BPM|mnm frq env fix|try4|fix5 BPM pitch+env full";' in modes
      and 'return "mnm|old|new|mnm fix|new fix|old fix|mnm frq env fix|try4|fix5 pitch+env full";' in modes,
      "FM+ STAT selector keeps FREQ/BPM distinction while PAR/DYN retain ordinary labels")
check("schema >= 32 && mode == dspModeMnmFrqEnvFix" in modes
      and "schema >= 33 && mode == dspModeTry4" in modes
      and "schema >= 34 && mode == dspModeMnmFix5Full" in modes
      and "dspSyntModeAllowedForSchema(machineId,value,schema)" in processor,
      "state migration accepts Fix-5 only from schema 34 and only for m8/m9/m10")

# Existing retained OLD FIX checks stay in force.
check("class OldFixStaticCore" in old_fix and "class OldFixParallelCore" in old_fix
      and "class OldFixDynamicCore" in old_fix
      and "#include \"Mnm" not in old_fix and "MnmFixCore" not in old_fix,
      "OLD FIX remains dedicated OLD STATIC/PAR/DYN code with no MNM dependency")
old_fix_dispatch = ""
if "if(oldFix){" in dsp and "// Baseline MNM" in dsp:
    old_fix_dispatch = dsp.split("if(oldFix){", 1)[1].split("// Baseline MNM", 1)[0]
check("oldFixStat.processStereo" in old_fix_dispatch
      and "oldFixPar.processStereo" in old_fix_dispatch
      and "oldFixDyn.processStereo" in old_fix_dispatch
      and "MnmFixKind" not in old_fix_dispatch,
      "OLD FIX dispatch remains separate from MNM")

# The +10 dB bridge belongs to retained NEW / NEW FIX only. Neither appended
# Package-4 nor Fix-5 dispatch may borrow it.
experimental_dispatch = ""
if "if(fix5Full){" in dsp and "if(newExact){" in dsp:
    experimental_dispatch = dsp.split("if(fix5Full){", 1)[1].split("if(newExact){", 1)[0]
check("kOutputBridgeGainDb = 10.0f" in bridge
      and dsp.count("monomachine::fm_new::kOutputBridgeGain") == 2
      and "kOutputBridgeGain" not in experimental_dispatch,
      "+10 dB bridge remains exclusive to retained NEW and NEW FIX")

# TRY4 has a direct Package-5 FM/AMP frame import, separate from m6. The
# Package-8 full tail is a verified routing oracle, not an audio-thread interpreter.
check("namespace fm_try4_voice" in try4 and "namespace try4voicefm" in try4_dsp
      and "namespace fm_mnm_frq_env_fix" in frq and "namespace mnmfrqenvfm" in frq_dsp,
      "TRY4 Package-5 and m6 Package-4 retain different wrapper/core namespaces")
check("VoicePageMap" in try4_page and "kMachine = 0x42c" in try4_page
      and "kTrigger = 0x428" in try4_page and "kPitch = 0x429" in try4_page
      and "voicePageState.setMachine" in try4 and "voicePageState.setAmp" in try4,
      "TRY4 uses the imported Package-5 FM/AMP page map and private page state")
check("inline int32_t MnmPanTables::s_sin[8192];" in try4_amp
      and "inline int32_t MnmPanTables::s_cos[8192];" in try4_amp
      and "inline bool MnmPanTables::s_built = false;" in try4_amp,
      "TRY4 pan-table storage has the local C++17 inline ODR repair for VST3 linking")
check("dsp/mnm/MnmFm.hpp" not in try4 and "MnmFmFix.hpp" not in try4
      and "OldFmFix.hpp" not in try4 and "fmTry4.processBlockStereo" in dsp
      and "fmTry4.setParameters" in dsp and "fmMnmFrqEnvFix" in dsp,
      "TRY4 dispatch uses only its isolated Package-5 core, not retained or m6 FM code")
check("setTry4AmpParams" in dsp and "try4TrackActive" in dsp
      and "usesTry4NativeAmpPath" in processor and "try4Active" in dsp,
      "TRY4 has its own native FM/AMP VOL²/pan/release lifecycle for all FM+ machines")
check("class EnvExactCore" in track_env and "class ContinuousEnvExactCore" in track_env
      and "frameCounter_" in track_env and "current_ + (frameTarget_ - current_)" in track_env
      and "TrackEnvExact.hpp" in dsp and "MnmFrqEnvTrackEnvelope" in dsp
      and "mnmFrqEnvEnvelope" in processor and "usesMnmFrqEnvFixTrackEnvelope" in processor,
      "mode 6 alone owns the Package-4 track AMP state and continuous 16-sample de-zipper")

# Fix-5 is a literal separately-named source import. The C++17 inline repair
# is deliberately local to its pan-table header to prevent ODR failures in the
# Synth/FX translation units; it does not touch any retained FM file.
check("namespace fm_fix5" in fix5 and "namespace fix5fm" in fix5_dsp
      and "Fix5AmpEnv.hpp" in fix5 and "Fix5FmPar.hpp" in fix5
      and "dsp/mnm/" not in fix5 and "MnmFmFix.hpp" not in fix5
      and "OldFmFix.hpp" not in fix5,
      "Fix-5 owns an isolated Pack-7 wrapper/namespace with no retained FM include")
check("inline int32_t MnmPanTables::s_sin" in fix5_amp
      and "inline int32_t MnmPanTables::s_cos" in fix5_amp
      and "inline bool MnmPanTables::s_built" in fix5_amp,
      "Fix-5 pan-table storage has the local C++17 inline ODR repair")
check("if(fix5Full){" in dsp and "fmFix5.processBlockStereo" in dsp
      and "fmFix5.setParameters" in dsp and "fmFix5.noteOff" in dsp
      and "setFix5AmpParams" in dsp and "fix5TrackActive" in processor,
      "Fix-5 has explicit m8/m9/m10 native stereo/frame/envelope production dispatch")
check("0x800000u / (24.0f*bpm)" in dsp
      and "fmFix5.setTempoWord(std::max<uint32_t>(1u,tickRecip))" in dsp,
      "Fix-5 AMP HOLD receives OS TickRecip rather than raw UI BPM")
check("setCandidateOwnsAmpVolPan" in dsp
      and "if(candidateOwnsAmpVolPan)return;" in track_volpan
      and "const float a=nativeFrame?1.0f:" in processor,
      "TRY4/Fix-5 native AMP/VOL squared/pan is not double-applied by host VOL/PAN or ADSR")

check("kParVisibleUnityDefaultRaw = {16, 32, 48}" in profiles
      and "parDefaultReadsUnity" in profiles
      and "parDefaultReadsUnity(knob,raw)" in editor,
      "FM+ PAR 1/1/1 remains a UI/default calibration with retained raw DSP laws intact")
check("mnmFrqEnvFixNeedsRawZeroInitialisation" in profiles
      and "mnmFrqEnvFixIsFrequencyKnob" in profiles
      and "initialiseMnmFrqEnvFixRawDefaults" in editor
      and "dspModeMnmFrqEnvFix" in editor,
      "fresh m6 selection/reset remains scoped to its raw-zero FM+ FRQ words")
check("zeroFrqEnvelope&&raw==0)return \"0.00\";" in editor,
      "MNM FRQ ENV FIX LCD continues to present raw-zero frequency as 0.00")

check("Fix5PluginLevelTests" in cmake and "tests/Fix5PluginLevelTests.cpp" in cmake
      and "fix5_plugin_level_vectors.txt" in cmake
      and "checks=9556" not in fix5_vector_test  # runtime evidence, not hard-coded
      and "Fix5Fm.hpp" in fix5_vector_test and "plugin_level_vectors" in fix5_vector_test
      and fix5_vectors.startswith("ALAW") and len(fix5_vectors) > 1000,
      "Pack-7 plugin-level vector regression is included with its immutable fixture")
check("Fix5HostIntegrationTests" in cmake and "tests/Fix5HostIntegrationTests.cpp" in cmake
      and "schema 33" in fix5_host_test and "allSoundOff" in fix5_host_test
      and "VOL squared" in fix5_host_test and "kFix5Machines" in fix5_host_test,
      "CMake registers processor-level Fix-5 migration, lifecycle, routing and gain-path coverage")
check("TRY4 использует отдельный Package-5 AMP/pan/page-frame путь" in experiment_tests
      and "TRY4 хранит отдельную Package-5 FM/AMP-карту" in experiment_tests
      and "Fix-5 and TRY4 must own distinct FmCore types" in experiment_tests
      and "schema 34 appends independent Fix-5" in fix_tests,
      "portable regression coverage includes TRY4/Fix-5 all-machine, isolation and migration assertions")
nova_data = source("Source/NovaData.h")
track_pages = source("Source/models/track_pages.hpp")
delay_stage = source("Source/dsp/TrackDelay.inl")
delay_route = source("Source/dsp/TrackDelayRouting.hpp")
delay_core = source("Source/dsp/mnm/MnmDelay.hpp")
delay_new_core = source("Source/dsp/mnm/MnmTrackDelayNew.hpp")
p2_routing_test = source("tests/P2RoutingTests.cpp")
rollback_state_test = source("tests/RollbackStateTests.cpp")
legacy_filter = source("Source/dsp/monomachine_filter.hpp")
real_filter = source("Source/dsp/mnm/MnmRealFilter.hpp")

# Correction of the rejected Track Delay default-route change. These are the
# retained factory values; no migration is allowed to rewrite a saved P2 route.
check("{64,64,0,64,64,28,0,127}" in nova_data
      and "int chorusIdx=0;" in nova_data
      and "if(fxm[i]->id==15)chorusIdx=static_cast<int>(i);" in nova_data
      and '"p2_machine","P2 FX slot",0,static_cast<float>(fxm.size()-1),static_cast<float>(chorusIdx)' in nova_data
      and '"p2_mix","P2 MIX",0,127,127,1' in nova_data
      and '{"DSND", "Delay Send Level",         "",   64,  false}' in track_pages,
      "factory DSND=64 and original P2 CHORUS/MIX=127 route are retained")

check("trackDelayDefaultRouteV1" not in processor
      and "repairLegacyFactoryP2DelayRoute" not in processor
      and "hasLegacyFactoryP2DelayRoute" not in processor
      and "requireFreshTrackDelayDefaults" not in p2_routing_test
      and "writeLegacyFactoryP2Route" not in rollback_state_test,
      "no Track Delay migration rewrites an already-saved P2 state")

# DSND is a host scale, not a surrogate left/right or ping-pong selector. The
# exact native L/R delay route remains deliberately separate until its vectors
# and host adapter close that path.
check("monoSendGain(params)" in delay_stage
      and "return std::clamp(raw, 0.0f, 127.0f) * (1.0f / 128.0f);" in delay_route
      and "bankSwap" not in delay_core and "bankSwap" not in delay_new_core
      and "DSND / ШКАЛА ЗАДЕРЖКИ" in editor
      and "НЕ НАЗНАЧАЕТ ЛЕВЫЙ/ПРАВЫЙ КАНАЛ" in editor
      and "МОНО-ПОСЫЛ" not in editor,
      "DSND remains a scale; no sign/side/ping-pong route is invented")

# The user-facing filter popup sorts presentation by response folder and name;
# persisted choice indices remain the IDs sent back to DragChoice.
check("const auto addSorted=" in editor
      and "compareIgnoreCase" in editor
      and "entry.second,entry.first" in editor,
      "hybrid filter menu uses deterministic type-then-name presentation without reserialising IDs")

# Envelope UX: all large tab pages hand a held drag across AMP/FIL/MOD; the
# faceplate category selection is updated after dock replacement and cleared
# whenever a non-envelope overlay replaces it.
check(editor.count("ampTab.heldDrag=handoff;filTab.heldDrag=handoff;modTab.heldDrag=handoff;") == 3
      and "void setEnvelopeCategorySelection(int active)" in editor
      and "dismissDock();setEnvelopeCategorySelection(2);" in editor
      and "setEnvelopeCategorySelection(static_cast<int>(wanted)-1);" in editor
      and "showDocked may dismiss the old mini" in editor,
      "AMP/FIL/MOD large-tab drag and selected-state handoff are symmetric")

# Keytracking positions share the same clamped BASE+WDTH LP control in OLD and
# MNM. P2's neutral-insert fast path may not hide a user-disabled HPF/LPF
# tracking switch.
check("&&filterTrackHpf&&filterTrackLpf&&!hasActiveFilterExtras();" in dsp
      and "trackedCutoff(lpKnob)" in real_filter
      and "const float lpControl = std::min(127.0f, effectiveBase + effectiveWidth);" in legacy_filter
      and "trackedFreq(lpControl)" in legacy_filter,
      "HPF/LPF keytrack switches and LP edge use their documented physical positions")

check(len(jucers) == 1 and "version=\"1.9.21\"" in jucer and "VST3" in jucer
      and "VERSION 1.9.21" in cmake,
      "Projucer/CMake project-visible version is 1.9.21 for the Windows VST3 build")

if failures:
    print(f"\nFM_FIX5_STATIC_VERIFY FAIL: {failures} check(s)", file=sys.stderr)
    sys.exit(1)
print("\nFM_FIX5_STATIC_VERIFY PASS")
