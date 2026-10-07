#pragma once

#include <array>
#include <cstddef>
#include <memory>

namespace combscanner
{
inline constexpr int kNumCombs = 8;
inline constexpr int kNumFxSlots = 5;
inline constexpr int kNumLfos = 4;
inline constexpr int kNumSequences = 4;
inline constexpr int kSequenceSteps = 16;
inline constexpr int kNumModRoutes = 16;
inline constexpr int kFftLatencySamples = 128;

// Stable, host-visible enum indices. Keep these in sync with PluginParameters.cpp.
enum class DspMode : int { classicFav = 0, moreControl = 1 };
enum class FxType : int { off = 0, filter, granular, fftStretch, limiter, saturator };
enum class LfoShape : int { sine = 0, triangle, saw, square, sampleHold };
enum class SequenceMode : int { forward = 0, reverse, pingPong, random };

// Modulation source indices: 0=off, 1..4=LFOs, 5..8=sequences,
// 9=envelope, 10=mod wheel, 11=last MIDI note velocity.
inline constexpr int kModSourceCount = 12;

// Destination indices: 0=off; globals 1..12; then five 8-comb banks in order:
// ratio, feedback, delay 1, delay 2, and level.
inline constexpr int kModDestinationCount = 53;
inline constexpr int kDestinationCombRatioBase = 13;
inline constexpr int kDestinationCombFeedbackBase = 21;
inline constexpr int kDestinationCombDelay1Base = 29;
inline constexpr int kDestinationCombDelay2Base = 37;
inline constexpr int kDestinationCombLevelBase = 45;

struct CombParameters
{
    bool enabled = true;
    float ratio = 1.0f;
    float ratio2 = 1.0f;
    float delay1Ms = 115.0f;
    float delay2Ms = 500.0f;
    float feedback = 0.72f;
    float damp = 0.78f;
    float phase = 0.65f;
    float diffusion = 0.50f;
    float crossMix = 0.35f;
    float driveDb = 0.0f;
    float levelDb = 0.0f;
    float pan = 0.0f;
    float scanWeight = 1.0f;
};

struct LfoParameters
{
    float rateHz = 0.35f;
    float depth = 0.50f;
    int shape = static_cast<int>(LfoShape::sine);
    bool tempoSync = false;
    int division = 2; // index into the sync-division table
};

struct SequenceParameters
{
    float stepRateHz = 2.0f;
    float slew = 0.0f;
    int length = kSequenceSteps;
    int mode = static_cast<int>(SequenceMode::forward);
    bool tempoSync = false;
    int division = 0;
    std::array<float, kSequenceSteps> steps {};
};

struct FxParameters
{
    int type = static_cast<int>(FxType::off);
    int afterComb = 1; // 1..8
    float mix = 0.0f;
    float size = 0.50f;
    float pitch = 0.0f; // normalized -1..1
    float blur = 0.20f;
    bool freeze = false;
};

struct ModulationRoute
{
    int source = 0;
    int destination = 0;
    float amount = 0.0f;
};

struct EngineParameters
{
    int mode = static_cast<int>(DspMode::classicFav);
    int model = 0;
    bool bypass = false;

    // Classic Fav shared controls (the v1-fav reference model).
    float classicFeedback = 0.80f;
    float classicDamp = 0.90f;
    float classicPhase = 0.75f;
    float classicDelay1Ms = 115.0f;
    float classicDelay2Ms = 500.0f;

    // Scanner / More Control macro section.
    float scan = 0.0f;
    float scanWidth = 0.22f;
    float scanRateHz = 0.0f;
    int scanShape = 0;
    float globalDiffusion = 0.55f;
    float globalCrossMix = 0.50f;
    float motion = 0.45f;
    float dryWet = 0.82f;
    float outputDb = 0.0f;
    float stereoWidth = 1.0f;

    float midiWheel = 0.0f;
    float midiVelocity = 0.0f;

    std::array<CombParameters, kNumCombs> combs {};
    std::array<FxParameters, kNumFxSlots> fxSlots {};
    std::array<LfoParameters, kNumLfos> lfos {};
    std::array<SequenceParameters, kNumSequences> sequences {};
    std::array<ModulationRoute, kNumModRoutes> routes {};
};

EngineParameters makeDefaultEngineParameters() noexcept;

class CombScannerEngine
{
public:
    CombScannerEngine();
    ~CombScannerEngine();

    CombScannerEngine(const CombScannerEngine&) = delete;
    CombScannerEngine& operator=(const CombScannerEngine&) = delete;

    void prepare(double sampleRate, int maximumBlockSize);
    void reset() noexcept;
    void setParameters(const EngineParameters& parameters) noexcept;
    void processBlock(float* left, float* right, int numSamples, double tempoBpm = 120.0) noexcept;

    [[nodiscard]] int getLatencySamples() const noexcept;
    [[nodiscard]] double getTailLengthSeconds() const noexcept;
    [[nodiscard]] float getEnvelopeFollower() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace combscanner
