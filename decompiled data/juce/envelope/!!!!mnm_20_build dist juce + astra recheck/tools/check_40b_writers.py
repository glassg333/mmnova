#!/usr/bin/env python3
"""check_40b_writers.py — кто пишет X:$40B (X:page+$0B)?
1) Дизасм INIT-зоны машины P:$00E0-$0100 (тезис Task 56: $00F1 пишет $20).
2) Полный скан ядра P:$0000-$0B4D на записи в X:(r6+$0B) при r6=$400
   (move a,x:(r6+$0b) / move #..,x:(r6+$0b) и т.п.), плюс машинные регионы.
3) Дизасм всей зоны $079F-$07A6 — единственный читатель?
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm, PM

def opnd_writes_x40b(txt):
    # эвристика: инструкция-назначение пишет в x:(r6+$b) / x:$40b
    t = txt.lower()
    return (",x:(r6+$b)" in t) or (",x:$40b" in t) or (",x:(r6+$0b)" in t)

def reads_x40b(txt):
    t = txt.lower()
    return ("x:(r6+$b)," in t) or ("x:$40b," in t) or ("x:(r6+$0b)," in t)

e = build_emu()

print("== INIT zone P:$00E8-$00FA (кто пишет $20/1 в X-страницу) ==")
for ln in disasm(0x00E0, 0x22):
    print(ln)

print()
print("== Ядро P:$0000-$0B4E: читатели/писатели X:(r6+$0B) ==")
for ln in disasm(0x0000, 0x0B4E):
    if opnd_writes_x40b(ln) or reads_x40b(ln):
        tag = "WRITE" if opnd_writes_x40b(ln) else "READ "
        print(tag, ln)

print()
print("== Машины P:$144000-$146000: читатели/писатели X:(r6+$0B)/X:$40B ==")
for ln in disasm(0x144000, 0x2000):
    if opnd_writes_x40b(ln) or reads_x40b(ln):
        tag = "WRITE" if opnd_writes_x40b(ln) else "READ "
        print(tag, ln)

print()
print("== Полный блок $079F-$07A6 (контекст чтения) ==")
for ln in disasm(0x079F, 0x8):
    print(ln)
