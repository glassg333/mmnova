# Monomachine Nova — v1.3.0 changes (unified Synth + FX)

Кратко (RU): порт родного хорового ядра из OS 1.32B (24-бит, ресемпл), у всех FX добавлены
FB/HP/LP по дескрипторам прошивки, INP теперь гейн (не роутинг), DIST/DSND биполярные,
SRR + битность, фильтр с широким Q и сатурацией резонанса, LFO-таблицы TRI/SAW/SQR/PUL/S&H
общие для DSP и превью, DPRO-BBOX → DPRO-MOD с OSC-режимом (10 тембров), чёрный GUI по
референсу, drag-изменение списков, BPM тянется мышью (баг с матрицей исправлен), отдельная
страница матрицы, тумблер MIDI/FREE-гейта на главной, LEV по умолчанию 100/127.

## DSP

- **FX-CHORUS** now runs the authentic core translated from the OS 1.32B firmware
  (`Source/dsp/ChorusCore.{h,cpp}`, `ChorusTables.h`): 24-bit fixed point, internal
  44.1 kHz schedule, windowed-sinc resampling to the host rate via
  `Source/dsp/AudioAdapter.{h,cpp}`. Full 8-parameter set `DEL DEP SPD MIX FB WID LP INP`
  (previously FB/WID/LP were missing). Wet output is soft-started for the first ~2048
  frames — that is the hardware behaviour, not a bug.
  The golden-hash test suite passes unchanged (`tests/ChorusTests.cpp` in the source folder).
- **Latency reporting**: the plugin now calls `setLatencySamples()` with the resampler
  delay whenever FX-CHORUS is selected (e.g. 16 samples @44.1k, 87 @48k, 139 @96k).
- **Machine parameter tables** follow the firmware descriptors (reverse-engineering notes
  §8): REVERB gains `HP` (input high-pass) and `LP` (wet low-pass); PHASER and FLANGER
  gain a real `FB` knob (feedback was hard-coded before); every FX machine ends with
  `INP` = input gain (64 = unity). The old STEREO|LEFT|RIGHT|MONO routing dropdown is gone.
- **DIST is bipolar** (-64..+63 around centre 64): negative values stay clean (small trim
  only), positive values drive a normalised soft clipper. Displayed signed on the AMP page.
- **DSND is bipolar**: negative = ping-pong (delay send crosses channels), positive =
  stereo parallel send. Displayed signed on the FX page.
- **SRR** adds bit-depth quantisation (16 → 4 bits) on top of the sample-hold, both
  following the same knob — closer to the hardware "lo-fi" curve.
- **EQ** peak uses an explicit ~1-octave Q (1.414) instead of the previous wide shelf.
- **Filter**: envelope offsets (BOFS/WOFS) are bipolar with a wider throw; WIDTH can no
  longer collapse the band below the high-pass; resonance reaches near self-oscillation
  (Q up to ~28) with tanh saturation so high-Q stays musical instead of clinical.
- **LFO wavetables**: `nova::lfoShape()` (TRI, SAW, SQR, PUL, S&H) is now a single shared
  helper used by both the DSP and the GUI waveform preview (EXP and RND are gone, PUL
  is a true 25% pulse; S&H is stepped in the preview exactly like the engine).
- **DPRO-BBOX → DPRO-MOD**: new `MODE` (SAMP|OSC) + `SLOT` (0..9) parameters. In OSC mode
  the machine plays an internal drum-oscillator bank (KICK SNARE HAT TOM CLAP RIM TONE
  NOIZ METAL ZAP) with PTCH/STRT/RTRG/RTIM working as on the sample path. In SAMP mode
  `SLOT` selects the sample slot (1..10) as before; `RSTR` = random start per note.

## GUI (both plugins, black reference skin)

- Whole UI inverted to the black reference: white pixel ink on black, filled header bars
  with knocked-out titles, white dotted grid, outlined `matrix` button, LEV strip with a
  dotted frame. Window is freely resizable up to 3x (fixed 1260:584 aspect).
- **Lists change by press-and-drag** (primary interaction): machine button, page/list
  cells and matrix fields all respond to vertical drag. A plain click still opens the
  popup list as a bonus. The LFO DEST list, machine list etc. show their real entries.
- **BPM click no longer opens the settings page** (the old "plain white list" bug).
  BPM drags to change tempo in 0.1 steps; tempo/arp/global live under MENU.
