#!/usr/bin/env python3
"""exp43_reverb_diag.py — why does the m13 wet path never produce output?
Watch all memory traffic of PROC $1453DC-$14561B during the impulse block:
 - writes to external delay banks Y:$114000..$117FFF
 - reads from those banks
 - values of the comb taps / accumulators at PCs of the 6 comb loops
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R

M24 = 0xFFFFFF


def run_impulse_traced(npre=2, npost=6):
    e = R.build()
    R.run_init(e)
    R.set_params(e, dec=64, damp=2, gate=127, mix=127, inp=64)
    R.warmup(e, 4)

    ext_w, ext_r = [], []
    base_rd, base_wr = e.rd, e.wr

    def rd_hook(space, ea):
        v = base_rd(space, ea)
        if 0x114000 <= ea < 0x118000 and 0x1453DC <= e.pc < 0x14561C:
            ext_r.append((e.pc, space, ea, v))
        return v

    def wr_hook(space, ea, val):
        if 0x114000 <= ea < 0x118000 and 0x1453DC <= e.pc < 0x14561C:
            ext_w.append((e.pc, space, ea, val & M24))
        return base_wr(space, ea, val)

    e.rd, e.wr = rd_hook, wr_hook
    for blk in range(npre + 1 + npost):
        s = R.sig_impulse(0, 0.5) if blk == npre else R.sig_zero()
        o = R.run_block(e, s)
        if blk == npre:
            imp_w = list(ext_w)
            imp_r = list(ext_r)
        if blk > npre:
            post_w = list(ext_w)
            break
    e.rd, e.wr = base_rd, base_wr

    print(f"impulse block: ext writes={len(imp_w)} ext reads={len(imp_r)}")
    print("first 24 writes:")
    for pc, sp, ea, v in imp_w[:24]:
        print(f"  P:{pc:06X} {sp}W {ea:06X} <= {v:06X}")
    print("first 12 reads:")
    for pc, sp, ea, v in imp_r[:12]:
        print(f"  P:{pc:06X} {sp}R {ea:06X} -> {v:06X}")
    if len(imp_w) > 24:
        print("last 8 writes:")
        for pc, sp, ea, v in imp_w[-8:]:
            print(f"  P:{pc:06X} {sp}W {ea:06X} <= {v:06X}")
    # after block
    if 'post_w' in dir():
        pass
    return e, imp_w, imp_r


if __name__ == "__main__":
    run_impulse_traced()
