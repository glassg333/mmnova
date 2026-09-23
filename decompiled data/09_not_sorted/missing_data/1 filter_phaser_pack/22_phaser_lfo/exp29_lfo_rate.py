#!/usr/bin/env python3
"""exp29_lfo_rate.py — FINAL measurement of the m18 (FX-PHASER) LFO rate and
   amplitude behavior, on the bit-exact model (exp25_model.py, OK=38400 BAD=0).

   Why: iteration 22/24 numbers («576.55 Hz at SPD=16», f ≈ 576.55·(SPD/16)^2)
   were measured on the SINGLE-PASS emulator (DO-loop bug, fixed in iter 25)
   and are stale. This probe measures the LFO directly:

   1. Isolated oscillator sim — step 2 of proc_model (self-contained: the
      phasor state x:(r6+$11/$12) is touched by nothing else), so the
      trajectory is identical inside the full model. Verified by a
      full-model cross-check (part 3).
   2. Rate: unwrapped phase of the (a,b) phasor per block -> period in
      blocks -> Hz. Fit against theory theta = step*2^-23 rad/sample,
      f = theta*fs/(2*pi), step = a1(mul(a1(mul(SPD<<16,SPD<<16)), $9566)).
   3. Amplitude: the recurrence is an EXPLICIT Euler rotation
      (a' = a + theta*b ; b' = b - theta*a, det = 1+theta^2 > 1), so the
      amplitude GROWS: |z_k| = |z_0|*(1+theta^2)^(k/2). Measured slope +
      time-to-fullscale (wrap point) per SPD.
   4. Depth path: mod deviation from the static coefficient c is
      |VR| * |phasor| (rotation preserves the norm); we record the actual
      V-ramp magnitude from the model (dbg VR) and the block index where
      the modulation crosses 0.01 / 0.1 (audibility thresholds).

   Output: exp29_lfo_rate.json
"""
import json
import math
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "..", "18_m18_final"))
from exp25_model import M18State, proc_model, mul, muln, acc_of, a1, s24, M24

FS = 44100.0
SAMP_PER_BLOCK = 16
BLOCKS_PER_SEC = FS / SAMP_PER_BLOCK


def osc_step(step, a, b):
    """One 16-sample oscillator block, instruction-exact copy of
    exp25_model.proc_model step 2 ($145057-$145069), INCLUDING the
    block-boundary behavior of the real DSP: the phasor state lives in
    24-bit X-memory words (x:(r6+$11/$12)), so at the end of the block
    the 56-bit accs are truncated to 24 bits (a1) and re-extended next
    block (acc_of); x1 restarts from the STORED a (st.phase_hi)."""
    x1 = a1(a)                        # x1 = st.phase_hi (block init)
    for _ in range(16):
        y0 = (b >> 24) & M24          # b,y0 latch (pre-ALU b)
        a_k = (a >> 24) & M24         # a,x1 latch — pre-ALU a (delayed)
        b = b + muln(x1, step)        # mac -x1,x0,b   (x1 = a_{k-2}!)
        a = a + mul(y0, step)         # mac y0,x0,a   (y0 = b_{k-1})
        x1 = a_k
    return acc_of(a1(a)), acc_of(a1(b))   # 24-bit store + re-extend


def step_of_spd(spd):
    """step = a1(mul(spd2, $9566)), spd2 = a1(mul(prm,prm)), prm = SPD<<16."""
    prm = (spd & 0xFFFF) << 16
    spd2 = a1(mul(prm, prm))
    return a1(mul(spd2, 0x9566)), spd2


def run_isolated(spd, max_blocks, seed=0x80):
    """Run the oscillator alone; return per-block (re, im) in 1.23 units.
    seed = initial a-component (24-bit). The recurrence is LINEAR (rotation
    + dilation), so rate/growth constants do not depend on the seed; we use
    a large seed (0x400000 = 0.5) to keep quantization noise negligible at
    low SPD. The REAL DSP always starts at seed 0x80 (=128 LSB, from
    `move #$80,x0` in INIT) — recorded separately as amp_start_dsp."""
    step, _ = step_of_spd(spd)
    a = acc_of(seed)
    b = 0
    traj = []
    for _ in range(max_blocks):
        a, b = osc_step(step, a, b)
        re = s24(a1(a)) / float(1 << 23)   # a-component (cos side)
        im = s24(a1(b)) / float(1 << 23)   # b-component (sin side)
        traj.append((re, im))
    return traj, step


