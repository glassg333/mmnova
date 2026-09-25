#!/usr/bin/env python3
"""exp31h — measure carrier frequency vs A_entry (kernel pitch value)."""
import sys
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, run_block

def run_n(e, n, params, a_entry):
    outs = []
    for i in range(n):
        outs.append(run_block(e, [0x400000] * 34, params, a_entry=a_entry))
    return outs

def car_seq(e, nblocks):
    """collect 16 carrier samples per block from Y:$63..$72"""
    seq = []
    for i in range(nblocks):
        run_block(e, [0x400000] * 34, [0, 0, 60, 127, 0, 0, 0, 64], a_entry=e._a)
        seq.extend(e.Y.get(a, 0) for a in range(0x63, 0x73))
    return seq

def zc_freq(seq):
    """zero crossings -> cycles per sample (rough, via sign changes /2)"""
    signs = [1 if (v & 0x800000) else 0 for v in seq]
    zc = sum(1 for i in range(1, len(signs)) if signs[i] != signs[i - 1])
    return zc / 2.0 / len(signs)

print("A_entry -> cycles/sample of carrier bank (16 words/block x 10 blocks)")
for a in (0x0E9150, 0x1D22A0, 0x3A4540, 0x748A80, 0x1D22A0000 >> 4):
    e = build()
    run_init(e)
    # warmup 3 blocks
    for i in range(3):
        run_block(e, [0x400000] * 34, [0, 0, 60, 127, 0, 0, 0, 64], a_entry=a)
    e._a = a
    seq = []
    for i in range(10):
        run_block(e, [0x400000] * 34, [0, 0, 60, 127, 0, 0, 0, 64], a_entry=a)
        seq.extend(e.Y.get(x, 0) for x in range(0x63, 0x73))
    f = zc_freq(seq)
    p10 = (e.X.get(0x538, 0) << 24) | e.Y.get(0x538, 0)
    print("A=%010X  p10=%012X  f=%0.6f cyc/smp  (fs44100 -> %8.1f Hz)" %
          (a, p10, f, f * 44100))
