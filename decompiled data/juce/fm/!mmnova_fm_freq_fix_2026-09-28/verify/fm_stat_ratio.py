#!/usr/bin/env python3
"""fm_stat_ratio.py — THE decisive table measurement.
A=1 (no 48-bit wraps): per-sample increments of the modulator phase ($14:$15)
and the carrier phase ($16:$17) for every 1FRQ knob value.
ratio_shape(k) = d14(k)/d16 — structural constants cancel; the true firmware
ladder (1/64..4 vs 1/32..8) falls out directly.
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from dsp_emu import EmuError
import fm_stat_probe as P

def fill_sine(e):
    for n in range(8192):
        e.X[0x14A000 + n] = int(round(math.sin(2 * math.pi * n / 8192) * (1 << 23))) & 0xFFFFFF
import math

def main():
    e = P.build()
    fill_sine(e)
    A = 1
    print(f"A={A} (f_carrier={2*0xbe37c/2**35*44100:.5f} Hz), 1FIN=64, 1FB=0, 2VOL-block muted")
    print(f"{'knob':>4} {'d14/sample':>16} {'d14/d16':>12}  vs plugin kFmRatio[...]")
    rows = []
    for k in range(0, 128):
        P.set_params(e, frq=k, fin=64, p6=0, p7=0, frq2=64, p9=0, tone=0)
        try:
            o1, s1 = P.run_block(e, A)
            o2, s2 = P.run_block(e, A)
            o3, s3 = P.run_block(e, A)
        except EmuError as ex:
            print(f"{k:4d} EMU ERROR {ex}"); return
        d14 = P.sign48(s3["p14"] - s2["p14"]) / 32.0
        d16 = P.sign48(s3["p16"] - s2["p16"]) / 32.0
        rows.append((k, d14, d16))
    # print compact: only when the value changes (plateau detection)
    prev = None
    for k, d14, d16 in rows:
        if prev is None or abs(d14 - prev) > 1.0:
            print(f"{k:4d} {d14:16.1f} {d14/d16:12.6f}")
        prev = d14
    # distinct values
    vals = []
    for k, d14, d16 in rows:
        if not vals or abs(vals[-1][1] - d14) > 1.0:
            vals.append((k, d14 / d16))
    print(f"\ndistinct plateaus: {len(vals)}")
    print("plateau starts:", [(k, round(r, 6)) for k, r in vals])

if __name__ == "__main__":
    main()
