#!/usr/bin/env python3
"""Iteration 23b: DIST element measurements on machine m14 (SWAVE-ENS),
which arms the real DIST handler $145C48 via its CONF every frame.
Chain: inject sine -> F3 timbre -> resonance -> DIST handler -> stage2 -> Y:$00-$1F.
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import sgn, inject_sine_tables, load_xy_memory

P = 0x400

class M14Case:
    def __init__(self, dist=0, vol=127, amp=0.9, mparams=(0.5,)*8):
        self.e = build_track_emu(machine=14)
        inject_sine_tables(self.e)
        load_xy_memory(self.e)
        for a in range(0x2C0, 0x2CA):
            self.e.X[a] = 0
        self.e.X[0xFF] = 0
        setup_track(self.e, base=64, trig=0, vol=vol, mach_params=list(mparams))
        self.e.X[P + 0x0B] = 64          # EQG raw divisor (as EnvCase; snapshot $FFFECF kills the path)
        self.e.Y[P + 0x0C] = (dist & 0xFFFF) << 16   # DIST knob
        self.phase = 0.0
        self.amp = amp
        self.dist = dist

    def frame(self, trig=0):
        e = self.e
        e.Y[P + 0x28] = trig
        e.Y[P + 0x0C] = (self.dist & 0xFFFF) << 16   # keep DIST knob pinned
        e.run(0x0100, end=0x02EB, max_steps=800000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            v = int(round(self.amp * 8388607 * math.sin(self.phase)))
            e.Y[0x100 + i] = v & 0xFFFFFF
            self.phase += 2 * math.pi / 32.0
        for i in range(16):
            v = int(round(self.amp * 8388607 * math.sin(self.phase)))
            e.X[i] = v & 0xFFFFFF
            e.Y[i] = v & 0xFFFFFF
            self.phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0B4C, max_steps=800000)
        return dict(
            out=[sgn(e.Y[i]) for i in range(0x20)],
            lvl=sgn(e.X.get(P + 0xDB, 0)),
            phs=sgn(e.Y.get(P + 0xDB, 0)),
            fade=sgn(e.Y.get(P + 0x34, 0)),
            slot=e.X.get(P + 0x0C, 0),
            knob=e.Y.get(P + 0x0C, 0),
        )

def rms(xs):
    return math.sqrt(sum(x * x for x in xs) / max(len(xs), 1))

def fft_mag(xs):
    n = len(xs)
    import cmath
    out = []
    for k in range(n // 2):
        s = sum(xs[t] * cmath.exp(-2j * math.pi * k * t / n) for t in range(n))
        out.append(abs(s) / n)
    return out

def run_case(dist, frames=60, amp=0.9):
    c = M14Case(dist=dist, amp=amp)
    recs = []
    err = None
    for f in range(frames):
        try:
            recs.append(c.frame(trig=1 if f == 0 else 0))
        except Exception as ex:
            err = "%s @ %06X" % (ex, c.e.pc)
            break
    return recs, err

def main():
    res = {"probe": "23b", "date": "2026-09-22"}
    # A: DIST sweep, steady-state last 8 frames
    A = {}
    for dist in (0, 32, 64, 96, 127):
        recs, err = run_case(dist)
        if not recs:
            A[dist] = {"err": err}
            continue
        tail = recs[-8:]
        wave = tail[-1]["out"][:16]
        A[dist] = {
            "err": err,
            "slot": hex(recs[0]["slot"]),
            "knob_kept": recs[-1]["knob"] >> 16,
            "fade_last": round(tail[-1]["fade"], 4),
            "lvl_last": round(tail[-1]["lvl"], 5),
            "rms_out": round(rms([v for r in tail for v in r["out"]]), 5),
            "peak_out": round(max(abs(v) for r in tail for v in r["out"]) / 8388608, 5),
            "wave16": [round(v / 8388608, 5) for v in wave],
        }
    res["A_dist_sweep_m14"] = A
    for d, a in A.items():
        if "err" in a and a.get("err"):
            print("DIST=%3d ERR %s" % (d, a["err"]))
            continue
        print("DIST=%3d slot=%s rms=%.5f peak=%.5f fade=%s lvl=%s wave[0:6]=%s" %
              (d, a.get("slot"), a["rms_out"], a["peak_out"], a.get("fade_last"),
               a.get("lvl_last"), a.get("wave16", [])[0:6]))

    # B: harmonics at DIST=0 vs 127 (128-sample capture)
    def spectrum(dist, frames=8):
        c = M14Case(dist=dist)
        for f in range(30):
            c.frame()
        cap = []
        for f in range(frames):
            r = c.frame()
            cap += r["out"][:16]
        m = fft_mag([v / 8388608 for v in cap])
        return m
    try:
        m0 = spectrum(0)
        m127 = spectrum(127)
        # fundamental = 128/32 = bin 4
        tops0 = sorted(range(len(m0)), key=lambda k: -m0[k])[:8]
        res["B_harmonics"] = {
            "dist0_top_bins": [(b, round(m0[b], 5)) for b in sorted(tops0)],
            "dist127_top_bins": [(b, round(m127[b], 5)) for b in sorted(
                range(len(m127)), key=lambda k: -m127[k])[:8]],
            "dist0_thd_approx": round(
                math.sqrt(sum(m0[b] ** 2 for b in range(1, 16) if b != 4)) /
                max(m0[4], 1e-9), 4),
            "dist127_thd_approx": round(
                math.sqrt(sum(m127[b] ** 2 for b in range(1, 16) if b != 4)) /
                max(m127[4], 1e-9), 4),
        }
        print("B:", json.dumps(res["B_harmonics"]))
    except Exception as ex:
        res["B_harmonics"] = {"err": str(ex)}
        print("B err:", ex)

    # C: amplitude linearity (transfer) DIST=0 vs 127
    C = {}
    for dist in (0, 127):
        C[dist] = {}
        for amp in (0.1, 0.3, 0.6, 0.9):
            recs, err = run_case(dist, frames=30, amp=amp)
            C[dist][amp] = round(rms([v for r in recs[-4:] for v in r["out"]]), 5) if recs else err
    res["C_linearity"] = C
    print("C:", json.dumps(C, indent=1))

    json.dump(res, open("/home/z/my-project/mining/probe23b_dist_m14.json", "w"), indent=1)
    print("saved /home/z/my-project/mining/probe23b_dist_m14.json")

if __name__ == "__main__":
    main()
