// Smoke test for the final MNM JUCE pack (final_pack/*.h)
// Cross-checks against emulator-measured values from VERIFICATION_REPORT_RU.md
#include "MnmFixed.h"
#include "MnMAmpDist.h"
#include "MnMFilter24.h"
#include "MnMVoiceChain.h"
#include <cstdio>
#include <cmath>

using namespace mnmsys;
static int fails = 0;
static void check(const char* what, double got, double want, double tol)
{
    bool ok = std::fabs(got - want) <= tol;
    printf("%-46s got %.7f want %.7f %s\n", what, got, want, ok ? "OK" : "FAIL");
    if (!ok) ++fails;
}

int main()
{
    // ---- 1. DIST law (measured on emulator: gain = 4.000*k, k law) ----
    {
        MnMDist d;
        const float Ds[4] = { 0.5f, 0.626f, 0.752f, 0.878f };
        const float peaks[4] = { 0.032f, 0.157f, 0.532f, 1.0f };
        for (int t = 0; t < 4; ++t)
        {
            d.setDist(Ds[t]);
            float in[16], out[16];
            for (int i = 0; i < 16; ++i) in[i] = 0.5f * std::sin(2.0*M_PI*3.0*i/16.0);
            float c = d.processBlock16(in, out, 16);
            double peak = 0; for (int i = 0; i < 16; ++i) peak = std::max(peak, std::fabs((double)out[i]));
            printf("D=%.3f k=%.4f c=%.4f peak=%.4f (emu peak~%.3f) x0=%.3f\n",
                   Ds[t], d.k(), c, peak, peaks[t], d.x0());
            if (t == 0) check("DIST D=0.5 passive (x0==0)", d.x0(), 0.0, 1e-6);
        }
        d.setDist(0.5f);
        check("DIST k(0) = 0.0162109", d.k(), 0.0162109, 1e-7);
        check("DIST c(0) = 1.0", d.c(), 1.0, 1e-6);
        d.setDist(1.0f);
        check("DIST k(1) = 1.0000", d.k(), 1.0, 1e-6);
        check("DIST c(255) = sqrt(2)/4", d.c(), 0.3535534, 1e-6);
    }

    // ---- 2. tables verbatim anchors ----
    check("drive[0] == 1.0",        q23ToF(kDriveCurve129[0]),   1.0, 1e-6);
    check("drive[128] == sqrt2/4",  q23ToF(kDriveCurve129[128]), 0.3535534, 1e-6);
    check("cutoff[0] == 0.000403",  q23ToF(kCutoffTable1728[0]), 0.0004030, 1e-6);
    check("decay[127] == -1.0",     q23ToF(kAmpDecayTable128[127]), -1.0, 0.0);
    check("rate[0] == 0.5 (ATK=0: +0.5/blk)", q23ToF(kAmpRateTable128[0]), 0.5, 0.0);

    // ---- 3. env: ATK=0 -> level 0.5 after 1 block; DEC multiplies ----
    {
        MnMAmpEnv e;
        e.setParams(0, 0, 16, 16);
        e.trigger();
        e.tickBlock();
        check("AMP env ATK=0 after 1 blk = 0.5", e.level(), 0.5, 1e-6);
        e.tickBlock(); e.tickBlock();
        check("AMP env ATK=0 after 3 blk = 1.0", e.level(), 1.0, 1e-6);
    }

    // ---- 4. filter: cutoff table top ~ 6.6 kHz ----
    {
        MnMFilter24 f;
        float in[16], out[16];
        for (int i = 0; i < 16; ++i) in[i] = 0.5f * std::sin(2.0*M_PI*3.0*i/16.0);
        f.setPages(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f);
        f.processBlock16(in, out, 16, true);
        printf("filter open: f=%.4f (TBL top=0.9131 => 2sin(pi*6656/44100))\n", f.currentF());
        check("filter top coeff == 0x741D05 (idx1699)", f.currentF(), q23ToF(0x741D05), 1e-3);
        // low cutoff
        f.setPages(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f);
        f.processBlock16(in, out, 16, true);
        check("filter min coeff ~= 0x000D35", f.currentF(), q23ToF(0x000D35), 1e-4);
    }

    // ---- 5. voice chain end-to-end ----
    {
        MnMVoiceChain vc;
        MnmVoiceParams p;
        p.dist = 0.5f; p.vol = 1.0f; p.pan = 0.5f;
        p.fBase = 1.0f; p.fWdth = 0.0f;
        vc.setParams(p);
        vc.setSustain(0.0f);
        vc.noteOn();
        float in[16], L[16], R[16];
        for (int i = 0; i < 16; ++i) in[i] = 0.5f * std::sin(2.0*M_PI*3.0*i/16.0);
        vc.processBlock16(in, L, R, 16);
        double peak = 0; for (int i = 0; i < 16; ++i) peak = std::max(peak, std::fabs((double)L[i]));
        printf("chain block1 peak L=%.4f R=%.4f env=%.4f\n", peak,
               std::fabs((double)R[0]), vc.envLevel());
        // пан по центру -> L == R
        check("chain pan center: L==R", std::fabs(L[7]) == std::fabs(R[7]) ? 1.0 : 0.0, 1.0, 1e-9);
    }

    printf(fails ? "\n%d CHECKS FAILED\n" : "\nALL CHECKS PASSED\n", fails);
    return fails ? 1 : 0;
}
