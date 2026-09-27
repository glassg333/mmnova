; ITERATION 28 — kernel echo/delay routing: annotated listings
; source: decoder_v2 code_full.asm (dsp1_pmem.bin), P-space addresses
; r6 = page_base + $28 (page = Y:$500/$600/$700; Y:$123 = current r6)
; y:(r6-0x28+k) = page word k ; x:(r6-0x28+k) = X-shadow cell k


; ==========================================================================
; MAIN LOOP: codec frame sync, banks, machine dispatch, per-voice flow
; ==========================================================================
000087: movep    x:<<$ffffeb,a                               ; 084E2B
000088: cmp      #>$180,a                                    ; 0140C5 000180
00008A: beq      int_000092                                  ; 05A408
00008B: cmp      #>$160,a                                    ; 0140C5 000160
00008D: beq      int_00009f                                  ; 05A412
00008E: cmp      #>$140,a                                    ; 0140C5 000140
000090: beq      int_0000ac                                  ; 05A41C
000091: bra      int_000087                                  ; 050FD6
000092: move     #>$140,r1                                   ; 61F400 000140
000094: move     #>$1a0,r2                                   ; 62F400 0001A0
000096: move     #>$200,r3                                   ; 63F400 000200
000098: move     #>$260,r0                                   ; 60F400 000260
00009A: move     #>$2cc,x0                                   ; 44F400 0002CC
00009C: move     #>$32c,x1                                   ; 45F400 00032C
00009E: bra      int_0000b8                                  ; 050C1A
00009F: move     #>$180,r1                                   ; 61F400 000180
0000A1: move     #>$1e0,r2                                   ; 62F400 0001E0
0000A3: move     #>$240,r3                                   ; 63F400 000240
0000A5: move     #>$2a0,r0                                   ; 60F400 0002A0
0000A7: move     #>$2fc,x0                                   ; 44F400 0002FC
0000A9: move     #>$2cc,x1                                   ; 45F400 0002CC
0000AB: bra      int_0000b8                                  ; 050C0D
0000AC: move     #>$160,r1                                   ; 61F400 000160
0000AE: move     #>$1c0,r2                                   ; 62F400 0001C0
0000B0: move     #>$220,r3                                   ; 63F400 000220
0000B2: move     #>$280,r0                                   ; 60F400 000280
0000B4: move     #>$32c,x0                                   ; 44F400 00032C
0000B6: move     #>$2fc,x1                                   ; 45F400 0002FC
0000B8: move     a,x:>$2c8                                   ; 567000 0002C8
0000BA: move     r1,x:>$2c0                                  ; 617000 0002C0
0000BC: move     r2,x:>$2c1                                  ; 627000 0002C1
0000BE: move     r3,x:>$2c2                                  ; 637000 0002C2
0000C0: move     r0,x:>$2c3                                  ; 607000 0002C3
0000C2: move     x0,x:>$2c9                                  ; 447000 0002C9
0000C4: move     x0,x:>$2cb                                  ; 447000 0002CB
0000C6: move     x1,x:>$2ca                                  ; 457000 0002CA
0000C8: move     #>$528,x0                                   ; 44F400 000528
0000CA: move     x0,y:>$123                                  ; 4C7000 000123
0000CC: move     #$0,x0                                      ; 240000
0000CD: move     x0,y:>$124                                  ; 4C7000 000124
0000CF: move     y:>$123,r6                                  ; 6EF000 000123
0000D1: move     #>$ffffff,m0                                ; 05F420 FFFFFF
0000D3: move     m0,m1                                       ; 0461A0
0000D4: move     m0,m2                                       ; 0462A0
0000D5: move     m0,m3                                       ; 0463A0
0000D6: move     m0,m6                                       ; 0466A0
0000D7: move     m0,m5                                       ; 0465A0
0000D8: move     m0,m4                                       ; 0464A0
0000D9: move     m0,m7                                       ; 0467A0
0000DA: move     #$0,x0                                      ; 240000
0000DB: move     x0,x:(r6-$1c)                               ; 039684
0000DC: move     y:(r6-$3),x0                                ; 03F6F4
0000DD: move     x0,x:>$2c4                                  ; 447000 0002C4
0000DF: move     y:>$124,r1                                  ; 69F000 000124
0000E1: move     y:(r6),a                                    ; 5EE600
0000E2: bclr     #$7,a1                                      ; 0ACC47
0000E3: move     a,y:(r6)                                    ; 5E6600
0000E4: bcc      func_000100                                 ; 05041C
0000E5: move     y:(r6-$4),x0                                ; 03F6B4
0000E6: move     x0,r0                                       ; 209000
0000E7: move     y:(r1+$120),a                               ; 0B71CE 000120
0000E9: cmp      x0,a                                        ; 200045
0000EA: beq      int_0000f7                                  ; 05A40D
0000EB: move     x:(r0+$10016b),r2                           ; 0A70D2 10016B
0000ED: move     x0,y:(r1+$120)                              ; 0B7184 000120
0000EF: move     y:(r6+$1),b                                 ; 0206FF
0000F0: move     #$20,x0                                     ; 242000
0000F1: move     x0,x:(r6-$1d)                               ; 038EC4
0000F2: move     #>$1,x0                                     ; 44F400 000001
0000F4: move     x0,x:(r6-$19)                               ; 039EC4
0000F5: move     b,x:(r6-$20)                                ; 03868F
0000F6: jsr      (r2)                                        ; 0BE280
0000F7: move     y:>$124,r1                                  ; 69F000 000124
0000F9: move     y:(r1+$120),r0                              ; 0B71D0 000120
0000FB: move     x:(r0+$10018d),r1                           ; 0A70D1 10018D
0000FD: move     y:>$123,r6                                  ; 6EF000 000123
0000FF: jsr      (r1)                                        ; 0BE180
000100: move     y:>$124,r1                                  ; 69F000 000124
000102: move     #>$100,r7                                   ; 67F400 000100
000104: move     y:(r1+$120),r0                              ; 0B71D0 000120
000106: move     x:(r0+$1001af),r1                           ; 0A70D1 1001AF
000108: move     y:>$123,r6                                  ; 6EF000 000123
00010A: move     #>$ffffff,m0                                ; 05F420 FFFFFF
00010C: move     m0,m1                                       ; 0461A0
00010D: move     m0,m2                                       ; 0462A0
00010E: move     m0,m3                                       ; 0463A0
00010F: move     m0,m4                                       ; 0464A0
000110: move     m0,m5                                       ; 0465A0
000111: move     m0,m6                                       ; 0466A0
000112: move     m0,m7                                       ; 0467A0
000113: move     x:>$2c4,x1                                  ; 45F000 0002C4
000115: move     x:>$2c3,r2                                  ; 62F000 0002C3
000117: brset    #$6,x1,func_0002ed                          ; 0CC5A6 0001D6
000119: brset    #$7,x1,func_000302                          ; 0CC5A7 0001E9
00011B: move     y:(r6+$b),b                                 ; 022EFF
00011C: move     #$40,y0                                     ; 264000
00011D: move     x:>$2c4,a                                   ; 56F000 0002C4
00011F: brclr    #$4,a,func_000125                           ; 0CCE84 000006
000121: move     x:>$2c3,r0                                  ; 60F000 0002C3
000123: tfr      y0,b                                        ; 200059
000124: jmp      func_000262                                 ; 0C0262
000125: btst     #$3,a                                       ; 0BCE63
000126: tfr      y0,b ifcs                                   ; 202859
000127: move     x:>$2c0,r0                                  ; 60F000 0002C0
000129: brset    #$c,a,func_000262                           ; 0CCEAC 000139
00012B: move     x:>$2c1,r0                                  ; 60F000 0002C1
00012D: brset    #$d,a,func_000262                           ; 0CCEAD 000135
00012F: move     x:>$2c2,r0                                  ; 60F000 0002C2
000131: jmp      func_000262                                 ; 0C0262
000132: move     #>$ffffff,m0                                ; 05F420 FFFFFF
000134: move     m0,m6                                       ; 0466A0
000135: move     m0,m1                                       ; 0461A0

