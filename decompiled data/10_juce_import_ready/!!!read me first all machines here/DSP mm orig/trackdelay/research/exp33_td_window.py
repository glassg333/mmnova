#!/usr/bin/env python3
"""
exp33_td_window.py — Track Delay focused probe: capture READS+WRITES only inside
P:$0939-$0B4C (delay+SRR+output tail), with param sweeps.
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu, setup_voice, Q23

def q23(x):
    v = int(round(x * Q23))
    return max(-0x800000, min(0x7FFFFF, v)) & 0xFFFFFF

TD_LO, TD_HI = 0x0939, 0x0B4D

def run_case(e, params, impulse=True):
    Y, X = e.Y, e.X
    setup_voice(e, r6=0x400, trig=1)
    inp = [0] * 34
    if impulse:
        inp[0] = 0x400000   # L impulse
        inp[16] = 0x400000  # R impulse
    for i, v in enumerate(inp):
        Y[0x400 + 0xDC + i] = v & 0xFFFFFF
    for off, val in ((0x1A, 'SRR'), (0x1B, 'TIM'), (0x1C, 'SND'), (0x1D, 'FDB'),
                     (0x1E, 'BAS'), (0x1F, 'WID')):
        Y[0x400 + off] = q23(params.get(val, 0.0))
    for a in range(0x000, 0x300):
        X[a] = 0; Y[a] = 0
    for a in range(0x114000, 0x118000):
        X[a] = 0; Y[a] = 0
    # --- instrument rd + wr, filtered to the delay window ---
    e.watch = set(range(0x000, 0x100)) | set(range(0x114000, 0x118000)) | {0x2C9}
    log = []
    base_rd = e.rd
    def rd_hook(space, ea):
        if e.pc >= TD_LO and e.pc < TD_HI and ea in e.watch:
            log.append((e.pc, space + "R", ea, base_rd(space, ea)))
        return base_rd(space, ea)
    base_wr = e.wr
    def wr_hook(space, ea, val):
        if e.pc >= TD_LO and e.pc < TD_HI and ea in e.watch:
            log.append((e.pc, space + "W", ea, val & 0xFFFFFF))
        return base_wr(space, ea, val)
    e.rd = rd_hook
    e.wr = wr_hook
    snapX, snapY = dict(e.X), dict(e.Y)
    e.run(0x04A8, end=0x0B4C, max_steps=2_000_000)
    e.rd = base_rd
    e.wr = base_wr
    out = {a: e.Y[a] for a in range(0x4FC, 0x600) if e.Y.get(a, 0) != snapY.get(a, 0)}
    return log, out, dict(e.X), dict(e.Y)

def summarize(log):
    # collapse consecutive helper-body entries: keep call-site granularity
    out = []
    for pc, op, ea, val in log:
        out.append(f"P:{pc:04X} {op} {ea:04X} <= {val:06X}")
    return out

if __name__ == "__main__":
    e = build_emu()
    pA = dict(TIM=0.3, SND=0.5, FDB=0.4, BAS=0.5, WID=0.5, SRR=0.0)
    log, out, X, Y = run_case(e, pA)
    print(f"=== CASE A  TIM=.3 SND=.5 FDB=.4 BAS=.5 WID=.5 === entries={len(log)}")
    s = summarize(log)
    print("\n".join(s[:200]))
    print("...")
    print("\n".join(s[-80:]))
    print("\n-- Y voice-page diffs $4FC+ (output buffer?) --")
    for a in sorted(out):
        print(f"  Y:{a:04X} = {out[a]:06X}")
    print("\n-- X scratch $40-$FF end state --")
    for a in range(0x40, 0x100):
        if X.get(a, 0):
            print(f"  X:{a:02X} = {X[a]:06X}")
    print("-- Y scratch $40-$FF end state --")
    for a in range(0x40, 0x100):
        if Y.get(a, 0):
            print(f"  Y:{a:02X} = {Y[a]:06X}")
