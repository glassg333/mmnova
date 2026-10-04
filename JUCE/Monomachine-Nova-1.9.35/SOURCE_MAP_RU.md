# Monomachine Nova 1.9.35 — карта исходников и безопасный план структуры

Дата актуальной package revision: 04.10.2026; последний source-only audit: 04.10.2026. Карта фиксирует структуру после снятия
retired FM candidates, документированных Phaser/DFB corrections и direct-LFO
DPTH option-stack alignment. Полная сборка, DAW, render и executable tests не
запускались.

## Что уже убрано из корня

Исторические Oracle/FM/filter/routing audits, handoff, отозванный Track Delay
hotfix, старые validation logs и каталог `oracle/` перенесены **без изменения
текста** в:

`PATCH_HISTORY/SNAPSHOTS/ROOT_AUDITS_AND_HANDOFFS_PRE_1.9.26/`

В корне оставлены только действующие Markdown-документы:

- `README_FIRST.md`;
- `README_1.9.35_SOURCE.md`;
- `VALIDATION_STATIC_AUDIT_2026-10-03_DFB_BASE_BEND.md`;
- `TRACK_DELAY_CORRECTION_2026-09-30.md` и старые validation logs как historical context;
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

Direct-LFO `DPTH` в `PluginEditor.cpp` сохраняет обычную `PAN` ручку
`{getWidth()/2-26,27,52,43}` и центрированный LCD. Вверх перенесён только
компактный option stack: `optionTop=directDepthKnob.y+8`, `UNI` — 12 px, `INV`
— ещё 12 px через зазор 1 px. В стандартной P1 ячейке это даёт текст около
`(1232,261)` и `(1232,274)` и сохраняет зазор перед `DPTH 127` LCD; parameter
IDs/state, callbacks, hit areas и Matrix/RMB representations не изменены.

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

| Бывший raw ID | Бывший каталог | Статус в 1.9.35 |
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
к снятию не согласованы и в 1.9.35 не выполняются.

## Track Delay: BASE BEND и RAW-GATED GUARD — реальный runtime путь

`Source/dsp/DelayFeedbackDynamics.hpp` — отдельный controller перед recurrence
всех `TrackChain` delay modes. Capture/HOLD logic и schema-42/schema-43
описания сохранены в `PATCH_HISTORY/SNAPSHOTS/` и не активны.

| Задача | Active source |
|---|---|
| Persisted P1/P2 BASE CURVE/HOLD и GUARD controls/defaults | `Source/NovaData.h` |
| APVTS raw pointers, block snapshot, schema `<44` migration | `Source/PluginProcessor.{h,cpp}` |
| BASE/Guard state, formula, cached LUT и follower | `Source/dsp/DelayFeedbackDynamics.hpp` |
| TrackChain setters и OLD/MNM/NEW ownership | `Source/NovaDSP.h`, `Source/dsp/TrackDelay.inl` |
| White embedded RMB overlay, pin, graph и bounded drag | `Source/PluginEditor.cpp` |
| Source-only regression specification | `tests/DelayFeedbackDspTests.cpp` |

Current schema **45** DFB law:

- `RAW=63` — exact unity `1.000000`. `BASE CURVE` shapes `RAW 0..62` toward
  this point, so DYNAMICS itself does not attenuate the RAW=63 endless tail.
- `HOLD @64` keeps its APVTS ID but is now the first over-unity GUARD entry:
  active range `1.0000..65/64`, fresh default `65/64`. It meets the retained
  `RAW/64` upper growth law continuously at `RAW=65`.
- A pre-44 project keeps neutral `BASE CURVE=1.00` and `HOLD @64=1.0000` as a
  safe lower entry; no parameter ID/state route is removed.
- RAW `<=63` is a full GUARD **and final-write-clip** bypass. RAW `>=64`
  interpolates `START @64`, `PLT @64`, `START @127`, `PLT @127`, plus common
  `OFFSET`. Fresh defaults are `0/.05`, `0/3.46`, offset `.18`, curve `1.08`,
  amount `2`, attack/release `1 ms`.
- `setGuardAndGovernor()` compares sanitized snapshots before rebuilding its
  `65×257` guard-drive LUT; BASE has a separate small LUT that likewise rebuilds
  only on BASE edit. No LUT `pow()` work occurs per audio block when controls
  have not changed.

RMB `DFB` is a white `400×386` `FloatingPanel` child overlay inside the editor,
not a detached native window. Its common post-child compositor explicitly resets
white `ink` before every static foreground label, so DFB/FILTER title/footer and
DTIM/DPTH text remain visible both pinned and unpinned. It has the shared pin,
no `X`, 10-Hz graph repaint and screen-coordinate bounded dragging. This UI has
no route into custom `FxSlotEngine`: `DFB_ROUTE_UNRESOLVED` remains the explicit
boundary.

Schema 42 still removes the non-working DLY CORE: `DLY` is only
`mnm|old|new`, and saved CORE value `3` from schema 40/41 maps explicitly to
`new=2`. DBAS/DWID retain the current 12 dB feedback-filter response.

## ARP SONG: активное окно UI

`SongLane` в `PluginEditor.cpp` хранит все 64 сериализованные позиции, но видит
и редактирует только `S START..S END`. Геометрия активного окна растягивается
динамически как у обычных ARP step rows. Горизонтальный ЛКМ-drag рисует
`S-PART` или `REPEAT` через каждую пройденную активную позицию (с заполнением
интервала между mouse events); ПКМ возвращает default только выбранной позиции. Это UI
поведение не меняет SONG transport, диапазон параметров или schema.

Под step lanes больше не рисуется redundant `PAGE 1..64`; authority остаётся
`EDIT PAGE` и English hover tooltip. У SONG toggle скрыт перекрывающийся label,
но hit area/tooltip сохранены. RMB COPY/PASTE открывает `WHOLE PAGE` как первый
ASCII пункт, а все function tooltips ARP/Surface в `PluginEditor.cpp` не
содержат кириллицу: pixel-font tooltip window получает только English/ASCII.

### Main-surface MOD ENV

`Surface::showEnvelopeMini()` выполняет AMP/FIL/MOD dock replacement как
атомарный category handoff: selection target ставится до `dismissDock()` и
сохраняется во время удаления старого envelope dock. `PixelButton` у controls с
`heldDrag` не использует generic drag-pressed fill, а при release real drag
временно маскирует originating `onClick` во время `TextButton::mouseUp()`.
Поэтому удерживаемый source не остаётся белым и не возвращается после release:
сохраняется последняя пересечённая category.

`ModEnvelopeMiniPage` — ровно compact shell AMP/FIL: `387×236`, graph
`8,32,371,114`, control row `x=8+i×95`, `y=152`. В mini нет особых page/route
buttons, `MATRIX SOURCE`, `MOD ENVn` или `RMB LARGE`; они не должны появляться
из-под category buttons как отдельный слой. Полная `ModEnvelopePage` по RMB
сохраняет выбор `MOD ENV1..4` и `ROUTE TO...`; переход full→compact передаёт
выбранный source обратно без смены параметров.

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
