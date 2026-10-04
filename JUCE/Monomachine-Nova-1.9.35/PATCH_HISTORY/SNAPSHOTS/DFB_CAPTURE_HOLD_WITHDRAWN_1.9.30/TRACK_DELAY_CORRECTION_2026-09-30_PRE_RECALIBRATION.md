# Monomachine Nova 1.9.30 — коррекция исходников, 30.09.2026; package revision 01.10.2026

## Статус

Этот документ является действующей коррекцией для исходной поставки 1.9.30. Полные
README и validation log 1.9.27 сохранены без сокращения в
`PATCH_HISTORY/SNAPSHOTS/RELEASE_1.9.27_SOURCE_ONLY/`.
Файлы `TRACK_DELAY_HOTFIX_2026-09-30.md` и
`VALIDATION_RUN_LOG_1.9.21_TRACK_DELAY_HOTFIX.txt` сохранены без изменения в
`PATCH_HISTORY/SNAPSHOTS/ROOT_AUDITS_AND_HANDOFFS_PRE_1.9.26/` как история
предыдущего, отозванного предположения. Их вывод о маршруте P2, нулевых
значениях DSND и миграции состояния **не является действующим**.

## Возвращённые заводские значения

В обеих ветках — `Monomachine_Nova_Synth` и `Monomachine_Nova_FX` — восстановлены
подтверждённые исходные значения:

- P1 EFFX `DSND = 64`;
- P2 EFFX `DSND = 64`;
- P2 снова использует исходный заводской выбор `FX-CHORUS`;
- `P2 MIX = 127`.

Из `PluginProcessor.cpp` полностью удалён маркер
`trackDelayDefaultRouteV1` и связанная миграция. Сохранённые состояния P2 больше
не переписываются в `FX-THRU`, `MIX=0` или `DSND=0`.

## Что не было сделано намеренно

Предыдущее объяснение через «скрытый P2 return» не имело достаточного
route-level доказательства. Поэтому эта коррекция **не** выдаёт изменение
заводских значений за исправление delay.

По проверенному upstream-пути DSND является host-side scale в маршруте echo
send/return. Pack 9 подтверждает L/R-пары выходной шины и отдельную 16-word
эхо-шину; Pack 8 векторами покрывает delay cells `$438…$43F`. Но в этих
материалах пока нет закрытого host-control mapping, который связывает
`$438…$43F` с отдельной «левой» ручкой. Напротив, `$414…$417` — stage-2
filter-envelope cells, а не доказанная ручка delay-return.

Этот пробел upstream-доказательства сохраняется: он не позволяет объявить
оригинной firmware-топологией знак или L/R-law DSND. Однако для действующей
source-only реализации 1.9.30 пользователь подтвердил наблюдаемую runtime-law:
положительная сторона DSND оставляет mono positive comb, отрицательная —
split-L/R negative comb. Это **не** новое заявление о firmware: это явно
помеченное user-observed acceptance behaviour текущего host route.

Schema 38 добавляет два default-OFF experimental switches `DSND + FB INVERT` и
`DSND - FB INVERT`. Они меняют исключительно знак feedback recurrence выбранной
стороны, но не меняют positive-mono/negative-split send route, DSND magnitude,
каналы, фазу или ping-pong. Поэтому отдельная «левая» ручка и speculative
ping-pong схема по-прежнему не добавлены.

## Выполненные source-only исправления рядом с этой коррекцией

- меню гибридных фильтров показывает значения в группах типа/response и
  сортирует имена внутри группы без изменения сериализованных choice ID;
- LP keytrack в OLD и MNM берёт ту же ограниченную физическую позицию
  `BASE+WDTH`, что и нетрекинговый путь;
- P2 neutral-insert bypass больше не скрывает пользовательское отключение
  HPF/LPF keytrack;
- полный экран AMP ENV / FIL ENV / MOD ENV поддерживает held-drag между всеми
  тремя tab-кнопками;
- верхние AMP/FIL ENV/MOD ENV кнопки получают selected-state после замены
  compact dock и очищаются при переходе на другую overlay-страницу.

## DFB dynamics schema 38

Для существующего TrackChain delay пути P1/P2 `DFB DYNAMICS` default ON. Это
один active-feedback state controller с точным contract:

| Условие | Состояние active feedback |
|---|---|
| `prepare/reset` | начинается с unity `1.0` |
| DFB `65..127` | один state плавно заряжается вверх, bounded максимум `1.125` |
| DFB `0..63` | этот же state плавно разряжается вниз; `63` — наиболее мягкое снижение |
| DFB `64` | точный `HOLD`: не назначает unity, а bit-identical удерживает накопленное/просевшее значение |
| DFB `0` | полный smooth `ZERO TAIL` release без clear |

Поэтому возврат с `63` к `64` фиксирует уже слегка просевший level и отличается
от продолжения `63`. `RISE` задаёт скорость заряда, `ZERO TAIL` — smooth
нисходящий ход; оба параметра и `DYNAMICS` доступны только в RMB mini-panel
DFB. Там же сохранены `RETURN COMP`/`FB CLIP` и два experimental default-OFF
DSND inversion switch. `DYNAMICS=OFF` остаётся отдельным legacy A/B путём
`raw / 63`.

Закон DFB является автономным controller law: здесь не добавлена никакая
вспомогательная интерпретация быстрых жестов или reset condition. `DBAS`/`DWID`
и 12 dB feedback-filter response не менялись; selector slope не добавлен.

`FxSlotEngine` не имеет подтверждённого routing в TrackChain DFB controller.
Он помечен `DFB_ROUTE_UNRESOLVED`; custom FX slot DFB path намеренно не
додуман и не заявлен сделанным.

## Проверка исходников

Предыдущий target-local `verify_dsp_mode_patch.py` сохранён как исторический
snapshot в `PATCH_HISTORY/SNAPSHOTS/RETIRED_VERIFY_DSP_MODE_PATCH/`. Он
проверял pre-cleanup registry и не является текущим executable audit.

Для текущей поставки запускается только корневой
`python3 FM_MODE_CLEANUP_STATIC_CHECK.py`: он проверяет six-slot FM contract,
schema 38 DFB parameter/runtime wiring, явный MSVC-safe `float` cast для ARP
SONG page и хэши текущих source manifests. Проверка подтверждает наличие
пользовательской DSND route/polarity реализации в исходнике, но не доказывает
её hardware-эквивалентность или speculative left/ping-pong поведение.

Ни рендер, ни полная VST3-сборка, ни DAW-вывод о фактическом delay в этой
коррекции не заявляются.
