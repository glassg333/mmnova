# Статический аудит Matrix UI feedback — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только source/static audit. Сборка, запуск editor/plugin/DSP,
CTest, VST3/DAW и UI/audio render намеренно не выполнялись.

## Scope

Финальный `Source/PluginEditor.cpp` сначала был проверен в Synth, затем
зеркально перенесён в FX. Перед заменой сохранён прежний FX editor:

- `PATCH_HISTORY/SNAPSHOTS/FX_PRE_MATRIX_FEEDBACK_1.9.35/PluginEditor.cpp`;
- `PATCH_HISTORY/SNAPSHOTS/FX_PRE_RESET_QUEUE_1.9.35/PluginEditor.cpp`.

Для каждого snapshot SHA-256 записан рядом в `SHA256SUMS.txt`. Активные Synth
и FX editor sources после Matrix feedback и reset-queue follow-up
byte-identical.

## Проверенные feedback-коррекции

1. Direct-LFO `DPTH` вернул исходные bounds `11,27,43,43`; компактные `UNI`
   и `INV` находятся рядом в правой полосе и не сдвигают/не расширяют ручку.
2. `RESET ALL PARAMETERS` не открывает и не закрывает host gesture для каждого
   APVTS parameter. Уже-default values не ставятся в очередь; реально
   отличающиеся значения сохраняют одно `setValueNotifyingHost()`, но идут
   ordered chunks по 12 async message-loop calls. Undo/redo не пересекаются с
   активной queue; при destruction editor остаток корректно flush'ится.
3. Большие AMP/FIL/MOD ENV tabs получили общий held-LMB handoff. Surface
   serialises async replacement, поэтому устаревший callback не может вернуть
   категорию источника после отпускания над назначением.
4. Matrix route table имеет единые читаемые 20 px/14 px requests, foldered
   `SOURCE` и `AUX SOURCE` menus (`MIDI`, `LFO`, `P2 LFO`, `ARP`, `MSEG`,
   `MACRO`, `MOD ENV`) и сразу доступный `RANDOM`.
5. `AUX SOURCE` и `AUX AMOUNT` показаны как отдельные coherent columns;
   AUX amount использует signed graph без вводящей в заблуждение надписи `RMB`.
   Активная область Matrix динамически остаётся в диапазоне 16..64 routes.
6. Route `DEPTH` graph теперь получает `MODE` и `INV`: BI заполняет обе
   половины; UNI показывает реальную сторону после INV. Direct-LFO graph
   проверен тем же правилом.
7. Исправлена off-by-one ошибка display-only Matrix `DEST`: raw stored
   `DEST = target + 1` преобразуется в zero-based target ровно один раз.
8. `MatrixLfoDock` — sibling Surface, а не child scrolling `MatrixPage`.
   Его `P1 INTERNAL LFO`/`P2 INTERNAL LFO` headers pinned к нижней границе
   Matrix viewport; раскрытая группа из шести нормальных light rows растёт
   вверх. PAGE/DEST menus просят JUCE direction `upwards`, а DPTH setup
   смещается вверх. При destination aim dock скрывается, чтобы не закрывать
   faceplate target.
9. В dock local P1/P2 prefix display-only скрывается, а cross-page marker
   сохраняется: на P1 P2-назначение по-прежнему явно видно. Canonical Matrix
   names/IDs и routing state не менялись.
10. Асинхронные Matrix/dock menus используют `SafePointer`; закрытие overlay
    очищает длительный target-preview outline. Obsolete
    `matrixPage->openLfoDepth` и scrolling `drawInternalGroups` отсутствуют.

## Выполненный static audit

```bash
python3 FM_MODE_CLEANUP_STATIC_CHECK.py
python3 MATRIX_UI_STATIC_CHECK.py
python3 LFO_FACEPLATE_STATIC_CHECK.py
python3 LFO_LOCK_ALIGNMENT_STATIC_CHECK.py
python3 LFO_SPEED_TOOLTIP_STATIC_CHECK.py
python3 LFO_TIMING_STATIC_CHECK.py
python3 POPUP_MENU_JUCE8_STATIC_CHECK.py
```

Результаты актуального source-only запуска:

```text
FM MODE CLEANUP STATIC CHECK: OK
MATRIX UI STATIC AUDIT: PASS (51 checks; source-only, no build/test/render run)
LFO FACEPLATE STATIC AUDIT: PASS (15 checks; source-only, no build/test/render run)
LFO LOCK ALIGNMENT STATIC AUDIT: PASS (20 checks; source-only, no build/test/render run)
LFO SPEED TOOLTIP STATIC AUDIT: PASS (35 checks; source-only, no build/test/render run)
LFO TIMING STATIC AUDIT: PASS (32 checks; source-only, no build/test/render run)
POPUP MENU JUCE8 STATIC AUDIT: PASS (16 checks; source-only, no build/test/render run)
```

Лексическая проверка `()[]{}` включена в новый 51-check audit для обеих active
editor copies. В reset scope audit также проверяет default-only queue, bounded
`12`-notification chunk, SafePointer async handoff, undo guard и lifecycle
flush. Это не является C++ compilation: наличие JUCE toolchain и runtime UI
данным документом не заявляется.

## Следующий gate

Перед бинарной поставкой остаются обычная пользовательская JUCE/VS сборка и
ручная проверка Matrix и RESET ALL в DAW. Queue design исключает одну длинную
source-level series host calls, но данный source-only record не измеряет
фактические миллисекунды и не утверждает успешную сборку, runtime popup
placement или звук.
