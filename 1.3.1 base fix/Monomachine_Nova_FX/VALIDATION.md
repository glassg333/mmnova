# Проверка v1.2.0 / FX

## Выполнено

- JUCE 8.0.9, Linux x86_64, GCC 14.2.0, CMake/Ninja.
- Release `-O1 -DNDEBUG`, последовательная сборка. Успешны обе цели VST3 и создание moduleinfo.json.
- Начальная тишина / MIDI sample offsets / Note Off / sustain / last-note priority у Synth.
- Все 15 Synth-режимов (GND намеренно тихий) и все 7 FX-режимов проверены на выходной сигнал.
- FX gate и прозрачный global dry path, Panic.
- Каждая машина на 44.1/48/96/192 кГц, блоки 0/1/17/32/257/4096, минимумы/максимумы SYNTHESIS-параметров.
- Проверка конечности выходных сэмплов; mono/stereo на детерминированном сигнале.
- Арпеджиатор: события/gate на 120 BPM в KEY/SID/ADD; LFO: все пять форм.
- Матрица: unit-проверка и проверка изменения аудио включённым маршрутом.
- Сохранение/восстановление параметров; у Synth загрузка WAV, round trip встроенного PCM/state и playback после удаления файла.
- Создание/рендеринг/масштабирование GUI в Xvfb: Preview.png.

Результат:

```text
Parameters: 130
FX-THRU peak=0.218125
FX-REVERB peak=0.19924
FX-CHORUS peak=0.19787
FX-DYNAMIX peak=0.215934
FX-RINGMOD peak=0.196817
FX-PHASER peak=0.108901
FX-FLANGER peak=0.103893
FX: PASS (520937 assertions including per-sample finite checks)
```

Счётчик — количество assertions, включая проверку каждого сэмпла. Это НЕ число независимых сценариев/музыкальных тестов.
В тестовом WAV writer JUCE выдаёт предупреждение о deprecated overload; это не ошибка сборки плагина.

## Не выполнено

- Windows/MSVC и macOS/Xcode сборка; реальное сохранение exporter через GUI Projucer.
- Загрузка в конкретную DAW, pluginval/Steinberg validator, signing/notarization.
- Длительный realtime soak test, тест всех сочетаний модуляций/автоматизации.
- Сравнение с аппаратной Monomachine и подтверждение точности эмуляции.
- GitHub Actions workflow вложен, но не запускался.

Защита от NaN на выходе не является лимитером и не доказывает устойчивость всех комбинаций DSP.
В архив входят исходники; Linux-бинарники тестовой сборки не включены и всё равно не подошли бы для Windows/macOS.
