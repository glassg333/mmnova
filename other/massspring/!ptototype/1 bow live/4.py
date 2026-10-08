#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v10)
#  * N=192: потолок спектра струны ~8 кГц (было 4 — «срезанный верх»)
#  * сила смычка ∝ √(0.08/β)·Z0·vb: прижим/скорость/R/β сильно влияют
#  * кривая трения острее (узкий Stribeck) — режимы различимы
#  * шум волоса = модуляция прижима + прямой соскребающий force
#  * честная формула предела натяжения; остальное как в v9
# Зависимости: python -m pip install numpy sounddevice numba
# Режимы: python bow_live.py | diag | render
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np
from numba import njit

SR, BLOCK = 44100, 512
N, L, MU = 192, 1.0, 0.04
INHARM = 2.0e-5
C0, C1 = N // 3, 2 * N // 3
KC, CC = 8.0e5, 0.5
FCAP_ABS = 150.0
FR = 0.55
KG, CG, GCAP = 150.0, 1.2, 0.02
F0A, F0B = 82.41, 82.41 * 1.4983
DX = L / (N + 1); M0 = MU * DX
KA1 = MU * (2 * L * F0A) ** 2 / DX
KB1 = MU * (2 * L * F0B) ** 2 / DX
K4A = INHARM * KA1 * (L / DX) ** 2
K4B = INHARM * KB1 * (L / DX) ** 2
Z0 = MU * 2 * L * F0A
BRA = 1e-4 * math.sqrt(KA1 * M0)
BRB = 1e-4 * math.sqrt(KB1 * M0)
J1, J2, J3 = int(0.28 * N), int(0.66 * N), N + 2 + int(0.5 * N)#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v11)
#  * ОТРЫВ под контролем: режимы СБРОС/СТЕНКИ/СВОБОДА + предел,
#    ручные СБРОС БЕЛОЙ/СБРОС СИНЕЙ
#  * ПИНЫ между струнами (Shift+клик — поставить, Ctrl+клик — убрать):
#    пин = МАССА-посредник: белая бьёт -> пин -> синяя (ингармоника)
#  * раздельные настройки струн (натяжение/нелин/затухание)
# Зависимости: python -m pip install numpy sounddevice numba
# Режимы: python bow_live.py | diag | render
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np
from numba import njit

SR, BLOCK = 44100, 512
N, L, MU = 192, 1.0, 0.04
INHARM = 2.0e-5
C0, C1 = N // 3, 2 * N // 3
KC, CC = 8.0e5, 0.5
FCAP_ABS = 150.0
FR = 0.55
KG, CG, GCAP = 150.0, 1.2, 0.02
F0A, F0B = 82.41, 82.41 * 1.4983
DX = L / (N + 1); M0 = MU * DX
KA1 = MU * (2 * L * F0A) ** 2 / DX
KB1 = MU * (2 * L * F0B) ** 2 / DX
K4A = INHARM * KA1 * (L / DX) ** 2
K4B = INHARM * KB1 * (L / DX) ** 2
Z0 = MU * 2 * L * F0A
BRA = 1e-4 * math.sqrt(KA1 * M0)
BRB = 1e-4 * math.sqrt(KB1 * M0)
J1, J2, J3 = int(0.28 * N), int(0.66 * N), N + 2 + int(0.5 * N)
HM_M = 25.0 * M0; HM_K = 4.0e5; HM_C = 0.8; HM_V = 1.3
MAXP = 8

BODYF = np.array([95.0, 160.0, 220.0, 380.0, 720.0, 1150.0])
BODYQ = np.array([6.0, 8.0, 9.0, 11.0, 13.0, 15.0])
BODYG = np.array([0.45, 0.38, 0.34, 0.30, 0.22, 0.16])

def body_coeffs(sr):
    nm = len(BODYF)
    r2 = np.empty(nm); c1 = np.empty(nm); b0 = np.empty(nm)
    for m in range(nm):
        w = 2 * math.pi * BODYF[m] / sr
        r = math.exp(-w / (2 * BODYQ[m]))
        r2[m] = r * r; c1[m] = 2 * r * math.cos(w); b0[m] = 0.5 * (1 - r * r)
    return r2, c1, b0

BR2_, BC1_, BB0_ = body_coeffs(SR)
BS1 = np.zeros(6); BS2 = np.zeros(6)
STATS = np.zeros(2)
DCS = np.zeros(4)
DCS[2] = math.exp(-2.0 * math.pi * 25.0 / SR)
HST = np.zeros(6)
PINX = np.zeros(MAXP)      # дробный индекс 0..N-1
PINSA = np.zeros(MAXP)     # y-центр в плоскости белой (м)
PINSB = np.zeros(MAXP)     # y-центр в плоскости синей (м)
PINR = np.zeros(MAXP)      # радиус (м)
PINY = np.zeros(MAXP)      # смещение пина (динамика)
PINV = np.zeros(MAXP)

