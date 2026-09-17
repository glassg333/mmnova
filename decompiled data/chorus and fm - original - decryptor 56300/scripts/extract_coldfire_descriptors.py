#!/usr/bin/env python3
"""
Extract the full ColdFire main image from the firmware BIN and locate the
complete machine descriptor table. This gives us the canonical machine names
for ALL slots (including m20, m21, m32, m33 which are missing from the
partial fx_descriptors_region.bin).

The ColdFire main image is in section 0 of the BIN at offset 0x4000+.
Section format: [u32 stored_len BE][u32 byte_sum BE][APLib-compressed stream].
The decompressor is the Monomachine variant of aPLib (no in-stream EOF marker).

We reuse the already-verified decompressor from the previous extraction
session (scripts/mm_aplib_mm.py + mm_final_extract.py from the repo).

Output: /home/z/my-project/work/extract/coldfire_main.bin (425,858 bytes)
        /home/z/my-project/work/extract/all_machine_descriptors.json
"""
import os, sys, struct

BIN_PATH = "/home/z/my-project/work/mmnova/original bin and syx firmware/elektron_sfx6-60_os1.32b.bin"
OUT_DIR = "/home/z/my-project/work/extract"
os.makedirs(OUT_DIR, exist_ok=True)

# Import the verified aPLib decompressor from the repo
sys.path.insert(0, "/home/z/my-project/work/mmnova/decompiled data/chorus and fm - original - decryptor 56300/scripts")
from mm_aplib_mm import read_sections, depack
from mm_final_extract import depack_full

def main():
    data = open(BIN_PATH, "rb").read()
    print(f"BIN size: {len(data)} bytes")
    sections = read_sections(data)
    print(f"Sections found: {len(sections)}")
    for i, s in enumerate(sections):
        print(f"  section {i}: offset=0x{s['off']:x} stored_len={s['slen']} byte_sum=0x{s['bsum']:x} sum_ok={s['sum_ok']}")

    # Section 0 = ColdFire main
    sec0 = sections[0]
    off = sec0['off'] + 8  # skip [u32 slen][u32 sum]
    out, n = depack_full(data, off, off + sec0['slen'])
    out_path = os.path.join(OUT_DIR, "coldfire_main.bin")
    open(out_path, "wb").write(out)
    print(f"\nColdFire main image extracted: {len(out)} bytes -> {out_path}")

    # Scan for machine descriptor blocks. Per REPORT.md, descriptors start at 0x58540+
    # with stride 0xB0 (176 bytes). Each block has:
    #   +0x04 page_id (machine's internal ID)
    #   +0x05 short_name (8 bytes, e.g. "FX", "FM+", "SWAVE", "DPRO", "VO")
    #   +0x0E display_name (8 bytes, e.g. "CHORUS", "REVERB", "PHASER")
    # Let's scan from 0x58000 to 0x60000 in 0xB0 stride and collect all valid blocks.
    print("\n=== Scanning for machine descriptors in ColdFire main ===")
    descriptors = []
    base = 0x58000
    end = min(0x60000, len(out))
    for off in range(base, end, 0xB0):
        if off + 0xB0 > len(out):
            break
        blk = out[off:off+0xB0]
        page_id = blk[4]
        short_name = blk[5:13].split(b"\x00")[0].decode("ascii", errors="replace")
        display_name = blk[14:22].split(b"\x00")[0].decode("ascii", errors="replace")
        # Heuristic: valid block has ASCII short_name (2-4 uppercase chars) and ASCII display_name
        if not short_name or not display_name:
            continue
        if not all(c.isalpha() or c in "+-" for c in short_name):
            continue
        if not all(c.isalpha() or c in "+-_" for c in display_name):
            continue
        if len(short_name) < 1 or len(display_name) < 2:
            continue
        # Read 8 params (6-byte stride starting at offset 0x17)
        params = []
        for i in range(8):
            s = blk[0x17 + i*6 : 0x17 + i*6 + 6]
            params.append(s.split(b"\x00")[0].decode("ascii", errors="replace"))
        defaults = list(blk[0x47:0x4F])
        descriptors.append({
            "coldfire_offset": f"0x{off:x}",
            "page_id": page_id,
            "short_name": short_name,
            "display_name": display_name,
            "full_name": f"{short_name}-{display_name}" if short_name != display_name else display_name,
            "params": params,
            "defaults": defaults,
        })

    print(f"Found {len(descriptors)} descriptor blocks:")
    for d in descriptors:
        params = "  ".join(f"{p:<6}" for p in d["params"])
        defs = " ".join(f"0x{v:02x}" for v in d["defaults"])
        print(f"  page_id={d['page_id']:2d}  {d['full_name']:<16s}  params: {params}")
        print(f"                                defaults: {defs}")

    # Save JSON
    import json
    json_path = os.path.join(OUT_DIR, "all_machine_descriptors.json")
    json.dump(descriptors, open(json_path, "w"), indent=2)
    print(f"\n{json_path} written ({len(descriptors)} descriptors)")

if __name__ == "__main__":
    main()
