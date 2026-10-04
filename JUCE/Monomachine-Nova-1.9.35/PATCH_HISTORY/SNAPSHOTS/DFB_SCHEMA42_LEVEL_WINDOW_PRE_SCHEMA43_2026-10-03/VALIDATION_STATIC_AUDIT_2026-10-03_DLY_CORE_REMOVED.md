# Статический аудит — schema 42, снятие DLY CORE

Дата: 03.10.2026. Версия active source metadata: **1.9.30**.

## Выполнено

Запускались только source-only static audits:

```bash
python3 DFB_GUARD_CORE_STATIC_CHECK.py
python3 FM_MODE_CLEANUP_STATIC_CHECK.py
```

Результат актуального запуска: **PASS, 93 checks** для DFB/GUARD LVL,
снятия DLY CORE и target identity; расширенный retained-FM/CHOR/Phaser/metadata
audit — **OK**.
Проверялись только исходники, manifests и текст обновлённых test sources;
сами tests не исполнялись.

Проверено статически:

- schema `42`, selector `DLY: mnm|old|new` и отсутствие `core` в menu,
  `TrackDelay.inl`, `NovaDSP.h`, active tests и shipping Source;
- saved DLY значение `3` из schema 40/41 явно мигрирует в `new=2`; остальные
  неверные/out-of-era значения не выбирают соседний renderer;
- strict `RAW / 64` BASE и level-window GUARD LVL без уменьшения BASE на raw
  `64` и ниже;
- отдельное compact `DocumentWindow` DFB, которое можно перемещать за границы
  VST editor; белые надписи и отсутствие старых anchor controls;
- сохранённые schema-39/40 DFB IDs, безопасная schema-41 GUARD LVL migration,
  P1/P2 parameter wiring и byte-identical Synth/FX common sources/tests;
- project metadata `VERSION 1.9.30` и target-specific identity: Synth
  `NOVA_SYNTH=1`, FX `NOVA_SYNTH=0`.

Дополнительно выполнен только лексический scan скобок в изменённых C++ source
files: **PASS**. Он не является компиляцией.

## Намеренно не выполнялось

Не запускались сборка, CTest, plugin/executable DSP tests, render, VST3 или
DAW-проверка. Это статический аудит исходников, а не подтверждение звучания.

`VALIDATION_STATIC_AUDIT_2026-10-03.md`,
`VALIDATION_STATIC_AUDIT_2026-10-02.md` и
`VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt` сохранены как материалы предыдущих
source-only ревизий.
