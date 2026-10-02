OMEGA8 VST — r18.8 (v25) — 2026-10-01
=====================================
Правки раскладки матрицы (по скриншотам с обводкой):

1) XMOD — ТРИ ОКНА ПО ЦЕНТРУ:
   PARAMETERS-блок перестроен в ровную сетку:
   [uni/voice/prior/vmod/mtrg] [octav/tune/sync/sub/filter]
   | lvl/osc1 | иконки волн | pwm/mode |
   | ...      | ...         | .../fine |  (все колонки выровнены)
   + отдельный ряд "xmod/dpth" ПО ЦЕНТРУ: [label][dpth 000..127][dest OSC1/PW/VCF]
   (в r18.7 xmod висел слева + дублировался справа-сверху — убрано)

2) Матрица в середине РАСПИТАНА, параметры ВЫРОВНЕНЫ:
   все колонки PARAMETERS — единая сетка (одни x-позиции на все строки);
   ENVELOPES/PER-VOICE PAN сдвинуты вправо (+60), центр получил место;
   навигация страниц — правый верх.

3) СИНКИ сверены по мануалу (omega8 manual.pdf, LFO edit page 2/3, pan page 4):
   LFO [SYNC]  = midi clock sync times: SELF / 1/1 / 1/2 / 1/4 / 1/8
   pan  [CLK]  = midi clock sync times: SELF / 1/1 / 1/2 / 1/4 / 1/8
   (в r18.7 стояли ошибочные DOWN/UP — исправлено)
   LFO [KEY]  = key trigger: SELF/DN/UP/ALL (factory DN)
   LFO [MODE] = MONO/POLY; LFO [QUAN] = OFF/ON
   VST: sync = индикатор (host midi-clock в DSP не берём — честно)

4) СТРАНИЦА BANK:
   пресеты теперь занимают основную часть страницы: 24 пресета банка
   (3 колонки x 8, реальные имена из factory-банка);
   строки банка (loaded / slot A / slot B / modes) — короткие, слева.

5) Сверено: MANUAL (перезапущен pdftotext: LFO pages 2/3 = WAVE/MODE/SYNC +
   KEY/QUAN; pan page 4 = WAVE/KEY/CLK; "sync" = osc2->osc1),
   presets/ (имена в BANK-сетке), firmware/ (off-байты 116/119/120/121..128).

DSP smoke: 2 multi parts + noteOn(60) -> finite=OK, maxAbs=0.5222, rms=0.1259
Step-playback секвенсора (авто-переключение частей по времени) = r18.9.
