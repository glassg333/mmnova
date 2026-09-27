#!/usr/bin/env python3
# Гипотеза: секция = несколько APLIB-подпотоков подряд.
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import depack, read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()
ref_vals = [int.from_bytes(w1[i:i+3], 'big') for i in range(405, len(w1) - len(w1) % 3, 3)]

secs = read_sections(data)
for k in (1, 2, 3):
    start = secs[k]["off"] + 8
    end = start + secs[k]["slen"]
    print(f"=== секция {k}: вход {start:#x}..{end:#x} ===")
    p = start
    total = bytearray()
    sub = 0
    while p < end - 8:
        try:
            out, ip = depack(data, p)
        except Exception as e:
            print(f"  подпоток {sub}: ОШИБКА {e}")
            break
        if len(out) == 0:
            print(f"  подпоток {sub}: пусто @ {p:#x}")
            break
        print(f"  подпоток {sub}: {p:#x} -> {len(out)} байт, ip_end {ip:#x}")
        total += out
        sub += 1
        prev = p
        p = ip
        if ip <= prev or ip >= end:
            break
    # сверка с эталоном
    my_vals = [int.from_bytes(total[i:i+3], 'little') for i in range(6, len(total) - (len(total)-6) % 3, 3)]
    n = min(len(my_vals), len(ref_vals))
    ok = 0
    for i in range(n):
        if my_vals[i] != ref_vals[i]: break
        ok = i + 1
    print(f"  итого {len(total)} байт; совпадение с эталоном: {ok}/{n} слов")
    if ok < n:
        lo = max(0, ok-4)
        print("   my :", [f"{v:06x}" for v in my_vals[lo:ok+6]])
        print("   ref:", [f"{v:06x}" for v in ref_vals[lo:ok+6]])
