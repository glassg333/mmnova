#!/usr/bin/env python3
# analyze_patches.py — статистика фабричных патчов (та, что вскрыла баг FREQ-32):
#   * OSC1/OSC2_FREQ кратны 12 -> шкала полутонов ОТ ноты (0..63), центра 64 НЕТ;
#   * OCTAVE = нибблы (hi октава [4=МИД], lo fine);
#   * GLIDE_FLAGS кластеры {64,68} vs {0,4} (bit6=on — conf2, открытый вопрос);
#   * имена только ASCII.
import glob, collections, sys
from bank_loader import load_bank

OFF = dict(GLIDE_TIME=0, GLIDE_FLAGS=1, OCTAVE=2, OSC1_FREQ=30, OSC2_FREQ=31,
           OSC2_FINE=78, MASTER_TUNE=77, XMOD_DPTH=41, NOISE=39)

pts = []
for f in sorted(glob.glob('../patches/*.syx') + glob.glob('../patches/*.mid')):
    pts += [p for _, p in load_bank(f)]
print('патчей:', len(pts))

def top(k, n=8):
    c = collections.Counter(p[OFF[k]] for p in pts)
    return c.most_common(n)

for k in ('OSC1_FREQ', 'OSC2_FREQ', 'OSC2_FINE', 'GLIDE_FLAGS', 'GLIDE_TIME', 'MASTER_TUNE'):
    print(f'{k}: top={top(k)} distinct={len(set(p[OFF[k]] for p in pts))}')

# кратность 12
for k in ('OSC1_FREQ', 'OSC2_FREQ'):
    c = collections.Counter(p[OFF[k]] % 12 == 0 for p in pts)
    print(f'{k} кратно 12: {dict(c)}')

# OCTAVE нибблы
hi = collections.Counter((p[OFF['OCTAVE']] >> 4) & 0xF for p in pts)
lo = collections.Counter(p[OFF['OCTAVE']] & 0xF for p in pts)
print('OCTAVE hi:', dict(sorted(hi.items())), ' lo:', dict(sorted(lo.items())))

# кросс-таблица глайда
ct = collections.Counter()
for p in pts:
    on = (p[OFF['GLIDE_FLAGS']] & 0x40) != 0
    t = p[OFF['GLIDE_TIME']]
    ct[('ON' if on else 'OFF', 't==0' if t == 0 else ('t<20' if t < 20 else 't>=20'))] += 1
print('glide crosstab:', dict(ct))

# имена: байты >=128?
bad = sum(1 for p in pts if any(b > 126 for b in p[160:176]))
print('имен с байтами >=128:', bad)
