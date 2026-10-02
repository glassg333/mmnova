#!/usr/bin/env python3
# bank_loader.py — реал-тайм декодер банка Omega 8 / CODE (эквивалент OmegaPatch.h::loadBank).
# Патч = 176 байт: 0..159 параметры, 160..175 имя (ASCII, 16 символов).
# .syx: F0 + 8-байтная заголовочная строка + N*176 [F7]
# .mid (SMF): F0 ... внутри трека; после F0 может идти VLQ-длина.
import sys, glob, struct

PATCH = 176

def load_bank(path):
    data = open(path, 'rb').read()
    msgs = []
    if data[:4] == b'MThd':
        p = 0
        while p < len(data) - 1:
            if data[p] == 0xF0:
                q = p + 1
                while q < len(data) and data[q] != 0xF7:
                    q += 1
                msgs.append(data[p:min(q + 1, len(data))])
                p = q + 1
            else:
                p += 1
    else:
        msgs = [data]
    out = []
    for s in msgs:
        if s[:1] != b'\xf0':
            continue
        off = 1
        if off < len(s) and s[off] >= 0x80:      # SMF VLQ-длина
            while off < len(s) and s[off] >= 0x80:
                off += 1
        if off + 8 >= len(s):
            continue
        hdr = s[off:off + 8]                     # 8-байтная заголовка Omega
        off += 8
        total = len(s) - off
        if total > 0 and s[-1] == 0xF7:
            total -= 1
        if total < PATCH or total % PATCH:
            continue
        for i in range(total // PATCH):
            pt = s[off + i * PATCH: off + (i + 1) * PATCH]
            if any(b > 127 for b in pt):
                continue
            out.append((hdr, pt))
    return out

def name(pt):
    return bytes(b if 32 <= b <= 126 else 32 for b in pt[160:176]).decode('ascii').strip()

if __name__ == '__main__':
    for f in sys.argv[1:] or glob.glob('../patches/*.syx'):
        pts = load_bank(f)
        print(f'{f}: {len(pts)} патчей')
        for hdr, pt in pts[:3]:
            print('   ', name(pt), '| FILT_TYPE=', pt[43], 'FRE1=', pt[30], 'FRE2=', pt[31],
                  'OCT=', hex(pt[2]), 'GLIDE=', pt[1])
