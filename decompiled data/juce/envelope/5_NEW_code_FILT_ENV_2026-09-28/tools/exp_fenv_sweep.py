#!/usr/bin/env python3
"""exp_fenv_sweep.py — ОДИН ПРОГОН: свип всех 4 ручек энвелопа фильтра
(ATK/DEC/BOFS/WOFS страницы FILT, P+$14-$17) на бит-точном эмуляторе OS 1.32.
Правило №18: никаких ручных проверок по одной.

Для каждой тройки (ручка, позиция) — 8 кадров полного трек-цикла $0100-$0B4C
с брейкпоинтами на границах блоков стадии 2 ($0A5D/$0A84/$0A9D/$0AB7/$0AD1).
База: ATK=DEC=BOFS=WOFS=64, триг в кадре 0.

Выход: work/fm_fenv/fenv_sweep_snap.json (тот же формат, что exp18_fdn_snap).
"""
import sys, os, json, math

sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase  # noqa

M24 = 0xFFFFFF
P = 0x400
OUT = "/home/z/my-project/work/fm_fenv/fenv_sweep_snap.json"


class BPCase(EnvCase):
    force_cf = None   # (cfx, cfy) — форс L:P+$CF перед снапшотом $0A5D

    def frame_bp(self, trig=0):
        e = self.e
        e.Y[P + 0x28] = trig
        e.run(0x0100, end=0x02EB, max_steps=500000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.Y[0x100 + i] = v & M24
            self.phase += 2 * math.pi / 32.0
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.X[i] = v & M24
            e.Y[i] = v & M24
            self.phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0A5D, max_steps=500000)
        if e.pc != 0x0A5D:
            raise RuntimeError("stage2 not reached (pc=%04X)" % e.pc)
        if self.force_cf is not None:
            cfx, cfy = self.force_cf
            e.X[P + 0xCF] = cfx & M24
            e.Y[P + 0xCF] = cfy & M24
        def snap():
            d = {}
            SNAP = (list(range(0x00, 0x22)) + list(range(0x20, 0x62)) +
                    list(range(0x62, 0x82)) + list(range(0x90, 0xA1)) + [0xC4])
            for sp in ("X", "Y"):
                mem = getattr(e, sp)
                for a in SNAP:
                    d["%s:%03X" % (sp, a)] = mem.get(a, 0) & M24
            for a in range(0x00, 0x100):
                d["Y:P+%02X" % a] = e.Y.get(P + a, 0) & M24
                d["X:P+%02X" % a] = e.X.get(P + a, 0) & M24
            return d
        s_in = snap()
        e.run(0x0A5D, end=0x0A84, max_steps=500000); s_l1 = snap()
        e.run(0x0A84, end=0x0A9D, max_steps=500000); s_l2 = snap()
        e.run(0x0A9D, end=0x0AB7, max_steps=500000); s_l3 = snap()
        e.run(0x0AB7, end=0x0AD1, max_steps=500000); s_dp = snap()
        e.run(0x0AD1, end=0x0B4C, max_steps=500000); s_out = snap()
        return s_in, s_l1, s_l2, s_l3, s_dp, s_out


def main():
    POS = [0, 1, 32, 64, 96, 126, 127]
    BASE = dict(filt_atk=64, filt_dec=64, bofs=64, wofs=64)
    cases = []
    for k, name in enumerate(("filt_atk", "filt_dec", "bofs", "wofs")):
        for pos in POS:
            kw = dict(BASE)
            kw[name] = pos
            cases.append((name, pos, kw, None))
    # форс состояния делеЯ L:P+$CF — покрытие пути L3 с ненулевым индексом
    # кривой (индекс = биты [40..17] состояния, читает X:$144B48+idx)
    CF_CASES = [
        (0x000010, 0x000000), (0x000100, 0x000000), (0x001000, 0x000000),
        (0x010000, 0x000000), (0x000000, 0x800000), (0x001234, 0x56789A),
    ]
    for j, (cfx, cfy) in enumerate(CF_CASES):
        kw = dict(BASE)
        kw["bofs"] = 30 + 10 * j
        cases.append(("force_cf", j, kw, (cfx, cfy)))
    out = []
    for i, (name, pos, kw, fcf) in enumerate(cases):
        c = BPCase(**kw)
        c.force_cf = fcf
        for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                       (0x12, 64), (0x13, 64)):
            c.e.Y[P + off] = (int(v) & 0xFFFF) << 16
        frames = []
        for k in range(8):
            ss = c.frame_bp(trig=1 if k == 0 else 0)
            frames.append(ss)
        atk = kw["filt_atk"]; dec = kw["filt_dec"]
        bofs = kw["bofs"]; wofs = kw["wofs"]
        out.append(dict(atk=atk, dec=dec, bofs=bofs, wofs=wofs,
                        varied=name, pos=pos, force_cf=fcf, frames=frames))
        print("[%2d/%d] %s=%d ok" % (i + 1, len(cases), name, pos), flush=True)
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    json.dump(out, open(OUT, "w"))
    print("saved %s (%d configs x 8 frames)" % (OUT, len(out)))


if __name__ == "__main__":
    main()
