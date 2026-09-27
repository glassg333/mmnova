// MnmFmPar.hpp — FM+PAR (m9) instruction-faithful mirror of P:$145EDB-$14619C.
// Included by MnmFmExact.hpp (same helpers/tables). 3 FM blocks + shared tail.
#pragma once
// (this file is #included INSIDE namespace mnmfm by MnmFmExact.hpp)

struct FmParVoice {
    int32_t w[0x40];
    int32_t L20X[32], L20Y[32];
    int32_t X80[32], Y80[32], YC0[32], XE0[32];
    int32_t Y1E[40], X1E[40];

    FmParVoice() { init(); }
    void init() {                                    // $145EC9 + $145ED4
        for (int i = 0; i < 0x40; ++i) w[i] = 0;
        w[0x10] = 0x14A000; w[0x14] = 0x14A000;
        w[0x18] = 0x14A000; w[0x1C] = 0x14A000;
        w[0x31] = 0x0022F800;                       // NOT covered by init's zero
                                                    // range ($11..$30): SRAM residue
        w[0x32] = 0x7FFFFF; w[0x33] = 0x7FFFFF; w[0x34] = 0x7FFFFF;
    }

    // one modulator block: phase loop + interp + diff + TONE LP + shaper
    // phaseW/incW: page offsets of the 48-bit phase and inc states
    // diffW: 24-bit diff state; lpW: 24-bit tone LP state; shpW: shaper state
    // envK: gated knob word for the shaper (2VOL-style), or -1 for none
    void modBlock(int64_t inc_new, int phaseW, int incW, int diffW, int lpW,
                  int shpW, int64_t envK, int64_t toneCoeff, int32_t (&outBuf)[32])
    {
        // ---- phase loop (store lag: b,l:(r1)+ latched PRE-ALU) ----
        int64_t b = (AL(w[incW]) | w[incW + 1]) & M56;
        w[incW] = A1r(inc_new); w[incW + 1] = A0(inc_new);
        int64_t delta = asr56((inc_new - b) & M56, 5);
        int64_t y48 = delta;
        int64_t a = b;                               // tfr b,a
        b = (AL(w[phaseW]) | w[phaseW + 1]) & M56;
        for (int i = 0; i < 32; ++i) {
            L20X[i] = A1r(b); L20Y[i] = A0(b);
            a = (a + y48) & M56;
            b = (b + a) & M56;
            b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;
            b = (b + AL(0x14A000)) & M56;
        }
        w[phaseW] = A1r(b); w[phaseW + 1] = A0(b);
        (void)phaseW; (void)incW;

        // ---- interp -> Y:$80 (same math as STAT: acc = 2*s0<<24 + 2*(s1-s0)*frac, >>1) ----
        for (int i = 0; i < 32; ++i) {
            int32_t s0 = sineRead(L20X[i]);
            int32_t s1 = sineRead(L20X[i] + 1);
            int64_t acc = (2 * ((int64_t)s24(s0) << 24)) & M56;
            acc = (acc + 2 * (int64_t)s24(s1) * L20Y[i]) & M56;
            acc = (acc - 2 * (int64_t)s24(s0) * L20Y[i]) & M56;
            outBuf[i] = A1(asr56(acc, 1));
        }

        // ---- first-difference cascade (interleaved stream, one state) ----
        {
            int64_t x0 = w[diffW];
            for (int i = 0; i < 32; ++i) {
                int64_t bb = AL(outBuf[i]);
                int32_t nx = A1(bb);                 // b,x0 / a,x0 latched pre-ALU
                bb = (bb - AL(x0)) & M56;
                outBuf[i] = A1(bb);
                x0 = nx;
            }
            w[diffW] = (int32_t)x0;
        }

        // ---- TONE one-pole (state lpW) ----
        {
            int64_t aa = AL(w[lpW]);
            int64_t x0 = outBuf[0];
            for (int i = 0; i < 32; ++i) {
                int32_t x1l = A1(aa);
                outBuf[i] = x1l;
                aa = mac_ss(aa, toneCoeff, x0);
                aa = (aa - 2 * (int64_t)s24(x1l) * s24(toneCoeff)) & M56;
                if (i < 31) x0 = outBuf[i + 1];
            }
            w[lpW] = A1(aa);
        }

        // ---- gated shaper (state shpW): out = in + 2*y1*prev_in <<4-ish ----
        if (envK >= 0) {
            int64_t y1 = (envK >= 0x400000) ? (envK - 0x400000) & M24 : 0;
            int64_t x0 = w[shpW];
            for (int i = 0; i < 32; ++i) {
                int64_t aa = (mpy_ss(x0, y1) << 4) & M56;
                int32_t x1 = outBuf[i];
                aa = (aa + AL(x1)) & M56;
                int32_t x0n = x1;
                outBuf[i] = A1(aa);
                x0 = x0n;
            }
            w[shpW] = (int32_t)x0;
        }
    }

