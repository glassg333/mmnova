# Monomachine Nova 1.9.35 — актуальные исходники (source-only)

Дата актуальной source revision: 04.10.2026.

Это добавочная исходная поставка для двух веток:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Предыдущие source archives 1.9.21 и 1.9.27 сохранены без удаления.
Полные активные README и validation log релиза 1.9.27 также сохранены без
сокращения в `PATCH_HISTORY/SNAPSHOTS/RELEASE_1.9.27_SOURCE_ONLY/`; текущий
source archive не перезаписывает исторические материалы.

## Что изменено в MODE SYNT

У FM+ `STAT` (m8), `PAR` (m9) и `DYN` (m10) осталось ровно шесть selectable
режимов в этом порядке:

| ID renderer | Новая подпись |
|---:|---|
| 0 | `mnm frq` |
| 1 | `old frq` |
| 2 | `new frq` |
| 3 | `mnm bpm` |
| 4 | `new bpm` |
| 5 | `old bpm` |

Числовые renderer IDs и DSP mappings этих шести путей не переставлялись. В
частности, `new bpm` остаётся ID 4, а `old bpm` — ID 5.

Из пользовательского MODE SYNT удалены experimental slots:

- `mnm frq env fix` (бывший raw ID 6);
- `try4` (бывший raw ID 7);
- `fix5 pitch+env full` (бывший raw ID 8).

Полные исходники импортированных экспериментов, candidate-only tests и fixture
сохранены с SHA-256 в `PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`, а из
shipping Source удалены. Они не попадают в choices, UI/state allow-list,
processor-owned runtime route или compilation graph.

## Состояние и защита от неверного выбора renderer

- Schema поднята до **45**: прежние FM-normalization rules и CHOR A/B
  сохранены; неработающий DLY CORE снят из поставляемых исходников. Schema 44
  DFB BASE CURVE/HOLD @64 и RAW-gated GUARD anchors @64/@127 сохраняются,
  а schema 45 добавляет persistent direct-LFO/MOD MATRIX polarity state.
- При загрузке старого state raw ID 6, 7 или 8 нормализуется в `mnm frq` (ID 0).
  Он не ограничивается до ID 5, поэтому не происходит скрытого перехода на
  `old bpm`.
- Та же защита есть в processor snapshot и в границе `MachineEngine::setModes`.
- Обычный mode parameter FM+ теперь имеет диапазон `0..5` и шесть labels.

## Изменённые рабочие исходники

В обеих ветках синхронно обновлены:

- `Source/models/DspModes.hpp`, `Source/models/modulation_matrix.hpp`,
  `Source/models/parameter_conversions.hpp`, `Source/models/track_pages.hpp`,
  `Source/NovaData.h`, `Source/NovaDSP.h`;
- `Source/PluginProcessor.{h,cpp}`;
- `Source/PluginEditor.cpp`;
- `Source/NovaDSP.h`;
- `Source/dsp/TrackVOLPAN.inl`, `Source/dsp/TrackDelay.inl`;
- `Source/dsp/DelayFeedbackDynamics.hpp`, `Source/dsp/mnm/MnmDelay.hpp`
  и `Source/dsp/mnm/MnmTrackDelayNew.hpp`;
- `Source/dsp/AudioAdapter.{h,cpp}` и `Source/FxSlotEngine.h`;
- `CMakeLists.txt`, `tests/FmModeRetirementTests.cpp` и обновлённые source-only
  проверки `tests/DelayFeedbackDspTests.cpp` / `tests/FmFixModeTests.cpp`.

Сняты candidate-only imports, пустой FM STAT tombstone, profile header, CMake
registrations, tests и fixture; точные версии до снятия лежат в
`PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`. Active m9 PAR unity readout
сохранён без candidate profile header. Указанные общие файлы и retained-mode
тесты Synth/FX проверяются на byte-identical mirror. Их ранее сохранённые
registry test sources также остаются в
`PATCH_HISTORY/SNAPSHOTS/FM_MODE_TESTS_PRE_CLEANUP_2026-09-30/`.

