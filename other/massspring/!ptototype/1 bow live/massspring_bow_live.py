#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# MASSSPRING BOW LIVE v1 — «губернатор» (переписанное ядро)
# Своя версия DSP mass-spring смычковой струны с рисованием.
# Собрано заново, чтобы убрать баги прототипов v9..v18:
#   1) ВЗРЫВ ПРИ 0  -> сила смычка масштабируется Z0=sqrt(k*m0):
#                      k->0 => сила->0. Плюс губернатор жёсткости.
#                      Деления на натяжение нигде нет.
#   2) РИСОВАННЫЕ СТРУНЫ «ТЕРЯЛИ НАТЯЖЕНИЕ» -> нарисованная
#                      полилиния РЕСЕМПЛИТСЯ в равномерную цепочку
#                      (одинаковые rest-длины, прямое равновесие).
#   3) КЛИКИ -> нет runtime-контактов вообще (нечему дребезжать),
#                      мягкая «рука», плавная огибающая смычка.
#   4) ВЗРЫВЫ ЖЁСТКИХ СЕТОК -> авто-подшаги по Куранту
#                      (dt <= SAFETY/w, w=sqrt(k/m)), при упоре в
#                      MAXSUB губернатор мягко снижает жёсткость.
# Ядро: semi-implicit Euler + неявный MSW-смычок (бисекция, v9),
#       потери zeta*2*sqrt(k*m) по оси пружины, воздух cam.
# Рисование: открытая линия = струна (концы закреплены),
#            замкнутый контур = заливка сеткой масс («лист»),
#            пересечения нового со старым = пружины-связи.
# Зависимости: python -m pip install numpy numba sounddevice
# Режимы: python massspring_bow_live.py | diag | render
# Управление: ЛКМ рисовать/хватать, ПКМ поставить смычок
#             (в режиме рисования ПКМ = отмена), ПРОБЕЛ смычок,
#             S эталон-струна, D рисование, Alt+ЛКМ пин,
#             r сброс, C очистить, Esc отмена рисования
# ============================================================
import os, sys, math, time, wave
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np
try:
    from numba import njit
except ImportError:
    print("Нужен numba:  python -m pip install numba")
    raise

# ---------------- КОНСТАНТЫ ----------------
SR, BLOCK = 44100, 512
MAXM   = 1600      # предел масс
MAXSPR = 8000      # предел пружин
MAXPRB = 512       # предел точек съёма звука (pick-up)
MAXSUB = 24        # предел подшагов на один аудиосэмпл
SAFETY = 0.8       # доля критерия Куранта: dt <= SAFETY/w (предел 2.0)
VMAX   = 60.0      # предел скорости массы, м/с (последний предохранитель)
GVCAP  = 0.5       # предел прироста скорости от «руки» за подшаг (упругая рука,
                   # без щелчков)
# рука: жёсткость подгоняется под материал узла (KREF), вязкость = 0.7 критической
MU     = 0.04      # линейная плотность, кг/м (v9)
FR     = 0.70      # кинетическое трение / статическое (MSW, v9)
CREF   = 120.0     # опорная скорость волны в сетке, м/с
DX_TGT = 0.012     # целевой сегмент струны, м
KAPPA  = 10.0      # осевая жёсткость = KAPPA*T0/dx: преднатяжение T0 при
                   # rest = dx*(1-1/KAPPA) — струна «знает» своё натяжение
NSEG_MIN, NSEG_MAX = 6, 96   # >=6: короткие каракули не дают f0 > 3 кГц (урок v17)
MESH_H0   = 0.035    # шаг сетки, м
MESH_MAXN = 220      # предел масс в листе
LINK_R    = 0.024    # радиус автосвязи при рисовании, м
MIN_POLY_AREA = 0.02 # мин. площадь заливки, м²
CLOSE_PX  = 26       # порог «замкнули контур», px
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)   # DC-блок 25 Гц (v9)

BODYF = np.array([95.0, 160.0, 220.0, 380.0, 720.0, 1150.0])
BODYQ = np.array([6.0, 8.0, 9.0, 11.0, 13.0, 15.0])
BODYG = np.array([0.60, 0.50, 0.45, 0.40, 0.30, 0.22])

def body_coeffs(sr):
    nm = len(BODYF)
    r2 = np.empty(nm); c1 = np.empty(nm); b0 = np.empty(nm)
    for m in range(nm):
        w = 2 * math.pi * BODYF[m] / sr
        r = math.exp(-w / (2 * BODYQ[m]))
        r2[m] = r * r; c1[m] = 2 * r * math.cos(w); b0[m] = 0.5 * (1 - r * r)
    return r2, c1, b0

BR2_, BC1_, BB0_ = body_coeffs(SR)
BS1 = np.zeros(6); BS2 = np.zeros(6)     # состояния BODY
DCS = np.zeros(4)                        # dcx, dcy, (dca), (резерв)
STATS = np.zeros(3)                      # стик, клипы-пружин, страж

# ---------------- МИР ----------------
X  = np.zeros(MAXM); Y  = np.zeros(MAXM)   # текущие координаты, м
VX = np.zeros(MAXM); VY = np.zeros(MAXM)
X0 = np.zeros(MAXM); Y0 = np.zeros(MAXM)   # rest (равновесие)
MASS  = np.ones(MAXM)
FIXED = np.zeros(MAXM, np.int64)
KREF  = np.zeros(MAXM)                     # «жёсткость материала» узла (для связей)
NODE_OBJ = np.full(MAXM, -1, np.int64)
SI = np.zeros(MAXSPR, np.int64); SJ = np.zeros(MAXSPR, np.int64)
SREST = np.ones(MAXSPR)
SBASE = np.zeros(MAXSPR)     # физическая жёсткость при единичном live-множителе
SMODE = np.zeros(MAXSPR, np.int64)  # 1 струна, 2 сетка-ребро, 3 диагональ, 4 связь
SNL   = np.zeros(MAXSPR)
MMIN  = np.ones(MAXSPR)
PI_ = np.zeros(MAXPRB, np.int64); PWX = np.zeros(MAXPRB); PWY = np.zeros(MAXPRB)

