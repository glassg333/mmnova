#!/usr/bin/env python3
"""exp80 — арбитраж карты ручек: $408-$40F (карта аудита/astra) vs $410-$417
(карта итерации 17). Для каждой ячейки-кандидата: инжектируем значение,
прогоняем кадр, снимаем (1) какие P-адреса читают ячейку, (2) меняются ли
выходные сэмплы/кольцо/Y:$04-$07/SVF-состояние.

Метод: полный кадр track_harness (машина 1 GND-SIN, живой сигнал), базовый
прогон vs инжекция. Читатели ячейки — через watch-механизм эмулятора
(watch_read: перехват rd по адресу ячейки).
"""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from track_harness import build_track_emu, setup_track, run_frame, read_out, R6

P = R6  # 0x400

def run_with_cell(cell, value):
    """Прогон кадра с инжекцией value в ячейку cell (X и Y — оба пространства,
    как это делал бы хост, если бы писал слово страницы). Возвращает
    (выход, список P-адресов-читателей, кольцо Y:$04-$07, writes в ячейку)."""
    e = build_track_emu(machine=1)
    setup_track(e, base=64, wdth=32, trig=1, hpq=32, lpq=96)
    # базовые значения Q, чтобы фильтр был жив
    e.Y[P + 0x12] = 32 << 16
    e.Y[P + 0x13] = 96 << 16
    e.X[P + 0x12] = 32 << 16
    e.X[P + 0x13] = 96 << 16
    # инжекция
    e.Y[cell] = value
    e.X[cell] = value
    # watch: читатели/писатели ячейки
    readers = []
    orig_rd_x = e.rd
    orig_rd_y = e.rd
    def rd_x(sp, ea, _orig=e.rd):
        v = _orig(sp, ea)
        return v
    # проще: monkey-patch rd/wr с логом
    log = []
    _rd = e.rd
    _wr = e.wr
    def rd(space, ea):
        v = _rd(space, ea)
        if ea == cell:
            log.append(("R", space, e.pc))
        return v
    def wr(space, ea, val):
        if ea == cell:
            log.append(("W", space, e.pc, val))
        return _wr(space, ea, val)
    e.rd = rd
    e.wr = wr
    try:
        run_frame(e)
    except Exception as ex:
        return None, [("ERR", str(ex), hex(e.pc))], None, log
    out = read_out(e)
    ring = [e.Y[P + i] for i in range(4, 8)]
    return out, log, ring, log

def fmt_log(log):
    rs = sorted(set((sp, hex(pc)) for (t, sp, *rest) in log if t == "R" for sp, pc in [(sp, rest[0])]))
    ws = sorted(set((sp, hex(pc), hex(v)) for (t, sp, pc, *v) in log if t == "W" for sp, pc, v in [(sp, pc, v[0])]))
    return rs, ws

cells = {
    "BASE? $408": P + 0x08,
    "WDTH? $409": P + 0x09,
    "HPQ? $40A": P + 0x0A,
    "LPQ? $40B": P + 0x0B,
    "BASE(ит17) $410": P + 0x10,
    "WDTH(ит17) $411": P + 0x11,
    "HPQ(ит17) $412": P + 0x12,
    "LPQ(ит17) $413": P + 0x13,
    "BOFS(ит17) $416": P + 0x16,
    "WOFS(ит17) $417": P + 0x17,
}

# базовый прогон без инжекций
base_out, _, base_ring, _ = run_with_cell(P + 0x33, 0)  # нейтральная ячейка
print("база: выход[0:4] =", [hex(v) for v in base_out[:4]], " кольцо Y04-07:", [hex(v) for v in base_ring])
print()
for name, cell in cells.items():
    out, log, ring, _ = run_with_cell(cell, 0x5A0000)  # заметное значение 0x5A>>16=90
    if out is None:
        print(f"{name}: ERR {log}")
        continue
    readers, writers = fmt_log(log)
    diff = sum(1 for a, b in zip(out, base_out) if a != b)
    ringdiff = (ring != base_ring)
    print(f"{name}: отличий выхода {diff}/16, кольцо {'ИЗМЕНИЛОСЬ' if ringdiff else 'не изменилось'}")
    print(f"   читатели: {readers if readers else 'НЕТ'}")
    if writers:
        print(f"   писатели: {writers}")
