#!/usr/bin/env python3
"""exp25d_secdiff.py — find the FIRST diverging section between the emulator
   and the model inside one m18 block (deep-fast dc config)."""
import sys

sys.path.insert(0, "/home/z/my-project/scripts")
from exp25_verify import build, run_init, run_emu_block, R6M, M24  # noqa
from exp25_model import M18State, proc_model

STOP = {
    "head_gain_done": 0x145057,   # after qb banks
    "osc_done": 0x14506A,         # after carrier + phase update
    "wid_table_done": 0x14507A,   # after s_wid + table read
    "quad_ramp_done": 0x14508B,   # after X/Y:$E0 ramps
    "sideband_done": 0x14509C,    # after a/b banks
    "onepole_done": 0x1450B0,     # after s18/s19
    "vramp_done": 0x1450C6,       # after v + V-ramp
    "mod_done": 0x1450D5,         # after mod bank
    "fbramp_done": 0x1450E7,      # after FB ramp
    "pass1_done": 0x14511D,       # after pass-1 rotator loop
    "pass2_done": 0x145129,       # after pass 2 (loop end)
}


def dump(e, tag):
    sec = {
        "Y00": [e.Y.get(a, 0) for a in range(0x00, 0x20)],
        "X20_2F": [e.X.get(a, 0) for a in range(0x20, 0x30)],
        "Y20_2F": [e.Y.get(a, 0) for a in range(0x20, 0x30)],
        "XE0": [e.X.get(a, 0) for a in range(0xE0, 0xF0)],
        "YE0": [e.Y.get(a, 0) for a in range(0xE0, 0xF0)],
        "X30_3F": [e.X.get(a, 0) for a in range(0x30, 0x40)],
        "X00": [e.X.get(a, 0) for a in range(0x00, 0x20)],
        "Y90": [e.Y.get(a, 0) for a in range(0x90, 0xA0)],
        "X60_7F": [e.X.get(a, 0) for a in range(0x60, 0x80)],
        "ph": [e.X.get(R6M + 0x11, 0), e.X.get(R6M + 0x12, 0)],
        "s18": [e.X.get(R6M + 0x18, 0), e.Y.get(R6M + 0x18, 0)],
        "s19": [e.X.get(R6M + 0x19, 0), e.Y.get(R6M + 0x19, 0)],
        "swid": [e.Y.get(R6M + 0x1B, 0)],
        "v": [e.Y.get(R6M + 0x1A, 0)],
        "s13": [e.Y.get(R6M + 0x13, 0)],
        "q": [e.Y.get(R6M + 0x1C, 0), e.Y.get(R6M + 0x1D, 0)],
    }
    return {tag: sec}


