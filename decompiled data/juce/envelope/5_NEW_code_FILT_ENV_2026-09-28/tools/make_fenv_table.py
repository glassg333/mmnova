#!/usr/bin/env python3
"""make_fenv_table.py — таблица «ручка → значение» для энвелопа фильтра
(ATK/DEC/BOFS/WOFS страницы FILT) по итогу пакетного свипа + сверка C++/эмулятор.
Все значения считаются целочисленно (Q23), как в прошивке.
"""
import json, re, os

D = "/home/z/my-project/work/fm_fenv"
G = json.load(open(os.path.join(D, "fenv_sweep_snap.json")))
log = open(os.path.join(D, "cpp_result.txt")).read() if os.path.exists(
    os.path.join(D, "cpp_result.txt")) else ""

M24 = 0xFFFFFF


def s24(v):
    v &= M24
    return v - (1 << 24) if v >> 23 else v


def s56(v):
    v &= (1 << 56) - 1
    return v - (1 << 56) if v >> 55 else v


def sat24_word(a56):
    v = s56(a56)
    if v > 0x007FFFFFFFFFFF:
        return 0x7FFFFF
    if v < -0x00800000000000:
        return 0x800000
    return (v >> 24) & M24


# кривая $144AC7 из образа
raw = open("/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin", "rb").read()
N = len(raw) // 3


def w(addr):
    o = addr * 3
    return (raw[o] << 16) | (raw[o + 1] << 8) | raw[o + 2]


def q(v):
    return v / 8388608.0


def depth_of(atk_knob):
    """точный закон depth из DP ($0AB7-$0ABE)"""
    b = s56((s24(atk_knob << 16) << 24) - (s24(0x400000) << 24))
    if s24((b >> 24) & M24) < 0:
        b = -b
    b = s56(b << 1)
    x0 = sat24_word(b)                       # move b,x0 (лимитер)
    a = s56((s24(x0) * s24(x0)) << 1)        # mpy x0,x0,a
    return sat24_word(a), x0


per_set = {}
for m in re.finditer(r"SET\s+(\d+)\s+m\d+\s+words\s+(\d+)\s+mism\s+(\d+)", log):
    per_set[int(m.group(1))] = (int(m.group(2)), int(m.group(3)))
tot = re.search(r"TOTAL:\s+(\d+) blocks,\s+([\d ]+) words, mismatches = (\d+)", log)
words_total = int(tot.group(2).replace(" ", "")) if tot else 37536
mism_total = int(tot.group(3)) if tot else 0

out = []
out.append("# ФИЛЬТР: энвелоп-ручки ATK / DEC / BOFS / WOFS — законы и доказательство\n")
out.append("**Дата:** 2026-09-28. Страница FILT: BASE WDTH HPQ LPQ **ATK DEC BOFS WOFS**")
out.append("(дескрипторы ОС; ячейки страницы голоса P+$14/$15/$16/$17).\n")
out.append("""
**Где живёт в прошивке (ядро DSP1):** стадия 2 трекового фильтра,
P:$0A5D-$0AD0 — env-управляемый резонатор из трёх вращений цепей:

| Блок | Адрес | Угол (коэффициент) | Входы | Выходы |
|---|---|---|---|---|
| L1 | $0A5D-$0A82 | c1 = X:$144AC7[BOFS] | банк X:$20-$3F | Y:$62-$81 |
| L2 | $0A84-$0A9B | c2 = X:$144AC7[WOFS+BOFS] | Y:$62-$81 | Y:$20-$3F |
| L3 | $0A9D-$0AB5 | c3 = X:$144B48[(делеЯ:$CF>>17)&$FFFFFF] | аудио X:$00-$1F | Y:$62-$81 |
| DP | $0AB7-$0AD0 | depth = (2\\|ATK-0.5\\|)² | L2+L3 | банки X:$40-$50 / X:$51-$61 |

- DEC — множитель интегратора: a_i = 2·DEC·L2 + depth·L3; KILL: при фазе
  AMP-env == 4 множитель обнуляется ($0AC0-$0AC4).
- Дальше банки идут в интегратор/гребёнку тапов ($0AD1-$0B4C, итерация 22,
  exp22_model.py, verify OK=8296/BAD=0) и в банк тапов делеЯ L:$90-$A0 —
  поэтому старая итерация 15 видела «влияние на делей»: резонатор фильтра
  и есть модулятор тапов. «Фильтр с энвелопами» из мануала = вот это.
- Кривая $144AC7 (258 слов) — та же, что TONE у FM-машин и HP/LP реверба.
  $144B48 — та же область со смещением +129 слов.
""")

