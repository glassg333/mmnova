#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v13.3, ФИНАЛ прототипа) — 2D-сеть масс (mini-miPhysics)
#  ЛКМ: рука (клик по струне = выбрать её материал) / РИСОВАТЬ струны
#  Shift+клик x2 = связь ПРУЖИНА/НИТКА; Alt+клик = пин (на струне —
#  резонатор f0/Q/накачка; на пустом — свободный мяч); Ctrl = удалить
#  ПКМ зажать-отпустить = щипок; МЯЧ = бросить мяч; ПРОБЕЛ = смычок (стр.0)
#  СЕТКА: размер радио-кнопками, спавн рядом с существующим, материал
#  наследуется от выбранной струны
#  Взрывы исключены: импульсные пределы + кламп скорости; лог -> окно+файл
# Зависимости: python -m pip install numpy sounddevice numba
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np
from numba import njit

SR, BLOCK = 44100, 512
ZOOM = 1000.0
CANH = 420
DX_M = 0.008
MU = 0.04
M0 = MU * DX_M
K_SCALE = 1.1e5
MAXM, MAXSPR, MAXLNK, MAXPIN, MAXPAIR = 1400, 3200, 48, 6, 6000
R_NODE = 0.003
FR = 0.55
VMAX = 50.0
DVS, DVC, DVL, DVP, DVH = 2.0, 0.30, 0.60, 1.00, 0.02
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)

X = np.zeros(MAXM); Y = np.zeros(MAXM)
VX = np.zeros(MAXM); VY = np.zeros(MAXM)
X0 = np.zeros(MAXM); Y0 = np.zeros(MAXM)
FIXM = np.zeros(MAXM, np.int64); SIDM = np.zeros(MAXM, np.int64)
RB = np.full(MAXM + MAXPIN, R_NODE)
SI = np.zeros(MAXSPR, np.int64); SJ = np.zeros(MAXSPR, np.int64)
SREST = np.zeros(MAXSPR); SSID = np.zeros(MAXSPR, np.int64)
SK = np.zeros(MAXSPR); SC = np.zeros(MAXSPR); SNL = np.zeros(MAXSPR)
LI = np.zeros(MAXLNK, np.int64); LJ = np.zeros(MAXLNK, np.int64)
LREST = np.zeros(MAXLNK); LTYPE = np.zeros(MAXLNK, np.int64)
PNH = np.full(MAXPIN, -1, np.int64)
PXP = np.zeros(MAXPIN); PYP = np.zeros(MAXPIN)
PVX = np.zeros(MAXPIN); PVY = np.zeros(MAXPIN)
PHX = np.zeros(MAXPIN); PHY = np.zeros(MAXPIN)
PI = np.zeros(MAXPAIR, np.int64); PJ = np.zeros(MAXPAIR, np.int64)
MN = np.zeros(16, np.int64); MNX = np.zeros(16); MNY = np.zeros(16)
MW = np.zeros(16)
GM = np.zeros(MAXM); CAMM = np.zeros(MAXM)
STATS = np.zeros(4)
DCS = np.zeros(2)

@njit(cache=True, fastmath=True)
def broadphase(Xa, Ya, n, px, py, npin, sidm, fixm, rb, pi_, pj_, maxpair):
    cnt = 0
    B = n + npin
    for a in range(B):
        if a < n:
            ax = Xa[a]; ay = Ya[a]; asid = sidm[a]; afix = fixm[a]; ar = rb[a]
        else:
            ax = px[a - n]; ay = py[a - n]; asid = -1; afix = 0; ar = rb[a]
        for b in range(a + 1, B):
            if b < n:
                if asid >= 0 and sidm[b] == asid:
                    continue
                if afix == 1 and fixm[b] == 1:
                    continue
                bx = Xa[b]; by = Ya[b]
            else:
                bx = px[b - n]; by = py[b - n]
            dx = bx - ax; dy = by - ay
            rr = ar + rb[b]
            if dx * dx + dy * dy < rr * rr:
                if cnt < maxpair:
                    pi_[cnt] = a; pj_[cnt] = b; cnt += 1
    return cnt

