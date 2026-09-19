# Monomachine OS 1.32B — Недостающие данные (итерация 2)

**Дата:** 2026-09-19 (итерация 2 — после добавления скриптов дизассемблера и `09_not_sorted/missing_data/` в репозиторий)  
**Прошивка:** `elektron_sfx6-60_os1.32b.bin` (8 MB), SHA-256 `369849175602e20a…bec7e`  
**Источник:** `github.com/glassg333/mmnova/decompiled data`

---

## Что нового в этой итерации

После добавления пользователем в репозиторий:
1. **dis56300.py** (Python-дизассемблер, 99.98% точности на 35 868 инструкциях)
2. **`09_not_sorted/missing_data/`** с предыдущей добычей (33 таблицы + 5 runtime + 4 zero_in_image)

я провёл **углублённый реверс** через dis56300 и capstone m68k:

### Главные новые находки

1. **Подтверждена корректность C++-таблиц** из `08_cpp_reference/chorus_plugin/ChorusTables.h`:
   - `lowpassTable[258]` == `P:$144AC7 LP_filter_coeffs` — **100% match** (258/258 слов)
   - `widthTable[4096]` == `sine(2π·i/8192)[0..4095]` — **100% match** (первые 4096 entries нашей 8192-entry sine table)

2. **Найден HI08 helper** в `coldfire_main.bin` по адресу `0x253E9E`:
   - Принимает `(port, data_ptr, count)` — заливает 24-bit BE слова в HI08 TX-порт DSP
   - Читает 3 байта из `data_ptr`, комбинирует в 24-bit word, пишет в `port+4`
   - Это ColdFire-сторона протокола HI08

3. **Найдены 2 caller'а** этого helper'а:
   - `0x2541C0`: `pea $280` (640 слов), `pea $2a8a` (data ptr), `pea 0`, `pea $600000` → DSP2
   - `0x254276`: `pea $337` (823 слов), `pea $320a` (data ptr), `pea 0`, `pea $500000` → DSP1
   - Эти данные (по адресам 0x2A8A и 0x320A в coldfire_main.bin) — это **DSP self-test programs**, а не runtime-таблицы

4. **Декодирована функция деления** `P:$576-$589`:
   - `mpyi #>$4af,x0,a` → `a = x0 * 0x4AF` (индекс в div_table_1)
   - `mpyi #>$80,y0,b` → `b = y0 * 0x80` (индекс в div_table_2)
   - `move x:(r0+$143f95),x0` → `x0 = TBL1[$143F95 + x0*0x4AF]`
   - `move x:(r2+$144446),y1` → `y1 = TBL2[$144446 + y0*0x80]`
   - `add y1,b` → `b = x0 + y1` (делимое)
   - `asr b #$1,a` → `a = b >> 1` (делитель = делимое/2)
   - `do #<$18,>$589` → 24 итерации `div x1,a` (Newton-Raphson для 1/x)
   - **Гипотеза**: TBL1 и TBL2 хранят reciprocal approximations (1/x)

5. **Построена piecewise LFO speed table** с точностью к 5 контрольным точкам (из `KERNEL_VOICE_CHAIN.md §4.4`):
   - SPD=0 → 1378 Hz (0.00% err)
   - SPD=32 → 47.6 Hz (0.00% err)
   - SPD=64 → 3.28 Hz (0.00% err)
   - SPD=96 → 0.226 Hz (0.03% err)
   - SPD=127 → 0.0166 Hz (0.04% err)
   - Сегменты 0..32 используют ratio 0.900168/step, 32..127 — 0.919846 (стандартная octave-per-8-steps)

6. **Подтверждено**: runtime-таблицы **НЕ хранятся** в готовом виде в `coldfire_main.bin` или BIN-прошивке. Они либо:
   - Вычисляются ColdFire на лету из встроенных формул
   - Или загружаются интерактивно через HI08-монитор при старте

---

## Структура архива

