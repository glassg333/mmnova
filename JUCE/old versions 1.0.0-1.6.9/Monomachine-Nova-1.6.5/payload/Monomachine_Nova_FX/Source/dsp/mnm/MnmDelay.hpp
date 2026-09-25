// FX-DLY (machine 32) "mnm" core -- Monomachine Nova 1.6.0 / revised 1.6.5
//
// Recovered geometry and parameter law from P:$147B38-$147D00
// (listings: memory_images/dsp1_machines_fx.txt):
//
//   * buffer: X:$114000, 2048 words per channel, second channel at +$800
//   * delay time: DEL parameter is turned into a read rate through the reciprocal
//     table X:$144C49 (values 1/1, 1/2, 1/3 ... 1/64)
//   * read pointer: fractional read with linear interpolation
//   * feedback topology: two independent loops with the optional cross feed
//     used by the ping-pong setting
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

#include <vector>

namespace monomachine {
namespace mnm {

class DelayCore {
public:
    // The firmware keeps the delay in X:$114000 with a 2048-word window per channel and a
    // variable-rate (interpolated) read; the plugin therefore sizes the line for one second of
    // host audio while keeping the firmware's max/min ratio of 1 : 1/64.
    static constexpr float kMaxSeconds = 1.0f;

    void prepare(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        const int frames = static_cast<int>(sr * kMaxSeconds) + 4;
        left.assign(static_cast<size_t>(frames), 0.0f);
        right.assign(static_cast<size_t>(frames), 0.0f);
        size = frames;
        write = 0;
        maxSamples = static_cast<float>(frames - 2);
        smoothSamples = -1.0f; // будет инициализирована первой целью
    }
    void reset() {
        std::fill(left.begin(), left.end(), 0.0f);
        std::fill(right.begin(), right.end(), 0.0f);
        write = 0;
        smoothSamples = -1.0f;
    }

    // delayParam:   DEL/DTIM knob 0..127 -> reciprocal table index (0..63)
    // feedback:     0..1
    // send:         0..1 — how much of the input is written into the line
    //               (DSND на hardware: 0 = посыла нет, цепочка прозрачна)
    // pingPong:     cross-feed the two feedback loops
    // slewSeconds:  0 = мгновенная смена длины (прежнее поведение, возможны
    //               клики); >0 = время сглаживания длины линии: повторы при
    //               прокрутке DTIM плавно репитчатся без щелчков.
    // outL/outR:    ЧИСТО мокрый сигнал; сухой добавляет вызывающий код.
    inline void process(float delParam, float feedback, float send, bool pingPong,
                        float slewSeconds, float inL, float inR,
                        float& outL, float& outR) {
        // DTIM 0..127 -> 64-entry reciprocal table. The table itself is exact (P:$144C49);
        // the knob direction is not verified yet, so DTIM 0 is the shortest setting:
        // index 0 -> 1/64 of the line, index 63 -> the full line.
        const int idx = std::clamp(static_cast<int>(delParam * 63.0f / 127.0f + 0.5f), 0, 63);
        const float target = std::clamp(maxSamples * kDelayRecip[static_cast<size_t>(63 - idx)],
                                        1.0f, maxSamples);
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
        const float aL = left[static_cast<size_t>(i0)] + frac * (left[static_cast<size_t>(i1)] - left[static_cast<size_t>(i0)]);
        const float aR = right[static_cast<size_t>(i0)] + frac * (right[static_cast<size_t>(i1)] - right[static_cast<size_t>(i0)]);

        const float fbL = pingPong ? aR : aL;
        const float fbR = pingPong ? aL : aR;
        left[static_cast<size_t>(write)] = inL * send + fbL * feedback;
        right[static_cast<size_t>(write)] = inR * send + fbR * feedback;
        write = (write + 1) % static_cast<size_t>(size);

        outL = aL;
        outR = aR;
    }

private:
    std::vector<float> left, right;
    size_t write = 0;
    int size = 1024;
    double sr = kDspRate;
    float maxSamples = 1022.0f;
    float smoothSamples = -1.0f;
};

}  // namespace mnm
}  // namespace monomachine
