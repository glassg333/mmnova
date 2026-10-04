# Monomachine Nova 1.9.30 — карта исходников и безопасный план структуры

Дата актуальной package revision: 03.10.2026; последний source-only audit: 03.10.2026. Карта фиксирует структуру после снятия
retired FM candidates, документированной коррекции Phaser и DFB BASE/GUARD law. Полная сборка,
DAW, render и executable tests не запускались.

## Что уже убрано из корня

Исторические Oracle/FM/filter/routing audits, handoff, отозванный Track Delay
hotfix, старые validation logs и каталог `oracle/` перенесены **без изменения
текста** в:

`PATCH_HISTORY/SNAPSHOTS/ROOT_AUDITS_AND_HANDOFFS_PRE_1.9.26/`

В корне оставлены только действующие Markdown-документы:

- `README_FIRST.md`;
- `README_1.9.30_SOURCE.md`;
- `TRACK_DELAY_CORRECTION_2026-09-30.md`;
- `VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt`;
- этот map.

## Главные точки входа

| Задача | Единственное место, с которого начинать чтение |
|---|---|
| Реестр MODE и допустимые raw IDs | `Source/models/DspModes.hpp` |
| Параметры машин, названия ручек и их порядок | `Source/models/machine_definitions.hpp` |
| Регистрация APVTS параметров | `Source/NovaData.h` |
| UI и меню MODE | `Source/PluginEditor.cpp` |
| Загрузка state, migration и P1/P2/FX-slot routing | `Source/PluginProcessor.cpp` |
| Реальный dispatch аудио и FX machine IDs | `Source/NovaDSP.h`, класс `MachineEngine` |
| Цепь track DSP | `Source/dsp/Track*.inl` |

Ключевые Track Delay/DFB sources и retained-mode tests зеркальны между
`Monomachine_Nova_Synth` и `Monomachine_Nova_FX`; это проверяет статический
аудит. Target-specific metadata и отдельные target files не объявляются
byte-identical. Любое изменение общего TrackChain пути надо выполнять зеркально.

## Публичный FM MODE SYNT: только шесть путей

| Raw ID / UI label | STAT m8 | PAR m9 | DYN m10 | Рабочие файлы |
|---:|---|---|---|---|
| 0 `mnm frq` | `mnm::FmCore` | `mnm::FmCore` | `mnm::FmCore` | `dsp/mnm/MnmFm.hpp` и его зависимости |
| 1 `old frq` | retained MNM alias | `MonomachineFmParallel` | `MonomachineFmDynamic` | `dsp/monomachine_fm_par.hpp`, `dsp/monomachine_fm_dynamic.hpp` |
| 2 `new frq` | `fm_new::FmExactCore` | то же family | то же family | `dsp/fm_new/` |
| 3 `mnm bpm` | `fm_fix::MnmFixCore` | то же family | то же family | `dsp/fm_fix/MnmFmFix.hpp` |
| 4 `new bpm` | `fm_new::FmExactFixCore` | то же family | то же family | `dsp/fm_new_fix/` |
| 5 `old bpm` | `OldFixStaticCore` | `OldFixParallelCore` | `OldFixDynamicCore` | `dsp/fm_fix/OldFmFix.hpp` |

`DspModes.hpp` публикует IDs `0..5`; `MachineEngine::setModes()` повторно
отсекает всё вне этого диапазона. `PluginProcessor.cpp` также нормализует
сохранённые retired raw IDs в `mnm frq`.

### Снятые FM candidates и сохранённая история

| Бывший raw ID | Бывший каталог | Статус в 1.9.30 |
|---:|---|---|
| 6 | `dsp/fm_mnm_frq_env_fix/` | Снят из shipping Source. |
| 7 | `dsp/fm_try4_voice/` | Снят из shipping Source. |
| 8 | `dsp/fm_fix5/` | Снят из shipping Source. |

До удаления были сняты `#include`, state, reset/pitch/note/render dispatch,
processor/editor hooks, candidate-only AMP bypass, CMake registrations, tests
и fixture. Полные исходные пути обеих целей, включая исторические тесты,
сохранены с SHA-256 в
`PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`. В shipping Source нет renderer
или runtime ветки этих трёх slots; raw IDs `6..8` мигрируют в `mnm frq`.

