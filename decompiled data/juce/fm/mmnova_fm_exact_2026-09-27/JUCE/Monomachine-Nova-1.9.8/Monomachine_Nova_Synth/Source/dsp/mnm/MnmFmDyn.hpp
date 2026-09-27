// MnmFmDyn.hpp — FM+DYN (m10) instruction-faithful mirror of P:$1461C1-...
// Included inside namespace mnmfm after MnmFmPar.hpp.
#pragma once

struct FmDynVoice {
    int32_t w[0x30];
    int32_t L20X[32], L20Y[32];
    int32_t X80[32], Y80[32], YC0[32], XE0[32];
    int32_t Y1E[40], X1E[40];

    FmDynVoice() { init(); }
    void init() {                                    // $14619D + $1461A8
        for (int i = 0; i < 0x30; ++i) w[i] = 0;
        w[0x10] = 0x14A000; w[0x14] = 0x14A000; w[0x18] = 0x14A000;
        w[0x2D] = 0x7FFFFF;                          // config: 2ENV level
        // $2C (1VEN init) and $2E (1FEN init) computed in config() from knobs
    }

    void config(const int knobs[8]) {                // $1461A8-$1461C0
        int64_t Kw[8];
        for (int i = 0; i < 8; ++i) Kw[i] = (int64_t)(knobs[i] & 0xFF) << 16;
        // $2D = $7fffff
        w[0x2D] = 0x7FFFFF;
        // $2C: a = 1VEN-$400000; x0 = 1VOL; b = 2*a*x0; if a<0: take b1, asl#2
        int64_t a = (AL(Kw[3]) - AL(0x400000)) & M56;          // 1VEN - $400000
        int64_t x0 = Kw[2];                                    // 1VOL
        int64_t x1 = A1r(a);
        int64_t b = mpy_ss(x1, x0);                            // 2*(1VEN-64)*1VOL
        int64_t acc = a;                                        // tst a
        bool neg = (s24(A1r(acc)) < 0) || (A1r(acc) & 0x800000);
        if (neg) {
            int64_t v = A1r(b);
            acc = AL(v);
            acc = (acc << 1) & M56;                            // asl
            acc = (acc << 1) & M56;                            // asl
            w[0x2C] = A1r(acc);
        } else {
            w[0x2C] = 0;                                       // a unchanged? a = AL(K7-$400000)
            // asm: a keeps its value only through the tfr ifmi; otherwise a stays
            // the (K7-$400000) acc, then asl asl; y:$2c = a1
            int64_t acc2 = (a << 1) & M56;
            acc2 = (acc2 << 1) & M56;
            w[0x2C] = A1r(acc2);
        }
        // $2E: b = 1FEN-$400000; asr b ifmi (arith shift only when negative);
        // y0 = b1; abs; y1 = b1; b = 2*y1*y0 -> $2E
        int64_t bb = (AL(Kw[1]) - AL(0x400000)) & M56;
        if (s24(A1r(bb)) < 0) bb = asr56(bb, 1);
        int64_t y0 = A1r(bb);
        int64_t y1v = s24(A1r(bb)) < 0 ? 0 : A1r(bb);
        // abs on the accumulator b (56-bit): keep magnitude of the A1-aligned value
        int64_t absb = (bb & (1ll << 55)) ? ((-(int64_t)(bb - (1ll << 56))) & M56) : bb;
        y1v = A1r(absb);
        int64_t b2 = mpy_ss(y1v, y0);
        w[0x2E] = A1r(b2);
    }

