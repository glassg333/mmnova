// ============================================================================
// MnmBBox.h — DPRO-BBOX (машина m07), воссоздан по листингу прошивки.
// Источник: 03_listings/machines_page_A/07_DPRO-BBOX_full.txt
//           (init $1474E9, config $1474ED, proc $14752B..$147631)
// Данные:   data/bbox_drum_bank.bin  (12 моделей, P:$103E7B..$11320F, 24-bit BE)
//           data/pitch_wavetable_2048.bin (X:$140000, wt[i]=0.5*2^(i/2048))
//
// ЧТО ТАКОЕ BBOX НА САМОМ ДЕЛЕ (НЕ сэмплер с пользовательскими слотами):
//  * 12 барабанных моделей, зашитых в прошивку; модель выбирается НОТОЙ
//    (config: индекс = f(V+$01) -> таблица {start,end} в P:$147632).
//  * Каждая нота = новый хит: фаза обнуляется, гейт открывается, звук идёт
//    до КОНЦА модели (жёсткий срез). Общая AHD/ADSR огибающая накладывается
//    post-цепочкой трека (func_0004A8), не машиной.
//  * Рендер: фазовая «лестница» 64 шага на блок 16 кадров (4x передискрет),
//    линейная интерполяция по банку, затем полифазный FIR-дециматор 4:1 —
//    его коэффициенты зашиты В КОДЕ машины (P:$1475D5-$1475F2) и дают
//    характерный lowpass-окрашенный тон BBOX.
//  * Ручки (param_i -> V+$04+i, '---' = не используется):
//      p0 PTCH — транспонирование (ROM: +PTCH*$2000 в питч-аккумулятор)
//      p1 STRT — старт внутри модели: startPos = start + len*STRT/2,
//                конец НЕ двигается (регион игры сжимается)
//      p4 RTRG — число ретригов: счётчик = min(127,(RTRG+$8000)*$80>>23)
//      p5 RTIM — интервал ретригов: фаза $7FFFFF -= (RTIM*6+$C00)>>23 за
//                блок; по переходу через 0 — рестарт фазы, гейт заново,
//                счётчик--. Абсолютная калибровка зависит от скейлинга
//                ColdFire (см. docs/BBOX_RU.md, раздел «открытые вопросы»).
//  * Гейн выхода: asl #$7 перед записью y:(r7)+.
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>
#include <cstdio>
#include "../MnmFixed.h"

namespace mnmdsp {

struct BBoxModel { uint32_t start; uint32_t length; };

// Таблица моделей: сгенерировано из P:$147632 (dsp1_pmem), offsets от $103E7B
static constexpr BBoxModel kBBoxModels[12] = {
    {      0,   918 },  // abs P:$103E7B..$104211
    {   1977,  4951 },  // abs P:$104634..$10598B
    {  10961,  3008 },  // abs P:$10694C..$10750C
    {  16018,  1025 },  // abs P:$107D0D..$10810E
    {  18478,  1025 },  // abs P:$1086A9..$108AAA
    {  24504,  5001 },  // abs P:$109E33..$10B1BC
    {  37680,  1600 },  // abs P:$10D1AB..$10D7EB
    {  41330,  4019 },  // abs P:$10DFED..$10EFA0
    {  47398,  6002 },  // abs P:$10F7A1..$110F13
    {  54388,   715 },  // abs P:$1112EF..$1115BA
    {  56896,  1927 },  // abs P:$111CBB..$112442
    {  60873,  1483 },  // abs P:$112C44..$11320F
};

// Полифазные коэффициенты дециматора — дословно из кода машины (24-bit signed)
static constexpr int32_t kBBoxFIR[10] = {
    0x3CE85F, (int32_t)0xF9F156, (int32_t)0x939958, 0x04D33E, 0x36F789,
    0x11B451, (int32_t)0x9023EC, (int32_t)0xEE4BAF, (int32_t)0xC652AD, 0x0D1E2D
};

class MnmBBox {
public:
    void setBank(const int32_t* samples, int count) { m_bank = samples; m_bankLen = count; }
    void setPitchTable(const int32_t* tbl2048)      { m_pitch = tbl2048; }

    void setKnobs(uint8_t ptch, uint8_t strt, uint8_t rtrg, uint8_t rtim) {
        m_ptch = ptch; m_strt = strt; m_rtrg = rtrg; m_rtim = rtim;
    }

