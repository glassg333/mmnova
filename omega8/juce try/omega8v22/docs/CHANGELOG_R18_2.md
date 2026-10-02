# r18.2 — RESET ALL (+ всё r18.1)

Ревизия `r18.2-20260930`. Проверка: `findstr /c:"r18.2-20260930" Source\PluginProcessor.h`.

## RESET ALL (новая кнопка, колонка PROGRAMMER, под ручкой/UP/1 OCT)

Звук при тестах часто исчезает (морф в тихий патч, застрявшие голоса, арп-состояние).
Кнопка одной нажатием возвращает звук:

1. `morph -> 0` (AUX/blend в полный патч A);
2. `Engine::clearVoices()` — все голоса: `reset()` (env + Svf/Moog/OBX/303) +
   `Oberheim12::reset()` + фазы осцилляторов/пан-ЛФО + smoothed cutoff/res/pan
   в нейтраль;
3. арп: `arp = Arp()` (held/phase/pattern в ноль);
4. CC-лэтчи: bend/pressure/cont1/cont2 = 0;
5. свежий push патча A: `engine.setPatch (bank[A])` под `bankLock` (тот же
   лок, что и `refreshPatchFromParams` в аудио-потоке — без гонок).

Дальше играешь ноту — новый голос, фильтр холодный, уровень честный.

## Состав = r18.1 (без изменений)

- питч: `FREQ - 32` (r10; замер A4 = 440.085 Hz при FREQ=32/OCT=MID);
- морф @50%: xfade 120 мс (t18g: одиночный прыжок +12st SEM->MINI ratio 1.00);
- AUX2 (CS80): причина «взрыва» = численный расход SVF при f/sr >= 0.30
  (сетка dbg8) — кламп 0.24*sr + AGC 1/(1+2.5*res^2) + Q HP 0.5+4*HPR;
  замер res=127: peak 1.000 -> 0.018, inf нет, DC -1.9e-6;
- матрица: `setVisible` в обработчике GLOBAL (баг невидимости r18) +
  оригинальная 2-колоночная раскладка (левый PARAMETERS + LFO/ENV3 +
  MOD MATRIX; центр ENVELOPES + PER-VOICE PAN x8; правый GLIDE/PAN);
- JUCE 8: `Path::quadraticTo` (в JUCE 8 `addQuadCurveFrom` удалён — сверено
  с тегами 8.0.0/8.0.15), полный syntax-check редактора = 0 ошибок.

E2E OBMoog: peak 0.105..0.234, DC <= 4.4e-4 (как r18.1).
