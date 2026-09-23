#!/usr/bin/env python3
"""exp26_cf_disasm.py — линейный дизасм coldfire_main.bin (capstone M68K, база $200000).
Выход: mining/exp26_cf_full.asm (листинг) + mining/exp26_cf_disasm.json (адрес->текст)."""
import json, sys
from capstone import Cs, CS_ARCH_M68K, CS_MODE_M68K_020

BIN = "/home/z/my-project/work/mmnova/decompiled data/05_descriptors/coldfire_main.bin"
BASE = 0x200000
OUT_TXT = "/home/z/my-project/mining/exp26_cf_full.asm"
OUT_JSON = "/home/z/my-project/mining/exp26_cf_disasm.json.gz"

data = open(BIN, "rb").read()
md = Cs(CS_ARCH_M68K, CS_MODE_M68K_020)
md.detail = False

lines = []
d = {}
pc = 0
n = len(data)
bad = 0
good = 0
while pc < n:
    chunk = data[pc:pc + 16]
    try:
        ins = next(md.disasm(chunk, BASE + pc))
    except StopIteration:
        # продвигаемся по 2 байта на невыровненном месте
        lines.append("%06x  %-20s .dc.w  $%04x" % (BASE + pc, "", int.from_bytes(data[pc:pc+2], "big")))
        pc += 2
        bad += 1
        continue
    size = ins.size
    if size == 0 or pc + size > n:
        lines.append("%06x  .dc.w  ?" % (BASE + pc))
        pc += 2
        bad += 1
        continue
    raw = data[pc:pc + size].hex()
    lines.append("%06x  %-14s %s %s" % (ins.address, raw, ins.mnemonic, ins.op_str))
    d[ins.address] = (ins.mnemonic + " " + ins.op_str, size)
    pc += size
    good += 1

open(OUT_TXT, "w").write("\n".join(lines))
import gzip
with gzip.open(OUT_JSON, "wt") as f:
    json.dump({hex(k): v for k, v in d.items()}, f)
print("good:", good, "bad:", bad, "lines:", len(lines))
print("saved", OUT_TXT)
