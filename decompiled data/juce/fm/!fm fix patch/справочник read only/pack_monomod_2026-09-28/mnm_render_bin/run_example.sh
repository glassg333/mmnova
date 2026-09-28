#!/bin/sh
# Пример: эталонный рендер FM+ STAT, нота 60, дефолты
DIR="$(cd "$(dirname "$0")" && pwd)"
SYX="$DIR/../monomod/Elektron_SFX6-60_OS1.32B.syx"
"$DIR/mnm-render" --fw "$SYX" --machine "FM+ STAT" --note 60 --dur 1.0 --tail 1.0 --dump-blocks blocks.txt --out stat_note60.wav
