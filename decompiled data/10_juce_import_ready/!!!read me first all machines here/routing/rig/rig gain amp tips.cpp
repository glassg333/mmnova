// Monomachine OS 1.32B — DSP1 kernel post-voice (func_0004A8) measurement rig.
// Runs the REAL firmware voice chain in the dsp56k emulator and measures:
//  - which memory the chain reads/writes (I/O mapping)
//  - the DIST (AMP page drive) transfer curve
//  - per-param sensitivity scan of the whole V-$28..V-$01 window
//
// Requires: dsp1_pmem.bin / dsp1_xmem.bin / dsp1_ymem.bin (OS 1.32B DSP1)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "dsp56kEmu/memory.h"
#include "dsp56kEmu/dsp.h"
#include "dsp56kEmu/dspregs.h"
#include "dsp56kEmu/peripherals.h"
#include "dsp56kEmu/types.h"

using namespace dsp56k;
using namespace std;

static const char* g_imgDir =
    "/home/z/my-project/scripts/staging/mmnova_firmware_data/02_memory_images";

static vector<uint32_t> loadWords24(const string& path)
{
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) { fprintf(stderr, "cannot open %s\n", path.c_str()); exit(1); }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    vector<uint32_t> v(sz / 3); vector<uint8_t> buf(sz);
    if (fread(buf.data(), 1, sz, f) != (size_t)sz) { fprintf(stderr, "read fail\n"); exit(1); }
    fclose(f);
    for (size_t i = 0; i < v.size(); ++i)
        v[i] = (uint32_t(buf[i*3]) << 16) | (uint32_t(buf[i*3+1]) << 8) | buf[i*3+2];
    return v;
}

static inline double q23(uint32_t v)
{
    int32_t s = (v & 0x800000) ? int32_t(v | 0xFF000000u) : int32_t(v);
    return double(s) / 8388608.0;
}
static inline uint32_t q23(double d)
{
    d = d < -1.0 ? -1.0 : d > 0.99999988 ? 0.99999988 : d;
    return uint32_t(int32_t(lround(d * 8388608.0))) & 0xFFFFFF;
}

struct Rig
{
    DefaultMemoryValidator validator;
    Memory mem;
    PeripheralsNop pX, pY;
    DSP dsp;

    static constexpr TWord V = 0x528;   // voice0 state base (Y)

    explicit Rig()
        : mem(validator, 0x160000, 0x160000, 0x100000)
        , dsp(mem, &pX, &pY)
    {
        auto P = loadWords24(string(g_imgDir) + "/dsp1_pmem.bin");
        auto X = loadWords24(string(g_imgDir) + "/dsp1_xmem.bin");
        auto Y = loadWords24(string(g_imgDir) + "/dsp1_ymem.bin");
        for (size_t i = 0; i < P.size(); ++i) mem.set(MemArea_P, TWord(i), P[i]);
        for (size_t i = 0; i < X.size(); ++i) mem.set(MemArea_X, TWord(i), X[i]);
        for (size_t i = 0; i < Y.size(); ++i) mem.set(MemArea_Y, TWord(i), Y[i]);

        // runtime sine ROM X:$14A000..$14BFFF, 8192 pts Q1.23
        for (int i = 0; i < 8192; ++i)
            mem.set(MemArea_X, 0x14A000 + TWord(i), q23(sin(2.0 * M_PI * i / 8192.0)));

        // jsr func_0004A8 ; jmp self
        mem.set(MemArea_P, 0x20000, 0x0BF080);
        mem.set(MemArea_P, 0x20001, 0x0004A8);
        mem.set(MemArea_P, 0x20002, 0x0AF080);
        mem.set(MemArea_P, 0x20003, 0x020002);

        for (TWord a = 0x400; a < 0x800; ++a) mem.set(MemArea_Y, a, 0);
    }

    void setParam(TWord voff, uint32_t val) { mem.set(MemArea_Y, V - voff, val & 0xFFFFFF); }
    uint32_t getParam(TWord voff) const { return mem.get(MemArea_Y, V - voff); }

    void setVoiceY(TWord off, uint32_t val) { mem.set(MemArea_Y, V + off, val & 0xFFFFFF); }

    // 16-sample test signal into every candidate buffer
    void fillBuffers(const double* sig)
    {
        for (TWord base = 0x140; base <= 0x280; base += 0x20)
            for (int i = 0; i < 32; ++i)
                mem.set(MemArea_X, base + TWord(i), q23(sig[i % 16]));
    }
    void fillBuffersSin(double freq, double amp, double ph = 0.0)
    {
        double s[16];
        for (int i = 0; i < 16; ++i) s[i] = amp * sin(ph + 2.0 * M_PI * freq * i / 16.0);
        fillBuffers(s);
    }
    void fillBuffersRamp(double amp)
    {
        double s[16];
        for (int i = 0; i < 16; ++i) s[i] = -amp + 2.0 * amp * i / 15.0;
        fillBuffers(s);
    }

