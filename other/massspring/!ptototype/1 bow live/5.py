#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v12) — сеть струн со связями (как ModularVST)
#  * СТРУНЫ: кнопка +СТРУНА (до 6), слайдеры для выбранной
#  * СВЯЗИ: Shift+клик узел -> Shift+клик узел другой струны;
#    тип ПРУЖИНА/НИТКА (нитка = только растяжение, металл)
#  * ПКМ = щипок (удержание тянет до 1.5 мм, отпуск = импакт)
#  * пины (prepared): Alt+клик; Ctrl+клик — удалить связь/пин
#  * натяжение 0: провис, лежит на соседней, НЕ взрывается
#  * АВТО-РЕСЕТ выкл по умолчанию; ручной СБРОС (r)
# Зависимости: python -m pip install numpy sounddevice numba
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np
from numba import njit

SR, BLOCK = 44100, 512
N, L, MU = 160, 1.0, 0.04
INHARM = 2.0e-5
MAXS, MAXLNK, MAXP = 6, 24, 8
KC, CC = 8.0e5, 0.5
FCAP_ABS = 150.0
FR = 0.55
KG, CG, GCAP = 150.0, 1.2, 0.02
F0_0 = 82.41
DX = L / (N + 1); M0 = MU * DX
Z0_0 = MU * 2 * L * F0_0
J1, J2 = int(0.28 * N) + 1, int(0.66 * N) + 1

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

LN1 = np.zeros(MAXLNK, np.int64); LN2 = np.zeros(MAXLNK, np.int64)
LREST = np.zeros(MAXLNK); LTYPE = np.zeros(MAXLNK, np.int64)
PINX = np.zeros(MAXP); PINW0 = np.zeros(MAXP)
PINR = np.zeros(MAXP); PINY = np.zeros(MAXP); PINV = np.zeros(MAXP)

