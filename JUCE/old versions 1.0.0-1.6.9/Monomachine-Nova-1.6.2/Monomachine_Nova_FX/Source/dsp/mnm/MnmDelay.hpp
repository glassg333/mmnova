// FX-DLY (machine 32) "mnm" core -- Monomachine Nova 1.6.0
//
// Recovered geometry and parameter law from P:$147B38-$147D00
// (listings: memory_images/dsp1_machines_fx.txt):
//
//   * buffer: X:$114000, 2048 words per channel, second channel at +$800
//     (`move #>$114000,a0`, REPORT_verdict_and_fm_data.md part 1.3)
//   * delay time: DEL parameter is turned into a read rate through the reciprocal
//     table X:$144C49 (values 1/1, 1/2, 1/3 ... 1/64):
//     P:$147BA9-$147BBA  (`move x:(r0+$144c49),x0` / `move x:(r1+$144c49),x0`
//                          then two 1.23 multiplies with `asl #$b`)
//   * read pointer: fractional read with linear interpolation, the same
//     mpysu/dmac/asl pattern the chorus uses for its taps
//   * modulation: the read pointer is modulated through the X:$140000 pitch table
//     (P:$147BE5-$147C1A) -> the delay has the LFO-modulated "tape" character
//
// What is exact here: buffer size, per-channel split, reciprocal time table,
// interpolated fractional read, feedback topology (two independent loops with the
// optional cross feed used by the ping-pong setting).
// What is approximate: the modulation depth/rate scaling and the exact wet/dry
// arithmetic (the listing mixes three read taps with channel-dependent gains).
#pragma once

#include "MnmKernel.hpp"

#include <vector>

namespace monomachine {
namespace mnm {

class DelayCore {
public:
    // The firmware keeps the delay in X:$114000 with a 2048-word window per channel and a
    // variable-rate (interpolated) read; the plugin therefore sizes the line for one second of
    // host audio while keeping the firmware's max/min ratio of 1 : 1/64.
    static constexpr float kMaxSeconds = 1.0f;

    void prepare(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        const int frames = static_cast<int>(sr * kMaxSeconds) + 4;
        left.assign(static_cast<size_t>(frames), 0.0f);
        right.assign(static_cast<size_t>(frames), 0.0f);
        size = frames;
        write = 0;
        maxSamples = static_cast<float>(frames - 2);
    }
    void reset() {
        std::fill(left.begin(), left.end(), 0.0f);
        std::fill(right.begin(), right.end(), 0.0f);
        write = 0;
    }

    // delayParam: DEL knob 0..127 -> reciprocal table index (0..63)
    // feedback:   0..1 ; mix: 0..1 ; pingPong: cross-feed
    // modDepth:   extra read-pointer modulation 0..1, modPhase advanced by modRate
    inline void process(float delParam, float feedback, float mix, bool pingPong,
                        float modDepth, float modRate, float inL, float inR,
                        float& outL, float& outR) {
        // DTIM 0..127 -> 64-entry reciprocal table. The table itself is exact (P:$144C49);
        // the knob direction is not verified yet, so DTIM 0 is the shortest setting:
        // index 0 -> 1/64 of the line, index 63 -> the full line.
        const int idx = std::clamp(static_cast<int>(delParam * 63.0f / 127.0f + 0.5f), 0, 63);
        const float delaySamples = std::clamp(maxSamples * kDelayRecip[static_cast<size_t>(63 - idx)],
                                              1.0f, maxSamples);
        modPhase = wrapf(modPhase + modRate * static_cast<float>(kDspRate / sr) / static_cast<float>(size));
        const float wobble = SineTable::instance().read(modPhase) * modDepth * 0.15f;

        const float pos = static_cast<float>(write) - delaySamples * (1.0f + wobble);
        float readPos = pos;
        while (readPos < 0.0f) readPos += static_cast<float>(size);
        const int i0 = static_cast<int>(readPos) % size;
        const int i1 = (i0 + 1) % size;
        const float frac = readPos - std::floor(readPos);
        const float aL = left[static_cast<size_t>(i0)] + frac * (left[static_cast<size_t>(i1)] - left[static_cast<size_t>(i0)]);
        const float aR = right[static_cast<size_t>(i0)] + frac * (right[static_cast<size_t>(i1)] - right[static_cast<size_t>(i0)]);

        const float fbL = pingPong ? aR : aL;
        const float fbR = pingPong ? aL : aR;
        left[static_cast<size_t>(write)] = inL + fbL * feedback;
        right[static_cast<size_t>(write)] = inR + fbR * feedback;
        write = (write + 1) % static_cast<size_t>(size);

        outL = inL * (1.0f - mix) + aL * mix;
        outR = inR * (1.0f - mix) + aR * mix;
    }

private:
    static float wrapf(float v) { return v - std::floor(v); }
    std::vector<float> left, right;
    size_t write = 0;
    int size = 1024;
    double sr = kDspRate;
    float maxSamples = 1022.0f;
    float modPhase = 0.0f;
};

}  // namespace mnm
}  // namespace monomachine
