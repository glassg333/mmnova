#!/usr/bin/env python3
"""exp70c — инструментальный лог ВСЕХ обращений к памяти в хвосте $0AD1-$0B4C
(+ вход env2 $04A8-$0504) для одного кадра: база против $421=2."""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/pack7/tools")
import exp64_fm_pluginlevel as exp64
from exp64_fm_pluginlevel import sgn, P

base = {0x410: 64, 0x411: 96, 0x412: 96, 0x413: 127,
        0x414: 64, 0x415: 100, 0x416: 96, 0x417: 96,
        0x418: 64, 0x419: 64, 0x41A: 64, 0x41B: 64,
        0x41C: 127, 0x41D: 96, 0x41E: 96, 0x41F: 96}

def build():
    e = exp64.build_fm_track()
    exp64.setup_page(e, atk=0, hold=127, dec=127, rel=0, vol=127, pan=64)
    for a, v in base.items():
        e.Y[P + a] = (v & 0xFFFF) << 16
    for phase, lo, hi in ((0, 0x145D12, 0x145D1C), (1, 0x145D1D, 0x145D20)):
        e.R[6] = P + 0x28; e.R[7] = 0x100
        for r in range(8): e.M[r] = 0xFFFFFF
        e.ret_stack.append(0xDEAD)
        e.run(lo, end=hi, max_steps=100000)
        if e.ret_stack and e.ret_stack[-1] == 0xDEAD: e.ret_stack.pop()
    return e

TRACE_PC_LO, TRACE_PC_HI = 0x04A8, 0x0504   # env2 + панч
TAIL_LO, TAIL_HI = 0x0AD1, 0x0B4D           # хвост

def traced_frame(e, trig, tag, max_lines=400):
    orig_rd, orig_wr = e.rd, e.wr
    lines = []
    def hook(pc):
        return TRACE_PC_LO <= pc <= TRACE_PC_HI or TAIL_LO <= pc <= TAIL_HI
    def rd(space, ea):
        if hook(e.pc):
            lines.append("R %05X %s:%03X -> %06X" % (e.pc, space.upper(), ea, orig_rd(space, ea)))
        return orig_rd(space, ea)
    def wr(space, ea, val):
        if hook(e.pc):
            lines.append("W %05X %s:%03X <- %06X" % (e.pc, space.upper(), ea, val & 0xFFFFFF))
        orig_wr(space, ea, val)
    e.rd, e.wr = rd, wr
    e.Y[P + 0x28] = trig
    e.run(0x0100, end=0x02EC, max_steps=500000)
    e.X[0x2C9] = 0x300
    e.run(0x02EC, end=0x0B4C, max_steps=500000)
    e.rd, e.wr = orig_rd, orig_wr
    print("=== %s: %d обращений (env2 $04A8-$0504 + хвост $0AD1-$0B4C) ===" % (tag, len(lines)))
    for ln in lines[:max_lines]:
        print("  " + ln)
    if len(lines) > max_lines:
        print("  ... (%d ещё)" % (len(lines) - max_lines))
    return lines

def main():
    e = build()
    for f in range(8):
        traced_frame(e, 1 if f == 0 else 0, "разогрев f%d" % f, max_lines=0)
    # сохранить рег-контекст на входе в хвост нельзя напрямую — логируем EA
    print("X:FF=%06X X:2C9=%06X Y:124=%06X Y:4FF=%06X Y:49F=%06X Y:47F=%06X"
          % (e.X[0xFF], e.X[0x2C9], e.Y[0x124], e.Y[0x4FF], e.Y[0x49F], e.Y[0x47F]))
    traced_frame(e, 0, "БАЗА (кадр 9, фаза 0)", max_lines=1000)
    print("после: Y4FF=%06X X400=%06X out(Y0-1F)=%s"
          % (e.Y[0x4FF], e.X[0x400],
             " ".join("%06X" % sgn(e.Y[a]) for a in range(0, 8))))

    e2 = build()
    for f in range(8):
        e2.Y[P + 0x28] = 1 if f == 0 else 0
        e2.run(0x0100, end=0x02EC, max_steps=500000)
        e2.X[0x2C9] = 0x300
        e2.run(0x02EC, end=0x0B4C, max_steps=500000)
    e2.Y[P + 0x21] = 2
    print("\nX:FF=%06X X:2C9=%06X" % (e2.X[0xFF], e2.X[0x2C9]))
    traced_frame(e2, 0, "$421=2 (release)", max_lines=1000)
    print("после: Y4FF=%06X X400=%06X out(Y0-1F)=%s"
          % (e2.Y[0x4FF], e2.X[0x400],
             " ".join("%06X" % sgn(e2.Y[a]) for a in range(0, 8))))

if __name__ == "__main__":
    main()
