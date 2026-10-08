#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE: интерактивный стенд физмод-синтеза (реалтайм)
#   та же физика, что в bow_proto.py (v4): смычок CORDIS-зажим,
#   контакт струн Hunt-Crossley, авто-морф скрежет->бас->скрежет
# Управление: см. таблицу в конце файла / консоли
# Зависимости: numpy + sounddevice  (pip install sounddevice)
# ============================================================
import os, math, time, wave, datetime
import numpy as np
import tkinter as tk
from tkinter import ttk

try:
    import sounddevice as sd
except Exception as e:
    print("Нет sounddevice. В терминале:  pip install sounddevice\n", e)
    raise SystemExit(1)

OUT_DIR = os.path.dirname(os.path.abspath(__file__))
SR, BLOCK = 44100, 512
N, L, MU = 72, 1.0, 0.04          # масс на струну, длина, плотность
M = 2 * N                          # обе струны в одном векторе
C0, C1, GAP = N // 3, 2 * N // 3, 1.2e-3
KC, CC, FCAP = 5.0e5, 0.5, 400.0
CST, FR = 1.0, 0.7                 # статика трения, глубина Stribeck-спада
KG, CG = 300.0, 2.0                # «рука»: пружина мыши
V_STICK = 2e-5

P = dict(R=0.0, speed=1.0, press=1.0, noise=0.30, beta=0.08, vol=0.35,
         ca=0.15, bow=True, coll=True, automorph=True)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}

def build_system(ovs):
    dt = 1.0 / (SR * ovs)
    def mk(f0):
        c = 2.0 * L * f0
        return MU * c * c / (L / (N + 1)), MU * (L / (N + 1)), MU * c
    kA, m, Z0 = mk(82.41)
    kB, _, _ = mk(82.41 * 1.4983)
    K = np.zeros((M, M)); D = np.zeros((M, M))
    for off, kk in ((0, kA), (N, kB)):
        i = np.arange(N)
        K[i + off, i + off] = -2.0 * kk
        K[i[:-1] + off, i[1:] + off] = kk
        K[i[1:] + off, i[:-1] + off] = kk
        BR = 1e-4 * math.sqrt(kk * m)
        D[i + off, i + off] = 2.0 * BR
        D[i[:-1] + off, i[1:] + off] = -BR
        D[i[1:] + off, i[:-1] + off] = -BR
    S = np.zeros((2 * M, 2 * M))
    S[:M, :M] = np.eye(M); S[:M, M:] = dt * np.eye(M)
    S[M:, :M] = (dt / m) * K
    S[M:, M:] = np.eye(M) - (dt / m) * D
    w = 2.0 * math.sqrt(max(kA, kB) / m)
    assert w * dt < 1.0, f"нестабильно w*dt={w*dt:.2f} — выбери качество выше"
    return dt, m, S

class ST: pass
ST.dt, ST.m, ST.S = build_system(2)
ST.s = np.zeros(2 * M); ST.s2 = np.zeros(2 * M)
ST.snap = np.zeros(M); ST.t = 0.0; ST.on = 0.0
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0; ST.stick = 0.0
ST.R_now = 0.0; ST.kick = False; ST.pluck = None
ST.grab = None; ST.rec = False; ST.pending_ovs = None
ST.recl = []
RNG = np.random.default_rng(7)
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)

def smoothstep(x):
    x = 0.0 if x < 0.0 else (1.0 if x > 1.0 else x)
    return x * x * (3.0 - 2.0 * x)

def R_auto(ph):                    # цикл 7 c: скрежет->бас->скрежет
    if ph < 1.2: return 0.0
    if ph < 2.7: return smoothstep((ph - 1.2) / 1.5)
    if ph < 4.2: return 1.0
    if ph < 5.7: return 1.0 - smoothstep((ph - 4.2) / 1.5)
    return 0.0