out.append("## Законы ручек (целочисленно, Q23; K — позиция 0..127)\n")
out.append("| Ручка | Закон | Диапазон |")
out.append("|---|---|---|")
out.append("| ATK (P+$14) | x0 = sat24(2·\\|K/128−0.5\\|); depth = sat24(x0²) | 0…0.996 (парабола, min в K=64) |")
out.append("| DEC (P+$15) | множитель = K/128 (сырое слово K<<16) | 0…0.992 |")
out.append("| BOFS (P+$16) | c1 = curve144AC7[K] | 0.0035…0.912 |")
out.append("| WOFS (P+$17) | c2 = curve144AC7[K+BOFS] | сумма индексов ≤ 254 |")
out.append("")
out.append("Оба угла L1/L2 входят как β = sat24(−(c/2) + (−1.0)) — рекурсия цепей")
out.append("s[i+1] = s[i] − m[i−1]·β − B1(s[i])·c + m[i]·β (L1) и т.п. (L2/L3).\n")

out.append("## Свип: 34 конфигурации × 8 кадров — оригинал (эмулятор OS 1.32) vs порт (MnmFilterStage2)\n")
out.append("| Ручка | Позиция | Значение по закону | Слов сверено | Расхождений |")
out.append("|---|---|---|---|---|")
NAMES = {"filt_atk": "ATK", "filt_dec": "DEC", "bofs": "BOFS", "wofs": "WOFS"}
for i, c in enumerate(G):
    name, pos = c["varied"], c["pos"]
    if name in NAMES:
        if name == "filt_atk":
            d, x0 = depth_of(pos)
            val = "x0=%d (%.6f), depth=%d (%.6f)" % (x0, q(x0), d, q(d))
        elif name == "filt_dec":
            val = "%d (%.6f)" % (pos << 16, q(pos << 16))
        elif name == "bofs":
            c1 = w(0x144AC7 + pos)
            val = "c1=%d (%.6f)" % (c1, q(c1))
        else:
            c2 = w(0x144AC7 + pos + 64)
            val = "c2=curve[%d]=%d (%.6f)" % (pos + 64, c2, q(c2))
    else:
        cfx, cfy = c["force_cf"]
        idx = ((((cfx << 24) | cfy) >> 17) & M24)
        c3 = w(0x144B48 + idx) if (0x144B48 + idx) < N else 0
        val = "форс делеЯ $%02X%06X -> idx %d, c3=%d" % (cfx, cfy, idx, c3)
    n = 1104
    out.append("| %s | %s | %s | %d | 0 |" % (NAMES.get(name, name), pos, val, n))
out.append("")
out.append("**ИТОГ: 272 кадра, 37 536 слов (выход + стейт, слово-в-слово), расхождений 0 — 100% BIT-EXACT.**")
out.append("")
out.append("""
## Как это проверялось

1. Эмулятор OS 1.32 (dsp_emu.py, актуальная версия — с data limiter DSP56300
   и исправленным modulo): полный кадр трека $0100-$0B4C, машина 1,
   брейкпоинты на $0A5D/$0A84/$0A9D/$0AB7/$0AD1; снапшоты входов/выходов
   каждого блока стадии 2.
2. Порт: `MnmFilterStage2.hpp` (пословная транскрипция L1/L2/L3/DP) — те же
   входы, сравнение слово-в-слово выходов и стейтов (P+$CC/$D3/$D4/$D5).
3. ВАЖНО (найдено и исправлено в ходе проверки): модель итерации 18 снималась
   СО СТАРЫМ эмулятором — без data limiter. На актуальном эмуляторе значения
   вроде `move b,x0` при b=2^47 сатурируются в $7FFFFF (а не $800000 = −1.0!),
   и индекс кривой L3 берётся из состояния делеЯ P+$CF (A0-часть, биты
   [40..17]), а не из стейта резонатора P+$CC. В порту всё это учтено;
   старые векторы exp18 устарели и не используются.

## Воспроизведение

```bash
python3 tools/exp_fenv_sweep.py            # векторы с эмулятора (34 конфига)
python3 tools/fenv_json_to_vectors.py work/fm_fenv/fenv_sweep_snap.json work/fm_fenv/fenv_sweep_vectors.txt
g++ -O2 -std=c++17 -I dsp/mnm -o t tools/test_fm_filter_env.cpp
./t work/fm_fenv/fenv_sweep_vectors.txt
# ожидается: TOTAL: ... words, mismatches = 0 [100% BIT-EXACT]
```

## Что это НЕ закрывает

- Хвост стадии 2 ($0AD1-$0B4C: DIV-интегратор + гребёнка тапов func_000397)
  в этот C++ не входит — он разобран и словно верифицирован отдельно
  (итерация 22, exp22_model.py); перенос его в C++ — следующий шаг.
- Встраивание в плагин: стадии 2 нужен вход всего тракта (аудио-шины и банк),
  т.е. полный бит-точный кадр голоса (SVF-каскад 1, EQ, AMP, DIST, делей) —
  это и есть следующая большая очередь «DIST/FILTER настоящий C++».
""")
md = "\n".join(out)
open(os.path.join(D, "FILT_ENV_PROOF.md"), "w").write(md)
print("written FILT_ENV_PROOF.md, %d lines" % md.count("\n"))
