// Stable direct-LFO PAGE/DEST compatibility and six-LFO-side regression.
#include "NovaData.h"

#include <cstdio>
#include <cstdlib>

namespace {
void require(bool condition, const char* message)
{
    if (! condition) {
        std::fprintf(stderr, "LFOMAPPING FAIL: %s\n", message);
        std::exit(1);
    }
}
}

int main()
{
    using namespace nova;
    require(kLfoPageChoiceCount == 21, "PAGE must retain 21 raw choices");
    require(kLfoDestinationCount == 8, "DEST must retain eight raw choices");

    // PAGE zero is pitch on both sides, but P2 labels its legacy value where it
    // belongs in the mirror hierarchy.
    require(lfoDirectTarget(false, 0, 7) == kPitchMatrixTarget, "P1 pitch mapping changed");
    require(lfoDirectTarget(true, 0, 0) == kPitchMatrixTarget, "P2 pitch mapping changed");
    require(lfoPageName(true, 0) == "P1 PITCH", "P2 PAGE zero label is not mirrored P1 pitch");

    // Each raw page maps a complete contiguous block of eight real targets.
    for (int source = 0; source < 2; ++source) {
        for (int page = 1; page < kLfoPageChoiceCount; ++page) {
            for (int dest = 0; dest < kLfoDestinationCount; ++dest) {
                const int target = lfoDirectTarget(source != 0, page, dest);
                int recovered = -1;
                require(lfoPageForDirectTarget(source != 0, target, recovered),
                        "real direct target could not be inverted");
                require(recovered == page, "inverse PAGE mapping changed raw compatibility");
            }
        }
    }

    require(lfoDirectTarget(false, 8, 0) == 188, "P1 LFO4 block offset changed");
    require(lfoDirectTarget(false, 18, 0) == 212, "P2 LFO4 block offset changed");
    require(lfoDirectTarget(true, 8, 0) == 212, "mirrored P2 LFO4 block offset changed");
    require(lfoDirectTarget(true, 18, 0) == 188, "mirrored P1 LFO4 block offset changed");

    require(!lfoDirectTargetIsContinuousForCurrentP1Machine(7, 2), "BBOX SLOT must not be direct-LFO selectable");
    require(!lfoDirectTargetIsContinuousForCurrentP1Machine(7, 3), "BBOX RAND must not be direct-LFO selectable");
    require(!lfoDirectTargetIsContinuousForCurrentP1Machine(7, 6), "BBOX CHRM must not be direct-LFO selectable");
    require(lfoDirectTargetIsContinuousForCurrentP1Machine(7, 1), "normal BBOX parameter was incorrectly excluded");

    std::puts("LFOMAPPING 21 PAGE values, mirrored P1/P2 six-LFO blocks, BBOX exclusion PASS");
    return 0;
}
