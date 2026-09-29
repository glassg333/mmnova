// =============================================================================
// test_voice_frame.cpp — bit-exact verification of the full Monomachine
// voice frame (OS 1.32B): transliterated kernel code + native FM core
// against the emulator captures (155 sets / 936 frames, rule 18).
//
//   g++ -O2 -std=c++17 -I dsp/mnm -o test_voice_frame test_voice_frame.cpp
//   ./test_voice_frame vectors/voice_frame_vectors.txt
// =============================================================================
#include "dsp/mnm/MnmVoiceFrame.hpp"

#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>

using namespace mnm;

struct SetVec {
    std::vector<std::pair<uint32_t, uint32_t>> seed;   // (packed addr, val24)
    std::vector<uint32_t> sx, sy;                      // raw windows
    std::vector<std::pair<uint32_t, uint32_t>> frameTrigAndDiffCount;
    // per-frame diffs, flattened in file order
    std::vector<std::pair<uint32_t, uint32_t>> diffs;
    std::vector<size_t> diffStart;                     // index per frame
    std::string tag;
};

static std::vector<SetVec> g_sets;

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1]
                                : "vectors/voice_frame_vectors.txt";
    // parse all sets
    {
        FILE* f = fopen(path, "r");
        if (!f) { fprintf(stderr, "cannot open %s\n", path); return 1; }
        std::string line;
        int c;
        SetVec cur;
        bool inSet = false;
        char buf[1 << 16];
        auto handle = [&](const std::string& s) {
            if (s.rfind("SET ", 0) == 0) {
                if (inSet) g_sets.push_back(cur);
                cur = SetVec();
                size_t sp1 = s.find(' ', 4);
                size_t sp2 = s.find(' ', sp1 + 1);
                cur.tag = s.substr(sp1 + 1, sp2 - sp1 - 1);
                inSet = true;
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
                for (size_t a = 0; a < cur.sx.size(); ++a)
                    cur.seed.push_back({(uint32_t)a, cur.sx[a]});
                for (size_t a = 0; a < cur.sy.size(); ++a)
                    cur.seed.push_back({0x1000000u | (uint32_t)a, cur.sy[a]});
            } else if (s.rfind("F ", 0) == 0) {
                int trig = atoi(s.c_str() + 2);
                int nd = atoi(s.c_str() + s.find(' ', 2) + 1);
                cur.frameTrigAndDiffCount.push_back({(uint32_t)trig, (uint32_t)nd});
                cur.diffStart.push_back(cur.diffs.size());
            } else if (s.rfind("D ", 0) == 0) {
                uint32_t a = strtoul(s.c_str() + 2, nullptr, 10);
                uint32_t v = strtoul(s.c_str() + s.find(' ', 2) + 1, nullptr, 16);
                cur.diffs.push_back({a, v});
            }
        };
        while (fgets(buf, sizeof(buf), f)) {
            std::string s(buf);
            while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
            handle(s);
        }
        if (inSet) g_sets.push_back(cur);
        fclose(f);
    }
    fprintf(stderr, "sets: %zu\n", g_sets.size());

    int64_t checks = 0, mismatches = 0;
    int setsWithMism = 0;
    const int MAXSHOW = 12;
    int shown = 0;

    for (size_t si = 0; si < g_sets.size(); ++si) {
        SetVec& sv = g_sets[si];
        VoiceFrame vf;
        vf.patchDispatch();
        vf.machineInit();          // core registers = post-INIT state
        vf.loadSeed(sv.seed);      // kernel memory = seed (post-INIT/CONF)
        vf.syncCoreFromKernel();   // core mem/st = seed values
        int setMism = 0;
        for (size_t fi = 0; fi < sv.frameTrigAndDiffCount.size(); ++fi) {
            uint32_t trig = sv.frameTrigAndDiffCount[fi].first;
            vf.frame(trig);
            // expected diffs for this frame
            size_t b = sv.diffStart[fi];
            size_t e = (fi + 1 < sv.diffStart.size())
                           ? sv.diffStart[fi + 1] : sv.diffs.size();
            // build expected map
            std::unordered_map<uint32_t, uint32_t> exp;
            for (size_t i = b; i < e; ++i) exp[sv.diffs[i].first] = sv.diffs[i].second;
            // actual diffs
            std::unordered_map<uint32_t, uint32_t> act;
            for (uint32_t a = 0; a < 0x700; ++a) {
                uint32_t xv = vf.getWord("x", a);
                uint32_t yv = vf.getWord("y", a);
                if (xv != sv.sx[a]) act[a] = xv;
                if (yv != sv.sy[a]) act[(0x1000000u) | a] = yv;
            }
            // compare
            for (const auto& kv : exp) {
                ++checks;
                auto it = act.find(kv.first);
                if (it == act.end() || it->second != kv.second) {
                    ++mismatches;
                    ++setMism;
                    if (shown < MAXSHOW) {
                        ++shown;
                        uint32_t a = kv.first & 0xFFFFFF;
                        fprintf(stderr,
                                "  MISMATCH set %zu (%s) frame %zu cell %s$%03X:"
                                " exp %06X got %s\n",
                                si, sv.tag.c_str(), fi,
                                (kv.first & 0x1000000) ? "Y" : "X", a,
                                kv.second,
                                it == act.end() ? "<seed>" :
                                    [] (uint32_t v) {
                                        static char b[8];
                                        snprintf(b, 8, "%06X", v);
                                        return b;
                                    }(it->second));
                    }
                }
            }
            for (const auto& kv : act) {
                if (exp.find(kv.first) == exp.end()) {
                    ++checks;
                    ++mismatches;
                    ++setMism;
                    if (shown < MAXSHOW) {
                        ++shown;
                        uint32_t a = kv.first & 0xFFFFFF;
                        fprintf(stderr,
                                "  EXTRA set %zu (%s) frame %zu cell %s$%03X:"
                                " got %06X (exp seed)\n",
                                si, sv.tag.c_str(), fi,
                                (kv.first & 0x1000000) ? "Y" : "X", a,
                                kv.second);
                    }
                }
            }
            checks += 0; // cells equal to seed on both sides already counted
        }
        if (setMism) ++setsWithMism;
        if (si % 20 == 0)
            fprintf(stderr, "  set %zu/%zu (%s)\n", si, g_sets.size(),
                    sv.tag.c_str());
    }
    printf("checks=%lld mismatches=%lld sets_with_mism=%d\n",
           (long long)checks, (long long)mismatches, setsWithMism);
    if (mismatches == 0)
        printf("[100%% VOICE-FRAME BIT-EXACT]\n");
    return mismatches ? 1 : 0;
}
