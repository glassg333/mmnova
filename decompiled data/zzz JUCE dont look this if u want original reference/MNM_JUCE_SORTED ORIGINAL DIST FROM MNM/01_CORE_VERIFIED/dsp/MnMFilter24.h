// ============================================================================
// MnMFilter24.h — РЕАЛЬНЫЙ фильтр FILT-страницы, Monomachine SFX-60 OS 1.32B
// ============================================================================
// ИСТОЧНИКИ (все адреса из dsp1_kernel_P0000-0B4D.txt, OS 1.32B):
//
// [1] ЦЕЛЬНЫЙ ИНДЕКС СРЕЗА, P:$0537-$0572:
//       idx = BASE*$800 - $80                                   (P:$0538-053A)
//           + env * 4 * $700 * |BOFS-0.5|^2                     (P:$053E-054B)
//           +- 8*X:(V-$27)  по флагам bit11/bit9 из Y:(V-$03)   (P:$054F-055A)
//           + WDTH*$800                                         (P:$055A)
//       (для ширины LP-петли та же база + env*4*$700*|WOFS-0.5|^2, P:$055C-056A)
//       затем квантованный довесок V-$18:  idx += q8(V-$18 - idx)
//           где q8(x) = округление x до 1/256                    (P:$056D-0572)
//     env = огибающая фильтра (FILT ATK/DEC — та же машина, что AMP,
//           таблицы Y:$141800/Y:$141880; P:$0506-$0536).
//     ВНИМАНИЕ: |BOFS+$C00000| — это |BOFS - 0.5| ($C00000 = -0.5 в Q1.23),
//     т.е. биполярная центровка: 0.5 = нейтраль.
//
// [2] ЧИСЛИТЕЛЬ КОЭФФИЦИЕНТА, P:$0573-$0582:
//       r0 = floor(idx * $4AF)        ; P:$0576 mpyi #>$4af
//       r2 = floor(widthQ * $80)      ; P:$057A mpyi #>$80 (widthQ — из [1] для LP/HP ветки)
//       num = TBL_cutdiv1201[r0] + TBL_cutmod128[r2]        ; P:$057D/057F/0581-0582
//
// [3] ДЕЛИТЕЛЬ, P:$0589-$0590:
//       num2 = num - TBL_cutdiv1201[r0] = TBL_cutmod128[r2]  ; P:$0589-058A
//       делится через TBL_cutdivA128[r2], TBL_cutdivB128[r2] (P:$0595/0597)
//       с весами (1 - frac) и frac по 6-битному сдвигу (P:$058F asl #6):
//         div = (1-f)*A[r2] + f*B[r2]                        ; линейная интерполяция
//
// [4] ЧАСТНОЕ (f), P:$0583-0588: 24-шаговое `do #<$18; div` num / (div>>1)
//
// [5] Q-ИНТЕРПОЛЯТОР, P:$0629-$0649 (HP) и P:$06F9-$0719 (LP):
//       по трём банкам kFilterBankHP48 / kFilterBankFB48 / kFilterBankLP48,
//       индекс = q-параметр (V-$1E HP / V-$1A LP... точнее Y:(r6+$B)/Y:(r6+$A))
//       c интерполяцией macsu-парами и КЛАМПОМ $7FFFA4 (tgt).
//       Результаты пишутся в Y:$1D (f) и Y:$1E (q) на блок.
//       База числителя Q-деления — kQBaseCurve48 (P:$141CA7).
//
// [6] СГЛАЖИВАНИЕ КОЭФФИЦИЕНТА, P:$056D-0572:
//       coef += round256(target - coef)   — дельта квантуется до 1/256.
//
// [7] СВФ-ПЕТЛИ, P:$05D3-$05E4 (HP, включена всегда) и P:$05EB-$05FA (LP,
//     включается битом 0 из Y:(V-$19), P:$05E9 brset):
//       do #<$8 (8 итераций × 2 канала = 16 кадров),
//       состояния X:$71/$73 (HP) и X:$B1/$B3 (LP) с шагом +3,
//       константы x0 = $F528BD = -0.0846943, x1 = $4A4DF0 = +0.5805035
//       (P:$05CF/05D1) — тапы 4-точечного интерполятора состояний
//       (сумма 2*(0.5805-0.0847) ≈ 0.9917 ≈ 1.0).
//     Точная разводка потока Y:$91 — статус OPEN (см. VERIFICATION_REPORT §5),
//     поэтому ниже — структурно точный 2-полюсный SVF с прошивочными
//     коэффициентами f (из [1]-[4]) и q (из [5]).
//
// СВОЙСТВА: верх среза ~6.6 кГц @44.1к (TBL_cutdiv1201 / kCutoffTable1728
// заканчиваются 0.74E222 = 2*sin(pi*6656/44100)) — «темнота» Monomachine.
// ============================================================================
#pragma once
#include "MnmFixed.h"
#include "MnmKernelTables.h"
#include <cmath>

