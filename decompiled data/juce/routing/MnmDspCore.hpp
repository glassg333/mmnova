// ============================================================================
// MnmDspCore.hpp — canonical bit-exact DSP56300 program core (C++17, header-only)
//
// Monomachine OS 1.32B mining, iteration 62. This is a 1:1 C++ port of the
// validated Python reference emulator (scripts/dsp_emu.py, 27/27 semantics
// tests; machine ports m1/m2/m3 reached 7680/7680 bit-exact against it).
//
// WHY A CORE, NOT A HAND-PORT: every previous "bit-exact" hand-port of the
// track chain diverged (the user holds 10 different versions). Executing the
// REAL firmware program text makes bit-exactness structural: one program
// image, one core, one vector set — versions cannot drift apart.
//
// Semantics locked to the reference:
//   * 24-bit words; X0/X1/Y0/Y1 signed 24-bit; A/B = 56-bit (A2:A1:A0)
//   * MAC: signed 24x24 -> 48-bit product, <<1, accumulate into 56-bit acc
//   * RND/MACR/MPYR: add $800000 to A0, round into A1, A0 = 0
//   * DIV: one non-restoring step (24-step loop = classic idiom)
//   * parallel moves latch PRE-ALU accumulator values (pipelined store)
//   * data-limit checking: A/B -> memory/x0..y1 saturates at $7FFFFF/$800000
//     (explicit a0/a1/a2 parts bypass it; R/N destinations bypass it)
//   * DO hardware loop: body = [do_pc+2 .. ext_end]; loop check after insn
//   * L memory: A1 -> X-half, A0 -> Y-half
//   * imm8 (no <> marker): RAW to A/B/R/N, LEFT-JUSTIFIED (xx<<16) elsewhere
//   * logical and/or/eor touch A1 only, A0 preserved, A2 = sign extension
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <regex>
#include <array>
#include <algorithm>
#include <functional>

namespace mnmdsp {

static const uint32_t MASK24 = 0xFFFFFFu;
static const uint64_t MASK48 = 0xFFFFFFFFFFFFull;
static const uint64_t MASK56 = 0xFFFFFFFFFFFFFFull;

struct Ins {
    std::string mnem;
    std::vector<std::string> toks;
    std::string text;
};

struct Flags { int c = 0, v = 0, z = 0, n = 0, e = 0, u = 0; };

struct Ea { char space; std::string mode; int rnum; long arg; std::string arg2; };

class Core {
public:
    std::unordered_map<uint32_t, Ins> prog;
    std::map<uint32_t, uint32_t> next_addr;
    std::unordered_map<std::string, uint32_t> labels;
    std::unordered_map<uint32_t, uint32_t> X, Y;

    int64_t A = 0, B = 0;                    // 56-bit signed
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
    uint32_t R[8] = {0,0,0,0,0,0,0,0};
    int32_t  N[8] = {0,0,0,0,0,0,0,0};
    uint32_t M[8] = {MASK24,MASK24,MASK24,MASK24,MASK24,MASK24,MASK24,MASK24};
    Flags f;
    uint32_t sr_int = 0;
    uint32_t pc = 0;
    // DO loop entries: [count, body_start, end_addr]
    std::vector<int64_t> do_stack;           // triples
    int64_t rep_count = 0;
    bool     rep_valid = false;
    uint32_t rep_after = 0;
    std::vector<uint32_t> ret_stack;
    uint32_t steps = 0;
    uint64_t max_steps = 50000000ull;
    std::vector<uint32_t> trace_ring;         // last 64 PCs (debug)
    bool trace_on = false;

    // ---------------- parsing ----------------
    static std::vector<std::string> split_ws(const std::string& s) {
        std::vector<std::string> out; std::string cur;
        for (char c : s) {
            if (isspace((unsigned char)c)) { if (!cur.empty()) { out.push_back(cur); cur.clear(); } }
            else cur += c;
        }
        if (!cur.empty()) out.push_back(cur);
        return out;
    }
    void parse(const std::vector<std::string>& lines) {
        static std::regex line_re("^\\s*([0-9A-Fa-f]{6}):\\s*(.*?)\\s*;\\s*([0-9A-Fa-f ]+)$");
        for (auto& raw : lines) {
            std::smatch m;
            if (!std::regex_match(raw, m, line_re)) continue;
            uint32_t addr = (uint32_t) strtoul(m[1].str().c_str(), nullptr, 16);
            std::string text = m[2].str();
            size_t b = text.find_first_not_of(" \t");
            if (b == std::string::npos) continue;
            size_t e = text.find_last_not_of(" \t");
            text = text.substr(b, e - b + 1);
            if (text.rfind("func_", 0) == 0 || text.rfind("int_", 0) == 0) continue;
            auto toks = split_ws(text);
            if (toks.empty()) continue;
            std::string mnem = toks[0];
            for (auto& c : mnem) c = (char) tolower((unsigned char) c);
            std::vector<std::string> ops(toks.begin() + 1, toks.end());
            for (auto& tk : ops) {
                size_t s = 0;
                while (true) {
                    size_t c = tk.find(',', s);
                    std::string part = tk.substr(s, c == std::string::npos ? std::string::npos : c - s);
                    std::string p = part;
                    while (!p.empty() && (p[0] == '>' || p[0] == '<' || p[0] == '#' || p[0] == '$')) p.erase(p.begin());
                    if (p.rfind("func_", 0) == 0 || p.rfind("int_", 0) == 0)
                        labels[p] = (uint32_t) strtoul(p.c_str() + 5, nullptr, 16);
                    if (c == std::string::npos) break;
                    s = c + 1;
                }
            }
            prog[addr] = Ins{mnem, ops, text};
        }
        std::vector<uint32_t> order; order.reserve(prog.size());
        for (auto& kv : prog) order.push_back(kv.first);
        std::sort(order.begin(), order.end());
        for (size_t i = 0; i < order.size(); ++i)
            next_addr[order[i]] = (i + 1 < order.size()) ? order[i + 1] : order[i] + 1;
    }

    // ---------------- memory ----------------
    inline uint32_t rd(char sp, uint32_t ea) const {
        if (sp == 'x') { auto it = X.find(ea); return it == X.end() ? 0 : it->second & MASK24; }
        auto it = Y.find(ea); return it == Y.end() ? 0 : it->second & MASK24;
    }
    std::vector<uint32_t> watch_set;          // (space<<24)|addr
    std::vector<std::string> watch_log;       // "pc sp:addr val"
    // debug: full write log  {step, pc, (spaceY<<24)|addr, val}
    bool log_all_writes = false;
    std::vector<std::array<uint32_t, 4>> wlog;
    inline void wr(char sp, uint32_t ea, uint32_t val) {
        if (log_all_writes)
            wlog.push_back({(uint32_t)(steps - 1), pc,
                            (uint32_t)((sp == 'y' ? 0x1000000u : 0u) | (ea & MASK24)),
                            val & MASK24});
        if (!watch_set.empty()) {
            uint32_t key = ((sp == 'x') ? 0u : 1u << 24) | ea;
            for (auto k : watch_set)
                if (k == ((sp == 'x') ? ea : (0x1000000u | ea))) {
                    char buf[48];
                    snprintf(buf, sizeof(buf), "%04x %c:%03x %06x", pc, sp, ea, val & MASK24);
                    watch_log.push_back(buf);
                    break;
                }
        }
        (sp == 'x' ? X : Y)[ea] = val & MASK24;
    }
    inline uint64_t rdL(uint32_t ea) const { return ((uint64_t) rd('x', ea) << 24) | rd('y', ea); }
    inline void wrL(uint32_t ea, uint64_t v48) { wr('x', ea, (uint32_t)(v48 >> 24)); wr('y', ea, (uint32_t)(v48 & MASK24)); }

