// ============================================================================
// MnmFltDistVoice.h — тракт FLT+DIST Monomachine SFX-60 OS 1.32B
//                     Сборка по КОРРЕКТНОЙ карте ячеек (паки 12–14,
//                     CONTROL_MODS_MAP_RU §3, GITHUB_SYNC_VERDICT §4)
//                     mnm_18: стадия 2 врезана, ступень DIST бит-точная,
//                     тон-индекс проведён (закрытие §7, worklog Task 6).
// ----------------------------------------------------------------------------
// КАРТА ЯЧЕЕК (доказано паками 12–14; страницы ColdFire):
//   FLT  CC72–79 → $410–$417: BASE, WDTH, HPQ, LPQ, ATK, DEC, BOFS, WOFS
//   AMP  CC56–59 → $400–$403: AHDR ATK/HOLD/DEC/REL; триггер $428 (1/2/3)
//   DIST          → $404; события $420/$421/$428
//   EFFX          → $418–$41F (env2-гейт конца цепи; уровень Y:$4FF)
//   $408–$40F     — mnm_20: НЕ пустая дыра, а вторая зона чтения DSP:
//   тон $0537/$0557 читает Y:$408/$409, Q-путь $06E0/$0610 — Y:$40A/$40B,
//   X:$40B — делитель гейна банков диста (константа init-а $200000).
//   Аудит/astra связывает с FILT именно $08–$0F; трасса итер. 17 — $10–$17.
//   Вопрос карты ручек хоста оставлен ОТКРЫТЫМ (exp80): порт экспонирует
//   слова обеих зон через сеттеры (setFiltWords + setFlt2Words).
//
// ТРАКТ (ядро DSP1; правка владельца mnm_16 — РЕЗОНАТОРА В ТРАКТЕ НЕТ):
//   кольцо коэффициентов (кольцо = СЛОВА $410/$411 ТОЧНО — P:$056D–$0572
//   сводит acc к $410; панч/HPQ/питч/WOFS в кольцо НЕ входят, exp72c)
//   → каскад func_000340 (P:$0340–$0364, бит-в-бит)
//   → СТАДИЯ 2 (P:$0A5D–$0AD0) = огибающая фильтра ATK/DEC/BOFS/WOFS
//     ($414–$417; бит-в-бит пак 5: 37536/0); выход L2 Y:$20–$3F → микс T6
//   → ступень DIST (P:$079F–$07D1, бит-в-бит mnm_18: банки 34 слова,
//     K1 = frac(8/D)·kDrive, пила $1447C6, x0 = max(0, 2·DIST−1))
//   → AHDR (P:$088E–$08D7, ячейки $400–$403, триггер $428 — пак 7)
//   → env2-гейт конца цепи (P:$04A8–$04F5, EFFX $418–$41B — пак 14)
//   → out
//
//   ПАНЧ DIST (P:$04FF–$0556, MnmDistDrive пака 13): уровень X:$4DB →
//   ИНДЕКС ТОНА (P:$0537–$056C, mnm_tone_index.h mnm_18): X:$4D9 → гейн
//   EQ $0857 (1 − kCutoff[128+idx]) и $06CD; Y:$4DA (WOFS-ветка) → фактор
//   среза диста $07D4 (clamp $6A3) и комбайнер Q-интерполятора $05FF.
//   Читатели X:$4D9/Y:$4DA доказаны трассой exp72c (непрямые включительно).
//
//   Q-интерполятор P:$0629–$0649 = интерполятор коэффициентов EQ-биквада
//   (таблицы $141A98/$142158/$142F06) — домен TONE; сами биквады func_000350
//   вне скоупа голоса (полнота хвоста — пак 8, 392824/0). Здесь проведён
//   слышимый путь панча: индекс → гейн EQ и фактор среза диста.
//
//   func_000397 (P:$0397–$0430) — начинка ДЕЛЕЙ-машины, в тракт не входит
//   (правка владельца mnm_16; бит-точный материал в mnm_15).
//
//   АДАПТЕРЫ БАНКОВ (документированные решения порта, НЕ факты прошивки):
//   банк диста inA = [L0..L15, R0..R15, prevL, prevR], inB = то же
//   (в ядре банки 34 слова = выход EQ-хвоста); входы L1 стадии 2 = выход
//   каскада кадра, x00/x10 = сухой вход кадра, угол L3 = 0 (без делэй-машины
//   состояние P+$CF нулевое). Модули сами по себе бит-точны (пины против
//   оракула в Tests/test_sanity.cpp).
// ============================================================================
#pragma once
#include <array>
#include <cmath>
#include <algorithm>
#include "mnm_dsp56300_math.h"
#include "mnm_filter_ring.h"
#include "mnm_cascade340.h"
#include "mnm_dist_stage.h"      // ступень DIST $079F–$07D1 (mnm_18)
#include "mnm_tone_index.h"      // индекс тона $0537–$056C (mnm_18)
#include "MnmFilterStage2.hpp"   // стадия 2 = огибающая фильтра (пак 10)
#include "MnmAmpEnv.hpp"         // AHDR $088E–$08D7 (пак 7)
#include "MnmEnv2.hpp"           // env2-гейт конца цепи (пак 14)
#include "MnmDistDrive.hpp"      // панч DIST (пак 13)

