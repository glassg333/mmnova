#!/usr/bin/env python3
"""exp49_comb56_step.py — single-step the m13 comb5/6 LFO tap loops at f39
to recover the exact firmware semantics (registers + memory deltas)."""
import sys

sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R
import exp48_reverb_intdump as E48

M24 = 0xFFFFFF

REGS = ["a", "b", "x0", "x1", "y0", "y1", "n1", "m1", "m3", "r1", "r3", "r4"]


def fmt_reg(e, name):
    v = e.get_reg(name)
    if name in ("a", "b"):
        return "%014X" % (v & ((1 << 56) - 1))
    return "%06X" % (v & M24)


def main():
    e = R.build()
    R.run_init(e)
    R.set_params(e, dec=100, damp=2, gate=127, mix=127, hp=8, lp=100, inp=64)
    R.warmup(e, 6)

    # advance to frame 39
    for f in range(39):
        bus = E48.frame_bus(f)
        if f == 38:
            # hook: step-comb5 loop at this frame
            pass
        R.run_block(e, bus)

    # now run frame 39 with tracing inside comb5 (1454AD..1454C5) and comb6 (1454C5..1454DA)
    bus = E48.frame_bus(39)
    for i in range(64):
        e.X[0x200 + i] = bus[i]
    e.R[6] = R.R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    e.ret_stack[0] = R.PROC_END

    orig_step = e.step
    log = []
    state = {"prev": None}

    def snap():
        return (e.pc, {r: fmt_reg(e, r) for r in REGS},
                e.R[1] & M24, e.R[3] & M24, e.R[4] & M24,
                e.Y.get(0x60, 0), e.Y.get(0x61, 0), e.Y.get(0x70, 0), e.Y.get(0x71, 0),
                [e.X.get(i, 0) & M24 for i in range(6)])

    def step():
        pc = e.pc
        if 0x145493 <= pc <= 0x1454E0:
            if state["prev"] is not None:
                log.append((state["prev"], snap()))
            state["prev"] = pc
        orig_step()

    e.step = step
    e.run(R.PROC, end=R.PROC_END, max_steps=600000)
    e.step = orig_step

    # print transitions: pc -> pc, with reg changes
    print("transitions inside comb5/6 region (frame 39):")
    prev = None
    for entry_pc, (pc, regs, r1, r3, r4, y60, y61, y70, y71, xw) in log:
        if entry_pc == prev:
            continue
        prev = entry_pc
    # instead: dump full sequence for first 40 steps of comb5 body
    n = 0
    for entry_pc, (pc, regs, r1, r3, r4, y60, y61, y70, y71, xw) in log:
        if 0x1454AD <= entry_pc <= 0x1454C5 and n < 72:
            print(f"@{entry_pc:06X} a={regs['a']} b={regs['b']} x0={regs['x0']} "
                  f"x1={regs['x1']} y0={regs['y0']} y1={regs['y1']} n1={regs['n1']} "
                  f"m1={regs['m1']} r1={r1:06X} r3={r3:06X} r4={r4:06X} "
                  f"Y60={y60:06X} Y61={y61:06X} X[0..5]={' '.join(f'{v:06X}' for v in xw)}")
            n += 1
    print("...")
    n = 0
    for entry_pc, (pc, regs, r1, r3, r4, y60, y61, y70, y71, xw) in log:
        if 0x1454C5 <= entry_pc <= 0x1454E0 and n < 60:
            print(f"@{entry_pc:06X} a={regs['a']} b={regs['b']} x0={regs['x0']} "
                  f"x1={regs['x1']} y0={regs['y0']} y1={regs['y1']} n1={regs['n1']} "
                  f"m1={regs['m1']} r1={r1:06X} r3={r3:06X} r4={r4:06X} "
                  f"Y70={y70:06X} Y71={y71:06X} X[0..5]={' '.join(f'{v:06X}' for v in xw)}")
            n += 1


if __name__ == "__main__":
    main()
