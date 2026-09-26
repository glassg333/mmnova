#!/usr/bin/env python3
"""
exp37_td_exact.py — Track Delay IR with EXACT main-loop entry context:
  r6 = Y:$123-$28 = $400, r7 = $100 (stack = Y:$100-$121), r5 = X:$2C3 = $260,
  X:$2C0=$140 X:$2C1=$1A0 X:$2C2=$200 X:$2C4=0, X:$FF=$140, X:$2C9=$2CC.
Machine skipped (X:$40C=0); machine output injected at Y:$4DC-$4FB.
Captures X:$140-$15F + Y:$0000-$001F per block.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, Q23

P = 0x400
M24 = 0xFFFFFF

def q23(x):
    return int(round(x * (1 << 23))) & M24

NB = 64
IMP_BLK = 2

def run_case(e, params, imp_slots=(0x0A, 0x19), imp_val=0x400000):
    Y, X = e.Y, e.X
    # main-loop bases (DSP1)
    X[0x2C0] = 0x140; X[0x2C1] = 0x1A0; X[0x2C2] = 0x200; X[0x2C3] = 0x260
    X[0x2C4] = 0; X[0x2C8] = 0; X[0x2C9] = 0x2CC
    X[0x00FF] = 0x140
    Y[0x123] = P + 0x28
    Y[0x124] = 0
    Y[0x4FF] = 0x7FFFFF     # global level full
    # zero scratch once (delay state starts empty)
    for a in range(0x00, 0x100):
        X[a] = 0; Y[a] = 0
    for a in range(0x100, 0x200):
        Y[a] = 0; X[a] = 0
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
        Y[P + 0x28] = 1 if blk == 0 else 0   # TRIG_EVENT
        Y[P + 0x0E] = 0; Y[P + 0x0F] = 0     # pan L/R? (page cells)
        Y[P + 0x10] = 0x400000; Y[P + 0x11] = 0x400000
        Y[P + 0x12] = 0; Y[P + 0x13] = 0
        Y[P + 0x14] = 0x400000; Y[P + 0x15] = 0x400000
        Y[P + 0x16] = 0x400000; Y[P + 0x17] = 0x400000
        Y[P + 0x70] = 0x400000; Y[P + 0x71] = 0x400000
        mach = [0] * 34
        if blk == IMP_BLK:
            for s in imp_slots:
                mach[s] = imp_val
        for i, v in enumerate(mach):
            Y[P + 0xDC + i] = v
        # clear output/mix areas
        for a in range(0x000, 0x30):
            X[a] = 0; Y[a] = 0
        for a in range(0x140, 0x160):
            X[a] = 0
        e.R[6] = P
        e.R[7] = 0x100
        e.R[5] = 0x260
        for r in range(8):
            e.M[r] = 0xFFFFFF
        e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
        frames.append(([Y[a] for a in range(0x00, 0x20)],
                       [X[a] for a in range(0x140, 0x160)],
                       [Y[a] for a in range(0x20, 0x30)]))
        cells.append(dict(X40=X.get(0x40, 0), X44=X.get(0x44, 0),
                          X32=X.get(0x32, 0), X33=X.get(0x33, 0),
                          X90=X.get(0x90, 0), Y90=Y.get(0x90, 0),
                          XC2=X.get(0xC2, 0), YC2=Y.get(0xC2, 0),
                          XC4=X.get(0xC4, 0), YC4=Y.get(0xC4, 0),
                          XC5=X.get(0xC5, 0), YC5=Y.get(0xC5, 0),
                          XFE=X.get(0xFE, 0), YFE=Y.get(0xFE, 0),
                          X20=[X.get(a, 0) for a in range(0x20, 0x40)]))
    return frames, cells

def show(tag, params, nshow=40):
    e = build_emu()
    frames, cells = run_case(e, params)
    print(f"\n=== {tag} {params}")
    for b, (oY, oX, oY2) in enumerate(frames):
        if b > IMP_BLK + nshow:
            break
        nzY = [(i, v) for i, v in enumerate(oY) if v]
        nzX = [(i, v) for i, v in enumerate(oX) if v]
        nz2 = [(i, v) for i, v in enumerate(oY2) if v]
        if nzY or nzX or nz2:
            print(f" blk{b:3d} Y00-1F:[{', '.join('%d:%06X' % t for t in nzY[:8])}] "
                  f"X140:[{', '.join('%d:%06X' % t for t in nzX[:8])}] Y20:[{', '.join('%d:%06X' % t for t in nz2[:6])}]")
    c = cells[IMP_BLK + 3]
    print(f"cells: X40={c['X40']:06X} X44={c['X44']:06X} X32={c['X32']:06X} X90={c['X90']:06X} Y90={c['Y90']:06X}")
    print(f"  XC2={c['XC2']:06X}/{c['YC2']:06X} XC4={c['XC4']:06X}/{c['YC4']:06X} XC5={c['XC5']:06X}/{c['YC5']:06X} XFE={c['XFE']:06X} YFE={c['YFE']:06X}")
    print(f"  X20-3F: {['%06X' % v for v in c['X20']]}")

if __name__ == "__main__":
    show("A: SND=.5 FDB=0 TIM=.3", dict(TIM=0.3, SND=0.5, FDB=0.0, BAS=0.5, WID=0.5))
