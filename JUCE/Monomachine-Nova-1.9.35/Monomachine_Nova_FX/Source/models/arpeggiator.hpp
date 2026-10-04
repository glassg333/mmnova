#pragma once
#include <array>
#include <cstddef>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdint>

namespace monomachine {

// Allocation-free, note-synchronised native arp. Not firmware emulation.
// The host selects one sixteen-step ARP page at a time; SONG changes that
// selected page only after its complete local START..END pass has finished.
class MonomachineArpeggiator {
public:
    enum class Mode : uint8_t { Off, Key, Sid, Add };
    enum class Play : uint8_t { True, Up, Down, Cycl, Rnd, Step, StepChord };
    enum class VelocityMode : uint8_t { Key, Hold, Step, StepKey, StepHold };
    static constexpr uint8_t kStepsPerPage = 16;
    struct Step { bool hold = false; int8_t transpose = 0; uint8_t velocity = 127; };
    struct ArpSettings {
        Mode mode = Mode::Off;
        Play play = Play::Up;
        VelocityMode velocityMode = VelocityMode::Key;
        double speed = 6;
        uint8_t range = 1, noteLength = 64, wrap = 16;
        // pageIndex is the selected ARP/P-LOCK page. The active sequencer
        // works only in this local inclusive 0..15 window.
        uint8_t pageIndex = 0;
        uint8_t stepPage = 0, stepPageLimit = 1; // retained 64-page host mirror
        uint8_t stepWindowStart = 0, stepWindowEnd = kStepsPerPage - 1;
        float stepRandom = 0.0f; // PAGE RND is committed by the processor at a cycle boundary.
        // 0=OFF, 1=ALL SYNC (the historic linked bag), 2=ALL UNSYNC.  The
        // arpeggiator owns the normal-step bag; the processor owns P-LOCK's
        // independent bag for mode 2.
        uint8_t stepRnd = 0;
        bool autoSwap = false; // retained UI-only legacy flag; window traversal is continuous
        std::array<Step, kStepsPerPage> steps{};
        bool sync = true;
        double milliseconds = 125;
    };
    struct ArpNoteEvent {
        bool triggered = false;
        uint8_t note = 60, velocity = 127;
        bool isNoteOff = false;
        // True only on the first step after a completed pass; retained for
        // legacy PAGE RND users. stepCycleFinished marks the final step of a
        // pass and lets SONG switch pages before the following trigger.
        bool stepCycleWrapped = false;
        bool stepCycleFinished = false;
    };

