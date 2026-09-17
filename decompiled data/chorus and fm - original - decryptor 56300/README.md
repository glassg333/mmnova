# mmnova_pack — недостающие машины Monomachine OS 1.32B

Эта сборка содержит **полные дизассемблированные листинги всех машин**, отсутствующих
в предыдущей поставке (`chorus and fm - original - decryptor 56300`), плюс все
 data-таблицы, на которые они ссылаются, и распакованные дескрипторы параметров.

## Источник

Все листинги извлечены из **уже верифицированного** образа P-памяти DSP1
(`decompiled data/dsp1_pmem.bin`, 1 343 588 слов, SHA-256 совпадает с прошивкой
`elektron_sfx6-60_os1.32b.bin`). Дизассемблирование выполнено скриптом
`scripts/extract_missing_machines.py` из `dsp1_machines_fx.txt` — того же
дизассемблера, что использовался в предыдущей сессии для chorus/FM.

**Никаких новых дизассемблирований «с нуля» не выполнялось** — использованы
только уже аутентифицированные данные репозитория `glassg333/mmnova`.

## Что внутри

```
mmnova_pack/
├── README.md                              этот файл
├── MANIFEST.txt                           список машин + адреса init/config/process
│
├── listings/                              новые листинги (16 машин)
│   ├── 01_GND-SIN_full.txt                165 строк   P:$144CC9..$144D99
│   ├── 02_GND-NOIS_full.txt               199 строк   P:$144D9A..$144E80
│   ├── 03_SID_full.txt                    329 строк   P:$14636F..$1464FB
│   ├── 04_SWAVE-SAW_full.txt              328 строк   P:$145932..$145AB4
│   ├── 05_SWAVE-PULS_full.txt             344 строк   P:$147214..$1473A1
│   ├── 06_DPRO-WAVE_full.txt              264 строки  P:$1473A2..$1474E8
│   ├── 07_DPRO-BBOX_full.txt              289 строк   P:$1474E9..$147660
│   ├── 11_VO-6_full.txt                   2273 строки  P:$1464FC..$147213  (самая большая машина)
│   ├── 13_FX-REV_full.txt                 1263 строки  P:$14538E..$145AB4  (reverb)
│   ├── 14_SWAVE-ENS_full.txt              468 строк   P:$145AB5..$145D11
│   │   └─ примечание: пользователь изначально назвал это «FX-EQ», но это не FX-машина,
│   │      а SWAVE-ENS (superwave ensemble). Дескриптор ColdFire @0x58950 говорит
│   │      SWAVE-ENS, params PCH2/PCH3/PCH4/WAVE/CHRL/CHRW/TUNE. EQ в Monomachine
│   │      не реализован как отдельная FX-машина — фильтрация делается через LP/HP
│   │      параметры внутри других эффектов (reverb, chorus).
│   ├── 16_FX-DYN_full.txt                 155 строк   P:$1477DE..$14789D  (compressor)
│   │   └─ ATK/REL/THRS/MIX/RAT/GAIN/RMS/INP
│   ├── 17_FX-RING_full.txt                510 строк   P:$14789E..$147B37  (ring mod)
│   ├── 18_FX-PHA_full.txt                 662 строки  P:$145036..$14538D  (phaser)
│   │   └─ CNTR/DEP/SPD/MIX/FB/WID — all-pass ladder, LFO inline
│   ├── 19_FX-FLA_full.txt                 345 строк   P:$144E81..$145035  (flanger)
│   │   └─ DEL/DEP/SPD/MIX/FB/WID — short comb delay, LFO inline
│   ├── 32_FX-DLY_full.txt                 402 строки  P:$147B38..$147D00  (delay)
│   │   └─ 2 канала, delay buffer X:$114000
│   └── 33_FX-EXT_full.txt                 737 строк   P:$147D01..$148063  (bitcrush / external)
│       └─ 4 канала, X:$114000 buffer + pitch tables $101AFB/$101BFB/$101CFB
│
├── tables/                                data-таблицы, извлечённые из P/X/Y-образов
│   ├── TABLES_MANIFEST.json               полный список с адресами и размерами
│   ├── P_144ac7_LP_filter_coeffs.*        LP-фильтр (shared FM TONE + CHORUS LP + REV LP)
│   ├── P_141900_PHASER_stage0_table.*     4 all-pass таблицы phaser
│   ├── P_141980_PHASER_stage1_table.*
│   ├── P_141a00_PHASER_stage2_table.*
│   ├── P_141b18_PHASER_stage3_table.*
│   ├── P_142f06_PHASER_helper_table.*
│   ├── P_143546_PHASER_cntr_table.*
│   ├── P_1444c6_FXDYN_attack_table.*      attack/release/RMS таблицы компрессора
│   ├── P_1446c6_FXDYN_release_table.*
│   ├── P_144746_FXDYN_rms_table.*
│   ├── P_123ef5_FXREV_decay_table.*       decay/damp таблицы реверба
│   ├── P_133191_FXREV_damp_table.*
│   ├── P_101a7b_SWAVE_wavetable_main.*    SWAVE wavetable
│   ├── P_101afb_SWAVE_PCH2_table.*        pitch tables (используются m4/m5/m14/m32/m33)
│   ├── P_101b7b_SWAVE_PCH3_table.*
│   ├── P_101bfb_SWAVE_PCH4_table.*
│   ├── P_101cfb_SWAVE_PCH5_table.*
│   ├── P_101d7b_DPRO_wavetable_ref.*
│   ├── P_10750c_DPRO_BBOX_table1.*        DPRO-BBOX tables (m7)
│   ├── P_10b1bc_DPRO_BBOX_table2.*
│   ├── P_10b5c7_GND_NOIS_table.*          GND-NOIS noise color (m2)
│   ├── P_10f7a1_DPRO_BBOX_table3.*
│   ├── P_11320f_DPRO_BBOX_table4.*
│   ├── P_11b451_DPRO_wavetable_main.*     DPRO wavetable (m6/m7)
│   ├── P_1001d1_VO6_dispatch_table.*      VO-6 tables (m11)
│   ├── P_1007f1_VO6_table2.*
│   ├── P_140000_SID_wavetable_0.*         SID wavetables (m3)
│   ├── P_140800_SID_wavetable_1.*
│   ├── P_1435c6_SID_helper_table.*
│   ├── P_143d06_SID_filter_table.*        SID filter coefficients
│   ├── P_144c49_m32_helper_table.*        FX-DLY helper
│   ├── P_145c48_m33_helper_table.*        FX-EXT helper
│   ├── Y_000132_sine_table_FM.*           синус-таблица (shared FM + LFO)
│   ├── Y_000334_LFO_wave_triangle.*       LFO triangle/saw волна (256 слов)
│   ├── X_0002c0_audio_buffer_bases.*      base pointers аудио-буферов
│   ├── X_000394_pitch_table_1.*           pitch conversion (32 слова)
│   └── X_0003f4_pitch_table_2.*           pitch conversion (32 слова)
│
├── descriptors/                           распакованные дескрипторы параметров
│   ├── fx_descriptors.json                8 блоков из fx_descriptors_region.bin
│   ├── fm_descriptors.json                4 блока из fm_machine_descriptors.bin
│   ├── all_machine_descriptors.json       полный скан ColdFire main (0x57FB0..0x58E20)
│   └── coldfire_main.bin                  распакованный ColdFire main (425 858 байт)
│
├── original_pack/                         предыдущая поставка (для полноты)
│   ├── REPORT_verdict_and_fm_data.md      вердикт по chorus + FM данные
│   ├── AGENTS_README.md
│   ├── worklog.md
│   ├── listings/                          chorus_full + fm_stat/par/dyn
│   ├── tables/                            fm_dyn_wavetable + fm_ratio_table + sin_table_y0132
│   ├── descriptors/                       fm_machine_descriptors.bin + fx_descriptors_region.bin
│   ├── data/                              my_dsp{1,2}_{x,y}mem.bin
│   └── scripts/                           mm_aplib_mm.py + mm_final_extract.py + dis56300.py и др.
│
└── memory_images/                         полные образы памяти (ссылка)
    ├── dsp1_pmem.bin                      1 343 588 слов P-памяти DSP1
    ├── dsp2_pmem.bin                      DSP2 (отличается только ядром P:$0..$B4C)
    ├── dsp1_xmem.bin                      1 044 слова X-памяти
    ├── dsp1_ymem.bin                      1 076 слов Y-памяти
    ├── dsp2_xmem.bin
    ├── dsp2_ymem.bin
    ├── dsp1_dispatch.txt                  листинг таблиц диспетчеризации (P:$100000..)
    ├── dsp1_machine_code.txt              листинг машинного кода (P:$140000..)
    ├── dsp1_machines_fx.txt               листинг всех FX-машин (P:$141A98..$148063)
    ├── dsp1_vectors_core.txt              листинг ядра + векторов прерываний
    └── dsp2_dispatch.txt / dsp2_machine_code.txt / dsp2_machines_fx.txt / dsp2_vectors_core.txt
```

