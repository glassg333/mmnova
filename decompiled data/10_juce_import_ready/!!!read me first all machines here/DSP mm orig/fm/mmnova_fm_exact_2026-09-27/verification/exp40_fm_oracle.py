#!/usr/bin/env python3
"""
exp40_fm_oracle.py — FM+STAT (m8) oracle: run the REAL firmware process function
P:$145D21-$145EC8 in the DSP56300 emulator and measure the exact knob laws.

Inputs set exactly like the kernel does:
  r6 = voice page (0x400), knobs as knob<<16 at y:(r6+$4..$B)
  A  = pitch word on entry (from X:$140000 pitch table, 2048 steps/octave)
  init ($145D12) and config ($145D1D) executed first, like the dispatch does.

Outputs: 32 words via r7 (16 L/R pairs).
"""
import sys, os, math, json
sys.path.insert(0, "/home/z/my-project/scripts")
from dsp_emu import DSP56300

REPO = "/home/z/my-project/work/mmnova_github"
DD = REPO + "/decompiled data"
LST = DD + "/03_listings/machines_page_A/08_FM-STAT_full.txt"
PM = DD + "/02_memory_images/dsp1_pmem.bin"
TBL = DD + "/04_tables"

PAGE = 0x400          # r6 voice page
OUTP = 0x700          # r7 output scratch (Y)
SENTINEL = 0x0F0F0F   # fake return address
ENTRY = 0x145D21

def load_tables():
    """Return dict of tables from the firmware dumps."""
    d = open(PM, "rb").read()
    def w(addr):
        o = addr*3
        return (d[o] << 16) | (d[o+1] << 8) | d[o+2]
    ratio = [w(0x141A80+i) for i in range(24)]
    tone  = [w(0x144AC7+i) for i in range(128)]
    pd = open(TBL + "/X_140000_pitch_wavetable_2048.bin", "rb").read()
    pitch = [(pd[3*i] << 16) | (pd[3*i+1] << 8) | pd[3*i+2] for i in range(2048)]
    return {"ratio": ratio, "tone": tone, "pitch": pitch}

def synth_sine():
    """8192-point sine in S1.23 — the $14A000 table is outside every available
    dump; the emulator oracle (and the C++ port) use the identical synthesized
    table, so all comparisons below remain internally exact."""
    t = []
    for i in range(8192):
        v = int(round(math.sin(2*math.pi*i/8192) * 8388607))
        t.append(v & 0xFFFFFF)
    return t

INIT_LST = """145d12: lua      (r6+$11),r0                                 ; 040E10
145d13: move     #>$14a000,x0                                ; 44F400 14A000
145d15: move     #$0,x1                                      ; 250000
145d16: do       #<$18,>$145d19                              ; 061880 145D18
145d18: move     x1,y:(r0)+                                  ; 4D5800
145d19: move     x0,y:(r6+$10)                               ; 0246A4
145d1a: move     x0,y:(r6+$14)                               ; 0256A4
145d1b: move     x0,y:(r6+$16)                               ; 025EA4
145d1c: rts                                                  ; 00000C
145d1d: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145d1f: move     a,y:(r6+$2a)                                ; 02AEAE
145d20: rts                                                  ; 00000C"""

def build():
    lines = INIT_LST.splitlines() + open(LST).read().splitlines()
    e = DSP56300(lines)
    # unified SRAM alias: P-dump visible in X and Y (same trick as run_kernel.py)
    data = open(PM, "rb").read()
    n = len(data)//3
    for i in range(0, n):
        o = i*3
        wv = (data[o] << 16) | (data[o+1] << 8) | data[o+2]
        e.X[i] = wv
        e.Y[i] = wv
    sine = synth_sine()
    for i, v in enumerate(sine):
        e.X[0x14A000+i] = v
        e.Y[0x14A000+i] = v
    return e

def call(e, addr):
    e.ret_stack.append(SENTINEL)
    e.run(addr, end=SENTINEL)

