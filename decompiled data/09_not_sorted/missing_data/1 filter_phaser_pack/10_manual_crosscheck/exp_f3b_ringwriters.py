#!/usr/bin/env python3
"""
exp_f3b_ringwriters.py — кто пишет в кольцо Y:$04-$07 в течение кадра,
baseline vs BOFS=127 vs WOFS=127 (энвелопа живая, триг в кадре 0).

Цель: найти точку впрыска env-офсетов (BOFS/WOFS) в коэффициенты фильтра.
Из exp_f3_fltenv: чтения секции $0537-$05A2 идентичны -> инжекция позже.
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase, sgn

P = 0x400

def set_param(e, off, v):
    e.Y[P + off] = (int(v) & 0xFFFF) << 16

def s2f(v):
    return v / 8388608.0

def run_case(name, nframes=4, **kw):
    c = EnvCase(**kw)
    e = c.e
    for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                   (0x12, 64), (0x13, 64)):
        set_param(e, off, v)
    writers = []
    _wr = e.wr
    _wrL = e.wrL
    def wr_hook(space, ea, val, _wr=_wr):
        if space == "y" and ea in (4, 5, 6, 7):
            writers.append((e.pc, ea, val & 0xFFFFFF))
        return _wr(space, ea, val)
    def wrL_hook(ea, v48, _wrL=_wrL):
        if ea in (4, 5, 6, 7):
            writers.append((e.pc, ("LX", ea), (v48 >> 24) & 0xFFFFFF))
            writers.append((e.pc, ("LY", ea), v48 & 0xFFFFFF))
        return _wrL(ea, v48)
    e.wr = wr_hook
    e.wrL = wrL_hook
    per_frame = []
    for k in range(nframes):
        writers.clear()
        r = c.frame(trig=1 if k == 0 else 0)
        snap = [(pc, ea, v) for (pc, ea, v) in writers]
        per_frame.append(dict(k=k, ph=r["phase"], lvl=r["level"],
                              ring=[sgn(e.Y[i]) for i in (4, 5, 6, 7)],
                              writers=snap))
        uniq = []
        for t in snap:
            if not uniq or uniq[-1][0] != t[0] or uniq[-1][1] != t[1]:
                uniq.append(t)
        print("%s f%d ph=%d lvl=%+0.4f ring=[%+0.5f %+0.5f %+0.5f %+0.5f] writers=%d" % (
            name, k, r["phase"], s2f(r["level"]),
            s2f(per_frame[-1]["ring"][0]), s2f(per_frame[-1]["ring"][1]),
            s2f(per_frame[-1]["ring"][2]), s2f(per_frame[-1]["ring"][3]), len(snap)))
        # компактная печать уникальных писателей
        seen = {}
        for pc, ea, v in snap:
            seen.setdefault((pc, str(ea)), []).append(v)
        for (pc, ea), vals in sorted(seen.items()):
            vs = ["%06X" % x for x in vals[:4]]
            print("    pc=%04X Y:%s n=%d %s%s" % (pc, ea, len(vals), " ".join(vs),
                  " ..." if len(vals) > 4 else ""))
    return per_frame

def main():
    out = {}
    for name, kw in (("baseline", dict(bofs=0, wofs=0, filt_atk=64, filt_dec=64)),
                     ("bofs127", dict(bofs=127, wofs=0, filt_atk=64, filt_dec=64)),
                     ("wofs127", dict(bofs=0, wofs=127, filt_atk=64, filt_dec=64))):
        print("=== %s ===" % name)
        out[name] = run_case(name, **kw)
    json.dump(out, open("/home/z/my-project/mining/exp_f3b_ringwriters.json", "w"),
              indent=1, default=str)
    print("saved /home/z/my-project/mining/exp_f3b_ringwriters.json")

if __name__ == "__main__":
    main()