    bool runBlock()
    {
        auto& r = dsp.regs();
        r.r[0] = TReg24(mem.get(MemArea_X, 0x2C0));
        r.r[5] = TReg24(mem.get(MemArea_X, 0x2C3));
        r.r[6] = TReg24(V - 0x28);
        r.r[7] = TReg24(0x100);
        for (int i = 0; i < 8; ++i) { r.n[i] = TReg24(0); r.m[i] = TReg24(0xFFFFFF); }
        r.sp = TReg24(0);

        dsp.setPC(TWord(0x20000));
        for (uint64_t step = 0; step < 400000; ++step)
        {
            TWord pc = dsp.getPC().toWord();
            if (pc == 0x163 || pc == 0x144 || pc == 0x20002)
                return true;
            dsp.exec();
        }
        return false;
    }

    // I/O detection: diff X/Y words $0..$400 before/after
    map<TWord, pair<uint32_t,uint32_t>> diffXY()
    {
        map<TWord, pair<uint32_t,uint32_t>> out;
        for (TWord a = 0; a < 0x400; ++a)
        {
            uint32_t bx = mem.get(MemArea_X, a), by = mem.get(MemArea_Y, a);
            return_0: {};
            (void)bx; (void)by;
        }
        return out;
    }
    // (real diff helper)
    void snapshot(vector<uint32_t>& x, vector<uint32_t>& y) const
    {
        x.resize(0x400); y.resize(0x400);
        for (TWord a = 0; a < 0x400; ++a) { x[a] = mem.get(MemArea_X, a); y[a] = mem.get(MemArea_Y, a); }
    }
    void compare(const vector<uint32_t>& x0, const vector<uint32_t>& y0) const
    {
        printf("--- changed X ---\n");
        for (TWord a = 0; a < 0x400; ++a)
            if (mem.get(MemArea_X, a) != x0[a])
                printf("X:%03X %06X -> %06X (%.5f -> %.5f)\n", a, x0[a], mem.get(MemArea_X, a), q23(x0[a]), q23(mem.get(MemArea_X, a)));
        printf("--- changed Y ---\n");
        for (TWord a = 0; a < 0x400; ++a)
            if (mem.get(MemArea_Y, a) != y0[a])
                printf("Y:%03X %06X -> %06X (%.5f -> %.5f)\n", a, y0[a], mem.get(MemArea_Y, a), q23(y0[a]), q23(mem.get(MemArea_Y, a)));
    }

    void printBlockX(TWord base) const
    {
        printf("X:%03X:", base);
        for (int i = 0; i < 16; ++i) printf(" %7.4f", q23(mem.get(MemArea_X, base + TWord(i))));
        printf("\n");
    }
    double rmsX(TWord base) const
    {
        double s = 0;
        for (int i = 0; i < 16; ++i) { double v = q23(mem.get(MemArea_X, base + TWord(i))); s += v * v; }
        return sqrt(s / 16.0);
    }
};

