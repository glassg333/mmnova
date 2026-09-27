#!/usr/bin/env python3
"""
exp40_td_detail.py — one-frame detailed trace of the delay region with
register snapshots at key PCs. Goal: exact data flow of send/feedback/sum.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
import exp39_td_ir2 as X39

KEY_PCS = [0x0939, 0x0949, 0x0953, 0x0961, 0x0976, 0x0981, 0x0A03, 0x0A06,
           0x0A10, 0x0A19, 0x0A22, 0x0A29, 0x0A2F, 0x0A38, 0x0A3F, 0x0A5D,
           0x0A63, 0x0A6F, 0x0A78, 0x0A80, 0x0A84, 0x0A8C, 0x0A9D, 0x0AA3,
           0x0AA9, 0x0AB7, 0x0ABF, 0x0AC5, 0x0ACF, 0x0AD1, 0x0AD5, 0x0AE5,
           0x0AEA, 0x0AFB, 0x0B0B, 0x0B13, 0x0B14, 0x0B18, 0x0B1E, 0x0B33,
           0x0B3E, 0x0B49]

def run_detail(params, impulse=True):
    e = X39.build_emu()
    sts = X39.impulse_train(3)
    # 3 warm frames silent, impulse on frame 0 here (reuse frame 0 = impulse)
    st = sts[0] if impulse else [0] * 34
    Y, X = e.Y, e.X
    regs = {}
    log = []
    e.watch = set(range(0x000, 0x100)) | {0x2C9, 0xFF}
    base_rd, base_wr = e.rd, e.wr
    def rd_hook(space, ea):
        if 0x0939 <= e.pc < 0x0B4D and ea in e.watch:
            log.append((e.pc, space + "R", ea, base_rd(space, ea)))
        return base_rd(space, ea)
    def wr_hook(space, ea, val):
        if 0x0939 <= e.pc < 0x0B4D and ea in e.watch:
            log.append((e.pc, space + "W", ea, val & 0xFFFFFF))
        return base_wr(space, ea, val)
    e.rd, e.wr = rd_hook, wr_hook
    old = e.step
    def st_hook():
        if e.pc in KEY_PCS:
            regs.setdefault(e.pc, []).append([e.R[i] for i in range(8)] +
                                             [e.M[i] for i in range(8)])
        old()
    e.step = st_hook
    X39.run_frames(e, [st, [0]*34], params)
    e.rd, e.wr = base_rd, base_wr
    e.step = old
    return log, regs, e

if __name__ == "__main__":
    p = dict(TIM=0.3, SND=0.5, FDB=0.4, BAS=0.5, WID=0.5)
    log, regs, e = run_detail(p)
    print("watch entries in delay window:", len(log))
    print("\n-- first 160 --")
    for pc, op, ea, val in log[:160]:
        print(f"P:{pc:04X} {op} {ea:04X} <= {val:06X}")
    print("\n-- regs at key PCs (first hit) --")
    for pc in KEY_PCS:
        if pc in regs:
            r = regs[pc][0]
            print(f"P:{pc:04X} r0-r7={['%04X'%v for v in r[:8]]} m0-m7={['%06X'%v for v in r[8:]]} (hits={len(regs[pc])})")