def audio_cb(outdata, frames, ti, status):
    try:
        if status: ST.underr += 1
        if ST.pending_ovs is not None:
            ST.dt, ST.m, ST.S = build_system(ST.pending_ovs)
            ST.pending_ovs = None
        dt, m, S, s, s2 = ST.dt, ST.m, ST.S, ST.s, ST.s2
        ovs = round(1.0 / (SR * dt))
        dampf = 1.0 - dt * P["ca"]
        wdt = dt / m
        noise = RNG.standard_normal(frames * ovs)
        buf = np.empty(frames)
        t0 = time.perf_counter()
        if ST.kick:
            x = np.sin(np.pi * np.linspace(0, 1, N)) ** 2
            s[M:M + N] += 0.6 * x; s[M + N:] -= 0.5 * x
            ST.kick = False
        if ST.pluck is not None:
            i, a = ST.pluck; s[M + i] += a; ST.pluck = None
        stk = 0
        for i in range(frames):
            t = ST.t; ST.t += 1.0 / SR
            R = R_auto(t % 7.0) if P["automorph"] else P["R"]
            ST.R_now = R
            vb0 = (0.06 + 0.34 * R) * P["speed"]
            FN0 = (6.0 - 4.0 * R) * 2.0 * Z0 * vb0 * P["press"]
            vf = 0.012 + 0.10 * R
            beta = P["beta"]; jb = int(beta * N)
            nz = P["noise"]
            ST.on = min(1.0, ST.on + 1.0 / (SR * 0.03)) if P["bow"] \
                else max(0.0, ST.on - 1.0 / (SR * 0.08))
            on = ST.on
            gr = ST.grab; coll = P["coll"]
            for o in range(ovs):
                np.dot(S, s, out=s2)
                s2[M:] *= dampf
                if gr is not None:                       # «рука»
                    gi, tg = gr
                    s2[M + gi] += wdt * (KG * (tg - s2[gi]) - CG * s2[M + gi])
                if coll:                                 # контакт струн
                    d = GAP + s2[N + C0:N + C1] - s2[C0:C1]
                    if d.min() < 0.0:
                        dl = np.maximum(-d, 0.0)
                        dd = s2[M + C0:M + C1] - s2[M + N + C0:M + N + C1]
                        Fc = KC * dl ** 1.5 * (1.0 + CC * dd)
                        np.maximum(Fc, 0.0, out=Fc)
                        np.minimum(Fc, FCAP, out=Fc)
                        s2[M + C0:M + C1] -= wdt * Fc
                        s2[M + N + C0:M + N + C1] += wdt * Fc
                if on > 0.0:                             # смычок-зажим
                    FN = FN0 * (1.0 + nz * noise[i * ovs + o]) * on
                    if FN < 0.0: FN = 0.0
                    vpre = s[M + jb]
                    vr = vb0 * on - vpre
                    cap = CST * FN * (1.0 - FR * math.exp(-abs(vr) / vf)) \
                        * wdt
                    dv = vb0 * on - s2[M + jb]
                    if abs(dv) <= cap:
                        s2[M + jb] = vb0 * on; stk += 1
                    else:
                        s2[M + jb] += math.copysign(cap, dv)
                s, s2 = s2, s
            mix = kA * s[0] + 0.8 * kB * s[N]
            buf[i] = mix - buf[i - 1] * -DCA if i else mix
        # dc-block корректно:
        # (упрощённый, но стабильный: см. ниже полный проход)
        y = buf * P["vol"]
        ST.level = 0.9 * ST.level + 0.1 * float(np.mean(np.abs(y)))
        outdata[:, 0] = np.tanh(y)
        ST.snap[:] = s[:M]
        ST.stick = 0.9 * ST.stick + 0.1 * stk / max(frames * ovs, 1)
        ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0) \
            / (frames / SR)
        if ST.rec:
            ST.recl.append(outdata[:, 0].copy())
    except Exception as e:
        import traceback; traceback.print_exc()

