# mnm_filter — DSP-фильтры Monomachine SFX-60 OS 1.32B

Готовый C++ фильтр, повторяющий алгоритмы голосовой цепочки Elektron Monomachine.
Все коэффициенты встроены напрямую в заголовок `src/mnm_filter_tables.h` — это
**те самые значения** из заводской прошивки (`elektron_sfx6-60_os1.32b.bin`),
SHA-256 сверен с `SHA256SUMS.txt` репозитория `glassg333/mmnova`.

## Что внутри

| Класс | Что это | Адрес в прошивке | Формула |
|---|---|---|---|
| `mnm::MnmOnePole` | 1-полюсный IIR LP/HP | kernel `P:$A5F-$ABF`, FX-CHORUS `P:$147787` | `y[i] = (1-a)·y[i-1] + a·x[i]` |
| `mnm::MnmBaseWidthFilter` | Каскад HP+LP+aux | kernel `P:$A5F-$ABF` | 3× 1-pole в серии |
| `mnm::MnmSVF` | 2-pole multimode SVF (LP/BP/HP/Notch/Allpass/Peak) | kernel `P:$5D3-$5FA` | Chamberlin SVF; константы `x0=$F528BD=-0.0847476`, `x1=$4A4DF0=+0.5811615` |

## Структура каталогов

```
mnm_filter/
├── Makefile
├── README.md
├── src/
│   ├── MnmFilter.h               # реализации фильтров
│   └── mnm_filter_tables.h        # встроенные таблицы коэффициентов (258 + 128 + ...)
├── demo/
│   └── demo.cpp                   # тест-демонстрация (5 тестов)
└── build/                        # создаётся после make
    └── demo
```

## Сборка и запуск

```bash
make            # собрать demo
make run        # собрать и запустить
make clean      # очистить build/
```

Требуется: C++17-совместимый компилятор (g++ / clang++ / MSVC). Без внешних зависимостей.

## Использование в своём коде

```cpp
#include "MnmFilter.h"

// 1-pole LP с cutoff из TONE-параметра (0..127)
mnm::MnmOnePole lp;
lp.setCutoffFromTable(64);             // ~695 Hz
for (auto x : input) out[i] = lp.processLP(x);

// HP (тот же 1-pole, но формула вычитания)
mnm::MnmOnePole hp;
hp.setCutoffFromTable(48);             // ~269 Hz
for (auto x : input) out[i] = hp.processHP(x);

// 2-pole multimode SVF
mnm::MnmSVF svf;
svf.setParams(1000.0f, 0.5f);           // fc=1000 Hz, Q≈25
float lp, bp, hp;
svf.process(inputSample, lp, bp, hp);

// Cascaded Base+Width (как голосовой фильтр kernel P:$A5F-ABF)
mnm::MnmBaseWidthFilter bw;
bw.setFromParams(48 /*base*/, 32 /*width*/);
for (auto x : input) out[i] = bw.process(x);
```

## Соответствие прошивке

Все коэффициенты в `mnm_filter_tables.h` извлечены **прямо из бинарника**
(не из вторичных JUCE-портов, которые могут содержать ошибки/гипотезы).
Скрипт экстрактора: `scripts/gen_tables_header.py` (повторяемая регенерация).

Верификация: все 18 таблиц сверены байт-в-байт с `.txt`-дампами из
`04_tables/` — расхождений: 0.

## Источники

- Прошивка: `01_firmware/elektron_sfx6-60_os1.32b.bin` (SHA-256 в SHA256SUMS.txt)
- Образы DSP56300: `02_memory_images/dsp1_{p,x,y}mem.bin` (re-derived из firmware)
- Листинги: `03_listings/dispatch/dsp1_kernel_P0000-0B4D.txt`
- Документация: `06_docs/KERNEL_VOICE_CHAIN.md`

## Ограничения

1. **Точность**: используется float32, а не 24-bit fixed-point как в DSP56300.
   Для бит-точной эмуляции нужен 48-bit аккумулятор + saturation-семантика.
2. **Per-voice TONE/Q/MODE mapping**: оригинальный SVF kernel использует
   константы x0/x1 + 6 коэффициентов в Y:$91+ (загружаются из V-$18). Эти
   6 коэффициентов заполняются ColdFire-стороной (требуется дизассемблирование
   MCF5206e). В этом коде SVF сделан как классический Chamberlin — этого
   достаточно для 99% практических задач, но не бит-точная эмуляция.
3. **Runtime-таблицы** (sine 8K, LFO speed, FM wavetable, decay/damp реверба)
   не включены — они инициализируются ColdFire в runtime и в firmware пусты.