ST = type("ST", (), {})()
ST.n = 0; ST.ns = 0; ST.nprb = 0
ST.objects = []
ST.grab = None            # (узел, tx, ty)
ST.bow_j = -1; ST.bow_nx = 0.0; ST.bow_ny = 1.0; ST.bow_z0b = 0.0
ST.bow_obj = -1           # индекс объекта-струны, на которой смычок
ST.dpts = []              # текущая рисуемая полилиния (px)
ST.pending = []           # структурные операции, применяются в начале блока
ST.gov = 1.0; ST.nsub = 1; ST.stick = 0.0; ST.cpu = 0.0; ST.underr = 0
ST.level = 0.0; ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0; ST.nreset = 0
ST.flash = ""; ST.flash_t = 0.0
RNG = np.random.default_rng(7)

P = dict(R=0.0, speed=1.0, press=1.0, beta=0.10, noise=0.20,
         fbase=110.0, tension=1.0, nl=0.04, zeta=0.0020, cmat=120.0,
         linkk=1.0, cam=0.02, zd=25.0, vol=0.35,
         bow=False, automorph=True, body=True, draw=False)
PALETTE = ["#e8e8f0", "#7fd0a0", "#d0a07f", "#a07fd0", "#7fb8d0"]

def flash(msg):
    ST.flash = msg; ST.flash_t = time.time()

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

def R_auto(ph):                      # авто-морф R: скрежет -> бас -> скрежет (v9)
    if ph < 1.2: return 0.0
    if ph < 2.7: return smoothstep((ph - 1.2) / 1.5)
    if ph < 4.2: return 1.0
    if ph < 5.7: return 1.0 - smoothstep((ph - 4.2) / 1.5)
    return 0.0

TT = np.linspace(0.0, 7.0, 701)
RT = np.array([R_auto(t) for t in TT])

# ---------------- ПОСТРОЕНИЕ СЦЕНЫ ----------------
def _dedupe(pts, eps=1e-6):
    out = [tuple(pts[0])]
    for p in pts[1:]:
        if math.hypot(p[0] - out[-1][0], p[1] - out[-1][1]) > eps:
            out.append(tuple(p))
    return out

def _resample(pts, nseg):
    pts = np.asarray(pts, float)
    seg = np.sqrt(np.sum(np.diff(pts, axis=0) ** 2, axis=1))
    s = np.concatenate([[0.0], np.cumsum(seg)])
    L = float(s[-1])
    t = np.linspace(0.0, L, nseg + 1)
    return np.interp(t, s, pts[:, 0]), np.interp(t, s, pts[:, 1]), L

def _spring(i, j, rest, k, mode, nl=0.0):
    q = ST.ns
    if q >= MAXSPR:
        flash("мир полон: пружины"); return False
    SI[q] = i; SJ[q] = j; SREST[q] = rest
    SBASE[q] = k; SMODE[q] = mode; SNL[q] = nl
    MMIN[q] = min(MASS[i], MASS[j])
    ST.ns += 1
    return True

def _link_new(new_ids, base):
    added = 0
    for j in new_ids:
        for i in range(base):
            if NODE_OBJ[i] == NODE_OBJ[j]:
                continue
            dx = X0[j] - X0[i]; dy = Y0[j] - Y0[i]
            d = math.hypot(dx, dy)
            if d < LINK_R:
                if _spring(i, j, max(d, 1e-4), 0.5 * min(KREF[i], KREF[j]), 4):
                    added += 1
    return added

def add_string(wpts, color=None):
    wpts = _dedupe(wpts)
    if len(wpts) < 2:
        flash("слишком короткая линия"); return
    if ST.n + NSEG_MAX + 2 > MAXM:
        flash("мир полон: массы"); return
    pts_tmp = np.asarray(wpts, float)
    seg = np.sqrt(np.sum(np.diff(pts_tmp, axis=0) ** 2, axis=1))
    L0 = float(seg.sum())
    if L0 < 0.05:
        flash("слишком короткая линия (мин 5 см)"); return
    nseg = int(round(L0 / DX_TGT))
    nseg = max(NSEG_MIN, min(NSEG_MAX, nseg))
    xi, yi, L = _resample(wpts, nseg)     # ГЛАВНЫЙ ФИКС: равномерный ресемпл
    dx = L / nseg
    m0 = MU * dx
    T0 = MU * (2.0 * L * P["fbase"]) ** 2
    k = KAPPA * T0 / dx                   # осевая жёсткость пружины
    r0 = dx * (1.0 - 1.0 / KAPPA)         # rest < dx => статическое T = T0
    base = ST.n
    idx = list(range(base, base + nseg + 1))
    for q, j in enumerate(idx):
        X0[j] = X[j] = float(xi[q]); Y0[j] = Y[j] = float(yi[q])
        VX[j] = VY[j] = 0.0
        MASS[j] = m0
        FIXED[j] = 1 if (q == 0 or q == nseg) else 0
        KREF[j] = k
        NODE_OBJ[j] = len(ST.objects)
    for q in range(nseg):
        _spring(idx[q], idx[q + 1], r0, k, 1, P["nl"])
    ux, uy = (xi[-1] - xi[0]) / L, (yi[-1] - yi[0]) / L
    ST.objects.append(dict(kind="string", nodes=idx, L=L, color=color,
                           perp=(-uy, ux), z0b=math.sqrt(T0 * MU)))
    nlinks = _link_new(idx, base)
    ST.n += nseg + 1
    _rebuild_probes()
    flash("струна: %d сегм., L=%.2f м, f0≈%.0f Гц" % (nseg, L, P["fbase"]))

