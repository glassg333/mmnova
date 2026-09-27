// Regression coverage for the live 1..64 ARP window.  This target is purposely
// JUCE-free so it can validate the scheduling model independently of a plugin host.
#include "models/arpeggiator.hpp"

#include <cstdio>
#include <cstdlib>
#include <set>

namespace {
void require(bool condition, const char* message)
{
    if (! condition) {
        std::fprintf(stderr, "ARPWINDOW FAIL: %s\n", message);
        std::exit(1);
    }
}

bool nextNote(monomachine::MonomachineArpeggiator& arp)
{
    for (int tries = 0; tries < 4; ++tries) {
        const auto event = arp.poll();
        if (event.triggered && ! event.isNoteOff)
            return true;
        arp.advance(100000);
    }
    return false;
}
}

int main()
{
    using Arp = monomachine::MonomachineArpeggiator;
    Arp arp;
    arp.reset(48000.0, 120.0);

    Arp::ArpSettings settings;
    settings.mode = Arp::Mode::Key;
    settings.play = Arp::Play::Step;
    settings.speed = 0.125;
    settings.noteLength = 127;
    settings.stepWindowStart = 0;
    settings.stepWindowEnd = 63;
    arp.setSettings(settings);
    arp.noteOn(60, 100);

    std::set<int> seen;
    for (int tries = 0; tries < 300 && seen.size() < 64; ++tries) {
        const auto event = arp.poll();
        if (event.triggered && ! event.isNoteOff)
            seen.insert(arp.stepEcho);
        arp.advance(100000);
    }
    require(seen.size() == 64, "64-step window did not visit all 64 absolute steps");

    settings.stepWindowStart = 37;
    settings.stepWindowEnd = 37;
    settings.stepRnd = false;
    arp.setSettings(settings);
    require(nextNote(arp), "one-step window did not generate an event");
    require(arp.stepEcho == 37, "START == END must be a one-step loop");

    // Build a full-window bag first, then shrink it while STEP RND stays on.
    // This specifically catches stale shuffled indices after a live trim.
    settings.stepWindowStart = 0;
    settings.stepWindowEnd = 63;
    settings.stepRnd = true;
    arp.setSettings(settings);
    require(nextNote(arp), "full shuffled window did not generate an event");

    settings.stepWindowStart = 11;
    settings.stepWindowEnd = 26;
    arp.setSettings(settings);
    for (int i = 0; i < 64; ++i) {
        require(nextNote(arp), "shuffled window did not generate an event");
        require(arp.stepEcho >= 11 && arp.stepEcho <= 26,
                "STEP RND escaped the active START..END window");
    }

    // Matrix modulation updates the window between snapshots through this
    // lightweight path; it must invalidate an old shuffle bag immediately.
    arp.setStepWindow(29, 30);
    for (int i = 0; i < 16; ++i) {
        require(nextNote(arp), "live matrix window update did not generate an event");
        require(arp.stepEcho == 29 || arp.stepEcho == 30,
                "live matrix window update escaped its two-step range");
    }

    std::puts("ARPWINDOW 1..64 scheduling, one-step trim, live window modulation, shuffled-window PASS");
    return 0;
}
