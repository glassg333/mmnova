# MonoJuice 0.8.0 — сборка (RU)

Этот пак содержит все авторские исходники с фиксом CHORUS MIX от 17.09.2026
(см. `CHANGELOG_RU.md`). Папки `JuceLibraryCode/` и `Builds/` в пак intentionally
отсутствуют: их генерирует Projucer из `MonoJuice.jucer` за один клик (см. ниже).
Бинарники, SDK и прошивка в пак не входят.

Проверенные конфигурации авторов: Linux / GCC / JUCE 7.0.10 (VST3 + Standalone),
экспорт Projucer 8.0.6 в VS2019/VS2022. MSVC-сборка и конкретные DAW авторами
не проверялись — собираете на свой страх, начинайте с Debug.

## Требования

- JUCE 7.0.10 (исходники, каталог `modules/`), Projucer (7 или 8);
- Windows: Visual Studio 2019 или 2022 (x64), CMake 3.22+ (для CMake-пути);
- Linux: GCC, CMake 3.22+, dev-пакеты ALSA/X11 (обычный набор для JUCE);
- ~2 ГБ свободного места под JUCE + артефакты сборки.

## Путь 1. Projucer + Visual Studio (Windows, основной)

1. Откройте `MonoJuice.jucer` в Projucer.
2. Если ваш JUCE лежит не в `F:/JUCE/modules` (путь захардкожен в `.jucer`
   с машины автора): в Projucer задайте путь к модулям
   (Config → Global Settings → JUCE Modules / либо поле модуля в проекте)
   и нажмите **Save** — Projucer перегенерирует `JuceLibraryCode/` и
   `Builds/VisualStudio20XX/` под вашу машину.
3. Откройте `Builds/VisualStudio2022/MonoJuice.sln` (или 2019).
4. Конфигурация **Release / x64**, собрать проекты `MonoJuice_VST3`
   (и `MonoJuice_StandalonePlugin` при желании).
5. Готовый `.vst3` лежит в `Builds/VisualStudio2022/x64/Release/VST3/`.
   Перед заменой плагина в DAW сохраните backup проекта; не держите разные
   версии одного plugin ID (`MnOn/NvF5`) одновременно в scan path.

## Путь 2. Projucer + Linux Makefile

1. Откройте `MonoJuice.jucer` в Projucer, укажите путь к JUCE, **Save**.
2. `make -C Builds/LinuxMakefile CONFIG=Release -j$(nproc)`

## Путь 3. CMake (Linux/Windows, без Projucer)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DJUCE_PATH=/путь/к/JUCE
cmake --build build --config Release -j
ctest --test-dir build --output-on-failure
```

Только тесты без JUCE (быстрая проверка пака):

```bash
cmake -B build-tests -DNOVA_BUILD_PLUGIN=OFF
cmake --build build-tests -j
ctest --test-dir build-tests --output-on-failure
```

Ожидается: `OriginalChorusGolden` (хеш `7007ca4d75708b0f`), `FXChain`,
`FX5Rack`, `MonoJuiceFactory` — все PASS. `JUCEWrapper` требует JUCE и
собирается только с плагином.

## Проверка целостности пака

```bash
python3 -c "
import json,hashlib,pathlib
m=json.load(open('MANIFEST_SHA256.json'))
bad=[n for n,e in m.items() if hashlib.sha256(pathlib.Path(n).read_bytes()).hexdigest()!=e]
print('OK' if not bad else f'BAD: {bad}')"
```

## Что изменилось относительно исходного дерева 0.8.0

База — фикс MIX (как в прошлом паке):

- `Source/dsp/AudioAdapter.h/.cpp` — сглаживание 8 параметров (~20 мс);
- `Source/dsp/Rack.h` — MIX хоруса вынесен наружу ядра;
- `Source/PluginEditor.cpp` — тултип MIX.

Новый слой — режимы v1/v2 + LFO + SYNC:

- `Source/dsp/Rack.h` — `C_MODE` (V1/V2) на слот, default V1 (= поведение
  до фикса MIX); LFO на слот (65 глубин + RATE/SHAPE/SYNC/DIV); `setTempo`;
- `Source/dsp/AudioAdapter.h/.cpp` — режим direct/smooth + float-путь
  параметров (V1 — старый код побитово, V2 — гладкий глайд);
- `Source/dsp/ChorusCore.h/.cpp` — аддитивный `setParametersF`
  (целочисленный путь и процедура не тронуты, золотой хеш цел);
- `Source/dsp/ParallelCombModel.h` — защитные клампы + фикс деления
  на ноль при K_LIM=0 (был липкий NaN-mute);
- `Source/Parameters.h` — 65 глубин `D_…` + LFO 130–133 + ключ `C_MODE`
  (всего 739 параметров);
- `Source/PluginProcessor.h/.cpp` — layout/fetch/settings, темп из плейхеда;
- `Source/PluginEditor.h/.cpp` — комбо C_MODE, LFO-страницы после базовых,
  индикатор PAGE (старые страницы/скриншоты не сдвинуты);
- `tests/RackTests.cpp`, `tests/WrapperTests.cpp` — новые ассерты (v1/v2,
  матрица, sync, экстримы; 739 параметров, 671 биндинг, таблица страниц);
- `README_RU.md`, `CHANGELOG_RU.md` — описание режимов и LFO;
- `TestsResults/unchanged-chorus.json`, `TestsResults/source-audit.json` —
  переподписаны пины (`ChorusTables.h` и остальные пины не изменились);
- `TestsResults/lfo-modes-0.8.0.log` — протокол проверки (4 сьюта + 30
  LFO-ассертов + побитовые батареи V1==pre-fix, V2==mixfix);
- `BUILD_RU.md` (этот файл).

Флаги точности сохранены: MSVC `/fp:precise`, остальные
`-fno-fast-math -ffp-contract=off` (см. `.jucer` и `CMakeLists.txt`).
