#!/usr/bin/env python3
# Гипотеза B: (v=3,b=255) = обычная дистанция 256, а не EOF.
# Декомпрессор работает до исчерпания входа секции.
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()
ref1 = [int.from_bytes(w1[i:i+3], 'big') for i in range(405, len(w1) - len(w1) % 3, 3)]
secs = read_sections(data)

def depack_b(d, ip, hard_end):
    out = bytearray()
    last = 1
    ip0 = ip
    tag = 0
    class Ctx: pass
    ctx = Ctx()
    def getbit():
        nonlocal tag, ip
        tag = (tag << 1) & 0xFFFFFFFF
        if (tag & 0xFF) == 0:
            nonlocal ip
            if ip >= hard_end:
                raise EOFError("вход исчерпан")
            b = d[ip]; ip += 1
            tag = ((b << 1) | 1) & 0xFFFFFFFF
            return (b >> 7) & 1
        return (tag >> 8) & 1
    def getgamma():
        v = 1
        while True:
            v = v * 2 + getbit()
            if getbit():
                return v
    try:
        while True:
            if getbit():
                out.append(d[ip]); ip += 1
                continue
            v = getgamma()
            if v == 2:
                dist = last
            else:
                b = d[ip]
                off = (v << 8) + b - 768
                ip += 1
                dist = off + 1
                last = dist
            ba = getbit(); bb = getbit()
            sl = 2 * ba + bb
            L = sl if sl else getgamma() + 2
            if dist > 3328:
                L += 1
            n = L + 1
            cp = len(out) - dist
            if cp < 0:
                raise ValueError(f"dist {dist} > out {len(out)}")
            for i in range(n):
                out.append(out[cp + i])
    except EOFError:
        pass
    return bytes(out), ip

for k in (1, 2, 3):
    s = secs[k]
    out, ip = depack_b(data, s["off"] + 8, s["off"] + 8 + s["slen"])
    print(f"секция {k}: {len(out)} байт, ip_end {ip:#x} (конец {s['off']+8+s['slen']:#x})")
    vals = [int.from_bytes(out[i:i+3], 'little') for i in range(6, len(out) - (len(out)-6) % 3, 3)]
    if k == 1:
        n = min(len(vals), len(ref1)); ok = 0
        for i in range(n):
            if vals[i] != ref1[i]: break
            ok = i + 1
        print(f"  совпадение с ref1: {ok}/{n}")
