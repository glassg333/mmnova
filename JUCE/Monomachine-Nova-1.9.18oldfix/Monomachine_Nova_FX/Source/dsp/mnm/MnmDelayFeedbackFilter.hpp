// Monomachine Nova 1.9.8 -- practical Track Delay feedback filter bridge.
//
// The recovered Track Delay material establishes a BASE/WIDTH-controlled,
// multi-stage filter inside the delay feedback path, but does not prove the
// original host-side DBAS/DWID parameter routing.  This is therefore an
// intentionally documented candidate mapping:
//
//   DBAS  = lower feedback edge (Korg-35 high-pass)
//   DWID  = upper edge width (Korg-35 low-pass at DBAS + DWID)
//   DBAS Q / DWID Q = optional resonance settings, default 0
//
// It is used by the native MNM delay and the opt-in NEW Track Delay candidate.
// Default DBAS=0, DWID=127 and both Q values zero is a strict bypass, so the
// existing MNM path remains untouched until a feedback-filter control is used.
#pragma once

#include "../hybrid_private/HybridDSP.hpp"

#include <algorithm>
#include <cmath>

namespace monomachine {
namespace mnm {

class DelayFeedbackFilter final {
public:
    void prepare(double sampleRate) noexcept {
        sampleRate_ = sampleRate > 0.0 ? sampleRate : 44100.0;
        lower_.prepare(sampleRate_);
        upper_.prepare(sampleRate_);
        configured_ = false;
        updateControls();
    }

    void reset() noexcept {
        lower_.reset();
        upper_.reset();
        // reset() clears audio memory but deliberately keeps the selected
        // controls.  configure() below recreates those coefficients without
        // making a control callback reset live delay state.
        configured_ = false;
        updateControls();
    }

    void setParameters(float base, float width, float baseQ, float widthQ) noexcept {
        const float nextBase = clampRaw(base);
        const float nextWidth = clampRaw(width);
        const float nextBaseQ = clampRaw(baseQ);
        const float nextWidthQ = clampRaw(widthQ);
        if (std::abs(nextBase - base_) < 1.0e-6f
            && std::abs(nextWidth - width_) < 1.0e-6f
            && std::abs(nextBaseQ - baseQ_) < 1.0e-6f
            && std::abs(nextWidthQ - widthQ_) < 1.0e-6f)
            return;
        base_ = nextBase;
        width_ = nextWidth;
        baseQ_ = nextBaseQ;
        widthQ_ = nextWidthQ;
        updateControls();
    }

    bool active() const noexcept { return active_; }

    // The Korg stages keep independent L/R state.  Processing the left channel
    // first preserves their one-control-frame advancement rule.  The final
    // safety curve is deliberately inactive in normal musical range and keeps
    // high-Q/high-feedback combinations finite instead of poisoning a delay
    // line with NaN/Inf.
    inline void processStereo(float& left, float& right) noexcept {
        if (!active_) return;
        left = upper_.process(0, lower_.process(0, left));
        right = upper_.process(1, lower_.process(1, right));
        left = stabilise(left);
        right = stabilise(right);
    }

    static inline float stabilise(float value) noexcept {
        if (!std::isfinite(value)) return 0.0f;
        constexpr float knee = 4.0f;
        const float magnitude = std::abs(value);
        if (magnitude <= knee) return value;
        // Continuous soft ceiling below +/-5.  At ordinary levels this is an
        // exact no-op; it is only the final protection for resonant feedback.
        const float folded = knee + (magnitude - knee) / (1.0f + magnitude - knee);
        return std::copysign(folded, value);
    }

private:
    static float clampRaw(float value) noexcept {
        return std::clamp(std::isfinite(value) ? value : 0.0f, 0.0f, 127.0f);
    }

    float cutoffFromRaw(float raw) const noexcept {
        // Same monotonic 20 Hz..20 kHz control law used by the retained OLD
        // filter.  The upper limit also honours the current host Nyquist.
        const float norm = clampRaw(raw) / 127.0f;
        const float hz = 20.0f * std::pow(1000.0f, norm);
        return std::clamp(hz, 20.0f,
                          std::min(20000.0f, static_cast<float>(sampleRate_ * 0.45)));
    }

    static float resonanceFromRaw(float raw) noexcept {
        // The imported Korg core accepts 0..1.  Keep the optional Q range
        // deliberately below self-oscillation; the feedback delay still has
        // an explicit final stabiliser for hot external input.
        const float n = clampRaw(raw) / 127.0f;
        return 0.74f * n * n;
    }

    void updateControls() noexcept {
        active_ = base_ > 0.5f || width_ < 126.5f || baseQ_ > 0.5f || widthQ_ > 0.5f;
        if (!active_) return;
        const float hpCutoff = cutoffFromRaw(base_);
        const float lpCutoff = cutoffFromRaw(std::min(127.0f, base_ + width_));
        // Korg35Stereo mode 2 = HP and mode 1 = LP.
        lower_.configure(2, hpCutoff, resonanceFromRaw(baseQ_));
        upper_.configure(1, lpCutoff, resonanceFromRaw(widthQ_));
        configured_ = true;
    }

    double sampleRate_ = 44100.0;
    float base_ = 0.0f, width_ = 127.0f, baseQ_ = 0.0f, widthQ_ = 0.0f;
    bool active_ = false, configured_ = false;
    nova::hybrid_private::Korg35Stereo lower_, upper_;
};

} // namespace mnm
} // namespace monomachine