@njit(cache=True, fastmath=True)
def core(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         S, N, a_dx, D0,
         karr, k4arr, brarr, camarr, fnlarr, garr, im, z0e,
         gap, kc, cc, fcap, fr, speed, press, nzd, jb, betap, coll,
         gc, gt, kg, cg, gcap, fnlc, aLP,
         j1, j2, j3, w3,
         body_on, bc1, br2, bb0, bgn, bs1, bs2, dcs, stats,
         nlk, ln1, ln2, lrest, ltype, kl, cl,
         npins, pinx, pinw0, pinr, piny, pinv, impin, pdamp,
         walls, yl, floor0):
    dcx = dcs[0]; dcy = dcs[1]; dca = dcs[2]; nzlp = dcs[3]
    bsq = math.sqrt(0.08 / max(betap, 0.02))
    for i in range(frames):
        R = Rarr[i]; on = onarr[i]
        vb0 = (0.06 + 0.34 * R) * speed
        fn0 = press * 1.2 * bsq * z0e * vb0
        vf = 0.004 + 0.030 * R
        for o in range(ovs):
            for s in range(S):                       # струны
                base = s * (N + 2)
                k = karr[s]; k4 = k4arr[s]; br = brarr[s]
                cam = camarr[s]; fnl = fnlarr[s]; g = garr[s]
                for j in range(1, N + 1):
                    jj = base + j
                    curv = yE[jj - 1] - 2.0 * yE[jj] + yE[jj + 1]
                    dv = vE[jj - 1] - 2.0 * vE[jj] + vE[jj + 1]
                    f = k * curv + br * dv - cam * vE[jj]
                    if fnl > 0.0 and 1 < j < N:
                        mlt = 1.0 + fnl * ((yE[jj] - yE[jj - 1]) ** 2
                                           + (yE[jj + 1] - yE[jj]) ** 2)
                        if mlt > fnlc: mlt = fnlc
                        f += k * curv * (mlt - 1.0)
                    if k4 > 0.0 and 2 <= j <= N - 1:
                        f -= k4 * (yE[jj - 2] - 4.0 * yE[jj - 1] + 6.0 * yE[jj]
                                   - 4.0 * yE[jj + 1] + yE[jj + 2])
                    vE[jj] += f * im - g * dt
            if coll:                                 # контакт соседних
                for s in range(S - 1):
                    b1 = s * (N + 2); b2 = b1 + N + 2
                    for j in range(1, N + 1):
                        dl = -(D0 - gap + yE[b2 + j] - yE[b1 + j])
                        if dl > 0.0:
                            if dl > 0.002: dl = 0.002
                            dmp = 1.0 + cc * (vE[b1 + j] - vE[b2 + j])
                            if dmp < 0.0: dmp = 0.0
                            elif dmp > 2.0: dmp = 2.0
                            fc = kc * dl ** 1.5 * dmp
                            if fc > fcap: fc = fcap
                            vE[b1 + j] += fc * im
                            vE[b2 + j] -= fc * im
                            stats[1] += 1.0
            for q in range(nlk):                     # СВЯЗИ
                i1 = ln1[q]; i2 = ln2[q]
                s1 = i1 // (N + 2); s2 = i2 // (N + 2)
                jj1 = i1 - s1 * (N + 2) - 1; jj2 = i2 - s2 * (N + 2) - 1
                dx = (jj2 - jj1) * a_dx
                dy = (yE[i2] - yE[i1]) + (s2 - s1) * D0
                d = math.sqrt(dx * dx + dy * dy) + 1e-12
                str_ = d - lrest[q]
                if ltype[q] == 1 and str_ <= 0.0:
                    continue
                F = kl * str_
                cdv = (vE[i2] - vE[i1]) * dy / d
                F += cl * cdv
                if F > fcap: F = fcap
                elif F < -fcap: F = -fcap
                Fy = F * dy / d
                vE[i1] += Fy * im
                vE[i2] -= Fy * im
            for p in range(npins):                   # ПИНЫ (мировые)
                yc = pinw0[p] + piny[p]
                r = pinr[p]
                j0 = int(pinx[p])
                if j0 > N - 1: j0 = N - 1
                if j0 < 0: j0 = 0
                for s in range(S):
                    jj = s * (N + 2) + j0 + 1
                    dy = yE[jj] - yc
                    ady = dy if dy >= 0.0 else -dy
                    if ady < r:
                        dl = r - ady
                        sg = 1.0 if dy >= 0.0 else -1.0
                        dmp = 1.0 + cc * (sg * (vE[jj] - pinv[p]))
                        if dmp < 0.0: dmp = 0.0
                        elif dmp > 2.0: dmp = 2.0
                        F = kc * dl ** 1.5 * dmp
                        if F > fcap: F = fcap
                        vE[jj] += sg * F * im
                        pinv[p] -= sg * F * impin
                pinv[p] *= (1.0 - pdamp)
                piny[p] += pinv[p] * dt
                if pinv[p] > 50.0 or pinv[p] < -50.0 or piny[p] > 1.0 \
                        or piny[p] < -1.0:
                    piny[p] = 0.0; pinv[p] = 0.0
            if gc >= 0:                              # рука/щипок (окно)
                s_ = gc // (N + 2); jjc = gc - s_ * (N + 2)
                lo = jjc - 4
                if lo < 1: lo = 1
                hi = jjc + 4
                if hi > N: hi = N
                base = s_ * (N + 2)
                for jj in range(lo, hi + 1):
                    w = 1.0 - abs(jj - jjc) / 5.0
                    if w > 0.0:
                        f = w * (kg * (gt - yE[base + jj]) - cg * vE[base + jj])
                        dv2 = f * im
                        if dv2 > gcap: dv2 = gcap
                        elif dv2 < -gcap: dv2 = -gcap
                        vE[base + jj] += dv2
            if on > 0.0 and fn0 > 0.0:               # смычок (неявный MSW)
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
            for s in range(S):                       # позиции
                base = s * (N + 2)
                for j in range(1, N + 1):
                    yE[base + j] += vE[base + j] * dt
            if walls:                                # СТЕНКИ ±yl
                tot = S * (N + 2)
                for jj in range(1, tot - 1):
                    if (jj % (N + 2)) != 0 and (jj % (N + 2)) != N + 1:
                        if yE[jj] > yl:
                            yE[jj] = yl
                            if vE[jj] > 0.0: vE[jj] = -0.5 * vE[jj]
                        elif yE[jj] < -yl:
                            yE[jj] = -yl
                            if vE[jj] < 0.0: vE[jj] = -0.5 * vE[jj]
            if floor0 > 0.5:                         # пол нижней струны
                for j in range(1, N + 1):
                    if yE[j] < -0.010:
                        yE[j] = -0.010
                        if vE[j] < 0.0: vE[j] = -0.3 * vE[j]
        x = 2.5 * vE[j1] + 1.2 * vE[j2] + w3 * vE[j3]
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

