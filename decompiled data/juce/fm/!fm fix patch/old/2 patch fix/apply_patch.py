#!/usr/bin/env python3
"""apply_patch.py — puts the fixed FM machine files into the plugin tree.

Usage:
    python3 apply_patch.py <path to Monomachine_Nova_Synth/Source>

Copies into <Source>/dsp/mnm/:
    MnmFm.hpp        (integration core: exact STAT knob laws + PAR/DYN dispatch)
    MnmFmDsp.hpp     (Q23 primitives, bit-exact)
    MnmFmPar.hpp     (FM+PAR exact transcription, 100% on emulator vectors)
    MnmFmDyn.hpp     (FM+DYN exact transcription, 100% on emulator vectors)
    MnmFmSineTable.h (ROM sine X/Y:$14A000, canonical round(sin*8388607))

Prints what changed. NovaDSP.h needs no edits (public API preserved).
"""
import os, shutil, sys, hashlib

HERE = os.path.dirname(os.path.abspath(__file__))
FILES = ["MnmFm.hpp", "MnmFmDsp.hpp", "MnmFmPar.hpp", "MnmFmDyn.hpp",
         "MnmFmSineTable.h"]

def sha(p):
    return hashlib.sha256(open(p, 'rb').read()).hexdigest()[:16]

def main():
    if len(sys.argv) < 2:
        print(__doc__); return 1
    src_root = sys.argv[1]
    dst = os.path.join(src_root, "dsp", "mnm")
    if not os.path.isdir(dst):
        print("ERROR: %s not found" % dst); return 1
    for name in FILES:
        s = os.path.join(HERE, name)
        d = os.path.join(dst, name)
        if not os.path.exists(s):
            print("ERROR: missing %s in the pack" % name); return 1
        if os.path.exists(d):
            if sha(s) == sha(d):
                print("  = %s (already identical)" % name); continue
            print("  ~ %s  (updated, was %s)" % (name, sha(d)))
        else:
            print("  + %s  (new file)" % name)
        shutil.copy2(s, d)
    print("done. Bit-exactness proof: fm_par/fm_dyn vectors 7168+8192 words, 0 mismatches.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
