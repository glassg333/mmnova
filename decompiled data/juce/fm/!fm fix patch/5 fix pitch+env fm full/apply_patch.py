#!/usr/bin/env python3
"""apply_patch.py — pack 7 installer (FM plugin-level: envelopes, gain path,
pitch law, 16-sample frame-rate fix).

Accepts: the Source dir, the Monomachine_Nova_Synth dir, or the repo root.
Installs 8 files into <Source>/dsp/mnm/, self-checks afterwards.
"""
import os, shutil, sys, hashlib

FILES = [
    "MnmFm.hpp", "MnmAmpEnv.hpp", "MnmAmpEnvTables.h",
    "MnmFmStat.hpp", "MnmFmPar.hpp", "MnmFmDyn.hpp",
    "MnmFmDsp.hpp", "MnmFmSineTable.h",
]

def find_source(start):
    p = os.path.abspath(start)
    for cand in (p, os.path.dirname(p), os.path.dirname(os.path.dirname(p))):
        if os.path.isdir(os.path.join(cand, "Source", "JuceLibraryCode")) or \
           os.path.isfile(os.path.join(cand, "Source", "PluginProcessor.cpp")):
            return os.path.join(cand, "Source")
    # maybe already inside Source/
    if os.path.isfile(os.path.join(p, "PluginProcessor.cpp")):
        return p
    return None

def main():
    here = os.path.dirname(os.path.abspath(__file__))
    src_dir = here if os.path.isdir(os.path.join(here, "dsp", "mnm")) else \
        os.path.join(here, "dsp", "mnm") and here
    if not os.path.isdir(os.path.join(here, "dsp", "mnm")):
        print("run from the pack root (dsp/mnm must exist)"); return 2
    target_root = sys.argv[1] if len(sys.argv) > 1 else os.getcwd()
    src = find_source(target_root)
    if src is None:
        print("cannot locate Source/ under %s" % target_root); return 2
    dst = os.path.join(src, "dsp", "mnm")
    os.makedirs(dst, exist_ok=True)
    ok = 0
    for f in FILES:
        s = os.path.join(here, "dsp", "mnm", f)
        d = os.path.join(dst, f)
        shutil.copyfile(s, d)
        h1 = hashlib.sha256(open(s, "rb").read()).hexdigest()
        h2 = hashlib.sha256(open(d, "rb").read()).hexdigest()
        mark = "OK" if h1 == h2 else "FAIL"
        ok += (h1 == h2)
        print("  %-22s -> %s [%s]" % (f, d, mark))
    print("%d/%d installed" % (ok, len(FILES)))
    return 0 if ok == len(FILES) else 1

if __name__ == "__main__":
    sys.exit(main())
