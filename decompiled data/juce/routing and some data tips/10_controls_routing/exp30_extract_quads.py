#!/usr/bin/env python3
"""exp30: извлечение полных квадратурных таблиц фейзера m18 ($14A000 sin / $14A800 cos).

Источники (все три бит-идентичны, проверено exp30):
  decompiled data/09_not_sorted/missing_data/recovered/X_14A000_sine_table_8K.bin
  decompiled data/09_not_sorted/missing_data/final_recovery/X_14A000_sine_table_8K.bin
  decompiled data/09_not_sorted/missing_data/4 iteracy/X_14A000_sine_table_8K_math.bin
  (+ три копии X_14A800_width_table_chorus_B.bin)

Верификация:
  1) head[0..7] совпадает с P_14A000_sine8k_head.bin из пака (итерация 1)
  2) таблица == math.sin/cos(i/8192*2pi) с err < 1 LSB/2
  3) recovered == final_recovery == math (побитно)

Выход: pack/22_phaser_lfo/tables_14A000_14A800/
  X_14A000_sine8k_full.bin/.txt, X_14A800_cos8k_full.bin/.txt, exp30_quad_verify.json
"""
import json
import math
import os
import struct

MISSING = "/home/z/my-project/mining/mmnova/decompiled data/09_not_sorted/missing_data"
PACK22 = "/home/z/my-project/mining/pack_work/filter_phaser_pack/22_phaser_lfo"
OUT = os.path.join(PACK22, "tables_14A000_14A800")
os.makedirs(OUT, exist_ok=True)

SRC = {
    "sine": os.path.join(MISSING, "recovered", "X_14A000_sine_table_8K.bin"),
    "cos": os.path.join(MISSING, "recovered", "X_14A800_width_table_chorus_B.bin"),
}
ALT = {
    "sine": [os.path.join(MISSING, "final_recovery", "X_14A000_sine_table_8K.bin"),
             os.path.join(MISSING, "4 iteracy", "X_14A000_sine_table_8K_math.bin")],
    "cos": [os.path.join(MISSING, "final_recovery", "X_14A800_width_table_chorus_B.bin"),
            os.path.join(MISSING, "4 iteracy", "X_14A800_width_table_chorus_B_math.bin")],
}


def load_words(path):
    d = open(path, "rb").read()
    assert len(d) == 24576, f"{path}: {len(d)} bytes != 24576"
    return [int.from_bytes(d[i:i + 3], "big", signed=True) for i in range(0, len(d), 3)]


def hex24(w):
    return f"{w & 0xFFFFFF:06X}"


result = {"tables": {}, "bit_identical_alt_sources": {}, "head_match_pack": None,
          "math_max_err": {}, "notes": []}

for name, path in SRC.items():
    w = load_words(path)
    # alt sources bit-compare
    alts = [load_words(p) == w for p in ALT[name]]
    result["bit_identical_alt_sources"][name] = alts

    base = "X_14A000_sine8k_full" if name == "sine" else "X_14A800_cos8k_full"
    # bin = raw copy
    with open(os.path.join(OUT, base + ".bin"), "wb") as f:
        f.write(open(path, "rb").read())
    # txt dump
    with open(os.path.join(OUT, base + ".txt"), "w") as f:
        addr = "X:$14A000" if name == "sine" else "X:$14A800"
        role = ("m18 phaser LFO quadrature SIN (read at $144ED4/$145076: x:(r0+$14a000),a)"
                if name == "sine" else
                "m18 phaser LFO quadrature COS (read at $144ED6/$145078: x:(r0+$14a800),b)")
        f.write(f"# {addr}  {base}  (8192 words)\n")
        f.write(f"# {role}\n")
        f.write("# 3 bytes/word big-endian signed 24-bit fraction = word/2^23\n")
        f.write("#  idx | hex     signed24   fraction(24-bit)\n")
        for i, v in enumerate(w):
            f.write(f"{i:6d} | {hex24(v)} {v:9d}   {v / 2**23:+.7f}\n")

    # math check
    fn = math.sin if name == "sine" else math.cos
    maxerr = 0.0
    for i in range(8192):
        maxerr = max(maxerr, abs(w[i] / 2**23 - fn(i / 8192 * 2 * math.pi)))
    result["math_max_err"][name] = maxerr
    result["tables"][name] = {
        "addr": "X:$14A000" if name == "sine" else "X:$14A800",
        "words": 8192,
        "first8": [hex24(v) for v in w[:8]],
        "extremum": ("max at idx 2048" if name == "sine" else "max at idx 0"),
    }

# head match with pack (iteration-1 head files)
head = open(os.path.join("/home/z/my-project/mining/pack_work/filter_phaser_pack/01_filter/tables",
                         "P_14A000_sine8k_head.bin"), "rb").read()
hw = [int.from_bytes(head[i:i + 3], "big", signed=True) for i in range(0, len(head), 3)]
w = load_words(SRC["sine"])
result["head_match_pack"] = [hw[i] == w[i] for i in range(len(hw))]

with open(os.path.join(OUT, "exp30_quad_verify.json"), "w") as f:
    json.dump(result, f, indent=2)

print(json.dumps(result, indent=2))
