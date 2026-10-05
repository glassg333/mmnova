// ============================================================================
// MnmVoiceFilterDist.h — собирающий модуль: ФИЛЬТР (два фильтра + Q + env)
//                         + ДИСТОРШН + AMP-гейт, готовый для импорта в JUCE
//                         Monomachine SFX-60 OS 1.32B
// ----------------------------------------------------------------------------
// Тракт (порядок подтверждён владельцем машины и мануалом OS 1.32):
//
//   in ──► аттенюация DIST (knob<0 ослабляет вход) ──►
//   [ФИЛЬТР: кольцо коэффициентов (BASE/WDTH + env BOFS/WOFS + трекинг)
//            → каскад func_000340 (гибрид FIR+SVF, halfband 2×;
//            Q — через интерполятор тон-пути $0629, включение OPEN п.6 NOTES)] ──►
//   [DIST: ступень k = 0.98379·x0²+0.01621, усиление 4k, жёсткий клип ±1.0;
//          кривая привода c затемняет срез следующего кадра] ──►
//   [AMP env AHDR: гейт громкости выходящего звука] ──► out
//
// ПАРАМЕТРЫ (страница FLT, ручки 0..127 → слово = knob<<16):
//   BASE  — срез нижнего фильтра (low cut)      [V-$20]
//   WDTH  — ширина «створа» → срез верхнего (hi cut = BASE+WDTH)  [V-$1F]
//   HPQ   — резонанс верхнего (high-pass)       [V-$1E]
//   LPQ   — резонанс нижнего (low-pass)         [V-$1D]
//   ATK   — атака фильтровой огибающей          [V-$1C]
//   DEC   — спад фильтровой огибающей           [V-$1B]
//   BOFS  — глубина env → BASE (биполярно, центр 64)   [V-$1A]
//   WOFS  — глубина env → WDTH (биполярно, центр 64)   [V-$19]
//   DIST  — дисторшн (−64..+63, БЕЗ огибающих)         [V-$24]
//   ATK/HOLD/DEC/REL/SUS AMP — гейт (страница AMP)
//
// СЕТКА: кадр = 16 сэмплов (родная сетка машины). Латентность фильтра = 1 кадр.
//
// НЕ ВХОДИТ (намеренно, чтобы не перепутать — см. DECOMPILATION_NOTES.md):
//   * track delay и его модуляция (P:$0939-$0B49: DTIM/DSND/DFB/DBAS/DWID,
//     таблицы $144AC7/$144B48 — у делейных фильтров НЕТ Q и НЕТ огибающих);
//   * «rotation resonator» stage2 (P:$0A5D-$0AD0) — это машина delay-модуляции;
//   * FM-машины и их огибающие freq 1/2/3;
//   * EQ-полоса (EQF/EQG, P:$0789-$0859).
// ============================================================================
#pragma once
#include <array>
#include <cmath>
#include <algorithm>
#include "mnm_fixed.h"
#include "mnm_filter_env.h"
#include "mnm_filter_coeff.h"
#include "mnm_cascade340.h"
#include "mnm_dist.h"
#include "mnm_amp_env.h"
// ИЗОЛИРОВАННЫЙ модуль Q (namespace mnmq): q-интерполятор + глай-тройки.
// Включение РАСЧЁТА — макросом MNM_ENABLE_EXACT_Q (по умолчанию ВЫКЛЮЧЕН,
// поведение тракта не меняется). Включение В ЗВУК — после порта движков
// func_000350/func_000365 (ступени A/B каскада) и регрессии на векторах
// reference/svf_trace_results.json (E1/E3/E4).
#include "mnm_filter_q.h"

namespace mnm {

class MnmVoiceFilterDist
{
public:
    // ---------- параметры страниц (слова Q1.23, обычно knob<<16) ------------
    void setFiltWords(uint32_t base, uint32_t wdth, uint32_t hpq, uint32_t lpq,
                      uint32_t atk, uint32_t dec, uint32_t bofs, uint32_t wofs) noexcept
    {
        baseW_ = base & 0xFFFFFFu;
        wdthW_ = wdth & 0xFFFFFFu;
        hpqW_  = hpq  & 0xFFFFFFu;
        lpqW_  = lpq  & 0xFFFFFFu;
        fenv_.setParams(atk, dec);
        bofsW_ = bofs & 0xFFFFFFu;
        wofsW_ = wofs & 0xFFFFFFu;
        recomputeBypass();
    }

    // ПОРТ-СОГЛАШЕНИЕ (не прошивка): дефолтные ручки (BASE=0, WDTH=127,
    // HPQ=LPQ=0) = прозрачный THRU, чтобы включение блока не меняло громкость.
    // Активируется только setBypassAtDefaults(true).
    void recomputeBypass() noexcept
    {
        bypassed_ = bypassAtDefaults_
                 && (baseW_ >> 16) == 0 && (wdthW_ >> 16) >= 127
                 && (hpqW_  >> 16) == 0 && (lpqW_  >> 16) == 0;
    }
    // ручки 0..127 (биполярные BOFS/WOFS: 64 = нейтраль)
    static uint32_t wordFromKnob(int knob) noexcept
    {
        knob = std::clamp(knob, 0, 127);
        return (uint32_t)knob << 16;
    }

