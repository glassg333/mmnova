#!/usr/bin/env python3
"""Статический source-only аудит Monomachine LFO HALF/SPD/MULT.

Скрипт не компилирует проект и не запускает DSP, plugin, CTest, DAW или render.
Он проверяет только текстовые инварианты active Synth/FX source и их mirror.
"""
from __future__ import annotations

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
TARGETS = ("Monomachine_Nova_Synth", "Monomachine_Nova_FX")
MIRRORED = (
    "Source/NovaDSP.h",
    "Source/NovaData.h",
    "Source/models/parameter_conversions.hpp",
    "Source/models/track_pages.hpp",
    "Source/PluginProcessor.h",
    "Source/PluginProcessor.cpp",
)

failures: list[str] = []
checks = 0


def read(target: str, relative: str) -> str:
    return (ROOT / target / relative).read_text(encoding="utf-8")


def require(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in haystack:
        failures.append(f"{label}: не найдено {needle!r}")


def forbid(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle in haystack:
        failures.append(f"{label}: найден устаревший фрагмент {needle!r}")


for relative in MIRRORED:
    checks += 1
    left = ROOT / TARGETS[0] / relative
    right = ROOT / TARGETS[1] / relative
    if not left.is_file() or not right.is_file():
        failures.append(f"mirror {relative}: отсутствует в Synth или FX")
    elif left.read_bytes() != right.read_bytes():
        failures.append(f"mirror {relative}: Synth/FX не byte-identical")

lfo = read(TARGETS[0], "Source/NovaDSP.h")
require(lfo, "static constexpr int kFree=0,kTrig=1,kHold=2,kOne=3,kHalf=4;",
        "HALF is the fifth/final trigger mode")
require(lfo, "kHalfCycle=0.5;", "HALF phase endpoint")
require(lfo, "kElektronLfoBaseSteps=2048.0", "Elektron calculator base steps")
require(lfo, "kElektronLfoRateDenominator=kElektronLfoBaseSteps*15.0", "tempo conversion to 30720")
require(lfo, "return std::max(0.0,bpm)*speed*multiplier/kElektronLfoRateDenominator;",
        "linear SPD times discrete MULT rate law")
require(lfo, "const int multIndex=std::clamp(static_cast<int>(std::lround(p[4])),0,6);",
        "MULT choice index clamp")
require(lfo, "const double multiplier=static_cast<double>(1<<multIndex); // 1X, 2X ... 64X",
        "MULT choice mapping")
forbid(lfo, "std::pow(2.0,(p[5]-64)/24.0)", "removed exponential centred SPD law")
require(lfo, "if(mode==kTrig||mode==kOne||mode==kHalf){", "HALF retriggers phase")
require(lfo, "phase=0;random=noise(rng);oneActive=mode==kOne;halfActive=mode==kHalf;",
        "HALF starts at waveform origin")
require(lfo, "if(phase+step>=kHalfCycle){", "HALF stops on half-cycle boundary")
require(lfo, "phase=kHalfCycle;", "HALF clamps overshoot at exactly half a cycle")
require(lfo, "halfHeld=interlace(shape(kind,0.0f),intl);halfActive=false;",
        "HALF retains final reached output level")
require(lfo, "return mode==kHalf&&!halfActive?halfHeld:interlace(result,intl);",
        "HALF returns the held final level until next trigger")

layout = read(TARGETS[0], "Source/NovaData.h")
require(layout, 'if(i==2) choices="FREE|TRIG|HOLD|ONE|HALF";',
        "APVTS TRIG list exposes final HALF entry")
require(layout, "for(int page=0;page<15;++page)", "fifteen pages include twelve LFO pages")
require(layout, "const char* sections[]{\"AMP\",\"FILTER\",\"FX\",\"LFO1\",\"LFO2\",\"LFO3\",\"LFO4\",\"LFO5\",\"LFO6\",\"P2 LFO1\",\"P2 LFO2\",\"P2 LFO3\",\"P2 LFO4\",\"P2 LFO5\",\"P2 LFO6\"};",
        "independent P1/P2 LFO parameter groups")

conversions = read(TARGETS[0], "Source/models/parameter_conversions.hpp")
require(conversions, 'const char* trigs[] = {"FREE", "TRIG", "HOLD", "ONE", "HALF"};',
        "legacy LCD conversion exposes HALF")
require(conversions, "std::clamp(static_cast<int>(val / 26), 0, 4)",
        "legacy LCD conversion reaches final HALF bucket")

pages = read(TARGETS[0], "Source/models/track_pages.hpp")
require(pages, "Trigger Mode (FREE/TRIG/HOLD/ONE/HALF)", "English trigger-mode description")
require(pages, "Tempo-synchronised linear LFO speed", "English SPD timing description")
require(pages, "Tempo multiplier (1x..64x)", "English MULT timing description")

processor_h = read(TARGETS[0], "Source/PluginProcessor.h")
require(processor_h, "std::array<std::array<float,8>,12> lfoParams{},effectiveLfoParams{};",
        "twelve independently retained LFO parameter arrays")
processor = read(TARGETS[0], "Source/PluginProcessor.cpp")
require(processor, "lfos[i].trigger(effectiveLfoParams[i]);lfos3[i].trigger(effectiveLfoParams[3+i]);",
        "P1 LFO1..6 trigger fan-out")
require(processor, "lfos2[i].trigger(effectiveLfoParams[6+i]);lfos4[i].trigger(effectiveLfoParams[9+i]);",
        "P2 LFO1..6 trigger fan-out")
require(processor, "for(auto& l:lfos)l.reset();for(auto& l:lfos3)l.reset();for(auto& l:lfos2)l.reset();for(auto& l:lfos4)l.reset();",
        "prepare/panic reset every active P1/P2 LFO")

if failures:
    print("LFO TIMING STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"LFO TIMING STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
