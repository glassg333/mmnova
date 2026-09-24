#!/usr/bin/env python3
"""svf_fit340.py — PROVE the exact recursion of func_000340 (cascade-1 kernel):
clean pure-Python re-implementation with per-instruction logging, compared
instruction-by-instruction against the emulator's step trace.
"""
import json
from svf_fit import s24, sext56, mpy, a1_of, M24, M56

def replay(pre):
    X = dict(enumerate(pre["X"]))
    Y = dict(enumerate(pre["Y"]))
    R = list(pre["R"])
    MREG = [m if m < 0x800000 else m - 0x1000000 for m in pre["M"]]
    r0, r1, r4 = R[0], R[1], R[4]
    n1 = 2
    x0 = pre["x0"]; x1 = pre["x1"]
    y0 = pre["y0"]; y1 = pre["y1"]
    a = int(pre["a"], 16); b = int(pre["b"], 16)
    log = []   # (pc, a, b) BEFORE executing that pc

    def rd(m, ad):
        return m.get(ad & M24, 0)

    def upd(rnum, r, delta):
        """DSP56300 address register update with modulo (M-register)."""
        m = MREG[rnum]
        if m == -1 or m == 0xFFFFFF:
            return (r + delta) & M24
        base = r & ~(m & M24)
        off = ((r & m) + delta) % ((m & M24) + 1)
        return base | off

    def mac(acc, s1, s2):
        return sext56(acc + ((s24(s1) * s24(s2)) << 1))

    # 0340: move x:(r0)+,x0
    x0 = rd(X, r0); r0 = upd(0, r0, 1)
    # 0341: move #$2,n1
    n1 = 2
    # 0342: tfr x0,a  x:(r0)+,x0  y:(r4)+,y0
    log.append((0x342, a, b))
    a = s24(x0) << 24
    x0 = rd(X, r0); r0 = upd(0, r0, 1)
    y0 = rd(Y, r4); r4 = upd(4, r4, 1)
    for i in range(16):
        # 0345: mac y0,x0,a  x:(r0)-,x1  y:(r4)+,y1
        log.append((0x345, a, b))
        a = mac(a, y0, x0)
        x1 = rd(X, r0); r0 = upd(0, r0, -1)
        y1 = rd(Y, r4); r4 = upd(4, r4, 1)
        # 0346: mac y0,x0,a
        log.append((0x346, a, b))
        a = mac(a, y0, x0)
        # 0347: mac y1,x1,a  x:(r1)+,x0
        log.append((0x347, a, b))
        a = mac(a, y1, x1)
        x0 = rd(X, r1); r1 = upd(1, r1, 1)
        # 0348: mac -x0,y1,a  x:(r1)-,x0
        log.append((0x348, a, b))
        a = sext56(a - ((s24(x0) * s24(y1)) << 1))
        x0 = rd(X, r1); r1 = upd(1, r1, -1)
        # 0349: mac -y0,x0,a  b,y:(r1)+n1
        log.append((0x349, a, b))
        a = sext56(a - ((s24(y0) * s24(x0)) << 1))
        Y[r1 & M24] = a1_of(b); r1 = upd(1, r1, n1)
        # 034A: mac -y0,x0,a  x:(r0)+,x0  y:(r4)+,y0
        log.append((0x34A, a, b))
        a = sext56(a - ((s24(y0) * s24(x0)) << 1))
        x0 = rd(X, r0); r0 = upd(0, r0, 1)
        y0 = rd(Y, r4); r4 = upd(4, r4, 1)
        # 034B: mpy x1,y0,b  y:(r4)+,y1
        log.append((0x34B, a, b))
        b = mpy(x1, y0)
        y1 = rd(Y, r4); r4 = upd(4, r4, 1)
        # 034C: tfr x0,a  a,x:(r1)-  a,y0
        log.append((0x34C, a, b))
        a_old = a
        a = s24(x0) << 24
        X[r1 & M24] = a1_of(a_old); r1 = upd(1, r1, -1)
        y0 = a1_of(a_old)
        # 034D: mac y1,y0,b  x:(r0)+,x0  y:(r4)+,y0
        log.append((0x34D, a, b))
        b = mac(b, y1, y0)
        x0 = rd(X, r0); r0 = upd(0, r0, 1)
        y0 = rd(Y, r4); r4 = upd(4, r4, 1)
        # 034E: asl #$2,b,b
        log.append((0x34E, a, b))
        b = sext56(b << 2)
    return X, Y, a, b, log

def main():
    calls = json.load(open("/home/z/my-project/mining/svf_trace_calls.json"))
    steps = json.load(open("/home/z/my-project/mining/svf_trace_steps.json"))
    for frame in (2, 3):
        pre = next(c for c in calls if c["kind"] == "pre" and c["site"] == 0x5A1
                   and c["frame"] == frame)
        post = next(c for c in calls if c["kind"] == "post" and c["site"] == 0x5A1
                    and c["frame"] == frame)
        win = next(s for s in steps if s["site"] == 0x5A1 and s["frame"] == frame)
        X2, Y2, a2, b2, log = replay(pre)

        # 1) per-instruction A/B comparison
        emap = {}
        for r in win["rows"]:
            emap.setdefault(r["pc"], []).append(r)
        mism = 0
        for (pc, a_r, b_r) in log:
            lst = emap.get(pc)
            if not lst:
                continue
            er = lst.pop(0)
            if er["A"][-14:] != "%014X" % (a_r & M56) or \
               er["B"][-14:] != "%014X" % (b_r & M56):
                mism += 1
                if mism <= 4:
                    print("f%d %04X: emu A=%s B=%s | rep A=%014X B=%014X" %
                          (frame, pc, er["A"][-14:], er["B"][-14:],
                           a_r & M56, b_r & M56))
        # 2) memory comparison
        mdiff = 0
        for addr in range(0x100):
            if X2.get(addr, 0) != post["X"][addr] or Y2.get(addr, 0) != post["Y"][addr]:
                mdiff += 1
        print("frame %d: instruction mismatches=%d, memory diffs=%d" %
              (frame, mism, mdiff))

if __name__ == "__main__":
    main()
