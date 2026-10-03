// =============================================================================
// MnmTail.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B, DSP1.
// ЧИСТЫЙ C++ ПОРТ ХВОСТА КАДРА ГОЛОСА: P:$0AD1-$0B4C (пак 11).
//
// Хвост = последний участок кадра голоса: DIV-интегратор длины делэя, интеграция
// фаз тапов, публикация выхода, DWID-рампа и мастер-микш с эхо-посылом.
// (Делэй и стадия 2 — РАЗНЫЕ блоки; здесь только хвост $0AD1-$0B4C.)
//
// Происхождение:
//   * блок разобран поблочно в итерации 22 и верифицирован слово-в-слово
//     против бит-точного эмулятора OS 1.32B (13_stage2_tail: OK=8296 BAD=0);
//   * этот хедер — дословная транскрипция закрытой формы exp22_model.py
//     (::model_tail) в C++ (56-битная аккумуляторная арифметика DSP56300);
//   * векторы vectors/tail_vectors.txt — 56 кадров (12 конфигураций x 4 кадра
//     + skip-ветвь + контроли); ОЖИДАЕМЫЕ ЗНАЧЕНИЯ взяты из снапшотов
//     эмулятора (не из питоновской модели);
//   * func_000397 (гребёнка 8 дробных тапов, доказана в итерации 14) в хвосте
//     — чёрный ящик: её эффекты приходят со срезом b7 (post-copy X:$71-$80),
//     ровно как в методике итерации 22.
//
// Блоки (адреса — комментарии-обоснование, код адресов не исполняет):
//   T1 DIV        $0AD1-$0AE9 : 24-шаговое невосстанавливающее деление
//                                (L:P+$CF asr 2) -> обратная длина, умножение
//                                на глобальный счётчик Y:$C4, порог cmp #$10
//                                -> ветвь bge $0B1E (skip T2/T3/T4)
//   T2 INTEGRATE  $0AEA-$0B0B : 17 записей L:$90-$A0 (X=целая позиция тапа,
//                                Y=дробь); де-зиппер ротация P+$CB
//   T3 CALL397    $0B0B-$0B13 : jsr func_000397 (эффекты внутри b7-среза)
//   T4 COPY       $0B14-$0B1D : X:$71-$80 -> Y:(X:$C5)+ окно $4000 (m4=$3FFF)
//   T5 DWID RAMP  $0B1E-$0B32 : 16 значений Y:$10-$1F, delta=(env*DWID)^2-env,
//                                a/b-цепочки с полшаговым сдвигом (анти-зиппер)
//   T6 MASTER MIX $0B33-$0B49 : outs[2k]  = (fb[k]  + L2[2k+1]) * mod[k]
//                               outs[2k+1]= (Xpre[$10+k] + L2[2k]) * mod[k]
//                               fb[k] = outs[k] при X:$FF=0 (самофидбек),
//                                       иначе чистая аудио-шина Xpre[k]
//                               echo[k] = asr1(outs[2k]+outs[2k+1]) -> эхо-кольцо
// =============================================================================
#pragma once
#include <cstdint>
#include <cstdio>
#include <string>
#include <map>

