#!/usr/bin/env python3
"""
exp34_td_multiblock.py — Track Delay impulse response over many blocks.
Runs the post-voice chain repeatedly (state persists in scratch), injects a
one-block impulse via the machine output buffer, captures the output block
(Y:$0000-$001F) + key delay cells for each block. Sweeps TIM/FDB/BAS/WID/SND.
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, Q23

def q23(x):
    v = int(round(x * Q23))
    return max(-0x800000, min(0x7FFFFF, v)) & 0xFFFFFF

NBLOCKS = 96
IMP_BLOCK = 2

def run_case(e, params, verbose=False):
    Y, X = e.Y, e.X
    # one-time voice page init
    setup_voice(e, r6=0x400, trig=1)
    Y[0x400 + 0x1A] = q23(params.get('SRR', 0.0))
    Y[0x400 + 0x1B] = q23(params.get('TIM', 0.3))
    Y[0x400 + 0x1C] = q23(params.get('SND', 0.5))
    Y[0x400 + 0x1D] = q23(params.get('FDB', 0.0))
    Y[0x400 + 0x1E] = q23(params.get('BAS', 0.5))
    Y[0x400 + 0x1F] = q23(params.get('WID', 0.5))
    # keep scratch + delay SRAM intact across blocks
    out_blocks = []
    cell_hist = []   # (X:$40..$4F, X:$90, Y:$90, X:$C2,$C4,$C5, X:$FE,Y:$FE)
    for blk in range(NBLOCKS):
        # machine output: impulse only on IMP_BLOCK
        for i in range(0x200):
            Y[0x400 + i] = 0
            X[0x400 + i] = 0
        if blk == IMP_BLOCK:
            Y[0x400 + 0xDC + 0] = 0x400000
            Y[0x400 + 0xDC + 16] = 0x400000
        Y[0x400 + 0x28] = 1 if blk == 0 else 0     # trig only once
        for a in range(0x000, 0x30):
            X[a] = 0; Y[a] = 0    # clear output/mix area only
        e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
        ob = [Y[a] for a in range(0x00, 0x20)]
        out_blocks.append(ob)
        cell_hist.append((
            [X.get(a, 0) for a in range(0x40, 0x50)],
            X.get(0x90, 0), Y.get(0x90, 0),
            X.get(0xC2, 0), Y.get(0xC2, 0),
            X.get(0xC4, 0), Y.get(0xC4, 0),
            X.get(0xC5, 0), Y.get(0xC5, 0),
            X.get(0xFE, 0), Y.get(0xFE, 0),
            X.get(0x32, 0), X.get(0x44, 0),
            [X.get(a, 0) for a in range(0x20, 0x40)],
        ))
    return out_blocks, cell_hist

def peak_per_block(out_blocks):
    res = []
    for ob in out_blocks:
        mx = 0; mi = -1
        for i, v in enumerate(ob):
            a = v - 0x1000000 if v >= 0x800000 else v
            if abs(a) > abs(mx):
                mx = a; mi = i
        res.append((mi, mx))
    return res

def f24(v):
    return (v - (1 << 24)) / (1 << 24) if v >= 0x800000 else v / (1 << 23) * 0.5 * 2 if False else (v - (1 << 24)) / (1 << 24)

def show(tag, params):
    e = build_emu()
    obs, cells = run_case(e, params)
    pk = peak_per_block(obs)
    nz = [(b, i, v) for b, (i, v) in enumerate(pk) if v != 0]
    print(f"\n=== {tag}  {params}")
    print("non-zero blocks (blk, idx, raw):")
    for b, i, v in nz[:40]:
        print(f"  blk {b:3d}  idx {i:2d}  {v:08X}")
    if len(nz) > 40:
        print(f"  ... {len(nz)-40} more")
    c = cells[IMP_BLOCK + 1] if IMP_BLOCK + 1 < len(cells) else cells[-1]
    print(f"cells after impulse: X40-4F={['%06X'%v for v in c[0]]}")
    print(f"  X90={c[1]:06X} Y90={c[2]:06X} XC2={c[3]:06X}/{c[4]:06X} XC4={c[5]:06X}/{c[6]:06X} XC5={c[7]:06X}/{c[8]:06X} XFE={c[9]:06X} YFE={c[10]:06X} X32={c[11]:06X} X44={c[12]:06X}")
    print(f"  X20-3F={['%06X'%v for v in c[13]]}")
    return obs, cells

if __name__ == "__main__":
    # 1) basic: SND only (FDB=0) — isolate delay path
    show("A: SND-only FDB=0 TIM=.3", dict(TIM=0.3, SND=0.5, FDB=0.0, BAS=0.5, WID=0.5))
    # 2) with feedback
    show("B: FDB=.4 TIM=.3", dict(TIM=0.3, SND=0.5, FDB=0.4, BAS=0.5, WID=0.5))
