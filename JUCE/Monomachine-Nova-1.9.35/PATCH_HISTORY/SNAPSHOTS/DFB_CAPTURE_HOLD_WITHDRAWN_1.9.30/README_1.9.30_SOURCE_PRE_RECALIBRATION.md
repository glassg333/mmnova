# Monomachine Nova 1.9.30 — актуальные исходники (source-only)

Дата: 01.10.2026.

Это добавочная исходная поставка для двух веток:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Предыдущие source archives 1.9.21 и 1.9.27 сохранены без удаления.
Полные активные README и validation log релиза 1.9.27 также сохранены без
сокращения в `PATCH_HISTORY/SNAPSHOTS/RELEASE_1.9.27_SOURCE_ONLY/`; текущий
source archive не перезаписывает исторические материалы.

## Что изменено в MODE SYNT

У FM+ `STAT` (m8), `PAR` (m9) и `DYN` (m10) осталось ровно шесть selectable
режимов в этом порядке:

| ID renderer | Новая подпись |
|---:|---|
| 0 | `mnm frq` |
| 1 | `old frq` |
| 2 | `new frq` |
| 3 | `mnm bpm` |
| 4 | `new bpm` |
| 5 | `old bpm` |

Числовые renderer IDs и DSP mappings этих шести путей не переставлялись. В
частности, `new bpm` остаётся ID 4, а `old bpm` — ID 5.

Из пользовательского MODE SYNT удалены experimental slots:

- `mnm frq env fix` (бывший raw ID 6);
- `try4` (бывший raw ID 7);
- `fix5 pitch+env full` (бывший raw ID 8).

Полные исходники импортированных экспериментов, candidate-only tests и fixture
сохранены с SHA-256 в `PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`, а из
shipping Source удалены. Они не попадают в choices, UI/state allow-list,
processor-owned runtime route или compilation graph.

## Состояние и защита от неверного выбора renderer

- Schema поднята до **38**: прежние FM-normalization rules и CHOR A/B
  сохранены; добавлен serializable DFB dynamics state с default `ON`.
- При загрузке старого state raw ID 6, 7 или 8 нормализуется в `mnm frq` (ID 0).
  Он не ограничивается до ID 5, поэтому не происходит скрытого перехода на
  `old bpm`.
- Та же защита есть в processor snapshot и в границе `MachineEngine::setModes`.
- Обычный mode parameter FM+ теперь имеет диапазон `0..5` и шесть labels.

## Изменённые рабочие исходники

В обеих ветках синхронно обновлены:

- `Source/models/DspModes.hpp`, `Source/NovaData.h`;
- `Source/PluginProcessor.{h,cpp}`;
- `Source/PluginEditor.cpp`;
- `Source/NovaDSP.h`;
- `Source/dsp/TrackVOLPAN.inl`, `Source/dsp/TrackDelay.inl`;
- `Source/dsp/DelayFeedbackDynamics.hpp`, `Source/dsp/mnm/MnmDelay.hpp` и
  `Source/dsp/mnm/MnmTrackDelayNew.hpp`;
- `Source/dsp/AudioAdapter.{h,cpp}` и `Source/FxSlotEngine.h`;
- `CMakeLists.txt`, `tests/FmModeRetirementTests.cpp` и обновлённые source-only
  проверки `tests/DelayFeedbackDspTests.cpp` / `tests/FmFixModeTests.cpp`.

Сняты candidate-only imports, пустой FM STAT tombstone, profile header, CMake
registrations, tests и fixture; точные версии до снятия лежат в
`PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`. Active m9 PAR unity readout
сохранён без candidate profile header. Указанные общие файлы и retained-mode
тесты Synth/FX проверяются на byte-identical mirror. Их ранее сохранённые
registry test sources также остаются в
`PATCH_HISTORY/SNAPSHOTS/FM_MODE_TESTS_PRE_CLEANUP_2026-09-30/`.

## Phaser

Для FX machine ID 18 в `Source/dsp/monomachine_effects.hpp` full-wet output
заменён с unity-gain all-pass image на bounded phase-difference:
`0.5f * (allpassCascade - input)`. Старая формула сохранена закомментированной
сразу рядом с новой. Это целевая практическая коррекция без заявления о
доказанном hardware-эквиваленте; machine ID, шесть передаваемых ручек и routing
не менялись.

## Chorus: реальный A/B runtime и INP

В DSP-меню `CHOR` теперь есть два **реальных** значения параметра `mode_cho`:

| index | подпись | вызываемое ядро |
|---:|---|---|
| 0 | `native` — default | `NativeChorusCore` через `AudioAdapter` |
| 1 | `core` — A/B | эталонный `ChorusCore` через тот же `AudioAdapter` |

Переключатель доходит до обычного P1, P2 и уже созданных пользовательских
FX-slots. Он не меняет host `INP`, dry/wet/MIX, gain, latency или routing.
При смене `native`/`core` `AudioAdapter` сознательно сбрасывает audio/tail state
обоих ядер: совместимый перенос X/Y/delay-memory не выдумывается. Поэтому A/B
начинается с чистого состояния, а Native остаётся и factory/default, и путём
для всех старых state (schema `<37` принудительно мигрирует в `native`).

`CHORUS SAFE` не выбирает ядро: при default OFF работает выбранное ядро и
сохраняет tail при machine `MIX=0`; SAFE=ON только обходит/сбрасывает это же
выбранное runtime-ядро при `MIX=0`.