## Phaser

Для FX machine ID 18 в `Source/dsp/monomachine_effects.hpp` full-wet output
заменён с unity-gain all-pass image на bounded phase-difference:
`0.5f * (allpassCascade - input)`. Старая формула сохранена закомментированной
сразу рядом с новой. Это целевая практическая коррекция без заявления о
доказанном hardware-эквиваленте; machine ID, шесть передаваемых ручек и routing
не менялись.

## Chorus: реальный A/B runtime и INP

В DSP-меню `CHOR` теперь есть два **реальных** значения параметра `mode_cho`:

| index | подпись | вызываемое ядро |
|---:|---|---|
| 0 | `native` — default | `NativeChorusCore` через `AudioAdapter` |
| 1 | `core` — A/B | эталонный `ChorusCore` через тот же `AudioAdapter` |

Переключатель доходит до обычного P1, P2 и уже созданных пользовательских
FX-slots. Он не меняет host `INP`, dry/wet/MIX, gain, latency или routing.
При смене `native`/`core` `AudioAdapter` сознательно сбрасывает audio/tail state
обоих ядер: совместимый перенос X/Y/delay-memory не выдумывается. Поэтому A/B
начинается с чистого состояния, а Native остаётся и factory/default, и путём
для всех старых state (schema `<37` принудительно мигрирует в `native`).

`CHORUS SAFE` не выбирает ядро: при default OFF работает выбранное ядро и
сохраняет tail при machine `MIX=0`; SAFE=ON только обходит/сбрасывает это же
выбранное runtime-ядро при `MIX=0`.

По пользовательскому сравнению только host-участок `INP −64..−60` получил
квадратичный ease-in: −64 остаётся нулём, −60 и все значения выше сохраняют
прежний коэффициент `raw/64`. Меняются только −63/−62/−61; это не изменение
native 24-bit core и не заявление о hardware-формуле.

## Track Delay: DFB BASE BEND + RAW GUARD

Старый capture/HOLD вариант и прежние schema-42/schema-43 описания сохранены
без сокращений в `PATCH_HISTORY/SNAPSHOTS/`; они не являются текущей
спецификацией.

Schema **44** отделяет музыкальную низкую часть BASE от safety-GUARD:

- при `DYNAMICS=ON` `RAW=63` остаётся фиксированной reference-точкой
  `63 / 64`; `RAW 0..63` идёт по настраиваемой поднимающей кривой
  `BASE CURVE`, поэтому значения `60..62` подходят к этой точке плавно, без
  прежнего резкого порога feedback-tail;
- `BASE CURVE=1.00` возвращает прежнюю строгую линию `RAW / 64` на low-side.
  Значения меньше `1.00` поднимают low RAW ближе к reference; значения больше
  `1.00` делают его осторожнее. Диапазон control: `0.10..2.00`, fresh default
  `0.55`;
- `HOLD @64` — отдельная очень малая граница между reference и верхней
  веткой. Диапазон намеренно ограничен `63/64..1.0000`; fresh default `0.9920`
  (около `−0.07 dB` от unity). То есть RAW 64 нельзя случайно ослабить сильнее,
  чем до reference RAW 63;
- `RAW=65` и выше сохраняют верхнюю линию `RAW / 64`; от HOLD @64 до RAW 65
  есть короткий непрерывный стык. Это не DTIM, delay-buffer, send или return
  gain law.

GUARD сохраняет отдельный контракт: RAW `0..63` — полный bypass, RAW `63..64`
только мягко armed follower, а активное окно RAW `64..127` задают четыре
persistent anchor: `START @64`, `PLT @64`, `START @127`, `PLT @127`.
`GUARD OFFSET`, `GUARD CURVE`, `GUARD AMOUNT`, `ATTACK`, `RELEASE` остаются
независимыми controls safety-governor; это не заявление о восстановленной
формуле firmware.

