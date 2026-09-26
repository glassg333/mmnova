#!/usr/bin/env python3
"""
exp42_srr_echo.py — SRR effect on the ECHO path.
Activate the echo by setting the host length cell P+$2A and LPQ; impulse input;
sweep SRR ($51A). Compare: T4 publish window (Y:$4000+), stage-2 banks,
mix ramp Y:$10-$1F, and the master outputs. Look for decimation (held samples)
or pure envelope-gain differences.
"""
import sys, math
sys.path.insert(0, "/home/z/my-project/scripts")
import exp41_srr_test as X41

M24 = 0xFFFFFF

def sgn(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v

def run_case(srr, nframes=48, imp_frame=4):
    e = X41.build_track()
    Y, X = e.Y, e.X
    X41.setup(e, srr=srr, tim=40, wid=100, eqf=90, eqg=60)
    # host delay length cell P+$2A (48-bit L pair at P+$2A): length ~ 8 blocks
    Y[0x400 + 0x2A] = 0x000100   # high (int part)
    Y[0x400 + 0x2B] = 0x000000   # low (frac)
    # LPQ cell ($513) drives gate/tables: mid value
    Y[0x400 + 0x13] = (64 & 0xFFFF) << 16
    outs = []
    pub = []
    for k in range(nframes):
        Y[0x124] = 0
        Y[0x123] = 0x400 + 0x28
        Y[0x400 + 0x28] = 1 if k == 0 else 0
        e.R[6] = 0x400 + 0x28
        e.R[7] = 0x100
        # impulse into machine output stack (post-machine inject is not possible
        # in full-frame mode; instead force GND-SIN pitch high and use its output)
        e.run(0x0100, end=0x0B4C, max_steps=2_000_000)
        outs.append([e.X.get(a, 0) for a in range(0x260, 0x270)])
        pub.append([e.Y.get(0x4000 + i, 0) for i in range(16)])
    return outs, pub, e

if __name__ == "__main__":
    for srr in (0, 48, 127):
        outs, pub, e = run_case(srr)
        # echo publish ring Y:$4000+ after echo develops
        p = pub[30]
        s = [sgn(v) for v in p]
        rep = sum(1 for i in range(1, 16) if s[i] == s[i-1] and s[i] != 0)
        mx = max(abs(v) for v in s)
        nz = sum(1 for v in s if v)
        o = [sgn(v) for v in outs[30]]
        print(f"SRR={srr:3d}: pub[30] max={mx:8d} nz={nz:2d} repeats={rep}")
        print(f"   pub = {['%06X' % (v & M24) for v in p[:8]]}")
        print(f"   out = {['%+8d' % v for v in o[:8]]}")
        # envelope state
        print(f"   Y:4FF={e.Y.get(0x4FF,0):06X}  Y:10-1F ramp={[ '%06X'%e.Y.get(a,0) for a in range(0x10,0x14)]}")
