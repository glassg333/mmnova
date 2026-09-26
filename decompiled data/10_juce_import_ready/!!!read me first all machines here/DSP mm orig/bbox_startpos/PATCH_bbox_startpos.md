# BBOX STRT (m7) — прошивочный закон старт-позиции, режим "ex"

## Диагноз
В порте (`Source/dsp/monomachine_bbox.hpp`, `startPosition()`, строка ~67)
закон выдуманный:

```cpp
double startPosition(const SampleSlot& slot)const{const double x=static_cast<double>(m_paramStrt)/127;return x*x*x*std::min(static_cast<double>(slot.activeFrames?slot.activeFrames:slot.data.size())*0.25,slot.originalSampleRate*0.025);}
```

— кубическая кривая с шапкой ~25 мс и делением на 4. На прошивке STRT
сдвигает старт на **до половины сэмпла** и линейно — это совершенно другой
звук (особенно на длинных семплах: 25 мс от трёхсекундного сэмпла — почти
ничто, отсюда жалоба «ручка start pos сделана неверно»).

## Прошивочный закон (листинг 07_DPRO-BBOX_full.txt, config P:$1474ED-$147510)

```
147503: move y:(r0+n0),x0   ; A = начало слота ROM (таблица $147632, по ноте)
147504: move y:(r1+n1),b    ; B = конец слота ROM   (таблица $147633)
147505: sub  #>$1,b         ; span = B−1−A
147507: sub  x0,b
147508: move y:(r6+$5),y0   ; STRT (param $5, knob<<16 => K/128)
14750a: mpy  y1,y0,a        ; a = span × STRT/128   (1.23)
14750b: lsr  a              ; a = span × STRT/256
14750d: add  x0,a
14750e: move a,y:(r6+$14)   ; СТАРТ = A + span×STRT/256
14750f: sub  y0,b
147510: move b,y:(r6+$15)   ; порог конца = span×(1−STRT/256) — окно
                            ; сжимается слева, конец слота зафиксирован
```

**STRT (K = 0..127): δ = span·K/256 — линейно, максимум ≈ 49.6% слота,
без капов по миллисекундам.** Проигрывание идёт от δ до конца слота.
Никакой кубической кривой в прошивке нет.

## Патч (режим "ex", существующее поведение остаётся в mnm/old)

1. `Source/dsp/monomachine_bbox.hpp` — рядом с `setChromatic` (строка ~128):

```cpp
    void setFirmwareStart(bool on) noexcept { m_firmwareStart = on; } // 1.7.11 ex: прошивочный закон STRT (P:$14750A)
```

2. Туда же, в `private:` (рядом с `m_chromatic`):

```cpp
    bool m_firmwareStart = false; // 1.7.11 ex
```

3. `startPosition()` (строка ~67) — ЗАМЕНИТЬ целиком:

```cpp
    // 1.6.12: STRT мягче (жалоба: на 50 уже срезал слишком много): кубическая
    // кривая и шапка 25 мс вместо квадратичной на 30 мс; STRT=0 всегда точный старт.
    double startPosition(const SampleSlot& slot)const{
        const double span=static_cast<double>(slot.activeFrames?slot.activeFrames:slot.data.size());
        if(m_firmwareStart)return span*(static_cast<double>(m_paramStrt)/256.0); // ex: OS 1.32B P:$14750A-$14750E: delta=span*STRT/256, линейно, до ~49.6%, окно до конца слота
        const double x=static_cast<double>(m_paramStrt)/127;return x*x*x*std::min(span*0.25,slot.originalSampleRate*0.025);
    }
```

4. `Source/NovaDSP.h`, `set()`, строка ~189:

```cpp
    // БЫЛО:
    if(id==7){bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);}
    // СТАЛО:
    if(id==7){bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);bbox.setFirmwareStart(syntMode==monomachine::dspModeEx);} // 1.7.11 ex
```

## Что НЕ трогается
- Random-start / random-slot — сознательные моды плагина (как и импорт
  семплов) — остаются как есть.
- Адаптивный деклик 1.6.12 — остаётся (в прошивке ROM-слоты затухают сами,
  у пользовательских семплов без деклика был бы щелчок; на закон STRT не влияет).
- RTIM/RTRG законы v6 — вне этой задачи.

## A/B
Кнопка DSP MODE → SYNT → режим `ex` на машине 7 (параметр `mode_synt_m7`):
mnm = кубика с капом 25 мс, ex = прошивка (линейно до половины сэмпла).
Разница слышна на длинных семплах: ex на STRT=127 играет вторую половину.
