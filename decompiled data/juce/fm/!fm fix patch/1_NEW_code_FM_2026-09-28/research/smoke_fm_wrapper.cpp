// smoke_fm_wrapper.cpp — integration smoke test: FmCore wrapper (MnmFm.hpp)
// must render FM STAT through the bit-exact core. The wrapper normalises the
// Q23 words by 1/17949485, so with default knobs (all 64) and pitch word
// 11776 the first samples must equal vector words 853, 853, 26257, 26257...
#include "MnmFm.hpp"
#include <array>
#include <cmath>
#include <cstdio>

using namespace monomachine::mnm;

int main() {
    FmCore core;
    core.reset(44100.0);
    std::array<float, 8> p{64, 64, 64, 64, 64, 64, 64, 64};
    core.setParameters(FmKind::Stat, p);
    core.noteOn(60.0f);
    core.setPitchWordOverride(11776u, true);

    float buf[40] = {0};
    core.processBlock(buf, 40);
    const double q = 17949485.0;
    const double expv[6] = {853 / q, 853 / q, 26257 / q, 26257 / q, 143842 / q, 143842 / q};
    int bad = 0;
    for (int i = 0; i < 6; ++i) {
        printf("stat out[%d] = %.9f  (expected %.9f)\n", i, buf[i], expv[i]);
        if (std::fabs((double)buf[i] - expv[i]) > 1e-6) ++bad;
    }
    // PAR and DYN smoke: must render without crashing, non-silent
    float peak = 0.0f;
    core.setParameters(FmKind::Par, p);
    core.processBlock(buf, 40);
    for (int i = 0; i < 40; ++i) peak = std::max(peak, std::fabs(buf[i]));
    printf("par peak = %.6f\n", peak);
    if (peak < 1e-4f) ++bad;
    peak = 0.0f;
    core.setParameters(FmKind::Dyn, p);
    core.processBlock(buf, 40);
    for (int i = 0; i < 40; ++i) peak = std::max(peak, std::fabs(buf[i]));
    printf("dyn peak = %.6f\n", peak);
    if (peak < 1e-4f) ++bad;
    printf(bad ? "SMOKE FAIL (%d)\n" : "SMOKE OK\n", bad);
    return bad ? 1 : 0;
}
