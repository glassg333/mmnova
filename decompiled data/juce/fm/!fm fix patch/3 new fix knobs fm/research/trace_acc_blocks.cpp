// trace_acc_blocks.cpp — acc на входе STAT PROC по блокам 0..15 для ноты 69
#include <cstdio>
#include <cstdint>
#include "firmware/Firmware.h"
#include "dsp/DspEngine.h"
#include "host/HostModel.h"
#include "dsp56kEmu/peripherals.h"
#include "dsp56kEmu/dsp.h"

using namespace mnm;

static void drainTx(dsp::DspEngine& eng) {
    auto& hi = eng.periph().getHI08();
    while (hi.hasTX()) hi.readTX();
}

static void stepBlock(dsp::DspEngine& eng, uint32_t procAddr, int blk) {
    auto& hi = eng.periph().getHI08();
    auto& dsp = eng.dsp();
    uint64_t guard = 0;
    int hits = 0;
    while (hi.txData().size() < 32) {
        dsp.exec();
        const uint32_t pc = dsp.getPC().toWord();
        if (pc == procAddr) {
            const auto& r = dsp.readRegs();
            uint64_t a = (uint64_t)r.a.var;
            uint32_t b = (uint32_t)(r.b.var & 0xFFFFFF);
            uint64_t a1 = (a >> 24) & 0xFFFFFF, a0 = a & 0xFFFFFF;
            if (hits < 4) fprintf(stderr, "  blk %2d hit#%d: a1=%06llX a0=%06llX w41=%06X\n",
                                  blk, hits, (unsigned long long)a1, (unsigned long long)a0, b);
            ++hits;
        }
        if (++guard > 4000000ull) break;
    }
    drainTx(eng);
    if (hits) fprintf(stderr, "  blk %2d: PROC вызван %d раз\n", blk, hits);
}

int main(int argc, char** argv) {
    const auto fw = fw::loadFirmware(argv[1]);
    dsp::DspEngine eng(fw);
    eng.setJitMaxInstructionsPerBlock(1);
    host::HostModel h;
    h.setMachine(host::Machine::FM_STAT);
    h.setLevel(100); h.setMasterTuneHz(440.0); h.setBpm(120.0);
    h.settle();
    h.noteOn(69);
    for (int blk = 0; blk < 16; ++blk) {
        const auto& b = h.nextBlock();
        eng.sendBlock(b.w.data());
        drainTx(eng);
        stepBlock(eng, 0x145D21, blk);
        drainTx(eng);
    }
    return 0;
}
