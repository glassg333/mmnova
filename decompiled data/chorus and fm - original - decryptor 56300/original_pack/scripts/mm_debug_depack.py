#!/usr/bin/env python3
# Отладка декомпрессора: сравнение ПОСЛЕДОВАТЕЛЬНОСТИ ЗНАЧЕНИЙ с эталоном w1.
# Секция (LE24) -> значения; w1[405:] (BE24) -> значения (поток как отправлен).
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
import mm_aplib_mm as mm
from mm_aplib_mm import read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
w1 = open("/home/z/my-project/repo-mmnova/decompiled data/dsp1_words.bin", "rb").read()

# эталонные значения (после boot-фазы 135 слов)
ref_vals = [int.from_bytes(w1[i:i+3], 'big') for i in range(405, len(w1) - len(w1) % 3, 3)]
print(f"эталон: {len(ref_vals)} слов после бут-фазы")

secs = read_sections(data)
# инструментированный depack: пишем лог (вых_позиция, вход_позиция, тип, параметры)
orig = mm.depack

def depack_trace(d, ip, maxlen=16 << 20):
    br = mm.BR(d, ip)
    out = bytearray()
    last = 1
    log = []
    while True:
        if br.getbit():
            out.append(d[br.ip]); br.ip += 1
            continue
        v = br.getgamma()
        if v == 2:
            dist = last
            log.append((len(out), br.ip, 'reuse', dist))
        else:
            b = d[br.ip]
            off = (v << 8) + b - 768
            if off == 0xFF:
                log.append((len(out), br.ip, 'EOF', v))
                break
            br.ip += 1
            dist = off + 1
            last = dist
            log.append((len(out), br.ip, 'match', dist, v, b))
        ba = br.getbit(); bb = br.getbit()
        sl = 2 * ba + bb
        L = sl if sl else br.getgamma() + 2
        if dist > 3328:
            L += 1
        n = L + 1
        log.append((len(out), br.ip, 'len', sl, L, n))
        cp = len(out) - dist
        if cp < 0:
            raise ValueError(f"dist {dist} > out {len(out)}")
        for i in range(n):
            out.append(out[cp + i])
    return bytes(out), br.ip, log

buf, ip, log = depack_trace(data, secs[1]["off"] + 8)
print(f"секция 1: распаковано {len(buf)} байт, вход потреблён до {ip:#x} (конец секции {secs[1]['off']+8+secs[1]['slen']:#x})")

# мои значения: пропускаем 2 слова JMP (6 байт)
my_vals = [int.from_bytes(buf[i:i+3], 'little') for i in range(6, len(buf) - (len(buf)-6) % 3, 3)]
n = min(len(my_vals), len(ref_vals))
mm_ok = 0
for i in range(n):
    if my_vals[i] != ref_vals[i]:
        break
    mm_ok = i + 1
print(f"совпадение значений: {mm_ok} слов из {n}")
if mm_ok < n:
    lo = max(0, mm_ok - 6)
    print("  my :", [f"{v:06x}" for v in my_vals[lo:mm_ok+8]])
    print("  ref:", [f"{v:06x}" for v in ref_vals[lo:mm_ok+8]])
    # какой лог-элемент покрывает выходную позицию расхождения
    outpos = 6 + mm_ok * 3
    for j, entry in enumerate(log):
        if entry[0] <= outpos < entry[0] + 6000:
            print("  ближайшие операции:", log[max(0,j-3):j+3])
            break
