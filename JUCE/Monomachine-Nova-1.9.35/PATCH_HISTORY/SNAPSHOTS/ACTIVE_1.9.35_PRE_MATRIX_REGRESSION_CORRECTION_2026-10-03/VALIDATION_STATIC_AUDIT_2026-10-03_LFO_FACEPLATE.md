# Статический аудит LFO PAGE/DEST faceplate — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только source/static audit; без сборки, открытия editor,
VST3/DAW, DSP/CTest и render.

## Исправленный scope

Изменён и byte-identical синхронизирован в обеих active ветках только
`Source/PluginEditor.cpp`:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

## PAGE / DEST: читаемый faceplate value

`PAGE` и `DEST` по-прежнему хранят и показывают полные canonical P1/P2 names
в popup и lock panel. На маленькой physical cell это приводило к автоматическому
уменьшению pixel-font: например, `P2 SYNT <parameter>` не помещался в 87 px
нижней строки.

Новая faceplate-only compaction не меняет raw PAGE/DEST value, routing или
state:

- у `PAGE` убирается только redundant side prefix (`P2 LFO1` → `LFO1`);
- у `DEST` PAGE уже задаёт банк, поэтому на faceplate остаётся только local
  target-control token (`P2 SYNT 1FRQ` → `1FRQ`, `P2 AMP MIX` → `MIX`,
  `P1 LFO4 DPTH` → `DPTH`);
- P1/P2 side продолжает показываться отдельным badge на icon.

Итог: длинный routing name не заставляет value уйти в scale 1. Для самой длинной
fixed faceplate строки `PITCH` расчёт PixelFont остаётся `5*6*3-3 = 87 px`,
то есть ровно в доступной строке `91-4 = 87 px` при requested `21 px` height
(scale 3).

## P1/P2 badge

Badge перенесён относительно старой позиции вправо и ниже:

```text
old: {cx+12, cy-20, 28, 15}, request 12 px → scale 1
new: {cx+15, cy-17, 30, 18}, request 16 px → scale 2
```

Внутренняя ширина badge равна 26 px; `P2` при scale 2 занимает 22 px. Поэтому
буквы P1/P2 больше не могут автоматически сжаться до мелкого scale 1 и остаются
видимыми на icon выбранного P1/P2 PAGE или DEST.

## WAVE dropdown placement

Удалён неверный horizontal clamp, который сравнивал surface-local `panelRight`
с global `getScreenX()`. При сдвинутом окне host это давало искусственный `x`
далеко от originating WAVE cell.

Теперь каждый choice popup, включая LFO `WAVE`, получает exact global rectangle
самой ячейки:

```cpp
const juce::Rectangle<int> popupAnchor{
    getScreenX(), getScreenY(), getWidth(), getHeight()
};
...withTargetComponent(this).withTargetScreenArea(popupAnchor)
```

JUCE сохраняет нормальный screen-aware выбор above/below, но origin dropdown
теперь привязан к соответствующей ручке, а не к смешанной системе координат.

## Выполненный static audit

Запущен только:

```bash
python LFO_FACEPLATE_STATIC_CHECK.py
```

Результат:

```text
LFO FACEPLATE STATIC AUDIT: PASS (15 checks; source-only, no build/test/render run)
```

Проверены mirror Synth/FX, target-local DEST compaction, 21 px value request,
новые bounds/font P1/P2 badge, отсутствие старого local/screen popup clamp,
anchor WAVE choice и pixel-font width arithmetic. Визуальный runtime render
намеренно не заявляется.
