// =============================================================================
// MnmFilterStage2.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Стадия 2 трекового фильтра: env-управляемый резонатор (ручки ATK/DEC/
// BOFS/WOFS страницы FILT), P:$0A5D-$0AD0 ядра DSP1.
//
// Пословная транскрипция verified-модели exp18_model.py (384/384 слов-точных
// блока на векторах эмулятора OS 1.32: L1/L2/L3/DP, 12 конфигураций x 8 кадров):
//   L1 $0A5D-$0A82 : вращение, угол c1 = X:$144AC7[BOFS]
//                    входы X:$30-$3F (a) / X:$20-$2F (b), стейт X/Y:(P+$D4),
//                    wrap X/Y:(P+$D3), выходы Y:$62-$81 (чересстрочно)
//   L2 $0A84-$0A9B : вращение, угол c2 = X:$144AC7[WOFS+BOFS]
//                    входы Y:$62-$81, стейт X/Y:(P+$D5), выходы Y:$20-$3F
//   L3 $0A9D-$0AB5 : вращение, угол c3 = X:$144B48[состояние делеЯ>>17]
//                    (= $144AC7+129), входы X:$00-$0F / X:$10-$1F (аудиошина),
//                    стейт X/Y:(P+$CC), выходы Y:$62-$81 (поверх L1)
//   DP $0AB7-$0AD0 : depth = (2|ATK-0.5|)^2; KILL: фаза AMP-env (X:P+$D8)==4
//                    -> DEC=0; a-банк X:$51-$61, b-банк X:$40-$50
//                      a_i = 2*(DEC*Y:$20+2i) + depth*Y:$62+2i
//                      b_i = 2*(DEC*Y:$21+2i) + depth*Y:$63+2i
//
// Арифметика: DSP56300, аккумулятор 56 бит (A2:A1:A0), B1 = биты [47..24],
// MAC со сдвигом <<1, 56-битное переполнение по модулю.
// Хвост $0AD1-$0B4C (DIV-интегратор, гребёнка тапов func_000397) — отдельный
// блок (итерация 22, exp22_model.py), сюда не входит.
// =============================================================================
#ifndef MNM_FILTER_STAGE2_HPP
#define MNM_FILTER_STAGE2_HPP

#include "MnmFilterCurves.h"
#include <cstdint>

namespace mnmfm {

static const uint32_t kM24 = 0xFFFFFFu;
static const int64_t  kM56 = (int64_t)((1ull << 56) - 1);
static const int64_t  kNeg56 = (int64_t)(1ull << 55); // бит знака 56-бит acc

inline int32_t s24(uint32_t v) {
    v &= kM24;
    return (int32_t)(v & 0x800000u ? (int64_t)v - (1ll << 24) : (int64_t)v);
}
inline int64_t s56(int64_t a) {
    a &= kM56;
    if (a & kNeg56) a -= (1ll << 56);
    return a;
}
inline int64_t acc24(uint32_t w) { return (int64_t)s24(w) << 24; }   // A1-загрузка
inline uint32_t B1(int64_t a) { return (uint32_t)((a >> 24) & kM24); }
inline int64_t smac(int64_t a, uint32_t x, uint32_t y) {
    int64_t v = (int64_t)s24(x) * (int64_t)s24(y);
    return s56(a + (v << 1));
}
inline int64_t neg56(int64_t a) { return a == 0 ? 0 : s56(-a); }

// DSP56300 Data Limiter (FM §5.4.1.2): пересылка ПОЛНОГО аккумулятора (a/b)
// в память или x0/x1/y0/y1 сатурируется к $7FFFFF/$800000, если 56-бит
// значение выходит за диапазон Q23. Части (a0/a1/a2/b0/b1/b2) — без лимитера.
inline uint32_t sat24(int64_t a) {
    int64_t v = s56(a);
    if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFF;   // max в диапазоне
    if (v < -0x00800000000000ll) return 0x800000;  // −1.0 допустим
    return (uint32_t)((v >> 24) & kM24);
}

// 24-битное слово с инверсией (для операндов модели вида (-x0)&M24)
inline uint32_t neg24(uint32_t x) { return (-x) & kM24; }

class MnmFilterStage2 {
public:
    // персистентный стейт (слова страницы голоса, X/Y-пары)
    uint32_t d3y = 0, d3x = 0;   // wrap L1        (Y/X:P+$D3)
    uint32_t d4y = 0, d4x = 0;   // стейт L1       (Y/X:P+$D4)
    uint32_t d5y = 0, d5x = 0;   // стейт L2       (Y/X:P+$D5)
    uint32_t ccy = 0, ccx = 0;   // стейт L3       (Y/X:P+$CC)

