# Статический аудит динамической подсказки LFO SPD — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только source/static audit; без сборки, открытия editor,
VST3/DAW, DSP/CTest и render.

## Scope

В обеих active ветках (`Monomachine_Nova_Synth` и `Monomachine_Nova_FX`)
изменён только общий UI source `Source/PluginEditor.cpp`; он синхронизирован
byte-identical.

Подсказка назначается именно для direct-LFO `SPD` cell (`pageIndex >= 3`,
`knob == 5`), то есть для всех независимых `P1 LFO1..6` и `P2 LFO1..6`.
`Cell` получил `SettableTooltipClient`, поэтому hover работает как на faceplate
cell, так и на вложенной rotary handle.

## Что показывает hover SPD

Текст UI намеренно English/ASCII. При hover он имеет следующую форму:

```text
LFO SPEED / SPD: 64 x MULT 2X | TEMPO NOW: 120.0 BPM
FULL CYCLE: 16 1/16 = 2.000 s = 0.500 Hz
HALF MODE: 8 1/16 = 1.000 s | LAW: 2048 / (SPD x MULT)
```

- `TEMPO NOW` читается через существующий `processor.currentBpm()`: это live
  `tempoDisplay`, который processor заполняет host tempo при включённом HOST
  SYNC либо global BPM в internal режиме.
- `SPD` читается из текущего active LFO control, а `MULT` — из index 4 того же
  LFO parameter group. MULT index `0..6` остаётся точным набором
  `1X..64X` (`1 << index`).
- Full cycle строго следует принятому calculator law
  `2048 / (SPD * MULT)` в sixteenth-notes. Секунды считаются как
  `steps * 15 / BPM`, Hz — как обратная величина. `HALF MODE` показывает
  половину full-cycle length для trigger mode `HALF`.
- При `SPD=0` вместо деления на ноль текст явно показывает
  `FULL CYCLE: STOPPED (SPD 0)`.

Подсказка обновляется в существующем UI timer с частотой 20 Hz, но cache не
перезаписывает строку, пока live BPM/SPD/MULT не изменились. Она отключается
в choice/pick state и очищается при rebind другой cell.

## Выполненный source-only audit

Запущены только текстовые проверки:

```bash
python LFO_SPEED_TOOLTIP_STATIC_CHECK.py
python LFO_FACEPLATE_STATIC_CHECK.py
python LFO_TIMING_STATIC_CHECK.py
```

Результаты:

```text
LFO SPEED TOOLTIP STATIC AUDIT: PASS (35 checks; source-only, no build/test/render run)
LFO FACEPLATE STATIC AUDIT: PASS (15 checks; source-only, no build/test/render run)
LFO TIMING STATIC AUDIT: PASS (32 checks; source-only, no build/test/render run)
```

Новый audit проверяет зеркальность Synth/FX, scope только для LFO SPD,
`currentBpm()`/`tempoDisplay`, active-LFO `MULT`, calculator law, output
`steps/seconds/Hz/HALF`, zero-SPD guard, English strings, dynamic timer refresh
и cache. Его два примера арифметики — `SPD=64, MULT=2X, BPM=120` и
`SPD=32, MULT=1X, BPM=120` — являются чистой статической проверкой формулы,
не DSP/plugin execution.

Runtime UI render, compilation, VST3/DAW, CTest, DSP test и audio render
намеренно не заявляются.
