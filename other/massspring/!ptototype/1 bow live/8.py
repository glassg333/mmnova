#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v18) — МАТЕРИАЛ «ЖЕЛЕЗО»: 3D-сетки масс
#  * Ячейка = 4 пружины по краям + 2 диагонали (СДВИГ) = держит форму,
#    трясётся как лист металла, ингармонично звенит
#  * Контакты с ГИСТЕРИЗИСОМ: chatter-клики исключены конструктивно
#  * Авто-подшаги: натяжение 0.2..12 — чем выше, тем больше подшагов,
#    КРИСТАЛЬНО ЧИСТО до высоких питчей без краша
#  * ЭНЕРГО-СТРАЖ + импульсные пределы: взрыв звука невозможен
#  * СЛОЙ Z: несколько листов на разных глубинах, сталкиваются
#  Режимы: СТРУНА+ (S, эталон), РИСОВАТЬ (по умолчанию):
#    замкнутая фигура = ЗАЛИВКА сеткой, открытая линия = полоса
#  ЛКМ рисовать/хват | ПКМ щипок | ПРОБЕЛ смычок (стр.0) | T тест
#  Shift связь | Alt пин | Ctrl удалить | r сброс | C очистить
# Зависимости: python -m pip install numpy sounddevice numba
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np
from numba import njit
import sounddevice as sd

SR, BLOCK = 44100, 512
ZOOM = 2000.0
CANH = 460
MU = 0.04
M0 = MU * 0.010
K_SCALE = 1.1e5
SHEAR = 0.45             # доля сдвиговой (диагональной) жёсткости
BEND = 0.25              # доля изгибной жёсткости (через ячейку)
LOSS_MIN = 0.0008        # минимальные потери (металл: Q высокий)
MAXSUB = 24
MAXM, MAXSPR, MAXLNK, MAXPIN, MAXPAIR = 2600, 9000, 48, 6, 12000
R_NODE = 0.0035
FR = 0.55
VMAX = 60.0
DVS, DVC, DVL, DVP, DVH = 1.2, 0.25, 0.50, 1.00, 0.05
PLUCK_AMT = 5.0e-3
PULL_RATE = 0.05
EGUARD = 3.0
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)

X = np.zeros(MAXM); Y = np.zeros(MAXM); Z = np.zeros(MAXM)
VX = np.zeros(MAXM); VY = np.zeros(MAXM); VZ = np.zeros(MAXM)
X0 = np.zeros(MAXM); Y0 = np.zeros(MAXM); Z0 = np.zeros(MAXM)
FIXM = np.zeros(MAXM, np.int64); SIDM = np.zeros(MAXM, np.int64)
RB = np.full(MAXM + MAXPIN, R_NODE)
SI = np.zeros(MAXSPR, np.int64); SJ = np.zeros(MAXSPR, np.int64)
SREST = np.zeros(MAXSPR); SSID = np.zeros(MAXSPR, np.int64)
SK = np.zeros(MAXSPR); SC = np.zeros(MAXSPR); SNL = np.zeros(MAXSPR)
SMODE = np.zeros(MAXSPR, np.int64)     # 1=edge 2=shear 3=bend
LI = np.zeros(MAXLNK, np.int64); LJ = np.zeros(MAXLNK, np.int64)
LREST = np.zeros(MAXLNK); LTYPE = np.zeros(MAXLNK, np.int64)
PNH = np.full(MAXPIN, -1, np.int64)
PXP = np.zeros(MAXPIN); PYP = np.zeros(MAXPIN)
PVX = np.zeros(MAXPIN); PVY = np.zeros(MAXPIN)
PZP = np.zeros(MAXPIN)
PHX = np.zeros(MAXPIN); PHY = np.zeros(MAXPIN); PHZ = np.zeros(MAXPIN)
PI = np.zeros(MAXPAIR, np.int64); PJ = np.zeros(MAXPAIR, np.int64)
MN = np.zeros(24, np.int64); MNX = np.zeros(24); MNY = np.zeros(24)
MNZ = np.zeros(24); MW = np.zeros(24)
GM = np.zeros(MAXM); CAMM = np.zeros(MAXM)
STATS = np.zeros(4)
DCS = np.zeros(2)

@njit(cache=True, fastmath=True)
def broadphase3d(Xa, Ya, Za, n, px, py, pz, npin, sidm, fixm, rb,
                 pi_, pj_, maxpair):
    cnt = 0
    B = n + npin
    for a in range(B):
        if a < n:
            ax = Xa[a]; ay = Ya[a]; az = Za[a]
            asid = sidm[a]; afix = fixm[a]; ar = rb[a]
        else:
            ax = px[a - n]; ay = py[a - n]; az = pz[a - n]
            asid = -1; afix = 0; ar = rb[a]
        for b in range(a + 1, B):
            if b < n:
                if asid >= 0 and sidm[b] == asid and SM_NEAR(asid, sidm[b]):
                    continue
                if afix == 1 and fixm[b] == 1:
                    continue
                bx = Xa[b]; by = Ya[b]; bz = Za[b]
            else:
                bx = px[b - n]; by = py[b - n]; bz = pz[b - n]
            dx = bx - ax; dy = by - ay; dz = bz - az
            rr = ar + rb[b]
            if dx*dx + dy*dy + dz*dz < rr*rr:
                if cnt < maxpair:
                    pi_[cnt] = a; pj_[cnt] = b; cnt += 1
    return cnt

@njit(cache=True)
def SM_NEAR(a, b):
    return False

