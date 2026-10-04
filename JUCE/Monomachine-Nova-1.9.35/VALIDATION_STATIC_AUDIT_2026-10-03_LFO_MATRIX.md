# Статический аудит LFO / MOD MATRIX — 1.9.35

Дата: 03.10.2026.  
Тип проверки: только исходный код, без компиляции и без запуска DSP.

## Проверенный scope

Проверка выполнена для обеих активных веток:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Для шести активных файлов подтвержден byte-identical mirror:

- `Source/NovaData.h`;
- `Source/PluginProcessor.h`;
- `Source/PluginProcessor.cpp`;
- `Source/PluginEditor.cpp`;
- `Source/models/DspModes.hpp`;
- `Source/models/modulation_matrix.hpp`.

## Результаты source audit

1. Schema равна `45`. Новые persistent APVTS IDs существуют для каждого из 12
   LFO: `mod_mode`, `mod_inv`, `mod_alt_dual`; у каждого Matrix route существует
   `rN_inv`.
2. Direct `DPTH` имеет fresh default `0`; normal UI ограничивает его диапазоном
   `0..127`, а `ALT DUAL` открывает `-127..0..+127`.
3. Runtime сначала применяет UNI/BI, затем `INV`; signed depth допускается
   только при `ALT DUAL`. Matrix runtime применяет route `INV` после UNI/BI.
4. P2 direct target назначается через локальный offset банка (`target - 132`,
   `target - 164` или `target - 212`), а не через сырой `target % 8`.
   Поэтому P2 `DSND`, `EQF` и остальные физические controls не меняются местами.
5. Matrix содержит столбец `INV` с состояниями `OFF`/`INV`; inverted route
   рисует DEPTH graph в обратную сторону. Сортировка поддерживает все восемь
   рабочих столбцов, включая `INV`, `AUX SRC` и `AUX DEPTH`.
6. `P1 INTERNAL LFO` и `P2 INTERNAL LFO` являются отдельными collapsible
   группами по шесть LFO. RMB на их `DPTH` вызывает persistent panel данного
   LFO; faceplate controls используют те же APVTS параметры.
7. Центральный Matrix `DEST` menu использует hover-aware custom rows. Hover
   устанавливает видимое `PREVIEW` в Matrix и запрашивает outline реального
   faceplate target; закрытие или выбор menu очищает preview.
8. Лексический audit delimiter-ов (`()[]{}`), проверка связей callback и поиск
   устаревшего direct `target % 8` подтверждены для Synth и FX.

## Не выполнялось

- полная или частичная сборка;
- CTest, executable/DSP tests;
- VST3/DAW проверка;
- audio render;
- визуальный runtime render UI.

Следующий gate перед бинарной поставкой — обычная пользовательская сборка и
ручная проверка UI в DAW. Этот документ не заявляет успешную компиляцию или
звуковую валидацию.
