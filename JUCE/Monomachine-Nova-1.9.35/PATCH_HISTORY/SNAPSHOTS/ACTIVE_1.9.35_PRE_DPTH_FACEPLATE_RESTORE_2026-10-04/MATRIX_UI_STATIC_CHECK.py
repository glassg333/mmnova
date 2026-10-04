#!/usr/bin/env python3
"""Статический source-only audit corrective Matrix/LFO UI pass для 1.9.35.

Не компилирует проект, не открывает editor, не запускает plugin/DSP/CTest,
VST3/DAW и не выполняет render. Проверяет только активные Synth/FX source.
"""
from __future__ import annotations

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
TARGETS = ("Monomachine_Nova_Synth", "Monomachine_Nova_FX")

failures: list[str] = []
checks = 0


def require(text: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in text:
        failures.append(f"{label}: не найдено {needle!r}")


def require_count(text: str, needle: str, expected: int, label: str) -> None:
    global checks
    checks += 1
    got = text.count(needle)
    if got != expected:
        failures.append(f"{label}: ожидалось {expected}, получено {got}: {needle!r}")


def forbid(text: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle in text:
        failures.append(f"{label}: остался устаревший фрагмент {needle!r}")


def lexical_balance(text: str) -> tuple[bool, str]:
    """Баланс (), [] и {}, игнорируя C++ comments/string/char literals."""
    state = "code"
    stack: list[tuple[str, int]] = []
    pairs = {")": "(", "]": "[", "}": "{"}
    line = 1
    i = 0
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if state == "code":
            if ch == "/" and nxt == "/":
                state = "line-comment"
                i += 1
            elif ch == "/" and nxt == "*":
                state = "block-comment"
                i += 1
            elif ch == '"':
                state = "string"
            elif ch == "'":
                state = "char"
            elif ch in "([{":
                stack.append((ch, line))
            elif ch in pairs:
                if not stack or stack[-1][0] != pairs[ch]:
                    return False, f"несовпадение {ch!r} на строке {line}"
                stack.pop()
        elif state == "line-comment":
            if ch == "\n":
                state = "code"
        elif state == "block-comment":
            if ch == "*" and nxt == "/":
                state = "code"
                i += 1
        elif state == "string":
            if ch == "\\":
                i += 1
            elif ch == '"':
                state = "code"
        elif state == "char":
            if ch == "\\":
                i += 1
            elif ch == "'":
                state = "code"
        if ch == "\n":
            line += 1
        i += 1
    if state != "code":
        return False, f"незавершённое лексическое состояние {state}"
    if stack:
        return False, f"незакрытый {stack[-1][0]!r} со строки {stack[-1][1]}"
    return True, ""


def source(target: str, name: str) -> Path:
    return ROOT / target / "Source" / name


paths = {
    target: {
        "editor": source(target, "PluginEditor.cpp"),
        "processor": source(target, "PluginProcessor.cpp"),
        "data": source(target, "NovaData.h"),
    }
    for target in TARGETS
}

for target, group in paths.items():
    for kind, path in group.items():
        checks += 1
        if not path.is_file():
            failures.append(f"{target}: отсутствует {kind}: {path.name}")

if failures:
    print("MATRIX UI STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

synth = {kind: path.read_text(encoding="utf-8") for kind, path in paths[TARGETS[0]].items()}
fx = {kind: path.read_text(encoding="utf-8") for kind, path in paths[TARGETS[1]].items()}

for kind in ("editor", "processor", "data"):
    checks += 1
    if synth[kind].encode("utf-8") != fx[kind].encode("utf-8"):
        failures.append(f"Synth/FX {kind} не byte-identical")

for target, group in (("Synth", synth), ("FX", fx)):
    for kind, text in group.items():
        checks += 1
        balanced, explanation = lexical_balance(text)
        if not balanced:
            failures.append(f"{target} {kind}: lexical delimiter audit: {explanation}")

editor = synth["editor"]
processor = synth["processor"]
data = synth["data"]

# Direct-LFO DPTH: c27 default cell starts at 1161,226, so the 43px rotary
# rectangle at 19,29 has its integer centre at 1201,276. Buttons are derived
# from the rectangle and cannot move the control.
require(editor, "const juce::Rectangle<int> directDepthKnob{19,29,43,43};",
        "DPTH возвращён в established c27 geometry (центр 1201,276)")
require(editor, "slider.setBounds(directDepthKnob);",
        "DPTH rotary использует фиксированную rectangle")
require(editor, "const int sideX=directDepthKnob.getRight()-4;",
        "UNI/INV X выводится относительно DPTH")
require(editor, "lfoModeButton.setBounds(sideX,directDepthKnob.getY()+18,sideW,12);",
        "UNI расположен относительно DPTH")
require(editor, "lfoInvButton.setBounds(sideX,directDepthKnob.getY()+34,sideW,12);",
        "INV расположен относительно DPTH")
forbid(editor, "slider.setBounds(11,27,43,43)",
       "не осталась смещённая DPTH geometry")

# Only P1 on P1 is compacted. P2 remains explicit in direct faceplate values,
# menus, Matrix dock labels and canonical Matrix target strings.
require(editor, "if(!sourceIsP2&&full.startsWithIgnoreCase(\"P1 \"))return full.substring(3);",
        "direct DEST list снимает только redundant P1-on-P1 prefix")
require(editor, "if(namedSide==2||namedSide!=localSide)side=namedSide;",
        "faceplate P2 badge остаётся и для P2-on-P2")
require(editor, "const bool redundantP1=!p2Source&&full.startsWithIgnoreCase(\"P1 \");return redundantP1?full.substring(3):full;",
        "PAGE popup сохраняет P2 marker")
require(editor, "return lfo<6&&text.startsWithIgnoreCase(\"P1 \")?text.substring(3):text;",
        "dock compacting ограничен local P1")
forbid(editor, "(localP2&&full.startsWithIgnoreCase(\"P2 \"))",
       "не осталось симметричного скрытия P2 в direct DEST")
forbid(editor, "const auto own=lfo>=6?juce::String(\"P2 \")",
       "не осталось симметричного скрытия P2 в dock")
require_count(editor, "juce::String matrixTargetName(int target) const {return targetName(processor,target);}", 2,
              "Matrix table и dock используют canonical P1/P2 target names")

# P2 DIST/VOL/PAN must identify targets 144..146 on the visible P2 AMP row.
require(editor, "setTargetOverride(144+(i-4));",
        "P2 DIST/VOL/PAN faceplate/patchcord targets = 144..146")
forbid(editor, "cells[1][static_cast<size_t>(i)]->setTargetOverride(132+i);",
       "P2 DIST/VOL/PAN больше не ошибочно указывают P2 SYNT 136..138")
require(data, "if(page<=4)return 140+(page-2)*8+dest;",
        "P2 direct PAGE AMP bank сохраняет ID 140..147")
require(data, "if(target>=132&&target<164)return (target-132)%8;",
        "P2 direct target destination использует локальный bank offset")
require(editor, "juce::PopupMenu p2,ps,pa,pf,pe;addRange(ps,132,8);addRange(pa,140,8);",
        "Matrix destination list сохраняет P2 AMP range 140..147")
require(editor, "for(int k=0;k<nova::kLfoPageChoiceCount;++k)pageNames[static_cast<size_t>(k)]=nova::lfoPageName(p2Source,k);",
        "LFO LOCKS PAGE list сохраняет canonical P2 names")
require(editor, "else rowNames[static_cast<size_t>(k)]=targetName(processor,target);",
        "LFO LOCKS DEST list сохраняет canonical P2 target names")

# Matrix layout and naming: readable DEPTH/POLAR widths, POLAR visible name,
# no display-only INTERNAL source suffix, and a divider above the pinned dock.
require(editor, 'const char* headers[]{"","ON","SOURCE","DESTINATION","DEPTH","POLAR","INV","AUX SOURCE","AUX AMOUNT"};',
        "Matrix header называется POLAR")
forbid(editor, 'const char* headers[]{"","ON","SOURCE","DESTINATION","DEPTH","MODE","INV","AUX SOURCE","AUX AMOUNT"};',
       "старый Matrix MODE header удалён")
require(editor, "int colw[kCols]{26,44,199,198,140,120,58,185,130};",
        "Matrix DEPTH/POLAR columns возвращены к readable widths")
require(editor, "pixel::text(g,routeRaw(r,\"mode\")>0?\"BIPOLAR\":\"UNIPOLAR\",{colX(5)+3,y+4,colWidth(5)-6,22},14,true);",
        "POLAR rows сохраняют readable scale-2 text")
require(editor, 'menu.addItem(3,"ONLY POLAR")',
        "Matrix clear wording следует POLAR header")
require(editor, "g.setColour(juce::Colours::white);g.drawHorizontalLine(0,0.0f,float(getWidth()));",
        "white top divider отделяет LFO dock от Matrix table")
require(editor, 'juce::String(side==0?"P1 LFO":"P2 LFO")',
        "dock headers явно показывают P1/P2 без INTERNAL suffix")
forbid(editor, "P1 LFO1 INTERNAL", "Matrix source name не содержит INTERNAL")
forbid(editor, "P2 LFO1 INTERNAL", "P2 Matrix source name не содержит INTERNAL")
forbid(editor, "P1 INTERNAL LFO", "dock header не содержит INTERNAL")
forbid(editor, "P2 INTERNAL LFO", "P2 dock header не содержит INTERNAL")

# PAGE in the pinned Matrix LFO dock is foldered and preserves raw page IDs.
require(editor, "// PAGE values are unchanged raw IDs; only their presentation is",
        "dock PAGE menu документирует display-only grouping")
require(editor, "juce::PopupMenu p1Param,p1Lfo,p2Param,p2Lfo;",
        "dock PAGE menu создаёт четыре явных P1/P2 folders")
require(editor, "menu.addSubMenu(\"P1 PARAM\",p1Param);menu.addSubMenu(\"P1 LFO\",p1Lfo);",
        "dock PAGE menu включает P1 PARAM/P1 LFO folders")
require(editor, "menu.addSubMenu(\"P2 PARAM\",p2Param);menu.addSubMenu(\"P2 LFO\",p2Lfo);",
        "dock PAGE menu включает P2 PARAM/P2 LFO folders")
require(editor, "for(int value=5;value<=10;++value)addPage(p1Lfo,value);",
        "P1 LFO1..6 pages остаются в P1 LFO folder")
require(editor, "for(int value=5;value<=10;++value)addPage(p2Lfo,value);",
        "P2 LFO1..6 pages остаются в P2 LFO folder")
require(editor, "withPreferredPopupDirection(juce::PopupMenu::Options::PopupDirection::upwards)",
        "dock PAGE/DEST popup requests upward direction")

# Direct DEST uses the common custom row with the same scale-2 pixel request
# and row height as PAGE items; explicit P2 text now gets adequate measured width.
require(editor, "void getIdealSize(int& w,int& h) override {w=juce::jlimit(120,700,text.length()*12+28);h=22;}",
        "hover DEST row сохраняет PAGE-compatible width/22px height")
require(editor, "pixel::text(g,text,{8,1,getWidth()-28,getHeight()-2},14,false);",
        "hover DEST row использует PAGE-compatible 14px request")
require(editor, "const juce::Rectangle<int> popupAnchor{getScreenX(),getScreenY(),getWidth(),getHeight()};",
        "faceplate PAGE/DEST/WAVE popup привязан к исходной cell")

# Factory/default and clear/reset Matrix rows stay ON. Route allocation uses an
# empty payload rather than an OFF checkbox so default ON does not overwrite a
# real route. Target-pick explicitly enables the assigned row.
require(data, 's.push_back({prefix+"on","Route "+juce::String(row+1)+" enable",0,1,1,1,"OFF|ON"});',
        "fresh Matrix ON default = enabled")
require(editor, "const float value=juce::String(f)==\"on\"?1.0f:0.0f;",
        "Matrix ERASE EVERYTHING restores ON instead of OFF")
require(editor, "if(auto* on=processor.parameters.getParameter(\"r\"+juce::String(pickRoute)+\"_on\")){on->beginChangeGesture();on->setValueNotifyingHost(on->convertTo0to1(1.0f));on->endChangeGesture();}",
        "Matrix target pick включает назначенную route")
require_count(processor, "auto vacantUnlocked=[this](int r){", 2,
              "MSEG и patchcord allocation поддерживают default-ON blank routes")
require(processor, "for(int r=0;r<64;++r)if(vacantUnlocked(r)){slot=r;break;}",
        "MSEG allocator сначала берёт blank ON row")
require(processor, "if(slot<0)for(int r=0;r<64;++r)if(vacantUnlocked(r)){slot=r;break;}",
        "patchcord allocator сначала берёт blank ON row")

# Reset queue, ENV handoff and pinned dock lifecycle remain active regression
# guards from the accepted previous pass.
require(editor, "constexpr size_t kResetHostNotificationsPerChunk=12;",
        "RESET ALL retains bounded 12-notification async chunks")
require(editor, "void flushResetParametersForDestruction()",
        "RESET ALL lifecycle flush retained")
require(editor, "if (resetInProgress||undoStack.empty()) return;",
        "undo guard retained during reset queue")
require(editor, "void requestEnvelopeCategory(int tab,bool held)",
        "held-LMB ENV handoff retained")
require(editor, "class MatrixLfoDock final : public juce::Component, private juce::Timer",
        "persistent bottom-pinned MatrixLfoDock retained")
require(editor, "matrixLfoDock->setBounds(viewport.getX()+8,viewport.getBottom()-height,width,height)",
        "dock remains pinned to Matrix viewport bottom")
require(editor, "if(matrixLfoDock)matrixLfoDock->setVisible(false);",
        "dock still hides during Matrix faceplate aim")
require(editor, "item.customComponent=new HoverTargetMenuItem",
        "JUCE 8 PopupMenu Item helper retained")
forbid(editor, "matrixPage->openLfoDepth", "obsolete MatrixPage LFO-depth ownership")
forbid(editor, "drawInternalGroups", "obsolete scrolling internal-LFO groups")

# Keep the intended table total within the 1145px Matrix content width.
checks += 1
matrix_widths = (26, 44, 199, 198, 140, 120, 58, 185, 130)
table_right = 13 + sum(matrix_widths) + 8 * 4
if table_right != 1145:
    failures.append(f"Matrix column arithmetic changed: tableRight={table_right}, ожидалось 1145")

if failures:
    print("MATRIX UI STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"MATRIX UI STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
