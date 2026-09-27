// ============================================================================
// test_readable.cpp — проверка ЧИТАЕМОГО порта цепи против оракула.
//
//   ./test_readable <oracle_dataset_dir> [last_frame]
//
// dataset_dir: work/chain_oracle/A_neutral (и др.). Использует:
//   full_x.bin / full_y.bin — полный образ памяти до кадра 0
//   frames.txt              — покадровые pokes (TRIG, шины, инжекция)
//   expected.txt            — мастер-выход Y:$00-$1F каждого кадра
//   stage_dump.bin          — состояние X/Y/регистров на 20 границах секций
//
// Проверка: после pokes кадра f читаемый код обязан дать СОСТОЯНИЕ,
// идентичное оракулу на каждой достигнутой границе, и мастер-выход кадра.
// Любое расхождение печатается с адресом/регистром и PC границы.
// ============================================================================
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include "mnm_chain_a.hpp"
#include "mnm_chain_b.hpp"
#define MNM_CHAIN_CHK
#include "mnm_chain_gen.hpp"
namespace mnmchain { void (*g_chk)(uint32_t) = nullptr; }
#include "mnm_chain_tables.hpp"

using namespace mnmchain;
using mnmfix::Mem;
using mnmfix::Regs;
using mnmfix::M24;

// ------------------------- оракул: stage_dump.bin ---------------------------
struct StageDump {
    uint32_t nb, nf, lo, hi;
    std::vector<uint32_t> pcs;
    struct Frame {
        std::vector<uint8_t> flags;      // [nb]
        std::vector<int32_t> mo, out;    // [34], [32]
        struct Bnd { std::vector<int32_t> x, y, r, n, m, x0123; int64_t A, B; int32_t sr; };
        std::vector<Bnd> bnds;           // только достигнутые, в порядке pcs
    };
    std::vector<Frame> frames;
    bool load(const std::string& path) {
        std::ifstream f(path, std::ios::binary);
        if (!f) return false;
        char magic[4]; f.read(magic, 4);
        if (memcmp(magic, "MSTG", 4) != 0) return false;
        uint16_t ver; f.read((char*)&ver, 2);
        f.read((char*)&nb, 4); f.read((char*)&nf, 4); f.read((char*)&lo, 4); f.read((char*)&hi, 4);
        pcs.resize(nb); f.read((char*)pcs.data(), 4 * nb);
        const uint32_t W = hi - lo;
        frames.resize(nf);
        for (uint32_t t = 0; t < nf; ++t) {
            Frame& fr = frames[t];
            fr.flags.resize(nb); f.read((char*)fr.flags.data(), nb);
            fr.mo.resize(34); f.read((char*)fr.mo.data(), 4 * 34);
            fr.out.resize(32); f.read((char*)fr.out.data(), 4 * 32);
            for (uint32_t b = 0; b < nb; ++b) {
                if (!fr.flags[b]) continue;
                Frame::Bnd bd;
                bd.x.resize(W); f.read((char*)bd.x.data(), 4 * W);
                bd.y.resize(W); f.read((char*)bd.y.data(), 4 * W);
                bd.r.resize(8); f.read((char*)bd.r.data(), 32);
                bd.n.resize(8); f.read((char*)bd.n.data(), 32);
                bd.m.resize(8); f.read((char*)bd.m.data(), 32);
                bd.x0123.resize(4); f.read((char*)bd.x0123.data(), 16);
                f.read((char*)&bd.A, 8); f.read((char*)&bd.B, 8); f.read((char*)&bd.sr, 4);
                fr.bnds.push_back(std::move(bd));
            }
        }
        return true;
    }
};

// ------------------------- frames.txt / expected.txt ------------------------
struct FramesFile {
    struct Frame { char sp; uint32_t ea; uint32_t val; };
    std::vector<std::vector<Frame>> pokes;
    bool load(const std::string& path) {
        std::ifstream f(path);
        if (!f) return false;
        std::string line;
        while (std::getline(f, line)) {
            if (line.rfind("FRAME", 0) == 0) { pokes.emplace_back(); continue; }
            if (pokes.empty() || line.size() < 2) continue;
            char sp = (char)tolower((unsigned char)line[0]);
            if (sp != 'x' && sp != 'y') continue;
            std::istringstream ss(line.substr(1).c_str());
            unsigned ea = 0, val = 0;
            ss >> std::hex >> ea >> std::hex >> val;
            pokes.back().push_back({sp, ea, val});
        }
        return true;
    }
};

