#!/usr/bin/env python3
"""Spot-verify extracted tables against the authentic memory images."""
import os, json

PACK = "/home/z/my-project/repo_mmnova/decompiled data/!decryptor 56300 - agent can make chorus and fm etc from original"
TBL = os.path.join(PACK, "tables")
IMG = os.path.join(PACK, "memory_images")

pmem = open(os.path.join(IMG, "dsp1_pmem.bin"), "rb").read()
xmem = open(os.path.join(IMG, "dsp1_xmem.bin"), "rb").read()
ymem = open(os.path.join(IMG, "dsp1_ymem.bin"), "rb").read()
# P images: 24-bit words, big-endian, base P:$0000 at file offset 0
def pword(addr):
    off = addr * 3
    return int.from_bytes(pmem[off:off+3], "big")

man = json.load(open(os.path.join(TBL, "TABLES_MANIFEST.json")))
print("manifest type:", type(man).__name__, "entries:", len(man) if hasattr(man, "__len__") else "?")
if isinstance(man, dict):
    items = list(man.items())[:3]
    print("sample keys:", [k for k, _ in items])
    print("sample val:", items[0])

ok, bad = 0, 0
for name in sorted(os.listdir(TBL)):
    if not name.endswith(".bin"):
        continue
    path = os.path.join(TBL, name)
    blob = open(path, "rb").read()
    # parse address from name: P_144ac7_... / X_0002c0_... / Y_000132_...
    tag, addr_hex = name.split("_")[0], name.split("_")[1]
    addr = int(addr_hex, 16)
    if tag == "P":
        data = pmem[addr*3:addr*3+len(blob)]
        match = (data == blob)
    elif tag == "X":
        data = xmem[addr*3:addr*3+len(blob)]
        match = (data == blob)
    elif tag == "Y":
        data = ymem[addr*3:addr*3+len(blob)]
        match = (data == blob)
    else:
        continue
    if match:
        ok += 1
    else:
        bad += 1
        print("MISMATCH:", name)
print(f"tables verified: {ok} OK, {bad} MISMATCH out of {ok+bad}")

# FM ratio table sanity: 24 ratios
print("\nFM ratio table P:$141A80 (first 24 words as 1.23 fixed):")
vals = [pword(0x141A80 + i) for i in range(24)]
print(["%.4f" % (v / 0x800000) for v in vals])
