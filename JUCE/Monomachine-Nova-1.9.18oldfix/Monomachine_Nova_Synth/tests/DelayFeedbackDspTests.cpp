// JUCE-free regression coverage for the 1.9.8 delay feedback repair.
// It deliberately tests the documented practical DBAS/DWID mapping rather
// than claiming an unresolved firmware host mapping is bit exact.
#include "dsp/mnm/MnmDelay.hpp"
#include "dsp/mnm/MnmTrackDelayNew.hpp"
#include "dsp/monomachine_filter.hpp"
#include "models/DspModes.hpp"

#include <cmath>
#include <cstdio>
#include <vector>

namespace {
int failures = 0;

void require(bool value, const char* text)
{
    std::printf("%-72s %s\n", text, value ? "OK" : "FAIL");
    if (!value) ++failures;
}

float rmsAfterDelay(monomachine::mnm::DelayCore& delay, float base, float width)
{
    delay.setFeedbackFilterParameters(base, width, 0.0f, 0.0f);
    float left = 0.0f, right = 0.0f;
    double sum = 0.0;
    int count = 0;
    for (int sample = 0; sample < 9000; ++sample) {
        const float input = sample == 0 ? 1.0f : 0.0f;
        delay.process(0.0f, 0.86f, 1.0f, false, 0, 0.03f, input, input, left, right);
        if (sample > 900) { sum += static_cast<double>(left) * left + static_cast<double>(right) * right; count += 2; }
    }
    return static_cast<float>(std::sqrt(sum / std::max(1, count)));
}

float rmsLegacyFilter(float base, float width, float frequency)
{
    monomachine::MonomachineFilter filter;
    filter.reset(44100.0);
    filter.setParameters(base, width, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    double sum = 0.0;
    constexpr int frames = 12000;
    for (int i = 0; i < frames; ++i) {
        const float input = std::sin(2.0 * 3.14159265358979323846 * frequency * i / 44100.0);
        float outL = 0.0f, outR = 0.0f;
        filter.processStereo(&input, &input, &outL, &outR, 1);
        if (i > 2000) sum += static_cast<double>(outL) * outL;
    }
    return static_cast<float>(std::sqrt(sum / static_cast<double>(frames - 2001)));
}

bool finite(float x) { return std::isfinite(x); }
} // namespace

int main()
{
    using namespace monomachine;
    using namespace monomachine::mnm;

    // Neutral feedback controls must not modify the native MNM line at all.
    {
        DelayFeedbackFilter neutral;
        neutral.prepare(44100.0);
        neutral.setParameters(0.0f, 127.0f, 0.0f, 0.0f);
        float l = 0.3125f, r = -0.625f;
        neutral.processStereo(l, r);
        require(!neutral.active() && l == 0.3125f && r == -0.625f,
                "neutral DBAS=0 DWID=127 Q=0 is an exact MNM feedback bypass");
    }

    // The two controls shape opposite edges of the feedback band.
    {
        DelayFeedbackFilter highPass;
        highPass.prepare(44100.0);
        highPass.setParameters(92.0f, 127.0f, 0.0f, 0.0f);
        DelayFeedbackFilter lowPass;
        lowPass.prepare(44100.0);
        lowPass.setParameters(0.0f, 34.0f, 0.0f, 0.0f);
        double lowIn = 0.0, lowOut = 0.0, highIn = 0.0, highOut = 0.0;
        for (int i = 0; i < 12000; ++i) {
            float low = std::sin(2.0f * 3.14159265358979323846f * 160.0f * i / 44100.0f);
            float hi = std::sin(2.0f * 3.14159265358979323846f * 8000.0f * i / 44100.0f);
            const float lowReference = low, hiReference = hi;
            highPass.processStereo(low, low);
            lowPass.processStereo(hi, hi);
            if (i > 2000) {
                lowIn += lowReference * lowReference; lowOut += low * low;
                highIn += hiReference * hiReference; highOut += hi * hi;
            }
        }
        require(lowOut < lowIn * 0.18,
                "DBAS raises the Korg feedback high-pass edge and removes low repeat energy");
        require(highOut < highIn * 0.18,
                "DWID narrows the Korg feedback low-pass edge and removes high repeat energy");
    }

    // MNM's core now routes DBAS/DWID into its recurrence, not a dead UI word.
    {
        DelayCore open, shaped;
        open.prepare(44100.0);
        shaped.prepare(44100.0);
        const float openEnergy = rmsAfterDelay(open, 0.0f, 127.0f);
        const float shapedEnergy = rmsAfterDelay(shaped, 104.0f, 20.0f);
        require(openEnergy > 1.0e-4f && shapedEnergy < openEnergy * 0.55f,
                "MNM DBAS/DWID audibly change delayed feedback energy");
    }

    // OLD keeps its historic filter topology, now with both optional Q inputs
    // actually supplied by TrackChain.  Its legacy DBAS/DWID law remains audible.
    {
        const float openLow = rmsLegacyFilter(0.0f, 127.0f, 150.0f);
        const float baseCutLow = rmsLegacyFilter(92.0f, 127.0f, 150.0f);
        const float openHigh = rmsLegacyFilter(0.0f, 127.0f, 8000.0f);
        const float widthCutHigh = rmsLegacyFilter(0.0f, 34.0f, 8000.0f);
        require(baseCutLow < openLow * 0.25f,
                "OLD DBAS lowers delayed low-frequency energy through its feedback HP edge");
        require(widthCutHigh < openHigh * 0.25f,
                "OLD DWID lowers delayed high-frequency energy through its feedback LP edge");
    }

    // A redundant control update must not erase resonator/history state.
    {
        DelayFeedbackFilter continuous, refreshed;
        continuous.prepare(44100.0); refreshed.prepare(44100.0);
        continuous.setParameters(54.0f, 61.0f, 86.0f, 91.0f);
        refreshed.setParameters(54.0f, 61.0f, 86.0f, 91.0f);
        float maxDifference = 0.0f;
        for (int i = 0; i < 4000; ++i) {
            float a = std::sin(0.013f * i) + 0.13f * std::sin(0.081f * i);
            float b = a;
            continuous.processStereo(a, a);
            refreshed.processStereo(b, b);
            if (i == 1700) refreshed.setParameters(54.0f, 61.0f, 86.0f, 91.0f);
            if (i > 1700) maxDifference = std::max(maxDifference, std::abs(a - b));
        }
        require(maxDifference < 1.0e-6f,
                "feedback filter retains state across redundant control snapshots");
    }

    // High Q and maximum feedback must remain finite in the practical NEW core.
    {
        TrackDelayNewCore delay;
        delay.prepare(44100.0);
        delay.setFeedbackFilterParameters(60.0f, 54.0f, 127.0f, 127.0f);
        float left = 0.0f, right = 0.0f, peak = 0.0f;
        bool stable = true;
        for (int i = 0; i < 180000; ++i) {
            const float input = i == 0 ? 1.0f : 0.0f;
            delay.process(0.0f, 0.95f, 1.0f, (i & 1) != 0, 1, 0.03f, input, -input, left, right);
            peak = std::max(peak, std::max(std::abs(left), std::abs(right)));
            stable = stable && finite(left) && finite(right);
        }
        require(stable && peak < 6.0f,
                "NEW Korg feedback loop remains finite at maximum Q/feedback");
    }

    // NEW owns a separate state machine and is intentionally not just a renamed
    // MNM line.  Running it cannot change an independent MNM core's output.
    {
        DelayCore reference, isolated;
        TrackDelayNewCore candidate;
        reference.prepare(44100.0); isolated.prepare(44100.0); candidate.prepare(44100.0);
        reference.setFeedbackFilterParameters(0.0f, 127.0f, 0.0f, 0.0f);
        isolated.setFeedbackFilterParameters(0.0f, 127.0f, 0.0f, 0.0f);
        candidate.setFeedbackFilterParameters(48.0f, 58.0f, 30.0f, 44.0f);
        float refL = 0.0f, refR = 0.0f, isoL = 0.0f, isoR = 0.0f, newL = 0.0f, newR = 0.0f;
        float isolationDifference = 0.0f, newDifference = 0.0f;
        for (int i = 0; i < 30000; ++i) {
            const float input = i == 0 ? 1.0f : 0.0f;
            reference.process(70.0f, 0.72f, 1.0f, false, 0, 0.03f, input, input, refL, refR);
            candidate.process(70.0f, 0.72f, 1.0f, false, 0, 0.03f, input, input, newL, newR);
            isolated.process(70.0f, 0.72f, 1.0f, false, 0, 0.03f, input, input, isoL, isoR);
            isolationDifference = std::max(isolationDifference, std::abs(refL - isoL));
            newDifference = std::max(newDifference, std::abs(refL - newL));
        }
        require(isolationDifference == 0.0f,
                "NEW processing is isolated from the existing MNM delay state");
        require(newDifference > 1.0e-4f,
                "NEW has an independent frame-scheduled/filter-feedback response");
    }

    require(dspModeNew == 2 && dspModeMnmFix == 3 && dspModeNewFix == 4 && dspModeOldFix == 5
            && dspSectionModeCount(DspDelay) == 3
            && dspModeAllowedForSection(DspDelay, dspModeNew)
            && !dspModeAllowedForSection(DspFilter, dspModeNew)
            && !dspModeAllowedForSection(DspDelay, dspModeMnmFix)
            && dspSyntModeAllowedForMachine(8, dspModeNew)
            && dspSyntModeAllowedForMachine(8, dspModeMnmFix)
            && dspSyntModeAllowedForMachine(8, dspModeNewFix)
            && dspSyntModeAllowedForMachine(8, dspModeOldFix)
            && !dspSyntModeAllowedForMachine(7, dspModeNew)
            && !dspSyntModeAllowedForMachine(7, dspModeMnmFix)
            && !dspSyntModeAllowedForMachine(7, dspModeOldFix)
            && dspModeLegacyIndexToCurrent(2) == dspModeMnm
            && kDspModeSchemaVersion == 31,
            "DLY NEW keeps ID 2; schema-31 OLD FIX=5 remains confined to m8/m9/m10 SYNT");

    if (failures != 0) {
        std::fprintf(stderr, "DELAY FEEDBACK DSP FAIL: %d failures\n", failures);
        return 1;
    }
    std::puts("DELAY FEEDBACK DSP PASS");
    return 0;
}
