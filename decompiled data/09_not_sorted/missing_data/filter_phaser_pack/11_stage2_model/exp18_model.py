#!/usr/bin/env python3
"""
exp18_model.py — bit-exact Python model of filter stage-2 blocks and word-exact
verification against emulator snapshots (exp18_fdn_snap.json).

Blocks (P:$0A5D-$0AD0):
  L1 $0A5D-$0A82: two chains (a: X:$30+i inputs, b: X:$20+i inputs)
      s_{i+1} = s_i - m_{i-1}*beta - B1(s_i)*c1 + m_i*beta
      beta = B1(-((c1>>1 acc) + (-1.0)))   [add #$800000 = -1.0, then neg]
      m_{-1} = wrap state (r7-$89) = (X:$3F_prev, X:$2F_prev)
      stores: Y:$62+2i = B1(a_i), Y:$63+2i = B1(b_i); states -> (r7-$88), wrap -> (r7-$89)
  L2 $0A84-$0A9B: angle c2 = X:$144AC7[(WOFS+BOFS)]; two chains consume
      interleaved L1 outputs (a-chain even w, b-chain odd w):
      s_{i+1} = s_i + w_i*c2 - B1(s_i)*c2   (a: w = Y:$62+2i, b: w = Y:$63+2i)
      states -> (r7-$87); stores Y:$20+2i / Y:$21+2i
  L3 $0A9D-$0AB5: angle c3 = X:$144B48[phase_idx]; same pattern,
      inputs X:$00+i (a-chain), X:$10+i (b-chain); outputs -> Y:$62+2i/63+2i
  DP $0AB7-$0AD0: depth = 4*(ATK-0.5)^2, KILL gate (X:P+$76 == 4 -> y0=0),
      a_i = 2*DEC*Y:$20+i + depth*Y:$62+2i ; b_i = 2*DEC*Y:$21+i + depth*Y:$63+2i
      X:$51 = B1(depth); X:$52+i = B1(a_i) i=0..14; X:$61 = B1(a_15)
      X:$40 = 0;    X:$41+i = B1(b_i) i=0..14; X:$50 = B1(b_15)
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")

M24 = 0xFFFFFF
P = 0x400
R7 = P + 0x15C         # r7 = $55C in track-frame context (measured)


def s24(v):
    v &= M24
    return v - (1 << 24) if v >> 23 else v


def acc(v):
    return s24(v) << 24


def B1(a):
    return (a >> 24) & M24


def smac(a, x, y):
    v = s24(x) * s24(y)
    r = (a + (v << 1)) & ((1 << 56) - 1)
    if r >> 55:
        r -= 1 << 56
    return r


def neg(a):
    return (-a) & ((1 << 56) - 1) if a else 0


def model_L1(e, s):
    """returns (updates dict, dbg) — updates applied to snapshot -> L1 output"""
    upd = {}
    bofs = s["Y:P+16"] >> 16
    c1 = e.X.get(0x144AC7 + bofs, 0) & M24
    b = acc(c1)
    y0 = B1(b)                    # y0 = old B1 = c1
    b = b >> 1                    # asr b (acc shift right)
    b = (b + acc(0x800000)) & ((1 << 56) - 1)   # add #>$800000 (-1.0)
    if b >> 55:
        b -= 1 << 56
    b = neg(b & ((1 << 56) - 1))  # neg
    y1 = B1(b)                    # move b,y1
    a_s = acc(s["Y:P+%02X" % (R7 - 0x88 - P)])    # y:(r7-$88)
    b_s = acc(s["X:P+%02X" % (R7 - 0x88 - P)])    # x:(r7-$88)
    x0 = s["Y:P+%02X" % (R7 - 0x89 - P)]          # old wrap (y-part)
    x1 = s["X:P+%02X" % (R7 - 0x89 - P)]          # old wrap (x-part)
    m30 = [s["X:%03X" % (0x30 + i)] for i in range(16)]
    m20 = [s["X:%03X" % (0x20 + i)] for i in range(16)]
    a_final = b_final = None
    for i in range(16):
        # A7A: a' = a - x0*y1 ; store B1(a_pre) -> Y:$62+2i ; x0 <- B1(a_pre)
        st = B1(a_s)
        upd["Y:%03X" % (0x62 + 2 * i)] = st
        a_s = smac(a_s, (-x0) & M24, y1)
        x0 = st
        # A7B
        st = B1(b_s)
        upd["Y:%03X" % (0x63 + 2 * i)] = st
        b_s = smac(b_s, (-y1) & M24, x1)
        x1 = st
        # A7C: a -= y0*x0 ; x0 <- X:(r0)=m30[i]
        a_s = smac(a_s, (-y0) & M24, x0)
        x0 = m30[i]
        # A7D: b -= y0*x1 ; x1 <- m20[i]
        b_s = smac(b_s, (-x1) & M24, y0)
        x1 = m20[i]
        # A7E: a += x0*y1 ; x0 <- m30[i] (post-inc same cell)
        a_s = smac(a_s, x0, y1)
        x0 = m30[i]
        # A7F: b += y1*x1 ; x1 <- m20[i]
        b_s = smac(b_s, y1, x1)
        x1 = m20[i]
    a_final, b_final = a_s, b_s
    upd["Y:P+%02X" % (R7 - 0x88 - P)] = B1(a_final)           # y:(r7-$88)
    upd["X:P+%02X" % (R7 - 0x88 - P)] = B1(b_final)           # x:(r7-$88)
    upd["Y:P+%02X" % (R7 - 0x89 - P)] = m30[15]               # y:(r7-$89) = X:$3F
    upd["X:P+%02X" % (R7 - 0x89 - P)] = m20[15]               # x:(r7-$89) = X:$2F
    return upd


def model_L2(e, s):
    upd = {}
    wofs = s["Y:P+17"] >> 16
    bofs = s["Y:P+16"] >> 16
    c2 = e.X.get(0x144AC7 + ((wofs + bofs) & M24), 0) & M24
    y0 = c2
    a_s = acc(s["Y:P+%02X" % (R7 - 0x87 - P)])
    b_s = acc(s["X:P+%02X" % (R7 - 0x87 - P)])
    # pre-loop: x1 = y:(r5)+ = Y:$62 (even chain), r5 -> $63
    x1 = s["Y:062"]
    for i in range(16):
        # A95: a += x1*y0; x0<-B1(a); store B1(a)->Y:$20+2i
        st = B1(a_s)
        upd["Y:%03X" % (0x20 + 2 * i)] = st
        a_s = smac(a_s, x1, y0)
        # A96: a -= y0*B1(a); x1 <- y:(r5)+ = Y:$63+2i (odd)
        a_s = smac(a_s, (-y0) & M24, st)
        x1 = s["Y:%03X" % (0x63 + 2 * i)]
        # A97: b += x1*y0; x0<-B1(b); store B1(b)->Y:$21+2i
        st = B1(b_s)
        upd["Y:%03X" % (0x21 + 2 * i)] = st
        b_s = smac(b_s, x1, y0)
        # A98: b -= y0*B1(b); x1 <- y:(r5)+ = Y:$64+2i (even next)
        b_s = smac(b_s, (-y0) & M24, st)
        if i < 15:
            x1 = s["Y:%03X" % (0x64 + 2 * i)]
    upd["Y:P+%02X" % (R7 - 0x87 - P)] = B1(a_s)
    upd["X:P+%02X" % (R7 - 0x87 - P)] = B1(b_s)
    return upd


def model_L3(e, s):
    upd = {}
    st89 = (s["Y:P+%02X" % (R7 - 0x8D - P)] << 24) | s["X:P+%02X" % (R7 - 0x8D - P)]
    # asr #$11 (17) then b0 -> r2: index = bits [47..24] of (st>>17)?
    v = st89 >> 17
    idx = (v >> 24) & M24
    c3 = e.X.get(0x144B48 + idx, 0) & M24
    y0 = c3
    a_s = acc(s["Y:P+%02X" % (R7 - 0x90 - P)])
    b_s = acc(s["X:P+%02X" % (R7 - 0x90 - P)])
    x1 = s["X:010"]                     # pre-loop x:(r3)+ = X:$10
    for i in range(16):
        # AAF: a += x1*y0; store B1(a)->Y:$62+2i; x0<-B1(a)
        st = B1(a_s)
        upd["Y:%03X" % (0x62 + 2 * i)] = st
        a_s = smac(a_s, x1, y0)
        # AB0: a -= y0*B1(a); x1 <- x:(r1)+ = X:$00+i
        a_s = smac(a_s, (-y0) & M24, st)
        x1 = s["X:%03X" % i]
        # AB1: b += x1*y0; store B1(b)->Y:$63+2i; x0<-B1(b)
        st = B1(b_s)
        upd["Y:%03X" % (0x63 + 2 * i)] = st
        b_s = smac(b_s, x1, y0)
        # AB2: b -= y0*B1(b); x1 <- x:(r3)+ = X:$11+i
        b_s = smac(b_s, (-y0) & M24, st)
        if i < 15:
            x1 = s["X:%03X" % (0x11 + i)]
    upd["Y:P+%02X" % (R7 - 0x90 - P)] = B1(a_s)
    upd["X:P+%02X" % (R7 - 0x90 - P)] = B1(b_s)
    return upd


def model_DP(e, s):
    """$0AB7-$0AD0: depth scaling -> coefficient banks X:$40-$50 / X:$51-$61"""
    upd = {}
    atkw = s["Y:P+14"]
    b = acc(atkw & M24)
    b = (b - acc(0x400000)) & ((1 << 56) - 1)
    if b >> 55:
        b -= 1 << 56
    if s24(B1(b)) < 0:
        b = ((~b) + 1) & ((1 << 56) - 1)      # abs
    b = (b << 1) & ((1 << 56) - 1)            # asl b
    if b >> 55:
        b -= 1 << 56
    x0 = B1(b)                                 # 2|ATK-0.5|
    a = smac(0, x0, x0)                        # mpy x0,x0,a
    depth = B1(a)
    y0 = s["Y:P+15"]                           # DEC word (24-bit cell)
    if s["X:P+D8"] == 4:                       # KILL gate
        y0 = 0
    x1 = depth                                 # move a,x1
    a_s = None
    # $0AC0: move x:(r7-$84),b  — b REUSED for the phase check; the pipeline
    # store at ACC(iter0) leaks it into X:$40 (b = AMP phase value here)
    b_s = acc(s["X:P+D8"])
    x0 = s["Y:020"]                            # AC6: x0 = Y:$20 (a_0 input)
    y1 = s["Y:062"]
    out_a, out_b = [], []
    for i in range(16):
        # AC9: store pre-a -> X:(r2)+ ; a = y0*x0 ; y1 <- Y:(r4)+ (even L3 out)
        pre_a = a_s if i > 0 else acc(depth)
        a_s = smac(0, y0, x0)
        out_a.append(B1(pre_a))
        y1 = s["Y:%03X" % (0x62 + 2 * i)]
        # ACA: asl #$1
        a_s = (a_s << 1) & ((1 << 56) - 1)
        if a_s >> 55:
            a_s -= 1 << 56
        # ACB: a += y1*x1 ; x0 <- Y:(r1)+  (odd cell = b_{i} input)
        a_s = smac(a_s, y1, x1)
        xb = s["Y:%03X" % (0x21 + 2 * i)]
        # ACC: store pre-b -> X:(r3)+ ; b = y0*x0 ; y1 <- Y:(r4)+ (odd L3 out)
        pre_b = b_s
        b_s = smac(0, y0, xb)
        out_b.append(B1(pre_b))
        y1 = s["Y:%03X" % (0x63 + 2 * i)]
        # ACD: asl #$1
        b_s = (b_s << 1) & ((1 << 56) - 1)
        if b_s >> 55:
            b_s -= 1 << 56
        # ACE: b += y1*x1 ; x0 <- Y:(r1)+  (next even cell = a_{i+1} input)
        b_s = smac(b_s, y1, x1)
        if i < 15:
            x0 = s["Y:%03X" % (0x22 + 2 * i)]
    # ACF/AD0: final stores
    upd["X:051"] = out_a[0]
    for i in range(1, 16):
        upd["X:%03X" % (0x51 + i)] = out_a[i]
    upd["X:061"] = B1(a_s)
    for i in range(16):
        upd["X:%03X" % (0x40 + i)] = out_b[i]
    upd["X:050"] = B1(b_s)
    return upd


def apply(s, upd):
    s2 = dict(s)
    for k, v in upd.items():
        s2[k] = v
    return s2


def main():
    G = json.load(open("/home/z/my-project/mining/exp18_fdn_snap.json"))
    # reconstruct emulator X for curve reads
    class E:
        pass
    e = E()
    import numpy as np
    raw = open("/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin", "rb").read()
    N = len(raw) // 3
    bb = np.frombuffer(raw[:N * 3], dtype=np.uint8).reshape(-1, 3).astype(np.uint32)
    W = (bb[:, 0] << 16) | (bb[:, 1] << 8) | bb[:, 2]
    e.X = {i: int(W[i]) for i in range(N)}
    e.Y = {}

    n_cfg = 0
    n_frames = 0
    n_bad = 0
    for ci, c in enumerate(G):
        cfg_bad = 0
        for k, snaps in enumerate(c["frames"]):
            s_in, s_l1, s_l2, s_l3, s_dp, s_out = snaps
            # L1
            upd = model_L1(e, s_in)
            got = apply(s_in, upd)
            bad1 = [kk for kk in upd if got[kk] != s_l1[kk]]
            # L2
            upd2 = model_L2(e, s_l1)
            got2 = apply(s_l1, upd2)
            bad2 = [kk for kk in upd2 if got2[kk] != s_l2[kk]]
            # L3
            upd3 = model_L3(e, s_l2)
            got3 = apply(s_l2, upd3)
            bad3 = [kk for kk in upd3 if got3[kk] != s_l3[kk]]
            # DP
            upd4 = model_DP(e, s_l3)
            got4 = apply(s_l3, upd4)
            bad4 = [kk for kk in upd4 if got4[kk] != s_dp[kk]]
            status = "OK " if not (bad1 or bad2 or bad3 or bad4) else "BAD"
            n_frames += 1
            if bad1 or bad2 or bad3 or bad4:
                cfg_bad += 1
                n_bad += 1
            if bad1 or bad2 or bad3 or bad4:
                print("cfg%d f%d %s L1=%d L2=%d L3=%d DP=%d bad" % (
                    ci, k, status, len(bad1), len(bad2), len(bad3), len(bad4)))
                for kk in (bad4[:4] or bad3[:4] or bad2[:4] or bad1[:4]):
                    src = {"L1": (got, s_l1, upd), "L2": (got2, s_l2, upd2),
                           "L3": (got3, s_l3, upd3), "DP": (got4, s_dp, upd4)}
                    which = "L1" if kk in bad1 else ("L2" if kk in bad2 else ("L3" if kk in bad3 else "DP"))
                    g, r, u = src[which]
                    print("    %s: model=%06X emu=%06X" % (kk, g[kk], r[kk]))
                for kk in (bad1[:3] or bad2[:3] or bad3[:3]):
                    src = {"L1": (got, s_l1, upd), "L2": (got2, s_l2, upd2), "L3": (got3, s_l3, upd3)}
                    which = "L1" if kk in bad1 else ("L2" if kk in bad2 else "L3")
                    g, r, u = src[which]
                    print("    %s: model=%06X emu=%06X" % (kk, g[kk], r[kk]))
                if k >= 2:
                    pass
        n_cfg += 1
        print("cfg%d (ATK=%d DEC=%d BOFS=%d WOFS=%d): %s" % (
            ci, c["atk"], c["dec"], c["bofs"], c["wofs"],
            "ALL WORD-EXACT (L1+L2+L3+DP)" if cfg_bad == 0 else "%d frames BAD" % cfg_bad))
    print("\n=== VERIFICATION SUMMARY ===")
    print("configs: %d, frames: %d, block-checks: %d (%d bad)" % (
        n_cfg, n_frames, n_frames * 4, n_bad))
    print("blocks: L1 $0A5D-$0A82, L2 $0A84-$0A9B, L3 $0A9D-$0AB5, DP $0AB7-$0AD0")
    print("RESULT: %s" % ("ALL WORD-EXACT" if n_bad == 0 else "MISMATCHES FOUND"))


if __name__ == "__main__":
    main()
