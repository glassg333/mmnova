#!/usr/bin/env python3
"""
exp_f2e.py — FINAL audio-level verdict for Ф2 (filter env) using the proven
live-chain harness (amp_env_measure.EnvCase: sine injected into the machine
output bus, real X/Y memory images, pinned OS pointers).

Tests:
  A. Positive controls: BASE sweep, WDTH sweep  -> output MUST change.
  B. P+$14..$17 (alleged FILT ATK/DEC/BOFS/WOFS) extremes -> output identical?
  C. Where do $14..$17 land instead: fingerprint the DELAY-side cells
     (Y:$10-$1F ramp bank, X:$40-$5E coeff banks, page P+$C8..$D8).
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase

P = 0x400

def set_param(e, off, v):
    """16-bit param in upper half of the 24-bit word (page convention)."""
    e.Y[P + off] = (int(v) & 0xFFFF) << 16

def run_case(name, nframes=16, base=64, wdth=64, hpq=64, lpq=64, **kw):
    c = EnvCase(**kw)
    e = c.e
    set_param(e, 0x10, base)
    set_param(e, 0x11, wdth)
    set_param(e, 0x12, hpq)
    set_param(e, 0x13, lpq)
    set_param(e, 0x05, 127)   # VOL per voice_page_map fix #3 (setup_track writes $0D = stale)
    set_param(e, 0x06, 64)    # PAN center
    outs = []
    for k in range(nframes):
        r = c.frame(trig=1 if k == 0 else 0)
        outs.append(r["out"])
    h = 0
    energy = 0
    for blk in outs:
        for v in blk:
            h = (h * 1000003 ^ (v & 0xFFFFFF)) & 0xFFFFFFFFFFFFFFFF
            energy += abs(v)
    # delay-side fingerprint (after last frame)
    d = 0
    for a in range(0x10, 0x20):
        d = (d * 1000003 ^ e.Y[a]) & 0xFFFFFFFFFFFFFFFF
    for a in range(0x40, 0x60):
        d = (d * 1000003 ^ e.X[a]) & 0xFFFFFFFFFFFFFFFF
    for off in range(0xC8, 0xDA):
        d = (d * 1000003 ^ e.Y[P + off]) & 0xFFFFFFFFFFFFFFFF
        d = (d * 1000003 ^ e.X[P + off]) & 0xFFFFFFFFFFFFFFFF
    print("%-22s audio=%016X e=%11d  delayfp=%016X" % (name, h, energy, d))
    return {"audio": "%016X" % h, "energy": energy, "delayfp": "%016X" % d}

def main():
    res = {}
    tests = [
        ("BASE=64 (ctrl)  ", dict(base=64)),
        ("BASE=90 (ctrl)  ", dict(base=90)),
        ("BASE=40 (ctrl)  ", dict(base=40)),
        ("WDTH=90 (ctrl)  ", dict(base=64, wdth=90)),
        ("HPQ=90  (ctrl)  ", dict(base=64, hpq=90)),
        ("LPQ=90  (ctrl)  ", dict(base=64, lpq=90)),
        ("FATK=0          ", dict(base=64, filt_atk=0)),
        ("FATK=127        ", dict(base=64, filt_atk=127)),
        ("FDEC=0          ", dict(base=64, filt_dec=0)),
        ("FDEC=127        ", dict(base=64, filt_dec=127)),
        ("BOFS=0          ", dict(base=64, bofs=0)),
        ("BOFS=127        ", dict(base=64, bofs=127)),
        ("WOFS=0          ", dict(base=64, wofs=0)),
        ("WOFS=127        ", dict(base=64, wofs=127)),
        ("ALL $14-17 max  ", dict(base=64, filt_atk=127, filt_dec=127, bofs=127, wofs=127)),
    ]
    for name, kw in tests:
        res[name.strip()] = run_case(name, **kw)
    print("\n=== identical audio groups ===")
    groups = {}
    for n, v in res.items():
        groups.setdefault(v["audio"], []).append(n)
    for h, names in groups.items():
        print("audio %s: %s" % (h, " | ".join(names)))
    print("\n=== identical delay-side groups ===")
    groups = {}
    for n, v in res.items():
        groups.setdefault(v["delayfp"], []).append(n)
    for h, names in groups.items():
        print("delay %s: %s" % (h, " | ".join(names)))
    json.dump(res, open("/home/z/my-project/mining/exp_f2e.json", "w"), indent=1)
    print("saved /home/z/my-project/mining/exp_f2e.json")

if __name__ == "__main__":
    main()
