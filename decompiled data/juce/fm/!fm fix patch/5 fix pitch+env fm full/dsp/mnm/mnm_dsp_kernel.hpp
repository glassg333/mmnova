// =============================================================================
// mnm_dsp_kernel.hpp — DSP56300 execution kernel (exact subset) for the
// Monomachine voice-frame port. OS 1.32B.
//
// This is a faithful C++ transcription of the semantics of the bit-exact
// python reference emulator (dsp_emu.py, 27 unit tests, proven against the
// firmware in packs 3/4/5/7). Only the instruction forms used by the track
// frame P:$0100-$0B4B are implemented; anything else aborts loudly.
//
// Memory model (matches the reference harness exactly):
//   * X/Y RAM: 0x800 words, zero-initialised;
//   * P-memory alias: the pmem dump words (X and Y) above $100000, served
//     from mnm_pmem_image.h (same bytes the harness loads);
//   * reads of unmapped addresses return 0 (same as the python dict model);
//   * the documented emulator divergence `move ab,l:` (B->X / A->Y; hardware
//     would be A->X / B->Y — see pack-7 PROOF note) is reproduced here so the
//     port matches the captured vectors word-for-word; 4 sites total.
// =============================================================================
#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <functional>

#include "mnm_pmem_image.h"
#include "MnmFmSineTable.h"   // mnmfm::kSine8192 (sin, $14A000)
#include "mnm_cos_table.h"    // mnm::kCos8192   (cos, $14A800)

namespace mnm {

static const uint32_t M24 = 0xFFFFFFu;
static const uint64_t M48 = 0xFFFFFFFFFFFFull;
static const uint64_t M56 = 0x00FFFFFFFFFFFFFFull;

inline int64_t sext(int64_t v, int bits) {
    int64_t m = int64_t(1) << (bits - 1);
    return (v & ((int64_t(1) << bits) - 1)) - ((v & m) << 1);
}

// ---------------------------------------------------------------------------
// pmem alias lookup: X/Y reads at $100000+ come from the dump image.
static inline bool pmem_word(uint32_t addr, uint32_t& out) {
    if (addr < 0x100000 || addr >= PMEM_IMG_BASE + PMEM_IMG_N) return false;
    out = pmem_img[addr - PMEM_IMG_BASE];
    return true;
}

// ---------------------------------------------------------------------------
struct DspFlags { uint8_t c = 0, v = 0, z = 0, n = 0, e = 0, u = 0; };

struct DSPKernel {
    // memories: low RAM + sparse overlay (dispatch mirrors at $2003xx etc.)
    uint32_t X[0x800] = {0};
    uint32_t Y[0x800] = {0};
    std::unordered_map<uint32_t, uint32_t> Xhi, Yhi;
    // registers
    int64_t A = 0, B = 0;               // 56-bit signed
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
    uint32_t R[8] = {0}, N[8] = {0}, M[8] = {M24, M24, M24, M24, M24, M24, M24, M24};
    DspFlags f;
    uint32_t srInt = 0;                    // raw SR bits (bset/bclr on sr)
    // flow
    std::unordered_map<uint32_t, uint32_t> nextAddr;   // pc -> next pc
    std::unordered_map<std::string, uint32_t> labels;
    struct DoEnt { int64_t cnt; uint32_t start, end; };
    std::vector<DoEnt> doStack;
    int64_t repCount = 0;
    uint32_t repAfter = 0xFFFFFFFFu;
    bool repArmed = false;
    std::vector<uint32_t> retStack;
    uint32_t pc = 0;
    bool halted = false;
    // program: parsed instruction stream
    struct Ins { std::string mnem; std::vector<std::string> toks; };
    std::unordered_map<uint32_t, Ins> prog;

    // native machine hooks (FM core): return the next pc
    std::function<uint32_t(DSPKernel&)> onProc;   // P:$145D21
    std::function<uint32_t(DSPKernel&)> onConf;   // P:$145D1D
    std::function<uint32_t(DSPKernel&)> onInit;   // P:$145D12
    uint32_t procAddr = 0x145D21, confAddr = 0x145D1D, initAddr = 0x145D12,
             nullAddr = 0x145D1C;

    // ---------------- parsing (identical token format to dsp_emu.py) --------
    void parseLine(uint32_t addr, const std::string& text) {
        // text = "000100: move     y:>$124,r1   ; 69F000 000124"
        static std::regex head("^([0-9A-Fa-f]{6}):\\s*(.*?)\\s*;\\s*[0-9A-Fa-f ]+$");
        std::smatch m;
        if (!std::regex_match(text, m, head)) return;
        std::string body = m[2].str();
        if (body.empty()) return;
        if (body.rfind("func_", 0) == 0 || body.rfind("int_", 0) == 0) return;
        static std::regex tokre("\\S+");
        auto begin = std::sregex_iterator(body.begin(), body.end(), tokre);
        auto end = std::sregex_iterator();
        std::vector<std::string> toks;
        std::string mnem;
        int i = 0;
        for (auto it = begin; it != end; ++it, ++i) {
            if (i == 0) mnem = it->str();
            else {
                std::string tk = it->str();
                toks.push_back(tk);
                // label harvesting (may be embedded: '#$0,x0,func_0005bb')
                size_t p = 0;
                while ((p = tk.find(',', p)) != std::string::npos) p++;
                size_t s = 0;
                while (s <= tk.size()) {
                    size_t c = tk.find(',', s);
                    std::string part = tk.substr(s, c == std::string::npos
                                                     ? std::string::npos : c - s);
                    while (!part.empty() && (part[0] == '>' || part[0] == '<'))
                        part.erase(0, 1);
                    if (part.rfind("func_", 0) == 0 || part.rfind("int_", 0) == 0)
                        labels[part] = uint32_t(strtoul(part.c_str() + 5, nullptr, 16));
                    if (c == std::string::npos) break;
                    s = c + 1;
                }
            }
        }
        prog[addr] = Ins{mnem, toks};
    }

    void buildNextAddr() {
        std::vector<uint32_t> order;
        order.reserve(prog.size());
        for (auto& kv : prog) order.push_back(kv.first);
        std::sort(order.begin(), order.end());
        for (size_t i = 0; i + 1 < order.size(); ++i)
            nextAddr[order[i]] = order[i + 1];
        if (!order.empty())
            nextAddr[order.back()] = order.back() + 1;
    }

    // ---------------- memory -------------------------------------------------
    uint32_t rd(const char* space, uint32_t ea) {
        if (ea < 0x800)
            return (space[0] == 'x' ? X[ea] : Y[ea]) & M24;
        // injected pan/quadrature tables ($14A000-$14BFFF): the harness
        // writes sin to $14A000.. then cos over $14A800.. (both X and Y)
        if (ea >= 0x14A000u && ea < 0x14A800u) return mnmfm::kSine8192[ea - 0x14A000u];
        if (ea >= 0x14A800u && ea < 0x14C000u) return kCos8192[ea - 0x14A800u];
        std::unordered_map<uint32_t, uint32_t>& hi =
            space[0] == 'x' ? Xhi : Yhi;
        auto it = hi.find(ea);
        if (it != hi.end()) return it->second;   // last write wins (dict model)
        uint32_t w;
        if (pmem_word(ea, w)) return w & M24;
        return 0;
    }
    void wr(const char* space, uint32_t ea, uint32_t val) {
        val &= M24;
        if (ea < 0x800) {
            (space[0] == 'x' ? X[ea] : Y[ea]) = val;
            return;
        }
        (space[0] == 'x' ? Xhi : Yhi)[ea] = val;
    }
    uint64_t rdL(uint32_t ea) {
        return (uint64_t(rd("x", ea)) << 24) | rd("y", ea);
    }
    void wrL(uint32_t ea, uint64_t v48) {
        wr("x", ea, uint32_t(v48 >> 24) & M24);
        wr("y", ea, uint32_t(v48) & M24);
    }