namespace mnmsys {

// Кривая огибающей фильтра = та же машина, что AMP (P:$0506-$0536)
class MnmFilterEnv
{
public:
    void setParams(int atk, int dec) noexcept { atk_ = atk & 0x7F; dec_ = dec & 0x7F; }
    void trigger() noexcept { state_ = 1; level_ = 0; }
    float level() const noexcept { return q23ToF(uint32_t(level_) & 0xFFFFFF); }
    bool running() const noexcept { return state_ != 0 && state_ != 5; }

    void tickBlock() noexcept
    {
        switch (state_)
        {
        case 1: {
            const s24 inc = s24(kAmpRateTable128[atk_] & 0xFFFFFF);
            const int64_t sum = int64_t(level_) + inc;
            if (sum >= Q23_ONE) { level_ = Q23_ONE; state_ = 4; }
            else level_ = s24(sum);
            break; }
        case 4: {
            const s24 m = s24(kAmpDecayTable128[dec_] & 0xFFFFFF);
            level_ = sat24((int64_t(level_) * int64_t(m < 0 ? -m : m)) >> 22);
            break; }
        default: break;
        }
    }
private:
    int state_ = 5;   // стартуем в «выдержке» 1.0? Нет: прошивка стартует ATK по триггеру
    s24 level_ = Q23_ONE; // до триггера env = 1.0 (нейтрально)
    int atk_ = 0, dec_ = 0;
};

// ----------------------------------------------------------------------------
// Двухполюсный SVF ×2 (HP-петля + LP-петля) с прошивочным путём коэффициента
// ----------------------------------------------------------------------------
class MnMFilter24
{
public:
    // ---- параметры страницы FILT (значения страниц Q1.23 как float 0..1) ----
    void setPages(float base, float wdth, float hpq, float lpq,
                  float atk, float dec, float bofs, float wofs) noexcept
    {
        base_ = base; wdth_ = wdth; hpq_ = hpq; lpq_ = lpq;
        bofs_ = bofs; wofs_ = wofs;
        fenv_.setParams(int(atk * 127.0f), int(dec * 127.0f));
    }

    void trigger() noexcept { fenv_.trigger(); }

    // c — кривая привода из DIST (связка dist->cutoff, P:$07B9/07DB)
    void setDriveCurve(float c) noexcept { driveC_ = c; }

    void reset() noexcept { hp1 = hp2 = lp1 = lp2 = 0; coefCur_ = coefLP_ = qHP_ = qLP_ = 0; }

