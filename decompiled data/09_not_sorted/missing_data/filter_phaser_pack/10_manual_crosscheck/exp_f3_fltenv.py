#!/usr/bin/env python3
"""
exp_f3_fltenv.py — РЕШАЮЩИЙ тест гипотезы «фильтр берёт основную энвелоуп»,
подтверждённой мануалом/сообществом MNM (tarekith tips + официальный MIDI CC map):

  FLT page: BAS($10) WDTH($11) HPQ($12) LPQ($13) ATK($14) DEC($15) BOFS($16) WOFS($17)
  «There is one AD-type filter envelope, ATK & DEC control the envelope shape.
   BOFS = how much the envelope modulates the cutoff BASE,
   WOFS = how much the envelope modulates the WIDTH.»

Итерация 15 (Ф2) не увидела эффекта в кольце коэффициентов, потому что трассировала
10-й кадр — AD-энвелопа с ATK/DEC=0.5 к тому моменту уже затухла. Здесь:

  1. Кольцо Y:$04-$07 ПО КАДРАМ (0..9) при живой энвелопе, BOFS/WOFS = 0 vs 127.
  2. Инверсия Y:$04 -> индекс coeff2 ($141CA7) -> сдвиг индекса от BOFS.
  3. Инверсия Y:$06/$07 -> индексы width1/width2 ($1444C6/$144546).
  4. Дифф чтений секции расчёта коэффициентов ($0537-$05A2) на кадре 2,
     baseline vs BOFS=127 -> точка впрыска env-офсета.
  5. Рамп-стейты P+$D4/$D5 и уровень/фаза энвелопа по кадрам.
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase, sgn, load_tbl

P = 0x400

COEFF2 = load_tbl("P_141CA7_filter_coeff_base_tbl.bin")
WIDTH1 = load_tbl("P_1444C6_filter_width_tbl_1.bin")
WIDTH2 = load_tbl("P_144546_filter_width_tbl_2.bin")

def s2f(v):
    return v / 8388608.0

def nearest(tbl, target):
    best, bi = None, -1
    for i, w in enumerate(tbl):
        d = abs(w - target)
        if best is None or d < best:
            best, bi = d, i
    return bi, best

def set_param(e, off, v):
    e.Y[P + off] = (int(v) & 0xFFFF) << 16

def run_case(name, nframes=10, trace_coeff_reads=False, **kw):
    c = EnvCase(**kw)
    e = c.e
    set_param(e, 0x05, 127)   # VOL
    set_param(e, 0x06, 64)    # PAN
    set_param(e, 0x10, 64)    # BASE
    set_param(e, 0x11, 64)    # WDTH
    set_param(e, 0x12, 64)    # HPQ
    set_param(e, 0x13, 64)    # LPQ
    readlog = []
    if trace_coeff_reads:
        _rd = e.rd
        def rd_hook(space, ea, _rd=_rd, log=readlog):
            v = _rd(space, ea)
            if 0x0537 <= e.pc <= 0x05A2:
                log.append((e.pc, space, ea, v))
            return v
        e.rd = rd_hook
    frames = []
    for k in range(nframes):
        r = c.frame(trig=1 if k == 0 else 0)
        rec = dict(
            k=k, ph=r["phase"], lvl=r["level"], amp=r["amp"],
            ring=[sgn(e.Y[i]) for i in (4, 5, 6, 7)],
            d4y=sgn(e.Y[P + 0xD4]), d4x=sgn(e.X[P + 0xD4]),
            d5y=sgn(e.Y[P + 0xD5]), d5x=sgn(e.X[P + 0xD5]),
        )
        # инверсии
        y04, y05, y06, y07 = rec["ring"]
        eps = 0.5 + s2f(y05) if y05 & 0x800000 else 0.5 + s2f(y05)
        denom = abs(s2f(abs(y05))) + 0.5
        tgt = abs(s2f(y04)) / denom if denom else 0
        idx, err = nearest(COEFF2, int(tgt * 8388608))
        rec["y04_idx"], rec["y04_err"] = idx, err
        i1, e1 = nearest(WIDTH1, y06)
        i2, e2 = nearest(WIDTH2, y07)
        rec["y06_idx"], rec["y06_err"] = i1, e1
        rec["y07_idx"], rec["y07_err"] = i2, e2
        frames.append(rec)
        print("  %-14s f%d ph=%d lvl=%+0.4f ring=[%+0.6f %+0.6f %+0.6f %+0.6f] "
              "idx04=%3d(e%06X) iw1=%2d iw2=%2d d4=%+0.4f d5=%+0.4f amp=%.4f" % (
              name, k, rec["ph"], s2f(rec["lvl"]),
              s2f(y04), s2f(y05), s2f(y06), s2f(y07),
              idx, err, i1, i2, s2f(rec["d4y"]), s2f(rec["d5y"]),
              s2f(rec["amp"])))
    return frames, readlog

def main():
    out = {}
    print("=== case A: baseline (BOFS=0 WOFS=0 ATK=64 DEC=64) ===")
    bl, _ = run_case("baseline", bofs=0, wofs=0, filt_atk=64, filt_dec=64)
    out["baseline"] = bl
    print("=== case B: BOFS=127 ===")
    b2, logb = run_case("bofs127", bofs=127, wofs=0, filt_atk=64, filt_dec=64,
                        trace_coeff_reads=True)
    out["bofs127"] = b2
    print("=== case C: WOFS=127 ===")
    b3, _ = run_case("wofs127", bofs=0, wofs=127, filt_atk=64, filt_dec=64)
    out["wofs127"] = b3
    print("=== case D: BOFS=127 WOFS=127 ===")
    b4, _ = run_case("both127", bofs=127, wofs=127, filt_atk=64, filt_dec=64)
    out["both127"] = b4
    print("=== case E: BOFS=127 ATK=0 ===")
    b5, _ = run_case("atk0", bofs=127, wofs=0, filt_atk=0, filt_dec=64)
    out["atk0"] = b5
    print("=== case F: BOFS=127 ATK=127 ===")
    b6, _ = run_case("atk127", bofs=127, wofs=0, filt_atk=127, filt_dec=64)
    out["atk127"] = b6
    print("=== case G: BOFS=127 DEC=0 ===")
    b7, _ = run_case("dec0", bofs=127, wofs=0, filt_atk=64, filt_dec=0)
    out["dec0"] = b7

    print("\n=== VERDICT: ring diffs vs baseline (frames 1..5) ===")
    for nm in ("bofs127", "wofs127", "both127"):
        diffs = []
        for f in range(1, 6):
            r1, r2 = out["baseline"][f]["ring"], out[nm][f]["ring"]
            if r1 != r2:
                diffs.append((f, [s2f(v) for v in r1], [s2f(v) for v in r2]))
        print("%s: %d frames differ" % (nm, len(diffs)))
        for f, r1, r2 in diffs[:3]:
            print("   f%d bl=%s | %s" % (f, ["%+0.5f" % v for v in r1],
                                         ["%+0.5f" % v for v in r2]))

    print("\n=== coeff-section read diff (frame 2, baseline vs bofs127) ===")
    # перезапуск baseline с трассировкой
    _, loga = run_case("baseline_t", bofs=0, wofs=0, filt_atk=64, filt_dec=64,
                       trace_coeff_reads=True)
    fa = {}
    for t in loga:
        fa.setdefault((t[0], t[1], t[2]), []).append(t[3])
    fb = {}
    for t in logb:
        fb.setdefault((t[0], t[1], t[2]), []).append(t[3])
    keys = sorted(set(fa) | set(fb))
    ndiff = 0
    for kkey in keys:
        va, vb = fa.get(kkey), fb.get(kkey)
        if va != vb:
            ndiff += 1
            print("  %04X %s:%04X  base=%s bofs=%s" % (
                kkey[0], kkey[1].upper(), kkey[2],
                ["%06X" % v for v in (va or [])][:3],
                ["%06X" % v for v in (vb or [])][:3]))
            if ndiff > 30:
                print("  ...")
                break
    if ndiff == 0:
        print("  (нет различий в секции $0537-$05A2 — env-офсеты входят в кольцо ИНАЧЕ)")

    json.dump(out, open("/home/z/my-project/mining/exp_f3_fltenv.json", "w"),
              indent=1, default=str)
    print("saved /home/z/my-project/mining/exp_f3_fltenv.json")

if __name__ == "__main__":
    main()
