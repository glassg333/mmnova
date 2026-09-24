#!/usr/bin/env python3
"""
exp22_model.py — Iteration 22: closed-form bit-exact model of the stage-2 tail
(P:$0AD1-$0B4C), verified word-in-word against emulator snapshots
(exp22_snap.json.gz: 12 configs x 4 frames).

Blocks:
  T1 DIV        $0AD1-$0AE9  24-step reciprocal DIV of (L:P+$CF asr 2),
                             product with global counter Y:$C4, threshold cmp #$10
  T2 INTEGRATE  $0AEA-$0B0B  17-entry tap-phase ramp -> L:$90-$A0 (X = int part,
                             Y = frac part); addend x0 = b1(asr y0) constant for
                             all 17 iterations ($0AFB is NOT in the do-body:
                             emulator DO body = [pc+2..end) excludes end);
                             de-zipper rotation of frame slots P+$CB
  T3 CALL397    $0B0B-$0B13  func_000397 (proven iter 14) — BLACK BOX here;
                             side effects: rewrites X:$70-$82 ring, writes 16
                             words to Y:$00-$0F (delay-section output)
  T4 COPY       $0B14-$0B1E  X:$71-$80 -> Y:(X:$C5)+ modulo-window $4000
                             (m2/m4 = $3FFF: base = ptr & ~$3FFF, NO wrap to 0
                             for aligned pointers); old-a/b pipeline order =
                             plain in-order copy of X:$71..X:$80
  T5 DWID RAMP  $0B1E-$0B33  16-entry zipper-free ramp Y:$10-$1F:
                             delta = (env^2*dwid^2) - env; a-chain = env + k*step,
                             b-chain = env + delta/16 + k*step, step = mpy(#$10,delta)
                             new state (env*dwid)^2 written back to Y:$4FF
  T6 MASTER MIX $0B33-$0B49  (do-body = $0B43..$0B48; `move r3,x:>$2c9` at $0B49
                             executes once AFTER the 16 iterations)
                             FEEDBACK structure when the out-ring (r1 = X:$FF)
                             overlaps the read bank X:$00-$1F (r2 = 0):
                             outs[0]   = (Xpre[0] + L2[1]) * mod[0]
                             outs[2k]  = (fb[k]  + L2[2k+1]) * mod[k]   k=1..15
                             outs[2k+1]= (Xpre[$10+k] + L2[2k]) * mod[k]
                             fb[k] = outs[k] (self-feedback) when X:$FF = 0,
                                     else the genuine audio bus Xpre[k]
                             echo[k] = asr1(outs[2k]+outs[2k+1]) -> X:(X:$2C9)+
                             outputs duplicated to X:(X:$FF)+ and Y:$0000+
"""
import json, gzip

M24 = 0xFFFFFF
M48 = (1 << 48) - 1
M56 = (1 << 56) - 1


def sext(v, b):
    v &= (1 << b) - 1
    return v - (1 << b) if v >> (b - 1) else v


def a1(acc): return (acc >> 24) & M24
def a0(acc): return acc & M24


def compose(v):
    """24-bit -> 56-bit accumulator (A1 = v sign-extended, A0 = 0)."""
    return (sext(v, 24) & M56) << 24


def part_b1(acc, v):
    """move a0/b0-like 24-bit source INTO accumulator: only B1 written."""
    return (acc & ~(M24 << 24)) | ((v & M24) << 24)


def asr56(acc, n):
    return (sext(acc & M56, 56) >> n) & M56


def lsr56(acc, n):
    return ((acc & M56) >> n) & M56


def div_step(acc, s):
    """One DSP56300 non-restoring DIV step (div s,acc)."""
    if (s & M24) == 0:
        return M48                                   # HW divide-by-zero
    D = ((acc >> 24) & M24) << 24 | (acc & M24)
    d1, d0 = (D >> 24) & M24, D & M24
    d1 = ((d1 << 1) | (d0 >> 23)) & M24
    d0 = (d0 << 1) & M24
    s_s, d1_s = sext(s & M24, 24), sext(d1, 24)
    if (d1_s < 0) == (s_s < 0):
        d1 = (d1 - (s & M24)) & M24
    else:
        d1 = (d1 + (s & M24)) & M24
    qbit = 1 if (sext(d1, 24) < 0) == (s_s < 0) else 0
    d0 |= qbit
    return sext((d1 << 24) | d0, 48) & M56


def mpy(sx, sy):
    return sext((sext(sx, 24) * sext(sy, 24)) << 1, 56) & M56


def mpyuu(ux, uy):
    return sext(((ux & M24) * (uy & M24)) << 1, 56) & M56


def dmac_su(acc, sx, uy):
    a1v = sx & M24
    s1v = a1v - (1 << 24) if a1v & 0x800000 else a1v
    return sext(acc + ((s1v * (uy & M24)) << 1), 56) & M56


def add48(acc, v48):
    return sext(acc + sext(v48, 48), 56) & M56


