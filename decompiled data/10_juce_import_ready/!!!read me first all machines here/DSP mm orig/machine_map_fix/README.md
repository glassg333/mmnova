# FIX: полная карта машин OS 1.32B + исправления имён файлов/заголовков

Дата: 2026-09-25. Уровень доказательств: T0 (строки имён прочитаны прямо из
ROM ColdFire: 05_descriptors/coldfire_main.bin, 425858 B, область дескрипторов
$57FC5–$58E35; поисковые пробы: «DLY» = 0 вхождений, «FX-DLY» = 0,
«FX-EQ» = 0, «FX-EXT» = 0, «DDRW» = 1, «DENS» = 1).

## 1. Итоговая карта машин (21 машина; слоты 20–31 в диспетчере DSP ПУСТЫ —
указатель-заглушка $100169, dsp1_dispatch.txt ~$10016C)

| ID | Имя (ROM) | Init | Proc | Слов | Слоты парам. (ROM) |
|----|-----------|------|------|------|--------------------|
| m1 | GND-SIN | $144CC9 | $144CD0 | 159 | ---×7 TUNE |
| m2 | GND-NOIS | $144D9A | $144DA2 | 193 | ST RED STON ---×4 TUNE |
| m3 | SID-6581 | $14636F | $146382 | 323 | PW PWAD PWRS WAVE MOD MSRC MFRQ TUNE |
| m4 | SWAVE-SAW | $145932 | $14594A | 322 | UNIL UNIW UNIX --- SUBX SUB1 SUB2 TUNE |
| m5 | SWAVE-PULS | $147214 | $14723E | 338 | UNIL UNIW SUB1 SUB2 PW PWAD PWRS TUNE |
| m6 | DPRO-WAVE | $1473A2 | $1473B8 | 258 | WAVE WP WPM WPRS SYNC SFRQ --- TUNE |
| m7 | DPRO-BBOX | $1474E9 | $14752B | 283 | PTCH STRT --- --- RTRG RTIM --- --- |
| m8 | FM-STAT | $145D12 | $145D21 | 343 | 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE |
| m9 | FM-PAR | $145EC9 | $145EDB | 605 | 1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE |
| m10 | FM-DYN | $14619D | $1461C1 | 726 | 1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB TUNE |
| m11 | VO-6 | $1464FC | $1469BF | 2267 | VOC1 VOC2 V-SW VOIC CONS CLEN CVOL TUNE |
| m12 | FX-THRU | $14764B | $14764C | 18 | ---×7 INP |
| m13 | FX-REVERB | $14538E | $1453DC | 1257 | DEC DAMP GATE MIX HP LP --- INP |
| m14 | SWAVE-ENS | $145AB5 | $145AE3 | 462 | PCH2 PCH3 PCH4 WAVE PW CHRL CHRW TUNE |
| m15 | FX-CHORUS | $147661 | $14767B | 292 | DEL DEP SPD MIX FB WID LP INP |
| m16 | FX-DYNAMIX | $1477DE | $1477E6 | 149 | ATK REL THRS MIX RAT GAIN RMS INP |
| m17 | FX-RINGMOD | $14789E | $1478C7 | 504 | WAVE EXT --- MIX ---×3 INP |
| m18 | FX-PHASER | $145036 | $145045 | 662 | CNTR DEP SPD MIX FB WID --- INP |
| m19 | FX-FLANGER | $144E81 | $144E9A | 345 | DEL DEP SPD MIX FB WID --- INP |
| m32 | DPRO-DDRW | $147B38 | $147B56 | 402 | WAV1 MIX WAV2 TIME BR1 WID BR2 TUNE |
| m33 | DPRO-DENS | $147D01 | $147D1D | 737 | PCH2 PCH3 PCH4 WAVE --- CHRL CHRW TUNE |

## 2. Исправления к дереву decompiled data

1. `03_listings/machines_page_A/32_FX-DLY_full.txt` → на самом деле
   **DPRO-DDRW** (m32). Косвенные подтверждения: init читает ДВЕ пары
   параметров ($147B3B/$147B3D: y:(r6+$4), y:(r6+$6) → состояния
   $1A/$1B и $2A/$2B — WAV1/WAV2) и MIX ×$408102 ($147B4D).
2. `03_listings/machines_page_A/33_FX-EXT_full.txt` → на самом деле
   **DPRO-DENS** (m33). Подтверждения: init использует ту же схему буфера
   $114000 + y:$124×$2000, что init хоруса m15 ($147D11–$147D15) — DENS
   имеет собственный хорус CHRL/CHRW; proc — цикл do #<3 на 4 голоса с
   таблицей $101AFB (PCH2/PCH3/PCH4 + база; те же SWAVE-таблицы
   $101AFB/$101B7B/$101BFB/$101CFB лежат в 04_tables).
3. Заголовок `14_SWAVE-ENS_full.txt` внутри файла гласит «# slot m14
   FX-EQ» — НЕВЕРНО; имя файла верно: m14 = SWAVE-ENS (ROM-дескриптор на
   $58965: 'SWAVE....ENS......PCH2..PCH3..PCH4..WAVE').
4. `03_listings/machines_page_A/MANIFEST.txt` — устаревший (m14 «FX-EQ»,
   нет m8/m9/m10/m15, m32/m33 без имён). Не использовать как источник имён.
5. Машины-эффекта EQ и DLY в OS 1.32B ОТСУТСТВУЮТ (0 строк в ROM).
   Дилей — глобальная пер-трековая стадия тракта (см.
   ../trackdelay_srr/STATUS.md); EQ — стадия кернела P:$0789–$0807
   (гейн-делитель X:(r6+$0B), 24-итерационный DIV, таблицы $1447C6/$143546).

## 3. Порт (JUCE/Monomachine-Nova-1.7.11)

- NovaDSP.h: ветка `id==14||id==33` (ансамбль) уже соответствует ROM-карте;
  id==13 = generic juce::Reverb (не прошивка); ids 1/2/4/5/6/11 = generic.
- Для режима `ex` новые секции заводить по ROM-именам: ENS (m14), DDRW
  (m32), DENS (m33).

## 4. Что осталось майнить (очередь пользователя)

REV m13 (частично: топология гребёнок + слот-карта добыты, exp26) →
SIN m1 / NOIS m2 (слайсы простые 159/193 слов, но параметры ST/RED/STON и
питч обрабатываются КЕРНЕЛОМ вне слайса: proc m1/m2 не читает r6+$00–$0F;
нужен контекстный майнинг) → SID m3 → SAW m4 → PULS m5 → ENS m14 →
WAVE m6 → VO-6 m11 (2267 слов, крупнейшая) → Track Delay + SRR
(см. ../trackdelay_srr/STATUS.md).
