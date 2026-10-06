#!/usr/bin/env python3
"""exp81b — пошаговая трасса $079F-$07BD при X:$40B=$200000, DIST=0.
Печатаем регистры на каждой инструкции — точная механика y1/K1/K2.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, R6

P = R6
e = build_track_emu(machine=1)
setup_track(e, base=64, wdth=32, trig=1)
e.X[P + 0x0B] = 0x200000
e.Y[P + 0x04] = 0  # DIST=0

state = {"on": False}
def hook(pc, emu):
    if pc == 0x079F:
        state["on"] = True
    if state["on"] and pc <= 0x07BE:
        regs = f"x0={emu.x0:06X} x1={emu.x1:06X} y0={emu.y0:06X} y1={emu.y1:06X} a={emu.get_reg('a'):012X} b={emu.get_reg('b'):012X} r2={emu.R[2]:06X}"
        ins = emu.prog.get(pc)
        print(f"{pc:06X}: {ins[2] if ins else '?':<28} | {regs}")
    if pc > 0x07BE:
        state["on"] = False

try:
    e.run(0x0100, end=0x0B4C, max_steps=500000, hook=hook)
except Exception as ex:
    print("ERR:", ex, hex(e.pc))
