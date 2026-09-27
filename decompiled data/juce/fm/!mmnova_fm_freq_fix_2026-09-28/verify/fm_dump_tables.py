#!/usr/bin/env python3
"""Dump the REAL firmware tables used by FM machines:
   P:$141A80 (24 words, FM STAT/PAR ratio table used at P:$145DA5-$145DAD)
   Also scan a wider window around it to see the actual table bounds,
   and X:$14A000 sine table first values.
Decisive question: does the firmware ratio table contain 0 / sub-1/32 entries?
"""
DD = "/home/z/my-project/work/mmnova/decompiled data"
PM = DD + "/02_memory_images/dsp1_pmem.bin"

data = open(PM, "rb").read()
n = len(data) // 3

def pword(addr):
    o = addr * 3
    return (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]

def signed(w):
    return w - (1 << 24) if w & 0x800000 else w

def as_q(w, frac=23):
    return signed(w) / float(1 << frac)

print("=== P:$141A80 .. +48 words (FM ratio table region) ===")
base = 0x141A80
for i in range(0, 48):
    w = pword(base + i)
    q = as_q(w)          # Q23 interpretation
    q20 = as_q(w, 20)    # ratio = raw / 0x80000 => divide by 2^19? try both
    q19 = as_q(w, 19)
    print(f"P:{base+i:06X}  raw={w:06X}  signed={signed(w):8d}  /2^23={q:+.9f}  /2^20={q20:+.6f}  /2^19={q19:+.6f}")

print()
print("=== X:$14A000 sine table, first 8 and around 0x400 (quarter) ===")
XM = DD + "/02_memory_images/dsp1_xmem.bin"
xd = open(XM, "rb").read()
xn = len(xd) // 3
def xword(addr):
    if addr >= xn: return None
    o = addr * 3
    return (xd[o] << 16) | (xd[o + 1] << 8) | xd[o + 2]
for a in [0x14A000 + i for i in range(8)]:
    w = xword(a)
    if w is None:
        print(f"X:{a:06X}  -- вне образа X (len={xn})"); continue
    print(f"X:{a:06X}  raw={w:06X}  /2^23={as_q(w):+.6f}")