def main():
    pv = [64, 127, 64, 127, 96, 100, 0, 127]  # deep-fast
    sig = [0x200000] * 34                     # dc

    e = build()
    run_init(e)
    run_emu_block(e, pv, sig)   # warm block 0

    # block 1 with section stops
    run_emu_block(e, pv, sig)   # proceeds to end (block 1)
    # now block 2 instrumented
    q = lambda v: (int(v) & 0xFFFF) << 16
    for i in range(8):
        e.Y[R6M + 0x04 + i] = q(pv[i])
    for i, v in enumerate(sig):
        e.X[0x200 + i] = v & M24
    e.R[6] = R6M
    e.R[7] = 0x100
    e.R[0] = 0x200
    if not e.ret_stack:
        e.ret_stack.append(0x14538D)

    emu_secs = {}
    first = True
    for name, stop in STOP.items():
        start = 0x145045 if first else e.pc
        first = False
        e.run(start, end=stop, max_steps=400000)
        emu_secs.update(dump(e, name))
        e.step()  # move past the stop address
    emu_secs.update(dump(e, "end"))

    # model for the same block
    st = M18State()
    # replicate warm blocks in the model
    st2 = M18State()
    for _ in range(2):
        proc_model(st2, [q(v) for v in pv], sig + [0],
                   [e.X.get(0x14A000 + i, 0) for i in range(8192)],
                   [e.X.get(0x14A800 + i, 0) for i in range(8192)])
    # recompute block-2 intermediates: instrument by re-running with hooks
    # (simpler: re-derive by copying proc_model body here via exec of sections)
    # -> instead: monkeypatch: run proc_model on a copy and capture via globals
    import exp25_model as M

    cap = {}
    orig_mul = M.mul

    # capture by re-implementing the section calls inline:
    st3 = M18State()
    for _ in range(2):
        proc_model(st3, [q(v) for v in pv], sig + [0],
                   [e.X.get(0x14A000 + i, 0) for i in range(8192)],
                   [e.X.get(0x14A800 + i, 0) for i in range(8192)])

    # We need intermediates of block 2 -> duplicate proc_model logic minimally:
    from exp25_model import (acc_of, a1, mul, KP1, C45, s24)
    prm = [q(v) for v in pv]
    ts = [e.X.get(0x14A000 + i, 0) for i in range(8192)]
    tc = [e.X.get(0x14A800 + i, 0) for i in range(8192)]

    inp2 = a1(mul(prm[7], prm[7]))
    qb = [0] * 32
    for j in range(16):
        qb[j] = a1(mul(sig[2 * j], inp2) << 2)
        qb[16 + j] = a1(mul(sig[2 * j + 1], inp2) << 2)

    spd2 = a1(mul(prm[2], prm[2]))
    step = a1(mul(spd2, 0x9566))
    a = acc_of(st2.phase_hi)
    b = acc_of(st2.phase_lo)
    Sc, Qc = [], []
    for k in range(16):
        Sc.append(a1(b)); Qc.append(a1(a))
        ao = (a >> 24) & M24; bo = (b >> 24) & M24
        b = b - mul(ao, step)
        a = a + mul(bo, step)

    sw_old = st2.s_wid
    sw_acc = acc_of(sw_old) + mul(KP1, prm[5]) - mul(KP1, sw_old)
    sw_new = a1(sw_acc)
    idx = a1(mul(sw_new, 0x800))
    s_cur = ts[idx]; c_cur = tc[idx]
    sp, cp = st2.q_sin, st2.q_cos
    ds_raw = a1(acc_of(s_cur) - acc_of(sp))
    dc_raw = a1(acc_of(c_cur) - acc_of(cp))
    acc_a = acc_of(sp); acc_b = acc_of(cp)
    XE = []
    for k in range(16):
        YE0 = a1(acc_a)
        XE.append(a1(acc_b))
        acc_a = acc_a + mul(8, ds_raw)
        acc_b = acc_b + mul(8, dc_raw)

    s18_acc = M.sext48(st2.s18)
    s18_acc = s18_acc + mul(KP1, prm[0]) - mul(KP1, (s18_acc >> 24) & M24)
    s19_acc = M.sext48(st2.s19)
    s19_acc = s19_acc + mul(KP1, prm[1]) - mul(KP1, (s19_acc >> 24) & M24)
    s19_hi = (s19_acc >> 24) & M24
    bb = s18_acc - acc_of(0x400000)
    c = a1(bb << 1)
    bb2 = abs(bb << 1) + acc_of(0x800000)
    y1v = a1(-bb2)
    v_acc = mul(s19_hi, y1v)
    v_new = a1(v_acc)
    dv_raw = a1(v_acc - acc_of(st2.v_prev))
    acc_v = acc_of(st2.v_prev)
    VR = []
    for k in range(16):
        VR.append(a1(acc_v))
        acc_v = acc_v + mul(0x80000, dv_raw)

    y1k = [YE0] + [0] * 15
    a_bank = []; b_bank = []
    for k in range(16):
        prod = mul(XE[k], Sc[k])
        a_bank.append(a1(prod + mul(y1k[k], Qc[k])))
        b_bank.append(a1(prod - mul(y1k[k], Qc[k])))

    src = a_bank + b_bank
    mod = [a1(mul(src[i], VR[i % 16]) + acc_of(c)) for i in range(32)]

    s13_new = a1((acc_of(prm[4]) - acc_of(0x400000)) << 1)
    ds13 = (s13_new - st2.s13) & M24
    acc_fb = acc_of(st2.s13)
    fbr = []
    for k in range(16):
        fbr.append(a1(acc_fb))
        acc_fb = acc_fb + mul(0x80000, ds13)

    model_secs = {
        "head_gain_done": {"Y00": qb},
        "osc_done": {"X20_2F": Sc, "Y20_2F": Qc,
                     "ph": [a1(a), a1(b)], "swid": [sw_new]},
        "wid_table_done": {"q": [c_cur, s_cur], "swid": [sw_new]},
        "quad_ramp_done": {"XE0": XE, "YE0": [YE0]},
        "onepole_done": {"s18": [a1(s18_acc), s18_acc & M24],
                         "s19": [a1(s19_acc), s19_acc & M24]},
        "vramp_done": {"XE0": VR, "v": [v_new]},
        "mod_done": {"X00": mod},
        "fbramp_done": {"Y90": fbr, "s13": [s13_new]},
    }

    print("=== section diffs (block 2, deep-fast dc) ===")
    for name, _ in STOP.items():
        esec = emu_secs.get(name, {})
        msec = model_secs.get(name, {})
        found = False
        for key in msec:
            if key in esec:
                ev, mv = esec[key], msec[key]
                d = [(i, x, y) for i, (x, y) in enumerate(zip(ev, mv)) if x != y]
                if d:
                    print("%-16s %s: %d diffs, first: emu=%06X model=%06X (idx %d)"
                          % (name, key, len(d), d[0][1], d[0][2], d[0][0]))
                    found = True
        if not found:
            print("%-16s match" % name)


if __name__ == "__main__":
    main()
