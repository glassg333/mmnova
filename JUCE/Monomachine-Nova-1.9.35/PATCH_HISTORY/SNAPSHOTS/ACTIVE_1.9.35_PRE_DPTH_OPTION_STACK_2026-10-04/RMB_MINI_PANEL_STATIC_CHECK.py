#!/usr/bin/env python3
"""Source-only invariant audit for pinnable RMB mini-panels.

No compiler, executable, plug-in, DSP, DAW or render work is started here.
It protects the two regressions fixed on 2026-10-04:
  * static parent text vanishing after a child-control paint/pin/front transition;
  * a singleton callout owner deleting an already-pinned different panel.
"""
from pathlib import Path
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
        failures.append(f"{label}: missing {needle!r}")


def forbid(haystack: str, needle: str, label: str) -> None:
    global checks
    checks += 1
    if needle in haystack:
        failures.append(f"{label}: forbidden {needle!r} remains")


def class_block(source: str, name: str) -> str:
    marker = "class " + name
    start = source.find(marker)
    if start < 0:
        return ""
    end = source.find("\nclass ", start + len(marker))
    return source[start:] if end < 0 else source[start:end]


synth = SYNTH.read_text(encoding="utf-8")
fx = FX.read_text(encoding="utf-8")
checks += 1
if synth != fx:
    failures.append("Synth/FX PluginEditor.cpp are no longer byte-identical")
editor = synth

# Named, compile-enforced common contract: parent static text must run after
# child controls with white ink restored, and every new FloatingPanel subclass
# must explicitly opt in.
require(editor, "FLOATING_PANEL_FOREGROUND_TEXT_CONTRACT", "durable named foreground marker")
require(editor, "virtual void paintFloatingForeground(juce::Graphics& g)=0;", "pure-virtual foreground contract")
require(editor, "g.setColour(ink);paintFloatingForeground(g);drawPin(g);", "post-child white-ink text + pin compositor")
require(editor, "pixel::text() uses the current Graphics colour.", "foreground colour causality comment")
require(editor, "if(handlePinClick(e))return;", "shared base pin handler")
require(editor, "if(handlePinClick(event))return;", "DFB shared pin handler")

panel_classes = (
    "RepitchSliderPanel",
    "DelayFeedbackFilterPanel",
    "DelayFeedbackSafetyPanel",
    "PortaPanel",
    "FilterExtraPanel",
    "LfoDepthPanel",
    "GuiSpeedPanel",
)
checks += 1
if editor.count("final : public FloatingPanel") != len(panel_classes):
    failures.append("FloatingPanel family count changed: every new member needs an audit update")
for name in panel_classes:
    block = class_block(editor, name)
    require(block, "void paintFloatingForeground(juce::Graphics& g) override", f"{name} foreground implementation")
    paint_start = block.find("void paint(")
    foreground_start = block.find("void paintFloatingForeground", paint_start)
    checks += 1
    if paint_start < 0 or foreground_start < 0:
        failures.append(f"{name}: cannot isolate background paint")
    else:
        background = block[paint_start:foreground_start]
        forbid(background, "pixel::text", f"{name} parent paint has no static pixel labels")
        forbid(background, "drawPin(g)", f"{name} pin uses common foreground compositor")
checks += 1
if editor.count("drawPin(g);") != 1:
    failures.append("drawPin must be emitted once by the common foreground compositor")

# DSND remains a regular parameter. Only its obsolete RMB informational panel is
# removed; DFB retains both actual + / - feedback inversion settings.
for obsolete in ("class DsndPanel", "openDsnd", "showDsndMenu", "\"dsnd\")"):
    forbid(editor, obsolete, "obsolete DSND RMB popup")
require(editor, "if(pageIndex==2&&knob==4){slider.modeMenu={};slider.setTooltip(\"DSND: + / - feedback inversion is configured in the DFB panel.\");}", "DSND RMB route disabled with DFB guidance")
require(class_block(editor, "DelayFeedbackSafetyPanel"), '"+ FB INVERT"', "DFB owns positive feedback inversion")
require(class_block(editor, "DelayFeedbackSafetyPanel"), '"- FB INVERT"', "DFB owns negative feedback inversion")

# Direct-LFO DPTH is a compact SET panel: no redundant modulation scale/readout
# appears here because its centred main faceplate already exposes DPTH.
dpth = class_block(editor, "LfoDepthPanel")
require(dpth, "setSize(228,121);", "compact DPTH SET bounds")
require(dpth, 'title()+" SET"', "concise DPTH SET title")
require(dpth, "mode.setBounds(100,28,120,24);invert.setBounds(100,57,120,24);dual.setBounds(100,86,120,24);", "compact direct-style switch widths")
require(dpth, 'pixel::text(g,"POLARITY"', "DPTH static foreground label")
require(class_block(editor, "RepitchSliderPanel"), 'pixel::text(g,"DTIM / BPM"', "DTIM static foreground title")
filter_extra = class_block(editor, "FilterExtraPanel")
require(filter_extra, 'g.setColour(ink);pixel::text(g,"Manual normal tracking"', "FILTER footer restores white ink after dim separator")
for redundant in ("0..127   127 = FULL", '"DPTH "+text', "DIRECT DPTH"):
    forbid(dpth, redundant, "redundant DPTH popup wording")

# Collection ownership: pinned entries survive a different callout while normal
# transient callouts still close on outside-click / replacement. Same kind is a
# deliberate targeted toggle, and a dock remains a full overlay boundary.
for token, label in (
    ("struct ExtraPanelEntry", "multi-panel entry type"),
    ("std::vector<ExtraPanelEntry> extraPanels", "multi-panel owner collection"),
    ("bool hasExtraPanelKind", "same-kind lookup"),
    ("bool sourceIsInsideExtraPanel", "outside-click membership lookup"),
    ("void closeExtraPanelKind", "targeted kind close"),
    ("void closeUnpinnedExtraPanels", "transient close path"),
    ("void closeExtraPanels", "full owner teardown"),
    ("if(hasExtraPanelKind(kind)){closeExtraPanelKind(kind);return;}", "same-kind toggle"),
    ("closeUnpinnedExtraPanels();", "new callout retains pins"),
    ("if(isPanelPinned(it->panel)){++it;continue;}", "pinned entries survive transient cleanup"),
    ("closeDfbGuardPanel(){closeExtraPanelKind(\"dfb-guard\");}", "DFB targeted lifecycle"),
    ("showDocked(std::unique_ptr<juce::Component> panel,juce::Rectangle<int> localAt,bool belowAnchor,bool keepEnvelopeSelection=false){ closeExtraPanels();", "dock full-overlay cleanup"),
    ("~Surface()override{flushResetParametersForDestruction();removeMouseListener(&rmbWatcher);closeExtraPanels();", "destructor collection cleanup"),
):
    require(editor, token, label)
for obsolete in ("std::unique_ptr<juce::Component> extraPanel", "juce::String extraKind", "closeExtraPanel()"):
    forbid(editor, obsolete, "removed singleton lifecycle")

watcher_start = editor.find("rmbWatcher.onDown=")
watcher_end = editor.find("viewport.setComponentID", watcher_start)
watcher = editor[watcher_start:watcher_end] if watcher_start >= 0 and watcher_end >= 0 else ""
require(watcher, "sourceIsInsideExtraPanel(src)", "watcher ignores every open panel")
require(watcher, "closeUnpinnedExtraPanels();", "watcher keeps pinned panels")
forbid(watcher, "closeExtraPanels();", "watcher does not close pinned collection")

if failures:
    print("RMB MINI-PANEL STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)
print(f"RMB MINI-PANEL STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
