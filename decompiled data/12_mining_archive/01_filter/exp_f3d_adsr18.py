#!/usr/bin/env python3
"""
exp_f3d_adrs18.py — что за ADSR-машина в $04A8-$04F4, читающая $18(ATK,табл.
$141800) / $19(DEC,$141880) / $1A(SUS=$1A^2) / $1B(REL,$141880), уровень Y:$04FF,
стейт X[P+$00], только трек 0?

Тест: $18-$1B = 0 (наши прошлые прогоны) vs ADSR-набор (ATK=20 DEC=100 SUS=127 REL=100).
Смотрим: Y:$04FF, X[P+$00] (стейт), аудио, интегратор X:$40-$5E.
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

def run_case(name, adsr=None, nframes=12, **kw):
    c = EnvCase(**kw)
    e = c.e
    for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                   (0x12, 64), (0x13, 64)):
        set_param(e, off, v)
    if adsr:
        for off, v in zip((0x18, 0x19, 0x1A, 0x1B), adsr):
            set_param(e, off, v)
    rows = []
    for k in range(nframes):
        r = c.frame(trig=1 if k == 0 else 0)
        row = dict(k=k, ph=r["phase"], lvl=r["level"], amp=r["amp"],
                   lvl4ff=sgn(e.Y[0x4FF]), st=sgn(e.X[P + 0x00]),
                   st_y=sgn(e.Y[P + 0x00]),
                   integ_head=[s16(e.X[a]) / 8388608.0 for a in (0x40, 0x41, 0x51, 0x5E)])
        rows.append(row)
        print("%s f%-2d ph=%d lvl=%+0.4f amp=%.4f Y:04FF=%+0.5f Xst=%+0.5f "
              "integ[40,41,51,5E]=%s" % (
              name, k, row["ph"], s2f(row["lvl"]), s2f(row["amp"]),
              s2f(row["lvl4ff"]), row["st"] / 8388608.0,
              ["%+0.4f" % v for v in row["integ_head"]]))
    return rows

def main():
    out = {}
    print("=== A: $18-$1B = 0 (как во всех прошлых прогонах) ===")
    out["adsr0"] = run_case("adsr0", adsr=None, bofs=0, wofs=0,
                            filt_atk=64, filt_dec=64)
    print("=== B: ADSR $18=20 $19=100 $1A=127 $1B=100 ===")
    out["adsr_on"] = run_case("adsr_on", adsr=(20, 100, 127, 100), bofs=0, wofs=0,
                              filt_atk=64, filt_dec=64)
    print("=== C: ADSR + BOFS=127 WOFS=127 ===")
    out["adsr_bofs"] = run_case("adsr_bofs", adsr=(20, 100, 127, 100), bofs=127,
                                wofs=127, filt_atk=64, filt_dec=64)
    json.dump(out, open("/home/z/my-project/mining/exp_f3d_adsr18.json", "w"),
              indent=1, default=str)
    print("saved /home/z/my-project/mining/exp_f3d_adsr18.json")

if __name__ == "__main__":
    main()
