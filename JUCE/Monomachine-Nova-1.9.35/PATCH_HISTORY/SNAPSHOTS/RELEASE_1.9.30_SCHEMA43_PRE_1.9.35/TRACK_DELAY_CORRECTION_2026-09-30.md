# Monomachine Nova 1.9.30 — коррекция исходников, актуализировано 03.10.2026

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

Schema 41 сохраняет experimental switches `DSND + FB INVERT` и
`DSND - FB INVERT`; в reference factory state оба включены вместе с
`RETURN COMP` и `FB CLIP`. Migration не перезаписывает существующее saved
value. Отдельная «левая» ручка и speculative ping-pong схема по-прежнему не
добавлены.

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

## DFB / DLY: действующая schema 43

Предыдущий schema-39 anchor/TUNE, capture/HOLD и schema-42 level-window material
сохранены без сокращений в
`PATCH_HISTORY/SNAPSHOTS/DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/`.
Они не являются действующей DSP/UI спецификацией.

Активная DFB-панель P1/P2 оставляет строгую BASE `RAW / 64` при
`DYNAMICS=ON`, но GUARD теперь привязан к RAW segment: RAW `0..63` является
bypass, RAW `63..64` мягко arm-ит safety follower, а RAW `64..127` использует
четыре persistent anchors `START @64`, `PLT @64`, `START @127`, `PLT @127`.
`GUARD OFFSET` сдвигает весь диапазон; `GUARD CURVE`, `GUARD AMOUNT`, `ATTACK`
и `RELEASE` остаются ручными project/preset controls. Factory defaults:
START/PLT на обоих endpoints `.55/.73`, OFFSET `0`, CURVE `1`, AMOUNT `1`,
ATTACK `37 ms`, RELEASE `184 ms`. Это user safety/governor design, не заявленная
найденная hardware formula.

DFB открывается по RMB как compact movable child overlay внутри VST editor, а
не отдельное ОС-окно: DAW focus не перехватывается. Весь текст белый; оба graph
и live marker остаются. Overlay закрывается крестом или повторным RMB по DFB.
В reference defaults включены `DYNAMICS`, оба `DSND ± FB INVERT`, `RETURN COMP`
и `FB CLIP / GUARD`.

Старые anchor/tail APVTS IDs и schema-40 `dfb_guard_raw` сохранены для старых
project/preset state, но не показываются и не управляют active implementation.
Schema `<43` переносит saved START/PLATEAU в одинаковые anchors @64/@127 и
добавляет OFFSET `0`; сохранённые CURVE/AMOUNT/ATTACK/RELEASE не изменяются.

По пользовательскому решению неработающий DLY `core` снят из DSP-menu,
`TrackDelay.inl`, state owner, tests и shipping Source. Остаются `mnm|old|new`.
Сохранённый DLY value `3` из schema 40/41 при загрузке явно мигрирует в `new=2`;
не подтверждается никакая firmware-exact claim для DFB или Track Delay.

DBAS/DWID и текущий 12 dB feedback-filter response сохранены, slope selector
не добавлялся. `FxSlotEngine` не получил speculative DFB wiring и остаётся
`DFB_ROUTE_UNRESOLVED`.

## Проверка исходников

Предыдущий target-local `verify_dsp_mode_patch.py` сохранён как исторический
snapshot в `PATCH_HISTORY/SNAPSHOTS/RETIRED_VERIFY_DSP_MODE_PATCH/`. Он
проверял pre-cleanup registry и не является текущим executable audit.

Для текущей поставки запускаются корневые
`python3 DFB_GUARD_CORE_STATIC_CHECK.py` и
`python3 FM_MODE_CLEANUP_STATIC_CHECK.py`: первый проверяет schema 43, DFB
RAW 63..64 arm и @64/@127/OFFSET parameter/runtime/live-graph wiring, embedded
VST panel, снятие DLY CORE, обновлённые исходные tests и byte-identical зеркала
Synth/FX; второй сохраняет расширенный
retained FM/CHOR/Phaser/metadata audit. Проверка source-only и не доказывает
hardware-эквивалентность.

Ни рендер, ни полная VST3-сборка, ни DAW-вывод о фактическом delay в этой
коррекции не заявляются.
