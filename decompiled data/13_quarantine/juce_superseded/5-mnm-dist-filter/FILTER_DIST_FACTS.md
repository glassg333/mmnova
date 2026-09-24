# AMP DIST + FILTER — что установлено и откуда

Тиры:  A = литерал/прямая цитата источника   B = вывод из двух источников   C = не знаю

## СТРАНИЦА AMPLIFICATION
Порядок ручек: ATK HOLD DEC REL DIST VOL PAN PORT                      [A]
  источник: мануал OS 1.32 "amplifier envelope (AHDR), distortion,
            volume, pan and portamento" + пресеты сообщества, где
            записано "A / H / D / R / DIST / VOL / PAN / PORT"

MIDI CC трека 1:
  ATK=56 HOLD=57 DEC=58 REL=59 DIST=60 VOL=61 PAN=62 PORT=63
  VOL=61($3D), PAN=62($3E), PORT=63($3F) — дословно из мануала      [A]
  остальные пять — обратный отсчёт по порядку ручек                 [B]

## DIST — что это
Диапазон -64..+63, дефолт 0 (в пресетах встречается -9, -24, 0)      [A]

Семантика (мануал, раздел Effects page):
  "increase your headroom by DECREASING the DIST parameter"          [A]
  "Always leaving full headroom would result in decreased signal
   quality, with increased noise. Therefore we allow you to adjust
   the distortion to allow for the headroom your sound needs."       [A]

=> DIST это регулятор ЗАПАСА ПО ГОЛОВКЕ в фиксированной точке,
   а не drive-секция. Минус = тише и чище, плюс = упор в потолок.

Механизм искажения (аудит прошивки):
  "Дисторшн - отдельной машины нет; это флаг насыщения ALU
   DSP56300 (bset #$14,sr) - клип на +-1.0"                          [A]

Литерал того же механизма (chorus_disasm.txt):
  1476a5: bset #$14,sr     ; SR.SM=1 -> ALU клипует на +-1.0
  1476aa: bclr #$14,sr     ; SR.SM=0 -> wraparound

=> клип ЖЁСТКИЙ на $7FFFFF / $800000. Не tanh, не soft-clip.

НЕ ЗНАЮ:                                                             [C]
  - кривую display -64..+63 -> внутренний множитель
    (репо §9 п.1: кривые считает ColdFire, не вскрыто)
  - offset DIST внутри блока V-$28..V-$01
  - применяется множитель до или после фильтра

## СТРАНИЦА FILTER
Порядок ручек: BASE WIDTH HPQ LPQ ATK DEC BOFS HOFS                  [A]
  источник: пресеты сообщества (BASE=2 WIDTH=79 HPQ=84 LPQ=31
            A=0 D=0 BOFS=-6 HOFS=16)

MIDI CC: BASE=72($48) дословно из мануала                            [A]
         далее подряд 73..79                                         [B]

Топология (мануал):
  "resonant 24dB low/high/band-pass filter"
  "hi/lo/band-pass resonant 24-db filter with envelope and
   INDIVIDUAL Q-control"                                             [A]

BOFS/HOFS: "control how much of the envelope amount will be added
            to the filter Base and Width parameters"                 [A]

## ЯДРО: SVF (KERNEL_VOICE_CHAIN.md §4.6)
  Две одинаковые петли do #<$8 (8 итераций x 2 аккумулятора = 16 смп)
    петля 1: P:$05D3 - $05E8
    петля 2: P:$05EB - $05FA
  x0 = $F528BD ; x1 = $4A4DF0
  состояния X:$71+n*3 (банк a) и X:$73+n*3 (банк b), n=0..7
  коэффициенты из Y:$91+                                             [A]

ТОЧНЫЕ значения коэффициентов (считай от hex, в доке округлено!):
  $F528BD = 16066749 - 2^24 = -710467
            -710467 / 2^23 = -0.084694564
  $4A4DF0 = 4869616
             4869616 / 2^23 = +0.580503464

Репо §9 п.3 спрашивает: две петли это L/R или LP/HP?
ОТВЕТ: LP/HP.                                                        [B]
  На панели РАЗДЕЛЬНЫЕ HPQ и LPQ. Раздельный резонанс требует двух
  независимых резонансных секций. При L/R второй Q не имел бы смысла.
  Плюс мануал явно: "individual Q-control".

## ЯДРО: расчёт среза (KERNEL_VOICE_CHAIN.md §4.5)
  P:$537-$53D  TONE:  (Y:(V-$20)) * $800 - $80
  P:$53E-$56C  INTL/DPTH: |Y:(V-$1A) + $C00000| * $700
  P:$586-$588  деление: do #<$18 ; div    (24 шага, 24-битное)
    делимое  = TBL[X:$143F95 + x0*$4AF] + TBL[X:$144446 + y0*$80]
    делитель = (a >> 1)                                              [A]

  X:$143F95 - 107 слов, X:$144446 - 128 слов.
  ОБЕ ТАБЛИЦЫ В РЕПО НЕ ВЫГРУЖЕНЫ. Без них точный срез не собрать.   [C]

## ПОПРАВКА К МОИМ ПРОШЛЫМ ОТВЕТАМ
1. Цепочка "Machine -> Distortion & SRR -> Filter" была взята из
   06_docs/README_architecture_routing.md. Этот файл ЗАГРЯЗНЁН:
   внутри текст чужой сессии (juce::MidiBuffer, MonomachineOne.vst3,
   "PR #1 merged", ветка arena/01a0a623-mytt, репозиторий mytt).
   Источником считать нельзя.
   Строгий KERNEL_VOICE_CHAIN.md даёт: machine osc -> [AMP env ->
   LFO1/2/3 -> SVF] -> микшер. Отдельной стадии дисторшна там нет.

2. Коэффициенты я округлял неверно (-0.08475685 / +0.58044434).
   Верно: -0.084694564 / +0.580503464.

3. SRR (sample rate reduction) живёт на странице EFFECTS
   (EQF EQG SRR DTIM DSND DFB DBAS DWID), а не на AMP.
   Там же 1-полосный EQ - он существует как ТРЕКОВЫЙ эффект,
   что не противоречит выводу аудита "FX-EQ как МАШИНЫ нет"
   (слот m14 действительно SWAVE-ENS).