    void process(const int knobs[8], int32_t pitch, int32_t out[32]) {
        // NOTE: config() runs at note-on only (see FmExactCore::noteOn) -- the
        // $2C/$2D/$2E init values are note-on snapshots, not per-block.
        int64_t Kw[8];
        for (int i = 0; i < 8; ++i) {
            Kw[i] = (int64_t)(knobs[i] & 0xFF) << 16;
            w[4 + i] = (int32_t)Kw[i];
        }
        // ---- L5 ----
        int64_t L5 = mpysu_dmac(0x0BE37C, AL(pitch));

        // ---- 1FEN: n1 = 128 - |1FEN-64|*2 ; $2E recursion with table $141880 ----
        int64_t bb = (AL(Kw[1]) - AL(0x400000)) & M56;
        bb = asr56(bb, 15);
        int64_t absb = (bb & (1ll << 55)) ? ((-(int64_t)(bb - (1ll << 56))) & M56) : bb;
        int64_t n1 = (AL(0x80) - absb) & M56;
        int n1i = (int)A1r(n1);
        // $2E <- -table[$141880+n1] * $2E   (mpy -x1,x0,b: 2*s(-x1)*s(x0))
        int64_t x0 = w[0x2E];
        int64_t x1 = (n1i >= 0 && n1i < 128) ? kDynCurve[n1i] : 0;
        int64_t b2 = mpy_ss((-x1) & M24, x0);
        w[0x2E] = A1r(b2);
        int64_t inc1 = (L5 + asr56(AL(w[0x2E]), 12)) & M56;    // add b,a
        // ---- 1FRQ continuous (clamp $7EFFFF -> $7FFFFF), inc1 = 2*frq*inc1 ----
        {
            int64_t frq = Kw[0];
            if (frq > 0x7EFFFF) frq = 0x7FFFFF;
            inc1 = (mpysu_dmac(frq, inc1 & ((1ll << 48) - 1)) << 1) & M56;
        }

        // ---- mod-1 phase loop -> L:$20 ----
        {
            int64_t b = (AL(w[0x12]) | w[0x13]) & M56;
            w[0x12] = A1r(inc1); w[0x13] = A0(inc1);
            int64_t delta = asr56((inc1 - b) & M56, 5);
            int64_t y48 = delta;
            int64_t a = b;
            b = (AL(w[0x10]) | w[0x11]) & M56;
            for (int i = 0; i < 32; ++i) {
                L20X[i] = A1r(b); L20Y[i] = A0(b);
                a = (a + y48) & M56;
                b = (b + a) & M56;
                b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;
                b = (b + AL(0x14A000)) & M56;
            }
            w[0x10] = A1r(b); w[0x11] = A0(b);
        }
        // ---- interp -> Y:$80 (>>1) ----
        for (int i = 0; i < 32; ++i) {
            int32_t s0 = sineRead(L20X[i]);
            int32_t s1 = sineRead(L20X[i] + 1);
            int64_t acc = (2 * ((int64_t)s24(s0) << 24)) & M56;
            acc = (acc + 2 * (int64_t)s24(s1) * L20Y[i]) & M56;
            acc = (acc - 2 * (int64_t)s24(s0) * L20Y[i]) & M56;
            Y80[i] = A1(asr56(acc, 1));
        }
        // ---- diff (state $1C) ----
        {
            int64_t x0 = w[0x1C];
            for (int i = 0; i < 32; ++i) {
                int64_t t = AL(Y80[i]);
                int32_t nx = A1(t);
                t = (t - AL(x0)) & M56;
                Y80[i] = A1(t);
                x0 = nx;
            }
            w[0x1C] = (int32_t)x0;
        }
        // ---- fixed LP 0.5 (state $2A): out=state; state += 1.0*(x-state) ----
        {
            int64_t y0c = 0x400000;
            int64_t aa = AL(w[0x2A]);
            int64_t x0 = Y80[0];
            for (int i = 0; i < 32; ++i) {
                int32_t x1l = A1(aa);
                Y80[i] = x1l;
                aa = mac_ss(aa, y0c, x0);
                aa = (aa - 2 * (int64_t)s24(x1l) * s24(y0c)) & M56;
                if (i < 31) x0 = Y80[i + 1];
            }
            w[0x2A] = A1(aa);
        }
        // ---- 1VOL(+1VEN recursion) gated shaper (state $20) ----
        {
            int64_t t = (AL(Kw[2]) + AL(w[0x2C])) & M56;       // add a,b
            t = (t - AL(0x400000)) & M56;
            int64_t y1 = 0;
            if (!(s24(A1r(t)) < 0 && !(A1r(t) & 0x800000))) {
                // tst b; tfr b,a ifpl  -> a = b only when positive (or zero)
                if (!(A1r(t) & 0x800000)) y1 = A1r(t);
            }
            int64_t x0 = w[0x20];
            for (int i = 0; i < 32; ++i) {
                int64_t aa = (mpy_ss(x0, y1) << 4) & M56;
                int32_t x1 = Y80[i];
                aa = (aa + AL(x1)) & M56;
                int32_t x0n = x1;
                Y80[i] = A1(aa);
                x0 = x0n;
            }
            w[0x20] = (int32_t)x0;
        }

        // ---- 1VEN: n1 from 1VEN; $2C <- -table*$2C ----
        {
            int64_t t = (AL(Kw[3]) - AL(0x400000)) & M56;
            t = asr56(t, 15);
            int64_t absb = (t & (1ll << 55)) ? ((-(int64_t)(t - (1ll << 56))) & M56) : t;
            int64_t n1v = (AL(0x80) - absb) & M56;
            int n1i = (int)A1r(n1v);
            int64_t x0 = w[0x2C];
            int64_t x1 = (n1i >= 0 && n1i < 128) ? kDynCurve[n1i] : 0;
            int64_t a2 = mpy_ss((-x1) & M24, x0);
            w[0x2C] = A1r(a2);
        }

        // ---- 2FRQ quadratic: x0 = A1(2*frq^2) ; inc2 = 8*x0*L5 (mpysu/dmac + asl#2) ----
        int64_t inc2;
        {
            int64_t frq = Kw[4];
            if (frq > 0x7EFFFF) frq = 0x7FFFFF;
            int64_t b = mpy_ss(frq, frq);
            int64_t x0 = A1(b);
            int64_t a = mpysu_dmac(x0, L5 & ((1ll << 48) - 1));
            inc2 = (a << 2) & M56;
        }
        // ---- mod-2 phase loop + 2FB warp -> YC0 (states $14/$15, $1E/$1F, depth 2FB*32) ----
        {
            int64_t b = (AL(w[0x16]) | w[0x17]) & M56;
            w[0x16] = A1r(inc2); w[0x17] = A0(inc2);
            int64_t y48 = asr56((inc2 - b) & M56, 5);
            int64_t a = b;
            b = (AL(w[0x14]) | w[0x15]) & M56;
            int64_t x0d = A1r(asr56(AL(Kw[6]), 11));           // 2FB*32
            int64_t fb = (AL(w[0x1E]) | w[0x1F]) & M56;
            for (int i = 0; i < 32; ++i) {
                b = (b + y48) & M56;
                b = (b + fb) & M56;
                b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;
                b = (b + AL(0x14A000)) & M56;
                int32_t r3 = A1r(b);
                b = (b - fb) & M56;
                int32_t x1 = sineRead(r3);
                YC0[i] = x1;
                fb = mpy_ss(x1, x0d);
            }
            w[0x14] = A1r(b); w[0x15] = A0(b);
            w[0x1E] = A1r(fb); w[0x1F] = A0(fb);
        }
        // ---- mod-2 diff (state $1D) ----
        {
            int64_t x0 = w[0x1D];
            for (int i = 0; i < 32; ++i) {
                int64_t t = AL(YC0[i]);
                int32_t nx = A1(t);
                t = (t - AL(x0)) & M56;
                YC0[i] = A1(t);
                x0 = nx;
            }
            w[0x1D] = (int32_t)x0;
        }
        // ---- mod-2 fixed LP 0.5 (state $2B) ----
        {
            int64_t y0c = 0x400000;
            int64_t aa = AL(w[0x2B]);
            int64_t x0 = YC0[0];
            for (int i = 0; i < 32; ++i) {
                int32_t x1l = A1(aa);
                YC0[i] = x1l;
                aa = mac_ss(aa, y0c, x0);
                aa = (aa - 2 * (int64_t)s24(x1l) * s24(y0c)) & M56;
                if (i < 31) x0 = YC0[i + 1];
            }
            w[0x2B] = A1(aa);
        }

        // ---- weights: w1 = 4*(2*1VOL^2 + $2C) ; 2ENV decay on $2D ; w2 = 4*(2*2ENV^2)*lvl ----
        int64_t y0w, y1w;
        {
            int64_t x0 = Kw[2];
            int64_t aa = mpy_ss(x0, x0);
            int64_t bb2 = AL(w[0x2C]);
            int64_t acc = (aa + bb2) & M56;
            acc = (acc << 2) & M56;
            y0w = A1(acc);
            // 2ENV decay on $2D (gated by 2ENV = K5)
            int64_t y1 = w[0x2D];
            if (Kw[5] >= 0x400000) {
                int64_t t = (AL(Kw[5]) - AL(0x400000)) & M56;
                int64_t aa2 = AL(0x7FFFFF);
                int64_t x0b = A1(t);
                int64_t b3 = mpy_ss(x0b, x0b);
                x0b = A1(b3);
                aa2 = (aa2 - 2 * (int64_t)s24(x0b) * s24(x0b)) & M56;
                x0b = A1(aa2);
                aa2 = mpy_ss(x0b, y1);
                y1 = A1(aa2);
                w[0x2D] = (int32_t)y1;
            }
            int64_t x0c = Kw[5];
            int64_t aa3 = (mpy_ss(x0c, x0c) << 2) & M56;
            int64_t x0d = A1(aa3);
            aa3 = mpy_ss(x0d, y1);
            y1w = A1(aa3);
        }

        // ---- mix -> X:$80 ----
        for (int i = 0; i < 32; ++i) {
            int64_t aa = mpy_ss(y0w, Y80[i]);
            int64_t bb3 = mpy_ss(y1w, YC0[i]);
            bb3 = (bb3 + aa) & M56;
            X80[i] = A1(bb3);
        }
        // ---- mix fixed LP 0.5 in place (state $29) ----
        {
            int64_t y0c = 0x400000;
            int64_t aa = AL(w[0x29]);
            int64_t x0 = X80[0];
            for (int i = 0; i < 32; ++i) {
                int32_t y1l = A1(aa);
                X80[i] = y1l;
                aa = mac_ss(aa, y0c, x0);
                aa = (aa - 2 * (int64_t)s24(y1l) * s24(y0c)) & M56;
                if (i < 31) x0 = X80[i + 1];
            }
            w[0x29] = A1(aa);
        }

        // ---- carrier loop (phase $18/$19, inc state $1A/$1B = L5) ----
        {
            int64_t a = L5;
            int64_t b = (AL(w[0x1A]) | w[0x1B]) & M56;
            w[0x1A] = A1r(a); w[0x1B] = A0(a);
            int64_t delta = asr56((a - b) & M56, 5);
            int64_t y48 = delta;
            a = b;
            b = (AL(w[0x18]) | w[0x19]) & M56;
            for (int i = 0; i < 32; ++i) {
                L20X[i] = A1r(b); L20Y[i] = A0(b);
                a = (a + y48) & M56;
                b = (b + a) & M56;
                int64_t a_save = a;
                int64_t am = asr56(AL(X80[i]), 12);
                b = (b + am) & M56;
                b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;
                b = (b + AL(0x14A000)) & M56;
                a = a_save;
            }
            w[0x18] = A1r(b); w[0x19] = A0(b);
        }
        // ---- carrier interp -> X:$E0 (asr #$3) ----
        for (int i = 0; i < 32; ++i) {
            int32_t s0 = sineRead(L20X[i]);
            int32_t s1 = sineRead(L20X[i] + 1);
            int64_t acc = (2 * ((int64_t)s24(s0) << 24)) & M56;
            acc = (acc + 2 * (int64_t)s24(s1) * L20Y[i]) & M56;
            acc = (acc - 2 * (int64_t)s24(s0) * L20Y[i]) & M56;
            XE0[i] = A1(asr56(acc, 3));
        }

        // ---- rotator 1 (XDE preloads $21/$22, chain $25/$26) ----
        int32_t XDE[40] = {0};
        XDE[0] = w[0x21]; XDE[1] = w[0x22];
        for (int i = 0; i < 32; ++i) XDE[2 + i] = XE0[i];
        int32_t (&Y1Er)[40] = this->Y1E;
        {
            const int64_t x1c = 0x0CFCE3, y0c2 = 0x2BC9CA;
            int64_t y1 = w[0x25];
            int64_t bb4 = AL(w[0x26]);
            int64_t aa = AL(XDE[0]);
            int64_t x0 = XDE[2];
            int chain = 0;
            for (int k = 0; k < 16; ++k) {
                int64_t y1l = y1;
                aa = mac_ss(aa, y0c2, x0);
                x0 = XDE[3 + 2 * k];
                Y1Er[chain++] = (int32_t)y1l;
                aa = (aa - 2 * (int64_t)s24(y1) * s24(y0c2)) & M56;
                int32_t bn = XDE[1 + 2 * k];
                y1 = A1(bb4);
                bb4 = AL(bn);
                int64_t y1l2 = y1;
                bb4 = mac_ss(bb4, x1c, x0);
                x0 = XDE[4 + 2 * k];
                Y1Er[chain++] = (int32_t)y1l2;
                bb4 = (bb4 - 2 * (int64_t)s24(y1) * s24(x1c)) & M56;
                int32_t an = XDE[2 + 2 * k];
                y1 = A1(aa);
                aa = AL(an);
            }
            Y1Er[32] = (int32_t)y1;
            Y1Er[33] = A1s(bb4);
            w[0x25] = (int32_t)y1;
            w[0x26] = A1s(bb4);
            w[0x21] = A1s(aa);
            w[0x22] = XDE[33];
        }
        // ---- rotator 2 (preloads $23/$24, chain $27/$28) ----
        {
            const int64_t y1c = 0x4E63DF, x0c = 0x6F0F12;
            int32_t Ym[40] = {0};
            Ym[0] = w[0x23]; Ym[1] = w[0x24];
            for (int i = 2; i < 40; ++i) Ym[i] = Y1Er[i];
            int64_t y1 = y1c;
            int64_t x1 = w[0x27];
            int64_t bb4 = AL(w[0x28]);
            int64_t x0 = x0c;
            int64_t y0 = Ym[2];
            int64_t aa = AL(Ym[0]);
            int32_t (&X1Er)[40] = this->X1E;
            for (int k = 0; k < 16; ++k) {
                int32_t x1l = (int32_t)x1;
                aa = mac_ss(aa, y0, x0);
                X1Er[2 * k] = x1l;
                y0 = Ym[3 + 2 * k];
                aa = (aa - 2 * (int64_t)s24(x1) * s24(x0c)) & M56;
                x1 = A1(bb4);
                bb4 = AL(Ym[1 + 2 * k]);
                int32_t x1l2 = (int32_t)x1;
                bb4 = mac_ss(bb4, y0, y1c);
                X1Er[1 + 2 * k] = x1l2;
                y0 = Ym[4 + 2 * k];
                bb4 = (bb4 - 2 * (int64_t)s24(y1c) * s24(x1)) & M56;
                x1 = A1(aa);
                aa = AL(Ym[2 + 2 * k]);
            }
            X1Er[32] = (int32_t)x1;
            X1Er[33] = A1s(bb4);
            w[0x27] = (int32_t)x1;
            w[0x28] = A1s(bb4);
            w[0x23] = A1s(aa);
            w[0x24] = Ym[33];

            // ---- output ----
            int32_t Yout[20] = {0};
            {
                const int64_t y0g = 0x400000;
                int r0i2 = 0, r4i2 = 0;
                int64_t aa2 = 0, bb5 = 0;
                int64_t x0g = X1Er[2 + r0i2]; r0i2++;
                for (int k = 0; k < 8; ++k) {
                    aa2 = mpy_ss(y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    aa2 = mac_ss(aa2, y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    Yout[r4i2++] = A1(bb5);
                    bb5 = mpy_ss(y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    bb5 = mac_ss(bb5, y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    Yout[r4i2++] = A1(aa2);
                }
                Yout[r4i2++] = A1(bb5);
            }
            for (int k = 0; k < 8; ++k) {
                int64_t aa2 = asr56(AL(Yout[1 + 2 * k]), 1);
                int64_t bb5 = asr56(AL(Yout[2 + 2 * k]), 1);
                out[4 * k + 0] = A1(aa2);
                out[4 * k + 1] = A1(aa2);
                out[4 * k + 2] = A1(bb5);
                out[4 * k + 3] = A1(bb5);
            }
        }
    }
};
