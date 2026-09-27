#!/usr/bin/env python3
"""exp25_rotator_probe.py — instrument the m18 rotator loop $145101-$14511C:
   1. which memory the loop actually writes (X/Y diff around one pass)
   2. r1/r2/r3/r4/r5 trajectories, m-register values
   3. does the loop body include $14511C (store to r3)?
   Verdict feeds the bit-exact model (exp25_model.py).
"""
import sys, json

sys.path.insert(0, "/home/z/my-project/scripts")
from exp23_m18_harness import build, run_init, f  # noqa

P = 0x400
R6M = P + 0x28
PROC = 0x145045
M24 = 0xFFFFFF


def snap(e, xs, ys):
    return {"X": {a: e.X.get(a, 0) for a in xs},
            "Y": {a: e.Y.get(a, 0) for a in ys}}


def diff(before, after, label):
    print("---- %s ----" % label)
    for space in ("X", "Y"):
        b, a = before[space], after[space]
        ch = [(k, b[k], a[k]) for k in sorted(b) if b[k] != a.get(k)]
        if ch:
            print(" %s: %d words changed" % (space, len(ch)))
            lo = min(k for k, _, _ in ch)
            hi = max(k for k, _, _ in ch)
            print("   range: $%02X-$%02X" % (lo, hi))
            # group contiguous
            grp = [ch[0]]
            runs = []
            for item in ch[1:]:
                if item[0] == grp[-1][0] + 1:
                    grp.append(item)
                else:
                    runs.append(grp)
                    grp = [item]
            runs.append(grp)
            for g in runs:
                print("   $%02X-$%02X: %s" % (g[0][0], g[-1][0],
                      " ".join("%06X" % v for _, _, v in g[:8]) + ("..." if len(g) > 8 else "")))
        else:
            print(" %s: no change" % space)


def main():
    e = build()
    run_init(e)
    prm = [96, 100, 16, 100, 40, 64, 64, 100]
    sig = [0x400000] + [0] * 33

    # set params + input manually, run to loop entry of pass 1
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(prm[i])
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(0x14538D)

    # warm: one full block first so one-poles settle a bit
    e.run(PROC, end=0x14538D, max_steps=400000)

    # now second block, instrumented
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200

    e.run(PROC, end=0x145101, max_steps=400000)
    print("m-registers at rotator entry:")
    for i in range(8):
        print("  m%d = %08X" % (i, e.M[i]))
    print("n1=%08X n2=%08X" % (e.N[1], e.N[2]))
    print("regs: r0=%06X r1=%06X r2=%06X r3=%06X r4=%06X r5=%06X"
          % (e.R[0], e.R[1], e.R[2], e.R[3], e.R[4], e.R[5]))
    print("x0=%06X x1=%06X y0=%06X y1=%06X" % (e.x0, e.x1, e.y0, e.y1))

    rng = list(range(0, 0x100))
    before = snap(e, rng, rng)

    # run pass 1 inner loop (16 iterations) -> stop at $14511D (after loop)
    e.run(e.pc, end=0x14511D, max_steps=10000)
    after1 = snap(e, rng, rng)
    diff(before, after1, "pass 1 inner loop ($145103-$14511C x16)")
    print("after pass1: r1=%06X r2=%06X r3=%06X r4=%06X r5=%06X"
          % (e.R[1], e.R[2], e.R[3], e.R[4], e.R[5]))

    # r6 state area before writeback
    print("r6+$1E..$24 (X) before writeback:",
          " ".join("%06X" % e.X.get(R6M + i, 0) for i in range(0x1E, 0x25)))
    print("r6+$1E..$24 (Y) before writeback:",
          " ".join("%06X" % e.Y.get(R6M + i, 0) for i in range(0x1E, 0x25)))

    # writeback + pass2 setup: run to $145129 (pass-2 loop entry region)
    e.run(e.pc, end=0x145129, max_steps=10000)
    print("pass 2 entry regs: r0=%06X r5=%06X r3=%06X r4=%06X"
          % (e.R[0], e.R[5], e.R[3], e.R[4]))
    print("r6+$1E..$24 (X) after writeback:",
          " ".join("%06X" % e.X.get(R6M + i, 0) for i in range(0x1E, 0x25)))

    # pass 2 inner loop: it already ran between B and C; instead capture C
    # (writeback + pass-2 full) and diff against B
    e.run(e.pc, end=0x145129, max_steps=10000)
    after2 = snap(e, rng, rng)
    diff(after1, after2, "writeback + pass-2 (from $14511D to $145129)")
    print("after pass2: r1=%06X r2=%06X r3=%06X" % (e.R[1], e.R[2], e.R[3]))

    # finish block
    e.run(e.pc, end=0x14538D, max_steps=10000)
    out = [e.Y[0x100 + i] for i in range(16)]
    print("block output:", " ".join("%.4f" % f(v) for v in out[:8]))

    # sanity: r6 states after full block
    print("final r6+$1E..$24 (X):",
          " ".join("%06X" % e.X.get(R6M + i, 0) for i in range(0x1E, 0x25)))


def microtrace():
    """instruction-by-instruction register trace of iterations 0..2 of pass 1."""
    e = build()
    run_init(e)
    prm = [96, 100, 16, 100, 40, 64, 64, 100]
    sig = [0x400000] + [0] * 33
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(prm[i])
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(0x14538D)
    e.run(PROC, end=0x14538D, max_steps=400000)

    # second block
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    e.run(PROC, end=0x145101, max_steps=400000)

    LO, HI = 0x145103, 0x14511C
    it = 0
    while it < 3:
        pc = e.pc
        if pc < LO or pc > HI:
            e.step()
            continue
        if pc == LO:
            print("\n=== iteration %d enter ===" % it)
            print(" r0=%03X r1=%03X r2=%03X r3=%03X r4=%03X r5=%03X "
                  "x0=%06X x1=%06X y0=%06X y1=%06X a=%014X b=%014X"
                  % (e.R[0], e.R[1], e.R[2], e.R[3], e.R[4], e.R[5],
                     e.x0, e.x1, e.y0, e.y1, e.A, e.B))
            if it == 0:
                print(" X:$20-27 =", " ".join("%06X" % e.X.get(a, 0)
                                              for a in range(0x20, 0x28)))
                print(" Y:$00-03 =", " ".join("%06X" % e.Y.get(a, 0)
                                              for a in range(0, 4)))
                print(" Y:$90-92 =", " ".join("%06X" % e.Y.get(a, 0)
                                              for a in range(0x90, 0x93)))
                print(" X:$00-03 =", " ".join("%06X" % e.X.get(a, 0)
                                              for a in range(0, 4)))
        print("  %06X: %s" % (pc, e.prog[pc].strip() if hasattr(e.prog[pc], "strip") else e.prog[pc]))
        e.step()
        if pc == HI:
            print("   -> a=%014X b=%014X x1=%06X y1=%06X"
                  % (e.A, e.B, e.x1, e.y1))
            it += 1
    print("\nprog type at 145103:", type(e.prog.get(0x145103)), e.prog.get(0x145103))


if __name__ == "__main__":
    main()