@njit(cache=True, fastmath=True)
def core2d(X, Y, VX, VY, FIXM, n,
           SI, SJ, SREST, SK, SC, SNL, nspr,
           LI, LJ, LREST, LTYPE, nlnk, kl, cll,
           PI, PJ, npair, RB,
           PXP, PYP, PVX, PVY, PNH, mpin, npin, pk_att, pk_c, gpin,
           GM, CAMM,
           grab_i, gtx, gty, dvh,
           bow_on, jb, bnx, bny, vb, fn0, vf, fr, nzamp, nzarr, aLP,
           dvs, dvc, dvl, dvp, vmax, dt, im,
           MN, MNX, MNY, MW, nmic,
           frames, ovs, out, stats, dcs):
    dcx = dcs[0]; dcy = dcs[1]; nzlp = 0.0
    clips = 0; vclips = 0; ncon = 0
    im_pin = dt / mpin
    mrat = mpin / M0
    for f in range(frames):
        for o in range(ovs):
            for q in range(nspr):
                i = SI[q]; j = SJ[q]
                dx = X[j] - X[i]; dy = Y[j] - Y[i]
                d = math.sqrt(dx * dx + dy * dy) + 1e-12
                e = d - SREST[q]
                strain = e / SREST[q]
                F = SK[q] * e * (1.0 + SNL[q] * strain * strain)
                nx = dx / d; ny = dy / d
                rel = (VX[j] - VX[i]) * nx + (VY[j] - VY[i]) * ny
                F += SC[q] * rel
                dv = F * im
                if dv > dvs:
                    dv = dvs; clips += 1
                elif dv < -dvs:
                    dv = -dvs; clips += 1
                if FIXM[i] == 0:
                    VX[i] += dv * nx; VY[i] += dv * ny
                if FIXM[j] == 0:
                    VX[j] -= dv * nx; VY[j] -= dv * ny
            for a in range(n):
                if FIXM[a] == 0:
                    VY[a] += GM[a] * dt
                    dmp = 1.0 - CAMM[a] * dt
                    if dmp < 0.0:
                        dmp = 0.0
                    VX[a] *= dmp; VY[a] *= dmp
            for q in range(npair):
                a = PI[q]; b = PJ[q]
                if a < n:
                    ax = X[a]; ay = Y[a]; avx = VX[a]; avy = VY[a]; af = FIXM[a]
                else:
                    ax = PXP[a - n]; ay = PYP[a - n]
                    avx = PVX[a - n]; avy = PVY[a - n]; af = 0
                if b < n:
                    bx = X[b]; by = Y[b]; bvx = VX[b]; bvy = VY[b]; bf = FIXM[b]
                else:
                    bx = PXP[b - n]; by = PYP[b - n]
                    bvx = PVX[b - n]; bvy = PVY[b - n]; bf = 0
                if af == 1 and bf == 1:
                    continue
                dx = bx - ax; dy = by - ay
                d = math.sqrt(dx * dx + dy * dy) + 1e-12
                pen = RB[a] + RB[b] - d
                if pen <= 0.0:
                    continue
                nx = dx / d; ny = dy / d
                closing = -((bvx - avx) * nx + (bvy - avy) * ny)
                dmp = 1.0 + 0.5 * closing
                if dmp < 0.0:
                    dmp = 0.0
                elif dmp > 2.0:
                    dmp = 2.0
                F = 8.0e5 * pen ** 1.5 * dmp
                dva = F * im
                if dva > dvc:
                    dva = dvc; clips += 1
                elif dva < -dvc:
                    dva = -dvc; clips += 1
                if a < n:
                    if af == 0:
                        VX[a] -= dva * nx; VY[a] -= dva * ny
                else:
                    PVX[a - n] -= dva * nx; PVY[a - n] -= dva * ny
                if b < n:
                    if bf == 0:
                        VX[b] += dva * nx; VY[b] += dva * ny
                else:
                    PVX[b - n] += dva * nx; PVY[b - n] += dva * ny
                ncon += 1
            for q in range(nlnk):
                i = LI[q]; j = LJ[q]
                dx = X[j] - X[i]; dy = Y[j] - Y[i]
                d = math.sqrt(dx * dx + dy * dy) + 1e-12
                e = d - LREST[q]
                if LTYPE[q] == 1 and e <= 0.0:
                    continue
                nx = dx / d; ny = dy / d
                rel = (VX[j] - VX[i]) * nx + (VY[j] - VY[i]) * ny
                F = kl * e + cll * rel
                dv = F * im
                if dv > dvl:
                    dv = dvl; clips += 1
                elif dv < -dvl:
                    dv = -dvl; clips += 1
                if FIXM[i] == 0:
                    VX[i] += dv * nx; VY[i] += dv * ny
                if FIXM[j] == 0:
                    VX[j] -= dv * nx; VY[j] -= dv * ny
            for p in range(npin):
                h = PNH[p]
                if h < 0:
                    PVY[p] -= gpin * dt
                    if PYP[p] < 0.006:
                        PYP[p] = 0.006
                        if PVY[p] < 0.0:
                            PVY[p] = -0.5 * PVY[p]
                    elif PYP[p] > 0.50:
                        PYP[p] = 0.50
                        if PVY[p] > 0.0:
                            PVY[p] = -0.5 * PVY[p]
                    if PXP[p] < 0.01:
                        PXP[p] = 0.01
                        if PVX[p] < 0.0:
                            PVX[p] = -0.5 * PVX[p]
                    elif PXP[p] > 0.99:
                        PXP[p] = 0.99
                        if PVX[p] > 0.0:
                            PVX[p] = -0.5 * PVX[p]
                else:
                    dx = X[h] - PXP[p]; dy = Y[h] - PYP[p]
                    Fx = pk_att * dx + pk_c * (VX[h] - PVX[p])
                    Fy = pk_att * dy + pk_c * (VY[h] - PVY[p])
                    dvx = Fx * im_pin; dvy = Fy * im_pin
                    if dvx > dvp:
                        dvx = dvp; clips += 1
                    elif dvx < -dvp:
                        dvx = -dvp; clips += 1
                    if dvy > dvp:
                        dvy = dvp; clips += 1
                    elif dvy < -dvp:
                        dvy = -dvp; clips += 1
                    PVX[p] += dvx; PVY[p] += dvy
                    rx = -dvx * mrat; ry = -dvy * mrat
                    if rx > dvc:
                        rx = dvc
                    elif rx < -dvc:
                        rx = -dvc
                    if ry > dvc:
                        ry = dvc
                    elif ry < -dvc:
                        ry = -dvc
                    if FIXM[h] == 0:
                        VX[h] += rx; VY[h] += ry
            if grab_i >= 0:
                dx = gtx - X[grab_i]; dy = gty - Y[grab_i]
                Fx = 120.0 * dx - 1.0 * VX[grab_i]
                Fy = 120.0 * dy - 1.0 * VY[grab_i]
                dvx = Fx * im
                if dvx > dvh:
                    dvx = dvh
                elif dvx < -dvh:
                    dvx = -dvh
                dvy = Fy * im
                if dvy > dvh:
                    dvy = dvh
                elif dvy < -dvh:
                    dvy = -dvh
                VX[grab_i] += dvx; VY[grab_i] += dvy
            if bow_on > 0.0 and fn0 > 0.0:
                nzl = nzlp + aLP * (nzarr[f * ovs + o] - nzlp)
                nzlp = nzl
                fn = fn0 * (1.0 + 0.5 * nzamp * nzl)
                if fn < 0.0:
                    fn = 0.0
                VX[jb] += nzamp * nzl * fn * 0.5 * im * bnx
                VY[jb] += nzamp * nzl * fn * 0.5 * im * bny
                vn = VX[jb] * bnx + VY[jb] * bny
                lo = vn - fn * im; hi = vn + fn * im
                for _ in range(40):
                    mid = 0.5 * (lo + hi)
                    u = vb - mid
                    mu = fr + (1.0 - fr) * math.exp(-abs(u) / vf)
                    if mid - vn - im * fn * mu < 0.0:
                        lo = mid
                    else:
                        hi = mid
                vnn = 0.5 * (lo + hi)
                dvb = vnn - vn
                VX[jb] += dvb * bnx; VY[jb] += dvb * bny
            for a in range(n):
                if FIXM[a] == 0:
                    X[a] += VX[a] * dt; Y[a] += VY[a] * dt
            for p in range(npin):
                PXP[p] += PVX[p] * dt; PYP[p] += PVY[p] * dt
            for a in range(n + npin):
                if a < n:
                    if FIXM[a] == 0:
                        v2 = VX[a] * VX[a] + VY[a] * VY[a]
                        if v2 > vmax * vmax:
                            kk = vmax / math.sqrt(v2)
                            VX[a] *= kk; VY[a] *= kk; vclips += 1
                else:
                    p = a - n
                    v2 = PVX[p] * PVX[p] + PVY[p] * PVY[p]
                    if v2 > vmax * vmax:
                        kk = vmax / math.sqrt(v2)
                        PVX[p] *= kk; PVY[p] *= kk; vclips += 1
        xs = 0.0
        for m in range(nmic):
            a = MN[m]
            if a < n:
                xs += MW[m] * (VX[a] * MNX[m] + VY[a] * MNY[m])
            else:
                p2 = a - n
                if p2 < npin:
                    xs += MW[m] * (PVX[p2] * MNX[m] + PVY[p2] * MNY[m])
        x = xs - dcx + dcs[1] * dcy
        dcx = xs; dcy = x
        out[f] = x
    stats[0] = clips; stats[1] = vclips; stats[2] = ncon; stats[3] = 0.0
    dcs[0] = dcx; dcs[1] = dcy

