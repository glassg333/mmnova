#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0 (v2) — bow-morph: скрежет -> чистый бас -> скрежет,
# затем столкновение двух струн -> инхармонический звон.
# Сохраняет всё В ПАПКУ, ГДЕ ЛЕЖИТ ЭТОТ ФАЙЛ.
# Запуск:  python bow_proto.py       (~1-3 мин)
#          python bow_proto.py fast  (черновик, ~в 2 раза быстрее)
# ============================================================
import os, sys, math, time, wave

OUT_DIR = os.path.dirname(os.path.abspath(__file__))

try:
    import numpy as np
except ImportError:
    print("\nОШИБКА: не установлена библиотека numpy.")
    print("Выполни в терминале:  pip install numpy\n")
    try: input("Enter — выход...")
    except Exception: pass
    sys.exit(1)

print("python:", sys.version.split()[0], "| numpy:", np.__version__)
print("ВСЁ СОХРАНЮ В ПАПКУ:", OUT_DIR)

FAST   = "fast" in [a.lower() for a in sys.argv]
SR     = 44100
OVS    = 2 if FAST else 4
dt     = 1.0 / (SR * OVS)
N, L, MU = 160, 1.0, 0.04
INHARM = 5e-5
BOW_END, KICK = 6.0, 6.3
TOTAL  = 8.5 if FAST else 9.5
NSAMP  = int(SR * TOTAL)

def make_string(f0):
    c  = 2.0 * L * f0
    T  = MU * c * c
    dx = L / (N + 1)
    k  = T / dx
    k4 = INHARM * k * (L / dx) ** 2
    return dict(m=MU * dx, k=k, k4=k4, Z0=MU * c,
                y=np.zeros(N), v=np.zeros(N))

A = make_string(82.41)                 # бас
B = make_string(82.41 * 1.4983)        # квинта

# --- матрицы жёсткости/демпфирования (строятся один раз) ---
idx = np.arange(N)
Lap = np.zeros((N, N)); Lap[idx, idx] = -2.0
Lap[idx[:-1], idx[:-1] + 1] += 1.0
Lap[idx[1:],  idx[1:]  - 1] += 1.0
Bih = np.zeros((N, N))
for off, c in ((-2, 1.0), (-1, -4.0), (0, 6.0), (1, -4.0), (2, 1.0)):
    j = idx + off
    ok = (j >= 0) & (j < N)
    Bih[idx[ok], j[ok]] += c

def build(s):
    K = s['k'] * Lap + s['k4'] * Bih
    D = (1e-4 * math.sqrt(s['k'] * s['m'])) * Lap + (0.15 * s['m']) * np.eye(N)
    return K, D

KmatA, DmatA = build(A)
KmatB, DmatB = build(B)

# --- проверка устойчивости (лимит semi-implicit Euler ~2.0) ---
w = math.sqrt(float(np.linalg.eigvalsh(-KmatB).max()) / B['m'])
if w * dt >= 1.8:
    print(f"НЕСТАБИЛЬНО (w*dt={w*dt:.2f}) — запусти с другим OVS.")
    sys.exit(1)
print(f"устойчивость: w*dt = {w*dt:.2f} (лимит 1.8) — ок")

# --- контакт струн (Hunt-Crossley) ---
C0, C1 = N // 3, 2 * N // 3
M = C1 - C0
GAP, KC, CC = 0.008, 3.0e5, 0.5
dA = np.empty(M); ddA = np.empty(M); dlA = np.empty(M); Fc = np.empty(M)

# --- смычок: R=0 скрежет ... R=1 чистый бас ---
V_STICK = 2e-5
def smoothstep(x):
    x = 0.0 if x < 0 else (1.0 if x > 1 else x)
    return x * x * (3.0 - 2.0 * x)

def regime(R, Z0):
    v_b  = 0.06 + 0.34 * R            # скорость смычка
    F_N  = (6.0 - 4.0 * R) * 2.0 * Z0 * v_b   # прижим (окно Шелленга)
    v_f  = 0.012 + 0.10 * R           # ширина спада трения
    beta = 0.045 + 0.065 * R          # точка смычка
    nz   = 0.45 - 0.40 * R            # шум волоса
    return F_N, v_b, v_f, beta, nz

def R_auto(t):
    a, b, c, d = 0.2*BOW_END, 0.45*BOW_END, 0.7*BOW_END, 0.95*BOW_END
    if t < a: return 0.0
    if t < b: return smoothstep((t - a) / (b - a))
    if t < c: return 1.0
    if t < d: return 1.0 - smoothstep((t - c) / (d - c))
    return 0.0