    // входы одного кадра (снапшот на $0A5D)
    struct In {
        uint32_t x00[16];        // аудиошина a-входы L3 (X:$00-$0F)
        uint32_t x10[16];        // аудиошина b-входы L3 (X:$10-$1F)
        uint32_t x20[16];        // банк b-входы L1      (X:$20-$2F)
        uint32_t x30[16];        // банк a-входы L1      (X:$30-$3F)
        uint32_t atk, dec;       // Y:P+$14 / $15 (слова <<16)
        uint32_t bofs, wofs;     // Y:P+$16 / $17
        uint32_t phase;          // X:P+$D8, фаза AMP-env (0..5)
        uint32_t cfy, cfx;       // Y/X:P+$CF — 48-бит состояние делеЯ (для угла L3)
    };
    // выходы одного кадра
    struct Out {
        uint32_t Y20[32];        // L2: Y:$20-$3F (a=чётные, b=нечётные)
        uint32_t Y62[32];        // L1->L3: Y:$62-$81 (финал = L3)
        uint32_t bankA[17];      // X:$51-$61 (a-коэффициенты, +X:$61 хвост)
        uint32_t bankB[17];      // X:$40-$50 (b-коэффициенты, +X:$50 хвост)
    };

    void frame(const In& in, Out& out) {
        runL1(in, out);
        runL2(in, out);
        runL3(in, out);
        runDP(in, out);
    }

    // NOTE: runL1/runL2/runL3/runDP ниже — публичны для пошаговых векторных
    // тестов (L3 перезаписывает Y62).

private:
    static inline uint32_t curveAC7(uint32_t idx) {
        uint32_t i = idx & kM24;
        return i < (uint32_t)kFilterCurveLen ? kFilterCurve[i] : 0u;
    }
    static inline uint32_t curveB48(uint32_t idx) {
        // X:$144B48 = X:$144AC7 + 129
        uint32_t i = 129u + (idx & kM24);
        return i < (uint32_t)kFilterCurveLen ? kFilterCurve[i] : 0u;
    }

    // ---------------- L1 $0A5D-$0A82 (public: пошаговый тест) ----------------
public:
    void runL1(const In& in, Out& out) {
        uint32_t bofs = (in.bofs >> 16) & kM24;
        uint32_t c1 = curveAC7(bofs);
        uint32_t y0 = c1;                          // y0 = старый B1(b) = c1
        int64_t b = acc24(c1);
        b = s56(b >> 1);                           // asr b (акк. на 1)
        b = s56(b + acc24(0x800000));              // add #>$800000 (-1.0)
        b = neg56(b);                              // neg
        uint32_t y1 = sat24(b);                    // move b,y1 (полный acc — лимитер!)
        int64_t a_s = acc24(d4y);
        int64_t b_s = acc24(d4x);
        uint32_t x0 = d3y;                         // прошлый wrap (y-часть)
        uint32_t x1 = d3x;                         // прошлый wrap (x-часть)
        for (int i = 0; i < 16; ++i) {
            // A7A: store B1(a); a -= x0*y1; x0 <- B1(a)  (a,x0 и a,y:(r4)+ — с лимитером)
            uint32_t st = sat24(a_s);
            out.Y62[2 * i] = st;
            a_s = smac(a_s, neg24(x0), y1);
            x0 = st;
            // A7B: store B1(b); b -= y1*x1; x1 <- B1(b)  (b,x1 и b,y:(r4)+ — с лимитером)
            st = sat24(b_s);
            out.Y62[2 * i + 1] = st;
            b_s = smac(b_s, neg24(y1), x1);
            x1 = st;
            // A7C: a -= y0*x0; x0 <- X:(r0) = X:$30+i
            a_s = smac(a_s, neg24(y0), x0);
            x0 = in.x30[i];
            // A7D: b -= x1*y0; x1 <- X:$20+i
            b_s = smac(b_s, neg24(x1), y0);
            x1 = in.x20[i];
            // A7E: a += x0*y1; x0 <- X:$30+i (повторно)
            a_s = smac(a_s, x0, y1);
            x0 = in.x30[i];
            // A7F: b += y1*x1; x1 <- X:$20+i (повторно)
            b_s = smac(b_s, y1, x1);
            x1 = in.x20[i];
        }
        d4y = sat24(a_s); d4x = sat24(b_s);        // move a/b,y/x:(r7-$88) — с лимитером
        d3y = in.x30[15]; d3x = in.x20[15];        // y/x:(r7-$89) = P+$D3 (wrap, mem->reg)
    }

