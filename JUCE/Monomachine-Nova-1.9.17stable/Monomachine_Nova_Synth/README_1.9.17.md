# Monomachine Nova Synth 1.9.17

## Меню MODE L / MODE H

- MODE L показывает только обычные `LOW CUT / HP`; MODE H — только `HIGH CUT / LP`. Противоположные response-типы не мешают обычному выбору.
- Все BP остаются в одной папке `BAND PASS`; для MODE L после неё добавлена последняя папка `DRY`.
- `R HYPER` находится последним внутри HP, LP и BP-папок; `R HYPER NOTCH` остаётся отдельным.
- `R HUV/KRAJ/MICRO/MUSIC/OBERHEIM/DVAL HP4` вынесены в MODE L → `DRY`. Это не обычные HP-реализации: пять используют `dry - LP`, DVAL — `dry + LP`. Удаление прямого входа не даст честный HP, поэтому они явно названы тестовой отдельной группой.
- Popup, колесо и drag используют одинаковую фильтрацию видимых пунктов.

## Проверка

В Synth и FX прошли JUCE-free C++17 `Import2FiltersTests`, `HybridDspTests` и `FilterRouteTests`; статический integrity-check также прошёл для обоих деревьев. Полная JUCE/VST3/DAW-сборка и слуховая проверка не выполнялись.

## Не заявляется

1.9.17 не исправляет и не объявляет решёнными native Monomachine FILT/Q/BOFS, AMP ENV или DIST.
