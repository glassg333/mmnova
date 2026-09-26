#!/usr/bin/env python3
"""Iteration 23: DIST element for JUCE.

A) DIST knob (y:(P+$0C)) sweep -> drive-env trajectory X:(P+$DB),
   phase Y:(P+$DB), counter X:(P+$DA), F3 index X:(P+$D9),
   and whether the AUDIO changes (machine out + master out).
B) y:(P+$0D) sweep at DIST=127 -> decay-rate mapping (tblE[$141A00]).
C) Force DIST-handler slot x:(P+$0C) = $145C48 -> transfer + fade cell Y:(P+$34).
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, run_frame

P = 0x400
FRAMES = 80

def q23s(v):
    s = v & 0x800000
    x = v - 0x1000000 if s else v
    return x / 8388608.0

def run_track(dist=0, vol=127, handler=0, frames=FRAMES, level_log=True):
    e = build_track_emu(machine=1)
    setup_track(e, base=64, trig=1, vol=vol)
    e.Y[P + 0x0C] = (dist & 0xFFFF) << 16
    e.Y[P + 0x0D] = (vol & 0xFFFF) << 16
    if handler:
        e.X[P + 0x0C] = handler
    traj = []
    outs_m, outs_k = [], []
    for f in range(frames):
        e.Y[P + 0x28] = 1 if f == 0 else 0   # trig once
        try:
            run_frame(e)
        except Exception as ex:
            return {"error": "%s @ %06X" % (ex, e.pc), "frame": f, "traj": traj}
        if level_log:
            traj.append({
                "f": f,
                "lvl_xDB": q23s(e.X.get(P + 0xDB, 0)),
                "phs_yDB": q23s(e.Y.get(P + 0xDB, 0)),
                "cnt_xDA": q23s(e.X.get(P + 0xDA, 0)),
                "idx_xD9": e.X.get(P + 0xD9, 0),
                "idx_yDA": e.Y.get(P + 0xDA, 0),
                "fade34": q23s(e.Y.get(P + 0x34, 0)),
            })
        outs_m.append([e.Y.get(0x4E6 + i, 0) for i in range(16)])
        outs_k.append([e.Y.get(0x00 + i, 0) for i in range(16)])
    return {"traj": traj, "outs_m": outs_m[-8:], "outs_k": outs_k[-8:],
            "slot_x0C": e.X.get(P + 0x0C, 0)}

def rms(xs):
    n = len(xs)
    return math.sqrt(sum(q23s(x) ** 2 for x in xs) / max(n, 1))

def peak(xs):
    return max(abs(q23s(x)) for x in xs)

def main():
    res = {"probe": "23", "date": "2026-09-22"}

    # --- A: DIST knob sweep ---
    A = {}
    for dist in (0, 32, 64, 96, 127):
        r = run_track(dist=dist)
        A[dist] = {
            "err": r.get("error"),
            "lvl_first10": [t["lvl_xDB"] for t in r["traj"][:10]],
            "lvl_last5": [t["lvl_xDB"] for t in r["traj"][-5:]],
            "phase_last": r["traj"][-1]["phs_yDB"] if r["traj"] else None,
            "idx_last": r["traj"][-1]["idx_xD9"] if r["traj"] else None,
            "rms_m": round(rms([v for fr in r["outs_m"] for v in fr]), 6),
            "peak_m": round(peak([v for fr in r["outs_m"] for v in fr]), 6),
            "rms_k": round(rms([v for fr in r["outs_k"] for v in fr]), 6),
            "out_m_first_frame": r["outs_m"][0][:8] if r["outs_m"] else [],
        }
    res["A_dist_sweep"] = A

    # --- B: y:$0D sweep at DIST=127 ---
    B = {}
    for vol in (0, 32, 64, 127):
        r = run_track(dist=127, vol=vol)
        B[vol] = {
            "lvl_first6": [t["lvl_xDB"] for t in r["traj"][:6]],
            "lvl_last3": [t["lvl_xDB"] for t in r["traj"][-3:]],
        }
    res["B_vol_decay"] = B

    # --- C: forced handler $145C48 ---
    r0 = run_track(dist=127, handler=0)
    r1 = run_track(dist=127, handler=0x145C48, frames=140)
    C = {
        "baseline_rms_m": round(rms([v for fr in r0["outs_m"] for v in fr]), 6),
        "handler_rms_m": round(rms([v for fr in r1.get("outs_m", []) for v in fr]), 6),
        "handler_err": r1.get("error"),
        "handler_fade_first12": [t["fade34"] for t in r1["traj"][:12]],
        "handler_fade_last3": [t["fade34"] for t in r1["traj"][-3:]],
        "handler_out_head": r1.get("outs_m", [[None]])[0][:8] if r1.get("outs_m") else [],
        "base_out_head": r0["outs_m"][0][:8] if r0["outs_m"] else [],
        "handler_rms_first_frames": [round(rms(r1["outs_m"][i]), 6) for i in range(min(8, len(r1.get("outs_m", []))))],
    }
    res["C_handler"] = C

    out = "/home/z/my-project/mining/probe23_dist.json"
    json.dump(res, open(out, "w"), indent=1)
    print("saved", out)
    for d in (0, 32, 64, 96, 127):
        a = A[d]
        print("DIST=%3d lvl[:6]=%s lvl[-2]=%s rms_m=%.6f rms_k=%.6f err=%s" %
              (d, ["%.4f" % v for v in a["lvl_first10"][:6]],
               ["%.4f" % v for v in a["lvl_last5"][-2:]],
               a["rms_m"], a["rms_k"], a["err"]))
    print("B:", json.dumps(B))
    print("C:", json.dumps(C, indent=1)[:1200])

if __name__ == "__main__":
    main()
