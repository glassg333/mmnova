// =============================================================================
// MnmEnv2.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B, DSP1
// env2 — конвертная машина КАДРА ГОЛОСА, P:$04A8-$04F4.
//
// ЧТО ЭТО: AD(S)-машина, управляющая глубиной модуляции тапов ДЕЛЭЯ.
// Выход Y:$4FF читается ровно в одном месте кадра — $0B1E-$0B22:
//     рампы Y:$10-$1F <- (DWID)^2 * (env2)^2
// НИ К КАКОМУ ФИЛЬТРУ env2 НЕ ПОДКЛЮЧЕНА: срез (стадия 1, BASE/WDTH/HPQ/LPQ)
// и блок резонанса фильтра (стадия 2, ATK/DEC/BOFS/WOFS, $0A5D-$0AD0)
// слов Y:$4FF не читают. Второго фильтра в треке не существует.
//
// ЯЧЕЙКИ (r6 = $400 в хвосте кадра; адреса — комментарии-обоснование):
//   X:$400  фаза (пишется машиной; 0=полный уровень, 1=attack, 2=release,
//           4=decay, 5=sustain, иное=полный уровень)
//   Y:$418  скорость атаки    (ручка<<16): шаг за кадр =  SHAPE_A[word>>16]
//   Y:$419  скорость спада    (ручка<<16): множитель = -SHAPE_B[word>>16]
//   Y:$41A  уровень сустейна  (слово): выход фазы 5 = word^2
//   Y:$41B  скорость релиза   (ручка<<16): множитель = -SHAPE_B[word>>16]
//   Y:$421  гейт-форсаж фазы: хост пишет номер фазы -> машина прыгает в неё
//           на 1 кадр, затем ячейка самоочищается ($04AD-$04B1)
//   Y:$4FF  уровень (выход); по умолчанию $7FFFFF = 1.0
//   Y:$124  слот машины кадра: 0 = слот синт-машины (env2 исполняется),
//           не 0 = FX-слот ($04A8-$04AB: вся машина пропускается)
//
// Поведение по фазам ($04B2-$04F3):
//   1 attack : level += SHAPE_A[atk>>16]; при выходе за 24 бита (флаг E,
//              bes $04BD) -> level = a1 (может быть $800000!), фаза = 4
//   4 decay  : level' = level * (-SHAPE_B[dec>>16]); пока level' > sust^2
//              (cmp b,a / ble $04D4, сравнение полных 48-бит произведений)
//              level = level', иначе фаза = 5 (без записи $4FF в этот кадр)
//   5 sustain: level = sust^2  (mpy y0,y0 / $04DE-$04E1)
//   2 release: level = level * (-SHAPE_B[rel>>16]) — без терминала
//   иное     : level = $7FFFFF ($04F1-$04F3)
//
// Порт пословно транскрибирован и верифицирован против эмулятора OS 1.32B:
// vectors/env2_vectors.txt (489 кейсов / 2040 кадров) — tools/test_env2.cpp.
// Арифметика: DSP56300, аккумулятор 56 бит; 24-битный операнд в ALU
// выравнивается к A1 (sext24<<24); MPY: произведение signed 24x24 <<1;
// запись в память = A1 (биты [47..24]); E = A2 != знак-расширение A1[23].
// =============================================================================
#ifndef MNM_ENV2_HPP
#define MNM_ENV2_HPP

#include "MnmEnv2Tables.h"
#include <cstdint>

namespace mnmenv2 {

static inline int32_t sgn24(uint32_t v) {
    v &= 0xFFFFFFu;
    return (int32_t)(v ^ 0x800000u) - 0x800000;
}

struct MnmEnv2 {
    uint32_t phase = 0;   // X:$400
    uint32_t atk   = 0;   // Y:$418
    uint32_t dec   = 0;   // Y:$419
    uint32_t sust  = 0;   // Y:$41A
    uint32_t rel   = 0;   // Y:$41B
    uint32_t gate  = 0;   // Y:$421 (one-shot)
    uint32_t level = 0x7FFFFF; // Y:$4FF
    uint32_t slot  = 0;   // Y:$124

    // Data Limiter (DSP56300 FM 5.4.1.2): перенос A/B -> память сатурируется
    // в $7FFFFF / $800000 при выходе за 24-битный диапазон (иначе A1).
    static uint32_t store_acc(int64_t acc) {
        if (acc > 0x007FFFFFFFFFFFLL) return 0x7FFFFF;
        if (acc < -0x00800000000000LL) return 0x800000;
        return (uint32_t)((acc >> 24) & 0xFFFFFF);
    }