    void reset(double rate = 44100, double bpm = 120) {
        sampleRate = rate; tempo = bpm; down.fill(false); orders.fill(0); latched.fill(0);
        serial = 0; step = 0; clock = gate = 0; sounding = false; chordRemaining = 0; chordIndex = 0; chordPassFinished=false; rng = 1234567; lastVelocity = 127;
        bagValid = false; configuredWindowKnown=false; modRateOct = 0.0; modGateAdd = 0.0f; // 1.7.1
    }
    void setTempo(double bpm) { tempo = std::clamp(bpm, 30.0, 300.0); }
    void setSettings(const ArpSettings& s) {
        // The processor refreshes ordinary APVTS settings once per audio block,
        // while Matrix may keep a modulated local window between those refreshes.
        // Track configured (base) bounds independently so that a stable Matrix
        // offset cannot rebuild a STEP RND bag every block.
        const uint8_t previousCurrentStart=settings.stepWindowStart,previousCurrentEnd=settings.stepWindowEnd;
        const uint8_t priorStepRnd=settings.stepRnd;
        settings = s;
        settings.range = std::clamp<uint8_t>(s.range, 1, 4);
        settings.wrap = std::clamp<uint8_t>(s.wrap, 1, 16);
        settings.stepPage = std::clamp<uint8_t>(s.stepPage, 0, 63);
        settings.stepPageLimit = std::clamp<uint8_t>(s.stepPageLimit, 1, 64);
        settings.pageIndex = std::min<uint8_t>(63,s.pageIndex);
        const uint8_t configuredStart=std::clamp<uint8_t>(s.stepWindowStart, 0, kStepsPerPage - 1);
        const uint8_t configuredEnd=std::clamp<uint8_t>(s.stepWindowEnd, configuredStart, kStepsPerPage - 1);
        const bool configuredWindowChanged=!configuredWindowKnown||configuredWindowStart!=configuredStart||configuredWindowEnd!=configuredEnd;
        configuredWindowKnown=true;configuredWindowStart=configuredStart;configuredWindowEnd=configuredEnd;
        if(configuredWindowChanged){settings.stepWindowStart=configuredStart;settings.stepWindowEnd=configuredEnd;}
        else {settings.stepWindowStart=previousCurrentStart;settings.stepWindowEnd=previousCurrentEnd;}
        settings.stepRandom = std::clamp(s.stepRandom, 0.0f, 1.0f);
        const uint8_t nextStepRnd=std::min<uint8_t>(static_cast<uint8_t>(2),s.stepRnd);
        const bool randomModeChanged=priorStepRnd!=nextStepRnd;
        settings.stepRnd=nextStepRnd;
        if (settings.stepRnd==0 || configuredWindowChanged || randomModeChanged) bagValid=false;
    }
    // Matrix modulation changes only the current local page window. Keeping
    // this separate avoids resetting note state or the STEP RND bag on every
    // audio mini-block.
    void setStepWindow(uint8_t start,uint8_t end) noexcept {
        start=std::clamp<uint8_t>(start,0,kStepsPerPage-1);
        end=std::clamp<uint8_t>(end,start,kStepsPerPage-1);
        if(settings.stepWindowStart!=start||settings.stepWindowEnd!=end)bagValid=false;
        settings.stepWindowStart=start;settings.stepWindowEnd=end;
    }
    void restartStepPass() noexcept { step=0; bagValid=false; chordRemaining=0; chordIndex=0; chordPassFinished=false; }
    void noteOn(uint8_t note, uint8_t vel = 127) {
        if (note > 127) return;
        if (settings.mode == Mode::Sid && std::none_of(down.begin(), down.end(), [](bool v) { return v; })) latched.fill(0);
        down[note] = true; orders[note] = ++serial; latched[note] = serial; velocity[note] = vel; lastVelocity = vel;
    }
    void noteOff(uint8_t note) {
        if (note > 127) return;
        down[note] = false; orders[note] = 0;
        if (settings.mode == Mode::Add && std::none_of(down.begin(), down.end(), [](bool x) { return x; })) latched.fill(0);
    }
    int samplesUntilEvent() const {
        if (settings.mode == Mode::Off) return std::numeric_limits<int>::max();
        if (poolSize() == 0) return sounding ? 0 : std::numeric_limits<int>::max();
        return std::max(0, static_cast<int>(std::ceil(sounding ? std::min(clock, gate) : clock)));
    }
    void advance(int samples) { clock -= samples; if (sounding) gate -= samples; }
    // 1.7.1: внешняя модуляция ARP RATE (октавы, +-2) и ARP GATE (прибавка к длине 1..127) -- цели 64/65 матрицы и P-LOCK
    void setMod(double rateOct, float gateAdd) { modRateOct = std::clamp(rateOct, -2.0, 2.0); modGateAdd = std::clamp(gateAdd, -63.0f, 63.0f); }

