# Полная карта страницы голоса Y:$500..$533  (OS 1.32B)

Источник: core/monomachine_engine.cpp, функции configureVoicePage() и
parameterPageOffset(). Это рабочий эмулятор на настоящей прошивке.

## ГЛАВНОЕ

1. ColdFire НЕ считает никаких кривых параметров.
   Дословно из кода:
     "ColdFire streams each user-facing 0..127 value as a 16.16 word"
     yMemory[0x500 + pageOffset] = value << 16;
   Вся математика — внутри DSP. Capstone M68K не нужен.

2. parameterPageOffset():
     index  0..23 -> offset  0..23   (AMP 0..7, FILT 8..15, EFFX 16..23)
     index 24..31 -> offset 44..51   (SynthA..H = Y:(R6+$4..$B))
     index 32     -> offset 31       (TrackLevel)

3. В KERNEL_VOICE_CHAIN.md §4.4-4.5 ошибка атрибуции:
   блок P:$506-$56C назван "LFO-фаза / INTL / DPTH".
   По реальным адресам это ЦЕЛИКОМ СТРАНИЦА ФИЛЬТРА:

     Y:$508  V-$20  FilterBase         -> (V-$20)*$800 - $80   @ P:$537
     Y:$509  V-$1F  FilterWidth
     Y:$50A  V-$1E  FilterHighPassQ    -> читается @ P:$537!
     Y:$50B  V-$1D  FilterLowPassQ
     Y:$50C  V-$1C  FilterAttack       -> TBL[Y:$141800 + idx] @ P:$509
     Y:$50D  V-$1B  FilterDecay        -> TBL[Y:$141A00 + idx] @ P:$52B
     Y:$50E  V-$1A  FilterBaseOffset   -> |x + $C00000| * $700 @ P:$53E
     Y:$50F  V-$19  FilterWidthOffset  -> вторая ветка         @ P:$53E

   Док сам ставил это открытым вопросом (§9 п.2) — теперь он закрыт.

## ТОЧНЫЙ АЛГОРИТМ МОДУЛЯТОРА ФИЛЬТРА (P:$506-$56C)

Огибающая фильтра — 3 состояния, фаза в X:(r7-$1), флаг в r7-$2:

  state 0 (ATTACK):
      phase += TBL[ Y:$141800 + (Y:$50C >> 16) ]      ; FilterAttack
      if carry -> state = 1, r7-$2 = 1

  state 1 (переход/колено):
      математика с (V-$05)^2 и $791FD0 (= 0.9462824)

  state 2 (DECAY):
      phase -= TBL[ Y:$141A00 + ((Y:$50D + $7FFF) >> 16) ]  ; FilterDecay
      if phase < 0 -> phase = 0

Глубина модуляции (P:$53E-$56C):
  BOFS в Y:$50E, WOFS в Y:$50F. Оба хранятся как 0..127 << 16,
  нейтраль = 64 = $400000. В 24-битной арифметике $C00000 = -$400000,
  поэтому:
      | Y:$50E + $C00000 | * $700  ==  | BOFS - $400000 | * $700
  Это модуль отклонения от центра 64, умноженный на $700.

База и резонанс (P:$537-$53D):
      base_term = Y:$508 * $800 - $80      ; FilterBase
      hpq_raw   = Y:$50A                   ; FilterHighPassQ

Дальше P:$586:
      do #<$18 ; div                       ; 24-битное деление
      делимое  = TBL[X:$143F95 + x0*$4AF] + TBL[X:$144446 + y0*$80]
      делитель = (a >> 1)
  Результат пишется в рабочую область Y:$91+, откуда две петли SVF
  (P:$05D3 HP и P:$05EB LP) берут готовые коэффициенты.

