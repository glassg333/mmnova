#!/usr/bin/env python3
"""Статический source-only аудит schema 44 DFB BASE BEND / GUARD / DLY cleanup.

Имя сохранено ради совместимости с прежними поставками. Бывший DLY CORE
целенаправленно отсутствует из shipping Source. Скрипт не компилирует и не
запускает CTest, VST3, рендер или DSP-тесты; проверяются только текстовые
инварианты активных Synth/FX исходников и byte-identical mirror.
"""
from __future__ import annotations

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
SYNTH = ROOT / "Monomachine_Nova_Synth" / "Source"
FX = ROOT / "Monomachine_Nova_FX" / "Source"

MIRRORED = (
    "NovaData.h",
    "NovaDSP.h",
    "PluginProcessor.h",
    "PluginProcessor.cpp",
    "PluginEditor.cpp",
    "dsp/DelayFeedbackDynamics.hpp",
    "dsp/TrackDelay.inl",
    "models/DspModes.hpp",
)
MIRRORED_TESTS = (
    "DelayFeedbackDspTests.cpp",
    "FmFixModeTests.cpp",
    "ModeRollbackTests.cpp",
)

failures: list[str] = []
checks = 0


def text(relative: str) -> str:
    return (SYNTH / relative).read_text(encoding="utf-8")


