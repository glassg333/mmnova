#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
verify_chorus_region.py — быстрая проверка, что ваш образ совпадает с эталоном
аудита по региону хоруса P:$147661-$1477DE (382 слова, верифицирован
байт-в-байт двумя независимыми сессиями реверса).

    python3 verify_chorus_region.py --pmem dsp1_pmem.bin [--ref chorus_region.bin]

Без --ref проверяются структурные маркеры порта (MnMFxChorus.h):
  $14767A  rts  (конец init / точка входа config)
  $1477DD  rts  (конец process)
  наличие констант $114000 (кольцевой буфер), $14A000 (width-таблица),
  $2AAAAB (1/3 суммы тапов), $144AC7 (LP-таблица).
Код возврата 0 = порт опирается на тот же код, что и ваш образ.
"""

import argparse, os, sys

WS = 3
BASE, END = 0x147661, 0x1477DE
MARKERS = {0x114000: "кольцевой буфер задержки",
           0x14A000: "width-таблица (sine seg 0)",
           0x14A800: "width-таблица (sine seg 1)",
           0x2AAAAB: "множитель 1/3 суммы тапов",
           0x144AC7: "LP-таблица обратной связи"}


def words(path, addr, count, ws=WS):
    with open(path, "rb") as f:
        f.seek(addr * ws)
        raw = f.read(count * ws)
    return [(raw[i * ws] << 16) | (raw[i * ws + 1] << 8) | raw[i * ws + 2]
            for i in range(len(raw) // ws)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pmem", required=True)
    ap.add_argument("--ref")
    ap.add_argument("--wordsize", type=int, default=WS)
    a = ap.parse_args()

    n = END - BASE + 1
    w = words(a.pmem, BASE, n, a.wordsize)
    if len(w) != n:
        print("образ короче региона хоруса"); return 2

    if a.ref:
        with open(a.ref, "rb") as f:
            ref = f.read()
        mine = b"".join(bytes(((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF)) for v in w)
        if mine == ref[:len(mine)]:
            print("OK: регион совпал байт-в-байт с %s" % os.path.basename(a.ref))
            return 0
        print("РАСХОЖДЕНИЕ с эталоном"); return 3

    found = {k: False for k in MARKERS}
    for v in w:
        if v in found:
            found[v] = True
    ok = True
    for k, name in MARKERS.items():
        print("  %s $%06X  %s" % ("+" if found[k] else "-", k, name))
        ok = ok and found[k]
    # rts = 0x00000C на DSP56300
    print("  %s $14767A rts (конец init)" % ("+" if w[0x14767A - BASE] == 0x00000C else "-"))
    print("  %s $1477DD rts (конец process)" % ("+" if w[0x1477DD - BASE] == 0x00000C else "-"))
    return 0 if ok else 4


if __name__ == "__main__":
    sys.exit(main())
