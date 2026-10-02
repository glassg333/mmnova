// test_dist_exp69.cpp — MnmDistDrive против векторов бит-точного эмулятора.
// Сборка: g++ -O2 -std=c++17 -I. -o test_dist_exp69 test_dist_exp69.cpp
// Прогон: ./test_dist_exp69 vectors_dist_exp69.txt
#include "MnmDistDrive.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    const char* path = argc > 1 ? argv[1] : "vectors_dist_exp69.txt";
    FILE* f = fopen(path, "r");
    if (!f) { fprintf(stderr, "no vectors: %s\n", path); return 2; }

    mnm::MnmDistDrive d;
    mnm::MnmDistDrive::Params p;
    long checks = 0, bad = 0, frame = 0;
    char line[256];
    while (fgets(line, sizeof line, f))
    {
        if (line[0] == '#' || line[0] == '\n') continue;
        int atk, fr, lvl, ph, idx;
        if (sscanf(line, "CASE atk %d FRAMES %*d", &atk) == 1)
        {
            d.reset(); p = {}; p.atkIdx = atk; p.vol = 100;
            frame = 0;
            continue;
        }
        if (sscanf(line, "F %d LVL %d PH %d IDX %d", &fr, &lvl, &ph, &idx) == 4)
        {
            const int32_t gl = d.tickRaw(p);
            const int     gp = d.getPhase();
            const int32_t gi = (uint32_t) d.toneIndex(p) & 0xFFFFFF;
            ++checks;
            if (gl != lvl) { ++bad; fprintf(stderr, "atk=%d f=%d LVL %06X != %06X\n", p.atkIdx, fr, gl & 0xFFFFFF, lvl); }
            if (gp != ph)  { ++bad; fprintf(stderr, "atk=%d f=%d PH %d != %d\n",  p.atkIdx, fr, gp, ph); }
            if (gi != (uint32_t) idx) { ++bad; fprintf(stderr, "atk=%d f=%d IDX %06X != %06X\n", p.atkIdx, fr, gi, idx); }
            ++frame;
        }
    }
    fclose(f);
    printf("checks=%ld mismatches=%ld %s\n", checks, bad,
           bad == 0 ? "[100% DIST-DRIVE BIT-EXACT]" : "[FAIL]");
    return bad == 0 ? 0 : 1;
}
