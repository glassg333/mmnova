// test_fm_filter_env.cpp — бит-точность MnmFilterStage2 против векторов
// эмулятора OS 1.32 (fenv_vectors.txt, источник exp18_fdn_snap.json).
// На каждый кадр: L1 -> L2 -> L3 -> DP, сравнение слово-в-слово:
//   E1 (36) : Y:$62-$81 + стейт P+$D4/P+$D3      после L1
//   E2 (34) : Y:$20-$3F + стейт P+$D5            после L2
//   E3 (34) : Y:$62-$81 + стейт P+$CC            после L3
//   E4 (34) : X:$40-$61                           после DP
// Exit code 0 только при 100% совпадении.
#include "MnmFilterStage2.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace mnmfm;

struct Cfg { int atk, dec, bofs, wofs; };

static bool loadVectors(const char* path, std::vector<Cfg>& cfgs,
                        std::vector<std::vector<std::vector<std::vector<int64_t>>>>& frames) {
    // frames[cfg][frame] = {IN(58), E1(36), E2(34), E3(34), E4(34)}
    std::ifstream f(path);
    if (!f) { fprintf(stderr, "cannot open %s\n", path); return false; }
    std::string line;
    Cfg cur{};
    int curCfg = -1, curFrame = -1, stage = -1;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (line.rfind("CFG ", 0) == 0) {
            sscanf(line.c_str(), "CFG %d ATK=%d DEC=%d BOFS=%d WOFS=%d",
                   (int*)&cur.atk, &cur.atk, &cur.dec, &cur.bofs, &cur.wofs);
            cfgs.push_back(cur);
            frames.emplace_back();
            curCfg = (int)cfgs.size() - 1;
            curFrame = -1;
            stage = -1;
        } else if (curCfg < 0) {
            continue;
        } else if (line == "F") {
            frames[curCfg].emplace_back();
            curFrame = (int)frames[curCfg].size() - 1;
            stage = 0;
        } else if (curFrame < 0) {
            continue;
        } else if (line.rfind("IN ", 0) == 0 || line.rfind("E1 ", 0) == 0 ||
                   line.rfind("E2 ", 0) == 0 || line.rfind("E3 ", 0) == 0 ||
                   line.rfind("E4 ", 0) == 0) {
            std::istringstream ss(line.substr(3));
            std::vector<int64_t> vals;
            int64_t v;
            while (ss >> v) vals.push_back(v);
            frames[curCfg][curFrame].push_back(std::move(vals));
            ++stage;
        }
    }
    return true;
}

static long long words = 0, badWords = 0;

static void cmp(const char* tag, int cfg, int fr, uint32_t got, int64_t exp) {
    ++words;
    if ((int64_t)got != exp) {
        ++badWords;
        if (badWords <= 12)
            printf("  BAD cfg%d f%d %s: got %06llX exp %06llX\n", cfg, fr, tag,
                   (unsigned long long)got, (unsigned long long)exp);
    }
}

int main(int argc, char** argv) {
    const char* vecpath = argc > 1 ? argv[1] :
        "/home/z/my-project/work/fm_fenv/fenv_vectors.txt";
    std::vector<Cfg> cfgs;
    std::vector<std::vector<std::vector<std::vector<int64_t>>>> frames;
    if (!loadVectors(vecpath, cfgs, frames)) return 2;

    for (size_t ci = 0; ci < cfgs.size(); ++ci) {
        MnmFilterStage2 st;
        long long w0 = words, b0 = badWords;
        for (size_t fi = 0; fi < frames[ci].size(); ++fi) {
            const auto& fr = frames[ci][fi];
            if (fr.size() < 5) continue;
            const auto& IN = fr[0];
            MnmFilterStage2::In in;
            for (int i = 0; i < 16; ++i) {
                in.x00[i] = (uint32_t)IN[i];
                in.x10[i] = (uint32_t)IN[16 + i];
                in.x20[i] = (uint32_t)IN[32 + i];
                in.x30[i] = (uint32_t)IN[48 + i];
            }
            in.atk = (uint32_t)IN[64]; in.dec = (uint32_t)IN[65];
            in.bofs = (uint32_t)IN[66]; in.wofs = (uint32_t)IN[67];
            in.phase = (uint32_t)IN[68];
            in.cfy = (uint32_t)IN[77]; in.cfx = (uint32_t)IN[78];
            // порядок стейтов в IN: D3Y D3X D4Y D4X D5Y D5X CCY CCX CFY CFX
            st.d3y = (uint32_t)IN[69];  st.d3x = (uint32_t)IN[70];
            st.d4y = (uint32_t)IN[71];  st.d4x = (uint32_t)IN[72];
            st.d5y = (uint32_t)IN[73];  st.d5x = (uint32_t)IN[74];
            st.ccy = (uint32_t)IN[75];  st.ccx = (uint32_t)IN[76];
            // (IN[77]=CFY, IN[78]=CFX — читаются хвостом $0AD1+, тут не нужны)
            MnmFilterStage2::Out out;
            const auto& E1 = fr[1]; const auto& E2 = fr[2];
            const auto& E3 = fr[3]; const auto& E4 = fr[4];
            // пошагово: L3 перезаписывает Y62, поэтому сверяем каждую стадию
            st.runL1(in, out);
            for (int i = 0; i < 32; ++i) cmp("L1.Y62", ci, fi, out.Y62[i], E1[i]);
            cmp("L1.d4y", ci, fi, st.d4y, E1[32]);
            cmp("L1.d4x", ci, fi, st.d4x, E1[33]);
            cmp("L1.d3y", ci, fi, st.d3y, E1[34]);
            cmp("L1.d3x", ci, fi, st.d3x, E1[35]);
            st.runL2(in, out);
            for (int i = 0; i < 32; ++i) cmp("L2.Y20", ci, fi, out.Y20[i], E2[i]);
            cmp("L2.d5y", ci, fi, st.d5y, E2[32]);
            cmp("L2.d5x", ci, fi, st.d5x, E2[33]);
            st.runL3(in, out);
            for (int i = 0; i < 32; ++i) cmp("L3.Y62", ci, fi, out.Y62[i], E3[i]);
            cmp("L3.ccy", ci, fi, st.ccy, E3[32]);
            cmp("L3.ccx", ci, fi, st.ccx, E3[33]);
            st.runDP(in, out);
            for (int i = 0; i < 0x22; ++i) {
                uint32_t got = (i < 17) ? out.bankB[i] : out.bankA[i - 17];
                cmp("DP.bank", ci, fi, got, E4[i]);
            }
        }
        printf("CFG %d (ATK=%d DEC=%d BOFS=%d WOFS=%d): words %lld, mism %lld\n",
               (int)ci, cfgs[ci].atk, cfgs[ci].dec, cfgs[ci].bofs, cfgs[ci].wofs,
               words - w0, badWords - b0);
    }
    printf("\nTOTAL: %lld words, mismatches = %lld", words, badWords);
    if (badWords == 0) { printf("  [100%% BIT-EXACT]\n"); return 0; }
    printf("  [MISMATCH]\n");
    return 1;
}
