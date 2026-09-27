#!/usr/bin/env python3
"""exp46_reverb_tables.py — extract m13 FX-REVERB ROM tables -> C header."""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R

e = R.build()

def dump(name, base, n):
    vals = [int(e.X.get(base + i, 0)) for i in range(n)]
    return name, base, vals

tables = [
    ("tbl_reverb_ap_coef_14561c", 0x14561C, 5),    # allpass coefficients
    ("tbl_reverb_ap_mask_145621", 0x145621, 5),    # allpass masks (len-1)
    ("tbl_reverb_comb_mask_145626", 0x145626, 6),  # comb masks (len-1)
    ("tbl_reverb_dec_14562c", 0x14562C, 200),      # DEC coefficient table (64x3 + spill)
    ("tbl_reverb_damp_1457b2", 0x1457B2, 200),     # DAMP coefficient table (64x3 + spill)
]

out = []
out.append("// mnm_reverb_tables.h — Monomachine OS 1.32B DSP1 ROM tables for FX-REVERB m13")
out.append("// extracted from dsp1_pmem.bin (sha-verified tree); PROC $1453DC-$14561B")
out.append("#pragma once")
out.append("#include <cstdint>")
out.append("namespace mnm { namespace rv {")
for name, base, n in tables:
    _, _, vals = dump(name, base, n)
    out.append("")
    out.append("// %s: $%X (%d words)" % (name, base, n))
    out.append("static const uint32_t %s[%d] = {" % (name, len(vals)))
    for i in range(0, len(vals), 8):
        row = ", ".join("0x%06X" % v for v in vals[i:i+8])
        out.append("  " + row + ",")
    out.append("};")
out.append("")
out.append("}} // namespace")
print("\n".join(out))
