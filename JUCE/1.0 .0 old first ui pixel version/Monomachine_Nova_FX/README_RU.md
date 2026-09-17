# MonomachineNova Unified FX • v1.2.0

**7 режимов аудиоэффектов, единый пиксельный GUI по предоставленному образцу.**
Это исходники отдельного Projucer/JUCE-проекта. Название проекта — MonomachineNova,
имя VST3 — MonomachineNova Unified FX. Скомпилированный Windows/macOS-бинарник в архив не входит.

## Важное различие с оригинальным устройством

Все 22 пункта dropdown реализованы в двух версиях (15 Synth + 7 FX), но это **самостоятельные нативные приближения**,
а не точная эмуляция Elektron Monomachine. Часть движков дописана заново; соответствие прошивке, тембрам,
параметрическим кривым и режимам аппаратуры не подтверждено. GND-GND намеренно выдаёт тишину.
Исходные ROM/firmware/PCM не используются. См. MODELS.md и VALIDATION.md.

## Быстрый запуск в Projucer

1. Распакуй этот архив в отдельную папку. **Не смешивай Synth и FX и не накладывай их на старые Source/Source FX.**
2. Установи JUCE 8; исходники проверены с **JUCE 8.0.9**. Открой `MonomachineNova.jucer` в Projucer.
3. В Global Paths укажи JUCE Modules → папку `JUCE/modules`. Проект использует глобальный путь для всех модулей.
4. Выбери exporter Visual Studio 2022, Xcode или Linux Makefile. Сохрани проект: это создаст `JuceLibraryCode` и `Builds`.
5. Открой в IDE. Для Windows выбери **Release / x64**, затем собери цель VST3.
   В Visual Studio нужны workload «Desktop development with C++» и Windows SDK.