Fresh-project defaults приняты по опубликованной настройке DFB panel:

| Control | Fresh default |
|---|---:|
| BASE CURVE | `0.55` |
| HOLD @64 | `0.9920` |
| START @64 / PLT @64 | `0.00` / `0.05` |
| START @127 / PLT @127 | `0.00` / `3.46` |
| GUARD OFFSET / CURVE / AMOUNT | `0.18` / `1.08` / `2.00` |
| ATTACK / RELEASE | `1 ms` / `1 ms` |

Migration является намеренной и не меняет прежние проекты на новую кривую
молча: при загрузке schema `<44` добавляются `BASE CURVE=1.00` и
`HOLD @64=1.0000`. Эта пара точно восстанавливает прежний `RAW / 64` на всём
диапазоне, включая дробный участок `64..65`. Schema-39 anchors и schema-40
`dfb_guard_raw` остаются compatibility-only APVTS/state IDs; migration `<43`
по-прежнему создаёт endpoint anchors/OFFSET без удаления старого state.

### CPU и DFB panel

- Guard drive LUT (`65 × 257`) больше не строится повторно на каждом audio
  block при неизменном APVTS snapshot: `setGuardAndGovernor()` сравнивает
  sanitized values и rebuilding выполняется только при реальном edit. BASE
  имеет отдельный маленький LUT и тоже rebuild только при смене BASE controls.
- RMB по DFB открывает только white embedded child overlay внутри VST/editor:
  это **не** `DocumentWindow` и не detached OS window. Панель `400×386`,
  наследует общий `FloatingPanel` pin, не имеет `X`, обновляет graphs в `10 Hz`
  и использует screen-coordinate bounded drag — без дрожания у границы editor.
- `DYNAMICS`, оба `DSND ± FB INVERT`, `RETURN COMP` и `FB CLIP / GUARD`
  остаются ON по fresh default. Графики показывают BASE `63 REF / 64 HOLD /
  65+ GROWTH` и GUARD window; текст панели остаётся белым.

`DLY` в DSP menu содержит только `mnm | old | new`. Неработающий `core` снят
из UI, runtime, state owner, tests и shipping Source. При загрузке state schema
40/41 сохранённое бывшее значение `core=3` явно мигрирует в `new=2`; прочие
ошибочные DLY values нормализуются в `mnm`.

DBAS/DWID сохраняют текущий 12 dB feedback-filter response; selector slope не
добавлялся. `FxSlotEngine` по-прежнему не получил speculative DFB routing:
`DFB_ROUTE_UNRESOLVED` остаётся границей текущей реализации.

## Direct LFO и MOD MATRIX (schema 45)

Schema **45** добавляет независимое persistent состояние для каждого из 12
internal LFO (`P1 LFO1..6`, `P2 LFO1..6`): `UNIPOLAR/BIPOLAR`, `INV` и
`ALT DUAL`. Это не один общий UI-флаг: все три значения сохраняются отдельно
для конкретного LFO в APVTS state.

- Fresh `DPTH` каждого direct LFO равен `0`. Обычный диапазон — `0..127`, где
  `127` означает 100% amount. В `ALT DUAL` ноль становится центром, а диапазон
  — `-127..0..+127`.
- UNI сохраняет one-sided источник, BI сохраняет bipolar источник. `INV`
  применяется **после** этого выбора: UNI меняет positive side на negative,
  BI меняет направление bipolar range. Signed `DPTH` из `ALT DUAL` остаётся
  отдельным множителем направления.
