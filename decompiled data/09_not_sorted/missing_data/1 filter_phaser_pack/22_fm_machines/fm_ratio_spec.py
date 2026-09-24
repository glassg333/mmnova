#!/usr/bin/env python3
"""fm_ratio_spec.py — decisive ratio-table semantics via output spectrum.
With 2VOL=127 (op2 on), 1ENV=0 (op1 off): output = carrier PM'd by op2's diff
-> sidebands at f_carrier ± k*f_mod. The sideband spacing = f_mod exactly.
Compare f_mod/f_carrier against raw/2^19 vs raw/2^20 table interpretations.
"""
import sys, math, json
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_block

def sgn(v):
    return v - (1 << 24) if v & 0x800000 else v

def spectrum(seq):
    """amplitude spectrum via naive DFT at resolved peaks (coarse FFT via numpy if present)"""
    try:
        import numpy as np
        x = np.array(seq, dtype=float)
        x -= x.mean()
        w = np.hanning(len(x))
        S = np.abs(np.fft.rfft(x * w))
        return S, len(x)
    except ImportError:
        return None, len(seq)

def peaks(S, n, thr=0.05):
    mx = S.max()
    out = []
    for i in range(2, len(S) - 1):
        if S[i] > thr * mx and S[i] >= S[i - 1] and S[i] >= S[i + 1]:
            out.append((i, S[i] / mx))
    # merge adjacent
    merged = []
    for f, a in out:
        if merged and f - merged[-1][0] <= 2:
            if a > merged[-1][1]:
                merged[-1] = (f, a)
        else:
            merged.append((f, a))
    return merged

A = 2000
f_carrier = 185 * 88200.0 / 8192  # 1991.8 Hz if 88.2k iter rate
print("assumed f_carrier = %.1f Hz (inc=185 @88.2k)" % f_carrier)
raw = [0x004000,0x008000,0x010000,0x018000,0x020000,0x028000,0x030000,0x040000,
       0x050000,0x060000,0x070000,0x080000,0x0A0000,0x0C0000,0x0E0000,0x100000,
       0x140000,0x180000,0x1C0000,0x200000,0x280000,0x300000,0x380000,0x400000]

for knob in (48, 80, 96, 112, 127):
    n = (((knob << 16) + 0x8000) * 24) >> 24
    e = build(); fm_init(e, 8)
    p = [64, 64, 0, 0, knob, 127, 127, 64]
    seq = []
    for _ in range(96):
        seq += fm_block(e, 8, p, A)
    S, N = spectrum(seq[64:])
    pk = peaks(S, N)
    # block rate = 2756.25; N samples = 94*32 words -> freq per bin = 2756.25/32 per word
    fbin = 2756.25 / 32.0
    top = [(round(f * fbin, 1), round(a, 3)) for f, a in pk[:8]]
    # sideband spacing around the strongest region
    print("2FRQ=%3d idx=%2d raw=%06X  peaks(Hz,rel): %s" % (knob, n, raw[n], top))
    print("   raw/2^19=%.4f -> f_mod=%.1f | raw/2^20=%.4f -> f_mod=%.1f | f_car=%.1f" % (
        raw[n] / 2**19, raw[n] / 2**19 * f_carrier, raw[n] / 2**20, raw[n] / 2**20 * f_carrier, f_carrier))
