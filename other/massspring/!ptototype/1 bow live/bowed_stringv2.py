#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# bowed_string.py — стабильная версия: mass-spring струны + смычок + GUI
#
# Переписано с нуля (референс: glassg333/mmnova, other/massspring
# "!ptototype/1 bow live"). Исправлены баги их версий:
#
#  1. ВЗРЫВ СТРУНЫ (в т.ч. при натяжении -> 0 и у рисованных струн):
#     у них шаг интегрирования фиксирован, а жёсткость растёт с
#     натяжением/нелинейностью — при большом T (или при k4, оставшемся
#     при T=0) w*dt >> 2 и symplectic-Euler расходится. Здесь шаг по
#     времени выбирается АВТОМАТИЧЕСКИ от максимальной собственной
#     частоты системы: ovs = ceil(w_max / (0.9*SR)) — при любом
#     натяжении w*dt <= 0.9. При T=0 жёсткость равна нулю, струна не
#     «взрывается», а честно провисает (гравитация + демпфер + «пол»).
#
#  2. КЛИКИ/ТРЕСК СМЫЧКА (chatter): добавлен LPF на коэффициент трения
#     (anti-chatter), плавный ramp вкл/выкл, сглаженная Stribeck-кривая.
#
#  3. РИСОВАННЫЕ СТРУНЫ ТЕРЯЛИ НАТЯЖЕНИЕ/«МЕСТА»: здесь натяжение и
#     материал — ГЛОБАЛЬНЫЕ параметры, пересчитываются каждый блок для
#     ВСЕХ струн; rest-длины пружин фиксируются один раз при создании.
#
#  4. ЭНЕРГО-СТРАЖ срабатывал постоянно: демпфирование здесь гарантированно
#     устойчивое (br и cam клипуются по dt), страж — последний предохранитель.
#
# Физика: цепочка масс m = mu*dx, пружины k = T/dx (T — натяжение),
# semi-implicit (symplectic) Euler, сила-лапласиан + нелинейность
# («резинка») + изгибная жёсткость k4 (ингармоника) + демпфирование по
# скорости. Смычок: stick-slip по Stribeck на выделенной струне.
# Струны сталкиваются друг с другом (зазор GAP) — ингармонический звон.
#
# Зависимости: python -m pip install numpy numba sounddevice
# Запуск:  python bowed_string.py             — GUI + звук в реальном времени
#          python bowed_string.py render      — рендер bow_render.wav (без GUI/аудио)
#          python bowed_string.py diag        — список аудио-устройств
# Тесты:   python test_dsp.py                 — headless-проверка стабильности
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np

try:
    from numba import njit
    NUMBA_OK = True
except Exception:                      # fallback: медленно, но работает
    NUMBA_OK = False
    def njit(*a, **k):
        if a and callable(a[0]):
            return a[0]
        def deco(f):
            return f
        return deco

# ---------------- константы ----------------
SR, BLOCK = 44100, 512
MAX_STR, N = 6, 48                     # макс. струн, узлов на струну
MU = 0.04                              # погонная масса, кг/м
T0 = 949.0                             # натяжение при tension=1 (Н) -> f0=110 Гц для L=0.7 м
INHARM_MAX = 2.0e-5                    # макс. доля изгибной жёсткости (как у них)
GAP, KC, CC, FCAP = 1.5e-3, 8.0e5, 0.5, 150.0   # коллизии струн
CST, FR = 1.0, 0.7                     # смычок: множитель силы, статич. трение
KG, CG, GCAP = 150.0, 1.2, 0.02        # «рука» (grab)
VMAX, EGUARD = 80.0, 64.0              # клип скорости, порог энерго-стража (v_rms=8)
Y_FLOOR = -0.10                        # «пол» под струной (провисание при T=0)
J1, J2, W1, W2 = int(0.28 * N) + 1, int(0.66 * N) + 1, 2.5, 1.2   # звукосниматель
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)          # DC-blocker
QMIN = {"АВТО": 1, "ЭКО": 1, "НОРМА": 2, "МАКС": 4}  # мин. оверсэмплинг
CFL = 0.9                                        # w_max*dt <= CFL

BODYF = np.array([95.0, 160.0, 220.0, 380.0, 720.0, 1150.0])
BODYQ = np.array([6.0, 8.0, 9.0, 11.0, 13.0, 15.0])
BODYG = np.array([0.60, 0.50, 0.45, 0.40, 0.30, 0.22])

def body_coeffs(sr):
    r2 = np.empty(6); c1 = np.empty(6); b0 = np.empty(6)
    for m in range(6):
        w = 2 * math.pi * BODYF[m] / sr
        r = math.exp(-w / (2 * BODYQ[m]))
        r2[m] = r * r; c1[m] = 2 * r * math.cos(w); b0[m] = 0.5 * (1 - r * r)
    return r2, c1, b0

BR2, BC1, BB0 = body_coeffs(SR)
BS1 = np.zeros(6); BS2 = np.zeros(6)

# ---------------- состояние ----------------
Y = np.zeros((MAX_STR, N + 2))         # поперечное смещение (м), [струна, узел]
V = np.zeros((MAX_STR, N + 2))         # поперечная скорость (м/с)
Y0 = np.zeros((MAX_STR, N + 2))        # начальный профиль (для сброса)
KS = np.zeros(MAX_STR)                 # k при tension=1:  k = T0/DX
MS = np.zeros(MAX_STR)                 # масса узла m = MU*DX
K4S = np.zeros(MAX_STR)                # k4 при tension=1, inhar=1
X0S = np.zeros(MAX_STR)                # начало струны по X (мировое, 0..1)
DXS = np.zeros(MAX_STR)                # шаг узлов DX = L/(N+1)
BASES = np.zeros(MAX_STR)              # базовая высота струны (мировое Y)
STATS = np.zeros(4)                    # clips, vclips, stick, contacts
MUZ = np.zeros(1)                      # LPF-состояние коэффициента трения
DCS = np.zeros(2)                      # DC-blocker

