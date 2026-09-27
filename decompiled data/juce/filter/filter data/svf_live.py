#!/usr/bin/env python3
"""svf_live.py — make the GND-SIN chain live: set machine param1 (SPEED) so the
kernel's inline sine oscillator produces audio, then trace cascade-2 taps.
Sweeps a few SPEED values, measures master amp, dumps the tap/coefficient state.
"""
import sys, json, math
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase, P

def run(speed_frac, nframes=6, base=64):
    c = EnvCase(atk=0, dec=127, vol=127)
    e = c.e
    # machine params: $05 = SPEED (param1), $06 = param2 (level-ish?)
    e.Y[P + 0x05] = int(speed_frac * 8388608) & 0xFFFFFF
    e.Y[P + 0x06] = int(0.5 * 8388608) & 0xFFFFFF
    amps = []
    for k in range(nframes):
        r = c.frame(trig=1 if k == 0 else 0)
        amps.append(r["amp"] / 8388608.0)
    return c, amps

if __name__ == "__main__":
    for s in (0.03125, 0.0625, 0.125, 0.25, 0.5):
        c, amps = run(s)
        e = c.e
        taps = [e.X[0x80 + i] for i in range(17)]
        coeffs = [e.Y[0x80 + i] for i in range(17)]
        print("speed=%.5f amp[last]=%.5f amps=%s" %
              (s, amps[-1], ["%.3f" % a for a in amps[-3:]]))
        print("   taps X:$80-90:", " ".join("%X" % (t & 0xFFFFFF) for t in taps))
        print("   coef Y:$80-90:", " ".join("%06X" % (t & 0xFFFFFF) for t in coeffs[:8]))
