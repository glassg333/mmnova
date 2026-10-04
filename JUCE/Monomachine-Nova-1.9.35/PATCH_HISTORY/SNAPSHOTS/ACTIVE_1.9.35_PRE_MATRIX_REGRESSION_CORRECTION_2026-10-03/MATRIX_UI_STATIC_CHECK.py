#!/usr/bin/env python3
"""Статический source-only audit Matrix UI follow-up для 1.9.35.

Не компилирует проект, не открывает editor, не запускает plugin/DSP/CTest,
VST3/DAW и не выполняет render. Проверяет только активные тексты исходников
Synth и FX после зеркального UI-прохода.
"""
from __future__ import annotations

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
SYNTH = ROOT / "Monomachine_Nova_Synth" / "Source" / "PluginEditor.cpp"
FX = ROOT / "Monomachine_Nova_FX" / "Source" / "PluginEditor.cpp"

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


if not SYNTH.is_file() or not FX.is_file():
    failures.append("PluginEditor.cpp отсутствует в Synth или FX")
    print("MATRIX UI STATIC AUDIT: FAIL")
    print("- PluginEditor.cpp отсутствует в Synth или FX")
    sys.exit(1)

synth_bytes = SYNTH.read_bytes()
fx_bytes = FX.read_bytes()
editor = synth_bytes.decode("utf-8")

checks += 1
if synth_bytes != fx_bytes:
    failures.append("Synth/FX PluginEditor.cpp не byte-identical")

for label, text in (("Synth", editor), ("FX", fx_bytes.decode("utf-8"))):
    checks += 1
    balanced, explanation = lexical_balance(text)
    if not balanced:
        failures.append(f"{label}: lexical delimiter audit: {explanation}")

# Fixed direct-LFO faceplate geometry from the supplied reference: UNI/INV use
# a compact side strip; DPTH retains its established knob/hit-area geometry.
require(editor, "slider.setBounds(11,27,43,43)",
        "DPTH не сдвигает и не меняет исходную rotary geometry")
require(editor, "lfoModeButton.setBounds(getWidth()-38,47,35,12)",
        "UNI находится в компактной боковой зоне")
require(editor, "lfoInvButton.setBounds(getWidth()-38,63,35,12)",
        "INV находится под UNI в компактной боковой зоне")

# General reset must avoid a begin/value/end triplet for every APVTS parameter
# and yield its potentially thousands of changed values in bounded host-call chunks.
require(editor, "if(std::abs(parameter->getValue()-def)>0.000001f)resetParameters.push_back(parameter);",
        "RESET ALL ставит в очередь только реально отличающиеся от default параметры")
require(editor, "constexpr size_t kResetHostNotificationsPerChunk=12;",
        "RESET ALL ограничивает один async turn двенадцатью host notifications")
require(editor, "void scheduleResetParameterChunk()",
        "RESET ALL имеет отдельный scheduler очереди")
require(editor, "juce::MessageManager::callAsync([safe,serial]",
        "RESET ALL отдаёт следующий chunk message loop")
require(editor, "safe->applyResetParameterChunk();",
        "RESET ALL scheduler применяет bounded chunk")
require(editor, "void flushResetParametersForDestruction()",
        "закрытие editor завершает уже начатый RESET ALL")
require(editor, "~Surface()override{flushResetParametersForDestruction();",
        "destruction не оставляет queued RESET ALL частично применённым")
require(editor, "if (resetInProgress||undoStack.empty()) return;",
        "undo не пересекается с активной reset queue")
forbid(editor, "parameter->beginChangeGesture();parameter->setValueNotifyingHost(def);parameter->endChangeGesture();",
       "устранён per-parameter gesture storm RESET ALL")

# All three large envelope pages use one held-LMB handoff path and Surface
# serialises the asynchronous component replacement.
require(editor, "ampTab.heldDrag=handoff;filTab.heldDrag=handoff;modTab.heldDrag=handoff;",
        "AMP/FIL/MOD ENV tabs получают held-LMB handoff")
require(editor, "void requestEnvelopeCategory(int tab,bool held)",
        "Surface coalesces tab handoff")
require(editor, "safe->envelopeHandoffSerial!=serial",
        "stale held-tab callback is cancelled")

# Matrix table: grouped source menus, readable type, AUX purpose and 16..64 rows.
require(editor, "class MatrixPage final : public juce::Component, private juce::Timer",
        "новая MatrixPage присутствует")
require(editor, 'const char* headers[]{"","ON","SOURCE","DESTINATION","DEPTH","MODE","INV","AUX SOURCE","AUX AMOUNT"};',
        "Matrix headers включают отдельные AUX SOURCE/AUX AMOUNT")
require(editor, 'pixel::text(g,"MODULATION MATRIX",{10,3,400,24},20);',
        "Matrix title использует читаемый 20px request")
require(editor, 'headers[c],{colX(c),30,colWidth(c),19},14,true',
        "Matrix headers используют единый 14px request")
