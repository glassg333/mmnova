#!/usr/bin/env python3
"""exp22_diag2.py — watch all writes in the tail blocks of one frame:
   $0AD1-$0B14 (DIV/integrate/dezip/func397), $0B14-$0B1E (copy),
   $0B1E-$0B33 (dwid), $0B33-$0B49 (mix)."""
import sys, math

PK = "/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack"
sys.path.insert(0, PK + "/05_emulator")
sys.path.insert(0, PK + "/06_amp_env")
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase   # noqa

M24 = 0xFFFFFF
M56 = (1 << 56) - 1

c = EnvCase(filt_atk=64, filt_dec=64, bofs=0, wofs=0)
e = c.e
for off, v in ((0x05, 127), (0x06, 64), (0x10, 64), (0x11, 64),
               (0x12, 64), (0x13, 64)):
    e.Y[0x400 + off] = (int(v) & 0xFFFF) << 16
e.Y[0x400 + 0x28] = 1
e.Y[0x400 + 0x1F] = 0            # dwid = 0
e.run(0x0100, end=0x02EB, max_steps=500000)
e.X[0x2C9] = 0x300
for i in range(0x22):
    e.Y[0x100 + i] = int(round(0.9 * 8388607 * math.sin(c.phase))) & 0xFFFFFF
    c.phase += 2 * math.pi / 32.0
for i in range(16):
    e.X[i] = int(round(0.9 * 8388607 * math.sin(c.phase))) & 0xFFFFFF
    e.Y[i] = e.X[i]
    c.phase += 2 * math.pi / 32.0
e.run(0x02EC, end=0x0AD1, max_steps=500000)
assert e.pc == 0x0AD1

def dump(tag):
    print("%s: pc=%04X r0=%03X r1=%03X r2=%03X r3=%03X r4=%03X r5=%03X r6=%03X "
          "m2=%06X m3=%06X m4=%06X m1=%06X m6=%06X A=%014X B=%014X"
          % (tag, e.pc, e.R[0], e.R[1], e.R[2], e.R[3], e.R[4], e.R[5],
             e.R[6], e.M[2], e.M[3], e.M[4], e.M[1], e.M[6],
             e.A & M56, e.B & M56))

dump("at 0AD1")
print("X:70-82:", ["%06X" % e.X.get(a, 0) for a in range(0x70, 0x83)])

# --- block 1: $0AD1..$0B14 (through the jsr; stop right after return)
e.watch = set(range(0x00, 0x100)) | set(range(0x4000, 0x4010)) | set(range(0x70, 0x83))
e.watch_log = []
e.run(0x0AD1, end=0x0B14, max_steps=400000)
dump("at 0B14 (post func397)")
print("watch writes in block1 (pc, sp, addr, val):")
for (pc, sp, ad, val) in e.watch_log:
    if sp == "Y" and ad < 0x40 or sp == "Y" and 0x4000 <= ad < 0x4010 or sp == "X" and 0x70 <= ad < 0x83:
        print("  pc=%04X %s:%04X <- %06X" % (pc, sp, ad, val))
print("X:70-82 after func397:", ["%06X" % e.X.get(a, 0) for a in range(0x70, 0x83)])
print("Y:00-0F:", ["%06X" % e.Y.get(a, 0) for a in range(0x10)])

# --- block 2: copy $0B14-$0B1E
e.watch_log = []
e.run(0x0B14, end=0x0B1E, max_steps=2000)
dump("at 0B1E (post copy)")
print("copy writes:")
for (pc, sp, ad, val) in e.watch_log:
    print("  pc=%04X %s:%04X <- %06X" % (pc, sp, ad, val))

# --- block 3: dwid $0B1E-$0B33
e.watch_log = []
e.run(0x0B1E, end=0x0B33, max_steps=4000)
dump("at 0B33 (post dwid)")

# --- block 4: mix $0B33-$0B49
e.watch_log = []
print("mix inputs: X:00-0F:", ["%06X" % e.X.get(a, 0) for a in range(0x10)])
print("            X:10-1F:", ["%06X" % e.X.get(a, 0) for a in range(0x10, 0x20)])
print("            Y:10-1F:", ["%06X" % e.Y.get(a, 0) for a in range(0x10, 0x20)])
print("            Y:20-3F:", ["%06X" % e.Y.get(a, 0) for a in range(0x20, 0x40)])
e.run(0x0B33, end=0x0B49, max_steps=4000)
dump("at 0B49 (post mix)")
print("mix writes (first 40):")
for (pc, sp, ad, val) in e.watch_log[:40]:
    print("  pc=%04X %s:%04X <- %06X" % (pc, sp, ad, val))
print("total mix writes:", len(e.watch_log))
print("Y:0000-001F:", ["%06X" % e.Y.get(a, 0) for a in range(0x20)])
print("X:0000-001F:", ["%06X" % e.X.get(a, 0) for a in range(0x20)])
print("X:0300-030F:", ["%06X" % e.X.get(a, 0) for a in range(0x300, 0x310)])
