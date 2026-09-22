#!/usr/bin/env python3
"""exp24d_echo4.py — echo probe round 4: force host cells
   X:P+$D0 = $4000 (open the retune gate), X:$C5 = $4000 (publish ring)
   then: retune glide, DTIM->length map, DSND/DFB/DBAS/DWID routing.
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
    e.X[P + off] = (v >> 24) & M24
    e.Y[P + off] = v & M24


def build(tempo=120, lpq=0, base=64, force_host=True):
    e = build_track_emu(machine=1)
    setup_track(e, base=base, trig=1, tempo=tempo, lpq=lpq)
    if force_host:
        e.X[P + 0xD0] = 0x4000        # open retune gate (host cell)
        e.X[0xC5] = 0x4000            # publish ring pointer (host cell)
    return e


def run_n(e, n, trig=0):
    for i in range(n):
        e.Y[P + 0x28] = trig
        run_frame(e)


def probe_glide_forced():
    print("=== A. forced gate: target L_CD -> L_CF glide ===")
    for T in (1 << 24, 64 << 24, 2048 << 24):
        e = build(tempo=120 << 16, lpq=64)
        set_efx(e, dtim=64, dwid=64, dsnd=64, dbas=32)
        run_n(e, 4)
        set_L48(e, 0xCD, T)
        traj = []
        for i in range(14):
            e.X[P + 0xD0] = 0x4000
            e.Y[P + 0x28] = 0
            run_frame(e)
            traj.append(L48(e, 0xCF))
        T2 = T
        alphas = []
        for k in range(len(traj) - 1):
            d0, d1 = T2 - traj[k], T2 - traj[k + 1]
            if d0:
                alphas.append(1.0 - d1 / d0)
        print("T=%13d" % T, traj[:8], "alpha:", ["%.4f" % a for a in alphas[:6]])


def probe_dtim_final():
    print("=== B. DTIM -> delay length (armed + gate open) ===")
    for dtim in (0, 8, 16, 32, 48, 64, 80, 96, 112, 127):
        e = build(tempo=120 << 16, lpq=127)
        set_efx(e, dtim=dtim, dwid=64, dsnd=64, dbas=32, dfb=64)
        e.Y[P + 0x2A] = 127 << 16
        run_n(e, 30)
        taps = [(e.X[0x90 + i], e.Y[0x90 + i]) for i in (0, 1, 8, 16)]
        o = read_out(e)
        ring = [e.Y.get(0x4000 + i, 0) for i in (0, 1, 2)]
        print("DTIM=%3d L_CF=%011d taps0=(%06X,%06X) t8=(%06X,%06X) t16=(%06X,%06X) outpk=%9d ring=%s" % (
            dtim, L48(e, 0xCF), taps[0][0], taps[0][1], taps[1][0], taps[1][1],
            taps[2][0], taps[2][1], max(abs(v) for v in o),
            ["%06X" % r for r in ring]))


def probe_glide_step():
    print("=== C. retune glide DTIM step 32->96 ===")
    e = build(tempo=120 << 16, lpq=127)
    set_efx(e, dtim=32, dwid=64, dsnd=64, dbas=32, dfb=64)
    e.Y[P + 0x2A] = 127 << 16
    run_n(e, 30)
    pre = L48(e, 0xCF)
    print("pre L_CF =", pre)
    e.Y[P + 0x1B] = q(96)
    traj = [pre]
    for i in range(26):
        e.X[P + 0xD0] = 0x4000
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
    print("alpha:", ["%.4f" % a for a in alphas[:12]])


def probe_knobs_final():
    print("=== D. DSND/DFB/DBAS/DWID routing (armed + gate open) ===")
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
            ring = [e.Y.get(0x4000 + i, 0) for i in (0, 2)]
            echo = [e.X.get(0x300 + i, 0) for i in (0, 2)]
            print("%s=%3d L_CF=%011d outpk=%9d t0=(%06X,%06X) t16=(%06X,%06X) pubring=%s echo=%s" % (
                name, v, L48(e, 0xCF), max(abs(x) for x in o),
                taps[0][0], taps[0][1], taps[2][0], taps[2][1],
                ["%06X" % r for r in ring], ["%06X" % r for r in echo]))


if __name__ == "__main__":
    probe_glide_forced()
    probe_dtim_final()
    probe_glide_step()
    probe_knobs_final()