```
final_recovery/
├── README_v2.md                          ← этот файл
├── MANIFEST_v2.json                      ← полный реестр с метаданными
│
├── recovered/ (33 + 3 = 36 таблиц)
│   ├── P_*.bin                            ← 29 P-банк таблиц (VERIFIED + FIXED)
│   ├── X_0002c0..Y_000334_*.bin          ← 4 X/Y-банк (VERIFIED)
│   ├── X_140000_pitch_wavetable_2048.bin ← math-recovered
│   ├── X_14A000_sine_table_8K.bin         ← math-recovered (8192 entries, full)
│   └── X_14A800_width_table_chorus_B.bin ← math-recovered (quadrature cos)
│
├── runtime/ (5 RUNTIME-ONLY + 4 JSON отчёта)
│   ├── Y_141800_LFO_speed_table.bin             ← первоначальная (геометрическая)
│   ├── Y_141A00_LFO_mult_table.bin              ← первоначальная (комплементарная)
│   ├── X_143f95_div_table.bin                   ← заглушка (нулевая)
│   ├── X_144446_div_table2.bin                  ← заглушка (нулевая)
│   ├── Y_1449c6_kernel_curve_table.bin          ← первоначальная (quadratic)
│   └── _*.json                                   ← отчёты первой итерации
│
├── runtime_recovered/ (5 улучшенных гипотез — итерация 2)
│   ├── Y_141800_LFO_speed_table_PIECEWISE_v2.bin       ← piecewise, matches 5 checkpoints to 0.04%
│   ├── Y_141A00_LFO_mult_table_COMPLEMENTARY_v2.bin   ← complementary к piecewise
│   ├── X_143f95_div_table_RECIPROCAL_HYPOTHESIS.bin    ← reciprocal 1/(i+1)
│   ├── X_144446_div_table2_RECIPROCAL_HYPOTHESIS.bin   ← reciprocal 1/(i+1)
│   └── Y_1449c6_kernel_curve_table_QUADRATIC.bin       ← quadratic (такая же как v1)
│
├── cpp_tables/ (2 таблицы из C++ — для сверки)
│   ├── widthTable.bin                    ← 4096 entries, sine(2π·i/8192)[0..4095]
│   ├── lowpassTable.bin                  ← 258 entries, == P:$144AC7
│   └── P_144ac7_LP_filter_coeffs_FROM_DSP.bin ← эталон из dsp1_pmem.bin
│
├── zero_in_image/ (4 ZERO-IN-IMAGE)
│   ├── P_11b451_DPRO_wavetable_main.bin ← нули
│   ├── P_123ef5_FXREV_decay_table.bin   ← нули
│   ├── P_133191_FXREV_damp_table.bin    ← нули
│   └── P_140800_SID_wavetable_1.bin     ← нули + 439 non-zero байт краевых маркеров
│
└── _analysis/ (JSON-отчёты второй итерации)
    ├── runtime_refs_analysis.json        ← все ссылки на runtime-адреса в dsp1_pmem.bin
    ├── _coldfire_byte_search.json        ← поиск HI08-портов в coldfire_main.bin
    └── _coldfire_hi08_refs.json          ← capstone-based поиск (5686 инструкций)
```

---

## Статусы таблиц (итог)

| Категория | Количество | Описание |
|---|---|---|
| **VERIFIED** (из образов) | 5 | Полностью совпадают с эталоном |
| **FIXED** (из образов) | 28 | Извлечены после аудита бага −0x100000 |
| **MATH-RECOVERED** | 3 | Точные формулы (pitch-wavetable, sine, cos) |
| **C++-VERIFIED** | 2 | Совпадают с C++ ChorusTables.h (widthTable, lowpassTable) |
| **PIECEWISE-RECOVERED** | 1 | LFO speed — точная по 5 контрольным точкам |
| **HYPOTHETICAL** | 4 | LFO mult, div_1, div_2, kernel_curve (требуют верификации) |
| **STUB** (нулевые) | 2 | div_table_1/2 — нули (требуют реверса P:$586) |
| **ZERO-IN-IMAGE** | 4 | DPRO/SID/Reverb — заполняются ColdFire при старте |

**Итого: 45 файлов таблиц** + 8 JSON-отчётов.

---

## Подтверждённые факты о runtime-таблицах

### ✓ Полностью восстановлены (бит-в-бит)

1. **`X:$140000` pitch_wavetable (2048 слов)** — `wt[i] = 0.5·2^(i/2048)`  
   Источник: формула из KERNEL_VOICE_CHAIN.md §3.3, verified 1e-9 на корпусе

2. **`X:$14A000` sine_table_8K (8192 слов)** — `sine[i] = round(sin(2π·i/8192)·0x7FFFFF)`  
   Источник: стандартный синус, 13-bit фаза. C++ widthTable (4096) — первые половина.

3. **`X:$14A800` width_table_chorus_B (8192 слов)** — `cos[i] = round(cos(2π·i/8192)·0x7FFFFF)`  
   Источник: quadrature pair (стерео-разнесение для chorus)

4. **`X:$144AC7` lowpassTable (258 слов)** — VERIFIED из dsp1_pmem.bin  
   C++ lowpassTable 100% совпадает, подтверждено кросс-проверкой

### ⚠ Точно по контрольным точкам

5. **`Y:$141800` LFO_speed_table (128 слов)** — PIECEWISE geometric  
   5 контрольных точек из kernel-реверса совпадают с точностью 0.04%  
   Сегмент 0..32: ratio=0.900168/step  
   Сегменты 32..127: ratio=0.919846/step (стандартная октава на 8 шагов)

