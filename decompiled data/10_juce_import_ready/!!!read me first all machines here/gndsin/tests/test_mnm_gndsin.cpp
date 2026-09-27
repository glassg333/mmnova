// test_mnm_gndsin.cpp — m1 vs emulator vectors (exp79).
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "../MnmGndSinExact.hpp"
#include "../reverb/MnmSinTable.hpp"

static int ok = 0, fail = 0;
using namespace mngnd;

// alias the sin table
namespace mngnd { int32_t kSinTabG[8192]; }
struct SinInit { SinInit() { memcpy(kSinTabG, mnmrev::kSinTab, sizeof(kSinTabG)); } } sinInit;

#include "m1_vectors.h"

int main() {
    for (size_t v = 0; v < g_vcount; v++) {
        GndSinExact osc; osc.reset();
        for (int b = 0; b < 4; b++) {
            int32_t out[32];
            osc.process((int64_t)g_vecs[v].pitch, out);
            for (int k = 0; k < 32; k++) {
                if (out[k] != g_vecs[v].blocks[b][k]) {
                    if (fail < 10)
                        printf("FAIL v%zu blk%d s%d want %06x got %06x\n", v, b, k,
                               g_vecs[v].blocks[b][k] & 0xFFFFFF, out[k] & 0xFFFFFF);
                    fail++;
                } else ok++;
            }
        }
    }
    printf("gndsin exact: OK=%d FAIL=%d\n", ok, fail);
    return fail ? 1 : 0;
}
