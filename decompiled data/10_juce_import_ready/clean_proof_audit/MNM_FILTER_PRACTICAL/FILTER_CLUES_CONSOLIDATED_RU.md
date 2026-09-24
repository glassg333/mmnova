# FILT — все зацепки из README/HTML и что из них реально следует

**Рабочая дата:** 2026-09-18. **Цель:** не объявлять старую реконструкцию
точной, а собрать из всех материалов рабочую схему и довести её до
практического C++-прогона.

## 0. Как читать этот файл

* **A / VERIFIED** — подтверждено первичным DSP-листингом, raw P-memory,
  прямым бинарным поиском или фактом из доступного эмулятора.
* **B / INFERENCE** — сходится по нескольким независимым источникам, но не
  является доказательством instruction-by-instruction.
* **C / UNKNOWN** — в материалах нет замыкающего свидетельства.
* **RETRACTED** — заявление из старого HTML/C++ отозвано: оно было оформлено
  как exact, хотя содержало гипотезу или заглушку.

HTML — презентационный артефакт. Его утверждение не становится A только из-за
ярлыка `VERIFIED`, `OFFICIAL` или `100%`. Приоритет: raw listing/ROM → рабочий
эмулятор с debug-памятью → manual/descriptor → README/HTML → старый C++.

Источники, из которых собрана эта выжимка:

* `MNM_BLOCKS_FOR_JUCE_CLEAN/README_PROOF_ONLY_RU.md`;
* `/tmp/mmnova-audit/mnmdsp_reference filter from emulation/README_RU.md`;
* `/tmp/mmnova-audit/mnmdsp_reference filter from emulation/docs/KERNEL_CHAIN_RU.md`;
* `/tmp/mmnova-audit/mnm-filter-q/FILTER_Q_FINDINGS.md`;
* `/tmp/mmnova-audit/mnm-filter-q/VOICE_PAGE_MAP.md`;
* `/tmp/mmnova-audit/mnm-filter-q/PLAN_MEASURE.md`;
* `/tmp/mmnova-audit/mnm-filter-q/index.html` и `index (2).html`;
* `/tmp/mmnova-audit/5 mnm-dist-filter/FILTER_DIST_FACTS.md` и `index.html`;
* `/tmp/mmnova-audit/mmnova-filter-official fav from manual и косвенно все вырисовывает его/index.html`;
* `/tmp/mmnova-audit/2 try filter dist env nice build with preview site есть из чего выбрать тип если не найти ориг/README_1.6.0_INTEGRATION_RU.md` и `index.html`;
* `/tmp/mmnova-audit/mnm-routing-verified/index.html`, `ROUTING_CONFIRMED.md`, `NEXT_3_PATHS.md`;
* `/tmp/mmnova-audit/gpt 6 astra cheker na oriig.html`;
* `/tmp/mmnova-refs/PAGE_TEMPLATES.txt`, `KERNEL_VOICE_CHAIN.md`,
  `dsp1_kernel_P0000-0B4D.txt`, `coldfire_main.bin`;
* исходники рабочего `Jick238/monomachine-lab`, в частности
  `core/monomachine_engine.cpp` и `core/monomachine_engine.h`.

---

## 1. Что именно называется фильтром

### 1.1 Имена ручек — A/B, но адреса runtime нельзя смешивать

Из `PAGE_TEMPLATES.txt`, HTML и enum рабочего эмулятора:

```text
FILT: BASE  WDTH  HPQ  LPQ  ATK  DEC  BOFS  WOFS
```

Смысл:

```text
BASE — нижняя/базовая частота
WDTH — ширина до второй точки среза
HPQ  — resonance HP-секции
LPQ  — resonance LP-секции
ATK/DEC — filter envelope
BOFS/WOFS — глубина этой envelope в BASE/WDTH
```

Сообщённая MIDI-карта трека 1:

```text
BASE=72 ($48), WDTH=73, HPQ=74, LPQ=75,
ATK=76, DEC=77, BOFS=78, WOFS=79
```

`BASE=72` прямо приписан мануалу; подряд 73…79 — вывод из порядка страницы.
Названия и порядок страницы — полезная рабочая схема, но конкретное место
каждого слова в DSP voice block нужно принимать только после проверки
`debugYWord()`/host transfer.

### 1.2 Топология по документации и manual — B, не полная трасса

Мануал, как его цитируют HTML:

