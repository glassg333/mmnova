# Параметры: ручки, диапазоны, значения по умолчанию

Все значения — из таблиц `Source/lab/EngineRegistry.cpp` (совпадают со структурами
`Parameters` движков в `Source/engines/`).

Идентификаторы параметров у хоста: `s<номер>_<ключ>` (например `s0_delay1`), модель — `s0_model`,
Trim страницы — `s0_trim`. Нумерация с 0.

## Общие (не зависят от страницы)

| ID | Подпись | Диапазон | По умолчанию | Что делает |
|----|---------|----------|--------------|------------|
| `slot` | активная страница | 0..4 | 0 | выбор страницы, автоматизируется |
| `mix` | Dry/Wet | 0..1 | 1.0 | 1.0 = только выход движка; меньше — подмешивается сухой вход |
| `output` | Output | −24..+24 дБ | 0 дБ | общая громкость плагина |
| `scan_rate` | Scan Rate | 0..2 Гц | 0.09 Гц | частота встроенной LFO на Scan (0 = статичный скан) |
| `scan_depth` | Scan Depth | 0..1 | 0.55 | глубина LFO на Scan |
| `reset_on_switch` | Reset on switch | вкл/выкл | вкл | чистить состояние движка при уходе/приходе страницы |
| `s<n>_trim` | Trim (на каждой странице) | −24..+24 дБ | 0 дБ | подровнять громкость варианта для честного A/B |

Формула LFO: `scan = 0.5 + (base − 0.5)·(1 − depth) + 0.5·depth·sin(2π·t·rate)`,
где `base` — значение ручки Scan.

---

## Страница 1 — variants 1 (fav) — БАЗА

`other/zigcomb/comb_scanner_variants 1 fav`, модель 01 = «Original Hypothesis» — это и есть
базовый DSP, сверенный с оригинальной Max-схемой (`zzz pic ref amxd original code scheme/cs ref`).

| Ключ | Подпись | Диапазон | По умолчанию |
|------|---------|----------|--------------|
| `gain` | Gain | 0..0.999 | 0.99 |
| `damp` | Damp | 0..0.999 | 0.90 |
| `phase` | Phase | 0..1 | 0.75 |
| `delay1` | Delay 1 | 0..2000 мс | 115 мс |
| `delay2` | Delay 2 | 0..2000 мс | 500 мс |
| `scan` | Scan | 0..1 | 0.0 |
| `model` | Model | 5 вариантов | 01 Original Hypothesis |

Оставлены только осмысленные модели: **01 Original Hypothesis, 02 Tight Crossfade,
03 Long Resonator, 04 Scrub Stretch, 06 Ping Pong** (5, 7–10 звучали почти одинаково).

Соответствие оригиналу: формула фидбека `feedback = 0.55 + 0.435·gain`, демпфирование
`0.08 + 0.20·(1 − damp)`, all-pass `0.12 + 0.80·phase` (кламп ±0.92), вторая линия
`input·0.42 + feedback·(0.52·reson + 0.48·allpass)`, сухой сигнал 0.12, выход `tanh(4.5·…)`.
Скан — кроссфейд между комбами (equal-power), панорамы нет. Прыжки Delay — мгновенные
(де-клик кроссфейдом ~5 мс), в петле мягкий лимитер (аналог `omx.peaklim~` из схемы).

## Страница 2 — variants 2

`other/zigcomb/comb_scanner_variants 2`, те же 5 моделей.

| Ключ | Подпись | Диапазон | По умолчанию |
|------|---------|----------|--------------|
| `gain` | Gain | 0..0.999 | 0.99 |
| `damp` | Damp | 0..0.999 | 0.90 |
| `phase` | Phase | 0..1 | 0.75 |
| `delay1` | Delay 1 | 0..2000 мс | 115 мс |
| `delay2` | Delay 2 | 0..2000 мс | 500 мс |
| `scan` | Scan | 0..1 | 0.0 |
| `model` | Model | 5 вариантов | 01 Original Hypothesis |

Здесь Phase сделана слышимой (глубина движения + лёгкое расстройство ±2.5% второй линии);
ручка Character убрана — звук открытый; Damp управляет демпфированием и заводом.

