# Статический аудит alignment P1/P2 в LFO LOCKS — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только source/static audit; без сборки, открытия editor,
VST3/DAW, DSP/CTest и render.

## Scope

В обеих active ветках byte-identical обновлён только
`Source/PluginEditor.cpp`, внутри `LfoLockPanel`. Persistent LFO PAGE/DEST
parameters, lock masks, solo values, direct routing и DSP не менялись.

## Выравнивание PAGE columns

Первая PAGE column по-прежнему имеет 11 visual rows: `PITCH`, затем side-local
`SYNT`, `AMP`, `FILT`, `EFFX` и шесть LFO groups. Во второй PAGE column раньше
было только 10 строк (`PAGE 11..20`), поэтому её первый `SYNT` находился выше
соответствующего `SYNT` первой стороны.

Во вторую column добавлена dim English/ASCII visual-only строка:

```text
P2 PITCH N/A
```

Она стоит в visual row 0 без lock/solo glyph, без current-value outline и без
parameter value. Реальные `PAGE 11..20` сдвинуты на visual rows `1..10`, поэтому
`P1/P2 SYNT`, `AMP`, `FILT`, `EFFX` и LFO1..6 теперь расположены на одинаковых
горизонтальных строках.

Это только spacer:

- hit-test для second-column row 0 возвращает `-1`;
- `mouseDown` и `mouseDrag` завершаются до изменения lock/solo/page state;
- visual rows `1..10` точно декодируются обратно в existing persistent values
  `11..20`;
- количество PAGE choices остаётся 21, единственный physical PITCH target —
  existing PAGE 0.

Следовательно, placeholder нельзя выбрать, залочить, solo-назначить или
записать в state; он только сохраняет геометрию двух списков параллельной.

## Выполненный source-only audit

Запущены только текстовые проверки:

```bash
python LFO_LOCK_ALIGNMENT_STATIC_CHECK.py
python LFO_SPEED_TOOLTIP_STATIC_CHECK.py
python LFO_FACEPLATE_STATIC_CHECK.py
python LFO_TIMING_STATIC_CHECK.py
```

Результаты:

```text
LFO LOCK ALIGNMENT STATIC AUDIT: PASS (20 checks; source-only, no build/test/render run)
LFO SPEED TOOLTIP STATIC AUDIT: PASS (35 checks; source-only, no build/test/render run)
LFO FACEPLATE STATIC AUDIT: PASS (19 checks; source-only, no build/test/render run)
LFO TIMING STATIC AUDIT: PASS (32 checks; source-only, no build/test/render run)
```

Новый audit проверяет byte-identical Synth/FX editor/data sources, 11 visual
rows в обеих PAGE columns, exact `P2 PITCH N/A` spacer, отсутствие lock/solo/
selection state на нём, mapping `visual 1..10 → PAGE 11..20`, неизменные mask/
solo ranges и неизменные 21 PAGE choices. Его mapping arithmetic является
чистой source-level проверкой, не UI/plugin execution.

Runtime UI render, compilation, VST3/DAW, CTest, DSP test и audio render
намеренно не заявляются.
