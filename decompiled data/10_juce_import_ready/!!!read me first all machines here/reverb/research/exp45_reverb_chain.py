#!/usr/bin/env python3
"""exp45_reverb_chain.py — block-by-block energy through the m13 wet chain.
For each block: comb buffers (Y:$20-7F), matrix1 wet (X:$0-1F sampled at
P:$1455A8 entry), gate ramp, final out. Find where the impulse dies.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R

M24 = 0xFFFFFF


def sgn(v):
    v = int(v) & M24
    return v - (1 << 24) if v & 0x800000 else v


def peak(vals):
    return max(abs(sgn(v)) for v in vals) / 8388608.0


def main():
    e = R.build()
    R.run_init(e)
    R.set_params(e, dec=100, damp=2, gate=127, mix=127, inp=64)
    R.warmup(e, 6)
    R.run_block(e, R.sig_impulse(0, 0.9))

    wet_at_lpin = []
    base_rd = e.rd
    def rd_hook(sp, ea):
        if e.pc == 0x1455AD and sp == 'x':   # first wet cell read at LP stage
            wet_at_lpin.append(base_rd(sp, ea))
        return base_rd(sp, ea)
    e.rd = rd_hook

    print("blk |  c1     c6     | wet(LPin) | gate_ramp | out")
    for blk in range(1, 176):
        wet_at_lpin.clear()
        o = R.run_block(e, R.sig_zero())
        c1 = peak([e.Y.get(0x20 + i, 0) for i in range(16)])
        c6 = peak([e.Y.get(0x70 + i, 0) for i in range(16)])
        wp = peak(wet_at_lpin) if wet_at_lpin else -1
        gr = sgn(e.Y.get(0x0, 0)) / 8388608.0
        op = peak(o)
        if blk <= 45 or (c6 > 0 or wp > 0 or op > 0) and blk % 5 == 0:
            print(f"{blk:3d} | {c1:.5f} {c6:.5f} | {wp:.5f} | {gr:+.4f} | {op:.5f}")
    e.rd = base_rd


if __name__ == "__main__":
    main()
