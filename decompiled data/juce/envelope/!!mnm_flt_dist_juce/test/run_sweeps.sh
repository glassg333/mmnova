#!/bin/bash
# Свипы верификации filter/dist через monomodule (dsp56300, реальный OS 1.32B).
# Машина GND NOIS (широкополосный сигнал — срезы видны в спектре).
# Дампы блоков: in (52 слов параметров) / out (32 слова = 16 кадров x 2 канала).
set -u
FW=/home/z/my-project/fw/Elektron_SFX6-60_OS1.32B.syx
BIN=/home/z/my-project/repos/monomodule/build/src/cli/mnm-render
OUT=/home/z/my-project/scripts/sweeps
mkdir -p "$OUT"

run() { # name filt amp note
  local name=$1 filt=$2 amp=$3 note=${4:-60}
  echo "== $name (filt=$filt amp=$amp note=$note)"
  "$BIN" --fw "$FW" --machine "GND NOIS" --note "$note" --dur 0.4 --tail 0.1 \
     --filt "$filt" --amp "$amp" --dump-blocks "$OUT/$name.dump" \
     --out "$OUT/$name.wav" 2>&1 | grep -E "wrote|fault|error"
}

# --- A. Закон DIST: свип ручки DIST (AMP-страница: ATK HOLD DEC REL DIST VOL PAN PORT)
# FILT нейтральный (BASE=0, WDTH=127, Q=0). DIST байты: 0,32,48,64,80,96,112,127
for D in 0 32 48 64 80 96 112 127; do
  run "dist_$(printf %03d $D)" "0,127,0,0,0,32,64,64" "0,0,64,64,$D,100,64,0"
done

# --- B. WOFS/BOFS env при коротком DEC=4 (OPEN-1: куда идёт WOFS; OPEN-3: домен)
# FILT: BASE WDTH HPQ LPQ ATK DEC BOFS WOFS
for W in 0 32 64 96 127; do
  run "wofs_$(printf %03d $W)" "40,40,0,0,0,4,64,$W" "0,0,64,64,64,100,64,0"
done
for B in 0 32 64 96 127; do
  run "bofs_$(printf %03d $B)" "40,40,0,0,0,4,$B,64" "0,0,64,64,64,100,64,0"
done

# --- C. Траектория огибающей ATK/DEC (OPEN-3): BOFS=127 (макс глубина)
run "atk0_dec4"  "64,64,0,0,0,4,127,64"  "0,0,64,64,64,100,64,0"
run "atk0_dec40" "64,64,0,0,0,40,127,64" "0,0,64,64,64,100,64,0"
run "atk20_dec4" "64,64,0,0,20,4,127,64" "0,0,64,64,64,100,64,0"

# --- D. Трекинг питча (OPEN-2): статический фильтр, разные ноты
for N in 36 60 84; do
  run "track_n$N" "64,64,0,0,0,32,64,64" "0,0,64,64,64,100,64,0" "$N"
done

echo "ALL DONE"
