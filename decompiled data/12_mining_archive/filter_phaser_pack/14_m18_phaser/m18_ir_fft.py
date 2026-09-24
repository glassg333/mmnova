#!/usr/bin/env python3
"""exp23_m18_ir.py — full impulse response of m18 (frozen LFO, SPD=0),
FFT -> notch frequencies vs CNTR."""
import sys, math, json

sys.path.insert(0, "/home/z/my-project/scripts")
from m18_harness import build, run_init, run_block, f  # noqa

NB = 256  # blocks -> 4096 samples


def ir(cntr, dep=100, spd=0):
    e = build()
    run_init(e)
    params = [cntr, dep, spd, 100, 40, 64, 64, 100]
    outs = []
    for b in range(NB):
        sig = [0] * 34
        if b == 0:
            sig[0] = 0x400000
        out = run_block(e, sig, params)
        outs.extend(out)
    return outs


def notches(x, sr=48000.0, thr=0.05):
    n = len(x)
    

    # simple radix-2 not needed: use direct DFT on 2048 points (n>=2048)
    N = 2048
    peak = max(abs(v) for v in x[:N])
    res = []
    prev = None
    for k in range(1, N // 2):
        re = sum(x[i] * math.cos(2 * math.pi * i * k / N) for i in range(N))
        im = -sum(x[i] * math.sin(2 * math.pi * i * k / N) for i in range(N))
        mag = math.hypot(re, im) / (peak or 1)
        res.append((k * sr / N, mag))
    # local minima below threshold
    mins = []
    for i in range(2, len(res) - 2):
        fr, m = res[i]
        if m < thr and m <= res[i - 1][1] and m <= res[i + 1][1]:
            mins.append((round(fr, 1), round(m, 3)))
    return mins, res


out = {}
for cntr in (32, 64, 96):
    x = [f(v) for v in ir(cntr)]
    mins, _ = notches(x)
    out["cntr%d" % cntr] = {"ir_head": x[:16], "notches": mins}
    print("cntr=%3d: ir[0:8] = %s" % (cntr, " ".join("%+.4f" % v for v in x[:8])))
    print("         notches (Hz, mag):", mins[:12])

json.dump(out, open("/home/z/my-project/scripts/exp23_m18_notches.json", "w"))
print("saved exp23_m18_notches.json")