namespace mnmtail {

static const uint64_t M24 = 0xFFFFFFull;
static const uint64_t M48 = (1ull << 48) - 1;
static const uint64_t M56 = (1ull << 56) - 1;

inline int64_t sext(uint64_t v, int b) {
    v &= (1ull << b) - 1;
    return (int64_t)v - ((int64_t)(v >> (b - 1)) << b);
}

// ключи out-карты, совместимые с exp22_model.py ("X:090", "Y:4003", "E:030A"...)
inline std::string kx(int a) { char b[16]; std::snprintf(b, sizeof b, "X:%03X", a); return std::string(b); }
inline std::string ky(int a) { char b[16]; std::snprintf(b, sizeof b, "Y:%03X", a); return std::string(b); }
inline std::string ky4(int a){ char b[16]; std::snprintf(b, sizeof b, "Y:%04X", a); return std::string(b); }
inline std::string kyo4(int a){ char b[16]; std::snprintf(b, sizeof b, "Yo:%04X", a); return std::string(b); }
inline std::string kxo4(int a){ char b[16]; std::snprintf(b, sizeof b, "Xo:%04X", a); return std::string(b); }
inline std::string ke4(int a){ char b[16]; std::snprintf(b, sizeof b, "E:%04X", a); return std::string(b); }

// --- 56-битные аккумуляторные операции (семантика DSP56300) ------------------
inline uint64_t compose(uint32_t v) {              // 24-бит -> acc (A1=sext,A0=0)
    return (uint64_t)(sext(v, 24) & M56) << 24;
}
inline uint64_t part_b1(uint64_t acc, uint32_t v) {// запись ТОЛЬКО B1
    return (acc & ~(M24 << 24)) | ((uint64_t)(v & M24) << 24);
}
inline uint64_t asr56(uint64_t acc, int n) {
    return ((uint64_t)(sext(acc & M56, 56) >> n)) & M56;
}
inline uint64_t lsr56(uint64_t acc, int n) {
    return ((acc & M56) >> n) & M56;
}
inline uint32_t a1(uint64_t acc) { return (uint32_t)((acc >> 24) & M24); }
inline uint32_t a0(uint64_t acc) { return (uint32_t)(acc & M24); }

inline uint64_t add56(uint64_t acc, int64_t v) {   // acc + sign-extended v
    return (uint64_t)(sext((uint64_t)((int64_t)acc + v), 56)) & M56;
}
inline uint64_t add48(uint64_t acc, uint64_t v48) { // add y,a (48-бит слагаемое)
    return (uint64_t)(sext((uint64_t)((int64_t)acc + (int64_t)sext(v48, 48)), 56)) & M56;
}

inline uint64_t div_step(uint64_t acc, uint32_t s) {
    if ((s & M24) == 0) return M48;                // деление на ноль (HW)
    uint64_t D = (((acc >> 24) & M24) << 24) | (acc & M24);   // 48-бит пара
    uint64_t d1 = (D >> 24) & M24, d0 = D & M24;
    d1 = ((d1 << 1) | (d0 >> 23)) & M24;
    d0 = (d0 << 1) & M24;
    int64_t s_s = sext(s & M24, 24), d1_s = sext((uint32_t)d1, 24);
    if ((d1_s < 0) == (s_s < 0)) d1 = (d1 - (s & M24)) & M24;
    else                         d1 = (d1 + (s & M24)) & M24;
    uint64_t qbit = ((sext((uint32_t)d1, 24) < 0) == (s_s < 0)) ? 1 : 0;
    d0 |= qbit;
    return (uint64_t)(sext((d1 << 24) | d0, 48)) & M56;
}

inline uint64_t mpy(uint32_t sx, uint32_t sy) {    // mpy ss: (x*y)<<1
    int64_t p = (int64_t)sext(sx, 24) * (int64_t)sext(sy, 24);
    return (uint64_t)(sext((uint64_t)(p << 1), 56)) & M56;
}
inline uint64_t mpyuu(uint32_t ux, uint32_t uy) {  // mpyuu: беззнаковое
    uint64_t p = ((ux & M24) * (uy & M24)) << 1;
    return (uint64_t)(sext(p, 56)) & M56;
}
inline uint64_t dmac_su(uint64_t acc, uint32_t sx, uint32_t uy) {
    uint64_t a1v = sx & M24;
    int64_t s1v = (a1v & 0x800000ull) ? (int64_t)(a1v - (1ull << 24)) : (int64_t)a1v;
    int64_t sum = (int64_t)acc + ((s1v * (int64_t)(uy & M24)) << 1);
    return (uint64_t)(sext((uint64_t)sum, 56)) & M56;
}

// --- входы/выходы ------------------------------------------------------------
struct TailIn0 {          // срез s0: момент pc=$0AD1 (r6=$400)
    uint32_t YPCB, XPCB;  // Y:(r6+$CB) / X:(r6+$CB) — слоты де-зиппера
    uint32_t YPCF, XPCF;  // L:P+$CF — 48-бит состояние длины делэя
    uint32_t YP1F;        // Y:(r6+$1F) = DWID (машинная ручка, CC87-семейство)
    uint32_t YPFF;        // Y:(r6+$FF) = состояние env-рампы (Y:$4FF)
    uint32_t X050, X061;  // источники де-зиппер ротации
    uint32_t YC4;         // Y:$C4 — глобальный счётчик хоста
    uint32_t XC5;         // X:$C5 — указатель хостового кольца выхода
};

struct TailB8 {           // срез b8: pc=$0B33 (после DWID, перед микшем)
    uint32_t XFF;         // X:$FF — кольцо мастер-выхода (0 в харнесе!)
    uint32_t X2C9;        // X:$2C9 — указатель эхо-кольца
    uint32_t l2[32];      // Y:$20-$3F — банк L2 (выход стадии 2)
    uint32_t xpre[32];    // X:$00-$1F — аудио-шина до микша
};

struct TailResult {
    bool took_bge;                        // ветвь skip ($0AE9 bge $0B1E)
    std::map<std::string, uint32_t> out;  // ключи совпадают с exp22_model
    uint32_t mod[16];                     // Y:$10-$1F — рампа модуляции
    uint32_t outs[32];                    // мастер-выход (X:(X:$FF)+ / Y:$0000+)
    uint32_t echo[16];                    // эхо-посыл
    uint32_t tapsX[17], tapsY[17];        // L:$90-$A0
};

inline void model_tail(const TailIn0& s0, const uint32_t* b7 /*16 слов, nullptr при skip*/,
                       const TailB8& b8, TailResult& res) {
    res.out.clear();
    uint64_t A = 0, B = 0;
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;

    // ---------------- T1: DIV ($0AD1-$0AE9) --------------------------------
    B = compose(s0.XPCF);                        // L:P+$CF -> B (X-слово в B1)
    B = (B & ~M24) | s0.YPCF;                    //   + Y-слово в B0
    B = asr56(B, 2);                             // asr #$2,b,b
    A = compose(1);                              // move #$1,a (1.0 в 24.24)
    y0 = a0(B);                                  // move b0,y0 (делитель S)
    for (int i = 0; i < 24; ++i) {               // 24 x div y0,a
        A = div_step(A, y0);
        B = part_b1(B, a0(A));                   // move a0,b (B1-only)
    }
    B = asr56(B, 17);                            // asr #$11,b,b
    y1 = a1(B); y0 = a0(B);                      // move b1,y1 / move b0,y0
    x0 = s0.YC4 & M24;                           // move y:>$c4,x0
    A = mpyuu(y0, x0);                           // mpyuu y0,x0,a
    A = dmac_su(A, y1, x0);                      // dmac su y1,x0,a
    A = asr56(A, 1);                             // asr a
    if (A == 0) B = 0;                           // clr b ifeq (гвард Y:$C4==0)
    B = add56(B, -(int64_t)A);                   // sub a,b
    A = B;                                       // tfr b,a
    res.took_bge = (sext(B - compose(0x10), 56) >= 0);   // cmp #<$10,b; bge
    res.out["took_bge"] = res.took_bge ? 1u : 0u;

    if (!res.took_bge) {
        // ---------------- T2: интеграция фаз тапов ($0AEA-$0B0B) ------------
        A = add56(A, (int64_t)compose(0x40));    // add #>$40,a
        B = compose(y0);                         // tfr y0,b
        B = asr56(B, 1);                         // asr b
        uint32_t x0_div = a1(B);                 // move b1,x0 (константа 17 итераций)
        B = part_b1(B, a0(A));                   // move a0,b (B1-only)
        B = lsr56(B, 1);                         // lsr b
        uint64_t y48 = ((uint64_t)y1 << 24) | y0;
        for (int k = 0; k < 17; ++k) {           // do #<$11, тело $0AF8-$0AFA
            res.tapsX[k] = a1(A);                // a,l:(r1)   — пре-ALU запись
            res.tapsY[k] = a1(B);                // b1,y:(r1)+ — пре-ALU запись
            res.out[kx(0x90 + k)] = res.tapsX[k];
            res.out[ky(0x90 + k)] = res.tapsY[k];
            A = add48(A, y48);                   // add y,a
            B = add56(B, (int64_t)compose(x0_div));  // add x0,b
            B = B & (0x7FFFFFull << 24);         // and x1,b (B1-only)
        }
        res.out["X:040"] = s0.YPCB;              // де-зиппер ротация ($0AFB-$0B0A)
        res.out["X:051"] = s0.XPCB;
        res.out["Y:P+CB"] = s0.X050;
        res.out["X:P+CB"] = s0.X061;

        // ---------------- T4: копия в хостовое кольцо ($0B14-$0B1D) ---------
        uint32_t xc5 = s0.XC5 & M24;
        uint32_t base = xc5 & ~0x3FFFu;          // m4 = $3FFF: окно $4000
        for (int k = 0; k < 16; ++k) {
            uint32_t addr = base | (((xc5 & 0x3FFFu) + (uint32_t)k) % 0x4000u);
            res.out[ky4((int)addr)] = b7[k];     // источник: post-func_397 X:$71-$80
        }
    }

    // ---------------- T5: DWID-рампа ($0B1E-$0B32) --------------------------
    uint32_t dwid = s0.YP1F;
    A = mpy(dwid, dwid);                         // mpy x0,x0,a
    uint32_t env = s0.YPFF;
    B = mpy(env, env);                           // mpy y0,y0,b
    x0 = a1(A);                                  // a,x0   (DWID^2)
    x1 = a1(B);                                  // move b,x1 (env^2)
    A = mpy(x1, x0);                             // env^2 * DWID^2
    res.out["Y:P+FF"] = a1(A);                   // move a,y:(r7-$5d) — новое состояние
    A = add56(A, -(int64_t)compose(env));        // sub y1,a (delta)
    B = compose(env);                            // tfr y1,b
    y0 = 0x10;                                   // #$10,y0
    x0 = a1(A);                                  // a,x0 (delta, пре-ALU)
    A = compose(env);                            // tfr y1,a
    B = add56(B, (int64_t)mpy(x0, 0x080000u));   // maci #>$80000,x0,b (delta/16)
    uint64_t step = mpy(y0, x0);                 // step = mpy($10, delta)
    for (int k = 0; k < 8; ++k) {
        res.mod[2 * k] = a1(A);                  // чётные: env + k*step
        A = add56(A, (int64_t)step);
        res.mod[2 * k + 1] = a1(B);              // нечётные: env + delta/16 + k*step
        B = add56(B, (int64_t)step);
        res.out[ky(0x10 + 2 * k)] = res.mod[2 * k];
        res.out[ky(0x11 + 2 * k)] = res.mod[2 * k + 1];
    }

    // ---------------- T6: мастер-микш с фидбеком ($0B33-$0B49) --------------
    uint32_t XFF = b8.XFF & M24;
    uint32_t r3 = b8.X2C9 & M24;
    uint32_t outs[32];
    uint32_t echoes[16];
    A = mpy(b8.xpre[0x10], res.mod[0]);          // $0B3F mpy x1,x0,a (затравка)
    x1 = b8.xpre[0];                             // $0B3F load x:(r2)+,x1
    B = mpy(x1, res.mod[0]);                     // $0B40 mpy x1,x0,b
    y0 = b8.l2[0];
    y1 = b8.l2[1];
    for (int k = 0; k < 16; ++k) {
        B = add56(B, (int64_t)mpy(res.mod[k], y1));      // $0B43 mac x0,y1,b
        outs[2 * k] = a1(B);
        A = add56(A, (int64_t)mpy(y0, res.mod[k]));      // $0B44 mac y0,x0,a
        outs[2 * k + 1] = a1(A);
        B = add56(B, (int64_t)A);                        // $0B45 add a,b
        B = asr56(B, 1);                                 // $0B46 asr b
        echoes[k] = a1(B);                               // $0B48 b,x:(r3)+ (пре-mpy)
        if (k < 15) A = mpy(b8.xpre[0x11 + k], res.mod[k + 1]);  // $0B47
        // фидбек: чтение x:(r2)+ = содержимое X:(k+1) этого же кадра при наложении
        int64_t j = (int64_t)(k + 1) - (int64_t)XFF;
        if (j >= 0 && j <= 2 * k + 1) x1 = outs[j];
        else x1 = (k + 1 < 0x20) ? b8.xpre[k + 1] : 0;
        if (k < 15) {                                    // $0B48 mpy x1,x0,b
            B = mpy(x1, res.mod[k + 1]);
            y0 = b8.l2[2 * k + 2];
            y1 = b8.l2[2 * k + 3];
        }
    }
    for (int j = 0; j < 32; ++j) {
        res.outs[j] = outs[j];
        res.out[kxo4((int)((XFF + (uint32_t)j) & M24))] = outs[j];  // X:(X:$FF)+j
        res.out[kyo4(j)] = outs[j];                                 // Y:$0000-$001F
    }
    for (int k = 0; k < 16; ++k) {
        res.echo[k] = echoes[k];
        res.out[ke4((int)((r3 + (uint32_t)k) & M24))] = echoes[k];
    }
    res.out["X:2C9"] = (r3 + 16) & M24;          // $0B49 — ПОСЛЕ снапшота b9
}

}  // namespace mnmtail
