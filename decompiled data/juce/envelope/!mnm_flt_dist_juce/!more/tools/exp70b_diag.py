#!/usr/bin/env python3
"""exp70b — диагностика ячеек env2/T5: кто куда пишет/читает, реальные r6/r7,
значения Y:$4FF / Y:$47F / X:$400 по кадрам, эффект гейта $421."""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/pack7/tools")
import exp64_fm_pluginlevel as exp64
from exp64_fm_pluginlevel import sgn, P

def q23(v):
    v = sgn(v & 0xFFFFFF)
    return v / 8388608.0

def db(x):
    return 20 * math.log10(max(x, 1e-12) / 8388607.0)

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

WATCH_W = {("y", 0x124), ("y", 0x47F), ("y", 0x4FF), ("x", 0x400), ("y", 0x420), ("y", 0x421)}
WATCH_R = {("y", 0x47F), ("y", 0x4FF), ("y", 0x418), ("y", 0x419), ("y", 0x41A), ("y", 0x41B)}

def frame(e, trig, log):
    orig_rd, orig_wr = e.rd, e.wr
    def rd(space, ea):
        if (space, ea) in WATCH_R:
            log.append(("R", e.pc, space, ea, sgn(orig_rd(space, ea))))
        return orig_rd(space, ea)
    def wr(space, ea, val):
        if (space, ea) in WATCH_W:
            log.append(("W", e.pc, space, ea, sgn(val & 0xFFFFFF)))
        orig_wr(space, ea, val)
    e.rd, e.wr = rd, wr
    e.Y[P + 0x28] = trig
    e.run(0x0100, end=0x02EC, max_steps=500000)
    e.X[0x2C9] = 0x300
    e.run(0x02EC, end=0x0B4C, max_steps=500000)
    e.rd, e.wr = orig_rd, orig_wr

def outs_rms(e):
    return math.sqrt(sum(q23(e.Y[a]) ** 2 for a in range(0x20)) / 32.0)

def main():
    e = build()
    # ---- кадр 1 (note on): полный лог событий по интересующим ячейкам
    log = []
    frame(e, 1, log)
    print("=== КАДР 1 (note-on): события W/R по y:$124/$47F/$4FF/$420/$421, x:$400 ===")
    for kind, pc, sp, ea, v in log:
        print("  %s %05X %s:%03X <- %06X" % (kind, pc, sp.upper(), ea, v & 0xFFFFFF)
              if kind == "W" else
              "  %s %05X %s:%03X -> %06X" % (kind, pc, sp.upper(), ea, v & 0xFFFFFF))
    print("  итог кадра: Y4FF=%06X Y47F=%06X X400=%06X Y124=%06X outRMS=%.0f"
          % (e.Y[0x4FF], e.Y[0x47F], e.X[0x400], e.Y[0x124], outs_rms(e)))

    # ---- ещё 7 кадров без событий, краткий итог
    for f in range(7):
        log = []
        frame(e, 0, log)
        if f == 6:
            print("=== КАДР 8 (steady): события ===")
            for kind, pc, sp, ea, v in log:
                print("  %s %05X %s:%03X %s %06X" % (kind, pc, sp.upper(), ea,
                      "<-" if kind == "W" else "->", v & 0xFFFFFF))
            print("  итог: Y4FF=%06X Y47F=%06X X400=%06X outRMS=%.0f"
                  % (e.Y[0x4FF], e.Y[0x47F], e.X[0x400], outs_rms(e)))

    # ---- гейт $421=2 (release) -> что происходит с $4FF/$47F/выходом
    print("=== ГЕЙТ $421=2 один кадр, далее 6 кадров ===")
    e2 = build()
    for f in range(8):
        frame(e2, 1 if f == 0 else 0, [])
    e2.Y[P + 0x21] = 2
    for f in range(7):
        frame(e2, 0, [])
        print("  f%d: Y4FF=%06X Y47F=%06X X400=%06X outRMS=%.0f (%.1f dB)"
              % (f, e2.Y[0x4FF], e2.Y[0x47F], e2.X[0x400], outs_rms(e2), db(outs_rms(e2))))

    # ---- $421=1 (attack) с нуля: сначала усыпить через $421=2, потом атака
    print("=== ГЕЙТ $421=1 после тишины ===")
    e3 = build()
    for f in range(8):
        frame(e3, 1 if f == 0 else 0, [])
    e3.Y[P + 0x21] = 2
    frame(e3, 0, [])
    e3.Y[P + 0x21] = 1
    for f in range(10):
        frame(e3, 0, [])
        print("  f%d: Y4FF=%06X Y47F=%06X X400=%06X outRMS=%.0f (%.1f dB)"
              % (f, e3.Y[0x4FF], e3.Y[0x47F], e3.X[0x400], outs_rms(e3), db(outs_rms(e3))))

if __name__ == "__main__":
    main()
