#!/usr/bin/env python3
"""svf_trace.py — bit-exact trace of the Monomachine SVF kernels:
  func_000340  cascade-1 SVF (inside 2x-oversample halfband loop), call sites $05A1/$05B5
  func_000397  cascade-2 resonant SVF, call sites $0938/$0978/$0A5A/$0B13
Runs the REAL track frame (machine GND-SIN -> filter chain) in the verified
DSP56300 emulator and captures:
  A) pre/post-call register+memory snapshots of every call (frames 2..3)
  B) instruction-level traces of selected calls (pc, A, B, xy, R, mem window)
Outputs JSON into /home/z/my-project/mining/ for the offline recursion fit.
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
import dsp_emu
from dsp_emu import DSP56300, MASK56
from amp_env_measure import EnvCase, P   # reuses harness builders

SITES_397 = [0x938, 0x978, 0xA5A, 0xB13]
SITES_340 = [0x5A1, 0x5B5]
ALL_SITES = SITES_397 + SITES_340
POST = {0x938: 0x939, 0x978: 0x979, 0xA5A: 0xA5B, 0xB13: 0xB14,
        0x5A1: 0x5A2, 0x5B5: 0x5B6}
POST_INV = {v: k for k, v in POST.items()}
ALL_POST = set(POST.values())
RNG = {0x938: (0x397, 0x3A9), 0x978: (0x397, 0x3A9),
       0xA5A: (0x397, 0x3A9), 0xB13: (0x397, 0x3A9),
       0x5A1: (0x340, 0x34F), 0x5B5: (0x340, 0x34F)}

def h56(v):
    v &= MASK56
    return "%014X" % v

class Tracer(DSP56300):
    """Adds call-site snapshotting + optional instruction-level window tracing."""
    def __init__(self, lines):
        super().__init__(lines)
        self.calls = []        # pre/post snapshots
        self.steplines = []    # instruction-level rows
        self.frame_no = 0
        self.arm = None        # active call site (step-trace target) or None
        self.active = None     # currently executing call site (for pairing)
        self.want_steps = set()  # call sites to instruction-trace
        self.snap_frames = (2, 3)

    def snap_mem(self):
        X = [self.X[a] for a in range(0x100)]
        Y = [self.Y[a] for a in range(0x100)]
        return X, Y

    def snapshot(self, kind, site):
        X, Y = self.snap_mem()
        return dict(kind=kind, site=site, frame=self.frame_no, pc=self.pc,
                    steps=self.steps,
                    R=list(self.R), N=list(self.N), M=list(self.M),
                    x0=self.x0, x1=self.x1, y0=self.y0, y1=self.y1,
                    a=h56(self.A), b=h56(self.B), X=X, Y=Y)

    def step(self):
        pc = self.pc
        if pc in ALL_SITES:
            self.active = pc
            self.arm = pc if pc in self.want_steps else None
            if self.arm is not None:
                lo, hi = RNG[pc]
                self.steplines.append(dict(site=pc, frame=self.frame_no,
                                           rows=[], lo=lo, hi=hi))
            if self.frame_no in self.snap_frames:
                self.calls.append(self.snapshot("pre", pc))
        elif pc in ALL_POST:
            site = POST_INV[pc]
            if self.frame_no in self.snap_frames:
                self.calls.append(self.snapshot("post", site))
            self.active = None
            self.arm = None
        if self.arm is not None:
            lo, hi = RNG[self.arm]
            if lo <= pc <= hi:
                X, Y = self.snap_mem()
                self.steplines[-1]["rows"].append(
                    dict(pc=pc, A=h56(self.A), B=h56(self.B),
                         x0=self.x0, x1=self.x1, y0=self.y0, y1=self.y1,
                         R=list(self.R), N=list(self.N),
                         X=X, Y=Y))
        super().step()

def sgn24(v):
    return v - 0x1000000 if v >= 0x800000 else v

def main():
    c = EnvCase(atk=0, dec=127, vol=127)   # steady signal, env wide open
    e = c.e
    # rewrap the live emulator into the tracing subclass (same object/state)
    e.__class__ = Tracer
    e.calls = []
    e.steplines = []
    e.frame_no = 0
    e.arm = None
    e.active = None
    e.want_steps = {0x938, 0x5A1}
    e.snap_frames = (2, 3)
    t = e
    c.e = t
    recs = []
    for k in range(4):
        r = c.frame(trig=1 if k == 0 else 0)
        recs.append(r)
        t.frame_no += 1
        print("frame %d: amp=%.5f lvl=%+0.4f calls=%d steps=%d" %
              (k, r["amp"] / 8388608.0, r["level"] / 8388608.0,
               len(t.calls), t.steps))
    out = "/home/z/my-project/mining/"
    # keep snapshots of frames 2..3 only (steady state)
    calls = [x for x in t.calls if x["frame"] in (2, 3)]
    json.dump(calls, open(out + "svf_trace_calls.json", "w"), indent=0)
    json.dump(t.steplines, open(out + "svf_trace_steps.json", "w"), indent=0)
    print("saved: svf_trace_calls.json (%d calls), svf_trace_steps.json (%d windows)"
          % (len(calls), len(t.steplines)))

    # ---- quick console summary of the first func_397 call (frame 2) ----
    pre = next(x for x in calls if x["kind"] == "pre" and x["site"] == 0x938)
    post = next(x for x in calls if x["kind"] == "post" and x["site"] == 0x938)
    print("\n=== func_397 @ $938 frame2 pre-call ===")
    print("R:", ["%06X" % v for v in pre["R"]])
    print("N:", ["%06X" % v for v in pre["N"]])
    print("M:", ["%06X" % v for v in pre["M"]])
    print("x1=%06X y0=%06X a=%s b=%s" % (pre["x1"], pre["y0"], pre["a"], pre["b"]))
    print("X:$80-$95:", " ".join("%06X" % v for v in pre["X"][0x80:0x96]))
    print("Y:$80-$95:", " ".join("%06X" % v for v in pre["Y"][0x80:0x96]))
    print("X:$20-$50:", " ".join("%06X" % v for v in pre["X"][0x20:0x50]))
    print("Y:$40-$60:", " ".join("%06X" % v for v in pre["Y"][0x40:0x60]))
    print("=== post ===")
    print("X:$20-$50:", " ".join("%06X" % v for v in post["X"][0x20:0x50]))
    print("X:$80-$95:", " ".join("%06X" % v for v in post["X"][0x80:0x96]))
    print("Y:$80-$95:", " ".join("%06X" % v for v in post["Y"][0x80:0x96]))
    # pointer cells X:$81-$88 as signed
    print("\npointer cells (X:$81-$88 as int):",
          [sgn24(v) for v in pre["X"][0x81:0x89]])
    print("coeff cells  (Y:$81-$88 as frac):",
          ["%.6f" % (sgn24(v) / 8388608.0) for v in pre["Y"][0x81:0x89]])

if __name__ == "__main__":
    main()
