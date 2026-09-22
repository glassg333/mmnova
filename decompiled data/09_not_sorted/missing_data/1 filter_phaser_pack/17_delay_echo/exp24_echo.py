#!/usr/bin/env python3
"""exp24_echo.py — probe the per-voice ECHO (EFFX page) on the bit-exact track harness.

Questions:
  1. DTIM (P+$1B): mapping knob -> delay length L:P+$CF, and the glide (retune speed)
     when DTIM steps (kernel block $09CB-$09EE, coeffs $CCCCD/$733333).
  2. DSND (P+$1C): where the send knob bites (echo ring / taps / output scale).
  3. DFB (P+$1D), DBAS (P+$1E): same.
  4. The EFX envelope ($04A8-$04F5): env Y:$4FF attack/decay/sustain/release rates
     indexed by $18/$19/$1A/$1B.
"""
import sys, json

PACK = "/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack/05_emulator"
sys.path.insert(0, PACK)
from track_harness import build_track_emu, setup_track, run_frame, read_out  # noqa

P = 0x400
M24 = 0xFFFFFF


def q(v):
    """knob 0..127 -> param cell (raw in high word)"""
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
    x = e.X[P + off]
    y = e.Y[P + off]
    v = (x << 24) | y
    if x & 0x800000:
        v -= 1 << 48
    return v


def snap(e):
    d = {}
    for off in (0x00, 0x21, 0x28, 0x29, 0x2A, 0x2B,
                0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3):
        d["Y%02X" % off] = e.Y[P + off]
        d["X%02X" % off] = e.X[P + off]
    d["L_CF"] = L48(e, 0xCF)
    d["L_CE"] = L48(e, 0xCE)
    d["L_CD"] = L48(e, 0xCD)
    d["Y4FF"] = e.Y[0x4FF]
    d["X2C9"] = e.X[0x2C9]
    d["XC5"] = e.X[0xC5]
    d["echo"] = [e.X.get(0x300 + i, 0) for i in range(8)]
    d["taps"] = [(e.X[0x90 + i], e.Y[0x90 + i]) for i in range(17)]
    d["out"] = read_out(e)
    return d


def build(e=1, **kw):
    em = build_track_emu(machine=1)
    setup_track(em, base=64, trig=1, tempo=120, **kw)
    return em


def f24(v):
    v &= M24
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0


def probe_dtim_steady():
    print("=== A. DTIM steady sweep (48 frames each) ===")
    res = {}
    for dtim in (0, 8, 16, 32, 64, 96, 127):
        e = build()
        set_efx(e, dtim=dtim, dwid=64, dsnd=64, dbas=32, dfb=0)
        for i in range(48):
            run_frame(e)
            e.Y[P + 0x28] = 0  # no retrig
        s = snap(e)
        res[dtim] = s
        print("DTIM=%3d  L_CF=%011d  L_CE=%011d  L_CD=%011d  Y2A=%06X Y29=%06X YD0=%06X  Y4FF=%06X  X2C9=%04X  out0=%s" % (
            dtim, s["L_CF"], s["L_CE"], s["L_CD"], s["Y2A"], s["Y29"], s["YD0"],
            s["Y4FF"], s["X2C9"], f24(s["out"][0])))
    return res


def probe_glide():
    print("=== B. DTIM step glide (retune speed) ===")
    e = build()
    set_efx(e, dtim=32, dwid=64, dsnd=64, dbas=32)
    traj = []
    for i in range(40):
        run_frame(e)
        e.Y[P + 0x28] = 0
        traj.append(("pre", i, L48(e, 0xCF), e.Y[P + 0x2A], e.Y[P + 0xD0]))
    # step DTIM 32 -> 96
    e.Y[P + 0x1B] = q(96)
    for i in range(30):
        run_frame(e)
        e.Y[P + 0x28] = 0
        traj.append(("post", i, L48(e, 0xCF), e.Y[P + 0x2A], e.Y[P + 0xD0]))
    for row in traj:
        print(row)
    return traj


def probe_send():
    print("=== C. DSND sweep (echo ring + output) ===")
    base = None
    for dsnd in (0, 32, 64, 127):
        e = build()
        set_efx(e, dtim=64, dwid=64, dsnd=dsnd, dbas=32, dfb=0)
        for i in range(48):
            run_frame(e)
            e.Y[P + 0x28] = 0
        s = snap(e)
        if base is None:
            base = s
        peak_out = max(abs(v) for v in s["out"])
        peak_echo = max(abs(v) for v in s["echo"]) if s["echo"] else 0
        print("DSND=%3d  out_peak=%9s  echo_peak=%9s  X2C9=%04X  L_CF=%011d  Y4FF=%06X" % (
            dsnd, peak_out, peak_echo, s["X2C9"], s["L_CF"], s["Y4FF"]))
    return


def probe_dfb_dbas():
    print("=== D. DFB / DBAS sweep ===")
    for name, key in (("DFB", "dfb"), ("DBAS", "dbas")):
        for v in (0, 32, 64, 127):
            e = build()
            kw = dict(dtim=64, dwid=64, dsnd=64)
            kw[key] = v
            set_efx(e, **kw)
            for i in range(48):
                run_frame(e)
                e.Y[P + 0x28] = 0
            s = snap(e)
            print("%s=%3d  L_CF=%011d  taps0=(%06X,%06X) taps16=(%06X,%06X)  X2C9=%04X  out0=%s" % (
                name, v, s["L_CF"], s["taps"][0][0], s["taps"][0][1],
                s["taps"][16][0], s["taps"][16][1], s["X2C9"], f24(s["out"][0])))


def probe_env():
    print("=== E. EFX envelope ($04A8): Y4FF traj for ATK/DEC/SUS/REL knobs ===")
    import collections
    cfgs = [
        ("all0",            dict(eqf=0, eqg=0, srr=0, dtim=0)),
        ("atk_hi",          dict(eqf=127, eqg=0, srr=0, dtim=0)),
        ("dec_hi",          dict(eqf=0, eqg=127, srr=0, dtim=0)),
        ("srr_hi",          dict(eqf=0, eqg=0, srr=127, dtim=0)),
        ("dtim_hi",         dict(eqf=0, eqg=0, srr=0, dtim=127)),
        ("mid",             dict(eqf=64, eqg=64, srr=64, dtim=64)),
    ]
    for name, kw in cfgs:
        e = build()
        set_efx(e, dwid=64, dsnd=64, **kw)
        traj = []
        for i in range(24):
            if i == 12:
                e.Y[P + 0x28] = 2  # note off -> release
            run_frame(e)
            traj.append(e.Y[0x4FF])
        print(name, " ".join("%06X" % v for v in traj))


if __name__ == "__main__":
    probe_dtim_steady()
    probe_glide()
    probe_send()
    probe_dfb_dbas()
    probe_env()
