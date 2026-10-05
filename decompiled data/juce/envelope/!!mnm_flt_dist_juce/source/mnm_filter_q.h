// ============================================================================
// mnm_filter_q.h — ИЗОЛИРОВАННЫЙ модуль Q (резонанс) фильтрового тракта FLT:
//                  q-интерполятор P:$05FF–$0649 + глайд-тройки P:$0693–$06B9.
//                  Monomachine SFX-6 OS 1.32B, ядро DSP1, домен слов Q1.23.
// ----------------------------------------------------------------------------
// СТАТУС: закрывает OPEN п.6 NOTES («Q-интерполятор → фидбек каскада»).
// В ТРАКТ ВЫКЛЮЧЕН ПО УМОЛЧАНИЮ: включение — макросом MNM_ENABLE_EXACT_Q
// (см. MnmVoiceFilterDist.h). До прогона-регрессии флаг не включать.
//
// ВЕРИФИКАЦИЯ (бит-точный эмулятор DSP56300, svf_trace.py, 2026-10-05,
// дампы X/Y на границах P:$05FF/$06CD/$0719/$077A; векторы в
// reference/svf_trace_results.json):
//   E1  ячейки: Q-слова — ФИЛЬТР A = Y:$40B (LPQ), ФИЛЬТР B = Y:$40A (HPQ),
//       КРОСС-ВКЛЮЧЕНИЕ подтверждено прогоном (перекрёстного влияния нет);
//       BASE/WDTH y:(r6+$8/$9) → индекс y:(r6+$DA); кольцо y:(r6+$10/$11)
//       читается расчётом коэффициентов, писателя в DSP-листингах нет (хост);
//   E3  глай = два полублока по 8 сэмплов (продолжение прошлого кадра →
//       подтягивание к свежему q);
//   E4  param4 y:(r6+$4) = индекс kQGainTbl (предкоррекция gQ).
//
// УРОВНИ (как в NOTES §4): обращения к памяти/ячейкам/таблицам — БВБ;
// аккумуляторная экстракция полинома (asl #$18) и масштаб div-ветки f —
// ТРАНСКР (помечено), закрываются регрессией на векторах E1/E3/E4.
//
// НЕ ПУТАТЬ (NOTES §5): модуль НЕ относится к delay-фильтрам
// (kHpqDamp/kHpqFb/kLpqFb $1448C6/$144946/$144BC9 — delay-территория),
// тап-сканеру func_000397, AMP-огибающей, FM freq-огибающим, ring mod,
// резонаторной машине.
// ============================================================================
#pragma once
#include <cstdint>
#include "mnm_fixed.h"
#include "mnm_tables_data.h"

