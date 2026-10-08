#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v3) — живой стенд физмод-синтеза
#   Режимы:
#     python bow_live.py          — интерактив: окно + звук
#     python bow_live.py diag     — показать аудио-устройства
#     python bow_live.py render   — офлайн-рендер 14 с -> bow_render.wav
#   Зависимости: pip install numpy sounddevice
#   (render работает и БЕЗ sounddevice)
# ============================================================
import os, sys, math, time, wave, datetime
import numpy as np

F32 = np.float32
SR, BLOCK = 44100, 512
N, L, MU = 48, 1.0, 0.04
M = 2 * N
C0, C1 = N // 3, 2 * N // 3
GAP, KC, CC, FCAP = 1.2e-3, 5.0e5, 0.5, 400.0
CST, FR = 1.0, 0.7
KG, CG = 300.0, 2.0
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)

P = dict(R=0.0, speed=1.0, press=1.0, noise=0.30, beta=0.08, vol=0.35,
         ca=0.15, bow=True, coll=True, automorph=True)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}

def build_system(ovs):
    dt = 1.0 / (SR * ovs)
    dx = L / (N + 1)
    def mk(f0):
        c = 2.0 * L * f0
        return MU * c * c / dx, MU * dx, MU * c
    kA, m, Z0 = mk(82.41)
    kB, _, _ = mk(82.41 * 1.4983)
    S = np.zeros((2 * M, 2 * M))
    i = np.arange(N)
    for off, kk in ((0, kA), (N, kB)):
        K = np.zeros((M, M)); D = np.zeros((M, M))
        K[i + off, i + off] = -2.0 * kk
        K[i[:-1] + off, i[1:] + off] = kk
        K[i[1:] + off, i[:-1] + off] = kk
        BR = 1e-4 * math.sqrt(kk * m)
        D[i + off, i + off] = 2.0 * BR
        D[i[:-1] + off, i[1:] + off] = -BR
        D[i[1:] + off, i[:-1] + off] = -BR
        a = dt / m
        S[:M, :M] -= a * D;      S[:M, M:] += a * K
        S[M:, :M] -= dt * a * D; S[M:, M:] += dt * a * K
    S[:M, :M] += np.eye(M); S[M:, :M] += dt * np.eye(M); S[M:, M:] += np.eye(M)
    S = S.astype(F32)
    w = 2.0 * math.sqrt(max(kA, kB) / m)
    assert w * dt < 1.0, f"нестабильно (w*dt={w*dt:.2f})"
    return dt, m, S, kA, kB, Z0

class ST: pass
ST.dt, ST.m, ST.S, ST.kA, ST.kB, ST.Z0 = build_system(1)
ST.s = np.zeros(2 * M, F32); ST.s2 = np.zeros(2 * M, F32)
ST.snap = np.zeros(M, F32)
ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0; ST.stick = 0.0
ST.rms_blk = 0.0; ST.gain = 0.05
ST.dcx = 0.0; ST.dcy = 0.0
ST.kick = False; ST.pluck = None; ST.grab = None
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