* фильтр resonant 24 dB low/high/band-pass;
* HPQ и LPQ — `individual Q-control`;
* у фильтра есть собственная envelope;
* FILTER-trig запускает эту envelope, отдельно от AMP envelope;
* `BASE` и `WDTH`: 8 шагов = 1 октава;
* filter normally tracks note pitch;
* при `BASE=0` HP point находится на две октавы ниже ноты:
  `fHP = noteHz / 4`;
* `LP point = HP point × 2^(WDTH/8)`;
* BOFS двигает BASE, WOFS двигает WDTH.

Это достаточно, чтобы построить полезную UI-модель. Это **не** доказывает,
что native host recurrence и mapping совпадают с ROM.

### 1.3 Две петли — пока не называть их безусловно HP/LP

Старые HTML закрывали вопрос так: две петли = HP и LP, потому что Q два.
Это разумная гипотеза (B), но первичный listing доказывает прежде всего:

* две 8-итерационные filter-shaped области `P:$05D3..$05E8` и
  `P:$05EB..$05FA`;
* литералы `0xF528BD` и `0x4A4DF0` в этих областях;
* состояния/буферы X:$71/X:$73 и рабочие коэффициенты около Y:$91+;
* отдельные вспомогательные циклы `func_00037C` и `func_00038A`, вызываемые
  позже (`P:$0806` и `P:$0879`).

Пока не прослежен полный data-flow, утверждение «первая петля именно HP,
вторая именно LP и они стоят последовательно» остаётся B.

---

## 2. Первичный DSP listing: что реально вычисляется

Ниже не пересказ старого `MnmFilter.h`, а опорные операции из
`dsp1_kernel_P0000-0B4D.txt`.

### 2.1 До filter-shaped loops: P:$04A8..$05D2

**AMP state / общая envelope — A:**

* `P:$04A8` — post-voice entry;
* `P:$04AC` читает/обновляет state;
* `P:$04B4..$04C6` attack через `Y:$141800`;
* `P:$04C7..$04DB` decay/hold через `Y:$141880` и square уровня;
* `P:$04DC..$04F0` hold/release;
* `Y:$04FF` — рассчитанный уровень AMP.

**Trigger/LFO-shaped state — A по опкодам, назначение параметров требует
аккуратности:**

* `P:$04FF..$0505` смотрит `r6+$20` и сбрасывает `r7-$1` при trigger;
* `P:$0509` читает `r6+$0C`, индексирует `Y:$141800` и добавляет в фазу;
* `P:$052B` читает `r6+$0D`, использует `Y:$141A00`;
* `P:$0537` читает `r6+$08`, умножает на `$800`, subtract `$80`, round;
* `P:$053E..$0556` читает `r6+$0E`, применяет `+$C00000`, `abs`, `$700`
  и envelope state;
* `P:$0557..$056C` читает `r6+$09` и `r6+$0F`, строя вторую ветку;
* `P:$056D..$0572` делает exact visible smoothing shape:

```text
b = Y:(r6+$10) - previous
b = round((b << 8))
b = b >> 8
current = previous + b
```

* `P:$0576..$058B` использует ещё `r6+$11`, две таблицы делимого и
  24-шаговое `div`.

Важно: в старых README эти offsets назывались то FILT ATK/DEC/BOFS/WOFS,
то LFO SPD/MULT/INTL/DPTH, то EFFX/HPQ/LPQ. Сам listing доказывает offset и
операцию, но не имя UI-поля без подтверждённого host transfer.

### 2.2 Табличное вычисление коэффициентов — A по коду и raw P-memory

В `P:$0576..$0588`:

```text
x0 = x:(r0 + $143F95)
y1 = x:(r2 + $144446)
b  = x0 + y1
a  = b >> 1
do #$18: div x1,a
```

Таблицы:

```text
X:$143F95 — 107 слов
X:$144446 — 128 слов
```

Сами таблицы раньше ошибочно считались «не выгруженными». В этом work package
они извлечены из `dsp1_pmem.bin` и сохранены как exact raw words:

```text
data/kDivP143F95.bin
 data/kDivP144446.bin
```

Это закрывает наличие данных, но не автоматически закрывает семантику каждого
DSP accumulator или точное отображение 0..127 в `r0/r2/x1/a`.

### 2.3 Q/interpolation path — A по listing, UI-law — C

`P:$0610..$0668` и повторный регион `P:$06D?` читают `r6+$0A`/`r6+$0B`,
строят индексы, используют таблицы:

```text
P:$141A98
P:$142158
P:$142F06
```

