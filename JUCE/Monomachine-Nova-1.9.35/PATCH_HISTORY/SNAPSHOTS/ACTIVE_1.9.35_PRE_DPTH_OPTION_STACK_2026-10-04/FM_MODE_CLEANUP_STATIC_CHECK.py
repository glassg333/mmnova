#!/usr/bin/env python3
"""Статический аудит retained FM, Phaser, CHORUS A/B, DFB RAW=63 UNITY/RAW>=64 GUARD и schema-45 LFO state.

Не компилирует проект, не запускает DSP и не выполняет render/DAW-тесты.
Запускать из любого каталога: python FM_MODE_CLEANUP_STATIC_CHECK.py
"""
from pathlib import Path
import hashlib
import json
import sys

ROOT = Path(__file__).resolve().parent
TARGETS = ("Monomachine_Nova_Synth", "Monomachine_Nova_FX")
MIRRORED = (
    "Source/models/DspModes.hpp",
    "Source/PluginProcessor.cpp",
    "Source/PluginProcessor.h",
    "Source/PluginEditor.cpp",
    "Source/NovaDSP.h",
    "Source/dsp/TrackVOLPAN.inl",
    "Source/dsp/AudioAdapter.h",
    "Source/dsp/AudioAdapter.cpp",
    "Source/FxSlotEngine.h",
    "Source/NovaData.h",
    "Source/dsp/TrackDelay.inl",
    "Source/dsp/DelayFeedbackDynamics.hpp",
    "Source/dsp/mnm/MnmDelay.hpp",
    "Source/dsp/mnm/MnmTrackDelayNew.hpp",
    "Source/dsp/monomachine_effects.hpp",
)
MIRRORED_TESTS = (
    "tests/DelayFeedbackDspTests.cpp",
    "tests/FmFixModeTests.cpp",
    "tests/FmNewModeTests.cpp",
    "tests/FmModeRetirementTests.cpp",
    "tests/ModeRollbackTests.cpp",
)
RETIRED_PATHS = (
    "Source/dsp/fm_mnm_frq_env_fix",
    "Source/dsp/fm_try4_voice",
    "Source/dsp/fm_fix5",
    "Source/dsp/monomachine_fm_stat.hpp",
    "Source/models/FmExperimentProfiles.hpp",
    "tests/FmExperimentsTests.cpp",
    "tests/Fix5PluginLevelTests.cpp",
    "tests/Fix5HostIntegrationTests.cpp",
    "tests/fixtures/fix5_plugin_level_vectors.txt",
)
ARCHIVE = ROOT / "PATCH_HISTORY" / "RETIRED_FM_CANDIDATES_1.9.26"
EXPECTED_CHOICES = "mnm frq|old frq|new frq|mnm bpm|new bpm|old bpm"
BANNED_SHIPPING_TOKENS = (
    "fm_mnm_frq_env_fix",
    "fm_try4_voice",
    "fm_fix5",
    "MnmFrqEnvTrackEnvelope",
    "mnmFrqEnvEnvelope",
    "usesMnmFrqEnvFixTrackEnvelope",
    "usesTry4NativeAmpPath",
    "usesFix5FullTrackPath",
    "setTry4AmpParams",
    "setFix5AmpParams",
    "try4TrackActive",
    "fix5TrackActive",
    "candidateOwnsAmpVolPan",
    "FmExperimentProfiles",
    "dspModeMnmFrqEnvFix",
    "dspModeTry4",
    "dspModeMnmFix5Full",
)

errors: list[str] = []


def read(target: str, relative: str) -> str:
    path = ROOT / target / relative
    if not path.is_file():
        errors.append(f"Нет файла: {target}/{relative}")
        return ""
    return path.read_text(encoding="utf-8")


def require(condition: bool, message: str) -> None:
    if not condition:
        errors.append(message)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def contains_cyrillic(value: str) -> bool:
    return any("\u0400" <= char <= "\u052f" for char in value)


