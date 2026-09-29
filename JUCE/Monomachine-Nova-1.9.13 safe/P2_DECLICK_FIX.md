# P2 manual-control de-click — current native paths

## Scope

Этот документ относится к P2 controls, включая:

- MIX выбранного FX-слота;
- group P2 MIX;
- P2 DIST;
- **P2 VOL** — это именно control, который в исходном сообщении был назван
  «AMP»;
- P2 PAN.

В Synth и FX используется общий `TrackChain`, поэтому сохранённая обработка
параметра DIST действует одинаково для соответствующих P1/P2 native путей.

## Текущее решение

У ручки DIST сохранён существующий трёхмиллисекундный аудиочастотный de-zipper:

- первая обработка инициализируется текущим target, поэтому стартовое/neutral
  состояние не меняется;
- затем сглаженное значение движется к control-cadence target по сэмплам;
- то же значение подаётся только в сохранённый `mnm` saturation path либо в
  сохранённый `old` bipolar reserve path.

Это не limiter, не второй DIST-блок и не подмена алгоритма. Не менялись
оригинальный список P2 FX-слотов, native `MachineEngine`, DELAY-маршрут или
CHORUS default. SAFE остаётся только явным тестовым non-original toggle; по
умолчанию используется native CHORUS.

## Rollback boundary

В текущем release source package отсутствуют FMA, source-derived `new`
селекторы и отдельный P2 core selector. Поэтому проверка de-click относится
исключительно к сохранённым native/old путям, а не маскирует проблему
экспериментальной ветвью.

`tests/P2DeclickTests.cpp` в каждом продукте сохраняет matched-pair coverage
для machine MIX, group MIX, DIST, VOL и PAN. Дополнительные rollback-проверки
находятся в `ModeRollbackTests.cpp` и `RollbackStateTests.cpp`.