namespace mnm {

class MnmFltDistVoice
{
public:
    // ---------- страница FLT ($410–$417) ------------------------------------
    void setFiltWords(uint32_t base, uint32_t wdth, uint32_t hpq, uint32_t lpq,
                      uint32_t atk, uint32_t dec, uint32_t bofs, uint32_t wofs) noexcept
    {
        baseW_ = base & 0xFFFFFFu;   // $410
        wdthW_ = wdth & 0xFFFFFFu;   // $411
        hpqW_  = hpq  & 0xFFFFFFu;   // $412 (потребитель — делей; HPQ Y:$409
        lpqW_  = lpq  & 0xFFFFFFu;   //  дыра — в тоне не участвует на стоке)
        atkW_  = atk  & 0xFFFFFFu;   // $414 → стадия 2
        decW_  = dec  & 0xFFFFFFu;   // $415
        bofsW_ = bofs & 0xFFFFFFu;   // $416
        wofsW_ = wofs & 0xFFFFFFu;   // $417
    }

    // ---------- зона $408–$40F (вторая зона чтения DSP, exp80) ---------------
    // На стоковой OS хост-писателя не доказаны (карта ручек хоста — ОТКРЫТ
    // вопрос: аудит/astra говорят FILT = $08–$0F, трасса итер. 17 — $10–$17).
    // Слова проходят в бит-точные модули тона/панча; Y:$40A/$40B (Q-ячейки
    // крест-накрест, E1–E4) — проводка Q-пути остаётся в очереди mnm_21.
    void setFlt2Words(uint32_t w408, uint32_t w409, uint32_t w40A, uint32_t w40B,
                      uint32_t w40C, uint32_t w40D, uint32_t w40E, uint32_t w40F) noexcept
    {
        w408_ = w408 & 0xFFFFFFu;   // тон: base индекса тона ($0537)
        w409_ = w409 & 0xFFFFFFu;   // тон: добавка 2048·HPQ ($055A)
        w40A_ = w40A & 0xFFFFFFu;   // Q HP-плеча ($06E0) — проводка Q: mnm_21
        w40B_ = w40B & 0xFFFFFFu;   // Q LP-плеча ($0610) — проводка Q: mnm_21
        w40C_ = w40C & 0xFFFFFFu;   // панч: atkIdx ($0509)
        w40D_ = w40D & 0xFFFFFFu;   // ($052B)
        w40E_ = w40E & 0xFFFFFFu;   // тон/панч: panTerm ($053E)
        w40F_ = w40F & 0xFFFFFFu;   // тон: WOFS-ветка ($055C)
    }

