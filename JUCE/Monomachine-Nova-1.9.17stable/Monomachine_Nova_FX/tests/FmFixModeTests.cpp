// JUCE-free regression checks for the opt-in FM+ MNM FIX / NEW FIX profiles.
// Existing mnm/old/new paths are covered separately and must remain unchanged.
#include "dsp/fm_fix/FmFixTables.hpp"
#include "dsp/fm_fix/MnmFmFix.hpp"
#include "dsp/fm_new/FmExactNew.hpp"
#include "dsp/fm_new_fix/FmExactNewFix.hpp"
#include "dsp/mnm/MnmFm.hpp"
#include "models/DspModes.hpp"
#include "models/machine_definitions.hpp"

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

    require(kDspModeSchemaVersion == 30
                && dspModeMnm == 0 && dspModeOld == 1 && dspModeNew == 2
                && dspModeMnmFix == 3 && dspModeNewFix == 4,
            "schema 30 appends FIX values without changing mnm/old/new IDs");
    require(!dspSyntModeUsesMeasuredFix(dspModeMnm)
                && !dspSyntModeUsesMeasuredFix(dspModeOld)
                && !dspSyntModeUsesMeasuredFix(dspModeNew)
                && dspSyntModeUsesMeasuredFix(dspModeMnmFix)
                && dspSyntModeUsesMeasuredFix(dspModeNewFix),
            "measured table/readout profile is confined to appended FIX mode IDs");

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
