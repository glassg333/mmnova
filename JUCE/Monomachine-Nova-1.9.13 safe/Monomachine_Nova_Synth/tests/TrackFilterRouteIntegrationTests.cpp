// Integration regression for the actual TrackFILT.inl dispatch body.
// It supplies counted stand-ins for the three renderer objects, then includes
// the production inl unchanged. No JUCE dependency is required.
#include "models/DspModes.hpp"
#include "dsp/hybrid_private/HybridDSP.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>

namespace nova {
namespace {
void require(bool value, const char* message)
{
    if (!value) {
        std::fprintf(stderr, "TrackFilterRouteIntegrationTests: %s\n", message);
        std::exit(1);
    }
}

struct CountedPhysicalCore {
    int resetCalls = 0;
    int processCalls = 0;
    void reset() noexcept { ++resetCalls; }
    void setParameters(float, float, float, float, float, float, float, float) noexcept {}
    void setExternalFilterModifiers(float, float, float, float) noexcept {}
    float process(int, float input) noexcept { ++processCalls; return input; }
};

struct CountedLegacyCore {
    int processCalls = 0;
    void setExternalFilterModifiers(float, float, float, float) noexcept {}
    void processStereo(float*, float*, float*, float*, int) noexcept { ++processCalls; }
};

struct InactiveSaturation {
    bool active() const noexcept { return false; }
    float process(int, float input) noexcept { return input; }
};

struct ConstantEnv {
    float tick() noexcept { return 0.0f; }
};
}

class TrackChain {
public:
    void stageFILT(float* l, float* r, int n);

    float lowerFilterSemitones() const noexcept { return 0.0f; }
    float upperFilterSemitones() const noexcept { return 0.0f; }
    bool hasNeutralMnmFilter() const noexcept { return false; }

    int filterMode = monomachine::dspModeOld;
    int hybridLowerMode = hybrid_private::hybridNative;
    int hybridUpperMode = hybrid_private::hybridNative;
    bool neutralFilterThru = false;
    bool neutralFilterThruActive = false;
    float filterEnvMix = 0.0f;
    float filterEnvBase = 0.0f;
    float filterEnvWidth = 0.0f;
    ConstantEnv filterModEnv{};
    std::array<float, 32> params{};
    CountedLegacyCore filter{};
    CountedPhysicalCore mnmFilter{};
    CountedPhysicalCore independentPhysicalFilter{};
    InactiveSaturation filterSaturation{};
};

#include "dsp/TrackFILT.inl"

namespace {
constexpr int kFrames = 11;

void resetCounts(TrackChain& chain)
{
    chain.filter.processCalls = 0;
    chain.mnmFilter.processCalls = 0;
    chain.independentPhysicalFilter.processCalls = 0;
}

void run(TrackChain& chain)
{
    std::array<float, kFrames> left{};
    std::array<float, kFrames> right{};
    left.fill(0.25f);
    right.fill(-0.17f);
    chain.stageFILT(left.data(), right.data(), kFrames);
}

void requireCounts(const TrackChain& chain, int legacy, int mnm, int independent,
                   const char* message)
{
    require(chain.filter.processCalls == legacy
            && chain.mnmFilter.processCalls == mnm
            && chain.independentPhysicalFilter.processCalls == independent, message);
}
}

int runTrackFilterRouteIntegrationTests()
{
    TrackChain chain;

    // The two native alternatives are still available only for NATIVE/NATIVE.
    chain.filterMode = monomachine::dspModeOld;
    chain.hybridLowerMode = hybrid_private::hybridNative;
    chain.hybridUpperMode = hybrid_private::hybridNative;
    run(chain);
    requireCounts(chain, kFrames, 0, 0,
                  "OLD + NATIVE/NATIVE must invoke only the legacy renderer");

    resetCounts(chain);
    chain.filterMode = monomachine::dspModeMnm;
    run(chain);
    requireCounts(chain, 0, 2 * kFrames, 0,
                  "MNM + NATIVE/NATIVE must invoke only the MNM native renderer");

    // One K35 selection is an independent route at either FILT DSP value.
    // The actual TrackFILT body above must call it exactly once per channel,
    // with no OLD or MNM-native process call before or after it.
    for (const int mode : {monomachine::dspModeOld, monomachine::dspModeMnm}) {
        resetCounts(chain);
        chain.filterMode = mode;
        chain.hybridLowerMode = hybrid_private::hybridK35Hp;
        chain.hybridUpperMode = hybrid_private::hybridNative;
        run(chain);
        requireCounts(chain, 0, 0, 2 * kFrames,
                      "selected K35 MODE L must not be serially rendered by OLD/MNM");
    }

    // The same exclusive route applies to an R family on the upper side.
    for (const int mode : {monomachine::dspModeOld, monomachine::dspModeMnm}) {
        resetCounts(chain);
        chain.filterMode = mode;
        chain.hybridLowerMode = hybrid_private::hybridNative;
        chain.hybridUpperMode = hybrid_private::hybridRDvalLp4;
        run(chain);
        requireCounts(chain, 0, 0, 2 * kFrames,
                      "selected R MODE H must not be serially rendered by OLD/MNM");
    }

    std::puts("TRACKFILTROUTE PASS: production TrackFILT selects one exclusive renderer");
    return 0;
}
} // namespace nova

int main()
{
    return nova::runTrackFilterRouteIntegrationTests();
}
