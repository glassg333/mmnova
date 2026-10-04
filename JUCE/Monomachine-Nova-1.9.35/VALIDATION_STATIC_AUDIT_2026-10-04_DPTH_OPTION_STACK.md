# Статическая проверка direct-LFO DPTH UNI/INV stack — 04.10.2026

## Сообщённая визуальная проблема

У direct-LFO ячейки `DPTH` компактные `UNI`/`INV` ранее занимали локальные строки
`directDepthKnob.y+18` и `+34`. Для стандартной P1 LFO ячейки это давало `INV` около
`(1235,286)` и оно визуально пересекалось с LCD/value `DPTH 127` около
`(1235,286)`.

## Точная правка

Только в `Cell::resized()` обеих byte-identical active копий
`PluginEditor.cpp` option stack теперь вычисляется так:

```cpp
const int optionTop=directDepthKnob.getY()+8;
lfoModeButton.setBounds(sideX,optionTop,sideW,12);     // UNI
lfoInvButton.setBounds(sideX,optionTop+13,sideW,12);   // INV
```

В стандартном faceplate это даёт начало текста `UNI` около `(1232,261)`, начало
`INV` около `(1232,274)` и пяти-пиксельный зазор перед LCD strip, который
начинается около `y=291`.

Не менялись исходная центрированная `DPTH` ручка
`{getWidth()/2-26,27,52,43}`, обычная центрированная LCD strip
`{2,getHeight()-26,getWidth()-4,23}`, parameter IDs `mod_mode`/`mod_inv`,
option state, callbacks, hit areas, tooltips, Matrix/RMB representations или
остальная faceplate geometry.

## Статическая защита

`MATRIX_UI_STATIC_CHECK.py` обновлён до **114 checks**. Помимо mirror и
лексического audit он требует exact `optionTop=y+8`, `UNI`/`INV` new bounds,
сохраняемый DPTH rotary/LCD contract и запрещает оба прежних lower/overlapping
bounds.

Выполнены только статические проверки исходников. Сборка, исполняемые UI/DSP
тесты, CTest, VST3/DAW validation и audio render не запускались.
