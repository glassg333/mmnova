#!/usr/bin/env python3
"""
dsp_emu.py — Text-driven DSP56300 emulator for Monomachine firmware mining (iteration 12).

Executes disassembled text produced by dis56300.py (verified 100% against the reference
dsp56k-emulator). Implements the instruction subset used by the kernel (P:$0000-$0B4D)
and the FX machines, with bit-accurate data-path semantics:

- 24-bit words; X0/X1/Y0/Y1 signed 24-bit; accumulators A/B = 56-bit (A2:A1:A0)
- MAC: signed 24x24 -> 48-bit product accumulated into 56-bit acc
- RND/MACR/MPYR: add $800000 to A0, round into A1, A0 = 0
- DIV: 24-step non-restoring division; A1:A0 = (A1_init:A0_init)/S in 24.24 (unsigned,
  C cleared before loop = classic idiom)
- Parallel data moves execute with PRE-ALU register values (pipelined store)
- DO hardware loop stack: body = [do_pc+1 .. ext_end] inclusive
- 8-bit immediate to accumulator goes to A1 (zero-extended)  [proven by div idioms]
- L memory (48-bit): A<->L maps A1->X-half, A0->Y-half; X/Y register pairs for "x"/"y"
"""

import re
import sys

MASK24 = 0xFFFFFF
MASK48 = 0xFFFFFFFFFFFF
MASK56 = 0xFFFFFFFFFFFFFF

ACC_REGS = {"a", "b"}
SIMPLE_REGS = {"x0", "x1", "y0", "y1", "a0", "a1", "a2", "b0", "b1", "b2",
               "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
               "n0", "n1", "n2", "n3", "n4", "n5", "n6", "n7",
               "m0", "m1", "m2", "m3", "m4", "m5", "m6", "m7",
               "sr", "ccr", "la", "lc", "ssh", "sp"}

CONDITIONS = {
    "ifcc": lambda f: not f["c"], "ifcs": lambda f: f["c"],
    "ifec": lambda f: not f["e"], "ifes": lambda f: f["e"],
    "ifeq": lambda f: f["z"], "ifne": lambda f: not f["z"],
    "ifmi": lambda f: f["n"], "ifpl": lambda f: not f["n"],
    "ifge": lambda f: f["n"] == f["v"],
    "iflt": lambda f: f["n"] != f["v"],
    "ifgt": lambda f: (f["n"] == f["v"]) and not f["z"],
    "ifle": lambda f: (f["n"] != f["v"]) or f["z"],
}

# conditions for Bcc (no 'if' prefix)
BR_CONDITIONS = {
    "cc": lambda f: not f["c"], "cs": lambda f: f["c"],
    "ec": lambda f: not f["e"], "es": lambda f: f["e"],
    "eq": lambda f: f["z"], "ne": lambda f: not f["z"],
    "mi": lambda f: f["n"], "pl": lambda f: not f["n"],
    "ge": lambda f: f["n"] == f["v"],
    "lt": lambda f: f["n"] != f["v"],
    "gt": lambda f: (f["n"] == f["v"]) and not f["z"],
    "le": lambda f: (f["n"] != f["v"]) or f["z"],
    "ra": lambda f: True, "nr": lambda f: False,
}


def sext(v, bits):
    s = 1 << (bits - 1)
    return (v & ((1 << bits) - 1)) - ((v & s) << 1)


class EmuError(Exception):
    pass


