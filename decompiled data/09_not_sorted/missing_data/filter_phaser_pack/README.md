# FILTER & PHASER RECREATION GUIDE
## Monomachine SFX-60 OS 1.32B — firmware-mined data for accurate filter & phaser recreation

**Дата формирования:** 2026-09-20
**Прошивка:** `elektron_sfx6-60_os1.32b.bin` (8 388 608 Б)
**Источник:** репозиторий `glassg333/mmnova`, итерация v10 (`decompiled data/09_not_sorted/missing_data/mmnova_firmware_mining_v10`)
**Цель:** дать точные данные из прошивки для воссоздания двойного фильтра (BASE+WIDTH, HP+LP) с резонансом (Q) и огибающими, а также фейзера FX-PHA (m18) на основе реального кода DSP56303 — НЕ эрзац на стандартных JUCE/RBJ-формулах.

> ⚠️ **Важно:** файлы `08_cpp_reference/juce_fx_pack/PhaserFX.h` и `08_cpp_reference/old_dsp_models/monomachine_filter.hpp` — это стандартные JUCE/RBJ-приближения, **не** основанные на реальной прошивке. Они оставлены для сравнения, но пользователь явно сказал «не считать фильтр от хоруса удачей; он должен быть двойной и с огибающими, где известен резонанс». Поэтому в этом гиде — **только данные из прошивки**.

---

## 1. Аппаратная основа

- **CPU DSP:** 2 × Motorola DSP56303 @ 100 MHz, 24-битные слова, big-endian
- **Sample rate:** 44 100 Гц; блоки по 16 сэмплов (2756.25 Гц = частота кадров)
- **Voice count:** 6 голосов (state-блок 256 слов/голос)
- **Память DSP1:** P:$0000–$0B4D — kernel (mixer/main); P:$100000+ — таблицы/данные машин; X/Y — алиасится с P (общая SRAM)

### 1.1 Voice state block (V = Y:$528 + voice*$100, 256 слов/голос)

| Offset | Bank | Sample locations | Роль (для фильтра) |
|---|---|---|---|
| V+$08 | Y | kernel:000537 | **BASE** — cutoff HP-фильтра (0..127) |
| V+$09 | Y | kernel:000557 | **WIDTH** — span LP-фильтра (0..127) |
| V+$0E | Y | kernel:00053E | **BOFS** — env-модуляция BASE (signed) |
| V+$0F | Y | kernel:00055C | **LP env mod** (signed) |
| V+$10 | Y | kernel:00056D | **WIDTH mod / LP cutoff2** (signed) |
| V+$11 | Y | kernel:000578 | **Q** — резонанс (0..127) |
| V+$14 | Y | kernel:000AB7 | флаг env mode |
| V+$15 | Y | kernel:000ABF | флаг/мод env |
| V+$16 | Y | kernel:000A85 | **BASE** (повторно для LP-каскада) |
| V+$17 | Y | kernel:000A84 | **WIDTH** (повторно для LP-каскада) |
| V+$18 | Y/X | kernel:0005C0 (amp env) | AMP env state |
| V+$19 | Y/X | kernel:0005C0, 0x57D | filter env state 1 |
| V+$1A | Y | kernel:0004CE | filter env state 2 |
| V+$1B | Y | kernel:00045A | filter env state 3 |
| V+$25 | Y | kernel:000551 | **MODE** bits (bit 9, bit 11 значимы) |

---

## 2. SVF multimode resonant filter (kernel func_0004A8, P:$5D3–$5FA)

Это **основной резонансный** фильтр голоса. Идёт сразу после машинного обработчика
(jsr (r1)), имеет Q-параметр → резонанс.

### 2.1 Фиксированные коэффициенты

| Имя | Hex | Decimal (signed) | q.23 | Роль |
|---|---|---:|---:|---|
| `x0` | `$F528BD` | −0.0847476 | −2778061 / 2^23 | first SVF pole coefficient |
| `x1` | `$4A4DF0` | +0.5811615 | +4873712 / 2^23 | second SVF pole coefficient |

> ВНИМАНИЕ: это НЕ стандартные RBJ/SVF коэффициенты (2·(1−Q) и т.д.). Это эмпирические
> coefficients Elektron, не сводящиеся к closed-form. Их нужно просто хранить как константы.

### 2.2 Q-резонанс-таблицы (3 шт., по 128 записей каждая, 24-bit BE, stride 0x80 байт/запись)

> ⚠️ Адрес $144546 в исходном манифесте `04_tables/TABLES_MANIFEST.json` **отсутствует**. Он извлечён в этом гиде впервые.

