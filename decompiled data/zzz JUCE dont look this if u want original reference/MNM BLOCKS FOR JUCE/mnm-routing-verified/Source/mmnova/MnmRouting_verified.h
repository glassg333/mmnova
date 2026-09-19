#pragma once
#include <cstdint>
#include <algorithm>

// ============================================================================
//  Роутинг Monomachine OS 1.32B. Все адреса проверены по dsp1_kernel.
//  Источник: 3 mnmdsp_reference/dsp/MnmRouting.h + docs/ROUTING_RU.md
//
//  Порядок тракта (сошлись три источника: слуховой тест, CHANGELOG
//  monomachine-lab, PAGE_TEMPLATES):
//     machine -> DIST -> [EQ] -> SRR -> FILTER -> AMP env -> DELAY
//  Спорно только положение EQ относительно фильтра.
// ============================================================================

namespace mmnova {

using s24 = int32_t;
using s56 = int64_t;
static constexpr s24 kOne24 = 0x7FFFFF;

inline s24 sat24 (s56 a) noexcept {
    if (a >  kOne24)   return kOne24;
    if (a < -0x800000) return -0x800000;
    return (s24) a;
}
inline s56 mpy (s24 x, s24 y) noexcept { return ((s56) x * (s56) y) << 1; }

static constexpr int kBlock      = 16;   // 16 кадров стерео на блок @44.1к
static constexpr int kBlockWords = 32;   // L,R интерлив
static constexpr int kBuffers    = 3;    // buf0..buf2, 4-й = EXT

// Базы буферов X:$2C0..$2C3 для DSP1 (из kernel P:$00AC-$00C0)
static constexpr uint32_t kBufBase[4] = { 0x160, 0x1C0, 0x220, 0x280 };

// ----------------------------------------------------------------------------
//  Флаги X:$2C4 — их пишет ColdFire, kernel только читает (P:$0113-$0131)
// ----------------------------------------------------------------------------
struct RouteFlags
{
    bool inputExt = false;   // bit4  : вход = буфер 3 (внешний вход)
    bool inBuf0   = false;   // bit12 : вход = буфер 0
    bool inBuf1   = false;   // bit13 : вход = буфер 1 (иначе буфер 2)

    bool outBuf0  = false;   // bit0  : выход в буфер 0
    bool outBuf1  = false;   // bit1  : выход в буфер 1
    bool outBuf2  = false;   // bit2  : выход в буфер 2

    // Запись выхода, P:$0143-$0218:
    //   бит установлен -> ПЕРЕЗАПИСЬ (func_0001ec) = INSERT в путь соседа
    //   бит сброшен    -> ДОБАВЛЕНИЕ (func_0001e2) = параллельно
    bool overBuf0 = false;   // bit12
    bool overBuf1 = false;   // bit13
    bool overBuf2 = false;   // bit14

    bool fillExt  = false;   // bit6/7 : залить ADC в буфер 3 (P:$02ED-$031D)
};

// ----------------------------------------------------------------------------
//  INP — гейн входа FX-машины.  P:$14768B-$147698
//
//  Паттерн в опкодах:
//     mpy x0,x0,a      ; слово = INP^2
//     mpy x1,x0,a      ; умножение на вход
//     asl #$2,a,a      ; x4 в 56-бит аккумуляторе ДО сатурации
//
//  ручка  32 -> -12 дБ
//  ручка  64 ->   0 дБ   (слово 0.25, эф. гейн 1.0)  <- дефолт всех FX
//  ручка 127 -> +12 дБ   (слово 1.00, эф. гейн 4.0)
//  Закон КВАДРАТИЧНЫЙ. Линейный INP ломает насыщение — не упрощать.
// ----------------------------------------------------------------------------
inline s24 inpGainWord (s24 inpWord) noexcept
{
    return sat24 (mpy (inpWord, inpWord) >> 24);      // слово = INP^2
}

inline s24 applyInpGain (s24 in, s24 inpWord) noexcept
{
    const s24 g = inpGainWord (inpWord);
    return sat24 ((s56) (mpy (in, g) >> 24) << 2);    // x4 до сатурации
}

// ----------------------------------------------------------------------------
//  MIX — выходная ступень ВСЕХ FX-машин.  P:$1477CB-$1477DD
//
//     x0 = MIX (слово V+$07)
//     a  = $7FFFFF - x0          ; вес сухого
//     out = a*dry + x0*wet
//
//  Одинаково у CHORUS / REVERB / DYNAMIX / PHASER / FLANGER / RINGMOD.
//  MIX везде = p3. Формула НЕ зависит от источника входа.
//
//  Про "0.78% сухого при MIX=127": вес сухого = $7FFFFF - MIX.
//  Сколько даёт ручка 127 — решает скейлинг ColdFire, который ещё не
//  расшифрован (кривые лежат в raw_tail дескрипторов):
//    если ручка -> 127*$10000 = $7F0000, остаток $FFFF/$800000 = 0.78% (-42 дБ)
//    если ручка -> полный $7FFFFF, остаток ровно 0
//  В любом случае -42 дБ неслышно. Реальный "сухой" рядом с эффектом — это
//  выход самого трека-источника при ПАРАЛЛЕЛЬНОМ включении (добавление
//  в буфер). Убирается роутингом/уровнем трека, а не правкой MIX.
// ----------------------------------------------------------------------------
inline s24 fxMixStage (s24 dry, s24 wet, s24 mixWord) noexcept
{
    const s24 dryGain = sat24 ((s56) kOne24 - mixWord);
    return sat24 ((mpy (dry, dryGain) + mpy (wet, mixWord)) >> 24);
}

// ----------------------------------------------------------------------------
//  Шина треков. Повторяет проход kernel: 3 голоса за проход.
//  Сумма в микс: func_0001A3, P:$01AD-$01C1  ->  buf0+buf1+buf2 -> buf0
// ----------------------------------------------------------------------------
class TrackBus
{
public:
    void clearAll() noexcept
    {
        for (auto& b : buf)  for (auto& v : b)  v = 0;
        for (auto& v : ext)  v = 0;
    }

    // Выбор входного буфера машины (P:$0113-$0131)
    const s24* inputFor (const RouteFlags& f) const noexcept
    {
        if (f.inputExt) return ext;
        if (f.inBuf0)   return buf[0];
        if (f.inBuf1)   return buf[1];
        return buf[2];
    }

    // Запись выхода голоса (P:$0143-$0218)
    void writeOut (const RouteFlags& f, const s24* src) noexcept
    {
        if (f.outBuf0) store (buf[0], src, f.overBuf0);
        if (f.outBuf1) store (buf[1], src, f.overBuf1);
        if (f.outBuf2) store (buf[2], src, f.overBuf2);
    }

    // Сумма в основной микс
    void sumToMain (s24* out) const noexcept
    {
        for (int i = 0; i < kBlockWords; ++i)
            out[i] = sat24 ((s56) buf[0][i] + buf[1][i] + buf[2][i]);
    }

    s24 buf[kBuffers][kBlockWords] {};
    s24 ext[kBlockWords] {};

private:
    static void store (s24* dst, const s24* src, bool overwrite) noexcept
    {
        if (overwrite)
            for (int i = 0; i < kBlockWords; ++i) dst[i] = src[i];            // INSERT
        else
            for (int i = 0; i < kBlockWords; ++i)
                dst[i] = sat24 ((s56) dst[i] + src[i]);                       // параллельно
    }
};

} // namespace mmnova
