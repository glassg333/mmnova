# Статическая проверка FloatingPanel foreground ink — 04.10.2026

## Причина исчезновения текста

`pixel::text()` не выбирает цвет самостоятельно: glyph rectangles рисуются
текущим `juce::Graphics` colour. Static labels RMB panels были правильно
перенесены в post-child `paintOverChildren()` для защиты от перекрытия child
controls при pin/front transition, но compositor не восстанавливал white `ink`
перед вызовом `paintFloatingForeground()`.

Поэтому результат зависел от предыдущего child paint state и мог быть black или
dim даже у **незакреплённой** панели:

- `DTIM / BPM` и `DPTH SET` теряли весь static text, потому что их labels
  рисуются родительским foreground;
- у `DLY / FEEDBACK Q`, `FILTER EXTRA` и DFB child controls продолжали
  самостоятельно рисовать свои labels, поэтому чаще исчезали только window
  title/footer.

## Active fix

Common final compositor `FloatingPanel::paintOverChildren()` теперь всегда
делает `g.setColour(ink)` непосредственно перед `paintFloatingForeground()` и
перед common pin. Это единая white-ink starting state для каждого current
FloatingPanel, как pinned, так и unpinned.

`FILTER EXTRA` дополнительно возвращает white `ink` после dim separator до
своего footer `Manual normal tracking`.

Не менялись pin geometry/behavior, drag, child controls, DFB overlay, IDs,
APVTS/state, DSP, layout или tooltips.

## Статическая защита

`RMB_MINI_PANEL_STATIC_CHECK.py` усилен до **72 checks**. Он проверяет common
white-ink compositor, presence foreground text в DTIM и DPTH, explicit FILTER
footer reset, all seven FloatingPanel implementations, Synth/FX byte identity
и прежний collection lifecycle.

Выполнены только source/static проверки. Compilation, executable/UI/DSP test,
CTest, VST3/DAW validation и audio render не запускались.
