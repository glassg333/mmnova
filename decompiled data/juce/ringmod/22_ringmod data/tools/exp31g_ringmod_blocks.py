#!/usr/bin/env python3
"""exp31g — run 6 blocks, watch carrier come alive (L:$FC pipeline delay)."""
import sys
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, run_block, f

e = build()
run_init(e)
params = [0, 0, 60, 127, 0, 0, 0, 64]
for blk in range(6):
    out = run_block(e, [0x400000] * 34, params, a_entry=0x123456)
    ph = (e.Y.get(0x528 + 0x0E, 0) << 24) | e.Y.get(0x528 + 0x0F, 0)
    lfb = (e.X.get(0xFB, 0) << 24) | e.Y.get(0xFB, 0)
    lfc = (e.X.get(0xFC, 0) << 24) | e.Y.get(0xFC, 0)
    lfd = (e.X.get(0xFD, 0) << 24) | e.Y.get(0xFD, 0)
    p10 = (e.X.get(0x538, 0) << 24) | e.Y.get(0x538, 0)
    car = [f(e.Y.get(a, 0)) for a in range(0x63, 0x6B)]
    nz = sum(1 for a in range(0x63, 0x73) if e.Y.get(a, 0))
    print("blk %d: phase=%012X p10=%012X LFB=%012X LFC=%012X LFD=%012X carrNZ=%d" %
          (blk, ph, p10, lfb, lfc, lfd, nz))
    print("   car:", " ".join("%+.4f" % v for v in car))
    print("   out:", " ".join(str(v >> 16) for v in out[:8]))
