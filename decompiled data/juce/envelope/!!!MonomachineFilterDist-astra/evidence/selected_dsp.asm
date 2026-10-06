000350: move x:(r0)+,x0 y:(r4)+,y1 ; F19800
000351: move #>$fffffe,n4        ; 74F400 FFFFFE
000353: move b,x1                ; 21E500
000354: do #<$10,>$364           ; 061080 000363
000356: asr #$4,b,b              ; 0C1C89
000357: mac x0,y1,b y:(r4)+,y1   ; 4FDCCA
000358: asl #$4,b,b              ; 0C1D89
000359: mac y1,x1,b a,x0 y:(r4)+n4,y0 ; 10CCFA
00035A: mac -y0,x0,b a,l:(r1)+   ; 4859DE
00035B: mac x0,y1,a x:(r0)+,x0 y:(r4)+,y1 ; F198C2
00035C: mac x1,y0,a b,x1         ; 21E5E2
00035D: asr #$4,b,b              ; 0C1C89
00035E: mac x0,y1,b y:(r4)+,y1   ; 4FDCCA
00035F: asl #$4,b,b              ; 0C1D89
000360: mac y1,x1,b a,x0 y:(r4)+,y0 ; 10DCFA
000361: mac -y0,x0,b a,l:(r1)+   ; 4859DE
000362: mac x0,y1,a x:(r0)+,x0 y:(r4)+,y1 ; F198C2
000363: mac x1,y0,a b,x1         ; 21E5E2
000364: rts                      ; 00000C
000365: move x:(r0)+,x1 y:(r4)+,y1 ; F59800
000366: move x:(r1),x0 y:(r5),y0 ; C0A100
000367: move #>$fffffe,n4        ; 74F400 FFFFFE
000369: do #<$10,>$37b           ; 061080 00037A
00036B: mac -y1,x1,b b,x1 y:(r4)+,y1 ; 1DDCFE
00036C: mac y0,x0,a a,l:(r2)     ; 4862D2
00036D: asl #$7,a,a              ; 0C1D0E
00036E: mac y1,x1,b x:(r2),x0 y:(r4)+n4,y0 ; D082FA
00036F: mac -y0,x0,b a,l:(r1)+   ; 4859DE
000370: move l:(r2),a            ; 48E200
000371: mac x0,y1,a x:(r1),x0 y:(r4)+,y1 ; F181C2
000372: mac x1,y0,a x:(r0)+,x1 y:(r5),y0 ; C4B8E2
000373: mac -y1,x1,b b,x1 y:(r4)+,y1 ; 1DDCFE
000374: mac y0,x0,a a,l:(r2)     ; 4862D2
000375: asl #$7,a,a              ; 0C1D0E
000376: mac y1,x1,b x:(r2),x0 y:(r4)+,y0 ; F082FA
000377: mac -y0,x0,b a,l:(r1)+   ; 4859DE
000378: move l:(r2),a            ; 48E200
000379: mac x0,y1,a x:(r1),x0 y:(r4)+,y1 ; F181C2
00037A: mac x1,y0,a x:(r0)+,x1 y:(r5),y0 ; C4B8E2
00037B: rts                      ; 00000C
00037C: move x:(r0)+,x0 y:(r4)+n4,y0 ; D09800
00037D: do #<$10,>$387           ; 061080 000386
00037F: mac y0,x0,a a,x:(r1)+ a,y1 ; 1919D2
000380: mac -y1,y0,a x:(r2)+,x0  ; 44DAB6
000381: mac y0,x0,b b,x:(r3)+ b,y1 ; 1F1BDA
000382: mac -y1,y0,b x:(r0)+,x0  ; 44D8BE
000383: mac y0,x0,a a,x:(r1)+ a,y1 ; 1919D2
000384: mac -y1,y0,a x:(r2)+,x0  ; 44DAB6
000385: mac y0,x0,b b,x:(r3)+ b,y1 ; 1F1BDA
000386: mac -y1,y0,b x:(r0)+,x0 y:(r4)+n4,y0 ; D098BE
000387: move a,x:(r1)+           ; 565900
000388: move b,x:(r3)+           ; 575B00
000389: rts                      ; 00000C
00038A: move x:(r0)+,y0          ; 46D800
00038B: do #<$10,>$394           ; 061080 000393
00038D: move l:(r4)+,x           ; 42DC00
00038E: mac -y0,x0,a a,x:(r1)+ a,y0 ; 1819D6
00038F: mac -x1,y0,a x:(r0),y0   ; 46E0E6
000390: mac y0,x0,a x:(r2)+,y0   ; 46DAD2
000391: mac -y0,x0,b b,x:(r3)+ b,y0 ; 1E1BDE
000392: mac -x1,y0,b x:(r2),y0   ; 46E2EE
000393: mac y0,x0,b x:(r0)+,y0   ; 46D8DA
000394: move a,x:(r1)+           ; 565900
000395: move b,x:(r3)+           ; 575B00
000396: rts                      ; 00000C
0004FF: move y:(r6+$20),a        ; 0286BE
000500: cmp #<$1,a               ; 014185
000501: bne func_000506          ; 052405
000502: move #$0,x0              ; 240000
000503: move x0,y:(r7-$1)        ; 03FFE4
000504: move x0,y:(r6+$20)       ; 0286A4
000505: move x0,x:(r7-$1)        ; 03FFC4
000506: move y:(r7-$1),a         ; 03FFFE
000507: tst a                    ; 200003
000508: bne func_000517          ; 05240F
000509: move y:(r6+$c),a         ; 0236BE
00050A: asr #$10,a,a             ; 0C1C20
00050B: move x:(r7-$1),b         ; 03FFDF
00050C: move a,r4                ; 21D400
00050D: move y:(r4+$141800),y0   ; 0B74C6 141800
00050F: add y0,b                 ; 200058
000510: bec func_000515          ; 055405
000511: move #>$1,x0             ; 44F400 000001
000513: move x0,y:(r7-$1)        ; 03FFE4
000514: move x0,x:(r7-$2)        ; 03FF84
000515: move b,x:(r7-$1)         ; 03FFCF
000516: bra func_000537          ; 050C41
000517: cmp #<$1,a               ; 014185
000518: bne func_00052b          ; 052413
000519: clr a                    ; 200013
00051A: move y:(r6+$23),y0       ; 028EF6
00051B: move a,x0                ; 21C400
00051C: mpy y0,x0,b              ; 2000D8
00051D: move x:(r7-$2),a         ; 03FF9E
00051E: move b,x1                ; 21E500
00051F: mpyi #>$791fd0,x1,b      ; 0141E8 791FD0
000521: asl b                    ; 20003A
000522: add #<$1,a               ; 014180
000523: cmp b,a                  ; 200005
000524: blt func_000529          ; 059405
000525: move #>$2,x0             ; 44F400 000002
000527: move x0,y:(r7-$1)        ; 03FFE4
000528: bra func_00052b          ; 050C03
000529: move a,x:(r7-$2)         ; 03FF8E
00052A: bra func_000537          ; 050C0D
00052B: move y:(r6+$d),a         ; 0236FE
00052C: add #>$7fff,a            ; 0140C0 007FFF
00052E: asr #$10,a,a             ; 0C1C20
00052F: rnd a                    ; 200011
000530: move a,r4                ; 21D400
000531: move x:(r7-$1),b         ; 03FFDF
000532: move y:(r4+$141a00),y0   ; 0B74C6 141A00
000534: sub y0,b                 ; 20005C
000535: clr b ifmi               ; 202B1B
000536: move b,x:(r7-$1)         ; 03FFCF
000537: move y:(r6+$8),x0        ; 0226B4
000538: mpyi #>$800,x0,a         ; 0141C0 000800
00053A: sub #>$80,a              ; 0140C4 000080
00053C: rnd a                    ; 200011
00053D: tfr a,b                  ; 200009
00053E: move y:(r6+$e),a         ; 023EBE
00053F: add #>$c00000,a          ; 0140C0 C00000
000541: abs a a,y0               ; 21C626
000542: move #>$700,x1           ; 45F400 000700
000544: move a,y1                ; 21C700
000545: mpy y1,y0,a              ; 2000B0
000546: asl #$2,a,a              ; 0C1D04
000547: move x:(r7-$1),y1        ; 03FFD7
000548: move a,y0                ; 21C600
000549: mpy y1,y0,a              ; 2000B0
00054A: move a,y1                ; 21C700
00054B: mpy y1,x1,a              ; 2000F0
00054C: add b,a                  ; 200010
00054D: cmp x1,a                 ; 200065
00054E: tfr x1,a ifge            ; 202161
00054F: move #$8,y0              ; 260800
000550: move x:(r6+$1),x1        ; 0206D5
000551: move y:(r6+$25),b        ; 0296FF
000552: move a1,x:(r7-$3)        ; 03F7CC
000553: mac x1,y0,a              ; 2000E2
000554: btst #$b,b               ; 0BCF6B
000555: bcc func_000557          ; 050402
000556: move a1,x:(r7-$3)        ; 03F7CC
000557: move y:(r6+$9),x0        ; 0226F4
000558: btst #$9,b               ; 0BCF69
000559: mac -x1,y0,a ifcc        ; 2020E6
00055A: maci #>$800,x0,a         ; 0141C2 000800
00055C: move y:(r6+$f),b         ; 023EFF
00055D: add #>$c00000,b          ; 0140C8 C00000
00055F: abs b b,y0               ; 21E62E
000560: move #>$700,x1           ; 45F400 000700
000562: move b,y1                ; 21E700
000563: mpy y1,y0,b              ; 2000B8
000564: asl #$2,b,b              ; 0C1D85
000565: move x:(r7-$1),y1        ; 03FFD7
000566: tfr a,b b,y0             ; 21E609
000567: mpy y1,y0,a              ; 2000B0
000568: move a,y1                ; 21C700
000569: mac y1,x1,b              ; 2000FA
00056A: clr b ifmi               ; 202B1B
00056B: move y:(r7-$3),a         ; 03F7FE
00056C: move b1,y:(r7-$2)        ; 03FFAD
0005BF: lua (r7-$18),r4          ; 043784
0005C0: move #>$fffffe,n4        ; 74F400 FFFFFE
0005C2: move r4,r5               ; 229500
0005C3: move #$91,r0             ; 309100
0005C4: move #$d1,r2             ; 32D100
0005C5: move l:(r4)+,x           ; 42DC00
0005C6: move x0,y:(r0)+          ; 4C5800
0005C7: move x1,y:(r0)+          ; 4D5800
0005C8: move l:(r4)+,x           ; 42DC00
0005C9: move x0,y:(r0)+          ; 4C5800
0005CA: move x1,y:(r2)+          ; 4D5A00
0005CB: move l:(r4)+,x           ; 42DC00
0005CC: move x0,y:(r2)+          ; 4C5A00
0005CD: move x1,y:(r2)+          ; 4D5A00
0005CE: move #$91,r4             ; 349100
0005CF: move #>$f528bd,x0        ; 44F400 F528BD
0005D1: move #>$4a4df0,x1        ; 45F400 4A4DF0
0005D3: move #$71,r1             ; 317100
0005D4: move #$73,r2             ; 327300
0005D5: move y:(r4)+,y0          ; 4EDC00
0005D6: move #$3,n1              ; 390300
0005D7: move n1,n2               ; 233A00
0005D8: do #<$8,>$5e2            ; 060880 0005E1
0005DA: mpy y0,x0,a a,x:(r1)+n1 y:(r4)+,y1 ; F909D0
0005DB: mpy x0,y1,b b,x:(r2)+n2  ; 574AC8
0005DC: mac y1,x1,a y:(r4)+,y0   ; 4EDCF2
0005DD: mac x1,y0,b y1,x:(r1)+   ; 4759EA
0005DE: mac x1,y0,a y:(r4)+,y1   ; 4FDCE2
0005DF: mac y1,x1,b y0,x:(r2)+   ; 465AFA
0005E0: mac x0,y1,a y:(r4)+n4,y0 ; 4ECCC2
0005E1: mac y0,x0,b y:(r4)+,y0   ; 4EDCDA
0005E2: move a,x:(r1)+n1         ; 564900
0005E3: move b,x:(r2)+n2         ; 574A00
0005E4: move y:(r4)+,y1          ; 4FDC00
0005E5: move y,l:(r5)+           ; 435D00
0005E6: move y:(r4)-,y1          ; 4FD400
0005E7: move y1,y:(r5)           ; 4F6500
0005E8: move x:(r6+$f),y1        ; 023ED7
0005E9: brset #$0,y1,func_0005fb ; 0CC7A0 000012
0005EB: move #$d1,r4             ; 34D100
0005EC: move #$b1,r1             ; 31B100
0005ED: move #$b3,r2             ; 32B300
0005EE: move y:(r4)+,y0          ; 4EDC00
0005EF: do #<$8,>$5f9            ; 060880 0005F8
0005F1: mpy y0,x0,a a,x:(r1)+n1 y:(r4)+,y1 ; F909D0
0005F2: mpy x0,y1,b b,x:(r2)+n2  ; 574AC8
0005F3: mac y1,x1,a y:(r4)+,y0   ; 4EDCF2
0005F4: mac x1,y0,b y1,x:(r1)+   ; 4759EA
0005F5: mac x1,y0,a y:(r4)+,y1   ; 4FDCE2
0005F6: mac y1,x1,b y0,x:(r2)+   ; 465AFA
0005F7: mac x0,y1,a y:(r4)+n4,y0 ; 4ECCC2
0005F8: mac y0,x0,b y:(r4)+,y0   ; 4EDCDA
0005F9: move a,x:(r1)+n1         ; 564900
0005FA: move b,x:(r2)+n2         ; 574A00
0005FB: move y0,x:(r5)+          ; 465D00
0005FC: move y:(r4)+,y0          ; 4EDC00
0005FD: move y:(r4)+,y1          ; 4FDC00
0005FE: move y,l:(r5)            ; 436500
0005FF: move y:(r7-$6),x1        ; 03EFB5
000600: tfr x1,a                 ; 200061
000601: sub #>$5bf,a             ; 0140C4 0005BF
000603: clr a ifmi               ; 202B13
000604: move a,y0                ; 21C600
000605: sub #>$80,a              ; 0140C4 000080
000607: clr a ifmi               ; 202B13
000608: move a,y1                ; 21C700
000609: move #>$63f,x0           ; 44F400 00063F
00060B: tfr x1,a                 ; 200061
00060C: cmp x0,a                 ; 200045
00060D: tfr x0,a ifgt            ; 202741
00060E: move a,x1                ; 21C500
00060F: move x1,r0               ; 20B000
000610: move y:(r6+$b),a         ; 022EFE
000611: mpyi #>$fffdf3,x1,b      ; 0141E8 FFFDF3
000613: maci #>$ffe666,y0,b      ; 0141DA FFE666
000615: maci #>$ffa666,y1,b      ; 0141FA FFA666
000617: asl #$18,b,b             ; 0C1DB1
000618: tfr x1,b b,y0            ; 21E669
000619: move a,x0                ; 21C400
00061A: mac y0,x0,a              ; 2000D2
00061B: clr a ifmi               ; 202B13
00061C: move a,x0                ; 21C400
00061D: maci #>$fff912,x0,b      ; 0141CA FFF912
00061F: move x:(r7-$1e),x0       ; 038F94
000620: move y:(r7-$1e),a        ; 038FBE
000621: move b1,x:(r7-$1e)       ; 038F8D
000622: move x1,y:(r7-$1e)       ; 038FA5
000623: add x1,a b1,r2           ; 21B260
000624: asr a                    ; 200022
000625: add x0,b                 ; 200048
000626: asr b a1,r1              ; 21912A
000627: move a0,x0               ; 210400
000628: move b0,x1               ; 212500
000629: move x:(r1+$141a98),y0   ; 0A71C6 141A98
00062B: move x:(r1+$141a99),y1   ; 0A71C7 141A99
00062D: tfr y0,a b1,r3           ; 21B351
00062E: macsu -y0,x0,a           ; 012695
00062F: macsu y1,x0,a            ; 01268C
000630: move x:(r1+$142158),y0   ; 0A71C6 142158
000632: move x:(r1+$142159),y1   ; 0A71C7 142159
000634: mpysu -y0,x0,b           ; 0127B5
000635: add y0,b a,y0            ; 21C658
000636: macsu y1,x0,b            ; 0126AC
000637: move x:(r3+$142f06),x0   ; 0A73C4 142F06
000639: move x:(r3+$142f07),y1   ; 0A73C7 142F07
00063B: mpysu y1,x1,a            ; 012787
00063C: add x0,a b,y1            ; 21E740
00063D: macsu -x0,x1,a           ; 01269A
00063E: move #>$7fffa4,x0        ; 44F400 7FFFA4
000640: cmp x0,a                 ; 200045
000641: tgt x0,a                 ; 027040
000642: move a,x0                ; 21C400
000643: mpy x0,y1,b              ; 2000C8
000644: mpy y0,x0,a              ; 2000D0
000645: move b,y0                ; 21E600
000646: add #>$800000,a          ; 0140C0 800000
000648: move b,y:$1e             ; 5F1E00
000649: move a,y:$1d             ; 5E1D00
00064A: mpy x0,x0,b              ; 200088
00064B: subr a,b                 ; 20000E
00064C: sub #>$400000,b          ; 0140CC 400000
00064E: andi #$fe,ccr            ; 00FEB9
00064F: move #>$800,a            ; 56F400 000800
000651: do #<$18,>$654           ; 061880 000653
000653: div y0,a                 ; 018050
000654: move b1,y1               ; 21A700
000655: move b0,y0               ; 212600
000656: move a0,x1               ; 210500
000657: mpysu x1,y0,b            ; 0127A6
000658: dmac ss x1,y1,b          ; 0124AF
000659: asl #$a,b,b              ; 0C1D95
00065A: move y:(r6+$4),a         ; 0216BE
00065B: asl a b,y1               ; 21E732
00065C: move a,x0                ; 21C400
00065D: mpyi #>$100,x0,a         ; 0141C0 000100
00065F: move a,r1                ; 21D100
000660: move x:(r1+$143c06),x0   ; 0A71C4 143C06
000662: mpy x0,y1,b              ; 2000C8
000663: move x:(r0+$141a98),y0   ; 0A70C6 141A98
000665: move b,y:$1c             ; 5F1C00
000666: move x:(r0+$142158),y1   ; 0A70C7 142158
000668: move x:(r2+$142f06),a    ; 0A72CE 142F06
00066A: move #>$7fffa4,x0        ; 44F400 7FFFA4
00066C: cmp x0,a                 ; 200045
00066D: tfr x0,a ifgt            ; 202741
00066E: move #$5,r4              ; 340500
00066F: move a,x0                ; 21C400
000670: mpy y0,x0,a              ; 2000D0
000671: mpy x0,y1,b              ; 2000C8
000672: move a,y:(r4)+           ; 5E5C00
000673: move b,y:(r4)+           ; 5F5C00
000674: mpy x0,x0,b b,y0         ; 21E688
000675: subr a,b                 ; 20000E
000676: add #>$400000,b          ; 0140C8 400000
000678: andi #$fe,ccr            ; 00FEB9
000679: move #>$800,a            ; 56F400 000800
00067B: do #<$18,>$67e           ; 061880 00067D
00067D: div y0,a                 ; 018050
00067E: move b1,y1               ; 21A700
00067F: move b0,y0               ; 212600
000680: move a0,x1               ; 210500
000681: mpysu x1,y0,b            ; 0127A6
000682: dmac ss x1,y1,b          ; 0124AF
000683: move #$2,n0              ; 380200
000684: asl #$a,b,b              ; 0C1D95
000685: move y:(r6+$4),a         ; 0216BE
000686: asl a b,y1               ; 21E732
000687: move #$5,r0              ; 300500
000688: move a,x0                ; 21C400
000689: mpyi #>$100,x0,a         ; 0141C0 000100
00068B: move a,r1                ; 21D100
00068C: move x:(r1+$143c06),x0   ; 0A71C4 143C06
00068E: move #$4,r1              ; 310400
00068F: mpy x0,y1,b y:(r0)+,a    ; 5ED8C8
000690: add #>$800000,a          ; 0140C0 800000
000692: move b,y:$4              ; 5F0400
000693: move l:(r7),x            ; 42E700
000694: move a,y:(r7)            ; 5E6700
000695: move y:$1d,a             ; 5E9D00
000696: sub x0,a y:(r0)-,y1      ; 4FD044
000697: move y1,x:(r7)           ; 476700
000698: move y:$1e,b             ; 5F9E00
000699: sub x1,b a,y0            ; 21C66C
00069A: tfr x0,a #$10,x0         ; 241041
00069B: tfr x1,b b,y1            ; 21E769
00069C: do #<$8,>$6a0            ; 060880 00069F
00069E: mac y0,x0,a a,y:(r0)+    ; 5E58D2
00069F: mac x0,y1,b b,y:(r0)+n0  ; 5F48CA
0006A0: move y:(r0)+,x0          ; 4CD800
0006A1: move l:(r7)+,ba          ; 4BDF00
0006A2: sub x0,a y:(r0)-,x1      ; 4DD044
0006A3: sub x1,b                 ; 20006C
0006A4: tfr x0,a a,y0            ; 21C641
0006A5: tfr x1,b b,y1            ; 21E769
0006A6: move #$10,x0             ; 241000
0006A7: do #<$8,>$6ab            ; 060880 0006AA
0006A9: mac y0,x0,a a,y:(r0)+    ; 5E58D2
0006AA: mac x0,y1,b b,y:(r0)+n0  ; 5F48CA
0006AB: move #$3,n0              ; 380300
0006AC: move n0,n1               ; 231900
0006AD: move #$1c,r0             ; 301C00
0006AE: move y:$1c,x1            ; 4D9C00
0006AF: tfr x1,a y:(r1),b        ; 5FE161
0006B0: move y:(r7),x0           ; 4CE700
0006B1: sub x0,a b,y:(r7)+       ; 5F5F44
0006B2: sub x1,b                 ; 20006C
0006B3: tfr x0,a a,y0            ; 21C641
0006B4: tfr x1,b b,y1            ; 21E769
0006B5: move #$10,x0             ; 241000
0006B6: do #<$8,>$6ba            ; 060880 0006B9
0006B8: mac y0,x0,a a,y:(r1)+n1  ; 5E49D2
0006B9: mac x0,y1,b b,y:(r0)+n0  ; 5F48CA
0006BA: move #$74,r0             ; 307400
0006BB: move r0,r1               ; 221100
0006BC: move #$4,r4              ; 340400
0006BD: move l:(r7)+,a           ; 48DF00
0006BE: move l:(r7)-,b           ; 49D700
0006BF: jsr func_000350          ; 0D0350
0006C0: move a,l:(r7)+           ; 485F00
0006C1: move b,l:(r7)+           ; 495F00
0006C2: move x:(r6+$f),x0        ; 023ED4
0006C3: jset #$0,x0,func_0006cb  ; 0AC420 0006CB
0006C5: move #$b4,r0             ; 30B400
0006C6: move r0,r1               ; 221100
0006C7: move #$4,r4              ; 340400
0006C8: move l:(r7)+,a           ; 48DF00
0006C9: move l:(r7)-,b           ; 49D700
0006CA: jsr func_000350          ; 0D0350
0006CB: move a,l:(r7)+           ; 485F00
0006CC: move b,l:(r7)+           ; 495F00
0006CD: move x:(r7-$d),x1        ; 03CFD5
0006CE: tfr x1,a                 ; 200061
0006CF: sub #>$5bf,a             ; 0140C4 0005BF
0006D1: clr a ifmi               ; 202B13
0006D2: move a,y0                ; 21C600
0006D3: sub #>$80,a              ; 0140C4 000080
0006D5: clr a ifmi               ; 202B13
0006D6: move a,y1                ; 21C700
0006D7: move #>$63f,x0           ; 44F400 00063F
0006D9: tfr x1,a                 ; 200061
0006DA: cmp x0,a                 ; 200045
0006DB: tfr x0,a ifgt            ; 202741
0006DC: tst a                    ; 200003
0006DD: clr a ifmi               ; 202B13
0006DE: move a,x1                ; 21C500
0006DF: move x1,r0               ; 20B000
0006E0: move y:(r6+$a),a         ; 022EBE
0006E1: mpyi #>$fffdf3,x1,b      ; 0141E8 FFFDF3
0006E3: maci #>$ffe666,y0,b      ; 0141DA FFE666
0006E5: maci #>$ffd99a,y1,b      ; 0141FA FFD99A
0006E7: asl #$18,b,b             ; 0C1DB1
0006E8: tfr x1,b b,y0            ; 21E669
0006E9: move a,x0                ; 21C400
0006EA: mac y0,x0,a              ; 2000D2
0006EB: clr a ifmi               ; 202B13
0006EC: move a,x0                ; 21C400
0006ED: maci #>$fff912,x0,b      ; 0141CA FFF912
0006EF: move x:(r7-$25),x0       ; 036FD4
0006F0: move y:(r7-$25),a        ; 036FFE
0006F1: move b1,x:(r7-$25)       ; 036FCD
0006F2: move x1,y:(r7-$25)       ; 036FE5
0006F3: add x1,a b1,r2           ; 21B260
0006F4: asr a                    ; 200022
0006F5: add x0,b                 ; 200048
0006F6: asr b a,r1               ; 21D12A
0006F7: move a0,x0               ; 210400
0006F8: move b0,x1               ; 212500
0006F9: move x:(r1+$141a98),y0   ; 0A71C6 141A98
0006FB: move x:(r1+$141a99),y1   ; 0A71C7 141A99
0006FD: tfr y0,a b1,r3           ; 21B351
0006FE: macsu -y0,x0,a           ; 012695
0006FF: macsu y1,x0,a            ; 01268C
000700: move x:(r1+$142158),y0   ; 0A71C6 142158
000702: move x:(r1+$142159),y1   ; 0A71C7 142159
000704: mpysu -y0,x0,b           ; 0127B5
000705: add y0,b a,y0            ; 21C658
000706: macsu y1,x0,b            ; 0126AC
000707: move x:(r3+$142f07),y1   ; 0A73C7 142F07
000709: move x:(r3+$142f06),x0   ; 0A73C4 142F06
00070B: mpysu y1,x1,a            ; 012787
00070C: add x0,a b,y1            ; 21E740
00070D: macsu -x0,x1,a           ; 01269A
00070E: move #>$7fffa4,x0        ; 44F400 7FFFA4
000710: cmp x0,a                 ; 200045
000711: tgt x0,a                 ; 027040
000712: move a,x0                ; 21C400
000713: mpy x0,y1,b              ; 2000C8
000714: mpy y0,x0,a              ; 2000D0
000715: move b,y0                ; 21E600
000716: add #>$800000,a          ; 0140C0 800000
000718: move b,y:$1e             ; 5F1E00
000719: move a,y:$1d             ; 5E1D00
00071A: mpy x0,x0,b              ; 200088
00071B: subr a,b                 ; 20000E
00071C: sub #>$400000,b          ; 0140CC 400000
00071E: andi #$fe,ccr            ; 00FEB9
00071F: move #>$800,a            ; 56F400 000800
000721: do #<$18,>$724           ; 061880 000723
000723: div y0,a                 ; 018050
000724: move b1,y1               ; 21A700
000725: move b0,y0               ; 212600
000726: move a0,x1               ; 210500
000727: mpysu x1,y0,b            ; 0127A6
000728: dmac ss x1,y1,b          ; 0124AF
000729: asl #$a,b,b              ; 0C1D95
00072A: move x:(r0+$141a98),y0   ; 0A70C6 141A98
00072C: move b,y:$1c             ; 5F1C00
00072D: move x:(r0+$142158),y1   ; 0A70C7 142158
00072F: move x:(r2+$142f06),a    ; 0A72CE 142F06
000731: move #>$7fffa4,x0        ; 44F400 7FFFA4
000733: cmp x0,a                 ; 200045
000734: tgt x0,a                 ; 027040
000735: move #$5,r4              ; 340500
000736: move a,x0                ; 21C400
000737: mpy y0,x0,a              ; 2000D0
000738: mpy x0,y1,b              ; 2000C8
000739: move a,y:(r4)+           ; 5E5C00
00073A: move b,y0                ; 21E600
00073B: mpy x0,x0,b b,y:(r4)+    ; 5F5C88
00073C: subr a,b                 ; 20000E
00073D: add #>$400000,b          ; 0140C8 400000
00073F: andi #$fe,ccr            ; 00FEB9
000740: move #>$800,a            ; 56F400 000800
000742: do #<$18,>$745           ; 061880 000744
000744: div y0,a                 ; 018050
000745: move b1,y1               ; 21A700
000746: move b0,y0               ; 212600
000747: move a0,x1               ; 210500
000748: mpysu x1,y0,b            ; 0127A6
000749: dmac ss x1,y1,b          ; 0124AF
00074A: move #$2,n0              ; 380200
00074B: asl #$a,b,b              ; 0C1D95
00074C: move b,y1                ; 21E700
00074D: move #$5,r0              ; 300500
00074E: move #$4,r1              ; 310400
00074F: move y:(r0)+,a           ; 5ED800
000750: add #>$800000,a          ; 0140C0 800000
000752: move b,y:$4              ; 5F0400
000753: move l:(r7),x            ; 42E700
000754: move a,y:(r7)            ; 5E6700
000755: move y:$1d,a             ; 5E9D00
000756: sub x0,a y:(r0)-,y1      ; 4FD044
000757: move y1,x:(r7)           ; 476700
000758: move y:$1e,b             ; 5F9E00
000759: sub x1,b a,y0            ; 21C66C
00075A: tfr x0,a #$10,x0         ; 241041
00075B: tfr x1,b b,y1            ; 21E769
00075C: do #<$8,>$760            ; 060880 00075F
00075E: mac y0,x0,a a,y:(r0)+    ; 5E58D2
00075F: mac x0,y1,b b,y:(r0)+n0  ; 5F48CA
000760: move y:(r0)+,x0          ; 4CD800
000761: move l:(r7)+,ba          ; 4BDF00
000762: sub x0,a y:(r0)-,x1      ; 4DD044
000763: sub x1,b                 ; 20006C
000764: tfr x0,a a,y0            ; 21C641
000765: tfr x1,b b,y1            ; 21E769
000766: move #$10,x0             ; 241000
000767: do #<$8,>$76b            ; 060880 00076A
000769: mac y0,x0,a a,y:(r0)+    ; 5E58D2
00076A: mac x0,y1,b b,y:(r0)+n0  ; 5F48CA
00076B: move #$3,n0              ; 380300
00076C: move n0,n1               ; 231900
00076D: move #$1c,r0             ; 301C00
00076E: move y:$1c,x1            ; 4D9C00
00076F: tfr x1,a y:(r1),b        ; 5FE161
000770: move y:(r7),x0           ; 4CE700
000771: sub x0,a b,y:(r7)+       ; 5F5F44
000772: sub x1,b                 ; 20006C
000773: tfr x0,a a,y0            ; 21C641
000774: tfr x1,b b,y1            ; 21E769
000775: move #$10,x0             ; 241000
000776: do #<$8,>$77a            ; 060880 000779
000778: mac y0,x0,a a,y:(r1)+n1  ; 5E49D2
000779: mac x0,y1,b b,y:(r0)+n0  ; 5F48CA
00077A: move #$0,r5              ; 350000
00077B: move #$1,r2              ; 320100
00077C: move #>$fffffe,n7        ; 77F400 FFFFFE
00077E: move #$74,r0             ; 307400
00077F: move #$72,r1             ; 317200
000780: move #$4,r4              ; 340400
000781: move #$10,x0             ; 241000
000782: move x0,y:(r5)           ; 4C6500
000783: move l:(r7)+,y           ; 43DF00
000784: move l:(r7)+,a           ; 48DF00
000785: move l:(r7)+n7,b         ; 49CF00
000786: move y0,x:(r1)+          ; 465900
000787: move y1,x:(r1)-          ; 475100
000788: jsr func_000365          ; 0D0365
000789: move x:(r1)+,x0          ; 44D900
00078A: move x:(r1),x1           ; 45E100
00078B: move x,l:(r7)+           ; 425F00
00078C: move a,l:(r7)+           ; 485F00
00078D: move b,l:(r7)+           ; 495F00
00078E: move x:(r6+$f),x0        ; 023ED4
00078F: jset #$0,x0,func_00079c  ; 0AC420 00079C
000791: move #$b4,r0             ; 30B400
000792: move #$b2,r1             ; 31B200
000793: move #$4,r4              ; 340400
000794: move l:(r7)+,y           ; 43DF00
000795: move l:(r7)+,a           ; 48DF00
000796: move l:(r7)+n7,b         ; 49CF00
000797: move y0,x:(r1)+          ; 465900
000798: move y1,x:(r1)-          ; 475100
000799: jsr func_000365          ; 0D0365
00079A: move x:(r1)+,x0          ; 44D900
00079B: move x:(r1),x1           ; 45E100
00079C: move x,l:(r7)+           ; 425F00
00079D: move a,l:(r7)+           ; 485F00
00079E: move b,l:(r7)+           ; 495F00
00079F: move x:(r6+$b),y0        ; 022ED6
0007A0: move #$8,a               ; 2E0800
0007A1: andi #$fe,ccr            ; 00FEB9
0007A2: do #<$18,>$7a5           ; 061880 0007A4
0007A4: div y0,a                 ; 018050
0007A5: move a0,y1               ; 210700
0007A6: move y:(r6+$4),a         ; 0216BE
0007A7: asl a #$b2,r0            ; 30B232
0007A8: add #>$800000,a          ; 0140C0 800000
0007AA: clr a ifmi               ; 202B13
0007AB: move #$72,r1             ; 317200
0007AC: move a,x0                ; 21C400
0007AD: mpyi #>$100,x0,b         ; 0141C8 000100
0007AF: mpy x0,x0,a #$70,r5      ; 357080
0007B0: move #$b0,r4             ; 34B000
0007B1: move b,r2                ; 21F200
0007B2: move a,x1                ; 21C500
0007B3: mpyi #>$7deccd,x1,a      ; 0141E0 7DECCD
0007B5: add #>$21333,a           ; 0140C0 021333
0007B7: move x:(r2+$1447c6),x1   ; 0A72C5 1447C6
0007B9: mpy x1,y0,b a,y0         ; 21C6E8
0007BA: mpy y1,y0,a              ; 2000B0
0007BB: move b,x1                ; 21E500
0007BC: move x:(r0)+,x0 a,y0     ; 109800
0007BD: do #<$22,>$7c3           ; 062280 0007C2
0007BF: mpy y0,x0,a x:(r1)+,x0 a,y:(r4)+ ; B299D0
0007C0: asl #$8,a,a              ; 0C1D10
0007C1: mpy y0,x0,b x:(r0)+,x0 b,y:(r5)+ ; B3B8D8
0007C2: asl #$8,b,b              ; 0C1D91
0007C3: bset #$b,sr              ; 0AF96B
0007C4: move #$70,r5             ; 357000
0007C5: move #$b0,r4             ; 34B000
0007C6: move r5,r1               ; 22B100
0007C7: move r4,r0               ; 229000
0007C8: move #$80,x0             ; 248000
0007C9: move y:(r4)+,y0          ; 4EDC00
0007CA: move y:(r5)+,y1          ; 4FDD00
0007CB: do #<$22,>$7cf           ; 062280 0007CE
0007CD: mpy y0,x0,a a,x:(r0)+ y:(r4)+,y0 ; F818D0
0007CE: mpy x0,y1,b b,x:(r1)+ y:(r5)+,y1 ; FD39C8
0007CF: move a,x:(r0)+           ; 565800
0007D0: move b,x:(r1)+           ; 575900
0007D1: bclr #$b,sr              ; 0AF94B
0007D2: move #>$6a3,x0           ; 44F400 0006A3
0007D4: move y:(r7-$14),a        ; 03B7BE
0007D5: cmp x0,a x1,y1           ; 20A745
0007D6: tfr x0,a ifge            ; 202141
0007D7: move l:(r7),x            ; 42E700
0007D8: move a,r0                ; 21D000
0007D9: move x:(r0+$143546),y0   ; 0A70C6 143546
0007DB: mpy y1,y0,b y0,a         ; 20CEB8
0007DC: move #$72,r0             ; 307200
0007DD: move #$4,r4              ; 340400
0007DE: sub x0,a ba,l:(r7)+      ; 4B5F44
0007DF: sub x1,b                 ; 20006C
0007E0: move a,y0                ; 21C600
0007E1: tfr x1,b b,y1            ; 21E769
0007E2: tfr x0,a #$8,x0          ; 240841
0007E3: do #<$10,>$7e7           ; 061080 0007E6
0007E5: mac y0,x0,a a,y:(r4)+    ; 5E5CD2
0007E6: mac x0,y1,b b,y:(r4)+    ; 5F5CCA
0007E7: move #$4,r4              ; 340400
0007E8: move r0,r1               ; 221100
0007E9: move #$b2,r2             ; 32B200
0007EA: move r2,r3               ; 225300
0007EB: move l:(r7)+,a           ; 48DF00
0007EC: move l:(r7)-,b           ; 49D700
0007ED: move x:(r0)+,x0 y:(r4)+,y0 ; F09800
0007EE: move y:(r4)+,x1          ; 4DDC00
0007EF: do #<$10,>$7fa           ; 061080 0007F9
0007F1: mac x1,x0,a a,x:(r1)+ a,y1 ; 1919A2
0007F2: mac -y1,y0,a x:(r2)+,x0  ; 44DAB6
0007F3: mac x1,x0,b b,x:(r3)+ b,y1 ; 1F1BAA
0007F4: mac -y1,y0,b x:(r0)+,x0  ; 44D8BE
0007F5: mac x1,x0,a a,x:(r1)+ a,y1 ; 1919A2
0007F6: mac -y1,y0,a x:(r2)+,x0  ; 44DAB6
0007F7: mac x1,x0,b b,x:(r3)+ b,y1 ; 1F1BAA
0007F8: mac -y1,y0,b x:(r0)+,x0 y:(r4)+,y0 ; F098BE
0007F9: move y:(r4)+,x1          ; 4DDC00
0007FA: move a,x:(r1)+           ; 565900
0007FB: move b,x:(r3)+           ; 575B00
0007FC: move a,l:(r7)+           ; 485F00
0007FD: move b,l:(r7)+           ; 495F00
0007FE: move #$4,r4              ; 340400
0007FF: move #$2,n4              ; 3C0200
000800: move #$72,r0             ; 307200
000801: move r0,r1               ; 221100
000802: move #$b2,r2             ; 32B200
000803: move r2,r3               ; 225300
000804: move l:(r7)+,a           ; 48DF00
000805: move l:(r7)-,b           ; 49D700
000806: jsr func_00037c          ; 0D037C
000807: move a,l:(r7)+           ; 485F00
000808: move b,l:(r7)+           ; 495F00
000809: move #>$eeb88,y0         ; 46F400 0EEB88
00080B: move #>$39c201,y1        ; 47F400 39C201
00080D: lua (r7-$30),r3          ; 042F03
00080E: move r3,r4               ; 227400
00080F: move #$6d,r0             ; 306D00
000810: move #$6c,r2             ; 326C00
000811: move l:(r3)+,x           ; 42DB00
000812: move x0,x:(r0)+          ; 445800
000813: move x1,x:(r0)+          ; 455800
000814: move l:(r3)+,x           ; 42DB00
000815: move x0,x:(r0)+          ; 445800
000816: move x1,x:(r0)+          ; 455800
000817: move x:(r3),x1           ; 45E300
000818: move x1,x:(r0)+          ; 455800
000819: move #$6d,r0             ; 306D00
00081A: move #>$f75277,x1        ; 45F400 F75277
00081C: move #>$fffffd,n0        ; 70F400 FFFFFD
00081E: move x:(r0)+,x0          ; 44D800
00081F: do #<$10,>$828           ; 061080 000827
000821: mpy x1,x0,a x:(r0)+,x0   ; 44D8A0
000822: mac y0,x0,a x:(r0)+,x0   ; 44D8D2
000823: mac x0,y1,a x:(r0)+,x0   ; 44D8C2
000824: mac x0,y1,a x:(r0)+,x0   ; 44D8C2
000825: mac y0,x0,a x:(r0)+n0,x0 ; 44C8D2
000826: mac x1,x0,a b,x:(r2)+    ; 575AA2
000827: tfr a,b x:(r0)+,x0       ; 44D809
000828: move a,x:(r2)+           ; 565A00
000829: tfr x0,a r0,r1           ; 221141
00082A: move x:(r0)+,x1          ; 45D800
00082B: move x,l:(r4)+           ; 425C00
00082C: move x:(r0)+,x0          ; 44D800
00082D: move x:(r0)+,x1          ; 45D800
00082E: move x,l:(r4)+           ; 425C00
00082F: move x:(r0)+,x1          ; 45D800
000830: move x1,x:(r4)           ; 456400
000831: move a,x0                ; 21C400
000832: move r1,r0               ; 223000
000833: move x:(r6+$f),x1        ; 023ED5
000834: jset #$0,x1,func_00084e  ; 0AC520 00084E
000836: move #$ad,r0             ; 30AD00
000837: move #$ac,r2             ; 32AC00
000838: move y:(r3)+,x0          ; 4CDB00
000839: move x0,x:(r0)+          ; 445800
00083A: move l:(r3)+,x           ; 42DB00
00083B: move x0,x:(r0)+          ; 445800
00083C: move x1,x:(r0)+          ; 455800
00083D: move l:(r3)+,x           ; 42DB00
00083E: move x0,x:(r0)+          ; 445800
00083F: move x1,x:(r0)+          ; 455800
000840: move #$ad,r0             ; 30AD00
000841: move #>$f75277,x1        ; 45F400 F75277
000843: move x:(r0)+,x0          ; 44D800
000844: do #<$10,>$84d           ; 061080 00084C
000846: mpy x1,x0,a x:(r0)+,x0   ; 44D8A0
000847: mac y0,x0,a x:(r0)+,x0   ; 44D8D2
000848: mac x0,y1,a x:(r0)+,x0   ; 44D8C2
000849: mac x0,y1,a x:(r0)+,x0   ; 44D8C2
00084A: mac y0,x0,a x:(r0)+n0,x0 ; 44C8D2
00084B: mac x1,x0,a b,x:(r2)+    ; 575AA2
00084C: tfr a,b x:(r0)+,x0       ; 44D809
00084D: move a,x:(r2)+           ; 565A00
00084E: move x0,y:(r4)+          ; 4C5C00
00084F: move x:(r0)+,x0          ; 44D800
000850: move x:(r0)+,x1          ; 45D800
000851: move x,l:(r4)+           ; 425C00
000852: move x:(r0)+,x0          ; 44D800
000853: move x:(r0)+,x1          ; 45D800
000854: move x,l:(r4)+           ; 425C00
000855: move #>$63f,x0           ; 44F400 00063F
000857: move x:(r7-$1a),a        ; 039F9E
000858: tst a                    ; 200003
000859: clr a ifmi               ; 202B13
00085A: cmp x0,a                 ; 200045
00085B: tfr x0,a ifgt            ; 202741
00085C: move a,r0                ; 21D000
00085D: move #>$7fffff,a         ; 56F400 7FFFFF
00085F: move l:(r7),x            ; 42E700
000860: move x:(r0+$1435c6),b    ; 0A70CF 1435C6
000862: sub b,a a,y0             ; 21C614
000863: add y0,a #$6c,r0         ; 306C50
000864: asr a #$4,r4             ; 340422
000865: sub x0,a ba,l:(r7)+      ; 4B5F44
000866: sub x1,b a,y0            ; 21C66C
000867: tfr x0,a #$8,x0          ; 240841
000868: tfr x1,b b,y1            ; 21E769
000869: do #<$10,>$86d           ; 061080 00086C
00086B: mac y0,x0,a a,y:(r4)     ; 5E64D2
00086C: mac x0,y1,b b,x:(r4)+    ; 575CCA
00086D: move #$4,r4              ; 340400
00086E: move r0,r1               ; 221100
00086F: move #$ac,r2             ; 32AC00
000870: move r2,r3               ; 225300
000871: move l:(r7)+,a           ; 48DF00
000872: move l:(r7)+,b           ; 49DF00
000873: move l:(r7),x            ; 42E700
000874: move x0,x:(r0)           ; 446000
000875: move x1,x:(r2)           ; 456200
000876: move x:(r0+$10),x0       ; 024094
000877: move x:(r2+$10),x1       ; 024295
000878: move x,l:(r7)-           ; 425700
000879: jsr func_00038a          ; 0D038A
00087A: move b,l:(r7)-           ; 495700
00087B: move a,l:(r7)            ; 486700
00087C: lua (r7+$3),r7           ; 040737
00087D: move #$4,r4              ; 340400
00087E: move #$6c,r0             ; 306C00
00087F: move r0,r1               ; 221100
000880: move #$ac,r2             ; 32AC00
000881: move r2,r3               ; 225300
000882: move l:(r7)+,a           ; 48DF00
000883: move l:(r7)+,b           ; 49DF00
000884: move l:(r7),x            ; 42E700
000885: move x0,x:(r0)           ; 446000
000886: move x1,x:(r2)           ; 456200
000887: move x:(r0+$10),x0       ; 024094
000888: move x:(r2+$10),x1       ; 024295
000889: move x,l:(r7)-           ; 425700
00088A: jsr func_00038a          ; 0D038A
00088B: move b,l:(r7)-           ; 495700
00088C: move a,l:(r7)            ; 486700
00088D: lua (r7+$3),r7           ; 040737
