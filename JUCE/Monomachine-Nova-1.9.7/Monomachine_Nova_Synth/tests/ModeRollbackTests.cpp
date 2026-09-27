// Regression guard for the 1.9.4 DSP-selector rollback and schema migrations.
#include "models/DspModes.hpp"

#include <cstdio>
#include <cstring>

int main()
{
    using namespace monomachine;
    const int sections[] = { DspSynt, DspFilter, DspDist, DspDelay };
    for (const int section : sections) {
        if (dspSectionModeCount(section) != 2
            || std::strcmp(dspSectionParameterChoices(section), "mnm|old") != 0) {
            std::fprintf(stderr, "MODE_ROLLBACK FAIL: section %d still exposes an experimental selector\n", section);
            return 1;
        }
    }
    if (dspModeIndexByName("fma") != -1 || dspModeIndexByName("new") != -1
        || dspModeLegacyIndexToCurrent(2) != dspModeMnm
        || dspModeLegacyIndexToCurrent(3) != dspModeMnm
        || kDspModeSchemaVersion != 22) {
        std::fputs("MODE_ROLLBACK FAIL: withdrawn mode migration contract\n", stderr);
        return 1;
    }
    std::puts("MODE_ROLLBACK PASS");
    return 0;
}
