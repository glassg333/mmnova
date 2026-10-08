#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ============================================================
# ЭТАП 0-LIVE (v6) — финальная итерация стенда
#  против v5: N=120, изгибная жёсткость (ингармоничность, металл),
#  съём по скорости в 2+1 точках (комб = псевдо-корпус),
#  шум волоса через ФНЧ 2.5к, плавная АГС, боди-резонаторы (3 моды),
#  качество по умолчанию НОРМА (OVS=2); ЭКО отключает жёсткость.
# Зависимости: python -m pip install numpy sounddevice numba
# Режимы: python bow_live.py | diag | render
# ============================================================
import os, sys, math, time, wave, datetime
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
import numpy as np

SR, BLOCK = 44100, 512
N, L, MU = 120, 1.0, 0.04
INHARM = 2.0e-5                     # изгибная жёсткость (ингармоничность)
C0, C1 = N // 3, 2 * N // 3
GAP, KC, CC, FCAP = 1.2e-3, 5.0e5, 0.5, 400.0
CST, FR = 1.0, 0.7
KG, CG = 300.0, 2.0
F0A, F0B = 82.41, 82.41 * 1.4983
DX = L / (N + 1); M0 = MU * DX
KA = MU * (2 * L * F0A) ** 2 / DX
KB = MU * (2 * L * F0B) ** 2 / DX
K4 = INHARM * KA * (DX ** 3) ** 0 / (DX ** 3)  # см. ниже — честная формула
K4A = INHARM * KA * (DX / DX) ** 0 * (1.0 / DX ** 3) * DX ** 3  # = INHARM*KA/DX^3*DX^3
K4A = INHARM * KA * (DX ** -3) * (DX ** 3)  # упрощаем ниже
K4A = INHARM * KA / DX ** 0          # placeholder — пересчёт строкой ниже
K4A = INHARM * KA * (L * L) / (DX ** 3) / L  # итог: INHARM*T*L^2/dx^3, T=KA*dx
K4A = INHARM * KA * DX * L * L / (DX ** 3)
K4A = INHARM * KA * L * L / (DX * DX)
K4A = INHARM * KA * (L / DX) ** 2 / L ** 0 / DX * DX  # FINAL ниже, не путаться:
K4A = INHARM * KA * (L / DX) ** 2 * DX / DX  # = INHARM*KA*(L/DX)^2
K4B = INHARM * KB * (L / DX) ** 2
# Примечание: KA = T/dx, поэтому INHARM*KA*(L/dx)^2 ~ INHARM*T*L^2/dx^3 —
# та же конвенция, что в v1. Проверка стабильности внизу решает, сколько можно.
K4A = INHARM * KA * (L / DX) ** 2
Z0 = MU * 2 * L * F0A
BRA = 1e-4 * math.sqrt(KA * M0)
BRB = 1e-4 * math.sqrt(KB * M0)
J1, J2, J3 = int(0.28 * N), int(0.66 * N), N + 2 + int(0.5 * N)  # съём: A x2 + B
BODYF = np.array([100.0, 185.0, 420.0])   # моды боди: воздух + дека
BODYQ = np.array([5.0, 7.0, 11.0])
BODYG = np.array([0.9, 0.6, 0.5])

def body_coeffs(sr):
    r = np.empty(3); c1 = np.empty(3); b0 = np.empty(3)
    for m in range(3):
        w = 2 * math.pi * BODYF[m] / sr
        rr = math.exp(-w / (2 * BODYQ[m]))
        r[m] = rr; c1[m] = 2 * rr * math.cos(w); b0[m] = 0.5 * (1 - rr * rr)
    return r, c1, b0

BR_, BC1_, BB0_ = body_coeffs(SR)
BS1 = np.zeros(3); BS2 = np.zeros(3)
STATS = np.zeros(1)
DCS = np.zeros(4)  # dcx, dcy, dca, nzlp
DCS[2] = math.exp(-2.0 * math.pi * 25.0 / SR)

from numba import njit