def init_page(e):
    # INIT  ($145D12): phase accs = table base, states 0  (r6 = voice page!)
    e.R[6] = PAGE
    call(e, 0x145D12)
    # CONFIG ($145D1D): y:$2a = $7fffff
    call(e, 0x145D1D)

def set_knobs(e, knobs):
    """knobs: list of 8 values 0..127 -> knob<<16 at y:(r6+$4..$B). x too (some reads?)."""
    for i, k in enumerate(knobs):
        e.Y[PAGE + 4 + i] = (k & 0xFF) << 16
        e.X[PAGE + 4 + i] = (k & 0xFF) << 16

def call_process(e, pitch_word):
    e.R[6] = PAGE
    e.R[7] = OUTP
    # A = pitch word (24-bit)
    e.A = pitch_word & 0xFFFFFF
    call(e, ENTRY)

def read_out(e):
    return [e.Y[OUTP+i] for i in range(32)]

def to_signed(v):
    return v - 0x1000000 if v & 0x800000 else v

def dominant_freq(sig, sr=44100.0):
    """FFT peak of a long signal (list of floats)."""
    N = len(sig)
    w = [0.5 - 0.5*math.cos(2*math.pi*i/N) for i in range(N)]
    x = [sig[i]*w[i] for i in range(N)]
    # coarse FFT via numpy if available
    try:
        import numpy as np
        X = np.abs(np.fft.rfft(np.array(x)))
        k = int(np.argmax(X[1:])) + 1
        return k * sr / N
    except ImportError:
        # brute force top frequency
        best, bf = 0.0, 0
        for k in range(1, N//2):
            re = sum(x[i]*math.cos(2*math.pi*k*i/N) for i in range(0, N, 2))
            im = sum(x[i]*math.sin(2*math.pi*k*i/N) for i in range(0, N, 2))
            p = re*re + im*im
            if p > best:
                best, bf = p, k
        return bf * sr / (N/1.0)

def run_long(e, knobs, pitch_word, blocks=16):
    """Run N blocks, collect mono output (L), phases carry over in the page."""
    out = []
    for _ in range(blocks):
        call_process(e, pitch_word)
        out += [to_signed(v)/8388608.0 for v in read_out(e)[0::2]]  # L channel
    return out

if __name__ == "__main__":
    tabs = load_tables()
    print("ratio table [0,11,15,23]:", [hex(tabs['ratio'][i]) for i in (0,11,15,23)])
    print("tone table [0,64,127]:", [hex(tabs['tone'][i]) for i in (0,64,127)])
    print("pitch [0,1024,2047]:", [hex(tabs['pitch'][i]) for i in (0,1024,2047)])

    def snap(e):
        return {nm: (e.Y[PAGE+hi] << 24) | e.Y[PAGE+lo]
                for nm, hi, lo in [("ph1", 0x10, 0x11), ("phM", 0x14, 0x15),
                                   ("ph2", 0x16, 0x17), ("phC", 0x1C, 0x1D)]}

    # --- pitch law: phase increment per block for each accumulator ---
    print("\n=== A sweep: phase inc per block (units/sample = delta/32/2^24) ===")
    for a in [2944, 5888, 11776, 23552, 47104, 94208, 188416]:
        e = build(); init_page(e)
        set_knobs(e, [15,64,100,0, 15,100,127,64])
        s0 = snap(e)
        call_process(e, a)
        s1 = snap(e)
        row = {nm: (s1[nm]-s0[nm]) for nm in s0}
        row = {nm: (v & ((1<<48)-1)) for nm, v in row.items()}
        def units(d48): return d48 / 2**24 / 32.0
        print(f"A={a:7d} ($({a:06X})): " +
              "  ".join(f"{nm}={units(v):10.4f}u/s f={units(v)*44100/8192:8.1f}Hz" for nm, v in row.items()))

