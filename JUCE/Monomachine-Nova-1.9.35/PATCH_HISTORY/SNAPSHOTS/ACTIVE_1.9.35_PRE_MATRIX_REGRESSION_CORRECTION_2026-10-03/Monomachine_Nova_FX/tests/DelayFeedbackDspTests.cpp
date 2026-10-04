// JUCE-free regression coverage for the 1.9.8 delay feedback repair.
// It deliberately tests the documented practical DBAS/DWID mapping rather
// than claiming an unresolved firmware host mapping is bit exact.
#include "dsp/mnm/MnmDelay.hpp"
#include "dsp/DelayFeedbackDynamics.hpp"
#include "dsp/TrackDelayRouting.hpp"
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
        delay.process(0.0f, 0.86f, 1.0f, 0, 0.03f, input, input, left, right);
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

struct DelayResponse {
    std::vector<float> left, right;
};

// DSND is centred bipolar: positive = one mono positive comb; negative =
// split L/R mono paths with negative feedback.
template <typename Delay>
DelayResponse dsndResponse(float inputL, float inputR, float send, bool negativeComb)
{
    Delay delay;
    delay.prepare(128.0); // DTIM=0 -> короткая линия, удобная для регрессии
    DelayResponse response;
    response.left.resize(40); response.right.resize(40);
    for (int i = 0; i < 40; ++i) {
        delay.process(0.0f, 0.0f, send, negativeComb, 0.0f,
                      i == 0 ? inputL : 0.0f, i == 0 ? inputR : 0.0f,
                      response.left[static_cast<size_t>(i)],
                      response.right[static_cast<size_t>(i)]);
    }
    return response;
}

float responseDifference(const DelayResponse& a, const DelayResponse& b)
{
    float difference = 0.0f;
    for (size_t i = 0; i < a.left.size(); ++i) {
        difference = std::max(difference, std::abs(a.left[i] - b.left[i]));
        difference = std::max(difference, std::abs(a.right[i] - b.right[i]));
    }
    return difference;
}

float responsePeak(const DelayResponse& response)
{
    float peak = 0.0f;
    for (size_t i = 0; i < response.left.size(); ++i)
        peak = std::max(peak, std::max(std::abs(response.left[i]), std::abs(response.right[i])));
    return peak;
}
} // namespace

