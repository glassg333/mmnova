// Offline checks for the "mnm" (OS 1.32B) DSP cores -- Monomachine Nova 1.6.0
//
// These tests use only the recovered tables and structures, no JUCE, so they build and run
// anywhere. They are the fast regression net for the firmware port: if a table, an index law
// or a state machine regresses, one of these lines fails.
#include "dsp/mnm/MnmFm.hpp"
#include "dsp/mnm/MnmKernel.hpp"
#include "dsp/mnm/MnmDelay.hpp"

#include <cmath>
#include <cstdio>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& what, float measured, float expected, float tolerance) {
    const bool ok = condition && std::fabs(measured - expected) <= tolerance;
    if (!ok) ++failures;
    std::printf("%-58s measured %12.6f  expected %12.6f  %s\n", what.c_str(), measured, expected,
                ok ? "OK" : "FAIL");
}

void checkTrue(bool condition, const std::string& what) {
    if (!condition) ++failures;
    std::printf("%-58s %s\n", what.c_str(), condition ? "OK" : "FAIL");
}

}  // namespace

int main() {
    using namespace monomachine::mnm;

    // ---------------------------------------------------------------- AMP envelope
    // P:$141800 attack table: ATK=0 must be instant (0.5 per block), ATK=64 is 152 ms.
    {
        AmpEnvelope env;
        env.setParameters(0.0f, 127.0f, 127.0f, 0.0f);
        env.trigger();
        // ATK=0 adds 0.5 per 16-sample block, so two blocks reach the top.
        for (int i = 0; i < 40; ++i) env.tick();
        check(true, "AMP attack ATK=0 reaches full in two blocks", env.value(), 1.0f, 0.02f);

        AmpEnvelope slow;
        slow.setParameters(64.0f, 127.0f, 127.0f, 0.0f);
        slow.trigger();
        int samples = 0;
        while (slow.value() < 1.0f && samples < 44100 * 5) { slow.tick(); ++samples; }
        check(true, "AMP attack ATK=64 ramp time (ms)", samples / 44.1f, 152.4f, 5.0f);

        // P:$141880 decay table: DEC=64 gives the shipped reference slope near -34.8 dB/s.
        AmpEnvelope dec;
        dec.setParameters(0.0f, 64.0f, 64.0f, 0.0f);
        dec.trigger();
        for (int i = 0; i < 64; ++i) dec.tick();             // settle the attack
        const float before = dec.value();
        for (int i = 0; i < 44100; ++i) dec.tick();          // one second of 16-sample blocks
        const float after = dec.value();
        check(true, "AMP decay DEC=64 slope (dB per second)",
              20.0f * std::log10(std::max(1.0e-9f, after / before)), -34.80f, 1.0f);
    }

    // ---------------------------------------------------------------- tone / LP tables
    {
        // P:$144AC7 bank A is a one-pole: c=0.002211 at 15.5 Hz, c=0.912 at 17 kHz.
        check(true, "LP bank A coefficient idx 0", kLpCoeffA[0], 0.0022113f, 2.0e-6f);
        // The table is two 129-word banks; the tail word (0.9120057, ~17 kHz) sits at index 128
        // and is unreachable for a 7-bit TONE parameter, so the arrays stop at index 127.
        check(true, "LP bank A coefficient idx 127", kLpCoeffA[127], 0.9096855f, 1.0e-6f);
        check(true, "LP bank B coefficient idx 0", kLpCoeffB[0], 0.0337372f, 2.0e-6f);
        ToneLowpass lp;
        lp.setSampleRate(44100.0);
        lp.setTone(64.0f, ToneLowpass::Bank::A);
        float y = 0.0f;
        for (int i = 0; i < 44100; ++i) y = lp.process(0, 1.0f);
        check(true, "LP DC gain is unity (one-pole y += c(x-y))", y, 1.0f, 0.01f);
    }

    // ---------------------------------------------------------------- FM
    {
        FmCore fm;
        fm.reset(44100.0);
        std::array<float, 8> stat{64.0f, 64.0f, 80.0f, 30.0f, 80.0f, 64.0f, 98.0f, 64.0f};
        fm.setParameters(FmKind::Stat, stat);
        fm.noteOn(60.0f, 1.0f);
        float buffer[kBlock]{};
        float peak = 0.0f, energy = 0.0f;
        for (int block = 0; block < 64; ++block) {
            fm.processBlock(buffer, kBlock);
            for (int i = 0; i < kBlock; ++i) {
                peak = std::max(peak, std::fabs(buffer[i]));
                energy += buffer[i] * buffer[i];
            }
        }
        checkTrue(peak > 0.2f && peak <= 1.01f, "FM+STAT produces a bounded signal");
        checkTrue(energy > 0.01f, "FM+STAT is not silent");
        // P:$141A80: 24 entries from 1/32 to 8; index law from P:$145DA5-$145DAD.
        check(true, "FM ratio table entry 0", kFmRatio[0], 1.0f / 32.0f, 1.0e-6f);
        check(true, "FM ratio table entry 23", kFmRatio[23], 8.0f, 1.0e-6f);
    }

    // ---------------------------------------------------------------- delay
    {
        DelayCore delay;
        delay.prepare(44100.0);
        float wl = 0.0f, wr = 0.0f;
        // DTIM = 127 marks the longest setting: the full one-second line.
        delay.process(127.0f, 0.0f, 1.0f, false, 0.0f, 0.0f, 1.0f, 1.0f, wl, wr);
        int samples = 1;
        for (; samples < 44100 * 2; ++samples) {
            delay.process(127.0f, 0.0f, 1.0f, false, 0.0f, 0.0f, 0.0f, 0.0f, wl, wr);
            if (std::fabs(wl) > 1.0e-5f) break;
        }
        check(true, "DLY longest setting echo time (ms)", samples / 44.1f, 1000.0f, 2.0f);

        DelayCore shortDelay;
        shortDelay.prepare(44100.0);
        shortDelay.process(0.0f, 0.0f, 1.0f, false, 0.0f, 0.0f, 1.0f, 1.0f, wl, wr);
        samples = 1;
        for (; samples < 44100; ++samples) {
            shortDelay.process(0.0f, 0.0f, 1.0f, false, 0.0f, 0.0f, 0.0f, 0.0f, wl, wr);
            if (std::fabs(wl) > 1.0e-5f) break;
        }
        // P:$144C49 index 63 = 1/64 of the line.
        check(true, "DLY shortest setting echo time (ms)", samples / 44.1f, 1000.0f / 64.0f, 1.0f);
    }

    // ---------------------------------------------------------------- saturation (DIST)
    {
        check(true, "DIST neutral is transparent", Saturator::process(0.5f, 64.0f), 0.5f, 1.0e-4f);
        checkTrue(std::fabs(Saturator::process(0.5f, 127.0f)) < 0.5f, "DIST at 127 compresses");
        checkTrue(std::fabs(Saturator::process(2.0f, 127.0f)) <= 1.0f, "DIST at 127 cannot exceed full scale");
    }

    std::printf(failures == 0 ? "\nMNM core checks: all OK\n" : "\nMNM core checks: %d FAILED\n", failures);
    return failures == 0 ? 0 : 1;
}
