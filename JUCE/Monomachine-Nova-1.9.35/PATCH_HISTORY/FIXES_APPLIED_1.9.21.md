# FIXES APPLIED — сводная полная история до 1.9.21

> Это не новая спецификация и не сокращённое резюме. Ниже дословно сохранены
> исторические numbered README, release, validation и routing-handoff документы.
> Byte-identical тела записаны один раз с перечислением всех исходных путей.
> Актуальный источник правил: `../README_FIRST.md` и
> `../README_1.9.21_FM_MODE_CLEANUP_SOURCE.md`.

## Правило чтения

Исторические записи могут описывать withdrawn FM IDs 6..8, Fix-5 или прежние
state-маршруты. Это доказательство истории, а не разрешение включать эти пути
в текущем MODE SYNT. Текущий контракт имеет шесть modes с IDs 0..5.

## Номерные заметки продукта

<a id="monomachine-nova-synth-readme-1-9-0-md"></a>
### `Monomachine_Nova_Synth/README_1.9.0.md`

`SHA-256: f6ac6dfa5c86875c1b9d1713d2198da26193193bded43bbfbc8c4aeda2073296`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.0.md`

---

# Monomachine Nova 1.9.0 — native DSP rollback

Дата документа: 26 сентября 2026. Этот выпуск для **Synth** полностью
отменяет экспериментальные DSP-ветви предыдущего 1.9.0-пакета и возвращает
штатные пути. Аналогичное изменение выполнено в парном проекте.

## Что сохранено

- Native/default остаётся основным путём для P1 и P2.
- Оригинальный P2-список и `MachineEngine` сохранены без смены номеров,
  выбора или подстановки ядер.
- Для SYNT, FILT, DIST и DLY доступны только `mnm|old`.
- Для AMP доступны только прежние `old|mnm|vital`.
- Порядок обработки не менялся: `EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY`.
- BBOX продолжает использовать прежние native законы: firmware-specific флаги
  старта и питча отключены.

## Что удалено

- Все селекторы и UI-пункты `new`; пользовательский пункт FMA также удалён.
- Импортированные исходники, bridge-классы и тесты экспериментальных ветвей,
  включая source-derived осцилляторы, фильтр, envelope, DIST, SRR, delay и P2
  ядра.
- Больше нет параметра `p2_fx_mode`; P2 всегда идёт через штатный native путь.
- Не добавлялись guessed substitute DSP, автоматический limiter, дополнительное
  filter-darkening либо fabricated envelope/DIST hybrid.

## Совместимость состояний

Схема DSP-режимов имеет версию **17**. При загрузке старого состояния:

- значения режимов, прежде указывавшие на FMA или withdrawn source-derived
  ветви, нормализуются к `mnm`;
- прежнее AMP-значение `new` нормализуется к `mnm`;
- устаревший элемент `p2_fx_mode` удаляется из загруженного дерева состояния.

Поэтому старые проекты открываются без обращения к удалённому коду; при
необходимости сохранить точное экспериментальное состояние следует использовать
отдельную историческую копию проекта, а не этот release source package.

## Проверка поставки

`ModeRollbackTests` проверяет двухзначный реестр (`mnm|old`), отсутствие имён
withdrawn режимов, версию схемы и миграцию старых индексов в `mnm`. Полная
проверка сборки и тестов выполняется для Synth и FX перед упаковкой; результаты
публикуются рядом с архивом в release-заметке.

Исходники в `Source/dsp/mnm_new/`, `Source/dsp/mnm/new/` и
`MnmAmpDistNew.hpp` намеренно отсутствуют из этой поставки.


---

<a id="monomachine-nova-synth-readme-1-9-1-md"></a>
### `Monomachine_Nova_Synth/README_1.9.1.md`

`SHA-256: c4fbd625a04e357525eb85d46256d3ab1ff2bde4203db0ed165b7c4d6520a694`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.1.md`

---

# Monomachine Nova Synth 1.9.1

See the package-level release notes at:

- `../RELEASE_1.9.1_MODE_S_AND_BLOCK_BYPASS.md`
- `../HYBRID_FILTER_AUDIT_2026-09-26.md`

Synth-specific scope is identical to the paired FX source project for MODE
L/H/S, classic-block bypass persistence, and schema-18 migration.

Key user-facing points:

- MODE S is the one audible DIST selection: `MNM`, `OLD`, `FOLD`, `ZERO`,
  `CLAMP`.
- MODE L and MODE H are ordered by lower/upper physical filter-side semantics.
- Every P1/P2 classic block has independent ON/OFF bypass without moving any
  physical FX-slot point.
- `MNM FIX` is deliberately deferred until the unresolved per-machine firmware
  handler transfer is bounded by evidence.

This project is source-only. The revision passed standalone and syntax checks;
it was not fully linked into a plugin binary in this environment.


---

<a id="monomachine-nova-synth-readme-1-9-10-md"></a>
### `Monomachine_Nova_Synth/README_1.9.10.md`

`SHA-256: dca97a0813df30eb65a914d212685753b0268b622c3688e352b70847c48065f8`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.10.md`

---

# Monomachine Nova Synth 1.9.10

Source-only patch release. This Synth package retains 1.9.8's DLY `mnm|old|new`
feedback path and 1.9.9's R-classic filters, then appends R Import 2 hybrid
families without moving existing IDs.

## Hybrid filter additions

`MODE L` / `MODE H` now contain compact `R IMPORT 2` groups for:

- `R ANALOG`, `R LINEAR` (LP/BP/HP at 12/24 poles);
- `R RBJ`, `R TPT` (LP/BP/HP);
- `R HUV`, `R HYPER`, `R KRAJ`, `R MICRO`, `R MUSIC`, `R OBER`, `R DVAL`.

MODE L keeps physical lower-side HP/low-cut ordering; MODE H keeps upper-side
LP/high-cut ordering. The nested groups keep the menu reachable at the lower
edge of the plug-in window.

## Saved state

Hybrid IDs `0..20` are frozen. IDs `21..51` are new and schema 25 records the
append. Pre-25 state never reinterprets malformed later values as a new sound.

## Source boundary

The requested `for import/2` collection is handled via independent,
self-contained equivalents because its framework dependencies and licensing
provenance are incomplete. See [`../R_IMPORT2_AUDIT_2026-09-27.md`](../R_IMPORT2_AUDIT_2026-09-27.md)
and [`../RELEASE_1.9.10_R_IMPORT2_FILTERS.md`](../RELEASE_1.9.10_R_IMPORT2_FILTERS.md).


---

<a id="monomachine-nova-synth-readme-1-9-11-md"></a>
### `Monomachine_Nova_Synth/README_1.9.11.md`

`SHA-256: a4199316f1cc043d1305c67194e48463f079668f6b402d8188ca28121ed7994f`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.11.md`

---

> **SUPERSEDED / NOT AN ACCEPTED ROUTE:** This product note documents the
> rejected 1.9.11 automatic MNM-carrier policy. The active correction is
> [`README_1.9.12.md`](README_1.9.12.md).

# Monomachine Nova Synth 1.9.11

Source-only filter-route repair release. It retains the 1.9.8 DLY
`mnm|old|new` feedback path, 1.9.9 R-classic choices, and 1.9.10 appended R
Import 2 IDs without moving any saved value.

## FILTER repair

`DSP FILT` may remain at its normal `old` default. Once either `MODE L` or
`MODE H` is non-`NATIVE`, Synth automatically renders through the
hybrid-capable filter core, so the selected R/K35/Moog response is audible.
With both selectors at `NATIVE`, `old` remains the retained legacy filter.

`R HUV` now uses a bounded normalized independent ladder equivalent and `R
DVAL` no longer reverses at open cutoff. These changes target the previously
quiet, clipped, or garbled imported-filter behavior while keeping the existing
physical BASE/WDTH law.

## Menu

All R choices now live in one `R CLASSIC` popup branch: `R 303`, `R MS20`, `R
MOOG`, `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`, and `R LADDERS`. The compact
cascade remains safe at the lower edge of the editor. Choice IDs stay exactly
as saved in 1.9.10.

## Saved state

The schema remains 25 and IDs `0..51` remain frozen. MODE L is the physical
lower / BASE edge; MODE H is the physical upper / BASE+WDTH edge. Intentional
mismatched responses can attenuate at extreme cutoff positions by design.

See [`../RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md`](../RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md)
and [`../VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md`](../VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md).


---

<a id="monomachine-nova-synth-readme-1-9-12-md"></a>
### `Monomachine_Nova_Synth/README_1.9.12.md`

