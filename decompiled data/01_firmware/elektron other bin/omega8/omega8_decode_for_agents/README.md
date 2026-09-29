# omega8 decode kit — r13

Материалы реверса Studio Electronics Omega 8 / CODE для VST-порта
(сессии r1..r13). Это **не сборка** — плагин остаётся в `omega8_vst_r12.tar.gz`
(папка omega8v12); следующая сборка плагина будет **r14 / omega8v14**.

## Состав
| путь | что это |
|------|---------|
| `DECODE_NOTES.md` | сводка декода: формат патча, конвенции байтов, conf, открытые вопросы, ловушки (FREQ-32, AGC, per-voice pan) |
| `PROMPTS.md` | 8 готовых промптов для продолжения декода (MULTI, glide-полярность, категории, CONT-источники, заголовок…) |
| `omega8_map.json` | карта смещений 139 ключей: name/kind/conf/note — главная «подсказка для декода» |
| `manual/omegaCODE_clean.txt` | текстовый мануал (OCR) — источник страниц-доказательств |
| `patches/` | фабричные банки A/B (+Multi, имена) для статистики и сверки |
| `firmware/` | дампы ОС Omega/CODE/SE1X (поиск строк/таблиц — в основном тупик, но файлы нужны) |
| `ui_official/` | официальные скриншоты редактора (OMEGACODE.jpg и др.) |
| `ui_decode_screens/` | скриншоты из репо, по которым раскалывались LCD/BLEND/банк/контроллеры |
| `analysis/` | python-инструменты: `bank_loader.py` (лоадер 176B), `analyze_patches.py` (статистика, ловила баг FRE-32), `dump_map.py` (карта → таблица) |
| `external_filters/` | фильтры из репо (oberhx/OB-Xd, Wurtz TB303, OberhVar, deps) + `obxd_engine/` — что адаптировали в r9 |
| `repo_tree/tree_new2.json` | индекс путей репо mmnova (поиск материалов) |

## Быстрый старт
```bash
cd analysis
python3 dump_map.py ../omega8_map.json          # карта смещений -> таблица
python3 analyze_patches.py                       # статистика фабрики (FREQ/GLIDE/OCTAVE)
python3 bank_loader.py ../patches/Omega8FactoryA-CS.syx
```

Правила, которые себя оправдали: **conf не поднимать без доказательства**
(скрин/мануал/статистика), патчи декодировать строго по карте, новые находки —
обратно в `omega8_map.json` (name/kind/conf/note).
