#!/usr/bin/env python3
"""Final descriptor regeneration with VERIFIED ColdFire layout.
Layout (verified against FM+STAT @0x58545 hexdump):
  +0x00 short_name (8-byte field, NUL-padded; +0x08 = display length tag)
  +0x09 display_name (NUL-padded, within 8-byte field to +0x10)
  +0x12 param names, 8 x 6-byte fields
  +0x42 defaults (8 bytes)
  +0x4A..0xB0 raw (mins/maxs/curves — kept as hex for future decoding)
"""
import os, sys, importlib.util, json

REPO_ROOT = "/home/z/my-project/repo_mmnova"
PACK = os.path.join(REPO_ROOT, "decompiled data/!decryptor 56300 - agent can make chorus and fm etc from original")
FW = os.path.join(REPO_ROOT, "decompiled data/!firmware bin and syx original/elektron_sfx6-60_os1.32b.bin")
OUT = "/home/z/my-project/scripts/out_descriptors"
os.makedirs(OUT, exist_ok=True)

buf = open(os.path.join(OUT, "coldfire_main.bin"), "rb").read()
assert len(buf) == 425858, len(buf)

def cstr(b):
    return b.split(b"\x00")[0].decode("ascii", errors="replace")

descriptors = []
base = 0x57FC5  # first table entry (page_id 0), stride 0xB0, matches FM+STAT @0x58545
while base + 0xB0 <= len(buf):
    blk = buf[base:base+0xB0]
    short = cstr(blk[0x00:0x08])
    tag = blk[0x08]
    disp = cstr(blk[0x09:0x11])
    params = [cstr(blk[0x12+i*6:0x18+i*6]) for i in range(8)]
    defaults = list(blk[0x42:0x4A])
    raw_tail = blk[0x4A:0xB0]
    # validity: known machine short name present
    valid_shorts = {"FM+", "GND", "SID", "SWAVE", "DPRO", "VO", "FX", "STAT"}
    if short in valid_shorts or (short and tag in (2,3,4,5,6,7,8)):
        descriptors.append({
            "coldfire_offset": f"0x{base:x}",
            "short_name": short,
            "display_name": disp,
            "full_name": (short.rstrip("+") + "-" + disp) if disp else short,
            "params": params,
            "defaults": [f"0x{v:02x}" for v in defaults],
            "raw_tail_hex": raw_tail.hex(),
        })
    base += 0xB0

print(f"coldfire descriptors: {len(descriptors)} blocks")
for d in descriptors:
    print("  %-8s %-10s @%s params=%s defaults=%s" % (
        d["short_name"], d["display_name"], d["coldfire_offset"],
        d["params"], d["defaults"]))
json.dump(descriptors, open(os.path.join(OUT, "all_machine_descriptors.json"), "w"), indent=2)

# ---- FX/FM descriptor JSONs: reuse repo parsers, skip its buggy printer ----
src_path = os.path.join(PACK, "scripts/decode_descriptors.py")
src = open(src_path, encoding="utf-8").read()
src = src.replace('BIN = "/home/z/my-project/work/mmnova/decompiled data/chorus and fm - original - decryptor 56300/descriptors/fx_descriptors_region.bin"',
                  f'BIN = r"{os.path.join(PACK, "descriptors/fx_descriptors_region.bin")}"')
src = src.replace('FM_BIN = "/home/z/my-project/work/mmnova/decompiled data/chorus and fm - original - decryptor 56300/descriptors/fm_machine_descriptors.bin"',
                  f'FM_BIN = r"{os.path.join(PACK, "descriptors/fm_machine_descriptors.bin")}"')
src = src.replace('OUT_DIR = "/home/z/my-project/work/extract/descriptors"',
                  f'OUT_DIR = r"{OUT}"')
src = src.replace('if __name__ == "__main__":\n    main()', '')
spec = importlib.util.spec_from_loader("dd", loader=None)
dd = importlib.util.module_from_spec(spec)
exec(compile(src, src_path, "exec"), dd.__dict__)

data = open(dd.BIN, "rb").read()
blocks = [dd.parse_fx_block(data, i*0xB0) for i in range(len(data)//0xB0)]
blocks = [b for b in blocks if b]
json.dump(blocks, open(os.path.join(OUT, "fx_descriptors.json"), "w"), indent=2)
print(f"fx_descriptors.json: {len(blocks)} blocks")

fmd = open(dd.FM_BIN, "rb").read()
fm_blocks = [dd.parse_fm_block(fmd, i*0x88) for i in range(len(fmd)//0x88)]
fm_blocks = [b for b in fm_blocks if b]
json.dump(fm_blocks, open(os.path.join(OUT, "fm_descriptors.json"), "w"), indent=2)
print(f"fm_descriptors.json: {len(fm_blocks)} blocks")
print("DONE")