# ---------------- bookkeeping ----------------
class Str:
    pass

STS = []
N = [0]; NSPR = [0]; NLNK = [0]; NPIN = [0]
MESHSZ = [14, 9]
P = dict(R=0.0, speed=1.0, press=1.0, beta=0.10, noise=0.25,
         t=1.0, nl=0.04, ca=0.02, grav=0.0,
         pf0=300.0, pq=8.0, pump=0.0, pm=20.0, pg=0.6,
         linkk=20000.0, pinr=6.0, vol=0.35,
         bow=False, automorph=False, protect=False, autoreset=False, verb=True)
SEL = [0]
STt = type("ST", (), {})()
STt.ovs = 2; STt.t = 0.0; STt.on = 0.0; STt.R_now = 0.0
STt.tool = "hand"; STt.ltype = 0; STt.lnk_start = -1
STt.grab = None; STt.plk = None; STt.rec = False; STt.recl = []
STt.pending_ovs = None; STt.level = 0.0; STt.gain = 0.0
STt.cpu = 0.0; STt.underr = 0
STt.logq = []; STt.draw_pts = None
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

def reset_positions():
    m = N[0]
    X[:m] = X0[:m]; Y[:m] = Y0[:m]
    VX[:m] = 0.0; VY[:m] = 0.0
    for p in range(NPIN[0]):
        PXP[p] = PHX[p]; PYP[p] = PHY[p]; PVX[p] = 0.0; PVY[p] = 0.0
    DCS[0] = 0.0; DCS[1] = 0.0

def spawn_ball():
    if NPIN[0] >= MAXPIN:
        log("лимит пинов", True); return
    p = NPIN[0]
    PNH[p] = -1
    PXP[p] = 0.40 + 0.2 * float(RNG.random())
    PYP[p] = 0.36
    PVX[p] = (float(RNG.random()) - 0.5) * 0.8
    PVY[p] = 0.6
    PHX[p] = PXP[p]; PHY[p] = PYP[p]
    RB[N[0] + p] = P["pinr"] * 1e-3
    NPIN[0] += 1
    log(f"МЯЧ #{p} брошен (грав={P['pg']:.2f}) — скачет по струнам и полу", True)

def f0_est(sid):
    s = STS[sid]
    nseg = len(s.nodes) - 1
    if nseg < 1:
        return 0.0
    return math.sqrt(K_SCALE * s.sp["t"] / M0) / (2.0 * nseg)

def add_node(x, y, sid, fix):
    a = N[0]
    if a >= MAXM:
        return -1
    X[a] = x; Y[a] = y; X0[a] = x; Y0[a] = y
    VX[a] = 0.0; VY[a] = 0.0; FIXM[a] = fix; SIDM[a] = sid; RB[a] = R_NODE
    N[0] = a + 1
    return a

def add_spring(i, j, sid):
    q = NSPR[0]
    if q >= MAXSPR or i < 0 or j < 0 or i == j:
        return
    rest = math.hypot(X[j] - X[i], Y[j] - Y[i])
    if rest < 2.0e-3:
        return
    SREST[q] = rest
    SI[q] = i; SJ[q] = j; SSID[q] = sid
    NSPR[0] = q + 1

