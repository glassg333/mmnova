#!/usr/bin/env python3
"""
exp22_tail.py — Iteration 22: bit-exact dissection of the stage-2 tail
(P:$0AD1-$0B4C): reciprocal DIV, tap-phase integration L:$90-$A0,
de-zipper chain, func_397 call args, DWID/env block, master mix-down.

Method: sub-block breakpoints with full register dumps at every boundary,
across the exp18 parameter grid + DWID variations. Output: JSON snapshot
for the closed-form model (exp22_model.py).
"""
import sys, json, math

PK = "/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack"
sys.path.insert(0, PK + "/05_emulator")
sys.path.insert(0, PK + "/06_amp_env")
sys.path.insert(0, PK + "/11_stage2_model")
from amp_env_measure import EnvCase          # noqa: E402
from track_harness import build_track_emu    # noqa: E402

M24 = 0xFFFFFF
P = 0x400

SNAP_EXTRA = list(range(0x90, 0xA2)) + [0xC4, 0xC5, 0xFF, 0x2C9]

KEY = (list(range(0x00, 0x10)) + list(range(0x10, 0x20)) +
       list(range(0x20, 0x40)) + [0x40, 0x50, 0x51, 0x61] +
       list(range(0x70, 0x83)) +
       list(range(0x90, 0xA1)) + [0xC4, 0xC5, 0xFF, 0x2C9, 0x4CB, 0x4CF] +
       list(range(0x4000, 0x4010)) + list(range(0x300, 0x310)))


def regs(e):
    d = dict(pc=e.pc, A=e.A, B=e.B, x0=e.x0, x1=e.x1, y0=e.y0, y1=e.y1,
             R=[e.R[i] for i in range(8)], M=[e.M[i] for i in range(8)],
             N=[e.N[i] & M24 for i in range(8)], f=dict(e.f))
    return d


def memsnap(e):
    d = {}
    for sp in ("X", "Y"):
        m = getattr(e, sp)
        for a in KEY:
            d["%s:%03X" % (sp, a)] = m.get(a, 0) & M24
    for o in range(0x100):
        d["Y:P+%02X" % o] = e.Y.get(P + o, 0) & M24
        d["X:P+%02X" % o] = e.X.get(P + o, 0) & M24
    for o in range(0x20):
        d["Y:%04X" % o] = e.Y.get(o, 0) & M24
    return d


def run_to(e, start, end):
    """run from start; return pc at stop. If bge jumped past end, detect."""
    e.run(start, end=end, max_steps=400000)
    return e.pc


class TailCase(EnvCase):
    def frame_tail(self, trig=0, dwid=0):
        e = self.e
        e.Y[P + 0x28] = trig
        e.Y[P + 0x1F] = (dwid & 0xFFFF) << 16
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
        e.run(0x02EC, end=0x0AD1, max_steps=500000)
        assert e.pc == 0x0AD1, "tail not reached pc=%04X" % e.pc
        rec = {"b0_ad1": dict(regs=regs(e), mem=memsnap(e))}
        # DIV block
        run_to(e, 0x0AD1, 0x0AE9)
        rec["b1_ae9_pre"] = dict(regs=regs(e))
        # execute the bge
        nxt = 0x0AEA
        e.run(0x0AE9, end=0x0AEA, max_steps=100)
        if e.pc == 0x0B1E:
            nxt = 0x0B1E
        rec["branch"] = {"took_bge": nxt == 0x0B1E, "regs": regs(e)}
        if nxt == 0x0B1E:
            rec["skip_note"] = "integration+call+copy skipped"
        else:
            run_to(e, 0x0AEA, 0x0AFD)
            rec["b3_afd_postloop"] = dict(regs=regs(e))
            run_to(e, 0x0AFD, 0x0B0B)
            rec["b4_b0b_postdezzip"] = dict(regs=regs(e),
                                            mem=memsnap(e))
            run_to(e, 0x0B0B, 0x0B13)
            rec["b5_b13_precall"] = dict(regs=regs(e))
            run_to(e, 0x0B13, 0x0B14)
            rec["b6_b14_postcall"] = dict(regs=regs(e))
            run_to(e, 0x0B14, 0x0B1E)
            rec["b7_b1e_postcopy"] = dict(regs=regs(e), mem=memsnap(e))
        run_to(e, e.pc, 0x0B33)
        rec["b8_b33_postdwid"] = dict(regs=regs(e), mem=memsnap(e))
        run_to(e, 0x0B33, 0x0B49)
        rec["b9_b49_postmix"] = dict(regs=regs(e), mem=memsnap(e))
        run_to(e, 0x0B49, 0x0B4C)
        rec["bA_b4c_done"] = dict(regs=regs(e))
        return rec


def run_grid():
    configs = [
        # atk, dec, bofs, wofs, dwid
        (64, 64, 0, 0, 0), (64, 64, 127, 0, 64), (64, 64, 0, 127, 127),
        (64, 64, 127, 127, 0), (0, 64, 64, 64, 64), (127, 64, 64, 64, 127),
        (64, 0, 64, 64, 32), (64, 127, 64, 64, 0), (10, 100, 30, 90, 100),
        (100, 10, 90, 30, 1), (64, 64, 64, 64, 64), (1, 1, 1, 1, 127),
    ]
    nframes = 4
    out = []
    for (atk, dec, bofs, wofs, dwid) in configs:
        c = TailCase(filt_atk=atk, filt_dec=dec, bofs=bofs, wofs=wofs)
        for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
                       (0x12, 64), (0x13, 64)):
            c.e.Y[P + off] = (int(v) & 0xFFFF) << 16
        frames = []
        for k in range(nframes):
            rec = c.frame_tail(trig=1 if k == 0 else 0, dwid=dwid)
            rec["frame"] = k
            frames.append(rec)
        out.append(dict(atk=atk, dec=dec, bofs=bofs, wofs=wofs, dwid=dwid,
                        frames=frames))
        print("collected atk=%d dec=%d bofs=%d wofs=%d dwid=%d"
              % (atk, dec, bofs, wofs, dwid), flush=True)
    import gzip
    with gzip.open("/home/z/my-project/scripts/exp22_snap.json.gz", "wt") as f:
        json.dump(out, f)
    print("saved exp22_snap.json.gz")

    # ---- quick human summary of the first config / first frame ----
    f0 = out[0]["frames"][0]
    print("\n--- summary cfg0 fr0 ---")
    print("r7 =", hex(f0["b0_ad1"]["regs"]["R"][7]))
    print("took_bge:", f0["branch"]["took_bge"])
    print("P+$CF =", f0["b0_ad1"]["mem"]["X:P+CF"], f0["b0_ad1"]["mem"]["Y:P+CF"])
    print("X:C4 =", hex(f0["b0_ad1"]["mem"]["X:0C4"]),
          "X:C5 =", hex(f0["b0_ad1"]["mem"]["X:0C5"]),
          "Y:C4 =", f0["b0_ad1"]["mem"]["Y:0C4"],
          "X:FF =", hex(f0["b0_ad1"]["mem"]["X:0FF"]),
          "X:2C9 =", hex(f0["b0_ad1"]["mem"]["X:2C9"]))
    print("L:$90..A0 X:", ["%06X" % f0["b9_b49_postmix"]["mem"]["X:%03X" % a]
                          for a in range(0x90, 0xA1)])
    print("L:$90..A0 Y:", ["%06X" % f0["b9_b49_postmix"]["mem"]["Y:%03X" % a]
                           for a in range(0x90, 0xA1)])


if __name__ == "__main__":
    run_grid()
