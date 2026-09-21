# Сверка с мануалами и интернетом → подтверждение в коде (итерация 17)

Дата: 2026-09-21. Задача: сравнить данные майнинга с мануалами Elektron Monomachine
и информацией в интернете, найти наводки «куда копать» и подтвердить/опровергнуть
каждую в коде (бит-точный эмулятор + дизасм).

## 1. Главный результат: гипотеза «фильтр берёт основную энвелоуп» — ПОДТВЕРЖДЕНА в уточнённой форме

Пользователь предположил: «энвелоуп не живёт внутри фильтра, возможно он берёт
основную энвелоуп от ампа и использует её каким-то образом». Интернет это
практически дословно подтверждает, а код — доказывает бит-точно.

### Что говорят мануал/сообщество (источники в §5)

1. Официальная MIDI CC-карта Monomachine (pencilresearch/midi, midi.guide):
   страница FLT содержит ВОСЕМЬ параметров:
   `FLT Base(72) Width(73) HP Q(74) LP Q(75) Attack(76) Decay(77)
    Base Offset(78) Width Offset(79)`.
2. Tarekith "Monomachine Tips and Tricks" (tarekith.com/assets/monomachine_tipsandtricks.htm):
   - «There is one AD-type filter envelope, ATK & DEC control the envelope shape».
   - «BOFS = Filter Base Envelope Offset, meaning how much the envelope modulates
     the cutoff base (BASE); WOFS = Filter Width Envelope Offset».
   - «The BOFS setting affects BOTH filters — high pass and low pass. The WOFS
     setting only controls the low pass filter. So the low pass filter is actually
     a slave filter, always following the "movements" of the [high pass]».
3. Elektronauts (Monomachine Love Thread p.5, тред Filters Quality & resonance):
   «You therefore have two resonance values, LPQ (where the upper filter cutoff is)
   and HPQ (where the lower cutoff is)»; «The filter works like a band pass filter
   with double resonance» — band-pass из HP (нижняя граница) + LP (верхняя граница),
   каждый со своим резонансом.
4. Elektronauts «No ADSR!» + synthvibrations: AMP-энвелопа мономашины = AHDR
   (Attack/Hold/Decay/Release, БЕЗ sustain) — совпадает с нашей картой $00–$03.

### Что доказано в коде (этот пак, бит-точно)

Официальному CC-порядку соответствует блок страницы голоса `$10–$17`:

| Оффсет | CC | Параметр | Доказательство |
|--------|----|----------|----------------|
| $10 | 72 | FLT BASE | чтение P:$056D (трасса абс. адресов: Y:P+$10) |
| $11 | 73 | FLT WDTH | P:$0578 (Y:P+$11) |
| $12 | 74 | FLT HPQ  | P:$08FA (Y:P+$12) |
| $13 | 75 | FLT LPQ  | P:$0985 (Y:P+$13) |
| $14 | 76 | FLT ATK  | P:$0A22, P:$0AB7 (Y:P+$14) — см. ниже |
| $15 | 77 | FLT DEC  | P:$0ABF (Y:P+$15) |
| $16 | 78 | FLT BOFS | P:$0A5D, P:$0A85 (Y:P+$16) |
| $17 | 79 | FLT WOFS | P:$0A84 (Y:P+$17) |

Механика env-блока (секция $0939–$0B49, файл disasm_kernel_filterenv_delay):

- **FLT ATK ($14)**: `(ATK − 0.5)²` (P:$0AB7–$0ABE: `sub #$400000; abs; asl; mpy x0,x0`)
  → генератор коэффициентов рампы вместе с **FLT DEC ($15)** (`mpy y0,x0` при
  y0 = $15), под **KILL-гейтом**: `x:(r7-$84) == 4 (фаза AMP-env KILL) → множитель = 0`
  (P:$0AC0–$0AC4: `cmp #<$4; bne; move #$0,y0`).
- **BOFS ($16)** → индекс (word>>16) в таблицу кривой **$144AC7** → цель рампы
  «офсет базы» (цепь состояний r7−$88, P:$0A5D–$0A7F).
- **WOFS ($17) + BOFS ($16)** → СУММА индексируется во вторую кривую **$144AC7** →
  рампа «офсета ширины» (состояния r7−$87, P:$0A84–$0A98). Это В ТОЧНОСТИ
  фраза сообщества «LP — раб, следующий за HP»: ширина отслеживает BOFS+WOFS,
  база — только BOFS.
