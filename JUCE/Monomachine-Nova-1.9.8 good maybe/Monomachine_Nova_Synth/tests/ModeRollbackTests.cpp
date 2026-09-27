// Regression guard for the 1.9.8 appended NEW delay selector and migrations.
#include "models/DspModes.hpp"

#include <cstdio>
#include <cstring>

int main()
{
    using namespace monomachine;
    for (const int section : { DspSynt, DspFilter, DspDist }) {
        if (dspSectionModeCount(section) != 2
            || std::strcmp(dspSectionParameterChoices(section), "mnm|old") != 0
            || dspModeAllowedForSection(section, dspModeNew)) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: section %d accepted DLY NEW\n", section);
            return 1;
        }
    }
    if (dspSectionModeCount(DspDelay) != 3
        || std::strcmp(dspSectionParameterChoices(DspDelay), "mnm|old|new") != 0
        || !dspModeAllowedForSection(DspDelay, dspModeNew)
        || dspModeIndexByName("new") != dspModeNew
        || dspModeIndexByName("fma") != -1
        || dspModeLegacyIndexToCurrent(2) != dspModeMnm
        || dspModeLegacyIndexToCurrent(3) != dspModeMnm
        || kDspModeSchemaVersion != 23) {
        std::fputs("MODE_ROLLBACK FAIL: appended DLY NEW compatibility contract\n", stderr);
        return 1;
    }
    std::puts("MODE_ROLLBACK PASS: DLY mnm|old|new with stable legacy indices");
    return 0;
}
