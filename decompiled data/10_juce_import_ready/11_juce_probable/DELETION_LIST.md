# DELETION LIST — что можно удалить из твоей папки `MNM BLOCKS FOR JUCE`

Принцип: ничего не теряется — всё либо перенесено в `MNM_JUCE_SORTED`, либо это
фейк/дубль. Если сомневаешься — просто замени содержимое папки на
`MNM_JUCE_SORTED` целиком и держи его как единственную правду.

## Удалять безопасно (фейк/мусор)
```
mnmdsp_reference filter from emulation/data/bbox_wav/          (фейк-WAV, 129-байтные HTML-обрывки)
3 mnmdsp_reference more version/data/bbox_wav/                 (тот же фейк, дубль)
```

## Удалять после того, как возьмёшь 01_CORE_VERIFIED (устаревшее)
```
1 try/
2 try filter dist env nice build with preview site есть из чего выбрать тип если не найти ориг/
3 mnmdsp_reference more version/          (подмножество более нового пака)
5 mnm-dist-filter/
mmnova-filter-official from manual и косвенно все вырисовывает его/
6 VERIFIED 2026-09-18/                    (отчёт сохранён в 01_CORE_VERIFIED/docs/)
mnm-filter-q/Source/mmnova/*.h            (код уступает 01_CORE; доки SAVE → перенесены)
```

## Держать перенесённым (не удалять, а заменить на копию из SORTED)
```
mnmdsp_reference filter from emulation/   → заменён на 01_CORE_VERIFIED (добавлены цепочка, таблицы, исправления)
mnm-routing-100/                          → 02_PROBABLE/
mnm-filter-q/ (доки)                      → 02_PROBABLE/
4 mmnova-honest  fav/                     → 02_PROBABLE/4-mmnova-honest-fav
mnm-routing-verified/ROUTING_CONFIRMED.md → содержимое учтено; index.html → 05_LAB_PAGES
mnm-routing-verified/NEXT_3_PATHS.md      → учтено в отчёте
оба корневых .txt                         → 02_PROBABLE/
gpt 6 astra cheker na oriig.html          → 05_LAB_PAGES/
```

## Итог
После замены папки на `MNM_JUCE_SORTED` у тебя:
- один источник правды для кода (01_CORE_VERIFIED/dsp),
- дословные таблицы прошивки (dsp/MnmKernelTables.h + data/),
- вся история в 02/03 с объяснением, почему файл устарел.
