#!/usr/bin/env python3
"""Fixed Table Extractor — fixes audit issue 5.1.
Bug: extract_tables.py did off = addr - 0x100000 for SRAM >= $100000,
reading zeros at $001A7B/$044AC7 instead of $101A7B/$144AC7.
Fix: word_offset = addr * 3 (24-bit LE words) in flat dsp1_pmem.bin."""
from pathlib import Path

VERIFIED_TABLES = [
    ("P_141a80_FM_ratios_24w",        0x141A80, 24),
    ("P_144ac7_LP_TONE_shared_258w",  0x144AC7, 258),
    ("Y_141800_LFO_AMP_rates_128w",   0x141800, 128),
    ("P_141880_FMDYN_wavetable_512w", 0x141880, 512),
    ("X_140000_Pitch_exp_2048w",      0x140000, 2048),
    ("P_101a7b_SWAVE_wavetable_main", 0x101A7B, 128),
    ("P_101bfb_SWAVE_PCH2_table",     0x101BFB, 128),
    ("P_101cfb_SWAVE_PCH3_table",     0x101CFB, 128),
]

def extract_word_le24(buf: bytes, word_addr: int) -> int:
    o = word_addr * 3  # NO -0x100000 subtraction!
    return buf[o] | (buf[o+1] << 8) | (buf[o+2] << 16)

if __name__ == "__main__":
    print("Extract non-zero DSP56303 tables directly from dsp1_pmem.bin.")
