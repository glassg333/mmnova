// test_mnm_ring.cpp — sanity check of the recovered RingCore
#include "MnmRing.hpp"
#include <cstdio>
#include <vector>
#include <cmath>

using monomachine::mnm::RingCore;

static double rms(const std::vector<float>& v) {
    double s = 0; for (float x : v) s += double(x) * x;
    return std::sqrt(s / std::max<size_t>(1, v.size()));
}

int main() {
    RingCore ring;
    ring.reset(44100.0);
    ring.setParameters(0 /*WAVE*/, 0 /*EXT*/, 127 /*MIX*/, 64 /*INP*/);

    // 1) carrier frequency check: ring of a slow sine by 1 kHz carrier -> product
    //    spectrum contains sum/difference; instead measure zero-crossing rate of
    //    the wet signal with DC input (constant input exposes the carrier shape).
    {
        std::vector<float> inL(44100, 0.25f), inR(44100, 0.25f), outL(44100), outR(44100);
        ring.setCarrierFrequency(1000.0f);
        ring.processStereo(inL.data(), inR.data(), outL.data(), outR.data(), 44100);
        int zc = 0;
        for (size_t i = 4411; i < outL.size(); ++i)
            if ((outL[i - 1] < 0) != (outL[i] < 0)) ++zc;
        double win = double(outL.size() - 4411) / 44100.0;
        double f_est = (zc / 2.0) / win; // two crossings per period, window-scaled
        printf("carrier 1000 Hz -> measured %.1f Hz (zero crossings), out RMS %.4f\n",
               f_est, rms(outL));
        if (std::abs(f_est - 1000.0) > 15.0) { printf("FAIL carrier freq\n"); return 1; }
    }

    // 2) keytrack clamp: 20000 Hz must clamp to 16500
    {
        ring.reset(44100.0);
        ring.setParameters(0, 0, 127, 64);
        std::vector<float> inL(44100, 0.25f), inR(44100, 0.25f), outL(44100), outR(44100);
        ring.setCarrierFrequency(20000.0f);
        ring.processStereo(inL.data(), inR.data(), outL.data(), outR.data(), 44100);
        int zc = 0;
        for (size_t i = 4411; i < outL.size(); ++i)
            if ((outL[i - 1] < 0) != (outL[i] < 0)) ++zc;
        double win = double(outL.size() - 4411) / 44100.0;
        double f_est = (zc / 2.0) / win;
        printf("carrier 20000 Hz -> measured %.1f Hz (clamp 16500)\n", f_est);
        if (std::abs(f_est - 16500.0) > 60.0) { printf("FAIL clamp\n"); return 1; }
    }

    // 3) INP law: unity at 64 (gain ~1.0 on the carrier-quiet path is hard to
    //    isolate; check INP=0 silences the wet path completely at MIX=127)
    {
        ring.reset(44100.0);
        ring.setParameters(0, 0, 127, 0);
        std::vector<float> inL(4410, 0.5f), inR(4410, 0.5f), outL(4410), outR(4410);
        ring.setCarrierFrequency(440.0f);
        ring.processStereo(inL.data(), inR.data(), outL.data(), outR.data(), 4410);
        double tail = rms(std::vector<float>(outL.begin() + 2205, outL.end())); // post-ramp
        printf("INP=0 wet tail RMS %.6f (expect 0)\n", tail);
        if (tail > 1e-4) { printf("FAIL INP=0\n"); return 1; }
    }

    // 4) EXT=127: carrier = opposite channel; with L silent and R constant,
    //    outL must be silent (L*ext(R)=L*const=0) and outR = R*R*gain (DC block
    //    absent -> non-zero)
    {
        ring.reset(44100.0);
        ring.setParameters(0, 127, 127, 64);
        std::vector<float> inL(4410, 0.0f), inR(4410, 0.25f), outL(4410), outR(4410);
        ring.setCarrierFrequency(440.0f);
        ring.processStereo(inL.data(), inR.data(), outL.data(), outR.data(), 4410);
        printf("EXT=127: outL RMS %.6f (expect 0), outR RMS %.4f (expect ~%.4f)\n",
               rms(outL), rms(outR), 0.25 * 4.0 * (64.0 / 127.0) * (64.0 / 127.0) * 0.25);
    }

    // 5) MIX=0 passes dry
    {
        ring.reset(44100.0);
        ring.setParameters(0, 0, 0, 64);
        std::vector<float> inL(4410), inR(4410), outL(4410), outR(4410);
        for (int i = 0; i < 4410; ++i) { inL[i] = inR[i] = 0.5f * std::sin(2 * M_PI * 220 * i / 44100.0); }
        ring.setCarrierFrequency(440.0f);
        ring.processStereo(inL.data(), inR.data(), outL.data(), outR.data(), 4410);
        double err = 0; for (int i = 2205; i < 4410; ++i) err += std::abs(outL[i] - inL[i]);
        printf("MIX=0 tail mean |out-in| = %.2e (expect ~0)\n", err / 2205);
        if (err / 2205 > 1e-3) { printf("FAIL dry path\n"); return 1; }
    }

    printf("ALL RING CORE SANITY CHECKS PASSED\n");
    return 0;
}