int main(int argc, char** argv)
{
    string mode = argc > 1 ? argv[1] : "map";

    if (mode == "map")
    {
        // bases
        for (TWord a = 0x2C0; a <= 0x2C4; ++a)
            printf("X:%03X = %06X\n", a, Rig().mem.get(MemArea_X, a));

        Rig s;
        vector<uint32_t> x0, y0;
        s.snapshot(x0, y0);
        s.fillBuffersSin(3.0 / 16.0, 0.5);
        bool ok = s.runBlock();
        printf("run ok=%d\n", ok);
        s.compare(x0, y0);
        return 0;
    }

    if (mode == "probe")
    {
        // signal on X/Y $0-$1F, accumulation -> X:$280; sweep each param to $7FFFFF
        auto runOne = [](Rig& s) -> double {
            if (!s.runBlock()) return -999;
            double e = 0;
            for (int i = 0; i < 32; ++i) { double v = q23(s.mem.get(MemArea_X, 0x280 + TWord(i))); e += v*v; }
            return sqrt(e/32.0);
        };
        double base;
        {
            Rig s;
            s.mem.set(MemArea_X, 0x2C9, 0x300);
            for (int i = 0; i < 16; ++i)
            {
                double v = 0.5*sin(2.0*M_PI*3.0*i/16.0);
                s.mem.set(MemArea_X, TWord(i), q23(v));    s.mem.set(MemArea_X, TWord(16+i), q23(v));
                s.mem.set(MemArea_Y, TWord(i), q23(v));    s.mem.set(MemArea_Y, TWord(16+i), q23(v));
            }
            base = runOne(s);
        }
        printf("base rms=%.6f\n", base);
        for (int vo = 0x28; vo >= 0x01; --vo)
        {
            Rig s;
            s.mem.set(MemArea_X, 0x2C9, 0x300);
            for (int i = 0; i < 16; ++i)
            {
                double v = 0.5*sin(2.0*M_PI*3.0*i/16.0);
                s.mem.set(MemArea_X, TWord(i), q23(v));    s.mem.set(MemArea_X, TWord(16+i), q23(v));
                s.mem.set(MemArea_Y, TWord(i), q23(v));    s.mem.set(MemArea_Y, TWord(16+i), q23(v));
            }
            s.setParam(TWord(vo), 0x7FFFFF);
            double r = runOne(s);
            printf("V-$%02X=$7FFFFF rms=%.6f delta=%+.6f\n", vo, r, r-base);
        }
        return 0;
    }

    if (mode == "sig")
    {
        // direct signal test: audio at X:$0-$1F (ch1) / Y:$0-$1F (ch2),
        // accumulation pointer X:$2C9 -> $300 (zeroed scratch)
        int dist = argc > 2 ? atoi(argv[2]) : -1;
        double freq = argc > 3 ? atof(argv[3]) : 3.0;

        Rig s;
        s.mem.set(MemArea_X, 0x2C9, 0x300);
        double sig[16];
        for (int i = 0; i < 16; ++i) sig[i] = 0.5 * sin(2.0 * M_PI * freq * i / 16.0);
        for (int i = 0; i < 16; ++i)
        {
            s.mem.set(MemArea_X, TWord(i),     q23(sig[i]));
            s.mem.set(MemArea_X, TWord(16+i),  q23(sig[i]));
            s.mem.set(MemArea_Y, TWord(i),     q23(sig[i]));
            s.mem.set(MemArea_Y, TWord(16+i),  q23(sig[i]));
        }
        if (dist >= 0) s.setParam(0x24, q23(double(dist) / 127.0 * 0.5));
        vector<uint32_t> x0, y0; s.snapshot(x0, y0);
        bool ok = s.runBlock();
        printf("sig ok=%d dist=%d freq=%.2f\n", ok, dist, freq);
        printf("OUT X:280-29F:");
        for (int i = 0; i < 32; ++i) printf(" %7.4f", q23(s.mem.get(MemArea_X, 0x280 + TWord(i))));
        printf("\nGAIN Y:20-3F:");
        for (int i = 0; i < 32; ++i) printf(" %7.4f", q23(s.mem.get(MemArea_Y, 0x20 + TWord(i))));
        printf("\nAUD X:0-1F after:");
        for (int i = 0; i < 32; ++i) printf(" %7.4f", q23(s.mem.get(MemArea_X, TWord(i))));
        printf("\n");
        s.compare(x0, y0);
        return 0;
    }

    if (mode == "distcurve")
    {
        // measure effective static transfer: for each DIST value, feed +DC blocks
        // read the processed audio X:$0-$1F AFTER the drive stage? output at X:280
        printf("dist  out[-0.9..0.9 DC sweep]\n");
        for (int d = 0; d <= 127; d += 16)
        {
            printf("d=%3d:", d);
            for (int k = 0; k < 9; ++k)
            {
                double in = -0.9 + 0.225 * k;
                Rig s;
                s.mem.set(MemArea_X, 0x2C9, 0x300);
                for (int i = 0; i < 16; ++i)
                {
                    s.mem.set(MemArea_X, TWord(i),    q23(in));
                    s.mem.set(MemArea_X, TWord(16+i), q23(in));
                    s.mem.set(MemArea_Y, TWord(i),    q23(in));
                    s.mem.set(MemArea_Y, TWord(16+i), q23(in));
                }
                s.setParam(0x24, q23(double(d) / 127.0 * 0.5));
                if (!s.runBlock()) { printf(" TIMEOUT"); break; }
                printf(" %8.5f", q23(s.mem.get(MemArea_X, 0x288)));
            }
            printf("\n");
        }
        return 0;
    }

    if (mode == "drive")
    {
        // inject DC into the drive-stage buffers X:$70-$8F / X:$B0-$CF (34 words),
        // sweep DIST param, read back the saturated/driven buffer
        printf("drive: in -> out (X:$70 ch1, X:$B0 ch2), DIST sweep\n");
        for (int d = 0; d <= 127; d += 16)
        {
            printf("d=%3d:", d);
            for (int k = 0; k < 9; ++k)
            {
                double in = -0.9 + 0.225 * k;
                Rig s;
                // buffers: X:$70-$91 = 34 words (16+16+2 states), same at $B0
                for (int i = 0; i < 16; ++i)
                {
                    s.mem.set(MemArea_X, 0x70 + TWord(i), q23(in));
                    s.mem.set(MemArea_X, 0x80 + TWord(i), q23(in));
                    s.mem.set(MemArea_X, 0x70 + TWord(16 + i), 0);
                    s.mem.set(MemArea_X, 0x80 + TWord(16 + i), 0);
                    s.mem.set(MemArea_X, 0xB0 + TWord(i), q23(in));
                    s.mem.set(MemArea_X, 0xC0 + TWord(i), q23(in));
                }
                s.setParam(0x24, q23(double(d) / 127.0 * 0.5));
                if (!s.runBlock()) { printf(" TIMEOUT"); break; }
                printf(" %8.5f", q23(s.mem.get(MemArea_X, 0x78)));
            }
            printf("\n");
        }
        return 0;
    }

    if (mode == "chain")
    {
        // steady-state filter gain: inject sine at X:$70/Y:$70 repeatedly (states persist),
        // measure post-chain output RMS at X:$0-$1F
        int nb = argc > 2 ? atoi(argv[2]) : 24;
        double amp = argc > 3 ? atof(argv[3]) : 0.25;
        printf("chain: %d blocks, amp=%.3f\n", nb, amp);
        for (int f = 1; f <= 15; f += 2)
        {
            Rig s;
            double g1 = 0, g2 = 0;
            for (int b = 0; b < nb; ++b)
            {
                for (int i = 0; i < 16; ++i)
                {
                    double v = amp * sin(2.0 * M_PI * f * i / 16.0);
                    s.mem.set(MemArea_X, 0x70 + TWord(i), q23(v));
                    s.mem.set(MemArea_X, 0x80 + TWord(i), q23(v));
                    s.mem.set(MemArea_Y, 0x70 + TWord(i), q23(v));
                    s.mem.set(MemArea_Y, 0x80 + TWord(i), q23(v));
                }
                if (!s.runBlock()) { printf(" TIMEOUT\n"); return 1; }
                if (b >= nb - 8)
                {
                    double e1 = 0, e2 = 0;
                    for (int i = 0; i < 16; ++i)
                    {
                        double v1 = q23(s.mem.get(MemArea_X, TWord(i)));
                        double v2 = q23(s.mem.get(MemArea_X, TWord(16 + i)));
                        e1 += v1*v1; e2 += v2*v2;
                    }
                    g1 = sqrt(e1/16.0); g2 = sqrt(e2/16.0);
                }
            }
            printf("f=%2d/16  outRMS ch1=%.6f ch2=%.6f  (in=%.6f)\n", f, g1, g2, amp/1.414214);
        }
        return 0;
    }

    if (mode == "env")
    {
        // real AMP env state machine: set ATK/DEC/SUS/REL in V-window,
        // note-on trigger (V-$08 = 1 on first block), print Y:$4FF per block
        int atk = argc > 2 ? atoi(argv[2]) : 16;
        int dec = argc > 3 ? atoi(argv[3]) : 16;
        int sus = argc > 4 ? atoi(argv[4]) : 90;   // knob 0..127
        int rel = argc > 5 ? atoi(argv[5]) : 16;
        int nb  = argc > 6 ? atoi(argv[6]) : 40;
        Rig s;
        s.setParam(0x10, uint32_t(atk) << 16);              // V-$10 ATK idx
        s.setParam(0x0F, uint32_t(dec) << 16);              // V-$0F DEC idx
        s.setParam(0x0E, q23(sqrt(double(sus)/127.0)));     // V-$0E sqrt(sustain)
        s.setParam(0x0D, uint32_t(rel) << 16);              // V-$0D REL idx
        s.setParam(0x07, 1);                                // V-$07 AMP trigger (state=1)
        s.fillBuffersSin(3.0/16.0, 0.5);
        for (int b = 0; b < nb; ++b)
        {
            if (b == 1) s.setParam(0x07, 0);                // clear trigger
            if (!s.runBlock()) { printf("TIMEOUT at b=%d\n", b); break; }
            uint32_t lv = s.mem.get(MemArea_Y, 0x4FF);
            printf("b=%3d state=%X level=%06X (%.4f)\n", b,
                s.mem.get(MemArea_X, 0x500) & 0xF, lv, q23(lv));
        }
        return 0;
    }

    printf("unknown mode\n");
    return 1;
}