    ArpNoteEvent poll() {
        ArpNoteEvent ev;
        if (settings.mode == Mode::Off) return ev;
        if (settings.play == Play::StepChord && chordRemaining > 0) {
            const size_t count = makePool();
            if (count == 0) { chordRemaining = 0; chordPassFinished=false; return ev; }
            const size_t noteIndex = std::min(chordIndex, count - 1);
            const auto note = pool[noteIndex];
            const int transposed = static_cast<int>(note) + static_cast<int>(chordTranspose);
            ev.note = static_cast<uint8_t>(std::clamp(transposed, 0, 127));
            ev.velocity = velocityFor(chordVelocity, velocity[note]); ev.triggered = true;
            ++chordIndex; --chordRemaining;
            if(chordRemaining==0&&chordPassFinished){ev.stepCycleFinished=true;chordPassFinished=false;}
            return ev;
        }
        const auto count = makePool();
        if (sounding && (gate <= 0 || count == 0)) {
            sounding = false; ev.triggered = true; ev.isNoteOff = true;
            if (count == 0) { clock = 0; step = 0; }
            return ev;
        }
        if (count == 0 || clock > 0) return ev;

        const double duration = settings.sync
            ? sampleRate * 60.0 / tempo / 24.0 * std::max(0.125, settings.speed)
            : sampleRate * std::clamp(settings.milliseconds, 5.0, 2000.0) / 1000.0;
        clock += std::max(2.0, duration * std::pow(2.0, -modRateOct)); // 1.7.1: модуляция RATE (октавы)

        size_t noteIndex = 0;
        uint8_t outputVelocity = 127;
        bool stepHold = false;
        const size_t total = count * settings.range;
        const uint64_t sequenceStep = step++;
        if (settings.play == Play::Step || settings.play == Play::StepChord) stepEcho = 0; else stepEcho = static_cast<uint8_t>(sequenceStep);

        if (settings.play == Play::Step || settings.play == Play::StepChord) {
            const size_t windowStart=static_cast<size_t>(settings.stepWindowStart);
            const size_t stepCount=static_cast<size_t>(settings.stepWindowEnd-settings.stepWindowStart)+1u;
            size_t localIndex=static_cast<size_t>(sequenceStep%stepCount);
            const bool newPass=localIndex==0;
            const bool finishesPass=localIndex+1==stepCount;
            // This is emitted even for a silent first step, so PAGE RND does
            // not depend on whether that step happened to have velocity zero.
            ev.stepCycleWrapped=newPass&&sequenceStep!=0;
            if(settings.stepRnd!=0){
                // The bag covers every active local step of the selected 16-step page.
                if(newPass||!bagValid){
                    for(size_t i=0;i<stepCount;++i)bag[i]=static_cast<uint8_t>(i);
                    for(size_t i=stepCount;i>1;--i){
                        rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;
                        const size_t q=static_cast<size_t>(rng%static_cast<uint32_t>(i));
                        const uint8_t tmp=bag[i-1];bag[i-1]=bag[q];bag[q]=tmp;
                    }
                    bagValid=true;
                }
                localIndex=bag[localIndex];
            }
            const size_t stepIndex=windowStart+localIndex;
            stepEcho=static_cast<uint8_t>(stepIndex);
            playingPage=settings.pageIndex;
            const auto& s=settings.steps[stepIndex];
            if(s.velocity==0){ev.stepCycleFinished=finishesPass;return ev;} // silent step still completes its slot
            noteIndex=settings.play==Play::StepChord?0:std::min(total-1,static_cast<size_t>(sequenceStep%std::max<size_t>(1,total)));
            if(settings.play==Play::StepChord){
                chordRemaining=std::min<size_t>(count,3)-1;chordIndex=1;chordTranspose=s.transpose;chordVelocity=s.velocity;
                if(chordRemaining==0)ev.stepCycleFinished=finishesPass;
                else chordPassFinished=finishesPass;
            }else ev.stepCycleFinished=finishesPass;
            const int transposed=static_cast<int>(pool[noteIndex%count])+static_cast<int>(s.transpose);
            ev.note=static_cast<uint8_t>(std::clamp(transposed,0,127));
            outputVelocity=velocityFor(s.velocity,velocity[pool[noteIndex%count]]);
            stepHold=s.hold;
        } else {
            const size_t cycle = std::max<size_t>(1, std::min<size_t>(total, settings.wrap));
            size_t index = 0;
            if (settings.play == Play::Rnd) { rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5; index = rng % cycle; }
            else if (settings.play == Play::Cycl && cycle > 1) { const size_t x = sequenceStep % (2 * cycle - 2); index = x < cycle ? x : 2 * cycle - 2 - x; }
            else index = static_cast<size_t>(sequenceStep % cycle);
            const auto note = pool[index % count];
            ev.note = static_cast<uint8_t>(std::min(127, static_cast<int>(note + 12 * (index / count))));
            outputVelocity = velocity[note];
        }

        const double normalGate = duration * std::clamp((static_cast<double>(settings.noteLength) + modGateAdd) / 128.0, 0.01, 0.99); // 1.7.1: модуляция GATE
        // 1.6.23: HOLD -- нота держится ДО СЛЕДУЮЩЕГО шага (по мануалу Monomachine
        // HOLD избавляет от NOTE OFF: нота играет, пока не придёт новый триггер).
        gate = std::max(1.0, stepHold ? duration - 1.0 : std::min(duration - 1.0, normalGate));
        ev.velocity = outputVelocity; ev.triggered = true; sounding = true;
        return ev;
    }

