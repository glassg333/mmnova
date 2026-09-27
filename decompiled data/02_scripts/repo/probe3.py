#!/usr/bin/env python3
# Полная карта опкодов: группировка 1-словных инструкций по битам 23-16
import pickle
from collections import defaultdict

pairs = pickle.load(open("/home/z/my-project/mm_work/gt_pairs.pkl", "rb"))

def bits24(w):
    s = f"{w:024b}"
    return f"{s[0:8]}_{s[8:16]}_{s[16:24]}"

groups = defaultdict(list)
for a, (words, text) in pairs.items():
    if len(words) == 1:
        groups[words[0] >> 16].append((a, words[0], text))

# сводная таблица: hi-byte → мнемоники
print("=== Карта: биты 23-16 → мнемоники (1-словные) ===")
for hi in sorted(groups):
    mn = {}
    for a, w, t in groups[hi]:
        m = t.split()[0]
        mn.setdefault(m, 0)
        mn[m] += 1
    top = sorted(mn.items(), key=lambda x: -x[1])
    ex = sorted(groups[hi], key=lambda r: r[1])
    sample = " ; ".join(f"{bits24(w)}:{t}" for _, w, t in ex[:2])
    print(f"  {hi:02x} ({hi:08b}): {top[:4]}  | {sample[:150]}")
