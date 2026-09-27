#include "MnmFmExact.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
struct Vec { int knobs[8]; int pitch; std::vector<std::vector<int>> outs; std::vector<int> states; };
static std::vector<Vec> load(const char* path) {
    std::ifstream f(path); std::vector<Vec> out; Vec cur; std::string line;
    while (std::getline(f, line)) {
        std::istringstream ss(line); char tag; ss >> tag;
        if (tag == 'V') { for (int i=0;i<8;i++) ss >> cur.knobs[i]; ss >> cur.pitch; int b; ss >> b; cur.outs.clear(); cur.states.clear(); }
        else if (tag == 'O') { std::vector<int> blk(32); for (auto& x : blk) ss >> x; cur.outs.push_back(blk); }
        else if (tag == 'S') { for (int i=0;i<0x25;i++){int x;ss>>x;cur.states.push_back(x);} out.push_back(cur); }
    }
    return out;
}
int main(int argc, char** argv) {
    auto vecs = load(argc>1?argv[1]:"/home/z/my-project/work/fm_fix/par_vectors.txt");
    mnmfm::initSineTable();
    int pass=0,total=0;
    for (size_t vi=0; vi<vecs.size(); ++vi) {
        Vec& v = vecs[vi];
        mnmfm::FmParVoice st;
        for (size_t bi=0; bi<v.outs.size(); ++bi) {
            int32_t out[32];
            st.process(v.knobs, v.pitch, out);
            total++;
            int bad=0, first=-1;
            for (int i=0;i<32;i++) if (out[i]!=v.outs[bi][i]) { bad++; if(first<0) first=i; }
            if (!bad) pass++;
            else if (bi==0) printf("vec %zu blk %zu: %2d/32 wrong first@%d: got %06X want %06X\n",
                vi, bi, bad, first, out[first]&0xFFFFFF, v.outs[bi][first]&0xFFFFFF);
        }
        printf("vec %zu states:", vi);
        for (int o=0x10;o<0x35;o++) {
            int got = st.w[o]&0xFFFFFF; int want = v.states[o-0x10];
            if (got!=want) printf(" [%02X]%06X!=%06X", o, got, want);
        }
        printf("\n");
    }
    printf("=== PAR blocks matching: %d / %d ===\n", pass, total);
    return 0;
}
