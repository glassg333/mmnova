#!/usr/bin/env python3
"""gen_tail_vectors.py — flatten the 10 exp66 section captures into one
delta-format vector file for the C++ voice-frame test.

Format (text):
  SET <idx> <tag> <params>
  SEEDX <1792 hex words>          # X $000-$6FF, unsigned 24-bit
  SEEDY <1792 hex words>
  F <trig> <ndiff>
  D <packed_addr> <hex24>         # cells differing from seed (packed = side<<24|addr)
  ...
The diff-list representation is equivalent to a full compare: a cell absent
from both expected and actual diff lists matches the seed on both sides.
"""
import gzip, json, sys

WLO, WHI = 0x000, 0x700

def cells(d):
    out = [0] * (WHI - WLO)
    for a in range(WLO, WHI):
        v = d.get("X%03X" % a)
        # keys use 3-digit hex for <0x1000
        pass
    return out

def extract(d, side):
    out = []
    for a in range(WLO, WHI):
        v = d.get("%s%03X" % (side, a) if a < 0x1000 else "%s%X" % (side, a))
        out.append(v & 0xFFFFFF)
    return out

def main():
    sets = []
    for n in range(1, 11):
        d = json.loads(gzip.open("/home/z/my-project/work/exp66_sec%02d.json.gz" % n).read())
        sets.extend(d["sets"])
    print("sets:", len(sets), "frames:", sum(len(s["frames"]) for s in sets))

    out = []
    for si, s in enumerate(sets):
        seed = s["seed"]
        sx = extract(seed, "X")
        sy = extract(seed, "Y")
        out.append("SET %d %s %s" % (si, s["tag"], json.dumps(s["params"], sort_keys=True)))
        out.append("SEEDX " + " ".join("%06X" % v for v in sx))
        out.append("SEEDY " + " ".join("%06X" % v for v in sy))
        for fr in s["frames"]:
            fx = extract(fr, "X")
            fy = extract(fr, "Y")
            diffs = []
            for a in range(WHI - WLO):
                if fx[a] != sx[a]:
                    diffs.append((a, fx[a], 0))
                if fy[a] != sy[a]:
                    diffs.append((a, fy[a], 1))
            out.append("F %d %d" % (fr["trig"], len(diffs)))
            for a, v, side in diffs:
                out.append("D %d %06X" % ((side << 24) | (WLO + a), v))
    path = "/home/z/my-project/work/pack8/vectors/voice_frame_vectors.txt"
    with open(path, "w") as f:
        f.write("\n".join(out) + "\n")
    print("wrote", path)

if __name__ == "__main__":
    main()
