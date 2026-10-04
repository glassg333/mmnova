# Monomachine Nova 1.9.35 — сначала прочитайте это

Это исходный пакет для двух продуктов: `Monomachine_Nova_Synth` и
`Monomachine_Nova_FX`. В проектных файлах, исходниках и проверке сборки указана
версия **1.9.35**. Готовый VST3 в пакет не входит.

## Действующие документы

- [`README_1.9.35_SOURCE.md`](README_1.9.35_SOURCE.md) — текущий FM MODE contract, schema 45 direct LFO/MOD MATRIX state, восстановленный LFO `HALF`, original linear SPD/MULT law, dynamic SPD hover timing, aligned LFO LOCKS P1/P2 lists и JUCE 8 PopupMenu compatibility, CHOR Native/Core A/B, schema 44 DFB BASE CURVE/HOLD @64 + RAW-gate/@64/@127 GUARD anchors/OFFSET, снятый DLY CORE, ARP SONG и MOD ENV compact UI;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_TIMING.md`](VALIDATION_STATIC_AUDIT_2026-10-03_LFO_TIMING.md) — выполненный source-only audit `HALF`, 12 independent LFO, `2048/(SPD*MULT)`, Modbang Calculator и Synth/FX mirror; без сборки/тестов;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_FACEPLATE.md`](VALIDATION_STATIC_AUDIT_2026-10-03_LFO_FACEPLATE.md) — выполненный source-only audit читаемого PAGE/DEST, enlarged P1/P2 badge и привязанного WAVE dropdown; без UI render;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_SPEED_TOOLTIP.md`](VALIDATION_STATIC_AUDIT_2026-10-03_LFO_SPEED_TOOLTIP.md) — выполненный source-only audit dynamic English SPD hover tooltip: live BPM, active `SPD/MULT`, `2048/(SPD*MULT)`, seconds/Hz/HALF и Synth/FX mirror; без UI render;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_LOCK_ALIGNMENT.md`](VALIDATION_STATIC_AUDIT_2026-10-03_LFO_LOCK_ALIGNMENT.md) — выполненный source-only audit non-selectable `P2 PITCH N/A` spacer: параллельные P1/P2 PAGE rows без изменения lock/solo/state/routing;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_POPUP_MENU_JUCE8.md`](VALIDATION_STATIC_AUDIT_2026-10-03_POPUP_MENU_JUCE8.md) — Windows MSVC C2665 follow-up: JUCE 8-compatible Item API для всех custom LFO/Matrix hover rows; повторная build остаётся следующим gate;
- [`FM_MODE_CLEANUP_STATIC_CHECK.py`](FM_MODE_CLEANUP_STATIC_CHECK.py), [`LFO_TIMING_STATIC_CHECK.py`](LFO_TIMING_STATIC_CHECK.py), [`LFO_FACEPLATE_STATIC_CHECK.py`](LFO_FACEPLATE_STATIC_CHECK.py), [`LFO_SPEED_TOOLTIP_STATIC_CHECK.py`](LFO_SPEED_TOOLTIP_STATIC_CHECK.py), [`LFO_LOCK_ALIGNMENT_STATIC_CHECK.py`](LFO_LOCK_ALIGNMENT_STATIC_CHECK.py), [`POPUP_MENU_JUCE8_STATIC_CHECK.py`](POPUP_MENU_JUCE8_STATIC_CHECK.py) и [`MATRIX_UI_STATIC_CHECK.py`](MATRIX_UI_STATIC_CHECK.py) — локальные текстовые audits active LFO/UI без компиляции или запуска DSP;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md`](VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md) — актуальный corrective source-only audit Matrix: DPTH как исходная `PAN` geometry/full centred LCD, P1 DEST leaf display без изменения popup/Matrix, P2 badges/names, P2 DIST/VOL/PAN IDs, `POLAR` widths, P1/P2 dock folders/divider, default-ON route reset/allocation, reset queue и Synth/FX mirror; без сборки/тестов;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_LFO_MATRIX.md`](VALIDATION_STATIC_AUDIT_2026-10-03_LFO_MATRIX.md) — сохранённый более ранний schema-45 record P2 target-local mapping, Matrix INV, groups и hover preview;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_DFB_BASE_BEND.md`](VALIDATION_STATIC_AUDIT_2026-10-03_DFB_BASE_BEND.md) — актуальный static audit current schema 45 с retained schema-44 DFB BASE BEND; без сборки/тестов;
- [`TRACK_DELAY_CORRECTION_2026-09-30.md`](TRACK_DELAY_CORRECTION_2026-09-30.md) — сохранённый исторический контекст factory DSND/P2, DFB и границ delay-route; текущую law задаёт schema-44 README;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_DFB_RAW_WINDOW.md`](VALIDATION_STATIC_AUDIT_2026-10-03_DFB_RAW_WINDOW.md) — сохранённый schema-43 static audit до BASE BEND;
- [`PATCH_HISTORY/SNAPSHOTS/DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/README_RU.md`](PATCH_HISTORY/SNAPSHOTS/DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/README_RU.md) — без сокращений сохранённые schema-42 README, audit и FX build feedback;
- [`VALIDATION_STATIC_AUDIT_2026-10-03.md`](VALIDATION_STATIC_AUDIT_2026-10-03.md) — сохранённый static audit предшествующей schema-41 source-only ревизии;
- [`VALIDATION_STATIC_AUDIT_2026-10-02.md`](VALIDATION_STATIC_AUDIT_2026-10-02.md) — сохранённый static audit предыдущей schema-40 source-only ревизии;
- [`VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt`](VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt) — сохранённый журнал предыдущей source-only ревизии;
- [`BUILD_FEEDBACK_2026-10-03_FX_IDENTITY.md`](BUILD_FEEDBACK_2026-10-03_FX_IDENTITY.md) — historical Windows FX C1189 pairing feedback до schema 43; актуальная source-проверка identity записана в новом audit;
- [`DFB_GUARD_CORE_STATIC_CHECK.py`](DFB_GUARD_CORE_STATIC_CHECK.py) — текущий статический аудит schema 45 с retained schema-44 DFB: BASE CURVE/HOLD @64, cached LUT, RAW 63..64 arm, @64/@127 anchors/OFFSET, embedded panel, снятие DLY CORE и Synth/FX parity;
- [`SOURCE_MAP_RU.md`](SOURCE_MAP_RU.md) — карта runtime/legacy DSP, FX, chorus и безопасный порядок будущей перестройки;
- [`PATCH_HISTORY/README_RU.md`](PATCH_HISTORY/README_RU.md) — структура истории и snapshots;
- [`PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/README_RU.md`](PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/README_RU.md) — exact SHA-256 архив снятых FM candidates/tests/fixture;
- [`PATCH_HISTORY/FIXES_APPLIED_1.9.21.md`](PATCH_HISTORY/FIXES_APPLIED_1.9.21.md) — один полный файл с прежними numbered patch/readme/release/validation записями.

Полные README и validation log предыдущей active revision 1.9.27 сохранены
без сокращения в
`PATCH_HISTORY/SNAPSHOTS/RELEASE_1.9.27_SOURCE_ONLY/`. Более ранние historical
`TRACK_DELAY_HOTFIX`, validation logs, Oracle research, FM audits и
handoff-документы также сохранены без изменения текста в
`PATCH_HISTORY/SNAPSHOTS/ROOT_AUDITS_AND_HANDOFFS_PRE_1.9.26/`.

Исторические материалы не являются действующей DSP/UI спецификацией. Эта
поставка проверяется source-only: перед бинарным выпуском необходимы обычная
Windows-цепочка Projucer/VS2022, CTest, сборка VST3 и проверка в DAW.
