#!/usr/bin/env python3
"""exp26_m13_reverb.py — run the real FX-REVERB m13 (INIT $14538E, PROC $1453DC-$14561B).

Measures:
  A. which memory the machine writes (topology trace)
  B. impulse response -> tail decay vs DEC, damping vs DAMP
  C. LFO modulation of comb tail (5.38 Hz hypothesis)
"""
import sys, json, math

sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, disasm  # noqa

P = 0x400
R6M = P + 0x28
INIT = 0x14538E
INIT_END = 0x1453DB
PROC = 0x1453DC
PROC_END = 0x14561B
M24 = 0xFFFFFF

# param cells r6+$4..$B = DEC DAMP GATE MIX HP LP --- INP
P_DEC, P_DAMP, P_GATE, P_MIX, P_HP, P_LP, P___, P_INP = 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B


def build():
    e = build_emu()
    lines = disasm(0x144000, 0x2000)
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    # the sin table X/Y:$14A000 (8192 words) lies beyond the SRAM dump end
    # ($148063) — reconstruct it the same way amp_env_measure.py does
    import math
    for i in range(8192):
        s = int(round(math.sin(2 * math.pi * i / 8192) * 8388607)) & 0xFFFFFF
        e.X[0x14A000 + i] = s
        e.Y[0x14A000 + i] = s
    return e


def q(v):
    return (int(v) & 0xFFFF) << 16


def set_params(e, dec=64, damp=2, gate=127, mix=32, hp=0, lp=127, inp=64):
    vals = {P_DEC: dec, P_DAMP: damp, P_GATE: gate, P_MIX: mix,
            P_HP: hp, P_LP: lp, P___: 0, P_INP: inp}
    for off, v in vals.items():
        e.Y[R6M + off] = q(v)


def zero_page(e):
    for i in range(0x100):
        e.Y[P + i] = 0
        e.X[P + i] = 0


def run_init(e):
    zero_page(e)
    e.Y[0x124] = 0          # track 0
    e.R[6] = R6M
    e.R[7] = 0x100
    e.ret_stack.append(INIT_END)
    e.run(INIT, end=INIT_END, max_steps=20000)


def run_block(e, sig, watch_writes=False):
    """sig: 16 samples (24-bit). Returns (out16, writes) where writes = list of
    (bank, addr) written if watch_writes."""
    for i in range(64):
        e.X[0x200 + i] = sig[i] if i < len(sig) else 0
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(PROC_END)
    else:
        e.ret_stack[0] = PROC_END
    e.run(PROC, end=PROC_END, max_steps=600000)
    out = [e.Y[0x100 + i] for i in range(16)]
    return out


def f24(v):
    v &= M24
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0


def sig_impulse(pos=0, amp=0.5):
    s = [0] * 64
    s[pos] = int(amp * 8388608) & M24
    return s


def sig_zero():
    return [0] * 64


def warmup(e, n=4):
    """Pass the 256-block fade-in instantly: INIT zeroes the counter (r6+$48)
    and the gain (r6+$47); the PROC sets gain=1.0 once counter >= $100.
    Poke both, then run a few silent blocks to settle the leaky states."""
    e.Y[R6M + 0x48] = 0x101
    e.Y[R6M + 0x47] = 0x7FFFFF
    for _ in range(n):
        run_block(e, sig_zero())


if __name__ == "__main__":
    out = {}

    # ---------- A. topology: run 3 blocks, snapshot ring contents ----------
    e = build()
    run_init(e)
    set_params(e)
    # base pointers written by INIT
    init_ptrs = {}
    for cell in (0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,
                 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43):
        init_ptrs[f"r6+{cell:#x}"] = e.Y[R6M + cell]
    out["init_pointers"] = init_ptrs
    out["init_cells"] = {
        "r6+0x20": e.Y[R6M + 0x20], "r6+0x25": e.Y[R6M + 0x25],
        "r6+0x32": e.Y[R6M + 0x32], "r6+0x47": e.Y[R6M + 0x47],
        "r6+0x48": e.Y[R6M + 0x48],
    }
    warmup(e, 300)

    # run 40 blocks with an impulse at block 2, collect output energy
    tail = []
    for blk in range(40):
        s = sig_impulse(0, 0.5) if blk == 2 else sig_zero()
        o = run_block(e, s)
        tail.append([f24(v) for v in o])
    out["impulse_tail_40blocks"] = tail

    # ---------- B. tail decay vs DEC (400 blocks: comb1 delay = 166 blocks) ----------
    tails = {}
    for dec in (32, 64, 96, 127):
        e = build()
        run_init(e)
        set_params(e, dec=dec, damp=0, inp=64, mix=127)
        warmup(e)
        seq = []
        for blk in range(400):
            s = sig_impulse(0, 0.5) if blk == 2 else sig_zero()
            o = run_block(e, s)
            seq.append(sum(v * v for v in o) ** 0.5)
        tails[str(dec)] = seq
    out["tail_vs_dec"] = tails

    # ---------- C. damping vs DAMP (HF energy of tail) ----------
    damps = {}
    for damp in (0, 20, 60, 127):
        e = build()
        run_init(e)
        set_params(e, dec=100, damp=damp, inp=64, mix=127)
        warmup(e)
        seq = []
        for blk in range(400):
            s = sig_impulse(0, 0.5) if blk == 2 else sig_zero()
            o = run_block(e, s)
            seq.append(sum(v * v for v in o) ** 0.5)
        damps[str(damp)] = seq
    out["tail_vs_damp"] = damps

    json.dump(out, open("/home/z/my-project/scripts/exp26_m13_out.json", "w"))
    # console summary
    print("init pointers:")
    for k, v in init_ptrs.items():
        print(f"  {k} = {v:#x}")
    print("init cells:", {k: hex(v) for k, v in out["init_cells"].items()})
    print("\nblock RMS (impulse @ blk2, DEC=64):")
    for i in (0, 2, 3, 5, 10, 20, 39):
        print(f"  blk{i:3}: {max(abs(x) for x in tail[i]):+.5f}")
    print("\ntail (peak abs) vs DEC:")
    for dec, seq in tails.items():
        print(f"  DEC={dec:>3}: blk3={seq[3]:.4f} blk10={seq[10]:.4f} "
              f"blk30={seq[30]:.4f} blk60={seq[60]:.4f} blk119={seq[119]:.4f}")
    print("\ntail vs DAMP (DEC=100):")
    for damp, seq in damps.items():
        print(f"  DAMP={damp:>3}: blk3={seq[3]:.4f} blk10={seq[10]:.4f} "
              f"blk30={seq[30]:.4f} blk59={seq[59]:.4f}")