@njit(cache=True, fastmath=True)
def core3d(X, Y, Z, VX, VY, VZ, FIXM, n,
           SI, SJ, SREST, SK, SC, SNL, SMODE, nspr,
           LI, LJ, LREST, LTYPE, nlnk, kl, cll,
           PI, PJ, npair, RB,
           PXP, PYP, PZP, PVX2, PVY2, PVZ2, PNH, mpin, npin,
           pk_att, pk_c, gpin,
           GM, CAMM,
           grab_i, gtx, gty, gtz, dvh,
           bow_on, jb, bnx, bny, vb, fn0, vf, fr, nzamp, nzarr, aLP,
           dvs, dvc, dvl, dvp, vmax, dt, im,
           MN, MNX, MNY, MNZ, MW, nmic,
           frames, ovs, out, stats, dcs):
    dcx = dcs[0]; dcy = dcs[1]; nzlp = 0.0
    clips = 0; vclips = 0; ncon = 0
    im_pin = dt / mpin
    mrat = mpin / M0
    for f in range(frames):
        for o in range(ovs):
            for q in range(nspr):
                i = SI[q]; j = SJ[q]
                dx = X[j] - X[i]; dy = Y[j] - Y[i]; dz = Z[j] - Z[i]
                d = math.sqrt(dx*dx + dy*dy + dz*dz) + 1e-12
                e = d - SREST[q]
                strain = e / SREST[q]
                F = SK[q] * e
                if SMODE[q] == 1:
                    F *= (1.0 + SNL[q] * strain * strain)
                nx = dx / d; ny = dy / d; nz2 = dz / d
                rel = ((VX[j]-VX[i])*nx + (VY[j]-VY[i])*ny + (VZ[j]-VZ[i])*nz2)
                F += SC[q] * rel
                dv = F * im
                if dv > dvs:
                    dv = dvs; clips += 1
                elif dv < -dvs:
                    dv = -dvs; clips += 1
                if FIXM[i] == 0:
                    VX[i] += dv*nx; VY[i] += dv*ny; VZ[i] += dv*nz2
                if FIXM[j] == 0:
                    VX[j] -= dv*nx; VY[j] -= dv*ny; VZ[j] -= dv*nz2
            for a in range(n):
                if FIXM[a] == 0:
                    VY[a] += GM[a] * dt
                    dmp = 1.0 - CAMM[a] * dt
                    if dmp < 0.0:
                        dmp = 0.0
                    VX[a] *= dmp; VY[a] *= dmp; VZ[a] *= dmp
            for q in range(npair):
                a = PI[q]; b = PJ[q]
                if a < n:
                    ax = X[a]; ay = Y[a]; az = Z[a]
                    avx = VX[a]; avy = VY[a]; avz = VZ[a]; af = FIXM[a]
                    ra = RB[a]
                else:
                    ax = PXP[a-n]; ay = PYP[a-n]; az = PZP[a-n]
                    avx = PVX2[a-n]; avy = PVY2[a-n]; avz = PVZ2[a-n]
                    af = 0; ra = RB[a]
                if b < n:
                    bx = X[b]; by = Y[b]; bz = Z[b]
                    bvx = VX[b]; bvy = VY[b]; bvz = VZ[b]; bf = FIXM[b]
                    rb_ = RB[b]
                else:
                    bx = PXP[b-n]; by = PYP[b-n]; bz = PZP[b-n]
                    bvx = PVX2[b-n]; bvy = PVY2[b-n]; bvz = PVZ2[b-n]
                    bf = 0; rb_ = RB[b]
                if af == 1 and bf == 1:
                    continue
                dx = bx - ax; dy = by - ay; dz = bz - az
                d = math.sqrt(dx*dx + dy*dy + dz*dz) + 1e-12
                pen = ra + rb_ - d
                if pen <= 0.0:
                    continue
                nx = dx/d; ny = dy/d; nz2 = dz/d
                closing = -((bvx-avx)*nx + (bvy-avy)*ny + (bvz-avz)*nz2)
                dmp = 1.0 + 0.6*closing
                if dmp < 0.0:
                    dmp = 0.0
                elif dmp > 2.0:
                    dmp = 2.0
                F = 8.0e5 * pen**1.5 * dmp
                dva = F * im
                if dva > dvc:
                    dva = dvc; clips += 1
                elif dva < -dvc:
                    dva = -dvc; clips += 1
                if a < n:
                    if af == 0:
                        VX[a] -= dva*nx; VY[a] -= dva*ny; VZ[a] -= dva*nz2
                else:
                    PVX2[a-n] -= dva*nx; PVY2[a-n] -= dva*ny; PVZ2[a-n] -= dva*nz2
                if b < n:
                    if bf == 0:
                        VX[b] += dva*nx; VY[b] += dva*ny; VZ[b] += dva*nz2
                else:
                    PVX2[b-n] += dva*nx; PVY2[b-n] += dva*ny; PVZ2[b-n] += dva*nz2
                ncon += 1
            for q in range(nlnk):
                i = LI[q]; j = LJ[q]
                dx = X[j]-X[i]; dy = Y[j]-Y[i]; dz = Z[j]-Z[i]
                d = math.sqrt(dx*dx+dy*dy+dz*dz) + 1e-12
                e = d - LREST[q]
                if LTYPE[q] == 1 and e <= 0.0:
                    continue
                nx = dx/d; ny = dy/d; nz2 = dz/d
                rel = ((VX[j]-VX[i])*nx + (VY[j]-VY[i])*ny + (VZ[j]-VZ[i])*nz2)
                F = kl*e + cll*rel
                dv = F * im
                if dv > dvl:
                    dv = dvl; clips += 1
                elif dv < -dvl:
                    dv = -dvl; clips += 1
                if FIXM[i] == 0:
                    VX[i] += dv*nx; VY[i] += dv*ny; VZ[i] += dv*nz2
                if FIXM[j] == 0:
                    VX[j] -= dv*nx; VY[j] -= dv*ny; VZ[j] -= dv*nz2
            for p in range(npin):
                h = PNH[p]
                if h < 0:
                    PVY2[p] -= gpin * dt
                    if PYP[p] < 0.006:
                        PYP[p] = 0.006
                        if PVY2[p] < 0.0:
                            PVY2[p] = -0.5*PVY2[p]
                    elif PYP[p] > 0.45:
                        PYP[p] = 0.45
                        if PVY2[p] > 0.0:
                            PVY2[p] = -0.5*PVY2[p]
                    if PXP[p] < 0.01:
                        PXP[p] = 0.01
                        if PVX2[p] < 0.0:
                            PVX2[p] = -0.5*PVX2[p]
                    elif PXP[p] > 0.99:
                        PXP[p] = 0.99
                        if PVX2[p] > 0.0:
                            PVX2[p] = -0.5*PVX2[p]
                    if PZP[p] < -0.05:
                        PZP[p] = -0.05
                        if PVZ2[p] < 0.0:
                            PVZ2[p] = -0.5*PVZ2[p]
                    elif PZP[p] > 0.05:
                        PZP[p] = 0.05
                        if PVZ2[p] > 0.0:
                            PVZ2[p] = -0.5*PVZ2[p]
                else:
                    dx = X[h]-PXP[p]; dy = Y[h]-PYP[p]; dz = Z[h]-PZP[p]
                    Fx = pk_att*dx + pk_c*(VX[h]-PVX2[p])
                    Fy = pk_att*dy + pk_c*(VY[h]-PVY2[p])
                    Fz = pk_att*dz + pk_c*(VZ[h]-PVZ2[p])
                    dvx = Fx*im_pin; dvy = Fy*im_pin; dvz = Fz*im_pin
                    if dvx > dvp: dvx = dvp; clips += 1
                    elif dvx < -dvp: dvx = -dvp; clips += 1
                    if dvy > dvp: dvy = dvp; clips += 1
                    elif dvy < -dvp: dvy = -dvp; clips += 1
                    if dvz > dvp: dvz = dvp; clips += 1
                    elif dvz < -dvp: dvz = -dvp; clips += 1
                    PVX2[p] += dvx; PVY2[p] += dvy; PVZ2[p] += dvz
                    rx = -dvx*mrat; ry = -dvy*mrat; rz = -dvz*mrat
                    if rx > dvc: rx = dvc
                    elif rx < -dvc: rx = -dvc
                    if ry > dvc: ry = dvc
                    elif ry < -dvc: ry = -dvc
                    if rz > dvc: rz = dvc
                    elif rz < -dvc: rz = -dvc
                    if FIXM[h] == 0:
                        VX[h] += rx; VY[h] += ry; VZ[h] += rz
            if grab_i >= 0:
                dx = gtx-X[grab_i]; dy = gty-Y[grab_i]; dz = gtz-Z[grab_i]
                Fx = 250.0*dx - 3.0*VX[grab_i]
                Fy = 250.0*dy - 3.0*VY[grab_i]
                Fz = 250.0*dz - 3.0*VZ[grab_i]
                dvx = Fx*im; dvy = Fy*im; dvz = Fz*im
                if dvx > dvh: dvx = dvh
                elif dvx < -dvh: dvx = -dvh
                if dvy > dvh: dvy = dvh
                elif dvy < -dvh: dvy = -dvh
                if dvz > dvh: dvz = dvh
                elif dvz < -dvh: dvz = -dvh
                VX[grab_i] += dvx; VY[grab_i] += dvy; VZ[grab_i] += dvz
            if bow_on > 0.0 and fn0 > 0.0:
                nzl = nzlp + aLP*(nzarr[f*ovs+o] - nzlp)
                nzlp = nzl
                fn = fn0*(1.0 + 0.5*nzamp*nzl)
                if fn < 0.0:
                    fn = 0.0
                VX[jb] += nzamp*nzl*fn*0.5*im*bnx
                VY[jb] += nzamp*nzl*fn*0.5*im*bny
                vn = VX[jb]*bnx + VY[jb]*bny
                lo = vn - fn*im; hi = vn + fn*im
                for _ in range(40):
                    mid = 0.5*(lo+hi)
                    u = vb - mid
                    mu = fr + (1.0-fr)*math.exp(-abs(u)/vf)
                    if mid - vn - im*fn*mu < 0.0:
                        lo = mid
                    else:
                        hi = mid
                vnn = 0.5*(lo+hi)
                dvb = vnn - vn
                VX[jb] += dvb*bnx; VY[jb] += dvb*bny
            for a in range(n):
                if FIXM[a] == 0:
                    X[a] += VX[a]*dt; Y[a] += VY[a]*dt; Z[a] += VZ[a]*dt
            for p in range(npin):
                PXP[p] += PVX2[p]*dt; PYP[p] += PVY2[p]*dt; PZP[p] += PVZ2[p]*dt
            for a in range(n + npin):
                if a < n:
                    if FIXM[a] == 0:
                        v2 = VX[a]*VX[a] + VY[a]*VY[a] + VZ[a]*VZ[a]
                        if v2 > vmax*vmax:
                            kk = vmax/math.sqrt(v2)
                            VX[a] *= kk; VY[a] *= kk; VZ[a] *= kk
                            vclips += 1
                else:
                    p = a - n
                    v2 = PVX2[p]*PVX2[p] + PVY2[p]*PVY2[p] + PVZ2[p]*PVZ2[p]
                    if v2 > vmax*vmax:
                        kk = vmax/math.sqrt(v2)
                        PVX2[p] *= kk; PVY2[p] *= kk; PVZ2[p] *= kk
                        vclips += 1
        xs = 0.0
        for m in range(nmic):
            a = MN[m]
            if a < n:
                xs += MW[m]*(VX[a]*MNX[m] + VY[a]*MNY[m] + VZ[a]*MNZ[m])
            else:
                p2 = a - n
                if p2 < npin:
                    xs += MW[m]*(PVX2[p2]*MNX[m] + PVY2[p2]*MNY[m] + PVZ2[p2]*MNZ[m])
        x = xs - dcx + dcs[1]*dcy
        dcx = xs; dcy = x
        out[f] = x
    stats[0] = clips; stats[1] = vclips; stats[2] = ncon; stats[3] = 0.0
    dcs[0] = dcx; dcs[1] = dcy

