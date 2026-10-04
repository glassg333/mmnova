# Статический аудит совместимости JUCE 8 PopupMenu — 1.9.35

Дата: 03.10.2026.  
Тип проверки после Windows build feedback: только source/static audit; повторная
сборка, VST3/DAW, CTest, DSP test и render в этой среде не запускались.

## Полученный build feedback

Пользовательская Release x64 сборка через Projucer/MSBuild с `C:\JUCE\modules`
нашла одинаковую ошибку MSVC C2665 в Synth и FX:

```text
juce::PopupMenu::addCustomItem: no overloaded function can convert all argument types
```

Она затронула четыре active call sites `PluginEditor.cpp`: direct-LFO `DEST`
popup, internal-LFO Matrix destination popup, `OFF` row Matrix `DEST` и обычные
Matrix target rows. Поэтому MSBuild успел собрать `VST3ManifestHelper`, но
`SharedCode` не собрался и итоговый `.vst3` отсутствовал — строка launcher о
«успешной цели» не является успешной сборкой плагина.

Причина не относится к последнему P2 spacer или LFO timing: старый стиль
`addCustomItem(id, component, enabled, ticked)` несовместим с JUCE 8, где
`addCustomItem()` больше не принимает эти два bool после custom component.

## Исправление

`HoverTargetMenuItem` сохранён: он всё ещё отвечает за pixel paint, selected
mark и hover target preview. Добавлен единый helper `addHoverTargetItem()`:

1. создаёт `juce::PopupMenu::Item`;
2. переносит в него исходные `itemID`, English fallback text, `isEnabled` и
   `isTicked`;
3. назначает тот же `HoverTargetMenuItem` в `customComponent`;
4. добавляет Item через JUCE 8-supported `menu.addItem(std::move(item))`.

Все четыре sites переведены на helper. Для direct-LFO DEST по-прежнему передаётся
`lfoChoiceAllowed(i)`; Matrix preview callbacks, `OFF`, selected state, raw item
IDs и canonical `lfoDirectTarget` mapping не менялись. Нет изменения routing,
state, DSP, LFO locks или P1/P2 mappings.

## Выполненный source-only audit

Запущены только текстовые проверки:

```bash
python POPUP_MENU_JUCE8_STATIC_CHECK.py
python LFO_LOCK_ALIGNMENT_STATIC_CHECK.py
python LFO_SPEED_TOOLTIP_STATIC_CHECK.py
python LFO_FACEPLATE_STATIC_CHECK.py
python LFO_TIMING_STATIC_CHECK.py
```

Результаты:

```text
POPUP MENU JUCE8 STATIC AUDIT: PASS (16 checks; source-only, no build/test/render run)
LFO LOCK ALIGNMENT STATIC AUDIT: PASS (20 checks; source-only, no build/test/render run)
LFO SPEED TOOLTIP STATIC AUDIT: PASS (35 checks; source-only, no build/test/render run)
LFO FACEPLATE STATIC AUDIT: PASS (15 checks; source-only, no build/test/render run)
LFO TIMING STATIC AUDIT: PASS (32 checks; source-only, no build/test/render run)
```

Новый audit подтверждает byte-identical Synth/FX editor source, отсутствие
legacy `menu/where.addCustomItem(...)` invocation, Item-based перенос
`enabled/ticked`, custom hover component и все четыре заменённые sites.

Следующий runtime gate — повторить пользовательскую Windows Release x64 VST3
сборку на исправленном архиве. До этого нельзя заявлять успешный compile или
готовый VST3.