NSTR = [0]
SEL = [0]

P = dict(tension=1.0, stroy=1.0, nl=0.05, inhar=0.5, ca=0.10, br=0.002,
         grav=0.0, speed=1.0, press=1.2, beta=0.12, noise=0.25, R=0.0,
         automorph=True, vol=0.5, bow=True, body=True, qual="АВТО")

class ST: pass
STt = ST()
STt.ovs = 1; STt.t = 0.0; STt.on = 0.0; STt.R_now = 0.0
STt.grab = None; STt.draw_pts = None
STt.rec = False; STt.recl = []
STt.level = 0.0; STt.gain = 0.0; STt.rms_ema = 0.0
STt.cpu = 0.0; STt.underr = 0; STt.err_n = 0
STt.guard_count = 0; STt.guard_last = 0.0
STt.stick = 0.0; STt.logq = []
STt.snap = np.zeros((MAX_STR, N))
STt.last_draw_err = ""; STt.last_tick_err = ""; STt.cpu_warned = False

RNG = np.random.default_rng(7)

def log(msg, major=True):
    STt.logq.append((major, msg))

def log_flush(lines):
    try:
        with open(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                               "physmod.log"), "a", encoding="utf-8") as f:
            for L in lines:
                f.write(L + "\n")
    except Exception:
        pass

# ---------------- ядро физики (numba) ----------------
@njit(cache=True, fastmath=True, nogil=True)
def core(Y, V, nstr, KS, MS, K4S, X0S, DXS, BASES,
         TENS, INHAR, NL, NLCLAMP, BRF, CA, GRAV,
         GAP, KC, CC, FCAP,
         bow_on, bow_s, jb, vb0, fn0, VF, FR, CST, NOISE, nzarr, ALP, ALP2,
         grab_s, grab_j, gtx, KG, CG, GCAP,
         ovs, dt, vmax, yfloor, J1, J2, W1, W2, N,
         body_on, bc1, br2, bb0, bgn, bs1, bs2, dca, dcs,
         out, stats, muz):
    clips = 0.0; vclips = 0.0; stick = 0.0; ncon = 0.0
    nzlp = 0.0; muzlp = muz[0]
    dcx = dcs[0]; dcy = dcs[1]
    frames = out.shape[0]
    for f in range(frames):
        for o in range(ovs):
            # --- силы в струнах ---
            for s in range(nstr):
                k = KS[s] * TENS
                k4 = K4S[s] * TENS * INHAR
                m = MS[s]; im = dt / m
                br = BRF * math.sqrt(k * m)          # скоростной демпфер
                brmax = 0.25 * m / dt                # гарантия устойчивости
                if br > brmax: br = brmax
                cam = CA * m                         # глобальный демпфер
                cammax = 0.5 / dt
                if cam > cammax: cam = cammax
                for j in range(1, N + 1):
                    curv = Y[s, j - 1] - 2.0 * Y[s, j] + Y[s, j + 1]
                    frc = k * curv + br * (V[s, j - 1] - 2.0 * V[s, j]
                                           + V[s, j + 1]) - cam * V[s, j]
                    if NL > 0.0:                     # нелинейность «резинка»
                        mm = 1.0 + NL * ((Y[s, j] - Y[s, j - 1]) ** 2
                                         + (Y[s, j + 1] - Y[s, j]) ** 2)
                        if mm > NLCLAMP: mm = NLCLAMP
                        frc += k * curv * (mm - 1.0)
                    if k4 > 0.0 and j >= 2 and j <= N - 1:   # изгиб (ингармоника)
                        frc -= k4 * (Y[s, j - 2] - 4.0 * Y[s, j - 1]
                                     + 6.0 * Y[s, j] - 4.0 * Y[s, j + 1]
                                     + Y[s, j + 2])
                    V[s, j] += frc * im
                if GRAV != 0.0:
                    for j in range(1, N + 1):
                        V[s, j] += GRAV * dt
            # --- «пол» (мягкий): при T=0 струна провисает и лежит ---
            for s in range(nstr):
                for j in range(1, N + 1):
                    if Y[s, j] < yfloor:
                        Y[s, j] = yfloor
                        if V[s, j] < 0.0:
                            V[s, j] *= -0.5
            # --- коллизии струн друг с другом (merge по X) ---
            for s1 in range(nstr):
                for s2 in range(s1 + 1, nstr):
                    if abs(BASES[s1] - BASES[s2]) > 0.09:
                        continue
                    lo = X0S[s1] if X0S[s1] > X0S[s2] else X0S[s2]
                    hi1 = X0S[s1] + (N + 1) * DXS[s1]
                    hi2 = X0S[s2] + (N + 1) * DXS[s2]
                    hi = hi1 if hi1 < hi2 else hi2
                    if hi - lo < 0.02:
                        continue
                    i = int((lo - X0S[s1]) / DXS[s1]) + 1
                    if i < 1: i = 1
                    jj = int((lo - X0S[s2]) / DXS[s2]) + 1
                    if jj < 1: jj = 1
                    while i <= N and jj <= N:
                        xi = X0S[s1] + i * DXS[s1]
                        xj = X0S[s2] + jj * DXS[s2]
                        if xi > hi or xj > hi: break
                        if abs(xi - xj) < 0.5 * (DXS[s1] + DXS[s2]):
                            dy = (BASES[s1] + Y[s1, i]) - (BASES[s2] + Y[s2, jj])
                            pen = GAP - abs(dy)
                            if pen > 0.0:
                                sg = 1.0 if dy >= 0.0 else -1.0
                                dmp = 1.0 + CC * (V[s2, jj] - V[s1, i]) * sg
                                if dmp < 0.0: dmp = 0.0
                                elif dmp > 2.0: dmp = 2.0
                                fc = KC * pen ** 1.5 * dmp
                                if fc > FCAP:
                                    fc = FCAP; clips += 1.0
                                V[s1, i] += sg * fc * dt / MS[s1]
                                V[s2, jj] -= sg * fc * dt / MS[s2]
                                ncon += 1.0
                        if xi < xj: i += 1
                        else: jj += 1
            # --- смычок (stick-slip, Stribeck + LPF anti-chatter) ---
            if bow_on > 0.0 and fn0 > 0.0 and bow_s >= 0:
                nzl = nzlp + ALP * (nzarr[f * ovs + o] - nzlp)
                nzlp = nzl
                fn = fn0 * (1.0 + NOISE * nzl) * bow_on
                if fn < 0.0: fn = 0.0
                vb_t = vb0 * bow_on
                vr = vb_t - V[bow_s, jb]
                mu = FR + (1.0 - FR) * math.exp(-abs(vr) / VF)
                muzlp += ALP2 * (mu - muzlp)
                cap = CST * fn * muzlp * dt / MS[bow_s]
                if abs(vr) <= cap:
                    V[bow_s, jb] = vb_t
                    stick += 1.0
                else:
                    V[bow_s, jb] += math.copysign(cap, vr)
            # --- «рука» (grab окном ±4) ---
            if grab_s >= 0:
                im = dt / MS[grab_s]
                lo = grab_j - 4; hi = grab_j + 4
                if lo < 1: lo = 1
                if hi > N: hi = N
                for j in range(lo, hi + 1):
                    w = 1.0 - abs(j - grab_j) / 5.0
                    if w > 0.0:
                        frc = w * (KG * (gtx - Y[grab_s, j])
                                   - CG * V[grab_s, j])
                        dv = frc * im
                        if dv > GCAP: dv = GCAP; clips += 1.0
                        elif dv < -GCAP: dv = -GCAP; clips += 1.0
                        V[grab_s, j] += dv
            # --- интегрирование (semi-implicit Euler) ---
            for s in range(nstr):
                for j in range(1, N + 1):
                    Y[s, j] += V[s, j] * dt
            # --- клип скоростей (страховка) ---
            for s in range(nstr):
                for j in range(1, N + 1):
                    v2 = V[s, j] * V[s, j]
                    if v2 > vmax * vmax:
                        V[s, j] *= vmax / math.sqrt(v2)
                        vclips += 1.0
        # --- звукосниматель + боди + DC-blocker ---
        x = 0.0
        for s in range(nstr):
            x += W1 * V[s, J1] + W2 * V[s, J2]
        if body_on > 0.5:
            wet = 0.0
            for q in range(6):
                yb = bb0[q] * x + bc1[q] * bs1[q] - br2[q] * bs2[q]
                bs2[q] = bs1[q]; bs1[q] = yb
                wet += bgn[q] * yb
            x = 0.8 * x + 0.5 * wet
        xs = x - dcx + dca * dcy
        dcx = x; dcy = xs
        out[f] = xs
    dcs[0] = dcx; dcs[1] = dcy
    stats[0] = clips; stats[1] = vclips; stats[2] = stick; stats[3] = ncon
    muz[0] = muzlp