    // ---------------- registers ---------------------------------------------
    uint32_t getReg24(const std::string& n) {
        if (n == "a") return uint32_t((A >> 24) & M24);
        if (n == "b") return uint32_t((B >> 24) & M24);
        if (n == "a0") return uint32_t(A & M24);
        if (n == "b0") return uint32_t(B & M24);
        if (n == "a1") return uint32_t((A >> 24) & M24);
        if (n == "b1") return uint32_t((B >> 24) & M24);
        if (n == "a2") return uint32_t((A >> 48) & 0xFF);
        if (n == "b2") return uint32_t((B >> 48) & 0xFF);
        if (n == "x0") return x0;
        if (n == "x1") return x1;
        if (n == "y0") return y0;
        if (n == "y1") return y1;
        if (n[0] == 'r') return R[atoi(n.c_str() + 1)];
        if (n[0] == 'n') return N[atoi(n.c_str() + 1) & 7];
        if (n[0] == 'm') return M[atoi(n.c_str() + 1)];
        if (n == "x") return (x1 << 24) | x0;
        if (n == "y") return (y1 << 24) | y0;
        if (n == "sr" || n == "ccr") return srInt & 0xFFFF;
        fprintf(stderr, "mnm kernel: get_reg %s\n", n.c_str());
        abort();
    }
    int64_t getAcc(const std::string& n) {
        if (n == "a") return sext(A, 56);
        if (n == "b") return sext(B, 56);
        return getReg24(n);
    }
    void setReg(const std::string& n, int64_t val) {
        if (n == "a") { A = sext(val, 56); return; }
        if (n == "b") { B = sext(val, 56); return; }
        if (n == "a0") { A = (A & ~int64_t(M24)) | (val & M24); return; }
        if (n == "b0") { B = (B & ~int64_t(M24)) | (val & M24); return; }
        if (n == "a1") { A = (A & ~(int64_t(M24) << 24)) | ((val & M24) << 24); return; }
        if (n == "b1") { B = (B & ~(int64_t(M24) << 24)) | ((val & M24) << 24); return; }
        if (n == "a2") { A = (A & ~int64_t(0xFFll << 48)) | ((val & 0xFF) << 48); return; }
        if (n == "b2") { B = (B & ~int64_t(0xFFll << 48)) | ((val & 0xFF) << 48); return; }
        if (n == "x0") { x0 = uint32_t(val) & M24; return; }
        if (n == "x1") { x1 = uint32_t(val) & M24; return; }
        if (n == "y0") { y0 = uint32_t(val) & M24; return; }
        if (n == "y1") { y1 = uint32_t(val) & M24; return; }
        if (n[0] == 'r') { R[atoi(n.c_str() + 1)] = uint32_t(val) & M24; return; }
        if (n[0] == 'n') { N[atoi(n.c_str() + 1) & 7] = uint32_t(sext(val, 24)); return; }
        if (n[0] == 'm') { M[atoi(n.c_str() + 1)] = uint32_t(val) & M24; return; }
        if (n == "x") { x1 = uint32_t(val >> 24) & M24; x0 = uint32_t(val) & M24; return; }
        if (n == "y") { y1 = uint32_t(val >> 24) & M24; y0 = uint32_t(val) & M24; return; }
        fprintf(stderr, "mnm kernel: set_reg %s\n", n.c_str());
        abort();
    }
    // python get_reg() semantics: full-width value (48-bit for x/y, 56-bit A/B)
    int64_t getRegPy(const std::string& n) {
        if (n == "a") return sext(A, 56);
        if (n == "b") return sext(B, 56);
        if (n == "x") return (int64_t(x1) << 24) | x0;
        if (n == "y") return (int64_t(y1) << 24) | y0;
        return getReg24(n);
    }
    bool isAccSrc(const std::string& n) {
        return n == "a" || n == "b" || n == "a0" || n == "a1" || n == "a2" ||
               n == "b0" || n == "b1" || n == "b2";
    }

    // data limit checker (FM 5.4.1.2): a/b -> memory or x/y regs saturates
    uint32_t accPartForDst(const std::string& src, int64_t sval, bool limit = true) {
        if (src == "a" || src == "b") {
            int64_t v = sext(sval, 56);
            if (limit) {
                if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFF;
                if (v < -0x00800000000000ll) return 0x800000;
            }
            return uint32_t(v >> 24) & M24;
        }
        if (src == "a0" || src == "b0") return uint32_t(sval) & M24;
        if (src == "a1" || src == "b1") return uint32_t(sval >> 24) & M24;
        if (src == "a2" || src == "b2") return uint32_t(sval >> 48) & 0xFF;
        return uint32_t(sval) & M24;
    }
    static int64_t composeAcc(uint32_t v24) {
        return sext(v24 & M24, 24) << 24;
    }
    int64_t aluSrc(const std::string& n) {
        if (!n.empty() && n[0] == '#')
            return sext(absVal(n) & M24, 24) << 24;
        if (n == "x0" || n == "x1" || n == "y0" || n == "y1")
            return sext(getReg24(n), 24) << 24;
        return sext(getAcc(n), 56);
    }

