Monomachine Nova 1.6.5 -- polniy arhiv
======================================

Zdes oba proekta v finalnom sostoyanii 1.6.5 (Synth i FX),
vse ispravleniya vneseny, fayl PluginEditor.cpp (stroka 590)
uzhe s razdelennymi ciklami knopok -- oshibka C3535 ne vernetsya.

KAK POSTAVIT:
1. Raspakuy etot arhiv.
2. Zameni svoyu papku Monomachine-Nova-1.6.4 na etu tselikom
   (ili prosto skopiruy s zamenoy poverhu).
3. Otkroy OBA .jucer v Projucer i nazhmi Save (vklyuchaet /utf-8,
   inache krakozyabry v UI vernutsya).
4. Sobiray kak obichno (Check-Build.ps1 / MSBuild Release x64).

PAYLOAD iz patch-kit BOLSHE NE NUZHEN -- udali ego, esli on
lezhit vnutri proekta, inache build-skript pytaetsya ego sobrat.

PROVERKI (u menya proydeny):
- verify_dsp_mode_patch.py: PASS dlya oboih proektov
  (zapusk: python verify_dsp_mode_patch.py Monomachine_Nova_Synth/Source)
- tests/MnmCoreTests.cpp: g++ -std=c++17 -I Source tests/MnmCoreTests.cpp -> all OK
- patch mmnova-1.6.5.patch primenyaetsya k chistomu main bez konfliktov

BAKAPY:
- monomachine_fm_stat_par.hpp.bak-v7 -- staryy fayl DSP, ostavlennyy
  prosto na sluchay, v sborke ne uchastvuet.

KRAKOZYABRY-FIX v2 (19.09.2026):
- VSE russkie stroki v UI teper hranitsya kak ASCII-eskeypy \xNN
  vnuri strokovih literalov. Kodirovka faylov bolshhe ne vliyaet
  ni na sborku, ni na UI: binarnik vsegda soderzhit tochniy UTF-8.
- Kommentarii mogli ostatis na russkom -- oni v UI ne popadayut.
- Posle zameny papki vse ravno otkroy OBA .jucer v Projucer i Save.

IZMENENIYA 1.6.6 (19.09.2026):
1. ENVELOPE (REFERENCE FIT): hold bolshe ne_globit zvuk (ataka idyot vo vremya
   hold, pik zamorozhen); susteyn-ne "fon" pri kruchenii DEC ubran (pol = 0).
2. FM DYN: 2FB teper unipolyarniy -- 0 = NOL fidbeka (ranshe 0 zvuchal kak
   polniy), 64 ~ polovina, 127 polniy. FM DYN -- mashina po umolchaniyu
   pri otkritii.
3. DELAY mnm: ruchka DSND rabotaet po logike old DSP (ping-pong = mono-miks
   vhoda v levuyu liniyu, stereo = posilno); unity pri DSND=0 sohranён.
4. REPITCH deleja: PKM na ruchke DTIM -> spisok OFF/FAST/MED/SLOW
   (kakoY pitch povtorov pri prokrutke DTIM).
5. MODE teper v zagolovkah stranits (kak REFERENCE FIT u envelope):
   SYNT header = mode_synt (ili algoritm SWAVE-SAW, kogda vybrana SWAVE;
   ee ruchka MODE ubrana iz setki), FILT header = mode_filt,
   EFFX header = SRR/DIST (mode_dist) i DLY (mode_dly).
6. ARP: gorizontalniy sekvensor -- tashchish mysheu i risuesh transpose
   (verhnaya zona) i velocity (nizhnyaya zona) po vsem stepam srazu;
   PKM po stepu = HOLD; knopka RANDOM srabativaet srazu (menu -- po PKM).
