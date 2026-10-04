# Monomachine Nova 1.9.35 — актуальная навигация Synth

Это Synth часть source-only пакета. Текущий FM+ MODE SYNT для STAT/PAR/DYN
содержит шесть selectable значений: `mnm frq`, `old frq`, `new frq`, `mnm bpm`,
`new bpm`, `old bpm` (renderer IDs `0..5`). Experimental raw IDs `6..8`
недоступны и при загрузке state нормализуются в `mnm frq`.

## Читать в таком порядке

1. [`../README_FIRST.md`](../README_FIRST.md) — текущая точка входа.
2. [`../README_1.9.35_SOURCE.md`](../README_1.9.35_SOURCE.md) — актуальный FM MODE contract, schema 45 direct LFO/MOD MATRIX state, восстановленный LFO `HALF`, linear SPD/MULT timing, dynamic SPD hover tooltip, aligned LFO LOCKS P1/P2 rows и JUCE 8 PopupMenu compatibility, DFB exact-unity RAW=63, hard RAW>=64 GUARD/final clip и BASE CURVE/HOLD @64 anchors, снятый DLY CORE, ARP и MOD ENV UI, CHOR Native/Core.
3. [`../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_TIMING.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_TIMING.md) — выполненный source-only audit `HALF`, 12 independent LFO, `2048/(SPD*MULT)`, Modbang Calculator и Synth/FX mirror; без сборки, VST3/DAW, render и запуска тестов.
4. [`../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_FACEPLATE.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_FACEPLATE.md) — выполненный source-only audit PAGE/DEST faceplate, enlarged P1/P2 badge и привязанного WAVE dropdown; без UI render.
5. [`../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_SPEED_TOOLTIP.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_SPEED_TOOLTIP.md) — выполненный source-only audit dynamic English SPD tooltip: live BPM, active SPD/MULT, `2048/(SPD*MULT)`, seconds/Hz/HALF и Synth/FX mirror; без UI render.
6. [`../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_LOCK_ALIGNMENT.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_LOCK_ALIGNMENT.md) — выполненный source-only audit non-selectable `P2 PITCH N/A` spacer: параллельные P1/P2 PAGE rows без изменения lock/solo/state/routing.
7. [`../VALIDATION_STATIC_AUDIT_2026-10-03_POPUP_MENU_JUCE8.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_POPUP_MENU_JUCE8.md) — follow-up MSVC C2665: JUCE 8 Item API для всех custom LFO/Matrix hover rows; повторная Windows build остаётся следующим gate.
8. [`../VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md) — актуальный source-only feedback audit Matrix: fixed DPTH geometry, held-LMB ENV handoff, reset queue по 12 host notifications, grouped SOURCE/AUX, BI/UNI graph, bottom-pinned upward LFO dock и Synth/FX mirror.
9. [`../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_MATRIX.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_LFO_MATRIX.md) — сохранённый более ранний schema-45 record P2 target-local mapping, Matrix INV, groups и hover preview.
10. [`../VALIDATION_STATIC_AUDIT_2026-10-03_DFB_BASE_BEND.md`](../VALIDATION_STATIC_AUDIT_2026-10-03_DFB_BASE_BEND.md) — сохранённый source-only static audit schema 44 DFB.
11. [`../TRACK_DELAY_CORRECTION_2026-09-30.md`](../TRACK_DELAY_CORRECTION_2026-09-30.md) — сохранённый исторический контекст Track Delay и граница `DFB_ROUTE_UNRESOLVED` для custom FX slots; не заменяет текущую schema-44 спецификацию.
12. [`../PATCH_HISTORY/README_RU.md`](../PATCH_HISTORY/README_RU.md) — навигация по истории.
13. [`../PATCH_HISTORY/FIXES_APPLIED_1.9.21.md`](../PATCH_HISTORY/FIXES_APPLIED_1.9.21.md) — полные старые patch/readme/validation записи.

Исторические документы не являются текущей DSP/UI спецификацией. Полная сборка,
VST3/DAW, render и test execution этой source-only поставкой не заявляются.
