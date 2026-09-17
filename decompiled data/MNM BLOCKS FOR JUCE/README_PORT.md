# MMNova → JUCE port kit (Monomachine-Nova-1.6.0)

Порт DSP-ядра Elektron Monomachine SFX-60 **OS 1.32B** в блоки JUCE.
Источник истины: репозиторий `glassg333/mmnova`, каталог `decompiled data`
(аудит `00_ОТЧЁТ_аудит_данных.md`, 2026-09-18), прошивка
`elektron_sfx6-60_os1.32b.bin`, SHA-256 `369849…acabec7e`.

ВАЖНО: в репозитории существует только ветка `main` (JUCE + decompiled data).
Ветки/каталога с исходниками плагина Monomachine-Nova-1.6.0 там нет, поэтому
порт сделан НЕ как патч к вашему коду, а как самодостаточный модуль
`Source/mmnova/`, не зависящий от ваших классов. Точки стыковки с плагином —
только два места, см. раздел «Интеграция».

--------------------------------------------------------------------------
## 1. Что соответствует железу 1:1

| блок | адрес в прошивке | файл порта | статус |
|---|---|---|---|
| диспетчер машин (34 входа) | X:$10016B / $10018D / $1001AF | MnMMachineFactory.h | A |
| pre-voice: portamento + pitch-LFO | P:$262–$2EA | MnMVoice.cpp `preVoice()` | A/B |
| экспонента питча wt[i]=0.5·2^(i/2048) | X:$140000, 2048 слов, 11 октав | MnMTables.cpp `buildPitchTable()` | A (аналитика, 1e-9) |
| масштаб инкремента | $1D22A | MnMTables.h `kPitchScale` | A |
| вызов машины блоком 16 кадров | P:$2EB `jsr (r1)` | MnMVoice.cpp `renderBlock()` | A |
| post-voice: AMP env + LFO + SVF | P:$4A8–$5FB | MnMVoice.cpp `postVoice()` | A/B |
| rate-таблица AMP/LFO | Y:$141800, 128 слов | MnMTables (FW dump) | C |
| SVF ×2 | коэф. $F528BD / $4A4DF0 | MnMVoice.cpp `SVF2` | B |
| CHORUS полностью | P:$147661–$1477DE (382 слова) | machines/MnMFxChorus.h | A |
| LP-таблица (TONE/LP) | P:$144AC7, 258 слов | MnMTables (FW dump) | C |
| FM ratio table (24 отношения) | P:$141A80 | MnMTables.cpp `kFmRatios` | A |
| FM+STAT / FM+PAR / FM+DYN | $145D21 / $145EDB / $1461C1 | machines/MnMFm.h | A(структура)/C(wavetable) |
| FM-DYN wavetable | P:$141880 | MnMTables (FW dump) | C |
| delay-буфер | X:$114000, 2×2048, канал B +$800 | MnMFxChorus.h / MnMFxMisc.h | A |
| дескрипторы параметров (23 машины) | ColdFire 0x57FC5, шаг 0xB0 | MnMDescriptors.cpp | A |

Поправки аудита, учтённые в порте:
* **m14 = SWAVE-ENS, а не FX-EQ.** Отдельной EQ-машины в 1.32B нет.
* Найдены **m22 DPRO-DDRW** и **m23 DPRO-DENS** (страницы B/C).
* MIX всегда `out = dry·($7FFFFF−MIX) + wet·MIX` → при MIX=127 остаётся 0.78 % сухого.
  Это НЕ баг, не «чинить».
* Дисторшна как машины нет — это `bset #$14,sr` (сатурация ALU) внутри chorus/FM.
* Fade-in wet первые 128 блоков (~46 мс) — часть характера, не убирать.

--------------------------------------------------------------------------
## 2. Интеграция в Monomachine-Nova-1.6.0

1. Скопировать каталог `Source/mmnova` в проект.
2. В CMake добавить строки из `CMakeLists.mmnova.txt` (или просто добавить
   `Source/mmnova/mmnova.cpp` в `target_sources`, остальное header-only).