    // ---------------- helpers ----------------
    static int64_t sext(int64_t v, int bits) {
        int64_t s = 1ll << (bits - 1);
        return (v & ((1ll << bits) - 1)) - ((v & s) << 1);
    }
    static int32_t sgn24(uint32_t v) { return (v & 0x800000) ? (int32_t)(v - (int64_t) 0x1000000) : (int32_t) v; }

    uint32_t acc_part_for_dst(const std::string& src, int64_t sval, bool limit = true) const {
        if (src == "a" || src == "b") {
            int64_t v = sext(sval, 56);
            if (limit) {
                if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFF;
                if (v < -0x00800000000000ll) return 0x800000;
            }
            return (uint32_t)((v >> 24) & MASK24);
        }
        if (src == "a0" || src == "b0") return (uint32_t)(sval & MASK24);
        if (src == "a1" || src == "b1") return (uint32_t)((sval >> 24) & MASK24);
        if (src == "a2" || src == "b2") return (uint32_t)((sval >> 48) & 0xFF);
        return (uint32_t)(sval & MASK24);
    }
    static int64_t compose_acc(uint32_t v) { return sext(v & MASK24, 24) << 24; }

    int64_t alu_src(const std::string& name) {
        if (!name.empty() && name[0] == '#') return sext((uint32_t) abs_val(name) & MASK24, 24) << 24;
        int64_t v = get_reg(name);
        if (name == "x0" || name == "x1" || name == "y0" || name == "y1")
            return sext(v, 24) << 24;
        return sext(v, 56);
    }
    static bool is_acc_src(const std::string& n) {
        return n == "a" || n == "b" || n == "a0" || n == "a1" || n == "a2" ||
               n == "b0" || n == "b1" || n == "b2";
    }
    static bool is_rnm(const std::string& n) {
        return n.size() == 2 && (n[0] == 'r' || n[0] == 'n' || n[0] == 'm') && isdigit((unsigned char) n[1]);
    }

    int64_t get_reg(const std::string& name) const {
        if (name == "a") return A;
        if (name == "b") return B;
        if (name == "a0") return (uint32_t)(A & MASK24);
        if (name == "b0") return (uint32_t)(B & MASK24);
        if (name == "a1") return (uint32_t)((A >> 24) & MASK24);
        if (name == "b1") return (uint32_t)((B >> 24) & MASK24);
        if (name == "a2") return (uint32_t)((A >> 48) & 0xFF);
        if (name == "b2") return (uint32_t)((B >> 48) & 0xFF);
        if (name == "x0") return x0;
        if (name == "x1") return x1;
        if (name == "y0") return y0;
        if (name == "y1") return y1;
        if (is_rnm(name)) {
            if (name[0] == 'r') return R[name[1] - '0'];
            if (name[0] == 'n') return (uint32_t) N[name[1] - '0'];
            return M[name[1] - '0'];
        }
        if (name == "x") return ((int64_t) x1 << 24) | x0;
        if (name == "y") return ((int64_t) y1 << 24) | y0;
        if (name == "sr" || name == "ccr") return sr_int & 0xFFFF;
        fprintf(stderr, "get_reg %s\n", name.c_str()); abort();
    }

    void set_reg(const std::string& name, int64_t val) {
        if (name == "a") { A = sext(val, 56); return; }
        if (name == "b") { B = sext(val, 56); return; }
        if (name == "a0") { A = (A & ~(int64_t) MASK24) | (val & MASK24); return; }
        if (name == "b0") { B = (B & ~(int64_t) MASK24) | (val & MASK24); return; }
        if (name == "a1") { A = (A & ~((int64_t) MASK24 << 24)) | ((val & MASK24) << 24); return; }
        if (name == "b1") { B = (B & ~((int64_t) MASK24 << 24)) | ((val & MASK24) << 24); return; }
        if (name == "a2") { A = (A & ~((int64_t) 0xFF << 48)) | ((val & 0xFF) << 48); return; }
        if (name == "b2") { B = (B & ~((int64_t) 0xFF << 48)) | ((val & 0xFF) << 48); return; }
        if (name == "x0") { x0 = (uint32_t)(val & MASK24); return; }
        if (name == "x1") { x1 = (uint32_t)(val & MASK24); return; }
        if (name == "y0") { y0 = (uint32_t)(val & MASK24); return; }
        if (name == "y1") { y1 = (uint32_t)(val & MASK24); return; }
        if (is_rnm(name)) {
            if (name[0] == 'r') R[name[1] - '0'] = (uint32_t)(val & MASK24);
            else if (name[0] == 'n') N[name[1] - '0'] = (int32_t) sext(val & MASK24, 24);
            else M[name[1] - '0'] = (uint32_t)(val & MASK24);
            return;
        }
        if (name == "x") { x1 = (uint32_t)((val >> 24) & MASK24); x0 = (uint32_t)(val & MASK24); return; }
        if (name == "y") { y1 = (uint32_t)((val >> 24) & MASK24); y0 = (uint32_t)(val & MASK24); return; }
        fprintf(stderr, "set_reg %s\n", name.c_str()); abort();
    }

    // ---------------- EA ----------------
    bool parse_ea(const std::string& tok_in, Ea& out) const {
        std::string tok = tok_in;
        tok.erase(std::remove(tok.begin(), tok.end(), '?'), tok.end());
        static const std::regex sp_re("^([xyl]):(.+)$");
        std::smatch m;
        if (!std::regex_match(tok, m, sp_re)) return false;
        char space = m[1].str()[0];
        std::string inner = m[2].str();
        static const std::regex pre_re("^([+-])\\((r\\d)\\)$");
        std::smatch mm;
        if (std::regex_match(inner, mm, pre_re)) {
            out = Ea{space, mm[1].str() == "+" ? "pre+" : "pre-",
                     mm[2].str()[1] - '0', mm[1].str() == "+" ? 1l : -1l, ""};
            return true;
        }
        static const std::regex reg_re("^\\((r\\d)\\)((?:[+-].*)?)$");
        if (std::regex_match(inner, mm, reg_re)) {
            int rnum = mm[1].str()[1] - '0';
            std::string after = mm[2].str();
            if (after.empty()) { out = Ea{space, "reg", rnum, 0, ""}; return true; }
            if (after == "+") { out = Ea{space, "post+", rnum, 1, ""}; return true; }
            if (after == "-") { out = Ea{space, "post-", rnum, -1, ""}; return true; }
            static const std::regex pn_re("^\\+n(\\d)$"), mn_re("^-n(\\d)$");
            std::smatch m2;
            if (std::regex_match(after, m2, pn_re)) { out = Ea{space, "post+", rnum, 0, "n" + m2[1].str()}; return true; }
            if (std::regex_match(after, m2, mn_re)) { out = Ea{space, "post-", rnum, 0, "n" + m2[1].str()}; return true; }
            return false;
        }
        static const std::regex disp_re("^\\((r\\d)([+-].+)\\)$");
        if (std::regex_match(inner, mm, disp_re)) {
            out = Ea{space, "disp", mm[1].str()[1] - '0', 0, mm[2].str()};
            return true;
        }
        std::string v = inner;
        while (!v.empty() && (v[0] == ':' || v[0] == '?' || v[0] == '$' || v[0] == '<' || v[0] == '>')) v.erase(v.begin());
        out = Ea{space, "abs", -1, abs_val_raw(v), ""};
        return true;
    }

