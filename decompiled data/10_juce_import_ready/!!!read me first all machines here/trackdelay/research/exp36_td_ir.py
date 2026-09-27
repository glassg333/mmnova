#!/usr/bin/env python3
"""
exp36_td_ir.py — Track Delay IR, working harness (run_kernel.setup_voice once,
per-block page refresh like exp34 which evolved state correctly).
Impulse at machine audio slots P+$E6 / P+$F5.
Captures per-block: Y:$0000-$001F (block out), X:$0000-$001F, delay cells.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, Q23

P = 0x400
M24 = 0xFFFFFF

def q23(x):
    return int(round(x * (1 << 23))) & M24

NB = 64
IMP_BLK = 2

def run_case(e, params, verbose=False):
    Y, X = e.Y, e.X
    setup_voice(e, r6=P, trig=1)
    frames = []
    cells = []
    for blk in range(NB):
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
        Y[P + 0x21] = 1 if blk == 0 else 0
        X[P + 0x0F] = 1
        X[P + 0x0B] = 64
        Y[P + 0x23] = 120
        Y[P + 0x10] = 0x400000; Y[P + 0x11] = 0x400000
        Y[P + 0x12] = 0; Y[P + 0x13] = 0
        Y[P + 0x14] = 0x400000; Y[P + 0x15] = 0x400000
        Y[P + 0x16] = 0x400000; Y[P + 0x17] = 0x400000
        Y[P + 0x70] = 0x400000; Y[P + 0x71] = 0x400000
        mach = [0] * 34
        if blk == IMP_BLK:
            mach[0x0A] = 0x400000   # P+$E6
            mach[0x19] = 0x400000   # P+$F5
        for i, v in enumerate(mach):
            Y[P + 0xDC + i] = v
        for a in range(0x000, 0x30):
            X[a] = 0; Y[a] = 0
        e.R[6] = P
        e.R[7] = 0
        e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
        frames.append(([Y[a] for a in range(0x00, 0x20)],
                       [X[a] for a in range(0x00, 0x20)],
                       [Y[a] for a in range(0x20, 0x30)]))
        cells.append((X.get(0x40, 0), X.get(0x44, 0), X.get(0x90, 0), Y.get(0x90, 0),
                      X.get(0xC2, 0), X.get(0xC4, 0), X.get(0xC5, 0),
                      X.get(0xFE, 0), Y.get(0xFE, 0),
                      [X.get(a, 0) for a in range(0x20, 0x40)]))
    return frames, cells

def show(tag, params, nshow=30):
    e = build_emu()
    frames, cells = run_case(e, params)
    print(f"\n=== {tag} {params}")
    for b, (oY, oX, oY2) in enumerate(frames):
        nz = [(i, v) for i, v in enumerate(oY) if v] + [('X' + str(i), v) for i, v in enumerate(oX) if v] + [('Y2', i, v) for i, v in enumerate(oY2) if v]
        if nz and b < IMP_BLK + nshow:
            print(f" blk{b:3d} Y:[{', '.join('%d:%06X' % (i, v) for i, v in nz if not isinstance(i, str))}] "
                  f"X:[{', '.join('%s:%06X' % (i, v) for i, v in nz if isinstance(i, str) and i.startswith('X'))}]")
    c = cells[IMP_BLK + 3]
    print(f"cells: X40={c[0]:06X} X44={c[1]:06X} X90={c[2]:06X} Y90={c[3]:06X} "
          f"XC2={c[4]:06X} XC4={c[5]:06X} XC5={c[6]:06X} XFE={c[7]:06X} YFE={c[8]:06X}")
    print(f"X20-3F: {['%06X' % v for v in c[9]]}")

if __name__ == "__main__":
    show("A: SND=.5 FDB=0 TIM=.3", dict(TIM=0.3, SND=0.5, FDB=0.0, BAS=0.5, WID=0.5))
    show("B: SND=.5 FDB=.4 TIM=.3", dict(TIM=0.3, SND=0.5, FDB=0.4, BAS=0.5, WID=0.5))
