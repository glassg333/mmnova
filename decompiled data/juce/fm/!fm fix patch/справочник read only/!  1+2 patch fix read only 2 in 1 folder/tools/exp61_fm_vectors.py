#!/usr/bin/env python3
"""exp61_fm_vectors.py — bit-exact vector capture for FM machines m9 (PAR) and
m10 (DYN) from the OS 1.32 firmware via the validated DSP56300 emulator.

Canonical sine table: round(sin(2*pi*i/8192) * 8388607) — the SAME formula the
validated m1/m2/m3 harnesses and delivered MnmGroundSin.hpp use (fm_harness.py
injected 8388608; re-injected here to stay consistent with the family).

Captured per block: 32 output words (signed) + persistent state
  y:(r6+$00..$3F) page, X:$5/Y:$5 (carrier inc48), X:$1E..$3F (rot-2 window)
Written to research/fm_par_vectors.txt and research/fm_dyn_vectors.txt.
"""
import sys, os

sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack/22_fm_machines")
import fm_harness as fh  # noqa
import math

M24 = 0xFFFFFF
R6 = fh.R6M  # 0x428
OUT_DIR = "/home/z/my-project/work/stage6/JUCE/Monomachine-Nova-1.7.11/research"


def build_canonical():
    e = fh.build()
    for i in range(8192):
        v = int(round(math.sin(2 * math.pi * i / 8192) * 8388607)) & M24
        e.X[0x14A000 + i] = v
        e.Y[0x14A000 + i] = v
    return e


def s24(v):
    return v - (1 << 24) if v & 0x800000 else v


def snapshot(e):
    """persistent state words: y page, X5/Y5, X:$1E-$3F"""
    st = []
    for off in range(0x40):
        st.append(e.Y.get(R6 + off, 0))
    st.append(e.X.get(5, 0))
    st.append(e.Y.get(5, 0))
    for a in range(0x1E, 0x40):
        st.append(e.X.get(a, 0))
    return st


def capture(mach, sets, blocks, path, name):
    lines = []
    lines.append("# %s bit-exact vectors (OS 1.32, dsp_emu.py)" % name)
    lines.append("# format: SET <i> A=<pitch>")
    lines.append("#         P <8 knob words 0..127>")
    lines.append("#         B <32 signed output words>")
    lines.append("#         S <state words: y-page(64) X5 Y5 X1E-3F(34)>")
    e = build_canonical()
    for si, (p, A) in enumerate(sets):
        e2 = build_canonical()
        fh.fm_init(e2, mach)
        lines.append("SET %d A=%d" % (si, A))
        lines.append("P " + " ".join(str(v) for v in p))
        for blk in range(blocks):
            out = fh.fm_block(e2, mach, p, A)
            st = snapshot(e2)
            lines.append("B " + " ".join(str(s24(v & M24)) for v in out))
            lines.append("S " + " ".join(str(v) for v in st))
        # sanity: same machine code path must be deterministic across instances
        lines.append("ENDSET")
    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")
    n = sum(1 for l in lines if l.startswith("B "))
    print("%s: %d sets x %d blocks -> %d blocks, %d words  (%s)" %
          (name, len(sets), blocks, n, n * 32, path))


PAR_SETS = [
    ([64, 64, 64, 64, 64, 64, 64, 64], 11776),
    ([0, 0, 0, 0, 0, 0, 0, 0],          8192),
    ([127, 127, 127, 127, 127, 127, 127, 127], 4096),
    ([11, 64, 21, 64, 32, 64, 64, 64],  16000),
    ([43, 64, 53, 64, 64, 64, 64, 64],   2048),
    ([75, 64, 85, 64, 96, 64, 64, 64],  24000),
    ([107, 64, 117, 64, 5, 64, 64, 64],  6400),
    ([64, 63, 64, 65, 64, 66, 64, 64],  12288),
    ([64, 80, 64, 90, 64, 127, 64, 64],  9102),
    ([64, 64, 64, 64, 64, 64, 0, 64],   11776),
    ([64, 64, 64, 64, 64, 64, 127, 64], 11776),
    ([30, 110, 40, 120, 50, 100, 90, 30], 3300),
    ([90, 90, 90, 90, 90, 90, 45, 90],  15000),
    ([64, 20, 100, 45, 10, 75, 110, 64],  512),
]

DYN_SETS = [
    ([64, 64, 64, 64, 64, 64, 64, 64], 11776),
    ([0, 0, 0, 0, 0, 0, 0, 0],          8192),
    ([127, 127, 127, 127, 127, 127, 127, 127], 4096),
    ([127, 64, 64, 64, 64, 64, 64, 64], 16000),   # 1FRQ clamp path
    ([126, 64, 64, 64, 0, 64, 64, 64],   2048),   # 1FRQ just below clamp
    ([64, 64, 64, 64, 127, 64, 64, 64], 24000),   # 2FRQ max (quadratic)
    ([64, 64, 64, 64, 0, 64, 64, 64],    6400),   # 2FRQ zero
    ([64, 0, 64, 64, 64, 64, 64, 64],   12288),   # 1FEN min
    ([64, 127, 64, 64, 64, 64, 64, 64],  9102),   # 1FEN max
    ([64, 64, 64, 0, 64, 64, 64, 64],   11776),   # 1VEN 0
    ([64, 64, 64, 63, 64, 64, 64, 64],  11776),   # 1VEN just below 64
    ([64, 64, 63, 64, 64, 64, 64, 64],   3300),   # 1VOL gate off
    ([64, 64, 65, 127, 64, 64, 64, 64], 15000),   # 1VOL gate on
    ([64, 64, 64, 64, 64, 127, 127, 64],  512),   # 2ENV+2FB max
    ([64, 64, 64, 64, 64, 63, 63, 64],  11776),   # 2ENV/2FB just below gate
    ([30, 100, 110, 20, 90, 40, 70, 127], 640),
]

if __name__ == "__main__":
    os.makedirs(OUT_DIR, exist_ok=True)
    capture(9, PAR_SETS, 16, os.path.join(OUT_DIR, "fm_par_vectors.txt"), "FM-PAR m9")
    capture(10, DYN_SETS, 16, os.path.join(OUT_DIR, "fm_dyn_vectors.txt"), "FM-DYN m10")
