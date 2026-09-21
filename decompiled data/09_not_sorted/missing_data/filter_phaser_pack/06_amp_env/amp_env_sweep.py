#!/usr/bin/env python3
"""amp_env_sweep.py — systematic parameter sweeps of the Monomachine track AMP
envelope (bit-exact, real kernel). Produces amp_env_sweep.json for the pack."""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase, P, sgn, ATT, DEC

def frames_until_phase(c, ph_target, maxf=4000):
    for k in range(maxf):
        r = c.frame(trig=1 if k == 0 else 0)
        if r["phase"] == ph_target and k > 0:
            return k, r
    return None, None

def sweep_attack():
    out = {}
    for atk in (0, 8, 16, 32, 48, 64, 80, 96, 112, 127):
        pred = 0x800000 / max(ATT[atk], 1)
        if pred > 600:   # too slow to emulate frame-by-frame; predicted only
            out[atk] = dict(frames=None, predicted=pred, ms=None, inc=ATT[atk],
                            note="predicted only (too slow to emulate)")
            continue
        c = EnvCase(atk=atk, dec=127, vol=127)
        c.e.Y[P + 0x05] = 0x7F << 16
        n = None
        for k in range(int(pred) + 40):
            r = c.frame(trig=1 if k == 0 else 0)
            if r["phase"] != 0 and k > 0:
                n = k
                break
        out[atk] = dict(frames=n, predicted=pred,
                        ms=round(n * 16 / 44100.0 * 1000, 3) if n else None,
                        inc=ATT[atk])
    return out

def sweep_decay():
    out = {}
    for dec in (0, 16, 32, 48, 64, 80, 96, 110, 120, 126, 127):
        c = EnvCase(atk=0, dec=dec, vol=127)
        c.e.Y[P + 0x05] = 0x7F << 16
        rec = []
        for k in range(300):
            r = c.frame(trig=1 if k == 0 else 0)
            rec.append((k, abs(r["level"]) / 8388608.0, r["phase"]))
        # measure frames from attack end to |level| < 0.5 and < 0.001
        def frames_to(thr):
            start = None
            for k, lv, ph in rec:
                if ph >= 2 and start is None:
                    start = k
                if start is not None and lv < thr:
                    return k - start
            return None
        f = DEC[dec] if dec < 127 else -8388608
        factor = abs(f) / 8388608.0
        out[dec] = dict(
            factor=factor,
            t_half=frames_to(0.5), t_to_60dB=frames_to(0.001),
            half_ms=round(frames_to(0.5) * 16 / 44100.0 * 1000, 2) if frames_to(0.5) else None,
            predicted_half=round(0.5, 4),
        )
    return out

def sweep_hold():
    out = {}
    for hold in (0, 1, 4, 16, 32, 64):
        for tempo in (120,):
            c = EnvCase(atk=0, dec=127, hold=hold, tempo=tempo, vol=127)
            c.e.Y[P + 0x05] = 0x7F << 16
            n = None
            for k in range(2000):
                r = c.frame(trig=1 if k == 0 else 0)
                if k >= 2 and r["phase"] == 2:
                    n = k
                    break
            out["h%d" % hold] = dict(hold_frames_in_ph1=n)
    return out

def release_and_kill():
    out = {}
    for rel in (0, 32, 64, 96, 127):
        c = EnvCase(atk=0, dec=127, rel=rel, vol=127)
        c.e.Y[P + 0x05] = 0x7F << 16
        for k in range(6):
            c.frame(trig=1 if k == 0 else 0)
        rec = []
        for k in range(300):
            r = c.frame(trig=2 if k == 0 else 0)
            rec.append(abs(r["level"]) / 8388608.0)
        f = DEC[rel] if rel < 127 else -8388608
        factor = abs(f) / 8388608.0
        # frames to 0.5
        n = None
        base = rec[0]
        for k, lv in enumerate(rec):
            if lv < 0.5 * rec[0]:
                n = k
                break
        out["rel%d" % rel] = dict(factor=factor, frames_to_half=n)
    # kill (trig=3): fixed index $20 = 32
    c = EnvCase(atk=0, dec=127, vol=127)
    c.e.Y[P + 0x05] = 0x7F << 16
    for k in range(6):
        c.frame(trig=1 if k == 0 else 0)
    rec = []
    for k in range(60):
        r = c.frame(trig=3 if k == 0 else 0)
        rec.append((abs(r["level"]) / 8388608.0, r["phase"]))
    out["kill"] = dict(first10=rec[:10], factor_idx32=abs(DEC[32]) / 8388608.0)
    return out

def retrigger():
    c = EnvCase(atk=32, dec=100, rel=40, vol=127)
    c.e.Y[P + 0x05] = 0x7F << 16
    rec = []
    for k in range(80):
        trig = 0
        if k == 0: trig = 1
        if k == 40: trig = 1   # retrigger mid-decay
        r = c.frame(trig=trig)
        rec.append((k, r["phase"], r["level"] / 8388608.0))
    return rec

if __name__ == "__main__":
    res = {}
    print("attack sweep..."); res["attack"] = sweep_attack()
    for k, v in res["attack"].items():
        print("  ATK=%3s frames=%s ms=%s (pred %.1f)" % (k, v["frames"], v["ms"], v["predicted"]))
    print("decay sweep...");  res["decay"] = sweep_decay()
    for k, v in res["decay"].items():
        print("  DEC=%3s factor=%.5f t_half=%s frames" % (k, v["factor"], v["t_half"]))
    print("hold sweep...");   res["hold"] = sweep_hold()
    print(res["hold"])
    print("release/kill..."); res["release"] = release_and_kill()
    print(res["release"])
    print("retrigger...");    res["retrigger"] = retrigger()
    json.dump(res, open("/home/z/my-project/mining/amp_env_sweep.json", "w"), indent=1)
    print("saved /home/z/my-project/mining/amp_env_sweep.json")
