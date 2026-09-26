// ============================================================================
// test_mnm_chain.cpp — bit-exact verification of MnmDspCore against the
// emulator-oracle vectors (exp61/exp62): 3 datasets x 32 frames, whole track
// chain $02EC-$0B4C + preamble $00CF-$0100 + machine dispatch $0100-$02EB.
//
// Build:  g++ -std=c++17 -O2 -I. test_mnm_chain.cpp -o test_mnm_chain
// Run:    ./test_mnm_chain <oracle_dir>
// ============================================================================
#include "MnmDspCore.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace mnmdsp;

static std::vector<std::string> read_lines(const std::string& p) {
    std::vector<std::string> out; std::ifstream f(p);
    std::string s;
    while (std::getline(f, s)) { while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back(); out.push_back(s); }
    return out;
}

struct Poke { char sp; uint32_t addr; uint32_t val; };
struct Snap { uint32_t frame; std::vector<std::pair<char, std::pair<uint32_t,uint32_t>>> cells; };

int main(int argc, char** argv) {
    std::string dir = argc > 1 ? argv[1] : ".";
    // program
    Core core;
    core.parse(read_lines(dir + "/chain_program.lst"));
    fprintf(stderr, "program: %zu instructions\n", core.prog.size());
    // tables
    {
        {
            std::ifstream f(dir + "/tables_x.bin", std::ios::binary);
            char buf[4]; uint32_t a = 0x100000;
            while (f.read(buf, 4)) {
                uint32_t v = (uint8_t) buf[0] | ((uint8_t) buf[1] << 8) | ((uint8_t) buf[2] << 16);
                core.X[a] = v; ++a;
            }
        }
        {
            std::ifstream f(dir + "/tables_y.bin", std::ios::binary);
            char buf[4]; uint32_t a = 0x100000;
            while (f.read(buf, 4)) {
                uint32_t v = (uint8_t) buf[0] | ((uint8_t) buf[1] << 8) | ((uint8_t) buf[2] << 16);
                core.Y[a] = v; ++a;
            }
        }
        fprintf(stderr, "tables loaded\n");
    }

    const char* sets[3] = {"A_neutral", "B_hot", "C_m14_handler"};
    int total_checked = 0, total_fail = 0;

    for (int si = 0; si < 3; ++si) {
        std::string name = sets[si];
        std::string d = dir + "/" + name;
        // ---- init ----
        Core* c = &core;
        // reset state
        c->A = c->B = 0; c->x0 = c->x1 = c->y0 = c->y1 = 0;
        for (int i = 0; i < 8; ++i) { c->R[i] = 0; c->N[i] = 0; c->M[i] = MASK24; }
        c->f = Flags{}; c->sr_int = 0; c->pc = 0;
        c->do_stack.clear(); c->rep_valid = false; c->ret_stack.clear();
        c->steps = 0;
        // NOTE: tables already loaded; init overwrites low cells only
        std::vector<Poke> frame_pokes[32];
        {
            std::ifstream f(d + "/init.txt");
            std::string kind;
            int64_t A = 0, B = 0; uint32_t sr = 0, x0 = 0, x1 = 0, y0 = 0, y1 = 0;
            while (f >> kind) {
                if (kind == "REGS") {
                    long long pc_, a, b; unsigned sr_, x0_, x1_, y0_, y1_;
                    f >> std::hex >> pc_ >> a >> b >> sr_ >> x0_ >> x1_ >> y0_ >> y1_;
                    c->A = a; c->B = b; c->sr_int = sr_;
                    c->x0 = x0_; c->x1 = x1_; c->y0 = y0_; c->y1 = y1_;
                } else if (kind == "RN") { for (int i = 0; i < 8; ++i) f >> std::hex >> c->R[i];
                } else if (kind == "NN") { for (int i = 0; i < 8; ++i) { unsigned v; f >> std::hex >> v; c->N[i] = (int32_t) Core::sext(v & MASK24, 24); }
                } else if (kind == "MN") { for (int i = 0; i < 8; ++i) f >> std::hex >> c->M[i];
                } else if (kind == "DO") { long long a1, a2, a3; f >> std::hex >> a1 >> a2 >> a3;
                    c->do_stack.push_back(a1); c->do_stack.push_back(a2); c->do_stack.push_back(a3);
                } else if (kind == "X" || kind == "Y") {
                    unsigned a, v; f >> std::hex >> a >> v;
                    (kind == "X" ? c->X : c->Y)[a] = v;
                }
            }
        }
        // ---- frames ----
        std::vector<std::vector<Poke>> pokes(32);
        {
            std::ifstream f(d + "/frames.txt");
            std::string kind; int cur = -1;
            while (f >> kind) {
                if (kind == "FRAME") { f >> std::dec >> cur; int n; f >> std::dec >> n; pokes[cur].reserve(n); }
                else { Poke p; p.sp = kind[0]; f >> std::hex >> p.addr >> p.val; pokes[cur].push_back(p); }
            }
        }
        // ---- expected ----
        std::vector<std::vector<int32_t>> outs;
        std::vector<Snap> snaps;
        {
            std::ifstream f(d + "/expected.txt");
            std::string kind;
            while (f >> kind) {
                if (kind == "OUT") {
                    int fr; f >> fr; std::vector<int32_t> o(32);
                    for (auto& v : o) { unsigned w; f >> std::dec >> v; }
                    // values are signed decimal
                    outs.push_back(o);
                } else if (kind == "SNAP") {
                    Snap s; f >> s.frame;
                    snaps.push_back(s);
                } else {
                    unsigned a, v; f >> std::hex >> a >> v;
                    snaps.back().cells.push_back({kind[0], {a, v}});
                }
            }
        }

        int frame_fail = 0;
        for (int fr = 0; fr < 32; ++fr) {
            // apply the TRIG poke (Y:P+$28) before the preamble
            for (auto& p : pokes[fr])
                if (p.addr == 0x428 && p.sp == 'Y') c->wr(p.sp, p.addr, p.val);
            c->run(0x00CF, 0x0100, (void (*)(uint32_t, Core&)) nullptr, 800000);
            c->run(0x0100, 0x02EB, (void (*)(uint32_t, Core&)) nullptr, 800000);
            for (auto& p : pokes[fr])
                if (!(p.addr == 0x428 && p.sp == 'Y')) c->wr(p.sp, p.addr, p.val);
            c->run(0x02EC, 0x0B4C, (void (*)(uint32_t, Core&)) nullptr, 800000);
            // compare outputs
            for (int i = 0; i < 32; ++i) {
                int32_t got = (int32_t) Core::sgn24(c->rd('Y', i));
                int32_t want = outs[fr][i];
                ++total_checked;
                if (got != want) {
                    ++total_fail; ++frame_fail;
                    if (frame_fail <= 4) {
                        fprintf(stderr, "%s f%02d out[%d] want=%d got=%d\n",
                                name.c_str(), fr, i, want, got);
                        fprintf(stderr, "  last PCs:");
                        for (auto p : c->trace_ring) fprintf(stderr, " %04X", p);
                        fprintf(stderr, "\n");
                        c->trace_on = true;
                    }
                }
            }
            // snapshots
            for (auto& s : snaps) {
                if (s.frame != (uint32_t) fr) continue;
                for (auto& cell : s.cells) {
                    uint32_t got = c->rd(cell.first, cell.second.first);
                    uint32_t want = cell.second.second;
                    ++total_checked;
                    if (got != want) {
                        ++total_fail; ++frame_fail;
                        if (frame_fail <= 40)
                            fprintf(stderr, "%s f%02d SNAP %c:%04X want=%06X got=%06X\n",
                                    name.c_str(), fr, cell.first, cell.second.first, want, got);
                    }
                }
            }
            if (frame_fail) {
                // first diverging cell for debugging
                for (auto& s2 : snaps) {
                    if (s2.frame != (uint32_t) fr) continue;
                    for (auto& cell : s2.cells) {
                        uint32_t got = c->rd(cell.first, cell.second.first);
                        if (got != cell.second.second) {
                            fprintf(stderr, "FIRST-DIFF %s f%02d %c:%04X want=%06X got=%06X\n",
                                    name.c_str(), fr, cell.first, cell.second.first,
                                    cell.second.second, got);
                            goto done_frames;
                        }
                    }
                }
                goto done_frames;
            }
        }
        done_frames:;
        fprintf(stderr, "%-14s %s (%d mismatches)\n", name.c_str(),
                frame_fail ? "FAIL" : "BIT-EXACT", frame_fail);
    }
    fprintf(stderr, "checked %d values, %d mismatches\n", total_checked, total_fail);
    return total_fail ? 1 : 0;
}
