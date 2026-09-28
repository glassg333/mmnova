# Monomachine Nova 1.9.16

## Добавлено

- MODE L/H сохраняет сериализованные IDs `0..51` и добавляет шесть HP-комплементов `52..57`: `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`, `R OBERHEIM HP4`, `R DVAL HP4`.
- Меню сортируется по фактическому DSP-ответу: HP/low-cut находится в первом разделе MODE L, LP/high-cut — в первом разделе MODE H.
- Комплементы получены из самих LP-ядер; это расширения/диагностические варианты, не модели оригинальной прошивки Monomachine. У DVAL инвертирован знак LP-выхода, поэтому его HP — `dry + LP`; у остальных пяти — `dry - LP`.
- Schema 28 принимает новые IDs; более старые schemas не переинтерпретируют их.

## Проверка

В Synth и FX прошли JUCE-free C++17 `Import2FiltersTests`, `HybridDspTests` и `FilterRouteTests`. Это проверяет исходные карты, спектральный ответ комплементов и маршруты, но не заменяет сборку JUCE/VST3, загрузку в DAW или слуховое сравнение.

## Не заявляется

1.9.16 не исправляет и не объявляет решёнными native Monomachine FILT/Q/BOFS, AMP ENV или DIST. Для границ исследования см. `../FILTER_SOURCE_EVIDENCE_2026-09-28.md` в source-пакете.