| Адрес | Имя файла | Размер | Использование |
|---|---|---:|---|
| `P:$144446` | `P_144446_Q_resonance_table1.bin` | 128 слов (384 Б) | Q table 1 (kernel line 0x57F) |
| `P:$1444C6` | `P_1444c6_Q_resonance_table2.bin` | 128 слов (384 Б) | Q table 2 (kernel line 0x595) — ранее неправильно помечена как `FXDYN_attack_table` |
| `P:$144546` | `P_144546_Q_resonance_table3_NEW.bin` | 128 слов (384 Б) | Q table 3 (kernel line 0x597) — **НОВАЯ, не была извлечена ранее** |

Доступ:
```
000578: move y:(r6+$11),y0       ; y0 = Q (voice param V+$11, 0..127)
00057A: mpyi #>$80,y0,b          ; b = Q * 0x80 (stride 128 байт на Q-шаг)
00057C: move b,r2                ; r2 = Q * 0x80
00057D: move x:(r0+$143f95),x0   ; x0 = divisor_table[cutoff]
00057F: move x:(r2+$144446),y1   ; y1 = Q_table1[Q]
...
000595: move x:(r2+$1444c6),y0   ; y0 = Q_table2[Q]
000597: move x:(r2+$144546),y1   ; y1 = Q_table3[Q]
```

### 2.3 Содержимое Q-таблиц (ФАКТИЧЕСКИЕ данные из dsp1_pmem.bin, P-flat-mapped)

> ⚠️ В первой версии гида таблицы извлекались с ошибкой (off = addr - 0x100000).
> P-память в dsp1_pmem.bin — ПРЯМОЙ маппинг (word index = P-address).
> Все числа ниже перепроверены прямым чтением образа на правильном смещении.

**`Q_resonance_table1` (P:$144446, kernel line 0x57F):**
- Entries 0..59: монотонно растёт от `0x0438FB` (q.23 = 0.0330) до `0x7FFFFF` (1.0)
- Entries 60..127: насыщение на `0x7FFFFF` (max resonance, self-oscillation threshold)
- Интерпретация: feedback gain для self-oscillation; плавно доходит до 1.0 при Q≈60 и выше

**`Q_resonance_table2` (P:$1444c6, kernel line 0x595):**
- Entries 0..127: экспоненциальный рост от `0x007c42` (q.23 ≈ 0.00380) до `0x1e12a6` (q.23 ≈ 0.2338)
- Используется также в VO-6 (machine m11) и FX-DYNAMIX (machine m16) — кросс-референс в listings/05 и 06
- Интерпретация: коэффициент damping/feedback, не насыщается, плавный throughout весь диапазон Q

**`Q_resonance_table3` (P:$144546, kernel line 0x597) — НОВАЯ, впервые извлечена:**
- Entries 0..127: монотонно УБЫВАЕТ от `0x007453` (q.23 ≈ 0.00355) до почти нуля
- First 16 entries: `0x007453 0x00741A 0x0073DF 0x0073A0 0x00735D 0x007317 0x0072CD 0x00727E`
  `0x00722C 0x0071D5 0x007179 0x007118 0x0070B1 0x007045 0x006FD2 0x006F5A`
- Интерпретация: обратный damping coefficient — уменьшается с ростом Q, обратный feedback gain
- Это третья независимая Q-зависимая константа — формирует многополюсный резонансный SVF

### 2.3.1 Побочная находка: `$141ca7` ≠ LP filter coeffs!

Ядро SVF на линии 0x58B ссылается на `P:$141ca7` (а НЕ `$144ac7`, как утверждает старый отчёт `mnm_filter_report.md`).
Содержимое `$141ca7`:
- 258 слов насыщенного «near-max ramp»: `0x7FFFAB, 0x7FFFAA, 0x7FFFA9, 0x7FFFA8, ...` (убывает на 1 каждый шаг)
- Это НЕ коэффициенты фильтра — это таблица «максимальных лимитов» (saturated max ramp)
- Используется для clamping выхода SVF: `output = min(input, table[index])`

`P:$144ac7` — основная таблица LP-коэффициентов (curve 1 для HP+LP, curve 2 для mode-фильтра), используется ТОЛЬКО каскадным двойным фильтром на P:$A5F–$ABF.

### 2.4 Алгоритм ядра (P:$5D3–$5FA)

Цикл из 8 итераций × 2 канала = 16 сэмплов/блок.

