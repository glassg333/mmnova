// ============================================================================
// mnm_dist_stage.h — ступень DIST Monomachine OS 1.32B, пословная транскрипция
//                     P:$079F–$07D1 ядра DSP1 (+ фактор среза $07D2–$07DB).
// ----------------------------------------------------------------------------
// Закрытие OPEN-пунктов §7: «банковая ступень DIST P:$07BD–$07D1
// (y1 = 8/X:(V−$1D))» и «кривая ColdFire ручка→слово DIST».
// Метод: exp72b/c/d на бит-точном эмуляторе dsp_emu.py (worklog Task 6);
// пины — Tests/test_sanity.cpp (сетка DIST −64..127 при D=8 + банк 34 слова).
//
// ЗАКОН (слова Q1.23; арифметика — mnm_dsp56300_math.h):
//   D  = X:$40B — ДЕЛИТЕЛЬ (сырое слово) для 24-шагового DIV: y1 = 8·2^24/D.
//        ИСПРАВЛЕНО mnm_20 (exp80/81): X:$40B НЕ дыра — константу пишет
//        init-блок СМЕНЫ МАШИНЫ: P:$00F0 move #$20,x0 → P:$00F1
//        move x0,x:(r6-$1D) при r6=$428 → X:$40B = $200000 (0.25 Q1.23).
//        Подтверждено живым прогоном свитча на оракуле (машины 1/14/19) и
//        независимо пакетом «astra» (README: default $200000 из P:$00F0-00F1).
//        Машины МОГУТ переопределить (setMachineHeadroomWord в войсе).
//        div0 возможен только если ячейку обнулили вручную — тогда a0 =
//        $FFFFFF (сатурируется, спецветка эмулятора).
//        При D=$200000: y1 = 8·2^24/$200000 = $000040 (64) — МАЛО.
//        K1 = limit24(y1·kDrive) = $000001 (≈0) — банк A на стоковой OS
//        ПРАКТИЧЕСКИ НЕМ (замер exp81: K1 = $000001 при любом DIST).
//        K2 = limit24(curve·D) = $1FFFFF при curve=$7FFFFF — банк B ЖИВ
//        на 0.25·curve (замер exp81). Старый текст «банки немы» был верен
//        только для банка A; divWord=0 в порту давал y1=$FFFFFF≈−1.0 →
//        банк A пропускал ИНВЕРТИРОВАННЫЙ полный драйв — баг исправлен.
//        «K1 = kDrive при D=16» — ОШИБКА старого комментария (верилось,
//        что y1 = frac(8/D) = $400000; на деле y1 = 128 = $000080).
//   w  = Y:$404 — слово ручки DIST (хост: v<<16, v биполярный −64..+127).
//        ColdFire-кривой НЕТ — DSP потребляет слово линейно; вся «кривая»:
//   x0 = max(0, 2·w − 1.0)   [$07A6 load; $07A7 asl; $07A8 add #$800000 —
//        это −1.0 ЗНАКОВОЕ (дизасм-ловушка!); $07AA clr ifmi]
//        → ступень активна только на верхней половине ручки (v ≥ 64).
//   r2 = B1((x0·$100)<<1)    [$07AD mpyi #$100 — дробный <<1; $07B1 move b,r2
//        — R-цель БЕЗ лимитера] = x0>>15 — «пила» (замер пака 12: заворот).
//   kDrive = limit24(x0²·$7DECCD + $021333)      [$07AF–$07B5]
//   curve  = $1447C6[r2]     (r2 ≤ $FF всегда, т.к. x0 ≤ $7FFFFF)
//   K1 = limit24(y1·kDrive)  [$07BA–$07BC] — гейн банки 1 (y1 ЗНАКОВОЕ!)
//   K2 = limit24(curve·D)    [$07B9/$07BB] — гейн банки 2
//   БАНКИ (34 слова):
//   pass1 $07BD–$07C2 (тело 34×; $07C3 bset — вне тела): конвейерные store
//        ПРЕДЫДУЩЕГО acc (параллельные перемещения пишут ДО ALU), затем
//        mpy y0,x0 (= K1·вход·2) и asl #8. store[0] = шапка = K1-acc;
//        store[i] = limit24((K1·in[i−1]·2)<<8). Аккумуляторы ПЕРЕТЕКАЮТ
//        в pass2 (пин p2[0] = B1(pass1-хвоста) — измерено).
//   pass2 $07C9–$07D1 (тело 34×; $07CF/$07D0 — вне тела): то же без asl,
//        x0 = $80, входы = результаты pass1 (Y-банки), выходы в X-банки
//        (перезаписывают вход X:$B2–$D3!).
//   $07C3/$07D1 bset/bclr #$b,sr — в семантике эмулятора-оракула no-op
//        (mpy всегда дробный <<1); порт повторяет оракула пословно.
//   ФАКТОР СРЕЗА $07D2–$07DB: c = clamp(Y:$4DA, 0, $6A3) (a ≤ $6A3, снизу
//        без клампа в коде — отрицательное Y:$4DA проходит как есть и даёт
//        чтение kCutoff[c] с отрицательным индексом! Порт: c берётся как
//        24-битное слово, таблица читается по беззнаковому c — как оракул);
//        kCut = limit24(K2·kCutoff[$143546+c]).
// ============================================================================
#pragma once
#include <cstdint>
#include <array>
#include "mnm_dsp56300_math.h"
#include "MnmDistTables.h"        // kDistCurve[256] = P:$1447C6
#include "mnm_firmware_tables.h"  // kCutoff[1728] = P:$143546