; ==========================================================================
; bank copies / echo bus readback into X-banks
; ==========================================================================
0001AD: move     x:>$2c0,r1                                  ; 61F000 0002C0
0001AF: move     x:>$2c1,r2                                  ; 62F000 0002C1
0001B1: move     x:>$2c2,r3                                  ; 63F000 0002C2
0001B3: move     r1,r0                                       ; 223000
0001B4: move     #>$7fffff,y0                                ; 46F400 7FFFFF
0001B6: move     x:(r1)+,x0                                  ; 44D900
0001B7: do       #<$10,>$1c1                                 ; 061080 0001C0
0001B9: mpy      y0,x0,a         x:(r1)+,x0                  ; 44D9D0
0001BA: mpy      y0,x0,b x:(r2)+,x0                          ; 44DAD8
0001BB: mac      y0,x0,a x:(r2)+,x0                          ; 44DAD2
0001BC: mac      y0,x0,b x:(r3)+,x0                          ; 44DBDA
0001BD: mac      y0,x0,a x:(r3)+,x0                          ; 44DBD2
0001BE: mac      y0,x0,b         x:(r1)+,x0                  ; 44D9DA
0001BF: move     a,x:(r0)+                                   ; 565800
0001C0: move     b,x:(r0)+                                   ; 575800
0001C1: jmp      int_000087                                  ; 0C0087
0001C2: move     x:>$2ca,r0                                  ; 60F000 0002CA
0001C4: move     #$10,n0                                     ; 381000
0001C5: move     x:>$2c0,r2                                  ; 62F000 0002C0
0001C7: lua      (r0)+n0,r1                                  ; 044811
0001C8: do       #<$10,>$1ce                                 ; 061080 0001CD
0001CA: move     x:(r0)+,x0                                  ; 44D800
0001CB: move     x:(r1)+,x1                                  ; 45D900
0001CC: move     x0,x:(r2)+                                  ; 445A00
0001CD: move     x1,x:(r2)+                                  ; 455A00
0001CE: move     x:>$2c1,r2                                  ; 62F000 0002C1
0001D0: move     x:>$2cb,r0                                  ; 60F000 0002CB
0001D2: do       #<$10,>$1d8                                 ; 061080 0001D7
0001D4: move     x:(r1)+,x0                                  ; 44D900
0001D5: move     x:(r0)+,x1                                  ; 45D800
0001D6: move     x0,x:(r2)+                                  ; 445A00
0001D7: move     x1,x:(r2)+                                  ; 455A00
0001D8: move     x:>$2c2,r2                                  ; 62F000 0002C2
0001DA: lua      (r0)+n0,r1                                  ; 044811
0001DB: do       #<$10,>$1e1                                 ; 061080 0001E0
0001DD: move     x:(r0)+,x0                                  ; 44D800
0001DE: move     x:(r1)+,x1                                  ; 45D900
0001DF: move     x0,x:(r2)+                                  ; 445A00
0001E0: move     x1,x:(r2)+                                  ; 455A00
0001E1: jmp      int_000087                                  ; 0C0087
0001E2: move     r2,r3                                       ; 225300
0001E3: move     x:(r2)+,b y:(r4)+,y0                        ; FC9A00
0001E4: add      y0,b                                        ; 200058
0001E5: do       #<$10,>$1eb                                 ; 061080 0001EA
0001E7: move     x:(r2)+,a y:(r4)+,y0                        ; F89A00
0001E8: add      y0,a b,x:(r3)+                              ; 575B50
0001E9: move     x:(r2)+,b y:(r4)+,y0                        ; FC9A00
0001EA: add      y0,b a,x:(r3)+                              ; 565B58