# --- параметры струн (динамические) ---
def new_string_params(idx):
    f0 = F0_0 * (1.4983 ** idx) * (1.0 + 0.003 * math.sin(idx * 2.3))
    return dict(t=0.8, nl=0.04, ca=0.02, grav=0.0, f0=f0)

SP = [new_string_params(0), new_string_params(1)]

P = dict(R=0.0, speed=1.0, press=1.0, gap=2.0, D0=12.0, pinr=3.0, pinm=15.0,
         linkk=20000.0, brk=0.25, noise=0.25, beta=0.08, vol=0.35,
         bow=True, coll=True, automorph=True, body=True, protect=False,
         autoreset=False, walls=False)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}
SEL = [0]                                    # выбранная струна

vE = np.zeros(len(SP) * (N + 2)); yE = np.zeros(len(SP) * (N + 2))
ST = type("ST", (), {})()
ST.ovs = 2; ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0
ST.snap = yE.copy()
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0; ST.stick = 0.0; ST.cont = 0.0
ST.rms_ema = 0.0; ST.gain = 0.0
ST.grab = None; ST.grab_pkm = False
ST.rec = False; ST.pending_ovs = None; ST.recl = []
ST.npins = 0; ST.nlk = 0; ST.lnk_start = -1
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

def k1_of(f0): return M0 * (2.0 * L * f0 / DX) ** 2
def k4_of(k1): return INHARM * k1 * (L / DX) ** 2
def tension_limit(ovs):
    k4e = k4_of(k1_of(SP[0]["f0"])) if ovs >= 2 else 0.0
    dt = 1.0 / (SR * ovs)
    w2 = (1.8 / dt) ** 2
    return max((w2 * M0 - 16.0 * k4e) / (4.0 * k1_of(SP[0]["f0"])), 0.0)

def rebuild_arrays():
    global vE, yE
    n = len(SP) * (N + 2)
    vE = np.resize(vE, n); yE = np.resize(yE, n)
    vE[n - (N + 2):] = 0.0; yE[n - (N + 2):] = 0.0
    ST.snap = yE.copy()

def reset_all():
    vE[:] = 0.0; yE[:] = 0.0; BS1[:] = 0.0; BS2[:] = 0.0
    DCS[0] = DCS[1] = 0.0; PINY[:] = 0.0; PINV[:] = 0.0