for target in TARGETS:
    modes = read(target, "Source/models/DspModes.hpp")
    processor = read(target, "Source/PluginProcessor.cpp")
    editor = read(target, "Source/PluginEditor.cpp")
    dsp = read(target, "Source/NovaDSP.h")
    data = read(target, "Source/NovaData.h")
    cmake = read(target, "CMakeLists.txt")

    require("inline constexpr int dspModeCount = 6;" in modes,
            f"{target}: MODE SYNT должен содержать ровно шесть selectable slots")
    require("inline constexpr int kDspModeSchemaVersion = 45;" in modes,
            f"{target}: ожидается schema 45: DFB RAW=63 UNITY/RAW>=64 GUARD + direct-LFO Matrix state; DLY CORE удалён")
    require(f'return "{EXPECTED_CHOICES}";' in modes,
            f"{target}: строка шести FM labels отличается от согласованной")
    for constant, number in (("dspModeMnm", 0), ("dspModeOld", 1),
                             ("dspModeNew", 2), ("dspModeMnmFix", 3),
                             ("dspModeNewFix", 4), ("dspModeOldFix", 5)):
        require(f"inline constexpr int {constant} = {number};" in modes,
                f"{target}: изменён retained renderer ID {constant}={number}")
    for constant, number in (("kRetiredSyntRawId6", 6),
                             ("kRetiredSyntRawId7", 7),
                             ("kRetiredSyntRawId8", 8)):
        require(f"inline constexpr int {constant} = {number};" in modes,
                f"{target}: потерян neutral legacy raw-ID marker {constant}")

    allowed_start = modes.find("inline bool dspSyntModeAllowedForMachine")
    allowed_end = modes.find("inline const char* dspSectionParameterChoices")
    allowed = modes[allowed_start:allowed_end]
    for retired_text in ("mnm frq env fix", "try4", "fix5 pitch+env full"):
        public_choices = modes[modes.find("inline const char* dspModeChoices"):
                              modes.find("inline int dspModeIndexByName")]
        require(retired_text not in public_choices,
                f"{target}: retired label «{retired_text}» остался в public MODE choices")
    for marker in ("dspModeMnmFrqEnvFix", "dspModeTry4", "dspModeMnmFix5Full"):
        require(marker not in allowed,
                f"{target}: retired {marker} всё ещё попал в legal MODE SYNT path")

    config = read(target, "Source/NovaConfig.h")
    expected_identity = 1 if target == "Monomachine_Nova_Synth" else 0
    expected_product = "Synth" if expected_identity == 1 else "FX"
    require(f"#define NOVA_SYNTH {expected_identity}" in config
            and f"Monomachine Nova {expected_product} source BUILD" in config
            and "FloatingPanel white foreground ink" in config,
            f"{target}: NovaConfig не соответствует JucePlugin target identity/current foreground marker")
    require("const int rawIdx=" in processor
            and "dspSyntModeAllowedForMachine(selectedMachineId,idx)" in processor
            and "idx=monomachine::dspModeMnm;" in processor,
            f"{target}: snapshot не содержит безопасную нормализацию raw MODE SYNT")
    require("dspSyntModeAllowedForSchema(machineId,value,schema)" in processor,
            f"{target}: state migration должна использовать schema allow-list")
    require("parDefaultReadsUnity" in editor,
            f"{target}: m9 PAR unity-display profile был потерян вместе с candidates")
    require("const int nextSynt=" in dsp
            and "syntModeIn<monomachine::dspModeCount" in dsp,
            f"{target}: DSP boundary не защищает от retired raw MODE value")
    require("kArpPageCount,static_cast<float>(part+1),1,{}" in data,
            f"{target}: ARP SONG page default должен явно приводить part+1 к float для MSVC")
    require("FmModeRetirementTests tests/FmModeRetirementTests.cpp" in cmake
            and "Fix5HostIntegrationTests" not in cmake
            and "Fix5PluginLevelTests" not in cmake
            and "FmExperimentsTests" not in cmake,
            f"{target}: CMake не заменил candidate tests на retained-mode test")

    shipping_files = list((ROOT / target / "Source").rglob("*"))
    shipping_files += [ROOT / target / "CMakeLists.txt"]
    shipping_files += list((ROOT / target / "tests").rglob("*"))
    shipping_text = "\n".join(path.read_text(encoding="utf-8", errors="replace")
                              for path in shipping_files if path.is_file())
    for token in BANNED_SHIPPING_TOKENS:
        require(token not in shipping_text,
                f"{target}: снятый candidate token остался в shipping tree: {token}")
    for relative in RETIRED_PATHS:
        require(not (ROOT / target / relative).exists(),
                f"{target}: retired path всё ещё лежит в shipping tree: {relative}")
        require((ARCHIVE / target / relative).exists(),
                f"{target}: отсутствует архивная копия retired path: {relative}")

    adapter_h = read(target, "Source/dsp/AudioAdapter.h")
    adapter_cpp = read(target, "Source/dsp/AudioAdapter.cpp")
    fx_slots = read(target, "Source/FxSlotEngine.h")
    delay_stage = read(target, "Source/dsp/TrackDelay.inl")
    delay_dynamics = read(target, "Source/dsp/DelayFeedbackDynamics.hpp")
    mnm_delay = read(target, "Source/dsp/mnm/MnmDelay.hpp")
    new_delay = read(target, "Source/dsp/mnm/MnmTrackDelayNew.hpp")
    phaser = read(target, "Source/dsp/monomachine_effects.hpp")
    require("const float phaseDifferenceL = 0.5f * (sL - inL_sample);" in phaser
            and "const float phaseDifferenceR = 0.5f * (sR - inR_sample);" in phaser
            and "// outL[i] = (1.0f - m_wetMix) * inL_sample + m_wetMix * sL;" in phaser,
            f"{target}: Phaser correction/nearby old formula is incomplete")

    require('case DspChorus: return "native|core";' in modes
            and "реальный A/B selector NativeChorusCore | ChorusCore" in modes
            and "CHOR native (default): NativeChorusCore через AudioAdapter" in modes
            and "CHOR core (A/B): эталонный ChorusCore" in modes,
            f"{target}: CHOR DSP menu не объявляет Native/Core A/B с Native default")
    require('#include "ChorusCore.h"' in adapter_h
            and "NativeChorusCore nativeCore" in adapter_h
            and "ChorusCore referenceCore" in adapter_h
            and "void setCoreMode(int mode) noexcept;" in adapter_h
            and "referenceCoreSelected" in adapter_h
            and "if(referenceCoreSelected)referenceCore.process16" in adapter_cpp
            and "else nativeCore.process16" in adapter_cpp
            and "reset(coreParameters);" in adapter_cpp,
            f"{target}: AudioAdapter не содержит реальный переключаемый Native/Core runtime")
    require("machine.setChorusCoreMode(dspModes[monomachine::DspChorus]);" in processor
            and "machine2.setChorusCoreMode(dspModes[monomachine::DspChorus]);" in processor
            and "for(auto& engine:fxEngines)engine.setChorusCoreMode(dspModes[monomachine::DspChorus]);" in processor
            and "if(schema<37)" in processor
            and "dspModeParamId(monomachine::DspChorus)" in processor,
            f"{target}: mode_cho не доходит до P1/P2/FX или legacy state не остаётся Native")
    require("void setChorusCoreMode(int mode) noexcept" in dsp
            and "void setChorusCoreMode(int mode) noexcept" in fx_slots,
            f"{target}: MachineEngine/FX slots не принимают CHOR selector")
    require("constexpr float kChorusInpKneeRaw=4.0f;" in dsp
            and "*(kneeT*kneeT);" in dsp
            and "const float inGain=inputRaw>=kChorusInpKneeRaw" in dsp
            and "?inputRaw*0.015625f:lowKneeGain;" in dsp,
            f"{target}: CHORUS INP −64..−60 quadratic host knee отсутствует")

    panel = editor[editor.find("class DelayFeedbackSafetyPanel"):editor.find("// 1.6.14:", editor.find("class DelayFeedbackSafetyPanel"))]
    require("dfb_dynamics" in data and "dfb_rise" in data
            and "dfb_zero_tail" in data and "dsnd_pos_invert" in data
            and "dsnd_neg_invert" in data and "dfb_low_div" in data
            and "dfb_soft_raw" in data and "dfb_soft_level" in data
            and "dfb_unity_raw" in data and "dfb_hot_raw" in data
            and "dfb_hot_level" in data and "dfb_ceiling" in data
            and "dfb_base_curve" in data and "dfb_base_hold_64" in data
            and "dfb_plateau" in data and "dfb_attack_ms" in data
            and "dfb_release_ms" in data and "dfb_guard_raw" in data
            and "dfb_guard_level_start" in data and "dfb_guard_level_start_127" in data
            and "dfb_guard_plateau_127" in data and "dfb_guard_offset" in data
            and "dfb_guard_curve" in data and "dfb_guard_amount" in data
            and 'DYNAMICS",0,1,1,1' in data
            and 'DSND + FB INVERT",0,1,1,1' in data
            and 'DSND - FB INVERT",0,1,1,1' in data
            and 'КОМП. ВОЗВРАТА",0,1,1,1' in data
            and 'КЛИПЕР ПЕТЛИ",0,1,1,1' in data
            and "class DelayFeedbackSafetyPanel final : public FloatingPanel, private juce::Timer" in editor
            and 'showCallout(std::move(panel),screen,"dfb-guard")' in editor
            and "DocumentWindow" not in panel
            and "BASE CURVE" in panel and "HOLD @64" in panel
            and "START @64" in panel and "PLT @64" in panel
            and "START @127" in panel and "PLT @127" in panel and "GUARD OFFSET" in panel
            and "GUARD CURVE" in panel and "GUARD AMOUNT" in panel
            and "63 UNITY / 64 ENTRY / 65+ GROWTH" in panel
            and "setSize(400,386);" in panel and "startTimerHz(10);" in panel
            and "paintFloatingForeground" in panel and "FLOATING_PANEL_FOREGROUND_TEXT_CONTRACT" in editor
            and "dragOriginScreen" in panel
            and "processor.delayFeedbackLoopLevel(p2)" in panel
            and "Colours::yellow" not in panel and "drawClose" not in panel
            and "closeButton" not in panel and "onClose" not in panel,
            f"{target}: DFB RAW=63 UNITY/RAW>=64 GUARD panel/defaults/live graph неполны")
    require("setDelayFeedbackDynamics(" in dsp
            and "setDelayFeedbackGuard(" in dsp
            and "delayFeedbackLoopLevel() const noexcept" in dsp
            and "delayFeedbackEffective() const noexcept" in dsp
            and "delayFeedbackDynamics.prepare(sr);" in dsp
            and "delayFeedbackDynamics.reset();" in dsp
            and "delayFeedbackDynamics.setLoopGuard(loopClip);" in dsp
            and "delayFeedbackInvertPositive" in dsp
            and "delayFeedbackInvertNegative" in dsp
            and "TrackDelayCore" not in dsp and "MnmTrackDelayCore" not in dsp,
            f"{target}: TrackChain не владеет DFB BASE/GUARD/polarity или оставшимся CORE state")
    require("delayFeedbackDynamics.process(feedbackRaw)" in delay_stage
            and "delayFeedbackDynamics.observeLoop(wetL,wetR);" in delay_stage
            and "delayFeedbackDynamics.observeLoop(a,b);" in delay_stage
            and "const bool guardWriteClip" in delay_stage and "guardOwnsRaw(feedbackRaw)" in delay_stage
            and "coreDelay" not in delay_stage and "dspModeDelayCore" not in delay_stage
            and "negativeDelayComb,invertFeedback" in delay_stage,
            f"{target}: DFB controller неполон или удалённый DLY CORE dispatch остался")
    require("struct Base" in delay_dynamics
            and "kDefaultBaseCurve = 0.55f" in delay_dynamics
            and "kDefaultBaseHoldAt64 = kRaw65Growth / 64.0f" in delay_dynamics
            and "kRaw63Reference = 63.0f" in delay_dynamics
            and "kRaw64Hold = 64.0f" in delay_dynamics
            and "kRaw65Growth = 65.0f" in delay_dynamics
            and "return reference * std::pow(raw / kRaw63Reference, base.lowCurve);" in delay_dynamics
            and "void setBase(Base requested)" in delay_dynamics
            and "baseCoefficientFromLut" in delay_dynamics and "kBaseLutSize" in delay_dynamics
            and "sameBase(next, base_)" in delay_dynamics
            and "setGuardAndGovernor" in delay_dynamics
            and "if (!guardChanged && !governorChanged) return;" in delay_dynamics
            and "struct Guard" in delay_dynamics
            and "float levelStartAt127" in delay_dynamics and "float levelOffset" in delay_dynamics
            and "float plateauLevelAt127" in delay_dynamics
            and "rawGateFor" in delay_dynamics and "guardOwnsRaw" in delay_dynamics and "guardDriveForRawLevel" in delay_dynamics
            and "return sanitiseRaw(rawFeedback) >= kGuardRawArm;" in delay_dynamics
            and "kGuardRawBypass = 63.0f" in delay_dynamics and "kGuardRawArm = 64.0f" in delay_dynamics
            and "void setGuard(Guard requested)" in delay_dynamics
            and "void setGovernor(Governor requested)" in delay_dynamics
            and "struct Curve" not in delay_dynamics
            and "setCurve(" not in delay_dynamics
            and "followerAlpha" in delay_dynamics
            and "void setLoopGuard(bool enabled)" in delay_dynamics
            and "void observeLoop(float left, float right)" in delay_dynamics
            and "guardDriveLut_" in delay_dynamics and "kGuardRawLutSize" in delay_dynamics
            and "rebuildGuardDriveLut" in delay_dynamics
            and "activeFeedback_ = raw / 63.0f;" in delay_dynamics,
            f"{target}: DFB RAW=63 UNITY/RAW>=64 GUARD law или cached LUT неполны")
    require("if(schema<41)" in processor and "if(schema<43)" in processor and "if(schema<44)" in processor and "if(schema<45)" in processor
            and "dfb_guard_raw" in processor and "dfb_base_curve" in processor
            and "dfb_base_hold_64" in processor and "dfb_guard_level_start" in processor
            and "dfb_guard_level_start_127" in processor and "dfb_guard_plateau_127" in processor
            and "dfb_guard_offset" in processor
            and 'ensureDfbBase(base+"dfb_base_curve",1.0f);' in processor
            and 'ensureDfbBase(base+"dfb_base_hold_64",1.0f);' in processor
            and "setDelayFeedbackBase(delayFeedbackBaseP1Raw,chain)" in processor
            and "guard.levelStart=delayValue(raw[0],0.0f);" in processor
            and "guard.levelStartAt127=delayValue(raw[1],0.0f);" in processor
            and "setDelayFeedbackGuard(delayFeedbackGuardP1Raw,chain)" in processor
            and "schema>=40 && schema<42 && value==3" in processor
            and "dspModeNew" in processor
            and "delayFeedbackLoopLevelP1.store(chain.delayFeedbackLoopLevel()" in processor,
            f"{target}: DFB RAW=63 UNITY/RAW>=64 GUARD / schema-45 LFO migration, removed CORE migration или live graph echo неполны")
    require("DFB_ROUTE_UNRESOLVED" in fx_slots
            and "setDelayFeedbackDynamics" not in fx_slots
            and "DelayFeedbackDynamics" not in fx_slots,
            f"{target}: FxSlotEngine не должен выдавать неподтверждённый DFB route за реализованный")
    require("bool invertFeedback" in mnm_delay and "negativeComb!=invertFeedback" in mnm_delay
            and "bool invertFeedback" in new_delay and "negativeComb!=invertFeedback" in new_delay,
            f"{target}: experimental DSND +/- switches не разделяют send route и feedback polarity")
    require("const bool freeTime=!bpm.getToggleState();" in editor
            and "maxTime.setVisible(freeTime);" in editor
            and "const int height=freeTime?170:128;" in editor,
            f"{target}: DTIM/BPM panel не скрывает free-time slider при BPM=ON")
    require("class SongLane" in editor and 'q.first=songBound("arp_song_part_start",1)-1' in editor
            and "for(int col=0;col<q.count;++col)" in editor
            and "void paintThrough(int target,float next)" in editor
            and "for(int part=lo;part<=hi;++part)" in editor
            and "pageGestures{},repeatGestures{}" in editor,
            f"{target}: ARP SONG active window / multi-step drawing path неполон")
    # The ARP canvas no longer paints its redundant PAGE 1..64 caption below
    # steps. The SONG toggle stays usable without its hidden label, and every
    # function tooltip has ASCII/English text for the pixel-font tooltip window.
    tooltip_code = "\n".join(line.split("//", 1)[0]
                             for line in editor.splitlines() if "setTooltip" in line)
    require('pixel::text(g,"PAGE 1..64"' not in editor
            and 'add(1,"WHOLE PAGE",Clip::arpClipboardAll);' in editor
            and 'songMode.setButtonText("")' in editor
            and not contains_cyrillic(tooltip_code),
            f"{target}: ARP captions/copy menu/English function tooltips неполны")

    # Main-surface MOD ENV must use the same compact shell as AMP/FIL. The
    # held-drag selection is deliberately committed before dock destruction;
    # PixelButton must not keep the original drag source visually pressed.
    mini_start = editor.find("class ModEnvelopeMiniPage final")
    mini_end = editor.find("class SamplePage final", mini_start)
    mod_mini = editor[mini_start:mini_end]
    envelope_swap_start = editor.find("void showEnvelopeMini(")
    envelope_swap_end = editor.find("void toggleEnvMini()", envelope_swap_start)
    envelope_swap = editor[envelope_swap_start:envelope_swap_end]
    require(mini_start >= 0 and mini_end > mini_start
            and "setSize(387,236);" in mod_mini
            and "const int x=8+i*95,y=152;" in mod_mini
            and "constexpr int graphX=8,graphY=32,graphWidth=371,graphHeight=114;" in mod_mini
            and "g.drawHorizontalLine(28,0.0f,387.0f);g.drawVerticalLine(96,0.0f,28.0f);g.drawVerticalLine(288,0.0f,28.0f);" in mod_mini
            and "routeButton" not in mod_mini and "pageButtons" not in mod_mini
            and "addRoute" not in mod_mini and "pixel::text(" not in mod_mini
            and "MATRIX SOURCE" not in mod_mini and "RMB LARGE" not in mod_mini,
            f"{target}: compact MOD ENV не совпадает с AMP/FIL grid или сохранил лишний UI")
    require("const bool dragPressed=down&&outlined&&!static_cast<bool>(heldDrag)" in editor
            and "const bool filled=dragPressed||getToggleState()" in editor
            and "g.setColour(ink);paintFloatingForeground(g);drawPin(g);" in editor
            and "const bool suppressReleaseClick=wasDragged;" in editor
            and "auto savedOnClick=std::move(onClick);onClick={};" in editor
            and "onClick=std::move(savedOnClick);wasDragged=false;" in editor
            and "void showDocked(std::unique_ptr<juce::Component> panel,juce::Rectangle<int> localAt,bool belowAnchor,bool keepEnvelopeSelection=false)" in editor
            and "dismissDock(!keepEnvelopeSelection);" in editor
            and "const int wantedSelection=static_cast<int>(wanted)-1;" in envelope_swap
            and envelope_swap.find("setEnvelopeCategorySelection(wantedSelection);") < envelope_swap.find("if(wanted==MiniEnvelopeKind::Amp)")
            and envelope_swap.count("false,true);") == 3,
            f"{target}: held-LMB ENV handoff не является атомарным или release source возвращает исходную category")

