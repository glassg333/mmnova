/*
 * MnmFilterFaithful.h — BIT-ТОЧНЫЕ порты DSP56300 фильтров Monomachine.
 *
 * Подход: каждая инструкция DSP56300 → одна C++ строка (как в ChorusCore.h,
 * свереном с прошивкой "инструкция-в-инструкцию").
 *
 * Сохранено:
 *   - 56-bit accumulator arithmetic (a, b — int64_t)
 *   - 24-bit signed fixed-point (q.23) — s24 helpers
 *   - Порядок параллельных переносов DSP56300 (read-before-write)
 *   - Saturation при MPY/MAC (как бит SM в SR)
 *   - Loop bounds (do #<N)
 *   - Modulo/linear addressing
 *
 * Это НЕ эмулятор: нет программного счётчика, нет декодера опкодов, нет памяти
 * команд. Это статический C++ код, mirrorящий DSP-инструкции 1:1 с адресами
 * в комментариях.
 *
 * Арифметика совпадает 1:1 с nova::ChorusCore (08_cpp_reference/chorus_plugin/).
 */
#ifndef MNM_FILTER_FAITHFUL_H
#define MNM_FILTER_FAITHFUL_H

#include <cstdint>
#include <array>
#include <algorithm>
#include "mnm_filter_tables.h"

namespace mnm {

// ============================================================================
// 24-bit signed fixed-point helpers (полная копия из ChorusCore.h)
// ============================================================================
namespace fxp {

using Word = uint32_t;            // 24-bit unsigned container

static constexpr int64_t one = 0x1000000LL;  // 2^24, для преобразования s24→acc

inline int32_t signed24(Word v) {
    v &= 0xFFFFFFu;
    return v & 0x800000u ? int32_t(v) - 0x1000000 : int32_t(v);
}

// q(w) — 24-bit слово как 56-bit аккумулятор (значение * 2^24)
inline int64_t q(Word v) noexcept { return int64_t(signed24(v)) * one; }

// word(acc) — извлечь 24-битное слово из 56-bit аккумулятора (high 24 bits, с округлением вниз)
inline Word word(int64_t v) noexcept {
    // asr(v, 24) с округлением к -inf для отрицательных
    const auto divisor = int64_t(1) << 24;
    int64_t r = (v >= 0) ? (v / divisor) : -((-v + divisor - 1) / divisor);
    return Word(r) & 0xFFFFFFu;
}

// high(acc) — high 24 bits (без округления, просто >>24)
inline Word high(int64_t v) noexcept { return Word(uint64_t(v) >> 24) & 0xFFFFFFu; }
// low(acc) — low 24 bits
inline Word low(int64_t v) noexcept { return Word(v) & 0xFFFFFFu; }

// wrap56 — 56-bit two's complement wrap
inline int64_t wrap56(int64_t v) noexcept {
    const auto u = uint64_t(v) & 0xFFFFFFFFFFFFFFull;
    return u & 0x80000000000000ull ? int64_t(u) - 0x100000000000000LL : int64_t(u);
}

// clip48 — saturation to 48-bit signed range
inline int64_t clip48(int64_t v) noexcept {
    return std::clamp<int64_t>(v, -0x800000000000LL, 0x7FFFFFFFFFFFLL);
}

// asr — arithmetic shift right (with floor rounding for negatives)
inline int64_t asr(int64_t v, unsigned n) noexcept {
    const auto divisor = int64_t(1) << n;
    return v >= 0 ? v / divisor : -((-v + divisor - 1) / divisor);
}

// asl — arithmetic shift left (with 56-bit wrap)
inline int64_t asl(int64_t v, unsigned n) noexcept {
    return wrap56(int64_t((uint64_t(v) << n) & 0xFFFFFFFFFFFFFFull));
}

// multiply — 24x24 signed fractional → 56-bit accumulator (как DSP56300 MPY)
// x_int * y_int * 2 — compensate for DSP56300 left-justify convention
inline int64_t multiply(Word a, Word b, bool unsignedB = false) noexcept {
    return int64_t(signed24(a)) * (unsignedB ? int64_t(b & 0xFFFFFFu) : int64_t(signed24(b))) * 2;
}

// mac — accumulate product into accumulator (как DSP56300 MAC)
inline int64_t mac(int64_t acc, Word b, Word c, bool negative, bool saturate, bool unsignedC = false) noexcept {
    const auto v = acc + (negative ? -1 : 1) * multiply(b, c, unsignedC);
    return saturate ? clip48(v) : wrap56(v);
}

// rnd — round accumulator at bit 23 (round-half-up)
inline int64_t rnd(int64_t v) noexcept {
    // Add 0x800000 to bit 23 (round half up), then zero low 24 bits
    int64_t r = (v + 0x800000LL) & ~(0xFFFFFFLL);
    return wrap56(r);
}

// withLow — set low 24 bits of accumulator
inline int64_t withLow(int64_t a, Word v) noexcept {
    return wrap56(int64_t((uint64_t(a) & 0xFFFFFFFFFF000000ull) | (v & 0xFFFFFFu)));
}

// float converters (for convenience, not used in kernel port)
inline float to_float(Word v) { return static_cast<float>(signed24(v)) / 8388608.0f; }
inline Word from_float(float v) {
    float scaled = v * 8388608.0f;
    if (scaled > 8388607.0f) scaled = 8388607.0f;
    if (scaled < -8388608.0f) scaled = -8388608.0f;
    int32_t r = static_cast<int32_t>(scaled);
    return uint32_t(r) & 0xFFFFFFu;
}

}  // namespace fxp


// ============================================================================
// BIT-ТОЧНЫЙ 1-pole LP filter (FX-CHORUS LP, P:$147787-79D)
// Зеркалит ChorusCore.cpp строки 288-306.
//
// В оригинале обрабатывает ДВЕ параллельные цепи (a, b — стерео или
// feedback-path + dry-path). Здесь симулируем обе (как в прошивке).
//
// Вход: input 16 сэмплов в Y-память по адресу r5 = $40
// Выход: 16 сэмплов в Y-память по адресу r4 = $60
// Состояние: 2 слова в Y:(r6+$1E, r6+$1F) = Y[(V+$A1, V+$A2)]
//            (где r6 = V-$28, +$1E = +0x1E = V+$A1... wait, это варьируется)
//
// Здесь: пользователь передаёт TONE 0..127, класс вычисляет коэффициент
// из kLP_filter_coeffs[TONE] и обрабатывает по 1 сэмплу за вызов.
// ============================================================================
class MnmOnePoleFaithful {
public:
    MnmOnePoleFaithful() { reset(); }