static bool loadExpected(const std::string& path, std::vector<std::vector<int32_t>>& out) {
    std::ifstream f(path);
    if (!f) return false;
    std::string line;
    while (std::getline(f, line)) {
        if (line.rfind("OUT", 0) != 0) continue;
        std::istringstream ss(line.substr(3));
        int fidx; ss >> fidx >> std::dec;
        std::vector<int32_t> v(32);
        for (int i = 0; i < 32; ++i) ss >> v[i] >> std::dec;
        if ((int)out.size() <= fidx) out.resize(fidx + 1);
        out[fidx] = v;
    }
    return true;
}

// ------------------------- сравнение состояния ------------------------------
static int g_mismatches = 0;
static std::vector<uint32_t> g_badset;          // (spaceY<<24)|addr mismatches текущей границы
static std::vector<uint32_t> g_prevbad;         // предыдущей границы (внутри кадра)

static int cmp_state(uint32_t pc, uint32_t frame, const StageDump::Frame::Bnd& bd,
                     const Mem& M, const Regs& g, const StageDump::Frame& fr, int bidx) {
    int bad = 0;
    char buf[256];
    const uint32_t W = M.WIN;   // дамп снят с окном 0..0x800
    g_badset.clear();
    for (uint32_t a = 0; a < W; ++a) {
        const uint32_t ox = (uint32_t)bd.x[a] & M24, oy = (uint32_t)bd.y[a] & M24;
        const uint32_t mx = M.X[a] & M24, my = M.Y[a] & M24;
        if (ox != mx) {
            if (bad < 12) { snprintf(buf, sizeof buf, "  X:%03X oracle=%06X mine=%06X", a, ox, mx); fprintf(stderr, "%s\n", buf); }
            g_badset.push_back(a); ++bad;
        }
        if (oy != my) {
            if (bad < 12) { snprintf(buf, sizeof buf, "  Y:%03X oracle=%06X mine=%06X", a, oy, my); fprintf(stderr, "%s\n", buf); }
            g_badset.push_back(0x1000000u | a); ++bad;
        }
        if (bad >= 4096) break;
    }
    for (int i = 0; i < 8; ++i) {
        if (((uint32_t)bd.r[i] & M24) != (g.r[i] & M24)) { if (bad < 40) fprintf(stderr, "  r%d oracle=%06X mine=%06X\n", i, (uint32_t)bd.r[i], g.r[i]); ++bad; }
        if (((uint32_t)bd.n[i] & M24) != ((uint32_t)g.n[i] & M24)) { if (bad < 40) fprintf(stderr, "  n%d oracle=%06X mine=%06X\n", i, (uint32_t)bd.n[i], (uint32_t)g.n[i]); ++bad; }
        if (((uint32_t)bd.m[i] & M24) != (g.m[i] & M24)) { if (bad < 40) fprintf(stderr, "  m%d oracle=%06X mine=%06X\n", i, (uint32_t)bd.m[i], g.m[i]); ++bad; }
    }
    const uint32_t ox0 = (uint32_t)bd.x0123[0] & M24, ox1 = (uint32_t)bd.x0123[1] & M24;
    const uint32_t oy0 = (uint32_t)bd.x0123[2] & M24, oy1 = (uint32_t)bd.x0123[3] & M24;
    if (ox0 != g.x0 || ox1 != g.x1 || oy0 != g.y0 || oy1 != g.y1) {
        if (bad < 40) fprintf(stderr, "  xy regs oracle=(%06X %06X %06X %06X) mine=(%06X %06X %06X %06X)\n",
                             ox0, ox1, oy0, oy1, g.x0, g.x1, g.y0, g.y1);
        ++bad;
    }
    if (bd.A != g.A) { if (bad < 40) fprintf(stderr, "  A oracle=%014llX mine=%014llX\n", (unsigned long long)bd.A, (unsigned long long)g.A); ++bad; }
    if (bd.B != g.B) { if (bad < 40) fprintf(stderr, "  B oracle=%014llX mine=%014llX\n", (unsigned long long)bd.B, (unsigned long long)g.B); ++bad; }
    // дельта: адреса, расходящиеся здесь, но не на предыдущей границе
    std::vector<uint32_t> fresh;
    for (uint32_t k : g_badset)
        if (std::find(g_prevbad.begin(), g_prevbad.end(), k) == g_prevbad.end())
            fresh.push_back(k);
    if (getenv("MNM_REGS")) {
        fprintf(stderr, "  REGS f%u %04X oracle r:", frame, pc);
        for (int i = 0; i < 8; ++i) fprintf(stderr, " %06X", (uint32_t)bd.r[i] & M24);
        fprintf(stderr, " | mine r:");
        for (int i = 0; i < 8; ++i) fprintf(stderr, " %06X", g.r[i] & M24);
        fprintf(stderr, "\n    n o:"); for (int i = 0; i < 8; ++i) fprintf(stderr, " %06X", (uint32_t)bd.n[i] & M24);
        fprintf(stderr, " m o:"); for (int i = 0; i < 8; ++i) fprintf(stderr, " %06X", (uint32_t)bd.m[i] & M24);
        fprintf(stderr, "\n    n m:"); for (int i = 0; i < 8; ++i) fprintf(stderr, " %06X", (uint32_t)g.n[i] & M24);
        fprintf(stderr, " m m:"); for (int i = 0; i < 8; ++i) fprintf(stderr, " %06X", g.m[i] & M24);
        fprintf(stderr, "\n    A o=%014llX m=%014llX B o=%014llX m=%014llX\n",
            (unsigned long long)bd.A, (unsigned long long)g.A, (unsigned long long)bd.B, (unsigned long long)g.B);
    }
    if (bad) {
        fprintf(stderr, "FRAME %u BOUNDARY %04X: bad=%d fresh=%d:", frame, pc, bad, (int)fresh.size());
        for (size_t i = 0; i < fresh.size() && i < 10; ++i)
            fprintf(stderr, " %c:%03X", (fresh[i] & 0x1000000) ? 'Y' : 'X', fresh[i] & 0xFFFFFF);
        fprintf(stderr, "\n");
        g_mismatches += bad;
    }
    g_prevbad = g_badset;
    return bad;
}

