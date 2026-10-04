#!/bin/bash
# Свип-2: чистый закон DIST на синусе + знаковость BOFS/WOFS + трекинг
set -u
FW=/home/z/my-project/fw/Elektron_SFX6-60_OS1.32B.syx
BIN=/home/z/my-project/repos/monomodule/build/src/cli/mnm-render
OUT=/home/z/my-project/scripts/sweeps
mkdir -p "$OUT"

runsin() { # name filt amp note
  local name=$1 filt=$2 amp=$3 note=${4:-69}
  "$BIN" --fw "$FW" --machine "GND SIN" --note "$note" --dur 0.4 --tail 0.05 \
     --filt "$filt" --amp "$amp" --dump-blocks "$OUT/$name.dump" \
     --out "$OUT/$name.wav" 2>&1 | grep -E "wrote|fault|error"
}
runnois() {
  local name=$1 filt=$2 note=${3:-60}
  "$BIN" --fw "$FW" --machine "GND NOIS" --note "$note" --dur 0.4 --tail 0.05 \
     --filt "$filt" --amp "0,0,64,64,64,100,64,0" --dump-blocks "$OUT/$name.dump" \
     --out "$OUT/$name.wav" 2>&1 | grep -E "wrote|fault|error"
}

# --- A2. Закон DIST на чистом синусе 440 Гц (нейтральный фильтр: затемнение среза
#         на рельсах не влияет на 440) — амплитуда фундаменталы = прямая мера 4k
for D in 32 40 48 56 64 72 80 88 96 104 112 120 127; do
  runsin "sindist_$(printf %03d $D)" "0,127,0,0,0,32,64,64" "0,0,64,64,$D,100,64,0"
done

# --- B2. Знаковость env-члена: BASE=24 (low cut глубоко в полосе), WDTH=96.
#         BOFS=0 (ожидаю: low cut вниз при триггере) vs BOFS=127 (вверх)
runnois "sig_bofs000" "24,96,0,0,0,4,0,64"
runnois "sig_bofs064" "24,96,0,0,0,4,64,64"
runnois "sig_bofs127" "24,96,0,0,0,4,127,64"
#         WOFS: hi cut в полосе (BASE=24, WDTH=96 -> верхний срез внутри 5-10к)
runnois "sig_wofs000" "24,96,0,0,0,4,64,0"
runnois "sig_wofs064" "24,96,0,0,0,4,64,64"
runnois "sig_wofs127" "24,96,0,0,0,4,64,127"

# --- C2. Траектория огибающей: BASE=24, BOFS=127/0, DEC=4 vs DEC=40 vs ATK=20
runnois "tr_bofs127_dec4"  "24,96,0,0,0,4,127,64"
runnois "tr_bofs000_dec4"  "24,96,0,0,0,4,0,64"
runnois "tr_bofs000_dec40" "24,96,0,0,0,40,0,64"
runnois "tr_bofs000_atk20" "24,96,0,0,20,4,0,64"

# --- D2. Трекинг: BASE=64 WDTH=64 (полоса в середине), ноты 36/60/84
for N in 36 48 60 72 84; do
  runnois "trk2_n$N" "64,64,0,0,0,32,64,64" "$N"
done

echo "SWEEP2 DONE"