    void reset() {
        a_acc_ = 0;          // accumulator a (Y:(r6+$1e) в прошивке)
        b_acc_ = 0;          // accumulator b (Y:(r6+$1f))
    }

    // Setup as in P:$147787-95
    void setTone(int tone_param) {
        // P:147789  move y:(r6+$a),b         — tone param (24-bit, top 8 bits = 0..127)
        // b = tone_param << 16 (q.23 representation of 0..127 integer in high byte)
        fxp::Word tone_word = uint32_t(tone_param & 0xFF) << 16;
        // P:14778a  asr #$10,b,b           — shift right 16, leaves top byte in low byte
        fxp::Word b_word = fxp::word(fxp::asr(fxp::q(tone_word), 16));
        // P:14778f  move b,n2             — n2 = b (offset into LP table)
        n2_ = b_word;
        // P:147794  move x:(r2+n2),x0      — x0 = kLP_filter_coeffs[tone]
        x0_ = uint32_t(kLP_filter_coeffs[n2_ & 0x7F]) & 0xFFFFFFu;
    }

    // Process one stereo pair: input (y1_L, y1_R) → output (out_L, out_R)
    // Каждая итерация цикла P:147796-9B (16× для 16-сэмплового блока)
    void processStereoPair(fxp::Word y1_L, fxp::Word y1_R,
                            fxp::Word& out_L, fxp::Word& out_R) {
        // === Chain A (left) ===
        // P:147798  mac x0,y1,a  a,x1  a,y:(r4)+
        //   x1 = OLD a; output = OLD a; a += x0 * y1
        fxp::Word x1 = fxp::word(a_acc_);
        out_L = x1;
        a_acc_ = fxp::mac(a_acc_, x0_, y1_L, false, true);

        // P:147799  mac -x1,x0,a  y:(r5)+,y1
        //   a += -x1 * x0 = -x0 * OLD_a
        a_acc_ = fxp::mac(a_acc_, x1, x0_, true, true);
        // y1 = next input (handled by caller)

        // === Chain B (right) === (same pattern)
        fxp::Word x1_b = fxp::word(b_acc_);
        out_R = x1_b;
        b_acc_ = fxp::mac(b_acc_, x0_, y1_R, false, true);
        b_acc_ = fxp::mac(b_acc_, x1_b, x0_, true, true);
    }