`SHA-256: fa529a372951d4d3232787ab2eaa5691ed2abec15c92bf470eee694d2ddafc76`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.12.md`

---

> **Superseded in this working tree by 1.9.13:** the original 1.9.12 archive remains unchanged. See [`README_1.9.13.md`](README_1.9.13.md).

# Monomachine Nova Synth 1.9.12

Source-only independent physical-filter route correction.

## FILTER behaviour

`DSP FILT` is again limited to its two native alternatives while both `MODE L`
and `MODE H` are `NATIVE`:

- `old` runs the retained OLD native filter;
- `mnm` runs the retained MNM native filter.

When either MODE side selects K35, MOOG/ladder, R CLASSIC, or R Import 2, this
product renders the selected family through a separate
`IndependentPhysicalFilterCore`. The OLD renderer and MNM native renderer are
both excluded from that call, so there is no OLD+selected or MNM+selected
serial path. The selected side replaces its own native physical stage once;
the other side follows its own explicit MODE choice.

## Compatibility

Schema remains 25. MODE L/H IDs `0..51`, their BASE/WDTH physical direction,
the unified R CLASSIC popup, R HUV/R DVAL repairs, and DLY
`mnm|old|new`/DBAS/DWID/Q work are retained without parameter remapping.

See [`../HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md`](../HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md),
[`../RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](../RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md),
and [`../VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](../VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md).


---

<a id="monomachine-nova-synth-readme-1-9-13-md"></a>
### `Monomachine_Nova_Synth/README_1.9.13.md`

`SHA-256: db2041d68d8153d10db0ac2ff52a25840dfcb21f5cab2b80b2a096c3607007a6`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.13.md`

---

# Monomachine Nova Synth 1.9.13

Source-only physical-filter sound/control follow-up.

When a MODE L/H side selects K35, MOOG/ladder, R CLASSIC, or R Import 2, it
uses the separate `IndependentPhysicalFilterCore` route from 1.9.12. In 1.9.13
that selected route no longer receives the native MNM carrier's generic output
soft clip: resonance is not silently made into a common saturation stage.
`FILT SAT=0` remains an exact bypass; a selected model's own documented
nonlinear behaviour is not replaced by a global limiter.

Physical cutoff/resonance targets now ramp for 10 ms at the real sample rate.
K35 and ladder ignore repeated identical host targets while a ramp is running,
so a live BASE/WDTH move is not rearmed at the filter-envelope cadence.

NATIVE/NATIVE `DSP FILT=old|mnm`, schema 25, IDs 0..51, physical side semantics,
R CLASSIC menu, R HUV/R DVAL repairs, and DLY `mnm|old|new` / DBAS/DWID/Q are
retained. See
[`../RELEASE_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](../RELEASE_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md)
and
[`../VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](../VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md).


---

<a id="monomachine-nova-synth-readme-1-9-14-md"></a>
### `Monomachine_Nova_Synth/README_1.9.14.md`

`SHA-256: 888bc249c44a77799ad89575dc0ed68dd74824d09818fc929bc9117bbe98bf94`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.14.md`

---

# Monomachine Nova Synth 1.9.14

Source-only follow-up: documented independent filter keyboard tracking and
compact envelope access. The same source changes are mirrored in the FX tree.

## Independent HPF / LPF key tracking

Two new P1 parameters and their P2 counterparts are binary switches:

- `filt_track_hpf` / `filt_track_lpf`
- `p2_filt_track_hpf` / `p2_filt_track_lpf`

They default to **ON**, matching normal Monomachine tracking. Each physical edge
can be disconnected independently.

With a switch on, the manual-derived target is:

```text
cutoff = played-note frequency / 4 * 2^(control / 8)
```

Thus HPF `BASE=0` begins two octaves below the played note and every `+8` BASE
units is an octave. LPF retains the existing Monomachine physical topology
`BASE + WDTH`: every `+8` BASE or `+8` WDTH units adds an octave to its edge.
The native MNM table core selects the closest retained table cutoff; the legacy
core calculates the same Hz law through its own coefficient path. Existing
VEL/KT amount parameters remain additive legacy modifiers; they are not these
new switches.

Schema 26 migrates states without these controls to both switches ON, preserving
the documented normal-tracking default.

## UI

- RMB on the FILT-page `BASE` or `WDTH` control opens `FILT / EXTRA` for the
  current P1/P2 faceplate.
- That compact panel contains visible `HPF KEYTRACK` and `LPF KEYTRACK`
  checkboxes. The older continuous `KT-L` / `KT-H` parameters remain serialized
  for old projects but are no longer presented as the normal tracking controls.
- RMB on the `FILT` category header no longer opens the extras panel.
- The main surface now reads `AMP`, `FIL ENV`, `MOD ENV`, then `MODE AMP`.
  Left-click `FIL ENV` or `MOD ENV` opens a compact duplicate that binds the
  same parameters as the retained full tab. RMB on its header button opens the
  retained full page; RMB on empty compact/full-page space switches the other
  direction, matching AMP-envelope navigation. The compact MOD ENV view keeps
  its selected source and exposes its `ROUTE` action.

## Validation performed on source

Standalone source tests passed for the real MNM filter (including the new
manual-law checks), existing filter-extra DSP behavior, and the production
`TrackFILT.inl` route integration. A legacy-core octave-equivalence smoke check
also passed. No JUCE plug-in build, DAW load, or interactive GUI test was run
for this source-only delivery.


---

<a id="monomachine-nova-synth-readme-1-9-15-md"></a>
### `Monomachine_Nova_Synth/README_1.9.15.md`

`SHA-256: 6d550354772279b550ae049e9ab4e64dd398417d34049f3442a836c399df326e`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.15.md`

---

# Monomachine Nova Synth 1.9.15 — source-only listening candidate

This tree is a **source-only candidate**. No ZIP archive, JUCE plug-in build,
DAW load, or interactive editor test has been claimed or produced yet. The
Synth and FX source changes are mirrored.

## P2 key tracking and native filter output

- Independent manual-derived `HPF KEYTRACK` and `LPF KEYTRACK` controls remain
  independent. Unchanged native parameter snapshots and unchanged MODE L/H
  selections now skip redundant coefficient rebuilds, which removes the
  pathological per-sample work reached by the one-sample P2 route.
- The native MNM filter no longer applies a hidden generic output soft clip.
  The DC blocker remains; intentional `FILT SAT` and `TRACK DIST` nonlinear
  paths remain separate. High-Q output is required to remain finite, but is
  allowed to exceed the former roughly ±2 knee.
- Filter-menu separator painting/height is repaired so dividers are visibly
  lines rather than blank selectable-looking gaps.

No broad filter-envelope or signal-order theory was introduced: the documented
route remains `headroom → EQ → FILTER → DISTORTION → AMP ENV → VOL/PAN → SRR →
DELAY → LEVEL`.

## MODE S A/B candidates

Existing serialized selections retain their IDs and behavior:

| ID | MODE S | Status |
|---:|---|---|
| 0 | `MNM` | retained reference |
| 1 | `OLD` | retained reference |
| 2–5 | `MNM FIX`, `FOLD`, `ZERO`, `CLAMP` | retained existing comparisons |
| 6 | `MNM+OLD` | appended candidate |
| 7 | `MNM V2` | appended candidate |
| 8 | `OLD V2` | appended candidate |

Schema 27 appends IDs 6–8 only. States written before schema 27 preserve valid
IDs 0–5; an impossible later value in an older state normalizes to `MNM` rather
than being interpreted as a new candidate. A schema-27 state retains each P1/P2
candidate independently.

All retained and appended transfer laws are exact unity at LCD `DIST=0` (raw
centre 64). Their positive side is deliberately different:

- **MNM+OLD** retains MNM's exact positive output/headroom compensation while
  substituting OLD's normalized `tanh` curve for the positive drive.
- **MNM V2** retains MNM's exact negative/centre path and hard pre-drive, but
  uses square-root positive output compensation. It isolates a level/headroom
  question from a soft-versus-hard curve question.
- **OLD V2** retains OLD's negative/centre path, then uses a stronger soft
  positive drive with a moderated positive output trim.

These are named comparison candidates, **not firmware-identity claims**. In
particular, the reported loud-but-under-distorted `OLD +63` behavior has not
been relabelled as solved. Compare references and candidates at matched output
levels before removing either `MNM` or `OLD`.

## Suggested listening matrix

For a sustained harmonic-rich source and a transient source, compare `MNM`,
`OLD`, `MNM+OLD`, `MNM V2`, and `OLD V2` at `DIST=0`, `+16`, `+32`, and `+63`.
Level-match after the block, then note (1) onset grit, (2) sustain compression,
(3) resonance interaction near the lower-filter region, and (4) output level.
Keep `FILT SAT` at zero for a first pass. This makes a `MODE S` conclusion
separate from intentional FILT saturation or downstream gain staging.

## Source validation performed

- `DistVariantTests`: exact zero identity, unchanged OLD reference law,
  negative headroom endpoints, candidate endpoint/curve distinction, and
  finite sweeps. `TrackDistVariantIntegrationTests` separately compiles the
  production `TrackDIST.inl` body and verifies exact dispatch of IDs 6–8.
- Existing standalone DSP targets passed in both trees, including `MnmCore`,
  `MnmRealFilter`, `HybridDsp`, `FilterRoute`,
  `TrackFilterRouteIntegration`, `Import2Filters`, `FilterExtras`,
  `DelayFeedback`, `ModEnvMatrix`, and `ArpWindow`.
- The project-local static verifier passed in both trees.

The JUCE modules required for a full plug-in/editor build were not available in
this environment. Therefore this document is not evidence of a compiled VST3,
DAW CPU percentage, GUI interaction test, or final sonic judgment.


---

<a id="monomachine-nova-synth-readme-1-9-16-md"></a>
### `Monomachine_Nova_Synth/README_1.9.16.md`

`SHA-256: 245d6c204ad633e9bcfc4d9590d94dea2a35f753aa0cbbf713e69682cf655f56`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.16.md`
- `Monomachine_Nova_FX/README_1.9.16.md`

---

# Monomachine Nova 1.9.16

## Добавлено

- MODE L/H сохраняет сериализованные IDs `0..51` и добавляет шесть HP-комплементов `52..57`: `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`, `R OBERHEIM HP4`, `R DVAL HP4`.
- Меню сортируется по фактическому DSP-ответу: HP/low-cut находится в первом разделе MODE L, LP/high-cut — в первом разделе MODE H.
- Комплементы получены из самих LP-ядер; это расширения/диагностические варианты, не модели оригинальной прошивки Monomachine. У DVAL инвертирован знак LP-выхода, поэтому его HP — `dry + LP`; у остальных пяти — `dry - LP`.
- Schema 28 принимает новые IDs; более старые schemas не переинтерпретируют их.

## Проверка

В Synth и FX прошли JUCE-free C++17 `Import2FiltersTests`, `HybridDspTests` и `FilterRouteTests`. Это проверяет исходные карты, спектральный ответ комплементов и маршруты, но не заменяет сборку JUCE/VST3, загрузку в DAW или слуховое сравнение.

## Не заявляется

1.9.16 не исправляет и не объявляет решёнными native Monomachine FILT/Q/BOFS, AMP ENV или DIST. Для границ исследования см. `../FILTER_SOURCE_EVIDENCE_2026-09-28.md` в source-пакете.


---

<a id="monomachine-nova-synth-readme-1-9-17-md"></a>
### `Monomachine_Nova_Synth/README_1.9.17.md`

`SHA-256: 82020bacb92c7ee10007379b7d0fc848e3cf091ca679b883fe8649d0b155287c`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.17.md`

---

# Monomachine Nova Synth 1.9.17

## Меню MODE L / MODE H

- MODE L показывает только обычные `LOW CUT / HP`; MODE H — только `HIGH CUT / LP`. Противоположные response-типы не мешают обычному выбору.
- Все BP остаются в одной папке `BAND PASS`; для MODE L после неё добавлена последняя папка `DRY`.
- `R HYPER` находится последним внутри HP, LP и BP-папок; `R HYPER NOTCH` остаётся отдельным.
- `R HUV/KRAJ/MICRO/MUSIC/OBERHEIM/DVAL HP4` вынесены в MODE L → `DRY`. Это не обычные HP-реализации: пять используют `dry - LP`, DVAL — `dry + LP`. Удаление прямого входа не даст честный HP, поэтому они явно названы тестовой отдельной группой.
- Popup, колесо и drag используют одинаковую фильтрацию видимых пунктов.

## Проверка

В Synth и FX прошли JUCE-free C++17 `Import2FiltersTests`, `HybridDspTests` и `FilterRouteTests`; статический integrity-check также прошёл для обоих деревьев. Полная JUCE/VST3/DAW-сборка и слуховая проверка не выполнялись.

## Не заявляется

1.9.17 не исправляет и не объявляет решёнными native Monomachine FILT/Q/BOFS, AMP ENV или DIST.


---

<a id="monomachine-nova-synth-readme-1-9-2-md"></a>
### `Monomachine_Nova_Synth/README_1.9.2.md`

`SHA-256: 45d31fb3c27caff789df4b4942c0b8ca73d888ed2b78f0fc820753029cff9016`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.2.md`

---

# Monomachine Nova Synth 1.9.2

Package notes:

- `../RELEASE_1.9.2_MNM_FIX_AND_IMPORTED_DECLICK.md`
- `../HYBRID_FILTER_AUDIT_2026-09-26.md`

Synth has the same schema-20 MODE L/H/S plus opt-in FILT/MOD ENV, classic-block bypass, experimental
MNM FIX, and 16-sample imported-side coefficient-ramp implementation as the
paired FX source project.

MNM FIX is an explicitly experimental, separately selectable A/B candidate;
MNM and OLD remain unchanged. This is a source-only revision, validated by
standalone and syntax checks rather than a completed full plugin binary build.


---

<a id="monomachine-nova-synth-readme-1-9-20-md"></a>
### `Monomachine_Nova_Synth/README_1.9.20.md`

`SHA-256: 7d240e64e99c7a2050e5fda6f5fa8cf4f0b7f26b465d0cf64ab16f9dd4603379`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.20.md`

---

# Monomachine Nova Synth 1.9.20 — исходники

Это половина **Synth** исходного пакета 1.9.20. Готовый VST3 здесь не
поставляется.

FM+ сохраняет ID 0…7 и добавляет независимый Fix-5 как ID 8/schema 34 только
для m8 STAT, m9 PAR и m10 DYN. Fix 4 остаётся отдельным. TRY4 (ID 7) теперь
активно использует собственный Package-5 FM/AMP-frame импорт в
`Source/dsp/fm_try4_voice/`; старый `fm_try4` удалён. Точный полный
Package-8 tail служит оракулом маршрутизации и не запускается интерпретатором
в audio callback; см. `../ROUTING_AUDIT_PACKAGE8_2026-09-29.md`.

В `Try4VoiceAmpEnv.hpp` storage PAN-таблиц сделано C++17 `inline`: это
необходимый ODR repair для VST3, чтобы `PluginProcessor.obj` и
`PluginEditor.obj` не давали `LNK2005/LNK1169`; сама DSP-математика не менялась.

DSND — прямой 0…127 Q23-уровень моно-посыла ColdFire. Он не является знаком
стерео, фазой или ping-pong-переключателем. LFO в DSND меняет только уровень
эхо-посыла.

## Hotfix Track Delay — 30.09.2026

Исправлен маршрут, из-за которого на свежем экземпляре одновременно работали
посыл P1 и полный скрытый путь P2: оба стартовали с `DSND=64`, поэтому хвост P2
маскировал изменения `P1 DSND` и `P1 DTIM` во всех режимах DLY.

* Новый P1 и P2 начинают с `DSND=0`; P2 выбирает прозрачный `FX-THRU`, а
  `P2 MIX=0`. Поэтому эффект появляется только после явного подъёма посыла и
  не подмешивается сам.
* При первом чтении прежнего состояния исправляется **только** точный старый
  заводской образ P2 (CHORUS/default words/`MIX=127`/`DSND=64`). Его P2
  переводится в `FX-THRU`, `P2 DSND=0`, `P2 MIX=0`. Сохранённый P1 и любой
  изменённый P2-параметр не переписываются; маркер
  `trackDelayDefaultRouteV1` делает операцию одноразовой.
* Регрессии покрывают свежие APVTS defaults, старое точное factory-state и
  сохранение custom P2-state в `tests/P2RoutingTests.cpp` и
  `tests/RollbackStateTests.cpp`.

Быстрая проверка в DAW: на новом экземпляре `DSND=0` не даёт хвоста; поднимите
`P1 DSND`, затем меняйте `P1 DTIM` — слышимое время эха должно меняться. Для
P2 сначала поднимите `P2 MIX` и его собственный `P2 DSND` намеренно.

Перед выпуском:

1. выполните `python3 verify_dsp_mode_patch.py`;
2. соберите проект через обычную Windows-цепочку Projucer/VS2022;
3. выполните CTest и тест VST3 в DAW на 44,1/48/96 кГц.

См. `FM_EXPERIMENTS_README.md`, `PROVENANCE.md` и `VALIDATION_REPORT.md`.


---

<a id="monomachine-nova-synth-readme-1-9-21-md"></a>
### `Monomachine_Nova_Synth/README_1.9.21.md`

`SHA-256: b89c11049e6fbaf5b8e661fb84e2012de6b6a3b69529bdce4cdf707d6f33c77f`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.21.md`

---

# Monomachine Nova Synth 1.9.21 — исходники

Это половина **Synth** исходного пакета 1.9.21. Готовый VST3 здесь не
поставляется.

FM+ сохраняет ID 0…7 и добавляет независимый Fix-5 как ID 8/schema 34 только
для m8 STAT, m9 PAR и m10 DYN. Fix 4 остаётся отдельным. TRY4 (ID 7) использует
собственный Package-5 FM/AMP-frame импорт; полный Package-8 tail служит
маршрутным оракулом и не запускается интерпретатором в audio callback.

## Действующая коррекция Track Delay

Восстановлены исходные factory значения: P1 EFFX `DSND=64`, P2 EFFX
`DSND=64`, исходный P2 `FX-CHORUS` и `P2 MIX=127`. Marker-gated migration
`trackDelayDefaultRouteV1` удалена: сохранённые состояния P2 не меняются
автоматически.

DSND трактуется как масштаб verified host-side delay-route. Это **не** знак,
L/R-выбор, стерео-вход, фаза или ping-pong control. Левый/правый route не
имитируется через DSND: его перенос отложен до прямого подтверждения кодом и
vectors.

## Source-only UI/DSP коррекции

- filter mode popup сортируется по response/type, затем по имени, без смены
  сохранённых choice ID;
- HPF/LPF keytrack использует согласованные физические позиции BASE/WDTH, а
  P2 bypass не маскирует отключённый пользователем keytrack;
- MOD ENV имеет симметричный с AMP/FIL ENV large-tab handoff;
- верхние AMP/FIL ENV/MOD ENV кнопки корректно показывают selected-state после
  dock/overlay transition;
- held-drag по AMP/FIL/MOD tabs работает в large mode.

## Проверка

Запустите `python3 verify_dsp_mode_patch.py` из этой директории. Проверка
статическая и source-only; рендер, полная VST3-сборка и DAW-проверка здесь не
заявляются.

Подробности и статус отозванной старой гипотезы: в корне пакета
`TRACK_DELAY_CORRECTION_2026-09-30.md`.


---

<a id="monomachine-nova-synth-readme-1-9-3-md"></a>
### `Monomachine_Nova_Synth/README_1.9.3.md`

`SHA-256: 04ca58d7439a0588355a492c78f9f86d910e6aeb2d050cbd3a131d7d87594582`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.3.md`

---

# Monomachine Nova Synth 1.9.3

See [`../RELEASE_1.9.3_FILT_EXTRAS_ENV.md`](../RELEASE_1.9.3_FILT_EXTRAS_ENV.md).

This source-only Synth revision adds independent P1/P2 FILT extras, one
opt-in post-FILT Korg/Odin `SAT` stage, additive FIL ENV through `BASE/WIDTH`,
and four MOD ENV matrix sources. `SAT=0` and `ENV FIL=0` are neutral; MNM and
OLD remain separate references. Schema 20 safely fills neutral values for
older projects.


---

<a id="monomachine-nova-synth-readme-1-9-4-md"></a>
### `Monomachine_Nova_Synth/README_1.9.4.md`

`SHA-256: 63d252027fe8aa6fc3a3d971a8c74ec4e972f19c8dc6c91c63f1146f22e0967a`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.4.md`
- `Monomachine_Nova_FX/README_1.9.4.md`

---

# Monomachine Nova Synth 1.9.4

See [`../RELEASE_1.9.4_LFO_ARP.md`](../RELEASE_1.9.4_LFO_ARP.md) for the canonical six-LFO P1/P2 mapping, Matrix/MSEG target repair, live 1–64 step ARP window, independent SYNC-able P-LOCK window, appended ARP/P-LOCK Matrix boundary targets, schema-22 migration, the imported FILT state-continuity repair, and validation boundary.


---

<a id="monomachine-nova-synth-readme-1-9-8-md"></a>
### `Monomachine_Nova_Synth/README_1.9.8.md`

`SHA-256: 44f2e8f934a1dc827d87ffb390d9fde79a50583ba3ba24fbaff6d7534b0587e7`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.8.md`

---

# Monomachine Nova Synth 1.9.8

This source release adds usable feedback-loop filtering to EFFX DLY and appends the opt-in `NEW` Track Delay mode while preserving `mnm` and `old` at their saved/automation IDs `0` and `1`.

- Select `MODE DLY` = `new` only when the new practical Track Delay candidate is desired.
- `DBAS` raises the feedback high-pass edge; reducing `DWID` lowers the feedback low-pass edge.
- Right-click `DBAS` or `DWID` on either P1/P2 EFFX DLY page for the matching optional `DBAS Q` / `DWID Q` settings.
- Existing states before schema 23 never silently turn a historic value `2` into `NEW`; they fall back to `mnm`.

See [`../RELEASE_1.9.8_TRACK_DELAY_NEW.md`](../RELEASE_1.9.8_TRACK_DELAY_NEW.md) for the exact scope, provenance, parameter IDs, compatibility rule, and validation boundary. See [`../VALIDATION_1.9.8_TRACK_DELAY_NEW.md`](../VALIDATION_1.9.8_TRACK_DELAY_NEW.md) for the completed checks and remaining JUCE/DAW validation.


---

<a id="monomachine-nova-synth-readme-1-9-9-md"></a>
### `Monomachine_Nova_Synth/README_1.9.9.md`

`SHA-256: b5623291f98b50579ba2edaa5cec78a61a46a808fded0de83d3a82ed0dd6e8ec`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_1.9.9.md`

---

# Monomachine Nova Synth 1.9.9

Source-only patch release. This Synth package retains 1.9.8's `DLY mnm|old|new`
feedback-filter work and adds the following hybrid physical-filter choices on
both P1 and P2:

- `R 303` — LP, BP, HP;
- `R MS20` — LP, BP, HP;
- `R MOOG` — LP12/LP24, BP12/BP24, HP12/HP24.

The old hybrid IDs `0..8` are untouched. The new responses append as IDs
`9..20` under schema 24. MODE L orders each filter family as HP → BP → LP;
MODE H orders it as LP → BP → HP, matching the lower/upper physical edges.
The popup is grouped and screen-aware so all choices remain reachable.

The source equations are an attributed, self-contained adaptation of the
user-supplied TB303/MS20/Moog files, not a claim that the incomplete upstream
`Filter.h` framework was compiled verbatim.

See [`../RELEASE_1.9.9_R_CLASSIC_FILTERS.md`](../RELEASE_1.9.9_R_CLASSIC_FILTERS.md)
for exact mapping, provenance, migration, safety behavior, and validation.


---

## Предыдущие product-level README и validation

<a id="monomachine-nova-synth-readme-ru-md"></a>
### `Monomachine_Nova_Synth/README_RU.md`

`SHA-256: 075e39ac089878a36899d27e36cdff6a98ac17ca26db4b7cebd6b8541266c034`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/README_RU.md`
- `Monomachine_Nova_FX/README_RU.md`

---

> **Текущий source-only пакет 1.9.21 (Fix-5 + коррекция factory DSND/P2, без рендера и DAW-прослушивания):** FM+ добавляет независимый ID 8 / schema 34 только для m8 STAT, m9 PAR и m10 DYN; retained ID 0..7, включая Fix 4 и OLD FIX, не переименованы и не заменены. Действующее описание, ограничения и команда проверки: [`README_1.9.21.md`](README_1.9.21.md), [`../TRACK_DELAY_CORRECTION_2026-09-30.md`](../TRACK_DELAY_CORRECTION_2026-09-30.md), [`../FM_FIX5_UPSTREAM_AUDIT_2026-09-29.md`](../FM_FIX5_UPSTREAM_AUDIT_2026-09-29.md).
>
> Историческое описание MODE L/H ниже сохранено как запись прежней 1.9.17 поставки.


---

<a id="monomachine-nova-synth-fm-experiments-readme-md"></a>
### `Monomachine_Nova_Synth/FM_EXPERIMENTS_README.md`

`SHA-256: 720fda8de51fdb6fd7c150ab40093dd475863b9596551b9bc6db5e8ca41a0ac1`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/FM_EXPERIMENTS_README.md`

---

# Monomachine Nova Synth 1.9.20 — контракты кандидатов FM+

Текущий продукт: **Synth**, версия проекта **1.9.20**.

## Реестр FM+ (без перенумерации)

| ID | Режим | Схема | Область |
|---:|---|---:|---|
| 0…5 | сохранённые режимы | прежняя | сохранённый контракт |
| 6 | `mnm frq env fix` | 32 | кандидат m8/m9/m10 |
| 7 | `try4` | 33 | отдельный Package-5 FM/AMP-frame кандидат m8/m9/m10 |
| 8 | `fix5 … pitch+env full` | 34 | отдельный Pack-7 кандидат m8/m9/m10 |

OLD FIX остаётся отдельным OLD-рендерером. +10 dB bridge остаётся только у
сохранённых NEW/NEW FIX. Видимое начальное `1 / 1 / 1` FM+ PAR — это
калибровка UI/дефолтов, а не изменение сохранённого DSP-закона.

## TRY4 (ID 7)

Активная реализация расположена в `Source/dsp/fm_try4_voice/`. Это не старый
`fm_try4` и не m6: путь имеет собственные Package-5 STAT/PAR/DYN-ядра, таблицы,
AMP/pitch/pan, `VoicePageMap`, нативный 16-семпловый frame renderer и lifecycle.
Полный Package-8 DSP-tail (FILT/EQ/delay/stage-2) не исполняется строковым
интерпретатором в audio callback; его точная карта и границы описаны в
`../ROUTING_AUDIT_PACKAGE8_2026-09-29.md`. Полное происхождение файлов
записано в `Source/dsp/fm_try4_voice/README_RU.md`.

## Fix-5 (ID 8)

`Source/dsp/fm_fix5/` содержит отдельный Pack-7 импорт. Нативный путь владеет
AMP, VOL², sin/cos PAN, 48-битным pitch, кадрами и release/kill. Общий host AMP
и VOL/PAN обходятся только для full-frame кандидатов, чтобы не умножать эти
законы второй раз. BPM переводится в `TickRecip=0x800000/(24*BPM)`.

## Проверка

`python3 verify_dsp_mode_patch.py` проверяет границы режимов, независимость
TRY4/Fix-5, неизменность сохранённых FM-ядер, версию и маршрут DSND. Полный
Windows/VST3/DAW барьер описан в `VALIDATION_REPORT.md`.


---

<a id="monomachine-nova-synth-stabilization-readme-md"></a>
### `Monomachine_Nova_Synth/STABILIZATION_README.md`

`SHA-256: 1e301f9d8359437d2cafb0c7cfac2794ece3dad29b5c7ea4c520deb9d9caf43e`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/STABILIZATION_README.md`

---

# Monomachine Nova 1.9.20 — FM+ stabilization and independent Fix-5 (Synth)

## Delivery baseline and versioning

The source lineage begins with the supplied 1.9.17 stable family and its Package-4 work. This delivery is intentionally branded/versioned **1.9.20**: the Synth CMake project, `.jucer`, generated JUCE version metadata, build audit script, config, and visible editor build text were updated together.

## Retained behavior still in force

- IDs `mnm=0`, `old=1`, `new=2`, `mnm fix=3`, `new fix=4`, and `old fix=5` remain stable.
- OLD FIX keeps dedicated OLD STATIC/PAR/DYN topology; m8/STAT OLD FIX is not an MNM alias.
- The +10 dB output bridge remains exclusively on retained NEW and NEW FIX.
- OLD FIX FM+ PAR retains its centered `-64..+63` TUNE display and targeted ENV-index slew.
- Existing candidate ID 6 and TRY4 ID 7 retain their separate states, tables, and namespaces.

## New independent Fix-5 candidate

- **ID 8 / schema 34:** isolated upstream Fix-5 route for FM+ m8/m9/m10 only.
- Its source lives under `Source/dsp/fm_fix5/`; it is not a replacement for Fix 4.
- The route owns native 16-sample frame handling, AMP envelope, VOL²/pan gain ramp, full pitch accumulator, TUNE, and release/kill lifecycle.
- The adapter neutralizes generic AMP and bypasses generic VOL/PAN only for this route. All retained modes preserve their original generic processing.
- m8 uses `fix5 BPM pitch+env full`; m9/m10 use `fix5 pitch+env full`.
- Schema <34 cannot activate the appended value; malformed/non-FM+ ID 8 values fall back to MNM.

## Existing UI calibration

FM+ PAR's retained default words still visibly read `1 / 1 / 1`. This is display/default calibration, not a retained DSP remap. The existing ID-6 raw-zero behavior remains candidate-only.

## Validation scope

The Pack-7 vector test passed in both product trees (9,556 checks, zero mismatches); static retained/isolation checks also passed. `Fix5HostIntegrationTests` is registered in CMake and syntax-audited but not run due lack of a configured build toolchain. No Windows VST3 or DAW test was performed here.

See `README_1.9.20.md`, `FM_EXPERIMENTS_README.md`, `VALIDATION_REPORT.md`, and the package-level audit for exact limits.


---

<a id="monomachine-nova-synth-validation-report-md"></a>
### `Monomachine_Nova_Synth/VALIDATION_REPORT.md`

`SHA-256: 2d405d1e5c80bbc2552f238d58d29765fab7b0246696289c5dd11947ba06c4ef`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_Synth/VALIDATION_REPORT.md`

---

# Отчёт о проверке — Monomachine Nova Synth 1.9.20

**Дата:** 29.09.2026  
**Статус:** исходники и автономные проверки пройдены; сборка VST3/DAW остаётся
внешним выпускным барьером.

## Подтверждено в этом дереве

- `python3 verify_dsp_mode_patch.py` завершился `FM_FIX5_STATIC_VERIFY PASS`.
  Он проверяет сохранённые хэши, ID/schema, изоляцию OLD FIX, Fix-5 и TRY4,
  маршрут AMP/VOL/PAN, версию 1.9.20 и контракт DSND.
- Автономные C++17-тесты с `-Wall -Wextra -Werror` прошли для Package-5 TRY4,
  Fix-5, delay/DSND, маршрутов фильтра и импортированных фильтров.
- `NovaDSP.h`, `PluginProcessor.cpp` и `PluginEditor.cpp` прошли синтаксический
  аудит против JUCE 8.0.12.
- Исправлен VST3 ODR-блокер TRY4: storage `MnmPanTables::s_sin`, `s_cos` и
  `s_built` теперь C++17 `inline`, поэтому две TU с `PluginProcessor` и
  `PluginEditor` не дают `LNK2005/LNK1169`. Строгий portable two-TU link
  regression после изменения — PASS; DSP-данные при этом не изменялись.

## Важные границы

TRY4 — отдельный Package-5 FM/AMP-frame путь, а не m6 и не старый клон.
Полный Package-8 `MnmVoiceFrame` проверен как точный routing-оракул, но не
интерпретируется в audio callback. DSND — Q23 моно-send: его LFO не меняет
сторону, фазу, банк или повторную атаку. Stage-2 использует отдельные слова
`$414..$417`; P1 FILT ATK/DEC не имеют права быть его swap/depth-алиасом.

## Не выполнено в этой среде

`cmake` отсутствует; не выполнялись CMake/CTest, сборка Windows/VS2022, VST3 и
DAW-прослушивание. Перед релизом обязательны эти проверки на 44,1/48/96 кГц,
включая recall schema 33/34, note-off/panic, tempo/HOLD, DSND/LFO и все
сохранённые FM-режимы.


---

## Номерные заметки продукта

<a id="monomachine-nova-fx-readme-1-9-0-md"></a>
### `Monomachine_Nova_FX/README_1.9.0.md`

`SHA-256: fe0f928532a42c23ac58977ce61d5a89092b7710fad661e455a502986db1249b`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.0.md`

---

# Monomachine Nova 1.9.0 — native DSP rollback

Дата документа: 26 сентября 2026. Этот выпуск для **FX** полностью
отменяет экспериментальные DSP-ветви предыдущего 1.9.0-пакета и возвращает
штатные пути. Аналогичное изменение выполнено в парном проекте.

## Что сохранено

- Native/default остаётся основным путём для P1 и P2.
- Оригинальный P2-список и `MachineEngine` сохранены без смены номеров,
  выбора или подстановки ядер.
- Для SYNT, FILT, DIST и DLY доступны только `mnm|old`.
- Для AMP доступны только прежние `old|mnm|vital`.
- Порядок обработки не менялся: `EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY`.
- BBOX продолжает использовать прежние native законы: firmware-specific флаги
  старта и питча отключены.

## Что удалено

- Все селекторы и UI-пункты `new`; пользовательский пункт FMA также удалён.
- Импортированные исходники, bridge-классы и тесты экспериментальных ветвей,
  включая source-derived осцилляторы, фильтр, envelope, DIST, SRR, delay и P2
  ядра.
- Больше нет параметра `p2_fx_mode`; P2 всегда идёт через штатный native путь.
- Не добавлялись guessed substitute DSP, автоматический limiter, дополнительное
  filter-darkening либо fabricated envelope/DIST hybrid.

## Совместимость состояний

Схема DSP-режимов имеет версию **17**. При загрузке старого состояния:

- значения режимов, прежде указывавшие на FMA или withdrawn source-derived
  ветви, нормализуются к `mnm`;
- прежнее AMP-значение `new` нормализуется к `mnm`;
- устаревший элемент `p2_fx_mode` удаляется из загруженного дерева состояния.

Поэтому старые проекты открываются без обращения к удалённому коду; при
необходимости сохранить точное экспериментальное состояние следует использовать
отдельную историческую копию проекта, а не этот release source package.

## Проверка поставки

`ModeRollbackTests` проверяет двухзначный реестр (`mnm|old`), отсутствие имён
withdrawn режимов, версию схемы и миграцию старых индексов в `mnm`. Полная
проверка сборки и тестов выполняется для Synth и FX перед упаковкой; результаты
публикуются рядом с архивом в release-заметке.

Исходники в `Source/dsp/mnm_new/`, `Source/dsp/mnm/new/` и
`MnmAmpDistNew.hpp` намеренно отсутствуют из этой поставки.


---

<a id="monomachine-nova-fx-readme-1-9-1-md"></a>
### `Monomachine_Nova_FX/README_1.9.1.md`

`SHA-256: 397fe5874a9af15ee01a48332b79263a8880040e60aebf2d4347937946e8059f`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.1.md`

---

# Monomachine Nova FX 1.9.1

See the package-level release notes at:

- `../RELEASE_1.9.1_MODE_S_AND_BLOCK_BYPASS.md`
- `../HYBRID_FILTER_AUDIT_2026-09-26.md`

FX-specific scope is identical to the paired Synth source project for MODE
L/H/S, classic-block bypass persistence, and schema-18 migration.

Key user-facing points:

- MODE S is the one audible DIST selection: `MNM`, `OLD`, `FOLD`, `ZERO`,
  `CLAMP`.
- MODE L and MODE H are ordered by lower/upper physical filter-side semantics.
- Every P1/P2 classic block has independent ON/OFF bypass without moving any
  physical FX-slot point.
- `MNM FIX` is deliberately deferred until the unresolved per-machine firmware
  handler transfer is bounded by evidence.

This project is source-only. The revision passed standalone and syntax checks;
it was not fully linked into a plugin binary in this environment.


---

<a id="monomachine-nova-fx-readme-1-9-10-md"></a>
### `Monomachine_Nova_FX/README_1.9.10.md`

`SHA-256: 37ace6ae51285e58826ee15c780be27d10ab727d31dd14261d9e31f162f5d6c4`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.10.md`

---

# Monomachine Nova FX 1.9.10

Source-only patch release. This FX package retains 1.9.8's DLY `mnm|old|new`
feedback path and 1.9.9's R-classic filters, then appends R Import 2 hybrid
families without moving existing IDs.

## Hybrid filter additions

`MODE L` / `MODE H` now contain compact `R IMPORT 2` groups for:

- `R ANALOG`, `R LINEAR` (LP/BP/HP at 12/24 poles);
- `R RBJ`, `R TPT` (LP/BP/HP);
- `R HUV`, `R HYPER`, `R KRAJ`, `R MICRO`, `R MUSIC`, `R OBER`, `R DVAL`.

MODE L keeps physical lower-side HP/low-cut ordering; MODE H keeps upper-side
LP/high-cut ordering. The nested groups keep the menu reachable at the lower
edge of the plug-in window.

## Saved state

Hybrid IDs `0..20` are frozen. IDs `21..51` are new and schema 25 records the
append. Pre-25 state never reinterprets malformed later values as a new sound.

## Source boundary

The requested `for import/2` collection is handled via independent,
self-contained equivalents because its framework dependencies and licensing
provenance are incomplete. See [`../R_IMPORT2_AUDIT_2026-09-27.md`](../R_IMPORT2_AUDIT_2026-09-27.md)
and [`../RELEASE_1.9.10_R_IMPORT2_FILTERS.md`](../RELEASE_1.9.10_R_IMPORT2_FILTERS.md).


---

<a id="monomachine-nova-fx-readme-1-9-11-md"></a>
### `Monomachine_Nova_FX/README_1.9.11.md`

`SHA-256: dcf3c619af2646961fca227ff34f4efcc54d318aa715904fabfc41208276f8d8`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.11.md`

---

> **SUPERSEDED / NOT AN ACCEPTED ROUTE:** This product note documents the
> rejected 1.9.11 automatic MNM-carrier policy. The active correction is
> [`README_1.9.12.md`](README_1.9.12.md).

# Monomachine Nova FX 1.9.11

Source-only filter-route repair release. It retains the 1.9.8 DLY
`mnm|old|new` feedback path, 1.9.9 R-classic choices, and 1.9.10 appended R
Import 2 IDs without moving any saved value.

## FILTER repair

`DSP FILT` may remain at its normal `old` default. Once either `MODE L` or
`MODE H` is non-`NATIVE`, FX automatically renders through the hybrid-capable
filter core, so the selected R/K35/Moog response is audible. With both
selectors at `NATIVE`, `old` remains the retained legacy filter.

`R HUV` now uses a bounded normalized independent ladder equivalent and `R
DVAL` no longer reverses at open cutoff. These changes target the previously
quiet, clipped, or garbled imported-filter behavior while keeping the existing
physical BASE/WDTH law.

## Menu

All R choices now live in one `R CLASSIC` popup branch: `R 303`, `R MS20`, `R
MOOG`, `R ANALOG`, `R LINEAR`, `R RBJ`, `R TPT`, and `R LADDERS`. The compact
cascade remains safe at the lower edge of the editor. Choice IDs stay exactly
as saved in 1.9.10.

## Saved state

The schema remains 25 and IDs `0..51` remain frozen. MODE L is the physical
lower / BASE edge; MODE H is the physical upper / BASE+WDTH edge. Intentional
mismatched responses can attenuate at extreme cutoff positions by design.

See [`../RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md`](../RELEASE_1.9.11_FILTER_ROUTE_REPAIR.md)
and [`../VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md`](../VALIDATION_1.9.11_FILTER_ROUTE_REPAIR.md).


---

<a id="monomachine-nova-fx-readme-1-9-12-md"></a>
### `Monomachine_Nova_FX/README_1.9.12.md`

`SHA-256: 40f8aaec80c4b107982ef999d2ea30700c4855c9a656276343b0079607c9ad28`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.12.md`

---

> **Superseded in this working tree by 1.9.13:** the original 1.9.12 archive remains unchanged. See [`README_1.9.13.md`](README_1.9.13.md).

# Monomachine Nova FX 1.9.12

Source-only independent physical-filter route correction.

## FILTER behaviour

`DSP FILT` is again limited to its two native alternatives while both `MODE L`
and `MODE H` are `NATIVE`:

- `old` runs the retained OLD native filter;
- `mnm` runs the retained MNM native filter.

When either MODE side selects K35, MOOG/ladder, R CLASSIC, or R Import 2, this
product renders the selected family through a separate
`IndependentPhysicalFilterCore`. The OLD renderer and MNM native renderer are
both excluded from that call, so there is no OLD+selected or MNM+selected
serial path. The selected side replaces its own native physical stage once;
the other side follows its own explicit MODE choice.

## Compatibility

Schema remains 25. MODE L/H IDs `0..51`, their BASE/WDTH physical direction,
the unified R CLASSIC popup, R HUV/R DVAL repairs, and DLY
`mnm|old|new`/DBAS/DWID/Q work are retained without parameter remapping.

See [`../HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md`](../HISTORICAL_FILTER_ROUTE_AUDIT_1.9.8_1.9.9.md),
[`../RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](../RELEASE_1.9.12_INDEPENDENT_FILTER_ROUTE.md),
and [`../VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md`](../VALIDATION_1.9.12_INDEPENDENT_FILTER_ROUTE.md).


---

<a id="monomachine-nova-fx-readme-1-9-13-md"></a>
### `Monomachine_Nova_FX/README_1.9.13.md`

`SHA-256: 644fe31a207900fdcc2f5a7681f3003fce3d0d980e95875d5fb19d134ada56b4`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.13.md`

---

# Monomachine Nova FX 1.9.13

Source-only physical-filter sound/control follow-up.

When a MODE L/H side selects K35, MOOG/ladder, R CLASSIC, or R Import 2, it
uses the separate `IndependentPhysicalFilterCore` route from 1.9.12. In 1.9.13
that selected route no longer receives the native MNM carrier's generic output
soft clip: resonance is not silently made into a common saturation stage.
`FILT SAT=0` remains an exact bypass; a selected model's own documented
nonlinear behaviour is not replaced by a global limiter.

Physical cutoff/resonance targets now ramp for 10 ms at the real sample rate.
K35 and ladder ignore repeated identical host targets while a ramp is running,
so a live BASE/WDTH move is not rearmed at the filter-envelope cadence.

NATIVE/NATIVE `DSP FILT=old|mnm`, schema 25, IDs 0..51, physical side semantics,
R CLASSIC menu, R HUV/R DVAL repairs, and DLY `mnm|old|new` / DBAS/DWID/Q are
retained. See
[`../RELEASE_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](../RELEASE_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md)
and
[`../VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](../VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md).


---

<a id="monomachine-nova-fx-readme-1-9-14-md"></a>
### `Monomachine_Nova_FX/README_1.9.14.md`

`SHA-256: 96ef58a2f707c63ed28d65291d2a35ae4494caa56859497cdea3235ed7b642ed`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.14.md`

---

# Monomachine Nova FX 1.9.14

Source-only follow-up: documented independent filter keyboard tracking and
compact envelope access. The same source changes are mirrored in the FX tree.

## Independent HPF / LPF key tracking

Two new P1 parameters and their P2 counterparts are binary switches:

- `filt_track_hpf` / `filt_track_lpf`
- `p2_filt_track_hpf` / `p2_filt_track_lpf`

They default to **ON**, matching normal Monomachine tracking. Each physical edge
can be disconnected independently.

With a switch on, the manual-derived target is:

```text
cutoff = played-note frequency / 4 * 2^(control / 8)
```

Thus HPF `BASE=0` begins two octaves below the played note and every `+8` BASE
units is an octave. LPF retains the existing Monomachine physical topology
`BASE + WDTH`: every `+8` BASE or `+8` WDTH units adds an octave to its edge.
The native MNM table core selects the closest retained table cutoff; the legacy
core calculates the same Hz law through its own coefficient path. Existing
VEL/KT amount parameters remain additive legacy modifiers; they are not these
new switches.

Schema 26 migrates states without these controls to both switches ON, preserving
the documented normal-tracking default.

## UI

- RMB on the FILT-page `BASE` or `WDTH` control opens `FILT / EXTRA` for the
  current P1/P2 faceplate.
- That compact panel contains visible `HPF KEYTRACK` and `LPF KEYTRACK`
  checkboxes. The older continuous `KT-L` / `KT-H` parameters remain serialized
  for old projects but are no longer presented as the normal tracking controls.
- RMB on the `FILT` category header no longer opens the extras panel.
- The main surface now reads `AMP`, `FIL ENV`, `MOD ENV`, then `MODE AMP`.
  Left-click `FIL ENV` or `MOD ENV` opens a compact duplicate that binds the
  same parameters as the retained full tab. RMB on its header button opens the
  retained full page; RMB on empty compact/full-page space switches the other
  direction, matching AMP-envelope navigation. The compact MOD ENV view keeps
  its selected source and exposes its `ROUTE` action.

## Validation performed on source

Standalone source tests passed for the real MNM filter (including the new
manual-law checks), existing filter-extra DSP behavior, and the production
`TrackFILT.inl` route integration. A legacy-core octave-equivalence smoke check
also passed. No JUCE plug-in build, DAW load, or interactive GUI test was run
for this source-only delivery.


---

<a id="monomachine-nova-fx-readme-1-9-15-md"></a>
### `Monomachine_Nova_FX/README_1.9.15.md`

`SHA-256: 3f45dbf283d19a742008ed5438afc3b7e1cb6dd92c9e650aa2462506411f97f2`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.15.md`

---

# Monomachine Nova FX 1.9.15 — source-only listening candidate

This tree is a **source-only candidate**. No ZIP archive, JUCE plug-in build,
DAW load, or interactive editor test has been claimed or produced yet. The
Synth and FX source changes are mirrored.

## P2 key tracking and native filter output

- Independent manual-derived `HPF KEYTRACK` and `LPF KEYTRACK` controls remain
  independent. Unchanged native parameter snapshots and unchanged MODE L/H
  selections now skip redundant coefficient rebuilds, which removes the
  pathological per-sample work reached by the one-sample P2 route.
- The native MNM filter no longer applies a hidden generic output soft clip.
  The DC blocker remains; intentional `FILT SAT` and `TRACK DIST` nonlinear
  paths remain separate. High-Q output is required to remain finite, but is
  allowed to exceed the former roughly ±2 knee.
- Filter-menu separator painting/height is repaired so dividers are visibly
  lines rather than blank selectable-looking gaps.

No broad filter-envelope or signal-order theory was introduced: the documented
route remains `headroom → EQ → FILTER → DISTORTION → AMP ENV → VOL/PAN → SRR →
DELAY → LEVEL`.

## MODE S A/B candidates

Existing serialized selections retain their IDs and behavior:

| ID | MODE S | Status |
|---:|---|---|
| 0 | `MNM` | retained reference |
| 1 | `OLD` | retained reference |
| 2–5 | `MNM FIX`, `FOLD`, `ZERO`, `CLAMP` | retained existing comparisons |
| 6 | `MNM+OLD` | appended candidate |
| 7 | `MNM V2` | appended candidate |
| 8 | `OLD V2` | appended candidate |

Schema 27 appends IDs 6–8 only. States written before schema 27 preserve valid
IDs 0–5; an impossible later value in an older state normalizes to `MNM` rather
than being interpreted as a new candidate. A schema-27 state retains each P1/P2
candidate independently.

All retained and appended transfer laws are exact unity at LCD `DIST=0` (raw
centre 64). Their positive side is deliberately different:

- **MNM+OLD** retains MNM's exact positive output/headroom compensation while
  substituting OLD's normalized `tanh` curve for the positive drive.
- **MNM V2** retains MNM's exact negative/centre path and hard pre-drive, but
  uses square-root positive output compensation. It isolates a level/headroom
  question from a soft-versus-hard curve question.
- **OLD V2** retains OLD's negative/centre path, then uses a stronger soft
  positive drive with a moderated positive output trim.

These are named comparison candidates, **not firmware-identity claims**. In
particular, the reported loud-but-under-distorted `OLD +63` behavior has not
been relabelled as solved. Compare references and candidates at matched output
levels before removing either `MNM` or `OLD`.

## Suggested listening matrix

For a sustained harmonic-rich source and a transient source, compare `MNM`,
`OLD`, `MNM+OLD`, `MNM V2`, and `OLD V2` at `DIST=0`, `+16`, `+32`, and `+63`.
Level-match after the block, then note (1) onset grit, (2) sustain compression,
(3) resonance interaction near the lower-filter region, and (4) output level.
Keep `FILT SAT` at zero for a first pass. This makes a `MODE S` conclusion
separate from intentional FILT saturation or downstream gain staging.

## Source validation performed

- `DistVariantTests`: exact zero identity, unchanged OLD reference law,
  negative headroom endpoints, candidate endpoint/curve distinction, and
  finite sweeps. `TrackDistVariantIntegrationTests` separately compiles the
  production `TrackDIST.inl` body and verifies exact dispatch of IDs 6–8.
- Existing standalone DSP targets passed in both trees, including `MnmCore`,
  `MnmRealFilter`, `HybridDsp`, `FilterRoute`,
  `TrackFilterRouteIntegration`, `Import2Filters`, `FilterExtras`,
  `DelayFeedback`, `ModEnvMatrix`, and `ArpWindow`.
- The project-local static verifier passed in both trees.

The JUCE modules required for a full plug-in/editor build were not available in
this environment. Therefore this document is not evidence of a compiled VST3,
DAW CPU percentage, GUI interaction test, or final sonic judgment.


---

<a id="monomachine-nova-fx-readme-1-9-17-md"></a>
### `Monomachine_Nova_FX/README_1.9.17.md`

`SHA-256: aaae1e02fddfd1e9ff7bb080cc1dda92bd477da9fcd3e91fcd1385aa01aab6fc`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.17.md`

---

# Monomachine Nova FX 1.9.17

## Меню MODE L / MODE H

- MODE L показывает только обычные `LOW CUT / HP`; MODE H — только `HIGH CUT / LP`. Противоположные response-типы не мешают обычному выбору.
- Все BP остаются в одной папке `BAND PASS`; для MODE L после неё добавлена последняя папка `DRY`.
- `R HYPER` находится последним внутри HP, LP и BP-папок; `R HYPER NOTCH` остаётся отдельным.
- `R HUV/KRAJ/MICRO/MUSIC/OBERHEIM/DVAL HP4` вынесены в MODE L → `DRY`. Это не обычные HP-реализации: пять используют `dry - LP`, DVAL — `dry + LP`. Удаление прямого входа не даст честный HP, поэтому они явно названы тестовой отдельной группой.
- Popup, колесо и drag используют одинаковую фильтрацию видимых пунктов.

## Проверка

В Synth и FX прошли JUCE-free C++17 `Import2FiltersTests`, `HybridDspTests` и `FilterRouteTests`; статический integrity-check также прошёл для обоих деревьев. Полная JUCE/VST3/DAW-сборка и слуховая проверка не выполнялись.

## Не заявляется

1.9.17 не исправляет и не объявляет решёнными native Monomachine FILT/Q/BOFS, AMP ENV или DIST.


---

<a id="monomachine-nova-fx-readme-1-9-2-md"></a>
### `Monomachine_Nova_FX/README_1.9.2.md`

`SHA-256: 5370b437cf09ae8c4625f529837a205eb35cbda2cc35364ac7f5d95265d2884b`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.2.md`

---

# Monomachine Nova FX 1.9.2

Package notes:

- `../RELEASE_1.9.2_MNM_FIX_AND_IMPORTED_DECLICK.md`
- `../HYBRID_FILTER_AUDIT_2026-09-26.md`

FX has the same schema-20 MODE L/H/S plus opt-in FILT/MOD ENV, classic-block bypass, experimental
MNM FIX, and 16-sample imported-side coefficient-ramp implementation as the
paired Synth source project.

MNM FIX is an explicitly experimental, separately selectable A/B candidate;
MNM and OLD remain unchanged. This is a source-only revision, validated by
standalone and syntax checks rather than a completed full plugin binary build.


---

<a id="monomachine-nova-fx-readme-1-9-20-md"></a>
### `Monomachine_Nova_FX/README_1.9.20.md`

`SHA-256: f747b374a4616ab34b8fe5c755b426839c604cdf95e889c5348fe0686db66af7`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.20.md`

---

# Monomachine Nova FX 1.9.20 — исходники

Это половина **FX** исходного пакета 1.9.20. Готовый VST3 здесь не
поставляется.

FM+ сохраняет ID 0…7 и добавляет независимый Fix-5 как ID 8/schema 34 только
для m8 STAT, m9 PAR и m10 DYN. Fix 4 остаётся отдельным. TRY4 (ID 7) теперь
активно использует собственный Package-5 FM/AMP-frame импорт в
`Source/dsp/fm_try4_voice/`; старый `fm_try4` удалён. Точный полный
Package-8 tail служит оракулом маршрутизации и не запускается интерпретатором
в audio callback; см. `../ROUTING_AUDIT_PACKAGE8_2026-09-29.md`.

В `Try4VoiceAmpEnv.hpp` storage PAN-таблиц сделано C++17 `inline`: это
необходимый ODR repair для VST3, чтобы `PluginProcessor.obj` и
`PluginEditor.obj` не давали `LNK2005/LNK1169`; сама DSP-математика не менялась.

DSND — прямой 0…127 Q23-уровень моно-посыла ColdFire. Он не является знаком
стерео, фазой или ping-pong-переключателем. LFO в DSND меняет только уровень
эхо-посыла.

## Hotfix Track Delay — 30.09.2026

Исправлен маршрут, из-за которого на свежем экземпляре одновременно работали
посыл P1 и полный скрытый путь P2: оба стартовали с `DSND=64`, поэтому хвост P2
маскировал изменения `P1 DSND` и `P1 DTIM` во всех режимах DLY.

* Новый P1 и P2 начинают с `DSND=0`; P2 выбирает прозрачный `FX-THRU`, а
  `P2 MIX=0`. Поэтому эффект появляется только после явного подъёма посыла и
  не подмешивается сам.
* При первом чтении прежнего состояния исправляется **только** точный старый
  заводской образ P2 (CHORUS/default words/`MIX=127`/`DSND=64`). Его P2
  переводится в `FX-THRU`, `P2 DSND=0`, `P2 MIX=0`. Сохранённый P1 и любой
  изменённый P2-параметр не переписываются; маркер
  `trackDelayDefaultRouteV1` делает операцию одноразовой.
* Регрессии покрывают свежие APVTS defaults, старое точное factory-state и
  сохранение custom P2-state в `tests/P2RoutingTests.cpp` и
  `tests/RollbackStateTests.cpp`.

Быстрая проверка в DAW: на новом экземпляре `DSND=0` не даёт хвоста; поднимите
`P1 DSND`, затем меняйте `P1 DTIM` — слышимое время эха должно меняться. Для
P2 сначала поднимите `P2 MIX` и его собственный `P2 DSND` намеренно.

Перед выпуском:

1. выполните `python3 verify_dsp_mode_patch.py`;
2. соберите проект через обычную Windows-цепочку Projucer/VS2022;
3. выполните CTest и тест VST3 в DAW на 44,1/48/96 кГц.

См. `FM_EXPERIMENTS_README.md`, `PROVENANCE.md` и `VALIDATION_REPORT.md`.


---

<a id="monomachine-nova-fx-readme-1-9-21-md"></a>
### `Monomachine_Nova_FX/README_1.9.21.md`

`SHA-256: caa931b207932067f821f628c848de19aad39251e1ecf90d53d673769a8df049`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.21.md`

---

# Monomachine Nova FX 1.9.21 — исходники

Это половина **FX** исходного пакета 1.9.21. Готовый VST3 здесь не
поставляется.

FM+ сохраняет ID 0…7 и добавляет независимый Fix-5 как ID 8/schema 34 только
для m8 STAT, m9 PAR и m10 DYN. Fix 4 остаётся отдельным. TRY4 (ID 7) использует
собственный Package-5 FM/AMP-frame импорт; полный Package-8 tail служит
маршрутным оракулом и не запускается интерпретатором в audio callback.

## Действующая коррекция Track Delay

Восстановлены исходные factory значения: P1 EFFX `DSND=64`, P2 EFFX
`DSND=64`, исходный P2 `FX-CHORUS` и `P2 MIX=127`. Marker-gated migration
`trackDelayDefaultRouteV1` удалена: сохранённые состояния P2 не меняются
автоматически.

DSND трактуется как масштаб verified host-side delay-route. Это **не** знак,
L/R-выбор, стерео-вход, фаза или ping-pong control. Левый/правый route не
имитируется через DSND: его перенос отложен до прямого подтверждения кодом и
vectors.

## Source-only UI/DSP коррекции

- filter mode popup сортируется по response/type, затем по имени, без смены
  сохранённых choice ID;
- HPF/LPF keytrack использует согласованные физические позиции BASE/WDTH, а
  P2 bypass не маскирует отключённый пользователем keytrack;
- MOD ENV имеет симметричный с AMP/FIL ENV large-tab handoff;
- верхние AMP/FIL ENV/MOD ENV кнопки корректно показывают selected-state после
  dock/overlay transition;
- held-drag по AMP/FIL/MOD tabs работает в large mode.

## Проверка

Запустите `python3 verify_dsp_mode_patch.py` из этой директории. Проверка
статическая и source-only; рендер, полная VST3-сборка и DAW-проверка здесь не
заявляются.

Подробности и статус отозванной старой гипотезы: в корне пакета
`TRACK_DELAY_CORRECTION_2026-09-30.md`.


---

<a id="monomachine-nova-fx-readme-1-9-3-md"></a>
### `Monomachine_Nova_FX/README_1.9.3.md`

`SHA-256: 60ac0e7f9b80bdedb57d7bcc9dfb888cb5238bf6c54e8b7dd00d7fd6e6c56b2f`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.3.md`

---

# Monomachine Nova FX 1.9.3

See [`../RELEASE_1.9.3_FILT_EXTRAS_ENV.md`](../RELEASE_1.9.3_FILT_EXTRAS_ENV.md).

This source-only FX revision adds independent P1/P2 FILT extras, one opt-in
post-FILT Korg/Odin `SAT` stage, additive FIL ENV through `BASE/WIDTH`, and
four MOD ENV matrix sources. `SAT=0` and `ENV FIL=0` are neutral; MNM and OLD
remain separate references. Schema 20 safely fills neutral values for older
projects.


---

<a id="monomachine-nova-fx-readme-1-9-8-md"></a>
### `Monomachine_Nova_FX/README_1.9.8.md`

`SHA-256: 0dcd82fbc41bdd81c834ccfa92deb26320f84f196ba875f70f302a325f6353f9`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.8.md`

---

# Monomachine Nova FX 1.9.8

This source release adds usable feedback-loop filtering to EFFX DLY and appends the opt-in `NEW` Track Delay mode while preserving `mnm` and `old` at their saved/automation IDs `0` and `1`.

- Select `MODE DLY` = `new` only when the new practical Track Delay candidate is desired.
- `DBAS` raises the feedback high-pass edge; reducing `DWID` lowers the feedback low-pass edge.
- Right-click `DBAS` or `DWID` on either P1/P2 EFFX DLY page for the matching optional `DBAS Q` / `DWID Q` settings.
- Existing states before schema 23 never silently turn a historic value `2` into `NEW`; they fall back to `mnm`.

See [`../RELEASE_1.9.8_TRACK_DELAY_NEW.md`](../RELEASE_1.9.8_TRACK_DELAY_NEW.md) for the exact scope, provenance, parameter IDs, compatibility rule, and validation boundary. See [`../VALIDATION_1.9.8_TRACK_DELAY_NEW.md`](../VALIDATION_1.9.8_TRACK_DELAY_NEW.md) for the completed checks and remaining JUCE/DAW validation.


---

<a id="monomachine-nova-fx-readme-1-9-9-md"></a>
### `Monomachine_Nova_FX/README_1.9.9.md`

`SHA-256: fef9cc9c2842c3a687557c0fbb407d159462c579918f3ee40efd84ac71018019`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/README_1.9.9.md`

---

# Monomachine Nova FX 1.9.9

Source-only patch release. This FX package retains 1.9.8's `DLY mnm|old|new`
feedback-filter work and adds the following hybrid physical-filter choices on
both P1 and P2:

- `R 303` — LP, BP, HP;
- `R MS20` — LP, BP, HP;
- `R MOOG` — LP12/LP24, BP12/BP24, HP12/HP24.

The old hybrid IDs `0..8` are untouched. The new responses append as IDs
`9..20` under schema 24. MODE L orders each filter family as HP → BP → LP;
MODE H orders it as LP → BP → HP, matching the lower/upper physical edges.
The popup is grouped and screen-aware so all choices remain reachable.

The source equations are an attributed, self-contained adaptation of the
user-supplied TB303/MS20/Moog files, not a claim that the incomplete upstream
`Filter.h` framework was compiled verbatim.

See [`../RELEASE_1.9.9_R_CLASSIC_FILTERS.md`](../RELEASE_1.9.9_R_CLASSIC_FILTERS.md)
for exact mapping, provenance, migration, safety behavior, and validation.


---

## Предыдущие product-level README и validation

<a id="monomachine-nova-fx-fm-experiments-readme-md"></a>
### `Monomachine_Nova_FX/FM_EXPERIMENTS_README.md`

`SHA-256: ddb3ebe10c70c6e599fe9d3a8bd6542941d87dc7d4a6834e85ca4e29489db3dc`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/FM_EXPERIMENTS_README.md`

---

# Monomachine Nova FX 1.9.20 — контракты кандидатов FM+

Текущий продукт: **FX**, версия проекта **1.9.20**.

## Реестр FM+ (без перенумерации)

| ID | Режим | Схема | Область |
|---:|---|---:|---|
| 0…5 | сохранённые режимы | прежняя | сохранённый контракт |
| 6 | `mnm frq env fix` | 32 | кандидат m8/m9/m10 |
| 7 | `try4` | 33 | отдельный Package-5 FM/AMP-frame кандидат m8/m9/m10 |
| 8 | `fix5 … pitch+env full` | 34 | отдельный Pack-7 кандидат m8/m9/m10 |

OLD FIX остаётся отдельным OLD-рендерером. +10 dB bridge остаётся только у
сохранённых NEW/NEW FIX. Видимое начальное `1 / 1 / 1` FM+ PAR — это
калибровка UI/дефолтов, а не изменение сохранённого DSP-закона.

## TRY4 (ID 7)

Активная реализация расположена в `Source/dsp/fm_try4_voice/`. Это не старый
`fm_try4` и не m6: путь имеет собственные Package-5 STAT/PAR/DYN-ядра, таблицы,
AMP/pitch/pan, `VoicePageMap`, нативный 16-семпловый frame renderer и lifecycle.
Полный Package-8 DSP-tail (FILT/EQ/delay/stage-2) не исполняется строковым
интерпретатором в audio callback; его точная карта и границы описаны в
`../ROUTING_AUDIT_PACKAGE8_2026-09-29.md`. Полное происхождение файлов
записано в `Source/dsp/fm_try4_voice/README_RU.md`.

## Fix-5 (ID 8)

`Source/dsp/fm_fix5/` содержит отдельный Pack-7 импорт. Нативный путь владеет
AMP, VOL², sin/cos PAN, 48-битным pitch, кадрами и release/kill. Общий host AMP
и VOL/PAN обходятся только для full-frame кандидатов, чтобы не умножать эти
законы второй раз. BPM переводится в `TickRecip=0x800000/(24*BPM)`.

## Проверка

`python3 verify_dsp_mode_patch.py` проверяет границы режимов, независимость
TRY4/Fix-5, неизменность сохранённых FM-ядер, версию и маршрут DSND. Полный
Windows/VST3/DAW барьер описан в `VALIDATION_REPORT.md`.


---

<a id="monomachine-nova-fx-stabilization-readme-md"></a>
### `Monomachine_Nova_FX/STABILIZATION_README.md`

`SHA-256: 7961cadaee1f3d4d761b3a02a94253180cef78528992efc2df771b813b9a695b`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/STABILIZATION_README.md`

---

# Monomachine Nova 1.9.20 — FM+ stabilization and independent Fix-5 (FX)

## Delivery baseline and versioning

The source lineage begins with the supplied 1.9.17 stable family and its Package-4 work. This delivery is intentionally branded/versioned **1.9.20**: the FX CMake project, `.jucer`, generated JUCE version metadata, build audit script, config, and visible editor build text were updated together.

## Retained behavior still in force

- IDs `mnm=0`, `old=1`, `new=2`, `mnm fix=3`, `new fix=4`, and `old fix=5` remain stable.
- OLD FIX keeps dedicated OLD STATIC/PAR/DYN topology; m8/STAT OLD FIX is not an MNM alias.
- The +10 dB output bridge remains exclusively on retained NEW and NEW FIX.
- OLD FIX FM+ PAR retains its centered `-64..+63` TUNE display and targeted ENV-index slew.
- Existing candidate ID 6 and TRY4 ID 7 retain their separate states, tables, and namespaces.

## New independent Fix-5 candidate

- **ID 8 / schema 34:** isolated upstream Fix-5 route for FM+ m8/m9/m10 only.
- Its source lives under `Source/dsp/fm_fix5/`; it is not a replacement for Fix 4.
- The route owns native 16-sample frame handling, AMP envelope, VOL²/pan gain ramp, full pitch accumulator, TUNE, and release/kill lifecycle.
- The adapter neutralizes generic AMP and bypasses generic VOL/PAN only for this route. All retained modes preserve their original generic processing.
- m8 uses `fix5 BPM pitch+env full`; m9/m10 use `fix5 pitch+env full`.
- Schema <34 cannot activate the appended value; malformed/non-FM+ ID 8 values fall back to MNM.

## Existing UI calibration

FM+ PAR's retained default words still visibly read `1 / 1 / 1`. This is display/default calibration, not a retained DSP remap. The existing ID-6 raw-zero behavior remains candidate-only.

## Validation scope

The Pack-7 vector test passed in both product trees (9,556 checks, zero mismatches); static retained/isolation checks also passed. `Fix5HostIntegrationTests` is registered in CMake and syntax-audited but not run due lack of a configured build toolchain. No Windows VST3 or DAW test was performed here.

See `README_1.9.20.md`, `FM_EXPERIMENTS_README.md`, `VALIDATION_REPORT.md`, and the package-level audit for exact limits.


---

<a id="monomachine-nova-fx-validation-report-md"></a>
### `Monomachine_Nova_FX/VALIDATION_REPORT.md`

`SHA-256: a9952dab588cc6adf8d957ad05aaa7e1e0e7acff25933cfa33de8f871b27e18b`

**Исходные пути с этим точным содержимым:**
- `Monomachine_Nova_FX/VALIDATION_REPORT.md`

---

# Отчёт о проверке — Monomachine Nova FX 1.9.20

**Дата:** 29.09.2026  
**Статус:** исходники и автономные проверки пройдены; сборка VST3/DAW остаётся
внешним выпускным барьером.

## Подтверждено в этом дереве

- `python3 verify_dsp_mode_patch.py` завершился `FM_FIX5_STATIC_VERIFY PASS`.
  Он проверяет сохранённые хэши, ID/schema, изоляцию OLD FIX, Fix-5 и TRY4,
  маршрут AMP/VOL/PAN, версию 1.9.20 и контракт DSND.
- Автономные C++17-тесты с `-Wall -Wextra -Werror` прошли для Package-5 TRY4,
  Fix-5, delay/DSND, маршрутов фильтра и импортированных фильтров.
- `NovaDSP.h`, `PluginProcessor.cpp` и `PluginEditor.cpp` прошли синтаксический
  аудит против JUCE 8.0.12.
- Исправлен VST3 ODR-блокер TRY4: storage `MnmPanTables::s_sin`, `s_cos` и
  `s_built` теперь C++17 `inline`, поэтому две TU с `PluginProcessor` и
  `PluginEditor` не дают `LNK2005/LNK1169`. Строгий portable two-TU link
  regression после изменения — PASS; DSP-данные при этом не изменялись.

## Важные границы

TRY4 — отдельный Package-5 FM/AMP-frame путь, а не m6 и не старый клон.
Полный Package-8 `MnmVoiceFrame` проверен как точный routing-оракул, но не
интерпретируется в audio callback. DSND — Q23 моно-send: его LFO не меняет
сторону, фазу, банк или повторную атаку. Stage-2 использует отдельные слова
`$414..$417`; P1 FILT ATK/DEC не имеют права быть его swap/depth-алиасом.

## Не выполнено в этой среде

`cmake` отсутствует; не выполнялись CMake/CTest, сборка Windows/VS2022, VST3 и
DAW-прослушивание. Перед релизом обязательны эти проверки на 44,1/48/96 кГц,
включая recall schema 33/34, note-off/panic, tempo/HOLD, DSND/LFO и все
сохранённые FM-режимы.


---

## Номерные package-level заметки

<a id="readme-1-9-14-source-package-md"></a>
### `README_1.9.14_SOURCE_PACKAGE.md`

`SHA-256: a5dc6df2b9424f5396a125ffbf3278cad8e043add47d1a60e5456f5fd968716b`

**Исходные пути с этим точным содержимым:**
- `README_1.9.14_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.14 source package

This archive is source-only and contains both maintained variants:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

Both trees include the same implementation changes for the 1.9.14 follow-up:
manual-law independent HPF/LPF filter key tracking, BASE/WDTH RMB access to the
compact filter-extra panel, and compact main-surface FIL ENV/MOD ENV access.
Each tree has its own `README_1.9.14.md` and a refreshed `SOURCE_BUILD.json`
manifest.

The package also retains the already-completed source corrections for MODE L/H
response grouping, the visible `R OBERHEIM LP4` name, and new matrix routes
appearing at the top.

## Building

No binaries, CMake build folders, JUCE checkout, object cache, or DAW artifacts
are included. To build, supply a JUCE 8 source checkout through `JUCE_DIR`, for
example:

```sh
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/path/to/JUCE
cmake --build build-synth --config Release
```

Use the analogous FX tree for the effects plug-in. The archive was delivered as
source because no full JUCE/VST3 or DAW runtime build was performed here.

See `RELEASE_1.9.14_KEYTRACKING_ENVELOPES.md` and
`VALIDATION_1.9.14_KEYTRACKING_ENVELOPES.md` for scope and source validation.


---

<a id="readme-1-9-15-source-package-md"></a>
### `README_1.9.15_SOURCE_PACKAGE.md`

`SHA-256: 052bb7e649ccdc8a4a119477bc39ade802d5a61d9af7a4c4cd25da6339d6e3cc`

**Исходные пути с этим точным содержимым:**
- `README_1.9.15_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.15 source package

This is a **source-only** package containing both maintained variants:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

Both trees carry the same 1.9.15 DSP, state-schema, UI-choice, test, and
source-audit changes. They identify themselves as version `1.9.15` in CMake,
Projucer metadata, generated JUCE metadata, UI build markers, and per-tree
source manifests.

## Scope

- P2 native HPF/LPF keytracking remains independently switchable, while
  unchanged filter snapshots and unchanged MODE L/H selections no longer
  rebuild coefficients for every one-sample P2 render slice.
- The hidden generic native-filter output soft clip was removed. The DC blocker
  remains; deliberate `FILT SAT` and post-filter `TRACK DIST` remain separate.
- Filter-choice divider drawing is repaired.
- MODE S retains all existing selections and appends comparison-only choices:
  `MNM+OLD` (ID 6), `MNM V2` (ID 7), and `OLD V2` (ID 8).
- Schema 27 protects states written before those appended IDs and preserves
  independent P1/P2 candidate selections in schema-27 states.

`MNM` and `OLD` remain listening references. The three new MODE S choices are
comparison candidates, not claims of firmware identity. See
`RELEASE_1.9.15_FILTER_CPU_DIST.md` for the exact intent and listening matrix.

## Build status

No binaries, JUCE checkout, object cache, CMake build tree, VST3, or DAW
artifacts are included. A full plug-in/editor build and DAW test were not
performed in this environment because the required JUCE modules were absent.

To build on a machine with JUCE 8 available:

```sh
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/path/to/JUCE
cmake --build build-synth --config Release
```

Use the analogous FX tree for the effect plug-in.

See `VALIDATION_1.9.15_FILTER_CPU_DIST.md` for the source-level checks that
were performed before packaging.


---

<a id="readme-1-9-16-source-package-md"></a>
### `README_1.9.16_SOURCE_PACKAGE.md`

`SHA-256: 9355895c3881d6bd5c5b20404cff404c713b207bf6b1d9b48ac8eb1a54b768a6`

**Исходные пути с этим точным содержимым:**
- `README_1.9.16_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.16 source package

This is a **source-only** package containing both maintained variants:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

## Included change

This package closes the requested MODE L/H extension-list omission without changing the native Monomachine filter:

- Existing MODE L/H IDs `0..51` remain unchanged.
- Schema `28` appends six derived HP complements at IDs `52..57`:
  `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`,
  `R OBERHEIM HP4`, and `R DVAL HP4`.
- Each new response is derived from its previously LP-only core. It is an extension feature, **not** a claim that the original Monomachine contains these imported filters.
- The DVAL complement is polarity-aware (`dry + LP`) because that recovered LP core has inverted output polarity; the other derived complements use `dry - LP`.
- MODE L/H popup grouping derives from actual response metadata, so the appended modes are shown as HP/low-cut responses rather than merely renamed.
- Synth and FX contain matching source and tests.

The native FILT/Q/BOFS behaviour reported in listening tests is **not claimed fixed by 1.9.16**. `FILTER_SOURCE_EVIDENCE_2026-09-28.md` is included as the bounded source-forensics record for that separate repair.

## Validation performed

For each variant, JUCE-free C++17 builds and runs passed for:

- `Import2FiltersTests`
- `HybridDspTests`
- `FilterRouteTests`

The checks cover response labels, appended-ID stability, finite/stereo behaviour, route exclusivity, and unchanged snapshot continuity. No complete JUCE/VST3/DAW build or listening validation was performed in this environment.

## Build

A JUCE 8 checkout is required to build either plug-in. No JUCE checkout, binary, CMake output, cache, or DAW artefact is included.

```sh
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/path/to/JUCE
cmake --build build-synth --config Release
```

Use the analogous `Monomachine_Nova_FX` command for the FX variant.


---

<a id="readme-1-9-17-fm-experiments-source-package-md"></a>
### `README_1.9.17_FM_EXPERIMENTS_SOURCE_PACKAGE.md`

`SHA-256: 593e31e924d26ec0c80bd0c8814f81fc47692bacab8cf0d8ac96b474b3d41cdd`

**Исходные пути с этим точным содержимым:**
- `README_1.9.17_FM_EXPERIMENTS_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.17 — FM+ package-4 source package

**Date:** 2026-09-29  
**Baseline:** supplied `Monomachine-Nova-1.9.17stable` source family only  
**Contents:** complete `Monomachine_Nova_Synth` and `Monomachine_Nova_FX` product trees.

## What changed

This package preserves all retained FM+ SYNT IDs and adds two FM+-only, append-only experimental routes:

- `mnm frq env fix` — ID 6, accepted from schema 32;
- `try4` — ID 7, accepted from schema 33.

ID 6 uses a separate imported core/table set with a true raw-zero frequency endpoint and a candidate-only continuous track-envelope repair. On a fresh explicit selection, only factory-default FRQ words are initialized to zero; custom words are preserved. ID 7 uses its own imported FM STAT/PAR/DYN implementation and does not reuse retained MNM/OLD/FM FIX DSP code.

The existing `mnm=0`, `old=1`, `new=2`, `mnm fix=3`, `new fix=4`, and `old fix=5` routes remain intact. The +10 dB bridge stays exclusive to NEW/NEW FIX. OLD FIX stays OLD (including dedicated STAT topology), not MNM.

## Product documentation

Read these in either product tree:

- `FM_EXPERIMENTS_README.md` — mode contracts, UI behavior, and Windows build instructions;
- `STABILIZATION_README.md` — retained/experimental scope boundaries;
- `VALIDATION_REPORT.md` — exact checks completed and limitations;
- `SOURCE_BUILD.json` — SHA-256 manifest for the delivered tree.

## Validation status

Both product trees passed the static verifier and five strict JUCE-free C++17 regression executables, including `FmExperimentsTests`. Processor/editor translation units also passed external-JUCE syntax-only parsing.

No Windows `.bat → Projucer → VS2022 Release|x64` build, actual VST3 artifact, or host/listening validation was possible in this Linux environment. Run that unchanged Windows pipeline before calling this VST3-build-validated.


---

<a id="readme-1-9-17-source-package-md"></a>
### `README_1.9.17_SOURCE_PACKAGE.md`

`SHA-256: 998d300a2a82b16e69dbd1e3b1d4667f023ee44a97f9d39304ad9f3b88ff9816`

**Исходные пути с этим точным содержимым:**
- `README_1.9.17_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.17 source package — current delivery

**Current delivery date:** 2026-09-29  
**Baseline:** supplied `Monomachine-Nova-1.9.17stable` source family only  
**Products:** matching `Monomachine_Nova_Synth` and `Monomachine_Nova_FX` trees

This worktree is the current ready-to-import **source** package. It supersedes earlier archive/candidate labels in the surrounding historical 1.9.17 notes; those files remain as evidence only and must not be read as current build-validation claims.

## FM+ package-4 additions

The retained FM+ SYNT registry stays stable through `old fix=5`, and two FM+-only modes are appended:

| ID | Name | First accepted schema |
|---:|---|---:|
| 6 | `mnm frq env fix` | 32 |
| 7 | `try4` | 33 |

- ID 6 has isolated imported FM state/tables, a literal raw-zero frequency endpoint, fresh-selection zero initialization for unmodified FRQ defaults, and a candidate-only continuous track-envelope repair.
- ID 7 is a separate imported STAT/PAR/DYN core and does not reuse retained MNM/OLD/FM FIX DSP implementation code.
- Retained IDs 0–5, OLD FIX topology, and the +10 dB bridge limited to NEW/NEW FIX remain protected.
- FM+ PAR displays its legacy raw defaults as `1 / 1 / 1`, without altering retained DSP laws.

## Start here

- [`README_1.9.17_FM_EXPERIMENTS_SOURCE_PACKAGE.md`](README_1.9.17_FM_EXPERIMENTS_SOURCE_PACKAGE.md) — package-level scope/build status.
- [`VALIDATION_1.9.17_FM_EXPERIMENTS.md`](VALIDATION_1.9.17_FM_EXPERIMENTS.md) — completed checks and remaining external gate.
- In either product tree: `FM_EXPERIMENTS_README.md`, `STABILIZATION_README.md`, `VALIDATION_REPORT.md`, and `SOURCE_BUILD.json`.

## Build status

The source/static checks documented above passed for both trees. A real Windows `.bat → Projucer → VS2022 Release|x64` VST3 build, VST3 artifact verification, and DAW/listening test were not possible in this Linux environment. That unchanged Windows pipeline is still required before a VST3-build-validated release claim.

## Historical records

Earlier `README_*`, `VALIDATION_*`, and archive-oriented notes preserve the prior retained-mode/filter evidence trail. They are not deleted, but the current delivery scope/status is defined by the documents listed above.


---

<a id="readme-1-9-20-fix5-source-package-md"></a>
### `README_1.9.20_FIX5_SOURCE_PACKAGE.md`

`SHA-256: 576cd4d332c90b30f633d921d3c0a4c3e385224c44389f82e2632b252e1544aa`

**Исходные пути с этим точным содержимым:**
- `README_1.9.20_FIX5_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.20 — исходный пакет

**Дата пакета:** 29.09.2026  
**Состав:** `Monomachine_Nova_Synth` и `Monomachine_Nova_FX`  
**Тип поставки:** исходники; VST3-бинарник не включён.

## Совместимость FM+

Реестр FM+ дополняется, а не перенумеровывается. Сохранены ID 0…7. Для
FM+ STAT (m8), PAR (m9) и DYN (m10) добавлен только ID 8 — `fix5 … pitch+env
full`; он принимается только состояниями со схемой **34**. В состояниях схемы
33 и ниже ID 8 безопасно возвращается к `mnm`. Fix 4 остаётся отдельным
режимом и не заменяется.

| ID | Режим | Условие |
|---:|---|---|
| 0…5 | сохранённые `mnm`/`old`/`new`/Fix 4/OLD FIX | прежнее |
| 6 | `mnm frq env fix` | схема 32, кандидат FM+ |
| 7 | `try4` | схема 33, отдельный Package-5 FM/AMP-frame кандидат |
| 8 | `fix5 … pitch+env full` | схема 34, только m8/m9/m10 |

## Изолированные новые пути

- `Source/dsp/fm_fix5/` — независимый импорт Fix-5/Pack-7 с собственными
  таблицами, состоянием, 16-семпловым кадром, AMP, VOL²/PAN и 48-битным pitch.
- `Source/dsp/fm_try4_voice/` — независимый импорт TRY4/Package-5. Он содержит
  собственные STAT/PAR/DYN-ядра, ROM, AMP/pitch/pan, отдельное состояние FM/AMP
  frame и `Try4VoicePageMap`; старое дерево `dsp/fm_try4/` удалено. Полный
  Package-8 DSP-tail не выдаётся за исполняемый TRY4-порт.
- В `Try4VoiceAmpEnv.hpp` storage таблиц PAN помечено `inline` по C++17:
  это устраняет VST3-линкерные `LNK2005/LNK1169` между `PluginProcessor.obj` и
  `PluginEditor.obj`, не меняя DSP-данные.
- Общий host-мост не повторяет AMP/VOL/PAN для этих нативных FM/AMP-frame путей.
  Сохранённые режимы не переписаны.

## Исправление посыла задержки

DSND трактуется как монофонический Q23-коэффициент ColdFire-посыла: `raw << 16`,
то есть 0…127 соответствует 0…127/128. Он не выбирает сторону, не меняет фазу
и не включает ping-pong. Точный Package-8 `MnmVoiceFrame` разделяет P1 FILT
ENV `$40C..$411` и самостоятельную native stage-2 группу `$414..$417`;
поэтому P1 FILT ATK/DEC не являются выбором delay-банка или глубины возврата.
Отдельный регрессионный тест проверяет, что LFO в DSND меняет только уровень
посыла. Полный разбор: `ROUTING_AUDIT_PACKAGE8_2026-09-29.md`.

## Проверка перед выпуском

Запускайте `python3 verify_dsp_mode_patch.py` из каталога каждого продукта.
Далее соберите оба проекта штатно в Windows через Projucer/VS2022, выполните
CTest и проверьте VST3 в DAW при 44,1/48/96 кГц: note-on/off/panic, HOLD/BPM,
VOL/PAN/TUNE/pitch-mod, DSND/LFO и переключение сохранённых режимов.


---

<a id="readme-1-9-20-runtime-integrated-source-package-md"></a>
### `README_1.9.20_RUNTIME_INTEGRATED_SOURCE_PACKAGE.md`

`SHA-256: 34e6394f2928bd2e35e2ab50215cc2175b8959fd27b6886292aedf6c40992025`

**Исходные пути с этим точным содержимым:**
- `README_1.9.20_RUNTIME_INTEGRATED_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.20 — runtime-integrated source package

Это один полный pre-patched source package. Импортируйте/собирайте:

`Monomachine_Nova_Synth/Monomachine Nova Synth.jucer`

Ваш существующий bat не нужно менять. После обычного Projucer resave он собирает VS2022 `Release|x64`; сохранённый в проекте `Directory.Build.targets` запускает bundled static runtime bridge перед линковкой VST3.

## Где находится runtime

Все требуемые исходники находятся внутри `Monomachine_Nova_Synth/ThirdParty/`:

- `MonomoduleCore/`
- `Dsp56300/`
- `NovaMonomoduleBridge/`

Bridge линкуется статически в VST3. Внешние локальные пути к Monomodule/dsp56300 и отдельная runtime DLL не требуются. Если static bridge не соберётся, Visual Studio должна завершить сборку ошибкой вместо успешного выпуска runtime-disabled VST3.

## Поведение firmware

- Выбор пользовательского OS SysEx остаётся в явной команде visible main `MENU`; отдельной нижней кнопки `OS SYSEX` нет.
- В state сохраняются только путь и SHA-256, а сам OS/SysEx и производные payload/PCM/WAV в пакет не включены.
- Бандл содержит actual whole-track Monomodule bridge для полного P1 synth и полного P2 FX selection.
- Частичная selection firmware sections не подменяется retained native renderer: она остаётся явно muted/unavailable (`FW MUTED`). Независимый firmware-stage execution FM/FILT/DIST/AMP/DLY/SRR не объявлен законченным этим пакетом.

`Monomachine_Nova_Synth/README_1.9.20.md` содержит точные source-level проверки и их границы. В доступной среде не выполнялись actual VS2022/MSVC Release build, VST3 host/DAW test, listening test или execution с реальным пользовательским OS SysEx.


---

<a id="readme-1-9-21-source-package-md"></a>
### `README_1.9.21_SOURCE_PACKAGE.md`

`SHA-256: 05b5b743f5fe6bf4b479191939a2ee6e5366e490ded80f9395b5307cd15587a1`

**Исходные пути с этим точным содержимым:**
- `README_1.9.21_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.21 — исходный пакет

Это source-only поставка двух продуктов:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Готовые VST3-бинарники в архив не включены. Версия **1.9.21** согласованно
задана в `.jucer`, CMake, generated JUCE metadata, UI build marker и
`Check-Build.ps1` обеих веток.

## Действующая коррекция 30.09.2026

После отзыва ошибочной Track Delay route-правки восстановлены исходные factory
значения в обоих продуктах:

- P1 EFFX `DSND=64`;
- P2 EFFX `DSND=64`;
- исходный P2 `FX-CHORUS` и `P2 MIX=127`;
- отсутствует миграция, переписывающая сохранённое P2-состояние в THRU.

DSND остаётся масштабом delay-route, а не ручкой левого/правого канала,
стерео-входа, знака, фазы или ping-pong. Реальный left/ping-pong путь не
подменён догадкой: перенос возможен только после полной сверки upstream-кода и
vectors.

В source также исправлены сортировка filter mode menu по типу и имени,
позиционирование/guard keytrack, parity MOD ENV с AMP/FIL ENV, selected-state
верхних envelope tabs и held-drag между AMP/FIL/MOD tabs в large mode.

## Документы и проверки

- `TRACK_DELAY_CORRECTION_2026-09-30.md` — действующий статус, границы и
  выполненные source-исправления;
- `VALIDATION_RUN_LOG_1.9.21_CORRECTION_SOURCE_ONLY.txt` — фактически
  выполненные узкие static/syntax/DSP проверки этой коррекции;
- `TRACK_DELAY_HOTFIX_2026-09-30.md` и
  `VALIDATION_RUN_LOG_1.9.21_TRACK_DELAY_HOTFIX.txt` сохранены как история
  отозванной гипотезы и не являются действующей спецификацией;
- `LEGACY_PATCH_READMES/README_RU.md` — возвращённые старые
  patch/readme/validation материалы в отдельном namespace без удаления или
  перезаписи;
- в каждом продукте: `README_1.9.21.md`, `README_RU.md`,
  `verify_dsp_mode_patch.py`, `SOURCE_BUILD.json`;
- `PACKAGE_MANIFEST_SHA256.txt` — SHA-256 всего source package.

Проверка этой поставки — source-only. Полная Windows Projucer/VS2022-сборка,
проверка VST3 и DAW-тест Synth/FX на 44,1/48/96 кГц остаются внешними gate для
бинарного выпуска.


---

## Номерные release и validation заметки

<a id="release-1-9-14-keytracking-envelopes-md"></a>
### `RELEASE_1.9.14_KEYTRACKING_ENVELOPES.md`

`SHA-256: 33775740f8162f128f91ab46b8657fdfcdf3a8c3f20c060a157f91f67b83353b`

**Исходные пути с этим точным содержимым:**
- `RELEASE_1.9.14_KEYTRACKING_ENVELOPES.md`

---

# 1.9.14 — filter key tracking and compact envelope access

## Implemented in both Synth and FX

### Manual-derived independent tracking

Four new state parameters are present:

- P1: `filt_track_hpf`, `filt_track_lpf`
- P2: `p2_filt_track_hpf`, `p2_filt_track_lpf`

They are binary, independently switchable HPF/LPF key-tracking controls and
default to ON. The target law is applied inside each filter core rather than by
forcing one raw-knob calibration onto both implementations:

```text
edge cutoff = played-note Hz / 4 × 2^(control / 8)
```

So HPF `BASE=0` is two octaves below the played note. `BASE +8` raises HPF by
one octave. LPF preserves the established physical `BASE + WDTH` topology, so
`+8` BASE or `+8` WDTH raises its edge one octave. HPF and LPF can be disabled
independently. Existing continuous `KT-L` / `KT-H` values remain legacy
additive modifiers, not replacements for these controls.

State schema is now 26. States that predate these parameters migrate both
switches to ON.

### Filter-extra interaction

- RMB on either visible FILT `BASE` or `WDTH` opens the compact `FILT / EXTRA`
  panel for the active P1/P2 faceplate.
- `HPF KEYTRACK` and `LPF KEYTRACK` are checkbox controls in that panel.
- RMB on the `FILT` category header no longer opens the panel.

### Envelope surface access

The main AMP header row now contains `AMP`, `FIL ENV`, `MOD ENV`, and `MODE`.

- Left-click `FIL ENV` or `MOD ENV` opens a compact view that binds the exact
  existing envelope parameters.
- RMB on the corresponding surface button opens the retained full tabbed page.
- RMB on empty compact or full envelope page space navigates between the two
  presentations, modeled after AMP.
- The compact MOD ENV view retains its source selection and includes its
  `ROUTE` action.

## Retained 1.9.14 source fixes

This package also includes the earlier source-only completed changes: stable
MODE L/H choice indices with response-led low-cut/high-pass and
high-cut/low-pass first folders, the visible `R OBERHEIM LP4` singleton label,
and promoted new patch-cord/matrix routes at GUI row zero.


---

<a id="release-1-9-15-filter-cpu-dist-md"></a>
### `RELEASE_1.9.15_FILTER_CPU_DIST.md`

`SHA-256: 95a92d4247b982361c99abc9fd96eda69d4e020ddea1bf8af9abab81e5c5b01a`

**Исходные пути с этим точным содержимым:**
- `RELEASE_1.9.15_FILTER_CPU_DIST.md`

---

# 1.9.15 — P2 filter CPU repair, native no-auto-clip, and MODE S comparison set

## P2 HPF/LPF keytracking CPU repair

The P2 route can reach filter setup in one-sample render slices. In the prior
source, an unchanged `setParameters()` snapshot still called `apply()`, and an
unchanged `setHybridTestModes()` snapshot did so too. With HPF and/or LPF
keytracking enabled, each unnecessary `apply()` also repeated tracking cutoff
lookup/coefficient work.

The controls remain independent and manual-derived. This revision only skips
rebuilding state when its input snapshot is unchanged. A real parameter, played
note, tracking-switch, or MODE L/H change still rebuilds the appropriate
filter state. It is therefore a removal of redundant control work, not a
smoothing ramp or a change to the filter law.

A deliberately pathological standalone hot-path measurement that called the
same parameter and MODE L/H snapshot for each of 44,100 samples changed from
1937.781 ms in 1.9.14 to 2.115 ms in this source revision. This demonstrates
that the redundant work was removed; it is not a DAW CPU-percentage claim.

## Native filter output

The former native `RealFilterCore` output safety transfer was a generic soft
clip with a nominal ceiling of 8 and an onset near absolute output 2. It was
not an intentional Monomachine saturation control. It is removed from the
native filter route.

The selected K35/ladder/R physical route was already dispatched to its
separate `IndependentPhysicalFilterCore` and did not receive that generic clip
in the normal production route. Thus removal can change native high-Q output,
but does not by itself explain or solve a complaint shared by all independent
filter choices. The selected physical route retains its own mathematical core
behavior; explicit `FILT SAT` and post-filter `TRACK DIST` remain separate.

## MODE S comparison choices

Existing serialized IDs retain their meaning:

| ID | Choice | Status |
|---:|---|---|
| 0 | `MNM` | retained reference |
| 1 | `OLD` | retained reference |
| 2–5 | `MNM FIX`, `FOLD`, `ZERO`, `CLAMP` | retained comparisons |
| 6 | `MNM+OLD` | appended 1.9.15 candidate |
| 7 | `MNM V2` | appended 1.9.15 candidate |
| 8 | `OLD V2` | appended 1.9.15 candidate |

All modes are exact unity at LCD `DIST=0`.

- `MNM+OLD`: exact current MNM negative/centre behavior and exact MNM positive
  output/headroom compensation, with OLD's normalized soft positive curve.
- `MNM V2`: exact current MNM negative/centre behavior and hard positive
  pre-drive, with square-root rather than current MNM output compensation.
- `OLD V2`: retained OLD negative/centre behavior, stronger soft positive drive,
  and moderated positive output trim.

These are explicit A/B choices, not a claim that a candidate equals firmware.
Neither `MNM` nor `OLD` should be removed before a listening decision.

## State and UI

Schema 27 appends IDs 6–8. A pre-schema-27 state preserves valid IDs 0–5; an
impossible future candidate ID in such an old state normalizes to `MNM` rather
than being repurposed as a new sound. Schema-27 states preserve P1/P2 choices
independently. The MODE S selector visibly exposes all nine choices in both
Synth and FX.

## Suggested listening matrix

Use a sustained harmonic-rich source and a transient source. Set `FILT SAT=0`
for the first pass, level-match after the block, and compare `MNM`, `OLD`,
`MNM+OLD`, `MNM V2`, and `OLD V2` at `DIST=0`, `+16`, `+32`, and `+63`.
Record onset grit, sustain compression, output level, and interaction with
filter resonance. This separates a MODE S transfer-law choice from filter
saturation or later gain staging.


---

<a id="validation-1-9-14-keytracking-envelopes-md"></a>
### `VALIDATION_1.9.14_KEYTRACKING_ENVELOPES.md`

`SHA-256: 5a133a6546317546e45e26fd7b9a6ff44ac1e5cf845eabdf9bc7890eafce8435`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.14_KEYTRACKING_ENVELOPES.md`

---

# 1.9.14 source validation

Validated on the final source trees for both `Monomachine_Nova_Synth` and
`Monomachine_Nova_FX`:

1. `verify_dsp_mode_patch.py Source` passed in each tree. The checker now
   verifies schema 26, both tracking parameters, state migration, TrackChain
   wiring, BASE/WDTH RMB ownership, absence of the FILT-category RMB action,
   and compact FIL/MOD envelope symbols.
2. Every `SOURCE_BUILD.json` inventory hash was refreshed and verified:
   159 entries in each variant.
3. The following standalone source tests were compiled and run successfully for
   Synth: `MnmCoreTests`, `DelayFeedbackDspTests`, `MnmRealFilterTests`,
   `HybridDspTests`, `FilterRouteTests`, `TrackFilterRouteIntegrationTests`,
   `Import2FiltersTests`, `FilterExtrasDspTests`, `ModEnvMatrixTests`,
   `ArpWindowTests`, and `ModeRollbackTests`.
4. `MnmRealFilterTests` now includes manual-law tracking assertions for
   `BASE=0`, `+8` octave steps, LPF `BASE+WDTH`, played-note octave movement,
   and independent HPF-off / LPF-on behavior.
5. A legacy-filter octave-equivalence smoke check passed. Targeting C4 with
   `BASE=8` matched C5 with `BASE=0` under key tracking.
6. The key filter/route tests (`DelayFeedbackDspTests`, `MnmRealFilterTests`,
   `TrackFilterRouteIntegrationTests`, `FilterExtrasDspTests`, and
   `ModeRollbackTests`) were also compiled and run from the FX tree.
7. The final ZIP was tested with `unzip -t` after creation.

## Not performed

This is not a full plug-in build validation. No JUCE checkout was available in
the source package, and no VST3/DAW load or interactive GUI session was run.
No claim is made that a DAW has exercised the new controls; the delivered
artifact is full source plus the source-level validation above.


---

<a id="validation-1-9-15-filter-cpu-dist-md"></a>
### `VALIDATION_1.9.15_FILTER_CPU_DIST.md`

`SHA-256: 9054ad8d38f5fb5b231f910673ae4a7f7666d960c14682eac85a3772d1580597`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.15_FILTER_CPU_DIST.md`

---

# 1.9.15 source validation

Validated on both final source trees:

- `Monomachine_Nova_Synth`
- `Monomachine_Nova_FX`

## Static and source-audit validation

1. `verify_dsp_mode_patch.py` passed in both trees.
2. Each refreshed `SOURCE_BUILD.json` SHA-256 inventory was verified after all
   edits: 162 entries in Synth and 162 entries in FX.
3. Mirrored production DSP/UI/schema files and new standalone test sources were
   compared byte-for-byte between Synth and FX where their product identity
   does not intentionally differ.
4. Source/tests were checked for removed generic native clip symbols:
   no `real_detail::softClip` or `kOutCeiling` remains.
5. CMake, `.jucer`, generated JUCE metadata, and visible build markers identify
   the source candidate as `1.9.15`.

## Standalone C++17 DSP targets

The following targets were compiled with `g++ -std=c++17 -O2 -I Source` and run
successfully in **both** source trees:

1. `MnmCoreTests`
2. `DelayFeedbackDspTests`
3. `DistVariantTests`
4. `TrackDistVariantIntegrationTests`
5. `MnmRealFilterTests`
6. `HybridDspTests`
7. `FilterRouteTests`
8. `TrackFilterRouteIntegrationTests`
9. `Import2FiltersTests`
10. `FilterExtrasDspTests`
11. `ModEnvMatrixTests`
12. `ArpWindowTests`

`DistVariantTests` verifies exact zero identity, preservation of the old
reference law, candidate endpoint/curve behavior, and finite sweeps.
`TrackDistVariantIntegrationTests` compiles the production `TrackDIST.inl`
body with its real MODE S enum and verifies exact dispatch for IDs 6–8.

## Not performed

No full JUCE plug-in/editor build, VST3 load, DAW session, interactive GUI
check, or final listening judgment was performed in this environment. The
package is source plus source-level validation; it is not evidence of a
compiled binary or a DAW CPU percentage.


---

<a id="validation-1-9-16-hp-complements-md"></a>
### `VALIDATION_1.9.16_HP_COMPLEMENTS.md`

`SHA-256: a87d62b45015fbd3c6df1f7e19b03fab87cf67b8ad9d7ff87e56ba3cddfb9478`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.16_HP_COMPLEMENTS.md`

---

# 1.9.16 validation — appended R HP complements

## Scope

This validation covers only the appended MODE L/H extension modes for the six R Import 2 cores that previously exposed LP output only. It does not claim a correction of the native Monomachine FILT/Q/BOFS implementation.

## Contract checked

- IDs `0..51` retain their prior MODE L/H mapping.
- IDs `52..57` append, without reordering, `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`, `R OBERHEIM HP4`, and `R DVAL HP4`.
- Schema `<28` rejects out-of-era `52..57` values rather than reinterpreting them; schema `28` permits them.
- HP output is the explicitly derived complement of the LP-only core. DVAL uses `dry + LP` because its LP output polarity is inverted; the remaining cores use `dry - LP`.
- Both Synth and FX source trees contain byte-identical implementation/test changes in the relevant files.

## Performed checks

The following JUCE-free C++17 targets were compiled and run separately in both source trees:

1. `Import2FiltersTests`
2. `HybridDspTests`
3. `FilterRouteTests`

All passed. Tests include observable low/high response checks for the six appended HP entries, selector/map stability, finite/stereo tests, repeat-snapshot continuity, and independent physical-route exclusivity.

## Not performed

- Full JUCE project/editor build.
- VST3/AU/DAW load.
- Subjective listening comparison.
- Any claim that imported extension models equal Monomachine firmware.
- Any resolution of the separate native FILT/Q/BOFS issue.


---

<a id="validation-1-9-17-fm-experiments-md"></a>
### `VALIDATION_1.9.17_FM_EXPERIMENTS.md`

`SHA-256: 905f975aa6eaad0e1f14e8a4d4e3ce607dad5e8beedb3e58e7251fd964482d29`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.17_FM_EXPERIMENTS.md`

---

# Package-level validation record — FM+ experiments

Date: 2026-09-29

## Passed in both product trees

- `python verify_dsp_mode_patch.py`
- strict C++17 direct compile/run:
  - `FmNewModeTests`
  - `FmFixModeTests`
  - `DelayFeedbackDspTests`
  - `ModeRollbackTests`
  - `FmExperimentsTests`
- `g++ -fsyntax-only` of `PluginProcessor.cpp` and `PluginEditor.cpp`, using temporary external JUCE 9.0.3 public headers and product-generated plugin defines.

## Not passed/not attempted as a release claim

- CMake configure/build: CMake is not installed in this environment.
- Projucer/VS2022 Windows VST3 build: unavailable here.
- Actual VST3 artifact inspection and DAW/listening test: unavailable here.

The Windows build remains the final required acceptance gate. Details and boundaries are repeated in each product's `VALIDATION_REPORT.md`.


---

<a id="validation-1-9-17-fm-legacy-isolation-md"></a>
### `VALIDATION_1.9.17_FM_LEGACY_ISOLATION.md`

`SHA-256: b8cece514d04f5c9da9d4078542ad7b42c879dc13f32ebf2bab4a228e1dfe67f`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.17_FM_LEGACY_ISOLATION.md`

---

> **Historical retained-mode record (2026-09-28).** This document records the ID 0–5 legacy-isolation repair only. The current 2026-09-29 delivery additionally appends package-4 IDs 6/7. For current validation status and limits, read [`VALIDATION_1.9.17_FM_EXPERIMENTS.md`](VALIDATION_1.9.17_FM_EXPERIMENTS.md) and each product's `VALIDATION_REPORT.md`. Claims below about the earlier candidate/archive and its target count are not the current release claim.

# 1.9.17 FM+ legacy-isolation repair validation — 2026-09-28

## Goal

Repair the FM+ FIX UI/readout path without changing retained `mnm`, `old`, or
`new` default knobs, source core, state, or sound route. This document is an
evidence record for the accompanying source package; it is not a claim of a
full JUCE/VST3/DAW/listening validation.

## Retained source-byte evidence

The following source files are restored from
`Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` in **both** Synth and FX:

| retained route | file | SHA-256 |
| --- | --- | --- |
| `mnm` | `Source/dsp/mnm/MnmFm.hpp` | `2addde732ce2d1550018e1450ebd370cd5a3c0290dfb9604e3c3c80cb859a23d` |
| `new` wrapper | `Source/dsp/fm_new/FmExactNew.hpp` | `9271249e7433c85961d32e28e4b4ee6b457dc1e8d50d0268d8246dcc8adafa3b` |
| `new` raw DSP | `Source/dsp/fm_new/FmExactDsp.hpp` | `64e2adbbd2ac0f3d6d859514ea98599de24757f92ca5531e150fa051d2cab0d2` |

The static verifier hashes these retained files, as well as the pre-existing
legacy FM source headers, in both product trees.

## Default/raw profile isolation

The global descriptor defaults are restored to the retained baseline:

```text
m8 STAT: {16,64,0,0,32,64,64,64}
m9 PAR:  {16,64,32,64,48,64,64,64}
m10 DYN: {16,0,64,0,32,80,30,64}
```

The previously recovered FIX profiles remain in
`Source/dsp/fm_fix/FmFixTables.hpp` only:

```text
m8 STAT: {60,64,80,30,80,64,98,64}
m9 PAR:  {60,64,80,64,102,80,98,64}
m10 DYN: {64,64,64,64,74,80,30,64}
```

They are never APVTS/machine defaults, migration writes, or mode-switch writes.
The only write path is the intentionally named UI command
`LOAD FIX FACTORY RAW VALUES (explicit)`, exposed only while `mnm fix` or
`new fix` is selected for m8/m9/m10.

## Separate core/state topology

| SYNT choice | core/state object | retained route shared? |
| --- | --- | --- |
| `mnm` | `monomachine::mnm::FmCore mnmFm` | no FIX API/table in retained source |
| `mnm fix` | `monomachine::fm_fix::MnmFixCore mnmFmFix` | no |
| `new` | `monomachine::fm_new::FmExactCore fmNew` | no FIX API/table in retained source |
| `new fix` | `monomachine::fm_new::FmExactFixCore fmNewFix` | no; raw copy is in `fmnewfix` namespace |

`NovaDSP.h` resets, notes, bends, and renders each object separately. Selecting
a FIX mode therefore cannot leave a control-law/table pointer or envelope state
inside a retained core.

## UI/readout isolation

`DspModes.hpp` gates `dspSyntModeUsesMeasuredFix()` to raw SYNT IDs 3 and 4
only. `PluginEditor.cpp` checks that selected machine-local mode before
formatting measured STAT/PAR/DYN/TUNE output. Retained raw IDs 0/1/2 (`mnm`,
`old`, `new`) retain their original `getFmListedRatio(raw/4)`, `v/16`,
`pow(2,(v-32)/24)`, and `raw-64` display paths; they do not fall through to a
new generic raw-`0..127` display.

## Completed checks

For **both Synth and FX**:

1. `verify_dsp_mode_patch.py` passed, including retained source-hash checks,
   core/state separation checks, FIX-only UI gating, original retained
   formatter ordering, and explicit-profile checks.
2. The literal original `PluginEditor.cpp` FM formatter fragments were compared
   against `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` and are present in
   both products behind the FIX gate.
3. All 16 JUCE-free C++17 CMake targets compiled and passed:
   `NovaChorusTests`, `MnmCoreTests`, `DelayFeedbackDspTests`,
   `DistVariantTests`, `TrackDistVariantIntegrationTests`,
   `MnmRealFilterTests`, `HybridDspTests`, `FilterRouteTests`,
   `TrackFilterRouteIntegrationTests`, `Import2FiltersTests`,
   `FilterExtrasDspTests`, `ModEnvMatrixTests`, `ArpWindowTests`,
   `FmNewModeTests`, `FmFixModeTests`, and `ModeRollbackTests`.
4. Product-equivalent changed source/test/verifier files were compared
   byte-for-byte between Synth and FX.
5. Both `SOURCE_BUILD.json` manifests were regenerated with 180 files and
   independently rehashed successfully.

The C++17 command emitted pre-existing Chorus unused-variable warnings; no
validation command failed.

## Not completed / deliberately not claimed

- No full JUCE/editor/VST3 build: a compatible JUCE checkout is not present.
- No host/DAW state load, automated plug-in render, or subjective listening
  comparison.
- No universal hardware-identical claim for the measured FIX candidate,
  particularly the approximate MNM DYN topology.


---

<a id="validation-1-9-17-mode-menu-dry-md"></a>
### `VALIDATION_1.9.17_MODE_MENU_DRY.md`

`SHA-256: 1cdc419c319c9558229d5a2d22ceaff7b0384508b376b1857b25e2a563ba89ad`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.17_MODE_MENU_DRY.md`

---

# 1.9.17 validation — MODE L/H, DRY and Hyperion ordering

## UI contract

- MODE L normal selection consists of HP/low-cut, NOTCH, BAND PASS, then final DRY.
- MODE H normal selection consists of LP/high-cut, NOTCH, then final BAND PASS.
- Each response folder puts ordinary families first and Hyperion last.
- DRY contains only the six explicitly direct-input LP-derived HP diagnostic modes and is MODE L-only.
- Popup, wheel, and drag share the visible-item gate, so hidden inverse response types cannot be reached through ordinary browsing.

## Performed validation

- Static integrity checker passed for Synth and FX.
- `Import2FiltersTests`, `HybridDspTests`, and `FilterRouteTests` compiled and passed in each tree.
- Synth/FX modified source/test/verifier copies are byte-identical.
- Product `SOURCE_BUILD.json` manifests were regenerated and verified.

## Not performed

No complete JUCE UI build, plug-in render, host/DAW state load, or listening test was performed here.


---

<a id="validation-1-9-20-fix5-md"></a>
### `VALIDATION_1.9.20_FIX5.md`

`SHA-256: 6d4cdfdf7ceefa15ae8d90ee96810ece3bb186dbb3413db51808286bfec069d4`

**Исходные пути с этим точным содержимым:**
- `VALIDATION_1.9.20_FIX5.md`

---

# Проверка исходного пакета 1.9.20

**Дата:** 29.09.2026  
**Продукты:** Synth и FX

## Выполнено

Сначала по указанному upstream-пути выполнен векторный Package-8 прогон
`MnmVoiceFrame`: **392824 checks, 0 mismatches**. Он подтвердил раздельные
адреса P1 FILT ENV `$40C..$411` и native stage-2 `$414..$417`; подробности —
в `ROUTING_AUDIT_PACKAGE8_2026-09-29.md`.

После этой сверки в обоих продуктах успешно завершились:

```text
python3 verify_dsp_mode_patch.py
FM_FIX5_STATIC_VERIFY PASS
```

Проверены автономные C++17-тесты с `-Wall -Wextra -Werror`, включая:

- `FmExperimentsTests` — Package-5 TRY4 STAT/PAR/DYN, page map, stereo/release,
  callback split и изоляция;
- `FmFixModeTests`;
- `DelayFeedbackDspTests` — моно DSND Q23, отсутствие изменения стороны/банка
  при LFO в DSND и отсутствие ложного P1 FILT ATK/DEC -> stage-2 alias;
- `MnmCoreTests`;
- `FilterRouteTests`, `TrackFilterRouteIntegrationTests`, `MnmRealFilterTests`,
  `FilterExtrasDspTests`, `Import2FiltersTests`.

Также `NovaDSP.h`, `PluginProcessor.cpp` и `PluginEditor.cpp` обоих продуктов
прошли `g++ -fsyntax-only` с внешним JUCE **8.0.12**, соответствующим версии
Projucer в доставленных заголовках.

## Исправление ODR линкера VST3

В присланном Windows-логе прежнее состояние падало при линковке обоих VST3 с
`LNK2005`/`LNK1169`: storage `try4voicefm::MnmPanTables::{s_sin,s_cos,s_built}`
был определён из одного header в `PluginProcessor.obj` и `PluginEditor.obj`.
Во всех двух product headers определения переведены в C++17 `inline`;
таблицы, их размер и DSP-математика не изменены.

После ремонта выполнен отдельный строгий C++17 two-translation-unit regression:
две единицы трансляции независимо включают TRY4 header, обращаются к обеим
таблицам и линкуются с `-Wall -Wextra -Werror`. Результат: **PASS** для Synth и
FX. Static verifier обоих продуктов также требует все три `inline`-определения.

## Не выполнено здесь

В среде нет `cmake`, Windows/VS2022 и DAW. Поэтому не утверждается, что
выполнены: конфигурация CMake, CTest, линковка processor-level тестов,
сборка VST3 или прослушивание в хосте. В частности Windows VST3 следует
пересобрать именно после этого repair: текст wrapper-а «MSBuild: сборка
успешна» недостаточен, если в логе присутствуют `LNK2005`/`LNK1169` или VST3
binary имеет 0 B.

## Обязательный выпускной барьер

Собрать Synth и FX штатным Windows-процессом, выполнить CTest и проверить в
DAW 44,1/48/96 кГц. Отдельно проверить сохранение схем 33/34, m8/m9/m10,
TRY4, Fix-5, DSND с LFO, note-off/panic, tempo/HOLD и сохранённые режимы.


---

## Уникальные legacy patch/validation заметки

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17stable-user-base-readme-1-9-17-source-package-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17stable-user-base/README_1.9.17_SOURCE_PACKAGE.md`

`SHA-256: 2dd95eae0effc462b28ac7f218f76ccb6f3f81296b429ece9298964c447c2102`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17stable-user-base/README_1.9.17_SOURCE_PACKAGE.md`

---

# Monomachine Nova 1.9.17 source package

This source-only package contains matching Synth and FX trees. The exact visible MODE L/H lists are included in `CURRENT_MODE_L_MODE_H_1.9.17.md`.

> **Legacy-isolation repair note (2026-09-28):** `Monomachine-Nova-1.9.17.zip` and `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` remain preserved. The earlier `POST-FM-FIX-source` and `POST-FM-FIX-UI-ISOLATION-source` archives are preserved for evidence but are **not delivery candidates**: the latter accidentally made recovered FIX words global descriptor defaults, changing fresh/reset retained-mode knobs and sound. This worktree restores the retained `MnmFm.hpp`, `FmExactNew.hpp`, and `FmExactDsp.hpp` source bytes from the candidate baseline. `mnm fix` now uses `MnmFixCore`; `new fix` uses a separately namespaced `FmExactFixCore`; neither shares a core/state object with retained `mnm` or `new`. Measured LCD FREQ/TUNE labels apply only to `mnm fix` / `new fix` for m8/m9/m10. Recovered FIX raw profiles are available only through the explicit `LOAD FIX FACTORY RAW VALUES (explicit)` action while a FIX mode is selected; they are never global defaults or automatic mode-switch writes. The delivery candidate is named `Monomachine-Nova-1.9.17-FM-LEGACY-ISOLATION-source-2026-09-28.zip`; it does not claim a full JUCE/DAW/listening-validated release. `new fix` remains the corrected-DYN-`2FRQ` listening candidate; the retained approximate MNM DYN route does not audibly mix its generated `mod2` path. `FM_NEW_AUDIT_2026-09-28.md` states the scope and validation boundary. A separate pre-FIX, route-labelled agent-handoff archive accompanies this delivery for delegated review.

## Included UI/menu layout

- MODE L exposes the normal low-cut/HP family; MODE H exposes the normal high-cut/LP family.
- `R HYPER` modes are deliberately deferred to the end of their HP/LP/BP response folders.
- All band-pass modes are grouped in a single `BAND PASS` folder.
- The six LP-derived, direct-input HP diagnostics are explicit only in the final MODE L `DRY` folder:
  `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`, `R OBERHEIM HP4`, and `R DVAL HP4`.
  Five use `dry - LP`; DVAL uses `dry + LP` because its LP output polarity is inverted. Removing dry would not create an honest HP response.
- Popup, wheel, and drag use the same visible-choice gate. Opposite response types do not appear in normal MODE L/H selection.

The underlying 0..57 algorithm IDs remain available for state/diagnostics; this UI package does not claim a native Monomachine filter repair.

## Validation performed

The following JUCE-free C++17 tests passed separately for Synth and FX:

- `NovaChorusTests`, `MnmCoreTests`, `DelayFeedbackDspTests`, `DistVariantTests`, and `TrackDistVariantIntegrationTests`
- `MnmRealFilterTests`, `HybridDspTests`, `FilterRouteTests`, and `TrackFilterRouteIntegrationTests`
- `Import2FiltersTests`, `FilterExtrasDspTests`, `ModEnvMatrixTests`, and `ArpWindowTests`
- `FmNewModeTests`, `FmFixModeTests`, and `ModeRollbackTests`

Static integrity checks also passed for both products after the manifests were regenerated. No full JUCE/editor/VST3/DAW build or subjective listening validation was performed.

`FM_FIX_UI_VALUE_MAP_2026-09-28.md` is the explicit m8/m9/m10 FIX-only LCD/list mapping for delegated UI review. `FM_RETAINED_READOUT_ROUTES_2026-09-28.md` records the literal original m8/m9/m10 formatter branches for retained `mnm`/`old`/`new`, side by side with their final locations. `VALIDATION_1.9.17_FM_LEGACY_ISOLATION.md` records the separate-core/default/source-hash evidence for this repair.

No JUCE checkout, binary, build directory, or cache is included.


---

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17stable-user-base-validation-1-9-17-fm-legacy-isolation-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17stable-user-base/VALIDATION_1.9.17_FM_LEGACY_ISOLATION.md`

`SHA-256: c40cc91a305f30ae0bca9eb61005377a2b572766234e59bd9fa5e1ea9bdaf104`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17stable-user-base/VALIDATION_1.9.17_FM_LEGACY_ISOLATION.md`

---

# 1.9.17 FM+ legacy-isolation repair validation — 2026-09-28

## Goal

Repair the FM+ FIX UI/readout path without changing retained `mnm`, `old`, or
`new` default knobs, source core, state, or sound route. This document is an
evidence record for the accompanying source package; it is not a claim of a
full JUCE/VST3/DAW/listening validation.

## Retained source-byte evidence

The following source files are restored from
`Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` in **both** Synth and FX:

| retained route | file | SHA-256 |
| --- | --- | --- |
| `mnm` | `Source/dsp/mnm/MnmFm.hpp` | `2addde732ce2d1550018e1450ebd370cd5a3c0290dfb9604e3c3c80cb859a23d` |
| `new` wrapper | `Source/dsp/fm_new/FmExactNew.hpp` | `9271249e7433c85961d32e28e4b4ee6b457dc1e8d50d0268d8246dcc8adafa3b` |
| `new` raw DSP | `Source/dsp/fm_new/FmExactDsp.hpp` | `64e2adbbd2ac0f3d6d859514ea98599de24757f92ca5531e150fa051d2cab0d2` |

The static verifier hashes these retained files, as well as the pre-existing
legacy FM source headers, in both product trees.

## Default/raw profile isolation

The global descriptor defaults are restored to the retained baseline:

```text
m8 STAT: {16,64,0,0,32,64,64,64}
m9 PAR:  {16,64,32,64,48,64,64,64}
m10 DYN: {16,0,64,0,32,80,30,64}
```

The previously recovered FIX profiles remain in
`Source/dsp/fm_fix/FmFixTables.hpp` only:

```text
m8 STAT: {60,64,80,30,80,64,98,64}
m9 PAR:  {60,64,80,64,102,80,98,64}
m10 DYN: {64,64,64,64,74,80,30,64}
```

They are never APVTS/machine defaults, migration writes, or mode-switch writes.
The only write path is the intentionally named UI command
`LOAD FIX FACTORY RAW VALUES (explicit)`, exposed only while `mnm fix` or
`new fix` is selected for m8/m9/m10.

## Separate core/state topology

| SYNT choice | core/state object | retained route shared? |
| --- | --- | --- |
| `mnm` | `monomachine::mnm::FmCore mnmFm` | no FIX API/table in retained source |
| `mnm fix` | `monomachine::fm_fix::MnmFixCore mnmFmFix` | no |
| `new` | `monomachine::fm_new::FmExactCore fmNew` | no FIX API/table in retained source |
| `new fix` | `monomachine::fm_new::FmExactFixCore fmNewFix` | no; raw copy is in `fmnewfix` namespace |

`NovaDSP.h` resets, notes, bends, and renders each object separately. Selecting
a FIX mode therefore cannot leave a control-law/table pointer or envelope state
inside a retained core.

## UI/readout isolation

`DspModes.hpp` gates `dspSyntModeUsesMeasuredFix()` to raw SYNT IDs 3 and 4
only. `PluginEditor.cpp` checks that selected machine-local mode before
formatting measured STAT/PAR/DYN/TUNE output. Retained raw IDs 0/1/2 (`mnm`,
`old`, `new`) retain their original `getFmListedRatio(raw/4)`, `v/16`,
`pow(2,(v-32)/24)`, and `raw-64` display paths; they do not fall through to a
new generic raw-`0..127` display.

## Completed checks

For **both Synth and FX**:

1. `verify_dsp_mode_patch.py` passed, including retained source-hash checks,
   core/state separation checks, FIX-only UI gating, original retained
   formatter ordering, and explicit-profile checks.
2. The literal original `PluginEditor.cpp` FM formatter fragments were compared
   against `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` and are present in
   both products behind the FIX gate.
3. All 16 JUCE-free C++17 CMake targets compiled and passed:
   `NovaChorusTests`, `MnmCoreTests`, `DelayFeedbackDspTests`,
   `DistVariantTests`, `TrackDistVariantIntegrationTests`,
   `MnmRealFilterTests`, `HybridDspTests`, `FilterRouteTests`,
   `TrackFilterRouteIntegrationTests`, `Import2FiltersTests`,
   `FilterExtrasDspTests`, `ModEnvMatrixTests`, `ArpWindowTests`,
   `FmNewModeTests`, `FmFixModeTests`, and `ModeRollbackTests`.
4. Product-equivalent changed source/test/verifier files were compared
   byte-for-byte between Synth and FX.
5. Both `SOURCE_BUILD.json` manifests were regenerated with 180 files and
   independently rehashed successfully.

The C++17 command emitted pre-existing Chorus unused-variable warnings; no
validation command failed.

## Not completed / deliberately not claimed

- No full JUCE/editor/VST3 build: a compatible JUCE checkout is not present.
- No host/DAW state load, automated plug-in render, or subjective listening
  comparison.
- No universal hardware-identical claim for the measured FIX candidate,
  particularly the approximate MNM DYN topology.


---

## Historical FM routing handoff до fix-routes

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17-agent-handoff-pre-fix-routes-2026-09-28-fx-fm-m10-dyn-new-readme-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m10_DYN/NEW/README.md`

`SHA-256: 27b593af8ff28514f86024e0d8ac7623cd492842fbec8126e92565806ea52ed6`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m10_DYN/NEW/README.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m8_STAT/NEW/README.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m9_PAR/NEW/README.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/Synth/FM/m10_DYN/NEW/README.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/Synth/FM/m8_STAT/NEW/README.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/Synth/FM/m9_PAR/NEW/README.md`

---

# FM NEW source provenance and isolation

`MODE SYNT = new` for FM+ STAT (m8), FM+ PAR (m9), and FM+ DYN (m10) owns
only the files in this directory.  It is **not** a replacement for
`Source/dsp/mnm/` and never calls the local `old` FM implementations.

## Reviewed import

These five raw cores were selectively imported from:

`glassg333/mmnova`,
`decompiled data/juce/fm/!fm fix patch/1_NEW_code_FM_2026-09-28/dsp/mnm/`

| original | isolated file | upstream SHA-256 |
| --- | --- | --- |
| `MnmFmSineTable.h` | `FmExactSineTable.h` | `e43b0dabae626d8a6c1ac34d2dcaea997bea4df3fd352dbac7ca6f7a9bee4445` |
| `MnmFmDsp.hpp` | `FmExactDsp.hpp` | `0ab1251a3053eb81f67da370cd7c318e035134a1fc41cd059c4937295f938df0` |
| `MnmFmStat.hpp` | `FmExactStat.hpp` | `935d436da340a2034413406fa346620fc66dfded2668876adcdbc1604b614568` |
| `MnmFmPar.hpp` | `FmExactPar.hpp` | `b790e0602347974e0ba5bc2557ec3a382134b9de81b115cc1f9e580a4a9d3dac` |
| `MnmFmDyn.hpp` | `FmExactDyn.hpp` | `6498815fb603fa0b27aca313d2b2e231d1e15895edf7593e68147c5b5730ec83` |

The raw namespace changed from `mnmfm` to `fmnew`, include names changed to
match this directory, and `FmExactDsp.hpp` replaces signed left shifts of
negative operands with range-safe multiplication by two.  This is a C++
undefined-behaviour repair, not a DSP-law change: the supplied STAT vector
harness passed 52,800/52,800 words after the repair.

`FmExactNew.hpp` is a deliberately small host wrapper.  It has independent
state and an exact-core FIFO; it does not depend on `MnmKernel.hpp`, the
current `MnmFm.hpp`, or `monomachine_fm_par/dynamic.hpp`.

## Scope and validation boundary

The upstream package supplies a complete STAT vector corpus and harness; its
PAR/DYN vector corpus is referenced by the upstream proof but was not shipped
inside this package.  Local regression tests therefore prove STAT's supplied
vectors, wrapper FIFO consistency, deterministic PAR/DYN smoke behaviour,
mode isolation, and legacy-core fingerprints.  They do **not** establish
hardware/DAW/listening equivalence for every PAR/DYN setting.

The firmware machine PROC does not consume the `TUNE` knob itself; the kernel
applies output pitch before dispatch. Nova's new-mode bridge applies the OS
1.32 manual's documented fine-tune range (+/-100 cents) at that boundary:
<https://www.elektron.se/wp-content/uploads/2024/09/monomachine_manual_OS1.32.pdf>.
The exact native kernel pitch-word quantisation still needs a dedicated
reference vector before it can be described as bit-exact end-to-end.


---

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17-agent-handoff-pre-fix-routes-2026-09-28-fx-fm-m10-dyn-route-note-ru-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m10_DYN/ROUTE_NOTE_RU.md`

`SHA-256: c0e6d22e52c473cf93b41e296e4f26bd43c8bf9795b6ffe01a6d7d5664b596a5`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m10_DYN/ROUTE_NOTE_RU.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/Synth/FM/m10_DYN/ROUTE_NOTE_RU.md`

---

# Маршрут базового кандидата до FIX

- **MNM:** `MnmFm.hpp`, соответствующий `FmKind`.
- **OLD:** m10 OLD routes to the retained MonomachineFmDynamic implementation.
- **NEW:** изолированный импорт `FmExactNew.hpp` плюс raw-кор.
- Это точные байтовые копии из указанного pre-FIX FM NEW candidate archive.


---

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17-agent-handoff-pre-fix-routes-2026-09-28-fx-fm-m8-stat-route-note-ru-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m8_STAT/ROUTE_NOTE_RU.md`

`SHA-256: 5e9d6404577d003e5327195261b33f48cdf73c0080767feb2bd2e2165978bb29`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m8_STAT/ROUTE_NOTE_RU.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/Synth/FM/m8_STAT/ROUTE_NOTE_RU.md`

---

# Маршрут базового кандидата до FIX

- **MNM:** `MnmFm.hpp`, соответствующий `FmKind`.
- **OLD:** m8 OLD is an intentional alias to MNM; the legacy stat header is deliberately empty.
- **NEW:** изолированный импорт `FmExactNew.hpp` плюс raw-кор.
- Это точные байтовые копии из указанного pre-FIX FM NEW candidate archive.


---

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17-agent-handoff-pre-fix-routes-2026-09-28-fx-fm-m9-par-route-note-ru-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m9_PAR/ROUTE_NOTE_RU.md`

`SHA-256: ca81f1eef4b0037aa06e9dcbaeb1ad1e25897a38bb61c3ace334b03bcbdc154f`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/FX/FM/m9_PAR/ROUTE_NOTE_RU.md`
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/Synth/FM/m9_PAR/ROUTE_NOTE_RU.md`

---

# Маршрут базового кандидата до FIX

- **MNM:** `MnmFm.hpp`, соответствующий `FmKind`.
- **OLD:** m9 OLD routes to the retained MonomachineFmParallel implementation.
- **NEW:** изолированный импорт `FmExactNew.hpp` плюс raw-кор.
- Это точные байтовые копии из указанного pre-FIX FM NEW candidate archive.


---

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17-agent-handoff-pre-fix-routes-2026-09-28-readme-ru-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/README_RU.md`

`SHA-256: a8069446708cab7febe34d2cba0579020b888fda016bfdd140f450055f135254`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/README_RU.md`

---

# Monomachine Nova — agent handoff: pre-FIX route sources

Это отдельный пакет для делегирования анализа. Он не заменяет полный source
archive и не содержит текущие post-FIX table/UI edits.

**Источник:** `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip`  
**SHA-256 источника:** `2670e89e1d85864b8c3cfe71761f5f8a69cc739009feed6b93b49997e7fc19a9`

- `Synth/` и `FX/` разложены параллельно.
- `FM/` разделён на m8 STAT, m9 PAR, m10 DYN и MNM/OLD/NEW.
- `DIST/` и `FILTER/` отдельно подписаны MNM/OLD.
- `ROUTE_MAP_RU.md` объясняет реальные call-sites и важные алиасы.
- `SOURCE_HASHES.json` содержит SHA-256 каждого скопированного исходного файла.

Не смешивайте этот **pre-FIX baseline** с текущим post-FIX worktree. Для
актуальной реализации используйте отдельный полный source archive, который
будет выпущен после исправления UI/table isolation.


---

<a id="legacy-patch-readmes-from-monomachine-nova-1-9-17-agent-handoff-pre-fix-routes-2026-09-28-route-map-ru-md"></a>
### `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/ROUTE_MAP_RU.md`

`SHA-256: 8db1291833bfe3874c647daeb8188c7da8bd9832bd802104d494049a60b9edda`

**Исходные пути с этим точным содержимым:**
- `LEGACY_PATCH_READMES/from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/ROUTE_MAP_RU.md`

---

# Карта маршрутов для другого агента — pre-FIX baseline

## Что это

Это **не прошивка Elektron** и не утверждение, что любой маршрут уже hardware-exact.
Это замороженные, побайтно скопированные исходники из
`Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` — то есть состояния до нынешних
FM FIX table/UI изменений. Они собраны по маршрутам, чтобы не искать в полном
дереве. `Synth/` и `FX/` лежат рядом и сохранены отдельно, даже если файлы
совпадают по содержимому.

## FM+ — MODE SYNT

| Машина | MNM | OLD | NEW |
|---|---|---|---|
| m8 FM+ STAT | `MnmFm.hpp`, `FmKind::Stat` | намеренный алиас MNM; `monomachine_fm_stat.hpp` пустой | `FmExactNew.hpp` + imported exact files |
| m9 FM+ PAR | `MnmFm.hpp`, `FmKind::Par` | `monomachine_fm_par.hpp`, `MonomachineFmParallel` | `FmExactNew.hpp` + imported exact files |
| m10 FM+ DYN | `MnmFm.hpp`, `FmKind::Dyn` | `monomachine_fm_dynamic.hpp`, `MonomachineFmDynamic` | `FmExactNew.hpp` + imported exact files |

Общая точка фактической диспетчеризации —
`FM/00_DISPATCH_AND_PARAMETER_DEFS/NovaDSP.h`, `MachineEngine::render`.
`DspModes.hpp` показывает стабильные raw IDs: `mnm=0`, `old=1`, `new=2`.

Важно: `NEW` здесь — изолированный импортированный кандидат, не доказанная
«оригинальная прошивка». `MNM` — локальный recovered/approximate route.
Эта маркировка намеренно не позволяет другому агенту выдать их за одинаковый
тип доказательства.

## DIST — MODE S, а не mode_dist

Для слышимого DIST A/B надо начинать с `DIST/*/TrackDIST.inl`:

- `MNM_MODE_S`: `monomachine::mnm::Saturator::process` из `MnmKernel.hpp`.
- `OLD_MODE_S`: `bipolarDist` из `NovaDSP.h`, который вызывает
  `monomachine::mnm::oldBipolarSaturator` из `MnmKernel.hpp`.

Поле `mode_dist` сохранилось для миграции состояния; в этой baseline версии
оно не является отдельным слышимым переключателем. Реальный выбор — MODE S.

## FILTER — MODE FILT при обоих MODE L/H = NATIVE

Начинать с `FILTER/*/TrackFILT.inl`:

- `MNM_MODE_FILT`: `mnm::FilterCore` / `MnmRealFilter.hpp` и реальные таблицы.
- `OLD_MODE_FILT`: `MonomachineFilter` / `monomachine_filter.hpp`.

Эти две ветви выбираются как MNM vs OLD только когда обе стороны MODE L/H
остаются `NATIVE`. Иные MODE L/H направляют звук в отдельный physical route,
не в MNM/OLD. Это важно для корректной диагностики.

## Как передать другому агенту

Дайте ему целиком нужную папку машины/маршрута и этот файл. Все `.hpp/.inl`
внутри — точные copies baseline archive; `SOURCE_HASHES.json` проверяет
происхождение каждого copied source file. Сначала попросите агента изучить
`NovaDSP.h`/`Track*.inl` как call-site, а потом соответствующее ядро.


---