for relative in MIRRORED + MIRRORED_TESTS:
    left = ROOT / TARGETS[0] / relative
    right = ROOT / TARGETS[1] / relative
    require(left.is_file() and right.is_file(), f"Нет mirrored file: {relative}")
    if left.is_file() and right.is_file():
        require(left.read_bytes() == right.read_bytes(),
                f"Synth/FX mirror differs: {relative}")

for target in TARGETS:
    manifest_path = ROOT / target / "SOURCE_BUILD.json"
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        require(manifest.get("build", "").startswith("1.9.35, 03.10.2026"),
                f"{target}: SOURCE_BUILD release metadata не 1.9.35 / 03.10.2026")
        for relative, expected_hash in manifest["files"].items():
            path = ROOT / target / relative
            actual_hash = sha256(path) if path.is_file() else ""
            require(actual_hash == expected_hash,
                    f"{target}: SOURCE_BUILD hash differs or file is absent: {relative}")
    except (OSError, KeyError, json.JSONDecodeError) as error:
        errors.append(f"{target}: SOURCE_BUILD.json unreadable: {error}")

try:
    sums = ARCHIVE / "SHA256SUMS.txt"
    require(sums.is_file(), "Не найден SHA256SUMS архивных candidate sources")
    if sums.is_file():
        for line in sums.read_text(encoding="utf-8").splitlines():
            expected, relative = line.split(maxsplit=1)
            path = ARCHIVE / relative.removeprefix("./")
            require(path.is_file() and sha256(path) == expected,
                    f"Архив candidate source повреждён: {relative}")
