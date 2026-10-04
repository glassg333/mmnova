# Статический аудит LFO HALF / SPD / MULT — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только исходный код и текстовые инварианты; без компиляции,
запуска DSP/plugin/CTest, VST3/DAW-проверки и audio render.

## Внешняя сверка timing law

Проверены два независимых источника:

1. Руководство Elektron Monomachine OS 1.32: `SPD` — линейная
   tempo-related base speed, `MULT` умножает её; при росте `MULT` в два раза
   полный цикл становится в два раза короче. Manual приводит полезные прямые
   значения `SPD` 16, 32, 64, 127 и anchor: `SPD=64`, `MULT=2x` даёт полный
   LFO-цикл на 16 sequencer sixteenth-notes.
2. Страница Modbang Elektron Tools действительно содержит видимую кнопку
   **`Calculate LFO Settings`**. В её inline JavaScript найден алгоритм
   `baseSteps = 2048` и `calculatedSteps = baseSteps / (spd * mult)`.
   То есть кнопка позволяет однозначно выявить прежнюю ошибку: ранее active
   source применял экспоненциальную, центрированную в 64 кривую `SPD`, а не
   линейное произведение `SPD × MULT`.

Принятая tempo-relative law для одного полного цикла:

```text
cycle length in 1/16 steps = 2048 / (SPD * MULT)
cycles per second          = BPM * SPD * MULT / 30720
```

`30720 = 2048 * 15`, потому что при tempo `BPM` за секунду проходит
`BPM / 15` sixteenth-notes. Например, при `120 BPM`, `SPD=64`, `MULT=2x`
получается `0.5 Hz`: 2 секунды, то есть ровно 16 шестнадцатых. При
`SPD=64`, `MULT=1x` полный цикл равен 32 шестнадцатым; при `SPD=127`,
`MULT=1x` — около 16.13 шестнадцатых, что объясняет manual recommendation
для прямого bar timing.

## Изменённые active sources

Одинаково в `Monomachine_Nova_Synth` и `Monomachine_Nova_FX` обновлены:

- `Source/NovaDSP.h`;
- `Source/NovaData.h`;
- `Source/models/parameter_conversions.hpp`;
- `Source/models/track_pages.hpp`;
- `Source/PluginProcessor.cpp`.

### HALF

- `TRIG` choices теперь: `FREE | TRIG | HOLD | ONE | HALF`; `HALF` имеет
  final raw value `4`, поэтому ранее существующие значения `0..3` не
  переиндексированы.
- При LFO trigger режим `HALF` сбрасывает phase на начало selected waveform,
  запускает ход только до `0.5` цикла, отсекает возможный overshoot строго на
  `0.5`, после чего останавливается.
- В момент остановки сохраняется фактический output после `INTL` в
  `halfHeld`; этот уровень возвращается неизменным до следующего trigger.
  Это относится также к inverse, square, exponential, ramp и random shape.
- `FREE`, `TRIG`, `HOLD` и `ONE` не были переименованы и не были
  переиндексированы.

### SPD / MULT

`nova::LFO::rateCyclesPerSecond()` больше не использует старую формулу
`2^((SPD-64)/24)`. `SPD` ограничивается raw диапазоном `0..127` и является
линейным множителем; `MULT` берётся из дискретных labels `1X, 2X, 4X, 8X,
16X, 32X, 64X`. Поэтому любой `MULT` ровно вдвое изменяет tempo-relative
cycle length, как в manual/calculator law.

Новые функциональные descriptions в active `track_pages.hpp` остаются
English: `Trigger Mode (FREE/TRIG/HOLD/ONE/HALF)`,
`Tempo multiplier (1x..64x)` и `Tempo-synchronised linear LFO speed`.

## 12 независимых LFO и state

Все 12 active groups остаются независимыми APVTS groups:
`P1 LFO1..6` и `P2 LFO1..6` (страницы `3..14`). Каждая группа сохраняет
собственное выбранное значение `TRIG`, в том числе raw `HALF=4`; новый
отдельный shared state не добавлен. Это сохраняет independent persistent
settings каждого LFO и не затрагивает уже принятые direct-LFO/MOD MATRIX
UNI/BI, INV, ALT DUAL, target-local P2 DEST и hover-preview правила.

На runtime `prepareToPlay` и `panic` теперь очищают все четыре массива
`lfos`, `lfos3`, `lfos2`, `lfos4`, то есть все P1/P2 LFO1..6; note trigger
уже fan-out'ит во все 12 экземпляров.

## Выполненный статический audit

Запущен только:

```bash
python LFO_TIMING_STATIC_CHECK.py
```

Результат:

```text
LFO TIMING STATIC AUDIT: PASS (32 checks; source-only, no build/test/render run)
```

Скрипт проверяет byte-identical Synth/FX mirror, final `HALF` choice в APVTS и
legacy LCD conversion, отсутствие прежней экспоненциальной SPD law, формулу
`2048 / (SPD * MULT)`, половинный phase endpoint и held final level, 12
independent parameter arrays, trigger fan-out и reset всех 12 LFO.

## Не выполнялось

- full/partial build;
- CTest или executable/DSP tests;
- запуск VST3/DAW;
- audio render либо прослушивание.

Следующий gate для звукового утверждения — отдельное прямое разрешение на
сборку и manual DAW test. Этот документ подтверждает только source/static
состояние.
