#include "../dsp/monomachine_bbox.hpp"
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::cout << "Testing Monomachine DPRO-BBOX (Machine 7) & Sample Engine...\n";

    monomachine::MonomachineBBox bbox;
    bbox.reset(44100.0);

    // Verify 10 slots exist and default drum kit is populated
    const auto& slots = bbox.getSlots();
    assert(slots.size() == 10);
    for (size_t i = 0; i < 10; ++i) {
        assert(slots[i].loaded && !slots[i].data.empty());
        std::cout << "Slot " << i << ": " << slots[i].name << " (" << slots[i].data.size() << " samples)\n";
    }

    // Test playback on slot 0 (Kick)
    bbox.setParameters(64, 0, 0, 0, 0, false, false);
    bbox.noteOn(60, 127);

    constexpr size_t kFrames = 256;
    std::vector<float> outL(kFrames, 0.0f);
    std::vector<float> outR(kFrames, 0.0f);

    bbox.processStereo(outL.data(), outR.data(), kFrames);

    float peak = 0.0f;
    for (size_t i = 0; i < kFrames; ++i) {
        peak = std::max(peak, std::abs(outL[i]));
    }
    std::cout << "BBox normal playback peak: " << peak << "\n";
    assert(peak > 0.1f);

    // Test random slot selection
    bbox.setParameters(64, 0, 0, 0, 0, true, true);
    bbox.noteOn(60, 127);
    bbox.processStereo(outL.data(), outR.data(), kFrames);

    // Test custom sample loading into slot 5
    std::vector<float> customSample(1000, 0.8f);
    bool ok = bbox.loadSample(5, "User Snare", customSample.data(), customSample.size());
    assert(ok);
    assert(bbox.getSlots()[5].name == "User Snare");

    std::cout << "Monomachine BBOX and 10-slot sample engine passed all checks!\n";
    return 0;
}
