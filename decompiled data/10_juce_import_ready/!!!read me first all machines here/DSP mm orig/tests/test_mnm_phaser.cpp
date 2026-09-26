// test_mnm_phaser.cpp — word-exact check of mnm_phaser.h against emulator
// reference vectors (test_vectors.h). Build:
//   g++ -std=c++17 -O2 -I. -o test_mnm_phaser test_mnm_phaser.cpp && ./test_mnm_phaser
#include "mnm_phaser.h"
#include "test_vectors.h"
#include <cstdio>

int main() {
    int ok = 0, bad = 0;
    mnm::PhaserState st;
    for (int b = 0; b < mnmtest::kNumBlocks; ++b) {
        const auto& blk = mnmtest::kBlocks[b];
        if (blk.reset) st = mnm::PhaserState{};
        mnm::PhaserParams prm;
        for (int i = 0; i < 8; ++i) prm[i] = blk.prm[i];
        int32_t out[32];
        mnm::phaser_block(st, {mnmtest::kSinTab, mnmtest::kCosTab}, prm,
                          blk.in, out);
        for (int i = 0; i < 32; ++i) {
            if ((out[i] & 0xFFFFFF) == (blk.out[i] & 0xFFFFFF)) ++ok;
            else {
                ++bad;
                if (bad <= 8)
                    printf("block %d word %d: got %06X want %06X\n",
                           b, i, out[i] & 0xFFFFFF, blk.out[i] & 0xFFFFFF);
            }
        }
    }
    printf("mnm_phaser.h: OK=%d BAD=%d of %d words\n",
           ok, bad, mnmtest::kNumBlocks * 32);
    return bad ? 1 : 0;
}