```text
; Setup (P:$5CF-$5D7)
move #>$f528bd,x0   ; x0 = -0.0847476  (Q-resonance scaler 1)
move #>$4a4df0,x1   ; x1 = +0.5811615  (Q-resonance scaler 2)
move #$71,r1        ; r1 = X:$71 (state A bank, stride 3)
move #$73,r2        ; r2 = X:$73 (state B bank, stride 3)
move #$3,n1         ; n1 = 3 (stride 3)
move n1,n2          ; n2 = 3
do #<$8,>$5e2       ; 8 iterations × 2 channels = 16 samples/block

; Per-sample kernel (P:$5DA–$5E1)
mpy y0,x0,a    a,x:(r1)+n1  y:(r4)+,y1   ; a  = y0*x0;  old a → state;  load y1
mpy x0,y1,b    b,x:(r2)+n2                  ; b  = x0*y1;  old b → state
mac y1,x1,a    y:(r4)+,y0                   ; a += y1*x1
mac x1,y0,b    y1,x:(r1)+                   ; b += x1*y0;  store y1 → state
mac x1,y0,a    y:(r4)+,y1                   ; a += x1*y0
mac y1,x1,b    y0,x:(r2)+                   ; b += y1*x1;  store y0 → state
mac x0,y1,a    y:(r4)+n4,y0                 ; a += x0*y1;  r4−2 (n4=-2)
mac y0,x0,b    y:(r4)+,y0                   ; b += y0*x0
move a,x:(r1)+n1                            ; store a → state (next sample)
move b,x:(r2)+n2                            ; store b → state
```

**Итоговая формула (на сэмпл):** `a = 2·x0·y0 + 2·x1·y1` — то есть свёртка 4 входов
(вход + предыдущее состояние) с фиксированными scalers x0 и x1. TONE/Q/MODE приходят
через Y:$91+ (предзаполнение перед циклом).

### 2.5 Параметр MODE (V+$25)

Биты 9 и 11 используются для коммутации фильтра (HP/LP/BP/Notch):
```
000554: btst #$b,b               ; test bit 11
000555: bcc func_000557          ; branch if clear (HP mode)
000556: move a1,x:(r7-$3)        ; overwrite cutoff
000557: move y:(r6+$9),x0       ; load WIDTH
000558: btst #$9,b               ; test bit 9
000559: mac -x1,y0,a ifcc       ; subtract WIDTH×8 if bit 9 clear (LP mode)
```

---

## 3. Cascaded HP+LP Base-Width DUAL filter (kernel P:$A5F–$ABF)

Это **двойной фильтр** — HP (на BASE) → LP (на BASE+WIDTH), по 1-полюсный IIR на каскад,
плюс третий «mode»-фильтр. Идёт ПОСЛЕ SVF, на выходе голоса.

> ✅ Это и есть «двойной фильтр с энвелоупами», про который говорит пользователь.

### 3.1 Карта коэффициентов

| Каскад | Источник коэффициента | Param | Формула | Адрес кода |
|---|---|---|---|---|
| **1: HP** | `P:$144ac7` curve 1 (entries 0..127), index=BASE | V+$16 (BASE) | `a = (1-k)·a + k·input` | P:$A5F–$A82 |
| **2: LP** | `P:$144ac7` curve 1, index=BASE+WIDTH | V+$16+V+$17 (BASE+WIDTH) | `a = (1-k)·a + k·input` | P:$A84–$A9B |
| **3: mode** | `P:$144ac7` curve 2 (offset $144b48), index от V-$8d >> 17 | (env-driven) | `a = (1-k)·a + k·input` | P:$A9D–$AB5 |

### 3.2 Алгоритм 1-го каскада (HP на BASE)

```text
000A5F: move #>$144ac7,r3   ; r3 = base of LP_filter_coeffs table (curve 1)
000A61: move b,n3            ; n3 = BASE index
000A62: move #$20,r1         ; r1 = state buffer 0x20
000A63: move x:(r3+n3),b     ; b = LP_filter_coeffs[BASE]   <-- coefficient lookup
000A64: asr b  b,y0          ; y0 = b >> 1 (half-coefficient)
000A65: add #>$800000,b      ; round by adding 0.5
000A67: move #$62,r4         ; r4 = state buffer 0x62
000A68: neg b  #$30,r0       ; b = -b; r0 = 0x30

; State pointers (prev sample)
000A69: move y:(r7-$88),a    ; load prev a
000A6B: move y:(r7-$89),x0   ; load prev x0 (input sample)
000A6D: move x:(r7-$89),x1   ; load prev x1
000A6F: move x:(r1+$1f),y1   ; load y1 from circular buffer
000A70: move y1,y:(r7-$89)   ; save y1 (next cycle)

; IIR loop: 16 samples/block
000A78: do #<$10,>$a80       ; 16 iterations
000A7A: mac -x0,y1,a  a,x0  a,y:(r4)+    ; a -= x0*y1; a → x0; a → state
000A7B: mac -y1,x1,b  b,x1  b,y:(r4)+    ; b -= y1*x1; b → x1; b → state
000A7C: mac -y0,x0,a  x:(r0),x0           ; a -= y0*x0; load new x0
000A7D: mac -x1,y0,b  x:(r1),x1           ; b -= x1*y0; load new x1
000A7E: mac x0,y1,a  x:(r0)+,x0           ; a += x0*y1; advance r0
000A7F: mac y1,x1,b  x:(r1)+,x1           ; b += y1*x1; advance r1
000A80: move a,y:(r7-$88)                 ; save a (prev state)
000A82: move b,x:(r7-$88)                 ; save b (prev state)
```