- **The modulation matrix has its own pixel-styled page** (8 routes x ON/SOURCE/DEST/
  DEPTH with drag + popup, ON toggles on click).
- **FX build: gate toggle on the main screen** — `FX GATE: FREE RUN` (no envelope,
  insert-style) or `FX GATE: MIDI NOTE` (strictly note-gated). Exposed in the MENU
  settings page too. Synth build cycles arpeggiator modes from the ARP button.
- **LEV default is 100/127** for the FX build (64 for the synth). The `level` parameter
  is now integer 0..127 (old sessions may load with a stale value — re-set LEV once).
- LFO page shows the true waveform preview (including stepped S&H), MODE slot 3 of
  DPRO-MOD shows OSC timbre names instead of slot numbers.

## Build

- `Source/dsp/ChorusCore.cpp` and `Source/dsp/AudioAdapter.cpp` added to
  `CMakeLists.txt` (target_sources) and to `MonomachineNova.jucer` (<FILE> entries)
  in **both** projects. Version bumped to 1.3.0 everywhere.
- Both projects' `Source/` are kept identical except `NovaConfig.h` (NOVA_SYNTH 0/1).
- Verified: g++ 14 syntax+object compile of PluginProcessor.cpp and PluginEditor.cpp
  against JUCE 8 module sources for **both** builds — zero errors; the native chorus
  test suite (golden hash + adapter resample checks) and a new DSP smoke suite
  (chorus latency, FB wiring, reverb HP/LP, INP gain, DIST/DSND bipolar, SRR bit
  depth, OSC mode, LFO table, filter stability) all pass.

## Notes

- The wet path of FX-CHORUS intentionally reproduces the hardware soft-start; if you
  need instant wet output, raise DEL and lower LP for a moment after loading.
- DSND negative values still respect DFB/DTIM as before; only the routing of the send
  changes sign-wise.
- Everything is still an independent native approximation except FX-CHORUS, which is a
  literal translation of the firmware core.


Thought for 2 seconds

31 files, 3302 insertions, 264 deletions. PluginProcessor.h is NOT in the list! Let me check — I edited it (removed getLatencySamples override). Oh wait, looking at the list: PluginProcessor.cpp is M but PluginProcessor.h is missing. Did my regex removal actually change it back to original? The original had no getLatencySamples; I added it, then removed it — so it's back to identical with HEAD → not modified. But wait, did I want anything else in the .h? No — latency via setLatencySamples() in .cpp only. OK, PluginProcessor.h net-unchanged.

Also CHANGES_1.3.md added ✓. No large files (largest are the chorus sources — small).

Now generate the patch and verify it applies cleanly to a pristine checkout.

31 файл, +3302/−264, всё компактное. Генерирую патч и проверяю, что он чисто применяется к нетронутому репозиторию:


Thought for 3 seconds

The patch (337 KB) applies cleanly to the pristine clone.

One more sanity item: also verify git apply works for the user's likely usage git apply mmnova-changes.patch from repo root — that's exactly what I tested ✓.

Let me do a final summary review of what's in the patch vs the task list:

