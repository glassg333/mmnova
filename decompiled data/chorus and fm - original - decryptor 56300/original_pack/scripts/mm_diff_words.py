#!/usr/bin/env python3
# Сравнивает мою декомпрессию с эталонным потоком dsp1_words.bin
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import depack, read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()
w2 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp2_words.bin", "rb").read()
print(f"dsp1_words.bin: {len(w1)} байт = {len(w1)//3} слов")
print(f"dsp2_words.bin: {len(w2)} байт = {len(w2)//3} слов")

secs = read_sections(data)
for k in (1, 2, 3):
    mine, ip = depack(data, secs[k]["off"] + 8)
    n = min(len(mine), len(w1))
    # ищем первый расхождённый байт против w1 и w2
    for name, ref in (("dsp1", w1), ("dsp2", w2)):
        m = 0
        while m < min(len(mine), len(ref)) and mine[m] == ref[m]:
            m += 1
        print(f"секция {k}: моя длина {len(mine)}, совпадение с {name}_words до байта {m} ({m/len(mine)*100:.1f}%)")
        if m < len(mine):
            lo = max(0, m-24)
            print(f"   mine[{lo}:{m+16}] = {mine[lo:m+16].hex()}")
            print(f"   {name} [{lo}:{m+16}] = {ref[lo:m+16].hex()}")
    open(f"/home/z/my-project/mm_work/mysec{k}.bin", "wb").write(mine)
