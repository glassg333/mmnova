#!/usr/bin/env python3
"""exp31c — find the input range of machine 17 by injecting DC into X ranges."""
import sys
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, run_block

def probe(rng, a_entry=0x123456):
    e = build()
    run_init(e)
    sig = {a: 0x400000 for a in rng}
    out = run_block(e, sig, [0, 0, 60, 127, 0, 0, 0, 64], a_entry=a_entry)
    return sum(abs(v) for v in out), out[:8]

rngs = {
    "X:00-0F": range(0x00, 0x10),
    "X:10-1F": range(0x10, 0x20),
    "X:20-2F": range(0x20, 0x30),
    "X:30-3F": range(0x30, 0x40),
    "X:40-4F": range(0x40, 0x50),
    "X:50-5F": range(0x50, 0x60),
    "X:60-7F": range(0x60, 0x80),
    "X:200-221": range(0x200, 0x222),
}
for name, rng in rngs.items():
    s, o = probe(rng)
    print("%-12s sum|out|=%12d  out8: %s" % (name, s, " ".join(str(v >> 8) for v in o)))