## Страница 3 — variants 3 (ctrl)

`other/zigcomb/comb_scanner_variants 3 more control`, те же 5 моделей.

| Ключ | Подпись | Диапазон | По умолчанию |
|------|---------|----------|--------------|
| `feedback` | Feedback | 0..0.999 | 0.99 |
| `damp` | Damp | 0..0.999 | 0.90 |
| `phase` | Phase | 0..1 | 0.75 |
| `diffusion` | Diffusion | 0..1 | 0.55 |
| `crossmix` | Cross Mix | 0..1 | 0.50 |
| `motion` | Motion | 0..1 | 0.55 |
| `delay1` | Delay 1 | 0..2000 мс | 115 мс |
| `delay2` | Delay 2 | 0..2000 мс | 500 мс |
| `scan` | Scan | 0..1 | 0.0 |
| `model` | Model | 5 вариантов | 01 Original Hypothesis |

## Страница 4 — cs_mm

`other/zigcomb/cs_mm`

| Ключ | Подпись | Диапазон | По умолчанию |
|------|---------|----------|--------------|
| `feedback` | Feedback | 0..0.999 | 0.62 |
| `damp` | Damp | 0..0.999 | 0.62 |
| `phase` | Phase | 0..1 | 0.35 |
| `diffusion` | Diffusion | 0..1 | 0.45 |
| `crossmix` | Cross Mix | 0..1 | 0.50 |
| `motion` | Motion | 0..1 | 0.50 |
| `chmix` | Chorus Mix | 0..1 | 0.58 |
| `chdepth` | Chorus Depth | 0..1 | 0.60 |
| `chrate` | Chorus Rate | 0..1 | 0.35 |
| `delay1` | Delay 1 | 0..2000 мс | 20 мс |
| `delay2` | Delay 2 | 0..2000 мс | 45 мс |
| `scan` | Scan | 0..1 | 0.0 |
| `model` | Model | 20 вариантов | 01 Harmonic Chorus Clean |

Дефолты задержек уменьшены до 20/45 мс — мелкий металлический звук на фидбеке, как ты просил.
Каждый комб модулируется всегда (при Motion = 0 остаётся слабая модуляция со своей частотой
и фазой у каждого комба), панорама от скана убрана, прыжки Delay мгновенные.

Модели (20): 01 Harmonic Chorus Clean, 02 Harmonic Chorus Wide, 03 Inharmonic Glass Flange,
04 Inharmonic Phase Teeth, 05 Stretched Slow Bloom, 06 Stretched Fast Motion, 07 Cluster Dark Swarm,
08 Cluster Bright Swarm, 09 Broken Tape Drift, 10 Percussive Metallic, 11 Prime Ring, 12 Prime Wide,
13 Octave Chorus, 14 Octave Phase, 15 Shimmer Comb, 16 Shimmer Dark, 17 Rubber Resonator,
18 Rubber Stereo, 19 Frozen Metal, 20 Liquid Metal.

## Страница 5 — zigzag

`other/zigcomb/1 try` + `2 try/zigzag_juce_package` (файлы идентичны)

| Ключ | Подпись | Диапазон | По умолчанию |
|------|---------|----------|--------------|
| `decay` | Decay | 0..200 | 29.74 |
| `damping` | Damping | 0..0.984 | 0.10 |
| `rotate` | Rotate | 0..1 | 0.25 |
| `fluctuate` | Fluctuate | **0..0.016** | 0.0 |

Damping можно увести в 0, Decay расширен до 200 (длиннее хвост).
**Fluctuate — диапазон 0..0.016, как в референсном патче** (`fluctuate 0`, там же
`rotate 0.0236`, `damping 0.1`, `delay 20 ms`): это доли процента, а не «хаос на 100%».

Это не комб-сканер: четыре линии задержки до 3 секунд, Hadamard-перемешивание, вращение пар,
DC-safe демпфирование и четыре all-pass диффузора из `zigzag_1.maxpat`.
Ручки соответствуют патчевым RADIUS / JUMP / ANGLE / MOVE (Radius = 0 даёт максимальный фидбек).
