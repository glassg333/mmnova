// Direct regression for the FILT DSP / physical-filter routing contract.
// This target has no JUCE dependency. TrackFILT.inl calls the same
// dispatchFilterRenderRoute helper exercised here.
#include "dsp/hybrid_private/HybridDSP.hpp"
#include "dsp/mnm/MnmRealFilter.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
void require(bool condition, const char* message)
{
    if (!condition) {
        std::fprintf(stderr, "FilterRouteTests: %s\n", message);
        std::exit(1);
    }
}

struct Invocations {
    int oldRenderer = 0;
    int mnmNativeRenderer = 0;
    int independentRenderer = 0;
};

Invocations dispatch(bool mnmSelected, int lowerMode, int upperMode)
{
    Invocations result;
    nova::hybrid_private::dispatchFilterRenderRoute(
        mnmSelected, lowerMode, upperMode,
        [&] { ++result.oldRenderer; },
        [&] { ++result.mnmNativeRenderer; },
        [&] { ++result.independentRenderer; });
    return result;
}

void requireOnly(const Invocations& actual, int oldCalls, int mnmCalls,
                 int independentCalls, const char* message)
{
    require(actual.oldRenderer == oldCalls
            && actual.mnmNativeRenderer == mnmCalls
            && actual.independentRenderer == independentCalls, message);
}

void runIndependentCore(int lowerMode, int upperMode, const char* message)
{
    // This is the distinct state holder used by TrackChain for a selected
    // physical family. It exercises the actual K35/R dispatch rather than a
    // mock adapter. Each sample gets precisely one call to this renderer.
    monomachine::mnm::IndependentPhysicalFilterCore core;
    core.setSampleRate(48000.0);
    core.setParameters(56.0f, 24.0f, 40.0f, 49.0f, 12.0f, 85.0f, 0.0f, 0.0f);
    core.setHybridTestModes(lowerMode, upperMode);
    core.trigger();
    float energy = 0.0f;
    for (int i = 0; i < 1024; ++i) {
        const float input = 0.72f * std::sin(static_cast<float>(i) * 0.071f)
                          + 0.12f * std::sin(static_cast<float>(i) * 0.223f);
        const float left = core.process(0, input);
        const float right = core.process(1, input * 0.83f);
        require(std::isfinite(left) && std::isfinite(right), message);
        energy += std::abs(left) + std::abs(right);
    }
    require(energy > 0.01f, message);
}


void verifySelectedPhysicalOutputIsNotGenericClipped()
{
    // The old 1.9.11 carrier sent every selected physical family through
    // RealFilterCore's native softClip() after the selected transfer. The
    // independent path must instead return the physical output itself: filter
    // resonance may be loud, but it must not silently become FILT saturation.
    monomachine::mnm::FilterCore nativeCarrier;
    monomachine::mnm::IndependentPhysicalFilterCore independent;
    nativeCarrier.setSampleRate(48000.0);
    nativeCarrier.setParameters(16.0f, 0.0f, 110.0f, 110.0f, 0.0f, 127.0f, 0.0f, 0.0f);
    nativeCarrier.setHybridTestModes(nova::hybrid_private::hybridK35Lp,
                                     nova::hybrid_private::hybridNative);
    nativeCarrier.trigger();
    independent.setSampleRate(48000.0);
    independent.setParameters(16.0f, 0.0f, 110.0f, 110.0f, 0.0f, 127.0f, 0.0f, 0.0f);
    independent.setHybridTestModes(nova::hybrid_private::hybridK35Lp,
                                   nova::hybrid_private::hybridNative);
    independent.trigger();

    bool exceededNativeClipKnee = false;
    bool sawTransferDifference = false;
    for (int i = 0; i < 48000; ++i) {
        const float input = 0.70f * std::sin(2.0f * 3.14159265358979323846f
                                              * 35.0f * static_cast<float>(i) / 48000.0f);
        const float nativeLeft = nativeCarrier.process(0, input);
        const float physicalLeft = independent.process(0, input);
        (void) nativeCarrier.process(1, input);
        (void) independent.process(1, input);
        require(std::isfinite(nativeLeft) && std::isfinite(physicalLeft),
                "selected physical output must remain finite");
        const float nativeLaw = monomachine::mnm::real_detail::softClip(
            physicalLeft, monomachine::mnm::real_detail::kOutCeiling);
        require(std::abs(nativeLeft - nativeLaw) < 1.0e-5f,
                "native MNM carrier must retain only its own documented safety law");
        if (std::abs(physicalLeft) > monomachine::mnm::real_detail::kOutCeiling * 0.25f) {
            exceededNativeClipKnee = true;
            sawTransferDifference |= std::abs(nativeLeft - physicalLeft) > 1.0e-4f;
        }
    }
    require(exceededNativeClipKnee && sawTransferDifference,
            "selected physical resonance must bypass the generic native soft clip");
}

