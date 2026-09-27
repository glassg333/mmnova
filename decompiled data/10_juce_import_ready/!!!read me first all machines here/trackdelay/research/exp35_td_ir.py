#!/usr/bin/env python3
"""
exp35_td_ir.py — Track Delay impulse response, take 2.
Correct machine-output offsets (audio at P+$E6..$F5). Runs the post-voice
chain $04A8-$0B4C per block, state persists. Captures output cells + delay
state. First: locate where the impulse lands and where the block output goes.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, Q23

P = 0x400
M24 = 0xFFFFFF

def q23(x):
    return int(round(x * (1 << 23))) & M24

def run_frame(e, mach_out, params):
    """mach_out: 34-word list (P+$DC..$FF). Returns 48 outputs Y:$0000-$002F, X:$0140-$015F."""
    Y, X = e.Y, e.X
    e.R[6] = P
    e.R[7] = 0
    e.ret_stack.clear()
    e.ret_stack.append(0x0B4C)
    for i in range(0x200):
        Y[P + i] = 0
        X[P + i] = 0
    Y[P + 0x1A] = q23(params.get('SRR', 0.0))
    Y[P + 0x1B] = q23(params.get('TIM', 0.3))
    Y[P + 0x1C] = q23(params.get('SND', 0.5))
    Y[P + 0x1D] = q23(params.get('FDB', 0.0))
    Y[P + 0x1E] = q23(params.get('BAS', 0.5))
    Y[P + 0x1F] = q23(params.get('WID', 0.5))
    Y[P + 0x28] = 1
    Y[P + 0x0E] = 0
    Y[P + 0x0F] = 0
    X[P + 0x0F] = 1
    Y[P + 0x10] = 0x400000
    Y[P + 0x11] = 0x400000
    Y[P + 0x12] = 0
    Y[P + 0x13] = 0
    Y[P + 0x14] = 0x400000
    Y[P + 0x15] = 0x400000
    Y[P + 0x16] = 0x400000
    Y[P + 0x17] = 0x400000
    for i, v in enumerate(mach_out):
        Y[P + 0xDC + i] = v & M24
    # clear output/mix area
    for a in range(0x000, 0x30):
        X[a] = 0
        Y[a] = 0
    e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
    outY = [Y[a] for a in range(0x00, 0x20)]
    outX = [X[a] for a in range(0x00, 0x20)]
    return outY, outX

if __name__ == "__main__":
    e = build_emu()
    # impulse in different machine-out slots, see what moves
    for slot in (0xE6, 0xF5, 0xDC, 0xEC):
        mach = [0] * 34
        mach[slot - 0xDC] = 0x400000
        oY, oX = run_frame(e, mach, dict(TIM=0.3, SND=0.5, FDB=0.0, BAS=0.5, WID=0.5))
        nzo = [(i, v) for i, v in enumerate(oY + oX) if v]
        print(f"\nslot P+{slot:02X}: nonzero out={nzo[:12]}")
        # key delay cells
        X = e.X
        print(f"   X40={X.get(0x40,0):06X} X44={X.get(0x44,0):06X} X90={X.get(0x90,0):06X} "
              f"XC2={X.get(0xC2,0):06X} XC4={X.get(0xC4,0):06X} XC5={X.get(0xC5,0):06X} XFE={X.get(0xFE,0):06X}")
