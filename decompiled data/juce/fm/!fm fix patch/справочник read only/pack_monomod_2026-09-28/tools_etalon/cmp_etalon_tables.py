#!/usr/bin/env python3
# Сверка таблиц нашего порта с эталоном Monomodule (оригинальная прошивка OS 1.32B).
import struct, sys, math

REF = "/home/z/my-project/work/monomod_ref"
PACK = "/home/z/my-project/work/pack_final/1_NEW_code_FM_2026-09-28/dsp/mnm"

def load_words(p):
    return [int(l, 16) for l in open(p).read().split()]

# --- 1. Синус-таблица Y:$14A000 ---
etalon_sine = load_words(f"{REF}/etalon_sine_14A000.txt")

# наша таблица из MnmFmSineTable.h: массив hex-слов
import re
src = open(f"{PACK}/MnmFmSineTable.h").read()
our = [int(x, 16) for x in re.findall(r"0x([0-9A-Fa-f]{6})", src)]
print(f"СИНУС-ТАБЛИЦА: эталон {len(etalon_sine)} слов, наш {len(our)} слов")
if len(our) >= len(etalon_sine):
    mism = [(i, a, b) for i, (a, b) in enumerate(zip(etalon_sine, our)) if a != b]
    print(f"  сравнение по {len(etalon_sine)} словам: расхождений {len(mism)}")
    if mism[:3]: print("  первые:", mism[:3])
else:
    mism = [(i, a, b) for i, (a, b) in enumerate(zip(our, etalon_sine)) if a != b]
    print(f"  сравнение по {len(our)} словам: расхождений {len(mism)}")
print(f"  эталон[0..3] = {[hex(x) for x in etalon_sine[:4]]}")
print(f"  наш   [0..3] = {[hex(x) for x in our[:4]]}")

# --- 2. Таблица отношений Y:$141A80 ---
etalon_ratio = load_words(f"{REF}/etalon_ratio_141A80.txt")
print(f"\nRATIO Y:$141A80 эталон (32 слова): {[hex(w) for w in etalon_ratio]}")
# наш kFmRatioExact: 24 float = raw/0x80000
m = re.search(r"kFmRatioExact[^{]*\{(.*?)\}", open(f"{PACK}/MnmFm.hpp").read(), re.S)
our_ratio = [float(x) for x in re.findall(r"([0-9.]+)f", m.group(1))]
ok, bad = 0, []
for i, fr in enumerate(our_ratio):
    raw = int(round(fr * 0x80000))
    if raw == etalon_ratio[i]: ok += 1
    else: bad.append((i, fr, hex(raw), hex(etalon_ratio[i])))
print(f"  наш kFmRatioExact({len(our_ratio)}) против эталона: совпало {ok}, не сошлось {len(bad)}")
for b in bad[:5]: print("   ", b)

# --- 3. Питч-LUT: проверка закона слово_питча -> частота (машина читает A, A/2 Гц при 44.1к) ---
lut = load_words(f"{REF}/etalon_pitchlut_140000.txt")
nz = [(i, w) for i, w in enumerate(lut) if w]
print(f"\nPITCH-LUT X:$140000: {len(lut)} слов, ненулевых {len(nz)}")
if nz:
    print(f"  первые ненулевые: {[(hex(i), hex(w)) for i, w in nz[:8]]}")
    # Ожидание: запись i -> 2*440*2**((i-центр)/12)*44100/44100*2? смотрим структуру
    for i, w in nz[:8]:
        f_hz = w / 2.0  # гипотеза A -> A/2 Гц
        print(f"   X:$1400{i:03x} = {w} ({hex(w)})  ~ {f_hz:.2f} Гц при /2")
