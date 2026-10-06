#!/usr/bin/env python3
"""exp80d — карта ручек против выхода фильтр-секции (граница astra-пакета):
состояние перед AMP $088E — пары L:$6D-$7C (A) и L:$AD-$BC (B), X+Y слова.
Это ровно тот сигнал, который производит фильтр-секция и потребляет AMP.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, R6

P = R6

def run(cell=None, value=0):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, wdth=32, trig=1, hpq=32, lpq=96)
    e.Y[P + 0x12] = 32 << 16; e.Y[P + 0x13] = 96 << 16
    e.X[P + 0x12] = 32 << 16; e.X[P + 0x13] = 96 << 16
    if cell: e.Y[cell] = value; e.X[cell] = value
    cap = {}
    def hook(pc, emu):
        if pc == 0x088E and "out" not in cap:
            cap["out"] = ([emu.X.get(i, 0) for i in range(0x6D, 0x7D)],
                          [emu.Y.get(i, 0) for i in range(0x6D, 0x7D)],
                          [emu.X.get(i, 0) for i in range(0xAD, 0xBD)],
                          [emu.Y.get(i, 0) for i in range(0xAD, 0xBD)])
    try:
        e.run(0x0100, end=0x0B4C, max_steps=500000, hook=hook)
    except Exception as ex:
        print("  ERR:", ex, hex(e.pc)); return None
    o = cap.get("out")
    if o is None: return None
    return o[0] + o[1] + o[2] + o[3]   # 80 слов

base = run()
nz = sum(1 for v in base if v)
print(f"база: перед-AMP ненулевых {nz}/80")
print()
cells = [("Y:$408 BASE?", P+0x08), ("Y:$409 WDTH?", P+0x09), ("Y:$40A HPQ?", P+0x0A),
         ("Y:$40B LPQ?", P+0x0B), ("Y:$410 EQF?", P+0x10), ("Y:$411 EQG?", P+0x11),
         ("Y:$412", P+0x12), ("Y:$413", P+0x13), ("Y:$416", P+0x16), ("Y:$417", P+0x17)]
for name, cell in cells:
    r = run(cell, 0x5A0000)
    if r is None: print(name, "ERR"); continue
    d = sum(1 for a, b in zip(r, base) if a != b)
    print(f"{name}=90: перед-AMP отличий {d}/80")
