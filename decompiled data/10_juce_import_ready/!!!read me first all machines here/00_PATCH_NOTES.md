# DSP_New v3 — все точные ядра OS 1.32B одним пакетом (12 модулей)

> **ЧТО НОВОГО В v4 (2026-09-25, вторая поставка дня):** добавлены
> `trackdelay_srr/STATUS.md` (добыча Track Delay P:$0939-$0A5B + SRR:
> кольцо 16384×2, таблицы (v+1)/256 и 1/(v+1), темп-K 7938000, лебедка
> ±4 кадра, SRR² → Y:$4FF; бит-точное ядро — следующая сессия) и
> `machine_map_fix/README.md` (T0-карта всех 21 машин из ROM ColdFire;
> ИСПРАВЛЕНО: m32 = DPRO-DDRW (не DLY), m33 = DPRO-DENS (не EXT),
> m14 = SWAVE-ENS (не EQ) — машин DLY/EQ в прошивке НЕТ, строка «DLY»
> в ROM 0 раз). Очередь пользователя: Track Delay+SRR → REV → SIN/NOIS →
> SID/SAW → PULS/ENS/WAVE → VO-6. Шаги правок кода v3 не изменены.

> **ЧТО НОВОГО В v3 (2026-09-25, поставка «все готовые для импорта вещи в одном
> месте»):** добавлены `flanger/` (FX-FLANGER m19), `dyn/` (FX-DYNAMIX m16,
> компрессор), `chorus_strict/` (машина 15, строгий ROM для A/B),
> `bbox_startpos/PATCH_bbox_pitch.md` (ПИТЧ-правила bbox: кейтрек прошивки
> отсутствует, слот-офсет +0.5 окт), `reverb_dly_status/STATUS.md` (статус
> реверба/дилея), `tests/test_mnm_flanger.cpp`. **ИСПРАВЛЕН ДЕФЕКТ v2:**
> `phaser/MnmPhaserExactTables.hpp` содержал НУЛИ (LFO фейзера молчал) —
> файл перегенерирован из реальной ROM-таблицы (widthTable утверждённого
> ChorusCore). Если шаг фейзера уже применён — перезапиши только этот файл.
> Шаги v3: 16 (флейнджер), 17 (динамикс), 18 (хорус strict), 19 (bbox питч),
> 20 (ремонт таблицы фейзера). Всё остальное — без изменений (v2).

Дата: 2026-09-25. Поставка для `JUCE/Monomachine-Nova-1.7.11` (обе копии —
`Monomachine_Nova_FX` и `Monomachine_Nova_Synth`; их `Source/` идентичны кроме
`NovaConfig.h`, поэтому ВСЕ правки применяются к обоим проектам одинаково).

Это РАСШИРЕННАЯ версия поставки `mmnova_dspnew_ex_2026-09-25` (дисторшн +
фильтр + энвелоуп, шаги 0–9 ниже без изменений) плюс НОВЫЕ модули:

| # | Папка | Модуль | Точность | Режим |
|---|---|---|---|---|
| 1 | `distortion/` | DIST — драйв-энвелоп + тимбр-индекс | энвелоп/индекс БИТ-В-БИТ (iter.23), по-сэмпловая стадия [РЕКОНСТР.] | DIST → `ex` |
| 2 | `filter/` | FILT — dual-каскад + резонанс-гребёнка | БИТ-В-БИТ (iter.14–16), разбег тапов [РЕКОНСТР.] | FILT → `ex` |
| 3 | `envelope/` | ENV — AMP-стейт-машина ATK/HOLD/DEC/REL/KILL (делей = HOLD) | приращения/фазы БИТ-В-БИТ (iter.13) | AMP MODE → `ex` |
| 4 | `fm/` | FM-машины m8/m9/m10 — законы ручек | законы БИТ-В-БИТ (iter.27–28), топология [РЕКОНСТР.] | SYNT → `ex` |
| 5 | `phaser/` | FX-PHASER m18 — полный порт | **СЛОВО-В-СЛОВО** (38400/38400 + 15360/15360) | PHA → `ex` |
| 6 | `ringmod/` | FX-RINGMOD m17 — прошивочное ядро | законы из листинга + эмулятор | RING → `ex` |
| 7 | `bbox_startpos/` | BBOX m7 — прошивочный закон STRT **+ ПИТЧ-правила (v3)** | законы из листинга P:$1474ED + эмулятор (exp_m7_bbox.py) | SYNT(m7) → `ex` |
| 8 | `chorus_lfo/` | LFO хоруса — ВЕРДИКТ: не сломан | файлы 1.6.0↔1.7.11 бит-в-бит | без изменений |
| 9 | `flanger/` **v3** | FX-FLANGER m19 — прошивочное ядро | законы из листинга + эмулятор, FIR-синтез [РЕКОНСТР.] | FLA → `ex` |
| 10 | `dyn/` **v3** | FX-DYNAMIX m16 (компрессор) — прошивочное ядро | законы из листинга + ROM-таблицы, стейты [РЕКОНСТР.] | DYN → `ex` |
| 11 | `chorus_strict/` **v3** | CHORUS m15 — строгий ROM (A/B к одобренному 1.6.0) | патч 3 задокументированных отклонений | CHORUS → `ex` |
| 12 | `reverb_dly_status/` **v3** | статус REV m13 / DLY m32 | REV — частично, DLY — не начат | — |

**Никакой интеграции «в структуру плагина» в этой папке нет** — только новые
DSP-файлы и эта инструкция. Другой агент применяет шаги ПО ОЧЕРЕДИ (каждый
шаг самодостаточен: скопировать файлы → построчные правки). Каждый модуль
добавляется ОТДЕЛЬНЫМ режимом `ex` в существующие списки DSP MODE — ничего
существующего не удаляется и не перезаписывается, старый звук остаётся
на прежних режимах (`mnm`/`old`), A/B — кнопкой в плагине.

Порядок применения (очередь для агента):
```
ШАГ 1  — скопировать все папки в Source/dsp/mnm_new/ (см. ниже — расширен)
ШАГ 2  — регистрация режима "ex" в DspModes.hpp (dist/filt)
ШАГ 3  — DIST ex (NovaDSP.h)
ШАГ 4  — FILT ex (NovaDSP.h)
ШАГ 5  — ENV ex (редактор огибающей)
ШАГ 6  — проверка и A/B
ШАГ 7  — чего НЕ трогать
ШАГ 8  — честные границы
ШАГ 9  — состав поставки v1
ШАГ 10 — FM-машины ex (DspModes SYNT + NovaDSP.h)        ← НОВОЕ
ШАГ 11 — FX-PHASER ex (новая секция PHA)                 ← НОВОЕ
ШАГ 12 — FX-RINGMOD ex (новая секция RING)               ← НОВОЕ
ШАГ 13 — BBOX STRT ex (m7)                               ← НОВОЕ
ШАГ 14 — LFO хоруса: вердикт, код не меняется            ← НОВОЕ
ШАГ 15 — тесты (все зелёные) + состав поставки v2        ← НОВОЕ
```

Секции с деталями каждого модуля: `distortion/` `filter/` `envelope/`
(см. ШАГИ 3–5), `fm/NOTES.md`, `phaser/NOTES.md`, `ringmod/NOTES.md`,
`bbox_startpos/PATCH_bbox_startpos.md`, `chorus_lfo/LFO_VERDICT.md`.

---

(v1 поставка — шаги 0–9 — без изменений, ниже; новые шаги 10–15 — в конце файла)

## 0. Суть

