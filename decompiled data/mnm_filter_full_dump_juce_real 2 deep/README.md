# mnm_filter — DSP-фильтры Monomachine SFX-60 OS 1.32B

Готовый C++ фильтр, повторяющий алгоритмы голосовой цепочки Elektron Monomachine.
Все коэффициенты встроены напрямую в заголовок `src/mnm_filter_tables.h` — это
**те самые значения** из заводской прошивки (`elektron_sfx6-60_os1.32b.bin`),
SHA-256 сверен с `SHA256SUMS.txt` репозитория `glassg333/mmnova`.

## Две реализации

| Файл | Уровень точности | Стандарт |
|---|---|---|
| `src/MnmFilter.h` | **Convenience** — float32, упрощённая структура | Математически эквивалентен прошивке, ~-100..-134 dB разница vs 24-bit |
| `src/MnmFilterFaithful.h` | **Faithful** — 24-bit q.23 fixed-point, 1:1 с DSP56300 инструкциями | Как хорус (ChorusCore.cpp), ~0 LSB при правильной эмуляции saturation |

## Сравнение с хорусом

Хорус был перенесён **построчно** (каждая DSP56300 инструкция → одна C++ строка),
с сохранением 24-бит fixed-point арифметики, порядка параллельных переносов и
saturation. Это верифицировано сверкой с dsp56k-emulator.

| Класс | Соответствие хорусу | Подтверждение |
|---|---|---|
| `MnmOnePole` (convenience) | ~-100..-134 dB vs 24-bit (математически эквивалентен) | `make compare` → все тесты < -84 dB |
| `MnmOnePoleFaithful` | ✓ bit-точный порт P:$147787-79D | 1:1 с ChorusCore.cpp строки 288-306 |
| `MnmBaseWidthFilter` | концептуально верно, ~-100 dB | нужен построчный порт P:$A5F-ABF |
| `MnmSVF` (convenience) | ✗ упрощён (Chamberlin vs kernel) | константы x0/x1 верны, но структура отличается |
| `MnmSVFFaithful` | ⚠ структура 1:1, но нет per-voice коэф. | нужен реверс ColdFire для V-$18 |

## Что внутри

| Класс | Что это | Адрес в прошивке | Формула |
|---|---|---|---|
| `mnm::MnmOnePole` | 1-полюсный IIR LP/HP (convenience, float32) | kernel `P:$A5F-$ABF`, FX-CHORUS `P:$147787` | `y[i] = (1-a)·y[i-1] + a·x[i]` |
| `mnm::MnmOnePoleFaithful` | 1-полюсный IIR LP (bit-точный, 24-bit q.23) | FX-CHORUS `P:$147787-79D` | 1:1 с DSP56300 инструкциями |
| `mnm::MnmBaseWidthFilter` | Каскад HP+LP+aux | kernel `P:$A5F-$ABF` | 3× 1-pole в серии |
| `mnm::MnmSVF` | 2-pole multimode SVF (LP/BP/HP/Notch/Allpass/Peak) | kernel `P:$5D3-$5FA` | Chamberlin SVF; константы `x0=$F528BD=-0.0847476`, `x1=$4A4DF0=+0.5811615` |
| `mnm::MnmSVFFaithful` | 2-pole SVF (структурный порт, не функциональный без V-$18) | kernel `P:$5D3-$5FA` | 1:1 с kernel, но нужны per-voice коэф. |

## Структура каталогов

```
mnm_filter/
├── Makefile                       # make / make run / make compare
├── README.md                      # этот файл
├── LICENSE                       # MIT
├── src/
│   ├── MnmFilter.h               # convenience-реализации (float32)
│   ├── MnmFilterFaithful.h      # bit-точные порты (24-bit q.23, 1:1 с DSP56300)
│   └── mnm_filter_tables.h       # встроенные таблицы коэффициентов
├── demo/
│   ├── demo.cpp                  # тест-демонстрация (5 тестов)
│   └── compare.cpp               # сравнение convenience vs faithful
├── demo_output_sample.txt       # пример вывода demo
└── compare_output_sample.txt    # пример вывода compare (со всеми dB-разницами)
```

## Сборка и запуск

```bash
make            # собрать demo + compare
make run        # собрать и запустить demo
make compare    # собрать и запустить compare (convenience vs faithful)
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

// BIT-ТОЧНАЯ версия 1-pole LP (как хорус)
mnm::MnmOnePoleFaithful lp_fxp;
lp_fxp.setTone(64);
mnm::fxp::s24 in_q23 = mnm::fxp::from_float(input_sample);
mnm::fxp::s24 out_q23 = lp_fxp.processOne(in_q23);
float result = mnm::fxp::to_float(out_q23);
```

## Соответствие прошивке

Все коэффициенты в `mnm_filter_tables.h` извлечены **прямо из бинарника**
(не из вторичных JUCE-портов, которые могут содержать ошибки/гипотезы).
Скрипт экстрактора: `scripts/gen_tables_header.py` (повторяемая регенерация).

Верификация: все 18 таблиц сверены байт-в-байт с `.txt`-дампами из
`04_tables/` — расхождений: 0.

## Известные ограничения

1. **MnmSVF (convenience)**: использует классический Chamberlin SVF вместо
   реального kernel-алгоритма. Константы x0/x1 правильные, но структура другая.
   Чтобы достичь уровня хоруса, нужен построчный порт P:$5D3-5FA с per-voice
   коэффициентами из V-$18 (требуется реверс ColdFire MCF5206e).
2. **Точность**: convenience-версии используют float32, а не 24-bit fixed-point.
   Разница ~-84..-134 dB в зависимости от cutoff (см. `compare_output_sample.txt`).
   Для бит-точности используйте `MnmFilterFaithful.h`.
3. **Per-voice TONE/Q/MODE mapping**: оригинальный SVF kernel использует
   константы x0/x1 + 6 коэффициентов в Y:$91+ (загружаются из V-$18). Эти
   6 коэффициентов заполняются ColdFire-стороной (требуется дизассемблирование
   MCF5206e). MnmSVFFaithful имеет структуру 1:1 с kernel, но не функционален
   без этих коэффициентов.
4. **Runtime-таблицы** (sine 8K, LFO speed, FM wavetable, decay/damp реверба)
   не включены — они инициализируются ColdFire в runtime и в firmware пусты.

## Источники

- Прошивка: `01_firmware/elektron_sfx6-60_os1.32b.bin` (SHA-256 в SHA256SUMS.txt)
- Образы DSP56300: `02_memory_images/dsp1_{p,x,y}mem.bin` (re-derived из firmware)
- Листинги: `03_listings/dispatch/dsp1_kernel_P0000-0B4D.txt`
- Документация: `06_docs/KERNEL_VOICE_CHAIN.md`
- Эталонный порт хоруса: `08_cpp_reference/chorus_plugin/ChorusCore.cpp`
