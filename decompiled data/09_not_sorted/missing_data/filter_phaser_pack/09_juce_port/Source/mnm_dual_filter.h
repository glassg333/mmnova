// ============================================================================
// mnm_dual_filter.h — dual-фильтр трека Monomachine, портируемый в JUCE.
//
// Источник: машинный код DSP56300 OS 1.32B, ядро P:$04A8–$0B49 +
// func_000340/func_000397. Все таблицы — в mnm_tables.h (образ прошивки).
//
// Структура (доказано на бит-точном эмуляторе, см. filter_phaser_pack):
//   кадр = 16 сэмплов (родная частота машины 44100 Гц, кадр = 0.3629 мс)
//
//   [1] КОЛЬЦО КОЭФФИЦИЕНТОВ (P:$0537–$05A1) — бит-в-бит:
//       idx  = (baseWord * 1199) >> 23          (0..1200)
//       widx = (wdthWord * 128)  >> 23          (0..127)
//       eps  = Div24(1.0 / avg) * (div2 - div1) * 64   (|eps| < 3e-5)
//       ring[0] = -coeff2[idx] * (0.5 + eps)    // срез (главный коэфф.)
//       ring[1] = eps
//       ring[2] = width1[widx]                  // +дельта ширины
//       ring[3] = width2[widx]                  // -дельта ширины
//
//   [2] КАСКАД 1 — гибрид FIR+SVF (func_000340, доказан бит-в-бит, iter.14):
//       16 итераций на кадр на канал; кольцо обходится по модулю 4;
//       состояние = окно истории (живая обратная связь, персистентно).
//       Выход = шина a_old (доказано: probe_audio_path2.py).
//
//   [3] РЕЗОНАНС — гребёнка 8 дробных тапов (func_000397, доказан iter.14):
//       тап:  out = x_mid + frac*(x_mid - x_prev)      (+ округление macr)
//       HPQ -> демпфирование hpqDamp[HPQ], фидбек hpqFb[HPQ]
//       LPQ -> фидбек lpqFb[LPQ]
//       разбег тапов: HPQ=0 -> шаг 1.0 сэмпла; HPQ=127 -> шаг 15.8828 сэмпла
//       (крайние точки прогрессии измерены бит-точно; промежуток — [РЕКОНСТР.])
//
//   [4] ЭНВЕЛОПЫ У ФИЛЬТЕРА НЕТ (доказано, 08_env_verdict/, iter.15).
//       Движение среза на реальной машине делает CPU (LFO/мод-матрица),
//       записывая слова BASE/WDTH/HPQ/LPQ между кадрами. В плагине это
//       делает хост/мод-матрица — просто вызывайте setParams() чаще.
//
// Точность: float, структура и таблицы 1:1; кольцо совпадает с бит-точным
// прогоном в пределах ±2 LSB (~-130 дБ). Помечено [РЕКОНСТР.] всё, что не
// доказано бит-в-бит.
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <array>
#include <algorithm>

#include "mnm_tables.h"