Три новых звуковых ядра, портированных с уровня инструкций DSP56300 и
исполненных на бит-точном эмуляторе (пак `decompiled data/09_not_sorted/
missing_data/filter_phaser_pack`):

| Папка | Файл | Что внутри | Точность |
|---|---|---|---|
| `distortion/` | `MnmDistExact.hpp` (+Tables) | драйв-энвелоп DIST P:$04FF–$0536 + тимбр-индекс P:$0537–$0556 + сатурация слова DSP | энвелоп/индекс БИТ-В-БИТ (iter.23); по-сэмпловая стадия [РЕКОНСТР.] — тело машинного обработчика $145C48 не декодировано |
| `filter/` | `MnmFilterExact.hpp` (+Tables) | кольцо коэффициентов P:$0537–$05A1 + каскад func_000340 + резонанс-гребёнка func_000397 с таблицами $1448C6/$144946/$144BC9 | каскад/гребёнка/коэффициенты БИТ-В-БИТ (iter.14–16); промежуток разбега тапов [РЕКОНСТР.] |
| `envelope/` | `MnmEnvExact.hpp` (+Tables) | стейт-машина AMP-энвелопа P:$088E–$08D7 (ATK/HOLD/DEC/REL/KILL, signed level, wrap −1.0, де-зиппер 3/32, внутрикадровый рамп) | приращения/фазы БИТ-В-БИТ (iter.13); закон HOLD подогнан к измеренной карте кадров (в порте пака был сдвиг ×128) |

Режим называется **`ex`** (exact). Он добавляется ЧЕТВЁРТЫМ пунктом к спискам
режимов — ничего существующего не удаляется и не меняется. Переключение —
через уже существующую кнопку **DSP MODE** (разделы DIST/FILT) и кнопку
**MODE** на AMP-панели (огибающая): это и есть A/B-сравнение, которое нужно.

Тесты: автономный прогон против векторов эмулятора — **19/19 ok**
(`атака DIST=64 == 0x4E05·n`, `0.5 → −1.0 → 0`, `тимбр-индекс −431`,
`ATK=64 → 419 кадров`, `HOLD=64@120 → ~114 кадров`, устойчивость фильтра
на HPQ=LPQ=127 и свипе BASE словами и т.д.).

## 1. ШАГ 1 — скопировать файлы (без правок сборки)

```text
DSP_New/distortion/  ->  Source/dsp/mnm_new/distortion/   (2 файла)
DSP_New/filter/      ->  Source/dsp/mnm_new/filter/       (2 файла)
DSP_New/envelope/    ->  Source/dsp/mnm_new/envelope/     (2 файла)
```

**v2 — полный список копирования (все 8 модулей, одной командой на папку):**

```text
DSP_New/distortion/    ->  Source/dsp/mnm_new/distortion/    (2 файла)
DSP_New/filter/        ->  Source/dsp/mnm_new/filter/        (2 файла)
DSP_New/envelope/      ->  Source/dsp/mnm_new/envelope/      (2 файла)
DSP_New/fm/            ->  Source/dsp/mnm_new/fm/            (MnmFmExact.hpp, mnm_fm_exact.hpp, ratio_table.txt, NOTES.md)
DSP_New/phaser/        ->  Source/dsp/mnm_new/phaser/        (mnm_phaser.h, MnmPhaserAdapter.hpp, MnmPhaserExactTables.hpp, NOTES.md)
DSP_New/ringmod/       ->  Source/dsp/mnm_new/ringmod/       (MnmRing.hpp, NOTES.md)
DSP_New/bbox_startpos/ ->  правки в существующие файлы       (см. ШАГ 13; папка не копируется)
DSP_New/chorus_lfo/    ->  правок НЕТ                        (см. ШАГ 14; только вердикт)
DSP_New/tests/         ->  НЕ копировать в Source            (автономные проверки вне сборки)
```

Все файлы header-only, без JUCE-зависимостей (только `<array>/<cstdint>/
<cmath>/<algorithm>`). В `CMakeLists.txt` и `*.jucer` добавлять НИЧЕГО НЕ
НАДО: заголовки не компилируются отдельно, они включаются в `NovaDSP.h`
(шаг 3). Включение — по пути от `Source/`:

```cpp
#include "dsp/mnm_new/filter/MnmFilterExact.hpp"
#include "dsp/mnm_new/distortion/MnmDistExact.hpp"
#include "dsp/mnm_new/envelope/MnmEnvExact.hpp"
```

Осторожно: в одном TU можно включать все три — пространства имён таблиц
разведены (`exact_tables::filt / ::env / ::dist`), конфликтов нет.

## 2. ШАГ 2 — зарегистрировать режим "ex" в `Source/models/DspModes.hpp`

**2.1.** Строка ~80, глобальный список:

```cpp
// БЫЛО:
inline const char* dspModeChoices() { return "mnm|old|fma"; }
// СТАЛО:
inline const char* dspModeChoices() { return "mnm|old|fma|ex"; }
```

**2.2.** Строки ~83–84:

```cpp
// БЫЛО:
inline constexpr int dspModeCount = 3;
// СТАЛО:
inline constexpr int dspModeCount = 4;
inline constexpr int dspModeEx = 3; // 1.7.11: точные ядра iter.13–23 (dist/filt/env)
```

**2.3.** Строка ~87, версия схемы состояния (по практике репозитория):

```cpp
// БЫЛО:
inline constexpr int kDspModeSchemaVersion = 13; // ...
// СТАЛО:
inline constexpr int kDspModeSchemaVersion = 14; // 1.7.11: +режим "ex" (точные dist/filt/env)
```

**2.4.** `dspModeName()` (строки ~89–96), добавить case:

```cpp
        case 3: return "ex";
```

**2.5.** `dspSectionModeChoices()` (строки ~127–138), два раздела:

```cpp
// БЫЛО:
        case DspFilter:  return "mnm|old";   // filter: REAL firmware dump, reserve = previous approximation
        case DspDist:    return "mnm|old";   // dist: firmware ALU saturation, reserve = previous bipolarDist
// СТАЛО:
        case DspFilter:  return "mnm|old|ex";   // ex = dual-каскад+гребёнка бит-в-бит (iter.14-16)
        case DspDist:    return "mnm|old|ex";   // ex = драйв-энвелоп DIST бит-в-бит (iter.21-23)
```

**2.6.** (опционально, для тултипов) `dspModeProvenanceForMode()` — добавить
ветку `if (mode == dspModeEx) { ... }`:

```cpp
    if (mode == dspModeEx) {
        switch (section) {
            case DspFilter:  return "FILT ex: bit-exact dual cascade func_000340 + tap-comb resonator func_000397, firmware tables $143F95/$144446/$1448C6/$144946/$144BC9; NO envelope (proven iter.15)";
            case DspDist:    return "DIST ex: bit-exact drive envelope P:$04FF-$0536 (attack = tblA[DIST]) + timbre index P:$0537-$0556; per-sample handler body [RECONSTRUCTION]";
            default:         return "ex: exact firmware core";
        }
    }
```

Проверка шага: `python3 verify_dsp_mode_patch.py Source` должен остаться PASS
(скрипт проверяет допустимость режимов по спискам разделов — `ex` теперь
легален для DspFilter/DspDist).

## 3. ШАГ 3 — DIST: ветка "ex" в `Source/NovaDSP.h`

**3.1.** Рядом со строкой 12 (включения):

```cpp
#include "dsp/mnm_new/distortion/MnmDistExact.hpp"
```