# --- рендер ---
rng   = np.random.default_rng(7)
noise = rng.standard_normal(NSAMP * OVS)
out   = np.zeros(NSAMP)
helio = np.zeros(NSAMP)
fA, fB, tmp = (np.zeros(N) for _ in range(3))
imdtA, imdtB = dt / A['m'], dt / B['m']
yA, vA, yB, vB = A['y'], A['v'], B['y'], B['v']
kicked, done = False, 0
t0 = time.time()

print(f"рендер {TOTAL:.1f} c, fast={FAST} — жди, прогресс ниже:")
try:
    for n in range(NSAMP):
        t = n / SR
        if t < BOW_END:
            on = smoothstep(t / 0.05) * smoothstep((BOW_END - t) / 0.15)
            F_N0, v_b, v_fall, beta, nz = regime(R_auto(t), A['Z0'])
            jb = int(beta * N)
            vb = v_b * on
        else:
            on = 0.0; jb = 8; vb = 0.0; F_N0 = v_fall = nz = 0.0
        for o in range(OVS):
            np.dot(KmatA, yA, out=fA)
            np.dot(DmatA, vA, out=tmp); fA -= tmp
            np.dot(KmatB, yB, out=fB)
            np.dot(DmatB, vB, out=tmp); fB -= tmp
            if on > 0.0:
                FN = F_N0 * (1.0 + nz * noise[n * OVS + o]) * on
                vr = vb - vA[jb]
                fA[jb] += FN * math.tanh(vr / V_STICK) * math.exp(-abs(vr) / v_fall)
            # столкновение струн
            np.subtract(yB[C0:C1], yA[C0:C1], out=dA); dA += GAP
            np.subtract(vB[C0:C1], vA[C0:C1], out=ddA)
            np.negative(dA, out=dlA); np.maximum(dlA, 0.0, out=dlA)
            np.power(dlA, 1.5, out=dlA); dlA *= KC
            np.multiply(ddA, CC, out=Fc); Fc += 1.0
            np.multiply(dlA, Fc, out=Fc); np.maximum(Fc, 0.0, out=Fc)
            fA[C0:C1] -= Fc; fB[C0:C1] += Fc
            # интегрирование (semi-implicit Euler)
            np.multiply(fA, imdtA, out=tmp); vA += tmp
            np.multiply(vA, dt, out=tmp);    yA += tmp
            np.multiply(fB, imdtB, out=tmp); vB += tmp
            np.multiply(vB, dt, out=tmp);    yB += tmp
        out[n]   = A['k'] * yA[0] + 0.8 * B['k'] * yB[0]
        helio[n] = vA[jb]
        done = n + 1
        if not kicked and t >= KICK:
            x = np.sin(np.pi * np.linspace(0.0, 1.0, N)) ** 2
            vA += 0.9 * x; vB -= 0.7 * x
            kicked = True
        if n and n % (SR // 2) == 0:
            el = time.time() - t0
            eta = el * (NSAMP - n) / n
            print(f"\r  {t:5.1f}/{TOTAL:.1f} c   осталось ~{eta:3.0f} с ",
                  end="", flush=True)
except KeyboardInterrupt:
    print("\nпрервано — сохраняю то, что успело посчитаться")
print()

def dcblock(x):
    a = math.exp(-2.0 * math.pi * 25.0 / SR)
    y = np.empty_like(x); s = px = 0.0
    for i2 in range(len(x)):
        y[i2] = x[i2] - px + a * s; px = x[i2]; s = y[i2]
    return y

wav_path = os.path.join(OUT_DIR, "bow_morph.wav")
if done > SR:
    x = dcblock(out[:done].copy())
    x *= 0.9 / (np.max(np.abs(x)) + 1e-12)
    with wave.open(wav_path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((np.clip(x, -1, 1) * 32767).astype("<i2").tobytes())
    print("\nГОТОВО. Аудио:", wav_path)
else:
    print("слишком мало данных — WAV не записан")

try:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    i0, i1 = int(2.9 * SR), int(3.9 * SR)
    if i1 < done:
        plt.figure(figsize=(12, 4))
        plt.plot(np.arange(i1 - i0) / SR, helio[i0:i1], lw=0.4)
        plt.title("Скорость струны в точке смычка (фаза чистого баса)")
        plt.tight_layout()
        p = os.path.join(OUT_DIR, "helmholtz.png")
        plt.savefig(p, dpi=120); print("график:", p)
except Exception as e:
    print("график пропущен (не критично):", e)