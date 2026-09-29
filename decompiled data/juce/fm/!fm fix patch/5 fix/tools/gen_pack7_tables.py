#!/usr/bin/env python3
"""gen_pack7_tables.py — extract OS 1.32 tables for the FM plugin-level pack:
  P:$141800  AMP/FLT attack increments  (128 words, Q23)
  P:$141880  AMP/FLT decay factors      (128 words, Q23, negative)
  P:$140000  pitch conversion table     (2048 words, Q22, 2^(i/2048))
Writes dsp/mnm/MnmAmpEnvTables.h
"""
PM = ("/home/z/my-project/.cache/relocated_from_workspace/mining/mmnova/"
      "decompiled data/02_memory_images/dsp1_pmem.bin")

data = open(PM, "rb").read()

def rd(i):
    o = i * 3
    return (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]

def sgn(v):
    return v - 0x1000000 if v >= 0x800000 else v

att = [rd(0x141800 + i) for i in range(128)]
dec = [sgn(rd(0x141880 + i)) for i in range(128)]
pit = [rd(0x140000 + i) for i in range(2048)]

def fmt(vals, per=6):
    lines = []
    for i in range(0, len(vals), per):
        lines.append("    " + ", ".join("%d" % v for v in vals[i:i+per]) + ",")
    return "\n".join(lines).rstrip(",")

hdr = """// =============================================================================
// MnmAmpEnvTables.h — OS 1.32B firmware tables, extracted from dsp1_pmem.bin
//
//   kMnmEnvAttack[128]   P:$141800  attack increments (Q23, positive)
//   kMnmEnvDecay[128]    P:$141880  decay/release factors (Q23, NEGATIVE)
//   kMnmPitchTable[2048] P:$140000  2^(i/2048) in Q22, used by the kernel
//                                   pitch word -> Hz conversion ($02DD-$02EA)
// =============================================================================
#pragma once
#include <cstdint>

namespace mnmfm {

inline constexpr int32_t kMnmEnvAttack[128] = {
%s
};

inline constexpr int32_t kMnmEnvDecay[128] = {
%s
};

inline constexpr uint32_t kMnmPitchTable[2048] = {
%s
};

}  // namespace mnmfm
""" % (fmt(att), fmt(dec), fmt(pit, 8))

out = "/home/z/my-project/work/pack7/dsp/mnm/MnmAmpEnvTables.h"
import os
os.makedirs(os.path.dirname(out), exist_ok=True)
open(out, "w").write(hdr)
print("wrote", out, len(hdr), "bytes;",
      "att[0]=%d att[32]=%d dec[0]=%d dec[127]=%d pit[0]=%d" % (
          att[0], att[32], dec[0], dec[127], pit[0]))