**3.2.** В классе `TrackChain`, поля (строка ~504–506):

```cpp
    // БЫЛО:
    monomachine::mnm::FilterCore mnmFilter;
    monomachine::mnm::DelayCore mnmDelay;
    // СТАЛО (добавить третьей строкой):
    monomachine::mnm::FilterCore mnmFilter;
    monomachine::mnm::DelayCore mnmDelay;
    monomachine::mnm::DistExactCore distEx;   // 1.7.11 ex: драйв-энвелоп DIST (iter.23)
```

**3.3.** В `TrackChain::trigger()` (строки ~410–413) — ретриг драйва по ноте
(в прошивке это y:(page+$20)==1, сброс уровня/фазы/счётчика P:$0500–$0505):

```cpp
    void trigger(){
        filter.triggerEnvelope();
        mnmFilter.trigger();
        distEx.retrig();   // 1.7.11 ex: перезапуск драйв-энвелопа DIST по ноте
    }
```

**3.4.** В `TrackChain::process(...)`, блок «DIST first» (строки ~427–439).
Ветка добавляется ПЕРВОЙ, существующие не трогаются:

```cpp
        // DIST first
        for(int i=0;i<n;++i){
            if(distMode==monomachine::dspModeEx){
                // 1.7.11 ex: DIST прошивки = ЭНВЕЛОП ДРАЙВА, а не статический клип.
                // Ручка DIST = ВРЕМЯ атаки драйва (tblA[$141800][DIST], бит-в-бит
                // iter.23): DIST=0 -> 0.5/кадр (топ за 2 кадра), DIST=64 -> топ за
                // ~420 кадров (152 мс), DIST=127 -> ~30 с. После топа — 24-битный
                // wrap в -1.0 и мгновенный спад (tblE[VOL²>>16], кламп 0). Слышимый
                // характер: «набегающий» овердрайв на атаке ноты.
                distEx.setKnob(params[12]);      // AMP-страница, CC 60 (слот y:(P+$0C))
                distEx.setVol(127.0f);           // VOL²-индекс DEC (при VOL<=127 всегда 0)
                distEx.processStereo(l[i],r[i]); // энвелоп ОДИН на трек, счётчик по парам
            }
            else if(distMode==monomachine::dspModeMnm&&softFmDist){ // 1.6.14: fma -- мягкая сатурация
                ... // существующий код без изменений
```

**ЧТО ИСПРАВЛЯЕТСЯ против текущего `mnm`:** текущий `Saturator::process` —
статический клип `amount=(param-64)/64; drive=1+amount*15` (MnmKernel.hpp
строки ~154–163). В прошивке ручка DIST вообще НЕ «количество драйва», а
индекс таблицы атаки драйв-энвелопа; сам драйв перезапускается нотой и
схлопывается в стационаре (замер iter.23: wrap → −1.0 → DEC → 0 за один
шаг, DEC-индекс = VOL²>>16 = 0 при любом VOL≤127).

## 4. ШАГ 4 — FILT: ветка "ex" в `Source/NovaDSP.h`

**4.1.** Включение (рядом с шагом 3.1):

```cpp
#include "dsp/mnm_new/filter/MnmFilterExact.hpp"
```

**4.2.** Поле (см. 3.2, рядом):

```cpp
    monomachine::mnm::FilterExactCore filtEx; // 1.7.11 ex: dual-каскад+гребёнка (iter.14-16)
```

**4.3.** В `TrackChain::prepare()` (строки ~371–380) добавить:

```cpp
        filtEx.reset();
        filtEx.setSampleRate(sr);
```

**4.4.** В `TrackChain::set(...)` (строки ~418–424) — ручки фильтра:

```cpp
        // БЫЛО (строка ~420):
        filter.setParameters(p[16],p[17],p[18],p[19],p[20],p[21],p[22]-64,p[23]-64);
        // СТАЛО (добавить ПОСЛЕ существующей строки):
        filtEx.setParameters(p[16],p[17],p[18],p[19]); // ex: ATK/DEC/BOFS/WOFS игнорируются
```

**4.5.** В `TrackChain::process(...)`, блок «FILT third» (строки ~449–467),
добавить ветку ПОСЛЕ существующей `if(filterMode==...mnm){...}`:

```cpp
        else if(filterMode==monomachine::dspModeEx){
            // 1.7.11 ex: бит-точный двойной фильтр прошивки.
            // ВАЖНО (итерация 15, доказано бит-точной эмуляцией
            // 08_env_verdict/): у фильтра прошивки НЕТ энвелопа и BOFS/WOFS —
            // P+$14..$17 страницы голоса = delay-модуляция. Ручки ATK/DEC/
            // BOFS/WOFS страницы FILT в режиме ex НЕ ДЕЙСТВУЮТ (как в железе);
            // движение среза делает мод-матрица плагина (setParameters можно
            // звать каждый сэмпл — сглаживание слова идёт по кадрам, как у CPU
            // оригинала). Резонанс = гребёнка 8 дробных тапов с таблицами
            // демпфирования/фидбека прошивки, НЕ TPT-SVF.
            filtEx.setParameters(params[16],params[17],params[18],params[19]);
            for(int i=0;i<n;++i){
                l[i]=filtEx.process(0,l[i]);   // латентность 1 кадр (16 сэмплов,
                r[i]=filtEx.process(1,r[i]);   // родная блочная сетка прошивки)
            }
        }
        else { // old legacy filter
            ... // существующий код без изменений
```

**ЧТО ИСПРАВЛЯЕТСЯ против текущего `mnm` (RealFilterCore):**
1. Резонанс был аппроксимацией TPT-SVF с выдуманным законом `Q=0.5..16`
   (шапка MnmRealFilter.hpp честно это помечает: «Резонанс HPQ/LPQ...
   не восстановлен»). Теперь резонансные таблицы прошивки извлечены и
   вставлены: `damp = tblHPQ[HPQ]` ($1448C6), `fb = tblHPQ2[HPQ]` ($144946),
   `fbLP = tblLPQ[LPQ]` ($144BC9) — характер резонанса родной.
2. Срез шёл через 258-словную таблицу P:$144AC7; в прошивке эта таблица
   обслуживает скорости рамп delay-модуляции (iter.15), а фильтр считает
   коэффициенты из div1/div2/width1/width2/coeff2 ($143F95/$144446/$1444C6/
   $144546/$141CA7) с точным DIV-путём eps — теперь так и есть.
3. Убрана выдуманная env-модуляция среза от ATK/DEC/BOFS/WOFS (её нет в
   прошивке — бит-точное доказательство iter.15).
4. DC-блокер 8 Гц и мягкий ограничитель выхода из RealFilterCore НЕ
   переносятся: в прошивке их нет, стабильность обеспечивает сам каскад
   (wrap A1 — родное поведение; тест F2: HPQ=LPQ=127 на шуме — устойчив).

## 5. ШАГ 5 — ENV: режим "ex" в редакторе огибающей

Огибающая не в DSP-меню (1.6.19: выбор живёт на AMP-панели, `amp_mode`).
Сейчас там `old | mnm | vital`. Добавляем `ex`.

**5.1.** `Source/NovaDSP.h`, включение:

```cpp
#include "dsp/mnm_new/envelope/MnmEnvExact.hpp"
```

**5.2.** Структура `nova::AmpEnvelope` (строки ~92–136):

Поле (рядом с `kernelEnv`, строка ~96):

```cpp
    monomachine::mnm::EnvExactCore exactEnv{};   // 1.7.11 ex: бит-точная стейт-машина P:$088E-$08D7
    float hostTempo = 120.0f;                    // 1.7.11 ex: темп хоста для закона HOLD
```

