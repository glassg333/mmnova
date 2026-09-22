#!/usr/bin/env python3
"""exp24b_echo2.py — arm the per-voice delay (kernel $0985-$09EE) and measure:
   - the arming gate: tbl1[LPQ]*TEMPO-scale vs P+$2A target
   - DTIM knob -> delay length L:P+$CF mapping (through host cell P+$2A)
   - retune glide after a DTIM step
   - DSND/DFB/DBAS with the delay armed
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
    e.Y[P + 0x18] = q(eqf)
    e.Y[P + 0x19] = q(eqg)
    e.Y[P + 0x1A] = q(srr)
    e.Y[P + 0x1B] = q(dtim)
    e.Y[P + 0x1C] = q(dsnd)
    e.Y[P + 0x1D] = q(dfb)
    e.Y[P + 0x1E] = q(dbas)
    e.Y[P + 0x1F] = q(dwid)


def L48(e, off):
    x, y = e.X[P + off], e.Y[P + off]
    v = (x << 24) | y
    if x & 0x800000:
        v -= 1 << 48
    return v


def build(tempo=120, lpq=0, base=64):
    e = build_track_emu(machine=1)
    setup_track(e, base=base, trig=1, tempo=tempo, lpq=lpq)
    return e


def f24(v):
    v &= M24
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0


def run_n(e, n, rearm=0):
    for i in range(n):
        e.Y[P + 0x28] = rearm
        run_frame(e)


def probe_arm():
    print("=== A. arming gate: LPQ x TEMPO x P$2A ===")
    for tempo in (120,):
        for lpq in (0, 32, 64, 96, 127):
            for v2a in (0, 64, 4096, 64 << 16):
                e = build(tempo=tempo, lpq=lpq)
                set_efx(e, dtim=64, dwid=64, dsnd=64, dbas=32)
                e.Y[P + 0x2A] = v2a
                run_n(e, 24)
                lc = L48(e, 0xCF)
                print("tempo=%3d lpq=%3d P2A=%8d -> L_CF=%011d L_CD=%011d taps16X=%06X outpk=%9d" % (
                    tempo, lpq, v2a, lc, L48(e, 0xCD), e.X[0x90 + 16],
                    max(abs(v) for v in read_out(e))))


def probe_dtim_map():
    print("=== B. DTIM mapping (delay armed) ===")
    best = None
    for lpq in (32, 64, 96, 127):
        for v2a in (64 << 16, 127 << 16):
            e = build(tempo=120, lpq=lpq)
            set_efx(e, dtim=64, dwid=64, dsnd=64, dbas=32)
            e.Y[P + 0x2A] = v2a
            run_n(e, 24)
            lc = L48(e, 0xCF)
            tag = (lc, lpq, v2a)
            if best is None or abs(lc) > abs(best[0]):
                best = tag
            print("lpq=%3d P2A=%8d -> L_CF=%011d" % (lpq, v2a, lc))
    print("best:", best)
    if not best or best[0] == 0:
        print("!! delay never armed — dump D0/D1/CE diagnostics")
        e = build(tempo=120, lpq=127)
        set_efx(e, dtim=64, dwid=64, dsnd=64, dbas=32)
        e.Y[P + 0x2A] = 127 << 16
        for i in range(12):
            e.Y[P + 0x28] = 0
            run_frame(e)
            print("f%02d L_CD=%011d L_CE=%011d L_CF=%011d YD0=%06X XD0=%06X YD1=%06X Y2A=%06X Y4FF=%06X" % (
                i, L48(e, 0xCD), L48(e, 0xCE), L48(e, 0xCF),
                e.Y[P + 0xD0], e.X[P + 0xD0], e.Y[P + 0xD1], e.Y[P + 0x2A], e.Y[0x4FF]))


def probe_glide():
    print("=== C. retune glide after DTIM step (if armed) ===")
    e = build(tempo=120, lpq=127)
    set_efx(e, dtim=32, dwid=64, dsnd=64, dbas=32)
    e.Y[P + 0x2A] = 127 << 16
    run_n(e, 30)
    l_pre = L48(e, 0xCF)
    print("pre-step L_CF =", l_pre)
    e.Y[P + 0x1B] = q(96)
    traj = [l_pre]
    for i in range(20):
        e.Y[P + 0x28] = 0
        run_frame(e)
        traj.append(L48(e, 0xCF))
    print("glide:", traj)
    # fit one-pole: L[k+1] = L[k] + a*(T - L[k])
    import math
    if traj[1] != traj[0]:
        # estimate alpha from consecutive ratios assuming final target T
        T = traj[-1]
        alphas = []
        for k in range(1, len(traj) - 2):
            d0 = T - traj[k]
            d1 = T - traj[k + 1]
            if d0:
                alphas.append(1.0 - d1 / d0)
        if alphas:
            print("alpha estimates:", ["%.4f" % a for a in alphas[:10]])


def probe_efx_armed():
    print("=== D. DSND/DFB/DBAS/DWID with delay armed ===")
    for name, key, vals in (("DSND", "dsnd", (0, 32, 64, 127)),
                            ("DFB", "dfb", (0, 32, 64, 127)),
                            ("DBAS", "dbas", (0, 32, 64, 127)),
                            ("DWID", "dwid", (0, 32, 64, 127))):
        for v in vals:
            e = build(tempo=120, lpq=127)
            kw = dict(dtim=64, dsnd=64, dfb=64, dbas=32, dwid=64)
            kw[key] = v
            set_efx(e, **kw)
            e.Y[P + 0x2A] = 127 << 16
            run_n(e, 30)
            o = read_out(e)
            taps = [(e.X[0x90 + i], e.Y[0x90 + i]) for i in range(17)]
            print("%s=%3d L_CF=%011d out_pk=%9d taps0=(%06X,%06X) taps8=(%06X,%06X) taps16=(%06X,%06X)" % (
                name, v, L48(e, 0xCF), max(abs(x) for x in o),
                taps[0][0], taps[0][1], taps[8][0], taps[8][1],
                taps[16][0], taps[16][1]))


if __name__ == "__main__":
    probe_arm()
    probe_dtim_map()
    probe_glide()
    probe_efx_armed()
