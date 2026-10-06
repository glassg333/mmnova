#!/usr/bin/env python3
"""exp80e — разделение пространств: X-only vs Y-only инжекции на границе AMP.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, R6

P = R6

def run(xy, cell=None, value=0):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, wdth=32, trig=1, hpq=32, lpq=96)
    e.Y[P + 0x12] = 32 << 16; e.Y[P + 0x13] = 96 << 16
    e.X[P + 0x12] = 32 << 16; e.X[P + 0x13] = 96 << 16
    if cell:
        if xy in ("Y", "XY"): e.Y[cell] = value
        if xy in ("X", "XY"): e.X[cell] = value
    cap = {}
    def hook(pc, emu):
        if pc == 0x088E and "out" not in cap:
            cap["out"] = ([emu.X.get(i, 0) for i in range(0x6D, 0x7D)],
                          [emu.Y.get(i, 0) for i in range(0x6D, 0x7D)],
                          [emu.X.get(i, 0) for i in range(0xAD, 0xBD)],
                          [emu.Y.get(i, 0) for i in range(0xAD, 0xBD)])
    try:
        e.run(0x0100, end=0x0B4C, max_steps=500000, hook=hook)
    except Exception:
        return None
    o = cap.get("out")
    return None if o is None else o[0] + o[1] + o[2] + o[3]

base = run("XY")
print(f"база: ненулевых {sum(1 for v in base if v)}/80")
cells = [("Y:$408/X:$408", P+0x08), ("Y:$409/X:$409", P+0x09), ("Y:$40A/X:$40A", P+0x0A),
         ("Y:$40B/X:$40B", P+0x0B), ("Y:$410/X:$410", P+0x10), ("Y:$411/X:$411", P+0x11)]
for name, cell in cells:
    ry = run("Y", cell, 0x5A0000)
    rx = run("X", cell, 0x5A0000)
    dy = sum(1 for a, b in zip(ry, base) if a != b)
    dx = sum(1 for a, b in zip(rx, base) if a != b)
    print(f"{name}=90: Y-only отличий {dy}/80, X-only отличий {dx}/80")