`configure()` (строка 99 — точная цитата):

```cpp
    // БЫЛО:
    void configure(int algorithm,const std::array<float,3>& shape){mode=std::clamp(algorithm,0,3); // 1.7.9: 3 = VITAL (кнопка MODE энвелоупа: old|mnm|vital)
    // СТАЛО:
    void configure(int algorithm,const std::array<float,3>& shape){mode=std::clamp(algorithm,0,4); // 1.7.11: 4 = ex (бит-точный AMP-энвелоп прошивки, iter.13)
```

`reset()` (строка ~101): добавить `exactEnv.reset();`

`on()` (строка ~102): добавить первой строкой `if(mode==4){exactEnv.noteOn();return;}`

`off()` (строка ~103): добавить `if(mode==4){exactEnv.noteOff();return;}`

`tick()` (строка ~104): добавить ветку ПЕРВОЙ:

```cpp
    float tick() {
        if(mode==4){
            // 1.7.11 ex: бит-точный AMP-энвелоп (iter.13). Фазы прошивки:
            // ATK/HOLD/DEC/REL/KILL; signed level с 24-битным wrap атаки в
            // -1.0 (родное поведение); HOLD = «делей» с темп-законом
            // (t0*TEMPO*1.8909 кадров); KILL = tblB[1] = 0.904/кадр (-60 дБ
            // за ~16 мс); ретриг из DEC/REL продолжает атаку с текущего
            // уровня (дип-к-тишине). Де-зиппер 3/32 + внутрикадровый рамп —
            // как в гейн-пути P:$08DF-$08F9. VOL^2/панорама остаются в
            // своих стадиях плагина (как и для остальных режимов).
            exactEnv.setParameters(p[0],p[1],p[2],p[3],hostTempo);
            return exactEnv.process();
        }
        if(mode==kKernelAlgorithm){
            ... // существующий код без изменений
```

**5.3.** `Source/PluginProcessor.cpp`:

Строка ~89 (выбор режима из параметра):

```cpp
    // БЫЛО:
    {const int am=juce::jlimit(0,2,juce::roundToInt(ampRaw[0]->load()));envelope.configure(am==1?nova::AmpEnvelope::kKernelAlgorithm:(am==2?3:0),{ampRaw[1]->load(),ampRaw[2]->load(),ampRaw[3]->load()});} // 1.7.9: MODE = old|mnm|vital
    // СТАЛО:
    {const int am=juce::jlimit(0,3,juce::roundToInt(ampRaw[0]->load()));envelope.configure(am==1?nova::AmpEnvelope::kKernelAlgorithm:(am==2?3:(am==3?4:0)),{ampRaw[1]->load(),ampRaw[2]->load(),ampRaw[3]->load()});} // 1.7.11: MODE = old|mnm|vital|ex
```

Строка ~105 (после чтения `bpm=global[Bpm];...`) добавить:

```cpp
    envelope.hostTempo=(float)bpm; // 1.7.11 ex: закон HOLD огибающей темпозависим (как в прошивке)
```

(если `envelope` недоступен в этой области — темп можно передавать в
`configure`/`tick` любым существующим каналом; суть: bpm должен доходить до
`exactEnv`.)

**5.4.** `Source/PluginEditor.cpp`, строка ~2700 (комбо MODE AMP-панели):

```cpp
    // БЫЛО:
    mode.addItem("old",1);mode.addItem("mnm",2);mode.addItem("vital",3);
    // СТАЛО:
    mode.addItem("old",1);mode.addItem("mnm",2);mode.addItem("vital",3);mode.addItem("ex",4);
```

и строка ~2701 (тултип) дополнить: `; ex (1.7.11) = bit-exact firmware
envelope (phases ATK/HOLD/DEC/REL/KILL, signed level wrap, tempo-scaled HOLD,
retrigger dip)`.

**ЧТО ИСПРАВЛЯЕТСЯ против текущего `mnm` (mnm::AmpEnvelope в MnmKernel.hpp
строки ~117–155):** текущий kernel — упрощённый AHDSR: атака `+=` до 1,
затухание `*=` к нулевому полу, HOLD замораживает пик по закону хоста
`norm(p[1])*2 сек`. В прошивке иначе: (1) HOLD = «делей» ДО затухания с
темпозависимым порогом (HOLD=64@120BPM → 41 мс, а не 2 с); (2) уровень —
ЗНАКОВЫЙ, атака wrap'ится через −1.0 (родное поведение, де-зиппер глушит
щелчок); (3) есть фаза KILL (0.904/кадр) по триггеру 3; (4) DEC=127 —
вечная заморозка; (5) ретриг не сбрасывает уровень (дип). Все законы — из
бит-точной эмуляции iter.13; таблицы приращений/множителей — дословные
($141800/$141880).

## 6. ШАГ 6 — проверка и A/B

1. `python3 verify_dsp_mode_patch.py Source` → PASS (оба проекта).
2. Синтакс-чек без сборки (практика репозитория):
   `g++ -std=c++17 -fsyntax-only` на `PluginProcessor.cpp` с заголовками JUCE 8.
3. Автономные тесты ядер (уже прогнаны при выпуске, 19/19): воспроизводятся
   `g++ -std=c++17 -I Source/dsp/mnm_new test_mnm_new.cpp`.
4. **A/B в плагине**: кнопка **DSP MODE** → раздел DIST («DISTORTION/SRR»)
   или FILT («FILTER») → выбрать `ex`. Огибающая: AMP-панель → MODE → `ex`.
   Режим помнится в состоянии (как mnm/old/fma).
5. **На что слушать**:
   - DIST: атака ноты — драйв «набегает» (длина = ручка DIST), стационар —
     почти чистый; на старом mnm клип статичный и от положения ручки.
   - FILT: резонанс HPQ/LPQ — другой характер (гребёнка тапов, родные
     таблицы демпфирования); ATK/DEC/BOFS/WOFS страницы FILT в ex не
     действуют — это НЕ баг (доказано: у фильтра прошивки их нет).
   - ENV: ретриг во время DEC — дип к тишине и новая атака (старый режим
     ретригует с нуля); KILL глушит за ~16 мс; HOLD короткий и темпозависимый.

## 7. Чего НЕ трогать (жёстко)

- **BBOX-моды импорта семплов** (`monomachine_bbox.hpp`, ручка start pos
  семпла и связанные отличия) — НАМЕРЕННЫЕ отличия от прошивки, оставлены
  по требованию владельца репозитория. Настоящая поставка их не касается.
- Существующие режимы `mnm | old | fma` — не удалять и не менять (резерв
  сравнения). Удаление резервов — отдельное решение владельца.
- Таблицы в `DSP_New/*/Mnm*Tables.hpp` — дословные слова прошивки
  (24-бит BE → float /2^23), генератор сверил 18 якорей и 1329
  кросс-точек float↔слово. Вручную не редактировать.

## 8. Честные границы (что реконструкция, а что доказано)

