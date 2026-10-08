#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v5)
#  Исправлено против v4:
#   1) чёрный экран: ST.snap не инициализировался -> draw() падал
#   2) «криповый» звук: DC-блокер работал раз в БЛОК (щелчки ~86 Гц),
#      теперь посэмплово внутри ядра физики
#   3) TypeError при отсутствии numba (дубль аргумента grab)
#   4) off-by-one в отрисовке/захвате второй струны
#   5) компиляция numba вынесена ДО старта звука и окна
# Зависимости: python -m pip install numpy sounddevice numba
# Режимы:  python bow_live.py | diag | render (WAV без GUI)
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np

SR, BLOCK = 44100, 512
N, L, MU = 48, 1.0, 0.04
NA = 2 * N + 3                      # 0=стена, 1..N=A, N+1=стена, N+2..2N+1=B, 2N+2=стена
C0, C1 = N // 3, 2 * N // 3
GAP, KC, CC, FCAP = 1.2e-3, 5.0e5, 0.5, 400.0
CST, FR = 1.0, 0.7
KG, CG = 300.0, 2.0
DCA = math.exp(-2.0 * math.pi * 25.0 / SR)
F0A, F0B = 82.41, 82.41 * 1.4983
DX = L / (N + 1); M0 = MU * DX
KA = MU * (2 * L * F0A) ** 2 / DX
KB = MU * (2 * L * F0B) ** 2 / DX
Z0 = MU * 2 * L * F0A
BRA = 1e-4 * math.sqrt(KA * M0)
BRB = 1e-4 * math.sqrt(KB * M0)

try:
    from numba import njit
    HAVE_NB = True
except Exception:
    HAVE_NB = False

CORE = None
if HAVE_NB:
    @njit(cache=True, fastmath=True)
    def block_nb(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
                 kA, kB, brA, brB, cam, im, z0, N, C0, C1, gap, kc, cc,
                 fcap, cst, fr, speed, press, nz, jb, coll,
                 grab_i, grab_t, kg, cg, dcs):
        dcx = dcs[0]; dcy = dcs[1]; dca = dcs[2]
        for i in range(frames):
            R = Rarr[i]; on = onarr[i]
            vb0 = (0.06 + 0.34 * R) * speed
            fn0 = (6.0 - 4.0 * R) * 2.0 * z0 * vb0 * press
            vf = 0.012 + 0.10 * R
            for o in range(ovs):
                for j in range(1, N + 1):
                    f = (kA * (yE[j-1] - 2.0*yE[j] + yE[j+1])
                         - brA * (vE[j-1] - 2.0*vE[j] + vE[j+1])
                         - cam * vE[j])
                    vE[j] += f * im
                for j in range(N + 2, 2*N + 2):
                    f = (kB * (yE[j-1] - 2.0*yE[j] + yE[j+1])
                         - brB * (vE[j-1] - 2.0*vE[j] + vE[j+1])
                         - cam * vE[j])
                    vE[j] += f * im
                if coll:
                    for j in range(C0, C1):
                        dl = -(gap + yE[N+2+j] - yE[1+j])
                        if dl > 0.0:
                            fc = kc * dl**1.5 * (1.0 + cc*(vE[1+j] - vE[N+2+j]))
                            if fc > 0.0:
                                if fc > fcap: fc = fcap
                                vE[1+j]   -= fc * im
                                vE[N+2+j] += fc * im
                if grab_i >= 0:
                    f = kg * (grab_t - yE[grab_i]) - cg * vE[grab_i]
                    vE[grab_i] += f * im
                if on > 0.0:
                    fn = fn0 * (1.0 + nz * noise[i*ovs+o]) * on
                    if fn < 0.0: fn = 0.0
                    vb_t = vb0 * on
                    vr = vb_t - vE[jb]
                    cap = cst * fn * (fr + (1.0-fr)*math.exp(-abs(vr)/vf)) * im
                    if abs(vr) <= cap:
                        vE[jb] = vb_t
                    else:
                        vE[jb] += math.copysign(cap, vr)
                for j in range(1, N + 1):
                    yE[j] += vE[j] * dt
                for j in range(N + 2, 2*N + 2):
                    yE[j] += vE[j] * dt
            mix = kA * yE[1] + 0.8 * kB * yE[N+2]
            x = mix - dcx + dca * dcy          # DC-блокер ПОСЭМПЛОВО
            dcx = mix; dcy = x
            out[i] = x
        dcs[0] = dcx; dcs[1] = dcy