; ==========================================================================
; voice bank select via X:$2C4 bits 0/1/2
; ==========================================================================
0001F3: brclr    #$0,a,func_000200                           ; 0CCE80 00000D
0001F5: move     #$0,r4                                      ; 340000
0001F6: move     x:>$2c0,r2                                  ; 62F000 0002C0
0001F8: bsclr    #$c,a,func_0001e2                           ; 0DCE8C FFFFEA
0001FA: move     x:>$2c4,a                                   ; 56F000 0002C4
0001FC: bsset    #$c,a,func_0001ec                           ; 0DCEAC FFFFF0
0001FE: move     x:>$2c4,a                                   ; 56F000 0002C4
000200: brclr    #$1,a,func_00020d                           ; 0CCE81 00000D
000202: move     #$0,r4                                      ; 340000
000203: move     x:>$2c1,r2                                  ; 62F000 0002C1
000205: bsclr    #$d,a,func_0001e2                           ; 0DCE8D FFFFDD
000207: move     x:>$2c4,a                                   ; 56F000 0002C4
000209: bsset    #$d,a,func_0001ec                           ; 0DCEAD FFFFE3
00020B: move     x:>$2c4,a                                   ; 56F000 0002C4
00020D: brclr    #$2,a,func_000218                           ; 0CCE82 00000B
00020F: move     #$0,r4                                      ; 340000
000210: move     x:>$2c2,r2                                  ; 62F000 0002C2
000212: bsclr    #$e,a,func_0001e2                           ; 0DCE8E FFFFD0
000214: move     x:>$2c4,a                                   ; 56F000 0002C4
000216: bsset    #$e,a,func_0001ec                           ; 0DCEAE FFFFD6
000218: bra      func_000163                                 ; 050E8B

