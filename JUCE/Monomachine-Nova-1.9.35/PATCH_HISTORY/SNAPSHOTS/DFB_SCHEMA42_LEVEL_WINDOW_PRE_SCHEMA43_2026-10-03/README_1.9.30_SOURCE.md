# Monomachine Nova 1.9.30 — актуальные исходники (source-only)

Дата актуальной source revision: 03.10.2026.

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

- Schema поднята до **42**: прежние FM-normalization rules и CHOR A/B
  сохранены; неработающий DLY CORE снят из поставляемых исходников, а DFB GUARD LVL
  остаётся настраиваемым окном уровня.
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
- `Source/dsp/DelayFeedbackDynamics.hpp`, `Source/dsp/mnm/MnmDelay.hpp`
  и `Source/dsp/mnm/MnmTrackDelayNew.hpp`;
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

## Track Delay: BASE + GUARD LVL и отдельный CORE A/B

Старый capture/HOLD вариант остаётся сохранённым в
`PATCH_HISTORY/SNAPSHOTS/DFB_CAPTURE_HOLD_WITHDRAWN_1.9.30/`; он не является
текущей спецификацией.

Schema **41** оставляет активную DFB-панель P1/P2 с явной BASE, но заменяет
неудобный raw-threshold GUARD на level-window design:

- верхняя BASE-линия при включённом `DYNAMICS` всегда строгая `RAW / 64`, с
  unity ровно на raw `64`;
- `GUARD LVL START` — начало давления по наблюдаемому уровню петли;
  `LVL END / PLT` использует сохранённый `PLATEAU` как конец окна;
  `GUARD CURVE` и `GUARD AMOUNT` задают форму и силу внутри этого окна;
- reference defaults: LVL START `.55`, LVL END/PLATEAU `.73`, CURVE `1`,
  AMOUNT `1`, ATTACK `37 ms`, RELEASE `184 ms`;
- ниже LVL START GUARD не давит; при raw `64` и ниже BASE не уменьшается
  GUARD-ом. После выбранного END сила не продолжает самопроизвольно расти с
  уровнем хвоста;
- DFB открывается в компактном отдельном OS-window: оно не обрезается рамкой
  VST и перемещается за пределы редактора. Внутри остаются оба live graph,
  все toggles и шесть узких controls; текст панели белый;
- `DYNAMICS`, оба `DSND ± FB INVERT`, `RETURN COMP` и `FB CLIP / GUARD`
  стартуют включёнными согласно выбранному reference state.

Старые schema-39 anchors и schema-40 `dfb_guard_raw` остаются в APVTS/state
для безопасной загрузки старых проектов, но не участвуют в active GUARD LVL
law и не показываются в окне. При migration schema `<41` добавляется LVL START;
прежний raw-curve переводится в нейтральный level CURVE `1`, тогда как
сохранённые `PLATEAU`/`ATTACK`/`RELEASE`/AMOUNT не переписываются.

`DLY` в DSP menu снова содержит только `mnm | old | new`. Неработающий `core`
снят из UI, runtime, state owner, tests и shipping Source. При загрузке state
schema 40/41 сохранённое бывшее значение `core=3` явно мигрирует в `new=2`;
прочие ошибочные DLY values нормализуются в `mnm`. Никакая hardware-exact claim
для DFB или Track Delay из этого не следует.

DBAS/DWID сохраняют текущий 12 dB feedback-filter response; selector slope не
добавлялся. `FxSlotEngine` по-прежнему не получил speculative DFB routing:
`DFB_ROUTE_UNRESOLVED` остаётся границей текущей реализации.

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

Для актуальной source-only ревизии используется только локальный static audit:

```bash
python DFB_GUARD_CORE_STATIC_CHECK.py
python FM_MODE_CLEANUP_STATIC_CHECK.py
```

Первый проверяет schema 42, DFB BASE/GUARD LVL parameter/runtime/live-graph
связку, отдельное компактное окно и отсутствие удалённого DLY CORE. Второй сохраняет расширенный audit
retained FM/CHOR/Phaser и source metadata. Оба проверяют reference defaults,
byte-identical Synth/FX mirrors и отсутствие speculative `FxSlotEngine` DFB
route; также они проверяют target pairing `Synth:NOVA_SYNTH=1` /
`FX:NOVA_SYNTH=0`. Ни один не компилирует и не исполняет DSP. Результат
актуального запуска записан отдельно от сохранённого старого журнала в
`VALIDATION_STATIC_AUDIT_2026-10-03_DLY_CORE_REMOVED.md`.