@njit(cache=True, fastmath=True)
def core(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         kA, kB, k4A, k4B, brA, brB, camA, camB, im, z0e,
         N, C0, C1, gap, kc, cc, fcap, fr,
         speed, press, nzd, jb, betap, coll,
         gc, gt, kg, cg, gcap, fnlA, fnlB, fnlc, aLP, j1, j2, j3,
         gB, body_on, bc1, br2, bb0, bgn, bs1, bs2, dcs, stats, hst,
         hm_k, hm_c, imh, hm_v,
         npins, pinx, pinsa, pinsb, pinr, piny, pinv, impin, pdamp,
         wallmode, yl):
    dcx = dcs[0]; dcy = dcs[1]; dca = dcs[2]; nzlp = dcs[3]
    bsq = math.sqrt(0.08 / max(betap, 0.02))
    for i in range(frames):
        R = Rarr[i]; on = onarr[i]
        vb0 = (0.06 + 0.34 * R) * speed
        fn0 = press * 1.2 * bsq * z0e * vb0
        vf = 0.004 + 0.030 * R
        for o in range(ovs):
            for j in range(1, N + 1):
                curv = yE[j - 1] - 2.0 * yE[j] + yE[j + 1]
                f = (kA * curv
                     + brA * (vE[j - 1] - 2.0 * vE[j] + vE[j + 1])
                     - camA * vE[j])
                if fnlA > 0.0 and 1 < j < N:
                    mlt = 1.0 + fnlA * ((yE[j] - yE[j - 1]) ** 2
                                        + (yE[j + 1] - yE[j]) ** 2)
                    if mlt > fnlc: mlt = fnlc
                    f += kA * curv * (mlt - 1.0)
                if k4A > 0.0 and 2 < j < N:
                    f -= k4A * (yE[j - 2] - 4.0 * yE[j - 1] + 6.0 * yE[j]
                                - 4.0 * yE[j + 1] + yE[j + 2])
                vE[j] += f * im
            for j in range(N + 2, 2 * N + 2):
                curv = yE[j - 1] - 2.0 * yE[j] + yE[j + 1]
                f = (kB * curv
                     + brB * (vE[j - 1] - 2.0 * vE[j] + vE[j + 1])
                     - camB * vE[j])
                if fnlB > 0.0 and N + 2 < j < 2 * N + 1:
                    mlt = 1.0 + fnlB * ((yE[j] - yE[j - 1]) ** 2
                                        + (yE[j + 1] - yE[j]) ** 2)
                    if mlt > fnlc: mlt = fnlc
                    f += kB * curv * (mlt - 1.0)
                if k4B > 0.0 and N + 4 <= j <= 2 * N:
                    f -= k4B * (yE[j - 2] - 4.0 * yE[j - 1] + 6.0 * yE[j]
                                - 4.0 * yE[j + 1] + yE[j + 2])
                vE[j] += f * im - gB * dt
            if coll:
                for j in range(C0, C1):
                    dl = -(gap + yE[N + 2 + j] - yE[1 + j])
                    if dl > 0.0:
                        dmp = 1.0 + cc * (vE[1 + j] - vE[N + 2 + j])
                        if dmp < 0.0: dmp = 0.0
                        elif dmp > 2.0: dmp = 2.0
                        fc = kc * dl ** 1.5 * dmp
                        if fc > fcap: fc = fcap
                        vE[1 + j] -= fc * im
                        vE[N + 2 + j] += fc * im
                        stats[1] += 1.0
            for p in range(npins):                        # ПИНЫ (массы)
                fx = pinx[p]; r = pinr[p]
                j0 = int(fx)
                if j0 > N - 2: j0 = N - 2
                if j0 < 0: j0 = 0
                w1 = fx - j0; w0 = 1.0 - w1
                ycp = piny[p]
                for base in (1, N + 2):                   # A и B
                    ycA = (pinsa[p] if base == 1 else pinsb[p]) + ycp
                    for jjw in ((base + j0, w0), (base + j0 + 1, w1)):
                        jj = jjw[0]; w = jjw[1]
                        if w <= 0.0: continue
                        dy = yE[jj] - ycA
                        ady = dy if dy >= 0.0 else -dy
                        if ady < r:
                            dl = r - ady
                            s = 1.0 if dy >= 0.0 else -1.0
                            closing = -s * (vE[jj] - pinv[p])
                            dmp = 1.0 + cc * closing
                            if dmp < 0.0: dmp = 0.0
                            elif dmp > 2.0: dmp = 2.0
                            F = kc * dl ** 1.5 * dmp
                            if F > fcap: F = fcap
                            vE[jj] += s * F * im * w
                            pinv[p] -= s * F * impin * w
                pinv[p] *= (1.0 - pdamp)
                piny[p] += pinv[p] * dt
                if wallmode == 1:
                    if piny[p] > yl:
                        piny[p] = yl
                        if pinv[p] > 0.0: pinv[p] = -0.5 * pinv[p]
                    elif piny[p] < -yl:
                        piny[p] = -yl
                        if pinv[p] < 0.0: pinv[p] = -0.5 * pinv[p]
                if pinv[p] > 50.0 or pinv[p] < -50.0 or piny[p] > 1.0 \
                        or piny[p] < -1.0:
                    piny[p] = 0.0; pinv[p] = 0.0
            if gc >= 0:
                if gc < N + 2:
                    lo = gc - 4
                    if lo < 1: lo = 1
                    hi = gc + 4
                    if hi > N: hi = N
                else:
                    lo = gc - 4
                    if lo < N + 2: lo = N + 2
                    hi = gc + 4
                    if hi > 2 * N + 1: hi = 2 * N + 1
                for jj in range(lo, hi + 1):
                    w = 1.0 - abs(jj - gc) / 5.0
                    if w > 0.0:
                        f = w * (kg * (gt - yE[jj]) - cg * vE[jj])
                        dv = f * im
                        if dv > gcap: dv = gcap
                        elif dv < -gcap: dv = -gcap
                        vE[jj] += dv
            if hst[2] > 0.5:
                jh = int(hst[3]); s = hst[4]
                xh = hst[0]; vh = hst[1]
                pen = -s * (xh - yE[jh])
                if pen > 0.0:
                    dmp = 1.0 + hm_c * (-s * (vh - vE[jh]))
                    if dmp < 0.0: dmp = 0.0
                    elif dmp > 2.0: dmp = 2.0
                    F = hm_k * pen ** 1.5 * dmp
                    if F > fcap: F = fcap
                    vE[jh] -= s * F * im
                    vh += s * F * imh
                elif s * vh > 0.0:
                    hst[2] = 0.0
                xh += vh * dt
                hst[0] = xh; hst[1] = vh
                hst[5] -= dt
                if hst[5] <= 0.0: hst[2] = 0.0
            if on > 0.0 and fn0 > 0.0:
                nzl = nzlp + aLP * (noise[i * ovs + o] - nzlp)
                nzlp = nzl
                fn = fn0 * (1.0 + 0.5 * nzd * nzl) * on
                if fn < 0.0: fn = 0.0
                vE[jb] += nzd * nzl * fn * 0.7 * im * on
                vb_t = vb0 * on
                vc = vE[jb]
                lo = vc - fn * im; hi = vc + fn * im
                for _ in range(48):
                    mid = 0.5 * (lo + hi)
                    u = vb_t - mid
                    mu = fr + (1.0 - fr) * math.exp(-abs(u) / vf)
                    if mid - vc - im * fn * mu < 0.0: lo = mid
                    else: hi = mid
                vn = 0.5 * (lo + hi)
                vE[jb] = vn
                if abs(vb_t - vn) < 0.004: stats[0] += 1.0
            for j in range(1, N + 1):
                yE[j] += vE[j] * dt
            for j in range(N + 2, 2 * N + 2):
                yE[j] += vE[j] * dt
            if wallmode == 1:                             # СТЕНКИ отрыва
                for j in range(1, N + 1):
                    if yE[j] > yl:
                        yE[j] = yl
                        if vE[j] > 0.0: vE[j] = -0.5 * vE[j]
                    elif yE[j] < -yl:
                        yE[j] = -yl
                        if vE[j] < 0.0: vE[j] = -0.5 * vE[j]
                for j in range(N + 2, 2 * N + 2):
                    if yE[j] > yl:
                        yE[j] = yl
                        if vE[j] > 0.0: vE[j] = -0.5 * vE[j]
                    elif yE[j] < -yl:
                        yE[j] = -yl
                        if vE[j] < 0.0: vE[j] = -0.5 * vE[j]
        x = 2.5 * vE[j1] + 1.2 * vE[j2] + 0.8 * vE[j3]
        if body_on > 0.5:
            wet = 0.0
            for q in range(6):
                yb = bb0[q] * x + bc1[q] * bs1[q] - br2[q] * bs2[q]
                bs2[q] = bs1[q]; bs1[q] = yb
                wet += bgn[q] * yb
            x = 0.8 * x + 0.5 * wet
        xs = x - dcx + dca * dcy
        dcx = x; dcy = xs
        out[i] = xs
    dcs[0] = dcx; dcs[1] = dcy; dcs[3] = nzlp

P = dict(R=0.0, speed=1.0, press=1.0, tA=0.8, tB=0.8, nlA=0.04, nlB=0.04,
         caA=0.02, caB=0.02, grav=0.0, gap=2.0, pinr=3.0, pinm=15.0,
         brk=0.25, noise=0.25, beta=0.08, vol=0.35,
         bow=True, coll=True, automorph=True, body=True, protect=False)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}
WALL_NAMES = ("СБРОС", "СТЕНКИ", "СВОБОДА")

vE = np.zeros(2 * N + 3); yE = np.zeros(2 * N + 3)
ST = type("ST", (), {})()
ST.ovs = 2; ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0
ST.snap = np.zeros(2 * N + 1)
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0; ST.stick = 0.0; ST.cont = 0.0
ST.rms_ema = 0.0; ST.gain = 0.0; ST.tclamp = False
ST.kick = False; ST.toss = False; ST.pluck = None; ST.grab = None
ST.rec = False; ST.pending_ovs = None; ST.recl = []
ST.wallmode = 0; ST.npins = 0; ST.rA = False; ST.rB = False
RNG = np.random.default_rng(7)

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

def R_auto(ph):
    if ph < 1.2: return 0.0
    if ph < 2.7: return smoothstep((ph - 1.2) / 1.5)
    if ph < 4.2: return 1.0
    if ph < 5.7: return 1.0 - smoothstep((ph - 4.2) / 1.5)
    return 0.0

