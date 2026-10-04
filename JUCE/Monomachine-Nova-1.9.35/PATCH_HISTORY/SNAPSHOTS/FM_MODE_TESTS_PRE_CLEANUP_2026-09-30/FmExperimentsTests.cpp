// JUCE-free regression suite for append-only FM candidates.
// It deliberately includes the retained MNM header as an isolation sentinel:
// the old core fingerprint must remain unchanged while every imported candidate
// owns separate namespaces, tables, page state, and rendering paths.
#include "dsp/fm_mnm_frq_env_fix/Import4Fm.hpp"
#include "dsp/fm_mnm_frq_env_fix/TrackEnvExact.hpp"
#include "dsp/fm_try4_voice/Try4VoiceFm.hpp"
#include "dsp/fm_fix5/Fix5Fm.hpp"
#include "dsp/mnm/MnmFm.hpp"
#include "models/DspModes.hpp"
#include "models/FmExperimentProfiles.hpp"
#include "models/machine_definitions.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

namespace {
int failures = 0;

void require(bool condition, const char* what)
{
    std::printf("%-82s %s\n", what, condition ? "OK" : "FAIL");
    if (!condition) ++failures;
}

uint32_t floatBits(float value)
{
    uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

uint64_t hashWords(const float* samples, int count)
{
    uint64_t hash = 1469598103934665603ull;
    for (int i = 0; i < count; ++i) {
        const uint32_t word = floatBits(samples[i]);
        for (int byte = 0; byte < 4; ++byte) {
            hash ^= (word >> (byte * 8)) & 0xffu;
            hash *= 1099511628211ull;
        }
    }
    return hash;
}

template <typename Core, typename Kind>
uint64_t renderHash(Kind kind, const std::array<float, 8>& parameters,
                    int firstFrames, int secondFrames, bool interleaveOther)
{
    Core core;
    core.reset(44100.0);
    core.setParameters(kind, parameters);
    if constexpr (std::is_same<Core, monomachine::fm_try4_voice::FmCore>::value) {
        core.setAmpParams(0, 0, 90, 40, 127, 64);
        core.setTempoWord(2912);
    }
    core.noteOn(60.0f, 1.0f);
    core.setPitchMod(0.25f);

    std::array<float, 193> output{};
    core.processBlock(output.data(), firstFrames);
    if (interleaveOther) {
        monomachine::fm_mnm_frq_env_fix::FmCore other;
        other.reset(44100.0);
        other.setParameters(monomachine::fm_mnm_frq_env_fix::FmKind::Dyn,
                            {0, 64, 64, 64, 0, 80, 30, 64});
        other.noteOn(51.0f, 1.0f);
        float scratch[97]{};
        other.processBlock(scratch, 97);
    }
    core.processBlock(output.data() + firstFrames, secondFrames);
    return hashWords(output.data(), firstFrames + secondFrames);
}

bool finiteAndAudible(monomachine::fm_mnm_frq_env_fix::FmKind kind,
                      const std::array<float, 8>& parameters)
{
    monomachine::fm_mnm_frq_env_fix::FmCore core;
    core.reset(44100.0);
    core.setParameters(kind, parameters);
    core.noteOn(60.0f, 1.0f);
    float output[192]{};
    core.processBlock(output, 192);
    float energy = 0.0f;
    for (float sample : output) {
        if (!std::isfinite(sample)) return false;
        energy += sample * sample;
    }
    return energy > 1.0e-8f;
}

bool fix5FiniteAudibleAndReleases(monomachine::fm_fix5::FmKind kind,
                                  const std::array<float, 8>& parameters)
{
    monomachine::fm_fix5::FmCore core;
    core.reset(44100.0);
    core.setParameters(kind, parameters);
    core.setAmpParams(0, 0, 90, 40, 127, 64);
    core.setTempoWord(120);
    core.noteOn(60.0f, 1.0f);
    std::array<float, 320> left{}, right{};
    core.processBlockStereo(left.data(), right.data(), static_cast<int>(left.size()));
    float energy = 0.0f;
    for (size_t i = 0; i < left.size(); ++i) {
        if (!std::isfinite(left[i]) || !std::isfinite(right[i])) return false;
        energy += left[i] * left[i] + right[i] * right[i];
    }
    // A non-centre PAN must use the imported stereo gain-ring, not a mono
    // collapse. Rendering a small next block also exercises the 16-sample queue.
    const bool stereo = std::abs(left[127] - right[127]) > 1.0e-9f;
    const int levelBeforeRelease = core.envLevel();
    core.noteOff();
    std::array<float, 4096> tailL{}, tailR{};
    core.processBlockStereo(tailL.data(), tailR.data(), static_cast<int>(tailL.size()));
    // REL=40 is intentionally a long native exponential tail; validate its
    // state/progression rather than incorrectly requiring a generic ADSR's
    // short fade. MachineEngine keeps rendering it until the native level ends.
    return energy > 1.0e-8f && stereo && core.envPhase() == fix5fm::MnmAmpEnv::kRel
        && core.envLevel() > 0 && core.envLevel() < levelBeforeRelease
        && std::isfinite(tailL.back()) && std::isfinite(tailR.back());
}

bool try4FiniteAudibleAndReleases(monomachine::fm_try4_voice::FmKind kind,
                                  const std::array<float, 8>& parameters)
{
    monomachine::fm_try4_voice::FmCore core;
    core.reset(44100.0);
    core.setParameters(kind, parameters);
    core.setAmpParams(0, 0, 90, 40, 111, 42);
    core.setTempoWord(2912);
    core.noteOn(60.0f, 1.0f);
    std::array<float, 320> left{}, right{};
    core.processBlockStereo(left.data(), right.data(), static_cast<int>(left.size()));
    float energy = 0.0f;
    for (size_t i = 0; i < left.size(); ++i) {
        if (!std::isfinite(left[i]) || !std::isfinite(right[i])) return false;
        energy += left[i] * left[i] + right[i] * right[i];
    }
    const auto& page = core.voicePageMap();
    const bool mapped = page.machineWords[0] == (static_cast<uint32_t>(parameters[0]) << 16)
        && page.volumeWord == (111u << 16) && page.panWord == (42u << 16);
    const bool stereo = std::abs(left[127] - right[127]) > 1.0e-9f;
    const int levelBeforeRelease = core.envLevel();
    core.noteOff();
    std::array<float, 4096> tailL{}, tailR{};
    core.processBlockStereo(tailL.data(), tailR.data(), static_cast<int>(tailL.size()));
    return energy > 1.0e-8f && mapped && stereo
        && core.envPhase() == try4voicefm::MnmAmpEnv::kRel
        && core.envLevel() > 0 && core.envLevel() < levelBeforeRelease
        && std::isfinite(tailL.back()) && std::isfinite(tailR.back());
}

bool continuousCandidateTrackEnvelope()
{
    monomachine::fm_mnm_frq_env_fix::ContinuousEnvExactCore envelope;
    envelope.reset();
    envelope.setParameters(20.0f, 0.0f, 64.0f, 40.0f, 120.0f);
    envelope.noteOn();

    std::array<float, 4096> output{};
    for (int i = 0; i < 1536; ++i) output[static_cast<size_t>(i)] = envelope.process();
    envelope.noteOff();
    for (int i = 1536; i < static_cast<int>(output.size()); ++i)
        output[static_cast<size_t>(i)] = envelope.process();

    bool finiteAndBounded = true;
    int changed = 0;
    int run = 1;
    int longestRun = 1;
    float maxStep = 0.0f;
    for (size_t i = 0; i < output.size(); ++i) {
        finiteAndBounded = finiteAndBounded && std::isfinite(output[i])
            && output[i] >= 0.0f && output[i] <= 1.000001f;
        if (i == 0) continue;
        maxStep = std::max(maxStep, std::abs(output[i] - output[i - 1]));
        if (output[i] == output[i - 1]) {
            ++run;
        } else {
            ++changed;
            longestRun = std::max(longestRun, run);
            run = 1;
        }
    }
    longestRun = std::max(longestRun, run);
    return finiteAndBounded && changed > 3600 && longestRun <= 8 && maxStep < 0.02f;
}

uint64_t retainedMnmFingerprint()
{
    monomachine::mnm::FmCore core;
    core.reset(44100.0);
    core.setParameters(monomachine::mnm::FmKind::Par,
                       {16, 64, 32, 64, 48, 64, 98, 64});
    core.noteOn(60.0f, 1.0f);
    core.setPitchMod(1.25f);
    float block[monomachine::mnm::kBlock]{};
    std::array<float, monomachine::mnm::kBlock * 23> all{};
    for (int i = 0; i < 23; ++i) {
        core.processBlock(block, monomachine::mnm::kBlock);
        std::memcpy(all.data() + i * monomachine::mnm::kBlock, block, sizeof block);
    }
    return hashWords(all.data(), static_cast<int>(all.size()));
}
} // namespace

int main()
{
    using namespace monomachine;

    // The new types must not be aliases of each other or of the retained MNM
    // class. This catches accidental include/reuse instead of an isolated import.
    static_assert(!std::is_same<fm_try4_voice::FmCore, fm_mnm_frq_env_fix::FmCore>::value,
                  "TRY4 and MNM FRQ ENV FIX must own distinct FmCore types");
    static_assert(!std::is_same<try4voicefm::FmMem, mnmfrqenvfm::FmMem>::value,
                  "TRY4 and MNM FRQ ENV FIX must own distinct table/state types");
    static_assert(!std::is_same<fm_try4_voice::FmCore, mnm::FmCore>::value,
                  "TRY4 must not be the retained MNM core");
    static_assert(!std::is_same<fm_fix5::FmCore, fm_try4_voice::FmCore>::value,
                  "Fix-5 and TRY4 must own distinct FmCore types");
    static_assert(!std::is_same<fm_fix5::FmCore, mnm::FmCore>::value,
                  "Fix-5 must not reuse the retained MNM core");

    require(kDspModeSchemaVersion == 34
                && dspModeMnm == 0 && dspModeOld == 1 && dspModeNew == 2
                && dspModeMnmFix == 3 && dspModeNewFix == 4 && dspModeOldFix == 5
                && dspModeMnmFrqEnvFix == 6 && dspModeTry4 == 7
                && dspModeMnmFix5Full == 8,
            "all retained IDs 0..7 are byte-stable and Fix-5 appends ID 8");
    require(std::strcmp(dspModeName(dspModeMnmFrqEnvFix), "mnm frq env fix") == 0
                && std::strcmp(dspModeName(dspModeTry4), "try4") == 0
                && std::strcmp(dspModeName(dspModeMnmFix5Full), "fix5 pitch+env full") == 0,
            "appended serialized names retain Fix-4/TRY4 and add independent Fix-5");
    require(dspSyntModeAllowedForSchema(9, dspModeOldFix, 31)
                && !dspSyntModeAllowedForSchema(9, dspModeMnmFrqEnvFix, 31)
                && dspSyntModeAllowedForSchema(9, dspModeMnmFrqEnvFix, 32)
                && !dspSyntModeAllowedForSchema(9, dspModeTry4, 32)
                && dspSyntModeAllowedForSchema(9, dspModeTry4, 33)
                && !dspSyntModeAllowedForSchema(9, dspModeMnmFix5Full, 33)
                && dspSyntModeAllowedForSchema(9, dspModeMnmFix5Full, 34)
                && !dspSyntModeAllowedForSchema(7, dspModeMnmFrqEnvFix, 33)
                && !dspSyntModeAllowedForSchema(7, dspModeTry4, 33)
                && !dspSyntModeAllowedForSchema(7, dspModeMnmFix5Full, 34),
            "schema gate preserves old serialized states and confines Fix-5 ID 8 to FM+ m8/m9/m10");
    require(dspSyntModeCountForMachine(8) == 9
                && std::strcmp(dspSyntModeChoicesForMachine(8),
                    "mnm FREQ|old FREQ|new FREQ|mnm fix BPM|new fix BPM|old fix BPM|mnm frq env fix|try4|fix5 BPM pitch+env full") == 0
                && std::strcmp(dspSyntModeChoicesForMachine(9),
                    "mnm|old|new|mnm fix|new fix|old fix|mnm frq env fix|try4|fix5 pitch+env full") == 0,
            "STAT selector names distinguish retained FREQ from every native-pitch FIX route, including Fix-5");

    const auto findMachine=[](int id) -> const MachineDef* {
        for (const auto& machine : getAllMachineDefinitions())
            if (machine.id == id) return &machine;
        return nullptr;
    };
    const auto* par = findMachine(9);
    require(par != nullptr
                && par->synthParams[0].defaultVal == 16
                && par->synthParams[2].defaultVal == 32
                && par->synthParams[4].defaultVal == 48
                && fm_experiments::parDefaultReadsUnity(0, 16)
                && fm_experiments::parDefaultReadsUnity(2, 32)
                && fm_experiments::parDefaultReadsUnity(4, 48)
                && !fm_experiments::parDefaultReadsUnity(0, 17),
            "FM+ PAR UI/default profile visibly calibrates retained raw defaults to 1 / 1 / 1 without DSP remap");

    require(try4FiniteAudibleAndReleases(fm_try4_voice::FmKind::Stat,
                                         {60, 64, 80, 30, 80, 64, 98, 64})
                && try4FiniteAudibleAndReleases(fm_try4_voice::FmKind::Par,
                                                {60, 64, 80, 64, 80, 64, 98, 64})
                && try4FiniteAudibleAndReleases(fm_try4_voice::FmKind::Dyn,
                                                {64, 64, 64, 64, 74, 80, 30, 64}),
            "TRY4 использует отдельный Package-5 AMP/pan/page-frame путь для STAT/PAR/DYN");

    require(fix5FiniteAudibleAndReleases(fm_fix5::FmKind::Stat,
                                         {60, 64, 80, 30, 80, 64, 98, 64})
                && fix5FiniteAudibleAndReleases(fm_fix5::FmKind::Par,
                                                {60, 64, 80, 64, 80, 64, 98, 64})
                && fix5FiniteAudibleAndReleases(fm_fix5::FmKind::Dyn,
                                                {64, 64, 64, 64, 74, 80, 30, 64}),
            "Fix-5 native AMP/VOL squared/pan/frame renderer is finite, stereo and releases for STAT/PAR/DYN");

    require(fm_experiments::mnmFrqEnvFixNeedsRawZeroInitialisation(8, {16, 64, 0, 0, 32, 64, 64, 64})
                && fm_experiments::mnmFrqEnvFixNeedsRawZeroInitialisation(9, {16, 64, 32, 64, 48, 64, 64, 64})
                && fm_experiments::mnmFrqEnvFixNeedsRawZeroInitialisation(10, {16, 0, 64, 0, 32, 80, 30, 64})
                && !fm_experiments::mnmFrqEnvFixNeedsRawZeroInitialisation(9, {0, 64, 32, 64, 48, 64, 64, 64})
                && fm_experiments::mnmFrqEnvFixIsFrequencyKnob(9, 0)
                && fm_experiments::mnmFrqEnvFixIsFrequencyKnob(9, 2)
                && fm_experiments::mnmFrqEnvFixIsFrequencyKnob(9, 4)
                && !fm_experiments::mnmFrqEnvFixIsFrequencyKnob(9, 1),
            "first m6 selection/reset initializes only fresh FM+ FRQ defaults to raw zero and preserves custom words");

    // TRY4 is not a one-entry ratio-table variant of m6: the Package-5
    // FM/AMP page map has its own raw machine/AMP words and trigger/pitch
    // state. $414..$417 are independently named native stage-2 cells, not
    // aliases of P1 FILT ATK/DEC in this FM/AMP-only renderer.
    fm_try4_voice::VoicePageMap try4Page{};
    try4Page.setMachine({60, 64, 80, 30, 80, 64, 98, 64});
    try4Page.setAmp(0, 0, 90, 40, 111, 42);
    try4Page.triggerWord = 1; try4Page.pitchWord = 0x123456789abcull;
    require(try4Page.machineWords[0] == (60u << 16)
                && try4Page.machineWords[7] == (64u << 16)
                && try4Page.volumeWord == (111u << 16)
                && try4Page.panWord == (42u << 16)
                && try4Page.triggerWord == 1 && try4Page.pitchWord == 0x123456789abcull
                && fm_try4_voice::VoicePageMap::kFilterAttack == 0x40c
                && fm_try4_voice::VoicePageMap::kStage2Attack == 0x414
                && fm_try4_voice::VoicePageMap::kStage2WidthOffset == 0x417,
            "TRY4 хранит отдельную Package-5 FM/AMP-карту и независимую stage-2 границу, а не endpoint-клон m6");

    require(finiteAndAudible(fm_mnm_frq_env_fix::FmKind::Stat,
                             {0, 64, 80, 30, 0, 64, 98, 64})
                && finiteAndAudible(fm_mnm_frq_env_fix::FmKind::Par,
                                    {0, 64, 0, 64, 0, 64, 98, 64})
                && finiteAndAudible(fm_mnm_frq_env_fix::FmKind::Dyn,
                                    {0, 64, 64, 64, 0, 80, 30, 64}),
            "MNM FRQ ENV FIX safely renders all FM+ machines at raw-zero frequency controls");

    const std::array<float, 8> try4Par{16, 64, 32, 64, 48, 64, 98, 64};
    const uint64_t try4Contiguous=renderHash<fm_try4_voice::FmCore>(fm_try4_voice::FmKind::Par, try4Par, 193, 0, false);
    const uint64_t try4Split=renderHash<fm_try4_voice::FmCore>(fm_try4_voice::FmKind::Par, try4Par, 37, 156, true);
    require(try4Contiguous == try4Split,
            "TRY4 Package-5 output/state is callback-split invariant and independent of concurrent m6 state");

    // Representative envelope sweep: exact imported operator envelopes must
    // remain finite, continuously sampled (no long held bit-crush plateaus),
    // and bounded across a changed ENV word at a host callback boundary.
    fm_mnm_frq_env_fix::FmCore envelope;
    envelope.reset(44100.0);
    std::array<float, 8> before{16, 64, 32, 8, 48, 8, 98, 64};
    std::array<float, 8> after{16, 100, 32, 100, 48, 100, 98, 64};
    envelope.setParameters(fm_mnm_frq_env_fix::FmKind::Par, before);
    envelope.noteOn(60.0f, 1.0f);
    float sweep[512]{};
    envelope.processBlock(sweep, 256);
    envelope.setParameters(fm_mnm_frq_env_fix::FmKind::Par, after);
    envelope.processBlock(sweep + 256, 256);
    bool finite = true;
    int changed = 0, run = 1, longestRun = 1;
    float maxStep = 0.0f;
    for (int i = 0; i < 512; ++i) finite = finite && std::isfinite(sweep[i]);
    for (int i = 1; i < 512; ++i) {
        maxStep = std::max(maxStep, std::abs(sweep[i] - sweep[i - 1]));
        if (sweep[i] != sweep[i - 1]) { ++changed; longestRun = std::max(longestRun, run); run = 1; }
        else ++run;
    }
    longestRun = std::max(longestRun, run);
    require(finite && changed > 240 && longestRun <= 2 && maxStep < 0.20f
                && std::abs(sweep[256] - sweep[255]) < 0.20f,
            "package-4 ENV route is finite/continuous through ENV change with no long bit-crush plateau");

    require(continuousCandidateTrackEnvelope(),
            "MNM FRQ ENV FIX track AMP keeps the patch-4 frame law while removing 16-sample boundary clicks");

    require(retainedMnmFingerprint() == 0x61adea6373f3da33ull,
            "retained MNM PAR fingerprint remains unchanged after isolated experimental imports");

    if (failures != 0) {
        std::fprintf(stderr, "FM_EXPERIMENTS_TESTS FAIL: %d check(s)\n", failures);
        return 1;
    }
    std::puts("FM_EXPERIMENTS_TESTS PASS");
    return 0;
}