int main()
{
    using namespace monomachine;
    using namespace monomachine::mnm;

    // Schema 44 keeps RAW=63 as the DYNAMICS reference. BASE CURVE bends
    // RAW 0..63 upward toward it; HOLD @64 is a small independent trim and
    // RAW=65 resumes the retained RAW/64 growth. GUARD still bypasses <=63.
    {
        nova::DelayFeedbackDynamics dynamics;
        dynamics.prepare(1000.0);
        dynamics.setControls(true, 1.0f, 0.50f);
        dynamics.setLoopGuard(true);
        nova::DelayFeedbackDynamics::Base base;
        base.lowCurve = 0.55f;base.holdAt64 = 0.992f;
        dynamics.setBase(base);
        nova::DelayFeedbackDynamics::Guard guard;
        guard.levelStart = 0.55f;guard.levelStartAt127 = 0.40f;guard.levelOffset = 0.0f;
        guard.levelCurve = 1.0f;guard.amount = 1.0f;
        nova::DelayFeedbackDynamics::Governor governor;
        governor.plateauLevel = 0.73f;governor.plateauLevelAt127 = 0.60f;
        governor.attackMilliseconds = 37.0f;governor.releaseMilliseconds = 184.0f;
        dynamics.setGuard(guard);dynamics.setGovernor(governor);
        const float raw40 = dynamics.process(40.0f);
        for (int i = 0; i < 1000; ++i) dynamics.observeLoop(1.0f, 1.0f);
        const float lowRawLoop = dynamics.loopLevel();
        const float raw60 = dynamics.process(60.0f);
        const float raw61 = dynamics.process(61.0f);
        const float raw62 = dynamics.process(62.0f);
        const float raw63 = dynamics.process(63.0f);
        const float raw64 = dynamics.process(64.0f);
        const float raw65 = dynamics.process(65.0f);
        const float gate40 = nova::DelayFeedbackDynamics::rawGateFor(40.0f);
        const float gate63 = nova::DelayFeedbackDynamics::rawGateFor(63.0f);
        const float gateSoft = nova::DelayFeedbackDynamics::rawGateFor(63.5f);
        const float gate64 = nova::DelayFeedbackDynamics::rawGateFor(64.0f);
        const auto window64 = nova::DelayFeedbackDynamics::guardWindowForRaw(64.0f, guard, governor);
        const auto window127 = nova::DelayFeedbackDynamics::guardWindowForRaw(127.0f, guard, governor);
        const float driveBelow = nova::DelayFeedbackDynamics::guardDriveForRawLevel(62.0f, 0.73f, guard, governor);
        const float driveSoft = nova::DelayFeedbackDynamics::guardDriveForRawLevel(63.5f, 0.73f, guard, governor);
        const float driveAt64 = nova::DelayFeedbackDynamics::guardDriveForRawLevel(64.0f, window64.plateau, guard, governor);
        const float driveAt127 = nova::DelayFeedbackDynamics::guardDriveForRawLevel(127.0f, window127.plateau, guard, governor);
        const float untouched40 = nova::DelayFeedbackDynamics::guardedCoefficient(40.0f, raw40, 1.0f, guard, governor, true);
        const float restrained64 = nova::DelayFeedbackDynamics::guardedCoefficient(64.0f, raw64, 0.73f, guard, governor, true);
        const float restrained65 = nova::DelayFeedbackDynamics::guardedCoefficient(65.0f, raw65, 0.73f, guard, governor, true);
        const nova::DelayFeedbackDynamics::Base legacyBase{1.0f,1.0f};
        const float legacy40Base = nova::DelayFeedbackDynamics::baseCoefficientForRaw(40.0f, legacyBase);
        const float legacy64Base = nova::DelayFeedbackDynamics::baseCoefficientForRaw(64.0f, legacyBase);
        const float legacy64HalfBase = nova::DelayFeedbackDynamics::baseCoefficientForRaw(64.5f, legacyBase);
        dynamics.setControls(false, 1.0f, 0.50f);
        const float legacy64 = dynamics.process(64.0f);
        const float expected60 = (63.0f / 64.0f) * std::pow(60.0f / 63.0f, 0.55f);
        require(std::abs(raw60 - expected60) < 1.0e-6f
                    && raw60 < raw61 && raw61 < raw62 && raw62 < raw63 && raw63 < raw64 && raw64 < raw65
                    && raw62 > 62.0f / 64.0f && std::abs(raw63 - 63.0f / 64.0f) < 1.0e-6f
                    && std::abs(raw64 - 0.992f) < 1.0e-6f && std::abs(raw65 - 65.0f / 64.0f) < 1.0e-6f
                    && std::abs(legacy40Base - 40.0f / 64.0f) < 1.0e-7f
                    && legacy64Base == 1.0f && std::abs(legacy64HalfBase - 64.5f / 64.0f) < 1.0e-7f
                    && gate40 == 0.0f && gate63 == 0.0f && std::abs(gateSoft - 0.5f) < 1.0e-6f && gate64 == 1.0f
                    && lowRawLoop == 0.0f && driveBelow == 0.0f && std::abs(driveSoft - 0.5f) < 1.0e-6f
                    && driveAt64 == 1.0f && driveAt127 == 1.0f
                    && std::abs(window64.start - 0.55f) < 1.0e-6f && std::abs(window127.plateau - 0.60f) < 1.0e-6f
                    && untouched40 == raw40 && restrained64 == raw64
                    && std::abs(restrained65 - 1.0f) < 1.0e-4f && legacy64 > 1.0f,
                "DFB BASE: 63 reference + curved 60..62 + HOLD @64 preserves low-RAW GUARD bypass");
    }

    // Four endpoint controls make the 64..127 tail segment explicit: START
    // and PLATEAU at each end interpolate together, while OFFSET moves the
    // whole protection window without changing the stored anchor values.
    {
        nova::DelayFeedbackDynamics::Guard guard;
        guard.levelStart = 0.40f;guard.levelStartAt127 = 0.70f;guard.levelOffset = 0.10f;
        guard.levelCurve = 2.0f;guard.amount = 1.50f;
        nova::DelayFeedbackDynamics::Governor governor;
        governor.plateauLevel = 0.80f;governor.plateauLevelAt127 = 1.20f;
        governor.attackMilliseconds = 5.0f;governor.releaseMilliseconds = 40.0f;
        const auto window64 = nova::DelayFeedbackDynamics::guardWindowForRaw(64.0f, guard, governor);
        const auto windowMid = nova::DelayFeedbackDynamics::guardWindowForRaw(95.5f, guard, governor);
        const auto window127 = nova::DelayFeedbackDynamics::guardWindowForRaw(127.0f, guard, governor);
        const float drive64Start = nova::DelayFeedbackDynamics::guardDriveForRawLevel(64.0f, 0.50f, guard, governor);
        const float driveMid = nova::DelayFeedbackDynamics::guardDriveForRawLevel(95.5f, 0.875f, guard, governor);
        const float drive127End = nova::DelayFeedbackDynamics::guardDriveForRawLevel(127.0f, window127.plateau, guard, governor);
        const float base80 = nova::DelayFeedbackDynamics::baseCoefficientForRaw(80.0f);
        const float restrainedMid = nova::DelayFeedbackDynamics::guardedCoefficient(95.5f, base80, 0.875f, guard, governor, true);
        require(std::abs(window64.start - 0.50f) < 1.0e-6f && std::abs(window64.plateau - 0.90f) < 1.0e-6f
                    && std::abs(windowMid.start - 0.65f) < 1.0e-6f && std::abs(windowMid.plateau - 1.10f) < 1.0e-6f
                    && std::abs(window127.start - 0.80f) < 1.0e-6f && std::abs(window127.plateau - 1.30f) < 1.0e-6f
                    && drive64Start == 0.0f && driveMid > 0.0f && driveMid < 1.0f && drive127End == 1.0f
                    && restrainedMid < base80 && restrainedMid > 1.0f,
                "DFB RAW WINDOW: @64/@127 anchors and OFFSET share graph/runtime law");
    }

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

    // DSND is centred bipolar: raw 64 is silent centre, negative range has
    // full magnitude at raw 0 and selects split-L/R negative comb, positive
    // range has full magnitude at raw 127 and selects mono positive comb.
    {
        std::array<float, 32> page{};
        page[nova::track_delay_routing::kDelaySend] = 0.0f;
        const float negative = nova::track_delay_routing::bipolarSendGain(page);
        const float negativeSign = nova::track_delay_routing::signedSend(page[nova::track_delay_routing::kDelaySend]);
        page[nova::track_delay_routing::kDelaySend] = 64.0f;
        const float centre = nova::track_delay_routing::bipolarSendGain(page);
        page[nova::track_delay_routing::kDelaySend] = 127.0f;
        const float positive = nova::track_delay_routing::bipolarSendGain(page);
        require(std::abs(negative - 1.0f) < 1.0e-7f && negativeSign == -64.0f
                    && centre == 0.0f && std::abs(positive - 1.0f) < 1.0e-7f,
                "DSND centred: -64=split negative comb, 0=off, +63=mono positive comb");
        require(nova::track_delay_routing::kFilterEnvelopeAttack == 20
                    && nova::track_delay_routing::kFilterEnvelopeDecay == 21
                    && nova::track_delay_routing::kFilterEnvelopeBaseOffset == 22
                    && nova::track_delay_routing::kFilterEnvelopeWidthOffset == 23
                    && nova::track_delay_routing::kDelayTime == 27
                    && nova::track_delay_routing::kDelaySend == 28
                    && nova::track_delay_routing::kDelayFeedback == 29
                    && nova::track_delay_routing::kDelayFilterBase == 30
                    && nova::track_delay_routing::kDelayFilterWidth == 31,
                "P1 map separates $40C..$411 filter envelope from host EFFX words");
        page[nova::track_delay_routing::kFilterEnvelopeAttack] = 64.0f;
        page[nova::track_delay_routing::kFilterEnvelopeDecay] = 127.0f;
        page[nova::track_delay_routing::kFilterEnvelopeBaseOffset] = 0.0f;
        page[nova::track_delay_routing::kFilterEnvelopeWidthOffset] = 127.0f;
        require(std::abs(nova::track_delay_routing::bipolarSendGain(page) - positive) < 1.0e-7f,
                "FILT ATK/DEC/BOFS/WOFS cannot alter DSND magnitude");
    }

    // Positive DSND folds input to one mono positive comb; negative DSND
    // preserves post-AMP/PAN L/R input paths and flips feedback recurrence.
    {
        const auto mnmMonoL = dsndResponse<DelayCore>(1.0f, 0.0f, 1.0f, false);
        const auto mnmMonoR = dsndResponse<DelayCore>(0.0f, 1.0f, 1.0f, false);
        const auto newMonoL = dsndResponse<TrackDelayNewCore>(1.0f, 0.0f, 1.0f, false);
        const auto newMonoR = dsndResponse<TrackDelayNewCore>(0.0f, 1.0f, 1.0f, false);
        const auto mnmSplitL = dsndResponse<DelayCore>(1.0f, 0.0f, 1.0f, true);
        const auto mnmSplitR = dsndResponse<DelayCore>(0.0f, 1.0f, 1.0f, true);
        const auto newSplitL = dsndResponse<TrackDelayNewCore>(1.0f, 0.0f, 1.0f, true);
        const auto newSplitR = dsndResponse<TrackDelayNewCore>(0.0f, 1.0f, 1.0f, true);
        const auto mnmMuted = dsndResponse<DelayCore>(1.0f, 0.0f, 0.0f, false);
        const auto newMuted = dsndResponse<TrackDelayNewCore>(1.0f, 0.0f, 0.0f, false);
        require(responsePeak(mnmMonoL) > 0.1f && responsePeak(newMonoL) > 0.1f
                    && responseDifference(mnmMonoL, mnmMonoR) < 1.0e-6f
                    && responseDifference(newMonoL, newMonoR) < 1.0e-6f,
                "positive DSND folds input into one mono positive comb");
        require(responseDifference(mnmSplitL, mnmSplitR) > 0.1f
                    && responseDifference(newSplitL, newSplitR) > 0.1f,
                "negative DSND preserves split L/R mono input paths");
        require(responsePeak(mnmMuted) == 0.0f && responsePeak(newMuted) == 0.0f,
                "DSND centre zero writes no new delay input");
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
            delay.process(0.0f, 0.95f, 1.0f, 1, 0.03f, input, -input, left, right);
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
            reference.process(70.0f, 0.72f, 1.0f, 0, 0.03f, input, input, refL, refR);
            candidate.process(70.0f, 0.72f, 1.0f, 0, 0.03f, input, input, newL, newR);
            isolated.process(70.0f, 0.72f, 1.0f, 0, 0.03f, input, input, isoL, isoR);
            isolationDifference = std::max(isolationDifference, std::abs(refL - isoL));
            newDifference = std::max(newDifference, std::abs(refL - newL));
        }
        require(isolationDifference == 0.0f,
                "NEW processing is isolated from the existing MNM delay state");
        require(newDifference > 1.0e-4f,
                "NEW has an independent frame-scheduled/filter-feedback response");
    }

    require(dspModeNew == 2 && dspModeMnmFix == 3 && dspModeNewFix == 4 && dspModeOldFix == 5
            && kRetiredSyntRawId6 == 6 && kRetiredSyntRawId7 == 7 && kRetiredSyntRawId8 == 8
            && dspModeCount == 6 && dspSectionModeCount(DspDelay) == 3
            && dspModeAllowedForSection(DspDelay, dspModeNew)
            && !dspModeAllowedForSection(DspFilter, dspModeNew)
            && !dspModeAllowedForSection(DspDelay, dspModeMnmFix)
            && !dspModeAllowedForSection(DspDelay, kRetiredSyntRawId8)
            && dspSyntModeAllowedForMachine(8, dspModeNew)
            && dspSyntModeAllowedForMachine(8, dspModeMnmFix)
            && dspSyntModeAllowedForMachine(8, dspModeNewFix)
            && dspSyntModeAllowedForMachine(8, dspModeOldFix)
            && !dspSyntModeAllowedForMachine(8, kRetiredSyntRawId6)
            && !dspSyntModeAllowedForMachine(8, kRetiredSyntRawId7)
            && !dspSyntModeAllowedForMachine(8, kRetiredSyntRawId8)
            && !dspSyntModeAllowedForMachine(7, dspModeNew)
            && !dspSyntModeAllowedForMachine(7, dspModeMnmFix)
            && !dspSyntModeAllowedForMachine(7, dspModeOldFix)
            && !dspSyntModeAllowedForMachine(7, kRetiredSyntRawId6)
            && !dspSyntModeAllowedForMachine(7, kRetiredSyntRawId7)
            && !dspSyntModeAllowedForMachine(7, kRetiredSyntRawId8)
            && dspModeLegacyIndexToCurrent(2) == dspModeMnm
            && kDspModeSchemaVersion == 44,
            "DLY NEW keeps ID 2; schema 44 adds BASE CURVE/HOLD while CORE remains removed");

    if (failures != 0) {
        std::fprintf(stderr, "DELAY FEEDBACK DSP FAIL: %d failures\n", failures);
        return 1;
    }
    std::puts("DELAY FEEDBACK DSP PASS");
    return 0;
}