## Важные замечания

### 1. LFO, фильтр, дисторшн, delay-send, биткраш — INLINE в каждой машине

В прошивке Monomachine **нет отдельных kernel-рутин** для LFO, фильтра, дисторшна,
delay-send или биткраша. Каждый эффект реализует эти компоненты **inline** в своём
коде:

- **LFO** — каждая FX-машина (chorus, phaser, flanger, ring-mod) имеет собственный
  LFO-осциллятор, реализованный как аккумулятор фазы + таблица синуса Y:$0132.
  Скорость (SPD) читается из `y:(r6+$6)`, фаза хранится в `y:(r6+$24)` (chorus),
  `y:(r6+$1B)` (phaser), и т.д. Универсального LFO-генератора нет.
  
- **Фильтр LP** — shared LP-таблица `P:$144AC7` (258 слов) используется chorus,
  FM (TONE), reverb (LP). Каждый эффект читает свой TONE/LP параметр, делает
  `asr #$10` → индекс в таблицу, применяет коэффициент. Никакого отдельного
  filter-модуля нет.
  
- **Дисторшн** — в Monomachine нет отдельной distortion-машины. Искажение
  реализовано через saturation flag (`bset #$14,sr` включает режим насыщения ALU)
  внутри chorus (см. `1476A5` в `chorus_full.txt`) и FM-машин. Это и есть
  «дисторшн» — DSP56300 имеет встроенный saturation mode, который клиппует
  аккумулятор на ±1.0.
  
