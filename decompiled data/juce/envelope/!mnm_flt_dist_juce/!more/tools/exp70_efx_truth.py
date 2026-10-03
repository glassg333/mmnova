#!/usr/bin/env python3
"""exp70 — полная эмуляция + трассировка блока $04A8 («env2») и ячеек EFX
$418-$41F на бит-точном эмуляторе OS 1.32B (оракул паков 7/8/10/11).

Вопросы (пользователь: «фулл заэмулируй и посмотри»):
  A. КТО читает Y:$418-$41F на самом деле — включая НЕПРЯМЫЕ чтения
     (другая база r6 / косвенная адресация), которых не видит греп.
  B. Что реально двигает каждая ручка EQF/EQG/SRR/DTIM/DWID и что слышимо:
     уровень T6-выхода, эхо-шина, ступенька гейна (staircase), ZOH-метрика
     (децимация), машинный банк (контроль живости).
  C. Гейт-форсаж Y:$421 = 1/2/4/5: траектория уровня Y:$4FF и T6-выхода в дБ.
  D. DWID=0/127: мастеринговый ли это уровень трека (гейт T6).
"""
import sys, json, gzip, math
sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/pack7/tools")
import exp64_fm_pluginlevel as exp64
from exp64_fm_pluginlevel import sgn, P

NF = 24
WIN = [(0x000, 0x700)]

def q23s(v):
    v &= 0xFFFFFF
    return v - 0x1000000 if v & 0x800000 else v

def db(x):
    return 20 * math.log10(max(x, 1e-12) / 8388607.0)

def rms(xs):
    return math.sqrt(sum(q23s(x) ** 2 for x in xs) / max(len(xs), 1))

def snap(e):
    d = {}
    for side in ("X", "Y"):
        mem = getattr(e, side)
        for lo, hi in WIN:
            for a in range(lo, hi):
                d["%s%03X" % (side, a)] = sgn(mem.get(a, 0))
    return d

REGIONS = [
    ("mach",   [("Y", 0x100, 0x123)]),
    ("t6out",  [("X", 0x0FF, 0x11F), ("Y", 0x000, 0x020)]),
    ("echo",   [("X", 0x300, 0x320)]),
    ("taps",   [("X", 0x08E, 0x0A2), ("Y", 0x08E, 0x0A2)]),
    ("s2bank", [("X", 0x040, 0x090), ("Y", 0x020, 0x040), ("Y", 0x060, 0x090)]),
    ("dstate", [("X", 0x4C4, 0x4DB), ("Y", 0x4C4, 0x4DB)]),
    ("rings",  [("X", 0x000, 0x040)]),
    ("ramps",  [("Y", 0x010, 0x020)]),
]

def region_of(addr):
    side, a = ("Y" if addr >= 0x1000000 else "X"), (addr & 0xFFFFFF)
    for name, spans in REGIONS:
        for s2, lo, hi in spans:
            if s2 == side and lo <= a < hi:
                return name
    return "rest"

def diff_regions(base_seed, base_frames, frames):
    per = {}
    for reg, _ in REGIONS + [("rest", None), ("page", None)]:
        per[reg] = (0, 0)
    for f in range(len(frames)):
        for key, v in frames[f].items():
            b = base_frames[f].get(key, base_seed.get(key, 0))
            d = abs(v - b)
            if d:
                side, cell = key[0], int(key[1:], 16)
                a = (0x1000000 if side == "Y" else 0) | cell
                reg = "page" if 0x400 <= cell < 0x434 else region_of(a)
                mx, cnt = per[reg]
                per[reg] = (max(mx, d), cnt + 1)
    return per

def build(knobs):
    e = exp64.build_fm_track()
    exp64.setup_page(e, atk=0, hold=127, dec=127, rel=0, vol=127, pan=64)
    for a, v in knobs.items():
        e.Y[a] = (v & 0xFFFF) << 16          # ключи АБСОЛЮТНЫЕ ($410-$41F), как в exp68
    for phase, lo, hi in ((0, 0x145D12, 0x145D1C), (1, 0x145D1D, 0x145D20)):
        e.R[6] = P + 0x28; e.R[7] = 0x100
        for r in range(8): e.M[r] = 0xFFFFFF
        e.ret_stack.append(0xDEAD)
        e.run(lo, end=hi, max_steps=100000)
        if e.ret_stack and e.ret_stack[-1] == 0xDEAD: e.ret_stack.pop()
    return e

