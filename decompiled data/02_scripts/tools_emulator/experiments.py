#!/usr/bin/env python3
"""
experiments.py — Bit-exact measurements of the Monomachine dual filter via emulation.
Experiments:
  A. coefficient sweep: BASE 0..127 -> Y:$04-07 coefficient cells
  B. impulse response of the full filter chain (BASE/WDTH/HPQ/LPQ fixed)
  C. resonance sweep: LPQ/HPQ -> ringing decay
  D. filter env trajectory: depth/speed -> cutoff modulation scale (open question 2)
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, run_block

R6 = 0x400
IN_BASE = R6 + 0xDC
NIN = 34

def snapshot(e):
    return (dict(e.X), dict(e.Y))

SVF_IN = 0x4E6   # r7-$18: the 16-sample audio input region (Y + X halves)

def set_input(e, sig):
    """sig: list of 16 samples -> SVF input region (machine output area)."""
    for i in range(NIN):
        e.Y[IN_BASE + i] = 0
        e.X[IN_BASE + i] = 0
    for i, v in enumerate(sig[:16]):
        e.Y[SVF_IN + i] = v & 0xFFFFFF
        e.X[SVF_IN + i] = v & 0xFFFFFF

def run_blocks(e, n, sig=None, collect=None):
    outs = []
    for k in range(n):
        e.R[6] = R6
        if sig is not None:
            set_input(e, sig)
        run_block(e)
        outs.append([e.Y[SVF_IN + i] for i in range(16)])
        if collect is not None:
            collect(k)
    return outs

def run_to_coeff(e):
    """Run env+coeff calc up to $059B (SVF coefficient read point)."""
    e.R[6] = R6
    e.run(0x04A8, end=0x05A1, max_steps=200000)

def fresh(base=64/127.0, wdth=0.0, hpq=0.0, lpq=0.0, mode=0, **kw):
    e = build_emu()
    setup_voice(e, r6=R6, base=base, wdth=wdth, hpq=hpq, lpq=lpq,
                mode=mode, trig=1, amp_atk=0, amp_dec=127, **kw)
    set_input(e, [0] * 16)
    run_block(e)   # init block (trig consumes, states initialize)
    # run a few blocks so the AMP env reaches sustain
    for k in range(6):
        e.R[6] = R6
        run_block(e)
    return e

def s2f(v):
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0

# ---------- A: coefficient sweep ----------
def exp_A():
    print("=== A: BASE sweep -> coefficient cells Y:$04..07 ===")
    rows = []
    for p in [0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 127]:
        e = fresh(base=p / 127.0)
        # re-run coeff calc only: set input zero
        set_input(e, [0] * 17)
        run_to_coeff(e)
        c4, c5, c6, c7 = [s2f(e.Y[i]) for i in (4, 5, 6, 7)]
        # also grab Y:$00-$03 (pan/gain?) and coeff cells written via r4=$4
        rows.append({"param": p, "Y04": c4, "Y05": c5, "Y06": c6, "Y07": c7})
        print("BASE=%3d  Y04=%+9.6f Y05=%+9.6f Y06=%+9.6f Y07=%+9.6f" % (p, c4, c5, c6, c7))
    return rows

# ---------- B: impulse response ----------
def exp_B(nblocks=48):
    print("=== B: impulse response (BASE=%.3f WDTH=%.3f HPQ=%.3f LPQ=%.3f) ===" % (64/127.0, 0.0, 0.0, 0.0))
    e = fresh(base=64/127.0)
    e.Y[R6 + 0x28] = 0   # no retrigger
    imp = [0x400000] + [0] * 15
    ir = run_blocks(e, nblocks, sig=imp)
    # continue with zero input
    ir += run_blocks(e, nblocks, sig=[0]*16)
    flat = []
    for blk in ir:
        flat.extend(blk[:16])
    print("first 32 outputs (Y:00..):", [round(s2f(v), 6) for v in flat[:32]])
    nz = sum(1 for v in flat if v != 0)
    print("nonzero samples:", nz, "of", len(flat))
    return flat

# ---------- C: resonance sweep ----------
def exp_C():
    print("=== C: LPQ/HPQ sweep -> peak ringing gain after 4 blocks ===")
    rows = []
    for q in [0, 16, 32, 48, 64, 80, 96, 112, 127]:
        e = fresh(base=64/127.0, lpq=q / 127.0)
        e.Y[R6 + 0x28] = 0
        set_input(e, [0x400000] + [0] * 16)
        for a in range(0x10):
            e.Y[a] = 0
        run_block(e)
        gains = []
        for blk in range(6):
            set_input(e, [0] * 17)
            for a in range(0x10):
                e.Y[a] = 0
            run_block(e)
            vals = [abs(s2f(v)) for v in e.Y[:0x10]]
            gains.append(max(vals) if vals else 0.0)
        rows.append({"lpq": q, "ring": gains})
        print("LPQ=%3d  |out| per block after imp: %s" % (q, ["%.6f" % g for g in gains]))
    return rows

# ---------- D: filter env trajectory (open question 2) ----------
def exp_D(depth=0.25, speed=0.75, n=64):
    print("=== D: filter env trajectory (depth=%.3f speed=%.3f) ===" % (depth, speed))
    e = fresh(base=64/127.0, filt_atk=0.5 + depth, filt_dec=speed)
    e.Y[R6 + 0x28] = 0
    set_input(e, [0] * 17)
    traj = []
    for k in range(n):
        run_block(e)
        traj.append({
            "blk": k,
            "Y04": s2f(e.Y[4]),
            "Y05": s2f(e.Y[5]),
            "env_acc": s2f(e.Y[R6 + 0xDB]),
            "env_ph": e.Y[R6 + 0xDA],
            "base_param": s2f(e.Y[R6 + 0x10]),
        })
    for t in traj[:40:2]:
        print("blk %3d  Y04=%+9.6f  env_acc=%+9.6f  env_ph=%06X" %
              (t["blk"], t["Y04"], t["env_acc"], t["env_ph"]))
    return traj

if __name__ == "__main__":
    which = sys.argv[1] if len(sys.argv) > 1 else "A"
    out = {}
    if which == "A": out["A"] = exp_A()
    elif which == "B": out["B"] = exp_B()
    elif which == "C": out["C"] = exp_C()
    elif which == "D": out["D"] = exp_D()
    json.dump(out, open("/home/z/my-project/mining/exp_%s.json" % which, "w"), indent=1)
