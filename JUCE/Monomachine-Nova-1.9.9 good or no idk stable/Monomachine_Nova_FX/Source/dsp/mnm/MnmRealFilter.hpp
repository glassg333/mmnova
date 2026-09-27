#pragma once
// =============================================================================
// MnmRealFilter.hpp -- основной фильтр (режим DSP MODE -> "mnm")
// Monomachine Nova 1.6.2, ревизия v4
// =============================================================================
// Источник данных: decompiled data/mnm_filter_full_dump_juce_real
//   src/MnmFilter.h          -> локальная копия MnmFilterRealDump.hpp
//   src/mnm_filter_tables.h  -> локальная копия MnmFilterTablesReal.hpp
//
// ЧТО ЗДЕСЬ РЕАЛЬНОЕ (взято из прошивки, без выдумок):
//   * kLP_filter_coeffs[258] (P:$144AC7) -- таблица коэффициентов. Кривая 1 (0..127)
//     задаёт стадии BASE/WIDTH голосового фильтра, кривая 2 (129..256) -- aux-стадию.
//     Коэффициент a = raw/2^23; сам фильтр -- MnmOnePole (kernel P:$A5F-$ABF):
//         LP: y = (1-a)*y_prev + a*x        HP: y = x - состояние
//   * Структура голосового фильтра: HP(BASE) -> LP(BASE+WIDTH) -> aux(кривая 2).
//   * Огибающая фильтра: BASE' = BASE + env*BOFS, WIDTH' = WIDTH + env*WOFS,
//     шаг огибающей раз в 16 сэмплов (kBlock) -- каденция прошивки,
//     таблицы ATK/DEC прошивки (kEnvAttackRate/kEnvDecayRate).
//
// ЧТО ЗДЕСЬ ПОРТ (в прошивке это per-voice work-area Y:$91+, в дампе помечено TODO):
//   * Резонанс HPQ/LPQ. Полные коэффициенты Q по этим адресам не восстановлены,
//     поэтому резонанс сделан отдельными ZDF/TPT-SVF стадиями на тех же частотах,
//     что и табличные стадии. Закон ручки: Q = 0.5 .. kMaxQ (16) -- документированная
//     аппроксимация порта, а не подлинный коэффициент прошивки. Константа kMaxQ
//     в одном месте, меняется одной строкой.
//
// ПОЧЕМУ TPT, А НЕ Chamberlin (как было в v2/v3 и в прежнем FilterCore):
//   классический Chamberlin SVF уходит в разнос, когда g^2 + 2*g*d >= 4, то есть
//   при большом срезе вместе с большой добротностью (на 44.1 кГц это срез выше
//   ~20 кГц при Q > ~30). Именно это давало "иногда взрывается / кричит".
//   TPT (topology-preserving transform, Zavalishin) устойчив при любом срезе < fs/2
//   и любом Q > 0, поэтому разнос невозможен ни при каких положениях ручек.
//   Дополнительно: мягкий ограничитель выхода (страховка на очень горячем сигнале)
//   и DC-блокер 8 Гц, чтобы на выходе не было постоянной составляющей.
//
// ВАЖНО ПРО АРГУМЕНТЫ: порядок как на странице FILTER прошивки
//   BASE, WDTH, HPQ, LPQ, ATK, DEC, BOFS, WOFS
// то есть ровно как у старого MonomachineFilter. BOFS/WOFS биполярные (64 = 0).
// В v3 здесь был порядок (..., BOFS, WOFS, ATK, DEC), и вызов в NovaDSP.h
// передавал значения перепутанными -- из-за этого ручки ATK/DEC двигали срез,
// а BOFS/WOFS меняли скорость огибающей. Это и был баг "по частотам".
//
// Файл -- ЕДИНСТВЕННОЕ место, где определён monomachine::mnm::FilterCore.
// =============================================================================