6. Найди результат `.vst3` в `Builds/`. Копируй **весь пакет**, а не только внутреннюю библиотеку:
   - Windows: `C:\Program Files\Common Files\VST3\`.
   - macOS: `~/Library/Audio/Plug-Ins/VST3/`.
   - Linux: `~/.vst3/`.
7. Пересканируй VST3 в DAW. Для Synth нужен MIDI/instrument track; FX вставляется на audio track.

Если post-build copy выдаёт Access denied, отключи Plugin Copy Step у exporter, собери и скопируй пакет вручную.
Не запускай DAW/IDE от администратора без необходимости. Имена пунктов Projucer могут слегка отличаться между версиями.

### Не заменяй только четыре старых файла

Нужна **вся папка Source из этого архива**. Кроме Processor/Editor в ней есть NovaConfig.h, NovaData.h,
NovaDSP.h, PixelFont.h и изменённые DSP/models. Копирование только четырёх файлов приведёт к ошибкам include/API.
Скомпилируются только `PluginProcessor.cpp` и `PluginEditor.cpp`; `.h/.hpp` отдельно компилировать не надо.
`JuceHeader.h` генерируется Projucer. В дереве не должно остаться вторых копий PluginProcessor.cpp и createPluginFilter().

Оба `.jucer` называются одинаково, потому что это **два независимых проекта**. Настройки:

| Пункт | Synth | FX |
|---|---|---|
| Plugin Name | MonomachineNova Unified Synth | MonomachineNova Unified FX |
| Plugin Code | NvSy | NvFx |
| Manufacturer | MnOn | MnOn |
| Is Synth | Да | Нет |
| MIDI Input | Да | Да |
| MIDI Output / MIDI Effect | Нет / Нет | Нет / Нет |
| Формат / C++ | VST3 / C++17 | VST3 / C++17 |

Коды отличаются от прошлых архивов: старая версия не подменяется автоматически, её state несовместим с новой схемой.

## GUI и dropdown

Сетка как в референсе: SYNTHESIS, AMPLIFICATION, LFO1; FILTER, EFFECTS, LFO2 [LFO3].
Пиксельный шрифт встроен в код — внешние шрифты, изображения и интернет не нужны.
Логотип — собственный геометрический знак; это не логотип Elektron.

- Клик по чёрной полосе выбора машины открывает **dropdown с заголовками категорий**.
- Synth показывает GND / SID / SWAVE / DPRO / FM+ / VO; FX — категорию FX. Дизайн общий.
- В SYNTHESIS меняются названия и назначения восьми ячеек. **Каждая машина сохраняет свою отдельную группу параметров**.
- Серые `---` — действительно неиспользуемые ячейки исходного описания; они отключены и не зарегистрированы как фиктивные параметры.
- Ручки: перетаскивание мышью, Shift для тонкой настройки, двойной щелчок — значение по умолчанию.
- PAGE, DEST, TRIG, WAVE и MULT у LFO выбираются кликом. Для DEST пункты 1–8 соответствуют отображаемому назначению.
- Клик по заголовку **LFO2 [LFO3]** переключает редактируемый LFO. Все три генератора работают независимо.
- **LEV** — общий линейный уровень; **BPM** открывает настройки темпа/арпеджиатора.
- **MENU** открывает настройки, матрицу модуляции, загрузку BBOX, Panic и Reset.
- Окно масштабируется с сохранением пропорций. Screenshot Preview.png получен рендерингом настоящего JUCE-редактора.

## Что находится в MENU

### TEMPO / ARPEGGIATOR / GLOBAL

Use host tempo = ON использует BPM DAW, если хост его сообщает; иначе — Internal tempo (30–300 BPM).
При включённом host tempo изменение внутреннего BPM не меняет темп, сообщаемый хостом.
**LFO и arp используют темп, но не привязаны к позиции PPQ/началу такта DAW и не останавливаются автоматически со Stop.**

Arp Mode: OFF, KEY, SID, ADD. KEY использует удерживаемые ноты; SID удерживает накопленный набор до Panic/All Notes Off;
ADD накапливает ноты, пока не отпущены все клавиши. Это выбранная нативная трактовка режимов, не точная аппаратная логика SID.
Direction: TRUE (порядок нажатия), UP, DOWN, CYCL, RND. Range: 1–4 октавы.
Speed — число тиков 24 PPQN на ноту: 6 = 1/16, 12 = 1/8, 24 = 1/4; меньшее значение быстрее.
Gate length: 1–127 из 128 частей шага. Реальная слышимая длина дополнительно зависит от амплитудной Release.
Arp внутренний: события управляют движком/gate, но MIDI наружу не выдаётся. У FX нужен направленный в плагин MIDI.

У FX здесь есть Global dry wet и FX MIDI gate. Gate OFF — обычная обработка входа; Gate ON — MIDI открывает огибающую wet-пути.
Dry не гейтируется: для полного затвора поставь wet=127. Delay/reverb могут иметь хвосты. LEV влияет и на Dry, и на Wet.
FX INP: STEREO / LEFT / RIGHT / MONO — выбор/сведение основного входа, не аппаратная маршрутизация соседних треков.

Macro X / MIDI CC1 и Macro Y / MIDI CC11 — источники для матрицы.
Последний CC заменяет значение соответствующего макроса в DSP до изменения ручки/нового CC/Panic;
положение GUI-ручки при этом не обязано совпадать с последним CC. MIDI-override не сохраняется как параметр патча.

### MODULATION MATRIX

8 маршрутов. Для каждого: ON, SRC, DEST, DEPTH (-64…+63).
Источники: KEY, VEL, MACRO X/Y, LFO1/2/3. Назначения: SYN A–H, AMP, FILTER, FX — всего 32 слота.
KEY отсчитывается относительно MIDI 60; VEL — 0…1; Macro X — биполярный, Macro Y — униполярный.
По умолчанию маршруты выключены. Пример: ROUTE 1 ON → SRC LFO1 → DEST FILTER BASE → DEPTH 20.
У самой страницы LFO при этом можно оставить DPTH=0: матрица получает сигнал генератора независимо от его прямой глубины.
SYN A–H относится к **текущей** машине; если ячейка `---`, модулировать в ней нечего.

### BBOX SAMPLES (Synth)

10 слотов: выбери LOAD SLOT и WAV/AIFF. Затем выбери DPRO-BBOX и нужный SLOT в SYNTHESIS.
При загрузке используются **первые 5 секунд**, первые два канала сводятся в mono, данные пересчитываются в 44.1 кГц.
Это простая линейная интерполяция, не mastering-ресэмплер. Оригинальный файл не изменяется.
Чтение/подготовка выполняются вне аудиоколбэка. Во время короткого обмена буферов возможен пропуск аудиоблока:
**для загрузки/сброса сэмплов останови транспорт**. Загруженный слот останавливает текущий BBOX playback.

Custom PCM встраивается в state DAW (32-bit float, 44.1 кГц mono); исходный WAV после сохранения проекта не требуется.
Максимум примерно 9 МБ PCM на 10 слотов, плюс Base64/XML; файл проекта DAW может заметно увеличиться.
Восстановление PCM выполняется асинхронно через message thread. Дождись открытия/восстановления проекта перед воспроизведением.
RESTORE SYNTHETIC KIT возвращает встроенные процедурные kick/snare/hat. Это не аппаратные PCM Monomachine.
BBOX выбирает слот ручкой SLOT, а MIDI-нота транспонирует его: MIDI 60 — базовая высота. Это не GM drum-map.

### PANIC и RESET

PANIC очищает ноты, latch arp и хвосты; параметры не меняет.
RESET ALL PARAMETERS возвращает все банки машин и общие параметры к значениям по умолчанию; сэмплы остаются.
Для полного сброса отдельно используй RESTORE SYNTHETIC KIT. Reset посылает изменения параметров хосту — учитывай запись автоматизации.

## Основные страницы

**AMPLIFICATION** — AHD с отдельной Release: ATK ≈1–501 мс; HOLD 0–126 ≈0–1.98 с; **HOLD=127 удерживает уровень до Note Off**.
DEC/REL задают постоянную времени ≈5–1505 мс, а не время до абсолютной тишины; затухание продолжается несколько постоянных времени.
VOL = 0–127, PAN отображается относительно 64. PORT — glide до 2 с, применяется на следующем Note On (Synth; также высота Ring Mod с MIDI).
Остальные FX не являются питч-синтезаторами: PORT в них не меняет сигнал.
Для удерживаемого синтезаторного звука поставь HOLD=127. При HOLD=0 нота затухает даже с зажатой клавишей.

**FILTER** — HPF(BASE) → LPF(BASE+WDTH), Q отдельно для обоих фильтров. BASE 0–127 приблизительно 20 Гц–20 кГц.
ATK/DEC управляют фильтровой огибающей, BOFS/WOFS — смещением базы/ширины (в GUI 0 соответствует raw64).
Огибающая запускается MIDI/arp-нотами; у FX это работает и при выключенном амплитудном MIDI gate.

**EFFECTS** — EQF: центр peak-EQ; EQG: примерно -18…+17.7 dB относительно raw64; фиксированный Q≈1.
SRR — sample-and-hold снижение частоты обновления, 0 выключено. DTIM ≈1–750 мс, без синхронизации delay к BPM.
DSND — send в delay, DFB — feedback до 95%; DBAS/DWID — HP/LP-фильтр петли delay.
Изменение времени задержки сглажено и может менять высоту эха. При DSND=0 уже накопленный хвост продолжает затухать.

**LFO1/2/3** — PAGE выбирает PTCH/SYN/AMP/FLT/FX; DEST — параметр 1–8 выбранной страницы.
Для PTCH назначения: 1,2,3,5,7 полутонов / 1,2,3 октавы. Высота у FX применяется к Ring Mod.
TRIG: FREE свободно; TRIG перезапускается нотой; HOLD удерживает значение, снятое на ноте; ONE — один цикл после ноты.
WAVE: TRI/SAW/SQR/EXP/RND; MULT: 1x–64x; SPD: скорость относительно темпа; INTL: фазовый сдвиг;
DPTH отображается -64…+63, значение 0 нейтрально. Пример вибрато: PAGE PTCH, DEST 1ST, WAVE TRI, DPTH 8.
Модуляция рассчитывается порциями не более 32 сэмплов; это не sample-accurate автоматизация параметров.

## Сигнальная цепь и ограничения

Machine synth / selected FX → SRR → distortion → HP/LP filter → peak EQ → amp/volume/pan → filtered send delay → global wet/dry (FX) → LEV.
У разных FX выбирается **одна FX-машина**; чтобы одновременно поставить chorus + phaser, используй два экземпляра FX в DAW.
SYNTHESIS-параметры эффектов управляют выбранной машиной, EFFECTS ниже — общей EQ/SRR/delay-цепочкой.
FX-THRU обходит только машину: общий фильтр/EQ/delay всё ещё действуют. Для прозрачного обхода wet-цепи используй Global dry wet=0.

Синтез монофонический, last-note priority, MIDI 1–16 (omni), CC64 sustain, pitch bend ±2 полутона.
Стерео у ENS/DENS — ширина внутреннего ансамбля, а не полифония. Нет MPE, аппаратного секвенсора и отдельных hardware output buses.
Portamento, режимы LFO/arp и дополнительные параметры BBOX — реализация этой интеграции; не выдаются за прошивку устройства.

**Нет выходного лимитера и полного oversampling.** Сильная резонансность/EQ/drive/feedback могут превысить 0 dBFS.
Начинай с небольшой громкости. Надпись OVER означает: уменьши LEV/VOL/усиление. Некоторые digital/voice/SID-режимы дают aliasing.
Быстрая автоматизация не всех DSP-параметров сглажена; возможны zipper-noise и щелчки при смене машины/резком gate.
Выбор GND-GND означает тишину по назначению. Не путай это с неисправностью.

## Сохранение и автоматизация

Все зарегистрированные параметры (Synth 189, FX 130) доступны хосту, включая отдельные банки SYNTHESIS машин.
State сохраняет параметры и custom BBOX PCM. Активные ноты, фазы, содержимое delay/reverb и MIDI CC-overrides не сохраняются.
Внешнего браузера .fxp/.vstpreset нет; сохраняй проект/пресет средствами DAW.
Версия Unified использует новую схему и новые коды: патчи из предыдущей версии 1.0 не импортируются автоматически.

## Сборка CMake вместо Projucer

CMake 3.22+, JUCE 8.0.9 (папка с исходниками, содержащая CMakeLists.txt).
Отдельный VST3 SDK для этой сборки не нужен: используется SDK из JUCE.

Windows PowerShell:

```powershell
$env:CMAKE_BUILD_PARALLEL_LEVEL="1"
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DJUCE_DIR="C:/JUCE"
cmake --build build --config Release --target MonomachineNovaUnifiedFX_VST3
```

macOS/Linux:

```sh
export CMAKE_BUILD_PARALLEL_LEVEL=1
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DJUCE_DIR=/path/to/JUCE
cmake --build build --config Release --target MonomachineNovaUnifiedFX_VST3 --parallel 1
```

Результат: `build/MonomachineNovaUnifiedFX_artefacts/Release/VST3/`.
Linux development-зависимости: ALSA, X11/Xext/Xinerama/Xrandr/Xcursor, FreeType/Fontconfig, OpenGL.
На машине с малым RAM используй один поток сборки; для модулей JUCE может понадобиться swap.

Тесты: конфигурация `-DNOVA_BUILD_TESTS=ON`, сборка `MonomachineNovaUnifiedFXTests`, затем `ctest --test-dir build -C Release --output-on-failure`.
На headless Linux нужен Xvfb (`xvfb-run -a`).
Также вложен шаблон GitHub Actions для Windows/macOS: см. BUILD_CI.md. Он не запускался в рамках этой проверки.

## Типичные ошибки

- `JuceHeader.h not found`: сначала Save Project в Projucer, проверь module paths.
- `NovaConfig.h / NovaDSP.h not found`: скопирована не вся новая Source.
- Нет `swapSample`/`clearBuffers`: используется старый DSP-заголовок из предыдущего проекта.
- Duplicate symbols / createPluginFilter already defined: две копии старых `.cpp` в одной цели.
- VST3 не виден: архитектура/ОС должны совпадать с DAW; проверь путь, пересканирование и quarantine/blacklist хоста.
- Нет звука Synth: нужен MIDI; не выбран ли GND-GND? Подними LEV/VOL, проверь BASE/WDTH.
- Нет звука FX: проверь входной аудиосигнал; выключи MIDI gate либо подай MIDI.
- Нота затухает при удержании: это AHD, поставь HOLD=127.
- Не слышны LFO: DPTH=0, выключенный маршрут, неподходящий DEST или TRIG/ONE/HOLD без MIDI-триггера.
- Не меняется темп: включён host tempo; меняй BPM DAW либо выключи Host Sync.

Если сборка падает, пришли **первую ошибку Build Output**, ОС, версию JUCE и выбранный exporter.
