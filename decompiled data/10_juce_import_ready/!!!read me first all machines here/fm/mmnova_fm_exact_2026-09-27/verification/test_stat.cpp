// test_stat.cpp — diff the C++ FM+STAT mirror against emulator ground truth
#include "MnmFmExact.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct Vec {
    int knobs[8]; int pitch; int blocks;
    std::vector<std::vector<int>> outs;
    std::vector<int> states;
};

static std::vector<Vec> load(const char* path) {
    std::ifstream f(path);
    std::vector<Vec> out;
    Vec cur;
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream ss(line);
        char tag; ss >> tag;
        if (tag == 'V') {
            for (int i = 0; i < 8; ++i) ss >> cur.knobs[i];
            ss >> cur.pitch >> cur.blocks;
            cur.outs.clear(); cur.states.clear();
        } else if (tag == 'O') {
            std::vector<int> blk(32);
            for (auto& x : blk) { ss >> x; }
            cur.outs.push_back(blk);
        } else if (tag == 'S') {
            for (int i = 0; i <= 0x2A - 0x10; ++i) { int x; ss >> x; cur.states.push_back(x); }
            out.push_back(cur);
        }
    }
    return out;
}

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "/home/z/my-project/work/fm_fix/stat_vectors.txt";
    auto vecs = load(path);
    mnmfm::initSineTable();
    int pass = 0, total = 0;
    for (size_t vi = 0; vi < vecs.size(); ++vi) {
        Vec& v = vecs[vi];
        mnmfm::FmStatVoice st;
        for (int bi = 0; bi < (int)v.outs.size(); ++bi) {
            int32_t out[32];
            st.process(v.knobs, v.pitch, out);
            total++;
            int bad = 0, first = -1;
            for (int i = 0; i < 32; ++i)
                if (out[i] != v.outs[bi][i]) { bad++; if (first < 0) first = i; }
            if (bad == 0) pass++;
            else if (bi == (int)v.outs.size() - 1) {
                printf("vec %zu blk %d: %2d/32 wrong, first@%d: got %06X want %06X\n",
                       vi, bi, bad, first, out[first] & 0xFFFFFF, v.outs[bi][first] & 0xFFFFFF);
                // state diff
                printf("  states:");
                for (int off = 0x10; off <= 0x2A; ++off) {
                    int got = st.w[off] & 0xFFFFFF;
                    int want = v.states[off - 0x10];
                    if (got != want) printf(" [%02X]%06X!=%06X", off, got, want);
                }
                printf("\n");
            }
        }
    }
    printf("=== blocks matching: %d / %d ===\n", pass, total);
    return 0;
}
