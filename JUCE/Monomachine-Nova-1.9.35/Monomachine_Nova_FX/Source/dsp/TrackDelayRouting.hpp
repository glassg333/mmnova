// Контракт host-side маршрутизации трекового delay.
//
// DSND хранится вокруг центра 64 и показывается как -64..+63. Его модуль
// задаёт send. Положительная сторона питает один моно comb с положительной
// обратной связью; отрицательная разделяет pre-delay на L/R mono paths и
// инвертирует feedback comb. Это наблюдаемое поведение Nova, не заявление о
// bit-exact host mapping оригинальной прошивки.
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>

namespace nova::track_delay_routing {
// P1 flattened page: SYNTH 0..7, AMP 8..15, FILT 16..23, EFFX 24..31.
inline constexpr std::size_t kFilterEnvelopeAttack = 20; // Y:$40C
inline constexpr std::size_t kFilterEnvelopeDecay = 21;  // Y:$40D
inline constexpr std::size_t kFilterEnvelopeBaseOffset = 22; // Y:$410
inline constexpr std::size_t kFilterEnvelopeWidthOffset = 23; // Y:$411

inline constexpr std::size_t kDelayTime = 27;          // EFFX DTIM
inline constexpr std::size_t kDelaySend = 28;          // EFFX DSND, centred send / comb-route sign
inline constexpr std::size_t kDelayFeedback = 29;      // EFFX DFB, recurrence magnitude
inline constexpr std::size_t kDelayFilterBase = 30;    // EFFX DBAS, feedback-loop HP cutoff
inline constexpr std::size_t kDelayFilterWidth = 31;   // EFFX DWID, LP span above DBAS

inline float signedSend(float raw) noexcept
{
    return std::clamp(raw, 0.0f, 127.0f) - 64.0f;
}

// Raw -64 and +63 both reach full send; raw 64 is centre (0) and writes no
// new input. The caller preserves the last route while at centre so a tail is
// not forcibly re-routed mid-repeat.
inline float bipolarSendGain(float signedValue) noexcept
{
    return std::abs(signedValue) / (signedValue < 0.0f ? 64.0f : 63.0f);
}

inline float bipolarSendGain(const std::array<float, 32>& page) noexcept
{
    return bipolarSendGain(signedSend(page[kDelaySend]));
}
} // namespace nova::track_delay_routing