# ---------------- bookkeeping ----------------
class Str:
    pass

STS = []
N = [0]; NSPR = [0]; NLNK = [0]; NPIN = [0]
ZLAYER = [0.0]
P = dict(R=0.0, speed=1.0, press=1.0, beta=0.10, noise=0.25,
         t=1.0, nl=0.06, ca=0.002, grav=0.0, shear=0.45, bend=0.25,
         pf0=300.0, pq=8.0, pump=0.0, pm=20.0, pg=0.6,
         linkk=20000.0, pinr=6.0, vol=0.35,
         bow=False, automorph=False, protect=False, verb=True)
SEL = [0]
STt = type("ST", (), {})()
STt.ovs = 2; STt.t = 0.0; STt.on = 0.0; STt.R_now = 0.0
STt.tool = "draw"; STt.ltype = 0; STt.lnk_start = -1
STt.grab = None; STt.plk = None; STt.rec = False; STt.recl = []
STt.pending_ovs = None; STt.level = 0.0; STt.gain = 0.0
STt.cpu = 0.0; STt.underr = 0; STt.nmic = 0; STt.err_n = 0
STt.guard_last = 0.0
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

def auto_ovs(t):
    return int(min(MAXSUB, max(2, round(2.0 * math.sqrt(t)))))

def reset_positions():
    for s in STS:
        nds = s.nodes
        if len(nds) < 2:
            continue
        for a in nds:
            X[a] = X0[a]; Y[a] = Y0[a]; Z[a] = Z0[a]
            VX[a] = 0.0; VY[a] = 0.0; VZ[a] = 0.0
    for p in range(NPIN[0]):
        PXP[p] = PHX[p]; PYP[p] = PHY[p]; PZP[p] = PHZ[p]
        PVX[p] = 0.0; PVY[p] = 0.0; PZP[p] = 0.0
    DCS[0] = 0.0; DCS[1] = 0.0

