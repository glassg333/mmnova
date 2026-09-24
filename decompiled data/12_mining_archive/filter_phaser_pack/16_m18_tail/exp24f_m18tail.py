#!/usr/bin/env python3
"""exp24f_m18tail.py — verify m18 (FX-PHASER) tail $14509C-$14513F empirically:
   A. one-poles s18/s19 toward param0 (CNTR) / param1 (DEP), k=$28F5C
   B. MIX law (param3): out_even = (1-M)*dry + M*wet1 ; out_odd = (1-M)*wet2 + M*dry
   C. LFO rate vs SPD (output oscillation period -> Hz), SPD^2 law
   D. state layout dump r6+$18..$1A
"""
import sys, math

sys.path.insert(0, "/home/z/my-project/scripts")
from exp23_m18_harness import build, run_init, run_block, f, i2f, R6M  # noqa

P = 0x400
R6M = P + 0x28
M24 = 0xFFFFFF


def sgn(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v


def f(v):
    return sgn(v) / 8388608.0


def run_blocks(e, sig, prm, n):
    outs = []
    for i in range(n):
        outs.append(run_block(e, sig, prm))
    return outs


def f2i(x):
    v = int(round(x * 8388608.0))
    return v & M24


def sine_sig(phase=0.0, amp=0.5):
    return [f2i(amp * math.sin(phase + k * 0.3)) for k in range(34)]


def zero_sig():
    return [0] * 34


def probe_onepoles():
    print("=== A. one-poles s18/s19 vs param0/param1 ===")
    for p0, p1 in ((127, 0), (0, 127), (64, 64)):
        e = build()
        run_init(e)
        prm = [p0, p1, 0, 0, 0, 0, 0, 100]
        sig = zero_sig()
        traj18, traj19 = [], []
        for i in range(30):
            run_block(e, sig, prm)
            traj18.append(e.Y[R6M + 0x18])
            traj19.append(e.Y[R6M + 0x19])
        print("p0=%3d p1=%3d  s18: %s -> %06X   s19: %s -> %06X" % (
            p0, p1, ["%06X" % t for t in traj18[:6]], traj18[-1],
            ["%06X" % t for t in traj19[:6]], traj19[-1]))
        e2 = build(); run_init(e2)
        print("   init s18=%06X s19=%06X" % (e2.Y[R6M + 0x18], e2.Y[R6M + 0x19]))


def probe_mix():
    print("=== B. MIX law (param3), SPD=0 static angle ===")
    e = build(); run_init(e)
    prm_base = [96, 32, 0, 0, 64, 96, 0, 100]
    sig = sine_sig(0.0, 0.5)
    run_block(e, sig, prm_base)  # warm
    for m in (0, 32, 64, 96, 127):
        prm = list(prm_base)
        prm[3] = m
        o = run_block(e, sig, prm)
        print("MIX=%3d out[0..5] = %s" % (m, ["%.4f" % f(v) for v in o[:6]]))


def probe_lfo_rate():
    print("=== C. LFO rate vs SPD (DC input, out[0] oscillation, 4000 blocks) ===")
    for spd in (16, 32, 64, 96, 127):
        e = build(); run_init(e)
        prm = [96, 100, spd, 127, 0, 64, 0, 100]
        sig = [f2i(0.25)] * 34  # DC input: only the LFO can move the output
        seq = []
        for i in range(4000):
            o = run_block(e, sig, prm)
            seq.append(f(o[0]))
        zc = 0
        prev = seq[0]
        crosses = []
        for i, v in enumerate(seq[1:], 1):
            if prev < 0 <= v:
                zc += 1
                crosses.append(i)
            prev = v
        per = None
        if len(crosses) >= 3:
            spans = [b - a for a, b in zip(crosses, crosses[2:])]
            per = sum(spans) / len(spans) / 2.0
        hz = 1.0 / (per * 16.0 / 44100.0) if per else 0.0
        lo, hi = min(seq), max(seq)
        print("SPD=%3d zc=%4d period=%s blocks -> %.4f Hz   out range [%.4f, %.4f]" % (
            spd, zc, ("%.1f" % per) if per else "-", hz, lo, hi))


def probe_states():
    print("=== D. state cells r6+$11..$1D after 40 blocks (SPD=64) ===")
    e = build(); run_init(e)
    prm = [96, 40, 64, 64, 32, 96, 0, 100]
    sig = sine_sig(0.0, 0.5)
    for i in range(40):
        run_block(e, sig, prm)
    for off in (0x10, 0x11, 0x12, 0x13, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F):
        xv = e.X.get(R6M + off, 0)
        yv = e.Y.get(R6M + off, 0)
        print("  r6+%02X: X=%06X Y=%06X" % (off, xv, yv))


if __name__ == "__main__":
    probe_onepoles()
    probe_mix()
    probe_lfo_rate()
    probe_states()
