#include "dsp/mnm/MnmFm.hpp"
#include "dsp/monomachine_fm_par.hpp"
#include <cstdio>
int main() {
    using namespace monomachine::mnm;
    // knob -> step -> ratio across the whole travel
    for (int k : {0, 4, 5, 10, 11, 16, 21, 26, 32, 37, 42, 48, 53, 58, 64, 69, 74, 79, 85, 90, 95, 101, 106, 111, 116, 122, 127}) {
        int s = fmStepIndex((float)k);
        printf("knob %3d -> step %2d -> ratio %9.6f\n", k, s, kFmRatio[s]);
    }
    // DYN linear law
    FmCore dyn; dyn.reset(44100.0);
    std::array<float,8> pd = {0, 64, 64, 64, 64, 64, 30, 64};
    dyn.setParameters(FmKind::Dyn, pd);   // 1FRQ = 0 -> ratio must be 0.0
    printf("\nDYN 1FRQ=0 -> continuous ratio %.4f (must be 0.0)\n", 0.0f / 16.0f);
    std::array<float,8> pd2 = {16, 64, 64, 64, 64, 64, 30, 64};
    dyn.setParameters(FmKind::Dyn, pd2);
    printf("DYN 1FRQ=16 -> ratio %.4f (must be 1.0)\n", 16.0f / 16.0f);
    // old PAR engine compiles and maps knobs through the measured step map
    monomachine::MonomachineFmParallel par; par.reset(44100.0);
    par.setParameters(0, 64, 64, 64, 127, 64, 64, 64);
    printf("old PAR: 1FRQ=0 -> ratio %.6f (1/64), 3FRQ=127 -> ratio %.6f (4/1)\n",
        monomachine::getFmListedRatio(0), monomachine::getFmListedRatio(23));
    return 0;
}
