#pragma once
#include "MnMFixed.h"
#include <cstdint>
#include <cstring>

// ============================================================================
//  MnmVoiceRouting.h — РОУТИНГ ГОЛОСА Monomachine OS 1.32B
//  Портировано БУКВАЛЬНО из опкодов P:$0135-$0220 (прислал пользователь).
//  Это НЕ реконструкция по доке — это прямое чтение инструкций.
//
//  Адреса и регистры (подтверждено опкодами):
//    X:$2C0 = $0160  → buf0 (выход трека 1)
//    X:$2C1 = $01C0  → buf1 (трек 2)
//    X:$2C2 = $0220  → buf2 (трек 3)
//    X:$2C3          → входной буфер машины (r0 в P:$0113-$0131)
//    X:$2C4          → флаги роутинга (см. RouteFlags)
//    X:$2C8          → база HI08 (роль DSP: $140/$160/$180)
//    Y:$123          → V (база текущего голоса, +$100 каждый)
//    Y:$124          → счётчик голоса 0..2 (3 голоса за проход)
//    X:$4FE          → watchdog (чистит ColdFire; эмулятор сбрасывает сам)
//
//  Вход в post-voice (P:$013D-$0142):
//    lua (r6-$28),r6   ; r6 = V - $28 = $500  ← база страницы голоса!
//    move #>$100,r7    ; r7 = $100 (рабочее окно Y:$100)
//    jmp  func_0004a8  ; → там фильтр, огибающая, DIST, EQ
// ============================================================================

namespace mmnova {

static constexpr int kBlockWords = 32;   // 16 стерео-кадров x 2 (L,R) = 32 слова
static constexpr int kVoicesPerPass = 3; // cmp #<$3,a ; P:$16E
static constexpr int kTrackBufs = 3;

// Биты слова X:$2C4
struct RouteFlags
{
    bool outBuf0 = false;  // bit0 : писать в buf0
    bool outBuf1 = false;  // bit1 : писать в buf1
    bool outBuf2 = false;  // bit2 : писать в buf2
    bool modeBit3 = false; // bit3 : ГЕЙТ! разрешает ADD/OVERWRITE (P:$0149)
    bool sumToMix = false; // bit8 : func_0001ad (сумма 3 буферов в микс)
    bool loadExt  = false; // bit10: func_0001c2 (заливка EXT/деинтерливинг)
    bool overBuf0 = false; // bit12: перезапись buf0 (иначе добавление)
    bool overBuf1 = false; // bit13
    bool overBuf2 = false; // bit14
};

// ---------------------------------------------------------------------------
//  func_0001e2 — ADD (накопление)  P:$01E2-$01EB
//  buf[i] += src[i]  для 32 слов.
//  Важно: do #<$10 = 16 итераций, но тело обрабатывает 2 слова за проход.
//  Один холостой предвыбор на i=32 (читается, не сохраняется).
// ---------------------------------------------------------------------------
static inline void addVoiceToBuffer (int32_t* xBuf, const int32_t* ySrc) noexcept
{
    for (int i = 0; i < kBlockWords; ++i)
        xBuf[i] = sat24 (static_cast<int64_t> (xBuf[i]) + ySrc[i]);
}

// ---------------------------------------------------------------------------
//  func_0001ec — OVERWRITE (перезапись / INSERT)  P:$01EC-$01F2
//  buf[i] = src[i]  для 32 слов. Чистое копирование.
// ---------------------------------------------------------------------------
static inline void overwriteBuffer (int32_t* xBuf, const int32_t* ySrc) noexcept
{
    for (int i = 0; i < kBlockWords; ++i)
        xBuf[i] = ySrc[i];
}

// ---------------------------------------------------------------------------
//  func_0001ad — СУММА 3 БУФЕРОВ В ОСНОВНОЙ МИКС (in-place в buf0)
//  Ключевой факт из опкода P:$01B4: y0 = $7FFFFF — коэффициент ЕДИНИЦА.
//  buf0[i] = 1.0*buf0[i] + 1.0*buf1[i] + 1.0*buf2[i] — БЕЗ деления.
//  Клип применяется только при записи в память (SR.SM).
// ---------------------------------------------------------------------------
static inline void sumBuffersToMix (int32_t* buf0,
                                    const int32_t* buf1,
                                    const int32_t* buf2) noexcept
{
    for (int i = 0; i < kBlockWords; ++i)
    {
        const int64_t sum = static_cast<int64_t> (buf0[i])
                          + static_cast<int64_t> (buf1[i])
                          + static_cast<int64_t> (buf2[i]);
        buf0[i] = sat24 (sum);   // move a,x:(r0)+ с насыщением SR.SM
    }
}

// ---------------------------------------------------------------------------
//  func_0001c2 — EXT де-интерливинг (заливка внешнего входа / ESAI ping-pong)
//  Источник: две половины, разнесённые на 16 слов (n0 = $10).
//  Приёмник: один интерливированный поток L,R,L,R...
//  Повторяется для buf1 (X:$2CB) и buf2 (X:$2CC).
// ---------------------------------------------------------------------------
static inline void loadExtInterleaved (int32_t* dstBuf,
                                       const int32_t* srcEven,
                                       const int32_t* srcOdd) noexcept
{
    for (int i = 0; i < 16; ++i)
    {
        dstBuf[i * 2 + 0] = srcEven[i];   // move x:(r0)+,x0 ; move x0,x:(r2)+
        dstBuf[i * 2 + 1] = srcOdd[i];    // move x:(r1)+,x1 ; move x1,x:(r2)+
    }
}

// ---------------------------------------------------------------------------
//  ПОЛНЫЙ ВЫХОД ГОЛОСА (эквивалент P:$0143-$0218)
//  Вызывается ПОСЛЕ func_0004a8 (post-voice: фильтр/огибающая/DIST/EQ).
//  src = 32 слова готового стерео голоса (Y:$0000..$001F при r4=0).
// ---------------------------------------------------------------------------
static inline void routeVoiceOutput (const RouteFlags& f,
                                     const int32_t* src,
                                     int32_t* buf0, int32_t* buf1, int32_t* buf2)
{
    if (! f.modeBit3)
    {
        // Бит 3 сброшен → биты 0/1/2 ВСЕГДА дают ADD (P:$014B-$0160)
        if (f.outBuf0) addVoiceToBuffer (buf0, src);
        if (f.outBuf1) addVoiceToBuffer (buf1, src);
        if (f.outBuf2) addVoiceToBuffer (buf2, src);
    }
    else
    {
        // Бит 3 установлен → биты 12/13/14 выбирают ADD или OVERWRITE
        // (P:$01F3-$0218: bsclr #$c → ADD, bsset #$c → OVERWRITE)
        if (f.outBuf0)
            f.overBuf0 ? overwriteBuffer (buf0, src) : addVoiceToBuffer (buf0, src);
        if (f.outBuf1)
            f.overBuf1 ? overwriteBuffer (buf1, src) : addVoiceToBuffer (buf1, src);
        if (f.outBuf2)
            f.overBuf2 ? overwriteBuffer (buf2, src) : addVoiceToBuffer (buf2, src);
    }
}

} // namespace mmnova