    void setDistKnob(int knobMinus64Plus63) noexcept { dist_.setKnob(knobMinus64Plus63); }
    void setAmpWords(uint32_t atk, uint32_t hold, uint32_t dec, uint32_t rel, uint32_t sus) noexcept
    {
        aenv_.setParams(atk, hold, dec, rel, sus);
    }

    // FILTER-trig (мануал: фильтровая огибающая триггерится FILTER-триггером;
    // в ядре — сброс фазы при Y:(V-$08)==1)
    void trigger() noexcept { fenv_.trigger(); }
    // нота OFF → AMP гейт уходит в release (фильтровая огибающая ядра
    // отдельной REL-ветки не имеет)
    void release() noexcept { aenv_.release(); fenv_.release(); }

    void setSampleRate(double sr) noexcept { (void)sr; } // сетка/таблицы прошивки 44.1к
    void setTracking(bool hpTrack, bool lpTrack) noexcept { tf_.hpTrack = hpTrack; tf_.lpTrack = lpTrack; }
    void setPitchTrackingWords(int32_t pitchAccWords) noexcept { tone_.pitchAcc = pitchAccWords; }
    void setBypassAtDefaults(bool b) noexcept { bypassAtDefaults_ = b; recomputeBypass(); }

    void reset() noexcept
    {
        casc_[0].reset(); casc_[1].reset();
        fenv_.reset(); aenv_.reset(); dist_.reset();
        tone_ = TonePathState{};
        cnt_ = 0;
        inBufL_.fill(0.f); inBufR_.fill(0.f);
        frameL_.fill(0.f); frameR_.fill(0.f);
        outL_.fill(0.f);   outR_.fill(0.f);
        prevL_.fill(0.f);  prevR_.fill(0.f);
    }

    // ---------- потоковый ввод: по одному сэмплу (L и R) ---------------------
    // Возвращает сэмплы ПРЕДЫДУЩЕГО кадра (латентность 16 сэмплов — сетка
    // прошивки: каскад считает кадр из всех 16 входов).
    inline float processL(float inL) noexcept { return push(0, inL); }
    inline float processR(float inR) noexcept { return push(1, inR); }

private:
    inline float push(int ch, float x) noexcept
    {
        if (bypassed_) return x;
        float out = (ch ? prevR_ : prevL_)[(size_t)cnt_];
        (ch ? inBufR_ : inBufL_)[(size_t)cnt_] = x;
        if (++cnt_ < kFrame) return out;

        cnt_ = 0;
        runFrame();
        prevL_.swap(outL_);
        prevR_.swap(outR_);
        return out;
    }

    // ---------- один кадр (16 сэмплов) — блочная сетка ядра -----------------
    void runFrame() noexcept
    {
        // 1) тики огибающих (раз в кадр, как в ядре)
        fenv_.tickBlock();
        aenv_.tickBlock();
        const float env = fenv_.phase();

        // 2) слова BASE/WDTH с env-модуляцией BOFS/WOFS (PORT WIRING —
        //    env-члены входят в индекс среза до кольца; домен — см. шапку
        //    mnm_filter_coeff.h)
        const uint32_t baseW = modulateWord(baseW_, bofsW_, env);
        const uint32_t wdthW = modulateWord(wdthW_, wofsW_, env);

        // 3) кольцо коэффициентов (бит-точное) + tone-путь (индекс среза)
        FilterRing ring = computeRing(baseW, wdthW);
        const int32_t toneIdx = computeToneIndex(baseW, wdthW, bofsW_, wofsW_,
                                                 env, tf_, tone_);
        // 4) связка DIST→срез (P:$07B9/$07DB): кривая привода c УМНОЖАЕТ
        //    коэффициент среза — больше drive → меньше c → темнее срез.
        //    В ядре c умножает kCutoff[$143546][idx]; в порту применена
        //    к выходному коэффициенту f кольца (нейтрально 1.0 при DIST≤0).
        ring.c[0] *= dist_.c();
        lastCutoffCoef_ = cutoffCoefficient(toneIdx, dist_.c()); // диагностика
        // Q (HPQ/LPQ): ОТКРЫТЫЙ П.6 NOTES ЗАКРЫТ — цепь верифицирована прогоном
        // (svf_trace.py E1/E3/E4, 2026-10-05): Q-слова — КРОСС: ячейка $40B (LPQ)
        // кормит интерполятор фильтра A, $40A (HPQ) — фильтра B; q-слова и
        // глай-тройки считает изолированный mnmq::QInterpolator (mnm_filter_q.h).
        // Расчёт включается флагом MNM_ENABLE_EXACT_Q; подача троек в движки
        // ступеней A/B — после их порта (func_000350/000365, NOTES §9).

        // 5) аттенюация входа (DIST<0): ослабляется вход тракта ДО фильтра —
        //    фиксированная точка каскада меньше упирается в потолок
        const float att = dist_.inputGain();
        if (att != 1.0f)
            for (int i = 0; i < kFrame; ++i)
            {
                inBufL_[(size_t)i] *= att;
                inBufR_[(size_t)i] *= att;
            }

        // 6) каскад func_000340 (L/R) — бит-точный
        casc_[0].processFrame(inBufL_.data(), frameL_.data(), ring.c);
        casc_[1].processFrame(inBufR_.data(), frameR_.data(), ring.c);

        // 7) DIST-ступень + AMP-гейт, по сэмплам
        const float gate = aenv_.level();
        for (int i = 0; i < kFrame; ++i)
        {
            float l = frameL_[(size_t)i];
            float r = frameR_[(size_t)i];
            l = dist_.process(l);                      // ступень 4k + клип ±1.0
            r = dist_.process(r);
            outL_[(size_t)i] = l * gate;               // AMP-гейт всего выходящего
            outR_[(size_t)i] = r * gate;
        }
    }