| Блок | Бит-в-бит (эмулятор) | Реконструкция/соглашение |
|---|---|---|
| DIST | стейт-машина драйв-энвелопа; атака tblA[DIST] {0x10CFEA/0x46A98/0x4E05/0x560/0x65}; wrap −1.0; HOLD 1 кадр → DEC; DEC tblE[VOL²], кламп 0; ретриг; тимбр-индекс (знаковые a1-срезы, −431 @drive=1418083) | по-сэмпловая стадия `sat24(x·(1+drive·15))` — тело машинного обработчика $145C48 (SWAVE) не декодировано; kDriveGain=15 подобран под потолок текущего Saturator'а (громкость A/B сопоставима); связка индекса с таблицей тембра $143546 — машино-специфична |
| FILT | кольцо коэффициентов (DIV-путь eps, 9/9 конфигураций); каскад func_000340 (±2 LSB); гребёнка func_000397; индексация и крайние точки разбега тапов (HPQ=0→1.0; 127→15.8828) | промежуток разбега тапов — линейная калибровка; bypass на дефолтных ручках — СОГЛАШЕНИЕ плагина (unity-переключение), в прошивке фильтр всегда в тракте |
| ENV | приращения атаки tblA; множители tblB (отрицательные); wrap атаки через −1.0 и знак-флип первого шага DEC; KILL→индекс 1 (0.904); DEC=127 заморозка; ретриг-дип; де-зиппер 3/32; внутрикадровый рамп | закон HOLD подогнан к измеренной карте кадров (5 точек эмуляции, ±2 кадра): thr = t0·TEMPO·1.8909; в порте пака был сдвиг нормировки ×128 — здесь исправлено |

Открытое (не мешает режимам, см. DIST_STATUS.md): полный список
«машина → DIST-обработчик» (у ColdFire-хоста, искать по эпилогу `jmp
$000981`), по-сэмпловое тело $145C48, 48-битное деление питча рингмода.

## 9. Состав поставки

```text
JUCE/Monomachine-Nova-1.7.11/DSP_New/
  00_PATCH_NOTES.md                <- этот файл
  distortion/MnmDistExact.hpp      <- драйв-энвелоп + тимбр-индекс + DistExactCore
  distortion/MnmDistExactTables.hpp<- tblA ($141800) + tblE ($141A00)
  filter/MnmFilterExact.hpp        <- кольцо коэффициентов + каскад + гребёнка
  filter/MnmFilterExactTables.hpp  <- div1/div2/width1/2/coeff2 + HPQ/LPQ таблицы
  envelope/MnmEnvExact.hpp         <- стейт-машина P:$088E-$08D7 + EnvExactCore
  envelope/MnmEnvExactTables.hpp   <- tblA ($141800) + tblB ($141880)
```

---

## 10. ШАГ 10 — FM-машины (m8/m9/m10): режим "ex" — точные законы итераций 27–28

Детали законов — `dsp/mnm_new/fm/NOTES.md` и `mnm_fm_exact.hpp` (каждая
константа цитирует адрес прошивки). Суть: текущее mnm-ядро (MnmFm.hpp,
1.6.0-эпоха) считает отношения по 32-значной шкале с линейными уровнями —
прошивка использует 24-значную таблицу P:$141A80, скрытый ×2 в канале
модулятора-1, квадратичные уровни с wrap-через-знак, FB = K·32 единиц и
униполярный TUNE. Это и есть «сломанные FM-машины».

**10.1.** Скопировать (ШАГ 1 расширенный, одной командой на папку):

```text
DSP_New/fm/ -> Source/dsp/mnm_new/fm/   (MnmFmExact.hpp, mnm_fm_exact.hpp, ratio_table.txt, NOTES.md)
```

**10.2.** `Source/NovaDSP.h`, включения (рядом с шагом 3.1):

```cpp
#include "dsp/mnm_new/fm/MnmFmExact.hpp"
```

**10.3.** Класс `MachineEngine`, поле (рядом с `mnmFm`):

```cpp
    monomachine::mnm::FmCoreExact mnmFmEx;   // 1.7.11 ex: точные законы FM (iter.27-28)
```

**10.4.** `MachineEngine::prepare()` (строка ~168, после `mnmFm.reset(sr);`):

```cpp
        mnmFm.reset(sr);
        mnmFmEx.reset(sr);   // 1.7.11 ex
```

**10.5.** `MachineEngine::setModes()` (строка ~177) — ЗАМЕНИТЬ:

```cpp
    // БЫЛО:
    void setModes(int syntModeIn,int distModeIn){if(syntMode!=syntModeIn){syntMode=syntModeIn;mnmFm.reset(sr);}distMode=distModeIn;}
    // СТАЛО:
    void setModes(int syntModeIn,int distModeIn,int phaModeIn=monomachine::dspModeMnm,int ringModeIn=monomachine::dspModeMnm){
        if(syntMode!=syntModeIn){syntMode=syntModeIn;mnmFm.reset(sr);mnmFmEx.reset(sr);} // 1.7.11 ex: +mnmFmEx
        distMode=distModeIn;phaMode=phaModeIn;ringMode=ringModeIn;} // 1.7.11 ex: режимы FX-машин
```

И рядом с полями `syntMode/distMode` (строка ~361) добавить:

```cpp
    int phaMode=monomachine::dspModeMnm,ringMode=monomachine::dspModeMnm; // 1.7.11 ex: FX-PHASER/FX-RINGMOD
```

**10.6.** `MachineEngine::on()` (строка ~202, после `mnmFm.noteOn(...)`):

```cpp
        mnmFm.setPitchMod(0.0f);mnmFm.noteOn(static_cast<float>(midi),1.0f);
        mnmFmEx.setPitchMod(0.0f);mnmFmEx.noteOn(static_cast<float>(midi),1.0f);   // 1.7.11 ex
```

**10.7.** `MachineEngine::setPitch()` (строка ~209, после `mnmFm.setPitchMod(bend);`):

```cpp
        mnmFm.setPitchMod(bend);
        mnmFmEx.setPitchMod(bend);   // 1.7.11 ex
```

**10.8.** `MachineEngine::render()`, ветку FM (строка ~216) — добавить ветку `ex`
ПЕРЕД существующей (существующую не трогать):

```cpp
        if(syntMode==monomachine::dspModeEx&&(id==8||id==9||id==10)){
            // 1.7.11 ex: точные законы ручек OS 1.32B (iter.27-28). Топология
            // движка = восстановленная (как у mnm-ядра), все РУЧЕЧНЫЕ законы
            // бит-в-бит: P:$141A80 (+скрытый x2 мод-1), уровни (K/64)^2 с
            // wrap-знаком, друп, FB=K*32, шейперы G=(K-64)/16, TUNE
            // униполярный +683*K/128, квантизатор фазы mix>>12. m10: слышимый
            // выход = чистая несущая (доказано итерацией 28).
            const auto kind=id==8?monomachine::mnm::FmKind::Stat:(id==9?monomachine::mnm::FmKind::Par:monomachine::mnm::FmKind::Dyn);
            std::array<float,8> values{};for(int i=0;i<8;++i)values[static_cast<size_t>(i)]=p[static_cast<size_t>(i)];
            mnmFmEx.setParameters(kind,values);
            int done=0;while(done<n){const int block=std::min(monomachine::mnm::kBlock,n-done);float blockBuf[monomachine::mnm::kBlock]{};
                mnmFmEx.processBlock(blockBuf,block);
                for(int i=0;i<block;++i){l[static_cast<size_t>(done+i)]+=blockBuf[i];r[static_cast<size_t>(done+i)]+=blockBuf[i];}
                done+=block;}
            return;
        }
```

**10.9.** `Source/models/DspModes.hpp`, `dspSectionModeChoices()` (строка ~127):

```cpp
    // БЫЛО:
    case DspSynt:    return "mnm|old|fma";   // synt: firmware FmCore, reserve = previous Nova FM; fma = 1.6.14 proper DIST for FM
    // СТАЛО:
    case DspSynt:    return "mnm|old|fma|ex";   // 1.7.11 ex: точные законы FM iter.27-28 (m8/m9/m10) + прошивочный STRT bbox m7
```