    // ---------------- L2 $0A84-$0A9B ----------------
    void runL2(const In& in, Out& out) {
        uint32_t wofs = (in.wofs >> 16) & kM24;
        uint32_t bofs = (in.bofs >> 16) & kM24;
        uint32_t c2 = curveAC7((wofs + bofs) & kM24);
        uint32_t y0 = c2;
        int64_t a_s = acc24(d5y);
        int64_t b_s = acc24(d5x);
        uint32_t x1 = out.Y62[0];                  // pre-loop x:(r5)+ = Y:$62
        for (int i = 0; i < 16; ++i) {
            // A95: store B1(a) -> Y:$20+2i; a += x1*y0   (a,x0 и a,y:(r4)+ — с лимитером)
            uint32_t st = sat24(a_s);
            out.Y20[2 * i] = st;
            a_s = smac(a_s, x1, y0);
            // A96: a -= y0*st; x1 <- Y:$63+2i
            a_s = smac(a_s, neg24(y0), st);
            x1 = out.Y62[2 * i + 1];
            // A97: store B1(b) -> Y:$21+2i; b += x1*y0   (b,x0 и b,y:(r4)+ — с лимитером)
            st = sat24(b_s);
            out.Y20[2 * i + 1] = st;
            b_s = smac(b_s, x1, y0);
            // A98: b -= y0*st; x1 <- Y:$64+2i (чётный след.)
            b_s = smac(b_s, neg24(y0), st);
            if (i < 15) x1 = out.Y62[2 * (i + 1)];
        }
        d5y = sat24(a_s); d5x = sat24(b_s);        // move a/b,y/x:(r7-$87) — с лимитером
    }

    // ---------------- L3 $0A9D-$0AB5 ----------------
    void runL3(const In& in, Out& out) {
        // угол L3: 48-бит состояние делеЯ L:P+$CF = X:(r7-$8D) : Y:(r7-$8D)
        // (move x:... ,b -> A1 биты [47..24]; move y:...,b0 -> A0 биты [23..0])
        // ВНИМАНИЕ: индекс КРИВОЙ берётся из P+$CF (деля), НЕ из стейта P+$CC!
        uint64_t st89 = ((uint64_t)(in.cfx & kM24) << 24) | (uint64_t)(in.cfy & kM24);
        uint64_t v = st89 >> 17;                   // asr #$11
        uint32_t idx = (uint32_t)(v & kM24);       // move b0,r2 — A0-часть (биты [40..17])!
        uint32_t c3 = curveB48(idx);
        uint32_t y0 = c3;
        int64_t a_s = acc24(ccy);
        int64_t b_s = acc24(ccx);
        uint32_t x1 = in.x10[0];                   // pre-loop x:(r3)+ = X:$10
        for (int i = 0; i < 16; ++i) {
            // AAF: store B1(a) -> Y:$62+2i; a += x1*y0   (a,x0 и a,y:(r4)+ — с лимитером)
            uint32_t st = sat24(a_s);
            out.Y62[2 * i] = st;
            a_s = smac(a_s, x1, y0);
            // AB0: a -= y0*st; x1 <- X:$00+i
            a_s = smac(a_s, neg24(y0), st);
            x1 = in.x00[i];
            // AB1: store B1(b) -> Y:$63+2i; b += x1*y0   (b,x0 и b,y:(r4)+ — с лимитером)
            st = sat24(b_s);
            out.Y62[2 * i + 1] = st;
            b_s = smac(b_s, x1, y0);
            // AB2: b -= y0*st; x1 <- X:$11+i
            b_s = smac(b_s, neg24(y0), st);
            if (i < 15) x1 = in.x10[i + 1];
        }
        ccy = sat24(a_s); ccx = sat24(b_s);        // move a/b,y/x:(r7-$90) — с лимитером
    }

