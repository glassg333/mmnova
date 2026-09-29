# Omega8 — VST3-версия синтезатора Studio Electronics Omega 8 / Omega CODE

Папка проекта (для репозитория `glassg333/mmnova` — положить как `omega8/`).
JUCE + Visual Studio (Windows). Ключевая задача — **запуск оригинальных патчей
(SE 176 байт) из фабричных банков `.syx`/`.mid`**: формат декомпилирован
статистическим анализом банков и сверен со скриншотом редактора Omega/CODE
(78/78 проверенных значений совпали с патчем REVELATION).

## Сборка

### Вариант A — ваш билдер (рекомендую, как у Monomachine Nova)
1. Папку `omega8` (с этим `Omega8.jucer`) положить в `E:\mm\`.
2. Перетащить папку на `JUCE\juce + vs builder\build_vst3.bat`
   (или запустить bat без аргументов и выбрать папку).
   Билдер сам: переключит модули на `useGlobalPath=1`, сделает
   `Projucer --resave`, соберёт Release x64 и скопирует .vst3 в
   `C:\Program Files\Common Files\VST3\!build test`.
3. Схема .jucer снята 1:1 с рабочего `Monomachine Nova Synth 1.9.9`
   (корень `<JUCERPROJECT>`, те же модули, включая
   `juce_audio_plugin_client` и `juce_audio_processors_headless`,
   экспортёр VS2022 x64 `/utf-8`) — см. `docs/BUILD_SCHEME_RU.md`.

### Вариант B — Projucer руками
1. Открыть `Omega8.jucer` в Projucer (`C:\JUCE\Projucer.exe`).
2. File → Save and Open in IDE → VS2022 → Release x64.
3. VST3: `Builds/VisualStudio2022/x64/Release/VST3/`.

### Вариант C — CMake (запасной)
```
cmake -B build -G "Visual Studio 17 2022" -DJUCE_DIR=C:/JUCE
cmake --build build --config Release
```

## Что работает
- **Загрузка оригинальных банков**: `.syx` и SMF `.mid` (только SysEx-потоки,
  размер кадра 8+128×176). Фабричный банк A встроен как ресурс — плагин
  стартует уже с 128 патчами; кнопка `LOAD BANK .syx/.mid` загружает любой
  другой (B, RAM-дампы, пользовательские).
- **Мгновенное переключение патчей на лету**: параметры **Preset A** и
  **Preset B** (0–127, автоматизируемые, ступенчатые) — крутятся из DAW.
- **Морф/переключение пресета — автоматизируемый параметр `Patch Morph A→B`**
  (0–100%): посимвольная интерполяция всех 160 байт параметров (причём
  в т.ч. линейно, не «скачком») — можно рисовать энкодером в DAW.
- 8 стерео-голосов с «летающей» панорамой (per-voice rate/pos/depth —
  массивы байтов 136/144/152), 2 LFO + пан-LFO, 3 огибающие, XMOD,
  матрица 24 destinations (как в мануале), фильтры:
  `SEM LP/BP/HP/BR`, `MINI` (лестница Moog), `AUX1` (Oberheim SVF),
  `AUX2` (CS80: HP hpf/hpr + LP).
- Арпеджиатор (автоматизируемый: on/off, rate, pattern, octaves).
- Пресеты/морф автоматизируемы; state DAW сохраняет (с путём банка).

## Карта байтов (декодировано; `Source/OmegaPatch.h`)
Подтверждены редактором (conf=3): LFO1 rate 8 / depth 10–12 / dest 13–15,
LFO2 21 / 23–25 / 26–28, OSC1/2 freq 30/31, PWM 34/35, level 37/38/noise 39,
XMOD dpth/dest 41/42, TYPE 43 (0=SEM LP,1=BP,2=HP,3=BR,4=MINI,5=OBER,6=CS80),
CUTOFF 44, RESO 45, TRACK 46, ENV1 AMT 47, env1 49–53, env2 54–58, env3
59–63 (A,D,Dk2,S,R), env3 matrix dest 64–66 / amt 67–69, EXT IN 76,
HPR 79, WIN 80, контроллеры 86–109 (modwhl, dynamics, bender, pressure,
cont1, cont2 — cont2 со знаком: храним = отобр.+64), DYN 112–114,
pan 136/144/152 (8×rate, pos, depth), имя 160–175, GLIDE time 0 / flags 1.
Сильные (conf=2): OCTAVE 2 (нibble: 64=MID, шаг 16 = октава), ENV3 AMT 3,
SUB 4, PRIOR 7, VOLUME 32, HPF 40, OSC2 FINE 78, MASTER TUNE 77.
Неизвестные (остались сырыми, на поведение влияют нейтрально): 6,17,18,33,36,
70–73,75,81–85,110,111,134 — байты загружаются/сохраняются как есть.

## Известные ограничения v1
- Стадия `Dk2` (вторая затухающая стадия огибающей) хранится, но в DSP пока
  не участвует (A/D/S/R участвуют) — нужен аппаратный сверочный тест.
- Источники cont1/cont2 замаплены на CC16/CC17; приоритет (LOW/HIGH/LAST) и
  режимы multi-trigger — декодированы, влияют минимально.
- Кнопка сравнения/редактирования страниц (как на железе) — только просмотр
  значений; звуковые параметры читаются из морф-патча.

## Структура
```
Omega8.jucer            — Projucer-проект (VS2022)
CMakeLists.txt          — альтернативный путь сборки
Source/OmegaPatch.h     — 176-байтовая структура + карта + загрузчик банков
Source/OmegaEngine.h    — DSP (осцилляторы, фильтры, env, LFO, пан, матрица)
Source/PluginProcessor.h/.cpp — параметры (Preset A/B, Morph!), MIDI, арп, банки
Source/PluginEditor.h   — панель в стиле железа (Multi/Modulation/Programmer/
                          Oscillators/Filter/Envelopes/Arpeggiator)
banks/*.syx             — фабричные банки (A встроен, остальные — кнопкой)
```