    // Конфиг машины на ноту ($1474ED-$147526)
    void noteOn(int note) {
        if (!m_bank || !m_pitch) return;
        m_note = note;
        // ROM: idx = f(V+$01): idx = ((V+$01)+$55)*3/64. Масштаб V+$01 задаёт
        // ColdFire; с сырым MIDI-номером прошивка даёт индексы 3..9 (ноты
        // 0..127 -> 3.98..9.94). Здесь: тот же закон, зажатый в 0..11.
        int idx = (int)(((int64_t)(note + 85) * 3) >> 6);
        if (idx < 0) idx = 0;
        if (idx > 11) idx = 11;
        m_model = idx;

        const BBoxModel& mdl = kBBoxModels[idx];
        const int64_t len    = mdl.length;
        const int64_t strtW  = ((int64_t)m_strt << 23) / 127;   // STRT -> Q1.23
        m_pos0 = mdl.start + (int32_t)((len * strtW) >> 24);    // start += len*STRT/2
        m_end  = mdl.start + mdl.length - 1;                    // конец на месте
        m_phase = 0.0;                                          // фаза с нуля
        m_gate  = 1.0;                                          // гейт открыт
        int64_t cnt = ((((int64_t)m_rtrg << 23) / 127) + 0x8000) * 0x80 >> 23;
        m_retrigCount = cnt > 127 ? 127 : (int)cnt;
        m_retrigPhase = 1.0;
        m_playing = true;
    }

    void allSoundOff() { m_playing = false; m_gate = 0.0; }

    // Обработка целыми блоками 16 кадров. n кратно 16.
    void processBlock(float* outL, float* outR, int n) {
        for (int fr = 0; fr < n; fr += kBlock)
            processOne(outL + fr, outR + fr);
    }

    bool playing() const { return m_playing; }
    int  model()   const { return m_model; }
    int  retrigsFired() const { return m_retrigsFired; }

private:
    void processOne(float* outL, float* outR) {
        if (!m_bank || !m_pitch) {
            for (int i = 0; i < kBlock; ++i) { outL[i] = outR[i] = 0.f; }
            return;
        }

        // --- фазовый инкремент ---
        // ROM ($14753B-$14755D): acc = (V+$01+$55)*2 + (V-$0A)*$B000
        //      + PTCH*$2000 + $D00 - $5800
        //      -> wt[acc&$7FF]>>10 <<(acc>>11) * $1D22A >>3, затем знак-трюк
        //      с $2F8E (48-бит) = инкремент фазы за шаг лестницы.
        // Масштабы V+$01/V-$0A известны только на стороне ColdFire, поэтому
        // акк собирается от MIDI-ноты (документированное упрощение):
        double semi  = (m_note - 60) + ((int)m_ptch - 64) * (24.0 / 64.0);
        int    acc   = (int)std::lround(semi * (2048.0 / 12.0)) + 0xD00;
        int32_t incW = pitchIncrement(m_pitch, acc);
        // Лестница = 64 шага на 16 кадров => воспроизведение на 4x частоте;
        // нативный темп модели = 1.0 слово банка за кадр => 0.25 слова/шаг:
        double inc = (double)incW / (double)kOne;
        if (inc < 0.0) inc = 0.0;

        // --- лестница фаз: 64 значения ($147582-$14758C) ---
        double ladder[64];
        double ph = m_phase;
        for (int i = 0; i < 64; ++i) { ladder[i] = ph; ph += inc; }
        m_phase = ph;

        // --- рендер + гейт ($14759D-$1475BE) ---
        int32_t render[64];
        for (int i = 0; i < 64; ++i) {
            double posF = ladder[i];
            int64_t pos = (int64_t)posF;
            if (m_pos0 + pos >= m_end) m_gate = 0.0;            // жёсткий срез
            int32_t s0 = bankRead(m_pos0 + pos);
            int32_t s1 = bankRead(m_pos0 + pos + 1);
            double frac = posF - (double)pos;
            double s = (s0 + (s1 - s0) * frac) * m_gate;
            render[i] = sat24((int64_t)(s * 128.0));            // asl #$7 ROM
        }

        // --- полифазный дециматор 4:1, 2 ветки ($1475C1-$147626) ---
        // ROM: 10 коэффициентов циклично (m5=$9 modulo 10), состояния между
        // блоками в V+$1E..$22. Здесь: банк A = c[0..4] (L), B = c[5..9] (R).
        int32_t out[2][kBlock];
        for (int i = 0; i < kBlock; ++i) {
            int64_t a = 0, b = 0;
            for (int t = 0; t < 5; ++t) {
                int32_t s = tapAt(i * 4 + t * 2, render);
                a += (int64_t)s * kBBoxFIR[t];
                b += (int64_t)s * kBBoxFIR[t + 5];
            }
            out[0][i] = sat24(a >> 16);
            out[1][i] = sat24(b >> 16);
        }
        std::memcpy(m_hist, render + 56, sizeof(int32_t) * 8);  // хвост окна

        for (int i = 0; i < kBlock; ++i) {
            outL[i] = (float)out[0][i] / (float)kOne;
            outR[i] = (float)out[1][i] / (float)kOne;
        }

        // --- ретриг ($147564-$147581) ---
        // ROM-константы: фаза $7FFFFF, декремент (RTIM*6+$C00) за блок, счётчик
        // RTRG. Точный шаг декремента зависит от скейлинга RTIM на ColdFire
        // (без него $7FFFFF/(RTIM*6+$C00) даёт 2730..0.17 блока — непрерывная
        // кривая 0.99 с -> audio-rate; она и реализована ниже).
        if (m_retrigCount > 0) {
            double rtW   = (double)m_rtim * kOne / 127.0;
            double denom = rtW * 6.0 + 0xC00;
            double interval = std::max(1.0, (double)0x800000 / denom); // блоков
            m_retrigPhase -= 1.0 / interval;                    // блок = 16 кадров
            if (m_retrigPhase < 0.0) {
                m_retrigPhase = 1.0;
                m_phase = 0.0;                                  // рестарт сэмпла
                m_gate  = 1.0;                                  // гейт заново
                --m_retrigCount;
                ++m_retrigsFired;
            }
        }
    }