    // ---------------- EA ------------------------------------------------------
    // (space, mode, rnum, arg, arg2) — python tuple flattened
    struct EA { char space; std::string mode; int rnum; std::string arg; std::string arg2; };
    static int32_t absVal(const std::string& v0) {
        std::string v = v0;
        if (!v.empty() && v[0] == '+') v.erase(0, 1);
        bool neg = !v.empty() && v[0] == '-';
        if (neg) v.erase(0, 1);
        size_t s = v.find_first_not_of("$#<>");
        v = (s == std::string::npos) ? "" : v.substr(s);
        int64_t val = v.empty() ? 0 : strtoll(v.c_str(), nullptr, 16);
        return int32_t(neg ? -val : val);
    }
    std::unordered_map<std::string, EA> eaMemo;
    bool parseEA(const std::string& tok0, EA& out) {
        auto memo = eaMemo.find(tok0);
        if (memo != eaMemo.end()) { out = memo->second; return true; }
        std::string tok = tok0;
        tok.erase(std::remove(tok.begin(), tok.end(), '?'), tok.end());
        static std::regex m0("^([xyl]):(.+)$");
        std::smatch mm;
        if (!std::regex_match(tok, mm, m0)) return false;
        char space = mm[1].str()[0];
        std::string inner = mm[2].str();
        // strip leading '>' (absolute marker)
        static std::regex pre("^([+-])\\((r\\d)\\)$");
        if (std::regex_match(inner, mm, pre)) {
            out = EA{space, mm[1].str() == "+" ? "pre+" : "pre-",
                     atoi(mm[2].str().c_str() + 1),
                     mm[1].str() == "+" ? "1" : "-1", ""};
            return true;
        }
        static std::regex m1("^\\((r\\d)\\)(([+-].*)?)$");
        if (std::regex_match(inner, mm, m1)) {
            int rnum = atoi(mm[1].str().c_str() + 1);
            std::string after = mm[2].str();
            if (after.empty()) { out = EA{space, "reg", rnum, "", ""}; return true; }
            if (after == "+") { out = EA{space, "post+", rnum, "1", ""}; return true; }
            if (after == "-") { out = EA{space, "post-", rnum, "-1", ""}; return true; }
            static std::regex m2("^\\+n(\\d)$"), m3("^-n(\\d)$");
            if (std::regex_match(after, mm, m2)) {
                out = EA{space, "post+", rnum, "", "n" + mm[1].str()}; return true;
            }
            if (std::regex_match(after, mm, m3)) {
                out = EA{space, "post-", rnum, "", "n" + mm[1].str()}; return true;
            }
        }
        static std::regex m4("^\\((r\\d)([+-].+)\\)$");
        if (std::regex_match(inner, mm, m4)) {
            out = EA{space, "disp", atoi(mm[1].str().c_str() + 1), mm[2].str(), ""};
            return true;
        }
        size_t s = inner.find_first_not_of(":?$<>");
        std::string v = (s == std::string::npos) ? "" : inner.substr(s);
        out = EA{space, "abs", -1, v, ""};
        eaMemo[tok0] = out;
        return true;
    }
    // returns (kind, addr): kind 0=abs,1=r,2=upd
    void evalEA(const EA& ea, int& kind, uint32_t& addr, int& rnum, int64_t& delta) {
        if (ea.mode == "abs") { kind = 0; addr = uint32_t(absVal(ea.arg)); return; }
        uint32_t r = ea.rnum >= 0 ? R[ea.rnum] : 0;
        if (ea.mode == "reg") { kind = 1; addr = r; rnum = ea.rnum; return; }
        if (ea.mode == "pre+" || ea.mode == "pre-") {
            updR(ea.rnum, atoi(ea.arg.c_str()));
            kind = 1; addr = R[ea.rnum]; rnum = ea.rnum; return;
        }
        if (ea.mode == "post+" || ea.mode == "post-") {
            delta = ea.arg.empty() ? int64_t(N[atoi(ea.arg2.c_str() + 1)])
                                   : int64_t(atoi(ea.arg.c_str()));
            kind = 2; addr = r; rnum = ea.rnum; return;
        }
        if (ea.mode == "disp") {
            int64_t v;
            static std::regex nd("[+-]n\\d");
            if (std::regex_match(ea.arg, nd)) {
                v = N[atoi(ea.arg.c_str() + 2)];
                if (ea.arg[0] == '-') v = -v;
            } else v = absVal(ea.arg);
            kind = 1; addr = uint32_t(r + v) & M24; rnum = ea.rnum; return;
        }
        fprintf(stderr, "mnm kernel: eval_ea %s\n", ea.mode.c_str());
        abort();
    }
    void updR(int rnum, int64_t delta) {
        uint32_t m = M[rnum], r = R[rnum];
        if (m == M24) { R[rnum] = uint32_t(r + delta) & M24; return; }
        uint32_t base = r & ~m;
        int64_t off = int64_t(r - base) + delta;
        if (off > int64_t(m)) off -= m + 1;
        else if (off < 0) off += m + 1;
        R[rnum] = uint32_t(base + off) & M24;
    }
    void commitEAUpdates(const std::vector<EA>& eas) {
        for (const EA& ea : eas) {
            if (ea.mode == "post+" || ea.mode == "post-") {
                int64_t d = ea.arg.empty() ? int64_t(N[atoi(ea.arg2.c_str() + 1)])
                                           : int64_t(atoi(ea.arg.c_str()));
                updR(ea.rnum, d);
            }
        }
    }

    // ---------------- ALU ------------------------------------------------------
    int32_t sgn24(uint32_t v) { return v & 0x800000 ? int32_t(v - (1 << 24)) : int32_t(v); }
    uint32_t sgn24raw(int32_t v) { return uint32_t(v) & M24; }
    int64_t flagsFrom(int64_t res) {
        int64_t r56 = sext(res, 56);
        f.n = r56 < 0 ? 1 : 0;
        f.z = r56 == 0 ? 1 : 0;
        int a1msb = int((r56 >> 47) & 1);
        int a2 = int((r56 >> 48) & 0xFF);
        int signExt = a1msb ? 0xFF : 0x00;
        f.e = a2 != signExt ? 1 : 0;
        return r56;
    }
    int64_t aluAdd(const std::string& dst, int64_t x, int64_t y, int carry = 0,
                   bool store = true) {
        int64_t a = x + y + carry;
        bool sx = sext(x, 56) < 0, sy = sext(y, 56) < 0, sr = sext(a, 56) < 0;
        f.v = (sx == sy && sr != sx) ? 1 : 0;
        f.c = (a >> 55) != 0 ? 1 : 0;
        a = flagsFrom(a);
        if (store) setReg(dst, a);
        return a;
    }
    int64_t aluSub(const std::string& dst, int64_t x, int64_t y, bool store = true) {
        int64_t a = x - y;
        bool sx = sext(x, 56) < 0, sy = sext(y, 56) < 0, sr = sext(a, 56) < 0;
        f.v = (sx != sy && sr != sx) ? 1 : 0;
        f.c = a < 0 ? 0 : 1;
        a = flagsFrom(a);
        if (store) setReg(dst, a);
        return a;
    }
    void rndAcc(const std::string& name) {
        int64_t v = getAcc(name);
        v = sext(v + 0x800000, 56);
        v = (v >> 24) << 24;
        setReg(name, v);
        flagsFrom(v);
    }
    // DIV: one non-restoring step (exact python port)
    void divStep(const std::string& sTok, const std::string& dTok) {
        int32_t s = sgn24(getReg24(sTok));
        if ((uint32_t(s) & M24) == 0) { setReg(dTok, 0xFFFFFFFFFFFFll & M48); return; }
        uint32_t d1 = uint32_t((getAcc(dTok) >> 24) & M24);
        uint32_t d0 = uint32_t(getAcc(dTok) & M24);
        d1 = ((d1 << 1) | (d0 >> 23)) & M24;
        d0 = (d0 << 1) & M24;
        int32_t sSigned = sext(uint32_t(s) & M24, 24);
        int32_t d1Signed = sext(d1, 24);
        if ((d1Signed < 0) == (sSigned < 0)) d1 = uint32_t(d1 - (uint32_t(s) & M24)) & M24;
        else d1 = uint32_t(d1 + (uint32_t(s) & M24)) & M24;
        int qbit = (sext(d1, 24) < 0) == (sSigned < 0) ? 1 : 0;
        d0 |= uint32_t(qbit);
        setReg(dTok, (int64_t(d1) << 24) | d0);
        f.c = uint8_t(qbit ^ 1);
    }

