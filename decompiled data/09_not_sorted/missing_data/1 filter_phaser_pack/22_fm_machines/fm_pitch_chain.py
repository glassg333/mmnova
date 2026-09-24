#!/usr/bin/env python3
"""fm_pitch_chain.py — close the pitch chain entirely inside the emulator:
1) run the kernel's conversion instructions $02DD-$02EA with a forced clamped
   pitch (as if the glide finished), read the resulting acc;
2) feed that acc to the FM-STAT machine as the PROC entry A;
3) measure the carrier phase increment per iteration.
Result: exact note -> frequency law of the firmware.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_block, R6M
from run_kernel import build_emu, disasm  # noqa

CONV = 0x02DD
CONV_END = 0x02EB


def kernel_A(pitch):
    """run $02DD..$02EA with A = clamped pitch; returns acc the machine receives"""
    e = build_emu()
    e.A = pitch & 0xFFFFFFFFFFFF
    e.ret_stack.append(0xDEAD)
    e.run(CONV, end=CONV_END, max_steps=100)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()
    return e.A


def inc_of(A48, blocks=40):
    e = build(); fm_init(e, 8)
    p = [64, 64, 0, 0, 64, 0, 127, 64]
    fm_block(e, 8, p, A48 & 0xFFFFFFFFFFFF)
    fm_block(e, 8, p, A48 & 0xFFFFFFFFFFFF)
    ph0 = e.Y[R6M + 0x16] & 0x1FFF
    tot = 0
    for _ in range(blocks):
        fm_block(e, 8, p, A48 & 0xFFFFFFFFFFFF)
        ph1 = e.Y[R6M + 0x16] & 0x1FFF
        tot += ((ph1 - ph0) % 8192)
        ph0 = ph1
    return tot / float(blocks) / 32.0


if __name__ == "__main__":
    print("note  pitch(2048/oct)  kernelA(acc)   a1     a0      inc/iter   f@88.2k    f@44.1k")
    for note in (12, 24, 33, 36, 45, 48, 57, 60, 69, 72, 81, 93, 105):
        p = note * 2048 // 12
        A = kernel_A(p)
        a1 = (A >> 24) & 0xFFFFFF
        a0 = A & 0xFFFFFF
        inc = inc_of(A)
        print("%4d  %8d  %012X  %6d %7d  %9.3f  %9.2f  %9.2f" % (
            note, p, A, a1, a0, inc, inc * 88200 / 8192, inc * 44100 / 8192))
