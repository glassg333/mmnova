#pragma once
// =============================================================================
// MnmFilter.h — Monomachine OS 1.32B, FILT-страница голоса (BASE WDTH HPQ LPQ
// ATK DEC BOFS WOFS): двухполюсный SVF-фильтр, два экземпляра на голос.
//
// Доказано по ядру DSP1 OS 1.32B (см. docs/KERNEL_CHAIN_RU.md):
//  1. Фильтр — 2-pole state-variable (TPT), пер-сэмпловая петля на 16 кадров:
//     func_00037C/func_00038A (P:$037C-$0397) + развёрнутые копии P:$07E7-$07FB
//     и P:$07F0-$07FB. Два независимых набора состояний (X:$72/$74 и X:$B2/$B4)
//     = два фильтра (HP-ветка и LP-ветка) либо L/R.
//  2. Коэффициент среза — ИЗ ТАБЛИЦЫ ПРОШИВКИ P:$143546 (≈1700 записей,
//     экспонента 2.8 Гц → Nyquist). Значение таблицы = 2*sin(pi*fc/fs) —
//     классический "g" TPT-SVF. Индекс = min(вычисленный, $6A3=1699).
//     Индекс собирается ядром из параметров (BASE/WDTH/оффсеты/огибающая)
//     через таблицы-делители X:$143F95 / X:$144446 и 24-битное деление
//     (P:$0583-$0589, do #<$18 + div).
//  3. Коэффициенты обновляются РАЗ В БЛОК со сглаживанием к цели:
//        coef += (target - coef), округлённое до 1/256 доли  (P:$056D-$0572)
//     — «зализывание» перестройки фильтра, слышимый характер MNM.
//  4. Резонанс — HPQ/LPQ параметры в коэффициенте k (второй набор таблиц
//     P:$141A98 / P:$142158 / P:$142F06 читается интерполятором P:$0629-$0668).
//
// Статус: структура петли — [EXACT-structure]; связь ручек BASE/WDTH/HPQ/LPQ
// с индексами таблиц — в ColdFire-скейлинге, маппинг закрывается rig'ом
// (docs «открытое»). Таблица среза прилагается как есть (data/kernel).
// =============================================================================

#include <cstdint>
#include <algorithm>
#include <cmath>

namespace mnmdsp {

// -----------------------------------------------------------------------------
// TPT 2-pole SVF — структура пер-сэмпловой петли прошивки (func_00037C).
//   a' = a + g*x          (интегратор 1)
//   ... с локальной обратной связью по резонансу k (HPQ/LPQ)
// Эквивалент известного "zero-delay" SVF:
//   lp' = lp + g*bp
//   bp' = bp + g*(hp) ,  hp = x - lp - k*bp
// Коэффициент g берётся из таблицы прошивки (2*sin(pi*fc/fs)).
// -----------------------------------------------------------------------------
class MnmSvf
{
public:
    void reset() { m_lp = 0; m_bp = 0; }

    void setG(float g2sin) { m_g = std::clamp(g2sin, 0.0f, 1.99f); } // из таблицы
    void setK(float resonance) { m_k = std::clamp(resonance, 0.0f, 1.9f); }

    float lp(float x)
    {
        const float hp = x - m_lp - m_k * m_bp;
        m_bp += m_g * hp;
        m_lp += m_g * m_bp;
        return m_lp;
    }
    float hp(float x)
    {
        const float h = x - m_lp - m_k * m_bp;
        m_bp += m_g * h;
        m_lp += m_g * m_bp;
        return h;
    }
    float bp(float x) { lp(x); return m_bp; }

private:
    float m_g = 0.0f, m_k = 0.0f, m_lp = 0.0f, m_bp = 0.0f;
};

// -----------------------------------------------------------------------------
// Коэффициент среза из таблицы прошивки.
// Таблица P:$143546: ~1700 записей, экспоненциальный маппинг 2.8 Гц → Nyquist,
// значение = 2*sin(pi*fc/fs) в Q1.23. Индекс голоса ограничивается $6A3 (1699).
// -----------------------------------------------------------------------------
class MnmCutoffTable
{
public:
    static constexpr int kMaxIndex = 0x6A3; // 1699 — кламп ядра (P:$07D6)

    void load(const uint32_t* table1700) { m_tbl = table1700; }

    // [EXACT] сглаживание коэффициента раз в блок: coef += (target-coef),
    // дельта округляется до 8 дробных бит (P:$056D-$0572: sub/asl#8/rnd/asr#8/add)
    float stepToward(int targetIndex)
    {
        const int idx = std::clamp(targetIndex, 0, kMaxIndex);
        const float target = wordToFloat(m_tbl[idx]);
        // Q1.23: дельта округляется до 1/256, затем добавляется
        float d = target - m_cur;
        d = std::round(d * 256.0f) / 256.0f;
        if (std::abs(d * 8388608.0f) < 1.0f) d = target - m_cur; // rnd-квант не двигает — берём цель
        m_cur += d * (1.0f / 1.0f);
        // прошивка делает один такой шаг на блок (не доходит до цели мгновенно)
        return m_cur;
    }

    float current() const { return m_cur; }
    void  snap(int targetIndex)
    {
        const int idx = std::clamp(targetIndex, 0, kMaxIndex);
        m_cur = wordToFloat(m_tbl[idx]);
    }

    static float wordToFloat(uint32_t v)
    {
        int32_t s = (v & 0x800000) ? int32_t(v | 0xFF000000u) : int32_t(v);
        return float(double(s) / 8388608.0);
    }

private:
    const uint32_t* m_tbl = nullptr;
    float m_cur = 0.0f;
};

// -----------------------------------------------------------------------------
// Голосовой фильтр FILT-страницы: HP(BASE) и LP(BASE+WDTH) ветки,
// по одной паре состояний на канал в прошивке (X:$72/$74, X:$B2/$B4).
// -----------------------------------------------------------------------------
class MnmVoiceFilter
{
public:
    void reset() { m_hp.reset(); m_lp.reset(); }

    void setResonance(float hpqNorm, float lpqNorm)
    {
        m_hp.setK(0.02f + 1.4f * hpqNorm);   // характер резонанса SVF;
        m_lp.setK(0.02f + 1.4f * lpqNorm);   // точный закон — из таблиц HPQ/LPQ
    }

    void setCoefficientTargets(int hpIndex, int lpIndex)
    {
        m_hpTarget = hpIndex;
        m_lpTarget = lpIndex;
    }

    void attachTable(const uint32_t* tbl)
    {
        m_ctHp.load(tbl); m_ctHp.snap(0);
        m_ctLp.load(tbl); m_ctLp.snap(0);
    }

    // раз в блок (16 кадров): сглаживаем коэффициенты к целям
    void beginBlock()
    {
        m_gHp = m_ctHp.stepToward(m_hpTarget);
        m_gLp = m_ctLp.stepToward(m_lpTarget);
        m_hp.setG(m_gHp);
        m_lp.setG(m_gLp);
    }

    // пер-сэмплово: HP(BASE) → LP(BASE+WDTH) — базовая цепочка FILT
    float processSample(float x)
    {
        float y = m_hp.hp(x);
        y = m_lp.lp(y);
        return y;
    }

private:
    MnmSvf m_hp, m_lp;
    MnmCutoffTable m_ctHp, m_ctLp;
    int m_hpTarget = 0, m_lpTarget = 0;
    float m_gHp = 0.f, m_gLp = 0.f;
};

} // namespace mnmdsp
