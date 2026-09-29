// =============================================================================
// test_block_routing.cpp — bit-exact verification of the block-level
// ROUTING/MIXER of Monomachine OS 1.32B (pack 9) against the emulator
// captures of the REAL block loop (22 sets, rule 18).
//
//   g++ -O2 -std=c++17 -I dsp/mnm -o test_block_routing test_block_routing.cpp
//   ./test_block_routing vectors/routing_vectors.txt
//
// Vector format:
//   SET <tag> | HDR <hex3> | FLAGS <f0> <f1> <f2> | SLOTS <s0> <s1> <s2>
//   IN <64 hex words> (X:$100-$13F) | SEEDX/SEEDY <0x700 words>
//   MODE <n> + D lines (absolute cells: X:$2C0-$2CB, y:$123/y:$124)
//   SB <k> <n> + D lines (diffs vs seed after sub-block k mixer)
//   END <n> + D lines (diffs vs seed at end of block)
// D addr is decimal, packed: bit24 = Y side; value = 6 hex digits.
// =============================================================================
#include "dsp/mnm/MnmBlockMixer.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>

using namespace mnm;

struct Section {
    std::vector<std::pair<uint32_t, uint32_t>> cells;  // (packed addr, val24)
};
struct SetVec {
    std::string tag;
    uint32_t hrx = 0;
    uint32_t flags[3] = {0, 0, 0};
    uint32_t slots[3] = {0, 0, 0};
    std::vector<uint32_t> sx, sy;                      // 0x700 words each
    Section mode, sb[3], end;
};

static std::vector<SetVec> g_sets;

static void parse(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) { fprintf(stderr, "cannot open %s\n", path); exit(1); }
    char buf[1 << 16];
    SetVec cur;
    bool inSet = false;
    Section* sec = nullptr;
    while (fgets(buf, sizeof(buf), f)) {
        std::string s(buf);
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
        if (s.rfind("SET ", 0) == 0) {
            if (inSet) g_sets.push_back(cur);
            cur = SetVec();
            cur.tag = s.substr(4);
            inSet = true; sec = nullptr;
        } else if (s.rfind("HDR ", 0) == 0) {
            cur.hrx = strtoul(s.c_str() + 4, nullptr, 16);
        } else if (s.rfind("FLAGS ", 0) == 0) {
            sscanf(s.c_str() + 6, "%u %u %u", &cur.flags[0], &cur.flags[1], &cur.flags[2]);
        } else if (s.rfind("SLOTS ", 0) == 0) {
            sscanf(s.c_str() + 6, "%u %u %u", &cur.slots[0], &cur.slots[1], &cur.slots[2]);
        } else if (s.rfind("IN ", 0) == 0) {
            // codec input window (informational: the seed already carries it)
        } else if (s.rfind("SEEDX ", 0) == 0) {
            size_t p = 6;
            while (p < s.size()) {
                cur.sx.push_back(strtoul(s.c_str() + p, nullptr, 16));
                p += 7;
            }
        } else if (s.rfind("SEEDY ", 0) == 0) {
            size_t p = 6;
            while (p < s.size()) {
                cur.sy.push_back(strtoul(s.c_str() + p, nullptr, 16));
                p += 7;
            }
        } else if (s.rfind("MODE ", 0) == 0) {
            int n = atoi(s.c_str() + 5);
            sec = &cur.mode;
            sec->cells.reserve(size_t(n));
        } else if (s.rfind("SB ", 0) == 0) {
            int k = atoi(s.c_str() + 3);
            int n = atoi(s.c_str() + s.find(' ', 3) + 1);
            sec = &cur.sb[k];
            sec->cells.reserve(size_t(n));
        } else if (s.rfind("END ", 0) == 0) {
            sec = &cur.end;
            sec->cells.reserve(size_t(atoi(s.c_str() + 4)));
        } else if (s.rfind("D ", 0) == 0) {
            uint32_t a = strtoul(s.c_str() + 2, nullptr, 10);
            uint32_t v = strtoul(s.c_str() + s.find(' ', 2) + 1, nullptr, 16);
            if (sec) sec->cells.push_back({a, v});
        }
    }
    if (inSet) g_sets.push_back(cur);
    fclose(f);
}

