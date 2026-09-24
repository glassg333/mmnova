#!/usr/bin/env python3
"""fm_measure2.py — FM-STAT (m8) knob laws, measured on the bit-precise emulator.

Measures (all relative, independent of the absolute-pitch question):
  1. 2FRQ knob -> ratio-table index boundaries (mod-2/carrier phase ratio)
  2. 1FIN knob -> ratio-1 multiplier
  3. 1ENV knob -> op1 level law (mix magnitude, 2VOL=0)
  4. 2VOL knob -> gate/level/peak-shaper law
  5. 1FB  knob -> feedback depth (mod-1 phase self-modulation)
  6. TONE knob -> one-pole coefficient index into $144AC7
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_block, R6M, M24

PM = "/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin"
data = open(PM, "rb").read()
def tbl(a):
    o = a * 3
    return (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]

def frac24(v):
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0

def run_machine(p, A, blocks=8):
    e = build(); fm_init(e, 8)
    for _ in range(2):
        fm_block(e, 8, p, A)
    outs = []
    for _ in range(blocks):
        outs.append(fm_block(e, 8, p, A))
    return e, outs

def mix_magnitude(e):
    """max |X:$80-i| over the last block's mixed operator diffs"""
    return max(abs(frac24(e.X[0x80 + i])) for i in range(32))

def phase_ratio_mod2carrier(e):
    """dphase(mod2 cell r6+$12) vs dphase(carrier L:$5 x32) over one block"""
    m2 = (e.Y[R6M + 0x12] & 0xFFFF)  # int part grows unbounded-ish; use delta over blocks
    return m2

results = {}

# ---------------- 1. 2FRQ -> ratio index (mod-2/carrier frequency ratio) --------
print("=== 2FRQ sweep: mod-2 frequency ratio to carrier ===")
rows = []
A = 2000
prev = None
for knob in range(0, 128, 4):
    p = [64, 64, 0, 0, knob, 0, 127, 64]
    e, outs = run_machine(p, A)
    # mod-2 phase = L:$20-$3F accumulated unbounded; read last two blocks' delta
    # simpler: read r6+$12 (mod2 int phase, masked+base each block) delta
    rows.append((knob, e.Y[R6M + 0x12] & 0x1FFF))
print("knob -> mod2 phase offset (rel. base):", ["%d:%d" % r for r in rows[:12]])
# boundaries: compute expected index n = ((knob<<16 + $8000)*24)>>24
bounds = []
for knob in range(128):
    n = (((knob << 16) + 0x8000) * 24) >> 24
    bounds.append(n)
edges = [k for k in range(1, 128) if bounds[k] != bounds[k - 1]]
results["ratio_index_edges"] = edges
results["ratio_index_of_knob"] = bounds
print("index edges at knobs:", edges)
print("ratio table raw:", ["%06X" % tbl(0x141A80 + i) for i in range(24)])

# ---------------- 2. 1FIN -> ratio-1 multiplier --------------------------------
print()
print("=== 1FIN sweep (1FRQ=64 -> nominal ratio idx=6; measure mod-1 phase delta) ===")
rows = []
for fin in (0, 16, 32, 64, 96, 112, 127):
    p = [64, fin, 127, 0, 64, 0, 127, 64]
    e, outs = run_machine(p, A)
    ph = []
    for _ in range(3):
        fm_block(e, 8, p, A)
        ph.append(e.Y[R6M + 0x14] & 0x1FFF)
    d = (ph[2] - ph[0]) % 8192
    mult = (0.5 + (fin - 64) / 256.0 / 1.0) if False else 1 + (fin - 64) / 256.0
    rows.append((fin, d, d / 3.0 / 32.0, mult))
    print("1FIN=%3d  dphase/blk=%5.1f  inc/iter=%7.3f  expected_mult=%.4f" % (fin, d, d / 96.0, mult))
results["fin_sweep"] = rows

# ---------------- 3. 1ENV -> op1 level -----------------------------------------
print()
print("=== 1ENV sweep: op1 mix magnitude (2VOL=0, 1FB=0, TONE=127) ===")
rows = []
for env in (0, 16, 32, 48, 64, 80, 96, 112, 127):
    p = [64, 64, env, 0, 64, 0, 127, 64]
    e, outs = run_machine(p, A)
    mg = mix_magnitude(e)
    pred = (env / 128.0) ** 2
    droop = 1 - 2 * ((max(0, env - 64) / 128.0) ** 4) if env >= 64 else 1.0
    rows.append((env, mg, pred * droop))
    print("1ENV=%3d  mix_max=%8.5f  predicted (k/128)^2*droop=%8.5f" % (env, mg, pred * droop))
results["env_sweep"] = rows

# ---------------- 4. 2VOL -> gate/level/shaper ---------------------------------
print()
print("=== 2VOL sweep: op2 contribution (1ENV=0 -> op1 silent) ===")
rows = []
for vol in (0, 32, 63, 64, 65, 80, 96, 112, 127):
    p = [64, 64, 0, 0, 64, vol, 127, 64]
    e, outs = run_machine(p, A)
    mg = mix_magnitude(e)
    rows.append((vol, mg))
    print("2VOL=%3d  mix_max=%8.5f  %s" % (vol, mg, "GATE" if vol < 64 else ""))
results["vol_sweep"] = rows

# ---------------- 5. 1FB -> feedback -------------------------------------------
print()
print("=== 1FB sweep: mod-1 self-modulation (observed via mix spectrum change) ===")
rows = []
for fb in (0, 32, 64, 96, 127):
    p = [64, 64, 127, fb, 64, 0, 127, 64]
    e, outs = run_machine(p, A)
    # feedback accum cell r6+$1C (a1) = last sin*gain product
    acc = e.Y[R6M + 0x1C]
    mg = mix_magnitude(e)
    rows.append((fb, frac24(acc), mg))
    print("1FB=%3d  fb_accum=%8.5f (pred gain=%.5f)  mix_max=%8.5f" % (
        fb, frac24(acc), fb * 32 / 8388608.0, mg))
results["fb_sweep"] = rows

# ---------------- 6. TONE -> LP coefficient ------------------------------------
print()
print("=== TONE sweep: op-diff lowpass (mix magnitude vs TONE) ===")
rows = []
for tone in (0, 32, 64, 96, 127):
    p = [64, 64, 127, 0, 64, 0, tone, 64]
    e, outs = run_machine(p, A)
    mg = mix_magnitude(e)
    coeff = frac24(tbl(0x144AC7 + tone))
    rows.append((tone, mg, coeff))
    print("TONE=%3d  mix_max=%8.5f  $144AC7[tone]=%8.5f" % (tone, mg, coeff))
results["tone_sweep"] = rows

json.dump(results, open("/home/z/my-project/scripts/fm_laws_stat.json", "w"), indent=1)
print()
print("saved fm_laws_stat.json")