#include "MnmKernel.hpp"          // kBlock, kDspRate, kEnvAttackRate/kEnvDecayRate, FilterCoreLegacy
#include "MnmFilterRealDump.hpp"  // ::mnm::MnmOnePole + таблицы прошивки (kLP_filter_coeffs, kSVF_x0/x1)
#include "../hybrid_private/HybridDSP.hpp" // private K35, ladder, and attributed R-classic hybrid modules
#include <limits>

namespace monomachine {
namespace mnm {

// Реальные константы SVF из прошивки (те же значения, что в дампе).
// Значения: -0.084747577 и +0.581161499 (24-битные слова $F528BD / $4A4DF0).
inline constexpr float kRealSVF_x0_F528BD = ::mnm::kSVF_x0;
inline constexpr float kRealSVF_x1_4A4DF0 = ::mnm::kSVF_x1;
inline constexpr int   kRealLPTableSize   = 258;
inline constexpr int   kRealLPCurve2Base  = 129;

namespace real_detail {

inline float clampf(float v, float lo, float hi) noexcept { return v < lo ? lo : (v > hi ? hi : v); }

// Мягкий ограничитель: ниже ~ceiling/4 прозрачен, выше асимптотически стремится к ceiling.
// Это страховка порта, а не часть алгоритма прошивки.
inline float softClip(float x, float ceiling) noexcept {
    const float a = x < 0.0f ? -x : x;
    if (a < ceiling * 0.25f) return x;
    return (x < 0.0f ? -1.0f : 1.0f) * (ceiling - (ceiling * 0.75f) / (1.0f + (a - ceiling * 0.25f) * 4.0f / ceiling));
}

// Коэффициент 1-полюсного фильтра прямо из таблицы прошивки (кривая 1).
inline float tableCoefCurve1(int tone) noexcept {
    const int t = static_cast<int>(clampf(static_cast<float>(tone), 0.0f, 127.0f));
    return static_cast<float>(::mnm::kLP_filter_coeffs[t]) / 8388608.0f;
}
// Кривая 2 (aux-стадия), индексы 129..256.
inline float tableCoefCurve2(int tone) noexcept {
    const int t = static_cast<int>(clampf(static_cast<float>(tone), 0.0f, 127.0f));
    return static_cast<float>(::mnm::kLP_filter_coeffs[kRealLPCurve2Base + t]) / 8388608.0f;
}

// Обратная формула дампа (MnmOnePole::cutoffHz): коэффициент a -> частота -3 дБ.
// Точна для закона y = (1-a)*y_prev + a*x. Возвращает NaN, если точки -3 дБ
// в звуковом диапазоне нет (a близко к 1 => фильтр фактически открыт).
inline float coefToCutoffHz(float a, double fs) noexcept {
    if (a <= 0.0f || a >= 1.0f) return std::nanf("");
    const float num = 2.0f - 2.0f * a - a * a;
    const float den = 2.0f * (1.0f - a);
    if (den == 0.0f) return std::nanf("");
    const float c = num / den;
    if (c < -1.0f || c > 1.0f) return std::nanf("");
    const float omega = std::acos(c);
    return omega * static_cast<float>(fs) / (2.0f * 3.14159265358979323846f);
}

// TONE (0..127) -> срез ЧЕРЕЗ РЕАЛЬНУЮ ТАБЛИЦУ прошивки.
// open (необязательный) = true, когда -3 дБ выше Найквиста, то есть таблица
// фактически "открыта" (высокие WIDTH) -- тогда возвращается безопасный верх.
inline float knobToCutoffHz(float knob, double sr, bool* open = nullptr) noexcept {
    if (open) *open = false;
    const float a = tableCoefCurve1(static_cast<int>(clampf(knob, 0.0f, 127.0f) + 0.5f));
    const float fc = coefToCutoffHz(a, kDspRate);
    if (!std::isfinite(fc)) { if (open) *open = true; return static_cast<float>(sr) * 0.45f; }
    return clampf(fc, 10.0f, static_cast<float>(sr) * 0.45f);
}

// ---- Закон резонанса (порт) --------------------------------------------------
// Прошивочный Q не восстановлен (см. шапку), поэтому здесь явно документированная
// аппроксимация: квадратичный закон от 0.5 до kMaxQ. Начало ручки (0..0.5) --
// резонансной стадии нет вообще, сигнал идёт строго через реальные таблицы.
inline constexpr float kMaxQ         = 16.0f;   // максимум добротности (~+24 дБ)
inline constexpr float kMinQ         = 0.5f;
inline constexpr float kQKnobActive  = 0.5f;    // ручка HPQ/LPQ <= 0.5 => стадия выключена
inline constexpr float kDcBlockHz    = 8.0f;    // DC-блокер на выходе активного фильтра
inline constexpr float kOutCeiling   = 8.0f;    // страховочный ограничитель выхода (+18 дБ)

inline float knobToQ(float q) noexcept {
    const float n = clampf(q, 0.0f, 127.0f) / 127.0f;
    return kMinQ + (kMaxQ - kMinQ) * n * n;
}

} // namespace real_detail

// -----------------------------------------------------------------------------
// MnmResoSVF -- устойчивая резонансная стадия (TPT/ZDF SVF).
// Устойчива при любом fc < fs/2 и любом Q > 0; разноса не бывает.
// -----------------------------------------------------------------------------
class MnmResoSVF {
public:
    void reset() noexcept { ic1_ = 0.0f; ic2_ = 0.0f; }