namespace mnmq {

using mnm::sext24;
using mnm::mpy56;
using mnm::mac56;
using mnm::accB1;
using mnm::div24;

// ---------------------------------------------------------------------------
// Ячейки страницы FLT (r6-смещения; верифицировано E1 + §8.4 NOTES)
// ---------------------------------------------------------------------------
struct QCellMap
{
    static constexpr int kBase      = 0x08;  // y:(r6+$8)  BASE   ($0537)
    static constexpr int kWidth     = 0x09;  // y:(r6+$9)  WDTH   ($0557)
    static constexpr int kQToFltB   = 0x0A;  // y:(r6+$a)  Q → интерполятор ФИЛЬТРА B ($06E0)
    static constexpr int kQToFltA   = 0x0B;  // y:(r6+$b)  Q → интерполятор ФИЛЬТРА A ($0610)
    static constexpr int kParam4    = 0x04;  // y:(r6+$4)  → kQGainTbl (E4)
    static constexpr int kIdxFltA   = 0xDA;  // y:(r6+$DA) индекс среза фильтра A ($056C→$05FF)
    static constexpr int kIdxFltB   = 0xD9;  // x:(r6+$D9) кейтрек-копия индекса фильтра B ($06CD)
};

// --- входы одного кадра интерполятора ---------------------------------------
struct QFrameInputs
{
    uint32_t idxWord;      // индекс среза (y:(r6+$DA) для A / x:(r6+$D9) для B)
    uint32_t qWord;        // Q-слово ячейки (A ← $40B, B ← $40A — КРОСС, E1)
    uint32_t param4Word;   // слово param4 (индекс kQGainTbl)
};

// --- результат кадра ---------------------------------------------------------
struct QFrameResult
{
    int32_t qHp;           // y:$1d = tC·tA + add #>$800000 ($0644–$0649)
    int32_t qLp;           // y:$1e = tC·tB                      ($0643/$0648)
    int32_t f1;            // y:$1c ($0665)
    int32_t f2;            // y:$4  ($0692)
    int32_t glide[16][3];  // [k] = (f, q_hp, q_lp) — тройки движка (y:$4+3k,$5+3k,$6+3k)
};

// mpysu/macsu: знаковый × БЕЗЗНАКОВЫЙ (инструкции $062E/$0634/$063B/$063D)
static inline int64_t mpySU(int32_t x, uint32_t yU) noexcept
{
    int64_t p = int64_t(x) * int64_t(yU & 0xFFFFFFu);
    return (int64_t)((uint64_t)(p << 1) & mnm::ACC56_MASK);
}
static inline int64_t macSU(int64_t acc, int32_t x, uint32_t yU) noexcept
{
    return (int64_t)(((uint64_t)acc + (uint64_t)mpySU(x, yU)) & mnm::ACC56_MASK);
}

// слово → аккумулятор (A1-позиция)
static inline int64_t wordToAcc(int32_t w) noexcept
{
    return int64_t(sext24(uint32_t(w & 0xFFFFFFu))) << 24;
}

// ============================================================================
// QInterpolator — один экземпляр на фильтр. У фильтра B третий коэффициент
// полинома $FFD99A ($06E5) против $FFA666 ($0615) у A (§8.3 NOTES).
// ============================================================================
class QInterpolator
{
public:
    explicit QInterpolator(bool isFilterB = false) noexcept : isB_(isFilterB) { reset(); }

    void reset() noexcept
    {
        prevIdxSlot_  = 0;   // y:(r7-$1e): прошлый индекс (Y-сторона, $0620/$0622)
        prevTcIdxSlot_ = 0;  // x:(r7-$1e): прошлый tC-индекс (X-сторона, $061F/$0621)
        seedHp_ = 0;         // y:(r7) = y:(r6+$E0): q-сид прошлого кадра ($0694)
        seedX_  = 0;         // x:(r7) = x:(r6+$E0): вторая половина сида ($0697)
        seedF_  = 0;         // y:(r7) = y:(r6+$E1): f-сид ($06B1)
    }

