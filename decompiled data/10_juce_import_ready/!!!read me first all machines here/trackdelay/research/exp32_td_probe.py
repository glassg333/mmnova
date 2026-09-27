#!/usr/bin/env python3
"""
exp32_td_probe.py — Track Delay dynamic probe.
Runs post-voice chain $04A8-$0B4C with impulse machine input; watches all
memory traffic in the internal X/Y scratch $00-$FF during the delay region,
plus overall diffs, for several parameter settings (TIM/SND/FDB/BAS/WID/SRR).
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, Q23

def q23(x):
    v = int(round(x * Q23))
    return max(-0x800000, min(0x7FFFFF, v)) & 0xFFFFFF

def run_case(e, params, impulse=True, trace_from=0x0939, trace_to=0x0B4D):
    Y, X = e.Y, e.X
    setup_voice(e, r6=0x400, trig=1)
    # machine output buffer: impulse at sample 0 (34 words: 16 L + 16 R + 2?)
    inp = [0] * 34
    if impulse:
        inp[0] = 0x400000
        inp[16] = 0x400000
    for i, v in enumerate(inp):
        Y[0x400 + 0xDC + i] = v & 0xFFFFFF
    # params (page base $400 == Y:$0500 layout: +$1A SRR, $1B TIM, $1C SND,
    # $1D FDB, $1E BAS, $1F WID)
    for off, val in ((0x1A, 'SRR'), (0x1B, 'TIM'), (0x1C, 'SND'), (0x1D, 'FDB'),
                     (0x1E, 'BAS'), (0x1F, 'WID')):
        Y[0x400 + off] = q23(params.get(val, 0.0))
    # clear internal scratch + delay SRAM
    for a in range(0x000, 0x300):
        X[a] = 0; Y[a] = 0
    for a in range(0x114000, 0x118000):
        X[a] = 0; Y[a] = 0
    e.watch = set(range(0x000, 0x100)) | {0x2C9, 0x2CC}
    e.watch_log = []
    snapX, snapY = dict(e.X), dict(e.Y)
    e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
    # output diff: find machine->out buffer (voice page $DC.. and beyond)
    out = {}
    for a in range(0x400, 0x600):
        if e.Y.get(a, 0) != snapY.get(a, 0) and a >= 0x4FC:
            out[a] = e.Y[a]
    return e.watch_log, out, dict(e.X), dict(e.Y)

def fmt_log(log, limit=400):
    lines = []
    for pc, sp, ea, val in log[:limit]:
        lines.append(f"P:{pc:04X} {sp}:{ea:04X} <= {val:06X}")
    return lines

if __name__ == "__main__":
    e = build_emu()
    # Case A: TIM=0.3, SND=0.5, FDB=0.4, BAS/WID mid, SRR off
    pA = dict(TIM=0.3, SND=0.5, FDB=0.4, BAS=0.5, WID=0.5, SRR=0.0)
    log, out, X, Y = run_case(e, pA)
    print("=== CASE A  TIM=.3 SND=.5 FDB=.4 BAS=.5 WID=.5  ===")
    print(f"watch entries: {len(log)}")
    print("\n-- first 120 watch entries (P:pc space:addr <= val) --")
    for l in fmt_log(log, 120):
        print(l)
    # voice page writes (outputs)
    print("\n-- Y writes in voice page $400-$5FF (out of $4FC..) --")
    for a in sorted(out):
        if 0x4FC <= a < 0x560:
            print(f"  Y:{a:04X} = {out[a]:06X}")
    # X scratch state at end
    print("\n-- X scratch $40-$FF at end --")
    for a in range(0x40, 0x100):
        if X.get(a, 0):
            print(f"  X:{a:02X} = {X[a]:06X}")
    print("\n-- Y scratch $40-$FF at end --")
    for a in range(0x40, 0x100):
        if Y.get(a, 0):
            print(f"  Y:{a:02X} = {Y[a]:06X}")