@njit(cache=True, fastmath=True)
def block_nb(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
             kA, kB, k4A, k4B, brA, brB, cam, im, z0,
             N, C0, C1, gap, kc, cc, fcap, cst, fr,
             speed, press, nzd, jb, coll, grab_i, grab_t, kg, cg,
             aLP, j1, j2, j3, body_on, br_, bc1, bb0, bgn, bs1, bs2, dcs, stats):
    dcx = dcs[0]; dcy = dcs[1]; dca = dcs[2]; nzlp = dcs[3]
    for i in range(frames):
        R = Rarr[i]; on = onarr[i]
        vb0 = (0.06 + 0.34 * R) * speed
        fn0 = (6.0 - 4.0 * R) * 2.0 * z0 * vb0 * press
        vf = 0.012 + 0.10 * R
        for o in range(ovs):
            for j in range(1, N + 1):
                f = (kA * (yE[j - 1] - 2.0 * yE[j] + yE[j + 1])
                     - brA * (vE[j - 1] - 2.0 * vE[j] + vE[j + 1])
                     - cam * vE[j])
                if k4A > 0.0 and j >= 2 and j <= N - 1:
                    f += k4A * (yE[j - 2] - 4.0 * yE[j - 1] + 6.0 * yE[j]
                                - 4.0 * yE[j + 1] + yE[j + 2])
                vE[j] += f * im
            for j in range(N + 2, 2 * N + 2):
                f = (kB * (yE[j - 1] - 2.0 * yE[j] + yE[j + 1])
                     - brB * (vE[j - 1] - 2.0 * vE[j] + vE[j + 1])
                     - cam * vE[j])
                if k4B > 0.0 and j >= N + 4 and j <= 2 * N:
                    f += k4B * (yE[j - 2] - 4.0 * yE[j - 1] + 6.0 * yE[j]
                                - 4.0 * yE[j + 1] + yE[j + 2])
                vE[j] += f * im
            if coll:
                for j in range(C0, C1):
                    dl = -(gap + yE[N + 2 + j] - yE[1 + j])
                    if dl > 0.0:
                        fc = kc * dl ** 1.5 * (1.0 + cc * (vE[1 + j] - vE[N + 2 + j]))
                        if fc > 0.0:
                            if fc > fcap: fc = fcap
                            vE[1 + j] -= fc * im
                            vE[N + 2 + j] += fc * im
            if grab_i >= 0:
                f = kg * (grab_t - yE[grab_i]) - cg * vE[grab_i]
                vE[grab_i] += f * im
            if on > 0.0:
                nzl = nzlp + aLP * (noise[i * ovs + o] - nzlp)
                nzlp = nzl
                fn = fn0 * (1.0 + nzd * nzl) * on
                if fn < 0.0: fn = 0.0
                vb_t = vb0 * on
                vr = vb_t - vE[jb]
                cap = cst * fn * (fr + (1.0 - fr) * math.exp(-abs(vr) / vf)) * im
                if abs(vr) <= cap:
                    vE[jb] = vb_t
                    stats[0] += 1.0
                else:
                    vE[jb] += math.copysign(cap, vr)
            for j in range(1, N + 1):
                yE[j] += vE[j] * dt
            for j in range(N + 2, 2 * N + 2):
                yE[j] += vE[j] * dt
        # съём: 2 скоростных тапа на A + 1 на B (комб = псевдо-корпус)
        x = 2.5 * vE[j1] + 1.2 * vE[j2] + 0.8 * vE[j3]
        if body_on > 0.5:
            acc = x
            for m in range(3):
                yb = bb0[m] * x + bc1[m] * bs1[m] - br_[m] * br_[m] * bs2[m]
                bs2[m] = bs1[m]; bs1[m] = yb
                acc += bgn[m] * yb
            x = acc
        xs = x - dcx + dca * dcy
        dcx = x; dcy = xs
        out[i] = xs
    dcs[0] = dcx; dcs[1] = dcy; dcs[3] = nzlp

P = dict(R=0.0, speed=1.0, press=1.0, noise=0.25, beta=0.08, vol=0.35,
         ca=0.08, bow=True, coll=True, automorph=True, body=True)
OVS_MAP = {"ЭКО": 1, "НОРМА": 2, "МАКС": 4}

vE = np.zeros(2 * N + 3); yE = np.zeros(2 * N + 3)
ST = type("ST", (), {})()
ST.ovs = 2; ST.t = 0.0; ST.on = 0.0; ST.R_now = 0.0
ST.snap = np.zeros(2 * N + 1)
ST.level = 0.0; ST.cpu = 0.0; ST.underr = 0
ST.rms_ema = 0.0; ST.gain = 0.0
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