    // ---------------- moves ----------------------------------------------------
    struct Latch { int64_t sval; int kind; std::string src, dst; }; // kind: imm/reg/reg48/mem/meml
    uint32_t moveImm(const std::string& src, const std::string& dst) {
        int32_t v = absVal(src);
        static std::regex imm8("^#\\$([0-9a-fA-F]{1,2})$");
        std::smatch m;
        bool isR = dst.size() == 2 && (dst[0] == 'r' || dst[0] == 'n' || dst[0] == 'm');
        if (std::regex_match(src, m, imm8) && dst != "a" && dst != "b" && !isR) {
            int32_t imm = int32_t(strtoul(m[1].str().c_str(), nullptr, 16));
            if (imm >= 0x80) imm -= 0x100;
            v = (imm << 16) & int32_t(M24);
        }
        return uint32_t(v);
    }
    void latchRead(const std::string& tk, std::vector<Latch>& latch,
                   std::vector<EA>& eas) {
        // bare pointer update: (rN)[+/-[nN]]
        static std::regex bare("^\\((r\\d)\\)([+-](n\\d)?)?$");
        std::smatch m;
        if (tk.find(',') == std::string::npos && std::regex_match(tk, m, bare)) {
            int rnum = atoi(m[1].str().c_str() + 1);
            std::string upd = m[2].str();
            if (upd.empty()) return;
            if (upd == "+") eas.push_back(EA{'x', "post+", rnum, "1", ""});
            else if (upd == "-") eas.push_back(EA{'x', "post-", rnum, "-1", ""});
            else if (upd.rfind("+n", 0) == 0) eas.push_back(EA{'x', "post+", rnum, "", upd.substr(1)});
            else if (upd.rfind("-n", 0) == 0) eas.push_back(EA{'x', "post-", rnum, "", upd.substr(1)});
            return;
        }
        size_t c = tk.find(',');
        if (c == std::string::npos || tk.find(',', c + 1) != std::string::npos) {
            fprintf(stderr, "mnm kernel: bad move '%s'\n", tk.c_str());
            abort();
        }
        std::string src = tk.substr(0, c), dst = tk.substr(c + 1);
        Latch L{0, 0, src, dst};
        if (!src.empty() && src[0] == '#') {
            L.sval = moveImm(src, dst); L.kind = 0;
        } else if (src == "ba" || src == "ab") {
            // NOTE: emulator semantics: 48-bit value = B1:A1 (B in high half)
            L.sval = (int64_t(uint64_t(getReg24("b")) << 24 |
                              uint64_t(getReg24("a"))));
            L.kind = 2;
        } else if (std::regex_match(src, std::regex("^(x0|x1|y0|y1|x|y|a|b|r\\d|n\\d|m\\d)$")) ||
                   std::regex_match(src, std::regex("^[ab]\\d$"))) {
            if (std::regex_match(src, std::regex("^[ab]\\d$")))
                L.sval = getAcc(src.substr(0, 1));   // RAW 56-bit accumulator
            else
                L.sval = getRegPy(src);
            L.kind = 1;
        } else {
            EA ea;
            if (!parseEA(src, ea)) { fprintf(stderr, "bad ea %s\n", src.c_str()); abort(); }
            int kind; uint32_t addr; int rnum; int64_t delta;
            evalEA(ea, kind, addr, rnum, delta);
            eas.push_back(ea);
            L.sval = ea.space == 'l' ? int64_t(rdL(addr)) : int64_t(rd(ea.space == 'x' ? "x" : "y", addr));
            L.kind = ea.space == 'l' ? 4 : 3;
        }
        latch.push_back(L);
    }
    void commitWrites(std::vector<Latch>& latch, std::vector<EA>& eas) {
        for (Latch& L : latch) {
            const std::string& dst = L.dst;
            int64_t sval = L.sval;
            if ((dst == "ba" || dst == "ab") && L.kind == 4) {
                uint32_t xw = uint32_t(sval >> 24) & M24, yw = uint32_t(sval) & M24;
                const std::string& first = (dst == "ba") ? "b" : "a";
                const std::string& second = (dst == "ba") ? "a" : "b";
                setReg(first, sext(xw, 24) << 24);
                setReg(second, sext(yw, 24) << 24);
                continue;
            }
            bool isAcc = dst == "a" || dst == "b";
            bool isPart = (dst.size() == 2 && dst[0] == 'a' && isdigit(dst[1])) ||
                          (dst.size() == 2 && dst[0] == 'b' && isdigit(dst[1]));
            bool isXY = dst == "x0" || dst == "x1" || dst == "y0" || dst == "y1" || dst == "x" || dst == "y";
            bool isRN = dst.size() == 2 && (dst[0] == 'r' || dst[0] == 'n') && isdigit(dst[1]);
            bool isM = dst.size() == 2 && dst[0] == 'm' && isdigit(dst[1]);
            if (isAcc || isPart || isXY || isRN || isM) {
                if ((dst == "a" || dst == "b") && L.kind == 4) {
                    sval = sext(sval & int64_t(M48), 48);
                } else if ((dst == "a" || dst == "b") && !isAccSrc(L.src)) {
                    sval = composeAcc(uint32_t(sval));
                } else if ((dst == "a" || dst == "b") && (L.src == "a0" || L.src == "b0")) {
                    int64_t acc = dst == "a" ? A : B;
                    sval = (acc & ~(int64_t(M24) << 24)) |
                           (int64_t(accPartForDst(L.src, sval) & M24) << 24);
                } else if ((dst == "a" || dst == "b") && (L.src == "a1" || L.src == "b1")) {
                    int64_t acc = dst == "a" ? A : B;
                    sval = (acc & ~int64_t(M48)) |
                           (int64_t(accPartForDst(L.src, sval) & M24) << 24);
                } else if ((isRN) && isAccSrc(L.src)) {
                    sval = accPartForDst(L.src, sval, false);
                } else if (isXY && isAccSrc(L.src)) {
                    sval = accPartForDst(L.src, sval);
                }
                setReg(dst, sval);
            } else {
                EA ea;
                if (!parseEA(dst, ea)) { fprintf(stderr, "bad dst %s\n", dst.c_str()); abort(); }
                int kind; uint32_t addr; int rnum; int64_t delta;
                evalEA(ea, kind, addr, rnum, delta);
                eas.push_back(ea);
                if (ea.space == 'l') {
                    if (L.src == "ba" || L.src == "ab") {
                        wrL(addr, uint64_t(sval));
                    } else if (isAccSrc(L.src)) {
                        if (L.src == "a" || L.src == "b") wrL(addr, uint64_t(sval) & M48);
                        else if (L.src == "a0" || L.src == "b0") wr("y", addr, uint32_t(sval) & M24);
                        else if (L.src == "a1" || L.src == "b1") wr("x", addr, uint32_t(sval >> 24) & M24);
                        else wr("x", addr, accPartForDst(L.src, sval));
                    } else {
                        wrL(addr, uint64_t(sval));
                    }
                } else {
                    if (isAccSrc(L.src)) sval = accPartForDst(L.src, sval);
                    wr(ea.space == 'x' ? "x" : "y", addr, uint32_t(sval) & M24);
                }
            }
        }
        commitEAUpdates(eas);
    }
    void doMoves(const std::vector<std::string>& toks, size_t from) {
        std::vector<Latch> latch;
        std::vector<EA> eas;
        for (size_t i = from; i < toks.size(); ++i)
            latchRead(toks[i], latch, eas);
        commitWrites(latch, eas);
    }

