#!/usr/bin/env python3
"""
exp_f2g.py — STERILE single-frame test: identical machine state, ONE frame,
snapshot at the delay-section entry ($0939). Any difference is caused within
the frame. Compares full $000-$1FF X/Y + voice page for env params.
"""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import inject_sine_tables, load_xy_memory

P = 0x400

def set_param(e, off, v):
    e.Y[P + off] = (int(v) & 0xFFFF) << 16

def build(**kw):
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
    m = {"wdth": 0x11, "hpq": 0x12, "lpq": 0x13, "filt_atk": 0x14,
         "filt_dec": 0x15, "bofs": 0x16, "wofs": 0x17}
    for k, off in m.items():
        if k in kw:
            set_param(e, off, kw[k])
    return e

def frame_to_0939(e, phase0):
    e.Y[P + 0x28] = 1
    e.run(0x0100, end=0x02EB, max_steps=500000)
    e.X[0x2C9] = 0x300
    for i in range(0x22):
        v = int(round(0.9 * 8388607 * math.sin(phase0 + i * 2 * math.pi / 32.0)))
        e.Y[0x100 + i] = v & 0xFFFFFF
    for i in range(16):
        v = int(round(0.9 * 8388607 * math.sin(phase0 + i * 2 * math.pi / 32.0)))
        e.X[i] = v & 0xFFFFFF
        e.Y[i] = v & 0xFFFFFF
    e.run(0x02EC, end=0x0939, max_steps=500000)

def snap(e):
    s = {}
    for a in range(0x200):
        s["X%03X" % a] = e.X.get(a, 0)
        s["Y%03X" % a] = e.Y.get(a, 0)
    for a in range(0x100):
        s["XP%02X" % a] = e.X.get(P + a, 0)
        s["YP%02X" % a] = e.Y.get(P + a, 0)
    return s

def main():
    tests = [
        ("baseline $14-17=0.5", {}),
        ("FATK=0",   dict(filt_atk=0)),
        ("FATK=127", dict(filt_atk=127)),
        ("FDEC=0",   dict(filt_dec=0)),
        ("FDEC=127", dict(filt_dec=127)),
        ("BOFS=0",   dict(bofs=0)),
        ("BOFS=127", dict(bofs=127)),
        ("WOFS=0",   dict(wofs=0)),
        ("WOFS=127", dict(wofs=127)),
        ("CTRL BASE=90", dict(base=90)),
        ("CTRL WDTH=120", dict(wdth=120)),
    ]
    snaps = {}
    for name, kw in tests:
        e = build(**kw)
        frame_to_0939(e, phase0=0.0)
        snaps[name] = snap(e)
        print("ran:", name)
    ref = snaps["baseline $14-17=0.5"]
    for name, st in snaps.items():
        if name.startswith("baseline"):
            continue
        diffs = [k for k in ref if ref[k] != st.get(k)]
        # classify
        coeff = [k for k in diffs if k in ("Y004", "Y005", "Y006", "Y007")]
        taps  = [k for k in diffs if k[1:3] in ("80", "81", "82", "83", "84", "85",
                 "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
                 "90", "91")]
        delayt= [k for k in diffs if k[1:3] in ("92", "93", "94", "95", "96", "97",
                 "98", "99", "9A", "9B", "9C", "9D", "9E", "9F")]
        print("\n%-20s ndiff=%d coeff-ring:%s taps$80-91:%d ($92-9F:%d)" %
              (name, len(diffs), coeff or "SAME", len(taps), len(delayt)))
        for k in diffs[:14]:
            print("    %s: %06X -> %06X" % (k, ref[k], st[k]))

if __name__ == "__main__":
    main()
