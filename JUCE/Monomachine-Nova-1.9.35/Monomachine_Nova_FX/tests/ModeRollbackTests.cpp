// Regression guard for the six retained FM+ MODE SYNT IDs, DLY NEW, and
// schema-44 BASE migration while retaining old experimental raw IDs 6..8 retirement.
#include "models/DspModes.hpp"

#include <cstdio>
#include <cstring>

int main()
{
    using namespace monomachine;

    // Generic paths stay isolated: the six FM+ choices cannot escape through
    // generic SYNT/FILT/DIST dispatch.
    for (const int section : { DspSynt, DspFilter, DspDist }) {
        if (dspSectionModeCount(section) != 2
            || std::strcmp(dspSectionParameterChoices(section), "mnm|old") != 0
            || dspModeAllowedForSection(section, dspModeNew)
            || dspModeAllowedForSection(section, dspModeMnmFix)
            || dspModeAllowedForSection(section, dspModeNewFix)
            || dspModeAllowedForSection(section, dspModeOldFix)
            || dspModeAllowedForSection(section, kRetiredSyntRawId6)
            || dspModeAllowedForSection(section, kRetiredSyntRawId7)
            || dspModeAllowedForSection(section, kRetiredSyntRawId8)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: section %d accepted unrelated FM renderer\n", section);
            return 1;
        }
    }

    if (dspModeCount != 6
        || kDspModeSchemaVersion != 44
        || dspSectionModeCount(DspDelay) != 3
        || std::strcmp(dspSectionParameterChoices(DspDelay), "mnm|old|new") != 0
        || !dspModeAllowedForSection(DspDelay, dspModeNew)
        || dspModeAllowedForSection(DspDelay, dspModeMnmFix)
        || dspModeAllowedForSection(DspDelay, kRetiredSyntRawId8)
        || dspModeIndexByName("mnm frq") != dspModeMnm
        || dspModeIndexByName("old frq") != dspModeOld
        || dspModeIndexByName("new frq") != dspModeNew
        || dspModeIndexByName("mnm bpm") != dspModeMnmFix
        || dspModeIndexByName("new bpm") != dspModeNewFix
        || dspModeIndexByName("old bpm") != dspModeOldFix
        || dspModeIndexByName("mnm frq env fix") != -1
        || dspModeIndexByName("try4") != -1
        || dspModeIndexByName("fix5 pitch+env full") != -1
        || dspModeIndexByName("fma") != -1
        || dspModeLegacyIndexToCurrent(2) != dspModeMnm
        || dspModeLegacyIndexToCurrent(3) != dspModeMnm
        || dspModeLegacyIndexToCurrent(4) != dspModeMnm
        || dspModeLegacyIndexToCurrent(5) != dspModeMnm) {
        std::fputs("MODE_ROLLBACK FAIL: retained labels/IDs or DLY compatibility\n", stderr);
        return 1;
    }

    for (const int machine : {8, 9, 10}) {
        if (!dspSyntSupportsNewForMachine(machine)
            || !dspSyntSupportsFixForMachine(machine)
            || dspSyntModeCountForMachine(machine) != 6
            || std::strcmp(dspSyntModeChoicesForMachine(machine),
                           "mnm frq|old frq|new frq|mnm bpm|new bpm|old bpm") != 0
            || !dspSyntModeAllowedForMachine(machine, dspModeNew)
            || !dspSyntModeAllowedForMachine(machine, dspModeMnmFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeNewFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeOldFix)
            || dspSyntModeAllowedForMachine(machine, kRetiredSyntRawId6)
            || dspSyntModeAllowedForMachine(machine, kRetiredSyntRawId7)
            || dspSyntModeAllowedForMachine(machine, kRetiredSyntRawId8)
            || !dspSyntModeAllowedForSchema(machine, dspModeOldFix, 31)
            || dspSyntModeAllowedForSchema(machine, kRetiredSyntRawId6, 32)
            || dspSyntModeAllowedForSchema(machine, kRetiredSyntRawId7, 33)
            || dspSyntModeAllowedForSchema(machine, kRetiredSyntRawId8, 34)
            || dspSyntModeAllowedForSchema(machine, kRetiredSyntRawId8, 36)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: FM machine %d registry/retirement mismatch\n", machine);
            return 1;
        }
    }

    for (const int machine : {0, 7, 11, 15, 22}) {
        if (dspSyntSupportsNewForMachine(machine)
            || dspSyntSupportsFixForMachine(machine)
            || dspSyntModeCountForMachine(machine) != 2
            || std::strcmp(dspSyntModeChoicesForMachine(machine), "mnm|old") != 0
            || dspSyntModeAllowedForMachine(machine, dspModeNew)
            || dspSyntModeAllowedForMachine(machine, dspModeMnmFix)
            || dspSyntModeAllowedForMachine(machine, dspModeNewFix)
            || dspSyntModeAllowedForMachine(machine, dspModeOldFix)
            || dspSyntModeAllowedForMachine(machine, kRetiredSyntRawId6)
            || dspSyntModeAllowedForMachine(machine, kRetiredSyntRawId7)
            || dspSyntModeAllowedForMachine(machine, kRetiredSyntRawId8)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: non-FM machine %d accepted FM renderer\n", machine);
            return 1;
        }
    }

    std::puts("MODE_ROLLBACK PASS: retained IDs 0..5; raw IDs 6..8 are retired");
    return 0;
}
