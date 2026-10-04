# Статическая проверка RMB mini-panels — 04.10.2026

## Граница проверки

Это **source-only** corrective pass для active revision `1.9.35` в обеих
поставляемых ветках:

- `Monomachine_Nova_Synth/Source/PluginEditor.cpp`;
- `Monomachine_Nova_FX/Source/PluginEditor.cpp`.

Перед изменением создан неизменяемый снимок
`PATCH_HISTORY/SNAPSHOTS/ACTIVE_1.9.35_PRE_RMB_FLOATING_PANEL_REWORK_2026-10-04/`.
В нём сохранены прежние editor sources, релевантные audits, manifests,
README и package manifest с `SHA256SUMS.txt`. Исходный release archive до
этого pass имел SHA-256
`83fde735ca4fdb7b486ccc7b8554daa4a596d3c58d977f0b0e046c47b71eb614`.

Ни compilation, ни CTest, ни DSP/executable test, ни VST3/DAW test, ни audio
render не выполнялись.

## Исправленный общий pin/text contract

`FloatingPanel` теперь содержит явный маркер
`FLOATING_PANEL_FOREGROUND_TEXT_CONTRACT`. Он требует от каждого pinnable
subclass pure-virtual `paintFloatingForeground()`. Базовый final
`paintOverChildren()` вызывает этот foreground после всех child controls и
только затем рисует pin.

Следствие: static parent title/status text больше не может остаться только в
`paint()` и оказаться под child sliders/buttons после pin/front transition.
Новый subclass без foreground implementation не пройдёт compilation, а
`RMB_MINI_PANEL_STATIC_CHECK.py` дополнительно проверяет весь текущий family:

- `DTIM / BPM` (`RepitchSliderPanel`);
- `DLY / FEEDBACK Q`;
- embedded `DFB / BASE + RAW GUARD`;
- `PORTAMENTO`;
- `FILTER EXTRA` (`BASE`/`WDTH`);
- direct-LFO `DPTH SET`;
- `GUI DRAG SPEED`.

Таким образом contract покрывает именно общий family, а не только отдельно
наблюдавшиеся `FILTER BASE`, DFB, DPTH и DTIM. DFB остаётся white embedded
VST/editor overlay; `DocumentWindow` или native detached window не добавлялись.

## Compact direct-LFO DPTH SET

RMB panel direct `DPTH` уменьшен с `324×154` до `228×121`. Header теперь
`P1 LFO<n> SET` / `P2 LFO<n> SET`; прежний `DIRECT DPTH` не рисуется. Убраны
дублирующие строки `0..127`, `127 = FULL` и вторичный DPTH readout: значение
по-прежнему видно в исходном centred LCD основной faceplate control.

Остались только три рабочие persistent settings конкретного LFO:
`UNIPOLAR/BIPOLAR`, `OFF/INV`, `ALT DUAL: ON/OFF`. Их buttons имеют компактную
ширину `120 px`; parameter IDs, APVTS state, automation и стандартная
геометрия главного DPTH knob/LCD не менялись. Все новые functional strings и
tooltips English/ASCII.

## DSND и lifecycle нескольких panel

Obsolete informational RMB panel `DsndPanel`, callback `openDsnd`, opener
`showDsndMenu()` и DSND-specific RMB branch удалены. Сам DSND parameter/knob
не удалён. Его RMB больше не открывает общий DSP-mode menu; English tooltip
направляет к DFB. Реальные `+ FB INVERT` и `- FB INVERT` остаются только в DFB.

Singleton `extraPanel` / `extraKind` заменён collection
`std::vector<ExtraPanelEntry>`:

- pinned panels разных kind остаются одновременно открытыми; это не ограничено
  двумя panel;
- новый unpinned callout по-прежнему закрывает прежний unpinned transient
  callout, но не удаляет pinned entries;
- повторный запрос того же kind остаётся targeted toggle и закрывает только
  его;
- outside click закрывает только unpinned entries;
- DFB close path закрывает только `dfb-guard`;
- переход к dock/полное уничтожение Surface корректно закрывает всю collection.

Это сохраняет normal transient close behavior и позволяет открыть второй RMB
panel при уже pinned panel другого control.

## Выполненные статические проверки

`RMB_MINI_PANEL_STATIC_CHECK.py` прошёл: **68 checks**. Он проверяет:

1. byte-identical Synth/FX editor;
2. named pure-virtual/final foreground contract и единый foreground pin;
3. foreground implementation каждого pinnable class и отсутствие
   `pixel::text`/`drawPin(g)` в его background `paint()` section;
4. compact `DPTH SET` dimensions/header/control widths и отсутствие redundant
   scale text;
5. удаление DSND RMB symbols при сохранении DFB inversion controls;
6. collection ownership, targeted same-kind/DFB close, unpinned-only watcher и
   full cleanup boundary dock/destructor.

Совместимый `DFB_GUARD_CORE_STATIC_CHECK.py` также обновлён на новый
foreground contract и прошёл source-only проверку.

Полный локальный suite прошёл без compilation/runtime работы:

| Проверка | Результат |
|---|---:|
| Python syntax (`py_compile *_STATIC_CHECK.py`) | PASS |
| DFB guard/core | 161 checks PASS (включая RAW=63 UNITY / RAW>=64 guard boundary) |
| ENV held-LMB release | 35 checks PASS (source category не возвращается на release) |
| FM metadata/source manifests | OK |
| LFO timing / faceplate / speed tooltip / locks | 32 / 28 / 35 / 20 PASS |
| JUCE 8 PopupMenu | 16 checks PASS |
| Matrix UI | 111 checks PASS |
| RMB mini-panels | 68 checks PASS |
| `SOURCE_BUILD.json`, Synth + FX | 167 + 167 hashes PASS |
| Mirrored `PluginEditor.cpp` | byte-identical PASS |

Clean archive и его SHA-256 создаются/сообщаются как отдельный delivery record;
этот report намеренно не встраивает собственный archive hash, чтобы не создавать
циклическую зависимость содержимого архива.