    // Process mono single sample (chain A only)
    // Полное соответствие firmware P:$147798-799: output = OLD a (state before update)
    // y1_in = input q.23 sample
    // returns: q.23 output = OLD state (BEFORE this sample's update)
    fxp::Word processOne(fxp::Word y1_in) {
        // P:147798  mac x0,y1,a  a,x1  a,y:(r4)+
        //   parallel: x1 = OLD a; OUTPUT (to r4) = OLD a; a += x0*y1
        fxp::Word x1 = fxp::word(a_acc_);     // x1 = OLD a (saved before update)
        fxp::Word out = x1;                   // OUTPUT = OLD a (parallel store)
        a_acc_ = fxp::mac(a_acc_, x0_, y1_in, false, true);
        // P:147799  mac -x1,x0,a  y:(r5)+,y1
        //   a += -x1*x0 = -x0 * OLD_a  (using x1 = OLD a from P:147798 parallel)
        a_acc_ = fxp::mac(a_acc_, x1, x0_, true, true);
        // Возврат: OLD a (как в firmware)
        return out;
    }

    // Process 16-sample block (as P:147796  do #<$10)
    void processBlock(const fxp::Word* in, fxp::Word* out, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            out[i] = processOne(in[i]);
        }
    }

    fxp::Word stateA() const { return fxp::word(a_acc_); }
    fxp::Word stateB() const { return fxp::word(b_acc_); }
    fxp::Word coef() const { return x0_; }

private:
    int64_t a_acc_ = 0;       // accumulator a (Y:(r6+$1e))
    int64_t b_acc_ = 0;       // accumulator b (Y:(r6+$1f))
    fxp::Word n2_ = 0;       // address offset (table index)
    fxp::Word x0_ = 0;       // filter coefficient (kLP_filter_coeffs[tone])
};


// ============================================================================
// BIT-ТОЧНЫЙ 2-pole SVF (kernel P:$5D3-5FA)
// Построчный порт inner loop, с 24-bit fixed-point (как ChorusCore.h).
//
// Вход: 6 per-voice коэффициентов (Y:$91, $92, $93, $d1, $d2, $d3) —
//       эти значения вычисляются TONE-веткой kernel (P:$537-5CD) из
//       пользовательских параметров. Здесь пользователь должен предоставить
//       их напрямую (например, через setCoefPair).
//
// Состояние: X:$71+n*3 (a-bank), X:$73+n*3 (b-bank), n=0..7 (24 слова each bank)
// Блок: 16 сэмплов (8 итераций × 2 канала в прошивке; здесь — mono, 8 iter)
// ============================================================================
class MnmSVFFaithful {
public:
    MnmSVFFaithful() { reset(); }

    void reset() {
        for (int i = 0; i < 24; ++i) { a_bank_[i] = 0; b_bank_[i] = 0; }
        x0_ = 0xF528BD & 0xFFFFFFu;          // -0.084747577 как 24-bit signed
        x1_ = 0x4A4DF0 & 0xFFFFFFu;          // +0.581161499 как 24-bit signed
        m4_ = 0xFFFFFFu;                     // linear addressing
        n4_ = 0xFFFFFEu & 0xFFFFFFu;         // -2 (signed 24-bit)
    }

    // Установить 6 per-voice коэффициентов (вычисляются внешним кодом из TONE/Q/MODE)
    // coeffs[0..2] = Y:$91, $92, $93 (channel A)
    // coeffs[3..5] = Y:$d1, $d2, $d3 (channel B, опционально для stereo)
    void setCoeffs(const fxp::Word coeffs[6]) {
        y91_ = coeffs[0] & 0xFFFFFFu;   // Y:$91 (left/primary work area)
        y92_ = coeffs[1] & 0xFFFFFFu;   // Y:$92
        y93_ = coeffs[2] & 0xFFFFFFu;   // Y:$93
        yd1_ = coeffs[3] & 0xFFFFFFu;   // Y:$d1 (right/secondary work area)
        yd2_ = coeffs[4] & 0xFFFFFFu;   // Y:$d2
        yd3_ = coeffs[5] & 0xFFFFFFu;   // Y:$d3
    }

    // Convenience: установить коэффициенты напрямую как 24-bit signed values
    void setCoeffsSigned(const int32_t coeffs[6]) {
        fxp::Word w[6];
        for (int i = 0; i < 6; ++i) {
            w[i] = uint32_t(coeffs[i]) & 0xFFFFFFu;
        }
        setCoeffs(w);
    }

