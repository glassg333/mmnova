# Статическая проверка ENV category held-LMB release — 04.10.2026

## Наблюдаемая ошибка

При удержании ЛКМ и переходе курсором между категориями `AMP`, `FIL ENV` и
`MOD ENV` target category уже правильно выбиралась во время drag. Однако после
release исходная категория могла открыться повторно: базовый `juce::Button`
диспатчит `onClick` во время `mouseUp` отдельно от virtual `clicked()`. Поэтому
одного прежнего `if (wasDragged) return` в `PixelButton::clicked()` было
недостаточно.

## Active fix

`PixelButton::mouseUp()` теперь при любом реальном drag:

1. сохраняет исходный `onClick`;
2. временно очищает его;
3. вызывает normal `TextButton::mouseUp()` только для корректного release/state
   cleanup;
4. восстанавливает callback после cleanup.

Таким образом release не может повторно вызвать source `AMP`, `FIL ENV` или
`MOD ENV`; выбранной остаётся последняя пересечённая category. Обычный click
без drag сохраняет прежнее действие. Arrow gestures и другие real-drag controls
также не превращаются в click на release — это соответствует прежнему намерению
`wasDragged`.

Проверяются оба пути category handoff:

- compact main-surface `AMP` / `FIL ENV` / `MOD ENV` через `heldDrag`;
- retained large AMP/FIL/MOD envelope tabs через serialised
  `requestEnvelopeCategory()`.

IDs, APVTS/state, envelope DSP, RMB large-page access, DFB, Matrix, layout и
визуальная геометрия не менялись.

## Статическая защита от регрессии

`ENV_CATEGORY_HELD_LMB_STATIC_CHECK.py` проверяет release-mask, порядок
`drag → mask → Button cleanup → restore`, оба category paths, Synth/FX
byte-identical `PluginEditor.cpp` и `SOURCE_BUILD` hash.

Выполненный result: **35 checks PASS**. Дополнительно
`FM_MODE_CLEANUP_STATIC_CHECK.py` и общий UI static suite прошли после
обновления manifest. Не заявляются compilation, executable/UI/DSP test,
CTest, VST3/DAW validation или audio render.
