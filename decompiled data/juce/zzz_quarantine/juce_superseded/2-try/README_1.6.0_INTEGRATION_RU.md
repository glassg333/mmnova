# Интеграция блоков DSP56303 в Monomachine-Nova-1.6.0

## ФИЛЬТР / ДИСТОРШН / ОГИБАЮЩАЯ (главный запрос)
- **Фильтр:** Source/DSP/Kernel/MultiModeFilterBlock.h
  - Global SVF Base/Width: $F528BD(-0.08476) / $4A4DF0(+0.58044), каскад P:$05D3/$05EB.
  - Литеральный 2-полюсник TwoPoleResonator_P144D21: $1E454E / $5B75B8 (из GND-SIN).
- **Дисторшн:** Source/DSP/Kernel/DistortionSaturationBlock.h
  - SR.SM клип ±1.0 (bset #$14,sr @ $1476A5) = аутентичный дисторшн, отдельной машины нет.
  - Цепочка Machine -> Distortion & SRR -> Filter (README_architecture_routing §1).
- **Огибающая:** Source/DSP/Kernel/AmpEnvelopeBlock.h
  - Машина состояний ATK(1)/DEC(4)/SUS(5)/REL(2), таблицы Y:$141800 / $141880.
  - LFO коэфф. $791FD0=0.94628, прогиб Decay до 150% (1.6.0).
- Собирает всё: Source/DSP/Kernel/KernelVoiceChainBlock.h (Dist -> Filter -> Env).

## Ядро и таблицы
- Dsp56300Core.h — Q23/56-bit ALU, SR.SM, фикс SRAM >= $100000.
- MmNovaTables_Fixed.h — восстановленные ненулевые таблицы.

## Машины (готово): FXChorusBlock, FMPlusBlocks, SwaveBlocks (MNM/OLD), DproBboxBlock, GndAndSidBlocks.

## Очередь 2 (долгая добыча Page B/C): VO-6, DPRO-WAVE/DDRW/DENS, точная докалибровка REVERB/PHASER.


📉 ОГИБАЮЩАЯ AMP (точная)
Машина состояний func_0004A8 (P:$04A8–$04F5).

state 1 ATK: +TBL[Y:$141800]
state 4 DEC: (DEC)² + TBL[$141880]
state 5 SUS: level=(DEC)²
state 2 REL: −TBL[$141880]
LFO $791FD0 = 0.9462824
1.6.0: Decay bend до 150%


зачем нужно было decay трогать до того как я попросил интегрировать в плагин нужно опять сверяться с оригиналом








🔥 ДИСТОРШН (точный)
Отдельной машины НЕТ. Дисторшн = флаг насыщения ALU SR.SM, хард-клип ±1.0 в Q23.

1476a5: bset #$14,sr (вкл. клип)
1476aa: bclr #$14,sr (выкл.)
Цепочка (arch README):
Machine → Dist & SRR → Filter
SRR: mpysu+dmac, табл. $101AFB





🎛 ФИЛЬТР (точный)
Глобальный Base/Width — 2-pole SVF ядра, двойной каскад P:$05D3 / P:$05EB.

$4A4DF0 = +0.58044434 (freq coeff)
$F528BD = −0.08475685 (damping/FB)
+ ЛИТЕРАЛЬНЫЙ 2-полюсник P:$144D21:
$1E454E = 0.23649215 (C1)
$5B75B8 = 0.71452600 (C2)2 try filter dist env nice build