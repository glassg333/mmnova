#!/usr/bin/env python3
# Финальная верификация гипотезы B: полная сверка всех трёх секций с эталонами
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()
w2 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp2_words.bin", "rb").read()
ref1 = [int.from_bytes(w1[i:i+3], 'big') for i in range(405, len(w1) - len(w1) % 3, 3)]
ref2 = [int.from_bytes(w2[i:i+3], 'big') for i in range(405, len(w2) - len(w2) % 3, 3)]
secs = read_sections(data)

def depack_b(d, ip, hard_end):
    out = bytearray(); last = 1; tag = 0
    def getbit():
        nonlocal tag, ip
        tag = (tag << 1) & 0xFFFFFFFF
        if (tag & 0xFF) == 0:
            if ip >= hard_end: raise EOFError
            b = d[ip]; ip += 1
            tag = ((b << 1) | 1) & 0xFFFFFFFF
            return (b >> 7) & 1
        return (tag >> 8) & 1
    def getgamma():
        v = 1
        while True:
            v = v * 2 + getbit()
            if getbit(): return v
    try:
        while True:
            if getbit():
                out.append(d[ip]); ip += 1
                continue
            v = getgamma()
            if v == 2: dist = last
            else:
                b = d[ip]
                dist = (v << 8) + b - 768 + 1
                ip += 1
                last = dist
            ba = getbit(); bb = getbit()
            sl = 2 * ba + bb
            L = sl if sl else getgamma() + 2
            if dist > 3328: L += 1
            cp = len(out) - dist
            if cp < 0: raise ValueError("dist>out")
            for i in range(L + 1):
                out.append(out[cp + i])
    except EOFError:
        pass
    return bytes(out), ip

outs = {}
for k in (1, 2, 3):
    s = secs[k]
    out, ip = depack_b(data, s["off"] + 8, s["off"] + 8 + s["slen"])
    outs[k] = out
    print(f"секция {k}: {len(out)} байт; первые 6: {out[:6].hex(' ')}; последние 12: {out[-12:].hex(' ')}")

def vals(buf, skip=6):
    return [int.from_bytes(buf[i:i+3], 'little') for i in range(skip, len(buf) - (len(buf)-skip) % 3, 3)]

v1, v2, v3 = vals(outs[1]), vals(outs[2]), vals(outs[3])
print(f"слов: sec1 {len(v1)}, sec2 {len(v2)}, sec3 {len(v3)}; ref1 {len(ref1)}, ref2 {len(ref2)}")

# DSP1: ref1 = sec1_records + sec3_records; sec1 вносит (len(v1)-2) слов?? JMP в начале sec1
# попробуем: ref1 = v1 + v3 (без JMP-префиксов) и ref2 = v2 + v3
def full_cmp(my, ref, name):
    n = min(len(my), len(ref)); ok = 0
    for i in range(n):
        if my[i] != ref[i]: break
        ok += 1
    print(f"{name}: {ok}/{n} слов совпало ({ok/n*100:.3f}%)")
    if ok < n:
        i = ok
        print(f"   первое расхождение @ {i}: my {my[i]:06x} vs ref {ref[i]:06x}")
        print(f"   my : {[f'{x:06x}' for x in my[max(0,i-4):i+4]]}")
        print(f"   ref: {[f'{x:06x}' for x in ref[max(0,i-4):i+4]]}")
    return ok

full_cmp(v1 + v3, ref1, "DSP1: [sec1+sec3] vs ref1")
full_cmp(v2 + v3, ref2, "DSP2: [sec2+sec3] vs ref2")