    void setParams(float fcHz, float q, double fs) noexcept {
        const float rate = (fs > 0.0) ? static_cast<float>(fs) : static_cast<float>(kDspRate);
        const float fc   = real_detail::clampf(fcHz, 5.0f, rate * 0.499f);
        g_ = std::tan(3.14159265358979323846f * fc / rate);          // предыскажение частоты
        k_ = 1.0f / real_detail::clampf(q, 0.2f, 200.0f);            // демпфирование = 1/Q
        const float a1 = 1.0f / (1.0f + g_ * (g_ + k_));
        a1_ = a1; a2_ = g_ * a1; a3_ = g_ * a2_;
    }

    inline void process(float x, float& lp, float& bp, float& hp) noexcept {
        const float v3 = x - ic2_;
        const float v1 = a1_ * ic1_ + a2_ * v3;
        const float v2 = ic2_ + a2_ * ic1_ + a3_ * v3;
        ic1_ = 2.0f * v1 - ic1_;
        ic2_ = 2.0f * v2 - ic2_;
        lp = v2; bp = v1; hp = x - k_ * v1 - v2;
    }

    float g() const noexcept { return g_; }
    float damping() const noexcept { return k_; }

private:
    float g_ = 0.1f, k_ = 1.0f;
    float a1_ = 1.0f, a2_ = 0.0f, a3_ = 0.0f;
    float ic1_ = 0.0f, ic2_ = 0.0f;
};

// -----------------------------------------------------------------------------
// RealFilterCore -- основной режим "mnm".
// API 1:1 с прежним FilterCore, поэтому NovaDSP.h подключается без изменений
// (кроме исправленного порядка аргументов в вызове setParameters).
// -----------------------------------------------------------------------------
class RealFilterCore {
public:
    void reset() noexcept {
        for (int c = 0; c < 2; ++c) {
            hp1_[c].reset(); lp2_[c].reset(); aux_[c].reset();
            svfHp_[c].reset(); svfLp_[c].reset(); dc_[c].reset();
        }
        hybridK35Lower_.reset(); hybridK35Upper_.reset();
        hybridLadderLower_.reset(); hybridLadderUpper_.reset();
        hybridReferenceLower_.reset(); hybridReferenceUpper_.reset();
        envLevel_ = 0.0f; envActive_ = false; released_ = false; blockCounter_ = 0;
        apply();
    }

