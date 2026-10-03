# r18 — БЛЕНД: фикс «ретриг» + extend-окно = оригинальная LCD-матрица OMEGACODE

Ревизия `r18-20260930`. Проверка сборки: `findstr /c:"r18-20260930" Source\PluginProcessor.h`
→ в логе MSBuild строка `omega8 build rev r18-20260930`.

Пункты пользователя: *(1) «Иероглифы пофиксил?» — да, r17: попап = «TITLE значение»,
чистый ASCII; (2) «есть скрин редактора степ секвенсора» — скрин найден в репо
(`…/omega8/UI/Studio electronics CODE _multilayer patch editor.png`), окно степ-секвенсора
= следующая задача (r19); (3) «раньше бленд морфился плавно, теперь только ретриг нот» —
**НАЙДЕНО И ПОЧИНЕНО** (раздел 1); (4) «интерфейс все ручки окна как в ориг поставь
особенно матрица» — **сделано** (раздел 2).*

Все цифры — **измерения** (standalone C++, без JUCE, 44.1 кГц, тест t18e в
`/home/user/dspcheck/` — в проект не входит).

**Совместимость API:** `LcdMatrix` написан под **JUCE 8** (сборочная среда
пользователя: MSVC/VS2022, `C:\JUCE`, JUCE 8) — каждый вызов сверен с
исходников JUCE 8 (теги 8.0.0..8.0.15, `juce_PopupMenu.h`, `juce_TextEditor.h`,
`juce_Component.h`; ВНИМАНИЕ: docs.juce.com/master = уже JUCE 9 — верификация шла по
тегам 8.x): колбэки `TextEditor::onReturnKey` / `onFocusLost`
(Enter/потеря фокуса = коммит; в JUCE 8 старый `TextEditor::Listener` переименован),
`PopupMenu::addItem (id, text, enabled, isTicked)` — нативная отметка текущего
пункта; показ меню — `showMenuAsync (Options, callback)`: в JUCE 8 синхронный
`show()` закрыт `#if JUCE_MODAL_LOOPS_PERMITTED`, а Projucer для плагинов
выставляет `JUCE_MODAL_LOOPS_PERMITTED=0` (модал-цикл везет хост), т.е. для
плагина асинхронный вариант — единственный способ; `withDeletionCheck (*this)`
защищает колбэк, если редактор удалят пока меню открыто. Линии —
`startNewSubPath` + `lineTo` (в JUCE 8 `addLineSegment` принимает только
`Line<float>`), `grabKeyboardFocus ()` без аргументов (JUCE 8),
`TextEditor::setText (String, bool)`, тултипы через базовый класс
`juce::SettableTooltipClient` (в JUCE 8 `setTooltip` вынесен из `Component`),
все координаты мыши — явный `Point<int>` (`Rectangle<int>::contains` в JUCE 8
не принимает `Point<float>`).
Строковые литералы — чистый ASCII (MSVC без BOM), кириллица только в
комментариях (как в r17).

---

## 1. БЛЕНД: два бага xfade в `OmegaEngine.h`

### 1.1. Кривая фейда делилась не на то (главный баг «ретриг»)

Пер-войс xfade (морф A→B, смена карты) гасил голос по `holdXf` сэмплов, но t-кривая
считалась как

```
t = 1 - holdXf / xfN        // xfN = 4 мс (≈176 сэмплов)
```

хотя «долгий» фейд (enum-прыжок морфа, смена AUX-карты) ставится длиной
`holdXf = xfLongN = 30 мс (≈1323 сэмпла)`. Итог: первые ~26 мс после любого
enum-перелёта `t` проходил от **−6.5 до 0** — голос B умножался на (−6.5…0):
инвертированный, усиленный, затухающий кусок поверх морфа. Ухо слышало это как
**ретриг/вспышку ноты** — ровно жалоба «теперь только ретриг».

Измерено (t18e: sustain-патч, морф 1 с, CUTOFF 100→30, OSC1_FREQ 12→24,
прыжок SEM→MINI на 50%):

| сборка | max/base | pic |
|---|---|---|
| r17 (баг) | **1.86** (0.521 → 0.970) | вспышка, softClip рядом с 1.0 |
| r18 (фикс) | **1.00** (0.521 → 0.520) | чистый морф |

«Рывок» ручки (0↔1 восемь раз, каждый переход через 50%):

| сборка | max/base | pic |
|---|---|---|
| r17 (баг) | **4.10** | клип в 1.000 |
| r18 (фикс) | **2.39** | честное резонансное кольцо, max 0.583, без клипа |

Остаток 2.39× — это настоящий ring резонансного SEM на мгновенном прыжке cutoff
(атака 0 мс, не свип) — музыкально корректно, амплитуда ниже уровня ноты.

**Фикс:** длина текущего фейда хранится в пер-войсе и делит t-кривую:

```cpp
int holdXfTotal = 0;                       // в Voice; ставится вместе с holdXf
setPatch / смена карты:  v.holdXf = xfLongN; v.holdXfTotal = xfLongN;
noteOn:                  v->holdXfTotal = 0;
t = 1.0f - (float) v.holdXf / (float) (v.holdXfTotal > 0 ? v.holdXfTotal : xfN);
```

### 1.2. Ложный «сдвиг карты» на note-on (второй баг, та же семья)

`lastCard` инициализировался `-1`, а `auxCard` = 0 → в **первом** аудио-блоке
(голос уже активен после noteOn) срабатывал ветка «смена карты AUX1»:
`holdXf = 30 мс, holdVal = lastG = 0`. При баге 1.1 это давало бам **на каждой атаке**
(инвертированный кусок от нуля). Это же объясняло, почему e2e «пиковые уровни» r16/r17
казались громкими (0.7–0.84):

| e2e (factory-банк, 128 патчей) | r17 (баги) | r18 (фикс) |
|---|---|---|
| peak (patch 0) | 0.826 | **0.156** |
| peak (patch 32, cut=0) | 0.132 | **0.024** |
| RMS | 0.006–0.088 | **без изменений** |
| max \|DC\| | 4e-5 | **≤ 9.6e-5** |

Честные уровни r18 совпадают с v15-эпохой (peak 0.098–0.24) — т.е. громкость вернулась
к норме, RMS sustain-энергии не тронут. `lastCard = 0` теперь.

**Регрессии:** t17 (Oberheim12) — без изменений; e2e: SEM-патчи и все остальные —
как в r17 (изменение изолировано в xfade-ветках, которых до note-on/морфа нет).

---

## 2. UI: extend-окно = оригинальная LCD-матрица

Скрин-референс (in-scope): `…/omega8/UI/OMEGACODE matrix dropdown extand window.jpg` —
синий текстовый LCD, белые моноширинные подписи, значения с красным маркером ▪,
выпадающие ячейки ▼. Реализован компонент `LcdMatrix` (PluginEditor.h),
extend-окно — **только** он (убраны r6 knob-grid: MW/DY/BN/PR/C1/C2/E3A/DYN/DLY/P1–P4,
и r17 нижняя полоса 10 ручек — всё это теперь ячейки матрицы).

Раскладка (3 колонки, высота 380 px):

```
PARAMETERS                LFO / ENV3                 GLIDE / PAN
uni/voice/prior/mtrg      lfo1: dest1..3+amt rate ~  p.wave
octav/tune/sync/sub       lfo2: dest1..3+amt rate ~  PER-VOICE PAN (8)
filter/glide/wave1/wave2  env3: dest1..3+amt         pos rate dpth × 8 голосов
pw/fine/xmod/vcf/in/win
lvl/freq/cutoff/reso      ENVELOPES
hpf/hpr/track/envlamt     Atk/Dec/Dk2/Sus/Rel/dyn/dly × env1/2/3
p.snc/p.key
MOD MATRIX
modwhl/dynamics/bender/
pressure/cont1/cont2 ×
(dest1 amt dest2 amt)
```

Интерактив (конвенции плагина сохранены):
- **enum-ячейка (▼)** — левый клик = выпадающий список (dest-список 25 значений,
  filter = OBLP/OBBP/OBHP/OBBR/MINI/AUX1/AUX2 — идёт через параметр `filtType`,
  как кликабельный TYPE на панели; LFO wave = ~ / SQR; XMOD vcf = FREQ/PW/CUTOFF);
- **int-ячейка** — драг в любую сторону, ~1 px = 1 шаг (как ручки); правый клик =
  ввод числа (inline-поле, Enter/разбор по blur);
- **wave1** — три кликабельные иконки tri/saw/pulse (битмаска; 0 = saw, как движок);
- display-форматы: octav как октавы (+1.3), tune/win/pw как %, xmod три цифры,
  glide ON/OFF (бит 0x40), cont2 amt со знаком (byte = disp + 64);
- матрица живая: таймер редактора копирует байты патча и перерисовывает —
  правка Q-диалом/хостом/морфом видна мгновенно.

Основная панель: убраны 10 ручек, которых **нет на оригинальной панели**
(FINE, TUNE, EXT IN, SUB, HPF, HPR, LFO1 D2/D3, LFO2 D2/D3) — они в матрице.
Остались ровно ручки оригинала: volume/glide, LFO1/LFO2 rate+depth, XMOD depth,
OSC1/2 freq+pw, osc1/2 level, noise, cutoff/reso/tracking, 4×ADSR, env1/env3 amt,
arp tempo + Programmer (LCD, Q-диал, bank/part, morph, список, GLOBAL, load).

---

## 3. Степ-секвенсор (следующий шаг, r19)

Оригинальный экран найден в репо: `…/UI/Studio electronics CODE _multilayer patch editor.png`
(«S T E P   S E Q U E N C E R», 16 шагов: status/note/vel/dur/modDest/modAmt/part,
список part 1–8, нижняя жёлтая строка midi clock/Poly/ch). Окно редактора под этот
скрин (чёрный LCD, белый текст) + движковая 16-шаговая секвенция — задача r19.
