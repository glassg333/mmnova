#!/usr/bin/env python3
"""exp25b_modloop_probe.py — trace the mod-bank loop $1450CD-$1450D4:
   r1/r2/r3 trajectories, x1 after the loop, mem[$3E-$42]."""
import sys

sys.path.insert(0, "/home/z/my-project/scripts")
from exp23_m18_harness import build, run_init, R6M  # noqa

PROC = 0x145045


def main():
    e = build()
    run_init(e)
    prm = [96, 100, 16, 100, 40, 64, 64, 100]
    sig = [0x400000] + [0] * 33
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(prm[i])
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24 if (M24 := 0xFFFFFF) else v
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(0x14538D)
    e.run(PROC, end=0x14538D, max_steps=400000)

    # second block
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & 0xFFFFFF
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    e.run(PROC, end=0x1450CD, max_steps=400000)
    print("mod-loop entry: r1=%03X r2=%03X r3=%03X x0=%06X x1=%06X y0=%06X"
          % (e.R[1], e.R[2], e.R[3], e.x0, e.x1, e.y0))
    print("  mem[$20-$27]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0x20, 0x28)))
    print("  mem[$30-$33]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0x30, 0x34)))
    print("  mem[$3E-$42]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0x3E, 0x43)))

    # step 3 iterations manually
    for it in range(3):
        print(" iter %d start: r1=%03X x1=%06X x0=%06X" % (it, e.R[1], e.x1, e.x0))
        for pc in range(0x1450CF, 0x1450D5):
            e.step()
    print(" after 3 iters: r1=%03X x1=%06X" % (e.R[1], e.x1))
    print("  mem[$00-$05]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0, 6)))

    # run to rotator entry
    e.run(e.pc, end=0x145101, max_steps=10000)
    print("rotator entry: r1=%03X r2=%03X x1=%06X x0=%06X" % (e.R[1], e.R[2], e.x1, e.x0))
    print("  mem[$3E-$42]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0x3E, 0x43)))
    print("  mem[$24-$28]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0x24, 0x29)))
    print("  mem[$00-$03]:", " ".join("%06X" % e.X.get(a, 0) for a in range(0, 4)))


if __name__ == "__main__":
    main()
