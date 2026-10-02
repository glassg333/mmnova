OMEGA8 VST — r18.7 (v24) — 2026-10-01
=====================================
1) XMOD: 3 окошка В ЦЕНТРЕ строки reso/hpr (как на фото оригинала):
   [label] [depth 0-99] [VCF dest dropdown]
2) LFO: добавлены колонки sync/key/mode/qun (все 3 строки LFO + ENV3),
   сверено с мануалом (LFO edit page2: WAVE/MODE/SYNC; page3: KEY/QUAN):
   sync = SELF/DOWN/UP (midi-clock, display only)
   key  = SELF/DOWN/UP/ALL (DOWN/ALL: reset LFO на noteOn; UP: reset на noteOff)
   mode = MONO/POLY (POLY: LFO-фаза зависит от слота голоса)
   qun  = OFF/ON (при ON: quantise LFO-скорости по темпу)
3) MULTI / секвенсор:
   - PAGE 3 матрицы = MULTI: 8 parts (patch из банка + vol + pan + on/off)
   - layer-playback: активные части играют одновременно (voice-слот
     распределяется по on-частям, pan части переопределяет)
   - лампы 1-8 = состояние parts (горит = часть активна; клик = on/off)
   - step-playback (авто-переключение частей по времени) = в r18.8
4) ФИКС РЕАЛЬНОГО БАГА: матрица теперь добавляется в editor
   (addChildComponent) — в r18.6 и раньше extend-окно было пустым,
   хотя кода матрицы было полно.
5) Проверено по: MANUAL (LFO pages 2/3, byte-map 121-128), presets/ (840 .syx),
   firmware/ (128 патчей x 512B), banks/Omega8FactoryMulti.syx (8 parts x 10B).

DSP smoke: 4 multi parts + noteOn(60) -> finite=OK, maxAbs=0.9191, rms=0.1366
Панель 1180x506 + extend 1180x886 с матрицей 1164x364 (3 страницы).