def analyze(traj, step):
    """Unwrapped phase -> period; log-amplitude slope -> growth."""
    phase = [math.atan2(im, re) for re, im in traj]
    amp = [math.hypot(re, im) for re, im in traj]
    # unwrap
    uw = [phase[0]]
    for k in range(1, len(phase)):
        d = phase[k] - phase[k - 1]
        while d > math.pi:
            d -= 2 * math.pi
        while d < -math.pi:
            d += 2 * math.pi
        uw.append(uw[-1] + d)
    n = len(uw)
    # linear fit of the last 60% of the unwrapped phase -> rad/block
    k0 = n // 5
    xs = list(range(k0, n))
    ys = uw[k0:]
    mx = sum(xs) / len(xs)
    my = sum(ys) / len(ys)
    num = sum((x - mx) * (y - my) for x, y in zip(xs, ys))
    den = sum((x - mx) ** 2 for x in xs)
    rad_per_block = num / den
    period_blocks = 2 * math.pi / abs(rad_per_block) if rad_per_block else None
    f_hz = abs(rad_per_block) * BLOCKS_PER_SEC / (2 * math.pi)
    # growth: fit log(amp) over the same window (amp > 0 always: starts 1.5e-5)
    lg = [math.log(max(v, 1e-30)) for v in amp[k0:]]
    my2 = sum(lg) / len(lg)
    num2 = sum((x - mx) * (y - my2) for x, y in zip(xs, lg))
    growth_per_block = num2 / den  # ln(amp) slope, per block
    theta = step / float(1 << 23)
    theory_f = theta * SAMP_PER_BLOCK * BLOCKS_PER_SEC / (2 * math.pi)
    # 3rd-order recurrence (x1 latch = 2-step delay): roots
    # lambda = 1 +/- i*theta + theta^2/2 -> |lambda| = sqrt(1+2*theta^2)
    theory_growth = 0.5 * math.log(1 + 2 * theta * theta) * SAMP_PER_BLOCK
    return {
        "step": step,
        "theta_rad_per_sample": theta,
        "period_blocks": period_blocks,
        "period_ms": period_blocks / BLOCKS_PER_SEC * 1000.0,
        "f_hz": f_hz,
        "f_hz_theory": theory_f,
        "rad_per_block": rad_per_block,
        "growth_per_block": growth_per_block,
        "growth_theory_per_block": theory_growth,
        "amp_start": amp[0],
        "amp_end": amp[-1],
    }


def blocks_until_amp(amp0, growth_per_block, target):
    """Blocks for the amplitude to reach `target` from amp0 (exponential)."""
    if growth_per_block <= 0 or amp0 <= 0:
        return None
    r = math.log(target / amp0) / growth_per_block
    return r if r > 0 else 0.0


def full_model_crosscheck(spd, blocks, dep=127, cntr=64):
    """Run the FULL proc_model with zero input; verify the phasor trajectory
    equals the isolated one, and record the modulation depth trajectory."""
    # hypothesis tables (iter 19/25): full sine period, 8192 words,
    # cos = sin + 2048 words (quarter turn). Rate/growth do NOT depend on
    # the table content; tables only shape the WID stereo path.
    n = 8192
    table_s = [int(round(0x7FFFFF * math.sin(2 * math.pi * i / n))) & M24
               for i in range(n)]
    table_c = [table_s[(i + 2048) % n] for i in range(n)]

    prm = [(v & 0xFFFF) << 16 for v in
           (cntr, dep, spd, 0, 0, 64, 0, 100)]  # CNTR DEP SPD MIX FB WID X INP
    sig = [0] * 34

    st = M18State()
    iso_step, _ = step_of_spd(spd)
    ia = acc_of(st.phase_hi)
    ib = acc_of(st.phase_lo)

    amp_traj = []
    vr_max_traj = []
    mod_dev_max = []
    match = True
    for _ in range(blocks):
        ia, ib = osc_step(iso_step, ia, ib)
        dbg = {}
        proc_model(st, prm, sig, table_s, table_c, dbg=dbg)
        if st.phase_hi != a1(ia) or st.phase_lo != a1(ib):
            match = False
        pa = s24(st.phase_hi) / float(1 << 23)
        pb = s24(st.phase_lo) / float(1 << 23)
        amp_traj.append(math.hypot(pa, pb))
        vr_max_traj.append(max(abs(s24(v)) / float(1 << 23) for v in dbg["VR"]))
        c = 2.0 * (s24((st.s18 >> 24) & M24) / float(1 << 23) - 0.5)
        dev = max(abs(s24(m) / float(1 << 23) - c) for m in dbg["mod"])
        mod_dev_max.append(dev)
    return {
        "phasor_matches_isolated": match,
        "amp_first": amp_traj[0],
        "amp_last": amp_traj[-1],
        "vr_abs_max": vr_max_traj[-1],
        "mod_dev_max_last": mod_dev_max[-1],
        "mod_dev_max_first": mod_dev_max[0],
    }