- **Delay-send / routing** — у каждой FX-машины есть параметр INP (input) —
  это и есть send-уровень. Mix routing реализован через `y:(r6+$7)` = MIX,
  и формула `wet × input + dry × (0x7FFFFF − MIX)` (см. `14767D..147686`
  в chorus). Отдельной send-bus шины нет — каждая FX читает свой INP
  и микширует с dry.
  
- **Биткраш** — в m33 (FX-EXT) рядом с delay-буфером $114000 есть
  pitch-table lookup $101AFB/$101BFB/$101CFB — это downsample/decimate,
  что и есть биткраш. Конкретные инструкции: `147D29..147D30` (mpysu+dmac
  pattern = fractional table lookup = sample-rate reduction).

### 2. Про «FX-EQ» в пользовательском нейминге

Пользователь изначально назвал m14 «FX-EQ», потому что машина попадала в
«секцию C» структуры. На самом деле это **SWAVE-ENS** (superwave ensemble) —
не отдельный эффект, а разновидность SWAVE-машины (как SAW и PULS).
Дескриптор ColdFire @0x58950 подтверждает: short_name=SWAVE, display_name=ENS,
params PCH2/PCH3/PCH4/WAVE/CHRL/CHRW/TUNE.

EQ как отдельный эффект в Monomachine **отсутствует** — фильтрация делается
через LP/HP параметры внутри reverb (HP+LP) и chorus (LP).

### 3. Про chorus 70% wet

Пользователь сообщил, что chorus звучит как 70% wet при standalone-подключении,
но 100% wet при посыле с мономашины. Это объясняется в `original_pack/REPORT_verdict_and_fm_data.md`:
при MIX=127 (0x7F) код оставляет ~0.78% dry сигнала из-за `cmp #>$80,a; bge skip`
— то есть wet не доходит до 100%. Это **оригинальное поведение прошивки**, не баг
декомпиляции. При посыле с мономашины входной сигнал уже обработан и нормирован,
поэтому остаточный dry не слышен.

### 4. Что НЕ вошло в пакет

- Полный независимый дизассемблер DSP56300 — заготовка `scripts/dis56300.py`
  в `original_pack/scripts/` парсит только базовые группы (move/mac/mpy/asl/...),
  для сверки FM/chorus листингов достаточно существующего `dsp1_machines_fx.txt`.
  
- Декодирование кривых параметров (байты `25 d3 30 ...` в дескрипторах) —
  это отдельная задача, кривые пока не расшифрованы.
  
- Портирование в C++ — листинги готовы, портирование можно делать по ним.

## Воспроизводимость

Все скрипты, использованные для извлечения, сохранены:
- `scripts/extract_missing_machines.py` — парсинг dispatch + slice листингов
- `scripts/decode_descriptors.py` — декодирование fx/fm descriptor region
- `scripts/extract_tables.py` — извлечение data-таблиц из P/X/Y-образов
- `scripts/extract_coldfire_descriptors.py` — распаковка ColdFire main + скан дескрипторов

Скрипты можно повторно запустить на любом обновлённом образе прошивки.