def model_tail(s0, b7, b8, ext):
    """
    s0  : snapshot at pc=$0AD1 (page slots, banks, Y:P+CB/CF, Y:P+1F, Y:P+FF)
    b7  : snapshot at pc=$0B1E post-copy (source X:$71-$80 post-func_397);
          None on bge-skip frames (T2/T4/COPY/CALL397 are skipped by the jmp)
    b8  : snapshot at pc=$0B33 post-DWID, pre-mix (mix inputs)
    ext : host globals: YC4, XC5, XFF, X2C9
    """
    out = {}
    # ---------------- T1: DIV ----------------
    B = compose(s0["X:P+CF"])
    B = (B & ~M24) | s0["Y:P+CF"]
    B = asr56(B, 2)                              # asr #$2,b,b
    A = compose(1)                               # move #$1,a
    y0 = a0(B)                                   # move b0,y0
    for _ in range(24):
        A = div_step(A, y0)                      # div y0,a
        B = part_b1(B, a0(A))                    # move a0,b (B1-only)
    B = asr56(B, 17)                             # asr #$11,b,b
    y1, y0 = a1(B), a0(B)                        # move b1,y1 / move b0,y0
    x0 = ext["YC4"] & M24                        # move y:>$c4,x0
    A = mpyuu(y0, x0)                            # mpyuu y0,x0,a
    A = dmac_su(A, y1, x0)                       # dmac su y1,x0,a
    A = asr56(A, 1)                              # asr a
    if A == 0:
        B = 0                                    # clr b ifeq
    B = sext(B - A, 56) & M56                    # sub a,b
    A = B                                        # tfr b,a
    took_bge = sext(B - compose(0x10), 56) >= 0  # cmp #<$10,b; bge
    out["took_bge"] = took_bge

    if not took_bge:
        # ---------------- T2: integrate + de-zipper ----------------
        A = sext(A + compose(0x40), 56) & M56        # add #>$40,a
        B = compose(y0)                              # tfr y0,b
        B = asr56(B, 1)                              # asr b
        x0_div = a1(B)                               # move b1,x0
        B = part_b1(B, a0(A))                        # move a0,b (B1-only)
        B = lsr56(B, 1)                              # lsr b
        y48 = (y1 << 24) | y0
        for k in range(17):                          # do #<$11, body = $0AF8..$0AFA
            out["X:%03X" % (0x90 + k)] = a1(A)       # a,l:(r1)  pre-ALU
            out["Y:%03X" % (0x90 + k)] = a1(B)       # b1,y:(r1)+ pre-ALU
            A = add48(A, y48)                        # add y,a
            B = sext(B + compose(x0_div), 56) & M56  # add x0,b (x0 = b1 constant)
            B = B & (0x7FFFFF << 24)                 # and x1,b (B1-only)
        out["X:040"] = s0["Y:P+CB"]                  # de-zipper rotation
        out["X:051"] = s0["X:P+CB"]
        out["Y:P+CB"] = s0["X:050"]
        out["X:P+CB"] = s0["X:061"]

        # ---------- T4: copy (source = post-func_397 X:$71-$80) ----------
        xc5 = ext["XC5"] & M24
        base = xc5 & ~0x3FFF
        for k in range(16):
            addr = base | (((xc5 & 0x3FFF) + k) % 0x4000)
            out["Y:%04X" % addr] = b7["X:%03X" % (0x71 + k)]

    # ---------------- T5: DWID ramp ----------------
    dwid = s0["Y:P+1F"]
    A = mpy(dwid, dwid)                          # mpy x0,x0,a
    env = s0["Y:P+FF"]
    B = mpy(env, env)                            # mpy y0,y0,b
    x0 = a1(A)                                   # a,x0   (dwid^2)
    x1 = a1(B)                                   # move b,x1 (env^2)
    A = mpy(x1, x0)                              # env^2 * dwid^2
    out["Y:P+FF"] = a1(A)                        # move a,y:(r7-$5d)
    A = sext(A - compose(env), 56) & M56         # sub y1,a
    B = compose(env)                             # tfr y1,b
    y0 = 0x10                                    # #$10,y0
    x0 = a1(A)                                   # a,x0 (pre-ALU: delta)
    A = compose(env)                             # tfr y1,a
    B = sext(B + mpy(x0, 0x080000), 56) & M56    # maci #>$80000,x0,b
    step = mpy(y0, x0)
    mod = [0] * 16
    for k in range(8):
        mod[2 * k] = a1(A)
        A = sext(A + step, 56) & M56
        mod[2 * k + 1] = a1(B)
        B = sext(B + step, 56) & M56
        out["Y:%03X" % (0x10 + 2 * k)] = mod[2 * k]
        out["Y:%03X" % (0x11 + 2 * k)] = mod[2 * k + 1]

    # ---------------- T6: master mix (feedback!) ----------------
    XFF = ext["XFF"] & M24
    r3 = ext["X2C9"] & M24
    l2 = [b8["Y:%03X" % (0x20 + k)] for k in range(32)]
    xpre = [b8["X:%03X" % a] for a in range(0x20)]
    outs = [0] * 32
    echoes = []
    A = mpy(xpre[0x10], mod[0])                  # $0B3F mpy x1,x0,a (x1 = L:$10 X)
    x1 = xpre[0]                                 # $0B3F load x:(r2)+,x1
    B = mpy(x1, mod[0])                          # $0B40 mpy x1,x0,b
    y0 = l2[0]
    y1 = l2[1]
    for k in range(16):
        B = sext(B + mpy(mod[k], y1), 56) & M56  # $0B43 mac x0,y1,b
        outs[2 * k] = a1(B)
        A = sext(A + mpy(y0, mod[k]), 56) & M56  # $0B44 mac y0,x0,a
        outs[2 * k + 1] = a1(A)
        B = sext(B + A, 56) & M56                # $0B45 add a,b
        B = asr56(B, 1)                          # $0B46 asr b
        echoes.append(a1(B))                     # $0B48 b,x:(r3)+ (pre-mpy)
        if k < 15:
            A = mpy(xpre[0x11 + k], mod[k + 1])  # $0B47 mpy x1,x0,a (new x1/x0)
        # feedback read x:(r2)+ : content of X:(k+1)
        j = k + 1 - XFF
        x1 = outs[j] if 0 <= j <= 2 * k + 1 else (xpre[k + 1] if k + 1 < 0x20 else 0)
        if k < 15:
            B = mpy(x1, mod[k + 1])              # $0B48 mpy x1,x0,b
            y0 = l2[2 * k + 2]
            y1 = l2[2 * k + 3]
    for j in range(32):
        out["Xo:%04X" % ((XFF + j) & M24)] = outs[j]
        out["Yo:%04X" % j] = outs[j]
    for k in range(16):
        out["E:%04X" % ((r3 + k) & M24)] = echoes[k]
    out["X:2C9"] = (r3 + 16) & M24               # written by $0B49 AFTER snapshot
    return out