namespace mnm {

static constexpr int   kFrame = 16;                 // сэмплов на кадр (как в железе)
static constexpr float kInv23 = 1.0f / 8388608.0f;  // Q1.23 -> float

// --- целочисленные помощники точной реплики --------------------------------
inline int32_t s24 (uint32_t w) noexcept
{
    w &= 0xFFFFFFu;
    return (w & 0x800000u) ? (int32_t) (w - 0x1000000u) : (int32_t) w;
}
inline int64_t sext56 (int64_t v) noexcept
{
    v &= ((int64_t) 1 << 56) - 1;
    return (v & ((int64_t) 1 << 55)) ? v - ((int64_t) 1 << 56) : v;
}

// ---------------------------------------------------------------------------
// Параметры = СЛОВА страницы голоса (ровно то, что CPU пишет в Y[P+$10..$13]).
// Конвенция рукояток: knob 0..127 -> word = knob<<16 (Q23 = knob/128); она
// использована во всех бит-точных прогонах пака (BASE=64 -> idx=599, средний
// срез, как на оригинале).
// ---------------------------------------------------------------------------
struct FilterParams
{
    uint32_t baseWord = 64u << 16;   // FILT BASE  (Y[P+$10]) — срез
    uint32_t wdthWord = 0u << 16;    // FILT WDTH  (Y[P+$11]) — ширина пары
    uint32_t hpqWord  = 0u << 16;    // FILT HPQ   (Y[P+$12]) — резонанс HP
    uint32_t lpqWord  = 0u << 16;    // FILT LPQ   (Y[P+$13]) — резонанс LP
};

inline uint32_t wordFromKnob (int knob) noexcept      // 0..127 -> Q23 word
{
    return (uint32_t) (knob & 0xFFFF) << 16;
}

// ---------------------------------------------------------------------------
// Кольцо коэффициентов. eps-путь — точная целочисленная репликация
// P:$057D..$059A (DIV x24), верифицирована 9/9 конфигураций бит-в-бит
// (scripts/probe_eps.py: Y:$05 совпал во всех точках).
// ---------------------------------------------------------------------------
struct CoeffRing { float c[4]; };

inline CoeffRing computeRing (uint32_t baseWord, uint32_t wdthWord) noexcept
{
    const int idx  = (int) std::min<uint64_t> (((uint64_t) baseWord * 1199) >> 23, 1200);
    const int widx = (int) std::min<uint64_t> (((uint64_t) wdthWord * 128)  >> 23, 127);

    // avg = (div1+div2)>>1, слово A1 аккумулятора (СЫРЫЕ СЛОВА — не флоаты!)
    const int32_t avgWord =
        ((kDiv1W[(size_t) idx] + kDiv2W[(size_t) widx]) >> 1) & 0xFFFFFF;

    // a = 1.0 (A1 = 1); 24 шага DIV x1,a (невосстанавливающее деление DSP56300)
    const auto sext24 = [] (uint32_t v) noexcept -> int32_t
    {
        v &= 0xFFFFFFu;
        return (v & 0x800000u) ? (int32_t) (v - 0x1000000u) : (int32_t) v;
    };
    uint64_t D = 1u << 24;                    // 48-битный акк. деления (dhi:dlo)
    for (int i = 0; i < 24; ++i)
    {
        uint32_t dhi = (D >> 24) & 0xFFFFFFu, dlo = D & 0xFFFFFFu;
        dhi = ((dhi << 1) | (dlo >> 23)) & 0xFFFFFFu;
        dlo = (dlo << 1) & 0xFFFFFFu;
        const int32_t ss = sext24 (avgWord), ds = sext24 (dhi);
        dhi = ((ds < 0) == (ss < 0)) ? (dhi - avgWord) & 0xFFFFFFu
                                     : (dhi + avgWord) & 0xFFFFFFu;
        const uint32_t qbit = ((sext24 (dhi) < 0) == (ss < 0)) ? 1u : 0u;
        dlo |= qbit;
        D = ((uint64_t) dhi << 24) | dlo;
    }
    const int32_t q0   = (int32_t) (D & 0xFFFFFFu);          // a0 = частное
    const int32_t dDif = kDiv2W[(size_t) widx] - kDiv1W[(size_t) idx];
    int64_t b = ((int64_t) s24 ((uint32_t) q0) * (int64_t) dDif) << 1;  // mpy
    b = sext56 (b << 6);                                     // asl #6
    const float eps = (float) s24 ((uint32_t) ((b >> 24) & 0xFFFFFFu)) * kInv23;

    CoeffRing r;
    const float c2 = kCoeff2[(size_t) idx];
    r.c[0] = -c2 * (0.5f + eps);      // Y:$04 (минус из `mpy -y0,x0,a`)
    r.c[1] = eps;                     // Y:$05
    r.c[2] = kWidth1[(size_t) widx];  // Y:$06
    r.c[3] = kWidth2[(size_t) widx];  // Y:$07
    return r;
}

// ===========================================================================
// DualFilter — КАСКАД 1 (точный порт func_000340 + кольцо коэффициентов).
// Кадр = 16 сэмплов, interleaved stereo. Выход = шина a_old (доказано).
// Окна истории персистентны — это живая обратная связь оригинала.
// ===========================================================================
class DualFilter
{
public:
    void reset() noexcept
    {
        for (auto& c : ch) { c.hx.fill (0.f); c.hy.fill (0.f); }
        for (float& v : ringSmooth) v = 0.f;
        ringInit = false;
    }

    // in/out: interleaved stereo, kFrame сэмплов (in[2*i]=L, in[2*i+1]=R).
    void processFrame (const float* in, float* out, const FilterParams& p) noexcept
    {
        // --- [1] кольцо коэффициентов ---
        const CoeffRing target = computeRing (p.baseWord, p.wdthWord);
        if (! ringInit)
        {
            for (int i = 0; i < 4; ++i) ringSmooth[i] = target.c[i];
            ringInit = true;
        }
        else
        {
            // CPU оригинала пишет слова раз в кадр; лёгкое сглаживание кольца
            // убирает ступеньки при быстрых модуляциях (безопасно: квантование
            // оригинала грубее этого сглаживания).
            for (int i = 0; i < 4; ++i)
                ringSmooth[i] += 0.25f * (target.c[i] - ringSmooth[i]);
        }
        const float* ring = ringSmooth;

        // --- [2] func_000340 на каждый канал ---
        for (int c = 0; c < 2; ++c)
        {
            Chan& s = ch[c];
            // 16 свежих сэмплов в окно + 4-сэмпловый хвост (аналог записи
            // машины в X:$97..$AC стадией $04F5 — регион шире кадра)
            for (int i = 0; i < kFrame + 4; ++i)
                s.hx[(kIn0 + i) & kMask] = in[2 * std::min (i, kFrame - 1) + c];

            cascade1 (s, ring);

            // выход = шина a_old: iter_i пишет aOld_i в hx[5+i] (=$95-$90)
            for (int i = 0; i < kFrame; ++i)
                out[2 * i + c] = s.hx[(kAOut0 + i) & kMask];
        }
    }

