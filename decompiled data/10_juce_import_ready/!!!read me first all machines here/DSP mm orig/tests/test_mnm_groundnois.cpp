// test_mnm_groundnois.cpp — validate MnmGroundNois.hpp against emulator
// frame vectors (research/m2_vectors.txt, exp57).
#include <cstdio>
#include <cstdlib>
#include "MnmGroundNois.hpp"

using mnm::gn::GroundNois;

int main(int argc, char** argv) {
    const char* vec = argc > 1 ? argv[1] : "m2_vectors.txt";
    FILE* f = fopen(vec, "r");
    if (!f) { printf("FAIL: no vectors\n"); return 1; }
    char line[64];
    long total = 0, pass = 0;
    GroundNois* m = nullptr;
    uint32_t st = 0, red = 0, ston = 0;
    uint32_t buf[32];
    int expect = -1;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#') {
            if (line[2] == 's') {   // "# st .. red .. ston .."
                int a, b, c; sscanf(line, "# st %d red %d ston %d", &a, &b, &c);
                delete m; m = new GroundNois();
                m->setKnobs((uint32_t)a << 16, (uint32_t)b << 16, (uint32_t)c << 16);
                st = a; red = b; ston = c;
                expect = 32;
            }
            continue;
        }
        if (expect != 32) continue;
        uint32_t want[32]; want[0] = (uint32_t)strtoul(line, nullptr, 16);
        for (int i = 1; i < 32; ++i) {
            if (!fgets(line, sizeof line, f)) return 1;
            want[i] = (uint32_t)strtoul(line, nullptr, 16);
        }
        m->processFrame(buf);
        for (int i = 0; i < 32; ++i) {
            ++total;
            if ((buf[i] & 0xFFFFFFu) == want[i]) ++pass;
        }
    }
    delete m; fclose(f);
    printf("m2 GND-NOIS frame vectors: %ld/%ld bit-exact (%.2f%%)\n",
           pass, total, 100.0 * pass / (total ? total : 1));
    return 0;
}