**A/B:** DSP MODE → SYNT → `ex` (для каждой машины 8/9/10 свой параметр
`mode_synt_m<id>` — помнится per-machine). Слушать: тембр отношений
(родная таблица), пропадание/инверсию тембра на 1ENV/2VOL выше ~90
(wrap-закон), униполярный TUNE (0..+397 центов вверх, не ±).

---

## 11. ШАГ 11 — FX-PHASER (m18): режим "ex" — слово-в-слово порт

Ядро `mnm_phaser.h` верифицировано слово-в-слово на эмуляторе
(38400/38400 grid + 15360/15360 stress слов). Детали — `phaser/NOTES.md`.
Слоты: `CNTR DEP SPD MIX FB WID --- INP`; INP²×4 и MIX — внутри ядра.

**11.1.** Скопировать:

```text
DSP_New/phaser/ -> Source/dsp/mnm_new/phaser/   (mnm_phaser.h, MnmPhaserAdapter.hpp, MnmPhaserExactTables.hpp, NOTES.md)
```

**11.2.** `Source/NovaDSP.h`, включения:

```cpp
#include "dsp/mnm_new/phaser/MnmPhaserAdapter.hpp"
```

**11.3.** Класс `MachineEngine`, поле (рядом с `phaser`, строка ~360):

```cpp
    monomachine::MonomachinePhaser phaser;monomachine::MnmPhaserExact phaserEx; // 1.7.11 ex: слово-в-слово порт m18
```

**11.4.** `MachineEngine::prepare()` и `clear()` — рядом с `phaser.reset(sr);`:

```cpp
        phaserEx.reset();   // 1.7.11 ex
```

**11.5.** `MachineEngine::set()` (строка ~192, после `if(id==18) phaser.setParameters(...)`):

```cpp
        if(id==18) phaser.setParameters(p[0],p[1],p[2],p[3],p[4],p[5]);
        if(id==18) phaserEx.setParameters(p.data());   // 1.7.11 ex: слоты CNTR DEP SPD MIX FB WID --- INP
```

**11.6.** `renderFX()` (строка ~317). Входной гейн — ИЗМЕНИТЬ строку:

```cpp
    // БЫЛО:
    if(id!=15){const float g=p[7]/64.0f;for(int i=0;i<n;++i){l[i]*=g;r[i]*=g;}}
    // СТАЛО (phaser/ring в ex применяют прошивочный INP сами — двойного гейна быть не должно):
    if(id!=15&&!(id==18&&phaMode==monomachine::dspModeEx)&&!(id==17&&ringMode==monomachine::dspModeEx)){const float g=p[7]/64.0f;for(int i=0;i<n;++i){l[i]*=g;r[i]*=g;}} // 1.7.11 ex
```

Ветку диспатча — добавить ПЕРЕД `else if(id==18)phaser.processStereo(...)`:

```cpp
        else if(id==18&&phaMode==monomachine::dspModeEx)phaserEx.processStereo(l,r,static_cast<size_t>(n)); // 1.7.11 ex: слово-в-слово m18
```

**11.7.** `Source/models/DspModes.hpp` — НОВАЯ СЕКЦИЯ (4 правки):

```cpp
// 1) enum DspSection (строка ~33) — добавить ПЕРЕД DspSectionCount:
    DspChorus,
    DspPhaser,   // 1.7.11 ex: FX-PHASER m18 (mnm = generic, ex = слово-в-слово порт)
    DspRing,     // 1.7.11 ex: FX-RINGMOD m17 (mnm = generic, ex = прошивочное ядро)
    DspSectionCount

// 2) dspModeParamId() — добавить case:
        case DspPhaser: return "mode_pha";  // 1.7.11 ex
        case DspRing:   return "mode_ring"; // 1.7.11 ex

// 3) dspSectionLabel() — добавить case:
        case DspPhaser: return "PHA";  // 1.7.11 ex
        case DspRing:   return "RING"; // 1.7.11 ex

// 4) dspSectionModeChoices() — добавить case:
        case DspPhaser: return "mnm|ex";   // 1.7.11 ex: слово-в-слово порт $145045-$14513F
        case DspRing:   return "mnm|ex";   // 1.7.11 ex: прошивочное ядро $14789E-$147B37
```

Параметры `mode_pha`/`mode_ring` создадутся автоматически (создание параметров
идёт циклом по секциям). Схема состояния (ШАГ 2.3, версия 14) покрывает и их:
старые состояния без этих параметров получат дефолт `mnm` (см. миграцию
PluginProcessor.cpp строк ~378/~405 — она уже циклом по секциям).

**11.8.** `Source/PluginProcessor.cpp`, строка ~90 — прокинуть режимы:

```cpp
    // БЫЛО:
    machine.setModes(dspModes[monomachine::DspSynt],dspModes[monomachine::DspDist]);
    // СТАЛО:
    machine.setModes(dspModes[monomachine::DspSynt],dspModes[monomachine::DspDist],dspModes[monomachine::DspPhaser],dspModes[monomachine::DspRing]); // 1.7.11 ex
```

(если такая строка есть и в Synth-процессоре — править там же; в FX-проектах
строка одна, `PluginProcessor.cpp:90`.)

**A/B:** DSP MODE → PHA → `ex`. Слушать: характер модуляции (leapfrog-
квадратура против чистого синуса), 6-секционная лестница с per-sample
коэффициентом, стерео-проходы, закон микса $7FFFFF−MIX.

---

## 12. ШАГ 12 — FX-RINGMOD (m17): режим "ex" — прошивочное ядро

Ядро `MnmRing.hpp` восстановлено из листинга m17 (P:$14789E-$147B37),
проверено в эмуляторе. Детали — `ringmod/NOTES.md`. Ключевое: несущая
кейтрекается от ноты дорожки (частота приходит в регистре A), кламп
16500 Гц, INP = 4·(INP/127)², WAVE/EXT-веса прошивки, MIX сглажен
однополюсником. Слот 2 на железе = `---` (ручка TUNE была изобретением
старого порта) — в режиме `ex` она игнорируется.

**12.1.** Скопировать:

```text
DSP_New/ringmod/ -> Source/dsp/mnm_new/ringmod/   (MnmRing.hpp, NOTES.md)
```

**12.2.** `Source/NovaDSP.h`, включение:

```cpp
#include "dsp/mnm_new/ringmod/MnmRing.hpp"
```

**12.3.** Класс `MachineEngine`, поле (рядом с `ring`, строка ~360):

```cpp
    monomachine::MonomachineRingMod ring;monomachine::mnm::RingCore ringEx; // 1.7.11 ex: прошивочное ядро m17
```

**12.4.** `prepare()`/`clear()` — рядом с `ring.reset(sr);`:

```cpp
        ringEx.reset(sr);   // 1.7.11 ex
```

**12.5.** `MachineEngine::set()` (строка ~195, после `if(id==17) ring.setParameters(...)`):

```cpp
        if(id==17) ring.setParameters(p[0],p[1],p[3],p[2]+pitchMod+(pitch-60));
        if(id==17) ringEx.setParameters(p[0],p[1],p[3],p[7]);   // 1.7.11 ex: WAVE EXT MIX INP (слот 2 = --- на железе)
```

**12.6.** `renderFX()` — ветка диспатча (после шага 11.6 входной гейн уже
корректен), добавить ПЕРЕД `else if(id==17){ring.setParameters(...)...}`:

```cpp
        else if(id==17&&ringMode==monomachine::dspModeEx){ringEx.setCarrierFrequency(hz(pitch+pitchMod));ringEx.processStereo(l,r,l,r,static_cast<size_t>(n));} // 1.7.11 ex: несущая кейтрекается, кламп 16500 Гц
```

**A/B:** DSP MODE → RING → `ex`. Слушать: несущая следует за нотой
(не за ручкой TUNE), квадратурные шины L/R, морф WAVE.

**12.7.** OPTIONAL (правда UI, можно пропустить): `Source/models/machine_definitions.hpp`,
описание машины 17, слот 2:

```cpp
    // БЫЛО:
    {"TUNE", "Ring carrier MIDI pitch", 0, 127, 60},
    // СТАЛО:
    {"---", "Unused on hardware (carrier is keytracked)", 0, 0, 0}, // 1.7.11 ex: дескриптор m17 слот 2 = ---
```

Если правка применена — дефолт слота 2 уходит из UI; параметры пресетов не
ломаются (значение просто игнорируется).

---

## 13. ШАГ 13 — BBOX STRT (m7): прошивочный закон старт-позиции в режиме "ex"

Полный разбор и листинг — `bbox_startpos/PATCH_bbox_startpos.md`. Кратко:
в порте закон выдуманный (куб × шапка 25 мс), в прошивке (config
P:$1474ED-$147510) — **δ = span·STRT/256, линейно, до ~49.6% слота,
окно до фиксированного конца**. Режим `ex` = прошивка; mnm/old остаются
как есть.

**13.1.** `Source/dsp/monomachine_bbox.hpp` — 3 правки (точные цитаты
БЫЛО/СТАЛО — в `bbox_startpos/PATCH_bbox_startpos.md`, шаги 1–3):

```cpp
    void setFirmwareStart(bool on) noexcept { m_firmwareStart = on; } // 1.7.11 ex: прошивочный закон STRT (P:$14750A)
    // ... в private:
    bool m_firmwareStart = false; // 1.7.11 ex
    // startPosition(): добавить первую ветку if(m_firmwareStart) return span*(STRT/256.0);
```

**13.2.** `Source/NovaDSP.h`, `set()` (строка ~189):

```cpp
    // БЫЛО:
    if(id==7){bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);}
    // СТАЛО:
    if(id==7){bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);bbox.setFirmwareStart(syntMode==monomachine::dspModeEx);} // 1.7.11 ex
```

Random-start/random-slot и адаптивный деклик НЕ трогаются (сознательные
моды плагина).

**A/B:** DSP MODE → SYNT → `ex` на машине 7 (`mode_synt_m7`). Слушать на
длинных семплах: ex на STRT=127 играет вторую половину сэмпла; mnm —
максимум ~25 мс смещения.

---

## 14. ШАГ 14 — LFO хоруса: вердикт (код НЕ меняется)

**Вердикт: LFO хоруса НЕ неверный.** Полное доказательство —
`chorus_lfo/LFO_VERDICT.md`. Кратко:

1. Машина 15 (CHORUS) рендерится прошивочным ядром `ChorusCore`
   (P:$147661-$1477DE) через `AudioAdapter` — это тот самый хорус, который
   ты утвердил как «идеальный» в 1.6.0.
2. Файлы `ChorusCore.cpp/.h`, `AudioAdapter.cpp/.h` **бит-в-бит совпадают**
   между 1.6.0 и 1.7.11 (сверено по sha256 в обе копии) — ничего не
   регрессировало.
3. LFO внутри — прошивочный: «текучий интегратор» скорости SPD, квадратура
   sin/cos из $14A000/$14A800, матрица ±0.866, fade-in 128 блоков, тапы ×1/3.
4. Три задокументированных отклонения порта (0.78% dry при MIX=127, внешний
   линейный INP, latency-выровненный dry) уже помечены в коде и неслышимы/
   минимальны; опциональный возврат 0.78% — в `chorus_lfo/LFO_VERDICT.md`.

Действие агента по этому шагу: **НИКАКОГО кода не менять** (правило 1.6.0:
одобренный хорус не трогать). `MonomachineChorus` из
`monomachine_voice_chain.hpp` — отдельный generic-узел SYNTH-цепочки, не
машина 15; его прошивочный статус не заявлялся, A/B «идеального» хоруса
он не касается.

---

## 15. ШАГ 15 — тесты и состав поставки v2

Все тесты прогнаны при выпуске пакета (g++ -std=c++17 -O2):

| Тест | Команда | Результат |
|---|---|---|
| DIST/FILT/ENV ядра | `g++ -std=c++17 -O2 -I DSP_New -o t tests/test_mnm_new.cpp && ./t` | **19 ok, 0 FAIL** |
| Законы FM (iter.27-28) | `g++ -std=c++17 -O2 -I DSP_New/fm -o t tests/test_mnm_fm_exact.cpp && ./t` | **all tests passed** |
| Фейзер (векторы эмулятора) | `g++ -std=c++17 -O2 -I DSP_New/phaser -I DSP_New/tests -o t tests/test_mnm_phaser.cpp && ./t` | **OK=1344 BAD=0** |
| Рингмод (sanity) | `g++ -std=c++17 -O2 -I DSP_New/ringmod -o t tests/test_mnm_ring.cpp && ./t` | **ALL CHECKS PASSED** |

Состав поставки v2:

```text
DSP_New/
  00_PATCH_NOTES.md            — эта инструкция (v2, шаги 0–15)
  distortion/{MnmDistExact.hpp,MnmDistExactTables.hpp}
  filter/{MnmFilterExact.hpp,MnmFilterExactTables.hpp}
  envelope/{MnmEnvExact.hpp,MnmEnvExactTables.hpp}
  fm/{MnmFmExact.hpp,mnm_fm_exact.hpp,ratio_table.txt,NOTES.md}
  phaser/{mnm_phaser.h,MnmPhaserAdapter.hpp,MnmPhaserExactTables.hpp,NOTES.md}
  ringmod/{MnmRing.hpp,NOTES.md}
  bbox_startpos/PATCH_bbox_startpos.md
  chorus_lfo/LFO_VERDICT.md
  tests/{test_mnm_new.cpp,api_check_mnm_new.cpp,test_mnm_fm_exact.cpp,
         test_mnm_phaser.cpp,test_mnm_ring.cpp,test_vectors.h}
```

В `CMakeLists.txt`/`*.jucer` добавлять НИЧЕГО НЕ НАДО — все новые файлы
header-only, включаются из `NovaDSP.h` (шаги 3.1/4.1/5.1/10.2/11.2/12.2).
Единственный .cpp в tests — автономные проверки вне сборки плагина.

После применения всех шагов: `python3 verify_dsp_mode_patch.py Source` →
PASS, синтакс-чек `g++ -fsyntax-only` на PluginProcessor.cpp обоих проектов,
затем A/B по шагам 6/10/11/12/13. Старые состояния никогда не включают `ex`
молча (схема 14, миграция ставит `mnm`).

---

# Шаг 16 (v3): FX-FLANGER m19 — режим `ex`

1. Скопировать `flanger/MnmFlaExact.hpp` + `flanger/MnmFlaExactTables.hpp` в
   `Source/dsp/mnm_new/` ОБОИХ проектов (FX и Synth).
2. `Source/NovaDSP.h` — рядом с другими `#include` mnm-ядер:
   `#include "dsp/mnm_new/MnmFlaExact.hpp"`
