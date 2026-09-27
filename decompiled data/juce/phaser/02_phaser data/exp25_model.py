#!/usr/bin/env python3
"""exp25_model.py — bit-exact Python model of the full m18 (FX-PHASER) PROC
   $145045-$14513F, derived from the listing + verified emulator semantics.

   Dataflow (per 16-sample block):
     1. head gain      Y:$00-$0F/Y:$10-$1F = 4*INP^2*in[k]      (even/odd)
     2. LFO oscillator 16 rotations of (a,b), step = SPD^2*$9566
        carrier: X:$20-$2F = b-chain, Y:$20-$2F = a-chain; state x:$11/$12
     3. WID one-pole -> table read sin/cos ($14A000/$14A800, idx = s_wid>>12)
        quad ramps: X:$E0-$EF = cos-side walk, Y:$E0 = sin-side (last store)
     4. CNTR/DEP one-poles (48-bit, k=$28F5C) -> v = s19*(-(|2(s18-.5)|+1))
        V-ramp X:$E0-$EF = v_prev + k*dv/16 (OVERWRITES quad cos-ramp)
        constant c = 2*(s18_hi - 0.5)
     5. sidebands: a_bank[k] = XE[k]*s[k] + YE[k]*q[k]
                   b_bank[k] = XE[k]*s[k] - YE[k]*q[k]   (YE[0]=Y:$E0, k>0 stale)
        -> X:$20-$2F / X:$30-$3F
     6. mod bank: mod[i] = a_bank|b_bank[i] * Vramp[i%16] + c   (X:$00-$1F)
     7. FB: s13 = 2*(FB-0.5); ramp Y:$90-$9F = s13_prev + k*ds/16
     8. TWO passes (even/odd), each a 6-section allpass ladder:
          P0 = fb*W6 + q ; Ri = W[i-1] + (R[i-1]-W[i])*IN ; OUT = c45*(q+R6)
        state slots: even r6+$1E..$24, odd r6+$26..$2C (n6 switch $1E->$26)
        outputs X:$60-$6F / X:$70-$7F
     9. mix: out_e = (1-M)*dry_e + M*wet1 ; out_o = (1-M)*dry_o + M*wet2
        M law: x1 = $7FFFFF - MIX

   All arithmetic is raw 24-bit / 56-bit-acc integer, matching dsp_emu.py:
     mul(x,y)  = sext24(x)*sext24(y)*2        (Q1.23 MAC, product left-shift 1)
     acc_of(w) = sext24(w)<<24                (24-bit reg -> acc)
     a1(acc)   = (acc>>24) & 0xFFFFFF         (acc -> 24-bit reg / memory)
"""
import sys

M24 = 0xFFFFFF
C45 = 0x5A8241
KP1 = 0x28F5C            # one-pole k
KN1 = 0xFD70A4           # -k (as encoded immediate)