    // ---------------- DP $0AB7-$0AD0 ----------------
    void runDP(const In& in, Out& out) {
        // depth = (2|ATK-0.5|)^2
        int64_t b = s56(acc24(in.atk & kM24) - acc24(0x400000));
        if (s24(B1(b)) < 0) b = neg56(b);          // abs
        b = s56(b << 1);                           // asl b
        uint32_t x0 = sat24(b);                    // move b,x0 (полный acc — лимитер!)
        int64_t a = smac(0, x0, x0);               // mpy x0,x0,a
        uint32_t depth = sat24(a);                 // move a,x1 (полный acc — лимитер!)
        int64_t depthAcc = a;                      // acc-значение (конвейерный store i=0)
        int64_t a_s = 0;                           // аккумулятор a-ветви (конвейер)
        uint32_t y0 = in.dec & kM24;               // DEC-слово (24-бит ячейка)
        if (in.phase == 4) y0 = 0;                 // KILL-гейт (фаза AMP-env == 4)
        uint32_t x1 = depth;                       // move a,x1
        // $0AC0: move x:(r7-$84),b — b переиспользован под проверку фазы;
        // конвейерная запись на iter0 протекает в X:$40 (b = фаза AMP-env)
        int64_t b_s = acc24(in.phase);
        uint32_t xa = out.Y20[0];                  // AC6: x0 = Y:$20 (a_0 вход)
        uint32_t y1 = out.Y62[0];                  // Y:$62 (L3 out a_0)
        uint32_t outA[16], outB[16];
        for (int i = 0; i < 16; ++i) {
            // AC9: store pre-a -> X:(r2)+; a = y0*x0; y1 <- Y:$62+2i (store — с лимитером)
            int64_t pre_a = (i == 0) ? depthAcc : a_s;
            a_s = smac(0, y0, xa);
            outA[i] = sat24(pre_a);
            y1 = out.Y62[2 * i];
            // ACA: asl #$1
            a_s = s56(a_s << 1);
            // ACB: a += y1*x1; x0 <- Y:$21+2i (b_i вход)
            a_s = smac(a_s, y1, x1);
            uint32_t xb = out.Y20[2 * i + 1];
            // ACC: store pre-b -> X:(r3)+; b = y0*x0; y1 <- Y:$63+2i (store — с лимитером)
            int64_t pre_b = b_s;
            b_s = smac(0, y0, xb);
            outB[i] = sat24(pre_b);
            y1 = out.Y62[2 * i + 1];
            // ACD: asl #$1
            b_s = s56(b_s << 1);
            // ACE: b += y1*x1; x0 <- Y:$22+2i (a_{i+1} вход)
            b_s = smac(b_s, y1, x1);
            if (i < 15) xa = out.Y20[2 * (i + 1)];
        }
        // ACF/AD0: финальные записи (move a/b,x:(r2)+/(r3)+ — с лимитером)
        for (int i = 0; i < 16; ++i) out.bankA[i] = outA[i]; // X:$51-$60
        out.bankA[16] = sat24(a_s);                          // X:$61
        for (int i = 0; i < 16; ++i) out.bankB[i] = outB[i]; // X:$40-$4F
        out.bankB[16] = sat24(b_s);                          // X:$50
    }
};

} // namespace mnmfm

#endif // MNM_FILTER_STAGE2_HPP
