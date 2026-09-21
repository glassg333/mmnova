#!/usr/bin/env python3
"""disasm_regions.py — чтения параметров страницы в EQ/DIST-регионах ядра."""
import sys, re
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import disasm

PAT = re.compile(r"r6\+\$([0-9a-fA-F]{1,2})\b")

def scan(lo, hi, label):
    lines = disasm(lo, hi)
    print("=== %s ($%04X-$%04X) ===" % (label, lo, hi))
    for l in lines:
        m = PAT.search(l)
        if m:
            print("  %s   (page $%s)" % (l.strip(), m.group(1)))
    print()

if __name__ == "__main__":
    scan(0x06C2, 0x078A, "EQ-prep / хвост фильтра")
    scan(0x0789, 0x085A, "EQ/DIST-секция")