def s24(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v


def acc_of(raw):
    """24-bit register -> 56-bit accumulator (left aligned)."""
    return s24(raw) << 24


def a1(acc):
    """acc -> 24-bit register (bits 47..24)."""
    return (acc >> 24) & M24


def mul(x_raw, y_raw):
    """Q1.23 multiply as 56-bit acc value (product << 1)."""
    return s24(x_raw) * s24(y_raw) * 2


def muln(x_raw, y_raw):
    """Product with the NEGATED first operand, as the DSP56300 does it:
    the operand negation is a 24-bit two's-complement wrap, so for
    x = $800000 (-2^23) the negation yields $800000 again (still negative).
    Used for every `mac -x1,...` style instruction."""
    nx = (-s24(x_raw)) & M24
    return s24(nx) * s24(y_raw) * 2


def sext48(v):
    v &= (1 << 48) - 1
    return v - (1 << 48) if v & (1 << 47) else v


class M18State:
    """Persistent machine state (mirrors r6 slots + scratch)."""
    def __init__(self):
        self.phase_hi = 0x000080   # x:(r6+$11) — INIT: move #$80,x0 (= $000080)
        self.phase_lo = 0x000000   # x:(r6+$12)
        self.s_wid = 0             # y:(r6+$1b)
        self.s18 = 0               # 48-bit: x:(r6+$18):y:(r6+$18)
        self.s19 = 0               # 48-bit: x:(r6+$19):y:(r6+$19)
        self.v_prev = 0            # y:(r6+$1a)
        self.s13 = 0               # y:(r6+$13)
        self.q_cos = 0             # y:(r6+$1c)
        self.q_sin = 0             # y:(r6+$1d)
        self.S_even = [0] * 7      # L:(r6+$1E..$24) X halves
        self.S_odd = [0] * 7       # L:(r6+$26..$2C) X halves


def proc_model(st, prm, sig, table_s, table_c, stale_ye=None, dbg=None):
    """One 16-sample block. prm = 8 raw param words (host writes v<<16).
       sig = 34 input words (only [0..31] consumed). Returns 32 outputs:
       [even0, odd0, even1, odd1, ...] as written to y:(r7)+.
       dbg: optional dict receiving intermediates (mod/qb/fbr/outs)."""
    if stale_ye is None:
        stale_ye = [0] * 16

    # ---- 1. head gain ($145045-$145056) -----------------------------------
    inp2 = a1(mul(prm[7], prm[7]))
    qb = [0] * 32
    for j in range(16):
        qb[j] = a1(mul(sig[2 * j], inp2) << 2)        # Y:$00-$0F
        qb[16 + j] = a1(mul(sig[2 * j + 1], inp2) << 2)  # Y:$10-$1F

    # ---- 2. LFO oscillator ($145057-$145069) -------------------------------
    spd2 = a1(mul(prm[2], prm[2]))
    step = a1(mul(spd2, 0x9566))
    a = acc_of(st.phase_hi)
    b = acc_of(st.phase_lo)
    x1 = st.phase_hi          # $145061: move a,x1 (initial)
    Sc = []   # X:$20-$2F (b-chain)
    Qc = []   # Y:$20-$2F (a-chain)
    for k in range(16):
        Sc.append(a1(b))          # store pre-ALU b -> X:(r0)+
        Qc.append(a1(a))          # store pre-ALU a -> Y:(r4)+
        y0 = (b >> 24) & M24      # b,y0 latch (pre-ALU b)
        a_k = (a >> 24) & M24     # a,x1 latch — pre-ALU a (DELAYED by design)
        b = b + muln(x1, step)    # mac -x1,x0,b  (x1 = a1(a_{k-1})!)
        a = a + mul(y0, step)     # mac y0,x0,a   (y0 = a1(b_k))
        x1 = a_k
    st.phase_hi = a1(a)
    st.phase_lo = a1(b)

    # ---- 3. WID one-pole, table read, quad ramps ($14506A-$145089) ---------
    sw_old = st.s_wid
    sw_acc = acc_of(sw_old) + mul(KP1, prm[5]) - mul(KP1, sw_old)
    sw_new = a1(sw_acc)
    st.s_wid = sw_new
    idx = a1(mul(sw_new, 0x800))
    s_cur = table_s[idx] if 0 <= idx < len(table_s) else 0
    c_cur = table_c[idx] if 0 <= idx < len(table_c) else 0
    sp, cp = st.q_sin, st.q_cos
    st.q_cos, st.q_sin = c_cur, s_cur
    ds_raw = a1(acc_of(s_cur) - acc_of(sp))   # sub x1,a ; a,y0 (a1 latch)
    dc_raw = a1(acc_of(c_cur) - acc_of(cp))
    acc_a = acc_of(sp)
    acc_b = acc_of(cp)
    XE = []
    YSR = []
    for k in range(16):
        YSR.append(a1(acc_a))              # a,y:(r0) — same addr as X (shared r0)
        XE.append(a1(acc_b))               # b,x:(r0)+ — the increment walks BOTH
        acc_a = acc_a + mul(8, ds_raw)     # mac y0,x0,a (x0 = 8)
        acc_b = acc_b + mul(8, dc_raw)     # mac x0,y1,b

    # ---- 4. CNTR/DEP one-poles, v, V-ramp ($14509C-$1450C5) ----------------
    s18_acc = sext48(st.s18)
    s18_acc = s18_acc + mul(KP1, prm[0]) - mul(KP1, (s18_acc >> 24) & M24)
    st.s18 = s18_acc & ((1 << 48) - 1)
    s19_acc = sext48(st.s19)
    s19_acc = s19_acc + mul(KP1, prm[1]) - mul(KP1, (s19_acc >> 24) & M24)
    st.s19 = s19_acc & ((1 << 48) - 1)
    s19_hi = (s19_acc >> 24) & M24
    bb = s18_acc - acc_of(0x400000)        # sub #>$400000,b
    c = a1(bb << 1)                        # asl b ; move b,x1
    bb = abs(bb << 1)                      # abs b
    bb = bb + acc_of(0x800000)             # add #>$800000,b
    bb = -bb                               # neg b
    y1v = a1(bb)
    v_acc = mul(s19_hi, y1v)               # mpy x0,y1,a
    v_new = a1(v_acc)
    v_prev = st.v_prev
    st.v_prev = v_new
    dv_raw = a1(v_acc - acc_of(v_prev))    # sub x0,a ; a,y0
    acc_v = acc_of(v_prev)
    VR = []
    for k in range(16):
        VR.append(a1(acc_v))               # a,x:(r3)+ pre-ALU
        acc_v = acc_v + mul(0x80000, dv_raw)

    # ---- 5. sidebands ($14508B-$14509B) -------------------------------------
    # y1 walks Y:$E0-$EF (sin-side quad ramp) in lockstep with x0 = X:$E0-$EF
    # (shared r0: y:(r0) no-inc + x:(r0)+ increment once per iteration)
    y1k = YSR
    a_bank = []
    b_bank = []
    for k in range(16):
        prod = mul(XE[k], Sc[k])
        a_bank.append(a1(prod + mul(y1k[k], Qc[k])))
        b_bank.append(a1(prod + muln(y1k[k], Qc[k])))

    # ---- 6. mod bank ($1450C6-$1450D4) --------------------------------------
    src = a_bank + b_bank
    mod = [a1(mul(src[i], VR[i % 16]) + acc_of(c)) for i in range(32)]

    # ---- 7. FB one-pole + ramp ($1450D5-$1450E6) ----------------------------
    s13_old = st.s13
    s13_new = a1((acc_of(prm[4]) - acc_of(0x400000)) << 1)
    ds13 = (s13_new - s13_old) & M24        # sub b,a ; a,y0 (a1 latch)
    st.s13 = s13_new
    acc_fb = acc_of(s13_old)                # ramp starts at the OLD state
    fbr = []
    for k in range(16):
        fbr.append(a1(acc_fb))             # b,y:(r4)+ pre-ALU
        acc_fb = acc_fb + mul(0x80000, ds13)

    # ---- 8. two passes of the 6-section allpass ladder ($1450E7-$145128) ----
    outs = [0] * 32
    for p in range(2):
        W = list(st.S_even if p == 0 else st.S_odd)
        base = 16 * p
        for k in range(16):
            IN = mod[base + k]
            q = qb[base + k]
            fb = fbr[k]
            P0 = a1(mul(fb, W[6]) + acc_of(q))
            R1 = a1(acc_of(W[0]) + mul(P0, IN) + muln(W[1], IN))
            R2 = a1(acc_of(W[1]) + mul(R1, IN) + muln(W[2], IN))
            R3 = a1(acc_of(W[2]) + mul(R2, IN) + muln(W[3], IN))
            R4 = a1(acc_of(W[3]) + mul(R3, IN) + muln(W[4], IN))
            R5 = a1(acc_of(W[4]) + mul(R4, IN) + muln(W[5], IN))
            R6 = a1(acc_of(W[5]) + mul(R5, IN) + muln(W[6], IN))
            outs[base + k] = a1(mul(C45, q) + mul(C45, R6))
            W = [P0, R1, R2, R3, R4, R5, R6]
        if p == 0:
            st.S_even = W
        else:
            st.S_odd = W

    # ---- 9. mix ($145129-$14513F) -------------------------------------------
    if dbg is not None:
        dbg.update(mod=mod, qb=qb, fbr=fbr, outs=outs, VR=VR, XE=XE, YSR=YSR,
                   Sc=Sc, Qc=Qc, a_bank=a_bank, b_bank=b_bank)
    mixp = prm[3]
    om = (0x7FFFFF - mixp) & M24
    final = []
    for k in range(16):
        oe = a1(mul(om, qb[k]) + mul(mixp, outs[k]))
        oo = a1(mul(om, qb[16 + k]) + mul(mixp, outs[16 + k]))
        final.append(oe)
        final.append(oo)
    return final