    static long abs_val_raw(const std::string& v_in) {
        std::string v = v_in;
        if (!v.empty() && v[0] == '+') v.erase(v.begin());
        bool neg = !v.empty() && v[0] == '-';
        if (neg) v.erase(v.begin());
        while (!v.empty() && (v[0] == '$' || v[0] == '#' || v[0] == '<' || v[0] == '>')) v.erase(v.begin());
        long val = v.empty() ? 0l : strtol(v.c_str(), nullptr, 16);
        return neg ? -val : val;
    }
    static long abs_val(const std::string& v) { return abs_val_raw(v); }

    // kind: 0 abs / 1 r / 2 upd(post) / 3 disp
    void eval_ea(const Ea& ea, int& kind, uint32_t& addr, int& rnum, long& delta) {
        if (ea.mode == "abs") {
            long v = ea.arg2.empty() ? ea.arg : abs_val_raw(ea.arg2);
            kind = 0; addr = (uint32_t) v; rnum = -1; delta = 0; return;
        }
        uint32_t r = (ea.rnum >= 0) ? R[ea.rnum] : 0;
        if (ea.mode == "reg") { kind = 1; addr = r; rnum = ea.rnum; delta = 0; return; }
        if (ea.mode == "pre+" || ea.mode == "pre-") {
            upd_r(ea.rnum, (long) ea.arg);
            kind = 1; addr = R[ea.rnum]; rnum = ea.rnum; delta = 0; return;
        }
        if (ea.mode == "post+" || ea.mode == "post-") {
            long d = ea.arg;
            if (ea.arg == 0 && !ea.arg2.empty()) d = N[ea.arg2[1] - '0'];
            kind = 2; addr = r; rnum = ea.rnum; delta = d; return;
        }
        if (ea.mode == "disp") {
            static const std::regex n_re("^[+-]n\\d$");
            long v;
            if (std::regex_match(ea.arg2, n_re)) {
                v = N[ea.arg2[2] - '0'];
                if (ea.arg2[0] == '-') v = -v;
            } else v = abs_val_raw(ea.arg2);
            kind = 3; addr = (uint32_t)((r + (uint32_t) v) & MASK24); rnum = ea.rnum; delta = 0; return;
        }
        fprintf(stderr, "eval_ea mode %s\n", ea.mode.c_str()); abort();
    }

    uint32_t move_imm(const std::string& src, const std::string& dst) {
        long v = abs_val(src);
        static const std::regex imm8_re("^#\\$([0-9a-fA-F]{1,2})$");
        std::smatch m;
        bool dst_acc = (dst == "a" || dst == "b");
        bool dst_rnm = is_rnm(dst);
        if (std::regex_match(src, m, imm8_re) && !dst_acc && !dst_rnm) {
            long imm = strtol(m[1].str().c_str(), nullptr, 16);
            if (imm >= 0x80) imm -= 0x100;
            v = (imm << 16) & (long) MASK24;
        }
        return (uint32_t)(v & 0xFFFFFFFFl);
    }

    void upd_r(int rnum, long delta) {
        if (rnum < 0) return;
        uint32_t m = M[rnum], r = R[rnum];
        if (m == MASK24) { R[rnum] = (uint32_t)((r + (uint32_t) delta) & MASK24); return; }
        uint32_t base = r & ~m;
        long off = (long)(r - base) + delta;
        if (off > (long) m) off -= (long) m + 1;
        else if (off < 0) off += (long) m + 1;
        R[rnum] = (uint32_t)((base + (uint32_t) off) & MASK24);
    }

    // ---------------- ALU helpers ----------------
    int64_t flags_from(int64_t res) {
        int64_t r56 = sext(res, 56);
        f.n = r56 < 0 ? 1 : 0;
        f.z = r56 == 0 ? 1 : 0;
        int a1msb = (int)((r56 >> 47) & 1);
        int a2 = (int)((r56 >> 48) & 0xFF);
        int sign_ext = a1msb ? 0xFF : 0x00;
        f.e = a2 != sign_ext ? 1 : 0;
        return r56;
    }
    int64_t alu_add(const std::string& dst, int64_t x, int64_t y, int carry = 0, bool store = true) {
        int64_t a = x + y + carry;
        int sx = sext(x, 56) < 0, sy = sext(y, 56) < 0, sr = sext(a, 56) < 0;
        f.v = (sx == sy && sr != sx) ? 1 : 0;
        f.c = ((a >> 55) != 0) ? 1 : 0;
        a = flags_from(a);
        if (store) set_reg(dst, a);
        return a;
    }
    int64_t alu_sub(const std::string& dst, int64_t x, int64_t y, bool store = true) {
        int64_t a = x - y;
        int sx = sext(x, 56) < 0, sy = sext(y, 56) < 0, sr = sext(a, 56) < 0;
        f.v = (sx != sy && sr != sx) ? 1 : 0;
        f.c = a < 0 ? 0 : 1;
        a = flags_from(a);
        if (store) set_reg(dst, a);
        return a;
    }
    void rnd_acc(const std::string& name) {
        int64_t v = get_reg(name);
        v = sext(v + 0x800000, 56);
        v = (v >> 24) << 24;
        set_reg(name, v);
        flags_from(v);
    }
    bool cond_ok(const std::string& c) const {
        if (c == "ifcc") return !f.c; if (c == "ifcs") return !!f.c;
        if (c == "ifec") return !f.e; if (c == "ifes") return !!f.e;
        if (c == "ifeq") return !!f.z; if (c == "ifne") return !f.z;
        if (c == "ifmi") return !!f.n; if (c == "ifpl") return !f.n;
        if (c == "ifge") return f.n == f.v; if (c == "iflt") return f.n != f.v;
        if (c == "ifgt") return (f.n == f.v) && !f.z;
        if (c == "ifle") return (f.n != f.v) || !!f.z;
        fprintf(stderr, "cond %s\n", c.c_str()); abort();
    }
    bool br_cond(const std::string& c) const {
        if (c == "cc") return !f.c; if (c == "cs") return !!f.c;
        if (c == "ec") return !f.e; if (c == "es") return !!f.e;
        if (c == "eq") return !!f.z; if (c == "ne") return !f.z;
        if (c == "mi") return !!f.n; if (c == "pl") return !f.n;
        if (c == "ge") return f.n == f.v; if (c == "lt") return f.n != f.v;
        if (c == "gt") return (f.n == f.v) && !f.z;
        if (c == "le") return (f.n != f.v) || !!f.z;
        if (c == "ra") return true; if (c == "nr") return false;
        fprintf(stderr, "br_cond %s\n", c.c_str()); abort();
    }

    // ---------------- parallel moves ----------------
    struct Latch { int64_t sval; std::string srckind; std::string src; std::string dst; };

