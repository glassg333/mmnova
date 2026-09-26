#!/usr/bin/env python3
"""Bisect M14Case vs dbg-init difference, and test X:$FF snapshot mode."""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import sgn, inject_sine_tables, load_xy_memory
from probe23b_dist_m14 import M14Case

P = 0x400

def snapshot(e):
    return ([e.X.get(i, 0) for i in range(0x300)], [e.Y.get(i, 0) for i in range(0x300)])

def build_dbg(machine=14, xff_zero=True):
    e = build_track_emu(machine=machine)
    inject_sine_tables(e); load_xy_memory(e)
    for a in range(0x2C0, 0x2CA):
        e.X[a] = 0
    if xff_zero:
        e.X[0xFF] = 0
    setup_track(e, base=64, trig=0, vol=127, mach_params=[0.5] * 8)
    e.X[P + 0x0B] = 64
    return e

def frames(e, n=4):
    phase = 0.0
    outs = []
    for f in range(n):
        e.Y[P + 0x28] = 1 if f == 0 else 0
        e.run(0x0100, end=0x02EB, max_steps=800000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(phase)))
            e.Y[0x100 + i] = v & 0xFFFFFF
            phase += 2 * math.pi / 32.0
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(phase)))
            e.X[i] = v & 0xFFFFFF
            e.Y[i] = v & 0xFFFFFF
            phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0B4C, max_steps=800000)
        outs.append(math.sqrt(sum(sgn(e.Y[i]) ** 2 for i in range(0x20)) / 32))
    return outs

def diff_mem(a, b, tag):
    xa, ya = a; xb, yb = b
    d = [(i, xa[i], xb[i]) for i in range(0x300) if xa[i] != xb[i]]
    d += [(0x1000 + i, ya[i], yb[i]) for i in range(0x300) if ya[i] != yb[i]]
    print(tag, "diffs:", len(d), [(hex(i), hex(v1 & 0xFFFFFF), hex(v2 & 0xFFFFFF)) for i, v1, v2 in d[:8]])

# 1) M14Case frame 0
c = M14Case(dist=0)
r0 = c.frame(trig=1)
print("M14Case f0 rms:", round(math.sqrt(sum(v * v for v in r0["out"]) / 32), 1))

# 2) dbg with X:$FF=0 (old mode)
e1 = build_dbg(xff_zero=True)
s1 = snapshot(e1)
print("dbg(XFF=0) frames:", [round(v, 1) for v in frames(e1)])

# 3) dbg with X:$FF snapshot (real mode)
e2 = build_dbg(xff_zero=False)
s2 = snapshot(e2)
diff_mem(s1, s2, "init XFF0 vs XFFsnap:")
print("dbg(XFF=snap) frames:", [round(v, 1) for v in frames(e2)])

# 4) M14Case but without X[0xFF]=0
c2 = M14Case(dist=0)
c2.e.X[0xFF] = c2.e.X.get(0xFF, 0)  # no-op check what snapshot had
print("M14Case X:$FF after init:", hex(c2.e.X.get(0xFF, 0)))