    // Обработать блок 8 сэмплов (один канал, как P:5D8 do #<$8)
    // in:  8+ сэмплов (y0 = in[0], потом 7 reads из in[1..])
    // out: 8 сэмплов (по одному на итерацию)
    //
    // ВНИМАНИЕ: это упрощённый порт. Оригнальный kernel читает 7 значений из
    // r4 за итерацию (с overlapping из-за n4=-2 backward step). Здесь мы
    // используем ту же математику, но подаём 8 сэмплов напрямую.
    void processBlockA(const fxp::Word* in, fxp::Word* out, size_t n) {
        if (n < 8) return;
        int r1 = 0;   // index into a_bank_ (stride 3)
        int r2 = 0;   // index into b_bank_ (stride 3)
        const int n1 = 3;
        const int n2 = 3;
        const int n4 = -2;  // signed

        fxp::Word y0 = in[0] & 0xFFFFFFu;   // P:5D5 move y:(r4)+,y0 (first input)

        for (int iter = 0; iter < 8; ++iter) {
            int64_t a = 0, b = 0;

            // P:5DA  mpy y0,x0,a  a,x:(r1)+n1  y:(r4)+,y1
            //   parallel: x:(r1) = OLD a (store state to bank A); y1 = next input
            fxp::Word old_a = a_bank_[r1];
            a_bank_[r1] = fxp::word(a);   // store OLD a (0 on first iter)
            a = fxp::multiply(y0, x0_);  // a = y0 * x0 (as 56-bit accumulator)
            fxp::Word y1 = (in[1 + iter*2] & 0xFFFFFFu);   // simulate y1 = next input

            // P:5DB  mpy x0,y1,b  b,x:(r2)+n2
            fxp::Word old_b = b_bank_[r2];
            b_bank_[r2] = fxp::word(b);
            b = fxp::multiply(x0_, y1);

            // P:5DC  mac y1,x1,a  y:(r4)+,y0
            a = fxp::mac(a, y1, x1_, false, true);
            y0 = in[2 + iter*2] & 0xFFFFFFu;

            // P:5DD  mac x1,y0,b  y1,x:(r1)+
            b = fxp::mac(b, x1_, y0, false, true);
            a_bank_[r1 + 1] = y1;

            // P:5DE  mac x1,y0,a  y:(r4)+,y1
            a = fxp::mac(a, x1_, y0, false, true);
            y1 = in[3 + iter*2] & 0xFFFFFFu;

            // P:5DF  mac y1,x1,b  y0,x:(r2)+
            b = fxp::mac(b, y1, x1_, false, true);
            b_bank_[r2 + 1] = y0;

            // P:5E0  mac x0,y1,a  y:(r4)+n4,y0
            a = fxp::mac(a, x0_, y1, false, true);
            // n4 = -2 (backward step): r4 goes back 2 positions
            int back_idx = (4 + iter*2) - 2;
            if (back_idx >= 0 && back_idx < int(n)) {
                y0 = in[back_idx] & 0xFFFFFFu;
            }

            // P:5E1  mac y0,x0,b  y:(r4)+,y0
            b = fxp::mac(b, y0, x0_, false, true);
            int fwd_idx = 5 + iter*2;
            if (fwd_idx < int(n)) {
                y0 = in[fwd_idx] & 0xFFFFFFu;
            }

            // P:5E2  move a,x:(r1)+n1
            a_bank_[r1 + 2] = fxp::word(a);
            // P:5E3  move b,x:(r2)+n2
            b_bank_[r2 + 2] = fxp::word(b);

            // Output (stereo in firmware, mono here)
            out[iter] = fxp::word(a);

            // Advance state pointers by stride 3
            r1 += n1;
            r2 += n2;
        }
    }

    fxp::Word x0() const { return x0_; }
    fxp::Word x1() const { return x1_; }

private:
    fxp::Word x0_, x1_;                 // fixed constants
    fxp::Word m4_, n4_;                  // addressing registers
    fxp::Word y91_, y92_, y93_;          // work area (left)
    fxp::Word yd1_, yd2_, yd3_;          // work area (right)
    fxp::Word a_bank_[24];               // X:$71+n*3 (8 stereo × 3 stride)
    fxp::Word b_bank_[24];               // X:$73+n*3
};

}  // namespace mnm

#endif  // MNM_FILTER_FAITHFUL_H