    // env-модуляция слова: word + envLevel·глубина·масштаб (порт-домен кольца).
    // BOFS/WOFS биполярны (центр 64): глубина = (knob−64)/64 ∈ [−1..1].
    // Env-ход в tone-домене = 4·$700·env·depth² (макс 1792, таблица $143546 =
    // 1728 слов); пересчёт в слова домена кольца: ·2^23/1728 (отношение
    // полных ходов таблиц $143546 и $143F95).
    static uint32_t modulateWord(uint32_t word, uint32_t ofsWord, float envPhase) noexcept
    {
        const float depth = (float)((int32_t)(ofsWord >> 16) - 64) / 64.0f;
        const double envIdx = 4.0 * (double)0x700 * (double)envPhase * depth * depth;
        const double ringWords = envIdx * (8388608.0 / 1728.0);
        int32_t w = (int32_t)(word & 0xFFFFFFu) + (int32_t)llround(ringWords);
        if (w < 0) w = 0;
        if (w > 0x7FFFFF) w = 0x7FFFFF;
        return (uint32_t)w;
    }

    // --- параметры-слова ------------------------------------------------------
    uint32_t baseW_ = 0, wdthW_ = 127u << 16, hpqW_ = 0, lpqW_ = 0;
    uint32_t bofsW_ = 64u << 16, wofsW_ = 64u << 16;
    // --- блоки ----------------------------------------------------------------
    MnmFilterEnv  fenv_;
    MnmAmpEnv     aenv_;
    Cascade340    casc_[2];
    MnmDist       dist_;
    TonePathState tone_;
    TrackingFlags tf_ {};
#ifdef MNM_ENABLE_EXACT_Q
    // изолированная Q-цепь (НЕ входит в тракт до порта ступеней A/B):
    // фильтр A ← ячейка $40B (LPQ), фильтр B ← ячейка $40A (HPQ) — кросс (E1)
    mnmq::QInterpolator qIntA_{false};
    mnmq::QInterpolator qIntB_{true};
    mnmq::QFrameResult  qLastA_{}, qLastB_{};
    bool qInit_ = false;
#endif
    // --- кадровые буферы ------------------------------------------------------
    std::array<float, kFrame> inBufL_{}, inBufR_{};
    std::array<float, kFrame> frameL_{}, frameR_{};
    std::array<float, kFrame> outL_{},  outR_{};
    std::array<float, kFrame> prevL_{}, prevR_{};
    int  cnt_ = 0;
    bool bypassAtDefaults_ = false;
    bool bypassed_ = false;
    float lastCutoffCoef_ = 0.f;   // диагностика: kCutoff[idx]·c последнего кадра
public:
    float lastCutoffCoefficient() const noexcept { return lastCutoffCoef_; }

    // --- изолированный расчёт Q (флаг MNM_ENABLE_EXACT_Q) -------------------
    // Один вызов на кадр ПОСЛЕ computeRing (индексы/слова уже посчитаны).
    // idxA = тон-индекс фильтра A (y:(r6+$DA)-домен), idxB — кейтрек-копия
    // фильтра B (x:(r6+$D9)); qWordA = ячейка $40B (LPQ), qWordB = $40A (HPQ).
#ifdef MNM_ENABLE_EXACT_Q
    void computeQFrame(uint32_t idxA, uint32_t idxB,
                       uint32_t qWordA, uint32_t qWordB,
                       uint32_t param4Word) noexcept
    {
        mnmq::QFrameInputs ia{ idxA, qWordA, param4Word };
        mnmq::QFrameInputs ib{ idxB, qWordB, param4Word };
        qLastA_ = qIntA_.processFrame(ia);
        qLastB_ = qIntB_.processFrame(ib);
        qInit_ = true;
    }
    const mnmq::QFrameResult& lastQFrameA() const noexcept { return qLastA_; }
    const mnmq::QFrameResult& lastQFrameB() const noexcept { return qLastB_; }
    bool qFrameValid() const noexcept { return qInit_; }
#endif
};

} // namespace mnm