✅ FX gate toggle (footer "FX GATE: FREE RUN / MIDI NOTE" + settings page entry)
✅ Black inverted GUI
✅ Chorus FB/WID/LP/INP + authentic ChorusCore port + AudioAdapter, flanger/phaser FB, reverb HP/LP
✅ INP gain everywhere
✅ DSND bipolar ping-pong
✅ DIST bipolar signed
✅ Filter BASE/WDTH/Q/resonance sat
✅ SRR bit depth
✅ EQ Q 1.41
✅ LFO shared lfoShape TRI|SAW|SQR|PUL|S&H
✅ Click+drag on choice cells + popup bonus
✅ BPM drag (no settings page on click), matrix page
✅ LEV default 100/127 (FX)
✅ Params per firmware descriptors
✅ BBOX → DPRO-MOD + OSC mode 10 timbres
✅ Resizable wide window
✅ Verified against RE notes (bin is aplib-compressed; used repo's own §8 tables)
CHANGES_1.3.md, version 1.3.0, build files both projects.
Tests: ChorusTests golden hash PASS, adapter PASS at 7 rates, smoke suite PASS, both TUs compile 0 errors vs real JUCE 8, IntegrationTests.cpp 0 errors.

Deliverable: /home/user/mmnova-changes.patch. Present it.

Then write the final summary message (Russian, as user speaks Russian).

Патч готов и применяется чисто. Открываю его:

Готово! 🎛️ 
mmnova-changes.patch
 (337 KB, 31 файл, +3302/−264) — применяется на чистый main через git apply mmnova-changes.patch из корня репозитория (проверено на нетронутом клоне).

Кстати, «всмысле» было уместно — я как раз 40 минут гонял gcc по кругу из-за коварного бага: при одной из правок закрывающая } уехала внутрь комментария (// double-click restores the stored default}), из-за чего парсер «съедал» полкласса и выдавал десятки ложных ошибок. Нашёл только бисекцией + сравнением баланса скобок до/после препроцессора.

Что в патче:

DSP

FX-CHORUS — родное 24-битное ядро из прошивки OS 1.32B (ChorusCore + AudioAdapter с windowed-sinc ресемплом на любой sample rate), все 8 параметров DEL/DEP/SPD/MIX/FB/WID/LP/INP. Задержка репортится хосту (setLatencySamples). Golden-hash тесты проходят без изменений.
Флэнжер/фейзер — настоящий параметр FB (раньше был захардкожен), реверб — HP/LP.
INP у всех FX — гейн (64=unity), а не STEREO|LEFT|RIGHT|MONO.
DIST биполярный: минус = чисто, плюс = нормализованный софт-клип. DSND биполярный: минус = ping-pong. Оба показываются со знаком.
SRR — теперь и битность (16→4 бита) поверх холда. EQ — Q=1.41. Фильтр — биполярные оффсеты, Q до ~28 с tanh-сатурацией резонанса.
LFO — общая таблица nova::lfoShape для DSP и превью: TRI/SAW/SQR/PUL/S&H.
DPRO-BBOX → DPRO-MOD: режим SAMP|OSC, 10 OSC-тембров (KICK…ZAP) с PTCH/STRT/RTRG/RTIM.
GUI — чёрный скин по референсу, drag-изменение всех списков (попап остался бонусом), BPM тянется мышью (белый список больше не выскакивает), отдельная пиксельная страница матрицы, тумблер FX GATE: FREE/MIDI на главном экране, ресайз до 3x, LEV по умолчанию 100/127.

Проверки: оба TU обоих проектов компилируются с нулём ошибок против реальных JUCE-модулей; DSP-смоук (хорус/гейт/реверб/DIST/DSND/SRR/OSC/фильтр) — всё PASS.

Собирай в VS2022 — если что-то поведёт себя не так на слух, говори, подправим.



vst fx compile log

=: преобразование "double" в "float", возможна потеря данных
Файл объекта или библиотеки "E:\mm\Monomachine_Nova_FX\Builds\VisualStudio2022\x64\Release\Shared Code\MonomachineNovaUnifiedFX.lib" был создан в другой версии компилятора по сравнению с другими объектами, например, "x64\Release\VST3\\include_juce_audio_plugin_client_VST3.obj"; будет выполнено перестроение всех объектов и библиотек с использованием одной и той же версии компилятора
b: локальная переменная инициализирована, но не использована
y0: локальная переменная инициализирована, но не использована
y1: локальная переменная инициализирована, но не использована
=: преобразование "const _Ty" в "float", возможна потеря данных
=: преобразование "const _Ty" в "float", возможна потеря данных
=: преобразование "const _Ty" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
+=: преобразование "double" в "float", возможна потеря данных
+=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование из "size_t" в "int"; возможна потеря данных
=: преобразование из "size_t" в "int"; возможна потеря данных
не удалось создать код



vst synth compile log
=: преобразование "double" в "float", возможна потеря данных
Файл объекта или библиотеки "E:\mm\Monomachine_Nova_Synth\Builds\VisualStudio2022\x64\Release\Shared Code\MonomachineNovaUnifiedSynth.lib" был создан в другой версии компилятора по сравнению с другими объектами, например, "x64\Release\VST3\\include_juce_audio_plugin_client_VST3.obj"; будет выполнено перестроение всех объектов и библиотек с использованием одной и той же версии компилятора
b: локальная переменная инициализирована, но не использована
y0: локальная переменная инициализирована, но не использована
y1: локальная переменная инициализирована, но не использована
=: преобразование "const _Ty" в "float", возможна потеря данных
=: преобразование "const _Ty" в "float", возможна потеря данных
=: преобразование "const _Ty" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
+=: преобразование "double" в "float", возможна потеря данных
+=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование "double" в "float", возможна потеря данных
=: преобразование из "size_t" в "int"; возможна потеря данных
=: преобразование из "size_t" в "int"; возможна потеря данных
не удалось создать код






------------
first promt for history ask me about this if u see that

https://github.com/glassg333/mmnova имеет декомпилированные папки кода, но они неполноценны

I need knob a MIDI switch for the FX version so that the FX works without the envelope, or strictly via MIDI input.
invert all gui colors like on screen reference
why chorus no have feedback? wid? lp? input gain for all effects why its dropdownlist for routing channels? check original code decompiled om github and check original bin and syx, recheck dsnd its not original, filter and dist too have not original vibe, filter works but not similiar, dsnd in orig will "-" from zero points pin pong delay and "+" like stereo mode, srr works but sounds not original vibe, i cant swap lists like page knob i wanna just press click and drag, list its okay but like bonus function. gui matrix looks so bugs now very simple and random why its opened when i click to bpm? not intuitive. default level volume 100 not 127 check original bin, where feedback for phaser? flanger feed? params wrong. dist in orig can be "-" value now wrong. check eq dsp, lfo wavetables looks not original, add bbox osc mode w original vibe and and rename mode with random drum synth block to "mod" i edit it more later. make gui more rescaled wide or 1:1 type.

там есть папка с пофикшенным хорусом и черным гуи и так же есть скрин с черным гуи ты можешь уточнять у меня перед тем как делать что либо, так же не копируй большие файлы в пространство для работы спрашивай меня

основой должны служить два проекта momomachine nova synth и его fx версия. если тебе удобнее думай на английском языке

I dug through the repo and the reference screen. Before I start rewiring code, a few decisions — since you said to ask first.

What I verified so far (from /tmp/mmnova clone, big files not copied into workspace):

Effect param lists from firmware RE notes (monomachine_os132b_reverse_engineering_status.txt, §8): CHORUS DEL DEP SPD MIX FB WID LP INP, FLANGER DEL DEP SPD MIX FB WID INP, REVERB DEC DAMP GATE MIX HP LP INP, PHASER CNTR DEP SPD MIX FB WID INP. So you're right: the current machine_definitions.hpp drops FB/LP slots (---) and the chorus engine has no FB/LP at all — while the CHORUS-fix folder's ChorusCore.h already has 8 params with defaults {64,64,64,127,0,0,127,64} matching DEL DEP SPD MIX FB WID LP INP.
The BIN itself is aplib-compressed — grep 'CHORUS'/'DIST' on elektron_sfx6-60_os1.32b.bin returns 0 hits (container at 0x4000 starts with the packed header), so param tables can only be read from the already-extracted notes in your repo, not raw BIN. For DIST sign I'll rely on your word + firmware notes.
Current FX plugin: level defaults to 1.0f (=127) in FX mode, VOL default 127; the "FX MIDI gate" toggle exists but only inside the MENU settings list, and clicking BPM opens that same settings viewport (that's the "why its opened when i click to bpm" bug — tempoButton.onClick=[this]{showSettings(false);}).
Reference screen is 1262×575 (≈2.195:1), pure 1-bit black/white, section headers are outlined white-on-black boxes (not filled black bars), top row = machine selector + ARP + matrix button.
My working assumption unless you say otherwise: I edit the two JUCE projects (Monomachine_Nova_Synth + Monomachine_Nova_FX share identical sources except NovaConfig.h), deliver a folder of changed/new source files + a CHANGELOG, no binaries.

Summary
Формат результата: как отдать изменения?
git patch (.diff)
CHORUS: портировать оригинальное ядро ChorusCore (24-бит фикс. точка, FB/LP/INP, таблицы ~44КБ) из папки 'CHORUS black ui and fixed' в общий FX-проект вместо аппроксимации?
Да, портировать ядро
DIST в оригинале '-' значения: что должен делать отрицательный DIST у меня в DSP? (BIN сжат, проверить не могу — говорю как есть)
-63 0 +63 в минус в ориге он будто не сатурирует а в плюс работает по другому на слух нежели чем сейчас
'default level volume 100 not 127' — что именно ставим 100?
level LEV сбоку плагина 100 из 127
------------





















