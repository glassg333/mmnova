# mnm_flt_dist_juce — Фильтр (FLT) и дисторшн (DIST) Monomachine для JUCE

Декомпилированные блоки **FILTER** (два фильтра + Q + фильтровые огибающие
BOFS/WOFS) и **DIST** (аттенюация/перегруз, без огибающих) тракта
Monomachine SFX-60 MkII, OS 1.32B — из дизассемблированного ядра DSP56303.

Полная доказательная база (адрес → формула, таблицы, OPEN-пункты):
**DECOMPILATION_NOTES.md** — читать перед правками.

## Что внутри

* `source/` — чистый C++17 **без зависимостей от JUCE** (header-only);
  включается в любой JUCE-проект (или standalone).
* `test/test_sanity.cpp` — самопроверка фактов декомпиляции:
  `g++ -std=c++17 -I../source -o test_sanity test_sanity.cpp && ./test_sanity`
* `reference/` — выписки листинга ядра по всем реализованным регионам +
  отчёт кросс-проверки таблиц.

## Быстрый старт

```cpp
#include "MnmVoiceFilterDist.h"

mnm::MnmVoiceFilterDist fx;
fx.setSampleRate(44100.0);
fx.setFiltWords(
    mnm::wordFromKnob(32),   // BASE  — низкий срез (low cut)
    mnm::wordFromKnob(80),   // WDTH  — ширина створа (верхний срез = BASE+WDTH, hi cut)
    mnm::wordFromKnob(84),   // HPQ   — резонанс верхнего фильтра
    mnm::wordFromKnob(31),   // LPQ   — резонанс нижнего фильтра
    mnm::wordFromKnob(10),   // FLT ATK
    mnm::wordFromKnob(40),   // FLT DEC
    mnm::wordFromKnob(58),   // BOFS  — env→BASE (биполярная, 64 = нейтраль)
    mnm::wordFromKnob(80));  // WOFS  — env→WDTH (биполярная, 64 = нейтраль)
fx.setDistKnob(20);          // DIST −64..+63 (без огибающих)
fx.setAmpWords(mnm::wordFromKnob(0),   // AMP ATK
               mnm::wordFromKnob(0),   // AMP HOLD
               mnm::wordFromKnob(64),  // AMP DEC
               mnm::wordFromKnob(64),  // AMP REL
               mnm::wordFromKnob(0));  // AMP SUS (уровень удержания)
fx.trigger();                // FILTER-trig / нота ON

// в processBlock — по сэмплу (модуль сам буферизует кадры по 16):
float outL = fx.processL(inL);
float outR = fx.processR(inR);
```

Обвязка `juce::AudioProcessor` (MIDI, параметры) — `source/MnmFilterDistJuceExample.h`.

## Важные свойства

* **Сетка кадров 16 сэмплов** (родная сетка машины). Латентность фильтра =
  ровно 1 кадр (~0.36 мс @44.1к) — это свойство прошивки, не «задержка плагина».
* **Таблицы — дословные слова прошивки** (17 таблиц из `dsp1_pmem.bin`,
  SHA-256 и кросс-чек — в DECOMPILATION_NOTES §2 и reference/table_report.json).
* Каскад `func_000340` и резонатор `func_000397` — **бит-в-бит** верифицированы
  авторами пака filter_phaser_pack против эмулятора DSP56300
  (svf_recursion_proven.md).
* DIST — точный закон ядра: `x0 = max(0, 2D−1)`, `k = 0.98379·x0²+0.01621`,
  ступень `4k` с жёстким клипом ±1.0 (24-бит слово), кривая привода затемняет
  срез. Ручка −64..+63: меньше нуля — ослабляется вход (фильтр слабее
  перегружается), больше нуля — дист усиливается.
* Порядок тракта: вход → аттенюация DIST → ФИЛЬТР (кольцо → каскад →
  резонанс/Q) → DIST (4k+клип) → AMP-гейт → выход.

## Что НЕ включено (чтобы не перепутать)

* track delay и его два фильтра (без Q и без огибающих) — P:$0939–$0B49;
* «rotation resonator» stage2 (машина delay-модуляции) — P:$0A5D–$0AD0;
* FM-машины и их огибающие freq 1/2/3;
* EQ-полоса (EQF/EQG).

## Честные ограничения

Открытые места декомпиляции (WOFS-ветка $56B, шкала трекинга, согласование
доменов tone↔кольцо, кривая ColdFire для DIST, банковая ступень $07BD–$07D1,
Q-интерполятор $0629–$0649) перечислены в DECOMPILATION_NOTES §4 и изолированы
в коде с пометками. Всё остальное — дословная декомпиляция или бит-в-бит
верификация из первоисточников.
