#!/usr/bin/env python3
"""
exp22_tail_skip.py — collect bge-skip frames of the stage-2 tail:
force the delay-length state L:P+$CF = 0 at $0AD1 so the DIV result
exceeds the #$10 threshold and `bge func_000b1e` is taken.
Sub-block snapshots b8 (post-DWID) and b9 (post-mix) still collected.
"""
import sys, json, math, gzip

PK = "/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack"
sys.path.insert(0, PK + "/05_emulator")
sys.path.insert(0, PK + "/06_amp_env")
sys.path.insert(0, "/home/z/my-project/scripts")
from exp22_tail import TailCase, regs, memsnap, run_to   # noqa

P = 0x400


class SkipCase(TailCase):
    def __init__(self, pcf_force=None, yc4_force=None, **kw):
        TailCase.__init__(self, **kw)
        self.pcf_force = pcf_force
        self.yc4_force = yc4_force

    def frame_tail(self, trig=0, dwid=0):
        e = self.e
        e.Y[P + 0x28] = trig
        e.Y[P + 0x1F] = (dwid & 0xFFFF) << 16
        e.run(0x0100, end=0x02EB, max_steps=500000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.Y[0x100 + i] = v & 0xFFFFFF
            self.phase += 2 * math.pi / 32.0
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(self.phase)))
            e.X[i] = v & 0xFFFFFF
            e.Y[i] = v & 0xFFFFFF
            self.phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0AD1, max_steps=500000)
        assert e.pc == 0x0AD1, "tail not reached pc=%04X" % e.pc
        # force the delay-length state -> reciprocal big -> bge taken
        # (S = low word of L:P+$CF asr 2; skip iff S <= $80 and S != 0;
        #  S == 0 hits the HW divide-by-zero guard -> B = 0 -> NO skip)
        if self.pcf_force is not None:
            e.Y[P + 0xCF] = self.pcf_force & 0xFFFFFF
            e.X[P + 0xCF] = 0
        if self.yc4_force is not None:
            e.Y[0xC4] = self.yc4_force & 0xFFFFFF
        rec = {"b0_ad1": dict(regs=regs(e), mem=memsnap(e))}
        run_to(e, 0x0AD1, 0x0AE9)
        rec["b1_ae9_pre"] = dict(regs=regs(e))
        e.step()                       # execute exactly the one bge instruction
        took = e.pc == 0x0B1E
        rec["branch"] = {"took_bge": took, "regs": regs(e)}
        if not took:
            rec["skip_note"] = "EXPECTED skip did not happen"
            run_to(e, 0x0AEA, 0x0B1E)
            rec["b7_b1e_postcopy"] = dict(regs=regs(e), mem=memsnap(e))
        run_to(e, e.pc, 0x0B33)
        rec["b8_b33_postdwid"] = dict(regs=regs(e), mem=memsnap(e))
        run_to(e, 0x0B33, 0x0B49)
        rec["b9_b49_postmix"] = dict(regs=regs(e), mem=memsnap(e))
        return rec


def run_grid():
    configs = [
        # atk, dec, bofs, wofs, dwid, pcf_force, yc4_force
        # skip needs B = asr17(div) - asr1(div*YC4) >= $10<<24:
        # with YC4 = 0.5: S (low word of L:P+CF asr2) must be <= 4
        (64, 64, 0, 0, 0, 0x10, 0x400000),
        (64, 64, 127, 127, 64, 0x8, 0x400000),
        (10, 100, 30, 90, 127, 0x10, 0),       # YC4=0 -> clr b ifeq -> NO skip
        (10, 100, 30, 90, 127, None, None),    # natural state control
    ]
    out = []
    for (atk, dec, bofs, wofs, dwid, pcf, yc4) in configs:
        c = SkipCase(pcf_force=pcf, yc4_force=yc4, filt_atk=atk, filt_dec=dec,
                     bofs=bofs, wofs=wofs)
        for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                       (0x12, 64), (0x13, 64)):
            c.e.Y[P + off] = (int(v) & 0xFFFF) << 16
        frames = []
        for k in range(2):
            rec = c.frame_tail(trig=1 if k == 0 else 0, dwid=dwid)
            rec["frame"] = k
            frames.append(rec)
        out.append(dict(atk=atk, dec=dec, bofs=bofs, wofs=wofs, dwid=dwid,
                        pcf=pcf, yc4=yc4, frames=frames))
        print("collected atk=%d dec=%d dwid=%d pcf=%s yc4=%s took=%s"
              % (atk, dec, dwid, pcf, yc4,
                 frames[0]["branch"]["took_bge"]), flush=True)
    with gzip.open("/home/z/my-project/scripts/exp22_skip.json.gz", "wt") as f:
        json.dump(out, f)
    print("saved exp22_skip.json.gz")


if __name__ == "__main__":
    run_grid()
