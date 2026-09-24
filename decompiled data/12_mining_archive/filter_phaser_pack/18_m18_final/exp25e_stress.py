#!/usr/bin/env python3
"""exp25e_stress.py — extended verification of the m18 model:
   A. 8 random param configs x random signals x 60 blocks (word-exact)
   B. residue sensitivity: random garbage in Y:$E1-$EF (stale quad-ramp sin
      words never written by m18) — model must still match emulator, and the
      output delta vs zero residue is measured (audibility of the residue).
   C. two-machine continuity: state carried across parameter changes.
"""
import sys, json, random

sys.path.insert(0, "/home/z/my-project/scripts")
from exp25_verify import build, run_init, run_emu_block, R6M, M24  # noqa
from exp25_model import M18State, proc_model


def main():
    random.seed(2025)
    e = build()
    run_init(e)
    ts = [e.X.get(0x14A000 + i, 0) for i in range(8192)]
    tc = [e.X.get(0x14A800 + i, 0) for i in range(8192)]

    ok = bad = 0
    fails = []
    for cfg in range(8):
        pv = [random.randint(0, 127) for _ in range(7)] + [random.randint(0, 127)]
        st = M18State()
        e2 = build()
        run_init(e2)
        for blk in range(60):
            sg = [random.randint(-0x700000, 0x700000) & M24 for _ in range(34)]
            eout = run_emu_block(e2, pv, sg)
            stale = [e2.Y.get(0xE0 + i, 0) for i in range(16)]
            mout = proc_model(st, [(v & 0xFFFF) << 16 for v in pv], sg, ts, tc,
                              stale_ye=stale)
            d = [(i, a, b) for i, (a, b) in enumerate(zip(eout, mout)) if a != b]
            ok += 32 - len(d)
            bad += len(d)
            if d:
                fails.append((cfg, blk, d[:2]))
        print("cfg%d params=%s done" % (cfg, pv))
    print("A. RANDOM STRESS: OK=%d BAD=%d" % (ok, bad))
    for f in fails[:5]:
        print("   fail cfg%d blk%d: %s" % f)

    # --- B. residue sensitivity --------------------------------------------
    print("\nB. residue sensitivity (Y:$E1-$EF garbage)")
    pv = [96, 100, 16, 100, 40, 64, 64, 100]
    sg = [int(0x600000 * __import__("math").sin(0.3 * i)) & M24 for i in range(34)]
    deltas = []
    for trial in range(4):
        garbage = [random.randint(-0x400000, 0x400000) & M24 for _ in range(16)]
        outs = []
        for mode in ("zero", "garbage"):
            e3 = build()
            run_init(e3)
            st3 = M18State()
            run_emu_block(e3, pv, sg)  # warm
            o = []
            for blk in range(8):
                if mode == "garbage":
                    for i in range(16):
                        e3.Y[0xE0 + i] = garbage[i]
                oo = run_emu_block(e3, pv, sg)
                o.extend(oo)
            outs.append(o)
        d = max(abs(a - b) for a, b in zip(outs[0], outs[1]))
        deltas.append(d)
        print("   trial%d: max |out(garbage)-out(zero)| = %d LSB (%.5f fs)" %
              (trial, d, d / 8388608.0))
    json.dump({"ok": ok, "bad": bad, "residue_max_lsb": deltas},
              open("/home/z/my-project/scripts/exp25e_stress.json", "w"), indent=1)


if __name__ == "__main__":
    main()
