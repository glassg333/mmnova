#include "../dsp/monomachine_filter.hpp"
#include "../dsp/monomachine_chorus.hpp"
#include "../dsp/monomachine_voice_chain.hpp"
#include <iostream>
#include <cassert>
#include <vector>

int main() {
    std::cout << "Testing Monomachine native DSP modules...\n";

    monomachine::MonomachineVoiceChain chain;
    chain.reset(44100.0);

    chain.chorus().setParameters(32, 64, 48, 64, 64);
    chain.filter().setParameters(20, 80, 40, 40, 0, 64, 0, 0);
    chain.setDistortion(10);
    chain.setVolume(100);
    chain.setPan(64);
    chain.setDelay(64, 30, 50);

    constexpr size_t kFrames = 128;
    std::vector<float> inL(kFrames, 0.5f);
    std::vector<float> inR(kFrames, -0.5f);
    std::vector<float> outL(kFrames, 0.0f);
    std::vector<float> outR(kFrames, 0.0f);

    chain.processBlock(inL.data(), inR.data(), outL.data(), outR.data(), kFrames);

    bool hasSignal = false;
    for (size_t i = 0; i < kFrames; ++i) {
        if (std::abs(outL[i]) > 0.001f || std::abs(outR[i]) > 0.001f) {
            hasSignal = true;
            break;
        }
    }

    assert(hasSignal && "Output should not be silence");
    std::cout << "Monomachine native DSP test passed successfully!\n";
    return 0;
}