except ValueError as error:
    errors.append(f"Неверный формат SHA256SUMS archive: {error}")

# CMakeLists are deliberately not byte-identical: each target preserves its
# own plugin identity although the retired-test replacement is the same.
expected_projects = {
    "Monomachine_Nova_Synth": ("project(MonomachineNovaUnifiedSynth", "IS_SYNTH TRUE", "VERSION 1.9.35"),
    "Monomachine_Nova_FX": ("project(MonomachineNovaUnifiedFX", "IS_SYNTH FALSE", "VERSION 1.9.35"),
}
for target, markers in expected_projects.items():
    cmake = read(target, "CMakeLists.txt")
    header = read(target, "JuceLibraryCode/JuceHeader.h")
    defines = read(target, "JuceLibraryCode/JucePluginDefines.h")
    jucer = read(target, f"Monomachine Nova {'Synth' if target.endswith('Synth') else 'FX'}.jucer")
    config = read(target, "Source/NovaConfig.h")
    build_check = read(target, "Check-Build.ps1")
    require(all(marker in cmake for marker in markers),
            f"{target}: CMake plugin identity/version повреждена")
    require('versionString  = "1.9.35"' in header
            and "versionNumber  = 0x10923" in header
            and "JucePlugin_Version                1.9.35" in defines
            and "JucePlugin_VersionCode            0x10923" in defines
            and 'JucePlugin_VersionString          "1.9.35"' in defines
            and 'version="1.9.35"' in jucer and "BUILD 1.9.35" in config
            and "BUILD 1.9.35" in build_check,
            f"{target}: source/package version metadata не согласована с 1.9.35")

