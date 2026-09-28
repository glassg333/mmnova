#!/usr/bin/env python3
"""exp62_fm_stage_dump.py — dump PAR/DYN intermediate stages for set 0 block 0
from the emulator, to diff against the C++ transcription stage by stage."""
import sys, json

sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/mmnova/decompiled data/09_not_sorted/missing_data/filter_phaser_pack/22_fm_machines")
import fm_harness as fh
import math

M24 = 0xFFFFFF
R6 = fh.R6M


def build_canonical():
    e = fh.build()
    for i in range(8192):
        v = int(round(math.sin(2 * math.pi * i / 8192) * 8388607)) & M24
        e.X[0x14A000 + i] = v
        e.Y[0x14A000 + i] = v
    return e


def dump_regions(e, which):
    d = {}
    def rng(space, a0, a1):
        mem = e.X if space == "x" else e.Y
        return [mem.get(a, 0) for a in range(a0, a1)]
    if which >= 1:   # after mod-1 phase loop
        d["ring1_x"] = rng("x", 0x20, 0x40); d["ring1_y"] = rng("y", 0x20, 0x40)
        d["st10"] = e.Y.get(R6+0x10, 0); d["st11"] = e.Y.get(R6+0x11, 0)
        d["st12"] = e.Y.get(R6+0x12, 0); d["st13"] = e.Y.get(R6+0x13, 0)
        d["x5"] = e.X.get(5, 0); d["y5"] = e.Y.get(5, 0)
    if which >= 2:   # after mod-1 interp
        d["mod1_sine"] = rng("y", 0x80, 0xA0)
    if which >= 3:   # after diff
        d["mod1_diff"] = rng("x", 0x80, 0xA0); d["st20"] = e.Y.get(R6+0x20, 0)
        d["mod1_diff_y"] = rng("y", 0x80, 0xA0)
    if which >= 4:   # after LP
        d["mod1_lp"] = rng("y", 0x80, 0xA0); d["st2c"] = e.Y.get(R6+0x2C, 0)
    if which >= 5:   # after shaper
        d["mod1_sh"] = rng("y", 0x80, 0xA0); d["st2f"] = e.Y.get(R6+0x2F, 0)
    if which >= 6:   # after mod2+mod3 stages
        d["mod2_sh"] = rng("y", 0xC0, 0xE0); d["mod3_sh"] = rng("y", 0xE0, 0x100)
        d["st2d"] = e.Y.get(R6+0x2D, 0); d["st30"] = e.Y.get(R6+0x30, 0)
        d["st2e"] = e.Y.get(R6+0x2E, 0); d["st31"] = e.Y.get(R6+0x31, 0)
        d["st21"] = e.Y.get(R6+0x21, 0); d["st22"] = e.Y.get(R6+0x22, 0)
    if which >= 7:   # after levels
        d["y0"] = e.y0; d["y1"] = e.y1
        d["st32"] = e.Y.get(R6+0x32, 0); d["st33"] = e.Y.get(R6+0x33, 0)
        d["st34"] = e.Y.get(R6+0x34, 0)
    if which >= 8:   # after mix12
        d["mix12"] = rng("x", 0x80, 0xA0)
    if which >= 9:   # after mix3
        d["mix"] = rng("x", 0x80, 0xA0)
    if which >= 10:  # after mix LP
        d["mixlp"] = rng("x", 0x80, 0xA0); d["st2b"] = e.Y.get(R6+0x2B, 0)
    if which >= 11:  # after carrier phase loop
        d["ringc_x"] = rng("x", 0x20, 0x40); d["ringc_y"] = rng("y", 0x20, 0x40)
        d["st1c"] = e.Y.get(R6+0x1C, 0); d["st1d"] = e.Y.get(R6+0x1D, 0)
        d["st1e"] = e.Y.get(R6+0x1E, 0); d["st1f"] = e.Y.get(R6+0x1F, 0)
    if which >= 12:  # after carrier interp
        d["car"] = rng("x", 0xE0, 0x100)
    if which >= 13:  # after rot1
        d["rot1"] = rng("y", 0x1E, 0x40)
        d["st23"] = e.Y.get(R6+0x23, 0); d["st24"] = e.Y.get(R6+0x24, 0)
        d["st27"] = e.Y.get(R6+0x27, 0); d["st28"] = e.Y.get(R6+0x28, 0)
    if which >= 14:  # after rot2
        d["rot2"] = rng("x", 0x1E, 0x40)
        d["st25"] = e.Y.get(R6+0x25, 0); d["st26"] = e.Y.get(R6+0x26, 0)
        d["st29"] = e.Y.get(R6+0x29, 0); d["st2a"] = e.Y.get(R6+0x2A, 0)
    if which >= 15:  # output
        d["out"] = [v - (1 << 24) if v & 0x800000 else v for v in rng("y", 0x100, 0x120)]
    return d


STOPS_PAR = [
    (0x145F0E, 1), (0x145F2F, 2), (0x145F42, 3), (0x145F53, 4), (0x145F6E, 5),
    (0x146085, 6), (0x1460BD, 7), (0x1460C8, 8), (0x1460F3, 9), (0x146104, 10),
    (0x146124, 11), (0x14614A, 12), (0x146169, 13), (0x146188, 14), (0x14619C, 15),
]
STOPS_DYN = [
    (0x146207, 1), (0x146223, 2), (0x14622E, 3), (0x14623A, 4), (0x14625B, 5),
    (0x14629E, 6), (0x1462C3, 7), (0x1462CE, 8), (0x1462DA, 9), (0x1462FC, 10),
    (0x146317, 11), (0x146336, 12), (0x146355, 13), (0x14636E, 14),
]


def run(mach, stops, out_json, knob, pitch):
    e = build_canonical()
    fh.fm_init(e, mach)
    # CONF
    fh.fm_conf(e, mach, knob)
    e.R[6] = R6; e.R[7] = 0x100
    for r in range(8):
        e.M[r] = M24
    e.A = pitch
    stages = {}
    stopmap = dict(stops)
    e.ret_stack.append(0xDEAD)
    e.pc = fh.MACH[mach]["proc"]
    try:
        while True:
            if e.pc in stopmap:
                stages[str(stopmap[e.pc])] = dump_regions(e, stopmap[e.pc])
            if e.pc == 0x14619C and mach == 9:
                stages["15"] = dump_regions(e, 15)
                break
            if e.pc == 0x14636E and mach == 10:
                stages["14"] = dump_regions(e, 14)
                stages["14"]["out"] = [v - (1 << 24) if v & 0x800000 else v
                                       for v in [e.Y.get(0x100+i, 0) for i in range(32)]]
                break
            e.step()
    except Exception as ex:
        print("stopped:", ex)
    with open(out_json, "w") as f:
        json.dump(stages, f)
    print("wrote", out_json, "stages:", sorted(int(k) for k in stages))


if __name__ == "__main__":
    mach = int(sys.argv[1]) if len(sys.argv) > 1 else 9
    if mach == 9:
        run(9, STOPS_PAR, "/home/z/my-project/work/fm_exact/emu_par_stages.json",
            [64, 64, 64, 64, 64, 64, 64, 64], 11776)
    else:
        run(10, STOPS_DYN, "/home/z/my-project/work/fm_exact/emu_dyn_stages.json",
            [64, 64, 64, 64, 64, 64, 64, 64], 11776)
