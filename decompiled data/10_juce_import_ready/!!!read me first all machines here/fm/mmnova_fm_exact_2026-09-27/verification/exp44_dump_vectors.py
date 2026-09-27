#!/usr/bin/env python3
"""
exp44_dump_vectors.py — produce FM+STAT test vectors from the emulator:
for each (knobs, pitch) vector: run init+config+4 process blocks, dump all
32*4 output words + final page states. The C++ mirror must match bit-exactly.
"""
import sys, json, random
sys.path.insert(0, "/home/z/my-project/scripts")
from exp40_fm_oracle import build, init_page, set_knobs, read_out, load_tables, PAGE

random.seed(40)
DEF = [60, 64, 80, 30, 80, 64, 98, 64]

vectors = []
def add(knobs, pitch, blocks=4):
    vectors.append({"knobs": list(knobs), "pitch": pitch, "blocks": blocks})

# defaults + each knob extreme
add(DEF, 11776)
for i in range(8):
    k = DEF[:]; k[i] = 0;    add(k, 11776)
    k = DEF[:]; k[i] = 127;  add(k, 11776)
# combos
add([15, 64, 100, 80, 15, 100, 64, 64], 11776)
add([127, 127, 127, 127, 127, 127, 127, 64], 23552)
add([0, 0, 0, 0, 0, 0, 0, 64], 23552)
add([64, 64, 64, 64, 64, 64, 64, 64], 46720)
for _ in range(8):
    add([random.randrange(128) for _ in range(8)], random.choice([11776, 23552, 46720]))

out = []
for v in vectors:
    e = build(); init_page(e)
    set_knobs(e, v["knobs"])
    blocks_out = []
    for _ in range(v["blocks"]):
        e.R[6] = PAGE; e.R[7] = 0x700
        e.A = v["pitch"] & 0xFFFFFF
        e.ret_stack.append(0x0F0F0F)
        e.run(0x145D21, end=0x0F0F0F)
        blocks_out.append(read_out(e))
    states = {hex(off): e.Y[PAGE + off] for off in range(0x10, 0x2B)}
    out.append({"knobs": v["knobs"], "pitch": v["pitch"],
                "outs": blocks_out, "states": states})

with open("/home/z/my-project/work/fm_fix/stat_vectors.json", "w") as f:
    json.dump(out, f)
print(f"dumped {len(out)} vectors -> work/fm_fix/stat_vectors.json")
nz = sum(1 for v in out for blk in v["outs"] for wv in blk if wv)
print("nonzero output words:", nz, "/", sum(len(v["outs"]) * 32 for v in out))