if errors:
    print("FM MODE CLEANUP STATIC CHECK: FAIL")
    for error in errors:
        print(f"- {error}")
    sys.exit(1)

print("FM MODE CLEANUP STATIC CHECK: OK")
print("- FM+ STAT/PAR/DYN: шесть доступных режимов FRQ/BPM")
print("- IDs 0..5 сохранены; raw IDs 6..8 нормализуются в MNM FRQ")
print("- Candidate sources/tests сохранены вне shipping Source с SHA-256")
print("- Phaser full-wet phase-difference correction сохраняет прежнюю формулу рядом")
print("- CHOR: mode_cho реально переключает NativeChorusCore (default) и ChorusCore в P1/P2/FX; legacy state остаётся Native")
print("- CHOR INP −64..−60 использует ограниченную квадратичную host-кривую")
print("- DFB: RAW 63 exact unity, RAW 64 guard entry, 65+ RAW/64, hard <=63 GUARD/clip bypass, cached LUT + four @64/@127 anchors/offset и P1/P2 live graph; legacy IDs retained")
print("- Schema-45: direct-LFO DPTH reset migration plus independent UNI/BI, INV and ALT DUAL state are retained")
print("- DLY CORE снят из shipping Source и UI; saved schema-40/41 CORE value 3 мигрирует в NEW=2")
print("- FxSlotEngine помечен DFB_ROUTE_UNRESOLVED: custom-slot DFB route не додуман")
print("- ARP SONG показывает только S START..S END и рисует S-PART/REPEAT через пройденные активные позиции")
print("- ARP: убрана лишняя PAGE 1..64 подпись, меню COPY/PASTE начинается с WHOLE PAGE, function tooltips English/ASCII")
print("- MOD ENV compact: shell/grid совпадает с AMP/FIL; лишние MATRIX/RMB labels сняты, held-LMB category handoff атомарен")
print("- Зеркальные Synth/FX sources и retained-mode tests byte-identical")
print("- SOURCE_BUILD manifests совпадают с каждым перечисленным source file")
print("- Default ARP SONG page содержит явный float cast для MSVC")
print("- CMake/.jucer/JUCE metadata, видимые BUILD markers и SOURCE_BUILD согласованы с 1.9.35")