    void setSampleRate(double sr) noexcept {
        const double nextHost = sr > 0.0 ? sr : kDspRate;
        // TrackChain receives a fresh DSP-mode snapshot at every host audio
        // callback. A sample-rate setup is stateful for the imported Korg,
        // Odin, and R-classic adapters: prepare() clears their integrators.
        // Treat an unchanged rate as a no-op so changing neither the rate nor
        // the selected mode can never turn the host block size into an audible
        // reset cadence.
        if (sampleRateConfigured_ && std::abs(nextHost - host_) < 1.0e-9)
            return;
        host_ = nextHost;
        for (int c = 0; c < 2; ++c) dc_[c].setSampleRate(host_);
        hybridK35Lower_.prepare(host_); hybridK35Upper_.prepare(host_);
        hybridLadderLower_.prepare(host_); hybridLadderUpper_.prepare(host_);
        hybridReferenceLower_.prepare(host_); hybridReferenceUpper_.prepare(host_);
        sampleRateConfigured_ = true;
        apply();
    }

    // Порядок аргументов = порядок страницы FILTER прошивки:
    // BASE, WDTH, HPQ, LPQ, ATK, DEC, BOFS, WOFS (BOFS/WOFS биполярные, 64 = 0).
    void setParameters(float base, float width, float hpq, float lpq,
                       float envAtk, float envDec, float bofs, float wofs) noexcept {
        baseParam_ = base;   widthParam_ = width;
        hpqParam_  = hpq;    lpqParam_   = lpq;
        atkParam_  = envAtk; decParam_   = envDec;
        bofsParam_ = bofs;   wofsParam_  = wofs;
        apply();
    }

    // aux-стадия (кривая 2). По умолчанию 127 = выключена. Значение 0 повторяет
    // буквальный дефолт MnmBaseWidthFilter из дампа (aux ~240 Гц), если нужно ровно так.
    void setAuxKnob(int knob) noexcept { auxKnob_ = knob < 0 ? 0 : (knob > 127 ? 127 : knob); apply(); }

    // Canonical IDs 0..8 remain the 1.9.8 contract. New IDs 9..20 append
    // R 303, R MS20, and R MOOG LP/BP/HP forms. BASE remains the physical
    // lower cutoff and BASE+WDTH the physical upper cutoff for every choice.
    void setHybridTestModes(int lowerMode, int upperMode) noexcept {
        constexpr int maxMode = nova::hybrid_private::kHybridFilterLastAlgorithm;
        const int lower = std::clamp(lowerMode, 0, maxMode);
        const int upper = std::clamp(upperMode, 0, maxMode);
        if (lower != hybridLowerMode_) {
            hybridLowerMode_ = lower;
            hybridK35Lower_.reset(); hybridLadderLower_.reset(); hybridReferenceLower_.reset();
        }
        if (upper != hybridUpperMode_) {
            hybridUpperMode_ = upper;
            hybridK35Upper_.reset(); hybridLadderUpper_.reset(); hybridReferenceUpper_.reset();
        }
        apply();
    }

    // Explicit new-control path. Values are neutral by default, so the
    // reconstructed MNM path keeps its exact former coefficients unless the
    // user enables VEL/KT or the separate FIL ENV. Semitone offsets apply to
    // the physical lower/upper edges after the existing BASE/WDTH law.
    void setExternalFilterModifiers(float lowerSemitones, float upperSemitones,
                                    float baseOffset, float widthOffset) noexcept {
        lowerSemitones = real_detail::clampf(lowerSemitones, -128.0f, 128.0f);
        upperSemitones = real_detail::clampf(upperSemitones, -128.0f, 128.0f);
        baseOffset = real_detail::clampf(baseOffset, -127.0f, 127.0f);
        widthOffset = real_detail::clampf(widthOffset, -127.0f, 127.0f);
        if (std::abs(lowerSemitones - externalLowerSemitones_) < 1.0e-6f
            && std::abs(upperSemitones - externalUpperSemitones_) < 1.0e-6f
            && std::abs(baseOffset - externalBaseOffset_) < 1.0e-6f
            && std::abs(widthOffset - externalWidthOffset_) < 1.0e-6f) return;
        externalLowerSemitones_ = lowerSemitones;
        externalUpperSemitones_ = upperSemitones;
        externalBaseOffset_ = baseOffset;
        externalWidthOffset_ = widthOffset;
        apply();
    }

