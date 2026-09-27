// Smoke test for MnmAmp.h / MnmFilter.h / MnmKernelTables.h
#include "MnmAmp.h"
#include "MnmFilter.h"
#include <cstdio>
#include <cmath>

// cutoff table from firmware dump (first 1700 words of P:$143546)
#include "cutoff_table_1700.h"

int main()
{
    using namespace mnmdsp;

    // --- AMP env: attack to full, release ---
    MnmAmpEnv env;
    MnmAmpEnv::Params p;
    p.atk = 32 << 16; p.dec = 40 << 16; p.sustainSqrt = int32_t(0.7f * 8388608); p.rel = 24 << 16;
    env.setParams(p);
    env.trigger();
    for (int b = 0; b < 12; ++b) {
        float l = float(env.step()) / 8388608.0f;
        if (b % 5 == 0) printf("env b=%2d state=%d level=%.4f\n", b, env.state(), l);
    }
    env.release();
    printf("release: b0=%.4f b5=%.4f\n",
        float(env.step())/8388608.0f, [&, s=env.state()](){ float x; for(int i=0;i<4;++i) x=float(env.step())/8388608.0f; return x; }());

    // --- DIST: DC sweep at D=64 ---
    MnmDist dist;
    dist.setKnob(64);
    printf("dist D=64 transfer:");
    for (int k = 0; k <= 8; ++k) {
        dist.reset();
        float in = -0.9f + 0.225f * k;
        float y = 0;
        for (int i = 0; i < 64; ++i) y = dist.processSample(in);  // settle state
        printf(" %.3f->%.3f", in, y);
    }
    printf("\n");

    // --- Filter: LP sweep ---
    MnmVoiceFilter vf;
    vf.attachTable(kMnmCutoffTable);
    printf("filter sweep (idx0 -> idx 1699 in 8 steps), steady-state LP gain at 440Hz:\n");
    for (int s = 0; s <= 8; ++s)
    {
        vf.reset();
        vf.setCoefficientTargets(0, s * 1699 / 8);      // HP at BASE=0 (passthrough ~2.8Hz), LP stepping
        vf.setResonance(0.0f, 0.35f);
        double g = 0;
        for (int b = 0; b < 64; ++b)
        {
            vf.beginBlock();
            double e = 0;
            for (int i = 0; i < 16; ++i)
            {
                float x = 0.25f * float(sin(2.0 * M_PI * 440.0 * (b * 16 + i) / 44100.0));
                float y = vf.processSample(x);
                e += double(y) * y;
            }
            if (b >= 48) g += e / 16.0;
        }
        g = sqrt(g / 16.0) / 0.25f;
        printf("  LP idx %4d: gain %.4f\n", s * 1699 / 8, g);
    }
    return 0;
}
