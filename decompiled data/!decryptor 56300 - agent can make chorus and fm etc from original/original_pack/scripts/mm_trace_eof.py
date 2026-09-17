#!/usr/bin/env python3
# Трассировка последних токенов перед ложным EOF + ручное декодирование битов
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import read_sections

data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
secs = read_sections(data)

class BR2:
    def __init__(self, data, pos):
        self.data, self.ip, self.tag = data, pos, 0
        self.hist = []           # (ip_до, байт) пополнений тега
    def getbit(self):
        self.tag = (self.tag << 1) & 0xFFFFFFFF
        if (self.tag & 0xFF) == 0:
            self.hist.append((self.ip, self.data[self.ip]))
            b = self.data[self.ip]; self.ip += 1
            self.tag = ((b << 1) | 1) & 0xFFFFFFFF
            return (b >> 7) & 1
        return (self.tag >> 8) & 1
    def getgamma(self):
        v = 1
        while True:
            v = v * 2 + self.getbit()
            if self.getbit():
                return v

def depack_trace(data, ip):
    br = BR2(data, ip)
    out = bytearray()
    last = 1
    toks = []
    while True:
        if br.getbit():
            out.append(data[br.ip]); br.ip += 1
            toks.append(('lit', len(out), br.ip))
            continue
        v = br.getgamma()
        if v == 2:
            dist = last
            toks.append(('reuse', len(out), br.ip, dist))
        else:
            b = data[br.ip]
            off = (v << 8) + b - 768
            if off == 0xFF:
                toks.append(('EOF', len(out), br.ip, v, b))
                break
            br.ip += 1
            dist = off + 1
            last = dist
            toks.append(('match', len(out), br.ip, dist, v, b))
        ba = br.getbit(); bb = br.getbit()
        sl = 2 * ba + bb
        L = sl if sl else br.getgamma() + 2
        if dist > 3328:
            L += 1
        n = L + 1
        toks.append(('len', len(out), br.ip, sl, L, n))
        cp = len(out) - dist
        for i in range(n):
            out.append(out[cp + i])
    return bytes(out), br.ip, toks

for k in (1, 3):
    buf, ip, toks = depack_trace(data, secs[k]["off"] + 8)
    print(f"=== секция {k}: out {len(buf)}, eof_ip {ip:#x} ===")
    for t in toks[-8:]:
        print("   ", t)
    print(f"   входные байты у EOF: {data[ip-8:ip+16].hex(' ')}")
