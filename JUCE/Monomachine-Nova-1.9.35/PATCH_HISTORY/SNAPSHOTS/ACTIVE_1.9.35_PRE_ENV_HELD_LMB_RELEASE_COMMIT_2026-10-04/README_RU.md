# Baseline до исправления held-LMB release у ENV категорий — 04.10.2026

Это неизменяемый source/docs/audit baseline active 1.9.35 перед исправлением
сценария: удерживаемый ЛКМ при переходе AMP / FIL ENV / MOD ENV выбирает
целевую категорию во время drag, но release ошибочно мог повторно вызвать
`onClick` исходной категории и вернуть её назад.

Снимок хранит обе версии `PluginEditor.cpp`, target-specific build markers,
релевантные tests/manifests, source-only audits и current DFB/RMB документацию.
Это исторический baseline, не активная спецификация.
