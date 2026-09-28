#!/bin/bash
# run_verify.sh — ПАК 8: ВСЯ верификация одним прогоном (правило №18).
# 1) бит-точность ядер FM: STAT / PAR+DYN / полный свип 168 сетов
# 2) питч-закон: 128/128 нот + тюн 400 + кламп низа
# 3) смоук обёртки (44.1 кГц, 1:1 путь)
# 4) точная AMP-энвелопа: карта кадров итерации 13 + nova-путь
set -u
P8=/home/z/my-project/work/pack8_fm_closed
T=/home/z/my-project/work/pack8_verify
rm -rf "$T"; mkdir -p "$T/inc" "$T/bin"
cp "$P8"/dsp/mnm/*.hpp "$P8"/dsp/mnm/*.h "$T/inc/" || exit 1
# MnmTables.hpp живёт в дереве плагина (пак его не меняет) — берём для тестового инклуда
cp "/home/z/my-project/work/mmnova/JUCE/Monomachine-Nova-1.9.9 good or no idk stable/Monomachine_Nova_Synth/Source/dsp/mnm/MnmTables.hpp" "$T/inc/" || exit 1
cp "$P8"/research/*.cpp "$T/" 2>/dev/null
cp /home/z/my-project/work/pack7_fix_pitch/research/test_fm_stat.cpp \
   /home/z/my-project/work/pack7_fix_pitch/research/test_pitch_exact.cpp \
   /home/z/my-project/work/pack7_fix_pitch/research/smoke_fm_wrapper.cpp \
   /home/z/my-project/work/pack7_fix_pitch/research/golden400.inc "$T/"
cp /home/z/my-project/work/pack_final/check/mmnova_stage8_fm_par_dyn_100_2026-09-27/research/test_fm_par_dyn.cpp "$T/"
cp /home/z/my-project/work/pack_final/4_PROOF_FM_knob_sweep_2026-09-28/tools/test_fm_all_knobs.cpp "$T/"
VEC=/home/z/my-project/work/pack_final/check/mmnova_stage8_fm_par_dyn_100_2026-09-27/research
SWEEP=/home/z/my-project/work/fm_sweep_all/fm_all_knob_vectors.txt
STATVEC=/home/z/my-project/work/pack7_fix_pitch/research/fm_stat_vectors.txt

fail=0
g++ -O2 -std=c++17 -I"$T/inc" "$T/test_fm_stat.cpp"     -o "$T/bin/t_stat"  2>"$T/log_stat.txt"  || { echo "COMPILE FAIL stat";  cat "$T/log_stat.txt";  fail=1; }
g++ -O2 -std=c++17 -I"$T/inc" "$T/test_fm_par_dyn.cpp"  -o "$T/bin/t_pardyn" 2>"$T/log_pd.txt"    || { echo "COMPILE FAIL pardyn"; cat "$T/log_pd.txt";    fail=1; }
g++ -O2 -std=c++17 -I"$T/inc" "$T/test_fm_all_knobs.cpp" -o "$T/bin/t_sweep" 2>"$T/log_sw.txt"   || { echo "COMPILE FAIL sweep"; cat "$T/log_sw.txt";   fail=1; }
g++ -O2 -std=c++17 -I"$T/inc" "$T/test_pitch_exact.cpp" -o "$T/bin/t_pitch" 2>"$T/log_pt.txt"   || { echo "COMPILE FAIL pitch"; cat "$T/log_pt.txt";   fail=1; }
g++ -O2 -std=c++17 -I"$T/inc" "$T/smoke_fm_wrapper.cpp" -o "$T/bin/t_smoke" 2>"$T/log_sm.txt"   || { echo "COMPILE FAIL smoke"; cat "$T/log_sm.txt";   fail=1; }
g++ -O2 -std=c++17 -I"$T/inc" "$T/test_amp_env.cpp"     -o "$T/bin/t_env"   2>"$T/log_env.txt"  || { echo "COMPILE FAIL env";   cat "$T/log_env.txt";  fail=1; }
[ "$fail" = 1 ] && exit 1

echo "=== 1) FM+STAT vectors (pack 7: 12800 out + 40000 state words) ==="
"$T/bin/t_stat" "$STATVEC" || fail=1
echo "=== 2) FM+PAR / FM+DYN vectors (pack 3: 7168 + 8192 words) ==="
"$T/bin/t_pardyn" "$VEC" || fail=1
echo "=== 3) FULL knob sweep: 168 sets x 8 blocks (177 408 words) ==="
"$T/bin/t_sweep" "$SWEEP" || fail=1
echo "=== 4) pitch law: 128/128 @440 + tune 400 + bottom clamp ==="
"$T/bin/t_pitch" || fail=1
echo "=== 5) wrapper smoke (44.1 kHz 1:1 path) ==="
"$T/bin/t_smoke" || fail=1
echo "=== 6) exact AMP envelope (iteration-13 facts + nova path) ==="
"$T/bin/t_env" || fail=1

echo
if [ "$fail" = 0 ]; then echo "VERIFY: ALL PASSED"; else echo "VERIFY: FAILURES PRESENT"; fi
exit $fail