3. В вашем `PluginProcessor`:

```cpp
#include "mmnova/MnMEngine.h"
...
mmnova::Engine engine;                       // 6 треков Monomachine
void prepareToPlay (double sr, int) override { engine.prepare (sr); }
void processBlock (juce::AudioBuffer<float>& b, juce::MidiBuffer& m) override
{
    engine.process (b, m);                   // сам разбирает note on/off и CC
}
```

4. Параметры: `mmnova::descriptors()` отдаёт 23 машины × 8 имён + заводские
   дефолты — можно строить APVTS автоматически (см. `MnMDescriptors.h`).

Движок работает во внутреннем sample-rate 44100 Гц (как железо) и ресемплит
на выход линейно, если хост идёт на другой частоте: `Engine::setHostRate`.
Отключить ресемплинг: `MMNOVA_NATIVE_RATE_ONLY`.

--------------------------------------------------------------------------
Статус точности
A — байт-в-байт по листингу (chorus P:$147661–$1477DE, kernel P:$262/$4A8, FM ratio P:$141A80, дескрипторы @0x57FC5).

B — структура из дизассемблера, коэффициенты аналитические (pitch wt[i]=0.5·2^(i/2048), sine 8192, SVF $F528BD/$4A4DF0).

C — требует дампа таблицы: tools/extract_tables_fixed.py (исправлен баг off = addr−0x100000 из аудита, Часть 5.1).

## 3. Таблицы прошивки (статус C)

Три таблицы не выводятся аналитически и должны быть выдернуты из образов:

| таблица | адрес | слов |
|---|---|---|
| LP-коэффициенты (TONE/LP) | P:$144AC7 | 258 |
| rate AMP-env + LFO SPD | Y:$141800 | 128 |
| FM-DYN wavetable | P:$141880 | 1024 |
| SWAVE / DPRO-BBOX / SID-filter | см. TABLES_MANIFEST.json | — |

Запуск (у себя, БЕЗ копирования образов в проект):

```
python3 tools/extract_tables_fixed.py \
    --pmem  "decompiled data/02_memory_images/dsp1_pmem.bin" \
    --ymem  "decompiled data/02_memory_images/dsp1_ymem.bin" \
    --out    Source/mmnova/MnMFirmwareTables.h
```

После этого собирать с `-DMMNOVA_HAVE_FW_TABLES=1`. Без флага модуль
компилируется и звучит, но LP/rate/wavetable берутся из аналитических
приближений (файл честно печатает `#warning`).

Скрипт исправляет баг оригинального `extract_tables.py` из репозитория:
там для всех адресов ≥ $100000 делалось `off = addr − 0x100000`, из-за чего
**26 из 43** дампов оказались нулевыми (Часть 5.1 аудита). Здесь адресация
прямая (внешняя SRAM алиасится между P:/X:/Y:, доказано в KERNEL_VOICE_CHAIN.md §1),
плюс проверка «дамп не нулевой» с ненулевым exit code.

--------------------------------------------------------------------------
## 4. Точность арифметики

Весь горячий тракт считается в семантике DSP56300 (`MnMFixed.h`):
* 24-битные дробные слова, 56-битный аккумулятор (int64),
* `mpy`/`mac` = знаковое умножение со сдвигом влево на 1 бит,
* `bset #$14,sr` = сатурация на ±1.0 (`sat24`),
* `asr #$10` для индексации таблиц параметром (param<<16 → 7-битный индекс),
* фазовые аккумуляторы 48-битные (L:$5 = X:$5/Y:$5), маска фазы $1FFF + линейная
  интерполяция синуса на 8192 точках.

Float-путь включается макросом `MMNOVA_FLOAT_PATH` (быстрее, звук чуть мягче).

--------------------------------------------------------------------------
## 5. Лицензия / происхождение

Код написан по результатам реверс-инжиниринга (описания алгоритмов), а не
скопирован из прошивки. Таблицы прошивки в комплект НЕ входят — они дампятся
локально из ваших образов.
