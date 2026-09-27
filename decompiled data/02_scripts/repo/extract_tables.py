#!/usr/bin/env python3
"""
Extract data tables referenced by the missing FX/synth machines.

Sources:
  - /home/z/my-project/work/mmnova/decompiled data/dsp1_pmem.bin  (24-bit BE P-memory)
  - /home/z/my-project/work/mmnova/decompiled data/dsp1_xmem.bin  (24-bit BE X-memory)
  - /home/z/my-project/work/mmnova/decompiled data/dsp1_ymem.bin  (24-bit BE Y-memory)
  - Disassembly listings to identify table base addresses and lengths

Tables to extract (identified by scanning disasm of FX-REV, FX-EQ, FX-DYN, FX-RING,
FX-PHA, FX-FLA, m32, m33, and the existing FM/CHORUS listings):

  LP filter coefficient table (shared)        P:$144AC7  258 words
  Phaser all-pass / delay table               P:$141900  256 words
  Phaser 2nd table                            P:$141980  128 words
  Phaser 3rd table                            P:$141A00  ~128 words
  Phaser 4th table                            P:$141B18  ~128 words
  Phaser helper                               P:$142F06  ~64 words
  Phaser coefficient                          P:$143546  ~64 words
  FX-DYN attack table                         P:$1444C6  ~128 words
  FX-DYN release table                        P:$1446C6  ~128 words
  FX-DYN RMS table                            P:$144746  ~64 words
  Reverb decay table                          P:$123EF5  ~256 words (in P-image)
  Reverb impulse table                        P:$133191  ~256 words (in P-image)
  Width/Mod table (chorus, shared)            X:$14A000  2048 words
  Width/Mod table 2 (chorus)                  X:$14A800  2048 words
  Delay line buffer (chorus, flanger, m33)    X:$114000  4096 words
  Reverb delay buffer                         X:$120000  ~8192 words
  EQ band table                               X:$140000  2048 words
  EQ helper table                             X:$1407FF  ~1 word (boundary)
  DPRO wavetable                              X:$11B451  ~2048 words
  SWAVE wavetable                             X:$101B7B  ~128 words (in P-image, near $101BFB)
  SWAVE PCH2 table                            X:$101BFB  ~128 words
  SWAVE PCH3 table                            X:$101CFB  ~128 words
  SWAVE PCH4 table                            X:$101D7B  ~128 words (used by DPRO)
  LFO sine wave (shared, FM machine)          Y:$0132    512+2 words (already extracted)
  LFO triangle / saw wave                     Y:$0334    256 words
  Engine pitch constant                       Y:$124     1 word (already known)

The output is a JSON manifest + raw binary dumps (one .bin per table, big-endian 24-bit words).
"""
import os, json, struct

PMEM = "/home/z/my-project/work/mmnova/decompiled data/dsp1_pmem.bin"
XMEM = "/home/z/my-project/work/mmnova/decompiled data/dsp1_xmem.bin"
YMEM = "/home/z/my-project/work/mmnova/decompiled data/dsp1_ymem.bin"
OUT = "/home/z/my-project/work/extract/tables"
os.makedirs(OUT, exist_ok=True)

def read_words_24(path):
    """Read a 24-bit-BE-word memory image into a list of ints."""
    data = open(path, "rb").read()
    return [int.from_bytes(data[i:i+3], "big") for i in range(0, len(data) - 2, 3)]

def words_to_hex(words, per_line=8):
    lines = []
    for i in range(0, len(words), per_line):
        chunk = words[i:i+per_line]
        lines.append(f"  +${i:04x}: " + " ".join(f"{w:06x}" for w in chunk))
    return "\n".join(lines)