TT = np.linspace(0.0, 7.0, 701)
RT = np.array([R_auto(t) for t in TT])

def tension_limit(ovs):
    k4e = K4A if ovs >= 2 else 0.0
    dt = 1.0 / (SR * ovs)
    return max(((1.8 / dt) ** 2 * M0 - 16.0 * k4e) / (4.0 * KA1), 0.0)

def reset_string(which):
    if which in (0, 2):
        vE[1:N + 1] = 0.0; yE[1:N + 1] = 0.0
    if which in (1, 2):
        vE[N + 2:2 * N + 2] = 0.0; yE[N + 2:2 * N + 2] = 0.0

def process_block(frames):
    if ST.pending_ovs is not None:
        ST.ovs = ST.pending_ovs; ST.pending_ovs = None
    ovs = ST.ovs; dt = 1.0 / (SR * ovs); im = dt / M0
    fnlc = 1.0 + 2.0 * max(P["nlA"], P["nlB"])
    k4e = K4A if ovs >= 2 else 0.0
    tmax = tension_limit(ovs)
    tA = min(P["tA"], tmax); tB = min(P["tB"], tmax)
    ST.tclamp = (P["tA"] > tmax + 1e-9) or (P["tB"] > tmax + 1e-9)
    kAe = KA1 * tA; kBe = KB1 * tB
    z0e = Z0 * math.sqrt(max(tA, 1e-6))
    camA = P["caA"] * M0; camB = P["caB"] * M0
    aLP = 1.0 - math.exp(-2.0 * math.pi * 2500.0 * dt)
    fcap = min(FCAP_ABS, 1.2 * M0 / dt)
    gapr = P["gap"] * 1e-3
    gB = P["grav"] * 1.0
    impin = dt / (P["pinm"] * M0)
    phase = (ST.t + np.arange(frames) / SR) % 7.0
    Rarr = np.interp(phase, TT, RT) if P["automorph"] else np.full(frames, P["R"])
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    onarr = np.clip(ST.on + stp * np.arange(1, frames + 1), 0.0, 1.0)
    ST.on = float(onarr[-1]); ST.t += frames / SR
    noise = RNG.standard_normal(frames * ovs)
    if ST.kick:
        zz = np.sin(np.pi * np.linspace(0.0, 1.0, C1 - C0)) ** 2
        vE[1 + C0:1 + C1] += 1.2 * zz
        vE[N + 2 + C0:N + 2 + C1] -= 0.9 * zz
        ST.kick = False
    if ST.toss:
        zz = np.sin(np.pi * np.linspace(0.0, 1.0, C1 - C0)) ** 2
        vE[N + 2 + C0:N + 2 + C1] -= 0.6 * zz
        ST.toss = False
    if ST.pluck is not None:
        j, sgn = ST.pluck; ST.pluck = None
        HST[0] = yE[j] + sgn * 0.006
        HST[1] = -sgn * HM_V
        HST[2] = 1.0; HST[3] = float(j); HST[4] = float(sgn); HST[5] = 3.0
    if ST.rA: reset_string(0); ST.rA = False
    if ST.rB: reset_string(1); ST.rB = False
    # авто-сброс по пределу отрыва (режим СБРОС)
    if ST.wallmode == 0 and P["brk"] > 0.0:
        mA = np.abs(yE[1:N + 1]).max(); mB = np.abs(yE[N + 2:2 * N + 2]).max()
        if mA > P["brk"]:
            reset_string(0); print(f"ОТРЫВ белой (|y|={mA:.3f}) -> СБРОС")
        if mB > P["brk"]:
            reset_string(1); print(f"ОТРЫВ синей (|y|={mB:.3f}) -> СБРОС")
    gc, gt = -1, 0.0
    if ST.grab is not None:
        i0, tg = ST.grab
        gc = i0 + 1 if i0 < N else i0 + 2
        gt = max(-0.03, min(0.03, tg))
    jb = int(P["beta"] * N) + 1
    out = np.empty(frames); STATS[0] = 0.0; STATS[1] = 0.0
    t0 = time.perf_counter()
    core(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         kAe, kBe, k4e, k4e * KB1 / KA1, BRA, BRB, camA, camB, im, z0e,
         N, C0, C1, gapr, KC, CC, fcap, FR,
         P["speed"], P["press"], P["noise"], jb, P["beta"], P["coll"],
         gc, gt, KG, CG, GCAP, P["nlA"], P["nlB"], fnlc, aLP, J1, J2, J3,
         gB, 1.0 if P["body"] else 0.0, BC1_, BR2_, BB0_, BODYG, BS1, BS2,
         DCS, STATS, HST, HM_K, HM_C, dt / HM_M, HM_V,
         ST.npins, PINX, PINSA, PINSB, PINR, PINY, PINV, impin, 0.0005,
         ST.wallmode, P["brk"])
    ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0) / (frames / SR)
    if (not np.isfinite(vE).all()) or (not np.isfinite(yE).all()) \
            or np.abs(yE).max() > 1.0 or np.abs(vE).max() > 80.0:
        vE[:] = 0.0; yE[:] = 0.0; BS1[:] = 0.0; BS2[:] = 0.0
        DCS[0] = DCS[1] = 0.0; HST[2] = 0.0
        PINY[:] = 0.0; PINV[:] = 0.0
        out[:] = 0.0
        print("КРАШ-ГАРД -> полный сброс")
    ST.snap[:] = yE[1:2 * N + 2]
    ST.stick = 0.9 * ST.stick + 0.1 * STATS[0] / max(frames * ovs, 1)
    ST.cont = 0.8 * ST.cont + 0.2 * min(STATS[1] / max(frames * ovs * 0.1, 1), 1.0)
    ST.R_now = float(Rarr[-1])
    if P["protect"]:
        rms = float(np.sqrt(np.mean(out * out))) + 1e-12
        ST.rms_ema = 0.9 * ST.rms_ema + 0.1 * rms
        g_t = min(0.10 / max(ST.rms_ema, 1e-9), 3.0)
        if ST.gain == 0.0: ST.gain = g_t
        else:
            slew = 0.03 if g_t < ST.gain else 0.006
            ST.gain += max(min(g_t - ST.gain, slew), -slew)
        y = np.tanh(out * (ST.gain * P["vol"] * 3.0))
    else:
        y = np.clip(out * (1.2 * P["vol"]), -0.98, 0.98)
    ST.level = 0.9 * ST.level + 0.1 * float(np.mean(np.abs(y)))
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status: ST.underr += 1
        outdata[:, 0] = process_block(frames)
        if ST.rec: ST.recl.append(outdata[:, 0].copy())
    except Exception:
        import traceback; traceback.print_exc()
        outdata[:] = 0