    // ---------------- exec -----------------------------------------------------
    bool condTrue(const std::string& c) {
        if (c == "ifcc") return !f.c;
        if (c == "ifcs") return !!f.c;
        if (c == "ifne") return !f.z;
        if (c == "ifeq") return !!f.z;
        if (c == "ifpl") return !f.n;
        if (c == "ifmi") return !!f.n;
        if (c == "ifge") return f.n == f.v;
        if (c == "iflt") return f.n != f.v;
        if (c == "ifgt") return !f.z && (f.n == f.v);
        if (c == "ifle") return f.z || (f.n != f.v);
        return true;
    }
    uint32_t branchTarget(const std::string& tok) {
        std::string t = tok;
        while (!t.empty() && (t[0] == '>' || t[0] == '<' || t[0] == '$')) t.erase(0, 1);
        auto it = labels.find(t);
        if (it != labels.end()) return it->second;
        static std::regex rp("^\\((r\\d)\\)$");
        std::smatch m;
        if (std::regex_match(t, m, rp))
            return R[atoi(m[1].str().c_str() + 1)] & M24;
        return uint32_t(absVal(tok));
    }
    static bool isMacOp(const std::string& m) {
        return m == "mac" || m == "mpy" || m == "macr" || m == "mpyr" ||
               m == "macsu" || m == "mpysu" || m == "mpyuu" || m == "dmac" ||
               m == "maci" || m == "mpyi" || m == "mpyri" || m == "macri";
    }

