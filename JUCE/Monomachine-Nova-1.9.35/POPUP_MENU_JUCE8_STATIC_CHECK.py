#!/usr/bin/env python3
"""Статический source-only аудит JUCE 8 PopupMenu custom-item compatibility.

Не компилирует проект и не запускает editor, plugin, DSP, CTest, DAW или render.
Проверяет текст active Synth/FX editor source после C2665 из Windows MSBuild:
JUCE 8 больше не принимает enabled/ticked bool после addCustomItem().
"""
from __future__ import annotations

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent
SYNTH = ROOT / "Monomachine_Nova_Synth" / "Source" / "PluginEditor.cpp"
FX = ROOT / "Monomachine_Nova_FX" / "Source" / "PluginEditor.cpp"

failures: list[str] = []
checks = 0


def require(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in haystack:
        failures.append(f"{label}: не найдено {needle!r}")


def require_no_legacy_call(haystack: str) -> None:
    global checks
    checks += 1
    if re.search(r"\b(?:menu|where)\.addCustomItem\s*\(", haystack):
        failures.append("остался legacy вызов menu/where.addCustomItem(...)")


checks += 1
if not SYNTH.is_file() or not FX.is_file():
    failures.append("PluginEditor.cpp отсутствует в Synth или FX")
elif SYNTH.read_bytes() != FX.read_bytes():
    failures.append("PluginEditor.cpp не byte-identical между Synth и FX")

editor = SYNTH.read_text(encoding="utf-8")
require(editor, "class HoverTargetMenuItem final : public juce::PopupMenu::CustomComponent",
        "сохранён custom hover renderer")
require(editor, "static void addHoverTargetItem(juce::PopupMenu& menu,int itemId,juce::String itemText,int target,bool enabled,bool selected,std::function<void(int)> onEnter)",
        "введён единый JUCE 8-compatible custom-item helper")
require(editor, "juce::PopupMenu::Item item;item.itemID=itemId;item.text=itemText;item.isEnabled=enabled;item.isTicked=selected;",
        "helper переносит ID, accessibility text, enabled и ticked в PopupMenu::Item")
require(editor, "item.customComponent=new HoverTargetMenuItem(std::move(itemText),target,selected,std::move(onEnter));",
        "helper сохраняет custom hover/preview component")
require(editor, "menu.addItem(std::move(item));", "helper использует поддерживаемый Item API")
require_no_legacy_call(editor)

# All four sites from the Windows C2665 report must use the helper.
require(editor, "addHoverTargetItem(menu,i+1,choiceName(i),target,lfoChoiceAllowed(i),selected,[preview]",
        "direct-LFO DEST popup keeps availability and selected state")
require(editor, "addHoverTargetItem(menu,d+1,target==nova::kPitchMatrixTarget?\"PITCH \"+juce::String(d+1):localTargetName(lfo,target),target,true,d==cur,[safe]",
        "Matrix LFO destination popup keeps pitch ranges, P1-only compact text and hover preview")
require(editor, "addHoverTargetItem(menu,9000,\"OFF\",-1,true,cur==0,[safe]",
        "Matrix destination OFF row uses Item API")
require(editor, "addHoverTargetItem(where,10000+target,matrixTargetName(target),target,true,cur==target+1,[safe]",
        "Matrix destination target rows use Item API")
checks += 1
if editor.count("addHoverTargetItem(") != 5:  # helper declaration + four call sites
    failures.append(f"ожидалось 5 упоминаний addHoverTargetItem, найдено {editor.count('addHoverTargetItem(')}")

require(editor, "const auto preview=lfoDestinationPreview;", "direct-LFO hover callback сохранён")
require(editor, "if(safe)safe->setDestinationPreview(t,true);", "Matrix hover preview сохранён")
require(editor, "lfoChoiceAllowed(i)", "blocked direct-LFO choices по-прежнему disabled")
require(editor, "nova::lfoDirectTarget", "canonical target mapping не изменён")

if failures:
    print("POPUP MENU JUCE8 STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"POPUP MENU JUCE8 STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
