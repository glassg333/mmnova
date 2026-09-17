#!/usr/bin/env python3
# Дизассемблирование ColdFire-декомпрессора прошивки Monomachine (BIN 0x1C4)
import struct
from capstone import *

BIN = "/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin"
data = open(BIN, "rb").read()

md = Cs(CS_ARCH_M68K, CS_MODE_BIG_ENDIAN | CS_MODE_M68K_040)
md.detail = False

start, end = 0x752, 0x8C8   # загрузчик DSP (func_752/func_7ce)
code = data[start:end]
for ins in md.disasm(code, start):
    print(f"{ins.address:06x}: {ins.bytes.hex():<14s} {ins.mnemonic} {ins.op_str}")