# ---------------- управление струнами ----------------
def reset_positions():
    Y[:] = Y0; V[:] = 0.0
    DCS[:] = 0.0; BS1[:] = 0.0; BS2[:] = 0.0; MUZ[0] = 0.0

def reset_all():
    NSTR[0] = 0; SEL[0] = 0
    Y[:] = 0.0; V[:] = 0.0; Y0[:] = 0.0
    KS[:] = 0.0; MS[:] = 0.0; K4S[:] = 0.0
    X0S[:] = 0.0; DXS[:] = 0.0; BASES[:] = 0.0
    STATS[:] = 0.0; MUZ[0] = 0.0; DCS[:] = 0.0
    BS1[:] = 0.0; BS2[:] = 0.0
    STt.ovs = 1; STt.t = 0.0; STt.on = 0.0; STt.R_now = 0.0
    STt.grab = None; STt.draw_pts = None
    STt.rec = False; STt.recl = []
    STt.level = 0.0; STt.gain = 0.0; STt.rms_ema = 0.0
    STt.cpu = 0.0; STt.underr = 0; STt.err_n = 0
    STt.guard_count = 0; STt.guard_last = 0.0; STt.stick = 0.0
    STt.logq.clear(); STt.snap[:] = 0.0
    STt.last_draw_err = ""; STt.last_tick_err = ""; STt.cpu_warned = False

def f0_of(s):
    T = T0 * P["tension"] * P["stroy"]
    if T <= 0.0 or s >= NSTR[0]:
        return 0.0
    L = (N + 1) * DXS[s]
    return math.sqrt(T / MU) / (2.0 * L)