def add_mesh(wpts):
    wpts = _dedupe(wpts)
    if len(wpts) < 4:
        return add_string(wpts)
    pts = np.asarray(wpts, float)
    x = pts[:, 0]; y = pts[:, 1]
    area = 0.5 * abs(np.dot(x, np.roll(y, -1)) - np.dot(y, np.roll(x, -1)))
    if area < MIN_POLY_AREA:
        flash("контур замкнут, но площадь мала (%.3f м²) — сделал струну" % area)
        return add_string(wpts)
    x0m, x1m = float(x.min()), float(x.max())
    y0m, y1m = float(y.min()), float(y.max())
    h = MESH_H0
    inside = None
    cnt = 10 ** 9
    for _ in range(8):
        gx = np.arange(x0m, x1m + h, h)
        gy = np.arange(y0m, y1m + h, h)
        GX, GY = np.meshgrid(gx, gy)
        inside = np.zeros(GX.shape, bool)
        for kk in range(len(pts)):
            ax_, ay_ = pts[kk]
            bx_, by_ = pts[(kk + 1) % len(pts)]
            cond = ((ay_ > GY) != (by_ > GY)) & \
                   (GX < (bx_ - ax_) * (GY - ay_) / ((by_ - ay_) + 1e-12) + ax_)
            inside ^= cond
        cnt = int(inside.sum())
        if cnt <= MESH_MAXN:
            break
        h *= 1.35
    if cnt < 4:
        flash("в контур не попали узлы — сетка не создана"); return
    if ST.n + cnt > MAXM:
        flash("мир полон: массы"); return
    ked = MU * CREF * CREF                 # ребро: k = mu*c^2
    ksh = 0.45 * ked                       # диагональ (сдвиг), урок v18
    m0 = MU * h
    base = ST.n
    obji = len(ST.objects)
    idmap = -np.ones(inside.shape, np.int64)
    nodes = []
    for r in range(inside.shape[0]):
        for c in range(inside.shape[1]):
            if inside[r, c]:
                j = base + len(nodes)
                idmap[r, c] = j
                X0[j] = X[j] = float(GX[r, c]); Y0[j] = Y[j] = float(GY[r, c])
                VX[j] = VY[j] = 0.0
                MASS[j] = m0; FIXED[j] = 0
                KREF[j] = ked; NODE_OBJ[j] = obji
                nodes.append(j)
    ns0 = ST.ns
    R_, C_ = inside.shape
    for r in range(R_):
        for c in range(C_):
            j = idmap[r, c]
            if j < 0: continue
            if c + 1 < C_ and idmap[r, c + 1] >= 0:
                _spring(j, idmap[r, c + 1], h, ked, 2)
            if r + 1 < R_ and idmap[r + 1, c] >= 0:
                _spring(j, idmap[r + 1, c], h, ked, 2)
            if r + 1 < R_ and c + 1 < C_ and idmap[r + 1, c + 1] >= 0:
                _spring(j, idmap[r + 1, c + 1], h * math.sqrt(2.0), ksh, 3)
            if r + 1 < R_ and c - 1 >= 0 and idmap[r + 1, c - 1] >= 0:
                _spring(j, idmap[r + 1, c - 1], h * math.sqrt(2.0), ksh, 3)
    if ST.ns - ns0 < 4:
        ST.ns = ns0
        flash("сетка распалась — фигура слишком вырождена"); return
    ST.objects.append(dict(kind="mesh", nodes=nodes, h=h, color="#8898c8",
                           spr0=ns0, spr1=ST.ns))
    nlinks = _link_new(nodes, base)
    ST.n += len(nodes)
    _rebuild_probes()
    flash("сетка: %d масс, шаг %.0f мм, %d пружин" %
          (len(nodes), h * 1000, ST.ns - ns0))