def make_block(frames):
    """один аудиоблок физики; состояние живёт в ST между блоками"""
    dt, m, S = ST.dt, ST.m, ST.S
    kA, kB, Z0 = ST.kA, ST.kB, ST.Z0
    s, s2 = ST.s, ST.s2
    ovs = round(1.0 / (SR * dt))
    dampf = F32(1.0 - dt * P["ca"])
    wdt = dt / m
    noise = RNG.standard_normal(frames * ovs).astype(F32)
    buf = np.empty(frames, dtype=F32)
    dcx, dcy = ST.dcx, ST.dcy
    if ST.kick:
        x = np.sin(np.pi * np.linspace(0, 1, N)).astype(F32) ** 2
        s[:N] += 0.6 * x; s[N:M] -= 0.5 * x
        ST.kick = False
    if ST.pluck is not None:
        j, amp = ST.pluck
        s[M + j] += F32(amp); ST.pluck = None
    stk = 0
    for i in range(frames):
        t = ST.t; ST.t += 1.0 / SR
        R = R_auto(t % 7.0) if P["automorph"] else P["R"]
        ST.R_now = R
        vb0 = (0.06 + 0.34 * R) * P["speed"]
        FN0 = (6.0 - 4.0 * R) * 2.0 * Z0 * vb0 * P["press"]
        vf = 0.012 + 0.10 * R
        jb = int(P["beta"] * N)
        nz = P["noise"]
        ST.on = min(1.0, ST.on + 1.0 / (SR * 0.03)) if P["bow"] \
            else max(0.0, ST.on - 1.0 / (SR * 0.08))
        on = ST.on
        gr = ST.grab; coll = P["coll"]
        for o in range(ovs):
            np.dot(S, s, out=s2)
            s2[:M] *= dampf
            if gr is not None:
                gi, tg = gr
                s2[gi] += wdt * (KG * (tg - s2[M + gi]) - CG * s2[gi])
            if coll:
                d = GAP + s2[M + N + C0:M + N + C1] - s2[M + C0:M + C1]
                if d.min() < 0.0:
                    dl = np.maximum(-d, 0.0)
                    dd = s2[C0:C1] - s2[N + C0:N + C1]
                    Fc = KC * dl ** 1.5 * (1.0 + CC * dd)
                    np.maximum(Fc, 0.0, out=Fc)
                    np.minimum(Fc, FCAP, out=Fc)
                    s2[C0:C1] -= wdt * Fc
                    s2[N + C0:N + C1] += wdt * Fc
            if on > 0.0:
                FN = FN0 * (1.0 + nz * float(noise[i * ovs + o])) * on
                if FN < 0.0: FN = 0.0
                vb_t = vb0 * on
                vold = float(s2[jb])
                vr = vb_t - vold
                cap = CST * FN * (FR + (1.0 - FR)
                      * math.exp(-abs(vr) / vf)) * wdt
                if abs(vr) <= cap:
                    s2[jb] = F32(vb_t)
                    s2[M + jb] += F32(dt * (vb_t - vold))
                    stk += 1
                else:
                    dv = math.copysign(cap, vr)
                    s2[jb] += F32(dv)
                    s2[M + jb] += F32(dt * dv)
            s, s2 = s2, s
    mix = kA * float(s[M]) + 0.8 * kB * float(s[M + N])
    x = mix - dcx + DCA * dcy
    ST.dcx, ST.dcy = mix, x
    buf[:] = x
    ST.s[:] = s
    ST.snap[:] = s[M:]
    ST.stick = 0.9 * ST.stick + 0.1 * stk / max(frames * ovs, 1)
    return buf

def post(buf):
    """автогромкость + мягкий лимитер"""
    rms = float(np.sqrt(np.mean(buf.astype(np.float64) ** 2))) + 1e-12
    ST.rms_blk = rms
    g_t = min(0.12 / rms, 3.0)
    ST.gain = min(max(ST.gain + (g_t - ST.gain)
                  * (0.25 if g_t < ST.gain else 0.015), 1e-4), 3.0)
    y = np.tanh(buf * F32(ST.gain * P["vol"] * 3.0))
    ST.level = 0.9 * ST.level + 0.1 * float(np.mean(np.abs(y)))
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status: ST.underr += 1
        if ST.pending_ovs is not None:
            ST.dt, ST.m, ST.S, ST.kA, ST.kB, ST.Z0 = build_system(ST.pending_ovs)
            ST.pending_ovs = None
        t0 = time.perf_counter()
        y = post(make_block(frames))
        ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0) / (frames / SR)
        outdata[:, 0] = y
        if ST.rec: ST.recl.append(y.copy())
    except Exception:
        import traceback; traceback.print_exc()

