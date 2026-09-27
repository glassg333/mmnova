# CHORUS — DSP-папка машины 15 (проверено + строгий ROM-режим "ex")

## Статус машины (см. также ../chorus_lfo/LFO_VERDICT.md)

LFO хоруса **не был сломан** — это старый флаг времён 1.5.x, закрытый в 1.6.0:
`ChorusCore.cpp/.h` + `AudioAdapter` бит-в-бит совпадают с одобренной 1.6.0
(«идеальный хорус» по файлу-маркеру), машина 15 рендерится через
AudioAdapter → ChorusCore = прошивочное ядро P:$147661-$1477DE.

Поэтому базовый режим `mnm` для машины 15 = прошивка, отдельное ядро не нужно.

## Что делает эта папка

Три ЗАДОКУМЕНТИРОВАННЫХ отклонения порта от ROM (не LFO, но рядом):

1. `ChorusCore.cpp:345` — при MIX=127 ядро форсится в 100% wet
   (`if(x0==0x7f0000u){x0=0x7fffffu;}`); ROM оставляет 0.78% сухого
   остатка (−42 дБ).
2. `NovaDSP.h:191` — микс вынесен наружу (`coreParams[3]=127`), сухой =
   выровненный по латентности delayedInput (у ROM внутри ядра свой микс).
3. `NovaDSP.h:191` — INP подаётся снаружи линейно `p[7]/64`; ROM считает
   квадратичный INP²×4 внутри ядра (разница слышна на краях INP).

**Строгий ROM-режим `ex`** = убрать все три отклонения. Для A/B с одобренным
режимом 1.6.0.

## Патч (режим "ex", режим mnm не трогается)

1. `Source/dsp/ChorusCore.h` — в public-секцию класса ChorusCore:

```cpp
    void setStrictRom(bool on) noexcept { strictRom = on; } // 1.7.11 ex: строгий ROM (без clamp MIX=127)
```

   и в private:

```cpp
    bool strictRom = false; // 1.7.11 ex
```

2. `Source/dsp/ChorusCore.cpp`, строка 345 — ЗАМЕНИТЬ:

```cpp
// БЫЛО:
if(x0==0x7f0000u){x0=0x7fffffu;} // MIX=127 clamp: full wet, zero dry (deliberate deviation; ROM leaves x0=0x7f0000 -> 0x7fff/0x800000 ~0.78% dry residue)
// СТАЛО:
if(!strictRom && x0==0x7f0000u){x0=0x7fffffu;} // 1.7.11 ex: в строгом ROM-режиме оставляем 0.78% сухого остатка (P:$1477CF-$1477D2)
```

3. `Source/NovaDSP.h`, строка ~191 (id==15) — ЗАМЕНИТЬ:

```cpp
// БЫЛО:
if(id==15) {for(int i=0;i<8;++i)chorusParams[static_cast<size_t>(i)]=b(i);auto coreParams=chorusParams;coreParams[3]=127;coreParams[7]=64;chorus->setParameters(coreParams);}
// СТАЛО:
if(id==15) {for(int i=0;i<8;++i)chorusParams[static_cast<size_t>(i)]=b(i);
    if(syntMode==monomachine::dspModeEx){chorus->setStrictRom(true);chorus->setParameters(chorusParams);} // 1.7.11 ex: ROM-микс и INP^2 внутри ядра
    else{auto coreParams=chorusParams;coreParams[3]=127;coreParams[7]=64;chorus->setParameters(coreParams);chorus->setStrictRom(false);}}
```

ВНИМАНИЕ: точное имя переключателя режимов (`syntMode==dspModeEx`) сверь с
тем, как это сделано в шаге bbox/NovaDSP.h этой же поставки — используется
тот же механизм, что и для машин 7/10/14/16/17/18/19.

4. Внешний микс/INP: в строгом режиме ядро считает микс и INP²×4 само
   (прошивочные законы P:$1477CF-$1477D2 и P:$1476xx) — внешняя линейная
   подача p[7]/64 в `AudioAdapter` в режиме ex не применяется (см. шаг 3:
   coreParams подаются без подмены). Если в твоей копии AudioAdapter дублирует
   внешнюю обработку — оставь как есть: ядро игнорирует внешние уровни только
   когда параметры не подменены, повторной обработки нет.

## A/B

DSP MODE → FX → режим `ex` на машине 15 (CHORUS):
- mnm = одобренный 1.6.0 (clamp MIX=127, внешний микс, линейный INP);
- ex = строгий ROM: на MIX=127 остаётся −42 дБ сухого остатка; INP² на краях
  ручки даёт иной входной уровень; фазовые отличия микса из-за отсутствия
  выноса.
Разница минимальна (отклонение №1 = −42 дБ, №2/№3 — на краях ручек) — это
ожидаемо; папка существует, чтобы A/B был возможен кнопкой, а не пайкой.
