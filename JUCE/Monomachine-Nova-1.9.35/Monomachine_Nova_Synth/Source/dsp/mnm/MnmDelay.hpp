// Per-track Monomachine delay approximation -- not a separate FX-DLY machine
//
// Recovered geometry and parameter law from P:$147B38-$147D00
// (listings: memory_images/dsp1_machines_fx.txt):
//
//   * buffer: X:$114000, 2048 words per channel, second channel at +$800
//   * delay time: DEL parameter is turned into a read rate through the reciprocal
//     table X:$144C49 (values 1/1, 1/2, 1/3 ... 1/64)
//   * read pointer: fractional read with linear interpolation
//   * observed Nova host topology: positive DSND feeds one mono positive comb;
//     negative DSND preserves L/R mono paths and inverts each feedback comb
//
// ПРАВКИ 1.6.5 (жалобы пользователя):
//   * «в mnm эта ручка сенда сломана» — раньше выход core был dry/wet-кроссфейдом
//     (out = in*(1-mix)+read*mix), а TrackChain добавлял dry ЕЩЁ раз: при DSND=0
//     сухой сигнал удваивался (+6 дБ), из-за чего «thru режим в fx vst делает
//     звук громче». Теперь core пишет в линию ВХОД*send и отдаёт ЧИСТО МОКРЫЙ
//     сигнал (out = read), сухой добавляется ровно один раз в TrackChain —
//     при DSND=0 цепочка прозрачна (unity).
//   * «на значениях от 80 до 127 при прокрутке тайм делея клики» — таблица
//     времён дискретная (64 ступени 1/(n+1)), скачок указателя чтения давал
//     щелчки. Теперь длина линии slewing-уется к цели с настраиваемой
//     скоростью (параметр dly_repitch в MENU): плавное изменение длины =
//     постепенный repitch повторов, как у ленточного дилея, кликов нет.
//     OFF возвращает прежнее мгновенное поведение (для сравнения).
#pragma once

#include "MnmKernel.hpp"
#include "MnmDelayFeedbackFilter.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace monomachine {
namespace mnm {

class DelayCore {
public:
    // Four seconds covers the documented BPM law at BPM=30 / DTIM=127. The
    // legacy raw/table API retains its former one-second geometry below.
    static constexpr float kMaxSeconds = 4.0f;
    static constexpr float kLegacyMaxSeconds = 1.0f;

    void prepare(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        const int frames = static_cast<int>(sr * kMaxSeconds) + 4;
        left.assign(static_cast<size_t>(frames), 0.0f);
        right.assign(static_cast<size_t>(frames), 0.0f);
        size = frames;
        write = 0;
        maxSamples = static_cast<float>(frames - 2);
        legacyMaxSamples = std::min(maxSamples, static_cast<float>(sr * kLegacyMaxSeconds));
        feedbackFilter.prepare(sr);
        smoothSamples = -1.0f; // будет инициализирована первой целью
    }
    void reset() {
        std::fill(left.begin(), left.end(), 0.0f);
        std::fill(right.begin(), right.end(), 0.0f);
        write = 0;
        smoothSamples = -1.0f;
        feedbackFilter.reset();
    }

    // Feedback DBAS/DWID is deliberately configured outside process() so a
    // host control callback never clears the candidate feedback-stage memory.
    void setFeedbackFilterParameters(float base, float width, float baseQ, float widthQ) noexcept {
        feedbackFilter.setParameters(base, width, baseQ, widthQ);
    }
    void setLoopClip(bool enabled) noexcept { loopClipAfterFeedback=enabled; }

    // Legacy raw/table entry point retained for callers outside TrackChain.
    inline void process(float delParam, float feedback, float send, bool negativeComb,
                        float slewSeconds, float inL, float inR,
                        float& outL, float& outR) {
        const int idx = std::clamp(static_cast<int>(delParam * 63.0f / 127.0f + 0.5f), 0, 63);
        const float target = std::clamp(legacyMaxSamples * kDelayRecip[static_cast<size_t>(63 - idx)],
                                        1.0f, legacyMaxSamples);
        processTargetSamples(target,feedback,send,negativeComb,false,slewSeconds,inL,inR,outL,outR);
    }