def render_wav(path, seconds=14.0):
    total = int(SR * seconds); pos = 0
    out = np.empty(total)
    print(f"рендер {seconds:.0f} c...")
    while pos < total:
        n = min(BLOCK, total - pos)
        out[pos:pos + n] = process_block(n)
        pos += n
        if (pos // BLOCK) % (SR // BLOCK * 2) == 0:
            print(f"  {pos / SR:4.0f}/{seconds:.0f} c")
    x = out.copy(); nf = SR // 5
    x[-nf:] *= np.linspace(1.0, 0.0, nf)
    p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
    x = np.clip(x * 0.85 / p, -1, 1)
    with wave.open(path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((x * 32767).astype("<i2").tobytes())
    print("ГОТОВО:", path)

def run_gui():
    import tkinter as tk
    root = tk.Tk(); root.title("physmod live v11")
    W = 980
    cv = tk.Canvas(root, width=W, height=300, bg="#101018", highlightthickness=0)
    cv.pack(fill="x")
    XM0, XM1 = 40, W - 40
    ZOOM, B_OFF = 8000.0, 70
    B_BASE, A_BASE = 110, 180
    def draw():
        try:
            cv.delete("all")
            sn = ST.snap
            flash = int(20 + 60 * ST.cont)
            cv.create_rectangle(XM0 + C0 * (XM1 - XM0) // N, 20,
                                XM0 + C1 * (XM1 - XM0) // N, 290,
                                fill="#%02x%02x%02x" % (flash, 26, 40), width=0)
            cv.create_text(XM0, 24, text="Shift+клик — пин, Ctrl+клик — убрать пин",
                           fill="#666680", anchor="w", font=("TkDefaultFont", 8))
            for off, base, col in ((0, A_BASE, "#e8e8f0"),
                                   (N + 1, B_BASE, "#7f7fd0")):
                pts = []
                for j in range(N):
                    pts += [XM0 + j * (XM1 - XM0) / (N - 1),
                            base - float(sn[off + j]) * ZOOM]
                cv.create_line(*pts, fill=col, width=2)
            for p in range(ST.npins):
                xpi = XM0 + PINX[p] * (XM1 - XM0) / (N - 1)
                ypi = PINSA[p] * ZOOM  # хранится как (base-y)/ZOOM для A
                # рисуем в экранных координатах: A_BASE - ycA*ZOOM
                yscr = A_BASE - float(PINSA[p]) * ZOOM - float(PINY[p]) * ZOOM
                rpx = max(3, int(float(PINR[p]) * ZOOM))
                cv.create_oval(xpi - rpx, yscr - rpx, xpi + rpx, yscr + rpx,
                               fill="#40b0ff", width=0)
            jb = int(P["beta"] * N)
            xj = XM0 + jb * (XM1 - XM0) / (N - 1)
            yj = A_BASE - float(sn[jb]) * ZOOM
            cv.create_oval(xj - 5, yj - 5, xj + 5, yj + 5, fill="#ff5040", width=0)
            lv = min(1.0, ST.level * 4)
            cv.create_rectangle(20, 280, 20 + int((W - 40) * lv), 292,
                                fill="#40c080", width=0)
        except Exception as e:
            print("draw:", e)
        root.after(40, draw)
    panel = tk.Frame(root); panel.pack(fill="x")
    cols = [[("R скрежет<->бас", "R", 0.0, 1.0),
             ("Скорость смычка", "speed", 0.0, 1.6),
             ("Прижим", "press", 0.0, 2.0),
             ("Позиция смычка", "beta", 0.02, 0.30),
             ("Шум волоса", "noise", 0.0, 1.0)],
            [("БЕЛАЯ: натяжение", "tA", 0.0, 2.2),
             ("БЕЛАЯ: нелин.", "nlA", 0.0, 0.15),
             ("БЕЛАЯ: затухание", "caA", 0.002, 0.12)],
            [("СИНЯЯ: натяжение", "tB", 0.0, 2.2),
             ("СИНЯЯ: нелин.", "nlB", 0.0, 0.15),
             ("СИНЯЯ: затухание", "caB", 0.002, 0.12),
             ("Гравитация синей", "grav", 0.0, 1.0)],
            [("Зазор струн, мм", "gap", 0.5, 6.0),
             ("Радиус пина, мм", "pinr", 1.0, 8.0),
             ("Масса пина, ×M", "pinm", 2.0, 100.0),
             ("Предел отрыва", "brk", 0.03, 0.50),
             ("Громкость", "vol", 0.0, 1.0)]]
    sv = {}
    for col, items in enumerate(cols):
        for r_, (lab, key, a, b) in enumerate(items):
            tk.Label(panel, text=lab, width=18, anchor="w").grid(
                row=r_, column=col * 2)
            var = tk.DoubleVar(value=P[key]); sv[key] = var
            sc = tk.Scale(panel, from_=a, to=b, resolution=0.005,
                          orient="horizontal", length=200, showvalue=1,
                          variable=var,
                          command=(lambda v, k=key: P.__setitem__(k, float(v))))
            sc.grid(row=r_, column=col * 2 + 1, sticky="w")
            sv[key + "_sc"] = sc
    buts = tk.Frame(root); buts.pack(fill="x")
    buts2 = tk.Frame(root); buts2.pack(fill="x")
    def toggle_bow():
        P["bow"] = not P["bow"]
        b_bow.config(text="СМЫЧОК: ВКЛ" if P["bow"] else "СМЫЧОК: выкл")
    def toggle_auto():
        P["automorph"] = not P["automorph"]
        b_auto.config(text="АВТО-МОРФ: ВКЛ" if P["automorph"] else "АВТО-МОРФ: выкл")
    def toggle_body():
        P["body"] = not P["body"]
        b_body.config(text="БОДИ: ВКЛ" if P["body"] else "БОДИ: выкл")
    def toggle_coll():
        P["coll"] = not P["coll"]
        b_coll.config(text="КОНТАКТ: ВКЛ" if P["coll"] else "КОНТАКТ: выкл")
    def toggle_prot():
        P["protect"] = not P["protect"]
        b_prot.config(text="ЗАЩИТА: АГС" if P["protect"] else "ЗАЩИТА: КЛИП")
    def cycle_wall():
        ST.wallmode = (ST.wallmode + 1) % 3
        b_wall.config(text="ОТРЫВ: " + WALL_NAMES[ST.wallmode])
    b_bow = tk.Button(buts, text="СМЫЧОК: ВКЛ", width=12, command=toggle_bow)
    b_auto = tk.Button(buts, text="АВТО-МОРФ: ВКЛ", width=14, command=toggle_auto)
    b_body = tk.Button(buts, text="БОДИ: ВКЛ", width=10, command=toggle_body)
    b_coll = tk.Button(buts, text="КОНТАКТ: ВКЛ", width=12, command=toggle_coll)
    b_prot = tk.Button(buts, text="ЗАЩИТА: КЛИП", width=12, command=toggle_prot)
    b_wall = tk.Button(buts, text="ОТРЫВ: СБРОС", width=13, command=cycle_wall)
    b_kick = tk.Button(buts2, text="ТОЛЧОК", width=8,
                       command=lambda: setattr(ST, "kick", True))
    b_toss = tk.Button(buts2, text="БРОСОК", width=8,
                       command=lambda: setattr(ST, "toss", True))
    b_rA = tk.Button(buts2, text="СБРОС БЕЛОЙ", width=12,
                     command=lambda: setattr(ST, "rA", True))
    b_rB = tk.Button(buts2, text="СБРОС СИНЕЙ", width=12,
                     command=lambda: setattr(ST, "rB", True))
    def clear_pins():
        ST.npins = 0; PINY[:]=0.0; PINV[:]=0.0
    b_cp = tk.Button(buts2, text="ПИНЫ: ОЧИСТИТЬ", width=14, command=clear_pins)
    def rec_toggle():
        if not ST.rec:
            ST.rec = True; ST.recl = []; b_rec.config(text="ЗАПИСЬ: идёт")
        else:
            ST.rec = False; b_rec.config(text="ЗАПИСЬ")
            if ST.recl:
                x = np.concatenate(ST.recl)
                p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
                x = np.clip(x * 0.9 / p, -1, 1)
                fn = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                  "jam_%s.wav"
                                  % datetime.datetime.now().strftime("%H%M%S"))
                with wave.open(fn, "wb") as w:
                    w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
                    w.writeframes((x * 32767).astype("<i2").tobytes())
                print("записано:", fn)
    b_rec = tk.Button(buts2, text="ЗАПИСЬ", width=9, command=rec_toggle)
    qual = tk.OptionMenu(buts2, tk.StringVar(value="НОРМА"), *OVS_MAP,
                         command=lambda v: setattr(ST, "pending_ovs", OVS_MAP[v]))
    qual.config(width=6)
    b_quit = tk.Button(buts2, text="ВЫХОД", width=7, command=root.destroy)
    for b in (b_bow, b_auto, b_body, b_coll, b_prot, b_wall):
        b.pack(side="left", padx=2, pady=3)
    for b in (b_kick, b_toss, b_rA, b_rB, b_cp, b_rec, qual, b_quit):
        b.pack(side="left", padx=2, pady=3)
    status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")
    def tick():
        try:
            if P["automorph"]: sv["R"].set(ST.R_now)
            tc = "  [натяжение: предел]" if ST.tclamp else ""
            status.config(text=f"CPU {ST.cpu*100:4.0f}%  сбои {ST.underr}  "
                               f"стик {ST.stick*100:3.0f}%  контакт {ST.cont*100:3.0f}%  "
                               f"пины {ST.npins}  R={ST.R_now:.2f}"
                               + ("  ЗАПИСЬ..." if ST.rec else "") + tc)
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def pick(e):
        if e.state & 0x0004:                              # Ctrl — убрать пин
            best, bd = -1, 1e9
            for p in range(ST.npins):
                xp = XM0 + PINX[p] * (XM1 - XM0) / (N - 1)
                d = abs(e.x - xp)
                if d < bd: bd, best = d, p
            if best >= 0 and bd < 20:
                ST.npins -= 1
                for arr in (PINX, PINSA, PINSB, PINR, PINY, PINV):
                    arr[best:-1] = arr[best + 1:]
            return
        if e.state & 0x0001:                              # Shift — пин
            if ST.npins < MAXP:
                fx = (e.x - XM0) * (N - 1) / (XM1 - XM0)
                fx = max(1.0, min(N - 2.0, fx))
                PINX[ST.npins] = fx
                PINSA[ST.npins] = (A_BASE - e.y) / ZOOM
                PINSB[ST.npins] = (B_BASE - e.y) / ZOOM
                PINR[ST.npins] = P["pinr"] * 1e-3
                PINY[ST.npins] = 0.0; PINV[ST.npins] = 0.0
                ST.npins += 1
            return
        j = round((e.x - XM0) * (N - 1) / (XM1 - XM0)); j = max(0, min(N - 1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j]) * ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N + 1 + j]) * ZOOM))
        i = j if dA <= dB else N + j
        base = A_BASE if i < N else B_BASE
        ST.grab = (i, max(-0.03, min(0.03, (base - e.y) / ZOOM)))
    def pluck(e):
        j = round((e.x - XM0) * (N - 1) / (XM1 - XM0)); j = max(0, min(N - 1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j]) * ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N + 1 + j]) * ZOOM))
        if dA <= dB:
            jh = j + 1; base = A_BASE; ystr = float(ST.snap[j])
        else:
            jh = j + N + 2; base = B_BASE; ystr = float(ST.snap[N + 1 + j])
        sgn = 1.0 if e.y < base - ystr * ZOOM else -1.0
        ST.pluck = (jh, sgn)
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", lambda e: (ST.grab and not (e.state & 0x0005))
            and pick(e))
    cv.bind("<ButtonRelease-1>", lambda e: setattr(ST, "grab", None))
    cv.bind("<Button-3>", pluck)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<k>", lambda e: setattr(ST, "kick", True))
    root.bind("<b>", lambda e: setattr(ST, "toss", True))
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    print("""БЕЛАЯ — смычок/молоток/рука. СИНЯЯ — резонатор. Пины: Shift+клик
поставить, Ctrl+клик убрать; пин = МАССА между струнами (энергия течёт через него).
ОТРЫВ: СБРОС — авто-ресет при превышении предела; СТЕНКИ — упругий отскок;
СВОБОДА — до краш-гарда. Ручные кнопки СБРОС БЕЛОЙ/СИНЕЙ.
Смычок требует натяжения белой > ~0.1. Низкое натяжение = резинка/кисель.
Тест пина: пин между струнами, ПКМ удар по белой напротив пина -> синяя звенит через пин.""")
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                             callback=audio_cb, latency="low")
    stream.start()
    draw(); tick(); root.mainloop()
    stream.stop(); stream.close()

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    for q, o in OVS_MAP.items():
        print(f"  {q}: предел натяжения {tension_limit(o):.2f}")
    if "diag" in args:
        print(sd.query_devices()); print("default:", sd.default.device); return
    print("компиляция ядра (один раз, до ~20 c)...")
    t0 = time.time()
    process_block(64); process_block(64)
    vE[:] = 0.0; yE[:] = 0.0; BS1[:] = 0.0; BS2[:] = 0.0
    DCS[0] = DCS[1] = 0.0; HST[:] = 0.0
    PINY[:] = 0.0; PINV[:] = 0.0
    ST.t = 0.0; ST.gain = 0.0
    print(f"ядро готово за {time.time()-t0:.1f} c")
    if "render" in args:
        render_wav(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "bow_render.wav")); return
    run_gui()

