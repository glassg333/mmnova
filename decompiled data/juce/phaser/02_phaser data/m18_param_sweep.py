#!/usr/bin/env python3
"""exp23_m18_sweep.py — param -> block mapping for the real phaser m18.
Forces knobs directly into the FX page Y:$42C-$433 (kernel does not copy them
in the harness), runs 4 frames per setting, dumps the affected banks."""
import sys, math

sys.path.insert(0, "/home/z/my-project/scripts")
import track_harness as th

M24 = 0xFFFFFF
R6 = 0x428


def build():
    th.PROC[18] = 0x145045
    th.PROC_INIT[18] = 0x145036
    e = th.build_track_emu(machine=18)
    for addr, (mnem, toks, text) in list(e.prog.items()):
        if mnem == "movep":
            e.prog[addr] = ("nop", [], text + "  ; STUBBED movep")
    th.setup_track(e, base=64, trig=1, vol=1.0)
    return e


def run4(e, knobs, phase0):
    phase = phase0
    for k in range(4):
        e.Y[0x400 + 0x28] = 1 if k == 0 else 0
        e.run(0x0100, end=0x02EB, max_steps=800000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            e.Y[0x100 + i] = int(round(0.9 * 8388607 * math.sin(phase))) & 0xFFFFFF
            phase += 2 * math.pi / 32.0
        for i in range(16):
            e.X[i] = int(round(0.9 * 8388607 * math.sin(phase))) & 0xFFFFFF
            e.Y[i] = e.X[i]
            phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0B4C, max_steps=800000)
    return phase


def dump(e, tag):
    print("== %s" % tag)
    print("  phase $439/$43A: %06X %06X  sm $43B: %06X %06X"
          % (e.X.get(R6 + 0x11, 0), e.X.get(R6 + 0x12, 0),
             e.Y.get(R6 + 0x1B, 0), e.Y.get(R6 + 0x1C, 0)))
    print("  Y:$00-$1F:", ["%06X" % e.Y.get(a, 0) for a in range(0x20)])
    print("  X:$E0-$EF:", ["%06X" % e.X.get(a, 0) for a in range(0xE0, 0xF0)])
    print("  Y:$E0-$EF:", ["%06X" % e.Y.get(a, 0) for a in range(0xE0, 0xF0)])
    print("  X:$20-$2F:", ["%06X" % e.X.get(a, 0) for a in range(0x20, 0x30)])
    print("  X:$30-$3F:", ["%06X" % e.X.get(a, 0) for a in range(0x30, 0x40)])
    print("  out X:$00-$0F:", ["%06X" % e.X.get(a, 0) for a in range(0x10)])
    print("  page $18-$1F:", ["%06X/%06X" % (e.X.get(R6 + a, 0), e.Y.get(R6 + a, 0))
                              for a in range(0x18, 0x20)])
    print("  page $30-$33:", ["%06X" % e.Y.get(R6 + a, 0) for a in range(0x30, 0x34)])


BASE = [64, 64, 64, 64, 64, 64, 64, 64]
e = build()
ph = run4(e, BASE, 0.0)
dump(e, "base all=64")

for p in range(8):
    for v in (0, 127):
        knobs = list(BASE)
        knobs[p] = v
        e = build()
        for i, kv in enumerate(knobs):
            e.Y[R6 + 4 + i] = (int(kv) & 0xFFFF) << 16
        ph = run4(e, knobs, 0.0)
        dump(e, "param%d=%d" % (p, v))