    void trigger() noexcept { envActive_ = true; released_ = false; envLevel_ = 0.0f; blockCounter_ = 0; apply(); }
    // Отпускание: огибающая ОБЯЗАНА вернуться к 0, иначе модуляция "зависает"
    // на последнем уровне (в v3 это давало постоянное смещение среза после нот).
    void release() noexcept { released_ = true; }

    inline float process(int ch, float x) noexcept {
        // Каденция прошивки: шаг огибающей раз в kBlock сэмплов, считаем по нулевому каналу.
        if (ch == 0) {
            if (blockCounter_ == 0 && (envActive_ || envLevel_ > 0.0f)) envStep();
            if (++blockCounter_ >= kBlock) blockCounter_ = 0;
        }
        if (bypass_) return x;                       // unity THRU (дефолтное положение)
        const int c = (ch == 0) ? 0 : 1;

        float y = x;
        if (hybridLowerMode_ == 0) {
            if (resHpActive_) { float lp, bp, hp; svfHp_[c].process(y, lp, bp, hp); y = hp; }
            y = hp1_[c].processHP(y);                // native lower / BASE side
        } else if (nova::hybrid_private::hybridUsesKorg35(hybridLowerMode_)) {
            y = hybridK35Lower_.process(c, y);
        } else if (nova::hybrid_private::hybridUsesOdinLadder(hybridLowerMode_)) {
            y = hybridLadderLower_.process(c, y);
        } else {
            y = hybridReferenceLower_.process(c, y);
        }
        if (hybridUpperMode_ == 0) {
            y = lp2_[c].processLP(y);                // native upper / BASE+WDTH side
            if (auxActive_) y = aux_[c].processLP(y);// native curve-2 aux remains intact
            if (resLpActive_) { float lp, bp, hp; svfLp_[c].process(y, lp, bp, hp); y = lp; }
        } else if (nova::hybrid_private::hybridUsesKorg35(hybridUpperMode_)) {
            y = hybridK35Upper_.process(c, y);
            if (auxActive_) y = aux_[c].processLP(y);// aux is neither selected side
        } else if (nova::hybrid_private::hybridUsesOdinLadder(hybridUpperMode_)) {
            y = hybridLadderUpper_.process(c, y);
            if (auxActive_) y = aux_[c].processLP(y);// aux is neither selected side
        } else {
            y = hybridReferenceUpper_.process(c, y);
            if (auxActive_) y = aux_[c].processLP(y);// aux is neither selected side
        }
        y = dc_[c].process(y);                       // нет постоянной составляющей на выходе
        y = real_detail::softClip(y, real_detail::kOutCeiling);
        return y;
    }

    float envelopeValue() const noexcept { return envLevel_; }
    bool  bypassed() const noexcept { return bypass_; }
    // Диагностика (тесты/UI): реальные коэффициенты текущих табличных стадий.
    float hpCoef() const noexcept { return hp1_[0].coef(); }
    float lpCoef() const noexcept { return lp2_[0].coef(); }
    bool  resonanceActive() const noexcept { return resHpActive_ || resLpActive_; }

private:
    // DC-блокер 8 Гц: y = x - x1 + R*y1
    struct DcBlocker {
        void reset() noexcept { x1_ = 0.0f; y1_ = 0.0f; }
        void setSampleRate(double fs) noexcept {
            const float rate = (fs > 0.0) ? static_cast<float>(fs) : static_cast<float>(kDspRate);
            r_ = std::exp(-2.0f * 3.14159265358979323846f * real_detail::kDcBlockHz / rate);
        }
        inline float process(float x) noexcept { const float y = x - x1_ + r_ * y1_; x1_ = x; y1_ = y; return y; }
        float x1_ = 0.0f, y1_ = 0.0f, r_ = 0.9988f;
    };