def main():
    out = {"fs": FS, "samp_per_block": SAMP_PER_BLOCK,
           "note": "Corrected (2-pass) LFO measurements on bit-exact "
                   "exp25_model; replaces stale 576.55 Hz iter-22/24 numbers",
           "spds": {}}

    # period needs ~5 cycles; growth window needs the same run.
    # seed 0x400000 (0.5): quantization noise ~1.2e-7 relative — clean fits
    # at every SPD. Real DSP start = 0x80 (128 LSB) -> amp_start_dsp.
    for spd in (8, 16, 32, 64, 100, 127):
        theta = step_of_spd(spd)[0] / float(1 << 23)
        rad_per_block = theta * SAMP_PER_BLOCK
        g_th = 0.5 * math.log(1 + 2 * theta * theta) * SAMP_PER_BLOCK
        # cap the run BEFORE the amplitude (0.5 seed) reaches ~0.9:
        # at amp >= 1.0 the 24-bit store wraps and the trajectory breaks
        cap = int(0.8 * math.log(0.9 / 0.5) / g_th) if g_th > 0 else 400000
        need = int(min(400000, max(2000, 2 * math.pi / abs(rad_per_block) * 6),
                       max(2000, cap)))
        traj, step = run_isolated(spd, need, seed=0x400000)
        res = analyze(traj, step)
        res["blocks_simulated"] = need
        res["amp_start_dsp"] = 128.0 / float(1 << 23)  # seed $80
        # time for the amplitude to reach 1.0 (a1() 24-bit wrap point),
        # from the REAL DSP start amplitude of 128 LSB
        res["blocks_to_fullscale"] = blocks_until_amp(
            res["amp_start_dsp"], res["growth_per_block"], 1.0)
        if res["blocks_to_fullscale"]:
            res["seconds_to_fullscale"] = (
                res["blocks_to_fullscale"] / BLOCKS_PER_SEC)
        res["blocks_to_dev_0.01"] = blocks_until_amp(
            res["amp_start_dsp"], res["growth_per_block"], 0.02)   # |VR|~0.5
        res["blocks_to_dev_0.1"] = blocks_until_amp(
            res["amp_start_dsp"], res["growth_per_block"], 0.2)
        out["spds"][str(spd)] = res

    # rate law check: f(2s)/f(s) should be ~4 (f ∝ SPD^2)
    r = out["spds"]
    out["rate_law_ratio_16_to_8"] = r["16"]["f_hz"] / r["8"]["f_hz"]
    out["rate_law_ratio_64_to_16"] = r["64"]["f_hz"] / r["16"]["f_hz"]
    out["rate_law_ratio_127_to_100"] = r["127"]["f_hz"] / r["100"]["f_hz"]
    # k = f / (SPD/16)^2 for reference
    out["k_hz_at_spd16_equiv"] = {
        s: r[s]["f_hz"] / (int(s) / 16.0) ** 2 for s in r}

    # full-model cross-check at SPD=127
    out["full_model_check"] = full_model_crosscheck(127, 300)

    p = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                     "exp29_lfo_rate.json")
    with open(p, "w") as f:
        json.dump(out, f, indent=1)

    # console summary
    print("SPD  step    f_Hz(meas)   f_Hz(theory)  period_blocks  "
          "growth/block  s_to_fullscale")
    for s in ("8", "16", "32", "64", "100", "127"):
        d = out["spds"][s]
        st = d.get("seconds_to_fullscale")
        st = f"{st:9.1f}" if st else "     None"
        print(f"{s:>3}  {d['step']:>6}  {d['f_hz']:10.5f}  "
              f"{d['f_hz_theory']:12.5f}  {d['period_blocks']:13.1f}  "
              f"{d['growth_per_block']:12.3e}  {st}")
    print("rate law f(2s)/f(s): 16/8 =",
          round(out["rate_law_ratio_16_to_8"], 4),
          "; 64/16 =", round(out["rate_law_ratio_64_to_16"], 4),
          "; 127/100 =", round(out["rate_law_ratio_127_to_100"], 4))
    print("full model check:", out["full_model_check"])


if __name__ == "__main__":
    main()
