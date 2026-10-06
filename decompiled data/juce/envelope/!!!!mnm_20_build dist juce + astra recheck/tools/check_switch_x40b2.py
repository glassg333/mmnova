#!/usr/bin/env python3
"""check_switch_x40b2.py — полный прогон смены машины ВКЛЮЧАЯ init машины.
Загружаем P-данные (таблицы $1001xx) как track_harness, ставим watch на
X:$40B/$40F/$408/$40C и смотрим: (1) свитч-блок $00F1/F4/F5; (2) что делает
init машины (jsr r2) с этими ячейками.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm, PM
from dsp_emu import DSP56300 as D

PROC_INIT = {1: 0x144CC9, 2: 0x144D9A, 14: 0x145AB5, 15: 0x147661, 19: 0x144E81}

def build_full(machine):
    lines = disasm(0x0000, 0x0B4E)
    lines += disasm(0x144000, 0x2000)
    e = D(lines)
    data = open(PM, "rb").read()
    n = len(data) // 3
    for i in range(0x100000, min(n, 0x160000)):
        o = i * 3
        w = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
        e.X[i] = w
        e.Y[i] = w
    slot = 0x1001AF + machine
    e.Y[0x120] = slot
    e.Y[0x121] = slot
    e.Y[0x122] = slot
    e.X[slot + 0x1001AF] = 0x144CD0          # proc (не вызовется здесь)
    e.X[slot + 0x10016B] = PROC_INIT.get(machine, 0x144CC9)
    return e

for M in (1, 14, 19):
    e = build_full(M)
    e.watch = {0x40B, 0x40F, 0x408, 0x40C}
    e.R[6] = 0x428
    e.Y[0x428] = 0x80
    e.Y[0x424] = M
    e.Y[0x429] = M
    e.Y[0x124] = 0x100
    e.Y[0x100 + 0x120] = 0
    e.pc = 0x00E1
    print(f"=== машина {M} (init {hex(PROC_INIT.get(M,0))}) ===")
    steps = 0
    entered_init = False
    try:
        while steps < 60000:
            pc_before = e.pc
            if pc_before == 0x00F6:
                entered_init = True
                r2v = e.R[2]
                r2v = int(r2v) if isinstance(r2v, (int, float)) else 0
                print(f"  -- jsr (r2) -> init {r2v:06X} --")
            e.step()
            steps += 1
            if entered_init and pc_before != 0x00F6 and e.pc == 0x00F7:
                print(f"  -- init вернулся ({steps} шагов) --")
                break
    except Exception as ex:
        print(f"  [стоп: {ex} на шаге {steps}]")
    for (pc, sp, ea, val) in e.watch_log:
        print(f"  WRITE {sp}:${ea:06X} = {val:06X}  (из PC {hex(pc)})")
    print(f"  итог: X:$40B={e.X.get(0x40B,0):06X} X:$40C={e.X.get(0x40C,0):06X} "
          f"X:$40F={e.X.get(0x40F,0):06X} X:$408={e.X.get(0x408,0):06X}")