- Faceplate `DPTH` снова использует исходную обычную geometry, как `PAN`:
  `{getWidth()/2-26,27,52,43}` и полный centred LCD strip
  `{2,getHeight()-26,getWidth()-4,23}`. Поэтому knob и value `0` не сдвинуты;
  компактные `UNI`/`INV` вычисляются относительно этой rectangle и никогда не
  отнимают место у ручки или её LCD. RMB по `DPTH` открывает persistent
  embedded panel данного LFO с
  `UNIPOLAR/BIPOLAR`, `OFF/INV` и `ALT DUAL`; UI-текст и tooltips панели
  остаются English/ASCII.
- Для P2 direct routing используется local offset target bank, а не raw
  `target % 8`: `132..163`, `164..187` и `212..235` декодируются от начала
  соответствующего банка. P2 AMP `DIST`/`VOL`/`PAN` на faceplate и patchcord
  имеют exact Matrix target IDs `144`/`145`/`146`; они больше не ошибочно
  указывают на P2 SYNT `136..138`.

### Matrix feedback corrective follow-up

Matrix — route table с readable title `20 px` и едиными table requests `14 px`.
Она показывает независимые `ON`, `SOURCE`, `DESTINATION`, signed `DEPTH`,
`POLAR`, `INV`, `AUX SOURCE` и `AUX AMOUNT`; `AUX` означает второй source,
умноженный на signed auxiliary amount, без ложной надписи `RMB`. `DEPTH=0` или
`DEST=OFF` уже инертны, поэтому fresh/reset Matrix rows остаются `ON`: это
позволяет сразу назначить target без лишнего включения. Allocation MSEG/cord
ищет blank unlocked payload, а не только `ON=OFF` row.

`SOURCE` и `AUX SOURCE` используют folders `MIDI`, `LFO`, `P2 LFO`, `ARP`,
`MSEG`, `MACRO` и `MOD ENV`; `RANDOM` остаётся сразу в root menu. Matrix
source names — `P1 LFO1..6` и `P2 LFO1..6`, без suffix `INTERNAL`. Canonical
Matrix target text также сохраняет видимые P1/P2 отношения. Routes можно
сортировать header-ом, переставлять drag-ом, назначать через DEST aim,
lock/solo и clear controls; visible table grows dynamically от 16 до 64 rows.

Main table header называется `POLAR`, не `MODE`. Widths `DEPTH=140` и
`POLAR=120` возвращают readable pixel scale signed graph и
`UNIPOLAR`/`BIPOLAR`, сохраняя 1145 px table width. DEPTH graph соответствует
polarity: BI заполняет positive и negative halves, UNI рисует фактическую
сторону после `INV`. Это же правило применяется к direct-LFO graph. Matrix
`DEST` хранит canonical raw value `target + 1`; display conversion обратно в
target выполняется один раз.

Pinned `MatrixLfoDock` остаётся sibling Surface и растёт вверх от нижней
границы Matrix viewport. Headers называются `P1 LFO` / `P2 LFO`; белый top
divider визуально отделяет dock от таблицы. PAGE menu больше не flat: raw IDs
не меняются, но choices размещены в явных folders `P1 PARAM`, `P1 LFO`,
`P2 PARAM`, `P2 LFO`, включая LFO1–LFO6. PAGE/DEST popup menus предпочитают
`upwards`, DPTH setup тоже открывается вверх; при DEST aim dock скрывается.

В expanded `P1 LFO`/`P2 LFO` dock удалены дублирующие header `SETUP` и ячейки
`DPTH SETUP`: остались семь рабочих columns `LFO`, `PAGE`, `DESTINATION`,
`DEPTH`, `POLARITY`, `INVERT`, `ALT DUAL`. Освободившаяся ширина возвращена
`PAGE`/`DESTINATION`; правый клик по `DEPTH` по-прежнему открывает тот же
setup, поэтому routing, IDs, state и direct-depth options не потеряны.

