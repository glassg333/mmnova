#!/usr/bin/env python3
"""exp51_m1m2_harness.py — run GND-SIN m1 ($144CD0) and GND-NOIS m2 ($144DA2)
in the emulator; probe oscillator behaviour (freq, waveform, filters)."""
import sys, math

sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm  # noqa

P = 0x400
R6M = P + 0x28
M1_PROC = 0x144CD0
M1_END = 0x144D21      # proc runs into tail functions; full body to $144D99
M2_PROC = 0x144DA2
M2_END = 0x144E81
M24 = 0xFFFFFF


def build():
    e = build_emu()
    lines = disasm(0x144000, 0x2000)
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    for i in range(8192):
        s = int(round(math.sin(2 * math.pi * i / 8192) * 8388607)) & M24
        e.X[0x14A000 + i] = s
        e.Y[0x14A000 + i] = s
    return e


def q(v):
    return (int(v) & 0xFFFF) << 16


def m1_init(e):
    e.Y[R6M + 0x10] = 0
    e.X[R6M + 0x10] = 0x14A000
    e.X[R6M + 0x12] = 0
    e.Y[R6M + 0x12] = 0
    # states
    for off in (0x14, 0x15, 0x16, 0x17, 0x1C, 0x1D, 0x1E, 0x1F):
        e.X[R6M + off] = 0
        e.Y[R6M + off] = 0


def m1_frame(e, pitch_q16, out=None):
    """Run one m1 PROC. pitch in A (q16 knob value like other machines)."""
    e.R[6] = R6M
    e.R[7] = 0x100
    if not e.ret_stack:
        e.ret_stack.append(M1_END)
    else:
        e.ret_stack[0] = M1_END
    e.A = (sext56(pitch_q16 << 24))
    e.run(M1_PROC, end=M1_END, max_steps=2000000)
    return [e.Y.get(0x100 + i, 0) & M24 for i in range(16)]


def sext56(v):
    v &= (1 << 56) - 1
    return (v ^ (1 << 55)) - (1 << 55)


def m2_init(e):
    e.Y[R6M + 0x10] = 0xA
    e.Y[R6M + 0x11] = 1
    for off in range(0x12, 0x20):
        e.X[R6M + off] = 0
        e.Y[R6M + off] = 0


def m2_frame(e, params=None):
    e.R[6] = R6M
    e.R[7] = 0x100
    if not e.ret_stack:
        e.ret_stack.append(M2_END)
    else:
        e.ret_stack[0] = M2_END
    if params:
        for off, v in params.items():
            e.Y[R6M + off] = q(v)
    e.run(M2_PROC, end=M2_END, max_steps=2000000)
    return [e.Y.get(0x100 + i, 0) & M24 for i in range(16)]


def f24(v):
    v = int(v) & M24
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0


def main():
    e = build()
    print("=== m1 GND-SIN: frequency probe (pitch knob -> Hz) ===")
    m1_init(e)
    import collections
    for knob in (32, 64, 96, 127):
        m1_init(e)
        # zero the phase accumulators, run 64 frames, find the waveform period
        sig = []
        for f in range(96):
            o = m1_frame(e, knob << 16)
            sig.extend(f24(v) for v in o[0::2])   # L channel
        # crude zero-crossing frequency estimate over the last 64 samples
        tail = sig[-64:]
        zc = sum(1 for i in range(1, len(tail)) if tail[i-1] < 0 <= tail[i])
        print(f"knob={knob:3d}  zero-crossings/64smp={zc:2d}  "
              f"peak={max(tail):+.3f}  min={min(tail):+.3f}  "
              f"first8={[f'{f24(v):+.3f}' for v in sig[40:48]]}")

    print()
    print("=== m2 GND-NOIS: noise + ladder filter probe ===")
    for st, red in ((0, 0), (64, 0), (127, 0), (64, 64), (64, 127)):
        m2_init(e)
        sig = []
        for f in range(24):
            o = m2_frame(e, {0x04: st, 0x05: red, 0x06: 0, 0x0B: 64})
            sig.extend(f24(v) for v in o[0::2])
        tail = sig[-64:]
        print(f"ST={st:3d} RED={red:3d}  peak={max(tail):+.3f}  "
              f"mean={sum(tail)/len(tail):+.4f}  first8={[f'{v:+.3f}' for v in tail[:8]]}")


if __name__ == "__main__":
    main()