Пустой `dsp/monomachine_fm_stat.hpp` tombstone и candidate-only
`models/FmExperimentProfiles.hpp` сняты и лежат в том же архиве. Активный m9
PAR unity readout сохранён локально в `PluginEditor.cpp`, без зависимости от
candidate profile header.

## FX машины: где реально находится звук

| Machine ID | Машина | Параметры задаёт | Реальный runtime код |
|---:|---|---|---|
| 12 | FX-THRU | `machine_definitions.hpp` | общий input gain в `MachineEngine::renderFX()` |
| 13 | REVERB | тот же descriptor | `juce::Reverb reverb` и HP/LP/gate/mix код прямо в `NovaDSP.h` |
| 15 | CHORUS | тот же descriptor | `AudioAdapter` → выбранный `NativeChorusCore` (default) или `ChorusCore` (A/B) |
| 16 | DYNAMIX | тот же descriptor | compressor/parallel mix прямо в `NovaDSP.h` |
| 17 | RINGMOD | тот же descriptor | `MonomachineRingMod` в `dsp/monomachine_effects.hpp` |
| 18 | PHASER | тот же descriptor | `MonomachinePhaser` в `dsp/monomachine_effects.hpp` |
| 19 | FLANGER | тот же descriptor | `MonomachineFlanger` в `dsp/monomachine_effects.hpp` |

`FxSlotEngine.h` не реализует отдельный звук: он создаёт обычный
`MachineEngine` и вызывает тот же `render(..., false)`, что используется
нативным FX/P2 путём. Поэтому перенос реализации эффекта в новый файл не
должен менять IDs, восемь ручек или route через FX slots.

Это описание относится только к FX machines. Оно **не** подтверждает TrackChain
DFB route для custom slots: в `FxSlotEngine.h` явно стоит
`DFB_ROUTE_UNRESOLVED`, а guessed DFB setter/parameter map намеренно отсутствует.

### Reverb

Отдельного файла reverb нет, потому что ядро — готовый `juce::Reverb`.
Обвязка Monomachine (HP, LP, gate и MIX) находится в `NovaDSP.h`.
Для хорошей структуры её можно позднее вынести в `dsp/effects/reverb/`, но
это должно быть только извлечение тех же строк без смены коэффициентов.

### Три реализации CHORUS: два реальных A/B runtime и prototype

| Файл / класс | Роль | Используется в обычном runtime? |
|---|---|---|
| `ChorusCore.{h,cpp}` / `nova::ChorusCore` | reference/audit core с X/Y/delay memory image | **Да**, при `DSP → CHOR → core` (`mode_cho=1`). |
| `ChorusNative.{h,cpp}` / `nova::NativeChorusCore` | основное runtime-ядро: статически переведённая процедура, inline memory access и quiet-path | **Да**, default `DSP → CHOR → native` (`mode_cho=0`). |
| `monomachine_chorus.hpp` / `monomachine::MonomachineChorus` | отдельный float delay-LFO prototype | Нет: в репозитории нет call site. |

`AudioAdapter.h` теперь держит оба первых core и реально вызывает выбранный:
`NativeChorusCore` при `mode_cho=0`, `ChorusCore` при `mode_cho=1`. Общими
остаются host↔44,1-kHz conversion, 16-frame scheduling, fixed-point bridge,
latency-aligned dry/wet, host INP knee и MIX routing. Runtime selector приходит
в P1, P2 и существующие пользовательские FX slots через `MachineEngine`.

При смене `native`/`core` адаптер сбрасывает audio/tail state обоих ядер. Это
сознательная безопасная граница A/B: X/Y/delay-memory transfer между двумя
независимыми реализациями не выдуман. Поэтому переключение не сохраняет хвост,
но не меняет gain/topology и не возвращает неиспользуемый float prototype.
Рабочая цепь по умолчанию остаётся
`MachineEngine → AudioAdapter → NativeChorusCore`; для `core` финальный узел
меняется только на `ChorusCore`.

`CHORUS SAFE` не выбирает core. При default OFF выбранное ядро продолжает
работать и при `MIX=0`, сохраняя свой tail/state. SAFE=ON только сбрасывает и
обходит выбранное runtime-ядро при `MIX=0` для проверки, поэтому не является
оригинальным режимом.

