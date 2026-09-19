// ============================================================================
// MnmRouting.h — РОУТИНГ Monomachine, восстановленный из kernel (dsp1_pmem)
// Часть референс-пака mnmdsp. Это САМОЕ важное для модификаций: как звук
// попадает в FX-машину, откуда «сухой», почему MIX работает одинаково везде.
//
// ДОКАЗАТЕЛЬНАЯ БАЗА (адреса = dsp1_kernel_P0000-0B4D.txt):
//  * Буферы: X:$2C0..$2C3 = базы { $160, $1C0, $220, $280 } (таблица
//    X_0002c0_audio_buffer_bases). Буферы 0/1/2 = выходы треков текущего
//    прохода (3 голоса), буфер 3 = внешний вход (codec ADC).
//  * Выбор входа машины (P:$0113-$0131), флаги X:$2C4 (пишет ColdFire):
//      bit4  set -> r0 = буфер 3 (EXT);      бит 3 -> b=$40 (служебный)
//      bit12 set -> r0 = буфер 0             (сосед N-2)
//      bit13 set -> r0 = буфер 1             (сосед N-1)
//      иначе     -> r0 = буфер 2
//  * Заливка EXT (P:$02ED-$031D): bit6/bit7 -> копия ADC-блока (X:$100/$120,
//    пинг-понг по x:<<$ffffee) в буфер 3.
//  * Запись выхода голоса (P:$0143-$0218): целевой буфер = биты 0/1/2;
//      бит 12/13/14 SET    -> ПЕРЕЗАПИСЬ буфера (func_0001ec, копия)
//                    CLEAR -> ДОБАВЛЕНИЕ  (func_0001e2, аккумуляция)
//  * Сумма в основной микс (func_0001A3, P:$01AD-$01C1): buf0+buf1+buf2 -> buf0.
//
// СЛЕДСТВИЕ — «соседний трек» (Neighbor Track):
//  FX-машина на треке N с входом = буфер соседа K и ПЕРЕЗАПИСЬЮ того же
//  буфера K работает как INSERT в пути соседа: её выход ЗАМЕНЯЕТ сухой сигнал
//  соседа в основном миксе. MIX машины становится честным кроссфейдом:
//      out = ($7FFFFF - MIX)*dry + MIX*wet      (15_FX-CHORUS P:$1477CB-$1477DD)
//  MIX=127 -> сухой внутри FX почти/полностью исчезает (см. docs/ROUTING_RU.md
//  про 0.78% и скейлинг ColdFire). Никакого «особого режима соседа» в коде
//  машин НЕТ — машина не знает, откуда пришёл вход.
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>
#include "MnmFixed.h"

namespace mnmdsp {

static constexpr int kBlock      = 16;  // 16 кадров стерео на блок @44.1кГц
static constexpr int kBlockWords = 32;  // 16*2 слова (L,R интерлив)
static constexpr int kBuffers    = 3;   // buf0..buf2 (4-й = EXT, приходит снаружи)

// Один «голос» = один трек текущего прохода. Флаги = слово X:$2C4 для голоса.
struct RouteFlags {
    bool inputExt   = false;  // bit4 : вход = внешний вход (буфер 3)
    bool inBuf0     = false;  // bit12: вход = буфер 0
    bool inBuf1     = false;  // bit13: вход = буфер 1 (иначе буфер 2)
    bool outBuf0    = false;  // bit0 : выход в буфер 0
    bool outBuf1    = false;  // bit1 : выход в буфер 1
    bool outBuf2    = false;  // bit2 : выход в буфер 2 (иначе никуда)
    bool overBuf0   = false;  // bit12: перезапись buf0 (иначе добавление)
    bool overBuf1   = false;  // bit13: перезапись buf1
    bool overBuf2   = false;  // bit14: перезапись buf2
    bool fillExt    = false;  // bit6/7: залить ADC в буфер 3 (делает хост)
};

// Шина из 3 буферов + EXT. Точная модель прохода kernel (3 голоса/проход).
class TrackBus {
public:
    // Вызывается хостом на каждый аудио-блок, НАКАНУНЕ прохода голосов:
    // залить внешний вход (эквивалент P:$02ED-$031D).
    void fillExtInput(const float* extL, const float* extR, int n) {
        for (int i = 0; i < n; ++i) {
            ext[i*2+0] = toWord(extL[i]);
            ext[i*2+1] = toWord(extR[i]);
        }
    }

