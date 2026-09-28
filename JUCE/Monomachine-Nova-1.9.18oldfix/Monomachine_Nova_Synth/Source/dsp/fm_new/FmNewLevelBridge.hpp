// FmNewLevelBridge.hpp — host-facing level compensation for FM+ NEW routes.
//
// The imported exact cores deliberately keep their raw Q23 output conversion
// byte-for-byte intact. The host bridge applies the user-measured +10 dB
// compensation after those cores only; MNM, OLD, and the raw exact headers are
// never rescaled or modified.
#pragma once

namespace monomachine {
namespace fm_new {

inline constexpr float kOutputBridgeGainDb = 10.0f;
inline constexpr float kOutputBridgeGain = 3.1622776601683795f; // 10^(10/20)

} // namespace fm_new
} // namespace monomachine
