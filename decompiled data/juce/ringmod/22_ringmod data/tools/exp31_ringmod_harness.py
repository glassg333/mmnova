#!/usr/bin/env python3
"""exp31_ringmod_harness.py — run machine 17 (FX-RINGMOD, proc $1478C7) in the
bit-exact DSP56300 emulator. Conventions from kernel $000087-$0002EB:
  r6 = $528 (FX slot-0 page), r7 = $100 (output bank Y:$100..), r0 = machine index.
Machines: init $14789E, conf $1478C6, proc $1478C7..$147B37 (next init m32 $147B38).
"""
import os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from run_kernel import build_emu, disasm  # noqa

INIT = 0x14789E
INIT_END = 0x1478C6
PROC = 0x1478C7
PROC_END = 0x147B38
R6 = 0x528
M24 = 0xFFFFFF


def _find_repo_root():
    here = os.path.dirname(os.path.abspath(__file__))
    p = here
    for _ in range(8):
        if os.path.isdir(os.path.join(p, "decompiled data", "02_memory_images")):
            return p
        p = os.path.dirname(p)
    return here

SINE_CANDIDATES = [
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tables",
                 "X_14A000_sine8k_full.bin"),
    os.path.join(_find_repo_root(), "decompiled data", "12_mining_archive",
                 "03_lfo", "X_14A000_sine8k_full.bin"),
]
SINE_BIN = next(p for p in SINE_CANDIDATES if os.path.isfile(p))


def build():
    e = build_emu()
    lines = disasm(0x144000, 0x4000)
    e.parse(lines)
    # sine 8K table is generated at DSP boot, not present in the static dump:
    # load the extracted full table (03_lfo/X_14A000_sine8k_full.bin)
    data = open(SINE_BIN, "rb").read()
    for i in range(len(data) // 3):
        o = i * 3
        e.X[0x14A000 + i] = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
    return e


def q(v):
    return (int(v) & 0xFFFF) << 16


def set_params(e, p):
    """p = [WAVE, EXT, TUNE, MIX, p4, p5, p6, INP] as 0..127"""
    for i in range(8):
        e.Y[R6 + 0x04 + i] = q(p[i])


def run_init(e):
    for a in range(0x4F0, 0x600):
        e.X[a] = 0
        e.Y[a] = 0
    e.R[6] = R6
    e.R[7] = 0x100
    e.ret_stack.append(INIT_END)
    e.run(INIT, end=INIT_END, max_steps=100000)


def run_block(e, sig, p, a_entry=0, r0_entry=17):
    """sig: dict addr->value or list (word0..n at X:0x200). Returns 32 out words."""
    set_params(e, p)
    if isinstance(sig, list):
        for i, v in enumerate(sig):
            e.X[0x200 + i] = v & M24
    else:
        for a, v in sig.items():
            e.X[a] = v & M24
    e.R[6] = R6
    e.R[7] = 0x100
    e.R[0] = r0_entry
    e.A = a_entry & 0xFFFFFFFFFFFF
    e.ret_stack.append(PROC_END)
    e.run(PROC, end=PROC_END, max_steps=2000000)
    return [e.Y.get(0x100 + i, 0) for i in range(32)]


def f(v):
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0


def i2f(v):
    return v / 8388608.0


def dump_page(e, lo=-0x30, hi=0x60):
    out = []
    for off in range(lo, hi):
        x = e.X.get(R6 + off, 0)
        y = e.Y.get(R6 + off, 0)
        if x or y:
            out.append((off, x, y))
    return out


if __name__ == "__main__":
    print("=== E1: INIT page dump (all params 0) ===")
    e = build()
    run_init(e)
    cells = dump_page(e)
    for off, x, y in cells:
        print("  r6%+#05x  X=%06X Y=%06X" % (off, x, y))

    print("\n=== E2: zero input, zero params, A=0 -> out32 ===")
    out = run_block(e, [0] * 34, [0, 0, 60, 127, 0, 0, 0, 64])
    print("  " + " ".join("%+011.6f" % f(v) for v in out))

    print("\n=== E3: input detection — inject DC $400000 at candidate buffers ===")
    cands = {
        "X:200 (phaser conv)": list(range(0x200, 0x222)),
        "X:140": list(range(0x140, 0x150)),
        "X:1A0": list(range(0x1A0, 0x1B0)),
        "X:260": list(range(0x260, 0x270)),
        "X:010-040": list(range(0x010, 0x040)),
    }
    for name, rng in cands.items():
        e2 = build()
        run_init(e2)
        sig = {}
        for a in rng:
            sig[a] = 0x400000
        out = run_block(e2, sig, [0, 0, 60, 127, 0, 0, 0, 64])
        s = sum(abs(v) for v in out)
        print("  %-22s sum|out| = %12d" % (name, s))
