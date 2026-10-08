#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0 (v3) — исправленное ядро прототипа
#  Исправлено относительно v1/v2:
#   1) убран член изгибной жёсткости (знак + слишком жёсткий для
#      явной схемы; в C++ для него нужен неявный интегратор)
#   2) смычок: прилипание решается как ограничение с запретом
#      проскока — устойчиво при любом dt (иначе buzz/взрыв)
#   3) знак демпфирования в контакте Hunt-Crossley
#  Запуск:  python bow_proto.py        (~1-2 мин)
#           python bow_proto.py fast   (~в 2 раза быстрее, черновик)
#  Результат: bow_morph.wav, helmholtz.png — рядом с этим файлом.
# ============================================================
import os, sys, math, time, wave

OUT_DIR = os.path.dirname(os.path.abspath(__file__))

try:
    import numpy as np
except ImportError:
    print("\nНет numpy. В терминале:  pip install numpy\n")
    try: input("Enter — выход...")
    except Exception: pass
    sys.exit(1)

print("python:", sys.version.split()[0], "| numpy:", np.__version__)
print("СОХРАНЮ ВСЁ В ПАПКУ:", OUT_DIR)

FAST = "fast" in [a.lower() for a in sys.argv]
SR   = 44100
OVS  = 2 if FAST else 4            # оверсемплинг физики
dt   = 1.0 / (SR * OVS)
N, L, MU = 160, 1.0, 0.04          # масс на струну, длина (м), плотность (кг/м)
BOW_END, KICK = 6.0, 6.3           # сек: конец смычка, момент толчка
TOTAL = 8.5 if FAST else 9.5
NSAMP = int(SR * TOTAL)

def make_string(f0):
    c  = 2.0 * L * f0              # скорость волны
    T  = MU * c * c                # натяжение
    dx = L / (N + 1)
    m  = MU * dx
    return dict(m=m, k=T / dx, Z0=MU * c, im=dt / m,
                y=np.zeros(N), v=np.zeros(N))

A = make_string(82.41)             # басовая струна (ми2)
B = make_string(82.41 * 1.4983)    # квинта, слегка расстроена -> биения

BR = 1e-4 * math.sqrt(A['k'] * A['m'])   # соседское демпфирование (ВЧ-завал)
CA = 0.15                                # общее затухание, 1/с

# --- устойчивость: явная схема держит w*dt < 2, берём запас ---
w_max = 2.0 * math.sqrt(A['k'] / A['m'])
print(f"устойчивость: w_max*dt = {w_max * dt:.2f} (нужно < 1.0)")
assert w_max * dt < 1.0, "увеличь OVS"

# --- контакт струн (Hunt-Crossley) ---
C0, C1 = N // 3, 2 * N // 3        # зона возможного контакта
GAP    = 1.2e-3                    # зазор между струнами, м
KC, CC = 3.0e5, 0.5                # жёсткость / демпфирование контакта

# --- смычок ---
V_STICK = 2e-5                     # ширина зоны прилипания в кривой, м/с
CST     = 1.0                      # статическая capacity (доля FN)

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

# R: 0 = скрежет ... 1 = чистый бас. Одна модель трения, морф по параметрам.
def regime(R, Z0):
    v_b  = 0.06 + 0.34 * R                      # скорость смычка, м/с
    F_N  = (6.0 - 4.0 * R) * 2.0 * Z0 * v_b     # прижим (окно Шелленга)
    v_f  = 0.012 + 0.10 * R                     # ширина спада трения
    beta = 0.045 + 0.065 * R                    # точка смычка (доля длины)
    nz   = 0.45 - 0.40 * R                      # шум волоса
    return F_N, v_b, v_f, beta, nz

def R_auto(t):                                  # жёстко -> бас -> жёстко
    a, b, c, d = 1.2, 2.7, 4.2, 5.7
    if t < a: return 0.0
    if t < b: return smoothstep((t - a) / (b - a))
    if t < c: return 1.0
    if t < d: return 1.0 - smoothstep((t - c) / (d - c))
    return 0.0

PA = np.empty(N + 2); QA = np.empty(N + 2)
PB = np.empty(N + 2); QB = np.empty(N + 2)

def string_force(s, P, Q):
    """упругость + демпфирование струны с защемлёнными концами"""
    y, v = s['y'], s['v']
    P[0] = P[-1] = 0.0; P[1:-1] = y
    Q[0] = Q[-1] = 0.0; Q[1:-1] = v
    return (s['k'] * (P[:-2] - 2.0 * y + P[2:])
            - BR * (Q[:-2] - 2.0 * v + Q[2:])
            - CA * s['m'] * v)

rng   = np.random.default_rng(7)
noise = rng.standard_normal(NSAMP * OVS)
out   = np.zeros(NSAMP)
helio = np.zeros(NSAMP)
FA = np.zeros(N); FB = np.zeros(N)

kicked, done = False, 0
t0 = time.time()
print(f"рендер {TOTAL:.1f} c (fast={FAST}), обычно ~1-2 мин. Прогресс:")

