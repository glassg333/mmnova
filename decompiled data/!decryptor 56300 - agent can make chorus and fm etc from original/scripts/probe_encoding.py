#!/usr/bin/env python3
# Разведка кодировки: кластеризация по битам для каждой мнемоники
import pickle
from collections import Counter, defaultdict

pairs = pickle.load(open("/home/z/my-project/mm_work/gt_pairs.pkl", "rb"))

def show(name, filt, limit=14):
    rows = [(w, t) for w, t in pairs.values() if filt(t)]
    print(f"=== {name}: {len(rows)} шт ===")
    seen = Counter()
    for w, t in rows:
        seen[(w >> 16, t)] += 1
    for (hi, t), n in seen.most_common(limit):
        print(f"  hi={hi:02x} ({hi:08b}) x{n}: {t}")

# move с 1 словом (24-битные) — группы по старшему байту
c = Counter()
for w, t in pairs.items():
    words, text = w, t
    if not text.startswith("move") or len(words) != 1:
        continue
    c[(words[0] >> 20, text.split(None, 1)[1][:18])] += 1
print("=== move (1 слово), группы по битам 23-20 ===")
for (hi, samp), n in sorted(c.items(), key=lambda x: -x[1])[:28]:
    print(f"  hi4={hi:x} x{n:5d}: напр. {samp}")
