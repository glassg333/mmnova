#!/usr/bin/env python3
# Дизассемблер DSP56300 v3 — полный, для прошивки Monomachine OS 1.32b.
import re

REG5 = ["?0","?1","?2","?3","x0","x1","y0","y1","a0","b0","a2","b2","a1","b1","a","b",
        "r0","r1","r2","r3","r4","r5","r6","r7",
        "n0","n1","n2","n3","n4","n5","n6","n7"]
REG4 = {4:"x0",5:"x1",6:"y0",7:"y1",8:"a0",9:"b0",10:"a2",11:"b2",12:"a1",13:"b1",14:"a",15:"b"}
# 6-битная таблица регистров memory-move (выявлена эмпирически)
REG6 = {0b000100:"x0",0b000101:"x1",0b000110:"y0",0b000111:"y1",
        0b001000:"a10",0b001001:"b10",0b001010:"a2",0b001011:"b2",
        0b001100:"a1",0b001101:"b1",0b001110:"a",0b001111:"b",
        0b010000:"a10",0b010001:"a1",0b010010:"a0",0b010011:"?43",
        0b010100:"b10",0b010101:"b1",0b010110:"b0",0b010111:"?47",
        0b011100:"a10",0b011101:"a1",0b011110:"a0",0b011111:"?4f",
        0b100100:"b10",0b100101:"b1",0b100110:"b0",0b100111:"?97",
        0b110100:"b10",0b110101:"b1",0b110110:"b0",
        0b111100:"b10",0b111101:"b1",0b111110:"b0"}

CC = {0:"sp",1:"ge",2:"ne",3:"pl",4:"nn",5:"ec",6:"lc",7:"gt",
      8:"cs",9:"lt",10:"eq",11:"mi",12:"nr",13:"rc",14:"le",15:"lc"}

CTRL = {0:"??",1:"??",2:"pc",3:"sr",4:"omr",5:"sp",6:"ssh",7:"ssl",8:"la",9:"lc",10:"?a",11:"?b",
        12:"x",13:"y",14:"?c",15:"?d",16:"vba",17:"?f",18:"bcR",19:"?11"}

def sx(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v

def ea_txt(ea):
    """EA-байт (bits 15:8) для X/Y-memory move."""
    rn = ea & 7
    if ea < 0x40:
        return f"${ea:02x}"
    if 0xC0 <= ea <= 0xC7: return f"(r{rn}-n{rn})"
    if 0xC8 <= ea <= 0xCF: return f"(r{rn}+n{rn})"
    if 0xD0 <= ea <= 0xD7: return f"(r{rn}-)"
    if 0xD8 <= ea <= 0xDF: return f"(r{rn}+)"
    if 0xE0 <= ea <= 0xE7: return f"(r{rn})"
    if 0xE8 <= ea <= 0xEF: return f"-(r{rn})"
    if 0x50 <= ea <= 0x57: return f"(r{rn}+)"
    if 0x58 <= ea <= 0x5F: return f"(r{rn}+)"
    if 0x40 <= ea <= 0x47: return f"(r{rn}+)"
    if 0x48 <= ea <= 0x4F: return f"(r{rn}-)"
    if 0x60 <= ea <= 0x67: return f"(r{rn}-)"
    if 0x68 <= ea <= 0x6F: return f"(r{rn}-)"
    if 0x70 <= ea <= 0x77: return f"(r{rn}+n{rn})"
    if 0x78 <= ea <= 0x7F: return f"(r{rn}-n{rn})"
    if 0x80 <= ea <= 0x87: return f"(r{rn}+n{rn})"
    if 0x88 <= ea <= 0x8F: return f"(r{rn}-n{rn})"
    if 0x90 <= ea <= 0x97: return f"(r{rn})"
    if 0x98 <= ea <= 0x9F: return f"(r{rn}+)"
    if 0xA0 <= ea <= 0xA7: return f"(r{rn}-)"
    return f"??{ea:02x}"

class Dis:
    def __init__(self, mem=None, labels=None):
        self.mem = mem or {}
        self.labels = labels or {}

    def L(self, a):
        return self.labels.get(a, f"_{a:06x}")

    # ---------- параллельные переносы ----------

    def memmove_str(self, reg6, ea, space):
        return f"{REG6.get(reg6, f'?{reg6}')},{space}:{ea_txt(ea)}"

    def decode(self, pc):
        w = self.mem.get(pc)
        if w is None:
            return ("dc", 1)
        if w == 0x000000: return ("nop", 1)
        if w == 0x000004: return ("rti", 1)
        if w == 0x00000c: return ("rts", 1)
        top = w >> 16

        # ============ 0000 001: короткое смещение (Rn+disp7) ============
        if (w >> 17) == 1:
            disp = sx((((w >> 11) & 0x3F) << 1) | ((w >> 6) & 1), 7)
            rn = (w >> 8) & 7
            sp = "y" if (w >> 5) & 1 else "x"
            wr = (w >> 4) & 1
            reg = REG4[w & 0xF]
            mem = f"{sp}:(r{rn}{disp:+#x})" if disp else f"{sp}:(r{rn})"
            return (f"move {reg},{mem}", 1) if not wr else (f"move {mem},{reg}", 1)

        # ============ 0010 0...: register-file move / imm ============
        if (w >> 21) == 0b001:
            if not (w >> 18) & 1:  # reg-reg: bits 23:18=001000
                src = REG5[(w >> 13) & 0x1F]
                dst = REG5[(w >> 8) & 0x1F]
                return (f"move {src},{dst}", 1)
            else:  # imm8: dst bits 20:16, imm bits 15:8
                dst = REG5[(w >> 16) & 0x1F]
                imm = (w >> 8) & 0xFF
                return (f"move #${imm:02x},{dst}", 1)

        return (f"??{w:06x}", 1)
