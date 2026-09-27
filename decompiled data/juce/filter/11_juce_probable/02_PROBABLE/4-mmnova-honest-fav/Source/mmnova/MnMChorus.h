#pragma once
#include "MnMFixed.h"
#include <array>

// ============================================================================
//  m15 FX-CHORUS — P:$147661..$1477DE (382 слова).
//  Единственный регион, верифицированный байт-в-байт двумя независимыми
//  сессиями реверса. Прочитан целиком.
//
//  init  $147661-$14767A : буфер X:$114000 + voice*$2000 (НЕ чистится между нотами)
//  proc  $14767B-$1477DD
//    $14767D-$147688 : cmp #>$80,a / bge / add #>$1,a / clr b
//                      -> wet замьючен первые 128 блоков (~46 мс).
//                      Это НЕ источник сухого остатка, это маскировка
//                      мусора в персистентном буфере. Не удалять.
//    $14768B-$147690 : mpy x0,x0,a / mpy x1,x0,a / asl #$2,a,a
//                      -> вход = in * INP^2 * 4. При INP=$40 ровно x1.0
//    $147699-$1476A2 : mpyi #>$956,x0,a  — шаг LFO из SPD^2
//    $1476A5         : bset #$14,sr — сатурация в рекурсивном модуляторе
//    $1476AD-$1476B4 : maci #>$28f5c / maci #>$fd70a4 — сглаживание WID
//    $1476B9/$1476BB : чтение X:$14A000 и X:$14A800 (width-таблицы)
//    сумма тапов     : * $2AAAAB (= 1/3, НЕ 1/6)
//    $1477AC-$1477C8 : в линию идёт dry*(1+FB)/2 - FB*LP(wet), LP = $144AC7
//    $1477CF-$1477DB : out = dry*($7FFFFF - MIX) + wet*MIX
//                      при MIX=127 остаётся 0.78% сухого — так на железе.
//                      "100% wet" достигается роутингом (соло FX-трека).
// ============================================================================

namespace mmnova {

class FxChorus
{
public:
    enum { DEL = 0, DEP, SPD, MIX, FB, WID, LP, INP };

    void init (int voiceIndex) noexcept
    {
        base = 0x114000 + voiceIndex * 0x2000;
        for (auto& ch : line) ch.fill (0);
        wr = 0;
        fade = 0;          // y:(r6+$21)
        widSmooth = 0;     // y:(r6+$24)
        cosZ = kOne24;     // x:(r6+$11)
        sinZ = 0;          // x:(r6+$12)
        lp.reset();
    }

    // p[] — 8 параметров как (val7 << 16). in — выход соседнего трека.
    // lpCoeff — значение из таблицы P:$144AC7 по индексу p[LP]>>16.
    void processBlock16 (s24* out, const s24* in, const s24* p, s24 lpCoeff) noexcept
    {
        // $14767D: гейт влажного на первые 128 блоков
        const s24 wetGain = (fade >= 0x80) ? kOne24 : 0;
        if (fade < 0x80) ++fade;

        const double del = 16.0 + paramNorm (p[DEL]) * 700.0;
        const double dep = paramNorm (p[DEP]) * 180.0;

        // $1476AD: maci #>$28F5C,y0,a ; maci #>$FD70A4,y1,a
        widSmooth = sat24 ((((s56) 0x028F5C * p[WID]) + ((s56) 0x7D70A4 * widSmooth)) >> 23);
        const double wid = toFloat (widSmooth);

        // $147699: mpy x0,x0,a ; mpyi #>$956,x0,a
        const s24 spdSq = fmul (p[SPD], p[SPD]);
        const s24 step  = sat24 (((s56) spdSq * 0x0956) >> 12);

        const s24 fbA = p[FB];
        const s24 mix = p[MIX];

        for (int n = 0; n < 16; ++n)                    // do #<$10
        {
            // $14768B: in * INP^2 * 4
            const s24 inpSq = fmul (p[INP], p[INP]);
            const s24 dry   = sat24 ((s56) fmul (in ? in[n] : 0, inpSq) << 2);

            // $1476A5: bset #$14,sr — рекурсивный квадратурный модулятор
            sinZ = sat24 ((s56) sinZ - fmul (cosZ, step));
            cosZ = sat24 ((s56) cosZ + fmul (sinZ, step));
            // (bclr #$14,sr)

            const double m0 = toFloat (sinZ);
            const double m1 = -0.5 * m0 + 0.86602540 * toFloat (cosZ);
            const double m2 = -0.5 * m0 - 0.86602540 * toFloat (cosZ);
            const double mm[3] = { m0, m1, m2 };

            // 6 отводов с дробной интерполяцией ($14775A-$14776A)
            s56 sum = 0;
            for (int k = 0; k < 3; ++k)
            {
                sum += (s56) tap (0, del + dep * (1.0 + mm[k]))        << 24;
                sum += (s56) tap (1, del + dep * (1.0 - mm[k] * wid))  << 24;
            }
            // сумма * $2AAAAB = 1/3
            const s24 wetRaw = sat24 (((sum >> 24) * 0x2AAAABLL) >> 23);
            const s24 wet    = fmul (wetRaw, wetGain);

            // $1477AC: dry*(1+FB)/2 - FB*LP(wet), клип SR.SM
            const s24 lpWet = lp.process (wet, lpCoeff);
            s56 fbIn = mpy (dry, sat24 ((s56)(kOne24 + fbA) >> 1));
            fbIn    -= mpy (fbA, lpWet);
            const s24 wrv = sat24 (fbIn >> 24);
            line[0][(size_t) wr] = wrv;
            line[1][(size_t) wr] = wrv;
            wr = (wr + 1) & (kLen - 1);

            // $1477CF: out = dry*($7FFFFF - MIX) + wet*MIX
            const s24 dryGain = sat24 ((s56) kOne24 - mix);
            out[n] = sat24 ((mpy (dry, dryGain) + mpy (wet, mix)) >> 24);
        }
    }

private:
    static constexpr int kLen = 2048;   // 2 x 2048, канал B +$800

    inline s24 tap (int ch, double d) const noexcept
    {
        d = std::clamp (d, 1.0, (double)(kLen - 2));
        const int di = (int) d;
        const uint32_t fr = (uint32_t) ((d - di) * 65536.0);
        const int a = (wr - di) & (kLen - 1);
        const int b = (wr - di - 1) & (kLen - 1);
        const s24 va = line[(size_t) ch][(size_t) a];
        const s24 vb = line[(size_t) ch][(size_t) b];
        return (s24) (va + (((s56)(vb - va) * (s56) fr) >> 16));
    }

    uint32_t base { 0x114000 };
    std::array<s24, kLen> line[2];
    int wr { 0 }, fade { 0 };
    s24 widSmooth { 0 }, cosZ { kOne24 }, sinZ { 0 };
    OnePoleLP lp;
};

} // namespace mmnova
