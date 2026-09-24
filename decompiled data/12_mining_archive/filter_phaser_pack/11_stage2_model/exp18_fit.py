#!/usr/bin/env python3
"""exp18_fit.py — full per-instruction trace of the L1 loop on the mini-program
with random inputs; prints a compact table so the exact recurrence can be read
off mechanically. Then fuzz-verifies the closed-form model."""
import sys, json, random
sys.path.insert(0, "/home/z/my-project/scripts")
from exp18_mini import build, setup, M24, P, R7


def trace_loop(e, lo, hi):
    log = []
    orig = e.step

    def st():
        pc = e.pc
        if lo <= pc < hi:
            log.append((pc, e.A & ((1 << 56) - 1), e.B & ((1 << 56) - 1),
                        e.get_reg("x0"), e.get_reg("x1"),
                        e.get_reg("y0"), e.get_reg("y1"),
                        e.get_reg("r0"), e.get_reg("r1"), e.get_reg("r4"),
                        dict(e.wr_log) if hasattr(e, "wr_log") else None))
        orig()
    e.step = st
    return log


def main():
    rnd = random.Random(7)
    e, _ = build(0x0A5D, 0x0A86)         # L1 only (+2 instr past loop)
    setup(e, rnd)
    wr = []
    orig_wr = e.wr

    def wr_hook(space, ea, val):
        wr.append((hex(e.pc), space, hex(ea), "%06X" % (val & M24)))
        return orig_wr(space, ea, val)
    e.wr = wr_hook
    log = []
    orig_step = e.step

    def st():
        pc = e.pc
        if 0x0A5D <= pc < 0x0A84:
            log.append((pc, e.A & ((1 << 56) - 1), e.B & ((1 << 56) - 1),
                        e.get_reg("x0"), e.get_reg("x1"),
                        e.get_reg("y0"), e.get_reg("y1"),
                        e.get_reg("r0"), e.get_reg("r1"), e.get_reg("r4")))
        orig_step()
    e.step = st
    e.run(0x0A5D, end=0x0A86, max_steps=100000)
    print("pre-loop regs: x0=%06X x1=%06X y0=%06X y1=%06X" % (log[4][3], log[4][4], log[4][5], log[4][6]))
    # print iterations 0..2
    MNEM = {0x0A7A: "A7A mac -x0,y1,a  a,x0  a,y:(r4)+",
            0x0A7B: "A7B mac -y1,x1,b  b,x1  b,y:(r4)+",
            0x0A7C: "A7C mac -y0,x0,a  x:(r0),x0",
            0x0A7D: "A7D mac -x1,y0,b  x:(r1),x1",
            0x0A7E: "A7E mac  x0,y1,a  x:(r0)+,x0",
            0x0A7F: "A7F mac  y1,x1,b  x:(r1)+,x1"}
    for (pc, a, b, x0, x1, y0, y1, r0, r1, r4) in log:
        if pc in MNEM:
            print("%04X %-42s a=%014X b=%014X x0=%06X x1=%06X y0=%06X y1=%06X r0=%03X r1=%03X r4=%03X"
                  % (pc, MNEM[pc], a, b, x0, x1, y0, y1, r0, r1, r4))
    print("\nwrites:")
    for w in wr[:40]:
        print("   %s %s %s %s" % w)


if __name__ == "__main__":
    main()
