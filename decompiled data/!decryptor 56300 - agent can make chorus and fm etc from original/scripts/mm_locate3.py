#!/usr/bin/env python3
# Точная локализация сбоя декомпрессора в секции 3 (правильные смещения эталона)
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import depack, read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()
w2 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp2_words.bin", "rb").read()
ref1 = [int.from_bytes(w1[i:i+3], 'big') for i in range(405, len(w1) - len(w1) % 3, 3)]
ref2 = [int.from_bytes(w2[i:i+3], 'big') for i in range(405, len(w2) - len(w2) % 3, 3)]

secs = read_sections(data)
buf1, ip1 = depack(data, secs[1]["off"] + 8)
buf2, ip2 = depack(data, secs[2]["off"] + 8)
buf3, ip3 = depack(data, secs[3]["off"] + 8)

def vals_le(buf, skip=6):
    return [int.from_bytes(buf[i:i+3], 'little') for i in range(skip, len(buf) - (len(buf)-skip) % 3, 3)]

v1, v2, v3 = vals_le(buf1), vals_le(buf2), vals_le(buf3)
print(f"sec1: {len(v1)} слов (JMP пропущен), sec2: {len(v2)}, sec3: {len(v3)}")

# sec1 vs ref1
def cmp(my, ref, name):
    n = min(len(my), len(ref)); ok = 0
    for i in range(n):
        if my[i] != ref[i]: break
        ok = i + 1
    print(f"{name}: {ok}/{n} слов совпало (моя длина {len(my)}, эталон {len(ref)})")
    return ok

ok1 = cmp(v1, ref1, "sec1 vs ref1")
# sec2 vs ref2
ok2 = cmp(v2, ref2, "sec2 vs ref2")
# sec3 vs ref1[ok1:] и ref2[ok2:]
ok3a = cmp(v3, ref1[ok1:], "sec3 vs ref1[3579:]")
ok3b = cmp(v3, ref2[ok2:], "sec3 vs ref2[...]")

# где именно расхождение в sec3
if ok3a < len(v3):
    i = ok3a
    print(f"\nsec3 слово #{i}: my {v3[i]:06x}, ref {ref1[ok1+i]:06x}")
    print(f"контекст my : {[f'{x:06x}' for x in v3[max(0,i-8):i+8]]}")
    print(f"контекст ref: {[f'{x:06x}' for x in ref1[ok1+max(0,i-8):ok1+i+8]]}")
