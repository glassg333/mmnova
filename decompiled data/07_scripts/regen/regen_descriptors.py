#!/usr/bin/env python3
"""Regenerate missing descriptor artifacts:
1. coldfire_main.bin (sec0 decompressed, 425,858 bytes expected)
2. all_machine_descriptors.json
3. fx_descriptors.json + fm_descriptors.json
"""
import os, sys, importlib.util, json

REPO_ROOT = "/home/z/my-project/repo_mmnova"
PACK = os.path.join(REPO_ROOT, "decompiled data/!decryptor 56300 - agent can make chorus and fm etc from original")
FW = os.path.join(REPO_ROOT, "decompiled data/!firmware bin and syx original/elektron_sfx6-60_os1.32b.bin")
OUT = "/home/z/my-project/scripts/out_descriptors"
os.makedirs(OUT, exist_ok=True)

# ---- import mm_aplib_mm / mm_final_extract from original_pack/scripts ----
sys.path.insert(0, os.path.join(PACK, "original_pack/scripts"))
import mm_aplib_mm
import mm_final_extract

data = open(FW, "rb").read()
print(f"BIN size: {len(data)}")
sections = mm_aplib_mm.read_sections(data)
for i, s in enumerate(sections):
    print(f"  section {i}: off=0x{s['off']:x} slen={s['slen']} sum_ok={s['sum_ok']}")

sec0 = sections[0]
off = sec0['off'] + 8
buf, n = mm_final_extract.depack_full(data, off, off + sec0['slen'])
cf_path = os.path.join(OUT, "coldfire_main.bin")
open(cf_path, "wb").write(buf)
print(f"coldfire_main.bin: {len(buf)} bytes (README claims 425,858)")

# ---- scan descriptors, reusing repo logic verbatim ----
descriptors = []
base, end = 0x58000, min(0x60000, len(buf))
for o in range(base, end, 0xB0):
    if o + 0xB0 > len(buf):
        break
    blk = buf[o:o+0xB0]
    page_id = blk[4]
    short_name = blk[5:13].split(b"\x00")[0].decode("ascii", errors="replace")
    display_name = blk[14:22].split(b"\x00")[0].decode("ascii", errors="replace")
    if not short_name or not display_name:
        continue
    if not all(c.isalpha() or c in "+-" for c in short_name):
        continue
    if not all(c.isalpha() or c in "+-_" for c in display_name):
        continue
    if len(short_name) < 1 or len(display_name) < 2:
        continue
    params = []
    for i in range(8):
        s = blk[0x17 + i*6: 0x17 + i*6 + 6]
        params.append(s.split(b"\x00")[0].decode("ascii", errors="replace"))
    defaults = list(blk[0x47:0x4F])
    descriptors.append({
        "coldfire_offset": f"0x{o:x}", "page_id": page_id,
        "short_name": short_name, "display_name": display_name,
        "full_name": f"{short_name}-{display_name}" if short_name != display_name else display_name,
        "params": params, "defaults": defaults,
    })
print(f"all_machine_descriptors: {len(descriptors)} blocks")
json.dump(descriptors, open(os.path.join(OUT, "all_machine_descriptors.json"), "w"), indent=2)

# ---- decode fx/fm descriptor regions via repo script ----
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
dd.main()
print("DONE ->", OUT)