    // Полный конвейер кадра: P:$05FF–$0649 (q-слова) + P:$0693–$06B9 (глайд).
    QFrameResult processFrame(const QFrameInputs& in) noexcept
    {
        QFrameResult r{};

        // ---------- 1) клампы индекса ($05FF–$060F) — БВБ ----------
        const int32_t idx = sext24(in.idxWord);
        int32_t ex1 = idx - 0x5BF;  if (ex1 < 0) ex1 = 0;   // $0601-$0603
        int32_t ex2 = ex1 - 0x80;   if (ex2 < 0) ex2 = 0;   // $0605-$0607
        const int32_t idxC = idx < 0x63F ? idx : 0x63F;     // $0609-$060E min(idx,$63F)

        // ---------- 2) полином ($0611–$0617) — БВБ инструкций ----------
        // b = mpyi($FFFDF3,idxC) + maci($FFE666,ex1) + maci(3rd,ex2); asl #$18
        // Константы как sext24-иммедиаты: $FFFDF3 = −$20D, $FFE666 = −$199A,
        // $FFA666 = −$599A (A) / $FFD99A = −$2666 (B).
        int64_t bp = mpy56(idxC, sext24(0xFFFDF3u));
        bp = mac56(bp, ex1, sext24(0xFFE666u));
        bp = mac56(bp, ex2, isB_ ? sext24(0xFFD99Au) : sext24(0xFFA666u));
        bp = (int64_t)(((uint64_t)bp << 24) & mnm::ACC56_MASK);      // asl #$18,b,b
        const int32_t poly = sext24(uint32_t(accB1(bp)));            // $0618 b,y0

        // ---------- 3) Q·(1+poly) и tC-индекс ($0618–$061D) — БВБ ----------
        int64_t a = wordToAcc(sext24(in.qWord));                     // $0610 move y:(r6+$b),a
        a = mac56(a, poly, sext24(in.qWord));                        // $061A mac y0,x0,a
        if (a & (1ll << 55)) a = 0;                                  // $061B clr a ifmi (минус → 0)
        const int32_t qScaled = accB1(a);                            // $061C move a,x0
        int64_t b3 = wordToAcc(idxC);                                // $0618 tfr x1,b
        b3 = mac56(b3, qScaled, sext24(0xFFF912u));                  // $061D maci #>$fff912,x0,b
        const int32_t tcIdxNow = accB1(b3);                          // B1 = tC-индекс (слово)

        // ---------- 4) half-sum сглаживание ($061F–$0628) — БВБ ----------
        // Y-слот: индекс; X-слот: tC-индекс. Новые слоты пишутся ДО усреднения.
        const int32_t newIdxSlot  = sext24(in.idxWord);              // $0622 move x1,y:(r7-$1e)
        const int32_t newTcSlot   = tcIdxNow;                        // $0621 move b1,x:(r7-$1e)
        int64_t ai = wordToAcc(prevIdxSlot_);                        // $0620 move y:(r7-$1e),a
        ai = ai + wordToAcc(idxC);                                   // $0623 add x1,a
        ai >>= 1;                                                    // $0624 asr a (арифм.)
        int64_t bt = wordToAcc(prevTcIdxSlot_);                      // $061F move x:(r7-$1e),x0
        bt = bt + wordToAcc(tcIdxNow);                               // $0625 add x0,b
        bt >>= 1;                                                    // $0626 asr b
        prevIdxSlot_  = newIdxSlot;
        prevTcIdxSlot_ = newTcSlot;
        const int32_t r1      = accB1(ai);                           // $0626 a1,r1 — целый(сгл. индекса)
        const int32_t r3      = accB1(bt);                           // $062D b1,r3 — целый(сгл. tC-индекса)
        const uint32_t fracI  = uint32_t(ai & 0xFFFFFFull);          // $0627 a0,x0 — дробь
        const uint32_t fracT  = uint32_t(bt & 0xFFFFFFull);          // $0628 b0,x1 — дробь

        // ---------- 5) тройник таблиц ($0629–$0642) — БВБ (macsu-лерp) ----------
        const int32_t tA = lerp3_(kQTableA, NA(kQTableA), r1, fracI);   // $0629–$062F
        const int32_t tB = lerp3_(kTbl2,    NA(kTbl2),    r1, fracI);   // $0630–$0636
        int32_t tC = lerp3_(kQTableC, NA(kQTableC), r3, fracT);         // $0637–$063D
        if (tC > (int32_t)0x7FFFA4) tC = (int32_t)0x7FFFA4;             // $0640-$0641 tgt

        // ---------- 6) q-слова ($0643–$0649) — БВБ ----------
        int64_t bb = mpy56(tC, tB);                                      // $0643 mpy x0,y1,b
        r.qLp = sext24(uint32_t(accB1(bb)));                             // $0648 y:$1e = b1 (wrap)
        int64_t aa = mpy56(tA, tC);                                      // $0644 mpy y0,x0,a
        aa += int64_t(sext24(0x800000u)) << 24;                          // $0646 add #>$800000,a
        r.qHp = sext24(uint32_t(accB1(aa)));                             // $0649 y:$1d = a1 (wrap)

        // ---------- 7) f1/f2 ($064A–$0692) — ТРАНСКР масштаба div-ветки ----------
        const int32_t gQ = kQGainTbl[qGainIndex_(in.param4Word)];        // $065A–$0662
        const int64_t tc2 = mpy56(tC, tC);                               // $064A mpy x0,x0,b
        // $064B subr a,b — семантика SUBR по FM DSP56300: S1 − S2 → S2,
        // т.е. b = a − b = q_hp-acc − tC². В трассе-доке §1 строка подписана
        // «tC² − q_hp» — знак ветки сверить векторами E4 ДО включения флага.
        const int64_t d1 = aa - tc2;                                     // subr a,b
        const uint64_t dv = div24(uint64_t(0x800000ull << 24), r.qLp);   // $064F–$0654: $800000/q_lp (ТРАНСКР делимого)
        const int32_t invQ = sext24(uint32_t(dv & 0xFFFFFFull));         // частное — младшие 24 бита
        r.f1 = fBranch_(d1, -1, invQ, gQ);                               // $0657–$0665 (−0.25)
        r.f2 = fBranch_(d1, +1, invQ, gQ);                               // $0666–$0692 (+0.25)

        // ---------- 8) глайд-тройки ($0693–$06B9) — структура по E3 ----------
        buildGlide_(r);

        return r;
    }

