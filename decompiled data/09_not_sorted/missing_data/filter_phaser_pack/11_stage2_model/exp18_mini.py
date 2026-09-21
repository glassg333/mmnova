#!/usr/bin/env python3
"""
exp18_mini.py — isolate stage-2 blocks as a mini-program on the bit-exact
emulator, fuzz with random inputs, and fit the closed-form recurrence.

Mini-program = exact disassembly of P:$0A5D-$0AD0 (decoder_v2 code_text),
executed standalone with r6/r7/memory prepared. This removes all frame
interference so semantics can be pinned down quickly.
"""
import sys, json, random
sys.path.insert(0, "/home/z/my-project/scripts")
from dsp_emu import DSP56300

M24 = 0xFFFFFF
P = 0x400
R7 = P + 0xFA


def load_prog(path="mining/decoder_v2/code_full.asm", lo=0x0A5D, hi=0x0AD1):
    lines = []
    for l in open(path, errors="ignore"):
        try:
            a = int(l[:6], 16)
        except ValueError:
            continue
        if lo <= a < hi:
            lines.append(l.rstrip("\n"))
    return lines


def s24(v):
    v &= M24
    return v - (1 << 24) if v >> 23 else v


def build(lo=0x0A5D, hi=0x0AD1):
    lines = load_prog(lo=lo, hi=hi)
    e = DSP56300(lines)
    return e, lines


def setup(e, rnd, curves=None):
    """random environment: r6/r7, page cells, banks, curve tables"""
    e.set_reg("r6", P)
    e.set_reg("r7", R7)
    e.set_reg("r5", 0x62)          # not used before set in code? safe
    e.set_reg("m0", 0xFFFFFF); e.set_reg("m1", 0xFFFFFF)
    e.set_reg("m2", 0xFFFFFF); e.set_reg("m3", 0xFFFFFF)
    e.set_reg("m4", 0xFFFFFF); e.set_reg("m5", 0xFFFFFF)
    e.set_reg("m6", 0xFFFFFF); e.set_reg("m7", 0xFFFFFF)
    # params
    e.Y[P + 0x14] = (rnd.randrange(128) << 16)          # ATK
    e.Y[P + 0x15] = (rnd.randrange(128) << 16)          # DEC
    e.Y[P + 0x16] = (rnd.randrange(128) << 16)          # BOFS
    e.Y[P + 0x17] = (rnd.randrange(128) << 16)          # WOFS
    # stack temps (states)
    for off in (0x69, 0x6A, 0x6D, 0x71, 0x72, 0x73, 0x76):
        e.X[P + off] = rnd.randrange(M24 + 1)
        e.Y[P + off] = rnd.randrange(M24 + 1)
    e.X[P + 0x76] = rnd.choice([0, 1, 2, 3, 4, 5])       # phase (KILL gate)
    # banks
    for a in list(range(0x00, 0x20)) + list(range(0x20, 0x62)) + list(range(0x62, 0x82)):
        e.X[a] = rnd.randrange(M24 + 1)
        e.Y[a] = rnd.randrange(M24 + 1)
    e.Y[0xC4] = rnd.randrange(M24 + 1)
    # curves
    if curves is None:
        curves = {}
        for i in range(0x200):
            curves[i] = rnd.randrange(M24 + 1)
    for i, v in curves.items():
        e.X[0x144AC7 + i] = v
        e.X[0x144B48 + i] = v
    return e


def main():
    rnd = random.Random(1818)
    e, lines = build()
    print("mini program: %d instructions" % len(lines))
    for l in lines[:6]:
        print("   ", l)
    setup(e, rnd)
    n = e.run(0x0A5D, end=0x0AD1, max_steps=200000)
    print("ran %d steps, pc=%04X" % (n, e.pc))
    # dump resulting banks
    out = {}
    for a in list(range(0x20, 0x62)) + list(range(0x62, 0x82)):
        out["X:%03X" % a] = e.X.get(a, 0) & M24
        out["Y:%03X" % a] = e.Y.get(a, 0) & M24
    for off in (0x69, 0x6A, 0x6D, 0x71, 0x72, 0x73, 0x76):
        out["X:P+%02X" % off] = e.X.get(P + off, 0) & M24
        out["Y:P+%02X" % off] = e.Y.get(P + off, 0) & M24
    json.dump(out, open("/home/z/my-project/mining/exp18_mini_out.json", "w"), indent=1)
    print("saved; sample: Y:062=%06X X:040=%06X X:051=%06X" % (
        out["Y:062"], out["X:040"], out["X:051"]))


if __name__ == "__main__":
    main()
