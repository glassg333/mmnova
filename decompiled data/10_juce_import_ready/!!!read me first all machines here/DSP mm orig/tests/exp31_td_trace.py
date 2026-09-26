#!/usr/bin/env python3
"""
exp31_td_trace.py — Iteration 31: Track Delay mining, step 1.
Run the post-voice chain $04A8-$0B4C in the DSP56300 emulator and:
 1. dump execution PC coverage per region (which of $0939/$0985/$0A5B run, in what order);
 2. find the delay ring buffer address (X/Y writes outside voice page);
 3. see which r6-offsets the delay stage actually reads (params Y:$051A-$051F).
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, Q23

def q23(x):
    v = int(round(x * Q23))
    return max(-0x800000, min(0x7FFFFF, v)) & 0xFFFFFF

e = build_emu()

# --- instrument: record PC hits in the tail region ---
e.trace = False
pc_hits = {}
old_step = e.step
def step_instr():
    pc = e.pc
    pc_hits[pc] = pc_hits.get(pc, 0) + 1
    old_step()
e.step = step_instr

# machine output: an impulse train so delay taps are visible (34 words at r6+$DC)
inp = [0] * 34
inp[0] = 0x400000   # big positive L
inp[1] = 0x200000   # big positive R (alternating L/R halves?)
for i in range(2, 34):
    inp[i] = 0x100000 + i * 0x2000

setup_voice(e, r6=0x400, trig=1)
Y = e.Y
X = e.X
# Track Delay + SRR params (page base r6=$400 corresponds to Y:$0500 layout):
# r6+$1A = SRR, $1B = TIM, $1C = SND, $1D = FDB, $1E = BAS, $1F = WID
Y[0x400 + 0x1A] = q23(0.0)    # SRR off first
Y[0x400 + 0x1B] = q23(0.3)    # DEL TIM
Y[0x400 + 0x1C] = q23(0.5)    # DEL SND
Y[0x400 + 0x1D] = q23(0.4)    # DEL FDB
Y[0x400 + 0x1E] = q23(0.5)    # DEL BAS
Y[0x400 + 0x1F] = q23(0.5)    # DEL WID
# zero scratch so delay cells are visible
for a in range(0x00, 0x200):
    X[a] = 0
    Y[a] = 0
# fill runtime delay buffer region (X/Y $114000+) with recognizable pattern
for i in range(0x4000):
    X[0x114000 + i] = 0
    Y[0x114000 + i] = 0

snapX = dict(e.X); snapY = dict(e.Y)

n = e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
print("block steps:", n)

# --- PC coverage summary by 256-word page ---
pages = {}
for pc, c in sorted(pc_hits.items()):
    pages.setdefault(pc & ~0xFF, []).append((pc, c))
print("\nPC pages executed:")
for pg in sorted(pages):
    lo = min(p for p, _ in pages[pg]); hi = max(p for p, _ in pages[pg])
    cnt = sum(pages[pg][i][1] for i in range(len(pages[pg])))
    print(f"  P:{pg:04X}-{hi:04X}  ({len(pages[pg])} pcs, {cnt} hits)")

# --- memory writes outside voice page and outside tables ---
print("\nX writes (0x000-0x2FF):")
dx = {a: e.X[a] for a in set(e.X) if e.X.get(a, 0) != snapX.get(a, 0) and a < 0x300}
for a in sorted(dx):
    print(f"  X:{a:04X} = {dx[a]:06X}")
print("Y writes (0x000-0x2FF):")
dy = {a: e.Y[a] for a in set(e.Y) if e.Y.get(a, 0) != snapY.get(a, 0) and a < 0x300}
for a in sorted(dy):
    print(f"  Y:{a:04X} = {dy[a]:06X}")
print("writes >= 0x114000 (delay SRAM):")
for mem, snap, cur in (("X", snapX, e.X), ("Y", snapY, e.Y)):
    d = {a: cur[a] for a in set(cur) if cur.get(a, 0) != snap.get(a, 0) and a >= 0x114000}
    if d:
        lo = min(d); hi = max(d)
        print(f"  {mem}: {len(d)} cells, range {lo:06X}..{hi:06X}")
        for a in sorted(d)[:16]:
            print(f"    {mem}:{a:06X} = {d[a]:06X}")