### CHORUS INP: локальная низкоуровневая кривая

`INP` — коэффициент host-обвязки до `AudioAdapter`, а не параметр, передаваемый
в native core: core принудительно получает unity INP=64 и full internal MIX=127,
после чего host смешивает latency-aligned dry/wet. Поэтому наблюдаемая кривая
одинаковая для обоих путей/частот host и не доказывает отдельную формулу OS core.

По пользовательскому сравнению изменён только участок UI `INP −64..−60`
(raw `0..4`): применён ограниченный квадратичный ease-in. Концы сохранены точно,
и `INP −60` и все значения выше не менялись:

| UI INP | raw | host gain |
|---:|---:|---:|
| −64 | 0 | 0 |
| −63 | 1 | 0.00390625 |
| −62 | 2 | 0.015625 |
| −61 | 3 | 0.03515625 |
| −60 | 4 | 0.0625; далее прежний `raw / 64` |

Это сознательная локальная плавность host fade, а не заявление о найденной
оригинальной аппаратной transfer function.

`monomachine_voice_chain.hpp` и `monomachine_chorus.hpp` сохраняются в
shipping Source. Они не являются третьим CHORUS mode и не участвуют в выбранном
Native/Core runtime, однако их удаление, архивирование или объявление готовыми
к снятию не согласованы и в 1.9.30 не выполняются.

## Track Delay: strict BASE и RAW-GATED GUARD — реальный runtime путь

`Source/dsp/DelayFeedbackDynamics.hpp` — отдельный controller перед recurrence
всех `TrackChain` delay modes. Capture/HOLD logic и schema-42 level-window
описание сохранены в
`PATCH_HISTORY/SNAPSHOTS/DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/`
и не активны.

- Schema 43: при `DYNAMICS=ON` BASE строго `RAW / 64`; raw `64` — единственная
  unity точка. RAW `0..63` — true GUARD bypass, поэтому low DFB не переносит
  pressure/level защиты.
- В узком RAW `63..64` follower мягко armed; это не меняет BASE на raw `64`.
  На активном участке RAW `64..127` runtime линейно связывает four anchors:
  `START @64`, `PLT @64`, `START @127`, `PLT @127`.
- `GUARD OFFSET` сдвигает обе границы сразу; `GUARD CURVE`, `GUARD AMOUNT`,
  `ATTACK`, `RELEASE` сохраняются P1/P2-независимо. Reference defaults:
  `.55/.73` на обоих endpoints, OFFSET `0`, CURVE/AMOUNT `1`, attack `37 ms`,
  release `184 ms`; все пять toggles ON.
- RMB по `DFB` открывает white compact child overlay внутри Surface/VST, а не
  отдельное окно ОС. Overlay не крадёт фокус DAW, закрывается повторным RMB или
  `X` и перемещается за header в рамках editor. В нём остаются оба live graph,
  live marker, five toggles и nine controls.
- Старые anchor/tail IDs и schema-40 `dfb_guard_raw` не удалены из APVTS/state,
  но скрыты из active UI. При schema `<43` migration переносит saved START и
  PLATEAU в оба endpoint, а OFFSET добавляет как `0`; существующие state values
  не переписываются.
- Schema 42 сняла неработающий DLY CORE из menu/runtime/source. Остаются
  selector `mnm|old|new`; saved CORE value `3` из schema 40/41 при загрузке
  явно переходит в `new=2`, не выбирая случайный соседний режим.
- DBAS/DWID feedback filter и его текущая 12 dB response не изменялись;
  selector slope не добавлен. Custom `FxSlotEngine` DFB route не реализован:
  см. `DFB_ROUTE_UNRESOLVED` выше.

## ARP SONG: активное окно UI

`SongLane` в `PluginEditor.cpp` хранит все 64 сериализованные позиции, но видит
и редактирует только `S START..S END`. Геометрия активного окна растягивается
динамически как у обычных ARP step rows. Горизонтальный ЛКМ-drag рисует
`S-PART` или `REPEAT` через каждую пройденную активную позицию (с заполнением
интервала между mouse events); ПКМ возвращает default только выбранной позиции. Это UI
поведение не меняет SONG transport, диапазон параметров или schema.

