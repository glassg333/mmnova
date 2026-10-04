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

require(editor, "const bool directDest=pageIndex>=3&&knob==1;",
        "DEST получает отдельный faceplate-only compact path")
require(editor, "if(directDest){const int lastSpace=full.lastIndexOfChar(' ');return lastSpace>=0?full.substring(lastSpace+1):full;}",
        "P1 DEST AMP PORT / LFO1 DPTH сокращается до leaf value")
require(editor, "if(directDest){const int lastSpace=compact.lastIndexOfChar(' ');if(lastSpace>=0)compact=compact.substring(lastSpace+1);}",
        "P2/cross-page DEST также сохраняет leaf value")
forbid(editor, "if(namedSide==0)return full;",
       "P1 own DEST больше не оставляет AMP PORT/LFO1 DPTH на faceplate")
require(editor, "if(!sourceIsP2&&full.startsWithIgnoreCase(\"P1 \"))return full.substring(3);",
        "DEST popup removes only redundant P1-on-P1 prefix")
require(editor, "if(namedSide==2||namedSide!=localSide)side=namedSide;",
        "P2 badge remains visible for P2-on-P2 and cross-page destination")
require(editor, "const bool redundantP1=!p2Source&&full.startsWithIgnoreCase(\"P1 \");return redundantP1?full.substring(3):full;",
        "PAGE popup keeps local P2 identity")
forbid(editor, "(localP2&&full.startsWithIgnoreCase(\"P2 \"))",
       "old symmetric P2 prefix suppression")
require(editor, "const bool directLfoChoiceValue=pageIndex>=3&&(knob==0||knob==1);",
        "direct PAGE/DEST получает отдельный LCD width contract")
require(editor, "const int valueX=directLfoChoiceValue?0:2;",
        "direct PAGE/DEST начинается от full-cell x=0")
require(editor, "const int valueW=directLfoChoiceValue?getWidth():getWidth()-4;",
        "direct PAGE/DEST получает 91px, DPTH сохраняет PAN 87px inset")
require(editor, "pixel::text(g,compactValue,{valueX,getHeight()-26,valueW,23},21,true);",
        "PAGE/DEST и DPTH используют ожидаемый centred 21px LCD path")
require(editor, "const juce::Rectangle<int> directDepthKnob{getWidth()/2-26,27,52,43};",
        "DPTH совпадает с исходной PAN-equivalent geometry")
forbid(editor, "isDirectLfoDepth()?50:getWidth()-4",
       "DPTH LCD больше не сужен/смещён")
forbid(editor, "const juce::Rectangle<int> directDepthKnob{19,29,43,43};",
       "промежуточная DPTH geometry удалена")
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

# Direct PAGE/DEST deliberately receives the whole 91px cell. PixelFont's
# conservative fitting test uses glyph_count * 6 * scale (not the final
# rendered width with its trailing-scale subtraction), so PITCH needs 90px to
# remain at scale 3. The ordinary 87px inset remains for DPTH/PAN-like values.
checks += 1
direct_choice_width = 91
scale = 21 // 7
longest_token = "PITCH"
fit_width = len(longest_token) * 6 * scale
rendered_width = fit_width - scale
if scale != 3 or fit_width > direct_choice_width or rendered_width != 87:
    failures.append("PITCH PAGE no longer fits at scale 3 in the full-cell LCD")

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