# ------------------------------ GUI ------------------------------
root = tk.Tk(); root.title("physmod live — смычок + струны")
W, H = 920, 340
cv = tk.Canvas(root, width=W, height=H, bg="#101018", highlightthickness=0)
cv.pack(fill="both", expand=True)
XM0, XM1 = 40, W - 40
ZOOM, B_OFF = 3500.0, 10

def draw():
    cv.delete("all")
    s = ST.snap
    midy, gapx = H // 2, H // 2 - 60
    cv.create_rectangle(XM0 + C0 * (XM1 - XM0) // N, 20,
                        XM0 + C1 * (XM1 - XM0) // N, H - 20,
                        fill="#1a1a28", width=0)
    for off, col in ((0, "#e8e8f0"), (N, "#7f7fd0")):
        pts = []
        for j in range(N):
            x = XM0 + j * (XM1 - XM0) / (N - 1)
            y = (gapx if off == 0 else gapx + B_OFF) - s[off + j] * ZOOM
            pts += [x, y]
        cv.create_line(*pts, fill=col, width=2)
    jb = int(P["beta"] * N)
    xj = XM0 + jb * (XM1 - XM0) / (N - 1)
    yj = gapx - s[jb] * ZOOM
    cv.create_oval(xj - 5, yj - 5, xj + 5, yj + 5, fill="#ff5040", width=0)
    if ST.grab is not None:
        gi = ST.grab[0]; off = 0 if gi < N else N
        xg = XM0 + (gi - off) * (XM1 - XM0) / (N - 1)
        yg = (gapx if off == 0 else gapx + B_OFF) - s[gi] * ZOOM
        cv.create_oval(xg - 8, yg - 8, xg + 8, yg + 8, outline="#ffd040",
                       width=2)
    lv = min(1.0, ST.level * 6)
    cv.create_rectangle(20, H - 18, 20 + int((W - 40) * lv), H - 8,
                        fill="#40c080", width=0)
    root.after(33, draw)

panel = tk.Frame(root); panel.pack(fill="x")
sliders = [("R  скрежет<->бас", "R", 0.0, 1.0),
           ("Скорость смычка", "speed", 0.0, 1.6),
           ("Прижим", "press", 0.0, 2.0),
           ("Шум волоса", "noise", 0.0, 1.0),
           ("Позиция смычка", "beta", 0.02, 0.30),
           ("Затухание", "ca", 0.02, 0.60),
           ("Громкость", "vol", 0.0, 1.0)]
sv = {}
for row, (lab, key, a, b) in enumerate(sliders):
    tk.Label(panel, text=lab, width=18, anchor="w").grid(row=row, column=0)
    var = tk.DoubleVar(value=P[key]); sv[key] = var
    ttk.Scale(panel, from_=a, to=b, variable=var,
              command=(lambda v, k=key: P.__setitem__(k, float(v)))
              ).grid(row=row, column=1, sticky="we")
panel.columnconfigure(1, weight=1)

buts = tk.Frame(root); buts.pack(fill="x")
def toggle_bow():
    P["bow"] = not P["bow"]
    b_bow.config(text="СМЫЧОК: ВКЛ" if P["bow"] else "СМЫЧОК: выкл")
b_bow = tk.Button(buts, text="СМЫЧОК: ВКЛ", width=14, command=toggle_bow)
b_kick = tk.Button(buts, text="ТОЛЧОК", width=10,
                   command=lambda: setattr(ST, "kick", True))
def rec_toggle():
    if not ST.rec:
        ST.rec = True; ST.recl = []; b_rec.config(text="ЗАПИСЬ: идёт")
    else:
        ST.rec = False; b_rec.config(text="ЗАПИСЬ")
        if ST.recl:
            x = np.concatenate(ST.recl)
            p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
            x = np.clip(x * 0.9 / p, -1, 1)
            fn = os.path.join(OUT_DIR, "jam_%s.wav"
                              % datetime.datetime.now().strftime("%H%M%S"))
            with wave.open(fn, "wb") as w:
                w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
                w.writeframes((x * 32767).astype("<i2").tobytes())
            print("записано:", fn)
