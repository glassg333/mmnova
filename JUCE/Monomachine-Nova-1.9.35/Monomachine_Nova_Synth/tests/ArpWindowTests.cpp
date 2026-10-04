// Regression coverage for one physical 16-step ARP page. This target is
// deliberately JUCE-free; processor-level SONG tests own page/repeat changes.
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
    settings.pageIndex = 42;
    settings.stepWindowStart = 0;
    settings.stepWindowEnd = Arp::kStepsPerPage - 1;
    arp.setSettings(settings);
    arp.noteOn(60, 100);

    std::set<int> seen;
    for (int tries = 0; tries < 100 && seen.size() < Arp::kStepsPerPage; ++tries) {
        const auto event = arp.poll();
        if (event.triggered && ! event.isNoteOff)
            seen.insert(arp.stepEcho);
        arp.advance(100000);
    }
    require(seen.size() == Arp::kStepsPerPage, "16-step page did not visit all local steps");
    require(arp.playingPage == 42, "selected physical page was not echoed");

    settings.stepWindowStart = 15;
    settings.stepWindowEnd = 15;
    settings.stepRnd = false;
    arp.setSettings(settings);
    require(nextNote(arp), "one-step local window did not generate an event");
    require(arp.stepEcho == 15, "START == END must be a one-step local loop");

    // Build a full-page bag first, then shrink it while STEP RND stays on.
    // This catches stale shuffled indices after a live local trim.
    settings.stepWindowStart = 0;
    settings.stepWindowEnd = 15;
    settings.stepRnd = 1;
    arp.setSettings(settings);
    require(nextNote(arp), "full shuffled page did not generate an event");

    settings.stepWindowStart = 3;
    settings.stepWindowEnd = 11;
    arp.setSettings(settings);
    for (int i = 0; i < 32; ++i) {
        require(nextNote(arp), "shuffled local page window did not generate an event");
        require(arp.stepEcho >= 3 && arp.stepEcho <= 11,
                "STEP RND escaped the active local START..END window");
    }

    // The processor consumes stepCycleFinished, including silent final steps,
    // to execute PAGE RND or SONG REPEAT/page advances exactly at pass end.
    Arp boundary;
    boundary.reset(48000.0, 120.0);
    Arp::ArpSettings boundarySettings;
    boundarySettings.mode=Arp::Mode::Key;
    boundarySettings.play=Arp::Play::Step;
    boundarySettings.speed=0.125;
    boundarySettings.pageIndex=7;
    boundarySettings.stepWindowStart=5;
    boundarySettings.stepWindowEnd=7; // three steps per page pass
    boundarySettings.steps[7].velocity=0; // completion must not require a note
    boundary.setSettings(boundarySettings);
    boundary.noteOn(64, 100);
    bool sawCompletion=false;
    for(int polls=0;polls<16&&!sawCompletion;++polls){
        const auto event=boundary.poll();
        if(event.stepCycleFinished){
            require(boundary.stepEcho==7, "completion must identify the final local step");
            sawCompletion=true;
        }
        boundary.advance(100000);
    }
    require(sawCompletion,"silent final page step did not emit pass completion");

    // Matrix modulation updates the selected page window without recreating
    // the ARP; it must invalidate an old local shuffle bag immediately.
    arp.setStepWindow(12, 13);
    for (int i = 0; i < 16; ++i) {
        require(nextNote(arp), "live local matrix window update did not generate an event");
        require(arp.stepEcho == 12 || arp.stepEcho == 13,
                "live local matrix window update escaped its two-step range");
    }

    // Snapshot refreshes carry ordinary settings every audio block. They must
    // not erase a stable Matrix-selected local window or restart its bag.
    arp.setSettings(settings); // configured base remains 3..11 from settings
    arp.setStepWindow(6, 7);
    arp.setSettings(settings); // emulate next processor snapshot
    for (int i = 0; i < 8; ++i) {
        require(nextNote(arp), "snapshot after a stable Matrix window lost an event");
        require(arp.stepEcho == 6 || arp.stepEcho == 7,
                "snapshot reset a stable Matrix-selected local window");
    }

    std::puts("ARPWINDOW 16-step page, pass completion, local shuffle and modulation PASS");
    return 0;
}