Для list-like Matrix controls восстановлен общий choice/machine gesture:
обычный LMB click открывает прежнее menu, а vertical LMB drag шагами по 8 px
пошагово листает допустимые raw choices и после реального drag не открывает
menu при release. Это действует для dock `PAGE`/`DESTINATION` и table
`SOURCE`/`DESTINATION`/`AUX SOURCE`. Один drag держит один host automation
begin/end gesture; folders, destination hover `PREVIEW`, locks, aim и RMB
actions остаются прежними.

Visible compaction разрешена только для redundant P1-on-P1. P2 badge/name
остаётся на faceplate PAGE/DEST, в popup, LFO LOCKS, Matrix dock и Matrix
names; P1 marker также сохраняется для cross-page P2 assignment. Faceplate
DEST по-прежнему показывает короткий local token (`P2 SYNT 1FRQ` → `1FRQ`),
но popup сохраняет readable canonical P2 context.

Matrix `DEST` popup использует hover-aware rows: hovering target показывает
`PREVIEW` прямо в Matrix и временно включает outline соответствующего
faceplate control. Закрытие или выбор menu очищает preview.

`RESET ALL PARAMETERS` сначала собирает только APVTS values, которые реально
отличаются от default, затем отправляет обычные host-visible
`setValueNotifyingHost()` в ordered chunks по 12 async message-loop calls.
Поэтому тысячи ARP/MATRIX values не занимают один непрерывный UI turn и не
создают прежний per-parameter begin/end gesture storm. Undo/redo ждут окончания
queue; при закрытии editor остаток reset корректно завершается. Это source
strategy, а не измеренный runtime benchmark.

Старые direct `DPTH` state values намеренно не сохраняются как legacy amount:
при migration в schema 45 они сбрасываются в `0`, как согласовано для этого
rework. Отдельный актуальный source-only record:
`VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md`.

## LFO: HALF и оригинальная tempo law SPD/MULT

Во всех 12 active LFO (`P1 LFO1..6`, `P2 LFO1..6`) `TRIG` теперь имеет
пятый и последний пункт `HALF`. При trigger он начинает selected waveform
с phase `0`, проходит **строго одну половину** цикла, останавливается на
phase `0.5` и сохраняет достигнутый output до следующего trigger. Сохраняется
фактический post-`INTL` level, поэтому остановка не продолжает interlace-gate
после достижения endpoint. `FREE`, `TRIG`, `HOLD` и `ONE` остаются raw
значениями `0..3`; `HALF=4` и независимая настройка каждого LFO сохраняются
в его существующем APVTS parameter group.

`SPD` больше не использует прежнюю экспоненциальную кривую вокруг 64. Для
Monomachine tempo-relative cycle используется law:

```text
steps per full cycle = 2048 / (SPD * MULT)
cycles per second    = BPM * SPD * MULT / 30720
```

`MULT` — ровно один из `1X, 2X, 4X, 8X, 16X, 32X, 64X`, а `SPD` — линейный
raw multiplier `0..127`. Следовательно, `SPD=64`, `MULT=2X` даёт 16
sixteenth-notes на полный cycle (при 120 BPM — 2 секунды); `MULT` вдвое
сокращает его длину. Это согласовано с Monomachine manual и с exposed inline
formula Modbang Calculator `2048 / (SPD * MULT)`. На странице Modbang
действительно видна кнопка `Calculate LFO Settings`; она позволила обнаружить
ошибку прежней exponent law.

`prepareToPlay` и `panic` теперь reset'ят все четыре runtime arrays
`lfos`, `lfos3`, `lfos2`, `lfos4`, то есть все 12 LFO, а note trigger
продолжает fan-out во все P1/P2 instances.

### Faceplate PAGE/DEST и WAVE popup

