#!/usr/bin/env python3
"""exp32_gen_vectors.py — generate bit-exact test vectors for FX machines
17 RINGMOD / 16 DYNAMIX / 19 FLANGER from the DSP56300 emulator.

Grid: param sets x input signals x 8 blocks. Saves JSON with inputs, outputs,
final page-state digest. These vectors are the executable reference for the
JUCE port (model must reproduce them word-for-word).
"""
import sys, json
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from exp31_ringmod_harness import build, run_init, run_block, R6

MACHINES = {
    "ringmod":  {"idx": 17, "init": 0x14789E, "proc": 0x1478C7, "end": 0x147B38},
    "dynamix":  {"idx": 16, "init": 0x1477DE, "proc": 0x1477E6, "end": 0x14789E},
    "flanger":  {"idx": 19, "init": 0x144E81, "proc": 0x144E9A, "end": 0x145036},
}

def mk_inputs():
    sigs = {}
    sigs["zero"] = [0] * 34
    sigs["dc_pos"] = [0x400000] * 34
    sigs["dc_neg"] = [0xC00000] * 34
    imp = [0] * 34
    imp[0] = 0x400000
    sigs["impulse"] = imp
    import math
    sigs["sine"] = [int(0x400000 * math.sin(2 * 3.141592653589793 * k / 16))
                    & 0xFFFFFF for k in range(16)] + [0] * 18
    alt = [0] * 34
    for k in range(16):
        alt[k] = 0x400000 if (k & 1) == 0 else 0xC00000
    sigs["alt16"] = alt
    return sigs

def mk_params():
    ps = []
    for wave in (0, 64, 127):
        for ext in (0, 64):
            for mix, inp in ((127, 64), (64, 100), (0, 64)):
                ps.append([wave, ext, 60, mix, 0, 0, 0, inp])
    ps.append([0, 0, 60, 127, 0, 0, 0, 0])       # INP=0
    ps.append([32, 96, 60, 100, 0, 0, 0, 127])   # INP=max
    return ps

A_ENTRIES = [0, 0x3A4, 0x1D22A0, 0x748A80]  # 0 and three pitch values

def gen(name, spec):
    print("=== %s (proc $%X) ===" % (name, spec["proc"]))
    cases = []
    sigs = mk_inputs()
    n = 0
    for pi, p in enumerate(mk_params()):
        for sname, sig in sigs.items():
            for a_entry in (A_ENTRIES if name == "ringmod" else [A_ENTRIES[0]]):
                e = build()
                run_init(e)
                blocks = []
                for blk in range(8):
                    out = run_block(e, sig, p, a_entry=a_entry)
                    blocks.append(out)
                state = {
                    "page_cells": {("%+d" % off): [e.X.get(R6 + off, 0), e.Y.get(R6 + off, 0)]
                                   for off in range(-0x30, 0x60)
                                   if e.X.get(R6 + off, 0) or e.Y.get(R6 + off, 0)},
                    "lowY": {("%03X" % a): e.Y.get(a, 0) for a in range(0x40, 0xD0)
                             if e.Y.get(a, 0)},
                }
                cases.append({
                    "case": n, "params": p, "signal": sname, "a_entry": a_entry,
                    "blocks_out": blocks, "state": state,
                })
                n += 1
                if n % 20 == 0:
                    print("  %d cases..." % n)
    doc = {
        "machine": name, "dsp_index": spec["idx"],
        "init": spec["init"], "proc": spec["proc"],
        "calling_convention": {
            "r6": "0x528 (FX slot-0 page)", "r7": "0x100 (output bank Y:$100..)",
            "r0_entry": spec["idx"], "a_entry": "kernel pitch value (see README)",
            "params": "y:(r6+$04..$0B) = machine params 0..7 as (v&0xFFFF)<<16",
            "blocks": "8 blocks of the grid; each run_block = one 16-sample pass",
        },
        "cases": cases,
    }
    path = "/home/z/my-project/work/gaps/vectors_%s.json" % name
    json.dump(doc, open(path, "w"))
    print("  saved %s (%d cases)" % (path, n))

if __name__ == "__main__":
    which = sys.argv[1] if len(sys.argv) > 1 else "ringmod"
    gen(which, MACHINES[which])
