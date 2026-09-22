#!/usr/bin/env python3
"""exp24c_echo3.py — echo probe round 3:
   1. force delay-length TARGET (page+$CD L-pair) directly -> measure retune glide
      of L:P+$CF (kernel $09CB-$09EE one-pole 0.1/0.9, gate X:P+$D0 >= $3FF0)
   2. arm via TEMPO<<16 + LPQ sweep -> DTIM mapping through P$2A
   3. with armed delay: DSND/DFB/DBAS/DWID effects
"""
import sys

PACK = "/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack/05_emulator"
sys.path.insert(0, PACK)
from track_harness import build_track_emu, setup_track, run_frame, read_out  # noqa

P = 0x400
M24 = 0xFFFFFF


def q(v):
    return (int(v) & 0xFFFF) << 16


def set_efx(e, eqf=0, eqg=0, srr=0, dtim=0, dsnd=0, dfb=0, dbas=0, dwid=0):
    e.Y[P + 0x18] = q(eqf); e.Y[P + 0x19] = q(eqg)
    e.Y[P + 0x1A] = q(srr); e.Y[P + 0x1B] = q(dtim)
    e.Y[P + 0x1C] = q(dsnd); e.Y[P + 0x1D] = q(dfb)
    e.Y[P + 0x1E] = q(dbas); e.Y[P + 0x1F] = q(dwid)


def L48(e, off):
    x, y = e.X[P + off], e.Y[P + off]
    v = (x << 24) | y
    if x & 0x800000:
        v -= 1 << 48
    return v


def set_L48(e, off, v):
    x = (v >> 24) & M24
    y = v & M24
    e.X[P + off] = x
    e.Y[P + off] = y


def build(tempo=120, lpq=0, base=64):
    e = build_track_emu(machine=1)
    setup_track(e, base=base, trig=1, tempo=tempo, lpq=lpq)
    return e


def run_n(e, n, trig=0):
    for i in range(n):
        e.Y[P + 0x28] = trig
        run_frame(e)


def probe_forced_target():
    print("=== A. forced target L_CD -> glide of L_CF (retune speed) ===")
    for T in (1 << 24, 16 << 24, 128 << 24, 2048 << 24):
        e = build(tempo=120 << 16, lpq=64)
        set_efx(e, dtim=64, dwid=64, dsnd=64, dbas=32)
        run_n(e, 6)
        set_L48(e, 0xCD, T)
        traj = []
        for i in range(16):
            e.Y[P + 0x28] = 0
            run_frame(e)
            traj.append(L48(e, 0xCF))
        print("T=%13d traj:" % T, traj)
        # alpha fit
        alphas = []
        for k in range(len(traj) - 2):
            d0 = T - traj[k]
            d1 = T - traj[k + 1]
            if d0:
                alphas.append(1.0 - d1 / d0)
        print("   alpha:", ["%.4f" % a for a in alphas[:8]])


def probe_arm2():
    print("=== B. arm via TEMPO<<16, LPQ sweep, P2A sweep ===")
    for lpq in (0, 64, 100, 127):
        for p2a in (0, 64 << 16, 127 << 16):
            e = build(tempo=120 << 16, lpq=lpq)
            set_efx(e, dtim=64, dwid=64, dsnd=64, dbas=32)
            e.Y[P + 0x2A] = p2a
            run_n(e, 20)
            print("lpq=%3d P2A=%8d L_CD=%011d L_CF=%011d XD0=%06X taps16X=%06X outpk=%9d" % (
                lpq, p2a, L48(e, 0xCD), L48(e, 0xCF), e.X[P + 0xD0],
                e.X[0x90 + 16], max(abs(v) for v in read_out(e))))


def probe_dtim_map2():
    print("=== C. DTIM map (armed) ===")
    e0 = build(tempo=120 << 16, lpq=127)
    set_efx(e0, dtim=64, dwid=64, dsnd=64, dbas=32)
    e0.Y[P + 0x2A] = 127 << 16
    run_n(e0, 24)
    lc = L48(e0, 0xCF)
    print("sanity armed L_CF =", lc)
    if lc == 0:
        # force arm: big target
        set_L48(e0, 0xCD, 64 << 24)
        run_n(e0, 10)
        print("after forced target L_CF =", L48(e0, 0xCF))
    for dtim in (0, 16, 32, 48, 64, 80, 96, 112, 127):
        e = build(tempo=120 << 16, lpq=127)
        set_efx(e, dtim=dtim, dwid=64, dsnd=64, dbas=32)
        e.Y[P + 0x2A] = 127 << 16
        run_n(e, 24)
        taps = [(e.X[0x90 + i], e.Y[0x90 + i]) for i in (0, 8, 16)]
        print("DTIM=%3d L_CF=%011d L_CD=%011d taps=(%06X,%06X)(%06X,%06X)(%06X,%06X) outpk=%9d" % (
            dtim, L48(e, 0xCF), L48(e, 0xCD),
            taps[0][0], taps[0][1], taps[1][0], taps[1][1], taps[2][0], taps[2][1],
            max(abs(v) for v in read_out(e))))


def probe_glide2():
    print("=== D. retune glide: DTIM step 32->96 (armed path) ===")
    e = build(tempo=120 << 16, lpq=127)
    set_efx(e, dtim=32, dwid=64, dsnd=64, dbas=32)
    e.Y[P + 0x2A] = 127 << 16
    run_n(e, 30)
    pre = L48(e, 0xCF)
    e.Y[P + 0x1B] = q(96)
    traj = [pre]
    for i in range(24):
        e.Y[P + 0x28] = 0
        run_frame(e)
        traj.append(L48(e, 0xCF))
    print("glide:", traj)
    T = traj[-1]
    alphas = []
    for k in range(len(traj) - 2):
        d0, d1 = T - traj[k], T - traj[k + 1]
        if d0:
            alphas.append(1.0 - d1 / d0)
    print("alpha:", ["%.4f" % a for a in alphas[:10]])


def probe_knobs3():
    print("=== E. DSND/DFB/DBAS/DWID (armed) ===")
    for name, key in (("DSND", "dsnd"), ("DFB", "dfb"), ("DBAS", "dbas"), ("DWID", "dwid")):
        for v in (0, 32, 64, 127):
            e = build(tempo=120 << 16, lpq=127)
            kw = dict(dtim=64, dsnd=64, dfb=64, dbas=32, dwid=64)
            kw[key] = v
            set_efx(e, **kw)
            e.Y[P + 0x2A] = 127 << 16
            run_n(e, 30)
            o = read_out(e)
            taps = [(e.X[0x90 + i], e.Y[0x90 + i]) for i in (0, 8, 16)]
            ring = [e.Y.get(0x4000 + i, 0) for i in (0, 4, 8)]
            print("%s=%3d L_CF=%011d outpk=%9d taps16=(%06X,%06X) ring4000=%s" % (
                name, v, L48(e, 0xCF), max(abs(x) for x in o),
                taps[2][0], taps[2][1], ["%06X" % r for r in ring]))


if __name__ == "__main__":
    probe_forced_target()
    probe_arm2()
    probe_dtim_map2()
    probe_glide2()
    probe_knobs3()