def extract_from_pmem(pmem_words, base_addr, length, name, notes=""):
    """Extract `length` words starting at P:$base_addr from the P-memory image.
    The P-memory image is a flat array of 1,343,588 words; addresses P:$100000+
    map to the image directly (offset = base_addr).
    """
    if base_addr >= 0x100000:
        off = base_addr - 0x100000
    else:
        # Low P-memory (kernel): also at start of image
        off = base_addr
    if off + length > len(pmem_words):
        length = len(pmem_words) - off
        print(f"  WARN: {name} truncated to {length} words")
    chunk = pmem_words[off:off+length]
    # write .bin (24-bit BE)
    binpath = os.path.join(OUT, f"P_{base_addr:06x}_{name}.bin")
    with open(binpath, "wb") as f:
        for w in chunk:
            f.write(w.to_bytes(3, "big"))
    # write .txt hex dump
    txtpath = os.path.join(OUT, f"P_{base_addr:06x}_{name}.txt")
    with open(txtpath, "w") as f:
        f.write(f"# Table: {name}\n")
        f.write(f"# Source: P-memory image dsp1_pmem.bin\n")
        f.write(f"# Base address: P:${base_addr:06x}\n")
        f.write(f"# Length: {length} words ({length*3} bytes)\n")
        if notes:
            f.write(f"# Notes: {notes}\n")
        f.write(f"#\n")
        f.write(words_to_hex(chunk))
        f.write("\n")
    print(f"  P:${base_addr:06x}  {name:<30s}  {length:5d} words  -> {os.path.basename(binpath)}")
    return {"region": "P", "base": f"${base_addr:06x}", "length": length, "name": name, "notes": notes,
            "bin": os.path.basename(binpath), "txt": os.path.basename(txtpath)}

def extract_from_xmem(xmem_words, base_addr, length, name, notes=""):
    """Extract from X-memory. X-memory image has 1,044 words for addresses X:$0000..$0413.
    For higher addresses (X:$100000+), we'd need a different mapping, but those addresses
    aren't in the image (they're runtime buffers).
    """
    # X-memory image only covers $0000..$0413 (low memory)
    # For runtime buffers like X:$114000, X:$14A000 etc, we can't extract from image
    if base_addr > 0x100000:
        # runtime buffer, no static image available
        print(f"  SKIP X:${base_addr:06x}  {name}: runtime buffer (no static image)")
        return None
    if base_addr + length > len(xmem_words):
        length = len(xmem_words) - base_addr
        print(f"  WARN: {name} truncated to {length} words")
    chunk = xmem_words[base_addr:base_addr+length]
    binpath = os.path.join(OUT, f"X_{base_addr:06x}_{name}.bin")
    with open(binpath, "wb") as f:
        for w in chunk:
            f.write(w.to_bytes(3, "big"))
    txtpath = os.path.join(OUT, f"X_{base_addr:06x}_{name}.txt")
    with open(txtpath, "w") as f:
        f.write(f"# Table: {name}\n")
        f.write(f"# Source: X-memory image dsp1_xmem.bin\n")
        f.write(f"# Base address: X:${base_addr:06x}\n")
        f.write(f"# Length: {length} words ({length*3} bytes)\n")
        if notes:
            f.write(f"# Notes: {notes}\n")
        f.write("#\n")
        f.write(words_to_hex(chunk))
        f.write("\n")
    print(f"  X:${base_addr:06x}  {name:<30s}  {length:5d} words  -> {os.path.basename(binpath)}")
    return {"region": "X", "base": f"${base_addr:06x}", "length": length, "name": name, "notes": notes,
            "bin": os.path.basename(binpath), "txt": os.path.basename(txtpath)}

def extract_from_ymem(ymem_words, base_addr, length, name, notes=""):
    if base_addr + length > len(ymem_words):
        length = len(ymem_words) - base_addr
        print(f"  WARN: {name} truncated to {length} words")
    chunk = ymem_words[base_addr:base_addr+length]
    binpath = os.path.join(OUT, f"Y_{base_addr:06x}_{name}.bin")
    with open(binpath, "wb") as f:
        for w in chunk:
            f.write(w.to_bytes(3, "big"))
    txtpath = os.path.join(OUT, f"Y_{base_addr:06x}_{name}.txt")
    with open(txtpath, "w") as f:
        f.write(f"# Table: {name}\n")
        f.write(f"# Source: Y-memory image dsp1_ymem.bin\n")
        f.write(f"# Base address: Y:${base_addr:06x}\n")
        f.write(f"# Length: {length} words ({length*3} bytes)\n")
        if notes:
            f.write(f"# Notes: {notes}\n")
        f.write("#\n")
        f.write(words_to_hex(chunk))
        f.write("\n")
    print(f"  Y:${base_addr:06x}  {name:<30s}  {length:5d} words  -> {os.path.basename(binpath)}")
    return {"region": "Y", "base": f"${base_addr:06x}", "length": length, "name": name, "notes": notes,
            "bin": os.path.basename(binpath), "txt": os.path.basename(txtpath)}