    void commit_writes(std::vector<Latch>& latch, std::vector<Ea>& eas) {
        for (auto& L : latch) {
            const std::string& dst = L.dst;
            int64_t sval = L.sval;
            if ((dst == "ba" || dst == "ab") && L.srckind == "meml") {
                uint32_t xw = (uint32_t)((uint64_t) sval >> 24) & MASK24;
                uint32_t yw = (uint32_t)((uint64_t) sval) & MASK24;
                const char* first = (dst == "ba") ? "b" : "a";
                const char* second = (dst == "ba") ? "a" : "b";
                set_reg(first, sext(xw, 24) << 24);
                set_reg(second, sext(yw, 24) << 24);
                continue;
            }
            bool dst_rnm = is_rnm(dst);
            if ((dst == "a" || dst == "b" || dst == "a0" || dst == "a1" || dst == "a2" ||
                 dst == "b0" || dst == "b1" || dst == "b2" ||
                 dst == "x0" || dst == "x1" || dst == "y0" || dst == "y1" ||
                 dst == "x" || dst == "y") || dst_rnm) {
                if ((dst == "a" || dst == "b") && L.srckind == "meml") {
                    sval = sext((int64_t)((uint64_t) sval & MASK48), 48);
                } else if ((dst == "a" || dst == "b") && !is_acc_src(L.src)) {
                    sval = compose_acc((uint32_t)(sval & MASK24));
                } else if ((dst == "a" || dst == "b") && (L.src == "a0" || L.src == "b0")) {
                    int64_t acc = (dst == "a") ? A : B;
                    sval = (acc & ~((int64_t) MASK24 << 24)) |
                           (((int64_t) acc_part_for_dst(L.src, sval)) << 24);
                } else if ((dst == "a" || dst == "b") && (L.src == "a1" || L.src == "b1")) {
                    int64_t acc = (dst == "a") ? A : B;
                    sval = (acc & ~(int64_t) MASK48) |
                           (((int64_t) acc_part_for_dst(L.src, sval)) << 24);
                } else if (dst_rnm && is_acc_src(L.src)) {
                    sval = acc_part_for_dst(L.src, sval, false);
                } else if ((dst == "x0" || dst == "x1" || dst == "y0" || dst == "y1") &&
                           is_acc_src(L.src)) {
                    sval = acc_part_for_dst(L.src, sval);
                }
                set_reg(dst, sval);
            } else {
                Ea ea;
                if (!parse_ea(dst, ea)) { fprintf(stderr, "bad dst %s\n", dst.c_str()); abort(); }
                int kind; uint32_t addr; int rnum; long delta;
                eval_ea(ea, kind, addr, rnum, delta);
                eas.push_back(ea);
                if (ea.space == 'l') {
                    if (L.src == "ba" || L.src == "ab") {
                        wrL(addr, (uint64_t) sval);
                    } else if (is_acc_src(L.src)) {
                        if (L.src == "a" || L.src == "b") wrL(addr, (uint64_t) sval & MASK48);
                        else if (L.src == "a0" || L.src == "b0") wr('y', addr, (uint32_t)(sval & MASK24));
                        else if (L.src == "a1" || L.src == "b1") wr('x', addr, (uint32_t)((sval >> 24) & MASK24));
                        else wr('x', addr, acc_part_for_dst(L.src, sval));
                    } else {
                        wrL(addr, (uint64_t) sval);
                    }
                } else {
                    if (is_acc_src(L.src)) sval = acc_part_for_dst(L.src, sval);
                    wr(ea.space, addr, (uint32_t)(sval & MASK24));
                }
            }
        }
        for (auto& ea : eas) {
            if (ea.mode == "post+" || ea.mode == "post-") {
                long d = ea.arg;
                if (ea.arg == 0 && !ea.arg2.empty()) d = N[ea.arg2[1] - '0'];
                upd_r(ea.rnum, d);
            }
        }
    }

    // latch source value for one move token; returns false for bare (rN)± updates
    void latch_moves(const std::vector<std::string>& toks, std::vector<Latch>& latch,
                     std::vector<Ea>& eas, uint32_t pcx) {
        static const std::regex bare_re("^\\((r\\d)\\)([+-](n\\d)?)?$");
        static const std::regex reg_re("^(x0|x1|y0|y1|x|y|a|b|r\\d|n\\d|m\\d)$");
        static const std::regex acc_re("^[ab]\\d$");
        for (auto& tk : toks) {
            if (tk.rfind("if", 0) == 0) continue;   // condition gate token
            std::smatch mm;
            if (tk.find(',') == std::string::npos && std::regex_match(tk, mm, bare_re)) {
                int rnum = mm[1].str()[1] - '0';
                std::string upd = mm[2].str();
                if (upd.empty()) continue;
                if (upd == "+") eas.push_back(Ea{'x', "post+", rnum, 1, ""});
                else if (upd == "-") eas.push_back(Ea{'x', "post-", rnum, -1, ""});
                else if (upd.rfind("+n", 0) == 0) eas.push_back(Ea{'x', "post+", rnum, 0, upd.substr(1)});
                else if (upd.rfind("-n", 0) == 0) eas.push_back(Ea{'x', "post-", rnum, 0, upd.substr(1)});
                continue;
            }
            size_t cm = tk.find(',');
            if (cm == std::string::npos || tk.find(',', cm + 1) != std::string::npos) {
                fprintf(stderr, "bad move '%s' at %06X\n", tk.c_str(), pcx); abort();
            }
            std::string src = tk.substr(0, cm), dst = tk.substr(cm + 1);
            int64_t sval; std::string srckind;
            if (!src.empty() && src[0] == '#') {
                sval = move_imm(src, dst); srckind = "imm";
            } else if (src == "ba" || src == "ab") {
                sval = (((B >> 24) & MASK24) << 24) | ((A >> 24) & MASK24);
                srckind = "reg48";
            } else if (std::regex_match(src, reg_re) || std::regex_match(src, acc_re)) {
                sval = std::regex_match(src, acc_re) ? (int64_t) get_reg(src.substr(0, 1))
                                                     : (int64_t) get_reg(src);
                srckind = "reg";
            } else {
                Ea ea;
                if (!parse_ea(src, ea)) { fprintf(stderr, "bad src '%s' at %06X\n", src.c_str(), pcx); abort(); }
                int kind; uint32_t addr; int rnum; long delta;
                eval_ea(ea, kind, addr, rnum, delta);
                eas.push_back(ea);
                sval = (ea.space == 'l') ? (int64_t) rdL(addr) : (int64_t) rd(ea.space, addr);
                srckind = (ea.space == 'l') ? "meml" : "mem";
            }
            latch.push_back(Latch{sval, srckind, src, dst});
        }
    }

    void do_moves(const std::vector<std::string>& toks) {
        std::vector<Latch> latch; std::vector<Ea> eas;
        latch_moves(toks, latch, eas, pc);
        commit_writes(latch, eas);
    }

    uint32_t branch_target(const std::string& tok) {
        std::string t = tok;
        while (!t.empty() && (t[0] == '>' || t[0] == '$' || t[0] == '<')) t.erase(t.begin());
        auto it = labels.find(t);
        if (it != labels.end()) return it->second;
        static const std::regex ind_re("^\\((r\\d)\\)$");
        std::smatch m;
        if (std::regex_match(t, m, ind_re)) return R[m[1].str()[1] - '0'] & MASK24;
        return (uint32_t) abs_val_raw(t);
    }

    // ---------------- instruction dispatch ----------------
    static bool is_mac_op(const std::string& m) {
        return m == "mac" || m == "mpy" || m == "macr" || m == "mpyr" || m == "macsu" ||
               m == "mpysu" || m == "mpyuu" || m == "dmac" || m == "maci" || m == "mpyi" ||
               m == "mpyri" || m == "macri" || m == "macuu" || m == "macus" || m == "macsu2";
    }

