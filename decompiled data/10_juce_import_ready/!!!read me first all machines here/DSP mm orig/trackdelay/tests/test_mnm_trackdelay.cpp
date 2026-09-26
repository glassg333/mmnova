// test_mnm_trackdelay.cpp — vector tests for MnmTrackDelayExact/MnmSrrExact
// against the DSP56300 emulator captures (OS 1.32B, session 2026-09-25).
//
// Build:  g++ -O2 -std=c++17 -I.. -o test_mnm_trackdelay test_mnm_trackdelay.cpp
// Run:    ./test_mnm_trackdelay [path/to/tdelay_vectors.json]
//
// The JSON vectors are parsed with a tiny hand-rolled reader (no deps).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "MnmTrackDelayExact.hpp"
#include "MnmSrrExact.hpp"

static int g_fail = 0, g_ok = 0;

static void check(const char* name, long long got, long long want,
                  long long tol = 0) {
    long long d = got - want;
    if (d < 0) d = -d;
    if (d <= tol) { ++g_ok; }
    else {
        ++g_fail;
        printf("FAIL %s: got %lld want %lld (d=%lld)\n", name, got, want, d);
    }
}

// ---- minimal JSON vector access -------------------------------------------
struct Jv {
    std::string src;
    size_t pos = 0;
    explicit Jv(const std::string& s) : src(s) {}
    void ws() { while (pos < src.size() && isspace((unsigned char)src[pos])) ++pos; }
    bool eat(char c) { ws(); if (pos < src.size() && src[pos] == c) { ++pos; return true; } return false; }
    std::string key() {
        ws(); std::string k;
        if (pos < src.size() && src[pos] == '"') { ++pos;
            while (pos < src.size() && src[pos] != '"') k += src[pos++]; ++pos; }
        eat(':'); return k;
    }
    std::string str() {
        ws(); std::string k;
        if (pos < src.size() && src[pos] == '"') { ++pos;
            while (pos < src.size() && src[pos] != '"') k += src[pos++]; ++pos; }
        return k;
    }
    long long num() { ws(); long long v = 0; bool neg = false;
        if (pos < src.size() && src[pos] == '-') { neg = true; ++pos; }
        while (pos < src.size() && isdigit((unsigned char)src[pos])) v = v * 10 + (src[pos++] - '0');
        return neg ? -v : v; }
    // iterate an object
    bool more() { ws(); return pos < src.size() && src[pos] != '}' && src[pos] != ']'; }
};