def spawn_ball():
    if NPIN[0] >= MAXPIN:
        log("лимит пинов", True); return
    p = NPIN[0]
    PNH[p] = -1
    PXP[p] = 0.40 + 0.2*float(RNG.random())
    PYP[p] = 0.36
    PZP[p] = ZLAYER[0]
    PVX[p] = (float(RNG.random()) - 0.5) * 0.8
    PVY[p] = 0.3
    PHX[p] = PXP[p]; PHY[p] = PYP[p]; PHZ[p] = PZP[p]
    RB[N[0] + p] = P["pinr"] * 1e-3
    NPIN[0] += 1
    log(f"МЯЧ #{p} (z={ZLAYER[0]:+.2f})", True)

def add_node(x, y, z, sid, fix):
    a = N[0]
    if a >= MAXM:
        return -1
    X[a] = x; Y[a] = y; Z[a] = z
    X0[a] = x; Y0[a] = y; Z0[a] = z
    VX[a] = 0.0; VY[a] = 0.0; VZ[a] = 0.0
    FIXM[a] = fix; SIDM[a] = sid; RB[a] = R_NODE
    N[0] = a + 1
    return a

def add_spring(i, j, sid, mode=1, mult=1.0, rest=None):
    q = NSPR[0]
    if q >= MAXSPR or i < 0 or j < 0 or i == j:
        return -1
    r = rest if rest is not None else math.sqrt(
        (X[j]-X[i])**2 + (Y[j]-Y[i])**2 + (Z[j]-Z[i])**2)
    if r < 5.0e-4:
        return -1
    SREST[q] = r
    SI[q] = i; SJ[q] = j; SSID[q] = sid; SMODE[q] = mode
    NSPR[0] = q + 1
    return q

def build_sheet(sid, pts2, sp_, closed):
    """полоса/заливка из точек: строим сетку-железо вдоль пути"""
    # создаём ленту шириной 2 узла вдоль пути (для замкнутой — по контуру
    # и центру), открытая линия = лента в 2 ряда
    nseg = len(pts2) - 1
    if nseg < 1:
        return None
    w = 0.022
    # нормаль первого сегмента в плоскости XY
    dx = pts2[1][0] - pts2[0][0]; dy = pts2[1][1] - pts2[0][1]
    L = math.hypot(dx, dy) + 1e-12
    nx_, ny_ = -dy / L, dx / L
    rowA = []; rowB = []
    a = add_node(pts2[0][0] + nx_*w, pts2[0][1] + ny_*w, ZLAYER[0], sid, 0)
    b = add_node(pts2[0][0] - nx_*w, pts2[0][1] - ny_*w, ZLAYER[0], sid, 0)
    if a < 0 or b < 0:
        return None
    rowA.append(a); rowB.append(b)
    if closed:
        FIXM[a] = 1; FIXM[b] = 1
    add_spring(a, b, sid, 1)
    for k in range(1, len(pts2)):
        dx = pts2[k][0] - pts2[k-1][0]; dy = pts2[k][1] - pts2[k-1][1]
        L = math.hypot(dx, dy) + 1e-12
        nx_ = -dy / L; ny_ = dx / L
        a2 = add_node(pts2[k][0] + nx_*w, pts2[k][1] + ny_*w, ZLAYER[0], sid,
                      1 if (closed and k == len(pts2)-1) else 0)
        b2 = add_node(pts2[k][0] - nx_*w, pts2[k][1] - ny_*w, ZLAYER[0], sid,
                      1 if (closed and k == len(pts2)-1) else 0)
        if a2 < 0 or b2 < 0:
            break
        rowA.append(a2); rowB.append(b2)
        add_spring(rowA[-2], a2, sid, 1)
        add_spring(rowB[-2], b2, sid, 1)
        add_spring(a2, b2, sid, 1)
        add_spring(rowA[-2], rowB[-2], sid, 1)     # поперечина
        add_spring(rowA[-2], b2, sid, 2)           # диагональ (сдвиг)
        add_spring(rowB[-2], a2, sid, 2)
        if closed and k == len(pts2) - 1:
            add_spring(a2, rowA[0], sid, 1)
            add_spring(b2, rowB[0], sid, 1)
            add_spring(a2, rowB[0], sid, 2)
            add_spring(b2, rowA[0], sid, 2)
    nodes = rowA + rowB
    return nodes, len(rowA)

def draw_material(pts):
    closed = False
    if len(pts) >= 3:
        d = math.hypot(pts[0][0] - pts[-1][0], pts[0][1] - pts[-1][1])
        if d < 0.04:
            closed = True
            pts = pts[:-1]
    total = 0.0
    for i in range(1, len(pts)):
        total += math.hypot(pts[i][0] - pts[i-1][0], pts[i][1] - pts[i-1][1])
    if total < 0.06:
        log(f"линия слишком короткая ({total*100:.0f} см)", True); return
    nseg = max(6, min(24, int(round(total / 0.012))))
    if N[0] + 2 * (nseg + 2) >= MAXM:
        log("лимит масс — C очистить", True); return
    # равномерная выборка по пути
    Ls = [0.0]
    for i in range(1, len(pts)):
        Ls.append(Ls[-1] + math.hypot(pts[i][0]-pts[i-1][0],
                                      pts[i][1]-pts[i-1][1]))
    tot = Ls[-1]
    pts2 = [pts[0]]
    j = 0
    for k in range(1, nseg + 1):
        dd = tot * k / nseg
        while j < len(pts) - 2 and Ls[j+1] < dd:
            j += 1
        seg = Ls[j+1] - Ls[j]
        w = (dd - Ls[j]) / seg if seg > 1e-12 else 0.0
        pts2.append((pts[j][0] + (pts[j+1][0]-pts[j][0])*w,
                     pts[j][1] + (pts[j+1][1]-pts[j][1])*w))
    sid = len(STS)
    s = Str()
    s.sp = dict(t=P["t"], nl=P["nl"], ca=max(P["ca"], LOSS_MIN),
                grav=P["grav"], shear=P["shear"], bend=P["bend"])
    res = build_sheet(sid, pts2, s.sp, closed)
    if res is None:
        return
    nodes, nrow = res
    s.nodes = nodes
    STS.append(s)
    SEL[0] = sid
    kind = "ЗАЛИВКА (замкнуто)" if closed else "ПОЛОСА"
    log(f"{kind} #{sid}: {nrow}x2 узлов, сдвиг+кромки, z={ZLAYER[0]:+.2f}, "
        f"t={s.sp['t']:.2f} — оверсэмплинг авто={auto_ovs(s.sp['t'])}x", True)