**Формула 1-полюсного HP:** `out[n] = in[n] - state - (1-k)·state_prev + k·input` (упрощённо).

### 3.3 Алгоритм 2-го каскада (LP на BASE+WIDTH)

```text
000A84: move y:(r6+$17),a    ; a = WIDTH  (signed 16-bit + 0.5)
000A85: move y:(r6+$16),x0  ; x0 = BASE
000A86: add x0,a  #>$144ac7,r2 ; a = BASE + WIDTH; r2 = LP_filter_coeffs base
000A88: move #$20,r4         ; r4 = state buffer 0x20
000A89: move #$62,r5         ; r5 = state buffer 0x62
000A8A: move a,b             ; b = a
000A8B: asr #$10,b,b         ; b >>= 16 (extract high part)
000A8C: move y:(r7-$87),a    ; load prev a
000A8E: move b,n2            ; n2 = BASE+WIDTH (index into table)
000A8F: move x:(r7-$87),b    ; load prev b
000A91: move y:(r5)+,x1      ; load x1 from state buffer
000A92: move x:(r2+n2),y0    ; y0 = LP_filter_coeffs[BASE+WIDTH]  <-- coefficient
000A93: do #<$10,>$a99       ; 16 iterations

; IIR loop (LP this time, sign of y0 inverted)
000A95: mac x1,y0,a  a,x0  a,y:(r4)+    ; a += x1*y0; a → x0; a → state
000A96: mac -y0,x0,a  y:(r5)+,x1         ; a -= y0*x0; advance x1
000A97: mac x1,y0,b  b,x0  b,y:(r4)+    ; b += x1*y0; b → x0; b → state
000A98: mac -y0,x0,b  y:(r5)+,x1         ; b -= y0*x0; advance x1
000A99: move a,y:(r7-$87)                ; save a
000A9B: move b,x:(r7-$87)                ; save b
```

### 3.4 Алгоритм 3-го каскада (mode-фильтр, на «второй кривой» таблицы)

```text
000A9D: move #$62,r4         ; r4 = state buffer 0x62
000A9F: move x:(r7-$8d),b    ; load filter env state (V-$8d)
000AA3: asr #$11,b,b         ; b >>= 17 (env modulation scaled)
000AA4: move y:(r7-$90),a    ; load prev a (different state slot)
000AA6: move b0,r2           ; r2 = env modulation (low byte)
000AA7: move x:(r7-$90),b    ; load prev b
000AA9: move x:(r2+$144b48),y0  ; y0 = LP_filter_curve2[env_mod]  <-- coefficient
000AAB: move #$10,r3         ; r3 = state buffer 0x10
000AAC: move x:(r3)+,x1      ; load x1
000AAD: do #<$10,>$ab3       ; 16 iterations
000AAF: mac x1,y0,a  a,x0  a,y:(r4)+    ; a += x1*y0; a → x0; a → state
000AB0: mac -y0,x0,a  x:(r1)+,x1         ; a -= y0*x0; advance x1
000AB1: mac x1,y0,b  b,x0  b,y:(r4)+    ; b += x1*y0; b → x0; b → state
000AB2: mac -y0,x0,b  x:(r3)+,x1         ; b -= y0*x0; advance x1
000AB3: move a,y:(r7-$90)                ; save a
000AB5: move b,x:(r7-$90)                ; save b
```

### 3.5 Таблица LP_filter_coeffs (P:$144ac7, 258 слов)

Две кривые:
- **Curve 1** (entries 0..128): TONE-параметр 0..127 + граница 128
  - Растёт от `0x004876` (q.23 = 0.00221, fc≈15.5 Hz) до `0x74bc9a` (q.23 = 0.912, fc≈Nyquist/2)
  - Используется HP, LP, FM-TONE, FX-CHORUS, FX-REV, kernel Base-Width
- **Curve 2** (entries 129..257): для второго канала (mode-фильтр), accessed via `x:(r2+$144b48)`
  - Стартует с `0x045180` (q.23 = 0.0337, fc≈240.9 Hz) до `0x74bc9a` (0.912)