    ArpNoteEvent processBlock(size_t samples) { auto ev = poll(); advance(static_cast<int>(samples)); return ev; }
    // Local active-step index (0..15) and selected page (0..63).
    uint8_t stepEcho = 0; uint8_t playingPage = 0;
    static std::vector<std::string> getArpDropdownHierarchy() { return { "OFF", "KEY", "SID (latch)", "ADD", "TRUE", "UP", "DOWN", "CYCL", "RND", "STEP", "STEP CHORD" }; }

private:
    uint8_t velocityFor(uint8_t stepVelocity, uint8_t keyVelocity) const noexcept {
        switch (settings.velocityMode) {
            case VelocityMode::Hold: return lastVelocity;
            case VelocityMode::Step: return stepVelocity;
            case VelocityMode::StepKey: return static_cast<uint8_t>((static_cast<int>(stepVelocity) + keyVelocity) / 2);
            case VelocityMode::StepHold: return static_cast<uint8_t>((static_cast<int>(stepVelocity) + lastVelocity) / 2);
            case VelocityMode::Key: default: return keyVelocity;
        }
    }
    bool useLatch() const { return settings.mode == Mode::Sid || settings.mode == Mode::Add; }
    size_t poolSize() const { size_t n = 0; for (size_t i = 0; i < 128; ++i) if ((useLatch() ? latched[i] : orders[i]) != 0) ++n; return n; }
    size_t makePool() {
        size_t n = 0; for (size_t i = 0; i < 128; ++i) if ((useLatch() ? latched[i] : orders[i]) != 0) pool[n++] = static_cast<uint8_t>(i);
        if (settings.play == Play::Down) std::reverse(pool.begin(), pool.begin() + static_cast<std::ptrdiff_t>(n));
        if (settings.play == Play::True || settings.play == Play::Step || settings.play == Play::StepChord)
            std::sort(pool.begin(), pool.begin() + static_cast<std::ptrdiff_t>(n), [this](uint8_t a, uint8_t b) { return (useLatch() ? latched[a] : orders[a]) < (useLatch() ? latched[b] : orders[b]); });
        return n;
    }
    ArpSettings settings;
    std::array<uint8_t, kStepsPerPage> bag{}; bool bagValid = false;
    uint8_t configuredWindowStart=0,configuredWindowEnd=kStepsPerPage-1;bool configuredWindowKnown=false;
    double modRateOct = 0.0; float modGateAdd = 0.0f; // 1.7.1
    std::array<bool, 128> down{};
    std::array<uint64_t, 128> orders{}, latched{};
    std::array<uint8_t, 128> velocity{}, pool{};
    uint64_t serial = 0, step = 0; size_t chordRemaining = 0, chordIndex = 0; bool chordPassFinished=false; int8_t chordTranspose = 0; uint8_t chordVelocity = 127; uint32_t rng = 1234567; uint8_t lastVelocity = 127;
    double sampleRate = 44100, tempo = 120, clock = 0, gate = 0;
    bool sounding = false;
};
}
