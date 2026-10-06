#!/usr/bin/env python3
"""check_switch_x40b.py — эмпирическая проверка: пишет ли блок смены машины
(P:$00E1-$00F6, r6=$428) X:$40B, и трогает ли X:$40B INIT самой машины.

Прогон: пошагово $00E1->$0100 со состоянием «машина сменилась» (бит7 Y:$428).
Использует штатный watch-механизм эмулятора (e.watch / e.watch_log).
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm

def build_full():
    lines = disasm(0x0000, 0x0B4E)
    lines += disasm(0x144000, 0x2000)   # регион машин (для jsr init)
    from dsp_emu import DSP56300 as D
    return D(lines)

def run_switch(machine, old_machine=0):
    e = build_full()
    e.watch = {0x40B, 0x40F, 0x408}
    # r6 = $428 (страница машины у диспетчера), флаг смены = бит7 Y:$428
    e.R[6] = 0x428
    e.Y[0x428] = 0x80
    e.Y[0x424] = machine          # y:(r6-$4) = новый индекс
    e.Y[0x429] = machine          # y:(r6+$1)
    e.Y[0x124] = 0x100
    e.Y[0x100 + 0x120] = old_machine
    e.pc = 0x00E1
    init_ptr = e.X.get(0x10016B + machine, None)
    steps = 0
    try:
        while e.pc != 0x0100 and steps < 50000:
            e.step()
            steps += 1
    except Exception as ex:
        print(f"  [стоп на шаге {steps}: {ex}]")
    return e, init_ptr, steps

for M in (1, 5, 19):
    print(f"=== машина {M} ===")
    e, init_ptr, steps = run_switch(M)
    print(f"  init-указатель X:$10016B+{M} = {hex(init_ptr) if init_ptr else '?'}")
    print(f"  шагов: {steps}, финальный PC: {hex(e.pc)}")
    for (pc, sp, ea, val) in e.watch_log:
        print(f"  WRITE {sp}:${ea:06X} = {val:06X}  (из PC {pc})")
    print(f"  итог X:$40B = {e.X.get(0x40B, 0):06X}, X:$40F = {e.X.get(0x40F, 0):06X}, X:$408 = {e.X.get(0x408, 0):06X}")