    // ---------- ручка DIST ($404) и ячейка X:$40B -----------------------------
    void setDistWord(uint32_t w) noexcept            // Y:$404, знаковое v<<16
    {
        distW_ = w & 0xFFFFFFu;
    }
    void setMachineHeadroomWord(uint32_t w) noexcept // X:$40B, OS-дефолт $200000
    {
        divW_ = w & 0xFFFFFFu;
    }

    // ---------- страница AMP ($400–$403, AHDR) -------------------------------
    void setAmpWords(uint32_t atk, uint32_t hold, uint32_t dec, uint32_t rel) noexcept
    {
        aAtkW_ = atk & 0xFFFFFFu;    // $400
        aHoldW_ = hold & 0xFFFFFFu;  // $401
        aDecW_ = dec & 0xFFFFFFu;    // $402
        aRelW_ = rel & 0xFFFFFFu;    // $403
    }

    // ---------- страница EFFX ($418–$41B, env2-гейт) -------------------------
    void setEnv2Words(uint32_t atk, uint32_t dec, uint32_t sust, uint32_t rel) noexcept
    {
        env2_.atk = atk & 0xFFFFFFu;
        env2_.dec = dec & 0xFFFFFFu;
        env2_.sust = sust & 0xFFFFFFu;
        env2_.rel = rel & 0xFFFFFFu;
    }

    // ---------- события ($420/$421/$428) -------------------------------------
    void trigger() noexcept          // note-on: $420:=1 (панч), $428:=1 (AHDR), $421:=1 (env2)
    {
        punch_.retrig();
        ahdr_.trig(1);
        env2_.gate = 1;
        env2_.slot = 0;
    }
    void release() noexcept          // note-off: $428:=2 (AHDR REL)
    {
        ahdr_.trig(2);
        env2_.gate = 2;
    }

    void reset() noexcept
    {
        casc_[0].reset(); casc_[1].reset();
        ahdr_.reset();
        punch_.reset();
        stage2_.d3y = stage2_.d3x = stage2_.d4y = stage2_.d4x = 0;
        stage2_.d5y = stage2_.d5x = stage2_.ccy = stage2_.ccx = 0;
        cnt_ = 0;
        inBufL_.fill(0.f); inBufR_.fill(0.f);
        frameL_.fill(0.f); frameR_.fill(0.f);
        outL_.fill(0.f);   outR_.fill(0.f);
        prevL_.fill(0.f);  prevR_.fill(0.f);
    }

    // ---------- потоковый ввод (латентность 1 кадр = 16 сэмплов) -------------
    inline float processL(float inL) noexcept { return push(0, inL); }
    inline float processR(float inR) noexcept { return push(1, inR); }

private:
    inline float push(int ch, float x) noexcept
    {
        float out = (ch ? prevR_ : prevL_)[(size_t)cnt_];
        (ch ? inBufR_ : inBufL_)[(size_t)cnt_] = x;
        if (++cnt_ < kFrame) return out;

        cnt_ = 0;
        runFrame();
        prevL_.swap(outL_);
        prevR_.swap(outR_);
        return out;
    }

