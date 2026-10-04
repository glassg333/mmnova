# Monomachine Nova 1.9.30 — сначала прочитайте это

Это исходный пакет для двух продуктов: `Monomachine_Nova_Synth` и
`Monomachine_Nova_FX`. В проектных файлах, исходниках и проверке сборки указана
версия **1.9.30**. Готовый VST3 в пакет не входит.

## Действующие документы

- [`README_1.9.30_SOURCE.md`](README_1.9.30_SOURCE.md) — текущий FM MODE contract, schema 42, CHOR Native/Core A/B, DFB BASE/GUARD LVL и снятый DLY CORE, активное поле ARP SONG и исправление MSVC C2397;
- [`TRACK_DELAY_CORRECTION_2026-09-30.md`](TRACK_DELAY_CORRECTION_2026-09-30.md) — действующая коррекция factory DSND/P2, DFB controller law и границы delay-route;
- [`VALIDATION_STATIC_AUDIT_2026-10-03_DLY_CORE_REMOVED.md`](VALIDATION_STATIC_AUDIT_2026-10-03_DLY_CORE_REMOVED.md) — фактически выполненный актуальный static audit schema 42 без сборки/тестов;
- [`VALIDATION_STATIC_AUDIT_2026-10-03.md`](VALIDATION_STATIC_AUDIT_2026-10-03.md) — сохранённый static audit предшествующей schema-41 source-only ревизии;
- [`VALIDATION_STATIC_AUDIT_2026-10-02.md`](VALIDATION_STATIC_AUDIT_2026-10-02.md) — сохранённый static audit предыдущей schema-40 source-only ревизии;
- [`VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt`](VALIDATION_RUN_LOG_1.9.30_SOURCE_ONLY.txt) — сохранённый журнал предыдущей source-only ревизии;
- [`BUILD_FEEDBACK_2026-10-03_FX_IDENTITY.md`](BUILD_FEEDBACK_2026-10-03_FX_IDENTITY.md) — полученный Windows FX C1189 pairing failure и точечное source-исправление;
- [`DFB_GUARD_CORE_STATIC_CHECK.py`](DFB_GUARD_CORE_STATIC_CHECK.py) — текущий статический аудит schema 42, DFB BASE/GUARD LVL, снятие DLY CORE и Synth/FX parity;
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