    int32_t tapAt(int idx, const int32_t* render) const {
        if (idx >= 0 && idx < 64) return render[idx];
        if (idx < 0) { int h = idx + 8; return (h >= 0 && h < 8) ? m_hist[h] : 0; }
        return 0;
    }
    int32_t bankRead(int64_t pos) const {
        if (pos < 0) pos = 0;
        if (pos >= m_bankLen) pos = m_bankLen - 1;
        return m_bank[pos];
    }

    const int32_t* m_bank = nullptr;
    int            m_bankLen = 0;
    const int32_t* m_pitch = nullptr;

    uint8_t m_ptch = 64, m_strt = 0, m_rtrg = 0, m_rtim = 0;
    int     m_note = 60;
    int     m_model = 0;
    int64_t m_pos0 = 0, m_end = 0;
    double  m_phase = 0.0;
    double  m_gate = 0.0;
    int     m_retrigCount = 0;
    int     m_retrigsFired = 0;
    double  m_retrigPhase = 0.0;
    bool    m_playing = false;
    int32_t m_hist[8] {};
};

// Загрузчик data/bbox_drum_bank.bin (24-bit BE, 3 байта/слово) -> int32 Q1.23
inline bool loadBBoxBank(const char* path, std::vector<int32_t>& out) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    fseek(f, 0, SEEK_END);
    long bytes = ftell(f);
    fseek(f, 0, SEEK_SET);
    out.resize(bytes / 3);
    for (size_t i = 0; i < out.size(); ++i) {
        uint8_t b[3];
        if (fread(b, 1, 3, f) != 3) { fclose(f); return false; }
        int32_t v = (b[0] << 24) | (b[1] << 16) | (b[2] << 8);
        out[i] = v >> 8;                                        // знаковый сдвиг
    }
    fclose(f);
    return true;
}

// Вариант для JUCE BinaryData (данные уже в памяти)
inline bool loadBBoxBankFromMemory(const void* data, size_t bytes, std::vector<int32_t>& out) {
    if (!data || bytes < 3) return false;
    const uint8_t* p = static_cast<const uint8_t*>(data);
    out.resize(bytes / 3);
    for (size_t i = 0; i < out.size(); ++i) {
        int32_t v = (p[i*3] << 24) | (p[i*3+1] << 16) | (p[i*3+2] << 8);
        out[i] = v >> 8;
    }
    return true;
}

} // namespace mnmdsp