def _rebuild_probes():
    items = []
    for ob in ST.objects:
        nodes = ob["nodes"]
        if ob["kind"] == "string":
            inner = nodes[1:-1]
            if not inner: continue
            step = max(1, len(inner) // 10)
            px_, py_ = ob["perp"]
            for jj in inner[::step]:
                items.append((jj, px_, py_))
        else:
            for jj in nodes[::4]:
                items.append((jj, 0.0, 1.0))
    if not items:
        ST.nprb = 0; return
    w2 = 0.0
    for _, wx, wy in items:
        w2 += wx * wx + wy * wy
    sc = 1.0 / math.sqrt(max(w2, 1e-12))   # нормировка мощности съёма
    m = min(len(items), MAXPRB)
    for q in range(m):
        jj, wx, wy = items[q]
        PI_[q] = jj; PWX[q] = wx * sc; PWY[q] = wy * sc
    ST.nprb = m

def spawn_std():
    add_string([(0.20, 0.30), (1.00, 0.30)], PALETTE[0])
    ST.bow_obj = len(ST.objects) - 1
    set_bow_pos()

def clear_all():
    ST.n = 0; ST.ns = 0; ST.nprb = 0
    ST.objects = []
    ST.grab = None
    ST.bow_j = -1; ST.bow_z0b = 0.0; ST.bow_obj = -1
    FIXED[:] = 0; NODE_OBJ[:] = -1
    BS1[:] = 0.0; BS2[:] = 0.0; DCS[0] = DCS[1] = 0.0
    flash("сцена очищена")

def reset_pos():
    n = ST.n
    X[:n] = X0[:n]; Y[:n] = Y0[:n]
    VX[:n] = 0.0; VY[:n] = 0.0
    BS1[:] = 0.0; BS2[:] = 0.0; DCS[0] = DCS[1] = 0.0
    flash("позиции сброшены к покою")

def toggle_pin(j):
    if j < 0 or j >= ST.n: return
    FIXED[j] = 1 - FIXED[j]
    if ST.grab is not None and ST.grab[0] == j:
        ST.grab = None
    flash("узел %d: %s" % (j, "ЗАПИНЕН" if FIXED[j] else "свободен"))

# ---------------- ЯДРО (numba) ----------------
@njit(cache=True, fastmath=True)
def core(X0g, Y0g, X, Y, VX, VY, FIXED, MASS, n,
         SI, SJ, SREST, SKE, SCE, SNL, SNLC, SMODE, MMIN, ns,
         PI_, PWX, PWY, nprb,
         grab_j, gtx, gty, kg, cg, gvcap,
         bow_on, bow_j, bnx, bny, bow_m,
         noise, Rarr, onarr, frames, nsub, dts,
         spd, press, nzd, fr, z0,
         cam, vmax,
         body_on, bc1, br2, bb0, bgn, bs1, bs2, dcs, dca,
         out, stats):
    dcx = dcs[0]; dcy = dcs[1]; stick = 0.0
    nf = 0
    for f in range(frames):
        R = Rarr[f]; on = onarr[f]
        vb0 = (0.06 + 0.34 * R) * spd
        fn0 = press * 0.5 * (6.0 - 4.0 * R) * z0 * vb0
        vf = 0.012 + 0.10 * R
        for s_ in range(nsub):
            for q in range(ns):
                i = SI[q]; j = SJ[q]
                dx = X[j] - X[i]; dy = Y[j] - Y[i]
                d = math.sqrt(dx * dx + dy * dy) + 1e-12
                e = d - SREST[q]
                nx = dx / d; ny = dy / d
                rel = (VX[j] - VX[i]) * nx + (VY[j] - VY[i]) * ny
                F = SKE[q] * e + SCE[q] * rel
                if SNL[q] > 0.0 and SMODE[q] == 1:
                    strain = e / SREST[q]
                    mlt = 1.0 + SNL[q] * strain * strain
                    if mlt > SNLC: mlt = SNLC
                    F += SKE[q] * e * (mlt - 1.0)
                # ВАЖНО: никаких клампов силы — они съедают статическое
                # преднатяжение (баг «струна теряла натяжение»). Стабильность
                # обеспечивают подшаги по Куранту + губернатор + VMAX.
                ai = F * dts / MASS[i]
                aj = F * dts / MASS[j]
                if FIXED[i] == 0:
                    VX[i] += ai * nx; VY[i] += ai * ny
                if FIXED[j] == 0:
                    VX[j] -= aj * nx; VY[j] -= aj * ny
            dm = 1.0 - cam * dts
            if dm < 0.0: dm = 0.0
            vm2 = vmax * vmax
            for a in range(n):
                if FIXED[a] == 0:
                    VX[a] *= dm; VY[a] *= dm
                    v2 = VX[a] * VX[a] + VY[a] * VY[a]
                    if v2 > vm2:
                        sc = vmax / math.sqrt(v2)
                        VX[a] *= sc; VY[a] *= sc
                else:
                    VX[a] = 0.0; VY[a] = 0.0
            if grab_j >= 0 and FIXED[grab_j] == 0:
                Fx = kg * (gtx - X[grab_j]) - cg * VX[grab_j]
                Fy = kg * (gty - Y[grab_j]) - cg * VY[grab_j]
                dvx = Fx * dts / MASS[grab_j]
                dvy = Fy * dts / MASS[grab_j]
                vm = math.sqrt(dvx * dvx + dvy * dvy)
                if vm > gvcap:
                    sc = gvcap / vm
                    dvx *= sc; dvy *= sc
                VX[grab_j] += dvx; VY[grab_j] += dvy
            if bow_on > 0.5 and fn0 > 0.0 and bow_j >= 0 and FIXED[bow_j] == 0:
                u = noise[nf]; nf += 1
                fn = fn0 * (1.0 + nzd * u) * on
                if fn < 0.0: fn = 0.0
                vn = VX[bow_j] * bnx + VY[bow_j] * bny
                a = fn * dts / bow_m
                if a > 1e-15:
                    vb = vb0 * on
                    lo = vn - a; hi = vn + a
                    for _ in range(48):
                        mid = 0.5 * (lo + hi)
                        mu = fr + (1.0 - fr) * math.exp(-abs(vb - mid) / vf)
                        if mid - vn - a * mu < 0.0: lo = mid
                        else: hi = mid
                    vn2 = 0.5 * (lo + hi)
                    dvn = vn2 - vn
                    VX[bow_j] += dvn * bnx; VY[bow_j] += dvn * bny
                    if abs(vb - vn2) < 0.004: stick += 1.0
            for a in range(n):
                X[a] += VX[a] * dts
                Y[a] += VY[a] * dts
        s = 0.0
        for p in range(nprb):
            k_ = PI_[p]
            s += PWX[p] * VX[k_] + PWY[p] * VY[k_]
        if body_on > 0.5:
            wet = 0.0
            for q in range(6):
                yb = bb0[q] * s + bc1[q] * bs1[q] - br2[q] * bs2[q]
                bs2[q] = bs1[q]; bs1[q] = yb
                wet += bgn[q] * yb
            s = 0.8 * s + 0.5 * wet
        xs = s - dcx + dca * dcy
        dcx = s; dcy = xs
        out[f] = xs
    stats[0] = stick; stats[1] = 0.0
    bad = False
    for a in range(n):
        if not (math.isfinite(X[a]) and math.isfinite(Y[a])
                and math.isfinite(VX[a]) and math.isfinite(VY[a])):
            bad = True
            break
    if bad:
        for a in range(n):
            X[a] = X0g[a]; Y[a] = Y0g[a]
            VX[a] = 0.0; VY[a] = 0.0
        for f in range(frames):
            out[f] = 0.0
        stats[2] = 1.0
    else:
        stats[2] = 0.0
    dcs[0] = dcx; dcs[1] = dcy

# ---------------- БЛОК ОБРАБОТКИ ----------------
def process_block(frames):
    # структурные операции (только между блоками — состояние не рвётся)
    if ST.pending:
        acts, ST.pending = ST.pending, []
        for a in acts:
            kind = a[0]
            if kind == "clear": clear_all()
            elif kind == "reset": reset_pos()
            elif kind == "string": add_string(a[1], a[2])
            elif kind == "mesh": add_mesh(a[1])
    ns = ST.ns
    out = np.empty(frames)
    if ns == 0 or ST.n == 0:
        out[:] = 0.0
        ST.gov = 1.0; ST.nsub = 1
        y = np.tanh(out * (1.25 * P["vol"])) * 0.98
        ST.level = 0.9 * ST.level + 0.1 * float(np.mean(np.abs(y)))
        return y
    # --- live-жёсткость: натяжение/сетка/связи (векторно, вне ядра) ---
    mode = SMODE[:ns]
    ske = SBASE[:ns] * np.where(mode == 1, P["tension"], 1.0)
    ske = ske * np.where((mode == 2) | (mode == 3),
                         (P["cmat"] / CREF) ** 2, 1.0)
    ske = ske * np.where(mode == 4, P["linkk"], 1.0)
    # --- рука (нужна ДО CFL: её пружина участвует в критерии) ---
    if ST.grab is not None:
        grab_j, gtx, gty, gkg, gcg = ST.grab
        if grab_j >= ST.n: grab_j = -1
    else:
        grab_j, gtx, gty, gkg, gcg = -1, 0.0, 0.0, 0.0, 0.0
    # --- ГУБЕРНАТОР: подшаги по Куранту + мягкое снижение жёсткости ---
    # max собственное значение цепочки = 4k/m -> w_eig = 2*sqrt(k/m)
    wmax = 2.0 * math.sqrt(float((ske / MMIN[:ns]).max())) if ns > 0 else 0.0
    if grab_j >= 0 and gkg > 0.0:      # пружина руки тоже участвует в CFL
        wgr = math.sqrt(gkg / float(MASS[grab_j]))
        if wgr > wmax:
            wmax = wgr
    dts0 = 1.0 / SR
    gov = 1.0
    nsub = 1
    if wmax > 0.0:
        nsub = int(math.ceil(wmax * dts0 / SAFETY))
        if nsub > MAXSUB:
            gov = (SAFETY * MAXSUB / (wmax * dts0)) ** 2
            ske = ske * gov
            nsub = MAXSUB
    ST.gov = gov; ST.nsub = nsub
    sce = 2.0 * P["zeta"] * np.sqrt(ske * MMIN[:ns])
    dts = dts0 / nsub
    # --- огибающие и шум ---
    phase = (ST.t + np.arange(frames) / SR) % 7.0
    Rarr = np.interp(phase, TT, RT) if P["automorph"] else np.full(frames, P["R"])
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    onarr = np.clip(ST.on + stp * np.arange(1, frames + 1), 0.0, 1.0)
    ST.on = float(onarr[-1]); ST.t += frames / SR
    ST.R_now = float(Rarr[-1])
    noise = RNG.standard_normal(frames * nsub)
    # --- смычок ---
    bow_on = 1.0 if P["bow"] else 0.0
    bj = ST.bow_j
    if bj >= ST.n:               # узел вне мира -> смычок молчит
        bj = -1
    z0 = 0.0
    if bj >= 0:
        z0 = ST.bow_z0b * math.sqrt(max(P["tension"], 0.0) * gov)
    snlc = 1.0 + 2.0 * P["nl"]
    t0 = time.perf_counter()
    core(X0, Y0, X, Y, VX, VY, FIXED, MASS, ST.n,
         SI, SJ, SREST, ske, sce, SNL, snlc, SMODE, MMIN, ns,
         PI_, PWX, PWY, ST.nprb,
         grab_j, gtx, gty, gkg, gcg, GVCAP,
         bow_on, bj, ST.bow_nx, ST.bow_ny,
         (MASS[bj] if bj >= 0 else 1.0),
         noise, Rarr, onarr, frames, nsub, dts,
         P["speed"], P["press"], P["noise"], FR, z0,
         P["cam"], VMAX,
         1.0 if P["body"] else 0.0, BC1_, BR2_, BB0_, BODYG, BS1, BS2, DCS, DCA,
         out, STATS)
    ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0) / (frames / SR)
    ST.stick = 0.9 * ST.stick + 0.1 * STATS[0] / max(frames * nsub, 1)
    # --- СТРАЖ: NaN/разлёт -> мягкий сброс (губернатор обычно не пускает) ---
    if STATS[2] > 0.5 or not np.isfinite(out).all() \
            or float(np.abs(VX[:ST.n]).max()) > VMAX * 1.5 \
            or float(np.abs(Y[:ST.n]).max()) > 2.0:
        reset_pos()
        ST.nreset += 1
        flash("СТРАЖ: сброс состояния (№%d)" % ST.nreset)
        out[:] = 0.0
    y = np.tanh(out * (1.25 * P["vol"])) * 0.98
    ST.level = 0.9 * ST.level + 0.1 * float(np.mean(np.abs(y)))
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status: ST.underr += 1
        outdata[:, 0] = process_block(frames)
    except Exception:
        import traceback; traceback.print_exc()
        outdata[:] = 0