Для direct-LFO `PAGE`/`DEST` popup, LFO LOCKS и Matrix сохраняют свои
текущие P1/P2 names: эта corrective правка их не меняет. Только в маленькой
faceplate cell `DEST` показывается leaf control из уже выбранной PAGE:
`P1 AMP PORT` / popup `AMP PORT` → `PORT`, `P1 LFO1 DPTH` / popup
`LFO1 DPTH` → `DPTH`, `P2 SYNT 1FRQ` → `1FRQ`, `P2 AMP MIX` → `MIX`.
PAGE уже задаёт bank, поэтому это не меняет raw routing/state и возвращает
requested `21 px` PixelFont value без scale-1. P2 name/badge остаётся видимым
на собственной P2 стороне, а P1 marker остаётся для cross-page P2 choice.

Для PAGE `PITCH` отдельная layout correction даёт direct PAGE/DEST всю 91 px
ширину cell вместо standard 87 px inset. `PixelFont` требует 90 px для пяти
букв `PITCH` на scale 3; теперь `PITCH` P1/P2 имеет тот же scale 3, что и
`SYNT`. Это также faceplate-only: dropdown/LOCKS/Matrix не меняются. Badge на
icon имеет scale 2 и не закрывает value.

LFO `WAVE` dropdown и остальные choice lists больше не смешивают local panel
coordinate с global screen coordinate: popup получает exact screen bounds своей
cell и остаётся у ручки даже при сдвинутом host window. English functional UI
texts не изменялись. Отдельный source-only record —
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_FACEPLATE.md`.

### Dynamic SPD hover tooltip

Hover над direct-LFO `SPD` у всех `P1 LFO1..6` и `P2 LFO1..6` теперь открывает
динамическую English/ASCII tooltip с живым `TEMPO NOW` (из
`processor.currentBpm()`), текущими значениями данного LFO `SPD`/`MULT` и
calculator-style timing. Она показывает полный cycle в `1/16`, секундах и Hz,
а также длину `HALF MODE`; формула прямо видна как
`2048 / (SPD x MULT)`. Например, `SPD=64`, `MULT=2X`, `BPM=120` показывает
`16 1/16 = 2.000 s = 0.500 Hz` и HALF `8 1/16 = 1.000 s`.

`currentBpm()` уже отражает host tempo при HOST SYNC и global BPM в internal
режиме. При `SPD=0` tooltip явно сообщает `STOPPED`, не выполняя деление на
ноль. Она обновляется в существующем UI timer (20 Hz), но cached text не
перезаписывается без изменения BPM/SPD/MULT; choice/pick state и rebind
очищают tooltip. Новый source-only record —
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_SPEED_TOOLTIP.md`.

### LFO LOCKS: P1/P2 row alignment

