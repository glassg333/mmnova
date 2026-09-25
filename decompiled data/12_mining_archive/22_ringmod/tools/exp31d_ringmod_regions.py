#!/usr/bin/env python3
"""exp31d — count executions of key PROC regions of machine 17."""
import sys
from collections import Counter
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, set_params, R6, PROC, PROC_END

e = build()
run_init(e)
set_params(e, [0, 0, 60, 127, 0, 0, 0, 64])
for i in range(34):
    e.X[0x200 + i] = 0x400000
e.R[6] = R6
e.R[7] = 0x100
e.R[0] = 17
e.A = 0x123456
e.ret_stack.append(PROC_END)
e.trace = True
e.run(PROC, end=PROC_END, max_steps=2000000)
e.trace = False

cnt = Counter()
for line in e.trace_log:
    pc = int(line.split(":")[0], 16)
    cnt[pc] += 1

regions = {
    "entry save A": range(0x1478C7, 0x1478CD),
    "recip A chain": range(0x1478CD, 0x1478E1),
    "step calc": range(0x1478E1, 0x147907),
    "carrier osc loop": range(0x147918, 0x14791E),
    "carrier interp loop": range(0x14792A, 0x147936),
    "smoothing $5F1BE3": range(0x14793B, 0x14795E),
    "func_0003d3 call": range(0x147966, 0x14796F),
    "page zero $88": range(0x147977, 0x147996),
    "mix ramps": range(0x14799C, 0x1479E0),
    "wavetable read": range(0x1479E0, 0x1479EA),
    "lattice loop": range(0x1479F3, 0x1479FA),
    "WAVE/EXT gains": range(0x147A04, 0x147A15),
    "INP banks loop": range(0x147A34, 0x147A3D),
    "func_147ab7 #1": range(0x147A4B, 0x147A4D),
    "func_147ad4 #1": range(0x147A53, 0x147A55),
    "MIX loop $147A68": range(0x147A68, 0x147A6F),
    "one-pole banks": range(0x147A87, 0x147A90),
    "func_147ab7 #2": range(0x147A97, 0x147A99),
    "func_147ad4 #2": range(0x147A9F, 0x147AA1),
    "output loop": range(0x147AB0, 0x147AB6),
    "alt tail $147B27": range(0x147B27, 0x147B38),
}
for name, rng in regions.items():
    n = sum(cnt.get(a, 0) for a in rng)
    print("%-20s %d" % (name, n))
print("total steps:", sum(cnt.values()))
