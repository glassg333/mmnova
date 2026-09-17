#include "../models/arpeggiator.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "Testing Monomachine Hardware Arpeggiator...\n";

    monomachine::MonomachineArpeggiator arp;
    arp.reset(44100.0, 120.0);

    monomachine::MonomachineArpeggiator::ArpSettings s;
    s.mode = monomachine::MonomachineArpeggiator::Mode::Key;
    s.play = monomachine::MonomachineArpeggiator::Play::Up;
    s.range = 2; // 2 octaves
    s.speed = 6; // 16th notes
    arp.setSettings(s);

    // Hold a C Major triad: C4(60), E4(64), G4(67)
    arp.noteOn(60);
    arp.noteOn(64);
    arp.noteOn(67);

    // Process blocks and track triggered notes
    std::vector<uint8_t> triggeredNotes;
    for (int block = 0; block < 100; ++block) {
        auto ev = arp.processBlock(128);
        if (ev.triggered) {
            triggeredNotes.push_back(ev.note);
            std::cout << "Arp Note: " << (int)ev.note << "\n";
        }
    }

    assert(!triggeredNotes.empty() && "Arpeggiator should trigger notes");
    std::cout << "Total arp notes triggered: " << triggeredNotes.size() << "\n";

    auto hierarchy = monomachine::MonomachineArpeggiator::getArpDropdownHierarchy();
    assert(hierarchy.size() >= 15);
    std::cout << "Arp menu hierarchy validated (" << hierarchy.size() << " items).\n";

    std::cout << "Arpeggiator tests passed!\n";
    return 0;
}
