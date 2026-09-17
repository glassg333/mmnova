#!/usr/bin/env python3
"""Audit all tables: zero-content detection + relocation search in images."""
import os, json

PACK = "/home/z/my-project/repo_mmnova/decompiled data/!decryptor 56300 - agent can make chorus and fm etc from original"
TBL = os.path.join(PACK, "tables")
IMG = os.path.join(PACK, "memory_images")

pmem = open(os.path.join(IMG, "dsp1_pmem.bin"), "rb").read()
xmem = open(os.path.join(IMG, "dsp1_xmem.bin"), "rb").read()
ymem = open(os.path.join(IMG, "dsp1_ymem.bin"), "rb").read()
imgs = {"P": pmem, "X": xmem, "Y": ymem}

audit = []
for name in sorted(os.listdir(TBL)):
    if not name.endswith(".bin"):
        continue
    blob = open(path := os.path.join(TBL, name), "rb").read()
    tag, addr_hex = name.split("_")[0], name.split("_")[1]
    addr = int(addr_hex, 16)
    nz = sum(1 for b in blob if b)
    pct = 100 * nz / len(blob) if blob else 0
    in_place = imgs[tag][addr*3:addr*3+len(blob)] == blob
    # if not in place, search first non-zero 12-byte window elsewhere
    found_at = None
    if not in_place and nz:
        first = next(i for i, b in enumerate(blob) if b)
        w = blob[first:first+12]
        idx = imgs[tag].find(w)
        if idx >= 0:
            found_at = f"0x{idx//3:06x}"
    status = "IN-PLACE" if in_place else ("ALL-ZERO" if nz == 0 else f"RELOC-> {found_at}")
    audit.append((name, len(blob)//3, f"{pct:.0f}%", status))

print(f"{'file':<44} {'words':>6} {'nz%':>5}  status")
for a in audit:
    print(f"{a[0]:<44} {a[1]:>6} {a[2]:>5}  {a[3]}")
print()
n_zero = sum(1 for a in audit if a[3] == "ALL-ZERO")
n_reloc = sum(1 for a in audit if "RELOC" in a[3])
n_ok = sum(1 for a in audit if a[3] == "IN-PLACE")
print(f"summary: {n_ok} in-place OK / {n_zero} all-zero / {n_reloc} relocated")
