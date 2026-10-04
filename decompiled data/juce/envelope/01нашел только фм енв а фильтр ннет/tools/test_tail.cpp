// test_tail.cpp — раннер векторов хвоста P:$0AD1-$0B4C (пак 11).
// Ожидаемые значения — слова бит-точного эмулятора OS 1.32B
// (снапшоты exp22_snap/exp22_skip, методика итерации 22).
// Запуск:  g++ -O2 -std=c++17 -I dsp/mnm -o test_tail tools/test_tail.cpp
//          ./test_tail vectors/tail_vectors.txt
// Ожидается: verify: OK=8296 BAD=0  [100% TAIL BIT-EXACT]
#include "MnmTail.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace mnmtail;

static uint32_t hx(const char* s) {
    return (uint32_t)strtoul(s, nullptr, 16);
}

int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s vectors.txt\n", argv[0]); return 2; }
    FILE* f = fopen(argv[1], "r");
    if (!f) { perror("open"); return 2; }
    char line[1 << 16];
    long n_ok = 0, n_bad = 0, n_frames = 0, n_skip = 0;
    std::vector<std::string> bads;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (strncmp(line, "CFG", 3) == 0) continue;
        if (strncmp(line, "IN0", 3) != 0) { fprintf(stderr, "bad line: %s\n", line); return 2; }
        TailIn0 in;
        {
            unsigned v[10];
            if (sscanf(line + 3,
                " YPCB=%X XPCB=%X YPCF=%X XPCF=%X YP1F=%X YPFF=%X "
                "X050=%X X061=%X YC4=%X XC5=%X",
                &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &v[9]) != 10)
            { fprintf(stderr, "bad IN0: %s\n", line); return 2; }
            in.YPCB = v[0]; in.XPCB = v[1]; in.YPCF = v[2]; in.XPCF = v[3];
            in.YP1F = v[4]; in.YPFF = v[5]; in.X050 = v[6]; in.X061 = v[7];
            in.YC4 = v[8];  in.XC5 = v[9];
        }
        if (!fgets(line, sizeof line, f)) break;
        uint32_t b7[16]; const uint32_t* b7p = nullptr;
        if (strncmp(line, "B7 SKIP", 7) == 0) {
            // skip-ветвь: T2/T4 не исполняются
        } else if (strncmp(line, "B7", 2) == 0) {
            char* p = line + 2;
            for (int k = 0; k < 16; ++k) b7[k] = strtoul(p, &p, 16);
            b7p = b7;
        } else { fprintf(stderr, "bad B7: %s\n", line); return 2; }
        if (!fgets(line, sizeof line, f)) break;
        TailB8 b8;
        {
            char* p = strstr(line, "XFF=");
            if (!p) { fprintf(stderr, "bad B8: %s\n", line); return 2; }
            unsigned vff, vc9;
            if (sscanf(p, "XFF=%X X2C9=%X", &vff, &vc9) != 2)
            { fprintf(stderr, "bad B8: %s\n", line); return 2; }
            b8.XFF = vff; b8.X2C9 = vc9;
            p = strstr(line, "L2 ");
            p += 3;
            for (int k = 0; k < 32; ++k) b8.l2[k] = strtoul(p, &p, 16);
            p = strstr(line, "XPRE ");
            p += 5;
            for (int k = 0; k < 32; ++k) b8.xpre[k] = strtoul(p, &p, 16);
        }
        if (!fgets(line, sizeof line, f)) break;
        std::vector<std::pair<std::string, uint32_t>> exp;
        {
            char* q2 = line + 4;                  // после "EXP "
            char* save = nullptr;
            for (char* t = strtok_r(q2, " \t\n", &save); t;
                 t = strtok_r(nullptr, " \t\n", &save)) {
                char* eq = strchr(t, '=');
                if (!eq) continue;
                *eq = 0;
                exp.emplace_back(std::string(t), hx(eq + 1));
            }
        }
        if (!fgets(line, sizeof line, f)) break;   // END
        TailResult res;
        model_tail(in, b7p, b8, res);
        ++n_frames;
        if (res.took_bge) ++n_skip;
        for (const auto& kv : exp) {
            auto it = res.out.find(kv.first);
            uint32_t got = (it == res.out.end()) ? 0xDEADBEu : it->second;
            if (got == kv.second) ++n_ok;
            else {
                ++n_bad;
                if ((int)bads.size() < 20) {
                    char gb[16], eb[16];
                    snprintf(gb, sizeof gb, "%06X", got);
                    snprintf(eb, sizeof eb, "%06X", kv.second);
                    bads.push_back(kv.first + " model=" + gb + " emu=" + eb);
                }
            }
        }
    }
    fclose(f);
    printf("verify: OK=%ld BAD=%ld (bge-skip frames: %ld, frames: %ld)\n",
           n_ok, n_bad, n_skip, n_frames);
    for (const auto& s : bads) printf("  %s\n", s.c_str());
    if (n_bad == 0 && n_frames > 0) printf("[100%% TAIL BIT-EXACT]\n");
    return (n_bad == 0 && n_frames > 0) ? 0 : 1;
}