// мост для CHK-границ внутри сгенерированного tail (заполняется в кадре)
static std::function<void(uint32_t)> g_chk_fn;
static void g_chk_impl(uint32_t pc) { if (g_chk_fn) g_chk_fn(pc); }

// ------------------------- главный цикл -------------------------------------
int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <dataset_dir> [last_frame]\n", argv[0]); return 2; }
    const std::string dir = argv[1];
    int last_frame = (argc > 2) ? atoi(argv[2]) : 1 << 30;

    StageDump sd;
    if (!sd.load(dir + "/stage_dump.bin")) { fprintf(stderr, "no stage_dump.bin\n"); return 2; }
    FramesFile ff;
    if (!ff.load(dir + "/frames.txt")) { fprintf(stderr, "no frames.txt\n"); return 2; }
    std::vector<std::vector<int32_t>> exp;
    loadExpected(dir + "/expected.txt", exp);

    Mem M; Regs g; const mnmfix::Rom& rom = kRom;
    // полный образ памяти
    {
        std::ifstream fx(dir + "/full_x.bin", std::ios::binary), fy(dir + "/full_y.bin", std::ios::binary);
        std::vector<uint32_t> img(M.WIN);
        fx.read((char*)img.data(), 4 * M.WIN); memcpy(M.X, img.data(), 4 * M.WIN);
        fy.read((char*)img.data(), 4 * M.WIN); memcpy(M.Y, img.data(), 4 * M.WIN);
        // регистры кадра 0 берём из дампа границы 02EC (вход цепи)
    }

    int total_checks = 0;
    for (uint32_t fr = 0; fr < sd.nf && (int)fr < last_frame; ++fr) {
        for (const auto& pk : ff.pokes[fr]) M.wr(pk.sp, pk.ea, pk.val);
        const StageDump::Frame& of = sd.frames[fr];

        // Вход кадра = состояние оракула на границе $02EC (эффект преамбулы
        // и машинной половины — вне скоупа читаемого порта). Секционная
        // проверка ниже остаётся чистой: любое расхождение = баг секции.
        {
            const auto& b0 = of.bnds[0];
            for (int i = 0; i < 8; ++i) { g.r[i] = (uint32_t)b0.r[i]; g.n[i] = b0.n[i]; g.m[i] = (uint32_t)b0.m[i]; }
            g.x0 = (uint32_t)b0.x0123[0] & M24; g.x1 = (uint32_t)b0.x0123[1] & M24;
            g.y0 = (uint32_t)b0.x0123[2] & M24; g.y1 = (uint32_t)b0.x0123[3] & M24;
            g.A = b0.A; g.B = b0.B;
            for (uint32_t a2 = 0; a2 < M.WIN; ++a2) { M.X[a2] = (uint32_t)b0.x[a2] & M24; M.Y[a2] = (uint32_t)b0.y[a2] & M24; }
            g_prevbad.clear();
            if (getenv("MNM_DBG")) fprintf(stderr, "  adopt f%u: bnds0 pc=%04X X400=%06X Y4FF=%06X Y123=%06X\n",
                fr, sd.pcs[0], M.X[0x400], M.Y[0x4FF], M.Y[0x123]);
        }

        // ---- читаемая цепь: секции в порядке исполнения прошивки ----
        int bidx = 0;
        auto check = [&](uint32_t pc) {
            if (bidx >= (int)sd.nb || sd.pcs[bidx] != pc || !of.flags[bidx]) {
                fprintf(stderr, "FRAME %u: oracle boundary %04X not found (bidx=%d)\n", fr, pc, bidx);
                ++g_mismatches; return;
            }
            cmp_state(pc, fr, of.bnds[bidx], M, g, of, bidx);
            ++total_checks; ++bidx;
        };
        if (getenv("MNM_REGS") && fr == 0)
            fprintf(stderr, "  f0 in: X400=%06X Y421=%06X Y428=%06X Y418=%06X Y4FF=%06X Y124=%06X X2C3=%06X\n",
                    M.X[0x400], M.Y[0x421], M.Y[0x428], M.Y[0x418], M.Y[0x4FF], M.Y[0x124], M.X[0x2C3]);
        check(0x02EC);
        sec_entry(M, g);
        check(0x04A8);
        sec_adsr(M, rom, g);
        check(0x04F5);
        sec_machcopy(M, g);
        // границы между $04FF и $0537 в оракуле нет — дист-энвелоп целиком
        sec_distenv(M, rom, g);
        check(0x0537);
        sec_coefring(M, rom, g);
        check(0x05A1);
        sec_cascade(M, g);
        check(0x05BB);
        sec_halfband(M, g);
        g_chk_fn = [&](uint32_t pc) { check(pc); };
        g_chk = &g_chk_impl;
        chain_tail(M, rom, g);
        g_chk = nullptr;
        // мастер-выход Y:$00-$1F против expected.txt
        if (fr < exp.size() && !exp[fr].empty()) {
            int bad = 0;
            for (int i = 0; i < 32; ++i) {
                uint32_t mv = M.Y[i] & M24;
                int32_t ev = exp[fr][i];
                if ((int32_t)mv != ev) {
                    if (bad < 4) fprintf(stderr, "  OUT f%u y:%02X oracle=%d mine=%d\n", fr, i, ev, (int32_t)mv);
                    ++bad;
                }
            }
            if (bad) { fprintf(stderr, "FRAME %u OUTPUT: %d mismatches\n", fr, bad); g_mismatches += bad; }
        }
    }
    fprintf(stderr, "dataset %s: boundary checks=%d, mismatches=%d\n",
            dir.c_str(), total_checks, g_mismatches);
    return g_mismatches ? 1 : 0;
}
