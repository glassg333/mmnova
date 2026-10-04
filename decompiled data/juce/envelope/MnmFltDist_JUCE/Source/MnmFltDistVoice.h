// ============================================================================
// MnmFltDistVoice.h — тракт FLT+DIST Monomachine SFX-60 OS 1.32B
//                     Сборка по КОРРЕКТНОЙ карте ячеек (паки 12–14,
//                     CONTROL_MODS_MAP_RU §3, GITHUB_SYNC_VERDICT §4)
// ----------------------------------------------------------------------------
// КАРТА ЯЧЕЕК (доказано паками 12–14; страницы ColdFire):
//   FLT  CC72–79 → $410–$417: BASE, WDTH, HPQ, LPQ, ATK, DEC, BOFS, WOFS
//   AMP  CC56–59 → $400–$403: AHDR ATK/HOLD/DEC/REL; триггер $428 (1/2/3)
//   DIST          → $404; события $420/$421/$428
//   EFFX          → $418–$41F (env2-гейт конца цепи: $418 ATK, $419 DEC,
//                   $41A SUS, $41B REL; гейт-флаг $421; уровень Y:$4FF)
//   $408–$40F     — ДЫРА страницы: на стоковой OS не пишется НИКЕМ
//
// ТРАКТ (ядро DSP1; правка владельца mnm_16 — РЕЗОНАТОРА В ТРАКТЕ НЕТ):
//   кольцо коэффициентов (BASE/WDTH + кольцо P:$0576–$059A)
//   → каскад func_000340 (P:$0340–$0364, бит-в-бит)
//   → СТАДИЯ 2 (P:$0A5D–$0AD0) = НАСТОЯЩАЯ огибающая фильтра
//     (ATK/DEC/BOFS/WOFS = $414–$417; бит-в-бит пак 5: 37536/0)  [*]
//   → ступень DIST 4k + жёсткий клип ±1.0 (P:$07BD–$07D1, скалярный
//     эквивалент; закон ручки DIST — OPEN, замеры пака 12 в README)
//   → AHDR (P:$088E–$08D7, ячейки $400–$403, триггер $428 — пак 7)
//   → env2-гейт конца цепи (P:$04A8–$04F5, EFFX $418–$41B — пак 14)
//   → out
//
//   func_000397 (P:$0397–$0430: сканер 8 дробных тапов + HPQ/LPQ) — это
//   начинка ДЕЛЕЙ-машины, ОТДЕЛЬНОГО модуля машины, в тракт FLT+DIST не
//   входит и в этом порте не собирается (правка владельца mnm_16; бит-
//   точный материал сохранён в пакете mnm_15). Слова HPQ=$412/LPQ=$413
//   принимаются со страницы FLT, но их потребитель — делей, не фильтр.
//
//   [*] Полнокадровая проводка стадии 2 (банки X:$00–$3F / Y:$20–$81,
//   фаза AHDR для KILL) — по бит-в-бит кадру пака 8 (392824/0); в этой
//   сборке MnmFilterStage2 включён в пакет, но в аудиотракт НЕ врезан —
//   проводка без векторов пака 8 была бы выдумкой. Слова ATK/DEC/BOFS/
//   WOFS проходят в stage2_.frame() через In-структуру.
//
//   ПАНЧ DIST (P:$04FF–$0556, MnmDistDrive пака 13): внутренний one-shot
//   на триггер $420 (note-on), фиксированная форма 0.7 мс / 23.2 мс —
//   читатели индекса тона (гейн EQ $06CD/$07D4/$0857) вне скоупа этого
//   модуля; панч тикает и экспонирует уровень для тестов.
//
// НЕ ПУТАТЬ: P:$0506–$0536 — НЕ «огибающая фильтра» (это машина панча
// DIST; ячейки $40C/$40D — дыра, атака/спад фиксированные). «AMP AHDR»
// бандла P:$04A8–$04F5 — это env2-гейт конца цепи (EFFX). См. вердикт §2.
// ============================================================================
#pragma once
#include <array>
#include <cmath>
#include <algorithm>
#include "mnm_dsp56300_math.h"
#include "mnm_filter_ring.h"
#include "mnm_cascade340.h"
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
        hpqW_  = hpq  & 0xFFFFFFu;   // $412
        lpqW_  = lpq  & 0xFFFFFFu;   // $413
        atkW_  = atk  & 0xFFFFFFu;   // $414
        decW_  = dec  & 0xFFFFFFu;   // $415
        bofsW_ = bofs & 0xFFFFFFu;   // $416
        wofsW_ = wofs & 0xFFFFFFu;   // $417
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

        // 3) кольцо коэффициентов (BASE/WDTH) + каскад — бит-точные
        //    (HPQ/LPQ здесь НЕ применяются: их потребитель — делей)
        FilterRing ring = computeRing(baseW_, wdthW_);
        casc_[0].processFrame(inBufL_.data(), frameL_.data(), ring.c);
        casc_[1].processFrame(inBufR_.data(), frameR_.data(), ring.c);

        // 4) панч DIST (пак 13): фиксированная форма, триггер $420
        MnmDistDrive::Params pp;                 // $40C/$40D/$408/$40E — дыра, нули
        punchLevel_ = punch_.tickRaw(pp);

        // 5) ступень DIST → гейты, по сэмплам
        const float ahdrLvl = q23ToF((uint32_t)ahdr_.level);
        const float env2Lvl = q23ToF(env2_.level);
        const float gate = ahdrLvl * env2Lvl;
        for (int i = 0; i < kFrame; ++i)
        {
            float l = frameL_[(size_t)i];
            float r = frameR_[(size_t)i];
            l = distStage(l);                    // ступень 4k + клип ±1.0 [*]
            r = distStage(r);
            outL_[(size_t)i] = l * gate;
            outR_[(size_t)i] = r * gate;
        }
        (void)punchLevel_;
    }

    // [*] Скалярный эквивалент ступени P:$07BD–$07D1: жёсткий клип 24-бит
    // слова. Закон ручки DIST (замеры пака 12: макс слышимости 0..32, пила
    // с заворотом каждые 64, −8 дБ на −64, нейтраль = самый чистый/громкий)
    // — OPEN; единичное усиление = нейтральная точка без выдумок.
    static inline float distStage(float x) noexcept
    {
        return q23ToF(sat24((int64_t)fToQ23(x)) & 0xFFFFFFu);
    }

    // --- слова страниц --------------------------------------------------------
    uint32_t baseW_ = 0, wdthW_ = 127u << 16, hpqW_ = 0, lpqW_ = 0;
    uint32_t atkW_ = 0, decW_ = 0, bofsW_ = 64u << 16, wofsW_ = 64u << 16;
    uint32_t aAtkW_ = 0, aHoldW_ = 0, aDecW_ = 0, aRelW_ = 0;
    // --- блоки ----------------------------------------------------------------
    Cascade340    casc_[2];
    mnmfm::MnmFilterStage2 stage2_;   // [*] проводка полного кадра — пак 8
    mnmfm::MnmAmpEnv ahdr_;           // AHDR (пак 7, namespace mnmfm)
    mnmenv2::MnmEnv2 env2_;           // env2-гейт (пак 14)
    MnmDistDrive  punch_;             // панч DIST (пак 13)
    int32_t       punchLevel_ = 0;
    // --- кадровые буферы ------------------------------------------------------
    std::array<float, kFrame> inBufL_{}, inBufR_{};
    std::array<float, kFrame> frameL_{}, frameR_{};
    std::array<float, kFrame> outL_{},  outR_{};
    std::array<float, kFrame> prevL_{}, prevR_{};
    int cnt_ = 0;
public:
    int32_t punchLevelRaw() const noexcept { return punchLevel_; }
    int32_t ahdrLevelRaw()  const noexcept { return ahdr_.level; }
    uint32_t env2LevelRaw() const noexcept { return env2_.level; }
};

} // namespace mnm
