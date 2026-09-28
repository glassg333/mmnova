// JUCE-free regression coverage for the 1.9.10/1.9.13 R Import 2 independent filters.
#include "dsp/hybrid_private/HybridDSP.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <utility>

namespace {
void require(bool condition, const char* message)
{
    if (!condition) {
        std::cerr << "Import2FiltersTests: " << message << '\n';
        std::exit(1);
    }
}

void requireFinite(float value, const char* message)
{
    require(std::isfinite(value), message);
}
}

int main()
{
    using namespace nova::hybrid_private;

    // Small-signal response checks validate the user-facing physical labels,
    // rather than assuming an enum name is a correct HP/LP implementation.
    const auto measuredGain = [](int mode, double hz) {
        ReferenceImport2Stereo filter;
        filter.prepare(48000.0);
        filter.configure(mode, 1100.0f, 0.10f);
        constexpr int samples = 48000;
        double square = 0.0;
        for (int index = 0; index < samples; ++index) {
            const float input = 0.05f * std::sin(2.0f * static_cast<float>(kPi)
                * static_cast<float>(hz) * static_cast<float>(index) / 48000.0f);
            const float output = filter.process(0, input);
            requireFinite(output, "response sweep produced NaN/Inf");
            if (index >= samples * 3 / 4) square += static_cast<double>(output) * output;
        }
        const double reference = 0.05 / std::sqrt(2.0);
        return 20.0 * std::log10(std::max(1.0e-12, std::sqrt(square / (samples / 4)) / reference));
    };
    struct ResponseFamily { int lp, bp, hp; };
    const std::array<ResponseFamily, 5> physicalFamilies{{
        { hybridRAnalogLp24,   hybridRAnalogBp24,   hybridRAnalogHp24 },
        { hybridRLinearLp24,   hybridRLinearBp24,   hybridRLinearHp24 },
        { hybridRRbjLp,        hybridRRbjBp,        hybridRRbjHp },
        { hybridRTptLp,        hybridRTptBp,        hybridRTptHp },
        { hybridRHyperionLp4,  hybridRHyperionBp4,  hybridRHyperionHp4 }
    }};
    for (const auto family : physicalFamilies) {
        const double lpLow = measuredGain(family.lp, 150.0);
        const double lpHigh = measuredGain(family.lp, 6000.0);
        const double hpLow = measuredGain(family.hp, 150.0);
        const double hpHigh = measuredGain(family.hp, 6000.0);
        const double bpLow = measuredGain(family.bp, 150.0);
        const double bpMid = measuredGain(family.bp, 1100.0);
        const double bpHigh = measuredGain(family.bp, 6000.0);
        require(lpLow > lpHigh + 8.0, "R Import 2 LP label is not a physical high-cut response");
        require(hpHigh > hpLow + 8.0, "R Import 2 HP label is not a physical low-cut response");
        require(bpMid > std::max(bpLow, bpHigh) + 1.0, "R Import 2 BP label does not peak near cutoff");
    }

    // The six LP-only source cores now expose explicitly labelled derived HP
    // complements. Five are dry minus the core LP output; DVAL is dry plus LP
    // because that core's LP output polarity is inverted. Verify the observable
    // response rather than accepting a label or an assumed subtraction on faith.
    const std::array<std::pair<int, int>, 6> complements{{
        { hybridRHuovilainenLp4, hybridRHuovilainenHp4 },
        { hybridRKrajeskiLp4, hybridRKrajeskiHp4 },
        { hybridRMicrotrackerLp4, hybridRMicrotrackerHp4 },
        { hybridRMusicLp4, hybridRMusicHp4 },
        { hybridROberheimLp4, hybridROberheimHp4 },
        { hybridRDvalLp4, hybridRDvalHp4 }
    }};
    for (const auto& [lp, hp] : complements) {
        const double lpLow = measuredGain(lp, 150.0);
        const double lpHigh = measuredGain(lp, 6000.0);
        const double hpLow = measuredGain(hp, 150.0);
        const double hpHigh = measuredGain(hp, 6000.0);
        require(lpLow > lpHigh + 4.0, "LP-only source core no longer behaves as high-cut");
        require(hpHigh > hpLow + 4.0, "derived HP complement is not a low-cut response");
    }

    // At fully open cutoff every single-output R ladder must keep an audible,
    // sane pass-band level. This directly catches the former HUV over-gain and
    // DVAL high-cutoff sign reversal that could sound clipped, quiet, or broken.
    const auto openPassGain = [](int mode) {
        ReferenceImport2Stereo filter;
        filter.prepare(48000.0);
        filter.configure(mode, 20000.0f, 0.0f);
        constexpr int samples = 96000;
        double square = 0.0;
        for (int index = 0; index < samples; ++index) {
            const float input = 0.05f * std::sin(2.0f * static_cast<float>(kPi)
                * 1000.0f * static_cast<float>(index) / 48000.0f);
            const float output = filter.process(0, input);
            requireFinite(output, "open pass-band level produced NaN/Inf");
            if (index >= samples / 2) square += static_cast<double>(output) * output;
        }
        const double reference = 0.05 / std::sqrt(2.0);
        return std::sqrt(square / (samples / 2)) / reference;
    };
    for (const int mode : { hybridRHuovilainenLp4, hybridRHyperionLp2, hybridRHyperionLp4,
                            hybridRKrajeskiLp4, hybridRMicrotrackerLp4, hybridRMusicLp4,
                            hybridROberheimLp4, hybridRDvalLp4 }) {
        const double gain = openPassGain(mode);
        require(gain > 0.20 && gain < 1.80,
                "R Import 2 open pass-band level is quiet, clipped, or over-gained");
    }

    // High-control, multi-rate stress: every appended model must remain finite
    // and inside the common independently-implemented safety bound.
    for (int mode = kHybridRImport2FirstAlgorithm; mode <= kHybridRImport2LastAlgorithm; ++mode) {
        for (const double rate : { 22050.0, 44100.0, 48000.0, 96000.0 }) {
            for (const float cutoff : { 20.0f, 250.0f, 1100.0f, 6000.0f, 20000.0f }) {
                for (const float resonance : { 0.0f, 0.5f, 1.0f }) {
                    ReferenceImport2Stereo filter;
                    filter.prepare(rate);
                    filter.configure(mode, cutoff, resonance);
                    for (int index = 0; index < 256; ++index) {
                        const float input = 1.4f * std::sin(static_cast<float>(index) * 0.117f)
                                          + 0.35f * std::sin(static_cast<float>(index) * 0.031f);
                        const float left = filter.process(0, input);
                        const float right = filter.process(1, 0.0f);
                        requireFinite(left, "R Import 2 stress output is non-finite");
                        requireFinite(right, "R Import 2 stress right output is non-finite");
                        require(std::abs(left) <= 4.001f && std::abs(right) <= 4.001f,
                                "R Import 2 common safety bound was exceeded");
                    }
                }
            }
        }
    }

    // Per-channel state must not leak: an impulse on left cannot appear on
    // right, including ladder models whose topology has several integrators.
    for (int mode = kHybridRImport2FirstAlgorithm; mode <= kHybridRImport2LastAlgorithm; ++mode) {
        ReferenceImport2Stereo filter;
        filter.prepare(48000.0);
        filter.configure(mode, 1200.0f, 0.4f);
        float leftEnergy = 0.0f;
        float rightPeak = 0.0f;
        for (int index = 0; index < 2048; ++index) {
            const float left = filter.process(0, index == 0 ? 1.0f : 0.0f);
            const float right = filter.process(1, 0.0f);
            requireFinite(left, "R Import 2 impulse left output is non-finite");
            requireFinite(right, "R Import 2 impulse right output is non-finite");
            leftEnergy += std::abs(left);
            rightPeak = std::max(rightPeak, std::abs(right));
        }
        require(leftEnergy > 0.01f, "every R Import 2 mode must render an impulse response");
        require(rightPeak < 1.0e-7f, "R Import 2 filter channels share state");
    }

    // Host snapshots resend unchanged values. Reconfigure with the identical
    // mode/controls must be bit-exact: only a genuine mode change can reset a
    // model, and a changed cutoff uses the adapter's sample-rate-aware 10 ms ramp.
    for (int mode = kHybridRImport2FirstAlgorithm; mode <= kHybridRImport2LastAlgorithm; ++mode) {
        ReferenceImport2Stereo reference, repeatedSnapshot;
        for (auto* filter : { &reference, &repeatedSnapshot }) {
            filter->prepare(48000.0);
            filter->configure(mode, 1350.0f, 0.65f);
        }
        for (int index = 0; index < 2048; ++index) {
            if (index != 0 && (index % 47) == 0)
                repeatedSnapshot.configure(mode, 1350.0f, 0.65f);
            const float input = 0.61f * std::sin(static_cast<float>(index) * 0.043f)
                              + 0.27f * std::sin(static_cast<float>(index) * 0.119f);
            const float expected = reference.process(0, input);
            const float actual = repeatedSnapshot.process(0, input);
            require(expected == actual, "unchanged R Import 2 host snapshot reset state");
        }
    }

    // Verify the control-ramp boundary directly. The later resonant response
    // is deliberately not treated as a click; the discontinuity at the change
    // sample is what the 16-sample interpolation owns and must contain.
    for (int mode = kHybridRImport2FirstAlgorithm; mode <= kHybridRImport2LastAlgorithm; ++mode) {
        ReferenceImport2Stereo filter;
        filter.prepare(48000.0);
        filter.configure(mode, 350.0f, 0.8f);
        float previous = 0.0f;
        for (int index = 0; index < 512; ++index)
            previous = filter.process(0, 0.5f * std::sin(static_cast<float>(index) * 0.071f));
        filter.configure(mode, 6500.0f, 0.8f);
        const float boundary = filter.process(0, 0.5f * std::sin(512.0f * 0.071f));
        requireFinite(boundary, "R Import 2 cutoff-ramp boundary is non-finite");
        require(std::abs(boundary - previous) < 0.35f,
                "R Import 2 10 ms ramp did not contain its control-boundary step");
        for (int index = 0; index < 32; ++index) {
            const float output = filter.process(0, 0.5f * std::sin(static_cast<float>(513 + index) * 0.071f));
            requireFinite(output, "R Import 2 cutoff-ramp tail is non-finite");
            require(std::abs(output) <= 4.001f, "R Import 2 cutoff-ramp tail escaped safety bound");
        }
    }

    std::cout << "R Import 2 physical response, finite/stereo, snapshot, and control-ramp checks passed\n";
    return 0;
}