В listing присутствует clamp:

```text
0x7FFFA4
```

И есть `mpysu`, `macsu`, `dmac`, `div` — то есть это не доказуемая формула
вида `q = 2 - knob * 1.88`. Формула `2 - 0.084694564 - n*1.88` из старых
HTML/C++ — только usable approximation и здесь так и помечается.

Сохранённые clean-фрагменты таблиц A/B/C имеют по 48 слов и совпадают с
P-memory; в package также лежат полные raw fragments этих диапазонов.

### 2.4 SVF-shaped loops — A по структуре, recurrence — частично B

В `P:$05CF..$05D1`:

```text
x0 = $F528BD  -> signed 24-bit = -710467 = -0.0846945638656616
x1 = $4A4DF0  ->  4869616       = +0.5805034637451172
```

(Предыдущие значения `-0.08475685` и `+0.58044434` были неправильным
округлением и отозваны.)

`P:$05D3..$05E7` и `P:$05EB..$05FB` — два `do #$8`; в каждой области по 8
итераций, а 16 audio words проходят парами через DSP dual-move/MAC pattern.

Но это не даёт права автоматически заменить цикл на любой textbook TPT SVF:
нужно учитывать 56-bit accumulator, `mpy/mac` order, state addresses, rounding
и routing output. В practical implementation ниже используется textbook
fixed-shape model, помеченная APPROX.

### 2.5 Второй coefficient/data path — A, назначение — ещё не полностью B

* `P:$07D2..$07E6` clamp `$6A3`, read `P:$143546`, подготовка 16 values;
* `P:$07E7..$07FB` — 16-итерационный dual-MAC block;
* `P:$0806` вызывает `func_00037C`;
* `P:$0855..$086C` clamp `$63F`, read `P:$1435C6`;
* `P:$0879` вызывает `func_00038A`.

Exact cutoff ranges:

```text
P:$143546: 1700 words, index 0..$6A3
P:$1435C6: 1600 words, index 0..$63F
```

Primary 1700/1700 words совпали с rig `cutoff_table_1700.h` и P-memory.
Сохранённый старый fragment secondary (128 слов) совпал с P-memory; здесь
добавлен весь range до доказанного clamp.

---

## 3. Voice page / ColdFire: где искать недостающую связку

### 3.1 Host-side схема — A для рабочего эмулятора, не обязательно A для ROM ABI

`Jick238/monomachine-lab/core/monomachine_engine.cpp` содержит:

```text
parameterPageOffset(index < 24) -> index
index 24..31 -> 44 + (index-24)
TrackLevel -> 31
all ordinary parameters -> value << 16
kVoicePageYAddress = 0x500
```

Это даёт практический способ подать UI 0..127 в существующий emulation
harness и читать `debugYWord()`. Но внутренний `setParameter()` сам по себе
не доказывает, что старый generated native filter правильно интерпретировал
все эти offsets.

### 3.2 Direct listing offsets — A, имена — конфликтная часть

При `r6 = Y:$528-$28 = Y:$500`:

```text
r6+$08 = Y:$508 = V-$20  (P:$0537)
r6+$09 = Y:$509 = V-$1F  (P:$0557)
r6+$0A = Y:$50A = V-$1E  (P:$0610 / $06E0)
r6+$0B = Y:$50B = V-$1D  (P:$0610)
r6+$0C = Y:$50C = V-$1C  (P:$0509)
r6+$0D = Y:$50D = V-$1B  (P:$052B)
r6+$0E = Y:$50E = V-$1A  (P:$053E)
r6+$0F = Y:$50F = V-$19  (P:$0557)
r6+$10 = Y:$510 = V-$18  (P:$056D)
r6+$11 = Y:$511 = V-$17  (P:$0576)
r6+$18 = Y:$518 = V-$10  (P:$04B4)
r6+$19 = Y:$519 = V-$0F  (P:$04CC)
r6+$1A = Y:$51A = V-$0E  (P:$04C9/$04DE)
r6+$1B = Y:$51B = V-$0D  (P:$04E5)
r6+$20 = Y:$520 = V-$08  (P:$04FF trigger)
```

Это факты об обращениях. Нельзя из них одной строкой получить каноническую
таблицу UI `AMP -> FILT -> EFFX`, поскольку старые docs одновременно
приписывали Y:$50C..$50F фильтру и LFO. Именно здесь нужен runtime trace:
поставить один host parameter в 0/127, сделать `renderBlock`, сравнить
`Y:$500..$527` и отдельно снять `Y:$91+`.

