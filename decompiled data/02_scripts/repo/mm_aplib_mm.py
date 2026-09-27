#!/usr/bin/env python3
# Точный 1:1 Python-порт ColdFire-декомпрессора Monomachine (BIN 0x1C4–0x362).
# Верифицировано по дизассемблу (capstone, MCF5206e):
#   getbit: d1<<=1; если младший байт==0 → d1 = byte*2+1; бит = (d1>>8)&1
#   гамма:  v=1; v = v*2 + bit; пока stop-bit==0
#   смещение: v = гамма; если v==2 → reuse (dist=d4=last dist, init 1)
#             иначе byte b: off = (v<<8) + b - 768; off==0xFF → EOF
#             dist = off+1; last = dist
#   длина:  sl = 2*ba+bb (2 бита); sl? L=sl : L=гамма+2
#           dist > 3328 (0xD00) → L += 1 (shi→0xFF→extb→-1; sub → +1)
#   копия:  n = L+1 байт из out[-dist], побайтно (перекрытие OK)
import struct, os, sys

OUTDIR = "/home/z/my-project/mm_work"

class BR:
    __slots__ = ("data", "ip", "tag")
    def __init__(self, data, pos):
        self.data, self.ip, self.tag = data, pos, 0
    def getbit(self):
        self.tag = (self.tag << 1) & 0xFFFFFFFF
        if (self.tag & 0xFF) == 0:
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

def depack(data, ip):
    br = BR(data, ip)
    out = bytearray()
    last_dist = 1
    while True:
        if br.getbit():
            out.append(data[br.ip]); br.ip += 1
            continue
        v = br.getgamma()
        if v == 2:
            dist = last_dist
        else:
            b = data[br.ip]
            off = (v << 8) + b - 768
            if off == 0xFF:
                break
            br.ip += 1
            dist = off + 1
            last_dist = dist
        ba = br.getbit(); bb = br.getbit()
        sl = 2 * ba + bb
        L = sl if sl else br.getgamma() + 2
        if dist > 3328:
            L += 1
        n = L + 1
        cp = len(out) - dist
        if cp < 0:
            raise ValueError(f"dist {dist} > out {len(out)}")
        for i in range(n):
            out.append(out[cp + i])
    return bytes(out), br.ip

def read_sections(data, base=0x4000):
    secs = []
    p = base
    while p + 8 <= len(data):
        (slen,) = struct.unpack_from(">I", data, p)
        if slen == 0 or slen > 0x1000000 or p + 8 + slen > len(data):
            break
        (bsum,) = struct.unpack_from(">I", data, p + 4)
        stream = data[p + 8: p + 8 + slen]
        calc = sum(stream) & 0xFFFFFFFF
        secs.append({"off": p, "slen": slen, "bsum": bsum, "calc": calc,
                     "sum_ok": calc == bsum})
        p += 8 + slen
    return secs

if __name__ == "__main__":
    BIN = "/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin"
    data = open(BIN, "rb").read()
    os.makedirs(OUTDIR, exist_ok=True)
    secs = read_sections(data)
    print(f"секций: {len(secs)}")
    for k, s in enumerate(secs):
        mark = "OK" if s["sum_ok"] else f"FAIL calc={s['calc']:#x}"
        out, ip = depack(data, s["off"] + 8)
        s["dec"] = out
        print(f"  секция {k}: @ {s['off']:#x} stored {s['slen']:#x} sum {mark} "
              f"-> {len(out)} байт (ip_end {ip:#x}, ожид. {s['off']+8+s['slen']:#x})")
        open(f"{OUTDIR}/sec{k}.bin", "wb").write(out)
