#!/usr/bin/env python3
"""exp26_cf_scan.py — сырые поиски констант/строк в coldfire_main.bin (база $200000).
Итерация 26: ColdFire-сторона эха (DTIM/DBAS/TEMPO -> P$2A, DSND/DFB роутинг)."""
import json, re, sys

BIN = "/home/z/my-project/work/mmnova/decompiled data/05_descriptors/coldfire_main.bin"
BASE = 0x200000

data = open(BIN, "rb").read()
print("size:", len(data), "hex:", data[:16].hex())

def find_all(pat: bytes, limit=64):
    out = []
    i = data.find(pat)
    while i != -1 and len(out) < limit:
        out.append(i)
        i = data.find(pat, i + 1)
    return out

# --- 1. Ключевые константы (big-endian байты) ---
consts = {
    "$21D10":      bytes.fromhex("021D10"),      # масштаб длины эха (DSP-сторона)
    "$791FD0":     bytes.fromhex("791FD0"),      # TEMPO-гейт константа (DSP-сторона)
    "$0CCCCD":     bytes.fromhex("0CCCCD"),      # retune 0.05 (DSP)
    "$733333":     bytes.fromhex("733333"),      # retune 0.45 (DSP)
    "$500000":     bytes.fromhex("500000"),      # HI08 DSP1
    "$600000":     bytes.fromhex("600000"),      # HI08 DSP2
    "$042A":       bytes.fromhex("042A"),        # Y:$042A = P$2A трека 0 (16-бит)
    "$3FFF":       bytes.fromhex("3FFF"),        # маска кольца
    "$FFC0":       bytes.fromhex("FFC0"),
}
res = {}
for name, pat in consts.items():
    hits = find_all(pat)
    res[name] = [hex(h + BASE) for h in hits]
    print(f"{name:10s} hits={len(hits):4d}  first: {[hex(h+BASE) for h in hits[:12]]}")

# --- 2. ASCII-строки: имена ручек/страниц ---
print("\n--- strings ---")
for s in [b"DTIM", b"DSND", b"DFB", b"DBAS", b"DWID", b"TEMPO", b"BPM", b"EFFX",
          b"DLY", b"EQF", b"EQG", b"SRR", b"LPQ", b"tune", b"ECHO", b"echo"]:
    hits = find_all(s, limit=20)
    if hits:
        ctxs = []
        for h in hits[:6]:
            ctx = data[max(0, h-8):h+len(s)+8]
            ctxs.append((hex(h + BASE), ctx))
        print(s.decode(), len(hits), ctxs)
    else:
        print(s.decode(), 0)

# --- 3. Длины/факты для ворклога ---
json.dump(res, open("/home/z/my-project/mining/exp26_cf_rawhits.json", "w"), indent=1)
print("\nsaved mining/exp26_cf_rawhits.json")
