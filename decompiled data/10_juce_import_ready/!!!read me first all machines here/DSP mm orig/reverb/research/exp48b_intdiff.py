#!/usr/bin/env python3
"""exp48b_intdiff.py — frame-level internal diff: emulator (reference) vs C++ port."""
import json

emu = json.load(open("/home/z/my-project/scripts/exp48_intdump_emu.json"))
prt = json.load(open("/home/z/my-project/scripts/exp48_intdump_port.json"))

M24 = 0xFFFFFF


def sgn(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v


def cmp_vec(a, b):
    """return list of differing indices"""
    return [i for i in range(min(len(a), len(b))) if (a[i] & M24) != (b[i] & M24)]


FIELDS = ["out", "ap", "comb", "wet"]


def frame_diff(f):
    e, p = emu[f], prt[f]
    d = {}
    # out
    d["out"] = cmp_vec(e["out"], p["out"])
    # ap: emulator stores 32 (Y:$0-$1F — duplicated stereo?), port 16
    ap_e = e.get("ap_out", [0]*32)
    ap_p = p["ap"] * 2  # port snapAp is 16; emu Y:$0-1F holds 16 unique? compare first 16
    d["ap"] = cmp_vec(ap_e[:16], p["ap"])
    # comb 6 banks: emu comb14 = Y:$20-$5F (4 banks of 16) + comb5 (Y:$60) + comb6 (Y:$70)
    cb_e = e.get("comb14", [])[:64] + e.get("comb5", []) + e.get("comb6", [])
    cb_p = []
    for c in range(6):
        cb_p += p["comb"][c]
    d["comb"] = cmp_vec(cb_e, cb_p)
    # wet: emu X:$0-$1F = gated matrix (pre-HP!) vs port snapWet = post-LP — different stages!
    # instead compare ramp-gated mtx: use emu "wet" vs port snapMtxL/R interleaved — via dbg only.
    return d


print("frame | out | ap | comb (first diffs)")
first_bad = None
for f in range(48):
    d = frame_diff(f)
    flag = ""
    if d["out"]:
        flag += f" OUT[{d['out'][:3]}...]"
        if first_bad is None:
            first_bad = ("out", f)
    if d["ap"]:
        flag += f" AP[{d['ap'][:3]}...]"
        if first_bad is None:
            first_bad = ("ap", f)
    if d["comb"]:
        flag += f" COMB[{d['comb'][:4]}...]"
        if first_bad is None:
            first_bad = ("comb", f, d["comb"][0])
    if flag:
        print(f"f{f:02d}:{flag}")

print("\nfirst divergence:", first_bad)
if first_bad:
    kind, f = first_bad[0], first_bad[1]
    e, p = emu[f], prt[f]
    if kind == "comb":
        i = first_bad[2]
        c, k = i // 16, i % 16
        src = ("comb14" if c < 4 else ("comb5" if c == 4 else "comb6"))
        vec = e[src] if c < 4 else e[src]
        ki = k if c < 4 else k
        print(f"comb{c+1}[{k}]: emu={vec[ki] & M24:06X} port={p['comb'][c][k] & M24:06X}")
        print("emu comb5 n1/y1/x0:", e.get("comb5_n1"), e.get("comb5_y1"), e.get("comb5_x0"))
        print("emu rd5/rd6:", e.get("comb5_r1"), e.get("comb6_r1"), "lfo:", e.get("lfo_phase"))
        print("port n1/frac/wE/wO/cD:", p["n1"], f"{p['frac']:06X}", f"{p['wE']:06X}",
              f"{p['wO']:06X}", f"{p['cD']:06X}")
        print("port rd5/rd6/wr5/wr6:", p["rd5"], p["rd6"], p["wr5"], p["wr6"],
              "st5/st6:", f"{p['st5']:014X}", f"{p['st6']:014X}")
        print("port lfo:", f"{p['lfo']:06X}")
        for cc in range(6):
            print(f"  comb{cc+1} emu:", " ".join(f"{x & M24:06X}" for x in
                  (e["comb14"][cc*16:cc*16+16] if cc < 4 else (e["comb5"] if cc == 4 else e["comb6"]))[:8]))
            print(f"  comb{cc+1} prt:", " ".join(f"{x & M24:06X}" for x in p["comb"][cc][:8]))
