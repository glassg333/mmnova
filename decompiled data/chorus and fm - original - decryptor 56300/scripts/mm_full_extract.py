#!/usr/bin/env python3
# Полная независимая экстракция DSP1/DSP2 образов из Monomachine OS 1.32b BIN.
# 1) APLIB-вариант Monomachine (порт ColdFire-декомпрессора 0x1C4, EOF = off==0xFF)
# 2) Загрузчик func_7ce: записи [cmd LE][addr LE][count LE][data LE], cmd 0=P 1=X 2=Y, 3=JMP P:addr
#    конец = байт 0x04 (cmd==4)
import struct, os, sys

sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import depack, read_sections

OUT = "/home/z/my-project/mm_work"
BIN = "/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin"

def words_le(buf):
    """24-bit little-endian слова"""
    assert len(buf) % 3 == 0
    return [int.from_bytes(buf[i:i+3], 'little') for i in range(0, len(buf), 3)]

def parse_stream(buf):
    """Разбирает поток записей func_7ce. Возвращает (records, final_jmp, consumed)."""
    records = []
    w = words_le(buf)
    i = 0
    final = None
    while i < len(w):
        cmd = w[i]
        if cmd == 3:                     # JMP P:addr (a4=6 → 2 слова)
            final = ("JMP", w[i+1])
            i += 2
            break
        if cmd == 4:                     # маркер конца
            i += 1
            break
        addr, cnt = w[i+1], w[i+2]
        data = w[i+3:i+3+cnt]
        if len(data) < cnt:
            raise ValueError(f"обрыв записи @ word {i}: cnt {cnt}, есть {len(data)}")
        records.append((cmd, addr, cnt, data))
        i += 3 + cnt
    return records, final, i, len(w)

def build_mem(records):
    mem = {}
    for cmd, addr, cnt, data in records:
        base = {0: 'P', 1: 'X', 2: 'Y'}[cmd]
        d = mem.setdefault(base, {})
        for k, v in enumerate(data):
            d[addr + k] = v
    return mem

def main():
    data = open(BIN, "rb").read()
    secs = read_sections(data)
    print(f"секций: {len(secs)}")
    streams = {}
    for k in range(len(secs)):
        dec, ip = depack(data, secs[k]["off"] + 8)
        open(f"{OUT}/sec{k}.bin", "wb").write(dec)
        streams[k] = dec
        print(f"  секция {k}: decompressed {len(dec)} байт")
    # секции 1..3 = потоки записей DSP
    allmem = {}
    for k in (1, 2, 3):
        buf = streams[k]
        recs, final, consumed, total = parse_stream(buf)
        print(f"  секция {k}: {len(recs)} записей, {total} слов, consumed {consumed}/{total}, final={final}")
        if final is None and consumed < total:
            print(f"    !! поток не дочитан: осталось {total-consumed} слов")
        mem = build_mem(recs)
        for bank, d in mem.items():
            allmem.setdefault(k, {}).setdefault(bank, {}).update(d)
        # сохраняем список записей
        with open(f"{OUT}/sec{k}_records.txt", "w") as f:
            for cmd, addr, cnt, dat in recs:
                name = {0: 'P', 1: 'X', 2: 'Y'}.get(cmd, f'cmd{cmd}')
                f.write(f"{name} {addr:#08x} {cnt:>6} слов\n")
        print(f"    banks: {[(b, len(d)) for b, d in allmem[k].items()]}")
    return streams, allmem

if __name__ == "__main__":
    main()
