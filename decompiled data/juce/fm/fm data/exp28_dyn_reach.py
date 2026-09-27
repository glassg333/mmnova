#!/usr/bin/env python3
"""exp28_dyn_reach.py v2 — m10 DYN: early-rts search + shaper reachability.

No bracket-m literals anywhere (IM gateway eats them) - uses mch var names.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_conf, RTS, MACH, R6M, s24

PROC_END = RTS[10]["proc"]      # 0x14636E
SHAPER_ZONE = 0x14636F


def trace_proc(e, pitch, max_steps=400000):
    seen = set()
    trail = []
    e.R[6] = R6M
    e.R[7] = 0x100
    for r in range(8):
        e.M[r] = 0xFFFFFF
    e.A = pitch & 0xFFFFFFFFFFFF
    e.pc = MACH[10]["proc"]
    e.ret_stack.append(0xDEAD)
    reason = "reached-rts"
    steps = 0
    try:
        while e.pc != PROC_END and steps < max_steps:
            seen.add(e.pc)
            trail.append(e.pc)
            e.step()
            steps += 1
    except Exception as ex:
        reason = "err:" + str(ex)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()
    return seen, reason, trail, steps


def zone_stat(seen):
    tail = sorted(x for x in seen if x >= SHAPER_ZONE)
    keys = {
        "out-pair(146355)": 0x146355 in seen,
        "reset(14636F)": 0x14636F in seen,
        "speed(146382)": 0x146382 in seen,
        "dispatch(1463E5)": 0x1463E5 in seen,
        "post(1463FB)": 0x1463FB in seen,
    }
    return tail, keys


if __name__ == "__main__":
    pitch = 11776
    cases = [
        (100, 64, 127, 0, 64, 127, 0, 0, "VEN=0  FRQ2=64"),
        (100, 64, 127, 40, 64, 127, 0, 0, "VEN=40 FRQ2=64"),
        (100, 64, 127, 64, 64, 127, 0, 0, "VEN=64 FRQ2=64"),
        (100, 64, 127, 90, 64, 127, 0, 0, "VEN=90 FRQ2=64"),
        (100, 64, 127, 127, 64, 127, 0, 0, "VEN=127 FRQ2=64"),
        (100, 64, 127, 64, 96, 127, 0, 0, "VEN=64 FRQ2=96(bit5)"),
        (100, 64, 127, 64, 64, 20, 0, 0, "VEN=64 ENV2=20(gate shut)"),
    ]
    e = build()
    for i, c in enumerate(cases):
        frq1, fen1, vol1, ven, frq2, env2, fb2, tune = c[:8]
        lbl = c[8]
        p = [frq1, fen1, vol1, ven, frq2, env2, fb2, tune]
        fm_init(e, 10)
        fm_conf(e, 10, p)
        seen, reason, trail, steps = trace_proc(e, pitch)
        tail, keys = zone_stat(seen)
        ks = " ".join(k + ("=Y" if v else "=n") for k, v in keys.items())
        print("case %d [%s]: steps=%d reason=%s" % (i, lbl, steps, reason))
        print("   " + ks)
        print("   tail-pcs: %d  max=%06X" % (len(tail), max(seen)))
        if reason != "reached-rts":
            print("   last pcs:", " ".join("%06X" % x for x in trail[-12:]))
        out = [s24(e.Y.get(0x100 + j, 0)) for j in range(8)]
        print("   out[:8]:", " ".join("%7d" % (v >> 8) for v in out))
