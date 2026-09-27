#!/usr/bin/env python3
"""exp48_reverb_intdump.py — per-frame internal-state dump of m13 FX-REVERB
from the emulator (reference). Frame-internal snapshot at key PCs, set A of
exp47 (dec=100 damp=2 gate=127 mix=127 inp=64), 48 frames.
Output: JSON to scripts/exp48_intdump_emu.json
"""
import sys, json

sys.path.insert(0, "/home/z/my-project/scripts")
import exp26_m13_reverb as R

M24 = 0xFFFFFF

# ---- replicate exp47 set A input ----
def frame_bus(k, seed=12345):
    s = [0] * 64
    if k < 3:
        v = (seed * 7919 + k * 104729) % 2000000 - 1000000
        for i in range(64):
            s[i] = int(v * (0.6 ** (i // 8))) & M24 if i < 32 else 0
    return s


def r6dump(e, offs):
    return {("%02X" % o): e.Y.get(R.R6M + o, 0) & M24 for o in offs}


def dump_Y(e, base, n):
    return [e.Y.get(base + i, 0) & M24 for i in range(n)]


def dump_X(e, base, n):
    return [e.X.get(base + i, 0) & M24 for i in range(n)]


R6_OFFS = [0x12, 0x13, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
           0x20, 0x21, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
           0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
           0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
           0x41, 0x42, 0x43, 0x47, 0x48]


def main():
    e = R.build()
    R.run_init(e)
    R.set_params(e, dec=100, damp=2, gate=127, mix=127, hp=8, lp=100, inp=64)
    R.warmup(e, 6)

    frames = []
    cur = {}
    orig_step = e.step

    def mk_dump():
        def dump(pc):
            if pc == 0x145445:      # after allpass cascade
                cur["ap_out"] = dump_Y(e, 0, 32)
                cur["x1g"] = e.get_reg("y1") & M24
            elif pc == 0x145493:    # after combs 1-4 taps
                cur["comb14"] = dump_Y(e, 0x20, 64)
            elif pc == 0x1454C5:    # after comb5 tap
                cur["comb5"] = dump_Y(e, 0x60, 16)
                cur["comb5_n1"] = e.get_reg("n1") & M24
                cur["comb5_y1"] = e.get_reg("y1") & M24
                cur["comb5_x0"] = e.get_reg("x0") & M24
                cur["comb5_r1"] = e.R[1] & M24
                cur["comb5_lfo"] = r6dump(e, [0x2A, 0x30, 0x37])
            elif pc == 0x1454DA:    # after comb6 tap
                cur["comb6"] = dump_Y(e, 0x70, 16)
                cur["comb6_n1"] = e.get_reg("n1") & M24
                cur["comb6_r1"] = e.R[1] & M24
                cur["comb6_lfo"] = r6dump(e, [0x2B, 0x31, 0x38])
                cur["lfo_phase"] = e.Y.get(R.R6M + 0x32, 0) & M24
            elif pc == 0x145547:    # after matrix
                cur["xbank"] = dump_X(e, 0, 0x80)
            elif pc == 0x145595:    # env -> attack path
                cur["env_branch"] = "atk"
                cur["rising_b"] = e.get_reg("b") & ((1 << 56) - 1)
                cur["env"] = r6dump(e, [0x1E, 0x1F, 0x20, 0x21, 0x25])
            elif pc == 0x14559F:    # env -> decay path
                cur["env_branch"] = "dec"
                cur["rising_b"] = e.get_reg("b") & ((1 << 56) - 1)
                cur["env"] = r6dump(e, [0x1E, 0x1F, 0x20, 0x21, 0x25])
            elif pc == 0x1455B4:    # after gate multiply
                cur["wet"] = dump_X(e, 0, 32)
                cur["ramp"] = dump_Y(e, 0, 32)
            return None
        return dump

    dump = mk_dump()

    def step():
        dump(e.pc)
        orig_step()

    e.step = step

    for f in range(48):
        cur = {}
        bus = frame_bus(f)
        o = R.run_block(e, bus)
        # end-of-frame state
        cur["out"] = [x & M24 for x in o]
        cur["r6end"] = r6dump(e, R6_OFFS)
        cur["bus"] = bus[:32]
        frames.append(cur)
        if f >= 36 or f <= 2:
            print(f"f{f:02d} branch={cur.get('env_branch')} "
                  f"out0={cur['out'][0]:06X} ramp0={cur.get('ramp',[0]*32)[0]:06X} "
                  f"qcnt={cur['r6end'].get('20', 0):06X}")

    e.step = orig_step
    json.dump(frames, open("/home/z/my-project/scripts/exp48_intdump_emu.json", "w"))
    print("dumped", len(frames), "frames")


if __name__ == "__main__":
    main()
