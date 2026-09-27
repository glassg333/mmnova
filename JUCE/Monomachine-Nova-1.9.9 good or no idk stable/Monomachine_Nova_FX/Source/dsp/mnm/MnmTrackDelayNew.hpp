// Monomachine Nova 1.9.8 -- opt-in NEW Track Delay candidate.
//
// This is not presented as a bit-exact firmware port.  It takes only the
// bounded conclusions from the recovered Track Delay material: a 16-sample
// control frame, reciprocal delay-time geometry, independent stereo loops,
// optional cross feedback, and a BASE/WIDTH multi-stage feedback filter.  The
// unresolved host mapping is deliberately supplied by the documented Nova
// DBAS/DWID mapping in MnmDelayFeedbackFilter.hpp.
#pragma once

#include "MnmDelayFeedbackFilter.hpp"
#include "MnmKernel.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace monomachine {
namespace mnm {

class TrackDelayNewCore final {
public:
    static constexpr float kMaxSeconds = 1.25f;
    static constexpr int kControlFrame = 16;

    void prepare(double sampleRate) {
        sampleRate_ = sampleRate > 0.0 ? sampleRate : kDspRate;
        const int frames = std::max(8, static_cast<int>(sampleRate_ * kMaxSeconds) + 4);
        left_.assign(static_cast<size_t>(frames), 0.0f);
        right_.assign(static_cast<size_t>(frames), 0.0f);
        size_ = frames;
        maxSamples_ = static_cast<float>(frames - 2);
        feedbackFilter_.prepare(sampleRate_);
        reset();
    }

    void reset() noexcept {
        std::fill(left_.begin(), left_.end(), 0.0f);
        std::fill(right_.begin(), right_.end(), 0.0f);
        write_ = 0;
        framePhase_ = 0;
        smoothSamples_ = -1.0f;
        scheduledTarget_ = 1.0f;
        feedbackFilter_.reset();
    }

    void setFeedbackFilterParameters(float base, float width, float baseQ, float widthQ) noexcept {
        feedbackFilter_.setParameters(base, width, baseQ, widthQ);
    }

    inline void process(float delParam, float feedback, float send, bool pingPong, int ppMode,
                        float slewSeconds, float inL, float inR,
                        float& outL, float& outR) noexcept {
        if (size_ < 3) { outL = outR = 0.0f; return; }

        // The original Track Delay evidence is frame scheduled.  Capture the
        // reciprocal target once per 16 samples while retaining audio-rate
        // interpolation and a host-rate-safe time slew in between frames.
        if (framePhase_ == 0) {
            const int index = std::clamp(static_cast<int>(delParam * 63.0f / 127.0f + 0.5f), 0, 63);
            scheduledTarget_ = std::clamp(maxSamples_ * kDelayRecip[static_cast<size_t>(63 - index)],
                                          1.0f, maxSamples_);
        }
        framePhase_ = (framePhase_ + 1) & (kControlFrame - 1);

        if (smoothSamples_ < 0.0f) smoothSamples_ = scheduledTarget_;
        const float safeSlew = std::max(0.012f, slewSeconds);
        const float slewAlpha = 1.0f - std::exp(-1.0f /
            (static_cast<float>(sampleRate_) * std::max(0.0005f, safeSlew)));
        smoothSamples_ += (scheduledTarget_ - smoothSamples_) * slewAlpha;
        if (std::abs(scheduledTarget_ - smoothSamples_) < 0.01f) smoothSamples_ = scheduledTarget_;

        float readPos = static_cast<float>(write_) - smoothSamples_;
        while (readPos < 0.0f) readPos += static_cast<float>(size_);
        while (readPos >= static_cast<float>(size_)) readPos -= static_cast<float>(size_);
        const size_t i0 = static_cast<size_t>(readPos) % static_cast<size_t>(size_);
        const size_t i1 = (i0 + 1) % static_cast<size_t>(size_);
        const float fraction = readPos - std::floor(readPos);
        float tapL = left_[i0] + fraction * (left_[i1] - left_[i0]);
        float tapR = right_[i0] + fraction * (right_[i1] - right_[i0]);

        // The two Korg-35 stages are in the recurrence itself.  This means a
        // DBAS/DWID move shapes both what is heard at the tap and every later
        // repeat, instead of being a cosmetic post-delay EQ.
        feedbackFilter_.processStereo(tapL, tapR);
        const float feedbackL = pingPong ? tapR : tapL;
        const float feedbackR = pingPong ? tapL : tapR;
        const float safeFeedback = std::clamp(feedback, 0.0f, 0.95f);
        const float safeSend = std::clamp(send, 0.0f, 1.0f);
        left_[write_] = DelayFeedbackFilter::stabilise((pingPong ? 0.5f * (inL + inR) : inL) * safeSend
                                                        + feedbackL * safeFeedback);
        right_[write_] = DelayFeedbackFilter::stabilise((pingPong ? 0.0f : inR) * safeSend
                                                         + feedbackR * safeFeedback);
        write_ = (write_ + 1) % static_cast<size_t>(size_);

        if (pingPong && ppMode == 1) {
            outL = 0.65f * tapL + 0.35f * tapR;
            outR = 0.65f * tapR + 0.35f * tapL;
        } else {
            outL = tapL;
            outR = tapR;
        }
    }

private:
    double sampleRate_ = kDspRate;
    std::vector<float> left_, right_;
    size_t write_ = 0;
    int size_ = 0, framePhase_ = 0;
    float maxSamples_ = 1.0f, smoothSamples_ = -1.0f, scheduledTarget_ = 1.0f;
    DelayFeedbackFilter feedbackFilter_;
};

} // namespace mnm
} // namespace monomachine
