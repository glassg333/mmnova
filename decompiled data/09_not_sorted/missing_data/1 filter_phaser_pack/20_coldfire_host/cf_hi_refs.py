#!/usr/bin/env python3
"""Find all code refs to 0x5000xx/0x6000xx in coldfire_main.bin (alignment-aware)."""
import capstone, re

CF = "/home/z/my-project/work/mmnova/decompiled data/05_descriptors/coldfire_main.bin"
cf = open(CF, "rb").read()
md = capstone.Cs(capstone.CS_ARCH_M68K, capstone.CS_MODE_BIG_ENDIAN | capstone.CS_MODE_M68K_040)

pat = re.compile(rb"\x00[\x50\x60]\x00[\x00-\xff]")  # 00 50/60 00 xx
hits = []
start = 0
while True:
    m = pat.search(cf, start)
    if not m:
        break
    hits.append(m.start())
    start = m.start() + 2
print("candidate ext words:", len(hits))

found = {}
for h in hits:
    for back in (2, 4, 6, 8, 10):
        o = h - back
        if o < 0:
            continue
        try:
            ins = next(md.disasm(cf[o:o + 12], o))
        except StopIteration:
            continue
        if re.search(r"\$[56][0-9a-fA-F]{5}", ins.op_str) and ins.address + ins.size >= h + 4:
            found.setdefault(ins.address, (ins.size, ins.mnemonic, ins.op_str))
            break
print("decoded refs:", len(found))
for a in sorted(found):
    s, m2, o2 = found[a]
    print(f"{a:#07x}: {cf[a:a+s].hex():<18} {m2} {o2}")
