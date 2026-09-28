// MODE S 1.9.15 A/B transfer-law regression.  This is deliberately
// JUCE-free: it proves the exact steady-state laws presented by MNM+OLD,
// MNM V2 and OLD V2 without claiming that any candidate is firmware exact.
#include "dsp/mnm/MnmKernel.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
void require(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "DISTVARIANT FAIL: %s\n", what);
        std::exit(1);
    }
}

bool same(float a, float b, float eps = 1.0e-6f) {
    return std::abs(a - b) <= eps;
}
}

int main() {
    using monomachine::mnm::Saturator;
    using monomachine::mnm::oldBipolarSaturator;
    using monomachine::mnm::mnmOldSaturator;
    using monomachine::mnm::mnmV2Saturator;
    using monomachine::mnm::oldV2Saturator;

    constexpr std::array<float, 7> probes{{-2.0f, -0.75f, -0.1f, 0.0f, 0.1f, 0.75f, 2.0f}};
    // All retained and appended MODE S laws are exact identity at LCD DIST=0
    // (raw page value 64). This directly guards the reported "MNM at zero"
    // complaint against a new candidate introducing its own coloration.
    for (const float x : probes) {
        require(same(Saturator::process(x, 64.0f), x), "MNM DIST=0 is not identity");
        require(same(oldBipolarSaturator(x, 64.0f), x), "OLD DIST=0 is not identity");
        require(same(mnmOldSaturator(x, 64.0f), x), "MNM+OLD DIST=0 is not identity");
        require(same(mnmV2Saturator(x, 64.0f), x), "MNM V2 DIST=0 is not identity");
        require(same(oldV2Saturator(x, 64.0f), x), "OLD V2 DIST=0 is not identity");
    }

    // oldBipolarSaturator is now shared by the retained OLD route and OLD V2.
    // Compare it with the pre-1.9.15 body across the whole legal raw range so
    // refactoring did not alter the OLD listening reference by accident.
    const auto previousOld = [](float input, float raw) {
        const float d = std::clamp(raw, 0.0f, 127.0f) - 64.0f;
        if (d <= 0.0f) return input * (1.0f + d * (0.4f / 64.0f));
        const float drive = 15.0f * d / 63.0f;
        if (drive < 0.0001f) return input;
        return std::tanh(input * drive) / std::tanh(drive);
    };
    for (int raw = 0; raw <= 127; ++raw)
        for (float x : probes)
            require(same(oldBipolarSaturator(x, static_cast<float>(raw)),
                         previousOld(x, static_cast<float>(raw))),
                    "retained OLD transfer changed while adding candidates");

    // Negative values remain explicit headroom laws: MNM-derived candidates
    // retain MNM's half-level endpoint, OLD V2 retains OLD's 0.6 endpoint.
    require(same(Saturator::process(1.0f, 0.0f), 0.5f), "MNM -64 headroom changed");
    require(same(mnmOldSaturator(1.0f, 0.0f), 0.5f), "MNM+OLD -64 headroom changed");
    require(same(mnmV2Saturator(1.0f, 0.0f), 0.5f), "MNM V2 -64 headroom changed");
    require(same(oldBipolarSaturator(1.0f, 0.0f), 0.6f), "OLD -64 headroom changed");
    require(same(oldV2Saturator(1.0f, 0.0f), 0.6f), "OLD V2 -64 headroom changed");

    // The positive endpoints intentionally differ for A/B. MNM+OLD has the
    // *exact* retained MNM compensation law, so at an already-clipped full-
    // scale input it has the same endpoint as MNM while its unsaturated curve
    // remains soft. MNM V2 and OLD V2 sit above it with hard/soft shape.
    const float mnm    = Saturator::process(1.0f, 127.0f);
    const float old    = oldBipolarSaturator(1.0f, 127.0f);
    const float mix    = mnmOldSaturator(1.0f, 127.0f);
    const float mnmV2  = mnmV2Saturator(1.0f, 127.0f);
    const float oldV2  = oldV2Saturator(1.0f, 127.0f);
    const float mnmAmountAtMax = 63.0f / 64.0f;
    require(old > mnmV2 && mnmV2 > oldV2 && oldV2 > mnm,
            "positive MODE S endpoint ordering is not the documented A/B set");
    require(same(mix, mnm),
            "MNM+OLD no longer shares MNM's exact positive headroom endpoint");
    require(std::abs(mix - (1.0f / (1.0f + 2.0f * mnmAmountAtMax))) < 1.0e-5f,
            "MNM+OLD no longer uses the retained MNM maximum headroom law");
    require(std::abs(mnmV2 - (1.0f / std::sqrt(1.0f + 2.0f * mnmAmountAtMax))) < 1.0e-5f,
            "MNM V2 no longer uses its documented sqrt compensation");
    require(!same(mnmOldSaturator(0.5f, 80.0f), Saturator::process(0.5f, 80.0f)),
            "MNM+OLD no longer differs from MNM on the intended soft-shape comparison");
    require(std::abs(oldV2 - (1.0f / 1.75f)) < 1.0e-5f,
            "OLD V2 no longer uses its documented positive trim");

    // All finite input/raw combinations remain finite; candidates may differ
    // in sound, but they must not create state or NaN failure modes.
    for (int raw = 0; raw <= 127; ++raw) for (float x = -4.0f; x <= 4.001f; x += 0.03125f) {
        require(std::isfinite(mnmOldSaturator(x, static_cast<float>(raw))), "MNM+OLD non-finite");
        require(std::isfinite(mnmV2Saturator(x, static_cast<float>(raw))), "MNM V2 non-finite");
        require(std::isfinite(oldV2Saturator(x, static_cast<float>(raw))), "OLD V2 non-finite");
    }

    std::printf("DISTVARIANT PASS: MNM+OLD / MNM V2 / OLD V2 identity, headroom, endpoints, finite sweep\n");
    return 0;
}
