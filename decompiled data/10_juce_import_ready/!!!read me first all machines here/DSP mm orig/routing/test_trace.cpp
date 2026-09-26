// test_trace.cpp — dump per-step (pc, regs) + full write log + per-frame memory
// for one oracle dataset, format-identical to exp64_oracle_trace.py output.
// Build:  g++ -std=c++17 -O2 -I. test_trace.cpp -o test_trace
// Run:    ./test_trace <oracle_dir> <dataset> <trace_frame>
#include "MnmDspCore.hpp"
#include <fstream>
#include <iostream>

using namespace mnmdsp;

static std::vector<std::string> read_lines(const std::string& p) {
    std::vector<std::string> out; std::ifstream f(p);
    std::string s;
    while (std::getline(f, s)) { while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back(); out.push_back(s); }
    return out;
}

struct Poke { char sp; uint32_t addr; uint32_t val; };

static FILE* g_tr = nullptr;
static void trace_hook(uint32_t pcx, Core& k) {
    fprintf(g_tr, "%u %04x %014llx %014llx %06x %06x %06x %06x %06x %06x %06x %06x %06x %06x %06x %06x %04x\n",
            (unsigned) k.steps, pcx,
            (unsigned long long)((uint64_t) k.A & MASK56),
            (unsigned long long)((uint64_t) k.B & MASK56),
            k.x0, k.x1, k.y0, k.y1,
            k.R[0], k.R[1], k.R[2], k.R[3], k.R[4], k.R[5], k.R[6], k.R[7],
            k.sr_int & 0xFFFF);
}

int main(int argc, char** argv) {
    std::string dir = argc > 1 ? argv[1] : ".";
    std::string name = argc > 2 ? argv[2] : "A_neutral";
    int trace_frame = argc > 3 ? atoi(argv[3]) : 22;
    std::string d = dir + "/" + name;

    Core core;
    core.parse(read_lines(dir + "/chain_program.lst"));
    {
        std::ifstream f(dir + "/full_x.bin", std::ios::binary);
        char buf[4]; uint32_t a = 0;
        while (f.read(buf, 4)) {
            uint32_t v = (uint8_t) buf[0] | ((uint8_t) buf[1] << 8) | ((uint8_t) buf[2] << 16);
            core.X[a] = v; ++a;
        }
        std::ifstream g(dir + "/full_y.bin", std::ios::binary);
        a = 0;
        while (g.read(buf, 4)) {
            uint32_t v = (uint8_t) buf[0] | ((uint8_t) buf[1] << 8) | ((uint8_t) buf[2] << 16);
            core.Y[a] = v; ++a;
        }
    }

    Core* c = &core;
    c->A = c->B = 0; c->x0 = c->x1 = c->y0 = c->y1 = 0;
    for (int i = 0; i < 8; ++i) { c->R[i] = 0; c->N[i] = 0; c->M[i] = MASK24; }
    c->f = Flags{}; c->sr_int = 0; c->pc = 0;
    c->do_stack.clear(); c->rep_valid = false; c->ret_stack.clear();
    c->steps = 0;
    {
        std::ifstream f(d + "/init.txt");
        std::string kind;
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
    std::vector<std::vector<Poke>> pokes;
    {
        std::ifstream f(d + "/frames.txt");
        std::string kind; int cur = -1;
        while (f >> kind) {
            if (kind == "FRAME") { int n; f >> std::dec >> cur >> n; pokes.resize(cur + 1); pokes[cur].reserve(n); }
            else { Poke p; p.sp = kind[0]; f >> std::hex >> p.addr >> p.val; pokes[cur].push_back(p); }
        }
    }
    int nframes = (int) pokes.size();

    FILE* fm = fopen("/tmp/cpp_frames64.txt", "w");
    auto dump_mem = [&](const char* tag) {
        fprintf(fm, "== %s\n", tag);
        for (auto& kv : c->X) if (kv.second && kv.first < 0x100000)
            fprintf(fm, "x %03x %06x\n", kv.first, kv.second & MASK24);
        for (auto& kv : c->Y) if (kv.second && kv.first < 0x100000)
            fprintf(fm, "y %03x %06x\n", kv.first, kv.second & MASK24);
    };
    dump_mem("init");

    for (int fr = 0; fr < nframes; ++fr) {
        for (auto& p : pokes[fr])
            if (p.addr == 0x428 && (p.sp == 'Y' || p.sp == 'y')) c->wr('y', p.addr, p.val);
        c->run(0x00CF, 0x0100, (void (*)(uint32_t, Core&)) nullptr, 800000);
        c->run(0x0100, 0x02EB, (void (*)(uint32_t, Core&)) nullptr, 800000);
        for (auto& p : pokes[fr])
            if (!(p.addr == 0x428 && (p.sp == 'Y' || p.sp == 'y')))
                c->wr((char) tolower(p.sp), p.addr, p.val);
        if (fr == trace_frame) {
            c->steps = 0;
            c->log_all_writes = true;
            g_tr = fopen("/tmp/cpp_trace64.txt", "w");
            c->run(0x02EC, 0x0B4C, trace_hook, 800000);
            fclose(g_tr);
        } else {
            c->run(0x02EC, 0x0B4C, (void (*)(uint32_t, Core&)) nullptr, 800000);
        }
        fprintf(fm, "== frame %d out\n", fr);
        for (int i = 0; i < 32; ++i) fprintf(fm, "%06x ", c->rd('y', i) & MASK24);
        fprintf(fm, "\n");
        char tag[32]; snprintf(tag, sizeof(tag), "frame %d", fr);
        dump_mem(tag);
    }
    fclose(fm);

    FILE* wf = fopen("/tmp/cpp_writes64.txt", "w");
    for (auto& w : c->wlog)
        fprintf(wf, "%u %04x %s %03x %06x\n", w[0], w[1],
                (w[2] & 0x1000000u) ? "y" : "x", w[2] & 0xFFFFFF, w[3]);
    fclose(wf);
    fprintf(stderr, "frames=%d steps=%u writes=%zu\n", nframes, (unsigned) c->steps, c->wlog.size());
    return 0;
}
