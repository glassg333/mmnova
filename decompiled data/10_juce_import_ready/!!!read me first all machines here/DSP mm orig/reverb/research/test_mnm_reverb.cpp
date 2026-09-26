
// test_mnm_reverb.cpp — differential test: MnmReverb.hpp vs emulator vectors
// g++ -std=c++17 -O2 -I. -o t test_mnm_reverb.cpp && ./t
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "MnmReverb.hpp"

struct Set {
    int dec, damp, gate, mix, inp, hp, lp;
    struct Fr { uint32_t bus[32], out[16]; };
    std::vector<Fr> frames;
    std::vector<char> name;
};
#include <vector>

int main() {
    FILE* f = fopen("/home/z/my-project/scripts/exp47_vectors.txt", "rb");
    if (!f) { fprintf(stderr, "no vectors\n"); return 1; }
    char line[4096];
    std::vector<Set> sets; int fails = 0, total = 0;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == 'S') {
            sets.push_back(Set());
            Set& cur = sets.back();
            char nm[8];
            sscanf(line, "SET %7s dec=%d damp=%d gate=%d mix=%d inp=%d hp=%d lp=%d",
                   nm, &cur.dec, &cur.damp, &cur.gate, &cur.mix, &cur.inp, &cur.hp, &cur.lp);
            cur.name.assign(nm, nm+strlen(nm));
            continue;
        }
        if (line[0] != 'F' || sets.empty()) continue;
        Set& cur = sets.back();
        Set::Fr fr;
        const char* p = line + 2;
        for (int i = 0; i < 32; i++) fr.bus[i] = strtoul(p, (char**)&p, 16) & 0xFFFFFF;
        while (*p && *p != '|') p++;
        p++;
        for (int i = 0; i < 16; i++) fr.out[i] = strtoul(p, (char**)&p, 16) & 0xFFFFFF;
        cur.frames.push_back(fr);
    }
    fclose(f);
    for (size_t si = 0; si < sets.size(); si++) {
        Set& S = sets[si];
        mnm::rv::TrackReverbCore core;
        core.fadeCnt = 0x101; core.fadeGain = 0x7FFFFF;
        core.setParams(S.dec << 16, S.damp << 16, S.gate << 16,
                       S.mix << 16, S.hp << 16, S.lp << 16, S.inp << 16);
        // replay the emulator's warmup (7 silent frames advance pointers/LFO)
        { int32_t zbus[32], zo[16]; memset(zbus, 0, sizeof zbus);
          for (int w = 0; w < 6; w++) core.processFrame(zbus, zo); }
        printf("set %s: dec=%d damp=%d gate=%d mix=%d inp=%d frames=%zu\n",
               S.name.data(), S.dec, S.damp, S.gate, S.mix, S.inp, S.frames.size());
        for (size_t fi = 0; fi < S.frames.size(); fi++) {
            const Set::Fr& fr = S.frames[fi];
            int32_t out[16];
            core.processFrame((const int32_t*)fr.bus, out);
            for (int i = 0; i < 16; i++) {
                total++;
                uint32_t got = (uint32_t)out[i] & 0xFFFFFF;
                if (got != fr.out[i]) {
                    fails++;
                    if (fails <= 14)
                        printf("  MISMATCH f%zu out[%2d]: got %06X want %06X\n",
                               fi, i, got, fr.out[i]);
                }
            }
        }
    }
    printf("%d/%d words match\n", total - fails, total);
    return fails ? 1 : 0;
}