static uint32_t seedVal(const SetVec& sv, uint32_t packed) {
    uint32_t a = packed & 0xFFFFFF;
    return (packed & 0x1000000) ? sv.sy[a] : sv.sx[a];
}

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "vectors/routing_vectors.txt";
    parse(path);
    fprintf(stderr, "sets: %zu\n", g_sets.size());

    int64_t checks = 0, mismatches = 0;
    int setsWithMism = 0;
    const int MAXSHOW = 12;
    int shown = 0;

    for (size_t si = 0; si < g_sets.size(); ++si) {
        SetVec& sv = g_sets[si];
        BlockMixer bm;
        bm.patchDispatch();
        std::vector<std::pair<uint32_t, uint32_t>> seed;
        seed.reserve(sv.sx.size() + sv.sy.size());
        for (size_t a = 0; a < sv.sx.size(); ++a)
            seed.push_back({(uint32_t)a, sv.sx[a]});
        for (size_t a = 0; a < sv.sy.size(); ++a)
            seed.push_back({0x1000000u | (uint32_t)a, sv.sy[a]});
        bm.loadSeed(seed);
        int setMism = 0;

        auto checkCell = [&](uint32_t packed, uint32_t expected) {
            ++checks;
            uint32_t a = packed & 0xFFFFFF;
            uint32_t got = bm.getWord((packed & 0x1000000) ? "y" : "x", a);
            if (got != expected) {
                ++mismatches; ++setMism;
                if (shown < MAXSHOW) {
                    ++shown;
                    fprintf(stderr,
                            "  MISMATCH set %zu (%s) cell %s$%03X:"
                            " exp %06X got %06X\n",
                            si, sv.tag.c_str(),
                            (packed & 0x1000000) ? "Y" : "X", a,
                            expected, got);
                }
            }
        };
        auto checkSection = [&](const Section& sec, const char* what) {
            // expected diff cells must match; everything else = seed
            std::unordered_map<uint32_t, uint32_t> exp;
            for (const auto& c : sec.cells) exp[c.first] = c.second;
            for (const auto& kv : exp) checkCell(kv.first, kv.second);
            for (uint32_t a = 0; a < 0x800; ++a) {
                if (exp.count(a) == 0) {
                    checkCell(a, seedVal(sv, a));
                    if (exp.count(0x1000000u | a) == 0)
                        checkCell(0x1000000u | a, seedVal(sv, 0x1000000u | a));
                } else if (exp.count(0x1000000u | a) == 0) {
                    checkCell(0x1000000u | a, seedVal(sv, 0x1000000u | a));
                }
            }
            (void)what;
        };

        fprintf(stderr, "set %zu: %s\n", si, sv.tag.c_str());
        fprintf(stderr, "  seed %zu/%zu, hrx=%03X\n", sv.sx.size(), sv.sy.size(), sv.hrx);
        // ---- S1: header poll + mode tables ----
        fprintf(stderr, "  runHeader...\n");
        bm.runHeader(sv.hrx);
        fprintf(stderr, "  runHeader ok\n");
        for (const auto& c : sv.mode.cells) checkCell(c.first, c.second);

        // ---- 3 sub-blocks: prologue + frame + mixer ----
        for (int k = 0; k < 3; ++k) {
            fprintf(stderr, "  SB%d... y528=%06X y628=%06X y728=%06X y724=%06X y122=%06X x2C4=%06X\n", k,
                bm.getWord("y",0x528), bm.getWord("y",0x628), bm.getWord("y",0x728),
                bm.getWord("y",0x724), bm.getWord("y",0x122), bm.getWord("x",0x2C4));
            bm.runSubBlock(k);
            fprintf(stderr, "  SB%d ok\n", k);
            char label[8]; snprintf(label, 8, "SB%d", k);
            checkSection(sv.sb[k], label);
        }

        // ---- end of block: MIX / copy (or the harness skip) ----
        fprintf(stderr, "  END... x2C4=%06X\n", bm.getWord("x",0x2C4));
        bm.runEnd();
        fprintf(stderr, "  END ok\n");
        checkSection(sv.end, "END");

        if (setMism) ++setsWithMism;
    }

    printf("checks=%lld mismatches=%lld", (long long)checks, (long long)mismatches);
    if (mismatches == 0) printf("  [100%% BLOCK-ROUTING BIT-EXACT]\n");
    else printf("  sets with mismatches: %d/%zu\n", setsWithMism, g_sets.size());
    return mismatches ? 1 : 0;
}