try:
    import sounddevice as sd
    if __name__ == "__main__":
        main()
except Exception:
    import traceback; traceback.print_exc()
finally:
    try: input("\nEnter — закрыть...")
    except Exception: pass
HM_M = 25.0 * M0; HM_K = 4.0e5; HM_C = 0.8; HM_V = 1.3

BODYF = np.array([95.0, 160.0, 220.0, 380.0, 720.0, 1150.0])
BODYQ = np.array([6.0, 8.0, 9.0, 11.0, 13.0, 15.0])
BODYG = np.array([0.45, 0.38, 0.34, 0.30, 0.22, 0.16])

def body_coeffs(sr):
    nm = len(BODYF)
    r2 = np.empty(nm); c1 = np.empty(nm); b0 = np.empty(nm)
    for m in range(nm):
        w = 2 * math.pi * BODYF[m] / sr
        r = math.exp(-w / (2 * BODYQ[m]))
        r2[m] = r * r; c1[m] = 2 * r * math.cos(w); b0[m] = 0.5 * (1 - r * r)
    return r2, c1, b0

BR2_, BC1_, BB0_ = body_coeffs(SR)
BS1 = np.zeros(6); BS2 = np.zeros(6)
STATS = np.zeros(2)
DCS = np.zeros(4)
DCS[2] = math.exp(-2.0 * math.pi * 25.0 / SR)
HST = np.zeros(6)

