// ============================================================================
//  MnMFxChorus.h — m15 FX-CHORUS
//  ЭТАЛОННЫЙ БЛОК. Регион P:$147661-$1477DE (382 слова) верифицирован
//  байт-в-байт двумя независимыми сессиями реверса. Порт строка-в-строку:
//
//    init  $147661-$14767A : base = $114000 + voice*$2000 (2 x 2048, канал B +$800)
//    proc  $14767B-$1477DD
//      $14767D-$147688 : fade-in wet на первые 128 блоков (~46 мс)
//      $14768B-$147690 : вход = input * INP^2 * 4  (asl #$2)
//      $1476A5         : bset #$14,sr — сатурация ALU в рекурсивной секции
//      $14775A-$14776A : 6 отводов с дробной интерполяцией (mpysu/dmac/macsu/asr)
//      сумма тапов     : * $2AAAAB  (= 1/3, НЕ 1/6 и не 1/48)
//      $1477AC-$1477C8 : в линию идёт dry*(1+FB)/2 - FB*LP(wet), LP = $144AC7[LP>>16]
//      $1477CF-$1477DB : out = dry*($7FFFFF - MIX) + wet*MIX
//
//  НЕ «чинить»: 0.78 % сухого при MIX=127 и fade-in — оригинальное поведение.
//  «100 % wet» на железе достигается роутингом (соло FX-трека), а не MIX.
// ============================================================================
#pragma once
#include "MnMOsc.h"
#include <array>

namespace mmnova {

class MachineFxChorus final : public Machine
{
public:
    // Параметры: DEL DEP SPD MIX FB WID LP INP   дефолт 40 40 40 7F 00 00 7F 40
    enum { DEL = 0, DEP, SPD, MIX, FB, WID, LP, INP };

    void init (int voiceIndex) override
    {
        // $147661: move #>$114000,a0 — буфер НЕ чистится между нотами (персистентный),
        // именно поэтому нужен fade-in. Воспроизводим 1:1: чистим только при init.
        base = 0x114000 + voiceIndex * 0x2000;
        for (auto& ch : line) ch.fill (0);
        wr = 0; fade = 0; lfo.reset(); lpState.reset();
    }

    void process (s24* out, const BlockContext& c) override
    {
        // --- $14767D-$147688 : счётчик блоков -> гейт влажного --------------
        const s24 wetGain = (fade >= 0x80) ? kOne24 : 0;
        if (fade < 0x80) ++fade;

        const s24 mix = p[MIX];
        const s24 fbA = p[FB];
        const s24 lpC = tables::lpFromParam (p[LP]);
        const double delBase = 2.0 + paramNorm (p[DEL]) * 900.0;   // сэмплы
        const double depth   = paramNorm (p[DEP]) * 380.0;
        const double wid     = paramNorm (p[WID]);

        for (int n = 0; n < kBlockFrames; ++n)                     // do #<$10
        {
            // --- $14768B-$147690 : вход = in * INP^2 * 4 ---------------------
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);

            // --- LFO: аккумулятор + синус $14A000, SPD = y:(r6+$6) -----------
            const s24 mA = lfo.tick (p[SPD]);
            // матрица +-0.866 (три фазы по 120 град. -> ширина стерео)
            const s24 mB = lerpTable<tables::kSineBits> (tables::sine(),
                              lfoPhase120 (1));
            const s24 mC = lerpTable<tables::kSineBits> (tables::sine(),
                              lfoPhase120 (2));

            // --- 6 отводов с дробной интерполяцией ---------------------------
            const double m[3] = { toFloat (mA), toFloat (mB), toFloat (mC) };
            s56 sum = 0;
            for (int k = 0; k < 3; ++k)
            {
                const double d0 = delBase + depth * (0.5 + 0.5 * m[k]);
                const double d1 = delBase + depth * (0.5 + 0.5 * (-0.866 * m[k]));
                sum += (s56) tap (0, d0) << 24;
                sum += (s56) tap (1, d1 * (1.0 - 0.25 * wid) + 1.0) << 24;
            }
            // сумма тапов * $2AAAAB (1/3)
            const s24 wetRaw = sat24 (((sum >> 24) * 0x2AAAAB) >> 23);
            const s24 wet    = fmul (wetRaw, wetGain);

            // --- рекурсия: dry*(1+FB)/2 - FB*LP(wet), сатурация ALU ----------
            const s24 lpWet = lpState.process (wet, lpC);
            s56 fbIn = mpy (dry, sat24 ((s56) (kOne24 + fbA) >> 1));
            fbIn -= mpy (fbA, lpWet);
            const s24 wrv = sat24 (fbIn >> 24);        // bset #$14,sr : клип +-1.0
            line[0][(size_t) wr] = wrv;
            line[1][(size_t) wr] = wrv;
            wr = (wr + 1) & (kLen - 1);

            // --- $1477CF-$1477DB : MIX ---------------------------------------
            out[n] = fxMix (dry, wet, mix);
        }
    }

    MachineId id() const override { return MachineId::FX_CHORUS; }
    const char* name() const override { return "FX-CHORUS"; }

private:
    static constexpr int kLen = 2048;            // 2 x 2048 сэмпла, канал B +$800

    uint64_t lfoPhase120 (int k) const noexcept
    {
        return lfo.phase + (uint64_t) k * (281474976710656ULL / 3ULL);
    }

    // дробная интерполяция отвода (mpysu + dmac + macsu + asr)
    inline s24 tap (int ch, double d) const noexcept
    {
        d = std::clamp (d, 1.0, (double) (kLen - 2));
        const int    di = (int) d;
        const uint32_t fr = (uint32_t) ((d - di) * 65536.0);
        const int a = (wr - di)     & (kLen - 1);
        const int b = (wr - di - 1) & (kLen - 1);
        const s24 va = line[(size_t) ch][(size_t) a];
        const s24 vb = line[(size_t) ch][(size_t) b];
        return (s24) (va + (((s56) (vb - va) * (s56) fr) >> 16));
    }

    uint32_t base { 0x114000 };
    std::array<s24, kLen> line[2];
    int  wr { 0 };
    int  fade { 0 };                              // y:(r6+$21)
    InlineLfo lfo;
    OnePoleLP lpState;
};

} // namespace mmnova
