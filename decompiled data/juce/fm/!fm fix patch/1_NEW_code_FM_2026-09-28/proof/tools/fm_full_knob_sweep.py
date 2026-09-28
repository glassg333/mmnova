#!/usr/bin/env python3
"""fm_full_knob_sweep.py — ОДИН ПРОГОН: свип ВСЕХ 8 ручек ВСЕХ трёх FM-машин
(m8 STAT, m9 PAR, m10 DYN) — правило №18, никаких ручных проверок по одной.

Для каждой тройки (машина, ручка, позиция) снимаются бит-точные векторы с
эмулятора OS 1.32 (8 блоков после 2 прогревочных) и на том же прогоне
меряются наблюдаемые величины: частота выхода по переходам через ноль и пик.
Векторы пишутся в один файл; C++-сторона (test_fm_all_knobs.cpp) прогоняет
каждый сет через портированные бит-точные ядра и сравнивает пословно.

Порядки ручек (аппаратные дескрипторы, FM_EXACT_MINING.md):
  m8 STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE
  m9 PAR : 1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE
  m10 DYN: 1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB TUNE
"""
import sys, os, json, math

sys.path.insert(0, "/home/z/my-project/scripts")
import fm_harness as fh  # noqa

M24 = 0xFFFFFF
R6 = fh.R6M  # 0x428
SR = 44100.0

OUT_DIR = "/home/z/my-project/work/fm_sweep_all"
VEC = os.path.join(OUT_DIR, "fm_all_knob_vectors.txt")
JSON_PATH = os.path.join(OUT_DIR, "knob_sweep_summary.json")

MACHINES = {
    8:  ("FM-STAT", ["1FRQ", "1FIN", "1ENV", "1FB", "2FRQ", "2VOL", "TONE", "TUNE"]),
    9:  ("FM-PAR",  ["1FRQ", "1ENV", "2FRQ", "2ENV", "3FRQ", "3ENV", "TONE", "TUNE"]),
    10: ("FM-DYN",  ["1FRQ", "1FEN", "1VOL", "1VEN", "2FRQ", "2ENV", "2FB", "TUNE"]),
}
POSITIONS = [0, 1, 32, 64, 96, 126, 127]
BASE = [64] * 8
PITCH = 11776
WARMUP = 0   # протокол exp61/exp63: без прогрева — C++-ядро стартует с блока 1,
             # любой прогрев на стороне эмулятора рассинхронизирует стейт
BLOCKS = 8


def build_canonical():
    e = fh.build()
    for i in range(8192):
        v = int(round(math.sin(2 * math.pi * i / 8192) * 8388607)) & M24
        e.X[0x14A000 + i] = v
        e.Y[0x14A000 + i] = v
    return e


def s24(v):
    return v - (1 << 24) if v & 0x800000 else v


def snapshot(e):
    st = []
    for off in range(0x40):
        st.append(e.Y.get(R6 + off, 0))
    st.append(e.X.get(5, 0))
    st.append(e.Y.get(5, 0))
    for a in range(0x1E, 0x40):
        st.append(e.X.get(a, 0))
    return st


def freq_hz(raw):
    w = [s24(v & M24) for v in raw]
    n = len(w)
    if n < 4:
        return 0.0
    peak = max(abs(x) for x in w)
    if peak < 1000:          # тишина/DC
        return 0.0
    zc = sum(1 for i in range(1, n) if (w[i - 1] < 0) != (w[i] < 0))
    return zc / 2.0 / (n / SR)


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    lines = [
        "# FM ALL-KNOB SWEEP (m8/m9/m10) bit-exact vectors (OS 1.32, dsp_emu.py)",
        "# формат: MACHINE <m> / SET <i> A=<pitch> / P <8 ручек 0..127>",
        "#         B <32 выходных слов> / S <стейт: y-page(64) X5 Y5 X1E-3F(34)>",
        "# базовые ручки: " + " ".join(str(v) for v in BASE) +
        " (варьируется ОДНА, позиции: " + " ".join(map(str, POSITIONS)) + ")",
    ]
    summary = []
    si = 0
    nblocks_total = 0
    for mach in (8, 9, 10):
        mname, knames = MACHINES[mach]
        for k in range(8):
            for pos in POSITIONS:
                p = list(BASE)
                p[k] = pos
                e = build_canonical()
                fh.fm_init(e, mach)
                for _ in range(WARMUP):
                    fh.fm_block(e, mach, p, PITCH)
                out_all = []
                lines.append("MACHINE %d" % mach)
                lines.append("SET %d A=%d" % (si, PITCH))
                lines.append("P " + " ".join(str(v) for v in p))
                for _ in range(BLOCKS):
                    out = fh.fm_block(e, mach, p, PITCH)
                    st = snapshot(e)
                    lines.append("B " + " ".join(str(s24(v & M24)) for v in out))
                    lines.append("S " + " ".join(str(v) for v in st))
                    out_all += [s24(v & M24) for v in out]
                    nblocks_total += 1
                lines.append("ENDSET")
                f_out = freq_hz(out_all)
                peak = max(abs(x) for x in out_all) if out_all else 0
                summary.append(dict(set=si, machine=mach, mname=mname,
                                    knob=k, kname=knames[k], pos=pos,
                                    A=PITCH, f_out=round(f_out, 2),
                                    peak=peak, blocks=BLOCKS))
                si += 1
                if si % 24 == 0:
                    print("  ... %d/%d sets" % (si, 3 * 8 * len(POSITIONS)), flush=True)
    with open(VEC, "w") as f:
        f.write("\n".join(lines) + "\n")
    with open(JSON_PATH, "w") as f:
        json.dump(summary, f, indent=1, ensure_ascii=False)
    words = nblocks_total * 132
    print("DONE: %d sets, %d blocks, %d words compared -> %s" %
          (si, nblocks_total, words, VEC))


if __name__ == "__main__":
    main()