def draw_string(pts):
    if N[0] + 2 >= MAXM:
        log("лимит масс — ОЧИСТИТЬ сцену", True); return
    sid = len(STS)
    s = Str()
    s.sp = dict(t=P["t"], nl=P["nl"], ca=P["ca"], grav=P["grav"])
    s.nodes = []
    a0 = add_node(pts[0][0], pts[0][1], sid, 1)
    if a0 < 0:
        return
    s.nodes.append(a0)
    last = pts[0]
    for (mx, my) in pts[1:]:
        if math.hypot(mx - last[0], my - last[1]) >= DX_M:
            a = add_node(mx, my, sid, 0)
            if a < 0:
                break
            add_spring(s.nodes[-1], a, sid)
            s.nodes.append(a)
            last = (mx, my)
    if math.hypot(pts[-1][0] - X[s.nodes[-1]],
                  pts[-1][1] - Y[s.nodes[-1]]) >= DX_M * 0.5:
        a = add_node(pts[-1][0], pts[-1][1], sid, 1)
        if a >= 0:
            add_spring(s.nodes[-1], a, sid)
            s.nodes.append(a)
    if len(s.nodes) >= 2:
        STS.append(s)
        SEL[0] = sid
        log(f"СТРУНА #{sid}: {len(s.nodes)} узлов, f0≈{f0_est(sid):.0f} Гц "
            f"(материал t={s.sp['t']:.2f})")

def add_mesh():
    cols, rows = MESHSZ[0], MESHSZ[1]
    if N[0] + cols * rows >= MAXM:
        log("лимит масс — ОЧИСТИТЬ сцену", True); return
    if N[0] > 0:
        maxx = 0.0
        for a in range(N[0]):
            if X[a] > maxx:
                maxx = X[a]
        x00 = min(maxx + 0.06, 0.88 - cols * 0.014)
        x00 = max(x00, 0.02)
        y00 = 0.30
    else:
        x00, y00 = 0.30, 0.30
    src = STS[min(SEL[0], len(STS) - 1)].sp if STS else \
        dict(t=P["t"], nl=P["nl"], ca=P["ca"], grav=P["grav"])
    sid = len(STS)
    s = Str()
    s.sp = dict(t=src["t"], nl=src["nl"], ca=src["ca"], grav=src["grav"])
    s.nodes = []
    sp = 0.014
    grid = []
    for r in range(rows):
        row = []
        for c in range(cols):
            fix = 1 if (r == 0 or r == rows - 1 or c == 0 or c == cols - 1) else 0
            a = add_node(x00 + c * sp, y00 + r * sp, sid, fix)
            if a < 0:
                break
            row.append(a); s.nodes.append(a)
        grid.append(row)
    for r in range(rows):
        for c in range(cols):
            if c + 1 < cols:
                add_spring(grid[r][c], grid[r][c + 1], sid)
            if r + 1 < rows:
                add_spring(grid[r][c], grid[r + 1][c], sid)
    if len(s.nodes) < 4:
        return
    STS.append(s)
    SEL[0] = sid
    log(f"СЕТКА {cols}x{rows} @({x00:.2f},{y00:.2f})м, материал t={s.sp['t']:.2f}, "
        f"f0≈{f0_est(sid):.0f} Гц — выбрана")

def to_world(e):
    return (e.x / ZOOM, (CANH - e.y) / ZOOM)

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

def R_auto(ph):
    if ph < 1.2:
        return 0.0
    if ph < 2.7:
        return smoothstep((ph - 1.2) / 1.5)
    if ph < 4.2:
        return 1.0
    if ph < 5.7:
        return 1.0 - smoothstep((ph - 4.2) / 1.5)
    return 0.0

TT = np.linspace(0.0, 7.0, 701)
RT = np.array([R_auto(t) for t in TT])