    void envStep() noexcept {
        const int a = static_cast<int>(real_detail::clampf(atkParam_, 0.0f, 127.0f));
        const int d = static_cast<int>(real_detail::clampf(decParam_, 0.0f, 127.0f));
        if (!released_) {
            if (envLevel_ < 1.0f) {
                envLevel_ += kEnvAttackRate[static_cast<size_t>(a)];
                if (envLevel_ > 1.0f) envLevel_ = 1.0f;
            } else {
                envLevel_ *= kEnvDecayRate[static_cast<size_t>(d)];
            }
        } else {
            envLevel_ *= kEnvDecayRate[static_cast<size_t>(d)];
            if (envLevel_ < 1.0e-4f) { envLevel_ = 0.0f; envActive_ = false; }
        }
        apply();
    }

    static int nearestNativeTone(float targetHz, double host) noexcept {
        int nearest = 0;
        float best = std::numeric_limits<float>::max();
        const float safe = real_detail::clampf(targetHz, 10.0f, static_cast<float>(host) * 0.45f);
        for (int tone = 0; tone < 127; ++tone) {
            const float at = real_detail::knobToCutoffHz(static_cast<float>(tone), host);
            const float distance = std::abs(std::log(std::max(at, 1.0f) / std::max(safe, 1.0f)));
            if (distance < best) { best = distance; nearest = tone; }
        }
        return nearest;
    }

    void apply() noexcept {
        // BASE' = BASE + env*BOFS plus the explicit, opt-in FIL ENV BASE
        // depth. WIDTH follows the same firmware BASE/WDTH topology.
        const float baseMod  = real_detail::clampf(baseParam_ + bofsParam_ * envLevel_ + externalBaseOffset_, 0.0f, 127.0f);
        const float widthMod = real_detail::clampf(widthParam_ + wofsParam_ * envLevel_ + externalWidthOffset_, 0.0f, 127.0f);
        const float lpKnob   = real_detail::clampf(baseMod + widthMod, 0.0f, 127.0f);

        // The existing physical table value remains bit-for-bit selected when
        // VEL/KT are zero. With an explicit VEL/KT value, select the closest
        // retained firmware-table cutoff to the requested transposed Hz.
        float fcHp = real_detail::knobToCutoffHz(baseMod, host_);
        float fcLp = real_detail::knobToCutoffHz(lpKnob, host_);
        const bool lowerTransposed = std::abs(externalLowerSemitones_) > 1.0e-6f;
        const bool upperTransposed = std::abs(externalUpperSemitones_) > 1.0e-6f;
        if (lowerTransposed) fcHp = real_detail::clampf(fcHp * std::exp2(externalLowerSemitones_ / 12.0f), 10.0f, static_cast<float>(host_) * 0.45f);
        if (upperTransposed) fcLp = real_detail::clampf(fcLp * std::exp2(externalUpperSemitones_ / 12.0f), 10.0f, static_cast<float>(host_) * 0.45f);
        for (int c = 0; c < 2; ++c) {
            hp1_[c].setCutoffFromTable(lowerTransposed ? nearestNativeTone(fcHp, host_) : static_cast<int>(baseMod + 0.5f));
            lp2_[c].setCutoffFromTable(upperTransposed ? nearestNativeTone(fcLp, host_) : static_cast<int>(lpKnob + 0.5f));
            aux_[c].setCutoffFromTableCurve2(auxKnob_);
        }
        auxActive_ = (auxKnob_ < 127);

        // ---- Resonance stages plus direct K35 / ladder / R-classic side
        // mappings. Every hybrid choice uses the same BASE / BASE+WDTH cutoff
        // law; only its named response changes.
        resHpActive_ = (real_detail::clampf(hpqParam_, 0.0f, 127.0f) > real_detail::kQKnobActive);
        resLpActive_ = (real_detail::clampf(lpqParam_, 0.0f, 127.0f) > real_detail::kQKnobActive);
        if (resHpActive_ || resLpActive_) {
            const float qHp = real_detail::knobToQ(hpqParam_);
            const float qLp = real_detail::knobToQ(lpqParam_);
            for (int c = 0; c < 2; ++c) {
                if (resHpActive_) svfHp_[c].setParams(fcHp, qHp, host_);
                if (resLpActive_) svfLp_[c].setParams(fcLp, qLp, host_);
            }
        }
        if (hybridLowerMode_ != 0) {
            if (nova::hybrid_private::hybridUsesKorg35(hybridLowerMode_))
                hybridK35Lower_.configure(hybridLowerMode_, fcHp, hpqParam_ / 127.0f);
            else if (nova::hybrid_private::hybridUsesOdinLadder(hybridLowerMode_))
                hybridLadderLower_.configure(hybridLowerMode_, fcHp, hpqParam_ / 127.0f);
            else
                hybridReferenceLower_.configure(hybridLowerMode_, fcHp, hpqParam_ / 127.0f);
        }
        if (hybridUpperMode_ != 0) {
            if (nova::hybrid_private::hybridUsesKorg35(hybridUpperMode_))
                hybridK35Upper_.configure(hybridUpperMode_, fcLp, lpqParam_ / 127.0f);
            else if (nova::hybrid_private::hybridUsesOdinLadder(hybridUpperMode_))
                hybridLadderUpper_.configure(hybridUpperMode_, fcLp, lpqParam_ / 127.0f);
            else
                hybridReferenceUpper_.configure(hybridUpperMode_, fcLp, lpqParam_ / 127.0f);
        }

        // THRU при дефолте: BASE=0, WIDTH=127, Q=0, BOFS/WOFS=0, огибающая в нуле.
        // An explicitly selected hybrid side is intentionally audible even here.
        bypass_ = (hybridLowerMode_ == 0 && hybridUpperMode_ == 0 &&
                   baseParam_ <= 0.5f && widthParam_ >= 126.5f &&
                   hpqParam_ <= 0.5f && lpqParam_ <= 0.5f &&
                   std::abs(bofsParam_) < 0.5f && std::abs(wofsParam_) < 0.5f &&
                   std::abs(externalBaseOffset_) < 1.0e-6f && std::abs(externalWidthOffset_) < 1.0e-6f &&
                   std::abs(externalLowerSemitones_) < 1.0e-6f && std::abs(externalUpperSemitones_) < 1.0e-6f &&
                   envLevel_ < 0.001f);
    }