def spawn_string():
    y = 0.10
    while y < 0.42:
        ok = True
        for a in range(N[0]):
            if abs(Y[a] - y) < 0.035 and 0.10 < X[a] < 0.90:
                ok = False; break
        if ok:
            break
        y += 0.05
    nseg = 24
    if N[0] + nseg + 2 >= MAXM:
        log("лимит масс", True); return
    pts2 = [(0.15 + i * 0.70 / nseg, y) for i in range(nseg + 1)]
    sid = len(STS)
    s = Str()
    s.sp = dict(t=1.0, nl=0.04, ca=0.02, grav=0.0, shear=0.0, bend=0.0)
    s.nodes = []
    a0 = add_node(pts2[0][0], pts2[0][1], ZLAYER[0], sid, 1)
    s.nodes.append(a0)
    for k in range(1, len(pts2)):
        a = add_node(pts2[k][0], pts2[k][1], ZLAYER[0], sid,
                     1 if k == len(pts2) - 1 else 0)
        add_spring(s.nodes[-1], a, sid, 1)
        s.nodes.append(a)
    STS.append(s)
    SEL[0] = sid
    log(f"СТРУНА+ #{sid}: {len(s.nodes)} узлов, f0≈{f0_est(sid):.0f} Гц", True)

def f0_est(sid):
    s = STS[sid]
    nseg = len(s.nodes) - 1
    if nseg < 2:
        return 0.0
    return math.sqrt(K_SCALE * s.sp["t"] / M0) / (2.0 * nseg)

def add_mesh():
    cols, rows = 16, 10
    sp = 0.014
    if N[0] + cols * rows >= MAXM:
        log("лимит масс", True); return
    if N[0] > 0:
        maxx = 0.0
        for a in range(N[0]):
            if X[a] > maxx: maxx = X[a]
        x00 = min(maxx + 0.05, 0.90 - (cols-1)*sp)
        x00 = max(x00, 0.02)
        y00 = 0.10
    else:
        x00, y00 = 0.30, 0.10
    src = dict(t=P["t"], nl=P["nl"], ca=max(P["ca"], LOSS_MIN),
               grav=P["grav"], shear=P["shear"], bend=P["bend"])
    sid = len(STS)
    s = Str()
    s.sp = src
    s.nodes = []
    grid = []
    for r in range(rows):
        row = []
        for c in range(cols):
            fix = 1 if (r == 0 or r == rows-1 or c == 0 or c == cols-1) else 0
            a = add_node(x00 + c*sp, y00 + r*sp, ZLAYER[0], sid, fix)
            if a < 0:
                break
            row.append(a); s.nodes.append(a)
        grid.append(row)
    diag = P["shear"]
    for r in range(rows):
        for c in range(cols):
            if c + 1 < cols:
                add_spring(grid[r][c], grid[r][c+1], sid, 1)
            if r + 1 < rows:
                add_spring(grid[r][c], grid[r+1][c], sid, 1)
    for r in range(rows - 1):
        for c in range(cols - 1):
            add_spring(grid[r][c], grid[r+1][c+1], sid, 2, mult=diag)
            add_spring(grid[r][c+1], grid[r+1][c], sid, 2, mult=diag)
    if len(s.nodes) < 4:
        return
    STS.append(s)
    SEL[0] = sid
    log(f"ПЛАСТИНА {cols}x{rows} ЖЕЛЕЗО @({x00:.2f},{y00:.2f}) z={ZLAYER[0]:+.2f} "
        f"t={src['t']:.2f} сдвиг={diag:.2f} — щипай ПКМ, трясти ЛКМ", True)

def to_world(e):
    return (e.x / ZOOM, (CANH - e.y) / ZOOM)

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

