#!/usr/bin/env python3
"""
measure_fr.py — Measure the Monomachine track filter frequency response via the
emulator. The steady-state response to a 16-sample-periodic impulse train gives
the frequency response sampled at k*2756.25 Hz (k = 0..15). Sweeps BASE, WDTH,
HPQ, LPQ. Also measures the envelope trajectory (open question 2).
"""
import sys, json, math, cmath
sys.path.insert(0, "/home/z/my-project/scripts")
from experiments import fresh, set_input, run_blocks, s2f, R6, SVF_IN

FS = 44100.0
BLOCK = 16

def freq_response(e, nblocks=6):
    """Steady-state periodic response -> complex spectrum of one period."""
    imp = [0x400000] + [0] * 15
    run_blocks(e, nblocks, sig=imp)
    last = run_blocks(e, 1, sig=imp)[0]
    prev = last
    # ensure steady state: run until two consecutive periods match
    for _ in range(6):
        cur = run_blocks(e, 1, sig=imp)[0]
        if cur == prev:
            break
        prev = cur
    spec = []
    for k in range(BLOCK):
        acc = 0j
        for n, v in enumerate(last):
            acc += s2f(v) * cmath.exp(-2j * math.pi * k * n / BLOCK)
        spec.append(acc)
    return spec, last

def db(x):
    return 20 * math.log10(abs(x) + 1e-12)

def sweep_base():
    print("=== frequency response vs BASE (WDTH=0, HPQ=LPQ=0) ===")
    rows = []
    for p in [0, 16, 32, 48, 64, 80, 96, 112, 127]:
        e = fresh(base=p / 127.0)
        spec, last = freq_response(e)
        mags = [db(v) for v in spec]
        # find the -3dB crossover relative to the low-frequency asymptote?? report curve
        row = {"param": p, "mags": mags}
        rows.append(row)
        print("BASE=%3d | " % p + " ".join("%5.1f" % m for m in mags[:8]))
    return rows

def sweep_wdth():
    print("=== frequency response vs WDTH (BASE=64) ===")
    rows = []
    for p in [0, 32, 64, 96, 127]:
        e = fresh(base=64 / 127.0, wdth=p / 127.0)
        spec, last = freq_response(e)
        mags = [db(v) for v in spec]
        rows.append({"param": p, "mags": mags})
        print("WDTH=%3d | " % p + " ".join("%5.1f" % m for m in mags[:8]))
    return rows

def sweep_res():
    print("=== frequency response vs LPQ (BASE=64) and HPQ ===")
    rows = []
    for p in [0, 32, 64, 96, 127]:
        e = fresh(base=64 / 127.0, lpq=p / 127.0)
        spec, last = freq_response(e)
        mags = [db(v) for v in spec]
        rows.append({"param": "lpq%d" % p, "mags": mags})
        print("LPQ =%3d | " % p + " ".join("%5.1f" % m for m in mags[:8]))
    for p in [0, 64, 127]:
        e = fresh(base=64 / 127.0, hpq=p / 127.0)
        spec, last = freq_response(e)
        mags = [db(v) for v in spec]
        rows.append({"param": "hpq%d" % p, "mags": mags})
        print("HPQ =%3d | " % p + " ".join("%5.1f" % m for m in mags[:8]))
    return rows

def sweep_mode():
    print("=== MODE bits sweep (BASE=64) ===")
    rows = []
    for mode in [0, 0x200, 0x800, 0xA00]:
        e = fresh(base=64 / 127.0, mode=mode)
        spec, last = freq_response(e)
        mags = [db(v) for v in spec]
        rows.append({"param": "mode%03X" % mode, "mags": mags})
        print("MODE=%03X | " % mode + " ".join("%5.1f" % m for m in mags[:8]))
    return rows

def env_traj():
    print("=== filter env: coefficient trajectory (depth/speed) ===")
    rows = []
    for depth in [0.0, 0.25, 0.5]:
        e = fresh(base=64 / 127.0, filt_atk=0.5 + depth, filt_dec=0.5)
        traj = []
        for k in range(48):
            e.R[6] = R6
            set_input(e, [0] * 16)
            e.run(0x04A8, end=0x05A1, max_steps=200000)
            # continue the rest of the block
            e.run(0x05A1, end=0x0B4C, max_steps=200000)
            traj.append(s2f(e.Y[4]))
        rows.append({"depth": depth, "traj": traj})
        print("depth=%.2f  Y04: " % depth + " ".join("%+07.4f" % v for v in traj[::4]))
    return rows

if __name__ == "__main__":
    which = sys.argv[1] if len(sys.argv) > 1 else "base"
    out = {"base": sweep_base, "wdth": sweep_wdth, "res": sweep_res,
           "mode": sweep_mode, "env": env_traj}[which]()
    json.dump(out, open("/home/z/my-project/mining/fr_%s.json" % which, "w"), indent=1)