    // returns jump target or ~0u for "continue"
    static const uint32_t NOJUMP = 0xFFFFFFFFu;
    uint32_t exec(const std::string& mnem, std::vector<std::string>& toks, uint32_t pcx) {
        // ---- DO ----
        if (mnem == "do") {
            std::string cnt_tok, end_tok;
            size_t cm = toks[0].find(',');
            if (cm != std::string::npos) { cnt_tok = toks[0].substr(0, cm); end_tok = toks[0].substr(cm + 1); }
            else { cnt_tok = toks[0]; end_tok = toks.back(); }
            while (!end_tok.empty() && end_tok[0] == '>') end_tok.erase(end_tok.begin());
            uint32_t end = labels.count(end_tok) ? labels[end_tok] : (uint32_t) abs_val_raw(end_tok);
            end -= 1;
            long cnt;
            if (!cnt_tok.empty() && cnt_tok[0] == '#') cnt = abs_val_raw(cnt_tok);
            else if ((cnt_tok.size() == 2 && cnt_tok[0] == 'n')) cnt = N[cnt_tok[1] - '0'] & (long) MASK24;
            else if (cnt_tok == "a1" || cnt_tok == "b1") cnt = (long)(get_reg(cnt_tok) & MASK24);
            else if (cnt_tok.size() == 2 && (cnt_tok[0] == 'x' || cnt_tok[0] == 'y')) cnt = (long)(get_reg(cnt_tok) & MASK24);
            else { fprintf(stderr, "do cnt %s\n", cnt_tok.c_str()); abort(); }
            if (cnt <= 0) return end + 1;
            do_stack.push_back((int64_t) cnt);
            do_stack.push_back((int64_t)(pcx + 2));
            do_stack.push_back((int64_t) end);
            return NOJUMP;
        }
        // ---- REP ----
        if (mnem == "rep") {
            const std::string& cnt_tok = toks[0];
            long cnt;
            if (!cnt_tok.empty() && cnt_tok[0] == '#') cnt = abs_val_raw(cnt_tok);
            else if (cnt_tok.size() == 2 && cnt_tok[0] == 'n') cnt = N[cnt_tok[1] - '0'] & (long) MASK24;
            else if (cnt_tok == "a1" || cnt_tok == "b1") cnt = (long)(get_reg(cnt_tok) & MASK24);
            else { fprintf(stderr, "rep cnt %s\n", cnt_tok.c_str()); abort(); }
            if (cnt <= 0) return NOJUMP;
            rep_count = cnt;
            return NOJUMP;
        }
        // ---- bit ops ----
        if (mnem == "btst" || mnem == "bset" || mnem == "bclr" || mnem == "bchg") {
            std::string bit_tok, loc;
            size_t cm = toks[0].find(',');
            if (cm != std::string::npos) { bit_tok = toks[0].substr(0, cm); loc = toks[0].substr(cm + 1); }
            else { bit_tok = toks[0]; loc = toks[1]; }
            int bit = (int) abs_val_raw(bit_tok);
            if (loc == "a" || loc == "b") {
                int64_t acc = get_reg(loc);
                uint32_t main = (uint32_t)((acc >> 24) & MASK24);
                int bitval = (main >> bit) & 1;
                f.c = bitval;
                if (mnem == "bset") set_reg(loc, (acc & ~(((int64_t) 1 << bit) << 24)) | (((int64_t) 1 << bit) << 24));
                else if (mnem == "bclr") set_reg(loc, acc & ~(((int64_t) 1 << bit) << 24));
                else if (mnem == "bchg") set_reg(loc, acc ^ (((int64_t) 1 << bit) << 24));
                return NOJUMP;
            }
            static const std::regex accs_re("^[ab]\\d?$");
            if (std::regex_match(loc, accs_re) || loc == "sr" || loc == "ccr" || is_rnm(loc) ||
                loc == "x0" || loc == "x1" || loc == "y0" || loc == "y1") {
                uint32_t val = get_reg(loc);
                int bitval = (val >> bit) & 1;
                if (mnem == "btst") f.c = bitval;
                else if (mnem == "bset") {
                    f.c = bitval;
                    if (loc == "sr") { sr_int |= (1u << bit);
                        if (bit == 0) f.c = 1; else if (bit == 1) f.v = 1;
                        else if (bit == 2) f.z = 1; else if (bit == 3) f.n = 1; }
                    else set_reg(loc, (int64_t)(val | (1u << bit)));
                } else if (mnem == "bclr") {
                    f.c = bitval;
                    if (loc == "sr") { sr_int &= ~(1u << bit);
                        if (bit == 0) f.c = 0; else if (bit == 1) f.v = 0;
                        else if (bit == 2) f.z = 0; else if (bit == 3) f.n = 0; }
                    else set_reg(loc, (int64_t)(val & ~(1u << bit)));
                } else { f.c = bitval; set_reg(loc, (int64_t)(val ^ (1u << bit))); }
                return NOJUMP;
            }
            Ea ea;
            if (!parse_ea(loc, ea)) { fprintf(stderr, "bitop loc %s\n", loc.c_str()); abort(); }
            int kind; uint32_t addr; int rnum; long delta;
            eval_ea(ea, kind, addr, rnum, delta);
            uint32_t val = rd(ea.space, addr);
            int bitval = (val >> bit) & 1;
            f.c = bitval;
            if (mnem == "bset") wr(ea.space, addr, val | (1u << bit));
            else if (mnem == "bclr") wr(ea.space, addr, val & ~(1u << bit));
            else if (mnem == "bchg") wr(ea.space, addr, val ^ (1u << bit));
            return NOJUMP;
        }
        // ---- branches / jumps / calls ----
        if (mnem == "jmp" || mnem == "bra") return branch_target(toks[0]);
        if (mnem == "jsr") {
            uint32_t tgt = branch_target(toks[0]);
            ret_stack.push_back(next_addr.count(pcx) ? next_addr[pcx] : pcx + 1);
            return tgt;
        }
        if (mnem == "bsr") {
            ret_stack.push_back(next_addr.count(pcx) ? next_addr[pcx] : pcx + 1);
            return branch_target(toks[0]);
        }
        if (mnem == "rts") { uint32_t t = ret_stack.back(); ret_stack.pop_back(); return t; }
        if (mnem.size() >= 2 && mnem[0] == 'b' && mnem != "bset" && mnem != "bclr" &&
            mnem != "btst" && mnem != "brset" && mnem != "brclr" && mnem != "bchg" &&
            toks.size() == 1) {
            std::string cond = mnem.substr(1);
            if (br_cond(cond)) return branch_target(toks[0]);
            return NOJUMP;
        }
        if (mnem == "jset" || mnem == "jclr" || mnem == "brset" || mnem == "brclr" ||
            mnem == "jsset" || mnem == "jsclr") {
            std::string bt, loc, tgt;
            if (toks.size() == 1) {
                size_t c1 = toks[0].find(',');
                size_t c2 = toks[0].find(',', c1 + 1);
                bt = toks[0].substr(0, c1);
                loc = toks[0].substr(c1 + 1, c2 - c1 - 1);
                tgt = toks[0].substr(c2 + 1);
            } else { bt = toks[0]; loc = toks[1]; tgt = toks[2]; }
            int bit = (int) abs_val_raw(bt);
            uint32_t val;
            if (loc == "a" || loc == "b") val = (uint32_t)((get_reg(loc) >> 24) & MASK24);
            else val = get_reg(loc);
            int bitval = (val >> bit) & 1;
            bool taken = (mnem.substr(mnem.size() - 3) == "set") ? (bitval == 1) : (bitval == 0);
            if ((mnem == "brset" || mnem == "brclr") && taken) return branch_target(tgt);
            if ((mnem == "jset" || mnem == "jclr" || mnem == "jsset" || mnem == "jsclr") && taken) {
                if (mnem.rfind("js", 0) == 0) ret_stack.push_back(pcx + 1);
                return branch_target(tgt);
            }
            return NOJUMP;
        }
        // ---- ANDI/ORI ccr ----
        if (mnem == "andi" || mnem == "ori") {
            size_t cm = toks[0].find(',');
            long v = abs_val_raw(toks[0].substr(0, cm));
            int cur = f.c | (f.v << 1) | (f.z << 2) | (f.n << 3);
            cur = (mnem == "andi") ? (cur & (int) v) : (cur | (int) v);
            f.c = cur & 1; f.v = (cur >> 1) & 1; f.z = (cur >> 2) & 1; f.n = (cur >> 3) & 1;
            return NOJUMP;
        }
        if (mnem == "move") { do_moves(toks); return NOJUMP; }
        // ---- LUA ----
        if (mnem == "lua") {
            std::string src_tok, dst;
            size_t cm = toks[0].find(',');
            if (cm != std::string::npos) { src_tok = toks[0].substr(0, cm); dst = toks[0].substr(cm + 1); }
            else { src_tok = toks[0]; dst = toks[1]; }
            static const std::regex re1("^\\((r\\d)\\)(\\+|-)?(n\\d)?$");
            std::smatch m;
            long delta = 0; int rnum;
            if (std::regex_match(src_tok, m, re1)) {
                rnum = m[1].str()[1] - '0';
                std::string op = m[2].str(), nn = m[3].str();
                if (op == "+") delta = nn.empty() ? 1 : N[nn[1] - '0'];
                else if (op == "-") delta = nn.empty() ? -1 : -N[nn[1] - '0'];
            } else {
                static const std::regex re2("^\\((r\\d)([+-]\\$?\\w+)\\)$");
                if (!std::regex_match(src_tok, m, re2)) { fprintf(stderr, "lua %s\n", src_tok.c_str()); abort(); }
                rnum = m[1].str()[1] - '0';
                delta = abs_val_raw(m[2].str());
            }
            set_reg(dst, (int64_t)((R[rnum] + (uint32_t) delta) & MASK24));
            return NOJUMP;
        }
        if (mnem == "nop" || mnem == "pflush" || mnem == "debug" || mnem == "wait" || mnem == "stop")
            return NOJUMP;
        // ---- RND ----
        if (mnem == "rnd") {
            std::string dst = "a";
            if (!toks.empty()) {
                size_t cm = toks[0].find(',');
                dst = (cm == std::string::npos) ? toks[0] : toks[0].substr(0, cm);
            }
            rnd_acc(dst);
            if (toks.size() > 1) do_moves(std::vector<std::string>(toks.begin() + 1, toks.end()));
            return NOJUMP;
        }
        // ---- DIV ----
        if (mnem == "div") {
            std::string s_tok, d_tok;
            size_t cm = toks[0].find(',');
            if (cm != std::string::npos) { s_tok = toks[0].substr(0, cm); d_tok = toks[0].substr(cm + 1); }
            else { s_tok = toks[0]; d_tok = toks[1]; }
            int32_t s = sgn24(get_reg(s_tok));
            int64_t acc = get_reg(d_tok);
            if ((s & (int32_t) MASK24) == 0) { set_reg(d_tok, (int64_t)(MASK48 & (int64_t) MASK48)); return NOJUMP; }
            uint32_t d1 = (uint32_t)((acc >> 24) & MASK24), d0 = (uint32_t)(acc & MASK24);
            d1 = ((d1 << 1) | (d0 >> 23)) & MASK24;
            d0 = (d0 << 1) & MASK24;
            int32_t s_signed = (int32_t) sext((uint32_t)(s & (int32_t) MASK24), 24);
            int32_t d1_signed = (int32_t) sext(d1, 24);
            if ((d1_signed < 0) == (s_signed < 0)) d1 = (uint32_t)((d1 - (s & (int32_t) MASK24)) & (int32_t) MASK24);
            else d1 = (uint32_t)((d1 + (s & (int32_t) MASK24)) & (int32_t) MASK24);
            int qbit = ((int32_t) sext(d1, 24) < 0) == (s_signed < 0) ? 1 : 0;
            d0 |= (uint32_t) qbit;
            set_reg(d_tok, ((int64_t) d1 << 24) | d0);
            f.c = qbit ^ 1;
            return NOJUMP;
        }
        // ---- ALU ----
        if (is_mac_op(mnem) || mnem == "add" || mnem == "sub" || mnem == "subr" ||
            mnem == "addr" || mnem == "addl" || mnem == "subl" || mnem == "addc" ||
            mnem == "sbc" || mnem == "tfr" || mnem == "cmp" || mnem == "cmpm" ||
            mnem == "tst" || mnem == "neg" || mnem == "negc" || mnem == "abs" ||
            mnem == "clr" || mnem == "not" || mnem == "and" || mnem == "or" ||
            mnem == "eor" || mnem == "asl" || mnem == "asr" || mnem == "lsl" ||
            mnem == "lsr" || mnem == "rol" || mnem == "ror" || mnem == "tge" ||
            mnem == "tgt" || mnem == "tle" || mnem == "tlt" || mnem == "teq" ||
            mnem == "tne" || mnem == "tpl" || mnem == "tmi" || mnem == "clb" ||
            mnem == "normf" || mnem == "max" || mnem == "maxm" || mnem == "insert")
            return exec_alu(mnem, toks, pcx);

        fprintf(stderr, "unimplemented %s at %06X\n", mnem.c_str(), pcx); abort();
    }

