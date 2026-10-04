// MnmFrqEnvFixSupport.hpp — private host shims for the isolated package-4 FM import.
// The imported STAT/PAR/DYN cores contain their own firmware operator
// envelopes and TONE processing. Nova's existing track AMP is external, so
// these wrapper compatibility stages are intentionally transparent.
#pragma once

namespace monomachine {
namespace fm_mnm_frq_env_fix {

inline constexpr int kBlock = 16;
inline constexpr double kDspRate = 44100.0;

class ImportEnvelopeCompat {
public:
    void reset() noexcept {}
    void trigger() noexcept {}
    void release() noexcept {}
    void setParameters(float, float, float, float) noexcept {}
    float tick() noexcept { return 1.0f; }
};

class ImportToneBypass {
public:
    void reset() noexcept {}
    void setSampleRate(double) noexcept {}
    void setTone(float) noexcept {}
    float process(int, float sample) noexcept { return sample; }
};

} // namespace fm_mnm_frq_env_fix
} // namespace monomachine
