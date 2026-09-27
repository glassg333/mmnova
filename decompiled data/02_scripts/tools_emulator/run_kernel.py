#!/usr/bin/env python3
"""
run_kernel.py — Run the Monomachine DSP1 kernel filter section ($04A8-$0B4D) in the
DSP56300 emulator with controlled voice-page state. Measures bit-exact behavior:
coefficient paths, envelope depth scaling, filter response.
"""
import sys, os, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from dsp_emu import DSP56300, EmuError

DIS = "/home/z/my-project/work/mmnova/decompiled data/07_scripts/DSP56300 disassemblerStandalone/scripts/dis56300.py"
PM = "/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin"
WORK = "/home/z/my-project/mining"

def disasm(start, count):
    import subprocess
    r = subprocess.run([sys.executable, DIS, PM, hex(start), str(count)],
                       capture_output=True, text=True)
    return r.stdout.splitlines()

def build_emu():
    # whole kernel P:$0000-$0B4D
    lines = disasm(0x0000, 0x0B4E)
    e = DSP56300(lines)
    # preload the shared SRAM image (P-memory dump aliases into X/Y above $100000)
    data = open(PM, "rb").read()
    n = len(data) // 3
    for i in range(0x100000, min(n, 0x160000)):
        o = i * 3
        w = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
        e.X[i] = w
        e.Y[i] = w
    return e

Q23 = 8388608.0

def q23(x):
    v = int(round(x * Q23))
    return max(-0x800000, min(0x7FFFFF, v)) & 0xFFFFFF

def setup_voice(e, r6=0x400, base=0.5, wdth=0.25, hpq=0.0, lpq=0.0,
                amp_atk=0, amp_hold=0, amp_dec=0, amp_rel=0,
                filt_atk=0.5, filt_dec=0.5, bofs=0.5, wofs=0.5,
                mode=0, tempo=120, eqf=0.5, trig=1,
                inp=None):
    """inp: list of 34 Y-words (machine output) placed at r6+$DC.."""
    Y = e.Y
    X = e.X
    # zero the voice page
    for i in range(0x200):
        Y[r6 + i] = 0
        X[r6 + i] = 0
    Y[r6 + 0x00] = (amp_atk & 0xFFFF) << 16
    Y[r6 + 0x01] = (amp_hold & 0xFFFF) << 16
    Y[r6 + 0x02] = (amp_dec & 0xFFFF) << 16
    Y[r6 + 0x03] = (amp_rel & 0xFFFF) << 16
    # machine params 0..7 (r6+$04..$0B) — markers
    for i in range(8):
        Y[r6 + 0x04 + i] = q23(0.5)
    Y[r6 + 0x0E] = 0          # pan L
    Y[r6 + 0x0F] = 0          # pan R
    X[r6 + 0x0F] = 1          # stereo flag bit0 = mono (skip 2nd SVF pass)
    X[r6 + 0x0B] = 64         # EQ gain (raw int, div divisor)
    Y[r6 + 0x10] = q23(base)  # FILT BASE
    Y[r6 + 0x11] = q23(wdth)  # FILT WDTH
    Y[r6 + 0x12] = q23(hpq)   # FILT HPQ
    Y[r6 + 0x13] = q23(lpq)   # FILT LPQ
    Y[r6 + 0x14] = q23(filt_atk)  # FILT ATK (env depth)
    Y[r6 + 0x15] = q23(filt_dec)  # FILT DEC
    Y[r6 + 0x16] = q23(bofs)      # FILT BOFS
    Y[r6 + 0x17] = q23(wofs)      # FILT WOFS
    Y[r6 + 0x23] = tempo          # TEMPO
    Y[r6 + 0x25] = mode           # MODE bits
    Y[r6 + 0x28] = trig           # TRIG_EVENT
    Y[r6 + 0x21] = 1              # env init flag (first run)
    # machine output buffer (r6+$DC..$FD): 34 words
    if inp is None:
        inp = [0] * 34
    for i, v in enumerate(inp):
        Y[r6 + 0xDC + i] = v & 0xFFFFFF
    # env state cells (r7-$1/-$2 = r6+$DB/$DA after lua)
    Y[r6 + 0xDB] = 0
    X[r6 + 0xDB] = 0
    Y[r6 + 0xDA] = 0
    X[r6 + 0xDA] = 0
    X[r6 + 0xD9] = 0
    Y[r6 + 0x71] = 0x400000   # filter env state (r7-$8d) initial
    Y[r6 + 0x70] = 0x400000
    # y:$124 = 0 (track counter gate for env block)
    Y[0x124] = 0
    # the filter section runs with r6 = voice page base (normally y:$123 - $28)
    e.R[6] = r6
    e.R[7] = 0
    # zero kernel scratch
    for a in list(range(0x00, 0x100)):
        X[a] = 0
        Y[a] = 0

def run_block(e, start=0x04A8, end=0x0B4C):
    steps = e.run(start, end=end, max_steps=2_000_000)
    return steps

def diff_mem(e, before_x, before_y, lo=0, hi=0x1000):
    dx = {a: e.X[a] for a in set(e.X) | set(before_x)
          if e.X.get(a, 0) != before_x.get(a, 0) and lo <= a < hi}
    dy = {a: e.Y[a] for a in set(e.Y) | set(before_y)
          if e.Y.get(a, 0) != before_y.get(a, 0) and lo <= a < hi}
    return dx, dy

if __name__ == "__main__":
    e = build_emu()
    bx, by = dict(e.X), dict(e.Y)
    setup_voice(e, trig=1)
    n = run_block(e)
    dx, dy = diff_mem(e, bx, by)
    print("block steps:", n)
    print("Y changed (page 0x400+):")
    for a in sorted(dy):
        if 0x400 <= a < 0x500:
            print("  Y:%04X = %06X" % (a, dy[a]))
    print("Y changed (kernel scratch <0x200):")
    for a in sorted(dy):
        if a < 0x200:
            print("  Y:%04X = %06X" % (a, dy[a]))
    print("X changed (kernel scratch <0x200):")
    for a in sorted(dx):
        if a < 0x200:
            print("  X:%04X = %06X" % (a, dx[a]))
