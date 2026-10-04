// JUCE-free regression checks for opt-in FM+ MNM FIX / NEW FIX / OLD FIX profiles.
// Existing mnm/old/new paths are covered separately and must remain unchanged.
#include "dsp/fm_fix/FmFixTables.hpp"
#include "dsp/fm_fix/MnmFmFix.hpp"
#include "dsp/fm_fix/OldFmFix.hpp"
#include "dsp/fm_new/FmExactNew.hpp"
#include "dsp/fm_new/FmNewLevelBridge.hpp"
#include "dsp/fm_new_fix/FmExactNewFix.hpp"
#include "dsp/mnm/MnmFm.hpp"
#include "dsp/monomachine_fm_dynamic.hpp"
#include "dsp/monomachine_fm_par.hpp"
#include "models/DspModes.hpp"
#include "models/machine_definitions.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace {
int failures = 0;

void require(bool condition, const char* what)
{
    std::printf("%-74s %s\n", what, condition ? "OK" : "FAIL");
    if (!condition) ++failures;
}

bool closeEnough(float actual, float expected, float tolerance = 1.0e-7f)
{
    return std::abs(actual - expected) <= tolerance;
}

uint32_t floatBits(float value)
{
    uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

uint64_t fnv1a(uint64_t hash, uint32_t word)
{
    for (int byte = 0; byte < 4; ++byte) {
        hash ^= (word >> (byte * 8)) & 0xffu;
        hash *= 1099511628211ull;
    }
    return hash;
}

uint64_t mnmHash(monomachine::mnm::FmKind kind, const std::array<float, 8>& p)
{
    // This is the untouched retained MnmFm.hpp core: no FIX flag/table exists
    // in the object, so this path cannot be contaminated by selecting FIX.
    monomachine::mnm::FmCore core;
    core.reset(44100.0);
    core.setParameters(kind, p);
    core.noteOn(60.0f, 1.0f);
    core.setPitchMod(0.0f);
    uint64_t hash = 1469598103934665603ull;
    float block[monomachine::mnm::kBlock]{};
    for (int blockIndex = 0; blockIndex < 24; ++blockIndex) {
        core.processBlock(block, monomachine::mnm::kBlock);
        for (float sample : block) hash = fnv1a(hash, floatBits(sample));
    }
    return hash;
}

uint64_t mnmFixHash(monomachine::fm_fix::MnmFixKind kind, const std::array<float, 8>& p)
{
    // FIX owns a different class and a separate state object from retained MNM.
    monomachine::fm_fix::MnmFixCore core;
    core.reset(44100.0);
    core.setParameters(kind, p);
    core.noteOn(60.0f, 1.0f);
    core.setPitchMod(0.0f);
    uint64_t hash = 1469598103934665603ull;
    float block[monomachine::mnm::kBlock]{};
    for (int blockIndex = 0; blockIndex < 24; ++blockIndex) {
        core.processBlock(block, monomachine::mnm::kBlock);
        for (float sample : block) hash = fnv1a(hash, floatBits(sample));
    }
    return hash;
}

uint64_t exactHash(monomachine::fm_new::FmExactKind kind, const std::array<float, 8>& p)
{
    // Retained NEW is the restored imported header/core, with no FIX table API.
    monomachine::fm_new::FmExactCore core;
    core.reset(44100.0);
    core.noteOn(60.0f);
    core.setParameters(kind, p);
    core.setPitchWordOverride(11776u, true);
    uint64_t hash = 1469598103934665603ull;
    float block[96]{};
    core.processBlock(block, 96);
    for (float sample : block) hash = fnv1a(hash, floatBits(sample));
    return hash;
}

uint64_t exactFixHash(monomachine::fm_new::FmExactKind kind, const std::array<float, 8>& p)
{
    // NEW FIX owns a separately namespaced imported core and a fixed measured table.
    monomachine::fm_new::FmExactFixCore core;
    core.reset(44100.0);
    core.noteOn(60.0f);
    core.setParameters(kind, p);
    core.setPitchWordOverride(11776u, true);
    uint64_t hash = 1469598103934665603ull;
    float block[96]{};
    core.processBlock(block, 96);
    for (float sample : block) hash = fnv1a(hash, floatBits(sample));
    return hash;
}
} // namespace

