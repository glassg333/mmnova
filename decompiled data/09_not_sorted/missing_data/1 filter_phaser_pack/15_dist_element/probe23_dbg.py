#!/usr/bin/env python3
"""Debug: what makes master out Y:$00-$1F non-zero (iter23)."""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import sgn, inject_sine_tables, load_xy_memory

P = 0x400

def one(machine, eqg=64, conf_bit=False, dist=0, frames=3):
    e = build_track_emu(machine=machine)
    inject_sine_tables(e)
    load_xy_memory(e)
    for a in range(0x2C0, 0x2CA):
        e.X[a] = 0
    e.X[0xFF] = 0
    setup_track(e, base=64, trig=0, vol=127, mach_params=[0.5] * 8)
    e.X[P + 0x0B] = eqg
    e.Y[P + 0x0C] = (dist & 0xFFFF) << 16
    phase = 0.0
    for f in range(frames):
        tr = 1 if f == 0 else 0
        if conf_bit and f == 0:
            tr |= 0x80
        e.Y[P + 0x28] = tr
        e.Y[P + 0x0C] = (dist & 0xFFFF) << 16
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
    mout = [sgn(e.Y[0x4E6 + i]) for i in range(16)]
    mstr = [sgn(e.Y[i]) for i in range(0x20)]
    print("machine=%d eqg=%d conf_bit=%s dist=%d" % (machine, eqg, conf_bit, dist))
    print("  machine out rms:", round(math.sqrt(sum(v * v for v in mout) / 16), 1),
          " master rms:", round(math.sqrt(sum(v * v for v in mstr) / 32), 1))
    print("  master[:8]:", [round(v / 8388608, 4) for v in mstr[:8]])
    print("  slot x:$40C =", hex(e.X.get(P + 0x0C, 0)))
    return e

one(1, eqg=0)
one(1, eqg=64)
one(14, eqg=64, conf_bit=False)
one(14, eqg=64, conf_bit=True)
one(14, eqg=64, conf_bit=True, dist=127)