    // Track Delay uses this path: it receives the already resolved musical or
    // BPM-OFF duration, rather than reinterpreting DTIM through a second law.
    inline void processSeconds(float delaySeconds, float feedback, float send, bool negativeComb,
                               float slewSeconds, float inL, float inR,
                               float& outL, float& outR) {
        processSeconds(delaySeconds,feedback,send,negativeComb,false,slewSeconds,inL,inR,outL,outR);
    }
    inline void processSeconds(float delaySeconds, float feedback, float send, bool negativeComb,
                               bool invertFeedback, float slewSeconds, float inL, float inR,
                               float& outL, float& outR) {
        const float finiteSeconds = std::isfinite(delaySeconds) ? delaySeconds : 0.0f;
        const float target = std::clamp(finiteSeconds * static_cast<float>(sr), 1.0f, maxSamples);
        processTargetSamples(target,feedback,send,negativeComb,invertFeedback,slewSeconds,inL,inR,outL,outR);
    }

private:
    inline float loopWrite(float value) const noexcept {
        if(!std::isfinite(value))return 0.0f;
        return loopClipAfterFeedback?DelayFeedbackFilter::stabilise(value):value;
    }
    inline void processTargetSamples(float target, float feedback, float send, bool negativeComb,
                                     bool invertFeedback, float slewSeconds, float inL, float inR,
                                     float& outL, float& outR) {
        if (smoothSamples < 0.0f) smoothSamples = target;
        if (slewSeconds > 0.0005f) {
            const float alpha = 1.0f - std::exp(-1.0f / (static_cast<float>(sr) * slewSeconds));
            smoothSamples += (target - smoothSamples) * alpha;
            if (std::fabs(target - smoothSamples) < 0.01f) smoothSamples = target;
        } else {
            smoothSamples = target;
        }

        const float pos = static_cast<float>(write) - smoothSamples;
        float readPos = pos;
        while (readPos < 0.0f) readPos += static_cast<float>(size);
        const int i0 = static_cast<int>(readPos) % size;
        const int i1 = (i0 + 1) % size;
        const float frac = readPos - std::floor(readPos);
        float aL = left[static_cast<size_t>(i0)] + frac * (left[static_cast<size_t>(i1)] - left[static_cast<size_t>(i0)]);
        float aR = right[static_cast<size_t>(i0)] + frac * (right[static_cast<size_t>(i1)] - right[static_cast<size_t>(i0)]);

        // DBAS/DWID are a documented host candidate feedback bridge, not
        // page words read by the verified Pack-8 DSP tail. The default bridge
        // is an exact bypass, retaining the previous timing/output until used.
        feedbackFilter.processStereo(aL, aR);

        // Positive DSND feeds one mono comb; negative DSND preserves the two
        // post-AMP/PAN mono lanes and flips feedback polarity. This remains
        // separate from firmware stage-2 words Y:$414..$417.
        const float mono = 0.5f * (inL + inR);
        const bool negativeFeedback=negativeComb!=invertFeedback;
        const float nextL = (negativeComb ? inL : mono) * send + (negativeFeedback ? -aL : aL) * feedback;
        const float nextR = (negativeComb ? inR : mono) * send + (negativeFeedback ? -aR : aR) * feedback;
        // Optional FB CLIP acts after the feedback write. With it disabled,
        // retain the unrestricted recurrence but never store NaN/Inf.
        left[static_cast<size_t>(write)] = loopWrite(nextL);
        right[static_cast<size_t>(write)] = loopWrite(nextR);
        write = (write + 1) % static_cast<size_t>(size);

        // Native P:$0A22/$0AB7 stage-2 belongs to its separate Y:$414..$417
        // words. This host compatibility core has no verified bridge for them,
        // therefore it returns the L/R banks without a P1 filter-control alias.
        outL = aL;
        outR = aR;
    }

    std::vector<float> left, right;
    size_t write = 0;
    int size = 1024;
    double sr = kDspRate;
    float maxSamples = 1022.0f, legacyMaxSamples = 1022.0f;
    bool loopClipAfterFeedback = false;
    float smoothSamples = -1.0f;
    DelayFeedbackFilter feedbackFilter;
};
}  // namespace mnm
}  // namespace monomachine