def block_np(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
             kA, kB, brA, brB, cam, im, z0, N, C0, C1, gap, kc, cc,
             fcap, cst, fr, speed, press, nz, jb, coll,
             grab_i, grab_t, kg, cg, dcs):
    dcx, dcy, dca = dcs[0], dcs[1], dcs[2]
    sA = slice(1, N + 1); sB = slice(N + 2, 2*N + 2)
    for i in range(frames):
        R = Rarr[i]; on = onarr[i]
        vb0 = (0.06 + 0.34 * R) * speed
        fn0 = (6.0 - 4.0 * R) * 2.0 * z0 * vb0 * press
        vf = 0.012 + 0.10 * R
        for o in range(ovs):
            vE[sA] += im * (kA*(yE[0:N] - 2*yE[1:N+1] + yE[2:N+2])
                            - brA*(vE[0:N] - 2*vE[1:N+1] + vE[2:N+2])
                            - cam*vE[1:N+1])
            vE[sB] += im * (kB*(yE[N+1:2*N+1] - 2*yE[N+2:2*N+2] + yE[N+3:2*N+3])
                            - brB*(vE[N+1:2*N+1] - 2*vE[N+2:2*N+2] + vE[N+3:2*N+3])
                            - cam*vE[N+2:2*N+2])
            if coll:
                d = gap + yE[N+2+C0:N+2+C1] - yE[1+C0:1+C1]
                dl = np.maximum(-d, 0.0)
                fc = kc * dl**1.5 * (1.0 + cc*(vE[1+C0:1+C1] - vE[N+2+C0:N+2+C1]))
                np.clip(fc, 0.0, fcap, out=fc)
                vE[1+C0:1+C1] -= im * fc
                vE[N+2+C0:N+2+C1] += im * fc
            if grab_i >= 0:
                vE[grab_i] += im * (kg*(grab_t - yE[grab_i]) - cg*vE[grab_i])
            if on > 0.0:
                fn = fn0 * (1.0 + nz * float(noise[i*ovs+o])) * on
                if fn < 0.0: fn = 0.0
                vb_t = vb0 * on
                vr = vb_t - vE[jb]
                cap = cst * fn * (fr + (1.0-fr)*math.exp(-abs(vr)/vf)) * im
                if abs(vr) <= cap: vE[jb] = vb_t
                else: vE[jb] += math.copysign(cap, vr)
            yE[sA] += dt * vE[sA]
            yE[sB] += dt * vE[sB]
        mix = kA * yE[1] + 0.8 * kB * yE[N+2]
        x = mix - dcx + dca * dcy
        dcx = mix; dcy = x
        out[i] = x
    dcs[0] = dcx; dcs[1] = dcy

CORE = block_nb if HAVE_NB else block_np

P = dict(R=0.0, speed=1.0, press=1.0, noise=0.30, beta=0.08, vol=0.35,
         ca=0.15, bow=True, coll=True, automorph=True)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}

vE = np.zeros(NA); yE = np.zeros(NA)
dcs = np.array([0.0, 0.0, DCA])
ST = type("ST", (), {})()
ST.ovs = 1; ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0
ST.snap = np.zeros(2 * N)                 # <-- было пропущено (чёрный экран)
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0; ST.stick = 0.0
ST.rms_blk = 0.0; ST.gain = 0.05
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

