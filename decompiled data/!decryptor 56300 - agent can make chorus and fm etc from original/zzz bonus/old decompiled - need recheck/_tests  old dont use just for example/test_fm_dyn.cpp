#include "../dsp/monomachine_fm_dynamic.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

int main() {
    std::cout << "Testing Monomachine FM+ DYNAMIC (Machine 10)...\n";

    monomachine::MonomachineFmDynamic fm;
    fm.reset(44100.0);

    // Test parameter formatting matching gui.jpg (1FRQ -> 1.0, 2FRQ -> 1.33)
    std::string str1Frq = monomachine::MonomachineFmDynamic::formatParamValue(0, 16);
    std::cout << "1FRQ format check (val 16): " << str1Frq << "\n";
    assert(str1Frq == "1.00");

    std::string str2Frq = monomachine::MonomachineFmDynamic::formatParamValue(4, 42);
    std::cout << "2FRQ format check (val 42): " << str2Frq << "\n";
    assert(str2Frq.find("1.3") != std::string::npos);

    // Test audio processing with MIDI Note 60 (Middle C)
    // 1FRQ=16 (1.0 ratio), 1FEN=64 (neutral), 1VOL=64, 1VEN=0, 2FRQ=42 (~1.33 ratio), 2ENV=80, 2FB=30, TUNE=64
    fm.setParameters(16, 64, 64, 0, 42, 80, 30, 64);
    fm.noteOn(60, 127);

    constexpr size_t kFrames = 512;
    std::vector<float> outL(kFrames, 0.0f);
    std::vector<float> outR(kFrames, 0.0f);

    fm.processStereo(outL.data(), outR.data(), kFrames);

    float peak = 0.0f;
    for (size_t i = 0; i < kFrames; ++i) {
        peak = std::max(peak, std::abs(outL[i]));
    }

    std::cout << "FM+ Dynamic output peak: " << peak << "\n";
    assert(peak > 0.1f && "FM+ Dynamic should produce audible sound");

    std::cout << "Monomachine FM+ DYNAMIC passed all checks successfully!\n";
    return 0;
}
