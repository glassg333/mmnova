# mnmdsp_reference — DSP Monomachine в блоках для JUCE

Референс-пак по декомпиляции Elektron Monomachine SFX-60 MKII OS 1.32B.
Каждый блок — самостоятельный заголовок без зависимостей от JUCE (интегрируется
куда угодно: AudioProcessor, SynthesiserVoice, dsp::ProcessorChain).

## Состав

```
dsp/
  MnmFixed.h        арифметика DSP56300: Q1.23 слова, 56-бит MAC, сатурация,
                    однополюсник y+=c*(x-y), питч-инкремент через таблицу ROM
  MnmRouting.h      РОУТИНГ kernel'а: 3 буфера треков + EXT, выбор входа по
                    флагам, перезапись/добавление буферов (INSERT-семантика
                    FX), сумма в микс, точные ступени fxMixStage()/inpGain()
  MnmParams.h       карта ручек ВСЕХ 22 машин: имена, порядок (p_i -> V+$04+i),
                    дефолты (сгенерировано из дескрипторов ColdFire)
  MnmAmp.h          AMP-страница: огибающая ATK/HOLD/DEC/REL [EXACT, проверено
                    исполнением ядра] + DIST-каскад (кривая привода из ROM,
                    k = 0.985461·D² + 0.016230, клип sat(128x)) + по-сэмпловые
                    рампы усиления (4-tap интерполятор ядра)
  MnmFilter.h       FILT-страница: 2-pole SVF (TPT), коэффициент среза из
                    таблицы прошивки P:$143546 (экспонента 2.8 Гц…6.6 кГц!),
                    сглаживание коэффициента как в ядре, HPQ/LPQ резонанс
  MnmKernelTables.h таблицы ядра VERBATIM из OS 1.32B: ATK/LFO rates, DEC/REL
                    множители, LFO cycle, кривая привода, kernel curve
  machines/
    MnmBBox.h       DPRO-BBOX faithfully: 12 ROM-моделей, нот-маппинг, STRT,
                    RTRG/RTIM-ролл, полифазный FIR из ROM, жёсткий гейт
    ChorusCore.*    FX-CHORUS побитовый порт (из аудита 1.32B, с приёмкой)
data/
  bbox_drum_bank.bin        банк барабанов BBOX из прошивки (24-bit BE)
  bbox_wav/model_*.wav      те же модели отдельными файлами для прослушки
  pitch_wavetable_2048.bin  X:$140000 — экспоненциальная таблица питча
  kernel/*.bin|.txt         таблицы ядра (AMP/LFO rates, DEC, кривая DIST,
                            cutoff-таблица фильтра, curves) — дампы P-памяти
rig/                        ИЗМЕРИТЕЛЬНЫЙ СТЕНД: исполняет НАСТОЯЩЕЕ ядро
                            OS 1.32B в эмуляторе dsp56k (dsp56300/dsp56300).
                            map / env / sig / chain / distcurve режимы.
                            Патчи эмулятора: MACI, MPYRI, PFLUSH.
docs/
  ROUTING_RU.md     соседний трек / INP / MIX — с адресами инструкций
  BBOX_RU.md        разбор BBOX и отличия от порта Nova
  KERNEL_CHAIN_RU.md голосовой тракт ядра: AMP env [EXACT] / DIST / FILT /
                     пан/усиление — все адреса, таблицы, статус, методика
```

## Быстрый старт (JUCE)

```cpp
#include "dsp/machines/MnmBBox.h"

// prepareToPlay / конструктор:
std::vector<int32_t> bank, pitch;
mnmdsp::loadBBoxBank(BinaryData::bbox_drum_bank_bin, size1, bank); // или из файла
// pitch-таблица: тот же лоадер для pitch_wavetable_2048.bin
bbox.setBank(bank.data(), (int)bank.size());
bbox.setPitchTable(pitch.data());
bbox.setKnobs(64, 0, 0, 0);            // PTCH STRT RTRG RTIM (0..127, как ручки)

// processBlock:
bbox.processBlock(outL, outR, numSamples);   // numSamples кратно 16
```

Хорус уже интегрирован в Nova FX (`ChorusCore.cpp`) — в пакете копия для
полноты. Для остальных FX-машин используй `MnmRouting.h`: любая FX-машина =
`вход×inpGain → обработка → fxMixStage(dry, wet, MIX)`.

## Правила, чтобы «звучало как оригинал»

1. Слова 24-бит Q1.23, аккумулятор 56-бит, сатурация при записи
   (`MnmFixed.h`), не float — на громких местах прошивка сатурирует иначе.
2. Блок 16 кадров — все LFO/огибающие машин тикают по-блочно (см. хорус:
   фаза LFO инкрементируется раз в блок).
3. Параметры приходят уже отмасштабированными (Q1.23), порядок p_i = V+$04+i.
4. FX-машина = INSERT: вход-буфер соседа + перезапись того же буфера
   (`MnmRouting.h`), MIX = `($7FFFFF−MIX)·dry + MIX·wet`.
5. Таблицы не «синтезировать», а брать из `data/` (они из прошивки).

## Статус портирования машин

| машина | состояние |
|---|---|
| FX-CHORUS | готово, побитово (ChorusCore) |
| DPRO-BBOX | готов каркас + банк ROM; FIR-расписание и RTIM приближены (см. docs) |
| FX-THRU   | тривиален: `out = in·4·INP²` (16 кадров) |
| AMP-страница (ENV) | ГОТОВО, [EXACT] — MnmAmp.h, проверено исполнением ядра |
| AMP-страница (DIST) | структура из ROM: k-формула + кривая + клип sat(128x); обвязка состояния — после ColdFire-маппинга |
| FILT-страница | структура SVF + таблица срезов из ROM (MnmFilter.h); закон HPQ/LPQ — после ColdFire-маппинга |
| FM+ STAT/PAR/DYN, VO-6, SID, SWAVE-*, DPRO-WAVE/DDRW/DENS, GND-*, FX-REV/DYN/RING/PHA/FLA | листинги и ручки есть (архив), порт — следующая итерация по той же схеме |

Дорожная карта порта одной машины: листинг → выделить константы/таблицы →
перенести структуру (обычно: init/config/process) → приемка на совпадение
выхода с эмулятором DSP (dsp56k) на фиксированном входе.
