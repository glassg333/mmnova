#!/usr/bin/env python3
"""Re-extract ALL data tables correctly from the verified memory images.

Repo bug: extract_tables.py did `off = base_addr - 0x100000` for P-tables
(docstring says direct mapping) → 26 of 43 tables are all-zero garbage.
The P-image is flat: word address == index (image covers P:$000000..$148263).

Outputs to /home/z/my-project/scripts/out_tables:
  - corrected .bin/.txt per table + TABLES_MANIFEST.json with verification
  - runtime-only tables (X/Y addresses beyond firmware images) are flagged
"""
import os, json

PACK = "/home/z/my-project/repo_mmnova/decompiled data/!decryptor 56300 - agent can make chorus and fm etc from original"
TBL = os.path.join(PACK, "tables")
IMG = os.path.join(PACK, "memory_images")
OUT = "/home/z/my-project/scripts/out_tables"
os.makedirs(OUT, exist_ok=True)

imgs = {
    "P": open(os.path.join(IMG, "dsp1_pmem.bin"), "rb").read(),
    "X": open(os.path.join(IMG, "dsp1_xmem.bin"), "rb").read(),
    "Y": open(os.path.join(IMG, "dsp1_ymem.bin"), "rb").read(),
}
img_max = {k: len(v) // 3 for k, v in imgs.items()}

def words_to_hex(words, per_line=8):
    out = []
    for i in range(0, len(words), per_line):
        chunk = words[i:i+per_line]
        out.append(f"  +${i:04x}: " + " ".join(f"{w:06x}" for w in chunk))
    return "\n".join(out)

manifest = []
for name in sorted(os.listdir(TBL)):
    if not name.endswith(".bin"):
        continue
    repo_blob = open(os.path.join(TBL, name), "rb").read()
    base_name = name[:-4]
    tag, addr_hex = base_name.split("_")[0], base_name.split("_")[1]
    addr = int(addr_hex, 16)
    nwords = len(repo_blob) // 3
    entry = {"file": base_name, "space": tag, "addr": f"${addr:06x}",
             "words": nwords, "repo_bytes": len(repo_blob)}
    if addr >= img_max[tag]:
        entry.update(status="RUNTIME-ONLY",
                     note="адрес вне образа прошивки (runtime-память, заливается ColdFire через HI08); из firmware не извлекаем",
                     repo_nz_pct=round(100*sum(1 for b in repo_blob if b)/max(1,len(repo_blob)),1))
        # keep repo blob as-is (unverifiable) for reference
        data = imgs[tag] if False else None
        bin_out = repo_blob
    else:
        off = addr * 3
        chunk = imgs[tag][off:off+len(repo_blob)]
        words = [int.from_bytes(chunk[i:i+3], "big") for i in range(0, len(chunk)-2, 3)]
        bin_out = chunk
        nz = sum(1 for b in chunk if b)
        repo_nz = sum(1 for b in repo_blob if b)
        entry["repo_nz_pct"] = round(100*repo_nz/max(1,len(repo_blob)),1)
        if nz == 0 and repo_nz == 0:
            entry.update(status="ZERO-IN-IMAGE",
                         note="в образе прошивки по этому адресу нули (runtime-область, инициализируется на живом устройстве)")
        else:
            same = (chunk == repo_blob)
            entry.update(status="VERIFIED" if same else "FIXED",
                         note="совпадает с образом" if same else "в репо был мусор (нулевые/сдвинутые байты) — перевыкачано из образа",
                         data_nz_pct=round(100*nz/len(chunk),1))
    open(os.path.join(OUT, base_name + ".bin"), "wb").write(bin_out)
    if entry["status"] in ("VERIFIED", "FIXED"):
        words = [int.from_bytes(bin_out[i:i+3], "big") for i in range(0, len(bin_out)-2, 3)]
        txt = (f"# Table: {base_name}\n# Source: {tag}-memory image dsp1_{tag.lower()}mem.bin (SHA-256 verified)\n"
               f"# Base address: {tag}:${addr:06x}\n# Length: {len(words)} words (24-bit BE)\n\n"
               + words_to_hex(words))
        open(os.path.join(OUT, base_name + ".txt"), "w").write(txt)
    manifest.append(entry)

json.dump(manifest, open(os.path.join(OUT, "TABLES_MANIFEST.json"), "w"), indent=2, ensure_ascii=False)

from collections import Counter
print(Counter(m["status"] for m in manifest))
for m in manifest:
    if m["status"] in ("FIXED",):
        print("FIXED:", m["file"], f"({m['data_nz_pct']}% real data, repo was {m['repo_nz_pct']}%)")
    elif m["status"] == "RUNTIME-ONLY":
        print("RUNTIME-ONLY:", m["file"], f"repo had {m['repo_nz_pct']}% nonzero")
    elif m["status"] == "ZERO-IN-IMAGE":
        print("ZERO-IN-IMAGE:", m["file"])
print("DONE ->", OUT)
