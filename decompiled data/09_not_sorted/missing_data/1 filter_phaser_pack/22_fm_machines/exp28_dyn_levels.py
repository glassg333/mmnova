#!/usr/bin/env python3
"""exp28_dyn_levels.py — m10 (real FM-DYN, $1461C1-$14636E): cascade level probe.

Checks open question (a): does the fixed LP #$40 (c = 64/2^23) silence the
modulators, and what is the real output magnitude of the pair-sum stage?
No bracket-m literals (gateway bug).
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_conf, s24, R6M, MACH, RTS

def mx(vals):
    return max(abs(v) for v in vals) if vals else 0

def grab(e):
    d = {}
    d["mix_raw X80"] = [s24(e.X.get(0x80 + i, 0)) for i in range(32)]
    d["mod1_lp Y80"] = [s24(e.Y.get(0x80 + i, 0)) for i in range(32)]
    d["mod2_lp YC0"] = [s24(e.Y.get(0xC0 + i, 0)) for i in range(32)]
    d["carrier XE0"] = [s24(e.X.get(0xE0 + i, 0)) for i in range(32)]
    d["rot2 X20"] = [s24(e.X.get(0x20 + i, 0)) for i in range(30)]
    d["out"] = [s24(e.Y.get(0x100 + i, 0)) for i in range(32)]
    return d

def run(nblocks=12, p=None, pitch=11776, lp_patch=None):
    if p is None:
        p = [100, 64, 127, 64, 64, 127, 0, 0]
    e = build()
    fm_init(e, 10)
    if lp_patch is not None:
        # patch 'move #$40,y0' (264000) at the three LP sites -> custom imm
        for site in (0x146233, 0x146297, 0x1462D3):
            # emulator stores parsed instruction objects; easiest: rewrite X? no -
            # patch PM word via e.pm if available
            pass
    for b in range(nblocks):
        fm_conf(e, 10, p)
        e.R[6] = R6M
        e.R[7] = 0x100
        for r in range(8):
            e.M[r] = 0xFFFFFF
        e.A = pitch & 0xFFFFFFFFFFFF
        e.ret_stack.append(0xDEAD)
        try:
            e.run(MACH[10]["proc"], end=RTS[10]["proc"], max_steps=2000000)
        except Exception as ex:
            if "00DEAD" not in str(ex):
                raise
        if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
            e.ret_stack.pop()
        d = grab(e)
        print("blk %2d  mix=%7d mod1lp=%7d mod2lp=%7d carr=%7d rot2=%7d OUT=%7d" % (
            b, mx(d["mix_raw X80"]) >> 6, mx(d["mod1_lp Y80"]) >> 6,
            mx(d["mod2_lp YC0"]) >> 6, mx(d["carrier XE0"]) >> 6,
            mx(d["rot2 X20"]) >> 6, mx(d["out"])))
    return e

if __name__ == "__main__":
    print("== default LP #$40 ==")
    run(12)