## ПОЛНАЯ ТАБЛИЦА 52 СЛОВ (Y:$500..$533, V = Y:$528)

  offset  Y-addr  V-off   содержимое                  дефолт
  ------  ------  ------  --------------------------  ----------
   0      $500    V-$28   AmpAttack << 16             0
   1      $501    V-$27   AmpHold << 16               0
   2      $502    V-$26   AmpDecay << 16              64
   3      $503    V-$25   AmpRelease << 16            64
   4      $504    V-$24   AmpDistortion << 16         64 (нейтраль)
   5      $505    V-$23   AmpVolume << 16             64
   6      $506    V-$22   AmpPan << 16                64 (центр)
   7      $507    V-$21   AmpPortamento << 16         0
   8      $508    V-$20   FilterBase << 16            0
   9      $509    V-$1F   FilterWidth << 16           127
  10      $50A    V-$1E   FilterHighPassQ << 16       0
  11      $50B    V-$1D   FilterLowPassQ << 16        0
  12      $50C    V-$1C   FilterAttack << 16          0
  13      $50D    V-$1B   FilterDecay << 16           32
  14      $50E    V-$1A   FilterBaseOffset << 16      64 (нейтраль)
  15      $50F    V-$19   FilterWidthOffset << 16     64 (нейтраль)
  16      $510    V-$18   EffectEqFrequency << 16     64
  17      $511    V-$17   EffectEqGain << 16          64
  18      $512    V-$16   EffectSampleRateReduction   0
  19      $513    V-$15   EffectDelayTime << 16       64
  20      $514    V-$14   EffectDelaySend << 16       64
  21      $515    V-$13   EffectDelayFeedback << 16   28
  22      $516    V-$12   EffectDelayBase << 16       0
  23      $517    V-$11   EffectDelayWidth << 16      127
  24..28  $518..C V-$10.. trackEnvelope[5] << 16      {0,0,127,127,0}
  31      $51F    V-$09   TrackLevel << 16            90
  35      $523    V-$05   tempoWord                   0x800000/(bpm*24)
  36      $524    V-$04   machineIndex                1..33
  40      $528    V+$00   noteEvent / flags           0x81 (1=on,2=off,3=fast)
  41      $529    V+$01   midiNoteWord                0x5800+(note*0x800+6)/12
  42      $52A    V+$02   velocity                    0x7FFFFF
  44..51  $52C..3 V+$04.. SynthA..SynthH << 16        {0,0,0,0,0,0,0,64}

## 9 ГРАНИЦ СТАДИЙ POST-VOICE

  stagePcs = { 0x04ff, 0x05a2, 0x06c0, 0x0789, 0x0807,
               0x087a, 0x0939, 0x0a5b, 0x0b14 }

  Порядок тракта (твой слуховой тест + MANUAL_RU.md дословно):
    synth -> dist -> srr -> filt -> eq -> env -> dsnd

  Совмещаем с 9 границами — каждый блок получает свой ТОЧНЫЙ адрес P:

    $04A8 - $04FF   счёт скаляра Y:$4FF (AMP env)     [без аудио]
    $04FF - $05A2   счёт скаляров фильтра + DIV $586  [без аудио]
    $05A2 - $05D2   DIST (Y:$504) + SRR (Y:$512) + загрузка Y:$91+   (48 слов!)
    $05D3 - $05FA   HP SVF ($05D3) + LP SVF ($05EB)
    $06C0 - $0789   EQ (после фильтра!) — EQF (Y:$510), EQG (Y:$511) (201 слово)
    $0789 - $0807   AMP ENV (x Y:$4FF) + VOL (Y:$505)                (126 слов)
    $0807 - $0A5B   TRACK DELAY (DSND/DTIM/DFB/DBAS/DWID), 3 подстадии
    $0A5B - $0B14   финальная сборка в X:$0000..$000F / Y:$0020..$002F

## ТОЧЕЧНЫЕ КОМАНДЫ (всего два маленьких куска!)

  1) DIST + SRR + загрузка двух Q в SVF (105 слов):
       python tools/mmnova_slice.py show \
         --listing "decompiled data/03_listings/dispatch/dsp1_kernel_P0000-0B4D.txt" \
         --from 0590 --to 05fa

  2) EQ после фильтра (201 слово):
       python tools/mmnova_slice.py show \
         --listing "decompiled data/03_listings/dispatch/dsp1_kernel_P0000-0B4D.txt" \
         --from 06c0 --to 0789