# ------------------------------------------------------------- verification
def verify():
    data = json.load(gzip.open("/home/z/my-project/scripts/exp22_snap.json.gz"))
    data += json.load(gzip.open("/home/z/my-project/scripts/exp22_skip.json.gz"))
    n_ok = n_bad = 0
    bads = []
    nbge = 0
    for cfg in data:
        for fr in cfg["frames"]:
            s0 = fr["b0_ad1"]["mem"]
            b7 = fr.get("b7_b1e_postcopy", {}).get("mem")
            m8 = fr["b8_b33_postdwid"]["mem"]
            m9 = fr["b9_b49_postmix"]["mem"]
            ext = dict(YC4=s0["Y:0C4"], XC5=s0["X:0C5"],
                       XFF=m8["X:0FF"], X2C9=m8["X:2C9"])
            res = model_tail(s0, b7, m8, ext)
            if res["took_bge"]:
                nbge += 1
            checks = []
            if not res["took_bge"]:
                for k in range(17):
                    checks.append(("X:%03X" % (0x90 + k), m8["X:%03X" % (0x90 + k)]))
                    checks.append(("Y:%03X" % (0x90 + k), m8["Y:%03X" % (0x90 + k)]))
                for key in ("X:040", "X:051", "Y:P+CB", "X:P+CB"):
                    checks.append((key, m8[key]))
                xc5 = s0["X:0C5"]
                base = xc5 & ~0x3FFF
                for k in range(16):
                    ad = base | (((xc5 & 0x3FFF) + k) % 0x4000)
                    checks.append(("Y:%04X" % ad, m8["Y:%04X" % ad]))
            for k in range(16):
                checks.append(("Y:%03X" % (0x10 + k), m8["Y:%03X" % (0x10 + k)]))
            checks.append(("Y:P+FF", m8["Y:P+FF"]))
            for k in range(32):
                checks.append(("Yo:%04X" % k, m9["Y:%03X" % k]))
                checks.append(("Xo:%04X" % ((m8["X:0FF"] + k) & M24),
                               m9["X:%03X" % k]))
            for k in range(16):
                checks.append(("E:%04X" % (m8["X:2C9"] + k),
                               m9["X:%03X" % (m8["X:2C9"] + k)]))
            # $0B49 (`move r3,x:>$2c9`) runs after the b9 snapshot:
            checks.append(("X:2C9", (m8["X:2C9"] + 16) & M24))
            for (key, expect) in checks:
                got = res.get(key)
                if got == expect:
                    n_ok += 1
                else:
                    n_bad += 1
                    if len(bads) < 30:
                        bads.append((cfg["atk"], cfg["dec"], cfg["bofs"],
                                     cfg["wofs"], cfg["dwid"], fr["frame"],
                                     key, got, expect))
    print("verify: OK=%d BAD=%d (bge-skip frames: %d)" % (n_ok, n_bad, nbge))
    for b in bads:
        print("  cfg(%d,%d,%d,%d,dwid%d) fr%d %s model=%06X emu=%06X" % b)


if __name__ == "__main__":
    verify()
