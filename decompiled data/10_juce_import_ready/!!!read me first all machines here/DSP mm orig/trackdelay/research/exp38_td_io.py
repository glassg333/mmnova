#!/usr/bin/env python3
"""
exp38_td_io.py — Track Delay IO mapping.
Machine output = Y:$100-$121 (popped at $04FC to X:$97-$A7 / X:$D7-$E7).
Inject impulse into the Y stack machine-output slots; capture the two channel
windows and the final outputs (Y:$0000-$001F, X:$140-$15F).
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, Q23

P = 0x400
M24 = 0xFFFFFF

def q23(x):
    return int(round(x * (1 << 23))) & M24

def run_frame(e, mach_stack, params, blk=0):
    Y, X = e.Y, e.X
    for i in range(0x100):
        Y[P + i] = 0
        X[P + i] = 0
    Y[P + 0x1A] = q23(params.get('SRR', 0.0))
    Y[P + 0x1B] = q23(params.get('TIM', 0.3))
    Y[P + 0x1C] = q23(params.get('SND', 0.5))
    Y[P + 0x1D] = q23(params.get('FDB', 0.0))
    Y[P + 0x1E] = q23(params.get('BAS', 0.5))
    Y[P + 0x1F] = q23(params.get('WID', 0.5))
    Y[P + 0x28] = 1 if blk == 0 else 0
    Y[P + 0x10] = 0x400000; Y[P + 0x11] = 0x400000
    Y[P + 0x12] = 0; Y[P + 0x13] = 0
    Y[P + 0x14] = 0x400000; Y[P + 0x15] = 0x400000
    Y[P + 0x16] = 0x400000; Y[P + 0x17] = 0x400000
    Y[P + 0x70] = 0x400000; Y[P + 0x71] = 0x400000
    for i, v in enumerate(mach_stack):
        Y[0x100 + i] = v & M24
    for a in range(0x000, 0x30):
        X[a] = 0; Y[a] = 0
    for a in range(0x90, 0xF0):
        X[a] = 0
    e.R[6] = P
    e.R[7] = 0x100
    e.R[5] = 0x260
    e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
    return ([X.get(a, 0) for a in range(0x97, 0xA8)],   # window A
            [X.get(a, 0) for a in range(0xD7, 0xE8)],   # window B
            [Y.get(a, 0) for a in range(0x00, 0x20)],
            [X.get(a, 0) for a in range(0x140, 0x160)])

if __name__ == "__main__":
    e = build_emu()
    p = dict(TIM=0.3, SND=0.5, FDB=0.0, BAS=0.5, WID=0.5)
    # probe: impulse in each stack slot 0..5, see where it surfaces
    for slot in range(6):
        st = [0] * 34
        st[slot] = 0x400000
        wA, wB, oY, oX = run_frame(e, st, p)
        nzA = [(i, v) for i, v in enumerate(wA) if v]
        nzB = [(i, v) for i, v in enumerate(wB) if v]
        nzO = [(i, v) for i, v in enumerate(oY) if v] + [('X', i, v) for i, v in enumerate(oX) if v]
        print(f"slot {slot:2d}: wA={nzA[:4]} wB={nzB[:4]} out={nzO[:6]}")