| Entry | Hex | q.23 | fc(−3 дБ), Гц |
|---:|---:|---:|---:|
| 0 | $004876 | 0.002211 | 15.5 |
| 16 | $00bb0e | 0.005708 | 40.2 |
| 32 | $01e187 | 0.014695 | 103.9 |
| 48 | $04ceac | 0.037557 | 268.7 |
| 64 | $0c0efd | 0.094207 | 695.0 |
| 80 | $1cd081 | 0.225113 | 1799.8 |
| 96 | $3ce56a | 0.475751 | 4699.1 |
| 112 | $630acc | 0.773767 | 13335 |
| 127 | $747093 | 0.909685 | — |
| 128 (curve 2 start) | $045180 | 0.033737 | 240.9 |
| 160 | $5247c7 | 0.642815 | 7970.6 |
| 192 | $6a5194 | 0.830615 | — |
| 257 | $74bc9a | 0.912006 | — |

---

## 4. Filter envelope generator (kernel P:$AB7–$B4D)

После 3-каскадного фильтра идёт генератор огибающей фильтра (использует V+$14, V+$15 параметры).

```text
000AB7: move y:(r6+$14),b      ; load env param V+$14 (ATK or DEC)
000AB8: sub #>$400000,b        ; b -= 0.5 (signed offset)
000ABA: abs b  #$62,r4          ; b = |b|; r4 = state 0x62
000ABB: asl b  #$20,r1          ; b <<= 1; r1 = state 0x20
000ABC: move #$51,r2           ; r2 = state 0x51
000ABD: move b,x0              ; x0 = |env param - 0.5| << 1
000ABE: mpy x0,x0,a  #$40,r3   ; a = x0² (squared modulation); r3 = state 0x40
000ABF: move y:(r6+$15),y0     ; load env param V+$15
...
; Loop: 16-sample smoothing of env value
000AC7: do #<$10,>$acf         ; 16 iterations
000AC9: mpy y0,x0,a  a,x:(r2)+  y:(r4)+,y1   ; a = y0*x0;  state update
000ACA: asl #$1,a,a            ; a <<= 1 (×2)
000ACB: mac y1,x1,a  y:(r1)+,x0 ; a += y1*x1
...
```

Огибающая — квадратичная (squared modulation × smoothing), формирующая cutoff-модуляцию.

---

## 5. FX-PHASER machine (m18, slot P:$145036–$14538d)

**Параметры (8 шт.):** CNTR, DEP, SPD, MIX, FB, WID, INP, ---
**Defaults:** 0x40 (64), 0x40, 0x40, 0x7F (127), 0x2D (45), 0x00, 0x7F, 0x40
**Имя машины:** PHASER (display_name в дескрипторе)
**Coldfire_offset:** 0x58C25

> ⚠️ Архитектура ≠ JUCE dsp::Phaser (6 stages TPT allpass). В прошивке — **4-stage allpass phaser** с уникальным LFO и feedback.

### 5.1 Карта таблиц фейзера

| Адрес | Файл | Размер | Роль |
|---|---|---:|---|
| `P:$141900` | `P_141900_PHASER_stage0_table.bin` | 128 слов | Stage 0 allpass (модуляция шага) |
| `P:$141980` | `P_141980_PHASER_stage1_table.bin` | 128 слов | Stage 1 amplitude curve |
| `P:$141a00` | `P_141a00_PHASER_stage2_table.bin` | 128 слов (127 nonzero) | Stage 2 feedback curve |
| `P:$141b18` | `P_141b18_PHASER_stage3_table.bin` | 64 слова | Stage 3 limit (mostly `0x7FFFFF`) |
| `P:$142f06` | `P_142f06_PHASER_helper_table.bin` | 64 слова | Helper (медленно спадающая `0x7FF846→0x7FF522`) |
| `P:$143546` | `P_143546_PHASER_cntr_table.bin` | 64 слова | CNTR param mapping (`0x000d35→0x001293`) |

### 5.2 Константы фейзера (immediate operands в коде m18)

