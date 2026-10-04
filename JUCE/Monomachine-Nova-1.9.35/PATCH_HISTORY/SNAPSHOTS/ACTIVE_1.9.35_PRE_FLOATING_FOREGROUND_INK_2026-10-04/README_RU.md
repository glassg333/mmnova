# Baseline до исправления FloatingPanel foreground ink — 04.10.2026

Неизменяемый source/docs/audit baseline active 1.9.35 непосредственно до
исправления исчезающего текста FloatingPanel. Foreground labels после перевода
в `paintOverChildren()` вызывали `pixel::text()` без явного восстановления
white `ink`; поэтому DTIM и LFO DPTH могли потерять весь static text, а
FILTER/DFB — главным образом title/footer при сохранённых child controls.

Снимок содержит обе версии editor, target markers/manifests и active static
records. Это historical baseline, не текущая спецификация.
