// Private hybrid 2 closed-test smoke checks.  This target has no JUCE dependency.
#include "dsp/hybrid_private/HybridDSP.hpp"
#include "dsp/mnm/MnmRealFilter.hpp"
#include "dsp/mnm/MnmKernel.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "HybridDspTests: " << message << '\n';
        std::exit(1);
    }
}
void requireFinite(float value, const char* message) {
    require(std::isfinite(value), message);
}
}

int main() {
    using namespace nova::hybrid_private;

    // The UI choice order follows the physical side, not FilterCore's internal
    // selector numbering. IDs 0..8 are the frozen 1.9.8 contract; the R family
    // is appended, never inserted. The maps are part of the saved-state contract.
    constexpr std::array<int, 21> expectedL{{
        0,2,7,8,5,6,1,3,4, 11,14,19,20,10,13,17,18,9,12,15,16}};
    constexpr std::array<int, 21> expectedH{{
        0,1,3,4,5,6,2,7,8, 9,12,15,16,10,13,17,18,11,14,19,20}};
    require(kHybridLegacyAlgorithmCount == 9 && kHybridFilterAlgorithmCount == 21,
            "R choices must append after the nine stable hybrid IDs");
    for (int choice = 0; choice < kHybridFilterAlgorithmCount; ++choice) {
        require(modeLChoiceToAlgorithm(choice) == expectedL[static_cast<size_t>(choice)],
                "MODE L must list true HP, then BP, then LP physical choices");
        require(modeHChoiceToAlgorithm(choice) == expectedH[static_cast<size_t>(choice)],
                "MODE H must list true LP, then BP, then HP physical choices");
    }

    // NATIVE is not an alternate implementation: invoking the three private
    // selectors at their defaults must retain the existing mnm output exactly.
    monomachine::mnm::FilterCore nativeA, nativeB;
    for (auto* filter : {&nativeA, &nativeB}) {
        filter->setSampleRate(48000.0);
        filter->setParameters(22.0f, 76.0f, 10.0f, 23.0f, 17.0f, 93.0f, -12.0f, 9.0f);
        filter->trigger();
    }
    nativeB.setHybridTestModes(0, 0);
    for (int i = 0; i < 1024; ++i) {
        const float input = std::sin(static_cast<float>(i) * 0.071f) + 0.31f * std::sin(static_cast<float>(i) * 0.231f);
        const int channel = i & 1;
        const float a = nativeA.process(channel, input);
        const float b = nativeB.process(channel, input);
        require(a == b, "MODE L/H NATIVE must be bit-identical to the retained mnm core");
    }

    // Exercise every independently selectable MODE L / MODE H combination
    // through the actual FilterCore bridge. This catches a bad BASE/WDTH map,
    // a missed R-family dispatch, unstable nonlinear state, or shared stereo
    // state at the integration boundary.
    for (int lowerMode = 0; lowerMode < kHybridFilterAlgorithmCount; ++lowerMode) {
        for (int upperMode = 0; upperMode < kHybridFilterAlgorithmCount; ++upperMode) {
            monomachine::mnm::FilterCore bridge;
            bridge.setSampleRate(48000.0);
            bridge.setParameters(26.0f, 74.0f, 42.0f, 51.0f, 8.0f, 96.0f, 11.0f, -7.0f);
            bridge.setHybridTestModes(lowerMode, upperMode);
            bridge.trigger();
            for (int i = 0; i < 2048; ++i) {
                const int channel = i & 1;
                const float in = 0.77f * std::sin(static_cast<float>(i) * 0.043f);
                const float out = bridge.process(channel, in);
                requireFinite(out, "every MODE L/H pair must stay finite");
                require(std::abs(out) <= monomachine::mnm::real_detail::kOutCeiling + 1.0e-3f,
                        "every MODE L/H pair must stay inside FilterCore's safety ceiling");
            }
        }
    }

    // Verify that the six ladder selectors survive the FilterCore dispatch,
    // rather than only exercising the adapter in isolation. A deterministic
    // energy fingerprint catches an accidental collapse of multiple selector
    // values onto the same response.
    std::array<float, 6> ladderBridgeEnergy{};
    for (int mode = 3; mode <= 8; ++mode) {
        monomachine::mnm::FilterCore bridge;
        bridge.setSampleRate(48000.0);
        bridge.setParameters(26.0f, 74.0f, 42.0f, 51.0f, 8.0f, 96.0f, 11.0f, -7.0f);
        bridge.setHybridTestModes(mode, 0);
        bridge.trigger();
        float energy = 0.0f;
        for (int i = 0; i < 4096; ++i) {
            const float in = 0.77f * std::sin(static_cast<float>(i) * 0.043f);
            const float out = bridge.process(i & 1, in);
            requireFinite(out, "ladder mode through FilterCore must stay finite");
            energy += std::abs(out);
        }
        ladderBridgeEnergy[static_cast<size_t>(mode - 3)] = energy;
    }
    for (size_t a = 0; a < ladderBridgeEnergy.size(); ++a) {
        for (size_t b = 0; b < a; ++b) {
            require(std::abs(ladderBridgeEnergy[a] - ladderBridgeEnergy[b]) > 0.01f,
                    "each ladder selector must reach its own FilterCore response");
        }
    }

    // The supplied 303/MS20/Moog implementations expose real LP/BP/HP
    // responses. Verify the response labels rather than merely relying on an
    // enum name: LP passes 150 Hz over 6 kHz, HP does the inverse, and BP is
    // strongest around its named cutoff. This guards the physical MODE L/H
    // lists against accidentally wiring an LP algorithm to an HP label.
    using reference_filters::ReferenceCore;
    using reference_filters::ReferenceMode;
    const auto referenceGain=[](ReferenceMode mode,double hz){
        ReferenceCore filter;filter.prepare(48000.0);filter.configure(mode,1100.0,0.15);
        constexpr int samples=48000;double square=0.0;
        for(int i=0;i<samples;++i){
            const float input=0.1f*std::sin(2.0f*static_cast<float>(kPi)*static_cast<float>(hz)*static_cast<float>(i)/48000.0f);
            const float output=filter.process(input);
            if(i>=samples*3/4)square+=static_cast<double>(output)*output;
        }
        return 20.0*std::log10(std::max(1.0e-12,std::sqrt(square/(samples/4))/(0.1/std::sqrt(2.0))));
    };
    struct ResponseFamily { ReferenceMode lp,bp,hp; };
    const std::array<ResponseFamily,4> referenceFamilies{{
        {ReferenceMode::TB303LP,ReferenceMode::TB303BP,ReferenceMode::TB303HP},
        {ReferenceMode::MS20LP,ReferenceMode::MS20BP,ReferenceMode::MS20HP},
        {ReferenceMode::MoogLP24,ReferenceMode::MoogBP24,ReferenceMode::MoogHP24},
        {ReferenceMode::MoogLP12,ReferenceMode::MoogBP12,ReferenceMode::MoogHP12}}};
    for(const auto family:referenceFamilies){
        const double lpLow=referenceGain(family.lp,150.0),lpHigh=referenceGain(family.lp,6000.0);
        const double hpLow=referenceGain(family.hp,150.0),hpHigh=referenceGain(family.hp,6000.0);
        const double bpLow=referenceGain(family.bp,150.0),bpMid=referenceGain(family.bp,1100.0),bpHigh=referenceGain(family.bp,6000.0);
        require(lpLow>lpHigh+6.0,"R LP label must pass low frequencies over high frequencies");
        require(hpHigh>hpLow+4.0,"R HP label must pass high frequencies over low frequencies");
        require(bpMid>std::min(bpLow,bpHigh)+2.0,"R BP label must peak near its named cutoff");
    }

    // Exhaustive high-control, multi-rate stress of the self-contained
    // reference bridge. The missing upstream Filter.h helpers are deliberately
    // replaced by finite analytic guards, so no sample/control endpoint may
    // leak a NaN/Inf or an unbounded value into the surrounding FilterCore.
    for(int mode=hybridR303Lp;mode<=hybridRMoogHp12;++mode){
        for(const double rate:{22050.0,44100.0,48000.0,96000.0}){
            for(const float cutoff:{20.0f,250.0f,1100.0f,6000.0f,20000.0f}){
                for(const float q:{0.0f,0.5f,1.0f}){
                    ReferenceClassicStereo reference;reference.prepare(rate);reference.configure(mode,cutoff,q);
                    for(int i=0;i<256;++i){
                        const float input=1.4f*std::sin(static_cast<float>(i)*0.117f)+0.35f*std::sin(static_cast<float>(i)*0.031f);
                        const float left=reference.process(0,input),right=reference.process(1,0.0f);
                        requireFinite(left,"R reference filter stress output must stay finite");
                        requireFinite(right,"R reference filter right channel must stay finite");
                        require(std::abs(left)<13.1f&&std::abs(right)<13.1f,"R reference filter finite guard must bound output");
                    }
                }
            }
        }
    }

    // Stereo state is per channel for each reference family, not a shared
    // mono integrator hidden behind a stereo wrapper.
    for(int mode=hybridR303Lp;mode<=hybridRMoogHp12;++mode){
        ReferenceClassicStereo reference;reference.prepare(48000.0);reference.configure(mode,1200.0f,0.4f);
        float leftEnergy=0.0f,rightPeak=0.0f;
        for(int i=0;i<1024;++i){
            const float left=reference.process(0,i==0?1.0f:0.0f);
            const float right=reference.process(1,0.0f);
            leftEnergy+=std::abs(left);rightPeak=std::max(rightPeak,std::abs(right));
        }
        require(leftEnergy>0.0001f,"every R reference response must render an impulse response");
        require(rightPeak<1.0e-7f,"R reference filter channels must not share state");
    }

    Korg35Stereo lower;
    lower.prepare(48000.0);
    lower.configure(2, 350.0f, 0.35f); // MODE L sensible default: K35 HP.
    float leftEnergy = 0.0f;
    for (int i = 0; i < 1024; ++i) {
        const float y = lower.process(0, i == 0 ? 1.0f : 0.0f);
        requireFinite(y, "Korg-35 HP impulse must stay finite");
        leftEnergy += std::abs(y);
        const float right = lower.process(1, 0.0f);
        requireFinite(right, "Korg-35 right channel must stay finite");
        require(std::abs(right) < 1.0e-7f, "Korg-35 channels must not share state");
    }
    require(leftEnergy > 0.01f, "Korg-35 HP must produce an impulse response");

    Korg35Stereo upper;
    upper.prepare(48000.0);
    upper.configure(1, 1800.0f, 0.5f); // MODE H sensible default: K35 LP.
    float response = 0.0f;
    for (int i = 0; i < 1024; ++i) {
        const float input = std::sin(static_cast<float>(i) * 0.17f);
        response += std::abs(upper.process(0, input));
        requireFinite(response, "Korg-35 LP output must stay finite");
    }
    require(response > 0.1f, "Korg-35 LP must pass a signal");

    // Every imported ladder response must render independently and retain
    // separate left/right state. Modes 3..8 are LP24, LP12, BP24, BP12,
    // HP24 and HP12 in that order.
    for (int mode = 3; mode <= 8; ++mode) {
        LadderStereo ladder;
        ladder.prepare(48000.0);
        ladder.configure(mode, 1200.0f, 0.35f);
        float impulseEnergy = 0.0f;
        for (int i = 0; i < 1024; ++i) {
            const float left = ladder.process(0, i == 0 ? 1.0f : 0.0f);
            const float right = ladder.process(1, 0.0f);
            requireFinite(left, "ladder impulse must stay finite");
            requireFinite(right, "ladder right channel must stay finite");
            require(std::abs(right) < 1.0e-7f, "ladder channels must not share state");
            impulseEnergy += std::abs(left);
        }
        require(impulseEnergy > 0.001f, "each ladder response must produce an impulse response");
    }

    // Imported MODE S keeps the native MNM lower half exactly. Above DIST=64
    // it retains only MNM's input pre-drive; the selected Odin Fold/Zero/Clamp
    // transfer then runs directly. A common post-level or blend back into MNM
    // would alter each imported transfer differently and is intentionally absent.
    for (int mode = 1; mode <= 3; ++mode) {
        OversamplingDistortionStereo controlled;
        controlled.setMode(mode);
        for (const float raw : {0.0f, 32.0f, 64.0f}) {
            for (int i = 0; i < 128; ++i) {
                const float input = 0.93f * std::sin(static_cast<float>(i) * 0.137f);
                const float native = monomachine::mnm::Saturator::process(input, raw);
                const float output = controlled.processMnmPreDriven(0, input, raw, native);
                require(std::abs(output - native) < 1.0e-7f,
                        "imported MODE S must retain MNM below and at DIST=64");
            }
        }

        // Above neutral, compare bit-for-bit with the direct imported call
        // driven only by the retained MNM pre-drive convention.
        OversamplingDistortionStereo preDriven;
        OversamplingDistortionStereo direct;
        preDriven.setMode(mode);
        direct.setMode(mode);
        constexpr float rawDriven = 108.0f;
        const float amountDriven = (rawDriven - 64.0f) / 64.0f;
        float drivenDifference = 0.0f;
        for (int i = 0; i < 512; ++i) {
            const float input = 0.93f * std::sin(static_cast<float>(i) * 0.137f);
            const float native = monomachine::mnm::Saturator::process(input, rawDriven);
            const float output = preDriven.processMnmPreDriven(0, input, rawDriven, native);
            const float expected = direct.process(0, input * (1.0f + amountDriven * 15.0f), amountDriven * 127.0f);
            requireFinite(output, "pre-driven imported DIST must stay finite");
            require(std::abs(output - expected) < 1.0e-7f,
                    "imported MODE S has a synthetic post-level or MNM blend");
            drivenDifference += std::abs(output - native);
        }
        require(drivenDifference > 0.01f, "direct imported MODE S must colour the MNM reference");

        // Do not impose a shared headroom limiter: each original imported
        // transfer owns its own output behaviour. Only finite rendering and
        // exact direct dispatch are required at the maximum control value.
        OversamplingDistortionStereo maxPreDriven;
        OversamplingDistortionStereo maxDirect;
        maxPreDriven.setMode(mode);
        maxDirect.setMode(mode);
        float peakAtMaxDist = 0.0f;
        constexpr float rawMaximum = 127.0f;
        const float amountMaximum = (rawMaximum - 64.0f) / 64.0f;
        for (int i = 0; i < 2048; ++i) {
            const float input = 0.8f * std::sin(static_cast<float>(i) * 0.21f);
            const float native = monomachine::mnm::Saturator::process(input, rawMaximum);
            const float output = maxPreDriven.processMnmPreDriven(0, input, rawMaximum, native);
            const float expected = maxDirect.process(0, input * (1.0f + amountMaximum * 15.0f), amountMaximum * 127.0f);
            requireFinite(output, "maximum pre-driven imported DIST must stay finite");
            require(std::abs(output - expected) < 1.0e-7f,
                    "maximum imported MODE S is not its direct transfer");
            peakAtMaxDist = std::max(peakAtMaxDist, std::abs(output));
        }
        require(peakAtMaxDist > 0.01f, "maximum imported MODE S did not render");
    }

    // MNM FIX is an explicit A/B notch-compensation candidate, not a claim
    // about the unresolved firmware handler. It stays exact at DIST=64 and
    // produces a bounded, positive 3.5 kHz correction above the neutral point.
    MnmFixNotchCompensatorStereo mnmFix;
    mnmFix.prepare(48000.0);
    float plainEnergy = 0.0f, fixedEnergy = 0.0f;
    for (int i = 0; i < 12000; ++i) {
        const float plain = 0.02f * std::sin(2.0f * static_cast<float>(kPi) * 3500.0f * static_cast<float>(i) / 48000.0f);
        const float fixed = mnmFix.process(0, plain, 127.0f);
        const float neutral = mnmFix.process(1, plain, 64.0f);
        require(std::abs(neutral - plain) < 1.0e-7f,
                "MNM FIX must be exact MNM at the neutral DIST point");
        if (i >= 2000) { plainEnergy += plain * plain; fixedEnergy += fixed * fixed; }
    }
    const float mnmFixGain = std::sqrt(fixedEnergy / plainEnergy);
    require(mnmFixGain > 1.25f && mnmFixGain < 1.60f,
            "MNM FIX 3.5 kHz compensation is outside its defined A/B range");

    // Direct high-Q cutoff-step regression after the selected 16-sample
    // imported-side coefficient ramp. Native FilterCore and OLD are not
    // involved; this asserts the click-reduction target directly.
    struct CutoffStepMeasure { float boundaryJump = 0.0f, maxJump = 0.0f; };
    const auto cutoffStepMeasure=[](int lowerMode,int upperMode,bool moveLower){
        monomachine::mnm::FilterCore filter;
        filter.setSampleRate(48000.0);
        filter.setHybridTestModes(lowerMode,upperMode);
        // Move only the selected physical side: for MODE L retain the upper
        // cutoff, for MODE H retain BASE/the lower cutoff.
        filter.setParameters(20.0f,moveLower?100.0f:40.0f,127.0f,127.0f,0.0f,127.0f,0.0f,0.0f);
        float previous=0.0f;CutoffStepMeasure measure{};
        for(int i=0;i<1024;++i){
            if(i==512){
                if(moveLower)filter.setParameters(84.0f,36.0f,127.0f,127.0f,0.0f,127.0f,0.0f,0.0f);
                else filter.setParameters(20.0f,104.0f,127.0f,127.0f,0.0f,127.0f,0.0f,0.0f);
            }
            const float output=filter.process(0,0.8f*std::sin(static_cast<float>(i)*0.071f));
            requireFinite(output,"high-Q cutoff step must stay finite");
            if(i>0){const float jump=std::abs(output-previous);measure.maxJump=std::max(measure.maxJump,jump);if(i==512)measure.boundaryJump=jump;}
            previous=output;
        }
        return measure;
    };
    const auto k35LpStep=cutoffStepMeasure(hybridK35Lp,hybridNative,true);
    const auto k35HpStep=cutoffStepMeasure(hybridK35Hp,hybridNative,true);
    const auto ladderHp24Step=cutoffStepMeasure(hybridNative,hybridMoogHp24,false);
    const auto ladderHp12Step=cutoffStepMeasure(hybridNative,hybridMoogHp12,false);
    // The requested 16-sample coefficient ramp specifically removes the
    // instant coefficient discontinuity at the control boundary. Retain and
    // print the later high-Q peak separately: that is resonant settling, not
    // something this short ramp can truthfully claim to eliminate.
    require(k35LpStep.boundaryJump<0.4f&&k35HpStep.boundaryJump<0.4f
            &&ladderHp24Step.boundaryJump<0.4f&&ladderHp12Step.boundaryJump<0.4f,
            "16-sample imported cutoff ramp did not contain the boundary step");
    for(int mode=hybridR303Lp;mode<=hybridRMoogHp12;++mode){
        const auto lowerStep=cutoffStepMeasure(mode,hybridNative,true);
        const auto upperStep=cutoffStepMeasure(hybridNative,mode,false);
        require(lowerStep.boundaryJump<0.8f&&upperStep.boundaryJump<0.8f,
                "R-classic cutoff ramp did not contain the control-boundary step");
    }

    for (int mode = 1; mode <= 3; ++mode) {
        OversamplingDistortionStereo saturation;
        saturation.setMode(mode);
        float energy = 0.0f;
        for (int i = 0; i < 2048; ++i) {
            const float input = 1.4f * std::sin(static_cast<float>(i) * 0.15f);
            const float left = saturation.process(0, input, 110.0f);
            const float right = saturation.process(1, 0.0f, 110.0f);
            requireFinite(left, "hybrid saturation output must stay finite");
            requireFinite(right, "hybrid saturation right channel must stay finite");
            energy += std::abs(left);
        }
        require(energy > 0.01f, "hybrid saturation mode must render a signal");
    }

    std::cout << "Hybrid K35/Odin and appended R 303/R MS20/R MOOG response, mapping, stability, stereo, and declick checks passed; MNM FIX gain=" << mnmFixGain << ", high-Q cutoff steps boundary/max: K35 LP=" << k35LpStep.boundaryJump << '/' << k35LpStep.maxJump << ", K35 HP=" << k35HpStep.boundaryJump << '/' << k35HpStep.maxJump << ", ladder HP24=" << ladderHp24Step.boundaryJump << '/' << ladderHp24Step.maxJump << ", ladder HP12=" << ladderHp12Step.boundaryJump << '/' << ladderHp12Step.maxJump << '\n';
    return 0;
}