    // слоты для регрессии/диагностики
    int32_t prevIdxSlot()  const noexcept { return prevIdxSlot_; }
    int32_t prevTcIdxSlot() const noexcept { return prevTcIdxSlot_; }

private:
    bool isB_;
    int32_t prevIdxSlot_ = 0;
    int32_t prevTcIdxSlot_ = 0;
    int32_t seedHp_ = 0, seedX_ = 0, seedF_ = 0;

    template <typename T, std::size_t N> static constexpr std::size_t NA(const T (&)[N]) noexcept { return N; }

    // lerp тройник: a = tab[i]·(1−frac) + tab[i+1]·frac, macsu-семантика
    static int32_t lerp3_(const uint32_t* tab, std::size_t n,
                          int32_t iWord, uint32_t frac) noexcept
    {
        std::size_t i = (std::size_t)((uint32_t)iWord & 0xFFFFFFu);
        // чтение за пределами таблицы в прошивке уходит в соседние слова
        // (kQTableA → kCoeff1/kCoeff2) — поведение сохраняем заворотом по n
        const int32_t w0 = (int32_t)tab[i % n];
        const int32_t w1 = (int32_t)tab[(i + 1) % n];
        int64_t acc = wordToAcc(w0);                 // $062D tfr y0,a
        acc = macSU(acc, -w0, frac);                 // $062E macsu -y0,x0,a
        acc = macSU(acc,  w1, frac);                 // $062F macsu y1,x0,a
        return sext24(uint32_t(accB1(acc)));
    }

    // индекс kQGainTbl: r1 = param4·$200 (E4; $0660/$068C)
    static int qGainIndex_(uint32_t param4Word) noexcept
    {
        int64_t acc = mpy56(sext24(param4Word), 0x200);
        int32_t idx = accB1(acc);
        if (idx < 0) idx = 0;
        constexpr int n = (int)(sizeof(kQGainTbl) / sizeof(kQGainTbl[0]));
        return idx < n ? idx : n - 1;
    }