    // FM+ envelope decay for one block; returns the new level, weights via 8*K^2
    static void envDecay(int64_t K, int32_t& lvl, int64_t& weight) {
        int64_t y1 = lvl;
        if (K >= 0x400000) {
            int64_t bb = (AL(K) - AL(0x400000)) & M56;
            int64_t aa = AL(0x7FFFFF);
            int64_t x0 = A1(bb);
            int64_t b2 = mpy_ss(x0, x0);
            x0 = A1(b2);
            aa = (aa - 2 * (int64_t)s24(x0) * s24(x0)) & M56;
            x0 = A1(aa);
            aa = mpy_ss(x0, y1);
            y1 = A1(aa);
            lvl = (int32_t)y1;
        }
        int64_t aa = (mpy_ss(K, K) << 2) & M56;
        int64_t x0 = A1(aa);
        aa = mpy_ss(x0, y1);
        weight = A1(aa);
    }

    void process(const int knobs[8], int32_t pitch, int32_t out[32]) {
        int64_t Kw[8];
        for (int i = 0; i < 8; ++i) {
            Kw[i] = (int64_t)(knobs[i] & 0xFF) << 16;
            w[4 + i] = (int32_t)Kw[i];
        }
        int n2 = (int)((w[0x0A] >> 16) & 0x7F);      // TONE knob
        int64_t toneC = kToneCoeff[n2];

        // ---- $145edb: L5 + 3 modulator increments (no x2, no fin in PAR) ----
        int64_t L5 = mpysu_dmac(0x0BE37C, AL(pitch));
        int64_t inc[3];
        const int ratioKnob[3] = {0, 2, 4};          // 1FRQ 2FRQ 3FRQ at r6+$4/$6/$8
        for (int m = 0; m < 3; ++m) {
            int64_t b = mpy_ss(0x18, (Kw[ratioKnob[m]] + 0x8000) & M24);
            int n1 = A1r(b);
            int64_t raw = (n1 >= 0 && n1 < 24) ? kRatioRaw[n1] : 0;
            inc[m] = (mpysu_dmac(raw, L5 & ((1ll << 48) - 1)) << 3) & M56;
        }

        // ---- 3 modulator blocks ----
        // states (verified against the listing): phases $10/$14/$18,
        // incs $12/$16/$1A, diffs $20/$21/$22, LPs $2C/$2D/$2E,
        // shapers $2F/$30/$31; env levels $32/$33/$34; mix LP $2B.
        modBlock(inc[0], 0x10, 0x12, 0x20, 0x2C, 0x2F, Kw[1], toneC, Y80);
        modBlock(inc[1], 0x14, 0x16, 0x21, 0x2D, 0x30, Kw[3], toneC, YC0);
        modBlock(inc[2], 0x18, 0x1A, 0x22, 0x2E, 0x31, Kw[5], toneC, XE0);

        if (gCkpt) gCkpt(20, this);
        // ---- env decays + weights (1ENV/2ENV/3ENV = K1/K3/K5, levels $32/$33/$34) ----
        int64_t wgt[3];
        envDecay(Kw[1], w[0x32], wgt[0]);
        envDecay(Kw[3], w[0x33], wgt[1]);
        envDecay(Kw[5], w[0x34], wgt[2]);

        // ---- 3-way mix -> X:$80 (weights 2*wgt[m]*mod[m]) ----
        for (int i = 0; i < 32; ++i) {
            int64_t aa = mpy_ss(wgt[0], Y80[i]);
            aa = mac_ss(aa, wgt[1], YC0[i]);
            aa = mac_ss(aa, wgt[2], XE0[i]);
            X80[i] = A1(aa);
        }

        if (gCkpt) gCkpt(21, this);
        // ---- mix TONE LP in place in X:$80, state $2B ----
        {
            int64_t aa = AL(w[0x2B]);
            int64_t x0 = X80[0];
            for (int i = 0; i < 32; ++i) {
                int32_t y1l = A1(aa);
                X80[i] = y1l;
                aa = mac_ss(aa, toneC, x0);
                aa = (aa - 2 * (int64_t)s24(y1l) * s24(toneC)) & M56;
                if (i < 31) x0 = X80[i + 1];
            }
            w[0x2B] = A1(aa);
        }

        if (gCkpt) gCkpt(22, this);
        // ---- carrier loop (phase $1C/$1D, inc state $1E/$1F) ----
        {
            int64_t a = L5;
            int64_t b = (AL(w[0x1E]) | w[0x1F]) & M56;
            w[0x1E] = A1r(a); w[0x1F] = A0(a);
            int64_t delta = asr56((a - b) & M56, 5);
            int64_t y48 = delta;
            a = b;
            b = (AL(w[0x1C]) | w[0x1D]) & M56;
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
            w[0x1C] = A1r(b); w[0x1D] = A0(b);
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

        // ---- rotator 1 (states $23/$24 y-chain, $27/$28) — same as STAT ----
        int32_t XDE[40] = {0};
        XDE[0] = w[0x23]; XDE[1] = w[0x24];
        for (int i = 0; i < 32; ++i) XDE[2 + i] = XE0[i];
        int32_t (&Y1Er)[40] = this->Y1E;
        {
            const int64_t x1c = 0x0CFCE3, y0c2 = 0x2BC9CA;
            int64_t y1 = w[0x27];
            int64_t bb = AL(w[0x28]);
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
                y1 = A1(bb);
                bb = AL(bn);
                int64_t y1l2 = y1;
                bb = mac_ss(bb, x1c, x0);
                x0 = XDE[4 + 2 * k];
                Y1Er[chain++] = (int32_t)y1l2;
                bb = (bb - 2 * (int64_t)s24(y1) * s24(x1c)) & M56;
                int32_t an = XDE[2 + 2 * k];
                y1 = A1(aa);
                aa = AL(an);
            }
            Y1Er[32] = (int32_t)y1;
            Y1Er[33] = A1s(bb);
            w[0x27] = (int32_t)y1;
            w[0x28] = A1s(bb);
            w[0x23] = A1s(aa);
            w[0x24] = XDE[33];
        }

        // ---- rotator 2 (states $25/$26, $29/$2A) ----
        {
            const int64_t y1c = 0x4E63DF, x0c = 0x6F0F12;
            int32_t Ym[40] = {0};
            Ym[0] = w[0x25]; Ym[1] = w[0x26];
            for (int i = 2; i < 40; ++i) Ym[i] = Y1Er[i];
            int64_t y1 = y1c;
            int64_t x1 = w[0x29];
            int64_t bb = AL(w[0x2A]);
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
                x1 = A1(bb);
                bb = AL(Ym[1 + 2 * k]);
                int32_t x1l2 = (int32_t)x1;
                bb = mac_ss(bb, y0, y1c);
                X1Er[1 + 2 * k] = x1l2;
                y0 = Ym[4 + 2 * k];
                bb = (bb - 2 * (int64_t)s24(y1c) * s24(x1)) & M56;
                x1 = A1(aa);
                aa = AL(Ym[2 + 2 * k]);
            }
            X1Er[32] = (int32_t)x1;
            X1Er[33] = A1s(bb);
            w[0x29] = (int32_t)x1;
            w[0x2A] = A1s(bb);
            w[0x25] = A1s(aa);
            w[0x26] = Ym[33];

            // ---- output (same as STAT: y0 = 0.5, pair sums, asr, L=R) ----
            int32_t Yout[20] = {0};
            {
                const int64_t y0g = 0x400000;
                int r0i2 = 0, r4i2 = 0;
                int64_t aa2 = 0, bb2 = 0;
                int64_t x0g = X1Er[2 + r0i2]; r0i2++;
                for (int k = 0; k < 8; ++k) {
                    aa2 = mpy_ss(y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    aa2 = mac_ss(aa2, y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    Yout[r4i2++] = A1(bb2);
                    bb2 = mpy_ss(y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    bb2 = mac_ss(bb2, y0g, x0g);
                    x0g = X1Er[2 + r0i2]; r0i2++;
                    Yout[r4i2++] = A1(aa2);
                }
                Yout[r4i2++] = A1(bb2);
            }
            for (int k = 0; k < 8; ++k) {
                int64_t aa2 = asr56(AL(Yout[1 + 2 * k]), 1);
                int64_t bb2 = asr56(AL(Yout[2 + 2 * k]), 1);
                out[4 * k + 0] = A1(aa2);
                out[4 * k + 1] = A1(aa2);
                out[4 * k + 2] = A1(bb2);
                out[4 * k + 3] = A1(bb2);
            }
        }
    }
};
