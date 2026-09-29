// Regression guard for opt-in FILT extras.  This target deliberately stays
// JUCE-free so it can prove neutral MNM/OLD behaviour independently of APVTS.
#include "dsp/hybrid_private/HybridDSP.hpp"
#include "dsp/mnm/MnmRealFilter.hpp"
#include "dsp/monomachine_filter.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace
{
void require(bool condition, const char* message)
{
    if (! condition) {
        std::fprintf(stderr, "FILTEREXTRAS FAIL: %s\n", message);
        std::exit(1);
    }
}

float stimulus(int sample)
{
    return 0.57f * std::sin(0.071f * static_cast<float>(sample))
         + 0.19f * std::sin(0.193f * static_cast<float>(sample));
}
}

int main()
{
    // MNM: merely calling the new modifier bridge with zeros must not recook
    // coefficients or alter the reconstructed core at all.
    monomachine::mnm::FilterCore mnmReference, mnmNeutral;
    for (auto* f : { &mnmReference, &mnmNeutral }) {
        f->setSampleRate(48000.0);
        f->setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
        f->trigger();
    }
    float mnmChanged = 0.0f;
    for (int i = 0; i < 2048; ++i) {
        mnmNeutral.setExternalFilterModifiers(0.0f, 0.0f, 0.0f, 0.0f);
        const float in = stimulus(i);
        const float a = mnmReference.process(i & 1, in);
        const float b = mnmNeutral.process(i & 1, in);
        require(a == b, "zero extras changed MNM output");
    }

    monomachine::mnm::FilterCore mnmModulated;
    mnmModulated.setSampleRate(48000.0);
    mnmModulated.setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
    mnmModulated.setExternalFilterModifiers(24.0f, -18.0f, 17.0f, -12.0f);
    mnmModulated.trigger();
    monomachine::mnm::FilterCore mnmUnmodulated;
    mnmUnmodulated.setSampleRate(48000.0);
    mnmUnmodulated.setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
    mnmUnmodulated.trigger();
    for (int i = 0; i < 2048; ++i) {
        const float in = stimulus(i);
        const float a = mnmUnmodulated.process(i & 1, in);
        const float b = mnmModulated.process(i & 1, in);
        require(std::isfinite(b), "MNM modifier path produced non-finite output");
        mnmChanged += std::abs(a - b);
    }
    require(mnmChanged > 0.01f, "MNM VEL/KT/FIL ENV bridge had no effect");

    // Imported K35 and ladder choices run inside the MNM core. Verify every
    // physical-side algorithm receives the same opt-in bridge, while a zero
    // bridge does not recook its coefficients or change its output.
    for (int mode = 1; mode <= 8; ++mode) {
        monomachine::mnm::FilterCore hybridReference, hybridNeutral, hybridModulated;
        for (auto* f : { &hybridReference, &hybridNeutral, &hybridModulated }) {
            f->setSampleRate(48000.0);
            f->setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
            f->setHybridTestModes(mode, mode);
            f->trigger();
        }
        hybridNeutral.setExternalFilterModifiers(0.0f, 0.0f, 0.0f, 0.0f);
        hybridModulated.setExternalFilterModifiers(24.0f, -18.0f, 17.0f, -12.0f);
        float hybridDifference = 0.0f;
        for (int i = 0; i < 2048; ++i) {
            const float in = stimulus(i);
            const float reference = hybridReference.process(i & 1, in);
            const float neutral = hybridNeutral.process(i & 1, in);
            const float modulated = hybridModulated.process(i & 1, in);
            require(reference == neutral, "zero extras changed imported K35/ladder output");
            require(std::isfinite(modulated), "imported K35/ladder modifier path produced non-finite output");
            hybridDifference += std::abs(reference - modulated);
        }
        require(hybridDifference > 0.01f, "an imported K35/ladder filter side ignored VEL/KT/FIL ENV");
    }

    // OLD remains a separate reference. Zero extras are bit-identical there,
    // too; nonzero modifiers must be opt-in rather than silently ignored.
    monomachine::MonomachineFilter oldReference, oldNeutral;
    for (auto* f : { &oldReference, &oldNeutral }) {
        f->reset(48000.0);
        f->setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
        f->triggerEnvelope();
    }
    float oldChanged = 0.0f;
    for (int i = 0; i < 2048; ++i) {
        oldNeutral.setExternalFilterModifiers(0.0f, 0.0f, 0.0f, 0.0f);
        const float inL = stimulus(i), inR = stimulus(i + 17);
        float refL = 0.0f, refR = 0.0f, zeroL = 0.0f, zeroR = 0.0f;
        oldReference.processStereo(&inL, &inR, &refL, &refR, 1);
        oldNeutral.processStereo(&inL, &inR, &zeroL, &zeroR, 1);
        require(refL == zeroL && refR == zeroR, "zero extras changed OLD reference output");
        oldChanged += std::abs(refL) + std::abs(refR);
    }
    require(oldChanged > 0.01f, "OLD reference test did not render signal");

    monomachine::MonomachineFilter oldModulated;
    oldModulated.reset(48000.0);
    oldModulated.setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
    oldModulated.setExternalFilterModifiers(24.0f, -18.0f, 17.0f, -12.0f);
    oldModulated.triggerEnvelope();
    monomachine::MonomachineFilter oldPlain;
    oldPlain.reset(48000.0);
    oldPlain.setParameters(28.0f, 71.0f, 37.0f, 48.0f, 11.0f, 92.0f, -9.0f, 12.0f);
    oldPlain.triggerEnvelope();
    float oldModifierDifference = 0.0f;
    for (int i = 0; i < 2048; ++i) {
        const float inL = stimulus(i), inR = stimulus(i + 17);
        float aL = 0.0f, aR = 0.0f, bL = 0.0f, bR = 0.0f;
        oldPlain.processStereo(&inL, &inR, &aL, &aR, 1);
        oldModulated.processStereo(&inL, &inR, &bL, &bR, 1);
        require(std::isfinite(bL) && std::isfinite(bR), "OLD modifier path produced non-finite output");
        oldModifierDifference += std::abs(aL - bL) + std::abs(aR - bR);
    }
    require(oldModifierDifference > 0.01f, "OLD VEL/KT/FIL ENV bridge had no effect");

    // SAT follows the Korg/Odin applyOverdrive law: raw zero is exact bypass,
    // mid values blend linearly with tanh(3*x), and maximum drives tanh(6*x).
    nova::hybrid_private::OdinKorgFilterSaturationStereo saturation;
    constexpr std::array<float, 7> probes{{-2.0f, -0.75f, -0.1f, 0.0f, 0.1f, 0.75f, 2.0f}};
    saturation.setAmount(0.0f);
    for (const float x : probes)
        require(saturation.process(0, x) == x, "SAT=0 is not an exact bypass");
    saturation.setAmount(32.0f);
    const float amount = 32.0f * (2.0f / 127.0f);
    for (const float x : probes) {
        const float expected = x * (1.0f - amount) + amount * std::tanh(3.0f * x);
        require(std::abs(saturation.process(1, x) - expected) < 1.0e-6f,
                "SAT midrange is not the Korg/Odin dry-to-overdrive law");
    }
    saturation.setAmount(127.0f);
    for (const float x : probes) {
        const float expected = std::tanh(6.0f * x);
        require(std::abs(saturation.process(0, x) - expected) < 1.0e-6f,
                "SAT maximum is not the Korg/Odin driven law");
    }

    std::puts("FILTEREXTRAS MNM/OLD/imported neutral and opt-in paths plus Korg/Odin SAT PASS");
    return 0;
}
