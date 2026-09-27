#pragma once
#include <array>
#include <algorithm>
#include <cmath>

namespace nova {

// One route is active in 1.8.4. The fixed line array is deliberate: a later
// cross-feedback or waveguide graph can add routes/matrix terms without
// replacing the one-sample timing model below.
struct FeedbackRouteState {
    bool on = false;
    int sendZone = 0;
    int returnZone = 0;
    float sendGain = 1.0f;
    float feedbackGain = 0.0f;
    float returnGain = 1.0f;
    float hpPole = 0.999f;
    bool clipEnabled = true; // explicit FB clipper; OFF means no hidden limiter either
};

class FeedbackNetwork {
public:
    static constexpr int kMaxLines = 4;
    using Routes = std::array<FeedbackRouteState, kMaxLines>;

    void prepare(double sampleRate) noexcept {
        sr = sampleRate > 0.0 ? sampleRate : 44100.0;
        clear();
    }

    void clear() noexcept {
        for (auto& line : lines)
            line.clear();
        routes = {};
        active = false;
    }

    bool isActive() const noexcept { return active; }

    // Called once for every rendered sample while a route is on. It snapshots
    // the previous delay sample before either SEND or RETURN is visited, so
    // point order cannot accidentally create a zero-sample feedback path.
    void beginSample(const Routes& next) noexcept {
        routes = next;
        active = false;
        for (int i = 0; i < kMaxLines; ++i) {
            if (!routes[static_cast<size_t>(i)].on)
                continue;
            active = true;
            lines[static_cast<size_t>(i)].beginSample();
        }
    }

    // RETURN is deliberately before a bridge's user FX-slots; SEND is after
    // them. This lets a bridge itself sit inside the loop while keeping the
    // original slot ordering unchanged.
    void returnAt(int zone, float* l, float* r, int n) noexcept {
        if (!active || n <= 0)
            return;
        for (int i = 0; i < kMaxLines; ++i) {
            const auto& route = routes[static_cast<size_t>(i)];
            if (route.on && route.returnZone == zone)
                lines[static_cast<size_t>(i)].inject(l, r, n, route.returnGain);
        }
    }

    void sendAt(int zone, const float* l, const float* r, int n) noexcept {
        if (!active || n <= 0)
            return;
        for (int i = 0; i < kMaxLines; ++i) {
            const auto& route = routes[static_cast<size_t>(i)];
            if (route.on && route.sendZone == zone)
                lines[static_cast<size_t>(i)].capture(l, r, n);
        }
    }

    // Commits all lines only after the whole sample has crossed the signal
    // graph. With an active route the processor renders one sample at a time,
    // which makes this exactly a one-sample feedback delay, not a block delay.
    void endSample() noexcept {
        if (!active)
            return;
        for (int i = 0; i < kMaxLines; ++i) {
            const auto& route = routes[static_cast<size_t>(i)];
            if (route.on)
                lines[static_cast<size_t>(i)].commit(route);
        }
    }

private:
    struct Line {
        float delayL = 0.0f, delayR = 0.0f;
        float returnedL = 0.0f, returnedR = 0.0f;
        float capturedL = 0.0f, capturedR = 0.0f;
        float hpInputL = 0.0f, hpInputR = 0.0f;
        float hpOutputL = 0.0f, hpOutputR = 0.0f;
        bool captured = false;

        void clear() noexcept {
            delayL = delayR = returnedL = returnedR = 0.0f;
            capturedL = capturedR = hpInputL = hpInputR = 0.0f;
            hpOutputL = hpOutputR = 0.0f;
            captured = false;
        }

        void beginSample() noexcept {
            returnedL = delayL;
            returnedR = delayR;
            capturedL = capturedR = 0.0f;
            captured = false;
        }

        void inject(float* l, float* r, int n, float gain) noexcept {
            // Feedback is rendered samplewise by the caller; keeping this
            // small loop makes the class independently testable as well.
            for (int i = 0; i < n; ++i) {
                l[i] += returnedL * gain;
                r[i] += returnedR * gain;
            }
        }

        void capture(const float* l, const float* r, int n) noexcept {
            // v1 has one SEND point per line. If a future graph intentionally
            // exposes more, the last visited point is deterministic.
            capturedL = l[n - 1];
            capturedR = r[n - 1];
            captured = true;
        }

        static float clean(float value) noexcept {
            return std::isfinite(value) ? value : 0.0f;
        }

        static float hardClip(float value) noexcept {
            // An explicit clipper, not an automatic limiter: no gain rider,
            // no envelope follower and no invisible makeup gain.
            return std::clamp(value, -1.0f, 1.0f);
        }

        float highPass(float input, float& previousInput, float& previousOutput, float pole) noexcept {
            input = clean(input);
            const float output = input - previousInput + pole * previousOutput;
            previousInput = input;
            previousOutput = clean(output);
            return previousOutput;
        }

        void commit(const FeedbackRouteState& route) noexcept {
            const float sourceL = captured ? capturedL : 0.0f;
            const float sourceR = captured ? capturedR : 0.0f;
            const float inputL = sourceL * route.sendGain + returnedL * route.feedbackGain;
            const float inputR = sourceR * route.sendGain + returnedR * route.feedbackGain;
            const float filteredL = highPass(inputL, hpInputL, hpOutputL, route.hpPole);
            const float filteredR = highPass(inputR, hpInputR, hpOutputR, route.hpPole);
            // The clipper is deliberately exposed to the user. With it OFF the
            // recurrence is unbounded apart from the manual gains/DC HP; there
            // is no automatic limiter or concealed gain rider.
            delayL = route.clipEnabled ? hardClip(filteredL) : filteredL;
            delayR = route.clipEnabled ? hardClip(filteredR) : filteredR;
        }
    };

    double sr = 44100.0;
    Routes routes{};
    std::array<Line, kMaxLines> lines{};
    bool active = false;
};

} // namespace nova