; ==========================================================================
; DMA4 re-arm: host block 47 words -> X:$2FC
; ==========================================================================
000259: movep    #>$4a5a20,x:<<$ffffdc                       ; 08F49C 4A5A20
00025B: movep    x:>$2ca,x:<<$ffffde                         ; 08F09E 0002CA
00025D: movep    #>$2f,x:<<$ffffdd                           ; 08F49D 00002F
00025F: movep    #>$8a52c0,x:<<$ffffdc                       ; 08F49C 8A52C0
000261: rti                                                  ; 000004

; ==========================================================================
; GLIDE ENGINE: DSND/DBAS/P$2A -> X:$501 float -> PROC
; ==========================================================================
000262: move     b,x:$1                                      ; 570100
000263: move     y:(r6+$1),b                                 ; 0206FF
000264: move     x:(r6-$21),a                                ; 037EDE
000265: cmp      a,b                                         ; 20000D
000266: beq      func_000270                                 ; 05A40A
000267: move     x:(r6-$20),x0                               ; 038694
000268: cmp      x0,b                                        ; 20004D
000269: blt      func_00026d                                 ; 059404
00026A: cmp      x0,a                                        ; 200045
00026B: bge      func_000270                                 ; 051405
00026C: move     a,x:(r6-$22)                                ; 037E8E
00026D: cmp      x0,a                                        ; 200045
00026E: blt      func_000270                                 ; 059402
00026F: move     a,x:(r6-$22)                                ; 037E8E
000270: move     b,x:(r6-$21)                                ; 037ECF
000271: move     y:(r6-$6),b                                 ; 03EEBF
000272: move     x:(r6-$26),a                                ; 036E9E
000273: cmp      a,b                                         ; 20000D
000274: beq      func_00027e                                 ; 05A40A
000275: move     x:(r6-$24),x0                               ; 037694
000276: cmp      x0,b                                        ; 20004D
000277: blt      func_00027b                                 ; 059404
000278: cmp      x0,a                                        ; 200045
000279: bge      func_00027e                                 ; 051405
00027A: move     a,x:(r6-$25)                                ; 036ECE
00027B: cmp      x0,a                                        ; 200045
00027C: blt      func_00027e                                 ; 059402
00027D: move     a,x:(r6-$25)                                ; 036ECE
00027E: move     b,x:(r6-$26)                                ; 036E8F
00027F: move     #>$1449c6,r4                                ; 64F400 1449C6
000281: move     r4,r3                                       ; 229300
000282: move     #>$7f,y0                                    ; 46F400 00007F
000284: move     y:(r6-$c),b                                 ; 03D6BF
000285: move     y:(r6-$21),a                                ; 037EFE
000286: asr      #$10,a,a                                    ; 0C1C20
000287: asr      #$10,b,b                                    ; 0C1CA1
000288: move     a,n4                                        ; 21DC00
000289: move     b,n3                                        ; 21FB00
00028A: move     y:(r6+$2),y1                                ; 020EB7
00028B: move     y:(r4+n4),x0                                ; 4CEC00
00028C: move     y:(r3+n3),x1                                ; 4DEB00
00028D: mpy      x0,y1,a                                     ; 2000C0
00028E: tfr      y0,a ifmi                                   ; 202B51
00028F: mpy      y1,x1,b                                     ; 2000F8
000290: tfr      y0,b ifmi                                   ; 202B59
000291: asr      #$8,a,a                                     ; 0C1C10
000292: asr      #$8,b,b                                     ; 0C1C91
000293: move     a0,y0                                       ; 210600
000294: move     b0,y1                                       ; 212700
000295: move     x:(r6-$26),b                                ; 036E9F
000296: move     x:(r6-$25),a                                ; 036EDE
000297: sub      a,b                                         ; 20001C
000298: move     x:(r6-$24),a                                ; 03769E
000299: move     x:(r6-$23),a0                               ; 0376D8
00029A: move     b,x0                                        ; 21E400
00029B: mac      x0,y1,a                                     ; 2000C2
00029C: tst      b                                           ; 20000B
00029D: bmi      func_0002a4                                 ; 05B407
00029E: move     y:(r6-$6),b                                 ; 03EEBF
00029F: cmp      b,a                                         ; 200005
0002A0: ble      func_0002a9                                 ; 05F409
0002A1: move     b,x:(r6-$25)                                ; 036ECF
0002A2: move     b,a                                         ; 21EE00
0002A3: bra      func_0002a9                                 ; 050C06
0002A4: move     y:(r6-$6),b                                 ; 03EEBF
0002A5: cmp      b,a                                         ; 200005
0002A6: bge      func_0002a9                                 ; 051403
0002A7: move     b,x:(r6-$25)                                ; 036ECF
0002A8: move     b,a                                         ; 21EE00
0002A9: move     x:(r6-$21),b                                ; 037EDF
0002AA: move     a,l:<$0                                     ; 480000
0002AB: move     a1,x:(r6-$24)                               ; 03768C
0002AC: move     a0,x:(r6-$23)                               ; 0376C8
0002AD: move     x:(r6-$22),a                                ; 037E9E
0002AE: sub      a,b                                         ; 20001C
0002AF: move     x:(r6-$20),a                                ; 03869E
0002B0: move     x:(r6-$1f),a0                               ; 0386D8
0002B1: move     b,x0                                        ; 21E400
0002B2: mac      y0,x0,a                                     ; 2000D2
0002B3: tst      b                                           ; 20000B
0002B4: bmi      func_0002bb                                 ; 05B407
0002B5: move     y:(r6+$1),b                                 ; 0206FF
0002B6: cmp      b,a                                         ; 200005
0002B7: ble      func_0002c0                                 ; 05F409
0002B8: move     b,x:(r6-$22)                                ; 037E8F
0002B9: move     b,a                                         ; 21EE00
0002BA: bra      func_0002c0                                 ; 050C06
0002BB: move     y:(r6+$1),b                                 ; 0206FF
0002BC: cmp      b,a                                         ; 200005
0002BD: bge      func_0002c0                                 ; 051403
0002BE: move     b,x:(r6-$22)                                ; 037E8F
0002BF: move     b,a                                         ; 21EE00
0002C0: move     x:$1,x0                                     ; 448100
0002C1: move     #>$2ab,x1                                   ; 45F400 0002AB
0002C3: mpy      x1,x0,b                                     ; 2000A8
0002C4: move     a1,x:(r6-$20)                               ; 03868C
0002C5: move     a0,x:(r6-$1f)                               ; 0386C8
0002C6: move     y:(r6-$2),x0                                ; 03FEB4
0002C7: add      x0,a                                        ; 200040
0002C8: move     y:(r6-$a),x0                                ; 03DEB4
0002C9: add      b,a                                         ; 200010
0002CA: maci     #>$b000,x0,a                                ; 0141C2 00B000
0002CC: sub      #>$5800,a                                   ; 0140C4 005800
0002CE: move     l:<$0,b                                     ; 498000
0002CF: add      b,a                                         ; 200010
0002D0: clr      a               ifmi                        ; 202B13
0002D1: move     #>$5800,x0                                  ; 44F400 005800
0002D3: cmp      x0,a                                        ; 200045
0002D4: tfr      x0,a ifge                                   ; 202141
0002D5: move     y:>$124,b                                   ; 5FF000 000124
0002D7: move     a,x:(r6-$27)                                ; 0366CE
0002D8: cmp      #>$2,b                                      ; 0140CD 000002
0002DA: beq      func_0002dd                                 ; 05A403
0002DB: move     a,x:(r6+$ff)                                ; 0A768E 0000FF
0002DD: asr      #$b,a,b                                     ; 0C1C17
0002DE: and      #>$7ff,a                                    ; 0140C6 0007FF
0002E0: move     a,r2                                        ; 21D200
0002E1: move     b,x0                                        ; 21E400
0002E2: move     x:(r2+$140000),a                            ; 0A72CE 140000
0002E4: asr      #$a,a,a                                     ; 0C1C14
0002E5: asl      x0,a,a                                      ; 0C1E48
0002E6: move     #>$1d22a,y0                                 ; 46F400 01D22A
0002E8: move     a,x0                                        ; 21C400
0002E9: mpy      y0,x0,a                                     ; 2000D0
0002EA: asr      #$3,a,a                                     ; 0C1C06
0002EB: jsr      (r1)                                        ; 0BE180

