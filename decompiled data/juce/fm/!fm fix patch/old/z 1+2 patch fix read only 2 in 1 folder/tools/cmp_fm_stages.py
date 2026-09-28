#!/usr/bin/env python3
"""cmp_fm_stages.py — diff emulator vs C++ stage dumps, first diverging key."""
import json, sys

def load_concat(path):
    txt = open(path).read()
    dec = json.JSONDecoder()
    out = {}
    i = 0
    while i < len(txt):
        while i < len(txt) and txt[i] in " \n\r\t":
            i += 1
        if i >= len(txt):
            break
        obj, j = dec.raw_decode(txt, i)
        i = j
        if isinstance(obj, list):
            out["out"] = obj          # trailing output array
        else:
            out[str(obj.get("_stage", len(out)))] = obj
    return out

which = sys.argv[1] if len(sys.argv) > 1 else "9"
if which == "9":
    emu = json.load(open("/home/z/my-project/work/fm_exact/emu_par_stages.json"))
    cpp = load_concat("/home/z/my-project/work/fm_exact/cpp_par_stages.json")
else:
    emu = json.load(open("/home/z/my-project/work/fm_exact/emu_dyn_stages.json"))
    cpp = load_concat("/home/z/my-project/work/fm_exact/cpp_dyn_stages.json")

for stage in sorted(emu, key=int):
    s = str(stage)
    e = dict(emu[s])
    if s == "15" and "out" in cpp:
        e = dict(e); e["out"] = e.get("out", cpp["out"])
    c = cpp.get(s)
    if c is None:
        c = cpp.get("out") if s == "15" else None
    if c is None:
        print("stage", s, ": missing in cpp"); continue
    bad = []
    for k in e:
        if k not in c:
            bad.append((k, "missing-in-cpp", None, None)); continue
        ev, cv = e[k], c[k]
        if isinstance(ev, list) or isinstance(cv, list):
            if ev != cv:
                for i in range(max(len(ev), len(cv))):
                    a = ev[i] if i < len(ev) else None
                    b = cv[i] if i < len(cv) else None
                    if a != b:
                        bad.append((k, "first diff at [%d]" % i, a, b))
                        break
        else:
            def norm(v):
                if isinstance(v, int):
                    return v & 0xFFFFFFFF
                return v
            if norm(ev) != norm(cv):
                bad.append((k, "value", "0x%06X/0x%08X" % (ev & 0xFFFFFF, ev & 0xFFFFFFFF) if isinstance(ev, int) else ev,
                            "0x%06X/0x%08X" % (cv & 0xFFFFFF, cv & 0xFFFFFFFF) if isinstance(cv, int) else cv))
    if bad:
        for k, msg, a, b in bad:
            print("stage %s key %-12s %s  emu=%s cpp=%s" % (s, k, msg, a, b))
        break
    else:
        print("stage %s: OK (%d keys)" % (s, len(e)))