    // ---------------- ALU with parallel moves ----------------
    uint32_t exec_alu(const std::string& mnem, std::vector<std::string>& toks, uint32_t pcx) {
        std::vector<std::string> T = toks;
        std::string cond;
        if (!T.empty() && T.back().size() > 2 && T.back().rfind("if", 0) == 0) {
            cond = T.back(); T.pop_back();
        }
        std::vector<std::string> alu_toks, move_toks;
        if (is_mac_op(mnem)) {
            int commas = 0;
            while (!T.empty() && commas < 2) { std::string tk = T.front(); T.erase(T.begin());
                commas += (int) std::count(tk.begin(), tk.end(), ','); alu_toks.push_back(tk); }
        } else if (mnem == "add" || mnem == "sub" || mnem == "addc" || mnem == "sbc" ||
                   mnem == "subr" || mnem == "addr" || mnem == "addl" || mnem == "subl" ||
                   mnem == "tfr" || mnem == "cmp" || mnem == "cmpm" || mnem == "and" ||
                   mnem == "or" || mnem == "eor" || mnem == "tge" || mnem == "tgt" ||
                   mnem == "tle" || mnem == "tlt" || mnem == "teq" || mnem == "tne" ||
                   mnem == "tpl" || mnem == "tmi" || mnem == "clb" || mnem == "normf" ||
                   mnem == "max" || mnem == "maxm" || mnem == "insert") {
            int commas = 0;
            while (!T.empty() && commas < 1) { std::string tk = T.front(); T.erase(T.begin());
                commas += (int) std::count(tk.begin(), tk.end(), ','); alu_toks.push_back(tk); }
        } else if (mnem == "asl" || mnem == "asr" || mnem == "lsl" || mnem == "lsr" ||
                   mnem == "rol" || mnem == "ror") {
            alu_toks.push_back(T.front()); T.erase(T.begin());
        } else if (mnem == "neg" || mnem == "negc" || mnem == "abs" || mnem == "clr" ||
                   mnem == "not" || mnem == "tst") {
            if (!T.empty()) { alu_toks.push_back(T.front()); T.erase(T.begin()); }
        }
        move_toks = T;
        std::vector<Latch> latch; std::vector<Ea> eas;
        latch_moves(move_toks, latch, eas, pcx);
        bool run_alu = cond.empty() ? true : cond_ok(cond);
        if (run_alu) alu_exec(mnem, alu_toks, pcx);
        commit_writes(latch, eas);
        return NOJUMP;
    }

