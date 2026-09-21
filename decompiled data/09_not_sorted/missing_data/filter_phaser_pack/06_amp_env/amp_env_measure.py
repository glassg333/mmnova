#!/usr/bin/env python3
"""amp_env_measure.py — bit-exact measurement of the Monomachine track AMP envelope.

Method: run the real track frame ($0100-$0B4C), inject a steady full-scale test
sine into the machine output bus (L:$0000-$000F) right where the machine is
called ($02EB), then watch:
  Y[P+$D7]        envelope level accumulator (r7-$23 at P:$088E, r7 = P+$FA)
  X[P+$D8]        envelope phase (0=ATK 1=HOLD 2=DEC 3=REL 4=KILL)
  Y[$0000-$001F]  master output block -> per-frame amplitude
"""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track

P = 0x400

def inject_sine_tables(e):
    """The sin/cos tables (X:$14A000 / X:$14A800, 8192 words each) lie beyond
    the pmem dump; use the math-recovered approximation (iteration-11)."""
    for i in range(8192):
        s = round(math.sin(2 * math.pi * i / 8192) * 0x7FFFFF) & 0xFFFFFF
        c = round(math.cos(2 * math.pi * i / 8192) * 0x7FFFFF) & 0xFFFFFF
        e.X[0x14A000 + i] = s
        e.Y[0x14A000 + i] = s
        e.X[0x14A800 + i] = c
        e.Y[0x14A800 + i] = c

MEMDIR = ("/home/z/my-project/work/mmnova/decompiled data/02_memory_images/")

def load_xy_memory(e, dsp=1):
    """Load the real X/Y memory readouts (kernel scratch + voice-page states,
    incl. SVF pointers X:$80-$8A and the DIST dispatch pointer X:P+$0C)."""
    for side, fn in (("X", "dsp1_xmem.bin"), ("Y", "dsp1_ymem.bin")):
        d = open(MEMDIR + fn, "rb").read()
        mem = e.X if side == "X" else e.Y
        for i in range(len(d) // 3):
            w = (d[3*i] << 16) | (d[3*i+1] << 8) | d[3*i+2]
            mem[i] = w

SR = 44100.0

TBL = ("/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/"
       "missing_data/filter_phaser_pack/01_filter/tables/")

def load_tbl(name):
    d = open(TBL + name, "rb").read()
    out = []
    for i in range(len(d) // 3):
        w = (d[3*i] << 16) | (d[3*i+1] << 8) | d[3*i+2]
        if w >= 0x800000:
            w -= 0x1000000
        out.append(w)
    return out

ATT = load_tbl("P_141800_env_shape_tbl_A.bin")   # attack increments (positive)
DEC = load_tbl("P_141880_env_shape_tbl_B.bin")   # decay/release factors (negative)

def sgn(v):
    return v - 0x1000000 if v >= 0x800000 else v

class EnvCase:
    def __init__(self, atk=32, hold=0, dec=90, rel=40, tempo=120, vol=127,
                 eqf=0, eqg=0, filt_atk=0, filt_dec=0, bofs=0, wofs=0):
        self.e = build_track_emu(machine=1)
        inject_sine_tables(self.e)
        load_xy_memory(self.e)
        # wipe stale OS buffer pointers from the snapshot (x:$2C0-$2C9):
        # in our harness the master X-writes must not land in the page
        for a in range(0x2C0, 0x2CA):
            self.e.X[a] = 0
        self.e.X[0xFF] = 0
        setup_track(self.e, base=64, trig=0, amp_atk=atk, amp_dec=dec,
                    tempo=tempo, vol=vol)
        Y = self.e.Y
        Y[P + 0x01] = (hold & 0xFFFF) << 16
        Y[P + 0x03] = (rel & 0xFFFF) << 16
        Y[P + 0x04] = (eqf & 0xFFFF) << 16          # EQF (kernel base r6=P, $07A6)
        self.e.X[P + 0x0B] = eqg if eqg else 64     # EQG raw divisor ($079F)
        Y[P + 0x14] = (filt_atk & 0xFFFF) << 16
        Y[P + 0x15] = (filt_dec & 0xFFFF) << 16
        Y[P + 0x16] = (bofs & 0xFFFF) << 16
        Y[P + 0x17] = (wofs & 0xFFFF) << 16
        self.phase = 0.0

    def frame(self, trig=0):
        e = self.e
        e.Y[P + 0x28] = trig
        e.run(0x0100, end=0x02EB, max_steps=500000)
        # The master-routing X-write pointer persists in x:$2C9 and walks
        # +16 per frame (the real OS re-arms it every frame). Pin it to
        # harmless scratch X:$300 so it never reaches the voice page.
        e.X[0x2C9] = 0x300
        # inject steady test sine (period 32 samples) into the machine output
        # area Y:$100-$121 ONLY (Y-side: that is what the machine itself
        # writes and what the $04F5 copy stage feeds to the chain).
        # NOTE: never touch X:$100-$121 - those are kernel SVF state cells!
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.Y[0x100 + i] = v & 0xFFFFFF
            self.phase += 2 * math.pi / 32.0
        # also the machine bus L:$0000-$000F (inter-machine routing)
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.X[i] = v & 0xFFFFFF
            e.Y[i] = v & 0xFFFFFF
            self.phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0B4C, max_steps=500000)
        return dict(
            level=sgn(e.Y[P + 0xD7]),
            phase=e.X[P + 0xD8],
            amp=max(abs(sgn(e.Y[i])) for i in range(0x20)),
            out=[sgn(e.Y[i]) for i in range(0x20)],
        )

def run_case(**kw):
    frames = kw.pop("nframes", 120)
    c = EnvCase(**kw)
    return [c.frame() for _ in range(frames)]

def show(rec, every=1, limit=None, show_out=False):
    rows = rec if limit is None else rec[:limit]
    for k, r in enumerate(rows):
        if k % every:
            continue
        extra = ""
        if show_out:
            extra = " out:" + " ".join("%+04X" % ((v >> 16) & 0xFFFF) for v in r["out"][:16])
        print("  f%3d ph=%d lvl=%06X (%+.5f) amp=%.5f%s" % (
            k, r["phase"], r["level"] & 0xFFFFFF,
            r["level"] / 8388608.0, r["amp"] / 8388608.0, extra))

if __name__ == "__main__":
    print("=== sanity: ATK=32 HOLD=0 DEC=90 REL=40 ===")
    rec = run_case(atk=32, dec=90, rel=40, nframes=40)
    show(rec, every=2, show_out=(rec[0]["amp"] == 0))