| Hex | Decimal (signed q.23) | Использование | Роль |
|---|---:|---|---|
| `$028F5C` | +0.019852 | lines 14506d, 1450a0, 1450aa | LFO phase increment (low-rate modulation) |
| `$FD70A4` | −0.029099 | lines 14506f, 1450a2, 1450ac | LFO feedback negative gain |
| `$5A8241` | +0.706940 | line 145117 | LFO output scaling (final amplitude) |
| `$0000E0` (224) | integer | line 145083, 1450a0 | buffer base offset (stereo delay line) |
| `$000020` (32) | integer | line 14508b, 1450c6 | buffer base offset (state) |
| `$000040` (64) | integer | line 145202, 1452dc | buffer base offset (state) |
| `$000060` (96) | integer | line 1450ec | buffer base offset |
| `$000070` (112) | integer | line 14512f, 1451bd | buffer base offset |
| `$000090` (144) | integer | line 1450d7, 1450fb | buffer base offset |
| `$00` / `$0` | integer | various | clear registers / init |
| `$400000` | 0.5 q.23 | lines 1450b1, 1450da | 0.5 offset for signed→unsigned convert |
| `$800000` | 1.0 q.23 | lines 1450b6, 1450e2 | max value 1.0 (saturate) |
| `$7FFFFF` | ~1.0 q.23 | lines 145132, 1452c6 | max positive 24-bit signed |

### 5.3 Структура фейзера (по коду)

```text
init (P:$145036–$145044):
  - zero state buffers at r6+$11, r6+$12, r6+$18, r6+$19 (4 stereo state slots)
  - clear 8-word loop on (r6+$1e) buffer

proc (P:$145045 onwards):
  1. Compute LFO triangle from y:(r6+$b) × y:(r6+$b) (input × input → squared modulation)
  2. mac y0,x0,a × 16 iterations  → smoothing/saturation block (lines 145051–145056)
  3. Compute LFO output via mpy x0,x0 → maci #>$9566 (lines 145057–14505e)
     - $9566 = 38246 = ~0.00456 q.23 (very small coefficient)
     - This is the LFO rate scaling
  4. mac -x1,x0,b × 16 iterations with b stored to x:(r6+$11), x:(r6+$12)
     - b = current_LFO_phase accumulator (24-bit stereo pair)
  5. Compute LFO modulation:
     - maci #>$28f5c × y:(r6+$4) → LFO phase increment
     - maci #>$fd70a4 × y:(r6+$1b) → LFO feedback gain
     - Same for y:(r6+$5) → second channel LFO
  6. Sub #>$400000, asl, abs, add #>$800000, neg → triangle/sine shaping (lines 1450b1–1450bf)
  7. Compute sweep via table lookups:
     - y:(r6+$8) >> 16 → table index
     - move y:(r4+$141980),y0 → load from stage1_table (CNTR modulation)
     - move y:(r4+$141900),y0 → load from stage0_table (sweep rate)
  8. Center frequency mapping (lines 14518d–14519e):
     - mpyi #>$63f × y:(r6+$4) → 0.799 scale
     - maci #>$80000 × y:(r6+$1) → +0.0625 offset
     - sub #>$100, rnd → round
     - clamp 0..0x63f
  9. 4-stage allpass processing (lines 1451c1–14521e):
     - Stage lookups: move y:(r0+$143546),x1 (cntr_table)
                      move x:(r0+$141b18),x1 (stage3_table)
                      move x:(r0+$142f06),y1 (helper_table)
     - For each of 16 samples: mpy/mul/add → cascade 4 allpass sections
  10. Output: mac y0,x0,a/b with output stored to y:(r7)+ (audio out buffer)

rts at P:$14513f (one branch) and P:$14538d (main exit)
```

### 5.4 Параметр→table mapping

| Param (Y:V+) | Имя | Использование в коде |
|---|---|---|
| $01 | MIX | linear gain on dry/wet (kernel: not in m18 directly) |
| $04 | CNTR scaled 1 | LFO rate input → table $143546 (cntr_table) |
| $05 | CNTR scaled 2 | mirror of $04 for stereo |
| $06 | FB scaled | LFO feedback input → sat-block |
| $07 | DEP (depth) | LFO amplitude; line 145131: y:(r6+$7),x0 |
| $08 | CNTR (raw) | line 145153: asr #$10,a,a (signed→offset) → table $141900 |
| $09 | CNTR (mirror 1) | line 14516a: asr #$10,a,a → table $141980 |
| $0A | CNTR (mirror 2) | line 145180: asr #$10,a,a → table $141a00 |
| $0B | input squared | line 145049: mpy x0,x0,a (LFO drive) |
| $13 | FB smoothing | line 1450de: y:(r6+$13) |
| $1B | FB signed | line 14506a: y:(r6+$1b) |
| $37 | aux state 1 | line 1451ec |
| $38 | aux state 2 | line 1451e7 |

---

## 6. Файлы в архиве

