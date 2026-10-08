#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
stage0_proto.py — Этап 0: офлайн-прототип физмод-ядра (NumPy, один файл).

Архитектура 1:1 с будущим C++-ядром:
  * MD-струна  : цепочка N масс, semi-implicit Euler, оверсэмплинг OS x.
  * Смычок     : трение MSW (термальная кривая + кинетический пол),
                 макро Regime R in [0..1]: 0 = скрежет, 1 = чистый бас.
                 Прижим по Шелленгу: F_s = mult(R) * 2*Z0*v_bow(R).
                 Около прилипания трение решается неявно.
  * Коллизии   : penalty Hunt-Crossley F = k*delta^1.5*(1+chi*ddelta), F >= 0.
  * Рендер     : WAV 44.1 kHz stereo (мостовые силы струн A и B)
                 + helmholtz.png (пила Хельмгольца, расписание R, коллизия).

Запуск (Windows, Git Bash):
  python stage0_proto.py           # полный рендер 10 с (~2-5 мин CPU)
  python stage0_proto.py --quick   # смоук-тест 2.2 с (~30-60 с)
Графики опциональны: python -m pip install matplotlib
"""
import sys, time, wave, math
import numpy as np

try:                                                   # FIX: matplotlib опционален
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    HAVE_MPL = True
except Exception:
    HAVE_MPL = False

# ============================ 1. ПАРАМЕТРЫ ==================================
SR       = 44100
OS       = 4
SRS      = SR * OS
DT       = 1.0 / SRS

T_TOTAL  = 10.0
T_STRIKE = 7.4
GAP      = 3.5e-3
STRIKE_V = 4.5

F0_A = 110.0
L    = 0.60
MU   = 3.44e-3
N    = 64
F0_B = 165.0 * 2.0 ** (0.30 / 12.0)

SIGMA_AIR = 0.30
ETA_CD    = 0.010

REG_VB   = (0.05, 0.22)
REG_MULT = (14.0, 4.5)
REG_VF   = (0.016, 0.030)
REG_BETA = (0.065, 0.14)
KD       = 0.25
NOISE_MAX = 0.35

R_SCHED = [(0.0, 0.0), (1.5, 0.0), (3.0, 1.0), (4.5, 1.0), (6.0, 0.0),
           (T_TOTAL, 0.0)]

KC     = 1.2e5
CHI    = 0.35
KC_CHI = KC * CHI
NEXP   = 1.5

V_GUARD = 60.0

# ============================ 2. КОНСТРУКТОР СТРУНЫ =========================
def make_string(f0):
    a  = L / (N + 1)
    m  = MU * a
    c  = 2.0 * L * f0
    k  = m * (c / a) ** 2
    Z0 = MU * c
    cd = ETA_CD * Z0
    T  = MU * c * c
    return dict(a=a, m=m, c=c, k=k, Z0=Z0, cd=cd, T=T,
                f_top=2.0 * math.sqrt(k / m) / (2.0 * math.pi))

SA = make_string(F0_A)
SB = make_string(F0_B)

# ============================ 3. СИМУЛЯЦИЯ ==================================
def run(quick=False):
    t_end    = 2.2 if quick else T_TOTAL
    t_strike = 1.5 if quick else T_STRIKE
    n_steps  = int(t_end * SRS)
    n_frames = int(t_end * SR)
    step_strike = int(t_strike * SRS)

    t_sim = np.arange(n_steps) * DT
    r_lin = np.interp(t_sim, [p[0] for p in R_SCHED], [p[1] for p in R_SCHED])
    R_sim = r_lin * r_lin * (3.0 - 2.0 * r_lin)
    vb_sim   = REG_VB[0]   + (REG_VB[1]   - REG_VB[0])   * R_sim
    mult_sim = REG_MULT[0] + (REG_MULT[1] - REG_MULT[0]) * R_sim
    vf_sim   = REG_VF[0]   + (REG_VF[1]   - REG_VF[0])   * R_sim
    beta_sim = REG_BETA[0] + (REG_BETA[1] - REG_BETA[0]) * R_sim
    Fs_sim   = mult_sim * 2.0 * SA["Z0"] * vb_sim

    i_f = np.clip(beta_sim * (N + 1) - 1.0, 0.0, N - 2)
    i1_arr = np.floor(i_f).astype(np.int64)
    w2_arr = i_f - i1_arr
    i1_l, w2_l = i1_arr.tolist(), w2_arr.tolist()

    rng = np.random.default_rng(20260101)
    wn  = rng.standard_normal(n_steps + 64)
    ker_t = np.arange(64) / SRS
    ker   = np.exp(-2.0 * math.pi * 1500.0 * ker_t)
    ker  /= np.sqrt(np.sum(ker * ker))
    n_hat = np.convolve(wn, ker)[:n_steps]
    n_hat /= (np.sqrt(np.mean(n_hat * n_hat)) + 1e-12)
    noise_amp_sim = NOISE_MAX * (1.0 - R_sim)
    n_l = n_hat.tolist()

    yA = np.zeros(N); vA = np.zeros(N)
    yB = np.zeros(N); vB = np.zeros(N)
    strike_shape = np.sin(np.pi * (np.arange(N) + 1) / (N + 1))

    segA_e = np.empty(N + 1); segA_v = np.empty(N + 1)
    segB_e = np.empty(N + 1); segB_v = np.empty(N + 1)
    TA = np.empty(N + 1); TB = np.empty(N + 1)
    fA = np.empty(N); fB = np.empty(N)
    dc = np.empty(N); dvel = np.empty(N); Fc = np.empty(N)

    recA = np.zeros(n_frames); recB = np.zeros(n_frames)
    vbp_tr = np.zeros(n_steps, dtype=np.float32)
    Fs_tr  = np.zeros(n_steps, dtype=np.float32)
    R_tr   = np.zeros(n_steps, dtype=np.float32)

    kA, cdA, mA = SA["k"], SA["cd"], SA["m"]
    kB, cdB, mB = SB["k"], SB["cd"], SB["m"]
    airA = 1.0 - SIGMA_AIR * DT
    KD_ = KD
    guard_hits = 0
    accA = 0.0; accB = 0.0
    t0 = time.time()

    for s in range(n_steps):
        segA_e[0] = yA[0];  segA_v[0] = vA[0]
        np.subtract(yA[1:], yA[:-1], out=segA_e[1:N])
        np.subtract(vA[1:], vA[:-1], out=segA_v[1:N])
        segA_e[N] = -yA[N - 1]; segA_v[N] = -vA[N - 1]
        np.multiply(kA, segA_e, out=TA); TA += cdA * segA_v
        np.subtract(TA[1:], TA[:-1], out=fA)
        bridgeA = TA[0]

        segB_e[0] = yB[0];  segB_v[0] = vB[0]
        np.subtract(yB[1:], yB[:-1], out=segB_e[1:N])
        np.subtract(vB[1:], vB[:-1], out=segB_v[1:N])
        segB_e[N] = -yB[N - 1]; segB_v[N] = -vB[N - 1]
        np.multiply(kB, segB_e, out=TB); TB += cdB * segB_v
        np.subtract(TB[1:], TB[:-1], out=fB)
        bridgeB = TB[0]

        np.add(yA, yB, out=dc)
        dc -= GAP
        np.maximum(dc, 0.0, out=dc)
        if dc.max() > 0.0:
            np.power(dc, NEXP, out=dc)
            np.add(vA, vB, out=dvel)
            Fc[:] = dc * (KC + KC_CHI * dvel)
            np.maximum(Fc, 0.0, out=Fc)
            fA -= Fc; fB -= Fc

        # ---- смычок (неявный около прилипания)
        vb  = float(vb_sim[s])
        Fs  = float(Fs_sim[s]); vf = float(vf_sim[s])
        cv0 = Fs * (2.0 + KD_) / vf
        hn  = Fs * float(noise_amp_sim[s]) * n_l[s]
        i1  = i1_l[s]; w2 = w2_l[s]; w1 = 1.0 - w2
        vbp = 0.0
        for idx, w in ((i1, w1), (i1 + 1, w2)):
            if w <= 0.0:
                continue
            vi = float(vA[idx])
            u  = vb - vi
            vbp += w * vi
            x = u / vf
            if abs(u) < vf:
                resid = Fs * (2.0 * x / (1.0 + x * x) + KD_ * math.tanh(x)) - cv0 * u
                vA[idx] = (vi + DT / mA * (fA[idx] + w * (cv0 * vb + resid + hn))) \
                          / (1.0 + DT * w * cv0 / mA)
            else:
                Fb = Fs * (2.0 * x / (1.0 + x * x) + KD_ * math.tanh(x))
                vA[idx] = vi + DT / mA * (fA[idx] + w * (Fb + hn))
            fA[idx] = 0.0                       # FIX: сила уже учтена выше,
                                                # иначе общий шаг добавит её 2-й раз
            vbp += w * float(vA[idx])
        vbp_tr[s] = vbp; Fs_tr[s] = Fs; R_tr[s] = R_sim[s]

        vA += DT * fA / mA
        yA += DT * vA
        vA *= airA

        if s == step_strike:
            vB += STRIKE_V * strike_shape

        vB += DT * fB / mB
        yB += DT * vB
        vB *= airA

        accA += bridgeA; accB += bridgeB
        if (s % OS) == OS - 1:
            fi = s // OS
            if fi < n_frames:
                recA[fi] = accA / OS; recB[fi] = accB / OS
            accA = 0.0; accB = 0.0

        if (s & 4095) == 0:                     # FIX: guards отдельно
            if not (np.isfinite(vA).all() and np.isfinite(vB).all()):
                raise RuntimeError(f"NaN/Inf на шаге {s} (t={s*DT:.3f} c)")
            if np.abs(vA).max() > V_GUARD or np.abs(vB).max() > V_GUARD:
                guard_hits += 1
                np.clip(vA, -V_GUARD, V_GUARD, out=vA)
                np.clip(vB, -V_GUARD, V_GUARD, out=vB)

        if (s % (SRS // 2)) == 0:               # FIX: прогресс отдельно,
            el = time.time() - t0               # теперь печатается каждые 0.5 с
            print(f"  t={s*DT:5.2f} c | {el:6.1f} c CPU | R={R_sim[s]:.2f} "
                  f"| max|yA|={np.abs(yA).max()*1e3:.2f} мм "
                  f"| max|yB|={np.abs(yB).max()*1e3:.2f} мм", flush=True)

    wall = time.time() - t0
    return recA, recB, vbp_tr, Fs_tr, R_tr, t_sim, wall, guard_hits

# ============================ 4. ДИАГНОСТИКА ================================
def diagnostics(recA, recB, SR_):
    def db(x):
        r = np.sqrt(np.mean(x * x)) + 1e-12
        return 20.0 * math.log10(r + 1e-12)
    seg = lambda a, b: slice(int(a * SR_), int(b * SR_))
    print("\n--- Диагностика (RMS, dBFS относительно 1.0) ---")
    for name, a, b in (("скрежет 0.3-1.4с", 0.3, 1.4),
                       ("бас 3.2-4.4с", 3.2, 4.4),
                       ("коллизия 7.4-9.9с", 7.4, 9.9)):
        if b * SR_ >= len(recA): continue
        print(f"  {name}: A={db(recA[seg(a,b)]):6.1f}  B={db(recB[seg(a,b)]):6.1f}")
    x = recA[seg(3.6, 4.4)].copy(); x -= x.mean()
    if len(x) > 256:
        ac = np.correlate(x, x, "full")[len(x) - 1:]
        lo, hi = int(SR_ / 250), int(SR_ / 80)
        lag = lo + int(np.argmax(ac[lo:hi]))
        print(f"  f0 (бас, автокорр) = {SR_/lag:.1f} Гц (цель 110.0)")
    vb_bass = REG_VB[1]; beta_bass = REG_BETA[1]
    print(f"  ожидаемый размах пилы: +{vb_bass:.2f} ... "
          f"{-vb_bass*(1.0-beta_bass)/beta_bass:.2f} м/с")

# ============================ 5. ГРАФИКА ====================================
def plots(recA, recB, vbp_tr, Fs_tr, R_tr, t_sim, SR_, quick=False):
    fig, ax = plt.subplots(2, 2, figsize=(12.5, 8.5), constrained_layout=True)
    fig.suptitle("Этап 0 — прототип физмод-ядра (MD-струны, смычок MSW, "
                 "Hunt-Crossley)", fontsize=13)

    t0, t1 = 0.9, 0.93
    m = (t_sim >= t0) & (t_sim <= t1)
    a = ax[0, 0]
    a.plot(t_sim[m] * 1000, vbp_tr[m], lw=0.8, color="#b58900",
           label="скорость струны")
    a.hlines(REG_VB[0], t0 * 1000, t1 * 1000,
             color="#cb4b16", ls="--", lw=1, label="скорость смычка")
    a.set_title(f"Скрежет (R=0), {int((t1-t0)*1000)} мс — stick-slip")
    a.set_xlabel("мс"); a.set_ylabel("м/с"); a.legend(fontsize=8)

    t0, t1 = 3.9, 3.93
    m = (t_sim >= t0) & (t_sim <= t1)
    a = ax[0, 1]
    a.plot(t_sim[m] * 1000, vbp_tr[m], lw=0.9, color="#268bd2")
    a.hlines(REG_VB[1], t0 * 1000, t1 * 1000, color="#cb4b16", ls="--", lw=1)
    a.set_title("Бас (R=1): ожидается пила Хельмгольца")
    a.set_xlabel("мс"); a.set_ylabel("м/с")

    a = ax[1, 0]
    dec = OS * 100
    tt = t_sim[::dec]
    a.plot(tt, R_tr[::dec], lw=1.5, color="#6c71c4", label="Regime R")
    a.plot(tt, Fs_tr[::dec] / np.max(Fs_tr), lw=1.0, color="#859900",
           label="F_s (норм.)")
    a.axvline(T_STRIKE if not quick else 1.5, color="#dc322f", ls=":",
              lw=1.2, label="удар по B")
    a.set_title("Макро Regime R(t) и прижим F_s(t)")
    a.set_xlabel("с"); a.legend(fontsize=8, loc="upper left")

    a = ax[1, 1]
    ts = np.arange(len(recA)) / SR_
    m = (ts >= (T_STRIKE - 0.1 if not quick else 1.4)) & \
        (ts <= (T_STRIKE + 1.0 if not quick else 2.2))
    pk = np.max(np.abs(recA[m])) + 1e-12
    a.plot(ts[m], recA[m] / pk, lw=0.4, color="#268bd2", alpha=0.8,
           label="мост A (смычковая)")
    a.plot(ts[m], recB[m] / pk, lw=0.4, color="#dc322f", alpha=0.8,
           label="мост B (ударенная)")
    a.set_title("Коллизия: негармонический звон двух струн")
    a.set_xlabel("с"); a.legend(fontsize=8)

    for r in ax:
        for a_ in r: a_.grid(alpha=0.25)
    fig.savefig("helmholtz.png", dpi=140)
    print("Сохранено: helmholtz.png")

# ============================ 6. WAV ========================================
def write_wav(recA, recB, path):
    peak = max(np.max(np.abs(recA)), np.max(np.abs(recB)))
    print(f"Пик мостовой силы до нормировки: {peak:.3f} Н")
    g = 0.89 / (peak + 1e-12)
    Lch = (recA * g * 32767).astype(np.int16)
    Rch = (recB * g * 32767).astype(np.int16)
    inter = np.empty(len(recA) * 2, dtype=np.int16)
    inter[0::2] = Lch; inter[1::2] = Rch
    with wave.open(path, "wb") as w:
        w.setnchannels(2); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes(inter.tobytes())
    print(f"Сохранено: {path}")

# ============================ 7. MAIN =======================================
if __name__ == "__main__":
    quick = "--quick" in sys.argv
    print(f"=== Этап 0: {'смоук-тест' if quick else 'полный рендер'} ===")
    print(f"Струна A: f0={F0_A:.1f} Гц, Z0={SA['Z0']:.3f} кг/с, T={SA['T']:.1f} Н, "
          f"k={SA['k']:.0f} Н/м, m={SA['m']*1e6:.1f} мг, f_top={SA['f_top']:.0f} Гц")
    print(f"Струна B: f0={F0_B:.1f} Гц, Z0={SB['Z0']:.3f} кг/с, T={SB['T']:.1f} Н")
    wmax = max(SA["f_top"], SB["f_top"]) * 2 * math.pi
    print(f"Устойчивость: omega_max*dt = {wmax*DT:.3f} (нужно < 2)")
    recA, recB, vbp_tr, Fs_tr, R_tr, t_sim, wall, gh = run(quick=quick)
    print(f"\nРендер завершён за {wall:.1f} с CPU "
          f"({wall/(t_sim[-1]+DT):.2f} c CPU на 1 c звука); guard: {gh}")
    if not quick:
        diagnostics(recA, recB, SR)
    write_wav(recA, recB, "bow_morph_render.wav")
    if HAVE_MPL:
        plots(recA, recB, vbp_tr, Fs_tr, R_tr, t_sim, SR, quick)
    else:
        print("matplotlib не установлен — графики пропущены "
              "(python -m pip install matplotlib)")