    const int32_t* inputBuffer(const RouteFlags& f) const {
        if (f.inputExt) return ext;
        if (f.inBuf0)   return buf[0];
        if (f.inBuf1)   return buf[1];
        return buf[2];
    }

    // После машины+post-цепочки: accumulate выход (Y:$00-$1F) в буферы.
    // Эквивалент P:$0143-$0218 (func_0001e2 add / func_0001ec overwrite).
    void accumulate(const RouteFlags& f, const int32_t* out) {
        for (int k = 0; k < kBuffers; ++k) {
            if (!(f.outBuf0 && k==0) && !(f.outBuf1 && k==1) && !(f.outBuf2 && k==2)) continue;
            bool over = (k==0 && f.overBuf0) || (k==1 && f.overBuf1) || (k==2 && f.overBuf2);
            if (over) std::memcpy(buf[k], out, sizeof(int32_t)*kBlockWords);
            else for (int i = 0; i < kBlockWords; ++i)
                     buf[k][i] = sat24((int64_t)buf[k][i] + out[i]);
        }
    }

    // func_0001A3: сумма буферов -> основной микс (buf0 перезаписывается суммой).
    void sumToMain(float* outL, float* outR, int n) {
        for (int i = 0; i < n; ++i) {
            int64_t l = (int64_t)buf[0][i*2]   + buf[1][i*2]   + buf[2][i*2];
            int64_t r = (int64_t)buf[0][i*2+1] + buf[1][i*2+1] + buf[2][i*2+1];
            outL[i] = toFloat(sat24(l));
            outR[i] = toFloat(sat24(r));
        }
    }

    void clearAll() { for (auto& b : buf) std::memset(b, 0, sizeof b); std::memset(ext, 0, sizeof ext); }
    int32_t* raw(int k) { return buf[k]; }

private:
    int32_t buf[kBuffers][kBlockWords] {};
    int32_t ext[kBlockWords] {};

    static int32_t toWord(float v)  { return sat24((int64_t)(v * (float)kOne)); }
    static float   toFloat(int32_t w) { return (float)w / (float)kOne; }
};

// === ВЫХОДНАЯ СТУПЕНЬ FX-МАШИН (эталон из FX-CHORUS P:$1477CB-$1477DD) ======
// out = ($7FFFFF - MIX)*dry + MIX*wet ; MIX = параметр p3 в слове V+$07.
// dry = копия входа (с гейном INP), wet = обработанный сигнал.
// ТА ЖЕ формула у REVERB/DYNAMIX/PHASER/FLANGER/RINGMOD (MIX = p3 всюду,
// кроме THRU/BBOX — у тех MIX нет).
static inline void fxMixStage(const int32_t* dry, const int32_t* wet,
                              int32_t mixWord, int32_t* out, int words) {
    const int32_t dryGain = sat24((int64_t)0x7FFFFF - mixWord);
    for (int i = 0; i < words; ++i)
        out[i] = sat24((int64_t)mpy(dryGain, dry[i]) + (int64_t)mpy(mixWord, wet[i]));
}

// === ВХОДНОЙ ГЕЙН FX-МАШИН (P:$14768B-$147698, то же в THRU $14764B) =======
// ROM: x0 = INP; a = INP*INP; x0 = a; a = input*x0; asl #2,a  ->  out = in*4*INP^2
// Гейн-СЛОВО = INP^2 (Q1.23), множитель x4 применяется к произведению в
// 56-бит аккумуляторе ДО сатурации записи.
//   INP=0.5 (ручка 64) -> слово 0.25, эф. гейн 1.0 (0 дБ)
//   INP=1.0 (ручка 127)-> слово 1.0,  эф. гейн 4.0 (+12 дБ)  <- КВАДРАТИЧНЫЙ закон
static inline int32_t inpGainWord(int32_t inpWord) {
    return mpy(inpWord, inpWord);                       // слово = INP^2
}
static inline float inpGainLinear(int32_t inpWord) {   // эф. гейн для float-путей
    double p = (double)inpWord / (double)kOne;
    return (float)(4.0 * p * p);
}

} // namespace mnmdsp
