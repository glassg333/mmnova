# Восстановленные legacy patch / README / validation logs

Этот каталог добавлен в поставку **1.9.21** только как архив документации.
Ни один существующий актуальный файл, DSP-исходник, проектный файл или тест не
заменялся и не удалялся.

## Что восстановлено по исходным путям

В активное дерево возвращены ранее отсутствовавшие документы:

- `FM_ORACLE_FIRST_CLEAN_ROOM_PLAN_2026-09-28.md`;
- `ORACLE_MODE_IMPLEMENTATION_STATUS_2026-09-28.md`;
- `SYNTH_FX_SIDE_BY_SIDE_FM_OLD_TOPOLOGY_HANDOFF_2026-09-28.md`;
- `oracle/fmplus/README.md`;
- `README_1.9.20_RUNTIME_INTEGRATED_SOURCE_PACKAGE.md`.

## Безопасные исторические снимки без перезаписи

- `from-Monomachine-Nova-1.9.17stable-user-base/` содержит полный набор
  человеческих README/patch/validation/audit документов из доступной stable
  safe-lineage, включая обе копии `README_1.9.13.md`.
- `from-Monomachine-Nova-1.9.17-AGENT-HANDOFF-PRE-FIX-ROUTES-2026-09-28/`
  сохраняет pre-fix routing README и route notes.

Точного каталога или архива с именем `Monomachine-Nova-1.9.13 safe` в данном
workspace не обнаружено. Однако его требуемые 1.9.13 README сохранены в
available stable lineage и уже присутствовали в активных продуктах byte-identically:

- Synth safe-lineage SHA-256: `db2041d68d8153d10db0ac2ff52a25840dfcb21f5cab2b80b2a096c3607007a6`;
- FX safe-lineage SHA-256: `644fe31a207900fdcc2f5a7681f3003fce3d0d980e95875d5fb19d134ada56b4`.

Полный SHA-256 список добавленных файлов — в
`RESTORATION_MANIFEST_SHA256.txt`; provenance и исходные пути — в
`RESTORATION_SOURCES_RU.md`.
