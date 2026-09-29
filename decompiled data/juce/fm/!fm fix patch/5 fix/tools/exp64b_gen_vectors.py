#!/usr/bin/env python3
"""exp64b_gen_vectors.py — flatten the exp64 batch result into a text vector
file for the C++ plugin-level test (test_fm_plugin_level.cpp).

Sources everything from work/exp64_fm_pluginlevel.json.gz (ONE emulator run,
rule 18 — no new emulation here).

Notes on the emulator A/B-swap quirk (see MnmAmpEnv.hpp header):
  * prevX (X[P+$FA]) holds the SIN target  = the plugin's R_t (RIGHT channel)
  * prevY (Y[P+$FA]) holds the COS target  = the plugin's L_t (LEFT channel)
The per-sample ring is NOT compared against the emulator because of the swap;
the ramp law itself (linear, 2^20*diff accumulator steps, a1 truncation) was
verified from the instruction trace, and the targets are verified exactly.
"""
import json, gzip, os

SRC = "/home/z/my-project/work/exp64_fm_pluginlevel.json.gz"
OUT = "/home/z/my-project/work/pack7/vectors/plugin_level_vectors.txt"

d = json.load(gzip.open(SRC))
os.makedirs(os.path.dirname(OUT), exist_ok=True)

lines = []
n_alaw = n_env = n_envframes = n_vp = n_vpframes = 0

for s in d["a_law"]:
    A = s["A"][0]
    assert all(a == A for a in s["A"]), "A not settled: %s" % s
    lines.append("ALAW %d %d %d %d" % (s["w41"], s["tune"], s["w30"], A))
    n_alaw += 1

for s in d["env"]:
    frames = s["frames"]
    lines.append("ENVSET %d %d %d %d %d" % (
        s["atk"], s["hold"], s["dec"], s["rel"], len(frames)))
    for r in frames:
        assert len(r["mach"]) >= 32
        lines.append("ENV %d %d %d %d %d %d" % (
            r["f"], r["trig"], r["lvl"] & 0xFFFFFF, r["ph"], r["cnt"],
            r["A"] if r["A"] is not None else 0))
        lines.append("MACH " + " ".join(str(w & 0xFFFFFF) for w in r["mach"][:32]))
        n_envframes += 1
    n_env += 1

for s in d["volpan"]:
    frames = s["frames"]
    lines.append("VPSET %d %d %d" % (s["vol"], s["pan"], len(frames)))
    for r in frames:
        ring = [w & 0xFFFFFF for w in r["ring"]]
        lines.append("VP %d %d %d %d %d %s" % (
            r["f"], r["lvl"] & 0xFFFFFF,
            r["prevX"] & 0xFFFFFF, r["prevY"] & 0xFFFFFF,
            r["A"] if r["A"] is not None else 0,
            " ".join(str(w) for w in ring)))
        n_vpframes += 1
    n_vp += 1

open(OUT, "w").write("\n".join(lines) + "\n")
print("wrote %s: %d ALAW, %d ENVSET (%d frames), %d VPSET (%d frames)" % (
    OUT, n_alaw, n_env, n_envframes, n_vp, n_vpframes))
