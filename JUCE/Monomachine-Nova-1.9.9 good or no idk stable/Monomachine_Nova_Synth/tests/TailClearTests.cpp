// Regression: PANIC / CLEAR TAILS must clear every stateful DSP branch.
// In particular, the native classic filter/delay and active FX-slots must not
// replay residue after the user has explicitly cleared tails.
#include "PluginProcessor.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

static void require(bool value, const char* message)
{
    if (! value)
        throw std::runtime_error(message);
}

static float nativeTrackResidueAfterClear()
{
    nova::TrackChain chain;
    chain.volumeReference = 64.0f;
    chain.prepare(44100.0);

    std::array<float, 32> p{};
    p.fill(64.0f);
    p[13] = 64.0f; p[14] = 64.0f; // VOL/PAN: unity / centre
    p[16] = 0.0f; p[17] = 127.0f; p[18] = 0.0f; p[19] = 0.0f;
    p[20] = 0.0f; p[21] = 93.0f; p[22] = 64.0f; p[23] = 64.0f;
    p[24] = 64.0f; p[25] = 64.0f; p[26] = 0.0f; p[27] = 127.0f;
    p[28] = 127.0f; p[29] = 127.0f; p[30] = 0.0f; p[31] = 127.0f;
    chain.set(p);
    chain.setModes(monomachine::dspModeMnm, monomachine::dspModeMnm,
                   monomachine::dspModeMnm, 0);

    float l[64]{}, r[64]{}, amp[64]{};
    for (auto& v : amp) v = 1.0f;
    for (int block = 0; block < 800; ++block)
    {
        for (int i = 0; i < 64; ++i)
            l[i] = r[i] = (block == 0 && i == 0) ? 0.8f : 0.0f;
        chain.process(l, r, amp, 64);
    }

    chain.clear();
    float residue = 0.0f;
    for (int block = 0; block < 800; ++block)
    {
        for (int i = 0; i < 64; ++i) l[i] = r[i] = 0.0f;
        chain.process(l, r, amp, 64);
        for (float sample : l) residue = std::max(residue, std::abs(sample));
    }
    return residue;
}

static std::pair<float, float> fxSlotTail()
{
    nova::FxSlotEngine slot;
    slot.prepare(44100.0);
    const std::array<float, 8> p{{70.0f, 95.0f, 41.0f, 127.0f,
                                   127.0f, 127.0f, 127.0f, 64.0f}};
    slot.setState(15, true, p); // native CHORUS

    float l[64]{}, r[64]{};
    float before = 0.0f;
    for (int block = 0; block < 100; ++block)
    {
        for (int i = 0; i < 64; ++i)
            l[i] = r[i] = 0.4f * std::sin(float(block * 64 + i) * 0.11f);
        slot.process(l, r, 64);
        for (float sample : l) before = std::max(before, std::abs(sample));
    }
    for (int block = 0; block < 256; ++block)
    {
        for (int i = 0; i < 64; ++i) l[i] = r[i] = 0.0f;
        slot.process(l, r, 64);
        for (float sample : l) before = std::max(before, std::abs(sample));
    }

    slot.clear();
    float after = 0.0f;
    for (int block = 0; block < 64; ++block)
    {
        for (int i = 0; i < 64; ++i) l[i] = r[i] = 0.0f;
        slot.process(l, r, 64);
        for (float sample : l) after = std::max(after, std::abs(sample));
    }
    return {before, after};
}

int main()
{
    try
    {
        const float track = nativeTrackResidueAfterClear();
        const auto slot = fxSlotTail();
        require(track < 1.0e-7f, "native TrackChain residue survived clear");
        require(slot.first > 0.01f && slot.second < 1.0e-7f,
                "FX-slot tail survived clear");
        std::cout << "TAILCLEAR track=" << track
                  << " slot=" << slot.first << " -> " << slot.second
                  << " PASS\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "TAILCLEAR FAIL: " << error.what() << '\n';
        return 1;
    }
}
