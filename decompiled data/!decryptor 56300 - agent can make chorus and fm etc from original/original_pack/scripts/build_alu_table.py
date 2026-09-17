#!/usr/bin/env python3
# Строит полную таблицу ALU-байтов (bits 7:0) из эталонных листингов.
import pickle, re
from collections import defaultdict

pairs = pickle.load(open("/home/z/my-project/mm_work/gt_pairs.pkl", "rb"))

# паттерн параллельного переноса памяти (X/Y/L)
MEM_PAT = re.compile(r"\s+[xy]:(?:<<\$[0-9a-f]+|\(r[0-7][+)n-][^ ,]*|\$[0-9a-f]+)|\s+l:(?:\?\:>\$[0-9a-f]+|\(r[0-7][^ ,]*|\$[0-9a-f]+)")

def split_alu(text):
    """Отделяет ALU-часть от переноса(ов) памяти. Возвращает (alu, moves)."""
    t = re.sub(r"\s+", " ", text).strip()
    # ищем позиции " x:"/" y:"/" l:" после первого слова
    idxs = [m.start() for m in re.finditer(r" (?=[xy]:)", t)]
    idxs += [m.start() for m in re.finditer(r" (?=l:)", t)]
    idxs = sorted(set(idxs))
    if not idxs:
        return t, []
    alu = t[:idxs[0]].strip()
    rest = t[idxs[0]:].strip()
    # разбиваем rest по " x:"/" y:"/" l:"
    parts = re.split(r"(?= [xy]:| l:)", " " + rest)
    moves = [p.strip() for p in parts if p.strip()]
    return alu, moves

# Для каждой группы (hi-byte) собираем ALU-байт -> текст ALU
groups = defaultdict(lambda: defaultdict(set))
for a, (words, text) in pairs.items():
    if len(words) != 1:
        continue
    w = words[0]
    hi = w >> 16
    alu, moves = split_alu(text)
    groups[hi][w & 0xFF].add(alu)

# печатаем группы 0x40-0x5F (X/Y-memory move) — там ALU-байт = bits 7:0
for hi in sorted(groups):
    if not (0x40 <= hi <= 0x5F):
        continue
    conflicts = {k: v for k, v in groups[hi].items() if len(v) > 1}
    print(f"=== hi={hi:02x}: {len(groups[hi])} ALU-байтов, конфликтов {len(conflicts)} ===")
    if hi in (0x44, 0x45, 0x4c, 0x4d, 0x50, 0x51, 0x56, 0x57):
        for k in sorted(groups[hi]):
            vals = sorted(groups[hi][k])
            print(f"  {k:02x}: {vals[0]}" + (" | " + vals[1] if len(vals) > 1 else ""))
