#!/usr/bin/env python3
"""fm_harness.py — bit-precise FM machine harness (m8 FM-STAT, m9 FM-PAR, m10 FM-DYN).

Contract (from kernel $00F7-$02EB):
  INIT once on machine change; CONF every frame; PROC every frame with:
    r6 = P+$28 (P=$400 -> $428), r7 = $100 (output Y:$100-$11F, 32 words)
    m0..m7 = $FFFFFF (linear), A = pitch word on PROC entry.
  Machine params: y:(r6+$4..$B) = knob<<16.
  Sine table X:$14A000 (8192 words) is NOT in the dump (ColdFire fills it):
  injected as round(sin(2*pi*i/8192)*2^23).
"""
import sys, math, json
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm, PM  # noqa
from dsp_emu import DSP56300, EmuError  # noqa

MACH = {
    8:  dict(init=0x145D12, conf=0x145D1D, proc=0x145D21, name="FM-STAT"),
    9:  dict(init=0x145EC9, conf=0x145ED4, proc=0x145EDB, name="FM-PAR"),
    10: dict(init=0x14619D, conf=0x1461A8, proc=0x1461C1, name="FM-DYN"),
}
R6M = 0x428
M24 = 0xFFFFFF
RTS = {  # rts addresses: init/conf/proc
    8:  dict(init=0x145D1C, conf=0x145D20, proc=0x145EC8),
    9:  dict(init=0x145ED3, conf=0x145EDA, proc=0x14619C),
    10: dict(init=0x1461A7, conf=0x1461C0, proc=0x14636E),
}


def build():
    e = build_emu()
    lines = disasm(0x145D00, 0x760)  # $145D00-$146460: m8/m9/m10 code
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    # inject sine table $14A000-$14BFFF (X and Y)
    for i in range(8192):
        v = int(round(math.sin(2 * math.pi * i / 8192) * 8388608)) & M24
        e.X[0x14A000 + i] = v
        e.Y[0x14A000 + i] = v
    return e


def q(v):
    """knob 0..127 -> param word"""
    return (int(v) & 0xFFFF) << 16


def set_params(e, p):
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(p[i]) & M24


def fm_init(e, machine):
    m = MACH[machine]
    for r in range(8):
        e.M[r] = M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.A = 0
    e.ret_stack.append(0xDEAD)
    e.run(m["init"], end=RTS[machine]["init"], max_steps=100000)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()


def fm_conf(e, machine, p):
    set_params(e, p)
    m = MACH[machine]
    e.R[6] = R6M
    e.R[7] = 0x100
    e.ret_stack.append(0xDEAD)
    e.run(m["conf"], end=RTS[machine]["conf"], max_steps=100000)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()


def fm_block(e, machine, p, pitch_a):
    """one block: CONF + PROC with pitch in A; returns 32 output words (signed)."""
    fm_conf(e, machine, p)
    m = MACH[machine]
    e.R[6] = R6M
    e.R[7] = 0x100
    for r in range(8):
        if e.M[r] != 0x1FFF or True:
            e.M[r] = M24
    e.A = pitch_a & 0xFFFFFFFFFFFF
    e.ret_stack.append(0xDEAD)
    try:
        e.run(m["proc"], end=RTS[machine]["proc"], max_steps=2000000)
    except EmuError as ex:
        # m10 has internal early rts exits -> they pop our guard; that is a
        # normal termination for those paths
        if "00DEAD" not in str(ex):
            raise
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()
    out = []
    for i in range(32):
        v = e.Y[0x100 + i]
        out.append(v - (1 << 24) if v & 0x800000 else v)
    return out


def run(e, start, end, max_steps=2000000):
    e.ret_stack.append(0xDEAD)
    e.run(start, end=end, max_steps=max_steps)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()


def s24(v):
    return v - (1 << 24) if v & 0x800000 else v


def i48(lo_hi):
    """signed 48-bit from (hi, lo) 24-bit pair"""
    hi, lo = lo_hi
    v = ((hi & M24) << 24) | (lo & M24)
    if v & (1 << 47):
        v -= 1 << 48
    return v


def phase48(e, base):
    return i48((e.Y[base], e.Y[base + 1]))


if __name__ == "__main__":
    # smoke test: pure carrier (1ENV=0, 2VOL=0, 1FB=0) at several pitch values
    for A in (1000, 2000, 4000, 8000, 16000):
        e = build()
        fm_init(e, 8)
        p = [64, 0, 0, 0, 64, 0, 127, 64]
        outs = []
        for _ in range(8):
            outs += fm_block(e, 8, p, A)
        # count positive-going zero crossings
        zc = sum(1 for i in range(1, len(outs)) if outs[i - 1] <= 0 < outs[i])
        print("A=%5d  len=%d  zc=%d  first16=%s" % (
            A, len(outs), zc, " ".join("%6d" % (v >> 12) for v in outs[:16])))
