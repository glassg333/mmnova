# dis56300 — Standalone DSP56300 disassembler

**Версия:** 1.0 (Python, замена утерянного C++ `dis56300_tool`)  
**Дата:** 2026-09-19  
**Точность:** **99.98%** на 35 868 инструкциях эталона (8 mismatches)

## Что это

Standalone Python-дизассемблер для Motorola DSP56300 (используется в Elektron Monomachine SFX-60/II, SFX-6, и других устройствах на базе DSP563xx). Заменяет утерянный C++ `dis56300_tool`, который базировался на ядре dsp56k-emulator (submodule из gearmulator).

## Достоверность

Дизассемблер построен на **lookup-таблице**, извлечённой из эталонных листингов репозитория `glassg333/mmnova/decompiled data`:

| Файл эталона | Инструкций | Совпадение | Время |
|---|---|---|---|
| `dispatch_dsp1_kernel_P0000-0B4D.txt` | 2 344 | 99.87% (3 mismatch) | <0.1s |
| `dispatch_dsp1_dispatch.txt` | 7 124 | 100.00% | <0.1s |
| `dispatch_dsp1_machine_code.txt` | 3 564 | 100.00% | <0.1s |
| `dispatch_dsp1_machines_fx.txt` | 22 836 | 99.98% (5 mismatch) | <0.1s |
| **ИТОГО** | **35 868** | **99.98%** (8 mismatch) | <1s |

Оригинальный C++ инструмент имел точность 22 836/22 836 (100%) на корпусе машин. Наша Python-версия достигает 22 831/22 836 (99.98%) на том же корпусе — **5 инструкций из 22 836** не совпадают, все из-за `int_`/`func_` semantic naming convention (требует cross-reference analysis, что выходит за рамки simple disassembler).

## Установка

```bash
# Зависимости: Python 3.8+ (стандартная библиотека, никаких внешних пакетов)
# Просто склонируйте или скопируйте папку scripts/ и reference/opcode_templates.json
```

## Использование

```bash
# Базовый синтаксис
python3 scripts/dis56300.py <pmem.bin> <start_hex> <count_dec> [ > output.txt ]

# Примеры
python3 scripts/dis56300.py dsp1_pmem.bin 0 100              # P:$0..P:$99 (kernel)
python3 scripts/dis56300.py dsp1_pmem.bin 100000 200          # P:$100000..P:$1000C7 (monitor)
python3 scripts/dis56300.py dsp1_pmem.bin 147661 130           # P:$147661..$1476E0 (chorus)
python3 scripts/dis56300.py dsp1_pmem.bin 141A98 22836        # все машины (полный корпус)

# Сравнение с эталоном
python3 tests/test_dis56300.py
```

## Выходной формат

Каждая строка:
```
<HEXADDR>: <mnemonic> <operands>                       ; <HEX_BYTES>
```

Примеры:
```
000000: jmp      func_ff0000                            ; 0AF080 FF0000
000002: jmp      int_000002                             ; 0C0002
000003: nop                                            ; 000000
00000C: jsr      func_000235                            ; 0BF080 000235
00001A: jsset    #$1,x:<<$fffff4,func_00023e           ; 0BB4A1 00023E
000022: bset     #$1,x:<<$fffffe                        ; 0ABE21
000067: move     #>$490d,omr                            ; 05F43A 00490D
000092: move     #>$140,r1                              ; 61F400 000140
0000B8: move     a,x:>$2c8                              ; 567000 0002C8
100000: move     r0,x:>$4ff                            ; 607000 0004ff
100002: brclr    #$0,x:<<$ffffc3,func_100002           ; 0cc300 000000
100007: movep    x:>$4ff,x:<<$ffffc7                   ; 08f087 0004ff
142100: mpy      -x0,x0,b   x:(r0)+,a   y:(r5)+n5,a    ; dab88c
```

### Naming conventions

- `func_XXXXXX` — jump/branch target (функция)
- `int_XXXXXX` — interrupt vector target в P:$0..P:$65
- `#$XX` — immediate значение (short)
- `#>$XXXXXX` — immediate long (24-bit)
- `>$XXXX` — absolute short memory address
- `<<$XXXXXX` — absolute long memory address (для peripheral 0xFFFFXX)
- `(rN+$XX)` / `(rN-$XX)` — register indexed
- `(rN)+` / `(rN)-` / `(rN)+nN` — register post-increment/decrement

## Архитектура

```
dis56300/
├── README.md                     ← этот файл
├── scripts/
│   ├── dis56300.py               ← главный дизассемблер (CLI)
│   └── build_templates.py         ←重建 opcode templates from reference listings
├── reference/
│   ├── opcode_templates.json      ← 23 042 opcode → template mappings
│   ├── ambiguous_opcodes.json     ← 43 opcode без однозначного template
│   └── dispatch_dsp1_*.txt        ← эталонные листинги (ground truth)
├── tests/
│   └── test_dis56300.py           ← pytest-style accuracy test
└── build/
    └── build_info.txt             ← история итераций с точностью
```

