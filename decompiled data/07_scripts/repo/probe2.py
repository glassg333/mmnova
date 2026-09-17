#!/usr/bin/env python3
# Битовая разведка: выровненные 24-битные паттерны по группам
import pickle, re
from collections import Counter

pairs = pickle.load(open("/home/z/my-project/mm_work/gt_pairs.pkl", "rb"))

def bits24(w):
    s = f"{w:024b}"
    return f"{s[0:8]} {s[8:16]} {s[16:24]}"

def dump(title, pred, limit=16, sort=True):
    rows = []
    for a, (words, text) in pairs.items():
        if len(words) == 1 and pred(text, words[0]):
            rows.append((a, words[0], text))
    print(f"=== {title}: {len(rows)} шт ===")
    if sort:
        rows.sort(key=lambda r: r[1])
    for a, w, t in rows[:limit]:
        print(f"  {a:06x} {bits24(w)} | {t}")

# группа 0: move с (Rn+disp)
dump("move hi4=0 (Rn+disp)", lambda t, w: (w >> 20) == 0 and t.startswith("move") and "(r" in t)
# группа 2: move регистр-регистр / короткий imm
dump("move hi4=2 (reg/imm)", lambda t, w: (w >> 20) == 2 and t.startswith("move"))
# группа 4
dump("move hi4=4", lambda t, w: (w >> 20) == 4 and t.startswith("move"))
# группа 5
dump("move hi4=5", lambda t, w: (w >> 20) == 5 and t.startswith("move"))