try:
    for n in range(NSAMP):
        t = n / SR
        if t < BOW_END:
            on = smoothstep(t / 0.05) * smoothstep((BOW_END - t) / 0.15)
            F_N0, v_b0, v_fall, beta, nz = regime(R_auto(t), A['Z0'])
            jb = int(beta * N)
            vb = v_b0 * on
        else:
            on = 0.0; jb = 8; vb = 0.0; F_N0 = v_fall = nz = 0.0

        for o in range(OVS):
            FA[:] = string_force(A, PA, QA)
            FB[:] = string_force(B, PB, QB)

            # --- столкновение струн (только если сблизились) ---
            if A['y'][C0:C1].max() - B['y'][C0:C1].min() > GAP:
                d   = GAP + B['y'][C0:C1] - A['y'][C0:C1]
                ddt = A['v'][C0:C1] - B['v'][C0:C1]   # скорость сближения
                dl  = np.maximum(-d, 0.0)             # глубина проникновения
                Fc  = KC * dl ** 1.5 * (1.0 + CC * ddt)
                np.maximum(Fc, 0.0, out=Fc)
                FA[C0:C1] -= Fc
                FB[C0:C1] += Fc

            # --- смычок: стик как ограничение + кинетика без проскока ---
            if on > 0.0:
                FN = F_N0 * (1.0 + nz * noise[n * OVS + o]) * on
                f_other = FA[jb]
                # доп. сила, чтобы точный стик на этом шаге:
                F_need = A['m'] * (vb - A['v'][jb]) / dt - f_other
                if abs(F_need) <= CST * FN:
                    FA[jb] = f_other + F_need          # СТИК (v станет ровно vb)
                else:
                    vr = vb - A['v'][jb]
                    Fk = FN * math.tanh(vr / V_STICK) * math.exp(-abs(vr) / v_fall)
                    vn = A['v'][jb] + (f_other + Fk) * dt / A['m']
                    if ((vr > 0.0 and vn > vb) or (vr < 0.0 and vn < vb)) \
                            and abs(F_need) <= CST * FN:
                        FA[jb] = f_other + F_need      # ПОЙМАЛИ без проскока
                    else:
                        FA[jb] = f_other + Fk          # скользим дальше

            # --- интегрирование (semi-implicit Euler) ---
            A['v'] += FA * A['im']; A['y'] += A['v'] * dt
            B['v'] += FB * B['im']; B['y'] += B['v'] * dt

        out[n]   = A['k'] * A['y'][0] + 0.8 * B['k'] * B['y'][0]
        helio[n] = A['v'][jb]
        done = n + 1
        if not kicked and t >= KICK:
            x = np.sin(np.pi * np.linspace(0.0, 1.0, N)) ** 2
            A['v'] += 0.6 * x
            B['v'] -= 0.5 * x
            kicked = True
        if done % SR == 0:
            el  = time.time() - t0
            eta = el * (TOTAL - t) / max(t, 0.5)
            print(f"  {t:5.1f}/{TOTAL:.1f} c   осталось ~{eta:3.0f} с")
        if done % (SR // 4) == 0 and not (
                np.isfinite(A['y']).all() and np.isfinite(B['y']).all()):
            print(f"\nЧИСЛЕННЫЙ ВЗРЫВ (NaN) при t={t:.2f} — сообщи этот момент")
            break
except KeyboardInterrupt:
    print("\nпрервано — сохраняю что успело")
print()

def dcblock(x):
    a = math.exp(-2.0 * math.pi * 25.0 / SR)
    y = np.empty_like(x); s = px = 0.0
    for i in range(len(x)):
        y[i] = x[i] - px + a * s
        px = x[i]; s = y[i]
    return y

wav_path = os.path.join(OUT_DIR, "bow_morph.wav")
if done > SR // 2:
    x = dcblock(out[:done].copy())
    x *= 0.9 / (np.max(np.abs(x)) + 1e-12)
    with wave.open(wav_path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((np.clip(x, -1.0, 1.0) * 32767).astype("<i2").tobytes())
    print("ГОТОВО. Аудио:", wav_path)
else:
    print("данных слишком мало — WAV не записан")

try:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    i0, i1 = int(3.2 * SR), min(int(4.0 * SR), done)
    if i1 - i0 > 100:
        plt.figure(figsize=(12, 4))
        plt.plot(np.arange(i1 - i0) / SR, helio[i0:i1], lw=0.4)
        plt.title("Скорость струны в точке смычка (фаза чистого баса): пила Хельмгольца")
        plt.xlabel("с"); plt.tight_layout()
        p = os.path.join(OUT_DIR, "helmholtz.png")
        plt.savefig(p, dpi=120)
        print("график:", p)
except Exception as e:
    print("график пропущен (не критично):", e)

print("""
Тайминг:
  0.0- 1.2  скрежет (stick-slip + шум волоса)
  1.2- 2.7  морф к басу (переход через срыв — самое выразительное)
  2.7- 4.2  чистый бас (на helmholtz.png — пила с плоским верхом)
  4.2- 5.7  морф обратно в скрежет
  6.3+      толчок: струны сталкиваются -> негармонический звон
Если звук не тот, крути в коде:
  regime()   — скорость/прижим/ширину спада/точку/шум как функцию R
  CST        — статика трения (больше = липче, меньше = больше срывов)
  GAP/KC/CC  — зазор и жёсткость столкновения
  CA         — общее затухание;  BR — ВЧ-завал
""")