def run_frames(e, n, collect=False, gate_at=None, gate_val=0):
    frames, seed = [], snap(e)
    for f in range(n):
        if gate_at is not None and f == gate_at:
            e.Y[P + 0x21] = gate_val & 0xFFFFFF
        e.Y[P + 0x28] = 1 if f == 0 else 0
        e.run(0x0100, end=0x02EC, max_steps=500000)
        e.X[0x2C9] = 0x300
        e.run(0x02EC, end=0x0B4C, max_steps=500000)
        if collect:
            frames.append(snap(e))
    return seed, frames

def readtrace(e, nframes=3):
    """полный лог чтений страницы $400-$433 (X и Y) + Y:$4FF на n кадрах;
    в кадры подмешивается гейт $421=1 (атака), чтобы исполнились фазовые ветки"""
    orig_rd = e.rd
    log = {}
    def rd(space, ea):
        if space == "y" and (0x400 <= ea <= 0x433 or ea == 0x4FF):
            log.setdefault(ea, set()).add(e.pc)
        elif space == "x" and 0x3F0 <= ea <= 0x433:
            log.setdefault(0x1000000 | ea, set()).add(e.pc)
        return orig_rd(space, ea)
    e.rd = rd
    for f in range(nframes):
        e.Y[P + 0x21] = 1          # форсаж фазы 1 (атака) -> env2 читает $418
        e.Y[P + 0x28] = 1 if f == 0 else 0
        e.run(0x0100, end=0x02EC, max_steps=500000)
        e.X[0x2C9] = 0x300
        e.run(0x02EC, end=0x0B4C, max_steps=500000)
    e.rd = orig_rd
    return {a: sorted(pcs) for a, pcs in log.items()}

def zoh_metric(ring_frames):
    """доля равных соседних сэмплов в T6-выходе (признак децимации/удержания)"""
    eq = tot = 0
    for fr in ring_frames:
        for i in range(len(fr) - 1):
            tot += 1
            if fr[i] == fr[i + 1]:
                eq += 1
    return eq / max(tot, 1)