    void runFrame() noexcept
    {
        // 1) AHDR (пак 7): темп-слово — вход-агностичный закон; 120 = как в
        //    векторах верификации пака 7
        ahdr_.tick((int)aAtkW_, (int)aHoldW_, (int)aDecW_, (int)aRelW_, 120);

        // 2) env2-гейт (пак 14): фазы P:$04A8–$04F5, уровень Y:$4FF
        env2_.frame();

        // 3) кольцо коэффициентов + каскад — бит-точные (mnm_17).
        //    Кольцо = слова $410/$411 ТОЧНО: панч/HPQ/питч/WOFS в кольцо
        //    не входят (P:$056D–$0572, трасса exp72c).
        FilterRing ring = computeRing(baseW_, wdthW_);
        casc_[0].processFrame(inBufL_.data(), frameL_.data(), ring.c);
        casc_[1].processFrame(inBufR_.data(), frameR_.data(), ring.c);

        // 4) панч DIST (пак 13): триггер $420; слова зоны $408–$40F проходят
        //    (exp80: читатели $0509/$052B живы), на стоке — нули (дыра UI)
        MnmDistDrive::Params pp;
        pp.atkIdx  = (int)(w40C_ >> 16);
        pp.param4  = (int32_t)sext24(w408_);
        pp.panTerm = (int32_t)sext24(w40E_);
        punchLevel_ = punch_.tickRaw(pp);

        // 5) индекс тона ($0537–$056C, mnm_18): уровень панча → X:$4D9/Y:$4DA
        MnmToneIndex::In tin;
        tin.param4 = w408_;                                // Y:$408 (exp80: читает $0537)
        tin.w40E = w40E_; tin.w40F = w40F_;                // Y:$40E/$40F ($053E/$055C)
        tin.hpqWord = w409_;                               // Y:$409 ($055A)
        tin.x401 = 0; tin.bits425 = 0;                     // дыра
        tin.levelWord = (uint32_t)punchLevel_ & 0xFFFFFFu; // X:$4DB
        const MnmToneIndex::Out tidx = MnmToneIndex::compute(tin);
        toneIdx_ = tidx.toneIdx;
        wofsWord_ = tidx.wofsWord;
        const uint32_t eqG = MnmToneIndex::eqGain(tidx.toneIdx);   // $0857
        const float eqGainF = q23ToF(eqG);
        // фактор среза диста ($07D2–$07DB): clamp(Y:$4DA, $6A3) → kCutoff
        const uint32_t kCut = MnmDistStage::cutoffFactor(distOut_, tidx.wofsWord);
        (void)kCut;   // слышимая связка живёт в банках диста (ниже)

        // 6) стадия 2 ($0A5D–$0AD0, пак 5/10): KILL = фаза AHDR
        mnmfm::MnmFilterStage2::In s2;
        for (int i = 0; i < kFrame; ++i)
        {
            s2.x30[i] = fToQ23(frameL_[(size_t)i]) & 0xFFFFFFu;   // L1 a = выход каскада L
            s2.x20[i] = fToQ23(frameR_[(size_t)i]) & 0xFFFFFFu;   // L1 b = выход каскада R
            s2.x00[i] = fToQ23(inBufL_[(size_t)i]) & 0xFFFFFFu;   // L3 a = сухой L
            s2.x10[i] = fToQ23(inBufR_[(size_t)i]) & 0xFFFFFFu;   // L3 b = сухой R
        }
        s2.atk = atkW_; s2.dec = decW_; s2.bofs = bofsW_; s2.wofs = wofsW_;
        s2.phase = (uint32_t)ahdr_.phase;                          // X:P+$D8 (KILL=4)
        s2.cfx = 0; s2.cfy = 0;                                    // P+$CF: без делэя 0
        stage2_.frame(s2, s2out_);

        // 7) ступень DIST ($079F–$07D1, mnm_18): банки 34 слова из кадра
        for (int i = 0; i < kFrame; ++i)
        {
            const uint32_t l = fToQ23(frameL_[(size_t)i]) & 0xFFFFFFu;
            const uint32_t r = fToQ23(frameR_[(size_t)i]) & 0xFFFFFFu;
            distInA_[(size_t)i]        = l;               // L0..L15
            distInA_[(size_t)(16 + i)] = r;               // R0..R15
            distInB_[(size_t)i]        = l;
            distInB_[(size_t)(16 + i)] = r;
        }
        distInA_[32] = fToQ23(prevL_[15]) & 0xFFFFFFu;    // prev-хвост
        distInA_[33] = fToQ23(prevR_[15]) & 0xFFFFFFu;
        distInB_[32] = distInA_[32];
        distInB_[33] = distInA_[33];
        MnmDistStage::In din;
        din.distWord = distW_;                            // Y:$404 — ручка DIST (хост)
        // X:$40B — константа init-а смены машины $200000 (P:$00F1, exp80/81;
        // независимо подтверждено пакетом astra). НЕ ноль: при нуле div0 даёт
        // y1=$FFFFFF≈−1.0 и банк A инвертирует сигнал (баг порт-версий ≤ mnm_19).
        din.divWord = divW_;
        din.wofsWord = tidx.wofsWord;                     // Y:$4DA
        dist_.frame(distInA_, distInB_, din, distOut_);

        // 8) микс кадра: каскад → гейн EQ ($0857) → гейт (AHDR·env2);
        //    стадия-2 L2 (Y:$20–$3F) суммируется в микс (T6, пак 14).
        //    mnm_20: при OS-дефолте X:$40B=$200000 банк A ≈ нем (K1=$000001),
        //    банк B жив на 0.25·curve (K2=$1FFFFF, замер exp81).
        const float ahdrLvl = q23ToF((uint32_t)ahdr_.level);
        const float env2Lvl = q23ToF(env2_.level);
        const float gate = ahdrLvl * env2Lvl;
        for (int i = 0; i < kFrame; ++i)
        {
            const float dryL = frameL_[(size_t)i] * eqGainF;
            const float dryR = frameR_[(size_t)i] * eqGainF;
            const float resL = q23ToF(s2out_.Y20[(size_t)(2 * i)]);
            const float resR = q23ToF(s2out_.Y20[(size_t)(2 * i + 1)]);
            // T6 ($0B45): запись микса через лимитер DSP — насыщение ±1.0
            outL_[(size_t)i] = q23ToF(fToQ23((dryL + resL) * gate));
            outR_[(size_t)i] = q23ToF(fToQ23((dryR + resR) * gate));
        }
    }