require(editor, 'addSource(menu,17); // RANDOM remains immediately available.',
        "RANDOM сразу доступен в SOURCE/AUX SOURCE")
require(editor, 'menu.addSubMenu("MIDI",midi);menu.addSubMenu("LFO",lfo);menu.addSubMenu("P2 LFO",p2l);menu.addSubMenu("ARP",arp);menu.addSubMenu("MSEG",mseg);menu.addSubMenu("MACRO",macro);menu.addSubMenu("MOD ENV",modenv);',
        "SOURCE/AUX SOURCE возвращают foldered menus")
require(editor, 'bool routeHasVisibleState(int r) const',
        "видимость Matrix rows учитывает полный route state")
require(editor, 'const int want=juce::jlimit(16,64,maxUsed+2);',
        "Matrix сохраняет dynamic 16..64 row range")
forbid(editor, "RMB DPTH", "misleading AUX RMB wording")

# Matrix and direct-LFO DPTH visuals follow UNI/BI/INV instead of using a
# permanently one-sided bar. AUX remains signed without pretending to be RMB.
require(editor, 'drawSignedGraph(g,{colX(4)+4,y+3,colWidth(4)-8,25},routeRaw(r,"depth"),routeRaw(r,"mode")>0,routeRaw(r,"inv")>0);',
        "route DEPTH graph получает MODE и INV")
require(editor, "if(bipolar){g.fillRect(mid-fw,r.getY(),fw,r.getHeight());g.fillRect(mid,r.getY(),fw,r.getHeight());}",
        "Matrix BI graph заполняет обе стороны")
require(editor, 'signedPercent(reversed?-raw:raw)',
        "Matrix UNI text отражает INV direction")
require(editor, "if(bipolar){g.fillRect(mid-fill,r.getY(),fill,r.getHeight());g.fillRect(mid,r.getY(),fill,r.getHeight());}",
        "direct-LFO BI graph заполняет обе стороны")
require(editor, "if(inverted)positive=!positive", "direct-LFO UNI graph учитывает INV")

# Destination labels must use raw DEST -> zero-based target conversion exactly once.
require(editor, "juce::String destName(int target) const", "Matrix destination helper принимает target ID")
forbid(editor, "const int id=stored-1", "устранён destination one-off shift")

# The internal-LFO dock is a Surface sibling, anchored to the Matrix viewport
# bottom; expanded content and menus open upwards. Its side-local labels remove
# redundant P1/P2 names but preserve cross-page indicators.
require(editor, "class MatrixLfoDock final : public juce::Component, private juce::Timer",
        "постоянный MatrixLfoDock присутствует")
require(editor, "std::unique_ptr<MatrixLfoDock> matrixLfoDock", "Surface владеет MatrixLfoDock")
require(editor, "matrixLfoDock=std::make_unique<MatrixLfoDock>(processor)", "dock создаётся в Matrix mode")
require(editor, "addAndMakeVisible(*matrixLfoDock)", "dock является sibling overlay Surface")
require(editor, "matrixPage.reset();matrixLfoDock.reset()", "dock уничтожается вместе с Matrix overlay")
require(editor, "matrixLfoDock->setBounds(viewport.getX()+8,viewport.getBottom()-height,width,height)",
        "dock pinned к нижней границе Matrix viewport")
require(editor, "int preferredHeight() const noexcept{return kHeaderH+(openSide>=0?kColumnHeaderH+6*kRowH:0);}",
        "expanded P1/P2 group grows six rows upward")
require(editor, "withPreferredPopupDirection(juce::PopupMenu::Options::PopupDirection::upwards)",
        "dock PAGE/DEST menus request upwards direction")
require(editor, "screen+juce::Point<int>(0,-170)", "dock DPTH setup panel is placed upward")
require(editor, "if(matrixLfoDock)matrixLfoDock->setVisible(false);", "dock cannot cover a faceplate aim target")
require(editor, "juce::String localPageName(int lfo,int page)", "dock strips local P1/P2 PAGE prefix")
require(editor, "juce::String localTargetName(int lfo,int target)", "dock strips local P1/P2 DEST prefix")

# Target preview and JUCE 8 custom PopupMenu Item use SafePointers.
require(editor, "const juce::Component::SafePointer<MatrixPage> safe(this)",
        "Matrix async menu callbacks retain SafePointer")
require(editor, "const juce::Component::SafePointer<MatrixLfoDock> safe(this)",
        "dock async menu callbacks retain SafePointer")
require(editor, "item.customComponent=new HoverTargetMenuItem", "JUCE 8 Item custom component API retained")
forbid(editor, "matrixPage->openLfoDepth", "obsolete MatrixPage LFO-depth ownership")
forbid(editor, "drawInternalGroups", "obsolete scrolling internal-LFO groups")

if failures:
    print("MATRIX UI STATIC AUDIT: FAIL")
    for failure in failures:
        print(f"- {failure}")
    sys.exit(1)

print(f"MATRIX UI STATIC AUDIT: PASS ({checks} checks; source-only, no build/test/render run)")