namespace mnm {

class MnmDistStage
{
public:
    static constexpr int kBank = 34;

    struct In
    {
        uint32_t distWord;   // Y:$404 — слово ручки DIST (знаковое v<<16)
        uint32_t divWord;    // X:$40B — делитель 8·2^24/D (OS: $200000, init $00F1)
        uint32_t wofsWord;   // Y:$4DA — слово WOFS-ветки (фактор среза)
    };

    struct Out
    {
        std::array<uint32_t, kBank> bankA{};   // X:$B0–$D3 (после pass2)
        std::array<uint32_t, kBank> bankB{};   // X:$72–$93 (после pass2)
        // скалярные пины (тесты/диагностика)
        uint32_t y1 = 0, x0 = 0, r2 = 0, kDrive = 0, curveW = 0, K1 = 0, K2 = 0;
    };

    // inA[i] = X:$B2+i (вход банки 1), inB[i] = X:$72+i (вход банки 2)
    void frame(const std::array<uint32_t, kBank>& inA,
               const std::array<uint32_t, kBank>& inB,
               const In& par, Out& out) noexcept
    {
        constexpr uint64_t M56 = 0xFFFFFFFFFFFFFFull;

        // ---- $079F–$07A5: y1 = frac(8 / divWord), 24 шага DIV ----------------
        const int32_t y0div = sext24(par.divWord & 0xFFFFFFu);
        // div0: железо/оракул сатурирует пару в $00FFFFFFFFFFFF (dsp_emu div-ветка)
        const uint64_t D = (y0div == 0) ? 0xFFFFFFFFFFFFull
                                        : div24((uint64_t(8) << 24), y0div);
        const uint32_t y1 = (uint32_t)(D & 0xFFFFFFull);      // $07A5 move a0,y1
        out.y1 = y1;

        // ---- $07A6–$07AC: x0 = max(0, 2·w − 1.0) ------------------------------
        A_ = sext56((uint64_t(int64_t(sext24(par.distWord & 0xFFFFFFu)) << 24)) & M56);
        A_ = sext56((uint64_t(A_) << 1) & M56);               // $07A7 asl a
        A_ = sext56((uint64_t(A_) + uint64_t(int64_t(sext24(0x800000u)) << 24)) & M56);
        if (A_ < 0) A_ = 0;                                   // $07AA clr a ifmi
        const uint32_t x0 = limit24(A_);                      // $07AC move a,x0
        out.x0 = x0;

        // ---- $07AD–$07B7: пила r2, kDrive, кривая -----------------------------
        B_ = mpy56(sext24(x0), sext24(0x100u));               // $07AD mpyi #$100,x0,b
        A_ = mpy56(sext24(x0), sext24(x0));                   // $07AF mpy x0,x0,a
        const uint32_t r2 = (uint32_t)(uint64_t(B_) >> 24) & 0xFFFFFFu; // $07B1 move b,r2
        const uint32_t x1sq = limit24(A_);                    // $07B2 move a,x1
        A_ = mpy56(sext24(x1sq), sext24(0x7DECCDu));          // $07B3 mpyi #>$7deccd,x1,a
        A_ = sext56((uint64_t(A_) + uint64_t(int64_t(sext24(0x021333u)) << 24)) & M56);
        const uint32_t curveW = kDistCurve[(r2 & 0xFFu)];     // $07B7 (r2 ≤ $FF)
        out.r2 = r2;
        out.curveW = curveW;

        // ---- $07B9–$07BC: K2, kDrive, K1 ---------------------------------------
        B_ = mpy56(sext24(curveW), sext24(par.divWord & 0xFFFFFFu)); // mpy x1,y0,b
        const uint32_t kDrive = limit24(A_);                  // $07B9 parallel a,y0
        A_ = mpy56(sext24(y1), sext24(kDrive));               // $07BA mpy y1,y0,a
        const uint32_t K2 = limit24(B_);                      // $07BB move b,x1
        const uint32_t K1 = limit24(A_);                      // $07BC parallel a,y0
        out.kDrive = kDrive;
        out.K1 = K1;
        out.K2 = K2;
        // $07BC parallel: x0 = X:(r0)+ = inA[0] (r0: $B2 -> $B3)

        // ---- pass1 $07BD–$07C2 (34×), $07C3 bset — вне тела -------------------
        // потоки входов (по указателям r0/r1, выверено на оракуле):
        //   a-mpy: inA[0] ($07BC pre-load), затем inA[i] ($07C1 load it_(i−1))
        //   b-mpy: inB[i] ($07BF load it_i)
        int64_t aAcc = A_;                                    // acc переносится
        int64_t bAcc = B_;
        uint32_t x0r = inA[0];                                // текущий x0 (a-mpy)
        for (int i = 0; i < kBank; ++i)
        {
            // $07BF: mpy y0,x0,a | x:(r1)+,x0 | a,y:(r4)+
            out.bankA[(size_t)i] = limit24(aAcc);             // store ПРЕДЫДУЩЕГО acc
            aAcc = mpy56(sext24(K1), sext24(x0r));            // y0 = K1 (регистр!)
            const uint32_t xb = inB[(size_t)i];               // x:(r1)+ = inB[i]
            aAcc = sext56((uint64_t(aAcc) << 8) & M56);       // $07C0 asl #$8,a,a
            // $07C1: mpy y0,x0,b | x:(r0)+,x0 | b,y:(r5)+
            out.bankB[(size_t)i] = limit24(bAcc);
            bAcc = mpy56(sext24(K1), sext24(xb));             // x0 из $07BF этой же итер.
            x0r = (i + 1 < kBank) ? inA[(size_t)(i + 1)]      // x:(r0)+ = inA[i+1]
                                  : inA[(size_t)kBank - 1];   // (34-й load за банком)
            bAcc = sext56((uint64_t(bAcc) << 8) & M56);       // $07C2 asl #$8,b,b
        }
        // хвост: aAcc = (K1·inA[33]·2)<<8; bAcc = (K1·inB[33]·2)<<8
        // (хвостовые acc перетекают в pass2 — пин p2[0])

        // ---- pass2 $07C9–$07D1 (34×), $07CF/$07D0 — вне тела ------------------
        // r0 = $B0, r1 = $70, r4 = $B0, r5 = $70, x0 = $80
        uint32_t y0a = out.bankA[0];                          // $07C9 (r4 -> 1)
        uint32_t y1b = out.bankB[0];                          // $07CA (r5 -> 1)
        for (int i = 0; i < kBank; ++i)
        {
            // $07CD: mpy y0,x0,a | a,x:(r0)+ | y:(r4)+,y0
            out.bankA[(size_t)i] = limit24(aAcc);             // store предыдущего acc
            aAcc = mpy56(sext24(y0a), sext24(0x80u));         // x0 = $80
            y0a = (i + 1 < kBank) ? out.bankA[(size_t)(i + 1)] : 0; // y:(r4)+
            // $07CE: mpy x0,y1,b | b,x:(r1)+ | y:(r5)+,y1
            out.bankB[(size_t)i] = limit24(bAcc);
            bAcc = mpy56(sext24(y1b), sext24(0x80u));
            y1b = (i + 1 < kBank) ? out.bankB[(size_t)(i + 1)] : 0;
        }
        // $07CF/$07D0: финальные store за банком (X:$D4/X:$94) — вне порта.
    }

    // Фактор среза $07D2–$07DB. c = clamp(Y:$4DA → $6A3 сверху; tfr ifge);
    // kCut = limit24(K2·kCutoff[$143546 + c]) — связка диста со срезом.
    static uint32_t cutoffFactor(const Out& pins, uint32_t wofsWord) noexcept
    {
        uint32_t c = wofsWord & 0xFFFFFFu;
        // $07D5 cmp x0,a; $07D6 tfr x0,a ifge → a = min(a, $6A3) по знаковому
        if (sext24(c) > sext24(0x6A3u)) c = 0x6A3u;
        const uint32_t tbl = (c < 1728u) ? kCutoff[c] : kCutoff[1727u];
        const int64_t k = mpy56(sext24(pins.K2), sext24(tbl)); // $07DB mpy y1,y0,b
        return limit24(k);
    }

private:
    static int64_t sext56(uint64_t v) noexcept
    {
        v &= 0xFFFFFFFFFFFFFFull;
        return (v & (1ull << 55)) ? (int64_t)(v - (1ull << 56)) : (int64_t)v;
    }
    int64_t A_ = 0, B_ = 0;
};

} // namespace mnm