    // --- слова страниц --------------------------------------------------------
    uint32_t baseW_ = 0, wdthW_ = 127u << 16, hpqW_ = 0, lpqW_ = 0;
    uint32_t distW_ = 0;                              // Y:$404 (ручка DIST)
    uint32_t divW_  = 0x200000u;                      // X:$40B (init смены машины)
    uint32_t w408_ = 0, w409_ = 0, w40A_ = 0, w40B_ = 0;   // зона $408–$40F
    uint32_t w40C_ = 0, w40D_ = 0, w40E_ = 0, w40F_ = 0;   // (exp80: читатели живы)
    uint32_t atkW_ = 0, decW_ = 0, bofsW_ = 64u << 16, wofsW_ = 64u << 16;
    uint32_t aAtkW_ = 0, aHoldW_ = 0, aDecW_ = 0, aRelW_ = 0;
    // --- блоки ----------------------------------------------------------------
    Cascade340    casc_[2];
    mnmfm::MnmFilterStage2 stage2_;              // стадия 2 (пак 5/10)
    mnmfm::MnmFilterStage2::Out  s2out_{};
    MnmDistStage  dist_;                          // ступень DIST (mnm_18)
    MnmDistStage::Out distOut_{};
    mnmfm::MnmAmpEnv ahdr_;                       // AHDR (пак 7, namespace mnmfm)
    mnmenv2::MnmEnv2 env2_;                       // env2-гейт (пак 14)
    MnmDistDrive  punch_;                         // панч DIST (пак 13)
    int32_t       punchLevel_ = 0;
    uint32_t      toneIdx_ = 0, wofsWord_ = 0;
    uint32_t      distDivWord_ = 0;               // X:$40B (0 на стоке)
    // --- кадровые буферы ------------------------------------------------------
    std::array<float, kFrame> inBufL_{}, inBufR_{};
    std::array<float, kFrame> frameL_{}, frameR_{};
    std::array<float, kFrame> outL_{},  outR_{};
    std::array<float, kFrame> prevL_{}, prevR_{};
    std::array<uint32_t, MnmDistStage::kBank> distInA_{}, distInB_{};
    int cnt_ = 0;
public:
    int32_t punchLevelRaw() const noexcept { return punchLevel_; }
    int32_t ahdrLevelRaw()  const noexcept { return ahdr_.level; }
    uint32_t env2LevelRaw() const noexcept { return env2_.level; }
    uint32_t toneIdxRaw()   const noexcept { return toneIdx_; }
    uint32_t wofsWordRaw()  const noexcept { return wofsWord_; }
};

} // namespace mnm
