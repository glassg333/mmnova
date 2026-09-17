#include "../models/machine_definitions.hpp"
#include "../models/track_pages.hpp"
#include "../ui/MonomachineGuiLayout.hpp"
#include "../dsp/monomachine_filter.hpp"
#include "../dsp/monomachine_chorus.hpp"
#include "../dsp/monomachine_effects.hpp"
#include "../dsp/monomachine_voice_chain.hpp"

#include <iostream>
#include <cassert>
#include <vector>

int main() {
    std::cout << "Testing Monomachine Full Architecture Suite...\n";

    // 1. Verify all machine definitions
    const auto& machines = monomachine::getAllMachineDefinitions();
    std::cout << "Loaded " << machines.size() << " machine definitions.\n";
    assert(machines.size() == 22 && "Should define exactly 22 stock Monomachine machines (15 synth + 7 FX)");

    // Check dropdown hierarchy
    auto menu = monomachine::MonomachineGuiLayout::getMachineDropdownMenuHierarchy();
    std::cout << "Dropdown items count: " << menu.size() << "\n";
    assert(!menu.empty());

    // 2. Check GUI 6-section grid layout
    auto grid = monomachine::MonomachineGuiLayout::createFullGridLayout(10); // FM+ DYN
    assert(grid[0].sectionTitle == "SYNTHESIS");
    assert(grid[1].sectionTitle == "AMPLIFICATION");
    assert(grid[2].sectionTitle == "LFO1");
    assert(grid[3].sectionTitle == "FILTER");
    assert(grid[4].sectionTitle == "EFFECTS");
    assert(grid[5].sectionTitle == "LFO2 [LFO3]");
    std::cout << "GUI 6-section grid validation passed.\n";

    // 3. Test DSP Modules: Phaser, Flanger, RingMod, Chorus, Filter
    constexpr size_t kFrames = 256;
    std::vector<float> inL(kFrames, 0.4f);
    std::vector<float> inR(kFrames, -0.4f);
    std::vector<float> outL(kFrames, 0.0f);
    std::vector<float> outR(kFrames, 0.0f);

    monomachine::MonomachinePhaser phaser;
    phaser.reset(44100.0);
    phaser.setParameters(64, 80, 50, 64, 64);
    phaser.processStereo(inL.data(), inR.data(), outL.data(), outR.data(), kFrames);

    monomachine::MonomachineFlanger flanger;
    flanger.reset(44100.0);
    flanger.setParameters(30, 70, 45, 64, 64);
    flanger.processStereo(outL.data(), outR.data(), outL.data(), outR.data(), kFrames);

    monomachine::MonomachineRingMod ringMod;
    ringMod.reset(44100.0);
    ringMod.setParameters(0, 0, 50, 60.0f);
    ringMod.processStereo(outL.data(), outR.data(), outL.data(), outR.data(), kFrames);

    monomachine::MonomachineChorus chorus;
    chorus.reset(44100.0);
    chorus.setParameters(32, 64, 48, 64, 64);
    chorus.processStereo(outL.data(), outR.data(), outL.data(), outR.data(), kFrames);

    monomachine::MonomachineFilter filter;
    filter.reset(44100.0);
    filter.setParameters(20, 80, 30, 30, 0, 64, 0, 0);
    filter.processStereo(outL.data(), outR.data(), outL.data(), outR.data(), kFrames);

    bool hasSignal = false;
    for (size_t i = 0; i < kFrames; ++i) {
        if (std::abs(outL[i]) > 0.0001f || std::abs(outR[i]) > 0.0001f) {
            hasSignal = true;
            break;
        }
    }
    assert(hasSignal && "DSP output should be valid audio signal");

    std::cout << "All DSP and GUI tests completed successfully!\n";
    return 0;
}