def process_block(frames):
    if ST.pending_ovs is not None:
        ST.ovs = ST.pending_ovs; ST.pending_ovs = None
    ovs = ST.ovs; dt = 1.0 / (SR * ovs); im = dt / M0; cam = P["ca"] * M0
    idx = np.arange(frames)
    if P["automorph"]:
        Rarr = np.array([R_auto((ST.t + k / SR) % 7.0) for k in idx])
    else:
        Rarr = np.full(frames, P["R"])
    onarr = np.empty(frames)
    for k in range(frames):
        ST.on = (min(1.0, ST.on + 1.0/(SR*0.03)) if P["bow"]
                 else max(0.0, ST.on - 1.0/(SR*0.08)))
        onarr[k] = ST.on
    ST.t += frames / SR
    noise = RNG.standard_normal(frames * ovs)
    if ST.kick:
        x = np.sin(np.pi * np.linspace(0, 1, N)) ** 2
        vE[1:N+1] += 0.6 * x; vE[N+2:2*N+2] -= 0.5 * x
        ST.kick = False
    if ST.pluck is not None:
        j, amp = ST.pluck
        yE[j] += amp; ST.pluck = None
    if ST.grab is not None:
        i0, tg = ST.grab
        gi = i0 + 1 if i0 < N else i0 + 2      # A: i+1;  B: i+2
    else:
        gi, tg = -1, 0.0
    jb = int(P["beta"] * N) + 1
    out = np.empty(frames)
    t0 = time.perf_counter()
    CORE(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
         KA, KB, BRA, BRB, cam, im, Z0, N, C0, C1, GAP, KC, CC, FCAP,
         CST, FR, P["speed"], P["press"], P["noise"], jb, P["coll"],
         gi, tg, KG, CG, dcs)
    ST.cpu = 0.9*ST.cpu + 0.1*(time.perf_counter()-t0) / (frames/SR)
    ST.snap[:] = yE[1:2*N+2]
    stk_t = getattr(CORE, "_last_stick", None)
    rms = float(np.sqrt(np.mean(out*out))) + 1e-12
    ST.rms_blk = rms
    g_t = min(0.12 / rms, 3.0)
    ST.gain = min(max(ST.gain + (g_t-ST.gain)*(0.25 if g_t < ST.gain else 0.015),
                      1e-4), 3.0)
    y = np.tanh(out * (ST.gain * P["vol"] * 3.0))
    ST.level = 0.9*ST.level + 0.1*float(np.mean(np.abs(y)))
    return y

def audio_cb(outdata, frames, ti, status):
    try:
        if status: ST.underr += 1
        y = process_block(frames)
        outdata[:, 0] = y
        if ST.rec: ST.recl.append(y.copy())
    except Exception:
        import traceback; traceback.print_exc()
        outdata[:] = 0

