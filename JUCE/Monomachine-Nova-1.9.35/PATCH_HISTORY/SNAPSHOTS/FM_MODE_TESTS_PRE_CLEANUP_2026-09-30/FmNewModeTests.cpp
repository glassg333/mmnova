// Focused JUCE-free regression checks for MODE SYNT = new on FM+ m8/m9/m10.
// The legacy mnm fingerprints below were recorded before this isolated import.
#include "dsp/fm_new/FmExactNew.hpp"
#include "dsp/mnm/MnmFm.hpp"
#include "models/DspModes.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

namespace {
int failures = 0;

void require(bool condition, const char* what)
{
    std::printf("%-72s %s\n", what, condition ? "OK" : "FAIL");
    if (!condition) ++failures;
}

uint32_t floatBits(float value)
{
    uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

uint64_t fnv1aWord(uint64_t hash, uint32_t word)
{
    for (int byte = 0; byte < 4; ++byte) {
        hash ^= (word >> (byte * 8)) & 0xffu;
        hash *= 1099511628211ull;
    }
    return hash;
}

uint64_t legacyMnmFingerprint(monomachine::mnm::FmKind kind, const std::array<float, 8>& parameters)
{
    monomachine::mnm::FmCore core;
    core.reset(44100.0);
    core.setParameters(kind, parameters);
    core.noteOn(60.0f, 1.0f);
    core.setPitchMod(1.25f);

    uint64_t hash = 1469598103934665603ull;
    float block[monomachine::mnm::kBlock]{};
    for (int blockIndex = 0; blockIndex < 23; ++blockIndex) {
        core.processBlock(block, monomachine::mnm::kBlock);
        for (float sample : block)
            hash = fnv1aWord(hash, floatBits(sample));
    }
    return hash;
}

bool finiteAndAudible(monomachine::fm_new::FmExactKind kind, const std::array<float, 8>& parameters)
{
    monomachine::fm_new::FmExactCore core;
    core.reset(44100.0);
    core.noteOn(60.0f);
    core.setParameters(kind, parameters);
    core.setPitchWordOverride(11776u, true);
    float output[96]{};
    core.processBlock(output, 96);
    float energy = 0.0f;
    for (float sample : output) {
        if (!std::isfinite(sample)) return false;
        energy += sample * sample;
    }
    return energy > 1.0e-5f;
}
} // namespace

int main()
{
    using namespace monomachine;

    // The section-wide SYNT list remains legacy-safe.  Only the three
    // per-machine FM+ parameters append NEW and the later FIX candidates.
    require(dspSectionModeCount(DspSynt) == 2
                && std::strcmp(dspSectionParameterChoices(DspSynt), "mnm|old") == 0
                && !dspModeAllowedForSection(DspSynt, dspModeNew)
                && !dspModeAllowedForSection(DspSynt, dspModeMnmFix)
                && !dspModeAllowedForSection(DspSynt, dspModeNewFix)
                && !dspModeAllowedForSection(DspSynt, dspModeOldFix)
                && !dspModeAllowedForSection(DspSynt, dspModeMnmFrqEnvFix)
                && !dspModeAllowedForSection(DspSynt, dspModeTry4)
                && !dspModeAllowedForSection(DspSynt, dspModeMnmFix5Full),
            "generic SYNT remains mnm|old; NEW/FIX cannot leak to every machine");
    for (int machine : {8, 9, 10}) {
        const char* expectedChoices=machine==8
            ? "mnm FREQ|old FREQ|new FREQ|mnm fix BPM|new fix BPM|old fix BPM|mnm frq env fix|try4|fix5 BPM pitch+env full"
            : "mnm|old|new|mnm fix|new fix|old fix|mnm frq env fix|try4|fix5 pitch+env full";
        require(dspSyntSupportsNewForMachine(machine)
                    && dspSyntSupportsFixForMachine(machine)
                    && dspSyntModeCountForMachine(machine) == 9
                    && std::strcmp(dspSyntModeChoicesForMachine(machine), expectedChoices) == 0
                    && dspSyntModeAllowedForMachine(machine, dspModeNew)
                    && dspSyntModeAllowedForMachine(machine, dspModeMnmFix)
                    && dspSyntModeAllowedForMachine(machine, dspModeNewFix)
                    && dspSyntModeAllowedForMachine(machine, dspModeOldFix)
                    && dspSyntModeAllowedForMachine(machine, dspModeMnmFrqEnvFix)
                    && dspSyntModeAllowedForMachine(machine, dspModeTry4)
                    && dspSyntModeAllowedForMachine(machine, dspModeMnmFix5Full),
                "m8/m9/m10 each expose isolated NEW/FIX, package-4 routes, and independent Fix-5");
    }
    for (int machine : {0, 1, 7, 11, 15, 22}) {
        require(!dspSyntSupportsNewForMachine(machine)
                    && !dspSyntSupportsFixForMachine(machine)
                    && dspSyntModeCountForMachine(machine) == 2
                    && std::strcmp(dspSyntModeChoicesForMachine(machine), "mnm|old") == 0
                    && !dspSyntModeAllowedForMachine(machine, dspModeNew)
                    && !dspSyntModeAllowedForMachine(machine, dspModeMnmFix)
                    && !dspSyntModeAllowedForMachine(machine, dspModeNewFix)
                    && !dspSyntModeAllowedForMachine(machine, dspModeOldFix)
                    && !dspSyntModeAllowedForMachine(machine, dspModeMnmFrqEnvFix)
                    && !dspSyntModeAllowedForMachine(machine, dspModeTry4)
                    && !dspSyntModeAllowedForMachine(machine, dspModeMnmFix5Full),
                "non-FM machine retains its exact mnm|old SYNT contract");
    }
    require(dspSectionModeCount(DspDelay) == 3 && dspModeAllowedForSection(DspDelay, dspModeNew),
            "pre-existing DLY NEW remains independently selectable");

    // Low-level STAT proof anchor from the reviewed supplied vector harness:
    // all knobs=64, pitch word=11776 -> duplicate Q23 output words.
    {
        fmnew::MnmFmStat stat;
        stat.init();
        const uint32_t knobs[8]{64, 64, 64, 64, 64, 64, 64, 64};
        uint32_t raw[32]{};
        stat.conf(knobs);
        stat.proc(11776u, raw);
        constexpr uint32_t expected[]{853u, 853u, 26257u, 26257u, 143842u, 143842u};
        bool exact = true;
        for (int index = 0; index < 6; ++index) exact = exact && raw[index] == expected[index];
        require(exact, "FM NEW STAT raw core retains supplied-vector opening words");
    }

    // Wrapper FIFO must make host callback chunking transparent.
    {
        const std::array<float, 8> parameters{64, 64, 64, 64, 64, 64, 64, 64};
        fm_new::FmExactCore contiguous;
        fm_new::FmExactCore split;
        contiguous.reset(44100.0); split.reset(44100.0);
        contiguous.noteOn(60.0f); split.noteOn(60.0f);
        contiguous.setParameters(fm_new::FmExactKind::Stat, parameters);
        split.setParameters(fm_new::FmExactKind::Stat, parameters);
        contiguous.setPitchWordOverride(11776u, true);
        split.setPitchWordOverride(11776u, true);
        float whole[40]{};
        float pieces[40]{};
        contiguous.processBlock(whole, 40);
        split.processBlock(pieces, 5);
        split.processBlock(pieces + 5, 17);
        split.processBlock(pieces + 22, 18);
        require(std::memcmp(whole, pieces, sizeof whole) == 0,
                "FM NEW wrapper output is invariant to 5/17/18 host callback splits");
    }

    require(finiteAndAudible(fm_new::FmExactKind::Par, {16, 64, 32, 64, 48, 64, 98, 64}),
            "FM NEW PAR renders finite non-silent exact-core output");
    require(finiteAndAudible(fm_new::FmExactKind::Dyn, {16, 0, 64, 0, 32, 80, 30, 64}),
            "FM NEW DYN renders finite non-silent exact-core output");

    // MNM is not wrapped or replaced.  These fixed pre-import fingerprints
    // make an unintended edit to Source/dsp/mnm/MnmFm.hpp observable.
    require(legacyMnmFingerprint(mnm::FmKind::Stat, {64, 64, 80, 30, 80, 64, 98, 64})
                == 0xe0551d911c8adbffull,
            "legacy MNM STAT fingerprint remains unchanged");
    require(legacyMnmFingerprint(mnm::FmKind::Par, {16, 64, 32, 64, 48, 64, 98, 64})
                == 0x61adea6373f3da33ull,
            "legacy MNM PAR fingerprint remains unchanged");
    require(legacyMnmFingerprint(mnm::FmKind::Dyn, {16, 0, 64, 0, 32, 80, 30, 64})
                == 0xf513af21c20af423ull,
            "legacy MNM DYN fingerprint remains unchanged");

    if (failures != 0) {
        std::fprintf(stderr, "FM_NEW_MODE_TESTS FAIL: %d check(s)\n", failures);
        return 1;
    }
    std::puts("FM_NEW_MODE_TESTS PASS");
    return 0;
}
