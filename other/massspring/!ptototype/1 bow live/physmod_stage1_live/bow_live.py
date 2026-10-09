#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
bow_live.py — Этап 1: ЖИВОЙ прототип физмод-синтезатора с GUI (Tkinter, один файл).

Что внутри:
  • MD-струны (цепочка масса-пружина, полунеявный Эйлер, оверсэмплинг)
  • Смычок MSW (термальная кривая трения, неявная линейная часть около прилипания)
  • Контакты струна-струна (Hunt-Crossley, F>=0, кап демпфера)
  • Guards: валидация при спавне, клип скорости, авто-заморозка, auto-panic, NaN-страж
  • Лог событий: SPAWN/REJECT/BOW/CONTACT/GUARD/NAN/PANIC/XRUN — сразу видно, что произошло
  • GUI: рисование струн мышью, смычок, щипок, ластик, ручки-кнобы, метры, скоп

Запуск:  py bow_live.py          (Windows)
         python3 bow_live.py     (Linux/macOS)
Зависимости: pip install numpy sounddevice   (tkinter входит в Python)

Все координаты строки — в метрах (мировые), canvas пересчитывает px<->м.
Смещения масс U — поперёк оси струны (1 DOF на массу, как в валидированном stage0).
"""
import math
import time
import random
import threading
import queue
import os
import sys
import wave

import numpy as np

# JIT-ядро (numba) — ускоряет внутренний цикл в ~50-100 раз; без него работает
# медленный numpy-путь (поставь: pip install numba)
try:
    from numba import njit
    HAVE_NUMBA = True
except Exception:
    HAVE_NUMBA = False

# ----------------------------------------------------------------------------
# Константы
# ----------------------------------------------------------------------------
SR = 44100            # частота дискретизации аудио
BLOCK = 256           # размер блока аудио (сэмплов)
N_NODES = 48          # масс в струне
MAX_STRINGS = 8       # слотов под струны
MU = 3.44e-3          # линейная плотность, кг/м (~нейлон 0.5мм)
R_STR = 0.8e-3        # визуальный радиус струны, м
KD = 0.25             # кинетический пол трения (tanh-член)
V_GUARD = 60.0        # клип скорости, м/с
BOW_DAMP = 0.01       # локальное демпфирование в точке смычка (доля Z0)
CHI = 0.3             # доля KC в демпфере контакта (Hunt-Crossley)
RING = 16384          # кольцевой буфер аудио, кадров

TAG_PAD = "-" * 78


def lerp(a, b, t):
    return a + (b - a) * t


# ----------------------------------------------------------------------------
# Лог: потокобезопасная очередь; GUI сливает её в панель + файл
# ----------------------------------------------------------------------------
LOG_Q = queue.Queue()


def log(ev, msg):
    LOG_Q.put((time.time(), ev, msg))


# ----------------------------------------------------------------------------
# Макро Regime (расписание Bow Morph из stage0, R 0..6)
# ----------------------------------------------------------------------------
R_SCHED = [(0.0, 0.0), (1.5, 0.0), (3.0, 1.0), (4.5, 1.0), (6.0, 0.0)]


def _smooth(t):
    return t * t * (3.0 - 2.0 * t)


def sched_s(R):
    R = min(max(R, 0.0), 6.0)
    for (r0, s0), (r1, s1) in zip(R_SCHED, R_SCHED[1:]):
        if R <= r1 + 1e-9:
            t = 0.0 if r1 <= r0 else (R - r0) / (r1 - r0)
            return s0 + (s1 - s0) * _smooth(t)
    return 0.0


def sched_params(R, vb_trim=1.0, press_trim=1.0):
    """R -> (v_bow, mult, v_f, beta, noise). 0..1.5 скрежет, 3..4.5 чистый бас."""
    s = sched_s(R)
    vb = lerp(0.05, 0.22, s) * vb_trim
    mult = lerp(14.0, 4.5, s) * press_trim
    vf = lerp(0.016, 0.030, s)
    beta = lerp(0.065, 0.14, s)
    noise = (1.0 - s) * 0.35
    return vb, mult, vf, beta, noise


def seg_seg_dist(p1, q1, p2, q2):
    """Мин. расстояние между отрезками 2D (чистый python, вызывается редко)."""
    def dot(a, b):
        return a[0] * b[0] + a[1] * b[1]

    d1 = (q1[0] - p1[0], q1[1] - p1[1])
    d2 = (q2[0] - p2[0], q2[1] - p2[1])
    r = (p1[0] - p2[0], p1[1] - p2[1])
    a, e = dot(d1, d1), dot(d2, d2)
    f = dot(d2, r)
    if a <= 1e-12 and e <= 1e-12:
        return math.hypot(r[0], r[1])
    if a <= 1e-12:
        s = 0.0
        t = min(max(f / e, 0.0), 1.0)
    else:
        c = dot(d1, r)
        if e <= 1e-12:
            t = 0.0
            s = min(max(-c / a, 0.0), 1.0)
        else:
            b, den = dot(d1, d2), (a * e - dot(d1, d2) * dot(d1, d2))
            s = min(max((b * f - c * e) / den, 0.0), 1.0) if den > 1e-12 else 0.0
            t = (b * s + f) / e
            if t < 0.0:
                t = 0.0
                s = min(max(-c / a, 0.0), 1.0)
            elif t > 1.0:
                t = 1.0
                s = min(max((b - c) / a, 0.0), 1.0)
    c1 = (p1[0] + d1[0] * s, p1[1] + d1[1] * s)
    c2 = (p2[0] + d2[0] * t, p2[1] + d2[1] * t)
    return math.hypot(c1[0] - c2[0], c1[1] - c2[1])


# ----------------------------------------------------------------------------
# JIT-ядро одного блока (та же математика, что numpy-путь, но без оверхеда)
# ----------------------------------------------------------------------------
if HAVE_NUMBA:
    @njit(cache=True)
    def _block_kernel(U, V, F, POS, rest, nrm, k, m, Z0, damp_f, dt_over_m,
                      pairs, pair_kc, pair_kcchi, pair_rmin,
                      bowing, bow_r, bow_c, bow_vb, bow_mult, bow_vf, bow_noise,
                      noise, outL, outR, wL, wR, dt, os_f, guards, v_guard,
                      kd_c, bow_damp_c, clip_cnt):
        S, N = U.shape
        n_in = noise.shape[0]
        touched = 0
        for it in range(n_in):
            # --- силы: демпфирование + пружины
            for r in range(S):
                df = damp_f[r]
                for j in range(N):
                    F[r, j] = -df * V[r, j]
                kr = k[r]
                for j in range(1, N - 1):
                    F[r, j] += kr * (U[r, j + 1] - 2.0 * U[r, j] + U[r, j - 1])
            # --- позиции узлов
            for r in range(S):
                nx_ = nrm[r, 0]
                ny_ = nrm[r, 1]
                for j in range(N):
                    POS[r, j, 0] = rest[r, j, 0] + nx_ * U[r, j]
                    POS[r, j, 1] = rest[r, j, 1] + ny_ * U[r, j]
            # --- контакты струна-струна (Hunt-Crossley, F>=0)
            P = pairs.shape[0]
            for p in range(P):
                r0 = pairs[p, 0]
                c0 = pairs[p, 1]
                r1 = pairs[p, 2]
                c1 = pairs[p, 3]
                dx = POS[r0, c0, 0] - POS[r1, c1, 0]
                dy = POS[r0, c0, 1] - POS[r1, c1, 1]
                dist = math.sqrt(dx * dx + dy * dy)
                if dist < 1e-6:
                    dist = 1e-6
                dc = pair_rmin[p] - dist
                if dc > 0.0:
                    nx = dx / dist
                    ny = dy / dist
                    s0 = V[r0, c0]
                    s1 = V[r1, c1]
                    p0 = nrm[r0, 0] * nx + nrm[r0, 1] * ny
                    p1 = nrm[r1, 0] * nx + nrm[r1, 1] * ny
                    vrel = -(s0 * p0 - s1 * p1)
                    Fm = dc ** 1.5 * (pair_kc[p] + pair_kcchi[p] * vrel)
                    if Fm < 0.0:
                        Fm = 0.0
                    F[r0, c0] += Fm * p0
                    F[r1, c1] -= Fm * p1
                    touched += 1
            # --- смычок (до общего интегрирования, формулы stage0)
            bow_val = 0.0
            if bowing:
                m1 = m[bow_r]
                Z1 = Z0[bow_r]
                Fs = bow_mult * 2.0 * Z1 * bow_vb
                vn = V[bow_r, bow_c]
                x = (bow_vb - vn) / bow_vf
                fr = Fs * (2.0 * x / (1.0 + x * x) + kd_c * math.tanh(x))
                f_ext = F[bow_r, bow_c]
                hn = bow_noise * Fs * noise[it]
                cv0 = bow_damp_c * Z1
                if abs(x) < 1.0:
                    num_ = f_ext + cv0 * bow_vb + fr - cv0 * vn + hn
                    den = 1.0 + dt * cv0 / m1
                    bow_val = (vn + dt * num_ / m1) / den
                else:
                    bow_val = vn + dt * (f_ext + fr + hn) / m1
            # --- полунеявный Эйлер
            for r in range(S):
                dom = dt_over_m[r]
                for j in range(N):
                    V[r, j] += dom * F[r, j]
            if bowing:
                V[bow_r, bow_c] = bow_val
            for r in range(S):
                for j in range(N):
                    U[r, j] += dt * V[r, j]
            # --- страж скорости каждые 4 шага
            if (it & 3) == 3:
                for r in range(S):
                    vmax = 0.0
                    for j in range(N):
                        av = V[r, j] if V[r, j] >= 0.0 else -V[r, j]
                        if av > vmax:
                            vmax = av
                    if vmax > v_guard:
                        if guards:
                            clip_cnt[r] += 1
                        for j in range(N):
                            if V[r, j] > v_guard:
                                V[r, j] = v_guard
                            elif V[r, j] < -v_guard:
                                V[r, j] = -v_guard
            # --- мостик: децимация до SR
            if (it % os_f) == os_f - 1:
                j2 = it // os_f
                l = 0.0
                rr = 0.0
                for r in range(S):
                    fb = k[r] * (U[r, 1] - U[r, 0])
                    l += fb * wL[r]
                    rr += fb * wR[r]
                outL[j2] = l
                outR[j2] = rr
        return touched


# ----------------------------------------------------------------------------
# Движок
# ----------------------------------------------------------------------------
class Engine:
    """Батч всех струн в массивах (S, N). Векторы в numpy, скаляры — python float."""

    def __init__(self, os_factor=4):
        self.os = int(os_factor)
        self.dt = 1.0 / (SR * self.os)
        self.S, self.N = MAX_STRINGS, N_NODES
        self.U = np.zeros((self.S, self.N))
        self.V = np.zeros((self.S, self.N))
        self.F = np.zeros((self.S, self.N))
        self.POS = np.zeros((self.S, self.N, 2))
        self.rest = np.zeros((self.S, self.N, 2))
        self.nrm = np.zeros((self.S, 2))
        self.active = np.zeros(self.S, dtype=bool)
        self.k = np.zeros(self.S)
        self.m = np.zeros(self.S)
        self.Z0 = np.zeros(self.S)
        self.f0 = np.zeros(self.S)
        self.c = np.zeros(self.S)
        self.a = np.zeros(self.S)
        self.L = np.zeros(self.S)
        self.pan = np.full(self.S, 0.5)
        self.damp = np.zeros(self.S)          # скорость затухания, 1/с
        self.damp_f = np.zeros(self.S)        # та же сила на единицу скорости (damp*m), Н·с/м
        self.dt_over_m = np.zeros(self.S)

        # контакты
        self.pairs = np.zeros((0, 4), dtype=np.intp)   # r0,c0,r1,c1
        self.pair_kc = np.zeros(0)
        self.pair_kcchi = np.zeros(0)
        self.pair_rmin = np.zeros(0)
        self.touching = False
        self.touch_off_t = 0.0

        # параметры-ручки (общие, пишутся GUI-потоком, читаются здесь)
        self.params = {
            "R": 3.5,            # макро Regime 0..6
            "f0_new": 110.0,     # натяжение (f0) для НОВЫХ струн
            "gap": 2.0e-3,       # зазор контакта, м
            "kc": 1.2e5,         # жёсткость контакта
            "sigma_air": 0.30,   # воздушное демпфирование, 1/с
            "eta_cd": 0.0002,    # внутреннее демпфирование (доля c/a)
            "vb_trim": 1.0,      # трим скорости смычка
            "press_trim": 1.0,   # трим напора
            "master": 0.9,
            "guards": 1,         # 1 = guards включены
        }

        # смычок
        self.bowing = False
        self.bow_r, self.bow_c = 0, 1
        self.bow_vb, self.bow_mult, self.bow_vf = 0.1, 8.0, 0.02
        self.bow_beta, self.bow_noise = 0.1, 0.0
        self.bow_x0_warn_t = 0.0

        # guards / статистика
        self.vel_hist = np.zeros((self.S, 10), dtype=bool)
        self.frozen = np.zeros(self.S, dtype=bool)
        self.nan_times = []
        self._step_cnt = 0
        self._clip_cnt = np.zeros(self.S, dtype=np.int64)
        self.gain = 1e-3
        self.stats = {"cpu": 0.0, "xruns": 0, "blocks": 0}
        self.os_kc_clamped = 0
        self.use_jit = HAVE_NUMBA

        self.rebuild_pairs()

        # прогрев JIT-ядра (компиляция один раз, кэшируется между запусками)
        if self.use_jit:
            t0 = time.perf_counter()
            try:
                self.process_block(1)
                self.U[:] = 0.0
                self.V[:] = 0.0
                self.gain = 1e-3
                log("AUDIO", "JIT-ядро numba скомпилировано за %.1f с" %
                    (time.perf_counter() - t0))
            except Exception as e:
                self.use_jit = False
                log("WARN", "numba не заработала (%s) — медленный numpy-путь, "
                            "рекомендуется OS=2" % str(e)[:80])

    # ------------------------------------------------------------------ утилиты
    def _rate_limit(self, key, period):
        now = time.time()
        if now - getattr(self, "_rl_" + key, 0.0) >= period:
            setattr(self, "_rl_" + key, now)
            return True
        return False

    def recompute_damp(self):
        """Суммарный коэффициент демпфирования (1/с) на строку: воздух + внутреннее."""
        eta_rate = self.params["eta_cd"] * self.c / np.maximum(self.a, 1e-9)
        cap = 0.5 / self.dt
        over = eta_rate > cap
        if np.any(over & self.active):
            if self._rate_limit("etacap", 2.0):
                log("GUARD", "демпфирование зажато потолком устойчивости %.0f 1/с" % cap)
        eta_rate = np.minimum(eta_rate, cap)
        self.damp = self.params["sigma_air"] + eta_rate
        self.damp_f = self.damp * self.m

    def f0_max(self):
        """Макс. f0 из условия omega_max*dt <= 1.0 (omega_max = 4*f0*(N+1))."""
        return 1.0 / (4.0 * (self.N + 1) * self.dt)

    # ------------------------------------------------------------------ струны
    def spawn(self, spec):
        """spec: x0,y0,x1,y1 (м), f0, pan, token. Возвращает slot или None."""
        x0, y0, x1, y1 = spec["x0"], spec["y0"], spec["x1"], spec["y1"]
        f0 = float(spec.get("f0", self.params["f0_new"]))
        L = math.hypot(x1 - x0, y1 - y0)
        if L < 0.04:
            log("REJECT", "струна слишком короткая (%.1f см < 4 см) — не создана" % (L * 100))
            return None
        slot = int(np.nonzero(~self.active)[0][0]) if (~self.active).any() else -1
        if slot < 0:
            log("REJECT", "достигнут лимит %d струн — удали что-нибудь ластиком" % self.S)
            return None

        f0_cap = self.f0_max()
        clamped = ""
        if f0 > f0_cap:
            f0 = f0_cap
            clamped = " (зажата до %.0f Гц guard'ом omega*dt)" % f0

        a = L / (self.N + 1)
        m = MU * a
        c = 2.0 * L * f0
        k = m * (c / a) ** 2
        Z0 = MU * c
        w_max = 2.0 * c / a

        # кап жёсткости контакта для этой струны: (w_str^2 + 2kc/m)*dt^2 <= 1.2^2
        kc_max = max(0.0, ((1.2 / self.dt) ** 2 - w_max ** 2) * m / 2.0)

        dx, dy = x1 - x0, y1 - y0
        tx, ty = dx / L, dy / L
        idx = (np.arange(1, self.N + 1) * a)[:, None]
        self.rest[slot, :, 0] = x0 + tx * idx[:, 0]
        self.rest[slot, :, 1] = y0 + ty * idx[:, 0]
        self.nrm[slot, 0] = -ty
        self.nrm[slot, 1] = tx

        self.L[slot], self.a[slot], self.m[slot] = L, a, m
        self.f0[slot], self.c[slot], self.k[slot], self.Z0[slot] = f0, c, k, Z0
        self.pan[slot] = min(max(float(spec.get("pan", 0.5)), 0.0), 1.0)
        self.dt_over_m[slot] = self.dt / m
        self.U[slot] = 0.0
        self.V[slot] = 0.0
        self.frozen[slot] = False
        self.active[slot] = True
        self.recompute_damp()
        self.rebuild_pairs()

        log("SPAWN",
            "струна #%d: f0=%.1f Гц%s, L=%.0f см, N=%d, k=%.3e Н/м, m=%.3e кг, "
            "Z0=%.3f кг/с | omega*dt=%.3f (<1.0 ok), kc_max=%.2e"
            % (slot, f0, clamped, L * 100, self.N, k, m, Z0, w_max * self.dt, kc_max))
        return slot

    def delete(self, slot):
        if 0 <= slot < self.S and self.active[slot]:
            f = self.f0[slot]
            self.active[slot] = False
            self.frozen[slot] = False
            self.U[slot] = 0.0
            self.V[slot] = 0.0
            self.k[slot] = self.m[slot] = self.Z0[slot] = self.c[slot] = 0.0
            if self.bowing and self.bow_r == slot:
                self.bow_off()
            self.rebuild_pairs()
            log("DELETE", "струна #%d (f0=%.1f Гц) удалена" % (slot, f))

    def clear_all(self):
        n = int(self.active.sum())
        self.active[:] = False
        self.frozen[:] = False
        self.U[:] = 0.0
        self.V[:] = 0.0
        self.k[:] = self.m[:] = self.Z0[:] = self.c[:] = 0.0
        self.bow_off()
        self.rebuild_pairs()
        log("CLEAR", "сцена очищена (%d струн)" % n)

    def retension_all(self, f0):
        """Применить натяжение f0 ко всем активным струнам (k пересчитывается)."""
        f0_cap = self.f0_max()
        n = 0
        for r in np.nonzero(self.active)[0]:
            f = min(float(f0), f0_cap)
            L = self.L[r]
            c = 2.0 * L * f
            self.f0[r] = f
            self.c[r] = c
            self.Z0[r] = MU * c
            self.k[r] = self.m[r] * (c / self.a[r]) ** 2
            n += 1
        self.recompute_damp()
        self.rebuild_pairs()
        if n:
            log("PARAM", "натяжение применено ко всем: f0=%.0f Гц (струн: %d)" % (f0, n))

    def set_os(self, os_factor):
        os_factor = int(os_factor)
        if os_factor == self.os:
            return
        self.os = os_factor
        self.dt = 1.0 / (SR * self.os)
        self.dt_over_m[:] = self.dt / np.maximum(self.m, 1e-12)
        cap = self.f0_max()
        for r in np.nonzero(self.active)[0]:
            if self.f0[r] > cap:
                f = cap
                L = self.L[r]
                c = 2.0 * L * f
                self.f0[r] = f
                self.c[r] = c
                self.Z0[r] = MU * c
                self.k[r] = self.m[r] * (c / self.a[r]) ** 2
                log("GUARD", "струна #%d: f0 зажата до %.0f Гц (OS=%d)" % (r, f, self.os))
        self.recompute_damp()
        self.rebuild_pairs()
        log("PARAM", "оверсэмплинг OS=%d (dt=%.3e с, f0_max=%.0f Гц)" % (self.os, self.dt, cap))

    # ------------------------------------------------------------------ контакты
    def rebuild_pairs(self):
        """Кандидаты столкновений: пары узлов разных струн, чей покой ближе R_MIN+10мм."""
        gap = float(self.params["gap"])
        rmin = 2.0 * R_STR + gap
        act = list(np.nonzero(self.active)[0])
        rows = []
        kc_g = float(self.params["kc"])
        for ii in range(len(act)):
            for jj in range(ii + 1, len(act)):
                r0, r1 = int(act[ii]), int(act[jj])
                A, B = self.rest[r0], self.rest[r1]
                pA, pB = (A[0][0], A[0][1]), (A[-1][0], A[-1][1])
                qA, qB = (B[0][0], B[0][1]), (B[-1][0], B[-1][1])
                if seg_seg_dist(pA, pB, qA, qB) > rmin + 10e-3:
                    continue
                D = np.linalg.norm(A[:, None, :] - B[None, :, :], axis=2)
                ci, cj = np.nonzero(D < rmin + 10e-3)
                if ci.size == 0:
                    continue
                # дедуп по наборам (нужно для безопасного scatter-add)
                _, u = np.unique(ci, return_index=True)
                ci, cj = ci[u], cj[u]
                _, u = np.unique(cj, return_index=True)
                ci, cj = ci[u], cj[u]
                for a_, b_ in zip(ci, cj):
                    rows.append((r0, int(a_), r1, int(b_)))
        if rows:
            self.pairs = np.array(rows, dtype=np.intp)
            # капы на пару: устойчивость контактной пружины и демпфера
            m_min = np.minimum(self.m[self.pairs[:, 0]], self.m[self.pairs[:, 2]])
            w_str = np.maximum(
                2.0 * self.c[self.pairs[:, 0]] / np.maximum(self.a[self.pairs[:, 0]], 1e-9),
                2.0 * self.c[self.pairs[:, 2]] / np.maximum(self.a[self.pairs[:, 2]], 1e-9))
            kc_cap = np.maximum(0.0, ((1.2 / self.dt) ** 2 - w_str ** 2) * m_min / 2.0)
            self.pair_kc = np.minimum(kc_g, kc_cap)
            d_ref = 1.5e-3
            chi_cap = 0.8 * m_min / (self.dt * d_ref ** 1.5)
            self.pair_kcchi = np.minimum(CHI * kc_g, chi_cap)
            n_cl = int(np.sum(self.pair_kc > kc_cap).sum())
            if n_cl and self._rate_limit("kccap", 2.0):
                log("GUARD", "жёсткость контакта зажата до %.2e на %d парах (устойчивость)" %
                    (float(kc_cap.min()), n_cl))
        else:
            self.pairs = np.zeros((0, 4), dtype=np.intp)
            self.pair_kc = np.zeros(0)
            self.pair_kcchi = np.zeros(0)
        self.pair_rmin = np.full(self.pairs.shape[0], rmin)

    def _contacts(self):
        P = self.pairs.shape[0]
        if P == 0:
            if self.touching and time.time() - self.touch_off_t > 0.2:
                self.touching = False
                log("CONTACT", "контакт завершён")
            return
        pr0, pc0 = self.pairs[:, 0], self.pairs[:, 1]
        pr1, pc1 = self.pairs[:, 2], self.pairs[:, 3]
        P0 = self.POS[pr0, pc0]
        P1 = self.POS[pr1, pc1]
        dx = P0[:, 0] - P1[:, 0]
        dy = P0[:, 1] - P1[:, 1]
        dist = np.hypot(dx, dy)
        np.maximum(dist, 1e-6, out=dist)
        dc = self.pair_rmin - dist
        mask = dc > 0.0
        if not mask.any():
            if self.touching and time.time() - self.touch_off_t > 0.2:
                self.touching = False
                log("CONTACT", "контакт завершён")
            return
        idx = np.nonzero(mask)[0]
        dcm = dc[idx]
        nx = dx[idx] / dist[idx]
        ny = dy[idx] / dist[idx]
        s0 = self.V[pr0[idx], pc0[idx]]
        s1 = self.V[pr1[idx], pc1[idx]]
        nr0 = self.nrm[pr0[idx]]
        nr1 = self.nrm[pr1[idx]]
        p0 = nr0[:, 0] * nx + nr0[:, 1] * ny
        p1 = nr1[:, 0] * nx + nr1[:, 1] * ny
        vrel = -(s0 * p0 - s1 * p1)          # >0 = сближение
        Fm = dcm ** 1.5 * (self.pair_kc[idx] + self.pair_kcchi[idx] * vrel)
        np.maximum(Fm, 0.0, out=Fm)
        # отталкивание: узел 0 толкается вдоль n̂ (от узла 1 к узлу 0)
        f0s = Fm * p0
        f1s = -Fm * p1
        self.F[pr0[idx], pc0[idx]] += f0s
        self.F[pr1[idx], pc1[idx]] += f1s
        now = time.time()
        if not self.touching:
            self.touching = True
            log("CONTACT", "контакт начат: %d пар узлов, глубина до %.2f мм" %
                (idx.size, float(dcm.max()) * 1e3))
        self.touch_off_t = now

    # ------------------------------------------------------------------ смычок
    def bow_on(self, slot, beta):
        if not (0 <= slot < self.S and self.active[slot]):
            log("REJECT", "смычок: строка #%d не активна" % slot)
            return
        self.bow_r = int(slot)
        self.bow_c = int(min(max(round(beta * (self.N + 1)) - 1, 1), self.N - 2))
        self.bowing = True
        self._bow_update_schedule()
        x0 = self.bow_vb / self.bow_vf
        msg = ("смычок ON: струна #%d (f0=%.1f), beta=%.2f (узел %d), vb=%.3f м/с, "
               "mult=%.1f, vf=%.3f, x0=vb/vf=%.1f" %
               (slot, self.f0[slot], beta, self.bow_c, self.bow_vb, self.bow_mult,
                self.bow_vf, x0))
        if x0 < 2.0:
            msg += "  [WARN: x0<2 — рабочая точка не на падающей ветви, будет залипание]"
            log("WARN", msg)
        else:
            log("BOW", msg)

    def bow_move(self, beta):
        if self.bowing:
            self.bow_c = int(min(max(round(beta * (self.N + 1)) - 1, 1), self.N - 2))

    def bow_off(self):
        if self.bowing:
            self.bowing = False
            log("BOW", "смычок OFF (струна #%d)" % self.bow_r)

    def _bow_update_schedule(self):
        vb, mult, vf, beta, noise = sched_params(
            self.params["R"], self.params["vb_trim"], self.params["press_trim"])
        self.bow_vb, self.bow_mult, self.bow_vf = vb, mult, vf
        self.bow_noise = noise

    def _bow_calc(self):
        """Возвращает (row, col, v_new) или None. Формулы stage0 (валидированы)."""
        r, c = self.bow_r, self.bow_c
        if not self.active[r] or self.frozen[r]:
            return None
        m1 = float(self.m[r])
        Z1 = float(self.Z0[r])
        Fs = self.bow_mult * 2.0 * Z1 * self.bow_vb
        vn = float(self.V[r, c])
        x = (self.bow_vb - vn) / self.bow_vf
        fr = Fs * (2.0 * x / (1.0 + x * x) + KD * math.tanh(x))
        f_ext = float(self.F[r, c])
        hn = self.bow_noise * Fs * 2.0 * (random.random() - 0.5)
        cv0 = BOW_DAMP * Z1
        if abs(x) < 1.0:
            # неявная линейная часть около прилипания (иначе схема нестабильна)
            num = f_ext + cv0 * self.bow_vb + fr - cv0 * vn + hn
            den = 1.0 + self.dt * cv0 / m1
            return r, c, (vn + self.dt * num / m1) / den
        return r, c, vn + self.dt * (f_ext + fr + hn) / m1

    # ------------------------------------------------------------------ guards
    def freeze_row(self, r, reason):
        if self.frozen[r]:
            return
        self.frozen[r] = True
        self.V[r] = 0.0
        self.U[r] *= 0.1
        if self.bowing and self.bow_r == r:
            self.bow_off()
        log("GUARD", "струна #%d (f0=%.1f) АВТО-ЗАМОРОЗКА: %s" % (r, self.f0[r], reason))

    def unfreeze_all(self):
        self.frozen[:] = False

    def panic(self, auto=False):
        self.V[:] = 0.0
        self.U[:] *= 0.0
        self.frozen[:] = False
        self.vel_hist[:] = False
        log("PANIC", "АВАРИЙНЫЙ СБРОС%s: все скорости обнулены" %
            (" (авто)" if auto else ""))

    # ------------------------------------------------------------------ шаг
    def _step(self):
        F = self.F
        F.fill(0.0)
        # пружины: лапласиан цепочки
        lap = self.U[:, 2:] + self.U[:, :-2]
        lap -= 2.0 * self.U[:, 1:-1]
        F[:, 1:-1] += self.k[:, None] * lap
        # демпфирование (воздух + внутреннее): сила = rate*m*v -> затухание rate, 1/с
        F -= self.damp_f[:, None] * self.V
        # позиции узлов (для контактов)
        np.multiply(self.nrm[:, None, :], self.U[:, :, None], out=self.POS)
        self.POS += self.rest
        # контакты
        self._contacts()
        # смычок: считаем новое v ДО общего интегрирования (как в stage0)
        bow_val = self._bow_calc() if self.bowing else None
        # полунеявный Эйлер
        self.V += self.dt_over_m[:, None] * F
        if bow_val is not None:
            self.V[bow_val[0], bow_val[1]] = bow_val[2]
        self.U += self.dt * self.V
        # страж скорости (каждые 4 шага, работает и при выключенных guards)
        self._step_cnt += 1
        if (self._step_cnt & 3) == 0:
            vr = np.abs(self.V).max(axis=1)
            if (vr > V_GUARD).any():
                np.clip(self.V, -V_GUARD, V_GUARD, out=self.V)
                if self.params["guards"]:
                    bad = (vr > V_GUARD) & self.active & ~self.frozen
                    if bad.any():
                        self._clip_cnt[bad] += 1
                        if self._rate_limit("velclip", 1.0):
                            log("GUARD", "клип скорости %.1f м/с на строках %s" %
                                (V_GUARD, list(np.nonzero(bad)[0])))

    def _block_guards(self):
        # NaN/Inf страж
        bad_nan = ~np.isfinite(self.V).all(axis=1) | ~np.isfinite(self.U).all(axis=1)
        bad_nan &= self.active
        if bad_nan.any():
            for r in np.nonzero(bad_nan)[0]:
                self.U[r] = 0.0
                self.V[r] = 0.0
                self.nan_times.append(time.time())
                log("NAN", "NaN/Inf на струне #%d — обнулена" % r)
            self.nan_times = [t for t in self.nan_times if time.time() - t < 5.0]
            if len(self.nan_times) >= 3:
                self.panic(auto=True)
                self.nan_times = []
        # авто-заморозка по повторяющемуся клипу скорости
        if self.params["guards"]:
            trip = self.vel_hist.sum(axis=1) >= 3
            trip &= self.active & ~self.frozen
            for r in np.nonzero(trip)[0]:
                self.freeze_row(int(r), "скорость упиралась в клип %d+ раз за 10 блоков"
                                % int(self.vel_hist[r].sum()))
            self.frozen &= self.active
            for r in np.nonzero(self.frozen & self.active)[0]:
                # замороженная струна гаснет плавно
                self.U[r] *= 0.999

    # ------------------------------------------------------------------ блок
    def process_block(self, nsamp=BLOCK):
        self._clip_cnt[:] = 0
        if self.use_jit:
            outL, outR, touched = self._process_block_jit(nsamp)
        else:
            outL, outR, touched = self._process_block_numpy(nsamp)
        # история клипов (за блок) для авто-заморозки
        self.vel_hist[:, 1:] = self.vel_hist[:, :-1]
        self.vel_hist[:, 0] = self._clip_cnt > 0
        self._touch_log(touched > 0)
        self._block_guards()
        # AGC + мягкий клип + мастер
        peak = max(float(np.abs(outL).max()), float(np.abs(outR).max()), 1e-12)
        gt = min(max(0.22 / peak, 1e-6), 1e3)
        self.gain += (gt - self.gain) * (0.2 if gt < self.gain else 0.01)
        g = self.gain * float(self.params["master"])
        np.multiply(outL, g, out=outL)
        np.multiply(outR, g, out=outR)
        np.tanh(1.3 * outL, out=outL)
        np.tanh(1.3 * outR, out=outR)
        outL *= 0.85
        outR *= 0.85
        self.stats["blocks"] += 1
        return outL, outR

    def _touch_log(self, now_touching):
        now = time.time()
        if now_touching:
            if not self.touching:
                log("CONTACT", "контакт начат")
            self.touching = True
            self.touch_off_t = now
        elif self.touching and now - self.touch_off_t > 0.2:
            self.touching = False
            log("CONTACT", "контакт завершён")

    def _process_block_jit(self, nsamp=BLOCK):
        outL = np.empty(nsamp)
        outR = np.empty(nsamp)
        noise = (np.random.random(nsamp * self.os) - 0.5) * 2.0
        touched = _block_kernel(
            self.U, self.V, self.F, self.POS, self.rest, self.nrm,
            self.k, self.m, self.Z0, self.damp_f, self.dt_over_m,
            self.pairs, self.pair_kc, self.pair_kcchi, self.pair_rmin,
            self.bowing, self.bow_r, self.bow_c, self.bow_vb, self.bow_mult,
            self.bow_vf, self.bow_noise,
            noise, outL, outR, 1.0 - self.pan, self.pan,
            self.dt, self.os, 1 if self.params["guards"] else 0, V_GUARD,
            KD, BOW_DAMP, self._clip_cnt)
        return outL, outR, touched

    def _process_block_numpy(self, nsamp=BLOCK):
        outL = np.empty(nsamp)
        outR = np.empty(nsamp)
        wL = 1.0 - self.pan
        wR = self.pan
        n_in = nsamp * self.os
        touched = 0
        for it in range(n_in):
            self._step()
            if (it % self.os) == self.os - 1:
                j = it // self.os
                fb = self.k * (self.U[:, 1] - self.U[:, 0])   # сила на мостик
                outL[j] = float(fb @ wL)
                outR[j] = float(fb @ wR)
        if self.pairs.shape[0] and self.touching:
            touched = 1
        return outL, outR, touched

    # ------------------------------------------------------------------ рендер
    def render_wav(self, seconds, path, R=None, bow_slot=None, beta=0.12):
        """Оффлайн-рендер: смычок по одной струне (или без него), файл WAV 16 бит."""
        U0, V0 = self.U.copy(), self.V.copy()
        bow_was = self.bowing
        self.bow_off()
        if R is not None:
            self.params["R"] = float(R)
        if bow_slot is not None and self.active[bow_slot]:
            self.bow_on(int(bow_slot), beta)
        nblocks = int(seconds * SR / BLOCK)
        chunks = []
        log("RENDER", "рендер %d с: R=%.1f, струна=%s" %
            (seconds, self.params["R"],
             "нет смычка" if not self.bowing else "#%d" % self.bow_r))
        for _ in range(nblocks):
            l, r = self.process_block(BLOCK)
            chunks.append(np.stack([l, r], axis=1))
        self.bow_off()
        self.U, self.V = U0, V0
        audio = np.concatenate(chunks, axis=0)
        audio = np.clip(audio, -1.0, 1.0)
        pcm = (audio * 32767.0).astype(np.int16)
        try:
            with wave.open(path, "wb") as w:
                w.setnchannels(2)
                w.setsampwidth(2)
                w.setframerate(SR)
                w.writeframes(pcm.tobytes())
            log("RENDER", "готово: %s (%.1f с, пик %.2f)" %
                (os.path.basename(path), seconds, float(np.abs(audio).max())))
            return path
        except OSError as e:
            log("RENDER", "не удалось записать WAV: %s" % e)
            return None

    # ------------------------------------------------------------------ события
    def handle(self, ev):
        """События из очереди GUI (выполняется в потоке симуляции)."""
        kind = ev[0]
        if kind == "spawn":
            slot = self.spawn(ev[1])
            if slot is not None and len(ev) > 2:
                q, tok = ev[2], ev[1].get("token", -1)
                q.put(("spawned", tok, slot))
        elif kind == "delete":
            self.delete(int(ev[1]))
        elif kind == "clear":
            self.clear_all()
        elif kind == "pluck":
            _, slot, along, amp = ev
            if 0 <= slot < self.S and self.active[slot] and not self.frozen[slot]:
                c = int(min(max(round(along * (self.N + 1)) - 1, 2), self.N - 3))
                # плавная полуволна-бугор на 5 узлов
                for off in (-2, -1, 0, 1, 2):
                    cc = min(max(c + off, 0), self.N - 1)
                    self.U[slot, cc] += amp * 0.5 * (1 + math.cos(math.pi * off / 2.5))
                log("EXCITE", "щипок струны #%d, beta=%.2f, +%.1f мм" %
                    (slot, along, amp * 1e3))
        elif kind == "bow_on":
            self.bow_on(int(ev[1]), float(ev[2]))
        elif kind == "bow_move":
            self.bow_move(float(ev[1]))
        elif kind == "bow_off":
            self.bow_off()
        elif kind == "retension":
            self.retension_all(float(ev[1]))
        elif kind == "os":
            self.set_os(int(ev[1]))
        elif kind == "panic":
            self.panic()
        elif kind == "unfreeze":
            self.unfreeze_all()
            log("PARAM", "заморозка снята со всех струн")
        elif kind == "render":
            self.render_wav(ev[1], ev[2])
        elif kind == "rebuild_pairs":
            self.rebuild_pairs()

    # ------------------------------------------------------------------ отладка
    def scene_state(self):
        """Для GUI: позиции узлов активных строк (S,N,2) + флаги."""
        return self.POS.copy(), self.active.copy(), self.frozen.copy(), \
            self.bowing, (self.bow_r, self.bow_c)
# ----------------------------------------------------------------------------
# Кольцевой буфер + аудио-выход + поток симуляции
# ----------------------------------------------------------------------------
class RingBuffer:
    def __init__(self, frames=RING):
        self.buf = np.zeros((frames, 2))
        self.n = frames
        self.wi = 0
        self.ri = 0

    def write(self, chunk):
        n = chunk.shape[0]
        end = (self.wi + n) % self.n
        if end >= self.wi:
            self.buf[self.wi:end] = chunk
        else:
            first = self.n - self.wi
            self.buf[self.wi:] = chunk[:first]
            self.buf[:end] = chunk[first:]
        self.wi = end

    def read(self, frames):
        avail = (self.wi - self.ri) % self.n
        if avail < frames:
            return None
        out = np.empty((frames, 2))
        end = (self.ri + frames) % self.n
        if end >= self.ri:
            out[:] = self.buf[self.ri:end]
        else:
            first = self.n - self.ri
            out[:first] = self.buf[self.ri:]
            out[first:] = self.buf[:end]
        self.ri = end
        return out

    def drop_old(self):
        """Сбросить накопившееся (иначе при старте/рендере будет отставание)."""
        self.ri = self.wi

    def peek(self, frames):
        """Последние frames кадров БЕЗ потребления (для GUI-скопа)."""
        avail = (self.wi - self.ri) % self.n
        frames = min(frames, avail)
        if frames <= 0:
            return None
        start = (self.wi - frames) % self.n
        if start + frames <= self.n:
            return self.buf[start:start + frames].copy()
        return np.concatenate([self.buf[start:], self.buf[:self.wi]], axis=0)


def try_open_audio(ring):
    """Открыть выходное аудио; при неудаче — визуальный режим."""
    try:
        import sounddevice as sd
    except Exception as e:
        log("AUDIO", "sounddevice не установлен (%s) — режим без звука (рендер в WAV работает)" % e)
        return None

    def cb(outdata, frames, time_info, status):
        chunk = ring.read(frames)
        if chunk is None:
            outdata.fill(0)
            return
        outdata[:] = chunk

    try:
        dev = sd.default.device[1]
        name = sd.query_devices(dev)["name"] if dev is not None and dev >= 0 else "?"
        stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=2,
                                 dtype="float32", callback=cb)
        stream.start()
        log("AUDIO", "устройство: %s | %d Гц, блок %d" % (name, SR, BLOCK))
        return stream
    except Exception as e:
        log("AUDIO", "аудио-выход недоступен (%s) — режим без звука" % e)
        return None


class SimThread(threading.Thread):
    """Тянет движок блоками в реальном времени, пишет в кольцо, ест очередь событий."""

    def __init__(self, engine, ev_q, ui_q):
        super().__init__(daemon=True)
        self.engine = engine
        self.ev_q = ev_q
        self.ui_q = ui_q
        self.ring = RingBuffer()
        self.running = True
        self.period = BLOCK / SR
        self.next_t = time.perf_counter() + 0.2
        self.cpu = 0.0

    def run(self):
        eng = self.engine
        self.next_t = time.perf_counter() + 0.1
        while self.running:
            # события (без блокировки)
            try:
                while True:
                    ev = self.ev_q.get_nowait()
                    if ev[0] == "quit":
                        self.running = False
                    elif ev[0] == "get_scene":
                        self.ui_q.put(("scene", eng.scene_state(), eng.stats.copy(),
                                       dict(eng.params)))
                    else:
                        eng.handle(ev)
            except queue.Empty:
                pass

            t0 = time.perf_counter()
            l, r = eng.process_block(BLOCK)
            self.ring.write(np.stack([l, r], axis=1))
            busy = time.perf_counter() - t0
            self.cpu += 0.05 * (busy / self.period - self.cpu)
            eng.stats["cpu"] = max(0.0, self.cpu)

            self.next_t += self.period
            now = time.perf_counter()
            if now > self.next_t + 5 * self.period:
                # сильно отстали (рендер/сон системы) — ресинхронизация
                self.next_t = now + self.period
                eng.stats["xruns"] += 1
                if eng._rate_limit("xrun", 2.0):
                    log("XRUN", "симуляция не успевает (CPU %.0f%%) — щель в звуке; "
                                "попробуй OS=2 или меньше струн" % (self.cpu * 100))
            sleep_for = self.next_t - time.perf_counter()
            if sleep_for > 0.0015:
                time.sleep(sleep_for - 0.0012)
            while time.perf_counter() < self.next_t:
                pass
# ----------------------------------------------------------------------------
# GUI (Tkinter)
# ----------------------------------------------------------------------------
import tkinter as tk
import tkinter.font as tkfont
from tkinter import filedialog, messagebox

WORLD_W = 0.9          # ширина мира, м
COL_BG = "#14161a"
COL_PANEL = "#1b1e24"
COL_FG = "#d8dce2"
COL_STR = "#6ee7ff"
COL_STR_BOW = "#ffaa33"
COL_STR_FROZEN = "#666c78"
COL_PIN = "#e8ecf2"

LOG_TAGS = {
    "SPAWN": "#7fd7ff", "REJECT": "#ff8888", "BOW": "#ffd27f", "WARN": "#ffb020",
    "GUARD": "#ff9040", "NAN": "#ff5050", "PANIC": "#ff2020", "CONTACT": "#9fd49f",
    "EXCITE": "#c0a0ff", "PARAM": "#8a93a0", "AUDIO": "#8a93a0", "RENDER": "#9fd4ff",
    "XRUN": "#ffb020", "CLEAR": "#8a93a0", "DELETE": "#8a93a0",
}
MODES = [("РИСОВАТЬ", "рисуй линию ЛКМ — при отпускании появится струна"),
         ("СМЫЧОК", "зажми ЛКМ около струны и веди — это смычок"),
         ("ЩИПОК", "клик ЛКМ по струне — щипок"),
         ("ЛАСТИК", "клик ЛКМ по струне — удалить")]


def now_str():
    return time.strftime("%H:%M:%S")


def pick_fonts(root):
    """Выбрать шрифты с кириллицей (на Windows это будет Segoe UI)."""
    fams = {f.lower(): f for f in tkfont.families(root)}
    def first(*names):
        for n in names:
            if n.lower() in fams:
                return fams[n.lower()]
        return "TkDefaultFont"
    sans = first("Segoe UI", "Helvetica Neue", "Helvetica", "Noto Sans",
                 "DejaVu Sans", "Arial", "Liberation Sans")
    mono = first("Consolas", "Menlo", "DejaVu Sans Mono", "Liberation Mono",
                 "Courier New")
    return sans, mono


GUI_SANS = "TkDefaultFont"
GUI_MONO = "TkFixedFont"


def fs(size, bold=False):
    return (GUI_SANS, size, "bold" if bold else "normal")


def fm(size, bold=False):
    return (GUI_MONO, size, "bold" if bold else "normal")


class Knob(tk.Canvas):
    """Ручка-кноб: тяни вертикально / колесо / двойной клик = сброс."""

    def __init__(self, master, name, label, lo, hi, default, fmt, on_change,
                 logscale=False, size=76):
        super().__init__(master, width=size, height=size + 30,
                         bg=COL_PANEL, highlightthickness=0)
        self.name, self.label = name, label
        self.lo, self.hi, self.default = lo, hi, default
        self.fmt, self.on_change, self.logscale = fmt, on_change, logscale
        self.u = self._to_u(default)
        self._drag_y = None
        self._size = size
        self.bind("<Button-1>", self._press)
        self.bind("<B1-Motion>", self._motion)
        self.bind("<ButtonRelease-1>", self._release)
        self.bind("<MouseWheel>", self._wheel)
        self.bind("<Button-4>", self._wheel)
        self.bind("<Button-5>", self._wheel)
        self.bind("<Double-Button-1>", self._reset)
        self._draw()

    def _to_u(self, v):
        v = min(max(v, self.lo), self.hi)
        if self.logscale:
            return math.log(v / self.lo) / math.log(self.hi / self.lo)
        return (v - self.lo) / (self.hi - self.lo)

    def _from_u(self, u):
        u = min(max(u, 0.0), 1.0)
        if self.logscale:
            return self.lo * (self.hi / self.lo) ** u
        return self.lo + (self.hi - self.lo) * u

    def value(self):
        return self._from_u(self.u)

    def _press(self, e):
        self._drag_y = e.y

    def _motion(self, e):
        if self._drag_y is not None:
            self.u -= (e.y - self._drag_y) * 0.006
            self._drag_y = e.y
            self._draw()
            self.on_change(self.name, self.value(), False)

    def _release(self, e):
        if self._drag_y is not None:
            self._drag_y = None
            self.on_change(self.name, self.value(), True)   # финальное значение -> лог

    def _wheel(self, e):
        d = -0.05 if getattr(e, "delta", 0) > 0 or e.num == 4 else 0.05
        self.u += d
        self._draw()
        self.on_change(self.name, self.value(), True)

    def _reset(self, e):
        self.u = self._to_u(self.default)
        self._draw()
        self.on_change(self.name, self.value(), True)

    def _draw(self):
        self.delete("all")
        s = self._size
        cx, cy, r = s // 2, s // 2, s // 2 - 8
        a0, sweep = 135, 270
        self.create_arc(cx - r, cy - r, cx + r, cy + r, start=a0, extent=sweep,
                        style="arc", outline="#3a4048", width=3)
        self.create_arc(cx - r, cy - r, cx + r, cy + r, start=a0,
                        extent=max(0.6, sweep * self.u), style="arc",
                        outline="#6ee7ff", width=3)
        ang = math.radians(a0 + sweep * self.u)
        x2 = cx + (r - 6) * math.cos(ang)
        y2 = cy - (r - 6) * math.sin(ang)
        self.create_oval(cx - 4, cy - 4, cx + 4, cy + 4, fill="#3a4048", outline="")
        self.create_line(cx, cy, x2, y2, fill=COL_FG, width=2)
        self.create_text(cx, s + 8, text=self.label, fill="#9aa3ad", font=fs(7))
        self.create_text(cx, s + 21, text=self.fmt(self.value()),
                         fill=COL_FG, font=fs(8, True))


class App:
    def __init__(self, engine, ev_q, ui_q, sim, no_audio):
        self.eng = engine
        self.ev_q, self.ui_q, self.sim = ev_q, ui_q, sim
        self.no_audio = no_audio
        self.mode = 0
        self.preview = None
        self.bow_slot = None

        self.root = tk.Tk()
        self.root.title("Физмод live — Этап 1  (MD-струны + смычок, без взрывов)")
        self.root.configure(bg=COL_BG)
        self.root.geometry("1300x860")
        global GUI_SANS, GUI_MONO
        GUI_SANS, GUI_MONO = pick_fonts(self.root)

        self._build_top()
        self._build_main()
        self._build_log()
        self._build_status()

        self.log_path = os.path.join(
            os.path.dirname(os.path.abspath(__file__)),
            "logs", "live_" + time.strftime("%Y%m%d_%H%MSS") + ".txt")
        os.makedirs(os.path.dirname(self.log_path), exist_ok=True)
        self._log_file = open(self.log_path, "a", encoding="utf-8")
        log("AUDIO", "лог пишется в %s" % self.log_path)

        self.root.protocol("WM_DELETE_WINDOW", self._close)
        self.root.bind("<KeyPress>", self._key)
        self.root.after(33, self._tick)

    # ------------------------------------------------------------------ виджеты
    def _build_top(self):
        bar = tk.Frame(self.root, bg=COL_PANEL)
        bar.pack(fill="x")
        self.mode_btns = []
        for i, (name, _) in enumerate(MODES):
            b = tk.Button(bar, text="%d %s" % (i + 1, name), relief="flat",
                          bg=COL_PANEL, fg=COL_FG, activebackground="#2a2f38",
                          font=fs(9, True),
                          command=lambda i=i: self.set_mode(i))
            b.pack(side="left", padx=3, pady=4)
            self.mode_btns.append(b)
        tk.Label(bar, bg=COL_PANEL, fg="#5a616b", text="   |   ").pack(side="left")
        tk.Label(bar, bg=COL_PANEL, fg="#9aa3ad", text="ОВЕРСЭМПЛИНГ").pack(side="left")
        self.os_var = tk.StringVar(value=str(self.eng.os))
        om = tk.OptionMenu(bar, self.os_var, "2", "4", command=self._os_change)
        om.configure(bg=COL_PANEL, fg=COL_FG, relief="flat", width=3)
        om.pack(side="left", padx=4)
        self.hint = tk.Label(bar, bg=COL_PANEL, fg="#7fd7ff", text="")
        self.hint.pack(side="right", padx=10)
        self.set_mode(0)

    def _build_main(self):
        mid = tk.Frame(self.root, bg=COL_BG)
        mid.pack(fill="both", expand=True)
        self.canvas = tk.Canvas(mid, bg=COL_BG, highlightthickness=0, cursor="crosshair")
        self.canvas.pack(side="left", fill="both", expand=True)
        for seq in ("<Button-1>", "<B1-Motion>", "<ButtonRelease-1>"):
            self.canvas.bind(seq, getattr(self, {"<Button-1>": "_c_press",
                                                 "<B1-Motion>": "_c_drag",
                                                 "<ButtonRelease-1>": "_c_release"}[seq]))
        self._build_panel(mid)

    def _build_panel(self, master):
        p = tk.Frame(master, bg=COL_PANEL, width=372)
        p.pack(side="right", fill="y")
        p.pack_propagate(False)
        tk.Label(p, bg=COL_PANEL, fg="#9aa3ad", text="РУЧКИ (тяни / колесо / 2×клик = сброс)",
                 font=fs(8)).pack(anchor="w", padx=8, pady=(6, 0))
        grid = tk.Frame(p, bg=COL_PANEL)
        grid.pack(anchor="w", padx=4)

        def kc(name, label, lo, hi, dflt, fmt, logscale=False, key=None, mul=1.0):
            idx = len(grid.winfo_children())

            def on_change(_n, v, final):
                self.eng.params[key or name] = v * mul
                if final:
                    log("PARAM", "%s = %s" % (label, fmt(v)))
                    if (key or name) in ("gap", "kc"):
                        self.ev_q.put(("rebuild_pairs",))
            kn = Knob(grid, name, label, lo, hi, dflt, fmt, on_change, logscale)
            kn.grid(row=idx // 3, column=idx % 3, padx=3, pady=2)
        kc("R", "Regime R", 0, 6, 3.5, lambda v: "%.1f" % v)
        kc("f0", "Натяжение f0, Гц", 55, 660, 110, lambda v: "%.0f" % v,
           logscale=True, key="f0_new")
        kc("vb", "Скорость ×", 0.3, 3, 1.0, lambda v: "%.2f" % v, key="vb_trim")
        kc("press", "Напор ×", 0.3, 3, 1.0, lambda v: "%.2f" % v, key="press_trim")
        kc("gap", "Зазор, мм", 0.5, 6, 2.0, lambda v: "%.1f" % v, key="gap", mul=1e-3)
        kc("kcv", "Контакт KC", 3e4, 5e5, 1.2e5, lambda v: "%.1e" % v,
           logscale=True, key="kc")
        kc("sig", "Воздух σ, 1/с", 0, 1.5, 0.30, lambda v: "%.2f" % v, key="sigma_air")
        kc("eta", "Внутр. демпф ×1e-3", 0, 1.0, 0.2, lambda v: "%.2f" % v,
           key="eta_cd", mul=1e-3)
        kc("vol", "Громкость", 0, 1.5, 0.9, lambda v: "%.2f" % v, key="master")

        btns = tk.Frame(p, bg=COL_PANEL)
        btns.pack(fill="x", padx=8, pady=4)

        def btn(text, cmd, bg="#242932", fg=COL_FG, r=0, c=0, bold=False):
            tk.Button(btns, text=text, relief="flat", bg=bg, fg=fg,
                      activebackground="#2f3540", font=fs(8, bold),
                      command=cmd).grid(row=r, column=c, sticky="we", padx=3, pady=3)
            btns.columnconfigure(c, weight=1)

        btn("PANIC — сброс", self._panic, bg="#7a1f1f", fg="#ffd9d9", bold=True, r=0, c=0)
        btn("Разморозить все", lambda: self.ev_q.put(("unfreeze",)), r=0, c=1)
        btn("f0 ко всем", self._retension, r=1, c=0)
        btn("Очистить струны", lambda: self.ev_q.put(("clear",)), r=1, c=1)
        btn("Рендер 8 с → WAV", self._render, r=2, c=0)
        btn("Сохранить лог…", self._save_log, r=2, c=1)

        # метры + скоп
        self.meters = tk.Canvas(p, width=352, height=70, bg=COL_PANEL,
                                highlightthickness=0)
        self.meters.pack(fill="x", padx=8, pady=4)
        self._scope_hist = np.zeros(256)

    def _build_log(self):
        f = tk.Frame(self.root, bg=COL_PANEL)
        f.pack(fill="x", side="bottom")
        self.log_text = tk.Text(f, height=11, bg="#101216", fg=COL_FG, relief="flat",
                                font=fm(8), state="disabled", wrap="none")
        self.log_text.pack(fill="x", padx=6, pady=(2, 4))
        for tag, col in LOG_TAGS.items():
            self.log_text.tag_configure(tag, foreground=col)
        self.log_text.tag_configure("PANIC", foreground="#ff2020",
                                    font=fm(8, True))

    def _build_status(self):
        self.status = tk.Label(self.root, bg=COL_BG, fg="#7a828c", anchor="w",
                               font=fm(8))
        self.status.pack(fill="x", side="bottom")

    # ------------------------------------------------------------------ события UI
    def set_mode(self, i):
        self.mode = i
        self._drop_preview()
        for j, b in enumerate(self.mode_btns):
            b.configure(bg=("#2f6f8f" if j == i else COL_PANEL),
                        fg=(COL_FG if j != i else "#ffffff"))
        self.hint.configure(text=MODES[i][1])

    def _os_change(self, val):
        self.ev_q.put(("os", int(val)))
        log("PARAM", "OS -> %s (применится на след. блоке)" % val)

    def _panic(self):
        self.ev_q.put(("panic",))

    def _retension(self):
        self.ev_q.put(("retension", self.eng.params["f0_new"]))

    def _render(self):
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            "render_%s.wav" % time.strftime("%H%M%S"))
        self.ev_q.put(("render", 8.0, path))
        log("RENDER", "поставлен в очередь (живой звук на время рендера замрёт)")

    def _save_log(self):
        path = filedialog.asksaveasfilename(defaultextension=".txt",
                                            initialfile="live_log.txt",
                                            filetypes=[("Текст", "*.txt")])
        if path:
            try:
                with open(path, "w", encoding="utf-8") as w:
                    w.write(self.log_text.get("1.0", "end"))
                messagebox.showinfo("Лог сохранён", path)
            except OSError as e:
                messagebox.showerror("Ошибка", str(e))

    def _key(self, e):
        ch = getattr(e, "char", "")
        if ch in "1234":
            self.set_mode(int(ch) - 1)
        elif e.keysym == "Escape" and self.bow_slot is not None:
            self.ev_q.put(("bow_off",))
            self.bow_slot = None

    # ------------------------------------------------------------------ канвас
    def _scale(self):
        w = max(self.canvas.winfo_width(), 100)
        h = max(self.canvas.winfo_height(), 100)
        return w / WORLD_W, w, h

    def _to_px(self, x, y, s, w, h):
        return x * s, h - y * s

    def _to_m(self, px, py, s, w, h):
        return px / s, (h - py) / s

    def _active_strings(self):
        st = []
        eng = self.eng
        for r in range(eng.S):
            if eng.active[r]:
                p0 = eng.rest[r, 0]
                p1 = eng.rest[r, -1]
                st.append((r, (p0[0], p0[1]), (p1[0], p1[1])))
        return st

    def _c_press(self, e):
        s, w, h = self._scale()
        if self.mode == 0:                       # рисование
            self.preview = (e.x, e.y, e.x, e.y)
        elif self.mode in (1, 2):                # смычок / щипок
            hit = self._hit(e.x, e.y, s, w, h, 30)
            if hit is None:
                return
            slot, along = hit
            if self.mode == 1:
                self.bow_slot = slot
                self.ev_q.put(("bow_on", slot, along))
            else:
                self.ev_q.put(("pluck", slot, along, 2.5e-3))
        elif self.mode == 3:                     # ластик
            hit = self._hit(e.x, e.y, s, w, h, 14)
            if hit is not None:
                self.ev_q.put(("delete", hit[0]))

    def _c_drag(self, e):
        s, w, h = self._scale()
        if self.mode == 0 and self.preview:
            self.preview = (self.preview[0], self.preview[1], e.x, e.y)
        elif self.mode == 1 and self.bow_slot is not None:
            hit = self._hit(e.x, e.y, s, w, h, 45)
            if hit is not None:
                self.ev_q.put(("bow_move", hit[1]))
            else:
                self.ev_q.put(("bow_off",))
                self.bow_slot = None

    def _c_release(self, e):
        s, w, h = self._scale()
        if self.mode == 0 and self.preview:
            x0, y0 = self._to_m(self.preview[0], self.preview[1], s, w, h)
            x1, y1 = self._to_m(e.x, e.y, s, w, h)
            if math.hypot(e.x - self.preview[0], e.y - self.preview[1]) >= 40:
                pan = min(max((x0 + x1) / 2 / WORLD_W, 0.0), 1.0)
                self.ev_q.put(("spawn", {"x0": x0, "y0": y0, "x1": x1, "y1": y1,
                                         "f0": float(self.eng.params["f0_new"]),
                                         "pan": pan}))
            self._drop_preview()
        if self.mode == 1 and self.bow_slot is not None:
            self.ev_q.put(("bow_off",))
            self.bow_slot = None

    def _drop_preview(self):
        if self.preview:
            self.canvas.delete("preview")
            self.preview = None

    def _hit(self, px, py, s, w, h, tol_px):
        """Ближайшая струна: -> (slot, along 0..1) или None."""
        best, bslot, balong = tol_px, None, 0.0
        for r, a, b in self._active_strings():
            ax, ay = self._to_px(a[0], a[1], s, w, h)
            bx, by = self._to_px(b[0], b[1], s, w, h)
            dx, dy = bx - ax, by - ay
            L2 = dx * dx + dy * dy
            t = 0.0 if L2 < 1e-9 else ((px - ax) * dx + (py - ay) * dy) / L2
            t = min(max(t, 0.0), 1.0)
            d = math.hypot(px - (ax + t * dx), py - (ay + t * dy))
            if d < best:
                best, bslot, balong = d, r, min(max(t, 0.08), 0.92)
        return None if bslot is None else (bslot, balong)

    # ------------------------------------------------------------------ тик GUI
    def _tick(self):
        try:
            self._drain_log()
            self._draw_scene()
            self._draw_meters()
            self._update_status()
        except Exception as e:
            if not getattr(self, "_tick_dead", False):
                self._tick_dead = True
                log("PANIC", "ошибка отрисовки GUI: %s (интерфейс заморожен, звук живёт)" % e)
        self.root.after(33, self._tick)

    def _drain_log(self):
        lines = []
        try:
            while True:
                ts, ev, msg = LOG_Q.get_nowait()
                ms = int((ts % 1) * 1000)
                lines.append("%s.%03d  %-8s %s" % (time.strftime("%H:%M:%S", time.localtime(ts)), ms, ev, msg))
                tag = LOG_TAGS.get(ev, COL_FG)
                self.log_text.configure(state="normal")
                at_end = self.log_text.yview()[1] > 0.99
                self.log_text.insert("end", lines[-1] + "\n", ev if ev in LOG_TAGS else ())
                if at_end:
                    self.log_text.see("end")
                self.log_text.configure(state="disabled")
        except queue.Empty:
            pass
        if lines:
            self._log_file.write("\n".join(lines) + "\n")
            self._log_file.flush()

    def _draw_scene(self):
        c = self.canvas
        s, w, h = self._scale()
        c.delete("str", "pins", "preview", "bowpt")
        if self.preview:
            c.create_line(*self.preview, fill="#7fd7ff", dash=(4, 3), tags="preview")
        pos, active, frozen, bow_on, (br, bc) = self.eng.scene_state()
        for r, a, b in self._active_strings():
            col = COL_STR_FROZEN if frozen[r] else (COL_STR_BOW if (bow_on and br == r) else COL_STR)
            pts = []
            for j in range(self.eng.N):
                x, y = self._to_px(pos[r, j, 0], pos[r, j, 1], s, w, h)
                pts += [x, y]
            c.create_line(*pts, fill=col, width=3, tags="str")
            for (ex, ey) in (a, b):
                x, y = self._to_px(ex, ey, s, w, h)
                c.create_oval(x - 4, y - 4, x + 4, y + 4, fill=COL_PIN, outline="", tags="pins")
        if bow_on and active[br]:
            x, y = self._to_px(pos[br, bc, 0], pos[br, bc, 1], s, w, h)
            c.create_line(x - 12, y, x + 12, y, fill="#ffaa33", tags="bowpt")
            c.create_line(x, y - 12, x, y + 12, fill="#ffaa33", tags="bowpt")
            c.create_oval(x - 7, y - 7, x + 7, y + 7, outline="#ffaa33", tags="bowpt")

    def _draw_meters(self):
        m = self.meters
        m.delete("all")
        ring = getattr(self.sim, "ring", None)
        chunk = ring.peek(256) if ring is not None else None
        if chunk is not None:
            mono = chunk.mean(axis=1)
            self._scope_hist[:-len(mono)] = self._scope_hist[len(mono):]
            self._scope_hist[-len(mono):] = mono
        W, H = 352, 70
        m.create_rectangle(0, 0, W, H, fill="#101216", outline="")
        rms_l = float(np.sqrt(np.mean(self._scope_hist ** 2))) if self._scope_hist.any() else 0.0
        bar = min(1.0, rms_l * 3)
        m.create_rectangle(6, 6, 6 + int((W - 12) * bar), 20, fill="#5fd06f", outline="")
        m.create_text(10, 32, anchor="w", text="RMS", fill="#5a616b", font=fm(7))
        n = len(self._scope_hist)
        pts = []
        for i in range(0, n, 2):
            x = 6 + (W - 12) * i / n
            yv = self._scope_hist[i]
            pts += [x, H / 2 - yv * (H / 2 - 4)]
        if len(pts) >= 4:
            m.create_line(*pts, fill="#6ee7ff")

    def _update_status(self):
        eng = self.eng
        n = int(eng.active.sum())
        nf = int(eng.frozen.sum())
        st = eng.stats
        audio = "выкл" if self.no_audio else ("ok" if getattr(self, "_audio_ok", True) else "нет")
        self.status.configure(text="струн: %d/8  |  заморожено: %d  |  CPU сим: %.0f%%  |  "
                                   "xruns: %d  |  OS: %d  |  аудио: %s  |  смычок: %s"
                              % (n, nf, st["cpu"] * 100, st["xruns"], eng.os, audio,
                                 "ON" if eng.bowing else "off"))

    # ------------------------------------------------------------------ закрытие
    def _close(self):
        try:
            self.ev_q.put(("quit",))
            self._log_file.close()
        finally:
            self.root.destroy()


# ----------------------------------------------------------------------------
# main
# ----------------------------------------------------------------------------
def main():
    os_flag = 4
    no_audio = "--no-audio" in sys.argv
    for a in sys.argv:
        if a.startswith("--os="):
            os_flag = int(a.split("=")[1])

    engine = Engine(os_factor=os_flag)
    ev_q, ui_q = queue.Queue(), queue.Queue()
    sim = SimThread(engine, ev_q, ui_q)

    app = App(engine, ev_q, ui_q, sim, no_audio)

    log("AUDIO", "старт: OS=%d, dt=%.2e с, f0_max=%.0f Гц" %
        (engine.os, engine.dt, engine.f0_max()))
    log("SPAWN", "подсказка: режим 1 — нарисуй струну; режим 2 — веди смычок с зажатой ЛКМ")

    # демо-сцена: две струны как в stage0 (110 и 167.9 Гц), 4.5 мм между ними
    cy = WORLD_W * 0.55
    engine.spawn({"x0": 0.15, "y0": cy + 0.00225, "x1": 0.75, "y1": cy + 0.00225,
                  "f0": 110.0, "pan": 0.4})
    engine.spawn({"x0": 0.15, "y0": cy - 0.00225, "x1": 0.75, "y1": cy - 0.00225,
                  "f0": 167.9, "pan": 0.6})

    if not no_audio:
        stream = try_open_audio(sim.ring)
        app._audio_ok = stream is not None
    sim.start()
    app.mainloop()
    sim.running = False


if __name__ == "__main__":
    main()
