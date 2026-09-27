#!/usr/bin/env python3
"""exp24e_onpole.py — single-frame verification of the retune one-pole and
   the target table T(P$2A, LPQ); plus T1 skip threshold check."""
import sys

PACK = "/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack/05_emulator"
sys.path.insert(0, PACK)
from track_harness import build_track_emu, setup_track, run_frame, read_out  # noqa

P = 0x400
M24 = 0xFFFFFF


def q(v):
    return (int(v) & 0xFFFF) << 16


def L48(e, off):
    x, y = e.X[P + off], e.Y[P + off]
    v = (x << 24) | y
    return v - (1 << 48) if x & 0x800000 else v


def set_L48(e, off, v):
    e.X[P + off] = (v >> 24) & M24
    e.Y[P + off] = v & M24


def build(tempo=120 << 16, lpq=0):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, trig=1, tempo=tempo, lpq=lpq)
    e.X[0xC5] = 0x4000
    return e


def onepole(L, T):
    """0.9*L + 0.1*T in 48-bit with DSP fractional doubling semantics."""
    def mul48(k, v):
        # k = 24-bit frac ($0CCCCD=0.05, $733333=0.45); product<<1 then split
        p = (k * v) << 1  # full precision, then DSP keeps [23..70] bits
        return p
    a = mul48(0x0CCCCD, T) + mul48(0x733333, L)
    a1 = (a >> 23) & M24          # product alignment: bits 23..46 -> a1
    a0 = a & M24
    # emulate acc width 72bit? keep simple 56-bit-ish then wrap 48
    res = (a1 << 24) | a0
    return res


def probe_onepole():
    print("=== A. single-frame one-pole check ===")
    cases = [
        (0, 64 << 24), (1000 << 24, 2000 << 24), (-(1 << 35), 1 << 35),
        (64 << 24, 300 << 24), ((1 << 47) - (1 << 24), 0),
    ]
    for L, T in cases:
        e = build()
        set_L48(e, 0xCF, L)
        set_L48(e, 0xCD, T)
        e.X[P + 0xD0] = 0x4000
        e.X[0xC5] = 0x4000
        e.X[0xFC] = 0x4000   # keep global position high so gate stays open
        e.Y[P + 0x28] = 0
        run_frame(e)
        got = L48(e, 0xCF)
        exp = onepole(L & ((1 << 48) - 1), T & ((1 << 48) - 1))
        exps = exp - (1 << 48) if exp & (1 << 47) else exp
        print("L=%012X T=%012X got=%012X exp=%012X %s" % (
            L & ((1 << 48) - 1), T & ((1 << 48) - 1), got & ((1 << 48) - 1),
            exp & ((1 << 48) - 1), "OK" if (got & ((1 << 48) - 1)) == (exp & ((1 << 48) - 1)) else "MISMATCH"))


def probe_target_table():
    print("=== B. target T = f(P$2A, LPQ) (48-bit) ===")
    for lpq in (0, 32, 64, 96, 127):
        row = []
        for p2a in (16 << 16, 64 << 16, 127 << 16):
            e = build(lpq=lpq)
            e.Y[P + 0x2A] = p2a
            for i in range(3):
                e.Y[P + 0x28] = 0
                run_frame(e)
            row.append(L48(e, 0xCD))
        print("lpq=%3d:" % lpq, ["%012X" % (t & ((1 << 48) - 1)) for t in row])
    print("== linearity check P$2A (lpq=64):")
    e = build(lpq=64)
    base = None
    for p2a in (1 << 16, 2 << 16, 4 << 16, 8 << 16):
        e.Y[P + 0x2A] = p2a
        e.Y[P + 0x28] = 0
        run_frame(e)
        t = L48(e, 0xCD)
        if base is None:
            base = t
        print("P2A=%08X T=%012X ratio=%.4f" % (p2a, t & ((1 << 48) - 1), (t / base) if base else 0))


def probe_skip_threshold():
    print("=== C. T1 skip: threshold on S = (L_CF asr2).y ===")
    for Lv in (1 << 24, 2 << 24, 4 << 24, 8 << 24, 16 << 24, 64 << 24, 2048 << 24):
        e = build(lpq=64)
        set_L48(e, 0xCF, Lv)
        set_L48(e, 0xCD, Lv)     # freeze length
        e.X[P + 0xD0] = 0x4000
        e.X[0xFC] = 0x4000
        e.X[0xC5] = 0x4000
        e.Y[P + 0x28] = 0
        run_frame(e)
        taps_x = [e.X[0x90 + i] for i in (0, 8, 16)]
        taps_y = [e.Y[0x90 + i] for i in (0, 8, 16)]
        pub = [e.Y.get(0x4000 + i, 0) for i in (0, 1)]
        echo = [e.X.get(0x300 + i, 0) for i in (0, 1)]
        print("L=%13d tapsX=%s tapsY=%s pub=%s echo=%s" % (
            Lv, ["%06X" % t for t in taps_x], ["%06X" % t for t in taps_y],
            ["%06X" % t for t in pub], ["%06X" % t for t in echo]))


if __name__ == "__main__":
    probe_onepole()
    probe_target_table()
    probe_skip_threshold()