static std::string load_file(const char* p) {
    std::ifstream f(p);
    std::stringstream ss; ss << f.rdbuf();
    return ss.str();
}

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "tdelay_vectors.json";
    std::string js = load_file(path);
    if (js.empty()) {
        printf("no vectors (%s) — running self-consistency checks only\n", path);
    }

    // ---------------------------------------------------------------
    printf("== 1. delay target law: closed form vs emulator vectors ==\n");
    {
        // closed form: CE(v,T) = (v+1) * floor(7938000*T/2^18) * 2^16
        // reference points measured in the emulator (exp64/exp65):
        check("CE(0,120)", mnm_td::delayTarget48(0 << 16, 120), 0x0E310000LL);
        check("CE(32,120)", mnm_td::delayTarget48(32 << 16, 120), 33LL * 3633 * 65536);
        // tempo 60: y0 = floor(7938000*60/2^18) = floor(1816.99) = 1816
        check("CE(0,60)", mnm_td::delayTarget48(0 << 16, 60),
              1816LL * 65536);
        // CE(127,120): the ROM table's last entry is 0x3FFFFF (not 0x400000)
        check("CE(127,120)", mnm_td::delayTarget48(127 << 16, 120),
              (int64_t)mnm_td::kDtim1[127] * 3633 * 2);
    }
    if (!js.empty()) {
        // full 128x3 table from the capture
        Jv p(js);
        if (p.eat('{')) {
            while (p.more()) {
                std::string k = p.key();
                if (k == "dtim") {
                    p.eat('{');
                    while (p.more()) {
                        std::string tempo = p.key();
                        int T = atoi(tempo.c_str());
                        p.eat('[');
                        for (int v = 0; v < 128; ++v) {
                            long long want = p.num();
                            if (v < 128) p.eat(',');
                            long long got = mnm_td::delayTarget48(v << 16, T);
                            // emulator captures may include the one-pole
                            // smoothed path; tolerate 48-bit LSB noise
                            check("dtim table", got, want, 256);   // emu capture noise <= y0/64 (~2^-37 rel)
                        }
                        p.eat(']');
                        p.eat(',');
                    }
                    p.eat('}');
                } else {
                    // skip value: string | number | array | object
                    p.eat('{'); p.eat('['); p.eat('"');
                    if (p.more()) { p.num(); }
                    // crude skip to matching close
                    int depth = 1;
                    while (depth > 0 && p.pos < js.size()) {
                        char c = js[p.pos++];
                        if (c == '{' || c == '[') ++depth;
                        else if (c == '}' || c == ']') --depth;
                    }
                    p.eat(',');
                }
            }
        }
    }

    // ---------------------------------------------------------------
    printf("== 2. glide slew: +4<<24 per block, clamp ==\n");
    {
        mnm_td::GlideState g;
        g.raw = mnm_td::kOne24;
        g.smoothed = mnm_td::kOne24;   // OS-prime
        g.primed = 1;
        g.current = 0;
        int64_t target = 1000LL << 24;
        for (int i = 0; i < 5; ++i)
            mnm_td::glideBlock(g, target);
        // after 5 blocks: 5 * (4<<24) = 20<<24
        check("glide 5 blocks", g.current, 20LL << 24);
        // settle
        for (int i = 0; i < 300; ++i) mnm_td::glideBlock(g, target);
        check("glide settle", g.current, target);
        // down-step clamp
        for (int i = 0; i < 3; ++i) mnm_td::glideBlock(g, 0);
        check("glide down 3 blocks", g.current, target - 3LL * (4LL << 24));
    }

    // ---------------------------------------------------------------
    printf("== 3. fractional tap primitive (func_000397 shape) ==\n");
    {
        // frac=0 -> x1; frac=$7FFFFF -> ~midpoint (rounded)
        check("tap f0", mnm_td::fracTap(1000, 2000, 0), 1000);
        // 0 + 0.99999988*0.99999988 = 0.99999977 -> 8388606
        check("tap fmax", mnm_td::fracTap(0, 0x7FFFFF, 0x7FFFFF), 8388606, 1);
    }

    // ---------------------------------------------------------------
    printf("== 4. EFX envelope: hold level = SRR^2>>24 ==\n");
    {
        mnm_td::EfxEnv e;
        e.env = 0x7FFFFF;
        // TRIG 2 -> release multiplies by kEnvDecay[0] (0x8d1897 = -0.898)
        e.block(2, 0, 0, 48 << 16, 0);
        // release: env = -env * kEnvDecay[DTIM=0] << 1 (24-bit wrap)
        int32_t dec = mnm_td::s24((int32_t)mnm_td::kEnvDecay[0]);
        int32_t want = (int32_t)((-(int64_t)(uint32_t)0x7FFFFF * (int64_t)dec << 1) & 0xFFFFFF);
        check("release kEnvDecay[0]", e.env, want);
        // hold: phase 5 -> env = SRR^2 * 256
        mnm_td::EfxEnv h; h.phase = 5;
        h.block(0, 0, 0, 64 << 16, 0);
        check("hold SRR=64", h.env, 64LL * 64 * 256);
        h.phase = 5;
        h.block(0, 0, 0, 48 << 16, 0);
        check("hold SRR=48", h.env, 48LL * 48 * 256);
    }

    // ---------------------------------------------------------------
    printf("== 5. SRR ramp shape (monotone, 16 words, step $10*X0) ==\n");
    {
        int32_t ramp[16];
        mnm_td::srrRamp(0x0AFB0C, 64 << 16, ramp);
        int32_t e = 0x0AFB0C;
        int64_t d = (int64_t)(uint32_t)(64 << 16);
        int32_t x0 = (int32_t)(((d * d) >> 24) & 0xFFFFFF);
        int64_t ee = (int64_t)(uint32_t)e;
        int32_t x1 = (int32_t)(((ee * ee) >> 24) & 0xFFFFFF);
        int64_t prod = ((int64_t)x1 * (int64_t)x0) << 1;
        int32_t X0 = (int32_t)((prod >> 24) & 0xFFFFFF);
        // store semantics: 48-bit accumulator, a1 (>>24) written to the cell
        for (int k = 0; k < 8; ++k) {
            int64_t acc = ((int64_t)(uint32_t)e << 24) + (int64_t)(k + 1) * mnm_td::mpy(0x10, X0);
            check("ramp lo", ramp[k], (acc >> 24) & 0xFFFFFF, 0);
            int64_t acc2 = ((int64_t)(uint32_t)e << 24) + mnm_td::mpysu(0x080000, (uint32_t)X0)
                           + (int64_t)(k + 1) * mnm_td::mpy(0x10, X0);
            check("ramp hi", ramp[k + 8], (acc2 >> 24) & 0xFFFFFF, 0);
        }
    }

    // ---------------------------------------------------------------
    printf("== 6. TrackDelay end-to-end sanity ==\n");
    {
        mnm_td::TrackDelay td;
        mnm_td::TrackDelayParams p;
        p.cell13 = 32 << 16;
        p.send = 1.f; p.feedback = 0.f;
        int32_t inL[16], inR[16], oL[16], oR[16];
        for (int f = 0; f < 16; ++f) { inL[f] = 0x100000; inR[f] = 0x100000; }
        td.process(inL, inR, oL, oR, p);
        // with send=1 and delay ~ (33*3633)>>24 = 4495 words >> block, the
        // echo of block 0 cannot return within block 0 -> out = dry
        check("dry passthrough", oL[0], 0x100000);
        // run ~4700 blocks so the ring wraps and the echo returns
        for (int b = 0; b < 4600; ++b) td.process(inL, inR, oL, oR, p);
        // now the output must contain dry + echo (> dry alone)
        if (oL[0] > 0x100000) ++g_ok; else { ++g_fail; printf("FAIL echo return: %d\n", oL[0]); }
        // silence in -> echo decays to silence (feedback 0)
        for (int f = 0; f < 16; ++f) { inL[f] = 0; inR[f] = 0; }
        for (int b = 0; b < 100; ++b) td.process(inL, inR, oL, oR, p);
        if (oL[0] == 0) ++g_ok; else { ++g_fail; printf("FAIL echo decay: %d\n", oL[0]); }
    }

    printf("\nOK=%d FAIL=%d -> %s\n", g_ok, g_fail, g_fail ? "FIX ME" : "ALL PASSED");
    return g_fail ? 1 : 0;
}
