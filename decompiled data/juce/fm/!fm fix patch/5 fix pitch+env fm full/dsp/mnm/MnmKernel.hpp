// Test stub for the pack-7 self-test: the real tree provides kDspRate and the
// machine base classes; only kDspRate is used by the mnm headers here.
#pragma once
#include <cstdint>
namespace monomachine {
inline constexpr double kDspRate = 44100.0;
}
