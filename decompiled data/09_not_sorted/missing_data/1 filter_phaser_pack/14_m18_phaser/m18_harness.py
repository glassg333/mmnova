#!/usr/bin/env python3
"""exp23_m18_harness.py — run the real phaser m18 (proc $145045) directly,
measure: output path, LFO rate vs SPD, impulse response -> notches."""
import sys, math, json

sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm  # noqa

P = 0x400
R6M = P + 0x28
PROC = 0x145045
INIT = 0x145036
PROC_END = 0x14538D

M24 = 0xFFFFFF


def build():
    e = build_emu()
    lines = disasm(0x144000, 0x2000)
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    return e


def set_params(e, p):
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(p[i])


def run_init(e):
    e.R[6] = R6M
    e.R[7] = 0x100
    e.ret_stack.append(0x14538E)
    e.run(INIT, end=0x14538E, max_steps=10000)


def run_block(e, sig, p):
    """one 16-sample block; sig = 34 words input at r0."""
    set_params(e, p)
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(0x14538D)
    e.run(PROC, end=0x14538D, max_steps=400000)
    out = [e.Y[0x100 + i] for i in range(16)]
    return out


def f(v):
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0


def i2f(v):
    return v / 8388608.0


if __name__ == "__main__":
    # ---------------- 1. static behaviour: impulse response ----------------
    print("=== m18 impulse response (SPD=0 => frozen LFO), CNTR/DEP sweep ===")
    results = {}
    for cntr in (32, 64, 96):
        for dep in (20, 64, 127):
            e = build()
            run_init(e)
            params = [cntr, dep, 0, 100, 40, 64, 64, 100]
            # impulse at sample 0 of the first block
            sig = [0x400000] + [0] * (16 + 18 - 1)
            out = run_block(e, sig, params)
            results["c%d_d%d" % (cntr, dep)] = [f(v) for v in out]
            print("cntr=%3d dep=%3d out:" % (cntr, dep),
                  " ".join("%+.4f" % f(v) for v in out[:8]))
    
    json.dump(results, open("/home/z/my-project/scripts/exp23_m18_ir.json", "w"))
    
    # ---------------- 2. LFO rate vs SPD ----------------
    print("\n=== LFO phase step vs SPD (param2) ===")
    for spd in (0, 16, 32, 64, 127):
        e = build()
        run_init(e)
        params = [64, 64, spd, 100, 40, 64, 64, 100]
        sig = [0] * 34
        run_block(e, sig, params)
        ph = (e.X.get(R6M + 0x11, 0) << 24) | e.X.get(R6M + 0x12, 0)
        print("spd=%3d phase after 1 block = %012X (hi=%06X lo=%06X)"
              % (spd, ph, e.X.get(R6M + 0x11, 0), e.X.get(R6M + 0x12, 0)))
    
    # ---------------- 3. depth banks vs DEP/param7 ----------------
    print("\n=== depth banks Y:$00-$1F vs param1 (dep) and param7 ===")
    for p1 in (0, 64, 127):
        for p7 in (0, 64, 127):
            e = build()
            run_init(e)
            params = [64, p1, 0, 100, 40, 64, 64, p7]
            sig = [0x400000] + [0] * 33
            run_block(e, sig, params)
            b = [i2f(e.Y.get(a, 0)) for a in range(0, 4)]
            print("dep=%3d p7=%3d Y:00-03:" % (p1, p7),
                  " ".join("%+.5f" % v for v in b))
