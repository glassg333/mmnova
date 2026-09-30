# MАНУАЛ MONOMACHINE: цитаты по фильтру, энвелоупам и делэю
(официальный Monomachine SFX-60/MkII User Manual, 158 стр., OS 1.5x-era, тот же UI что 1.32;
файл мануала: Monomachine_SFX60_MkII_Manual.pdf; полный текст: Monomachine_Manual_fulltext.txt.
Нумерация страниц по внутренней нумерации мануала, «1-xx».)

## 1. FILTER PAGE — стр. 29–31 («TRACK EFFECTS»)

- «In the Filter page you find the controls for the combined resonant 24dB low/band/high-pass
  filter of the Monomachine, including the filter envelope.»
- «BASE (filter base) controls the base filter cut-off frequency. When WDTH is set to its maximal
  value, the BASE parameter functions as the cut off parameter of a high-pass filter.»
- «WDTH (filter width) controls the filter gap width, that is the distance between the high pass
  and low pass cut-off frequencies.»
- «HPQ (high pass filter Q) ... LPQ (low pass filter Q) controls how much the volume is boosted
  around the low-pass cut off frequency.»

## 2. FILTER ENVELOPE — стр. 31

- «The filter envelope is trigged every time a FILTER-trig is received.»
- «ATK (filter envelope attack) controls the attack of the filter envelope»
- «DEC (filter envelope decay) controls the decay of the filter envelope»
- «BOFS (filter base envelope offset) sets the value that will be added to the filter BASE
  parameter. BOFS is controlled by ATK and DEC.»
- «WOFS (filter width envelope offset) sets the value that will be added to the filter WDTH
  parameter. WOFS is controlled by ATK and DEC.»
- FILTER TRACKING: «The filter normally tracks the pitch of the note played. The high pass filter
  starts cutting two octaves under the base pitch when the BASE parameter is set to zero.»

ВЫВОД: мануал описывает ATK/DEC/BOFS/WOFS ИСКЛЮЧИТЕЛЬНО внутри FILTER PAGE —
это энвелопа ФИЛЬТРА, её адресаты — BASE/WDTH фильтра. Никакого упоминания делэя.

## 3. DELAY — стр. 33–34 (EFFECTS PAGE)

- «The delay of the Monomachine is a single tap delay with real-time adjustable delay time...
  The delay time is always synchronised to the global tempo setting.»
- «DSND (delay send) controls the level of sound that will be sent to the delay loop...
  For negative values the stereo image of the signal will be preserved and for positive values
  it will be switched left-right. The feedback loop will also be switched depending if the DSND
  is set to positive or negative values... positive values for ping-pong delay.»
- «DFB (delay feedback) parameter controls how much of the delay output sound will be fed back
  into the input of the delay.»
- «DBAS (delay filter base) controls the high pass filtering of the signal from the delay
  feedback loop.»
- «DWID (delay filter width) controls the low pass filtering of the signal from the delay
  feedback loop, relative the DBAS parameter.»
- Рис. 5 (стр. 34): блок-схема «TRACK EFFECT DELAY — AUDIO SIGNAL PATH»:
  DELAY LOOP (DTIM = delay time, «TAPE SPEED») + DELAY FEEDBACK (DFB) +
  DELAY FILTER: «DBAS = FILTER BASE, DWID = FILTER WIDTH»; DSND = send level.
- «Using the DBAS and DWID parameters you can make the delay "echoes" sound sequentially
  different as the sound goes round the delay loop.»

ВЫВОД: делэй — ОТДЕЛЬНЫЙ блок со своим фильтром (DBAS/DWID) и своим
поведением (DSND-знак = пинг-понг). В схеме фильтр страницы FLT и делэй —
разные блоки тракта.

## 4. MIDI-карта (Appendix B, стр. B-2/B-3)

- CC72–79: Filter Base / Width / HPQ / LPQ / Attack / Decay / Base Offset / Width Offset
- CC80–87: Effects EQ Freq / EQ Gain / SRR / Delay Time / Delay Send / Delay Feedback /
  Delay Filter Base / Delay Filter Width

ВЫВОД: Base/Width Offset (CC78/79, ячейки $416/$417) и Delay Filter Base/Width
(CC86/87, ячейки $41E/$41F) — разные параметры. Версия «BOFS/WOFS = фильтр делэя»
не соответствует официальной карте.

## 5. Сверка с прошивкой (OS 1.32B) после exp68

| Мануал | Прошивка | Статус |
|---|---|---|
| FILTER ENVELOPE (ATK/DEC/BOFS/WOFS) — часть фильтра | стадия 2 ($0A5D–$0AD0), выход → микс кадра; делэй не трогает | СОВПАДАЕТ (после исправления exp68) |
| DELAY: свой фильтр DBAS/DWID | DBAS $41E → пролог $02C8 (база тапов); DWID $41F → $0B1E (глубина модуляции тапов, ×env2) | СОВПАДАЕТ |
| DSND-знак → пинг-понг | ветка по знаку $0284/$029C–$02A8 в прологе; сам уровень — хост | СОВПАДАЕТ |
| DFB — фидбек | кадром не читается (хост/блок-уровень) | СОГЛАСУЕТСЯ |
| «filter envelope» против «delay» | два независимых блока; связь «стадия 2 → тапы делэя» ОПРОВЕРГНУТА (exp68) | СОВПАДАЕТ с моделью пользователя |
