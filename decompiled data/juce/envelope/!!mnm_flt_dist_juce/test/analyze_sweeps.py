#!/usr/bin/env python3
# Анализ свипов: поведение реального DSP-кода (dsp56300 core, OS 1.32B)
# против ожиданий закона DIST (4k) и env-модели BOFS/WOFS.
import numpy as np, glob, os, re, json

SR = 44100
D = "/home/z/my-project/scripts/sweeps"

def parse_dump(path):
    ins, outs = [], []
    with open(path) as f:
        for line in f:
            if "in:" in line:
                parts = line.split()
                i = parts.index("in:")
                ins.append([int(x, 16) for x in parts[i+1:]])
            elif "out:" in line:
                parts = line.split()
                i = parts.index("out:")
                outs.append([int(x, 16) for x in parts[i+1:]])
    ins = np.array(ins, dtype=np.int64)
    outs = np.array(outs, dtype=np.int64)
    # 24-bit signed
    for a in (ins, outs):
        a[a >= 0x800000] -= 0x1000000
    return ins, outs

def audio(outs, ch=0):
    # 32 слова на блок = 16 кадров x 2 канала (чередование L,R)
    o = outs[:, ch::2].flatten() / float(1 << 23)
    return o

def rms_db(x):
    return 20*np.log10(np.sqrt(np.mean(x**2)) + 1e-12)

def band_energies(x, nfft=256, hop=128):
    # энергия ниже 2кГц и выше 6кГц по окнам
    w = np.hanning(nfft)
    n = (len(x) - nfft) // hop
    freqs = np.fft.rfftfreq(nfft, 1/SR)
    lo = (freqs > 100) & (freqs < 2000)
    hi = freqs > 6000
    elo, ehi, etot = [], [], []
    for i in range(max(n, 1)):
        seg = (x[i*hop:i*hop+nfft] - np.mean(x[i*hop:i*hop+nfft])) * w
        S = np.abs(np.fft.rfft(seg))**2
        t = S.sum() + 1e-20
        elo.append(S[lo].sum()/t); ehi.append(S[hi].sum()/t); etot.append(t)
    return np.array(elo), np.array(ehi), np.array(etot)

def load(name):
    ins, outs = parse_dump(f"{D}/{name}.dump")
    return ins, outs, audio(outs)

def words_of(ins, idx_from, count):
    return ins[:, idx_from:idx_from+count] / float(1 << 23)

res = {}

# ---------- A. Закон DIST ----------
print("=== A. DIST: усиление тракта vs байт ручки (FILT нейтральный) ===")
base_rms = None
dist_curve = {}
for b in [0, 32, 48, 64, 80, 96, 112, 127]:
    ins, outs, x = load(f"dist_{b:03d}")
    seg = x[SR//4: SR//4 + SR//2]           # установившийся режим
    r = rms_db(seg)
    D_word = b / 128.0
    x0 = max(0.0, 2*D_word - 1.0)
    k = 0.9837891*x0*x0 + 0.0162109
    dist_curve[b] = dict(rms_db=r, D=D_word, law_4k=4*k)
    print(f"  byte {b:3d}  D={D_word:.3f}  rms={r:7.2f} dB   закон 4k={4*k:.4f}")

base_rms = dist_curve[64]['rms_db']
for b in dist_curve:
    dist_curve[b]['rel_db'] = dist_curve[b]['rms_db'] - base_rms
    print(f"  byte {b:3d}: rel={dist_curve[b]['rel_db']:+7.2f} dB", end="")
    print()

# ожидание из закона: rel(127)-rel(64) = 20log10(3.94/0.0648) = 35.7 dB (если y1=const)
print(f"  ожидание закона (127 vs 64): +{20*np.log10(dist_curve[127]['law_4k']/dist_curve[64]['law_4k']):.1f} dB, измерено {dist_curve[127]['rel_db']:+.1f} dB")
# ---------- B. WOFS/BOFS (OPEN-1) ----------
print("=== B. BOFS/WOFS при DEC=4: доля энергии <2кГц и >6кГц (окно 0..0.15с после триггера) ===")
env = {}
for grp, knob in [("bofs", [0, 32, 64, 96, 127]), ("wofs", [0, 32, 64, 96, 127])]:
    for v in knob:
        ins, outs, x = load(f"{grp}_{v:03d}")
        seg = x[: int(0.15*SR)]
        elo, ehi, etot = band_energies(seg)
        # усредняем по окнам, где ещё есть энергия (триггер в t=0)
        env[f"{grp}_{v}"] = dict(lo=float(np.mean(elo)), hi=float(np.mean(ehi)))
        print(f"  {grp}={v:3d}: доля<2кГц={np.mean(elo):.3f}  доля>6кГц={np.mean(ehi):.3f}")

w = env
print(f"  BOFS 0->127: доля<2кГц {w['bofs_0']['lo']:.3f}->{w['bofs_127']['lo']:.3f} (движение low cut)")
print(f"  WOFS 0->127: доля>6кГц {w['wofs_0']['hi']:.3f}->{w['wofs_127']['hi']:.3f} (движение hi cut)")

# ---------- C. Траектория огибающей ----------
print("=== C. Огибающая ATK/DEC (BOFS=127, BASE=WDTH=64): траектория доля<2кГц по 20мс ===")
for name in ["atk0_dec4", "atk0_dec40", "atk20_dec4"]:
    ins, outs, x = load(name)
    seg = x[: int(0.5*SR)]
    elo, ehi, etot = band_energies(seg, nfft=256, hop=256)
    traj = [f"{v:.2f}" for v in elo[:20]]
    print(f"  {name}: {traj}")

# ---------- D. Трекинг ----------
print("=== D. Трекинг: установившаяся доля>6кГц и <2кГц vs нота ===")
for n in [36, 60, 84]:
    ins, outs, x = load(f"track_n{n}")
    seg = x[SR//4: SR//4 + SR//2]
    elo, ehi, etot = band_energies(seg)
    print(f"  note {n}: <2кГц={np.mean(elo):.3f} >6кГц={np.mean(ehi):.3f}")

# ---------- проверка слов FILT, реально ушедших в DSP ----------
print("=== Слова FILT (words 8-15), block 0, пример wofs_127 ===")
ins, outs, x = load("wofs_127")
print("  ", (ins[0, 8:16] >> 16).tolist(), "(должно быть [40,40,0,0,0,4,64,127])")

json.dump(dict(dist=dist_curve, env=env), open(f"{D}/results.json", "w"), indent=1)
print("saved", f"{D}/results.json")
