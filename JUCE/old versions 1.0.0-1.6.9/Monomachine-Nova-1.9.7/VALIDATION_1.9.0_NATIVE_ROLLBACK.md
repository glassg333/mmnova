# Validation — Monomachine Nova 1.9.0 native DSP rollback

Дата: **26 сентября 2026**. Проверены оба продукта: Synth и FX.

## Passed

1. `python3 verify_dsp_mode_patch.py` (root dispatcher) — PASS для обоих
   проектов. Проверяет отсутствие withdrawn selectors, импортированных source
   деревьев, obsolete CMake targets и пользовательских FMA/new entries; также
   проверяет schema 17 и state migration hooks.
2. `g++ -fsyntax-only` для `PluginProcessor.cpp` и `PluginEditor.cpp` каждого
   продукта — PASS. В editor есть только восемь существующих JUCE 8 Font
   deprecation warnings (четыре на продукт), без compile errors.
3. `ModeRollbackTests.cpp` — PASS для Synth и FX.
4. `MnmCoreTests.cpp` — PASS для Synth и FX.
5. `g++ -fsyntax-only` для затронутых JUCE integration tests
   `TrackTopologyTests.cpp`, `TailClearTests.cpp`, `P2DeclickTests.cpp` и
   `RollbackStateTests.cpp` — PASS для Synth и FX.

## Full CMake build limitation in this environment

Была выполнена configure-попытка CMake 3.30.5 с JUCE 8.0.4 из
`/tmp/JUCE`; проектный CMake прошёл до bootstrap helper. Полный build здесь
невозможен из-за лимита памяти sandbox: GCC получает `Killed` при компиляции
самого JUCE `juceaide` translation unit `juce_gui_basics.cpp` (до компиляции
исходников Nova). Это ограничение среды, а не source error Nova; поэтому
полный VST3/CTest прогон не заявляется как PASS.

Для воспроизводимой Windows/MSVC проверки следует использовать сохранённые
CMake-проекты и `Check-Build.ps1` каждого продукта. Сохранённые MSVC repair
настройки (`/utf-8` и retained source fixes) не были удалены.

## Package integrity

`SOURCE_BUILD.json` в обоих каталогах содержит SHA-256 для активных
исходников и тестов release package. Независимая hash/inventory-проверка
прошла: Synth — 69 entries, FX — 70 entries. В инвентаре отсутствуют
withdrawn `mnm_new`, `mnm/new`, `MnmAmpDistNew` и их тесты.

## Archive verification

Чистый release archive был распакован в новую временную директорию. Проверены
`unzip -t`, `sha256sum -c SHA256SUMS`, per-product `SOURCE_BUILD.json` и root
static dispatcher — всё PASS. SHA-256 самих файлов доставки приводится рядом
с передаваемыми архивами.
