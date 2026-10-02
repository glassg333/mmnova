# r18.3 (2026-09-30) — матрица по оригиналу

Пользователь прислал скрин оригинального firmware (полный UI omega 8) и
обвёл сломанную раскладку r18.2. Сравнение показало: в оригинальной матрице
ТОЛЬКО три секции — остальное (PARAMETERS-кластер, ENVELOPES, GLIDE/PAN)
в оригинале отсутствует (те параметры живут в верхней панели: осцилляторы,
фильтр, env A/D/S/R, LFO-колышки).

## Изменения (PluginEditor.h, buildCells)
- Убран: левый кластер PARAMETERS (uni/octav/voice/tune/prior/sync/vmod/sub/
  mtrg/filter + сетка lvl/osc/noise + иконки волн + pwm/fine/in/win/envlamt/
  invt/cutoff/hpf/reso/hpr/track/mode + строка xmod|dpth).
- Убран: блок ENVELOPES (7 строк dyn/dly/Atk/Dec/Dk2/Sus/Rel × env1-3).
  DLY/DYN переехали в правую секцию (как в оригинале).
- Убран: блок GLIDE / PAN (glide/mode/time/wave/sync/key) — в оригинале нет.
  (Побочный эффект: пропали из сборки r18.2 строки glide/mode — блок убран
  целиком, вопрос закрыт перестройкой.)
- Новая раскладка (1164×364, контент до x≈910):
  - CONTROLLERS (x8): 6 строк modwh1/dynamics/bender/pressure/cont1/cont2 ×
    [dest1 80][amt 36][dest2 80][amt 36] (те же байты MODW/DYN/BEND/PRES/C1/C2).
  - LFO1 / LFO2 / ENVS / XMOD (x360): строки LFO1/LFO2/ENV3 ×
    [dest1 60][amt 28][dest2 60][amt 28][dest3 60][amt 28];
    строка XMOD [dest 60][dpth 28]; строка W1/W2 (волны LFO1/LFO2, 60).
  - ENV DLY / DYN (x740): строки E1/E2/E3 × [dly 40][dyn 40];
    строки 1..8 × [pos 40][rate 40][dpth 40] (kArrPos/Rate/Depth).
- Аудит rects: 117 элементов, 0 пересечений box-box и label-box.
- Сверху UI (колышки LFO1/LFO2 RATE, ENV AMT, XMOD и т.д.) не тронут.

## Проверки
- Syntax-check JUCE 8.0.15 (g++14, core…gui_extra): 0 ошибок.
- Рендер матрицы headless-JUCE-приложением → omega8_r18_3_matrix_real.png
  (реальный вывод того же кода, что собирает пользователь).

## Не менялось
Движок (r18.2: pitch −32, xfade 120 мс, AUX2-SVF clamp, clearVoices,
RESET ALL) — байт в байт.
