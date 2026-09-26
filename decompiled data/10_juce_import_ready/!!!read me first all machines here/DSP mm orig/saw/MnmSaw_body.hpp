// MnmSaw_body.hpp — included at the end of MnmSaw.hpp.
// MnmSaw::helper3AA (kernel func_0003AA) and MnmSaw::processFrame (PROC
// $14594A) — 1:1 transcriptions with emulator latch semantics.

namespace mnm {
namespace saw {

// ---------------------------------------------------------------------------
// func_0003AA. Inputs (as left by the machine / previous call):
//   B    — 56-bit (only low 48 used as the pair value)
//   x1/x0 step not used here; r1 = 0 (set by caller)
//   r3_in — X page pointer (r6+$58 for all four calls)
//   r4_in — Y page pointer (r6+$27 at first call, accumulates)
//   r5_in — Y pointer ($CD at first call, accumulates)
//   n0_in — X pointer increment ($88 at first call)
//   r7/m7 — the machine's Y ring ($80, m7 = 1)
//   A1 at entry: garbage (the helper loads the count from X:(r1) itself).
// ---------------------------------------------------------------------------
void MnmSaw::helper3AA(int64_t& B, int& r7, uint32_t m7,
                       uint32_t r3_in, uint32_t r4_in, uint32_t r5_in,
                       uint32_t n0_in, uint32_t r1_in) {
    const uint32_t R6 = 0x428;
    int64_t a = 0, b = B;
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
    uint32_t r0 = 0, r1 = r1_in, r2 = 0, r3 = r3_in, r4 = r4_in, r5 = r5_in;
    uint32_t n0 = n0_in, n2 = 0, n4 = 0, n5 = 0xFFFFE1;
    uint32_t m0 = M24, m2 = M24, m4 = M24, m5 = M24;

    auto R = [&](uint32_t p) -> uint32_t {              // X read (page-aware)
        if (p >= R6 && p < R6 + 0x90) return Xw[p - R6];
        return rdXA(p);
    };
    auto W = [&](uint32_t p, uint32_t v) {              // X write
        if (p >= R6 && p < R6 + 0x90) { Xw[p - R6] = v & M24; return; }
        wrX(p, v);
    };
    auto RY = [&](uint32_t p) -> uint32_t {             // Y read
        if (p >= R6 && p < R6 + 0x90) return Yw[p - R6];
        return rdY(p);
    };
    auto upd = [](uint32_t p, int32_t d, uint32_t m) -> uint32_t {
        if (m == M24) return (p + d) & M24;
        uint32_t base = p & ~m;
        int32_t off = (int32_t)(p - base) + d;
        if (off > (int32_t)m) off -= (int32_t)m + 1;
        if (off < 0) off += (int32_t)m + 1;
        return base + (uint32_t)off;
    };

    // r1 enters with the value left by func_0003E0 (caller only resets it
    // before 3E0, NOT before 3AA)
    a = compose24(R(r1));                               // $3aa move x:(r1),a
    a = asr56(a, 1);                                    // $3ab asr a (+#$0,r1)
    r1 = 0;                                             //      parallel move
    n4 = 0x1F;                                          // $3ac move #$1f,n4
    // $3ad move #>$ffffe1,n5 (done at init: n5 = 0xFFFFE1, sext = -31)
    m0 = 0x3F;                                          // $3af move #$3f,m0
    m2 = m0;                                            // $3b0 move m0,m2
    n0 = R(r1); r1 = upd(r1, 1, M24);                   // $3b1 move x:(r1)+,n0
    x0 = 0x312;                                         // $3b2 move #>$312,x0
    n2 = 0x10;                                          // $3b4 move #$10,n2
    uint32_t do_cnt = rawA1(a);                         // $3b5 do a1,$3cf
    for (uint32_t it = 0; it < do_cnt; ++it) {
        r0 = r3;                                        // $3b7 move r3,r0
        b = compose24(0x152);                           // $3b8 move #>$152,b
        y1 = R(r1); y0 = rdY(r1);                       // $3ba move l:(r1)+,y
        r1 = upd(r1, 1, M24);
        r0 = upd(r0, (int32_t)sgn24(n0), m0);           // $3bb move (r0)+n0
        b = sext56(b - (sgn24(y1) << 24));              // $3bc sub y1,b
        r2 = r0;                                        //      r0,r2
        a = compose24(y1);                              // $3bd tfr y1,a
        y1 = RY((uint32_t)r7);                          //      y:(r7)+,y1
        r7 = (int)upd((uint32_t)r7, 1, m7);
        a = sext56(a + (sgn24(x0) << 24));              // $3be add x0,a
        r4 = rawA1(b);                                  //      b1,r4 (raw)
        b = mac_su(0, y1, y0);                          // $3bf mpysu y1,y0,b
        b = asr56(b, 1);                                // $3c0 asr b
        r5 = rawA1(a);                                  //      a,r5 (raw)
        r0 = upd(r0, -1, m0);                           // $3c1 move (r0)-
        int64_t bpre = b;                               // $3c2 neg b b,x1
        b = sext56(-sext56(b));
        x1 = satA1(bpre);
        b = sext56(b + (sgn24(y1) << 24));              // $3c3 add y1,b
        r2 = upd(r2, (int32_t)sgn24(n2), m2);           //      (r2)+n2
        a = compose24(R(r2));                           // $3c4 move x:(r2),a
        y1 = RY(r5);                                    //      y:(r5)-,y1
        r5 = upd(r5, (int32_t)sgn24(n5), m5);
        int64_t bpre2 = b;                              // $3c5 move x:(r0),b
        b = compose24(R(r0));
        y0 = satA1(bpre2);                              //      b,y0 (latched)
        for (int j = 0; j < 16; ++j) {                  // $3c6 do #$10,$3cd
            a = mac_s(a, y1, x1);                       // $3c8 mac y1,x1,a
            y1 = RY(r5); r5 = upd(r5, (int32_t)sgn24(n5), m5);
            int64_t b3c9 = b;                           // $3c9 mac y1,y0,a b,x:(r0)+
            a = mac_s(a, y1, y0);
            W(r0, satA1(b3c9)); r0 = upd(r0, 1, m0);
            b = compose24(R(r0));                       // $3ca move x:(r0),b
            y1 = RY(r4); r4 = upd(r4, (int32_t)sgn24(n4), m4);
            int64_t a3cb = a;                           // $3cb mac y1,x1,b a,x:(r2)+ y:(r4)+n4,y1
            b = mac_s(b, y1, x1);
            W(r2, satA1(a3cb)); r2 = upd(r2, 1, m2);
            y1 = RY(r4); r4 = upd(r4, (int32_t)sgn24(n4), m4);
            b = mac_s(b, y1, y0);                       // $3cc mac y1,y0,b x:(r2),a y:(r5)-,y1
            a = compose24(R(r2));
            y1 = RY(r5); r5 = upd(r5, (int32_t)sgn24(n5), m5);
        }
        n0 = R(r1); r1 = upd(r1, 1, M24);               // $3cd move x:(r1)+,n0
        W(r0, satA1(b)); r0 = upd(r0, 1, m0);           // $3ce move b,x:(r0)+
    }
    B = b;                                              // (m0/m2 restore elided)
}

// ---------------------------------------------------------------------------
// PROC $14594A — one 16-sample frame.
// ---------------------------------------------------------------------------
void MnmSaw::processFrame(uint32_t* out32) {
    const uint32_t R6 = 0x428;
    int64_t a = 0, b = 0;
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
    uint32_t r0, r1, r2, r3, r4, r5, r7 = 0;
    uint32_t n0 = 0, n2 = 0, n3 = 0, n6 = 0;
    uint32_t m0 = M24, m1 = M24, m2 = M24, m3v = M24, m4 = M24, m5 = M24, m7 = M24;
    auto upd = [](uint32_t p, int32_t d, uint32_t m) -> uint32_t {
        if (m == M24) return (p + d) & M24;
        uint32_t base = p & ~m;
        int32_t off = (int32_t)(p - base) + d;
        if (off > (int32_t)m) off -= (int32_t)m + 1;
        if (off < 0) off += (int32_t)m + 1;
        return base + (uint32_t)off;
    };
    auto RYw = [&](uint32_t off) -> uint32_t { return Yw[off]; };   // y:(r6+off)
    auto WXw = [&](uint32_t off, uint32_t v) { Xw[off] = v & M24; };
    auto WYw = [&](uint32_t off, uint32_t v) { Yw[off] = v & M24; };
    // absolute X accessors (page window $400-$48F or kernel scratch/tables)
    auto XR = [&](uint32_t absa) -> uint32_t {
        if (absa >= R6 && absa < R6 + 0x90) return Xw[absa - R6];
        return rdXA(absa);
    };
    auto XSt = [&](uint32_t absa, uint32_t v) {
        if (absa >= R6 && absa < R6 + 0x90) { Xw[absa - R6] = v & M24; return; }
        wrX(absa, v);
    };

    // ---- block 1: input clamp + wavetable position ($14594A-$145974) ----
    x0 = 0x006000;                                        // move #>$6000,x0 (raw 24-bit)
    b = compose24(inX);                                   // move x:(r6-$27),b
    if (sext56(b) > ((int64_t)0x6000 << 24))              // cmp x0,b ; tfr x0,b ifgt
        b = compose24(0x006000);
    {                                                     // move a,l:?>$40 (A = pitch)
        int64_t A56 = compose24(pitchA1);
        Xa[0x40] = rawA1(A56); Ya[0x40] = rawA0(A56);
    }
    Ya[0x24] = satA1(b);                                  // move b,y:$24
    x1 = 0x000080;                                        // move #>$80,x1 (raw)
    r0 = 0x80;                                            // move #$80,r0
    a = compose24(Ya[0x24]);                              // move y:$24,a
    r4 = 0x22;                                            // lua (r6+$22),r4 (r6-rel)
    r5 = 0xC8;                                            // move #$c8,r5
    r2 = 0x1407FF;                                        // move #>$1407ff,r2
    Xa[0x80] = satA1(a); r0 = 0x81;                       // move a,x:(r0)+
    r3 = 0x140000;                                        // move #>$140000,r3
    r0 = 0x80; r1 = 0xC0;                                 // move #$80,r0 ; #$c0,r1
    y0 = 0x17A6;                                          // move #>$17a6,y0
    y1 = 0x56982B;                                        // move #>$56982b,y1
    a = compose24(Xa[0x80]); r0 = 0x81;                   // move x:(r0)+,a
    b = asr56(a, 11);                                     // asr #$b,a,b
    a = logicA1(a, 0x7FF);                                // and #>$7ff,a
    {                                                     // neg a  a1,n3
        int64_t apre = a;
        a = sext56(-sext56(a));
        n3 = rawA1(apre);                                 // latched PRE-neg
    }
    x0 = satA1(b);                                        // move b,x0
    {                                                     // move a1,n2 (post-neg)
        n2 = rawA1(a);
    }
    b = compose24(mw[(size_t)((0x140000 + (uint32_t)sgn24(n3)) - 0x140000) & 0x7FF]);
                                                          // move x:(r3+n3),b
    b = asr56(b, 12);                                     // asr #$c,b,b
    {                                                     // move x:(r2+n2),a
        uint32_t addr = (uint32_t)(0x1407FF + (uint32_t)sgn24(n2));
        // reads $1407FF - idx (idx = 0..$7FF): inside the machine-wave page
        uint32_t v = (addr >= 0x140000 && addr < 0x140800)
                     ? mw[addr - 0x140000] : 0;
        a = compose24(v);
    }
    a = asr56(a, (int)(x0 & 63));                         // asr x0,a,a
    b = asl56(b, (int)(x0 & 63));                         // asl x0,b,b
    x0 = satA1(a);                                        // move a,x0
    a = mac_s(0, y0, x0);                                 // $145970 mpy y0,x0,a
    x0 = satA1(b);                                        //      b,x0 (latched b)
    b = mac_s(0, x0, y1);                                 // $145971 mpy x0,y1,b
    {                                                     //      l:(r4),x — pair from r6+$22
        x1 = Xw[0x22]; x0 = Yw[0x22];
    }
    {                                                     // $145972 move a,l:(r1) (r1=$C0)
        Xa[0xC0] = rawA1(a); Ya[0xC0] = rawA0(a);
    }
    {                                                     // $145973 move b,l:(r4) (r6+$22)
        Xw[0x22] = rawA1(b); Yw[0x22] = rawA0(b);         // l: acc store = RAW a1/a0
    }
    {                                                     // $145974 move x,l:(r5)+ ($C8)
        Xa[0xC8] = x1; Ya[0xC8] = x0;
        r5 = 0xC9;
    }

    // ---- block 2: UNIW gains ($145975-$14597E) ----
    r7 = 0;                                               // move #$0,r7
    y0 = RYw(0x05);                                       // move y:(r6+$5),y0
    a = mpyri_s(0x80, y0);                                // mpyri #>$80,y0,a
    b = mpyri_s(0x40, y0);                                // mpyri #>$40,y0,b
    Ya[r7] = satA1(a); r7++;                              // move a,y:(r7)+
    Ya[r7] = satA1(b); r7++;                              // move b,y:(r7)+
    Ya[r7] = satA1(a); r7++;                              // move a,y:(r7)+
    Ya[r7] = satA1(b); r7++;                              // move b,y:(r7)+

    // ---- block 3: unison voice taps ($14597F-$145992) ----
    r7 = 0;                                               // move #$0,r7
    {                                                     // move l:(r4)+,x (r6+$22)
        x1 = Xw[0x22]; x0 = Yw[0x22]; r4 = 0x23;
    }
    r0 = Ya[r7]; r7 = 1;                                  // move y:(r7)+,r0
    for (int k = 0; k < 2; ++k) {                         // do #<$2,$145993
        y0 = rdXA((r0 + 0x101BFB) & M24);                  // move x:(r0+$101bfb),y0
        y1 = rdXA((r0 + 0x101CFB) & M24);                  // move x:(r0+$101cfb),y1
        a = mac_su(0, y0, x0);                            // mpysu y0,x0,a
        a = dmac_su(a, y0, x1);                           // dmac su y0,x1,a
        uint32_t r0next = Ya[r7]; r7++;                   //      y:(r7)+,r0
        a = asl56(a, 1);                                  // asl a
        b = mac_su(0, y1, x0);                            // mpysu y1,x0,b
        b = dmac_su(b, y1, x1);                           // dmac su y1,x1,b
        {                                                 //      l:(r4),y (pair r6+$23)
            y1 = Xw[0x23]; y0 = Yw[0x23];
        }
        b = asl56(b, 1);                                  // asl b
        {                                                 // move a,l:(r4)+ (r6+$23) raw
            Xw[0x23] = rawA1(a); Yw[0x23] = rawA0(a); r4 = 0x24;
        }
        {                                                 // move y,l:(r5)+ ($C8+k)
            Xa[r5 & 0xFF] = y1; Ya[r5 & 0xFF] = y0; r5++;
        }
        {                                                 // move l:(r4),y (r6+$24)
            y1 = Xw[0x24]; y0 = Yw[0x24];
        }
        {                                                 // move b,l:(r4)+ (r6+$24) raw
            Xw[0x24] = rawA1(b); Yw[0x24] = rawA0(b); r4 = 0x25;
        }
        {                                                 // move y,l:(r5)+
            Xa[r5 & 0xFF] = y1; Ya[r5 & 0xFF] = y0; r5++;
        }
        r0 = r0next;                                      // r0 for next iteration
    }

    // ---- block 4: second tap pair ($145993-$1459A1) ----
    {                                                     // move l:(r1)+,x (r1=$C0)
        x1 = Xa[0xC0]; x0 = Ya[0xC0]; r1 = 0xC1;
    }
    for (int k = 0; k < 2; ++k) {                     // do #<$2,$1459a2
            y0 = rdXA((r0 + 0x101B7B) & M24);              // move x:(r0+$101b7b),y0
            y1 = rdXA((r0 + 0x101C7B) & M24);              // move x:(r0+$101c7b),y1
            a = mac_su(0, y0, x0);                        // mpysu y0,x0,a
            a = dmac_su(a, y0, x1);                       // dmac su y0,x1,a
            a = asl56(a, 1);                              // asl a   y:(r7)+,r0
            r0 = Ya[r7]; r7++;                            //      y:(r7)+,r0 (r7 continues: 3,4)
            b = mac_su(0, y1, x0);                        // mpysu y1,x0,b
            b = dmac_su(b, y1, x1);                       // dmac su y1,x1,b
            b = asl56(b, 1);                              // asl b
            {                                             // move a,l:(r1)+  (raw)
                Xa[r1 & 0xFF] = rawA1(a); Ya[r1 & 0xFF] = rawA0(a); r1++;
            }
            {                                             // move b,l:(r1)+  (raw)
                Xa[r1 & 0xFF] = rawA1(b); Ya[r1 & 0xFF] = rawA0(b); r1++;
            }
        }

    // ---- block 5: unison spread normalize ($1459A2-$1459B9) ----
    r3 = 0xC8;                                            // move #$c8,r3
    r0 = 0xC0;                                            // move #$c0,r0
    r2 = 0x18;                                            // lua (r6+$18),r2 (r6-rel)
    x1 = Xw[0x22];                                        // move x:(r6+$22),x1
    a = compose24(Xa[0xC8]);                              // move x:(r3),a
    if (sext56(a) != (sgn24(x1) << 24)) {                 // cmp x1,a ; beq skip
        x1 = 0x18;                                        // move #>$18,x1 (= 24)
        {                                                 // move l:(r0)+,y
            y1 = Xa[0xC0]; y0 = Ya[0xC0]; r0 = 0xC1;
        }
        for (int k = 0; k < 5; ++k) {                     // do #<$5,$1459ba
            x0 = Xa[r3 & 0xFF]; r3++;                     // move x:(r3)+,x0
            a = mac_su(0, x0, y0);                        // mpysu x0,y0,a
            a = dmac_su(a, x0, y1);                       // dmac su x0,y1,a
            {                                             // clb a,b
                uint32_t c = clbA1(a);
                b = compose24(c);
            }
            a = normfB1(a, b);                            // normf b1,a
            {                                             // neg b  l:(r2),y
                int64_t bpre = b;
                b = sext56(-sext56(b));
                y1 = Xw[r2]; y0 = Yw[r2];
            }
            {                                             // sub x1,b  a,x0
                int64_t apre = a;
                b = sext56(b - (sgn24(x1) << 24));
                x0 = satA1(apre);
            }
            a = mac_su(0, x0, y0);                        // mpysu x0,y0,a
            a = dmac_su(a, x0, y1);                       // dmac su x0,y1,a
            a = normfB1(a, b);                            // normf b1,a
            {                                             // move l:(r0)+,y
                y1 = Xa[r0 & 0xFF]; y0 = Ya[r0 & 0xFF]; r0++;
            }
            {                                             // move a,l:(r2)+ (r6 page)
                Xw[r2] = satA1(a); Yw[r2] = rawA0(a); r2++;
            }
        }
    }

    // ---- block 6: knob->increment ($1459BA-$1459D3) ----
    x0 = 0x0A05BC;                                        // move #>$a05bc,x0
    auxXm1d = x0;                                         // move x0,x:(r6-$1d)
    y0 = RYw(0x04);                                       // move y:(r6+$4),y0 (UNIL)
    a = mpyi_s(0x1D41D, y0);                              // mpyi #>$1d41d,y0,a
    b = compose24(0x1D41D);                               // move #>$1d41d,b
    Ya[0x21] = satA1(b);                                  // move b,y:$21 (absolute)
    y0 = RYw(0x08);                                       // move y:(r6+$8),y0 (SUBX)
    b = maci_s(b, 0xEBE67, y0);                           // maci #>$ebe67,y0,b
    Ya[0x22] = satA1(a);                                  // move a,y:$22 (absolute)
    y0 = satA1(a);                                        // $1459c7 move a,y0
    y1 = RYw(0x06);                                       // move y:(r6+$6),y1 (UNIX)
    a = mac_s(0, y1, y0);                                 // mpy y1,y0,a
    Ya[0x20] = satA1(b);                                  // move b,y:$20 (absolute)
    Ya[0x23] = satA1(a);                                  // move a,y:$23 (absolute)
    y1 = RYw(0x09);                                       // move y:(r6+$9),y1 (SUB1)
    a = mpyi_s(0x80000, y1);                              // mpyi #>$80000,y1,a
    y1 = RYw(0x0A);                                       // move y:(r6+$a),y1 (SUB2)
    b = mpyi_s(0x80000, y1);                              // mpyi #>$80000,y1,b
    WYw(0x0E, satA1(a));                                  // move a,y:(r6+$e)
    WYw(0x11, satA1(b));                                  // move b,y:(r6+$11)

    // ---- block 7: helper call 1 (state r6+$18) ($1459D4-$1459F6) ----
    uint32_t rr1 = 0;
    {
        uint32_t rr1 = 0;                                     // move #$0,r1
        rr1 = 0;
        int64_t X48 = ((int64_t)Xa[0xC0] << 24) | Ya[0xC0];   // move l:?>$c0,x
        int64_t B48 = ((int64_t)Xw[0x18] << 24) | Yw[0x18];   // move x:(r6+$18),b ; y:(r6+$18),b0
        helper3E0(B48, (uint32_t)(X48 >> 24) & M24, X48 & M24, rr1);
        Xw[0x18] = rawA1(B48); Yw[0x18] = (uint32_t)B48 & M24;
    }
    r7 = 0x80; m7 = 0x1;                                  // move #$80,r7 ; #$1,m7
    x0 = 0x000040;                                        // move #>$40,x0 (raw)
    y0 = Ya[0x20];                                        // move y:$20,y0
    Ya[r7] = y0; r7 = (r7 + 1) & 0x1 | 0x80;              // move y0,y:(r7)+ (m7=1)
    y0 = Ya[0x21];                                        // move y:$21,y0
    Ya[r7] = y0;                                          // move y0,y:(r7) (no upd)
    a = compose24(Yw[0x27]);                              // move y:(r6+$27),a
    r0 = R6; r3 = R6;                                     // move r6,r0 ; move r6,r3
    n0 = rawA1(a);                                        // move a,n0
    a = sext56(a + ((int64_t)0x10 << 24));                // add #<$10,a
    {                                                     // cmp #>$98,a ; sub x0,a ifeq
        if (sext56(a) == (int64_t)0x98 << 24) a = sext56(a - (sgn24(x0) << 24));
    }
    WYw(0x27, satA1(a));                                  // move a,y:(r6+$27)
    n3 = rawA1(a);                                        // move a,n3
    r7 = Yw[0x2D];                                        // move y:(r6+$2d),r7
    r0 = upd(r0, (int32_t)sgn24(n0), M24);                // move (r0)+n0
    x1 = 0;                                               // move #$0,x1
    for (int i = 0; i < 16; ++i) {                        // rep #<$10
        XSt(r0 + i, 0);                                   // move x1,x:(r0)+
    }
    // (rep advances r0 by 16)
    r3 = upd(r3, (int32_t)sgn24(n3), M24);                // move (r3)+n3
    {
        int rr7 = (int)r7; uint32_t mm7 = m7;
        helper3AA(b, rr7, mm7, r3, r4, r5, n0, rr1);
        r7 = rr7; m7 = mm7;
    }
    Yw[0x2D] = (uint32_t)r7;                              // move r7,y:(r6+$2d)

    // ---- block 8: helper calls 2-4 (states r6+$19/$1a/$1b) ----
    {
        rr1 = 0;
        int64_t X48 = ((int64_t)Xa[0xC1] << 24) | Ya[0xC1];   // move l:?>$c1,x
        int64_t B48 = ((int64_t)Xw[0x19] << 24) | Yw[0x19];
        helper3E0(B48, (uint32_t)(X48 >> 24) & M24, X48 & M24, rr1);
        Xw[0x19] = rawA1(B48); Yw[0x19] = (uint32_t)B48 & M24;
        int rr7 = (int)r7; uint32_t mm7 = m7;
        helper3AA(b, rr7, mm7, r3, r4, r5, n0, rr1);
        r7 = rr7;
    }
    y0 = Ya[0x22];                                        // move y:$22,y0
    Ya[r7] = y0; r7 = (int)upd((uint32_t)r7, 1, m7);      // move y0,y:(r7)+
    Ya[r7] = y0; r7 = (int)upd((uint32_t)r7, 1, m7);      // move y0,y:(r7)+
    {
        int rr7 = (int)r7; uint32_t mm7 = m7;
        helper3AA(b, rr7, mm7, r3, r4, r5, n0, rr1);           // jsr func_0003aa
        r7 = rr7;
    }
    {
        rr1 = 0;
        int64_t X48 = ((int64_t)Xa[0xC2] << 24) | Ya[0xC2];   // move l:?>$c2,x
        int64_t B48 = ((int64_t)Xw[0x1A] << 24) | Yw[0x1A];
        helper3E0(B48, (uint32_t)(X48 >> 24) & M24, X48 & M24, rr1);
        Xw[0x1A] = rawA1(B48); Yw[0x1A] = (uint32_t)B48 & M24;
        int rr7 = (int)r7; uint32_t mm7 = m7;
        helper3AA(b, rr7, mm7, r3, r4, r5, n0, rr1);
        r7 = rr7;
    }
    {
        rr1 = 0;
        int64_t X48 = ((int64_t)Xa[0xC3] << 24) | Ya[0xC3];   // move l:?>$c3,x
        int64_t B48 = ((int64_t)Xw[0x1B] << 24) | Yw[0x1B];
        helper3E0(B48, (uint32_t)(X48 >> 24) & M24, X48 & M24, rr1);
        Xw[0x1B] = rawA1(B48); Yw[0x1B] = (uint32_t)B48 & M24;
        int rr7 = (int)r7; uint32_t mm7 = m7;
        helper3AA(b, rr7, mm7, r3, r4, r5, n0, rr1);
        r7 = rr7;
    }
    y0 = Ya[0x23];                                        // move y:$23,y0
    Ya[r7] = y0; r7 = (int)upd((uint32_t)r7, 1, m7);
    Ya[r7] = y0; r7 = (int)upd((uint32_t)r7, 1, m7);
    {
        int rr7 = (int)r7; uint32_t mm7 = m7;
        helper3AA(b, rr7, mm7, r3, r4, r5, n0, rr1);
        r7 = rr7;
    }
    m7 = M24;                                             // move #>$ffffff,m7

    // ---- block 9: morph index ($145A22-$145A36) ----
    x1 = Ya[0x24];                                        // move y:$24,x1
    a = mpyi_s(0x15555, x1);                              // mpyi #>$15555,x1,a
    a = sext56(a - ((int64_t)0x14 << 24));                // sub #<$14,a
    if (sext56(a) < 0) a = 0;                             // clr a ifmi
    {                                                     // move a1,r2 ; move a0,y0
        r2 = rawA1(a);
        y0 = rawA0(a);
    }
    x1 = 0x7EB852;                                        // move #>$7eb852,x1
    x0 = T_MORPH_A[r2 & 0x63F];                           // move x:(r2+$143d06),x0
    y1 = rdXA(0x143D06 + ((r2 + 1) & 0x7FF));             // move x:(r2+$143d07),y1
    a = mac_su(0, y1, y0);                                // mpysu y1,y0,a
    a = mac_su(a, neg24(x0), y0);                         // macsu -x0,y0,a
    a = asr56(a, 1);                                      // asr a
    a = sext56(a + (sgn24(x0) << 24));                    // add x0,a
    y1 = Yw[0x2E];                                        // move y:(r6+$2e),y1
    Yw[0x2E] = satA1(a);                                  // move a,y:(r6+$2e)

    // ---- block 10: 16-tap smoother ($145A37-$145A42) ----
    r0 = r3;                                              // move r3,r0
    r1 = 0;                                               // move #$0,r1
    m0 = 0xFFFFFF; m1 = 0xFFFFFF;                         // (linear)
    a = compose24(Xw[0x1F]);                              // move x:(r6+$1f),a
    x0 = XR(r0); r0++;                                    // move x:(r0)+,x0
    for (int k = 0; k < 16; ++k) {                        // do #<$10,$145a42
        int64_t astore = a;                               //      a,x:(r1)+ a,y0
        XSt(r1, satA1(astore)); r1++;                     //      x:(r1) absolute
        y0 = satA1(astore);
        a = mac_s(a, x1, x0);                             // mac x1,x0,a
        x0 = XR(r0); r0++;                                //      x:(r0)+,x0
        a = mac_s(a, neg24(y1), y0);                      // mac -y1,y0,a
    }
    WXw(0x1F, satA1(a));                                  // move a,x:(r6+$1f)

    // ---- block 11: morph frame index ($145A43-$145A64) ----
    x1 = Ya[0x24];                                        // move y:$24,x1
    a = mpyi_s(0xB3333, x1);                              // mpyi #>$b3333,x1,a
    a = sext56(a + ((int64_t)0xFFFF80 << 8));             // add #>$ffff80,a
    if (sext56(a) < 0) a = 0;                             // clr a ifmi
    x0 = 0x63F;                                           // move #>$63f,x0
    if (sext56(a) > (int64_t)0x63F) a = compose24(0x63F); // cmp x0,a ; tfr x0,a ifgt
    r2 = Xw[0x2E];                                        // move x:(r6+$2e),r2
    Xw[0x2E] = satA1(a);                                  // move a,x:(r6+$2e)
    a = compose24(0x7FFFFF);                              // move #>$7fffff,a
    x1 = rdXA(0x1435C6 + (r2 & 0x7FF));                   // move x:(r2+$1435c6),x1
    {                                                     // sub x1,a  a,x0
        int64_t apre = a;
        a = sext56(a - (sgn24(x1) << 24));
        x0 = satA1(apre);
    }
    a = sext56(a + (sgn24(x0) << 24));                    // add x0,a
    a = asr56(a, 1);                                      // asr a
    r0 = 0; r1 = 0;                                       // move #$0,r0 ; move r0,r1
    y1 = satA1(a);                                        // move a,y1
    a = compose24(Xw[0x1D]);                              // move x:(r6+$1d),a
    x0 = Yw[0x1E];                                        // move y:(r6+$1e),x0
    b = asl56(a, 3);                                      // asl #$3,a,b
    for (int k = 0; k < 10; ++k) {                        // do #<$10,$145a63
        int64_t astore = a;                               //      a,y0
        y0 = satA1(astore);
        a = mac_s(a, neg24(x0), y1);                      // mac -x0,y1,a
        x0 = XR(r0); r0++;                                //      x:(r0)+,x0
        a = mac_s(a, x0, y1);                             // mac x0,y1,a
        int64_t bstore = b;                               //      b,x:(r1)+
        XSt(r1, satA1(bstore)); r1++;
        a = mac_s(a, neg24(x1), y0);                      // mac -x1,y0,a
        b = asl56(a, 3);                                  // asl #$3,a,b
    }
    Yw[0x1E] = satA1(x0);                                 // move x0,y:(r6+$1e)
    Xw[0x1D] = satA1(a);                                  // move a,x:(r6+$1d)

    // ---- block 12: pitch -> 48-bit increment ($145A65-$145A6C) ----
    x0 = 0x17C6F9;                                        // move #>$17c6f9,x0
    {                                                     // move l:?>$40,y
        y1 = Xa[0x40]; y0 = Ya[0x40];
    }
    b = mac_su(0, x0, y0);                                // mpysu x0,y0,b
    b = dmac_ss(b, x0, y1);                               // dmac ss x0,y1,b
    r5 = 0x0C;                                            // lua (r6+$c),r5 (r6-rel)
    {                                                     // move b,l:?>$40
        Xa[0x40] = rawA1(b); Ya[0x40] = rawA0(b);
    }

    // ---- block 13: main oscillator, 2 voices ($145A6E-$145AA7) ----
    // r5 = r6+$C ABSOLUTE ($434); per voice: phase pair L:(r5), interp
    // result L:(r5+1), level y1 = Y:(r5+2). Workspace L:$10-$1F ABSOLUTE
    // (16 words); r2 = X:$10 (table base+carry), r0 = X:$11, r4 = $10.
    r5 = 0x434;                                           // lua (r6+$c),r5
    {                                                     // move b,l:?>$40
        Xa[0x40] = rawA1(b); Ya[0x40] = rawA0(b);
    }
    for (int v = 0; v < 2; ++v) {                         // do #<$2,$145aa9
        a = ((int64_t)Xa[0x40] << 24) | Ya[0x40];         // move l:?>$40,a
        a = asr56(a, 1);                                  // asr a
        {                                                 // move l:(r5),b
            b = ((int64_t)XR(r5) << 24) | rdY(r5);
        }
        {                                                 // move a,l:?>$40
            Xa[0x40] = rawA1(a); Ya[0x40] = rawA0(a);
        }
        {                                                 // move a,l:(r5)+
            XSt(r5, rawA1(a)); wrY(r5, rawA0(a)); r5++;
        }
        a = sext56(a - sext56(b));                        // sub b,a
        a = asr56(a, 4);                                  // asr #$4,a,a
        r1 = 0x10;                                        // move #$10,r1
        y1 = rawA1(a);                                    // move a1,y1
        y0 = rawA0(a);                                    // move a0,y0
        {                                                 // tfr b,a
            a = b;
        }
        {                                                 // move l:(r5),b
            b = ((int64_t)XR(r5) << 24) | rdY(r5);
        }
        x0 = 0x1FFF;                                      // move #>$1fff,x0
        x1 = 0x14A000;                                    // move #>$14a000,x1
        {
            int64_t Y48 = ((int64_t)y1 << 24) | y0;
            for (int k = 0; k < 16; ++k) {                // do #<$10,$145a88
                a = sext56(a + Y48);                      // add y,a
                int64_t bstore = b;                       //      b,l:(r1)+ latch
                b = sext56(b + sext56(a));                // add a,b
                {                                         //      store latch pair
                    XSt(r1, rawA1(bstore)); wrY(r1, rawA0(bstore)); r1++;
                }
                b = (b & ~((int64_t)M24 << 24) & ~(0xFFll << 48))
                    | ((int64_t)(((b >> 24) & M24) & x0) << 24);   // and x0,b
                b = sext56(b + (sgn24(x1) << 24));        // add x1,b
            }
        }
        r1 = 0x10;                                        // move #$10,r1
        r4 = r1;                                          // move r1,r4
        {                                                 // move b1,x:(r5) ; b0,y:(r5)+
            XSt(r5, rawA1(b)); wrY(r5, rawA0(b)); r5++;
        }
        r2 = XR(r1); r1++;                                // move x:(r1)+,r2
        m2 = 0x1FFF; m0 = 0x1FFF;                         // move #>$1fff,m2 ; m2,m0
        r3 = 0;                                           // move #$0,r3
        y1 = rdY(r5); r5++;                               // move y:(r5)+,y1
        y0 = rdY(r4); r4 = upd(r4, 1, m4);                // move y:(r4)+,y0
        x1 = rdXA(upd(r2, 1, m2));                        // move x:(r2)+,x1 (post-inc)
        r0 = XR(r1); r1++;                                // move x:(r1)+,r0
        for (int k = 0; k < 8; ++k) {                     // do #<$8,$145aa5
            // $145a97 mpysu -x1,y0,a   x:(r2),x0
            int64_t astore = mac_su(0, neg24(x1), y0);
            x0 = rdXA(r2);
            // $145a98 add x1,a   x:(r2),x0
            astore = sext56(astore + (sgn24(x1) << 24));
            x0 = rdXA(r2);
            // $145a99 add x1,a   x:(r1)+,r2
            astore = sext56(astore + (sgn24(x1) << 24));
            r2 = XR(r1); r1++;
            // $145a9a macsu x0,y0,a
            astore = mac_su(astore, x0, y0);
            // $145a9b asr a   x:(r0)+,x1   y:(r4)+,y0
            astore = asr56(astore, 1);
            x1 = rdXA(upd(r0, 1, m0));
            y0 = rdY(upd(r4, 1, m4));
            a = astore;
            // $145a9c mpysu -x1,y0,b
            int64_t bstore = mac_su(0, neg24(x1), y0);
            // $145a9d add x1,b   x:(r0),x0
            bstore = sext56(bstore + (sgn24(x1) << 24));
            x0 = rdXA(r0);
            // $145a9e add x1,b   x:(r1)+,r0
            bstore = sext56(bstore + (sgn24(x1) << 24));
            r0 = XR(r1); r1++;
            // $145a9f macsu x0,y0,b
            bstore = mac_su(bstore, x0, y0);
            // $145aa0 asr b   x:(r3)+,a   a,y0
            bstore = asr56(bstore, 1);
            int64_t a3 = a;
            a = compose24(XR(r3)); r3 = upd(r3, 1, m3v);
            y0 = satA1(a3);
            // $145aa1 mac y1,y0,a   x:(r3)-,b   b,y0
            int64_t b3 = b;
            b = compose24(XR(r3)); r3 = upd(r3, -1, m3v);
            y0 = satA1(b3);
            a = mac_s(a, y1, y0);
            // $145aa2 mac x0,y1,b   x:(r2)+,x1   y:(r4)+,y0
            b = mac_s(b, x0, y1);
            x1 = rdXA(upd(r2, 1, m2));
            y0 = rdY(upd(r4, 1, m4));
            // $145aa3 move a,x:(r3)+
            XSt(r3, satA1(a)); r3 = upd(r3, 1, m3v);
            // $145aa4 move b,x:(r3)+
            XSt(r3, satA1(b)); r3 = upd(r3, 1, m3v);
        }
        m0 = M24; m2 = M24;                               // $145aa5/$aa7
    }

    // ---- block 14: output ($145AA9-$145AB4) ----
    r7 = 0x100;                                           // move #>$100,r7
    r0 = 0;                                               // move #$0,r0
    a = compose24(Xa[r0]); r0++;                          // move x:(r0)+,a
    b = compose24(Xa[r0]); r0++;                          // move x:(r0)+,b
    for (int k = 0; k < 8; ++k) {                         // do #<$8,$145ab4
        out32[4 * k + 0] = satA1(a);                      // move a,y:(r7)+
        int64_t astore = a;                               //      x:(r0)+,a  a,y:(r7)+
        a = compose24(Xa[r0]); r0++;
        out32[4 * k + 1] = satA1(astore);
        int64_t bstore = b;                               //      b,y:(r7)+  x:(r0)+,b
        b = compose24(Xa[r0]); r0++;
        out32[4 * k + 2] = satA1(bstore);
        out32[4 * k + 3] = satA1(b);
    }
}

} // namespace saw
} // namespace mnm