    // returns new pc, or 0xFFFFFFFF for fallthrough
    uint32_t exec(const Ins& ins, uint32_t pcNow) {
        const std::string& mnem = ins.mnem;
        const std::vector<std::string>& toks = ins.toks;
        if (mnem == "do") {
            std::string cntTok, endTok;
            {
                size_t c = toks[0].find(',');
                if (c != std::string::npos) {
                    cntTok = toks[0].substr(0, c);
                    endTok = toks[0].substr(c + 1);
                    if (!endTok.empty() && endTok[0] == '>') endTok.erase(0, 1);
                } else { cntTok = toks[0]; endTok = toks.back(); }
            }
            int64_t cnt;
            if (!cntTok.empty() && cntTok[0] == '#') cnt = absVal(cntTok);
            else if (cntTok[0] == 'n') cnt = getReg24(cntTok) & M24;
            else if (cntTok == "a1" || cntTok == "b1") cnt = getReg24(cntTok) & M24;
            else if (cntTok == "x0" || cntTok == "x1" || cntTok == "y0" || cntTok == "y1")
                cnt = getReg24(cntTok) & M24;
            else { fprintf(stderr, "do cnt %s\n", cntTok.c_str()); abort(); }
            uint32_t end = labels.count(endTok) ? labels[endTok] : uint32_t(absVal(endTok));
            end -= 1;
            if (cnt <= 0) return end + 1;
            doStack.push_back(DoEnt{cnt, pcNow + 2, end});
            return 0xFFFFFFFFu;
        }
        if (mnem == "rep") {
            int64_t cnt = toks[0][0] == '#' ? absVal(toks[0]) : getReg24(toks[0]) & M24;
            if (cnt <= 0) return 0xFFFFFFFFu;
            repCount = cnt;
            repArmed = true;
            return 0xFFFFFFFFu;
        }
        if (mnem == "btst" || mnem == "bset" || mnem == "bclr" || mnem == "bchg") {
            std::string bitTok, loc;
            size_t c = toks[0].find(',');
            if (c != std::string::npos) { bitTok = toks[0].substr(0, c); loc = toks[0].substr(c + 1); }
            else { bitTok = toks[0]; loc = toks[1]; }
            int bit = absVal(bitTok);
            if (loc == "a" || loc == "b") {
                int64_t acc = getAcc(loc);
                uint32_t main = uint32_t(acc >> 24) & M24;
                f.c = uint8_t((main >> bit) & 1);
                if (mnem == "bset") setReg(loc, (acc & ~int64_t((1 << bit) << 24)) | (int64_t(1 << bit) << 24));
                else if (mnem == "bclr") setReg(loc, acc & ~int64_t((1 << bit) << 24));
                else if (mnem == "bchg") setReg(loc, acc ^ (int64_t(1 << bit) << 24));
                return 0xFFFFFFFFu;
            }
            static std::regex simple("^[ab]\\d?$|^(x0|x1|y0|y1|sr|ccr|r\\d|n\\d|m\\d)$");
            if (std::regex_match(loc, simple)) {
                uint32_t val = getReg24(loc);
                uint32_t bitval = (val >> bit) & 1;
                f.c = uint8_t(bitval);
                if (loc == "sr") {
                    if (mnem == "bset") {
                        srInt |= (1u << bit);
                        if (bit == 0) f.c = 1; else if (bit == 1) f.v = 1;
                        else if (bit == 2) f.z = 1; else if (bit == 3) f.n = 1;
                    } else if (mnem == "bclr") {
                        srInt &= ~(1u << bit);
                        if (bit == 0) f.c = 0; else if (bit == 1) f.v = 0;
                        else if (bit == 2) f.z = 0; else if (bit == 3) f.n = 0;
                    } else {
                        srInt ^= (1u << bit);
                    }
                    return 0xFFFFFFFFu;
                }
                if (mnem == "bset") setReg(loc, val | (1u << bit));
                else if (mnem == "bclr") setReg(loc, val & ~(1u << bit));
                else if (mnem == "bchg") setReg(loc, val ^ (1u << bit));
                return 0xFFFFFFFFu;
            }
            EA ea;
            parseEA(loc, ea);
            int kind; uint32_t addr; int rnum; int64_t delta;
            evalEA(ea, kind, addr, rnum, delta);
            uint32_t val = rd(ea.space == 'x' ? "x" : "y", addr);
            uint32_t bitval = (val >> bit) & 1;
            f.c = uint8_t(bitval);
            if (mnem == "bset") wr(ea.space == 'x' ? "x" : "y", addr, val | (1u << bit));
            else if (mnem == "bclr") wr(ea.space == 'x' ? "x" : "y", addr, val & ~(1u << bit));
            else if (mnem == "bchg") wr(ea.space == 'x' ? "x" : "y", addr, val ^ (1u << bit));
            return 0xFFFFFFFFu;
        }
        if (mnem == "jmp" || mnem == "bra") return branchTarget(toks[0]);
        if (mnem == "jsr") {
            uint32_t tgt = branchTarget(toks[0]);
            retStack.push_back(nextAddr.count(pcNow) ? nextAddr[pcNow] : pcNow + 1);
            return tgt;
        }
        if (mnem == "bsr") {
            retStack.push_back(nextAddr.count(pcNow) ? nextAddr[pcNow] : pcNow + 1);
            return branchTarget(toks[0]);
        }
        if (mnem == "rts") {
            if (retStack.empty()) { halted = true; return 0xFFFFFFFFu; }
            uint32_t t = retStack.back();
            retStack.pop_back();
            return t;
        }
        if (mnem.size() > 1 && mnem[0] == 'b' && mnem != "bset" && mnem != "bclr" &&
            mnem != "btst" && mnem != "brset" && mnem != "brclr" && mnem != "bchg" &&
            toks.size() == 1) {
            static const std::unordered_map<std::string, int> bc = {
                {"cc", 0}, {"cs", 1}, {"ne", 2}, {"eq", 3}, {"pl", 4}, {"mi", 5},
                {"ge", 6}, {"lt", 7}, {"gt", 8}, {"le", 9},
                {"ec", 10}, {"es", 11}, {"vc", 12}, {"vs", 13}};
            std::string cond = mnem.substr(1);
            auto it = bc.find(cond);
            if (it != bc.end()) {
                bool taken;
                switch (it->second) {
                    case 0: taken = !f.c; break; case 1: taken = !!f.c; break;
                    case 2: taken = !f.z; break; case 3: taken = !!f.z; break;
                    case 4: taken = !f.n; break; case 5: taken = !!f.n; break;
                    case 6: taken = f.n == f.v; break; case 7: taken = f.n != f.v; break;
                    case 8: taken = !f.z && (f.n == f.v); break;
                    case 9: taken = f.z || (f.n != f.v); break;
                    case 10: taken = !f.e; break;
                    case 11: taken = !!f.e; break;
                    case 12: taken = !f.v; break;
                    default: taken = !!f.v; break;
                }
                if (taken) return branchTarget(toks[0]);
                return 0xFFFFFFFFu;
            }
        }
        if (mnem == "jset" || mnem == "jclr" || mnem == "brset" || mnem == "brclr") {
            std::string bt, loc, tgt;
            if (toks.size() == 1) {
                size_t c1 = toks[0].find(','), c2 = toks[0].find(',', c1 + 1);
                bt = toks[0].substr(0, c1);
                loc = toks[0].substr(c1 + 1, c2 - c1 - 1);
                tgt = toks[0].substr(c2 + 1);
            } else { bt = toks[0]; loc = toks[1]; tgt = toks[2]; }
            int bit = absVal(bt);
            uint32_t val = (loc == "a" || loc == "b")
                               ? uint32_t((getAcc(loc) >> 24) & M24) : getReg24(loc);
            uint32_t bitval = (val >> bit) & 1;
            bool taken = (mnem.size() >= 4 && mnem.substr(mnem.size() - 3) == "set") ? bitval == 1 : bitval == 0;
            if ((mnem == "brset" || mnem == "brclr") && taken) return branchTarget(tgt);
            if ((mnem == "jset" || mnem == "jclr") && taken) return branchTarget(tgt);
            return 0xFFFFFFFFu;
        }
        if (mnem == "bsset" || mnem == "bsclr" || mnem == "bschg") {
            // Bit test + modify + branch (DSP56300 FM):
            //   bsset: bit := 1, branch if the bit WAS set
            //   bsclr: bit := 0, branch if the bit WAS clear
            //   bschg: bit := ~bit, branch if the bit WAS clear
            // For A/B destinations bit n (0..23) addresses A1/B1 (acc bits n+24).
            std::string bt, loc, tgt;
            if (toks.size() == 1) {
                size_t c1 = toks[0].find(','), c2 = toks[0].find(',', c1 + 1);
                bt = toks[0].substr(0, c1);
                loc = toks[0].substr(c1 + 1, c2 - c1 - 1);
                tgt = toks[0].substr(c2 + 1);
            } else { bt = toks[0]; loc = toks[1]; tgt = toks[2]; }
            int bit = absVal(bt);
            int bitval;
            if (loc == "a" || loc == "b") {
                int64_t acc = getAcc(loc);
                bitval = int((acc >> (bit + 24)) & 1);
                int64_t mask = int64_t(1) << (bit + 24);
                if (mnem == "bsset") acc |= mask;
                else if (mnem == "bsclr") acc &= ~mask;
                else acc ^= mask;
                setReg(loc, acc);
            } else {
                uint32_t val = getReg24(loc);
                bitval = int((val >> bit) & 1);
                if (mnem == "bsset") val |= (1u << bit);
                else if (mnem == "bsclr") val &= ~(1u << bit);
                else val ^= (1u << bit);
                setReg(loc, val);
            }
            bool taken = (mnem == "bsset") ? (bitval == 1) : (bitval == 0);
            if (taken) return branchTarget(tgt);
            return 0xFFFFFFFFu;
        }
        if (mnem == "andi" || mnem == "ori") {
            uint32_t v = uint32_t(absVal(toks[0].substr(0, toks[0].find(','))));
            uint32_t cur = uint32_t(f.c | (f.v << 1) | (f.z << 2) | (f.n << 3));
            if (mnem == "andi") cur &= v; else cur |= v;
            f.c = uint8_t(cur & 1);
            f.v = uint8_t((cur >> 1) & 1);
            f.z = uint8_t((cur >> 2) & 1);
            f.n = uint8_t((cur >> 3) & 1);
            return 0xFFFFFFFFu;
        }
        if (mnem == "move") { doMoves(toks, 0); return 0xFFFFFFFFu; }
        if (mnem == "movep") { doMoves(toks, 0); return 0xFFFFFFFFu; }  // I/O port move: same 24-bit data path
        if (mnem == "lua") {
            std::string srcTok, dst;
            size_t c = toks[0].find(',');
            if (c != std::string::npos) { srcTok = toks[0].substr(0, c); dst = toks[0].substr(c + 1); }
            else { srcTok = toks[0]; dst = toks[1]; }
            static std::regex m1("^\\((r\\d)\\)(\\+|-)?(n\\d)?$");
            std::smatch mm;
            int64_t delta = 0;
            int rnum;
            if (std::regex_match(srcTok, mm, m1)) {
                rnum = atoi(mm[1].str().c_str() + 1);
                if (mm[2].str() == "+") delta = mm[3].matched ? int64_t(N[atoi(mm[3].str().c_str() + 1)]) : 1;
                else if (mm[2].str() == "-") delta = mm[3].matched ? -int64_t(N[atoi(mm[3].str().c_str() + 1)]) : -1;
            } else {
                static std::regex m2("^\\((r\\d)([+-]\\$?\\w+)\\)$");
                if (!std::regex_match(srcTok, mm, m2)) { fprintf(stderr, "lua %s\n", srcTok.c_str()); abort(); }
                rnum = atoi(mm[1].str().c_str() + 1);
                delta = absVal(mm[2].str());
            }
            setReg(dst, int64_t(R[rnum] + delta) & M24);
            return 0xFFFFFFFFu;
        }
        if (mnem == "nop" || mnem == "pflush" || mnem == "debug" || mnem == "wait" ||
            mnem == "stop") return 0xFFFFFFFFu;
        if (mnem == "rnd") {
            rndAcc(toks[0].substr(0, toks[0].find(',')));
            if (toks.size() > 1) doMoves(toks, 1);
            return 0xFFFFFFFFu;
        }
        if (mnem == "div") {
            std::string sTok, dTok;
            size_t c = toks[0].find(',');
            if (c != std::string::npos) { sTok = toks[0].substr(0, c); dTok = toks[0].substr(c + 1); }
            else { sTok = toks[0]; dTok = toks[1]; }
            divStep(sTok, dTok);
            return 0xFFFFFFFFu;
        }
        if (isMacOp(mnem) ||
            mnem == "add" || mnem == "sub" || mnem == "subr" || mnem == "addr" ||
            mnem == "addc" || mnem == "sbc" || mnem == "tfr" || mnem == "cmp" ||
            mnem == "cmpm" || mnem == "tst" || mnem == "neg" || mnem == "abs" ||
            mnem == "clr" || mnem == "and" || mnem == "or" || mnem == "eor" ||
            mnem == "asl" || mnem == "asr" || mnem == "lsl" || mnem == "lsr" ||
            mnem == "tge" || mnem == "tgt" || mnem == "tle" || mnem == "tlt" ||
            mnem == "teq" || mnem == "tne" || mnem == "tpl" || mnem == "tmi") {
            execAlu(mnem, toks, pcNow);
            return 0xFFFFFFFFu;
        }
        fprintf(stderr, "mnm kernel: unimplemented %s at %06X\n", mnem.c_str(), pcNow);
        abort();
    }

