// ============================================================================
// mnm_dsp_fix.hpp — Q23 / 56-бит арифметика DSP56300 и состояние памяти/регистров.
//
// Семантика 1:1 с бит-точным эталоном (MnmDspCore.hpp / dsp_emu.py, 20616/20616):
//   - 24-бит слова памяти (Q1.23), знаковое чтение s24()
//   - 56-бит аккумуляторы A/B: MAC = acc + (s24*s24)<<1; сатурация при записи
//     аккумулятора в 24-битную память (экстракция A1 с лимитом)
//   - rnd (MACR/MPYR/RND): acc += 0x800000; младшие 24 бит обнуляются
//   - адресная арифметика Rn = (Rn & ~Mn) | ((Rn + Nn) & Mn)
//   - L-память: l:(ea) = { X:(ea) — старшие 24 бит, Y:(ea) — младшие }
//   - флаги C/V/N/Z как на железе (нужно веткам bge/blt/bec и tcc)
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>

namespace mnmfix {

static constexpr uint32_t M24 = 0xFFFFFFu;

// знаковое 24-битное чтение
inline int32_t s24(uint32_t v) noexcept {
    v &= M24;
    return (v & 0x800000u) ? (int32_t)(v - 0x1000000u) : (int32_t)v;
}
// сатурация до 56 бит
inline int64_t sat56(int64_t v) noexcept {
    constexpr int64_t HI = 0x007FFFFFFFFFFFll, LO = -0x00800000000000ll;
    if (v > HI) return HI;
    if (v < LO) return LO;
    return v;
}
inline int64_t sext56(int64_t v) noexcept {
    v &= (1ll << 56) - 1;
    return (v & (1ll << 55)) ? v - (1ll << 56) : v;
}

// ---------------------------------------------------------------------------
// Память X/Y низкого окна (0..0x800). В плагине живёт в экземпляре голоса.
// ---------------------------------------------------------------------------
struct Mem {
    static constexpr uint32_t WIN = 0x800;
    uint32_t X[WIN], Y[WIN];
    void clear() { std::memset(X, 0, sizeof X); std::memset(Y, 0, sizeof Y); }
    inline uint32_t rd(char sp, uint32_t ea) const noexcept {
        ea &= M24;
        if (sp == 'x') return ea < WIN ? X[ea] : 0;
        return ea < WIN ? Y[ea] : 0;
    }
    inline void wr(char sp, uint32_t ea, uint32_t v) noexcept {
        ea &= M24; v &= M24;
        if (ea < WIN) (sp == 'x' ? X : Y)[ea] = v;
    }
    inline uint64_t rdL(uint32_t ea) const noexcept {
        return ((uint64_t)rd('x', ea) << 24) | rd('y', ea);
    }
    inline void wrL(uint32_t ea, uint64_t v48) noexcept {
        wr('x', ea, (uint32_t)(v48 >> 24));
        wr('y', ea, (uint32_t)(v48 & M24));
    }
};

// ---------------------------------------------------------------------------
// ROM прошивки: таблицы $100000-$14C800 (X и Y-образы РАЗЛИЧАЮТСЯ!).
// Заполняется из tables_x.bin / tables_y.bin (mnm_chain_tables.hpp).
// ---------------------------------------------------------------------------
struct Rom {
    static const uint32_t BASE = 0x100000u, SIZE = 0x4C800u;   // $100000..$14C7FF
    const uint32_t* X;   // [SIZE]
    const uint32_t* Y;   // [SIZE]
    inline uint32_t rd(char sp, uint32_t ea) const noexcept {
        const uint32_t i = (ea & M24) - BASE;
        if (i >= SIZE) return 0;
        return (sp == 'x' ? X : Y)[i];
    }
};

// ---------------------------------------------------------------------------
// Регистровый файл (живёт между секциями кадра, как в прошивке).
// ---------------------------------------------------------------------------
struct Regs {
    uint32_t r[8] = {0,0,0,0,0,0,0,0};
    int32_t  n[8] = {0,0,0,0,0,0,0,0};
    uint32_t m[8] = {M24,M24,M24,M24,M24,M24,M24,M24};
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;   // слова (без знака), знаким через s24
    int64_t  A = 0, B = 0;                     // 56-бит знаковые
    int      fc = 0, fv = 0, fn = 0, fz = 0, fe = 0;  // флаги C V N Z E(расширение)

