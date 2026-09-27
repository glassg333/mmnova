#!/usr/bin/env python3
# Решатель полей регистровых move: ищем (позиция src, позиция dst, таблица регов)
import pickle, re, itertools

pairs = pickle.load(open("/home/z/my-project/mm_work/gt_pairs.pkl", "rb"))
pat = re.compile(r"^move\s+([ab][012]?|[xy][01]|r[0-7]|n[0-7]|m[0-7]),\s*([ab][012]?|[xy][01]|r[0-7]|n[0-7]|m[0-7])$")
rows = []
for a, (words, text) in pairs.items():
    if len(words) != 1:
        continue
    m = pat.match(text)
    if m:
        rows.append((words[0], m.group(1), m.group(2)))
print(f"пар: {len(rows)}")

regs = ["x0","x1","y0","y1","a0","b0","a2","b2","a1","b1","a","b",
        "r0","r1","r2","r3","r4","r5","r6","r7",
        "n0","n1","n2","n3","n4","n5","n6","n7",
        "m0","m1","m2","m3","m4","m5","m6","m7"]

def extract(w, pos, width):
    return (w >> pos) & ((1 << width) - 1)

# для каждой ширины/позиции строим отображение код->регистр из данных и ищем противоречия
best = []
for width in (5, 6):
    for spos in range(0, 25 - width):
        for dpos in range(0, 25 - width):
            smap, dmap = {}, {}
            ok = True
            for w, s, d in rows:
                sc, dc = extract(w, spos, width), extract(w, dpos, width)
                if sc in smap and smap[sc] != s: ok = False; break
                if dc in dmap and dmap[dc] != d: ok = False; break
                smap[sc] = s; dmap[dc] = d
            if ok:
                # полнота: сколько уникальных регов покрыто
                best.append((width, spos, dpos, len(set(smap.values())), len(set(dmap.values()))))
best.sort(key=lambda t: -(t[3] + t[4]))
for b in best[:10]:
    print("width", b[0], "spos", b[1], "dpos", b[2], "src regs", b[3], "dst regs", b[4])

if best:
    width, spos, dpos, _, _ = best[0]
    smap, dmap = {}, {}
    for w, s, d in rows:
        smap[extract(w, spos, width)] = s
        dmap[extract(w, dpos, width)] = d
    print("S-таблица:", {k: smap[k] for k in sorted(smap)})
    print("D-таблица:", {k: dmap[k] for k in sorted(dmap)})
