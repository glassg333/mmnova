// Regression guard for stable mnm|old|new IDs, retained DLY NEW, and the
// schema-30 FM+ m8/m9/m10-only appended MNM FIX / NEW FIX candidates.
#include "models/DspModes.hpp"

#include <cstdio>
#include <cstring>

int main()
{
    using namespace monomachine;
    // The generic SYNT registry stays two-choice so NEW/FIX cannot escape
    // through generic section code.  FILT/DIST remain two-choice as before.
    for (const int section : { DspSynt, DspFilter, DspDist }) {
        if (dspSectionModeCount(section) != 2
            || std::strcmp(dspSectionParameterChoices(section), "mnm|old") != 0
            || dspModeAllowedForSection(section, dspModeNew)
            || dspModeAllowedForSection(section, dspModeMnmFix)
            || dspModeAllowedForSection(section, dspModeNewFix)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: section %d accepted unrelated NEW/FIX\n", section);
            return 1;
        }
    }
    if (dspSectionModeCount(DspDelay) != 3
        || std::strcmp(dspSectionParameterChoices(DspDelay), "mnm|old|new") != 0
        || !dspModeAllowedForSection(DspDelay, dspModeNew)
        || dspModeAllowedForSection(DspDelay, dspModeMnmFix)
        || dspModeAllowedForSection(DspDelay, dspModeNewFix)
        || dspModeIndexByName("new") != dspModeNew
        || dspModeIndexByName("mnm fix") != dspModeMnmFix
        || dspModeIndexByName("new fix") != dspModeNewFix
        || dspModeIndexByName("fma") != -1
        || dspModeLegacyIndexToCurrent(2) != dspModeMnm
        || dspModeLegacyIndexToCurrent(3) != dspModeMnm
        || dspModeLegacyIndexToCurrent(4) != dspModeMnm
        || kDspModeSchemaVersion != 30) {
        std::fputs("MODE_ROLLBACK FAIL: retained DLY NEW/schema compatibility\n", stderr);
        return 1;
    }
    for (const int machine : {8, 9, 10}) {
        if (!dspSyntSupportsNewForMachine(machine)
            || !dspSyntSupportsFixForMachine(machine)
            || dspSyntModeCountForMachine(machine) != 5
            || std::strcmp(dspSyntModeChoicesForMachine(machine), "mnm|old|new|mnm fix|new fix") != 0
            || !dspSyntModeAllowedForMachine(machine, dspModeNew)
            || !dspSyntModeAllowedForMachine(machine, dspModeMnmFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeNewFix)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: FM machine %d lacks isolated NEW/FIX\n", machine);
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
            || dspSyntModeAllowedForMachine(machine, dspModeNewFix)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: non-FM machine %d accepted NEW/FIX\n", machine);
            return 1;
        }
    }
    std::puts("MODE_ROLLBACK PASS: stable mnm|old|new plus FM-only appended MNM FIX/NEW FIX");
    return 0;
}