    // E-flag: A2 != sign-extension of A1 MSB (dsp56300, 56-bit accumulator)
    static bool flag_e(int64_t acc) {
        const int a1msb = (int)((acc >> 47) & 1);
        const uint32_t a2 = (uint32_t)((acc >> 48) & 0xFF);
        return a2 != (a1msb ? 0xFFu : 0x00u);
    }

    void frame() {
        // $04A8-$04AB: skip for non-synth machine slots
        if (slot != 0) return;

        // $04AC-$04B1: a = x:(r6+$0); b = y:(r6+$21); if b!=0 a = b;
        //              y:(r6+$21) = 0 (unconditional); x:(r6+$0) = a
        {
            int64_t A = (int64_t)sgn24(phase) << 24;      // move x:(r6+$0),a
            const uint32_t b = gate & 0xFFFFFF;           // move y:(r6+$21),b
            if (b != 0) A = (int64_t)sgn24(b) << 24;      // tfr b,a ifne
            gate = 0;                                     // move x0,y:(r6+$21) (x0=0)
            phase = store_acc(A);                         // move a,x:(r6+$0) (limiter)
        }

        const uint32_t ph = phase & 0xFFFFFF;

        if (ph == 1) {
            // ---- $04B4-$04C6: attack ------------------------------------
            const uint32_t idx = (atk >> 16) & 0xFFFFFF;  // asr #$10 (knob 0..127)
            const int64_t  step = (int64_t)sgn24(SHAPE_A[idx & 127]) << 24;
            const int64_t  A = ((int64_t)sgn24(level) << 24) + step; // add y0,a
            level = store_acc(A);                         // move a,y:>$4ff (both paths, limiter)
            if (flag_e(A)) phase = 4;                     // bes $04C1 -> phase 4
            return;                                       // bra $04F5
        }
        if (ph == 4) {
            // ---- $04C9-$04DB: decay towards sust^2 -----------------------
            const int64_t Bp = ((int64_t)sgn24(sust) * (int64_t)sgn24(sust)) << 1;
            const uint32_t idx = (dec >> 16) & 0xFFFFFF;
            // mpy -x1,y0: negation happens in 24-bit two's complement
            // (-sgn24(level)) & $FFFFFF, then sign-extended back; at
            // level = $800000 the negation wraps (double negative).
            const int32_t nlevel = sgn24((uint32_t)(-(int64_t)sgn24(level)) & 0xFFFFFFu);
            const int64_t  Ap = ((int64_t)nlevel *
                                 (int64_t)sgn24(SHAPE_B[idx & 127])) << 1;
            // cmp b,a -> A - B; ble -> taken iff A <= B (n != v || z)
            const int64_t sub = Ap - Bp;
            const bool n = sub < 0, z = sub == 0;
            const bool sx = Ap < 0, sy = Bp < 0, sr = sub < 0;
            const bool v = (sx != sy) && (sr != sx);
            if (n != v || z) { phase = 5; return; }       // $04D8-$04DB (no write)
            level = store_acc(Ap);                        // $04D5-$04D6 (limiter)
            return;
        }
        if (ph == 5) {
            // ---- $04DE-$04E2: sustain = sust^2 ---------------------------
            const int64_t A = ((int64_t)sgn24(sust) * (int64_t)sgn24(sust)) << 1;
            level = store_acc(A);                         // move a,y:>$4ff (limiter)
            return;
        }
        if (ph == 2) {
            // ---- $04E5-$04EF: release (same shape table as decay) --------
            const uint32_t idx = (rel >> 16) & 0xFFFFFF;
            // mpy -x1,y0: negation happens in 24-bit two's complement
            // (-sgn24(level)) & $FFFFFF, then sign-extended back; at
            // level = $800000 the negation wraps (double negative).
            const int32_t nlevel = sgn24((uint32_t)(-(int64_t)sgn24(level)) & 0xFFFFFFu);
            const int64_t  Ap = ((int64_t)nlevel *
                                 (int64_t)sgn24(SHAPE_B[idx & 127])) << 1;
            level = store_acc(Ap);                        // move a,y:>$4ff (limiter)
            return;
        }
        // ---- $04F1-$04F3: default — full level --------------------------
        level = 0x7FFFFF;
    }
};

} // namespace mnmenv2

#endif // MNM_ENV2_HPP