@njit(cache=True, fastmath=True)
def core(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         kA, kB, k4A, k4B, brA, brB, cam, im, z0e,
         N, C0, C1, gap, kc, cc, fcap, fr,
         speed, press, nzd, jb, betap, coll,
         gc, gt, kg, cg, gcap, fnl, fnlclamp, aLP, j1, j2, j3,
         gB, body_on, bc1, br2, bb0, bgn, bs1, bs2, dcs, stats, hst,
         hm_k, hm_c, imh, hm_v):
    dcx = dcs[0]; dcy = dcs[1]; dca = dcs[2]; nzlp = dcs[3]
    bsq = math.sqrt(0.08 / max(betap, 0.02))
    for i in range(frames):
        R = Rarr[i]; on = onarr[i]
        vb0 = (0.06 + 0.34 * R) * speed
        fn0 = press * 1.2 * bsq * z0e * vb0
        vf = 0.004 + 0.030 * R
        for o in range(ovs):
            for j in range(1, N + 1):
                curv = yE[j - 1] - 2.0 * yE[j] + yE[j + 1]
                f = (kA * curv
                     + brA * (vE[j - 1] - 2.0 * vE[j] + vE[j + 1])
                     - cam * vE[j])
                if fnl > 0.0 and 1 < j < N:
                    mlt = 1.0 + fnl * ((yE[j] - yE[j - 1]) ** 2
                                       + (yE[j + 1] - yE[j]) ** 2)
                    if mlt > fnlclamp: mlt = fnlclamp
                    f += kA * curv * (mlt - 1.0)
                if k4A > 0.0 and 2 < j < N:
                    f -= k4A * (yE[j - 2] - 4.0 * yE[j - 1] + 6.0 * yE[j]
                                - 4.0 * yE[j + 1] + yE[j + 2])
                vE[j] += f * im
            for j in range(N + 2, 2 * N + 2):
                curv = yE[j - 1] - 2.0 * yE[j] + yE[j + 1]
                f = (kB * curv
                     + brB * (vE[j - 1] - 2.0 * vE[j] + vE[j + 1])
                     - cam * vE[j])
                if fnl > 0.0 and N + 2 < j < 2 * N + 1:
                    mlt = 1.0 + fnl * ((yE[j] - yE[j - 1]) ** 2
                                       + (yE[j + 1] - yE[j]) ** 2)
                    if mlt > fnlclamp: mlt = fnlclamp
                    f += kB * curv * (mlt - 1.0)
                if k4B > 0.0 and N + 4 <= j <= 2 * N:
                    f -= k4B * (yE[j - 2] - 4.0 * yE[j - 1] + 6.0 * yE[j]
                                - 4.0 * yE[j + 1] + yE[j + 2])
                vE[j] += f * im - gB * dt
            if coll:
                for j in range(C0, C1):
                    dl = -(gap + yE[N + 2 + j] - yE[1 + j])
                    if dl > 0.0:
                        dmp = 1.0 + cc * (vE[1 + j] - vE[N + 2 + j])
                        if dmp < 0.0: dmp = 0.0
                        elif dmp > 2.0: dmp = 2.0
                        fc = kc * dl ** 1.5 * dmp
                        if fc > fcap: fc = fcap
                        vE[1 + j] -= fc * im
                        vE[N + 2 + j] += fc * im
                        stats[1] += 1.0
            if gc >= 0:
                if gc < N + 2:
                    lo = gc - 4
                    if lo < 1: lo = 1
                    hi = gc + 4
                    if hi > N: hi = N
                else:
                    lo = gc - 4
                    if lo < N + 2: lo = N + 2
                    hi = gc + 4
                    if hi > 2 * N + 1: hi = 2 * N + 1
                for jj in range(lo, hi + 1):
                    w = 1.0 - abs(jj - gc) / 5.0
                    if w > 0.0:
                        f = w * (kg * (gt - yE[jj]) - cg * vE[jj])
                        dv = f * im
                        if dv > gcap: dv = gcap
                        elif dv < -gcap: dv = -gcap
                        vE[jj] += dv
            if hst[2] > 0.5:
                jh = int(hst[3]); s = hst[4]
                xh = hst[0]; vh = hst[1]
                pen = -s * (xh - yE[jh])
                if pen > 0.0:
                    dmp = 1.0 + hm_c * (-s * (vh - vE[jh]))
                    if dmp < 0.0: dmp = 0.0
                    elif dmp > 2.0: dmp = 2.0
                    F = hm_k * pen ** 1.5 * dmp
                    if F > fcap: F = fcap
                    vE[jh] -= s * F * im
                    vh += s * F * imh
                elif s * vh > 0.0:
                    hst[2] = 0.0
                xh += vh * dt
                hst[0] = xh; hst[1] = vh
                hst[5] -= dt
                if hst[5] <= 0.0: hst[2] = 0.0
            if on > 0.0 and fn0 > 0.0:
                nzl = nzlp + aLP * (noise[i * ovs + o] - nzlp)
                nzlp = nzl
                fn = fn0 * (1.0 + 0.5 * nzd * nzl) * on
                if fn < 0.0: fn = 0.0
                vE[jb] += nzd * nzl * fn * 0.7 * im * on   # соскребающий шум
                vb_t = vb0 * on
                vc = vE[jb]
                lo = vc - fn * im; hi = vc + fn * im
                for _ in range(48):
                    mid = 0.5 * (lo + hi)
                    u = vb_t - mid
                    mu = fr + (1.0 - fr) * math.exp(-abs(u) / vf)
                    if mid - vc - im * fn * mu < 0.0: lo = mid
                    else: hi = mid
                vn = 0.5 * (lo + hi)
                vE[jb] = vn
                if abs(vb_t - vn) < 0.004: stats[0] += 1.0
            for j in range(1, N + 1):
                yE[j] += vE[j] * dt
            for j in range(N + 2, 2 * N + 2):
                yE[j] += vE[j] * dt
        x = 2.5 * vE[j1] + 1.2 * vE[j2] + 0.8 * vE[j3]
        if body_on > 0.5:
            wet = 0.0
            for q in range(6):
                yb = bb0[q] * x + bc1[q] * bs1[q] - br2[q] * bs2[q]
                bs2[q] = bs1[q]; bs1[q] = yb
                wet += bgn[q] * yb
            x = 0.8 * x + 0.5 * wet
        xs = x - dcx + dca * dcy
        dcx = x; dcy = xs
        out[i] = xs
    dcs[0] = dcx; dcs[1] = dcy; dcs[3] = nzlp

P = dict(R=0.0, speed=1.0, press=1.0, tension=0.8, nl=0.04, grav=0.0,
         gap=2.0, noise=0.25, beta=0.08, ca=0.02, vol=0.35,
         bow=True, coll=True, automorph=True, body=True, protect=False)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}

vE = np.zeros(2 * N + 3); yE = np.zeros(2 * N + 3)
ST = type("ST", (), {})()
ST.ovs = 2; ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0
ST.snap = np.zeros(2 * N + 1)
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0; ST.stick = 0.0; ST.cont = 0.0
ST.rms_ema = 0.0; ST.gain = 0.0; ST.tclamp = False
ST.kick = False; ST.toss = False; ST.pluck = None; ST.grab = None
ST.rec = False; ST.pending_ovs = None; ST.recl = []
RNG = np.random.default_rng(7)

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

def R_auto(ph):
    if ph < 1.2: return 0.0
    if ph < 2.7: return smoothstep((ph - 1.2) / 1.5)
    if ph < 4.2: return 1.0
    if ph < 5.7: return 1.0 - smoothstep((ph - 4.2) / 1.5)
    return 0.0

TT = np.linspace(0.0, 7.0, 701)
RT = np.array([R_auto(t) for t in TT])

def tension_limit(ovs):
    k4e = K4A if ovs >= 2 else 0.0
    dt = 1.0 / (SR * ovs)
    tmax = ((1.8 / dt) ** 2 * M0 - 16.0 * k4e) / (4.0 * KA1)
    return max(tmax, 0.0)