## Phaser: что подтверждено статически

Для ID 18 `MachineEngine::set()` передаёт `CNTR, DEP, SPD, MIX, FB, WID` в
`MonomachinePhaser::setParameters()`, а `renderFX()` вызывает
`phaser.processStereo(l, r, l, r, n)` ровно один раз.

Старая формула оставлена комментарием непосредственно рядом с заменой:

```cpp
// out = (1.0f - wet) * input + wet * allpassCascade;
phaseDifference = 0.5f * (allpassCascade - input);
out = (1.0f - wet) * input + wet * phaseDifference;
```

При `MIX=0` это остаётся точным bypass. При `MIX=127` теперь выходит
ограниченная разность all-pass каскада и входа: прямой компонент вычтен явно,
поэтому эффект не сводится к unity-gain фазовой копии входа. Коэффициент `0.5`
ограничивает номинальную разность. Это документированная практическая
коррекция, а не заявление о доказанной аппаратной формуле Monomachine.
Отдельного параллельного host dry маршрута в этом месте статически нет.

## Что активно и что нельзя переносить

- `dsp/hybrid_private/` активно используется фильтром, DIST, processor
  migration и UI; это **не** legacy folder.
- `dsp/mnm/`, `dsp/fm_fix/`, `dsp/fm_new/`, `dsp/fm_new_fix/`,
  `monomachine_fm_par.hpp`, `monomachine_fm_dynamic.hpp` обслуживают retained
  public paths и остаются в shipping Source.
- Пустой `dsp/monomachine_fm_stat.hpp` tombstone уже снят из shipping Source
  и сохранён в `PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`.
- `dsp/monomachine_voice_chain.hpp` и `dsp/monomachine_chorus.hpp` сохраняются
  без удаления или архивирования; никакого согласованного решения об их снятии нет.

## Рекомендуемая будущая структура

Не использовать `chorus1/2/3`: номера скрывают смысл. Лучше роли:

```text
Source/dsp/
  effects/
    chorus/
      runtime/       AudioAdapter + NativeChorusCore (default) | ChorusCore (A/B)
      reference/     tables/audit provenance for ChorusCore
      preserved/     float prototype; удаление/архивирование не согласованы
    modulation/
      Phaser.hpp
      Flanger.hpp
    ringmod/
      RingMod.hpp
    reverb/          JUCE wrapper + existing Monomachine HP/LP/gate glue
    dynamics/        DYNAMIX wrapper
  fm/
    retained/        mnm, old, new, mnm_bpm, new_bpm, old_bpm
    retired/         только для новых явно согласованных кандидатов до архивации
  track/
    TrackEQ.inl, TrackFILT.inl, TrackDIST.inl, TrackENV.inl,
    TrackVOLPAN.inl, TrackSRR.inl, TrackDelay.inl,
    DelayFeedbackDynamics.hpp, routing helpers
  filters/
    monomachine filter + hybrid_private
  machines/
    bbox и прочие отдельные machine cores
```

Для Phaser/Flanger/RingMod самый безопасный первый шаг — вырезать каждый класс
в свой header и оставить `monomachine_effects.hpp` как thin compatibility
facade, включающую три новых файла. Тогда `NovaDSP.h`, machine IDs и ручки не
меняются, а один конкретный effect core позже можно заменить изолированно.

## Порядок безопасного рефакторинга

1. Сначала механическое перемещение без изменения тел функций, параметров,
   public names и порядка вызовов.
2. После каждого маленького шага — проверка include/CMake paths, parity Synth/FX,
   `SOURCE_BUILD.json` и package manifest.
3. Только пользовательская Windows Release build подтверждает C++/link result;
   без неё не объявлять folder refactor собранным.
4. Phaser уже исправлен отдельной локальной заменой mixer formula; прежняя
   формула оставлена рядом комментарием. Для дальнейшей настройки нужен
   воспроизводимый preset/вход/ожидаемое сравнение, а не догадка о железе.
5. Experimental FM imports уже вынесены из shipping Source с полным SHA-256
   архивом в `PATCH_HISTORY`; новые candidates удалять только тем же порядком.
