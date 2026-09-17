#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
parse_descriptors.py — разобрать блоки дескрипторов ColdFire правильно.

Цель: найти байты кривых. ROUTING_RU.md §6 говорит, что скейлинг ручек
лежит "в raw_tail дескрипторов, формат не расшифрован". То есть это
ДАННЫЕ, а не код — дизассемблер ColdFire не нужен.

Формат блока (шаг 0xB0 = 176 байт):
  +0x00  short name
  +0x09  display name
  +0x12  параметры 8 x 6 байт   <- 4 байта имя + 2 байта ???
  +0x42  defaults 8 байт
  +0x4A  raw_tail 102 байта     <- не расшифрован
  +0xB0  конец

Проверка формата: у SID параметры PWRS/WAVE/MOD/MSRC должны дать
тип 02/05/04/02 — это уже видел автор первого порта.

Использование:
  python parse_descriptors.py --bin "decompiled data/05_descriptors/coldfire_main.bin"
  python parse_descriptors.py --bin coldfire_main.bin --find HPQ
"""
import argparse, sys, re


def clean(b):
    return "".join(chr(c) if 32 <= c < 127 else "." for c in b)


def dump_block(d, off, idx):
    b = d[off:off + 0xB0]
    if len(b) < 0xB0:
        return False
    print("--- блок %d  @0x%05x" % (idx, off))
    print("  short   : %s" % clean(b[0x00:0x09]))
    print("  display : %s" % clean(b[0x09:0x12]))
    for p in range(8):
        q = b[0x12 + p * 6: 0x12 + (p + 1) * 6]
        print("  p%d: имя=%-4s  хвост=%02x %02x" % (p, clean(q[0:4]).strip("."), q[4], q[5]))
    print("  defaults: %s" % " ".join("%02x" % x for x in b[0x42:0x4A]))
    tail = b[0x4A:0xB0]
    for i in range(0, len(tail), 16):
        print("  tail+%02x : %s" % (i, " ".join("%02x" % x for x in tail[i:i + 16])))
    print()
    return True


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bin", required=True, help="coldfire_main.bin")
    ap.add_argument("--base", default="0x57FC5")
    ap.add_argument("--count", type=int, default=26)
    ap.add_argument("--find", help="искать строку и показать блок вокруг неё")
    a = ap.parse_args()

    d = open(a.bin, "rb").read()
    print("образ: %d байт" % len(d))

    if a.find:
        # Ищем строку параметра (например HPQ или WOFS) — так находится
        # блок дескрипторов ОБЩИХ СТРАНИЦ (AMP/FILT/EFFX/LFO), в котором
        # и лежат кривые HPQ/LPQ/BOFS/WOFS.
        pat = a.find.encode()
        hits = [m.start() for m in re.finditer(re.escape(pat), d)]
        print("найдено вхождений '%s': %d" % (a.find, len(hits)))
        for h in hits[:20]:
            print("\n  @0x%05x  контекст:" % h)
            lo = max(0, h - 64)
            chunk = d[lo:h + 96]
            for i in range(0, len(chunk), 16):
                print("    %05x  %-47s  %s" % (
                    lo + i,
                    " ".join("%02x" % x for x in chunk[i:i + 16]),
                    clean(chunk[i:i + 16])))
        return 0

    base = int(a.base, 16)
    for i in range(a.count):
        if not dump_block(d, base + i * 0xB0, i):
            break
    return 0


if __name__ == "__main__":
    sys.exit(main())