def process_block(frames):
    if ST.pending_ovs is not None:
        ST.ovs = ST.pending_ovs; ST.pending_ovs = None
    S = len(SP)
    ovs = ST.ovs; dt = 1.0 / (SR * ovs); im = dt / M0
    karr = np.empty(S); k4arr = np.empty(S); brarr = np.empty(S)
    camarr = np.empty(S); fnlarr = np.empty(S); garr = np.empty(S)
    tmax = tension_limit(ovs) if ovs >= 2 else 99.0
    for s in range(S):
        p = SP[s]
        k1 = k1_of(p["f0"])
        teff = min(p["t"], tmax) if p["t"] > 0.0 else 0.0
        k = k1 * teff
        karr[s] = k
        k4arr[s] = k4_of(k1) if ovs >= 2 else 0.0
        brarr[s] = 1e-4 * math.sqrt(max(k, 1.0) * M0)
        camarr[s] = p["ca"] * M0
        fnlarr[s] = p["nl"]
        garr[s] = p["grav"]
    fnlc = 1.0 + 2.0 * max(p_["nl"] for p_ in SP)
    z0e = Z0_0 * math.sqrt(max(SP[0]["t"], 1e-6))
    aLP = 1.0 - math.exp(-2.0 * math.pi * 2500.0 * dt)
    fcap = min(FCAP_ABS, 1.2 * M0 / dt)
    gapr = min(P["gap"] * 1e-3, P["D0"] * 1e-3 * 0.9)
    D0m = P["D0"] * 1e-3
    kl = P["linkk"]
    cl = 0.10 * math.sqrt(kl * 2.0 * M0)
    impin = dt / (P["pinm"] * M0)
    phase = (ST.t + np.arange(frames) / SR) % 7.0
    Rarr = np.interp(phase, TT, RT) if P["automorph"] else np.full(frames, P["R"])
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    onarr = np.clip(ST.on + stp * np.arange(1, frames + 1), 0.0, 1.0)
    ST.on = float(onarr[-1]); ST.t += frames / SR
    noise = RNG.standard_normal(frames * ovs)
    gc, gt = -1, 0.0
    if ST.grab is not None:
        gi, gt = ST.grab
        gc = gi
    jb = int(P["beta"] * N) + 1
    j3 = (N + 2) + N // 2 + 1 if S > 1 else j1
    w3 = 0.8 if S > 1 else 0.0
    out = np.empty(frames); STATS[0] = 0.0; STATS[1] = 0.0
    t0c = time.perf_counter()
    core(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         S, N, DX, D0m, karr, k4arr, brarr, camarr, fnlarr, garr, im, z0e,
         gapr, KC, CC, fcap, FR, P["speed"], P["press"], P["noise"], jb,
         P["beta"], P["coll"],
         gc, gt, KG, CG, GCAP, fnlc, aLP, J1, J2, j3, w3,
         1.0 if P["body"] else 0.0, BC1_, BR2_, BB0_, BODYG, BS1, BS2,
         DCS, STATS,
         ST.nlk, LN1, LN2, LREST, LTYPE, kl, cl,
         ST.npins, PINX, PINW0, PINR, PINY, PINV, impin, 0.0005,
         1.0 if P["walls"] else 0.0, P["brk"],
         1.0)                                     # пол нижней струны
    ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0c) / (frames / SR)
    # защита: NaN -> чистка (всегда); перегруз -> кламп или авторесет
    bad = (not np.isfinite(vE).all()) or (not np.isfinite(yE).all())
    if bad:
        reset_all(); out[:] = 0.0
        print("АВАРИЯ (NaN) -> чистка состояния")
    else:
        over = np.abs(yE).max() > 0.6 or np.abs(vE).max() > 80.0
        if over:
            np.clip(vE, -40.0, 40.0, out=vE)
            np.clip(yE, -0.5, 0.5, out=yE)
            if P["autoreset"]:
                for s in range(S):
                    b = s * (N + 2)
                    if np.abs(yE[b + 1:b + N + 1]).max() > P["brk"]:
                        vE[b:b + N + 2] = 0.0; yE[b:b + N + 2] = 0.0
                        print(f"авто-ресет струны {s}")
    ST.snap = yE.copy()
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
    root = tk.Tk(); root.title("physmod live v12 — сеть струн")
    W = 980
    cv = tk.Canvas(root, width=W, height=380, bg="#101018", highlightthickness=0)
    cv.pack(fill="x")
    XM0, XM1 = 40, W - 40
    ZOOM = 8000.0
    def base_scr(s):
        D0px = P["D0"] * 1e-3 * ZOOM
        return 340 - s * D0px
    def node_screen(s, j):
        gi = s * (N + 2) + j + 1
        return (XM0 + j * (XM1 - XM0) / (N - 1),
                base_scr(s) - float(yE[gi]) * ZOOM)
    def draw():
        try:
            cv.delete("all")
            D0px = P["D0"] * 1e-3 * ZOOM
            cv.create_text(XM0, 12,
                text="Shift+клик: связь (2 клика) | Alt+клик: пин | Ctrl+клик: удалить | ПКМ: щипок | ЛКМ: рука",
                fill="#666680", anchor="w", font=("TkDefaultFont", 8))
            for s in range(len(SP)):
                col = "#e8e8f0" if s == 0 else ("#7f7fd0" if s % 2 else "#6fbf9f")
                pts = []
                for j in range(N):
                    x_, y_ = node_screen(s, j)
                    pts += [x_, y_]
                cv.create_line(*pts, fill=col,
                               width=3 if s == SEL[0] else 2)
                cv.create_text(XM0, base_scr(s) - 26,
                               text=("▶ " if s == SEL[0] else "")
                                    + f"стр.{s} f0={SP[s]['f0']:.1f}",
                               fill="#8888a0" if s != SEL[0] else "#ffcc60",
                               anchor="w", font=("TkDefaultFont", 8))
            for q in range(ST.nlk):                  # связи
                s1 = LN1[q] // (N + 2); j1_ = LN1[q] - s1 * (N + 2) - 1
                s2 = LN2[q] // (N + 2); j2_ = LN2[q] - s2 * (N + 2) - 1
                x1_, y1_ = node_screen(s1, j1_)
                x2_, y2_ = node_screen(s2, j2_)
                cv.create_line(x1_, y1_, x2_, y2_,
                               fill="#ffd040" if LTYPE[q] == 0 else "#ff70a0",
                               width=1)
            for p in range(ST.npins):
                x_ = XM0 + PINX[p] * (XM1 - XM0) / (N - 1)
                y_ = 340 - (PINW0[p] + PINY[p]) * ZOOM
                r_ = max(3, int(PINR[p] * ZOOM))
                cv.create_oval(x_ - r_, y_ - r_, x_ + r_, y_ + r_,
                               fill="#40b0ff", width=0)
            jb = int(P["beta"] * N)
            x_, y_ = node_screen(0, jb)
            cv.create_oval(x_ - 5, y_ - 5, x_ + 5, y_ + 5,
                           fill="#ff5040", width=0)
            if ST.lnk_start >= 0:
                s1 = ST.lnk_start // (N + 2)
                j1_ = ST.lnk_start - s1 * (N + 2) - 1
                x_, y_ = node_screen(s1, j1_)
                cv.create_oval(x_ - 7, y_ - 7, x_ + 7, y_ + 7,
                               outline="#ffd040", width=2)
            lv = min(1.0, ST.level * 4)
            cv.create_rectangle(20, 362, 20 + int((W - 40) * lv), 374,
                                fill="#40c080", width=0)
        except Exception as e:
            print("draw:", e)
        root.after(40, draw)
    panel = tk.Frame(root); panel.pack(fill="x")
    def mk(row, col, lab, key, a, b):
        tk.Label(panel, text=lab, width=17, anchor="w").grid(row=row, column=col)
        var = tk.DoubleVar(value=P[key])
        sc = tk.Scale(panel, from_=a, to=b, resolution=0.005,
                      orient="horizontal", length=180, showvalue=1, variable=var,
                      command=(lambda v, k=key: P.__setitem__(k, float(v))))
        sc.grid(row=row, column=col + 1, sticky="w")
        return sc
    mk(0, 0, "R скрежет<->бас", "R", 0.0, 1.0)
    mk(1, 0, "Скорость смычка", "speed", 0.0, 1.6)
    mk(2, 0, "Прижим", "press", 0.0, 2.0)
    mk(3, 0, "Позиция смычка", "beta", 0.02, 0.30)
    mk(4, 0, "Шум волоса", "noise", 0.0, 1.0)
    mk(0, 2, "Зазор соседних, мм", "gap", 0.5, 8.0)
    mk(1, 2, "Шаг стека, мм", "D0", 8.0, 30.0)
    mk(2, 2, "Жёсткость связи", "linkk", 500.0, 200000.0)
    mk(3, 2, "Предел отрыва", "brk", 0.03, 0.50)
    mk(4, 2, "Громкость", "vol", 0.0, 1.0)
    per = tk.Frame(panel); per.grid(row=0, column=4, rowspan=6, padx=8)
    per_labels = [("Натяжение", "t", 0.0, 2.2), ("Нелин.", "nl", 0.0, 0.15),
                  ("Затухание", "ca", 0.002, 0.12), ("Гравитация", "grav", 0.0, 1.0)]
    pv = {}
    for r_, (lab, key, a, b) in enumerate(per_labels):
        tk.Label(per, text=f"[выбранная] {lab}", width=16, anchor="w").grid(
            row=r_, column=0)
        var = tk.DoubleVar(value=SP[SEL[0]][key])
        sc = tk.Scale(per, from_=a, to=b, resolution=0.005,
                      orient="horizontal", length=160, showvalue=1, variable=var,
                      command=(lambda v, k=key: SP[SEL[0]].__setitem__(k, float(v))))
        sc.grid(row=r_, column=1, sticky="w")
        pv[key] = (var, sc)
    def sel_change(d):
        SEL[0] = (SEL[0] + d) % len(SP)
        for key, (var, sc) in pv.items():
            var.set(SP[SEL[0]][key])
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
    def toggle_ar():
        P["autoreset"] = not P["autoreset"]
        b_ar.config(text="АВТО-РЕСЕТ: ВКЛ" if P["autoreset"]
                    else "АВТО-РЕСЕТ: выкл")
    def toggle_walls():
        P["walls"] = not P["walls"]
        b_walls.config(text="СТЕНКИ: ВКЛ" if P["walls"] else "СТЕНКИ: выкл")
    def cycle_ltype():
        ST._ltype = 1 - getattr(ST, "_ltype", 0)
        b_lt.config(text="СВЯЗЬ: НИТКА" if ST._ltype else "СВЯЗЬ: ПРУЖИНА")
    ST._ltype = 0
    b_bow = tk.Button(buts, text="СМЫЧОК: ВКЛ", width=12, command=toggle_bow)
    b_auto = tk.Button(buts, text="АВТО-МОРФ: ВКЛ", width=14, command=toggle_auto)
    b_body = tk.Button(buts, text="БОДИ: ВКЛ", width=10, command=toggle_body)
    b_coll = tk.Button(buts, text="КОНТАКТ: ВКЛ", width=12, command=toggle_coll)
    b_prot = tk.Button(buts, text="ЗАЩИТА: КЛИП", width=12, command=toggle_prot)
    b_ar = tk.Button(buts, text="АВТО-РЕСЕТ: выкл", width=14, command=toggle_ar)
    b_walls = tk.Button(buts, text="СТЕНКИ: выкл", width=12, command=toggle_walls)
    b_lt = tk.Button(buts, text="СВЯЗЬ: ПРУЖИНА", width=14, command=cycle_ltype)
    b_prev = tk.Button(buts, text="◄ стр.", width=6, command=lambda: sel_change(-1))
    b_next = tk.Button(buts, text="стр. ►", width=6, command=lambda: sel_change(1))
    for b in (b_bow, b_auto, b_body, b_coll, b_prot, b_ar, b_walls, b_lt,
              b_prev, b_next):
        b.pack(side="left", padx=2, pady=3)
    def add_string():
        if len(SP) < MAXS:
            SP.append(new_string_params(len(SP)))
            rebuild_arrays()
    def del_string():
        if len(SP) > 1:
            s = len(SP) - 1
            keep = [q for q in range(ST.nlk)
                    if LN1[q] // (N + 2) != s and LN2[q] // (N + 2) != s]
            ST.nlk = len(keep)
            for nq, q in enumerate(keep):
                LN1[nq] = LN1[q]; LN2[nq] = LN2[q]
                LREST[nq] = LREST[q]; LTYPE[nq] = LTYPE[q]
            SP.pop()
            if SEL[0] >= len(SP): SEL[0] = len(SP) - 1
            rebuild_arrays()
    def reset_all_btn():
        reset_all()
    b_add = tk.Button(buts2, text="+СТРУНА", width=9, command=add_string)
    b_del = tk.Button(buts2, text="−СТРУНА", width=9, command=del_string)
    def clear_lnk():
        ST.nlk = 0; ST.lnk_start = -1
    b_cl = tk.Button(buts2, text="СВЯЗИ: ОЧИСТ.", width=12, command=clear_lnk)
    def clear_pins():
        ST.npins = 0; PINY[:] = 0.0; PINV[:] = 0.0
    b_cp = tk.Button(buts2, text="ПИНЫ: ОЧИСТ.", width=12, command=clear_pins)
    b_res = tk.Button(buts2, text="СБРОС (r)", width=10, command=reset_all_btn)
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
    for b in (b_add, b_del, b_cl, b_cp, b_res, b_rec, qual, b_quit):
        b.pack(side="left", padx=2, pady=3)
    status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")
    def tick():
        try:
            if P["automorph"]: pass
            status.config(text=f"CPU {ST.cpu*100:4.0f}%  сбои {ST.underr}  "
                               f"стик {ST.stick*100:3.0f}%  контакт {ST.cont*100:3.0f}%  "
                               f"струн {len(SP)}  связей {ST.nlk}  пинов {ST.npins}"
                               + ("  ЗАПИСЬ..." if ST.rec else ""))
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def nearest_node(e):
        best = (0, 0); bd = 1e18
        for s in range(len(SP)):
            for dj in (-2, -1, 0, 1, 2):
                j = int(round((e.x - XM0) * (N - 1) / (XM1 - XM0))) + dj
                if j < 0 or j > N - 1: continue
                x_, y_ = node_screen(s, j)
                d = (e.x - x_) ** 2 + (e.y - y_) ** 2
                if d < bd: bd, best = d, (s, j)
        return best, bd
    def pick(e):
        if e.state & 0x0004:                          # Ctrl: удалить связь/пин
            bq, bd = -1, 1e18
            for q in range(ST.nlk):
                s1 = LN1[q] // (N + 2); j1_ = LN1[q] - s1 * (N + 2) - 1
                s2 = LN2[q] // (N + 2); j2_ = LN2[q] - s2 * (N + 2) - 1
                x1_, y1_ = node_screen(s1, j1_)
                x2_, y2_ = node_screen(s2, j2_)
                mx, my = (x1_ + x2_) / 2, (y1_ + y2_) / 2
                d = (e.x - mx) ** 2 + (e.y - my) ** 2
                if d < bd: bd, bq = d, q
            bp, bdp = -1, 1e18
            for p in range(ST.npins):
                x_ = XM0 + PINX[p] * (XM1 - XM0) / (N - 1)
                y_ = 340 - (PINW0[p] + PINY[p]) * ZOOM
                d = (e.x - x_) ** 2 + (e.y - y_) ** 2
                if d < bdp: bdp, bp = d, p
            if bq >= 0 and (bdp < 0 or bd <= bdp):
                ST.nlk -= 1
                for qq in range(bq, ST.nlk):
                    LN1[qq] = LN1[qq + 1]; LN2[qq] = LN2[qq + 1]
                    LREST[qq] = LREST[qq + 1]; LTYPE[qq] = LTYPE[qq + 1]
            elif bp >= 0:
                ST.npins -= 1
                for qq in range(bp, ST.npins):
                    PINX[qq] = PINX[qq + 1]; PINW0[qq] = PINW0[qq + 1]
                    PINR[qq] = PINR[qq + 1]; PINY[qq] = PINY[qq + 1]
                    PINV[qq] = PINV[qq + 1]
            ST.lnk_start = -1
            return
        if e.state & 0x0001:                          # Shift: связь
            (s, j), d = nearest_node(e)
            gi = s * (N + 2) + j + 1
            if ST.lnk_start < 0:
                ST.lnk_start = gi
            elif ST.lnk_start != gi and ST.nlk < MAXLNK:
                s1 = ST.lnk_start // (N + 2); j1_ = ST.lnk_start - s1 * (N + 2) - 1
                dx = (j - j1_) * DX
                dy = (yE[gi] - yE[ST.lnk_start]) + (s - s1) * P["D0"] * 1e-3
                LN1[ST.nlk] = ST.lnk_start; LN2[ST.nlk] = gi
                LREST[ST.nlk] = math.sqrt(dx * dx + dy * dy)
                LTYPE[ST.nlk] = ST._ltype
                ST.nlk += 1; ST.lnk_start = -1
            return
        if e.state & 0x20000:                         # Alt: пин
            if ST.npins < MAXP:
                PINX[ST.npins] = max(0.0, min(N - 1.0,
                    (e.x - XM0) * (N - 1) / (XM1 - XM0)))
                PINW0[ST.npins] = (340 - e.y) / ZOOM
                PINR[ST.npins] = P["pinr"] * 1e-3
                PINY[ST.npins] = 0.0; PINV[ST.npins] = 0.0
                ST.npins += 1
            ST.lnk_start = -1
            return
        (s, j), d = nearest_node(e)                   # ЛКМ: рука
        gi = s * (N + 2) + j + 1
        base_scr_s = base_scr(s)
        target = max(-0.03, min(0.03, (base_scr_s - e.y) / ZOOM - s * P["D0"] * 1e-3))
        ST.grab = (gi, target); ST.grab_pkm = False
    def pluck(e):                                     # ПКМ: щипок
        (s, j), d = nearest_node(e)
        gi = s * (N + 2) + j + 1
        base_scr_s = base_scr(s)
        sgn = 1.0 if e.y < base_scr_s else -1.0
        ST.grab = (gi, sgn * 0.0015); ST.grab_pkm = True
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", lambda e: (ST.grab and not ST.grab_pkm
            and not (e.state & 0x0005)) and pick(e))
    cv.bind("<ButtonRelease-1>", lambda e: (setattr(ST, "grab", None),
                                            setattr(ST, "grab_pkm", False)))
    cv.bind("<Button-3>", pluck)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<r>", lambda e: reset_all())
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    print("""СЕТЬ СТРУН: +СТРУНА добавляет; Shift+клик узел -> Shift+клик узел
другой струны = СВЯЗЬ (ПРУЖИНА/НИТКА — кнопка). Нитка тянет только на
растяжении -> ингармоничный металл. Связывай крест-накрест.
ПКМ: удерживай — струна тянется на 1.5 мм, отпусти — импакт.
Alt+клик — пин (prepared). Ctrl+клик — удалить ближайшее.
Слайдеры Натяжение/Нелин/Затухание/Гравитация — для ВЫБРАННОЙ струны (◄ ►).
Натяжение 0 + Гравитация: струна провисает и ЛЕЖИТ на соседней — не взрыв.
АВТО-РЕСЕТ выкл: перегруз только клампится; СБРОС (r) — руками.
Смычок — на струне 0, требует её натяжения > ~0.1.""")
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                             callback=audio_cb, latency="low")
    stream.start()
    draw(); tick(); root.mainloop()
    stream.stop(); stream.close()

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    if "diag" in args:
        import sounddevice as sd
        print(sd.query_devices()); print("default:", sd.default.device); return
    print("компиляция ядра (один раз, до ~20 c)...")
    t0 = time.time()
    process_block(64); process_block(64)
    reset_all(); ST.t = 0.0; ST.gain = 0.0
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