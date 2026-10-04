# Статический аудит — DFB BASE/GUARD и DLY CORE

Дата: 02.10.2026. Версия active source metadata: **1.9.30**.

## Выполнено

Запускались только source-only static audits:

```bash
python3 DFB_GUARD_CORE_STATIC_CHECK.py
python3 FM_MODE_CLEANUP_STATIC_CHECK.py
```

Результат: **PASS, 88 checks** для DFB/GUARD/CORE и **OK** для расширенного
retained-FM/CHOR/Phaser/metadata audit. В эти 88 checks входят только текстовая
проверка обновлённых test sources и их зеркал; сами tests не исполнялись.

Проверено статически:

- schema `40`, selector `DLY: mnm|old|new|core` и безопасная migration-грань;
- strict `RAW / 64` BASE, новые `GUARD START/CURVE/AMOUNT` с reference
  `65 / .10 / 1`, а также defaults `PLATEAU=.73`, `ATTACK=37 ms`,
  `RELEASE=184 ms`;
- сохранение старых schema-39 DFB IDs в APVTS/state и отсутствие старых anchor
  controls в активной RMB-panel;
- белый текст/marker в DFB RMB-panel;
- реальный `TrackDelayCore` dispatch, самостоятельные ring/filter state и
  16-frame / 0.1 / ±4 retune-инварианты;
- byte-identical Synth/FX mirror для изменённых общих исходников;
- project metadata `VERSION 1.9.30`.

## Намеренно не выполнялось

Не запускались сборка, CTest, plugin/executable DSP tests, render, VST3 или
DAW-проверка. Это статический аудит исходников, а не подтверждение звучания
или hardware-exact DFB/Track Delay formula.

Предыдущий `VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt` сохранён без изменения
как журнал предшествующей source-only ревизии.
