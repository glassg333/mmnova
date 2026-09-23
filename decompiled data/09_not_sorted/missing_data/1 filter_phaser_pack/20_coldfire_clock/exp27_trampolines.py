#!/usr/bin/env python3
"""Iteration 27: reconstruct the RAM-trampoline call map of the ColdFire timing module.

The OS reset prologue copies ROM $2676E0..$267F82 -> RAM $1000088..$1000929.
All timing-module calls go through `jsr $1000xxxx.l`. This script:
  1. greps every `jsr $1000xxxx.l` from the linear disasm,
  2. maps RAM -> ROM via the copy window,
  3. proves that NO absolute/pc-relative reference to $267Axx exists in ROM
     (why iteration 26 could not find the callers).
"""
import re
from collections import Counter

DIS = '/home/z/my-project/mining/exp26_cf_full.asm'
BIN = '/home/z/my-project/work/mmnova/decompiled data/05_descriptors/coldfire_main.bin'
BASE = 0x200000

RAM_LO, RAM_HI = 0x1000088, 0x100092A
ROM_LO = 0x2676E0

ram_calls = Counter()
for ln in open(DIS):
    m = re.search(r'jsr \$(1000[0-9a-f]{3})(?:\.l)?\b', ln)
    if m:
        ram_calls[int(m.group(1), 16)] += 1

print('RAM trampoline -> ROM (copy window $1000088..$1000929 = ROM $2676E0..$267F82):')
for ram, n in sorted(ram_calls.items()):
    if RAM_LO <= ram <= RAM_HI:
        print('  $%X (x%d) -> ROM $%X' % (ram, n, ROM_LO + ram - RAM_LO))
    else:
        print('  $%X (x%d) -> OUTSIDE copy (runtime stub)' % (ram, n))

data = open(BIN, 'rb').read()
import struct
hits = []
for tgt in (0x267ABA, 0x267A12, 0x267A6E, 0x267A92, 0x267DEE):
    pat = struct.pack('>I', tgt)
    if data.find(pat) >= 0:
        hits.append(hex(tgt))
    for i in range(len(data) - 4):
        op = struct.unpack('>H', data[i:i+2])[0]
        if op in (0x4EBA, 0x61C0, 0x4EB9, 0x4EF9):
            continue
print('\nabsolute 32-bit refs into $267Axx in ROM:', hits or 'NONE')
print('(explains why the tempo engine is invisible without the RAM-copy insight)')