- Носители env-модуляции, измеренные бит-точно (exp_f3c_envdump.py):
  - сглаженные стейты офсетов **P+$D4 / P+$D5** (d4: база, d5: ширина);
  - банк коэффициентов фидбек-секции **X:$40–$5E** (31 ячейка):
    - baseline (BOFS=0, WOFS=0): +0.004…+0.116;
    - WOFS=127: **+0.573** (фидбек открыт, энергия ×3.8);
    - BOFS=127: **ровно 0.000** (фидбек схлопнут, энергия ×0.054).

### ИТОГ — уточнённая архитектура dual filter

1. **Стадия 1 (статический SVF-каскад)**: BASE/WDTH/HPQ/LPQ → кольцо коэффициентов
   Y:$04–$07 (запись P:$0599/$059A). **Env-free** — подтверждено ТРИЖДЫ:
   расчёт коэффициентов читает одни и те же значения при любых $14–$17
   (exp_f3_fltenv, дифф чтений $0537–$05A2 пуст), страница никогда не
   самомодифицируется (P+$10..$13 константны, exp_f3c), банки тапов L:$80–$9F
   идентичны во всех конфигах.
2. **Стадия 2 (резонатор/фидбек на машине делэя, $0939–$0B49)**: собственная
   AD-энвелопа (FLT ATK/DEC), запускаемая тем же TRIG, KILL-гейтуемая фазой 4
   AMP-энвелопы, с назначениями:
   - BOFS·env → офсет «базы» фидбек-секции,
   - (WOFS+BOFS)·env → офсет «ширины» (LP-раб следует за HP),
   коэффициенты — через кривые $144AC7 и интегратор X:$40–$5E.

То есть: фильтр НЕ берёт AMP-энвелопу напрямую — у него СВОЯ AD-энвелопа
(FLT ATK/DEC), синхронизированная с AMP-энвелопой общим триггером и KILL-гейтом.
Сообщество слышит это как «filter envelope, whose destinations are base and width».
Требование пользователя «двойной фильтр с энвелоупами, где известен резонанс»
закрыто полностью: двойность = SVF-каскад + резонаторная стадия, энвелоп = AD
FLT-блока, резонанс = таблицы HPQ/LPQ + коэффициенты X:$40–$5E.

## 2. Сверка карты страницы с официальной CC-картой

| CC | Параметр UI | Оффсет страницы | Статус подтверждения в коде |
|----|-------------|-----------------|------------------------------|
| 48–55 | Synth P1–P8 | P+$2C..$33 (машинный r6+$04..$0B) | доказано ранее (фейзер/хорус/SIN) |
| 56 | AMP Attack | P+$00 | доказано (P:$08A0) |
| 57 | AMP Hold | P+$01 | доказано (P:$08B0) |
| 58 | AMP Decay | P+$02 | доказано (P:$08C6) |
| 59 | AMP Release | P+$03 | доказано (P:$08CA) |
| 60 | AMP Dist | P+$04 | доказано; **новое:** читается секцией $0789–$0859 (P:$07A6) |
| 61 | AMP VOL | P+$05 | доказано гейн-путём (итерация 13) |
| 62 | AMP PAN | P+$06 | доказано (P:$08DB–$08EB) |
| 63 | AMP Porta | P+$07 | карта итерации 13 |
| 72 | FLT Base | P+$10 | доказано (P:$056D) |
| 73 | FLT Width | P+$11 | доказано (P:$0578) |
| 74 | FLT HP Q | P+$12 | доказано (P:$08FA) |
| 75 | FLT LP Q | P+$13 | доказано (P:$0985) |
| 76 | FLT Attack | P+$14 | доказано (P:$0A22/$0AB7) |
| 77 | FLT Decay | P+$15 | доказано (P:$0ABF) |
| 78 | FLT Base Offset | P+$16 | доказано (P:$0A5D/$0A85) |
| 79 | FLT Width Offset | P+$17 | доказано (P:$0A84) |
| 80–87 | EFX EQF/EQG/SRR/DTIM/DSND/DFB/DBAS/DWID | P+$18..$1F | частично: P+$1C читает $0284, P+$1E читает $02C8, P+$1F (DWID) читает P:$0B1E; блок $18–$1B аудиально активен (exp_f3d: ×5 громкость) |
| 88–95 / 104–111 / 112–119 | LFO1/2/3 | CPU-сторона | согласуется: LFO на DSP отсутствует (итерация 15) |