int main()
{
    using namespace monomachine;

    const auto findMachine=[](int id)->const MachineDef*{
        for(const auto& machine:getAllMachineDefinitions())if(machine.id==id)return &machine;
        return nullptr;
    };
    const auto* legacyStat=findMachine(8);
    const auto* legacyPar=findMachine(9);
    const auto* legacyDyn=findMachine(10);
    const auto* statFixFactory=fm_fix::factoryDefaultsForMachine(8);
    const auto* parFixFactory=fm_fix::factoryDefaultsForMachine(9);
    const auto* dynFixFactory=fm_fix::factoryDefaultsForMachine(10);
    require(legacyStat!=nullptr&&legacyPar!=nullptr&&legacyDyn!=nullptr
                &&legacyStat->synthParams[0].defaultVal==16&&legacyStat->synthParams[4].defaultVal==32
                &&legacyPar->synthParams[0].defaultVal==16&&legacyPar->synthParams[2].defaultVal==32
                &&legacyDyn->synthParams[0].defaultVal==16&&legacyDyn->synthParams[4].defaultVal==32
                &&statFixFactory!=nullptr&&(*statFixFactory)[0]==60&&(*statFixFactory)[4]==80
                &&parFixFactory!=nullptr&&(*parFixFactory)[0]==60&&(*parFixFactory)[4]==102
                &&dynFixFactory!=nullptr&&(*dynFixFactory)[0]==64&&(*dynFixFactory)[4]==74,
            "retained global defaults stay baseline; recovered factory profiles are explicit FIX-only data");

    require(kDspModeSchemaVersion == 34
                && dspModeMnm == 0 && dspModeOld == 1 && dspModeNew == 2
                && dspModeMnmFix == 3 && dspModeNewFix == 4 && dspModeOldFix == 5
                && dspModeMnmFrqEnvFix == 6 && dspModeTry4 == 7 && dspModeMnmFix5Full == 8
                && dspSyntModeCountForMachine(8) == 9
                && std::strcmp(dspSyntModeChoicesForMachine(8),
                    "mnm FREQ|old FREQ|new FREQ|mnm fix BPM|new fix BPM|old fix BPM|mnm frq env fix|try4|fix5 BPM pitch+env full") == 0
                && !dspSyntModeUsesMeasuredFix(dspModeMnmFix5Full),
            "schema 34 appends independent Fix-5 ID 8 without renumbering retained mnm/old/new/FIX IDs 0..7");
    require(!dspSyntModeUsesMeasuredFix(dspModeMnm)
                && !dspSyntModeUsesMeasuredFix(dspModeOld)
                && !dspSyntModeUsesMeasuredFix(dspModeNew)
                && dspSyntModeUsesMeasuredFix(dspModeMnmFix)
                && dspSyntModeUsesMeasuredFix(dspModeNewFix)
                && dspSyntModeUsesMeasuredFix(dspModeOldFix),
            "measured table/readout profile is confined to appended FIX mode IDs including OLD FIX");
    require(closeEnough(20.0f * std::log10(fm_new::kOutputBridgeGain), 10.0f, 1.0e-5f)
                && closeEnough(fm_new::kOutputBridgeGainDb, 10.0f),
            "NEW and NEW FIX share the explicit user-measured +10 dB output bridge");

    // STAT/PAR hardware-facing frequency law and literal observed table order.
    require(fm_fix::statParRatioIndex(0) == 0
                && fm_fix::statParRatioIndex(48) == 9
                && fm_fix::statParRatioIndex(60) == 11
                && fm_fix::statParRatioIndex(80) == 15
                && fm_fix::statParRatioIndex(127) == 23,
            "STAT/PAR raw control values select the measured 24-entry indices");
    require(std::strcmp(fm_fix::statParRatioLabelForRaw(0), "1/64") == 0
                && std::strcmp(fm_fix::statParRatioLabelForRaw(48), "5/32") == 0
                && std::strcmp(fm_fix::statParRatioLabelForRaw(60), "1/2") == 0
                && std::strcmp(fm_fix::statParRatioLabelForRaw(80), "1") == 0
                && std::strcmp(fm_fix::statParRatioLabelForRaw(127), "4") == 0,
            "STAT/PAR literal labels include observed non-monotonic 5/32 slot");
    require(closeEnough(fm_fix::statParRatioForRaw(60), 0.5f)
                && closeEnough(fm_fix::statParRatioForRaw(80), 1.0f)
                && fm_fix::statParRatioWordForRaw(48) == 0x014000u
                && static_cast<uint32_t>(std::lround(fm_fix::statParRatioForRaw(48) * 524288.0f))
                    == fm_fix::statParRatioWordForRaw(48),
            "STAT/PAR factory defaults and MNM float value derive from one exact word table");

    // Dynamic observed control laws: direct K/64 and squared (K/64)^2.
    require(closeEnough(fm_fix::dynRatio1ForRaw(0), 0.0f)
                && closeEnough(fm_fix::dynRatio1ForRaw(1), 1.0f / 64.0f)
                && closeEnough(fm_fix::dynRatio1ForRaw(64), 1.0f)
                && closeEnough(fm_fix::dynRatio1ForRaw(127), 127.0f / 64.0f),
            "DYN 1FRQ follows measured direct K/64 control law");
    require(closeEnough(fm_fix::dynRatio2ForRaw(8), 1.0f / 64.0f)
                && closeEnough(fm_fix::dynRatio2ForRaw(16), 1.0f / 16.0f)
                && closeEnough(fm_fix::dynRatio2ForRaw(32), 0.25f)
                && closeEnough(fm_fix::dynRatio2ForRaw(64), 1.0f)
                && closeEnough(fm_fix::dynRatio2ForRaw(127), 127.0f * 127.0f / 4096.0f),
            "DYN 2FRQ follows measured squared (K/64)^2 control law");

    require(closeEnough(fm_fix::tuneSemitonesForRaw(0), -2.0f)
                && closeEnough(fm_fix::tuneSemitonesForRaw(64), 0.0f)
                && closeEnough(fm_fix::tuneSemitonesForRaw(127), 63.0f / 32.0f),
            "FIX TUNE endpoints are -64=-2 semitones, centre=0, +63=+1.96875");

    require(fm_fix::oldFixParallelTuneReadoutForRaw(0) == -64
                && fm_fix::oldFixParallelTuneReadoutForRaw(64) == 0
                && fm_fix::oldFixParallelTuneReadoutForRaw(127) == 63,
            "OLD FIX PAR TUNE LCD retains the centred raw -64..+63 control readout");

    // Retained NEW and NEW FIX have different C++ namespaces/memory objects:
    // no pointer or register state is shared with retained NEW.
    fmnew::FmMem retainedTableMemory{};
    fmnewfix::FmMem fixTableMemory{};
    fixTableMemory.setRatioTable(fm_fix::kStatParRatioWord.data());
    require(retainedTableMemory.tables(0x141A80u) == 0x004000u
                && fixTableMemory.tables(0x141A80u) == 0x002000u
                && fixTableMemory.tables(0x141A89u) == 0x014000u,
            "NEW FIX owns a separately namespaced STAT/PAR table; retained imported table remains default");

    const std::array<float, 8> stat{60, 64, 80, 30, 80, 64, 98, 64};
    const std::array<float, 8> par{60, 64, 80, 64, 102, 80, 98, 64};
    const std::array<float, 8> dyn{16, 64, 64, 64, 74, 80, 30, 64};
    {
        // OLD FIX owns dedicated OLD renderer classes.  STAT is restored from
        // the archived OLD STATIC topology; it must not be an MNM-core alias.
        fm_fix::OldFixStaticCore oldFixStat;
        fm_fix::MnmFixCore mnmFixStat;
        oldFixStat.reset(44100.0); mnmFixStat.reset(44100.0);
        oldFixStat.setParameters(60,64,80,30,80,64,98,64);
        mnmFixStat.setParameters(fm_fix::MnmFixKind::Stat,stat);
        oldFixStat.noteOn(60); mnmFixStat.noteOn(60.0f,1.0f);
        float oldL[128]{},oldR[128]{},mnmL[128]{};
        oldFixStat.processStereo(oldL,oldR,128); mnmFixStat.processBlock(mnmL,128);
        float statDifference=0.0f; bool statFinite=true;
        for(int i=0;i<128;++i){statDifference+=std::abs(oldL[i]-mnmL[i]);statFinite=statFinite&&std::isfinite(oldL[i])&&std::isfinite(oldR[i]);}
        require(statFinite&&statDifference>1.0e-3f,
            "OLD FIX STAT uses its dedicated archived OLD topology, never an MNM alias");

        // OLD FIX PAR keeps the OLD PAR class and changes only measured controls.
        MonomachineFmParallel retainedOldPar;
        fm_fix::OldFixParallelCore oldFixPar;
        retainedOldPar.reset(44100.0); oldFixPar.reset(44100.0);
        retainedOldPar.setParameters(60,64,80,64,102,80,98,64);
        oldFixPar.setParameters(60,64,80,64,102,80,98,64);
        retainedOldPar.noteOn(60); oldFixPar.noteOn(60);
        float fixL[128]{},fixR[128]{};
        retainedOldPar.processStereo(oldL,oldR,128); oldFixPar.processStereo(fixL,fixR,128);
        float parDifference=0.0f; bool parFinite=true;
        for(int i=0;i<128;++i){parDifference+=std::abs(oldL[i]-fixL[i]);parFinite=parFinite&&std::isfinite(fixL[i])&&std::isfinite(fixR[i]);}
        require(parFinite&&parDifference>1.0e-3f,
            "OLD FIX PAR retains OLD topology while its measured controls differ from retained OLD");

        MonomachineFmDynamic retainedOldDyn;
        fm_fix::OldFixDynamicCore oldFixDyn;
        retainedOldDyn.reset(44100.0); oldFixDyn.reset(44100.0);
        retainedOldDyn.setParameters(16,64,64,64,74,80,30,64);
        oldFixDyn.setParameters(16,64,64,64,74,80,30,64);
        retainedOldDyn.noteOn(60); oldFixDyn.noteOn(60);
        std::fill(oldL,oldL+128,0.0f);std::fill(oldR,oldR+128,0.0f);std::fill(fixL,fixL+128,0.0f);std::fill(fixR,fixR+128,0.0f);
        retainedOldDyn.processStereo(oldL,oldR,128); oldFixDyn.processStereo(fixL,fixR,128);
        float dynDifference=0.0f; bool dynFinite=true;
        for(int i=0;i<128;++i){dynDifference+=std::abs(oldL[i]-fixL[i]);dynFinite=dynFinite&&std::isfinite(fixL[i])&&std::isfinite(fixR[i]);}
        require(dynFinite&&dynDifference>1.0e-3f,
            "OLD FIX DYN retains OLD topology while its measured controls differ from retained OLD");

        // Step an ENV word during a sounding note while a matched reference
        // remains at its old value. The first changed sample is constrained by
        // the 12 ms one-pole rather than a discontinuous FM-index jump.
        const auto inducedFirstSampleDelta=[](int envIndex) {
            fm_fix::OldFixParallelCore changed, reference;
            const uint8_t base[]{60,0,80,0,102,0,98,64};
            changed.reset(44100.0); reference.reset(44100.0);
            changed.setParameters(base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
            reference.setParameters(base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
            changed.noteOn(60); reference.noteOn(60);
            float warmL[512]{},warmR[512]{};
            changed.processStereo(warmL,warmR,512); reference.processStereo(warmL,warmR,512);
            uint8_t stepped[8]; std::copy(base,base+8,stepped); stepped[envIndex]=127;
            changed.setParameters(stepped[0],stepped[1],stepped[2],stepped[3],stepped[4],stepped[5],stepped[6],stepped[7]);
            reference.setParameters(base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
            float changedL[1]{},changedR[1]{},referenceL[1]{},referenceR[1]{};
            changed.processStereo(changedL,changedR,1); reference.processStereo(referenceL,referenceR,1);
            return std::max(std::abs(changedL[0]-referenceL[0]),std::abs(changedR[0]-referenceR[0]));
        };
        const float env1FirstDelta=inducedFirstSampleDelta(1);
        const float env2FirstDelta=inducedFirstSampleDelta(3);
        const float env3FirstDelta=inducedFirstSampleDelta(5);
        require(env1FirstDelta<0.02f&&env2FirstDelta<0.02f&&env3FirstDelta<0.02f,
            "OLD FIX PAR 1ENV/2ENV/3ENV writes are slewed without a one-sample FM click");
    }
    require(mnmHash(mnm::FmKind::Stat, stat) != mnmFixHash(fm_fix::MnmFixKind::Stat, stat),
            "MNM FIX STAT uses a separate opt-in core with measured frequency law");
    require(mnmHash(mnm::FmKind::Par, par) != mnmFixHash(fm_fix::MnmFixKind::Par, par),
            "MNM FIX PAR uses a separate opt-in core with measured frequency law");
    require(mnmHash(mnm::FmKind::Dyn, dyn) != mnmFixHash(fm_fix::MnmFixKind::Dyn, dyn),
            "MNM FIX DYN uses a separate opt-in core with measured laws");
    require(exactHash(fm_new::FmExactKind::Stat, stat) != exactFixHash(fm_new::FmExactKind::Stat, stat),
            "NEW FIX STAT uses a separate measured-table core while retained NEW remains separate");
    require(exactHash(fm_new::FmExactKind::Par, par) != exactFixHash(fm_new::FmExactKind::Par, par),
            "NEW FIX PAR uses a separate measured-table core while retained NEW remains separate");
    require(exactHash(fm_new::FmExactKind::Stat, stat) == exactHash(fm_new::FmExactKind::Stat, stat)
                && exactFixHash(fm_new::FmExactKind::Stat, stat) == exactFixHash(fm_new::FmExactKind::Stat, stat),
            "NEW/NEW FIX state objects are explicitly separate and deterministic");

    if (failures != 0) {
        std::fprintf(stderr, "FM_FIX_MODE_TESTS FAIL: %d check(s)\n", failures);
        return 1;
    }
    std::puts("FM_FIX_MODE_TESTS PASS");
    return 0;
}
