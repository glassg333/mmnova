#!/usr/bin/env python3
"""exp80b — BASE/WDTH: $408/$409 (аудит) или $410/$411 (итерация 17)?
Снимаю КОЛЬЦО КОЭФФИЦИЕНТОВ Y:$04-$07 сразу после блока кольца ($059A)
и входы тона. Инжекции в каждую ячейку-кандидата.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, run_frame, read_out, R6

P = R6

def run(cell=None, value=0):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, wdth=32, trig=1, hpq=32, lpq=96)
    e.Y[P + 0x12] = 32 << 16; e.Y[P + 0x13] = 96 << 16
    e.X[P + 0x12] = 32 << 16; e.X[P + 0x13] = 96 << 16
    if cell: e.Y[cell] = value; e.X[cell] = value
    cap = {}
    def hook(pc, emu):
        if pc == 0x059C and "ring" not in cap:   # сразу после кольца ($0573-$059A)
            cap["ring"] = [emu.Y[i] for i in range(4, 8)]   # АБСОЛЮТНЫЕ Y:$04-$07
            cap["x40d9"] = emu.X.get(0x4D9, 0)   # индекс тона B1
            cap["y4da"] = emu.Y.get(0x4DA, 0)    # WOFS-ветка
    try:
        e.run(0x0100, end=0x0B4C, max_steps=500000, hook=hook)
    except Exception as ex:
        print("  ERR:", ex, hex(e.pc)); return None
    out = read_out(e)
    return out, cap.get("ring"), cap.get("x40d9"), cap.get("y4da")

base = run()
print(f"база: ring={['%06X' % v for v in base[1]]} x4D9={base[2]:06X} y4DA={base[3]:06X}")
print(f"      выход[0:6] = {['%06X' % v for v in base[0][:6]]}")
print()
for name, cell in [("Y:$408", P+0x08), ("Y:$409", P+0x09), ("Y:$410", P+0x10), ("Y:$411", P+0x11)]:
    r = run(cell, 0x5A0000)
    ringd = r[1] != base[1]
    print(f"инжекция {name}=90: ring {'ИЗМЕНИЛОСЬ ' + str(['%06X' % v for v in r[1]]) if ringd else 'не изменился'}, "
          f"x4D9 {r[2]:06X}{'≠' if r[2]!=base[2] else '='}, y4DA {r[3]:06X}{'≠' if r[3]!=base[3] else '='}, "
          f"выход отличий {sum(1 for a,b in zip(r[0], base[0]) if a!=b)}/16")