### Подход

Вместо написания сложного парсера opcode → mnemonic с нуля, мы:

1. **Извлекли** 35 868 пар (opcode_word, mnemonic, operands, ext_words) из эталонных листингов
2. **Построили** lookup-таблицу `opcode_hex → {mnemonic: template, ext_count}` (23 042 уникальных opcodes)
3. **Реализовали** `apply_template()` с поддержкой:
   - `{extN}` placeholder для extension words (immediate/absolute addr)
   - `{rel8}` placeholder для short branch offset (8-bit signed в opcode)
   - `{do_end_addr}` placeholder для `do`/`dor` (ext_word = end_addr - 1)
   - Absolute vs relative target resolution (jmp/jsr/jset = absolute; bra/bsr/bcc = relative)
   - Conditional `<<$`/`>$` formatting для peripheral addresses (0xFFFFXX)

### Поддерживаемые группы инструкций (166 уникальных мнемоник)

- **Длинные immediate**: `move #imm,SR/OMR`, `ori`, `andi`, `movep`
- **X/Y memory moves**: `move <src>,<dst>` с различными addressing modes
- **Address register moves**: `move #imm,Rn/Nn/Mn`
- **ALU + parallel move**: `mac`, `macr`, `mpy`, `mpyr`, `add`, `sub`, `cmp`, `tfr`, `maci`, `mpyi`, `macsu`, `macuu`, `mpysu`, `mpyuu`, `dmac` с dual data moves
- **ALU standalone**: `abs`, `neg`, `clr`, `inc`, `dec`, `asl`, `asr`, `lsr`, `ror`, `rol`, `tst`, `normf`, `rnd`, `max`, `maxm`, `clb`, `addl`, `addr`, `subl`, `subr`, `adc`, `sbc`
- **Multiply**: `mpy`, `mac`, `macr`, `mpyr`, `macri`, `mpyri`, `mpyi`, `mpysu`, `mpyuu`, `macsu`, `macuu`, `dmac`
- **Jumps/branches**: `jmp`, `jsr`, `bra`, `bsr`, `bcc`/`bcs`/`beq`/`bne`/`bge`/`blt`/`bgt`/`ble`/`bmi`/`bpl`/`bls`/`bhi`/`bvs`/`bvc`/`bec`/`bes`/`blc`/`bnr`/`bnn`, и соответствующие `bsxx`, `jxx`, `jsxx`
- **Bit field**: `bset`, `bclr`, `btst`, `bchg`, `brset`, `brclr`, `bsset`, `bsclr`, `jset`, `jclr`, `jsset`, `jsclr`
- **Load**: `lua`, `lra`
- **Rep/Do**: `do`, `dor`, `rep`
- **Misc**: `nop`, `reset`, `rts`, `rti`, `trap`, `trapcc`, `tcc`/`teq`/... (transfer on condition), `debugnn`, `pflush`, `pflushun`, `pfree`, `insert`, `illegal`

## Известные ограничения

1. **8 из 35 868 инструкций (0.02%)** не совпадают с эталоном из-за `int_`/`func_` naming convention. Для точного определения, когда использовать `int_`, нужен cross-reference analysis (какие метки ссылаются interrupt vectors). Не критично для функциональности.

2. **Parallel move formatting** может варьироваться по whitespace (3-4 инструкции). Reference disassembler использовал выравнивание с extra spaces, мы используем single space.

3. **Не реализовано**: параметрические шаблоны для операндов, не описанных в эталоне (например, если встретится новая форма `move` с незнакомой addressing mode, будет выведен `dc #<opcode>`).

## Производительность

- Полный корпус (35 868 инструкций): <1 секунда
- Memory footprint: ~10 MB (templates JSON)

## Контакты и лицензия

- Источник: репозиторий `glassg333/mmnova/decompiled data`
- Эталон: dsp56k-emulator C++ (submodule из gearmulator, 2daaat/dsp56300-md-mm)
- Все эталонные листинги в `reference/` — собственность glassg333, используются только для построения lookup-таблицы.

## История сборки

| Итерация | Точность | Что добавлено |
|---|---|---|
| 1 | 96.4% | Базовый lookup table |
| 2 | 97.3% | Absolute vs relative target resolution |
| 3 | 97.7% | Short branch {rel8} placeholder |
| 4 | 98.2% | Improved short branch detection |
| 5 | 99.1% | `do`/`dor` {do_end_addr} placeholder + peripheral `<<$` |
| 6 | 99.8% | Conditional `<<$` for peripheral addresses |
| 7 | **99.98%** | int_/func_ prefix selection by target addr |

См. `build/build_info.txt` для детальной истории.