    void alu_exec(const std::string& mnem, std::vector<std::string>& toks, uint32_t pcx) {
        if (is_mac_op(mnem)) {
            std::vector<std::string> T = toks;
            std::string sign2;
            if (!T.empty() && (T[0] == "ss" || T[0] == "su" || T[0] == "us" || T[0] == "uu")) {
                sign2 = T[0]; T.erase(T.begin());
            }
            bool neg = false;
            std::string core = T[0];
            if (!core.empty() && core[0] == '-' && core.find(',') != std::string::npos) {
                core = core.substr(1); neg = true;
            }
            std::vector<std::string> parts;
            {
                size_t s = 0;
                while (true) {
                    size_t c = core.find(',', s);
                    parts.push_back(core.substr(s, c == std::string::npos ? std::string::npos : c - s));
                    if (c == std::string::npos) break; s = c + 1;
                }
            }
            std::string s1, s2, dst;
            if (parts.size() == 3) { s1 = parts[0]; s2 = parts[1]; dst = parts[2]; }
            else if (parts.size() == 2 && T.size() > 1) { s1 = parts[0]; s2 = parts[1]; dst = T[1]; }
            else { fprintf(stderr, "mac ops at %06X\n", pcx); abort(); }
            uint32_t v1, v2;
            if (mnem.size() && mnem.back() == 'i' && (mnem == "mpyi" || mnem == "maci" || mnem == "mpyri" || mnem == "macri")) {
                v1 = (uint32_t)(abs_val_raw(s1) & (long) MASK24);
                v2 = get_reg(s2);
            } else { v1 = get_reg(s1); v2 = get_reg(s2); }
            if (neg) v1 = (uint32_t)(-(int32_t) sgn24(v1));
            int64_t prod;
            if (mnem == "mpyuu" || mnem == "macuu" || sign2 == "uu") {
                prod = (int64_t)((uint64_t)(v1 & MASK24) * (uint64_t)(v2 & MASK24));
            } else if ((mnem == "macsu" || mnem == "mpysu") && sign2.empty() || sign2 == "su") {
                uint32_t a1 = v1 & MASK24;
                int64_t s1v = (a1 & 0x800000) ? (int64_t) a1 - (1ll << 24) : (int64_t) a1;
                prod = s1v * (int64_t)(v2 & MASK24);
            } else if (sign2 == "us") {
                uint32_t a2 = v2 & MASK24;
                int64_t s2v = (a2 & 0x800000) ? (int64_t) a2 - (1ll << 24) : (int64_t) a2;
                prod = (int64_t)(v1 & MASK24) * s2v;
            } else {
                uint32_t a1 = v1 & MASK24, a2 = v2 & MASK24;
                int64_t s1v = (a1 & 0x800000) ? (int64_t) a1 - (1ll << 24) : (int64_t) a1;
                int64_t s2v = (a2 & 0x800000) ? (int64_t) a2 - (1ll << 24) : (int64_t) a2;
                prod = s1v * s2v;
            }
            prod <<= 1;
            int64_t acc = get_reg(dst);
            int64_t res;
            if (mnem == "mac" || mnem == "macr" || mnem == "maci" || mnem == "macsu" ||
                mnem == "dmac" || mnem == "macri" || mnem == "macuu") res = acc + prod;
            else res = prod;
            int64_t res56 = flags_from(res);
            f.v = (-(1ll << 47) <= res56 && res56 < (1ll << 47)) ? 0 : 1;
            set_reg(dst, res56);
            if (mnem == "macr" || mnem == "mpyr" || mnem == "mpyri" || mnem == "macri")
                rnd_acc(dst);
            return;
        }
        // non-MAC ALU
        if (mnem == "add" || mnem == "sub" || mnem == "subr" || mnem == "addr" ||
            mnem == "addc" || mnem == "sbc" || mnem == "addl" || mnem == "subl" ||
            mnem == "tfr" || mnem == "cmp" || mnem == "cmpm" || mnem == "and" ||
            mnem == "or" || mnem == "eor" || mnem == "tge" || mnem == "tgt" ||
            mnem == "tle" || mnem == "tlt" || mnem == "teq" || mnem == "tne" ||
            mnem == "tpl" || mnem == "tmi" || mnem == "clb" || mnem == "normf" ||
            mnem == "max" || mnem == "maxm" || mnem == "insert") {
            std::string core = toks[0];
            std::vector<std::string> parts;
            {
                size_t s = 0;
                while (true) {
                    size_t c = core.find(',', s);
                    parts.push_back(core.substr(s, c == std::string::npos ? std::string::npos : c - s));
                    if (c == std::string::npos) break; s = c + 1;
                }
            }
            std::string s, d;
            if (parts.size() == 2) { s = parts[0]; d = parts[1]; }
            else if (parts.size() == 1 && toks.size() > 1) { s = parts[0]; d = toks[1]; }
            else { s = ""; d = parts[0]; }
            if (mnem == "add") alu_add(d, get_reg(d), alu_src(s));
            else if (mnem == "addc") alu_add(d, get_reg(d), alu_src(s), f.c);
            else if (mnem == "sub") alu_sub(d, get_reg(d), alu_src(s));
            else if (mnem == "subr") alu_sub(d, alu_src(s), get_reg(d));
            else if (mnem == "addr") alu_add(d, alu_src(s), get_reg(d));
            else if (mnem == "sbc") alu_sub(d, get_reg(d), alu_src(s) + ((int64_t)(f.c ? (1ll << 24) : 0)));
            else if (mnem == "tfr") set_reg(d, alu_src(s));
            else if (mnem == "cmp" || mnem == "cmpm") alu_sub(d, get_reg(d), alu_src(s), false);
            else if (mnem == "and" || mnem == "or" || mnem == "eor") {
                uint32_t a1 = (uint32_t)((get_reg(d) >> 24) & MASK24);
                uint32_t b = (uint32_t)((s.empty() ? 0 : (s[0] == '#' ? (uint32_t)(abs_val_raw(s) & (long) MASK24) : get_reg(s))) & MASK24);
                uint32_t r = (mnem == "and") ? (a1 & b) : (mnem == "or") ? (a1 | b) : (a1 ^ b);
                uint32_t b0 = (uint32_t)(get_reg(d) & MASK24);
                int64_t r56 = ((int64_t) r << 24) | b0;
                if (r & 0x800000) r56 |= (int64_t) 0xFF << 48;
                f.n = (r & 0x800000) ? 1 : 0;
                f.z = (r == 0) ? 1 : 0;
                set_reg(d, r56);
            } else if (mnem == "tge" || mnem == "tgt" || mnem == "tle" || mnem == "tlt" ||
                       mnem == "teq" || mnem == "tne" || mnem == "tpl" || mnem == "tmi") {
                static const char* mp[8] = {"ifge","ifgt","ifle","iflt","ifeq","ifne","ifpl","ifmi"};
                const char* cc = nullptr;
                for (int i = 0; i < 8; ++i) { std::string t1 = "t"; t1 += mp[i][2] + (mp[i] + 2 - mp[i] - 2); }
                std::string map;
                if (mnem == "tge") map = "ifge"; else if (mnem == "tgt") map = "ifgt";
                else if (mnem == "tle") map = "ifle"; else if (mnem == "tlt") map = "iflt";
                else if (mnem == "teq") map = "ifeq"; else if (mnem == "tne") map = "ifne";
                else if (mnem == "tpl") map = "ifpl"; else map = "ifmi";
                std::string cs = map.substr(2);
                if (br_cond(cs)) set_reg(d, alu_src(s));
            } else if (mnem == "max" || mnem == "maxm") {
                int64_t sv = sext((int64_t) get_reg(s), 56), dv = sext((int64_t) get_reg(d), 56);
                if (sv > dv) set_reg(d, get_reg(s)); else set_reg(s, get_reg(d));
                int64_t r = get_reg(d);
                f.n = sext(r, 56) < 0 ? 1 : 0;
                f.z = ((uint64_t) r & MASK48) == 0 ? 1 : 0;
                f.c = 0; f.v = 0;
            } else if (mnem == "clb") {
                uint32_t v = get_reg(s);
                if (s == "a" || s == "b") v = (uint32_t)((v >> 24) & MASK24); else v &= MASK24;
                int sign = (v >> 23) & 1, cnt = 1;
                for (int bit = 22; bit >= 0; --bit) {
                    if (((v >> bit) & 1) == sign) ++cnt; else break;
                }
                set_reg(d, (uint32_t)(cnt & (int) MASK24));
                f.n = 0; f.z = cnt == 0 ? 1 : 0;
            } else if (mnem == "normf") {
                uint32_t sv = get_reg(s);
                if (s == "a" || s == "b") sv = (uint32_t)((sv >> 24) & MASK24); else sv &= MASK24;
                long sh = (long) sext(sv, 24) - 1;
                int64_t acc = get_reg(d);
                int64_t r = (sh >= 0) ? (acc << sh) : (acc >> (-sh));
                r = sext(r, 56);
                set_reg(d, r);
                f.n = r < 0 ? 1 : 0;
                f.z = ((uint64_t) r & MASK56) == 0 ? 1 : 0;
            }
            return;
        }
        if (mnem == "asl" || mnem == "asr" || mnem == "lsl" || mnem == "lsr" ||
            mnem == "rol" || mnem == "ror") {
            std::vector<std::string> fields;
            for (auto& tk : toks) {
                size_t s = 0;
                while (true) {
                    size_t c = tk.find(',', s);
                    fields.push_back(tk.substr(s, c == std::string::npos ? std::string::npos : c - s));
                    if (c == std::string::npos) break; s = c + 1;
                }
            }
            int cnt = -1;
            size_t start = 0;
            if (!fields.empty() && !fields[0].empty() && fields[0][0] == '#') {
                cnt = (int)(abs_val_raw(fields[0]) & 63);
                start = 1;
            }
            std::vector<std::string> rest(fields.begin() + start, fields.end());
            std::string s, d;
            if (rest.size() == 3) {
                cnt = (int)(get_reg(rest[0]) & 63);
                s = rest[1]; d = rest[2];
            } else if (rest.size() == 2) {
                s = rest[0]; d = rest[1];
                if (cnt < 0) cnt = 1;
            } else if (rest.size() == 1) {
                s = d = rest[0];
            } else { fprintf(stderr, "shift fields at %06X\n", pcx); abort(); }
            if (cnt < 0) cnt = 1;
            int64_t v = get_reg(s);
            int64_t r;
            if (mnem == "asl") {
                r = v << cnt;
                f.c = (cnt > 0 && cnt <= 56) ? (int)((v >> (56 - cnt)) & 1) : 0;
            } else if (mnem == "asr") {
                r = v >> cnt;
                f.c = cnt ? (int)((v >> (cnt - 1)) & 1) : 0;
            } else if (mnem == "lsl") {
                r = (int64_t)(((uint64_t) v << cnt) & MASK56);
                f.c = (cnt > 0 && cnt <= 56) ? (int)((v >> (56 - cnt)) & 1) : 0;
            } else if (mnem == "lsr") {
                r = (int64_t)(((uint64_t) v & MASK56) >> cnt);
                f.c = cnt ? (int)((v >> (cnt - 1)) & 1) : 0;
            } else { // rol/ror
                uint32_t a1 = (uint32_t)((v >> 24) & MASK24);
                uint32_t b0 = (uint32_t)(v & MASK24);
                int c;
                if (mnem == "rol") { c = (a1 >> 23) & 1; a1 = ((a1 << 1) | c) & MASK24; }
                else { c = a1 & 1; a1 = (a1 >> 1) | ((uint32_t) c << 23); }
                f.c = c;
                int64_t rr = ((int64_t) a1 << 24) | b0;
                if (a1 & 0x800000) rr |= (int64_t) 0xFF << 48;
                rr = flags_from(rr);
                set_reg(d, rr);
                return;
            }
            r = flags_from(r);
            set_reg(d, r);
            return;
        }
        if (mnem == "neg" || mnem == "negc" || mnem == "abs" || mnem == "clr" ||
            mnem == "not" || mnem == "tst") {
            std::string d = toks.empty() ? "a" : toks[0];
            int64_t v;
            if (mnem == "clr") { v = 0; f.u = 0; }
            else if (mnem == "neg") {
                v = flags_from(-get_reg(d));
                f.v = sext(v, 56) == -(1ll << 55) ? 1 : 0;
                set_reg(d, v);
                return;
            } else if (mnem == "abs") v = get_reg(d) < 0 ? -get_reg(d) : get_reg(d);
            else if (mnem == "not") v = ~get_reg(d);
            else if (mnem == "tst") { flags_from(get_reg(d)); return; }
            else v = 0;
            v = flags_from(v);
            set_reg(d, v);
            return;
        }
        fprintf(stderr, "alu_exec fallthrough %s at %06X\n", mnem.c_str(), pcx); abort();
    }

