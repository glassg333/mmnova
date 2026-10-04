#!/usr/bin/env python3
# Анализ свип-2: закон DIST на синусе, знаковость BOFS/WOFS, траектории, трекинг
import numpy as np, json

SR = 44100
D = "/home/z/my-project/scripts/sweeps"

def parse_dump(path):
    ins, outs = [], []
    for line in open(path):
        if "in:" in line:
            p = line.split(); i = p.index("in:"); ins.append([int(x,16) for x in p[i+1:]])
        elif "out:" in line:
            p = line.split(); i = p.index("out:"); outs.append([int(x,16) for x in p[i+1:]])
    ins = np.array(ins, dtype=np.int64); outs = np.array(outs, dtype=np.int64)
    for a in (ins, outs):
        a[a >= 0x800000] -= 0x1000000
    return ins, outs

def audio(outs, ch=0):
    return outs[:, ch::2].flatten() / float(1 << 23)

def load(name): 
    ins, outs = parse_dump(f"{D}/{name}.dump"); return ins, outs, audio(outs)

def band_db(x, f1, f2, nfft=512):
    nfft = min(nfft, len(x))
    if nfft < 64: return float('nan')
    seg = x[:nfft] * np.hanning(nfft)
    S = np.abs(np.fft.rfft(seg))**2
    fr = np.fft.rfftfreq(nfft, 1/SR)
    m = (fr >= f1) & (fr <= f2)
    return 10*np.log10(S[m].sum()/S.sum() + 1e-20)

def fund_amp(x, f0=440.0, nfft=4096):
    seg = x[len(x)//3: len(x)//3 + nfft] * np.hanning(nfft)
    S = np.abs(np.fft.rfft(seg))
    fr = np.fft.rfftfreq(nfft, 1/SR)
    i = np.argmin(np.abs(fr - f0))
    lo, hi = max(0,i-3), i+4
    return S[lo:hi].max() / (nfft/2)

print("=== A2. DIST на синусе 440 Гц: амплитуда фундаменталы vs байт ===")
print(f"{'byte':>4} {'D':>6} {'amp440':>10} {'dB':>8} {'rel_dB':>8} {'закон4k':>8} {'ожид_rel':>9}")
base = None
curve = {}
for b in [32,40,48,56,64,72,80,88,96,104,112,120,127]:
    ins, outs, x = load(f"sindist_{b:03d}")
    a = fund_amp(x)
    Dw = b/128.0
    x0 = max(0.0, 2*Dw-1.0); k = 0.9837891*x0*x0 + 0.0162109
    curve[b] = dict(amp=float(a), dB=float(20*np.log10(a+1e-12)), law=4*k)
    if b == 64: base = 20*np.log10(a+1e-12)
for b, c in curve.items():
    rel = c['dB'] - base
    exp_rel = 20*np.log10(c['law']/curve[64]['law'])
    print(f"{b:>4} {c['D'] if 'D' in c else b/128.0:>6.3f} {c['amp']:>10.4f} {c['dB']:>8.2f} {rel:>+8.2f} {c['law']:>8.4f} {exp_rel:>+9.2f}")

print()
print("=== B2. Знаковость env-члена (BASE=24, WDTH=96; триггер в t=0, DEC=4) ===")
print("окна: [0..60ms]=пик env, [200..300ms]=после декея, [350..400ms]=хвост")
for name in ["sig_bofs000","sig_bofs064","sig_bofs127","sig_wofs000","sig_wofs064","sig_wofs127",
             "sig2_wofs000","sig2_wofs064","sig2_wofs127"]:
    ins, outs, x = load(name)
    w1 = band_db(x[int(0.00*SR):int(0.06*SR)], 150, 900)
    w2 = band_db(x[int(0.20*SR):int(0.30*SR)], 150, 900)
    w3 = band_db(x[int(0.35*SR):int(0.40*SR)], 150, 900)
    h1 = band_db(x[int(0.00*SR):int(0.06*SR)], 5000, 10500)
    h3 = band_db(x[int(0.35*SR):int(0.40*SR)], 5000, 10500)
    print(f"  {name:14s}  low(150-900): {w1:+6.2f} -> {w2:+6.2f} -> {w3:+6.2f} dB | hi(5-10.5k): {h1:+6.2f} -> {h3:+6.2f} dB")

print()
print("=== C2. Траектория low-band (150-900 Гц), окно 20мс, шаг 20мс, 0..500мс ===")
for name in ["tr_bofs127_dec4","tr_bofs000_dec4","tr_bofs000_dec40","tr_bofs000_atk20"]:
    ins, outs, x = load(name)
    traj = []
    for t in range(0, 440, 20):
        seg = x[int(t/1000*SR):int((t+20)/1000*SR)]
        traj.append(band_db(seg, 150, 900, nfft=256))
    print(f"  {name:18s}: " + " ".join(f"{v:5.1f}" for v in traj))

print()
print("=== D2. Трекинг: центроид полосы пропускания vs нота (BASE=64, WDTH=64) ===")
cents = {}
for n in [36, 48, 60, 72, 84]:
    ins, outs, x = load(f"trk2_n{n}")
    seg = x[int(0.25*SR):int(0.4*SR)]
    seg = seg * np.hanning(len(seg))
    S = np.abs(np.fft.rfft(seg))**2
    fr = np.fft.rfftfreq(len(seg), 1/SR)
    m = (fr > 100) & (fr < 12000)
    c = (S[m]*fr[m]).sum()/S[m].sum()
    cents[n] = float(c)
    print(f"  note {n:2d}: центроид = {c:8.1f} Гц")
ns = sorted(cents)
for a_, b_ in zip(ns, ns[1:]):
    oct_notes = (b_ - a_)/12.0
    oct_cent = np.log2(cents[b_]/cents[a_])
    print(f"  {a_}->{b_}: заметка x{oct_notes:.1f} окт, центроид x{oct_cent:.3f} окт -> трекинг {oct_cent/oct_notes*100:.0f}%")
