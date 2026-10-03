#!/usr/bin/env python3
"""exp70d — дизасм ColdFire-билдера страницы трека ($042590-$04272C):
что именно пишется в слова 24-33 (EQF/EQG/SRR/DTIM/DSND/DFB/DBAS/DWID/$420/$421)."""
from capstone import Cs, CS_ARCH_M68K, CS_MODE_BIG_ENDIAN, CS_MODE_M68K_040

BIN = "/home/z/my-project/work/mmnova/decompiled data/05_descriptors/coldfire_main.bin"
data = open(BIN, "rb").read()
LO, HI = 0x42590, 0x42730

md = Cs(CS_ARCH_M68K, CS_MODE_BIG_ENDIAN | CS_MODE_M68K_040)
md.skipdata = True
BASE = 0
code = data[LO - BASE:HI - BASE]
for ins in md.disasm(code, LO):
    print("%06X: %-24s %s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
