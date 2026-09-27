#!/usr/bin/env python3
"""
exp18_fdn.py — Iteration 18 E1: bit-exact model of filter stage 2 (P:$0A5D-$0B49).

Stage 2 architecture (from decoder_v2 listing):
  L1 $0A5D-$0A82  rotation, angle = X:$144AC7[BOFS]        X:$20-3F -> Y:$62-81
  L2 $0A84-$0A9B  rotation, angle = X:$144AC7[WOFS+BOFS]   Y:$62-81 -> Y:$20-3F
  L3 $0A9D-$0AB5  rotation, angle = X:$144B48[phase>>17]   X:$00-0F -> Y:$62-81
  DP $0AB7-$0AD0  depth = 4*(ATK-0.5)^2, KILL gate phase==4,
                  coeff banks X:$40-4F (b) and X:$51-60 (a) from Y:$00-0F
Frame verification with breakpoints: model must reproduce emulator banks
word-exactly for every stage and frame across a parameter grid.
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase, sgn, inject_sine_tables, load_xy_memory
from track_harness import build_track_emu, setup_track

M24 = 0xFFFFFF
M56 = (1 << 56) - 1
P = 0x400


def s24(v):
    v &= M24
    return v - (1 << 24) if v >> 23 else v


def s56(v):
    v &= M56
    return v - (1 << 56) if v >> 55 else v


def acc_from_mem(v):
    """load 24-bit cell into accumulator (A1, sign-extended above, zeros below)"""
    return s24(v) << 16


def store24(acc):
    return (acc >> 16) & M24


def mac(acc, x, y):
    return s56(acc + ((s24(x) * s24(y)) << 1))


def mpy(x, y):
    return s56((s24(x) * s24(y)) << 1)


def asr(acc, n):
    return s56(acc) >> n


def asl1(acc):
    return s56(acc << 1)


SNAP_XY = (list(range(0x00, 0x22)) + list(range(0x20, 0x62)) +
           list(range(0x62, 0x82)) + list(range(0x90, 0xA1)) + [0xC4])


def snap(e):
    d = {}
    for sp in ("X", "Y"):
        mem = getattr(e, sp)
        for a in SNAP_XY:
            d["%s:%03X" % (sp, a)] = mem.get(a, 0) & M24
    for a in range(0x00, 0x100):
        d["Y:P+%02X" % a] = e.Y.get(P + a, 0) & M24
        d["X:P+%02X" % a] = e.X.get(P + a, 0) & M24
    return d


class BPCase(EnvCase):
    def frame_bp(self, trig=0):
        e = self.e
        e.Y[P + 0x28] = trig
        e.run(0x0100, end=0x02EB, max_steps=500000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.Y[0x100 + i] = v & M24
            self.phase += 2 * math.pi / 32.0
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.X[i] = v & M24
            e.Y[i] = v & M24
            self.phase += 2 * math.pi / 32.0
        hits = []
        e.run(0x02EC, end=0x0A5D, max_steps=500000)
        if e.pc != 0x0A5D:
            raise RuntimeError("stage2 not reached (pc=%04X)" % e.pc)
        s_in = snap(e)
        e.run(0x0A5D, end=0x0A84, max_steps=500000)
        s_l1 = snap(e)
        e.run(0x0A84, end=0x0A9D, max_steps=500000)
        s_l2 = snap(e)
        e.run(0x0A9D, end=0x0AB7, max_steps=500000)
        s_l3 = snap(e)
        e.run(0x0AB7, end=0x0AD1, max_steps=500000)
        s_dp = snap(e)
        e.run(0x0AD1, end=0x0B4C, max_steps=500000)
        s_out = snap(e)
        return s_in, s_l1, s_l2, s_l3, s_dp, s_out


# ------------------------------------------------------------- model
def curve(e, base, idx):
    return e.X.get(base + idx, 0) & M24


def model_L1(e, s_in):
    """returns dict of bank updates: Y:$62..$81, states y/x r7-$88, r7-$89"""
    YP = lambda o: s_in["Y:P+%02X" % o]
    XP = lambda o: s_in["X:P+%02X" % o]
    bofs = YP(0x16) >> 16
    c1 = curve(e, 0x144AC7, bofs & M24)
    acc_b = acc_from_mem(c1)
    acc_b = asr(acc_b, 1)                 # asr b
    y0 = acc_from_mem(c1)                 # y0 = old b (B1) = c1
    acc_b = s56(acc_b + (0x800000 << 16)) if False else s56(acc_b + acc_from_mem(0x800000))
    acc_b = -acc_b                        # neg
    y1 = acc_b                            # move b,y1  (acc as y1? y1 is 24-bit reg!)
    # NOTE: y1 is a 24-bit register: move b,y1 takes B1 = (acc>>16)&M24
    y1 = store24(acc_b)
    y0 = store24(y0)
    a = s_in["Y:P+%02X" % (0x4FA - 0x88 - P if False else 0)]  # placeholder
    return {}


def run_grid():
    """collect emulator snapshots for the grid; save to JSON"""
    configs = [
        # atk, dec, bofs, wofs
        (64, 64, 0, 0), (64, 64, 127, 0), (64, 64, 0, 127), (64, 64, 127, 127),
        (0, 64, 64, 64), (127, 64, 64, 64), (64, 0, 64, 64), (64, 127, 64, 64),
        (10, 100, 30, 90), (100, 10, 90, 30), (64, 64, 64, 64), (1, 1, 1, 1),
    ]
    nframes = 8
    out = []
    for (atk, dec, bofs, wofs) in configs:
        c = BPCase(filt_atk=atk, filt_dec=dec, bofs=bofs, wofs=wofs)
        for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                       (0x12, 64), (0x13, 64)):
            c.e.Y[P + off] = (int(v) & 0xFFFF) << 16
        frames = []
        for k in range(nframes):
            ss = c.frame_bp(trig=1 if k == 0 else 0)
            frames.append([{k2: v for k2, v in s.items()} for s in ss])
        out.append(dict(atk=atk, dec=dec, bofs=bofs, wofs=wofs, frames=frames))
        print("collected atk=%d dec=%d bofs=%d wofs=%d" % (atk, dec, bofs, wofs), flush=True)
    json.dump(out, open("/home/z/my-project/mining/exp18_fdn_snap.json", "w"))
    print("saved /home/z/my-project/mining/exp18_fdn_snap.json")


if __name__ == "__main__":
    run_grid()
