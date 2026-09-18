#pragma once
#include "MnMVoicePage.h"

// ============================================================================
//  Огибающая и модулятор фильтра — P:$0506..$056C в func_0004A8.
//
//  В KERNEL_VOICE_CHAIN.md этот кусок был ошибочно назван "LFO-фаза".
//  parameterPageOffset доказал, что читаемые им адреса Y:$508..$50F —
//  это целиком страница FILT:
//
//    Y:$508 (V-$20) = FilterBase        Y:$50C (V-$1C) = FilterAttack
//    Y:$509 (V-$1F) = FilterWidth       Y:$50D (V-$1B) = FilterDecay
//    Y:$50A (V-$1E) = FilterHighPassQ   Y:$50E (V-$1A) = FilterBaseOffset
//    Y:$50B (V-$1D) = FilterLowPassQ    Y:$50F (V-$19) = FilterWidthOffset
// ============================================================================

namespace mmnova {

class FilterEnvelopeMod
{
public:
    static constexpr s24 kKnee_791FD0 = 0x791FD0;   // 0.9462824  (P:$51x)

    void reset() noexcept { phase = 0; state = 0; cycleDone = false; }

    // P:$04FF-0505: Y:(V-$08)==1 (note-on) сбрасывает фазу
    void triggerNoteOn() noexcept { phase = 0; state = 0; cycleDone = false; }

    // ------------------------------------------------------------------
    //  P:$0506-0536 — шаг на один 16-сэмпловый блок.
    //  rateTbl_141800 = Y:$141800 (128 слов, общая с AMP ATK)
    //  multTbl_141A00 = Y:$141A00 (128 слов)
    //  [ТИР A по структуре, ТИР C по содержимому двух таблиц пока не задамплены]
    // ------------------------------------------------------------------
    s24 tickBlock (const s24* rateTbl_141800, const s24* multTbl_141A00,
                   uint8_t filtAtk7, uint8_t filtDec7) noexcept
    {
        if (state == 0)   // ATTACK: phase += TBL[$141800 + (V-$1C)>>16]
        {
            const s56 next = static_cast<s56>(phase) + rateTbl_141800[filtAtk7 & 0x7F];
            if (next >= kOne24) { phase = kOne24; state = 2; cycleDone = true; }
            else                { phase = static_cast<s24>(next); }
        }
        else              // DECAY:  phase -= TBL[$141A00 + ((V-$1B)+$7FFF)>>16]
        {
            const s56 next = static_cast<s56>(phase) - multTbl_141A00[filtDec7 & 0x7F];
            phase = (next > 0) ? static_cast<s24>(next) : 0;
        }
        return phase;     // 0 .. $7FFFFF
    }

    // ------------------------------------------------------------------
    //  P:$053E-056C — модуль глубины BOFS / WOFS.
    //  В памяти лежит (val7 << 16), нейтраль 64 = $400000.
    //  В 24-битной арифметике $C00000 = -$400000, поэтому инструкция
    //    add #>$C00000,a ; abs a ; mpyi #>$700,...
    //  считает |param - $400000| * $700, а знак определяет направление.
    // ------------------------------------------------------------------
    static int32_t offsetDelta (uint8_t ofsKnob7, s24 envLevel) noexcept
    {
        const s24 word = paramWord (ofsKnob7);            // 0..$7F0000
        const int32_t centered = word - 0x400000;         // +$C00000 в 24 бит
        const int32_t mag = (std::abs (centered) * 0x700) >> 16;
        const int32_t mod = fmul (mag, envLevel);
        return (centered < 0) ? -mod : mod;
    }

    // P:$0537-053D: (Y:(V-$20)) * $800 - $80
    static int32_t baseTerm (uint8_t baseKnob7) noexcept
    {
        return (paramWord (baseKnob7) >> 5) - 0x80;       // *$800 в Q23
    }

private:
    s24  phase     { 0 };      // X:(r7-$1)
    int  state     { 0 };      // 0=atk, 1=knee, 2=dec
    bool cycleDone { false };  // r7-$2
};

} // namespace mmnova
