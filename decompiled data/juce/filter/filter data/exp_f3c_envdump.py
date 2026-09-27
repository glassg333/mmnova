#!/usr/bin/env python3
"""
exp_f3c_envdump.py — ГДЕ живёт env-модуляция BOFS/WOFS:
  1. Самомодификация параметров страницы P+$10..$17 (BASE/WDTH/HPQ/LPQ)?
  2. Движение банков тапов L:$80-$8F (резонатор) и L:$90-$9F (делэй)?
  3. Интегратор X:$40-$5E.
  4. Все читатели Y:$04-$07 за кадр (где потребляются).

Кадры 0..5, триг в кадре 0, baseline vs BOFS=127 vs WOFS=127.
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase, sgn

P = 0x400

def set_param(e, off, v):
    e.Y[P + off] = (int(v) & 0xFFFF) << 16

def s2f(v):
    return v / 8388608.0

def s16(v):
    return v - 0x1000000 if v & 0x800000 else v

def run_case(name, nframes=6, trace_readers=False, **kw):
    c = EnvCase(**kw)
    e = c.e
    for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                   (0x12, 64), (0x13, 64)):
        set_param(e, off, v)
    reads = []
    if trace_readers:
        _rd = e.rd
        def rd_hook(space, ea, _rd=_rd):
            v = _rd(space, ea)
            if space == "y" and ea in (4, 5, 6, 7):
                reads.append((e.pc, ea, v))
            return v
        e.rd = rd_hook
    rows = []
    for k in range(nframes):
        reads.clear()
        r = c.frame(trig=1 if k == 0 else 0)
        page = [sgn(e.Y[P + o]) for o in range(0x10, 0x18)]
        taps_res = []   # L:$80-$8F -> X-part / Y-part
        for a in range(0x80, 0x90):
            taps_res.append((s16(e.X[a]) / 8388608.0, s16(e.Y[a]) / 8388608.0))
        taps_del = []
        for a in range(0x90, 0xA0):
            taps_del.append((s16(e.X[a]) / 8388608.0, s16(e.Y[a]) / 8388608.0))
        integ = [s16(e.X[a]) / 8388608.0 for a in range(0x40, 0x5F)]
        row = dict(k=k, ph=r["phase"], lvl=r["level"], amp=r["amp"],
                   page=page, taps_res=taps_res, taps_del=taps_del, integ=integ,
                   readers=[(pc, ea, v) for (pc, ea, v) in reads])
        rows.append(row)
        pv = " ".join("%+0.4f" % s2f(v) for v in page)
        print("%s f%d lvl=%+0.4f amp=%.4f page10-17=[%s]" % (name, k, s2f(r["level"]),
              s2f(r["amp"]), pv))
        print("   taps_res[:4]=%s" % " ".join("(%.3f,%.3f)" % t for t in taps_res[:4]))
        print("   taps_del[:4]=%s" % " ".join("(%.3f,%.3f)" % t for t in taps_del[:4]))
    if trace_readers:
        seen = {}
        for pc, ea, v in reads:
            seen.setdefault((pc, ea), []).append(v)
        print("%s: readers of Y:$04-$07:" % name)
        for (pc, ea), vals in sorted(seen.items()):
            vs = ["%06X" % x for x in vals[:3]]
            print("    pc=%04X Y:%d n=%d %s%s" % (pc, ea, len(vals), " ".join(vs),
                  " ..." if len(vals) > 3 else ""))
    return rows

def main():
    out = {}
    for name, kw, tr in (("baseline", dict(bofs=0, wofs=0, filt_atk=64, filt_dec=64), False),
                         ("bofs127", dict(bofs=127, wofs=0, filt_atk=64, filt_dec=64), True),
                         ("wofs127", dict(bofs=0, wofs=127, filt_atk=64, filt_dec=64), False)):
        print("=== %s ===" % name)
        out[name] = run_case(name, trace_readers=tr, **kw)
    print("\n=== page param diffs (P+$10..$17), f5 ===")
    for nm, rows in out.items():
        print(nm, " ".join("%+0.5f" % s2f(v) for v in rows[-1]["page"]))
    print("\n=== tap bank diffs f5 (resonator L:$80-8F) ===")
    a, b, cc = out["baseline"][-1]["taps_res"], out["bofs127"][-1]["taps_res"], out["wofs127"][-1]["taps_res"]
    for i in range(0, 16, 4):
        print("  [%d] bl=%s bofs=%s wofs=%s" % (i,
              " ".join("(%.3f,%.3f)" % t for t in a[i:i+4]),
              " ".join("(%.3f,%.3f)" % t for t in b[i:i+4]),
              " ".join("(%.3f,%.3f)" % t for t in cc[i:i+4])))
    print("=== tap bank diffs f5 (delay L:$90-9F) ===")
    a, b, cc = out["baseline"][-1]["taps_del"], out["bofs127"][-1]["taps_del"], out["wofs127"][-1]["taps_del"]
    for i in range(0, 16, 4):
        print("  [%d] bl=%s bofs=%s wofs=%s" % (i,
              " ".join("(%.3f,%.3f)" % t for t in a[i:i+4]),
              " ".join("(%.3f,%.3f)" % t for t in b[i:i+4]),
              " ".join("(%.3f,%.3f)" % t for t in cc[i:i+4])))
    json.dump(out, open("/home/z/my-project/mining/exp_f3c_envdump.json", "w"),
              indent=1, default=str)
    print("saved /home/z/my-project/mining/exp_f3c_envdump.json")

if __name__ == "__main__":
    main()
