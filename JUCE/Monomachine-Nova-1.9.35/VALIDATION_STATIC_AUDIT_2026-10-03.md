# Статический аудит — schema 41 DFB GUARD LVL и DLY CORE

Дата: 03.10.2026. Версия active source metadata: **1.9.30**.

## Выполнено

Запускались только source-only static audits:

```bash
python3 DFB_GUARD_CORE_STATIC_CHECK.py
python3 FM_MODE_CLEANUP_STATIC_CHECK.py
```

Результат актуального запуска: **PASS, 101 checks** для DFB/GUARD LVL/CORE и
**OK** для расширенного retained-FM/CHOR/Phaser/metadata audit. В checks входит
только текстовая проверка обновлённых test sources и их Synth/FX-зеркал; сами
tests не исполнялись.

Проверено статически:

- schema `41`, selector `DLY: mnm|old|new|core` и безопасная migration-грань;
- strict `RAW / 64` BASE; GUARD LVL не уменьшает BASE на raw `64` и ниже;
- отдельное level-window управление: `LVL START=.55`, `LVL END/PLATEAU=.73`,
  `CURVE=1`, `AMOUNT=1`, `ATTACK=37 ms`, `RELEASE=184 ms`;
- migration из schema `<41`: добавление LVL START, neutralisation прежнего
  raw-curve в level CURVE `1`, сохранение PLATEAU/ATTACK/RELEASE/AMOUNT;
- отдельное компактное `DocumentWindow` DFB, которое можно перемещать за
  границы VST editor; белые надписи и отсутствие старых anchor controls;
- реальный `TrackDelayCore` dispatch, самостоятельные ring/filter state и
  16-frame / 0.1 / ±4 retune-инварианты;
- byte-identical Synth/FX mirror для изменённых общих исходников и active tests;
- project metadata `VERSION 1.9.30`.

Дополнительно выполнен только лексический scan скобок в изменённых C++ source
files: **PASS**. Он не является компиляцией.

## Намеренно не выполнялось

Не запускались сборка, CTest, plugin/executable DSP tests, render, VST3 или
DAW-проверка. Это статический аудит исходников, а не подтверждение звучания
или hardware-exact DFB/Track Delay formula.

`VALIDATION_STATIC_AUDIT_2026-10-02.md` и
`VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt` сохранены как материалы предыдущих
source-only ревизий.
