#!/usr/bin/env python3
"""fm_measure1.py — measure FM-STAT (m8) carrier pitch law + phase increments."""
import sys, math, json
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_block, R6M, M24


def sgn48(hi, lo):
    v = ((hi & M24) << 24) | (lo & M24)
    return v - (1 << 48) if v & (1 << 47) else v


def words_per_cycle(outs):
    """count positive zero crossings over the stream; L/R interleaved"""
    zc = sum(1 for i in range(1, len(outs)) if outs[i - 1] <= 0 < outs[i])
    return len(outs) / (zc / 2.0) if zc else float("inf")


print("=== 1. carrier pitch law (1ENV=0, 2VOL=0, 1FB=0 -> pure carrier) ===")
rows = []
for A in (250, 500, 1000, 2000, 4000, 8000, 16000):
    e = build()
    fm_init(e, 8)
    p = [64, 64, 0, 0, 64, 0, 127, 64]
    outs = []
    for _ in range(12):
        outs += fm_block(e, 8, p, A)
    wpc = words_per_cycle(outs)
    L5 = sgn48(e.Y[5], e.X[5])  # L:$5 pair: X:$5=hi? check both
    ph = (e.Y[R6M + 0x16], e.Y[R6M + 0x17])
    rows.append(dict(A=A, words_per_cycle=wpc))
    print("A=%6d  words/cycle=%9.3f  L5(X,Y)=%06X/%06X  ph=%06X/%06X" % (
        A, wpc, e.X[5], e.Y[5], ph[0], ph[1]))

# phase increment per iteration vs A (from r6+$16/$17 deltas over blocks)
print()
print("=== 2. carrier phase increment per iteration ===")
for A in (1000, 4000, 16000):
    e = build()
    fm_init(e, 8)
    p = [64, 64, 0, 0, 64, 0, 127, 64]
    fm_block(e, 8, p, A)  # warm up (ramp)
    fm_block(e, 8, p, A)
    ph0 = sgn48(e.Y[R6M + 0x16], e.Y[R6M + 0x17])
    fm_block(e, 8, p, A)
    ph1 = sgn48(e.Y[R6M + 0x16], e.Y[R6M + 0x17])
    # phase stored = masked&base each block -> delta of (ph & 0x1FFF) w/ wrap
    m0 = ph0 & 0x1FFF
    m1 = ph1 & 0x1FFF
    d = (m1 - m0) % 8192
    print("A=%6d  dphase/block=%d  dphase/iter=%.4f table-units  inc/A=%.6f" % (
        A, d, d / 32.0, d / 32.0 / A))
print()
print("(expected inc_tu = A * 0x0BE37C / 2^24 = A*0.045813 if raw 48-bit int path)")
print("A=1000 -> %.4f tu/iter" % (1000 * 0x0BE37C / 2**24))
