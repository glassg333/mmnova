#pragma once
#include "MnMFixed.h"

// ============================================================================
//  m1 GND-SIN — P:$144CC9..$144D4D (159 слов). Листинг прочитан ЦЕЛИКОМ.
//
//  144cc9: move #>$14a000,x0        ; база синус-таблицы
//  144cd0: move #>$17c6f9,x0        ; масштаб инкремента = 0.18624806
//  144cd4: mpysu x0,y0,a
//  144cd5: dmac ss x0,y1,a          ; 48-битное умножение питча
//  144cdb: asr #$4,a,a
//  144ce3: move #>$1fff,x0          ; маска фазы 8192
//  144ce7: do #<$10                 ; 16-кадровый блок
//  144cfb: mpysu -x1,y0,a / add x1,a / macsu x0,y0,a / asr a  ; интерполяция
//
//  Пост-фильтр P:$144D21..$144D4C (тоже прочитан целиком):
//  144d29: move #>$1e454e,x1        ; C1 = 0.23649215
//  144d2b: move #>$5b75b8,y0        ; C2 = 0.71452600
//  144d33: do #<$10
//  144d35: mac  y0,x0,a  ...        ; a += C2*x
//  144d36: mac -y1,y0,a  ...        ; a -= s1*C2
//  144d37: mac  x1,x0,b  ...        ; b += C1*x
//  144d38: mac -y1,x1,b  ...        ; b -= s1*C1
// ============================================================================

namespace mmnova {

// Литеральный 2-полюсник из хвоста GND-SIN. Используется как характерный
// сглаживатель осцилляторных машин.
class TwoPoleResonator_P144D21
{
public:
    static constexpr s24 kC1_1E454E = 0x1E454E; // 0.23649215
    static constexpr s24 kC2_5B75B8 = 0x5B75B8; // 0.71452600

    void reset() noexcept { s1 = 0; s2 = 0; }

    inline s24 process (s24 x) noexcept
    {
        const s24 a = sat24 ((mpy (kC2_5B75B8, x) - mpy (s1, kC2_5B75B8)) >> 24);
        const s24 b = sat24 (((mpy (kC1_1E454E, x) - mpy (s1, kC1_1E454E)) >> 24) + s2);
        s1 = a;
        s2 = b;
        return b;
    }
private:
    s24 s1 { 0 }, s2 { 0 };
};

// GND-SIN. sineTable = 8192 записи (X:$14A000, считается ColdFire в рантайме,
// в прошивке её нет — генерится идентично как sin(2*pi*i/8192)).
class GndSin
{
public:
    static constexpr s24 kScale_17C6F9 = 0x17C6F9; // 0.18624806

    void init() noexcept { phase = 0; post.reset(); }

    // pitchInc48 — 48-битный инкремент из pre-voice P:$262
    void processBlock16 (s24* out, const s24* sineTable, uint64_t pitchInc48) noexcept
    {
        const uint64_t inc = (uint64_t) ((double) pitchInc48 * toFloat (kScale_17C6F9));
        for (int i = 0; i < 16; ++i)          // do #<$10
        {
            const s24 raw = lerpTable<13> (sineTable, phase);   // маска $1FFF
            phase += inc;
            out[i] = post.process (raw);                         // P:$144D21
        }
    }
private:
    uint64_t phase { 0 };
    TwoPoleResonator_P144D21 post;
};

} // namespace mmnova