void verifyRepeatedSelectedSnapshots()
{
    // setModes()/setHybridTestModes() arrives once per host callback. An
    // unchanged selected K35/R snapshot and an unchanged sample rate must not
    // reset the independent renderer or make an audible block cadence.
    monomachine::mnm::IndependentPhysicalFilterCore steady;
    monomachine::mnm::IndependentPhysicalFilterCore refreshed;
    for (auto* core : {&steady, &refreshed}) {
        core->setSampleRate(48000.0);
        core->setParameters(51.0f, 31.0f, 35.0f, 52.0f, 9.0f, 90.0f, 0.0f, 0.0f);
        core->setHybridTestModes(nova::hybrid_private::hybridK35Hp,
                                 nova::hybrid_private::hybridRDvalLp4);
        core->trigger();
    }
    for (int i = 0; i < 2048; ++i) {
        refreshed.setSampleRate(48000.0);
        refreshed.setHybridTestModes(nova::hybrid_private::hybridK35Hp,
                                     nova::hybrid_private::hybridRDvalLp4);
        const float input = 0.67f * std::sin(static_cast<float>(i) * 0.057f);
        const float steadyLeft = steady.process(0, input);
        const float steadyRight = steady.process(1, input * 0.91f);
        const float refreshedLeft = refreshed.process(0, input);
        const float refreshedRight = refreshed.process(1, input * 0.91f);
        require(steadyLeft == refreshedLeft && steadyRight == refreshedRight,
                "unchanged selected-family snapshots must not reset or drop out");
    }
}
}

int main()
{
    using namespace nova::hybrid_private;

    // The only two FILT DSP-native cases. Their selected renderer is exactly
    // one call and no independent-family renderer is involved.
    requireOnly(dispatch(false, hybridNative, hybridNative), 1, 0, 0,
                "OLD + NATIVE/NATIVE must invoke OLD exactly once");
    requireOnly(dispatch(true, hybridNative, hybridNative), 0, 1, 0,
                "MNM + NATIVE/NATIVE must invoke MNM exactly once");

    // K35 and all R generations must be independent of the OLD/MNM selector.
    // The helper is the one called by TrackFILT.inl, so this fails if a future
    // edit can invoke a selected family in series with either native renderer.
    constexpr int selectedModes[]{
        hybridK35Hp, hybridR303Lp, hybridRHuovilainenLp4, hybridRDvalLp4
    };
    for (const int selected : selectedModes) {
        for (const bool mnmSelected : {false, true}) {
            const auto lower = dispatch(mnmSelected, selected, hybridNative);
            requireOnly(lower, 0, 0, 1,
                        "selected MODE L must invoke only the independent renderer once");
            const auto upper = dispatch(mnmSelected, hybridNative, selected);
            requireOnly(upper, 0, 0, 1,
                        "selected MODE H must invoke only the independent renderer once");
        }
    }

    // 1.9.8/1.9.9 established two physical sides. A special selection replaces
    // its own native side; a native companion exists only when the other MODE
    // control is explicitly NATIVE. It is not an OLD/MNM outer renderer.
    const auto lowerPlan = physicalSideRendererFor(hybridK35Hp);
    const auto upperNativePlan = physicalSideRendererFor(hybridNative);
    const auto lowerNativePlan = physicalSideRendererFor(hybridNative);
    const auto upperPlan = physicalSideRendererFor(hybridRDvalLp4);
    require(lowerPlan == PhysicalSideRenderer::SelectedFamily
            && upperNativePlan == PhysicalSideRenderer::Native,
            "K35 MODE L must replace only the selected lower physical side");
    require(lowerNativePlan == PhysicalSideRenderer::Native
            && upperPlan == PhysicalSideRenderer::SelectedFamily,
            "R MODE H must replace only the selected upper physical side");

    // Actual independent-core smoke checks for an old K35 family, the 1.9.9 R
    // family, and the R Import 2 HUV/DVAL families. The route dispatch above
    // proves these cannot be preceded or followed by OLD/MNM native rendering.
    runIndependentCore(hybridK35Hp, hybridNative, "K35 independent core must remain finite/audible");
    runIndependentCore(hybridNative, hybridR303Lp, "R 303 independent core must remain finite/audible");
    runIndependentCore(hybridRHuovilainenLp4, hybridNative, "R HUV independent core must remain finite/audible");
    runIndependentCore(hybridNative, hybridRDvalLp4, "R DVAL independent core must remain finite/audible");
    verifyRepeatedSelectedSnapshots();
    verifySelectedPhysicalOutputIsNotGenericClipped();

    std::puts("FILTERROUTE PASS: selected K35/R is exclusive, snapshot-continuous, and not generically MNM-clipped");
    return 0;
}