; ==========================================================================
; mod bank X:$00-$1F fill (stage-2 tail, tables 1448C6/144946)
; ==========================================================================
0008E7: move     x1,y:$0                                     ; 4D0000
0008E8: move     a,x0                                        ; 21C400
0008E9: move     n0,n1                                       ; 231900
0008EA: move     #$6d,r1                                     ; 316D00
0008EB: move     x:(r2+$14a801),y1                           ; 0A72C7 14A801
0008ED: move     y1,y:$1                                     ; 4F0100
0008EE: mpy      x1,x0,b #$20,r5                             ; 3520A8
0008EF: mpy      x0,y1,a l:(r7),y                            ; 43E7C0
0008F0: move     ab,l:(r7)+                                  ; 4A5F00
0008F1: sub      y0,b #$ad,r0                                ; 30AD5C
0008F2: sub      y1,a r5,r4                                  ; 22B474
0008F3: tfr      y0,a a,x1                                   ; 21C551
0008F4: tfr      y1,b b,x0                                   ; 21E479
0008F5: move     #$8,y0                                      ; 260800
0008F6: do       #<$10,>$8fa                                 ; 061080 0008F9
0008F8: mac      y0,x0,a a,y:(r5)+                           ; 5E5DD2
0008F9: mac      x1,y0,b b,y:(r5)+                           ; 5F5DEA
0008FA: move     y:(r6+$12),x0                               ; 024EB4
0008FB: mpyri    #>$80,x0,a                                  ; 0141C1 000080
0008FD: move     x:(r6+$f),x0                                ; 023ED4
0008FE: jclr     #$0,x0,func_000901                          ; 0AC400 000901
000900: move     r1,r0                                       ; 223000
000901: move     #$0,r2                                      ; 320000
000902: move     a1,r5                                       ; 219500
000903: move     l:(r7),ab                                   ; 4AE700
000904: do       #<$10,>$910                                 ; 061080 00090F
000906: move     a,x:(r2)+ y:(r4)+,y1                        ; F91A00
000907: move     b,x:(r3)+ y:(r4)+,y0                        ; FC1B00
000908: move     l:(r1)+,x                                   ; 42D900
000909: mpysu    y0,x0,a                                     ; 012785
00090A: dmac     ss y0,x1,a                                  ; 01248E
00090B: move     l:(r0)+,x                                   ; 42D800
00090C: mpysu    y1,x0,b                                     ; 0127AC
00090D: dmac     ss y1,x1,b                                  ; 0124A7
00090E: asl      #$4,a,a                                     ; 0C1D08
00090F: asl      #$4,b,b                                     ; 0C1D89
000910: move     a,x:(r2)+                                   ; 565A00
000911: move     ab,l:(r7)+                                  ; 4A5F00
000912: move     y:(r5+$1448c6),y0                           ; 0B75C6 1448C6
000914: move     y:(r5+$144946),y1                           ; 0B75C7 144946
000916: tfr      y0,b b,x:(r3)+                              ; 575B59