def render_wav(path, seconds=14.0):
    total = int(SR * seconds); pos = 0
    out = np.empty(total, F32)
    print(f"рендер {seconds:.0f} c офлайн...")
    while pos < total:
        n = min(BLOCK, total - pos)
        out[pos:pos + n] = post(make_block(n))
        pos += n
        if (pos // BLOCK) % (SR // BLOCK * 2) == 0:
            print(f"  {pos / SR:4.0f}/{seconds:.0f} c")
    x = out.astype(np.float64)
    nf = SR // 5
    x[-nf:] *= np.linspace(1.0, 0.0, nf)
    p = float(np.percentile(np.abs(x), 99.9)) + 1e-9
    x = np.clip(x * 0.85 / p, -1.0, 1.0)
    with wave.open(path, "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((x * 32767).astype("<i2").tobytes())
    print("ГОТОВО:", path)

# ------------------------------ GUI ------------------------------
def run_gui():
    import tkinter as tk
    from tkinter import ttk
    root = tk.Tk(); root.title("physmod live v3 — смычок + струны")
    W, H = 920, 360
    cv = tk.Canvas(root, width=W, height=H, bg="#101018", highlightthickness=0)
    cv.pack(fill="both", expand=True)
    XM0, XM1 = 40, W - 40
    ZOOM, B_OFF = 15000.0, 70
    B_BASE = 120            # струна B (сверху) — физически выше
    A_BASE = 120 + B_OFF    # струна A (снизу) — на ней смычок

    def draw():
        cv.delete("all")
        sn = ST.snap
        cv.create_rectangle(XM0 + C0 * (XM1 - XM0) // N, 20,
                            XM0 + C1 * (XM1 - XM0) // N, H - 20,
                            fill="#1a1a28", width=0)
        for off, base, col in ((N, B_BASE, "#7f7fd0"), (0, A_BASE, "#e8e8f0")):
            pts = []
            for j in range(N):
                pts += [XM0 + j * (XM1 - XM0) / (N - 1),
                        base - float(sn[off + j]) * ZOOM]
            cv.create_line(*pts, fill=col, width=2)
        jb = int(P["beta"] * N)
        xj = XM0 + jb * (XM1 - XM0) / (N - 1)
        yj = A_BASE - float(sn[jb]) * ZOOM
        cv.create_oval(xj - 5, yj - 5, xj + 5, yj + 5, fill="#ff5040", width=0)
        if ST.grab is not None:
            gi = ST.grab[0]
            base = A_BASE if gi < N else B_BASE
            j = gi if gi < N else gi - N
            xg = XM0 + j * (XM1 - XM0) / (N - 1)
            yg = base - float(sn[gi]) * ZOOM
            cv.create_oval(xg - 8, yg - 8, xg + 8, yg + 8,
                           outline="#ffd040", width=2)
        lv = min(1.0, ST.level * 4)
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
    qual.set("ЭКО")
    qual.bind("<<ComboboxSelected>>",
              lambda e: setattr(ST, "pending_ovs", OVS_MAP[qual.get()]))
    b_quit = tk.Button(buts, text="ВЫХОД", width=8, command=root.destroy)
    for b in (b_bow, b_kick, b_rec, b_auto, qual, b_quit):
        b.pack(side="left", padx=3, pady=4)

    status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")
    def tick():
        if P["automorph"]: sv["R"].set(ST.R_now)
        status.config(text=f"CPU {ST.cpu*100:4.0f}%  сбои {ST.underr}  "
                           f"сигнал {ST.rms_blk:6.3f}  стик {ST.stick*100:3.0f}%  "
                           f"R={ST.R_now:.2f}" + ("  ЗАПИСЬ..." if ST.rec else ""))
        root.after(200, tick)

    def pick(e):
        j = round((e.x - XM0) * (N - 1) / (XM1 - XM0))
        j = max(0, min(N - 1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j]) * ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N + j]) * ZOOM))
        i = j if dA <= dB else N + j
        base = A_BASE if i < N else B_BASE
        tg = (base - e.y) / ZOOM
        ST.grab = (i, max(-0.01, min(0.01, tg)))
    def pluck(e):
        pick(e)
        if ST.grab is not None:
            i = ST.grab[0]; ST.grab = None
            ST.pluck = (i, 0.008 * (1 if e.y < H // 2 else -1))
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", lambda e: ST.grab and pick(e))
    cv.bind("<ButtonRelease-1>", lambda e: setattr(ST, "grab", None))
    cv.bind("<Button-3>", pluck)
    cv.bind_all("<MouseWheel>", lambda e: sv["beta"].set(
        max(0.02, min(0.30, P["beta"] + 0.005 * (1 if e.delta > 0 else -1)))))
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<k>", lambda e: setattr(ST, "kick", True))
    root.protocol("WM_DELETE_WINDOW", root.destroy)

    print("""УПРАВЛЕНИЕ:
  ЛКМ — схватить и тянуть струну;  ПКМ — щипок;  колесо — двигать смычок
  ПРОБЕЛ — смычок вкл/выкл;  K — толчок (столкновение струн)
  АВТО-МОРФ — демо-цикл скрежет->бас->скрежет;  ЗАПИСЬ — jam_ЧЧММСС.wav
  Строка 'сигнал' в статусе: если 0.000 — физика молчит, пришли скрин статуса.
""")
    stream = None
    try:
        stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                                 callback=audio_cb, latency="low")
        stream.start()
    except Exception as e:
        print("АУДИО НЕ ОТКРЫЛОСЬ:", e)
        print("Файл можно услышать без окна:  python bow_live.py render")
    draw(); tick(); root.mainloop()
    if stream:
        stream.stop(); stream.close()

OUT_DIR = os.path.dirname(os.path.abspath(__file__))
try:
    import sounddevice as sd
    HAVE_SD = True
except Exception as _e:
    HAVE_SD = False
    _SD_ERR = _e

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    if "diag" in args:
        if not HAVE_SD:
            print("sounddevice не установлен:", _SD_ERR)
            return
        print(sd.query_devices()); print("default:", sd.default.device)
        return
    if "render" in args:
        render_wav(os.path.join(OUT_DIR, "bow_render.wav"))
        return
    if not HAVE_SD:
        print("Нет библиотеки sounddevice:", _SD_ERR)
        print("Установи:  pip install sounddevice")
        print("Или сделай WAV без окна:  python bow_live.py render")
        return
    run_gui()

if __name__ == "__main__":
    try:
        main()
    except SystemExit:
        raise
    except Exception:
        import traceback; traceback.print_exc()
    finally:
        try: input("\nEnter — закрыть...")
        except Exception: pass