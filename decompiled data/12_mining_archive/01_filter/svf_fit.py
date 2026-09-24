#!/usr/bin/env python3
"""svf_fit.py — PROVE the exact recursion of func_000397 (cascade-2 tap scanner):
re-implement the loop in pure Python from the disassembly, run it on the captured
pre-call state, and compare bit-exact against the emulator's post-call state.
"""
import json

M24 = 0xFFFFFF
M56 = (1 << 56) - 1

def s24(v):
    v &= M24
    return v - (1 << 24) if v & 0x800000 else v

def sext56(v):
    v &= M56
    return v - (1 << 56) if v & (1 << 55) else v

def mpy(s1, s2):
    """acc = s1*s2 (fractional, <<1), 56-bit"""
    return sext56((s24(s1) * s24(s2)) << 1)

def add_reg24(acc, r24):
    return sext56(acc + (s24(r24) << 24))

def macr(acc, s1, s2):
    v = sext56(acc + ((s24(s1) * s24(s2)) << 1) + 0x800000)
    return (v >> 24) << 24

def a1_of(acc):
    return (acc >> 24) & M24

def replay_func397(pre):
    X = dict(enumerate(pre["X"]))
    Y = dict(enumerate(pre["Y"]))
    R = list(pre["R"])
    N = [n if n < 0x800000 else n - 0x1000000 for n in pre["N"]]
    r0, r1, r2, r3, r4 = R[0], R[1], R[2], R[3], R[4]
    n0, n1 = N[0], N[1]
    x1 = pre["x1"]
    y0 = pre["y0"]
    a = int(pre["a"], 16)
    b = int(pre["b"], 16)
    st = []

    def rd(mem, addr):
        return mem.get(addr & M24, 0)

    for i in range(8):
        # 0399: move x:(r4),r1
        r1 = rd(X, r4)
        # 039A: mpy -x1,y0,a  a,x:(r2)+
        a_old, a = a, mpy(-x1, y0)
        X[r2 & M24] = a1_of(a_old); r2 += 1
        # 039B: add x1,a  x:(r0)+n0,x1
        a = add_reg24(a, x1)
        x1 = rd(X, r0); r0 = (r0 + n0) & M24
        # 039C: macr x1,y0,a  x:(r0)+,x1
        a = macr(a, x1, y0)
        x1 = rd(X, r0); r0 = (r0 + 1) & M24
        # 039D: mpy -x1,y0,b  b,x:(r3)+
        b_old, b = b, mpy(-x1, y0)
        X[r3 & M24] = a1_of(b_old); r3 += 1
        # 039E: add x1,b  x:(r0),x1
        b = add_reg24(b, x1)
        x1 = rd(X, r0)
        # 039F: macr x1,y0,b  x:(r1)+,x1  y:(r4)+,y0
        b = macr(b, x1, y0)
        x1 = rd(X, r1); r1 = (r1 + 1) & M24
        y0 = rd(Y, r4); r4 = (r4 + 1) & M24
        # 03A0: move x:(r4),r0
        r0 = rd(X, r4)
        # 03A1: mpy -x1,y0,a  a,x:(r2)+
        a_old, a = a, mpy(-x1, y0)
        X[r2 & M24] = a1_of(a_old); r2 += 1
        # 03A2: add x1,a  x:(r1)+n1,x1
        a = add_reg24(a, x1)
        x1 = rd(X, r1); r1 = (r1 + n1) & M24
        # 03A3: macr x1,y0,a  x:(r1)+,x1
        a = macr(a, x1, y0)
        x1 = rd(X, r1); r1 = (r1 + 1) & M24
        # 03A4: mpy -x1,y0,b  b,x:(r3)+
        b_old, b = b, mpy(-x1, y0)
        X[r3 & M24] = a1_of(b_old); r3 += 1
        # 03A5: add x1,b  x:(r1),x1
        b = add_reg24(b, x1)
        x1 = rd(X, r1)
        # 03A6: macr x1,y0,b  x:(r0)+,x1  y:(r4)+,y0
        b = macr(b, x1, y0)
        x1 = rd(X, r0); r0 = (r0 + 1) & M24
        y0 = rd(Y, r4); r4 = (r4 + 1) & M24
        st.append((i, a, b))
    # 03A7/03A8: move a,x:(r2)+ / move b,x:(r3)+
    X[r2 & M24] = a1_of(a); X[r3 & M24] = a1_of(b)
    return X, a, b, st

def main():
    calls = json.load(open("/home/z/my-project/mining/svf_trace_calls.json"))
    sites = sorted(set(c["site"] for c in calls))
    n_ok = n_bad = 0
    for site in sites:
        pres = [c for c in calls if c["kind"] == "pre" and c["site"] == site]
        posts = [c for c in calls if c["kind"] == "post" and c["site"] == site]
        for pre, post in zip(pres, posts):
            X2, a2, b2, _ = replay_func397(pre)
            diffs = []
            for addr in range(0x100):
                if X2.get(addr, 0) != post["X"][addr]:
                    diffs.append((addr, X2.get(addr, 0), post["X"][addr]))
            if post["a"] != "%014X" % (a2 & M56):
                diffs.append(("accA", a2 & M56, int(post["a"], 16)))
            if post["b"] != "%014X" % (b2 & M56):
                diffs.append(("accB", b2 & M56, int(post["b"], 16)))
            if not diffs:
                n_ok += 1
            else:
                n_bad += 1
                print("site %04X frame %d: %d diffs, first:" %
                      (site, pre["frame"], len(diffs)), diffs[:5])
    print("func_000397 replay: %d calls bit-exact, %d mismatched" % (n_ok, n_bad))

if __name__ == "__main__":
    main()