; ==========================================================================
; ring write (X:$C5) + MASTER MIX -> echo bus X:$2C9
; ==========================================================================
000B15: move     x:>$c5,r4                                   ; 64F000 0000C5
000B17: move     m2,m4                                       ; 0464A2
000B18: move     x:(r0)+,a                                   ; 56D800
000B19: move     x:(r0)+,b                                   ; 57D800
000B1A: do       #<$8,>$b1e                                  ; 060880 000B1D
000B1C: move     x:(r0)+,a       a,y:(r4)+                   ; BA9800
000B1D: move     x:(r0)+,b       b,y:(r4)+                   ; BF9800
000B1E: move     y:(r6+$1f),x0                               ; 027EF4
000B1F: mpy      x0,x0,a                                     ; 200080
000B20: move     y:>$4ff,y0                                  ; 4EF000 0004FF
000B22: mpy      y0,y0,b a,x0                                ; 21C498
000B23: move     y:(r7-$5d),y1                               ; 0B77C7 FFFFA3
000B25: move     b,x1                                        ; 21E500
000B26: mpy      x1,x0,a                                     ; 2000A0
000B27: move     #$10,r5                                     ; 351000
000B28: move     a,y:(r7-$5d)                                ; 0B778E FFFFA3
000B2A: sub      y1,a                                        ; 200074
000B2B: tfr      y1,b #$10,y0                                ; 261079
000B2C: tfr      y1,a a,x0                                   ; 21C471
000B2D: maci     #>$80000,x0,b                               ; 0141CA 080000
000B2F: do       #<$8,>$b33                                  ; 060880 000B32
000B31: mac      y0,x0,a a,y:(r5)+                           ; 5E5DD2
000B32: mac      y0,x0,b b,y:(r5)+                           ; 5F5DDA
000B33: move     #$20,r4                                     ; 342000
000B34: move     #$10,r5                                     ; 351000
000B35: move     #$0,r2                                      ; 320000
000B36: move     x:>$2c9,r3                                  ; 63F000 0002C9
000B38: move     x:>$ff,r1                                   ; 61F000 0000FF
000B3A: move     #>$ffffff,m2                                ; 05F422 FFFFFF
000B3C: move     m2,m4                                       ; 0464A2
000B3D: move     #$0,r6                                      ; 360000
000B3E: move     l:(r5)+,x                                   ; 42DD00
000B3F: mpy      x1,x0,a x:(r2)+,x1 y:(r4)+,y0               ; F49AA0
000B40: mpy      x1,x0,b y:(r4)+,y1                          ; 4FDCA8
000B41: do       #<$10,>$b49                                 ; 061080 000B48
000B43: mac      x0,y1,b                                     ; 2000CA
000B44: mac      y0,x0,a l:(r5)+,x                           ; 42DDD2
000B45: add      a,b b,x:(r1)+ b,y:(r6)+                     ; BF5918
000B46: asr      b a,x:(r1)+ a,y:(r6)+                       ; BA592A
000B47: mpy      x1,x0,a x:(r2)+,x1 y:(r4)+,y0               ; F49AA0
000B48: mpy      x1,x0,b b,x:(r3)+ y:(r4)+,y1                ; FD1BA8
000B49: move     r3,x:>$2c9                                  ; 637000 0002C9
000B4B: pflush                                               ; 000003
000B4C: jmp      func_000143                                 ; 0C0143
