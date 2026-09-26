#!/usr/bin/env python3
"""
exp39_td_ir2.py — Track Delay IR over blocks (correct IO):
machine out stack Y:$100-$121; capture X:$260-$28F (r5 out) + Y:$0000-$001F.
Sweeps TIM/FDB/BAS/WID/SND/SRR; reports echo pattern + feedback filter.
"""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, Q23

P = 0x400
M24 = 0xFFFFFF

def q23(x):
    return int(round(x * (1 << 23))) & M24

def run_frames(e, mach_stacks, params):
    Y, X = e.Y, e.X
    outs = []
    cells = []
    for blk, st in enumerate(mach_stacks):
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
        for i, v in enumerate(st):
            Y[0x100 + i] = v & M24
        for a in range(0x000, 0x30):
            X[a] = 0; Y[a] = 0
        e.R[6] = P
        e.R[7] = 0x100
        e.R[5] = 0x260
        e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
        outs.append([X.get(a, 0) for a in range(0x260, 0x280)])
        cells.append(dict(X90=X.get(0x90, 0), Y90=Y.get(0x90, 0),
                          XC2=X.get(0xC2, 0), XC4=X.get(0xC4, 0), XC5=X.get(0xC5, 0),
                          XFE=X.get(0xFE, 0), YFE=Y.get(0xFE, 0),
                          buf=[X.get(a, 0) for a in range(0x20, 0x50)]))
    return outs, cells

def signed(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v

def impulse_train(nblocks, imp_blk=2):
    sts = []
    for b in range(nblocks):
        st = [0] * 34
        if b == imp_blk:
            st[1] = 0x400000   # odd slot -> R window; try L too
            st[0] = 0x400000
        sts.append(st)
    return sts

def run_tag(tag, params, nblocks=80):
    e = build_emu()
    outs, cells = run_frames(e, impulse_train(nblocks), params)
    print(f"\n=== {tag} {params}")
    for b, ob in enumerate(outs):
        nz = [(i, signed(v)) for i, v in enumerate(ob) if v]
        if nz:
            print(f" blk{b:3d}: {nz[:10]}")
    c = cells[30]
    print(f"cells@30: X90={c['X90']:06X} Y90={c['Y90']:06X} XC2={c['XC2']:06X} "
          f"XC4={c['XC4']:06X} XC5={c['XC5']:06X} XFE={c['XFE']:06X} YFE={c['YFE']:06X}")
    print(f"  buf X20-4F: {['%06X' % v for v in c['buf']]}")
    return outs, cells

if __name__ == "__main__":
    run_tag("A FDB=0", dict(TIM=0.3, SND=0.5, FDB=0.0, BAS=0.5, WID=0.5))
    run_tag("B FDB=.4", dict(TIM=0.3, SND=0.5, FDB=0.4, BAS=0.5, WID=0.5))
