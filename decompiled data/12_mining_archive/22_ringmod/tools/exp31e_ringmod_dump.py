#!/usr/bin/env python3
"""exp31e — dump intermediate banks of machine 17 after one block."""
import sys
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, run_block, f

e = build()
run_init(e)
out = run_block(e, [0x400000] * 34, [0, 0, 60, 127, 0, 0, 0, 64], a_entry=0x123456)

print("out32:", " ".join(str(v >> 8) for v in out))
print("\nX:$00-$0F (gains etc):")
print("  " + " ".join("%06X" % e.X.get(a, 0) for a in range(0x00, 0x10)))
print("\nY:$40-$D0 (banks $56/$63/$72/$84/$97/$AA):")
for lo in range(0x40, 0xD0, 8):
    row = " ".join("%08X" % (e.Y.get(a, 0) >> 4) for a in range(lo, lo + 8))
    print("  Y:%03X  %s" % (lo, row))
print("\nL states $F8-$FF (X/Y):")
for a in range(0xF8, 0x100):
    print("  L:%02X  X=%06X Y=%06X" % (a, e.X.get(a, 0), e.Y.get(a, 0)))
print("\npage r6 (X/Y nonzero, $520-$590):")
for off in range(-8, 0x60):
    x = e.X.get(R6 := 0x528 + off, 0)
    y = e.Y.get(0x528 + off, 0)
    if x or y:
        print("  %+#05x X=%06X Y=%06X" % (off, x, y))