def render_wav(path, seconds=14.0):
    total = int(SR * seconds); pos = 0
    out = np.empty(total)
    print(f"рендер {seconds:.0f} c" + ("" if HAVE_NB else " (медленно, без numba)"))
    while pos < total:
        n = min(BLOCK, total - pos)
        out[pos:pos+n] = process_block(n)
        pos += n
        if (pos // BLOCK) % (SR // BLOCK * 2) == 0:
            print(f"  {pos/SR:4.0f}/{seconds:.0f} c")
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
    from tkinter import ttk
    root = tk.Tk(); root.title("physmod live v5 — смычок + струны")
    W, H = 920, 360
    cv = tk.Canvas(root, width=W, height=H, bg="#101018", highlightthickness=0)
    cv.pack(fill="both", expand=True)
    XM0, XM1 = 40, W - 40
    ZOOM, B_OFF = 15000.0, 70
    B_BASE, A_BASE = 120, 190
    def draw():
        try:
            cv.delete("all")
            sn = ST.snap
            cv.create_rectangle(XM0 + C0*(XM1-XM0)//N, 20,
                                XM0 + C1*(XM1-XM0)//N, H-20,
                                fill="#1a1a28", width=0)
            for off, base, col in ((0, A_BASE, "#e8e8f0"), (N+1, B_BASE, "#7f7fd0")):
                pts = []
                for j in range(N):
                    pts += [XM0 + j*(XM1-XM0)/(N-1),
                            base - float(sn[off+j])*ZOOM]
                cv.create_line(*pts, fill=col, width=2)
            jb = int(P["beta"]*N)
            xj = XM0 + jb*(XM1-XM0)/(N-1); yj = A_BASE - float(sn[jb])*ZOOM
            cv.create_oval(xj-5, yj-5, xj+5, yj+5, fill="#ff5040", width=0)
            if ST.grab is not None:
                gi = ST.grab[0]
                base = A_BASE if gi < N else B_BASE
                j = gi if gi < N else gi - N
                sj = j if gi < N else N + 1 + j
                xg = XM0 + j*(XM1-XM0)/(N-1); yg = base - float(sn[sj])*ZOOM
                cv.create_oval(xg-8, yg-8, xg+8, yg+8, outline="#ffd040", width=2)
            lv = min(1.0, ST.level*4)
            cv.create_rectangle(20, H-18, 20+int((W-40)*lv), H-8,
                                fill="#40c080", width=0)
        except Exception as e:
            print("draw:", e)
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
                fn = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                  "jam_%s.wav"
                                  % datetime.datetime.now().strftime("%H%M%S"))
                with wave.open(fn, "wb") as w:
                    w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
                    w.writeframes((x*32767).astype("<i2").tobytes())
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
        try:
            if P["automorph"]: sv["R"].set(ST.R_now)
            advice = "" if HAVE_NB else "  [БЕЗ numba — тормозит, ставь numba]"
            status.config(text=f"CPU {ST.cpu*100:4.0f}%  сбои {ST.underr}  "
                               f"сигнал {ST.rms_blk:6.3f}  R={ST.R_now:.2f}"
                               + ("  ЗАПИСЬ..." if ST.rec else "") + advice)
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def pick(e):
        j = round((e.x - XM0)*(N-1)/(XM1-XM0)); j = max(0, min(N-1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j])*ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N+1+j])*ZOOM))
        i = j if dA <= dB else N + j
        base = A_BASE if i < N else B_BASE
        ST.grab = (i, max(-0.01, min(0.01, (base - e.y)/ZOOM)))
    def pluck(e):
        pick(e)
        if ST.grab is not None:
            i = ST.grab[0]; ST.grab = None
            j = i + 1 if i < N else i + 2
            ST.pluck = (j, 0.008 * (1 if e.y < H//2 else -1))
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", lambda e: ST.grab and pick(e))
    cv.bind("<ButtonRelease-1>", lambda e: setattr(ST, "grab", None))
    cv.bind("<Button-3>", pluck)
    cv.bind_all("<MouseWheel>", lambda e: sv["beta"].set(
        max(0.02, min(0.30, P["beta"] + 0.005*(1 if e.delta > 0 else -1)))))
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<k>", lambda e: setattr(ST, "kick", True))
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    print("""УПРАВЛЕНИЕ: ЛКМ — тянуть струну; ПКМ — щипок; колесо — смычок по струне;
ПРОБЕЛ — смычок; K — толчок (столкновение); АВТО-МОРФ — демо-цикл 7 с;
ЗАПИСЬ — jam_ЧЧММСС.wav. Смычок на нижней (белой) струне.""")
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                             callback=audio_cb, latency="low")
    stream.start()
    draw(); tick(); root.mainloop()
    stream.stop(); stream.close()

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__} | "
          f"numba: {'да' if HAVE_NB else 'НЕТ'}")
    if "diag" in args:
        print(sd.query_devices()); print("default:", sd.default.device); return
    if HAVE_NB:
        print("компиляция ядра (один раз, до ~20 c)...")
        t0 = time.time()
        process_block(64); process_block(64)
        vE[:] = 0.0; yE[:] = 0.0; dcs[0] = dcs[1] = 0.0
        ST.t = 0.0; ST.gain = 0.05
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
    print("\nНет sounddevice:  python -m pip install sounddevice")
    print("Без numba тоже запустится, но тормозит:  python -m pip install numba")
finally:
    try: input("\nEnter — закрыть...")
    except Exception: pass