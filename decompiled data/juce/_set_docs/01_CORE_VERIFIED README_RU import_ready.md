# 01_CORE_VERIFIED — канонический набор для JUCE

Единственный каталог, из которого бери код в плагин. Состав:

```
dsp/
  MnmFixed.h        Q1.23 / 56-бит MAC / sat24 / деление DSP56300
  MnmKernelTables.h 17 таблиц прошивки ДОСЛОВНО (~6000 слов, якоря проверены)
  MnMAmpDist.h      AMP: огибающая AHDR [EXACT] + РЕАЛЬНЫЙ DIST (P:$079F-07D1)
  MnMFilter24.h     РЕАЛЬНЫЙ фильтр: срез из таблицы прошивки, кламп $6A3,
                    сглаживание 1/256, Q по 3 банкам, 2 SVF-петли
  MnMVoiceChain.h   СКВОЗНАЯ цепочка: env → DIST → фильтр → пан/громкость
  MnmRouting.h      роутинг треков: буферы, INSERT-семантика FX, fxMix, INP
  MnmParams.h       23 машины × 8 ручек + дефолты (из дескрипторов ColdFire)
  machines/
    ChorusCore.*    FX-CHORUS побитово (принят в плагин)
    MnmBBox.h       DPRO-BBOX: каркас + ROM-банк (FIR/RTIM приближены)
data/
  kernel/           таблицы (.bin 24-bit BE + .txt с float)
  bbox_drum_bank.bin      банк BBOX из прошивки
  pitch_wavetable_2048.bin X:$140000
rig/                измерительный стенд: исполняет НАСТОЯЩЕЕ ядро OS 1.32B
docs/               ROUTING_RU, BBOX_RU, KERNEL_CHAIN_RU, ERRATA к нему,
                    VERIFICATION_REPORT_RU (главный отчёт перепроверки)
TEST_REPORT.txt     прогон численных проверок (см. ниже)
```

## Быстрый старт

```cpp
#include "dsp/MnMVoiceChain.h"
using namespace mnmsys;

MnmVoiceChain vc;
MnmVoiceParams p;
p.dist = 0.5f + 0.5f * distKnob / 127.0f;   // закон ColdFire — OPEN, консервативно
p.ampAtk = ...; p.fBase = ...;              // страницы 0..1 как в прошивке
vc.setParams(p);

// в processBlock (блоками по 16!):
for each 16-frame chunk:
    vc.noteOn();            // по нотам
    vc.processBlock16(in, outL, outR, 16);
```

## Инварианты «звучит как оригинал»

1. Не float: весь тракт в Q1.23 с sat24 (`MnmFixed.h`), блоки 16 кадров.
2. Таблицы — из `MnmKernelTables.h`/`data/`, не синтезировать.
3. DIST: параметр в терминах прошивки 0.5..1.0; ниже — пассивен (это не баг).
4. MIX: `out = ($7FFFFF−MIX)·dry + MIX·wet` — при 127 остаётся 0.78% dry.
5. Пан-закон и точная разводка Y:$91 помечены OPEN — см. VERIFICATION_REPORT §5.

## Проверка целостности

Таблицы генерируются скриптом из образов прошивки; якоря:
`P:$1447C6=7FFFFF`, `P:$143546=000D35`, `P:$1449C6+256=7FFFFF`,
`Y:$141880+127=800000`. Если сомневаешься в файле — сверь эти слова.
