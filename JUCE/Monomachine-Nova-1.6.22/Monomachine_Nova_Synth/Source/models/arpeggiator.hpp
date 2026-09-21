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
class MonomachineArpeggiator {
public:
    enum class Mode : uint8_t { Off, Key, Sid, Add };
    enum class Play : uint8_t { True, Up, Down, Cycl, Rnd, Step, StepChord };
    enum class VelocityMode : uint8_t { Key, Hold, Step, StepKey, StepHold };
    struct Step { bool hold = false; int8_t transpose = 0; uint8_t velocity = 127; };
    struct ArpSettings {
        Mode mode = Mode::Off;
        Play play = Play::Up;
        VelocityMode velocityMode = VelocityMode::Key;
        double speed = 6;
        uint8_t range = 1, noteLength = 64, wrap = 16;
        uint8_t stepPage = 0, stepPageLimit = 1;
        float stepRandom = 0.0f;
        std::array<Step, 128> steps{};
        bool sync = true;
        double milliseconds = 125;
    };
    struct ArpNoteEvent { bool triggered = false; uint8_t note = 60, velocity = 127; bool isNoteOff = false; };

    void reset(double rate = 44100, double bpm = 120) {
        sampleRate = rate; tempo = bpm; down.fill(false); orders.fill(0); latched.fill(0);
        serial = 0; step = 0; clock = gate = 0; sounding = false; chordRemaining = 0; chordIndex = 0; rng = 1234567; lastVelocity = 127;
    }
    void setTempo(double bpm) { tempo = std::clamp(bpm, 30.0, 300.0); }
    void setSettings(const ArpSettings& s) {
        settings = s;
        settings.range = std::clamp<uint8_t>(s.range, 1, 4);
        settings.wrap = std::clamp<uint8_t>(s.wrap, 1, 16);
        settings.stepPage = std::clamp<uint8_t>(s.stepPage, 0, 15);
        settings.stepPageLimit = std::clamp<uint8_t>(s.stepPageLimit, 1, 16);
        settings.stepRandom = std::clamp(s.stepRandom, 0.0f, 1.0f);
        if (settings.stepRandom <= 0.5f) randomPage = settings.stepPage; // 1.6.22: PAGE RND выключен -- играем выбранную страницу, без затяжного нулевого хвоста
    }
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

    ArpNoteEvent poll() {
        ArpNoteEvent ev;
        if (settings.mode == Mode::Off) return ev;
        if (settings.play == Play::StepChord && chordRemaining > 0) {
            const size_t count = makePool();
            if (count == 0) { chordRemaining = 0; return ev; }
            const size_t noteIndex = std::min(chordIndex, count - 1);
            const auto note = pool[noteIndex];
            const int transposed = static_cast<int>(note) + static_cast<int>(chordTranspose);
            ev.note = static_cast<uint8_t>(std::clamp(transposed, 0, 127));
            ev.velocity = velocityFor(chordVelocity, velocity[note]); ev.triggered = true;
            ++chordIndex; --chordRemaining;
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
        clock += std::max(2.0, duration);

        size_t noteIndex = 0;
        uint8_t outputVelocity = 127;
        bool stepHold = false;
        const size_t total = count * settings.range;
        const uint64_t sequenceStep = step++;
        if (settings.play == Play::Step || settings.play == Play::StepChord) stepEcho = 0; else stepEcho = static_cast<uint8_t>(sequenceStep);

        if (settings.play == Play::Step || settings.play == Play::StepChord) {
            const size_t availablePages = 16u - static_cast<size_t>(settings.stepPage);
            const size_t stepCount = std::max<size_t>(1, std::min<size_t>(availablePages, settings.stepPageLimit) * 8u);
            size_t stepIndex = static_cast<size_t>(sequenceStep % stepCount);
            // 1.6.21: PAGE RND -- при каждом новом проходе по паттерну выбирается
            // случайная страница (в пределах stepPageLimit); шаги идут по порядку.
            const bool newPass = (sequenceStep % stepCount) == 0;
            if (newPass && settings.stepRandom > 0.5f) {
                rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
                randomPage = static_cast<uint8_t>(settings.stepPage + (rng % std::max<size_t>(1, std::min<size_t>(availablePages, settings.stepPageLimit))));
            }
            const uint8_t playPage = settings.stepRandom > 0.5f ? randomPage : settings.stepPage;
            stepIndex %= 8; // внутри страницы шаг всегда по порядку
            stepEcho = static_cast<uint8_t>(stepIndex);
            const auto& s = settings.steps[static_cast<size_t>(playPage) * 8 + stepIndex];
            if (s.velocity == 0) return ev; // a silent step still consumes its clock slot
            noteIndex = settings.play == Play::StepChord ? 0 : std::min(total - 1, static_cast<size_t>(sequenceStep % std::max<size_t>(1, total)));
            if (settings.play == Play::StepChord) { chordRemaining = std::min<size_t>(count, 3) - 1; chordIndex = 1; chordTranspose = s.transpose; chordVelocity = s.velocity; }
            const int transposed = static_cast<int>(pool[noteIndex % count]) + static_cast<int>(s.transpose);
            ev.note = static_cast<uint8_t>(std::clamp(transposed, 0, 127));
            outputVelocity = velocityFor(s.velocity, velocity[pool[noteIndex % count]]);
            stepHold = s.hold;
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

        const double normalGate = duration * std::clamp(static_cast<double>(settings.noteLength) / 128.0, 0.01, 0.99);
        gate = std::max(1.0, std::min(duration - 1.0, normalGate * (stepHold ? 1.75 : 1.0)));
        ev.velocity = outputVelocity; ev.triggered = true; sounding = true;
        return ev;
    }

    ArpNoteEvent processBlock(size_t samples) { auto ev = poll(); advance(static_cast<int>(samples)); return ev; }
    // 1.6.14: индекс текущего шага внутри цикла (для STEP RND в редакторе).
    uint8_t stepEcho = 0; uint8_t randomPage = 0; // 1.6.21: страница, выбранная PAGE RND на текущий проход
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
    std::array<bool, 128> down{};
    std::array<uint64_t, 128> orders{}, latched{};
    std::array<uint8_t, 128> velocity{}, pool{};
    uint64_t serial = 0, step = 0; size_t chordRemaining = 0, chordIndex = 0; int8_t chordTranspose = 0; uint8_t chordVelocity = 127; uint32_t rng = 1234567; uint8_t lastVelocity = 127;
    double sampleRate = 44100, tempo = 120, clock = 0, gate = 0;
    bool sounding = false;
};
}
