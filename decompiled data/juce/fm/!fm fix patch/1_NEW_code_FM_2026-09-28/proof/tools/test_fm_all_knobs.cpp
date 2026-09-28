// test_fm_all_knobs.cpp — пакетный C++-тестер порта: прогоняет ВСЕ сеты
// свипа ручек (fm_all_knob_vectors.txt) через бит-точные ядра
// MnmFmStat / MnmFmPar / MnmFmDyn и сравнивает слово-в-слово
// (32 выходных + 100 стейт-слов на блок) с векторами эмулятора OS 1.32.
// Exit code 0 только при 100% совпадении.
#include "MnmFmStat.hpp"
#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace mnmfm;

struct Set { int machine; uint32_t knob[8]; uint32_t pitch; };

static bool loadVectors(const char* path, std::vector<Set>& sets,
                        std::vector<std::vector<int64_t>>& blocks) {
    std::ifstream f(path);
    if (!f) { fprintf(stderr, "cannot open %s\n", path); return false; }
    std::string line;
    Set cur{};
    cur.machine = 8;
    bool haveSet = false;
    std::vector<int64_t>* dst = nullptr;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (line.rfind("MACHINE ", 0) == 0) {
            sscanf(line.c_str(), "MACHINE %d", &cur.machine);
        } else if (line.rfind("SET ", 0) == 0) {
            unsigned A;
            sscanf(line.c_str(), "SET %d A=%u", (int*)&cur.pitch, &A);
            cur.pitch = A; haveSet = true;
            sets.push_back(cur);
            blocks.emplace_back();
            dst = &blocks.back();
        } else if (line.rfind("P ", 0) == 0 && haveSet) {
            std::istringstream ss(line.substr(2));
            for (int i = 0; i < 8; ++i) ss >> sets.back().knob[i];
        } else if ((line.rfind("B ", 0) == 0 || line.rfind("S ", 0) == 0) && dst) {
            std::istringstream ss(line.substr(2));
            int64_t v;
            while (ss >> v) dst->push_back(v);
        }
    }
    return true;
}

// Фабрика: создать и заинитить ядро нужной машины.
static void* makeCore(int machine) {
    if (machine == 8)  { auto* m = new MnmFmStat(); m->init(); return m; }
    if (machine == 9)  { auto* m = new MnmFmPar();  m->init(); return m; }
    if (machine == 10) { auto* m = new MnmFmDyn();  m->init(); return m; }
    return nullptr;
}
static void procBlock(const Set& s, void* m, uint32_t out[32]) {
    if (s.machine == 8)  ((MnmFmStat*)m)->proc(s.pitch, out);
    if (s.machine == 9)  ((MnmFmPar*)m)->proc(s.pitch, out);
    if (s.machine == 10) ((MnmFmDyn*)m)->proc(s.pitch, out);
}
static void conf(const Set& s, void* m) {
    if (s.machine == 8)  ((MnmFmStat*)m)->conf(s.knob);
    if (s.machine == 9)  ((MnmFmPar*)m)->conf(s.knob);
    if (s.machine == 10) ((MnmFmDyn*)m)->conf(s.knob);
}
static int64_t stWord(const Set& s, void* m, int i) {
    if (s.machine == 8)  return ((MnmFmStat*)m)->mem.st[i];
    if (s.machine == 9)  return ((MnmFmPar*)m)->mem.st[i];
    return ((MnmFmDyn*)m)->mem.st[i];
}
static int64_t memX(const Set& s, void* m, int a) {
    if (s.machine == 8)  return ((MnmFmStat*)m)->mem.X[a];
    if (s.machine == 9)  return ((MnmFmPar*)m)->mem.X[a];
    return ((MnmFmDyn*)m)->mem.X[a];
}
static int64_t memY(const Set& s, void* m, int a) {
    if (s.machine == 8)  return ((MnmFmStat*)m)->mem.Y[a];
    if (s.machine == 9)  return ((MnmFmPar*)m)->mem.Y[a];
    return ((MnmFmDyn*)m)->mem.Y[a];
}
static void initCore(const Set& s, void* m) {
    if (s.machine == 8)  ((MnmFmStat*)m)->init();
    if (s.machine == 9)  ((MnmFmPar*)m)->init();
    if (s.machine == 10) ((MnmFmDyn*)m)->init();
}

int main(int argc, char** argv) {
    const char* vecpath = argc > 1 ? argv[1] :
        "/home/z/my-project/work/fm_sweep_all/fm_all_knob_vectors.txt";
    std::vector<Set> sets;
    std::vector<std::vector<int64_t>> blocks;
    if (!loadVectors(vecpath, sets, blocks)) return 2;

    long long words = 0, badWords = 0, totalBlocks = 0;
    int firstBadSet = -1, firstBadBlk = -1;
    for (size_t si = 0; si < sets.size(); ++si) {
        const Set& s = sets[si];
        void* m = makeCore(s.machine);
        if (!m) { fprintf(stderr, "bad machine %d in set %d\n", s.machine, (int)si); return 2; }
        const std::vector<int64_t>& ref = blocks[si];
        size_t pos = 0;
        int blk = 0;
        long long setWords = 0, setBad = 0;
        while (pos + 132 <= ref.size()) {
            uint32_t out[32];
            conf(s, m);
            procBlock(s, m, out);
            for (int i = 0; i < 32; ++i) {
                uint32_t p = out[i];
                int64_t sv = (p & 0x800000u) ? (int64_t)p - (1ll << 24) : (int64_t)p;
                int64_t exp = ref[pos + i];
                ++words; ++setWords;
                if (sv != exp) { ++badWords; ++setBad; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            }
            pos += 32;
            for (int i = 0; i < 64; ++i) {
                int64_t got = stWord(s, m, i);
                int64_t exp = ref[pos + i];
                ++words; ++setWords;
                if (got != exp) { ++badWords; ++setBad; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            }
            pos += 64;
            int64_t xs5 = memX(s, m, 5), ys5 = memY(s, m, 5);
            ++words; ++setWords;
            if (xs5 != ref[pos]) { ++badWords; ++setBad; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            ++words; ++setWords;
            if (ys5 != ref[pos + 1]) { ++badWords; ++setBad; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            pos += 2;
            for (int a = 0x1E; a < 0x40; ++a) {
                int64_t got = memX(s, m, a);
                int64_t exp = ref[pos + (a - 0x1E)];
                ++words; ++setWords;
                if (got != exp) { ++badWords; ++setBad; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            }
            pos += 0x40 - 0x1E;
            ++totalBlocks; ++blk;
        }
        printf("SET %3zd  m%-2d  words %6lld  mism %4lld\n", si, s.machine, setWords, setBad);
        delete m;
    }
    printf("\nTOTAL: %lld blocks, %lld words, mismatches = %lld", totalBlocks, words, badWords);
    if (badWords == 0) {
        printf("  [100%% BIT-EXACT]\n");
        return 0;
    }
    printf("  FIRST BAD: set %d block %d\n", firstBadSet, firstBadBlk);
    return 1;
}
