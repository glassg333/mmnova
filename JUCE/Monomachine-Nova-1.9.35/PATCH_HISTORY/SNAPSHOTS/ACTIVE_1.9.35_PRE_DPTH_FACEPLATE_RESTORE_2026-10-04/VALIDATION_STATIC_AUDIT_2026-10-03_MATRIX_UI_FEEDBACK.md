# Статический corrective audit Matrix/LFO feedback — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только source/static audit. Сборка, открытие editor/plugin,
DSP/CTest, VST3/DAW и UI/audio render намеренно не выполнялись.

## Сохранение предыдущего состояния

Перед corrective проходом active файлы и прежние документы сохранены без
перезаписи в:

- `PATCH_HISTORY/SNAPSHOTS/ACTIVE_1.9.35_PRE_MATRIX_REGRESSION_CORRECTION_2026-10-03/`.

В snapshot лежат SHA-256 для исходных `PluginEditor.cpp`,
`PluginProcessor.cpp`, `NovaData.h`, прежних Matrix/LFO audits и документов.
Это историческая точка до исправления runtime feedback; active Synth/FX source
после неё зеркально синхронизируется заново.

## Исправленные feedback-регрессии

1. Direct-LFO `DPTH` возвращён в default cell `c27` rectangle
   `{19,29,43,43}`. При faceplate grid это даёт rotary centre `(1201,276)`.
   `UNI` и `INV` выводятся из той же rectangle (`sideX`, `Y+18`, `Y+34`),
   поэтому больше не могут сдвигать или менять размер ручки.
2. Убрана только redundant индикация **P1 на собственной P1 стороне**.
   `P2` остаётся явно видимым:
   - на P2 PAGE/DEST faceplate icon через badge;
   - в PAGE/DEST popup names;
   - в `LFO LOCKS` PAGE/DEST lists;
   - в Matrix table, Matrix source names и Matrix LFO dock.
   Cross-page P1 marker на P2 также остаётся, потому что он описывает связь,
   а не повторяет локальную сторону.
3. Matrix table использует canonical `targetName()` для visible P1/P2 target
   names. Source names теперь `P1 LFO1..6` / `P2 LFO1..6` — без ложного
   suffix `INTERNAL`.
4. Dock headers переименованы в `P1 LFO` / `P2 LFO`; перед dock рисуется
   постоянный белый верхний divider. Это визуально отделяет pinned controls
   от scrolling Matrix table даже в collapsed состоянии.
5. Matrix dock `PAGE` больше не flat: raw IDs не меняются, но menu явно
   раскладывается по четырём folders `P1 PARAM`, `P1 LFO`, `P2 PARAM`,
   `P2 LFO`. В LFO folders остаются страницы LFO1–LFO6. Popup по-прежнему
   запрашивает JUCE direction `upwards`.
6. Matrix table header теперь `POLAR`, а не `MODE`. Widths перераспределены
   в `SOURCE=199`, `DESTINATION=198`, `DEPTH=140`, `POLAR=120`, без выхода за
   fixed 1145 px content width. Это сохраняет readable pixel scale для
   signed DEPTH graph и `UNIPOLAR`/`BIPOLAR`.
7. P2 faceplate `DIST`, `VOL`, `PAN` восстановлены как targets `144..146`.
   Ошибочный override `136..138` указывал на P2 SYNT. Matrix target menu
   сохраняет P2 AMP bank `140..147`; direct P2 `PAGE/DEST` оставляет
   page-local formula `140+(page-2)*8+dest` и inverse local-bank offsets.
8. Fresh Matrix route `ON` уже имеет APVTS default `1`; corrective pass также
   сделал `ERASE EVERYTHING` reset-ом к `ON=1`, а Matrix target-pick включает
   выбранную row. Поскольку `DEPTH=0` и `DEST=OFF` инертны, это не создаёт
   слышимой модуляции. MSEG и patchcord allocation теперь находят truly blank
   unlocked payload, а не ищут только `ON=OFF`; поэтому default-ON rows не
   заставляют assignment перезаписывать последний slot.
9. Direct-LFO `DEST` popup продолжает использовать тот же scale-2 pixel
   contract, что PAGE (`14 px`, `22 px` row, measured width). Вместе с
   сохранёнными P2 names это устраняет потерю контекста для P2 назначения.
10. Принятые ранее ограничения сохранены: RESET ALL queue остаётся bounded
    (`12` host notifications на async turn), ENV held-LMB handoff остаётся
    serialised, dock остаётся sibling Surface / bottom-pinned и скрывается
    во время Matrix aim; JUCE 8 `PopupMenu::Item` helper сохраняется.

## Выполненный final source-only audit

Запущены только статические проверки:

```bash
python3 -m py_compile *_STATIC_CHECK.py
python3 DFB_GUARD_CORE_STATIC_CHECK.py
python3 FM_MODE_CLEANUP_STATIC_CHECK.py
python3 LFO_TIMING_STATIC_CHECK.py
python3 LFO_FACEPLATE_STATIC_CHECK.py
python3 LFO_SPEED_TOOLTIP_STATIC_CHECK.py
python3 LFO_LOCK_ALIGNMENT_STATIC_CHECK.py
python3 POPUP_MENU_JUCE8_STATIC_CHECK.py
python3 MATRIX_UI_STATIC_CHECK.py
```

Все завершились успешно:

```text
STATIC AUDIT: PASS (148 checks; source-only, no build/test/render run)
FM MODE CLEANUP STATIC CHECK: OK
LFO TIMING STATIC AUDIT: PASS (32 checks; source-only, no build/test/render run)
LFO FACEPLATE STATIC AUDIT: PASS (19 checks; source-only, no build/test/render run)
LFO SPEED TOOLTIP STATIC AUDIT: PASS (35 checks; source-only, no build/test/render run)
LFO LOCK ALIGNMENT STATIC AUDIT: PASS (20 checks; source-only, no build/test/render run)
POPUP MENU JUCE8 STATIC AUDIT: PASS (16 checks; source-only, no build/test/render run)
MATRIX UI STATIC AUDIT: PASS (73 checks; source-only, no build/test/render run)
```

`MATRIX_UI_STATIC_CHECK.py` проверяет Synth/FX byte identity `PluginEditor.cpp`,
`PluginProcessor.cpp` и `NovaData.h`, lexical delimiters, exact DPTH geometry,
P2 marker/list paths, P2 DIST/VOL/PAN IDs, canonical Matrix names,
folders/divider, POLAR widths, ON reset/allocation и retained reset/dock guards.
FM audit дополнительно проверяет regenerated source manifests и release metadata
`1.9.35`.

Это не является C++ compilation: наличие JUCE toolchain и runtime UI данным
документом не заявляется.

## Следующий gate

Перед бинарной поставкой остаются обычная пользовательская JUCE/VS сборка и
ручная проверка Matrix и RESET ALL в DAW. Данный source-only record не
утверждает успешную сборку, runtime popup placement, физический UI render,
patchcord sound или звук P2 DIST/VOL/PAN.