Вторая `PAGE 11..20` column LFO LOCKS теперь имеет dim visual-only первую
строку `P2 PITCH N/A`: у P2 нет отдельного physical PITCH page, но spacer
ставит последующие `SYNT/AMP/FILT/EFFX/LFO1..6` на те же горизонтальные строки,
что и в первой P1/PITCH column. Placeholder не имеет lock/solo/current outline,
hit-test возвращает no-hit, поэтому его нельзя выбрать, lock/solo-назначить или
записать в state. Реальные rows `1..10` второй column продолжают отображать
те же persistent `PAGE 11..20`; routing, 21 PAGE choices и masks не менялись.
Отдельный source-only record —
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_LOCK_ALIGNMENT.md`.

### JUCE 8 PopupMenu compatibility

Windows Release x64 build feedback обнаружил MSVC C2665 в четырёх legacy
`PopupMenu::addCustomItem(id, component, enabled, ticked)` calls: JUCE 8 больше
не принимает enabled/ticked bool после custom component. Все hover-aware
LFO/Matrix target rows переведены на единый `PopupMenu::Item` helper, который
сохраняет `itemID`, fallback text, enabled/ticked state, pixel custom renderer
и hover preview. Direct-LFO availability, Matrix `OFF`, canonical target IDs,
routing и state не изменены. Отдельный source-only record —
`VALIDATION_STATIC_AUDIT_2026-10-03_POPUP_MENU_JUCE8.md`; следующий gate —
повторная Windows VST3 build, поэтому готовый binary пока не заявляется.

## ARP SONG: активное окно и рисование

`S START`/`S END` теперь задают не только playback range: в поле SONG
показываются **только** активные позиции этого диапазона. Они динамически
занимают ширину строки тем же образом, что и обычные ARP step rows; скрытые
SONG positions не удаляются и не переписываются.

ЛКМ-перетаскивание по строке `S-PART` или `REPEAT` работает кистью: значение рисуется
через все активные позиции, которые пересекает курсор, включая пропущенные при
быстром горизонтальном движении. Для каждой затронутой позиции открывается и
закрывается собственный жест хоста. ПКМ по одной ячейке, как прежде, возвращает
только эту позицию к её заданному default.

## ARP: компактные подписи и читаемые подсказки

В ARP page убрана лишняя статическая надпись `PAGE 1..64` под step lanes:
значение по-прежнему задаётся `EDIT PAGE` и объясняется hover tooltip. У SONG
toggle убран перекрывающийся текст на кнопке; сама кнопка и её hover tooltip
остались активными.

Первый пункт RMB-меню COPY/PASTE теперь `WHOLE PAGE`, а не строка с кириллицей,
поэтому он больше не оказывается пустым в pixel-font menu. Все function
hover-tooltips в `PluginEditor.cpp` переведены на English/ASCII: русский текст
не передаётся в font, который показывает его кракозябрами.

## MOD ENV: компактная категория на основной поверхности

Удерживание ЛКМ на `MOD ENV` и переход курсором на `AMP` или `FIL ENV` теперь
является одним атомарным handoff: target category назначается до удаления
старого dock, а source category не рисуется как нажатая только из-за ещё
удерживаемой кнопки мыши. Поэтому после перехода не остаётся белого/selected
`MOD ENV` и не создаётся визуальный слой прежнего compact panel.

Compact `MOD ENV` использует тот же shell, что и AMP/FIL ENV: `387×236`,
reserved header `28 px`, envelope graph `8,32,371,114` и row controls
`x=8/103/198/293`, `y=152`. Из compact view удалены MOD-specific page/route
widgets и рисуемые строки `MOD ENV`, `RMB LARGE`, `MATRIX SOURCE · MOD ENVn`.
Выбор `MOD ENV1..4` и `ROUTE TO...` не потерян: он остаётся в retained large
MOD ENV page, доступной RMB по категории. Compact panel редактирует источник,
который был передан ему из этой large page (обычный ЛКМ entry открывает
`MOD ENV1`).

## Исправление MSVC C2397

В `Source/NovaData.h` для SONG part page default добавлен явный
`static_cast<float>(part+1)`. Это исправляет MSVC C2397: переменный `int`
в list-initializer `Spec::def` нельзя неявно сужать до `float`.

## Самостоятельная сборка

Нужен отдельный checkout JUCE 8. Пример для каждой ветки (подставьте абсолютный
путь к своему JUCE):

```bash
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/абсолютный/путь/к/JUCE
cmake --build build-synth --config Release

cmake -S Monomachine_Nova_FX -B build-fx -DJUCE_DIR=/абсолютный/путь/к/JUCE
cmake --build build-fx --config Release
```

`NOVA_BUILD_TESTS` по умолчанию выключен. В этой поставке не заявляется
результат полной сборки, VST3/DAW-проверки, render или выполнения тестов:
пользовательская сборка остаётся следующим gate.

## Статическая проверка этой поставки

Для актуальной source-only ревизии используется только локальный static audit:

```bash
python -m py_compile *_STATIC_CHECK.py
python DFB_GUARD_CORE_STATIC_CHECK.py
python FM_MODE_CLEANUP_STATIC_CHECK.py
python LFO_TIMING_STATIC_CHECK.py
python LFO_FACEPLATE_STATIC_CHECK.py
python LFO_SPEED_TOOLTIP_STATIC_CHECK.py
python LFO_LOCK_ALIGNMENT_STATIC_CHECK.py
python POPUP_MENU_JUCE8_STATIC_CHECK.py
python MATRIX_UI_STATIC_CHECK.py
```

`FM_MODE_CLEANUP_STATIC_CHECK.py` перед UI-specific checks сверяет manifest
hashes обеих целей, version metadata `1.9.35`, schema 45 migration поверх
schema-44 DFB, mirrored shipping sources и retained historical candidates.

Первый LFO-specific audit проверяет mirror Synth/FX, final `HALF` в APVTS и LCD
conversion, отсутствие прежней exponential SPD law, linear
`2048 / (SPD * MULT)` timing, phase endpoint/held level, independent state,
trigger fan-out и reset всех 12 LFO. Его результат записан в
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_TIMING.md`.