def require(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in haystack:
        failures.append(f"{label}: не найдено {needle!r}")


def forbid(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle in haystack:
        failures.append(f"{label}: запрещённый фрагмент найден: {needle!r}")


for relative in MIRRORED:
    checks += 1
    left = SYNTH / relative
    right = FX / relative
    if not left.is_file() or not right.is_file():
        failures.append(f"mirror {relative}: отсутствует в Synth или FX")
    elif left.read_bytes() != right.read_bytes():
        failures.append(f"mirror {relative}: Synth/FX не byte-identical")

for relative in MIRRORED_TESTS:
    checks += 1
    left = ROOT / "Monomachine_Nova_Synth" / "tests" / relative
    right = ROOT / "Monomachine_Nova_FX" / "tests" / relative
    if not left.is_file() or not right.is_file():
        failures.append(f"test mirror {relative}: отсутствует в Synth или FX")
    elif left.read_bytes() != right.read_bytes():
        failures.append(f"test mirror {relative}: Synth/FX не byte-identical")

# NovaConfig intentionally differs by product. Mirroring it would recreate the
# JucePlugin_IsSynth pairing failure caught by a Windows build.
synth_config = (ROOT / "Monomachine_Nova_Synth" / "Source" / "NovaConfig.h").read_text(encoding="utf-8")
fx_config = (ROOT / "Monomachine_Nova_FX" / "Source" / "NovaConfig.h").read_text(encoding="utf-8")
require(synth_config, "#define NOVA_SYNTH 1", "Synth NovaConfig identity")
require(synth_config, "Monomachine Nova Synth source BUILD", "Synth NovaConfig build marker")
require(synth_config, "schema 44", "Synth NovaConfig schema marker")
require(fx_config, "#define NOVA_SYNTH 0", "FX NovaConfig identity")
require(fx_config, "Monomachine Nova FX source BUILD", "FX NovaConfig build marker")
require(fx_config, "schema 44", "FX NovaConfig schema marker")

modes = text("models/DspModes.hpp")
require(modes, "kDspModeSchemaVersion = 44", "schema")
require(modes, 'case DspDelay:  return "mnm|old|new"', "DLY choices after CORE removal")
forbid(modes, "dspModeDelayCore", "removed DLY CORE selector")
forbid(modes, "mnm|old|new|core", "removed DLY CORE menu item")

layout = text("NovaData.h")
for suffix in (
    "dfb_base_curve", "dfb_base_hold_64", "dfb_guard_raw",
    "dfb_guard_level_start", "dfb_guard_level_start_127",
    "dfb_guard_plateau_127", "dfb_guard_offset", "dfb_guard_curve",
    "dfb_guard_amount",
):
    require(layout, suffix, "layout BASE/GUARD")
require(layout, '"dfb_base_curve",title+"BASE CURVE",0.10f,2.00f,0.55f', "BASE CURVE fresh default")
require(layout, '"dfb_base_hold_64",title+"BASE HOLD @64",63.0f/64.0f,1.0f,0.992f', "HOLD @64 fresh default")
require(layout, '"dfb_guard_level_start",title+"GUARD LVL START @64",0.0f,4.0f,0.0f', "START @64 screenshot default")
require(layout, '"dfb_guard_level_start_127",title+"GUARD LVL START @127",0.0f,4.0f,0.0f', "START @127 screenshot default")
require(layout, '"dfb_guard_plateau_127",title+"GUARD PLATEAU @127",0.05f,4.0f,3.46f', "PLATEAU @127 screenshot default")
require(layout, '"dfb_guard_offset",title+"GUARD LVL OFFSET",-2.0f,2.0f,0.18f', "common OFFSET screenshot default")
require(layout, '"dfb_plateau",title+"PLATEAU",0.05f,4.0f,0.05f', "PLATEAU @64 screenshot default")
require(layout, '"dfb_attack_ms",title+"ATTACK MS",1.0f,1000.0f,1.0f', "ATTACK screenshot default")
require(layout, '"dfb_release_ms",title+"RELEASE MS",1.0f,5000.0f,1.0f', "RELEASE screenshot default")
for suffix in ("dfb_low_div", "dfb_soft_raw", "dfb_unity_raw", "dfb_hot_raw", "dfb_ceiling", "dfb_rise", "dfb_zero_tail"):
    require(layout, suffix, "retained legacy DFB ID")
for enabled in ("dfb_dynamics", "dsnd_pos_invert", "dsnd_neg_invert", "dfb_return_comp", "dfb_clip"):
    require(layout, f'prefix)+"{enabled}",', "reference toggle ID")

processor = text("PluginProcessor.cpp")
require(processor, "setDelayFeedbackBase", "processor BASE runtime wiring")
require(processor, "setDelayFeedbackGuard", "processor GUARD runtime wiring")
require(processor, "if(schema<41)", "schema-41 historical migration")
require(processor, "if(schema<43)", "schema-43 raw-window migration")
require(processor, "if(schema<44)", "schema-44 BASE migration")
require(processor, 'ensureDfbBase(base+"dfb_base_curve",1.0f);', "old state neutral BASE CURVE")
require(processor, 'ensureDfbBase(base+"dfb_base_hold_64",1.0f);', "old state neutral HOLD @64")
require(processor, "std::array<std::atomic<float>*,2>", "two BASE APVTS controls")
require(processor, "std::array<std::atomic<float>*,9>", "nine raw-window controls")
require(processor, "base.lowCurve=delayValue(raw[0]", "BASE CURVE DSP wiring")
require(processor, "base.holdAt64=delayValue(raw[1]", "HOLD @64 DSP wiring")
require(processor, "guard.levelStart=delayValue(raw[0],0.0f);", "START @64 DSP wiring")
require(processor, "guard.levelStartAt127=delayValue(raw[1],0.0f);", "START @127 DSP wiring")
require(processor, "guard.levelOffset=delayValue(raw[2],nova::DelayFeedbackDynamics::kDefaultGuardOffset);", "OFFSET DSP wiring")
require(processor, "governor.plateauLevelAt127=delayValue(raw[6],nova::DelayFeedbackDynamics::kDefaultPlateauLevelAt127);", "PLATEAU @127 DSP wiring")
require(processor, "schema>=40 && schema<42 && value==3", "removed CORE state migration")
require(processor, "dspModeNew", "CORE-to-NEW migration target")

header = text("dsp/DelayFeedbackDynamics.hpp")
require(header, "struct Base", "persisted BASE state")
require(header, "kDefaultBaseCurve = 0.55f", "BASE CURVE default")
require(header, "kDefaultBaseHoldAt64 = 0.992f", "HOLD @64 default")
require(header, "kRaw63Reference = 63.0f", "RAW 63 reference")
require(header, "kRaw64Hold = 64.0f", "RAW 64 hold")
require(header, "kRaw65Growth = 65.0f", "RAW 65 upper path")
require(header, "return reference * std::pow(raw / kRaw63Reference, base.lowCurve);", "curved low BASE law")
require(header, "return raw / 64.0f;", "retained upper RAW/64 path")
require(header, "void setBase(Base requested)", "BASE snapshot setter")
require(header, "sameBase(next, base_)", "BASE setter cache")
require(header, "kBaseLutSize", "BASE LUT")
require(header, "baseCoefficientFromLut", "BASE LUT audio path")
require(header, "setGuardAndGovernor", "combined GUARD snapshot setter")
require(header, "if (!guardChanged && !governorChanged) return;", "GUARD LUT cache")
require(header, "kGuardRawBypass = 63.0f", "raw bypass boundary")
require(header, "kGuardRawArm = 64.0f", "raw arm boundary")
require(header, "static float rawGateFor", "narrow soft raw arm")
require(header, "struct GuardWindow", "raw-window state")
require(header, "float levelStartAt127", "START @127 state")
require(header, "float plateauLevelAt127", "PLATEAU @127 state")
require(header, "float levelOffset", "common OFFSET state")
require(header, "guardWindowForRaw", "shared raw anchor interpolation")
require(header, "guardDriveForRawLevel", "raw/level GUARD law")
require(header, "kGuardRawLutSize", "two-dimensional no-pow GUARD LUT")
require(header, "RAW <= 63 is a true guard bypass", "low-RAW follower reset")
forbid(header, "guardDriveForLevel", "removed level-only GUARD law")
forbid(header, "struct Curve", "removed active anchor curve")
forbid(header, "setCurve(", "removed active anchor curve API")

ui = text("PluginEditor.cpp")
ui_start = ui.find("class DelayFeedbackSafetyPanel")
ui_end = ui.find("// 1.6.14:", ui_start)
checks += 1
if ui_start < 0 or ui_end < 0:
    failures.append("DFB panel: границы класса не найдены")
    panel = ""
else:
    panel = ui[ui_start:ui_end]
for visible in ("BASE CURVE", "HOLD @64", "START @64", "PLT @64", "START @127", "PLT @127", "GUARD OFFSET", "GUARD CURVE", "GUARD AMOUNT", "ATTACK", "RELEASE", "63 REF / 64 HOLD / 65+ GROWTH"):
    require(panel, visible, "BASE/GUARD visible control/graph")
require(panel, "setSize(400,386);", "narrow embedded DFB panel")
require(panel, "startTimerHz(10);", "light DFB graph repaint rate")
require(panel, "public FloatingPanel", "common pin contract")
require(panel, "drawPin(g);", "visible pin control")
require(panel, "dragOriginScreen", "screen-coordinate bounded drag")
require(panel, "void mouseDrag", "in-editor movable panel")
require(panel, "juce::Colours::white", "RMB white ink")
for hidden in ("LOW DIV", "SOFT RAW", "UNITY RAW", "HOT RAW", "CEILING", "ZERO TAIL", "GUARD RAW (LEGACY)"):
    forbid(panel, hidden, "obsolete RMB control")
for hidden in ("DocumentWindow", "drawClose", "closeButton", "onClose"):
    forbid(panel, hidden, "DFB cannot expose a native/close window control")
forbid(panel, "Colours::yellow", "RMB yellow text/marker")
forbid(ui, "DfbGuardWindow", "removed detached DFB window class")
forbid(ui, "dfbGuardWindow", "removed detached DFB window state")
require(ui, 'showCallout(std::move(panel),screen,"dfb-guard")', "DFB in-editor callout")
require(ui, "closeDfbGuardPanel", "DFB child close on P1/P2 switch")

track = text("dsp/TrackDelay.inl")
require(track, "RAW 0..63 bypasses", "TrackDelay low-RAW contract")
require(track, "returns to RAW/64 at 65+", "TrackDelay BASE upper-path contract")
forbid(track, "coreDelay", "removed CORE audio dispatch")
forbid(track, "dspModeDelayCore", "removed CORE mode branch")

chain = text("NovaDSP.h")
require(chain, "setDelayFeedbackBase", "TrackChain BASE path")
require(chain, "raw-gated 64..127 GUARD", "TrackChain raw-window contract")
forbid(chain, "MnmTrackDelayCore", "removed CORE include")
forbid(chain, "TrackDelayCore", "removed CORE owned state")
forbid(chain, "coreDelay", "removed CORE lifecycle/filter state")

feedback_test = (ROOT / "Monomachine_Nova_Synth" / "tests" / "DelayFeedbackDspTests.cpp").read_text(encoding="utf-8")
require(feedback_test, "DFB BASE: 63 reference + curved 60..62 + HOLD @64", "schema-44 BASE regression source")
require(feedback_test, "kDspModeSchemaVersion == 44", "schema-44 delay test")
forbid(feedback_test, "DelayFeedbackDynamics::Curve", "stale anchor-curve test")
forbid(feedback_test, "TrackDelayCore", "removed CORE test")
fm_test = (ROOT / "Monomachine_Nova_Synth" / "tests" / "FmFixModeTests.cpp").read_text(encoding="utf-8")
require(fm_test, "kDspModeSchemaVersion == 44", "schema-44 FM retained-ID test")

for project in (ROOT / "Monomachine_Nova_Synth" / "CMakeLists.txt", ROOT / "Monomachine_Nova_FX" / "CMakeLists.txt"):
    checks += 1
    if "VERSION 1.9.35" not in project.read_text(encoding="utf-8"):
        failures.append(f"metadata: {project.relative_to(ROOT)} не содержит VERSION 1.9.35")

if failures:
    print("STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)
print(f"STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