def main():
    res = {"probe": "exp70", "date": "2026-10-03"}
    base = {0x410: 64, 0x411: 96, 0x412: 96, 0x413: 127,          # FLT BASE WDTH HPQ LPQ
            0x414: 64, 0x415: 100, 0x416: 96, 0x417: 96,          # ст.2 ATK DEC BOFS WOFS
            0x418: 64, 0x419: 64, 0x41A: 64, 0x41B: 64,           # EFX EQF EQG SRR DTIM
            0x41C: 127, 0x41D: 96, 0x41E: 96, 0x41F: 96}          # DSND DFB DBAS DWID

    # ---------- A. КТО читает $400-$433 (+$4FF) при активной фазе ----------
    e = build(base)
    run_frames(e, 4)
    rt = readtrace(e, 3)
    res["readers"] = {("Y" if a < 0x1000000 else "X") + "%03X" % (a & 0xFFFFFF): pcs
                      for a, pcs in sorted(rt.items())}
    print("=== A. ЧИТАТЕЛИ СТРАНИЦЫ (3 кадра, включая непрямые) ===")
    for k, pcs in res["readers"].items():
        cell = int(k[1:], 16)
        if 0x418 <= cell <= 0x41F or k.startswith("X"):
            print("  %s: %d pc: %s" % (k, len(pcs),
                  ",".join("%05X" % p for p in pcs[:14]) + ("..." if len(pcs) > 14 else "")))

    # ---------- B. Свипы ручек ----------
    runs = {}
    seed_b, frames_b = run_frames(build(base), NF, collect=True)
    runs["base"] = (seed_b, frames_b)

    def outs_of(fr):  return [fr["Y%03X" % a] for a in range(0x20)]
    def echo_of(fr):  return [fr["X%03X" % a] for a in range(0x300, 0x320)]
    def mach_of(fr):  return [fr["Y%03X" % a] for a in range(0x100, 0x123)]

    print("\n=== B. СВИПЫ: база и ручки EFX ===")
    print("  base: out RMS f1/f8/f23 = %.0f/%.0f/%.0f, echo RMS = %.0f, mach RMS = %.0f, Y4FF f8=%d, фаза X400 f8=%d"
          % (rms(outs_of(frames_b[0])), rms(outs_of(frames_b[7])), rms(outs_of(frames_b[23])),
             rms(echo_of(frames_b[7])), rms(mach_of(frames_b[7])), frames_b[7]["Y4FF"], frames_b[7]["X400"]))
    stair_base = max(abs(v) for v in outs_of(frames_b[7])) and \
        (max(abs(v) for v in outs_of(frames_b[7])) - min(abs(v) for v in outs_of(frames_b[7]))) / max(1, max(abs(v) for v in outs_of(frames_b[7])))
    res["base"] = dict(out_rms=[rms(outs_of(f)) for f in frames_b],
                       echo_rms=[rms(echo_of(f)) for f in frames_b],
                       y4ff=[f["Y4FF"] for f in frames_b], phase=[f["X400"] for f in frames_b])

    sweeps = {}
    for cell, name in ((0x418, "EQF"), (0x419, "EQG"), (0x41A, "SRR"),
                       (0x41B, "DTIM"), (0x41F, "DWID")):
        for v in (0, 32, 96, 127):
            if v == 64 and name != "DWID":
                continue
            k = dict(base); k[cell] = v
            sd, fr = run_frames(build(k), NF, collect=True)
            tag = "%s%d" % (name, v)
            runs[tag] = (sd, fr)
            o8 = outs_of(fr[7])
            mx = max(abs(x) for x in o8)
            stair = (mx - min(abs(x) for x in o8)) / max(1, mx)
            dr = diff_regions(seed_b, frames_b, fr)
            top = sorted(((r, c) for r, (m, c) in dr.items() if c), key=lambda t: -t[1])[:6]
            zoh = zoh_metric([outs_of(f) for f in fr[6:18]])
            sweeps[tag] = dict(out_rms=[rms(outs_of(f)) for f in fr],
                               echo_rms=[rms(echo_of(f)) for f in fr],
                               y4ff=[f["Y4FF"] for f in fr],
                               phase=[f["X400"] for f in fr],
                               stair=stair, zoh=zoh,
                               diff={r: [m, c] for r, (m, c) in dr.items() if c})
            print("  %-7s out RMS f1/f8/f23 = %8.0f/%8.0f/%8.0f | echo %8.0f | stair %.2f | ZOH %.2f | Y4FF f8=%d | diff: %s"
                  % (tag, rms(outs_of(fr[0])), rms(outs_of(fr[7])), rms(outs_of(fr[23])),
                     rms(echo_of(fr[7])), stair, zoh, fr[7]["Y4FF"],
                     ", ".join("%s:%d" % (r, c) for r, c in top)))
    res["sweeps"] = sweeps

    # ---------- C. Гейт-форсаж $421 = 1/2/4/5 ----------
    print("\n=== C. ГЕЙТ Y:$421 (форсаж фазы, one-shot) ===")
    gates = {}
    for v in (1, 2, 4, 5):
        e = build(base)
        run_frames(e, 8)
        sd, fr = run_frames(e, 16, collect=True, gate_at=0, gate_val=v)
        # NB: gate_at=0 -> значение положено перед 9-м кадром (после 8 разогрева)
        lvl = [f["Y4FF"] for f in fr]
        orms = [rms(outs_of(f)) for f in fr]
        gates["gate%d" % v] = dict(y4ff=lvl, out_rms=orms,
                                   phase=[f["X400"] for f in fr])
        print("  $421=%d: Y4FF кадры0-7: %s" % (v, " ".join("%06X" % (l & 0xFFFFFF) for l in lvl[:8])))
        print("            dB T6-out: %s" % " ".join("%5.1f" % db(r) for r in orms[:8]))
        print("            фаза X400: " + " ".join(str(p) for p in gates["gate%d" % v]["phase"][:8]))
    res["gates"] = gates

    # ---------- D. DWID как уровень: 0 / 96 / 127 + sustain через $421=5 ----------
    print("\n=== D. DWID (Y:$41F) как множитель уровня T6 ===")
    for v in (0, 96, 127):
        k = dict(base); k[0x41F] = v
        sd, fr = run_frames(build(k), NF, collect=True)
        print("  DWID=%3d: out RMS f23 = %9.0f (%s) | Y4FF f23 = %06X"
              % (v, rms(outs_of(fr[23])), db(rms(outs_of(fr[23]))), fr[23]["Y4FF"] & 0xFFFFFF))

    # sustain-ветка: $421=5, SRR = 0/64/127 -> уровень = SRR^2 ?
    print("\n=== D2. $421=5 (сустейн), SRR ($41A) = 0/32/64/127 ===")
    for v in (0, 32, 64, 127):
        k = dict(base); k[0x41A] = v
        e = build(k)
        run_frames(e, 8)
        sd, fr = run_frames(e, 6, collect=True, gate_at=0, gate_val=5)
        expect = (v * v) & 0xFFFFFF
        print("  SRR=%3d: Y4FF f1 = %06X (ожидание SRR^2 = %06X) | out RMS f1 = %9.0f (%s)"
              % (v, fr[0]["Y4FF"] & 0xFFFFFF, expect, rms(outs_of(fr[0])), db(rms(outs_of(fr[0])))))

    with gzip.open("/home/z/my-project/work/exp70_efx_truth.json.gz", "wb") as f:
        f.write(json.dumps(res).encode())
    print("\nsaved work/exp70_efx_truth.json.gz")

if __name__ == "__main__":
    main()
