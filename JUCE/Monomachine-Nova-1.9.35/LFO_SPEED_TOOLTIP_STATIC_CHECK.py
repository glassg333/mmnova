#!/usr/bin/env python3
"""Статический source-only аудит динамической подсказки LFO SPD.

Не компилирует проект и не запускает editor, plugin, DSP, CTest, DAW или render.
Проверяет только active Synth/FX source, зеркальность редактора и формулу,
которую показывает UI-подсказка.
"""
from __future__ import annotations

from pathlib import Path
import math
import sys

ROOT = Path(__file__).resolve().parent
SYNTH = ROOT / "Monomachine_Nova_Synth"
FX = ROOT / "Monomachine_Nova_FX"
EDITOR_REL = Path("Source/PluginEditor.cpp")
PROCESSOR_H_REL = Path("Source/PluginProcessor.h")
PROCESSOR_REL = Path("Source/PluginProcessor.cpp")

failures: list[str] = []
checks = 0


def require(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in haystack:
        failures.append(f"{label}: не найдено {needle!r}")


def forbid(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle in haystack:
        failures.append(f"{label}: найден нежелательный фрагмент {needle!r}")


def close(actual: float, expected: float, label: str) -> None:
    global checks
    checks += 1
    if not math.isclose(actual, expected, rel_tol=0.0, abs_tol=1.0e-12):
        failures.append(f"{label}: {actual!r}, ожидалось {expected!r}")


for relative in (EDITOR_REL, PROCESSOR_H_REL, PROCESSOR_REL):
    checks += 1
    left = SYNTH / relative
    right = FX / relative
    if not left.is_file() or not right.is_file():
        failures.append(f"mirror {relative}: отсутствует в Synth или FX")
    elif left.read_bytes() != right.read_bytes():
        failures.append(f"mirror {relative}: Synth/FX не byte-identical")

editor = (SYNTH / EDITOR_REL).read_text(encoding="utf-8")
processor_h = (SYNTH / PROCESSOR_H_REL).read_text(encoding="utf-8")
processor = (SYNTH / PROCESSOR_REL).read_text(encoding="utf-8")

require(editor,
        "class Cell final : public juce::Component, public juce::SettableTooltipClient,",
        "подсказка доступна и на faceplate cell, а не только на ручке")
require(editor, "bool isLfoSpeed() const noexcept { return pageIndex>=3&&knob==5; }",
        "подсказка ограничена SPD всех 12 LFO")
require(editor, "juce::String makeLfoSpeedTooltip() const {",
        "выделен генератор динамического текста")
require(editor,
        "const int spd=juce::jlimit(0,127,juce::roundToInt(parameter->convertFrom0to1(parameter->getValue())));",
        "SPD считывается из активного LFO control")
require(editor,
        "processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,4))",
        "MULT считывается из активного LFO control")
require(editor, "const int mult=1<<multIndex;",
        "MULT использует выбор 1X..64X")
require(editor, "const double reportedBpm=processor.currentBpm();",
        "подсказка получает live effective BPM через processor API")
require(editor,
        "const double bpm=std::isfinite(reportedBpm)&&reportedBpm>0.0?reportedBpm:120.0;",
        "некорректный tempo имеет безопасный UI fallback")
require(editor, "const double cycle16ths=2048.0/(static_cast<double>(spd)*static_cast<double>(mult));",
        "показана калькуляторная формула полного LFO-цикла")
require(editor, "const double cycleSeconds=cycle16ths*15.0/bpm;",
        "шестнадцатые переведены в секунды от текущего BPM")
require(editor, "const double hertz=1.0/cycleSeconds;",
        "выводится обратная частота цикла")
require(editor, "if(spd==0)return headline+\"\\nFULL CYCLE: STOPPED (SPD 0) | LAW: 2048 / (SPD x MULT)\";",
        "SPD 0 явно показывается как stopped без деления на ноль")
require(editor, "LFO SPEED / SPD:", "заголовок подсказки остаётся английским")
require(editor, "TEMPO NOW:", "подсказка явно показывает текущий BPM")
require(editor, "FULL CYCLE:", "подсказка показывает полный цикл")
require(editor, "HALF MODE:", "подсказка показывает timing для HALF")
require(editor, "LAW: 2048 / (SPD x MULT)", "подсказка документирует закон calculator")
require(editor, "refreshLfoSpeedTooltip();\n        repaint();", "подсказка готова сразу после bind")
require(editor, "void timerCallback()override{refreshLfoDepthControls();refreshLfoSpeedTooltip();repaint();}",
        "BPM и timing обновляются динамически в UI timer")
require(editor, "if(next==lfoSpeedTooltipCache)return;",
        "неизменный текст не перезаписывается каждый UI tick")
require(editor, "!isChoice&&!pickMode", "подсказка отключается для menu/pick state")
require(editor, "lfoSpeedTooltipCache.clear();setTooltip({});", "tooltip cell очищается при rebind")

require(processor_h, "double currentBpm() const {return tempoDisplay.load();}",
        "currentBpm читает live tempo display")
require(processor_h, "std::atomic<float> peak{0};std::atomic<double> tempoDisplay{120};",
        "tempo display имеет безопасное стартовое значение")
require(processor, "tempoDisplay.store(bpm);", "processor обновляет live tempo display")

# Calculator-law sanity checks only: these are pure static arithmetic, not DSP/plugin execution.
def calculator(spd: float, multiplier: float, bpm: float) -> tuple[float, float, float]:
    steps = 2048.0 / (spd * multiplier)
    seconds = steps * 15.0 / bpm
    return steps, seconds, 1.0 / seconds

steps, seconds, hz = calculator(64.0, 2.0, 120.0)
close(steps, 16.0, "calculator SPD 64 / MULT 2X: sixteenth steps")
close(seconds, 2.0, "calculator SPD 64 / MULT 2X at 120 BPM: seconds")
close(hz, 0.5, "calculator SPD 64 / MULT 2X at 120 BPM: hertz")
steps, seconds, hz = calculator(32.0, 1.0, 120.0)
close(steps, 64.0, "calculator SPD 32 / MULT 1X: sixteenth steps")
close(seconds, 8.0, "calculator SPD 32 / MULT 1X at 120 BPM: seconds")
close(hz, 0.125, "calculator SPD 32 / MULT 1X at 120 BPM: hertz")

forbid(editor, "std::pow(2.0,(", "tooltip не возвращает прежний exponential SPD law")

if failures:
    print("LFO SPEED TOOLTIP STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"LFO SPEED TOOLTIP STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