    const float* lastRing() const noexcept { return ringSmooth; }

private:
    // Окно = аналог X/Y:$90..$CF (L) / $D0..$10F (R); персистентно.
    static constexpr int kWin   = 64;
    static constexpr int kMask  = kWin - 1;
    static constexpr int kIn0   = 7;    // вход: X:$97-$90
    static constexpr int kAOut0 = 5;    // шина a_old: iter_i -> hx[5+i] (X:$95+)

    struct Chan
    {
        std::array<float, kWin> hx{};  // X-сторона (вход + состояния a_old)
        std::array<float, kWin> hy{};  // Y-сторона (состояния b_old)
    };
    Chan ch[2];
    float ringSmooth[4] {};
    bool  ringInit = false;

    // 24-битный wrap записи в память DSP (извлечение A1 заворачивает в ±1.0) —
    // это НЕ ограничение, а родное поведение железа, стабилизирующее ОС.
    static float wrap1 (float v) noexcept
    {
        return v - 2.f * std::round (v * 0.5f);
    }

    // Точная прогулка func_000340 — перевод svf_fit340.py 1:1 (float).
    // Номера инструкций в комментариях = PC оригинала.
    void cascade1 (Chan& s, const float* ring) noexcept
    {
        int r0 = 6;                       // $96-$90 (r0 = $96 у вызывающего)
        int r1 = 3;                       // $93-$90 (r1 = $93)
        int r4 = 0;                       // кольцо ring, mod 4 (M4 = $000003)
        // Аккумулятор B НЕ персистентен между кадрами: первый же mpy на $034B
        // сбрасывает его (вход b функции неважен — проверено по листингу).
        float b = 0.f;

        float x0 = s.hx[r0]; r0 = (r0 + 1) & kMask;               // 0340
        float a  = x0;                                            // 0342 tfr x0,a
        x0 = s.hx[r0]; r0 = (r0 + 1) & kMask;                     // 0342 par x:(r0)+,x0
        float y0 = ring[r4 & 3]; ++r4;                            // 0342 par y:(r4)+,y0

        for (int it = 0; it < 16; ++it)
        {
            a += y0 * x0;                                         // 0345 mac y0,x0,a
            const float x1 = s.hx[r0]; r0 = (r0 - 1) & kMask;     // 0345 par x:(r0)-,x1
            const float y1 = ring[r4 & 3]; ++r4;                  // 0345 par y:(r4)+,y1
            a += y0 * x0;                                         // 0346 mac y0,x0,a
            a += y1 * x1;                                         // 0347 mac y1,x1,a
            float xs = s.hx[r1]; r1 = (r1 + 1) & kMask;           // 0347 par x:(r1)+,x0
            a -= xs * y1;                                         // 0348 mac -x0,y1,a
            xs = s.hx[r1]; r1 = (r1 - 1) & kMask;                 // 0348 par x:(r1)-,x0
            a -= y0 * xs;                                         // 0349 mac -y0,x0,a
            s.hy[r1] = wrap1 (b); r1 = (r1 + 2) & kMask;          // 0349 par b,y:(r1)+n1 (wrap A1)
            a -= y0 * xs;                                         // 034A mac -y0,x0,a
            x0 = s.hx[r0]; r0 = (r0 + 1) & kMask;                 // 034A par x:(r0)+,x0
            y0 = ring[r4 & 3]; ++r4;                              // 034A par y:(r4)+,y0
            b = x1 * y0;                                          // 034B mpy x1,y0,b (сброс B)
            const float y1b = ring[r4 & 3]; ++r4;                 // 034B par y:(r4)+,y1
            const float aOld = wrap1 (a);                         // 034C (a1-экстракция)
            a = x0;                                               // 034C tfr x0,a
            s.hx[r1] = aOld; r1 = (r1 - 1) & kMask;               // 034C par a,x:(r1)-
            y0 = aOld;                                            // 034C par a,y0
            b += y1b * y0;                                        // 034D mac y1,y0,b
            x0 = s.hx[r0]; r0 = (r0 + 1) & kMask;                 // 034D par x:(r0)+,x0
            y0 = ring[r4 & 3]; ++r4;                              // 034D par y:(r4)+,y0
            b *= 4.f;                                             // 034E asl #2,b
        }
        // s.b не сохраняем: B сбрасывается каждым mpy $034B
    }
};

// ===========================================================================
// MonoMachineDualFilter — ГОТОВЫЙ звуковой узел для плагина:
// каскад 1 (точный) + EQ-байпас (Ф3 открыта: EQ-таблицы не разделены,
// на звук dual-фильтра не влияют) + резонатор (гребёнка 8 дробных тапов).
// ===========================================================================
class MonoMachineDualFilter
{
public:
    void prepare (double /*sampleRate*/) noexcept { reset(); }

