// Regression guard for stable mnm|old|new IDs, retained DLY NEW, package-4
// FM+ m8/m9/m10 candidates, and schema-34 append-only Fix-5 PITCH+ENV FULL.
#include "models/DspModes.hpp"

#include <cstdio>
#include <cstring>

int main()
{
    using namespace monomachine;
    // Generic section paths stay two-choice so appended FM candidates cannot
    // escape through generic SYNT/FILT/DIST dispatch.
    for (const int section : { DspSynt, DspFilter, DspDist }) {
        if (dspSectionModeCount(section) != 2
            || std::strcmp(dspSectionParameterChoices(section), "mnm|old") != 0
            || dspModeAllowedForSection(section, dspModeNew)
            || dspModeAllowedForSection(section, dspModeMnmFix)
            || dspModeAllowedForSection(section, dspModeNewFix)
            || dspModeAllowedForSection(section, dspModeOldFix)
            || dspModeAllowedForSection(section, dspModeMnmFrqEnvFix)
            || dspModeAllowedForSection(section, dspModeTry4)
            || dspModeAllowedForSection(section, dspModeMnmFix5Full)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: section %d accepted unrelated FM candidate\n", section);
            return 1;
        }
    }
    if (dspSectionModeCount(DspDelay) != 3
        || std::strcmp(dspSectionParameterChoices(DspDelay), "mnm|old|new") != 0
        || !dspModeAllowedForSection(DspDelay, dspModeNew)
        || dspModeAllowedForSection(DspDelay, dspModeMnmFix)
        || dspModeAllowedForSection(DspDelay, dspModeNewFix)
        || dspModeAllowedForSection(DspDelay, dspModeOldFix)
        || dspModeAllowedForSection(DspDelay, dspModeMnmFrqEnvFix)
        || dspModeAllowedForSection(DspDelay, dspModeTry4)
        || dspModeAllowedForSection(DspDelay, dspModeMnmFix5Full)
        || dspModeIndexByName("new") != dspModeNew
        || dspModeIndexByName("mnm fix") != dspModeMnmFix
        || dspModeIndexByName("new fix") != dspModeNewFix
        || dspModeIndexByName("old fix") != dspModeOldFix
        || dspModeIndexByName("mnm frq env fix") != dspModeMnmFrqEnvFix
        || dspModeIndexByName("try4") != dspModeTry4
        || dspModeIndexByName("fix5 pitch+env full") != dspModeMnmFix5Full
        || dspModeIndexByName("fma") != -1
        || dspModeLegacyIndexToCurrent(2) != dspModeMnm
        || dspModeLegacyIndexToCurrent(3) != dspModeMnm
        || dspModeLegacyIndexToCurrent(4) != dspModeMnm
        || dspModeLegacyIndexToCurrent(5) != dspModeMnm
        || kDspModeSchemaVersion != 34) {
        std::fputs("MODE_ROLLBACK FAIL: retained DLY NEW/schema compatibility\n", stderr);
        return 1;
    }
    for (const int machine : {8, 9, 10}) {
        const char* expected=machine==8
            ? "mnm FREQ|old FREQ|new FREQ|mnm fix BPM|new fix BPM|old fix BPM|mnm frq env fix|try4|fix5 BPM pitch+env full"
            : "mnm|old|new|mnm fix|new fix|old fix|mnm frq env fix|try4|fix5 pitch+env full";
        if (!dspSyntSupportsNewForMachine(machine)
            || !dspSyntSupportsFixForMachine(machine)
            || dspSyntModeCountForMachine(machine) != 9
            || std::strcmp(dspSyntModeChoicesForMachine(machine), expected) != 0
            || !dspSyntModeAllowedForMachine(machine, dspModeNew)
            || !dspSyntModeAllowedForMachine(machine, dspModeMnmFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeNewFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeOldFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeMnmFrqEnvFix)
            || !dspSyntModeAllowedForMachine(machine, dspModeTry4)
            || !dspSyntModeAllowedForMachine(machine, dspModeMnmFix5Full)
            || dspSyntModeAllowedForSchema(machine, dspModeMnmFix5Full, 33)
            || !dspSyntModeAllowedForSchema(machine, dspModeMnmFix5Full, 34)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: FM machine %d lacks isolated appended routes\n", machine);
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
            || dspSyntModeAllowedForMachine(machine, dspModeMnmFrqEnvFix)
            || dspSyntModeAllowedForMachine(machine, dspModeTry4)
            || dspSyntModeAllowedForMachine(machine, dspModeMnmFix5Full)
            || dspSyntModeAllowedForSchema(machine, dspModeMnmFix5Full, 34)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: non-FM machine %d accepted FM candidate\n", machine);
            return 1;
        }
    }
    std::puts("MODE_ROLLBACK PASS: stable IDs 0..7 plus FM-only Fix-5 append ID 8");
    return 0;
}
