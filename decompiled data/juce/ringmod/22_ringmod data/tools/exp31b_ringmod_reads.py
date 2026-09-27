#!/usr/bin/env python3
"""exp31b — trace which X/Y addresses machine 17 PROC actually reads."""
import sys
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, run_block, R6

e = build()
run_init(e)

reads = {}
orig_rd = e.rd


def rd(space, ea):
    k = (space, ea)
    reads[k] = reads.get(k, 0) + 1
    return orig_rd(space, ea)


e.rd = rd
# input: DC at phaser convention + a few probes
sig = [0x400000] * 34
out = run_block(e, sig, [0, 0, 60, 127, 0, 0, 0, 64], a_entry=0x123456)

xs = sorted([(ea, c) for (sp, ea), c in reads.items() if sp == "x"])
ys = sorted([(ea, c) for (sp, ea), c in reads.items() if sp == "y"])
print("X reads (non-page):")
for ea, c in xs:
    if not (0x500 <= ea < 0x600):
        print("  X:%06X n=%d" % (ea, c))
print("X reads in page $500-$600:")
for ea, c in xs:
    if 0x500 <= ea < 0x600:
        print("  X:%06X n=%d" % (ea, c))
print("Y reads:")
for ea, c in ys[:80]:
    print("  Y:%06X n=%d" % (ea, c))