    void reset() noexcept
    {
        f.reset();
        for (int c = 0; c < 2; ++c)
        {
            hist[c].fill (0.f);
            histPos[c] = 0;
            tapPos[c] = 0.f;
            fbState[c] = 0.f;
        }
        tapStep = 1.f;
    }

    void setParams (const FilterParams& p) noexcept { params = p; }
    FilterParams& paramsRef() noexcept { return params; }

    // in/out: interleaved stereo, ровно kFrame (=16) сэмплов.
    void processFrame (const float* in, float* out) noexcept
    {
        // --- каскад 1 (точный порт) ---
        f.processFrame (in, tmpOut, params);

        // --- резонансные параметры (индексация доказана: HPQ/LPQ напрямую) ---
        const int hpq = (int) std::min<uint64_t> (((uint64_t) params.hpqWord * 128) >> 23, 127);
        const int lpq = (int) std::min<uint64_t> (((uint64_t) params.lpqWord * 128) >> 23, 127);
        const float damp = kHpqDamp[(size_t) hpq];
        const float fbHP = kHpqFb  [(size_t) hpq];
        const float fbLP = kLpqFb  [(size_t) lpq];

        // Разбег тапов (крайние точки бит-точны, промежуток — линейная
        // калибровка [РЕКОНСТР.]):
        //   HPQ=0   -> шаг 1.0     (позиции 0,1,2,...)
        //   HPQ=127 -> шаг 15.8828 (позиции F,1F,2F,3E,...,10D)
        const float stepTarget = 1.f + 14.8828f * ((float) hpq / 127.f);
        tapStep += 0.25f * (stepTarget - tapStep);

        for (int c = 0; c < 2; ++c)
        {
            // выход каскада-1 в историю резонатора (кольцо 1024 = аналог
            // адресного кольца DSP)
            for (int i = 0; i < kFrame; ++i)
                push (c, tmpOut[2 * i + c]);

            for (int i = 0; i < kFrame; ++i)
            {
                const float x = tmpOut[2 * i + c];

                // 8 дробных тапов: out = x_mid + frac*(x_mid - x_prev)
                // (точная формула func_000397; здесь n0 = 1 сэмпл [РЕКОНСТР.])
                float res = 0.f;
                float pos = tapPos[c];
                for (int t = 0; t < 8; ++t)
                {
                    const float pf = std::floor (pos);
                    const float frac = pos - pf;
                    const int   back = (int) pf;
                    const float xPrev = peek (c, back);
                    const float xMid  = peek (c, back + 1);
                    res += xMid + frac * (xMid - xPrev);
                    pos += tapStep;
                }
                tapPos[c] = std::fmod (pos, (float) kHist);
                res *= 0.125f;   // среднее тапов

                // демпфированная обратная связь (грейны hpqFb/lpqFb доказаны)
                const float fb = fbState[c] * damp + res * 0.5f * (fbHP + fbLP);
                fbState[c] = fb;

                const float y = x + fb;
                push (c, y);
                out[2 * i + c] = y;
            }
        }
    }

private:
    static constexpr int kHist = 1024;
    DualFilter f;
    std::array<float, kHist> hist[2] {};
    int   histPos[2] {};
    float tapPos[2] {};
    float tapStep = 1.f;
    float fbState[2] {};
    float tmpOut[kFrame * 2] {};
    FilterParams params;

    void push (int c, float v) noexcept
    {
        hist[c][(size_t) histPos[c]] = v;
        histPos[c] = (histPos[c] + 1) % kHist;
    }
    float peek (int c, int back) const noexcept   // back сэмплов назад
    {
        const int idx = histPos[c] - 1 - back;
        int m = idx % kHist; if (m < 0) m += kHist;
        return hist[c][(size_t) m];
    }
};

} // namespace mnm