def process_block(frames):
    if STt.pending_ovs is not None:
        STt.ovs = STt.pending_ovs
        STt.pending_ovs = None
    ovs = STt.ovs
    dt = 1.0 / (SR * ovs); im = dt / M0
    n = N[0]; nspr = NSPR[0]
    for q in range(nspr):
        sp = STS[SSID[q]].sp
        k = K_SCALE * sp["t"]
        SK[q] = k
        SC[q] = 1e-4 * math.sqrt(max(k, 1.0) * M0)
        SNL[q] = sp["nl"]
    for a in range(n):
        sp = STS[SIDM[a]].sp
        CAMM[a] = sp["ca"]
        GM[a] = -sp["grav"] * 3.0
    npin = NPIN[0]
    mpin = P["pm"] * M0
    w = 2.0 * math.pi * P["pf0"]
    pk_att = w * w * mpin
    pk_c = w * mpin / max(P["pq"], 0.5) * (1.0 - P["pump"])
    for p in range(npin):
        RB[n + p] = P["pinr"] * 1e-3
    npair = 0
    if n + npin > 1:
        npair = broadphase(X[:n], Y[:n], n, PXP, PYP, npin, SIDM, FIXM, RB,
                           PI, PJ, MAXPAIR)
    nmic = 0
    for s in STS:
        if len(s.nodes) >= 3:
            k_ = len(s.nodes) // 2
            mid = s.nodes[k_]
            a1 = s.nodes[k_ - 1]; a2 = s.nodes[k_ + 1]
            tx = X[a2] - X[a1]; ty = Y[a2] - Y[a1]
            tl = math.hypot(tx, ty)
            if tl > 1e-6 and nmic < 16:
                MN[nmic] = mid
                MNX[nmic] = -ty / tl; MNY[nmic] = tx / tl
                MW[nmic] = 1.0; nmic += 1
    for p in range(npin):
        if nmic < 16:
            MN[nmic] = n + p; MNX[nmic] = 0.0; MNY[nmic] = 1.0
            MW[nmic] = 0.6; nmic += 1
    grab_i, gtx, gty = -1, 0.0, 0.0
    if STt.grab is not None:
        grab_i, gtx, gty = STt.grab
    if STt.plk is not None:
        gi, dxn, dyn, t0 = STt.plk
        amt = min(1.5e-3, 0.045 * (time.time() - t0))
        grab_i = gi
        gtx = X0[gi] + dxn * amt; gty = Y0[gi] + dyn * amt
    bow_on = 0.0; jb = 0; bnx = 0.0; bny = 1.0
    z0s = math.sqrt(K_SCALE * M0)
    if STS and len(STS[0].nodes) >= 3:
        nds = STS[0].nodes
        idx = min(len(nds) - 2, int(P["beta"] * (len(nds) - 1)))
        jb = nds[idx]
        a1 = nds[max(0, idx - 1)]; a2 = nds[min(len(nds) - 1, idx + 1)]
        tx = X[a2] - X[a1]; ty = Y[a2] - Y[a1]
        tl = math.hypot(tx, ty)
        if tl > 1e-9:
            bnx = -ty / tl; bny = tx / tl; bow_on = 1.0
        z0s = math.sqrt(K_SCALE * STS[0].sp["t"] * M0)
    phase = (STt.t + np.arange(frames) / SR) % 7.0
    Rarr = np.interp(phase, TT, RT) if P["automorph"] else np.full(frames, P["R"])
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    STt.on = min(1.0, max(0.0, STt.on + stp * frames / SR))
    onv = STt.on
    STt.t += frames / SR
    vb = ((0.06 + 0.34 * float(Rarr[-1])) * P["speed"]) * onv
    fn0 = P["press"] * 1.2 * math.sqrt(0.08 / max(P["beta"], 0.02)) * z0s * vb
    vf = 0.004 + 0.030 * float(Rarr[-1])
    nzarr = RNG.standard_normal(frames * ovs)
    aLP = 1.0 - math.exp(-2.0 * math.pi * 2500.0 * dt)
    out = np.empty(frames)
    t0c = time.perf_counter()
    core2d(X, Y, VX, VY, FIXM, n,
           SI, SJ, SREST, SK, SC, SNL, nspr,
           LI, LJ, LREST, LTYPE, NLNK[0], P["linkk"],
           0.15 * math.sqrt(P["linkk"] * M0),
           PI, PJ, npair, RB,
           PXP, PYP, PVX, PVY, PNH, mpin, npin, pk_att, pk_c,
           P["pg"] * 3.0,
           GM, CAMM,
           grab_i, gtx, gty, DVH,
           bow_on, jb, bnx, bny, vb, fn0, vf, FR, P["noise"], nzarr, aLP,
           DVS, DVC, DVL, DVP, VMAX, dt, im,
           MN, MNX, MNY, MW, nmic,
           frames, ovs, out, STATS, DCS)
    STt.cpu = 0.9 * STt.cpu + 0.1 * (time.perf_counter() - t0c) / (frames / SR)
    if not (np.isfinite(X[:n]).all() and np.isfinite(VX[:n]).all()):
        reset_positions()
        out[:] = 0.0
        log("АВАРИЯ (NaN) -> полный сброс. Пришли лог!", True)
    if (STATS[0] > frames * ovs * 0.02 or STATS[1] > 10) and P["verb"]:
        log(f"клипы: сил {int(STATS[0])}/блок, скоростей {int(STATS[1])}", True)
        if P["autoreset"] and STATS[0] > frames * ovs * 0.2:
            reset_positions()
            log("авто-ресет по перегрузу", True)
    y = np.empty(frames)
    dcx, dcy = DCS[0], DCS[1]
    for i in range(frames):
        x = out[i]
        yi = x - dcx + DCA * dcy
        dcx = x; dcy = yi
        y[i] = yi
    DCS[0] = dcx; DCS[1] = dcy
    if P["protect"]:
        rms = float(np.sqrt(np.mean(y * y))) + 1e-12
        g_t = min(0.10 / rms, 3.0)
        if STt.gain == 0.0:
            STt.gain = g_t
        else:
            slew = 0.05 if g_t < STt.gain else 0.01
            STt.gain += max(min(g_t - STt.gain, slew), -slew)
        y = np.tanh(y * (STt.gain * P["vol"] * 3.0))
    else:
        y = np.clip(y * (1.2 * P["vol"]), -0.98, 0.98)
    STt.level = 0.9 * STt.level + 0.1 * float(np.mean(np.abs(y)))
    STt.R_now = float(Rarr[-1])
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status:
            STt.underr += 1
        outdata[:, 0] = process_block(frames)
        if STt.rec:
            STt.recl.append(outdata[:, 0].copy())
    except Exception:
        import traceback
        traceback.print_exc()
        outdata[:] = 0

# ---------------- GUI ----------------
BG, FG = "#14141c", "#e0e0e8"

