#!/usr/bin/env python3
"""Статический source-only аудит фикса ENV category held-LMB release.

Проверяет, что реальный drag маскирует originating Button::onClick на mouseUp,
поэтому AMP/FIL ENV/MOD ENV остаётся на последней пересечённой категории, а не
возвращается в категорию начала жеста. Не запускает сборку, CTest, DSP, UI,
VST3/DAW или рендер.
"""
from __future__ import annotations

from pathlib import Path
import hashlib
import json
import sys

ROOT = Path(__file__).resolve().parent
TARGETS = ("Monomachine_Nova_Synth", "Monomachine_Nova_FX")

checks = 0
failures: list[str] = []

def require(text: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle not in text:
        failures.append(f"{label}: не найдено {needle!r}")

def require_order(text: str, needles: tuple[str, ...], label: str) -> None:
    global checks
    checks += 1
    pos = -1
    for needle in needles:
        next_pos = text.find(needle, pos + 1)
        if next_pos < 0:
            failures.append(f"{label}: не найдено {needle!r}")
            return
        pos = next_pos

editors: dict[str, str] = {}
for target in TARGETS:
    root = ROOT / target
    editor_path = root / "Source" / "PluginEditor.cpp"
    editor = editor_path.read_text(encoding="utf-8")
    editors[target] = editor

    pixel_start = editor.find("class PixelButton : public juce::TextButton")
    pixel_end = editor.find("// 1.6.19: MSEG OUT", pixel_start)
    pixel = editor[pixel_start:pixel_end]
    require(pixel, "if(heldDrag&&e.getDistanceFromDragStart()>3){wasDragged=true;heldDrag(e.getScreenPosition());}",
            f"{target}: held-drag threshold")
    require(pixel, "const bool suppressReleaseClick=wasDragged;",
            f"{target}: release suppression flag")
    require(pixel, "auto savedOnClick=std::move(onClick);onClick={};",
            f"{target}: originating onClick masked")
    require(pixel, "juce::TextButton::mouseUp(e);",
            f"{target}: Button release cleanup retained")
    require(pixel, "onClick=std::move(savedOnClick);wasDragged=false;",
            f"{target}: originating onClick restored after cleanup")
    require(pixel, "void clicked()override{if(wasDragged)return;juce::TextButton::clicked();}",
            f"{target}: virtual click guard retained")
    require_order(pixel, (
        "if(heldDrag&&e.getDistanceFromDragStart()>3){wasDragged=true;heldDrag(e.getScreenPosition());}",
        "const bool suppressReleaseClick=wasDragged;",
        "auto savedOnClick=std::move(onClick);onClick={};",
        "juce::TextButton::mouseUp(e);",
        "onClick=std::move(savedOnClick);wasDragged=false;",
    ), f"{target}: drag-to-release commit order")

    require(editor, "ampButton.heldDrag=envCategoryDrag;filterEnvButton.heldDrag=envCategoryDrag;modEnvButton.heldDrag=envCategoryDrag;",
            f"{target}: compact AMP/FIL/MOD handoff")
    require(editor, "if(ampButton.getScreenBounds().contains(screen))showEnvelopeMini(MiniEnvelopeKind::Amp,0,false);",
            f"{target}: compact AMP destination")
    require(editor, "else if(filterEnvButton.getScreenBounds().contains(screen))showEnvelopeMini(MiniEnvelopeKind::Filter,0,false);",
            f"{target}: compact FIL destination")
    require(editor, "else if(modEnvButton.getScreenBounds().contains(screen))showEnvelopeMini(MiniEnvelopeKind::Mod,0,false);",
            f"{target}: compact MOD destination")
    require(editor, "void requestEnvelopeCategory(int tab,bool held)",
            f"{target}: retained large-page handoff")
    require(editor, "if(safe==nullptr||safe->envelopeHandoffSerial!=serial||safe->envelopeCategory==tab)return;",
            f"{target}: stale large-page release rejection")
    require(editor, "const int wantedSelection=static_cast<int>(wanted)-1;",
            f"{target}: compact target selection")

    config = (root / "Source" / "NovaConfig.h").read_text(encoding="utf-8")
    require(config, "ENV held-LMB release commit", f"{target}: visible ENV build marker")

    manifest = json.loads((root / "SOURCE_BUILD.json").read_text(encoding="utf-8"))
    checks += 1
    if "ENV held-LMB release commit 04.10.2026:" not in manifest.get("build", ""):
        failures.append(f"{target}: SOURCE_BUILD ENV release metadata отсутствует")
    expected = manifest["files"].get("Source/PluginEditor.cpp")
    checks += 1
    actual = hashlib.sha256(editor_path.read_bytes()).hexdigest()
    if expected != actual:
        failures.append(f"{target}: SOURCE_BUILD hash PluginEditor.cpp не совпадает")

checks += 1
if editors[TARGETS[0]].encode() != editors[TARGETS[1]].encode():
    failures.append("Synth/FX: PluginEditor.cpp не byte-identical")

if failures:
    print("ENV CATEGORY HELD-LMB STATIC AUDIT: FAIL")
    for failure in failures:
        print("- " + failure)
    sys.exit(1)

print(f"ENV CATEGORY HELD-LMB STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
