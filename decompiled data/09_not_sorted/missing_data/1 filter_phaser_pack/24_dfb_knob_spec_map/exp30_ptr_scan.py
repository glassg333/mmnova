#!/usr/bin/env python3
"""exp30: скан 32-битных указателей на сеттеры/функции спецификаций в ColdFire ROM.

Цель: найти dispatch-таблицы, ссылающиеся на $2427B4/$242968 (сеттеры троек
спецификаций $7001FA/$700242/$70028A) и на обработчики $242058/$2403D4/$242044.
"""
import struct
import sys

ROM = "/home/z/my-project/mining/mmnova/decompiled data/05_descriptors/coldfire_main.bin"
TARGETS = {
    0x2427B4: "setter_spec_triples_A (writes $7001FA/$700242/$70028A + mirrors)",
    0x242968: "setter_spec_triples_B (same arrays, branch of A?)",
    0x242058: "handler_decode_specs",
    0x2403D4: "router_group_to_cell_addr",
    0x242044: "helper",
    0x242BD0: "getter_runtime_state",
    0x23F8BA: "sub_23F8BA (called with d1=idx)",
    0x267A12: "glide_engine",
    0x267ABA: "tempo_engine",
    0x267DEE: "page_builder",
}
# ROM base address in ColdFire address space: exp26 established the mapping.
# The full disasm showed file offsets == addresses (e.g. line '2427b4').
d = open(ROM, "rb").read()
print(f"ROM size: {len(d)} bytes = ${len(d):X}")

# The disasm addresses go up to what? check max: file is 425858 = $67F82 bytes.
# Addresses in disasm like $2427B4 are ROM addresses; ROM base likely $200000
# (prolog $200006 was mentioned in worklog). So file offset = addr - $200000.
BASE = 0x200000
print(f"Assumed ROM base: ${BASE:X} (file covers ${BASE:X}..${BASE + len(d):X})")

for tgt, desc in sorted(TARGETS.items()):
    pat = struct.pack(">I", tgt)  # big-endian ColdFire
    hits = []
    off = 0
    while True:
        i = d.find(pat, off)
        if i < 0:
            break
        hits.append(i + BASE)
        off = i + 1
    if hits:
        print(f"\n${tgt:X} ({desc}):")
        for h in hits:
            print(f"  ptr at ROM ${h:X} (file off ${h - BASE:X})")
    else:
        print(f"\n${tgt:X} ({desc}): NO pointer refs")
