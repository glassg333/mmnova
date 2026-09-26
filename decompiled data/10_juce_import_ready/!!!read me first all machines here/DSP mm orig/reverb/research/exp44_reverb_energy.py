#!/usr/bin/env python3
"""exp44_reverb_energy.py — trace impulse energy through m13 stages.
Stages: input bus X:$200 -> pre Y:$20-? -> comb lines (ext $114000+) ->
comb out Y:$20-2F -> sum X:$20-5F -> allpass Y:$0-1F -> LP -> wet X:$0-1F -> out.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R

M24 = 0xFFFFFF


def sgn(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v


def peak(vals):
    return max(abs(sgn(v)) for v in vals) / 8388608.0


def snap(e, tag):
    wet = [e.X.get(i, 0) for i in range(0x20)]
    combbuf = [e.Y.get(0x20 + i, 0) for i in range(0x10)]
    apbuf = [e.Y.get(i, 0) for i in range(0x10)]
    c6 = [e.X.get(0x116800 + i, 0) for i in range(0x200)]
    print(f"{tag}: wet={peak(wet):.5f} combBuf={peak(combbuf):.5f} "
          f"apBuf={peak(apbuf):.5f} comb6line={peak(c6):.5f}")


def main():
    e = R.build()
    R.run_init(e)
    R.set_params(e, dec=100, damp=2, gate=127, mix=127, inp=64)
    R.warmup(e, 6)
    ptrs = {c: e.Y.get(R.R6M + c, 0) for c in (0x33, 0x37, 0x39, 0x3E)}
    print("comb ptrs:", {hex(k): hex(v) for k, v in ptrs.items()})

    # impulse block
    o = R.run_block(e, R.sig_impulse(0, 0.9))
    snap(e, "imp-blk  out(16Y:100)=" + str(peak(o)))
    for blk in range(1, 46):
        o = R.run_block(e, R.sig_zero())
        if blk in (1, 2, 4, 8, 16, 30, 37, 38, 39, 40, 42, 45):
            snap(e, f"blk{blk:3d}  out={peak(o):.5f}")
    # where did the impulse land in comb6 line? find max cell
    best = max(range(0x200), key=lambda i: abs(sgn(e.X.get(0x116800 + i, 0))))
    print(f"comb6 line peak at +{best} ({best/16:.1f} blocks), "
          f"val={sgn(e.X.get(0x116800+best,0))/8388608:.5f}")
    # check all 6 line regions for energy
    for name, base in (("c1", 0x114000), ("c2", 0x115000), ("c3", 0x116000),
                       ("c4", 0x116800), ("c5", 0x117000), ("c6", 0x114C00)):
        vals = [abs(sgn(e.X.get(base + i, 0))) for i in range(0x400)]
        print(f"  {name} @ {base:06X}: peak={max(vals)/8388608:.5f} "
              f"nz={sum(1 for v in vals if v)}")


if __name__ == "__main__":
    main()
