#!/usr/bin/env python3
# Локализует мои секции внутри эталонного потока dsp1_words.bin
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import depack, read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()

secs = read_sections(data)
decs = {}
for k in (1, 2, 3):
    decs[k], ip = depack(data, secs[k]["off"] + 8)

# эталон: после boot-фазы (2+133 слов = 405 байт) начинаются записи
print("dsp1_words[405:429] =", w1[405:429].hex())
# ищем начало sec1 (030000 660000) в w1
pat = bytes.fromhex("030000660000")
idx = w1.find(pat)
print("паттерн 030000660000 найден в w1 по смещению:", idx)
# сравниваем sec1 с w1 начиная с idx
for k in (1, 2, 3):
    m = decs[k]
    if idx >= 0:
        n = min(len(m), len(w1) - idx)
        mm = 0
        while mm < n and m[mm] == w1[idx + mm]:
            mm += 1
        print(f"секция {k}: len {len(m)}, совпадает с w1[{idx}+...] до {mm} байт ({mm/len(m)*100:.2f}%)")
        if mm < len(m):
            lo = max(0, mm - 30)
            print(f"  mine[{lo}:{mm+24}] = {m[lo:mm+24].hex(' ')}")
            print(f"  w1  [{idx+lo}:{idx+mm+24}] = {w1[idx+lo:idx+mm+24].hex(' ')}")
            # какое слово
            print(f"  слово #{mm//3}, смещение {mm%3}")
