#!/usr/bin/env python3
# Независимая экстракция DSP1/DSP2 образов из elektron_sfx6-60_os1.32b.bin
# Порт APLIB-варианта (mischa85/elektron-firmware-tool, MIT) + парсер записей DSP.
import struct, sys, os

BIN = "/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin"
OUT = "/home/z/my-project/mm_work"

OFFSET_BIAS, REUSE_GAMMA, FAR_THRESHOLD = 767, 2, 3328

class BitState:
    def __init__(self, data, pos):
        self.data, self.ip, self.iend, self.tag, self.err = data, pos, len(data), 0, False
    def gb(self):
        if self.ip >= self.iend:
            self.err = True; return 0
        b = self.data[self.ip]; self.ip += 1; return b
    def getbit(self):
        self.tag <<= 1
        if (self.tag & 0xFF) == 0:
            by = self.gb(); self.tag = (by << 1) | 1
            return (by >> 7) & 1
        return (self.tag >> 8) & 1
    def getgamma(self):
        v = 1
        while True:
            v = (v << 1) + self.getbit()
            if self.getbit(): break
            if self.err: break
            if v > 0x02000000: self.err = True; break
        return v

def ap_depack(src, start, srclen, outcap=16 << 20):
    """src: bytes; start: позиция потока после 8-байтного заголовка"""
    b = BitState(src, start)
    out = bytearray()
    last_off = 1
    while True:
        if b.err: break
        if b.getbit():  # literal
            if b.ip >= b.iend: break
            out.append(src[b.ip]); b.ip += 1
            continue
        g = b.getgamma()
        if g == REUSE_GAMMA:
            off = last_off
        else:
            off = (g << 8) + b.gb()
            if b.err: break
            if off == OFFSET_BIAS: break   # end of stream
            off -= OFFSET_BIAS
            last_off = off
        ba, bb = b.getbit(), b.getbit()
        sl = 2 * ba + bb
        L = sl if sl else b.getgamma() + 2
        if b.err: break
        if off > FAR_THRESHOLD: L += 1
        n = L + 1
        if off == 0 or len(out) < off: raise ValueError(f"bad offset {off} at out {len(out)}")
        cp = len(out) - off
        for i in range(n):
            out.append(out[cp + i])
        if len(out) > outcap: raise ValueError("overflow")
    return bytes(out)

def read_sections(data, base=0x4000):
    secs = []
    p = base
    while p + 8 <= len(data):
        (slen,) = struct.unpack_from(">I", data, p)
        if slen == 0 or slen > 0x1000000 or p + 8 + slen > len(data): break
        (bsum,) = struct.unpack_from(">I", data, p + 4)
        stream = data[p + 8: p + 8 + slen]
        calc = sum(stream) & 0xFFFFFFFF
        secs.append({"off": p, "slen": slen, "bsum_stored": bsum, "bsum_calc": calc,
                     "sum_ok": calc == bsum, "stream": stream})
        p += 8 + slen
    return secs

def bytes_to_words_be(buf):
    """24-bit big-endian words (как в готовом образе P-памяти)"""
    assert len(buf) % 3 == 0
    return [int.from_bytes(buf[i:i+3], 'big') for i in range(0, len(buf), 3)]

def parse_records(words):
    """Парсит записи [cmd][addr][count][data]; cmd 0=P 1=X 2=Y 3=JMP.
    Первый блок — бутлоадер [len][addr][data]."""
    records = []
    i = 0
    # Фаза 1 — бутлоадер: [len][addr][len слов]
    if len(words) >= 2 and 0 < words[0] < 512 and words[1] == 0x100:
        ln, adr = words[0], words[1]
        boot = words[2:2+ln]
        records.append(("boot", adr, ln, boot))
        i = 2 + ln
    else:
        raise ValueError(f"не бутлоадер в начале: {words[:4]:#x}")
    final_jmp = None
    while i < len(words):
        cmd, adr, cnt = words[i], words[i+1], words[i+2]
        if cmd == 3:
            final_jmp = (adr, cnt)
            i += 3
            break
        if cmd > 3:
            raise ValueError(f"bad cmd {cmd:#x} at word {i} (next {words[i+1]:#x} {words[i+2]:#x})")
        data = words[i+3:i+3+cnt]
        if len(data) < cnt:
            raise ValueError(f"truncated at word {i}: want {cnt} got {len(data)}")
        records.append((cmd, adr, cnt, data))
        i += 3 + cnt
    return records, final_jmp, i

def main():
    os.makedirs(OUT, exist_ok=True)
    data = open(BIN, "rb").read()
    print(f"BIN: {len(data)} байт")
    secs = read_sections(data)
    print(f"секций в контейнере: {len(secs)}")
    for k, s in enumerate(secs):
        print(f"  секция {k}: off {s['off']:#x} stored {s['slen']:#x} bytesum {'OK' if s['sum_ok'] else f'FAIL {s['bsum_calc']:#x} vs {s['bsum_stored']:#x}'}")
        dec = ap_depack(data, s['off'] + 8, s['slen'] + 8)
        s['dec'] = dec
        print(f"    -> распаковано {len(dec)} байт = {len(dec)//3} слов(если 24-bit)")
    return secs, data

if __name__ == "__main__":
    secs, data = main()
    # сохраняем распакованные секции
    for k, s in enumerate(secs):
        open(f"{OUT}/sec{k}.bin", "wb").write(s['dec'])
    print("Сохранено:", [f"{OUT}/sec{k}.bin" for k in range(len(secs))])