    // Rn = (Rn & ~Mn) | ((Rn + delta) & Mn)
    inline uint32_t pstep(int i, int32_t delta) noexcept {
        const uint32_t rv = r[i], mv = m[i];
        r[i] = (uint32_t)(((uint64_t)(rv & ~mv)) | (((uint64_t)((int64_t)rv + delta)) & (uint64_t)mv));
        return r[i];
    }
    // ПОСТ-инкремент: возвращает СТАРЫЙ адрес (используется данными), затем
    // обновляет Rn — точная семантика (rN)+ / (rN)+nN на DSP56300
    inline uint32_t ppost(int i) noexcept {
        const uint32_t old = r[i];
        pstep(i, 1);
        return old;
    }
    inline uint32_t pnext(int i) noexcept {
        const uint32_t old = r[i];
        pstep(i, n[i]);
        return old;
    }
    inline uint32_t ppre(int i) noexcept {
        pstep(i, -1);
        return r[i];             // -(rN): пре-декремент, используется НОВЫЙ адрес
    }
    inline uint32_t ppostdec(int i) noexcept {   // (rN)-: пост-декремент
        const uint32_t old = r[i];
        pstep(i, -1);
        return old;
    }
};

// аккумулятор → 24-бит слово при записи в память (экстракция A1 + лимит)
inline uint32_t acc_to24(int64_t acc) noexcept {
    if (acc > 0x007FFFFFFFFFFFll) return 0x7FFFFF;
    if (acc < -0x00800000000000ll) return 0x800000;
    return (uint32_t)((acc >> 24) & M24);
}

// MAC/MPY: dst = (acc ? dst : 0) + (s24(s1)*s24(s2) << 1); сатурация; флаги V
inline void mac_core(int64_t& dst, uint32_t s1, uint32_t s2, bool accumulate, int& vflag) noexcept {
    int64_t prod = ((int64_t)s24(s1) * (int64_t)s24(s2)) << 1;
    int64_t res = sat56(accumulate ? dst + prod : prod);
    vflag = (-(1ll << 47) <= res && res < (1ll << 47)) ? 0 : 1;
    dst = res;
}
// MACSU: s1 знаковый, s2 беззнаковый
inline void macsu_core(int64_t& dst, uint32_t s1, uint32_t s2, bool accumulate, int& vflag) noexcept {
    int64_t prod = ((int64_t)s24(s1) * (int64_t)(s2 & M24)) << 1;
    int64_t res = sat56(accumulate ? dst + prod : prod);
    vflag = (-(1ll << 47) <= res && res < (1ll << 47)) ? 0 : 1;
    dst = res;
}
// округление аккумулятора (rnd): += 0x800000, обнулить младшие 24 бита
inline void rnd_acc(int64_t& dst) noexcept {
    dst = sext56(dst + 0x800000ll);
    dst = (dst >> 24) << 24;
}
// MACR/MPYR
inline void macr_core(int64_t& dst, uint32_t s1, uint32_t s2, bool accumulate, int& vflag) noexcept {
    mac_core(dst, s1, s2, accumulate, vflag);
    rnd_acc(dst);
}

// флаги N/Z после ALU (C/V выставляются в конкретных операциях)
inline void flags_nz(Regs& g, int64_t res) noexcept {
    g.fn = (res < 0) ? 1 : 0;
    g.fz = (res == 0) ? 1 : 0;
}

} // namespace mnmfix
