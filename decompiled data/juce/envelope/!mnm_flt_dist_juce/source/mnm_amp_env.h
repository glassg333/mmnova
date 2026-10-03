// ============================================================================
// mnm_amp_env.h — AMP-ОГИБАЮЩАЯ AHDR (громкость-гейт выходящего звука)
//                 Monomachine OS 1.32B
// ----------------------------------------------------------------------------
// Декомпиляция P:$04A8-$04F5 ядра DSP1 (func_0004A8, блок-16 тик). Выписка:
// reference/kernel_P04A8-0536_amp_env_filterenv.txt. Дополнительно исполнено
// на эмуляторе (MnMAmpDistCORE_VERIFIED.h): attack "+0.5/блок при ATK=0",
// насыщение на 1.0, сустейн = (параметр)², decay/release МУЛЬТИПЛИКАТИВНЫЕ
// таблицей kEnvShapeB (Y:$141880).
//
// Машина состояний (r6 = V-$28):
//   state 1 (ATK) [P:$04B4-04C0]: level += kEnvShapeA[(V-$10)>>16]
//                 глобальный Y:$4FF; перенос → state 3/4 (переход)
//   state 4 (DEC) [P:$04C9-04DB]: level = level·|kEnvShapeB[(V-$11)>>16]|
//                 пока level > (V-$12)²; иначе → state 5
//   state 5 (SUS) [P:$04DE-04E2]: level = (V-$12)²  (константа)
//   state 2 (REL) [P:$04E5-04F0]: level = level·|kEnvShapeB[(V-$13)>>16]|
//                 (release МУЛЬТИПЛИКАТИВНЫЙ — ERRATA п.4)
//   default       [P:$04F1-04F3]: level = $7FFFFF (полный уровень)
//   ХОЛД: стадия HOLD описана во втором источнике (P:$088E-$08D7,
//   svf_recursion_proven.md §"стадия 1 HOLD: phase += 1; если
//   phase > HOLD·TEMPO·$791FD0·2 → стадия 2") — включена как реконструкция
//   с пометкой (точная связь с счётчиком блоков — OPEN).
//
// РОЛЬ (подтверждено владельцем машины): единственная «основная» огибающая
// секции AMP — гейтит громкость всего выходящего звука трека. НЕ ПУТАТЬ
// с огибающей фильтра (mnm_filter_env.h) и с огибающими FM-машин (freq 1/2/3).
// ============================================================================
#pragma once
#include <cmath>
#include <algorithm>
#include "mnm_fixed.h"
#include "mnm_tables_data.h"

namespace mnm {

class MnmAmpEnv
{
public:
    // параметры страницы AMP: 0..127; слова = knob<<16
    void setParams(uint32_t atkWord, uint32_t holdWord,
                   uint32_t decWord, uint32_t relWord,
                   uint32_t susWord) noexcept
    {
        atk_  = (atkWord  >> 16) & 0x7F;
        hold_ = (holdWord >> 16) & 0x7F;
        dec_  = (decWord  >> 16) & 0x7F;
        rel_  = (relWord  >> 16) & 0x7F;
        sus_  = mpyA1((int32_t)(susWord & 0xFFFFFFu), (int32_t)(susWord & 0xFFFFFFu)); // (V-$12)²
        (void)hold_;   // HOLD-стадия — реконструкция (см. шапку); держим счётчик через setHoldBlocks
    }

    void trigger() noexcept            // нота ON
    {
        state_ = 1;                    // ATTACK
        level_ = 0;
        holdCnt_ = 0;
    }
    void release() noexcept { if (state_ != 0) state_ = 2; }   // нота OFF → REL

    bool  active() const noexcept { return state_ != 0; }
    float level()  const noexcept { return q23ToF((uint32_t)level_ & 0xFFFFFFu); }

    // тик раз в блок 16 кадров (сетка прошивки)
    void tickBlock() noexcept
    {
        switch (state_)
        {
        case 1: {  // ATTACK: level += kEnvShapeA[atk] (насыщение 1.0)
            const int64_t sum = level_ + (int64_t)(kEnvShapeA[atk_] & 0xFFFFFFu);
            if (sum >= (1ll << 24)) { level_ = Q23_ONE; state_ = 3; }   // → HOLD
            else level_ = (int32_t)sum;
            break; }
        case 3: {  // HOLD (реконструкция по P:$088E-$08D7): счётчик блоков
            if (++holdCnt_ >= holdBlocks_) state_ = 4;
            break; }
        case 4: {  // DECAY: level = level·|kEnvShapeB[dec]| до сустейна
            const int32_t m = (int32_t)(kEnvShapeB[dec_] & 0xFFFFFFu);
            const int64_t p = level_ * (int64_t)(m < 0 ? -m : m);
            const int32_t next = sat24(p >> 22);
            if (next <= sus_) { level_ = sus_; state_ = 5; }
            else level_ = next;
            break; }
        case 5:    // SUSTAIN: level = (V-$12)²
            level_ = sus_;
            break;
        case 2: {  // RELEASE: level = level·|kEnvShapeB[rel]| (мультипликативно)
            const int32_t m = (int32_t)(kEnvShapeB[rel_] & 0xFFFFFFu);
            const int64_t p = level_ * (int64_t)(m < 0 ? -m : m);
            level_ = sat24(p >> 22);
            break; }
        default:   // idle: уровень полный (гейт открыт) — P:$04F1-04F3
            level_ = Q23_ONE;
            break;
        }
    }

    void setHoldBlocks(int blocks) noexcept { holdBlocks_ = blocks; }
    void reset() noexcept { state_ = 0; level_ = Q23_ONE; holdCnt_ = 0; }

private:
    static inline int32_t mpyA1(int32_t x, int32_t y) noexcept
    {
        return (int32_t)(((int64_t)x * (int64_t)y * 2) >> 24);
    }
    int    state_ = 0;      // 0=idle 1=ATK 2=REL 3=HOLD 4=DEC 5=SUS (нумерация ядра)
    int32_t level_ = Q23_ONE;
    int32_t sus_   = 0;
    int    holdCnt_ = 0, holdBlocks_ = 1;
    int    atk_ = 0, hold_ = 0, dec_ = 64, rel_ = 64;
};

} // namespace mnm
