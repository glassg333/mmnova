#!/usr/bin/env python3
"""exp45_checkpoint_dump.py — dump emulator buffers at every section boundary
of one FM+STAT block (defaults, pitch 11776) for C++ mirror diffing."""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from exp40_fm_oracle import *

CKPTS = [
    ("01_L20",    0x145D59, [("X", 0x20, 32), ("Y", 0x20, 32)]),
    ("02_Y80i",   0x145D70, [("Y", 0x80, 32)]),
    ("03_Y80d",   0x145D7B, [("Y", 0x80, 32), ("Y", PAGE + 0x1A, 1)]),
    ("04_Y80t",   0x145D8C, [("Y", 0x80, 32), ("Y", PAGE + 0x28, 1)]),
    ("05_Y80s",   0x145DA1, [("Y", 0x80, 32), ("Y", PAGE + 0x1E, 1)]),
    ("06_YC0",    0x145DD8, [("Y", 0xC0, 32), ("Y", PAGE + 0x14, 2), ("Y", PAGE + 0x1C, 2)]),
    ("07_YC0d",   0x145DE3, [("Y", 0xC0, 32), ("Y", PAGE + 0x1B, 1)]),
    ("08_YC0t",   0x145DF4, [("Y", 0xC0, 32), ("Y", PAGE + 0x29, 1)]),
    ("09_X80",    0x145E23, [("X", 0x80, 32), ("Y", PAGE + 0x2A, 1)]),
    ("10_Y80m",   0x145E34, [("Y", 0x80, 32), ("Y", PAGE + 0x27, 1)]),
    ("11_L20c",   0x145E56, [("X", 0x20, 32), ("Y", 0x20, 32)]),
    ("12_XE0",    0x145E71, [("X", 0xE0, 32)]),
    ("13_final",  0x0F0F0F, [("Y", 0x700, 32), ("Y", PAGE + 0x10, 0x1B)]),
]

e = build(); init_page(e)
set_knobs(e, [60,64,80,30,80,64,98,64])
e.R[6] = PAGE; e.R[7] = 0x700; e.A = 11776
e.ret_stack.append(0x0F0F0F)
lines = []
prev = 0x145D21
for name, end, bufs in CKPTS:
    if end != 0x0F0F0F:
        e.run(prev, end=end)
    else:
        e.run(prev, end=end)
    prev = end if end != 0x0F0F0F else prev
    parts = [name]
    for sp, base, n in bufs:
        mem = e.X if sp == "X" else e.Y
        parts.append(" ".join("%06X" % (mem[base+i] & 0xFFFFFF) for i in range(n)))
    lines.append(" | ".join(parts))
with open("/home/z/my-project/work/fm_fix/emu_ckpts.txt", "w") as f:
    f.write("\n".join(lines) + "\n")
print("\n".join(l[:120] for l in lines[:4]))
print("checkpoints dumped")
