#!/usr/bin/env python3
"""
exp_f2c.py — Ф2 with the FULL track frame ($0100..$0B4C) and a live machine
(GND-SIN), so the filter-env section has real targets. Sweeps env params and
traces writes to find the env footprint:
  - raw coefficient ring Y:$04-07 (write at $0599)
  - tap pairs L:$80-$90 / L:$90-$9D
  - ramp banks X/Y:$20-$7D, states page+$C8-$D8
  - output bus
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from dsp_emu import EmuError

R6 = 0x400

def s2f(v):
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0

class Tracer:
    def __init__(self, emu):
        self.emu = emu
        self.log = []
        self.on = False
        self._wr = emu.wr
        self._wrL = emu.wrL
        emu.wr = self._h_wr
        emu.wrL = self._h_wrL
    def _h_wr(self, space, ea, val):
        if self.on and space in ("x", "y"):
            self.log.append((self.emu.pc, space.upper(), ea, val & 0xFFFFFF))
        return self._wr(space, ea, val)
    def _h_wrL(self, ea, v48):
        if self.on:
            self.log.append((self.emu.pc, "LX", ea, (v48 >> 24) & 0xFFFFFF))
            self.log.append((self.emu.pc, "LY", ea, v48 & 0xFFFFFF))
        return self._wrL(ea, v48)
    def start(self):
        self.on = True; self.log = []
    def stop(self):
        self.on = False

def snap(e):
    out = {}
    for sp, M in (("X", e.X), ("Y", e.Y)):
        for a in range(0x100):
            out["%s:%02X" % (sp, a)] = M.get(a, 0)
        for a in range(R6, R6 + 0x100):
            out["%s:P%02X" % (sp, a - R6)] = M.get(a, 0)
    return out

def run_config(**kw):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, trig=1, amp_dec=127, **kw)
    tr = Tracer(e)
    n_ok = 0
    for fr in range(10):
        try:
            if fr == 9:
                tr.start()
            e.run(0x0100, end=0x0B4C, max_steps=900000)
            n_ok += 1
        except EmuError as ex:
            print("  frame %d ERR: %s" % (fr, ex))
            break
        e.Y[R6 + 0x28] = 0   # no retrigger after first frame
    tr.stop()
    return e, tr, n_ok

def coef_writes(trlog):
    """raw coefficient values written at $0599-$059A (and L-pair taps)."""
    y04 = y05 = y06 = y07 = None
    for (pc, sp, ea, v) in trlog:
        if pc == 0x0599 and sp == "Y" and ea == 4: y04 = v
        if pc == 0x0599 and sp == "Y" and ea == 5: y05 = v
        if pc == 0x059A and sp == "Y" and ea == 6: y06 = v
        if pc == 0x059A and sp == "Y" and ea == 7: y07 = v
    return y04, y05, y06, y07

CFGS = [
    ("baseline ", dict(bofs=0.5, wofs=0.5, filt_atk=0.5, filt_dec=0.5)),
    ("BOFS=1.0 ", dict(bofs=1.0, wofs=0.5, filt_atk=0.5, filt_dec=0.5)),
    ("BOFS=0.0 ", dict(bofs=0.0, wofs=0.5, filt_atk=0.5, filt_dec=0.5)),
    ("WOFS=1.0 ", dict(bofs=0.5, wofs=1.0, filt_atk=0.5, filt_dec=0.5)),
    ("WOFS=0.0 ", dict(bofs=0.5, wofs=0.0, filt_atk=0.5, filt_dec=0.5)),
    ("ATK=1.0  ", dict(bofs=1.0, wofs=0.5, filt_atk=1.0, filt_dec=0.5)),
    ("ATK=0.0  ", dict(bofs=0.0, wofs=0.5, filt_atk=0.0, filt_dec=0.5)),
    ("DEC=0.0  ", dict(bofs=1.0, wofs=0.5, filt_atk=1.0, filt_dec=0.0)),
    ("DEC=1.0  ", dict(bofs=1.0, wofs=0.5, filt_atk=1.0, filt_dec=1.0)),
]

def main():
    res = {}
    base_snap = None
    for name, kw in CFGS:
        e, tr, n_ok = run_config(**kw)
        y04, y05, y06, y07 = coef_writes(tr.log)
        s = snap(e)
        if base_snap is None:
            base_snap = s if name.startswith("baseline") else None
        # ramp/tap fingerprints
        taps80 = [s["X:P%02X" % a] if False else s.get("X:%02X" % a, 0) for a in range(0x80, 0x91)]
        ramp = [s.get("Y:%02X" % a, 0) for a in range(0x62, 0x72)]
        out = [s.get("Y:%02X" % a, 0) for a in range(0x00, 0x10)]
        res[name.strip()] = {
            "frames": n_ok, "rawY04": y04, "rawY05": y05, "rawY06": y06, "rawY07": y07,
            "tapsX80": taps80[:6], "rampY62": ramp[:6], "outY00": out[:6],
            "snap": s,
        }
        print("%s frames=%d rawY04=%s rampY62[0]=%s outY00[0]=%s" %
              (name, n_ok,
               None if y04 is None else "%.6f" % s2f(y04),
               ("%06X" % ramp[0]) if ramp else "-",
               ("%06X" % out[0]) if out else "-"))
    # diff vs baseline
    bl = res.get("baseline")
    if bl:
        print("=== diff vs baseline (kernel banks + page) ===")
        for name in res:
            if name == "baseline":
                continue
            diffs = []
            for k in bl["snap"]:
                if bl["snap"][k] != res[name]["snap"][k]:
                    diffs.append((k, bl["snap"][k], res[name]["snap"][k]))
            print("%s: %d cells differ" % (name, len(diffs)))
            for k, v1, v2 in diffs[:24]:
                print("   %s: %06X -> %06X" % (k, v1, v2))
    json.dump({n: {k: v for k, v in res[n].items() if k != "snap"} for n in res},
              open("/home/z/my-project/mining/exp_f2c.json", "w"), indent=1, default=str)
    print("saved /home/z/my-project/mining/exp_f2c.json")

if __name__ == "__main__":
    main()
