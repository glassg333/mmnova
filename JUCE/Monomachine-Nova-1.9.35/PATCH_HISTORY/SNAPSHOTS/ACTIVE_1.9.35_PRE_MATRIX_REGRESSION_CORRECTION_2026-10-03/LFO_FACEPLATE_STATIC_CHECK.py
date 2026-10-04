#!/usr/bin/env python3
"""Статический source-only audit LFO PAGE/DEST faceplate geometry.

Не открывает editor, не компилирует проект и не запускает plugin/DSP/CTest/render.
Проверяет только active Synth/FX исходники и pixel-font arithmetic.
"""
from __future__ import annotations

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
SYNTH = ROOT / "Monomachine_Nova_Synth" / "Source" / "PluginEditor.cpp"
FX = ROOT / "Monomachine_Nova_FX" / "Source" / "PluginEditor.cpp"
PIXEL = ROOT / "Monomachine_Nova_Synth" / "Source" / "PixelFont.h"

failures: list[str] = []
checks = 0


def require(text: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in text:
        failures.append(f"{label}: не найдено {needle!r}")


def forbid(text: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle in text:
        failures.append(f"{label}: остался устаревший фрагмент {needle!r}")


if not SYNTH.is_file() or not FX.is_file():
    failures.append("PluginEditor.cpp отсутствует в Synth или FX")
else:
    checks += 1
    if SYNTH.read_bytes() != FX.read_bytes():
        failures.append("PluginEditor.cpp не byte-identical между Synth и FX")

editor = SYNTH.read_text(encoding="utf-8")
pixel = PIXEL.read_text(encoding="utf-8")

require(editor, "if(pageIndex>=3&&knob==1){const int lastSpace=compact.lastIndexOfChar(' ');if(lastSpace>=0)compact=compact.substring(lastSpace+1);}",
        "DEST faceplate shows target-local short value")
require(editor, "pixel::text(g,compactValue,{2,getHeight()-26,isDirectLfoDepth()?50:getWidth()-4,23},21,true);",
        "PAGE/DEST faceplate value keeps requested 21px pixel height")
require(editor, "const juce::Rectangle<int> badge{cx+15,cy-17,30,18};",
        "P1/P2 badge moved right and lower with larger bounds")
require(editor, "pixel::text(g,side==1?\"P1\":\"P2\",badge.reduced(2,0),16,true);",
        "P1/P2 badge uses scale-2-capable 16px request")
forbid(editor, "const juce::Rectangle<int> badge{cx+12,cy-20,28,15};",
       "old small/upper-left P1/P2 badge")
forbid(editor, "badge.reduced(2,0),12,true",
       "old scale-1 P1/P2 badge font")
require(editor, "if(pageIndex>=3&&knob==3)", "WAVE choice uses the common cell popup path")
require(editor, "const juce::Rectangle<int> popupAnchor{getScreenX(),getScreenY(),getWidth(),getHeight()};",
        "popup anchor uses exact global cell bounds")
require(editor, "withTargetComponent(this).withTargetScreenArea(popupAnchor)",
        "popup is anchored to its originating cell")
forbid(editor, "const int panelRight=72+", "removed mixed local/screen popup clamp")
forbid(editor, "const int lx=std::max(0,std::min(getScreenX(),panelRight-estW));",
       "removed detached popup x coordinate")
require(pixel, "while(scale>1&&s.length()*6*scale>r.getWidth())--scale;",
        "pixel font width-fit rule is explicit")

# The faceplate has a 91px cell and a 4px horizontal value inset: 87px usable.
# PAGE labels compact to PITCH/SYNT/AMP/FILT/EFFX/LFO1..6; DEST compacting keeps
# a target-local token such as PITCH, 1OCT, MULT, DPTH or a machine parameter ID.
# The longest retained fixed token is PITCH (5 glyphs). At requested height 21,
# PixelFont uses scale=3, whose width is 5*6*3-3 = 87 exactly.
checks += 1
usable_width = 91 - 4
scale = 21 // 7
longest_token = "PITCH"
rendered_width = len(longest_token) * 6 * scale - scale
if scale != 3 or rendered_width > usable_width:
    failures.append("21px PAGE/DEST token no longer fits at scale 3")

# The new 16px badge requests scale=2. Its P2 text is 2*6*2-2 = 22px and the
# inner 30px badge width is 26px, so it cannot collapse to scale 1.
checks += 1
badge_inner_width = 30 - 4
badge_scale = 16 // 7
badge_text_width = 2 * 6 * badge_scale - badge_scale
if badge_scale != 2 or badge_text_width > badge_inner_width:
    failures.append("P1/P2 badge no longer fits at scale 2")

if failures:
    print("LFO FACEPLATE STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"LFO FACEPLATE STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