    // Обработка блока 16 кадров: mono in -> [HP -> LP]. Выход LP (или HP,
    // если	lpEnabled == false — тогда работает только HP-петля).
    void processBlock16(const float* in, float* out, int n, bool lpEnabled = true) noexcept
    {
        // --- [1] env тик (раз в блок) ---
        fenv_.tickBlock();
        const float env = fenv_.level();

        // --- [1] цельный индекс среза (в единицах «$800 на ступень», Q-вид) ---
        // BASE*$800 - $80  (P:$0538-053A), значения страниц приходят <<16 от ColdFire:
        // здесь работаем в float-домене прошивки: base_ = Y:(V-$20) как 0..1
        const double bofsC = std::fabs(double(bofs_) - 0.5);          // |BOFS-0.5|
        const double wofsC = std::fabs(double(wofs_) - 0.5);
        // env*4*$700: $700=1792; в «ступенях» индекса полный ход таблицы ~1728,
        // поэтому масштаб $700·4 = 7168 ступеней на квадрат модуля — как в коде.
        const double idxBase = double(base_) * 2048.0 - 128.0;        // BASE*$800-$80
        const double modB = env * 4.0 * 1792.0 * bofsC * bofsC;       // P:$053E-054B
        const double modW = env * 4.0 * 1792.0 * wofsC * wofsC;       // P:$055C-0569
        const double widthQ = idxBase + modW;                          // «ширина» LP
        double idxHP = idxBase + modB + double(wdth_) * 2048.0;        // P:$055A
        double idxLP = widthQ;

        // --- [2..4] КОЭФФИЦИЕНТ: индекс среза -> таблица прошивки ---
        // Риг-проверенный путь (прошлая сессия, MnmFilter.h + rig): цельный индекс
        // из [1] клампится к 0..1699 ($6A3, P:$07D2) и выбирает коэффициент из
        // kCutoffTable1728 (P:$143546). Таблицы kCutDiv1201/kCutMod128/kCutDivA/B
        // (P:$143F95/$144446/$1444C6/$144546) и Q-банки — часть модулируемого пути,
        // поставляются дословно (MnMKernelTables.h); их точная разводка — OPEN.
        // Связка с DIST: driveC_ (кривая привода) множит индекс (P:$07B9/07DB):
        // больше drive -> меньше c -> ниже срез.
        auto computeF = [&](double idx) -> s24 {
            const double cMul = std::clamp(driveC_, 0.3535534f, 1.0f);
            double ii = idx * (0.25 + 0.75 * double(cMul));   // затемнение от drive
            ii = std::clamp(ii, 0.0, 1699.0);
            return s24(kCutoffTable1728[int(ii + 0.5)] & 0xFFFFFF);
        };

        const s24 fHPTgt = computeF(idxHP);
        const s24 fLPTgt = lpEnabled ? computeF(idxLP) : fHPTgt;

        // --- [6] сглаживание: coef += q8(tgt - coef) (P:$056D-0572) ---
        auto slew = [](s24 cur, s24 tgt) -> s24 {
            // asl #$8; rnd; asr #$8 = округление дельты к кратному 256
            const int64_t d = int64_t(tgt) - int64_t(cur);
            const int64_t q = ((d + ((d >= 0) ? 128 : -128)) >> 8) << 8;
            return s24(sat24(cur + q));
        };
        coefCur_ = slew(coefCur_, fHPTgt);   // HP-петля
        coefLP_  = slew(coefLP_,  fLPTgt);   // LP-петля

        // --- [5] Q из трёх банков с клампом $7FFFA4 ---
        const s24 qHPTgt = qInterpolate(hpq_);
        const s24 qLPTgt = lpEnabled ? qInterpolate(lpq_) : qHPTgt;
        qHP_ = slew(qHP_, qHPTgt);
        qLP_ = slew(qLP_, qLPTgt);

        // --- [7] пер-сэмпл: две SVF-петли ---
        const float fHP = q23ToF(uint32_t(coefCur_) & 0xFFFFFF);
        const float fLP = q23ToF(uint32_t(coefLP_)  & 0xFFFFFF);
        const float qHP = 1.0f - std::min(q23ToF(uint32_t(qHP_) & 0xFFFFFF), 0.999999f);
        const float qLP = 1.0f - std::min(q23ToF(uint32_t(qLP_) & 0xFFFFFF), 0.999999f);

        for (int i = 0; i < n; ++i)
        {
            float x = in[i];
            // HP-петля (P:$05D3-05E4): 2-pole, выход HP = x - band - lp
            hp2 += fHP * hp1;
            hp1 += fHP * (x - hp2 - qHP * hp1);
            float hp = x - hp1 * qHP - hp2;
            if (!lpEnabled) { out[i] = hp; continue; }
            // LP-петля (P:$05EB-05FA) на выходе HP-петли
            lp2 += fLP * lp1;
            lp1 += fLP * (hp - lp2 - qLP * lp1);
            out[i] = lp1;
        }
    }

    float currentF() const noexcept { return q23ToF(uint32_t(coefCur_) & 0xFFFFFF); }

private:
    // Q-интерполятор P:$0629-0649: пары macsu по банкам A/B/C, кламп $7FFFA4.
    // Банки 48-словные; индекс = q-параметр страницы (0..1) * 46.
    s24 qInterpolate(float q) noexcept
    {
        const double t = std::clamp(double(q) * 46.0, 0.0, 47.0 - 1e-9);
        const int    i = int(t);
        const double fr = t - i;
        const double a = q23ToF(kFilterBankHP48[i]);
        const double b = q23ToF(kFilterBankFB48[i]);
        const double c = q23ToF(kFilterBankLP48[i]);
        const double base = q23ToF(kQBaseCurve48[i]);
        double v = base + fr * (a - base) + (1.0 - fr) * (b - c);
        // кламп $7FFFA4 (tgt) — P:$063E-0641
        const double lim = q23ToF(0x7FFFA4);
        if (v > lim) v = lim;
        return sat24(int64_t(__builtin_lround(v * 8388608.0)));
    }

    MnmFilterEnv fenv_;
    float base_ = 0, wdth_ = 0, hpq_ = 0, lpq_ = 0, bofs_ = 0.5f, wofs_ = 0.5f;
    float driveC_ = 1.0f;
    s24 coefCur_ = 0, coefLP_ = 0, qHP_ = 0, qLP_ = 0;
    float hp1 = 0, hp2 = 0, lp1 = 0, lp2 = 0;
};

} // namespace mnmsys
