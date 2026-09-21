000000: jmp      func_ff0000                                 ; 0AF080 FF0000
000002: jmp      int_000002                                  ; 0C0002
000003: nop                                                  ; 000000
000004: jmp      int_000004                                  ; 0C0004
000005: nop                                                  ; 000000
000006: jmp      int_000006                                  ; 0C0006
000007: nop                                                  ; 000000
000008: jmp      int_000008                                  ; 0C0008
000009: nop                                                  ; 000000
00000A: jmp      int_00000a                                  ; 0C000A
00000B: nop                                                  ; 000000
00000C: jsr      func_000235                                 ; 0BF080 000235
00000E: nop                                                  ; 000000
00000F: nop                                                  ; 000000
000010: jsr      func_000219                                 ; 0BF080 000219
000012: jsr      func_000220                                 ; 0BF080 000220
000014: jsr      func_000227                                 ; 0BF080 000227
000016: jsr      func_00022e                                 ; 0BF080 00022E
000018: jmp      int_000018                                  ; 0C0018
000019: nop                                                  ; 000000
00001A: jsset    #$1,x:<<$fffff4,func_00023e                 ; 0BB4A1 00023E
00001C: jmp      int_00001c                                  ; 0C001C
00001D: nop                                                  ; 000000
00001E: jmp      int_00001e                                  ; 0C001E
00001F: nop                                                  ; 000000
000020: jsset    #$4,x:<<$fffff4,func_000259                 ; 0BB4A4 000259
000022: bset     #$1,x:<<$fffffe                             ; 0ABE21
000023: nop                                                  ; 000000
000024: jmp      int_000024                                  ; 0C0024
000025: nop                                                  ; 000000
000026: jmp      int_000026                                  ; 0C0026
000027: nop                                                  ; 000000
000028: jmp      int_000028                                  ; 0C0028
000029: nop                                                  ; 000000
00002A: jmp      int_00002a                                  ; 0C002A
00002B: nop                                                  ; 000000
00002C: jmp      int_00002c                                  ; 0C002C
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
000136: move     m0,m2                                       ; 0462A0
000137: move     m0,m3                                       ; 0463A0
000138: move     y:>$123,r6                                  ; 6EF000 000123
00013A: move     m0,m4                                       ; 0464A0
00013B: move     m0,m5                                       ; 0465A0
00013C: move     m0,m7                                       ; 0467A0
00013D: lua      (r6-$28),r6                                 ; 042E86
00013E: move     #>$100,r7                                   ; 67F400 000100
000140: move     x:>$2c3,r5                                  ; 65F000 0002C3
000142: jmp      func_0004a8                                 ; 0C04A8
000143: move     y:>$123,r6                                  ; 6EF000 000123
000145: move     #$0,x0                                      ; 240000
000146: move     x:>$2c4,a                                   ; 56F000 0002C4
000148: move     x0,y:(r6)                                   ; 4C6600
000149: brset    #$3,a,func_0001f3                           ; 0CCEA3 0000AA
00014B: brclr    #$0,a,func_000153                           ; 0CCE80 000008
00014D: move     x:>$2c0,r2                                  ; 62F000 0002C0
00014F: move     #$0,r4                                      ; 340000
000150: bsr      func_0001e2                                 ; 050912
000151: move     x:>$2c4,a                                   ; 56F000 0002C4
000153: brclr    #$1,a,func_00015b                           ; 0CCE81 000008
000155: move     x:>$2c1,r2                                  ; 62F000 0002C1
000157: move     #$0,r4                                      ; 340000
000158: bsr      func_0001e2                                 ; 05090A
000159: move     x:>$2c4,a                                   ; 56F000 0002C4
00015B: brclr    #$2,a,func_000163                           ; 0CCE82 000008
00015D: move     x:>$2c2,r2                                  ; 62F000 0002C2
00015F: move     #$0,r4                                      ; 340000
000160: bsr      func_0001e2                                 ; 050902
000161: move     x:>$2c4,a                                   ; 56F000 0002C4
000163: move     y:>$123,b                                   ; 5FF000 000123
000165: add      #>$100,b                                    ; 0140C8 000100
000167: move     y:>$124,a                                   ; 5EF000 000124
000169: add      #<$1,a                                      ; 014180
00016A: move     b,y:>$123                                   ; 5F7000 000123
00016C: move     a,y:>$124                                   ; 5E7000 000124
00016E: cmp      #<$3,a                                      ; 014385
00016F: beq      func_0001a3                                 ; 05A454
000170: move     x:>$2c8,y0                                  ; 46F000 0002C8
000172: move     y:>$124,a                                   ; 5EF000 000124
000174: cmp      #<$1,a                                      ; 014185
000175: bne      func_000179                                 ; 052404
000176: move     #>$a,x0                                     ; 44F400 00000A
000178: bra      func_00017f                                 ; 050C07
000179: movep    x:<<$ffffeb,a                               ; 084E2B
00017A: sub      y0,a                                        ; 200054
00017B: tst      a                                           ; 200003
00017C: bmi      func_000184                                 ; 05B408
00017D: move     #>$15,x0                                    ; 44F400 000015
00017F: movep    x:<<$ffffeb,a                               ; 084E2B
000180: sub      y0,a                                        ; 200054
000181: nop                                                  ; 000000
000182: cmp      x0,a                                        ; 200045
000183: blt      func_00017f                                 ; 0597DC
000184: move     y:>$124,a                                   ; 5EF000 000124
000186: cmp      #<$1,a                                      ; 014185
000187: beq      func_000189                                 ; 05A402
000188: bra      int_0000cf                                  ; 050E87
000189: move     x:<<$ffffb8,x0                              ; 44F000 FFFFB8
00018B: movep    x:<<$ffffbd,b                               ; 044FDD
00018C: eor      #<$2,b                                      ; 01428B
00018D: move     #>$60,x1                                    ; 45F400 000060
00018F: move     b,x:<<$ffffbd                               ; 577000 FFFFBD
000191: move     x:>$2c0,a                                   ; 56F000 0002C0
000193: add      #<$20,a                                     ; 016080
000194: cmp      #>$1a0,a                                    ; 0140C5 0001A0
000196: sub      x1,a ifeq                                   ; 202A64
000197: movep    #>$a5140,x:<<$ffffdc                        ; 08F49C 0A5140
000199: movep    #>$41,x:<<$fffff1                           ; 08F4B1 000041
00019B: movep    a1,x:<<$ffffde                              ; 08CC1E
00019C: movep    #>$ffffb8,x:<<$ffffdf                       ; 08F49F FFFFB8
00019E: movep    #>$301f,x:<<$ffffdd                         ; 08F49D 00301F
0001A0: movep    #>$ca5140,x:<<$ffffdc                       ; 08F49C CA5140
0001A2: bra      int_0000cf                                  ; 050E4D
0001A3: move     #$0,x0                                      ; 240000
0001A4: move     x0,x:>$4fe                                  ; 447000 0004FE
0001A6: move     x:>$2c4,a                                   ; 56F000 0002C4
0001A8: brset    #$8,a,func_0001ad                           ; 0CCEA8 000005
0001AA: brset    #$a,a,func_0001c2                           ; 0CCEAA 000018
0001AC: jmp      int_000087                                  ; 0C0087
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
0001EB: rts                                                  ; 00000C
0001EC: move     y:(r4)+,a                                   ; 5EDC00
0001ED: move     y:(r4)+,b                                   ; 5FDC00
0001EE: do       #<$10,>$1f2                                 ; 061080 0001F1
0001F0: move     a,x:(r2)+ y:(r4)+,a                         ; FA1A00
0001F1: move     b,x:(r2)+ y:(r4)+,b                         ; FF1A00
0001F2: rts                                                  ; 00000C
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
000219: movep    #>$500,x:<<$ffffda                          ; 08F49A 000500
00021B: movep    #>$33,x:<<$ffffd9                           ; 08F499 000033
00021D: movep    #>$8e9ac4,x:<<$ffffd8                       ; 08F498 8E9AC4
00021F: rti                                                  ; 000004
000220: movep    #>$600,x:<<$ffffda                          ; 08F49A 000600
000222: movep    #>$33,x:<<$ffffd9                           ; 08F499 000033
000224: movep    #>$8e9ac4,x:<<$ffffd8                       ; 08F498 8E9AC4
000226: rti                                                  ; 000004
000227: movep    #>$700,x:<<$ffffda                          ; 08F49A 000700
000229: movep    #>$33,x:<<$ffffd9                           ; 08F499 000033
00022B: movep    #>$8e9ac4,x:<<$ffffd8                       ; 08F498 8E9AC4
00022D: rti                                                  ; 000004
00022E: movep    x:<<$ffffc6,x:<<$ffffda                     ; 087086 FFFFDA
000230: movep    #>$7ff,x:<<$ffffd9                          ; 08F499 0007FF
000232: movep    #>$8e9ac4,x:<<$ffffd8                       ; 08F498 8E9AC4
000234: rti                                                  ; 000004
000235: move     r0,x:>$4ff                                  ; 607000 0004FF
000237: brclr    #$0,x:<<$ffffc3,func_000237                 ; 0CC300 000000
000239: movep    x:<<$ffffc6,r0                              ; 085006
00023A: movep    x:(r0),x:<<$ffffc7                          ; 08E087
00023B: move     x:>$4ff,r0                                  ; 60F000 0004FF
00023D: rti                                                  ; 000004
00023E: move     a2,x:>$2c7                                  ; 527000 0002C7
000240: move     a1,x:>$2c6                                  ; 547000 0002C6
000242: move     a0,x:>$2c5                                  ; 507000 0002C5
000244: move     x:>$4fe,a                                   ; 56F000 0004FE
000246: add      #<$1,a                                      ; 014180
000247: cmp      #<$3,a                                      ; 014385
000248: bge      func_000252                                 ; 05140A
000249: move     a,x:>$4fe                                   ; 567000 0004FE
00024B: move     x:>$2c6,a1                                  ; 54F000 0002C6
00024D: move     x:>$2c7,a2                                  ; 52F000 0002C7
00024F: move     x:>$2c5,a0                                  ; 50F000 0002C5
000251: rti                                                  ; 000004
000252: movep    #>$0,x:<<$ffffe8                            ; 08F4A8 000000
000254: movep    #>$0,x:<<$ffffe4                            ; 08F4A4 000000
000256: movep    #>$0,x:<<$ffffe0                            ; 08F4A0 000000
000258: jmp      func_000258                                 ; 0C0258
000259: movep    #>$4a5a20,x:<<$ffffdc                       ; 08F49C 4A5A20
00025B: movep    x:>$2ca,x:<<$ffffde                         ; 08F09E 0002CA
00025D: movep    #>$2f,x:<<$ffffdd                           ; 08F49D 00002F
00025F: movep    #>$8a52c0,x:<<$ffffdc                       ; 08F49C 8A52C0
000261: rti                                                  ; 000004
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
0002EC: jmp      func_000132                                 ; 0C0132
0002ED: move     #>$100,b                                    ; 57F400 000100
0002EF: move     #>$120,x0                                   ; 44F400 000120
0002F1: move     x:<<$ffffee,a                               ; 56F000 FFFFEE
0002F3: cmp      x0,a                                        ; 200045
0002F4: tlt      x0,b                                        ; 029048
0002F5: move     #$3f,m0                                     ; 053FA0
0002F6: move     b,r0                                        ; 21F000
0002F7: move     #$2,n0                                      ; 380200
0002F8: brset    #$7,x1,func_000319                          ; 0CC5A7 000021
0002FA: do       #<$10,>$2ff                                 ; 061080 0002FE
0002FC: move     x:(r0)+n0,x0                                ; 44C800
0002FD: move     x0,x:(r2)+                                  ; 445A00
0002FE: move     x0,x:(r2)+                                  ; 445A00
0002FF: move     #>$ffffff,m0                                ; 05F420 FFFFFF
000301: jmp      func_00011b                                 ; 0C011B
000302: move     #>$100,b                                    ; 57F400 000100
000304: move     #>$120,x0                                   ; 44F400 000120
000306: move     x:<<$ffffee,a                               ; 56F000 FFFFEE
000308: cmp      x0,a                                        ; 200045
000309: tlt      x0,b                                        ; 029048
00030A: move     #$3f,m0                                     ; 053FA0
00030B: move     b,r0                                        ; 21F000
00030C: move     #$2,n0                                      ; 380200
00030D: brset    #$6,x1,func_000319                          ; 0CC5A6 00000C
00030F: move     x:(r0)+,x0                                  ; 44D800
000310: do       #<$10,>$315                                 ; 061080 000314
000312: move     x:(r0)+n0,x0                                ; 44C800
000313: move     x0,x:(r2)+                                  ; 445A00
000314: move     x0,x:(r2)+                                  ; 445A00
000315: move     x:(r0)-,x0                                  ; 44D000
000316: move     #>$ffffff,m0                                ; 05F420 FFFFFF
000318: jmp      func_00011b                                 ; 0C011B
000319: do       #<$20,>$31d                                 ; 062080 00031C
00031B: move     x:(r0)+,x0                                  ; 44D800
00031C: move     x0,x:(r2)+                                  ; 445A00
00031D: move     #>$ffffff,m0                                ; 05F420 FFFFFF
00031F: jmp      func_00011b                                 ; 0C011B
000320: nop                                                  ; 000000
000321: nop                                                  ; 000000
000322: nop                                                  ; 000000
000323: nop                                                  ; 000000
000324: nop                                                  ; 000000
000325: nop                                                  ; 000000
000326: nop                                                  ; 000000
000327: nop                                                  ; 000000
000328: nop                                                  ; 000000
000329: nop                                                  ; 000000
00032A: nop                                                  ; 000000
00032B: nop                                                  ; 000000
00032C: nop                                                  ; 000000
00032D: nop                                                  ; 000000
00032E: nop                                                  ; 000000
00032F: nop                                                  ; 000000
000330: nop                                                  ; 000000
000331: nop                                                  ; 000000
000332: nop                                                  ; 000000
000333: nop                                                  ; 000000
000334: nop                                                  ; 000000
000335: nop                                                  ; 000000
000336: nop                                                  ; 000000
000337: nop                                                  ; 000000
000338: nop                                                  ; 000000
000339: nop                                                  ; 000000
00033A: nop                                                  ; 000000
00033B: nop                                                  ; 000000
00033C: nop                                                  ; 000000
00033D: nop                                                  ; 000000
00033E: nop                                                  ; 000000
00033F: nop                                                  ; 000000
000340: move     x:(r0)+,x0                                  ; 44D800
000341: move     #$2,n1                                      ; 390200
000342: tfr      x0,a x:(r0)+,x0 y:(r4)+,y0                  ; F09841
000343: do       #<$10,>$34f                                 ; 061080 00034E
000345: mac      y0,x0,a x:(r0)-,x1 y:(r4)+,y1               ; F590D2
000346: mac      y0,x0,a                                     ; 2000D2
000347: mac      y1,x1,a x:(r1)+,x0                          ; 44D9F2
000348: mac      -x0,y1,a x:(r1)-,x0                         ; 44D1C6
000349: mac      -y0,x0,a b,y:(r1)+n1                        ; 5F49D6
00034A: mac      -y0,x0,a x:(r0)+,x0 y:(r4)+,y0              ; F098D6
00034B: mpy      x1,y0,b y:(r4)+,y1                          ; 4FDCE8
00034C: tfr      x0,a a,x:(r1)- a,y0                         ; 181141
00034D: mac      y1,y0,b x:(r0)+,x0 y:(r4)+,y0               ; F098BA
00034E: asl      #$2,b,b                                     ; 0C1D85
00034F: rts                                                  ; 00000C
000350: move     x:(r0)+,x0      y:(r4)+,y1                  ; F19800
000351: move     #>$fffffe,n4                                ; 74F400 FFFFFE
000353: move     b,x1                                        ; 21E500
000354: do       #<$10,>$364                                 ; 061080 000363
000356: asr      #$4,b,b                                     ; 0C1C89
000357: mac      x0,y1,b y:(r4)+,y1                          ; 4FDCCA
000358: asl      #$4,b,b                                     ; 0C1D89
000359: mac      y1,x1,b a,x0 y:(r4)+n4,y0                   ; 10CCFA
00035A: mac      -y0,x0,b a,l:(r1)+                          ; 4859DE
00035B: mac      x0,y1,a x:(r0)+,x0 y:(r4)+,y1               ; F198C2
00035C: mac      x1,y0,a b,x1                                ; 21E5E2
00035D: asr      #$4,b,b                                     ; 0C1C89
00035E: mac      x0,y1,b y:(r4)+,y1                          ; 4FDCCA
00035F: asl      #$4,b,b                                     ; 0C1D89
000360: mac      y1,x1,b a,x0 y:(r4)+,y0                     ; 10DCFA
000361: mac      -y0,x0,b a,l:(r1)+                          ; 4859DE
000362: mac      x0,y1,a x:(r0)+,x0 y:(r4)+,y1               ; F198C2
000363: mac      x1,y0,a b,x1                                ; 21E5E2
000364: rts                                                  ; 00000C
000365: move     x:(r0)+,x1 y:(r4)+,y1                       ; F59800
000366: move     x:(r1),x0 y:(r5),y0                         ; C0A100
000367: move     #>$fffffe,n4                                ; 74F400 FFFFFE
000369: do       #<$10,>$37b                                 ; 061080 00037A
00036B: mac      -y1,x1,b b,x1 y:(r4)+,y1                    ; 1DDCFE
00036C: mac      y0,x0,a a,l:(r2)                            ; 4862D2
00036D: asl      #$7,a,a                                     ; 0C1D0E
00036E: mac      y1,x1,b x:(r2),x0 y:(r4)+n4,y0              ; D082FA
00036F: mac      -y0,x0,b a,l:(r1)+                          ; 4859DE
000370: move     l:(r2),a                                    ; 48E200
000371: mac      x0,y1,a x:(r1),x0 y:(r4)+,y1                ; F181C2
000372: mac      x1,y0,a x:(r0)+,x1 y:(r5),y0                ; C4B8E2
000373: mac      -y1,x1,b b,x1 y:(r4)+,y1                    ; 1DDCFE
000374: mac      y0,x0,a a,l:(r2)                            ; 4862D2
000375: asl      #$7,a,a                                     ; 0C1D0E
000376: mac      y1,x1,b x:(r2),x0 y:(r4)+,y0                ; F082FA
000377: mac      -y0,x0,b a,l:(r1)+                          ; 4859DE
000378: move     l:(r2),a                                    ; 48E200
000379: mac      x0,y1,a x:(r1),x0 y:(r4)+,y1                ; F181C2
00037A: mac      x1,y0,a x:(r0)+,x1 y:(r5),y0                ; C4B8E2
00037B: rts                                                  ; 00000C
00037C: move     x:(r0)+,x0 y:(r4)+n4,y0                     ; D09800
00037D: do       #<$10,>$387                                 ; 061080 000386
00037F: mac      y0,x0,a a,x:(r1)+ a,y1                      ; 1919D2
000380: mac      -y1,y0,a x:(r2)+,x0                         ; 44DAB6
000381: mac      y0,x0,b b,x:(r3)+ b,y1                      ; 1F1BDA
000382: mac      -y1,y0,b x:(r0)+,x0                         ; 44D8BE
000383: mac      y0,x0,a a,x:(r1)+ a,y1                      ; 1919D2
000384: mac      -y1,y0,a x:(r2)+,x0                         ; 44DAB6
000385: mac      y0,x0,b b,x:(r3)+ b,y1                      ; 1F1BDA
000386: mac      -y1,y0,b x:(r0)+,x0 y:(r4)+n4,y0            ; D098BE
000387: move     a,x:(r1)+                                   ; 565900
000388: move     b,x:(r3)+                                   ; 575B00
000389: rts                                                  ; 00000C
00038A: move     x:(r0)+,y0                                  ; 46D800
00038B: do       #<$10,>$394                                 ; 061080 000393
00038D: move     l:(r4)+,x                                   ; 42DC00
00038E: mac      -y0,x0,a a,x:(r1)+ a,y0                     ; 1819D6
00038F: mac      -x1,y0,a x:(r0),y0                          ; 46E0E6
000390: mac      y0,x0,a x:(r2)+,y0                          ; 46DAD2
000391: mac      -y0,x0,b b,x:(r3)+ b,y0                     ; 1E1BDE
000392: mac      -x1,y0,b x:(r2),y0                          ; 46E2EE
000393: mac      y0,x0,b x:(r0)+,y0                          ; 46D8DA
000394: move     a,x:(r1)+                                   ; 565900
000395: move     b,x:(r3)+                                   ; 575B00
000396: rts                                                  ; 00000C
000397: do       #<$8,>$3a7                                  ; 060880 0003A6
000399: move     x:(r4),r1                                   ; 61E400
00039A: mpy      -x1,y0,a a,x:(r2)+                          ; 565AE4
00039B: add      x1,a x:(r0)+n0,x1                           ; 45C860
00039C: macr     x1,y0,a x:(r0)+,x1                          ; 45D8E3
00039D: mpy      -x1,y0,b b,x:(r3)+                          ; 575BEC
00039E: add      x1,b x:(r0),x1                              ; 45E068
00039F: macr     x1,y0,b x:(r1)+,x1 y:(r4)+,y0               ; F499EB
0003A0: move     x:(r4),r0                                   ; 60E400
0003A1: mpy      -x1,y0,a a,x:(r2)+                          ; 565AE4
0003A2: add      x1,a x:(r1)+n1,x1                           ; 45C960
0003A3: macr     x1,y0,a x:(r1)+,x1                          ; 45D9E3
0003A4: mpy      -x1,y0,b b,x:(r3)+                          ; 575BEC
0003A5: add      x1,b x:(r1),x1                              ; 45E168
0003A6: macr     x1,y0,b x:(r0)+,x1 y:(r4)+,y0               ; F498EB
0003A7: move     a,x:(r2)+                                   ; 565A00
0003A8: move     b,x:(r3)+                                   ; 575B00
0003A9: rts                                                  ; 00000C
0003AA: move     x:(r1),a                                    ; 56E100
0003AB: asr      a #$0,r1                                    ; 310022
0003AC: move     #$1f,n4                                     ; 3C1F00
0003AD: move     #>$ffffe1,n5                                ; 75F400 FFFFE1
0003AF: move     #$3f,m0                                     ; 053FA0
0003B0: move     m0,m2                                       ; 0462A0
0003B1: move     x:(r1)+,n0                                  ; 70D900
0003B2: move     #>$312,x0                                   ; 44F400 000312
0003B4: move     #$10,n2                                     ; 3A1000
0003B5: do       a1,>$3cf                                    ; 06CC00 0003CE
0003B7: move     r3,r0                                       ; 227000
0003B8: move     #>$152,b                                    ; 57F400 000152
0003BA: move     l:(r1)+,y                                   ; 43D900
0003BB: move     (r0)+n0                                     ; 204800
0003BC: sub      y1,b r0,r2                                  ; 22127C
0003BD: tfr      y1,a y:(r7)+,y1                             ; 4FDF71
0003BE: add      x0,a b1,r4                                  ; 21B440
0003BF: mpysu    y1,y0,b                                     ; 0127A3
0003C0: asr      b a,r5                                      ; 21D52A
0003C1: move     (r0)-                                       ; 205000
0003C2: neg      b b,x1                                      ; 21E53E
0003C3: add      y1,b (r2)+n2                                ; 204A78
0003C4: move     x:(r2),a y:(r5)-,y1                         ; E9A200
0003C5: move     x:(r0),b b,y0                               ; 1EA000
0003C6: do       #<$10,>$3cd                                 ; 061080 0003CC
0003C8: mac      y1,x1,a y:(r5)+n5,y1                        ; 4FCDF2
0003C9: mac      y1,y0,a b,x:(r0)+                           ; 5758B2
0003CA: move     x:(r0),b y:(r4)+,y1                         ; FD8000
0003CB: mac      y1,x1,b a,x:(r2)+ y:(r4)+n4,y1              ; D91AFA
0003CC: mac      y1,y0,b x:(r2),a y:(r5)-,y1                 ; E9A2BA
0003CD: move     x:(r1)+,n0                                  ; 70D900
0003CE: move     b,x:(r0)+                                   ; 575800
0003CF: move     #>$ffffff,m0                                ; 05F420 FFFFFF
0003D1: move     m0,m2                                       ; 0462A0
0003D2: rts                                                  ; 00000C
0003D3: move     #>$10,y1                                    ; 47F400 000010
0003D5: bra      func_0003db                                 ; 050C06
0003D6: move     b0,y0                                       ; 212600
0003D7: mpysu    y1,y0,a                                     ; 012783
0003D8: move     l:(r0)+,x                                   ; 42D800
0003D9: add      x,b b1,x:(r1)+                              ; 555928
0003DA: move     a,l:(r1)+                                   ; 485900
0003DB: cmp      #<$10,b                                     ; 01508D
0003DC: ble      func_0003d6                                 ; 05F7DA
0003DD: sub      #<$10,b                                     ; 01508C
0003DE: move     r1,x:(r1)                                   ; 616100
0003DF: rts                                                  ; 00000C
0003E0: move     #>$10,y1                                    ; 47F400 000010
0003E2: bra      func_0003e7                                 ; 050C05
0003E3: move     b0,y0                                       ; 212600
0003E4: mpysu    y1,y0,a                                     ; 012783
0003E5: add      x,b b1,x:(r1)+                              ; 555928
0003E6: move     a,l:(r1)+                                   ; 485900
0003E7: cmp      #<$10,b                                     ; 01508D
0003E8: ble      func_0003e3                                 ; 05F7DB
0003E9: sub      #<$10,b                                     ; 01508C
0003EA: move     r1,x:(r1)                                   ; 616100
0003EB: rts                                                  ; 00000C
0003EC: move     #$1f,r0                                     ; 301F00
0003ED: move     x:$1f,a                                     ; 569F00
0003EE: do       n2,>$3f4                                    ; 06DA00 0003F3
0003F0: add      x1,b b,x0 y:(r4)+,y0                        ; 18DC68
0003F1: mpy      y0,x0,a a,x:(r0)+ y:(r5)+,y0                ; F838D0
0003F2: add      y0,a                                        ; 200050
0003F3: mac      -y0,x0,a                                    ; 2000D6
0003F4: move     #>$ffffff,m4                                ; 05F424 FFFFFF
0003F6: move     m4,m5                                       ; 0465A4
0003F7: move     a,x:(r0)+                                   ; 565800
0003F8: move     #$20,r0                                     ; 302000
0003F9: move     r0,r2                                       ; 221200
0003FA: move     x:(r6+$31),x1                               ; 02C6D5
0003FB: move     x:(r0)+,x0                                  ; 44D800
0003FC: mpy      x1,x0,a x:(r0)+,x0                          ; 44D8A0
0003FD: move     x:(r6+$30),y1                               ; 02C697
0003FE: mpy      x1,x0,a x:(r0)+,x0 a,y0                     ; 1098A0
0003FF: mpy      y1,y0,b                                     ; 2000B8
000400: asl      #$16,b,b                                    ; 0C1DAD
000401: do       n2,>$406                                    ; 06DA00 000405
000403: mpy      x1,x0,a x:(r0)+,x0 a,y0                     ; 1098A0
000404: mpy      y1,y0,b b,x:(r2)+                           ; 575AB8
000405: asl      #$16,b,b                                    ; 0C1DAD
000406: move     r6,n6                                       ; 22DE00
000407: bra      int_000421                                  ; 050C1A
000408: move     #$fc,r2                                     ; 32FC00
000409: move     #$1f,r0                                     ; 301F00
00040A: move     #>$8000,x1                                  ; 45F400 008000
00040C: move     #>$8008,y0                                  ; 46F400 008008
00040E: move     y0,x:(r2)+                                  ; 465A00
00040F: move     x0,x:(r2)-                                  ; 445200
000410: move     x:(r0),b y:(r5)+,y0                         ; FCA000
000411: mpy      x1,y0,a x:(r2)+,x0 y:(r5)+,y0               ; F0BAE0
000412: insert   x0,y0,a                                     ; 0C1B58
000413: do       n2,>$41a                                    ; 06DA00 000419
000415: move     b,x:(r0)+ y:(r5)+,y0                        ; FC3800
000416: mpy      x1,y0,a a0,x0                               ; 2104E0
000417: mpy      x0,y1,b x:(r2)-,x0 y:(r4)+,y0               ; F092C8
000418: mac      y0,x0,b         x:(r2)+,x0      y:(r5)+,y0  ; F0BADA
000419: insert   x0,y0,a                                     ; 0C1B58
00041A: move     b,x:(r0)+                                   ; 575800
00041B: move     a0,x0                                       ; 210400
00041C: mpy      x0,y1,b x:(r2)-,x0 y:(r4)+,y0               ; F092C8
00041D: mac      y0,x0,b r6,n6                               ; 22DEDA
00041E: move     m0,m4                                       ; 0464A0
00041F: move     m4,m5                                       ; 0465A4
000420: move     b,x:(r0)+                                   ; 575800
000421: move     #$0,r6                                      ; 360000
000422: move     r6,r4                                       ; 22D400
000423: move     #>$344,x1                                   ; 45F400 000344
000425: move     #>$10,x0                                    ; 44F400 000010
000427: move     #>$47f,r2                                   ; 62F400 00047F
000429: do       #<$10,>$42f                                 ; 061080 00042E
00042B: tfr      x1,a y:(r4)+,y1                             ; 4FDC61
00042C: mac      x0,y1,a b,l:(r2)+                           ; 495AC2
00042D: tfr      x1,b y:(r4)+,y1                             ; 4FDC69
00042E: mac      x0,y1,b a,l:(r2)+                           ; 485ACA
00042F: move     #>$480,r0                                   ; 60F400 000480
000431: move     r7,n0                                       ; 22F800
000432: move     r0,r7                                       ; 221700
000433: move     x:(r6)+,r3                                  ; 63DE00
000434: move     x:(r0)+,r4                                  ; 64D800
000435: move     #$20,n4                                     ; 3C2000
000436: move     x:(r0)+,r5                                  ; 65D800
000437: move     n4,n5                                       ; 239D00
000438: move     b,l:(r2)+                                   ; 495A00
000439: move     x:(r3)+,x0 y:(r4)+n4,y1                     ; D19B00
00043A: do       #<$10,>$454                                 ; 061080 000453
00043C: mpy      x0,y1,a x:(r3)+,x1 y:(r4)+n4,y1             ; D59BC0
00043D: mpy      x0,y1,b b,x:(r1)+ y:(r4)+n4,y1              ; DD19C8
00043E: mac      y1,x1,a l:(r4)+n4,y                         ; 43CCF2
00043F: mac      y1,x1,b x:(r3)+,x0 y:(r4)+n4,y1             ; D19BFA
000440: mac      y0,x0,a x:(r6)+,r2                          ; 62DED2
000441: mac      x0,y1,b x:(r3)+,x0 y:(r4)+n4,y1             ; D19BCA
000442: mac      x0,y1,a l:(r4)+n4,y                         ; 43CCC2
000443: mac      y0,x0,b x:(r3)+,x1 y:(r4)+n4,y0             ; D49BDA
000444: mac      y1,x1,a x:(r0)+,r4                          ; 64D8F2
000445: move     x:(r2)+,x0 y:(r5)+n5,y1                     ; D1BA00
000446: mac      x1,y0,b a,x1 y:(r7)+,y0                     ; 14DFEA
000447: macsu    x1,y0,b                                     ; 0126A6
000448: mpy      x0,y1,a x:(r2)+,x1 y:(r5)+n5,y1             ; D5BAC0
000449: mpy      x0,y1,b b,x:(r1)+ y:(r5)+n5,y1              ; DD39C8
00044A: mac      y1,x1,a l:(r5)+n5,y                         ; 43CDF2
00044B: mac      y1,x1,b x:(r2)+,x0 y:(r5)+n5,y1             ; D1BAFA
00044C: mac      y0,x0,a x:(r6)+,r3                          ; 63DED2
00044D: mac      x0,y1,b x:(r2)+,x0 y:(r5)+n5,y1             ; D1BACA
00044E: mac      x0,y1,a l:(r5)+n5,y                         ; 43CDC2
00044F: mac      y0,x0,b x:(r2)+,x1 y:(r5)+n5,y0             ; D4BADA
000450: mac      y1,x1,a x:(r0)+,r5                          ; 65D8F2
000451: move     x:(r3)+,x0 y:(r4)+n4,y1                     ; D19B00
000452: mac      x1,y0,b a,x1 y:(r7)+,y0                     ; 14DFEA
000453: macsu    x1,y0,b                                     ; 0126A6
000454: move     n6,r6                                       ; 23D600
000455: move     b,x:(r1)+                                   ; 575900
000456: move     n0,r7                                       ; 231700
000457: rts                                                  ; 00000C
000458: move     #$1e,r4                                     ; 341E00
000459: move     #$20,r5                                     ; 352000
00045A: move     r4,r0                                       ; 229000
00045B: move     l:(r6)+,x                                   ; 42DE00
00045C: move     x1,y:(r4)+                                  ; 4D5C00
00045D: move     x0,y:(r4)-                                  ; 4C5400
00045E: move     l:(r6)-,x                                   ; 42D600
00045F: tfr      x0,b y:(r5)+,y0                             ; 4EDD49
000460: move     y:(r4)+,a                                   ; 5EDC00
000461: move     #>$5da6b,y1                                 ; 47F400 05DA6B
000463: move     #>$1583e8,x0                                ; 44F400 1583E8
000465: do       #<$10,>$46b                                 ; 061080 00046A
000467: mac      y0,x0,a         x1,x:(r0)+      y:(r5)+,y0  ; F438D2
000468: mac      -x1,x0,a        b,x1            y:(r4)+,b   ; 1FDCA6
000469: mac      y1,y0,b         x1,x:(r0)+      y:(r5)+,y0  ; F438BA
00046A: mac      -y1,x1,b        a,x1            y:(r4)+,a   ; 16DCFE
00046B: move     x1,x:(r0)+                                  ; 455800
00046C: move     b,x:(r0)+ y:(r4)+,y0                        ; FC1800
00046D: move     a,x:(r6)                                    ; 566600
00046E: tfr      x1,a y0,y:(r6)+                             ; 4E5E61
00046F: move     #$20,r1                                     ; 312000
000470: move     ab,l:(r6)+                                  ; 4A5E00
000471: move     #$1e,r0                                     ; 301E00
000472: move     r0,r4                                       ; 221400
000473: move     l:(r6),y                                    ; 43E600
000474: tfr      y0,b x:(r1)+,x0                             ; 44D959
000475: move     x:(r0)+,a                                   ; 56D800
000476: move     #>$2a8f65,x1                                ; 45F400 2A8F65
000478: move     #>$4092ff,y0                                ; 46F400 4092FF
00047A: do       #<$10,>$480                                 ; 061080 00047F
00047C: mac      y0,x0,a         x:(r1)+,x0      y1,y:(r4)+  ; B199D2
00047D: mac      -y1,y0,a        x:(r0)+,b       b,y1        ; 1F98B6
00047E: mac      x1,x0,b         x:(r1)+,x0      y1,y:(r4)+  ; B199AA
00047F: mac      -y1,x1,b        x:(r0)+,a       a,y1        ; 1998FE
000480: move     y1,y:(r4)+                                  ; 4F5C00
000481: tfr      y1,a b,y:(r4)+                              ; 5F5C71
000482: move     #$20,r5                                     ; 352000
000483: move     ab,l:(r6)+                                  ; 4A5E00
000484: move     #$1e,r4                                     ; 341E00
000485: move     r4,r0                                       ; 229000
000486: move     l:(r6),x                                    ; 42E600
000487: tfr      x0,b y:(r5)+,y0                             ; 4EDD49
000488: move     y:(r4)+,a                                   ; 5EDC00
000489: move     #>$54e3ce,y1                                ; 47F400 54E3CE
00048B: move     #>$66e24c,x0                                ; 44F400 66E24C
00048D: do       #<$10,>$493                                 ; 061080 000492
00048F: mac      y0,x0,a         x1,x:(r0)+      y:(r5)+,y0  ; F438D2
000490: mac      -x1,x0,a        b,x1            y:(r4)+,b   ; 1FDCA6
000491: mac      y1,y0,b         x1,x:(r0)+      y:(r5)+,y0  ; F438BA
000492: mac      -y1,x1,b        a,x1            y:(r4)+,a   ; 16DCFE
000493: move     x1,x:(r6)                                   ; 456600
000494: move     x1,x:(r0)+ b,y:(r6)+                        ; B75800
000495: move     b,x:(r0)+                                   ; 575800
000496: move     #$1e,r0                                     ; 301E00
000497: move     #$1f,r1                                     ; 311F00
000498: move     #$fe,r7                                     ; 37FE00
000499: move     #$2,n7                                      ; 3F0200
00049A: move     #>$778898,x1                                ; 45F400 778898
00049C: move     l:(r6),y                                    ; 43E600
00049D: tfr      y0,b x:(r1)+,x0                             ; 44D959
00049E: do       #<$10,>$4a4                                 ; 061080 0004A3
0004A0: mac      x1,x0,b x:(r1)+,x0                          ; 44D9AA
0004A1: mac      -y1,x1,b x:(r0)+,a a,y:(r7)+n7              ; 9AF8FE
0004A2: add      b,a x:(r1)+,x0                              ; 44D910
0004A3: asr      a x:(r0)+,b b,y1                            ; 1F9822
0004A4: move     y1,x:(r6)                                   ; 476600
0004A5: move     b,y:(r6)                                    ; 5F6600
0004A6: move     a,y:(r7)+n7                                 ; 5E4F00
0004A7: rts                                                  ; 00000C
0004A8: move     y:>$124,a                                   ; 5EF000 000124
0004AA: tst      a                                           ; 200003
0004AB: bne      func_0004f5                                 ; 05248A
0004AC: move     x:(r6+$0),a                                 ; 02069E
0004AD: move     y:(r6+$21),b                                ; 0286FF
0004AE: tst      b #$0,x0                                    ; 24000B
0004AF: tfr      b,a ifne                                    ; 202201
0004B0: move     x0,y:(r6+$21)                               ; 0286E4
0004B1: move     a,x:(r6+$0)                                 ; 02068E
0004B2: cmp      #<$1,a                                      ; 014185
0004B3: bne      func_0004c7                                 ; 052414
0004B4: move     y:(r6+$18),a                                ; 0266BE
0004B5: asr      #$10,a,a                                    ; 0C1C20
0004B6: move     #>$141800,r4                                ; 64F400 141800
0004B8: move     a,n4                                        ; 21DC00
0004B9: move     y:>$4ff,a                                   ; 5EF000 0004FF
0004BB: move     y:(r4+n4),y0                                ; 4EEC00
0004BC: add      y0,a                                        ; 200050
0004BD: bes      func_0004c1                                 ; 05D404
0004BE: move     a,y:>$4ff                                   ; 5E7000 0004FF
0004C0: bra      func_0004f5                                 ; 050C55
0004C1: move     a,y:>$4ff                                   ; 5E7000 0004FF
0004C3: move     #>$4,x0                                     ; 44F400 000004
0004C5: move     x0,x:(r6+$0)                                ; 020684
0004C6: bra      func_0004f5                                 ; 050C4F
0004C7: cmp      #<$4,a                                      ; 014485
0004C8: bne      func_0004dc                                 ; 052414
0004C9: move     y:(r6+$1a),y0                               ; 026EB6
0004CA: mpy      y0,y0,b #>$141880,r4                        ; 64F498 141880
0004CC: move     y:(r6+$19),a                                ; 0266FE
0004CD: asr      #$10,a,a                                    ; 0C1C20
0004CE: move     a,n4                                        ; 21DC00
0004CF: move     y:>$4ff,x1                                  ; 4DF000 0004FF
0004D1: move     y:(r4+n4),y0                                ; 4EEC00
0004D2: mpy      -x1,y0,a                                    ; 2000E4
0004D3: cmp      b,a                                         ; 200005
0004D4: ble      func_0004d8                                 ; 05F404
0004D5: move     a,y:>$4ff                                   ; 5E7000 0004FF
0004D7: bra      func_0004f5                                 ; 050C1E
0004D8: move     #>$5,x0                                     ; 44F400 000005
0004DA: move     x0,x:(r6+$0)                                ; 020684
0004DB: bra      int_0004f5                                  ; 050C1A
0004DC: cmp      #<$5,a                                      ; 014585
0004DD: bne      func_0004e3                                 ; 052406
0004DE: move     y:(r6+$1a),x0                               ; 026EB4
0004DF: mpy      x0,x0,a                                     ; 200080
0004E0: move     a,y:>$4ff                                   ; 5E7000 0004FF
0004E2: bra      func_0004f5                                 ; 050C13
0004E3: cmp      #<$2,a                                      ; 014285
0004E4: bne      func_0004f1                                 ; 05240D
0004E5: move     y:(r6+$1b),a                                ; 026EFE
0004E6: asr      #$10,a,a                                    ; 0C1C20
0004E7: move     #>$141880,r4                                ; 64F400 141880
0004E9: move     a,n4                                        ; 21DC00
0004EA: move     y:>$4ff,x1                                  ; 4DF000 0004FF
0004EC: move     y:(r4+n4),y0                                ; 4EEC00
0004ED: mpy      -x1,y0,a                                    ; 2000E4
0004EE: move     a,y:>$4ff                                   ; 5E7000 0004FF
0004F0: jmp      func_0004f5                                 ; 0C04F5
0004F1: move     #>$7fffff,x0                                ; 44F400 7FFFFF
0004F3: move     x0,y:>$4ff                                  ; 4C7000 0004FF
0004F5: move     #$dc,n6                                     ; 3EDC00
0004F6: move     r5,x:>$ff                                   ; 657000 0000FF
0004F8: move     #$97,r2                                     ; 329700
0004F9: move     #$d7,r3                                     ; 33D700
0004FA: do       #<$11,>$4fe                                 ; 061180 0004FD
0004FC: move     a,x:(r2)+ y:(r7)+,a                         ; FA7A00
0004FD: move     b,x:(r3)+ y:(r7)+,b                         ; FF7B00
0004FE: lua      (r6)+n6,r7                                  ; 044E17
0004FF: move     y:(r6+$20),a                                ; 0286BE
000500: cmp      #<$1,a                                      ; 014185
000501: bne      func_000506                                 ; 052405
000502: move     #$0,x0                                      ; 240000
000503: move     x0,y:(r7-$1)                                ; 03FFE4
000504: move     x0,y:(r6+$20)                               ; 0286A4
000505: move     x0,x:(r7-$1)                                ; 03FFC4
000506: move     y:(r7-$1),a                                 ; 03FFFE
000507: tst      a                                           ; 200003
000508: bne      func_000517                                 ; 05240F
000509: move     y:(r6+$c),a                                 ; 0236BE
00050A: asr      #$10,a,a                                    ; 0C1C20
00050B: move     x:(r7-$1),b                                 ; 03FFDF
00050C: move     a,r4                                        ; 21D400
00050D: move     y:(r4+$141800),y0                           ; 0B74C6 141800
00050F: add      y0,b                                        ; 200058
000510: bec      func_000515                                 ; 055405
000511: move     #>$1,x0                                     ; 44F400 000001
000513: move     x0,y:(r7-$1)                                ; 03FFE4
000514: move     x0,x:(r7-$2)                                ; 03FF84
000515: move     b,x:(r7-$1)                                 ; 03FFCF
000516: bra      func_000537                                 ; 050C41
000517: cmp      #<$1,a                                      ; 014185
000518: bne      func_00052b                                 ; 052413
000519: clr      a                                           ; 200013
00051A: move     y:(r6+$23),y0                               ; 028EF6
00051B: move     a,x0                                        ; 21C400
00051C: mpy      y0,x0,b                                     ; 2000D8
00051D: move     x:(r7-$2),a                                 ; 03FF9E
00051E: move     b,x1                                        ; 21E500
00051F: mpyi     #>$791fd0,x1,b                              ; 0141E8 791FD0
000521: asl      b                                           ; 20003A
000522: add      #<$1,a                                      ; 014180
000523: cmp      b,a                                         ; 200005
000524: blt      func_000529                                 ; 059405
000525: move     #>$2,x0                                     ; 44F400 000002
000527: move     x0,y:(r7-$1)                                ; 03FFE4
000528: bra      func_00052b                                 ; 050C03
000529: move     a,x:(r7-$2)                                 ; 03FF8E
00052A: bra      int_000537                                  ; 050C0D
00052B: move     y:(r6+$d),a                                 ; 0236FE
00052C: add      #>$7fff,a                                   ; 0140C0 007FFF
00052E: asr      #$10,a,a                                    ; 0C1C20
00052F: rnd      a                                           ; 200011
000530: move     a,r4                                        ; 21D400
000531: move     x:(r7-$1),b                                 ; 03FFDF
000532: move     y:(r4+$141a00),y0                           ; 0B74C6 141A00
000534: sub      y0,b                                        ; 20005C
000535: clr      b               ifmi                        ; 202B1B
000536: move     b,x:(r7-$1)                                 ; 03FFCF
000537: move     y:(r6+$8),x0                                ; 0226B4
000538: mpyi     #>$800,x0,a                                 ; 0141C0 000800
00053A: sub      #>$80,a                                     ; 0140C4 000080
00053C: rnd      a                                           ; 200011
00053D: tfr      a,b                                         ; 200009
00053E: move     y:(r6+$e),a                                 ; 023EBE
00053F: add      #>$c00000,a                                 ; 0140C0 C00000
000541: abs      a a,y0                                      ; 21C626
000542: move     #>$700,x1                                   ; 45F400 000700
000544: move     a,y1                                        ; 21C700
000545: mpy      y1,y0,a                                     ; 2000B0
000546: asl      #$2,a,a                                     ; 0C1D04
000547: move     x:(r7-$1),y1                                ; 03FFD7
000548: move     a,y0                                        ; 21C600
000549: mpy      y1,y0,a                                     ; 2000B0
00054A: move     a,y1                                        ; 21C700
00054B: mpy      y1,x1,a                                     ; 2000F0
00054C: add      b,a                                         ; 200010
00054D: cmp      x1,a                                        ; 200065
00054E: tfr      x1,a ifge                                   ; 202161
00054F: move     #$8,y0                                      ; 260800
000550: move     x:(r6+$1),x1                                ; 0206D5
000551: move     y:(r6+$25),b                                ; 0296FF
000552: move     a1,x:(r7-$3)                                ; 03F7CC
000553: mac      x1,y0,a                                     ; 2000E2
000554: btst     #$b,b                                       ; 0BCF6B
000555: bcc      func_000557                                 ; 050402
000556: move     a1,x:(r7-$3)                                ; 03F7CC
000557: move     y:(r6+$9),x0                                ; 0226F4
000558: btst     #$9,b                                       ; 0BCF69
000559: mac      -x1,y0,a ifcc                               ; 2020E6
00055A: maci     #>$800,x0,a                                 ; 0141C2 000800
00055C: move     y:(r6+$f),b                                 ; 023EFF
00055D: add      #>$c00000,b                                 ; 0140C8 C00000
00055F: abs      b b,y0                                      ; 21E62E
000560: move     #>$700,x1                                   ; 45F400 000700
000562: move     b,y1                                        ; 21E700
000563: mpy      y1,y0,b                                     ; 2000B8
000564: asl      #$2,b,b                                     ; 0C1D85
000565: move     x:(r7-$1),y1                                ; 03FFD7
000566: tfr      a,b             b,y0                        ; 21E609
000567: mpy      y1,y0,a                                     ; 2000B0
000568: move     a,y1                                        ; 21C700
000569: mac      y1,x1,b                                     ; 2000FA
00056A: clr      b               ifmi                        ; 202B1B
00056B: move     y:(r7-$3),a                                 ; 03F7FE
00056C: move     b1,y:(r7-$2)                                ; 03FFAD
00056D: move     y:(r6+$10),b                                ; 0246BF
00056E: sub      a,b                                         ; 20001C
00056F: asl      #$8,b,b                                     ; 0C1D91
000570: rnd      b                                           ; 200019
000571: asr      #$8,b,b                                     ; 0C1C91
000572: add      b,a                                         ; 200010
000573: move     #$3,m4                                      ; 0503A4
000574: move     a,x0                                        ; 21C400
000575: move     x0,y:(r7-$3)                                ; 03F7E4
000576: mpyi     #>$4af,x0,a                                 ; 0141C0 0004AF
000578: move     y:(r6+$11),y0                               ; 0246F6
000579: move     a,r0                                        ; 21D000
00057A: mpyi     #>$80,y0,b                                  ; 0141D8 000080
00057C: move     b,r2                                        ; 21F200
00057D: move     x:(r0+$143f95),x0                           ; 0A70C4 143F95
00057F: move     x:(r2+$144446),y1                           ; 0A72C7 144446
000581: move     x0,b                                        ; 208F00
000582: add      y1,b                                        ; 200078
000583: asr      b #$1,a                                     ; 2E012A
000584: andi     #$fe,ccr                                    ; 00FEB9
000585: move     b,x1                                        ; 21E500
000586: do       #<$18,>$589                                 ; 061880 000588
000588: div      x1,a                                        ; 018060
000589: move     y1,b                                        ; 20EF00
00058A: sub      x0,b a0,x1                                  ; 21054C
00058B: move     x:(r0+$141ca7),y0                           ; 0A70C6 141CA7
00058D: move     b,x0                                        ; 21E400
00058E: mpy      x1,x0,b                                     ; 2000A8
00058F: asl      #$6,b,b                                     ; 0C1D8D
000590: move     #$4,r4                                      ; 340400
000591: move     b,x0                                        ; 21E400
000592: mpy      -y0,x0,a                                    ; 2000D4
000593: sub      y0,a #$96,r0                                ; 309654
000594: asr      a #$93,r1                                   ; 319322
000595: move     x:(r2+$1444c6),y0                           ; 0A72C6 1444C6
000597: move     x:(r2+$144546),y1                           ; 0A72C7 144546
000599: move     a,y:(r4)+                                   ; 5E5C00
00059A: move     x0,y:(r4)+                                  ; 4C5C00
00059B: move     l:(r7)+,x                                   ; 42DF00
00059C: move     x1,x:(r0)+ y0,y:(r4)+                       ; B41800
00059D: move     x0,x:(r0)- y1,y:(r4)+                       ; B11000
00059E: move     l:(r7)-,x                                   ; 42D700
00059F: move     x1,x:(r1)+                                  ; 455900
0005A0: move     x0,x:(r1)-                                  ; 445100
0005A1: jsr      func_000340                                 ; 0D0340
0005A2: move     x:-(r0),y0                                  ; 46F800
0005A3: move     x:-(r0),y1                                  ; 47F800
0005A4: move     x:(r1)+,x1                                  ; 45D900
0005A5: move     x:(r1)-,x0                                  ; 44D100
0005A6: move     b,y:(r1)                                    ; 5F6100
0005A7: move     y,l:(r7)+                                   ; 435F00
0005A8: move     x,l:(r7)+                                   ; 425F00
0005A9: move     x:(r6+$f),x0                                ; 023ED4
0005AA: jset     #$0,x0,func_0005bb                          ; 0AC420 0005BB
0005AC: move     #$4,r4                                      ; 340400
0005AD: move     #$d6,r0                                     ; 30D600
0005AE: move     #$d3,r1                                     ; 31D300
0005AF: move     l:(r7)+,x                                   ; 42DF00
0005B0: move     x1,x:(r0)+                                  ; 455800
0005B1: move     x0,x:(r0)-                                  ; 445000
0005B2: move     l:(r7)-,x                                   ; 42D700
0005B3: move     x1,x:(r1)+                                  ; 455900
0005B4: move     x0,x:(r1)-                                  ; 445100
0005B5: jsr      func_000340                                 ; 0D0340
0005B6: move     x:-(r0),y0                                  ; 46F800
0005B7: move     x:-(r0),y1                                  ; 47F800
0005B8: move     x:(r1)+,x1                                  ; 45D900
0005B9: move     x:(r1)-,x0                                  ; 44D100
0005BA: move     b,y:(r1)                                    ; 5F6100
0005BB: move     y,l:(r7)+                                   ; 435F00
0005BC: move     x,l:(r7)+                                   ; 425F00
0005BD: move     #>$ffffff,m4                                ; 05F424 FFFFFF
0005BF: lua      (r7-$18),r4                                 ; 043784
0005C0: move     #>$fffffe,n4                                ; 74F400 FFFFFE
0005C2: move     r4,r5                                       ; 229500
0005C3: move     #$91,r0                                     ; 309100
0005C4: move     #$d1,r2                                     ; 32D100
0005C5: move     l:(r4)+,x                                   ; 42DC00
0005C6: move     x0,y:(r0)+                                  ; 4C5800
0005C7: move     x1,y:(r0)+                                  ; 4D5800
0005C8: move     l:(r4)+,x                                   ; 42DC00
0005C9: move     x0,y:(r0)+                                  ; 4C5800
0005CA: move     x1,y:(r2)+                                  ; 4D5A00
0005CB: move     l:(r4)+,x                                   ; 42DC00
0005CC: move     x0,y:(r2)+                                  ; 4C5A00
0005CD: move     x1,y:(r2)+                                  ; 4D5A00
0005CE: move     #$91,r4                                     ; 349100
0005CF: move     #>$f528bd,x0                                ; 44F400 F528BD
0005D1: move     #>$4a4df0,x1                                ; 45F400 4A4DF0
0005D3: move     #$71,r1                                     ; 317100
0005D4: move     #$73,r2                                     ; 327300
0005D5: move     y:(r4)+,y0                                  ; 4EDC00
0005D6: move     #$3,n1                                      ; 390300
0005D7: move     n1,n2                                       ; 233A00
0005D8: do       #<$8,>$5e2                                  ; 060880 0005E1
0005DA: mpy      y0,x0,a a,x:(r1)+n1 y:(r4)+,y1              ; F909D0
0005DB: mpy      x0,y1,b b,x:(r2)+n2                         ; 574AC8
0005DC: mac      y1,x1,a         y:(r4)+,y0                  ; 4EDCF2
0005DD: mac      x1,y0,b y1,x:(r1)+                          ; 4759EA
0005DE: mac      x1,y0,a y:(r4)+,y1                          ; 4FDCE2
0005DF: mac      y1,x1,b y0,x:(r2)+                          ; 465AFA
0005E0: mac      x0,y1,a y:(r4)+n4,y0                        ; 4ECCC2
0005E1: mac      y0,x0,b         y:(r4)+,y0                  ; 4EDCDA
0005E2: move     a,x:(r1)+n1                                 ; 564900
0005E3: move     b,x:(r2)+n2                                 ; 574A00
0005E4: move     y:(r4)+,y1                                  ; 4FDC00
0005E5: move     y,l:(r5)+                                   ; 435D00
0005E6: move     y:(r4)-,y1                                  ; 4FD400
0005E7: move     y1,y:(r5)                                   ; 4F6500
0005E8: move     x:(r6+$f),y1                                ; 023ED7
0005E9: brset    #$0,y1,func_0005fb                          ; 0CC7A0 000012
0005EB: move     #$d1,r4                                     ; 34D100
0005EC: move     #$b1,r1                                     ; 31B100
0005ED: move     #$b3,r2                                     ; 32B300
0005EE: move     y:(r4)+,y0                                  ; 4EDC00
0005EF: do       #<$8,>$5f9                                  ; 060880 0005F8
0005F1: mpy      y0,x0,a a,x:(r1)+n1 y:(r4)+,y1              ; F909D0
0005F2: mpy      x0,y1,b b,x:(r2)+n2                         ; 574AC8
0005F3: mac      y1,x1,a         y:(r4)+,y0                  ; 4EDCF2
0005F4: mac      x1,y0,b y1,x:(r1)+                          ; 4759EA
0005F5: mac      x1,y0,a y:(r4)+,y1                          ; 4FDCE2
0005F6: mac      y1,x1,b y0,x:(r2)+                          ; 465AFA
0005F7: mac      x0,y1,a y:(r4)+n4,y0                        ; 4ECCC2
0005F8: mac      y0,x0,b         y:(r4)+,y0                  ; 4EDCDA
0005F9: move     a,x:(r1)+n1                                 ; 564900
0005FA: move     b,x:(r2)+n2                                 ; 574A00
0005FB: move     y0,x:(r5)+                                  ; 465D00
0005FC: move     y:(r4)+,y0                                  ; 4EDC00
0005FD: move     y:(r4)+,y1                                  ; 4FDC00
0005FE: move     y,l:(r5)                                    ; 436500
0005FF: move     y:(r7-$6),x1                                ; 03EFB5
000600: tfr      x1,a                                        ; 200061
000601: sub      #>$5bf,a                                    ; 0140C4 0005BF
000603: clr      a               ifmi                        ; 202B13
000604: move     a,y0                                        ; 21C600
000605: sub      #>$80,a                                     ; 0140C4 000080
000607: clr      a               ifmi                        ; 202B13
000608: move     a,y1                                        ; 21C700
000609: move     #>$63f,x0                                   ; 44F400 00063F
00060B: tfr      x1,a                                        ; 200061
00060C: cmp      x0,a                                        ; 200045
00060D: tfr      x0,a            ifgt                        ; 202741
00060E: move     a,x1                                        ; 21C500
00060F: move     x1,r0                                       ; 20B000
000610: move     y:(r6+$b),a                                 ; 022EFE
000611: mpyi     #>$fffdf3,x1,b                              ; 0141E8 FFFDF3
000613: maci     #>$ffe666,y0,b                              ; 0141DA FFE666
000615: maci     #>$ffa666,y1,b                              ; 0141FA FFA666
000617: asl      #$18,b,b                                    ; 0C1DB1
000618: tfr      x1,b b,y0                                   ; 21E669
000619: move     a,x0                                        ; 21C400
00061A: mac      y0,x0,a                                     ; 2000D2
00061B: clr      a               ifmi                        ; 202B13
00061C: move     a,x0                                        ; 21C400
00061D: maci     #>$fff912,x0,b                              ; 0141CA FFF912
00061F: move     x:(r7-$1e),x0                               ; 038F94
000620: move     y:(r7-$1e),a                                ; 038FBE
000621: move     b1,x:(r7-$1e)                               ; 038F8D
000622: move     x1,y:(r7-$1e)                               ; 038FA5
000623: add      x1,a b1,r2                                  ; 21B260
000624: asr      a                                           ; 200022
000625: add      x0,b                                        ; 200048
000626: asr      b a1,r1                                     ; 21912A
000627: move     a0,x0                                       ; 210400
000628: move     b0,x1                                       ; 212500
000629: move     x:(r1+$141a98),y0                           ; 0A71C6 141A98
00062B: move     x:(r1+$141a99),y1                           ; 0A71C7 141A99
00062D: tfr      y0,a b1,r3                                  ; 21B351
00062E: macsu    -y0,x0,a                                    ; 012695
00062F: macsu    y1,x0,a                                     ; 01268C
000630: move     x:(r1+$142158),y0                           ; 0A71C6 142158
000632: move     x:(r1+$142159),y1                           ; 0A71C7 142159
000634: mpysu    -y0,x0,b                                    ; 0127B5
000635: add      y0,b a,y0                                   ; 21C658
000636: macsu    y1,x0,b                                     ; 0126AC
000637: move     x:(r3+$142f06),x0                           ; 0A73C4 142F06
000639: move     x:(r3+$142f07),y1                           ; 0A73C7 142F07
00063B: mpysu    y1,x1,a                                     ; 012787
00063C: add      x0,a b,y1                                   ; 21E740
00063D: macsu    -x0,x1,a                                    ; 01269A
00063E: move     #>$7fffa4,x0                                ; 44F400 7FFFA4
000640: cmp      x0,a                                        ; 200045
000641: tgt      x0,a                                        ; 027040
000642: move     a,x0                                        ; 21C400
000643: mpy      x0,y1,b                                     ; 2000C8
000644: mpy      y0,x0,a                                     ; 2000D0
000645: move     b,y0                                        ; 21E600
000646: add      #>$800000,a                                 ; 0140C0 800000
000648: move     b,y:$1e                                     ; 5F1E00
000649: move     a,y:$1d                                     ; 5E1D00
00064A: mpy      x0,x0,b                                     ; 200088
00064B: subr     a,b                                         ; 20000E
00064C: sub      #>$400000,b                                 ; 0140CC 400000
00064E: andi     #$fe,ccr                                    ; 00FEB9
00064F: move     #>$800,a                                    ; 56F400 000800
000651: do       #<$18,>$654                                 ; 061880 000653
000653: div      y0,a                                        ; 018050
000654: move     b1,y1                                       ; 21A700
000655: move     b0,y0                                       ; 212600
000656: move     a0,x1                                       ; 210500
000657: mpysu    x1,y0,b                                     ; 0127A6
000658: dmac     ss x1,y1,b                                  ; 0124AF
000659: asl      #$a,b,b                                     ; 0C1D95
00065A: move     y:(r6+$4),a                                 ; 0216BE
00065B: asl      a b,y1                                      ; 21E732
00065C: move     a,x0                                        ; 21C400
00065D: mpyi     #>$100,x0,a                                 ; 0141C0 000100
00065F: move     a,r1                                        ; 21D100
000660: move     x:(r1+$143c06),x0                           ; 0A71C4 143C06
000662: mpy      x0,y1,b                                     ; 2000C8
000663: move     x:(r0+$141a98),y0                           ; 0A70C6 141A98
000665: move     b,y:$1c                                     ; 5F1C00
000666: move     x:(r0+$142158),y1                           ; 0A70C7 142158
000668: move     x:(r2+$142f06),a                            ; 0A72CE 142F06
00066A: move     #>$7fffa4,x0                                ; 44F400 7FFFA4
00066C: cmp      x0,a                                        ; 200045
00066D: tfr      x0,a            ifgt                        ; 202741
00066E: move     #$5,r4                                      ; 340500
00066F: move     a,x0                                        ; 21C400
000670: mpy      y0,x0,a                                     ; 2000D0
000671: mpy      x0,y1,b                                     ; 2000C8
000672: move     a,y:(r4)+                                   ; 5E5C00
000673: move     b,y:(r4)+                                   ; 5F5C00
000674: mpy      x0,x0,b b,y0                                ; 21E688
000675: subr     a,b                                         ; 20000E
000676: add      #>$400000,b                                 ; 0140C8 400000
000678: andi     #$fe,ccr                                    ; 00FEB9
000679: move     #>$800,a                                    ; 56F400 000800
00067B: do       #<$18,>$67e                                 ; 061880 00067D
00067D: div      y0,a                                        ; 018050
00067E: move     b1,y1                                       ; 21A700
00067F: move     b0,y0                                       ; 212600
000680: move     a0,x1                                       ; 210500
000681: mpysu    x1,y0,b                                     ; 0127A6
000682: dmac     ss x1,y1,b                                  ; 0124AF
000683: move     #$2,n0                                      ; 380200
000684: asl      #$a,b,b                                     ; 0C1D95
000685: move     y:(r6+$4),a                                 ; 0216BE
000686: asl      a b,y1                                      ; 21E732
000687: move     #$5,r0                                      ; 300500
000688: move     a,x0                                        ; 21C400
000689: mpyi     #>$100,x0,a                                 ; 0141C0 000100
00068B: move     a,r1                                        ; 21D100
00068C: move     x:(r1+$143c06),x0                           ; 0A71C4 143C06
00068E: move     #$4,r1                                      ; 310400
00068F: mpy      x0,y1,b y:(r0)+,a                           ; 5ED8C8
000690: add      #>$800000,a                                 ; 0140C0 800000
000692: move     b,y:$4                                      ; 5F0400
000693: move     l:(r7),x                                    ; 42E700
000694: move     a,y:(r7)                                    ; 5E6700
000695: move     y:$1d,a                                     ; 5E9D00
000696: sub      x0,a y:(r0)-,y1                             ; 4FD044
000697: move     y1,x:(r7)                                   ; 476700
000698: move     y:$1e,b                                     ; 5F9E00
000699: sub      x1,b a,y0                                   ; 21C66C
00069A: tfr      x0,a #$10,x0                                ; 241041
00069B: tfr      x1,b            b,y1                        ; 21E769
00069C: do       #<$8,>$6a0                                  ; 060880 00069F
00069E: mac      y0,x0,a a,y:(r0)+                           ; 5E58D2
00069F: mac      x0,y1,b b,y:(r0)+n0                         ; 5F48CA
0006A0: move     y:(r0)+,x0                                  ; 4CD800
0006A1: move     l:(r7)+,ba                                  ; 4BDF00
0006A2: sub      x0,a y:(r0)-,x1                             ; 4DD044
0006A3: sub      x1,b                                        ; 20006C
0006A4: tfr      x0,a            a,y0                        ; 21C641
0006A5: tfr      x1,b            b,y1                        ; 21E769
0006A6: move     #$10,x0                                     ; 241000
0006A7: do       #<$8,>$6ab                                  ; 060880 0006AA
0006A9: mac      y0,x0,a a,y:(r0)+                           ; 5E58D2
0006AA: mac      x0,y1,b b,y:(r0)+n0                         ; 5F48CA
0006AB: move     #$3,n0                                      ; 380300
0006AC: move     n0,n1                                       ; 231900
0006AD: move     #$1c,r0                                     ; 301C00
0006AE: move     y:$1c,x1                                    ; 4D9C00
0006AF: tfr      x1,a y:(r1),b                               ; 5FE161
0006B0: move     y:(r7),x0                                   ; 4CE700
0006B1: sub      x0,a b,y:(r7)+                              ; 5F5F44
0006B2: sub      x1,b                                        ; 20006C
0006B3: tfr      x0,a            a,y0                        ; 21C641
0006B4: tfr      x1,b            b,y1                        ; 21E769
0006B5: move     #$10,x0                                     ; 241000
0006B6: do       #<$8,>$6ba                                  ; 060880 0006B9
0006B8: mac      y0,x0,a a,y:(r1)+n1                         ; 5E49D2
0006B9: mac      x0,y1,b b,y:(r0)+n0                         ; 5F48CA
0006BA: move     #$74,r0                                     ; 307400
0006BB: move     r0,r1                                       ; 221100
0006BC: move     #$4,r4                                      ; 340400
0006BD: move     l:(r7)+,a                                   ; 48DF00
0006BE: move     l:(r7)-,b                                   ; 49D700
0006BF: jsr      func_000350                                 ; 0D0350
0006C0: move     a,l:(r7)+                                   ; 485F00
0006C1: move     b,l:(r7)+                                   ; 495F00
0006C2: move     x:(r6+$f),x0                                ; 023ED4
0006C3: jset     #$0,x0,func_0006cb                          ; 0AC420 0006CB
0006C5: move     #$b4,r0                                     ; 30B400
0006C6: move     r0,r1                                       ; 221100
0006C7: move     #$4,r4                                      ; 340400
0006C8: move     l:(r7)+,a                                   ; 48DF00
0006C9: move     l:(r7)-,b                                   ; 49D700
0006CA: jsr      func_000350                                 ; 0D0350
0006CB: move     a,l:(r7)+                                   ; 485F00
0006CC: move     b,l:(r7)+                                   ; 495F00
0006CD: move     x:(r7-$d),x1                                ; 03CFD5
0006CE: tfr      x1,a                                        ; 200061
0006CF: sub      #>$5bf,a                                    ; 0140C4 0005BF
0006D1: clr      a               ifmi                        ; 202B13
0006D2: move     a,y0                                        ; 21C600
0006D3: sub      #>$80,a                                     ; 0140C4 000080
0006D5: clr      a               ifmi                        ; 202B13
0006D6: move     a,y1                                        ; 21C700
0006D7: move     #>$63f,x0                                   ; 44F400 00063F
0006D9: tfr      x1,a                                        ; 200061
0006DA: cmp      x0,a                                        ; 200045
0006DB: tfr      x0,a            ifgt                        ; 202741
0006DC: tst      a                                           ; 200003
0006DD: clr      a               ifmi                        ; 202B13
0006DE: move     a,x1                                        ; 21C500
0006DF: move     x1,r0                                       ; 20B000
0006E0: move     y:(r6+$a),a                                 ; 022EBE
0006E1: mpyi     #>$fffdf3,x1,b                              ; 0141E8 FFFDF3
0006E3: maci     #>$ffe666,y0,b                              ; 0141DA FFE666
0006E5: maci     #>$ffd99a,y1,b                              ; 0141FA FFD99A
0006E7: asl      #$18,b,b                                    ; 0C1DB1
0006E8: tfr      x1,b b,y0                                   ; 21E669
0006E9: move     a,x0                                        ; 21C400
0006EA: mac      y0,x0,a                                     ; 2000D2
0006EB: clr      a               ifmi                        ; 202B13
0006EC: move     a,x0                                        ; 21C400
0006ED: maci     #>$fff912,x0,b                              ; 0141CA FFF912
0006EF: move     x:(r7-$25),x0                               ; 036FD4
0006F0: move     y:(r7-$25),a                                ; 036FFE
0006F1: move     b1,x:(r7-$25)                               ; 036FCD
0006F2: move     x1,y:(r7-$25)                               ; 036FE5
0006F3: add      x1,a b1,r2                                  ; 21B260
0006F4: asr      a                                           ; 200022
0006F5: add      x0,b                                        ; 200048
0006F6: asr      b a,r1                                      ; 21D12A
0006F7: move     a0,x0                                       ; 210400
0006F8: move     b0,x1                                       ; 212500
0006F9: move     x:(r1+$141a98),y0                           ; 0A71C6 141A98
0006FB: move     x:(r1+$141a99),y1                           ; 0A71C7 141A99
0006FD: tfr      y0,a b1,r3                                  ; 21B351
0006FE: macsu    -y0,x0,a                                    ; 012695
0006FF: macsu    y1,x0,a                                     ; 01268C
000700: move     x:(r1+$142158),y0                           ; 0A71C6 142158
000702: move     x:(r1+$142159),y1                           ; 0A71C7 142159
000704: mpysu    -y0,x0,b                                    ; 0127B5
000705: add      y0,b a,y0                                   ; 21C658
000706: macsu    y1,x0,b                                     ; 0126AC
000707: move     x:(r3+$142f07),y1                           ; 0A73C7 142F07
000709: move     x:(r3+$142f06),x0                           ; 0A73C4 142F06
00070B: mpysu    y1,x1,a                                     ; 012787
00070C: add      x0,a b,y1                                   ; 21E740
00070D: macsu    -x0,x1,a                                    ; 01269A
00070E: move     #>$7fffa4,x0                                ; 44F400 7FFFA4
000710: cmp      x0,a                                        ; 200045
000711: tgt      x0,a                                        ; 027040
000712: move     a,x0                                        ; 21C400
000713: mpy      x0,y1,b                                     ; 2000C8
000714: mpy      y0,x0,a                                     ; 2000D0
000715: move     b,y0                                        ; 21E600
000716: add      #>$800000,a                                 ; 0140C0 800000
000718: move     b,y:$1e                                     ; 5F1E00
000719: move     a,y:$1d                                     ; 5E1D00
00071A: mpy      x0,x0,b                                     ; 200088
00071B: subr     a,b                                         ; 20000E
00071C: sub      #>$400000,b                                 ; 0140CC 400000
00071E: andi     #$fe,ccr                                    ; 00FEB9
00071F: move     #>$800,a                                    ; 56F400 000800
000721: do       #<$18,>$724                                 ; 061880 000723
000723: div      y0,a                                        ; 018050
000724: move     b1,y1                                       ; 21A700
000725: move     b0,y0                                       ; 212600
000726: move     a0,x1                                       ; 210500
000727: mpysu    x1,y0,b                                     ; 0127A6
000728: dmac     ss x1,y1,b                                  ; 0124AF
000729: asl      #$a,b,b                                     ; 0C1D95
00072A: move     x:(r0+$141a98),y0                           ; 0A70C6 141A98
00072C: move     b,y:$1c                                     ; 5F1C00
00072D: move     x:(r0+$142158),y1                           ; 0A70C7 142158
00072F: move     x:(r2+$142f06),a                            ; 0A72CE 142F06
000731: move     #>$7fffa4,x0                                ; 44F400 7FFFA4
000733: cmp      x0,a                                        ; 200045
000734: tgt      x0,a                                        ; 027040
000735: move     #$5,r4                                      ; 340500
000736: move     a,x0                                        ; 21C400
000737: mpy      y0,x0,a                                     ; 2000D0
000738: mpy      x0,y1,b                                     ; 2000C8
000739: move     a,y:(r4)+                                   ; 5E5C00
00073A: move     b,y0                                        ; 21E600
00073B: mpy      x0,x0,b b,y:(r4)+                           ; 5F5C88
00073C: subr     a,b                                         ; 20000E
00073D: add      #>$400000,b                                 ; 0140C8 400000
00073F: andi     #$fe,ccr                                    ; 00FEB9
000740: move     #>$800,a                                    ; 56F400 000800
000742: do       #<$18,>$745                                 ; 061880 000744
000744: div      y0,a                                        ; 018050
000745: move     b1,y1                                       ; 21A700
000746: move     b0,y0                                       ; 212600
000747: move     a0,x1                                       ; 210500
000748: mpysu    x1,y0,b                                     ; 0127A6
000749: dmac     ss x1,y1,b                                  ; 0124AF
00074A: move     #$2,n0                                      ; 380200
00074B: asl      #$a,b,b                                     ; 0C1D95
00074C: move     b,y1                                        ; 21E700
00074D: move     #$5,r0                                      ; 300500
00074E: move     #$4,r1                                      ; 310400
00074F: move     y:(r0)+,a                                   ; 5ED800
000750: add      #>$800000,a                                 ; 0140C0 800000
000752: move     b,y:$4                                      ; 5F0400
000753: move     l:(r7),x                                    ; 42E700
000754: move     a,y:(r7)                                    ; 5E6700
000755: move     y:$1d,a                                     ; 5E9D00
000756: sub      x0,a y:(r0)-,y1                             ; 4FD044
000757: move     y1,x:(r7)                                   ; 476700
000758: move     y:$1e,b                                     ; 5F9E00
000759: sub      x1,b a,y0                                   ; 21C66C
00075A: tfr      x0,a #$10,x0                                ; 241041
00075B: tfr      x1,b            b,y1                        ; 21E769
00075C: do       #<$8,>$760                                  ; 060880 00075F
00075E: mac      y0,x0,a a,y:(r0)+                           ; 5E58D2
00075F: mac      x0,y1,b b,y:(r0)+n0                         ; 5F48CA
000760: move     y:(r0)+,x0                                  ; 4CD800
000761: move     l:(r7)+,ba                                  ; 4BDF00
000762: sub      x0,a y:(r0)-,x1                             ; 4DD044
000763: sub      x1,b                                        ; 20006C
000764: tfr      x0,a            a,y0                        ; 21C641
000765: tfr      x1,b            b,y1                        ; 21E769
000766: move     #$10,x0                                     ; 241000
000767: do       #<$8,>$76b                                  ; 060880 00076A
000769: mac      y0,x0,a a,y:(r0)+                           ; 5E58D2
00076A: mac      x0,y1,b b,y:(r0)+n0                         ; 5F48CA
00076B: move     #$3,n0                                      ; 380300
00076C: move     n0,n1                                       ; 231900
00076D: move     #$1c,r0                                     ; 301C00
00076E: move     y:$1c,x1                                    ; 4D9C00
00076F: tfr      x1,a y:(r1),b                               ; 5FE161
000770: move     y:(r7),x0                                   ; 4CE700
000771: sub      x0,a b,y:(r7)+                              ; 5F5F44
000772: sub      x1,b                                        ; 20006C
000773: tfr      x0,a            a,y0                        ; 21C641
000774: tfr      x1,b            b,y1                        ; 21E769
000775: move     #$10,x0                                     ; 241000
000776: do       #<$8,>$77a                                  ; 060880 000779
000778: mac      y0,x0,a a,y:(r1)+n1                         ; 5E49D2
000779: mac      x0,y1,b b,y:(r0)+n0                         ; 5F48CA
00077A: move     #$0,r5                                      ; 350000
00077B: move     #$1,r2                                      ; 320100
00077C: move     #>$fffffe,n7                                ; 77F400 FFFFFE
00077E: move     #$74,r0                                     ; 307400
00077F: move     #$72,r1                                     ; 317200
000780: move     #$4,r4                                      ; 340400
000781: move     #$10,x0                                     ; 241000
000782: move     x0,y:(r5)                                   ; 4C6500
000783: move     l:(r7)+,y                                   ; 43DF00
000784: move     l:(r7)+,a                                   ; 48DF00
000785: move     l:(r7)+n7,b                                 ; 49CF00
000786: move     y0,x:(r1)+                                  ; 465900
000787: move     y1,x:(r1)-                                  ; 475100
000788: jsr      func_000365                                 ; 0D0365
000789: move     x:(r1)+,x0                                  ; 44D900
00078A: move     x:(r1),x1                                   ; 45E100
00078B: move     x,l:(r7)+                                   ; 425F00
00078C: move     a,l:(r7)+                                   ; 485F00
00078D: move     b,l:(r7)+                                   ; 495F00
00078E: move     x:(r6+$f),x0                                ; 023ED4
00078F: jset     #$0,x0,func_00079c                          ; 0AC420 00079C
000791: move     #$b4,r0                                     ; 30B400
000792: move     #$b2,r1                                     ; 31B200
000793: move     #$4,r4                                      ; 340400
000794: move     l:(r7)+,y                                   ; 43DF00
000795: move     l:(r7)+,a                                   ; 48DF00
000796: move     l:(r7)+n7,b                                 ; 49CF00
000797: move     y0,x:(r1)+                                  ; 465900
000798: move     y1,x:(r1)-                                  ; 475100
000799: jsr      func_000365                                 ; 0D0365
00079A: move     x:(r1)+,x0                                  ; 44D900
00079B: move     x:(r1),x1                                   ; 45E100
00079C: move     x,l:(r7)+                                   ; 425F00
00079D: move     a,l:(r7)+                                   ; 485F00
00079E: move     b,l:(r7)+                                   ; 495F00
00079F: move     x:(r6+$b),y0                                ; 022ED6
0007A0: move     #$8,a                                       ; 2E0800
0007A1: andi     #$fe,ccr                                    ; 00FEB9
0007A2: do       #<$18,>$7a5                                 ; 061880 0007A4
0007A4: div      y0,a                                        ; 018050
0007A5: move     a0,y1                                       ; 210700
0007A6: move     y:(r6+$4),a                                 ; 0216BE
0007A7: asl      a #$b2,r0                                   ; 30B232
0007A8: add      #>$800000,a                                 ; 0140C0 800000
0007AA: clr      a               ifmi                        ; 202B13
0007AB: move     #$72,r1                                     ; 317200
0007AC: move     a,x0                                        ; 21C400
0007AD: mpyi     #>$100,x0,b                                 ; 0141C8 000100
0007AF: mpy      x0,x0,a #$70,r5                             ; 357080
0007B0: move     #$b0,r4                                     ; 34B000
0007B1: move     b,r2                                        ; 21F200
0007B2: move     a,x1                                        ; 21C500
0007B3: mpyi     #>$7deccd,x1,a                              ; 0141E0 7DECCD
0007B5: add      #>$21333,a                                  ; 0140C0 021333
0007B7: move     x:(r2+$1447c6),x1                           ; 0A72C5 1447C6
0007B9: mpy      x1,y0,b a,y0                                ; 21C6E8
0007BA: mpy      y1,y0,a                                     ; 2000B0
0007BB: move     b,x1                                        ; 21E500
0007BC: move     x:(r0)+,x0 a,y0                             ; 109800
0007BD: do       #<$22,>$7c3                                 ; 062280 0007C2
0007BF: mpy      y0,x0,a x:(r1)+,x0 a,y:(r4)+                ; B299D0
0007C0: asl      #$8,a,a                                     ; 0C1D10
0007C1: mpy      y0,x0,b x:(r0)+,x0 b,y:(r5)+                ; B3B8D8
0007C2: asl      #$8,b,b                                     ; 0C1D91
0007C3: bset     #$b,sr                                      ; 0AF96B
0007C4: move     #$70,r5                                     ; 357000
0007C5: move     #$b0,r4                                     ; 34B000
0007C6: move     r5,r1                                       ; 22B100
0007C7: move     r4,r0                                       ; 229000
0007C8: move     #$80,x0                                     ; 248000
0007C9: move     y:(r4)+,y0                                  ; 4EDC00
0007CA: move     y:(r5)+,y1                                  ; 4FDD00
0007CB: do       #<$22,>$7cf                                 ; 062280 0007CE
0007CD: mpy      y0,x0,a a,x:(r0)+ y:(r4)+,y0                ; F818D0
0007CE: mpy      x0,y1,b b,x:(r1)+ y:(r5)+,y1                ; FD39C8
0007CF: move     a,x:(r0)+                                   ; 565800
0007D0: move     b,x:(r1)+                                   ; 575900
0007D1: bclr     #$b,sr                                      ; 0AF94B
0007D2: move     #>$6a3,x0                                   ; 44F400 0006A3
0007D4: move     y:(r7-$14),a                                ; 03B7BE
0007D5: cmp      x0,a x1,y1                                  ; 20A745
0007D6: tfr      x0,a ifge                                   ; 202141
0007D7: move     l:(r7),x                                    ; 42E700
0007D8: move     a,r0                                        ; 21D000
0007D9: move     x:(r0+$143546),y0                           ; 0A70C6 143546
0007DB: mpy      y1,y0,b y0,a                                ; 20CEB8
0007DC: move     #$72,r0                                     ; 307200
0007DD: move     #$4,r4                                      ; 340400
0007DE: sub      x0,a ba,l:(r7)+                             ; 4B5F44
0007DF: sub      x1,b                                        ; 20006C
0007E0: move     a,y0                                        ; 21C600
0007E1: tfr      x1,b            b,y1                        ; 21E769
0007E2: tfr      x0,a #$8,x0                                 ; 240841
0007E3: do       #<$10,>$7e7                                 ; 061080 0007E6
0007E5: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
0007E6: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
0007E7: move     #$4,r4                                      ; 340400
0007E8: move     r0,r1                                       ; 221100
0007E9: move     #$b2,r2                                     ; 32B200
0007EA: move     r2,r3                                       ; 225300
0007EB: move     l:(r7)+,a                                   ; 48DF00
0007EC: move     l:(r7)-,b                                   ; 49D700
0007ED: move     x:(r0)+,x0      y:(r4)+,y0                  ; F09800
0007EE: move     y:(r4)+,x1                                  ; 4DDC00
0007EF: do       #<$10,>$7fa                                 ; 061080 0007F9
0007F1: mac      x1,x0,a a,x:(r1)+ a,y1                      ; 1919A2
0007F2: mac      -y1,y0,a x:(r2)+,x0                         ; 44DAB6
0007F3: mac      x1,x0,b b,x:(r3)+ b,y1                      ; 1F1BAA
0007F4: mac      -y1,y0,b x:(r0)+,x0                         ; 44D8BE
0007F5: mac      x1,x0,a a,x:(r1)+ a,y1                      ; 1919A2
0007F6: mac      -y1,y0,a x:(r2)+,x0                         ; 44DAB6
0007F7: mac      x1,x0,b b,x:(r3)+ b,y1                      ; 1F1BAA
0007F8: mac      -y1,y0,b x:(r0)+,x0 y:(r4)+,y0              ; F098BE
0007F9: move     y:(r4)+,x1                                  ; 4DDC00
0007FA: move     a,x:(r1)+                                   ; 565900
0007FB: move     b,x:(r3)+                                   ; 575B00
0007FC: move     a,l:(r7)+                                   ; 485F00
0007FD: move     b,l:(r7)+                                   ; 495F00
0007FE: move     #$4,r4                                      ; 340400
0007FF: move     #$2,n4                                      ; 3C0200
000800: move     #$72,r0                                     ; 307200
000801: move     r0,r1                                       ; 221100
000802: move     #$b2,r2                                     ; 32B200
000803: move     r2,r3                                       ; 225300
000804: move     l:(r7)+,a                                   ; 48DF00
000805: move     l:(r7)-,b                                   ; 49D700
000806: jsr      func_00037c                                 ; 0D037C
000807: move     a,l:(r7)+                                   ; 485F00
000808: move     b,l:(r7)+                                   ; 495F00
000809: move     #>$eeb88,y0                                 ; 46F400 0EEB88
00080B: move     #>$39c201,y1                                ; 47F400 39C201
00080D: lua      (r7-$30),r3                                 ; 042F03
00080E: move     r3,r4                                       ; 227400
00080F: move     #$6d,r0                                     ; 306D00
000810: move     #$6c,r2                                     ; 326C00
000811: move     l:(r3)+,x                                   ; 42DB00
000812: move     x0,x:(r0)+                                  ; 445800
000813: move     x1,x:(r0)+                                  ; 455800
000814: move     l:(r3)+,x                                   ; 42DB00
000815: move     x0,x:(r0)+                                  ; 445800
000816: move     x1,x:(r0)+                                  ; 455800
000817: move     x:(r3),x1                                   ; 45E300
000818: move     x1,x:(r0)+                                  ; 455800
000819: move     #$6d,r0                                     ; 306D00
00081A: move     #>$f75277,x1                                ; 45F400 F75277
00081C: move     #>$fffffd,n0                                ; 70F400 FFFFFD
00081E: move     x:(r0)+,x0                                  ; 44D800
00081F: do       #<$10,>$828                                 ; 061080 000827
000821: mpy      x1,x0,a x:(r0)+,x0                          ; 44D8A0
000822: mac      y0,x0,a         x:(r0)+,x0                  ; 44D8D2
000823: mac      x0,y1,a x:(r0)+,x0                          ; 44D8C2
000824: mac      x0,y1,a x:(r0)+,x0                          ; 44D8C2
000825: mac      y0,x0,a         x:(r0)+n0,x0                ; 44C8D2
000826: mac      x1,x0,a b,x:(r2)+                           ; 575AA2
000827: tfr      a,b x:(r0)+,x0                              ; 44D809
000828: move     a,x:(r2)+                                   ; 565A00
000829: tfr      x0,a r0,r1                                  ; 221141
00082A: move     x:(r0)+,x1                                  ; 45D800
00082B: move     x,l:(r4)+                                   ; 425C00
00082C: move     x:(r0)+,x0                                  ; 44D800
00082D: move     x:(r0)+,x1                                  ; 45D800
00082E: move     x,l:(r4)+                                   ; 425C00
00082F: move     x:(r0)+,x1                                  ; 45D800
000830: move     x1,x:(r4)                                   ; 456400
000831: move     a,x0                                        ; 21C400
000832: move     r1,r0                                       ; 223000
000833: move     x:(r6+$f),x1                                ; 023ED5
000834: jset     #$0,x1,func_00084e                          ; 0AC520 00084E
000836: move     #$ad,r0                                     ; 30AD00
000837: move     #$ac,r2                                     ; 32AC00
000838: move     y:(r3)+,x0                                  ; 4CDB00
000839: move     x0,x:(r0)+                                  ; 445800
00083A: move     l:(r3)+,x                                   ; 42DB00
00083B: move     x0,x:(r0)+                                  ; 445800
00083C: move     x1,x:(r0)+                                  ; 455800
00083D: move     l:(r3)+,x                                   ; 42DB00
00083E: move     x0,x:(r0)+                                  ; 445800
00083F: move     x1,x:(r0)+                                  ; 455800
000840: move     #$ad,r0                                     ; 30AD00
000841: move     #>$f75277,x1                                ; 45F400 F75277
000843: move     x:(r0)+,x0                                  ; 44D800
000844: do       #<$10,>$84d                                 ; 061080 00084C
000846: mpy      x1,x0,a x:(r0)+,x0                          ; 44D8A0
000847: mac      y0,x0,a         x:(r0)+,x0                  ; 44D8D2
000848: mac      x0,y1,a x:(r0)+,x0                          ; 44D8C2
000849: mac      x0,y1,a x:(r0)+,x0                          ; 44D8C2
00084A: mac      y0,x0,a         x:(r0)+n0,x0                ; 44C8D2
00084B: mac      x1,x0,a b,x:(r2)+                           ; 575AA2
00084C: tfr      a,b x:(r0)+,x0                              ; 44D809
00084D: move     a,x:(r2)+                                   ; 565A00
00084E: move     x0,y:(r4)+                                  ; 4C5C00
00084F: move     x:(r0)+,x0                                  ; 44D800
000850: move     x:(r0)+,x1                                  ; 45D800
000851: move     x,l:(r4)+                                   ; 425C00
000852: move     x:(r0)+,x0                                  ; 44D800
000853: move     x:(r0)+,x1                                  ; 45D800
000854: move     x,l:(r4)+                                   ; 425C00
000855: move     #>$63f,x0                                   ; 44F400 00063F
000857: move     x:(r7-$1a),a                                ; 039F9E
000858: tst      a                                           ; 200003
000859: clr      a               ifmi                        ; 202B13
00085A: cmp      x0,a                                        ; 200045
00085B: tfr      x0,a            ifgt                        ; 202741
00085C: move     a,r0                                        ; 21D000
00085D: move     #>$7fffff,a                                 ; 56F400 7FFFFF
00085F: move     l:(r7),x                                    ; 42E700
000860: move     x:(r0+$1435c6),b                            ; 0A70CF 1435C6
000862: sub      b,a a,y0                                    ; 21C614
000863: add      y0,a #$6c,r0                                ; 306C50
000864: asr      a #$4,r4                                    ; 340422
000865: sub      x0,a ba,l:(r7)+                             ; 4B5F44
000866: sub      x1,b a,y0                                   ; 21C66C
000867: tfr      x0,a #$8,x0                                 ; 240841
000868: tfr      x1,b            b,y1                        ; 21E769
000869: do       #<$10,>$86d                                 ; 061080 00086C
00086B: mac      y0,x0,a a,y:(r4)                            ; 5E64D2
00086C: mac      x0,y1,b b,x:(r4)+                           ; 575CCA
00086D: move     #$4,r4                                      ; 340400
00086E: move     r0,r1                                       ; 221100
00086F: move     #$ac,r2                                     ; 32AC00
000870: move     r2,r3                                       ; 225300
000871: move     l:(r7)+,a                                   ; 48DF00
000872: move     l:(r7)+,b                                   ; 49DF00
000873: move     l:(r7),x                                    ; 42E700
000874: move     x0,x:(r0)                                   ; 446000
000875: move     x1,x:(r2)                                   ; 456200
000876: move     x:(r0+$10),x0                               ; 024094
000877: move     x:(r2+$10),x1                               ; 024295
000878: move     x,l:(r7)-                                   ; 425700
000879: jsr      func_00038a                                 ; 0D038A
00087A: move     b,l:(r7)-                                   ; 495700
00087B: move     a,l:(r7)                                    ; 486700
00087C: lua      (r7+$3),r7                                  ; 040737
00087D: move     #$4,r4                                      ; 340400
00087E: move     #$6c,r0                                     ; 306C00
00087F: move     r0,r1                                       ; 221100
000880: move     #$ac,r2                                     ; 32AC00
000881: move     r2,r3                                       ; 225300
000882: move     l:(r7)+,a                                   ; 48DF00
000883: move     l:(r7)+,b                                   ; 49DF00
000884: move     l:(r7),x                                    ; 42E700
000885: move     x0,x:(r0)                                   ; 446000
000886: move     x1,x:(r2)                                   ; 456200
000887: move     x:(r0+$10),x0                               ; 024094
000888: move     x:(r2+$10),x1                               ; 024295
000889: move     x,l:(r7)-                                   ; 425700
00088A: jsr      func_00038a                                 ; 0D038A
00088B: move     b,l:(r7)-                                   ; 495700
00088C: move     a,l:(r7)                                    ; 486700
00088D: lua      (r7+$3),r7                                  ; 040737
00088E: move     y:(r6+$28),a                                ; 02A6BE
00088F: move     y:(r7-$23),b                                ; 0377FF
000890: cmp      #<$1,a                                      ; 014185
000891: bne      func_000894                                 ; 052403
000892: move     #$0,x0                                      ; 240000
000893: move     x0,x:(r7-$22)                               ; 037F84
000894: cmp      #<$2,a                                      ; 014285
000895: blt      func_00089d                                 ; 059408
000896: move     #>$3,x0                                     ; 44F400 000003
000898: cmp      #<$3,a                                      ; 014385
000899: bne      func_00089c                                 ; 052403
00089A: move     #>$4,x0                                     ; 44F400 000004
00089C: move     x0,x:(r7-$22)                               ; 037F84
00089D: move     x:(r7-$22),a                                ; 037F9E
00089E: tst      a #$40,r0                                   ; 304003
00089F: bne      func_0008ae                                 ; 05240F
0008A0: move     y:(r6+$0),a                                 ; 0206BE
0008A1: asr      #$10,a,a                                    ; 0C1C20
0008A2: move     a,r4                                        ; 21D400
0008A3: move     y:(r7-$23),a                                ; 0377FE
0008A4: move     y:(r4+$141800),y0                           ; 0B74C6 141800
0008A6: add      y0,a                                        ; 200050
0008A7: move     a,y:(r7-$23)                                ; 0377EE
0008A8: bec      func_0008d8                                 ; 055450
0008A9: move     #>$1,x0                                     ; 44F400 000001
0008AB: move     x0,x:(r7-$22)                               ; 037F84
0008AC: move     x0,y:(r7-$22)                               ; 037FA4
0008AD: bra      func_0008d8                                 ; 050C4B
0008AE: cmp      #<$1,a                                      ; 014185
0008AF: bne      func_0008c6                                 ; 052417
0008B0: move     y:(r6+$1),a                                 ; 0206FE
0008B1: cmp      #>$10000,a                                  ; 0140C5 010000
0008B3: clr      a               iflt                        ; 202913
0008B4: move     y:(r6+$23),y0                               ; 028EF6
0008B5: move     a,x0                                        ; 21C400
0008B6: mpy      y0,x0,b                                     ; 2000D8
0008B7: move     y:(r7-$22),a                                ; 037FBE
0008B8: move     b,x1                                        ; 21E500
0008B9: mpyi     #>$791fd0,x1,b                              ; 0141E8 791FD0
0008BB: asl      b                                           ; 20003A
0008BC: add      #<$1,a                                      ; 014180
0008BD: cmp      b,a                                         ; 200005
0008BE: bgt      func_0008c1                                 ; 057403
0008BF: move     a,y:(r7-$22)                                ; 037FAE
0008C0: bra      func_0008d8                                 ; 050C18
0008C1: move     #>$2,x0                                     ; 44F400 000002
0008C3: move     x0,x:(r7-$22)                               ; 037F84
0008C4: move     x0,a                                        ; 208E00
0008C5: bra      func_0008d8                                 ; 050C13
0008C6: cmp      #<$2,a                                      ; 014285
0008C7: bne      func_0008ca                                 ; 052403
0008C8: move     y:(r6+$2),b                                 ; 020EBF
0008C9: bra      func_0008ce                                 ; 050C05
0008CA: move     y:(r6+$3),b                                 ; 020EFF
0008CB: cmp      #<$4,a                                      ; 014485
0008CC: bne      func_0008ce                                 ; 052402
0008CD: move     #$20,b                                      ; 2F2000
0008CE: add      #>$7fff,b                                   ; 0140C8 007FFF
0008D0: asr      #$10,b,b                                    ; 0C1CA1
0008D1: rnd      b                                           ; 200019
0008D2: move     b,r4                                        ; 21F400
0008D3: move     y:(r7-$23),x1                               ; 0377F5
0008D4: move     y:(r4+$141880),y0                           ; 0B74C6 141880
0008D6: mpy      -x1,y0,a                                    ; 2000E4
0008D7: move     a,y:(r7-$23)                                ; 0377EE
0008D8: move     y:(r6+$6),x0                                ; 021EB4
0008D9: move     #>$408e05,y0                                ; 46F400 408E05
0008DB: mpy      y0,x0,b #$11,r3                             ; 3311D8
0008DC: asl      b #$10,n0                                   ; 38103A
0008DD: move     b,a                                         ; 21EE00
0008DE: asr      #$c,a,a                                     ; 0C1C18
0008DF: move     y:(r6+$5),x0                                ; 0216F4
0008E0: mpy      x0,x0,a a,r2                                ; 21D280
0008E1: move     a,x0                                        ; 21C400
0008E2: move     y:(r7-$23),x1                               ; 0377F5
0008E3: mpy      x1,x0,a                                     ; 2000A0
0008E4: mpy      x1,x0,a                                     ; 2000A0
0008E5: move     x:(r2+$14a000),x1                           ; 0A72C5 14A000
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
000917: cmp      #>$400000,b                                 ; 0140CD 400000
000919: move     l:(r7),a                                    ; 48E700
00091A: asr      a ifeq                                      ; 202A22
00091B: move     a,l:(r7)                                    ; 486700
00091C: tfr      y1,b #>$7fffff,x1                           ; 45F479 7FFFFF
00091E: asr      #$10,b,b                                    ; 0C1CA1
00091F: move     y,l:?:>$fe                                  ; 437000 0000FE
000921: move     b0,a                                        ; 212E00
000922: lsr      a b1,y1                                     ; 21A723
000923: move     b0,y0                                       ; 212600
000924: move     a1,x0                                       ; 218400
000925: move     l:(r7)+,a                                   ; 48DF00
000926: move     a0,b                                        ; 210F00
000927: lsr      b #$80,r1                                   ; 31802B
000928: move     a1,r0                                       ; 219000
000929: do       #<$11,>$92e                                 ; 061180 00092D
00092B: add      y,a a,l:(r1)                                ; 486130
00092C: add      x0,b b1,y:(r1)+                             ; 5D5948
00092D: and      x1,b                                        ; 20006E
00092E: move     y:>$80,y0                                   ; 4EF000 000080
000930: move     #$81,r4                                     ; 348100
000931: move     #$22,r2                                     ; 322200
000932: move     #$34,r3                                     ; 333400
000933: move     l:(r7)+,ab                                  ; 4ADF00
000934: move     x:(r0)+,x1                                  ; 45D800
000935: move     a,x:(r2)+                                   ; 565A00
000936: move     b,x:(r3)+                                   ; 575B00
000937: move     l:(r7)-,ab                                  ; 4AD700
000938: jsr      func_000397                                 ; 0D0397
000939: move     #>$10,b                                     ; 57F400 000010
00093B: move     l:-(r7),a                                   ; 48FF00
00093C: sub      a,b #$11,n0                                 ; 38111C
00093D: move     y:>$fe,y0                                   ; 4EF000 0000FE
00093F: move     b1,x1                                       ; 21A500
000940: move     b0,x0                                       ; 212400
000941: mpysu    y0,x0,b                                     ; 0127A5
000942: dmac     ss y0,x1,b                                  ; 0124AE
000943: asl      b #>$22,x1                                  ; 45F43A 000022
000945: move     #>$7fffff,x0                                ; 44F400 7FFFFF
000947: add      x,b n0,n1                                   ; 231928
000948: rnd      b y:(r7)+,y0                                ; 4EDF19
000949: move     b,r0                                        ; 21F000
00094A: move     x:>$fe,a                                    ; 56F000 0000FE
00094C: asr      #$10,a,a                                    ; 0C1C20
00094D: move     x:(r0)+,x1                                  ; 45D800
00094E: move     x:(r0)+n0,y1                                ; 47C800
00094F: move     x:(r0)+,x0                                  ; 44D800
000950: move     x:(r0),y0                                   ; 46E000
000951: move     x,l:(r7)+                                   ; 425F00
000952: move     y,l:(r7)-                                   ; 435700
000953: move     x:(r0+$4b),b                                ; 0A70CF 00004B
000955: move     y:(r0+$4b),b0                               ; 0B70C9 00004B
000957: sub      #<$10,b                                     ; 01508C
000958: cmp      a,b y:>$fe,y0                               ; 4EF00D 0000FE
00095A: tfr      a,b             ifgt                        ; 202709
00095B: move     #>$23,a                                     ; 56F400 000023
00095D: move     l:-(r7),x                                   ; 42FF00
00095E: move     b,l:(r7)                                    ; 496700
00095F: mpysu    y0,x0,b                                     ; 0127A5
000960: dmac     ss y0,x1,b                                  ; 0124AE
000961: asl      b #$f,r2                                    ; 320F3A
000962: sub      b,a r2,r3                                   ; 225314
000963: move     y:>$fe,b                                    ; 5FF000 0000FE
000965: asr      #$16,b,b                                    ; 0C1CAD
000966: move     b1,y1                                       ; 21A700
000967: move     b0,y0                                       ; 212600
000968: asr      b #>$7fffff,x1                              ; 45F42A 7FFFFF
00096A: move     b0,x0                                       ; 212400
00096B: move     a0,b                                        ; 210F00
00096C: lsr      b #$80,r1                                   ; 31802B
00096D: move     a1,r0                                       ; 219000
00096E: do       #<$10,>$973                                 ; 061080 000972
000970: add      y,a a,l:(r1)                                ; 486130
000971: add      x0,b b1,y:(r1)+                             ; 5D5948
000972: and      x1,b                                        ; 20006E
000973: move     y:>$80,y0                                   ; 4EF000 000080
000975: move     #$81,r4                                     ; 348100
000976: move     x:(r0)+,x1                                  ; 45D800
000977: move     #$f,m2                                      ; 050FA2
000978: jsr      func_000397                                 ; 0D0397
000979: move     #>$ffffff,m2                                ; 05F422 FFFFFF
00097B: lua      (r6+$28),r6                                 ; 041686
00097C: move     x:(r6-$1c),a                                ; 03969E
00097D: tst      a                                           ; 200003
00097E: beq      func_000981                                 ; 05A403
00097F: move     a,r0                                        ; 21D000
000980: jmp      (r0)                                        ; 0AE080
000981: lua      (r6-$28),r6                                 ; 042E86
000982: move     #>$15c,n6                                   ; 76F400 00015C
000984: lua      (r6)+n6,r7                                  ; 044E17
000985: move     y:(r6+$13),a                                ; 024EFE
000986: add      #>$8000,a                                   ; 0140C0 008000
000988: asr      #$10,a,a                                    ; 0C1C20
000989: move     #>$1,b                                      ; 57F400 000001
00098B: move     a,r0                                        ; 21D000
00098C: move     b1,x:(r7-$8f)                               ; 0A778D FFFF71
00098E: move     b0,y:(r7-$8f)                               ; 0B7789 FFFF71
000990: move     x:(r0+$144bc9),x1                           ; 0A70C5 144BC9
000992: move     x:(r0+$144c49),x0                           ; 0A70C4 144C49
000994: move     y:(r6+$23),y0                               ; 028EF6
000995: mpyi     #>$791fd0,y0,a                              ; 0141D0 791FD0
000997: asl      #$5,a,a                                     ; 0C1D0A
000998: move     a,y0                                        ; 21C600
000999: mpy      x1,y0,a                                     ; 2000E0
00099A: cmp      #>$4000,a                                   ; 0140C5 004000
00099C: blt      func_0009ab                                 ; 05940F
00099D: move     y:(r6+$2a),y0                               ; 02AEB6
00099E: mpy      y0,x0,b                                     ; 2000D8
00099F: move     #>$21d10,x1                                 ; 45F400 021D10
0009A1: move     b1,y1                                       ; 21A700
0009A2: move     b0,y0                                       ; 212600
0009A3: mpysu    x1,y0,b                                     ; 0127A6
0009A4: dmac     ss x1,y1,b                                  ; 0124AF
0009A5: move     #>$3ff0,a                                   ; 56F400 003FF0
0009A7: move     b1,x:(r7-$8f)                               ; 0A778D FFFF71
0009A9: move     b0,y:(r7-$8f)                               ; 0B7789 FFFF71
0009AB: move     a1,x:(r7-$8e)                               ; 0A778C FFFF72
0009AD: move     a0,y:(r7-$8e)                               ; 0B7788 FFFF72
0009AF: move     x:(r7-$8e),b                                ; 0A77CF FFFF72
0009B1: move     y:(r7-$8e),b0                               ; 0B77C9 FFFF72
0009B3: move     x:(r7-$8c),a                                ; 0A77CE FFFF74
0009B5: move     y:(r7-$8c),a0                               ; 0B77C8 FFFF74
0009B7: move     #$40,y1                                     ; 274000
0009B8: move     a,l:?:>$fc                                  ; 487000 0000FC
0009BA: move     y1,x:>$fe                                   ; 477000 0000FE
0009BC: cmp      b,a                                         ; 200005
0009BD: beq      func_0009cb                                 ; 05A40E
0009BE: bgt      func_0009c5                                 ; 057407
0009BF: sub      a,b #>$4,y0                                 ; 46F41C 000004
0009C1: cmpm     y0,b                                        ; 20005F
0009C2: tfr      y0,b ifgt                                   ; 202759
0009C3: add      b,a #$30,y1                                 ; 273010
0009C4: bra      func_0009cb                                 ; 050C07
0009C5: sub      a,b #>$fffffc,y0                            ; 46F41C FFFFFC
0009C7: cmp      y0,b                                        ; 20005D
0009C8: tfr      y0,b            iflt                        ; 202959
0009C9: add      b,a #>$500000,y1                            ; 47F410 500000
0009CB: move     x:(r7-$8d),b                                ; 0A77CF FFFF73
0009CD: move     y:(r7-$8d),b0                               ; 0B77C9 FFFF73
0009CF: asl      #$a,b,b                                     ; 0C1D95
0009D0: cmp      #>$3ff,b                                    ; 0140CD 0003FF
0009D2: blt      func_0009d7                                 ; 059405
0009D3: move     y1,x:>$fe                                   ; 477000 0000FE
0009D5: move     a,l:?:>$fc                                  ; 487000 0000FC
0009D7: move     y:(r7-$8f),y0                               ; 0B77C6 FFFF71
0009D9: move     y:(r7-$8d),y1                               ; 0B77C7 FFFF73
0009DB: move     #>$ccccd,x0                                 ; 44F400 0CCCCD
0009DD: move     #>$733333,x1                                ; 45F400 733333
0009DF: mpysu    x0,y0,a                                     ; 01278D
0009E0: macsu    x1,y1,a                                     ; 01268F
0009E1: move     x:(r7-$8f),y0                               ; 0A77C6 FFFF71
0009E3: move     x:(r7-$8d),y1                               ; 0A77C7 FFFF73
0009E5: dmac     ss x0,y0,a                                  ; 01248D
0009E6: mac      y1,x1,a                                     ; 2000F2
0009E7: move     x:(r7-$8c),b                                ; 0A77CF FFFF74
0009E9: cmp      #>$3ff0,b                                   ; 0140CD 003FF0
0009EB: blt      func_0009f0                                 ; 059405
0009EC: move     a1,x:(r7-$8d)                               ; 0A778C FFFF73
0009EE: move     a0,y:(r7-$8d)                               ; 0B7788 FFFF73
0009F0: move     #>$3fff,x0                                  ; 44F400 003FFF
0009F2: move     y:(r7-$8b),x1                               ; 0B77C5 FFFF75
0009F4: move     x:(r7-$86),b                                ; 0A77CF FFFF7A
0009F6: move     y:(r7-$86),b0                               ; 0B77C9 FFFF7A
0009F8: move     l:?:>$fc,y                                  ; 43F000 0000FC
0009FA: move     y1,x:(r7-$8c)                               ; 0A7787 FFFF74
0009FC: move     y0,y:(r7-$8c)                               ; 0B7786 FFFF74
0009FE: move     y1,a                                        ; 20EE00
0009FF: sub      b,a                                         ; 200014
000A00: neg      a                                           ; 200036
000A01: and      x0,a                                        ; 200046
000A02: move     #$0,a2                                      ; 2A0000
000A03: add      x1,a a,l:?:>$c2                             ; 487060 0000C2
000A05: add      x1,b                                        ; 200068
000A06: move     a1,r4                                       ; 219400
000A07: move     b,l:?:>$c4                                  ; 497000 0000C4
000A09: add      #>$4000,a                                   ; 0140C0 004000
000A0B: add      #>$4000,b                                   ; 0140C8 004000
000A0D: move     a1,r5                                       ; 219500
000A0E: move     b,l:?:>$c5                                  ; 497000 0000C5
000A10: move     x:(r7-$8d),a                                ; 0A77CE FFFF73
000A12: move     y:(r7-$8d),a0                               ; 0B77C8 FFFF73
000A14: asl      #$4,a,a                                     ; 0C1D08
000A15: move     x:(r7-$86),b                                ; 0A77CF FFFF7A
000A17: move     y:(r7-$86),b0                               ; 0B77C9 FFFF7A
000A19: add      a,b #$40,r0                                 ; 304018
000A1A: and      x0,b #$62,r1                                ; 31624E
000A1B: move     #>$3fff,m4                                  ; 05F424 003FFF
000A1D: move     b1,x:(r7-$86)                               ; 0A778D FFFF7A
000A1F: move     b0,y:(r7-$86)                               ; 0B7789 FFFF7A
000A21: move     m4,m5                                       ; 0465A4
000A22: move     y:(r6+$14),a                                ; 0256BE
000A23: sub      #>$400000,a                                 ; 0140C4 400000
000A25: bmi      func_000a29                                 ; 0D104B 000004
000A27: move     #$62,r0                                     ; 306200
000A28: move     #$40,r1                                     ; 314000
000A29: move     y:(r4)+,a                                   ; 5EDC00
000A2A: move     y:(r4)+,b                                   ; 5FDC00
000A2B: do       #<$10,>$a2f                                 ; 061080 000A2E
000A2D: move     a,x:(r0)+ y:(r4)+,a                         ; FA1800
000A2E: move     b,x:(r0)+ y:(r4)+,b                         ; FF1800
000A2F: move     a,x:(r0)+ y:(r5)+,a                         ; FA3800
000A30: move     b,x:(r0)+ y:(r5)+,b                         ; FF3800
000A31: do       #<$11,>$a35                                 ; 061180 000A34
000A33: move     a,x:(r1)+ y:(r5)+,a                         ; FA3900
000A34: move     b,x:(r1)+ y:(r5)+,b                         ; FF3900
000A35: move     #>$ffffff,m4                                ; 05F424 FFFFFF
000A37: move     m4,m5                                       ; 0465A4
000A38: move     x:>$fe,y1                                   ; 47F000 0000FE
000A3A: move     x:(r7-$8d),x1                               ; 0A77C5 FFFF73
000A3C: move     y:(r7-$8d),x0                               ; 0B77C4 FFFF73
000A3E: mpysu    y1,x0,b                                     ; 0127AC
000A3F: dmac     ss y1,x1,b                                  ; 0124A7
000A40: asl      b #>$40,a                                   ; 56F43A 000040
000A42: move     y:>$c2,a0                                   ; 58F000 0000C2
000A44: move     b1,y1                                       ; 21A700
000A45: move     b0,y0                                       ; 212600
000A46: asr      b #>$7fffff,x1                              ; 45F42A 7FFFFF
000A48: move     #$22,n0                                     ; 382200
000A49: move     n0,n1                                       ; 231900
000A4A: move     b0,x0                                       ; 212400
000A4B: move     a0,b                                        ; 210F00
000A4C: lsr      b a1,r0                                     ; 21902B
000A4D: move     #$e0,r1                                     ; 31E000
000A4E: do       #<$11,>$a53                                 ; 061180 000A52
000A50: add      y,a a,l:(r1)                                ; 486130
000A51: add      x0,b b1,y:(r1)+                             ; 5D5948
000A52: and      x1,b                                        ; 20006E
000A53: move     #$2f,r2                                     ; 322F00
000A54: move     r2,r3                                       ; 225300
000A55: move     y:>$e0,y0                                   ; 4EF000 0000E0
000A57: move     #$e1,r4                                     ; 34E100
000A58: move     x:(r0)+,x1                                  ; 45D800
000A59: move     #$f,m2                                      ; 050FA2
000A5A: jsr      func_000397                                 ; 0D0397
000A5B: move     #>$ffffff,m2                                ; 05F422 FFFFFF
000A5D: move     y:(r6+$16),b                                ; 025EBF
000A5E: asr      #$10,b,b                                    ; 0C1CA1
000A5F: move     #>$144ac7,r3                                ; 63F400 144AC7
000A61: move     b,n3                                        ; 21FB00
000A62: move     #$20,r1                                     ; 312000
000A63: move     x:(r3+n3),b                                 ; 57EB00
000A64: asr      b b,y0                                      ; 21E62A
000A65: add      #>$800000,b                                 ; 0140C8 800000
000A67: move     #$62,r4                                     ; 346200
000A68: neg      b #$30,r0                                   ; 30303E
000A69: move     y:(r7-$88),a                                ; 0B77CE FFFF78
000A6B: move     y:(r7-$89),x0                               ; 0B77C4 FFFF77
000A6D: move     x:(r7-$89),x1                               ; 0A77C5 FFFF77
000A6F: move     x:(r1+$1f),y1                               ; 0279D7
000A70: move     y1,y:(r7-$89)                               ; 0B7787 FFFF77
000A72: move     x:(r1+$f),y1                                ; 0239D7
000A73: move     y1,x:(r7-$89)                               ; 0A7787 FFFF77
000A75: move     b,y1                                        ; 21E700
000A76: move     x:(r7-$88),b                                ; 0A77CF FFFF78
000A78: do       #<$10,>$a80                                 ; 061080 000A7F
000A7A: mac      -x0,y1,a a,x0 a,y:(r4)+                     ; 125CC6
000A7B: mac      -y1,x1,b b,x1 b,y:(r4)+                     ; 1F5CFE
000A7C: mac      -y0,x0,a x:(r0),x0                          ; 44E0D6
000A7D: mac      -x1,y0,b x:(r1),x1                          ; 45E1EE
000A7E: mac      x0,y1,a x:(r0)+,x0                          ; 44D8C2
000A7F: mac      y1,x1,b x:(r1)+,x1                          ; 45D9FA
000A80: move     a,y:(r7-$88)                                ; 0B778E FFFF78
000A82: move     b,x:(r7-$88)                                ; 0A778F FFFF78
000A84: move     y:(r6+$17),a                                ; 025EFE
000A85: move     y:(r6+$16),x0                               ; 025EB4
000A86: add      x0,a #>$144ac7,r2                           ; 62F440 144AC7
000A88: move     #$20,r4                                     ; 342000
000A89: move     #$62,r5                                     ; 356200
000A8A: move     a,b                                         ; 21CF00
000A8B: asr      #$10,b,b                                    ; 0C1CA1
000A8C: move     y:(r7-$87),a                                ; 0B77CE FFFF79
000A8E: move     b,n2                                        ; 21FA00
000A8F: move     x:(r7-$87),b                                ; 0A77CF FFFF79
000A91: move     y:(r5)+,x1                                  ; 4DDD00
000A92: move     x:(r2+n2),y0                                ; 46EA00
000A93: do       #<$10,>$a99                                 ; 061080 000A98
000A95: mac      x1,y0,a a,x0 a,y:(r4)+                      ; 125CE2
000A96: mac      -y0,x0,a y:(r5)+,x1                         ; 4DDDD6
000A97: mac      x1,y0,b b,x0 b,y:(r4)+                      ; 1B5CEA
000A98: mac      -y0,x0,b y:(r5)+,x1                         ; 4DDDDE
000A99: move     a,y:(r7-$87)                                ; 0B778E FFFF79
000A9B: move     b,x:(r7-$87)                                ; 0A778F FFFF79
000A9D: move     #$62,r4                                     ; 346200
000A9E: move     #$0,r1                                      ; 310000
000A9F: move     x:(r7-$8d),b                                ; 0A77CF FFFF73
000AA1: move     y:(r7-$8d),b0                               ; 0B77C9 FFFF73
000AA3: asr      #$11,b,b                                    ; 0C1CA3
000AA4: move     y:(r7-$90),a                                ; 0B77CE FFFF70
000AA6: move     b0,r2                                       ; 213200
000AA7: move     x:(r7-$90),b                                ; 0A77CF FFFF70
000AA9: move     x:(r2+$144b48),y0                           ; 0A72C6 144B48
000AAB: move     #$10,r3                                     ; 331000
000AAC: move     x:(r3)+,x1                                  ; 45DB00
000AAD: do       #<$10,>$ab3                                 ; 061080 000AB2
000AAF: mac      x1,y0,a a,x0 a,y:(r4)+                      ; 125CE2
000AB0: mac      -y0,x0,a x:(r1)+,x1                         ; 45D9D6
000AB1: mac      x1,y0,b b,x0 b,y:(r4)+                      ; 1B5CEA
000AB2: mac      -y0,x0,b x:(r3)+,x1                         ; 45DBDE
000AB3: move     a,y:(r7-$90)                                ; 0B778E FFFF70
000AB5: move     b,x:(r7-$90)                                ; 0A778F FFFF70
000AB7: move     y:(r6+$14),b                                ; 0256BF
000AB8: sub      #>$400000,b                                 ; 0140CC 400000
000ABA: abs      b #$62,r4                                   ; 34622E
000ABB: asl      b #$20,r1                                   ; 31203A
000ABC: move     #$51,r2                                     ; 325100
000ABD: move     b,x0                                        ; 21E400
000ABE: mpy      x0,x0,a #$40,r3                             ; 334080
000ABF: move     y:(r6+$15),y0                               ; 0256F6
000AC0: move     x:(r7-$84),b                                ; 0A77CF FFFF7C
000AC2: cmp      #<$4,b                                      ; 01448D
000AC3: bne      func_000ac5                                 ; 052402
000AC4: move     #$0,y0                                      ; 260000
000AC5: move     a,x1                                        ; 21C500
000AC6: move     y:(r1)+,x0                                  ; 4CD900
000AC7: do       #<$10,>$acf                                 ; 061080 000ACE
000AC9: mpy      y0,x0,a a,x:(r2)+ y:(r4)+,y1                ; F91AD0
000ACA: asl      #$1,a,a                                     ; 0C1D02
000ACB: mac      y1,x1,a y:(r1)+,x0                          ; 4CD9F2
000ACC: mpy      y0,x0,b b,x:(r3)+ y:(r4)+,y1                ; FD1BD8
000ACD: asl      #$1,b,b                                     ; 0C1D83
000ACE: mac      y1,x1,b y:(r1)+,x0                          ; 4CD9FA
000ACF: move     a,x:(r2)+                                   ; 565A00
000AD0: move     b,x:(r3)+                                   ; 575B00
000AD1: move     x:(r7-$8d),b                                ; 0A77CF FFFF73
000AD3: move     y:(r7-$8d),b0                               ; 0B77C9 FFFF73
000AD5: asr      #$2,b,b                                     ; 0C1C85
000AD6: move     #$1,a                                       ; 2E0100
000AD7: move     b0,y0                                       ; 212600
000AD8: andi     #$fe,ccr                                    ; 00FEB9
000AD9: do       #<$18,>$adc                                 ; 061880 000ADB
000ADB: div      y0,a                                        ; 018050
000ADC: move     a0,b                                        ; 210F00
000ADD: asr      #$11,b,b                                    ; 0C1CA3
000ADE: move     y:>$c4,x0                                   ; 4CF000 0000C4
000AE0: move     b1,y1                                       ; 21A700
000AE1: move     b0,y0                                       ; 212600
000AE2: mpyuu    y0,x0,a                                     ; 0127C5
000AE3: dmac     su y1,x0,a                                  ; 01258C
000AE4: asr      a                                           ; 200022
000AE5: clr      b               ifeq                        ; 202A1B
000AE6: sub      a,b                                         ; 20001C
000AE7: tfr      b,a                                         ; 200001
000AE8: cmp      #<$10,b                                     ; 01508D
000AE9: bge      func_000b1e                                 ; 051455
000AEA: move     #>$3fff,m2                                  ; 05F422 003FFF
000AEC: add      #>$40,a                                     ; 0140C0 000040
000AEE: move     #$90,r1                                     ; 319000
000AEF: tfr      y0,b x:>$c4,r2                              ; 62F059 0000C4
000AF1: asr      b #>$7fffff,x1                              ; 45F42A 7FFFFF
000AF3: move     b1,x0                                       ; 21A400
000AF4: move     a0,b                                        ; 210F00
000AF5: lsr      b a1,r0                                     ; 21902B
000AF6: do       #<$11,>$afb                                 ; 061180 000AFA
000AF8: add      y,a a,l:(r1)                                ; 486130
000AF9: add      x0,b b1,y:(r1)+                             ; 5D5948
000AFA: and      x1,b                                        ; 20006E
000AFB: move     y:(r7-$91),x0                               ; 0B77C4 FFFF6F
000AFD: move     x0,x:>$40                                   ; 447000 000040
000AFF: move     x:(r7-$91),x0                               ; 0A77C4 FFFF6F
000B01: move     x0,x:>$51                                   ; 447000 000051
000B03: move     x:>$50,x0                                   ; 44F000 000050
000B05: move     x0,y:(r7-$91)                               ; 0B7784 FFFF6F
000B07: move     x:>$61,x0                                   ; 44F000 000061
000B09: move     x0,x:(r7-$91)                               ; 0A7784 FFFF6F
000B0B: move     x:-(r2),a                                   ; 56FA00
000B0C: move     #$10,n0                                     ; 381000
000B0D: move     n0,n1                                       ; 231900
000B0E: move     y:>$90,y0                                   ; 4EF000 000090
000B10: move     x:(r0)+,x1                                  ; 45D800
000B11: move     #$91,r4                                     ; 349100
000B12: move     #$70,r3                                     ; 337000
000B13: jsr      func_000397                                 ; 0D0397
000B14: move     #$71,r0                                     ; 307100
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
000B4D: nop                                                  ; 000000
100000: move     r0,x:>$4ff                                  ; 607000 0004FF
100002: brclr    #$0,x:<<$ffffc3,func_100002                 ; 0CC300 000000
100004: move     x:<<$ffffc6,r0                              ; 60F000 FFFFC6
100006: bra      r0                                          ; 0D18C0
100169: nop                                                  ; 000000
10016A: rts                                                  ; 00000C
144CC9: move     #>$14a000,x0                                ; 44F400 14A000
144CCB: move     x0,x:(r6+$10)                               ; 024684
144CCC: move     #$0,x0                                      ; 240000
144CCD: move     x0,y:(r6+$10)                               ; 0246A4
144CCE: rts                                                  ; 00000C
144CCF: rts                                                  ; 00000C
144CD0: move     #>$17c6f9,x0                                ; 44F400 17C6F9
144CD2: move     a1,y1                                       ; 218700
144CD3: move     a0,y0                                       ; 210600
144CD4: mpysu    x0,y0,a                                     ; 01278D
144CD5: dmac     ss x0,y1,a                                  ; 012484
144CD6: move     x:(r6+$12),b                                ; 024E9F
144CD7: move     y:(r6+$12),b0                               ; 024EB9
144CD8: move     a1,x:(r6+$12)                               ; 024E8C
144CD9: move     a0,y:(r6+$12)                               ; 024EA8
144CDA: sub      b,a                                         ; 200014
144CDB: asr      #$4,a,a                                     ; 0C1C08
144CDC: move     #>$0,r1                                     ; 61F400 000000
144CDE: move     a1,y1                                       ; 218700
144CDF: move     a0,y0                                       ; 210600
144CE0: tfr      b,a                                         ; 200001
144CE1: move     x:(r6+$10),b                                ; 02469F
144CE2: move     y:(r6+$10),b0                               ; 0246B9
144CE3: move     #>$1fff,x0                                  ; 44F400 001FFF
144CE5: move     #>$14a000,x1                                ; 45F400 14A000
144CE7: do       #<$10,>$144ced                              ; 061080 144CEC
144CE9: add      y,a                                         ; 200030
144CEA: add      a,b             b,l:(r1)+                   ; 495918
144CEB: and      x0,b                                        ; 20004E
144CEC: add      x1,b                                        ; 200068
144CED: move     #>$0,r1                                     ; 61F400 000000
144CEF: move     r1,r4                                       ; 223400
144CF0: move     b1,x:(r6+$10)                               ; 02468D
144CF1: move     b0,y:(r6+$10)                               ; 0246A9
144CF2: move     x:(r1)+,r2                                  ; 62D900
144CF3: move     #>$1fff,m2                                  ; 05F422 001FFF
144CF5: move     m2,m0                                       ; 0460A2
144CF6: move     y:(r4)+,y0                                  ; 4EDC00
144CF7: move     x:(r2)+,x1                                  ; 45DA00
144CF8: move     x:(r1)+,r0                                  ; 60D900
144CF9: do       #<$8,>$144d0a                               ; 060880 144D09
144CFB: mpysu    -x1,y0,a                                    ; 012796
144CFC: add      x1,a            x:(r2),x0                   ; 44E260
144CFD: add      x1,a            x:(r1)+,r2                  ; 62D960
144CFE: macsu    x0,y0,a                                     ; 01268D
144CFF: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
144D00: asr      #$2,a,a                                     ; 0C1C04
144D01: mpysu    -x1,y0,b                                    ; 0127B6
144D02: add      x1,b            x:(r0),x0       a,y:(r7)+   ; B2E068
144D03: add      x1,b            x:(r1)+,r0                  ; 60D968
144D04: macsu    x0,y0,b                                     ; 0126AD
144D05: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
144D06: asr      #$2,b,b                                     ; 0C1C85
144D07: move     a,y:(r7)+                                   ; 5E5F00
144D08: move     b,y:(r7)+                                   ; 5F5F00
144D09: move     b,y:(r7)+                                   ; 5F5F00
144D0A: move     #>$ffffff,m0                                ; 05F420 FFFFFF
144D0C: move     #>$ffffff,m2                                ; 05F422 FFFFFF
144D0E: do       #<$10,>$144d20                              ; 061080 144D1F
144D10: do       #<$18,>$144d1f                              ; 061880 144D1E
144D12: nop                                                  ; 000000
144D13: nop                                                  ; 000000
144D14: nop                                                  ; 000000
144D15: nop                                                  ; 000000
144D16: nop                                                  ; 000000
144D17: nop                                                  ; 000000
144D18: nop                                                  ; 000000
144D19: nop                                                  ; 000000
144D1A: nop                                                  ; 000000
144D1B: nop                                                  ; 000000
144D1C: nop                                                  ; 000000
144D1D: nop                                                  ; 000000
144D1E: nop                                                  ; 000000
144D1F: nop                                                  ; 000000
144D20: rts                                                  ; 00000C
144D21: move     #>$1e,r0                                    ; 60F400 00001E
144D23: move     #>$20,r1                                    ; 61F400 000020
144D25: move     y:(r6+$14),x0                               ; 0256B4
144D26: move     x0,x:(r0)+                                  ; 445800
144D27: move     y:(r6+$15),x0                               ; 0256F4
144D28: move     x0,x:(r0)-                                  ; 445000
144D29: move     #>$1e454e,x1                                ; 45F400 1E454E
144D2B: move     #>$5b75b8,y0                                ; 46F400 5B75B8
144D2D: move     y:(r6+$1c),y1                               ; 0276B7
144D2E: move     y:(r6+$1d),b                                ; 0276FF
144D2F: move     #>$1e,r4                                    ; 64F400 00001E
144D31: move     x:(r1)+,x0                                  ; 44D900
144D32: move     x:(r0)+,a                                   ; 56D800
144D33: do       #<$10,>$144d39                              ; 061080 144D38
144D35: mac      y0,x0,a         x:(r1)+,x0      y1,y:(r4)+  ; B199D2
144D36: mac      -y1,y0,a        x:(r0)+,b       b,y1        ; 1F98B6
144D37: mac      x1,x0,b         x:(r1)+,x0      y1,y:(r4)+  ; B199AA
144D38: mac      -y1,x1,b        x:(r0)+,a       a,y1        ; 1998FE
144D39: move     y1,y:(r4)+                                  ; 4F5C00
144D3A: move     b,y:(r4)+                                   ; 5F5C00
144D3B: move     y1,y:(r6+$1c)                               ; 0276A7
144D3C: move     b,y:(r6+$1d)                                ; 0276EF
144D3D: move     #>$20,r4                                    ; 64F400 000020
144D3F: move     a,y:(r6+$14)                                ; 0256AE
144D40: move     x:(r0)+,x0                                  ; 44D800
144D41: move     x0,y:(r6+$15)                               ; 0256E4
144D42: move     #>$1f,r0                                    ; 60F400 00001F
144D44: move     #$40,x0                                     ; 244000
144D45: move     y:(r4)+,y0                                  ; 4EDC00
144D46: do       #<$8,>$144d4c                               ; 060880 144D4B
144D48: mpy      y0,x0,a         y:(r4)+,y0                  ; 4EDCD0
144D49: mac      y0,x0,a         b,x:(r0)+       y:(r4)+,y0  ; FC18D2
144D4A: mpy      y0,x0,b         y:(r4)+,y0                  ; 4EDCD8
144D4B: mac      y0,x0,b         a,x:(r0)+       y:(r4)+,y0  ; F818DA
144D4C: move     b,x:(r0)+                                   ; 575800
144D4D: rts                                                  ; 00000C
144D4E: move     #>$1e,r0                                    ; 60F400 00001E
144D50: move     #>$20,r1                                    ; 61F400 000020
144D52: move     y:(r6+$14),x0                               ; 0256B4
144D53: move     x0,x:(r0)+                                  ; 445800
144D54: move     y:(r6+$15),x0                               ; 0256F4
144D55: move     x0,x:(r0)-                                  ; 445000
144D56: move     #>$cfce3,x1                                 ; 45F400 0CFCE3
144D58: move     #>$2bc9ca,y0                                ; 46F400 2BC9CA
144D5A: move     y:(r6+$1c),y1                               ; 0276B7
144D5B: move     y:(r6+$1d),b                                ; 0276FF
144D5C: move     #>$1e,r4                                    ; 64F400 00001E
144D5E: move     x:(r1)+,x0                                  ; 44D900
144D5F: move     x:(r0)+,a                                   ; 56D800
144D60: do       #<$10,>$144d66                              ; 061080 144D65
144D62: mac      y0,x0,a         x:(r1)+,x0      y1,y:(r4)+  ; B199D2
144D63: mac      -y1,y0,a        x:(r0)+,b       b,y1        ; 1F98B6
144D64: mac      x1,x0,b         x:(r1)+,x0      y1,y:(r4)+  ; B199AA
144D65: mac      -y1,x1,b        x:(r0)+,a       a,y1        ; 1998FE
144D66: move     y1,y:(r4)+                                  ; 4F5C00
144D67: move     b,y:(r4)+                                   ; 5F5C00
144D68: move     y1,y:(r6+$1c)                               ; 0276A7
144D69: move     b,y:(r6+$1d)                                ; 0276EF
144D6A: move     a,y:(r6+$14)                                ; 0256AE
144D6B: move     x:(r0)+,x0                                  ; 44D800
144D6C: move     x0,y:(r6+$15)                               ; 0256E4
144D6D: move     #>$1e,r4                                    ; 64F400 00001E
144D6F: move     #>$20,r5                                    ; 65F400 000020
144D71: move     y:(r6+$16),x0                               ; 025EB4
144D72: move     x0,y:(r4)+                                  ; 4C5C00
144D73: move     y:(r6+$17),x0                               ; 025EF4
144D74: move     x0,y:(r4)-                                  ; 4C5400
144D75: move     #>$4e63df,y1                                ; 47F400 4E63DF
144D77: move     #>$6f0f12,x0                                ; 44F400 6F0F12
144D79: move     y:(r6+$1e),x1                               ; 027EB5
144D7A: move     y:(r6+$1f),b                                ; 027EFF
144D7B: move     #>$1e,r0                                    ; 60F400 00001E
144D7D: move     y:(r5)+,y0                                  ; 4EDD00
144D7E: move     y:(r4)+,a                                   ; 5EDC00
144D7F: do       #<$10,>$144d85                              ; 061080 144D84
144D81: mac      y0,x0,a         x1,x:(r0)+      y:(r5)+,y0  ; F438D2
144D82: mac      -x1,x0,a        b,x1            y:(r4)+,b   ; 1FDCA6
144D83: mac      y1,y0,b         x1,x:(r0)+      y:(r5)+,y0  ; F438BA
144D84: mac      -y1,x1,b        a,x1            y:(r4)+,a   ; 16DCFE
144D85: move     x1,x:(r0)+                                  ; 455800
144D86: move     b,x:(r0)+                                   ; 575800
144D87: move     x1,y:(r6+$1e)                               ; 027EA5
144D88: move     b,y:(r6+$1f)                                ; 027EEF
144D89: move     a,y:(r6+$16)                                ; 025EAE
144D8A: move     y:(r4)+,x0                                  ; 4CDC00
144D8B: move     x0,y:(r6+$17)                               ; 025EE4
144D8C: move     #>$20,r0                                    ; 60F400 000020
144D8E: move     #>$1f,r4                                    ; 64F400 00001F
144D90: move     #$40,y0                                     ; 264000
144D91: move     x:(r0)+,x0                                  ; 44D800
144D92: do       #<$8,>$144d98                               ; 060880 144D97
144D94: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
144D95: mac      y0,x0,a         x:(r0)+,x0      b,y:(r4)+   ; B398D2
144D96: mpy      y0,x0,b         x:(r0)+,x0                  ; 44D8D8
144D97: mac      y0,x0,b         x:(r0)+,x0      a,y:(r4)+   ; B298DA
144D98: move     b,y:(r4)+                                   ; 5F5C00
144D99: rts                                                  ; 00000C
144D9A: move     #>$a,x0                                     ; 44F400 00000A
144D9C: move     x0,y:(r6+$10)                               ; 0246A4
144D9D: move     #>$1,x0                                     ; 44F400 000001
144D9F: move     x0,y:(r6+$11)                               ; 0246E4
144DA0: rts                                                  ; 00000C
144DA1: rts                                                  ; 00000C
144DA2: move     #$0,x0                                      ; 240000
144DA3: move     x0,x:(r6-$19)                               ; 039EC4
144DA4: move     y:(r6+$10),b                                ; 0246BF
144DA5: move     y:(r6+$11),x1                               ; 0246F5
144DA6: move     #>$6,r1                                     ; 61F400 000006
144DA8: do       #<$10,>$144db2                              ; 061080 144DB1
144DAA: add      x1,b            x1,x0                       ; 20A468
144DAB: move     x0,a1                                       ; 208C00
144DAC: move     b1,x1                                       ; 21A500
144DAD: move     b1,x:(r1)+                                  ; 555900
144DAE: add      x1,a            x1,x0                       ; 20A460
144DAF: move     x0,b1                                       ; 208D00
144DB0: move     a1,x1                                       ; 218500
144DB1: move     a1,x:(r1)+                                  ; 545900
144DB2: move     x1,y:(r6+$10)                               ; 0246A5
144DB3: move     b1,y:(r6+$11)                               ; 0246ED
144DB4: move     #>$144e7a,r2                                ; 62F400 144E7A
144DB6: move     #>$0,r4                                     ; 64F400 000000
144DB8: move     #$6,m4                                      ; 0506A4
144DB9: move     #>$0,r0                                     ; 60F400 000000
144DBB: move     #>$26,r1                                    ; 61F400 000026
144DBD: move     x:(r2)+,a                                   ; 56DA00
144DBE: move     x:(r2)+,b                                   ; 57DA00
144DBF: move     x:(r2)+,a       a,y:(r4)+                   ; BA9A00
144DC0: move     x:(r2)+,b       b,y:(r4)+                   ; BF9A00
144DC1: move     x:(r2)+,a       a,y:(r4)+                   ; BA9A00
144DC2: move     x:(r2)+,b       b,y:(r4)+                   ; BF9A00
144DC3: move     x:(r2)+,a       a,y:(r4)+                   ; BA9A00
144DC4: move     b,y:(r4)+                                   ; 5F5C00
144DC5: move     a,y:(r4)+                                   ; 5E5C00
144DC6: move     x:(r6+$14),x1                               ; 025695
144DC7: move     y:(r6+$14),x0                               ; 0256B4
144DC8: move     x1,x:(r0)+                                  ; 455800
144DC9: move     x0,x:(r0)+                                  ; 445800
144DCA: move     x:(r6+$13),x1                               ; 024ED5
144DCB: move     y:(r6+$13),x0                               ; 024EF4
144DCC: move     x1,x:(r0)+                                  ; 455800
144DCD: move     x0,x:(r0)+                                  ; 445800
144DCE: move     x:(r6+$12),x1                               ; 024E95
144DCF: move     y:(r6+$12),x0                               ; 024EB4
144DD0: move     x1,x:(r0)+                                  ; 455800
144DD1: move     x0,x:(r0)+                                  ; 445800
144DD2: move     #>$0,r0                                     ; 60F400 000000
144DD4: move     #>$fffffb,n0                                ; 70F400 FFFFFB
144DD6: move     x:(r6+$17),x1                               ; 025ED5
144DD7: move     y:(r6+$17),x0                               ; 025EF4
144DD8: move     x1,x:(r1)+                                  ; 455900
144DD9: move     x0,x:(r1)+                                  ; 445900
144DDA: move     x:(r6+$16),x1                               ; 025E95
144DDB: move     y:(r6+$16),x0                               ; 025EB4
144DDC: move     x1,x:(r1)+                                  ; 455900
144DDD: move     x0,x:(r1)+                                  ; 445900
144DDE: move     x:(r6+$15),x1                               ; 0256D5
144DDF: move     y:(r6+$15),x0                               ; 0256F4
144DE0: move     x1,x:(r1)+                                  ; 455900
144DE1: move     x0,x:(r1)+                                  ; 445900
144DE2: move     #>$26,r1                                    ; 61F400 000026
144DE4: move     #>$fffffb,n1                                ; 71F400 FFFFFB
144DE6: move     x:(r0)+,x0      y:(r4)+,y0                  ; F09800
144DE7: do       #<$10,>$144dfb                              ; 061080 144DFA
144DE9: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
144DEA: mpy      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098D8
144DEB: mac      y0,x0,a         x:(r0)+,x0                  ; 44D8D2
144DEC: mac      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098DA
144DED: mac      y0,x0,a         x:(r0)+,x0                  ; 44D8D2
144DEE: mac      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098DA
144DEF: mac      y0,x0,a         x:(r0)+n0,x0                ; 44C8D2
144DF0: mac      y0,x0,b         x:(r1)+,x0      y:(r4)+,y0  ; F099DA
144DF1: mac      y0,x0,a         x:(r1)+,x0                  ; 44D9D2
144DF2: mac      y0,x0,b         x:(r1)+,x0      y:(r4)+,y0  ; F099DA
144DF3: mac      y0,x0,a         x:(r1)+,x0                  ; 44D9D2
144DF4: mac      y0,x0,b         x:(r1)+,x0      y:(r4)+,y0  ; F099DA
144DF5: mac      y0,x0,a         x:(r1)+,x0                  ; 44D9D2
144DF6: mac      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098DA
144DF7: asl      #$2,a,a                                     ; 0C1D04
144DF8: asl      #$2,b,b                                     ; 0C1D85
144DF9: move     a,x:(r1)+                                   ; 565900
144DFA: move     b,x:(r1)+n1                                 ; 574900
144DFB: move     x:(r0)+,x1                                  ; 45D800
144DFC: move     x0,x:(r6+$14)                               ; 025684
144DFD: move     x1,y:(r6+$14)                               ; 0256A5
144DFE: move     x:(r0)+,x1                                  ; 45D800
144DFF: move     x:(r0)+,x0                                  ; 44D800
144E00: move     x1,x:(r6+$13)                               ; 024EC5
144E01: move     x0,y:(r6+$13)                               ; 024EE4
144E02: move     x:(r0)+,x1                                  ; 45D800
144E03: move     x:(r0)+,x0                                  ; 44D800
144E04: move     x1,x:(r6+$12)                               ; 024E85
144E05: move     x0,y:(r6+$12)                               ; 024EA4
144E06: move     x:(r1)+,x1                                  ; 45D900
144E07: move     x:(r1)+,x0                                  ; 44D900
144E08: move     x1,x:(r6+$17)                               ; 025EC5
144E09: move     x0,y:(r6+$17)                               ; 025EE4
144E0A: move     x:(r1)+,x1                                  ; 45D900
144E0B: move     x:(r1)+,x0                                  ; 44D900
144E0C: move     x1,x:(r6+$16)                               ; 025E85
144E0D: move     x0,y:(r6+$16)                               ; 025EA4
144E0E: move     x:(r1)+,x1                                  ; 45D900
144E0F: move     x:(r1)+,x0                                  ; 44D900
144E10: move     x1,x:(r6+$15)                               ; 0256C5
144E11: move     x0,y:(r6+$15)                               ; 0256E4
144E12: move     #>$ffffff,m4                                ; 05F424 FFFFFF
144E14: move     #>$6,r0                                     ; 60F400 000006
144E16: move     #>$4c,r1                                    ; 61F400 00004C
144E18: move     x:(r6+$18),a                                ; 02669E
144E19: move     y:(r6+$18),b                                ; 0266BF
144E1A: move     #$2,x0                                      ; 240200
144E1B: move     #>$fffcb9,x1                                ; 45F400 FFFCB9
144E1D: move     x:(r0)+,y1                                  ; 47D800
144E1E: do       #<$10,>$144e24                              ; 061080 144E23
144E20: mac      x0,y1,a         a,x:(r1)+       a,y0        ; 1819C2
144E21: mac      x1,y0,a         x:(r0)+,y1                  ; 47D8E2
144E22: mac      x0,y1,b         b,x:(r1)+       b,y0        ; 1E19CA
144E23: mac      x1,y0,b         x:(r0)+,y1                  ; 47D8EA
144E24: move     a,x:(r6+$18)                                ; 02668E
144E25: move     b,y:(r6+$18)                                ; 0266AF
144E26: move     y:(r6+$5),b                                 ; 0216FF
144E27: move     #>$7fffff,a                                 ; 56F400 7FFFFF
144E29: sub      b,a                                         ; 200014
144E2A: sub      b,a                                         ; 200014
144E2B: bpl      func_144e38                                 ; 0D1043 00000D
144E2D: move     #$0,x1                                      ; 250000
144E2E: sub      #>$400000,b                                 ; 0140CC 400000
144E30: asl      b                                           ; 20003A
144E31: move     #>$7fffff,a                                 ; 56F400 7FFFFF
144E33: sub      b,a                                         ; 200014
144E34: move     b,y1                                        ; 21E700
144E35: move     a,y0                                        ; 21C600
144E36: bra      func_144e3d                                 ; 0D10C0 000007
144E38: asr      #$2,a,a                                     ; 0C1C04
144E39: asl      b                                           ; 20003A
144E3A: move     a,x1                                        ; 21C500
144E3B: move     b,y0                                        ; 21E600
144E3C: move     #$0,y1                                      ; 270000
144E3D: move     #>$6,r0                                     ; 60F400 000006
144E3F: move     #>$2c,r1                                    ; 61F400 00002C
144E41: move     #>$4c,r2                                    ; 62F400 00004C
144E43: move     #>$80,r3                                    ; 63F400 000080
144E45: move     x:(r0)+,x0                                  ; 44D800
144E46: do       #<$10,>$144e50                              ; 061080 144E4F
144E48: mpy      x1,x0,a x:(r0)+,x0                          ; 44D8A0
144E49: mpy      x1,x0,b         x:(r1)+,x0                  ; 44D9A8
144E4A: mac      y0,x0,a         x:(r1)+,x0                  ; 44D9D2
144E4B: mac      y0,x0,b         x:(r2)+,x0                  ; 44DADA
144E4C: mac      x0,y1,a         x:(r2)+,x0                  ; 44DAC2
144E4D: mac      x0,y1,b         x:(r0)+,x0                  ; 44D8CA
144E4E: move     a,x:(r3)+                                   ; 565B00
144E4F: move     b,x:(r3)+                                   ; 575B00
144E50: move     #>$80,r1                                    ; 61F400 000080
144E52: move     #$40,x0                                     ; 244000
144E53: move     y:(r6+$4),y0                                ; 0216B6
144E54: move     x0,a                                        ; 208E00
144E55: move     x0,b                                        ; 208F00
144E56: mac      y0,x0,a                                     ; 2000D2
144E57: mac      -y0,x0,b                                    ; 2000DE
144E58: move     a,y0                                        ; 21C600
144E59: move     x:(r1)+,x0      b,y1                        ; 139900
144E5A: move     y:(r6+$6),a                                 ; 021EBE
144E5B: cmp      #>$400000,a                                 ; 0140C5 400000
144E5D: blt      func_144e69                                 ; 0D1049 00000C
144E5F: do       #<$10,>$144e67                              ; 061080 144E66
144E61: mpy      y0,x0,a         x:(r1)+,x1                  ; 45D9D0
144E62: mac      y1,x1,a                                     ; 2000F2
144E63: mpy      x0,y1,b                                     ; 2000C8
144E64: mac      x1,y0,b         x:(r1)+,x0                  ; 44D9EA
144E65: move     a,y:(r7)+                                   ; 5E5F00
144E66: move     b,y:(r7)+                                   ; 5F5F00
144E67: bra      func_144e71                                 ; 0D10C0 00000A
144E69: do       #<$10,>$144e71                              ; 061080 144E70
144E6B: mpy      y0,x0,a         x:(r1)+,x1                  ; 45D9D0
144E6C: mac      y1,x1,a                                     ; 2000F2
144E6D: mpy      y0,x0,b                                     ; 2000D8
144E6E: mac      y1,x1,b         x:(r1)+,x0                  ; 44D9FA
144E6F: move     a,y:(r7)+                                   ; 5E5F00
144E70: move     b,y:(r7)+                                   ; 5F5F00
144E71: nop                                                  ; 000000
144E72: do       #<$78,>$144e75                              ; 067880 144E74
144E74: nop                                                  ; 000000
144E75: move     #>$ffffff,m0                                ; 05F420 FFFFFF
144E77: move     #>$ffffff,m2                                ; 05F422 FFFFFF
144E79: rts                                                  ; 00000C
144E7A: macr     -x0,x0,b        b,x:(r4)+       y:(r3)+,b   ; FF7C8F
144E7B: dc       $05e4f1                                     ; 05E4F1
144E7C: add      a,b             x:(r2)-,x1      y:(r6)+,y0  ; F4D218
144E7D: bses     func_144eb7                                 ; 05D05A
144E7E: dc       $10b5c7                                     ; 10B5C7
144E7F: macr     -x0,x0,b        b,x:(r2)-       b,y:(r7)+   ; BF728F
144E80: mac      -x1,x0,b        y:(r6)-,y1                  ; 4FD6AE
144E81: move     #>$114000,a0                                ; 50F400 114000
144E83: move     y:>$124,x1                                  ; 4DF000 000124
144E85: maci     #>$2000,x1,a                                ; 0141E2 002000
144E87: move     a0,x0                                       ; 210400
144E88: move     x0,y:(r6+$10)                               ; 0246A4
144E89: move     #$80,x0                                     ; 248000
144E8A: move     x0,x:(r6+$11)                               ; 0246C4
144E8B: move     #$0,x0                                      ; 240000
144E8C: move     x0,x:(r6+$12)                               ; 024E84
144E8D: move     y:(r6+$4),x0                                ; 0216B4
144E8E: mpyi     #>$fffc03,x0,a                              ; 0141C0 FFFC03
144E90: sub      #<$3,a                                      ; 014384
144E91: move     a,x:(r6+$1d)                                ; 0276CE
144E92: move     #$0,x0                                      ; 240000
144E93: move     x0,y:(r6+$21)                               ; 0286E4
144E94: move     x0,y:(r6+$20)                               ; 0286A4
144E95: move     x0,y:(r6+$1d)                               ; 0276E4
144E96: move     x0,x:(r6+$22)                               ; 028E84
144E97: move     x0,y:(r6+$22)                               ; 028EA4
144E98: move     x0,y:(r6+$13)                               ; 024EE4
144E99: rts                                                  ; 00000C
144E9A: move     #$0,x0                                      ; 240000
144E9B: move     x0,x:(r6-$19)                               ; 039EC4
144E9C: move     #$80,b                                      ; 2F8000
144E9D: move     y:(r6+$21),a                                ; 0286FE
144E9E: cmp      #>$80,a                                     ; 0140C5 000080
144EA0: bge      func_144ea3                                 ; 051403
144EA1: add      #<$1,a                                      ; 014180
144EA2: clr      b                                           ; 20001B
144EA3: move     a,y:(r6+$21)                                ; 0286EE
144EA4: move     b,y:(r6+$20)                                ; 0286AF
144EA5: move     #$0,r4                                      ; 340000
144EA6: move     y:(r6+$b),x0                                ; 022EF4
144EA7: mpy      x0,x0,a                                     ; 200080
144EA8: move     x:(r0)+,x1                                  ; 45D800
144EA9: move     a,x0                                        ; 21C400
144EAA: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
144EAB: asl      #$2,a,a                                     ; 0C1D04
144EAC: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
144EAD: asl      #$2,b,b                                     ; 0C1D85
144EAE: do       #<$10,>$144eb4                              ; 061080 144EB3
144EB0: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r4)+   ; B698A0
144EB1: asl      #$2,a,a                                     ; 0C1D04
144EB2: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r4)+   ; B798A8
144EB3: asl      #$2,b,b                                     ; 0C1D85
144EB4: move     y:(r6+$6),x0                                ; 021EB4
144EB5: mpy      x0,x0,a                                     ; 200080
144EB6: move     #>$20,r0                                    ; 60F400 000020
144EB8: move     a,x0                                        ; 21C400
144EB9: mpyi     #>$9566,x0,a                                ; 0141C0 009566
144EBB: move     r0,r4                                       ; 221400
144EBC: move     a,x0                                        ; 21C400
144EBD: move     x:(r6+$11),a                                ; 0246DE
144EBE: move     x:(r6+$12),b                                ; 024E9F
144EBF: move     a,x1                                        ; 21C500
144EC0: bset     #$14,sr                                     ; 0AF974
144EC1: do       #<$10,>$144ec5                              ; 061080 144EC4
144EC3: mac      -x1,x0,b        b,x:(r0)+       b,y0        ; 1E18AE
144EC4: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
144EC5: bclr     #$14,sr                                     ; 0AF954
144EC6: move     a,x:(r6+$11)                                ; 0246CE
144EC7: move     b,x:(r6+$12)                                ; 024E8F
144EC8: move     y:(r6+$24),a                                ; 0296BE
144EC9: move     y:(r6+$9),y0                                ; 0226F6
144ECA: move     a,y1                                        ; 21C700
144ECB: maci     #>$28f5c,y0,a                               ; 0141D2 028F5C
144ECD: maci     #>$fd70a4,y1,a                              ; 0141F2 FD70A4
144ECF: move     a,y:(r6+$24)                                ; 0296AE
144ED0: move     a,y0                                        ; 21C600
144ED1: mpyi     #>$800,y0,a                                 ; 0141D0 000800
144ED3: move     a,r0                                        ; 21D000
144ED4: move     x:(r0+$14a000),a                            ; 0A70CE 14A000
144ED6: move     x:(r0+$14a800),b                            ; 0A70CF 14A800
144ED8: move     y:(r6+$25),x0                               ; 0296F4
144ED9: move     y:(r6+$26),x1                               ; 029EB5
144EDA: move     b,y:(r6+$25)                                ; 0296EF
144EDB: move     a,y:(r6+$26)                                ; 029EAE
144EDC: sub      x1,a                                        ; 200064
144EDD: sub      x0,b                                        ; 20004C
144EDE: tfr      x1,a            a,y0                        ; 21C661
144EDF: tfr      x0,b            b,y1                        ; 21E749
144EE0: move     #$8,x0                                      ; 240800
144EE1: move     #>$e0,r0                                    ; 60F400 0000E0
144EE3: do       #<$10,>$144ee7                              ; 061080 144EE6
144EE5: mac      y0,x0,a         a,y:(r0)                    ; 5E60D2
144EE6: mac      x0,y1,b         b,x:(r0)+                   ; 5758CA
144EE7: move     #>$e0,r5                                    ; 65F400 0000E0
144EE9: move     #>$20,r0                                    ; 60F400 000020
144EEB: move     r0,r4                                       ; 221400
144EEC: move     #>$20,r1                                    ; 61F400 000020
144EEE: move     #>$30,r2                                    ; 62F400 000030
144EF0: move     x:(r5),y0                                   ; 46E500
144EF1: move     x:(r0)+,x0      y:(r5)+,y1                  ; F1B800
144EF2: do       #<$10,>$144efa                              ; 061080 144EF9
144EF4: mpy      y0,x0,a         y:(r4)+,x1                  ; 4DDCD0
144EF5: mac      y1,x1,a                                     ; 2000F2
144EF6: mpy      y0,x0,b         x:(r5),y0                   ; 46E5D8
144EF7: mac      -y1,x1,b        x:(r0)+,x0      y:(r5)+,y1  ; F1B8FE
144EF8: move     a,x:(r1)+                                   ; 565900
144EF9: move     b,x:(r2)+                                   ; 575A00
144EFA: move     y:(r6+$4),x0                                ; 0216B4
144EFB: mpyi     #>$fffc03,x0,a                              ; 0141C0 FFFC03
144EFD: sub      #<$3,a                                      ; 014384
144EFE: move     x:(r6+$1d),b                                ; 0276DF
144EFF: move     y:(r6+$1d),b0                               ; 0276F9
144F00: move     b,l:?:>$ff                                  ; 497000 0000FF
144F02: move     a,x0                                        ; 21C400
144F03: move     b,x1                                        ; 21E500
144F04: maci     #>$28f5c,x0,b                               ; 0141CA 028F5C
144F06: maci     #>$fd70a4,x1,b                              ; 0141EA FD70A4
144F08: move     b1,x:(r6+$1d)                               ; 0276CD
144F09: move     b0,y:(r6+$1d)                               ; 0276E9
144F0A: move     x:(r6+$22),a                                ; 028E9E
144F0B: move     y:(r6+$22),a0                               ; 028EB8
144F0C: move     y:(r6+$5),x0                                ; 0216F4
144F0D: move     a,x1                                        ; 21C500
144F0E: maci     #>$fd70a4,x0,a                              ; 0141C2 FD70A4
144F10: maci     #>$fd70a4,x1,a                              ; 0141E2 FD70A4
144F12: move     a1,x:(r6+$22)                               ; 028E8C
144F13: move     a0,y:(r6+$22)                               ; 028EA8
144F14: add      #<$2,b                                      ; 014288
144F15: asl      #$a,b,b                                     ; 0C1D95
144F16: move     a,x1                                        ; 21C500
144F17: move     b,x0                                        ; 21E400
144F18: mpy      x1,x0,a                                     ; 2000A0
144F19: move     y:(r6+$23),x0                               ; 028EF4
144F1A: move     a,y:(r6+$23)                                ; 028EEE
144F1B: sub      x0,a            #>$80000,y1                 ; 47F444 080000
144F1D: tfr      x0,a            a,y0                        ; 21C641
144F1E: move     #>$e0,r3                                    ; 63F400 0000E0
144F20: move     #$f,m3                                      ; 050FA3
144F21: do       #<$10,>$144f24                              ; 061080 144F23
144F23: mac      y1,y0,a         a,x:(r3)+                   ; 565BB2
144F24: move     #>$20,r1                                    ; 61F400 000020
144F26: move     #>$1f,r2                                    ; 62F400 00001F
144F28: move     x:(r6+$1d),b                                ; 0276DF
144F29: move     y:(r6+$1d),b0                               ; 0276F9
144F2A: move     l:?:>$ff,y                                  ; 43F000 0000FF
144F2C: sub      y,b                                         ; 20003C
144F2D: add      #<$10,b                                     ; 015088
144F2E: asr      #$4,b,b                                     ; 0C1C89
144F2F: move     b1,y1                                       ; 21A700
144F30: move     b0,y0                                       ; 212600
144F31: move     l:(r2),a                                    ; 48E200
144F32: move     x:(r1)+,x1                                  ; 45D900
144F33: move     x:(r3)+,x0                                  ; 44DB00
144F34: do       #<$2,>$144f3f                               ; 060280 144F3E
144F36: move     l:?:>$ff,b                                  ; 49F000 0000FF
144F38: do       #<$10,>$144f3e                              ; 061080 144F3D
144F3A: mpy      x1,x0,a         a,l:(r2)+                   ; 485AA0
144F3B: asr      #$a,a,a                                     ; 0C1C14
144F3C: add      b,a             x:(r1)+,x1                  ; 45D910
144F3D: add      y,b             x:(r3)+,x0                  ; 44DB38
144F3E: nop                                                  ; 000000
144F3F: move     a,l:(r2)+                                   ; 485A00
144F40: move     #>$ffffff,m3                                ; 05F423 FFFFFF
144F42: clr      a                                           ; 200013
144F43: clr      b               #>$20,r4                    ; 64F41B 000020
144F45: do       #<$10,>$144f4c                              ; 061080 144F4B
144F47: move     y:(r4)+,a1                                  ; 5CDC00
144F48: lsr      a               y:(r4)-,b1                  ; 5DD423
144F49: lsr      b                                           ; 20002B
144F4A: move     a1,y:(r4)+                                  ; 5C5C00
144F4B: move     b1,y:(r4)+                                  ; 5D5C00
144F4C: move     #>$20,r4                                    ; 64F400 000020
144F4E: move     #>$9e,r0                                    ; 60F400 00009E
144F50: move     y:(r4)+,y0                                  ; 4EDC00
144F51: move     x:(r0)+,a                                   ; 56D800
144F52: move     x:(r0)-,b                                   ; 57D000
144F53: do       #<$10,>$144f57                              ; 061080 144F56
144F55: mpy      y0,y0,a         a,x:(r0)+       y:(r4)+,y0  ; F81890
144F56: mpy      y0,y0,b         b,x:(r0)+       y:(r4)+,y0  ; FC1898
144F57: move     a,x:(r0)+                                   ; 565800
144F58: move     b,x:(r0)+                                   ; 575800
144F59: move     #>$a0,r0                                    ; 60F400 0000A0
144F5B: move     #>$20,r4                                    ; 64F400 000020
144F5D: move     #>$a0,r5                                    ; 65F400 0000A0
144F5F: move     x:(r0)+,x0      y:(r4)+,y0                  ; F09800
144F60: do       #<$10,>$144f66                              ; 061080 144F65
144F62: mpy      y0,x0,a         x:(r0)+,x0      y:(r4)+,y0  ; F098D0
144F63: mpy      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098D8
144F64: move     a,y:(r5)+                                   ; 5E5D00
144F65: move     b,y:(r5)+                                   ; 5F5D00
144F66: move     #>$20,r4                                    ; 64F400 000020
144F68: move     #>$40,r0                                    ; 60F400 000040
144F6A: move     #>$a0,r1                                    ; 61F400 0000A0
144F6C: move     #>$a0,r5                                    ; 65F400 0000A0
144F6E: move     #$80,y1                                     ; 278000
144F6F: move     #$0,x0                                      ; 240000
144F70: move     #>$0,r2                                     ; 62F400 000000
144F72: move     #$a,m2                                      ; 050AA2
144F73: move     #>$eaaaab,x0                                ; 44F400 EAAAAB
144F75: move     x0,x:(r2)+                                  ; 445A00
144F76: move     #$40,x0                                     ; 244000
144F77: move     x0,x:(r2)+                                  ; 445A00
144F78: move     #>$d55555,x0                                ; 44F400 D55555
144F7A: move     x0,x:(r2)+                                  ; 445A00
144F7B: move     #$40,x0                                     ; 244000
144F7C: move     x0,x:(r2)+                                  ; 445A00
144F7D: move     #$80,x0                                     ; 248000
144F7E: move     x0,x:(r2)+                                  ; 445A00
144F7F: move     #$c0,x0                                     ; 24C000
144F80: move     x0,x:(r2)+                                  ; 445A00
144F81: move     #$c0,x0                                     ; 24C000
144F82: move     x0,x:(r2)+                                  ; 445A00
144F83: move     #$40,x0                                     ; 244000
144F84: move     x0,x:(r2)+                                  ; 445A00
144F85: move     #>$7fffff,x0                                ; 44F400 7FFFFF
144F87: move     x0,x:(r2)+                                  ; 445A00
144F88: move     #>$155555,x0                                ; 44F400 155555
144F8A: move     x0,x:(r2)+                                  ; 445A00
144F8B: move     #>$eaaaab,x0                                ; 44F400 EAAAAB
144F8D: move     x0,x:(r2)+                                  ; 445A00
144F8E: move     x:(r2)+,x0      y:(r5),y0                   ; C0BA00
144F8F: do       #<$20,>$144fa2                              ; 062080 144FA1
144F91: mpy      y0,x0,a         x:(r2)+,x0                  ; 44DAD0
144F92: move     x:(r1)+,x1                                  ; 45D900
144F93: mac      x1,x0,a         x:(r2)+,x0      y:(r4),y0   ; C09AA2
144F94: mac      y0,x0,a         x:(r2)+,x0      y:(r5),y0   ; C0BAD2
144F95: mpy      y0,x0,b x:(r2)+,x0                          ; 44DAD8
144F96: mac      x1,x0,b         x:(r2)+,x0      y:(r4),y0   ; C09AAA
144F97: mac      y0,x0,b                                     ; 2000DA
144F98: sub      y1,b            x:(r2)+,x0      y:(r5),y0   ; C0BA7C
144F99: move     a,x:(r0)+                                   ; 565800
144F9A: move     b,x:(r0)+                                   ; 575800
144F9B: mpy      y0,x0,a         x:(r2)+,x0                  ; 44DAD0
144F9C: mac      x1,x0,a         x:(r2)+,x0      y:(r4),y0   ; C09AA2
144F9D: mac      y0,x0,a         x:(r2)+,x0      y:(r5)+,y0  ; F0BAD2
144F9E: mpy      y0,x0,b         x:(r2)+,x0      y:(r4)+,y0  ; F09AD8
144F9F: mac      y0,x0,b         x:(r2)+,x0      y:(r5),y0   ; C0BADA
144FA0: move     a,x:(r0)+                                   ; 565800
144FA1: move     b,x:(r0)+                                   ; 575800
144FA2: move     #>$ffffff,m2                                ; 05F422 FFFFFF
144FA4: move     #>$0,r4                                     ; 64F400 000000
144FA6: move     #$2,n4                                      ; 3C0200
144FA7: move     y:(r6+$10),a                                ; 0246BE
144FA8: tfr      a,b             a,r0                        ; 21D009
144FA9: add      #>$1000,a                                   ; 0140C0 001000
144FAB: move     #>$fff,m0                                   ; 05F420 000FFF
144FAD: move     m0,m1                                       ; 0461A0
144FAE: move     m0,m2                                       ; 0462A0
144FAF: move     m0,m3                                       ; 0463A0
144FB0: move     a,r1                                        ; 21D100
144FB1: move     r0,r2                                       ; 221200
144FB2: move     r1,r3                                       ; 223300
144FB3: move     x:(r6+$1d),n2                               ; 0A76DA 00001D
144FB5: move     n2,n3                                       ; 235B00
144FB6: move     (r2)+n2                                     ; 204A00
144FB7: move     (r3)+n3                                     ; 204B00
144FB8: clr      a                                           ; 200013
144FB9: move     y:(r6+$1d),a1                               ; 0276FC
144FBA: lsr      a                                           ; 200023
144FBB: move     #>$7fffff,b                                 ; 57F400 7FFFFF
144FBD: sub      a,b             a,x0                        ; 21C41C
144FBE: move     y:(r6+$8),a                                 ; 0226BE
144FBF: sub      #>$400000,a                                 ; 0140C4 400000
144FC1: move     a,x1                                        ; 21C500
144FC2: mpyi     #>$410410,x1,a                              ; 0141E0 410410
144FC4: asl      #$2,a,a                                     ; 0C1D04
144FC5: move     a,x1                                        ; 21C500
144FC6: mpyi     #>$7eb852,x1,a                              ; 0141E0 7EB852
144FC8: move     a,x1                                        ; 21C500
144FC9: mpy      x1,x0,b         b,x0                        ; 21E4A8
144FCA: mpy      x1,x0,a                                     ; 2000A0
144FCB: move     b,y0                                        ; 21E600
144FCC: move     a,y1                                        ; 21C700
144FCD: move     y:(r6+$20),x0                               ; 0286B4
144FCE: mpy      -y0,x0,a                                    ; 2000D4
144FCF: mpy      -x0,y1,b        #>$7fffff,x1                ; 45F4CC 7FFFFF
144FD1: move     a,y0                                        ; 21C600
144FD2: move     b,y1                                        ; 21E700
144FD3: do       #<$2,>$144fe3                               ; 060280 144FE2
144FD5: nop                                                  ; 000000
144FD6: do       #<$10,>$144fdd                              ; 061080 144FDC
144FD8: move     x:(r2)+,x0                                  ; 44DA00
144FD9: mpy      x0,y1,a         x:(r2),x0                   ; 44E2C0
144FDA: mac      y0,x0,a         y:(r4)+n4,x0                ; 4CCCD2
144FDB: mac      x1,x0,a                                     ; 2000A2
144FDC: move     a,x:(r0)+                                   ; 565800
144FDD: move     r5,b                                        ; 22AF00
144FDE: move     r0,r5                                       ; 221500
144FDF: move     r1,r0                                       ; 223000
144FE0: move     r3,r2                                       ; 227200
144FE1: move     #>$1,r4                                     ; 64F400 000001
144FE3: move     #>$ffffff,m0                                ; 05F420 FFFFFF
144FE5: move     m0,m1                                       ; 0461A0
144FE6: move     m0,m2                                       ; 0462A0
144FE7: move     m0,m3                                       ; 0463A0
144FE8: move     y:(r6+$10),a                                ; 0246BE
144FE9: move     b,y:(r6+$10)                                ; 0246AF
144FEA: move     #>$fff,m0                                   ; 05F420 000FFF
144FEC: move     m0,m4                                       ; 0464A0
144FED: move     a,r0                                        ; 21D000
144FEE: add      #>$1000,a                                   ; 0140C0 001000
144FF0: move     a,r1                                        ; 21D100
144FF1: move     #>$20,r5                                    ; 65F400 000020
144FF3: move     #>$40,r2                                    ; 62F400 000040
144FF5: move     #>$e0,r3                                    ; 63F400 0000E0
144FF7: move     x:(r5)+,n0                                  ; 70DD00
144FF8: do       #<$2,>$145006                               ; 060280 145005
144FFA: lua      (r0)+n0,r4                                  ; 044814
144FFB: do       #<$10,>$145005                              ; 061080 145004
144FFD: move     x:(r2)+,x0      y:(r4)+,y0                  ; F09A00
144FFE: move     x:(r5)+,n0                                  ; 70DD00
144FFF: mpy      y0,x0,a         x:(r2)+,x0      y:(r4)+,y0  ; F09AD0
145000: mac      y0,x0,a         x:(r2)+,x0      y:(r4)+,y0  ; F09AD2
145001: mac      y0,x0,a         x:(r2)+,x0      y:(r4)+,y0  ; F09AD2
145002: mac      y0,x0,a                                     ; 2000D2
145003: lua      (r0)+n0,r4                                  ; 044814
145004: move     a,x:(r3)+                                   ; 565B00
145005: move     r1,r0                                       ; 223000
145006: move     #>$ffffff,m4                                ; 05F424 FFFFFF
145008: move     #>$ffffff,m0                                ; 05F420 FFFFFF
14500A: move     #>$3e,r4                                    ; 64F400 00003E
14500C: move     #>$0,r5                                     ; 65F400 000000
14500E: move     #>$e0,r0                                    ; 60F400 0000E0
145010: move     #$1f,m0                                     ; 051FA0
145011: move     #$10,n0                                     ; 381000
145012: move     #>$5a8241,y1                                ; 47F400 5A8241
145014: move     y:(r6+$20),x0                               ; 0286B4
145015: mpy      -x0,y1,a                                    ; 2000C4
145016: move     y:(r5)+,x1                                  ; 4DDD00
145017: move     a,y0                                        ; 21C600
145018: do       #<$10,>$14501f                              ; 061080 14501E
14501A: mpy      x1,y0,a         x:(r0)+n0,x1    a,y:(r4)+   ; B688E0
14501B: mac      y1,x1,a         y:(r5)+,x1                  ; 4DDDF2
14501C: mpy      x1,y0,b         x:(r0)+n0,x1    b,y:(r4)+   ; B788E8
14501D: mac      y1,x1,b         y:(r5)+,x1                  ; 4DDDFA
14501E: move     (r0)+                                       ; 205800
14501F: move     a,y:(r4)+                                   ; 5E5C00
145020: move     b,y:(r4)+                                   ; 5F5C00
145021: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145023: move     #>$0,r4                                     ; 64F400 000000
145025: move     #>$40,r5                                    ; 65F400 000040
145027: move     y:(r6+$7),x0                                ; 021EF4
145028: move     #>$7fffff,a                                 ; 56F400 7FFFFF
14502A: sub      x0,a                                        ; 200044
14502B: move     y:(r4)+,y0                                  ; 4EDC00
14502C: move     a,x1                                        ; 21C500
14502D: do       #<$10,>$145035                              ; 061080 145034
14502F: mpy      x1,y0,a         y:(r5)+,y0                  ; 4EDDE0
145030: mac      y0,x0,a         y:(r4)+,y0                  ; 4EDCD2
145031: mpy      x1,y0,b         y:(r5)+,y0                  ; 4EDDE8
145032: mac      y0,x0,b         y:(r4)+,y0                  ; 4EDCDA
145033: move     a,y:(r7)+                                   ; 5E5F00
145034: move     b,y:(r7)+                                   ; 5F5F00
145035: rts                                                  ; 00000C
145036: move     #$80,x0                                     ; 248000
145037: move     x0,x:(r6+$11)                               ; 0246C4
145038: move     #$0,x0                                      ; 240000
145039: move     x0,x:(r6+$12)                               ; 024E84
14503A: move     x0,x1                                       ; 208500
14503B: move     x0,x:(r6+$19)                               ; 0266C4
14503C: move     x0,y:(r6+$19)                               ; 0266E4
14503D: move     x0,x:(r6+$18)                               ; 026684
14503E: move     x0,y:(r6+$18)                               ; 0266A4
14503F: lua      (r6+$1e),r4                                 ; 040EE4
145040: do       #<$8,>$145044                               ; 060880 145043
145042: move     x,l:(r4)+                                   ; 425C00
145043: move     x,l:(r4)+                                   ; 425C00
145044: rts                                                  ; 00000C
145045: move     #$0,x0                                      ; 240000
145046: move     x0,x:(r6-$19)                               ; 039EC4
145047: move     #$0,r4                                      ; 340000
145048: move     #$10,r5                                     ; 351000
145049: move     y:(r6+$b),x0                                ; 022EF4
14504A: mpy      x0,x0,a                                     ; 200080
14504B: move     x:(r0)+,x1                                  ; 45D800
14504C: move     a,x0                                        ; 21C400
14504D: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
14504E: asl      #$2,a,a                                     ; 0C1D04
14504F: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
145050: asl      #$2,b,b                                     ; 0C1D85
145051: do       #<$10,>$145057                              ; 061080 145056
145053: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r4)+   ; B698A0
145054: asl      #$2,a,a                                     ; 0C1D04
145055: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r5)+   ; B7B8A8
145056: asl      #$2,b,b                                     ; 0C1D85
145057: move     y:(r6+$6),x0                                ; 021EB4
145058: mpy      x0,x0,a                                     ; 200080
145059: move     #$20,r0                                     ; 302000
14505A: move     a,x0                                        ; 21C400
14505B: mpyi     #>$9566,x0,a                                ; 0141C0 009566
14505D: move     r0,r4                                       ; 221400
14505E: move     a,x0                                        ; 21C400
14505F: move     x:(r6+$11),a                                ; 0246DE
145060: move     x:(r6+$12),b                                ; 024E9F
145061: move     a,x1                                        ; 21C500
145062: bset     #$14,sr                                     ; 0AF974
145063: do       #<$10,>$145067                              ; 061080 145066
145065: mac      -x1,x0,b        b,x:(r0)+       b,y0        ; 1E18AE
145066: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145067: bclr     #$14,sr                                     ; 0AF954
145068: move     a,x:(r6+$11)                                ; 0246CE
145069: move     b,x:(r6+$12)                                ; 024E8F
14506A: move     y:(r6+$1b),a                                ; 026EFE
14506B: move     y:(r6+$9),y0                                ; 0226F6
14506C: move     a,y1                                        ; 21C700
14506D: maci     #>$28f5c,y0,a                               ; 0141D2 028F5C
14506F: maci     #>$fd70a4,y1,a                              ; 0141F2 FD70A4
145071: move     a,y:(r6+$1b)                                ; 026EEE
145072: move     a,y0                                        ; 21C600
145073: mpyi     #>$800,y0,a                                 ; 0141D0 000800
145075: move     a,r0                                        ; 21D000
145076: move     x:(r0+$14a000),a                            ; 0A70CE 14A000
145078: move     x:(r0+$14a800),b                            ; 0A70CF 14A800
14507A: move     y:(r6+$1c),x0                               ; 0276B4
14507B: move     y:(r6+$1d),x1                               ; 0276F5
14507C: move     b,y:(r6+$1c)                                ; 0276AF
14507D: move     a,y:(r6+$1d)                                ; 0276EE
14507E: sub      x1,a                                        ; 200064
14507F: sub      x0,b                                        ; 20004C
145080: tfr      x1,a            a,y0                        ; 21C661
145081: tfr      x0,b            b,y1                        ; 21E749
145082: move     #$8,x0                                      ; 240800
145083: move     #>$e0,r0                                    ; 60F400 0000E0
145085: do       #<$10,>$145089                              ; 061080 145088
145087: mac      y0,x0,a         a,y:(r0)                    ; 5E60D2
145088: mac      x0,y1,b         b,x:(r0)+                   ; 5758CA
145089: move     #>$e0,r5                                    ; 65F400 0000E0
14508B: move     #>$20,r0                                    ; 60F400 000020
14508D: move     r0,r4                                       ; 221400
14508E: move     #>$20,r1                                    ; 61F400 000020
145090: move     #>$30,r2                                    ; 62F400 000030
145092: move     x:(r5),y0                                   ; 46E500
145093: move     x:(r0)+,x0      y:(r5)+,y1                  ; F1B800
145094: do       #<$10,>$14509c                              ; 061080 14509B
145096: mpy      y0,x0,a         y:(r4)+,x1                  ; 4DDCD0
145097: mac      y1,x1,a                                     ; 2000F2
145098: mpy      y0,x0,b         x:(r5),y0                   ; 46E5D8
145099: mac      -y1,x1,b        x:(r0)+,x0      y:(r5)+,y1  ; F1B8FE
14509A: move     a,x:(r1)+                                   ; 565900
14509B: move     b,x:(r2)+                                   ; 575A00
14509C: move     x:(r6+$18),b                                ; 02669F
14509D: move     y:(r6+$18),b0                               ; 0266B9
14509E: move     y:(r6+$4),x0                                ; 0216B4
14509F: move     b,x1                                        ; 21E500
1450A0: maci     #>$28f5c,x0,b                               ; 0141CA 028F5C
1450A2: maci     #>$fd70a4,x1,b                              ; 0141EA FD70A4
1450A4: move     b1,x:(r6+$18)                               ; 02668D
1450A5: move     b0,y:(r6+$18)                               ; 0266A9
1450A6: move     x:(r6+$19),a                                ; 0266DE
1450A7: move     y:(r6+$19),a0                               ; 0266F8
1450A8: move     y:(r6+$5),x0                                ; 0216F4
1450A9: move     a,x1                                        ; 21C500
1450AA: maci     #>$28f5c,x0,a                               ; 0141C2 028F5C
1450AC: maci     #>$fd70a4,x1,a                              ; 0141E2 FD70A4
1450AE: move     a1,x:(r6+$19)                               ; 0266CC
1450AF: move     a0,y:(r6+$19)                               ; 0266E8
1450B0: move     a,x0                                        ; 21C400
1450B1: sub      #>$400000,b                                 ; 0140CC 400000
1450B3: asl      b                                           ; 20003A
1450B4: move     b,x1                                        ; 21E500
1450B5: abs      b                                           ; 20002E
1450B6: add      #>$800000,b                                 ; 0140C8 800000
1450B8: neg      b                                           ; 20003E
1450B9: move     b,y1                                        ; 21E700
1450BA: mpy      x0,y1,a                                     ; 2000C0
1450BB: move     y:(r6+$1a),x0                               ; 026EB4
1450BC: move     a,y:(r6+$1a)                                ; 026EAE
1450BD: sub      x0,a            #>$80000,y1                 ; 47F444 080000
1450BF: tfr      x0,a            a,y0                        ; 21C641
1450C0: move     #>$e0,r3                                    ; 63F400 0000E0
1450C2: move     #$f,m3                                      ; 050FA3
1450C3: do       #<$10,>$1450c6                              ; 061080 1450C5
1450C5: mac      y1,y0,a         a,x:(r3)+                   ; 565BB2
1450C6: move     #>$20,r1                                    ; 61F400 000020
1450C8: move     #>$0,r2                                     ; 62F400 000000
1450CA: move     x1,y0                                       ; 20A600
1450CB: move     x:(r1)+,x1                                  ; 45D900
1450CC: move     x:(r3)+,x0                                  ; 44DB00
1450CD: do       #<$10,>$1450d5                              ; 061080 1450D4
1450CF: mpy      x1,x0,a         x:(r1)+,x1                  ; 45D9A0
1450D0: add      y0,a            x:(r3)+,x0                  ; 44DB50
1450D1: mpy      x1,x0,b         x:(r1)+,x1                  ; 45D9A8
1450D2: add      y0,b            x:(r3)+,x0                  ; 44DB58
1450D3: move     a,x:(r2)+                                   ; 565A00
1450D4: move     b,x:(r2)+                                   ; 575A00
1450D5: move     #>$ffffff,m3                                ; 05F423 FFFFFF
1450D7: move     #>$90,r4                                    ; 64F400 000090
1450D9: move     y:(r6+$8),a                                 ; 0226BE
1450DA: sub      #>$400000,a                                 ; 0140C4 400000
1450DC: asl      a                                           ; 200032
1450DD: move     a,y1                                        ; 21C700
1450DE: move     y:(r6+$13),b                                ; 024EFF
1450DF: move     a,y:(r6+$13)                                ; 024EEE
1450E0: sub      b,a                                         ; 200014
1450E1: move     a,y0                                        ; 21C600
1450E2: mpyi     #>$80000,y0,a                               ; 0141D0 080000
1450E4: do       #<$10,>$1450e7                              ; 061080 1450E6
1450E6: add      a,b             b,y:(r4)+                   ; 5F5C18
1450E7: move     #$1e,n6                                     ; 3E1E00
1450E8: move     #>$0,r0                                     ; 60F400 000000
1450EA: move     #>$0,r5                                     ; 65F400 000000
1450EC: move     #>$60,r3                                    ; 63F400 000060
1450EE: do       #<$2,>$145129                               ; 060280 145128
1450F0: lua      (r6)+n6,r4                                  ; 044E14
1450F1: move     #>$20,r2                                    ; 62F400 000020
1450F3: do       #<$7,>$1450f7                               ; 060780 1450F6
1450F5: move     l:(r4)+,x                                   ; 42DC00
1450F6: move     x,l:(r2)+                                   ; 425A00
1450F7: move     #>$21,r1                                    ; 61F400 000021
1450F9: move     #>$20,r2                                    ; 62F400 000020
1450FB: move     #>$90,r4                                    ; 64F400 000090
1450FD: move     #>$fffffb,n2                                ; 72F400 FFFFFB
1450FF: move     n2,n1                                       ; 235900
145100: move     x:(r0)+,x0      y:(r4)+,y1                  ; F19800
145101: do       #<$10,>$14511d                              ; 061080 14511C
145103: mpy      y1,x1,b         y:(r5),y0                   ; 4EE5F8
145104: add      y0,b            x:(r2)+,y0                  ; 46DA58
145105: tfr      y0,a            b,x1                        ; 21E551
145106: mac      x1,x0,a         x:(r2)+,x1                  ; 45DAA2
145107: mac      -x1,x0,a        b,x:(r1)+                   ; 5759A6
145108: tfr      x1,b            x:(r2)+,x1                  ; 45DA69
145109: mac      -x1,x0,b        a,y0                        ; 21C6AE
14510A: mac      y0,x0,b         a,x:(r1)+                   ; 5659DA
14510B: tfr      x1,a            x:(r2)+,x1                  ; 45DA61
14510C: mac      -x1,x0,a        b,y0                        ; 21E6A6
14510D: mac      y0,x0,a         b,x:(r1)+                   ; 5759D2
14510E: tfr      x1,b            x:(r2)+,x1                  ; 45DA69
14510F: mac      -x1,x0,b        a,y0                        ; 21C6AE
145110: mac      y0,x0,b         a,x:(r1)+                   ; 5659DA
145111: tfr      x1,a            x:(r2)+,x1                  ; 45DA61
145112: mac      -x1,x0,a        b,y0                        ; 21E6A6
145113: mac      y0,x0,a         b,x:(r1)+                   ; 5759D2
145114: tfr      x1,b            x:(r2)+n2,x1                ; 45CA69
145115: mac      -x1,x0,b        a,y0                        ; 21C6AE
145116: mac      y0,x0,b         a,x:(r1)+                   ; 5659DA
145117: move     #>$5a8241,y1                                ; 47F400 5A8241
145119: move     x:(r0)+,x0      y:(r5)+,y0                  ; F0B800
14511A: mpy      y1,y0,a         b,x1                        ; 21E5B0
14511B: mac      y1,x1,a         b,x:(r1)+n1     y:(r4)+,y1  ; FD09F2
14511C: move     a,x:(r3)+                                   ; 565B00
14511D: lua      (r6)+n6,r4                                  ; 044E14
14511E: do       #<$7,>$145122                               ; 060780 145121
145120: move     l:(r2)+,y                                   ; 43DA00
145121: move     y,l:(r4)+                                   ; 435C00
145122: move     #$26,n6                                     ; 3E2600
145123: move     #>$10,r0                                    ; 60F400 000010
145125: move     #>$10,r5                                    ; 65F400 000010
145127: move     #>$70,r3                                    ; 63F400 000070
145129: move     #>$0,r4                                     ; 64F400 000000
14512B: move     #>$10,r3                                    ; 63F400 000010
14512D: move     #>$60,r2                                    ; 62F400 000060
14512F: move     #>$70,r1                                    ; 61F400 000070
145131: move     y:(r6+$7),x0                                ; 021EF4
145132: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145134: sub      x0,a                                        ; 200044
145135: move     y:(r4)+,y0                                  ; 4EDC00
145136: move     a,x1                                        ; 21C500
145137: do       #<$10,>$14513f                              ; 061080 14513E
145139: mpy      x1,y0,a         x:(r2)+,y0                  ; 46DAE0
14513A: mac      y0,x0,a         y:(r3)+,y0                  ; 4EDBD2
14513B: mpy      x1,y0,b         x:(r1)+,y0                  ; 46D9E8
14513C: mac      y0,x0,b         y:(r4)+,y0                  ; 4EDCDA
14513D: move     a,y:(r7)+                                   ; 5E5F00
14513E: move     b,y:(r7)+                                   ; 5F5F00
14513F: rts                                                  ; 00000C
145140: rts                                                  ; 00000C
145141: move     #$0,x0                                      ; 240000
145142: move     x0,y:(r6+$11)                               ; 0246E4
145143: rts                                                  ; 00000C
145144: do       #<$10,>$14514c                              ; 061080 14514B
145146: move     x:(r0)+,a                                   ; 56D800
145147: move     x:(r0)+,b                                   ; 57D800
145148: asr      #$2,a,a                                     ; 0C1C04
145149: asr      #$2,b,b                                     ; 0C1C85
14514A: move     a,y:(r7)+                                   ; 5E5F00
14514B: move     b,y:(r7)+                                   ; 5F5F00
14514C: rts                                                  ; 00000C
14514D: move     r7,x:>$ff                                   ; 677000 0000FF
14514F: move     y:(r6+$11),a                                ; 0246FE
145150: cmp      #<$0,a                                      ; 014085
145151: bne      func_145166                                 ; 0D1042 000015
145153: move     y:(r6+$8),a                                 ; 0226BE
145154: asr      #$10,a,a                                    ; 0C1C20
145155: move     y:(r6+$12),b                                ; 024EBF
145156: move     a,r4                                        ; 21D400
145157: nop                                                  ; 000000
145158: nop                                                  ; 000000
145159: nop                                                  ; 000000
14515A: move     y:(r4+$141980),y0                           ; 0B74C6 141980
14515C: add      y0,b                                        ; 200058
14515D: bec      func_145163                                 ; 0D1045 000006
14515F: move     #>$1,x0                                     ; 44F400 000001
145161: move     x0,y:(r6+$11)                               ; 0246E4
145162: move     x0,y:(r6+$10)                               ; 0246A4
145163: move     b,y:(r6+$12)                                ; 024EAF
145164: bra      func_14518d                                 ; 0D10C0 000029
145166: cmp      #>$1,a                                      ; 0140C5 000001
145168: bne      func_145180                                 ; 0D1042 000018
14516A: move     y:(r6+$9),a                                 ; 0226FE
14516B: asr      #$10,a,a                                    ; 0C1C20
14516C: move     y:(r6+$10),b                                ; 0246BF
14516D: move     a,r4                                        ; 21D400
14516E: nop                                                  ; 000000
14516F: nop                                                  ; 000000
145170: nop                                                  ; 000000
145171: move     y:(r4+$141900),y0                           ; 0B74C6 141900
145173: add      #>$20,b                                     ; 0140C8 000020
145175: cmp      y0,b                                        ; 20005D
145176: blt      func_14517d                                 ; 0D1049 000007
145178: move     #>$2,x0                                     ; 44F400 000002
14517A: move     x0,y:(r6+$11)                               ; 0246E4
14517B: bra      func_145180                                 ; 0D10C0 000005
14517D: move     b,y:(r6+$10)                                ; 0246AF
14517E: bra      func_14518d                                 ; 0D10C0 00000F
145180: move     y:(r6+$a),a                                 ; 022EBE
145181: asr      #$10,a,a                                    ; 0C1C20
145182: move     y:(r6+$12),b                                ; 024EBF
145183: move     a,r4                                        ; 21D400
145184: nop                                                  ; 000000
145185: nop                                                  ; 000000
145186: nop                                                  ; 000000
145187: move     y:(r4+$141a00),y0                           ; 0B74C6 141A00
145189: sub      y0,b                                        ; 20005C
14518A: clr      b               ifmi                        ; 202B1B
14518B: nop                                                  ; 000000
14518C: move     b,y:(r6+$12)                                ; 024EAF
14518D: move     y:(r6+$4),x0                                ; 0216B4
14518E: move     y:(r6+$1),x1                                ; 0206F5
14518F: mpyi     #>$63f,x0,a                                 ; 0141C0 00063F
145191: maci     #>$80000,x1,a                               ; 0141E2 080000
145193: sub      #>$100,a                                    ; 0140C4 000100
145195: rnd      a                                           ; 200011
145196: tfr      a,b                                         ; 200009
145197: clr      b               ifmi                        ; 202B1B
145198: cmp      #>$63f,b                                    ; 0140CD 00063F
14519A: blt      func_14519e                                 ; 0D1049 000004
14519C: move     #>$63f,b                                    ; 57F400 00063F
14519E: move     y:(r6+$5),x0                                ; 0216F4
14519F: move     b,y:(r6+$14)                                ; 0256AF
1451A0: maci     #>$73f,x0,a                                 ; 0141C2 00073F
1451A2: move     #>$63f,b                                    ; 57F400 00063F
1451A4: move     a,x0                                        ; 21C400
1451A5: sub      a,b                                         ; 20001C
1451A6: move     y:(r6+$7),a                                 ; 021EFE
1451A7: asl      a                                           ; 200032
1451A8: add      #>$800000,a                                 ; 0140C0 800000
1451AA: bpl      func_1451ad                                 ; 0D1043 000003
1451AC: move     x0,b                                        ; 208F00
1451AD: move     y:(r6+$12),y1                               ; 024EB7
1451AE: move     a,y0                                        ; 21C600
1451AF: mpy      y1,y0,a                                     ; 2000B0
1451B0: move     b,y0                                        ; 21E600
1451B1: move     a,y1                                        ; 21C700
1451B2: mpy      y1,y0,a                                     ; 2000B0
1451B3: add      x0,a                                        ; 200040
1451B4: clr      a               ifmi                        ; 202B13
1451B5: cmp      #>$63f,a                                    ; 0140C5 00063F
1451B7: blt      func_1451bc                                 ; 0D1049 000005
1451B9: move     #>$63f,a                                    ; 56F400 00063F
1451BB: nop                                                  ; 000000
1451BC: move     a,y:(r6+$16)                                ; 025EAE
1451BD: move     #>$7e,r2                                    ; 62F400 00007E
1451BF: move     #>$be,r3                                    ; 63F400 0000BE
1451C1: do       #<$10,>$1451c7                              ; 061080 1451C6
1451C3: move     x:(r0)+,x0                                  ; 44D800
1451C4: move     x0,x:(r2)+                                  ; 445A00
1451C5: move     x:(r0)+,x0                                  ; 44D800
1451C6: move     x0,x:(r3)+                                  ; 445B00
1451C7: move     y:(r6+$16),a                                ; 025EBE
1451C8: move     y:(r6+$6),x0                                ; 021EB4
1451C9: move     a,r0                                        ; 21D000
1451CA: tfr      a,b                                         ; 200009
1451CB: maci     #>$fff912,x0,b                              ; 0141CA FFF912
1451CD: move     x:(r0+$143546),x1                           ; 0A70C5 143546
1451CF: add      x1,b                                        ; 200068
1451D0: move     a1,r3                                       ; 219300
1451D1: move     b1,r2                                       ; 21B200
1451D2: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1451D4: move     x:(r3+$141b18),x1                           ; 0A73C5 141B18
1451D6: move     x:(r2+$142f06),y1                           ; 0A72C7 142F06
1451D8: mpy      y1,x1,a         y1,x0                       ; 20E4F0
1451D9: mpy      -x0,y1,b                                    ; 2000CC
1451DA: move     #>$7a,r3                                    ; 63F400 00007A
1451DC: add      #>$c00000,a                                 ; 0140C0 C00000
1451DE: asr      a                                           ; 200022
1451DF: move     y:(r6+$2d),y1                               ; 02B6F7
1451E0: move     y1,x:(r3)+                                  ; 475B00
1451E1: move     a,x0                                        ; 21C400
1451E2: asl      #$2,a,a                                     ; 0C1D04
1451E3: add      b,a                                         ; 200010
1451E4: asr      #$2,a,a                                     ; 0C1C04
1451E5: move     x0,x:>$35                                   ; 447000 000035
1451E7: move     y:(r6+$38),y0                               ; 02E6B6
1451E8: sub      y0,b            b,x0                        ; 21E45C
1451E9: move     x0,y:(r6+$38)                               ; 02E6A4
1451EA: move     y0,x:>$36                                   ; 467000 000036
1451EC: move     y:(r6+$37),y1                               ; 02DEF7
1451ED: sub      y1,a            a,x0                        ; 21C474
1451EE: move     x0,y:(r6+$37)                               ; 02DEE4
1451EF: move     y1,x:>$37                                   ; 477000 000037
1451F1: move     b,x:>$32                                    ; 577000 000032
1451F3: move     a,x:>$33                                    ; 567000 000033
1451F5: move     x:>$35,a                                    ; 56F000 000035
1451F7: move     y:(r6+$39),x1                               ; 02E6F5
1451F8: sub      x1,a            a,x0                        ; 21C464
1451F9: move     x0,y:(r6+$39)                               ; 02E6E4
1451FA: move     x1,x:>$38                                   ; 457000 000038
1451FC: move     a,x:>$34                                    ; 567000 000034
1451FE: move     #>$0,r4                                     ; 64F400 000000
145200: move     #>$20,r5                                    ; 65F400 000020
145202: move     #>$40,r7                                    ; 67F400 000040
145204: move     x:>$36,a                                    ; 56F000 000036
145206: move     x:>$37,b                                    ; 57F000 000037
145208: move     x:>$32,y0                                   ; 46F000 000032
14520A: move     x:>$33,y1                                   ; 47F000 000033
14520C: move     #$8,x0                                      ; 240800
14520D: do       #<$10,>$145211                              ; 061080 145210
14520F: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
145210: mac      x0,y1,b         b,y:(r5)+                   ; 5F5DCA
145211: move     x:>$38,a                                    ; 56F000 000038
145213: move     x:>$34,x1                                   ; 45F000 000034
145215: do       #<$10,>$145219                              ; 061080 145218
145217: mac      x1,x0,a         a,y:(r7)+                   ; 5E5FA2
145218: nop                                                  ; 000000
145219: move     #>$7a,r0                                    ; 60F400 00007A
14521B: move     #>$7c,r1                                    ; 61F400 00007C
14521D: move     #>$0,r4                                     ; 64F400 000000
14521F: move     #>$20,r5                                    ; 65F400 000020
145221: move     #>$40,r7                                    ; 67F400 000040
145223: move     y:(r6+$2f),x0                               ; 02BEF4
145224: move     x0,x:(r3)+                                  ; 445B00
145225: move     x0,a                                        ; 208E00
145226: move     y:(r6+$2e),a0                               ; 02BEB8
145227: move     y:(r6+$30),x0                               ; 02C6B4
145228: move     x0,x:(r3)+                                  ; 445B00
145229: move     y:(r6+$32),x0                               ; 02CEB4
14522A: move     x0,x:(r3)+                                  ; 445B00
14522B: move     x0,b                                        ; 208F00
14522C: move     y:(r6+$31),b0                               ; 02C6F9
14522D: move     #>$7c,r2                                    ; 62F400 00007C
14522F: move     x:(r0)+,x0      y:(r4),y0                   ; C09800
145230: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
145231: mac      y0,x0,a                                     ; 2000D2
145232: mac      y0,x0,a                                     ; 2000D2
145233: mac      y0,x0,a                                     ; 2000D2
145234: do       #<$10,>$145242                              ; 061080 145241
145236: mac      y0,x0,a         x:(r0)-,x0      y:(r5),y0   ; C0B0D2
145237: mac      y0,x0,a         x:(r1)+,x0      y:(r4)+,y0  ; F099D2
145238: mac      y0,x0,b         x:(r1)+,x0      y:(r7)+,y0  ; F0F9DA
145239: mac      y0,x0,b         a,l:(r2)+                   ; 485ADA
14523A: mac      y0,x0,b                                     ; 2000DA
14523B: mac      y0,x0,b                                     ; 2000DA
14523C: mac      y0,x0,b         x:(r1)-,x0      y:(r5)+,y0  ; F0B1DA
14523D: mac      y0,x0,b         x:(r0)+,x0      y:(r4),y0   ; C098DA
14523E: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
14523F: mac      y0,x0,a         b,l:(r3)+                   ; 495BD2
145240: mac      y0,x0,a                                     ; 2000D2
145241: mac      y0,x0,a                                     ; 2000D2
145242: move     l:-(r3),a                                   ; 48FB00
145243: move     x:-(r3),y1                                  ; 47FB00
145244: move     y1,y:(r6+$30)                               ; 02C6A7
145245: move     a1,y:(r6+$32)                               ; 02CEAC
145246: move     a0,y:(r6+$31)                               ; 02C6E8
145247: move     l:-(r2),a                                   ; 48FA00
145248: move     x:-(r2),y1                                  ; 47FA00
145249: move     y1,y:(r6+$2d)                               ; 02B6E7
14524A: move     a1,y:(r6+$2f)                               ; 02BEEC
14524B: move     a0,y:(r6+$2e)                               ; 02BEA8
14524C: move     #>$ba,r0                                    ; 60F400 0000BA
14524E: move     r0,r3                                       ; 221300
14524F: move     #>$bc,r1                                    ; 61F400 0000BC
145251: move     r1,r2                                       ; 223200
145252: move     #>$0,r4                                     ; 64F400 000000
145254: move     #>$20,r5                                    ; 65F400 000020
145256: move     #>$40,r7                                    ; 67F400 000040
145258: move     y:(r6+$27),y1                               ; 029EF7
145259: move     y1,x:(r3)+                                  ; 475B00
14525A: move     y:(r6+$29),x0                               ; 02A6F4
14525B: move     x0,x:(r3)+                                  ; 445B00
14525C: move     x0,a                                        ; 208E00
14525D: move     y:(r6+$28),a0                               ; 02A6B8
14525E: move     y:(r6+$2a),x0                               ; 02AEB4
14525F: move     x0,x:(r3)+                                  ; 445B00
145260: move     y:(r6+$2c),x0                               ; 02B6B4
145261: move     x0,x:(r3)+                                  ; 445B00
145262: move     x0,b                                        ; 208F00
145263: move     y:(r6+$2b),b0                               ; 02AEF9
145264: move     x:(r0)+,x0      y:(r4),y0                   ; C09800
145265: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
145266: mac      y0,x0,a                                     ; 2000D2
145267: mac      y0,x0,a                                     ; 2000D2
145268: mac      y0,x0,a                                     ; 2000D2
145269: do       #<$10,>$145277                              ; 061080 145276
14526B: mac      y0,x0,a         x:(r0)-,x0      y:(r5),y0   ; C0B0D2
14526C: mac      y0,x0,a         x:(r1)+,x0      y:(r4)+,y0  ; F099D2
14526D: mac      y0,x0,b         x:(r1)+,x0      y:(r7)+,y0  ; F0F9DA
14526E: mac      y0,x0,b         a,l:(r2)+                   ; 485ADA
14526F: mac      y0,x0,b                                     ; 2000DA
145270: mac      y0,x0,b                                     ; 2000DA
145271: mac      y0,x0,b         x:(r1)-,x0      y:(r5)+,y0  ; F0B1DA
145272: mac      y0,x0,b         x:(r0)+,x0      y:(r4),y0   ; C098DA
145273: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
145274: mac      y0,x0,a         b,l:(r3)+                   ; 495BD2
145275: mac      y0,x0,a                                     ; 2000D2
145276: mac      y0,x0,a                                     ; 2000D2
145277: move     l:-(r3),a                                   ; 48FB00
145278: move     x:-(r3),y1                                  ; 47FB00
145279: move     y1,y:(r6+$2a)                               ; 02AEA7
14527A: move     a1,y:(r6+$2c)                               ; 02B6AC
14527B: move     a0,y:(r6+$2b)                               ; 02AEE8
14527C: move     l:-(r2),a                                   ; 48FA00
14527D: move     x:-(r2),y1                                  ; 47FA00
14527E: move     y1,y:(r6+$27)                               ; 029EE7
14527F: move     a1,y:(r6+$29)                               ; 02A6EC
145280: move     a0,y:(r6+$28)                               ; 02A6A8
145281: move     #>$74,r0                                    ; 60F400 000074
145283: move     #>$b4,r1                                    ; 61F400 0000B4
145285: move     r0,r4                                       ; 221400
145286: move     r1,r5                                       ; 223500
145287: move     x:(r0)+,x0                                  ; 44D800
145288: mpy      x0,x0,a         x:(r1)+,x0                  ; 44D980
145289: asl      #$4,a,a                                     ; 0C1D08
14528A: mpy      x0,x0,b         x:(r0)+,x0                  ; 44D888
14528B: asl      #$4,b,b                                     ; 0C1D89
14528C: do       #<$10,>$145292                              ; 061080 145291
14528E: mpy      x0,x0,a         x:(r1)+,x0      a,y:(r5)+   ; B2B980
14528F: asl      #$4,a,a                                     ; 0C1D08
145290: mpy      x0,x0,b         x:(r0)+,x0      b,y:(r4)+   ; B39888
145291: asl      #$4,b,b                                     ; 0C1D89
145292: move     #>$74,r0                                    ; 60F400 000074
145294: move     #>$b4,r1                                    ; 61F400 0000B4
145296: move     r0,r4                                       ; 221400
145297: move     r1,r5                                       ; 223500
145298: move     #>$0,r2                                     ; 62F400 000000
14529A: move     #>$20,r3                                    ; 63F400 000020
14529C: move     x:(r0)+,x0      y:(r4)+,y0                  ; F09800
14529D: do       #<$10,>$1452a5                              ; 061080 1452A4
14529F: mpy      y0,x0,a         x:(r1)+,x0      y:(r5)+,y0  ; F0B9D0
1452A0: mpy      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098D8
1452A1: asl      #$4,a,a                                     ; 0C1D08
1452A2: asl      #$4,b,b                                     ; 0C1D89
1452A3: move     a,y:(r2)+                                   ; 5E5A00
1452A4: move     b,y:(r3)+                                   ; 5F5B00
1452A5: move     #>$74,r0                                    ; 60F400 000074
1452A7: move     #>$b4,r1                                    ; 61F400 0000B4
1452A9: move     #>$0,r4                                     ; 64F400 000000
1452AB: move     #>$20,r5                                    ; 65F400 000020
1452AD: move     #$0,x0                                      ; 240000
1452AE: nop                                                  ; 000000
1452AF: move     y:(r4)+,y0                                  ; 4EDC00
1452B0: do       #<$10,>$1452b7                              ; 061080 1452B6
1452B2: move     x:(r0),a                                    ; 56E000
1452B3: mac      -y0,x0,a        x:(r1),b        y:(r5)+,y0  ; FCA1D6
1452B4: mac      -y0,x0,b        y:(r4)+,y0                  ; 4EDCDE
1452B5: move     a,x:(r0)+                                   ; 565800
1452B6: move     b,x:(r1)+                                   ; 575900
1452B7: move     y:(r6+$14),r1                               ; 0B76D1 000014
1452B9: move     #$1f,n6                                     ; 3E1F00
1452BA: move     #>$74,r0                                    ; 60F400 000074
1452BC: move     x:(r1+$142f06),x0                           ; 0A71C4 142F06
1452BE: mpy      -x0,x0,b        x0,a                        ; 208E8C
1452BF: asl      a                                           ; 200032
1452C0: add      #>$800000,a                                 ; 0140C0 800000
1452C2: lua      (r6)+n6,r4                                  ; 044E14
1452C3: move     a,y0                                        ; 21C600
1452C4: move     x0,a                                        ; 208E00
1452C5: asl      a                                           ; 200032
1452C6: add      #>$7fffff,a                                 ; 0140C0 7FFFFF
1452C8: sub      b,a                                         ; 200014
1452C9: asr      #$3,a,a                                     ; 0C1C06
1452CA: nop                                                  ; 000000
1452CB: move     a,x:>$3c                                    ; 567000 00003C
1452CD: move     y0,a                                        ; 20CE00
1452CE: move     y:(r6+$34),y0                               ; 02D6B6
1452CF: sub      y0,b            b,x0                        ; 21E45C
1452D0: move     x0,y:(r6+$34)                               ; 02D6A4
1452D1: move     y0,x:>$36                                   ; 467000 000036
1452D3: move     y:(r6+$35),y1                               ; 02D6F7
1452D4: sub      y1,a            a,x0                        ; 21C474
1452D5: move     x0,y:(r6+$35)                               ; 02D6E4
1452D6: move     y1,x:>$37                                   ; 477000 000037
1452D8: move     b,x:>$32                                    ; 577000 000032
1452DA: move     a,x:>$33                                    ; 567000 000033
1452DC: move     #>$e0,r5                                    ; 65F400 0000E0
1452DE: move     #>$40,r7                                    ; 67F400 000040
1452E0: move     x:>$36,a                                    ; 56F000 000036
1452E2: move     x:>$37,b                                    ; 57F000 000037
1452E4: move     x:>$32,y0                                   ; 46F000 000032
1452E6: move     x:>$33,y1                                   ; 47F000 000033
1452E8: move     r1,r2                                       ; 223200
1452E9: move     #$8,x0                                      ; 240800
1452EA: do       #<$10,>$1452ee                              ; 061080 1452ED
1452EC: mac      y0,x0,a a,y:(r5)+                           ; 5E5DD2
1452ED: mac      x0,y1,b         b,y:(r7)+                   ; 5F5FCA
1452EE: move     x:>$3c,a                                    ; 56F000 00003C
1452F0: move     y:(r6+$33),y1                               ; 02CEF7
1452F1: sub      y1,a            a,x0                        ; 21C474
1452F2: move     x0,y:(r6+$33)                               ; 02CEE4
1452F3: move     y1,b                                        ; 20EF00
1452F4: neg      a               a,y1                        ; 21C736
1452F5: asl      a                                           ; 200032
1452F6: move     #>$0,r7                                     ; 67F400 000000
1452F8: tfr      b,a             a,y0                        ; 21C601
1452F9: neg      a                                           ; 200036
1452FA: asl      a                                           ; 200032
1452FB: move     x:(r2)+,x0                                  ; 44DA00
1452FC: do       #<$10,>$145300                              ; 061080 1452FF
1452FE: mac      x0,y1,b         b,y:(r7)+                   ; 5F5FCA
1452FF: mac      y0,x0,a         x:(r2)+,x0      a,y:(r7)+   ; B2FAD2
145300: move     y:(r4)+,x0                                  ; 4CDC00
145301: move     x0,x:(r0)+                                  ; 445800
145302: move     y:(r4)+,b                                   ; 5FDC00
145303: move     y:(r4)+,b0                                  ; 59DC00
145304: move     y:(r4)+,x1                                  ; 4DDC00
145305: move     b,l:(r0)+                                   ; 495800
145306: move     x1,x:(r0)+                                  ; 455800
145307: move     y:(r4)+,a                                   ; 5EDC00
145308: move     y:(r4)+,a0                                  ; 58DC00
145309: move     y:(r4)+,y1                                  ; 4FDC00
14530A: move     a,l:(r0)+                                   ; 485800
14530B: move     y:(r4)+,y0                                  ; 4EDC00
14530C: move     y0,x:(r0)+                                  ; 465800
14530D: move     y1,x:(r0)+                                  ; 475800
14530E: move     #>$76,r0                                    ; 60F400 000076
145310: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145312: move     #>$fffffd,n0                                ; 70F400 FFFFFD
145314: move     #>$74,r1                                    ; 61F400 000074
145316: move     n0,n1                                       ; 231900
145317: move     #>$78,r2                                    ; 62F400 000078
145319: move     r0,r3                                       ; 221300
14531A: move     #>$e0,r5                                    ; 65F400 0000E0
14531C: move     #>$40,r7                                    ; 67F400 000040
14531E: move     #>$0,r4                                     ; 64F400 000000
145320: move     #$26,n6                                     ; 3E2600
145321: move     x:(r0)+,x0      y:(r5),y0                   ; C0B800
145322: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
145323: do       #<$10,>$145331                              ; 061080 145330
145325: mac      y0,x0,a         x:(r0)+,x0      y:(r4)+,y1  ; F198D2
145326: mac      x0,y1,a         x:(r0)+,x0      y:(r4),y0   ; C098C2
145327: mac      y0,x0,a         x:(r0)+n0,x0                ; 44C8D2
145328: mac      x0,y1,a         x:(r1)+,x0      y:(r5)+,y0  ; F0B9C2
145329: mac      y0,x0,b         x:(r1)+,x0      y:(r7)+,y0  ; F0F9DA
14532A: move     a,l:(r2)+                                   ; 485A00
14532B: mac      y0,x0,b         x:(r1)+,x0                  ; 44D9DA
14532C: mac      x0,y1,b         x:(r1)+,x0      y:(r4)+,y0  ; F099CA
14532D: mac      y0,x0,b         x:(r1)+n1,x0                ; 44C9DA
14532E: mac      x0,y1,b         x:(r0)+,x0      y:(r5),y0   ; C0B8CA
14532F: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
145330: move     b,l:(r3)+                                   ; 495B00
145331: lua      (r6)+n6,r7                                  ; 044E17
145332: move     #>$b4,r0                                    ; 60F400 0000B4
145334: move     x:(r2)+,y0                                  ; 46DA00
145335: move     x:(r2)-,y1                                  ; 47D200
145336: move     l:-(r2),x                                   ; 42FA00
145337: move     #$17,n6                                     ; 3E1700
145338: move     y0,y:(r7)-                                  ; 4E5700
145339: move     y1,y:(r7)-                                  ; 4F5700
14533A: move     x0,y:(r7)-                                  ; 4C5700
14533B: move     x1,y:(r7)-                                  ; 4D5700
14533C: lua      (r6)+n6,r4                                  ; 044E14
14533D: move     x:-(r2),x0                                  ; 44FA00
14533E: move     x0,y:(r7)-                                  ; 4C5700
14533F: move     l:-(r2),x                                   ; 42FA00
145340: move     x0,y:(r7)-                                  ; 4C5700
145341: move     x1,y:(r7)-                                  ; 4D5700
145342: move     x:-(r2),x0                                  ; 44FA00
145343: move     x0,y:(r7)-                                  ; 4C5700
145344: move     y:(r4)+,x0                                  ; 4CDC00
145345: move     x0,x:(r0)+                                  ; 445800
145346: move     y:(r4)+,b                                   ; 5FDC00
145347: move     y:(r4)+,b0                                  ; 59DC00
145348: move     y:(r4)+,x1                                  ; 4DDC00
145349: move     b,l:(r0)+                                   ; 495800
14534A: move     x1,x:(r0)+                                  ; 455800
14534B: move     y:(r4)+,a                                   ; 5EDC00
14534C: move     y:(r4)+,a0                                  ; 58DC00
14534D: move     y:(r4)+,y1                                  ; 4FDC00
14534E: move     a,l:(r0)+                                   ; 485800
14534F: move     y:(r4)+,y0                                  ; 4EDC00
145350: move     y0,x:(r0)+                                  ; 465800
145351: move     y1,x:(r0)+                                  ; 475800
145352: move     #>$b6,r0                                    ; 60F400 0000B6
145354: move     #>$b4,r1                                    ; 61F400 0000B4
145356: move     #>$b8,r2                                    ; 62F400 0000B8
145358: move     r0,r3                                       ; 221300
145359: move     #>$e0,r5                                    ; 65F400 0000E0
14535B: move     #>$40,r7                                    ; 67F400 000040
14535D: move     #>$0,r4                                     ; 64F400 000000
14535F: move     #$1e,n6                                     ; 3E1E00
145360: move     x:(r0)+,x0      y:(r5),y0                   ; C0B800
145361: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
145362: do       #<$10,>$145370                              ; 061080 14536F
145364: mac      y0,x0,a         x:(r0)+,x0      y:(r4)+,y1  ; F198D2
145365: mac      x0,y1,a         x:(r0)+,x0      y:(r4),y0   ; C098C2
145366: mac      y0,x0,a         x:(r0)+n0,x0                ; 44C8D2
145367: mac      x0,y1,a         x:(r1)+,x0      y:(r5)+,y0  ; F0B9C2
145368: mac      y0,x0,b         x:(r1)+,x0      y:(r7)+,y0  ; F0F9DA
145369: move     a,l:(r2)+                                   ; 485A00
14536A: mac      y0,x0,b         x:(r1)+,x0                  ; 44D9DA
14536B: mac      x0,y1,b         x:(r1)+,x0      y:(r4)+,y0  ; F099CA
14536C: mac      y0,x0,b         x:(r1)+n1,x0                ; 44C9DA
14536D: mac      x0,y1,b         x:(r0)+,x0      y:(r5),y0   ; C0B8CA
14536E: mac      y0,x0,a         x:(r0)+,x0      y:(r7),y0   ; C0F8D2
14536F: move     b,l:(r3)+                                   ; 495B00
145370: lua      (r6)+n6,r4                                  ; 044E14
145371: move     x:(r2)+,y0                                  ; 46DA00
145372: move     x:(r2)-,y1                                  ; 47D200
145373: move     l:-(r2),x                                   ; 42FA00
145374: move     y0,y:(r4)-                                  ; 4E5400
145375: move     y1,y:(r4)-                                  ; 4F5400
145376: move     x0,y:(r4)-                                  ; 4C5400
145377: move     x1,y:(r4)-                                  ; 4D5400
145378: move     x:-(r2),x0                                  ; 44FA00
145379: move     x0,y:(r4)-                                  ; 4C5400
14537A: move     l:-(r2),x                                   ; 42FA00
14537B: move     x0,y:(r4)-                                  ; 4C5400
14537C: move     x1,y:(r4)-                                  ; 4D5400
14537D: move     x:-(r2),x0                                  ; 44FA00
14537E: move     x0,y:(r4)-                                  ; 4C5400
14537F: move     x:>$ff,r7                                   ; 67F000 0000FF
145381: move     #>$76,r2                                    ; 62F400 000076
145383: move     #>$b6,r3                                    ; 63F400 0000B6
145385: do       #<$10,>$14538d                              ; 061080 14538C
145387: move     x:(r2)+,a                                   ; 56DA00
145388: move     x:(r3)+,b                                   ; 57DB00
145389: asl      #$7,a,a                                     ; 0C1D0E
14538A: asl      #$7,b,b                                     ; 0C1D8F
14538B: move     a,y:(r7)+                                   ; 5E5F00
14538C: move     b,y:(r7)+                                   ; 5F5F00
14538D: rts                                                  ; 00000C
14538E: lua      (r6+$26),r3                                 ; 041663
14538F: lua      (r6+$33),r5                                 ; 041E35
145390: lua      (r6+$39),r4                                 ; 041E94
145391: lua      (r6+$3f),r7                                 ; 041EF7
145392: move     #$0,x1                                      ; 250000
145393: rep      #<$6                                        ; 0606A0
145394: move     x1,y:(r3)+                                  ; 4D5B00
145395: move     x1,y:(r6+$20)                               ; 0286A5
145396: move     #>$f4240,x1                                 ; 45F400 0F4240
145398: move     x1,y:(r6+$25)                               ; 0296E5
145399: move     #>$14a000,x1                                ; 45F400 14A000
14539B: move     x1,y:(r6+$32)                               ; 02CEA5
14539C: move     #>$114000,a0                                ; 50F400 114000
14539E: move     y:>$124,x1                                  ; 4DF000 000124
1453A0: maci     #>$2000,x1,a                                ; 0141E2 002000
1453A2: move     a0,x0                                       ; 210400
1453A3: move     #>$0,a                                      ; 56F400 000000
1453A5: add      x0,a            #>$1000,b                   ; 57F440 001000
1453A7: add      x0,b            a,y:(r5)+                   ; 5E5D48
1453A8: add      #>$a67,a                                    ; 0140C0 000A67
1453AA: move     b,y:(r5)+                                   ; 5F5D00
1453AB: add      #>$98f,b                                    ; 0140C8 00098F
1453AD: move     a,y:(r4)+                                   ; 5E5C00
1453AE: move     b,y:(r4)+                                   ; 5F5C00
1453AF: move     #>$2000,a                                   ; 56F400 002000
1453B1: add      x0,a            #>$2800,b                   ; 57F440 002800
1453B3: add      x0,b            a,y:(r5)+                   ; 5E5D48
1453B4: add      #>$713,a                                    ; 0140C0 000713
1453B6: move     b,y:(r5)+                                   ; 5F5D00
1453B7: add      #>$655,b                                    ; 0140C8 000655
1453B9: move     a,y:(r4)+                                   ; 5E5C00
1453BA: move     b,y:(r4)+                                   ; 5F5C00
1453BB: move     #>$3000,a                                   ; 56F400 003000
1453BD: add      x0,a            #>$c00,b                    ; 57F440 000C00
1453BF: add      x0,b            a,y:(r5)+                   ; 5E5D48
1453C0: add      #>$577,a                                    ; 0140C0 000577
1453C2: move     b,y:(r5)+                                   ; 5F5D00
1453C3: add      #>$265,b                                    ; 0140C8 000265
1453C5: move     a,y:(r4)+                                   ; 5E5C00
1453C6: move     #>$19c0,a                                   ; 56F400 0019C0
1453C8: add      x0,a            b,y:(r4)+                   ; 5F5C40
1453C9: move     #>$1a00,b                                   ; 57F400 001A00
1453CB: add      x0,b            a,y:(r7)+                   ; 5E5F48
1453CC: move     #>$1a80,a                                   ; 56F400 001A80
1453CE: add      x0,a            b,y:(r7)+                   ; 5F5F40
1453CF: move     #>$1b00,b                                   ; 57F400 001B00
1453D1: add      x0,b            a,y:(r7)+                   ; 5E5F48
1453D2: move     #>$1c00,a                                   ; 56F400 001C00
1453D4: add      x0,a            b,y:(r7)+                   ; 5F5F40
1453D5: move     a,y:(r7)+                                   ; 5E5F00
1453D6: move     #$0,x0                                      ; 240000
1453D7: move     x0,y:(r6+$48)                               ; 0B7684 000048
1453D9: move     x0,y:(r6+$47)                               ; 0B7684 000047
1453DB: rts                                                  ; 00000C
1453DC: move     #$0,x0                                      ; 240000
1453DD: move     x0,x:(r6-$19)                               ; 039EC4
1453DE: move     r7,y:(r6+$45)                               ; 0B7697 000045
1453E0: move     r0,y:(r6+$46)                               ; 0B7690 000046
1453E2: move     #>$7fffff,b                                 ; 57F400 7FFFFF
1453E4: move     y:(r6+$48),a                                ; 0B76CE 000048
1453E6: cmp      #>$100,a                                    ; 0140C5 000100
1453E8: bge      func_1453ed                                 ; 0D1041 000005
1453EA: add      #>$1,a                                      ; 0140C0 000001
1453EC: clr      b                                           ; 20001B
1453ED: move     a,y:(r6+$48)                                ; 0B768E 000048
1453EF: move     b,y:(r6+$47)                                ; 0B768F 000047
1453F1: move     #>$1457ac,a0                                ; 50F400 1457AC
1453F3: tst      b                                           ; 20000B
1453F4: beq      func_1453fe                                 ; 0D104A 00000A
1453F6: clr      a                                           ; 200013
1453F7: move     #>$1457b2,a0                                ; 50F400 1457B2
1453F9: move     y:(r6+$5),b                                 ; 0216FF
1453FA: asr      #$11,b,b                                    ; 0C1CA3
1453FB: move     b,x0                                        ; 21E400
1453FC: maci     #>$3,x0,a                                   ; 0141C2 000003
1453FE: move     a0,y:(r6+$13)                               ; 024EE8
1453FF: clr      a                                           ; 200013
145400: move     #>$14562c,a0                                ; 50F400 14562C
145402: move     y:(r6+$4),b                                 ; 0216BF
145403: asr      #$11,b,b                                    ; 0C1CA3
145404: move     b,x0                                        ; 21E400
145405: maci     #>$3,x0,a                                   ; 0141C2 000003
145407: move     a0,y:(r6+$12)                               ; 024EA8
145408: move     #>$f4240,a                                  ; 56F400 0F4240
14540A: move     y:(r6+$6),x0                                ; 021EB4
14540B: mpy      x0,x0,b                                     ; 200088
14540C: asr      #$b,b,b                                     ; 0C1C97
14540D: cmp      #>$fa0,b                                    ; 0140CD 000FA0
14540F: tfr      a,b             ifgt                        ; 202709
145410: move     b,y:(r6+$25)                                ; 0296EF
145411: move     #>$0,r7                                     ; 67F400 000000
145413: move     y:(r6+$b),y0                                ; 022EF6
145414: mpy      y0,y0,a         x:(r0)+,x0                  ; 44D890
145415: move     a,y0                                        ; 21C600
145416: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
145417: mac      y0,x0,a                                     ; 2000D2
145418: asl      a               x:(r0)+,x0                  ; 44D832
145419: do       #<$8,>$145421                               ; 060880 145420
14541B: mpy      y0,x0,b         x:(r0)+,x0                  ; 44D8D8
14541C: mac      y0,x0,b         a,y:(r7)+                   ; 5E5FDA
14541D: asl      b               x:(r0)+,x0                  ; 44D83A
14541E: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
14541F: mac      y0,x0,a         b,y:(r7)+                   ; 5F5FD2
145420: asl      a               x:(r0)+,x0                  ; 44D832
145421: move     y:(r6+$4),x1                                ; 0216B5
145422: mpyi     #>$66666,x1,b                               ; 0141E8 066666
145424: add      #>$59999a,b                                 ; 0140C8 59999A
145426: move     y:(r6+$47),x0                               ; 0B76C4 000047
145428: move     b,x1                                        ; 21E500
145429: mpy      x1,x0,a                                     ; 2000A0
14542A: move     #>$14561c,r4                                ; 64F400 14561C
14542C: move     a,y1                                        ; 21C700
14542D: lua      (r6+$3f),r2                                 ; 041EF2
14542E: do       #<$5,>$145442                               ; 060580 145441
145430: move     x:(r4+$5),m1                                ; 0A74E1 000005
145432: move     y:(r2),r1                                   ; 69E200
145433: move     #>$0,r7                                     ; 67F400 000000
145435: move     x:(r4)+,y0                                  ; 46DC00
145436: mpy      y1,y0,a         r7,r5                       ; 22F5B0
145437: move     x:(r1),x0                                   ; 44E100
145438: move     a,x1                                        ; 21C500
145439: move     x:(r2),b                                    ; 57E200
14543A: do       #<$10,>$145440                              ; 061080 14543F
14543C: tfr      x0,a            b,x:(r1)+       b,y0        ; 1E1941
14543D: mac      -x1,y0,a        y:(r7)+,b                   ; 5FDFE6
14543E: move     x:(r1),x0                                   ; 44E100
14543F: mac      x1,x0,b         a,y:(r5)+                   ; 5E5DAA
145440: move     r1,y:(r2)                                   ; 696200
145441: move     b,x:(r2)+                                   ; 575A00
145442: move     #>$ffffff,m1                                ; 05F421 FFFFFF
145444: move     m1,m5                                       ; 0465A1
145445: move     y:(r6+$13),r2                               ; 0B76D2 000013
145447: move     y:(r6+$33),r1                               ; 0B76D1 000033
145449: move     #>$a76,m1                                   ; 05F421 000A76
14544B: move     y:(r6+$26),a                                ; 029EBE
14544C: move     y:(r6+$2c),a0                               ; 02B6B8
14544D: move     #>$20,r4                                    ; 64F400 000020
14544F: move     y:(r2)+,x0                                  ; 4CDA00
145450: move     x:(r1)+,y0                                  ; 46D900
145451: do       #<$10,>$145455                              ; 061080 145454
145453: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145454: mac      -x1,x0,a        x:(r1)+,y0                  ; 46D9A6
145455: move     x:(r1)-,y0                                  ; 46D100
145456: move     a1,y:(r6+$26)                               ; 029EAC
145457: move     a0,y:(r6+$2c)                               ; 02B6A8
145458: move     r1,y:(r6+$33)                               ; 0B7691 000033
14545A: move     y:(r2)+,x0                                  ; 4CDA00
14545B: move     y:(r6+$34),r1                               ; 0B76D1 000034
14545D: move     #>$99e,m1                                   ; 05F421 00099E
14545F: move     y:(r6+$27),a                                ; 029EFE
145460: move     y:(r6+$2d),a0                               ; 02B6F8
145461: move     #>$30,r4                                    ; 64F400 000030
145463: move     x:(r1)+,y0                                  ; 46D900
145464: do       #<$10,>$145468                              ; 061080 145467
145466: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145467: mac      -x1,x0,a        x:(r1)+,y0                  ; 46D9A6
145468: move     x:(r1)-,y0                                  ; 46D100
145469: move     a1,y:(r6+$27)                               ; 029EEC
14546A: move     a0,y:(r6+$2d)                               ; 02B6E8
14546B: move     r1,y:(r6+$34)                               ; 0B7691 000034
14546D: move     y:(r2)+,x0                                  ; 4CDA00
14546E: move     y:(r6+$35),r1                               ; 0B76D1 000035
145470: move     #>$722,m1                                   ; 05F421 000722
145472: move     y:(r6+$28),a                                ; 02A6BE
145473: move     y:(r6+$2e),a0                               ; 02BEB8
145474: move     #>$40,r4                                    ; 64F400 000040
145476: move     x:(r1)+,y0                                  ; 46D900
145477: do       #<$10,>$14547b                              ; 061080 14547A
145479: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
14547A: mac      -x1,x0,a        x:(r1)+,y0                  ; 46D9A6
14547B: move     x:(r1)-,y0                                  ; 46D100
14547C: move     a1,y:(r6+$28)                               ; 02A6AC
14547D: move     a0,y:(r6+$2e)                               ; 02BEA8
14547E: move     r1,y:(r6+$35)                               ; 0B7691 000035
145480: move     y:(r2)+,x0                                  ; 4CDA00
145481: move     y:(r6+$36),r1                               ; 0B76D1 000036
145483: move     #>$664,m1                                   ; 05F421 000664
145485: move     y:(r6+$29),a                                ; 02A6FE
145486: move     y:(r6+$2f),a0                               ; 02BEF8
145487: move     #>$50,r4                                    ; 64F400 000050
145489: move     x:(r1)+,y0                                  ; 46D900
14548A: do       #<$10,>$14548e                              ; 061080 14548D
14548C: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
14548D: mac      -x1,x0,a        x:(r1)+,y0                  ; 46D9A6
14548E: move     x:(r1)-,y0                                  ; 46D100
14548F: move     a1,y:(r6+$29)                               ; 02A6EC
145490: move     a0,y:(r6+$2f)                               ; 02BEE8
145491: move     r1,y:(r6+$36)                               ; 0B7691 000036
145493: move     #>$0,r3                                     ; 63F400 000000
145495: move     #$1,m3                                      ; 0501A3
145496: move     #$10,n4                                     ; 3C1000
145497: move     y:(r6+$32),r4                               ; 0B76D4 000032
145499: move     #>$1fff,m4                                  ; 05F424 001FFF
14549B: move     y:(r6+$37),r1                               ; 0B76D1 000037
14549D: move     #>$7ff,m1                                   ; 05F421 0007FF
14549F: move     y:(r4)+n4,b                                 ; 5FCC00
1454A0: asr      #$15,b,b                                    ; 0C1CAB
1454A1: move     r4,y:(r6+$32)                               ; 0B7694 000032
1454A3: move     b1,n1                                       ; 21B900
1454A4: move     #$0,b1                                      ; 2D0000
1454A5: asr      b                                           ; 20002A
1454A6: move     b0,y1                                       ; 212700
1454A7: move     #>$7fffff,b                                 ; 57F400 7FFFFF
1454A9: sub      y1,b                                        ; 20007C
1454AA: move     #>$ffffff,m4                                ; 05F424 FFFFFF
1454AC: move     b,x0                                        ; 21E400
1454AD: lua      (r1)+n1,r1                                  ; 044911
1454AE: move     y:(r2)+,x1                                  ; 4DDA00
1454AF: mpy      x1,x0,b                                     ; 2000A8
1454B0: mpy      y1,x1,a                                     ; 2000F0
1454B1: move     b,x0                                        ; 21E400
1454B2: move     a,y1                                        ; 21C700
1454B3: move     x1,x:(r3)+                                  ; 455B00
1454B4: move     x0,x:(r3)+                                  ; 445B00
1454B5: move     y:(r6+$2a),a                                ; 02AEBE
1454B6: move     y:(r6+$30),a0                               ; 02C6B8
1454B7: move     #>$60,r4                                    ; 64F400 000060
1454B9: move     x:(r1)+,x1                                  ; 45D900
1454BA: do       #<$10,>$1454bf                              ; 061080 1454BE
1454BC: mac      x1,x0,a         x:(r1)+,x1      a,y:(r4)    ; 8699A2
1454BD: mac      y1,x1,a         x:(r3)+,x0      y:(r4)+,y0  ; F09BF2
1454BE: mac      -y0,x0,a        x:(r3)+,x0                  ; 44DBD6
1454BF: move     x:(r1)-,x1                                  ; 45D100
1454C0: lua      (r1)-n1,r1                                  ; 044111
1454C1: move     a1,y:(r6+$2a)                               ; 02AEAC
1454C2: move     a0,y:(r6+$30)                               ; 02C6A8
1454C3: move     r1,y:(r6+$37)                               ; 0B7691 000037
1454C5: move     y:(r6+$38),r1                               ; 0B76D1 000038
1454C7: move     #>$3ff,m1                                   ; 05F421 0003FF
1454C9: lua      (r1)+n1,r1                                  ; 044911
1454CA: move     y:(r6+$2b),a                                ; 02AEFE
1454CB: move     y:(r6+$31),a0                               ; 02C6F8
1454CC: move     #>$70,r4                                    ; 64F400 000070
1454CE: move     x:(r1)+,x1                                  ; 45D900
1454CF: do       #<$10,>$1454d4                              ; 061080 1454D3
1454D1: mac      x1,x0,a         x:(r1)+,x1      a,y:(r4)    ; 8699A2
1454D2: mac      y1,x1,a         x:(r3)+,x0      y:(r4)+,y0  ; F09BF2
1454D3: mac      -y0,x0,a        x:(r3)+,x0                  ; 44DBD6
1454D4: move     x:(r1)-,x1                                  ; 45D100
1454D5: lua      (r1)-n1,r1                                  ; 044111
1454D6: move     a1,y:(r6+$2b)                               ; 02AEEC
1454D7: move     a0,y:(r6+$31)                               ; 02C6E8
1454D8: move     r1,y:(r6+$38)                               ; 0B7691 000038
1454DA: move     #>$ffffff,m1                                ; 05F421 FFFFFF
1454DC: move     y:(r6+$12),r0                               ; 0B76D0 000012
1454DE: move     r6,r1                                       ; 22D100
1454DF: move     #$4,m3                                      ; 0504A3
1454E0: move     #>$20,r3                                    ; 63F400 000020
1454E2: move     y:(r0+$2),x0                                ; 0208B4
1454E3: move     y:(r0+$5),x1                                ; 0210F5
1454E4: move     x0,x:(r3)+                                  ; 445B00
1454E5: move     x1,x:(r3)+                                  ; 455B00
1454E6: move     y:(r0+$3),x0                                ; 0208F4
1454E7: move     y:(r0+$1),x1                                ; 0200F5
1454E8: move     x0,x:(r3)+                                  ; 445B00
1454E9: move     x1,x:(r3)+                                  ; 455B00
1454EA: move     y:(r0),x0                                   ; 4CE000
1454EB: move     x0,x:(r3)+                                  ; 445B00
1454EC: move     #>$0,r2                                     ; 62F400 000000
1454EE: move     #>$40,r0                                    ; 60F400 000040
1454F0: move     #>$70,r6                                    ; 66F400 000070
1454F2: move     #>$50,r4                                    ; 64F400 000050
1454F4: move     #>$30,r7                                    ; 67F400 000030
1454F6: move     #>$20,r5                                    ; 65F400 000020
1454F8: move     y:(r0)+,y1                                  ; 4FD800
1454F9: move     x:(r3)+,x0                                  ; 44DB00
1454FA: mpy      x0,y1,a         x:(r3)+,x0      y:(r6)+,y1  ; F1DBC0
1454FB: do       #<$10,>$145506                              ; 061080 145505
1454FD: mac      -x0,y1,a        x:(r3)+,x0      y:(r4)+,y1  ; F19BC6
1454FE: mac      -x0,y1,a        x:(r3)+,x0      y:(r7)+,y1  ; F1FBC6
1454FF: tfr      a,b             x:(r3)+,x1                  ; 45DB09
145500: mac      x0,y1,a         y:(r5)+,y1                  ; 4FDDC2
145501: asl      a               x:(r3)+,x0                  ; 44DB32
145502: mac      y1,x1,b         y:(r0)+,y1                  ; 4FD8FA
145503: asl      b               a,x:(r2)+                   ; 565A3A
145504: mpy      x0,y1,a         x:(r3)+,x0      y:(r6)+,y1  ; F1DBC0
145505: move     b,x:(r2)+                                   ; 575A00
145506: move     #>$ffffff,m3                                ; 05F423 FFFFFF
145508: move     r1,r6                                       ; 223600
145509: move     #>$30,r1                                    ; 61F400 000030
14550B: move     #>$40,r2                                    ; 62F400 000040
14550D: move     y:(r6+$12),r0                               ; 0B76D0 000012
14550F: move     #>$20,r4                                    ; 64F400 000020
145511: move     #>$50,r5                                    ; 65F400 000050
145513: move     x:(r0),x0                                   ; 44E000
145514: move     x:(r0+$3),x1                                ; 0208D5
145515: move     y:(r4)+,y0                                  ; 4EDC00
145516: move     y:(r5)+,y1                                  ; 4FDD00
145517: mpy      y0,x0,a                                     ; 2000D0
145518: mpy      y0,x0,b                                     ; 2000D8
145519: do       #<$10,>$14551f                              ; 061080 14551E
14551B: mac      y1,x1,a         y:(r4)+,y0                  ; 4EDCF2
14551C: mac      -y1,x1,b        y:(r5)+,y1                  ; 4FDDFE
14551D: mpy      y0,x0,a         a,x:(r1)+                   ; 5659D0
14551E: mpy      y0,x0,b         b,x:(r2)+                   ; 575AD8
14551F: move     #>$20,r1                                    ; 61F400 000020
145521: move     #>$50,r2                                    ; 62F400 000050
145523: move     #>$60,r4                                    ; 64F400 000060
145525: move     #>$70,r5                                    ; 65F400 000070
145527: move     x:(r0+$4),x0                                ; 021094
145528: move     x:(r0+$5),x1                                ; 0210D5
145529: move     y:(r4)+,y0                                  ; 4EDC00
14552A: move     y:(r5)+,y1                                  ; 4FDD00
14552B: mpy      -y0,x0,a                                    ; 2000D4
14552C: mpy      -y0,x0,b                                    ; 2000DC
14552D: do       #<$10,>$145533                              ; 061080 145532
14552F: mac      y1,x1,a         y:(r4)+,y0                  ; 4EDCF2
145530: mac      -y1,x1,b        y:(r5)+,y1                  ; 4FDDFE
145531: mpy      -y0,x0,a        a,x:(r1)+                   ; 5659D4
145532: mpy      -y0,x0,b        b,x:(r2)+                   ; 575ADC
145533: move     #>$60,r1                                    ; 61F400 000060
145535: move     #>$70,r2                                    ; 62F400 000070
145537: move     #>$30,r4                                    ; 64F400 000030
145539: move     #>$40,r5                                    ; 65F400 000040
14553B: move     x:(r0+$1),x0                                ; 0200D4
14553C: move     x:(r0+$2),x1                                ; 020895
14553D: move     y:(r4)+,y0                                  ; 4EDC00
14553E: move     y:(r5)+,y1                                  ; 4FDD00
14553F: mpy      y0,x0,a                                     ; 2000D0
145540: mpy      y0,x0,b                                     ; 2000D8
145541: do       #<$10,>$145547                              ; 061080 145546
145543: mac      y1,x1,a         y:(r4)+,y0                  ; 4EDCF2
145544: mac      -y1,x1,b        y:(r5)+,y1                  ; 4FDDFE
145545: mpy      y0,x0,a         a,x:(r1)+                   ; 5659D0
145546: mpy      y0,x0,b         b,x:(r2)+                   ; 575AD8
145547: move     #>$20,r0                                    ; 60F400 000020
145549: lua      (r6+$39),r1                                 ; 041E91
14554A: move     #>$145626,r2                                ; 62F400 145626
14554C: move     #>$155556,x0                                ; 44F400 155556
14554E: move     #>$0,r4                                     ; 64F400 000000
145550: do       #<$6,>$145562                               ; 060680 145561
145552: move     x:(r2)+,m3                                  ; 05DA23
145553: move     y:(r1),r3                                   ; 6BE100
145554: move     x:(r0)+,b       y:(r4)+,y0                  ; FC9800
145555: mac      y0,x0,b                                     ; 2000DA
145556: do       #<$7,>$14555c                               ; 060780 14555B
145558: move     x:(r0)+,a       y:(r4)+,y0                  ; F89800
145559: mac      y0,x0,a         b,x:(r3)+                   ; 575BD2
14555A: move     x:(r0)+,b       y:(r4)+,y0                  ; FC9800
14555B: mac      y0,x0,b         a,x:(r3)+                   ; 565BDA
14555C: move     x:(r0)+,a       y:(r4)+,y0                  ; F89800
14555D: mac      y0,x0,a         b,x:(r3)+                   ; 575BD2
14555E: move     #>$0,r4                                     ; 64F400 000000
145560: move     a,x:(r3)+                                   ; 565B00
145561: move     r3,y:(r1)+                                  ; 6B5900
145562: move     #>$ffffff,m3                                ; 05F423 FFFFFF
145564: move     #>$0,r4                                     ; 64F400 000000
145566: move     y:(r6+$1e),a                                ; 027EBE
145567: clr      b                                           ; 20001B
145568: move     #>$1062,y1                                  ; 47F400 001062
14556A: move     #>$7fef9e,y0                                ; 46F400 7FEF9E
14556C: do       #<$10,>$145573                              ; 061080 145572
14556E: move     a,x1            y:(r4)+,a                   ; 16DC00
14556F: abs      a                                           ; 200026
145570: mpy      x1,y0,a         a,x0                        ; 21C4E0
145571: mac      x0,y1,a                                     ; 2000C2
145572: maxm     a,b                                         ; 200015
145573: move     a,y:(r6+$1e)                                ; 027EAE
145574: move     y:(r6+$1f),a                                ; 027EFE
145575: move     b,y:(r6+$1f)                                ; 027EEF
145576: sub      a,b                                         ; 20001C
145577: move     y:(r6+$20),a                                ; 0286BE
145578: move     y:(r6+$25),y0                               ; 0296F6
145579: cmp      y0,a                                        ; 200055
14557A: blt      func_145581                                 ; 0D1049 000007
14557C: move     #>$7ffff0,x0                                ; 44F400 7FFFF0
14557E: move     x0,y:(r6+$20)                               ; 0286A4
14557F: bra      func_145584                                 ; 0D10C0 000005
145581: add      #>$1,a                                      ; 0140C0 000001
145583: move     a,y:(r6+$20)                                ; 0286AE
145584: move     y:(r6+$25),a                                ; 0296FE
145585: cmp      #>$7fffff,a                                 ; 0140C5 7FFFFF
145587: beq      func_145595                                 ; 0D104A 00000E
145589: cmp      #>$a8,b                                     ; 0140CD 0000A8
14558B: bge      func_145593                                 ; 0D1041 000008
14558D: move     y:(r6+$20),a                                ; 0286BE
14558E: cmp      y0,a                                        ; 200055
14558F: bge      func_14559f                                 ; 0D1041 000010
145591: bra      func_145595                                 ; 0D10C0 000004
145593: move     #$0,y1                                      ; 270000
145594: move     y1,y:(r6+$20)                               ; 0286A7
145595: move     y:(r6+$21),a                                ; 0286FE
145596: move     #>$0,r4                                     ; 64F400 000000
145598: move     #>$147ae,x0                                 ; 44F400 0147AE
14559A: rep      #<$10                                       ; 0610A0
14559B: add      x0,a            a,y:(r4)+                   ; 5E5C40
14559C: move     a,y:(r6+$21)                                ; 0286EE
14559D: bra      func_1455a8                                 ; 0D10C0 00000B
14559F: move     #>$0,r4                                     ; 64F400 000000
1455A1: move     #>$7eb852,x0                                ; 44F400 7EB852
1455A3: move     y:(r6+$21),x1                               ; 0286F5
1455A4: move     x1,a                                        ; 20AE00
1455A5: rep      #<$10                                       ; 0610A0
1455A6: mpy      x1,x0,a         a,x1            a,y:(r4)+   ; 165CA0
1455A7: move     a,y:(r6+$21)                                ; 0286EE
1455A8: move     #>$0,r4                                     ; 64F400 000000
1455AA: move     #>$0,r0                                     ; 60F400 000000
1455AC: move     r0,r1                                       ; 221100
1455AD: move     x:(r0)+,x0      y:(r4)+,y0                  ; F09800
1455AE: do       #<$10,>$1455b4                              ; 061080 1455B3
1455B0: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
1455B1: mpy      y0,x0,b         x:(r0)+,x0      y:(r4)+,y0  ; F098D8
1455B2: move     a,x:(r1)+                                   ; 565900
1455B3: move     b,x:(r1)+                                   ; 575900
1455B4: move     #>$144ac7,r2                                ; 62F400 144AC7
1455B6: move     y:(r6+$8),b                                 ; 0226BF
1455B7: asr      #$10,b,b                                    ; 0C1CA1
1455B8: move     #>$144ac7,r3                                ; 63F400 144AC7
1455BA: move     b,n3                                        ; 21FB00
1455BB: move     #>$0,r0                                     ; 60F400 000000
1455BD: move     #>$1,r1                                     ; 61F400 000001
1455BF: move     #$2,n0                                      ; 380200
1455C0: move     n0,n1                                       ; 231900
1455C1: move     #>$20,r2                                    ; 62F400 000020
1455C3: move     #$40,y1                                     ; 274000
1455C4: move     x:(r3+n3),x1                                ; 45EB00
1455C5: move     x1,b                                        ; 20AF00
1455C6: add      #>$800000,b                                 ; 0140C8 800000
1455C8: move     y:(r6+$18),a                                ; 0266BE
1455C9: move     b,y0                                        ; 21E600
1455CA: mpy      -y1,y0,b                                    ; 2000BC
1455CB: move     y:(r6+$19),a0                               ; 0266F8
1455CC: move     b,y1                                        ; 21E700
1455CD: move     y:(r6+$1a),b                                ; 026EBF
1455CE: move     y:(r6+$1b),b0                               ; 026EF9
1455CF: move     y:(r6+$1c),x0                               ; 0276B4
1455D0: mac      -x0,y1,a        a,x:(r2)+       a,y0        ; 181AC6
1455D1: mac      -x1,y0,a        x:(r0),x0                   ; 44E0E6
1455D2: mac      x0,y1,a                                     ; 2000C2
1455D3: move     y:(r6+$1d),x0                               ; 0276F4
1455D4: move     x:(r0+$1e),y0                               ; 027896
1455D5: move     y0,y:(r6+$1c)                               ; 0276A6
1455D6: move     x:(r0+$1f),y0                               ; 0278D6
1455D7: move     y0,y:(r6+$1d)                               ; 0276E6
1455D8: mac      -x0,y1,b        b,x:(r2)+       b,y0        ; 1E1ACE
1455D9: mac      -x1,y0,b        x:(r1),x0                   ; 44E1EE
1455DA: mac      x0,y1,b         x:(r0)+n0,x0                ; 44C8CA
1455DB: do       #<$f,>$1455e3                               ; 060F80 1455E2
1455DD: mac      -x0,y1,a        a,x:(r2)+       a,y0        ; 181AC6
1455DE: mac      -x1,y0,a        x:(r0),x0                   ; 44E0E6
1455DF: mac      x0,y1,a         x:(r1)+n1,x0                ; 44C9C2
1455E0: mac      -x0,y1,b        b,x:(r2)+       b,y0        ; 1E1ACE
1455E1: mac      -x1,y0,b        x:(r1),x0                   ; 44E1EE
1455E2: mac      x0,y1,b         x:(r0)+n0,x0                ; 44C8CA
1455E3: move     a1,y:(r6+$18)                               ; 0266AC
1455E4: move     a0,y:(r6+$19)                               ; 0266E8
1455E5: move     b1,y:(r6+$1a)                               ; 026EAD
1455E6: move     b0,y:(r6+$1b)                               ; 026EE9
1455E7: move     #>$144ac7,r2                                ; 62F400 144AC7
1455E9: move     y:(r6+$9),b                                 ; 0226FF
1455EA: asr      #$10,b,b                                    ; 0C1CA1
1455EB: move     #>$20,r1                                    ; 61F400 000020
1455ED: move     b,n2                                        ; 21FA00
1455EE: move     y:(r6+$14),a                                ; 0256BE
1455EF: move     y:(r6+$15),a0                               ; 0256F8
1455F0: move     #>$0,r0                                     ; 60F400 000000
1455F2: move     x:(r2+n2),y0                                ; 46EA00
1455F3: move     y:(r6+$16),b                                ; 025EBF
1455F4: move     y:(r6+$17),b0                               ; 025EF9
1455F5: move     x:(r1)+,x1                                  ; 45D900
1455F6: do       #<$10,>$1455fc                              ; 061080 1455FB
1455F8: mac      x1,y0,a         a,x:(r0)+       a,y1        ; 1918E2
1455F9: mac      -y1,y0,a        x:(r1)+,x1                  ; 45D9B6
1455FA: mac      x1,y0,b         b,x:(r0)+       b,y1        ; 1F18EA
1455FB: mac      -y1,y0,b        x:(r1)+,x1                  ; 45D9BE
1455FC: move     a1,y:(r6+$14)                               ; 0256AC
1455FD: move     a0,y:(r6+$15)                               ; 0256E8
1455FE: move     b1,y:(r6+$16)                               ; 025EAD
1455FF: move     b0,y:(r6+$17)                               ; 025EE9
145600: move     #>$0,r0                                     ; 60F400 000000
145602: move     y:(r6+$46),r1                               ; 0B76D1 000046
145604: move     y:(r6+$45),r7                               ; 0B76D7 000045
145606: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145608: move     y:(r6+$7),y0                                ; 021EF6
145609: move     y:(r6+$b),x0                                ; 022EF4
14560A: sub      y0,a                                        ; 200054
14560B: mpy      x0,x0,b                                     ; 200088
14560C: move     a,x1                                        ; 21C500
14560D: move     b,x0                                        ; 21E400
14560E: mpy      x1,x0,b                                     ; 2000A8
14560F: move     x:(r0)+,x0                                  ; 44D800
145610: mpy      y0,x0,a         x:(r1)+,x1      b,y1        ; 1799D0
145611: mac      y1,x1,a         x:(r0)+,x0                  ; 44D8F2
145612: asl      #$2,a,a                                     ; 0C1D04
145613: do       #<$10,>$14561b                              ; 061080 14561A
145615: mpy      y0,x0,b         x:(r1)+,x1                  ; 45D9D8
145616: mac      y1,x1,b         x:(r0)+,x0      a,y:(r7)+   ; B2F8FA
145617: asl      #$2,b,b                                     ; 0C1D85
145618: mpy      y0,x0,a         x:(r1)+,x1                  ; 45D9D0
145619: mac      y1,x1,a         x:(r0)+,x0      b,y:(r7)+   ; B3F8F2
14561A: asl      #$2,a,a                                     ; 0C1D04
14561B: rts                                                  ; 00000C
14561C: macr     x0,y1,b         x:(r1)+,x1      y0,y:(r5)   ; 84B9CB
14561D: ror      b               n1,y:(r1)-                  ; 79512F
14561E: cmpm     x1,b            x:(r0)-,a       y1,y:(r7)   ; 89F06F
14561F: mpyr     -x0,x0,b        x:$1c,n1                    ; 719C8D
145620: mac      -x1,y0,b        x:(r3)+n3,x1    y0,y:(r7)+n7 ; 94EBEE
145621: dc       $00002e                                     ; 00002E
145622: dc       $000042                                     ; 000042
145623: dc       $000064                                     ; 000064
145624: reset                                                ; 000094
145625: dc       $0000de                                     ; 0000DE
145626: dc       $000a76                                     ; 000A76
145627: dc       $00099e                                     ; 00099E
145628: dc       $000722                                     ; 000722
145629: dc       $000664                                     ; 000664
14562A: dc       $0007ff                                     ; 0007FF
14562B: dc       $0003ff                                     ; 0003FF
14562C: mac      -x1,x0,a        b,x1            y:(r6),b    ; 1FE6A6
14562D: or       y1,b            r5,r7                       ; 22B77A
14562E: mpy      y1,x1,b         #$88,a1                     ; 2C88F8
14562F: mpy      y0,y0,b         #$f9,b                      ; 2FF998
145630: rnd      a               #$55,r4                     ; 345511
145631: mpyr     x1,x0,b         y1,x:$31                    ; 4731A9
145632: maxm     a,b             a0,n6                       ; 211E15
145633: cmpm     x1,b            n7,a                        ; 23EE6F
145634: mac      -y0,x0,b        #$ae,b1                     ; 2DAEDE
145635: mpyr     y0,y0,b         #$14,r1                     ; 311499
145636: maxm     a,b             #$5f,r5                     ; 355F15
145637: or       y0,b            x:(r7)+n7,y1                ; 47CF5A
145638: mpyr     -y0,y0,a        r2,r5                       ; 225595
145639: mpy      x0,x0,b         #$24,x1                     ; 252488
14563A: sub      x1,a            #$d1,a                      ; 2ED164
14563B: macr     -y0,y0,b        #$2b,r2                     ; 322B9F
14563C: mpyr     -x0,x0,b        #$64,r6                     ; 36648D
14563D: mac      -x1,x0,b        a,l:(r0+n0)                 ; 4868AE
14563E: macr     y1,y0,b         n4,a1                       ; 238CBB
14563F: sub      x1,a            #$59,y0                     ; 265964
145640: or       y0,a            #$f0,b                      ; 2FF052
145641: and      y1,b            #$3e,r3                     ; 333E7E
145642: cmpm     y0,b            #$65,r7                     ; 37655F
145643: macr     -y1,y0,a        l:-(r5),a                   ; 48FDB7
145644: sub      x,a             #$c3,x0                     ; 24C324
145645: mac      x1,x0,b         #$8c,y1                     ; 278CAA
145646: sub      y1,a            #$b,r1                      ; 310B74
145647: addl     b,a             #$4d,r4                     ; 344D12
145648: cmp      y1,a            #$61,n0                     ; 386175
145649: mpyr     x0,x0,b         l:<$e,b                     ; 498E89
14564A: cmp      x1,b            #$f8,x1                     ; 25F86D
14564B: addr     b,a             #$be,a0                     ; 28BE02
14564C: macr     y0,y0,b         #$22,r2                     ; 32229B
14564D: add      y,b             #$57,r5                     ; 355738
14564E: mpyr     -y1,y0,b        #$58,n1                     ; 3958BD
14564F: adc      y,b             ab,l:<$1b                   ; 4A1B39
145650: sub      y,b             #$2c,y1                     ; 272C3C
145651: sub      a,b             #$ed,b0                     ; 29ED1C
145652: mac      -y0,y0,b        #$35,r3                     ; 33359E
145653: mac      y0,x0,a         #$5c,r6                     ; 365CD2
145654: add      x,b             #$4b,n2                     ; 3A4B28
145655: mac      y0,x0,b         l:<$23,ab                   ; 4AA3DA
145656: adc      y,b             #$5e,a0                     ; 285E39
145657: mpy      -x1,x0,b        #$19,b2                     ; 2B19AC
145658: and      y0,a            #$44,r4                     ; 344456
145659: mpy      x0,y1,b         #$5d,r7                     ; 375DC8
14565A: mpyr     x1,x0,b         #$38,n3                     ; 3B38A9
14565B: macr     x0,x0,a         ba,l:<$28                   ; 4B2883
14565C: add      b,a             #$8e,b0                     ; 298E10
14565D: or       x1,b            #$43,a1                     ; 2C436A
14565E: mpyr     x1,x0,a         #$4e,r5                     ; 354EA1
14565F: addr     b,a             #$5a,n0                     ; 385A02
145660: adc      y,b             #$21,n4                     ; 3C2139
145661: add      x0,b            l:<$29,ba                   ; 4BA948
145662: sub      y1,a            #$bb,a2                     ; 2ABB74
145663: clr      a               #$6a,b1                     ; 2D6A13
145664: or       x1,a            #$54,r6                     ; 365462
145665: cmpm     x1,b            #$51,n1                     ; 39516F
145666: mpy      y0,x0,a         #$4,n5                      ; 3D04D0
145667: rol      b               x0,y:$26                    ; 4C263F
145668: max      a,b             #$e6,b2                     ; 2BE61D
145669: tfr      x1,b            #$8d,a                      ; 2E8D69
14566A: cmpm     y1,b            #$55,r7                     ; 37557F
14566B: macr     -y1,x1,b        #$43,n2                     ; 3A43FF
14566C: sub      x1,b            #$e3,n5                     ; 3DE36C
14566D: and      y1,b            y:$1f,x0                    ; 4C9F7E
14566E: mpy      -x0,y1,a        #$d,b1                      ; 2D0DC4
14566F: sub      y,a             #$ad,b                      ; 2FAD34
145670: mpyr     x1,y0,a         #$51,n0                     ; 3851E1
145671: macr     -x1,x0,a        #$31,n3                     ; 3B31A7
145672: tst      b               #$bd,n6                     ; 3EBD0B
145673: rnd      b               x1,y:$15                    ; 4D1519
145674: sbc      x,b             #$32,a                      ; 2E322D
145675: rol      b               #$c9,r0                     ; 30C93F
145676: and      y1,a            #$49,n1                     ; 394976
145677: sub      y0,b            #$1a,n4                     ; 3C1A5C
145678: macr     -x1,x0,b        #$91,n7                     ; 3F91AF
145679: abs      a               y:$7,x1                     ; 4D8726
14567A: clr      b               #$53,b                      ; 2F531B
14567B: or       y0,b            #$e1,r1                     ; 31E15A
14567C: ror      b               #$3c,n2                     ; 3A3C2F
14567D: rnd      b               #$fe,n4                     ; 3CFE19
14567E: sub      y0,b            a10,l:(r1)                  ; 40615C
14567F: dc       $4df5bc                                     ; 4DF5BC
145680: tfr      y0,b            #$70,r0                     ; 307059
145681: tfr      y0,b            #$f5,r2                     ; 32F559
145682: macr     -y1,x1,b        #$29,n3                     ; 3B29FF
145683: macr     y0,x0,b         #$dc,n5                     ; 3DDCDB
145684: not      a               b10,l:<$2c                  ; 412C17
145685: macr     -x1,y0,b        y0,y:(r0)                   ; 4E60EF
145686: mpyr     -y1,y0,a        #$89,r1                     ; 3189B5
145687: not      a               #$5,r4                      ; 340517
145688: mac      -y0,x0,b        #$12,n4                     ; 3C12DE
145689: mpy      x1,x0,a         #$b6,n6                     ; 3EB6A0
14568A: dc       $41f1e8                                     ; 41F1E8
14568B: mpy      -y0,x0,a        y:(r0)+n0,y0                ; 4EC8D4
14568C: dc       $329f04                                     ; 329F04
14568D: cmpm     x1,b            #$10,r5                     ; 35106F
14568E: mpyr     -x0,y1,a        #$f6,n4                     ; 3CF6C5
14568F: eor      x1,b            #$8b,n7                     ; 3F8B6B
145690: mpy      y0,x0,b         l:<$32,x                    ; 42B2D8
145691: mpyr     x0,x0,a         y1,y:$2d                    ; 4F2D81
145692: sub      a,b             #$b0,r3                     ; 33B01C
145693: sub      x0,a            #$17,r6                     ; 361744
145694: mpy      y1,y0,a         #$d5,n5                     ; 3DD5B0
145695: sbc      y,b             a10,l:(r3)+                 ; 405B3D
145696: mac      y1,x1,a         y,l:(r6+n6)                 ; 436EF2
145697: tst      b               y:$f,y1                     ; 4F8F0B
145698: mac      y0,x0,b         #$bc,r4                     ; 34BCDA
145699: or       y1,b            #$19,r7                     ; 37197A
14569A: macr     -y0,y0,b        #$af,n6                     ; 3EAF9F
14569B: subl     a,b             b10,l:<$26                  ; 41261E
14569C: tfr      x0,a            x0,x:$26                    ; 442641
14569D: macr     -x0,x0,a        y:(r5+n5),y1                ; 4FED87
14569E: max      a,b             #$c5,r5                     ; 35C51D
14569F: macr     y1,x1,b         #$16,n0                     ; 3816FB
1456A0: macr     y0,y0,a         #$84,n7                     ; 3F8493
1456A1: sub      b,a             l:(r4+n4),b10               ; 41EC14
1456A2: mpy      -y0,x0,a        x:(r0)+,x0                  ; 44D8D4
1456A3: tfr      a,b             a0,x:(r1)+n1                ; 504909
1456A4: mpyr     x0,y1,b         #$c8,r6                     ; 36C8C9
1456A5: mac      y1,y0,a         #$f,n1                      ; 390FB2
1456A6: macr     -x0,x0,b        a10,l:(r4)-                 ; 40548F
1456A7: adc      x,b             l:<$2d,x                    ; 42AD29
1456A8: mac      y1,y0,b         x:$6,x1                     ; 4586BA
1456A9: mpyr     -x1,x0,a        x:$21,a0                    ; 50A1A5
1456AA: macr     -x0,y1,a        #$c7,r7                     ; 37C7C7
1456AB: mpyr     y0,y0,a         #$3,n2                      ; 3A0391
1456AC: mpy      y0,y0,b         b10,l:<$1f                  ; 411F98
1456AD: add      x1,b            y,l:(r1+n1)                 ; 436968
1456AE: tfr      b,a             y0,x:$30                    ; 463001
1456AF: dc       $50f770                                     ; 50F770
1456B0: addr     b,a             #$c2,n0                     ; 38C202
1456B1: mpyr     x0,x0,b         #$f2,n2                     ; 3AF289
1456B2: macr     -y1,y0,a        l:(r5),b10                  ; 41E5B7
1456B3: mpy      -y0,x0,b        x0,x:$20                    ; 4420DC
1456B4: mpy      -y1,y0,b        x:(r4)-,y0                  ; 46D4BC
1456B5: cmp      y1,b            b0,x:(r2)+n2                ; 514A7D
1456B6: add      x1,b            #$b7,n1                     ; 39B768
1456B7: mac      y0,y0,a         #$dc,n3                     ; 3BDC92
1456B8: mpyr     -y1,x1,a        l:<$26,x                    ; 42A6F5
1456B9: macr     y0,y0,a         x:(r3)-,x0                  ; 44D393
1456BA: dc       $4774fb                                     ; 4774FB
1456BB: macr     -y0,x0,b        x:$1a,b0                    ; 519ADF
1456BC: mpy      -x1,y0,b        #$a7,n2                     ; 3AA7EC
1456BD: mpy      -x1,x0,a        #$c1,n4                     ; 3CC1A4
1456BE: eor      y0,b            y,l:(r3)                    ; 43635B
1456BF: mpy      -y0,y0,b        x:$1,x1                     ; 45819C
1456C0: mpyr     y0,x0,a         a,l:<$10                    ; 4810D1
1456C1: mac      x1,x0,b         x:(r0+n0),b0                ; 51E8AA
1456C2: macr     x0,x0,a         #$93,n3                     ; 3B9383
1456C3: macr     y1,y0,b         #$a1,n5                     ; 3DA1BB
1456C4: macr     -y1,x1,a        x0,x:$1a                    ; 441AF7
1456C5: subr     b,a             y0,x:$2b                    ; 462B06
1456C6: tfr      y0,a            l:<$28,a                    ; 48A851
1456C7: mpyr     y1,x1,a         a2,x:$33                    ; 5233F1
1456C8: abs      a               #$7a,n4                     ; 3C7A26
1456C9: mac      -y0,x0,a        #$7c,n6                     ; 3E7CD6
1456CA: mpyr     -y0,x0,a        x:(r5)+n5,x0                ; 44CDD5
1456CB: macr     x1,y0,a         x:(r7)+n7,y0                ; 46CFE3
1456CC: mac      -x0,x0,b        b,l:<$3b                    ; 493B8E
1456CD: mac      -x0,y1,a        a2,x:-(r4)                  ; 527CC6
1456CE: mac      -x0,y1,b        #$5b,n5                     ; 3D5BCE
1456CF: mac      -y1,x1,a        #$52,n7                     ; 3F52F6
1456D0: cmp      b,a             x1,x:-(r4)                  ; 457C05
1456D1: sub      x0,a            y1,x:>$49ca9d               ; 477044 49CA9D
1456D3: asl      b               x:(r3)-n3,a2                ; 52C33A
1456D4: eor      y1,b            #$38,n6                     ; 3E387B
1456D5: sub      a,b             a10,l:<$24                  ; 40241C
1456D6: mac      -y0,y0,a        y0,x:$25                    ; 462596
1456D7: lsl      b               a,l:<$c                     ; 480C3B
1456D8: macr     y0,y0,a         ab,l:(r5)-                  ; 4A5593
1456D9: tfr      x1,a            b2,x:$7                     ; 530761
1456DA: lsr      b               #$10,n7                     ; 3F102B
1456DB: cmpm     x0,b            l:?:>$46ca98,a10            ; 40F04F 46CA98
1456DD: mpy      -y0,x0,b        l:<$23,a                    ; 48A3DC
1456DE: macr     x0,x0,a         l:(r4)+,ab                  ; 4ADC83
1456DF: or       x0,b            b2,x:(r1)+n1                ; 53494A
1456E0: mpyr     x1,y0,a         #$e2,n7                     ; 3FE2E1
1456E1: mpyr     -y0,y0,a        l:<$37,b10                  ; 41B795
1456E2: subl     a,b             y1,x:(r3+n3)                ; 476B1E
1456E3: asl      b               b,l:<$37                    ; 49373A
1456E4: mpy      -x0,x0,a        ba,l:(r7)+                  ; 4B5F84
1456E5: cmpm     b,a             x:$9,b2                     ; 538907
1456E6: mac      x1,x0,a         l:<$30,a10                  ; 40B0A2
1456E7: mpy      y1,x1,b         x,l:-(r1)                   ; 4279F8
1456E8: asl      b               a,l:<$7                     ; 48073A
1456E9: tfr      x1,b            l:(r6)-n6,b                 ; 49C669
1456EA: mac      x1,x0,b         l:(r6)+,ba                  ; 4BDEAA
1456EB: mpyr     x1,x0,b         x:(r6)-n6,b2                ; 53C6A9
1456EC: sub      y1,a            b10,l:-(r1)                 ; 417974
1456ED: mpy      x0,x0,a         y,l:<$37                    ; 433780
1456EE: macr     -y1,x1,b        l:<$1e,a                    ; 489EFF
1456EF: cmp      y1,b            ab,l:(r1)-                  ; 4A517D
1456F0: tst      b               x0,y:(r2)+                  ; 4C5A0B
1456F1: rol      b               a1,x:$2                     ; 54023F
1456F2: and      y0,b            x,l:<$3d                    ; 423D5E
1456F3: lsl      b               l:?:>$493280,y              ; 43F03B 493280
1456F5: mpy      -x0,x0,b        l:(r0)+,ab                  ; 4AD88C
1456F6: mpy      -y1,y0,b        y:(r1)-,x0                  ; 4CD1BC
1456F7: macr     y0,x0,b         a1,x:$3b                    ; 543BDB
1456F8: sub      x1,b            l:-(r4),x                   ; 42FC6C
1456F9: sbc      y,a             x:$24,x0                    ; 44A435
1456FA: mpyr     y0,x0,a         l:(r1)-n1,b                 ; 49C1D1
1456FB: macr     x1,x0,b         ba,l:(r3)+                  ; 4B5BAB
1456FC: mpy      -y0,x0,a        x1,y:(r5)-n5                ; 4D45D4
1456FD: dc       $54738a                                     ; 54738A
1456FE: mpyr     x1,x0,b         l:<$36,y                    ; 43B6A9
1456FF: cmp      y1,b            x1,x:(r3)-                  ; 45537D
145700: cmpm     b,a             ab,l:(r5)+n5                ; 4A4D07
145701: mac      -x1,y0,b        l:(r2)+,ba                  ; 4BDAEE
145702: and      x1,a            y:$36,x1                    ; 4DB666
145703: cmp      y0,b            x:$29,a1                    ; 54A95D
145704: add      x,a             x0,x:(r4+n4)                ; 446C20
145705: lsr      a               x:-(r6),x1                  ; 45FE23
145706: rol      a               l:(r4)-,ab                  ; 4AD437
145707: eor      x1,b            x0,y:(r6)-                  ; 4C566B
145708: mac      x0,x0,b         y0,y:$23                    ; 4E238A
145709: tfr      x1,a            x:(r5)+,a1                  ; 54DD61
14570A: mpy      x1,y0,a         x1,x:$1c                    ; 451CE0
14570B: neg      a               x:$24,y0                    ; 46A436
14570C: and      y1,a            ba,l:(r7)-                  ; 4B5776
14570D: add      y,b             y:(r6)+n6,x0                ; 4CCE38
14570E: sub      y0,a            y:$d,y0                     ; 4E8D54
14570F: mac      -x1,x0,a        b1,x:$f                     ; 550FA6
145710: mpyr     y1,x1,b         x:(r0)+n0,x1                ; 45C8F9
145711: mpy      x0,y1,b         y1,x:(r5)-n5                ; 4745C8
145712: mpyr     y0,x0,b         l:(r6)-,ba                  ; 4BD6D9
145713: eor      x1,b            x1,y:(r2)-n2                ; 4D426B
145714: dc       $4ef3da                                     ; 4EF3DA
145715: adc      y,b             b1,x:(r0)-n0                ; 554039
145716: or       y1,b            y0,x:>$47e2ec               ; 46707A 47E2EC
145718: and      y1,a            x0,y:(r2)-                  ; 4C5276
145719: add      a,b             y:$33,x1                    ; 4DB318
14571A: adc      y,a             y1,y:(r7)-                  ; 4F5731
14571B: adc      x,b             b1,x:(r7+n7)                ; 556F29
14571C: cmp      y1,a            y1,x:$13                    ; 471375
14571D: mpy      -y1,y0,a        a,l:-(r3)                   ; 487BB4
14571E: tfr      x1,a            y:(r2)+n2,x0                ; 4CCA61
14571F: and      y0,a            y0,y:$20                    ; 4E2056
145720: cmp      x1,b            y:$37,y1                    ; 4FB76D
145721: mpyr     x0,x0,a         x:$1c,b1                    ; 559C81
145722: mpy      -y1,x1,b        x:$31,y1                    ; 47B1FC
145723: sub      y,a             b,l:<$10                    ; 491034
145724: mpyr     y1,y0,a         x1,y:$3e                    ; 4D3EB1
145725: adc      y,b             y:$a,y0                     ; 4E8A39
145726: macr     x1,x0,a         a0,x:$14                    ; 5014A3
145727: add      y0,a            x:(r0)+n0,b1                ; 55C850
145728: adc      x,a             a,l:(r4)+n4                 ; 484C21
145729: cmpm     y1,b            l:<$20,b                    ; 49A07F
14572A: sub      y1,b            y:$2f,x1                    ; 4DAF7C
14572B: macr     -y0,x0,a        y:>$506ee9,y0               ; 4EF0D7 506EE9
14572D: dc       $55f2a1                                     ; 55F2A1
14572E: macr     -y1,x1,a        l:(r1),a                    ; 48E1F7
14572F: macr     x1,x0,b         ab,l:<$2c                   ; 4A2CAB
145730: mac      -y0,x0,a        y0,y:$1c                    ; 4E1CD6
145731: and      x0,a            y1,y:(r4)-                  ; 4F5446
145732: tfr      y0,a            x:(r6)-n6,a0                ; 50C651
145733: mac      x0,x0,a         a,x:$1b                     ; 561B82
145734: dc       $497393                                     ; 497393
145735: macr     x0,y1,b         l:<$34,ab                   ; 4AB4CB
145736: mpy      -y0,x0,a        y:$6,y0                     ; 4E86D4
145737: mac      y0,y0,b         y:$34,y1                    ; 4FB49A
145738: mpy      y1,x1,a         b0,x:$1a                    ; 511AF0
145739: mpyr     -y1,x1,b        a,x:(r2)-n2                 ; 5642FD
14573A: dc       $4a0108                                     ; 4A0108
14573B: mpyr     -y1,x1,a        ba,l:<$38                   ; 4B38F5
14573C: mac      -x0,x0,b        y:(r5+n5),y0                ; 4EED8E
14573D: macr     -x1,y0,a        a0,x:$11                    ; 5011E7
14573E: mac      y0,x0,b         b0,x:(r4+n4)                ; 516CDA
14573F: subl     a,b             a,x:(r1+n1)                 ; 56691E
145740: sub      x1,b            l:<$a,ab                    ; 4A8A6C
145741: rol      b               l:<$39,ba                   ; 4BB93F
145742: not      a               y1,y:(r1)-                  ; 4F5117
145743: or       x0,a            a0,x:(r4+n4)                ; 506C42
145744: adc      x,a             x:$3c,b0                    ; 51BC21
145745: mac      y1,x1,a         x:$d,a                      ; 568DF2
145746: macr     y0,x0,a         ba,l:<$f                    ; 4B0FD3
145747: mpy      -y1,y0,b        x0,y:$35                    ; 4C35BC
145748: mpy      -x0,x0,a        y:$31,y1                    ; 4FB184
145749: mpy      x0,y1,a         x:(r3)-n3,a0                ; 50C3C0
14574A: mpyr     y0,x0,b         a2,x:$8                     ; 5208D9
14574B: mpyr     x0,x0,a         x:$31,a                     ; 56B181
14574C: or       y0,a            l:<$11,ba                   ; 4B9152
14574D: mpy      -x0,x0,a        y:$2e,x0                    ; 4CAE84
14574E: mac      x1,y0,b         a0,x:$e                     ; 500EEA
14574F: sub      y1,a            b0,x:$18                    ; 511874
145750: sub      b,a             a2,x:(r3)-                  ; 525314
145751: mpy      y0,x0,b         x:(r3)-,a                   ; 56D3D8
145752: macr     -y1,x1,b        x0,y:$e                     ; 4C0EFF
145753: macr     x1,x0,b         x1,y:$23                    ; 4D23AB
145754: cmp      y0,b            a0,x:(r1+n1)                ; 50695D
145755: or       y1,a            b0,x:(r2+n2)                ; 516A72
145756: mpyr     -x1,y0,a        x:$1a,a2                    ; 529AE5
145757: dc       $56f500                                     ; 56F500
145758: macr     -x1,y0,b        y:$8,x0                     ; 4C88EF
145759: add      x0,b            y:$15,x1                    ; 4D9548
14575A: macr     y1,x1,a         x:(r0)-n0,a0                ; 50C0F3
14575B: mpyr     -x0,y1,b        x:$39,b0                    ; 51B9CD
14575C: and      y0,b            x:(r0),a2                   ; 52E05E
14575D: dc       $571504                                     ; 571504
14575E: add      y,b             y:-(r7),x0                  ; 4CFF38
14575F: cmpm     x1,b            y0,y:$3                     ; 4E036F
145760: mac      -y1,y0,b        b0,x:$15                    ; 5115BE
145761: mpyr     y0,y0,b         a2,x:$6                     ; 520699
145762: macr     -x0,x0,b        b2,x:$23                    ; 53238F
145763: mpyr     -x1,y0,b        b,x:$33                     ; 5733ED
145764: dc       $4d71ef                                     ; 4D71EF
145765: neg      a               y0,y:(r6+n6)                ; 4E6E36
145766: mac      y0,x0,a         b0,x:(r7)                   ; 5167D2
145767: macr     -x1,y0,a        a2,x:(r0)-                  ; 5250E7
145768: macr     x0,x0,b         b2,x:(r4)                   ; 53648B
145769: mpy      -x0,y1,a        b,x:(r1)-                   ; 5751C4
14576A: asr      b               y:(r1),x1                   ; 4DE12A
14576B: mac      y1,y0,a         y:(r5)-,y0                  ; 4ED5B2
14576C: eor      x0,a            x:$37,b0                    ; 51B743
14576D: mac      x0,y1,b         x:$18,a2                    ; 5298CA
14576E: or       x1,a            x:$23,b2                    ; 53A362
14576F: mac      y0,y0,a         b,x:(r6+n6)                 ; 576E92
145770: mac      -y1,x1,b        y0,y:(r4)+n4                ; 4E4CFE
145771: mpyr     y1,x1,b         y1,y:$39                    ; 4F39F9
145772: lsr      a               a2,x:$4                     ; 520423
145773: sub      y0,a            x:(r6)+,a2                  ; 52DE54
145774: sub      x,a             x:(r0),b2                   ; 53E024
145775: tfr      x1,a            x:$a,b                      ; 578A61
145776: mpyr     x0,x0,a         y:$35,y0                    ; 4EB581
145777: not      b               y:$1b,y1                    ; 4F9B1F
145778: mac      -x0,x0,a        a2,x:(r6)+n6                ; 524E86
145779: macr     -y0,y0,a        b2,x:$21                    ; 532197
14577A: mac      x1,y0,a         a1,x:$1a                    ; 541AE2
14577B: adc      y,b             x:$25,b                     ; 57A539
14577C: mpy      x0,y1,b         y1,y:$1a                    ; 4F1AC8
14577D: asl      b               y:-(r1),y1                  ; 4FF93A
14577E: cmp      y1,b            x:$16,a2                    ; 52967D
14577F: macr     x1,x0,a         b2,x:(r2)                   ; 5362A3
145780: macr     x1,x0,b         a1,x:(r3)-                  ; 5453AB
145781: asr      a               x:$3f,b                     ; 57BF22
145782: mpy      x1,y0,b         y1,y:-(r4)                  ; 4F7CE8
145783: cmp      y0,b            a0,x:(r4)-                  ; 50545D
145784: addl     a,b             x:(r4)+,a2                  ; 52DC1A
145785: mac      x0,x0,b         x:$21,b2                    ; 53A18A
145786: macr     -x0,x0,b        x:$a,a1                     ; 548A8F
145787: sub      x,a             x:(r0)+,b                   ; 57D824
145788: mpyr     -y1,x1,a        y:(r3)+,y1                  ; 4FDBF5
145789: mpyr     -y0,y0,b        x:$2c,a0                    ; 50AC9D
14578A: add      y1,a            b2,x:$1f                    ; 531F70
14578B: sub      y0,b            x:(r6)+,b2                  ; 53DE5C
14578C: mpy      -y0,y0,b        x:$3f,a1                    ; 54BF9C
14578D: cmpm     x0,a            x:>$503805,b                ; 57F047 503805
14578F: subr     a,b             b0,x:$2                     ; 51020E
145790: mac      -x0,x0,b        b2,x:(r0)                   ; 53608E
145791: adc      x,b             a1,x:$19                    ; 541929
145792: dc       $54f2e1                                     ; 54F2E1
145793: macr     y0,y0,a         a0,y:$7                     ; 580793
145794: sub      x,b             x:$11,a0                    ; 50912C
145795: mpy      -x0,y1,a        b0,x:(r4)-                  ; 5154C4
145796: macr     -x0,x0,a        x:$1f,b2                    ; 539F87
145797: tfr      b,a             a1,x:(r2)-                  ; 545201
145798: cmp      x1,b            b1,x:$24                    ; 55246D
145799: subr     a,b             a0,y:$1e                    ; 581E0E
14579A: cmp      y1,b            x:(r7),a0                   ; 50E77D
14579B: mac      y0,x0,a         x:$24,b0                    ; 51A4D2
14579C: or       x1,b            x:(r4)+,b2                  ; 53DC6A
14579D: mac      y1,x1,a         x:$8,a1                     ; 5488F2
14579E: sub      x0,b            b1,x:(r4)-                  ; 55544C
14579F: mpy      x0,y1,a         a0,y:$33                    ; 5833C0
1457A0: dc       $513b0c                                     ; 513B0C
1457A1: dc       $51f24b                                     ; 51F24B
1457A2: cmpm     x0,a            a1,x:$17                    ; 541747
1457A3: cmp      a,b             x:$3e,a1                    ; 54BE0D
1457A4: mac      -x0,x0,b        x:$2,b1                     ; 55828E
1457A5: mpyr     y1,y0,a         a0,y:(r0)+n0                ; 5848B1
1457A6: mac      -x1,y0,b        x:$b,b0                     ; 518BEE
1457A7: or       x0,a            a2,x:$3d                    ; 523D42
1457A8: ror      b               a1,x:(r0)-                  ; 54502F
1457A9: dc       $54f160                                     ; 54F160
1457AA: neg      b               x:$2f,b1                    ; 55AF3E
1457AB: mpyr     -x1,y0,a        a0,y:(r4)+                  ; 585CE5
1457AC: nop                                                  ; 000000
1457AD: nop                                                  ; 000000
1457AE: nop                                                  ; 000000
1457AF: nop                                                  ; 000000
1457B0: nop                                                  ; 000000
1457B1: nop                                                  ; 000000
1457B2: dc       $7ff599                                     ; 7FF599
1457B3: dc       $7ff671                                     ; 7FF671
1457B4: mpyr     -x1,y0,b        y:-(r0),n7                  ; 7FF8ED
1457B5: macr     x1,x0,b         y:-(r1),n7                  ; 7FF9AB
1457B6: mpyr     x0,x0,b         y:-(r2),n7                  ; 7FFA89
1457B7: macr     y0,y0,b         y:-(r5),n7                  ; 7FFD9B
1457B8: macr     -y1,y0,b        x:$37,n7                    ; 77B7BF
1457B9: cmp      y1,b            n0,y:(r6)+                  ; 785E7D
1457BA: mpy      -y1,y0,b        n2,y:(r6)+n6                ; 7A4EBC
1457BB: macr     -x0,x0,a        y:(r4),n2                   ; 7AE487
1457BC: cmp      y1,a            y:$14,n3                    ; 7B9475
1457BD: eor      x0,b            n6,y:$b                     ; 7E0B4B
1457BE: mpyr     -y1,y0,b        n0,x:$1                     ; 7001BD
1457BF: mpyr     y0,x0,b         n1,x:$39                    ; 7139D9
1457C0: mpy      -y1,y0,a        x:(r4),n4                   ; 74E4B4
1457C1: dc       $760308                                     ; 760308
1457C2: cmpm     a,b             n7,x:(r5)-                  ; 77550F
1457C3: mpyr     -x0,x0,b        n4,y:$20                    ; 7C208D
1457C4: mpy      -y0,x0,a        y:(r2)+n2,r0                ; 68CAD4
1457C5: mac      -x1,x0,b        y:$1,r2                     ; 6A81AE
1457C6: mac      -y1,x1,b        y:$37,r7                    ; 6FB7FE
1457C7: asr      a               n1,x:(r3)-                  ; 715322
1457C8: tst      a               n3,x:$3b                    ; 733B03
1457C9: or       x0,a            n2,y:$3d                    ; 7A3D42
1457CA: macr     -y0,x0,a        r2,x:$a                     ; 620AD7
1457CB: macr     -x0,x0,b        r4,x:$2f                    ; 642F8F
1457CC: mpyr     -x1,y0,a        y:(r5)-n5,r2                ; 6AC5E5
1457CD: mpy      -y0,x0,b        y:(r2)-,r4                  ; 6CD2DC
1457CE: cmpm     b,a             r7,y:(r5)-n5                ; 6F4507
1457CF: cmp      x0,b            n0,y:(r1)                   ; 78614D
1457D0: subl     a,b             y:$3a,b2                    ; 5BBA1E
1457D1: cmpm     x1,b            a,y:$3d                     ; 5E3D6F
1457D2: mpyr     y0,x0,a         r6,x:$b                     ; 660BD1
1457D3: sub      y0,a            y:$0,r0                     ; 688054
1457D4: dc       $6b71e1                                     ; 6B71E1
1457D5: macr     y0,y0,a         x:$c,n6                     ; 768C93
1457D6: cmpm     y1,b            x:(r1)-,b1                  ; 55D17F
1457D7: macr     y0,y0,b         y:$25,a0                    ; 58A59B
1457D8: cmpm     x0,a            x:$7,r1                     ; 618747
1457D9: mpy      y1,y0,b         r4,x:(r1)+                  ; 6459B8
1457DA: sub      y0,b            x:(r0)-n0,r7                ; 67C05C
1457DB: macr     -y1,x1,a        x:$3e,n4                    ; 74BEF7
1457DC: cmpm     x0,a            a0,x:(r2)+n2                ; 504A47
1457DD: mpyr     y1,y0,b         b2,x:(r2)                   ; 5362B9
1457DE: macr     x1,y0,b         b1,y:$35                    ; 5D35EB
1457DF: eor      x0,b            r0,x:(r5)+                  ; 605D4B
1457E0: eor      y0,a            r4,x:$2f                    ; 642F53
1457E1: and      y0,b            x:-(r0),n2                  ; 72F85E
1457E2: asl      a               ba,l:<$1e                   ; 4B1E32
1457E3: macr     -y1,y0,b        y0,y:(r7+n7)                ; 4E6FBF
1457E4: cmpm     y1,a            b0,y:$15                    ; 591577
1457E5: tfr      x1,a            y:$9,a1                     ; 5C8961
1457E6: macr     -x1,x0,a        x:$3d,r0                    ; 60BDA7
1457E7: mpy      -x1,x0,b        n1,x:$38                    ; 7138AC
1457E8: add      x1,a            y0,x:(r7)-n7                ; 464760
1457E9: mpyr     y1,x1,a         l:(r7)-n7,b                 ; 49C7F1
1457EA: mpy      -x0,y1,a        b1,x:$23                    ; 5523C4
1457EB: sub      y0,b            y:(r4)+,a0                  ; 58DC5C
1457EC: sub      x0,a            b1,y:(r2+n2)                ; 5D6A44
1457ED: mac      -x0,y1,a        r7,y:-(r7)                  ; 6F7FC6
1457EE: cmp      y0,a            l:(r0)-n0,b10               ; 41C055
1457EF: mpyr     y0,x0,b         x1,x:(r6)                   ; 4566D9
1457F0: mac      -y1,y0,b        b0,x:(r6)+                  ; 515EBE
1457F1: mpy      -y1,y0,a        b1,x:(r4)-                  ; 5554B4
1457F2: adc      x,a             a2,y:$34                    ; 5A3421
1457F3: macr     y0,y0,a         y:(r5)+n5,r5                ; 6DCD93
1457F4: macr     -x1,y0,b        #$83,n5                     ; 3D83EF
1457F5: cmp      x0,a            b10,l:(r0)+n0               ; 414845
1457F6: cmp      x1,b            y:(r4)-n4,x1                ; 4DC46D
1457F7: mpy      -x1,y0,b        x:>$571a3d,b0               ; 51F0EC 571A3D
1457F9: macr     -y1,x1,a        r4,y:$21                    ; 6C21F7
1457FA: cmpm     y0,b            #$8d,n1                     ; 398D5F
1457FB: eor      x0,a            #$68,n5                     ; 3D6843
1457FC: mpy      -x1,y0,b        ab,l:(r2)-                  ; 4A52EC
1457FD: macr     -y0,y0,a        y:$2f,y0                    ; 4EAF97
1457FE: macr     -y0,y0,b        a1,x:$1b                    ; 541B9F
1457FF: macr     y0,x0,b         r2,y:-(r4)                  ; 6A7CDB
145800: abs      a               #$d8,r5                     ; 35D826
145801: subl     a,b             #$c3,n1                     ; 39C31E
145802: and      x1,b            y1,x:$8                     ; 47086E
145803: tfr      y0,b            l:<$f,ba                    ; 4B8F59
145804: add      y0,b            b0,x:$37                    ; 513758
145805: lsr      a               y:(r6)+,r0                  ; 68DE23
145806: cmpm     a,b             #$60,r2                     ; 32600F
145807: add      y0,b            #$55,r6                     ; 365558
145808: asl      b               l:(r3),y                    ; 43E33A
145809: mpy      x1,y0,a         l:<$e,a                     ; 488EE0
14580A: mpy      x0,x0,a         y0,y:(r4+n4)                ; 4E6C80
14580B: mpy      y1,y0,b         r7,x:(r5)-n5                ; 6745B8
14580C: lsr      b               #$21,b                      ; 2F212B
14580D: macr     -x1,x0,a        #$1b,r3                     ; 331BA7
14580E: macr     -x1,x0,a        l:(r1),a10                  ; 40E1A7
14580F: mac      x1,y0,b         x:$2c,x1                    ; 45ACEA
145810: rol      a               l:<$3a,ba                   ; 4BBA37
145811: mpyr     x0,x0,a         x:$33,r5                    ; 65B381
145812: macr     x0,y1,b         #$17,a1                     ; 2C17CB
145813: mpyr     -y1,x1,a        #$12,r0                     ; 3012F5
145814: sbc      x,a             #$2,n6                      ; 3E0225
145815: tfr      x0,a            l:(r0+n0),x                 ; 42E841
145816: mac      -x1,x0,a        b,l:<$1f                    ; 491FA6
145817: cmpm     x1,a            r4,x:$27                    ; 642767
145818: sub      y1,b            #$40,b0                     ; 29407C
145819: eor      y0,b            #$38,b1                     ; 2D385B
14581A: ror      b               #$43,n3                     ; 3B432F
14581B: mpy      -y1,y0,b        a10,l:<$3f                  ; 403FBC
14581C: mpy      -y1,x1,b        x:$1b,y0                    ; 469BFC
14581D: add      y0,a            x:$21,r2                    ; 62A150
14581E: cmpm     b,a             #$98,y0                     ; 269807
14581F: clr      b               #$89,a2                     ; 2A891B
145820: cmpm     y0,a            #$a3,n0                     ; 38A357
145821: sub      y,b             #$b2,n5                     ; 3DB23C
145822: add      y1,a            x0,x:$2e                    ; 442E70
145823: abs      a               r1,x:$21                    ; 612126
145824: cmp      x1,a            #$1b,x0                     ; 241B65
145825: mpy      -x1,x0,a        #$2,a0                      ; 2802A4
145826: lsl      b               #$21,r6                     ; 36213B
145827: macr     -x1,x0,b        #$3e,n3                     ; 3B3EAF
145828: rol      b               l:(r6)-,b10                 ; 41D63F
145829: macr     y0,x0,a         y:$26,b                     ; 5FA6D3
14582A: mac      -x0,y1,a        a,y1                        ; 21C7C6
14582B: mac      x0,x0,b         #$a2,x1                     ; 25A28A
14582C: macr     x0,x0,b         #$bb,r3                     ; 33BB8B
14582D: subr     a,b             #$e4,n0                     ; 38E40E
14582E: macr     -x1,x0,b        #$92,n7                     ; 3F92AF
14582F: neg      b               a,y:$32                     ; 5E323E
145830: mpy      -x0,x0,a        x:(r2)+,b       b,y1        ; 1F9A84
145831: macr     -x0,x0,a        n3,y0                       ; 236687
145832: dc       $317104                                     ; 317104
145833: eor      y0,b            #$a1,r6                     ; 36A15B
145834: dc       $3d6308                                     ; 3D6308
145835: eor      y0,a            y:(r3)-n3,a1                ; 5CC353
145836: adc      x,b             x:(r1)-,b       a,y1        ; 1D9129
145837: cmpm     y1,a            a2,a1                       ; 214C77
145838: sub      y1,a            #$40,b                      ; 2F4074
145839: macr     x1,x0,a         #$75,r4                     ; 3475A3
14583A: mpyr     -y0,y0,b        #$46,n3                     ; 3B469D
14583B: mac      y1,x1,b         b2,y:(r1)+                  ; 5B59FA
14583C: or       x1,a            x:(r1+n1),a     b,y1        ; 1BA962
14583D: and      y0,a            b,x1            b,y:(r2)-   ; 1F5256
14583E: mpyr     -y1,y0,a        #$28,b1                     ; 2D28B5
14583F: mpy      -y1,x1,b        #$5f,r2                     ; 325FFC
145840: mpy      -x0,y1,a        #$3c,n1                     ; 393CC4
145841: dc       $59f61f                                     ; 59F61F
145842: dc       $19e108                                     ; 19E108
145843: dc       $1d763f                                     ; 1D763F
145844: mpy      -x1,x0,b        #$28,b2                     ; 2B28AC
145845: macr     -x0,x0,a        #$5f,r0                     ; 305F87
145846: macr     y0,x0,b         #$44,r7                     ; 3744DB
145847: mpy      -x1,x0,b        y:$17,a0                    ; 5897AC
145848: dc       $183614                                     ; 183614
145849: dc       $1bb669                                     ; 1BB669
14584A: and      x0,b            #$3f,b0                     ; 293F4E
14584B: sub      x1,b            #$73,a                      ; 2E736C
14584C: or       x0,a            #$5e,r5                     ; 355E42
14584D: macr     x0,x0,b         b,x:$3e                     ; 573E8B
14584E: mac      x1,x0,a         x:(r6),x1       b,y0        ; 16A6A2
14584F: adc      x,b             a,x:(r1)-       b,y0        ; 1A1129
145850: mac      y0,y0,b         #$6b,y1                     ; 276B9A
145851: mpy      -y0,x0,b        #$9a,a1                     ; 2C9ADC
145852: eor      x1,a            #$88,r3                     ; 338863
145853: mpyr     x1,x0,b         x:(r2+n2),b1                ; 55EAA9
145854: macr     x1,y0,b         x1,x:>$1884e9   a,y1        ; 1530EB 1884E9
145856: macr     y0,y0,b         #$ac,x1                     ; 25AC9B
145857: rnd      a               #$d5,a2                     ; 2AD511
145858: mac      x1,x0,b         #$c2,r1                     ; 31C2AA
145859: mpyr     y1,x1,a         x:$1b,a1                    ; 549BF1
14585A: add      x0,b            a,x0            y:(r3)-,b   ; 13D348
14585B: abs      b               x1,x:(r0)-      b,y1        ; 17102E
14585C: cmp      x1,a            #$1,x0                      ; 240165
14585D: sub      x0,b            #$21,b0                     ; 29214C
14585E: mpyr     x0,x0,b         #$c,r0                      ; 300C89
14585F: cmpm     x0,b            b2,x:(r2)-                  ; 53524F
145860: sub      x,b             x:(r4)+n4,x0    b,y0        ; 128C2C
145861: dc       $15b194                                     ; 15B194
145862: rnd      b               r3,b0                       ; 226919
145863: mac      -y0,x0,a        #$7e,y1                     ; 277ED6
145864: and      y1,a            #$65,a                      ; 2E6576
145865: macr     -x1,x0,b        a2,x:$d                     ; 520DAF
145866: lsr      a               a,x0            y1,y:(r2)+  ; 115A23
145867: mpyr     x0,y1,b         a,x1            y0,y:(r7)   ; 1467C9
145868: dc       $20e2e1                                     ; 20E2E1
145869: macr     -y1,x1,b        #$ec,x1                     ; 25ECFF
14586A: macr     -x1,y0,b        #$cc,a1                     ; 2CCCEF
14586B: mac      -y1,x1,b        x:(r5)+n5,a0                ; 50CDFE
14586C: mac      y0,x0,a         x0,x:-(r3)      a,y0        ; 103BD2
14586D: dc       $133191                                     ; 133191
14586E: mpy      y1,x1,a         b,x1            b,y:(r5+n5) ; 1F6DF0
14586F: subl     a,b             #$6b,x0                     ; 246B1E
145870: or       y1,a            #$42,b2                     ; 2B4272
145871: adc      x,b             y:$13,y1                    ; 4F9329
145872: jsne     func_000ff3                                 ; 0F2FF3
145873: mpy      -x0,y1,a        x0,x:(r5)+n5    b,y0        ; 120DC4
145874: mac      x0,x0,a         b,x:(r1)+n1     b,y0        ; 1E0982
145875: mpyr     y0,y0,a         r7,n0                       ; 22F891
145876: mpy      -x0,x0,a        #$c5,b0                     ; 29C584
145877: sub      a,b             y0,y:(r5)+                  ; 4E5D1C
145878: jpl      func_000557                                 ; 0E3557
145879: or       x0,b            a,x0            y:-(r3),y0  ; 10FB4A
14587A: macr     y0,x0,b         #>$2194bb,b     a,y0        ; 1CB4DB 2194BB
14587C: macr     -x1,x0,b        #$55,a0                     ; 2855AF
14587D: mpyr     -x0,y1,a        x1,y:$2b                    ; 4D2BC5
14587E: dc       $0d4ae1                                     ; 0D4AE1
14587F: jsle     func_00091b                                 ; 0FF91B
145880: or       x0,b            b,x0            b,y:(r7+n7) ; 1B6F4A
145881: tfr      a,b             ifle.u                      ; 203F09
145882: and      y1,b            #$f2,y0                     ; 26F27E
145883: addl     b,a             l:-(r7),ba                  ; 4BFF12
145884: dc       $0c6f87                                     ; 0C6F87
145885: jscc     func_000640                                 ; 0F0640
145886: sub      x,a             a,x:-(r0)       b,y0        ; 1A3824
145887: dc       $1ef6e9                                     ; 1EF6E9
145888: macr     x0,x0,a         #$9b,x1                     ; 259B83
145889: mpyr     y1,x1,a         l:(r6)-,ab                  ; 4AD6F1
14588A: bchg     #$f,y:<<$ffffe2                             ; 0BA24F
14588B: jne      func_0001cf                                 ; 0E21CF
14588C: mpy      -x0,y1,a        a,x:(r6)+n6     a,y1        ; 190EC4
14588D: mac      y0,x0,a         x:-(r3),b       a,y1        ; 1DBBD2
14588E: or       y0,a            #$50,x0                     ; 245052
14588F: add      y0,a            l:<$33,b                    ; 49B350
145890: bclr     #$10,m2                                     ; 0AE250
145891: dc       $0d4aef                                     ; 0D4AEF
145892: dc       $17f28f                                     ; 17F28F
145893: rol      b               x:(r5)+n5,b     a,y0        ; 1C8D3F
145894: mpy      -x0,x0,a        n0,r0                       ; 231084
145895: max      a,b             l:<$14,a                    ; 48941D
145896: jset     #$11,x:$2e,func_0c80d1                      ; 0A2EB1 0C80D1
145898: mpy      y1,x1,a         a,x1            y:(r2),a    ; 16E2F0
145899: mpyr     y1,y0,a         b,x0            b,y:(r2+n2) ; 1B6AB1
14589A: mpy      -y1,y0,a        a,n3                        ; 21DBB4
14589B: add      x0,b            y1,x:-(r1)                  ; 477948
14589C: mpyr     -x1,x0,a        y0,b            b,y:(r6)-n6 ; 0986A5
14589D: dc       $0bc2b2                                     ; 0BC2B2
14589E: tfr      y0,b            a,x1            y:(r7)+,y1  ; 15DF59
14589F: macr     -x1,x0,b        b,x0            a,y:(r3)-   ; 1A53AF
1458A0: mac      x0,x0,a         x1,r1                       ; 20B182
1458A1: macr     -y1,y0,b        y0,x:(r2)                   ; 4662BF
1458A2: movep    p:(r1+n1),x:<<$ffffee                       ; 08E96E
1458A3: dc       $0b0fde                                     ; 0B0FDE
1458A4: add      x0,a            a,x1            y:(r7),y0   ; 14E740
1458A5: mac      x0,y1,a         b,x0            y1,y:(r7)-n7 ; 1947C2
1458A6: mpyr     y0,y0,a         x:(r1)-,b       b,y1        ; 1F9191
1458A7: or       y1,a            x1,x:(r0)-                  ; 455072
1458A8: movep    x:<<$ffffd8,p:(r6)-                         ; 085658
1458A9: jset     #$8,x:(r7),func_13fa24                      ; 0A67A8 13FA24
1458AB: eor      y1,b            b,x0            y0,y:(r6)-n6 ; 18467B
1458AC: mpyr     -x0,x0,a        b,x1            a,y:-(r3)   ; 1E7B85
1458AD: add      y0,a            x0,x:(r2)-n2                ; 444250
1458AE: move     p:(r4)+n4,ssl                               ; 07CCBD
1458AF: movep    p:(r1)+n1,y:<<$ffffee                       ; 09C96E
1458B0: mpy      x0,x0,b         x0,x:(r7)-      b,y1        ; 131788
1458B1: sub      x1,b            a,x1            b,y:(r7)+n7 ; 174F6C
1458B2: tfr      a,b             b,x1            y1,y:(r7+n7) ; 1D6F09
1458B3: or       x0,b            y,l:<$38                    ; 43384A
1458B4: movep    x:<<$ffff81,x:(r4)+n4                       ; 074C01
1458B5: mac      y0,y0,b         b,#>$123ef5     x0,b        ; 09349A 123EF5
1458B7: abs      b               a,x1            a,y:(r2)    ; 16622E
1458B8: mpyr     x0,y1,b         b,x1            y0,y:(r3+n3) ; 1C6BC9
1458B9: and      x0,b            x,l:<$32                    ; 42324E
1458BA: dor      #<$d3,>*+$8a89d                             ; 06D390 08A89C
1458BC: mac      y1,x1,b         a,x0            y1,y:(r7+n7) ; 116FFA
1458BD: cmp      y0,b            a,x1            y1,y:-(r6)  ; 157E5D
1458BE: dc       $1b7172                                     ; 1B7172
1458BF: and      x0,b            b10,l:<$30                  ; 41304E
1458C0: rep      #<$362                                      ; 0662E3
1458C1: mac      -x1,y0,b        a,x:(r4)        x0,a        ; 0824EE
1458C2: adc      x,b             x:(r2+n2),x0    a,y0        ; 10AA29
1458C3: mpyr     y0,y0,b         x:(r3),x1       a,y0        ; 14A399
1458C4: mac      -y1,y0,a        b,x0            a,y:-(r7)   ; 1A7FB6
1458C5: asl      b               a10,l:<$32                  ; 40323A
1458C6: move     y:-(r1),sr                                  ; 05F979
1458C7: move     p:<$29,r2                                   ; 07A912
1458C8: jsls     func_000d1c                                 ; 0FED1C
1458C9: macr     -x0,x0,a        a,x0            y:(r1)-,b   ; 13D187
1458CA: eor      x0,b            x:(r6)-,a       a,y1        ; 19964B
1458CB: addr     b,a             #$38,n7                     ; 3F3802
1458CC: blt      func_145846                                 ; 0596DA
1458CD: dc       $073491                                     ; 073491
1458CE: jspl     func_00086d                                 ; 0F386D
1458CF: mac      -x0,y1,b        x0,x:(r7)-n7    b,y1        ; 1307CE
1458D0: mac      -x1,y0,a        #>$3e4198,a     a,y0        ; 18B4E6 3E4198
1458D2: dc       $053a97                                     ; 053A97
1458D3: rep      #<$bc6                                      ; 06C6FB
1458D4: jcs      func_000bc0                                 ; 0E8BC0
1458D5: add      a,b             a,x0            a,y:(r6)-n6 ; 124618
1458D6: tfr      x0,a            a,x1            y:(r3)+,b   ; 17DB41
1458D7: mpyr     -x1,y0,b        #$4e,n5                     ; 3D4EED
1458D8: dc       $04e446                                     ; 04E446
1458D9: rep      #<$85f                                      ; 065FE8
1458DA: dc       $0de6b9                                     ; 0DE6B9
1458DB: maxm     a,b             x:(r4)+n4,x0    a,y1        ; 118C15
1458DC: add      a,b             x1,x:(r1)+n1    b,y1        ; 170918
1458DD: macr     y1,x1,a         #$5f,n4                     ; 3C5FF3
1458DE: bsclr    #$5,x:<<$ffff93,func_1a57d1                 ; 049385 05FEF3
1458E0: dc       $0d4901                                     ; 0D4901
1458E1: cmpm     y1,a            a,x0            y:(r1)+,y0  ; 10D977
1458E2: asr      b               x1,x:-(r6)      b,y0        ; 163E2A
1458E3: mac      y0,y0,b         #$74,n3                     ; 3B749A
1458E4: move     sr,y1                                       ; 0447F9
1458E5: bseq     func_1458c5                                 ; 05A3C0
1458E6: dc       $0cb245                                     ; 0CB245
1458E7: mac      y1,x1,a         x0,x:(r5+n5)    a,y0        ; 102DF2
1458E8: sbc      y,a             a,x1            y1,y:-(r2)  ; 157A35
1458E9: mac      -y0,x0,a        #$8c,n2                     ; 3A8CD6
1458EA: lua      (r1+$4),n4                                  ; 04014C
1458EB: dc       $054df8                                     ; 054DF8
1458EC: dc       $0c2237                                     ; 0C2237
1458ED: jscs     func_00093d                                 ; 0F893D
1458EE: mac      -y1,x1,b        x:-(r4),x1      a,y0        ; 14BCFE
1458EF: macr     -y0,y0,a        #$a8,n1                     ; 39A897
1458F0: dc       $03bf2d                                     ; 03BF2D
1458F1: dc       $04fd49                                     ; 04FD49
1458F2: jsclr    #$a,x:<<$ffffd8,func_0eeb15                 ; 0B988A 0EEB15
1458F4: tfr      x0,b            x1,x:(r6)-n6    a,y0        ; 140649
1458F5: mpyr     y0,x0,a         #$c7,n0                     ; 38C7D1
1458F6: tcs      y0,a r1,r2                                  ; 038152
1458F7: brset    #$4,y:<<$ffffb1,func_1f6dee                 ; 04B164 0B14F7
1458F9: jec      func_000335                                 ; 0E5335
1458FA: mpy      -y0,x0,b        a,x0            b,y:(r5)-   ; 1355DC
1458FB: and      y1,a            #$ea,r7                     ; 37EA76
1458FC: tnn      y1,a r7,r3                                  ; 034773
1458FD: dc       $046a01                                     ; 046A01
1458FE: dc       $0a9738                                     ; 0A9738
1458FF: dc       $0dc15f                                     ; 0DC15F
145900: mpyr     x0,x0,a         x:(r3+n3),x0    b,y0        ; 12AB81
145901: add      y1,b            #$10,r7                     ; 371078
145902: tge      x0,b r1,r7                                  ; 03114F
145903: lua      (r6-$33),n3                                 ; 0426DB
145904: bclr     #$b,x:$1f                                   ; 0A1F0B
145905: dc       $0d3554                                     ; 0D3554
145906: tfr      b,a             x0,x:(r7)-n7    b,y0        ; 120701
145907: mac      x0,y1,b         #$39,r6                     ; 3639CA
145908: move     b0,y:(r6+$36)                               ; 02DEA9
145909: dc       $03e7b3                                     ; 03E7B3
14590A: add      y,a             y0,b            b,y:(r4+n4) ; 09AC30
14590B: dc       $0caedb                                     ; 0CAEDB
14590C: asr      b               a,x0            y1,y:(r0+n0) ; 11682A
14590D: tfr      x1,a            #$66,r5                     ; 356661
14590E: teq      r7,r7                                       ; 02AF47
14590F: teq      x0,b r4,r3                                  ; 03AC4B
145910: cmp      x1,b            b,x:-(r6)       x0,b        ; 093E6D
145911: dc       $0c2db9                                     ; 0C2DB9
145912: mac      x0,y1,b         a,x0            y:(r6)+n6,y0 ; 10CECA
145913: abs      b               #$96,r4                     ; 34962E
145914: dc       $0282f3                                     ; 0282F3
145915: tgt      x1,b r4,r2                                  ; 03746A
145916: movep    x:(r5)-,x:<<$ffffc6                         ; 08D586
145917: dc       $0bb1ba                                     ; 0BB1BA
145918: mpy      y1,y0,a         x0,x:-(r2)      a,y0        ; 103AB0
145919: sbc      x,a             #$c9,r3                     ; 33C925
14591A: tec      r1,r3                                       ; 02597B
14591B: move     x:(r7-$31),b2                               ; 033FDB
14591C: dc       $087146                                     ; 087146
14591D: jsset    #$8,x:$3a,func_0fabae                       ; 0B3AA8 0FABAE
14591F: lsl      b               #$ff,r2                     ; 32FF3B
145920: move     b,y:(r2+$c)                                 ; 0232AF
145921: tcc      x1,b r6,r4                                  ; 030E6C
145922: and      y1,a            a,x:(r1)-       x0,a        ; 081176
145923: bclr     #$12,a0                                     ; 0AC852
145924: jsne     func_000197                                 ; 0F2197
145925: eor      x1,a            #$38,r2                     ; 323863
145926: tcc      r6,r3                                       ; 020E63
145927: move     a1,y:(r7+$37)                               ; 02DFEC
145928: dc       $07b5e5                                     ; 07B5E5
145929: jclr     #$8,x:(r2)+,func_0e9c41                     ; 0A5A88 0E9C41
14592B: mpyr     y0,y0,a         #$74,r1                     ; 317491
14592C: dc       $01ec6f                                     ; 01EC6F
14592D: dc       $02b42f                                     ; 02B42F
14592E: movep    x:<<$ffffa3,y:(r6)+                         ; 075E63
14592F: movep    sc,y:<<$ffffda                              ; 09F11A
145930: jge      func_000b80                                 ; 0E1B80
145931: mpyr     y1,y0,b         #$b3,r0                     ; 30B3B9
145932: move     #$58,n6                                     ; 3E5800
145933: move     #$0,x0                                      ; 240000
145934: lua      (r6)+n6,r0                                  ; 044E10
145935: move     #$18,n6                                     ; 3E1800
145936: move     #$0,x1                                      ; 250000
145937: lua      (r6)+n6,r1                                  ; 044E11
145938: rep      #<$30                                       ; 0630A0
145939: move     x0,x:(r0)+                                  ; 445800
14593A: rep      #<$f                                        ; 060FA0
14593B: move     x,l:(r1)+                                   ; 425900
14593C: move     x0,x:$c                                     ; 440C00
14593D: move     x0,x:$f                                     ; 440F00
14593E: move     #>$88,x0                                    ; 44F400 000088
145940: move     x0,y:(r6+$27)                               ; 029EE4
145941: move     #>$80,x0                                    ; 44F400 000080
145943: move     x0,y:(r6+$2d)                               ; 02B6E4
145944: move     #>$14a000,x0                                ; 44F400 14A000
145946: move     x0,x:(r6+$d)                                ; 0236C4
145947: move     x0,x:(r6+$10)                               ; 024684
145948: rts                                                  ; 00000C
145949: rts                                                  ; 00000C
14594A: move     #>$6000,x0                                  ; 44F400 006000
14594C: move     x:(r6-$27),b                                ; 0366DF
14594D: cmp      x0,b                                        ; 20004D
14594E: tfr      x0,b            ifgt                        ; 202749
14594F: move     a,l:?:>$40                                  ; 487000 000040
145951: move     b,y:$24                                     ; 5F2400
145952: move     #>$80,x1                                    ; 45F400 000080
145954: move     #$80,r0                                     ; 308000
145955: move     y:$24,a                                     ; 5EA400
145956: lua      (r6+$22),r4                                 ; 041624
145957: move     #$c8,r5                                     ; 35C800
145958: move     #>$1407ff,r2                                ; 62F400 1407FF
14595A: move     a,x:(r0)+                                   ; 565800
14595B: move     #>$140000,r3                                ; 63F400 140000
14595D: move     #$80,r0                                     ; 308000
14595E: move     #$c0,r1                                     ; 31C000
14595F: move     #>$17a6,y0                                  ; 46F400 0017A6
145961: move     #>$56982b,y1                                ; 47F400 56982B
145963: move     x:(r0)+,a                                   ; 56D800
145964: asr      #$b,a,b                                     ; 0C1C17
145965: and      #>$7ff,a                                    ; 0140C6 0007FF
145967: neg      a               a1,n3                       ; 219B36
145968: move     b,x0                                        ; 21E400
145969: move     a1,n2                                       ; 219A00
14596A: move     x:(r3+n3),b                                 ; 57EB00
14596B: asr      #$c,b,b                                     ; 0C1C99
14596C: move     x:(r2+n2),a                                 ; 56EA00
14596D: asr      x0,a,a                                      ; 0C1E68
14596E: asl      x0,b,b                                      ; 0C1E59
14596F: move     a,x0                                        ; 21C400
145970: mpy      y0,x0,a         b,x0                        ; 21E4D0
145971: mpy      x0,y1,b         l:(r4),x                    ; 42E4C8
145972: move     a,l:(r1)                                    ; 486100
145973: move     b,l:(r4)                                    ; 496400
145974: move     x,l:(r5)+                                   ; 425D00
145975: move     #$0,r7                                      ; 370000
145976: move     y:(r6+$5),y0                                ; 0216F6
145977: mpyri    #>$80,y0,a                                  ; 0141D1 000080
145979: mpyri    #>$40,y0,b                                  ; 0141D9 000040
14597B: move     a,y:(r7)+                                   ; 5E5F00
14597C: move     b,y:(r7)+                                   ; 5F5F00
14597D: move     a,y:(r7)+                                   ; 5E5F00
14597E: move     b,y:(r7)+                                   ; 5F5F00
14597F: move     #$0,r7                                      ; 370000
145980: move     l:(r4)+,x                                   ; 42DC00
145981: move     y:(r7)+,r0                                  ; 68DF00
145982: do       #<$2,>$145993                               ; 060280 145992
145984: move     x:(r0+$101bfb),y0                           ; 0A70C6 101BFB
145986: move     x:(r0+$101cfb),y1                           ; 0A70C7 101CFB
145988: mpysu    y0,x0,a                                     ; 012785
145989: dmac     su y0,x1,a                                  ; 01258E
14598A: asl      a               y:(r7)+,r0                  ; 68DF32
14598B: mpysu    y1,x0,b                                     ; 0127AC
14598C: dmac     su y1,x1,b                                  ; 0125A7
14598D: asl      b               l:(r4),y                    ; 43E43A
14598E: move     a,l:(r4)+                                   ; 485C00
14598F: move     y,l:(r5)+                                   ; 435D00
145990: move     l:(r4),y                                    ; 43E400
145991: move     b,l:(r4)+                                   ; 495C00
145992: move     y,l:(r5)+                                   ; 435D00
145993: move     l:(r1)+,x                                   ; 42D900
145994: do       #<$2,>$1459a2                               ; 060280 1459A1
145996: move     x:(r0+$101b7b),y0                           ; 0A70C6 101B7B
145998: move     x:(r0+$101c7b),y1                           ; 0A70C7 101C7B
14599A: mpysu    y0,x0,a                                     ; 012785
14599B: dmac     su y0,x1,a                                  ; 01258E
14599C: asl      a               y:(r7)+,r0                  ; 68DF32
14599D: mpysu    y1,x0,b                                     ; 0127AC
14599E: dmac     su y1,x1,b                                  ; 0125A7
14599F: asl      b                                           ; 20003A
1459A0: move     a,l:(r1)+                                   ; 485900
1459A1: move     b,l:(r1)+                                   ; 495900
1459A2: move     #$c8,r3                                     ; 33C800
1459A3: move     #$c0,r0                                     ; 30C000
1459A4: lua      (r6+$18),r2                                 ; 040E82
1459A5: move     x:(r6+$22),x1                               ; 028E95
1459A6: move     x:(r3),a                                    ; 56E300
1459A7: cmp      x1,a                                        ; 200065
1459A8: beq      int_1459ba                                  ; 05A412
1459A9: move     #>$18,x1                                    ; 45F400 000018
1459AB: move     l:(r0)+,y                                   ; 43D800
1459AC: do       #<$5,>$1459ba                               ; 060580 1459B9
1459AE: move     x:(r3)+,x0                                  ; 44DB00
1459AF: mpysu    x0,y0,a                                     ; 01278D
1459B0: dmac     su x0,y1,a                                  ; 012584
1459B1: clb      a,b                                         ; 0C1E01
1459B2: normf    b1,a                                        ; 0C1E26
1459B3: neg      b               l:(r2),y                    ; 43E23E
1459B4: sub      x1,b            a,x0                        ; 21C46C
1459B5: mpysu    x0,y0,a                                     ; 01278D
1459B6: dmac     su x0,y1,a                                  ; 012584
1459B7: normf    b1,a                                        ; 0C1E26
1459B8: move     l:(r0)+,y                                   ; 43D800
1459B9: move     a,l:(r2)+                                   ; 485A00
1459BA: move     #>$a05bc,x0                                 ; 44F400 0A05BC
1459BC: move     x0,x:(r6-$1d)                               ; 038EC4
1459BD: move     y:(r6+$4),y0                                ; 0216B6
1459BE: mpyi     #>$1d41d,y0,a                               ; 0141D0 01D41D
1459C0: move     #>$1d41d,b                                  ; 57F400 01D41D
1459C2: move     b,y:$21                                     ; 5F2100
1459C3: move     y:(r6+$8),y0                                ; 0226B6
1459C4: maci     #>$ebe67,y0,b                               ; 0141DA 0EBE67
1459C6: move     a,y:$22                                     ; 5E2200
1459C7: move     a,y0                                        ; 21C600
1459C8: move     y:(r6+$6),y1                                ; 021EB7
1459C9: mpy      y1,y0,a                                     ; 2000B0
1459CA: move     b,y:$20                                     ; 5F2000
1459CB: move     a,y:$23                                     ; 5E2300
1459CC: move     y:(r6+$9),y1                                ; 0226F7
1459CD: mpyi     #>$80000,y1,a                               ; 0141F0 080000
1459CF: move     y:(r6+$a),y1                                ; 022EB7
1459D0: mpyi     #>$80000,y1,b                               ; 0141F8 080000
1459D2: move     a,y:(r6+$e)                                 ; 023EAE
1459D3: move     b,y:(r6+$11)                                ; 0246EF
1459D4: move     l:?:>$c0,x                                  ; 42F000 0000C0
1459D6: move     x:(r6+$18),b                                ; 02669F
1459D7: move     y:(r6+$18),b0                               ; 0266B9
1459D8: move     #$0,r1                                      ; 310000
1459D9: jsr      func_0003e0                                 ; 0D03E0
1459DA: move     b1,x:(r6+$18)                               ; 02668D
1459DB: move     b0,y:(r6+$18)                               ; 0266A9
1459DC: move     #$80,r7                                     ; 378000
1459DD: move     #$1,m7                                      ; 0501A7
1459DE: move     #>$40,x0                                    ; 44F400 000040
1459E0: move     y:$20,y0                                    ; 4EA000
1459E1: move     y0,y:(r7)+                                  ; 4E5F00
1459E2: move     y:$21,y0                                    ; 4EA100
1459E3: move     y0,y:(r7)                                   ; 4E6700
1459E4: move     y:(r6+$27),a                                ; 029EFE
1459E5: move     r6,r0                                       ; 22D000
1459E6: move     r6,r3                                       ; 22D300
1459E7: move     a,n0                                        ; 21D800
1459E8: add      #<$10,a                                     ; 015080
1459E9: cmp      #>$98,a                                     ; 0140C5 000098
1459EB: sub      x0,a            ifeq                        ; 202A44
1459EC: move     a,y:(r6+$27)                                ; 029EEE
1459ED: move     a,n3                                        ; 21DB00
1459EE: move     y:(r6+$2d),r7                               ; 0B76D7 00002D
1459F0: move     (r0)+n0                                     ; 204800
1459F1: move     #$0,x1                                      ; 250000
1459F2: rep      #<$10                                       ; 0610A0
1459F3: move     x1,x:(r0)+                                  ; 455800
1459F4: move     (r3)+n3                                     ; 204B00
1459F5: jsr      func_0003aa                                 ; 0D03AA
1459F6: move     r7,y:(r6+$2d)                               ; 0B7697 00002D
1459F8: move     l:?:>$c1,x                                  ; 42F000 0000C1
1459FA: move     x:(r6+$19),b                                ; 0266DF
1459FB: move     y:(r6+$19),b0                               ; 0266F9
1459FC: move     #$0,r1                                      ; 310000
1459FD: jsr      func_0003e0                                 ; 0D03E0
1459FE: move     b1,x:(r6+$19)                               ; 0266CD
1459FF: move     b0,y:(r6+$19)                               ; 0266E9
145A00: move     y:$22,y0                                    ; 4EA200
145A01: move     y0,y:(r7)+                                  ; 4E5F00
145A02: move     y0,y:(r7)+                                  ; 4E5F00
145A03: jsr      func_0003aa                                 ; 0D03AA
145A04: move     l:?:>$c2,x                                  ; 42F000 0000C2
145A06: move     x:(r6+$1a),b                                ; 026E9F
145A07: move     y:(r6+$1a),b0                               ; 026EB9
145A08: move     #$0,r1                                      ; 310000
145A09: jsr      func_0003e0                                 ; 0D03E0
145A0A: move     b1,x:(r6+$1a)                               ; 026E8D
145A0B: move     b0,y:(r6+$1a)                               ; 026EA9
145A0C: jsr      func_0003aa                                 ; 0D03AA
145A0D: move     l:?:>$c3,x                                  ; 42F000 0000C3
145A0F: move     x:(r6+$1b),b                                ; 026EDF
145A10: move     y:(r6+$1b),b0                               ; 026EF9
145A11: move     #$0,r1                                      ; 310000
145A12: jsr      func_0003e0                                 ; 0D03E0
145A13: move     b1,x:(r6+$1b)                               ; 026ECD
145A14: move     b0,y:(r6+$1b)                               ; 026EE9
145A15: move     y:$23,y0                                    ; 4EA300
145A16: move     y0,y:(r7)+                                  ; 4E5F00
145A17: move     y0,y:(r7)+                                  ; 4E5F00
145A18: jsr      func_0003aa                                 ; 0D03AA
145A19: move     l:?:>$c4,x                                  ; 42F000 0000C4
145A1B: move     x:(r6+$1c),b                                ; 02769F
145A1C: move     y:(r6+$1c),b0                               ; 0276B9
145A1D: move     #$0,r1                                      ; 310000
145A1E: jsr      func_0003e0                                 ; 0D03E0
145A1F: move     b1,x:(r6+$1c)                               ; 02768D
145A20: move     b0,y:(r6+$1c)                               ; 0276A9
145A21: jsr      func_0003aa                                 ; 0D03AA
145A22: move     #>$ffffff,m7                                ; 05F427 FFFFFF
145A24: move     y:$24,x1                                    ; 4DA400
145A25: mpyi     #>$15555,x1,a                               ; 0141E0 015555
145A27: sub      #<$14,a                                     ; 015484
145A28: clr      a               ifmi                        ; 202B13
145A29: move     a1,r2                                       ; 219200
145A2A: move     a0,y0                                       ; 210600
145A2B: move     #>$7eb852,x1                                ; 45F400 7EB852
145A2D: move     x:(r2+$143d06),x0                           ; 0A72C4 143D06
145A2F: move     x:(r2+$143d07),y1                           ; 0A72C7 143D07
145A31: mpysu    y1,y0,a                                     ; 012783
145A32: macsu    -x0,y0,a                                    ; 01269D
145A33: asr      a                                           ; 200022
145A34: add      x0,a                                        ; 200040
145A35: move     y:(r6+$2e),y1                               ; 02BEB7
145A36: move     a,y:(r6+$2e)                                ; 02BEAE
145A37: move     r3,r0                                       ; 227000
145A38: move     #$0,r1                                      ; 310000
145A39: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145A3B: move     m0,m1                                       ; 0461A0
145A3C: move     x:(r6+$1f),a                                ; 027EDE
145A3D: move     x:(r0)+,x0                                  ; 44D800
145A3E: do       #<$10,>$145a42                              ; 061080 145A41
145A40: mac      x1,x0,a         a,x:(r1)+       a,y0        ; 1819A2
145A41: mac      -y1,y0,a        x:(r0)+,x0                  ; 44D8B6
145A42: move     a,x:(r6+$1f)                                ; 027ECE
145A43: move     y:$24,x1                                    ; 4DA400
145A44: mpyi     #>$b3333,x1,a                               ; 0141E0 0B3333
145A46: add      #>$ffff80,a                                 ; 0140C0 FFFF80
145A48: clr      a               ifmi                        ; 202B13
145A49: move     #>$63f,x0                                   ; 44F400 00063F
145A4B: cmp      x0,a                                        ; 200045
145A4C: tfr      x0,a            ifgt                        ; 202741
145A4D: move     x:(r6+$2e),r2                               ; 0A76D2 00002E
145A4F: move     a,x:(r6+$2e)                                ; 02BE8E
145A50: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145A52: move     x:(r2+$1435c6),x1                           ; 0A72C5 1435C6
145A54: sub      x1,a            a,x0                        ; 21C464
145A55: add      x0,a                                        ; 200040
145A56: asr      a                                           ; 200022
145A57: move     #$0,r0                                      ; 300000
145A58: move     r0,r1                                       ; 221100
145A59: move     a,y1                                        ; 21C700
145A5A: move     x:(r6+$1d),a                                ; 0276DE
145A5B: move     y:(r6+$1e),x0                               ; 027EB4
145A5C: asl      #$3,a,b                                     ; 0C1D07
145A5D: do       #<$10,>$145a63                              ; 061080 145A62
145A5F: mac      -x0,y1,a        x:(r0)+,x0      a,y0        ; 1098C6
145A60: mac      x0,y1,a                                     ; 2000C2
145A61: mac      -x1,y0,a        b,x:(r1)+                   ; 5759E6
145A62: asl      #$3,a,b                                     ; 0C1D07
145A63: move     x0,y:(r6+$1e)                               ; 027EA4
145A64: move     a,x:(r6+$1d)                                ; 0276CE
145A65: move     #>$17c6f9,x0                                ; 44F400 17C6F9
145A67: move     l:?:>$40,y                                  ; 43F000 000040
145A69: mpysu    x0,y0,b                                     ; 0127AD
145A6A: dmac     ss x0,y1,b                                  ; 0124A4
145A6B: lua      (r6+$c),r5                                  ; 0406C5
145A6C: move     b,l:?:>$40                                  ; 497000 000040
145A6E: do       #<$2,>$145aa9                               ; 060280 145AA8
145A70: move     l:?:>$40,a                                  ; 48F000 000040
145A72: asr      a                                           ; 200022
145A73: move     l:(r5),b                                    ; 49E500
145A74: move     a,l:?:>$40                                  ; 487000 000040
145A76: move     a,l:(r5)+                                   ; 485D00
145A77: sub      b,a                                         ; 200014
145A78: asr      #$4,a,a                                     ; 0C1C08
145A79: move     #$10,r1                                     ; 311000
145A7A: move     a1,y1                                       ; 218700
145A7B: move     a0,y0                                       ; 210600
145A7C: tfr      b,a                                         ; 200001
145A7D: move     l:(r5),b                                    ; 49E500
145A7E: move     #>$1fff,x0                                  ; 44F400 001FFF
145A80: move     #>$14a000,x1                                ; 45F400 14A000
145A82: do       #<$10,>$145a88                              ; 061080 145A87
145A84: add      y,a                                         ; 200030
145A85: add      a,b             b,l:(r1)+                   ; 495918
145A86: and      x0,b                                        ; 20004E
145A87: add      x1,b                                        ; 200068
145A88: move     #$10,r1                                     ; 311000
145A89: move     r1,r4                                       ; 223400
145A8A: move     b1,x:(r5)                                   ; 556500
145A8B: move     b0,y:(r5)+                                  ; 595D00
145A8C: move     x:(r1)+,r2                                  ; 62D900
145A8D: move     #>$1fff,m2                                  ; 05F422 001FFF
145A8F: move     m2,m0                                       ; 0460A2
145A90: move     #$0,r3                                      ; 330000
145A91: move     y:(r5)+,y1                                  ; 4FDD00
145A92: move     y:(r4)+,y0                                  ; 4EDC00
145A93: move     x:(r2)+,x1                                  ; 45DA00
145A94: move     x:(r1)+,r0                                  ; 60D900
145A95: do       #<$8,>$145aa5                               ; 060880 145AA4
145A97: mpysu    -x1,y0,a                                    ; 012796
145A98: add      x1,a            x:(r2),x0                   ; 44E260
145A99: add      x1,a            x:(r1)+,r2                  ; 62D960
145A9A: macsu    x0,y0,a                                     ; 01268D
145A9B: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145A9C: mpysu    -x1,y0,b                                    ; 0127B6
145A9D: add      x1,b            x:(r0),x0                   ; 44E068
145A9E: add      x1,b            x:(r1)+,r0                  ; 60D968
145A9F: macsu    x0,y0,b                                     ; 0126AD
145AA0: asr      b               x:(r3)+,a       a,y0        ; 189B2A
145AA1: mac      y1,y0,a         x:(r3)-,b       b,y0        ; 1E93B2
145AA2: mac      x0,y1,b         x:(r2)+,x1      y:(r4)+,y0  ; F49ACA
145AA3: move     a,x:(r3)+                                   ; 565B00
145AA4: move     b,x:(r3)+                                   ; 575B00
145AA5: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145AA7: move     #>$ffffff,m2                                ; 05F422 FFFFFF
145AA9: move     #>$100,r7                                   ; 67F400 000100
145AAB: move     #$0,r0                                      ; 300000
145AAC: move     x:(r0)+,a                                   ; 56D800
145AAD: move     x:(r0)+,b                                   ; 57D800
145AAE: do       #<$8,>$145ab4                               ; 060880 145AB3
145AB0: move     a,y:(r7)+                                   ; 5E5F00
145AB1: move     x:(r0)+,a       a,y:(r7)+                   ; BAF800
145AB2: move     b,y:(r7)+                                   ; 5F5F00
145AB3: move     x:(r0)+,b       b,y:(r7)+                   ; BFF800
145AB4: rts                                                  ; 00000C
145AB5: move     #$58,n6                                     ; 3E5800
145AB6: move     #$0,x0                                      ; 240000
145AB7: move     #$0,x1                                      ; 250000
145AB8: lua      (r6)+n6,r0                                  ; 044E10
145AB9: lua      (r6+$23),r1                                 ; 041631
145ABA: do       #<$30,>$145abd                              ; 063080 145ABC
145ABC: move     x0,x:(r0)+                                  ; 445800
145ABD: do       #<$c,>$145ac0                               ; 060C80 145ABF
145ABF: move     x,l:(r1)+                                   ; 425900
145AC0: move     #>$88,x0                                    ; 44F400 000088
145AC2: move     x0,y:(r6+$1a)                               ; 026EA4
145AC3: move     #>$80,x0                                    ; 44F400 000080
145AC5: move     x0,y:(r6+$1b)                               ; 026EE4
145AC6: move     x0,y:(r6+$1c)                               ; 0276A4
145AC7: move     x0,y:(r6+$1d)                               ; 0276E4
145AC8: move     x0,y:(r6+$1e)                               ; 027EA4
145AC9: move     #>$c0,x0                                    ; 44F400 0000C0
145ACB: move     x0,y:(r6+$1f)                               ; 027EE4
145ACC: move     x0,y:(r6+$20)                               ; 0286A4
145ACD: move     x0,y:(r6+$21)                               ; 0286E4
145ACE: move     x0,y:(r6+$22)                               ; 028EA4
145ACF: move     #>$7f5c29,x0                                ; 44F400 7F5C29
145AD1: move     x0,x:(r6+$37)                               ; 02DEC4
145AD2: clr      a               #>$147ae,x0                 ; 44F413 0147AE
145AD4: move     x0,y:(r6+$37)                               ; 02DEE4
145AD5: move     a,x:(r6+$31)                                ; 02C6CE
145AD6: move     a,y:(r6+$34)                                ; 02D6AE
145AD7: move     a,y:(r6+$32)                                ; 02CEAE
145AD8: move     #>$114000,a0                                ; 50F400 114000
145ADA: move     y:>$124,x1                                  ; 4DF000 000124
145ADC: maci     #>$2000,x1,a                                ; 0141E2 002000
145ADE: move     #$80,x0                                     ; 248000
145ADF: move     a0,y:(r6+$36)                               ; 02DEA8
145AE0: move     x0,x:(r6+$30)                               ; 02C684
145AE1: rts                                                  ; 00000C
145AE2: rts                                                  ; 00000C
145AE3: move     #>$800,x1                                   ; 45F400 000800
145AE5: move     #>$4d55,x0                                  ; 44F400 004D55
145AE7: move     x:(r6-$27),a                                ; 0366DE
145AE8: cmp      x0,a                                        ; 200045
145AE9: tfr      x0,a ifge                                   ; 202141
145AEA: sub      x1,a                                        ; 200064
145AEB: clr      a               ifmi                        ; 202B13
145AEC: move     #>$17a6,y0                                  ; 46F400 0017A6
145AEE: move     #>$56982b,y1                                ; 47F400 56982B
145AF0: asr      #$b,a,b                                     ; 0C1C17
145AF1: and      #>$7ff,a                                    ; 0140C6 0007FF
145AF3: neg      a               a1,r3                       ; 219336
145AF4: move     b,x0                                        ; 21E400
145AF5: move     a1,r2                                       ; 219200
145AF6: move     x:(r3+$140000),b                            ; 0A73CF 140000
145AF8: asr      #$c,b,b                                     ; 0C1C99
145AF9: asl      x0,b,b                                      ; 0C1E59
145AFA: move     x:(r2+$1407ff),a                            ; 0A72CE 1407FF
145AFC: move     b,x1                                        ; 21E500
145AFD: asr      x0,a,a                                      ; 0C1E68
145AFE: move     a,x0                                        ; 21C400
145AFF: mpyi     #>$17a6,x0,a                                ; 0141C0 0017A6
145B01: mpyi     #>$56982b,x1,b                              ; 0141E8 56982B
145B03: move     #$c0,r0                                     ; 30C000
145B04: lua      (r6+$2a),r1                                 ; 0416A1
145B05: move     #$c8,r5                                     ; 35C800
145B06: move     #$8c,r7                                     ; 378C00
145B07: move     a,l:<$0                                     ; 480000
145B08: move     b,l:<$1                                     ; 490100
145B09: move     #>$2000,x1                                  ; 45F400 002000
145B0B: lua      (r6+$4),r4                                  ; 040644
145B0C: move     #>$101a7b,r2                                ; 62F400 101A7B
145B0E: move     #>$101afb,r3                                ; 63F400 101AFB
145B10: move     #>$80,x1                                    ; 45F400 000080
145B12: move     l:(r1),y                                    ; 43E100
145B13: move     b,l:(r1)+                                   ; 495900
145B14: move     a,l:(r0)+                                   ; 485800
145B15: do       #<$3,>$145b2a                               ; 060380 145B29
145B17: move     y:(r4)+,x0                                  ; 4CDC00
145B18: mpy      x1,x0,a                                     ; 2000A0
145B19: move     a1,n2                                       ; 219A00
145B1A: move     y,l:(r5)+                                   ; 435D00
145B1B: move     a1,n3                                       ; 219B00
145B1C: move     l:<$0,y                                     ; 438000
145B1D: move     x:(r2+n2),x0                                ; 44EA00
145B1E: mpysu    x0,y0,a                                     ; 01278D
145B1F: dmac     ss x0,y1,a                                  ; 012484
145B20: move     x:(r3+n3),x0                                ; 44EB00
145B21: move     l:<$1,y                                     ; 438100
145B22: mpysu    x0,y0,b                                     ; 0127AD
145B23: dmac     ss x0,y1,b                                  ; 0124A4
145B24: move     x0,y:(r7)+                                  ; 4C5F00
145B25: asl      #$2,b,b                                     ; 0C1D85
145B26: asl      #$2,a,a                                     ; 0C1D04
145B27: move     a,l:(r0)+                                   ; 485800
145B28: move     l:(r1),y                                    ; 43E100
145B29: move     b,l:(r1)+                                   ; 495900
145B2A: move     y,l:(r5)+                                   ; 435D00
145B2B: move     #$a0,r4                                     ; 34A000
145B2C: move     #$c0,r0                                     ; 30C000
145B2D: move     #>$147ae,b                                  ; 57F400 0147AE
145B2F: move     y:(r6+$8),y0                                ; 0226B6
145B30: maci     #>$7d70a4,y0,b                              ; 0141DA 7D70A4
145B32: move     b,y0                                        ; 21E600
145B33: move     l:(r0),x                                    ; 42E000
145B34: do       #<$4,>$145b3c                               ; 060480 145B3B
145B36: move     l:(r0)+,b                                   ; 49D800
145B37: mpysu    y0,x0,a                                     ; 012785
145B38: dmac     ss y0,x1,a                                  ; 01248E
145B39: sub      a,b             l:(r0),x                    ; 42E01C
145B3A: move     a,l:(r4)+                                   ; 485C00
145B3B: move     b,l:(r4)+                                   ; 495C00
145B3C: move     #$c8,r3                                     ; 33C800
145B3D: move     #$c0,r0                                     ; 30C000
145B3E: lua      (r6+$23),r2                                 ; 041632
145B3F: lua      (r6+$2a),r1                                 ; 0416A1
145B40: move     #>$18,x1                                    ; 45F400 000018
145B42: move     l:(r0)+,y                                   ; 43D800
145B43: do       #<$4,>$145b56                               ; 060480 145B55
145B45: move     x:(r3),x0                                   ; 44E300
145B46: mpysu    x0,y0,a                                     ; 01278D
145B47: dmac     su x0,y1,a                                  ; 012584
145B48: clb      a,b                                         ; 0C1E01
145B49: normf    b1,a                                        ; 0C1E26
145B4A: neg      b               l:(r2),y                    ; 43E23E
145B4B: sub      x1,b            a,x0                        ; 21C46C
145B4C: mpysu    x0,y0,a                                     ; 01278D
145B4D: dmac     su x0,y1,a                                  ; 012584
145B4E: normf    b1,a                                        ; 0C1E26
145B4F: move     x:(r1)+,b                                   ; 57D900
145B50: move     x:(r3)+,x0                                  ; 44DB00
145B51: cmp      x0,b                                        ; 20004D
145B52: move     l:(r2),b                                    ; 49E200
145B53: tfr      b,a             ifeq                        ; 202A01
145B54: move     l:(r0)+,y                                   ; 43D800
145B55: move     a,l:(r2)+                                   ; 485A00
145B56: move     #$c0,r0                                     ; 30C000
145B57: move     #$1,m0                                      ; 0501A0
145B58: move     l:?:>$a0,a                                  ; 48F000 0000A0
145B5A: move     l:?:>$a1,y                                  ; 43F000 0000A1
145B5C: move     a,l:(r0)+                                   ; 485800
145B5D: move     y,l:(r0)                                    ; 436000
145B5E: move     y:(r6+$1f),r0                               ; 0B76D0 00001F
145B60: move     x:(r6+$23),b                                ; 028EDF
145B61: move     y:(r6+$23),b0                               ; 028EF9
145B62: move     #$0,r1                                      ; 310000
145B63: jsr      func_0003d3                                 ; 0D03D3
145B64: move     r0,y:(r6+$1f)                               ; 0B7690 00001F
145B66: move     b1,x:(r6+$23)                               ; 028ECD
145B67: move     b0,y:(r6+$23)                               ; 028EE9
145B68: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145B6A: move     #$28,x0                                     ; 242800
145B6B: move     x0,x:(r6-$1d)                               ; 038EC4
145B6C: move     #$80,r7                                     ; 378000
145B6D: move     #$1,m7                                      ; 0501A7
145B6E: move     y:(r6+$7),x0                                ; 021EF4
145B6F: move     #>$5c28f,a                                  ; 56F400 05C28F
145B71: move     #>$5c28f,b                                  ; 57F400 05C28F
145B73: maci     #>$f62183,x0,b                              ; 0141CA F62183
145B75: maci     #>$fe4226,x0,a                              ; 0141C2 FE4226
145B77: move     b,y:>$82                                    ; 5F7000 000082
145B79: move     b,y:(r7)+                                   ; 5F5F00
145B7A: move     #>$40,x0                                    ; 44F400 000040
145B7C: move     a,y:(r7)                                    ; 5E6700
145B7D: move     a,y:>$83                                    ; 5E7000 000083
145B7F: move     y:(r6+$1a),a                                ; 026EBE
145B80: move     r6,r0                                       ; 22D000
145B81: move     r6,r3                                       ; 22D300
145B82: move     a,n0                                        ; 21D800
145B83: add      #>$10,a                                     ; 0140C0 000010
145B85: cmp      #>$98,a                                     ; 0140C5 000098
145B87: sub      x0,a            ifeq                        ; 202A44
145B88: move     a,y:(r6+$1a)                                ; 026EAE
145B89: move     a,n3                                        ; 21DB00
145B8A: move     (r0)+n0                                     ; 204800
145B8B: move     #$0,x1                                      ; 250000
145B8C: do       #<$10,>$145b8f                              ; 061080 145B8E
145B8E: move     x1,x:(r0)+                                  ; 455800
145B8F: move     (r3)+n3                                     ; 204B00
145B90: move     y:(r6+$1b),r7                               ; 0B76D7 00001B
145B92: jsr      func_0003aa                                 ; 0D03AA
145B93: move     r7,y:(r6+$1b)                               ; 0B7697 00001B
145B95: move     #$c0,r0                                     ; 30C000
145B96: move     #$1,m0                                      ; 0501A0
145B97: move     l:?:>$a2,a                                  ; 48F000 0000A2
145B99: move     l:?:>$a3,y                                  ; 43F000 0000A3
145B9B: move     a,l:(r0)+                                   ; 485800
145B9C: move     y,l:(r0)                                    ; 436000
145B9D: move     y:(r6+$20),r0                               ; 0B76D0 000020
145B9F: move     x:(r6+$24),b                                ; 02969F
145BA0: move     y:(r6+$24),b0                               ; 0296B9
145BA1: move     #$0,r1                                      ; 310000
145BA2: jsr      func_0003d3                                 ; 0D03D3
145BA3: move     r0,y:(r6+$20)                               ; 0B7690 000020
145BA5: move     b1,x:(r6+$24)                               ; 02968D
145BA6: move     b0,y:(r6+$24)                               ; 0296A9
145BA7: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145BA9: move     #$80,r7                                     ; 378000
145BAA: move     y:>$8c,a                                    ; 5EF000 00008C
145BAC: cmp      #>$200000,a                                 ; 0140C5 200000
145BAE: move     y:>$82,a                                    ; 5EF000 000082
145BB0: move     y:>$83,b                                    ; 5FF000 000083
145BB2: clr      a               ifeq                        ; 202A13
145BB3: clr      b               ifeq                        ; 202A1B
145BB4: move     a,y:(r7)+                                   ; 5E5F00
145BB5: move     b,y:(r7)                                    ; 5F6700
145BB6: move     y:(r6+$1c),r7                               ; 0B76D7 00001C
145BB8: jsr      func_0003aa                                 ; 0D03AA
145BB9: move     r7,y:(r6+$1c)                               ; 0B7697 00001C
145BBB: move     #$c0,r0                                     ; 30C000
145BBC: move     #$1,m0                                      ; 0501A0
145BBD: move     l:?:>$a4,a                                  ; 48F000 0000A4
145BBF: move     l:?:>$a5,y                                  ; 43F000 0000A5
145BC1: move     a,l:(r0)+                                   ; 485800
145BC2: move     y,l:(r0)                                    ; 436000
145BC3: move     y:(r6+$21),r0                               ; 0B76D0 000021
145BC5: move     x:(r6+$25),b                                ; 0296DF
145BC6: move     y:(r6+$25),b0                               ; 0296F9
145BC7: move     #$0,r1                                      ; 310000
145BC8: jsr      func_0003d3                                 ; 0D03D3
145BC9: move     r0,y:(r6+$21)                               ; 0B7690 000021
145BCB: move     b1,x:(r6+$25)                               ; 0296CD
145BCC: move     b0,y:(r6+$25)                               ; 0296E9
145BCD: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145BCF: move     #$80,r7                                     ; 378000
145BD0: move     y:>$8d,a                                    ; 5EF000 00008D
145BD2: cmp      #>$200000,a                                 ; 0140C5 200000
145BD4: move     y:>$82,a                                    ; 5EF000 000082
145BD6: move     y:>$83,b                                    ; 5FF000 000083
145BD8: clr      a               ifeq                        ; 202A13
145BD9: clr      b               ifeq                        ; 202A1B
145BDA: move     a,y:(r7)+                                   ; 5E5F00
145BDB: move     b,y:(r7)                                    ; 5F6700
145BDC: move     y:(r6+$1d),r7                               ; 0B76D7 00001D
145BDE: jsr      func_0003aa                                 ; 0D03AA
145BDF: move     r7,y:(r6+$1d)                               ; 0B7697 00001D
145BE1: move     #$c0,r0                                     ; 30C000
145BE2: move     #$1,m0                                      ; 0501A0
145BE3: move     l:?:>$a6,a                                  ; 48F000 0000A6
145BE5: move     l:?:>$a7,y                                  ; 43F000 0000A7
145BE7: move     a,l:(r0)+                                   ; 485800
145BE8: move     y,l:(r0)                                    ; 436000
145BE9: move     y:(r6+$22),r0                               ; 0B76D0 000022
145BEB: move     x:(r6+$26),b                                ; 029E9F
145BEC: move     y:(r6+$26),b0                               ; 029EB9
145BED: move     #$0,r1                                      ; 310000
145BEE: jsr      func_0003d3                                 ; 0D03D3
145BEF: move     r0,y:(r6+$22)                               ; 0B7690 000022
145BF1: move     b1,x:(r6+$26)                               ; 029E8D
145BF2: move     b0,y:(r6+$26)                               ; 029EA9
145BF3: move     #>$ffffff,m0                                ; 05F420 FFFFFF
145BF5: move     #$80,r7                                     ; 378000
145BF6: move     y:>$8e,a                                    ; 5EF000 00008E
145BF8: cmp      #>$200000,a                                 ; 0140C5 200000
145BFA: move     y:>$82,a                                    ; 5EF000 000082
145BFC: move     y:>$83,b                                    ; 5FF000 000083
145BFE: clr      a               ifeq                        ; 202A13
145BFF: clr      b               ifeq                        ; 202A1B
145C00: move     a,y:(r7)+                                   ; 5E5F00
145C01: move     b,y:(r7)                                    ; 5F6700
145C02: move     y:(r6+$1e),r7                               ; 0B76D7 00001E
145C04: jsr      func_0003aa                                 ; 0D03AA
145C05: move     r7,y:(r6+$1e)                               ; 0B7697 00001E
145C07: move     #>$ffffff,m7                                ; 05F427 FFFFFF
145C09: move     x:(r6-$27),x1                               ; 0366D5
145C0A: mpyi     #>$15555,x1,a                               ; 0141E0 015555
145C0C: sub      #>$15,a                                     ; 0140C4 000015
145C0E: clr      a               ifmi                        ; 202B13
145C0F: move     a1,r2                                       ; 219200
145C10: move     a0,y0                                       ; 210600
145C11: move     #$40,x1                                     ; 254000
145C12: move     x:(r2+$143d06),x0                           ; 0A72C4 143D06
145C14: move     x:(r2+$143d07),y1                           ; 0A72C7 143D07
145C16: mpysu    y1,y0,a                                     ; 012783
145C17: macsu    -x0,y0,a                                    ; 01269D
145C18: asr      a                                           ; 200022
145C19: add      x0,a            r3,r0                       ; 227040
145C1A: move     #$0,r1                                      ; 310000
145C1B: move     y:(r6+$2e),y1                               ; 02BEB7
145C1C: move     a,y:(r6+$2e)                                ; 02BEAE
145C1D: move     x:(r6+$29),a                                ; 02A6DE
145C1E: move     x:(r0)+,x0                                  ; 44D800
145C1F: do       #<$10,>$145c23                              ; 061080 145C22
145C21: mac      x1,x0,a         a,x:(r1)+       a,y0        ; 1819A2
145C22: mac      -y1,y0,a        x:(r0)+,x0                  ; 44D8B6
145C23: move     a,x:(r6+$29)                                ; 02A6CE
145C24: move     x:(r6-$27),x1                               ; 0366D5
145C25: mpyi     #>$0,x1,a                                   ; 0141E0 000000
145C27: add      #>$ffff80,a                                 ; 0140C0 FFFF80
145C29: clr      a               ifmi                        ; 202B13
145C2A: move     #>$63f,x0                                   ; 44F400 00063F
145C2C: cmp      x0,a                                        ; 200045
145C2D: tfr      x0,a            ifgt                        ; 202741
145C2E: move     a,r2                                        ; 21D200
145C2F: move     #>$7fffff,b                                 ; 57F400 7FFFFF
145C31: move     x:(r2+$1435c6),a                            ; 0A72CE 1435C6
145C33: sub      a,b             b,x0                        ; 21E41C
145C34: add      x0,b                                        ; 200048
145C35: asr      b               #$0,r0                      ; 30002A
145C36: move     b,y1                                        ; 21E700
145C37: move     a,x1                                        ; 21C500
145C38: move     #>$100,r7                                   ; 67F400 000100
145C3A: move     x:(r6+$27),a                                ; 029EDE
145C3B: move     y:(r6+$28),x0                               ; 02A6B4
145C3C: do       #<$10,>$145c42                              ; 061080 145C41
145C3E: asl      #$3,a,b                                     ; 0C1D07
145C3F: mac      -x0,y1,a        x:(r0)+,x0      a,y0        ; 1098C6
145C40: mac      x0,y1,a         b,y:(r7)+                   ; 5F5FC2
145C41: mac      -x1,y0,a        b,y:(r7)+                   ; 5F5FE6
145C42: move     x0,y:(r6+$28)                               ; 02A6A4
145C43: move     a,x:(r6+$27)                                ; 029ECE
145C44: move     #>$145c48,x0                                ; 44F400 145C48
145C46: move     x0,x:(r6-$1c)                               ; 039684
145C47: rts                                                  ; 00000C
145C48: move     #>$7fffff,b                                 ; 57F400 7FFFFF
145C4A: move     y:(r6+$34),a                                ; 02D6BE
145C4B: cmp      #>$40,a                                     ; 0140C5 000040
145C4D: bge      func_145c51                                 ; 051404
145C4E: add      #>$1,a                                      ; 0140C0 000001
145C50: clr      b                                           ; 20001B
145C51: move     a,y:(r6+$34)                                ; 02D6AE
145C52: move     b,y:(r6+$33)                                ; 02CEEF
145C53: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145C55: move     y:(r6+$9),x1                                ; 0226F5
145C56: mpyi     #>$66666,x1,b                               ; 0141E8 066666
145C58: move     y:(r6+$32),x1                               ; 02CEB5
145C59: maci     #>$79999a,x1,b                              ; 0141EA 79999A
145C5B: move     #$c0,r5                                     ; 35C000
145C5C: move     b,y:(r6+$32)                                ; 02CEAF
145C5D: sub      x1,a            a,b                         ; 21CF64
145C5E: move     y:(r6+$a),x0                                ; 022EB4
145C5F: move     a,y:>$c4                                    ; 5E7000 0000C4
145C61: tfr      b,a             b,y1                        ; 21E701
145C62: maci     #>$c00000,x0,a                              ; 0141C2 C00000
145C64: mpy      y1,x1,b         y:$1,y1                     ; 4F81F8
145C65: move     a,y0                                        ; 21C600
145C66: mpy      x1,y0,a         y:$0,y0                     ; 4E80E0
145C67: move     y:(r6+$33),x0                               ; 02CEF4
145C68: mpy      y0,x0,b         b,x1                        ; 21E5D8
145C69: mpy      x0,y1,a         a,x0                        ; 21C4C0
145C6A: move     b,y0                                        ; 21E600
145C6B: mpy      y0,x0,b         a,y1                        ; 21C7D8
145C6C: mpy      y1,x1,a         #$20,r1                     ; 3120F0
145C6D: move     #$30,r0                                     ; 303000
145C6E: mpy      x0,y1,a         a,y:(r5)+                   ; 5E5DC0
145C6F: mpy      x1,y0,b         b,y:(r5)+                   ; 5F5DE8
145C70: move     a,y:(r5)+                                   ; 5E5D00
145C71: move     b,y:(r5)+                                   ; 5F5D00
145C72: move     x:(r6+$30),y1                               ; 02C697
145C73: move     x:(r6+$31),b                                ; 02C6DF
145C74: tfr      y1,a            #>$ef,x0                    ; 44F471 0000EF
145C76: bset     #$14,sr                                     ; 0AF974
145C77: do       #<$10,>$145c7b                              ; 061080 145C7A
145C79: mac      -x0,y1,b        b,x:(r0)+       b,y0        ; 1E18CE
145C7A: mac      y0,x0,a a,x:(r1)+ a,y1                      ; 1919D2
145C7B: bclr     #$14,sr                                     ; 0AF954
145C7C: move     #>$fffe70,n0                                ; 70F400 FFFE70
145C7E: move     y:(r6+$36),r0                               ; 0B76D0 000036
145C80: move     #>$3ff,m0                                   ; 05F420 0003FF
145C82: move     m0,m4                                       ; 0464A0
145C83: move     b,x:(r6+$31)                                ; 02C6CF
145C84: lua      (r0)+n0,r0                                  ; 044810
145C85: move     #$20,r2                                     ; 322000
145C86: move     r0,r1                                       ; 221100
145C87: move     r0,r4                                       ; 221400
145C88: move     #>$154,x0                                   ; 44F400 000154
145C8A: move     x:(r2)+,x1                                  ; 45DA00
145C8B: mpy      x1,x0,b         x:(r2)+,x1                  ; 45DAA8
145C8C: move     a,x:(r6+$30)                                ; 02C68E
145C8D: move     b1,n4                                       ; 21BC00
145C8E: move     (r0)+                                       ; 205800
145C8F: move     #$7f,r3                                     ; 337F00
145C90: move     #$f,n0                                      ; 380F00
145C91: move     (r4)+n4                                     ; 204C00
145C92: move     y:(r4)+,y0                                  ; 4EDC00
145C93: do       #<$2,>$145ca2                               ; 060280 145CA1
145C95: do       n0,>$145ca0                                 ; 06D800 145C9F
145C97: mpy      x1,x0,b         b0,x1                       ; 2125A8
145C98: move     a,x:(r3)+       y:(r4)+,y1                  ; F91B00
145C99: move     r0,r4                                       ; 221400
145C9A: move     b1,n4                                       ; 21BC00
145C9B: move     (r0)+                                       ; 205800
145C9C: mpysu    y1,x1,a                                     ; 012787
145C9D: macsu    -y0,x1,a                                    ; 01269E
145C9E: asr      a               (r4)+n4                     ; 204C22
145C9F: add      y0,a            x:(r2)+,x1      y:(r4)+,y0  ; F49A50
145CA0: move     #$11,n0                                     ; 381100
145CA1: move     r1,r0                                       ; 223000
145CA2: move     m1,m0                                       ; 0460A1
145CA3: move     a,x:(r3)+                                   ; 565B00
145CA4: move     #$0,r0                                      ; 300000
145CA5: move     #$10,r1                                     ; 311000
145CA6: move     y:(r6+$36),r4                               ; 0B76D4 000036
145CA8: move     #$40,y0                                     ; 264000
145CA9: move     x:(r0)+,x0                                  ; 44D800
145CAA: mpy      y0,x0,a         x:(r1)+,x0                  ; 44D9D0
145CAB: mac      y0,x0,a         x:(r0)+,x0                  ; 44D8D2
145CAC: do       #<$8,>$145cb2                               ; 060880 145CB1
145CAE: mpy      y0,x0,b         x:(r1)+,x0                  ; 44D9D8
145CAF: mac      y0,x0,b         x:(r0)+,x0      a,y:(r4)+   ; B298DA
145CB0: mpy      y0,x0,a         x:(r1)+,x0                  ; 44D9D0
145CB1: mac      y0,x0,a         x:(r0)+,x0      b,y:(r4)+   ; B398D2
145CB2: move     m0,m4                                       ; 0464A0
145CB3: move     r4,y:(r6+$36)                               ; 0B7694 000036
145CB5: move     #$0,r0                                      ; 300000
145CB6: move     #$f,r3                                      ; 330F00
145CB7: move     r0,r4                                       ; 221400
145CB8: move     #$80,r1                                     ; 318000
145CB9: move     #$90,r2                                     ; 329000
145CBA: move     #$c0,r5                                     ; 35C000
145CBB: move     #$3,m5                                      ; 0503A5
145CBC: move     y:>$c4,y1                                   ; 4FF000 0000C4
145CBE: move     x:(r0)+,x1                                  ; 45D800
145CBF: move     x:(r3)+,b                                   ; 57DB00
145CC0: move     x:(r3)-,x0                                  ; 44D300
145CC1: move     #$2,n3                                      ; 3B0200
145CC2: do       #<$10,>$145ccb                              ; 061080 145CCA
145CC4: mpy      y1,x1,a         b,x:(r3)+n3                 ; 574BF0
145CC5: mpy      x0,y1,b         x:(r1)+,x1      y:(r5)+,y0  ; F4B9C8
145CC6: mac      x1,y0,a         y:(r5)+,y0                  ; 4EDDE2
145CC7: mac      x1,y0,b         x:(r2)+,x1      y:(r5)+,y0  ; F4BAEA
145CC8: mac      x1,y0,a         x:(r3)-,x0      y:(r5)+,y0  ; F0B3E2
145CC9: mac      x1,y0,b         x:(r0)+,x1                  ; 45D8EA
145CCA: move     a,x:(r4)+                                   ; 565C00
145CCB: move     b,x:(r3)+                                   ; 575B00
145CCC: move     m4,m5                                       ; 0465A4
145CCD: jmp      func_000981                                 ; 0C0981
145CCE: move     #>$500,r6                                   ; 66F400 000500
145CD0: move     #>$100,n6                                   ; 76F400 000100
145CD2: move     #>$120000,r5                                ; 65F400 120000
145CD4: move     #>$8000,n5                                  ; 75F400 008000
145CD6: do       #<$3,>$145d11                               ; 060380 145D10
145CD8: move     r6,r4                                       ; 22D400
145CD9: move     #>$2,x0                                     ; 44F400 000002
145CDB: move     x0,y:(r6+$0)                                ; 0206A4
145CDC: move     #$0,x0                                      ; 240000
145CDD: move     #$0,x1                                      ; 250000
145CDE: rep      #<$34                                       ; 0634A0
145CDF: move     x,l:(r4)+                                   ; 425C00
145CE0: move     x0,y:(r6+$d7)                               ; 0B7684 0000D7
145CE2: move     x0,x:(r6+$fa)                               ; 0A7684 0000FA
145CE4: move     x0,y:(r6+$fa)                               ; 0B7684 0000FA
145CE6: move     x0,y:(r6+$21)                               ; 0286E4
145CE7: move     x0,y:(r6+$12)                               ; 024EA4
145CE8: move     x0,x:(r6+$fc)                               ; 0A7684 0000FC
145CEA: move     x0,y:(r6+$fc)                               ; 0B7684 0000FC
145CEC: move     r5,y:(r6+$d1)                               ; 0B7695 0000D1
145CEE: move     x0,x:(r6+$d6)                               ; 0A7684 0000D6
145CF0: move     x0,y:(r6+$d6)                               ; 0B7684 0000D6
145CF2: move     x0,y:(r6+$d5)                               ; 0B7684 0000D5
145CF4: move     x0,x:(r6+$d5)                               ; 0A7684 0000D5
145CF6: move     x0,y:(r6+$d4)                               ; 0B7684 0000D4
145CF8: move     x0,x:(r6+$d4)                               ; 0A7684 0000D4
145CFA: move     x0,y:(r6+$d3)                               ; 0B7684 0000D3
145CFC: move     x0,x:(r6+$d3)                               ; 0A7684 0000D3
145CFE: move     x0,y:(r6+$d0)                               ; 0B7684 0000D0
145D00: move     x0,y:(r6+$cf)                               ; 0B7684 0000CF
145D02: move     x0,y:(r6+$cb)                               ; 0B7684 0000CB
145D04: move     x0,x:(r6+$cb)                               ; 0A7684 0000CB
145D06: move     #>$2000,x0                                  ; 44F400 002000
145D08: move     x0,x:(r6+$d0)                               ; 0A7684 0000D0
145D0A: move     #>$1,x0                                     ; 44F400 000001
145D0C: move     x0,x:(r6+$cf)                               ; 0A7684 0000CF
145D0E: lua      (r6)+n6,r6                                  ; 044E16
145D0F: lua      (r5)+n5,r5                                  ; 044D15
145D10: nop                                                  ; 000000
145D11: rts                                                  ; 00000C
145D12: lua      (r6+$11),r0                                 ; 040E10
145D13: move     #>$14a000,x0                                ; 44F400 14A000
145D15: move     #$0,x1                                      ; 250000
145D16: do       #<$18,>$145d19                              ; 061880 145D18
145D18: move     x1,y:(r0)+                                  ; 4D5800
145D19: move     x0,y:(r6+$10)                               ; 0246A4
145D1A: move     x0,y:(r6+$14)                               ; 0256A4
145D1B: move     x0,y:(r6+$16)                               ; 025EA4
145D1C: rts                                                  ; 00000C
145D1D: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145D1F: move     a,y:(r6+$2a)                                ; 02AEAE
145D20: rts                                                  ; 00000C
145D21: move     #>$be37c,x0                                 ; 44F400 0BE37C
145D23: move     a1,y1                                       ; 218700
145D24: move     a0,y0                                       ; 210600
145D25: mpysu    x0,y0,a                                     ; 01278D
145D26: dmac     ss x0,y1,a                                  ; 012484
145D27: nop                                                  ; 000000
145D28: move     a,l:?:>$5                                   ; 487000 000005
145D2A: move     #>$1fff,m2                                  ; 05F422 001FFF
145D2C: move     #>$1fff,m0                                  ; 05F420 001FFF
145D2E: move     #>$141a80,r1                                ; 61F400 141A80
145D30: move     y:(r6+$8),b                                 ; 0226BF
145D31: add      #>$8000,b                                   ; 0140C8 008000
145D33: move     b,x0                                        ; 21E400
145D34: move     #>$18,x1                                    ; 45F400 000018
145D36: mpy      x1,x0,b                                     ; 2000A8
145D37: move     b1,n1                                       ; 21B900
145D38: move     y:(r1+n1),x0                                ; 4CE900
145D39: move     l:?:>$5,a                                   ; 48F000 000005
145D3B: move     a1,y1                                       ; 218700
145D3C: move     a0,y0                                       ; 210600
145D3D: mpysu    x0,y0,a                                     ; 01278D
145D3E: dmac     ss x0,y1,a                                  ; 012484
145D3F: asl      #$3,a,a                                     ; 0C1D06
145D40: move     y:(r6+$12),b                                ; 024EBF
145D41: move     y:(r6+$13),b0                               ; 024EF9
145D42: move     a1,y:(r6+$12)                               ; 024EAC
145D43: move     a0,y:(r6+$13)                               ; 024EE8
145D44: sub      b,a                                         ; 200014
145D45: asr      #$5,a,a                                     ; 0C1C0A
145D46: move     #>$20,r1                                    ; 61F400 000020
145D48: move     a1,y1                                       ; 218700
145D49: move     a0,y0                                       ; 210600
145D4A: tfr      b,a                                         ; 200001
145D4B: move     y:(r6+$10),b                                ; 0246BF
145D4C: move     y:(r6+$11),b0                               ; 0246F9
145D4D: move     #>$1fff,x0                                  ; 44F400 001FFF
145D4F: move     #>$14a000,x1                                ; 45F400 14A000
145D51: do       #<$20,>$145d57                              ; 062080 145D56
145D53: add      y,a                                         ; 200030
145D54: add      a,b             b,l:(r1)+                   ; 495918
145D55: and      x0,b                                        ; 20004E
145D56: add      x1,b                                        ; 200068
145D57: move     b1,y:(r6+$10)                               ; 0246AD
145D58: move     b0,y:(r6+$11)                               ; 0246E9
145D59: move     #>$20,r1                                    ; 61F400 000020
145D5B: move     #>$80,r5                                    ; 65F400 000080
145D5D: move     r1,r4                                       ; 223400
145D5E: move     x:(r1)+,r2                                  ; 62D900
145D5F: move     y:(r4)+,y0                                  ; 4EDC00
145D60: move     x:(r2)+,x1                                  ; 45DA00
145D61: move     x:(r1)+,r0                                  ; 60D900
145D62: do       #<$10,>$145d70                              ; 061080 145D6F
145D64: mpysu    -x1,y0,a                                    ; 012796
145D65: add      x1,a            x:(r2),x0                   ; 44E260
145D66: add      x1,a            x:(r1)+,r2                  ; 62D960
145D67: macsu    x0,y0,a                                     ; 01268D
145D68: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145D69: mpysu    -x1,y0,b                                    ; 0127B6
145D6A: add      x1,b            x:(r0),x0                   ; 44E068
145D6B: add      x1,b            x:(r1)+,r0                  ; 60D968
145D6C: macsu    x0,y0,b                                     ; 0126AD
145D6D: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
145D6E: move     a,y:(r5)+                                   ; 5E5D00
145D6F: move     b,y:(r5)+                                   ; 5F5D00
145D70: move     #>$80,r4                                    ; 64F400 000080
145D72: move     #>$80,r5                                    ; 65F400 000080
145D74: move     y:(r6+$1a),x0                               ; 026EB4
145D75: do       #<$20,>$145d7a                              ; 062080 145D79
145D77: move     y:(r5)+,b                                   ; 5FDD00
145D78: sub      x0,b            b,x0                        ; 21E44C
145D79: move     b,y:(r4)+                                   ; 5F5C00
145D7A: move     x0,y:(r6+$1a)                               ; 026EA4
145D7B: move     y:(r6+$28),a                                ; 02A6BE
145D7C: move     #>$80,r5                                    ; 65F400 000080
145D7E: move     #>$80,r4                                    ; 64F400 000080
145D80: move     #>$144ac7,r2                                ; 62F400 144AC7
145D82: move     y:(r6+$a),b                                 ; 022EBF
145D83: asr      #$10,b,b                                    ; 0C1CA1
145D84: move     b,n2                                        ; 21FA00
145D85: move     y:(r5)+,x0                                  ; 4CDD00
145D86: move     y:(r2+n2),y0                                ; 4EEA00
145D87: do       #<$20,>$145d8b                              ; 062080 145D8A
145D89: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145D8A: mac      -x1,y0,a        y:(r5)+,x0                  ; 4CDDE6
145D8B: move     a,y:(r6+$28)                                ; 02A6AE
145D8C: move     y:(r6+$9),b                                 ; 0226FF
145D8D: clr      a                                           ; 200013
145D8E: cmp      #>$400000,b                                 ; 0140CD 400000
145D90: blt      func_145d96                                 ; 0D1049 000006
145D92: sub      #>$400000,b                                 ; 0140CC 400000
145D94: add      a,b                                         ; 200018
145D95: move     b,a                                         ; 21EE00
145D96: move     a,y1                                        ; 21C700
145D97: move     y:(r6+$1e),x0                               ; 027EB4
145D98: move     #>$80,r0                                    ; 60F400 000080
145D9A: do       #<$20,>$145da0                              ; 062080 145D9F
145D9C: mpy      x0,y1,a         y:(r0),x1                   ; 4DE0C0
145D9D: asl      #$4,a,a                                     ; 0C1D08
145D9E: add      x1,a            x1,x0                       ; 20A460
145D9F: move     a,y:(r0)+                                   ; 5E5800
145DA0: move     x0,y:(r6+$1e)                               ; 027EA4
145DA1: move     l:?:>$5,a                                   ; 48F000 000005
145DA3: move     #>$141a80,r1                                ; 61F400 141A80
145DA5: move     y:(r6+$4),b                                 ; 0216BF
145DA6: add      #>$8000,b                                   ; 0140C8 008000
145DA8: move     b,x0                                        ; 21E400
145DA9: move     #>$18,x1                                    ; 45F400 000018
145DAB: mpy      x1,x0,b                                     ; 2000A8
145DAC: move     b1,n1                                       ; 21B900
145DAD: move     y:(r1+n1),x0                                ; 4CE900
145DAE: move     y:(r6+$5),b                                 ; 0216FF
145DAF: sub      #>$400000,b                                 ; 0140CC 400000
145DB1: asr      #$2,b,b                                     ; 0C1C85
145DB2: add      #>$400000,b                                 ; 0140C8 400000
145DB4: move     b,x1                                        ; 21E500
145DB5: mpy      x1,x0,b                                     ; 2000A8
145DB6: asl      b                                           ; 20003A
145DB7: move     b,x0                                        ; 21E400
145DB8: move     a1,y1                                       ; 218700
145DB9: move     a0,y0                                       ; 210600
145DBA: mpysu    x0,y0,a                                     ; 01278D
145DBB: dmac     ss x0,y1,a                                  ; 012484
145DBC: asl      #$3,a,a                                     ; 0C1D06
145DBD: move     a1,y1                                       ; 218700
145DBE: move     a0,y0                                       ; 210600
145DBF: move     y:(r6+$14),b                                ; 0256BF
145DC0: move     y:(r6+$15),b0                               ; 0256F9
145DC1: move     y:(r6+$7),a                                 ; 021EFE
145DC2: asr      #$b,a,a                                     ; 0C1C16
145DC3: move     #>$c0,r5                                    ; 65F400 0000C0
145DC5: move     a,x0                                        ; 21C400
145DC6: move     y:(r6+$1c),a                                ; 0276BE
145DC7: move     y:(r6+$1d),a0                               ; 0276F8
145DC8: do       #<$20,>$145dd4                              ; 062080 145DD3
145DCA: add      y,b                                         ; 200038
145DCB: add      a,b                                         ; 200018
145DCC: and      #>$1fff,b                                   ; 0140CE 001FFF
145DCE: add      #>$14a000,b                                 ; 0140C8 14A000
145DD0: move     b1,r3                                       ; 21B300
145DD1: sub      a,b                                         ; 20001C
145DD2: move     x:(r3),x1                                   ; 45E300
145DD3: mpy      x1,x0,a         x1,y:(r5)+                  ; 4D5DA0
145DD4: move     b1,y:(r6+$14)                               ; 0256AD
145DD5: move     b0,y:(r6+$15)                               ; 0256E9
145DD6: move     a1,y:(r6+$1c)                               ; 0276AC
145DD7: move     a0,y:(r6+$1d)                               ; 0276E8
145DD8: move     #>$c0,r5                                    ; 65F400 0000C0
145DDA: move     #>$c0,r4                                    ; 64F400 0000C0
145DDC: move     y:(r6+$1b),x0                               ; 026EF4
145DDD: do       #<$20,>$145de2                              ; 062080 145DE1
145DDF: move     y:(r5)+,b                                   ; 5FDD00
145DE0: sub      x0,b            b,x0                        ; 21E44C
145DE1: move     b,y:(r4)+                                   ; 5F5C00
145DE2: move     x0,y:(r6+$1b)                               ; 026EE4
145DE3: move     y:(r6+$29),a                                ; 02A6FE
145DE4: move     #>$c0,r5                                    ; 65F400 0000C0
145DE6: move     #>$c0,r4                                    ; 64F400 0000C0
145DE8: move     #>$144ac7,r2                                ; 62F400 144AC7
145DEA: move     y:(r6+$a),b                                 ; 022EBF
145DEB: asr      #$10,b,b                                    ; 0C1CA1
145DEC: move     b,n2                                        ; 21FA00
145DED: move     y:(r5)+,x0                                  ; 4CDD00
145DEE: move     y:(r2+n2),y0                                ; 4EEA00
145DEF: do       #<$20,>$145df3                              ; 062080 145DF2
145DF1: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145DF2: mac      -x1,y0,a        y:(r5)+,x0                  ; 4CDDE6
145DF3: move     a,y:(r6+$29)                                ; 02A6EE
145DF4: move     #>$80,r0                                    ; 60F400 000080
145DF6: move     #>$c0,r1                                    ; 61F400 0000C0
145DF8: move     #>$80,r2                                    ; 62F400 000080
145DFA: move     y:(r6+$9),x0                                ; 0226F4
145DFB: mpy      x0,x0,a                                     ; 200080
145DFC: asl      #$2,a,a                                     ; 0C1D04
145DFD: nop                                                  ; 000000
145DFE: move     a,y0                                        ; 21C600
145DFF: move     y:(r6+$6),b                                 ; 021EBF
145E00: move     y:(r6+$2a),y1                               ; 02AEB7
145E01: cmp      #>$400000,b                                 ; 0140CD 400000
145E03: blt      func_145e12                                 ; 0D1049 00000F
145E05: sub      #>$400000,b                                 ; 0140CC 400000
145E07: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145E09: move     b,x0                                        ; 21E400
145E0A: mpy      x0,x0,b                                     ; 200088
145E0B: move     b,x0                                        ; 21E400
145E0C: mac      -x0,x0,a                                    ; 200086
145E0D: move     a,x0                                        ; 21C400
145E0E: mpy      x0,y1,a                                     ; 2000C0
145E0F: nop                                                  ; 000000
145E10: move     a,y1                                        ; 21C700
145E11: move     y1,y:(r6+$2a)                               ; 02AEA7
145E12: move     y:(r6+$6),x0                                ; 021EB4
145E13: mpy      x0,x0,a                                     ; 200080
145E14: asl      #$2,a,a                                     ; 0C1D04
145E15: move     a,x0                                        ; 21C400
145E16: mpy      x0,y1,a                                     ; 2000C0
145E17: move     a,y1                                        ; 21C700
145E18: move     y:(r0)+,x0                                  ; 4CD800
145E19: move     y:(r1)+,x1                                  ; 4DD900
145E1A: mpy      y0,x0,a                                     ; 2000D0
145E1B: mpy      y1,x1,b         y:(r0)+,x0                  ; 4CD8F8
145E1C: add      a,b             y:(r1)+,x1                  ; 4DD918
145E1D: do       #<$1f,>$145e22                              ; 061F80 145E21
145E1F: mpy      y0,x0,a         b,x:(r2)+                   ; 575AD0
145E20: mpy      y1,x1,b         y:(r0)+,x0                  ; 4CD8F8
145E21: add      a,b             y:(r1)+,x1                  ; 4DD918
145E22: move     b,x:(r2)+                                   ; 575A00
145E23: move     y:(r6+$27),a                                ; 029EFE
145E24: move     #>$80,r5                                    ; 65F400 000080
145E26: move     #>$80,r4                                    ; 64F400 000080
145E28: move     #>$144ac7,r2                                ; 62F400 144AC7
145E2A: move     y:(r6+$a),b                                 ; 022EBF
145E2B: asr      #$10,b,b                                    ; 0C1CA1
145E2C: move     b,n2                                        ; 21FA00
145E2D: move     x:(r5)+,x0                                  ; 44DD00
145E2E: move     y:(r2+n2),y0                                ; 4EEA00
145E2F: do       #<$20,>$145e33                              ; 062080 145E32
145E31: mac      y0,x0,a         a,x:(r4)+       a,y1        ; 191CD2
145E32: mac      -y1,y0,a        x:(r5)+,x0                  ; 44DDB6
145E33: move     a,y:(r6+$27)                                ; 029EEE
145E34: move     l:?:>$5,a                                   ; 48F000 000005
145E36: move     y:(r6+$18),b                                ; 0266BF
145E37: move     y:(r6+$19),b0                               ; 0266F9
145E38: move     a1,y:(r6+$18)                               ; 0266AC
145E39: move     a0,y:(r6+$19)                               ; 0266E8
145E3A: sub      b,a                                         ; 200014
145E3B: asr      #$5,a,a                                     ; 0C1C0A
145E3C: move     #>$20,r1                                    ; 61F400 000020
145E3E: move     #>$80,r0                                    ; 60F400 000080
145E40: move     a1,y1                                       ; 218700
145E41: move     a0,y0                                       ; 210600
145E42: tfr      b,a                                         ; 200001
145E43: move     y:(r6+$16),b                                ; 025EBF
145E44: move     y:(r6+$17),b0                               ; 025EF9
145E45: do       #<$20,>$145e54                              ; 062080 145E53
145E47: add      y,a                                         ; 200030
145E48: add      a,b             b,l:(r1)+                   ; 495918
145E49: move     a1,x1                                       ; 218500
145E4A: move     a0,x0                                       ; 210400
145E4B: move     x:(r0)+,a                                   ; 56D800
145E4C: asr      #$c,a,a                                     ; 0C1C18
145E4D: add      a,b                                         ; 200018
145E4E: and      #>$1fff,b                                   ; 0140CE 001FFF
145E50: add      #>$14a000,b                                 ; 0140C8 14A000
145E52: move     x1,a                                        ; 20AE00
145E53: move     x0,a0                                       ; 208800
145E54: move     b1,y:(r6+$16)                               ; 025EAD
145E55: move     b0,y:(r6+$17)                               ; 025EE9
145E56: move     #>$20,r1                                    ; 61F400 000020
145E58: move     #>$e0,r3                                    ; 63F400 0000E0
145E5A: move     r1,r4                                       ; 223400
145E5B: move     x:(r1)+,r2                                  ; 62D900
145E5C: move     y:(r4)+,y0                                  ; 4EDC00
145E5D: move     x:(r2)+,x1                                  ; 45DA00
145E5E: move     x:(r1)+,r0                                  ; 60D900
145E5F: do       #<$10,>$145e71                              ; 061080 145E70
145E61: mpysu    -x1,y0,a                                    ; 012796
145E62: add      x1,a            x:(r2),x0                   ; 44E260
145E63: add      x1,a            x:(r1)+,r2                  ; 62D960
145E64: macsu    x0,y0,a                                     ; 01268D
145E65: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145E66: asr      a                                           ; 200022
145E67: asr      a                                           ; 200022
145E68: mpysu    -x1,y0,b                                    ; 0127B6
145E69: add      x1,b            x:(r0),x0                   ; 44E068
145E6A: add      x1,b            x:(r1)+,r0                  ; 60D968
145E6B: macsu    x0,y0,b                                     ; 0126AD
145E6C: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
145E6D: asr      b                                           ; 20002A
145E6E: asr      b                                           ; 20002A
145E6F: move     a,x:(r3)+                                   ; 565B00
145E70: move     b,x:(r3)+                                   ; 575B00
145E71: move     #>$de,r0                                    ; 60F400 0000DE
145E73: move     #>$e0,r1                                    ; 61F400 0000E0
145E75: move     y:(r6+$1f),x0                               ; 027EF4
145E76: move     x0,x:(r0)+                                  ; 445800
145E77: move     y:(r6+$20),x0                               ; 0286B4
145E78: move     x0,x:(r0)-                                  ; 445000
145E79: move     #>$cfce3,x1                                 ; 45F400 0CFCE3
145E7B: move     #>$2bc9ca,y0                                ; 46F400 2BC9CA
145E7D: move     y:(r6+$23),y1                               ; 028EF7
145E7E: move     y:(r6+$24),b                                ; 0296BF
145E7F: move     #>$1e,r4                                    ; 64F400 00001E
145E81: move     x:(r1)+,x0                                  ; 44D900
145E82: move     x:(r0)+,a                                   ; 56D800
145E83: do       #<$10,>$145e89                              ; 061080 145E88
145E85: mac      y0,x0,a         x:(r1)+,x0      y1,y:(r4)+  ; B199D2
145E86: mac      -y1,y0,a        x:(r0)+,b       b,y1        ; 1F98B6
145E87: mac      x1,x0,b         x:(r1)+,x0      y1,y:(r4)+  ; B199AA
145E88: mac      -y1,x1,b        x:(r0)+,a       a,y1        ; 1998FE
145E89: move     y1,y:(r4)+                                  ; 4F5C00
145E8A: move     b,y:(r4)+                                   ; 5F5C00
145E8B: move     y1,y:(r6+$23)                               ; 028EE7
145E8C: move     b,y:(r6+$24)                                ; 0296AF
145E8D: move     a,y:(r6+$1f)                                ; 027EEE
145E8E: move     x:(r0)+,x0                                  ; 44D800
145E8F: move     x0,y:(r6+$20)                               ; 0286A4
145E90: move     #>$1e,r4                                    ; 64F400 00001E
145E92: move     #>$20,r5                                    ; 65F400 000020
145E94: move     y:(r6+$21),x0                               ; 0286F4
145E95: move     x0,y:(r4)+                                  ; 4C5C00
145E96: move     y:(r6+$22),x0                               ; 028EB4
145E97: move     x0,y:(r4)-                                  ; 4C5400
145E98: move     #>$4e63df,y1                                ; 47F400 4E63DF
145E9A: move     #>$6f0f12,x0                                ; 44F400 6F0F12
145E9C: move     y:(r6+$25),x1                               ; 0296F5
145E9D: move     y:(r6+$26),b                                ; 029EBF
145E9E: move     #>$1e,r0                                    ; 60F400 00001E
145EA0: move     y:(r5)+,y0                                  ; 4EDD00
145EA1: move     y:(r4)+,a                                   ; 5EDC00
145EA2: do       #<$10,>$145ea8                              ; 061080 145EA7
145EA4: mac      y0,x0,a         x1,x:(r0)+      y:(r5)+,y0  ; F438D2
145EA5: mac      -x1,x0,a        b,x1            y:(r4)+,b   ; 1FDCA6
145EA6: mac      y1,y0,b         x1,x:(r0)+      y:(r5)+,y0  ; F438BA
145EA7: mac      -y1,x1,b        a,x1            y:(r4)+,a   ; 16DCFE
145EA8: move     x1,x:(r0)+                                  ; 455800
145EA9: move     b,x:(r0)+                                   ; 575800
145EAA: move     x1,y:(r6+$25)                               ; 0296E5
145EAB: move     b,y:(r6+$26)                                ; 029EAF
145EAC: move     a,y:(r6+$21)                                ; 0286EE
145EAD: move     y:(r4)+,x0                                  ; 4CDC00
145EAE: move     x0,y:(r6+$22)                               ; 028EA4
145EAF: move     #>$20,r0                                    ; 60F400 000020
145EB1: move     #>$1f,r4                                    ; 64F400 00001F
145EB3: move     #$40,y0                                     ; 264000
145EB4: move     x:(r0)+,x0                                  ; 44D800
145EB5: do       #<$8,>$145ebb                               ; 060880 145EBA
145EB7: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
145EB8: mac      y0,x0,a         x:(r0)+,x0      b,y:(r4)+   ; B398D2
145EB9: mpy      y0,x0,b         x:(r0)+,x0                  ; 44D8D8
145EBA: mac      y0,x0,b         x:(r0)+,x0      a,y:(r4)+   ; B298DA
145EBB: move     b,y:(r4)+                                   ; 5F5C00
145EBC: move     #>$20,r4                                    ; 64F400 000020
145EBE: do       #<$8,>$145ec8                               ; 060880 145EC7
145EC0: move     y:(r4)+,a                                   ; 5EDC00
145EC1: move     y:(r4)+,b                                   ; 5FDC00
145EC2: asr      a                                           ; 200022
145EC3: asr      b                                           ; 20002A
145EC4: move     a,y:(r7)+                                   ; 5E5F00
145EC5: move     a,y:(r7)+                                   ; 5E5F00
145EC6: move     b,y:(r7)+                                   ; 5F5F00
145EC7: move     b,y:(r7)+                                   ; 5F5F00
145EC8: rts                                                  ; 00000C
145EC9: lua      (r6+$11),r0                                 ; 040E10
145ECA: move     #>$14a000,x0                                ; 44F400 14A000
145ECC: move     #$0,x1                                      ; 250000
145ECD: rep      #<$20                                       ; 0620A0
145ECE: move     x1,y:(r0)+                                  ; 4D5800
145ECF: move     x0,y:(r6+$10)                               ; 0246A4
145ED0: move     x0,y:(r6+$14)                               ; 0256A4
145ED1: move     x0,y:(r6+$18)                               ; 0266A4
145ED2: move     x0,y:(r6+$1c)                               ; 0276A4
145ED3: rts                                                  ; 00000C
145ED4: move     #>$7fffff,a                                 ; 56F400 7FFFFF
145ED6: nop                                                  ; 000000
145ED7: move     a,y:(r6+$32)                                ; 02CEAE
145ED8: move     a,y:(r6+$33)                                ; 02CEEE
145ED9: move     a,y:(r6+$34)                                ; 02D6AE
145EDA: rts                                                  ; 00000C
145EDB: move     #>$be37c,x0                                 ; 44F400 0BE37C
145EDD: move     a1,y1                                       ; 218700
145EDE: move     a0,y0                                       ; 210600
145EDF: mpysu    x0,y0,a                                     ; 01278D
145EE0: dmac     ss x0,y1,a                                  ; 012484
145EE1: move     #>$141a80,r1                                ; 61F400 141A80
145EE3: move     y:(r6+$4),b                                 ; 0216BF
145EE4: add      #>$8000,b                                   ; 0140C8 008000
145EE6: move     #>$18,x1                                    ; 45F400 000018
145EE8: move     b,x0                                        ; 21E400
145EE9: mpy      x1,x0,b                                     ; 2000A8
145EEA: move     a,l:?:>$5                                   ; 487000 000005
145EEC: move     b1,n1                                       ; 21B900
145EED: move     a1,y1                                       ; 218700
145EEE: move     a0,y0                                       ; 210600
145EEF: move     #>$1fff,m2                                  ; 05F422 001FFF
145EF1: move     #>$1fff,m0                                  ; 05F420 001FFF
145EF3: move     y:(r1+n1),x0                                ; 4CE900
145EF4: mpysu    x0,y0,a                                     ; 01278D
145EF5: dmac     ss x0,y1,a                                  ; 012484
145EF6: asl      #$3,a,a                                     ; 0C1D06
145EF7: move     y:(r6+$12),b                                ; 024EBF
145EF8: move     y:(r6+$13),b0                               ; 024EF9
145EF9: move     a1,y:(r6+$12)                               ; 024EAC
145EFA: move     a0,y:(r6+$13)                               ; 024EE8
145EFB: sub      b,a                                         ; 200014
145EFC: asr      #$5,a,a                                     ; 0C1C0A
145EFD: move     #>$20,r1                                    ; 61F400 000020
145EFF: move     a1,y1                                       ; 218700
145F00: move     a0,y0                                       ; 210600
145F01: tfr      b,a                                         ; 200001
145F02: move     y:(r6+$10),b                                ; 0246BF
145F03: move     y:(r6+$11),b0                               ; 0246F9
145F04: move     #>$1fff,x0                                  ; 44F400 001FFF
145F06: move     #>$14a000,x1                                ; 45F400 14A000
145F08: do       #<$20,>$145f0e                              ; 062080 145F0D
145F0A: add      y,a                                         ; 200030
145F0B: add      a,b             b,l:(r1)+                   ; 495918
145F0C: and      x0,b                                        ; 20004E
145F0D: add      x1,b                                        ; 200068
145F0E: move     #>$20,r1                                    ; 61F400 000020
145F10: move     #>$80,r5                                    ; 65F400 000080
145F12: move     b1,y:(r6+$10)                               ; 0246AD
145F13: move     r1,r4                                       ; 223400
145F14: move     x:(r1)+,r2                                  ; 62D900
145F15: move     b0,y:(r6+$11)                               ; 0246E9
145F16: move     y:(r4)+,y0                                  ; 4EDC00
145F17: move     x:(r1)+,r0                                  ; 60D900
145F18: move     x:(r2)+,x1                                  ; 45DA00
145F19: mpysu    -x1,y0,a                                    ; 012796
145F1A: add      x1,a            x:(r2),x0                   ; 44E260
145F1B: add      x1,a            x:(r1)+,r2                  ; 62D960
145F1C: macsu    x0,y0,a                                     ; 01268D
145F1D: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145F1E: mpysu    -x1,y0,b                                    ; 0127B6
145F1F: add      x1,b            x:(r0),x0       a,y:(r5)+   ; B2A068
145F20: add      x1,b            x:(r1)+,r0                  ; 60D968
145F21: macsu    x0,y0,b                                     ; 0126AD
145F22: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
145F23: do       #<$f,>$145f2f                               ; 060F80 145F2E
145F25: mpysu    -x1,y0,a                                    ; 012796
145F26: add      x1,a            x:(r2),x0       b,y:(r5)+   ; B3A260
145F27: add      x1,a            x:(r1)+,r2                  ; 62D960
145F28: macsu    x0,y0,a                                     ; 01268D
145F29: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145F2A: mpysu    -x1,y0,b                                    ; 0127B6
145F2B: add      x1,b            x:(r0),x0       a,y:(r5)+   ; B2A068
145F2C: add      x1,b            x:(r1)+,r0                  ; 60D968
145F2D: macsu    x0,y0,b                                     ; 0126AD
145F2E: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
145F2F: move     y:(r6+$20),x0                               ; 0286B4
145F30: move     b,y:(r5)+                                   ; 5F5D00
145F31: move     #>$80,r5                                    ; 65F400 000080
145F33: move     #>$80,r3                                    ; 63F400 000080
145F35: move     y:(r5)+,b                                   ; 5FDD00
145F36: move     y:(r5)+,a                                   ; 5EDD00
145F37: sub      x0,b            b,x0                        ; 21E44C
145F38: sub      x0,a            a,x0                        ; 21C444
145F39: do       #<$f,>$145f3f                               ; 060F80 145F3E
145F3B: move     b,x:(r3)+       y:(r5)+,b                   ; FF3B00
145F3C: move     a,x:(r3)+       y:(r5)+,a                   ; FA3B00
145F3D: sub      x0,b            b,x0                        ; 21E44C
145F3E: sub      x0,a            a,x0                        ; 21C444
145F3F: move     b,x:(r3)+                                   ; 575B00
145F40: move     a,x:(r3)+                                   ; 565B00
145F41: move     x0,y:(r6+$20)                               ; 0286A4
145F42: move     #>$80,r5                                    ; 65F400 000080
145F44: move     #>$144ac7,r2                                ; 62F400 144AC7
145F46: move     y:(r6+$a),b                                 ; 022EBF
145F47: asr      #$10,b,b                                    ; 0C1CA1
145F48: move     y:(r6+$2c),a                                ; 02B6BE
145F49: move     b,n2                                        ; 21FA00
145F4A: move     #>$80,r4                                    ; 64F400 000080
145F4C: move     x:(r5)+,x0                                  ; 44DD00
145F4D: move     y:(r2+n2),y0                                ; 4EEA00
145F4E: do       #<$20,>$145f52                              ; 062080 145F51
145F50: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145F51: mac      -x1,y0,a        x:(r5)+,x0                  ; 44DDE6
145F52: move     a,y:(r6+$2c)                                ; 02B6AE
145F53: move     y:(r6+$5),b                                 ; 0216FF
145F54: clr      a                                           ; 200013
145F55: cmp      #>$400000,b                                 ; 0140CD 400000
145F57: blt      func_145f5e                                 ; 0D1049 000007
145F59: sub      #>$400000,b                                 ; 0140CC 400000
145F5B: add      a,b                                         ; 200018
145F5C: nop                                                  ; 000000
145F5D: move     b,a                                         ; 21EE00
145F5E: move     y:(r6+$2f),x0                               ; 02BEF4
145F5F: move     a,y1                                        ; 21C700
145F60: move     #>$80,r0                                    ; 60F400 000080
145F62: move     #>$80,r1                                    ; 61F400 000080
145F64: do       #<$10,>$145f6e                              ; 061080 145F6D
145F66: mpy      x0,y1,a         y:(r0)+,x1                  ; 4DD8C0
145F67: asl      #$4,a,a                                     ; 0C1D08
145F68: add      x1,a            x1,x0                       ; 20A460
145F69: mpy      x0,y1,b         y:(r0)+,x1                  ; 4DD8C8
145F6A: asl      #$4,b,b                                     ; 0C1D89
145F6B: add      x1,b            x1,x0                       ; 20A468
145F6C: move     a,y:(r1)+                                   ; 5E5900
145F6D: move     b,y:(r1)+                                   ; 5F5900
145F6E: move     x0,y:(r6+$2f)                               ; 02BEE4
145F6F: move     #>$141a80,r1                                ; 61F400 141A80
145F71: move     y:(r6+$6),b                                 ; 021EBF
145F72: add      #>$8000,b                                   ; 0140C8 008000
145F74: move     #>$18,x1                                    ; 45F400 000018
145F76: move     b,x0                                        ; 21E400
145F77: mpy      x1,x0,b                                     ; 2000A8
145F78: move     l:?:>$5,a                                   ; 48F000 000005
145F7A: move     b1,n1                                       ; 21B900
145F7B: move     a1,y1                                       ; 218700
145F7C: move     a0,y0                                       ; 210600
145F7D: nop                                                  ; 000000
145F7E: move     y:(r1+n1),x0                                ; 4CE900
145F7F: mpysu    x0,y0,a                                     ; 01278D
145F80: dmac     ss x0,y1,a                                  ; 012484
145F81: asl      #$3,a,a                                     ; 0C1D06
145F82: move     y:(r6+$16),b                                ; 025EBF
145F83: move     y:(r6+$17),b0                               ; 025EF9
145F84: move     a1,y:(r6+$16)                               ; 025EAC
145F85: move     a0,y:(r6+$17)                               ; 025EE8
145F86: sub      b,a                                         ; 200014
145F87: asr      #$5,a,a                                     ; 0C1C0A
145F88: move     #>$40,r1                                    ; 61F400 000040
145F8A: move     a1,y1                                       ; 218700
145F8B: move     a0,y0                                       ; 210600
145F8C: tfr      b,a                                         ; 200001
145F8D: move     y:(r6+$14),b                                ; 0256BF
145F8E: move     y:(r6+$15),b0                               ; 0256F9
145F8F: move     #>$1fff,x0                                  ; 44F400 001FFF
145F91: move     #>$14a000,x1                                ; 45F400 14A000
145F93: do       #<$20,>$145f99                              ; 062080 145F98
145F95: add      y,a                                         ; 200030
145F96: add      a,b             b,l:(r1)+                   ; 495918
145F97: and      x0,b                                        ; 20004E
145F98: add      x1,b                                        ; 200068
145F99: move     #>$40,r1                                    ; 61F400 000040
145F9B: move     #>$c0,r5                                    ; 65F400 0000C0
145F9D: move     b1,y:(r6+$14)                               ; 0256AD
145F9E: move     r1,r4                                       ; 223400
145F9F: move     x:(r1)+,r2                                  ; 62D900
145FA0: move     b0,y:(r6+$15)                               ; 0256E9
145FA1: move     y:(r4)+,y0                                  ; 4EDC00
145FA2: move     x:(r1)+,r0                                  ; 60D900
145FA3: move     x:(r2)+,x1                                  ; 45DA00
145FA4: mpysu    -x1,y0,a                                    ; 012796
145FA5: add      x1,a            x:(r2),x0                   ; 44E260
145FA6: add      x1,a            x:(r1)+,r2                  ; 62D960
145FA7: macsu    x0,y0,a                                     ; 01268D
145FA8: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145FA9: mpysu    -x1,y0,b                                    ; 0127B6
145FAA: add      x1,b            x:(r0),x0       a,y:(r5)+   ; B2A068
145FAB: add      x1,b            x:(r1)+,r0                  ; 60D968
145FAC: macsu    x0,y0,b                                     ; 0126AD
145FAD: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
145FAE: do       #<$f,>$145fba                               ; 060F80 145FB9
145FB0: mpysu    -x1,y0,a                                    ; 012796
145FB1: add      x1,a            x:(r2),x0       b,y:(r5)+   ; B3A260
145FB2: add      x1,a            x:(r1)+,r2                  ; 62D960
145FB3: macsu    x0,y0,a                                     ; 01268D
145FB4: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
145FB5: mpysu    -x1,y0,b                                    ; 0127B6
145FB6: add      x1,b            x:(r0),x0       a,y:(r5)+   ; B2A068
145FB7: add      x1,b            x:(r1)+,r0                  ; 60D968
145FB8: macsu    x0,y0,b                                     ; 0126AD
145FB9: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
145FBA: move     y:(r6+$21),x0                               ; 0286F4
145FBB: move     b,y:(r5)+                                   ; 5F5D00
145FBC: move     #>$c0,r5                                    ; 65F400 0000C0
145FBE: move     #>$80,r3                                    ; 63F400 000080
145FC0: move     y:(r5)+,b                                   ; 5FDD00
145FC1: move     y:(r5)+,a                                   ; 5EDD00
145FC2: sub      x0,b            b,x0                        ; 21E44C
145FC3: sub      x0,a            a,x0                        ; 21C444
145FC4: do       #<$f,>$145fca                               ; 060F80 145FC9
145FC6: move     b,x:(r3)+       y:(r5)+,b                   ; FF3B00
145FC7: move     a,x:(r3)+       y:(r5)+,a                   ; FA3B00
145FC8: sub      x0,b            b,x0                        ; 21E44C
145FC9: sub      x0,a            a,x0                        ; 21C444
145FCA: move     b,x:(r3)+                                   ; 575B00
145FCB: move     a,x:(r3)+                                   ; 565B00
145FCC: move     x0,y:(r6+$21)                               ; 0286E4
145FCD: move     #>$80,r5                                    ; 65F400 000080
145FCF: move     #>$144ac7,r2                                ; 62F400 144AC7
145FD1: move     y:(r6+$a),b                                 ; 022EBF
145FD2: asr      #$10,b,b                                    ; 0C1CA1
145FD3: move     y:(r6+$2d),a                                ; 02B6FE
145FD4: move     b,n2                                        ; 21FA00
145FD5: move     #>$c0,r4                                    ; 64F400 0000C0
145FD7: move     x:(r5)+,x0                                  ; 44DD00
145FD8: move     y:(r2+n2),y0                                ; 4EEA00
145FD9: do       #<$20,>$145fdd                              ; 062080 145FDC
145FDB: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
145FDC: mac      -x1,y0,a        x:(r5)+,x0                  ; 44DDE6
145FDD: move     a,y:(r6+$2d)                                ; 02B6EE
145FDE: move     y:(r6+$7),b                                 ; 021EFF
145FDF: clr      a                                           ; 200013
145FE0: cmp      #>$400000,b                                 ; 0140CD 400000
145FE2: blt      func_145fe9                                 ; 0D1049 000007
145FE4: sub      #>$400000,b                                 ; 0140CC 400000
145FE6: add      a,b                                         ; 200018
145FE7: nop                                                  ; 000000
145FE8: move     b,a                                         ; 21EE00
145FE9: move     y:(r6+$30),x0                               ; 02C6B4
145FEA: move     a,y1                                        ; 21C700
145FEB: move     #>$c0,r0                                    ; 60F400 0000C0
145FED: move     #>$c0,r1                                    ; 61F400 0000C0
145FEF: do       #<$10,>$145ff9                              ; 061080 145FF8
145FF1: mpy      x0,y1,a         y:(r0)+,x1                  ; 4DD8C0
145FF2: asl      #$4,a,a                                     ; 0C1D08
145FF3: add      x1,a            x1,x0                       ; 20A460
145FF4: mpy      x0,y1,b         y:(r0)+,x1                  ; 4DD8C8
145FF5: asl      #$4,b,b                                     ; 0C1D89
145FF6: add      x1,b            x1,x0                       ; 20A468
145FF7: move     a,y:(r1)+                                   ; 5E5900
145FF8: move     b,y:(r1)+                                   ; 5F5900
145FF9: move     x0,y:(r6+$30)                               ; 02C6A4
145FFA: move     #>$141a80,r1                                ; 61F400 141A80
145FFC: move     y:(r6+$8),b                                 ; 0226BF
145FFD: add      #>$8000,b                                   ; 0140C8 008000
145FFF: move     #>$18,x1                                    ; 45F400 000018
146001: move     b,x0                                        ; 21E400
146002: mpy      x1,x0,b                                     ; 2000A8
146003: move     l:?:>$5,a                                   ; 48F000 000005
146005: move     b1,n1                                       ; 21B900
146006: move     a1,y1                                       ; 218700
146007: move     a0,y0                                       ; 210600
146008: nop                                                  ; 000000
146009: move     y:(r1+n1),x0                                ; 4CE900
14600A: mpysu    x0,y0,a                                     ; 01278D
14600B: dmac     ss x0,y1,a                                  ; 012484
14600C: asl      #$3,a,a                                     ; 0C1D06
14600D: move     y:(r6+$1a),b                                ; 026EBF
14600E: move     y:(r6+$1b),b0                               ; 026EF9
14600F: move     a1,y:(r6+$1a)                               ; 026EAC
146010: move     a0,y:(r6+$1b)                               ; 026EE8
146011: sub      b,a                                         ; 200014
146012: asr      #$5,a,a                                     ; 0C1C0A
146013: move     #>$20,r1                                    ; 61F400 000020
146015: move     a1,y1                                       ; 218700
146016: move     a0,y0                                       ; 210600
146017: tfr      b,a                                         ; 200001
146018: move     y:(r6+$18),b                                ; 0266BF
146019: move     y:(r6+$19),b0                               ; 0266F9
14601A: move     #>$1fff,x0                                  ; 44F400 001FFF
14601C: move     #>$14a000,x1                                ; 45F400 14A000
14601E: do       #<$20,>$146024                              ; 062080 146023
146020: add      y,a                                         ; 200030
146021: add      a,b             b,l:(r1)+                   ; 495918
146022: and      x0,b                                        ; 20004E
146023: add      x1,b                                        ; 200068
146024: move     #>$20,r1                                    ; 61F400 000020
146026: move     #>$e0,r5                                    ; 65F400 0000E0
146028: move     b1,y:(r6+$18)                               ; 0266AD
146029: move     r1,r4                                       ; 223400
14602A: move     x:(r1)+,r2                                  ; 62D900
14602B: move     b0,y:(r6+$19)                               ; 0266E9
14602C: move     y:(r4)+,y0                                  ; 4EDC00
14602D: move     x:(r1)+,r0                                  ; 60D900
14602E: move     x:(r2)+,x1                                  ; 45DA00
14602F: mpysu    -x1,y0,a                                    ; 012796
146030: add      x1,a            x:(r2),x0                   ; 44E260
146031: add      x1,a            x:(r1)+,r2                  ; 62D960
146032: macsu    x0,y0,a                                     ; 01268D
146033: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
146034: mpysu    -x1,y0,b                                    ; 0127B6
146035: add      x1,b            x:(r0),x0       a,y:(r5)+   ; B2A068
146036: add      x1,b            x:(r1)+,r0                  ; 60D968
146037: macsu    x0,y0,b                                     ; 0126AD
146038: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
146039: do       #<$f,>$146045                               ; 060F80 146044
14603B: mpysu    -x1,y0,a                                    ; 012796
14603C: add      x1,a            x:(r2),x0       b,y:(r5)+   ; B3A260
14603D: add      x1,a            x:(r1)+,r2                  ; 62D960
14603E: macsu    x0,y0,a                                     ; 01268D
14603F: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
146040: mpysu    -x1,y0,b                                    ; 0127B6
146041: add      x1,b            x:(r0),x0       a,y:(r5)+   ; B2A068
146042: add      x1,b            x:(r1)+,r0                  ; 60D968
146043: macsu    x0,y0,b                                     ; 0126AD
146044: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
146045: move     y:(r6+$22),x0                               ; 028EB4
146046: move     b,y:(r5)+                                   ; 5F5D00
146047: move     #>$e0,r5                                    ; 65F400 0000E0
146049: move     #>$80,r3                                    ; 63F400 000080
14604B: move     y:(r5)+,b                                   ; 5FDD00
14604C: move     y:(r5)+,a                                   ; 5EDD00
14604D: sub      x0,b            b,x0                        ; 21E44C
14604E: sub      x0,a            a,x0                        ; 21C444
14604F: do       #<$f,>$146055                               ; 060F80 146054
146051: move     b,x:(r3)+       y:(r5)+,b                   ; FF3B00
146052: move     a,x:(r3)+       y:(r5)+,a                   ; FA3B00
146053: sub      x0,b            b,x0                        ; 21E44C
146054: sub      x0,a            a,x0                        ; 21C444
146055: move     b,x:(r3)+                                   ; 575B00
146056: move     a,x:(r3)+                                   ; 565B00
146057: move     x0,y:(r6+$22)                               ; 028EA4
146058: move     #>$80,r5                                    ; 65F400 000080
14605A: move     #>$144ac7,r2                                ; 62F400 144AC7
14605C: move     y:(r6+$a),b                                 ; 022EBF
14605D: asr      #$10,b,b                                    ; 0C1CA1
14605E: move     y:(r6+$2e),a                                ; 02BEBE
14605F: move     b,n2                                        ; 21FA00
146060: move     #>$e0,r4                                    ; 64F400 0000E0
146062: move     x:(r5)+,x0                                  ; 44DD00
146063: move     y:(r2+n2),y0                                ; 4EEA00
146064: do       #<$20,>$146068                              ; 062080 146067
146066: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
146067: mac      -x1,y0,a        x:(r5)+,x0                  ; 44DDE6
146068: move     a,y:(r6+$2e)                                ; 02BEAE
146069: move     y:(r6+$9),b                                 ; 0226FF
14606A: clr      a                                           ; 200013
14606B: cmp      #>$400000,b                                 ; 0140CD 400000
14606D: blt      func_146074                                 ; 0D1049 000007
14606F: sub      #>$400000,b                                 ; 0140CC 400000
146071: add      a,b                                         ; 200018
146072: nop                                                  ; 000000
146073: move     b,a                                         ; 21EE00
146074: move     y:(r6+$31),x0                               ; 02C6F4
146075: move     a,y1                                        ; 21C700
146076: move     #>$e0,r0                                    ; 60F400 0000E0
146078: move     #>$e0,r1                                    ; 61F400 0000E0
14607A: do       #<$10,>$146084                              ; 061080 146083
14607C: mpy      x0,y1,a         y:(r0)+,x1                  ; 4DD8C0
14607D: asl      #$4,a,a                                     ; 0C1D08
14607E: add      x1,a            x1,x0                       ; 20A460
14607F: mpy      x0,y1,b         y:(r0)+,x1                  ; 4DD8C8
146080: asl      #$4,b,b                                     ; 0C1D89
146081: add      x1,b            x1,x0                       ; 20A468
146082: move     a,y:(r1)+                                   ; 5E5900
146083: move     b,y:(r1)+                                   ; 5F5900
146084: move     x0,y:(r6+$31)                               ; 02C6E4
146085: move     #>$80,r0                                    ; 60F400 000080
146087: move     #>$c0,r1                                    ; 61F400 0000C0
146089: move     #>$80,r2                                    ; 62F400 000080
14608B: move     y:(r6+$7),b                                 ; 021EFF
14608C: move     y:(r6+$33),y0                               ; 02CEF6
14608D: cmp      #>$400000,b                                 ; 0140CD 400000
14608F: blt      func_14609e                                 ; 0D1049 00000F
146091: sub      #>$400000,b                                 ; 0140CC 400000
146093: move     #>$7fffff,a                                 ; 56F400 7FFFFF
146095: move     b,x0                                        ; 21E400
146096: mpy      x0,x0,b                                     ; 200088
146097: move     b,x0                                        ; 21E400
146098: mac      -x0,x0,a                                    ; 200086
146099: move     a,x0                                        ; 21C400
14609A: mpy      y0,x0,a                                     ; 2000D0
14609B: nop                                                  ; 000000
14609C: move     a,y0                                        ; 21C600
14609D: move     y0,y:(r6+$33)                               ; 02CEE6
14609E: move     y:(r6+$7),x0                                ; 021EF4
14609F: mpy      x0,x0,a                                     ; 200080
1460A0: asl      #$2,a,a                                     ; 0C1D04
1460A1: move     a,x0                                        ; 21C400
1460A2: mpy      y0,x0,a                                     ; 2000D0
1460A3: move     a,y1                                        ; 21C700
1460A4: move     y:(r6+$5),b                                 ; 0216FF
1460A5: move     y:(r6+$32),y0                               ; 02CEB6
1460A6: cmp      #>$400000,b                                 ; 0140CD 400000
1460A8: blt      func_1460b7                                 ; 0D1049 00000F
1460AA: sub      #>$400000,b                                 ; 0140CC 400000
1460AC: move     #>$7fffff,a                                 ; 56F400 7FFFFF
1460AE: move     b,x0                                        ; 21E400
1460AF: mpy      x0,x0,b                                     ; 200088
1460B0: move     b,x0                                        ; 21E400
1460B1: mac      -x0,x0,a                                    ; 200086
1460B2: move     a,x0                                        ; 21C400
1460B3: mpy      y0,x0,a                                     ; 2000D0
1460B4: nop                                                  ; 000000
1460B5: move     a,y0                                        ; 21C600
1460B6: move     y0,y:(r6+$32)                               ; 02CEA6
1460B7: move     y:(r6+$5),x0                                ; 0216F4
1460B8: mpy      x0,x0,a                                     ; 200080
1460B9: asl      #$2,a,a                                     ; 0C1D04
1460BA: move     a,x0                                        ; 21C400
1460BB: mpy      y0,x0,a                                     ; 2000D0
1460BC: move     a,y0                                        ; 21C600
1460BD: move     y:(r0)+,x0                                  ; 4CD800
1460BE: move     y:(r1)+,x1                                  ; 4DD900
1460BF: mpy      y0,x0,a                                     ; 2000D0
1460C0: mac      y1,x1,a         y:(r0)+,x0                  ; 4CD8F2
1460C1: move     y:(r1)+,x1                                  ; 4DD900
1460C2: do       #<$1f,>$1460c7                              ; 061F80 1460C6
1460C4: mpy      y0,x0,a         a,x:(r2)+                   ; 565AD0
1460C5: mac      y1,x1,a         y:(r0)+,x0                  ; 4CD8F2
1460C6: move     y:(r1)+,x1                                  ; 4DD900
1460C7: move     a,x:(r2)+                                   ; 565A00
1460C8: move     #>$e0,r0                                    ; 60F400 0000E0
1460CA: move     #>$80,r1                                    ; 61F400 000080
1460CC: move     #>$80,r4                                    ; 64F400 000080
1460CE: move     y:(r6+$9),b                                 ; 0226FF
1460CF: move     y:(r6+$34),y0                               ; 02D6B6
1460D0: cmp      #>$400000,b                                 ; 0140CD 400000
1460D2: blt      func_1460e1                                 ; 0D1049 00000F
1460D4: sub      #>$400000,b                                 ; 0140CC 400000
1460D6: move     #>$7fffff,a                                 ; 56F400 7FFFFF
1460D8: move     b,x0                                        ; 21E400
1460D9: mpy      x0,x0,b                                     ; 200088
1460DA: move     b,x0                                        ; 21E400
1460DB: mac      -x0,x0,a                                    ; 200086
1460DC: move     a,x0                                        ; 21C400
1460DD: mpy      y0,x0,a                                     ; 2000D0
1460DE: nop                                                  ; 000000
1460DF: move     a,y0                                        ; 21C600
1460E0: move     y0,y:(r6+$34)                               ; 02D6A6
1460E1: move     y:(r6+$9),x0                                ; 0226F4
1460E2: mpy      x0,x0,a                                     ; 200080
1460E3: asl      #$2,a,a                                     ; 0C1D04
1460E4: move     a,x0                                        ; 21C400
1460E5: mpy      y0,x0,a                                     ; 2000D0
1460E6: move     a,y0                                        ; 21C600
1460E7: move     y:(r0)+,y1                                  ; 4FD800
1460E8: move     x:(r1)+,a                                   ; 56D900
1460E9: mac      y1,y0,a         y:(r0)+,y1                  ; 4FD8B2
1460EA: move     x:(r1)+,b                                   ; 57D900
1460EB: do       #<$f,>$1460f1                               ; 060F80 1460F0
1460ED: mac      y1,y0,b         a,x:(r4)+       y:(r0)+,y1  ; F91CBA
1460EE: move     x:(r1)+,a                                   ; 56D900
1460EF: mac      y1,y0,a         b,x:(r4)+       y:(r0)+,y1  ; FD1CB2
1460F0: move     x:(r1)+,b                                   ; 57D900
1460F1: mac      y1,y0,b         a,x:(r4)+       y:(r0)+,y1  ; F91CBA
1460F2: move     b,x:(r4)+                                   ; 575C00
1460F3: move     #>$80,r5                                    ; 65F400 000080
1460F5: move     #>$144ac7,r2                                ; 62F400 144AC7
1460F7: move     y:(r6+$a),b                                 ; 022EBF
1460F8: asr      #$10,b,b                                    ; 0C1CA1
1460F9: move     y:(r6+$2b),a                                ; 02AEFE
1460FA: move     b,n2                                        ; 21FA00
1460FB: move     #>$80,r4                                    ; 64F400 000080
1460FD: move     x:(r5)+,x0                                  ; 44DD00
1460FE: move     y:(r2+n2),y0                                ; 4EEA00
1460FF: do       #<$20,>$146103                              ; 062080 146102
146101: mac      y0,x0,a         a,x:(r4)+       a,y1        ; 191CD2
146102: mac      -y1,y0,a        x:(r5)+,x0                  ; 44DDB6
146103: move     a,y:(r6+$2b)                                ; 02AEEE
146104: move     l:?:>$5,a                                   ; 48F000 000005
146106: move     y:(r6+$1e),b                                ; 027EBF
146107: move     y:(r6+$1f),b0                               ; 027EF9
146108: move     a1,y:(r6+$1e)                               ; 027EAC
146109: move     a0,y:(r6+$1f)                               ; 027EE8
14610A: sub      b,a                                         ; 200014
14610B: asr      #$5,a,a                                     ; 0C1C0A
14610C: move     #>$20,r1                                    ; 61F400 000020
14610E: move     #>$80,r0                                    ; 60F400 000080
146110: move     a1,y1                                       ; 218700
146111: move     a0,y0                                       ; 210600
146112: tfr      b,a                                         ; 200001
146113: move     y:(r6+$1c),b                                ; 0276BF
146114: move     y:(r6+$1d),b0                               ; 0276F9
146115: do       #<$20,>$146124                              ; 062080 146123
146117: add      y,a                                         ; 200030
146118: add      a,b             b,l:(r1)+                   ; 495918
146119: move     a1,x1                                       ; 218500
14611A: move     a0,x0                                       ; 210400
14611B: move     x:(r0)+,a                                   ; 56D800
14611C: asr      #$c,a,a                                     ; 0C1C18
14611D: add      a,b                                         ; 200018
14611E: and      #>$1fff,b                                   ; 0140CE 001FFF
146120: add      #>$14a000,b                                 ; 0140C8 14A000
146122: move     x1,a                                        ; 20AE00
146123: move     x0,a0                                       ; 208800
146124: move     #>$20,r1                                    ; 61F400 000020
146126: move     #>$e0,r5                                    ; 65F400 0000E0
146128: move     b1,y:(r6+$1c)                               ; 0276AD
146129: move     r1,r4                                       ; 223400
14612A: move     x:(r1)+,r2                                  ; 62D900
14612B: move     b0,y:(r6+$1d)                               ; 0276E9
14612C: move     y:(r4)+,y0                                  ; 4EDC00
14612D: move     x:(r1)+,r0                                  ; 60D900
14612E: move     y:(r2)+,x1                                  ; 4DDA00
14612F: mpysu    -x1,y0,a                                    ; 012796
146130: add      x1,a            y:(r2),y1                   ; 4FE260
146131: add      x1,a            x:(r1)+,r2                  ; 62D960
146132: macsu    y1,y0,a                                     ; 012683
146133: move     x:(r0)+,x1      y:(r4)+,y0                  ; F49800
146134: asr      #$3,a,a                                     ; 0C1C06
146135: mpysu    -x1,y0,b                                    ; 0127B6
146136: add      x1,b            a,x:(r5)+       y:(r0),y1   ; C91D68
146137: add      x1,b            x:(r1)+,r0                  ; 60D968
146138: macsu    y1,y0,b                                     ; 0126A3
146139: move     x:(r2)+,x1      y:(r4)+,y0                  ; F49A00
14613A: asr      #$3,b,b                                     ; 0C1C87
14613B: do       #<$f,>$146149                               ; 060F80 146148
14613D: mpysu    -x1,y0,a                                    ; 012796
14613E: add      x1,a            b,x:(r5)+       y:(r2),y1   ; CD5D60
14613F: add      x1,a            x:(r1)+,r2                  ; 62D960
146140: macsu    y1,y0,a                                     ; 012683
146141: move     x:(r0)+,x1      y:(r4)+,y0                  ; F49800
146142: asr      #$3,a,a                                     ; 0C1C06
146143: mpysu    -x1,y0,b                                    ; 0127B6
146144: add      x1,b            a,x:(r5)+       y:(r0),y1   ; C91D68
146145: add      x1,b            x:(r1)+,r0                  ; 60D968
146146: macsu    y1,y0,b                                     ; 0126A3
146147: move     x:(r2)+,x1      y:(r4)+,y0                  ; F49A00
146148: asr      #$3,b,b                                     ; 0C1C87
146149: move     b,x:(r5)+                                   ; 575D00
14614A: move     #>$de,r0                                    ; 60F400 0000DE
14614C: move     #>$e0,r1                                    ; 61F400 0000E0
14614E: move     y:(r6+$23),x0                               ; 028EF4
14614F: move     x0,x:(r0)+                                  ; 445800
146150: move     y:(r6+$24),x0                               ; 0296B4
146151: move     x0,x:(r0)-                                  ; 445000
146152: move     #>$cfce3,x1                                 ; 45F400 0CFCE3
146154: move     #>$2bc9ca,y0                                ; 46F400 2BC9CA
146156: move     y:(r6+$27),y1                               ; 029EF7
146157: move     y:(r6+$28),b                                ; 02A6BF
146158: move     #>$1e,r4                                    ; 64F400 00001E
14615A: move     x:(r1)+,x0                                  ; 44D900
14615B: move     x:(r0)+,a                                   ; 56D800
14615C: do       #<$10,>$146162                              ; 061080 146161
14615E: mac      y0,x0,a         x:(r1)+,x0      y1,y:(r4)+  ; B199D2
14615F: mac      -y1,y0,a        x:(r0)+,b       b,y1        ; 1F98B6
146160: mac      x1,x0,b         x:(r1)+,x0      y1,y:(r4)+  ; B199AA
146161: mac      -y1,x1,b        x:(r0)+,a       a,y1        ; 1998FE
146162: move     y1,y:(r4)+                                  ; 4F5C00
146163: move     b,y:(r4)+                                   ; 5F5C00
146164: move     y1,y:(r6+$27)                               ; 029EE7
146165: move     b,y:(r6+$28)                                ; 02A6AF
146166: move     a,y:(r6+$23)                                ; 028EEE
146167: move     x:(r0)+,x0                                  ; 44D800
146168: move     x0,y:(r6+$24)                               ; 0296A4
146169: move     #>$1e,r4                                    ; 64F400 00001E
14616B: move     #>$20,r5                                    ; 65F400 000020
14616D: move     y:(r6+$25),x0                               ; 0296F4
14616E: move     x0,y:(r4)+                                  ; 4C5C00
14616F: move     y:(r6+$26),x0                               ; 029EB4
146170: move     x0,y:(r4)-                                  ; 4C5400
146171: move     #>$4e63df,y1                                ; 47F400 4E63DF
146173: move     #>$6f0f12,x0                                ; 44F400 6F0F12
146175: move     y:(r6+$29),x1                               ; 02A6F5
146176: move     y:(r6+$2a),b                                ; 02AEBF
146177: move     #>$1e,r0                                    ; 60F400 00001E
146179: move     y:(r5)+,y0                                  ; 4EDD00
14617A: move     y:(r4)+,a                                   ; 5EDC00
14617B: do       #<$10,>$146181                              ; 061080 146180
14617D: mac      y0,x0,a         x1,x:(r0)+      y:(r5)+,y0  ; F438D2
14617E: mac      -x1,x0,a        b,x1            y:(r4)+,b   ; 1FDCA6
14617F: mac      y1,y0,b         x1,x:(r0)+      y:(r5)+,y0  ; F438BA
146180: mac      -y1,x1,b        a,x1            y:(r4)+,a   ; 16DCFE
146181: move     x1,x:(r0)+                                  ; 455800
146182: move     b,x:(r0)+                                   ; 575800
146183: move     x1,y:(r6+$29)                               ; 02A6E5
146184: move     b,y:(r6+$2a)                                ; 02AEAF
146185: move     a,y:(r6+$25)                                ; 0296EE
146186: move     y:(r4)+,x0                                  ; 4CDC00
146187: move     x0,y:(r6+$26)                               ; 029EA4
146188: move     #>$20,r0                                    ; 60F400 000020
14618A: move     #$40,y0                                     ; 264000
14618B: move     x:(r0)+,x0                                  ; 44D800
14618C: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
14618D: mac      y0,x0,a         x:(r0)+,x0                  ; 44D8D2
14618E: asr      a                                           ; 200022
14618F: mpy      y0,x0,b         x:(r0)+,x0                  ; 44D8D8
146190: mac      y0,x0,b         x:(r0)+,x0      a,y:(r7)+   ; B2F8DA
146191: asr      b               a,y:(r7)+                   ; 5E5F2A
146192: do       #<$7,>$14619a                               ; 060780 146199
146194: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
146195: mac      y0,x0,a         x:(r0)+,x0      b,y:(r7)+   ; B3F8D2
146196: asr      a               b,y:(r7)+                   ; 5F5F22
146197: mpy      y0,x0,b         x:(r0)+,x0                  ; 44D8D8
146198: mac      y0,x0,b         x:(r0)+,x0      a,y:(r7)+   ; B2F8DA
146199: asr      b               a,y:(r7)+                   ; 5E5F2A
14619A: move     b,y:(r7)+                                   ; 5F5F00
14619B: move     b,y:(r7)+                                   ; 5F5F00
14619C: rts                                                  ; 00000C
14619D: lua      (r6+$11),r0                                 ; 040E10
14619E: move     #>$14a000,x0                                ; 44F400 14A000
1461A0: move     #$0,x1                                      ; 250000
1461A1: do       #<$1a,>$1461a4                              ; 061A80 1461A3
1461A3: move     x1,y:(r0)+                                  ; 4D5800
1461A4: move     x0,y:(r6+$10)                               ; 0246A4
1461A5: move     x0,y:(r6+$14)                               ; 0256A4
1461A6: move     x0,y:(r6+$18)                               ; 0266A4
1461A7: rts                                                  ; 00000C
1461A8: move     #>$7fffff,a                                 ; 56F400 7FFFFF
1461AA: move     a,y:(r6+$2d)                                ; 02B6EE
1461AB: move     y:(r6+$7),a                                 ; 021EFE
1461AC: sub      #>$400000,a                                 ; 0140C4 400000
1461AE: move     y:(r6+$6),x0                                ; 021EB4
1461AF: move     a,x1                                        ; 21C500
1461B0: mpy      x1,x0,b                                     ; 2000A8
1461B1: tst      a                                           ; 200003
1461B2: move     b,x0                                        ; 21E400
1461B3: tfr      x0,a            ifmi                        ; 202B41
1461B4: asl      a                                           ; 200032
1461B5: asl      a                                           ; 200032
1461B6: move     y:(r6+$5),b                                 ; 0216FF
1461B7: move     a,y:(r6+$2c)                                ; 02B6AE
1461B8: sub      #>$400000,b                                 ; 0140CC 400000
1461BA: asr      b               ifmi                        ; 202B2A
1461BB: move     b,y0                                        ; 21E600
1461BC: abs      b                                           ; 20002E
1461BD: move     b,y1                                        ; 21E700
1461BE: mpy      y1,y0,b                                     ; 2000B8
1461BF: move     b,y:(r6+$2e)                                ; 02BEAF
1461C0: rts                                                  ; 00000C
1461C1: move     #>$be37c,x0                                 ; 44F400 0BE37C
1461C3: move     a1,y1                                       ; 218700
1461C4: move     a0,y0                                       ; 210600
1461C5: mpysu    x0,y0,a                                     ; 01278D
1461C6: dmac     ss x0,y1,a                                  ; 012484
1461C7: nop                                                  ; 000000
1461C8: move     a,l:?:>$5                                   ; 487000 000005
1461CA: move     #>$1fff,m2                                  ; 05F422 001FFF
1461CC: move     #>$1fff,m0                                  ; 05F420 001FFF
1461CE: move     #>$141880,r1                                ; 61F400 141880
1461D0: move     y:(r6+$5),b                                 ; 0216FF
1461D1: move     #>$80,a                                     ; 56F400 000080
1461D3: sub      #>$400000,b                                 ; 0140CC 400000
1461D5: asr      #$f,b,b                                     ; 0C1C9F
1461D6: abs      b                                           ; 20002E
1461D7: sub      b,a                                         ; 200014
1461D8: move     a,n1                                        ; 21D900
1461D9: move     y:(r6+$2e),x0                               ; 02BEB4
1461DA: move     y:(r1+n1),x1                                ; 4DE900
1461DB: mpy      -x1,x0,b                                    ; 2000AC
1461DC: move     b,y:(r6+$2e)                                ; 02BEAF
1461DD: asr      #$c,b,b                                     ; 0C1C99
1461DE: move     l:?:>$5,a                                   ; 48F000 000005
1461E0: add      b,a                                         ; 200010
1461E1: move     #>$7fffff,x0                                ; 44F400 7FFFFF
1461E3: move     y:(r6+$4),b                                 ; 0216BF
1461E4: cmp      #>$7effff,b                                 ; 0140CD 7EFFFF
1461E6: tfr      x0,b            ifgt                        ; 202749
1461E7: move     b,x0                                        ; 21E400
1461E8: move     a1,y1                                       ; 218700
1461E9: move     a0,y0                                       ; 210600
1461EA: mpysu    x0,y0,a                                     ; 01278D
1461EB: dmac     ss x0,y1,a                                  ; 012484
1461EC: asl      #$1,a,a                                     ; 0C1D02
1461ED: move     y:(r6+$12),b                                ; 024EBF
1461EE: move     y:(r6+$13),b0                               ; 024EF9
1461EF: move     a1,y:(r6+$12)                               ; 024EAC
1461F0: move     a0,y:(r6+$13)                               ; 024EE8
1461F1: sub      b,a                                         ; 200014
1461F2: asr      #$5,a,a                                     ; 0C1C0A
1461F3: move     #>$20,r1                                    ; 61F400 000020
1461F5: move     a1,y1                                       ; 218700
1461F6: move     a0,y0                                       ; 210600
1461F7: tfr      b,a                                         ; 200001
1461F8: move     y:(r6+$10),b                                ; 0246BF
1461F9: move     y:(r6+$11),b0                               ; 0246F9
1461FA: move     #>$1fff,x0                                  ; 44F400 001FFF
1461FC: move     #>$14a000,x1                                ; 45F400 14A000
1461FE: do       #<$20,>$146204                              ; 062080 146203
146200: add      y,a                                         ; 200030
146201: add      a,b             b,l:(r1)+                   ; 495918
146202: and      x0,b                                        ; 20004E
146203: add      x1,b                                        ; 200068
146204: nop                                                  ; 000000
146205: move     b1,y:(r6+$10)                               ; 0246AD
146206: move     b0,y:(r6+$11)                               ; 0246E9
146207: move     #>$20,r1                                    ; 61F400 000020
146209: move     #>$80,r5                                    ; 65F400 000080
14620B: nop                                                  ; 000000
14620C: move     r1,r4                                       ; 223400
14620D: nop                                                  ; 000000
14620E: nop                                                  ; 000000
14620F: move     x:(r1)+,r2                                  ; 62D900
146210: move     y:(r4)+,y0                                  ; 4EDC00
146211: nop                                                  ; 000000
146212: nop                                                  ; 000000
146213: move     x:(r2)+,x1                                  ; 45DA00
146214: move     x:(r1)+,r0                                  ; 60D900
146215: do       #<$10,>$146223                              ; 061080 146222
146217: mpysu    -x1,y0,a                                    ; 012796
146218: add      x1,a            x:(r2),x0                   ; 44E260
146219: add      x1,a            x:(r1)+,r2                  ; 62D960
14621A: macsu    x0,y0,a                                     ; 01268D
14621B: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
14621C: mpysu    -x1,y0,b                                    ; 0127B6
14621D: add      x1,b            x:(r0),x0                   ; 44E068
14621E: add      x1,b            x:(r1)+,r0                  ; 60D968
14621F: macsu    x0,y0,b                                     ; 0126AD
146220: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
146221: move     a,y:(r5)+                                   ; 5E5D00
146222: move     b,y:(r5)+                                   ; 5F5D00
146223: move     #>$80,r4                                    ; 64F400 000080
146225: move     #>$80,r5                                    ; 65F400 000080
146227: move     y:(r6+$1c),x0                               ; 0276B4
146228: do       #<$20,>$14622d                              ; 062080 14622C
14622A: move     y:(r5)+,b                                   ; 5FDD00
14622B: sub      x0,b            b,x0                        ; 21E44C
14622C: move     b,y:(r4)+                                   ; 5F5C00
14622D: move     x0,y:(r6+$1c)                               ; 0276A4
14622E: move     y:(r6+$2a),a                                ; 02AEBE
14622F: move     #>$80,r5                                    ; 65F400 000080
146231: move     #>$80,r4                                    ; 64F400 000080
146233: move     #$40,y0                                     ; 264000
146234: move     y:(r5)+,x0                                  ; 4CDD00
146235: do       #<$20,>$146239                              ; 062080 146238
146237: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
146238: mac      -x1,y0,a        y:(r5)+,x0                  ; 4CDDE6
146239: move     a,y:(r6+$2a)                                ; 02AEAE
14623A: move     #>$141880,r1                                ; 61F400 141880
14623C: move     y:(r6+$7),b                                 ; 021EFF
14623D: move     #>$80,a                                     ; 56F400 000080
14623F: sub      #>$400000,b                                 ; 0140CC 400000
146241: asr      #$f,b,b                                     ; 0C1C9F
146242: abs      b                                           ; 20002E
146243: sub      b,a                                         ; 200014
146244: move     a,n1                                        ; 21D900
146245: move     y:(r6+$2c),x0                               ; 02B6B4
146246: move     y:(r1+n1),x1                                ; 4DE900
146247: mpy      -x1,x0,a                                    ; 2000A4
146248: move     a,y:(r6+$2c)                                ; 02B6AE
146249: move     y:(r6+$6),b                                 ; 021EBF
14624A: add      a,b                                         ; 200018
14624B: clr      a                                           ; 200013
14624C: sub      #>$400000,b                                 ; 0140CC 400000
14624E: tst      b                                           ; 20000B
14624F: tfr      b,a             ifpl                        ; 202301
146250: move     a,y1                                        ; 21C700
146251: move     #>$80,r0                                    ; 60F400 000080
146253: move     y:(r6+$20),x0                               ; 0286B4
146254: do       #<$20,>$14625a                              ; 062080 146259
146256: mpy      x0,y1,a         y:(r0),x1                   ; 4DE0C0
146257: asl      #$4,a,a                                     ; 0C1D08
146258: add      x1,a            x1,x0                       ; 20A460
146259: move     a,y:(r0)+                                   ; 5E5800
14625A: move     x0,y:(r6+$20)                               ; 0286A4
14625B: move     #>$7fffff,x0                                ; 44F400 7FFFFF
14625D: move     y:(r6+$8),b                                 ; 0226BF
14625E: cmp      #>$7effff,b                                 ; 0140CD 7EFFFF
146260: tfr      x0,b            ifgt                        ; 202749
146261: move     b,x0                                        ; 21E400
146262: move     l:?:>$5,a                                   ; 48F000 000005
146264: mpy      x0,x0,b                                     ; 200088
146265: move     a1,y1                                       ; 218700
146266: move     a0,y0                                       ; 210600
146267: move     b,x0                                        ; 21E400
146268: mpysu    x0,y0,a                                     ; 01278D
146269: dmac     ss x0,y1,a                                  ; 012484
14626A: asl      #$2,a,a                                     ; 0C1D04
14626B: nop                                                  ; 000000
14626C: move     a1,y1                                       ; 218700
14626D: move     a0,y0                                       ; 210600
14626E: move     y:(r6+$14),b                                ; 0256BF
14626F: move     y:(r6+$15),b0                               ; 0256F9
146270: move     y:(r6+$a),a                                 ; 022EBE
146271: asr      #$b,a,a                                     ; 0C1C16
146272: move     #>$c0,r5                                    ; 65F400 0000C0
146274: move     a,x0                                        ; 21C400
146275: move     y:(r6+$1e),a                                ; 027EBE
146276: move     y:(r6+$1f),a0                               ; 027EF8
146277: do       #<$20,>$146283                              ; 062080 146282
146279: add      y,b                                         ; 200038
14627A: add      a,b                                         ; 200018
14627B: and      #>$1fff,b                                   ; 0140CE 001FFF
14627D: add      #>$14a000,b                                 ; 0140C8 14A000
14627F: move     b1,r3                                       ; 21B300
146280: sub      a,b                                         ; 20001C
146281: move     x:(r3),x1                                   ; 45E300
146282: mpy      x1,x0,a         x1,y:(r5)+                  ; 4D5DA0
146283: move     b1,y:(r6+$14)                               ; 0256AD
146284: move     b0,y:(r6+$15)                               ; 0256E9
146285: move     a1,y:(r6+$1e)                               ; 027EAC
146286: move     a0,y:(r6+$1f)                               ; 027EE8
146287: move     #>$c0,r5                                    ; 65F400 0000C0
146289: move     #>$c0,r4                                    ; 64F400 0000C0
14628B: move     y:(r6+$1d),x0                               ; 0276F4
14628C: do       #<$20,>$146291                              ; 062080 146290
14628E: move     y:(r5)+,b                                   ; 5FDD00
14628F: sub      x0,b            b,x0                        ; 21E44C
146290: move     b,y:(r4)+                                   ; 5F5C00
146291: move     x0,y:(r6+$1d)                               ; 0276E4
146292: move     y:(r6+$2b),a                                ; 02AEFE
146293: move     #>$c0,r5                                    ; 65F400 0000C0
146295: move     #>$c0,r4                                    ; 64F400 0000C0
146297: move     #$40,y0                                     ; 264000
146298: move     y:(r5)+,x0                                  ; 4CDD00
146299: do       #<$20,>$14629d                              ; 062080 14629C
14629B: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
14629C: mac      -x1,y0,a        y:(r5)+,x0                  ; 4CDDE6
14629D: move     a,y:(r6+$2b)                                ; 02AEEE
14629E: move     #>$80,r0                                    ; 60F400 000080
1462A0: move     #>$c0,r1                                    ; 61F400 0000C0
1462A2: move     #>$80,r2                                    ; 62F400 000080
1462A4: move     y:(r6+$6),x0                                ; 021EB4
1462A5: mpy      x0,x0,a                                     ; 200080
1462A6: move     y:(r6+$2c),b                                ; 02B6BF
1462A7: add      b,a                                         ; 200010
1462A8: asl      #$2,a,a                                     ; 0C1D04
1462A9: move     a,y0                                        ; 21C600
1462AA: move     y:(r6+$9),b                                 ; 0226FF
1462AB: move     y:(r6+$2d),y1                               ; 02B6F7
1462AC: cmp      #>$400000,b                                 ; 0140CD 400000
1462AE: blt      func_1462bd                                 ; 0D1049 00000F
1462B0: sub      #>$400000,b                                 ; 0140CC 400000
1462B2: move     #>$7fffff,a                                 ; 56F400 7FFFFF
1462B4: move     b,x0                                        ; 21E400
1462B5: mpy      x0,x0,b                                     ; 200088
1462B6: move     b,x0                                        ; 21E400
1462B7: mac      -x0,x0,a                                    ; 200086
1462B8: move     a,x0                                        ; 21C400
1462B9: mpy      x0,y1,a                                     ; 2000C0
1462BA: nop                                                  ; 000000
1462BB: move     a,y1                                        ; 21C700
1462BC: move     y1,y:(r6+$2d)                               ; 02B6E7
1462BD: move     y:(r6+$9),x0                                ; 0226F4
1462BE: mpy      x0,x0,a                                     ; 200080
1462BF: asl      #$2,a,a                                     ; 0C1D04
1462C0: move     a,x0                                        ; 21C400
1462C1: mpy      x0,y1,a                                     ; 2000C0
1462C2: move     a,y1                                        ; 21C700
1462C3: move     y:(r0)+,x0                                  ; 4CD800
1462C4: move     y:(r1)+,x1                                  ; 4DD900
1462C5: mpy      y0,x0,a                                     ; 2000D0
1462C6: mpy      y1,x1,b         y:(r0)+,x0                  ; 4CD8F8
1462C7: add      a,b             y:(r1)+,x1                  ; 4DD918
1462C8: do       #<$1f,>$1462cd                              ; 061F80 1462CC
1462CA: mpy      y0,x0,a         b,x:(r2)+                   ; 575AD0
1462CB: mpy      y1,x1,b         y:(r0)+,x0                  ; 4CD8F8
1462CC: add      a,b             y:(r1)+,x1                  ; 4DD918
1462CD: move     b,x:(r2)+                                   ; 575A00
1462CE: move     y:(r6+$29),a                                ; 02A6FE
1462CF: move     #>$80,r5                                    ; 65F400 000080
1462D1: move     #>$80,r4                                    ; 64F400 000080
1462D3: move     #$40,y0                                     ; 264000
1462D4: move     x:(r5)+,x0                                  ; 44DD00
1462D5: do       #<$20,>$1462d9                              ; 062080 1462D8
1462D7: mac      y0,x0,a         a,x:(r4)+       a,y1        ; 191CD2
1462D8: mac      -y1,y0,a        x:(r5)+,x0                  ; 44DDB6
1462D9: move     a,y:(r6+$29)                                ; 02A6EE
1462DA: move     l:?:>$5,a                                   ; 48F000 000005
1462DC: move     y:(r6+$1a),b                                ; 026EBF
1462DD: move     y:(r6+$1b),b0                               ; 026EF9
1462DE: move     a1,y:(r6+$1a)                               ; 026EAC
1462DF: move     a0,y:(r6+$1b)                               ; 026EE8
1462E0: sub      b,a                                         ; 200014
1462E1: asr      #$5,a,a                                     ; 0C1C0A
1462E2: move     #>$20,r1                                    ; 61F400 000020
1462E4: move     #>$80,r0                                    ; 60F400 000080
1462E6: move     a1,y1                                       ; 218700
1462E7: move     a0,y0                                       ; 210600
1462E8: tfr      b,a                                         ; 200001
1462E9: move     y:(r6+$18),b                                ; 0266BF
1462EA: move     y:(r6+$19),b0                               ; 0266F9
1462EB: do       #<$20,>$1462fa                              ; 062080 1462F9
1462ED: add      y,a                                         ; 200030
1462EE: add      a,b             b,l:(r1)+                   ; 495918
1462EF: move     a1,x1                                       ; 218500
1462F0: move     a0,x0                                       ; 210400
1462F1: move     x:(r0)+,a                                   ; 56D800
1462F2: asr      #$c,a,a                                     ; 0C1C18
1462F3: add      a,b                                         ; 200018
1462F4: and      #>$1fff,b                                   ; 0140CE 001FFF
1462F6: add      #>$14a000,b                                 ; 0140C8 14A000
1462F8: move     x1,a                                        ; 20AE00
1462F9: move     x0,a0                                       ; 208800
1462FA: move     b1,y:(r6+$18)                               ; 0266AD
1462FB: move     b0,y:(r6+$19)                               ; 0266E9
1462FC: move     #>$20,r1                                    ; 61F400 000020
1462FE: move     #>$e0,r3                                    ; 63F400 0000E0
146300: move     r1,r4                                       ; 223400
146301: move     x:(r1)+,r2                                  ; 62D900
146302: move     y:(r4)+,y0                                  ; 4EDC00
146303: move     x:(r2)+,x1                                  ; 45DA00
146304: move     x:(r1)+,r0                                  ; 60D900
146305: do       #<$10,>$146317                              ; 061080 146316
146307: mpysu    -x1,y0,a                                    ; 012796
146308: add      x1,a            x:(r2),x0                   ; 44E260
146309: add      x1,a            x:(r1)+,r2                  ; 62D960
14630A: macsu    x0,y0,a                                     ; 01268D
14630B: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
14630C: asr      a                                           ; 200022
14630D: asr      a                                           ; 200022
14630E: mpysu    -x1,y0,b                                    ; 0127B6
14630F: add      x1,b            x:(r0),x0                   ; 44E068
146310: add      x1,b            x:(r1)+,r0                  ; 60D968
146311: macsu    x0,y0,b                                     ; 0126AD
146312: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
146313: asr      b                                           ; 20002A
146314: asr      b                                           ; 20002A
146315: move     a,x:(r3)+                                   ; 565B00
146316: move     b,x:(r3)+                                   ; 575B00
146317: move     #>$de,r0                                    ; 60F400 0000DE
146319: move     #>$e0,r1                                    ; 61F400 0000E0
14631B: move     y:(r6+$21),x0                               ; 0286F4
14631C: move     x0,x:(r0)+                                  ; 445800
14631D: move     y:(r6+$22),x0                               ; 028EB4
14631E: move     x0,x:(r0)-                                  ; 445000
14631F: move     #>$cfce3,x1                                 ; 45F400 0CFCE3
146321: move     #>$2bc9ca,y0                                ; 46F400 2BC9CA
146323: move     y:(r6+$25),y1                               ; 0296F7
146324: move     y:(r6+$26),b                                ; 029EBF
146325: move     #>$1e,r4                                    ; 64F400 00001E
146327: move     x:(r1)+,x0                                  ; 44D900
146328: move     x:(r0)+,a                                   ; 56D800
146329: do       #<$10,>$14632f                              ; 061080 14632E
14632B: mac      y0,x0,a         x:(r1)+,x0      y1,y:(r4)+  ; B199D2
14632C: mac      -y1,y0,a        x:(r0)+,b       b,y1        ; 1F98B6
14632D: mac      x1,x0,b         x:(r1)+,x0      y1,y:(r4)+  ; B199AA
14632E: mac      -y1,x1,b        x:(r0)+,a       a,y1        ; 1998FE
14632F: move     y1,y:(r4)+                                  ; 4F5C00
146330: move     b,y:(r4)+                                   ; 5F5C00
146331: move     y1,y:(r6+$25)                               ; 0296E7
146332: move     b,y:(r6+$26)                                ; 029EAF
146333: move     a,y:(r6+$21)                                ; 0286EE
146334: move     x:(r0)+,x0                                  ; 44D800
146335: move     x0,y:(r6+$22)                               ; 028EA4
146336: move     #>$1e,r4                                    ; 64F400 00001E
146338: move     #>$20,r5                                    ; 65F400 000020
14633A: move     y:(r6+$23),x0                               ; 028EF4
14633B: move     x0,y:(r4)+                                  ; 4C5C00
14633C: move     y:(r6+$24),x0                               ; 0296B4
14633D: move     x0,y:(r4)-                                  ; 4C5400
14633E: move     #>$4e63df,y1                                ; 47F400 4E63DF
146340: move     #>$6f0f12,x0                                ; 44F400 6F0F12
146342: move     y:(r6+$27),x1                               ; 029EF5
146343: move     y:(r6+$28),b                                ; 02A6BF
146344: move     #>$1e,r0                                    ; 60F400 00001E
146346: move     y:(r5)+,y0                                  ; 4EDD00
146347: move     y:(r4)+,a                                   ; 5EDC00
146348: do       #<$10,>$14634e                              ; 061080 14634D
14634A: mac      y0,x0,a         x1,x:(r0)+      y:(r5)+,y0  ; F438D2
14634B: mac      -x1,x0,a        b,x1            y:(r4)+,b   ; 1FDCA6
14634C: mac      y1,y0,b         x1,x:(r0)+      y:(r5)+,y0  ; F438BA
14634D: mac      -y1,x1,b        a,x1            y:(r4)+,a   ; 16DCFE
14634E: move     x1,x:(r0)+                                  ; 455800
14634F: move     b,x:(r0)+                                   ; 575800
146350: move     x1,y:(r6+$27)                               ; 029EE5
146351: move     b,y:(r6+$28)                                ; 02A6AF
146352: move     a,y:(r6+$23)                                ; 028EEE
146353: move     y:(r4)+,x0                                  ; 4CDC00
146354: move     x0,y:(r6+$24)                               ; 0296A4
146355: move     #>$20,r0                                    ; 60F400 000020
146357: move     #>$1f,r4                                    ; 64F400 00001F
146359: move     #$40,y0                                     ; 264000
14635A: move     x:(r0)+,x0                                  ; 44D800
14635B: do       #<$8,>$146361                               ; 060880 146360
14635D: mpy      y0,x0,a         x:(r0)+,x0                  ; 44D8D0
14635E: mac      y0,x0,a         x:(r0)+,x0      b,y:(r4)+   ; B398D2
14635F: mpy      y0,x0,b         x:(r0)+,x0                  ; 44D8D8
146360: mac      y0,x0,b         x:(r0)+,x0      a,y:(r4)+   ; B298DA
146361: move     b,y:(r4)+                                   ; 5F5C00
146362: move     #>$20,r4                                    ; 64F400 000020
146364: do       #<$8,>$14636e                               ; 060880 14636D
146366: move     y:(r4)+,a                                   ; 5EDC00
146367: move     y:(r4)+,b                                   ; 5FDC00
146368: asr      a                                           ; 200022
146369: asr      b                                           ; 20002A
14636A: move     a,y:(r7)+                                   ; 5E5F00
14636B: move     a,y:(r7)+                                   ; 5E5F00
14636C: move     b,y:(r7)+                                   ; 5F5F00
14636D: move     b,y:(r7)+                                   ; 5F5F00
14636E: rts                                                  ; 00000C
14636F: move     #$0,y0                                      ; 260000
146370: move     y0,y:(r6+$19)                               ; 0266E6
146371: move     y0,y:(r6+$1b)                               ; 026EE6
146372: move     y0,y:(r6+$1c)                               ; 0276A6
146373: move     y0,y:(r6+$1d)                               ; 0276E6
146374: move     y0,y:(r6+$1e)                               ; 027EA6
146375: move     y0,y:(r6+$1f)                               ; 027EE6
146376: move     #>$19b5,y0                                  ; 46F400 0019B5
146378: move     y0,y:(r6+$1a)                               ; 026EA6
146379: rts                                                  ; 00000C
14637A: move     y:(r6+$6),a                                 ; 021EBE
14637B: move     y:(r6+$19),b                                ; 0266FF
14637C: cmp      #>$400000,a                                 ; 0140C5 400000
14637E: clr      b               ifge                        ; 20211B
14637F: nop                                                  ; 000000
146380: move     b,y:(r6+$19)                                ; 0266EF
146381: rts                                                  ; 00000C
146382: asr      #$f,a,a                                     ; 0C1C1E
146383: nop                                                  ; 000000
146384: move     a0,x0                                       ; 210400
146385: move     #>$fd9fb3,y0                                ; 46F400 FD9FB3
146387: macuu    x0,y0,a                                     ; 0126CD
146388: lsr      #$5,a                                       ; 0C1ECA
146389: move     a,x0                                        ; 21C400
14638A: asl      #$4,a,a                                     ; 0C1D08
14638B: move     y:(r6+$7),b                                 ; 021EFF
14638C: cmp      #>$666666,b                                 ; 0140CD 666666
14638E: tfr      x0,a            iflt                        ; 202941
14638F: move     a,y:(r6+$16)                                ; 025EAE
146390: move     a,x0                                        ; 21C400
146391: move     y:(r6+$a),b                                 ; 022EBF
146392: asr      #$6,b,b                                     ; 0C1C8D
146393: add      #>$400,b                                    ; 0140C8 000400
146395: nop                                                  ; 000000
146396: move     b,y0                                        ; 21E600
146397: move     #>$0,r0                                     ; 60F400 000000
146399: move     #>$0,r4                                     ; 64F400 000000
14639B: move     #$bf,m0                                     ; 05BFA0
14639C: move     #$bf,m4                                     ; 05BFA4
14639D: move     y:(r6+$9),a                                 ; 0226FE
14639E: cmp      #>$400000,a                                 ; 0140C5 400000
1463A0: blt      func_1463a9                                 ; 0D1049 000009
1463A2: move     x:(r6-$1),a                                 ; 03FEDE
1463A3: tst      a                                           ; 200003
1463A4: bne      func_1463af                                 ; 0D1042 00000B
1463A6: move     y:(r6-$1),a                                 ; 03FEFE
1463A7: bra      func_1463af                                 ; 0D10C0 000008
1463A9: move     y:(r6+$a),x0                                ; 022EB4
1463AA: move     #>$2000,x1                                  ; 45F400 002000
1463AC: mpy      x1,x0,a                                     ; 2000A0
1463AD: add      #>$800,a                                    ; 0140C0 000800
1463AF: asr      #$b,a,b                                     ; 0C1C17
1463B0: and      #>$7ff,a                                    ; 0140C6 0007FF
1463B2: move     a,r2                                        ; 21D200
1463B3: move     b,x0                                        ; 21E400
1463B4: move     x:(r2+$140000),a                            ; 0A72CE 140000
1463B6: asr      #$a,a,a                                     ; 0C1C14
1463B7: asl      x0,a,a                                      ; 0C1E48
1463B8: move     #>$20b40,y0                                 ; 46F400 020B40
1463BA: move     a,x0                                        ; 21C400
1463BB: mpy      y0,x0,a                                     ; 2000D0
1463BC: asr      #$12,a,a                                    ; 0C1C24
1463BD: move     a0,x0                                       ; 210400
1463BE: move     #>$fd9fb3,y0                                ; 46F400 FD9FB3
1463C0: mpyuu    x0,y0,a                                     ; 0127CD
1463C1: lsr      #$2,a                                       ; 0C1EC4
1463C2: move     a,y0                                        ; 21C600
1463C3: move     y:(r6+$16),x0                               ; 025EB4
1463C4: move     y:(r6+$8),a                                 ; 0226BE
1463C5: cmp      #>$400000,a                                 ; 0140C5 400000
1463C7: blt      func_1463db                                 ; 0D1049 000014
1463C9: clr      a                                           ; 200013
1463CA: clr      b                                           ; 20001B
1463CB: move     y:(r6+$17),a1                               ; 025EFC
1463CC: move     y:(r6+$18),b1                               ; 0266BD
1463CD: add      x0,b                                        ; 200048
1463CE: add      y0,a                                        ; 200050
1463CF: bclr     #$0,b2                                      ; 0ACB40
1463D0: clr      a               ifcs                        ; 202813
1463D1: do       #<$bf,>$1463d7                              ; 06BF80 1463D6
1463D3: add      x0,b            b1,y:(r4)+                  ; 5D5C48
1463D4: add      y0,a            a1,x:(r0)+                  ; 545850
1463D5: bclr     #$0,b2                                      ; 0ACB40
1463D6: clr      a               ifcs                        ; 202813
1463D7: move     b1,y:(r4)+                                  ; 5D5C00
1463D8: move     a1,x:(r0)+                                  ; 545800
1463D9: bra      func_1463e3                                 ; 0D10C0 00000A
1463DB: clr      a                                           ; 200013
1463DC: clr      b                                           ; 20001B
1463DD: move     y:(r6+$17),a1                               ; 025EFC
1463DE: move     y:(r6+$18),b1                               ; 0266BD
1463DF: do       #<$c0,>$1463e3                              ; 06C080 1463E2
1463E1: add      y0,b            b1,y:(r4)+                  ; 5D5C58
1463E2: add      x0,a            a1,x:(r0)+                  ; 545840
1463E3: move     b1,y:(r6+$18)                               ; 0266AD
1463E4: move     a1,y:(r6+$17)                               ; 025EEC
1463E5: move     y:(r6+$8),a                                 ; 0226BE
1463E6: brset    #$15,a,func_146493                          ; 0CCEB5 0000AD
1463E8: move     y:(r6+$7),a                                 ; 021EFE
1463E9: cmp      #>$199999,a                                 ; 0140C5 199999
1463EB: bcs      func_14647d                                 ; 0D1048 000092
1463ED: cmp      #>$333333,a                                 ; 0140C5 333333
1463EF: bcs      func_146474                                 ; 0D1048 000085
1463F1: cmp      #>$4ccccc,a                                 ; 0140C5 4CCCCC
1463F3: bcs      func_1464a1                                 ; 0D1048 0000AE
1463F5: cmp      #>$666666,a                                 ; 0140C5 666666
1463F7: bcs      func_1464de                                 ; 0D1048 0000E7
1463F9: bra      func_1464c0                                 ; 0D10C0 0000C7
1463FB: move     #>$f0,r5                                    ; 65F400 0000F0
1463FD: move     #$9,m5                                      ; 0509A5
1463FE: move     #$2,n5                                      ; 3D0200
1463FF: move     #>$f0,r1                                    ; 61F400 0000F0
146401: move     #>$f2,r2                                    ; 62F400 0000F2
146403: move     #>$f4,r3                                    ; 63F400 0000F4
146405: move     y:(r6+$1b),a                                ; 026EFE
146406: move     y:(r6+$1c),b                                ; 0276BF
146407: move     a,x:(r1+$0)                                 ; 02018E
146408: move     b,x:(r1+$1)                                 ; 0201CF
146409: move     y:(r6+$1d),a                                ; 0276FE
14640A: move     y:(r6+$1e),b                                ; 027EBF
14640B: move     a,x:(r2+$0)                                 ; 02028E
14640C: move     y:(r6+$1f),a                                ; 027EFE
14640D: move     b,x:(r2+$1)                                 ; 0202CF
14640E: move     a,x:(r3+$0)                                 ; 02038E
14640F: move     #>$3ef3c2,y0                                ; 46F400 3EF3C2
146411: move     y0,y:(r5)+                                  ; 4E5D00
146412: move     #>$fe01eb,y0                                ; 46F400 FE01EB
146414: move     y0,y:(r5)+                                  ; 4E5D00
146415: move     #>$82f338,y0                                ; 46F400 82F338
146417: move     y0,y:(r5)+                                  ; 4E5D00
146418: move     #>$1d9bc,y0                                 ; 46F400 01D9BC
14641A: move     y0,y:(r5)+                                  ; 4E5D00
14641B: move     #>$3cd5c5,y0                                ; 46F400 3CD5C5
14641D: move     y0,y:(r5)+                                  ; 4E5D00
14641E: move     #>$650d5,y0                                 ; 46F400 0650D5
146420: move     y0,y:(r5)+                                  ; 4E5D00
146421: move     #>$8400a4,y0                                ; 46F400 8400A4
146423: move     y0,y:(r5)+                                  ; 4E5D00
146424: move     #>$f9af2b,y0                                ; 46F400 F9AF2B
146426: move     y0,y:(r5)+                                  ; 4E5D00
146427: move     #>$c22e5f,y0                                ; 46F400 C22E5F
146429: move     y0,y:(r5)+                                  ; 4E5D00
14642A: move     #>$4866c,y0                                 ; 46F400 04866C
14642C: move     y0,y:(r5)+                                  ; 4E5D00
14642D: move     #>$6186,x1                                  ; 45F400 006186
14642F: do       #<$10,>$146461                              ; 061080 146460
146431: move     x:(r1)+,x0      y:(r5)+n5,y0                ; D0B900
146432: mpy      -y0,x0,a        x:(r1)-,x0      y:(r5)+n5,y0 ; D0B1D4
146433: mac      -y0,x0,a        x0,x:(r1)+      y:(r4)+,y1  ; F119D6
146434: asl      a                                           ; 200032
146435: macr     y1,x1,a         x:(r2)+,x0      y:(r5)+n5,y0 ; D0BAF3
146436: do       #<$a,>$146444                               ; 060A80 146443
146438: mpy      -y0,x0,b        x:(r2)-,x0      y:(r5)+n5,y0 ; D0B2DC
146439: mac      -y0,x0,b        x0,x:(r2)+                  ; 445ADE
14643A: asl      b               a,x:(r1)-                   ; 56513A
14643B: macr     y1,x1,b         x:(r3),x0       y:(r5)+n5,y0 ; D0A3FB
14643C: nop                                                  ; 000000
14643D: mpy      -y0,x0,b        b,x:(r2)-                   ; 5752DC
14643E: asl      b                                           ; 20003A
14643F: macr     y1,x1,b         x:(r1)+,x0      y:(r5)+n5,y0 ; D0B9FB
146440: mpy      -y0,x0,a        x:(r1)-,x0      y:(r5)+n5,y0 ; D0B1D4
146441: mac      -y0,x0,a        x0,x:(r1)+      y:(r4)+,y1  ; F119D6
146442: asl      a               b,x:(r3)                    ; 576332
146443: macr     y1,x1,a         x:(r2)+,x0      y:(r5)+n5,y0 ; D0BAF3
146444: mpy      -y0,x0,b        x:(r2)-,x0      y:(r5)+n5,y0 ; D0B2DC
146445: mac      -y0,x0,b        x0,x:(r2)+                  ; 445ADE
146446: asl      b               a,x:(r1)-                   ; 56513A
146447: macr     y1,x1,b         x:(r3),x0       y:(r5)+n5,y0 ; D0A3FB
146448: nop                                                  ; 000000
146449: mpy      -y0,x0,b        b,x:(r2)-                   ; 5752DC
14644A: asl      b                                           ; 20003A
14644B: macr     y1,x1,b         x:(r1)+,x0      y:(r5)+,y0  ; F0B9FB
14644C: nop                                                  ; 000000
14644D: mpy      -y0,x0,a        b,x:(r3)        y:(r5)+,y0  ; FC23D4
14644E: mpy      y0,x0,b         x:(r1)-,x0      y:(r5)+,y0  ; F0B1D8
14644F: mac      -y0,x0,a        x0,x:(r1)+      y:(r5)+,y0  ; F039D6
146450: asl      a               y:(r4)+,y1                  ; 4FDC32
146451: macr     y1,x1,a                                     ; 2000F3
146452: mac      y0,x0,b         x:(r2)+,x0      y:(r5)+,y0  ; F0BADA
146453: mpy      -y0,x0,a        a,x:(r1)-       y:(r5)+,y0  ; F831D4
146454: mac      y0,x0,b         x:(r2)-,x0      y:(r5)+,y0  ; F0B2DA
146455: mac      -y0,x0,a        x0,x:(r2)+      y:(r5)+,y0  ; F03AD6
146456: asl      a                                           ; 200032
146457: macr     y1,x1,a                                     ; 2000F3
146458: mac      y0,x0,b         x:(r3),x0       y:(r5)+,y0  ; F0A3DA
146459: mpy      -y0,x0,a        a,x:(r2)-       y:(r5)+,y0  ; F832D4
14645A: asl      a                                           ; 200032
14645B: macr     y1,x1,a                                     ; 2000F3
14645C: mac      y0,x0,b                                     ; 2000DA
14645D: asl      #$6,b,b                                     ; 0C1D8D
14645E: nop                                                  ; 000000
14645F: move     a,x:(r3)        b,y:(r7)+                   ; BB6300
146460: move     b,y:(r7)+                                   ; 5F5F00
146461: move     x:(r1+$0),a                                 ; 02019E
146462: move     x:(r1+$1),b                                 ; 0201DF
146463: move     a,y:(r6+$1b)                                ; 026EEE
146464: move     b,y:(r6+$1c)                                ; 0276AF
146465: move     x:(r2+$0),a                                 ; 02029E
146466: move     x:(r2+$1),b                                 ; 0202DF
146467: move     a,y:(r6+$1d)                                ; 0276EE
146468: move     x:(r3+$0),a                                 ; 02039E
146469: move     b,y:(r6+$1e)                                ; 027EAF
14646A: move     a,y:(r6+$1f)                                ; 027EEE
14646B: rts                                                  ; 00000C
14646C: move     #$c,n0                                      ; 380C00
14646D: do       #<$10,>$146473                              ; 061080 146472
14646F: move     x:(r0)+n0,a                                 ; 56C800
146470: nop                                                  ; 000000
146471: move     a,y:(r7)+                                   ; 5E5F00
146472: move     a,y:(r7)+                                   ; 5E5F00
146473: rts                                                  ; 00000C
146474: move     x:(r0)+,a                                   ; 56D800
146475: move     x:(r0)+,b                                   ; 57D800
146476: do       #<$5f,>$14647a                              ; 065F80 146479
146478: move     x:(r0)+,a       a,y:(r4)+                   ; BA9800
146479: move     x:(r0)+,b       b,y:(r4)+                   ; BF9800
14647A: move     a,y:(r4)+                                   ; 5E5C00
14647B: move     b,y:(r4)+                                   ; 5F5C00
14647C: bra      func_1463fb                                 ; 050EDF
14647D: move     #$40,x0                                     ; 244000
14647E: move     x:(r0)+,a                                   ; 56D800
14647F: move     x:(r0)+,b                                   ; 57D800
146480: do       #<$5f,>$14648a                              ; 065F80 146489
146482: abs      a                                           ; 200026
146483: abs      b                                           ; 20002E
146484: sub      x0,a                                        ; 200044
146485: sub      x0,b                                        ; 20004C
146486: asl      a                                           ; 200032
146487: asl      b                                           ; 20003A
146488: move     x:(r0)+,a       a,y:(r4)+                   ; BA9800
146489: move     x:(r0)+,b       b,y:(r4)+                   ; BF9800
14648A: abs      a                                           ; 200026
14648B: abs      b                                           ; 20002E
14648C: sub      x0,a                                        ; 200044
14648D: sub      x0,b                                        ; 20004C
14648E: asl      a                                           ; 200032
14648F: asl      b                                           ; 20003A
146490: move     a,y:(r4)+                                   ; 5E5C00
146491: move     b,y:(r4)+                                   ; 5F5C00
146492: bra      func_1463fb                                 ; 050EC9
146493: move     x:(r0)+,x0      y:(r4),y0                   ; C09800
146494: mpyr     y0,x0,b         x:(r0)+,x0      y:(r4),y0   ; C098D9
146495: abs      b                                           ; 20002E
146496: do       #<$5f,>$14649c                              ; 065F80 14649B
146498: mpyr     y0,x0,a         x:(r0)+,x0      y:(r4),y0   ; C098D1
146499: abs      a               b,y:(r4)+                   ; 5F5C26
14649A: mpyr     y0,x0,b         x:(r0)+,x0      y:(r4),y0   ; C098D9
14649B: abs      b               a,y:(r4)+                   ; 5E5C2E
14649C: mpyr     y0,x0,a                                     ; 2000D1
14649D: abs      a               b,y:(r4)+                   ; 5F5C26
14649E: nop                                                  ; 000000
14649F: move     a,y:(r4)+                                   ; 5E5C00
1464A0: bra      func_1463fb                                 ; 050E9B
1464A1: move     y:(r6+$5),a                                 ; 0216FE
1464A2: asr      #$9,a,a                                     ; 0C1C12
1464A3: move     y:(r6+$19),x0                               ; 0266F4
1464A4: add      x0,a                                        ; 200040
1464A5: nop                                                  ; 000000
1464A6: move     a1,y:(r6+$19)                               ; 0266EC
1464A7: move     y:(r6+$4),x0                                ; 0216B4
1464A8: add      x0,a                                        ; 200040
1464A9: add      x0,a                                        ; 200040
1464AA: move     a1,b                                        ; 218F00
1464AB: nop                                                  ; 000000
1464AC: abs      b                                           ; 20002E
1464AD: nop                                                  ; 000000
1464AE: move     b,x1                                        ; 21E500
1464AF: move     x:(r0)+,a                                   ; 56D800
1464B0: move     x:(r0)+,b                                   ; 57D800
1464B1: do       #<$5f,>$1464b9                              ; 065F80 1464B8
1464B3: sub      x1,b                                        ; 20006C
1464B4: sub      x1,a                                        ; 200064
1464B5: asl      #$7,a,a                                     ; 0C1D0E
1464B6: asl      #$7,b,b                                     ; 0C1D8F
1464B7: move     x:(r0)+,a       a,y:(r4)+                   ; BA9800
1464B8: move     x:(r0)+,b       b,y:(r4)+                   ; BF9800
1464B9: sub      x1,b                                        ; 20006C
1464BA: sub      x1,a                                        ; 200064
1464BB: asl      #$7,a,a                                     ; 0C1D0E
1464BC: asl      #$7,b,b                                     ; 0C1D8F
1464BD: move     a,y:(r4)+                                   ; 5E5C00
1464BE: move     b,y:(r4)+                                   ; 5F5C00
1464BF: bra      func_1463fb                                 ; 050E5C
1464C0: move     #$4,n0                                      ; 380400
1464C1: move     #$4,n4                                      ; 3C0400
1464C2: move     y:(r6+$1a),a                                ; 026EBE
1464C3: move     y:(r6+$24),b                                ; 0296BF
1464C4: move     #>$658100,x1                                ; 45F400 658100
1464C6: do       #<$30,>$1464cc                              ; 063080 1464CB
1464C8: rol      a               a1,y:(r4)+n4                ; 5C4C37
1464C9: eor      x1,a            ifcs                        ; 202863
1464CA: rol      b               b1,y:(r4)+n4                ; 5D4C3F
1464CB: eor      x1,b            ifcs                        ; 20286B
1464CC: move     a1,y:(r6+$1a)                               ; 026EAC
1464CD: move     b1,y:(r6+$24)                               ; 0296AD
1464CE: move     y:(r6+$22),x0                               ; 028EB4
1464CF: move     y:(r6+$23),b                                ; 028EFF
1464D0: do       #<$30,>$1464db                              ; 063080 1464DA
1464D2: move     x:(r0)+n0,a     y:(r4),y0                   ; C88800
1464D3: nop                                                  ; 000000
1464D4: sub      x0,a            a,x0                        ; 21C444
1464D5: tmi      y0,b                                        ; 02B058
1464D6: nop                                                  ; 000000
1464D7: move     b,y:(r4)+                                   ; 5F5C00
1464D8: move     b,y:(r4)+                                   ; 5F5C00
1464D9: move     b,y:(r4)+                                   ; 5F5C00
1464DA: move     b,y:(r4)+                                   ; 5F5C00
1464DB: move     x0,y:(r6+$22)                               ; 028EA4
1464DC: move     b,y:(r6+$23)                                ; 028EEF
1464DD: bra      func_10178a                                 ; 050E1E
1464DE: move     #$2,n0                                      ; 380200
1464DF: move     #>$140800,r1                                ; 61F400 140800
1464E1: move     #>$140800,r5                                ; 65F400 140800
1464E3: move     x:(r0)+n0,a                                 ; 56C800
1464E4: move     x:(r0)+n0,b                                 ; 57C800
1464E5: lsr      #$c,a                                       ; 0C1ED8
1464E6: lsr      #$c,b                                       ; 0C1ED9
1464E7: move     a1,n1                                       ; 219900
1464E8: move     b1,n5                                       ; 21BD00
1464E9: move     x:(r1+n1),x0                                ; 44E900
1464EA: move     y:(r5+n5),y0                                ; 4EED00
1464EB: move     x0,y:(r4)+                                  ; 4C5C00
1464EC: move     x0,y:(r4)+                                  ; 4C5C00
1464ED: do       #<$2f,>$1464f9                              ; 062F80 1464F8
1464EF: move     x:(r0)+n0,a     y0,y:(r4)+                  ; B88800
1464F0: move     x:(r0)+n0,b     y0,y:(r4)+                  ; BC8800
1464F1: lsr      #$c,a                                       ; 0C1ED8
1464F2: lsr      #$c,b                                       ; 0C1ED9
1464F3: move     x:(r1+n1),x0                                ; 44E900
1464F4: move     a1,n1                                       ; 219900
1464F5: move     y:(r5+n5),y0                                ; 4EED00
1464F6: move     b1,n5                                       ; 21BD00
1464F7: move     x0,y:(r4)+                                  ; 4C5C00
1464F8: move     x0,y:(r4)+                                  ; 4C5C00
1464F9: move     y0,y:(r4)+                                  ; 4E5C00
1464FA: move     y0,y:(r4)+                                  ; 4E5C00
1464FB: bra      func_1463fb                                 ; 050E00
1464FC: move     #$0,x0                                      ; 240000
1464FD: move     #>$58,n6                                    ; 76F400 000058
1464FF: lua      (r6)+n6,r0                                  ; 044E10
146500: move     x0,x:(r6+$c)                                ; 023684
146501: move     x0,y:(r6+$c)                                ; 0236A4
146502: move     #>$88,x1                                    ; 45F400 000088
146504: move     x1,y:(r6+$d)                                ; 0236E5
146505: rep      #<$30                                       ; 0630A0
146506: move     x0,x:(r0)+                                  ; 445800
146507: move     #>$a,x0                                     ; 44F400 00000A
146509: move     x0,x:(r6+$1e)                               ; 027E84
14650A: move     #>$1,x0                                     ; 44F400 000001
14650C: move     x0,y:(r6+$1e)                               ; 027EA4
14650D: move     b,y:>$10                                    ; 5F7000 000010
14650F: move     #$0,x0                                      ; 240000
146510: move     x0,x:(r6+$14)                               ; 025684
146511: move     x0,x:(r6+$15)                               ; 0256C4
146512: move     x0,x:(r6+$16)                               ; 025E84
146513: move     x0,x:(r6+$17)                               ; 025EC4
146514: move     x0,x:(r6+$18)                               ; 026684
146515: move     x0,x:(r6+$19)                               ; 0266C4
146516: move     x0,x:(r6+$1a)                               ; 026E84
146517: move     x0,x:(r6+$1b)                               ; 026EC4
146518: move     x0,y:(r6+$14)                               ; 0256A4
146519: move     x0,y:(r6+$15)                               ; 0256E4
14651A: move     x0,y:(r6+$16)                               ; 025EA4
14651B: move     x0,y:(r6+$17)                               ; 025EE4
14651C: move     x0,y:(r6+$18)                               ; 0266A4
14651D: move     x0,y:(r6+$19)                               ; 0266E4
14651E: move     x0,y:(r6+$1a)                               ; 026EA4
14651F: move     x0,y:(r6+$1b)                               ; 026EE4
146520: move     x0,x:(r6+$1c)                               ; 027684
146521: move     x0,y:(r6+$1d)                               ; 0276E4
146522: move     x0,y:(r6+$1c)                               ; 0276A4
146523: move     x0,y:(r6+$1d)                               ; 0276E4
146524: move     y0,y:(r6+$23)                               ; 028EE6
146525: move     x0,y:(r6+$25)                               ; 0296E4
146526: move     #>$64,x0                                    ; 44F400 000064
146528: move     x0,y:(r6+$33)                               ; 02CEE4
146529: move     y:(r6+$4),x0                                ; 0216B4
14652A: move     y:(r6+$5),y0                                ; 0216F6
14652B: move     #>$1300a8,x1                                ; 45F400 1300A8
14652D: mpy      x1,x0,a         #>$19aa73,y1                ; 47F4A0 19AA73
14652F: add      #>$3c0d29,a                                 ; 0140C0 3C0D29
146531: move     #>$49f427,b                                 ; 57F400 49F427
146533: mac      y1,y0,b         #>$6277c6,a                 ; 56F4BA 6277C6
146535: move     a,x:(r6+$31)                                ; 02C6CE
146536: move     b,x:(r6+$32)                                ; 02CE8F
146537: move     #>$679171,b                                 ; 57F400 679171
146539: move     a,y:(r6+$31)                                ; 02C6EE
14653A: move     b,y:(r6+$32)                                ; 02CEAF
14653B: rts                                                  ; 00000C
14653C: move     x:(r6+$10),a                                ; 02469E
14653D: move     y:(r6+$10),a0                               ; 0246B8
14653E: asl      a                                           ; 200032
14653F: move     a,x0                                        ; 21C400
146540: move     x0,x:(r6+$10)                               ; 024684
146541: move     a0,y:(r6+$10)                               ; 0246A8
146542: move     #>$1,y0                                     ; 46F400 000001
146544: nop                                                  ; 000000
146545: move     y0,x:(r6+$1f)                               ; 027EC6
146546: move     #$0,y0                                      ; 260000
146547: nop                                                  ; 000000
146548: move     y0,y:(r6+$1f)                               ; 027EE6
146549: move     #>$2,y0                                     ; 46F400 000002
14654B: move     y0,x:(r6+$23)                               ; 028EC6
14654C: move     #>$0,y0                                     ; 46F400 000000
14654E: move     y0,y:(r6+$25)                               ; 0296E6
14654F: move     #>$3,y0                                     ; 46F400 000003
146551: nop                                                  ; 000000
146552: move     y0,x:(r6+$27)                               ; 029EC6
146553: move     #>$7ffff8,y0                                ; 46F400 7FFFF8
146555: nop                                                  ; 000000
146556: move     y0,y:(r6+$27)                               ; 029EE6
146557: move     #>$1,y0                                     ; 46F400 000001
146559: nop                                                  ; 000000
14655A: move     y0,y:(r6+$28)                               ; 02A6A6
14655B: move     #>$1,y0                                     ; 46F400 000001
14655D: move     #$0,x0                                      ; 240000
14655E: move     #>$f,x1                                     ; 45F400 00000F
146560: move     y0,y:(r6+$36)                               ; 02DEA6
146561: move     x0,x:(r6+$37)                               ; 02DEC4
146562: move     x1,y:(r6+$37)                               ; 02DEE5
146563: move     y:(r6+$8),a                                 ; 0226BE
146564: move     a,x:(r6+$36)                                ; 02DE8E
146565: move     #>$101a51,n4                                ; 74F400 101A51
146567: bsr      func_14720f                                 ; 0D1080 000CA8
146569: move     y:(r4+n4),r0                                ; 68EC00
14656A: move     y:(r6+$9),x0                                ; 0226F4
14656B: move     y:(r6+$9),x1                                ; 0226F5
14656C: mpy      x1,x0,b                                     ; 2000A8
14656D: move     b,x0                                        ; 21E400
14656E: mpy      x0,x0,b                                     ; 200088
14656F: move     b,x0                                        ; 21E400
146570: move     #>$562,x1                                   ; 45F400 000562
146572: mpy      x1,x0,b                                     ; 2000A8
146573: move     b,x1                                        ; 21E500
146574: move     #$0,y0                                      ; 260000
146575: move     y0,y:(r6+$20)                               ; 0286A6
146576: jmp      (r0)                                        ; 0AE080
146577: move     #>$76e3,y0                                  ; 46F400 0076E3
146579: move     y0,x:(r6+$20)                               ; 028686
14657A: move     #>$dc,y0                                    ; 46F400 0000DC
14657C: move     y0,x:(r6+$21)                               ; 0286C6
14657D: move     #>$4a4e,y0                                  ; 46F400 004A4E
14657F: move     y0,y:(r6+$21)                               ; 0286E6
146580: move     #>$be3,y0                                   ; 46F400 000BE3
146582: move     y0,x:(r6+$22)                               ; 028E86
146583: move     #>$0,y0                                     ; 46F400 000000
146585: move     y0,x:(r6+$24)                               ; 029686
146586: move     #>$edc6,y0                                  ; 46F400 00EDC6
146588: move     y0,y:(r6+$24)                               ; 0296A6
146589: move     #$0,y0                                      ; 260000
14658A: nop                                                  ; 000000
14658B: move     y0,x:(r6+$25)                               ; 0296C6
14658C: move     #>$146b3d,r1                                ; 61F400 146B3D
14658E: jmp      func_1469be                                 ; 0AF080 1469BE
146590: move     #>$edc6,y0                                  ; 46F400 00EDC6
146592: move     y0,x:(r6+$20)                               ; 028686
146593: move     x1,y:(r6+$20)                               ; 0286A5
146594: move     #>$d,y0                                     ; 46F400 00000D
146596: move     y0,x:(r6+$21)                               ; 0286C6
146597: move     #$0,y0                                      ; 260000
146598: move     y0,y:(r6+$21)                               ; 0286E6
146599: move     #>$76e3,y0                                  ; 46F400 0076E3
14659B: move     y0,x:(r6+$22)                               ; 028E86
14659C: move     #>$89,y0                                    ; 46F400 000089
14659E: move     y0,x:(r6+$24)                               ; 029686
14659F: move     #>$2526e,y0                                 ; 46F400 02526E
1465A1: move     y0,y:(r6+$24)                               ; 0296A6
1465A2: move     y:(r6+$6),a                                 ; 021EBE
1465A3: move     #>$1,y0                                     ; 46F400 000001
1465A5: move     #>$3,b                                      ; 57F400 000003
1465A7: add      #>$8000,a                                   ; 0140C0 008000
1465A9: lsr      #$10,a                                      ; 0C1EE0
1465AA: rnd      a                                           ; 200011
1465AB: move     #>$40,x0                                    ; 44F400 000040
1465AD: cmp      x0,a                                        ; 200045
1465AE: tfr      y0,b            iflt                        ; 202959
1465AF: move     b,x:(r6+$27)                                ; 029ECF
1465B0: move     x1,b                                        ; 20AF00
1465B1: add      #>$113,b                                    ; 0140C8 000113
1465B3: move     b,x:(r6+$28)                                ; 02A68F
1465B4: move     #$0,y0                                      ; 260000
1465B5: nop                                                  ; 000000
1465B6: move     y0,x:(r6+$25)                               ; 0296C6
1465B7: move     #>$146b41,r1                                ; 61F400 146B41
1465B9: jmp      func_1469be                                 ; 0AF080 1469BE
1465BB: move     #>$edc6,y0                                  ; 46F400 00EDC6
1465BD: move     y0,x:(r6+$20)                               ; 028686
1465BE: move     x1,y:(r6+$20)                               ; 0286A5
1465BF: move     #>$37,y0                                    ; 46F400 000037
1465C1: move     y0,x:(r6+$21)                               ; 0286C6
1465C2: move     #$0,y0                                      ; 260000
1465C3: move     y0,y:(r6+$21)                               ; 0286E6
1465C4: move     #>$2e709e,y0                                ; 46F400 2E709E
1465C6: move     y0,x:(r6+$22)                               ; 028E86
1465C7: move     #>$8f,y0                                    ; 46F400 00008F
1465C9: add      y0,b                                        ; 200058
1465CA: move     b,x:(r6+$24)                                ; 02968F
1465CB: move     #>$18c4a,y0                                 ; 46F400 018C4A
1465CD: move     y0,y:(r6+$24)                               ; 0296A6
1465CE: move     #>$1,y0                                     ; 46F400 000001
1465D0: nop                                                  ; 000000
1465D1: move     y0,x:(r6+$23)                               ; 028EC6
1465D2: move     y:(r6+$6),a                                 ; 021EBE
1465D3: move     #>$1,y0                                     ; 46F400 000001
1465D5: move     #>$3,b                                      ; 57F400 000003
1465D7: add      #>$8000,a                                   ; 0140C0 008000
1465D9: lsr      #$10,a                                      ; 0C1EE0
1465DA: rnd      a                                           ; 200011
1465DB: move     #>$40,x0                                    ; 44F400 000040
1465DD: cmp      x0,a                                        ; 200045
1465DE: tfr      y0,b            iflt                        ; 202959
1465DF: move     b,x:(r6+$27)                                ; 029ECF
1465E0: move     #>$0,y0                                     ; 46F400 000000
1465E2: move     y0,x:(r6+$28)                               ; 02A686
1465E3: move     y:(r6+$a),a                                 ; 022EBE
1465E4: move     a,x0                                        ; 21C400
1465E5: move     #>$147ae,a                                  ; 56F400 0147AE
1465E7: move     #>$a3d71,y0                                 ; 46F400 0A3D71
1465E9: mac      y0,x0,a                                     ; 2000D2
1465EA: move     a,y0                                        ; 21C600
1465EB: nop                                                  ; 000000
1465EC: move     y0,x:(r6+$25)                               ; 0296C6
1465ED: move     #>$146b63,r1                                ; 61F400 146B63
1465EF: jmp      func_1469be                                 ; 0AF080 1469BE
1465F1: move     #>$2e709e,y0                                ; 46F400 2E709E
1465F3: move     y0,x:(r6+$20)                               ; 028686
1465F4: move     #>$0,y0                                     ; 46F400 000000
1465F6: move     y0,x:(r6+$21)                               ; 0286C6
1465F7: move     #>$17384f,y0                                ; 46F400 17384F
1465F9: move     y0,y:(r6+$21)                               ; 0286E6
1465FA: move     #>$76e3,y0                                  ; 46F400 0076E3
1465FC: move     y0,x:(r6+$22)                               ; 028E86
1465FD: move     #>$0,y0                                     ; 46F400 000000
1465FF: move     y0,x:(r6+$24)                               ; 029686
146600: move     #>$2526e,y0                                 ; 46F400 02526E
146602: move     y0,y:(r6+$24)                               ; 0296A6
146603: move     #>$1,y0                                     ; 46F400 000001
146605: nop                                                  ; 000000
146606: move     y0,x:(r6+$23)                               ; 028EC6
146607: move     #>$89,y0                                    ; 46F400 000089
146609: move     y:(r6+$af),y1                               ; 0B76C7 0000AF
14660B: mpy      y1,y0,a                                     ; 2000B0
14660C: move     a,y:(r6+$25)                                ; 0296EE
14660D: move     y:(r6+$6),a                                 ; 021EBE
14660E: move     #>$1,y0                                     ; 46F400 000001
146610: move     #>$3,b                                      ; 57F400 000003
146612: add      #>$8000,a                                   ; 0140C0 008000
146614: lsr      #$10,a                                      ; 0C1EE0
146615: rnd      a                                           ; 200011
146616: move     #>$40,x0                                    ; 44F400 000040
146618: cmp      x0,a                                        ; 200045
146619: tfr      y0,b            iflt                        ; 202959
14661A: move     #>$7ffff8,a                                 ; 56F400 7FFFF8
14661C: move     #>$770a3d,y0                                ; 46F400 770A3D
14661E: tfr      y0,a            iflt                        ; 202951
14661F: move     b,x:(r6+$27)                                ; 029ECF
146620: move     a,y:(r6+$27)                                ; 029EEE
146621: move     #>$7c,y0                                    ; 46F400 00007C
146623: move     y0,x:(r6+$28)                               ; 02A686
146624: move     #$0,y0                                      ; 260000
146625: nop                                                  ; 000000
146626: move     y0,x:(r6+$25)                               ; 0296C6
146627: move     #>$146ba0,r1                                ; 61F400 146BA0
146629: jmp      func_1469be                                 ; 0AF080 1469BE
14662B: move     #>$2e709e,y0                                ; 46F400 2E709E
14662D: move     y0,x:(r6+$20)                               ; 028686
14662E: move     #>$37,y0                                    ; 46F400 000037
146630: move     y0,x:(r6+$21)                               ; 0286C6
146631: move     #$0,y0                                      ; 260000
146632: move     y0,y:(r6+$21)                               ; 0286E6
146633: move     #>$edc6,y0                                  ; 46F400 00EDC6
146635: move     y0,x:(r6+$22)                               ; 028E86
146636: move     #>$39,y0                                    ; 46F400 000039
146638: move     y0,x:(r6+$24)                               ; 029686
146639: move     #>$edc6,y0                                  ; 46F400 00EDC6
14663B: move     y0,y:(r6+$24)                               ; 0296A6
14663C: move     #>$1,y0                                     ; 46F400 000001
14663E: nop                                                  ; 000000
14663F: move     y0,x:(r6+$23)                               ; 028EC6
146640: move     #>$89,y0                                    ; 46F400 000089
146642: move     y:(r6+$af),y1                               ; 0B76C7 0000AF
146644: mpy      y1,y0,a                                     ; 2000B0
146645: move     a,y:(r6+$25)                                ; 0296EE
146646: move     y:(r6+$6),a                                 ; 021EBE
146647: move     #>$1,y0                                     ; 46F400 000001
146649: move     #>$3,b                                      ; 57F400 000003
14664B: add      #>$8000,a                                   ; 0140C0 008000
14664D: lsr      #$10,a                                      ; 0C1EE0
14664E: rnd      a                                           ; 200011
14664F: move     #>$40,x0                                    ; 44F400 000040
146651: cmp      x0,a                                        ; 200045
146652: tfr      y0,b            iflt                        ; 202959
146653: move     b,x:(r6+$27)                                ; 029ECF
146654: move     #>$0,y0                                     ; 46F400 000000
146656: move     y0,x:(r6+$28)                               ; 02A686
146657: move     y:(r6+$a),a                                 ; 022EBE
146658: move     a,x0                                        ; 21C400
146659: move     #>$147ae,a                                  ; 56F400 0147AE
14665B: move     #$40,y0                                     ; 264000
14665C: mac      y0,x0,a                                     ; 2000D2
14665D: move     a,y0                                        ; 21C600
14665E: nop                                                  ; 000000
14665F: move     y0,x:(r6+$25)                               ; 0296C6
146660: move     #>$146bc6,r1                                ; 61F400 146BC6
146662: jmp      func_1469be                                 ; 0AF080 1469BE
146664: move     #>$edc6,y0                                  ; 46F400 00EDC6
146666: move     y0,x:(r6+$20)                               ; 028686
146667: move     x1,y:(r6+$20)                               ; 0286A5
146668: move     #>$37,y0                                    ; 46F400 000037
14666A: move     y0,x:(r6+$21)                               ; 0286C6
14666B: move     #$0,y0                                      ; 260000
14666C: move     y0,y:(r6+$21)                               ; 0286E6
14666D: move     #>$edc6,y0                                  ; 46F400 00EDC6
14666F: move     y0,x:(r6+$22)                               ; 028E86
146670: move     #>$37,y0                                    ; 46F400 000037
146672: move     y0,x:(r6+$24)                               ; 029686
146673: move     #>$18c4a,y0                                 ; 46F400 018C4A
146675: move     y0,y:(r6+$24)                               ; 0296A6
146676: move     y:(r6+$6),a                                 ; 021EBE
146677: move     #>$1,y0                                     ; 46F400 000001
146679: move     #>$3,b                                      ; 57F400 000003
14667B: add      #>$8000,a                                   ; 0140C0 008000
14667D: lsr      #$10,a                                      ; 0C1EE0
14667E: rnd      a                                           ; 200011
14667F: move     #>$40,x0                                    ; 44F400 000040
146681: cmp      x0,a                                        ; 200045
146682: tfr      y0,b            iflt                        ; 202959
146683: move     b,x:(r6+$27)                                ; 029ECF
146684: move     x1,b                                        ; 20AF00
146685: add      #>$7c,b                                     ; 0140C8 00007C
146687: move     b,x:(r6+$28)                                ; 02A68F
146688: move     #>$19999a,y0                                ; 46F400 19999A
14668A: nop                                                  ; 000000
14668B: move     y0,x:(r6+$25)                               ; 0296C6
14668C: move     #>$146d06,r1                                ; 61F400 146D06
14668E: jmp      func_1469be                                 ; 0AF080 1469BE
146690: move     #>$edc6,y0                                  ; 46F400 00EDC6
146692: move     y0,x:(r6+$20)                               ; 028686
146693: move     x1,y:(r6+$20)                               ; 0286A5
146694: move     #>$37,y0                                    ; 46F400 000037
146696: move     y0,x:(r6+$21)                               ; 0286C6
146697: move     #>$12937,y0                                 ; 46F400 012937
146699: move     y0,y:(r6+$21)                               ; 0286E6
14669A: move     #>$76e3,y0                                  ; 46F400 0076E3
14669C: move     y0,x:(r6+$22)                               ; 028E86
14669D: move     #>$c0,y0                                    ; 46F400 0000C0
14669F: add      y0,b                                        ; 200058
1466A0: move     b,x:(r6+$24)                                ; 02968F
1466A1: move     #>$edc6,y0                                  ; 46F400 00EDC6
1466A3: move     y0,y:(r6+$24)                               ; 0296A6
1466A4: move     #>$1,y0                                     ; 46F400 000001
1466A6: nop                                                  ; 000000
1466A7: move     y0,x:(r6+$23)                               ; 028EC6
1466A8: move     #>$89,y0                                    ; 46F400 000089
1466AA: move     y:(r6+$af),y1                               ; 0B76C7 0000AF
1466AC: mpy      y1,y0,a                                     ; 2000B0
1466AD: move     a,y:(r6+$25)                                ; 0296EE
1466AE: move     y:(r6+$6),a                                 ; 021EBE
1466AF: move     #>$1,y0                                     ; 46F400 000001
1466B1: move     #>$3,b                                      ; 57F400 000003
1466B3: add      #>$8000,a                                   ; 0140C0 008000
1466B5: lsr      #$10,a                                      ; 0C1EE0
1466B6: rnd      a                                           ; 200011
1466B7: move     #>$40,x0                                    ; 44F400 000040
1466B9: cmp      x0,a                                        ; 200045
1466BA: tfr      y0,b            iflt                        ; 202959
1466BB: move     b,x:(r6+$27)                                ; 029ECF
1466BC: move     #>$0,y0                                     ; 46F400 000000
1466BE: move     y0,x:(r6+$28)                               ; 02A686
1466BF: move     y:(r6+$a),a                                 ; 022EBE
1466C0: move     a,x0                                        ; 21C400
1466C1: move     #>$ccccd,a                                  ; 56F400 0CCCCD
1466C3: move     #>$733333,y0                                ; 46F400 733333
1466C5: mac      y0,x0,a                                     ; 2000D2
1466C6: move     a,y0                                        ; 21C600
1466C7: nop                                                  ; 000000
1466C8: move     y0,x:(r6+$25)                               ; 0296C6
1466C9: move     #>$146d32,r1                                ; 61F400 146D32
1466CB: jmp      func_1469be                                 ; 0AF080 1469BE
1466CD: move     #>$2e709e,y0                                ; 46F400 2E709E
1466CF: move     y0,x:(r6+$20)                               ; 028686
1466D0: move     #>$18,y0                                    ; 46F400 000018
1466D2: move     y0,x:(r6+$21)                               ; 0286C6
1466D3: move     #>$420c4,y0                                 ; 46F400 0420C4
1466D5: move     y0,y:(r6+$21)                               ; 0286E6
1466D6: move     #>$edc6,y0                                  ; 46F400 00EDC6
1466D8: move     y0,x:(r6+$22)                               ; 028E86
1466D9: move     #>$1b,y0                                    ; 46F400 00001B
1466DB: move     y0,x:(r6+$24)                               ; 029686
1466DC: move     #>$2526e,y0                                 ; 46F400 02526E
1466DE: move     y0,y:(r6+$24)                               ; 0296A6
1466DF: move     #>$1,y0                                     ; 46F400 000001
1466E1: nop                                                  ; 000000
1466E2: move     y0,x:(r6+$23)                               ; 028EC6
1466E3: move     #>$89,y0                                    ; 46F400 000089
1466E5: move     y:(r6+$af),y1                               ; 0B76C7 0000AF
1466E7: mpy      y1,y0,a                                     ; 2000B0
1466E8: move     a,y:(r6+$25)                                ; 0296EE
1466E9: move     y:(r6+$6),a                                 ; 021EBE
1466EA: move     #>$1,y0                                     ; 46F400 000001
1466EC: move     #>$3,b                                      ; 57F400 000003
1466EE: add      #>$8000,a                                   ; 0140C0 008000
1466F0: lsr      #$10,a                                      ; 0C1EE0
1466F1: rnd      a                                           ; 200011
1466F2: move     #>$40,x0                                    ; 44F400 000040
1466F4: cmp      x0,a                                        ; 200045
1466F5: tfr      y0,b            iflt                        ; 202959
1466F6: move     b,x:(r6+$27)                                ; 029ECF
1466F7: move     #>$0,y0                                     ; 46F400 000000
1466F9: move     y0,x:(r6+$28)                               ; 02A686
1466FA: move     y:(r6+$a),a                                 ; 022EBE
1466FB: move     a,x0                                        ; 21C400
1466FC: move     #>$66666,a                                  ; 56F400 066666
1466FE: move     #>$266666,y0                                ; 46F400 266666
146700: mac      y0,x0,a                                     ; 2000D2
146701: move     a,y0                                        ; 21C600
146702: nop                                                  ; 000000
146703: move     y0,x:(r6+$25)                               ; 0296C6
146704: move     #>$146bfc,r1                                ; 61F400 146BFC
146706: jmp      func_1469be                                 ; 0AF080 1469BE
1469BE: rts                                                  ; 00000C
1469BF: move     #$0,x0                                      ; 240000
1469C0: move     x0,x:>$44                                   ; 447000 000044
1469C2: move     x0,x:>$45                                   ; 447000 000045
1469C4: move     x0,x:>$46                                   ; 447000 000046
1469C6: move     x0,x:>$47                                   ; 447000 000047
1469C8: move     x0,x:>$48                                   ; 447000 000048
1469CA: move     x0,x:>$49                                   ; 447000 000049
1469CC: move     x0,x:>$4a                                   ; 447000 00004A
1469CE: move     x0,x:>$4b                                   ; 447000 00004B
1469D0: move     a,y:>$10                                    ; 5E7000 000010
1469D2: move     a,y:(r6+$11)                                ; 0246EE
1469D3: move     y:(r6+$4),x0                                ; 0216B4
1469D4: move     y:(r6+$5),y0                                ; 0216F6
1469D5: move     #>$1300a8,x1                                ; 45F400 1300A8
1469D7: mpy      x1,x0,a         #>$19aa73,y1                ; 47F4A0 19AA73
1469D9: add      #>$3c0d29,a                                 ; 0140C0 3C0D29
1469DB: move     #>$49f427,b                                 ; 57F400 49F427
1469DD: mac      y1,y0,b         a,x:>$40                    ; 5670BA 000040
1469DF: move     #>$6277c6,a                                 ; 56F400 6277C6
1469E1: move     b,x:>$41                                    ; 577000 000041
1469E3: move     #>$679171,b                                 ; 57F400 679171
1469E5: move     a,x:>$42                                    ; 567000 000042
1469E7: move     b,x:>$43                                    ; 577000 000043
1469E9: move     x:>$41,a                                    ; 56F000 000041
1469EB: move     x:>$42,b                                    ; 57F000 000042
1469ED: sub      a,b                                         ; 20001C
1469EE: move     #>$1a52f,x0                                 ; 44F400 01A52F
1469F0: cmp      x0,b                                        ; 20004D
1469F1: bge      func_1469f6                                 ; 0D1041 000005
1469F3: add      x0,a                                        ; 200040
1469F4: move     a,x:>$42                                    ; 567000 000042
1469F6: move     x:(r6+$23),b                                ; 028EDF
1469F7: cmp      #>$1,b                                      ; 0140CD 000001
1469F9: bgt      func_146a0c                                 ; 0D1047 000013
1469FB: move     x:(r6+$31),x0                               ; 02C6D4
1469FC: move     x0,x:>$40                                   ; 447000 000040
1469FE: move     x:(r6+$32),x0                               ; 02CE94
1469FF: move     x0,x:>$41                                   ; 447000 000041
146A01: move     y:(r6+$31),x0                               ; 02C6F4
146A02: move     x0,x:>$42                                   ; 447000 000042
146A04: move     y:(r6+$32),x0                               ; 02CEB4
146A05: move     x0,x:>$43                                   ; 447000 000043
146A07: move     y:(r6+$33),x0                               ; 02CEF4
146A08: move     x0,y:>$10                                   ; 4C7000 000010
146A0A: jmp      func_146a1b                                 ; 0AF080 146A1B
146A0C: move     x:>$40,x0                                   ; 44F000 000040
146A0E: move     x0,x:(r6+$31)                               ; 02C6C4
146A0F: move     x:>$41,x0                                   ; 44F000 000041
146A11: move     x0,x:(r6+$32)                               ; 02CE84
146A12: move     x:>$42,x0                                   ; 44F000 000042
146A14: move     x0,y:(r6+$31)                               ; 02C6E4
146A15: move     x:>$43,x0                                   ; 44F000 000043
146A17: move     x0,y:(r6+$32)                               ; 02CEA4
146A18: move     y:>$10,x0                                   ; 4CF000 000010
146A1A: move     x0,y:(r6+$33)                               ; 02CEE4
146A1B: move     y:>$10,a                                    ; 5EF000 000010
146A1D: clb      a,b                                         ; 0C1E01
146A1E: normf    b1,a                                        ; 0C1E26
146A1F: move     a,x0                                        ; 21C400
146A20: move     #>$2b1100,a                                 ; 56F400 2B1100
146A22: andi     #$fe,ccr                                    ; 00FEB9
146A23: rep      #<$18                                       ; 0618A0
146A24: div      x0,a                                        ; 018040
146A25: move     a0,a                                        ; 210E00
146A26: add      #>$1d,b                                     ; 0140C8 00001D
146A28: normf    b1,a                                        ; 0C1E26
146A29: move     x:(r6+$c),b                                 ; 02369F
146A2A: move     y:(r6+$c),b0                                ; 0236B9
146A2B: move     a1,x1                                       ; 218500
146A2C: move     a0,x0                                       ; 210400
146A2D: move     #$0,r1                                      ; 310000
146A2E: bsr      func_0003e0                                 ; 0D1080 EB99B2
146A30: move     b1,x:(r6+$c)                                ; 02368D
146A31: move     b0,y:(r6+$c)                                ; 0236A9
146A32: move     r7,y:(r6+$f)                                ; 0B7697 00000F
146A34: move     #>$80,r7                                    ; 67F400 000080
146A36: move     #$1,m7                                      ; 0501A7
146A37: move     #>$40,x0                                    ; 44F400 000040
146A39: move     #>$ccccd,y0                                 ; 46F400 0CCCCD
146A3B: move     y0,y:(r7)+                                  ; 4E5F00
146A3C: move     y0,y:(r7)                                   ; 4E6700
146A3D: move     y:(r6+$d),a                                 ; 0236FE
146A3E: add      #>$10,a                                     ; 0140C0 000010
146A40: cmp      #>$98,a                                     ; 0140C5 000098
146A42: sub      x0,a            ifeq                        ; 202A44
146A43: move     a,y:(r6+$d)                                 ; 0236EE
146A44: move     a,n6                                        ; 21DE00
146A45: lua      (r6)+n6,r3                                  ; 044E13
146A46: move     #$0,x1                                      ; 250000
146A47: move     #$3f,m3                                     ; 053FA3
146A48: move     #$10,n3                                     ; 3B1000
146A49: move     (r3)-n3                                     ; 204300
146A4A: rep      #<$10                                       ; 0610A0
146A4B: move     x1,x:(r3)+                                  ; 455B00
146A4C: bsr      func_0003aa                                 ; 0D1080 EB995E
146A4E: move     y:(r6+$f),r7                                ; 0B76D7 00000F
146A50: move     #>$ffffff,m7                                ; 05F427 FFFFFF
146A52: move     #>$ffffff,m3                                ; 05F423 FFFFFF
146A54: move     #>$30,r5                                    ; 65F400 000030
146A56: move     x:(r6+$1e),b                                ; 027E9F
146A57: move     y:(r6+$1e),x1                               ; 027EB5
146A58: do       #<$8,>$146a62                               ; 060880 146A61
146A5A: add      x1,b            x1,x0                       ; 20A468
146A5B: move     x0,a1                                       ; 208C00
146A5C: move     b1,x1                                       ; 21A500
146A5D: move     b1,x:(r5)+                                  ; 555D00
146A5E: add      x1,a            x1,x0                       ; 20A460
146A5F: move     x0,b1                                       ; 208D00
146A60: move     a1,x1                                       ; 218500
146A61: move     a1,x:(r5)+                                  ; 545D00
146A62: move     x1,x:(r6+$1e)                               ; 027E85
146A63: move     b1,y:(r6+$1e)                               ; 027EAD
146A64: move     #>$30,r1                                    ; 61F400 000030
146A66: move     #>$f,r2                                     ; 62F400 00000F
146A68: move     y:(r6+$7),a                                 ; 021EFE
146A69: move     a,b                                         ; 21CF00
146A6A: asr      #$9,b,b                                     ; 0C1C93
146A6B: add      #>$800000,a                                 ; 0140C0 800000
146A6D: asr      #$1,a,a                                     ; 0C1C02
146A6E: move     b,x0                                        ; 21E400
146A6F: move     a,x1                                        ; 21C500
146A70: move     #$10,y1                                     ; 271000
146A71: move     x:(r2),b                                    ; 57E200
146A72: do       #<$10,>$146a78                              ; 061080 146A77
146A74: move     x:(r1),y0                                   ; 46E100
146A75: mpy      y1,y0,a         b,x:(r2)+                   ; 575AB0
146A76: mpy      y0,x0,b         x:(r3)+,y0                  ; 46DBD8
146A77: mac      x1,y0,b         a,x:(r1)+                   ; 5659EA
146A78: move     b,x:(r2)+                                   ; 575A00
146A79: move     x:(r6+$23),a                                ; 028EDE
146A7A: cmp      #>$2,a                                      ; 0140C5 000002
146A7C: blt      func_146a84                                 ; 0D1049 000008
146A7E: beq      func_146aa0                                 ; 0D104A 000022
146A80: bgt      func_146aad                                 ; 0D1047 00002D
146A82: jmp      func_146abd                                 ; 0AF080 146ABD
146A84: move     y:(r6+$af),x0                               ; 0B76C4 0000AF
146A86: move     y:(r6+$23),x1                               ; 028EF5
146A87: mpy      x1,x0,a                                     ; 2000A0
146A88: cmp      #>$147ae,a                                  ; 0140C5 0147AE
146A8A: ble      func_146a91                                 ; 0D104F 000007
146A8C: sub      #>$edc6,a                                   ; 0140C4 00EDC6
146A8E: move     a,y:(r6+$23)                                ; 028EEE
146A8F: jmp      func_146b2d                                 ; 0AF080 146B2D
146A91: move     #$0,x0                                      ; 240000
146A92: move     x0,y:(r6+$23)                               ; 028EE4
146A93: move     y:(r6+$25),b                                ; 0296FF
146A94: cmp      #>$0,b                                      ; 0140CD 000000
146A96: ble      func_146a9d                                 ; 0D104F 000007
146A98: sub      #>$1,b                                      ; 0140CC 000001
146A9A: move     b,y:(r6+$25)                                ; 0296EF
146A9B: jmp      func_146b2d                                 ; 0AF080 146B2D
146A9D: move     #>$2,x0                                     ; 44F400 000002
146A9F: move     x0,x:(r6+$23)                               ; 028EC4
146AA0: move     x:(r6+$24),b                                ; 02969F
146AA1: cmp      #>$0,b                                      ; 0140CD 000000
146AA3: ble      func_146aaa                                 ; 0D104F 000007
146AA5: sub      #>$1,b                                      ; 0140CC 000001
146AA7: move     b,x:(r6+$24)                                ; 02968F
146AA8: jmp      func_146abd                                 ; 0AF080 146ABD
146AAA: move     #>$3,x0                                     ; 44F400 000003
146AAC: move     x0,x:(r6+$23)                               ; 028EC4
146AAD: move     y:(r6+$23),a                                ; 028EFE
146AAE: move     y:(r6+$24),b                                ; 0296BF
146AAF: cmp      #>$7eb852,a                                 ; 0140C5 7EB852
146AB1: bgt      func_146ab7                                 ; 0D1047 000006
146AB3: add      a,b                                         ; 200018
146AB4: move     b,y:(r6+$23)                                ; 028EEF
146AB5: jmp      func_146abd                                 ; 0AF080 146ABD
146AB7: move     #>$7fffac,x0                                ; 44F400 7FFFAC
146AB9: move     #>$4,x1                                     ; 45F400 000004
146ABB: move     x0,y:(r6+$23)                               ; 028EE4
146ABC: move     x1,x:(r6+$23)                               ; 028EC5
146ABD: move     x:(r6+$1f),a                                ; 027EDE
146ABE: cmp      #>$2,a                                      ; 0140C5 000002
146AC0: blt      func_146acc                                 ; 0D1049 00000C
146AC2: beq      func_146ad9                                 ; 0D104A 000017
146AC4: cmp      #>$3,a                                      ; 0140C5 000003
146AC6: beq      func_146ae6                                 ; 0D104A 000020
146AC8: bgt      func_146af7                                 ; 0D1047 00002F
146ACA: jmp      func_146b06                                 ; 0AF080 146B06
146ACC: move     y:(r6+$1f),a                                ; 027EFE
146ACD: move     x:(r6+$20),b                                ; 02869F
146ACE: cmp      #>$7eb852,a                                 ; 0140C5 7EB852
146AD0: bgt      func_146ad6                                 ; 0D1047 000006
146AD2: add      a,b                                         ; 200018
146AD3: move     b,y:(r6+$1f)                                ; 027EEF
146AD4: jmp      func_146b06                                 ; 0AF080 146B06
146AD6: move     #>$2,x0                                     ; 44F400 000002
146AD8: move     x0,x:(r6+$1f)                               ; 027EC4
146AD9: move     y:(r6+$20),b                                ; 0286BF
146ADA: cmp      #>$0,b                                      ; 0140CD 000000
146ADC: ble      func_146ae3                                 ; 0D104F 000007
146ADE: sub      #>$1,b                                      ; 0140CC 000001
146AE0: move     b,y:(r6+$20)                                ; 0286AF
146AE1: jmp      func_146b06                                 ; 0AF080 146B06
146AE3: move     #>$3,x0                                     ; 44F400 000003
146AE5: move     x0,x:(r6+$1f)                               ; 027EC4
146AE6: move     x:(r6+$21),b                                ; 0286DF
146AE7: cmp      #>$0,b                                      ; 0140CD 000000
146AE9: ble      func_146af4                                 ; 0D104F 00000B
146AEB: move     y:(r6+$21),x0                               ; 0286F4
146AEC: move     y:(r6+$1f),a                                ; 027EFE
146AED: sub      #>$1,b                                      ; 0140CC 000001
146AEF: sub      x0,a                                        ; 200044
146AF0: move     b,x:(r6+$21)                                ; 0286CF
146AF1: move     a,y:(r6+$1f)                                ; 027EEE
146AF2: jmp      func_146b06                                 ; 0AF080 146B06
146AF4: move     #>$4,x0                                     ; 44F400 000004
146AF6: move     x0,x:(r6+$1f)                               ; 027EC4
146AF7: move     y:(r6+$1f),a                                ; 027EFE
146AF8: move     x:(r6+$22),b                                ; 028E9F
146AF9: cmp      #>$147ae,a                                  ; 0140C5 0147AE
146AFB: blt      func_146b01                                 ; 0D1049 000006
146AFD: sub      b,a                                         ; 200014
146AFE: move     a,y:(r6+$1f)                                ; 027EEE
146AFF: jmp      func_146b06                                 ; 0AF080 146B06
146B01: move     #$0,x0                                      ; 240000
146B02: move     #>$5,x1                                     ; 45F400 000005
146B04: move     x0,y:(r6+$1f)                               ; 027EE4
146B05: move     x1,x:(r6+$1f)                               ; 027EC5
146B06: move     x:(r6+$27),a                                ; 029EDE
146B07: cmp      #>$2,a                                      ; 0140C5 000002
146B09: blt      func_146b0f                                 ; 0D1049 000006
146B0B: beq      func_146b1d                                 ; 0D104A 000012
146B0D: jmp      func_146b2d                                 ; 0AF080 146B2D
146B0F: move     x:(r6+$28),b                                ; 02A69F
146B10: cmp      #>$0,b                                      ; 0140CD 000000
146B12: ble      func_146b19                                 ; 0D104F 000007
146B14: sub      #>$1,b                                      ; 0140CC 000001
146B16: move     b,x:(r6+$28)                                ; 02A68F
146B17: jmp      func_146b2d                                 ; 0AF080 146B2D
146B19: move     x:(r6+$27),a                                ; 029EDE
146B1A: add      #>$1,a                                      ; 0140C0 000001
146B1C: move     a,x:(r6+$27)                                ; 029ECE
146B1D: move     y:(r6+$27),a                                ; 029EFE
146B1E: cmp      #>$147ae,a                                  ; 0140C5 0147AE
146B20: blt      func_146b27                                 ; 0D1049 000007
146B22: sub      #>$2526e,a                                  ; 0140C4 02526E
146B24: move     a,y:(r6+$27)                                ; 029EEE
146B25: jmp      func_146b2d                                 ; 0AF080 146B2D
146B27: move     x:(r6+$27),a                                ; 029EDE
146B28: add      #>$1,a                                      ; 0140C0 000001
146B2A: move     #$0,x0                                      ; 240000
146B2B: move     a,x:(r6+$27)                                ; 029ECE
146B2C: move     x0,y:(r6+$27)                               ; 029EE4
146B2D: move     x:(r6+$23),a                                ; 028EDE
146B2E: cmp      #>$1,a                                      ; 0140C5 000001
146B30: beq      func_146b3d                                 ; 0D104A 00000D
146B32: move     y:(r6+$28),a                                ; 02A6BE
146B33: cmp      #>$1,a                                      ; 0140C5 000001
146B35: bgt      func_146b3d                                 ; 0D1047 000008
146B37: move     #>$101a66,n4                                ; 74F400 101A66
146B39: bsr      func_14720f                                 ; 0D1080 0006D6
146B3B: move     y:(r4+n4),r0                                ; 68EC00
146B3C: jmp      (r0)                                        ; 0AE080
146B3D: bsr      func_1471ec                                 ; 0D1080 0006AF
146B3F: jmp      func_146f90                                 ; 0AF080 146F90
146B41: move     x:>$40,a                                    ; 56F000 000040
146B43: move     #>$40543d,b                                 ; 57F400 40543D
146B45: sub      a,b                                         ; 20001C
146B46: move     y:(r6+$1f),x0                               ; 027EF4
146B47: move     b,x1                                        ; 21E500
146B48: mpy      x1,x0,a                                     ; 2000A0
146B49: move     a,x:>$44                                    ; 567000 000044
146B4B: move     x:>$41,a                                    ; 56F000 000041
146B4D: move     #>$607e5b,b                                 ; 57F400 607E5B
146B4F: sub      a,b                                         ; 20001C
146B50: move     y:(r6+$1f),x0                               ; 027EF4
146B51: move     b,x1                                        ; 21E500
146B52: mpy      x1,x0,a                                     ; 2000A0
146B53: move     a,x:>$45                                    ; 567000 000045
146B55: move     x:>$42,a                                    ; 56F000 000042
146B57: move     #>$652eba,b                                 ; 57F400 652EBA
146B59: sub      a,b                                         ; 20001C
146B5A: move     y:(r6+$1f),x0                               ; 027EF4
146B5B: move     b,x1                                        ; 21E500
146B5C: mpy      x1,x0,a                                     ; 2000A0
146B5D: move     a,x:>$46                                    ; 567000 000046
146B5F: bsr      func_1471ec                                 ; 0D1080 00068D
146B61: jmp      func_146f90                                 ; 0AF080 146F90
146B63: move     y:(r6+$23),a                                ; 028EFE
146B64: move     #>$7ffff8,b                                 ; 57F400 7FFFF8
146B66: sub      a,b #>$41cf5,y0                             ; 46F41C 041CF5
146B68: move     b,x0                                        ; 21E400
146B69: mpy      y0,x0,a                                     ; 2000D0
146B6A: move     a,x:>$44                                    ; 567000 000044
146B6C: move     a,x:>$45                                    ; 567000 000045
146B6E: move     a,x:>$46                                    ; 567000 000046
146B70: move     a,x:>$47                                    ; 567000 000047
146B72: move     y:(r6+$1f),x0                               ; 027EF4
146B73: move     #>$41cf5,y0                                 ; 46F400 041CF5
146B75: mpy      y0,x0,a                                     ; 2000D0
146B76: move     a,x:>$48                                    ; 567000 000048
146B78: move     a,x:>$49                                    ; 567000 000049
146B7A: move     a,x:>$4a                                    ; 567000 00004A
146B7C: move     a,x:>$4b                                    ; 567000 00004B
146B7E: move     x:(r6+$23),b                                ; 028EDF
146B7F: move     y:(r6+$28),a                                ; 02A6BE
146B80: move     #>$2,x0                                     ; 44F400 000002
146B82: cmp      #>$3,b                                      ; 0140CD 000003
146B84: tfr      x0,a            ifgt                        ; 202741
146B85: move     a,y:(r6+$28)                                ; 02A6AE
146B86: move     #>$30e9a,y0                                 ; 46F400 030E9A
146B88: move     #>$61d34,y1                                 ; 47F400 061D34
146B8A: move     #>$ad3392,x0                                ; 44F400 AD3392
146B8C: move     #>$1f06d6,x1                                ; 45F400 1F06D6
146B8E: move     y0,y:>$30                                   ; 4E7000 000030
146B90: move     y1,y:>$31                                   ; 4F7000 000031
146B92: move     y0,y:>$32                                   ; 4E7000 000032
146B94: move     x0,y:>$33                                   ; 4C7000 000033
146B96: move     x1,y:>$34                                   ; 4D7000 000034
146B98: move     #>$30,r1                                    ; 61F400 000030
146B9A: move     #>$20,r5                                    ; 65F400 000020
146B9C: bsr      func_1471ac                                 ; 0D1080 000610
146B9E: jmp      func_146f90                                 ; 0AF080 146F90
146BA0: move     y:(r6+$23),a                                ; 028EFE
146BA1: move     #>$7ffff8,b                                 ; 57F400 7FFFF8
146BA3: move     x:>$40,x1                                   ; 45F000 000040
146BA5: sub      a,b                                         ; 20001C
146BA6: mpyi     #>$ccccd,x1,a                               ; 0141E0 0CCCCD
146BA8: move     b,x0                                        ; 21E400
146BA9: move     a,x1                                        ; 21C500
146BAA: mpy      -x1,x0,a                                    ; 2000A4
146BAB: move     a,x:>$44                                    ; 567000 000044
146BAD: move     y:(r6+$23),a                                ; 028EFE
146BAE: move     #>$7ffff8,b                                 ; 57F400 7FFFF8
146BB0: move     x:>$41,x1                                   ; 45F000 000041
146BB2: sub      a,b                                         ; 20001C
146BB3: mpyi     #>$ccccd,x1,a                               ; 0141E0 0CCCCD
146BB5: move     b,x0                                        ; 21E400
146BB6: move     a,x1                                        ; 21C500
146BB7: mpy      -x1,x0,a                                    ; 2000A4
146BB8: move     a,x:>$45                                    ; 567000 000045
146BBA: move     x:(r6+$23),b                                ; 028EDF
146BBB: move     y:(r6+$28),a                                ; 02A6BE
146BBC: move     #>$2,x0                                     ; 44F400 000002
146BBE: cmp      #>$3,b                                      ; 0140CD 000003
146BC0: tfr      x0,a            ifgt                        ; 202741
146BC1: move     a,y:(r6+$28)                                ; 02A6AE
146BC2: bsr      func_1471ec                                 ; 0D1080 00062A
146BC4: jmp      func_146f90                                 ; 0AF080 146F90
146BC6: move     y:(r6+$23),a                                ; 028EFE
146BC7: move     #>$7ffff8,b                                 ; 57F400 7FFFF8
146BC9: move     #>$41cf5,x0                                 ; 44F400 041CF5
146BCB: sub      a,b                                         ; 20001C
146BCC: move     b,x1                                        ; 21E500
146BCD: mpy      x1,x0,a                                     ; 2000A0
146BCE: move     a,x:>$46                                    ; 567000 000046
146BD0: move     y:(r6+$1f),a                                ; 027EFE
146BD1: move     #>$7ffff8,b                                 ; 57F400 7FFFF8
146BD3: move     #>$41cf5,x0                                 ; 44F400 041CF5
146BD5: sub      a,b                                         ; 20001C
146BD6: move     b,x1                                        ; 21E500
146BD7: mpy      x1,x0,a                                     ; 2000A0
146BD8: move     a,x:>$4a                                    ; 567000 00004A
146BDA: move     #>$6a9d,y0                                  ; 46F400 006A9D
146BDC: move     #>$d53a,y1                                  ; 47F400 00D53A
146BDE: move     #>$8f6951,x0                                ; 44F400 8F6951
146BE0: move     #>$324122,x1                                ; 45F400 324122
146BE2: move     y0,y:>$30                                   ; 4E7000 000030
146BE4: move     y1,y:>$31                                   ; 4F7000 000031
146BE6: move     y0,y:>$32                                   ; 4E7000 000032
146BE8: move     x0,y:>$33                                   ; 4C7000 000033
146BEA: move     x1,y:>$34                                   ; 4D7000 000034
146BEC: move     #>$30,r1                                    ; 61F400 000030
146BEE: move     #>$20,r5                                    ; 65F400 000020
146BF0: bsr      func_1471ac                                 ; 0D1080 0005BC
146BF2: move     x:(r6+$23),b                                ; 028EDF
146BF3: move     y:(r6+$28),a                                ; 02A6BE
146BF4: move     #>$2,x0                                     ; 44F400 000002
146BF6: cmp      #>$3,b                                      ; 0140CD 000003
146BF8: tfr      x0,a            ifgt                        ; 202741
146BF9: move     a,y:(r6+$28)                                ; 02A6AE
146BFA: jmp      func_146f90                                 ; 0AF080 146F90
146BFC: move     y:(r6+$23),a                                ; 028EFE
146BFD: move     #>$7ffff8,b                                 ; 57F400 7FFFF8
146BFF: move     #>$511c4c,x0                                ; 44F400 511C4C
146C01: sub      a,b                                         ; 20001C
146C02: move     b,x1                                        ; 21E500
146C03: mpy      x1,x0,a                                     ; 2000A0
146C04: move     a,x:>$46                                    ; 567000 000046
146C06: move     x:(r6+$23),b                                ; 028EDF
146C07: move     y:(r6+$28),a                                ; 02A6BE
146C08: move     #>$2,x0                                     ; 44F400 000002
146C0A: cmp      #>$3,b                                      ; 0140CD 000003
146C0C: tfr      x0,a            ifgt                        ; 202741
146C0D: move     a,y:(r6+$28)                                ; 02A6AE
146C0E: move     #>$f879,y0                                  ; 46F400 00F879
146C10: move     #>$1f0f3,y1                                 ; 47F400 01F0F3
146C12: move     #>$98261c,x0                                ; 44F400 98261C
146C14: move     #>$2bbbca,x1                                ; 45F400 2BBBCA
146C16: move     y0,y:>$30                                   ; 4E7000 000030
146C18: move     y1,y:>$31                                   ; 4F7000 000031
146C1A: move     y0,y:>$32                                   ; 4E7000 000032
146C1C: move     x0,y:>$33                                   ; 4C7000 000033
146C1E: move     x1,y:>$34                                   ; 4D7000 000034
146C20: move     #>$30,r1                                    ; 61F400 000030
146C22: move     #>$20,r5                                    ; 65F400 000020
146C24: bsr      func_1471ac                                 ; 0D1080 000588
146C26: jmp      func_146f90                                 ; 0AF080 146F90
146C28: move     y:(r6+$1f),x0                               ; 027EF4
146C29: move     #>$3c0d29,x1                                ; 45F400 3C0D29
146C2B: mpy      -x1,x0,a                                    ; 2000A4
146C2C: move     a,x:>$44                                    ; 567000 000044
146C2E: move     x:>$42,a                                    ; 56F000 000042
146C30: move     #>$5aa728,b                                 ; 57F400 5AA728
146C32: sub      a,b                                         ; 20001C
146C33: move     b,x1                                        ; 21E500
146C34: mpy      x1,x0,a                                     ; 2000A0
146C35: move     a,x:>$46                                    ; 567000 000046
146C37: bsr      func_1471ec                                 ; 0D1080 0005B5
146C39: jmp      func_146f90                                 ; 0AF080 146F90
146C3B: move     y:(r6+$36),a                                ; 02DEBE
146C3C: cmp      #>$2,a                                      ; 0140C5 000002
146C3E: blt      func_146c4a                                 ; 0D1049 00000C
146C40: beq      func_146c5a                                 ; 0D104A 00001A
146C42: cmp      #>$3,a                                      ; 0140C5 000003
146C44: beq      func_146c6a                                 ; 0D104A 000026
146C46: bgt      func_146c79                                 ; 0D1047 000033
146C48: jmp      func_146c89                                 ; 0AF080 146C89
146C4A: move     x:(r6+$37),b                                ; 02DEDF
146C4B: cmp      #>$7eb852,b                                 ; 0140CD 7EB852
146C4D: bge      func_146c54                                 ; 0D1041 000007
146C4F: add      #>$28f5c,b                                  ; 0140C8 028F5C
146C51: move     b,x:(r6+$37)                                ; 02DECF
146C52: jmp      func_146c89                                 ; 0AF080 146C89
146C54: move     #>$7fffac,y0                                ; 46F400 7FFFAC
146C56: move     #>$2,x0                                     ; 44F400 000002
146C58: move     y0,x:(r6+$37)                               ; 02DEC6
146C59: move     x0,y:(r6+$36)                               ; 02DEA4
146C5A: move     y:(r6+$37),b                                ; 02DEFF
146C5B: cmp      #>$0,b                                      ; 0140CD 000000
146C5D: ble      func_146c64                                 ; 0D104F 000007
146C5F: sub      #>$1,b                                      ; 0140CC 000001
146C61: move     b,y:(r6+$37)                                ; 02DEEF
146C62: jmp      func_146c89                                 ; 0AF080 146C89
146C64: move     #>$f,y0                                     ; 46F400 00000F
146C66: move     #>$3,x0                                     ; 44F400 000003
146C68: move     y0,y:(r6+$37)                               ; 02DEE6
146C69: move     x0,y:(r6+$36)                               ; 02DEA4
146C6A: move     x:(r6+$37),b                                ; 02DEDF
146C6B: cmp      #>$28f5c,b                                  ; 0140CD 028F5C
146C6D: ble      func_146c74                                 ; 0D104F 000007
146C6F: sub      #>$28f5c,b                                  ; 0140CC 028F5C
146C71: move     b,x:(r6+$37)                                ; 02DECF
146C72: jmp      func_146c89                                 ; 0AF080 146C89
146C74: move     #$0,y0                                      ; 260000
146C75: move     #>$4,x0                                     ; 44F400 000004
146C77: move     y0,x:(r6+$37)                               ; 02DEC6
146C78: move     x0,y:(r6+$36)                               ; 02DEA4
146C79: move     y:(r6+$37),b                                ; 02DEFF
146C7A: cmp      #>$0,b                                      ; 0140CD 000000
146C7C: ble      func_146c83                                 ; 0D104F 000007
146C7E: sub      #>$1,b                                      ; 0140CC 000001
146C80: move     b,y:(r6+$37)                                ; 02DEEF
146C81: jmp      func_146c89                                 ; 0AF080 146C89
146C83: move     #>$f,y0                                     ; 46F400 00000F
146C85: move     #>$1,x0                                     ; 44F400 000001
146C87: move     y0,y:(r6+$37)                               ; 02DEE6
146C88: move     x0,y:(r6+$36)                               ; 02DEA4
146C89: move     y:(r6+$1f),x0                               ; 027EF4
146C8A: move     x:(r6+$37),x1                               ; 02DED5
146C8B: mpy      x1,x0,a                                     ; 2000A0
146C8C: move     a,x0                                        ; 21C400
146C8D: move     #>$3c0d29,x1                                ; 45F400 3C0D29
146C8F: mpy      -x1,x0,a                                    ; 2000A4
146C90: move     a,x:>$44                                    ; 567000 000044
146C92: move     y:(r6+$1f),x0                               ; 027EF4
146C93: move     x:>$42,a                                    ; 56F000 000042
146C95: move     #>$5aa728,b                                 ; 57F400 5AA728
146C97: sub      a,b                                         ; 20001C
146C98: move     b,x1                                        ; 21E500
146C99: mpy      x1,x0,a                                     ; 2000A0
146C9A: move     a,x:>$46                                    ; 567000 000046
146C9C: move     #>$30,r1                                    ; 61F400 000030
146C9E: move     #>$20,r5                                    ; 65F400 000020
146CA0: move     #>$10,r3                                    ; 63F400 000010
146CA2: move     y:(r6+$1f),y0                               ; 027EF6
146CA3: move     #>$59999a,y1                                ; 47F400 59999A
146CA5: mpy      y1,y0,a                                     ; 2000B0
146CA6: move     #>$7fffac,b                                 ; 57F400 7FFFAC
146CA8: sub      a,b                                         ; 20001C
146CA9: move     b,y0                                        ; 21E600
146CAA: do       #<$10,>$146cb0                              ; 061080 146CAF
146CAC: move     x:(r3),y1                                   ; 47E300
146CAD: mpy      y1,y0,a         x:(r1)+,x0                  ; 44D9B0
146CAE: move     x0,x:(r5)+                                  ; 445D00
146CAF: move     a,x:(r3)+                                   ; 565B00
146CB0: move     x:(r6+$1f),b                                ; 027EDF
146CB1: move     y:(r6+$28),a                                ; 02A6BE
146CB2: move     #>$2,x0                                     ; 44F400 000002
146CB4: cmp      #>$5,b                                      ; 0140CD 000005
146CB6: tfr      x0,a            ifeq                        ; 202A41
146CB7: move     a,y:(r6+$28)                                ; 02A6AE
146CB8: jmp      func_146f90                                 ; 0AF080 146F90
146CBA: move     x:>$41,a                                    ; 56F000 000041
146CBC: move     #>$6262b7,b                                 ; 57F400 6262B7
146CBE: sub      a,b                                         ; 20001C
146CBF: move     y:(r6+$1f),x0                               ; 027EF4
146CC0: move     b,x1                                        ; 21E500
146CC1: mpy      x1,x0,a                                     ; 2000A0
146CC2: move     a,x:>$45                                    ; 567000 000045
146CC4: move     a,x:>$49                                    ; 567000 000049
146CC6: move     #>$4774a,y0                                 ; 46F400 04774A
146CC8: move     #>$8ee95,y1                                 ; 47F400 08EE95
146CCA: move     #>$b7e897,x0                                ; 44F400 B7E897
146CCC: move     #>$19f492,x1                                ; 45F400 19F492
146CCE: move     y0,y:>$30                                   ; 4E7000 000030
146CD0: move     y1,y:>$31                                   ; 4F7000 000031
146CD2: move     y0,y:>$32                                   ; 4E7000 000032
146CD4: move     x0,y:>$33                                   ; 4C7000 000033
146CD6: move     x1,y:>$34                                   ; 4D7000 000034
146CD8: move     #$30,r1                                     ; 313000
146CD9: move     #$20,r5                                     ; 352000
146CDA: bsr      func_1471ac                                 ; 0D1080 0004D2
146CDC: jmp      func_146f90                                 ; 0AF080 146F90
146F90: move     #>$ffffff,m4                                ; 05F424 FFFFFF
146F92: move     #>$10,r0                                    ; 60F400 000010
146F94: move     x:>$44,a                                    ; 56F000 000044
146F96: move     x:>$40,x1                                   ; 45F000 000040
146F98: add      x1,a            #>$614,x0                   ; 44F460 000614
146F9A: move     a,x1                                        ; 21C500
146F9B: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
146F9D: mpy      x1,y0,b         #>$1001d1,r3                ; 63F4E8 1001D1
146F9F: move     a1,r2                                       ; 219200
146FA0: move     b1,n3                                       ; 21BB00
146FA1: move     #>$10,r1                                    ; 61F400 000010
146FA3: move     #>$30,r5                                    ; 65F400 000030
146FA5: move     x:(r2+$141b23),x1                           ; 0A72C5 141B23
146FA7: move     #>$8074b7,x0                                ; 44F400 8074B7
146FA9: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
146FAA: move     x:(r6+$29),x0                               ; 02A6D4
146FAB: move     x:(r6+$2a),x1                               ; 02AE95
146FAC: sub      x0,a            a,y0                        ; 21C644
146FAD: sub      x1,b            b,y1                        ; 21E76C
146FAE: move     y0,x:(r6+$29)                               ; 02A6C6
146FAF: move     y1,x:(r6+$2a)                               ; 02AE87
146FB0: tfr      x0,a            a,y0                        ; 21C641
146FB1: tfr      x1,b            b,y1                        ; 21E769
146FB2: move     #$8,x0                                      ; 240800
146FB3: move     #>$10,r4                                    ; 64F400 000010
146FB5: do       #<$10,>$146fb9                              ; 061080 146FB8
146FB7: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
146FB8: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
146FB9: move     x:(r6+$14),b                                ; 02569F
146FBA: move     x:(r6+$15),y1                               ; 0256D7
146FBB: move     #>$10,r4                                    ; 64F400 000010
146FBD: move     #>$7f1766,x1                                ; 45F400 7F1766
146FBF: bsr      func_1471f5                                 ; 0D1080 000236
146FC1: move     b,x:(r6+$14)                                ; 02568F
146FC2: move     y1,x:(r6+$15)                               ; 0256C7
146FC3: move     x:>$45,a                                    ; 56F000 000045
146FC5: move     x:>$41,x1                                   ; 45F000 000041
146FC7: add      x1,a            #>$614,x0                   ; 44F460 000614
146FC9: move     a,x1                                        ; 21C500
146FCA: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
146FCC: mpy      x1,y0,b         #>$1007f1,r3                ; 63F4E8 1007F1
146FCE: move     a1,r2                                       ; 219200
146FCF: move     b1,n3                                       ; 21BB00
146FD0: move     #>$30,r1                                    ; 61F400 000030
146FD2: move     #>$10,r5                                    ; 65F400 000010
146FD4: move     x:(r2+$141b23),x1                           ; 0A72C5 141B23
146FD6: move     #>$80af13,x0                                ; 44F400 80AF13
146FD8: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
146FD9: move     x:(r6+$2b),x0                               ; 02AED4
146FDA: move     x:(r6+$2c),x1                               ; 02B695
146FDB: sub      x0,a            a,y0                        ; 21C644
146FDC: sub      x1,b            b,y1                        ; 21E76C
146FDD: move     y0,x:(r6+$2b)                               ; 02AEC6
146FDE: move     y1,x:(r6+$2c)                               ; 02B687
146FDF: tfr      x0,a            a,y0                        ; 21C641
146FE0: tfr      x1,b            b,y1                        ; 21E769
146FE1: move     #$8,x0                                      ; 240800
146FE2: move     #>$10,r4                                    ; 64F400 000010
146FE4: do       #<$10,>$146fe8                              ; 061080 146FE7
146FE6: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
146FE7: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
146FE8: move     x:(r6+$16),b                                ; 025E9F
146FE9: move     x:(r6+$17),y1                               ; 025ED7
146FEA: move     #>$7ea3b7,x1                                ; 45F400 7EA3B7
146FEC: move     #>$10,r4                                    ; 64F400 000010
146FEE: bsr      func_1471f5                                 ; 0D1080 000207
146FF0: move     b,x:(r6+$16)                                ; 025E8F
146FF1: move     y1,x:(r6+$17)                               ; 025EC7
146FF2: move     x:>$46,a                                    ; 56F000 000046
146FF4: move     x:>$42,x1                                   ; 45F000 000042
146FF6: add      x1,a            #>$614,x0                   ; 44F460 000614
146FF8: move     a,x1                                        ; 21C500
146FF9: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
146FFB: mpy      x1,y0,b         #>$100e11,r3                ; 63F4E8 100E11
146FFD: move     a1,r2                                       ; 219200
146FFE: move     b1,n3                                       ; 21BB00
146FFF: move     #>$10,r1                                    ; 61F400 000010
147001: move     #>$30,r5                                    ; 65F400 000030
147003: move     y:(r2+$141b23),x1                           ; 0B72C5 141B23
147005: move     #>$80e96e,x0                                ; 44F400 80E96E
147007: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
147008: move     x:(r6+$2d),x0                               ; 02B6D4
147009: move     x:(r6+$2e),x1                               ; 02BE95
14700A: sub      x0,a            a,y0                        ; 21C644
14700B: sub      x1,b            b,y1                        ; 21E76C
14700C: move     y0,x:(r6+$2d)                               ; 02B6C6
14700D: move     y1,x:(r6+$2e)                               ; 02BE87
14700E: tfr      x0,a            a,y0                        ; 21C641
14700F: tfr      x1,b            b,y1                        ; 21E769
147010: move     #$8,x0                                      ; 240800
147011: move     #>$10,r4                                    ; 64F400 000010
147013: do       #<$10,>$147017                              ; 061080 147016
147015: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
147016: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
147017: move     x:(r6+$18),b                                ; 02669F
147018: move     x:(r6+$19),y1                               ; 0266D7
147019: move     #>$7e3072,x1                                ; 45F400 7E3072
14701B: move     #>$10,r4                                    ; 64F400 000010
14701D: bsr      func_1471f5                                 ; 0D1080 0001D8
14701F: move     b,x:(r6+$18)                                ; 02668F
147020: move     y1,x:(r6+$19)                               ; 0266C7
147021: move     x:>$47,a                                    ; 56F000 000047
147023: move     x:>$43,x1                                   ; 45F000 000043
147025: add      x1,a            #>$614,x0                   ; 44F460 000614
147027: move     a,x1                                        ; 21C500
147028: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
14702A: mpy      x1,y0,b         #>$101431,r3                ; 63F4E8 101431
14702C: move     a1,r2                                       ; 219200
14702D: move     b1,n3                                       ; 21BB00
14702E: move     #>$30,r1                                    ; 61F400 000030
147030: move     #>$10,r5                                    ; 65F400 000010
147032: move     y:(r2+$141b23),x1                           ; 0B72C5 141B23
147034: move     #>$815e23,x0                                ; 44F400 815E23
147036: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
147037: move     x:(r6+$2f),x0                               ; 02BED4
147038: move     x:(r6+$30),x1                               ; 02C695
147039: sub      x0,a            a,y0                        ; 21C644
14703A: sub      x1,b            b,y1                        ; 21E76C
14703B: move     y0,x:(r6+$2f)                               ; 02BEC6
14703C: move     y1,x:(r6+$30)                               ; 02C687
14703D: tfr      x0,a            a,y0                        ; 21C641
14703E: tfr      x1,b            b,y1                        ; 21E769
14703F: move     #$8,x0                                      ; 240800
147040: move     #>$10,r4                                    ; 64F400 000010
147042: do       #<$10,>$147046                              ; 061080 147045
147044: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
147045: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
147046: move     x:(r6+$1a),b                                ; 026E9F
147047: move     x:(r6+$1b),y1                               ; 026ED7
147048: move     #>$7d4b22,x1                                ; 45F400 7D4B22
14704A: move     #>$10,r4                                    ; 64F400 000010
14704C: bsr      func_1471f5                                 ; 0D1080 0001A9
14704E: move     b,x:(r6+$1a)                                ; 026E8F
14704F: move     y1,x:(r6+$1b)                               ; 026EC7
147050: move     #>$30,r5                                    ; 65F400 000030
147052: move     #$0,x0                                      ; 240000
147053: rep      #<$10                                       ; 0610A0
147054: move     x0,x:(r5)+                                  ; 445D00
147055: move     x:>$48,a                                    ; 56F000 000048
147057: move     x:>$40,x1                                   ; 45F000 000040
147059: add      x1,a            #>$614,x0                   ; 44F460 000614
14705B: move     a,x1                                        ; 21C500
14705C: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
14705E: mpy      x1,y0,b         #>$1001d1,r3                ; 63F4E8 1001D1
147060: move     a1,r2                                       ; 219200
147061: move     b1,n3                                       ; 21BB00
147062: move     #>$20,r1                                    ; 61F400 000020
147064: move     #>$30,r5                                    ; 65F400 000030
147066: move     y:(r2+$141b23),x1                           ; 0B72C5 141B23
147068: move     #>$8074b7,x0                                ; 44F400 8074B7
14706A: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
14706B: move     y:(r6+$29),x0                               ; 02A6F4
14706C: move     y:(r6+$2a),x1                               ; 02AEB5
14706D: sub      x0,a            a,y0                        ; 21C644
14706E: sub      x1,b            b,y1                        ; 21E76C
14706F: move     y0,y:(r6+$29)                               ; 02A6E6
147070: move     y1,y:(r6+$2a)                               ; 02AEA7
147071: tfr      x0,a            a,y0                        ; 21C641
147072: tfr      x1,b            b,y1                        ; 21E769
147073: move     #$8,x0                                      ; 240800
147074: move     #>$10,r4                                    ; 64F400 000010
147076: do       #<$10,>$14707a                              ; 061080 147079
147078: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
147079: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
14707A: move     y:(r6+$14),a                                ; 0256BE
14707B: move     y:(r6+$15),y1                               ; 0256F7
14707C: move     #>$10,r4                                    ; 64F400 000010
14707E: move     #>$7f1766,x1                                ; 45F400 7F1766
147080: bsr      func_147204                                 ; 0D1080 000184
147082: move     a,y:(r6+$14)                                ; 0256AE
147083: move     y1,y:(r6+$15)                               ; 0256E7
147084: move     x:>$49,a                                    ; 56F000 000049
147086: move     x:>$41,x1                                   ; 45F000 000041
147088: add      x1,a            #>$614,x0                   ; 44F460 000614
14708A: move     a,x1                                        ; 21C500
14708B: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
14708D: mpy      x1,y0,b         #>$1007f1,r3                ; 63F4E8 1007F1
14708F: move     a1,r2                                       ; 219200
147090: move     b1,n3                                       ; 21BB00
147091: move     #>$20,r1                                    ; 61F400 000020
147093: move     #>$30,r5                                    ; 65F400 000030
147095: move     x:(r2+$141b23),x1                           ; 0A72C5 141B23
147097: move     #>$80af13,x0                                ; 44F400 80AF13
147099: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
14709A: move     y:(r6+$2b),x0                               ; 02AEF4
14709B: move     y:(r6+$2c),x1                               ; 02B6B5
14709C: sub      x0,a            a,y0                        ; 21C644
14709D: sub      x1,b            b,y1                        ; 21E76C
14709E: move     y0,y:(r6+$2b)                               ; 02AEE6
14709F: move     y1,y:(r6+$2c)                               ; 02B6A7
1470A0: tfr      x0,a            a,y0                        ; 21C641
1470A1: tfr      x1,b            b,y1                        ; 21E769
1470A2: move     #$8,x0                                      ; 240800
1470A3: move     #>$10,r4                                    ; 64F400 000010
1470A5: do       #<$10,>$1470a9                              ; 061080 1470A8
1470A7: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
1470A8: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
1470A9: move     y:(r6+$16),a                                ; 025EBE
1470AA: move     y:(r6+$17),y1                               ; 025EF7
1470AB: move     #>$10,r4                                    ; 64F400 000010
1470AD: move     #>$7ea3b7,x1                                ; 45F400 7EA3B7
1470AF: bsr      func_147204                                 ; 0D1080 000155
1470B1: move     a,y:(r6+$16)                                ; 025EAE
1470B2: move     y1,y:(r6+$17)                               ; 025EE7
1470B3: move     x:>$4a,a                                    ; 56F000 00004A
1470B5: move     x:>$42,x1                                   ; 45F000 000042
1470B7: add      x1,a            #>$614,x0                   ; 44F460 000614
1470B9: move     a,x1                                        ; 21C500
1470BA: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
1470BC: mpy      x1,y0,b         #>$100e11,r3                ; 63F4E8 100E11
1470BE: move     a1,r2                                       ; 219200
1470BF: move     b1,n3                                       ; 21BB00
1470C0: move     #>$20,r1                                    ; 61F400 000020
1470C2: move     #>$30,r5                                    ; 65F400 000030
1470C4: move     y:(r2+$141b23),x1                           ; 0B72C5 141B23
1470C6: move     #>$80e96e,x0                                ; 44F400 80E96E
1470C8: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
1470C9: move     y:(r6+$2d),x0                               ; 02B6F4
1470CA: move     y:(r6+$2e),x1                               ; 02BEB5
1470CB: sub      x0,a            a,y0                        ; 21C644
1470CC: sub      x1,b            b,y1                        ; 21E76C
1470CD: move     y0,y:(r6+$2d)                               ; 02B6E6
1470CE: move     y1,y:(r6+$2e)                               ; 02BEA7
1470CF: tfr      x0,a            a,y0                        ; 21C641
1470D0: tfr      x1,b            b,y1                        ; 21E769
1470D1: move     #$8,x0                                      ; 240800
1470D2: move     #>$10,r4                                    ; 64F400 000010
1470D4: do       #<$10,>$1470d8                              ; 061080 1470D7
1470D6: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
1470D7: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
1470D8: move     y:(r6+$18),a                                ; 0266BE
1470D9: move     y:(r6+$19),y1                               ; 0266F7
1470DA: move     #>$10,r4                                    ; 64F400 000010
1470DC: move     #>$7e3072,x1                                ; 45F400 7E3072
1470DE: bsr      func_147204                                 ; 0D1080 000126
1470E0: move     a,y:(r6+$18)                                ; 0266AE
1470E1: move     y1,y:(r6+$19)                               ; 0266E7
1470E2: move     x:>$4b,a                                    ; 56F000 00004B
1470E4: move     x:>$43,x1                                   ; 45F000 000043
1470E6: add      x1,a            #>$614,x0                   ; 44F460 000614
1470E8: move     a,x1                                        ; 21C500
1470E9: mpy      x1,x0,a         #>$620,y0                   ; 46F4A0 000620
1470EB: mpy      x1,y0,b         #>$101431,r3                ; 63F4E8 101431
1470ED: move     a1,r2                                       ; 219200
1470EE: move     b1,n3                                       ; 21BB00
1470EF: move     #>$20,r1                                    ; 61F400 000020
1470F1: move     #>$30,r5                                    ; 65F400 000030
1470F3: move     x:(r2+$141b23),x1                           ; 0A72C5 141B23
1470F5: move     #>$815e23,x0                                ; 44F400 815E23
1470F7: mpy      x1,x0,b         y:(r3+n3),a                 ; 5EEBA8
1470F8: move     y:(r6+$2f),x0                               ; 02BEF4
1470F9: move     y:(r6+$30),x1                               ; 02C6B5
1470FA: sub      x0,a            a,y0                        ; 21C644
1470FB: sub      x1,b            b,y1                        ; 21E76C
1470FC: move     y0,y:(r6+$2f)                               ; 02BEE6
1470FD: move     y1,y:(r6+$30)                               ; 02C6A7
1470FE: tfr      x0,a            a,y0                        ; 21C641
1470FF: tfr      x1,b            b,y1                        ; 21E769
147100: move     #$8,x0                                      ; 240800
147101: move     #>$10,r4                                    ; 64F400 000010
147103: do       #<$10,>$147107                              ; 061080 147106
147105: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
147106: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
147107: move     y:(r6+$1a),a                                ; 026EBE
147108: move     y:(r6+$1b),y1                               ; 026EF7
147109: move     #>$10,r4                                    ; 64F400 000010
14710B: move     #>$7d4b22,x1                                ; 45F400 7D4B22
14710D: bsr      func_147204                                 ; 0D1080 0000F7
14710F: move     a,y:(r6+$1a)                                ; 026EAE
147110: move     y1,y:(r6+$1b)                               ; 026EE7
147111: move     #>$19999a,x0                                ; 44F400 19999A
147113: move     x0,x:(r6-$1d)                               ; 038EC4
147114: move     #>$10,r0                                    ; 60F400 000010
147116: move     y:(r6+$11),a                                ; 0246FE
147117: add      #>$20c5,a                                   ; 0140C0 0020C5
147119: move     x:(r6+$10),b                                ; 02469F
14711A: move     y:(r6+$10),b0                               ; 0246B9
14711B: move     a,y0                                        ; 21C600
14711C: move     x:(r6+$12),a                                ; 024E9E
14711D: move     y:(r6+$12),a0                               ; 024EB8
14711E: move     x:(r0)+,x0                                  ; 44D800
14711F: do       #<$10,>$147125                              ; 061080 147124
147121: mac      x0,x0,b         b,x1                        ; 21E58A
147122: mac      -x1,y0,b        x:(r0)+,x0                  ; 44D8EE
147123: mac      x1,y0,a         a,x1                        ; 21C5E2
147124: mac      -x1,y0,a                                    ; 2000E6
147125: move     b,x0                                        ; 21E400
147126: move     x0,x:(r6+$10)                               ; 024684
147127: move     b0,y:(r6+$10)                               ; 0246A9
147128: move     a,x0                                        ; 21C400
147129: move     x0,x:(r6+$12)                               ; 024E84
14712A: move     a0,y:(r6+$12)                               ; 024EA8
14712B: or       #>$1,a                                      ; 0140C2 000001
14712D: clb      a,b                                         ; 0C1E01
14712E: normf    b1,a                                        ; 0C1E26
14712F: move     a1,x0                                       ; 218400
147130: mpyi     #>$200,x0,a                                 ; 0141C0 000200
147132: move     a1,r1                                       ; 219100
147133: neg      b                                           ; 20003E
147134: move     x:(r1+$1444c6),b0                           ; 0A71C9 1444C6
147136: asl      #$11,b,b                                    ; 0C1DA3
147137: move     #$60,a                                      ; 2E6000
147138: sub      b,a                                         ; 200014
147139: clr      a               ifmi                        ; 202B13
14713A: asr      #$7,a,a                                     ; 0C1C0E
14713B: rnd      a                                           ; 200011
14713C: asr      #$b,a,b                                     ; 0C1C17
14713D: and      #>$7ff,a                                    ; 0140C6 0007FF
14713F: neg      a                                           ; 200036
147140: move     a1,r2                                       ; 219200
147141: move     b1,x0                                       ; 21A400
147142: move     x:(r2+$1407ff),a                            ; 0A72CE 1407FF
147144: asr      x0,a,b                                      ; 0C1E69
147145: asl      #$c,b,b                                     ; 0C1D99
147146: move     b,y:>$ff                                    ; 5F7000 0000FF
147148: move     #>$10,r1                                    ; 61F400 000010
14714A: move     #>$30,r5                                    ; 65F400 000030
14714C: move     #>$20,r2                                    ; 62F400 000020
14714E: move     #>$7ffff8,x0                                ; 44F400 7FFFF8
147150: move     x:(r6+$23),b                                ; 028EDF
147151: cmp      #>$1,b                                      ; 0140CD 000001
147153: beq      func_147159                                 ; 0D104A 000006
147155: move     y:(r6+$af),a                                ; 0B76CE 0000AF
147157: asl      #$8,a,a                                     ; 0C1D10
147158: move     a,x0                                        ; 21C400
147159: move     y:(r6+$23),x1                               ; 028EF5
14715A: mpy      x1,x0,a                                     ; 2000A0
14715B: move     a,x0                                        ; 21C400
14715C: move     y:(r6+$27),x1                               ; 029EF5
14715D: mpy      x1,x0,a                                     ; 2000A0
14715E: move     a,y:(r6+$23)                                ; 028EEE
14715F: move     y:>$ff,x1                                   ; 4DF000 0000FF
147161: move     a,x0                                        ; 21C400
147162: mpy      x1,x0,a                                     ; 2000A0
147163: move     y:(r6+$1f),b                                ; 027EFF
147164: move     y:(r6+$26),x0                               ; 029EB4
147165: move     x:(r6+$26),x1                               ; 029E95
147166: sub      x0,a            a,y0                        ; 21C644
147167: sub      x1,b            b,y1                        ; 21E76C
147168: move     y0,y:(r6+$26)                               ; 029EA6
147169: move     y1,x:(r6+$26)                               ; 029E87
14716A: tfr      x0,a            a,y0                        ; 21C641
14716B: tfr      x1,b            b,y1                        ; 21E769
14716C: move     #$8,x0                                      ; 240800
14716D: move     #>$10,r4                                    ; 64F400 000010
14716F: do       #<$10,>$147173                              ; 061080 147172
147171: mac      y0,x0,a         a,y:(r4)+                   ; 5E5CD2
147172: mac      x0,y1,b         b,y:(r4)+                   ; 5F5CCA
147173: move     x:(r1)+,x0                                  ; 44D900
147174: move     #>$10,r4                                    ; 64F400 000010
147176: move     y:(r4)+,x1                                  ; 4DDC00
147177: move     x:(r6+$25),y1                               ; 0296D7
147178: do       #<$10,>$147181                              ; 061080 147180
14717A: mpy      x1,x0,a         x:(r5)+,x0                  ; 44DDA0
14717B: asl      #$c,a,a                                     ; 0C1D18
14717C: mpy      x0,y1,b         y:(r4)+,y0                  ; 4EDCC8
14717D: move     y:(r4)+,x1                                  ; 4DDC00
14717E: move     b,x0                                        ; 21E400
14717F: mac      y0,x0,a         x:(r1)+,x0                  ; 44D9D2
147180: move     a,x:(r2)+                                   ; 565A00
147181: move     #>$3fffd6,y0                                ; 46F400 3FFFD6
147183: move     #>$941206,y1                                ; 47F400 941206
147185: move     #>$2e6f69,x0                                ; 44F400 2E6F69
147187: move     #>$8b8d50,x1                                ; 45F400 8B8D50
147189: move     y0,y:>$30                                   ; 4E7000 000030
14718B: move     y1,y:>$31                                   ; 4F7000 000031
14718D: move     x0,y:>$32                                   ; 4C7000 000032
14718F: move     x1,y:>$33                                   ; 4D7000 000033
147191: move     #>$372839,y0                                ; 46F400 372839
147193: move     y0,y:>$34                                   ; 4E7000 000034
147195: move     #>$20,r1                                    ; 61F400 000020
147197: move     #>$30,r5                                    ; 65F400 000030
147199: bsr      func_1471cb                                 ; 0D1080 000032
14719B: move     #>$30,r1                                    ; 61F400 000030
14719D: move     x:(r1)+,a                                   ; 56D900
14719E: move     x:(r1)+,b                                   ; 57D900
14719F: do       #<$8,>$1471a5                               ; 060880 1471A4
1471A1: move     a,y:(r7)+                                   ; 5E5F00
1471A2: move     x:(r1)+,a       a,y:(r7)+                   ; BAF900
1471A3: move     b,y:(r7)+                                   ; 5F5F00
1471A4: move     x:(r1)+,b       b,y:(r7)+                   ; BFF900
1471A5: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1471A7: move     #>$ffffff,m2                                ; 05F422 FFFFFF
1471A9: move     #>$ffffff,m4                                ; 05F424 FFFFFF
1471AB: rts                                                  ; 00000C
1471AC: move     x:(r6+$1c),x0                               ; 027694
1471AD: move     x:(r6+$1d),y1                               ; 0276D7
1471AE: move     y:(r6+$1c),y0                               ; 0276B6
1471AF: move     y:(r6+$1d),x1                               ; 0276F5
1471B0: move     #>$30,r4                                    ; 64F400 000030
1471B2: move     #>$4,m4                                     ; 05F424 000004
1471B4: lua      (r1-$1),r2                                  ; 0439F2
1471B5: lua      (r1-$2),r3                                  ; 0439E3
1471B6: move     x0,x:(r2)                                   ; 446200
1471B7: move     y1,x:(r3)                                   ; 476300
1471B8: move     x:(r1)+,x0      y:(r4)+,y1                  ; F19900
1471B9: do       #<$10,>$1471c2                              ; 061080 1471C1
1471BB: mpy      x0,y1,a         x:(r2)+,x0      y:(r4)+,y1  ; F19AC0
1471BC: mac      x0,y1,a         x:(r3)+,x0      y:(r4)+,y1  ; F19BC2
1471BD: mac      x0,y1,a         y:(r4)+,y1                  ; 4FDCC2
1471BE: mac      -y1,y0,a        y:(r4)+,y1                  ; 4FDCB6
1471BF: mac      -y1,x1,a        x:(r1)+,x0      y:(r4)+,y1  ; F199F6
1471C0: asl      a               y0,x1                       ; 20C532
1471C1: move     a,x:(r5)+       a,y0                        ; 181D00
1471C2: move     x:(r2),x0                                   ; 44E200
1471C3: move     x:-(r2),y1                                  ; 47FA00
1471C4: move     x0,x:(r6+$1c)                               ; 027684
1471C5: move     y1,x:(r6+$1d)                               ; 0276C7
1471C6: move     y0,y:(r6+$1c)                               ; 0276A6
1471C7: move     x1,y:(r6+$1d)                               ; 0276E5
1471C8: move     #>$ffffff,m4                                ; 05F424 FFFFFF
1471CA: rts                                                  ; 00000C
1471CB: move     x:(r6+$34),x0                               ; 02D694
1471CC: move     x:(r6+$35),y1                               ; 02D6D7
1471CD: move     y:(r6+$34),y0                               ; 02D6B6
1471CE: move     y:(r6+$35),x1                               ; 02D6F5
1471CF: move     #>$20,r1                                    ; 61F400 000020
1471D1: move     #>$30,r5                                    ; 65F400 000030
1471D3: move     #>$30,r4                                    ; 64F400 000030
1471D5: move     #>$4,m4                                     ; 05F424 000004
1471D7: lua      (r1-$1),r2                                  ; 0439F2
1471D8: lua      (r1-$2),r3                                  ; 0439E3
1471D9: move     x0,x:(r2)                                   ; 446200
1471DA: move     y1,x:(r3)                                   ; 476300
1471DB: move     x:(r1)+,x0      y:(r4)+,y1                  ; F19900
1471DC: do       #<$10,>$1471e5                              ; 061080 1471E4
1471DE: mpy      x0,y1,a         x:(r2)+,x0      y:(r4)+,y1  ; F19AC0
1471DF: mac      x0,y1,a         x:(r3)+,x0      y:(r4)+,y1  ; F19BC2
1471E0: mac      x0,y1,a         y:(r4)+,y1                  ; 4FDCC2
1471E1: mac      -y1,y0,a        y:(r4)+,y1                  ; 4FDCB6
1471E2: mac      -y1,x1,a        x:(r1)+,x0      y:(r4)+,y1  ; F199F6
1471E3: asl      a               y0,x1                       ; 20C532
1471E4: move     a,x:(r5)+       a,y0                        ; 181D00
1471E5: move     x:(r2),x0                                   ; 44E200
1471E6: move     x:-(r2),y1                                  ; 47FA00
1471E7: move     x0,x:(r6+$34)                               ; 02D684
1471E8: move     y1,x:(r6+$35)                               ; 02D6C7
1471E9: move     y0,y:(r6+$34)                               ; 02D6A6
1471EA: move     x1,y:(r6+$35)                               ; 02D6E5
1471EB: rts                                                  ; 00000C
1471EC: move     #>$30,r1                                    ; 61F400 000030
1471EE: move     #>$20,r5                                    ; 65F400 000020
1471F0: do       #<$10,>$1471f4                              ; 061080 1471F3
1471F2: move     x:(r1)+,x0                                  ; 44D900
1471F3: move     x0,x:(r5)+                                  ; 445D00
1471F4: rts                                                  ; 00000C
1471F5: move     x:(r1)+,x0      y:(r4)+,y0                  ; F09900
1471F6: mpy      y0,x0,a         b,x0            y:(r4)+,y0  ; 18DCD0
1471F7: mac      -y0,x0,a                                    ; 2000D6
1471F8: mac      -y0,x0,a                                    ; 2000D6
1471F9: mac      -y1,x1,a        x:(r1)+,x0      y:(r4)+,y0  ; F099F6
1471FA: tfr      a,b             b,y1                        ; 21E709
1471FB: do       #<$f,>$147202                               ; 060F80 147201
1471FD: mpy      y0,x0,a         a,x0            y:(r4)+,y0  ; 10DCD0
1471FE: mac      -y0,x0,a        x0,x:(r5)+                  ; 445DD6
1471FF: mac      -y0,x0,a                                    ; 2000D6
147200: mac      -y1,x1,a        x:(r1)+,x0      y:(r4)+,y0  ; F099F6
147201: tfr      a,b             b,y1                        ; 21E709
147202: move     a,x:(r5)+                                   ; 565D00
147203: rts                                                  ; 00000C
147204: move     x:(r1)+,x0      y:(r4)+,y0                  ; F09900
147205: move     x:-(r5),b                                   ; 57FD00
147206: do       #<$10,>$14720d                              ; 061080 14720C
147208: mpy      y0,x0,a         a,x0            y:(r4)+,y0  ; 10DCD0
147209: mac      -y0,x0,a        b,x:(r5)+                   ; 575DD6
14720A: mac      -y0,x0,a        x:(r5),b                    ; 57E5D6
14720B: mac      -y1,x1,a        x0,y1                       ; 2087F6
14720C: add      a,b             x:(r1)+,x0      y:(r4)+,y0  ; F09918
14720D: move     b,x:(r5)+                                   ; 575D00
14720E: rts                                                  ; 00000C
14720F: move     x:(r6+$36),x0                               ; 02DE94
147210: mpyi     #>$15,x0,a                                  ; 0141C0 000015
147212: move     a1,r4                                       ; 219400
147213: rts                                                  ; 00000C
147214: move     r6,r0                                       ; 22D000
147215: move     #$58,n0                                     ; 385800
147216: move     #$0,x0                                      ; 240000
147217: move     x0,x:(r6+$16)                               ; 025E84
147218: move     x0,y:(r6+$16)                               ; 025EA4
147219: move     x0,x:(r6+$17)                               ; 025EC4
14721A: move     x0,y:(r6+$17)                               ; 025EE4
14721B: move     x0,x:(r6+$18)                               ; 026684
14721C: move     x0,y:(r6+$18)                               ; 0266A4
14721D: move     (r0)+n0                                     ; 204800
14721E: move     x0,y:(r6+$34)                               ; 02D6A4
14721F: move     x0,x:(r6+$23)                               ; 028EC4
147220: move     x0,x:(r6+$24)                               ; 029684
147221: move     x0,x:(r6+$25)                               ; 0296C4
147222: move     x0,x:(r6+$20)                               ; 028684
147223: move     x0,x:(r6+$21)                               ; 0286C4
147224: move     x0,x:(r6+$22)                               ; 028E84
147225: rep      #<$30                                       ; 0630A0
147226: move     x0,x:(r0)+                                  ; 445800
147227: move     #>$88,x0                                    ; 44F400 000088
147229: move     x0,y:(r6+$19)                               ; 0266E4
14722A: move     #>$80,x0                                    ; 44F400 000080
14722C: move     x0,y:(r6+$1a)                               ; 026EA4
14722D: move     x0,y:(r6+$1b)                               ; 026EE4
14722E: move     x0,y:(r6+$1c)                               ; 0276A4
14722F: move     x0,y:(r6+$1d)                               ; 0276E4
147230: move     x0,y:(r6+$1e)                               ; 027EA4
147231: move     x0,y:(r6+$1f)                               ; 027EE4
147232: move     #>$14a000,x0                                ; 44F400 14A000
147234: move     x0,x:(r6+$2e)                               ; 02BE84
147235: move     x0,x:(r6+$31)                               ; 02C6C4
147236: rts                                                  ; 00000C
147237: move     y:(r6+$a),a                                 ; 022EBE
147238: move     y:(r6+$34),b                                ; 02D6BF
147239: cmp      #>$400000,a                                 ; 0140C5 400000
14723B: clr      b               ifge                        ; 20211B
14723C: move     b,y:(r6+$34)                                ; 02D6AF
14723D: rts                                                  ; 00000C
14723E: move     #>$17c6f9,x0                                ; 44F400 17C6F9
147240: move     a1,y1                                       ; 218700
147241: move     a0,y0                                       ; 210600
147242: mpysu    x0,y0,b                                     ; 0127AD
147243: dmac     ss x0,y1,b                                  ; 0124A4
147244: move     b,l:?:>$40                                  ; 497000 000040
147246: move     #>$4074,y0                                  ; 46F400 004074
147248: cmp      y0,a                                        ; 200055
147249: tfr      y0,a            ifge                        ; 202151
14724A: clb      a,b                                         ; 0C1E01
14724B: normf    b1,a                                        ; 0C1E26
14724C: move     a,x0                                        ; 21C400
14724D: move     #>$2b1100,a                                 ; 56F400 2B1100
14724F: andi     #$fe,ccr                                    ; 00FEB9
147250: rep      #<$18                                       ; 0618A0
147251: div      x0,a                                        ; 018040
147252: move     a0,a                                        ; 210E00
147253: add      #<$1d,b                                     ; 015D88
147254: normf    b1,a                                        ; 0C1E26
147255: move     a,l:<$0                                     ; 480000
147256: move     y:(r6+$9),a                                 ; 0226FE
147257: asr      #$9,a,a                                     ; 0C1C12
147258: move     y:(r6+$34),x0                               ; 02D6B4
147259: add      x0,a                                        ; 200040
14725A: move     y:(r6+$8),b                                 ; 0226BF
14725B: move     a1,y:(r6+$34)                               ; 02D6AC
14725C: move     a1,y0                                       ; 218600
14725D: add      y0,b                                        ; 200058
14725E: move     #$80,x1                                     ; 258000
14725F: asr      b                                           ; 20002A
147260: cmp      #>$400000,b                                 ; 0140CD 400000
147262: add      x1,b            ifge                        ; 202168
147263: asl      b                                           ; 20003A
147264: neg      b               ifmi                        ; 202B3E
147265: move     b,y:(r6+$26)                                ; 029EAF
147266: move     l:<$0,a                                     ; 488000
147267: move     #>$147ae,b                                  ; 57F400 0147AE
147269: move     y:(r6+$26),y0                               ; 029EB6
14726A: maci     #>$7d70a4,y0,b                              ; 0141DA 7D70A4
14726C: move     a1,x1                                       ; 218500
14726D: move     a0,x0                                       ; 210400
14726E: tfr      a,b             b,y0                        ; 21E609
14726F: move     y0,x:$1                                     ; 460100
147270: mpysu    y0,x0,a                                     ; 012785
147271: dmac     ss y0,x1,a                                  ; 01248E
147272: sub      a,b                                         ; 20001C
147273: move     a1,x:(r6+$27)                               ; 029ECC
147274: move     a0,y:(r6+$27)                               ; 029EE8
147275: move     b1,x:(r6+$2a)                               ; 02AE8D
147276: move     b0,y:(r6+$2a)                               ; 02AEA9
147277: move     y:(r6+$5),y1                                ; 0216F7
147278: mpysu    -y1,x0,b                                    ; 0127BC
147279: dmac     ss -y1,x1,b                                 ; 0124B7
14727A: asr      #$6,b,b                                     ; 0C1C8D
14727B: add      x,b                                         ; 200028
14727C: clr      b               ifmi                        ; 202B1B
14727D: mpysu    y1,x0,a                                     ; 01278C
14727E: dmac     ss y1,x1,a                                  ; 012487
14727F: asr      #$6,a,a                                     ; 0C1C0C
147280: add      x,a                                         ; 200020
147281: move     b,l:<$0                                     ; 490000
147282: move     x:$1,y0                                     ; 468100
147283: move     a1,x1                                       ; 218500
147284: move     a0,x0                                       ; 210400
147285: tfr      a,b                                         ; 200009
147286: mpysu    y0,x0,a                                     ; 012785
147287: dmac     ss y0,x1,a                                  ; 01248E
147288: sub      a,b                                         ; 20001C
147289: move     a1,x:(r6+$28)                               ; 02A68C
14728A: move     a0,y:(r6+$28)                               ; 02A6A8
14728B: move     b1,x:(r6+$2b)                               ; 02AECD
14728C: move     b0,y:(r6+$2b)                               ; 02AEE9
14728D: move     l:<$0,x                                     ; 428000
14728E: move     x:$1,y0                                     ; 468100
14728F: move     l:<$0,b                                     ; 498000
147290: mpysu    y0,x0,a                                     ; 012785
147291: dmac     ss y0,x1,a                                  ; 01248E
147292: sub      a,b                                         ; 20001C
147293: move     a1,x:(r6+$29)                               ; 02A6CC
147294: move     a0,y:(r6+$29)                               ; 02A6E8
147295: move     b1,x:(r6+$2c)                               ; 02B68D
147296: move     b0,y:(r6+$2c)                               ; 02B6A9
147297: move     #>$19999a,x0                                ; 44F400 19999A
147299: move     x0,x:(r6-$1d)                               ; 038EC4
14729A: move     y:(r6+$6),y1                                ; 021EB7
14729B: mpyi     #>$200000,y1,b                              ; 0141F8 200000
14729D: move     y:(r6+$7),y1                                ; 021EF7
14729E: mpyi     #>$200000,y1,a                              ; 0141F0 200000
1472A0: move     a,y:(r6+$32)                                ; 02CEAE
1472A1: move     b,y:(r6+$2f)                                ; 02BEEF
1472A2: move     #$80,r0                                     ; 308000
1472A3: move     #$1,m0                                      ; 0501A0
1472A4: move     x:(r6+$27),x1                               ; 029ED5
1472A5: move     y:(r6+$27),x0                               ; 029EF4
1472A6: move     x,l:(r0)+                                   ; 425800
1472A7: move     x:(r6+$2a),x1                               ; 02AE95
1472A8: move     y:(r6+$2a),x0                               ; 02AEB4
1472A9: move     x,l:(r0)                                    ; 426000
1472AA: move     y:(r6+$1d),r0                               ; 0B76D0 00001D
1472AC: move     x:(r6+$16),b                                ; 025E9F
1472AD: move     y:(r6+$16),b0                               ; 025EB9
1472AE: move     #$0,r1                                      ; 310000
1472AF: move     l:(r0)+,a                                   ; 48D800
1472B0: move     l:(r0)+,a                                   ; 48D800
1472B1: cmp      a,b                                         ; 20000D
1472B2: tfr      a,b             ifgt                        ; 202709
1472B3: jsr      func_0003d3                                 ; 0D03D3
1472B4: move     r0,y:(r6+$1d)                               ; 0B7690 00001D
1472B6: move     b1,x:(r6+$16)                               ; 025E8D
1472B7: move     b0,y:(r6+$16)                               ; 025EA9
1472B8: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1472BA: move     #$80,r7                                     ; 378000
1472BB: move     #$1,m7                                      ; 0501A7
1472BC: move     #>$40,x0                                    ; 44F400 000040
1472BE: move     #>$70a3d,y0                                 ; 46F400 070A3D
1472C0: move     y0,y:(r7)+                                  ; 4E5F00
1472C1: move     #>$f8f5c3,y0                                ; 46F400 F8F5C3
1472C3: move     y:(r6+$19),a                                ; 0266FE
1472C4: move     y0,y:(r7)                                   ; 4E6700
1472C5: move     r6,r0                                       ; 22D000
1472C6: move     r6,r3                                       ; 22D300
1472C7: move     a,n0                                        ; 21D800
1472C8: add      #<$10,a                                     ; 015080
1472C9: cmp      #>$98,a                                     ; 0140C5 000098
1472CB: sub      x0,a            ifeq                        ; 202A44
1472CC: move     y:(r6+$1a),r7                               ; 0B76D7 00001A
1472CE: move     a,y:(r6+$19)                                ; 0266EE
1472CF: move     a,n3                                        ; 21DB00
1472D0: move     (r0)+n0                                     ; 204800
1472D1: move     #$0,x1                                      ; 250000
1472D2: rep      #<$10                                       ; 0610A0
1472D3: move     x1,x:(r0)+                                  ; 455800
1472D4: move     (r3)+n3                                     ; 204B00
1472D5: jsr      func_0003aa                                 ; 0D03AA
1472D6: move     r7,y:(r6+$1a)                               ; 0B7697 00001A
1472D8: move     #$80,r0                                     ; 308000
1472D9: move     #$1,m0                                      ; 0501A0
1472DA: move     x:(r6+$28),x1                               ; 02A695
1472DB: move     y:(r6+$28),x0                               ; 02A6B4
1472DC: move     x,l:(r0)+                                   ; 425800
1472DD: move     x:(r6+$2b),x1                               ; 02AED5
1472DE: move     y:(r6+$2b),x0                               ; 02AEF4
1472DF: move     x,l:(r0)                                    ; 426000
1472E0: move     y:(r6+$1e),r0                               ; 0B76D0 00001E
1472E2: move     x:(r6+$17),b                                ; 025EDF
1472E3: move     y:(r6+$17),b0                               ; 025EF9
1472E4: move     #$0,r1                                      ; 310000
1472E5: move     l:(r0)+,a                                   ; 48D800
1472E6: move     l:(r0)+,a                                   ; 48D800
1472E7: cmp      a,b                                         ; 20000D
1472E8: tfr      a,b             ifgt                        ; 202709
1472E9: jsr      func_0003d3                                 ; 0D03D3
1472EA: move     r0,y:(r6+$1e)                               ; 0B7690 00001E
1472EC: move     b1,x:(r6+$17)                               ; 025ECD
1472ED: move     b0,y:(r6+$17)                               ; 025EE9
1472EE: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1472F0: move     #$80,r7                                     ; 378000
1472F1: move     #$1,m7                                      ; 0501A7
1472F2: move     y:(r6+$4),x0                                ; 0216B4
1472F3: move     #>$70a3d,y0                                 ; 46F400 070A3D
1472F5: mpy      y0,x0,a                                     ; 2000D0
1472F6: move     a,y:(r7)+                                   ; 5E5F00
1472F7: neg      a                                           ; 200036
1472F8: move     a,y:(r7)                                    ; 5E6700
1472F9: move     y:(r6+$1b),r7                               ; 0B76D7 00001B
1472FB: jsr      func_0003aa                                 ; 0D03AA
1472FC: move     r7,y:(r6+$1b)                               ; 0B7697 00001B
1472FE: move     #$80,r0                                     ; 308000
1472FF: move     #$1,m0                                      ; 0501A0
147300: move     x:(r6+$29),x1                               ; 02A6D5
147301: move     y:(r6+$29),x0                               ; 02A6F4
147302: move     x,l:(r0)+                                   ; 425800
147303: move     x:(r6+$2c),x1                               ; 02B695
147304: move     y:(r6+$2c),x0                               ; 02B6B4
147305: move     x,l:(r0)                                    ; 426000
147306: move     y:(r6+$1f),r0                               ; 0B76D0 00001F
147308: move     x:(r6+$18),b                                ; 02669F
147309: move     y:(r6+$18),b0                               ; 0266B9
14730A: move     #$0,r1                                      ; 310000
14730B: move     l:(r0)+,a                                   ; 48D800
14730C: move     l:(r0)+,a                                   ; 48D800
14730D: cmp      a,b                                         ; 20000D
14730E: tfr      a,b             ifgt                        ; 202709
14730F: jsr      func_0003d3                                 ; 0D03D3
147310: move     r0,y:(r6+$1f)                               ; 0B7690 00001F
147312: move     b1,x:(r6+$18)                               ; 02668D
147313: move     b0,y:(r6+$18)                               ; 0266A9
147314: move     #>$ffffff,m0                                ; 05F420 FFFFFF
147316: move     #$80,r7                                     ; 378000
147317: move     #$1,m7                                      ; 0501A7
147318: move     y:(r6+$4),x0                                ; 0216B4
147319: move     #>$70a3d,y0                                 ; 46F400 070A3D
14731B: mpy      y0,x0,a                                     ; 2000D0
14731C: move     a,y:(r7)+                                   ; 5E5F00
14731D: neg      a                                           ; 200036
14731E: move     a,y:(r7)                                    ; 5E6700
14731F: move     y:(r6+$1c),r7                               ; 0B76D7 00001C
147321: jsr      func_0003aa                                 ; 0D03AA
147322: move     r7,y:(r6+$1c)                               ; 0B7697 00001C
147324: move     #>$ffffff,m7                                ; 05F427 FFFFFF
147326: move     #>$333333,x1                                ; 45F400 333333
147328: move     #>$54,y1                                    ; 47F400 000054
14732A: move     r3,r0                                       ; 227000
14732B: move     #$0,r1                                      ; 310000
14732C: move     #>$ffffff,m0                                ; 05F420 FFFFFF
14732E: move     m0,m1                                       ; 0461A0
14732F: move     x:(r6+$25),a                                ; 0296DE
147330: move     x:(r0)+,x0                                  ; 44D800
147331: do       #<$10,>$147335                              ; 061080 147334
147333: mac      x1,x0,a         a,x:(r1)+       a,y0        ; 1819A2
147334: mac      -y1,y0,a        x:(r0)+,x0                  ; 44D8B6
147335: move     a,x:(r6+$25)                                ; 0296CE
147336: move     x:(r6-$27),x1                               ; 0366D5
147337: mpyi     #>$80000,x1,a                               ; 0141E0 080000
147339: add      #>$ffff80,a                                 ; 0140C0 FFFF80
14733B: clr      a               ifmi                        ; 202B13
14733C: move     #>$63f,x0                                   ; 44F400 00063F
14733E: cmp      x0,a                                        ; 200045
14733F: tfr      x0,a            ifgt                        ; 202741
147340: move     x:(r6+$33),r2                               ; 0A76D2 000033
147342: move     a,x:(r6+$33)                                ; 02CECE
147343: move     r2,x:(r6-$1a)                               ; 0A7692 FFFFE6
147345: move     #>$7fffff,a                                 ; 56F400 7FFFFF
147347: move     x:(r2+$1435c6),x1                           ; 0A72C5 1435C6
147349: sub      x1,a            a,x0                        ; 21C464
14734A: add      x0,a                                        ; 200040
14734B: asr      a                                           ; 200022
14734C: move     #$0,r0                                      ; 300000
14734D: move     r0,r1                                       ; 221100
14734E: move     a,y1                                        ; 21C700
14734F: move     x:(r6+$23),a                                ; 028EDE
147350: move     y:(r6+$24),x0                               ; 0296B4
147351: asl      #$2,a,b                                     ; 0C1D05
147352: do       #<$10,>$147358                              ; 061080 147357
147354: mac      -x0,y1,a        x:(r0)+,x0      a,y0        ; 1098C6
147355: mac      x0,y1,a                                     ; 2000C2
147356: mac      -x1,y0,a        b,x:(r1)+                   ; 5759E6
147357: asl      #$2,a,b                                     ; 0C1D05
147358: move     x0,y:(r6+$24)                               ; 0296A4
147359: move     a,x:(r6+$23)                                ; 028ECE
14735A: lua      (r6+$2d),r5                                 ; 0416D5
14735B: do       #<$2,>$147396                               ; 060280 147395
14735D: move     l:?:>$40,a                                  ; 48F000 000040
14735F: asr      a                                           ; 200022
147360: move     l:(r5),b                                    ; 49E500
147361: move     a,l:?:>$40                                  ; 487000 000040
147363: move     a,l:(r5)+                                   ; 485D00
147364: sub      b,a                                         ; 200014
147365: asr      #$4,a,a                                     ; 0C1C08
147366: move     #$10,r1                                     ; 311000
147367: move     a1,y1                                       ; 218700
147368: move     a0,y0                                       ; 210600
147369: tfr      b,a                                         ; 200001
14736A: move     l:(r5),b                                    ; 49E500
14736B: move     #>$1fff,x0                                  ; 44F400 001FFF
14736D: move     #>$14a000,x1                                ; 45F400 14A000
14736F: do       #<$10,>$147375                              ; 061080 147374
147371: add      y,a                                         ; 200030
147372: add      a,b             b,l:(r1)+                   ; 495918
147373: and      x0,b                                        ; 20004E
147374: add      x1,b                                        ; 200068
147375: move     #$10,r1                                     ; 311000
147376: move     r1,r4                                       ; 223400
147377: move     b1,x:(r5)                                   ; 556500
147378: move     b0,y:(r5)+                                  ; 595D00
147379: move     x:(r1)+,r2                                  ; 62D900
14737A: move     #>$1fff,m2                                  ; 05F422 001FFF
14737C: move     m2,m0                                       ; 0460A2
14737D: move     #$0,r3                                      ; 330000
14737E: move     y:(r5)+,y1                                  ; 4FDD00
14737F: move     y:(r4)+,y0                                  ; 4EDC00
147380: move     x:(r2)+,x1                                  ; 45DA00
147381: move     x:(r1)+,r0                                  ; 60D900
147382: do       #<$8,>$147392                               ; 060880 147391
147384: mpysu    -x1,y0,a                                    ; 012796
147385: add      x1,a            x:(r2),x0                   ; 44E260
147386: add      x1,a            x:(r1)+,r2                  ; 62D960
147387: macsu    x0,y0,a                                     ; 01268D
147388: asr      a               x:(r0)+,x1      y:(r4)+,y0  ; F49822
147389: mpysu    -x1,y0,b                                    ; 0127B6
14738A: add      x1,b            x:(r0),x0                   ; 44E068
14738B: add      x1,b            x:(r1)+,r0                  ; 60D968
14738C: macsu    x0,y0,b                                     ; 0126AD
14738D: asr      b               x:(r3)+,a       a,y0        ; 189B2A
14738E: mac      y1,y0,a         x:(r3)-,b       b,y0        ; 1E93B2
14738F: mac      x0,y1,b         x:(r2)+,x1      y:(r4)+,y0  ; F49ACA
147390: move     a,x:(r3)+                                   ; 565B00
147391: move     b,x:(r3)+                                   ; 575B00
147392: move     #>$ffffff,m0                                ; 05F420 FFFFFF
147394: move     #>$ffffff,m2                                ; 05F422 FFFFFF
147396: move     #>$100,r7                                   ; 67F400 000100
147398: move     #$0,r0                                      ; 300000
147399: move     x:(r0)+,a                                   ; 56D800
14739A: move     x:(r0)+,b                                   ; 57D800
14739B: do       #<$8,>$1473a1                               ; 060880 1473A0
14739D: move     a,y:(r7)+                                   ; 5E5F00
14739E: move     x:(r0)+,a       a,y:(r7)+                   ; BAF800
14739F: move     b,y:(r7)+                                   ; 5F5F00
1473A0: move     x:(r0)+,b       b,y:(r7)+                   ; BFF800
1473A1: rts                                                  ; 00000C
1473A2: move     #$0,x0                                      ; 240000
1473A3: move     #$10,x1                                     ; 251000
1473A4: move     x0,y:(r6+$10)                               ; 0246A4
1473A5: move     x0,y:(r6+$11)                               ; 0246E4
1473A6: move     x0,y:(r6+$17)                               ; 025EE4
1473A7: move     x0,y:(r6+$18)                               ; 0266A4
1473A8: move     x1,x:(r6-$1d)                               ; 038EC5
1473A9: rts                                                  ; 00000C
1473AA: move     #$0,x0                                      ; 240000
1473AB: move     x0,y:(r6+$19)                               ; 0266E4
1473AC: move     y:(r6+$7),a                                 ; 021EFE
1473AD: cmp      #>$400000,a                                 ; 0140C5 400000
1473AF: bcs      func_1473b3                                 ; 0D1048 000004
1473B1: move     x0,y:(r6+$17)                               ; 025EE4
1473B2: move     x0,y:(r6+$18)                               ; 0266A4
1473B3: rts                                                  ; 00000C
1473B4: nop                                                  ; 000000
1473B5: rts                                                  ; 00000C
1473B6: nop                                                  ; 000000
1473B7: rts                                                  ; 00000C
1473B8: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1473BA: move     #>$ffffff,m1                                ; 05F421 FFFFFF
1473BC: move     #>$ffffff,m2                                ; 05F422 FFFFFF
1473BE: move     #>$ffffff,m3                                ; 05F423 FFFFFF
1473C0: move     #>$ffffff,m4                                ; 05F424 FFFFFF
1473C2: move     #>$ffffff,m5                                ; 05F425 FFFFFF
1473C4: move     #>$ffffff,m6                                ; 05F426 FFFFFF
1473C6: move     #>$ffffff,m7                                ; 05F427 FFFFFF
1473C8: move     #>$2f8e,x0                                  ; 44F400 002F8E
1473CA: move     a1,y1                                       ; 218700
1473CB: move     a0,y0                                       ; 210600
1473CC: mpysu    x0,y0,a                                     ; 01278D
1473CD: dmac     ss x0,y1,a                                  ; 012484
1473CE: nop                                                  ; 000000
1473CF: move     a1,y:(r6+$12)                               ; 024EAC
1473D0: move     a0,y:(r6+$13)                               ; 024EE8
1473D1: move     y:(r6+$8),a                                 ; 0226BE
1473D2: cmp      #>$2aaaaa,a                                 ; 0140C5 2AAAAA
1473D4: blt      func_14740f                                 ; 0D1049 00003B
1473D6: cmp      #>$555555,a                                 ; 0140C5 555555
1473D8: blt      func_1473e1                                 ; 0D1049 000009
1473DA: move     x:(r6-$1),a                                 ; 03FEDE
1473DB: tst      a                                           ; 200003
1473DC: bne      func_1473eb                                 ; 0D1042 00000F
1473DE: move     y:(r6-$1),a                                 ; 03FEFE
1473DF: bra      func_1473eb                                 ; 0D10C0 00000C
1473E1: move     y:(r6+$9),a                                 ; 0226FE
1473E2: tst      a                                           ; 200003
1473E3: beq      func_14740f                                 ; 0D104A 00002C
1473E5: move     a,x0                                        ; 21C400
1473E6: move     #>$1a00,x1                                  ; 45F400 001A00
1473E8: mpy      x1,x0,a                                     ; 2000A0
1473E9: add      #>$1000,a                                   ; 0140C0 001000
1473EB: asr      #$b,a,b                                     ; 0C1C17
1473EC: and      #>$7ff,a                                    ; 0140C6 0007FF
1473EE: move     a,r2                                        ; 21D200
1473EF: move     b,x0                                        ; 21E400
1473F0: move     x:(r2+$140000),a                            ; 0A72CE 140000
1473F2: asr      #$a,a,a                                     ; 0C1C14
1473F3: asl      x0,a,a                                      ; 0C1E48
1473F4: move     #>$20b40,y0                                 ; 46F400 020B40
1473F6: move     a,x0                                        ; 21C400
1473F7: mpy      y0,x0,a                                     ; 2000D0
1473F8: asl      #$c,a,a                                     ; 0C1D18
1473F9: move     a,x0                                        ; 21C400
1473FA: move     #>$ac44,a                                   ; 56F400 00AC44
1473FC: andi     #$fe,ccr                                    ; 00FEB9
1473FD: rep      #<$11                                       ; 0611A0
1473FE: div      x0,a                                        ; 018040
1473FF: move     #$0,a1                                      ; 2C0000
147400: asr      #$4,a,a                                     ; 0C1C08
147401: move     a0,y1                                       ; 210700
147402: clr      b                                           ; 20001B
147403: move     y:(r6+$10),b0                               ; 0246B9
147404: move     y:(r6+$11),b1                               ; 0246FD
147405: move     #$0,y0                                      ; 260000
147406: move     y:(r6+$19),a                                ; 0266FE
147407: add      #>$1,a                                      ; 0140C0 000001
147409: cmp      y1,a                                        ; 200075
14740A: tfr      y0,a            ifge                        ; 202151
14740B: tfr      y0,b            ifge                        ; 202159
14740C: move     a,y:(r6+$19)                                ; 0266EE
14740D: move     b0,y:(r6+$10)                               ; 0246A9
14740E: move     b1,y:(r6+$11)                               ; 0246ED
14740F: clr      a                                           ; 200013
147410: move     y:(r6+$17),a1                               ; 025EFC
147411: move     y:(r6+$6),b                                 ; 021EBF
147412: lsr      #$a,b                                       ; 0C1ED5
147413: add      b,a                                         ; 200010
147414: move     a1,y:(r6+$17)                               ; 025EEC
147415: move     y:(r6+$5),a                                 ; 0216FE
147416: clr      b                                           ; 20001B
147417: move     y:(r6+$17),b1                               ; 025EFD
147418: add      a,b                                         ; 200018
147419: cmp      #>$7fffff,b                                 ; 0140CD 7FFFFF
14741B: ble      func_147424                                 ; 0D104F 000009
14741D: clr      a                                           ; 200013
14741E: move     #>$ffffff,a1                                ; 54F400 FFFFFF
147420: sub      b,a                                         ; 200014
147421: tst      a                                           ; 200003
147422: neg      a               ifmi                        ; 202B36
147423: tfr      a,b                                         ; 200009
147424: move     b,x0                                        ; 21E400
147425: mpyi     #>$200,x0,b                                 ; 0141C8 000200
147427: add      #>$200,b                                    ; 0140C8 000200
147429: move     b1,y:(r6+$1f)                               ; 027EED
14742A: move     b0,y:(r6+$20)                               ; 0286A9
14742B: lsr      b                                           ; 20002B
14742C: move     b1,y:(r6+$21)                               ; 0286ED
14742D: move     y:(r6+$4),a                                 ; 0216BE
14742E: move     a,x0                                        ; 21C400
14742F: move     #>$20,x1                                    ; 45F400 000020
147431: mpy      x1,x0,a                                     ; 2000A0
147432: move     a,x0                                        ; 21C400
147433: move     #>$80,x1                                    ; 45F400 000080
147435: move     #>$101d7b,a0                                ; 50F400 101D7B
147437: mac      x1,x0,a                                     ; 2000A2
147438: move     a0,y:(r6+$14)                               ; 0256A8
147439: clr      b                                           ; 20001B
14743A: move     y:(r6+$10),b1                               ; 0246BD
14743B: move     y:(r6+$11),b0                               ; 0246F9
14743C: clr      a                                           ; 200013
14743D: move     y:(r6+$12),a1                               ; 024EBC
14743E: move     y:(r6+$13),a0                               ; 024EF8
14743F: move     #>$0,r0                                     ; 60F400 000000
147441: move     y:(r6+$1f),x0                               ; 027EF4
147442: move     #>$200,y0                                   ; 46F400 000200
147444: do       #<$40,>$14744b                              ; 064080 14744A
147446: move     b1,x:(r0)+                                  ; 555800
147447: move     b0,x:(r0)+                                  ; 515800
147448: add      a,b                                         ; 200018
147449: cmp      x0,b                                        ; 20004D
14744A: sub      y0,b            ifge                        ; 20215C
14744B: move     b1,y:(r6+$10)                               ; 0246AD
14744C: move     b0,y:(r6+$11)                               ; 0246E9
14744D: move     y:(r6+$14),r0                               ; 0B76D0 000014
14744F: move     r0,r3                                       ; 221300
147450: move     #>$0,r1                                     ; 61F400 000000
147452: move     #>$1473b4,r2                                ; 62F400 1473B4
147454: move     #>$1473b5,r5                                ; 65F400 1473B5
147456: move     #>$0,r4                                     ; 64F400 000000
147458: do       #<$40,>$147476                              ; 064080 147475
14745A: clr      a               x:(r4)+,b                   ; 57DC13
14745B: lsr      b                                           ; 20002B
14745C: rol      a                                           ; 200037
14745D: move     b,n0                                        ; 21F800
14745E: move     a1,n2                                       ; 219A00
14745F: move     a1,n5                                       ; 219D00
147460: move     y:(r6+$21),x1                               ; 0286F5
147461: move     #>$100,y1                                   ; 47F400 000100
147463: move     x:(r2+n2),x0                                ; 44EA00
147464: move     y:(r5+n5),y0                                ; 4EED00
147465: add      a,b                                         ; 200018
147466: cmp      x1,b                                        ; 20006D
147467: sub      y1,b            ifge                        ; 20217C
147468: move     x:(r4)+,x1                                  ; 45DC00
147469: move     b,n3                                        ; 21FB00
14746A: move     y:(r0+n0),a                                 ; 5EE800
14746B: lsl      x0,a                                        ; 0C1E18
14746C: move     #>$ffffff,b                                 ; 57F400 FFFFFF
14746E: move     a1,x0                                       ; 218400
14746F: sub      x1,b            y:(r3+n3),a                 ; 5EEB6C
147470: lsl      y0,a                                        ; 0C1E1A
147471: move     b,y1                                        ; 21E700
147472: move     a1,y0                                       ; 218600
147473: mpysu    x0,y1,a                                     ; 012784
147474: macsu    y0,x1,a                                     ; 01268E
147475: move     a,y:(r1)+                                   ; 5E5900
147476: move     #>$0,r4                                     ; 64F400 000000
147478: move     #>$f0,r5                                    ; 65F400 0000F0
14747A: move     #$9,m5                                      ; 0509A5
14747B: move     #$2,n5                                      ; 3D0200
14747C: move     #>$f0,r1                                    ; 61F400 0000F0
14747E: move     #>$f2,r2                                    ; 62F400 0000F2
147480: move     #>$f4,r3                                    ; 63F400 0000F4
147482: move     y:(r6+$28),a                                ; 02A6BE
147483: move     y:(r6+$29),b                                ; 02A6FF
147484: move     a,x:(r1+$0)                                 ; 02018E
147485: move     b,x:(r1+$1)                                 ; 0201CF
147486: move     y:(r6+$2a),a                                ; 02AEBE
147487: move     y:(r6+$2b),b                                ; 02AEFF
147488: move     a,x:(r2+$0)                                 ; 02028E
147489: move     y:(r6+$2c),a                                ; 02B6BE
14748A: move     b,x:(r2+$1)                                 ; 0202CF
14748B: move     a,x:(r3+$0)                                 ; 02038E
14748C: move     #>$3ce85f,y0                                ; 46F400 3CE85F
14748E: move     y0,y:(r5)+                                  ; 4E5D00
14748F: move     #>$f9f156,y0                                ; 46F400 F9F156
147491: move     y0,y:(r5)+                                  ; 4E5D00
147492: move     #>$939958,y0                                ; 46F400 939958
147494: move     y0,y:(r5)+                                  ; 4E5D00
147495: move     #>$4d33e,y0                                 ; 46F400 04D33E
147497: move     y0,y:(r5)+                                  ; 4E5D00
147498: move     #>$36f789,y0                                ; 46F400 36F789
14749A: move     y0,y:(r5)+                                  ; 4E5D00
14749B: move     #>$11b451,y0                                ; 46F400 11B451
14749D: move     y0,y:(r5)+                                  ; 4E5D00
14749E: move     #>$9023ec,y0                                ; 46F400 9023EC
1474A0: move     y0,y:(r5)+                                  ; 4E5D00
1474A1: move     #>$ee4baf,y0                                ; 46F400 EE4BAF
1474A3: move     y0,y:(r5)+                                  ; 4E5D00
1474A4: move     #>$c652ad,y0                                ; 46F400 C652AD
1474A6: move     y0,y:(r5)+                                  ; 4E5D00
1474A7: move     #>$d1e2d,y0                                 ; 46F400 0D1E2D
1474A9: move     y0,y:(r5)+                                  ; 4E5D00
1474AA: move     #>$6186,x1                                  ; 45F400 006186
1474AC: do       #<$10,>$1474de                              ; 061080 1474DD
1474AE: move     x:(r1)+,x0      y:(r5)+n5,y0                ; D0B900
1474AF: mpy      -y0,x0,a        x:(r1)-,x0      y:(r5)+n5,y0 ; D0B1D4
1474B0: mac      -y0,x0,a        x0,x:(r1)+      y:(r4)+,y1  ; F119D6
1474B1: asl      a                                           ; 200032
1474B2: macr     y1,x1,a         x:(r2)+,x0      y:(r5)+n5,y0 ; D0BAF3
1474B3: do       #<$2,>$1474c1                               ; 060280 1474C0
1474B5: mpy      -y0,x0,b        x:(r2)-,x0      y:(r5)+n5,y0 ; D0B2DC
1474B6: mac      -y0,x0,b        x0,x:(r2)+                  ; 445ADE
1474B7: asl      b               a,x:(r1)-                   ; 56513A
1474B8: macr     y1,x1,b         x:(r3),x0       y:(r5)+n5,y0 ; D0A3FB
1474B9: nop                                                  ; 000000
1474BA: mpy      -y0,x0,b        b,x:(r2)-                   ; 5752DC
1474BB: asl      b                                           ; 20003A
1474BC: macr     y1,x1,b         x:(r1)+,x0      y:(r5)+n5,y0 ; D0B9FB
1474BD: mpy      -y0,x0,a        x:(r1)-,x0      y:(r5)+n5,y0 ; D0B1D4
1474BE: mac      -y0,x0,a        x0,x:(r1)+      y:(r4)+,y1  ; F119D6
1474BF: asl      a               b,x:(r3)                    ; 576332
1474C0: macr     y1,x1,a         x:(r2)+,x0      y:(r5)+n5,y0 ; D0BAF3
1474C1: mpy      -y0,x0,b        x:(r2)-,x0      y:(r5)+n5,y0 ; D0B2DC
1474C2: mac      -y0,x0,b        x0,x:(r2)+                  ; 445ADE
1474C3: asl      b               a,x:(r1)-                   ; 56513A
1474C4: macr     y1,x1,b         x:(r3),x0       y:(r5)+n5,y0 ; D0A3FB
1474C5: nop                                                  ; 000000
1474C6: mpy      -y0,x0,b        b,x:(r2)-                   ; 5752DC
1474C7: asl      b                                           ; 20003A
1474C8: macr     y1,x1,b         x:(r1)+,x0      y:(r5)+,y0  ; F0B9FB
1474C9: nop                                                  ; 000000
1474CA: mpy      -y0,x0,a        b,x:(r3)        y:(r5)+,y0  ; FC23D4
1474CB: mpy      y0,x0,b         x:(r1)-,x0      y:(r5)+,y0  ; F0B1D8
1474CC: mac      -y0,x0,a        x0,x:(r1)+      y:(r5)+,y0  ; F039D6
1474CD: asl      a               y:(r4)+,y1                  ; 4FDC32
1474CE: macr     y1,x1,a                                     ; 2000F3
1474CF: mac      y0,x0,b         x:(r2)+,x0      y:(r5)+,y0  ; F0BADA
1474D0: mpy      -y0,x0,a        a,x:(r1)-       y:(r5)+,y0  ; F831D4
1474D1: mac      y0,x0,b         x:(r2)-,x0      y:(r5)+,y0  ; F0B2DA
1474D2: mac      -y0,x0,a        x0,x:(r2)+      y:(r5)+,y0  ; F03AD6
1474D3: asl      a                                           ; 200032
1474D4: macr     y1,x1,a                                     ; 2000F3
1474D5: mac      y0,x0,b         x:(r3),x0       y:(r5)+,y0  ; F0A3DA
1474D6: mpy      -y0,x0,a        a,x:(r2)-       y:(r5)+,y0  ; F832D4
1474D7: asl      a                                           ; 200032
1474D8: macr     y1,x1,a                                     ; 2000F3
1474D9: mac      y0,x0,b                                     ; 2000DA
1474DA: asl      #$5,b,b                                     ; 0C1D8B
1474DB: nop                                                  ; 000000
1474DC: move     a,x:(r3)        b,y:(r7)+                   ; BB6300
1474DD: move     b,y:(r7)+                                   ; 5F5F00
1474DE: move     x:(r1+$0),a                                 ; 02019E
1474DF: move     x:(r1+$1),b                                 ; 0201DF
1474E0: move     a,y:(r6+$28)                                ; 02A6AE
1474E1: move     b,y:(r6+$29)                                ; 02A6EF
1474E2: move     x:(r2+$0),a                                 ; 02029E
1474E3: move     x:(r2+$1),b                                 ; 0202DF
1474E4: move     a,y:(r6+$2a)                                ; 02AEAE
1474E5: move     x:(r3+$0),a                                 ; 02039E
1474E6: move     b,y:(r6+$2b)                                ; 02AEEF
1474E7: move     a,y:(r6+$2c)                                ; 02B6AE
1474E8: rts                                                  ; 00000C
1474E9: move     #$0,x0                                      ; 240000
1474EA: move     x0,y:(r6+$10)                               ; 0246A4
1474EB: move     x0,y:(r6+$11)                               ; 0246E4
1474EC: rts                                                  ; 00000C
1474ED: move     y:(r6+$1),a                                 ; 0206FE
1474EE: add      #>$55,a                                     ; 0140C0 000055
1474F0: move     a,x0                                        ; 21C400
1474F1: mpyi     #>$c000,x0,a                                ; 0141C0 00C000
1474F3: move     a,x0                                        ; 21C400
1474F4: mpyi     #>$55559,x0,a                               ; 0141C0 055559
1474F6: move     a,x1                                        ; 21C500
1474F7: move     a,y:(r6+$1b)                                ; 026EEE
1474F8: mpyi     #>$c,x1,a                                   ; 0141E0 00000C
1474FA: move     a0,x1                                       ; 210500
1474FB: move     x0,a                                        ; 208E00
1474FC: sub      x1,a                                        ; 200064
1474FD: move     a1,n0                                       ; 219800
1474FE: move     a1,n1                                       ; 219900
1474FF: move     #>$147632,r0                                ; 60F400 147632
147501: move     #>$147633,r1                                ; 61F400 147633
147503: move     y:(r0+n0),x0                                ; 4CE800
147504: move     y:(r1+n1),b                                 ; 5FE900
147505: sub      #>$1,b                                      ; 0140CC 000001
147507: sub      x0,b                                        ; 20004C
147508: move     y:(r6+$5),y0                                ; 0216F6
147509: move     b,y1                                        ; 21E700
14750A: mpy      y1,y0,a                                     ; 2000B0
14750B: lsr      a                                           ; 200023
14750C: move     a,y0                                        ; 21C600
14750D: add      x0,a                                        ; 200040
14750E: move     a,y:(r6+$14)                                ; 0256AE
14750F: sub      y0,b                                        ; 20005C
147510: move     b,y:(r6+$15)                                ; 0256EF
147511: move     #>$7fffff,x0                                ; 44F400 7FFFFF
147513: move     x0,y:(r6+$16)                               ; 025EA4
147514: move     #$0,x0                                      ; 240000
147515: move     x0,y:(r6+$10)                               ; 0246A4
147516: move     x0,y:(r6+$11)                               ; 0246E4
147517: move     y:(r6+$8),a                                 ; 0226BE
147518: add      #>$8000,a                                   ; 0140C0 008000
14751A: move     a,y0                                        ; 21C600
14751B: move     #>$7fffff,y1                                ; 47F400 7FFFFF
14751D: mpyi     #>$80,y0,a                                  ; 0141D0 000080
14751F: cmp      #>$7f,a                                     ; 0140C5 00007F
147521: tfr      y1,a            ifge                        ; 202171
147522: move     a,y:(r6+$19)                                ; 0266EE
147523: move     #>$7fffff,y0                                ; 46F400 7FFFFF
147525: move     y0,y:(r6+$1a)                               ; 026EA6
147526: rts                                                  ; 00000C
147527: nop                                                  ; 000000
147528: rts                                                  ; 00000C
147529: nop                                                  ; 000000
14752A: rts                                                  ; 00000C
14752B: move     #>$ffffff,m0                                ; 05F420 FFFFFF
14752D: move     #>$ffffff,m1                                ; 05F421 FFFFFF
14752F: move     #>$ffffff,m2                                ; 05F422 FFFFFF
147531: move     #>$ffffff,m3                                ; 05F423 FFFFFF
147533: move     #>$ffffff,m4                                ; 05F424 FFFFFF
147535: move     #>$ffffff,m5                                ; 05F425 FFFFFF
147537: move     #>$ffffff,m6                                ; 05F426 FFFFFF
147539: move     #>$ffffff,m7                                ; 05F427 FFFFFF
14753B: move     y:(r6+$1b),x0                               ; 026EF4
14753C: mpyi     #>$200,x0,a                                 ; 0141C0 000200
14753E: move     a0,a1                                       ; 210C00
14753F: move     y:(r6-$a),x0                                ; 03DEB4
147540: maci     #>$b000,x0,a                                ; 0141C2 00B000
147542: sub      #>$5800,a                                   ; 0140C4 005800
147544: move     y:(r6+$4),x0                                ; 0216B4
147545: move     #>$2000,x1                                  ; 45F400 002000
147547: mac      x1,x0,a                                     ; 2000A2
147548: add      #>$d00,a                                    ; 0140C0 000D00
14754A: asr      #$b,a,b                                     ; 0C1C17
14754B: and      #>$7ff,a                                    ; 0140C6 0007FF
14754D: move     a,r2                                        ; 21D200
14754E: move     b,x0                                        ; 21E400
14754F: move     x:(r2+$140000),a                            ; 0A72CE 140000
147551: asr      #$a,a,a                                     ; 0C1C14
147552: asl      x0,a,a                                      ; 0C1E48
147553: move     #>$1d22a,y0                                 ; 46F400 01D22A
147555: move     a,x0                                        ; 21C400
147556: mpy      y0,x0,a                                     ; 2000D0
147557: asr      #$3,a,a                                     ; 0C1C06
147558: move     #>$2f8e,x0                                  ; 44F400 002F8E
14755A: move     a1,y1                                       ; 218700
14755B: move     a0,y0                                       ; 210600
14755C: mpysu    x0,y0,a                                     ; 01278D
14755D: dmac     ss x0,y1,a                                  ; 012484
14755E: nop                                                  ; 000000
14755F: move     a1,y:(r6+$12)                               ; 024EAC
147560: move     a0,y:(r6+$13)                               ; 024EE8
147561: clr      b                                           ; 20001B
147562: move     y:(r6+$10),b1                               ; 0246BD
147563: move     y:(r6+$11),b0                               ; 0246F9
147564: move     y:(r6+$19),a                                ; 0266FE
147565: tst      a                                           ; 200003
147566: beq      func_147582                                 ; 0D104A 00001C
147568: move     y:(r6+$9),x0                                ; 0226F4
147569: move     #$6,x1                                      ; 250600
14756A: mpy      x1,x0,a                                     ; 2000A0
14756B: add      #>$c00,a                                    ; 0140C0 000C00
14756D: move     a1,x1                                       ; 218500
14756E: move     #>$7fffff,x0                                ; 44F400 7FFFFF
147570: move     #$0,y0                                      ; 260000
147571: move     y:(r6+$1a),a                                ; 026EBE
147572: sub      x1,a                                        ; 200064
147573: tfr      x0,a            ifmi                        ; 202B41
147574: tfr      y0,b ifmi                                   ; 202B59
147575: move     a,y:(r6+$1a)                                ; 026EAE
147576: bpl      func_147580                                 ; 0D1043 00000A
147578: move     y:(r6+$19),a                                ; 0266FE
147579: move     #>$1,x0                                     ; 44F400 000001
14757B: sub      x0,a                                        ; 200044
14757C: move     a,y:(r6+$19)                                ; 0266EE
14757D: move     #>$7fffff,x0                                ; 44F400 7FFFFF
14757F: move     x0,y:(r6+$16)                               ; 025EA4
147580: move     b0,y:(r6+$10)                               ; 0246A9
147581: move     b1,y:(r6+$11)                               ; 0246ED
147582: clr      a                                           ; 200013
147583: move     y:(r6+$12),a1                               ; 024EBC
147584: move     y:(r6+$13),a0                               ; 024EF8
147585: move     #>$0,r0                                     ; 60F400 000000
147587: do       #<$40,>$14758c                              ; 064080 14758B
147589: move     b1,x:(r0)+                                  ; 555800
14758A: move     b0,x:(r0)+                                  ; 515800
14758B: add      a,b                                         ; 200018
14758C: move     y:(r6+$16),a                                ; 025EBE
14758D: tst      a                                           ; 200003
14758E: beq      func_147592                                 ; 0D104A 000004
147590: move     b1,y:(r6+$10)                               ; 0246AD
147591: move     b0,y:(r6+$11)                               ; 0246E9
147592: move     y:(r6+$14),r0                               ; 0B76D0 000014
147594: move     r0,r3                                       ; 221300
147595: move     #>$0,r1                                     ; 61F400 000000
147597: move     #>$147527,r2                                ; 62F400 147527
147599: move     #>$147528,r5                                ; 65F400 147528
14759B: move     #>$0,r4                                     ; 64F400 000000
14759D: move     y:(r6+$16),x0                               ; 025EB4
14759E: do       #<$40,>$1475bf                              ; 064080 1475BE
1475A0: clr      a               x:(r4)+,b                   ; 57DC13
1475A1: move     y:(r6+$15),y0                               ; 0256F6
1475A2: lsr      b                                           ; 20002B
1475A3: cmp      y0,b                                        ; 20005D
1475A4: tfr      x0,a            iflt                        ; 202941
1475A5: move     a,y:(r6+$16)                                ; 025EAE
1475A6: clr      a                                           ; 200013
1475A7: rol      a                                           ; 200037
1475A8: move     b,n0                                        ; 21F800
1475A9: add      a,b                                         ; 200018
1475AA: move     a1,n2                                       ; 219A00
1475AB: move     a1,n5                                       ; 219D00
1475AC: move     b,n3                                        ; 21FB00
1475AD: move     x:(r2+n2),x0                                ; 44EA00
1475AE: move     y:(r5+n5),y0                                ; 4EED00
1475AF: move     y:(r0+n0),a                                 ; 5EE800
1475B0: lsl      x0,a                                        ; 0C1E18
1475B1: move     #>$ffffff,b                                 ; 57F400 FFFFFF
1475B3: move     a1,x0                                       ; 218400
1475B4: move     x:(r4)+,x1                                  ; 45DC00
1475B5: sub      x1,b            y:(r3+n3),a                 ; 5EEB6C
1475B6: lsl      y0,a                                        ; 0C1E1A
1475B7: move     b,y1                                        ; 21E700
1475B8: move     a1,y0                                       ; 218600
1475B9: mpysu    x0,y1,a                                     ; 012784
1475BA: macsu    y0,x1,a                                     ; 01268E
1475BB: move     y:(r6+$16),x0                               ; 025EB4
1475BC: move     a,x1                                        ; 21C500
1475BD: mpy      x1,x0,a                                     ; 2000A0
1475BE: move     a,y:(r1)+                                   ; 5E5900
1475BF: move     #>$0,r4                                     ; 64F400 000000
1475C1: move     #>$f0,r5                                    ; 65F400 0000F0
1475C3: move     #$9,m5                                      ; 0509A5
1475C4: move     #$2,n5                                      ; 3D0200
1475C5: move     #>$f0,r1                                    ; 61F400 0000F0
1475C7: move     #>$f2,r2                                    ; 62F400 0000F2
1475C9: move     #>$f4,r3                                    ; 63F400 0000F4
1475CB: move     y:(r6+$1e),a                                ; 027EBE
1475CC: move     y:(r6+$1f),b                                ; 027EFF
1475CD: move     a,x:(r1+$0)                                 ; 02018E
1475CE: move     b,x:(r1+$1)                                 ; 0201CF
1475CF: move     y:(r6+$20),a                                ; 0286BE
1475D0: move     y:(r6+$21),b                                ; 0286FF
1475D1: move     a,x:(r2+$0)                                 ; 02028E
1475D2: move     y:(r6+$22),a                                ; 028EBE
1475D3: move     b,x:(r2+$1)                                 ; 0202CF
1475D4: move     a,x:(r3+$0)                                 ; 02038E
1475D5: move     #>$3ce85f,y0                                ; 46F400 3CE85F
1475D7: move     y0,y:(r5)+                                  ; 4E5D00
1475D8: move     #>$f9f156,y0                                ; 46F400 F9F156
1475DA: move     y0,y:(r5)+                                  ; 4E5D00
1475DB: move     #>$939958,y0                                ; 46F400 939958
1475DD: move     y0,y:(r5)+                                  ; 4E5D00
1475DE: move     #>$4d33e,y0                                 ; 46F400 04D33E
1475E0: move     y0,y:(r5)+                                  ; 4E5D00
1475E1: move     #>$36f789,y0                                ; 46F400 36F789
1475E3: move     y0,y:(r5)+                                  ; 4E5D00
1475E4: move     #>$11b451,y0                                ; 46F400 11B451
1475E6: move     y0,y:(r5)+                                  ; 4E5D00
1475E7: move     #>$9023ec,y0                                ; 46F400 9023EC
1475E9: move     y0,y:(r5)+                                  ; 4E5D00
1475EA: move     #>$ee4baf,y0                                ; 46F400 EE4BAF
1475EC: move     y0,y:(r5)+                                  ; 4E5D00
1475ED: move     #>$c652ad,y0                                ; 46F400 C652AD
1475EF: move     y0,y:(r5)+                                  ; 4E5D00
1475F0: move     #>$d1e2d,y0                                 ; 46F400 0D1E2D
1475F2: move     y0,y:(r5)+                                  ; 4E5D00
1475F3: move     #>$6186,x1                                  ; 45F400 006186
1475F5: do       #<$10,>$147627                              ; 061080 147626
1475F7: move     x:(r1)+,x0      y:(r5)+n5,y0                ; D0B900
1475F8: mpy      -y0,x0,a        x:(r1)-,x0      y:(r5)+n5,y0 ; D0B1D4
1475F9: mac      -y0,x0,a        x0,x:(r1)+      y:(r4)+,y1  ; F119D6
1475FA: asl      a                                           ; 200032
1475FB: macr     y1,x1,a         x:(r2)+,x0      y:(r5)+n5,y0 ; D0BAF3
1475FC: do       #<$2,>$14760a                               ; 060280 147609
1475FE: mpy      -y0,x0,b        x:(r2)-,x0      y:(r5)+n5,y0 ; D0B2DC
1475FF: mac      -y0,x0,b        x0,x:(r2)+                  ; 445ADE
147600: asl      b               a,x:(r1)-                   ; 56513A
147601: macr     y1,x1,b         x:(r3),x0       y:(r5)+n5,y0 ; D0A3FB
147602: nop                                                  ; 000000
147603: mpy      -y0,x0,b        b,x:(r2)-                   ; 5752DC
147604: asl      b                                           ; 20003A
147605: macr     y1,x1,b         x:(r1)+,x0      y:(r5)+n5,y0 ; D0B9FB
147606: mpy      -y0,x0,a        x:(r1)-,x0      y:(r5)+n5,y0 ; D0B1D4
147607: mac      -y0,x0,a        x0,x:(r1)+      y:(r4)+,y1  ; F119D6
147608: asl      a               b,x:(r3)                    ; 576332
147609: macr     y1,x1,a         x:(r2)+,x0      y:(r5)+n5,y0 ; D0BAF3
14760A: mpy      -y0,x0,b        x:(r2)-,x0      y:(r5)+n5,y0 ; D0B2DC
14760B: mac      -y0,x0,b        x0,x:(r2)+                  ; 445ADE
14760C: asl      b               a,x:(r1)-                   ; 56513A
14760D: macr     y1,x1,b         x:(r3),x0       y:(r5)+n5,y0 ; D0A3FB
14760E: nop                                                  ; 000000
14760F: mpy      -y0,x0,b        b,x:(r2)-                   ; 5752DC
147610: asl      b                                           ; 20003A
147611: macr     y1,x1,b         x:(r1)+,x0      y:(r5)+,y0  ; F0B9FB
147612: nop                                                  ; 000000
147613: mpy      -y0,x0,a        b,x:(r3)        y:(r5)+,y0  ; FC23D4
147614: mpy      y0,x0,b         x:(r1)-,x0      y:(r5)+,y0  ; F0B1D8
147615: mac      -y0,x0,a        x0,x:(r1)+      y:(r5)+,y0  ; F039D6
147616: asl      a               y:(r4)+,y1                  ; 4FDC32
147617: macr     y1,x1,a                                     ; 2000F3
147618: mac      y0,x0,b         x:(r2)+,x0      y:(r5)+,y0  ; F0BADA
147619: mpy      -y0,x0,a        a,x:(r1)-       y:(r5)+,y0  ; F831D4
14761A: mac      y0,x0,b         x:(r2)-,x0      y:(r5)+,y0  ; F0B2DA
14761B: mac      -y0,x0,a        x0,x:(r2)+      y:(r5)+,y0  ; F03AD6
14761C: asl      a                                           ; 200032
14761D: macr     y1,x1,a                                     ; 2000F3
14761E: mac      y0,x0,b         x:(r3),x0       y:(r5)+,y0  ; F0A3DA
14761F: mpy      -y0,x0,a        a,x:(r2)-       y:(r5)+,y0  ; F832D4
147620: asl      a                                           ; 200032
147621: macr     y1,x1,a                                     ; 2000F3
147622: mac      y0,x0,b                                     ; 2000DA
147623: asl      #$7,b,b                                     ; 0C1D8F
147624: nop                                                  ; 000000
147625: move     a,x:(r3)        b,y:(r7)+                   ; BB6300
147626: move     b,y:(r7)+                                   ; 5F5F00
147627: move     x:(r1+$0),a                                 ; 02019E
147628: move     x:(r1+$1),b                                 ; 0201DF
147629: move     a,y:(r6+$1e)                                ; 027EAE
14762A: move     b,y:(r6+$1f)                                ; 027EEF
14762B: move     x:(r2+$0),a                                 ; 02029E
14762C: move     x:(r2+$1),b                                 ; 0202DF
14762D: move     a,y:(r6+$20)                                ; 0286AE
14762E: move     x:(r3+$0),a                                 ; 02039E
14762F: move     b,y:(r6+$21)                                ; 0286EF
147630: move     a,y:(r6+$22)                                ; 028EAE
147631: rts                                                  ; 00000C
147632: eor      y1,b            x0,x:-(r6)      a,y0        ; 103E7B
147633: rnd      a               a,x0            y0,y:(r2)-n2 ; 104211
147634: sub      y,a             a,x0            y0,y:(r6)-n6 ; 104634
147635: macr     x0,x0,b         a,x0            y0,y:(r1)+  ; 10598B
147636: sub      x0,b            a,x0            y0,y:(r1+n1) ; 10694C
147637: dc       $10750c                                     ; 10750C
147638: cmp      a,b             a,x0            y0,y:-(r5)  ; 107D0D
147639: subr     a,b             x:(r1)-n1,x0    a,y0        ; 10810E
14763A: mpyr     x1,x0,b         x:(r6)-n6,x0    a,y0        ; 1086A9
14763B: mac      x1,x0,b         x:(r2)+n2,x0    a,y0        ; 108AAA
14763C: lsl      a               x:(r6)+,x0      a,y0        ; 109E33
14763D: dc       $10b1bc                                     ; 10B1BC
14763E: macr     x1,x0,b         a,x0            y:(r1)-,y0  ; 10D1AB
14763F: macr     x1,y0,b         a,x0            y:(r7)-,y0  ; 10D7EB
147640: mpyr     -x1,y0,b        a,x0            y:(r7)+,y0  ; 10DFED
147641: mpy      x1,x0,a         a,x0            y:(r7+n7),y0 ; 10EFA0
147642: dc       $10f7a1                                     ; 10F7A1
147643: clr      a               x0,x:(r7)+n7    a,y1        ; 110F13
147644: macr     -x1,y0,b        x0,x:(r2)-      a,y1        ; 1112EF
147645: mac      y1,y0,b         x0,x:(r5)-      a,y1        ; 1115BA
147646: macr     y1,y0,b         x0,x:(r4)+      a,y1        ; 111CBB
147647: or       x0,a            x0,x:(r4)       a,y1        ; 112442
147648: sub      x0,a            x0,x:(r4+n4)    a,y1        ; 112C44
147649: dc       $11320f                                     ; 11320F
14764A: dc       $1134c4                                     ; 1134C4
14764B: rts                                                  ; 00000C
14764C: move     #$0,x0                                      ; 240000
14764D: move     x0,x:(r6-$19)                               ; 039EC4
14764E: move     y:(r6+$b),x0                                ; 022EF4
14764F: mpy      x0,x0,a                                     ; 200080
147650: move     x:(r0)+,x1                                  ; 45D800
147651: move     a,x0                                        ; 21C400
147652: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
147653: asl      #$2,a,a                                     ; 0C1D04
147654: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
147655: asl      #$2,b,b                                     ; 0C1D85
147656: do       #<$10,>$14765c                              ; 061080 14765B
147658: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r7)+   ; B6F8A0
147659: asl      #$2,a,a                                     ; 0C1D04
14765A: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r7)+   ; B7F8A8
14765B: asl      #$2,b,b                                     ; 0C1D85
14765C: do       #<$640,>$147660                             ; 064086 14765F
14765E: nop                                                  ; 000000
14765F: nop                                                  ; 000000
147660: rts                                                  ; 00000C
147661: move     #>$114000,a0                                ; 50F400 114000
147663: move     y:>$124,x1                                  ; 4DF000 000124
147665: maci     #>$2000,x1,a                                ; 0141E2 002000
147667: move     a0,x0                                       ; 210400
147668: move     x0,y:(r6+$10)                               ; 0246A4
147669: move     #$80,x0                                     ; 248000
14766A: move     x0,x:(r6+$11)                               ; 0246C4
14766B: move     #$0,x0                                      ; 240000
14766C: move     x0,x:(r6+$12)                               ; 024E84
14766D: move     y:(r6+$4),x0                                ; 0216B4
14766E: mpyi     #>$fffc20,x0,a                              ; 0141C0 FFFC20
147670: move     a,x:(r6+$1d)                                ; 0276CE
147671: move     #>$14a000,x0                                ; 44F400 14A000
147673: move     x0,y:(r6+$1b)                               ; 026EE4
147674: move     #$0,x0                                      ; 240000
147675: move     x0,y:(r6+$21)                               ; 0286E4
147676: move     x0,y:(r6+$20)                               ; 0286A4
147677: move     x0,y:(r6+$1d)                               ; 0276E4
147678: move     x0,x:(r6+$22)                               ; 028E84
147679: move     x0,y:(r6+$22)                               ; 028EA4
14767A: rts                                                  ; 00000C
14767B: move     #$0,x0                                      ; 240000
14767C: move     x0,x:(r6-$19)                               ; 039EC4
14767D: move     #>$7fffff,b                                 ; 57F400 7FFFFF
14767F: move     y:(r6+$21),a                                ; 0286FE
147680: cmp      #>$80,a                                     ; 0140C5 000080
147682: bge      func_147687                                 ; 0D1041 000005
147684: add      #>$1,a                                      ; 0140C0 000001
147686: clr      b                                           ; 20001B
147687: move     a,y:(r6+$21)                                ; 0286EE
147688: move     b,y:(r6+$20)                                ; 0286AF
147689: move     #>$0,r4                                     ; 64F400 000000
14768B: move     y:(r6+$b),x0                                ; 022EF4
14768C: mpy      x0,x0,a                                     ; 200080
14768D: move     x:(r0)+,x1                                  ; 45D800
14768E: move     a,x0                                        ; 21C400
14768F: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
147690: asl      #$2,a,a                                     ; 0C1D04
147691: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
147692: asl      #$2,b,b                                     ; 0C1D85
147693: do       #<$10,>$147699                              ; 061080 147698
147695: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r4)+   ; B698A0
147696: asl      #$2,a,a                                     ; 0C1D04
147697: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r4)+   ; B798A8
147698: asl      #$2,b,b                                     ; 0C1D85
147699: move     y:(r6+$6),x0                                ; 021EB4
14769A: mpy      x0,x0,a                                     ; 200080
14769B: move     a,x0                                        ; 21C400
14769C: mpyi     #>$956,x0,a                                 ; 0141C0 000956
14769E: move     #>$20,r0                                    ; 60F400 000020
1476A0: move     r0,r4                                       ; 221400
1476A1: move     a,x0                                        ; 21C400
1476A2: move     x:(r6+$11),a                                ; 0246DE
1476A3: move     x:(r6+$12),b                                ; 024E9F
1476A4: move     a,x1                                        ; 21C500
1476A5: bset     #$14,sr                                     ; 0AF974
1476A6: do       #<$10,>$1476aa                              ; 061080 1476A9
1476A8: mac      -x1,x0,b        b,x:(r0)+       b,y0        ; 1E18AE
1476A9: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
1476AA: bclr     #$14,sr                                     ; 0AF954
1476AB: move     a,x:(r6+$11)                                ; 0246CE
1476AC: move     b,x:(r6+$12)                                ; 024E8F
1476AD: move     y:(r6+$24),a                                ; 0296BE
1476AE: move     y:(r6+$9),y0                                ; 0226F6
1476AF: move     a,y1                                        ; 21C700
1476B0: maci     #>$28f5c,y0,a                               ; 0141D2 028F5C
1476B2: maci     #>$fd70a4,y1,a                              ; 0141F2 FD70A4
1476B4: move     a,y:(r6+$24)                                ; 0296AE
1476B5: move     a,y0                                        ; 21C600
1476B6: mpyi     #>$800,y0,a                                 ; 0141D0 000800
1476B8: move     a,r0                                        ; 21D000
1476B9: move     x:(r0+$14a000),a                            ; 0A70CE 14A000
1476BB: move     x:(r0+$14a800),b                            ; 0A70CF 14A800
1476BD: move     y:(r6+$25),x0                               ; 0296F4
1476BE: move     y:(r6+$26),x1                               ; 029EB5
1476BF: move     b,y:(r6+$25)                                ; 0296EF
1476C0: move     a,y:(r6+$26)                                ; 029EAE
1476C1: sub      x1,a                                        ; 200064
1476C2: sub      x0,b                                        ; 20004C
1476C3: tfr      x1,a            a,y0                        ; 21C661
1476C4: tfr      x0,b            b,y1                        ; 21E749
1476C5: move     #$8,x0                                      ; 240800
1476C6: move     #>$80,r0                                    ; 60F400 000080
1476C8: do       #<$10,>$1476cc                              ; 061080 1476CB
1476CA: mac      y0,x0,a         a,y:(r0)                    ; 5E60D2
1476CB: mac      x0,y1,b         b,x:(r0)+                   ; 5758CA
1476CC: move     #>$80,r5                                    ; 65F400 000080
1476CE: move     #>$20,r0                                    ; 60F400 000020
1476D0: move     r0,r4                                       ; 221400
1476D1: move     #>$50,r1                                    ; 61F400 000050
1476D3: move     r1,r2                                       ; 223200
1476D4: move     x:(r5),y0                                   ; 46E500
1476D5: move     x:(r0)+,x0      y:(r5)+,y1                  ; F1B800
1476D6: do       #<$10,>$1476de                              ; 061080 1476DD
1476D8: mpy      y0,x0,a         y:(r4)+,x1                  ; 4DDCD0
1476D9: mac      y1,x1,a                                     ; 2000F2
1476DA: mpy      x1,y0,b         x:(r5),y0                   ; 46E5E8
1476DB: mac      -x0,y1,b        x:(r0)+,x0      y:(r5)+,y1  ; F1B8CE
1476DC: move     a,x:(r1)+                                   ; 565900
1476DD: move     b,y:(r2)+                                   ; 5F5A00
1476DE: move     #>$20,r1                                    ; 61F400 000020
1476E0: move     #>$50,r0                                    ; 60F400 000050
1476E2: move     #>$30,r3                                    ; 63F400 000030
1476E4: move     #>$60,r2                                    ; 62F400 000060
1476E6: move     #$c0,y0                                     ; 26C000
1476E7: move     #>$6ed9ec,y1                                ; 47F400 6ED9EC
1476E9: move     x:(r1),x0                                   ; 44E100
1476EA: do       #<$10,>$1476f2                              ; 061080 1476F1
1476EC: mpy      y0,x0,a         y:(r1)+,x1                  ; 4DD9D0
1476ED: mac      y1,x1,a         x:(r0),x0                   ; 44E0F2
1476EE: mpy      y0,x0,b         y:(r0)+,x1                  ; 4DD8D8
1476EF: mac      y1,x1,b         x:(r1),x0                   ; 44E1FA
1476F0: move     a,x:(r3)+                                   ; 565B00
1476F1: move     b,x:(r2)+                                   ; 575A00
1476F2: move     #$c0,y0                                     ; 26C000
1476F3: move     #>$912614,y1                                ; 47F400 912614
1476F5: move     #>$20,r1                                    ; 61F400 000020
1476F7: move     #>$50,r0                                    ; 60F400 000050
1476F9: move     x:(r1),x0                                   ; 44E100
1476FA: do       #<$10,>$147702                              ; 061080 147701
1476FC: mpy      y0,x0,a         y:(r1)+,x1                  ; 4DD9D0
1476FD: mac      y1,x1,a         x:(r0),x0                   ; 44E0F2
1476FE: mpy      y0,x0,b         y:(r0)+,x1                  ; 4DD8D8
1476FF: mac      y1,x1,b         x:(r1),x0                   ; 44E1FA
147700: move     a,x:(r3)+                                   ; 565B00
147701: move     b,x:(r2)+                                   ; 575A00
147702: move     y:(r6+$4),x0                                ; 0216B4
147703: mpyi     #>$fffc20,x0,a                              ; 0141C0 FFFC20
147705: add      #>$fffff0,a                                 ; 0140C0 FFFFF0
147707: move     x:(r6+$1d),b                                ; 0276DF
147708: move     y:(r6+$1d),b0                               ; 0276F9
147709: move     b,l:?:>$ff                                  ; 497000 0000FF
14770B: move     a,x0                                        ; 21C400
14770C: move     b,x1                                        ; 21E500
14770D: maci     #>$28f5c,x0,b                               ; 0141CA 028F5C
14770F: maci     #>$fd70a4,x1,b                              ; 0141EA FD70A4
147711: move     b1,x:(r6+$1d)                               ; 0276CD
147712: move     b0,y:(r6+$1d)                               ; 0276E9
147713: move     x:(r6+$22),a                                ; 028E9E
147714: move     y:(r6+$22),a0                               ; 028EB8
147715: move     y:(r6+$5),x0                                ; 0216F4
147716: move     a,x1                                        ; 21C500
147717: maci     #>$fd70a4,x0,a                              ; 0141C2 FD70A4
147719: maci     #>$fd70a4,x1,a                              ; 0141E2 FD70A4
14771B: move     a1,x:(r6+$22)                               ; 028E8C
14771C: move     a0,y:(r6+$22)                               ; 028EA8
14771D: sub      #>$fffff0,b                                 ; 0140CC FFFFF0
14771F: asl      #$8,b,b                                     ; 0C1D91
147720: move     a,x1                                        ; 21C500
147721: move     b,x0                                        ; 21E400
147722: mpy      x1,x0,a                                     ; 2000A0
147723: move     y:(r6+$23),x0                               ; 028EF4
147724: move     a,y:(r6+$23)                                ; 028EEE
147725: sub      x0,a            #>$80000,y1                 ; 47F444 080000
147727: tfr      x0,a            a,y0                        ; 21C641
147728: move     #>$80,r3                                    ; 63F400 000080
14772A: move     #$f,m3                                      ; 050FA3
14772B: do       #<$10,>$14772e                              ; 061080 14772D
14772D: mac      y1,y0,a         a,x:(r3)+                   ; 565BB2
14772E: move     #>$20,r1                                    ; 61F400 000020
147730: move     #>$1f,r2                                    ; 62F400 00001F
147732: move     x:(r6+$1d),b                                ; 0276DF
147733: move     y:(r6+$1d),b0                               ; 0276F9
147734: move     l:?:>$ff,y                                  ; 43F000 0000FF
147736: sub      y,b                                         ; 20003C
147737: add      #>$10,b                                     ; 0140C8 000010
147739: asr      #$4,b,b                                     ; 0C1C89
14773A: move     b1,y1                                       ; 21A700
14773B: move     b0,y0                                       ; 212600
14773C: move     l:(r2),a                                    ; 48E200
14773D: move     x:(r1)+,x1                                  ; 45D900
14773E: move     x:(r3)+,x0                                  ; 44DB00
14773F: do       #<$6,>$14774a                               ; 060680 147749
147741: move     l:?:>$ff,b                                  ; 49F000 0000FF
147743: do       #<$10,>$147749                              ; 061080 147748
147745: mpy      x1,x0,a         a,l:(r2)+                   ; 485AA0
147746: asr      #$8,a,a                                     ; 0C1C10
147747: add      b,a             x:(r1)+,x1                  ; 45D910
147748: add      y,b             x:(r3)+,x0                  ; 44DB38
147749: nop                                                  ; 000000
14774A: move     a,l:(r2)+                                   ; 485A00
14774B: move     #>$ffffff,m3                                ; 05F423 FFFFFF
14774D: move     y:(r6+$10),r0                               ; 0B76D0 000010
14774F: move     #>$7ff,m0                                   ; 05F420 0007FF
147751: move     m0,m4                                       ; 0464A0
147752: move     r0,a                                        ; 220E00
147753: add      #>$800,a                                    ; 0140C0 000800
147755: move     a,r1                                        ; 21D100
147756: move     #>$20,r2                                    ; 62F400 000020
147758: move     #>$80,r3                                    ; 63F400 000080
14775A: do       #<$2,>$14776a                               ; 060280 147769
14775C: move     x:(r2),n0                                   ; 70E200
14775D: lua      (r0)+n0,r4                                  ; 044814
14775E: do       #<$30,>$147769                              ; 063080 147768
147760: move     y:(r2)+,y1                                  ; 4FDA00
147761: move     y:(r4)+,x0                                  ; 4CDC00
147762: move     y:(r4)+,x1                                  ; 4DDC00
147763: mpysu    x1,y1,a                                     ; 01278F
147764: macsu    -x0,y1,a                                    ; 012694
147765: asr      a               x:(r2),n0                   ; 70E222
147766: add      x0,a                                        ; 200040
147767: lua      (r0)+n0,r4                                  ; 044814
147768: move     a,x:(r3)+                                   ; 565B00
147769: move     r1,r0                                       ; 223000
14776A: move     #>$ffffff,m4                                ; 05F424 FFFFFF
14776C: move     #>$ffffff,m0                                ; 05F420 FFFFFF
14776E: move     #>$3e,r4                                    ; 64F400 00003E
147770: move     #>$80,r0                                    ; 60F400 000080
147772: move     #$5f,m0                                     ; 055FA0
147773: move     #$10,n0                                     ; 381000
147774: move     #>$2aaaab,y1                                ; 47F400 2AAAAB
147776: move     y:(r6+$20),x0                               ; 0286B4
147777: mpy      x0,y1,a                                     ; 2000C0
147778: move     x:(r0)+n0,x1                                ; 45C800
147779: move     a,y1                                        ; 21C700
14777A: do       #<$10,>$147783                              ; 061080 147782
14777C: mpy      y1,x1,a         x:(r0)+n0,x1    a,y:(r4)+   ; B688F0
14777D: mac      y1,x1,a         x:(r0)+n0,x1                ; 45C8F2
14777E: mac      y1,x1,a         x:(r0)+n0,x1                ; 45C8F2
14777F: mpy      y1,x1,b         x:(r0)+n0,x1    b,y:(r4)+   ; B788F8
147780: mac      y1,x1,b         x:(r0)+n0,x1                ; 45C8FA
147781: mac      y1,x1,b         (r0)+                       ; 2058FA
147782: move     x:(r0)+n0,x1                                ; 45C800
147783: move     a,y:(r4)+                                   ; 5E5C00
147784: move     b,y:(r4)+                                   ; 5F5C00
147785: move     #>$ffffff,m0                                ; 05F420 FFFFFF
147787: move     #>$144ac7,r2                                ; 62F400 144AC7
147789: move     y:(r6+$a),b                                 ; 022EBF
14778A: asr      #$10,b,b                                    ; 0C1CA1
14778B: move     #>$40,r5                                    ; 65F400 000040
14778D: move     #>$60,r4                                    ; 64F400 000060
14778F: move     b,n2                                        ; 21FA00
147790: move     y:(r6+$1e),a                                ; 027EBE
147791: move     y:(r6+$1f),b                                ; 027EFF
147792: move     #>$0,r0                                     ; 60F400 000000
147794: move     x:(r2+n2),x0                                ; 44EA00
147795: move     y:(r5)+,y1                                  ; 4FDD00
147796: do       #<$10,>$14779c                              ; 061080 14779B
147798: mac      x0,y1,a         a,x1            a,y:(r4)+   ; 165CC2
147799: mac      -x1,x0,a        y:(r5)+,y1                  ; 4FDDA6
14779A: mac      x0,y1,b         b,x1            b,y:(r4)+   ; 1F5CCA
14779B: mac      -x1,x0,b        y:(r5)+,y1                  ; 4FDDAE
14779C: move     a,y:(r6+$1e)                                ; 027EAE
14779D: move     b,y:(r6+$1f)                                ; 027EEF
14779E: move     #>$0,r4                                     ; 64F400 000000
1477A0: move     #>$60,r5                                    ; 65F400 000060
1477A2: move     #$2,n4                                      ; 3C0200
1477A3: move     n4,n5                                       ; 239D00
1477A4: move     y:(r6+$10),a                                ; 0246BE
1477A5: move     a,r0                                        ; 21D000
1477A6: add      #>$800,a                                    ; 0140C0 000800
1477A8: move     #>$7ff,m0                                   ; 05F420 0007FF
1477AA: move     m0,m1                                       ; 0461A0
1477AB: move     a,r1                                        ; 21D100
1477AC: move     y:(r6+$8),a                                 ; 0226BE
1477AD: neg      a                                           ; 200036
1477AE: sub      #>$fd71,a                                   ; 0140C4 00FD71
1477B0: move     #>$7fffff,b                                 ; 57F400 7FFFFF
1477B2: sub      a,b                                         ; 20001C
1477B3: asr      b                                           ; 20002A
1477B4: move     a,x1                                        ; 21C500
1477B5: move     b,x0                                        ; 21E400
1477B6: move     y:(r4)+n4,y0                                ; 4ECC00
1477B7: do       #<$10,>$1477bc                              ; 061080 1477BB
1477B9: mpy      y0,x0,a         y:(r5)+n5,y0                ; 4ECDD0
1477BA: mac      x1,y0,a         y:(r4)+n4,y0                ; 4ECCE2
1477BB: move     a,x:(r0)+                                   ; 565800
1477BC: move     #>$1,r4                                     ; 64F400 000001
1477BE: move     #>$61,r5                                    ; 65F400 000061
1477C0: move     r0,y:(r6+$10)                               ; 0B7690 000010
1477C2: move     y:(r4)+n4,y0                                ; 4ECC00
1477C3: do       #<$10,>$1477c8                              ; 061080 1477C7
1477C5: mpy      y0,x0,a         y:(r5)+n5,y0                ; 4ECDD0
1477C6: mac      x1,y0,a         y:(r4)+n4,y0                ; 4ECCE2
1477C7: move     a,x:(r1)+                                   ; 565900
1477C8: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1477CA: move     m0,m1                                       ; 0461A0
1477CB: move     #>$0,r4                                     ; 64F400 000000
1477CD: move     #>$40,r5                                    ; 65F400 000040
1477CF: move     y:(r6+$7),x0                                ; 021EF4
1477D0: move     #>$7fffff,a                                 ; 56F400 7FFFFF
1477D2: sub      x0,a                                        ; 200044
1477D3: move     y:(r4)+,y0                                  ; 4EDC00
1477D4: move     a,x1                                        ; 21C500
1477D5: do       #<$10,>$1477dd                              ; 061080 1477DC
1477D7: mpy      x1,y0,a         y:(r5)+,y0                  ; 4EDDE0
1477D8: mac      y0,x0,a         y:(r4)+,y0                  ; 4EDCD2
1477D9: mpy      x1,y0,b         y:(r5)+,y0                  ; 4EDDE8
1477DA: mac      y0,x0,b         y:(r4)+,y0                  ; 4EDCDA
1477DB: move     a,y:(r7)+                                   ; 5E5F00
1477DC: move     b,y:(r7)+                                   ; 5F5F00
1477DD: rts                                                  ; 00000C
1477DE: move     #>$20c5,x0                                  ; 44F400 0020C5
1477E0: move     x0,y:(r6+$10)                               ; 0246A4
1477E1: move     #$0,x0                                      ; 240000
1477E2: move     x0,y:(r6+$11)                               ; 0246E4
1477E3: move     x0,y:(r6+$12)                               ; 024EA4
1477E4: move     x0,y:(r6+$13)                               ; 024EE4
1477E5: rts                                                  ; 00000C
1477E6: move     #$0,x0                                      ; 240000
1477E7: move     x0,x:(r6-$19)                               ; 039EC4
1477E8: move     #>$0,r4                                     ; 64F400 000000
1477EA: move     y:(r6+$b),x0                                ; 022EF4
1477EB: mpy      x0,x0,a                                     ; 200080
1477EC: move     x:(r0)+,x1                                  ; 45D800
1477ED: move     a,x0                                        ; 21C400
1477EE: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
1477EF: asl      #$2,a,a                                     ; 0C1D04
1477F0: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
1477F1: asl      #$2,b,b                                     ; 0C1D85
1477F2: do       #<$10,>$1477f8                              ; 061080 1477F7
1477F4: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r4)+   ; B698A0
1477F5: asl      #$2,a,a                                     ; 0C1D04
1477F6: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r4)+   ; B798A8
1477F7: asl      #$2,b,b                                     ; 0C1D85
1477F8: move     #>$0,r4                                     ; 64F400 000000
1477FA: move     #>$61,r0                                    ; 60F400 000061
1477FC: move     y:(r4)+,y0                                  ; 4EDC00
1477FD: do       #<$10,>$147802                              ; 061080 147801
1477FF: mpy      y0,y0,a         y:(r4)+,y0                  ; 4EDC90
147800: mpy      y0,y0,b         b,x:(r0)+       y:(r4)+,y0  ; FC1898
147801: maxm     a,b                                         ; 200015
147802: move     b,x:(r0)+                                   ; 575800
147803: move     y:(r6+$a),x0                                ; 022EB4
147804: mpyi     #>$80,x0,b                                  ; 0141C8 000080
147806: move     #>$62,r0                                    ; 60F400 000062
147808: move     r0,r1                                       ; 221100
147809: move     b,r4                                        ; 21F400
14780A: move     x:(r6+$15),a                                ; 0256DE
14780B: move     y:(r6+$15),a0                               ; 0256F8
14780C: move     x:(r4+$1446c6),y0                           ; 0A74C6 1446C6
14780E: move     x:(r0)+,x0                                  ; 44D800
14780F: do       #<$10,>$147814                              ; 061080 147813
147811: mac      y0,x0,a         a,x0                        ; 21C4D2
147812: mac      -y0,x0,a        x:(r0)+,x0                  ; 44D8D6
147813: move     a,l:(r1)+                                   ; 485900
147814: move     a,x0                                        ; 21C400
147815: move     x0,x:(r6+$15)                               ; 0256C4
147816: move     a0,y:(r6+$15)                               ; 0256E8
147817: move     #>$62,r0                                    ; 60F400 000062
147819: move     #>$62,r3                                    ; 63F400 000062
14781B: move     #>$80,r5                                    ; 65F400 000080
14781D: do       #<$10,>$14784a                              ; 061080 147849
14781F: move     l:(r3)+,a                                   ; 48DB00
147820: or       #>$1,a                                      ; 0140C2 000001
147822: clb      a,b                                         ; 0C1E01
147823: normf    b1,a                                        ; 0C1E26
147824: move     a1,x0                                       ; 218400
147825: mpyi     #>$200,x0,a                                 ; 0141C0 000200
147827: move     a1,r1                                       ; 219100
147828: neg      b                                           ; 20003E
147829: move     x:(r1+$1444c6),b0                           ; 0A71C9 1444C6
14782B: asl      #$11,b,b                                    ; 0C1DA3
14782C: move     y:(r6+$6),x0                                ; 021EB4
14782D: mpyi     #>$d00000,x0,a                              ; 0141C0 D00000
14782F: add      #>$320000,a                                 ; 0140C0 320000
147831: sub      a,b             b,y0                        ; 21E61C
147832: move     a,y1                                        ; 21C700
147833: move     #$40,a                                      ; 2E4000
147834: move     y:(r6+$8),x1                                ; 0226B5
147835: maci     #>$c04000,x1,a                              ; 0141E2 C04000
147837: asl      a                                           ; 200032
147838: move     b,x0                                        ; 21E400
147839: move     a,x1                                        ; 21C500
14783A: mpy      x1,x0,a                                     ; 2000A0
14783B: add      y1,a                                        ; 200070
14783C: sub      y0,a                                        ; 200054
14783D: clr      a               ifmi                        ; 202B13
14783E: asr      #$7,a,a                                     ; 0C1C0E
14783F: rnd      a                                           ; 200011
147840: asr      #$b,a,b                                     ; 0C1C17
147841: and      #>$7ff,a                                    ; 0140C6 0007FF
147843: neg      a                                           ; 200036
147844: move     a1,r2                                       ; 219200
147845: move     b1,x0                                       ; 21A400
147846: move     x:(r2+$1407ff),a                            ; 0A72CE 1407FF
147848: asr      x0,a,b                                      ; 0C1E69
147849: move     b,x:(r0)+                                   ; 575800
14784A: move     y:(r6+$4),x0                                ; 0216B4
14784B: mpyi     #>$80,x0,b                                  ; 0141C8 000080
14784D: move     y:(r6+$5),x0                                ; 0216F4
14784E: move     b,r4                                        ; 21F400
14784F: mpyi     #>$80,x0,b                                  ; 0141C8 000080
147851: move     b,r0                                        ; 21F000
147852: move     #>$fd,r3                                    ; 63F400 0000FD
147854: move     #>$fe,r2                                    ; 62F400 0000FE
147856: move     #>$ff,r1                                    ; 61F400 0000FF
147858: move     #>$80,r5                                    ; 65F400 000080
14785A: move     x:(r4+$1446c6),a                            ; 0A74CE 1446C6
14785C: move     x:(r0+$144746),b                            ; 0A70CF 144746
14785E: move     #>$62,r4                                    ; 64F400 000062
147860: move     a,x:(r2)                                    ; 566200
147861: move     b,x:(r1)                                    ; 576100
147862: move     y:(r6+$11),a                                ; 0246FE
147863: move     x:(r6+$11),b                                ; 0246DF
147864: do       #<$10,>$147870                              ; 061080 14786F
147866: move     x:(r4),y1                                   ; 47E400
147867: move     x:(r2),x0                                   ; 44E200
147868: cmp      y1,b                                        ; 20007D
147869: bge      func_14786c                                 ; 0D1041 000003
14786B: move     x:(r1),x0                                   ; 44E100
14786C: tfr      y1,b                                        ; 200079
14786D: mac      x0,y1,a         a,y1                        ; 21C7C2
14786E: mac      -x0,y1,a                                    ; 2000C6
14786F: move     a,x:(r4)+                                   ; 565C00
147870: move     a,y:(r6+$11)                                ; 0246EE
147871: move     b,x:(r6+$11)                                ; 0246CF
147872: move     #>$62,r0                                    ; 60F400 000062
147874: move     #>$40,r5                                    ; 65F400 000040
147876: move     #>$0,r4                                     ; 64F400 000000
147878: move     x:(r0)+,x0      y:(r4)+,y0                  ; F09800
147879: do       #<$10,>$14787f                              ; 061080 14787E
14787B: mpy      y0,x0,a         y:(r4)+,y1                  ; 4FDCD0
14787C: mpy      x0,y1,b         x:(r0)+,x0      y:(r4)+,y0  ; F098C8
14787D: move     a,y:(r5)+                                   ; 5E5D00
14787E: move     b,y:(r5)+                                   ; 5F5D00
14787F: move     #>$40,r5                                    ; 65F400 000040
147881: move     #>$0,r4                                     ; 64F400 000000
147883: move     #>$7fffff,a                                 ; 56F400 7FFFFF
147885: move     y:(r6+$7),x0                                ; 021EF4
147886: sub      x0,a                                        ; 200044
147887: move     y:(r6+$9),y0                                ; 0226F6
147888: mpy      y0,y0,b                                     ; 200098
147889: move     a,x1                                        ; 21C500
14788A: move     b,y1                                        ; 21E700
14788B: mpyi     #>$7f8000,y1,b                              ; 0141F8 7F8000
14788D: add      #>$8000,b                                   ; 0140C8 008000
14788F: move     b,y1                                        ; 21E700
147890: mpy      x0,y1,a                                     ; 2000C0
147891: move     y:(r5)+,y0                                  ; 4EDD00
147892: move     a,x0                                        ; 21C400
147893: do       #<$10,>$14789d                              ; 061080 14789C
147895: mpy      y0,x0,a         y:(r5)+,y0                  ; 4EDDD0
147896: asl      #$8,a,a                                     ; 0C1D10
147897: mpy      y0,x0,b         y:(r4)+,y0                  ; 4EDCD8
147898: asl      #$8,b,b                                     ; 0C1D91
147899: mac      x1,y0,a         y:(r4)+,y0                  ; 4EDCE2
14789A: mac      x1,y0,b         y:(r5)+,y0                  ; 4EDDEA
14789B: move     a,y:(r7)+                                   ; 5E5F00
14789C: move     b,y:(r7)+                                   ; 5F5F00
14789D: rts                                                  ; 00000C
14789E: move     x0,y:(r6+$e)                                ; 023EA4
14789F: clr      a                                           ; 200013
1478A0: move     r6,r0                                       ; 22D000
1478A1: move     r6,r1                                       ; 22D100
1478A2: move     #$58,n0                                     ; 385800
1478A3: move     #$14,n1                                     ; 391400
1478A4: move     a,x:(r6+$55)                                ; 0A768E 000055
1478A6: move     a,y:(r6+$55)                                ; 0B768E 000055
1478A8: move     (r0)+n0                                     ; 204800
1478A9: move     (r1)+n1                                     ; 204900
1478AA: move     a,x:(r6+$57)                                ; 0A768E 000057
1478AC: rep      #<$30                                       ; 0630A0
1478AD: move     a,x:(r0)+                                   ; 565800
1478AE: rep      #<$41                                       ; 0641A0
1478AF: move     a,l:(r1)+                                   ; 485900
1478B0: move     #>$88,x0                                    ; 44F400 000088
1478B2: move     x0,x:(r6+$56)                               ; 0A7684 000056
1478B4: move     #>$80,x0                                    ; 44F400 000080
1478B6: move     x0,y:(r6+$56)                               ; 0B7684 000056
1478B8: move     x0,x:(r6+$11)                               ; 0246C4
1478B9: move     #>$40,x0                                    ; 44F400 000040
1478BB: move     x:<<$ffffee,a                               ; 56F000 FFFFEE
1478BD: and      #>$fffffe,a                                 ; 0140C6 FFFFFE
1478BF: sub      #>$22,a                                     ; 0140C4 000022
1478C1: cmp      #>$100,a                                    ; 0140C5 000100
1478C3: add      x0,a            iflt                        ; 202940
1478C4: move     a,y:(r6+$11)                                ; 0246EE
1478C5: rts                                                  ; 00000C
1478C6: rts                                                  ; 00000C
1478C7: move     #$0,x0                                      ; 240000
1478C8: move     x0,x:(r6-$19)                               ; 039EC4
1478C9: move     a,l:?:>$ff                                  ; 487000 0000FF
1478CB: move     r0,x:>$fe                                   ; 607000 0000FE
1478CD: move     l:?:>$ff,a                                  ; 48F000 0000FF
1478CF: move     #>$4074,y0                                  ; 46F400 004074
1478D1: cmp      y0,a                                        ; 200055
1478D2: tge      y0,a                                        ; 021050
1478D3: clb      a,b                                         ; 0C1E01
1478D4: normf    b1,a                                        ; 0C1E26
1478D5: move     a,x0                                        ; 21C400
1478D6: move     #>$2b1100,a                                 ; 56F400 2B1100
1478D8: andi     #$fe,ccr                                    ; 00FEB9
1478D9: rep      #<$18                                       ; 0618A0
1478DA: div      x0,a                                        ; 018040
1478DB: move     a0,a                                        ; 210E00
1478DC: add      #>$1d,b                                     ; 0140C8 00001D
1478DE: normf    b1,a                                        ; 0C1E26
1478DF: move     a,l:?:>$fd                                  ; 487000 0000FD
1478E1: move     x:(r6+$55),b                                ; 0A76CF 000055
1478E3: move     y:(r6+$55),b0                               ; 0B76C9 000055
1478E5: asl      b                                           ; 20003A
1478E6: asl      #$8,b,b                                     ; 0C1D91
1478E7: move     l:?:>$ff,y                                  ; 43F000 0000FF
1478E9: move     #>$17c6f9,x0                                ; 44F400 17C6F9
1478EB: mpysu    x0,y0,a                                     ; 01278D
1478EC: dmac     ss x0,y1,a                                  ; 012484
1478ED: move     b1,y1                                       ; 21A700
1478EE: move     b0,y0                                       ; 212600
1478EF: move     x:(r6+$10),b                                ; 02469F
1478F0: move     y:(r6+$10),b0                               ; 0246B9
1478F1: move     a1,x:(r6+$10)                               ; 02468C
1478F2: move     a0,y:(r6+$10)                               ; 0246A8
1478F3: sub      b,a                                         ; 200014
1478F4: asr      #$4,a,a                                     ; 0C1C08
1478F5: move     b,l:?:>$fc                                  ; 497000 0000FC
1478F7: asl      #$a,b,b                                     ; 0C1D95
1478F8: move     a,l:?:>$fb                                  ; 487000 0000FB
1478FA: move     b1,x1                                       ; 21A500
1478FB: move     b0,x0                                       ; 212400
1478FC: mpy      y1,x1,b                                     ; 2000F8
1478FD: asl      #$4,b,b                                     ; 0C1D89
1478FE: move     y:(r6+$56),a                                ; 0B76CE 000056
147900: move     #>$1000,x0                                  ; 44F400 001000
147902: cmp      #>$80,a                                     ; 0140C5 000080
147904: add      x0,b            ifeq                        ; 202A48
147905: move     #>$1fff,x0                                  ; 44F400 001FFF
147907: move     #>$14a000,x1                                ; 45F400 14A000
147909: sub      #>$800,b                                    ; 0140CC 000800
14790B: and      x0,b                                        ; 20004E
14790C: sub      x0,b                                        ; 20004C
14790D: neg      b                                           ; 20003E
14790E: add      x1,b                                        ; 200068
14790F: move     #>$7f,r1                                    ; 61F400 00007F
147911: move     y:(r6+$e),a                                 ; 023EBE
147912: move     y:(r6+$f),a0                                ; 023EF8
147913: move     b1,y:(r6+$e)                                ; 023EAD
147914: move     b0,y:(r6+$f)                                ; 023EE9
147915: tfr      a,b                                         ; 200009
147916: move     l:?:>$fc,a                                  ; 48F000 0000FC
147918: do       #<$10,>$14791d                              ; 061080 14791C
14791A: add      a,b             b,l:(r1)+                   ; 495918
14791B: and      x0,b                                        ; 20004E
14791C: add      x1,b                                        ; 200068
14791D: move     b,l:(r1)+                                   ; 495900
14791E: move     #>$80,r1                                    ; 61F400 000080
147920: move     r1,r4                                       ; 223400
147921: move     #>$63,r5                                    ; 65F400 000063
147923: move     x:(r1)+,r2                                  ; 62D900
147924: move     #>$1fff,m2                                  ; 05F422 001FFF
147926: move     m2,m3                                       ; 0463A2
147927: move     y:(r4)+,y0                                  ; 4EDC00
147928: move     x:(r2)+,x1                                  ; 45DA00
147929: move     x:(r1)+,r3                                  ; 63D900
14792A: do       #<$8,>$147936                               ; 060880 147935
14792C: mpysu    -x1,y0,a                                    ; 012796
14792D: add      x1,a            x:(r2),x0       b,y:(r5)+   ; B3A260
14792E: add      x1,a            x:(r1)+,r2                  ; 62D960
14792F: macsu    x0,y0,a                                     ; 01268D
147930: asr      a               x:(r3)+,x1      y:(r4)+,y0  ; F49B22
147931: mpysu    -x1,y0,b                                    ; 0127B6
147932: add      x1,b            x:(r3),x0       a,y:(r5)+   ; B2A368
147933: add      x1,b            x:(r1)+,r3                  ; 63D968
147934: macsu    x0,y0,b                                     ; 0126AD
147935: asr      b               x:(r2)+,x1      y:(r4)+,y0  ; F49A2A
147936: move     #>$ffffff,m3                                ; 05F423 FFFFFF
147938: move     #>$ffffff,m2                                ; 05F422 FFFFFF
14793A: move     b,y:(r5)+                                   ; 5F5D00
14793B: move     x:(r6+$12),a                                ; 024E9E
14793C: move     l:?:>$ff,x                                  ; 42F000 0000FF
14793E: move     x1,x:(r6+$12)                               ; 024E85
14793F: move     y:(r6+$12),a0                               ; 024EB8
147940: move     x0,y:(r6+$12)                               ; 024EA4
147941: move     #>$17,x1                                    ; 45F400 000017
147943: asl      #$8,a,a                                     ; 0C1D10
147944: move     l:?:>$fd,y                                  ; 43F000 0000FD
147946: move     a,x0                                        ; 21C400
147947: mpysu    x0,y0,a                                     ; 01278D
147948: dmac     ss x0,y1,a                                  ; 012484
147949: clb      a,b                                         ; 0C1E01
14794A: normf    b1,a                                        ; 0C1E26
14794B: neg      b                                           ; 20003E
14794C: sub      x1,b            a,x0                        ; 21C46C
14794D: move     x:(r6+$55),y1                               ; 0A76C7 000055
14794F: move     y:(r6+$55),y0                               ; 0B76C6 000055
147951: mpysu    x0,y0,a                                     ; 01278D
147952: dmac     ss x0,y1,a                                  ; 012484
147953: move     #>$5f1be3,x0                                ; 44F400 5F1BE3
147955: move     a1,y1                                       ; 218700
147956: move     a0,y0                                       ; 210600
147957: mpysu    x0,y0,a                                     ; 01278D
147958: dmac     ss x0,y1,a                                  ; 012484
147959: normf    b1,a                                        ; 0C1E26
14795A: move     a1,x:(r6+$55)                               ; 0A768C 000055
14795C: move     a0,y:(r6+$55)                               ; 0B7688 000055
14795E: move     l:?:>$fd,a                                  ; 48F000 0000FD
147960: move     #>$80,r0                                    ; 60F400 000080
147962: move     #$1,m0                                      ; 0501A0
147963: asr      a                                           ; 200022
147964: move     a,l:(r0)+                                   ; 485800
147965: move     a,l:(r0)                                    ; 486000
147966: move     x:(r6+$11),r0                               ; 0A76D0 000011
147968: move     x:(r6+$55),b                                ; 0A76CF 000055
14796A: move     y:(r6+$55),b0                               ; 0B76C9 000055
14796C: move     #$0,r1                                      ; 310000
14796D: bsr      func_0003d3                                 ; 0D1080 EB8A66
14796F: move     r0,x:(r6+$11)                               ; 0A7690 000011
147971: move     b1,x:(r6+$55)                               ; 0A768D 000055
147973: move     b0,y:(r6+$55)                               ; 0B7689 000055
147975: move     #>$ffffff,m0                                ; 05F420 FFFFFF
147977: move     #>$80,r7                                    ; 67F400 000080
147979: move     #$1,m7                                      ; 0501A7
14797A: move     #>$40,x0                                    ; 44F400 000040
14797C: move     #>$5ae148,y0                                ; 46F400 5AE148
14797E: move     y0,y:(r7)+                                  ; 4E5F00
14797F: move     #>$a51eb8,y0                                ; 46F400 A51EB8
147981: move     y0,y:(r7)                                   ; 4E6700
147982: move     x:(r6+$56),a                                ; 0A76CE 000056
147984: move     r6,r0                                       ; 22D000
147985: move     r6,r3                                       ; 22D300
147986: move     a,n0                                        ; 21D800
147987: add      #>$10,a                                     ; 0140C0 000010
147989: cmp      #>$98,a                                     ; 0140C5 000098
14798B: sub      x0,a            ifeq                        ; 202A44
14798C: move     y:(r6+$56),r7                               ; 0B76D7 000056
14798E: move     a,x:(r6+$56)                                ; 0A768E 000056
147990: move     a,n3                                        ; 21DB00
147991: move     (r0)+n0                                     ; 204800
147992: move     (r3)+n3                                     ; 204B00
147993: move     #$0,x1                                      ; 250000
147994: rep      #<$10                                       ; 0610A0
147995: move     x1,x:(r0)+                                  ; 455800
147996: move     #>$ffffff,m0                                ; 05F420 FFFFFF
147998: bsr      func_0003aa                                 ; 0D1080 EB8A12
14799A: move     r7,y:(r6+$56)                               ; 0B7697 000056
14799C: move     #>$2ccccd,x1                                ; 45F400 2CCCCD
14799E: move     #>$54,y1                                    ; 47F400 000054
1479A0: move     r3,r0                                       ; 227000
1479A1: move     #>$f,r1                                     ; 61F400 00000F
1479A3: move     #>$ffffff,m0                                ; 05F420 FFFFFF
1479A5: move     y:(r6+$c),a                                 ; 0236BE
1479A6: move     x:(r0)+,x0                                  ; 44D800
1479A7: do       #<$10,>$1479ab                              ; 061080 1479AA
1479A9: mac      x1,x0,a         a,x:(r1)+       a,y0        ; 1819A2
1479AA: mac      -y1,y0,a        x:(r0)+,x0                  ; 44D8B6
1479AB: move     x:(r6-$27),x1                               ; 0366D5
1479AC: move     a,y:(r6+$c)                                 ; 0236AE
1479AD: move     a,x:(r1)+                                   ; 565900
1479AE: mpyi     #>$15555,x1,a                               ; 0141E0 015555
1479B0: sub      #>$2,a                                      ; 0140C4 000002
1479B2: clr      a               ifmi                        ; 202B13
1479B3: move     #>$ff,y0                                    ; 46F400 0000FF
1479B5: cmp      y0,a                                        ; 200055
1479B6: tfr      y0,a            ifge                        ; 202151
1479B7: move     y:(r6+$13),y1                               ; 024EF7
1479B8: move     a,r2                                        ; 21D200
1479B9: move     #>$f,r1                                     ; 61F400 00000F
1479BB: move     r1,r0                                       ; 223000
1479BC: move     x:(r2+$143d06),x0                           ; 0A72C4 143D06
1479BE: move     x0,y:(r6+$13)                               ; 024EE4
1479BF: move     y:(r6+$4),b                                 ; 0216BF
1479C0: sub      #>$400000,b                                 ; 0140CC 400000
1479C2: clr      b               ifmi                        ; 202B1B
1479C3: asl      b                                           ; 20003A
1479C4: move     b,x0                                        ; 21E400
1479C5: move     #>$7fffff,b                                 ; 57F400 7FFFFF
1479C7: sub      x0,b                                        ; 20004C
1479C8: mpyi     #>$19999a,x0,a                              ; 0141C0 19999A
1479CA: move     b,x1                                        ; 21E500
1479CB: move     a,x0                                        ; 21C400
1479CC: move     y:(r6+$d),a                                 ; 0236FE
1479CD: move     x:(r0)+,y0                                  ; 46D800
1479CE: do       #<$10,>$1479d4                              ; 061080 1479D3
1479D0: mac      y1,y0,a         b,x:(r1)+       a,y0        ; 1C19B2
1479D1: mac      -y1,y0,a                                    ; 2000B6
1479D2: mpy      x1,y0,b         x:(r0)+,y0                  ; 46D8E8
1479D3: mac      y0,x0,b                                     ; 2000DA
1479D4: move     x:(r6-$27),x1                               ; 0366D5
1479D5: move     a,y:(r6+$d)                                 ; 0236EE
1479D6: move     b,x:(r1)+                                   ; 575900
1479D7: mpyi     #>$80000,x1,a                               ; 0141E0 080000
1479D9: add      #>$ffff80,a                                 ; 0140C0 FFFF80
1479DB: clr      a               ifmi                        ; 202B13
1479DC: move     #>$63f,x0                                   ; 44F400 00063F
1479DE: cmp      x0,a                                        ; 200045
1479DF: tfr      x0,a            ifgt                        ; 202741
1479E0: move     x:(r6+$13),r2                               ; 0A76D2 000013
1479E2: move     a,x:(r6+$13)                                ; 024ECE
1479E3: move     #>$7fffff,a                                 ; 56F400 7FFFFF
1479E5: move     x:(r2+$1435c6),x1                           ; 0A72C5 1435C6
1479E7: sub      x1,a            a,x0                        ; 21C464
1479E8: add      x0,a                                        ; 200040
1479E9: asr      a                                           ; 200022
1479EA: move     #>$10,r0                                    ; 60F400 000010
1479EC: move     r0,r1                                       ; 221100
1479ED: move     a,y1                                        ; 21C700
1479EE: move     x:(r6+$57),a                                ; 0A76CE 000057
1479F0: move     y:(r6+$57),x0                               ; 0B76C4 000057
1479F2: asl      #$4,a,b                                     ; 0C1D09
1479F3: do       #<$10,>$1479f9                              ; 061080 1479F8
1479F5: mac      -x0,y1,a        x:(r0)+,x0      a,y0        ; 1098C6
1479F6: mac      x0,y1,a                                     ; 2000C2
1479F7: mac      -x1,y0,a        b,y:(r1)+                   ; 5F59E6
1479F8: asl      #$4,a,b                                     ; 0C1D09
1479F9: move     x0,y:(r6+$57)                               ; 0B7684 000057
1479FB: move     a,x:(r6+$57)                                ; 0A768E 000057
1479FD: move     y:(r6+$11),r0                               ; 0B76D0 000011
1479FF: move     #$3f,m0                                     ; 053FA0
147A00: move     #$2,n0                                      ; 380200
147A01: move     #>$0,r2                                     ; 62F400 000000
147A03: move     #$1,m2                                      ; 0501A2
147A04: move     y:(r6+$4),b                                 ; 0216BF
147A05: asl      #$1,b,b                                     ; 0C1D83
147A06: move     #$7f,a                                      ; 2E7F00
147A07: tfr      a,b             b,y0                        ; 21E609
147A08: move     y:(r6+$5),y1                                ; 0216F7
147A09: sub      y1,a                                        ; 200074
147A0A: sub      y0,b            a,x1                        ; 21C55C
147A0B: move     #>$64,r4                                    ; 64F400 000064
147A0D: mpy      x1,y0,b         b,x0                        ; 21E4E8
147A0E: mpy      x1,x0,a                                     ; 2000A0
147A0F: move     #>$10,r7                                    ; 67F400 000010
147A11: move     #>$ffffff,m7                                ; 05F427 FFFFFF
147A13: move     a,x:(r2)+                                   ; 565A00
147A14: move     b,x:(r2)+                                   ; 575A00
147A15: move     #>$3e,r5                                    ; 65F400 00003E
147A17: move     x:(r2)+,x0      y:(r4)+,y0                  ; F09A00
147A18: move     y:(r7)+,x1                                  ; 4DDF00
147A19: do       #<$10,>$147a20                              ; 061080 147A1F
147A1B: mpy      y0,x0,a         x:(r2)+,x0      a,y:(r5)+   ; B2BAD0
147A1C: mac      x1,x0,a         x:(r0)+,x0      b,y:(r5)+   ; B3B8A2
147A1D: tfr      a,b             y:(r7)+,x1                  ; 4DDF09
147A1E: mac      x0,y1,a x:(r0)+,x0                          ; 44D8C2
147A1F: mac      x0,y1,b         x:(r2)+,x0      y:(r4)+,y0  ; F09ACA
147A20: move     a,y:(r5)+                                   ; 5E5D00
147A21: move     b,y:(r5)+                                   ; 5F5D00
147A22: move     r0,y:(r6+$11)                               ; 0B7690 000011
147A24: move     #>$ffffff,m0                                ; 05F420 FFFFFF
147A26: move     #>$ffffff,m2                                ; 05F422 FFFFFF
147A28: move     x:>$fe,r0                                   ; 60F000 0000FE
147A2A: move     y:(r6+$b),x0                                ; 022EF4
147A2B: mpy      x0,x0,a                                     ; 200080
147A2C: move     #>$72,r4                                    ; 64F400 000072
147A2E: move     #>$97,r1                                    ; 61F400 000097
147A30: move     #>$aa,r2                                    ; 62F400 0000AA
147A32: move     x:(r0)+,x1                                  ; 45D800
147A33: move     a,x0                                        ; 21C400
147A34: do       #<$10,>$147a3c                              ; 061080 147A3B
147A36: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r4)+   ; B698A0
147A37: move     b,y:(r1)+                                   ; 5F5900
147A38: asl      #$2,a,a                                     ; 0C1D04
147A39: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r4)+   ; B798A8
147A3A: move     a,y:(r2)+                                   ; 5E5A00
147A3B: asl      #$2,b,b                                     ; 0C1D85
147A3C: move     a,y:(r4)+                                   ; 5E5C00
147A3D: move     b,y:(r1)+                                   ; 5F5900
147A3E: move     b,y:(r4)+                                   ; 5F5C00
147A3F: move     #>$0,r7                                     ; 67F400 000000
147A41: move     #$1,m7                                      ; 0501A7
147A42: move     #$34,n6                                     ; 3E3400
147A43: move     #>$96,y0                                    ; 46F400 000096
147A45: move     y0,y:(r7)+                                  ; 4E5F00
147A46: move     #>$a9,y0                                    ; 46F400 0000A9
147A48: move     y0,y:(r7)+                                  ; 4E5F00
147A49: lua      (r6)+n6,r3                                  ; 044E13
147A4A: move     r3,r2                                       ; 227200
147A4B: jsr      func_147ab7                                 ; 0BF080 147AB7
147A4D: move     #>$64,r4                                    ; 64F400 000064
147A4F: move     #>$84,r5                                    ; 65F400 000084
147A51: move     #>$8,x0                                     ; 44F400 000008
147A53: jsr      func_147ad4                                 ; 0BF080 147AD4
147A55: move     #>$10,r0                                    ; 60F400 000010
147A57: move     #>$d,r1                                     ; 61F400 00000D
147A59: move     #>$40,r4                                    ; 64F400 000040
147A5B: move     y:(r6+$7),a                                 ; 021EFE
147A5C: move     #>$ffffff,m7                                ; 05F427 FFFFFF
147A5E: move     #$2,n0                                      ; 380200
147A5F: move     n0,n1                                       ; 231900
147A60: asr      a               a,x1                        ; 21C522
147A61: move     y:(r6+$54),y0                               ; 0B76C6 000054
147A63: move     x:(r6+$54),y1                               ; 0A76C7 000054
147A65: move     a,x0                                        ; 21C400
147A66: do       #<$10,>$147a6e                              ; 061080 147A6D
147A68: mpy      y0,x0,b         b,x:(r1)+n1     y:(r4)+,y0  ; FC09D8
147A69: mac      y0,x0,b                                     ; 2000DA
147A6A: mpy      x1,y0,a         a,x:(r1)+n1                 ; 5649E0
147A6B: mpy      x0,y1,b         b,x:(r0)+n0     y:(r4)+,y1  ; FD08C8
147A6C: mac      x0,y1,b                                     ; 2000CA
147A6D: mpy      y1,x1,a         a,x:(r0)+n0                 ; 5648F0
147A6E: move     b,x:(r1)+n1                                 ; 574900
147A6F: move     a,x:(r1)+                                   ; 565900
147A70: move     y0,y:(r6+$54)                               ; 0B7686 000054
147A72: move     y1,x:(r6+$54)                               ; 0A7687 000054
147A74: move     #$7f,a                                      ; 2E7F00
147A75: sub      x1,a                                        ; 200064
147A76: clr      a               ifmi                        ; 202B13
147A77: move     #>$10,r0                                    ; 60F400 000010
147A79: move     #>$64,r5                                    ; 65F400 000064
147A7B: move     r5,r4                                       ; 22B400
147A7C: move     a,x1                                        ; 21C500
147A7D: move     y:(r5)+,y0                                  ; 4EDD00
147A7E: move     y:(r5)+,y1                                  ; 4FDD00
147A7F: move     x:(r0)+,x0                                  ; 44D800
147A80: move     r6,r2                                       ; 22D200
147A81: move     #>$97,r6                                    ; 66F400 000097
147A83: move     #>$a9,r7                                    ; 67F400 0000A9
147A85: do       #<$10,>$147a8f                              ; 061080 147A8E
147A87: mpy      y0,x0,a         x:(r0)+,x0      a,y:(r6)+   ; B2D8D0
147A88: mac      x1,y0,a         y:(r6),y0                   ; 4EE6E2
147A89: mpy      x0,y1,b         x:(r0)+,x0      b,y:(r7)+   ; B3F8C8
147A8A: mac      y1,x1,b         y:(r7),y1                   ; 4FE7FA
147A8B: mpy      y0,x0,a         x:(r0)+,x0      a,y:(r4)+   ; B298D0
147A8C: mac      x1,y0,a         y:(r5)+,y0                  ; 4EDDE2
147A8D: mpy      x0,y1,b         x:(r0)+,x0      b,y:(r4)+   ; B398C8
147A8E: mac      y1,x1,b         y:(r5)+,y1                  ; 4FDDFA
147A8F: move     a,y:(r6)+                                   ; 5E5E00
147A90: move     b,y:(r7)+                                   ; 5F5F00
147A91: move     r2,r6                                       ; 225600
147A92: move     #>$0,r7                                     ; 67F400 000000
147A94: move     #$1,m7                                      ; 0501A7
147A95: lua      (r6+$14),r3                                 ; 040E43
147A96: move     r3,r2                                       ; 227200
147A97: jsr      func_147ab7                                 ; 0BF080 147AB7
147A99: move     #>$56,r4                                    ; 64F400 000056
147A9B: move     #>$76,r5                                    ; 65F400 000076
147A9D: move     #>$7,x0                                     ; 44F400 000007
147A9F: jsr      func_147ad4                                 ; 0BF080 147AD4
147AA1: move     #>$98,r2                                    ; 62F400 000098
147AA3: move     #>$aa,r3                                    ; 63F400 0000AA
147AA5: move     #>$56,r4                                    ; 64F400 000056
147AA7: move     #$40,x1                                     ; 254000
147AA8: move     #$40,y1                                     ; 274000
147AA9: move     y:(r2)+,x0                                  ; 4CDA00
147AAA: move     #>$100,r7                                   ; 67F400 000100
147AAC: move     #>$ffffff,m7                                ; 05F427 FFFFFF
147AAE: do       #<$10,>$147ab6                              ; 061080 147AB5
147AB0: mpy      x1,x0,a         y:(r4)+,y0                  ; 4EDCA0
147AB1: mac      y1,y0,a         y:(r3)+,x0                  ; 4CDBB2
147AB2: mpy      x1,x0,b         y:(r4)+,y0                  ; 4EDCA8
147AB3: mac      y1,y0,b         y:(r2)+,x0                  ; 4CDABA
147AB4: move     a,y:(r7)+                                   ; 5E5F00
147AB5: move     b,y:(r7)+                                   ; 5F5F00
147AB6: rts                                                  ; 00000C
147AB7: move     #$2,n0                                      ; 380200
147AB8: move     n0,n1                                       ; 231900
147AB9: move     n0,n4                                       ; 231C00
147ABA: move     n0,n5                                       ; 231D00
147ABB: move     #>$349cf2,x0                                ; 44F400 349CF2
147ABD: move     #>$d31fd6,y1                                ; 47F400 D31FD6
147ABF: jsr      func_147ae1                                 ; 0BF080 147AE1
147AC1: move     #>$568ef0,y1                                ; 47F400 568EF0
147AC3: move     #>$1dafa8,x1                                ; 45F400 1DAFA8
147AC5: jsr      func_147b03                                 ; 0BF080 147B03
147AC7: move     #>$75b6,x0                                  ; 44F400 0075B6
147AC9: move     #>$188629,y1                                ; 47F400 188629
147ACB: jsr      func_147ae1                                 ; 0BF080 147AE1
147ACD: move     #>$b39915,y1                                ; 47F400 B39915
147ACF: move     #>$170fca,x1                                ; 45F400 170FCA
147AD1: jsr      func_147b03                                 ; 0BF080 147B03
147AD3: rts                                                  ; 00000C
147AD4: move     x:(r2)+,a                                   ; 56DA00
147AD5: move     x:(r2)+,b                                   ; 57DA00
147AD6: do       x0,>$147ada                                 ; 06C400 147AD9
147AD8: move     x:(r2)+,a       a,y:(r4)+                   ; BA9A00
147AD9: move     x:(r2)+,b       b,y:(r4)+                   ; BF9A00
147ADA: move     y:(r5)+,a                                   ; 5EDD00
147ADB: move     y:(r5)+,b                                   ; 5FDD00
147ADC: do       x0,>$147ae0                                 ; 06C400 147ADF
147ADE: move     a,x:(r3)+       y:(r5)+,a                   ; FA3B00
147ADF: move     b,x:(r3)+       y:(r5)+,b                   ; FF3B00
147AE0: rts                                                  ; 00000C
147AE1: move     y:(r7)+,r4                                  ; 6CDF00
147AE2: move     y:(r7)+,r5                                  ; 6DDF00
147AE3: move     r4,r0                                       ; 229000
147AE4: move     r5,r1                                       ; 22B100
147AE5: move     l:(r2)+,a                                   ; 48DA00
147AE6: move     l:(r2)+,b                                   ; 49DA00
147AE7: move     a,l:(r4)+                                   ; 485C00
147AE8: move     b,l:(r4)-                                   ; 495400
147AE9: move     b1,x1                                       ; 21A500
147AEA: move     l:(r2)+,a                                   ; 48DA00
147AEB: move     l:(r2)+,b                                   ; 49DA00
147AEC: move     b,l:(r5)-                                   ; 495500
147AED: move     a,l:(r5)                                    ; 486500
147AEE: move     y:(r4)+,a                                   ; 5EDC00
147AEF: do       #<$10,>$147af9                              ; 061080 147AF8
147AF1: mac      -x1,x0,a        y:(r4)+,y0                  ; 4EDCA6
147AF2: mac      y0,x0,a         b,x:(r1)-       y:(r4)-,y0  ; EC11D2
147AF3: mac      y1,y0,a         x:(r0)+n0,y0                ; 46C8B2
147AF4: mac      -y1,y0,a        b,x1            y:(r5)+,b   ; 1FDDB6
147AF5: mac      -x1,x0,b        y:(r5)+,y0                  ; 4EDDAE
147AF6: mac      y0,x0,b         a,x:(r0)-       y:(r5)-,y0  ; E830DA
147AF7: mac      y1,y0,b         x:(r1)+n1,y0                ; 46C9BA
147AF8: mac      -y1,y0,b        a,x1            y:(r4)+,a   ; 16DCBE
147AF9: move     l:(r0)+,a                                   ; 48D800
147AFA: move     b,x:(r1)-                                   ; 575100
147AFB: move     l:(r0)+,b                                   ; 49D800
147AFC: move     a,l:(r3)+                                   ; 485B00
147AFD: move     b,l:(r3)+                                   ; 495B00
147AFE: move     l:(r1)+,a                                   ; 48D900
147AFF: move     l:(r1),b                                    ; 49E100
147B00: move     a,l:(r3)+                                   ; 485B00
147B01: move     b,l:(r3)+                                   ; 495B00
147B02: rts                                                  ; 00000C
147B03: move     y:(r7)+,r0                                  ; 68DF00
147B04: move     y:(r7)+,r1                                  ; 69DF00
147B05: move     r0,r4                                       ; 221400
147B06: move     r1,r5                                       ; 223500
147B07: move     l:(r2)+,a                                   ; 48DA00
147B08: move     l:(r2)+,b                                   ; 49DA00
147B09: move     a,l:(r0)+                                   ; 485800
147B0A: move     b,l:(r0)-                                   ; 495000
147B0B: move     b0,y0                                       ; 212600
147B0C: move     l:(r2)+,a                                   ; 48DA00
147B0D: move     l:(r2)+,b                                   ; 49DA00
147B0E: move     b,l:(r1)-                                   ; 495100
147B0F: move     a,l:(r1)                                    ; 486100
147B10: move     b0,b                                        ; 212F00
147B11: move     x:(r0)+,a                                   ; 56D800
147B12: do       #<$10,>$147b1c                              ; 061080 147B1B
147B14: mac      -y1,y0,a        x:(r0)+,x0                  ; 44D8B6
147B15: mac      x0,y1,a         x:(r0)-,x0      b,y:(r5)-   ; A3B0C2
147B16: mac      x1,x0,a         y:(r4)+n4,x0                ; 4CCCA2
147B17: mac      -x1,x0,a        x:(r1)+,b       b,y0        ; 1E99A6
147B18: mac      -y1,y0,b        x:(r1)+,x0                  ; 44D9BE
147B19: mac      x0,y1,b         x:(r1)-,x0      a,y:(r4)-   ; A291CA
147B1A: mac      x1,x0,b         y:(r5)+n5,x0                ; 4CCDAA
147B1B: mac      -x1,x0,b        x:(r0)+,a       a,y0        ; 1898AE
147B1C: move     l:(r4)+,a                                   ; 48DC00
147B1D: move     b,y:(r5)-                                   ; 5F5500
147B1E: move     l:(r4)+,b                                   ; 49DC00
147B1F: move     a,l:(r3)+                                   ; 485B00
147B20: move     b,l:(r3)+                                   ; 495B00
147B21: move     l:(r5)+,a                                   ; 48DD00
147B22: move     l:(r5),b                                    ; 49E500
147B23: move     a,l:(r3)+                                   ; 485B00
147B24: move     b,l:(r3)+                                   ; 495B00
147B25: rts                                                  ; 00000C
147B26: rts                                                  ; 00000C
147B27: move     #$0,x0                                      ; 240000
147B28: move     x0,x:(r6-$19)                               ; 039EC4
147B29: move     y:(r6+$b),x0                                ; 022EF4
147B2A: mpy      x0,x0,a                                     ; 200080
147B2B: move     x:(r0)+,x1                                  ; 45D800
147B2C: move     a,x0                                        ; 21C400
147B2D: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
147B2E: asl      #$2,a,a                                     ; 0C1D04
147B2F: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
147B30: asl      #$2,b,b                                     ; 0C1D85
147B31: do       #<$10,>$147b37                              ; 061080 147B36
147B33: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r7)+   ; B6F8A0
147B34: asl      #$2,a,a                                     ; 0C1D04
147B35: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r7)+   ; B7F8A8
147B36: asl      #$2,b,b                                     ; 0C1D85
147B37: rts                                                  ; 00000C
147B38: clr      a                                           ; 200013
147B39: move     #$32,n6                                     ; 3E3200
147B3A: move     a,x:(r6+$19)                                ; 0266CE
147B3B: move     a,y:(r6+$19)                                ; 0266EE
147B3C: move     a,x:(r6+$29)                                ; 02A6CE
147B3D: move     a,y:(r6+$29)                                ; 02A6EE
147B3E: lua      (r6)+n6,r0                                  ; 044E10
147B3F: rep      #<$5                                        ; 0605A0
147B40: move     a,l:(r0)+                                   ; 485800
147B41: move     #$7e,y0                                     ; 267E00
147B42: move     y:(r6+$4),a                                 ; 0216BE
147B43: and      y0,a                                        ; 200056
147B44: asr      #$6,a,a                                     ; 0C1C0C
147B45: move     y:(r6+$6),b                                 ; 021EBF
147B46: and      y0,b                                        ; 20005E
147B47: asr      #$6,b,b                                     ; 0C1C8D
147B48: move     a1,y:(r6+$1a)                               ; 026EAC
147B49: move     b1,y:(r6+$2a)                               ; 02AEAD
147B4A: move     a1,y:(r6+$1b)                               ; 026EEC
147B4B: move     b1,y:(r6+$2b)                               ; 02AEED
147B4C: move     y:(r6+$5),x1                                ; 0216F5
147B4D: mpyi     #>$408102,x1,a                              ; 0141E0 408102
147B4F: asl      a               #>$7fffff,x0                ; 44F432 7FFFFF
147B51: move     x0,y:(r6+$1c)                               ; 0276A4
147B52: move     x0,y:(r6+$2c)                               ; 02B6A4
147B53: move     a,y:(r6+$37)                                ; 02DEEE
147B54: rts                                                  ; 00000C
147B55: rts                                                  ; 00000C
147B56: move     y:(r6+$9),x0                                ; 0226F4
147B57: mpyri    #>$80,x0,b                                  ; 0141C9 000080
147B59: move     a,l:<$0                                     ; 480000
147B5A: move     b1,r0                                       ; 21B000
147B5B: move     y:(r6+$5),a                                 ; 0216FE
147B5C: asl      a                                           ; 200032
147B5D: add      #>$800000,a                                 ; 0140C0 800000
147B5F: abs      a               #>$7fffff,b                 ; 57F426 7FFFFF
147B61: sub      a,b                                         ; 20001C
147B62: move     x:(r0+$101bfb),a                            ; 0A70CE 101BFB
147B64: move     b,x0                                        ; 21E400
147B65: move     x:(r0+$101cfb),b                            ; 0A70CF 101CFB
147B67: sub      #>$400000,b                                 ; 0140CC 400000
147B69: sub      #>$400000,a                                 ; 0140C4 400000
147B6B: move     b,y0                                        ; 21E600
147B6C: move     a,y1                                        ; 21C700
147B6D: mpy      y0,x0,b                                     ; 2000D8
147B6E: mpy      x0,y1,a                                     ; 2000C0
147B6F: add      #>$400000,b                                 ; 0140C8 400000
147B71: add      #>$400000,a                                 ; 0140C0 400000
147B73: move     b,y1                                        ; 21E700
147B74: move     a,y0                                        ; 21C600
147B75: move     l:<$0,x                                     ; 428000
147B76: mpysu    y0,x0,a                                     ; 012785
147B77: dmac     su y0,x1,a                                  ; 01258E
147B78: asl      a               #$7e,y0                     ; 267E32
147B79: mpysu    y1,x0,b                                     ; 0127AC
147B7A: dmac     su y1,x1,b                                  ; 0125A7
147B7B: asl      b               a,l:<$0                     ; 48003A
147B7C: move     y:(r6+$1a),x0                               ; 026EB4
147B7D: move     y:(r6+$4),a                                 ; 0216BE
147B7E: and      y0,a            b,l:<$1                     ; 490156
147B7F: asr      #$6,a,b                                     ; 0C1C0D
147B80: cmp      x0,b                                        ; 20004D
147B81: beq      func_147b8b                                 ; 05A40A
147B82: move     y:(r6+$1c),a                                ; 0276BE
147B83: cmp      #>$7fff00,a                                 ; 0140C5 7FFF00
147B85: blt      func_147b8b                                 ; 059406
147B86: move     y:(r6+$1a),x0                               ; 026EB4
147B87: move     b,y:(r6+$1a)                                ; 026EAF
147B88: move     x0,y:(r6+$1b)                               ; 026EE4
147B89: move     #$0,x0                                      ; 240000
147B8A: move     x0,y:(r6+$1c)                               ; 0276A4
147B8B: move     y:(r6+$6),b                                 ; 021EBF
147B8C: and      y0,b                                        ; 20005E
147B8D: asr      #$6,b,b                                     ; 0C1C8D
147B8E: move     y:(r6+$2a),x0                               ; 02AEB4
147B8F: cmp      x0,b                                        ; 20004D
147B90: beq      func_147b9a                                 ; 05A40A
147B91: move     y:(r6+$2c),a                                ; 02B6BE
147B92: cmp      #>$7fff00,a                                 ; 0140C5 7FFF00
147B94: blt      func_147b9a                                 ; 059406
147B95: move     y:(r6+$2a),x0                               ; 02AEB4
147B96: move     b,y:(r6+$2a)                                ; 02AEAF
147B97: move     x0,y:(r6+$2b)                               ; 02AEE4
147B98: move     #$0,x0                                      ; 240000
147B99: move     x0,y:(r6+$2c)                               ; 02B6A4
147B9A: move     y:(r6+$2),y0                                ; 020EB6
147B9B: move     #>$7eb852,x1                                ; 45F400 7EB852
147B9D: move     #>$8000,x0                                  ; 44F400 008000
147B9F: move     y:(r6+$7),a                                 ; 021EFE
147BA0: cmp      x0,a            y0,y1                       ; 20C745
147BA1: bgt      func_147ba3                                 ; 057402
147BA2: move     x1,y0                                       ; 20A600
147BA3: asr      #$10,a,a                                    ; 0C1C20
147BA4: move     y:(r6+$7),b                                 ; 021EFF
147BA5: cmp      x0,b            a,r0                        ; 21D04D
147BA6: bgt      func_147ba8                                 ; 057402
147BA7: move     x1,y1                                       ; 20A700
147BA8: asr      #$10,b,b                                    ; 0C1CA1
147BA9: move     #>$21d10,x1                                 ; 45F400 021D10
147BAB: move     b,r1                                        ; 21F100
147BAC: move     x:(r0+$144c49),x0                           ; 0A70C4 144C49
147BAE: mpy      y0,x0,b                                     ; 2000D8
147BAF: move     x:(r1+$144c49),x0                           ; 0A71C4 144C49
147BB1: mpy      x0,y1,a         b1,y1                       ; 21A7C0
147BB2: move     b0,y0                                       ; 212600
147BB3: mpysu    x1,y0,b                                     ; 0127A6
147BB4: dmac     ss x1,y1,b                                  ; 0124AF
147BB5: asl      #$b,b,b                                     ; 0C1D97
147BB6: move     a1,y1                                       ; 218700
147BB7: move     a0,y0                                       ; 210600
147BB8: mpysu    x1,y0,a                                     ; 012786
147BB9: dmac     ss x1,y1,a                                  ; 01248F
147BBA: asl      #$b,a,a                                     ; 0C1D16
147BBB: move     b,x:(r6+$1f)                                ; 027ECF
147BBC: move     b0,y:(r6+$1f)                               ; 027EE9
147BBD: move     a,x:(r6+$2f)                                ; 02BECE
147BBE: move     a0,y:(r6+$2f)                               ; 02BEE8
147BBF: move     #>$147ce0,x1                                ; 45F400 147CE0
147BC1: move     #$14,n0                                     ; 381400
147BC2: lua      (r6+$10),r4                                 ; 040E04
147BC3: move     n0,n3                                       ; 231B00
147BC4: move     #$a,n4                                      ; 3C0A00
147BC5: move     #$f8,r1                                     ; 31F800
147BC6: move     #$0,r2                                      ; 320000
147BC7: do       #<$2,>$147be5                               ; 060280 147BE4
147BC9: move     l:(r2)+,y                                   ; 43DA00
147BCA: move     #>$17c70,x0                                 ; 44F400 017C70
147BCC: mpysu    x0,y0,b                                     ; 0127AD
147BCD: dmac     ss x0,y1,b                                  ; 0124A4
147BCE: clb      b,a                                         ; 0C1E02
147BCF: add      x1,a            #>$1aa17,x0                 ; 44F460 01AA17
147BD1: move     b,l:(r4)                                    ; 496400
147BD2: move     a,r0                                        ; 21D000
147BD3: mpysu    x0,y0,b                                     ; 0127AD
147BD4: dmac     ss x0,y1,b                                  ; 0124A4
147BD5: clb      b,a                                         ; 0C1E02
147BD6: add      x1,a            l:(r4)+,b                   ; 49DC60
147BD7: move     x:(r0)+n0,y0                                ; 46C800
147BD8: move     a,r3                                        ; 21D300
147BD9: asr      y0,b,a                                      ; 0C1E7A
147BDA: move     x:(r0)+n0,x0    y0,y:(r4)+                  ; B08800
147BDB: move     a,l:(r1)+                                   ; 485900
147BDC: move     x0,y:(r4)+                                  ; 4C5C00
147BDD: move     x:(r0),y0                                   ; 46E000
147BDE: move     x:(r3)+n3,x0    y0,y:(r4)+                  ; B08B00
147BDF: move     x:(r3)+n3,a                                 ; 56CB00
147BE0: asr      x0,b,b                                      ; 0C1E79
147BE1: move     x0,y:(r4)+                                  ; 4C5C00
147BE2: move     x:(r3),x0       a,y:(r4)+                   ; B28300
147BE3: move     x0,y:(r4)+n4                                ; 4C4C00
147BE4: move     b,l:(r1)+                                   ; 495900
147BE5: move     #>$1407ff,r3                                ; 63F400 1407FF
147BE7: move     y:(r6+$8),x1                                ; 0226B5
147BE8: mpyri    #>$6000,x1,a                                ; 0141E1 006000
147BEA: asr      #$b,a,b                                     ; 0C1C17
147BEB: and      #>$7ff,a                                    ; 0140C6 0007FF
147BED: neg      a               a1,r2                       ; 219236
147BEE: move     b1,x0                                       ; 21A400
147BEF: move     a1,n3                                       ; 219B00
147BF0: move     x:(r2+$140000),a                            ; 0A72CE 140000
147BF2: asr      #$d,a,a                                     ; 0C1C1A
147BF3: asl      x0,a,a                                      ; 0C1E48
147BF4: move     x:(r3+n3),b                                 ; 57EB00
147BF5: asr      #$8,b,b                                     ; 0C1C91
147BF6: asr      x0,b,b                                      ; 0C1E79
147BF7: move     a1,x:(r6+$30)                               ; 02C68C
147BF8: move     a0,y:(r6+$30)                               ; 02C6A8
147BF9: move     b1,x:(r6+$31)                               ; 02C6CD
147BFA: move     b0,y:(r6+$31)                               ; 02C6E9
147BFB: lua      (r6+$10),r7                                 ; 040E07
147BFC: move     #$f8,n7                                     ; 3FF800
147BFD: move     #$60,r1                                     ; 316000
147BFE: move     #$81,n3                                     ; 3B8100
147BFF: bsr      func_147c50                                 ; 050891
147C00: move     l:?:>$f8,a                                  ; 48F000 0000F8
147C02: move     #$61,r0                                     ; 306100
147C03: move     #$82,r1                                     ; 318200
147C04: move     #$80,r4                                     ; 348000
147C05: bsr      func_147c3e                                 ; 050859
147C06: move     #>$1407ff,r3                                ; 63F400 1407FF
147C08: move     y:(r6+$a),x1                                ; 022EB5
147C09: mpyri    #>$6000,x1,a                                ; 0141E1 006000
147C0B: asr      #$b,a,b                                     ; 0C1C17
147C0C: and      #>$7ff,a                                    ; 0140C6 0007FF
147C0E: neg      a               a1,r2                       ; 219236
147C0F: move     b1,x0                                       ; 21A400
147C10: move     a1,n3                                       ; 219B00
147C11: move     x:(r2+$140000),a                            ; 0A72CE 140000
147C13: asr      #$d,a,a                                     ; 0C1C1A
147C14: asl      x0,a,a                                      ; 0C1E48
147C15: move     x:(r3+n3),b                                 ; 57EB00
147C16: asr      #$8,b,b                                     ; 0C1C91
147C17: asr      x0,b,b                                      ; 0C1E79
147C18: move     a1,x:(r6+$30)                               ; 02C68C
147C19: move     a0,y:(r6+$30)                               ; 02C6A8
147C1A: move     b1,x:(r6+$31)                               ; 02C6CD
147C1B: move     b0,y:(r6+$31)                               ; 02C6E9
147C1C: lua      (r6+$20),r7                                 ; 041607
147C1D: move     #$fa,n7                                     ; 3FFA00
147C1E: move     #$80,r1                                     ; 318000
147C1F: move     #$a1,n3                                     ; 3BA100
147C20: bsr      func_147c50                                 ; 050850
147C21: move     l:?:>$fa,a                                  ; 48F000 0000FA
147C23: move     #$81,r0                                     ; 308100
147C24: move     #$a2,r1                                     ; 31A200
147C25: move     #$1f,r4                                     ; 341F00
147C26: bsr      func_147c3e                                 ; 050818
147C27: move     #$0,r0                                      ; 300000
147C28: move     y:(r6+$5),x1                                ; 0216F5
147C29: mpyi     #>$2a9296,x1,a                              ; 0141E0 2A9296
147C2B: asl      a               #$1f,r7                     ; 371F32
147C2C: move     y:(r6+$37),x0                               ; 02DEF4
147C2D: move     a,y:(r6+$37)                                ; 02DEEE
147C2E: sub      x0,a            #$20,r4                     ; 342044
147C2F: asr      #$5,a,a                                     ; 0C1C0A
147C30: move     #$81,r5                                     ; 358100
147C31: tfr      x0,b            a,y1                        ; 21C749
147C32: move     #>$547ae1,x0                                ; 44F400 547AE1
147C34: do       #<$20,>$147c3b                              ; 062080 147C3A
147C36: add      y1,b            b,x1            y:(r4)+,y0  ; 1CDC78
147C37: mpy      x1,y0,a         a,x:(r0)        y:(r5)+,y0  ; F820E0
147C38: mac      -x1,y0,a        x:(r0),x1                   ; 45E0E6
147C39: mac      y0,x0,a         x1,y:(r7)+                  ; 4D5FD2
147C3A: asl      #$2,a,a                                     ; 0C1D04
147C3B: lua      (r6+$32),r6                                 ; 041E26
147C3C: move     a,y:(r7)+                                   ; 5E5F00
147C3D: jmp      func_000458                                 ; 0C0458
147C3E: clb      a,b                                         ; 0C1E01
147C3F: normf    b1,a                                        ; 0C1E26
147C40: sub      #>$780000,a                                 ; 0140C4 780000
147C42: clr      a               iflt                        ; 202913
147C43: asl      #$4,a,a                                     ; 0C1D08
147C44: move     #>$7fffff,b                                 ; 57F400 7FFFFF
147C46: sub      a,b             a,x1            y:(r4),a    ; 16E41C
147C47: move     x:(r0)+,x0      b,y1                        ; 139800
147C48: do       #<$10,>$147c4e                              ; 061080 147C4D
147C4A: mpy      x0,y1,b         x:(r1)+,x0                  ; 44D9C8
147C4B: mac      x1,x0,b         x:(r0)+,x0      a,y:(r4)+   ; B298AA
147C4C: mpy      x0,y1,a         x:(r1)+,x0                  ; 44D9C0
147C4D: mac      x1,x0,a         x:(r0)+,x0      b,y:(r4)+   ; B398A2
147C4E: move     a,y:(r4)+                                   ; 5E5C00
147C4F: rts                                                  ; 00000C
147C50: move     y:(r7+$a),b                                 ; 022FBF
147C51: move     y:(r7+$b),a                                 ; 022FFE
147C52: move     y:(r7+$2),x0                                ; 020FB4
147C53: add      x0,b            #$0,r2                      ; 320048
147C54: add      x0,a            b1,x1                       ; 21A540
147C55: move     x:(r7+$9),b                                 ; 0227DF
147C56: move     y:(r7+$9),b0                                ; 0227F9
147C57: move     y:(r7+$1),x0                                ; 0207F4
147C58: asr      x0,b,b                                      ; 0C1E79
147C59: move     y:(r7+$3),x0                                ; 020FF4
147C5A: move     #>$0,y1                                     ; 47F400 000000
147C5C: add      x0,b            #$80,y0                     ; 268048
147C5D: sub      y,b             n7,r0                       ; 23F03C
147C5E: and      x0,b            a1,y1                       ; 21874E
147C5F: add      x1,b                                        ; 200068
147C60: sub      x1,b            b1,r4                       ; 21B46C
147C61: add      y1,b            l:(r0),a                    ; 48E078
147C62: move     b1,r5                                       ; 21B500
147C63: move     #>$20,b1                                    ; 55F400 000020
147C65: add      a,b             b,l:(r2)                    ; 496218
147C66: asl      a                                           ; 200032
147C67: move     a1,y1                                       ; 218700
147C68: move     a0,y0                                       ; 210600
147C69: move     l:(r2),a                                    ; 48E200
147C6A: do       #<$10,>$147c6e                              ; 061080 147C6D
147C6C: add      y,a             a,l:(r2)+                   ; 485A30
147C6D: add      y,b             b,l:(r2)+                   ; 495A38
147C6E: move     l:(r0),b                                    ; 49E000
147C6F: asl      #$17,b,b                                    ; 0C1DAF
147C70: move     #$4,a                                       ; 2E0400
147C71: move     b,x0                                        ; 21E400
147C72: andi     #$fe,ccr                                    ; 00FEB9
147C73: do       #<$18,>$147c76                              ; 061880 147C75
147C75: div      x0,a                                        ; 018040
147C76: move     y:(r7+$3),m4                                ; 0B77E4 000003
147C78: move     m4,m5                                       ; 0465A4
147C79: move     a0,x0                                       ; 210400
147C7A: move     x:(r7+$f),y1                                ; 023FD7
147C7B: move     y:(r7+$f),y0                                ; 023FF6
147C7C: mpysu    x0,y0,a                                     ; 01278D
147C7D: dmac     ss x0,y1,a                                  ; 012484
147C7E: move     y:(r7+$c),b                                 ; 0237BF
147C7F: move     a,x1                                        ; 21C500
147C80: move     #$25,n2                                     ; 3A2500
147C81: jsr      func_0003ec                                 ; 0D03EC
147C82: move     y:(r7+$a),b                                 ; 022FBF
147C83: move     y:(r7+$b),a                                 ; 022FFE
147C84: move     y:(r7+$5),x0                                ; 0217F4
147C85: add      x0,b            #$0,r1                      ; 310048
147C86: add      x0,a            b1,x1                       ; 21A540
147C87: move     x:(r7+$9),b                                 ; 0227DF
147C88: move     y:(r7+$9),b0                                ; 0227F9
147C89: move     y:(r7+$4),x0                                ; 0217B4
147C8A: asr      x0,b,b                                      ; 0C1E79
147C8B: move     y:(r7+$6),x0                                ; 021FB4
147C8C: move     #>$0,y1                                     ; 47F400 000000
147C8E: add      x0,b            #$88,y0                     ; 268848
147C8F: sub      y,b             n7,r0                       ; 23F03C
147C90: and      x0,b            a1,y1                       ; 21874E
147C91: add      x1,b                                        ; 200068
147C92: sub      x1,b            b1,r4                       ; 21B46C
147C93: add      y1,b            (r0)+                       ; 205878
147C94: move     l:(r0),a                                    ; 48E000
147C95: move     b1,r5                                       ; 21B500
147C96: move     #>$20,b1                                    ; 55F400 000020
147C98: add      a,b             b,l:(r1)                    ; 496118
147C99: asl      a                                           ; 200032
147C9A: move     a1,y1                                       ; 218700
147C9B: move     a0,y0                                       ; 210600
147C9C: move     l:(r1),a                                    ; 48E100
147C9D: do       #<$10,>$147ca1                              ; 061080 147CA0
147C9F: add      y,a             a,l:(r1)+                   ; 485930
147CA0: add      y,b             b,l:(r1)+                   ; 495938
147CA1: move     l:(r0),b                                    ; 49E000
147CA2: move     n3,r1                                       ; 237100
147CA3: asl      #$17,b,b                                    ; 0C1DAF
147CA4: move     #$4,a                                       ; 2E0400
147CA5: move     b,x0                                        ; 21E400
147CA6: andi     #$fe,ccr                                    ; 00FEB9
147CA7: do       #<$18,>$147caa                              ; 061880 147CA9
147CA9: div      x0,a                                        ; 018040
147CAA: move     y:(r7+$6),m4                                ; 0B77E4 000006
147CAC: move     m4,m5                                       ; 0465A4
147CAD: move     a0,x0                                       ; 210400
147CAE: move     x:(r7+$f),y1                                ; 023FD7
147CAF: move     y:(r7+$f),y0                                ; 023FF6
147CB0: mpysu    x0,y0,a                                     ; 01278D
147CB1: dmac     ss x0,y1,a                                  ; 012484
147CB2: move     y:(r7+$c),b                                 ; 0237BF
147CB3: move     a,x1                                        ; 21C500
147CB4: move     #$16,n2                                     ; 3A1600
147CB5: jsr      func_0003ec                                 ; 0D03EC
147CB6: move     n7,r0                                       ; 23F000
147CB7: move     y:(r7+$c),b                                 ; 0237BF
147CB8: move     x:(r7+$f),a                                 ; 023FDE
147CB9: move     y:(r7+$f),a0                                ; 023FF8
147CBA: add      a,b             l:(r7),a                    ; 48E718
147CBB: asl      #$5,a,a                                     ; 0C1D0A
147CBC: move     b,y:(r7+$c)                                 ; 0237AF
147CBD: move     x:(r7+$9),b                                 ; 0227DF
147CBE: move     y:(r7+$9),b0                                ; 0227F9
147CBF: add      a,b                                         ; 200018
147CC0: and      #>$3ff,b                                    ; 0140CE 0003FF
147CC2: move     b1,x:(r7+$9)                                ; 0227CD
147CC3: move     b0,y:(r7+$9)                                ; 0227E9
147CC4: rts                                                  ; 00000C
147CC5: nop                                                  ; 000000
147CC6: nop                                                  ; 000000
147CC7: nop                                                  ; 000000
147CC8: nop                                                  ; 000000
147CC9: nop                                                  ; 000000
147CCA: pflushun                                             ; 000001
147CCB: pfree                                                ; 000002
147CCC: pflush                                               ; 000003
147CCD: rti                                                  ; 000004
147CCE: illegal                                              ; 000005
147CCF: trap                                                 ; 000006
147CD0: dc       $000007                                     ; 000007
147CD1: inc      a                                           ; 000008
147CD2: inc      a                                           ; 000008
147CD3: inc      a                                           ; 000008
147CD4: inc      a                                           ; 000008
147CD5: inc      a                                           ; 000008
147CD6: inc      a                                           ; 000008
147CD7: inc      a                                           ; 000008
147CD8: inc      a                                           ; 000008
147CD9: move     x1,x:(r0)-n0    a,y1                        ; 150000
147CDA: move     x1,x:(r0)-n0    a,y1                        ; 150000
147CDB: move     x1,x:(r0)-n0    a,y1                        ; 150000
147CDC: move     x1,x:(r0)-n0    a,y1                        ; 150000
147CDD: move     x1,x:(r0)-n0    a,y1                        ; 150000
147CDE: move     x1,x:(r4)-n4    a,y1                        ; 150400
147CDF: move     x1,x:(r6)-n6    a,y1                        ; 150600
147CE0: move     x1,x:(r7)-n7    a,y1                        ; 150700
147CE1: mpy      x0,x0,a         x1,x:(r7)-n7    a,y1        ; 150780
147CE2: mpy      x0,y1,a         x1,x:(r7)-n7    a,y1        ; 1507C0
147CE3: mpy      x1,y0,a         x1,x:(r7)-n7    a,y1        ; 1507E0
147CE4: mpy      y1,x1,a         x1,x:(r7)-n7    a,y1        ; 1507F0
147CE5: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CE6: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CE7: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CE8: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CE9: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CEA: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CEB: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CEC: mpy      y1,x1,b         x1,x:(r7)-n7    a,y1        ; 1507F8
147CED: dc       $0003ff                                     ; 0003FF
147CEE: dc       $0003ff                                     ; 0003FF
147CEF: dc       $0003ff                                     ; 0003FF
147CF0: dc       $0003ff                                     ; 0003FF
147CF1: dc       $0003ff                                     ; 0003FF
147CF2: dc       $0001ff                                     ; 0001FF
147CF3: dc       $0000ff                                     ; 0000FF
147CF4: dc       $00007f                                     ; 00007F
147CF5: dc       $00003f                                     ; 00003F
147CF6: traple                                               ; 00001F
147CF7: plockr   >*+$7                                       ; 00000F 000007
147CF9: pflush                                               ; 000003
147CFA: pflush                                               ; 000003
147CFB: pflush                                               ; 000003
147CFC: pflush                                               ; 000003
147CFD: pflush                                               ; 000003
147CFE: pflush                                               ; 000003
147CFF: pflush                                               ; 000003
147D00: pflush                                               ; 000003
147D01: clr      a                                           ; 200013
147D02: move     #$3c,n6                                     ; 3E3C00
147D03: move     a,x:(r6+$14)                                ; 02568E
147D04: move     a,y:(r6+$14)                                ; 0256AE
147D05: move     a,x:(r6+$1c)                                ; 02768E
147D06: move     a,y:(r6+$1c)                                ; 0276AE
147D07: move     a,x:(r6+$24)                                ; 02968E
147D08: move     a,y:(r6+$24)                                ; 0296AE
147D09: move     a,x:(r6+$2c)                                ; 02B68E
147D0A: move     a,y:(r6+$2c)                                ; 02B6AE
147D0B: lua      (r6)+n6,r0                                  ; 044E10
147D0C: rep      #<$5                                        ; 0605A0
147D0D: move     a,l:(r0)+                                   ; 485800
147D0E: move     a,x:(r6+$31)                                ; 02C6CE
147D0F: move     a,y:(r6+$34)                                ; 02D6AE
147D10: move     a,y:(r6+$32)                                ; 02CEAE
147D11: move     #>$114000,a0                                ; 50F400 114000
147D13: move     y:>$124,x1                                  ; 4DF000 000124
147D15: maci     #>$2000,x1,a                                ; 0141E2 002000
147D17: move     #$80,x0                                     ; 248000
147D18: move     a0,y:(r6+$36)                               ; 02DEA8
147D19: move     x0,x:(r6+$30)                               ; 02C684
147D1A: move     #$0,x0                                      ; 240000
147D1B: rts                                                  ; 00000C
147D1C: rts                                                  ; 00000C
147D1D: move     #$28,x0                                     ; 242800
147D1E: move     x0,x:(r6-$1d)                               ; 038EC4
147D1F: move     a,l:<$0                                     ; 480000
147D20: move     #$1,r1                                      ; 310100
147D21: lua      (r6+$4),r4                                  ; 040644
147D22: move     #>$101afb,r3                                ; 63F400 101AFB
147D24: move     l:<$0,y                                     ; 438000
147D25: move     #>$80,x1                                    ; 45F400 000080
147D27: do       #<$3,>$147d31                               ; 060380 147D30
147D29: move     y:(r4)+,x0                                  ; 4CDC00
147D2A: mpy      x1,x0,b                                     ; 2000A8
147D2B: move     b1,n3                                       ; 21BB00
147D2C: move     x:(r3+n3),x0                                ; 44EB00
147D2D: mpysu    x0,y0,b                                     ; 0127AD
147D2E: dmac     ss x0,y1,b                                  ; 0124A4
147D2F: asl      #$2,b,b                                     ; 0C1D85
147D30: move     b,l:(r1)+                                   ; 495900
147D31: move     #$0,r0                                      ; 300000
147D32: lua      (r6+$39),r4                                 ; 041E94
147D33: move     #>$510625,y0                                ; 46F400 510625
147D35: move     #>$4e20,x0                                  ; 44F400 004E20
147D37: move     x:(r0)+,x1                                  ; 45D800
147D38: do       #<$3,>$147d40                               ; 060380 147D3F
147D3A: tfr      y0,a            x:(r0)+,b                   ; 57D851
147D3B: cmp      x0,b                                        ; 20004D
147D3C: clr      a               ifge                        ; 202113
147D3D: cmp      x1,b                                        ; 20006D
147D3E: clr      a               ifeq                        ; 202A13
147D3F: move     a,y:(r4)+                                   ; 5E5C00
147D40: move     y:(r6+$7),b                                 ; 021EFF
147D41: and      #>$7e0000,b                                 ; 0140CE 7E0000
147D43: asr      #$6,b,b                                     ; 0C1C8D
147D44: move     #>$147ce0,x1                                ; 45F400 147CE0
147D46: move     b1,y:(r6+$15)                               ; 0256ED
147D47: move     #$14,n0                                     ; 381400
147D48: lua      (r6+$10),r4                                 ; 040E04
147D49: move     n0,n3                                       ; 231B00
147D4A: move     #$5,n4                                      ; 3C0500
147D4B: move     #$f8,r1                                     ; 31F800
147D4C: move     #$0,r2                                      ; 320000
147D4D: do       #<$4,>$147d5f                               ; 060480 147D5E
147D4F: move     l:(r2)+,y                                   ; 43DA00
147D50: move     #>$17c70,x0                                 ; 44F400 017C70
147D52: mpysu    x0,y0,b                                     ; 0127AD
147D53: dmac     ss x0,y1,b                                  ; 0124A4
147D54: clb      b,a                                         ; 0C1E02
147D55: add      x1,a                                        ; 200060
147D56: move     b,l:(r4)+                                   ; 495C00
147D57: move     a,r0                                        ; 21D000
147D58: move     x:(r0)+n0,y0                                ; 46C800
147D59: asr      y0,b,a                                      ; 0C1E7A
147D5A: move     x:(r0)+n0,x0    y0,y:(r4)+                  ; B08800
147D5B: move     a,l:(r1)+                                   ; 485900
147D5C: move     x0,y:(r4)+                                  ; 4C5C00
147D5D: move     x:(r0),y0                                   ; 46E000
147D5E: move     y0,y:(r4)+n4                                ; 4E4C00
147D5F: lua      (r6+$10),r7                                 ; 040E07
147D60: move     #$f8,r0                                     ; 30F800
147D61: move     #$60,r1                                     ; 316000
147D62: bsr      func_147d8b                                 ; 050849
147D63: lua      (r6+$18),r7                                 ; 040E87
147D64: move     #$f9,r0                                     ; 30F900
147D65: move     #$81,r1                                     ; 318100
147D66: bsr      func_147d8b                                 ; 050845
147D67: lua      (r6+$20),r7                                 ; 041607
147D68: move     #$fa,r0                                     ; 30FA00
147D69: move     #$a2,r1                                     ; 31A200
147D6A: bsr      func_147d8b                                 ; 050841
147D6B: lua      (r6+$28),r7                                 ; 041687
147D6C: move     #$fb,r0                                     ; 30FB00
147D6D: move     #$c3,r1                                     ; 31C300
147D6E: bsr      func_147d8b                                 ; 05081D
147D6F: lua      (r6+$39),r4                                 ; 041E94
147D70: move     #>$fffffe,n4                                ; 74F400 FFFFFE
147D72: move     #$61,r0                                     ; 306100
147D73: move     #$82,r1                                     ; 318200
147D74: move     #$a3,r2                                     ; 32A300
147D75: move     #$c4,r3                                     ; 33C400
147D76: move     #$1f,r7                                     ; 371F00
147D77: move     #>$510625,x1                                ; 45F400 510625
147D79: move     x:(r0)+,x0                                  ; 44D800
147D7A: move     y:(r7),b                                    ; 5FE700
147D7B: do       #<$10,>$147d85                              ; 061080 147D84
147D7D: mpy      x1,x0,a         x:(r1)+,x0      y:(r4)+,y0  ; F099A0
147D7E: mac      y0,x0,a         x:(r2)+,x0      y:(r4)+,y0  ; F09AD2
147D7F: mac      y0,x0,a         x:(r3)+,x0      y:(r4)+n4,y0 ; D09BD2
147D80: mac      y0,x0,a         x:(r0)+,x0      b,y:(r7)+   ; B3F8D2
147D81: mpy      x1,x0,b         x:(r1)+,x0      y:(r4)+,y0  ; F099A8
147D82: mac      y0,x0,b         x:(r2)+,x0      y:(r4)+,y0  ; F09ADA
147D83: mac      y0,x0,b         x:(r3)+,x0      y:(r4)+n4,y0 ; D09BDA
147D84: mac      y0,x0,b         x:(r0)+,x0      a,y:(r7)+   ; B2F8DA
147D85: move     #>$145c48,x0                                ; 44F400 145C48
147D87: move     x0,x:(r6-$1c)                               ; 039684
147D88: lua      (r6+$3c),r6                                 ; 041EC6
147D89: move     b,y:(r7)+                                   ; 5F5F00
147D8A: jmp      func_000458                                 ; 0C0458
147D8B: move     y:(r6+$15),a                                ; 0256FE
147D8C: move     y:(r7+$2),x0                                ; 020FB4
147D8D: add      x0,a            #$0,r2                      ; 320040
147D8E: move     x:(r7+$4),b                                 ; 02179F
147D8F: move     y:(r7+$4),b0                                ; 0217B9
147D90: move     y:(r7+$1),x0                                ; 0207F4
147D91: asr      x0,b,b                                      ; 0C1E79
147D92: move     y:(r7+$3),x0                                ; 020FF4
147D93: move     #>$0,y1                                     ; 47F400 000000
147D95: add      x0,b            #$80,y0                     ; 268048
147D96: sub      y,b             a,x1                        ; 21C53C
147D97: and      x0,b                                        ; 20004E
147D98: move     b1,a                                        ; 21AE00
147D99: sub      x0,a                                        ; 200044
147D9A: sub      #>$1,a                                      ; 0140C4 000001
147D9C: asl      a                                           ; 200032
147D9D: add      x1,a                                        ; 200060
147D9E: add      x1,b            a1,r5                       ; 219568
147D9F: move     l:(r0),a                                    ; 48E000
147DA0: move     b1,r4                                       ; 21B400
147DA1: move     #>$20,b1                                    ; 55F400 000020
147DA3: add      a,b             b,l:(r2)                    ; 496218
147DA4: asl      a                                           ; 200032
147DA5: move     a1,y1                                       ; 218700
147DA6: move     a0,y0                                       ; 210600
147DA7: move     l:(r2),a                                    ; 48E200
147DA8: do       #<$10,>$147dac                              ; 061080 147DAB
147DAA: add      y,a             a,l:(r2)+                   ; 485A30
147DAB: add      y,b             b,l:(r2)+                   ; 495A38
147DAC: move     y:(r7+$3),m4                                ; 0B77E4 000003
147DAE: move     m4,b                                        ; 044FA4
147DAF: asl      b               l:(r0),a                    ; 48E03A
147DB0: add      #>$1,b                                      ; 0140C8 000001
147DB2: move     b1,m5                                       ; 04CDA5
147DB3: clb      a,b                                         ; 0C1E01
147DB4: normf    b1,a                                        ; 0C1E26
147DB5: move     y:(r7+$1),b                                 ; 0207FF
147DB6: tst      b                                           ; 20000B
147DB7: clr      a               ifeq                        ; 202A13
147DB8: sub      #>$780000,a                                 ; 0140C4 780000
147DBA: clr      a               ifmi                        ; 202B13
147DBB: asl      #$4,a,a                                     ; 0C1D08
147DBC: move     #>$7fffff,b                                 ; 57F400 7FFFFF
147DBE: sub      a,b             a,y1                        ; 21C71C
147DBF: clr      b               ifmi                        ; 202B1B
147DC0: move     #$24,n2                                     ; 3A2400
147DC1: move     b,x0                                        ; 21E400
147DC2: jsr      func_000408                                 ; 0D0408
147DC3: move     x:(r7+$4),b                                 ; 02179F
147DC4: move     y:(r7+$4),b0                                ; 0217B9
147DC5: move     l:(r7),a                                    ; 48E700
147DC6: asl      #$5,a,a                                     ; 0C1D0A
147DC7: add      a,b                                         ; 200018
147DC8: and      #>$3ff,b                                    ; 0140CE 0003FF
147DCA: move     b1,x:(r7+$4)                                ; 02178D
147DCB: move     b0,y:(r7+$4)                                ; 0217A9
147DCC: rts                                                  ; 00000C
147DCD: clr      a                                           ; 200013
147DCE: move     #$32,n6                                     ; 3E3200
147DCF: move     a,x:(r6+$19)                                ; 0266CE
147DD0: move     a,y:(r6+$19)                                ; 0266EE
147DD1: move     a,x:(r6+$29)                                ; 02A6CE
147DD2: move     a,y:(r6+$29)                                ; 02A6EE
147DD3: lua      (r6)+n6,r0                                  ; 044E10
147DD4: rep      #<$5                                        ; 0605A0
147DD5: move     a,l:(r0)+                                   ; 485800
147DD6: move     #$7e,y0                                     ; 267E00
147DD7: move     y:(r6+$4),a                                 ; 0216BE
147DD8: and      y0,a                                        ; 200056
147DD9: asr      #$6,a,a                                     ; 0C1C0C
147DDA: move     y:(r6+$6),b                                 ; 021EBF
147DDB: and      y0,b                                        ; 20005E
147DDC: asr      #$6,b,b                                     ; 0C1C8D
147DDD: move     a1,y:(r6+$1a)                               ; 026EAC
147DDE: move     b1,y:(r6+$2a)                               ; 02AEAD
147DDF: move     a1,y:(r6+$1b)                               ; 026EEC
147DE0: move     b1,y:(r6+$2b)                               ; 02AEED
147DE1: move     y:(r6+$5),x1                                ; 0216F5
147DE2: mpyi     #>$408102,x1,a                              ; 0141E0 408102
147DE4: asl      a               #>$7fffff,x0                ; 44F432 7FFFFF
147DE6: move     x0,y:(r6+$1c)                               ; 0276A4
147DE7: move     x0,y:(r6+$2c)                               ; 02B6A4
147DE8: move     a,y:(r6+$37)                                ; 02DEEE
147DE9: rts                                                  ; 00000C
147DEA: rts                                                  ; 00000C
147DEB: move     y:(r6+$9),x0                                ; 0226F4
147DEC: mpyri    #>$80,x0,b                                  ; 0141C9 000080
147DEE: move     a,l:<$0                                     ; 480000
147DEF: move     b1,r0                                       ; 21B000
147DF0: move     y:(r6+$5),a                                 ; 0216FE
147DF1: asl      a                                           ; 200032
147DF2: add      #>$800000,a                                 ; 0140C0 800000
147DF4: abs      a               #>$7fffff,b                 ; 57F426 7FFFFF
147DF6: sub      a,b                                         ; 20001C
147DF7: move     x:(r0+$101bfb),a                            ; 0A70CE 101BFB
147DF9: move     b,x0                                        ; 21E400
147DFA: move     x:(r0+$101cfb),b                            ; 0A70CF 101CFB
147DFC: sub      #>$400000,b                                 ; 0140CC 400000
147DFE: sub      #>$400000,a                                 ; 0140C4 400000
147E00: move     b,y0                                        ; 21E600
147E01: move     a,y1                                        ; 21C700
147E02: mpy      y0,x0,b                                     ; 2000D8
147E03: mpy      x0,y1,a                                     ; 2000C0
147E04: add      #>$400000,b                                 ; 0140C8 400000
147E06: add      #>$400000,a                                 ; 0140C0 400000
147E08: move     b,y1                                        ; 21E700
147E09: move     a,y0                                        ; 21C600
147E0A: move     l:<$0,x                                     ; 428000
147E0B: mpysu    y0,x0,a                                     ; 012785
147E0C: dmac     su y0,x1,a                                  ; 01258E
147E0D: asl      a               #$7e,y0                     ; 267E32
147E0E: mpysu    y1,x0,b                                     ; 0127AC
147E0F: dmac     su y1,x1,b                                  ; 0125A7
147E10: asl      b               a,l:<$0                     ; 48003A
147E11: move     y:(r6+$1a),x0                               ; 026EB4
147E12: move     y:(r6+$4),a                                 ; 0216BE
147E13: and      y0,a            b,l:<$1                     ; 490156
147E14: asr      #$6,a,b                                     ; 0C1C0D
147E15: cmp      x0,b                                        ; 20004D
147E16: beq      func_147e20                                 ; 05A40A
147E17: move     y:(r6+$1c),a                                ; 0276BE
147E18: cmp      #>$7fff00,a                                 ; 0140C5 7FFF00
147E1A: blt      func_147e20                                 ; 059406
147E1B: move     y:(r6+$1a),x0                               ; 026EB4
147E1C: move     b,y:(r6+$1a)                                ; 026EAF
147E1D: move     x0,y:(r6+$1b)                               ; 026EE4
147E1E: move     #$0,x0                                      ; 240000
147E1F: move     x0,y:(r6+$1c)                               ; 0276A4
147E20: move     y:(r6+$6),b                                 ; 021EBF
147E21: and      y0,b                                        ; 20005E
147E22: asr      #$6,b,b                                     ; 0C1C8D
147E23: move     y:(r6+$2a),x0                               ; 02AEB4
147E24: cmp      x0,b                                        ; 20004D
147E25: beq      func_147e2f                                 ; 05A40A
147E26: move     y:(r6+$2c),a                                ; 02B6BE
147E27: cmp      #>$7fff00,a                                 ; 0140C5 7FFF00
147E29: blt      func_147e2f                                 ; 059406
147E2A: move     y:(r6+$2a),x0                               ; 02AEB4
147E2B: move     b,y:(r6+$2a)                                ; 02AEAF
147E2C: move     x0,y:(r6+$2b)                               ; 02AEE4
147E2D: move     #$0,x0                                      ; 240000
147E2E: move     x0,y:(r6+$2c)                               ; 02B6A4
147E2F: move     y:(r6+$2),y0                                ; 020EB6
147E30: move     #>$7eb852,x1                                ; 45F400 7EB852
147E32: move     #>$8000,x0                                  ; 44F400 008000
147E34: move     y:(r6+$7),a                                 ; 021EFE
147E35: cmp      x0,a            y0,y1                       ; 20C745
147E36: bgt      func_147e38                                 ; 057402
147E37: move     x1,y0                                       ; 20A600
147E38: asr      #$10,a,a                                    ; 0C1C20
147E39: move     y:(r6+$7),b                                 ; 021EFF
147E3A: cmp      x0,b            a,r0                        ; 21D04D
147E3B: bgt      func_147e3d                                 ; 057402
147E3C: move     x1,y1                                       ; 20A700
147E3D: asr      #$10,b,b                                    ; 0C1CA1
147E3E: move     #>$21d10,x1                                 ; 45F400 021D10
147E40: move     b,r1                                        ; 21F100
147E41: move     x:(r0+$144c49),x0                           ; 0A70C4 144C49
147E43: mpy      y0,x0,b                                     ; 2000D8
147E44: move     x:(r1+$144c49),x0                           ; 0A71C4 144C49
147E46: mpy      x0,y1,a         b1,y1                       ; 21A7C0
147E47: move     b0,y0                                       ; 212600
147E48: mpysu    x1,y0,b                                     ; 0127A6
147E49: dmac     ss x1,y1,b                                  ; 0124AF
147E4A: asl      #$b,b,b                                     ; 0C1D97
147E4B: move     a1,y1                                       ; 218700
147E4C: move     a0,y0                                       ; 210600
147E4D: mpysu    x1,y0,a                                     ; 012786
147E4E: dmac     ss x1,y1,a                                  ; 01248F
147E4F: asl      #$b,a,a                                     ; 0C1D16
147E50: move     b,x:(r6+$1f)                                ; 027ECF
147E51: move     b0,y:(r6+$1f)                               ; 027EE9
147E52: move     a,x:(r6+$2f)                                ; 02BECE
147E53: move     a0,y:(r6+$2f)                               ; 02BEE8
147E54: move     #>$147ce0,x1                                ; 45F400 147CE0
147E56: move     #$14,n0                                     ; 381400
147E57: lua      (r6+$10),r4                                 ; 040E04
147E58: move     n0,n3                                       ; 231B00
147E59: move     #$a,n4                                      ; 3C0A00
147E5A: move     #$f8,r1                                     ; 31F800
147E5B: move     #$0,r2                                      ; 320000
147E5C: do       #<$2,>$147e7a                               ; 060280 147E79
147E5E: move     l:(r2)+,y                                   ; 43DA00
147E5F: move     #>$17c70,x0                                 ; 44F400 017C70
147E61: mpysu    x0,y0,b                                     ; 0127AD
147E62: dmac     ss x0,y1,b                                  ; 0124A4
147E63: clb      b,a                                         ; 0C1E02
147E64: add      x1,a            #>$1aa17,x0                 ; 44F460 01AA17
147E66: move     b,l:(r4)                                    ; 496400
147E67: move     a,r0                                        ; 21D000
147E68: mpysu    x0,y0,b                                     ; 0127AD
147E69: dmac     ss x0,y1,b                                  ; 0124A4
147E6A: clb      b,a                                         ; 0C1E02
147E6B: add      x1,a            l:(r4)+,b                   ; 49DC60
147E6C: move     x:(r0)+n0,y0                                ; 46C800
147E6D: move     a,r3                                        ; 21D300
147E6E: asr      y0,b,a                                      ; 0C1E7A
147E6F: move     x:(r0)+n0,x0    y0,y:(r4)+                  ; B08800
147E70: move     a,l:(r1)+                                   ; 485900
147E71: move     x0,y:(r4)+                                  ; 4C5C00
147E72: move     x:(r0),y0                                   ; 46E000
147E73: move     x:(r3)+n3,x0    y0,y:(r4)+                  ; B08B00
147E74: move     x:(r3)+n3,a                                 ; 56CB00
147E75: asr      x0,b,b                                      ; 0C1E79
147E76: move     x0,y:(r4)+                                  ; 4C5C00
147E77: move     x:(r3),x0       a,y:(r4)+                   ; B28300
147E78: move     x0,y:(r4)+n4                                ; 4C4C00
147E79: move     b,l:(r1)+                                   ; 495900
147E7A: move     #>$1407ff,r3                                ; 63F400 1407FF
147E7C: move     y:(r6+$8),x1                                ; 0226B5
147E7D: mpyri    #>$6000,x1,a                                ; 0141E1 006000
147E7F: asr      #$b,a,b                                     ; 0C1C17
147E80: and      #>$7ff,a                                    ; 0140C6 0007FF
147E82: neg      a               a1,r2                       ; 219236
147E83: move     b1,x0                                       ; 21A400
147E84: move     a1,n3                                       ; 219B00
147E85: move     x:(r2+$140000),a                            ; 0A72CE 140000
147E87: asr      #$d,a,a                                     ; 0C1C1A
147E88: asl      x0,a,a                                      ; 0C1E48
147E89: move     x:(r3+n3),b                                 ; 57EB00
147E8A: asr      #$8,b,b                                     ; 0C1C91
147E8B: asr      x0,b,b                                      ; 0C1E79
147E8C: move     a1,x:(r6+$30)                               ; 02C68C
147E8D: move     a0,y:(r6+$30)                               ; 02C6A8
147E8E: move     b1,x:(r6+$31)                               ; 02C6CD
147E8F: move     b0,y:(r6+$31)                               ; 02C6E9
147E90: lua      (r6+$10),r7                                 ; 040E07
147E91: move     #$f8,n7                                     ; 3FF800
147E92: move     #$60,r1                                     ; 316000
147E93: move     #$81,n3                                     ; 3B8100
147E94: bsr      func_147c50                                 ; 050891
147E95: move     l:?:>$f8,a                                  ; 48F000 0000F8
147E97: move     #$61,r0                                     ; 306100
147E98: move     #$82,r1                                     ; 318200
147E99: move     #$80,r4                                     ; 348000
147E9A: bsr      func_147c3e                                 ; 050859
147E9B: move     #>$1407ff,r3                                ; 63F400 1407FF
147E9D: move     y:(r6+$a),x1                                ; 022EB5
147E9E: mpyri    #>$6000,x1,a                                ; 0141E1 006000
147EA0: asr      #$b,a,b                                     ; 0C1C17
147EA1: and      #>$7ff,a                                    ; 0140C6 0007FF
147EA3: neg      a               a1,r2                       ; 219236
147EA4: move     b1,x0                                       ; 21A400
147EA5: move     a1,n3                                       ; 219B00
147EA6: move     x:(r2+$140000),a                            ; 0A72CE 140000
147EA8: asr      #$d,a,a                                     ; 0C1C1A
147EA9: asl      x0,a,a                                      ; 0C1E48
147EAA: move     x:(r3+n3),b                                 ; 57EB00
147EAB: asr      #$8,b,b                                     ; 0C1C91
147EAC: asr      x0,b,b                                      ; 0C1E79
147EAD: move     a1,x:(r6+$30)                               ; 02C68C
147EAE: move     a0,y:(r6+$30)                               ; 02C6A8
147EAF: move     b1,x:(r6+$31)                               ; 02C6CD
147EB0: move     b0,y:(r6+$31)                               ; 02C6E9
147EB1: lua      (r6+$20),r7                                 ; 041607
147EB2: move     #$fa,n7                                     ; 3FFA00
147EB3: move     #$80,r1                                     ; 318000
147EB4: move     #$a1,n3                                     ; 3BA100
147EB5: bsr      func_147c50                                 ; 050850
147EB6: move     l:?:>$fa,a                                  ; 48F000 0000FA
147EB8: move     #$81,r0                                     ; 308100
147EB9: move     #$a2,r1                                     ; 31A200
147EBA: move     #$1f,r4                                     ; 341F00
147EBB: bsr      func_147ed3                                 ; 050818
147EBC: move     #$0,r0                                      ; 300000
147EBD: move     y:(r6+$5),x1                                ; 0216F5
147EBE: mpyi     #>$2a9296,x1,a                              ; 0141E0 2A9296
147EC0: asl      a               #$1f,r7                     ; 371F32
147EC1: move     y:(r6+$37),x0                               ; 02DEF4
147EC2: move     a,y:(r6+$37)                                ; 02DEEE
147EC3: sub      x0,a            #$20,r4                     ; 342044
147EC4: asr      #$5,a,a                                     ; 0C1C0A
147EC5: move     #$81,r5                                     ; 358100
147EC6: tfr      x0,b            a,y1                        ; 21C749
147EC7: move     #>$547ae1,x0                                ; 44F400 547AE1
147EC9: do       #<$20,>$147ed0                              ; 062080 147ECF
147ECB: add      y1,b            b,x1            y:(r4)+,y0  ; 1CDC78
147ECC: mpy      x1,y0,a         a,x:(r0)        y:(r5)+,y0  ; F820E0
147ECD: mac      -x1,y0,a        x:(r0),x1                   ; 45E0E6
147ECE: mac      y0,x0,a         x1,y:(r7)+                  ; 4D5FD2
147ECF: asl      #$2,a,a                                     ; 0C1D04
147ED0: lua      (r6+$32),r6                                 ; 041E26
147ED1: move     a,y:(r7)+                                   ; 5E5F00
147ED2: jmp      func_000458                                 ; 0C0458
147ED3: clb      a,b                                         ; 0C1E01
147ED4: normf    b1,a                                        ; 0C1E26
147ED5: sub      #>$780000,a                                 ; 0140C4 780000
147ED7: clr      a               iflt                        ; 202913
147ED8: asl      #$4,a,a                                     ; 0C1D08
147ED9: move     #>$7fffff,b                                 ; 57F400 7FFFFF
147EDB: sub      a,b             a,x1            y:(r4),a    ; 16E41C
147EDC: move     x:(r0)+,x0      b,y1                        ; 139800
147EDD: do       #<$10,>$147ee3                              ; 061080 147EE2
147EDF: mpy      x0,y1,b         x:(r1)+,x0                  ; 44D9C8
147EE0: mac      x1,x0,b         x:(r0)+,x0      a,y:(r4)+   ; B298AA
147EE1: mpy      x0,y1,a         x:(r1)+,x0                  ; 44D9C0
147EE2: mac      x1,x0,a         x:(r0)+,x0      b,y:(r4)+   ; B398A2
147EE3: move     a,y:(r4)+                                   ; 5E5C00
147EE4: rts                                                  ; 00000C
147EE5: move     y:(r7+$a),b                                 ; 022FBF
147EE6: move     y:(r7+$b),a                                 ; 022FFE
147EE7: move     y:(r7+$2),x0                                ; 020FB4
147EE8: add      x0,b            #$0,r2                      ; 320048
147EE9: add      x0,a            b1,x1                       ; 21A540
147EEA: move     x:(r7+$9),b                                 ; 0227DF
147EEB: move     y:(r7+$9),b0                                ; 0227F9
147EEC: move     y:(r7+$1),x0                                ; 0207F4
147EED: asr      x0,b,b                                      ; 0C1E79
147EEE: move     y:(r7+$3),x0                                ; 020FF4
147EEF: move     #>$0,y1                                     ; 47F400 000000
147EF1: add      x0,b            #$80,y0                     ; 268048
147EF2: sub      y,b             n7,r0                       ; 23F03C
147EF3: and      x0,b            a1,y1                       ; 21874E
147EF4: add      x1,b                                        ; 200068
147EF5: sub      x1,b            b1,r4                       ; 21B46C
147EF6: add      y1,b            l:(r0),a                    ; 48E078
147EF7: move     b1,r5                                       ; 21B500
147EF8: move     #>$20,b1                                    ; 55F400 000020
147EFA: add      a,b             b,l:(r2)                    ; 496218
147EFB: asl      a                                           ; 200032
147EFC: move     a1,y1                                       ; 218700
147EFD: move     a0,y0                                       ; 210600
147EFE: move     l:(r2),a                                    ; 48E200
147EFF: do       #<$10,>$147f03                              ; 061080 147F02
147F01: add      y,a             a,l:(r2)+                   ; 485A30
147F02: add      y,b             b,l:(r2)+                   ; 495A38
147F03: move     l:(r0),b                                    ; 49E000
147F04: asl      #$17,b,b                                    ; 0C1DAF
147F05: move     #$4,a                                       ; 2E0400
147F06: move     b,x0                                        ; 21E400
147F07: andi     #$fe,ccr                                    ; 00FEB9
147F08: do       #<$18,>$147f0b                              ; 061880 147F0A
147F0A: div      x0,a                                        ; 018040
147F0B: move     y:(r7+$3),m4                                ; 0B77E4 000003
147F0D: move     m4,m5                                       ; 0465A4
147F0E: move     a0,x0                                       ; 210400
147F0F: move     x:(r7+$f),y1                                ; 023FD7
147F10: move     y:(r7+$f),y0                                ; 023FF6
147F11: mpysu    x0,y0,a                                     ; 01278D
147F12: dmac     ss x0,y1,a                                  ; 012484
147F13: move     y:(r7+$c),b                                 ; 0237BF
147F14: move     a,x1                                        ; 21C500
147F15: move     #$25,n2                                     ; 3A2500
147F16: jsr      func_0003ec                                 ; 0D03EC
147F17: move     y:(r7+$a),b                                 ; 022FBF
147F18: move     y:(r7+$b),a                                 ; 022FFE
147F19: move     y:(r7+$5),x0                                ; 0217F4
147F1A: add      x0,b            #$0,r1                      ; 310048
147F1B: add      x0,a            b1,x1                       ; 21A540
147F1C: move     x:(r7+$9),b                                 ; 0227DF
147F1D: move     y:(r7+$9),b0                                ; 0227F9
147F1E: move     y:(r7+$4),x0                                ; 0217B4
147F1F: asr      x0,b,b                                      ; 0C1E79
147F20: move     y:(r7+$6),x0                                ; 021FB4
147F21: move     #>$0,y1                                     ; 47F400 000000
147F23: add      x0,b            #$88,y0                     ; 268848
147F24: sub      y,b             n7,r0                       ; 23F03C
147F25: and      x0,b            a1,y1                       ; 21874E
147F26: add      x1,b                                        ; 200068
147F27: sub      x1,b            b1,r4                       ; 21B46C
147F28: add      y1,b            (r0)+                       ; 205878
147F29: move     l:(r0),a                                    ; 48E000
147F2A: move     b1,r5                                       ; 21B500
147F2B: move     #>$20,b1                                    ; 55F400 000020
147F2D: add      a,b             b,l:(r1)                    ; 496118
147F2E: asl      a                                           ; 200032
147F2F: move     a1,y1                                       ; 218700
147F30: move     a0,y0                                       ; 210600
147F31: move     l:(r1),a                                    ; 48E100
147F32: do       #<$10,>$147f36                              ; 061080 147F35
147F34: add      y,a             a,l:(r1)+                   ; 485930
147F35: add      y,b             b,l:(r1)+                   ; 495938
147F36: move     l:(r0),b                                    ; 49E000
147F37: move     n3,r1                                       ; 237100
147F38: asl      #$17,b,b                                    ; 0C1DAF
147F39: move     #$4,a                                       ; 2E0400
147F3A: move     b,x0                                        ; 21E400
147F3B: andi     #$fe,ccr                                    ; 00FEB9
147F3C: do       #<$18,>$147f3f                              ; 061880 147F3E
147F3E: div      x0,a                                        ; 018040
147F3F: move     y:(r7+$6),m4                                ; 0B77E4 000006
147F41: move     m4,m5                                       ; 0465A4
147F42: move     a0,x0                                       ; 210400
147F43: move     x:(r7+$f),y1                                ; 023FD7
147F44: move     y:(r7+$f),y0                                ; 023FF6
147F45: mpysu    x0,y0,a                                     ; 01278D
147F46: dmac     ss x0,y1,a                                  ; 012484
147F47: move     y:(r7+$c),b                                 ; 0237BF
147F48: move     a,x1                                        ; 21C500
147F49: move     #$16,n2                                     ; 3A1600
147F4A: jsr      func_0003ec                                 ; 0D03EC
147F4B: move     n7,r0                                       ; 23F000
147F4C: move     y:(r7+$c),b                                 ; 0237BF
147F4D: move     x:(r7+$f),a                                 ; 023FDE
147F4E: move     y:(r7+$f),a0                                ; 023FF8
147F4F: add      a,b             l:(r7),a                    ; 48E718
147F50: asl      #$5,a,a                                     ; 0C1D0A
147F51: move     b,y:(r7+$c)                                 ; 0237AF
147F52: move     x:(r7+$9),b                                 ; 0227DF
147F53: move     y:(r7+$9),b0                                ; 0227F9
147F54: add      a,b                                         ; 200018
147F55: and      #>$3ff,b                                    ; 0140CE 0003FF
147F57: move     b1,x:(r7+$9)                                ; 0227CD
147F58: move     b0,y:(r7+$9)                                ; 0227E9
147F59: rts                                                  ; 00000C
147F5A: move     #$80,x0                                     ; 248000
147F5B: move     x0,x:(r6+$11)                               ; 0246C4
147F5C: move     #$0,x0                                      ; 240000
147F5D: move     x0,x:(r6+$12)                               ; 024E84
147F5E: move     x0,x1                                       ; 208500
147F5F: move     x0,x:(r6+$19)                               ; 0266C4
147F60: move     x0,y:(r6+$19)                               ; 0266E4
147F61: move     x0,x:(r6+$18)                               ; 026684
147F62: move     x0,y:(r6+$18)                               ; 0266A4
147F63: lua      (r6+$1e),r4                                 ; 040EE4
147F64: do       #<$8,>$147f68                               ; 060880 147F67
147F66: move     x,l:(r4)+                                   ; 425C00
147F67: move     x,l:(r4)+                                   ; 425C00
147F68: rts                                                  ; 00000C
147F69: move     #$0,x0                                      ; 240000
147F6A: move     x0,x:(r6-$19)                               ; 039EC4
147F6B: move     #$0,r4                                      ; 340000
147F6C: move     #$10,r5                                     ; 351000
147F6D: move     y:(r6+$b),x0                                ; 022EF4
147F6E: mpy      x0,x0,a                                     ; 200080
147F6F: move     x:(r0)+,x1                                  ; 45D800
147F70: move     a,x0                                        ; 21C400
147F71: mpy      x1,x0,a         x:(r0)+,x1                  ; 45D8A0
147F72: asl      #$2,a,a                                     ; 0C1D04
147F73: mpy      x1,x0,b         x:(r0)+,x1                  ; 45D8A8
147F74: asl      #$2,b,b                                     ; 0C1D85
147F75: do       #<$10,>$147f7b                              ; 061080 147F7A
147F77: mpy      x1,x0,a         x:(r0)+,x1      a,y:(r4)+   ; B698A0
147F78: asl      #$2,a,a                                     ; 0C1D04
147F79: mpy      x1,x0,b         x:(r0)+,x1      b,y:(r5)+   ; B7B8A8
147F7A: asl      #$2,b,b                                     ; 0C1D85
147F7B: move     y:(r6+$6),x0                                ; 021EB4
147F7C: mpy      x0,x0,a                                     ; 200080
147F7D: move     a,x0                                        ; 21C400
147F7E: mpyi     #>$4ab3,x0,a                                ; 0141C0 004AB3
147F80: move     #$20,r0                                     ; 302000
147F81: move     r0,r4                                       ; 221400
147F82: move     a,x0                                        ; 21C400
147F83: move     x:(r6+$11),a                                ; 0246DE
147F84: move     x:(r6+$12),b                                ; 024E9F
147F85: move     a,x1                                        ; 21C500
147F86: bset     #$14,sr                                     ; 0AF974
147F87: do       #<$10,>$147f8b                              ; 061080 147F8A
147F89: mac      -x1,x0,b        b,x:(r0)+       b,y0        ; 1E18AE
147F8A: mac      y0,x0,a         a,x1            a,y:(r4)+   ; 165CD2
147F8B: bclr     #$14,sr                                     ; 0AF954
147F8C: move     a,x:(r6+$11)                                ; 0246CE
147F8D: move     b,x:(r6+$12)                                ; 024E8F
147F8E: move     y:(r6+$1b),a                                ; 026EFE
147F8F: move     y:(r6+$9),y0                                ; 0226F6
147F90: move     a,y1                                        ; 21C700
147F91: maci     #>$28f5c,y0,a                               ; 0141D2 028F5C
147F93: maci     #>$fd70a4,y1,a                              ; 0141F2 FD70A4
147F95: move     a,y:(r6+$1b)                                ; 026EEE
147F96: move     a,y0                                        ; 21C600
147F97: mpyi     #>$800,y0,a                                 ; 0141D0 000800
147F99: move     a,r0                                        ; 21D000
147F9A: move     x:(r0+$14a000),a                            ; 0A70CE 14A000
147F9C: move     x:(r0+$14a800),b                            ; 0A70CF 14A800
147F9E: move     y:(r6+$1c),x0                               ; 0276B4
147F9F: move     y:(r6+$1d),x1                               ; 0276F5
147FA0: move     b,y:(r6+$1c)                                ; 0276AF
147FA1: move     a,y:(r6+$1d)                                ; 0276EE
147FA2: sub      x1,a                                        ; 200064
147FA3: sub      x0,b                                        ; 20004C
147FA4: tfr      x1,a            a,y0                        ; 21C661
147FA5: tfr      x0,b            b,y1                        ; 21E749
147FA6: move     #$8,x0                                      ; 240800
147FA7: move     #>$e0,r0                                    ; 60F400 0000E0
147FA9: do       #<$10,>$147fad                              ; 061080 147FAC
147FAB: mac      y0,x0,a         a,y:(r0)                    ; 5E60D2
147FAC: mac      x0,y1,b         b,x:(r0)+                   ; 5758CA
147FAD: move     #>$e0,r5                                    ; 65F400 0000E0
147FAF: move     #>$20,r0                                    ; 60F400 000020
147FB1: move     r0,r4                                       ; 221400
147FB2: move     #>$20,r1                                    ; 61F400 000020
147FB4: move     #>$30,r2                                    ; 62F400 000030
147FB6: move     x:(r5),y0                                   ; 46E500
147FB7: move     x:(r0)+,x0      y:(r5)+,y1                  ; F1B800
147FB8: do       #<$10,>$147fc0                              ; 061080 147FBF
147FBA: mpy      y0,x0,a         y:(r4)+,x1                  ; 4DDCD0
147FBB: mac      y1,x1,a                                     ; 2000F2
147FBC: mpy      y0,x0,b         x:(r5),y0                   ; 46E5D8
147FBD: mac      -y1,x1,b        x:(r0)+,x0      y:(r5)+,y1  ; F1B8FE
147FBE: move     a,x:(r1)+                                   ; 565900
147FBF: move     b,x:(r2)+                                   ; 575A00
147FC0: move     x:(r6+$18),b                                ; 02669F
147FC1: move     y:(r6+$18),b0                               ; 0266B9
147FC2: move     y:(r6+$4),x0                                ; 0216B4
147FC3: move     b,x1                                        ; 21E500
147FC4: maci     #>$28f5c,x0,b                               ; 0141CA 028F5C
147FC6: maci     #>$fd70a4,x1,b                              ; 0141EA FD70A4
147FC8: move     b1,x:(r6+$18)                               ; 02668D
147FC9: move     b0,y:(r6+$18)                               ; 0266A9
147FCA: move     x:(r6+$19),a                                ; 0266DE
147FCB: move     y:(r6+$19),a0                               ; 0266F8
147FCC: move     y:(r6+$5),x0                                ; 0216F4
147FCD: move     a,x1                                        ; 21C500
147FCE: maci     #>$28f5c,x0,a                               ; 0141C2 028F5C
147FD0: maci     #>$fd70a4,x1,a                              ; 0141E2 FD70A4
147FD2: move     a1,x:(r6+$19)                               ; 0266CC
147FD3: move     a0,y:(r6+$19)                               ; 0266E8
147FD4: move     a,x0                                        ; 21C400
147FD5: sub      #>$400000,b                                 ; 0140CC 400000
147FD7: asl      b                                           ; 20003A
147FD8: move     b,x1                                        ; 21E500
147FD9: abs      b                                           ; 20002E
147FDA: add      #>$800000,b                                 ; 0140C8 800000
147FDC: neg      b                                           ; 20003E
147FDD: move     b,y1                                        ; 21E700
147FDE: mpy      x0,y1,a                                     ; 2000C0
147FDF: move     y:(r6+$1a),x0                               ; 026EB4
147FE0: move     a,y:(r6+$1a)                                ; 026EAE
147FE1: sub      x0,a            #>$80000,y1                 ; 47F444 080000
147FE3: tfr      x0,a            a,y0                        ; 21C641
147FE4: move     #>$e0,r3                                    ; 63F400 0000E0
147FE6: move     #$f,m3                                      ; 050FA3
147FE7: do       #<$10,>$147fea                              ; 061080 147FE9
147FE9: mac      y1,y0,a         a,x:(r3)+                   ; 565BB2
147FEA: move     #>$20,r1                                    ; 61F400 000020
147FEC: move     #>$0,r2                                     ; 62F400 000000
147FEE: move     x1,y0                                       ; 20A600
147FEF: move     x:(r1)+,x1                                  ; 45D900
147FF0: move     x:(r3)+,x0                                  ; 44DB00
147FF1: do       #<$10,>$147ff9                              ; 061080 147FF8
147FF3: mpy      x1,x0,a         x:(r1)+,x1                  ; 45D9A0
147FF4: add      y0,a            x:(r3)+,x0                  ; 44DB50
147FF5: mpy      x1,x0,b         x:(r1)+,x1                  ; 45D9A8
147FF6: add      y0,b            x:(r3)+,x0                  ; 44DB58
147FF7: move     a,x:(r2)+                                   ; 565A00
147FF8: move     b,x:(r2)+                                   ; 575A00
147FF9: move     #>$ffffff,m3                                ; 05F423 FFFFFF
147FFB: move     #>$90,r4                                    ; 64F400 000090
147FFD: move     y:(r6+$8),a                                 ; 0226BE
147FFE: sub      #>$400000,a                                 ; 0140C4 400000
148000: asl      a                                           ; 200032
148001: move     a,y1                                        ; 21C700
148002: move     y:(r6+$13),b                                ; 024EFF
148003: move     a,y:(r6+$13)                                ; 024EEE
148004: sub      b,a                                         ; 200014
148005: move     a,y0                                        ; 21C600
148006: mpyi     #>$80000,y0,a                               ; 0141D0 080000
148008: do       #<$10,>$14800b                              ; 061080 14800A
14800A: add      a,b             b,y:(r4)+                   ; 5F5C18
14800B: move     #$1e,n6                                     ; 3E1E00
14800C: move     #>$0,r0                                     ; 60F400 000000
14800E: move     #>$0,r5                                     ; 65F400 000000
148010: move     #>$60,r3                                    ; 63F400 000060
148012: do       #<$2,>$14804d                               ; 060280 14804C
148014: lua      (r6)+n6,r4                                  ; 044E14
148015: move     #>$20,r2                                    ; 62F400 000020
148017: do       #<$7,>$14801b                               ; 060780 14801A
148019: move     l:(r4)+,x                                   ; 42DC00
14801A: move     x,l:(r2)+                                   ; 425A00
14801B: move     #>$21,r1                                    ; 61F400 000021
14801D: move     #>$20,r2                                    ; 62F400 000020
14801F: move     #>$90,r4                                    ; 64F400 000090
148021: move     #>$fffffb,n2                                ; 72F400 FFFFFB
148023: move     n2,n1                                       ; 235900
148024: move     x:(r0)+,x0      y:(r4)+,y1                  ; F19800
148025: do       #<$10,>$148041                              ; 061080 148040
148027: mpy      y1,x1,b         y:(r5),y0                   ; 4EE5F8
148028: add      y0,b            x:(r2)+,y0                  ; 46DA58
148029: tfr      y0,a            b,x1                        ; 21E551
14802A: mac      x1,x0,a         x:(r2)+,x1                  ; 45DAA2
14802B: mac      -x1,x0,a        b,x:(r1)+                   ; 5759A6
14802C: tfr      x1,b            x:(r2)+,x1                  ; 45DA69
14802D: mac      -x1,x0,b        a,y0                        ; 21C6AE
14802E: mac      y0,x0,b         a,x:(r1)+                   ; 5659DA
14802F: tfr      x1,a            x:(r2)+,x1                  ; 45DA61
148030: mac      -x1,x0,a        b,y0                        ; 21E6A6
148031: mac      y0,x0,a         b,x:(r1)+                   ; 5759D2
148032: tfr      x1,b            x:(r2)+,x1                  ; 45DA69
148033: mac      -x1,x0,b        a,y0                        ; 21C6AE
148034: mac      y0,x0,b         a,x:(r1)+                   ; 5659DA
148035: tfr      x1,a            x:(r2)+,x1                  ; 45DA61
148036: mac      -x1,x0,a        b,y0                        ; 21E6A6
148037: mac      y0,x0,a         b,x:(r1)+                   ; 5759D2
148038: tfr      x1,b            x:(r2)+n2,x1                ; 45CA69
148039: mac      -x1,x0,b        a,y0                        ; 21C6AE
14803A: mac      y0,x0,b         a,x:(r1)+                   ; 5659DA
14803B: move     #>$5a8241,y1                                ; 47F400 5A8241
14803D: move     x:(r0)+,x0      y:(r5)+,y0                  ; F0B800
14803E: mpy      y1,y0,a         b,x1                        ; 21E5B0
14803F: mac      y1,x1,a         b,x:(r1)+n1     y:(r4)+,y1  ; FD09F2
148040: move     a,x:(r3)+                                   ; 565B00
148041: lua      (r6)+n6,r4                                  ; 044E14
148042: do       #<$7,>$148046                               ; 060780 148045
148044: move     l:(r2)+,y                                   ; 43DA00
148045: move     y,l:(r4)+                                   ; 435C00
148046: move     #$26,n6                                     ; 3E2600
148047: move     #>$10,r0                                    ; 60F400 000010
148049: move     #>$10,r5                                    ; 65F400 000010
14804B: move     #>$70,r3                                    ; 63F400 000070
14804D: move     #>$0,r4                                     ; 64F400 000000
14804F: move     #>$10,r3                                    ; 63F400 000010
148051: move     #>$60,r2                                    ; 62F400 000060
148053: move     #>$70,r1                                    ; 61F400 000070
148055: move     y:(r6+$7),x0                                ; 021EF4
148056: move     #>$7fffff,a                                 ; 56F400 7FFFFF
148058: sub      x0,a                                        ; 200044
148059: move     y:(r4)+,y0                                  ; 4EDC00
14805A: move     a,x1                                        ; 21C500
14805B: do       #<$10,>$148063                              ; 061080 148062
14805D: mpy      x1,y0,a         x:(r2)+,y0                  ; 46DAE0
14805E: mac      y0,x0,a         y:(r3)+,y0                  ; 4EDBD2
14805F: mpy      x1,y0,b         x:(r1)+,y0                  ; 46D9E8
148060: mac      y0,x0,b         y:(r4)+,y0                  ; 4EDCDA
148061: move     a,y:(r7)+                                   ; 5E5F00
148062: move     b,y:(r7)+                                   ; 5F5F00
148063: rts                                                  ; 00000C
