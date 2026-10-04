// Monomachine Nova 1.9.8 -- opt-in NEW Track Delay candidate.
//
// This is not presented as a bit-exact firmware port.  It takes only the
// bounded conclusions from the recovered Track Delay material: a 16-sample
// control frame, reciprocal delay-time geometry, an observed centred-DSND
// mono-positive / split-LR-negative comb route, and a BASE/WIDTH feedback filter.
// The unresolved host mapping is deliberately supplied by the documented Nova
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
    static constexpr float kMaxSeconds = 4.0f;
    static constexpr float kLegacyMaxSeconds = 1.25f;
    static constexpr int kControlFrame = 16;

    void prepare(double sampleRate) {
        sampleRate_ = sampleRate > 0.0 ? sampleRate : kDspRate;
        const int frames = std::max(8, static_cast<int>(sampleRate_ * kMaxSeconds) + 4);
        left_.assign(static_cast<size_t>(frames), 0.0f);
        right_.assign(static_cast<size_t>(frames), 0.0f);
        size_ = frames;
        maxSamples_ = static_cast<float>(frames - 2);
        legacyMaxSamples_ = std::min(maxSamples_, static_cast<float>(sampleRate_ * kLegacyMaxSeconds));
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
    void setLoopClip(bool enabled) noexcept { loopClipAfterFeedback_=enabled; }

    // Retained raw/table API for direct callers. TrackChain uses processSeconds
    // so DTIM is not passed through a second, reciprocal timing conversion.
    inline void process(float delParam, float feedback, float send, bool negativeComb,
                        float slewSeconds, float inL, float inR,
                        float& outL, float& outR) noexcept {
        if (size_ < 3) { outL = outR = 0.0f; return; }
        if (framePhase_ == 0) {
            const int index = std::clamp(static_cast<int>(delParam * 63.0f / 127.0f + 0.5f), 0, 63);
            scheduledTarget_ = std::clamp(legacyMaxSamples_ * kDelayRecip[static_cast<size_t>(63 - index)],
                                          1.0f, legacyMaxSamples_);
        }
        processScheduled(feedback,send,negativeComb,false,slewSeconds,inL,inR,outL,outR);
    }

    inline void processSeconds(float delaySeconds, float feedback, float send, bool negativeComb,
                               float slewSeconds, float inL, float inR,
                               float& outL, float& outR) noexcept {
        processSeconds(delaySeconds,feedback,send,negativeComb,false,slewSeconds,inL,inR,outL,outR);
    }
    inline void processSeconds(float delaySeconds, float feedback, float send, bool negativeComb,
                               bool invertFeedback, float slewSeconds, float inL, float inR,
                               float& outL, float& outR) noexcept {
        if (size_ < 3) { outL = outR = 0.0f; return; }
        if (framePhase_ == 0) {
            const float finiteSeconds = std::isfinite(delaySeconds) ? delaySeconds : 0.0f;
            scheduledTarget_ = std::clamp(finiteSeconds * static_cast<float>(sampleRate_),
                                          1.0f, maxSamples_);
        }
        processScheduled(feedback,send,negativeComb,invertFeedback,slewSeconds,inL,inR,outL,outR);
    }

private:
    inline float loopWrite(float value) const noexcept {
        if(!std::isfinite(value))return 0.0f;
        return loopClipAfterFeedback_?DelayFeedbackFilter::stabilise(value):value;
    }
    inline void processScheduled(float feedback, float send, bool negativeComb, bool invertFeedback, float slewSeconds,
                                 float inL, float inR, float& outL, float& outR) noexcept {
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

        // The two Korg-35 stages are in this host-candidate recurrence. A
        // DBAS/DWID move shapes both what is heard at the tap and every later
        // repeat; it is not claimed to be a DSP-tail page-word mapping.
        feedbackFilter_.processStereo(tapL, tapR);
        // Do not cap the resolved feedback above unity: TrackChain's DFB
        // controller owns the retained base law, tail/rise and level plateau;
        // this core preserves that resolved value into mode-specific saturation.
        const float safeFeedback = std::max(0.0f, feedback);
        const float safeSend = std::clamp(send, 0.0f, 1.0f);
        // Positive DSND writes one mono positive comb. Negative DSND preserves
        // post-AMP/PAN L/R input lanes and inverts each feedback recurrence;
        // the dormant legacy selector is not consulted.
        const float mono = 0.5f * (inL + inR);
        const bool negativeFeedback=negativeComb!=invertFeedback;
        left_[write_] = loopWrite((negativeComb ? inL : mono) * safeSend + (negativeFeedback ? -tapL : tapL) * safeFeedback);
        right_[write_] = loopWrite((negativeComb ? inR : mono) * safeSend + (negativeFeedback ? -tapR : tapR) * safeFeedback);
        write_ = (write_ + 1) % static_cast<size_t>(size_);

        // Pack-8 P:$0A22/$0AB7 uses independent stage-2 words Y:$414..$417.
        // No P1 FILT ATK/DEC alias is valid here, so preserve L/R return lanes.
        outL = tapL;
        outR = tapR;
    }

    double sampleRate_ = kDspRate;
    std::vector<float> left_, right_;
    size_t write_ = 0;
    int size_ = 0, framePhase_ = 0;
    float maxSamples_ = 1.0f, legacyMaxSamples_ = 1.0f, smoothSamples_ = -1.0f, scheduledTarget_ = 1.0f;
    bool loopClipAfterFeedback_ = false;
    DelayFeedbackFilter feedbackFilter_;
};
} // namespace mnm
} // namespace monomachine
