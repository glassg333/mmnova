# HPQ / LPQ / модулятор фильтра — статус

A = подтверждено источником  B = обоснованная гипотеза  C = неизвестно

## РОУТИНГ                                                    [A]
Твой слуховой тест через эмуляцию:
  synth -> dist -> eq -> srr -> filt -> env -> dsnd

Независимое подтверждение, CHANGELOG monomachine-lab 0.1.4:
  "Kept the original machine, distortion/SRR, filter, EQ, AMP
   and delay routines"

Совпадает: dist рано, env после фильтра, delay последний, SRR до фильтра.
Расходится: порядок eq/srr относительно filt. Слуховой тест сильнее.

Снято противоречие с KERNEL_VOICE_CHAIN ("AMP env -> LFO -> SVF"):
там порядок БЛОКОВ КОДА внутри func_0004A8 (сначала считается
значение огибающей), а умножение звука на неё — в конце.

## ИМЕНА ПАРАМЕТРОВ                                           [A]
core/monomachine_engine.h, enum Parameter, значения 0..127:

  AmpAttack AmpHold AmpDecay AmpRelease
  AmpDistortion AmpVolume AmpPan AmpPortamento

  FilterBase FilterWidth FilterHighPassQ FilterLowPassQ
  FilterAttack FilterDecay FilterBaseOffset FilterWidthOffset

  EffectEqFrequency EffectEqGain EffectSampleRateReduction
  EffectDelayTime EffectDelaySend EffectDelayFeedback
  EffectDelayBase EffectDelayWidth

  SynthA..SynthH  TrackLevel

Два резонанса = FilterHighPassQ + FilterLowPassQ.
Модулятор = FilterAttack/Decay + FilterBaseOffset/WidthOffset.

## БАЗА СТРАНИЦЫ ПАРАМЕТРОВ                                   [A]
  static constexpr uint32_t kVoicePageYAddress = 0x000500;

Сходится с ядром: V = Y:$528, страничные параметры в V-$28..V-$01.
  $528 - $28 = $500

=> весь блок ручек лежит в Y:$500..Y:$527 (40 слов).
ColdFire пишет туда уже пересчитанные fixed-point значения.

## ГИПОТЕЗА: "LFO INTL/DPTH" в доке = модулятор фильтра       [B]
Док сам помечает это открытым вопросом (§9 п.2).

Доводы:
 1. Раскладка FILT-страницы подряд от $508 даёт ровно
    BASE WDTH HPQ LPQ ATK DEC BOFS WOFS -> $508..$50F,
    и $508 уже подтверждён как база среза (P:$537).
 2. Формула |Y:(V-$1A) + $C00000| * $700.
    В 24 битах $C00000 = -$400000, то есть |x - 0.5| — взятие
    модуля от БИПОЛЯРНОГО параметра с центром $400000.
    BOFS/WOFS биполярные (-64..+63). У однополярного SPD
    такой центровки быть не должно.
 3. LFO1/2/3 — отдельные страницы, в enum эмулятора их нет
    среди 33 автоматизируемых. Состояние LFO док кладёт в
    отдельное окно r7 = V+$B4, не в страничный блок.
 4. Y:$141800 док сам называет общей таблицей для "LFO SPD и
    AMP env rates" — огибающая фильтра берёт скорость оттуда же.

Если верно:
  Y:$508 BASE   Y:$50C FILT ATK  -> Y:$141800
  Y:$509 WDTH   Y:$50D FILT DEC  -> Y:$141A00
  Y:$50A HPQ    Y:$50E BOFS      -> |x-$400000|*$700
  Y:$50B LPQ    Y:$50F WOFS

  BASE' = BASE + env*BOFS ; WDTH' = WDTH + env*WOFS
  -> DIV P:$586 -> Y:$91+ (f и q) -> HP SVF $05D3 / LP SVF $05EB

## ЧТО ОСТАЁТСЯ НЕИЗВЕСТНЫМ                                   [C]
 - точный адрес каждой ручки внутри $500..$527
 - кривая 0..127 -> q для HPQ и LPQ
 - кривая 0..127 -> множитель для DIST
 - форма огибающей фильтра (AD? AHD?)

ВСЁ ЭТО СНИМАЕТСЯ ИЗМЕРЕНИЕМ, БЕЗ ДИЗАССЕМБЛЕРА — см. PLAN_MEASURE.md

## ПОБОЧНОЕ
 - домен питча: 0x800 на октаву, преднастройка 0x5800,
   GND-SIN = 261.6 Гц на MIDI 60                              [A]
 - баг extract_tables.py подтверждён в оригинале:
   if base_addr >= 0x100000: off = base_addr - 0x100000       [A]
 - новые адреса: FX-DYN RMS P:$144746 (~64),
   reverb delay buffer X:$120000 (~8192),
   DPRO wavetable X:$11B451                                   [A]
 - "EQ band table X:$140000" в том скрипте — ошибочное имя.
   Это таблица питча wt[i]=0.5*2^(i/2048), проверено 1e-9.
   $1407FF = её конец, читается с отрицательным индексом.