def process_block(frames):
    if ST.pending_ovs is not None:
        ST.ovs = ST.pending_ovs; ST.pending_ovs = None
    ovs = ST.ovs; dt = 1.0 / (SR * ovs); im = dt / M0
    pn = P["nl"]; fnlclamp = 1.0 + 2.0 * pn
    k4e = K4A if ovs >= 2 else 0.0
    t = P["tension"]
    tmax = tension_limit(ovs)
    teff = min(t, tmax) if t > 0.0 else 0.0
    ST.tclamp = (t > 0.0) and (teff < t - 1e-6)
    kAe = KA1 * teff; kBe = KB1 * teff
    z0e = Z0 * math.sqrt(max(teff, 1e-6))
    cam = P["ca"] * M0
    aLP = 1.0 - math.exp(-2.0 * math.pi * 2500.0 * dt)
    fcap = min(FCAP_ABS, 1.2 * M0 / dt)
    gapr = P["gap"] * 1e-3
    gB = P["grav"] * 1.0
    phase = (ST.t + np.arange(frames) / SR) % 7.0
    Rarr = np.interp(phase, TT, RT) if P["automorph"] else np.full(frames, P["R"])
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    onarr = np.clip(ST.on + stp * np.arange(1, frames + 1), 0.0, 1.0)
    ST.on = float(onarr[-1]); ST.t += frames / SR
    noise = RNG.standard_normal(frames * ovs)
    if ST.kick:
        zz = np.sin(np.pi * np.linspace(0.0, 1.0, C1 - C0)) ** 2
        vE[1 + C0:1 + C1] += 1.2 * zz
        vE[N + 2 + C0:N + 2 + C1] -= 0.9 * zz
        ST.kick = False
    if ST.toss:
        zz = np.sin(np.pi * np.linspace(0.0, 1.0, C1 - C0)) ** 2
        vE[N + 2 + C0:N + 2 + C1] -= 0.6 * zz
        ST.toss = False
    if ST.pluck is not None:
        j, sgn = ST.pluck; ST.pluck = None
        HST[0] = yE[j] + sgn * 0.006
        HST[1] = -sgn * HM_V
        HST[2] = 1.0; HST[3] = float(j); HST[4] = float(sgn); HST[5] = 3.0
    gc, gt = -1, 0.0
    if ST.grab is not None:
        i0, tg = ST.grab
        gc = i0 + 1 if i0 < N else i0 + 2
        gt = max(-0.03, min(0.03, tg))
    jb = int(P["beta"] * N) + 1
    out = np.empty(frames); STATS[0] = 0.0; STATS[1] = 0.0
    t0 = time.perf_counter()
    core(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         kAe, kBe, k4e, k4e * KB1 / KA1, BRA, BRB, cam, im, z0e,
         N, C0, C1, gapr, KC, CC, fcap, FR,
         P["speed"], P["press"], P["noise"], jb, P["beta"], P["coll"],
         gc, gt, KG, CG, GCAP, pn, fnlclamp, aLP, J1, J2, J3,
         gB, 1.0 if P["body"] else 0.0, BC1_, BR2_, BB0_, BODYG, BS1, BS2,
         DCS, STATS, HST, HM_K, HM_C, dt / HM_M, HM_V)
    ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0) / (frames / SR)
    if (not np.isfinite(vE).all()) or (not np.isfinite(yE).all()) \
            or np.abs(yE).max() > 0.25 or np.abs(vE).max() > 80.0:
        vE[:] = 0.0; yE[:] = 0.0; BS1[:] = 0.0; BS2[:] = 0.0
        DCS[0] = DCS[1] = 0.0; HST[2] = 0.0; out[:] = 0.0
        print("ПЕРЕГРУЗ -> сброс состояния")
    ST.snap[:] = yE[1:2 * N + 2]
    ST.stick = 0.9 * ST.stick + 0.1 * STATS[0] / max(frames * ovs, 1)
    ST.cont = 0.8 * ST.cont + 0.2 * min(STATS[1] / max(frames * ovs * 0.1, 1), 1.0)
    ST.R_now = float(Rarr[-1])
    if P["protect"]:
        rms = float(np.sqrt(np.mean(out * out))) + 1e-12
        ST.rms_ema = 0.9 * ST.rms_ema + 0.1 * rms
        g_t = min(0.10 / max(ST.rms_ema, 1e-9), 3.0)
        if ST.gain == 0.0: ST.gain = g_t
        else:
            slew = 0.03 if g_t < ST.gain else 0.006
            ST.gain += max(min(g_t - ST.gain, slew), -slew)
        y = np.tanh(out * (ST.gain * P["vol"] * 3.0))
    else:
        y = np.clip(out * (1.2 * P["vol"]), -0.98, 0.98)
    ST.level = 0.9 * ST.level + 0.1 * float(np.mean(np.abs(y)))
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status: ST.underr += 1
        outdata[:, 0] = process_block(frames)
        if ST.rec: ST.recl.append(outdata[:, 0].copy())
    except Exception:
        import traceback; traceback.print_exc()
        outdata[:] = 0