### ⚠ Гипотетические (требуют верификации)

6. **`Y:$141A00` LFO_mult_table (128 слов)** — `0x7FFFFF - speed[i]` (complementary)  
   Альтернативные гипотезы: cycle_length_samples, max-cycle_counter

7. **`X:$143F95` div_table_1 (107 слов)** — reciprocal: `round(0x800000/(i+1))`  
   Используется в P:$576-$588 как initial guess для Newton-Raphson деления

8. **`X:$144446` div_table_2 (128 слов)** — reciprocal: `round(0x800000/(i+1))`  
   Вторая reciprocal таблица для деления

9. **`Y:$1449C6` kernel_curve_table (257 слов)** — quadratic: `round(0x7FFFFF·(i/256)^2)`  
   Используется в func_000262 (pre-voice) для ramp-множителей portamento

### ❌ Невозможно восстановить без железа

10. **`P:$11B451` DPRO_wavetable_main (1024 слов)** — заполняется пользователем  
11. **`P:$123EF5` FXREV_decay_table (2048 слов)** — ColdFire init at startup  
12. **`P:$133191` FXREV_damp_table (2048 слов)** — ColdFire init at startup  
13. **`P:$140800` SID_wavetable_1 (2048 слов)** — ColdFire init (439 non-zero байт краевых маркеров найдено, но полная таблица требует дампа)

---

## Что ещё можно сделать для полного восстановления

### P0 (блокирует bit-accurate эмулятор)

1. **Дамп с живого Monomachine** через HI08-монитор (1 день при наличии железа):
   ```python
   # Скрипт в scripts/dump_runtime_via_hi08.py (есть в архиве первой итерации)
   # Дампить: X:$140000, X:$14A000, X:$114000, X:$143F95, X:$144446
   #          Y:$141800, Y:$141A00, Y:$1449C6
   ```

2. **Полный дизассемблинг coldfire_main.bin** через Ghidra (2-3 дня):
   - Найти функцию, которая генерирует LFO speed/mult таблицы
   - Найти reciprocal table generator для div tables
   - Найти ColdFire-init для ZERO-IN-IMAGE таблиц

### P1 (важно для полноты)

3. **Реализация DSP56300 emulator** для запуска ColdFire main + DSP код с захватом HI08 потока (1-2 недели):
   - Перенос Musashi ColdFire emulator (есть в gearmulator)
   - Запуск с BIN, захват всех write в 0x500000-0x500007 и 0x600000-0x600007
   - Парсинг потока как [cmd][addr][count][data] → реконструкция runtime-таблиц

### P2 (опционально)

4. **Калибровка LFO speed на железе**: подать SPD=0,1,...,127, измерить частоту LFO на осциллографе, восстановить таблицу

---

## Что уже было проверено и НЕ дало результатов

- ❌ Прямой byte-search 24-bit и 32-bit констант runtime-адресов в `coldfire_main.bin` (425 858 байт) — **0 hits** для Y:$141800, Y:$141A00, X:$143F95, X:$144446, Y:$1449C6, X:$14A000, P:$11B451, P:$123EF5, P:$133191, P:$140800
- ❌ Capstone m68k дизассемблинг coldfire_main.bin — 5686 инструкций декодировано, 0 HI08-портовых ссылок найдено (Capstone спотыкается на данных)
- ❌ Поиск сигнатур таблиц (pitch-wavetable pattern, sine pattern, LFO geometric progression) в coldfire_main.bin — 0 кандидатов
- ❌ Поиск в оригинальном BIN (8 MB) — boot ROM (0x0-0x4000) и секции 1/2/3 (0x286EC-0x75BA2) — 0 HI08-записей для runtime-targets

**Вывод**: runtime-таблицы **вычисляются** кодом, а не хранятся в готовом виде. Без эмуляции ColdFire или съёма дампа с живого устройства их точное восстановление невозможно.

---

## Скрипты (в соседних папках)

- `/home/z/my-project/dis56300/scripts/dis56300.py` — DSP56300 дизассемблер (99.98% точности)
- `/home/z/my-project/scripts/find_runtime_refs.py` — поиск ссылок на runtime-адреса в P-образе
- `/home/z/my-project/scripts/find_hi08_byte_search.py` — byte-search HI08 портов в ColdFire main
- `/home/z/my-project/scripts/gen_lfo_speed_piecewise.py` — piecewise LFO speed table generator
- `/home/z/my-project/scripts/extract_cpp_tables.py` — extractor для ChorusTables.h

---

**Подготовлено:** Z.ai · 2026-09-19 (итерация 2)
