#!/usr/bin/env python3
"""exp81 — пины ступени DIST при ПРАВИЛЬНОМ X:$40B = $200000 (init свитча $00F1).
Снимаем y1 (8/D), kDrive, K1, K2 для сетки DIST-ручки и сверяем с формулами:
  y1 = frac(8/0.25) = $400000 (0.5)
  K1 = limit24(y1·kDrive) = kDrive/2
  K2 = limit24(curve·D) = curve/4
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, R6

P = R6

def sgn(w):
    w &= 0xFFFFFF
    return w - 0x1000000 if w & 0x800000 else w

def limit24(v):
    v &= 0xFFFFFFFFFFFFFFFF
    # повторяем limit24 порта: насыщение 24-битного слова
    lo, hi = -(1 << 23), (1 << 23) - 1
    s = v >> 24  # приблизительно — берём фактические значения из эмулятора
    return v

def run(dist_v, div_word=0x200000):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, wdth=32, trig=1)
    e.X[P + 0x0B] = div_word           # X:$40B — init свитча
    e.Y[P + 0x04] = (dist_v & 0xFFFF) << 16   # DIST ручка v<<16
    cap = {}
    def hook(pc, emu):
        if pc == 0x07A6 and "y1" not in cap:
            cap["y1"] = emu.y1
            cap["x40b"] = emu.X.get(P + 0x0B, 0)
        if pc == 0x07BD and "gains" not in cap:
            cap["gains"] = (emu.y0, emu.x1)   # y0 = K1, x1 = K2 (после $07BB/$07BC)
        if pc == 0x07BC and "kdrive" not in cap:
            # после $07B9: a,y0 → y0 = kDrive; b = K2 (x1 на $07BB)
            cap["kdrive"] = emu.y0
    try:
        e.run(0x0100, end=0x0B4C, max_steps=500000, hook=hook)
    except Exception as ex:
        return {"err": str(ex), "pc": hex(e.pc)}
    return cap

print("X:$40B = $200000 (0.25 Q1.23), сетка DIST:")
print(f"{'DIST':>5} {'слово':>8} {'y1':>8} {'kDrive':>8} {'K1':>8} {'K2':>8}  K1=kDrive/2?")
for v in (0, 32, 64, 96, 127, -64):
    r = run(v)
    if "err" in r:
        print(f"{v:>5} ERR {r}")
        continue
    y1, kd, K1, K2 = r.get("y1"), r.get("kdrive"), r.get("gains", (None, None))[0], r.get("gains", (None, None))[1]
    half = ((kd & 0xFFFFFF) >> 1) if kd is not None else None
    ok = "?" if (K1 is None or kd is None) else ("да" if abs((K1 & 0xFFFFFF) - half) <= 2 else "НЕТ")
    print(f"{v:>5} {(v & 0xFFFF) << 16:08X} {y1:06X} {kd:06X} {K1:06X} {K2:06X}  {ok}")
