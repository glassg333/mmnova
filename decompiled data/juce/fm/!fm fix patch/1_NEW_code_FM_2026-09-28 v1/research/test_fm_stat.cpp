// test_fm_stat.cpp — bit-exactness harness for MnmFmStat (m8 FM+STAT).
// Diffs the C++ transcription against emulator vectors word-for-word:
//   32 output words + 99 state words (y-page, X5/Y5, X:$1E-$3F) per block.
// Exit code 0 only on a 100% match.
#include "MnmFmStat.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace mnmfm;

struct Set { uint32_t knob[8]; uint32_t pitch; };

static bool loadVectors(const char* path, std::vector<Set>& sets,
                        std::vector<std::vector<int64_t>>& blocks) {
    std::ifstream f(path);
    if (!f) { fprintf(stderr, "cannot open %s\n", path); return false; }
    std::string line;
    Set cur{};
    bool haveSet = false;
    std::vector<int64_t>* dst = nullptr;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (line.rfind("SET ", 0) == 0) {
            int idx; unsigned A;
            sscanf(line.c_str(), "SET %d A=%u", &idx, &A);
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

int main(int argc, char** argv) {
    const char* vecpath = argc > 1 ? argv[1] :
        "/home/z/my-project/work/fm_stat_core/fm_stat_vectors.txt";
    std::vector<Set> sets;
    std::vector<std::vector<int64_t>> blocks;
    if (!loadVectors(vecpath, sets, blocks)) return 2;

    long long words = 0, badWords = 0, totalBlocks = 0;
    int firstBadSet = -1, firstBadBlk = -1;

    for (size_t si = 0; si < sets.size(); ++si) {
        MnmFmStat m;
        m.init();
        const std::vector<int64_t>& ref = blocks[si];
        size_t pos = 0;
        int blk = 0;
        while (pos + 131 <= ref.size()) {
            uint32_t out[32];
            m.conf(sets[si].knob);
            m.proc(sets[si].pitch, out);
            for (int i = 0; i < 32; ++i) {
                uint32_t p = out[i];
                int64_t sv = (p & 0x800000u) ? (int64_t)p - (1ll << 24) : (int64_t)p;
                int64_t exp = ref[pos + i];
                ++words;
                if (sv != exp) {
                    ++badWords;
                    if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; }
                }
            }
            pos += 32;
            for (int i = 0; i < 64; ++i) {
                int64_t got = m.mem.st[i];
                int64_t exp = ref[pos + i];
                ++words;
                if (got != exp) { ++badWords; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            }
            pos += 64;
            int64_t xs5 = m.mem.X[5], ys5 = m.mem.Y[5];
            if (xs5 != ref[pos]) { ++badWords; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            ++words;
            if (ys5 != ref[pos + 1]) { ++badWords; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            ++words;
            pos += 2;
            for (int a = 0x1E; a < 0x40; ++a) {
                int64_t got = m.mem.X[a];
                int64_t exp = ref[pos + (a - 0x1E)];
                ++words;
                if (got != exp) { ++badWords; if (firstBadSet < 0) { firstBadSet = (int)si; firstBadBlk = blk; } }
            }
            pos += 0x40 - 0x1E;
            ++totalBlocks;
            ++blk;
        }
    }
    printf("FM-STAT: %lld blocks, %lld words, mismatches = %lld", totalBlocks, words, badWords);
    if (badWords == 0) {
        printf("  [100%% BIT-EXACT]\n");
        return 0;
    }
    printf("  FIRST BAD: set %d block %d\n", firstBadSet, firstBadBlk);
    MnmFmStat m;
    m.init();
    const Set& s = sets[firstBadSet];
    const std::vector<int64_t>& ref = blocks[firstBadSet];
    size_t pos = 0;
    uint32_t out[32];
    for (int blk = 0; blk <= firstBadBlk; ++blk) {
        m.conf(s.knob);
        m.proc(s.pitch, out);
        pos += 131;
    }
    pos -= 131;
    printf("  knobs:");
    for (int i = 0; i < 8; ++i) printf(" %u", s.knob[i]);
    printf("  A=%u\n", s.pitch);
    int shown = 0;
    for (int i = 0; i < 32 && shown < 12; ++i) {
        uint32_t p = out[i];
        int64_t sv = (p & 0x800000u) ? (int64_t)p - (1ll << 24) : (int64_t)p;
        if (sv != ref[pos + i]) { printf("    out[%2d]: got %7lld exp %7lld\n", i, (long long)sv, (long long)ref[pos + i]); ++shown; }
    }
    // first mismatching state word
    pos += 32;
    for (int i = 0; i < 64 && shown < 18; ++i) {
        if ((int64_t)m.mem.st[i] != ref[pos + i]) {
            printf("    st[$%02X]: got %7lld exp %7lld\n", i, (long long)m.mem.st[i], (long long)ref[pos + i]);
            ++shown;
        }
    }
    return 1;
}
