#!/usr/bin/env python3
"""exp80c — карта ручек против ФИНАЛЬНОГО выхода трека (кольцо T6 Y:$0000-$001F),
плюс потребление кольца коэффициентов каскадом (Y:$04-$07 перед вызовом $05A1).
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
        if pc == 0x05A1 and "ring_at_casc" not in cap:
            cap["ring_at_casc"] = [emu.Y[i] for i in range(4, 8)]
    try:
        e.run(0x0100, end=0x0B4C, max_steps=500000, hook=hook)
    except Exception as ex:
        print("  ERR:", ex, hex(e.pc)); return None
    out = [e.Y[i] for i in range(0x20)]            # T6 выходное кольцо Y:$0000-$001F
    return out, cap.get("ring_at_casc")

base = run()
nz = sum(1 for v in base[0] if v)
print(f"база: T6-выход ненулевых {nz}/32, первые 4: {['%06X' % v for v in base[0][:4]]}")
print(f"      кольцо перед каскадом: {['%06X' % v for v in base[1]]}")
print()
cells = [("Y:$408", P+0x08), ("Y:$409", P+0x09), ("Y:$40A", P+0x0A), ("Y:$40B", P+0x0B),
         ("Y:$410", P+0x10), ("Y:$411", P+0x11), ("Y:$412", P+0x12), ("Y:$413", P+0x13),
         ("Y:$416", P+0x16), ("Y:$417", P+0x17)]
for name, cell in cells:
    r = run(cell, 0x5A0000)
    if r is None: continue
    d = sum(1 for a, b in zip(r[0], base[0]) if a != b)
    ringd = r[1] != base[1]
    print(f"{name}=90: T6-выход отличий {d}/32; кольцо@каскад {'ИЗМЕНИЛОСЬ' if ringd else 'нет'}")
