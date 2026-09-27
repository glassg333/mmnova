#!/usr/bin/env python3
"""
exp41_fm_trace.py — full instruction trace of one FM+STAT process block.
Ground truth for writing the C++ model. Logs pc, A, B, x0,x1,y0,y1 per step.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from exp40_fm_oracle import *

e = build()
init_page(e)
KNOBS = [15, 64, 100, 80, 15, 100, 64, 64]   # 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE
set_knobs(e, KNOBS)
e.R[6] = PAGE
e.R[7] = OUTP
e.A = 11776

trace = []
def hook(pc, emu):
    trace.append((pc, emu.A, emu.B, emu.x0, emu.x1, emu.y0, emu.y1,
                  emu.R[0], emu.R[1], emu.R[2], emu.R[3], emu.R[4], emu.R[5], emu.R[7]))

e.ret_stack.append(SENTINEL)
e.run(ENTRY, end=SENTINEL, hook=hook)

print(f"trace len {len(trace)}")
out = read_out(e)
print("output L/R:", [hex(v) for v in out])
# states
for off in [0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x1a,0x1b,0x1c,0x1d,0x1e,
            0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a]:
    print(f"  y:$(r6+{off:02X}) = {e.Y[PAGE+off]:06X}")

# save trace
with open("/home/z/my-project/work/fm_fix/stat_trace.txt", "w") as f:
    lines = open(LST).read().splitlines()
    txt = {}
    for ln in lines:
        head = ln.split(":")[0].strip()
        if not head:
            continue
        try:
            addr = int(head, 16)
        except ValueError:
            continue
        txt[addr] = ln.strip()
    for (pc, a, b, x0, x1, y0, y1, r0, r1, r2, r3, r4, r5, r7) in trace:
        src = txt.get(pc, f"{pc:06x}: ???")
        f.write(f"{src} | A={a:014X} B={b:014X} x0={x0:06X} x1={x1:06X} y0={y0:06X} y1={y1:06X} "
                f"r0={r0:04X} r1={r1:04X} r2={r2:04X} r5={r5:04X}\n")
print("trace saved to work/fm_fix/stat_trace.txt")
