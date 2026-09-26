#!/usr/bin/env python3
"""exp47_reverb_vectors.py — generate reference vectors for the m13 C++ port.
Runs the emulator with fixed param sets, dumps 24 output frames per set."""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R

M24 = 0xFFFFFF

def run_set(dec, damp, gate, mix, inp, nframes=24, seed=12345):
    e = R.build()
    R.run_init(e)
    R.set_params(e, dec=dec, damp=damp, gate=gate, mix=mix, hp=8, lp=100, inp=inp)
    R.warmup(e, 6)
    frames = []
    for k in range(nframes):
        # deterministic test signal: decaying noise burst + sine
        s = [0]*64
        if k < 3:
            v = (seed*7919 + k*104729) % 2000000 - 1000000
            for i in range(64):
                s[i] = int(v * (0.6 ** (i//8)) ) & M24 if i < 32 else 0
        o = R.run_block(e, s)
        frames.append({"bus": s[:32], "out": [x & M24 for x in o]})
    return frames

if __name__ == "__main__":
    out = {}
    for name, args in {
        "A": (100, 2, 127, 127, 64),
        "B": (60, 70, 40, 64, 100),
        "C": (20, 127, 90, 32, 30),
    }.items():
        out[name] = {"params": dict(zip(("dec","damp","gate","mix","inp"), args)),
                     "frames": run_set(*args)}
    json.dump(out, open("/home/z/my-project/scripts/exp47_vectors.json", "w"))
    print("sets:", list(out), "frames/set:", len(out["A"]["frames"]))
    print("out[0] A:", [hex(v) for v in out["A"]["frames"][0]["out"][:6]])
