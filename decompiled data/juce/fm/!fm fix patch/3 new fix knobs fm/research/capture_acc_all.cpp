// capture_acc_all.cpp — золотая таблица: acc на входе PROC для 3 FM-машин,
// все ноты 0..127, мастер-тюн 440 и 400 Гц. Эталон Monomodule (оригинальная ОС).
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <string>
#include <vector>
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

// гоняет блок до конца; при pc==procAddr возвращает true (регистры в cap)
static void stepBlock(dsp::DspEngine& eng, uint32_t procAddr, uint64_t& a56, uint32_t& w41, bool& hit) {
    auto& hi = eng.periph().getHI08();
    auto& dsp = eng.dsp();
    uint64_t guard = 0;
    while (hi.txData().size() < 32) {
        dsp.exec();
        const uint32_t pc = dsp.getPC().toWord();
        if (procAddr && pc == procAddr) {
            const auto& r = dsp.readRegs();
            a56 = (uint64_t)r.a.var;
            w41 = (uint32_t)(r.b.var & 0xFFFFFF);
            hit = true;
        }
        if (++guard > 4000000ull) return;
    }
}

int main(int argc, char** argv) {
    const char* syx = argc > 1 ? argv[1] : "Elektron_SFX6-60_OS1.32B.syx";
    try {
        const auto fw = fw::loadFirmware(syx);
        dsp::DspEngine eng(fw);
        eng.setJitMaxInstructionsPerBlock(1);
        struct M { host::Machine m; uint32_t proc; const char* name; } ms[] = {
            { host::Machine::FM_STAT, 0x145D21, "STAT" },
            { host::Machine::FM_PAR,  0x145EC9, "PAR" },
            { host::Machine::FM_DYN,  0x14619D, "DYN" },
        };
        FILE* out = fopen("/home/z/my-project/work/fm_pitch_golden.json", "w");
        fprintf(out, "[\n");
        bool first = true;
        for (auto& m : ms) {
            for (double tuneHz : {440.0, 400.0}) {
                host::HostModel h;
                h.setMachine(m.m);
                h.setLevel(100); h.setMasterTuneHz(tuneHz); h.setBpm(120.0);
                h.settle();
                for (int note = 0; note <= 127; ++note) {
                    h.noteOn(note);
                    uint64_t a56 = 0; uint32_t w41 = 0; bool hit = false;
                    for (int blk = 0; blk < 12; ++blk) {
                        const auto& b = h.nextBlock();
                        eng.sendBlock(b.w.data());
                        drainTx(eng);
                        stepBlock(eng, blk >= 10 ? m.proc : 0, a56, w41, hit);
                        drainTx(eng);
                    }
                    if (!hit) { fprintf(stderr, "FAIL %s n%d\n", m.name, note); continue; }
                    const uint64_t a1 = (a56 >> 24) & 0xFFFFFF, a0 = a56 & 0xFFFFFF;
                    if (!first) fprintf(out, ",\n");
                    first = false;
                    fprintf(out, "{\"machine\":\"%s\",\"note\":%d,\"tune\":%.1f,\"w41\":%u,\"a1\":%llu,\"a0\":%llu}",
                            m.name, note, tuneHz, w41,
                            (unsigned long long)a1, (unsigned long long)a0);
                }
                fprintf(stderr, "%s tune=%.0f done\n", m.name, tuneHz);
            }
        }
        fprintf(out, "\n]\n");
        fclose(out);
        printf("golden table written\n");
        return 0;
    } catch (const std::exception& e) {
        fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