b_rec = tk.Button(buts, text="ЗАПИСЬ", width=10, command=rec_toggle)
def auto_t():
    P["automorph"] = not P["automorph"]
    b_auto.config(text="АВТО-МОРФ: ВКЛ" if P["automorph"]
                  else "АВТО-МОРФ: выкл")
b_auto = tk.Button(buts, text="АВТО-МОРФ: ВКЛ", width=16, command=auto_t)
qual = ttk.Combobox(buts, values=list(OVS_MAP), width=7, state="readonly")
qual.set("НОРМА")
qual.bind("<<ComboboxSelected>>",
          lambda e: setattr(ST, "pending_ovs", OVS_MAP[qual.get()]))
b_quit = tk.Button(buts, text="ВЫХОД", width=8, command=root.destroy)
for c, w_ in ((b_bow, None), (b_kick, None), (b_rec, None), (b_auto, None),
              (qual, None), (b_quit, None)):
    w_.pack(side="left", padx=3, pady=4)

status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")
def tick():
    if P["automorph"]: sv["R"].set(ST.R_now)
    status.config(text=f"CPU {ST.cpu*100:4.0f}%   сбоев {ST.underr}   "
                       f"стик {ST.stick*100:3.0f}%   R={ST.R_now:.2f}"
                       + ("   ЗАПИСЬ..." if ST.rec else ""))
    root.after(200, tick)

def pick(e):
    j = round((e.x - XM0) * (N - 1) / (XM1 - XM0))
    j = max(0, min(N - 1, j))
    midy = H // 2 - 60
    dA = abs(e.y - (midy - ST.snap[j] * ZOOM))
    dB = abs(e.y - (midy + B_OFF - ST.snap[N + j] * ZOOM))
    i = j if dA <= dB else N + j
    off = 0 if i < N else N
    tg = ((midy if off == 0 else midy + B_OFF) - e.y) / ZOOM
    ST.grab = (i, max(-0.04, min(0.04, tg)))
cv.bind("<Button-1>", pick)
cv.bind("<B1-Motion>", lambda e: ST.grab and pick(e))
cv.bind("<ButtonRelease-1>", lambda e: setattr(ST, "grab", None))
def pluck(e):
    pick(e); i = ST.grab[0]; ST.grab = None
    ST.pluck = (i, 2.0 * (1 if e.y < H // 2 else -1))
cv.bind("<Button-3>", pluck)
cv.bind_all("<MouseWheel>", lambda e: sv["beta"].set(
    max(0.02, min(0.30, P["beta"] + 0.005 * (1 if e.delta > 0 else -1)))))
root.bind("<space>", lambda e: toggle_bow())
root.bind("<k>", lambda e: setattr(ST, "kick", True))
root.protocol("WM_DELETE_WINDOW", root.destroy)

print(__doc__)
print("""УПРАВЛЕНИЕ:
  ЛКМ на канвасе  — схватить массу струны и тянуть («рука»)
  ПКМ             — щипок
  колесо мыши     — двигать смычок по струне
  ПРОБЕЛ          — смычок вкл/выкл;  K — толчок
  АВТО-МОРФ       — цикл скрежет->бас->скрежет (демо R)
  ЗАПИСЬ          — пишет jam_ЧЧММСС.wav в папку скрипта
  КАЧЕСТВО        — ЭКО (если заикается) / НОРМА / МАКС
""")
try:
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK,
                             callback=audio_cb, latency="low")
    stream.start()
except Exception as e:
    print("Не удалось открыть аудио:", e); raise SystemExit(1)
draw(); tick(); root.mainloop()
stream.stop(); stream.close()