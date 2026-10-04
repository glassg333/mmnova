# PATCH HISTORY — навигация

Этот каталог заменяет россыпь historical numbered README, patch, release и
validation файлов в корне source package и в обоих product directories.

- `FIXES_APPLIED_1.9.21.md` — полная дословная сводка. Каждая включённая
  историческая запись сохранена целиком; byte-identical тела записаны один раз
  с перечислением всех прежних путей и SHA-256.
- `RETIRED_FM_CANDIDATES_1.9.26/` — exact SHA-256 архив снятых из shipping
  Source FM imports raw IDs 6..8, tombstone, candidate-only profile/tests и
  fixture с сохранёнными относительными путями обеих целей.
- `SNAPSHOTS/` — неидентичные historical manifests, pre-cleanup test snapshots,
  restoration metadata и прежние agent-facing README/PROVENANCE. В том числе
  `RELEASE_1.9.27_SOURCE_ONLY/` хранит без сокращения прежние active README и
  validation log; `DFB_CAPTURE_HOLD_WITHDRAWN_1.9.30/` — снятую после runtime
  comparison DFB capture/HOLD спецификацию; 
  `DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/` — полные README/audit/
  FX build feedback непосредственно перед активной RAW-gated DFB коррекцией;
  `RELEASE_1.9.30_SCHEMA43_PRE_1.9.35/` — полные active документы и manifests
  непосредственно перед подтверждённым metadata bump до 1.9.35;
  `FX_PRE_MATRIX_FEEDBACK_1.9.35/` — FX editor непосредственно до зеркального
  Matrix feedback pass; `FX_PRE_RESET_QUEUE_1.9.35/` — editor до текущей
  bounded RESET ALL queue; `ACTIVE_1.9.35_PRE_DFB_RAW63_UNITY_GUARD_ENTRY_2026-10-04/`
  — exact source/docs/audit baseline перед исправлением RAW=63 unity.
  `ARCHIVES/PRE_DFB_RAW63_UNITY_GUARD_ENTRY_2026-10-04/` сохраняет также
  неизменённый previous clean 1.9.35 ZIP и его SHA-256;
  `ACTIVE_1.9.35_PRE_ENV_HELD_LMB_RELEASE_COMMIT_2026-10-04/` и
  `ARCHIVES/PRE_ENV_HELD_LMB_RELEASE_COMMIT_2026-10-04/` сохраняют source/docs
  baseline и clean ZIP непосредственно до ENV release fix;
  `ACTIVE_1.9.35_PRE_FLOATING_FOREGROUND_INK_2026-10-04/` и
  `ARCHIVES/PRE_FLOATING_FOREGROUND_INK_2026-10-04/` — baseline и clean ZIP
  непосредственно до white-ink fix FloatingPanel foreground text;
  `ACTIVE_1.9.35_PRE_DPTH_OPTION_STACK_2026-10-04/` и
  `ARCHIVES/PRE_DPTH_OPTION_STACK_2026-10-04/` — baseline и SHA-256 clean ZIP
  непосредственно до подъёма direct-LFO `DPTH` `UNI`/`INV` над LCD value.
  Двоичные Archive ZIP сохраняются в worktree с их `SHA256SUMS.txt`, но
  намеренно не вкладываются рекурсивно в следующий clean delivery ZIP;
  README/SHA records остаются в package. Это архивы, не действующая спецификация.
- `SNAPSHOTS/ROOT_AUDITS_AND_HANDOFFS_PRE_1.9.26/` — бывшие root-level Oracle,
  FM, filter/routing audits, handoff, withdrawn Track Delay hotfix и старые
  validation logs. Их текст перемещён без сокращения; `oracle/fmplus/` также
  находится внутри этого snapshot.

## Что читать сейчас

1. `../README_FIRST.md` — входная точка пакета.
2. `../README_1.9.35_SOURCE.md` — текущий FM MODE/DFB/Matrix feedback contract.
3. `../VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md` — актуальный
   source-only record Matrix UI и bounded RESET ALL queue.
4. `../VALIDATION_STATIC_AUDIT_2026-10-04_DFB_RAW63_UNITY.md` — active
   source-only record exact RAW=63 unity и hard RAW>=64 GUARD/final clip.
5. `../VALIDATION_STATIC_AUDIT_2026-10-04_DPTH_OPTION_STACK.md` — актуальная
   статическая запись подъёма `UNI`/`INV` над direct-LFO `DPTH` LCD/value.
6. `../TRACK_DELAY_CORRECTION_2026-09-30.md` — сохранённый контекст Track Delay.
7. `../FM_MODE_CLEANUP_STATIC_CHECK.py` — текущий узкий статический аудит.

Архив может содержать retired FM paths как сохранённую историю, но не как
shipping Source. В текущем MODE SYNT действуют только шесть values с IDs `0..5`.

Исторические материалы в этом каталоге не следует принимать за current release
notes или за источник активного DSP routing contract.
