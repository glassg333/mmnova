// Compiles the production TrackDIST.inl body with its real MODE S enum and
// real candidate transfers, without JUCE. This guards the actual dispatch, not
// only the standalone transfer functions.
#include "dsp/hybrid_private/HybridDSP.hpp"
#include "dsp/mnm/MnmKernel.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace nova {
inline float bipolarDist(float input, float raw) {
    return monomachine::mnm::oldBipolarSaturator(input, raw);
}

class TrackChain {
public:
    void stageDIST(float* l, float* r, int n);

    std::array<float, 32> params{};
    bool distSmoothingReady = false;
    float smoothedDist = 0.0f;
    float distSlew = 1.0f; // exact target during this deterministic route test
    int hybridSaturationMode = nova::hybrid_private::hybridDistMnm;
    nova::hybrid_private::OversamplingDistortionStereo hybridSaturation;
    nova::hybrid_private::MnmFixNotchCompensatorStereo mnmFixNotch;
};

#include "dsp/TrackDIST.inl"
} // namespace nova

namespace {
void require(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "TRACKDISTVARIANT FAIL: %s\n", what);
        std::exit(1);
    }
}
bool same(float a, float b) { return std::abs(a - b) < 1.0e-6f; }
}

int main() {
    constexpr std::array<int, 3> modes{{ nova::hybrid_private::hybridDistMnmOld,
                                         nova::hybrid_private::hybridDistMnmV2,
                                         nova::hybrid_private::hybridDistOldV2 }};
    constexpr std::array<float, 3> probes{{ -0.53f, 0.0f, 0.41f }};

    for (const int mode : modes) {
        nova::TrackChain chain;
        chain.hybridSaturationMode = mode;
        chain.params[12] = 64.0f;
        std::array<float, 3> l = probes;
        std::array<float, 3> r = probes;
        chain.stageDIST(l.data(), r.data(), static_cast<int>(l.size()));
        for (size_t i = 0; i < l.size(); ++i)
            require(same(l[i], probes[i]) && same(r[i], probes[i]),
                    "production MODE S candidate is not exact identity at DIST=0");
    }

    // At a positive setting, each branch must select precisely its documented
    // function rather than fall through to MNM or an imported saturation mode.
    struct Case { int mode; float (*transfer)(float, float); };
    const std::array<Case, 3> cases{{
        { nova::hybrid_private::hybridDistMnmOld, monomachine::mnm::mnmOldSaturator },
        { nova::hybrid_private::hybridDistMnmV2,  monomachine::mnm::mnmV2Saturator },
        { nova::hybrid_private::hybridDistOldV2,  monomachine::mnm::oldV2Saturator },
    }};
    for (const auto& c : cases) {
        nova::TrackChain chain;
        chain.hybridSaturationMode = c.mode;
        chain.params[12] = 96.0f;
        float l[] { 0.37f, -0.61f };
        float r[] { -0.42f, 0.18f };
        const auto expectedL0 = c.transfer(l[0], 96.0f);
        const auto expectedL1 = c.transfer(l[1], 96.0f);
        const auto expectedR0 = c.transfer(r[0], 96.0f);
        const auto expectedR1 = c.transfer(r[1], 96.0f);
        chain.stageDIST(l, r, 2);
        require(same(l[0], expectedL0) && same(l[1], expectedL1)
                && same(r[0], expectedR0) && same(r[1], expectedR1),
                "production TrackDIST dispatched an appended candidate incorrectly");
    }

    std::puts("TRACKDISTVARIANT PASS: production MODE S IDs 6..8 dispatch exact candidate laws");
    return 0;
}