    void execAlu(const std::string& mnem, const std::vector<std::string>& toks,
                 uint32_t pcNow) {
        std::vector<std::string> T(toks);
        std::string cond;
        if (!T.empty() && T.back().rfind("if", 0) == 0) { cond = T.back(); T.pop_back(); }
        std::vector<std::string> aluToks, moveToks;
        if (isMacOp(mnem)) {
            int commas = 0;
            while (!T.empty() && commas < 2) {
                std::string tk = T.front(); T.erase(T.begin());
                commas += int(std::count(tk.begin(), tk.end(), ','));
                aluToks.push_back(tk);
            }
        } else if (mnem == "add" || mnem == "sub" || mnem == "addc" || mnem == "sbc" ||
                   mnem == "subr" || mnem == "addr" || mnem == "tfr" || mnem == "cmp" ||
                   mnem == "cmpm" || mnem == "and" || mnem == "or" || mnem == "eor" ||
                   mnem == "tge" || mnem == "tgt" || mnem == "tle" || mnem == "tlt" ||
                   mnem == "teq" || mnem == "tne" || mnem == "tpl" || mnem == "tmi") {
            int commas = 0;
            while (!T.empty() && commas < 1) {
                std::string tk = T.front(); T.erase(T.begin());
                commas += int(std::count(tk.begin(), tk.end(), ','));
                aluToks.push_back(tk);
            }
        } else if (mnem == "asl" || mnem == "asr" || mnem == "lsl" || mnem == "lsr") {
            aluToks.push_back(T.front()); T.erase(T.begin());
        } else { // neg/abs/clr/tst
            if (!T.empty()) { aluToks.push_back(T.front()); T.erase(T.begin()); }
        }
        moveToks = T;
        // latch parallel move reads
        std::vector<Latch> latch;
        std::vector<EA> eas;
        for (const std::string& tk : moveToks) latchRead(tk, latch, eas);
        bool runAlu = cond.empty() || condTrue(cond);
        if (runAlu) aluExec(mnem, aluToks, pcNow);
        commitWrites(latch, eas);
    }