3. `Source/NovaDSP.h`, в `set()` рядом с `if(id==19) flanger.setParameters(...)`:

```cpp
    // 1.7.11 ex: прошивочный FX-FLANGER (m19, P:$144E9A). Слоты DEL DEP WID MIX FB SPD --- INP.
    if(id==19 && syntMode==monomachine::dspModeEx){
        const int fp[8]={b(0),b(1),b(5),b(3),b(4),b(2),0,b(7)};
        flaEx.setSampleRate(sr); flaEx.setParameters(fp);}
```

   ВНИМАНИЕ на порядок слотов: у generic-флейнджера порта порядок ручек
   отличается; прошивочные слоты — DEL(p0) DEP(p1) WID(p2) MIX(p3) FB(p4)
   SPD(p5) --- INP(p7) — маппинг b(...) сверь с реальными именами ручек
   FLA-страницы в своей копии (DEL DEP SPD MIX FB WID INP по дескриптору
   страницы; т.е. если ручки идут DEL DEP SPD MIX FB WID INP, то в массив:
   {b(0),b(1),b(5),b(3),b(4),b(2),0,b(6)} для p6=---/INP=b(6) — ПРОВЕРЬ по
   своей странице).
4. В рендер-путь id==19 (там, где сейчас `flanger.process...`) — ветка `ex`:
   скармливать ядру блоки по 16 сэмплов (float L/R), выход — как у m18-ветки
   фейзера (модель: `MnmPhaserAdapter.hpp`).
5. Детали/границы: `flanger/NOTES.md`. Тест: `tests/test_mnm_flanger.cpp`.

# Шаг 17 (v3): FX-DYNAMIX m16 (компрессор) — режим `ex`

1. Скопировать `dyn/MnmDynExact.hpp` + `dyn/MnmDynExactTables.hpp` в
   `Source/dsp/mnm_new/` обоих проектов; `#include "dsp/mnm_new/MnmDynExact.hpp"`.
2. Слоты прошивки: p0=ATK p1=REL p2=THRESH p3=MIX p4=RATIO p5=OUT p6=ENV
   p7=INP. В `set()` id==16:

```cpp
    if(id==16 && syntMode==monomachine::dspModeEx){
        const int dp[8]={b(0),b(1),b(2),b(3),b(4),b(5),b(6),b(7)};
        dynEx.setSampleRate(sr); dynEx.setParameters(dp);}
```

3. Рендер-ветка `ex` аналогично шагу 16 (16-сэмпловые кадры).
4. Детали: `dyn/NOTES.md`.

# Шаг 18 (v3): CHORUS m15 — строгий ROM `ex`

Пошагово в `chorus_strict/PATCH_chorus_strict.md` (3 правки: ChorusCore.h,
ChorusCore.cpp:345, NovaDSP.h:191). Вердикт по LFO — `chorus_lfo/LFO_VERDICT.md`
(не сломан; этот шаг только для A/B строгого ROM).

# Шаг 19 (v3): BBOX m7 — питч-правила `ex`

Пошагово в `bbox_startpos/PATCH_bbox_pitch.md` (7 правок: setFirmwarePitch,
гашение выдуманного кейтрека, слот-офсет +0.5 окт/слот, NovaDSP.h id==7).
Диагноз: PTCH-кривая в порту была ВЕРНАЯ (2^((k-64)/32) — совпала с
прошивкой), неверно было: кейтрек note-60 (в прошивке rate от ноты НЕ
зависит — нота выбирает сэмпл, slot = note/24, слот даёт +6 полутонов).
Доказательства: scripts/exp_m7_bbox.py (эмулятор).

# Шаг 20 (v3): РЕМОНТ таблицы фейзера (если v2 уже применена)

`phaser/MnmPhaserExactTables.hpp` в поставке v2 содержал нули → LFO фейзера
в режиме ex молчал. Файл в v3 перегенерирован из реальной ROM-таблицы
X:$14A000 (источник — widthTable утверждённого ChorusCore, ChorusCore::readX
маппит X:$14A000-$14B000 на неё 1:1). Просто перезапиши файл — имена
kSinTab/kCosTab и namespace не менялись, больше ничего править не надо.

# Статус REV/DLY (v3, без шагов)

`reverb_dly_status/STATUS.md`: FX-REVERB m13 — добыты init-топология и
слот-карта DEC DAMP GATE MIX HP LP --- INP, ядра ещё нет (в порту generic
juce::Reverb); FX-DLY m32 — не начат. Компрессор m16 закрыт шагом 17.

# Шаг 21 (v5): TRACK DELAY + SRR — прошивочные законы `ex`

Папка `trackdelay_srr/` v5. Файлы:
- `MnmTrackDelayExact.hpp` — ядро Track Delay (законы P:$0939-$0A5B)
- `MnmSrrExact.hpp` — EFX-огибающая (P:$04A8-$04F3) + SRR-рампа (P:$0B1E-$0B33) + дециматор
- `MnmTrackDelayTables.hpp` — 6 ROM-таблиц (дамп X-памяти: $141800, $141880, $144AC7, $144B48, $144BC9, $144C49)
- `NOTES.md` — полные доказательства и честные границы
- `tests/test_mnm_trackdelay.cpp` + `tests/tdelay_vectors.json` — 31 проверка (g++ -O2 -std=c++17 -I.. -o t test_mnm_trackdelay.cpp && ./t)

БИТ-ТОЧНО (векторы эмулятора):
1. Время дилея: v=(cell13+$8000)>>16; y0=floor(7938000·TEMPO/2^18);
   target48 = kDtim1[v]·y0·2. Проверено: target(0,120) = 0x0E310000 ТОЧНО.
2. Глайд tape-style: ±4<<24 за блок, коммит по гейту smoothed<<10 ≥ $3FF.
3. Однонолюсник цели 0.1/0.9 ($CCCCD/$733333), запись при current ≥ $3FF0.
4. Коэффициенты петли: BAS → $144AC7[BAS], WID → $144AC7[BAS+WID] (захват).
5. EFX-огибающая: фазы 0/1/2/4/5, атака kEnvAttack[EQF], декей kEnvDecay[EQG],
   hold = SRR²·256, релиз kEnvDecay[DTIM].
6. SRR-рампа Y:$10-$1F: 16 пер-кадровых коэффициентов из env²·DWID²-цепочки.

[RECONSTR.] (документировано в NOTES.md §3-4): челнок staging↔кольцо
(хост-контракт L:$C2/C4/C5 + Y:$FE не воспроизводим в харнессе), send/feedback
гейны (ячейки DSND/DFB P+$1C/$1D НЕ читаются DSP — хост мапит ручки в
P+$13..$17), пер-сэмпловый дециматор SRR.

КАРТА ЯЧЕЕК (главное открытие v5): стадия дилея читает ФИЛЬТРОВУ страницу:
DTIM = P+$13, стерео-обмен = P+$14 (≥0.5), гейн = P+$15, BAS = P+$16,
WID = P+$17, DWID(SRR) = P+$1F. Ячейки DSND/DFB/DBAS (P+$1C/$1D/$1E) в DSP
коде НЕ читаются. FX P+$18..$1B обслуживают EFX-огибающую.

Интеграция (для агента): модуль автономный — скопировать 3 .hpp в
Source/dsp/mnm_new/trackdelay/, рендер-ветку `ex` для пер-трековой стадии
подключить к TrackDelay::process (16-кадровые блоки), ручки плагина:
TIME→cell13=v<<16, BAS→cell16, WID→cell17, SEND/FDB→float 0..1.