    // f-ветка: ($800000/q_lp)·(q_hp − tC² ± 0.25) → ×kQGain → asl #$10
    // ($064C/$0674, $0657–$0665, $0666–$0692; порядок ×kQGain/asl — ТРАНСКР)
    static int32_t fBranch_(int64_t d1, int sign, int32_t invQ, int32_t gQ) noexcept
    {
        int64_t b = d1;
        b += sign > 0 ? (int64_t(0x400000) << 24) : -(int64_t(0x400000) << 24); // ±0.25
        int64_t p = mpy56(accB1(b), invQ);           // ·($800000/q_lp)
        p = mpy56(accB1(p), gQ);                      // ×kQGain ($0662/$068F mpy)
        p = (int64_t)(((uint64_t)p << 10) & mnm::ACC56_MASK);                   // asl #$10
        return sext24(uint32_t(accB1(p)));
    }

    // глайд: два полублока по 8 ($069C–$06AA q-треть, $06AB–$06B9 f-треть)
    void buildGlide_(QFrameResult& r) noexcept
    {
        // шаг = adj·$10 на сэмпл (mac y0,x0 с x0=$10; дробное произведение)
        // шаг на сэмпл: mac y0,x0,a c x0=$10 → acc += (step·$10)<<1;
        // слово = acc>>24 → приращение слова = step·$20>>24 = step>>19
        const int32_t stepHp = r.qHp - seedHp_;       // $0696 sub x0,a (x0 = сид)
        const int32_t stepLp = r.qLp - seedX_;        // $0699 sub x1,b
        // полублок 1: старт от сида прошлого кадра
        int32_t a = seedHp_, bq = seedX_;
        for (int k = 0; k < 8; ++k)                   // $069C do #<$8
        {
            a  = (int32_t)((uint32_t)(a  + ((int64_t)stepHp * 0x20 >> 24)) & 0xFFFFFFu);
            bq = (int32_t)((uint32_t)(bq + ((int64_t)stepLp * 0x20 >> 24)) & 0xFFFFFFu);
            r.glide[k][1] = a;                        // y:$5+3k
            r.glide[k][2] = bq;                       // y:$6+3k
            r.glide[k][0] = r.f1;                     // f-слот (y:$4+3k), f-петля ниже
        }
        // полублок 2 ($06A0–$06AA): старт от свежих y:$1d/y:$1e, шаг — ротация
        const int32_t stepHp2 = seedHp_ - r.qHp;      // $06A2/$06A4 tfr x0,a a,y0
        const int32_t stepLp2 = seedX_  - r.qLp;      // $06A3/$06A5
        a = r.qHp; bq = r.qLp;
        for (int k = 8; k < 16; ++k)                  // $06A7 do #<$8
        {
            a  = (int32_t)((uint32_t)(a  + ((int64_t)stepHp2 * 0x20 >> 24)) & 0xFFFFFFu);
            bq = (int32_t)((uint32_t)(bq + ((int64_t)stepLp2 * 0x20 >> 24)) & 0xFFFFFFu);
            r.glide[k][1] = a;
            r.glide[k][2] = bq;
            r.glide[k][0] = r.f2;
        }
        // f-треть ($06AB–$06B9): половина 1 от f-сида к f1, половина 2 от f1 к f2
        const int32_t stepF1 = r.f1 - seedF_;         // $06B1 sub x0,a
        int32_t f = seedF_;
        for (int k = 0; k < 8; ++k)
            r.glide[k][0] = f = (int32_t)((uint32_t)(f + ((int64_t)stepF1 * 0x20 >> 24)) & 0xFFFFFFu);
        const int32_t stepF2 = r.f2 - r.f1;           // $06B2 sub x1,b
        f = r.f1;
        for (int k = 8; k < 16; ++k)
            r.glide[k][0] = f = (int32_t)((uint32_t)(f + ((int64_t)stepF2 * 0x20 >> 24)) & 0xFFFFFFu);
        // сиды следующего кадра: $0694 y:(r7)=a(сгл. q_hp+округление);
        // $0697 x:(r7)=y1; $06B1 f2 → y:(r7)+
        seedHp_ = r.qHp;
        seedX_  = r.qLp;   // ТРАНСКР: точное содержимое x-половины сида (y1)
        seedF_  = r.f2;
    }
};

} // namespace mnmq
