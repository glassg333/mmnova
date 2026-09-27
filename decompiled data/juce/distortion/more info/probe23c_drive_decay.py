#!/usr/bin/env python3
"""Iteration 23c: drive-env DEC rate vs y:$0D, and output-gain coupling.
DIST=0 -> fast attack -> DEC runs; watch lvl X:$4DB + master rms per frame."""
import sys, math, json
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import sgn, inject_sine_tables, load_xy_memory

P = 0x400

def run(dist=0, volparam=127, y0d=None, frames=24):
    e = build_track_emu(machine=14)
    inject_sine_tables(e); load_xy_memory(e)
    for a in range(0x2C0, 0x2CA):
        e.X[a] = 0
    setup_track(e, base=64, trig=0, vol=127, mach_params=[0.5] * 8)
    e.X[P + 0x0B] = 64
    e.Y[P + 0x0C] = (dist & 0xFFFF) << 16
    if y0d is not None:
        e.Y[P + 0x0D] = (y0d & 0xFFFF) << 16
    phase = 0.0
    rows = []
    for f in range(frames):
        e.Y[P + 0x28] = 1 if f == 0 else 0
        e.Y[P + 0x0C] = (dist & 0xFFFF) << 16
        if y0d is not None:
            e.Y[P + 0x0D] = (y0d & 0xFFFF) << 16
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
        out = [sgn(e.Y[i]) for i in range(0x20)]
        rows.append(dict(
            f=f, lvl=round(sgn(e.X.get(P + 0xDB, 0)) / 8388608, 5),
            phs=round(sgn(e.Y.get(P + 0xDB, 0)) / 8388608, 3),
            idx=hex(e.X.get(P + 0xD9, 0) & 0xFFFF),
            rms=round(math.sqrt(sum(v * v for v in out) / 32), 1),
            y0d_seen=(e.Y.get(P + 0x0D, 0) >> 16) & 0xFFFF,
        ))
    return rows

def main():
    res = {}
    for tag, kw in (("dist0_vol127", dict(dist=0, volparam=127)),
                    ("dist0_y0d0", dict(dist=0, y0d=0)),
                    ("dist0_y0d32", dict(dist=0, y0d=32)),
                    ("dist0_y0d64", dict(dist=0, y0d=64)),
                    ("dist0_y0d127", dict(dist=0, y0d=127))):
        rows = run(**kw)
        res[tag] = rows
        print("== %s ==" % tag)
        for r in rows[:14]:
            print("  f%02d lvl=%.5f phs=%.2f idx=%s rms=%.1f y0d=%d" %
                  (r["f"], r["lvl"], r["phs"], r["idx"], r["rms"], r["y0d_seen"]))
    json.dump(res, open("/home/z/my-project/mining/probe23c_drive_decay.json", "w"), indent=1)
    print("saved probe23c_drive_decay.json")

if __name__ == "__main__":
    main()