def add_string(x0, x1, base, prof=None):
    """Создать струну от x0 до x1 (мировые X, 0..1) на высоте base.
    prof — начальный профиль отклонений (м), например после рисования."""
    if NSTR[0] >= MAX_STR:
        log("лимит струн (6) — C очистить", True); return -1
    if x1 - x0 < 0.06:
        log("линия слишком короткая (<6 см)", True); return -1
    s = NSTR[0]
    L = x1 - x0; DX = L / (N + 1)
    KS[s] = T0 / DX                    # k = T/DX при tension=1
    MS[s] = MU * DX
    K4S[s] = INHARM_MAX * (N + 1) ** 2 * KS[s]
    X0S[s] = x0; DXS[s] = DX; BASES[s] = base
    Y[s, :] = 0.0; V[s, :] = 0.0
    if prof is not None:
        Y[s, 1:N + 1] = np.clip(np.asarray(prof)[1:N + 1], -0.035, 0.035)
    Y0[s, :] = Y[s, :]
    NSTR[0] = s + 1
    f0 = f0_of(s)
    log(f"СТРУНА #{s}: L={L:.2f} м, y={base:.2f}, f0≈{f0:.0f} Гц "
        f"(T={P['tension']:.2f})", True)
    return s

def pluck_string(s, j, sgn=1.0):
    """Щипок: гладкий импульс скорости по гауссиане (как у них)."""
    if s < 0 or s >= NSTR[0]:
        return
    j = max(1, min(N, j))
    for d in range(-4, 5):
        jj = j + d
        if 1 <= jj <= N:
            V[s, jj] += sgn * 0.7 * math.exp(-(d / 2.5) ** 2)

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

def R_auto(ph):
    if ph < 1.2: return 0.0
    if ph < 2.7: return smoothstep((ph - 1.2) / 1.5)
    if ph < 4.2: return 1.0
    if ph < 5.7: return 1.0 - smoothstep((ph - 4.2) / 1.5)
    return 0.0

# ---------------- аудио-пайплайн ----------------
def process_block(frames):
    nstr = NSTR[0]
    TENS = P["tension"] * P["stroy"]
    nl = P["nl"]; nlclamp = 1.0 + 2.0 * nl
    inhar = P["inhar"]
    # --- авто-оверсэмплинг по CFL: w_max*dt <= 0.9 при ЛЮБОМ натяжении ---
    wmax = 0.0
    for s in range(nstr):
        k = KS[s] * TENS; m = MS[s]
        w = 2.0 * math.sqrt(k / m) + 4.0 * math.sqrt(K4S[s] * TENS * inhar / m)
        if w > wmax: wmax = w
    if nl > 0.0:
        wmax *= math.sqrt(1.0 + 2.0 * nl)
    ovs = int(math.ceil(wmax / (CFL * SR))) if wmax > 0.0 else 1
    ovs = max(ovs, QMIN[P["qual"]]); ovs = min(ovs, 32)
    if ovs != STt.ovs:
        STt.ovs = ovs
        log(f"оверсэмплинг авто: {ovs}x (w_max={wmax:.0f} рад/с)", False)
    dt = 1.0 / (SR * ovs)
    # --- смычок: параметры ---
    if P["automorph"]:
        R = R_auto(STt.t % 7.0)
    else:
        R = P["R"]
    STt.R_now = R
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    STt.on = min(1.0, max(0.0, STt.on + stp * frames))
    STt.t += frames / SR
    vb0 = (0.06 + 0.34 * R) * P["speed"]
    z0 = math.sqrt(T0 * TENS * MU) if TENS > 0.0 else 0.0   # волновое сопротивление
    fn0 = P["press"] * 0.12 * (6.0 - 4.0 * R) * z0 * vb0
    vf = 0.012 + 0.10 * R
    nzarr = RNG.standard_normal(frames * ovs)
    aLP = 1.0 - math.exp(-2.0 * math.pi * 2500.0 * dt)   # LPF шума волоса
    aLP2 = 1.0 - math.exp(-2.0 * math.pi * 6000.0 * dt)  # LPF трения (anti-chatter)
    # --- ввод ---
    grab_s, grab_j, gtx = -1, 0, 0.0
    if STt.grab is not None:
        grab_s, grab_j, gtx = STt.grab
        gtx = max(-0.06, min(0.06, gtx))
    if nstr > 0:
        bow_s = SEL[0] if 0 <= SEL[0] < nstr else 0
    else:
        bow_s = -1
    jb = max(1, min(N, int(P["beta"] * N) + 1))
    bow_on = 1.0 if (P["bow"] and nstr > 0) else 0.0
    out = np.empty(frames)
    t0c = time.perf_counter()
    core(Y, V, nstr, KS, MS, K4S, X0S, DXS, BASES,
         TENS, inhar, nl, nlclamp, P["br"], P["ca"], -P["grav"] * 9.8,
         GAP, KC, CC, FCAP,
         bow_on, bow_s, jb, vb0, fn0, vf, FR, CST, P["noise"], nzarr,
         aLP, aLP2,
         grab_s, grab_j, gtx, KG, CG, GCAP,
         ovs, dt, VMAX, Y_FLOOR, J1, J2, W1, W2, N,
         1.0 if P["body"] else 0.0, BC1, BR2, BB0, BODYG, BS1, BS2, DCA, DCS,
         out, STATS, MUZ)
    STt.cpu = 0.9 * STt.cpu + 0.1 * (time.perf_counter() - t0c) / (frames / SR)
    # --- энерго-страж (последний предохранитель) ---
    if nstr > 0:
        Vv = V[:nstr, 1:N + 1]
        Yv = Y[:nstr, 1:N + 1]
        if not (np.isfinite(Vv).all() and np.isfinite(Yv).all()):
            reset_positions(); out[:] = 0.0; STt.guard_count += 1
            log("АВАРИЯ (NaN) -> сброс", True)
        else:
            ekin = float(np.mean(Vv * Vv))
            if ekin > EGUARD * EGUARD:
                V[:] *= 0.5; STt.guard_count += 1
                if time.time() - STt.guard_last > 1.0:
                    log(f"ЭНЕРГО-СТРАЖ: v_rms={math.sqrt(ekin):.1f} -> гашение",
                        True)
                    STt.guard_last = time.time()
    # --- AGC + tanh (всегда включены — защита от клипа) ---
    y = out
    rms = float(np.sqrt(np.mean(y * y))) + 1e-12
    STt.rms_ema = 0.9 * STt.rms_ema + 0.1 * rms
    g_t = min(0.10 / max(STt.rms_ema, 1e-9), 3.0)
    if STt.gain == 0.0:
        STt.gain = g_t
    else:
        slew = 0.05 if g_t < STt.gain else 0.008
        STt.gain += max(min(g_t - STt.gain, slew), -slew)
    y = np.tanh(y * (STt.gain * P["vol"] * 3.0))
    STt.level = 0.9 * STt.level + 0.1 * float(np.mean(np.abs(y)))
    STt.snap = Y[:, 1:N + 1].copy()
    STt.stick = 0.9 * STt.stick + 0.1 * STATS[2] / max(frames * ovs, 1)
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status:
            STt.underr += 1
        outdata[:, 0] = process_block(frames)
        if STt.rec:
            STt.recl.append(outdata[:, 0].copy())
    except Exception:
        STt.err_n += 1
        if STt.err_n <= 3:
            import traceback
            traceback.print_exc()
        outdata[:] = 0

