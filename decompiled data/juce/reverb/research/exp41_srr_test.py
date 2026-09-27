#!/usr/bin/env python3
"""
exp41_srr_test.py — does the SRR knob ($51A) decimate the dry path?
Full track frame ($0100-$0B4C) with real GND-SIN machine; compare outputs for
SRR=0 vs SRR=127: look for zero-order-hold (repeated samples) / spectral change.
"""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm, PM
from dsp_emu import DSP56300

P = 0x400
M24 = 0xFFFFFF

def build_track():
    lines = disasm(0x0000, 0x0B4E)
    lines += disasm(0x144000, 0x2000)
    e = DSP56300(lines)
    data = open(PM, "rb").read()
    n = len(data) // 3
    for i in range(0x100000, min(n, 0x160000)):
        o = i * 3
        w = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
        e.X[i] = w
        e.Y[i] = w
    for i in range(8192):
        s = int(round(math.sin(2 * math.pi * i / 8192) * 8388607)) & 0xFFFFFF
        e.X[0x14A000 + i] = s
        e.Y[0x14A000 + i] = s
    slot = 0x1001AF + 1
    e.Y[0x120] = slot; e.Y[0x121] = slot; e.Y[0x122] = slot
    e.X[slot + 0x1001AF] = 0x144CD0
    e.X[slot + 0x10016B] = 0x144CC9
    e.X[0x2C0] = 0x140; e.X[0x2C1] = 0x1A0; e.X[0x2C2] = 0x200; e.X[0x2C3] = 0x260
    e.X[0x2C4] = 0; e.X[0x2C8] = 0; e.X[0x2C9] = 0x2CC
    e.X[0x00FF] = 0x140
    return e

def setup(e, srr=0, eqf=0, eqg=0, tim=0, snd=0, fdb=0, bas=0, wid=0, pitch=0x5800):
    Y, X = e.Y, e.X
    for i in range(0x100):
        Y[P + i] = 0; X[P + i] = 0
    q = lambda v: (int(v) & 0xFFFF) << 16
    Y[P + 0x18] = q(eqf); Y[P + 0x19] = q(eqg)
    Y[P + 0x1A] = q(srr); Y[P + 0x1B] = q(tim)
    Y[P + 0x1C] = q(snd); Y[P + 0x1D] = q(fdb)
    Y[P + 0x1E] = q(bas); Y[P + 0x1F] = q(wid)
    Y[P + 0x29] = pitch          # pitch target V+$01
    Y[P + 0x28] = 1              # V+$00 flags/trig
    Y[P + 0x23] = 120 << 16      # TEMPO
    Y[P + 0x0D] = 127 << 16      # volume full
    Y[P + 0x10] = 0x400000; Y[P + 0x11] = 0x400000
    Y[P + 0x12] = 0; Y[P + 0x13] = 0x40 << 16   # LPQ mid (echo gate test)
    Y[P + 0x14] = 0x400000; Y[P + 0x15] = 0x400000
    Y[P + 0x16] = 0x400000; Y[P + 0x17] = 0x400000
    Y[P + 0x21] = 1
    Y[0x120] = e.Y[0x120]; Y[0x121] = e.Y[0x121]; Y[0x122] = e.Y[0x122]
    Y[0x123] = P + 0x28
    Y[0x124] = 0

def run_frames(e, n):
    outs = []
    for k in range(n):
        Y = e.Y
        Y[0x124] = 0
        Y[0x123] = P + 0x28
        Y[P + 0x28] = 1 if k == 0 else 0
        e.R[6] = P + 0x28
        e.R[7] = 0x100
        e.run(0x0100, end=0x0B4C, max_steps=2_000_000)
        outs.append(([e.Y.get(a, 0) for a in range(0x100, 0x110)], [e.X.get(a, 0) for a in range(0x97, 0xA7)]))
    return outs

def sgn(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v

if __name__ == "__main__":
    for srr in (0, 64, 127):
        e = build_track()
        setup(e, srr=srr)
        outs = run_frames(e, 24)
        blk = outs[16][1]
        s = [sgn(v) for v in blk]
        # count repeated consecutive samples (ZOH indicator)
        rep = sum(1 for i in range(1, len(s)) if s[i] == s[i-1] and s[i] != 0)
        mx = max(abs(v) for v in s)
        nz = sum(1 for v in s if v)
        print(f"SRR={srr:3d}: blk16 = {['%+9d' % v for v in s[:10]]}")
        print(f"          max={mx:9d} nonzero={nz} repeats={rep}")
        print('          machout[0:8] =', ['%06X' % (v & M24) for v in outs[16][0][:8]])
