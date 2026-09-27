// test_mnm_td_vectors.cpp — full-frame Track Delay state regression vs the
// FIXED emulator (and-op b0 preservation). Vectors: research/td_vectors.txt.
// Build: g++ -O2 -std=c++17 -I../Monomachine_Nova_Synth/Source/dsp/mnm \
//            test_mnm_td_vectors.cpp -o ttdv
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include "MnmTrackDelay.hpp"

using namespace mnm::td;

static const uint32_t VM24 = 0xFFFFFFu;

struct Frame {
    uint32_t bus[32];
    std::vector<std::pair<int, uint32_t>> Xst, Yst;   // (addr, val) scratch+SRAM
};

struct Set {
    std::string name;
    std::vector<Frame> frames;
};

static uint32_t q23v(double f) {
    long v = lround(f * 8388608.0);
    return (uint32_t)(v & VM24);
}

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "td_vectors.txt";
    FILE* f = fopen(path, "r");
    if (!f) { printf("FAIL: no %s\n", path); return 1; }

    std::vector<Set> sets;
    char line[128];
    while (fgets(line, sizeof line, f)) {
        if (!strncmp(line, "# set ", 6)) {
            sets.push_back(Set());
            sets.back().name = line + 6;
            while (sets.back().name.size() && sets.back().name[0] == ' ')
                sets.back().name.erase(0, 1);
            while (sets.back().name.size() && (sets.back().name.back() == '\n' || sets.back().name.back() == '\r'))
                sets.back().name.pop_back();
        } else if (!strncmp(line, "# frame ", 8)) {
            sets.back().frames.push_back(Frame());
        } else if (!strncmp(line, "# bus", 5)) {
            Frame& fr = sets.back().frames.back();
            for (int i = 0; i < 32; ++i) {
                if (!fgets(line, sizeof line, f)) return 1;
                fr.bus[i] = (uint32_t)strtoul(line, nullptr, 16);
            }
        } else if (!strncmp(line, "# state", 7)) {
            Frame& fr = sets.back().frames.back();
            for (int i = 0; i < 2 * (256 + 256); ++i) {
                if (!fgets(line, sizeof line, f)) return 1;
                int addr = (int)strtol(line + 1, nullptr, 16);
                uint32_t v = (uint32_t)strtoul(line + 6, nullptr, 16);
                if (line[0] == 'X') fr.Xst.push_back({addr, v});
                else fr.Yst.push_back({addr, v});
            }
        }
    }
    fclose(f);

    // param sets — must mirror exp56_td_vectors.py
    struct PS { const char* name; double srr, tim, snd, fdb, bas, wid, atk, dec, rel; int lpq; } ps[] = {
        {"A", 0.0, 0.3, 0.5, 0.4, 0.5, 0.5, 0.0, 0.0, 0.0, 64},
        {"B", 0.7, 0.8, 0.9, 0.6, 0.2, 0.1, 0.1, 0.2, 0.3, 100},
    };

    long total = 0, pass = 0;
    long env_cells = 0;   // known envelope-state cells excluded from strict pass
    for (auto& S : sets) {
        const PS& P = ps[S.name[0] == 'A' ? 0 : 1];
        TrackDelayCore c;
        TrackDelayCore::Params p{};
        p.lpq      = q23v(P.lpq / 127.0);
        p.filtAtk  = q23v(P.atk);
        p.filtDec  = q23v(P.dec);
        p.bofs     = q23v(P.atk);
        p.wofs     = q23v(P.wid);
        p.eqf      = q23v(P.atk);
        p.eqg      = q23v(P.lpq / 127.0);
        p.srr      = q23v(P.srr);
        p.dtim     = q23v(P.tim);
        p.dsnd     = q23v(P.snd);
        p.dfb      = q23v(P.fdb);
        p.dbas     = q23v(P.bas);
        p.dwid     = q23v(P.wid);

        uint32_t oL[16], oR[16], ec[16];
        for (size_t fi = 0; fi < S.frames.size(); ++fi) {
            const Frame& fr = S.frames[fi];
            c.processFrame(p, fr.bus, oL, oR, ec);
            // compare internal state: scratch (0x00-0xFF) then SRAM (0x4000-0x40FF)
            for (auto& [addr, want] : fr.Xst) {
                uint32_t got;
                if (addr < 0x100) got = c.Xb[addr] & VM24;
                else got = c.ring[addr - 0x4000] & VM24;
                ++total;
                if (got == want) ++pass;
            }
            for (auto& [addr, want] : fr.Yst) {
                uint32_t got;
                if (addr < 0x100) got = c.Yb[addr] & VM24;
                else got = c.ringB[addr - 0x4000] & VM24;
                ++total;
                if (got == want) ++pass;
            }
        }
    }
    printf("Track Delay state regression: %ld/%ld words bit-exact (%.2f%%)\n",
           pass, total, 100.0 * pass / (total ? total : 1));
    return 0;
}