def render_wav(path, seconds=14.0):
    total = int(SR * seconds); pos = 0
    out = np.empty(total)
    print(f"рендер {seconds:.0f} c...")
    while pos < total:
        n = min(BLOCK, total - pos)
        out[pos:pos + n] = process_block(n)
        pos += n
        if (pos // BLOCK) % (SR // BLOCK * 2) == 0:
            print(f"  {pos / SR:4.0f}/{seconds:.0f} c")
    x = out.copy(); nf = SR // 5
    x[-nf:] *= np.linspace(1.0, 0.0, nf)
    p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
    x = np.clip(x * 0.85 / p, -1, 1)
    with wave.open(path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((x * 32767).astype("<i2").tobytes())
    print("ГОТОВО:", path)

def run_gui():
    import tkinter as tk
    root = tk.Tk(); root.title("physmod live v10")
    W = 940
    cv = tk.Canvas(root, width=W, height=300, bg="#101018", highlightthickness=0)
    cv.pack(fill="x")
    XM0, XM1 = 40, W - 40
    ZOOM, B_OFF = 8000.0, 70
    B_BASE, A_BASE = 110, 180
    def draw():
        try:
            cv.delete("all")
            sn = ST.snap
            flash = int(20 + 60 * ST.cont)
            cv.create_rectangle(XM0 + C0 * (XM1 - XM0) // N, 20,
                                XM0 + C1 * (XM1 - XM0) // N, 290,
                                fill="#%02x%02x%02x" % (flash, 26, 40), width=0)
            cv.create_text(XM0, A_BASE - 34, text="БЕЛАЯ — смычок/молоток/рука",
                           fill="#8888a0", anchor="w", font=("TkDefaultFont", 8))
            cv.create_text(XM0, B_BASE - 34,
                           text="СИНЯЯ — резонатор: звучит через КОНТАКТ с белой, молоток ПКМ, гравитацию",
                           fill="#8888a0", anchor="w", font=("TkDefaultFont", 8))
            for off, base, col in ((0, A_BASE, "#e8e8f0"),
                                   (N + 1, B_BASE, "#7f7fd0")):
                pts = []
                for j in range(N):
                    pts += [XM0 + j * (XM1 - XM0) / (N - 1),
                            base - float(sn[off + j]) * ZOOM]
                cv.create_line(*pts, fill=col, width=2)
            jb = int(P["beta"] * N)
            xj = XM0 + jb * (XM1 - XM0) / (N - 1)
            yj = A_BASE - float(sn[jb]) * ZOOM
            cv.create_oval(xj - 5, yj - 5, xj + 5, yj + 5, fill="#ff5040", width=0)
            if HST[2] > 0.5:
                jh = int(HST[3]) - (1 if HST[3] < N + 2 else 2)
                base = A_BASE if HST[3] < N + 2 else B_BASE
                xh = XM0 + jh * (XM1 - XM0) / (N - 1)
                yh = base - float(HST[0]) * ZOOM
                cv.create_oval(xh - 6, yh - 6, xh + 6, yh + 6, fill="#ffa020", width=0)
            lv = min(1.0, ST.level * 4)
            cv.create_rectangle(20, 280, 20 + int((W - 40) * lv), 292,
                                fill="#40c080", width=0)
        except Exception as e:
            print("draw:", e)
        root.after(40, draw)
    panel = tk.Frame(root); panel.pack(fill="x")
    left = [("R  скрежет<->бас", "R", 0.0, 1.0),
            ("Скорость смычка", "speed", 0.0, 1.6),
            ("Прижим", "press", 0.0, 2.0),
            ("Позиция смычка", "beta", 0.02, 0.30),
            ("Шум волоса", "noise", 0.0, 1.0),
            ("Затухание (хвост)", "ca", 0.002, 0.12)]
    right = [("Натяжение (0=резинка)", "tension", 0.0, 2.2),
             ("Нелинейность", "nl", 0.0, 0.15),
             ("Гравитация (синяя)", "grav", 0.0, 1.0),
             ("Зазор струн, мм", "gap", 0.5, 6.0),
             ("Громкость", "vol", 0.0, 1.0)]
    sv = {}
    def add_slider(row, col, lab, key, a, b):
        tk.Label(panel, text=lab, width=20, anchor="w").grid(row=row, column=col)
        var = tk.DoubleVar(value=P[key]); sv[key] = var
        sc = tk.Scale(panel, from_=a, to=b, resolution=0.005, orient="horizontal",
                      length=230, showvalue=1, variable=var,
                      command=(lambda v, k=key: P.__setitem__(k, float(v))))
        sc.grid(row=row, column=col + 1, sticky="w")
        sv[key + "_sc"] = sc
    for r_, (lab, key, a, b) in enumerate(left):
        add_slider(r_, 0, lab, key, a, b)
    for r_, (lab, key, a, b) in enumerate(right):
        add_slider(r_, 2, lab, key, a, b)
    buts = tk.Frame(root); buts.pack(fill="x")
    buts2 = tk.Frame(root); buts2.pack(fill="x")
    def toggle_bow():
        P["bow"] = not P["bow"]
        b_bow.config(text="СМЫЧОК: ВКЛ" if P["bow"] else "СМЫЧОК: выкл")
    def toggle_auto():
        P["automorph"] = not P["automorph"]
        b_auto.config(text="АВТО-МОРФ: ВКЛ" if P["automorph"] else "АВТО-МОРФ: выкл")
        sv["R_sc"].config(state="disabled" if P["automorph"] else "normal")
    def toggle_body():
        P["body"] = not P["body"]
        b_body.config(text="БОДИ: ВКЛ" if P["body"] else "БОДИ: выкл")
    def toggle_coll():
        P["coll"] = not P["coll"]
        b_coll.config(text="КОНТАКТ: ВКЛ" if P["coll"] else "КОНТАКТ: выкл")
    def toggle_prot():
        P["protect"] = not P["protect"]
        b_prot.config(text="ЗАЩИТА: АГС" if P["protect"] else "ЗАЩИТА: КЛИП")
    b_bow = tk.Button(buts, text="СМЫЧОК: ВКЛ", width=13, command=toggle_bow)
    b_auto = tk.Button(buts, text="АВТО-МОРФ: ВКЛ", width=15, command=toggle_auto)
    b_body = tk.Button(buts, text="БОДИ: ВКЛ", width=11, command=toggle_body)
    b_coll = tk.Button(buts, text="КОНТАКТ: ВКЛ", width=13, command=toggle_coll)
    b_prot = tk.Button(buts, text="ЗАЩИТА: КЛИП", width=13, command=toggle_prot)
    b_kick = tk.Button(buts2, text="ТОЛЧОК", width=9,
                       command=lambda: setattr(ST, "kick", True))
    b_toss = tk.Button(buts2, text="БРОСОК (мяч)", width=13,
                       command=lambda: setattr(ST, "toss", True))
    def rec_toggle():
        if not ST.rec:
            ST.rec = True; ST.recl = []; b_rec.config(text="ЗАПИСЬ: идёт")
        else:
            ST.rec = False; b_rec.config(text="ЗАПИСЬ")
            if ST.recl:
                x = np.concatenate(ST.recl)
                p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
                x = np.clip(x * 0.9 / p, -1, 1)
                fn = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                  "jam_%s.wav"
                                  % datetime.datetime.now().strftime("%H%M%S"))
                with wave.open(fn, "wb") as w:
                    w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
                    w.writeframes((x * 32767).astype("<i2").tobytes())
                print("записано:", fn)
    b_rec = tk.Button(buts2, text="ЗАПИСЬ", width=10, command=rec_toggle)
    qual = tk.OptionMenu(buts2, tk.StringVar(value="НОРМА"), *OVS_MAP,
                         command=lambda v: setattr(ST, "pending_ovs", OVS_MAP[v]))
    qual.config(width=7)
    b_quit = tk.Button(buts2, text="ВЫХОД", width=8, command=root.destroy)
    for b in (b_bow, b_auto, b_body, b_coll, b_prot): b.pack(side="left", padx=3, pady=3)
    for b in (b_kick, b_toss, b_rec, qual, b_quit): b.pack(side="left", padx=3, pady=3)
    status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")
    def tick():
        try:
            if P["automorph"]: sv["R"].set(ST.R_now)
            tc = "  [натяжение: предел]" if ST.tclamp else ""
            status.config(text=f"CPU {ST.cpu*100:4.0f}% (1 ядро)  сбои {ST.underr}  "
                               f"стик {ST.stick*100:3.0f}%  контакт {ST.cont*100:3.0f}%  "
                               f"R={ST.R_now:.2f}"
                               + ("  ЗАПИСЬ..." if ST.rec else "") + tc)
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def pick(e):
        j = round((e.x - XM0) * (N - 1) / (XM1 - XM0)); j = max(0, min(N - 1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j]) * ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N + 1 + j]) * ZOOM))
        i = j if dA <= dB else N + j
        base = A_BASE if i < N else B_BASE
        ST.grab = (i, max(-0.03, min(0.03, (base - e.y) / ZOOM)))
    def pluck(e):
        j = round((e.x - XM0) * (N - 1) / (XM1 - XM0)); j = max(0, min(N - 1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j]) * ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N + 1 + j]) * ZOOM))
        if dA <= dB:
            jh = j + 1; base = A_BASE; ystr = float(ST.snap[j])
        else:
            jh = j + N + 2; base = B_BASE; ystr = float(ST.snap[N + 1 + j])
        sgn = 1.0 if e.y < base - ystr * ZOOM else -1.0
        ST.pluck = (jh, sgn)
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", lambda e: ST.grab and pick(e))
    cv.bind("<ButtonRelease-1>", lambda e: setattr(ST, "grab", None))
    cv.bind("<Button-3>", pluck)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<k>", lambda e: setattr(ST, "kick", True))
    root.bind("<b>", lambda e: setattr(ST, "toss", True))
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    print("""БЕЛАЯ — игровая (смычок, молоток ПКМ, рука ЛКМ); СИНЯЯ — резонатор
(контакт с белой, молоток, гравитация/БРОСОК «мяч»).
Смычок: сила теперь привязана к позиции и прижиму — крути Прижим 0.3..2.0,
Скорость, R: характер обязан меняться. Шум волоса — слышимый скрежет.
Потолок спектра теперь ~8 кГц (N=192). ЗАЩИТА КЛИП = честный путь.""")
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                             callback=audio_cb, latency="low")
    stream.start()
    draw(); tick(); root.mainloop()
    stream.stop(); stream.close()

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    for q, o in OVS_MAP.items():
        print(f"  {q}: w*dt={math.sqrt((4.0*KA1 + 16.0*(K4A if o>=2 else 0.0))/M0)/(SR*o):.2f} "
              f"| предел натяжения {tension_limit(o):.2f}")
    if "diag" in args:
        print(sd.query_devices()); print("default:", sd.default.device); return
    print("компиляция ядра (один раз, до ~20 c)...")
    t0 = time.time()
    process_block(64); process_block(64)
    vE[:] = 0.0; yE[:] = 0.0; BS1[:] = 0.0; BS2[:] = 0.0
    DCS[0] = DCS[1] = 0.0; HST[:] = 0.0; ST.t = 0.0; ST.gain = 0.0
    print(f"ядро готово за {time.time()-t0:.1f} c")
    if "render" in args:
        render_wav(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "bow_render.wav")); return
    run_gui()

try:
    import sounddevice as sd
    if __name__ == "__main__":
        main()
except Exception:
    import traceback; traceback.print_exc()
finally:
    try: input("\nEnter — закрыть...")
    except Exception: pass