    void aluExec(const std::string& mnem, std::vector<std::string>& toks,
                 uint32_t pcNow) {
        if (isMacOp(mnem)) {
            std::string sign2;
            if (!toks.empty() && (toks[0] == "ss" || toks[0] == "su" ||
                                  toks[0] == "us" || toks[0] == "uu")) {
                sign2 = toks[0]; toks.erase(toks.begin());
            }
            bool neg = false;
            std::string core = toks[0];
            if (!core.empty() && core[0] == '-' && core.find(',') != std::string::npos) {
                core = core.substr(1); neg = true;
            }
            std::vector<std::string> parts;
            size_t s = 0;
            while (true) {
                size_t c = core.find(',', s);
                parts.push_back(core.substr(s, c == std::string::npos ? std::string::npos : c - s));
                if (c == std::string::npos) break;
                s = c + 1;
            }
            std::string s1, s2, dst;
            if (parts.size() == 3) { s1 = parts[0]; s2 = parts[1]; dst = parts[2]; }
            else { s1 = parts[0]; s2 = parts[1]; dst = toks[1]; }
            uint32_t v1u, v2u;
            if (mnem.size() && mnem.back() == 'i') {
                v1u = uint32_t(absVal(s1)) & M24;   // raw immediate (python abs_val)
                v2u = getReg24(s2);
            } else {
                v1u = getReg24(s1);
                v2u = getReg24(s2);
            }
            if (neg) v1u = sgn24raw(-sgn24(v1u));
            int64_t prod;
            if (mnem == "mpyuu" || mnem == "macuu" || sign2 == "uu") {
                prod = int64_t(v1u & M24) * int64_t(v2u & M24);
            } else if ((mnem == "macsu" || mnem == "mpysu") || sign2 == "su") {
                int64_t a1 = v1u & M24;
                int64_t s1v = (a1 & 0x800000) ? a1 - (1 << 24) : a1;
                prod = s1v * int64_t(v2u & M24);
            } else if (sign2 == "us") {
                int64_t a2 = v2u & M24;
                int64_t s2v = (a2 & 0x800000) ? a2 - (1 << 24) : a2;
                prod = int64_t(v1u & M24) * s2v;
            } else {
                int64_t a1 = v1u & M24, a2 = v2u & M24;
                int64_t s1v = (a1 & 0x800000) ? a1 - (1 << 24) : a1;
                int64_t s2v = (a2 & 0x800000) ? a2 - (1 << 24) : a2;
                prod = s1v * s2v;
            }
            prod <<= 1;
            int64_t acc = getAcc(dst);
            bool isMac = mnem.rfind("mac", 0) == 0 || mnem == "dmac";
            int64_t res = isMac ? acc + prod : prod;
            int64_t res56 = flagsFrom(res);
            f.v = (-(1ll << 47) <= res56 && res56 < (1ll << 47)) ? 0 : 1;
            setReg(dst, res56);
            if (mnem == "macr" || mnem == "mpyr" || mnem == "mpyri" || mnem == "macri")
                rndAcc(dst);
            return;
        }
        if (mnem == "add" || mnem == "sub" || mnem == "subr" || mnem == "addr" ||
            mnem == "addc" || mnem == "sbc" || mnem == "tfr" || mnem == "cmp" ||
            mnem == "cmpm" || mnem == "and" || mnem == "or" || mnem == "eor" ||
            mnem == "tge" || mnem == "tgt" || mnem == "tle" || mnem == "tlt" ||
            mnem == "teq" || mnem == "tne" || mnem == "tpl" || mnem == "tmi") {
            std::string core = toks[0];
            std::vector<std::string> parts;
            size_t s = 0;
            while (true) {
                size_t c = core.find(',', s);
                parts.push_back(core.substr(s, c == std::string::npos ? std::string::npos : c - s));
                if (c == std::string::npos) break;
                s = c + 1;
            }
            std::string sd, d;
            if (parts.size() == 2) { sd = parts[0]; d = parts[1]; }
            else if (parts.size() == 1 && toks.size() > 1) { sd = parts[0]; d = toks[1]; }
            else { sd = ""; d = parts[0]; }
            if (mnem == "add") aluAdd(d, getAcc(d), aluSrc(sd));
            else if (mnem == "addc") aluAdd(d, getAcc(d), aluSrc(sd), f.c);
            else if (mnem == "sub") aluSub(d, getAcc(d), aluSrc(sd));
            else if (mnem == "subr") aluSub(d, aluSrc(sd), getAcc(d));
            else if (mnem == "addr") aluAdd(d, aluSrc(sd), getAcc(d));
            else if (mnem == "sbc") aluSub(d, getAcc(d), aluSrc(sd) + ((1ll << 24) * f.c));
            else if (mnem == "tfr") setReg(d, aluSrc(sd));
            else if (mnem == "cmp" || mnem == "cmpm") aluSub(d, getAcc(d), aluSrc(sd), false);
            else if (mnem == "and" || mnem == "or" || mnem == "eor") {
                uint32_t a1 = uint32_t((getAcc(d) >> 24) & M24);
                uint32_t b = sd[0] == '#' ? uint32_t(absVal(sd)) & M24 : getReg24(sd);
                uint32_t r = mnem == "and" ? (a1 & b) : mnem == "or" ? (a1 | b) : (a1 ^ b);
                uint32_t b0 = uint32_t(getAcc(d) & M24);
                int64_t r56 = (int64_t(r) << 24) | b0;
                if (r & 0x800000) r56 |= int64_t(0xFF) << 48;
                f.n = (r & 0x800000) ? 1 : 0;
                f.z = r == 0 ? 1 : 0;
                setReg(d, r56);
            } else {
                // conditional transfers tge/tgt/...
                std::string c = mnem.substr(1);
                std::string cc = "if" + c;
                if (condTrue(cc)) setReg(d, aluSrc(sd));
            }
            return;
        }
        if (mnem == "asl" || mnem == "asr" || mnem == "lsl" || mnem == "lsr") {
            std::vector<std::string> fields;
            for (const std::string& tk : toks) {
                size_t s = 0;
                while (true) {
                    size_t c = tk.find(',', s);
                    fields.push_back(tk.substr(s, c == std::string::npos ? std::string::npos : c - s));
                    if (c == std::string::npos) break;
                    s = c + 1;
                }
            }
            int64_t cnt = -1;
            size_t restIdx = 0;
            if (!fields.empty() && fields[0][0] == '#') {
                cnt = absVal(fields[0]) & 63;
                restIdx = 1;
            }
            std::vector<std::string> rest(fields.begin() + restIdx, fields.end());
            std::string sd, d;
            if (rest.size() == 3) {
                cnt = getRegPy(rest[0]) & 63;
                sd = rest[1]; d = rest[2];
            } else if (rest.size() == 2) { sd = rest[0]; d = rest[1]; if (cnt < 0) cnt = 1; }
            else if (rest.size() == 1) { sd = d = rest[0]; if (cnt < 0) cnt = 1; }
            else { fprintf(stderr, "shift fields\n"); abort(); }
            int64_t v = getAcc(sd);
            int64_t r;
            if (mnem == "asl") {
                r = v << cnt;
                f.c = (0 < cnt && cnt <= 56) ? uint8_t((uint64_t(v) >> (56 - cnt)) & 1) : 0;
            } else if (mnem == "asr") {
                r = v >> cnt;
                f.c = cnt ? uint8_t((uint64_t(v) >> (cnt - 1)) & 1) : 0;
            } else if (mnem == "lsl") {
                r = (v << cnt) & int64_t(M56);
                f.c = (0 < cnt && cnt <= 56) ? uint8_t((uint64_t(v) >> (56 - cnt)) & 1) : 0;
            } else {
                r = (v & int64_t(M56)) >> cnt;
                f.c = cnt ? uint8_t((uint64_t(v) >> (cnt - 1)) & 1) : 0;
            }
            r = flagsFrom(r);
            setReg(d, r);
            return;
        }
        // neg / abs / clr / tst
        std::string d = toks.empty() ? "a" : toks[0];
        if (d.find(',') != std::string::npos) d = d.substr(0, d.find(','));
        if (mnem == "clr") {
            f.u = 0;
            setReg(d, flagsFrom(0));
        } else if (mnem == "neg") {
            int64_t v = flagsFrom(-getAcc(d));
            f.v = sext(v, 56) == -(1ll << 55) ? 1 : 0;
            setReg(d, v);
        } else if (mnem == "abs") {
            setReg(d, flagsFrom(llabs(getAcc(d))));
        } else { // tst
            flagsFrom(getAcc(d));
        }
    }

    // ---------------- run -------------------------------------------------------
    std::function<void(uint32_t, DSPKernel&)> hook;   // optional debug hook
    // Runs from `start` until pc == `end` (exclusive). Returns steps executed.
    int64_t run(uint32_t start, uint32_t end, int64_t maxSteps = 50000000) {
        pc = start;
        halted = false;
        int64_t n = 0;
        while (!halted && n < maxSteps) {
            uint32_t cur = pc;
            if (cur == end) return n;
            if (hook) hook(cur, *this);
            // native machine hooks
            if (cur == procAddr && onProc) { pc = onProc(*this); ++n; continue; }
            if (cur == confAddr && onConf) { pc = onConf(*this); ++n; continue; }
            if (cur == initAddr && onInit) { pc = onInit(*this); ++n; continue; }
            if (cur == nullAddr) {
                if (retStack.empty()) { halted = true; return n; }
                pc = retStack.back();
                retStack.pop_back();
                ++n;
                continue;
            }
            auto it = prog.find(cur);
            if (it == prog.end()) {
                fprintf(stderr, "mnm kernel: no instruction at %06X\n", cur);
                halted = true;
                abort();
            }
            uint32_t nextPc = nextAddr.count(cur) ? nextAddr[cur] : cur + 1;
            uint32_t handled = exec(it->second, cur);
            pc = (handled != 0xFFFFFFFFu) ? handled : nextPc;
            ++n;
            // REP management
            if (repArmed) {
                if (cur == repAfter) {
                    if (repCount > 1) { --repCount; pc = cur; }
                    else repArmed = false;
                } else repArmed = false;
            }
            if (it->second.mnem == "rep") repAfter = nextPc;
            // DO management
            for (size_t i = 0; i < doStack.size(); ++i) {
                DoEnt& e = doStack[i];
                if (cur <= e.end && e.end < nextPc) {
                    e.cnt -= 1;
                    if (e.cnt > 0) pc = e.start;
                    else doStack.erase(doStack.begin() + i);
                    break;
                }
            }
        }
        return n;
    }
};

} // namespace mnm
