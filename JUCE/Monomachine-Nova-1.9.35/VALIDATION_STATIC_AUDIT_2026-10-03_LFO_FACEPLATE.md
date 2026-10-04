# Статический аудит LFO PAGE/DEST faceplate — 1.9.35

Дата исходного record: 03.10.2026; corrective update: 04.10.2026.  
Тип проверки: только source/static audit; без сборки, открытия editor,
VST3/DAW, DSP/CTest и render.

## Scope и сохранение истории

Проверяется только active `Source/PluginEditor.cpp`, byte-identical между:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Предыдущая версия record сохранена до corrective update в
`PATCH_HISTORY/SNAPSHOTS/ACTIVE_1.9.35_PRE_MATRIX_REGRESSION_CORRECTION_2026-10-03/`.

## PAGE / DEST: readable value без потери P2 связи

Raw PAGE/DEST values, target IDs, routing и state не изменяются. Faceplate
по-прежнему использует compact local control token для маленькой physical cell:

- `P1 AMP PORT` / existing popup `AMP PORT` → `PORT`;
- `P1 LFO4 DPTH` / existing popup `LFO4 DPTH` → `DPTH`;
- `P2 SYNT 1FRQ` → `1FRQ`;
- `P2 AMP MIX` → `MIX`.

Ключевая P1 correction: `choiceName()` по-прежнему снимает только redundant
`P1 ` в текущем dropdown, поэтому faceplate получает `AMP PORT` или
`LFO1 DPTH`. Отдельная faceplate-only branch берёт последний token и возвращает
`PORT`/`DPTH`; popup, LFO LOCKS, target IDs и Matrix не меняются.

Это сохраняет requested `21 px` PixelFont value в available width `87 px`:
длиннейший fixed token `PITCH` при scale 3 занимает точно `5*6*3-3 = 87 px`.

Corrective правило применяется только к **redundant P1-on-P1** prefix. P2
никогда не скрывается симметрично: P2 badge остаётся видимым на P2 PAGE/DEST
faceplate icon, а P2 full name остаётся в popup/list. При cross-page P2 choice
остаётся и P1 marker, потому что он описывает реальную страницу/target связь.

## PAGE `PITCH`: scale 3 как у `SYNT`

`PITCH` состоит из пяти glyphs. При requested `21 px` `PixelFont` сначала
пытается scale 3, но его conservative fit check требует `5*6*3 = 90 px`;
обычный faceplate inset имеет только 87 px, поэтому прежний `PITCH` визуально
падал на scale 2 и выглядел меньше `SYNT`.

Только direct-LFO PAGE/DEST LCD теперь использует весь 91 px cell:

```cpp
const bool directLfoChoiceValue=pageIndex>=3&&(knob==0||knob==1);
const int valueX=directLfoChoiceValue?0:2;
const int valueW=directLfoChoiceValue?getWidth():getWidth()-4;
```

Итог: `PITCH` P1/P2 получает scale 3. `DPTH` сохраняет normal `PAN` 87 px
inset; popup, LFO LOCKS, Matrix и state не затронуты.

## Direct-LFO DPTH: восстановлена исходная PAN geometry

Повторная визуальная feedback-коррекция не меняет LFO state, target IDs или
UNI/INV semantics. Она возвращает только faceplate placement, который был до
высвобождения места для compact controls:

```cpp
const juce::Rectangle<int> directDepthKnob{getWidth()/2-26,27,52,43};
// directLfoChoiceValue is false for DPTH:
const int valueX=directLfoChoiceValue?0:2;
const int valueW=directLfoChoiceValue?getWidth():getWidth()-4;
pixel::text(g,compactValue,{valueX,getHeight()-26,valueW,23},21,true);
```

Это в точности normal rotary/LCD contract `PAN`: `DPTH` располагается по центру,
а `0` использует всю доступную 87 px LCD ширину. `UNI` и `INV` остаются
вычисленными от directDepthKnob и не меняют bounds ручки или LCD.

## P1/P2 badge

Badge расположен справа и ниже относительно прежнего положения:

```text
old: {cx+12, cy-20, 28, 15}, request 12 px → scale 1
new: {cx+15, cy-17, 30, 18}, request 16 px → scale 2
```

Внутренняя ширина равна 26 px; `P2` при scale 2 занимает 22 px. Поэтому marker
не может collapse до scale 1 и не закрывает compact faceplate value.

## WAVE dropdown placement

Удалён неверный horizontal clamp, который сравнивал surface-local `panelRight`
с global `getScreenX()`. Каждый choice popup, включая LFO `WAVE`, получает
exact global rectangle самой cell:

```cpp
const juce::Rectangle<int> popupAnchor{
    getScreenX(), getScreenY(), getWidth(), getHeight()
};
...withTargetComponent(this).withTargetScreenArea(popupAnchor)
```

JUCE сохраняет screen-aware above/below choice, а origin dropdown остаётся у
соответствующей ручки даже при сдвинутом host window.

## Выполненный static audit

Запущен только:

```bash
python LFO_FACEPLATE_STATIC_CHECK.py
```

Результат:

```text
LFO FACEPLATE STATIC AUDIT: PASS (28 checks; source-only, no build/test/render run)
```

Проверены Synth/FX mirror, target-local DEST compaction, правило
P1 own DEST leaf compaction (`AMP PORT`→`PORT`, `LFO1 DPTH`→`DPTH`) без
изменения popup/LOCKS/Matrix, PITCH full-cell 91 px scale-3 correction, P2
badge сохранить, PAN-equivalent DPTH/full LCD, 21 px value, scale-2 badge,
отсутствие old P2 suppression, WAVE popup anchor и pixel-font arithmetic.
Визуальный runtime render намеренно не заявляется.