def init_engine():
    """Прогрев ядра (компиляция numba, один раз ~20-40 с)."""
    if NSTR[0] == 0:
        add_string(0.15, 0.85, 0.25)
    process_block(64); process_block(64)

def render_wav(path, seconds=6.0):
    """Рендер без GUI/аудио: щипок -> смычок -> щипок. Для тестов/проверки."""
    init_engine()
    reset_all()
    P.update(tension=1.0, stroy=1.0, nl=0.05, inhar=0.5, ca=0.10, br=0.002,
             grav=0.0, bow=False, press=1.2, speed=1.0, beta=0.12, noise=0.25,
             R=0.0, automorph=False, vol=0.6, body=True)
    add_string(0.15, 0.85, 0.25)
    total = int(SR * seconds); out = np.empty(total); pos = 0
    pluck_at = int(0.5 * SR); bow_on_at = int(1.0 * SR)
    bow_off_at = int(4.0 * SR); pluck2_at = int(4.5 * SR)
    print(f"рендер {seconds:.0f} с...")
    while pos < total:
        n = min(BLOCK, total - pos)
        if pos <= pluck_at < pos + n: pluck_string(0, N // 2, 1.0)
        if pos <= bow_on_at < pos + n: P["bow"] = True
        if pos <= bow_off_at < pos + n: P["bow"] = False
        if pos <= pluck2_at < pos + n: pluck_string(0, N // 3, -1.0)
        out[pos:pos + n] = process_block(n)
        pos += n
        if (pos // BLOCK) % (SR // BLOCK) == 0:
            print(f"  {pos / SR:4.1f}/{seconds:.0f} с")
    nf = SR // 5
    out[-nf:] *= np.linspace(1.0, 0.0, nf)
    p = float(np.percentile(np.abs(out), 99.9)) + 1e-9
    out = np.clip(out * 0.85 / p, -1, 1)
    with wave.open(path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((out * 32767).astype("<i2").tobytes())
    print("ГОТОВО:", path)

# ---------------- GUI ----------------
BG, FG = "#14141c", "#e0e0e8"
ZOOM_Y = 600.0
CANH = 420

def run_gui():
    import tkinter as tk
    try:
        import sounddevice as sd
    except Exception:
        print("Нужен sounddevice: python -m pip install sounddevice")
        return
    root = tk.Tk()
    root.title("bowed_string — mass-spring струны + смычок (стабильная версия)")
    root.configure(bg=BG)
    W = 1000
    cv = tk.Canvas(root, width=W, height=CANH, bg="#0d0d14",
                   highlightthickness=1, highlightbackground="#26263a")
    cv.pack(fill="x")

    def node_xy(s, j):
        px_ = 30 + (X0S[s] + j * DXS[s]) * (W - 60)
        # концы струны (j=0 и j=N+1) закреплены: отклонение = 0
        dy = 0.0 if (j <= 0 or j > N) else STt.snap[s, j - 1]
        py_ = CANH - 30 - (BASES[s] + dy) * ZOOM_Y
        return (px_, py_)

    def world(e):
        return ((e.x - 30) / (W - 60), (CANH - 30 - e.y) / ZOOM_Y)

    def draw():
        try:
            cv.delete("all")
            cv.create_text(10, 12, anchor="w", fill="#666680",
                           font=("TkDefaultFont", 9),
                           text="ЛКМ за узел — тянуть | ЛКМ drag с пустого — "
                                "рисовать струну | ПКМ — щипок | ПРОБЕЛ — смычок")
            for s in range(NSTR[0]):
                col = "#4cc2a0" if s == SEL[0] else "#9aa8d8"
                pts = []
                for j in range(0, N + 2):
                    x_, y_ = node_xy(s, j)
                    pts += [x_, y_]
                cv.create_line(*pts, fill=col, width=2 if s == SEL[0] else 1)
                for j in (0, N + 1):               # «мосты» на концах
                    x_, y_ = node_xy(s, j)
                    cv.create_rectangle(x_ - 4, y_ - 6, x_ + 4, y_ + 6,
                                        fill="#8890a8", width=0)
                x_, y_ = node_xy(s, 1)
                cv.create_text(x_ + 6, y_ - 12, anchor="w", fill="#666680",
                               font=("TkDefaultFont", 8),
                               text=f"#{s} f0≈{f0_of(s):.0f} Гц")
            if P["bow"] and STt.on > 0.01 and NSTR[0] > 0:
                s = SEL[0] if 0 <= SEL[0] < NSTR[0] else 0
                jb = max(1, min(N, int(P["beta"] * N) + 1))
                x_, y_ = node_xy(s, jb)
                cv.create_oval(x_ - 5, y_ - 5, x_ + 5, y_ + 5,
                               fill="#ff5040", width=0)
            if STt.draw_pts:
                for i in range(1, len(STt.draw_pts)):
                    x1_, y1_ = STt.draw_pts[i - 1]
                    x2_, y2_ = STt.draw_pts[i]
                    cv.create_line(x1_ * (W - 60) + 30,
                                   CANH - 30 - y1_ * ZOOM_Y,
                                   x2_ * (W - 60) + 30,
                                   CANH - 30 - y2_ * ZOOM_Y,
                                   fill="#ffffff", width=1)
            lv = min(1.0, STt.level * 4)
            cv.create_rectangle(10, CANH - 12, 10 + int((W - 20) * lv),
                                CANH - 4, fill="#40c080", width=0)
        except Exception as e:
            if str(e) != STt.last_draw_err:      # не спамим, но видно
                STt.last_draw_err = str(e)
                log(f"ОШИБКА отрисовки: {e}", True)
            print("draw:", e)
        root.after(40, draw)

    panel = tk.Frame(root, bg=BG); panel.pack(fill="x")

    def mk(row, col, lab, key, a, b, res=0.01):
        tk.Label(panel, text=lab, width=16, anchor="w", bg=BG, fg=FG
                 ).grid(row=row, column=col)
        sc = tk.Scale(panel, from_=a, to=b, resolution=res,
                      orient="horizontal", length=150, showvalue=1,
                      bg=BG, fg=FG, troughcolor="#22222e",
                      highlightthickness=0, activebackground="#33334a",
                      command=(lambda v, k=key: P.__setitem__(k, float(v))))
        sc.set(P[key])
        sc.grid(row=row, column=col + 1, sticky="w")
        return sc

    tk.Label(panel, text="— МАТЕРИАЛ (все струны) —", bg=BG, fg="#ffcc60"
             ).grid(row=0, column=0)
    mk(1, 0, "Натяжение T", "tension", 0.0, 4.0)
    mk(2, 0, "Строй (xT)", "stroy", 0.25, 4.0)
    mk(3, 0, "Нелинейность", "nl", 0.0, 0.3, res=0.005)
    mk(4, 0, "Ингармоника", "inhar", 0.0, 1.0)
    mk(5, 0, "Затухание", "ca", 0.0, 0.5, res=0.005)
    mk(6, 0, "ВЧ-затухание", "br", 0.0, 0.02, res=0.0005)
    mk(7, 0, "Гравитация", "grav", 0.0, 1.0)
    tk.Label(panel, text="— СМЫЧОК / ВЫХОД —", bg=BG, fg="#ffcc60"
             ).grid(row=0, column=2)
    mk(1, 2, "Скорость смычка", "speed", 0.0, 1.6)
    mk(2, 2, "Прижим", "press", 0.0, 2.0)
    mk(3, 2, "Позиция смычка", "beta", 0.02, 0.5, res=0.005)
    mk(4, 2, "Шум волоса", "noise", 0.0, 1.0)
    mk(5, 2, "R скрежет<->бас", "R", 0.0, 1.0)
    mk(6, 2, "Громкость", "vol", 0.0, 1.0)

    buts = tk.Frame(root, bg=BG); buts.pack(fill="x")
    buts2 = tk.Frame(root, bg=BG); buts2.pack(fill="x")

    def dbtn(parent, text, cmd, width=12):
        return tk.Button(parent, text=text, command=cmd, width=width,
                         bg="#1e1e2a", fg=FG, activebackground="#2a2a3c",
                         activeforeground=FG, relief="flat")

    def toggle_bow():
        P["bow"] = not P["bow"]
        b_bow.config(text="СМЫЧОК: ВКЛ" if P["bow"] else "СМЫЧОК: выкл")

    def toggle_body():
        P["body"] = not P["body"]
        b_body.config(text="БОДИ: ВКЛ" if P["body"] else "БОДИ: выкл")

    def toggle_auto():
        P["automorph"] = not P["automorph"]
        b_auto.config(text="АВТО-МОРФ: ВКЛ" if P["automorph"]
                      else "АВТО-МОРФ: выкл")

    def do_clear():
        reset_all()
        log("ОЧИСТКА сцены")

    def do_reset():
        reset_positions()
        log("СБРОС позиций")

    def add_default_string():
        base = 0.10
        while base < 0.45:
            ok = True
            for s in range(NSTR[0]):
                ov = min(0.85, X0S[s] + (N + 1) * DXS[s]) - max(0.15, X0S[s])
                if abs(BASES[s] - base) < 0.035 and ov > 0.3:
                    ok = False; break
            if ok:
                break
            base += 0.05
        add_string(0.15, 0.85, round(base, 3))

    def do_test():
        if NSTR[0] == 0:
            add_string(0.15, 0.85, 0.25)
        SEL[0] = 0
        log("ТЕСТ: автощипок через 0.5 с", True)
        root.after(500, lambda: pluck_string(SEL[0], N // 2, 1.0))

    def rec_toggle():
        if not STt.rec:
            STt.rec = True; STt.recl = []
            b_rec.config(text="ЗАПИСЬ: идёт")
            log("ЗАПИСЬ начата")
        else:
            STt.rec = False
            b_rec.config(text="ЗАПИСЬ")
            if STt.recl:
                x = np.concatenate(STt.recl)
                p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
                x = np.clip(x * 0.9 / p, -1, 1)
                fn = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                  "jam_%s.wav"
                                  % datetime.datetime.now().strftime("%H%M%S"))
                with wave.open(fn, "wb") as w:
                    w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
                    w.writeframes((x * 32767).astype("<i2").tobytes())
                log(f"ЗАПИСЬ сохранена: {os.path.basename(fn)}")

    b_str = dbtn(buts, "СТРУНА+ (S)", add_default_string, 12)
    b_bow = dbtn(buts, "СМЫЧОК: ВКЛ", toggle_bow, 13)
    b_body = dbtn(buts, "БОДИ: ВКЛ", toggle_body, 10)
    b_auto = dbtn(buts, "АВТО-МОРФ: ВКЛ", toggle_auto, 15)
    b_test = dbtn(buts, "ТЕСТ (T)", do_test, 10)
    b_rec = dbtn(buts, "ЗАПИСЬ", rec_toggle, 10)
    b_res = dbtn(buts, "СБРОС (r)", do_reset, 11)
    b_clear = dbtn(buts, "ОЧИСТИТЬ (C)", do_clear, 13)
    b_quit = dbtn(buts, "ВЫХОД", root.destroy, 8)
    for b in (b_str, b_bow, b_body, b_auto, b_test, b_rec, b_res, b_clear,
              b_quit):
        b.pack(side="left", padx=2, pady=3)

    qual = tk.OptionMenu(buts2, tk.StringVar(value=P["qual"]),
                         *QMIN.keys(),
                         command=lambda v: P.__setitem__("qual", v))
    qual.config(width=6, bg="#1e1e2a", fg=FG, activebackground="#2a2a3c",
                relief="flat", highlightthickness=0)
    qual.pack(side="left", padx=2, pady=3)
    struna_var = tk.StringVar(value="#0")
    struna_menu = tk.OptionMenu(buts2, struna_var,
                                *[f"#{i}" for i in range(MAX_STR)],
                                command=lambda v: SEL.__setitem__(
                                    0, int(v[1:])))
    struna_menu.config(width=5, bg="#1e1e2a", fg=FG,
                       activebackground="#2a2a3c", relief="flat",
                       highlightthickness=0)
    struna_menu.pack(side="left", padx=2, pady=3)
    tk.Label(buts2, text="струна для смычка:", bg=BG, fg="#666680"
             ).pack(side="left", padx=(8, 0))

    status = tk.Label(root, text="", anchor="w", bg=BG, fg="#9fdf9f")
    status.pack(fill="x")
    logbox = tk.Text(root, height=6, bg="#0d0d14", fg="#9fdf9f",
                     insertbackground=FG, relief="flat", font=("Consolas", 8))
    logbox.pack(fill="both", expand=True)

    def tick():
        try:
            drain = []
            for major, msg in STt.logq:
                if major:
                    drain.append(f"[{datetime.datetime.now().strftime('%H:%M:%S')}] {msg}")
            STt.logq.clear()
            if drain:
                for L in drain:
                    logbox.insert("end", L + "\n")
                logbox.see("end")
                log_flush(drain)
            s = SEL[0] if 0 <= SEL[0] < NSTR[0] else 0
            status.config(text=f"CPU {min(STt.cpu, 9.99) * 100:5.0f}%  "
                               f"ovs {STt.ovs}x  струн {NSTR[0]}  "
                               f"guard {STt.guard_count}  "
                               f"stick {STt.stick * 100:3.0f}%  "
                               f"R={STt.R_now:.2f}  f0(SEL)≈{f0_of(s):.0f} Гц"
                               + ("  ЗАПИСЬ..." if STt.rec else ""))
            struna_var.set(f"#{s}")
            if STt.cpu > 0.9 and not STt.cpu_warned:
                STt.cpu_warned = True
                log("ВЫСОКАЯ НАГРУЗКА CPU (>90%). Если numba не установлен — "
                    "ядро на чистом Python и будет тормозить. Установи: "
                    "python -m pip install numba", True)
        except Exception as e:
            if str(e) != STt.last_tick_err:
                STt.last_tick_err = str(e)
                log(f"ОШИБКА тика: {e}", True)
            print("tick:", e)
        root.after(200, tick)

    # ---------------- мышь / клавиши ----------------
    def find_node(e, maxd):
        best, bd = (-1, -1), maxd * maxd
        for s in range(NSTR[0]):
            for j in range(1, N + 1):
                x_, y_ = node_xy(s, j)
                d = (e.x - x_) ** 2 + (e.y - y_) ** 2
                if d < bd:
                    bd, best = d, (s, j)
        return best, math.sqrt(bd)

    def finish_draw(pts):
        if len(pts) < 2:
            return
        xs = np.array([p[0] for p in pts]); ys = np.array([p[1] for p in pts])
        x0, x1 = float(xs.min()), float(xs.max())
        x0 = max(0.03, x0); x1 = min(0.97, x1)
        if x1 - x0 < 0.06:
            log("линия слишком короткая (<6 см)", True); return
        base = float(np.mean(ys))
        base = max(0.05, min(0.45, base))
        for _ in range(12):                      # авто-разводка по высоте
            conflict = False
            for s in range(NSTR[0]):
                ov = min(x1, X0S[s] + (N + 1) * DXS[s]) - max(x0, X0S[s])
                if abs(BASES[s] - base) < 0.03 and ov > 0.3:
                    conflict = True; break
            if not conflict:
                break
            base = base + 0.05 if base < 0.25 else base - 0.05
            base = max(0.05, min(0.45, base))
        L = x1 - x0
        order = np.argsort(xs)
        xj = x0 + np.arange(1, N + 1) * L / (N + 1)
        yline = np.interp(xj, xs[order], ys[order])
        prof = np.zeros(N + 2)
        prof[1:N + 1] = yline - base
        add_string(x0, x1, round(base, 3), prof)

    def on_lmb(e):
        (s, j), d = find_node(e, 10.0)
        if s >= 0:
            wx, wy = world(e)
            STt.grab = (s, j, wy - BASES[s])
            SEL[0] = s
        else:
            STt.draw_pts = [world(e)]

    def on_move(e):
        if STt.grab is not None:
            wx, wy = world(e)
            s, j, _ = STt.grab
            STt.grab = (s, j, max(-0.06, min(0.06, wy - BASES[s])))
        elif STt.draw_pts is not None:
            wx, wy = world(e)
            lx, ly = STt.draw_pts[-1]
            if (wx - lx) ** 2 + (wy - ly) ** 2 >= (5.0 / (W - 60)) ** 2:
                STt.draw_pts.append((wx, wy))

    def on_release(e):
        if STt.draw_pts is not None:
            if len(STt.draw_pts) >= 2:
                finish_draw(STt.draw_pts)
            STt.draw_pts = None
        if STt.grab is not None:
            STt.grab = None

    def on_rmb(e):
        (s, j), d = find_node(e, 30.0)
        if s >= 0:
            SEL[0] = s
            wx, wy = world(e)
            sgn = 1.0 if wy > BASES[s] + STt.snap[s, j - 1] else -1.0
            pluck_string(s, j, sgn)

    cv.bind("<Button-1>", on_lmb)
    cv.bind("<B1-Motion>", on_move)
    cv.bind("<ButtonRelease-1>", on_release)
    cv.bind("<Button-3>", on_rmb)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<s>", lambda e: add_default_string())
    root.bind("<t>", lambda e: do_test())
    root.bind("<c>", lambda e: do_clear())
    root.bind("<r>", lambda e: do_reset())
    root.bind("<b>", lambda e: toggle_body())
    root.protocol("WM_DELETE_WINDOW", root.destroy)

    if NSTR[0] == 0:
        add_string(0.15, 0.85, 0.25)
    if not NUMBA_OK:
        log("ВНИМАНИЕ: numba не найден — ядро на чистом Python, будет "
            "CPU >100% и рывки. Установи: python -m pip install numba", True)
    log("Готово. Рисуй струны ЛКМ (drag с пустого места), щипай ПКМ, "
        "смычок — ПРОБЕЛ. Натяжение 0 — струна честно провисает, без взрыва.")
    try:
        dev = sd.query_devices(None, 'output')
        log(f"аудио-выход: {dev['name']}")
    except Exception as e:
        log(f"аудио-выход не найден: {e}", True)
    stream = None
    try:
        stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                                 callback=audio_cb, latency="low")
        stream.start()
    except Exception as e1:
        try:
            stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK,
                                     channels=1, callback=audio_cb,
                                     latency="high")
            stream.start()
            log("аудио: fallback high latency")
        except Exception as e2:
            log(f"АУДИО НЕ ОТКРЫЛОСЬ: {e2}", True)
    draw(); tick(); root.mainloop()
    if stream:
        stream.stop(); stream.close()

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    if NUMBA_OK:
        print("numba: OK (JIT-ядро, GIL отпускается)")
    else:
        print("ВНИМАНИЕ: numba НЕ найден — ядро на чистом Python!\n"
              "Будет CPU >100% и рывки GUI. Установи: "
              "python -m pip install numba")
    if "diag" in args:
        import sounddevice as sd
        print(sd.query_devices())
        print("default:", sd.default.device)
        return
    print("компиляция ядра (один раз, до ~40 с; дальше — из кэша)...")
    t0 = time.time()
    init_engine()
    reset_all()
    print(f"ядро готово за {time.time() - t0:.1f} с")
    # самопроверка производительности: один блок 512 сэмплов = 11.6 мс
    add_string(0.15, 0.85, 0.25)
    t0 = time.time()
    for _ in range(5):
        process_block(512)
    dt = (time.time() - t0) / 5.0
    reset_all()
    print(f"производительность: {dt * 1000:.1f} мс на блок 512 "
          f"(норма < 11.6 мс; если сильно больше — нет numba)")
    if "render" in args:
        secs = 6.0
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            "bow_render.wav")
        for i, a in enumerate(args):
            if a == "render" and i + 1 < len(args):
                try:
                    secs = float(args[i + 1])
                except ValueError:
                    pass
        render_wav(path, secs)
        return
    run_gui()

if __name__ == "__main__":
    try:
        main()
    except Exception:
        import traceback
        traceback.print_exc()
    finally:
        try:
            input("\nEnter — закрыть...")
        except Exception:
            pass
