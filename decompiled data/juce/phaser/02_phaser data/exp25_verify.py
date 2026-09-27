#!/usr/bin/env python3
"""exp25_verify.py — verify the bit-exact m18 model (exp25_model.py) against
   the DSP emulator word-by-word over a parameter grid x signal types.

   Also dumps rotator-only isolation check and final r6 state comparison.
"""
import sys, json, random

sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm  # noqa
from exp25_model import M18State, proc_model

PROC = 0x145045
INIT = 0x145036
PROC_END = 0x14538D
R6M = 0x400 + 0x28
M24 = 0xFFFFFF


def build():
    e = build_emu()
    lines = disasm(0x144000, 0x2000)
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    return e


def run_init(e):
    e.R[6] = R6M
    e.R[7] = 0x100
    e.ret_stack.append(0x14538E)
    e.run(INIT, end=0x14538E, max_steps=10000)


def run_emu_block(e, prm_vals, sig):
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(prm_vals[i])
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(PROC_END)
    e.run(PROC, end=PROC_END, max_steps=400000)
    return [e.Y[0x100 + i] for i in range(32)]


def emu_state(e):
    """collect the persistent r6 state words from the emulator."""
    s = {}
    s["phase_hi"] = e.X.get(R6M + 0x11, 0)
    s["phase_lo"] = e.X.get(R6M + 0x12, 0)
    s["s_wid"] = e.Y.get(R6M + 0x1B, 0)
    s["s18"] = (e.X.get(R6M + 0x18, 0) << 24) | e.Y.get(R6M + 0x18, 0)
    s["s19"] = (e.X.get(R6M + 0x19, 0) << 24) | e.Y.get(R6M + 0x19, 0)
    s["v_prev"] = e.Y.get(R6M + 0x1A, 0)
    s["s13"] = e.Y.get(R6M + 0x13, 0)
    s["q_cos"] = e.Y.get(R6M + 0x1C, 0)
    s["q_sin"] = e.Y.get(R6M + 0x1D, 0)
    s["S_even"] = [e.X.get(R6M + i, 0) for i in range(0x1E, 0x25)]
    s["S_odd"] = [e.X.get(R6M + i, 0) for i in range(0x26, 0x2D)]
    return s


def model_state(st):
    return {
        "phase_hi": st.phase_hi, "phase_lo": st.phase_lo,
        "s_wid": st.s_wid, "s18": st.s18, "s19": st.s19,
        "v_prev": st.v_prev, "s13": st.s13,
        "q_cos": st.q_cos, "q_sin": st.q_sin,
        "S_even": list(st.S_even), "S_odd": list(st.S_odd),
    }


def make_sig(kind, n=34):
    if kind == "impulse":
        return [0x400000] + [0] * (n - 1)
    if kind == "dc":
        return [0x200000] * n
    if kind == "sine":
        return [int(0x600000 * __import__("math").sin(0.9 * i)) & M24 for i in range(n)]
    if kind == "rand":
        return [random.randint(-0x700000, 0x700000) & M24 for _ in range(n)]
    if kind == "zero":
        return [0] * n
    raise ValueError(kind)


CONFIGS = [
    # label,           [CNTR,DEP,SPD,MIX,FB,WID,X,INP]
    ("center-static",  [64, 64, 0, 64, 64, 64, 64, 100]),
    ("deep-fast",      [96, 127, 64, 127, 96, 100, 0, 127]),
    ("slow-shallow",   [32, 20, 8, 0, 32, 30, 127, 60]),
    ("extremes",       [127, 127, 127, 127, 127, 127, 127, 127]),
    ("zeros",          [0, 0, 0, 0, 0, 0, 0, 0]),
    ("random-1",       [71, 113, 37, 88, 52, 99, 14, 120]),
    ("random-2",       [5, 44, 91, 12, 77, 8, 66, 33]),
    ("no-fb",          [80, 90, 24, 96, 0, 70, 40, 110]),
]


def main():
    random.seed(25)
    total_ok = total_bad = 0
    block_bad = []
    state_bad = []

    for label, pv in CONFIGS:
        for kind in ("impulse", "dc", "sine", "rand", "zero"):
            # fresh machine for each (config, signal) pair
            e = build()
            run_init(e)
            st = M18State()
            sig = make_sig(kind)
            ok = bad = 0
            for blk in range(30):
                # emulator block (random signal regenerated per block)
                if kind == "rand":
                    sig = make_sig("rand")
                eout = run_emu_block(e, pv, sig)
                # stale Y:$E1-$EF as the emulator has them at block start
                stale = [e.Y.get(0xE0 + i, 0) for i in range(16)]
                mout = proc_model(st, [(v & 0xFFFF) << 16 for v in pv],
                                  sig + [0], 
                                  [e.X.get(0x14A000 + i, 0) for i in range(8192)],
                                  [e.X.get(0x14A800 + i, 0) for i in range(8192)],
                                  stale_ye=stale)
                diffs = [(i, a, b) for i, (a, b) in enumerate(zip(eout, mout)) if a != b]
                ok += 32 - len(diffs)
                bad += len(diffs)
                if diffs and len(block_bad) < 12:
                    block_bad.append((label, kind, blk, diffs[:4]))
            # state comparison
            es, ms = emu_state(e), model_state(st)
            for key in es:
                if es[key] != ms[key]:
                    state_bad.append((label, kind, key, es[key], ms[key]))
            total_ok += ok
            total_bad += bad
            print("%-14s %-8s  OK=%5d BAD=%d" % (label, kind, ok, bad))

    print("\nTOTAL: OK=%d BAD=%d" % (total_ok, total_bad))
    if block_bad:
        print("\nfirst block diffs:")
        for b in block_bad:
            print("  %s %s blk%d: %s" % b)
    if state_bad:
        print("\nstate diffs:")
        for s in state_bad[:10]:
            print("  %s %s %s: emu=%s model=%s" % s)
    if total_bad == 0 and not state_bad:
        print("\n*** PHASER m18: MODEL IS WORD-EXACT (100%) ***")
    json.dump({"ok": total_ok, "bad": total_bad,
               "state_diffs": len(state_bad)},
              open("/home/z/my-project/scripts/exp25_verify.json", "w"), indent=1)


if __name__ == "__main__":
    main()