def process_block(frames):
    ovs = auto_ovs(P["t"]) if STS else 2
    if ovs != STt.ovs:
        STt.ovs = ovs
        log(f"оверсэмплинг авто: {ovs}x (t={P['t']:.2f})", False)
    dt = 1.0 / (SR * ovs); im = dt / M0
    n = N[0]; nspr = NSPR[0]
    for q in range(nspr):
        sp = STS[SSID[q]].sp
        base = K_SCALE * sp["t"]
        mode = SMODE[q]
        if mode == 1:
            k = base
            SNL[q] = sp["nl"]
        elif mode == 2:
            k = base * sp["shear"] * 0.7071
            SNL[q] = sp["nl"] * 2.0
        else:
            k = base * sp["bend"]
            SNL[q] = 0.0
        SK[q] = k
        SC[q] = 0.5e-4 * math.sqrt(max(k, 1.0) * M0)
    for a in range(n):
        sp = STS[SIDM[a]].sp
        CAMM[a] = sp["ca"]
        GM[a] = -sp["grav"] * 9.8
    npin = NPIN[0]
    mpin = P["pm"] * M0
    w = 2.0 * math.pi * P["pf0"]
    pk_att = w * w * mpin
    pk_c = w * mpin / max(P["pq"], 0.5) * (1.0 - P["pump"])
    for p in range(npin):
        RB[n + p] = P["pinr"] * 1e-3
    npair = 0
    if n + npin > 1:
        npair = broadphase3d(X[:n], Y[:n], Z[:n], n, PXP, PYP, PZP, npin,
                             SIDM, FIXM, RB, PI, PJ, MAXPAIR)
    nmic = 0
    for s in STS:
        if len(s.nodes) >= 3:
            k_ = len(s.nodes) // 2
            mid = s.nodes[k_]
            if nmic < 24:
                MN[nmic] = mid
                MNX[nmic] = 0.0; MNY[nmic] = 0.0; MNZ[nmic] = 1.0
                MW[nmic] = 1.0; nmic += 1
    for p in range(npin):
        if nmic < 24:
            MN[nmic] = n + p; MNX[nmic] = 0.0; MNY[nmic] = 0.0; MNZ[nmic] = 1.0
            MW[nmic] = 0.6; nmic += 1
    STt.nmic = nmic
    grab_i, gtx, gty, gtz = -1, 0.0, 0.0, 0.0
    if STt.grab is not None:
        grab_i, gtx, gty, gtz = STt.grab
    if STt.plk is not None:
        gi, dxn, dyn, t0 = STt.plk
        amt = min(PLUCK_AMT, PULL_RATE * (time.time() - t0))
        grab_i = gi
        gtx = X0[gi] + dxn * amt
        gty = Y0[gi] + dyn * amt
        gtz = Z0[gi]
    bow_on = 0.0; jb = 0; bnx = 0.0; bny = 1.0
    z0s = math.sqrt(K_SCALE * M0)
    if STS and len(STS[0].nodes) >= 3:
        nds = STS[0].nodes
        idx = min(len(nds) - 2, int(P["beta"] * (len(nds) - 1)))
        jb = nds[idx]
        a1 = nds[max(0, idx-1)]; a2 = nds[min(len(nds)-1, idx+1)]
        tx = X[a2]-X[a1]; ty = Y[a2]-Y[a1]
        tl = math.hypot(tx, ty)
        if tl > 1e-9:
            bnx = -ty/tl; bny = tx/tl; bow_on = 1.0
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
    core3d(X, Y, Z, VX, VY, VZ, FIXM, n,
           SI, SJ, SREST, SK, SC, SNL, SMODE, nspr,
           LI, LJ, LREST, LTYPE, NLNK[0], P["linkk"],
           0.15 * math.sqrt(P["linkk"] * M0),
           PI, PJ, npair, RB,
           PXP, PYP, PZP, PVX + MAXM - n, PVY + MAXM - n, PZP + MAXM - n,
           PNH, mpin, npin, pk_att, pk_c,
           P["pg"] * 9.8,
           GM, CAMM,
           grab_i, gtx, gty, gtz, DVH,
           bow_on, jb, bnx, bny, vb, fn0, vf, FR, P["noise"], nzarr, aLP,
           DVS, DVC, DVL, DVP, VMAX, dt, im,
           MN, MNX, MNY, MNZ, MW, nmic,
           frames, ovs, out, STATS, DCS)
    STt.cpu = 0.9 * STt.cpu + 0.1 * (time.perf_counter() - t0c) / (frames / SR)
    if n > 0:
        ekin = float(np.mean(VX[:n]*VX[:n] + VY[:n]*VY[:n] + VZ[:n]*VZ[:n]))
        if ekin > EGUARD:
            VX[:n] *= 0.5; VY[:n] *= 0.5; VZ[:n] *= 0.5
            if time.time() - STt.guard_last > 1.0:
                log(f"ЭНЕРГО-СТРАЖ: v_rms={math.sqrt(ekin):.1f} -> гашение", True)
                STt.guard_last = time.time()
    if not (np.isfinite(X[:n]).all() and np.isfinite(VX[:n]).all()):
        reset_positions()
        out[:] = 0.0
        log("АВАРИЯ (NaN) -> сброс", True)
    if STATS[1] > 10 and P["verb"]:
        log(f"клипы: сил {int(STATS[0])}/блок, скор {int(STATS[1])}", True)
    y = np.empty(frames)
    dcx, dcy = DCS[0], DCS[1]
    for i in range(frames):
        x = out[i]
        yi = x - dcx + DCA * dcy
        dcx = x; dcy = yi
        y[i] = yi
    DCS[0] = dcx; DCS[1] = dcy
    if P["protect"]:
        rms = float(np.sqrt(np.mean(y*y))) + 1e-12
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
        STt.err_n += 1
        if STt.err_n <= 3:
            import traceback
            traceback.print_exc()
        outdata[:] = 0

# ---------------- GUI ----------------
BG, FG = "#14141c", "#e0e0e8"