def run_gui():
    import tkinter as tk
    root = tk.Tk()
    root.title("physmod live v13.3 — 2D сеть масс")
    root.configure(bg=BG)
    W = 1000
    cv = tk.Canvas(root, width=W, height=CANH, bg="#0d0d14",
                   highlightthickness=1, highlightbackground="#26263a")
    cv.pack(fill="x")
    bow_dot = [0, 0]
    def node_xy(a):
        return (X[a] * ZOOM, CANH - Y[a] * ZOOM)
    def draw():
        try:
            cv.delete("all")
            cv.create_text(10, 12, text="ЛКМ: рука/рисовать | Shift: связь | "
                           "Alt: пин | Ctrl: удалить | ПКМ: щипок | колесо: смычок",
                           fill="#666680", anchor="w", font=("TkDefaultFont", 9))
            for si, s in enumerate(STS):
                pts = []
                for a in s.nodes:
                    x_, y_ = node_xy(a)
                    pts += [x_, y_]
                if len(pts) >= 4:
                    col = "#4cc2a0" if si == SEL[0] else "#c8c8d8"
                    cv.create_line(*pts, fill=col,
                                   width=2 if si == SEL[0] else 1)
            for q in range(NLNK[0]):
                x1_, y1_ = node_xy(LI[q])
                x2_, y2_ = node_xy(LJ[q])
                cv.create_line(x1_, y1_, x2_, y2_,
                               fill="#ffd040" if LTYPE[q] == 0 else "#ff70a0",
                               width=1)
            n = N[0]
            for p in range(NPIN[0]):
                px_, py_ = PXP[p] * ZOOM, CANH - PYP[p] * ZOOM
                r_ = max(4, int(RB[n + p] * ZOOM))
                cv.create_oval(px_ - r_, py_ - r_, px_ + r_, py_ + r_,
                               fill="#40b0ff", width=0)
                h = PNH[p]
                if h >= 0:
                    hx_, hy_ = node_xy(h)
                    cv.create_line(px_, py_, hx_, hy_, fill="#40b0ff", width=1)
            if bow_dot[0]:
                x_, y_ = node_xy(bow_dot[1])
                cv.create_oval(x_ - 5, y_ - 5, x_ + 5, y_ + 5,
                               fill="#ff5040", width=0)
            if STt.lnk_start >= 0:
                x_, y_ = node_xy(STt.lnk_start)
                cv.create_oval(x_ - 7, y_ - 7, x_ + 7, y_ + 7,
                               outline="#ffd040", width=2)
            lv = min(1.0, STt.level * 4)
            cv.create_rectangle(10, CANH - 14, 10 + int((W - 20) * lv), CANH - 4,
                                fill="#40c080", width=0)
        except Exception as e:
            log(f"draw: {e}", False)
        root.after(40, draw)
    panel = tk.Frame(root, bg=BG); panel.pack(fill="x")
    mat_sliders = {}
    def mat_apply():
        if STS:
            STS[min(SEL[0], len(STS) - 1)].sp.update(
                t=P["t"], nl=P["nl"], ca=P["ca"], grav=P["grav"])
    def mk(row, col, lab, key, a, b, res=0.005):
        tk.Label(panel, text=lab, width=17, anchor="w", bg=BG, fg=FG
                 ).grid(row=row, column=col)
        sc = tk.Scale(panel, from_=a, to=b, resolution=res, orient="horizontal",
                      length=150, showvalue=1, bg=BG, fg=FG,
                      troughcolor="#22222e", highlightthickness=0,
                      activebackground="#33334a",
                      command=(lambda v, k=key: (P.__setitem__(k, float(v)),
                                                 mat_apply())))
        sc.set(P[key])
        sc.grid(row=row, column=col + 1, sticky="w")
        if key in ("t", "nl", "ca", "grav"):
            mat_sliders[key] = sc
        return sc
    mk(0, 0, "R скрежет<->бас", "R", 0.0, 1.0)
    mk(1, 0, "Скорость смычка", "speed", 0.0, 1.6)
    mk(2, 0, "Прижим", "press", 0.0, 2.0)
    mk(3, 0, "Позиция смычка", "beta", 0.02, 0.5)
    mk(4, 0, "Шум волоса", "noise", 0.0, 1.0)
    mk(0, 2, "Материал: натяжение", "t", 0.0, 2.2)
    mk(1, 2, "Материал: нелин.", "nl", 0.0, 0.15)
    mk(2, 2, "Материал: затухание", "ca", 0.002, 0.12)
    mk(3, 2, "Материал: гравитация", "grav", 0.0, 1.0)
    mk(0, 4, "ПИН: f0 резонатора", "pf0", 40.0, 1200.0, res=1.0)
    mk(1, 4, "ПИН: Q (звон)", "pq", 2.0, 40.0, res=0.5)
    mk(2, 4, "ПИН: накачка", "pump", 0.0, 0.95)
    mk(3, 4, "ПИН: масса, x узла", "pm", 2.0, 80.0, res=1.0)
    mk(4, 4, "ПИН: гравитация (мяч)", "pg", 0.0, 1.0)
    mk(0, 6, "Жёсткость связей", "linkk", 500.0, 200000.0, res=100.0)
    mk(1, 6, "Радиус пина, мм", "pinr", 4.0, 10.0, res=0.5)
    mk(2, 6, "Громкость", "vol", 0.0, 1.0)
    buts = tk.Frame(root, bg=BG); buts.pack(fill="x")
    buts2 = tk.Frame(root, bg=BG); buts2.pack(fill="x")
    def dbtn(parent, text, cmd, width=12):
        return tk.Button(parent, text=text, command=cmd, width=width,
                         bg="#1e1e2a", fg=FG, activebackground="#2a2a3c",
                         activeforeground=FG, relief="flat")
    def toggle_tool():
        STt.tool = "draw" if STt.tool == "hand" else "hand"
        b_tool.config(text="ИНСТРУМЕНТ: " + ("РИСОВАТЬ" if STt.tool == "draw"
                                             else "РУКА"))
    def toggle_ltype():
        STt.ltype = 1 - STt.ltype
        b_lt.config(text="СВЯЗЬ: " + ("НИТКА" if STt.ltype else "ПРУЖИНА"))
    def toggle_bow():
        P["bow"] = not P["bow"]
        b_bow.config(text="СМЫЧОК: ВКЛ" if P["bow"] else "СМЫЧОК: выкл")
    def toggle_verb():
        P["verb"] = not P["verb"]
        b_log.config(text="ЛОГ: ПОДРОБНО" if P["verb"] else "ЛОГ: кратко")
    def toggle_ar():
        P["autoreset"] = not P["autoreset"]
        b_ar.config(text="АВТО-РЕСЕТ: ВКЛ" if P["autoreset"]
                    else "АВТО-РЕСЕТ: выкл")
    def do_clear():
        N[0] = 0; NSPR[0] = 0; NLNK[0] = 0; NPIN[0] = 0
        STS.clear(); STt.lnk_start = -1; SEL[0] = 0
        bow_dot[0] = 0
        log("ОЧИСТКА сцены")
    def do_reset():
        reset_positions()
        log("СБРОС (в позиции покоя)")
    b_tool = dbtn(buts, "ИНСТРУМЕНТ: РУКА", toggle_tool, 18)
    b_lt = dbtn(buts, "СВЯЗЬ: ПРУЖИНА", toggle_ltype, 15)
    b_bow = dbtn(buts, "СМЫЧОК: выкл", toggle_bow, 13)
    b_log = dbtn(buts, "ЛОГ: ПОДРОБНО", toggle_verb, 14)
    b_ar = dbtn(buts, "АВТО-РЕСЕТ: выкл", toggle_ar, 15)
    b_grid = dbtn(buts, "СЕТКА: 14x9", lambda: add_mesh(), 14)
    b_clear = dbtn(buts, "ОЧИСТИТЬ", do_clear, 9)
    b_res = dbtn(buts, "СБРОС (r)", do_reset, 10)
    for b in (b_tool, b_lt, b_bow, b_log, b_ar, b_grid, b_clear, b_res):
        b.pack(side="left", padx=2, pady=3)
    meshf = tk.Frame(root, bg=BG); meshf.pack(fill="x")
    tk.Label(meshf, text="Размер сетки:", bg=BG, fg=FG).pack(side="left", padx=4)
    for opt, cr in (("малая 6x4", (6, 4)), ("средняя 10x6", (10, 6)),
                    ("большая 14x9", (14, 9)), ("плотная 20x12", (20, 12))):
        tk.Radiobutton(meshf, text=opt, value=opt,
                       bg=BG, fg=FG, selectcolor="#22222e",
                       activebackground=BG, activeforeground=FG,
                       command=lambda cr=cr: (MESHSZ.__setitem__(0, cr[0]),
                                              MESHSZ.__setitem__(1, cr[1]),
                                              b_grid.config(
                                                  text=f"СЕТКА: {cr[0]}x{cr[1]}"))
                       ).pack(side="left", padx=3)
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
    b_ball = dbtn(buts2, "МЯЧ", spawn_ball, 8)
    b_rec = dbtn(buts2, "ЗАПИСЬ", rec_toggle, 9)
    qual = tk.OptionMenu(buts2, tk.StringVar(value="НОРМА"),
                         "ЭКО", "НОРМА", "МАКС",
                         command=lambda v: setattr(STt, "pending_ovs",
                                                   {"ЭКО": 1, "НОРМА": 2,
                                                    "МАКС": 4}[v]))
    qual.config(width=6, bg="#1e1e2a", fg=FG, activebackground="#2a2a3c",
                relief="flat", highlightthickness=0)
    b_quit = dbtn(buts2, "ВЫХОД", root.destroy, 7)
    for b in (b_ball, b_rec, qual, b_quit):
        b.pack(side="left", padx=2, pady=3)
    status = tk.Label(root, text="", anchor="w", bg=BG, fg="#9fdf9f")
    status.pack(fill="x")
    logbox = tk.Text(root, height=7, bg="#0d0d14", fg="#9fdf9f",
                     insertbackground=FG, relief="flat", font=("Consolas", 8))
    logbox.pack(fill="both", expand=True)
    def tick():
        try:
            drain = []
            for major, msg in STt.logq:
                if major or P["verb"]:
                    drain.append(f"[{datetime.datetime.now().strftime('%H:%M:%S')}] {msg}")
            STt.logq.clear()
            if drain:
                for L in drain:
                    logbox.insert("end", L + "\n")
                logbox.see("end")
                log_flush(drain)
            if STS and len(STS[0].nodes) >= 3:
                idx = min(len(STS[0].nodes) - 2,
                          int(P["beta"] * (len(STS[0].nodes) - 1)))
                bow_dot[0] = 1 if (STt.on > 0.01 and P["bow"]) else 0
                bow_dot[1] = STS[0].nodes[idx]
            else:
                bow_dot[0] = 0
            sel_ = min(SEL[0], max(0, len(STS) - 1)) if STS else 0
            cpu_pct = min(STt.cpu, 9.99) * 100.0
            status.config(text=f"CPU {cpu_pct:5.0f}%  сбои {STt.underr}  "
                               f"масс {N[0]}  пружин {NSPR[0]}  связей {NLNK[0]}  "
                               f"пинов {NPIN[0]}  выбрана стр.{sel_}  "
                               f"R={STt.R_now:.2f}"
                               + ("  ЗАПИСЬ..." if STt.rec else ""))
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def nearest_node_screen(e):
        best, bd = -1, 1e18
        for a in range(N[0]):
            x_, y_ = node_xy(a)
            d = (e.x - x_) ** 2 + (e.y - y_) ** 2
            if d < bd:
                bd, best = d, a
        return best, math.sqrt(bd)
    def pick(e):
        if e.state & 0x0004:
            bq, bd = -1, 1e18
            for q in range(NLNK[0]):
                mx = (X[LI[q]] + X[LJ[q]]) / 2
                my = (Y[LI[q]] + Y[LJ[q]]) / 2
                d = (e.x - mx * ZOOM) ** 2 + (e.y - (CANH - my * ZOOM)) ** 2
                if d < bd:
                    bd, bq = d, q
            bp, bdp = -1, 1e18
            for p in range(NPIN[0]):
                d = (e.x - PXP[p] * ZOOM) ** 2 + (e.y - (CANH - PYP[p] * ZOOM)) ** 2
                if d < bdp:
                    bdp, bp = d, p
            if bq >= 0 and (bdp < 0 or bd <= bdp):
                NLNK[0] -= 1
                for qq in range(bq, NLNK[0]):
                    LI[qq] = LI[qq + 1]; LJ[qq] = LJ[qq + 1]
                    LREST[qq] = LREST[qq + 1]; LTYPE[qq] = LTYPE[qq + 1]
                log("связь удалена")
            elif bp >= 0:
                NPIN[0] -= 1
                for qq in range(bp, NPIN[0]):
                    PNH[qq] = PNH[qq + 1]; PXP[qq] = PXP[qq + 1]
                    PYP[qq] = PYP[qq + 1]; PVX[qq] = PVX[qq + 1]
                    PVY[qq] = PVY[qq + 1]; PHX[qq] = PHX[qq + 1]
                    PHY[qq] = PHY[qq + 1]
                log("пин удалён")
            STt.lnk_start = -1
            return
        if e.state & 0x0001:
            a, d = nearest_node_screen(e)
            if a < 0 or d > 30:
                log("связь: кликни ближе к узлу", True)
                return
            if STt.lnk_start < 0:
                STt.lnk_start = a
                log(f"связь: от узла {a} (стр.{SIDM[a]}) — второй Shift+клик")
            elif STt.lnk_start != a and NLNK[0] < MAXLNK:
                i, j = STt.lnk_start, a
                LI[NLNK[0]] = i; LJ[NLNK[0]] = j
                LREST[NLNK[0]] = math.hypot(X[j] - X[i], Y[j] - Y[i])
                LTYPE[NLNK[0]] = STt.ltype
                log(f"СВЯЗЬ {'НИТКА' if STt.ltype else 'ПРУЖИНА'}: "
                    f"{i}(стр.{SIDM[i]}) ↔ {j}(стр.{SIDM[j]}), "
                    f"{LREST[NLNK[0]]*1e3:.0f} мм")
                NLNK[0] += 1
                STt.lnk_start = -1
            return
        if e.state & 0x20000:
            if NPIN[0] >= MAXPIN:
                log("лимит пинов", True)
                return
            p = NPIN[0]
            a, d = nearest_node_screen(e)
            if d > 40:
                a = -1
            PNH[p] = a
            wx, wy = to_world(e)
            PXP[p] = wx; PYP[p] = wy
            PVX[p] = 0.0; PVY[p] = 0.0
            PHX[p] = wx; PHY[p] = wy
            RB[N[0] + p] = P["pinr"] * 1e-3
            if a >= 0:
                log(f"ПИН #{p} → резонатор на узле {a} (стр.{SIDM[a]}), "
                    f"f0={P['pf0']:.0f} Q={P['pq']:.0f}")
            else:
                log(f"ПИН #{p} СВОБОДНЫЙ (мяч), грав={P['pg']:.2f}")
            NPIN[0] += 1
            return
        if STt.tool == "draw":
            wx, wy = to_world(e)
            STt.draw_pts = [(wx, wy)]
            return
        a, d = nearest_node_screen(e)
        if a >= 0 and d < 30:
            SEL[0] = SIDM[a]
            STt.grab = (a, X[a], Y[a])
            sp = STS[SEL[0]].sp
            P["t"], P["nl"], P["ca"], P["grav"] = \
                sp["t"], sp["nl"], sp["ca"], sp["grav"]
            for k2 in ("t", "nl", "ca", "grav"):
                mat_sliders[k2].set(P[k2])
            mat_apply()
            log(f"стр.{SEL[0]} выбрана: t={sp['t']:.2f} nl={sp['nl']:.2f} "
                f"ca={sp['ca']:.2f} grav={sp['grav']:.2f} (слайдеры синхронизированы)")
    def motion(e):
        if STt.tool == "draw" and STt.draw_pts is not None:
            wx, wy = to_world(e)
            if math.hypot(wx - STt.draw_pts[-1][0],
                          wy - STt.draw_pts[-1][1]) >= 7.0 / ZOOM:
                STt.draw_pts.append((wx, wy))
        elif STt.grab is not None and not (e.state & 0x0005):
            a = STt.grab[0]
            wx, wy = to_world(e)
            STt.grab = (a, wx, wy)
    def release(e):
        if STt.tool == "draw" and STt.draw_pts is not None:
            if len(STt.draw_pts) >= 2:
                draw_string(STt.draw_pts)
            STt.draw_pts = None
        if e.num == 1:
            STt.grab = None
        if e.num == 3:
            STt.plk = None
    def pluck(e):
        a, d = nearest_node_screen(e)
        if a >= 0 and d < 30:
            wx, wy = to_world(e)
            dx = wx - X[a]; dy = wy - Y[a]
            dl = math.hypot(dx, dy) + 1e-9
            STt.plk = (a, dx / dl, dy / dl, time.time())
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", motion)
    cv.bind("<ButtonRelease-1>", release)
    cv.bind("<Button-3>", pluck)
    cv.bind("<ButtonRelease-3>", release)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<r>", lambda e: do_reset())
    root.bind_all("<MouseWheel>", lambda e: P.__setitem__(
        "beta", max(0.02, min(0.5, P["beta"] + 0.01 * (1 if e.delta > 0 else -1)))))
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    log("v13.3: ИНСТРУМЕНТ→РИСОВАТЬ = рисуй струны; клик по струне = выбрать её;")
    log("слайдеры 'Материал' применяются к выбранной live; СЕТКА — размер сверху;")
    log("Shift+клик x2 = связь; Alt+клик = пин; МЯЧ = бросок; ПКМ = щипок.")
    stream = None
    try:
        stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                                 callback=audio_cb, latency="low")
        stream.start()
    except Exception as e:
        log(f"АУДИО НЕ ОТКРЫЛОСЬ: {e}", True)
    draw(); tick(); root.mainloop()
    if stream:
        stream.stop(); stream.close()

def main():
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    try:
        import sounddevice as sd
    except Exception:
        import traceback
        traceback.print_exc()
        print("Нет sounddevice: python -m pip install sounddevice")
        return
    if "diag" in [a.lower() for a in sys.argv[1:]]:
        print(sd.query_devices())
        print("default:", sd.default.device)
        return
    print("компиляция ядра (до ~20 c)...")
    t0 = time.time()
    draw_string([(0.1, 0.2), (0.8, 0.2)])
    process_block(64)
    N[0] = 0; NSPR[0] = 0; NLNK[0] = 0; NPIN[0] = 0
    STS.clear(); SEL[0] = 0
    STt.logq.clear()
    DCS[0] = 0.0; DCS[1] = 0.0
    print(f"ядро готово за {time.time()-t0:.1f} c")
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