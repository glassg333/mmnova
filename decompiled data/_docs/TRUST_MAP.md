# TRUST_MAP.md — уровень доверия по папкам (v2, 2026-09-24)

| Папка | Уровень | Что это и насколько можно верить |
|---|---|---|
| `01_firmware/` | T0-сырьё | Оригинальная прошивка OS1.32B (.bin, .syx). Бит-в-бит источник всего остального. |
| `02_memory_images/` | T0-сырьё | Образы P/X/Y памяти обоих DSP (dsp1/dsp2 pmem/words/xmem/ymem), chorus_region, loader_blob. Пруфы всех моделей сверялись именно с `dsp1_pmem.bin`. |
| `03_listings/` | T0/T1 | Дизассемблер-листинги (dispatch, kernel, machines_page_A 01–33). Сгенерированы из образов детерминированно; при сомнении перегенерируются скриптами `07_scripts/`. |
| `04_tables/` | T0-сырьё | Таблицы из P/X/Y памяти (.bin + человекочитаемый .txt). Канон 80 шт. + PLAIN-таблицы из final_recovery (коллизии имен суффиксятся `__fr`). Это эталонные данные. |
| `05_descriptors/` | T1 | Дескрипторы машин/параметров. |
| `06_docs/` | T1 | Документы, карты доверия, отчёты. `reference_screenshots/` — скриншоты эмулятора (ram program show hex on emulation.png — проверенный визуальный референс). |
| `07_scripts/` | T1 | Инструменты добычи (дизассемблеры, экстракторы, эмулятор). Запускаемы, но НЕ данные. |
| `08_cpp_reference/` | T2→T1 | C++-референсы. Часть сверена с поведением прошивки — см. пруфы в `10_juce_import_ready/clean_proof_audit/`. |
| `09_dsp_models_verified/` | **T0-результат** | Намеренные whitelist-копии проверенного: `09_juce_port/Source` (сверка с dsp1_pmem.bin + смоук-тест) и `22_fm_machines/mnm_fm_exact.hpp` (FM-законы). НЕ перепроверять, брать как есть. |
| `10_juce_import_ready/01_CORE_VERIFIED/` | T0-результат + T1 | Верифицированное ядро (data/dsp/docs/rig, TEST_main.cpp, TEST_REPORT.txt, README_RU.md). |
| `10_juce_import_ready/clean_proof_audit/` | T1 | Чистовая версия с пруфами: CLEAN_SHA256SUMS.tsv, FILE_MANIFEST.tsv, ROUTING_BUFFER_EXACT.md, SOURCES.md, MNM_FILTER_PRACTICAL, proof/, data/, metadata/. |
| `10_juce_import_ready/_set_docs/` | T1 | Родные доки набора MNM_JUCE_SORTED (00_README_СНАЧАЛА, DELETION_LIST, MANIFEST, SHA256SUMS). |
| `11_juce_probable/02_PROBABLE/` | T2 | Правдоподобные JUCE-кандидаты, НЕ сверенные бит-в-бит. |
| `11_juce_probable/05_LAB_PAGES/` | T2 | HTML-лабораторные страницы (фильтр-Q, routing-verified, превью). |
| `11_juce_probable/mmnova_honest_fav/` | T2 | Любимая авторская попытка (Source, tools, PORT_PLAN.md, README_ЧЕСТНО.md). |
| `11_juce_probable/mnm_filter_full_dump_2deep/` | T2 | Глубокий дамп фильтра для JUCE. |
| `12_mining_archive/` | T3 | Архив истории добычи: filter_phaser_pack (итерации 01–24), v7–v10, iteracy4/5, all_tables_v4, hi08_extracted, zero_in_image_recovered, final_recovery_analysis, _docs. Это журнал, а не эталон: эталоны уже вынесены в 04/09/10. |
| `13_quarantine/hypotheses_unverified/` | НЕ ДАННЫЕ | Гипотезы (HYPOTHESIS/LOGSCALE/PIECEWISE/COMPLEMENTARY/RECIPROCAL) — варианты интерпретации таблиц. НЕ использовать как данные без верификации. |
| `13_quarantine/juce_04_quarantine/` | НЕ ДАННЫЕ | Карантин набора 04_QUARANTINE (кроме доказанных bbox_wav-фейков — те удалены, см. REMOVED_AS_DUPLICATE.tsv). |
| `13_quarantine/juce_old_tries/`, `juce_superseded/` | T3/НЕ ДАННЫЕ | Старые и superseded попытки портирования. История, не эталон. |
| `digitone2.txt`, `routing.txt` (корень) | T1 | Заметки оригинального репозитория о Digitone II и маршрутизации. В v1 отсутствовали (устаревший клон) — восстановлены из полного клона. |

## Спорные/известные ограничения
- Таблицы `Y_1449c6_kernel_curve_table` существуют в вариантах plain и QUADRATIC; выбор по сверке
  хэша с исходником прошивки описан в `12_mining_archive/final_recovery_analysis/`.
- Таблицы с суффиксом `__fr` — вторая одноимённая таблица из final_recovery с ДРУГИМИ байтами;
  обе сохранены намеренно (не терять ни одного уникального контента).
- Пак iter28 из v1 (`09_dsp_models_verified`, 261 файл с доп. аудитами) не входит в v2:
  воркспейс откатился, исходники пака = те же `09_juce_port` + `22_fm_machines` (здесь, в полном составе),
  дополнительные аудиты остались в архиве v1 у владельца репозитория.
