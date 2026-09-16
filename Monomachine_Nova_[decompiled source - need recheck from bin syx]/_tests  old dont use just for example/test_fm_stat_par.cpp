#include "../dsp/monomachine_fm_stat_par.hpp"
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::cout << "Testing Monomachine FM+ STATIC (Machine 8) & FM+ PARALLEL (Machine 9)...\n";

    // 1. Test FM+ STATIC
    monomachine::MonomachineFmStatic fmStat;
    fmStat.reset(44100.0);
    // 1FRQ=56 (Ratio 1.0), 1FIN=64, 1ENV=64, 1FB=30, 2FRQ=80 (Ratio 2.0), 2VOL=64, TONE=64, TUNE=64
    fmStat.setParameters(56, 64, 64, 30, 80, 64, 64, 64);
    fmStat.noteOn(60, 127);

    constexpr size_t kFrames = 256;
    std::vector<float> outL(kFrames, 0.0f);
    std::vector<float> outR(kFrames, 0.0f);

    fmStat.processStereo(outL.data(), outR.data(), kFrames);
    float peakStat = 0.0f;
    for (size_t i = 0; i < kFrames; ++i) peakStat = std::max(peakStat, std::abs(outL[i]));
    std::cout << "FM+ STATIC output peak: " << peakStat << "\n";
    assert(peakStat > 0.1f && "FM+ STATIC should generate sound");

    // 2. Test FM+ PARALLEL
    monomachine::MonomachineFmParallel fmPar;
    fmPar.reset(44100.0);
    // 3 parallel operators: 1FRQ=56, 1ENV=50, 2FRQ=80, 2ENV=50, 3FRQ=96, 3ENV=50, TONE=64, TUNE=64
    fmPar.setParameters(56, 50, 80, 50, 96, 50, 64, 64);
    fmPar.noteOn(60, 127);

    std::fill(outL.begin(), outL.end(), 0.0f);
    std::fill(outR.begin(), outR.end(), 0.0f);
    fmPar.processStereo(outL.data(), outR.data(), kFrames);

    float peakPar = 0.0f;
    for (size_t i = 0; i < kFrames; ++i) peakPar = std::max(peakPar, std::abs(outL[i]));
    std::cout << "FM+ PARALLEL output peak: " << peakPar << "\n";
    assert(peakPar > 0.1f && "FM+ PARALLEL should generate sound");

    std::cout << "FM+ STATIC and FM+ PARALLEL verified successfully!\n";
    return 0;
}
