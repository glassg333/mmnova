#!/usr/bin/env python3
"""exp18_trace.py — micro-trace of stage-2 loop L1 ($0A78-$0A82) from the emulator:
logs every instruction with full register/memory state to pin down exact semantics."""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from exp18_fdn import BPCase, M24, P, SNAP_XY, snap

LO, HI = 0x0A5D, 0x0A84

def main():
    c = BPCase(filt_atk=64, filt_dec=64, bofs=127, wofs=0)
    for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                   (0x12, 64), (0x13, 64)):
        c.e.Y[P + off] = (int(v) & 0xFFFF) << 16
    e = c.e
    c.frame_bp(trig=1)          # frame 0
    # ---- frame 1 with trace
    e.Y[P + 0x28] = 0
    e.run(0x0100, end=0x02EB, max_steps=500000)
    e.X[0x2C9] = 0x300
    for i in range(0x22):
        v = int(round(0.9 * 8388607 * math.sin(c.phase)))
        e.Y[0x100 + i] = v & M24
        c.phase += 2 * math.pi / 32.0
    for i in range(16):
        v = int(round(0.9 * 8388607 * math.sin(c.phase)))
        e.X[i] = v & M24
        e.Y[i] = v & M24
        c.phase += 2 * math.pi / 32.0
    e.run(0x02EC, end=LO, max_steps=500000)
    s_in = snap(e)
    log = []
    orig_step = e.step

    def step_trace():
        pc = e.pc
        if LO <= pc < HI:
            rec = {
                "pc": "%04X" % pc,
                "line": str(e.prog.get(pc)),
                "a": e.A & ((1 << 56) - 1), "b": e.B & ((1 << 56) - 1),
                "x0": e.get_reg("x0") & M24, "x1": e.get_reg("x1") & M24,
                "y0": e.get_reg("y0") & M24, "y1": e.get_reg("y1") & M24,
                "r0": e.get_reg("r0"), "r1": e.get_reg("r1"),
                "r3": e.get_reg("r3"), "r4": e.get_reg("r4"),
                "n3": e.get_reg("n3") & M24,
            }
            log.append(rec)
        orig_step()

    e.step = step_trace
    e.run(LO, end=0x0A84, max_steps=500000)
    s_l1 = snap(e)
    json.dump({"log": log, "s_in": s_in, "s_l1": s_l1,
               "regs_after": {"r0": e.get_reg("r0"), "r1": e.get_reg("r1"),
                              "r3": e.get_reg("r3"), "r4": e.get_reg("r4")}},
              open("/home/z/my-project/mining/exp18_L1_trace.json", "w"), indent=1)
    print("traced %d instructions, saved" % len(log))
    # quick view of first iterations
    for r in log[:24]:
        print("%(pc)s a=%(a)014X b=%(b)014X x0=%(x0)06X x1=%(x1)06X y0=%(y0)06X y1=%(y1)06X r0=%(r0)04X r1=%(r1)04X r4=%(r4)04X" % r)

if __name__ == "__main__":
    main()
