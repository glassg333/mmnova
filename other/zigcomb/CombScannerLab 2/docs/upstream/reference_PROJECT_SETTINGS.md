# Настройки JUCE / Visual Studio

Из репозитория Monomachine Nova использовались только `.jucer`-файлы двух подпроектов и настройки JUCE/Visual Studio. Код, DSP/GUI-архитектура и другие части репозитория не изучались и не переносились.

## Параметры `CombScannerPro.jucer`

- Projucer audio-plugin project; C++17; формат VST3; MIDI input включён.
- Exporter: Visual Studio 2022 (`Builds/VisualStudio2022`). Solution собирается MSBuild Visual Studio 18 / 2026, Release x64.
- Конфигурации Debug и Release: x64; оптимизация 1 и 3 соответственно; LTO отключён; MSVC flag `/utf-8`.
- Подключено 13 JUCE-модулей, включая `juce_audio_processors_headless`, присутствующий в обоих эталонных `.jucer`-файлах. Модули берутся из глобальной установки JUCE; каталог JUCE не дублируется в проекте. Для headless-модуля явно задан путь `C:/JUCE/modules`.
- `tools/BuildVST3.bat` запускает Projucer с `--resave --fix-missing-dependencies` при первом создании solution или если `.jucer` изменился с момента последнего генератора. При повторной сборке только C++-кода solution не пересоздаётся; `-ForceResave` принудительно запускает Projucer. Плагин собирается MSBuild в выбранной конфигурации и x64.
- Плагин не копируется автоматически в системную VST3-папку. При переносе нужно сохранить весь каталог `CombScannerPro.vst3`.

## Сверка с эталоном

Оба изученных проекта Monomachine Nova используют `juce_audio_processors_headless`, глобальные JUCE modules и Visual Studio 2022 exporter с x64-конфигурациями. Локальный `.jucer` приведён к этим настройкам для модулей и Windows-exporter. Названия и значения, специфичные для самого Comb Scanner, сохранены.

## Статус проверки

Локально проверена целостность XML и согласованность настроек `.jucer`; Windows-сборка через VS 2026 и проверка плагина в DAW не запускались и не подтверждены.