## 3. Наводки из интернета → статус в коде

| Наводка (источник) | Статус |
|--------------------|--------|
| «Band pass with double resonance; HPQ — нижняя граница, LPQ — верхняя» (Elektronauts) | ПОДТВЕРЖДЕНА: HPQ→таблицы $1448C6/$144946 (нижний угол), LPQ→$144BC9 (верхний угол), WDTH = разнос углов (width1/width2[WDTH] в кольце Y:$06/$07) |
| «BOFS двигает оба фильтра, WOFS — только LP; LP — раб» (tarekith) | ПОДТВЕРЖДЕНА В КОДЕ: рампа базы ← BOFS ($16), рампа ширины ← WOFS+BOFS ($17+$16) — P:$0A5D–$0A92 |
| «There is one AD-type filter envelope» (tarekith) | ПОДТВЕРЖДЕНА: AD-машина с ATK=$14/DEC=$15, KILL-гейт по фазе 4 AMP-env |
| «WDTH=0 + LPQ=127 = self-osc kick» (tarekith/SID) | согласуется: WDTH=0 → углы совпадают, LPQ=127 → макс. фидбек (таблица шага 1/256) |
| «Cutoff key tracking off в Assign-меню» (Reddit r/Elektron) | НОВАЯ НАВОДКА: кейтрекинг = CPU-сторона (масштабирование BASE словом ноты до записи в страницу). На DSP следов нет (индекс = BASE·1199>>23 без ноты) — искать в ColdFire-части репо |
| «DBAS/DWID — фильтровать частоты делэя» (Elektronauts) | ПОДТВЕРЖДЕНА ЧАСТИЧНО: P+$1F читается delay-секцией (P:$0B1E); у делэя своя секция фильтра с рампами ($144B48) — это «Delay FLT Base/Width» из CC-карты |
| «EQF — частота, EQG — биполярный ±64 гейн» (gearspace) | блок $18–$1B аудиально активен (exp_f3d); точное разделение EQ-таблиц $143546/$1435C6 — Ф3 (см. §4) |
| Monomachine = Nova-движок (Elektronauts; Sound on Sound о Nova: LP/BP/HP 12/18/24 дБ; Крис Хаггетт — дизайнер Nova/Supernova) | контекст подтверждён; наша dual-структура «SVF + резонатор» согласуется с Nova-наследием |

## 4. Методологическая поправка + новые лиды

### Поправка к exp_f3 (важно для будущих прогонов)
Ячейки **Y:$04–$07 дважды используемы**: в кадре это кольцо коэффициентов SVF
(запись P:$0599/$059A, чтение func_000340), а в конце кадра ($0B41–$0B49) тот же
диапазон перезаписывается как **мастер-выход Y:$0000–$001F** (писатели P:$0B45/$0B46).
«Дифф кольца» в exp_f3 был диффом выходных сэмплов. Корректный метод: трассировать
ЗАПИСИ на P:$0599/$059A (как в exp_f2c) или дифф чтений секции коэффициентов.

### Новая машина: ADSR на $04A8–$04F4 (лид №1)
Полноценная ADSR-машина: ATK←P+$18 (таблица $141800), DEC←P+$19 ($141880),
SUS←(P+$1A)², REL←P+$1B ($141880); уровень **Y:$04FF**, стейт **X[P+$00]**,
шлюз `tst y:$124` (только трек 0). Работает в нашем харнесе (чтения $04B4 n=3
каждый кадр). Блок $18–$1B **аудиально активен**: ADSR-набор (20/100/127/100)
против нулей меняет громкость ×5 (exp_f3d).
Две гипотезы (разрешить следующей итерацией):
- **H1**: $18–$1B = скрытая ADSR (драйв/EFX-env), и EFX-блок сдвинут: $1C=EQF,
  $1D=EQG, $1E=SRR, $1F=DTIM, $20=DSND, $21=DFB, $22=DBAS, $23=DWID.
  За: $0284 читает P+$1C, $02C8 читает P+$1E (предобработка EFX),
  P:$0B1E читает P+$1F (= DTIM — логичнее для рампы времени делэя!).
- **H2**: CC-порядок верен ($18=EQF…$1F=DWID), а машина $04A8 = скрытая
  динамическая EFX-энвелопа.
