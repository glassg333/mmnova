// Regression coverage for the documented Monomachine track-effect topology.
// The TrackChain compositor is shared by P1/P2 and must remain:
// EQ -> FILT -> DIST -> AMP ENV -> VOL/PAN -> SRR -> DELAY.
#include "NovaDSP.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace
{
constexpr int kFrames = 64;

void require(bool value, const char* message)
{
    if (! value)
        throw std::runtime_error(message);
}

std::array<float, 32> controls()
{
    std::array<float, 32> p{};
    p.fill(64.0f);
    p[12] = 116.0f; // DIST
    p[13] = 91.0f;  // VOL
    p[14] = 102.0f; // PAN
    p[16] = 37.0f;  // FILT BASE
    p[17] = 73.0f;  // FILT WDTH
    p[18] = 0.0f;   // HPQ
    p[19] = 0.0f;   // LPQ
    p[20] = 18.0f;  // FILT ATK
    p[21] = 81.0f;  // FILT DEC
    p[22] = 64.0f;  // BOFS neutral
    p[23] = 64.0f;  // WOFS neutral
    p[24] = 97.0f;  // EQ frequency
    p[25] = 106.0f; // EQ gain
    p[26] = 93.0f;  // SRR
    p[27] = 88.0f;  // DELAY time
    p[28] = 108.0f; // DELAY send
    p[29] = 0.0f;   // no feedback: order still remains observable
    return p;
}

void configure(nova::TrackChain& chain)
{
    chain.prepare(48000.0);
    chain.setModes(monomachine::dspModeMnm, monomachine::dspModeMnm,
                   monomachine::dspModeMnm, monomachine::dspModeMnm);
    chain.set(controls());
    chain.trigger();
}

float maxDifference(const std::array<float, kFrames>& a,
                    const std::array<float, kFrames>& b)
{
    float result = 0.0f;
    for (int i = 0; i < kFrames; ++i)
        result = std::max(result, std::abs(a[static_cast<size_t>(i)] - b[static_cast<size_t>(i)]));
    return result;
}

void requireFinite(const std::array<float, kFrames>& values, const char* message)
{
    for (const float value : values)
        require(std::isfinite(value), message);
}
}

int main()
{
    try
    {
        nova::TrackChain compositor, documented, previous;
        configure(compositor);
        configure(documented);
        configure(previous);

        std::array<float, kFrames> inL{}, inR{}, amp{};
        for (int i = 0; i < kFrames; ++i)
        {
            const float t = static_cast<float>(i);
            inL[static_cast<size_t>(i)] = 0.62f * std::sin(0.173f * t) + (i == 0 ? 0.38f : 0.0f);
            inR[static_cast<size_t>(i)] = 0.47f * std::cos(0.119f * t) - (i == 7 ? 0.21f : 0.0f);
            amp[static_cast<size_t>(i)] = 0.12f + 0.82f * static_cast<float>(i) / static_cast<float>(kFrames - 1);
        }

        auto actualL = inL, actualR = inR;
        compositor.process(actualL.data(), actualR.data(), amp.data(), kFrames);

        auto documentedL = inL, documentedR = inR;
        documented.stageEQ(documentedL.data(), documentedR.data(), kFrames);
        documented.stageFILT(documentedL.data(), documentedR.data(), kFrames);
        documented.stageDIST(documentedL.data(), documentedR.data(), kFrames);
        documented.stageENV(documentedL.data(), documentedR.data(), amp.data(), kFrames);
        documented.stageVOLPAN(documentedL.data(), documentedR.data(), kFrames);
        documented.stageSRR(documentedL.data(), documentedR.data(), kFrames);
        documented.stageDELAY(documentedL.data(), documentedR.data(), kFrames);

        // The preceding refactor's DIST-before-FILT default is deliberately a
        // negative control. It must remain observably different from the
        // documented post-filter DIST path.
        auto previousL = inL, previousR = inR;
        previous.stageDIST(previousL.data(), previousR.data(), kFrames);
        previous.stageEQ(previousL.data(), previousR.data(), kFrames);
        previous.stageFILT(previousL.data(), previousR.data(), kFrames);
        previous.stageENV(previousL.data(), previousR.data(), amp.data(), kFrames);
        previous.stageVOLPAN(previousL.data(), previousR.data(), kFrames);
        previous.stageSRR(previousL.data(), previousR.data(), kFrames);
        previous.stageDELAY(previousL.data(), previousR.data(), kFrames);

        const float documentedError = std::max(maxDifference(actualL, documentedL),
                                                maxDifference(actualR, documentedR));
        const float previousOrderDelta = std::max(maxDifference(actualL, previousL),
                                                  maxDifference(actualR, previousR));
        require(documentedError < 2.0e-6f,
                "TrackChain::process no longer matches the documented stage order");
        require(previousOrderDelta > 1.0e-4f,
                "topology regression test did not distinguish the previous DIST-before-FILT order");
        requireFinite(actualL, "non-finite left result");
        requireFinite(actualR, "non-finite right result");

        std::printf("TRACKTOPOLOGY documentedError=%.9g previousDelta=%.9g PASS\n",
                    documentedError, previousOrderDelta);
        return 0;
    }
    catch (const std::exception& error)
    {
        std::fprintf(stderr, "TRACKTOPOLOGY FAIL: %s\n", error.what());
        return 1;
    }
}
