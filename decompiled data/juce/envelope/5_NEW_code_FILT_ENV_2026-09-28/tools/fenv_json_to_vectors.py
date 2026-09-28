#!/usr/bin/env python3
"""fenv_json_to_vectors.py — конвертер снапшотов exp18 (JSON) -> текстовые
векторы для C++-тестера MnmFilterStage2 (формат в духе FM-векторов).

На каждый кадр:
  F
  IN  <X00[16]> <X10[16]> <X20[16]> <X30[16]> | ATK DEC BOFS WOFS PHASE |
      D3Y D3X D4Y D4X D5Y D5X CCY CCX CFY CFX
  E1  <Y:$62-$81 (32, чересстрочно a/b)> <D4Y D4X D3Y D3X>          (36 слов)
  E2  <Y:$20-$3F (32)> <D5Y D5X>                                    (34)
  E3  <Y:$62-$81 (32)> <CCY CCX>                                    (34)
  E4  <X:$40-$61 (34)>
Все слова 24-бит без знака (hex-free, десятичные 0..16777215).
"""
import json, sys, os

SRC = sys.argv[1] if len(sys.argv) > 1 else \
    "/home/z/my-project/.cache/relocated_from_workspace/mining/exp18_fdn_snap.json"
DST = sys.argv[2] if len(sys.argv) > 2 else \
    "/home/z/my-project/work/fm_fenv/fenv_vectors.txt"

P = 0x400


def w(s, key):
    return s[key] & 0xFFFFFF


def main():
    G = json.load(open(SRC))
    out = []
    out.append("# FM FILTER stage-2 (ATK/DEC/BOFS/WOFS) bit-exact vectors")
    out.append("# источник: exp18_fdn_snap.json (эмулятор OS 1.32, брейкпоинты")
    out.append("# $0A5D/$0A84/$0A9D/$0AB7/$0AD1; трек-кадр $0100-$0B4C, машина 1)")
    n_frames = 0
    for ci, c in enumerate(G):
        out.append("CFG %d ATK=%d DEC=%d BOFS=%d WOFS=%d" %
                   (ci, c["atk"], c["dec"], c["bofs"], c["wofs"]))
        for snaps in c["frames"]:
            s_in, s_l1, s_l2, s_l3, s_dp, s_out = snaps
            out.append("F")
            ins = []
            for i in range(16):
                ins.append(w(s_in, "X:%03X" % (0x00 + i)))
            for i in range(16):
                ins.append(w(s_in, "X:%03X" % (0x10 + i)))
            for i in range(16):
                ins.append(w(s_in, "X:%03X" % (0x20 + i)))
            for i in range(16):
                ins.append(w(s_in, "X:%03X" % (0x30 + i)))
            for a in (0x14, 0x15, 0x16, 0x17):
                ins.append(w(s_in, "Y:P+%02X" % a))
            ins.append(w(s_in, "X:P+D8"))
            for a in (0xD3, 0xD4, 0xD5, 0xCC, 0xCF):
                ins.append(w(s_in, "Y:P+%02X" % a))
                ins.append(w(s_in, "X:P+%02X" % a))
            out.append("IN " + " ".join(str(v) for v in ins))
            e1 = [w(s_l1, "Y:%03X" % (0x62 + i)) for i in range(32)]
            e1 += [w(s_l1, "Y:P+D4"), w(s_l1, "X:P+D4"),
                   w(s_l1, "Y:P+D3"), w(s_l1, "X:P+D3")]
            out.append("E1 " + " ".join(str(v) for v in e1))
            e2 = [w(s_l2, "Y:%03X" % (0x20 + i)) for i in range(32)]
            e2 += [w(s_l2, "Y:P+D5"), w(s_l2, "X:P+D5")]
            out.append("E2 " + " ".join(str(v) for v in e2))
            e3 = [w(s_l3, "Y:%03X" % (0x62 + i)) for i in range(32)]
            e3 += [w(s_l3, "Y:P+CC"), w(s_l3, "X:P+CC")]
            out.append("E3 " + " ".join(str(v) for v in e3))
            e4 = [w(s_dp, "X:%03X" % (0x40 + i)) for i in range(0x22)]
            out.append("E4 " + " ".join(str(v) for v in e4))
            n_frames += 1
    os.makedirs(os.path.dirname(DST), exist_ok=True)
    open(DST, "w").write("\n".join(out) + "\n")
    print("configs: %d, frames: %d -> %s" % (len(G), n_frames, DST))


if __name__ == "__main__":
    main()