class DSP56300:
    def __init__(self, prog_lines, trace=False):
        self.prog = {}          # addr -> parsed instruction
        self.order = []         # execution order (for listing parse)
        self.labels = {}        # func_xxx/int_xxx -> addr
        self.parse(prog_lines)
        # memory
        self.X = {}
        self.Y = {}
        # registers
        self.x0 = self.x1 = self.y0 = self.y1 = 0
        self.A = 0              # 56-bit signed
        self.B = 0
        self.R = [0] * 8
        self.N = [0] * 8
        self.M = [MASK24] * 8
        self.f = {"c": 0, "v": 0, "z": 0, "n": 0, "e": 0, "u": 0}
        self.sr_int = 0          # raw SR bits (bset/bclr on sr)
        self.pc = 0
        self.do_stack = []      # list of [count, body_start, end_addr]
        self.ret_stack = []
        self.trace = trace
        self.trace_log = []
        self.watch = set()      # addresses (X/Y) to watch
        self.watch_log = []
        self.halted = False
        self.steps = 0
        self.max_steps = 50_000_000

    # ---------------- parsing ----------------
    def parse(self, lines):
        token_re = re.compile(r"\S+")
        for raw in lines:
            raw = raw.rstrip("\n")
            m = re.match(r"^\s*([0-9A-Fa-f]{6}):\s*(.*?)\s*;\s*([0-9A-Fa-f ]+)$", raw)
            if not m:
                continue
            addr = int(m.group(1), 16)
            text = m.group(2).strip()
            if text.startswith("func_") or text.startswith("int_"):
                continue
            if not text:
                continue
            parts = text.split(None, 1)
            mnem = parts[0].lower()
            ops = parts[1] if len(parts) > 1 else ""
            toks = token_re.findall(ops)
            # labels used as targets (may be embedded: '#$0,x0,func_0005bb')
            for tk in toks:
                for part in tk.split(","):
                    part = part.lstrip(">$<")
                    if part.startswith("func_") or part.startswith("int_"):
                        self.labels[part] = int(part.split("_")[1], 16)
            self.prog[addr] = (mnem, toks, text)
            self.order.append(addr)
        self.order.sort()
        self.next_addr = {}
        for i, a in enumerate(self.order):
            self.next_addr[a] = self.order[i + 1] if i + 1 < len(self.order) else a + 1

    # ---------------- memory ----------------
    def rd(self, space, ea):
        d = self.X if space == "x" else self.Y
        return d.get(ea, 0) & MASK24

    def wr(self, space, ea, val):
        d = self.X if space == "x" else self.Y
        val &= MASK24
        d[ea] = val
        if ea in self.watch:
            self.watch_log.append((self.pc, space, ea, val))

    def rdL(self, ea):
        return (self.rd("x", ea) << 24) | self.rd("y", ea)

    def wrL(self, ea, val48):
        self.wr("x", ea, (val48 >> 24) & MASK24)
        self.wr("y", ea, val48 & MASK24
                )

    def rd_mem(self, space, ea):
        if space == "l":
            return self.rdL(ea)
        return self.rd(space, ea)

    def wr_mem(self, space, ea, val):
        if space == "l":
            self.wrL(ea, val)
        else:
            self.wr(space, ea, val)

    def acc_part_for_dst(self, src_name, sval):
        """Return the value to store given the SOURCE register name.
        sval is the RAW 56-bit accumulator value.
        a/b -> A1 (24-bit main part); a0/b0 -> A0; a1/b1 -> A1; a2/b2 -> A2."""
        if src_name in ("a", "b"):
            return (sval >> 24) & MASK24
        if src_name in ("a0", "b0"):
            return sval & MASK24
        if src_name in ("a1", "b1"):
            return (sval >> 24) & MASK24
        if src_name in ("a2", "b2"):
            return (sval >> 48) & 0xFF
        return sval & MASK24

    def compose_acc(self, sval):
        """24-bit value -> 56-bit accumulator (A1 = val, sign-extended, A0 = 0)."""
        return sext(sval & MASK24, 24) << 24

    def alu_src(self, name):
        """ALU source operand value aligned to the accumulator (24-bit regs at A1)."""
        if name.startswith("#"):
            return sext(self.abs_val(name) & MASK24, 24) << 24
        v = self.get_reg(name)
        if name in ("x0", "x1", "y0", "y1"):
            return sext(v, 24) << 24
        return sext(v, 56)

    def is_acc_src(self, name):
        return name in ("a", "b", "a0", "a1", "a2", "b0", "b1", "b2")

    # ---------------- register access ----------------
    def get_reg(self, name):
        if name in ("a", "b"):
            return self.A if name == "a" else self.B
        if name == "a0": return self.A & MASK24
        if name == "b0": return self.B & MASK24
        if name == "a1": return (self.A >> 24) & MASK24
        if name == "b1": return (self.B >> 24) & MASK24
        if name == "a2": return (self.A >> 48) & 0xFF
        if name == "b2": return (self.B >> 48) & 0xFF
        if name == "x0": return self.x0
        if name == "x1": return self.x1
        if name == "y0": return self.y0
        if name == "y1": return self.y1
        if name.startswith("r"): return self.R[int(name[1])]
        if name.startswith("n"): return self.N[int(name[1]) & 7]
        if name.startswith("m"): return self.M[int(name[1])]
        if name == "x": return (self.x1 << 24) | self.x0
        if name == "y": return (self.y1 << 24) | self.y0
        if name in ("sr", "ccr"): return self.sr_int & 0xFFFF
        raise EmuError("get_reg %s" % name)

    def set_reg(self, name, val):
        if name in ("a", "b"):
            if name == "a": self.A = sext(val, 56)
            else: self.B = sext(val, 56)
            return
        if name == "a0": self.A = (self.A & ~MASK24) | (val & MASK24); return
        if name == "b0": self.B = (self.B & ~MASK24) | (val & MASK24); return
        if name == "a1": self.A = (self.A & ~(MASK24 << 24)) | ((val & MASK24) << 24); return
        if name == "b1": self.B = (self.B & ~(MASK24 << 24)) | ((val & MASK24) << 24); return
        if name == "a2": self.A = (self.A & ~(0xFF << 48)) | ((val & 0xFF) << 48); return
        if name == "b2": self.B = (self.B & ~(0xFF << 48)) | ((val & 0xFF) << 48); return
        if name == "x0": self.x0 = val & MASK24; return
        if name == "x1": self.x1 = val & MASK24; return
        if name == "y0": self.y0 = val & MASK24; return
        if name == "y1": self.y1 = val & MASK24; return
        if name.startswith("r"): self.R[int(name[1])] = val & MASK24; return
        if name.startswith("n"): self.N[int(name[1]) & 7] = sext(val, 24); return
        if name.startswith("m"): self.M[int(name[1])] = val & MASK24; return
        if name == "x": self.x1 = (val >> 24) & MASK24; self.x0 = val & MASK24; return
        if name == "y": self.y1 = (val >> 24) & MASK24; self.y0 = val & MASK24; return
        raise EmuError("set_reg %s" % name)

    # ---------------- EA / moves ----------------
    EA_RE = re.compile(r"^([xy l])(?::)\(?([^)]*)\)?$")

    def parse_ea(self, tok):
        """Return (space, mode, rnum, arg, arg2) — evaluated later.
        Modes: reg / post+ / post- / pre- / pre+ / disp (incl. index nN, no update) / abs"""
        tok = tok.replace("?", "")   # strip I/O-short marker (l:?:>$fe -> l:>$fe)
        m = re.match(r"^([xyl]):(.+)$", tok)
        if not m:
            return None
        space = m.group(1)
        inner = m.group(2).strip()
        # pre-increment/decrement: "-(r0)" / "+(r0)"
        mm = re.match(r"^([+-])\((r\d)\)$", inner)
        if mm:
            return (space, "pre+" if mm.group(1) == "+" else "pre-",
                    int(mm.group(2)[1]), 1 if mm.group(1) == "+" else -1, None)
        # (rN) optionally with inside displacement/index, optionally followed by post-update
        mm = re.match(r"^\((r\d)\)((?:[+-].*)?)$", inner)
        if mm:
            rnum = int(mm.group(1)[1])
            after = mm.group(2)
            if after == "":
                return (space, "reg", rnum, 0, None)
            if after == "+":
                return (space, "post+", rnum, 1, None)
            if after == "-":
                return (space, "post-", rnum, -1, None)
            mm2 = re.match(r"^\+n(\d)$", after)
            if mm2:
                return (space, "post+", rnum, None, "n" + mm2.group(1))
            mm2 = re.match(r"^-n(\d)$", after)
            if mm2:
                return (space, "post-", rnum, None, "n" + mm2.group(1))
            raise EmuError("ea %r" % tok)
        # (rN+disp) / (rN-disp) / (rN+nN) — index/displacement, NO update
        mm = re.match(r"^\((r\d)([+-].+)\)$", inner)
        if mm:
            rnum = int(mm.group(1)[1])
            return (space, "disp", rnum, mm.group(2), None)
        # absolute (with optional I/O short marker '?')
        v = inner.lstrip(":?$<>")
        return (space, "abs", None, v, None)

    def eval_ea(self, ea):
        space, mode, rnum, arg, arg2 = ea
        if mode == "abs":
            return ("abs", self.abs_val(arg))
        r = self.R[rnum] if rnum is not None else 0
        if mode == "reg":
            return ("r", r, rnum, 0)
        if mode in ("pre+", "pre-"):
            # pre-update: apply now, EA = updated value
            self.upd_r(rnum, arg)
            return ("r", self.R[rnum], rnum, 0)
        if mode in ("post+", "post-"):
            delta = arg if arg is not None else self.N[int(arg2[1])]
            return ("upd", r, rnum, delta)
        if mode == "disp":
            a = str(arg)
            if re.match(r"^[+-]n\d$", a):
                v = self.N[int(a[2])]
                if a[0] == "-":
                    v = -v
            else:
                v = self.abs_val(a)
            return ("disp", (r + v) & MASK24, rnum, 0)
        raise EmuError("eval_ea %s" % (ea,))

    def abs_val(self, v):
        v = str(v)
        if v.startswith("+"):
            v = v[1:]
        neg = v.startswith("-")
        if neg:
            v = v[1:]
        v = v.lstrip("$#<>")
        val = int(v, 16) if v else 0
        return -val if neg else val

    def upd_r(self, rnum, delta):
        if rnum is None:
            return
        m = self.M[rnum]
        r = self.R[rnum]
        if m == MASK24:
            self.R[rnum] = (r + delta) & MASK24
        else:
            base = r & ~m
            off = ((r & m) + delta) % (m + 1)
            self.R[rnum] = base | off

    # ---------------- ALU ----------------
    def sgn24(self, v):
        return v - (1 << 24) if v & 0x800000 else v

    def set_flags_add(self, res, a_in, b_in):
        f = self.f
        f["n"] = (res >> 55) & 1
        f["z"] = 1 if (res & MASK56) == 0 else 0
        f["c"] = 1 if res < -(1 << 55) or res >= (1 << 55) else 0
        f["c"] = 1 if (res >> 55) != 0 and res > 0 else f["c"]
        # simpler: recompute properly below
        return res

    def _flags_from(self, res):
        """Normalize 56-bit result and set NZE."""
        res56 = sext(res, 56)
        self.f["n"] = 1 if res56 < 0 else 0
        self.f["z"] = 1 if res56 == 0 else 0
        # E: A2 != sign-extension of A1 MSB
        a1msb = (res56 >> 47) & 1
        a2 = (res56 >> 48) & 0xFF
        sign_ext = 0xFF if a1msb else 0x00
        self.f["e"] = 1 if a2 != sign_ext else 0
        return res56

    def alu_add(self, dst, x, y, carry=0, store=True, rev=False):
        a = x + y + carry
        # V for 56-bit two's complement overflow
        sx, sy, sr = sext(x, 56) < 0, sext(y, 56) < 0, sext(a, 56) < 0
        self.f["v"] = 1 if (sx == sy and sr != sx) else 0
        self.f["c"] = 1 if (a >> 55) != 0 else 0
        a = self._flags_from(a)
        if store:
            self.set_reg(dst, a)
        return a

    def alu_sub(self, dst, x, y, store=True):
        """x - y"""
        a = x - y
        sx, sy, sr = sext(x, 56) < 0, sext(y, 56) < 0, sext(a, 56) < 0
        self.f["v"] = 1 if (sx != sy and sr != sx) else 0
        self.f["c"] = 0 if a < 0 else 1
        a = self._flags_from(a)
        if store:
            self.set_reg(dst, a)
        return a

    def rnd_acc(self, name):
        v = self.get_reg(name)
        v = v + 0x800000          # 0.5 LSB of A1
        v = sext(v, 56)
        v = (v >> 24 << 24)       # A0 = 0
        self.set_reg(name, v)
        self._flags_from(v)

    # ---------------- main step ----------------
    def step(self):
        pc = self.pc
        ins = self.prog.get(pc)
        if ins is None:
            self.halted = True
            raise EmuError("no instruction at %06X" % pc)
        mnem, toks, text = ins
        self.steps += 1
        if self.steps > self.max_steps:
            raise EmuError("step limit")
        if self.trace:
            self.trace_log.append("%06X: %s | A=%014X B=%014X x0=%06X x1=%06X y0=%06X y1=%06X" %
                                  (pc, text, self.A & MASK56, self.B & MASK56, self.x0, self.x1, self.y0, self.y1))
        next_pc = self.next_addr.get(pc, pc + 1)
        handled = self.exec(mnem, toks, pc)
        if handled is None:
            self.pc = next_pc
        else:
            self.pc = handled
        # DO-loop management (after the end instruction executed)
        # la may be the SECOND word of a 2-word instruction (e.g. move #imm,rN
        # spanning la-1..la): the hardware loop ends after the instruction
        # whose word range covers la.
        for entry in list(self.do_stack):
            if pc <= entry[2] < next_pc:
                entry[0] -= 1
                if entry[0] > 0:
                    self.pc = entry[1]
                else:
                    self.do_stack.remove(entry)
                break

    # ---------------- instruction dispatch ----------------
    MAC_OPS = {"mac", "mpy", "macr", "mpyr", "macsu", "mpysu", "mpyuu", "dmac",
               "maci", "mpyi", "mpyri", "macri"}

    def exec(self, mnem, toks, pc):
        f = self.f

        # ---- DO ----
        if mnem == "do":
            if "," in toks[0]:
                cnt_tok, end_tok = toks[0].split(",", 1)
            else:
                cnt_tok, end_tok = toks[0], toks[-1]
            end_tok = end_tok.lstrip(">")
            end = self.labels[end_tok] if end_tok in self.labels else self.abs_val(end_tok)
            end -= 1  # printed end is one past; ext = last body instr
            if cnt_tok.startswith("#"):
                cnt = self.abs_val(cnt_tok)
            elif re.match(r"^n\d$", cnt_tok):
                cnt = self.get_reg(cnt_tok) & MASK24
            elif cnt_tok in ("a1", "b1"):
                cnt = self.get_reg(cnt_tok) & MASK24
            else:
                raise EmuError("do cnt %s" % cnt_tok)
            if cnt <= 0:
                # zero-count: executes body once per DSP56300 spec? (hardware: skips)
                return end + 1
            self.do_stack.append([cnt, pc + 2, end])  # DO occupies 2 words (opcode+ext)
            return None

        # ---- bit ops ----
        if mnem in ("btst", "bset", "bclr", "bchg"):
            if "," in toks[0]:
                bit_tok, loc = toks[0].split(",", 1)
            else:
                bit_tok, loc = toks[0], toks[1]
            bit = self.abs_val(bit_tok)
            if loc in ("a", "b"):
                # bit-manipulation on accumulator addresses B1 (main 24-bit part)
                acc = self.get_reg(loc)
                main = (acc >> 24) & MASK24
                bitval = (main >> bit) & 1
                f["c"] = bitval
                if mnem == "bset":
                    self.set_reg(loc, (acc & ~((1 << bit) << 24)) | ((1 << bit) << 24))
                elif mnem == "bclr":
                    self.set_reg(loc, acc & ~((1 << bit) << 24))
                elif mnem == "bchg":
                    self.set_reg(loc, acc ^ ((1 << bit) << 24))
                return None
            if re.match(r"^[ab]\d?$", loc) or loc in SIMPLE_REGS:
                val = self.get_reg(loc)
                bitval = (val >> bit) & 1
                if mnem == "btst":
                    f["c"] = bitval
                elif mnem == "bset":
                    f["c"] = bitval
                    if loc == "sr":
                        self.sr_int |= (1 << bit)
                        if bit == 0: f["c"] = 1
                        elif bit == 1: f["v"] = 1
                        elif bit == 2: f["z"] = 1
                        elif bit == 3: f["n"] = 1
                    else:
                        self.set_reg(loc, val | (1 << bit))
                elif mnem == "bclr":
                    f["c"] = bitval
                    if loc == "sr":
                        self.sr_int &= ~(1 << bit)
                        if bit == 0: f["c"] = 0
                        elif bit == 1: f["v"] = 0
                        elif bit == 2: f["z"] = 0
                        elif bit == 3: f["n"] = 0
                    else:
                        self.set_reg(loc, val & ~(1 << bit))
                else:
                    f["c"] = bitval
                    self.set_reg(loc, val ^ (1 << bit))
                return None
            ea = self.parse_ea(loc)
            ev = self.eval_ea(ea)
            val = self.rd(ea[0], ev[1])
            bitval = (val >> bit) & 1
            f["c"] = bitval
            if mnem == "bset":
                self.wr(ea[0], ev[1], val | (1 << bit))
            elif mnem == "bclr":
                self.wr(ea[0], ev[1], val & ~(1 << bit))
            elif mnem == "bchg":
                self.wr(ea[0], ev[1], val ^ (1 << bit))
            return None

        # ---- branches / jumps / calls ----
        if mnem in ("jmp", "bra"):
            return self.branch_target(toks[0])
        if mnem == "jsr":
            tgt = self.branch_target(toks[0])
            self.ret_stack.append(pc + 1)
            return tgt
        if mnem == "rts":
            return self.ret_stack.pop()
        if mnem.startswith("b") and mnem not in ("bset", "bclr", "btst", "brset", "brclr", "bchg") and len(toks) == 1:
            cond = mnem[1:]
            if cond in BR_CONDITIONS:
                if BR_CONDITIONS[cond](f):
                    return self.branch_target(toks[0])
                return None
        if mnem in ("jset", "jclr", "brset", "brclr", "jsset", "jsclr"):
            if len(toks) == 1:
                bt, loc, tgt = toks[0].split(",", 2)
            else:
                bt, loc, tgt = toks[0], toks[1], toks[2]
            bit = self.abs_val(bt)
            if re.match(r"^[ab]$", loc):
                val = (self.get_reg(loc) >> 24) & MASK24
            else:
                val = self.get_reg(loc)
            bitval = (val >> bit) & 1
            taken = bitval == 1 if mnem.endswith("set") else bitval == 0
            if mnem in ("brset", "brclr") and taken:
                return self.branch_target(tgt)
            if mnem in ("jset", "jclr", "jsset", "jsclr") and taken:
                if mnem.startswith("js"):
                    self.ret_stack.append(pc + 1)
                return self.branch_target(tgt)
            return None

        # ---- ANDI/ORI ccr ----
        if mnem in ("andi", "ori"):
            v = self.abs_val(toks[0].split(",")[0])
            cur = (f["c"] | (f["v"] << 1) | (f["z"] << 2) | (f["n"] << 3))
            if mnem == "andi":
                cur &= v
            else:
                cur |= v
            f["c"] = cur & 1
            f["v"] = (cur >> 1) & 1
            f["z"] = (cur >> 2) & 1
            f["n"] = (cur >> 3) & 1
            return None

        # ---- MOVE (standalone, 1-2 parallel moves) ----
        if mnem == "move":
            self.do_moves(toks)
            return None

        # ---- LUA ----
        if mnem == "lua":
            if "," in toks[0]:
                src_tok, dst = toks[0].split(",", 1)
            else:
                src_tok, dst = toks[0], toks[1]
            m = re.match(r"^\((r\d)\)(\+|-)?(n\d)?$", src_tok)
            if m:
                rnum = int(m.group(1)[1])
                delta = 0
                if m.group(2) == "+":
                    delta = self.N[int(m.group(3)[1])] if m.group(3) else 1
                elif m.group(2) == "-":
                    delta = -self.N[int(m.group(3)[1])] if m.group(3) else -1
            else:
                m2 = re.match(r"^\((r\d)([+-]\$?\w+)\)$", src_tok)
                if not m2:
                    raise EmuError("lua %r" % src_tok)
                rnum = int(m2.group(1)[1])
                delta = self.abs_val(m2.group(2))
            self.set_reg(dst, (self.R[rnum] + delta) & MASK24)
            return None

        # ---- NOP/PFLUSH/ENDDO etc ----
        if mnem in ("nop", "pflush", "debug", "wait", "stop"):
            return None

        # ---- RND ----
        if mnem == "rnd":
            dst = toks[0].split(",")[0] if toks else "a"
            self.rnd_acc(dst)
            # remaining tokens: parallel moves
            if len(toks) > 1:
                self.do_moves(toks[1:])
            return None

        # ---- DIV ----
        if mnem == "div":
            if "," in toks[0]:
                s_tok, d_tok = toks[0].split(",")
            else:
                s_tok, d_tok = toks[0], toks[1]
            s = self.sgn24(self.get_reg(s_tok))
            d1 = (self.get_reg(d_tok) >> 24) & MASK24
            if (s & MASK24) == 0:
                # hardware DIV by zero saturates to max; keep running
                self.set_reg(d_tok, 0xFFFFFFFFFFFF & MASK48)
                return None
            # DSP56300 DIV: ONE non-restoring division step.
            # 1) shift D1:D0 left 1 (48-bit)
            D = ((self.get_reg(d_tok) >> 24) & MASK24) << 24 | (self.get_reg(d_tok) & MASK24)
            d1 = (D >> 24) & MASK24
            d0 = D & MASK24
            d1 = ((d1 << 1) | (d0 >> 23)) & MASK24
            d0 = (d0 << 1) & MASK24
            s_signed = sext(s & MASK24, 24)
            d1_signed = sext(d1, 24)
            # 2) same sign -> subtract, else add
            if (d1_signed < 0) == (s_signed < 0):
                d1 = (d1 - (s & MASK24)) & MASK24
            else:
                d1 = (d1 + (s & MASK24)) & MASK24
            # 3) quotient bit: 1 if new D1 sign == S sign
            qbit = 1 if (sext(d1, 24) < 0) == (s_signed < 0) else 0
            d0 |= qbit
            self.set_reg(d_tok, (d1 << 24) | d0)
            f["c"] = qbit ^ 1
            return None

        # ---- ALU ops with optional parallel moves ----
        if mnem in self.MAC_OPS or mnem in ("add", "sub", "subr", "addr", "addl", "subl",
                                            "addc", "sbc", "tfr", "cmp", "cmpm", "tst",
                                            "neg", "negc", "abs", "clr", "not", "and",
                                            "or", "eor", "asl", "asr", "lsl", "lsr",
                                            "rol", "ror", "tge", "tgt", "tle", "tlt",
                                            "teq", "tne", "tpl", "tmi", "clb", "normf",
                                            "max", "maxm", "insert"):
            return self.exec_alu(mnem, toks, pc)

        raise EmuError("unimplemented %s %s at %06X" % (mnem, " ".join(toks), pc))

    def div48(self, dividend, divisor):
        """DSP56300 DIV loop x24 for unsigned: returns (q24, r24) with (dividend48/divisor) in 24.24.
        dividend is 48-bit, divisor 24-bit (>0). Equivalent to 24 non-restoring steps."""
        if divisor == 0:
            raise EmuError("div0")
        q = dividend // divisor
        r = dividend % divisor
        # 24.24: q fits in 48 bits; hardware produces q in D1:D0 as (dividend<<24)/divisor
        q48 = (dividend << 24) // divisor
        return (q48 >> 24) & MASK24, q48 & MASK24

    def branch_target(self, tok):
        tok = tok.lstrip(">$<")
        if tok in self.labels:
            return self.labels[tok]
        m = re.match(r"^\((r\d)\)$", tok)
        if m:
            return self.R[int(m.group(1)[1])] & MASK24
        return self.abs_val(tok)

    # ---------------- parallel moves ----------------
    def commit_writes(self, latch, eas):
        for sval, srckind, src, dst in latch:
            if dst in ("ba", "ab") and srckind == "meml":
                # L pair -> {B1,A1} (ba) or {A1,B1} (ab)
                xw = (sval >> 24) & MASK24
                yw = sval & MASK24
                first, second = ("b", "a") if dst == "ba" else ("a", "b")
                self.set_reg(first, sext(xw, 24) << 24)
                self.set_reg(second, sext(yw, 24) << 24)
                continue
            if re.match(r"^[ab]\d?$", dst) or dst in ("x0", "x1", "y0", "y1", "x", "y") \
               or re.match(r"^(r\d|n\d|m\d)$", dst):
                if dst in ("a", "b") and srckind == "meml":
                    sval = sext(sval & MASK48, 48)
                elif dst in ("a", "b") and not self.is_acc_src(src):
                    sval = self.compose_acc(sval)
                elif dst in ("a", "b") and src in ("a0", "b0"):
                    acc = self.A if dst == "a" else self.B
                    sval = (acc & ~(MASK24 << 24)) | (self.acc_part_for_dst(src, sval) << 24)
                elif dst in ("a", "b") and src in ("a1", "b1"):
                    acc = self.A if dst == "a" else self.B
                    sval = (acc & ~MASK48) | (self.acc_part_for_dst(src, sval) << 24)
                elif re.match(r"^[rn]\d$|^[rn][0-7]$", dst) and self.is_acc_src(src):
                    sval = self.acc_part_for_dst(src, sval)
                elif dst in ("x0", "x1", "y0", "y1") and self.is_acc_src(src):
                    sval = self.acc_part_for_dst(src, sval)
                self.set_reg(dst, sval)
            else:
                ea = self.parse_ea(dst)
                ev = self.eval_ea(ea)
                eas.append(ea)
                if ea[0] == "l":
                    if src in ("ba", "ab"):
                        self.wrL(ev[1], sval)
                    elif self.is_acc_src(src):
                        if src in ("a", "b"):
                            self.wrL(ev[1], sval & MASK48)
                        elif src in ("a0", "b0"):
                            self.wr("y", ev[1], sval & MASK24)
                        elif src in ("a1", "b1"):
                            self.wr("x", ev[1], (sval >> 24) & MASK24)
                        else:
                            self.wr("x", ev[1], self.acc_part_for_dst(src, sval))
                    else:
                        self.wrL(ev[1], sval)
                else:
                    if self.is_acc_src(src):
                        sval = self.acc_part_for_dst(src, sval)
                    self.wr(ea[0], ev[1], sval & MASK24)
        for ea in eas:
            space, mode, rnum, arg, arg2 = ea
            if mode in ("post+", "post-"):
                delta = arg if arg is not None else self.N[int(arg2[1])]
                self.upd_r(rnum, delta)

    def do_moves(self, toks):
        latch = []   # (sval, srckind, srcname, dst)
        eas = []
        for tk in toks:
            if tk.startswith("if"):
                continue
            if tk.count(",") == 0 and re.match(r"^\((r\d)\)([+-](n\d)?)?$", tk):
                mm = re.match(r"^\((r\d)\)([+-](n\d)?)?$", tk)
                rnum = int(mm.group(1)[1])
                upd = mm.group(2)
                if upd == "":
                    continue
                if upd == "+":
                    eas.append(("", "post+", rnum, 1, None))
                elif upd == "-":
                    eas.append(("", "post-", rnum, -1, None))
                elif upd.startswith("+n"):
                    eas.append(("", "post+", rnum, None, upd[1:]))
                elif upd.startswith("-n"):
                    eas.append(("", "post-", rnum, None, upd[1:]))
                continue
            if tk.count(",") == 0 and tk.startswith("("):
                # bare address-register update: move (r2)+n2
                eas.append(self.parse_ea("x:" + tk))
                self.commit_writes(latch, eas)
                return
            if tk.count(",") != 1:
                raise EmuError("bad move %r" % tk)
            src, dst = tk.split(",")
            if src.startswith("#"):
                sval = self.abs_val(src) & MASK24
                srckind = "imm"
            elif re.match(r"^(x0|x1|y0|y1|x|y|a|b|r\d|n\d|m\d)$", src) \
                    or re.match(r"^[ab]\d$", src):
                # a0/a1/a2/b0/b1/b2 latch the RAW 56-bit accumulator; the
                # part extraction happens once, in commit_writes.
                if re.match(r"^[ab]\d$", src):
                    sval = self.get_reg(src[0])
                else:
                    sval = self.get_reg(src)
                srckind = "reg"
            elif src in ("ba", "ab"):
                sval = ((self.B >> 24) & MASK24) << 24 | (self.A >> 24) & MASK24
                srckind = "reg48"
            else:
                ea = self.parse_ea(src)
                ev = self.eval_ea(ea)
                eas.append(ea)
                sval = self.rd_mem(ea[0], ev[1])
                srckind = "meml" if ea[0] == "l" else "mem"
            latch.append((sval, srckind, src, dst))
        self.commit_writes(latch, eas)

    # ---------------- ALU with parallel moves ----------------
    def exec_alu(self, mnem, toks, pc):
        f = self.f
        toks = list(toks)
        # split condition suffix
        cond = None
        if toks and toks[-1].startswith("if"):
            cond = toks[-1]
            toks = toks[:-1]
        # classify tokens: ALU operand tokens first
        alu_toks = []
        move_toks = []
        if mnem in self.MAC_OPS:
            commas = 0
            while toks and commas < 2:
                tk = toks.pop(0)
                commas += tk.count(",")
                alu_toks.append(tk)
        elif mnem in ("add", "sub", "addc", "sbc", "subr", "addr", "addl", "subl",
                      "tfr", "cmp", "cmpm", "and", "or", "eor", "tge", "tgt", "tle",
                      "tlt", "teq", "tne", "tpl", "tmi", "clb", "normf", "max", "maxm",
                      "insert"):
            commas = 0
            while toks and commas < 1:
                tk = toks.pop(0)
                commas += tk.count(",")
                alu_toks.append(tk)
        elif mnem in ("asl", "asr", "lsl", "lsr", "rol", "ror"):
            # single token: '#n,S,D' / '#n,D' / 'S,D' / 'D'
            alu_toks.append(toks.pop(0))
        elif mnem in ("neg", "negc", "abs", "clr", "not", "tst"):
            alu_toks.append(toks.pop(0) if toks else "")
        move_toks = toks

        # ---- latch parallel move reads ----
        latch = []
        eas = []
        for tk in move_toks:
            # pointer-update-only parallel move, e.g. '(r0)+', '(r4)+n2', '(r0)-'
            if tk.count(",") == 0 and re.match(r"^\((r\d)\)([+-](n\d)?)?$", tk):
                mm = re.match(r"^\((r\d)\)([+-](n\d)?)?$", tk)
                rnum = int(mm.group(1)[1])
                upd = mm.group(2)
                if upd == "":
                    continue
                if upd == "+":
                    eas.append(("", "post+", rnum, 1, None))
                elif upd == "-":
                    eas.append(("", "post-", rnum, -1, None))
                elif upd.startswith("+n"):
                    eas.append(("", "post+", rnum, None, upd[1:]))
                elif upd.startswith("-n"):
                    eas.append(("", "post-", rnum, None, upd[1:]))
                continue
            if tk.count(",") != 1:
                raise EmuError("bad par move %s at %06X" % (tk, pc))
            src, dst = tk.split(",")
            if src.startswith("#"):
                sval = self.abs_val(src) & MASK24
                srckind = "imm"
            elif src in ("ba", "ab"):
                sval = ((self.B >> 24) & MASK24) << 24 | (self.A >> 24) & MASK24
                srckind = "reg48"
            elif re.match(r"^(x0|x1|y0|y1|x|y|a|b|r\d|n\d|m\d)$", src) \
                    or re.match(r"^[ab]\d$", src):
                # a0/a1/a2/b0/b1/b2 latch the RAW 56-bit accumulator; the
                # part extraction happens once, in commit_writes.
                if re.match(r"^[ab]\d$", src):
                    sval = self.get_reg(src[0])
                else:
                    sval = self.get_reg(src)
                srckind = "reg"
            else:
                ea = self.parse_ea(src)
                ev = self.eval_ea(ea)
                eas.append(ea)
                sval = self.rd_mem(ea[0], ev[1])
                srckind = "meml" if ea[0] == "l" else "mem"
            latch.append((sval, srckind, src, dst))

        # ---- condition gate ----
        run_alu = True
        if cond is not None:
            run_alu = CONDITIONS[cond](f)

        # ---- ALU ----
        if run_alu:
            self.alu_exec(mnem, alu_toks, pc)

        # ---- writes (pre-ALU latched values) ----
        self.commit_writes(latch, eas)
        return None

    def alu_exec(self, mnem, toks, pc):
        f = self.f
        if mnem in self.MAC_OPS:
            # parse operands
            sign2 = None
            if toks and toks[0] in ("ss", "su", "us", "uu"):
                sign2 = toks.pop(0)
            if toks and toks[0].startswith("-") and "," in toks[0]:
                core = toks[0][1:]
                neg = True
            else:
                core = toks[0]
                neg = False
            parts = core.split(",")
            if len(parts) == 3:
                s1, s2, dst = parts
            elif len(parts) == 2 and toks[1:]:
                s1, s2 = parts
                dst = toks[1]
            else:
                raise EmuError("mac ops %s" % toks)
            if mnem.endswith("i"):
                # immediate operand (mpyi/maci/mpyri): value from the token
                v1 = self.abs_val(s1) & MASK24
                v2 = self.get_reg(s2)
            else:
                v1 = self.get_reg(s1)
                v2 = self.get_reg(s2)
            if neg:
                v1 = (-self.sgn24(v1)) & MASK24
            # signedness: mpyuu = u*u; macsu/mpysu = s*u; dmac ss/su/us/uu
            if mnem == "mpyuu" or (sign2 == "uu"):
                prod = (v1 & MASK24) * (v2 & MASK24)
            elif (mnem in ("macsu", "mpysu") and not sign2) or sign2 == "su":
                a1 = v1 & MASK24
                s1v = a1 - (1 << 24) if a1 & 0x800000 else a1
                prod = s1v * (v2 & MASK24)
            elif sign2 == "us":
                a2 = v2 & MASK24
                s2v = a2 - (1 << 24) if a2 & 0x800000 else a2
                prod = (v1 & MASK24) * s2v
            else:
                a1 = v1 & MASK24
                a2 = v2 & MASK24
                s1v = a1 - (1 << 24) if a1 & 0x800000 else a1
                s2v = a2 - (1 << 24) if a2 & 0x800000 else a2
                prod = s1v * s2v
            # DSP56300 fractional MAC: the 24x24 product is left-shifted 1 bit
            prod <<= 1
            acc = self.get_reg(dst)
            if mnem in ("mac", "macr", "maci", "macsu", "dmac", "macri"):
                res = acc + prod
            else:
                res = prod
            res56 = self._flags_from(res)
            self.f["v"] = 1 if not (-(1 << 47) <= res56 < (1 << 47)) else 0
            self.set_reg(dst, res56)
            if mnem in ("macr", "mpyr", "mpyri", "macri"):
                self.rnd_acc(dst)
            return

        # non-MAC ALU
        if mnem in ("add", "sub", "subr", "addr", "addc", "sbc", "addl", "subl",
                    "tfr", "cmp", "cmpm", "and", "or", "eor", "tge", "tgt", "tle",
                    "tlt", "teq", "tne", "tpl", "tmi", "clb", "normf", "max", "maxm",
                    "insert"):
            core = toks[0]
            parts = core.split(",")
            if len(parts) == 2:
                s, d = parts
            elif len(parts) == 1 and len(toks) > 1:
                s, d = parts[0], toks[1]
            else:
                s, d = None, parts[0]
            if mnem == "add":
                self.alu_add(d, self.get_reg(d), self.alu_src(s))
            elif mnem == "addc":
                self.alu_add(d, self.get_reg(d), self.alu_src(s), carry=f["c"])
            elif mnem == "sub":
                self.alu_sub(d, self.get_reg(d), self.alu_src(s))
            elif mnem == "subr":
                self.alu_sub(d, self.alu_src(s), self.get_reg(d))
            elif mnem == "addr":
                self.alu_add(d, self.alu_src(s), self.get_reg(d))
            elif mnem == "sbc":
                self.alu_sub(d, self.get_reg(d), self.alu_src(s) + ((1 << 24) if f["c"] else 0))
            elif mnem in ("tfr",):
                self.set_reg(d, self.alu_src(s))
            elif mnem in ("cmp", "cmpm"):
                self.alu_sub(d, self.get_reg(d), self.alu_src(s), store=False)
            elif mnem in ("and", "or", "eor"):
                a1 = (self.get_reg(d) >> 24) & MASK24
                b = (self.abs_val(s) if s.startswith("#") else self.get_reg(s)) & MASK24
                if mnem == "and": r = a1 & b
                elif mnem == "or": r = a1 | b
                else: r = a1 ^ b
                r56 = (self.get_reg(d) & ~MASK48) | (r << 24)
                self.f["n"] = 1 if sext(r56, 56) < 0 else 0
                self.f["z"] = 1 if r == 0 else 0
                self.set_reg(d, r56)
            elif mnem in ("tge", "tgt", "tle", "tlt", "teq", "tne", "tpl", "tmi"):
                conds = {"tge": "ifge", "tgt": "ifgt", "tle": "ifle", "tlt": "iflt",
                         "teq": "ifeq", "tne": "ifne", "tpl": "ifpl", "tmi": "ifmi"}
                if CONDITIONS[conds[mnem]](f):
                    self.set_reg(d, self.get_reg(s))
            elif mnem in ("clb", "normf", "max", "maxm", "insert"):
                raise EmuError("unimplemented alu %s at %06X" % (mnem, pc))
            return

        if mnem in ("asl", "asr", "lsl", "lsr", "rol", "ror"):
            # forms (alu_toks may be 1 token): '#n,S,D' / '#n,D' / 'S,D' / 'D'
            fields = []
            for tk in toks:
                fields.extend(tk.split(","))
            if fields[0].startswith("#"):
                cnt = self.abs_val(fields[0]) & 63
                rest = fields[1:]
            else:
                cnt = None
                rest = fields
            if len(rest) == 3:
                # dynamic count from register: 'asl x0,a,a'
                cnt = self.get_reg(rest[0]) & 63
                s, d = rest[1], rest[2]
            elif len(rest) == 2:
                s, d = rest
                if cnt is None:
                    cnt = 1 if not rest[0].isdigit() else 1
            elif len(rest) == 1:
                s = d = rest[0]
            else:
                raise EmuError("shift fields %s" % fields)
            if cnt is None:
                if len(rest) == 1:
                    cnt = 1                      # 'asr b' — shift in place by 1
                elif len(rest) == 2:
                    cnt = 1
            v = self.get_reg(s)
            if mnem == "asl":
                r = v << cnt
                self.f["c"] = (v >> (56 - cnt)) & 1 if cnt else 0
            elif mnem == "asr":
                r = v >> cnt
                self.f["c"] = (v >> (cnt - 1)) & 1 if cnt else 0
            elif mnem == "lsl":
                r = (v << cnt) & MASK56
                self.f["c"] = (v >> (56 - cnt)) & 1 if cnt else 0
            elif mnem == "lsr":
                r = (v & MASK56) >> cnt
                self.f["c"] = (v >> (cnt - 1)) & 1 if cnt else 0
            else:
                raise EmuError("shift %s" % mnem)
            r = self._flags_from(r)
            self.set_reg(d, r)
            return

        if mnem in ("neg", "negc", "abs", "clr", "not", "tst"):
            d = toks[0] if toks else "a"
            if mnem == "clr":
                v = 0
                self.f["u"] = 0
            elif mnem == "neg":
                v = self._flags_from(-self.get_reg(d))
                self.f["v"] = 1 if sext(v, 56) == -(1 << 55) else 0
                self.set_reg(d, v)
                return
            elif mnem == "abs":
                v = abs(self.get_reg(d))
            elif mnem == "not":
                v = ~self.get_reg(d)
            elif mnem == "tst":
                self._flags_from(self.get_reg(d))
                return
            v = self._flags_from(v)
            self.set_reg(d, v)
            return
        raise EmuError("alu_exec fallthrough %s" % mnem)

    # ---------------- run ----------------
    def run(self, start, end=None, max_steps=None):
        self.pc = start
        self.halted = False
        n = 0
        limit = max_steps or self.max_steps
        while not self.halted and n < limit:
            pc = self.pc
            if end is not None and pc == end:
                return n
            if pc not in self.prog:
                self.halted = True
                raise EmuError("no instruction at %06X (after %d steps)" % (pc, self.steps))
            self.step()
            n += 1
        return n


if __name__ == "__main__":
    print("module — import from harness scripts")
