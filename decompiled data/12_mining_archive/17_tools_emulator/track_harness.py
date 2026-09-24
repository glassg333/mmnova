#!/usr/bin/env python3
"""
track_harness.py — Full track emulation: kernel from the track entry $0100 with a
real machine (GND-SIN) dispatched, so the machine writes its own output/state and
the filter chain processes a real synth signal. Param sweeps then reach the DSP.

Layout established by mining:
  y:$120+track   = machine slot pointer (points into the $1001xx dispatch tables)
  y:$123         = P+$28 where P = the filter-section r6 (voice page base)
  y:$124         = track index (0..2)
  x:(slot+$1001AF) = machine proc pointer (slot = y:$120 value)
  filter params  = Y[P+$10..$17], TRIG = Y[P+$28], machine params = Y[P+$2C..]
  machine output = Y[P+$DC..$FF] (16 audio samples at P+$E6..$F5)
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm, PM

R6 = 0x400

def build_track_emu(machine=1):
    """machine: DSP machine index (1 = GND-SIN, 19 = FX-PHASER, ...)"""
    lines = disasm(0x0000, 0x0B4E)
    # also disassemble the machine code region so jsr (r1) can execute
    lines += disasm(0x144000, 0x2000)
    e = DSP56300 = None
    from dsp_emu import DSP56300 as D
    e = D(lines)
    data = open(PM, "rb").read()
    n = len(data) // 3
    for i in range(0x100000, min(n, 0x160000)):
        o = i * 3
        w = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
        e.X[i] = w
        e.Y[i] = w
    # machine dispatch: slot pointer -> proc table row; patch the proc pointer
    # the kernel reads x:(r0+$1001AF) with r0 = y:$120
    slot = 0x1001AF + machine
    e.Y[0x120] = slot
    e.Y[0x121] = slot   # tracks 1/2 same machine (mono tests use track 0 only)
    e.Y[0x122] = slot
    e.X[slot + 0x1001AF] = PROC[machine]
    e.X[slot + 0x10016B] = PROC_INIT[machine]
    return e

# proc pointers from machine_dispatch_tables.txt (iteration-11 pack)
PROC = {1: 0x144CD0, 2: 0x144DA2, 15: 0x14767B, 19: 0x144E9A}
PROC_INIT = {1: 0x144CC9, 2: 0x144D9A, 15: 0x147661, 19: 0x144E81}

def setup_track(e, base=0.5, wdth=0.25, hpq=0.0, lpq=0.0, mode=0,
                amp_atk=0, amp_dec=127, filt_atk=0.5, filt_dec=0.5,
                bofs=0.5, wofs=0.5, tempo=120, trig=1, vol=1.0,
                mach_params=None):
    Y, X = e.Y, e.X
    P = R6
    for i in range(0x100):
        Y[P + i] = 0
        X[P + i] = 0
    Y[P + 0x00] = (amp_atk & 0xFFFF) << 16
    Y[P + 0x01] = 0
    Y[P + 0x02] = (amp_dec & 0xFFFF) << 16
    Y[P + 0x03] = 0
    Y[P + 0x0D] = int(vol * 127) << 16          # AMP VOL?
    Y[P + 0x0E] = 0                              # PAN L
    Y[P + 0x0F] = 0                              # PAN R
    X[P + 0x0F] = 1                              # mono flag
    Y[P + 0x10] = int(base / 127.0 * 8388608) if base <= 1 else 0
    # NOTE: base given as raw param 0..127 here:
    Y[P + 0x10] = (int(base) & 0xFFFF) << 16 if base > 1 else int(base / 127.0 * 8388608)
    Y[P + 0x11] = (int(wdth) & 0xFFFF) << 16 if wdth > 1 else int(wdth / 127.0 * 8388608)
    Y[P + 0x12] = (int(hpq) & 0xFFFF) << 16 if hpq > 1 else int(hpq / 127.0 * 8388608)
    Y[P + 0x13] = (int(lpq) & 0xFFFF) << 16 if lpq > 1 else int(lpq / 127.0 * 8388608)
    Y[P + 0x14] = int(filt_atk * 8388608)
    Y[P + 0x15] = int(filt_dec * 8388608)
    Y[P + 0x16] = int(bofs * 8388608)
    Y[P + 0x17] = int(wofs * 8388608)
    Y[P + 0x23] = tempo
    Y[P + 0x25] = mode
    Y[P + 0x28] = trig
    Y[P + 0x21] = 1
    if mach_params:
        for i, v in enumerate(mach_params[:8]):
            Y[P + 0x2C + i] = int(v * 8388608) & 0xFFFFFF
    Y[0x120] = e.Y[0x120]; Y[0x121] = e.Y[0x121]; Y[0x122] = e.Y[0x122]
    Y[0x123] = P + 0x28
    Y[0x124] = 0
    e.R[6] = P + 0x28

def run_frame(e):
    """One track frame: from the track entry $0100 to the master routing."""
    e.run(0x0100, end=0x0B4C, max_steps=500000)

def read_out(e):
    return [e.Y[0x4E6 + i] for i in range(16)]

if __name__ == "__main__":
    from dsp_emu import EmuError
    e = build_track_emu(machine=1)
    setup_track(e, base=64, trig=1)
    try:
        run_frame(e)
        print("frame ok, steps =", e.steps)
        print("machine out:", [("%06X" % v) for v in read_out(e)])
    except EmuError as ex:
        print("ERR:", ex, "at pc=%06X" % e.pc)
        ins = e.prog.get(e.pc)
        print(ins)