КАК ЗАКРЫТЬ: трасса потребителей Y:$04FF/L:$4FF (X-пары) + свип $1C–$23 на
аудио-эффект + проверка переходов стейта X[P+$00] (1→4→5).

### Ф3 (EQ-таблицы $143546/$1435C6/$143C06) — уточнение
Секция $0789–$0859 читает **DIST (P+$04)** и X:P+$0B — это драйв/EQ-гибрид,
а не чистый EQ. EQF/EQG-блок ($18/$19) аудиально активен, но его читатели в
$06C2–$0859 не найдены литеральным сканом `r6+$18/$19` — искать косвенную
адресацию или потребление через стейты P+$F0..$FE (банк, читаемый $07EC–$093B).

### Кейтрекинг (лид №2)
Reddit: «The filter behaves differently when you turn off filter tracking in the
Assign menu». На DSP следов нет → реализован на CPU (ColdFire-часть репозитория):
BASE_word = f(нота, трекинг). Проверить в 05_descriptors/ColdFire-дизасме.

## 5. Источники (снапшоты в sources/)

- `sources/mn_midi.csv` — официальная MIDI CC-карта MonoMachine
  (github.com/pencilresearch/midi, Elektron/MonoMachine.csv; дублирует midi.guide).
- Tarekith, «Monomachine Tips and Tricks» — http://tarekith.com/assets/monomachine_tipsandtricks.htm
  (цитаты: «one AD-type filter envelope…», «BOFS affects BOTH filters…»,
  «The filter works like a band pass filter with double resonance»).
- Elektronauts: тред 47957 «Elektron Monomachine Filters Quality & resonance»
  (цитата мануала про LPQ), тред 5322 «Monomachine Love Thread» p.5
  («two resonance values, LPQ… upper… HPQ… lower… Key tracking»).
- Gearspace: «EQF - frequency. EQG - gain which is bipolar, or +/- 64».
- Reddit r/Elektron «Monomachine tips?» (filter tracking в Assign-меню).
- Sound on Sound, обзор Novation Nova (фильтровая секция Nova: LP/BP/HP,
  12/18/24 дБ) — контекст движка; Novation «Chris Huggett tributes»
  (Supernova/Nova — дизайн Хаггетта).
- audiofanzine «Tips for Using the Monomachine» («There is infact only one filter
  envelope. BOFS, WOFS are the filter envelope destinations»; AMP env = A/H/D/R).

## 6. Эксперименты итерации 17 (в этой папке)

| Файл | Что доказывает |
|------|----------------|
| `exp_f3_fltenv.py` | $14–$17 живые: кольцо/выход меняются при живой энвелопе; стерильный кадр — нет (объяснение «негатива» Ф2) |
| `exp_f3b_ringwriters.py` | все писатели Y:$04–$07 за кадр; двойное использование Y:$04–$07 (кольцо ↔ мастер-выход) |
| `exp_f3c_envdump.py` | страница не самомодифицируется; тапы статичны; носитель env = X:$40–$5E (WOFS→0.573, BOFS→0.000) + стейты P+$D4/$D5 |
| `exp_f3d_adsr18.py` | блок $18–$1B аудиально активен (×5); ADSR-машина $04A8 дышит (чтения $18 каждый кадр) |
| `trace_param_reads.py` | абсолютные адреса чтений: $10–$17/$04/$1F → доказательство карты FLT-блока |
| `disasm_regions.py` | скан чтений страницы в EQ/DIST-регионах |
| `results/*.json` | сырые результаты всех прогонов |

## 7. Что это меняет для JUCE-порта (09_juce_port)

В `mnm_dual_filter.h` добавить стадию 2 (env-управляемый резонатор):
- параметры: `fltAtk, fltDec, bofs, wofs` (0..127);
- AD-энвелопа: атака/спад по кривым P_141800/P_141880 (уже в mnm_tables.h),
  цель базового офсета = curve_144AC7[bofs]·env, ширины = curve_144AC7[bofs+wofs]·env,
  сглаживание — линейный slew (стейты d4/d5), KILL-гейт по фазе 4 amp-env;
- коэффициенты фидбека X:$40–$5E пересчитывать по формулам §1 (порт Q23).
До порта стадии 2 фильтр звучит «суше» оригинала — статический SVF-каскад верен.