def render_wav(path, seconds=14.0):
    total = int(SR * seconds); pos = 0
    buf = np.empty(total)
    print("рендер %.0f с..." % seconds)
    while pos < total:
        nb = min(BLOCK, total - pos)
        buf[pos:pos + nb] = process_block(nb)
        pos += nb
        if (pos // BLOCK) % (SR // BLOCK) == 0:
            print("  %4.0f/%.0f с" % (pos / SR, seconds))
    x = buf.copy()
    nf = SR // 5
    x[-nf:] *= np.linspace(1.0, 0.0, nf)
    p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
    x = np.clip(x * 0.85 / p, -1, 1)
    with wave.open(path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((x * 32767).astype("<i2").tobytes())
    print("ГОТОВО:", path)
    return buf

# ---------------- GUI ----------------
PPM = 700.0          # пикселей на метр (вид)

def set_bow_pos():
    """Ставит узел смычка по P['beta'] вдоль струны ST.bow_obj."""
    oi = ST.bow_obj
    if oi < 0 or oi >= len(ST.objects):
        ST.bow_j = -1; ST.bow_z0b = 0.0; return
    ob = ST.objects[oi]
    if ob["kind"] != "string":
        ST.bow_j = -1; ST.bow_z0b = 0.0; return
    nodes = ob["nodes"]
    q = int(round(P["beta"] * (len(nodes) - 1)))
    q = max(1, min(len(nodes) - 2, q))       # только внутренние узлы
    j = nodes[q]
    ST.bow_nx, ST.bow_ny = ob["perp"]
    ST.bow_z0b = ob["z0b"]
    ST.bow_j = j                              # последним — атомарно для аудио

def run_gui():
    import tkinter as tk
    import sounddevice as sd
    root = tk.Tk()
    root.title("massspring bow live v1 — governor edition")
    CANW, CANH = 940, 420
    cv = tk.Canvas(root, width=CANW, height=CANH, bg="#101018",
                   highlightthickness=0)
    cv.pack(fill="x")

    def node_px(j):
        zd = P["zd"]
        return ((X0[j] + zd * (X[j] - X0[j])) * PPM,
                (Y0[j] + zd * (Y[j] - Y0[j])) * PPM)

    def nearest_node(px, py, rmax, strings_only=False):
        best = -1; bd = rmax * rmax; bo = -1
        for oi, ob in enumerate(ST.objects):
            if strings_only and ob["kind"] != "string":
                continue
            for q, j in enumerate(ob["nodes"]):
                if strings_only and (q == 0 or q == len(ob["nodes"]) - 1):
                    continue
                xj, yj = node_px(j)
                d = (px - xj) ** 2 + (py - yj) ** 2
                if d < bd:
                    bd = d; best = j; bo = oi
        return best, bo, math.sqrt(bd) if best >= 0 else 0.0

    def draw():
        try:
            cv.delete("all")
            for ob in ST.objects:
                nodes = ob["nodes"]
                if ob["kind"] == "string":
                    pts = []
                    for j in nodes:
                        xj, yj = node_px(j)
                        pts += [xj, yj]
                    cv.create_line(*pts, fill=ob["color"], width=2,
                                   capstyle="round")
                else:
                    for q in range(ob["spr0"], ob["spr1"]):
                        if SMODE[q] != 2: continue
                        a = SI[q]; b = SJ[q]
                        xa, ya = node_px(a); xb, yb = node_px(b)
                        cv.create_line(xa, ya, xb, yb, fill=ob["color"],
                                       width=1)
            for j in range(ST.n):
                if FIXED[j]:
                    xj, yj = node_px(j)
                    cv.create_rectangle(xj - 4, yj - 4, xj + 4, yj + 4,
                                        outline="#c05050", width=2)
            if ST.bow_j >= 0 and ST.bow_j < ST.n:
                xj, yj = node_px(ST.bow_j)
                cv.create_oval(xj - 6, yj - 6, xj + 6, yj + 6,
                               fill="#ff5040", width=0)
            if ST.grab is not None:
                xj, yj = node_px(ST.grab[0])
                cv.create_oval(xj - 9, yj - 9, xj + 9, yj + 9,
                               outline="#ffd040", width=2)
            if len(ST.dpts) >= 2:
                flat = []
                for px, py in ST.dpts:
                    flat += [px, py]
                cv.create_line(*flat, fill="#50e0ff", width=2,
                               dash=(6, 4))
            if time.time() - ST.flash_t < 2.5 and ST.flash:
                cv.create_text(CANW // 2, 18, text=ST.flash,
                               fill="#ffe080", font=("TkDefaultFont", 11))
            if P["draw"]:
                cv.create_text(12, 12, anchor="w",
                               text="РЕЖИМ РИСОВАНИЯ: ЛКМ — вести линию, "
                                    "замкнуть = сетка; ПКМ/Esc — отмена",
                               fill="#50e0ff", font=("TkDefaultFont", 9))
            else:
                cv.create_text(12, 12, anchor="w",
                               text="ИГРА: ЛКМ — схватить массу, ПКМ — "
                                    "смычок на струну, ПРОБЕЛ — смычок "
                                    "вкл/выкл, Alt+ЛКМ — пин",
                               fill="#8888a0", font=("TkDefaultFont", 9))
            lv = min(1.0, ST.level * 4)
            cv.create_rectangle(12, CANH - 16,
                                12 + int((CANW - 24) * lv), CANH - 6,
                                fill="#40c080", width=0)
        except Exception as e:
            print("draw:", e)
        root.after(40, draw)

    panel = tk.Frame(root); panel.pack(fill="x")
    left = [("R  скрежет <-> бас", "R", 0.0, 1.0, 0.005),
            ("Скорость смычка", "speed", 0.0, 1.6, 0.01),
            ("Прижим", "press", 0.0, 2.0, 0.01),
            ("Позиция смычка β", "beta", 0.02, 0.45, 0.005),
            ("Шум волоса", "noise", 0.0, 1.0, 0.01),
            ("Воздух (затухание)", "cam", 0.002, 0.12, 0.001)]
    right = [("Нотка f0, Гц", "fbase", 55.0, 220.0, 1.0),
             ("Натяжение (0 = верёвка)", "tension", 0.0, 2.2, 0.01),
             ("Нелинейность", "nl", 0.0, 0.15, 0.005),
             ("Потери ζ", "zeta", 0.0002, 0.02, 0.0001),
             ("Жёсткость сетки, м/с", "cmat", 20.0, 600.0, 5.0),
             ("Связи фигур", "linkk", 0.0, 2.0, 0.01),
             ("Масштаб колебаний", "zd", 2.0, 100.0, 1.0),
             ("Громкость", "vol", 0.0, 1.0, 0.005)]
    sv = {}

    def add_slider(row, col, lab, key, a, b, res):
        tk.Label(panel, text=lab, width=22, anchor="w").grid(
            row=row, column=col)
        var = tk.DoubleVar(value=P[key]); sv[key] = var

        def cmd(v, k=key):
            P[k] = float(v)
            if k == "beta":
                set_bow_pos()
        sc = tk.Scale(panel, from_=a, to=b, resolution=res,
                      orient="horizontal", length=250, showvalue=1,
                      variable=var, command=cmd)
        sc.grid(row=row, column=col + 1, sticky="w")
        sv[key + "_sc"] = sc

    for r_, (lab, key, a, b, res) in enumerate(left):
        add_slider(r_, 0, lab, key, a, b, res)
    for r_, (lab, key, a, b, res) in enumerate(right):
        add_slider(r_, 2, lab, key, a, b, res)

    buts = tk.Frame(root); buts.pack(fill="x")
    def mk_toggle(key, lab_on, lab_off):
        b = tk.Button(buts, width=15,
                      command=lambda: toggle(key))
        def refresh():
            b.config(text=lab_on if P[key] else lab_off)
        def toggle(k=key):
            P[k] = not P[k]
            if k == "draw":
                ST.dpts = []
            refresh()
        refresh()
        return b, refresh

    b_draw, rf_draw = mk_toggle("draw", "РИСОВАТЬ", "РИСОВАТЬ: выкл")
    b_bow, rf_bow = mk_toggle("bow", "СМЫЧОК: ВКЛ", "СМЫЧОК: выкл")
    b_auto, rf_auto = mk_toggle("automorph", "АВТО-МОРФ: ВКЛ",
                                "АВТО-МОРФ: выкл")
    b_body, rf_body = mk_toggle("body", "BODY: ВКЛ", "BODY: выкл")
    for b in (b_draw, b_bow, b_auto, b_body):
        b.pack(side="left", padx=3, pady=3)

    buts2 = tk.Frame(root); buts2.pack(fill="x")
    tk.Button(buts2, text="ЭТАЛОН (S)", width=12,
              command=lambda: ST.pending.append(
                  ("string", [(0.20, 0.30), (1.00, 0.30)],
                   PALETTE[len(ST.objects) % len(PALETTE)]))
              ).pack(side="left", padx=3, pady=3)
    tk.Button(buts2, text="СБРОС (r)", width=10,
              command=lambda: ST.pending.append(("reset",))
              ).pack(side="left", padx=3, pady=3)
    tk.Button(buts2, text="ОЧИСТИТЬ (C)", width=13,
              command=lambda: ST.pending.append(("clear",))
              ).pack(side="left", padx=3, pady=3)
    tk.Button(buts2, text="ВЫХОД", width=8,
              command=root.destroy).pack(side="left", padx=3, pady=3)

    status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")

    def tick():
        try:
            if P["automorph"]:
                sv["R"].set(ST.R_now)
            gov = ("  ГУБ x%.2f" % ST.gov) if ST.gov < 0.999 else ""
            status.config(text=
                "CPU %3.0f%%   подшаги %d   стик %3.0f%%   страж %d   "
                "масс %d / пружин %d   сбои %d   R=%.2f%s"
                % (ST.cpu * 100, ST.nsub, ST.stick * 100, ST.nreset,
                   ST.n, ST.ns, ST.underr, ST.R_now, gov))
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)

    # ---------- мышь ----------
    def on_lmb(e):
        if P["draw"]:
            ST.dpts = [(e.x, e.y)]
            return
        alt = bool(e.state & 0x0008) or bool(e.state & 0x20000)
        j, _, _ = nearest_node(e.x, e.y, 14)
        if j < 0:
            return
        if alt:
            toggle_pin(j)
            return
        if FIXED[j]:
            flash("узел запинен — свободен только Alt+ЛКМ")
            return
        tx = X0[j] + (e.x / PPM - X0[j]) / P["zd"]
        ty = Y0[j] + (e.y / PPM - Y0[j]) / P["zd"]
        tx = max(X0[j] - 0.12, min(X0[j] + 0.12, tx))
        ty = max(Y0[j] - 0.12, min(Y0[j] + 0.12, ty))
        kg = min(2.0e6, float(KREF[j]))
        cg = 1.4 * math.sqrt(kg * float(MASS[j]))
        ST.grab = (j, tx, ty, kg, cg)

    def on_motion(e):
        if P["draw"]:
            if ST.dpts:
                lx, ly = ST.dpts[-1]
                if math.hypot(e.x - lx, e.y - ly) >= 6:
                    ST.dpts.append((e.x, e.y))
            return
        if ST.grab is not None:
            j = ST.grab[0]
            tx = X0[j] + (e.x / PPM - X0[j]) / P["zd"]
            ty = Y0[j] + (e.y / PPM - Y0[j]) / P["zd"]
            tx = max(X0[j] - 0.12, min(X0[j] + 0.12, tx))
            ty = max(Y0[j] - 0.12, min(Y0[j] + 0.12, ty))
            kg = ST.grab[3]; cg = ST.grab[4]
            ST.grab = (j, tx, ty, kg, cg)

    def on_release(e):
        if P["draw"] and len(ST.dpts) >= 3:
            wpts = [(px / PPM, py / PPM) for px, py in ST.dpts]
            closed = math.hypot(ST.dpts[0][0] - ST.dpts[-1][0],
                                ST.dpts[0][1] - ST.dpts[-1][1]) < CLOSE_PX
            col = PALETTE[len(ST.objects) % len(PALETTE)]
            ST.pending.append(("mesh" if closed else "string", wpts, col))
        ST.dpts = []
        ST.grab = None

    def on_rmb(e):
        if P["draw"]:
            ST.dpts = []
            flash("рисование отменено")
            return
        j, oi, d = nearest_node(e.x, e.y, 40, strings_only=True)
        if j < 0:
            flash("рядом нет струны — подведи курсор ближе")
            return
        ob = ST.objects[oi]
        loc = ob["nodes"].index(j)
        frac = loc / (len(ob["nodes"]) - 1)
        P["beta"] = frac
        sv["beta"].set(frac)
        ST.bow_obj = oi
        set_bow_pos()
        flash("смычок: струна #%d, β=%.2f%s"
              % (oi, frac, "" if P["bow"] else "  (ПРОБЕЛ — включить)"))

    cv.bind("<Button-1>", on_lmb)
    cv.bind("<B1-Motion>", on_motion)
    cv.bind("<ButtonRelease-1>", on_release)
    cv.bind("<Button-3>", on_rmb)

    def k_bow(e=None):
        P["bow"] = not P["bow"]; rf_bow()
    def k_draw(e=None):
        P["draw"] = not P["draw"]; ST.dpts = []; rf_draw()
    def k_std(e=None):
        ST.pending.append(("string", [(0.20, 0.30), (1.00, 0.30)],
                           PALETTE[len(ST.objects) % len(PALETTE)]))
    def k_reset(e=None):
        ST.pending.append(("reset",))
    def k_clear(e=None):
        ST.pending.append(("clear",))
    root.bind("<space>", k_bow)
    root.bind("<Key-d>", k_draw)
    root.bind("<Key-s>", k_std)
    root.bind("<Key-r>", k_reset)
    root.bind("<Key-c>", k_clear)
    root.bind("<Escape>", lambda e: ST.dpts.clear())
    root.protocol("WM_DELETE_WINDOW", root.destroy)

    print("""ЛИНИЯ = струна (концы закреплены), ЗАМКНУТЫЙ КОНТУР = сетка-«лист».
Пересечения нового со старым сшиваются пружинами-связями автоматически.
ПКМ по струне ставит смычок, ПРОБЕЛ включает. β-слайдер двигает смычок по струне.
Натяжение 0 = верёвка: смычок молчит (силы нет) — это НЕ баг, а физика.
При перегрузе включается ГУБ (мягкое снижение жёсткости) — звук вялый, но живой.""")
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                             callback=audio_cb, latency="low")
    stream.start()
    draw(); tick()
    root.mainloop()
    stream.stop(); stream.close()

# ---------------- ЗАПУСК ----------------
def warmup():
    print("компиляция ядра (один раз, до ~20 c)...")
    t0 = time.time()
    process_block(64); process_block(64)
    reset_pos()
    ST.t = 0.0; ST.on = 0.0; ST.stick = 0.0; ST.cpu = 0.0
    print("ядро готово за %.1f c" % (time.time() - t0))

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print("python %s | numpy %s" % (sys.version.split()[0], np.__version__))
    if "diag" in args:
        import sounddevice as sd
        print(sd.query_devices())
        print("default:", sd.default.device)
        return
    spawn_std()
    warmup()
    if "render" in args:
        P["bow"] = True
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            "bow_render.wav")
        render_wav(path)
        return
    run_gui()

if __name__ == "__main__":
    main()