def run_gui():
    import tkinter as tk
    root = tk.Tk()
    root.title("physmod v18 — ЖЕЛЕЗО: материалы из сеток")
    root.configure(bg=BG)
    W = 1000
    cv = tk.Canvas(root, width=W, height=CANH, bg="#0d0d14",
                   highlightthickness=1, highlightbackground="#26263a")
    cv.pack(fill="x")
    bow_dot = [0, 0]
    def node_xy(a):
        px_ = X[a] * ZOOM + Z[a] * ZOOM * 0.35
        py_ = CANH - Y[a] * ZOOM + Z[a] * ZOOM * 0.20
        return (px_, py_)
    def draw():
        try:
            cv.delete("all")
            cv.create_text(10, 12, text="РИСОВАТЬ: замкнуто=ЗАЛИВКА сеткой, "
                           "открыто=ПОЛОСА | ЛКМ хват | ПКМ щипок | T тест | "
                           "S струна | C очистить | ПРОБЕЛ смычок",
                           fill="#666680", anchor="w", font=("TkDefaultFont", 9))
            order = sorted(range(len(STS)),
                           key=lambda si: (Z[STS[si].nodes[0]] if STS[si].nodes
                                           else 0))
            for si in order:
                s = STS[si]
                col = ("#4cc2a0" if si == SEL[0]
                       else ("#9aa8d8" if s.sp["t"] >= 1.0 else "#c8b8d8"))
                pts = []
                for a in s.nodes:
                    x_, y_ = node_xy(a)
                    pts += [x_, y_]
                if len(pts) >= 4:
                    cv.create_line(*pts, fill=col,
                                   width=2 if si == SEL[0] else 1)
                for a in s.nodes:
                    x_, y_ = node_xy(a)
                    if FIXM[a] == 1:
                        cv.create_rectangle(x_-3, y_-3, x_+3, y_+3,
                                            fill="#8890a8", width=0)
                    else:
                        cv.create_oval(x_-2, y_-2, x_+2, y_+2, fill=col, width=0)
            n = N[0]
            for p in range(NPIN[0]):
                px_ = PXP[p] * ZOOM + PZP[p] * ZOOM * 0.35
                py_ = CANH - PYP[p] * ZOOM + PZP[p] * ZOOM * 0.20
                r_ = max(4, int(RB[n + p] * ZOOM))
                cv.create_oval(px_-r_, py_-r_, px_+r_, py_+r_,
                               fill="#40b0ff", width=0)
            if bow_dot[0]:
                x_, y_ = node_xy(bow_dot[1])
                cv.create_oval(x_-5, y_-5, x_+5, y_+5, fill="#ff5040", width=0)
            if STt.draw_pts:
                for i2 in range(1, len(STt.draw_pts)):
                    x1_, y1_ = STt.draw_pts[i2-1]
                    x2_, y2_ = STt.draw_pts[i2]
                    cv.create_line(x1_*ZOOM, CANH - y1_*ZOOM,
                                   x2_*ZOOM, CANH - y2_*ZOOM,
                                   fill="#ffffff", width=1)
            lv = min(1.0, STt.level * 4)
            cv.create_rectangle(10, CANH-14, 10 + int((W-20)*lv), CANH-4,
                                fill="#40c080", width=0)
        except Exception as e:
            log(f"draw: {e}", False)
        root.after(40, draw)
    panel = tk.Frame(root, bg=BG); panel.pack(fill="x")
    mat_sliders = {}
    def mat_apply():
        for s in STS:
            s.sp.update(t=P["t"], nl=P["nl"], ca=max(P["ca"], LOSS_MIN),
                        grav=P["grav"], shear=P["shear"], bend=P["bend"])
    def mk(row, col, lab, key, a, b, res=0.005):
        tk.Label(panel, text=lab, width=18, anchor="w", bg=BG, fg=FG
                 ).grid(row=row, column=col)
        sc = tk.Scale(panel, from_=a, to=b, resolution=res, orient="horizontal",
                      length=140, showvalue=1, bg=BG, fg=FG,
                      troughcolor="#22222e", highlightthickness=0,
                      activebackground="#33334a",
                      command=(lambda v, k=key: (P.__setitem__(k, float(v)),
                                                 mat_apply())))
        sc.set(P[key])
        sc.grid(row=row, column=col + 1, sticky="w")
        return sc
    mk(0, 0, "R скрежет<->бас", "R", 0.0, 1.0)
    mk(1, 0, "Скорость смычка", "speed", 0.0, 1.6)
    mk(2, 0, "Прижим", "press", 0.0, 2.0)
    mk(3, 0, "Позиция смычка", "beta", 0.02, 0.5)
    mk(4, 0, "Шум волоса", "noise", 0.0, 1.0)
    mk(0, 2, "ЖЕЛЕЗО: натяжение", "t", 0.2, 12.0, res=0.01)
    mk(1, 2, "ЖЕЛЕЗО: нелин. (шорох)", "nl", 0.0, 0.5)
    mk(2, 2, "ЖЕЛЕЗО: потери (Q)", "ca", 0.0008, 0.12)
    mk(3, 2, "ЖЕЛЕЗО: гравитация", "grav", 0.0, 1.0)
    mk(4, 2, "Сдвиг (форма листа)", "shear", 0.0, 1.0)
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
    def toggle_bow():
        P["bow"] = not P["bow"]
        b_bow.config(text="СМЫЧОК: ВКЛ" if P["bow"] else "СМЫЧОК: выкл")
    def do_clear():
        N[0] = 0; NSPR[0] = 0; NLNK[0] = 0; NPIN[0] = 0
        STS.clear(); STt.lnk_start = -1; SEL[0] = 0
        bow_dot[0] = 0
        log("ОЧИСТКА сцены")
    def do_reset():
        reset_positions()
        log("СБРОС")
    def pluck_node_normal(a, sgn=1.0):
        sid = SIDM[a]
        nds = STS[sid].nodes
        k = nds.index(a)
        if len(nds) >= 3 and 0 < k < len(nds) - 1:
            tx = X[nds[k+1]] - X[nds[k-1]]
            ty = Y[nds[k+1]] - Y[nds[k-1]]
        else:
            tx, ty = 1.0, 0.0
        tl = math.hypot(tx, ty) + 1e-12
        STt.plk = (a, -ty/tl*sgn, tx/tl*sgn, time.time())
    def do_test():
        nseg = 24
        pts2 = [(0.15 + i*0.70/nseg, 0.15) for i in range(nseg+1)]
        sid = len(STS)
        s = Str()
        s.sp = dict(t=1.0, nl=0.04, ca=0.02, grav=0.0, shear=0.0, bend=0.0)
        s.nodes = []
        a0 = add_node(pts2[0][0], pts2[0][1], ZLAYER[0], sid, 1)
        s.nodes.append(a0)
        for k in range(1, len(pts2)):
            a = add_node(pts2[k][0], pts2[k][1], ZLAYER[0], sid,
                         1 if k == len(pts2)-1 else 0)
            add_spring(s.nodes[-1], a, sid, 1)
            s.nodes.append(a)
        STS.append(s)
        SEL[0] = sid
        mid = s.nodes[len(s.nodes)//2]
        log("ТЕСТ: струна t=1.0, автощипок 0.6 c", True)
        root.after(600, lambda: pluck_node_normal(mid, 1.0))
    b_tool = dbtn(buts, "ПЛАСТИНА (железо)", add_mesh, 17)
    b_str = dbtn(buts, "СТРУНА+ (S)", spawn_string, 12)
    b_bow = dbtn(buts, "СМЫЧОК: выкл", toggle_bow, 13)
    b_zm = dbtn(buts, "СЛОЙ Z −", lambda: ZLAYER.__setitem__(
        0, round(ZLAYER[0] - 0.02, 3)), 9)
    b_zp = dbtn(buts, "СЛОЙ Z +", lambda: ZLAYER.__setitem__(
        0, round(ZLAYER[0] + 0.02, 3)), 9)
    b_clear = dbtn(buts, "ОЧИСТИТЬ (C)", do_clear, 12)
    b_res = dbtn(buts, "СБРОС (r)", do_reset, 10)
    for b in (b_tool, b_str, b_bow, b_zm, b_zp, b_clear, b_res):
        b.pack(side="left", padx=2, pady=3)
    zlab = tk.Label(buts, text=f"z={ZLAYER[0]:+.2f}", bg=BG, fg="#ffcc60")
    zlab.pack(side="left", padx=6)
    def upd_z():
        zlab.config(text=f"z={ZLAYER[0]:+.2f}")
        root.after(200, upd_z)
    upd_z()
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
                x = np.clip(x*0.9/p, -1, 1)
                fn = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                  "jam_%s.wav"
                                  % datetime.datetime.now().strftime("%H%M%S"))
                with wave.open(fn, "wb") as w:
                    w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
                    w.writeframes((x*32767).astype("<i2").tobytes())
                log(f"ЗАПИСЬ сохранена: {os.path.basename(fn)}")
    b_test = dbtn(buts2, "ТЕСТ (T)", do_test, 10)
    b_ball = dbtn(buts2, "МЯЧ", spawn_ball, 8)
    b_rec = dbtn(buts2, "ЗАПИСЬ", rec_toggle, 9)
    qual = tk.OptionMenu(buts2, tk.StringVar(value="АВТО"),
                         "АВТО", "ЭКО", "НОРМА", "МАКС",
                         command=lambda v: None)
    qual.config(width=6, bg="#1e1e2a", fg=FG, activebackground="#2a2a3c",
                relief="flat", highlightthickness=0)
    b_quit = dbtn(buts2, "ВЫХОД", root.destroy, 7)
    for b in (b_test, b_ball, b_rec, qual, b_quit):
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
            cpu_pct = min(STt.cpu, 9.99) * 100.0
            status.config(text=f"CPU {cpu_pct:5.0f}%  ovs {STt.ovs}x  "
                               f"сбои {STt.underr}  масс {N[0]}  пружин {NSPR[0]}  "
                               f"контакт {STATS[2]:.0f}  R={STt.R_now:.2f}"
                               + ("  ЗАПИСЬ..." if STt.rec else ""))
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def nearest_node_screen(e, maxd=30.0):
        best, bd = -1, maxd * maxd
        for a in range(N[0]):
            x_, y_ = node_xy(a)
            d = (e.x - x_) ** 2 + (e.y - y_) ** 2
            if d < bd:
                bd, best = d, a
        return best, math.sqrt(bd)
    def pick(e):
        if e.state & 0x0004:
            bp, bdp = -1, 1e18
            for p in range(NPIN[0]):
                px_ = PXP[p] * ZOOM + PZP[p] * ZOOM * 0.35
                py_ = CANH - PYP[p] * ZOOM + PZP[p] * ZOOM * 0.20
                d = (e.x - px_) ** 2 + (e.y - py_) ** 2
                if d < bdp:
                    bdp, bp = d, p
            if bp >= 0:
                NPIN[0] -= 1
                for qq in range(bp, NPIN[0]):
                    PNH[qq] = PNH[qq+1]; PXP[qq] = PXP[qq+1]
                    PYP[qq] = PYP[qq+1]; PZP[qq] = PZP[qq+1]
                    PVX[qq] = PVX[qq+1]; PVY[qq] = PVY[qq+1]
                    PHX[qq] = PHX[qq+1]; PHY[qq] = PHY[qq+1]
                    PHZ[qq] = PHZ[qq+1]
                log("пин удалён")
            return
        if e.state & 0x20000:
            if NPIN[0] >= MAXPIN:
                log("лимит пинов", True); return
            p = NPIN[0]
            a, d = nearest_node_screen(e)
            if d > 40:
                a = -1
            PNH[p] = a
            wx, wy = to_world(e)
            PXP[p] = wx; PYP[p] = wy; PZP[p] = ZLAYER[0]
            PVX[p] = 0.0; PVY[p] = 0.0
            PHX[p] = wx; PHY[p] = wy; PHZ[p] = ZLAYER[0]
            RB[N[0] + p] = P["pinr"] * 1e-3
            log(f"ПИН #{p} {'→ резонатор' if a >= 0 else 'СВОБОДНЫЙ (мяч)'}")
            NPIN[0] += 1
            return
        wx, wy = to_world(e)
        STt.draw_pts = [(wx, wy)]
    def motion(e):
        if STt.draw_pts is not None:
            wx, wy = to_world(e)
            if math.hypot(wx - STt.draw_pts[-1][0],
                          wy - STt.draw_pts[-1][1]) >= 7.0 / ZOOM:
                STt.draw_pts.append((wx, wy))
        elif STt.grab is not None:
            a = STt.grab[0]
            wx, wy = to_world(e)
            STt.grab = (a, wx, wy, Z0[a])
    def release(e):
        if STt.draw_pts is not None:
            if len(STt.draw_pts) >= 2:
                draw_material(STt.draw_pts)
            STt.draw_pts = None
        if e.num == 1:
            STt.grab = None
        if e.num == 3:
            STt.plk = None
    def pluck(e):
        a, d = nearest_node_screen(e)
        if a >= 0 and d < 30:
            wx, wy = to_world(e)
            sid = SIDM[a]
            nds = STS[sid].nodes
            k = nds.index(a)
            if len(nds) >= 3 and 0 < k < len(nds) - 1:
                tx = X[nds[k+1]] - X[nds[k-1]]
                ty = Y[nds[k+1]] - Y[nds[k-1]]
            else:
                tx, ty = 1.0, 0.0
            tl = math.hypot(tx, ty) + 1e-12
            nx_ = -ty/tl; ny_ = tx/tl
            side = (wx - X[a]) * nx_ + (wy - Y[a]) * ny_
            sgn = 1.0 if side >= 0 else -1.0
            STt.plk = (a, nx_*sgn, ny_*sgn, time.time())
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", motion)
    cv.bind("<ButtonRelease-1>", release)
    cv.bind("<Button-3>", pluck)
    cv.bind("<ButtonRelease-3>", release)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<r>", lambda e: do_reset())
    root.bind("<t>", lambda e: do_test())
    root.bind("<s>", lambda e: spawn_string())
    root.bind("<c>", lambda e: do_clear())
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    log("v18 «ЖЕЛЕЗО»: ПЛАСТИНА = готовый лист (сдвиг+кромки, коллизии).")
    log("Рисуй: замкнутый контур = ЗАЛИВКА сеткой, открытый = ПОЛОСА.")
    log("СЛОЙ Z: листы на разных глубинах сталкиваются. Натяжение до 12 —")
    log("оверсэмплинг растёт автоматически, кристально чисто без краша.")
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
            stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                                     callback=audio_cb, latency="high")
            stream.start()
            log("аудио: fallback high latency")
        except Exception as e2:
            log(f"АУДИО НЕ ОТКРЫЛОСЬ: {e2}", True)
    draw(); tick(); root.mainloop()
    if stream:
        stream.stop(); stream.close()

def main():
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    if "diag" in [a.lower() for a in sys.argv[1:]]:
        print(sd.query_devices()); return
    print("компиляция ядра (до ~30 c)...")
    t0 = time.time()
    spawn_string()
    process_block(64)
    N[0] = 0; NSPR[0] = 0; NLNK[0] = 0; NPIN[0] = 0
    STS.clear(); SEL[0] = 0
    STt.logq.clear(); DCS[0] = 0.0; DCS[1] = 0.0; STATS[:] = 0.0
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