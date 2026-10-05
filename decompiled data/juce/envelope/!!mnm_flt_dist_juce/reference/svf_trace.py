#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
svf_trace.py — прогон ядра DSP1 (P:$04A8-$0B4C) на бит-точном эмуляторе
с дампами X/Y на границах P:$05FF / $06CD / $0719 / $077A.

Закрывает OPEN-пункты §8.5 трассы Q (reference/kernel_P05FF-0719_q_feedback_trace.md):
  E1: разводка ячеек FLT-страницы: $408-$40F (ручки) vs $410/$411 (кольцо)
  E2: соответствие движков A (func_000350) / B (func_000365) плечам hp/lp
      + кросс-включение Q-ячеек $40A/$40B в интерполяторы B/A
  E3: алгебра концов глайд-таблиц y:$(4..7)+3k (ступенька BASE между блоками)
  E4: семантика param4 (y:(r6+$4)) — kQGainTbl и тон-фактор DIST

Метод: верbatim-код ядра, тестовый стейт голоса (r6=$400), стимул в
машинный выходной буфер (SVF_IN = r6+$E6, методика experiments.py).
"""
import sys, os, json, subprocess

BASE_DIR = "/home/z/my-project/repos/mmnova/decompiled data"
DIS = os.path.join(BASE_DIR, "02_scripts/DSP56300 disassemblerStandalone/scripts/dis56300.py")
PM  = os.path.join(BASE_DIR, "03_memory_images/dsp1_pmem.bin")
TOOL_EMU = os.path.join(BASE_DIR, "02_scripts/tools_emulator")
sys.path.insert(0, TOOL_EMU)
from dsp_emu import DSP56300  # noqa: E402

R6 = 0x400
# ячейки страницы (r6-смещения, верифицированы по листингу):
P_PARAM4 = R6 + 0x04      # y:(r6+$4)  param4 (kQGainTbl / тон-фактор DIST)
P_BASE   = R6 + 0x08      # y:(r6+$8)  BASE   ($0537)
P_WDTH   = R6 + 0x09      # y:(r6+$9)  WDTH   ($0557)
P_QB     = R6 + 0x0A      # y:(r6+$a)  Q -> интерполятор фильтра B ($06E0)
P_QA     = R6 + 0x0B      # y:(r6+$b)  Q -> интерполятор фильтра A ($0610); X:$40b = DIST
P_ATK    = R6 + 0x0C      # y:(r6+$c)  ATK ($0509)
P_DEC    = R6 + 0x0D      # y:(r6+$d)  DEC ($052B)
P_BOFS   = R6 + 0x0E      # y:(r6+$e)  BOFS ($053E)
P_WOFS   = R6 + 0x0F      # y:(r6+$f)  WOFS (Y); X-сторона бит0 = однократный проход
P_RING1  = R6 + 0x10      # y:(r6+$10) кольцо: сглаженный индекс среза ($056D)
P_RING2  = R6 + 0x11      # y:(r6+$11) кольцо: сглаженная ширина ($0578)
P_ENVL   = R6 + 0x1B      # X:$4DB ENV-уровень (читатели $0547/$0565)
P_IDXDA  = R6 + 0xDA      # y:(r6+$DA) индекс среза для Q-интерполятора A ($056C -> $05FF)
P_KD9    = R6 + 0xD9      # x:(r6+$D9) кейтрек-копия индекса -> Q-интерполятор B ($06CD)
P_MBUF   = R6 + 0xDC      # машинный выходной буфер (34 слова)
SVF_IN   = R6 + 0xE6      # 16-сэмпловая входная область (методика experiments.py)

# режимные ячейки вне страницы
M_MODE   = 0x425          # биты 11/9 Y:$425 — кейтрек
M_TRIG   = 0x428          # триггер
M_TGATE  = 0x124          # гейт счётчика трека (машина огибающей)

Q23 = 8388608.0
def q23(x):
    v = int(round(x * Q23))
    return max(-0x800000, min(0x7FFFFF, v)) & 0xFFFFFF

def s2f(v):
    v &= 0xFFFFFF
    return (v - (1 << 24)) / Q23 if v & 0x800000 else v / Q23

def disasm(start, count):
    r = subprocess.run([sys.executable, DIS, PM, hex(start), str(count)],
                       capture_output=True, text=True)
    return r.stdout.splitlines()

def build_emu():
    lines = disasm(0x0000, 0x0B4E)
    e = DSP56300(lines)
    data = open(PM, "rb").read()
    n = len(data) // 3
    for i in range(0x100000, min(n, 0x160000)):
        o = i * 3
        w = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
        e.X[i] = w
        e.Y[i] = w
    return e

def setup(e, base=0.5, wdth=0.5, qA=0.0, qB=0.0, atk=0.5, dec=0.5,
          bofs=0.5, wofs=0.5, param4=0.0, ring1=None, ring2=None,
          env=0.0, mode=0, stereo_bit=1):
    Y, X = e.Y, e.X
    for i in range(0x200):
        Y[R6 + i] = 0
        X[R6 + i] = 0
    for a in range(0x100):          # kernel scratch
        X[a] = 0
        Y[a] = 0
    Y[0x400] = 0; Y[0x401] = 0; Y[0x402] = 0xFFFFF0; Y[0x403] = 0  # AHDR
    Y[P_PARAM4] = q23(param4)
    Y[P_BASE] = q23(base)
    Y[P_WDTH] = q23(wdth)
    Y[P_QB]   = q23(qB)
    Y[P_QA]   = q23(qA)
    Y[P_ATK]  = q23(atk)
    Y[P_DEC]  = q23(dec)
    Y[P_BOFS] = q23(bofs)
    Y[P_WOFS] = q23(wofs)
    X[P_WOFS] = stereo_bit          # бит0 X:$40f: 1 = однократный проход движка
    X[P_QA]   = 0                   # X:$40b = DIST-слово (0 = дист выкл)
    Y[P_RING1] = q23(ring1) if ring1 is not None else 0
    Y[P_RING2] = q23(ring2) if ring2 is not None else 0
    X[P_ENVL] = q23(env)
    Y[M_MODE] = mode
    Y[M_TRIG] = 1
    Y[M_TGATE] = 0
    Y[P_IDXDA] = 0
    X[P_KD9] = 0
    Y[R6 + 0x21] = 1                # env init flag
    for i in range(0x40):
        Y[P_MBUF + i] = 0
        X[P_MBUF + i] = 0
    e.R[6] = R6
    e.R[7] = 0

def set_input(e, sig):
    """Стимул = блок-рейт слова машины: движок A читает l:$4E2 ($06BD/$06BE),
    движок B — l:$4E6 ($0783-$0785). Пишем x/y половины обоих слотов.
    (Верифицировано маркерами + чтениями листинга.)"""
    for i in range(0x40):
        e.Y[P_MBUF + i] = 0
        e.X[P_MBUF + i] = 0
    e.Y[R6 + 0xE2] = sig[0] & 0xFFFFFF
    e.X[R6 + 0xE2] = sig[0] & 0xFFFFFF
    e.Y[R6 + 0xE6] = sig[0] & 0xFFFFFF
    e.X[R6 + 0xE6] = sig[0] & 0xFFFFFF

def run_seg(e, start, end):
    return e.run(start, end=end, max_steps=2_000_000)

def run_block_segments(e):
    """Один блок с дампами на границах. Возвращает dict снапшотов."""
    out = {}
    e.R[6] = R6
    run_seg(e, 0x04A8, 0x05FF)      # prep: env, coeff ring, индекс
    out["at_05FF"] = seg_snap(e)
    run_seg(e, 0x05FF, 0x06CD)      # Q-A расчёт + глайд A + движок A
    out["at_06CD"] = seg_snap(e)
    run_seg(e, 0x06CD, 0x071A)      # Q-B расчёт (y:$1d/$1e перезаписываются)
    out["at_071A"] = seg_snap(e)
    run_seg(e, 0x071A, 0x077A)      # глайд B
    out["at_077A"] = seg_snap(e)
    run_seg(e, 0x077A, 0x0B4C)      # движок B + DIST + хвост
    out["at_end"] = seg_snap(e)
    return out

def seg_snap(e):
    Y, X = e.Y, e.X
    glide_a = [Y[4 + 3 * k] for k in range(16)]
    glide_qh = [Y[5 + 3 * k] for k in range(16)]
    glide_ql = [Y[6 + 3 * k] for k in range(16)]
    return {
        "y1d_qhp": Y[0x1D], "y1e_qlp": Y[0x1E], "y1c_f1": Y[0x1C],
        "y4_f2": Y[4 - 3 + 3 * 0] if False else None,  # f2 хранится в y:$4 (y:$04 = тройка k=0 f)
        "Y04": Y[4], "Y05": Y[5], "Y06": Y[6], "Y07": Y[7],
        "idxDA": Y[P_IDXDA], "kD9": X[P_KD9],
        "ring1": Y[P_RING1], "ring2": Y[P_RING2],
        "glideA_f": glide_a, "glideA_qhp": glide_qh, "glideA_qlp": glide_ql,
        "A_out": [ (X[0x74 + i], Y[0x74 + i]) for i in range(16) ],
        "B_out": [ (X[0x72 + i], Y[0x72 + i]) for i in range(16) ],
        "frameE2": (X[R6 + 0xE2], Y[R6 + 0xE2]),
        "frameE3": (X[R6 + 0xE3], Y[R6 + 0xE3]),
        "frameE6": (X[R6 + 0xE6], Y[R6 + 0xE6]),
        "frameE7": (X[R6 + 0xE7], Y[R6 + 0xE7]),
        "frameE8": (X[R6 + 0xE8], Y[R6 + 0xE8]),
    }

def buf_out(e, base_x, base_y, n=16):
    return [(e.X[base_x + i], e.Y[base_y + i]) for i in range(n)]

# ------------------------------------------------------------------ E1
def E1_cells():
    """Какая ячейка реально двигает индекс/коэффициенты/q."""
    print("=== E1: разводка ячеек (BASE/WDTH $408/$409 vs кольцо $410/$411; Q $40A/$40B) ===")
    res = {}
    def probe(name, **kw):
        e = build_emu()
        setup(e, **kw)
        set_input(e, [0] * 16)
        segs = None
        for blk in range(3):
            segs = run_block_segments(e)
        row = {
            "idxDA": s2f(segs["at_05FF"]["idxDA"]),
            "ring1": s2f(segs["at_05FF"]["ring1"]),
            "ring2": s2f(segs["at_05FF"]["ring2"]),
            "Y04": s2f(segs["at_06CD"]["Y04"]),
            "qhp_afterA": s2f(segs["at_06CD"]["y1d_qhp"]),
            "qlp_afterA": s2f(segs["at_06CD"]["y1e_qlp"]),
            "qhp_afterB": s2f(segs["at_071A"]["y1d_qhp"]),
            "qlp_afterB": s2f(segs["at_071A"]["y1e_qlp"]),
        }
        res[name] = row
        print("%-22s idxDA=%+9.6f ring=(%+8.5f,%+8.5f) Y04=%+9.6f "
              "qA=(%+8.5f,%+8.5f) qB=(%+8.5f,%+8.5f)" %
              (name, row["idxDA"], row["ring1"], row["ring2"], row["Y04"],
               row["qhp_afterA"], row["qlp_afterA"], row["qhp_afterB"], row["qlp_afterB"]))
        return row

    probe("all_zero")
    probe("BASE@$408=0.7", base=0.7)
    probe("WDTH@$409=0.7", wdth=0.7)
    probe("ring1@$410=0.7", ring1=0.7)
    probe("ring2@$411=0.7", ring2=0.7)
    probe("QA@$40B=0.8(Q->A)", qA=0.8)
    probe("QB@$40A=0.8(Q->B)", qB=0.8)
    probe("BOTH_Q=0.8", qA=0.8, qB=0.8)
    return res

# ------------------------------------------------------------------ E2
def _taps(e):
    """Блок-рейт отводы: A = l:$4E2(a)/l:$4E3(b), B = l:$4E6/l:$4E7/l:$4E8,
    DIST-буфер y:$4B0, per-sample хвосты A(L:$74..) B(L:$72..)."""
    return {
        "A_a": s2f(e.Y[R6 + 0xE2]) , "A_a_x": s2f(e.X[R6 + 0xE2]),
        "A_b": s2f(e.Y[R6 + 0xE3]), "A_b_x": s2f(e.X[R6 + 0xE3]),
        "B_1": s2f(e.Y[R6 + 0xE6]), "B_1_x": s2f(e.X[R6 + 0xE6]),
        "B_2": s2f(e.Y[R6 + 0xE7]), "B_2_x": s2f(e.X[R6 + 0xE7]),
        "B_3": s2f(e.Y[R6 + 0xE8]), "B_3_x": s2f(e.X[R6 + 0xE8]),
        "A_s15": s2f(e.Y[0x74 + 15]), "B_s15": s2f(e.Y[0x72 + 15]),
    }

def _run_seq(e, words, nblocks, **kw):
    """Прогон nblocks блоков; words — цикличная последовательность блок-слов."""
    seq = []
    for k in range(nblocks):
        set_input(e, [words[k % len(words)]])
        run_block_segments(e)
        seq.append(_taps(e))
    return seq

def _stats(seq, key):
    vals = [s[key] for s in seq]
    n = len(vals)
    mean = sum(vals) / n
    # амплитуда блочного чередования (последовательные разности /2)
    alt = sum(abs(vals[i] - vals[i - 1]) for i in range(1, n)) / (n - 1) / 2
    peak = max(abs(v) for v in vals)
    return mean, alt, peak

def E2_shoulders():
    """Блок-рейт перенос: DC / минус-DC / блочный квадрат ±0.5 (1.38кГц).
    LP-плечо: высокий DC-гейн, низкий AC. HP-плечо: DC≈0, AC высокий."""
    print("=== E2: плечи hp/lp (блок-рейт DC и переменный ток) ===")
    res = {}
    KEYS = ["A_a", "A_b", "B_1", "B_2", "B_3"]
    def run_case(name, words, nsettle=10, nmeas=48, **kw):
        e = build_emu()
        setup(e, **kw)
        for _ in range(nsettle):
            set_input(e, [words[0]])
            run_block_segments(e)
        seq = _run_seq(e, words, nmeas)
        row = {}
        for k in KEYS:
            m, a, p = _stats(seq, k)
            row[k] = {"mean": m, "alt": a, "peak": p}
        res[name] = row
        print("%-22s " % name + "  ".join(
            "%s: dc=%+7.4f ac=%7.4f pk=%7.4f" % (k, row[k]["mean"], row[k]["alt"], row[k]["peak"])
            for k in KEYS))
        return row

    plus = [q23(0.25)]
    minus = [q23(-0.25)]
    square = [q23(0.25), q23(-0.25)]
    kw = dict(base=0.5, wdth=0.5)
    run_case("DC+0.25", plus, **kw)
    run_case("DC-0.25", minus, **kw)
    run_case("SQ_+-0.25", square, **kw)
    # импульсный отклик: один блок +0.25, дальше ноль — звон по стадиям
    def impulse(name, nblocks=48, **kw):
        e = build_emu()
        setup(e, **kw)
        set_input(e, [q23(0.25)])
        run_block_segments(e)
        set_input(e, [0])
        seq = _run_seq(e, [0], nblocks)
        res[name] = [{"blk": i, **s} for i, s in enumerate(seq)]
        dec = [abs(s["A_a"]) for s in seq]
        decb = [abs(s["B_1"]) for s in seq]
        print("%-22s A_a: %s...  B_1: %s..." %
              (name, ["%.5f" % v for v in dec[:8]], ["%.5f" % v for v in decb[:8]]))
    impulse("imp_neutral", base=0.5, wdth=0.5)
    impulse("imp_qA_0.95", base=0.5, wdth=0.5, qA=0.95)
    impulse("imp_qB_0.95", base=0.5, wdth=0.5, qB=0.95)
    impulse("imp_base0", base=0.0, wdth=0.5)
    impulse("imp_base1", base=1.0, wdth=0.5)
    impulse("imp_wdth1", base=0.5, wdth=1.0)
    return res

# ------------------------------------------------------------------ E3
def E3_glide():
    """Концы глайд-таблиц при ступеньке BASE."""
    print("=== E3: глайд при ступеньке BASE (0.3 -> 0.7) ===")
    e = build_emu()
    setup(e, base=0.3, wdth=0.5)
    set_input(e, [0] * 16)
    for _ in range(30):               # выход на стационар
        run_block_segments(e)
    prev_end = {
        "f":  [s2f(e.Y[4 + 3 * k]) for k in range(16)],
        "qh": [s2f(e.Y[5 + 3 * k]) for k in range(16)],
        "ql": [s2f(e.Y[6 + 3 * k]) for k in range(16)],
    }
    # ступенька: правим ячейку BASE, кольцо не трогаем
    e.Y[P_BASE] = q23(0.7)
    set_input(e, [0] * 16)
    segs = run_block_segments(e)
    new = {
        "f":  [s2f(v) for v in segs["at_06CD"]["glideA_f"]],
        "qh": [s2f(v) for v in segs["at_06CD"]["glideA_qhp"]],
        "ql": [s2f(v) for v in segs["at_06CD"]["glideA_qlp"]],
    }
    out = {"prev_end": prev_end, "step_block": new,
           "idxDA": s2f(segs["at_05FF"]["idxDA"]),
           "ring1_before": None}
    print("k, prev_f, new_f, delta_f | prev_qh, new_qh")
    for k in range(16):
        print("%2d  f: %+9.6f -> %+9.6f (d=%+9.6f)   qhp: %+9.6f -> %+9.6f" %
              (k, prev_end["f"][k], new["f"][k],
               new["f"][k] - prev_end["f"][k],
               prev_end["qh"][k], new["qh"][k]))
    # стационарный блок (без ступеньки) для сравнения
    e.Y[P_BASE] = q23(0.7)
    set_input(e, [0] * 16)
    segs2 = run_block_segments(e)
    st = {
        "f":  [s2f(v) for v in segs2["at_06CD"]["glideA_f"]],
        "qh": [s2f(v) for v in segs2["at_06CD"]["glideA_qhp"]],
    }
    out["steady_block"] = st
    print("steady after step: f[0]=%+9.6f f[15]=%+9.6f" % (st["f"][0], st["f"][15]))
    return out

# ------------------------------------------------------------------ E4
def E4_param4():
    """param4: kQGainTbl (Q-предкоррекция) и тон-фактор DIST."""
    print("=== E4: sweep param4 (y:$404) ===")
    res = {}
    for p in [0.0, 0.25, 0.5, 0.75, 1.0]:
        e = build_emu()
        setup(e, base=0.5, wdth=0.5, qA=0.9, param4=p)
        set_input(e, [q23(0.5)] * 16)
        for _ in range(6):
            segs = run_block_segments(e)
        row = {
            "f1": s2f(segs["at_06CD"]["y1c_f1"]),
            "Y04": s2f(segs["at_06CD"]["Y04"]),
            "qhp_afterA": s2f(segs["at_06CD"]["y1d_qhp"]),
            "dist_rms": rms([s2f(e.Y[R6 + 0xB0 + i]) for i in range(16)]),
        }
        res["p%.2f" % p] = row
        print("param4=%.2f  f1=%+9.6f Y04=%+9.6f qhp_A=%+8.5f dist_rms=%8.5f" %
              (p, row["f1"], row["Y04"], row["qhp_afterA"], row["dist_rms"]))
    return res

def rms(v):
    return (sum(x * x for x in v) / len(v)) ** 0.5

if __name__ == "__main__":
    which = sys.argv[1] if len(sys.argv) > 1 else "all"
    out = {}
    if which in ("all", "E1"): out["E1"] = E1_cells()
    if which in ("all", "E2"): out["E2"] = E2_shoulders()
    if which in ("all", "E3"): out["E3"] = E3_glide()
    if which in ("all", "E4"): out["E4"] = E4_param4()
    with open("/home/z/my-project/scripts/svf_trace_results.json", "w") as f:
        json.dump(out, f, indent=1)
    print("saved: /home/z/my-project/scripts/svf_trace_results.json")