Второй проверяет PAGE/DEST target-local faceplate compaction, requested
21 px value, scale-2 P1/P2 badge, правило «compact только P1-on-P1», exact
global popup anchor для WAVE и отсутствие смешанного local/screen coordinate
clamp. Его record — `VALIDATION_STATIC_AUDIT_2026-10-03_LFO_FACEPLATE.md`.

Третий проверяет dynamic SPD tooltip: scope всех 12 direct LFO, live
`currentBpm()`/`tempoDisplay`, active-LFO `MULT`, calculator law
`2048/(SPD*MULT)`, `steps/seconds/Hz/HALF`, zero-SPD guard, English UI text,
20 Hz refresh/cache и mirror Synth/FX. Его record —
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_SPEED_TOOLTIP.md`.

Четвёртый проверяет LFO LOCKS P1/P2 alignment: 11 visual rows обеих PAGE
columns, non-selectable `P2 PITCH N/A`, mapping rows `1..10 → PAGE 11..20`,
неизменные 21 choices/mask/solo ranges и mirror Synth/FX. Его record —
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_LOCK_ALIGNMENT.md`.

Пятый проверяет JUCE 8 PopupMenu compatibility: все четыре прежних custom-item
calls переведены на Item API, custom hover/preview и enabled/ticked state
сохранены, legacy `menu/where.addCustomItem(...)` invocation отсутствуют, а
Synth/FX editor остаётся byte-identical. Его record —
`VALIDATION_STATIC_AUDIT_2026-10-03_POPUP_MENU_JUCE8.md`.

Шестой (`MATRIX_UI_STATIC_CHECK.py`) проверяет corrective feedback pass:
DPTH как исходная `PAN` geometry/full centred LCD и relative UNI/INV, P2
badge/name paths, P2 DIST/VOL/PAN IDs `144..146`, `POLAR`/readable Matrix
widths, source labels без
`INTERNAL`, white dock divider и foldered P1/P2 PAGE menu. Также проверяются
fresh/clear/target-pick `ON`, blank-route allocation, reset queue, held-LMB
ENV handoff, upward pinned dock, SafePointer lifecycle и byte-identical
Synth/FX editor/processor/data. Дополнительно audit фиксирует seven-column
P1/P2 dock без `SETUP`/`DPTH SETUP`, retained RMB `DEPTH` setup и общий
8-px click-versus-drag selector для dock `PAGE`/`DESTINATION` и table
`SOURCE`/`DESTINATION`/`AUX SOURCE`. Его record —
`VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md`.

Итоговый corrective запуск 04.10.2026 прошёл: DFB `148 checks`, LFO timing
`32`, faceplate `28`, SPD tooltip `35`, LOCKS `20`, JUCE8 menu `16`, Matrix
`111`; FM manifest/metadata audit — `OK`. `DFB_GUARD_CORE_STATIC_CHECK.py`
подтверждает
current schema 45 с retained schema-44 DFB contract. Более ранний
`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_MATRIX.md` сохранён как historical
schema-45 record. Ни один audit не компилирует и не исполняет DSP.
