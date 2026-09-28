// Дамп таблиц из эталона Monomodule (оригинальная прошивка на dsp56300):
//   Y:$14A000 — синус-таблица 8192 слов (строит jsr $100077)
//   Y:$141A80 — таблица отношений FRQ 24 слова (проверка kFmRatioExact)
//   X:$140000 — LUT питч->частота (256 слов? дампим 512) — закон питч-слова
// Сборка: g++ -std=c++17 -O2 -I<monomod>/src/core -I<dsp56300-src>/source dump_etalon_tables.cpp \
//          libmnmcore.a libdsp56kEmu.a libdsp56kBase.a libasmjit.a -lpthread -o dump_tables
#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>
#include "firmware/Firmware.h"
#include "dsp/DspEngine.h"

int main(int argc, char** argv) {
    const char* syx = argc > 1 ? argv[1] : "Elektron_SFX6-60_OS1.32B.syx";
    const char* outDir = argc > 2 ? argv[2] : ".";
    try {
        const auto fw = mnm::fw::loadFirmware(syx);
        std::fprintf(stderr, "firmware %s loaded\n", fw.version.c_str());
        mnm::dsp::DspEngine eng(fw);   // reset() внутри: строит синус-таблицу, инит структур

        // Синус-таблица
        {
            std::vector<uint32_t> t(8192);
            for (uint32_t i = 0; i < 8192; ++i) t[i] = eng.peek(mnm::fw::Space::Y, 0x14A000 + i) & 0xFFFFFF;
            std::string p = std::string(outDir) + "/etalon_sine_14A000.txt";
            FILE* f = fopen(p.c_str(), "w");
            for (auto w : t) std::fprintf(f, "%06x\n", w);
            fclose(f);
            std::fprintf(stderr, "sine  -> %s (%zu words)\n", p.c_str(), t.size());
        }
        // Таблица отношений FRQ
        {
            std::vector<uint32_t> t(32);
            for (uint32_t i = 0; i < 32; ++i) t[i] = eng.peek(mnm::fw::Space::Y, 0x141A80 + i) & 0xFFFFFF;
            std::string p = std::string(outDir) + "/etalon_ratio_141A80.txt";
            FILE* f = fopen(p.c_str(), "w");
            for (auto w : t) std::fprintf(f, "%06x\n", w);
            fclose(f);
            std::fprintf(stderr, "ratio -> %s (%zu words)\n", p.c_str(), t.size());
        }
        // Питч-LUT X:$140000 (структура кернела $2C0..$2EB на неё смотрит)
        {
            std::vector<uint32_t> t(4096);
            for (uint32_t i = 0; i < 1024; ++i) t[i] = eng.peek(mnm::fw::Space::X, 0x140000 + i) & 0xFFFFFF;
            std::string p = std::string(outDir) + "/etalon_pitchlut_140000_full.txt";
            FILE* f = fopen(p.c_str(), "w");
            for (auto w : t) std::fprintf(f, "%06x\n", w);
            fclose(f);
            std::fprintf(stderr, "pitchLUT -> %s (%zu words)\n", p.c_str(), t.size());
        }
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
