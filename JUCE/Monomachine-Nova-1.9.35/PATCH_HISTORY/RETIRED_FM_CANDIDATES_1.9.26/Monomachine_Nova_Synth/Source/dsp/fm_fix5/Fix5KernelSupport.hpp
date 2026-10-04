#pragma once
// Isolated Fix-5 support. The upstream self-test stub exported this constant in
// monomachine; keep it inside fm_fix5 so the import has no common FM symbol.
namespace monomachine { namespace fm_fix5 {
inline constexpr double kDspRate = 44100.0;
} }