def main():
    pmem = read_words_24(PMEM)
    xmem = read_words_24(XMEM)
    ymem = read_words_24(YMEM)
    print(f"P-memory: {len(pmem)} words")
    print(f"X-memory: {len(xmem)} words")
    print(f"Y-memory: {len(ymem)} words")
    print()

    tables = []

    # ==== P-memory tables ====
    print("=== P-memory tables ===")

    # LP filter coefficient table, shared by FM TONE + CHORUS LP + FX-EQ LP
    tables.append(extract_from_pmem(pmem, 0x144AC7, 258, "LP_filter_coeffs",
        "Shared lowpass coefficient table; FM/CHORUS/EQ TONE param indexes into this. 258 words = 1 + 128 entries * 2 (or similar)."))

    # Phaser all-pass tables (4 stages)
    tables.append(extract_from_pmem(pmem, 0x141900, 128, "PHASER_stage0_table",
        "FX-PHASER stage-0 all-pass coefficient table (referenced by m18)."))
    tables.append(extract_from_pmem(pmem, 0x141980, 128, "PHASER_stage1_table",
        "FX-PHASER stage-1 all-pass coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x141A00, 128, "PHASER_stage2_table",
        "FX-PHASER stage-2 all-pass coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x141B18, 64, "PHASER_stage3_table",
        "FX-PHASER stage-3 all-pass coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x142F06, 64, "PHASER_helper_table",
        "FX-PHASER helper/modulation table."))
    tables.append(extract_from_pmem(pmem, 0x143546, 64, "PHASER_cntr_table",
        "FX-PHASER CNTR parameter mapping table."))

    # FX-DYN (compressor) attack/release/RMS tables
    tables.append(extract_from_pmem(pmem, 0x1444C6, 128, "FXDYN_attack_table",
        "FX-DYNAMIX ATK parameter curve / attack coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x1446C6, 128, "FXDYN_release_table",
        "FX-DYNAMIX REL parameter curve / release coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x144746, 64, "FXDYN_rms_table",
        "FX-DYNAMIX RMS window / RMS averaging coefficient table."))

    # FX-REV (reverb) tables in P-memory
    tables.append(extract_from_pmem(pmem, 0x123EF5, 256, "FXREV_decay_table",
        "FX-REVERB DEC (decay) parameter coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x133191, 256, "FXREV_damp_table",
        "FX-REVERB DAMP parameter coefficient table."))

    # FX-EQ / SWAVE band tables
    tables.append(extract_from_pmem(pmem, 0x101A7B, 128, "SWAVE_wavetable_main",
        "SWAVE-SAW/PULS main wavetable (referenced by m4, m5, m14)."))
    tables.append(extract_from_pmem(pmem, 0x101AFB, 128, "SWAVE_PCH2_table",
        "SWAVE PCH2 pitch table (referenced by m4, m14, m33)."))
    tables.append(extract_from_pmem(pmem, 0x101B7B, 128, "SWAVE_PCH3_table",
        "SWAVE PCH3 pitch table (referenced by m4)."))
    tables.append(extract_from_pmem(pmem, 0x101BFB, 128, "SWAVE_PCH4_table",
        "SWAVE PCH4 pitch table (referenced by m4, m5, m32, m33)."))
    tables.append(extract_from_pmem(pmem, 0x101CFB, 128, "SWAVE_PCH5_table",
        "SWAVE PCH5 pitch table (referenced by m5, m14, m32, m33)."))
    tables.append(extract_from_pmem(pmem, 0x101D7B, 128, "DPRO_wavetable_ref",
        "DPRO-WAVE wavetable reference (referenced by m6)."))

    # DPRO / BBOX tables
    tables.append(extract_from_pmem(pmem, 0x10750C, 256, "DPRO_BBOX_table1",
        "DPRO-BBOX table 1 (referenced by m7)."))
    tables.append(extract_from_pmem(pmem, 0x10B1BC, 256, "DPRO_BBOX_table2",
        "DPRO-BBOX table 2."))
    tables.append(extract_from_pmem(pmem, 0x10B5C7, 256, "GND_NOIS_table",
        "GND-NOIS noise color table (referenced by m2)."))
    tables.append(extract_from_pmem(pmem, 0x10F7A1, 256, "DPRO_BBOX_table3",
        "DPRO-BBOX table 3."))
    tables.append(extract_from_pmem(pmem, 0x11320F, 256, "DPRO_BBOX_table4",
        "DPRO-BBOX table 4."))
    tables.append(extract_from_pmem(pmem, 0x11B451, 256, "DPRO_wavetable_main",
        "DPRO-WAVE main wavetable (referenced by m6, m7)."))

    # VO-6 tables
    tables.append(extract_from_pmem(pmem, 0x1001D1, 256, "VO6_dispatch_table",
        "VO-6 internal dispatch/wavetable region (referenced by m11)."))
    tables.append(extract_from_pmem(pmem, 0x1007F1, 256, "VO6_table2",
        "VO-6 helper table."))

    # SID wavetables
    tables.append(extract_from_pmem(pmem, 0x140000, 256, "SID_wavetable_0",
        "SID-6581 wavetable 0 (referenced by m3, m4, m14)."))
    tables.append(extract_from_pmem(pmem, 0x140800, 256, "SID_wavetable_1",
        "SID-6581 wavetable 1."))
    tables.append(extract_from_pmem(pmem, 0x1435C6, 128, "SID_helper_table",
        "SID-6581 helper table (referenced by m3, m4, m5, m14, m17)."))
    tables.append(extract_from_pmem(pmem, 0x143D06, 128, "SID_filter_table",
        "SID-6581 filter coefficient table (referenced by m3, m4, m5, m14, m17)."))

    # m32 / m33 helper tables (these are likely DELAY/EXT machines)
    tables.append(extract_from_pmem(pmem, 0x144C49, 128, "m32_helper_table",
        "m32 (slot 32) helper coefficient table."))
    tables.append(extract_from_pmem(pmem, 0x145C48, 128, "m33_helper_table",
        "m33 (slot 33) helper coefficient table."))

    # ==== Y-memory tables ====
    print("\n=== Y-memory tables ===")
    # Sine table (FM) - already extracted in original pack, but include for completeness
    tables.append(extract_from_ymem(ymem, 0x0132, 512, "sine_table_FM",
        "FM sine lookup table with linear interpolation (mask $1FFF). Already in original pack."))
    tables.append(extract_from_ymem(ymem, 0x0334, 256, "LFO_wave_triangle",
        "LFO secondary waveform table (triangle/saw) at Y:$0334, used by FM modulators and FX LFOs."))

    # ==== X-memory low tables ====
    print("\n=== X-memory tables ===")
    tables.append(extract_from_xmem(xmem, 0x02C0, 4, "audio_buffer_bases",
        "Base pointers for audio I/O buffers (X:$02C0..$02C3). DSP1 and DSP2 differ."))
    tables.append(extract_from_xmem(xmem, 0x0394, 32, "pitch_table_1",
        "Pitch conversion table 1 (X:$0394..$03B3)."))
    tables.append(extract_from_xmem(xmem, 0x03F4, 32, "pitch_table_2",
        "Pitch conversion table 2 (X:$03F4..$0413)."))

    # Save manifest
    manifest = [t for t in tables if t]
    json.dump(manifest, open(os.path.join(OUT, "TABLES_MANIFEST.json"), "w"), indent=2)
    print(f"\nTABLES_MANIFEST.json written ({len(manifest)} tables)")
    print(f"Output dir: {OUT}")

if __name__ == "__main__":
    main()
