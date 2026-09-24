#!/usr/bin/env python3
"""
exp_f2f.py — DECISIVE Ф2 test: snapshot the ENTIRE kernel state right BEFORE
the delay section ($0939) — if P+$14..$17 extremes leave it bit-identical,
the env params act ONLY in the delay/master stage and the FILTER is env-free.
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import inject_sine_tables, load_xy_memory

P = 0x400

def snap(e, lo=0, hi=0x200):
    s = {}
    for a in range(lo, hi):
        s["X%03X" % a] = e.X.get(a, 0)
        s["Y%03X" % a] = e.Y.get(a, 0)
    for a in range(0x100):
        s["XP%02X" % a] = e.X.get(P + a, 0)
        s["YP%02X" % a] = e.Y.get(P + a, 0)
    return s

def set_param(e, off, v):
    e.Y[P + off] = (int(v) & 0xFFFF) << 16

def run_state(name, nframes=14, **kw):
    base = kw.pop("base", 64)
    e = build_track_emu(machine=1)
    inject_sine_tables(e)
    load_xy_memory(e)
    for a in range(0x2C0, 0x2CA):
        e.X[a] = 0
    e.X[0xFF] = 0
    setup_track(e, base=base, trig=0, amp_atk=0, amp_dec=127, tempo=120, vol=127)
    set_param(e, 0x05, 127)
    set_param(e, 0x06, 64)
    for off, key in ((0x11, "wdth"), (0x12, "hpq"), (0x13, "lpq")):
        if key in kw:
            set_param(e, off, kw[key])
    for off, key in ((0x14, "filt_atk"), (0x15, "filt_dec"), (0x16, "bofs"), (0x17, "wofs")):
        if key in kw:
            set_param(e, off, kw[key])
    last = None
    for k in range(nframes):
        e.Y[P + 0x28] = 1 if k == 0 else 0
        e.run(0x0100, end=0x02EB, max_steps=500000)
        e.X[0x2C9] = 0x300
        ph = getattr(run_state, "_ph", 0.0) + 2 * math.pi / 32.0 * 0x22
        run_state._ph = ph
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(ph + i * 2 * math.pi / 32.0)))
            e.Y[0x100 + i] = v & 0xFFFFFF
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(ph + i * 2 * math.pi / 32.0)))
            e.X[i] = v & 0xFFFFFF
            e.Y[i] = v & 0xFFFFFF
        e.run(0x02EC, end=0x0939, max_steps=500000)   # UP TO the delay section
        last = snap(e)
        e.run(0x0939, end=0x0B4C, max_steps=500000)   # finish the frame
    return last

def main():
    ref = run_state("baseline")
    tests = [
        ("FATK=127",  dict(filt_atk=127)),
        ("FATK=0",    dict(filt_atk=0)),
        ("FDEC=127",  dict(filt_dec=127)),
        ("BOFS=127",  dict(bofs=127)),
        ("BOFS=0",    dict(bofs=0)),
        ("WOFS=127",  dict(wofs=127)),
        ("WOFS=0",    dict(wofs=0)),
        ("BASE=90+",  dict(base=90)),      # positive control MUST differ
        ("HPQ=120+",  dict(hpq=120)),      # positive control
    ]
    out = {}
    for name, kw in tests:
        st = run_state(name, **kw)
        diffs = [k for k in ref if ref[k] != st.get(k)]
        print("%-9s: %d cells differ pre-delay" % (name, len(diffs)))
        for k in diffs[:16]:
            print("    %s: %06X -> %06X" % (k, ref[k], st[k]))
        out[name] = {"ndiff": len(diffs), "diffs": {k: [ref[k], st[k]] for k in diffs[:64]}}
    json.dump(out, open("/home/z/my-project/mining/exp_f2f.json", "w"), indent=1, default=str)
    print("saved /home/z/my-project/mining/exp_f2f.json")

if __name__ == "__main__":
    main()
