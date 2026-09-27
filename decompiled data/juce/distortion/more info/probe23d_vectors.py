#!/usr/bin/env python3
"""Iteration 23d: final DIST element vectors.
- x:$2C3=0x200 (r5=master buffer ptr outside X:$00-$1F) -> no feedback decay
- verify attack step == tblA[DIST] bit-exact
- capture per-frame drive level trajectory + output wave as JUCE test vectors
"""
import sys, math, json
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track
from amp_env_measure import sgn, inject_sine_tables, load_xy_memory, TBL

P = 0x400

def load_tbl3(name):
    d = open(TBL + name, "rb").read()
    out = []
    for i in range(len(d) // 3):
        w = (d[3*i] << 16) | (d[3*i+1] << 8) | d[3*i+2]
        out.append(w)
    return out

TBLA = load_tbl3("P_141800_env_shape_tbl_A.bin")
TBLA_S = [v - 0x1000000 if v >= 0x800000 else v for v in TBLA]

def run(dist, frames=40, vol=127, retrig=False):
    e = build_track_emu(machine=14)
    inject_sine_tables(e); load_xy_memory(e)
    for a in range(0x2C0, 0x2CA):
        e.X[a] = 0
    e.X[0x2C3] = 0x200          # r5 source: master buffer ptr (pure-bus mix)
    setup_track(e, base=64, trig=0, vol=vol, mach_params=[0.5] * 8)
    e.X[P + 0x0B] = 64
    phase = 0.0
    rows = []
    for f in range(frames):
        e.Y[P + 0x28] = 1 if (f == 0 or retrig) else 0
        e.Y[P + 0x0C] = (dist & 0xFFFF) << 16
        e.run(0x0100, end=0x02EB, max_steps=800000)
        e.X[0x2C9] = 0x300
        for i in range(0x22):
            v = int(round(0.9 * 8388607 * math.sin(phase)))
            e.Y[0x100 + i] = v & 0xFFFFFF
            phase += 2 * math.pi / 32.0
        for i in range(16):
            v = int(round(0.9 * 8388607 * math.sin(phase)))
            e.X[i] = v & 0xFFFFFF
            e.Y[i] = v & 0xFFFFFF
            phase += 2 * math.pi / 32.0
        e.run(0x02EC, end=0x0B4C, max_steps=800000)
        out = [sgn(e.Y[i]) for i in range(0x20)]
        rows.append(dict(
            f=f, lvl=sgn(e.X.get(P + 0xDB, 0)), phs=sgn(e.Y.get(P + 0xDB, 0)),
            idx=e.X.get(P + 0xD9, 0),
            rms=round(math.sqrt(sum(v * v for v in out) / 32), 1),
            wave=[v for v in out[:16]],
        ))
    return rows

def main():
    res = {}
    # 1) stability check: DIST=0, no feedback decay?
    rows = run(0, frames=30)
    rms_mid = rows[20]["rms"]
    print("stability DIST=0: rms f5=%.1f f10=%.1f f20=%.1f f29=%.1f" %
          (rows[5]["rms"], rows[10]["rms"], rows[20]["rms"], rows[29]["rms"]))
    res["stable"] = abs(rows[20]["rms"] - rows[10]["rms"]) < rows[10]["rms"] * 0.3

    # 2) bit-exact attack step vs tblA
    steps = {}
    for dist in (0, 16, 32, 64, 96, 127):
        rows = run(dist, frames=8)
        ls = [r["lvl"] for r in rows]
        # first frames are pure attack (level from 0)
        d = [(ls[i+1] - ls[i]) & 0xFFFFFF for i in range(min(4, len(ls)-1))]
        exp = TBLA_S[dist] & 0xFFFFFF
        ok = all(x == exp for x in d)
        steps[dist] = dict(step_meas=[hex(x) for x in d], step_tblA=hex(exp), match=ok)
        print("DIST=%3d step meas %s tblA %s match=%s" %
              (dist, [hex(x) for x in d], hex(exp), ok))
    res["attack_steps"] = steps

    # 3) full trajectories + output waves as test vectors
    vec = {}
    for dist in (0, 64, 127):
        rows = run(dist, frames=80)
        vec[dist] = dict(
            lvl=[r["lvl"] for r in rows],
            phs=[r["phs"] for r in rows],
            idx=[r["idx"] for r in rows],
            rms=[r["rms"] for r in rows],
            wave_f70=rows[70]["wave"],
        )
        r = vec[dist]
        print("DIST=%3d lvl[0:8]=%s lvl[70]=%d rms[10;40;70]=%d;%d;%d idx[70]=%s" %
              (dist, [v for v in r["lvl"][:8]], r["lvl"][70],
               r["rms"][10], r["rms"][40], r["rms"][70], hex(r["idx"][70])))
    res["vectors"] = vec
    json.dump(res, open("/home/z/my-project/mining/probe23d_vectors.json", "w"))
    print("saved probe23d_vectors.json")

if __name__ == "__main__":
    main()