    double host_ = kDspRate;
    bool sampleRateConfigured_ = false;
    float baseParam_ = 0.0f,  widthParam_ = 127.0f;
    float hpqParam_ = 0.0f,   lpqParam_ = 0.0f;
    float atkParam_ = 0.0f,   decParam_ = 64.0f;
    float bofsParam_ = 0.0f,  wofsParam_ = 0.0f;
    // VEL/KT derive semitone offsets per physical side. FIL ENV supplies
    // BASE/WDTH offsets and is neutral at zero.
    float externalLowerSemitones_ = 0.0f, externalUpperSemitones_ = 0.0f;
    float externalBaseOffset_ = 0.0f, externalWidthOffset_ = 0.0f;
    int   auxKnob_ = 127;
    float envLevel_ = 0.0f;
    bool  envActive_ = false, released_ = false, bypass_ = false, auxActive_ = false;
    bool  resHpActive_ = false, resLpActive_ = false;
    int   blockCounter_ = 0;
    int   hybridLowerMode_ = 0, hybridUpperMode_ = 0;
    nova::hybrid_private::Korg35Stereo hybridK35Lower_, hybridK35Upper_;
    nova::hybrid_private::LadderStereo hybridLadderLower_, hybridLadderUpper_;
    nova::hybrid_private::ReferenceClassicStereo hybridReferenceLower_, hybridReferenceUpper_;

    // По экземпляру на канал: стерео не делит состояние между L и R
    // (в прежнем FilterCore состояние было общее -- каналы протекали друг в друга).
    ::mnm::MnmOnePole hp1_[2], lp2_[2], aux_[2];   // реальные табличные стадии
    MnmResoSVF        svfHp_[2], svfLp_[2];        // устойчивые резонансные стадии
    DcBlocker         dc_[2];
};

// Режим "mnm" (dspModeMnm, индекс 0) -- это реальный фильтр. NovaDSP.h использует
// имя FilterCore, поэтому оно указывает сюда.
using FilterCore = RealFilterCore;

} // namespace mnm
} // namespace monomachine