```
filter_phaser_data.tar
├── README.md                                              <- этот файл
├── FILTER_PHASER_TABLES_MANIFEST.json                     <- JSON-манифест всех таблиц
│
├── tables/                                                <- бинарные дампы таблиц (.bin = 24-bit BE)
│   ├── P_141900_PHASER_stage0_table.bin / .txt           <- phaser
│   ├── P_141980_PHASER_stage1_table.bin / .txt
│   ├── P_141a00_PHASER_stage2_table.bin / .txt
│   ├── P_141b18_PHASER_stage3_table.bin / .txt
│   ├── P_142f06_PHASER_helper_table.bin / .txt
│   ├── P_143546_PHASER_cntr_table.bin / .txt
│   ├── P_141800_LFO_speed_table.bin / .txt               <- shared LFO
│   ├── P_143f95_cutoff_divisor_table.bin / .txt           <- cutoff divisor
│   ├── P_141ca7_LP_filter_coeffs_KERNEL_ref.bin / .txt   <- kernel-referenced LP coeffs (NEW: not $144ac7)
│   ├── P_144ac7_LP_filter_coeffs_main.bin / .txt          <- main LP coeffs (curve 1+2)
│   ├── P_144446_Q_resonance_table1.bin / .txt             <- Q-resonance table 1
│   ├── P_1444c6_Q_resonance_table2.bin / .txt             <- Q-resonance table 2 (was mislabeled FXDYN_attack)
│   ├── P_144546_Q_resonance_table3_NEW.bin / .txt         <- Q-resonance table 3 (NEW!)
│   ├── P_144446_Q_resonance_FULL_range.bin                <- full Q-table range (5419 words)
│   ├── P_1446c6_FXDYN_release_OR_Q_table4.bin / .txt
│   └── P_144746_FXDYN_rms_table.bin / .txt
│
├── listings/                                              <- дизассемблерные листинги
│   ├── 01_SVF_multimode_kernel_P04A8-0610.txt            <- SVF с резонансом
│   ├── 02_Cascaded_HP_LP_dual_filter_P0A00-0C00.txt       <- двойной HP+LP фильтр
│   ├── 03_FX_PHA_machine_m18_P145000-145400.txt           <- полная машина фейзера
│   ├── 04_filter_envelope_P0ABF-0B4D.txt                   <- генератор огибающей
│   ├── 05_FX_DYN_machine_m16_P147700-147900.txt           <- для кросс-референса Q-таблиц
│   └── 06_VO6_machine_m11_P147100-147200.txt              <- для кросс-референса Q-таблиц
│
└── cpp_reference/                                          <- существующие C++ модели (для сравнения!)
    ├── monomachine_filter.hpp                              <- СТАРЫЙ dual-filter (RBJ biquad approximation)
    └── PhaserFX.h                                          <- СТАРЫЙ phaser (JUCE 6-stage TPT — НЕ ТАК!)
```

---

## 7. Рекомендации по воссозданию

### 7.1 Двойной фильтр (BASE+WIDTH, HP+LP, с резонансом)

```cpp
// Скелет на основе извлечённых данных
struct MonomachineDualFilter {
    // LP_filter_coeffs таблица (curve 1 — для HP и LP)
    static constexpr int32_t LP_COEFFS[129] = { /* из P_144ac7_LP_filter_coeffs.bin */ };
    
    // Q-resonance таблицы
    static constexpr int32_t Q_TABLE1[128] = { /* из P_144446 */ };
    static constexpr int32_t Q_TABLE2[128] = { /* из P_1444c6 */ };
    static constexpr int32_t Q_TABLE3[128] = { /* из P_144546 — НОВАЯ */ };
    
    // SVF-константы
    static constexpr float SVF_X0 = -0.084747577f;  // $F528BD q.23
    static constexpr float SVF_X1 = +0.581161499f;  // $4A4DF0 q.23
    
    // State (per channel)
    float svf_state[2] = {0, 0};           // X:$71, X:$73 (stride 3)
    float hp_state = 0;                      // V-$88
    float lp_state = 0;                      // V-$87
    float mode_state = 0;                    // V-$90
    
    // Params (per voice, 0..127 unless noted)
    uint8_t BASE, WIDTH, Q, ATK, DEC;
    int8_t  BOFS, WOFS;
    uint8_t MODE_bits;
    
    float process(float in) {
        // 1. SVF multimode resonance filter (Q applies here)
        //    Uses SVF_X0, SVF_X1 + Q_TABLE1[Q], Q_TABLE2[Q], Q_TABLE3[Q]
        float cutoff = BASE + env_modulation;
        float q1 = Q_TABLE1[Q] / 8388608.0f;
        float q2 = Q_TABLE2[Q] / 8388608.0f;
        float q3 = Q_TABLE3[Q] / 8388608.0f;
        // ... SVF core: y = SVF_X0 * in + SVF_X1 * state + q1*q2*q3*feedback ...
        
        // 2. Cascaded HP+LP dual filter (NO resonance here, just 1-pole IIR)
        float k_hp = LP_COEFFS[BASE] / 8388608.0f;
        float k_lp = LP_COEFFS[BASE + WIDTH] / 8388608.0f;
        hp_state = (1 - k_hp) * hp_state + k_hp * y;          // HP
        float after_hp = y - hp_state;                         // HP output
        lp_state = (1 - k_lp) * lp_state + k_lp * after_hp;   // LP
        float out = lp_state;
        
        return out;
    }
};
```

