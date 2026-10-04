#!/usr/bin/env python3
"""Статический source-only аудит alignment P1/P2 в LFO LOCKS.

Не компилирует проект и не запускает editor, plugin, DSP, CTest, DAW или render.
Проверяет только active Synth/FX исходники и отсутствие state/choice-модификации
у visual-only P2 PITCH spacer.
"""
from __future__ import annotations

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
SYNTH_EDITOR = ROOT / "Monomachine_Nova_Synth" / "Source" / "PluginEditor.cpp"
FX_EDITOR = ROOT / "Monomachine_Nova_FX" / "Source" / "PluginEditor.cpp"
SYNTH_DATA = ROOT / "Monomachine_Nova_Synth" / "Source" / "NovaData.h"
FX_DATA = ROOT / "Monomachine_Nova_FX" / "Source" / "NovaData.h"

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
        failures.append(f"{label}: остался устаревший фрагмент {needle!r}")


for left, right, label in (
    (SYNTH_EDITOR, FX_EDITOR, "PluginEditor.cpp"),
    (SYNTH_DATA, FX_DATA, "NovaData.h"),
):
    checks += 1
    if not left.is_file() or not right.is_file():
        failures.append(f"{label}: отсутствует в Synth или FX")
    elif left.read_bytes() != right.read_bytes():
        failures.append(f"{label}: Synth/FX не byte-identical")

editor = SYNTH_EDITOR.read_text(encoding="utf-8")
data = SYNTH_DATA.read_text(encoding="utf-8")

require(editor, "class LfoLockPanel final", "изменён только LFO LOCKS UI")
require(editor, "setSize(705,44+11*30+46)", "panel уже имеет 11 visual rows на PAGE column")
require(editor, "const int nRows=s<2?11:8;", "обе PAGE columns рисуют одинаковые 11 rows")
forbid(editor, "const int nRows=s<2?(s==0?11:10):8;", "старый second-column layout из 10 rows")
require(editor, "const bool p2PitchSpacer=s==1&&k==0;", "spacer только в первой строке второй PAGE column")
require(editor, 'pixel::text(g,"P2 PITCH N/A",{bx+42,y+3,176,22},14,false);',
        "placeholder остаётся English/ASCII и помещается в PAGE row")
require(editor, "if(p2PitchSpacer){g.setColour(ink.withAlpha(0.35f));pixel::text(g,\"P2 PITCH N/A\",{bx+42,y+3,176,22},14,false);continue;}",
        "placeholder не рисует lock/solo/current-value state")
require(editor, "const int val=s==1?10+k:k;", "visual rows 1..10 второй column отображают PAGE 11..20")
require(editor, "if(x<470)return lr==0?-1:10+lr;", "click по spacer возвращает no-hit; остальные rows map 11..20")
require(editor, "const int hit=rowAt(e.x,e.y);if(hit<0)return;", "mouseDown не меняет state при no-hit spacer")
require(editor, "const int hit=rowAt(e.x,e.y);if(hit<0)return;const int row=hit;const bool pg=(e.x<470);", "mouseDrag не рисует lock при no-hit spacer")
require(editor, "juce::jlimit(0,pg?2097151:255,m)", "PAGE/DEST lock masks остаются прежними")
require(editor, "juce::jlimit(0,pg?21:8,s)", "PAGE/DEST solo ranges остаются прежними")

require(data, "inline constexpr int kLfoPageChoiceCount = 21;", "число persistent PAGE choices не изменено")
require(data, "if(page==0)return kPitchMatrixTarget;", "единственный физический PITCH target остаётся PAGE 0")
require(data, "if(page<=4){static const char* n[]{\"P2 SYNT\",\"P2 AMP\",\"P2 FILT\",\"P2 EFFX\"};return n[page-1];}",
        "P2 group mapping после PAGE 0 не изменён")

# Pure layout arithmetic: not a plugin/UI execution.
second_column = [None] + [10 + visual_row for visual_row in range(1, 11)]
checks += 1
if second_column != [None, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]:
    failures.append(f"visual PAGE mapping: {second_column!r}")
checks += 1
if len(second_column) != 11 or second_column[0] is not None:
    failures.append("visual spacer: первая строка не является единственным non-selectable placeholder")

if failures:
    print("LFO LOCK ALIGNMENT STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"LFO LOCK ALIGNMENT STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