По пользовательскому сравнению только host-участок `INP −64..−60` получил
квадратичный ease-in: −64 остаётся нулём, −60 и все значения выше сохраняют
прежний коэффициент `raw/64`. Меняются только −63/−62/−61; это не изменение
native 24-bit core и не заявление о hardware-формуле.

## Track Delay: DFB dynamics и экспериментальная polarity

`DFB DYNAMICS` действует в уже существующем TrackChain пути всех трёх Track
Delay cores (`OLD`, `MNM`, `NEW`) и **включена по умолчанию**, включая migration
старых state. Это один stateful controller active feedback, а не фиксированная
формула для каждого raw-значения:

| DFB raw | Поведение DFB DYNAMICS=ON |
|---:|---|
| после `prepare/reset` | active feedback стартует с `1.0` — это только начальное состояние controller |
| `65..127` | один active feedback плавно заряжается вверх; максимум bounded коэффициента — `1.125` |
| `63` | тот же active feedback плавно и наиболее мягко разряжается вниз |
| `0..62` | тот же state разряжается сильнее; `0` использует полный smooth `ZERO TAIL` release |
| `64` | настоящий точный `HOLD`: state не меняется и не назначается в `1.0`; возврат с `63` фиксирует уже слегка просевшее значение |

`RISE` (default `1.20 s`) задаёт скорость заряда: при raw `127` это время
движения от unity к bounded максимуму, а меньшие значения `65..126` движутся
пропорционально медленнее. `ZERO TAIL` (default `0.65 s`) задаёт плавный
нисходящий ход к нулю; raw `0` не выдаёт clear. `64` после любого времени на
`65+` или `63−` bit-identical удерживает достигнутый controller state.

Это автономный закон **только feedback controller**. Короткому DFB-жесту
не приписывается отдельный guessed reset/pulse law.

ПКМ по ручке `DFB` открывает одно мини-окно: `DFB DYNAMICS`, `RISE`, `ZERO
TAIL`, прежние `RETURN COMP`/`FB CLIP` и два default-OFF experimental switches:
`DSND + FB INVERT` и `DSND - FB INVERT`. Они меняют только знак recurrence,
не DSND mono/split send route. `DYNAMICS=OFF` оставляет отдельный A/B путь с
прежней прямой law `raw / 63`.

`DBAS`/`DWID` и выбранная текущая feedback-filter response **не менялись**:
остаётся каскад двух двухполюсных Korg-35 edge stages (nominally 12 dB на
каждую границу); 12/24 selector не добавлен. Новый controller не является
panic-clear, automatic RMS normaliser или обязательным clipper: `FB CLIP`
по-прежнему отдельная явная защита в том же RMB-окне.

`FxSlotEngine` не получил speculative DFB wiring: у custom FX slots нет
подтверждённого route к TrackChain delay controller. Это явно отмечено в
исходнике как `DFB_ROUTE_UNRESOLVED`; данный релиз не выдаёт custom-slot DFB
за реализованную функцию.

## ARP SONG: активное окно и рисование

`S START`/`S END` теперь задают не только playback range: в поле SONG
показываются **только** активные позиции этого диапазона. Они динамически
занимают ширину строки тем же образом, что и обычные ARP step rows; скрытые
SONG positions не удаляются и не переписываются.

ЛКМ-перетаскивание по строке `S-PART` или `REPEAT` работает кистью: значение рисуется
через все активные позиции, которые пересекает курсор, включая пропущенные при
быстром горизонтальном движении. Для каждой затронутой позиции открывается и
закрывается собственный жест хоста. ПКМ по одной ячейке, как прежде, возвращает
только эту позицию к её заданному default.

## Исправление MSVC C2397

В `Source/NovaData.h` для SONG part page default добавлен явный
`static_cast<float>(part+1)`. Это исправляет MSVC C2397: переменный `int`
в list-initializer `Spec::def` нельзя неявно сужать до `float`.

## Самостоятельная сборка

Нужен отдельный checkout JUCE 8. Пример для каждой ветки (подставьте абсолютный
путь к своему JUCE):

```bash
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/абсолютный/путь/к/JUCE
cmake --build build-synth --config Release

cmake -S Monomachine_Nova_FX -B build-fx -DJUCE_DIR=/абсолютный/путь/к/JUCE
cmake --build build-fx --config Release
```

`NOVA_BUILD_TESTS` по умолчанию выключен. В этой поставке не заявляется
результат полной сборки, VST3/DAW-проверки, render или выполнения тестов:
пользовательская сборка остаётся следующим gate.

## Статическая проверка этой поставки

Для 1.9.30 используется только локальный source-only аудит:

```bash
python FM_MODE_CLEANUP_STATIC_CHECK.py
```

Он проверяет retained FM contract, schema 38, реальный CHOR Native/Core route,
stateful DFB law `65+ / 63− / 64 HOLD / 0 tail`, default-ON RMB family,
byte-identical Synth/FX mirrors, явную границу `DFB_ROUTE_UNRESOLVED` для
`FxSlotEngine`, ARP SONG и хэши `SOURCE_BUILD.json`. Он не компилирует и не
исполняет DSP. Фактический результат и hashes этой поставки записаны в
`VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt`.
