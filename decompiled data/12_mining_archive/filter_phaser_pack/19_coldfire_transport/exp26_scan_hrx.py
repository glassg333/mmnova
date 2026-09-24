#!/usr/bin/env python3
"""exp26_scan_hrx.py — скан dsp1_pmem.bin на все обращения к HI08 HRX/HTX ($FFFFC6/$FFFFC7).
Находит second-stage loader (парсер команд хоста) и все места приёма данных хоста."""
import struct

PM = "/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin"
data = open(PM, "rb").read()
n = len(data) // 3
words = [0] * n
for i in range(n):
    o = i * 3
    words[i] = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]

print("total words:", n)

hits = []
for i in range(n - 1):
    w = words[i]
    # movep X:<<$FFC6,D  : $08x006 (b0=0x08, addr low6=6)
    if (w & 0xFF0000) == 0x080000 and (w & 0x3F) == 0x06 and (w & 0x00C0) == 0:
        hits.append((i, "movep x:<<$FFC6 <- HRX read", hex(w)))
    # 2-word absolute form: 44F000/60F000/56F000 + FFFFC6
    if w in (0x44F000, 0x60F000, 0x56F000, 0x4CF000, 0x5CF000) and i + 1 < n and words[i+1] == 0xFFFFC6:
        hits.append((i, "move x:<<$FFFFC6 (abs form)", hex(w) + "+FFFFC6"))
    # HTX writes $FFC7: movep D,x:<<$FFC7
    if (w & 0xFF0000) == 0x080000 and (w & 0x3F) == 0x07 and (w & 0x00C0) == 0:
        hits.append((i, "movep -> x:<<$FFC7 (HTX write)", hex(w)))
    # HSR $FFC5/$FFC3 brclr polling
    if w in (0x0CC300, 0x0CC500):
        hits.append((i, "brclr #$0,x:<<$FFC3 (HSR poll)", hex(w)))

print("HRX/HTX hits:", len(hits))
for a, t, w in hits:
    print(f"P:{a:06X}  {t}   [{w}]")