### 3.3 Что уже видно из ColdFire image

`coldfire_main.bin` (425858 bytes) содержит строковые пулы:

```text
HPQ  @ 0x508ba и 0x528bd
LPQ  @ 0x508c0 и 0x528c1
BASE @ 0x508ae, ...
WDTH @ 0x508b4, ...
BOFS @ 0x508d2
WOFS @ 0x508d8
```

Это подтверждает реальные UI labels, но **не** даёт scaling bytes. Структурные
machine descriptors начинаются около `0x57FC5`, stride `0xB0`; простой parser
находит реальные machine pages, но common FILT scaling block в таком формате
не найден. Поэтому строку нельзя выдавать за 0..127-to-Q curve.

---

## 4. Что из старых README/HTML брать в код, а что нет

### Брать прямо в practical layer

1. `Params` с восемью UI ручками 0..127.
2. `BASE=0`, keytracking `noteHz/4`.
3. 8 steps per octave для BASE/WDTH.
4. `effBase = base + env*bofsShift` и `effWidth = width + env*wofsShift`,
   с clamp — manual/community rule.
5. 16-sample processing cadence.
6. exact 24-bit table words and exact structural literals.
7. HPQ/LPQ as separate controls; use independent states.
8. note/FILTER trigger reset of filter envelope.

### Не брать как exact

1. `qFromKnob()` with slope 1.85/1.88.
2. `cutoffCoeff()` через guessed `20*pow(1000,n)` or `tan()`.
3. `env += 1/(1+atk*12)` and `env -= 1/(1+dec*16)`.
4. claim that `MnMFilter24_Official.h` is official/reference.
5. claim that two loops are proven serial HP then LP.
6. `DIST` placement from contaminated `README_architecture_routing.md`.
7. literal GND-SIN 2-pole values `$1E454E/$5B75B8` as the global FILT.
8. old rounded values `-0.08475685`, `+0.58044434`.

### Contradictory routing notes

Sources report:

```text
synth -> dist -> srr -> filt -> eq -> env -> dsnd
synth -> dist -> eq -> srr -> filt -> env -> dsnd
osc -> AMP_env x DIST -> FILT -> EFFX -> LFO
AMP env -> LFO -> SVF -> mixer  (order of code blocks, not necessarily signal)
```

The contaminated architecture README is explicitly rejected. The practical
filter package processes only the filter block and does not claim full voice
routing.

---

## 5. Exact next step for bit-exact work

The old HTML already contained the right experimental plan. It is now made
concrete:

```text
A. Use local full listing:
   show P:$0537..$05D2 and P:$05D3..$05FB.

B. Run the real engine with one changed parameter at a time:
   for v=0..127:
     setParameter(FilterHighPassQ,v)
     renderBlock()
     dump Y:$500..$527 and Y:$91..$AF

C. Repeat for FilterLowPassQ, BASE, WDTH, ATK, DEC, BOFS, WOFS.

D. Use the resulting 128-row tables to replace only the mapping functions.

E. Compare output block and internal Y/X states against native filter.
```

The `debugYWord()` plan is not theoretical: it exists in the external
monomachine-lab Engine API. What is absent in this workspace is the full
emulator dependency/toolchain, so it is not copied here. The practical native
filter below uses the documented/manual layer now, while keeping every
unknown mapping isolated in named functions.

---

## 6. The practical decision made here

This work package does **not** pretend to be exact. It implements a usable
filter with the strongest available clues:

* UI-level 8-parameter API;
* note tracking and 8-step octave law;
* exact raw cutoff table as the quantizer for the computed frequency coefficient;
* exact structural literals retained as constants;
* independent HP/LP states;
* block-16 filter envelope tick;
* all guessed laws isolated under `approximate...()` functions.

Thus it can be put behind JUCE immediately, and later the measured tables can
replace the approximations without changing the host-facing API.

---

## 7. Files made and practical proof

* `include/MnmFilterFromClues.h` — usable, explicitly APPROX native filter;
* `include/MnmFilterEvidenceTables.h` — raw exact tables and coefficient fragments;
* `tools/render_filter_demo.cpp` — standalone renderer, no JUCE dependency;
* `demo/filter_demo.wav` — generated by compiling and running that renderer;
* `tests/selfcheck.py` — verifies raw/header equality and preserved clean fragments.

No compiler, JUCE, emulator or toolchain is included in the archive.