TT = np.linspace(0.0, 7.0, 701)
RT = np.array([R_auto(t) for t in TT])

def stability(ovs):
    k4e = K4A if ovs >= 2 else 0.0
    w2 = (4.0 * KA + 16.0 * k4e) / M0
    return math.sqrt(w2) / (SR * ovs)

def process_block(frames):
    if ST.pending_ovs is not None:
        ST.ovs = ST.pending_ovs; ST.pending_ovs = None
    ovs = ST.ovs; dt = 1.0 / (SR * ovs)
    k4e = K4A if ovs >= 2 else 0.0
    im = dt / M0; cam = P["ca"] * M0
    aLP = 1.0 - math.exp(-2.0 * math.pi * 2500.0 * dt)
    phase = (ST.t + np.arange(frames) / SR) % 7.0
    Rarr = np.interp(phase, TT, RT) if P["automorph"] else np.full(frames, P["R"])
    stp = (1.0 / (SR * 0.03)) if P["bow"] else (-1.0 / (SR * 0.08))
    onarr = np.clip(ST.on + stp * np.arange(1, frames + 1), 0.0, 1.0)
    ST.on = float(onarr[-1]); ST.t += frames / SR
    noise = RNG.standard_normal(frames * ovs)
    if ST.kick:
        x = np.sin(np.pi * np.linspace(0, 1, N)) ** 2
        vE[1:N + 1] += 0.6 * x; vE[N + 2:2 * N + 2] -= 0.5 * x
        ST.kick = False
    if ST.pluck is not None:
        j, amp = ST.pluck; yE[j] += amp; ST.pluck = None
    if ST.grab is not None:
        i0, tg = ST.grab
        gi = i0 + 1 if i0 < N else i0 + 2
    else:
        gi, tg = -1, 0.0
    jb = int(P["beta"] * N) + 1
    out = np.empty(frames); STATS[0] = 0.0
    t0 = time.perf_counter()
    block_nb(vE, yE, out, noise, Rarr, onarr, frames, ovs, dt,
             KA, KB, k4e, k4e * KB / KA, BRA, BRB, cam, im, Z0,
             N, C0, C1, GAP, KC, CC, FCAP, CST, FR,
             P["speed"], P["press"], P["noise"], jb, P["coll"],
             gi, tg, KG, CG, aLP, J1, J2, J3,
             1.0 if P["body"] else 0.0, BR_, BC1_, BB0_, BODYG, BS1, BS2,
             DCS, STATS)
    ST.cpu = 0.9 * ST.cpu + 0.1 * (time.perf_counter() - t0) / (frames / SR)
    ST.snap[:] = yE[1:2 * N + 2]
    ST.stick = 0.9 * getattr(ST, "stick", 0.0) + 0.1 * STATS[0] / max(frames * ovs, 1)
    rms = float(np.sqrt(np.mean(out * out))) + 1e-12
    ST.rms_ema = 0.85 * ST.rms_ema + 0.15 * rms
    g_t = min(0.10 / max(ST.rms_ema, 1e-9), 2.5)
    if ST.gain == 0.0:
        ST.gain = g_t
    else:
        slew = 0.06 if g_t < ST.gain else 0.02
        ST.gain += max(min(g_t - ST.gain, slew), -slew)
    y = np.tanh(out * (ST.gain * P["vol"] * 3.0))
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
    from tkinter import ttk
    root = tk.Tk(); root.title("physmod live v6")
    W, H = 920, 360
    cv = tk.Canvas(root, width=W, height=H, bg="#101018", highlightthickness=0)
    cv.pack(fill="both", expand=True)
    XM0, XM1 = 40, W - 40
    ZOOM, B_OFF = 3000.0, 70
    B_BASE, A_BASE = 120, 190
    def draw():
        try:
            cv.delete("all")
            sn = ST.snap
            cv.create_rectangle(XM0 + C0 * (XM1 - XM0) // N, 20,
                                XM0 + C1 * (XM1 - XM0) // N, H - 20,
                                fill="#1a1a28", width=0)
            for off, base, col in ((0, A_BASE, "#e8e8f0"), (N + 1, B_BASE, "#7f7fd0")):
                pts = []
                for j in range(N):
                    pts += [XM0 + j * (XM1 - XM0) / (N - 1),
                            base - float(sn[off + j]) * ZOOM]
                cv.create_line(*pts, fill=col, width=2)
            jb = int(P["beta"] * N)
            xj = XM0 + jb * (XM1 - XM0) / (N - 1)
            yj = A_BASE - float(sn[jb]) * ZOOM
            cv.create_oval(xj - 5, yj - 5, xj + 5, yj + 5, fill="#ff5040", width=0)
            lv = min(1.0, ST.level * 4)
            cv.create_rectangle(20, H - 18, 20 + int((W - 40) * lv), H - 8,
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
               ("Затухание", "ca", 0.02, 0.30),
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
    def toggle_body():
        P["body"] = not P["body"]
        b_body.config(text="БОДИ: ВКЛ" if P["body"] else "БОДИ: выкл")
    b_bow = tk.Button(buts, text="СМЫЧОК: ВКЛ", width=14, command=toggle_bow)
    b_body = tk.Button(buts, text="БОДИ: ВКЛ", width=12, command=toggle_body)
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
    for b in (b_bow, b_body, b_kick, b_rec, b_auto, qual, b_quit):
        b.pack(side="left", padx=3, pady=4)
    status = tk.Label(root, text="", anchor="w"); status.pack(fill="x")
    def tick():
        try:
            if P["automorph"]: sv["R"].set(ST.R_now)
            status.config(text=f"CPU {ST.cpu*100:4.0f}%  сбои {ST.underr}  "
                               f"стик {ST.stick*100:3.0f}%  R={Rarr[-1]:.2f}"
                               + ("  ЗАПИСЬ..." if ST.rec else ""))
        except Exception as e:
            print("tick:", e)
        root.after(200, tick)
    def pick(e):
        j = round((e.x - XM0) * (N - 1) / (XM1 - XM0)); j = max(0, min(N - 1, j))
        dA = abs(e.y - (A_BASE - float(ST.snap[j]) * ZOOM))
        dB = abs(e.y - (B_BASE - float(ST.snap[N + 1 + j]) * ZOOM))
        i = j if dA <= dB else N + j
        base = A_BASE if i < N else B_BASE
        ST.grab = (i, max(-0.01, min(0.01, (base - e.y) / ZOOM)))
    def pluck(e):
        pick(e)
        if ST.grab is not None:
            i = ST.grab[0]; ST.grab = None
            j = i + 1 if i < N else i + 2
            ST.pluck = (j, 0.008 * (1 if e.y < H // 2 else -1))
    cv.bind("<Button-1>", pick)
    cv.bind("<B1-Motion>", lambda e: ST.grab and pick(e))
    cv.bind("<ButtonRelease-1>", lambda e: setattr(ST, "grab", None))
    cv.bind("<Button-3>", pluck)
    root.bind("<space>", lambda e: toggle_bow())
    root.bind("<k>", lambda e: setattr(ST, "kick", True))
    root.protocol("WM_DELETE_WINDOW", root.destroy)
    print("""УПРАВЛЕНИЕ: ЛКМ — тянуть; ПКМ — щипок; ПРОБЕЛ — смычок; K — толчок;
БОДИ — резонанс корпуса вкл/выкл; ЗАПИСЬ — jam_ЧЧММСС.wav. НОРМА минимум!""")
    stream = sd.OutputStream(samplerate=SR, blocksize=BLOCK, channels=1,
                             callback=audio_cb, latency="low")
    stream.start()
    draw(); tick(); root.mainloop()
    stream.stop(); stream.close()

def main():
    args = [a.lower() for a in sys.argv[1:]]
    print(f"python {sys.version.split()[0]} | numpy {np.__version__}")
    for q, o in OVS_MAP.items():
        print(f"  качество {q}: w*dt = {stability(o):.2f} "
              + ("ОК" if stability(o) < 1.0 else "НЕСТАБИЛЬНО (жёсткость off)"))
    if "diag" in args:
        print(sd.query_devices()); return
    if "render" in args:
        process_block(64)
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