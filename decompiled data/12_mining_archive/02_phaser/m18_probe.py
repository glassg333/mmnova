#!/usr/bin/env python3
"""exp23_m18_probe.py — probe the real phaser m18 ($145045) in the track harness:
entry registers, write map, param reads, ladder banks."""
import sys, math

sys.path.insert(0, "/home/z/my-project/scripts")
import track_harness as th
from dsp_emu import DSP56300  # noqa

M24 = 0xFFFFFF
M56 = (1 << 56) - 1

th.PROC[18] = 0x145045
th.PROC_INIT[18] = 0x145036
e = th.build_track_emu(machine=18)
# the kernel's frame-timer reads use movep (peripheral $FFEB) — stub them out:
for addr, (mnem, toks, text) in list(e.prog.items()):
    if mnem == "movep":
        e.prog[addr] = ("nop", [], text + "  ; STUBBED movep")
th.setup_track(e, base=64, trig=1, mach_params=[64, 40, 30, 50, 60, 70, 80, 90])

# patch mach_params into the machine param area — find where params live:
# the harness wrote them to Y[P+$2C..$33] (P = 0x400). The FX machine r6 = ?
# First run one frame and log the PROC entry state.
e.watch = set(range(0, 0x200)) | set(range(0x400, 0x500)) | set(range(0x100000, 0x115000))
e.watch_log = []

phase = 0.0
frames = []
for k in range(3):
    e.Y[0x400 + 0x28] = 1 if k == 0 else 0
    e.run(0x0100, end=0x02EB, max_steps=500000)
    e.X[0x2C9] = 0x300
    for i in range(0x22):
        e.Y[0x100 + i] = int(round(0.9 * 8388607 * math.sin(phase))) & 0xFFFFFF
        phase += 2 * math.pi / 32.0
    for i in range(16):
        e.X[i] = int(round(0.9 * 8388607 * math.sin(phase))) & 0xFFFFFF
        e.Y[i] = e.X[i]
        phase += 2 * math.pi / 32.0
    if k < 2:
        e.run(0x02EC, end=0x0B4C, max_steps=500000)

# now frame 3: stop right at the m18 PROC entry to read registers
e.run(0x0100, end=0x02EB, max_steps=500000)
for i in range(0x22):
    e.Y[0x100 + i] = int(round(0.9 * 8388607 * math.sin(phase))) & 0xFFFFFF
    phase += 2 * math.pi / 32.0
for i in range(16):
    e.X[i] = int(round(0.9 * 8388607 * math.sin(phase))) & 0xFFFFFF
    e.Y[i] = e.X[i]
    phase += 2 * math.pi / 32.0
# run until pc == PROC (the jsr (r1) dispatch lands there)
e.run(0x02EC, end=0x145045, max_steps=500000)
print("at m18 PROC entry: pc=%06X" % e.pc)
print("  r0=%06X r1=%06X r2=%06X r3=%06X r4=%06X r5=%06X r6=%06X r7=%06X"
      % tuple(e.R))
print("  m0=%06X m1=%06X m3=%06X m4=%06X m5=%06X m6=%06X"
      % (e.M[0], e.M[1], e.M[3], e.M[4], e.M[5], e.M[6]))
R6 = e.R[6]
print("  machine page r6 = %04X" % R6)
print("  params y:(r6+$4..$B):",
      ["%06X" % e.Y.get(R6 + i, 0) for i in range(4, 12)])
print("  y:(r6+$0..$3):", ["%06X" % e.Y.get(R6 + i, 0) for i in range(4)])

e.watch_log = []
e.watch = set(range(0, 0x80)) | set(range(0xE0, 0x100)) | \
          set(range(R6 - 0x20, R6 + 0x60))
e.run(0x145045, end=0x14538D, max_steps=200000)
print("\nafter PROC: pc=%06X" % e.pc)
print("writes summary (addr <- val), first 80:")
seen = []
for (pc, sp, ad, val) in e.watch_log:
    seen.append((pc, sp, ad, val))
for w in seen[:80]:
    print("  pc=%06X %s:%04X <- %06X" % w)
print("total writes:", len(seen))
print("\nY:$00-$1F:", ["%06X" % e.Y.get(a, 0) for a in range(0x20)])
print("X:$20-$3F:", ["%06X" % e.X.get(a, 0) for a in range(0x20, 0x40)])
print("Y:$20-$3F:", ["%06X" % e.Y.get(a, 0) for a in range(0x20, 0x40)])
print("X:$E0-$EF:", ["%06X" % e.X.get(a, 0) for a in range(0xE0, 0xF0)])
print("Y:$E0-$EF:", ["%06X" % e.Y.get(a, 0) for a in range(0xE0, 0xF0)])
print("page r6+$11/$12 (phase): %06X %06X" % (e.X.get(R6 + 0x11, 0), e.X.get(R6 + 0x12, 0)))
print("page r6+$18/$19 (L-pairs): %06X/%06X %06X/%06X"
      % (e.X.get(R6 + 0x18, 0), e.Y.get(R6 + 0x18, 0),
         e.X.get(R6 + 0x19, 0), e.Y.get(R6 + 0x19, 0)))
print("page r6+$1A-$1F:", ["%06X" % e.Y.get(R6 + a, 0) for a in range(0x1A, 0x20)])
