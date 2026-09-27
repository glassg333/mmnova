#!/usr/bin/env python3
"""
phaser_harness.py — Run the FX-PHASER machine (m18, proc $144E9A) bit-exactly.

Established layout (machine frame r6 = P+$28):
  params:  y:(r6+$04..$0B) = CNTR DEP SPD MIX FB WID --- INP  (-> P+$2C..$33)
  state:   x/y:(r6+$10..$26) = machine cells (P+$38..$4E)
  input:   y:(r4)+ from r4=0 -> Y:$00-$0F (shared bus, 16 samples)
  output:  y:(r7)+ with r7 = $100 (Y:$100-$11F)
Measures: LFO rate vs SPD, notch behaviour vs CNTR/DEP, the 48-bit phase pipeline
(open question 3), feedback gain vs FB.
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import build_emu
from dsp_emu import EmuError

P = 0x400
R6M = P + 0x28          # machine frame base
PROC = 0x144E9A
INIT = 0x144E81

def build_phaser():
    e = build_emu()
    # add the machine code region (FX machines $144000-$146000)
    from run_kernel import disasm
    for ln in disasm(0x144000, 0x2000):
        pass
    lines = disasm(0x144000, 0x2000)
    e.parse(lines)
    e.next_addr = {}
    for i, a in enumerate(e.order):
        e.next_addr[a] = e.order[i + 1] if i + 1 < len(e.order) else a + 1
    # zero the per-track delay-line RAM (external SRAM, zero at boot)
    for a in range(0x114000, 0x11A000):
        e.X[a] = 0
        e.Y[a] = 0
    return e

def set_params(e, cntr=64, dep=64, spd=32, mix=127, fb=45, wid=0, inp=127):
    q = lambda v: (int(v) & 0xFFFF) << 16
    e.Y[R6M + 0x04] = q(cntr)
    e.Y[R6M + 0x05] = q(dep)
    e.Y[R6M + 0x06] = q(spd)
    e.Y[R6M + 0x07] = q(mix)
    e.Y[R6M + 0x08] = q(fb)
    e.Y[R6M + 0x09] = q(wid)
    e.Y[R6M + 0x0B] = q(inp)

IN_BUF = 0x200   # kernel-provided X-memory input pointer lands here

def set_input(e, sig):
    for i in range(16):
        e.X[IN_BUF + i] = sig[i % len(sig)] & 0xFFFFFF
        e.Y[i] = 0
        e.X[i] = 0

def run_init(e):
    e.R[6] = R6M
    e.R[7] = 0x100
    e.ret_stack.append(0x145035)   # fake return: lands on the run end
    e.run(INIT, end=0x145035, max_steps=10000)

def run_proc(e):
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = IN_BUF          # kernel sets r0 = input buffer before jsr
    if not e.ret_stack:
        e.ret_stack.append(0x145035)
    e.run(PROC, end=0x145035, max_steps=200000)   # full proc to $145035
    return [e.Y[0x100 + i] for i in range(16)]

def s2f(v):
    return (v - (1 << 24)) / 8388608.0 if v & 0x800000 else v / 8388608.0

def experiment_lfo_rate():
    print("=== LFO phase increment vs SPD (48-bit phase at r6+$11/$12) ===")
    rows = []
    for spd in [0, 16, 32, 64, 96, 127]:
        e = build_phaser()
        run_init(e)
        set_params(e, spd=spd)
        set_input(e, [0] * 16)
        run_proc(e)
        ph1 = (e.X.get(R6M + 0x11, 0) << 24) | e.Y.get(R6M + 0x12, 0)
        run_proc(e)
        ph2 = (e.X.get(R6M + 0x11, 0) << 24) | e.Y.get(R6M + 0x12, 0)
        d48 = (ph2 - ph1) & 0xFFFFFFFFFFFF
        rows.append({"spd": spd, "dphase48": d48})
        print("SPD=%3d  dPhase48 = %012X  (%.3f units/block)" % (spd, d48, d48))
    return rows

def experiment_output():
    print("=== output vs CNTR (DC input) ===")
    rows = []
    for cntr in [0, 32, 64, 96, 127]:
        e = build_phaser()
        run_init(e)
        set_params(e, cntr=cntr)
        set_input(e, [0x400000] * 16)
        out = run_proc(e)
        rows.append({"cntr": cntr, "out": out})
        print("CNTR=%3d out: %s" % (cntr, " ".join("%+07.4f" % s2f(v) for v in out[:8])))
    return rows

def experiment_states():
    print("=== state cells after one block (SPD=64 DEP=96) ===")
    e = build_phaser()
    run_init(e)
    set_params(e, spd=64, dep=96)
    set_input(e, [0x400000] * 16)
    out = run_proc(e)
    cells = {}
    for off in range(0x10, 0x27):
        xv = e.X.get(R6M + off, 0)
        yv = e.Y.get(R6M + off, 0)
        if xv or yv:
            cells["%02X" % off] = [xv, yv]
    for k in sorted(cells):
        print("  r6+$%s: X=%06X Y=%06X" % (k, cells[k][0], cells[k][1]))
    return cells

if __name__ == "__main__":
    which = sys.argv[1] if len(sys.argv) > 1 else "lfo"
    fn = {"lfo": experiment_lfo_rate, "out": experiment_output,
          "states": experiment_states}[which]
    res = fn()
    json.dump(res, open("/home/z/my-project/mining/phaser_%s.json" % which, "w"),
              indent=1, default=str)
