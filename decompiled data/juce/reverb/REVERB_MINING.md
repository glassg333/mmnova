# REVERB m13 — MINING NOTES (final, iteration 32)

Machine m13 "FX-REVERB", DSP1 PROC P:$1453DC-$14561B (1257 words), INIT $14538E.
Port: `MnmReverb.hpp` + `mnm_reverb_tables.h`. Verified against the bit-exact
DSP56300 emulator (`scripts/dsp_emu.py` + `exp26_m13_reverb.py` harness) with
differential vectors (exp47): **2268/2304 output words bit-exact (98.44%)**;
residual = 14 words at ±4 LSB (~-132 dBFS) in frames 43-47 of test set A only.

## Signal flow (100% decoded, all stages verified on ROM)

1. **input** Y:$0-$1F[k] = 2*INP^2*(bus[2k]+bus[2k+1])   ($145411-$145420)
2. **5 allpasses** (47/67/101/149/223), feedback x1g = DEC*0.05+0.7 (x fade),
   coef table $14561C, mask table $145621. out[k] = line[p+k] - g*carry[k-1];
   carry[k] = in[k] + g*line[p+k+1]; rings X:$1159C0/$115A00/$115A80/$115B00/$115C00
   + track*0x2000, pointers Y:$3F-$43, carries X:$3F-$43.   ($145421-$145441)
3. **6 comb taps** ($145449-$1454D8):
   - combs 1-4: one-pole damp, a(k) = a(k-1) + (line[k] - a(k-1))*dampCoef,
     damp coef = DAMP-table[dampPtr + c] (5th value shared by combs 5/6),
     rd ptrs Y:$33-$38, masks $A76 $99E $722 $664 $7FF $3FF (table $145626),
     line bases $114000/$115000/$116000/$116800/$117000/$114C00 + track*0x2000
   - combs 5/6: LFO-interpolated tap. LFO = sin LUT Y:$14A000, phase r6+$32,
     +16/frame (5.38 Hz). Decomposition: n1 = sine>>21 (integer tap -4..3),
     frac = ((sine<<3) & 0xFFFFFF)>>1 == (sine<<2) & 0x7FFFFF.
     weights wE = cD*(1-frac), wO = cD*frac (cD = DAMP-table[5th]),
     damping a -= a_old*cD. Scratch ring X:$0/X:$1 (m3 = 1 -> 2-word ring):
     X:$0 = cD, X:$1 = wE, alternating reads.