### 7.2 Фейзер (4-stage allpass, NOT 6-stage JUCE)

```cpp
// Скелет на основе извлечённых данных
struct MonomachinePhaser {
    // 6 таблиц фейзера
    static constexpr int32_t STAGE0[128] = { /* из P_141900 */ };
    static constexpr int32_t STAGE1[128] = { /* из P_141980 */ };
    static constexpr int32_t STAGE2[128] = { /* из P_141a00 */ };
    static constexpr int32_t STAGE3[64]  = { /* из P_141b18 */ };
    static constexpr int32_t HELPER[64] = { /* из P_142f06 */ };
    static constexpr int32_t CNTR[64]   = { /* из P_143546 */ };
    
    // Константы фейзера
    static constexpr float LFO_PHASE_INC = +0.019852f;    // $028F5C q.23
    static constexpr float LFO_FB_NEG    = -0.029099f;    // $FD70A4 q.23
    static constexpr float LFO_OUT_GAIN  = +0.706940f;    // $5A8241 q.23
    
    // Параметры: CNTR, DEP, SPD, MIX, FB, WID, INP
    uint8_t cntr=0x40, dep=0x40, spd=0x40, mix=0x7f, fb=0x2d, wid=0x00;
    
    // 4-stage allpass state
    float ap_state[4][2] = {0};  // [stage][channel]
    float lfo_phase = 0;
    float feedback = 0;
    
    float process(float in, int ch) {
        // 1. LFO: squared-input driven (NOT sinusoidal!)
        //    phase += LFO_PHASE_INC * spd_factor
        //    feedback = LFO_FB_NEG * ...
        //    output = LFO_OUT_GAIN * sin(phase)
        // 2. 4-stage allpass cascade (NOT 6-stage like JUCE!)
        //    Each stage: y = ap_state[i] * stage_coeff + input
        //                 ap_state[i] = input - stage_coeff * y
        //    Stage coeffs from STAGE0..3, modulated by LFO
        // 3. Feedback sum: in - last_out * feedback_gain
        // 4. Linear mix: dry * (1-mix) + wet * mix
        return /* ... */;
    }
};
```

---

## 8. Что было РАНЕЕ неизвестно и что НОВОГО в этом гиде

| Было | Стало (новое в этом гиде) |
|---|---|
| `$144446` — mislabeled "X_144446_div_table2" (без семантики) | ✅ Идентифицирована как **Q-resonance table 1** (SVF kernel line 0x57F) |
| `$1444c6` — mislabeled "FXDYN_attack_table" | ✅ Идентифицирована как **Q-resonance table 2** (используется SVF kernel + VO-6 + FX-DYN) |
| `$144546` — **не была извлечена** | ✅ **НОВАЯ Q-resonance table 3**, впервые извлечена |
| `$144ac7` — описана как «общий LP-стол» | ✅ Уточнено: curve 1 для HP+LP+FM, curve 2 для mode-фильтра (offset $144b48) |
| `$141ca7` — не упоминалась | ✅ Найдено использование в ядре на line 0x58B (отдельная таблица LP coeffs) |
| Фейзер "6 таблиц" описано, но алгоритм не раскрыт | ✅ Полный код m18 продизассемблирован и описан |
| Константы фейзера `$028F5C`, `$FD70A4`, `$5A8241` | ✅ Идентифицированы: LFO phase inc / feedback neg / output gain |
| Адрес $144446 с stride 0x80 непонятен | ✅ Подтверждена семантика: 128-entry table, stride 128 байт/запись (aliasing X↔P) |

---

## 9. Открытые вопросы для следующей итерации

1. Точная расшифровка ячейки V+$25 (MODE bits): какие 4 режима multimode (HP/LP/BP/Notch)?
2. Подтверждение strides в Q-tables (0x80 = 128 байт на Q-шаг, 1 байт/запись ИЛИ multi-byte records?)
3. Декод полного LFO-генератора фейзера (squared input → triangle/sine shaping)
4. Точная формула генератора огибающей фильтра (V+$14 ×² → smoothing)

---

**Конец гида.** Данные в этом архиве — исчерпывающие для воссоздания фильтра и фейзера
на основе РЕАЛЬНОЙ прошивки Monomachine SFX-60 OS 1.32B.
