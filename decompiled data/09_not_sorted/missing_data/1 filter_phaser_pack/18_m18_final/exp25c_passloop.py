#!/usr/bin/env python3
"""exp25c_passloop.py — trace the m18 pass loop: n6/r4 values, copy source,
   writeback target for pass 1 and pass 2."""
import sys

sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm  # noqa

PROC = 0x145045
INIT = 0x145036
R6M = 0x400 + 0x28
M24 = 0xFFFFFF


def build():
    e = build_emu()
    lines = disasm(0x144000, 0x2000)
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    return e


def main():
    e = build()
    # INIT
    e.R[6] = R6M
    e.R[7] = 0x100
    e.ret_stack.append(0x14538E)
    e.run(INIT, end=0x14538E, max_steps=10000)
    print("after INIT: x:$11=%06X (phase_hi)" % e.X.get(R6M + 0x11, 0))
    print("L:(r6+$1E..$2D) X:",
          " ".join("%06X" % e.X.get(R6M + i, 0) for i in range(0x1E, 0x2E)))

    # one block
    prm = [96, 100, 16, 100, 40, 64, 64, 100]
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(prm[i])
    sig = [0x400000] + [0] * 33
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    e.ret_stack.append(0x14538D)

    # single-step through the pass loop, logging key events
    e.run(PROC, end=0x1450EE, max_steps=400000)
    print("\nat pass-loop entry: n6=%06X" % e.N[6])
    hits = 0
    while hits < 2 and e.pc != 0x145129:
        pc = e.pc
        if pc == 0x1450F0:  # lua (pass setup)
            e.step()
            print("  pass setup: n6=%03X -> r4=%03X" % (e.N[6], e.R[4]))
        elif pc == 0x14511D:  # lua (writeback setup)
            e.step()
            print("  writeback:  n6=%03X -> r4=%03X  (r2=%03X)" % (e.N[6], e.R[4], e.R[2]))
        elif pc == 0x145122:  # n6 = $26
            e.step()
            print("  n6 := %03X" % e.N[6])
            hits += 1
        else:
            e.step()
    print("\nafter block 1:")
    print("L:(r6+$1E..$2D) X:",
          " ".join("%06X" % e.X.get(R6M + i, 0) for i in range(0x1E, 0x2E)))
    print("L:(r6+$1E..$2D) Y:",
          " ".join("%06X" % e.Y.get(R6M + i, 0) for i in range(0x1E, 0x2E)))
    print("X:$30-$36:", " ".join("%06X" % e.X.get(a, 0) for a in range(0x30, 0x37)))

    # second block: watch writeback targets again
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    e.run(PROC, end=0x1450EE, max_steps=400000)
    hits = 0
    while hits < 2 and e.pc != 0x145129:
        pc = e.pc
        if pc == 0x1450F0:
            e.step()
            print("  b2 pass setup: n6=%03X -> r4=%03X" % (e.N[6], e.R[4]))
        elif pc == 0x14511D:
            e.step()
            print("  b2 writeback:  n6=%03X -> r4=%03X  (r2=%03X)" % (e.N[6], e.R[4], e.R[2]))
        elif pc == 0x145122:
            e.step()
            hits += 1
        else:
            e.step()
    print("\nafter block 2:")
    print("L:(r6+$1E..$2D) X:",
          " ".join("%06X" % e.X.get(R6M + i, 0) for i in range(0x1E, 0x2E)))


if __name__ == "__main__":
    main()