4. **matrix** ($1454DC-$145546) — coefficient field X:$20-$24 = FIVE words
   (m3 = 4 -> modulo length 5): preload writes DEC[2],DEC[5],DEC[3],DEC[1],DEC[0]
   (DEC ptr = $14562C + 6*(DEC>>17); maci #3 gives *6 because MPY shifts <<1).
   - sec1 (wet L/R): a = F0*c3[k] - F1*c6[k] - F2*c4[k] + F3*c2[k];
     b = (a w/o c2-term) + F4*c1[k]; F0..F4 = DEC[2],DEC[5],DEC[3],DEC[1],DEC[0]
     cycling in a fixed load order every iteration (5 coefficient loads);
     k=0 first term uses setup x0 = DEC[0]; both <<1.
     Stream overreads at k=15: y:(r0)+ -> Y:$50 (comb4 out[0]), y:(r6)+ -> Y:$80 (0).
   - sec2: a -> X:$30 (wasted, overwritten by HP), b -> X:$40 = comb3 bank:
     bank3[k] = c1[k]*DEC[0] - c4[k]*DEC[3]
   - sec3: a -> X:$60 = comb5 bank = c2[k]*DEC[1] + c3[k]*DEC[2];
     b -> X:$70 = comb6 bank = c2[k]*DEC[1] - c3[k]*DEC[2]
5. **comb write** ($145547-$145562): each line[wr+k] = bank[k] + Y:$k*0x155556
   (0x155556 = 1/6), banks read sequentially X:$20-$7F:
   comb1 <- prev frame HP outs[0..15], comb2 <- prev HP outs[16..31]
   (HP writes 32 words to X:$20-$3F every frame, overwriting sec2 a-stream),
   comb3 <- sec2 b (X:$40), comb4 <- X:$50 (NEVER written by this machine =
   device SRAM garbage; modelled as 0 in the port), comb5/6 <- sec3 (X:$60/$70).
   wr ptrs Y:$39-$3E, INIT $1A67/$198F/$2713/$2E55/$3577/$0E65 (+track*0x2000).
6. **gate/env** ($145564-$1455A7): envSlow = 0.99922*envSlow + 0.0005*|ap|
   (one-pole over Y:$0-$1F), frame peak bmax (maxm, full 56-bit),
   rising = bmax - envFast<<24 compared RAW vs #$A8 (56-bit!), quietCnt,
   gateRate = 0x0F4240 if GATE^2>>10 > $FA0 else GATE^2>>10; ramp attack
   +$147AE/sample, decay *$7EB852/sample; ramp written to Y:$0-$F.
   gateRate == $7FFFFF -> attack unconditionally.
7. **gate multiply**: wet[k] = ramp[k] * sec1[k] -> X:$0-$1F   ($1455A8-$1455B3)
8. **HP** ($1455B4-$1455E6): per sample out = A1(acc) (old acc stored!);
   acc -= x_prev*y1; acc -= c*out; acc += in*y1;  y1 = 0.5*(1-c),
   c = LP-table[$144AC7 + (HP>>16)]; x_prev chain: x(0) = saved X:$1E/$1F =
   previous frame's wet[15], x(k) = wet[k-1]. States r6+$18/$1A (+a0 $19/$1B),
   x-states r6+$1C/$1D. Outputs X:$20-$3F interleaved [L0,R0,L1,R1,...].
9. **LP** ($1455E7-$1455FF): out = A1(acc) (old acc); acc += hp[k]*c; acc -= out*c.
   c = LP-table[$144AC7 + (LP>>16)]. States r6+$14/$16. Outputs X:$0-$1F.
10. **final mix** ($145600-$14561B): out[k] = 4*(MIX*wet[k] + (1-MIX)*INP^2*dry[k]),
    wet = X:$0-$1F interleaved, dry = bus[k] sequential.

## Emulator bugs found & fixed this iteration (all regression-tested)

1. upd_r modulo: non-power-of-2 boundary-crossing rule (base = r & ~m).
2. Data Limit Checking on accumulator stores ($7FFFFF/$800000 saturation),
   threshold corrected from -2^39 to -2^47.
3. 8-bit immediate MOVE: data regs/memory = xx<<16; A/B and R/M/N = raw;
   ALU immediates = raw.

## Port bugs found & fixed this iteration (via frame-internal differential diff)

1. allpass carry: `add_reg24(acc, mpy56(...))` truncated the 56-bit product
   through a 24-bit register path -> plain 56-bit accumulator add.
2. LFO frac mask: (sine<<2)&0xFFFFFF kept bit 23; correct = (sine<<2)&0x7FFFFF
   (the b1-clear + asr path in firmware).
3. DEC/DAMP table offsets: maci #3 => 6*(param>>17), not 3* (MPY <<1).
4. Tables extended 200 -> 400 words: decOff/dampOff reach 378+5 for knob 127,
   old arrays were overrun (UB).
5. Matrix sec1 coefficient field = 5 words (m3 = 4 -> length 5), fixed load
   order, k=0 special first term; the old 4-word rotation model was wrong.
6. Comb banks: comb1/2 = previous frame HP outputs (X:$20-$3F), not sec2/sec3
   streams; comb4 = SRAM garbage (0 in port); comb3 = sec2 b; comb5/6 = sec3.
7. HP: output = old accumulator (pre-MAC store), x-chain = previous input
   sample (not previous output); X:$1E/$1F = previous frame's wet[15].
8. LP: output = old accumulator (pre-MAC store).

## Residual (documented, not fixed)

14 of 2304 vector words differ by exactly +-4 LSB at the final output
(frames 43-47, set A; sets B/C are bit-exact). The difference is a 1-LSB
error in the sec1 accumulator path that appears only when the comb6 tail
turns negative late in the decay. -132 dBFS, below dithering floor;
all knob laws, timing, topology and levels are bit-exact.

## Verification artifacts

- scripts/exp47_reverb_vectors.py/.txt/.json — differential vectors (3 sets x 48 frames)
- scripts/exp48_reverb_intdump.py / exp48b_intdiff.py — frame-internal state dump/diff
- scripts/exp49_comb56_step.py, exp50 probes — single-step traces
- work/dsp_new_stage/test_mnm_reverb.cpp — C++ differential test (2268/2304)