    // ---------------- step / run ----------------
    void step() {
        uint32_t pcx = pc;
        auto it = prog.find(pcx);
        if (it == prog.end()) {
            fprintf(stderr, "no instruction at %06X (after %u steps)\n", pcx, steps);
            abort();
        }
        const Ins& ins = it->second;
        ++steps;
        if (trace_on) { trace_ring.push_back(pcx); if (trace_ring.size() > 64) trace_ring.erase(trace_ring.begin()); }
        if (steps > max_steps) { fprintf(stderr, "step limit\n"); abort(); }
        uint32_t next_pc = next_addr.count(pcx) ? next_addr[pcx] : pcx + 1;
        uint32_t handled = exec(ins.mnem, const_cast<std::vector<std::string>&>(ins.toks), pcx);
        pc = (handled == NOJUMP) ? next_pc : handled;
        // REP management (mirrors reference exactly)
        if (rep_valid) {
            if (pcx == rep_after) {
                if (rep_count > 1) { --rep_count; pc = pcx; }
                else rep_valid = false;
            } else {
                rep_valid = false;
            }
        }
        if (ins.mnem == "rep") { rep_after = next_pc; rep_valid = true; }
        // DO-loop management
        for (size_t i = 0; i < do_stack.size(); i += 3) {
            int64_t cnt = do_stack[i], body = do_stack[i + 1], end = do_stack[i + 2];
            if ((int64_t) pcx <= end && end < (int64_t) next_pc) {
                do_stack[i] = cnt - 1;
                if (cnt - 1 > 0) pc = (uint32_t) body;
                else do_stack.erase(do_stack.begin() + i, do_stack.begin() + i + 3);
                break;
            }
        }
    }

    uint64_t run(uint32_t start, uint32_t end, void (*hook)(uint32_t, Core&) , uint64_t limit) {
        pc = start;
        uint64_t n = 0;
        while (n < limit) {
            uint32_t pcx = pc;
            if (end != 0 && pcx == end) return n;
            if (!prog.count(pcx)) {
                fprintf(stderr, "no instruction at %06X (after %u steps)\n", pcx, steps);
                abort();
            }
            if (hook) hook(pcx, *this);
            step();
            ++n;
        }
        return n;
    }
    uint64_t run(uint32_t start, uint32_t end) {
        return run(start, end, (void (*)(uint32_t, Core&)) nullptr, max_steps);
    }

    // PART3_MARKER (test helpers below)
};

} // namespace mnmdsp
