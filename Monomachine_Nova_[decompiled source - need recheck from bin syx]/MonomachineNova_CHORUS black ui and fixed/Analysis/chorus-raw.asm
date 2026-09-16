147661: move    #>$114000,a0                                          ; 50f400 114000
147663: move    y:>$124,x1                                            ; 4df000 000124
147665: maci    #>$2000,x1,a                                          ; 0141e2 002000
147667: move    a0,x0                                                 ; 210400
147668: move    x0,y:(r6+$10)                                         ; 0246a4
147669: move    #$80,x0                                               ; 248000
14766a: move    x0,x:(r6+$11)                                         ; 0246c4
14766b: move    #$0,x0                                                ; 240000
14766c: move    x0,x:(r6+$12)                                         ; 024e84
14766d: move    y:(r6+$4),x0                                          ; 0216b4
14766e: mpyi    #>$fffc20,x0,a                                        ; 0141c0 fffc20
147670: move    a,x:(r6+$1d)                                          ; 0276ce
147671: move    #>$14a000,x0                                          ; 44f400 14a000
147673: move    x0,y:(r6+$1b)                                         ; 026ee4
147674: move    #$0,x0                                                ; 240000
147675: move    x0,y:(r6+$21)                                         ; 0286e4
147676: move    x0,y:(r6+$20)                                         ; 0286a4
147677: move    x0,y:(r6+$1d)                                         ; 0276e4
147678: move    x0,x:(r6+$22)                                         ; 028e84
147679: move    x0,y:(r6+$22)                                         ; 028ea4
14767a: rts                                                           ; 00000c

14767b: move    #$0,x0                                                ; 240000
14767c: move    x0,x:(r6-$19)                                         ; 039ec4
14767d: move    #>$7fffff,b                                           ; 57f400 7fffff
14767f: move    y:(r6+$21),a                                          ; 0286fe
147680: cmp     #>$80,a                                               ; 0140c5 000080
147682: bge     func_147687                                           ; 0d1041 000005
147684: add     #>$1,a                                                ; 0140c0 000001
147686: clr     b                                                     ; 20001b

func_147687:                                                          ; (callers: $147682)
147687: move    a,y:(r6+$21)                                          ; 0286ee
147688: move    b,y:(r6+$20)                                          ; 0286af
147689: move    #>$0,r4                                               ; 64f400 000000
14768b: move    y:(r6+$b),x0                                          ; 022ef4
14768c: mpy     x0,x0,a                                               ; 200080
14768d: move    x:(r0)+,x1                                            ; 45d800
14768e: move    a,x0                                                  ; 21c400
14768f: mpy     x1,x0,a         x:(r0)+,x1                            ; 45d8a0
147690: asl     #$2,a,a                                               ; 0c1d04
147691: mpy     x1,x0,b         x:(r0)+,x1                            ; 45d8a8
147692: asl     #$2,b,b                                               ; 0c1d85
147693: do      #<$10,>$147699                                        ; 061080 147698
147695: mpy     x1,x0,a         x:(r0)+,x1      a,y:(r4)+             ; b698a0
147696: asl     #$2,a,a                                               ; 0c1d04
147697: mpy     x1,x0,b         x:(r0)+,x1      b,y:(r4)+             ; b798a8
147698: asl     #$2,b,b                                               ; 0c1d85
147699: move    y:(r6+$6),x0                                          ; 021eb4
14769a: mpy     x0,x0,a                                               ; 200080
14769b: move    a,x0                                                  ; 21c400
14769c: mpyi    #>$956,x0,a                                           ; 0141c0 000956
14769e: move    #>$20,r0                                              ; 60f400 000020
1476a0: move    r0,r4                                                 ; 221400
1476a1: move    a,x0                                                  ; 21c400
1476a2: move    x:(r6+$11),a                                          ; 0246de
1476a3: move    x:(r6+$12),b                                          ; 024e9f
1476a4: move    a,x1                                                  ; 21c500
1476a5: bset    #$14,sr                                               ; 0af974
1476a6: do      #<$10,>$1476aa                                        ; 061080 1476a9
1476a8: mac     -x1,x0,b        b,x:(r0)+       b,y0                  ; 1e18ae
1476a9: mac     y0,x0,a         a,x1            a,y:(r4)+             ; 165cd2
1476aa: bclr    #$14,sr                                               ; 0af954
1476ab: move    a,x:(r6+$11)                                          ; 0246ce
1476ac: move    b,x:(r6+$12)                                          ; 024e8f
1476ad: move    y:(r6+$24),a                                          ; 0296be
1476ae: move    y:(r6+$9),y0                                          ; 0226f6
1476af: move    a,y1                                                  ; 21c700
1476b0: maci    #>$28f5c,y0,a                                         ; 0141d2 028f5c
1476b2: maci    #>$fd70a4,y1,a                                        ; 0141f2 fd70a4
1476b4: move    a,y:(r6+$24)                                          ; 0296ae
1476b5: move    a,y0                                                  ; 21c600
1476b6: mpyi    #>$800,y0,a                                           ; 0141d0 000800
1476b8: move    a,r0                                                  ; 21d000
1476b9: move    x:(r0+$14a000),a                                      ; 0a70ce 14a000
1476bb: move    x:(r0+$14a800),b                                      ; 0a70cf 14a800
1476bd: move    y:(r6+$25),x0                                         ; 0296f4
1476be: move    y:(r6+$26),x1                                         ; 029eb5
1476bf: move    b,y:(r6+$25)                                          ; 0296ef
1476c0: move    a,y:(r6+$26)                                          ; 029eae
1476c1: sub     x1,a                                                  ; 200064
1476c2: sub     x0,b                                                  ; 20004c
1476c3: tfr     x1,a            a,y0                                  ; 21c661
1476c4: tfr     x0,b            b,y1                                  ; 21e749
1476c5: move    #$8,x0                                                ; 240800
1476c6: move    #>$80,r0                                              ; 60f400 000080
1476c8: do      #<$10,>$1476cc                                        ; 061080 1476cb
1476ca: mac     y0,x0,a         a,y:(r0)                              ; 5e60d2
1476cb: mac     x0,y1,b         b,x:(r0)+                             ; 5758ca
1476cc: move    #>$80,r5                                              ; 65f400 000080
1476ce: move    #>$20,r0                                              ; 60f400 000020
1476d0: move    r0,r4                                                 ; 221400
1476d1: move    #>$50,r1                                              ; 61f400 000050
1476d3: move    r1,r2                                                 ; 223200
1476d4: move    x:(r5),y0                                             ; 46e500
1476d5: move    x:(r0)+,x0      y:(r5)+,y1                            ; f1b800
1476d6: do      #<$10,>$1476de                                        ; 061080 1476dd
1476d8: mpy     y0,x0,a         y:(r4)+,x1                            ; 4ddcd0
1476d9: mac     y1,x1,a                                               ; 2000f2
1476da: mpy     x1,y0,b         x:(r5),y0                             ; 46e5e8
1476db: mac     -x0,y1,b        x:(r0)+,x0      y:(r5)+,y1            ; f1b8ce
1476dc: move    a,x:(r1)+                                             ; 565900
1476dd: move    b,y:(r2)+                                             ; 5f5a00
1476de: move    #>$20,r1                                              ; 61f400 000020
1476e0: move    #>$50,r0                                              ; 60f400 000050
1476e2: move    #>$30,r3                                              ; 63f400 000030
1476e4: move    #>$60,r2                                              ; 62f400 000060
1476e6: move    #$c0,y0                                               ; 26c000
1476e7: move    #>$6ed9ec,y1                                          ; 47f400 6ed9ec
1476e9: move    x:(r1),x0                                             ; 44e100
1476ea: do      #<$10,>$1476f2                                        ; 061080 1476f1
1476ec: mpy     y0,x0,a         y:(r1)+,x1                            ; 4dd9d0
1476ed: mac     y1,x1,a         x:(r0),x0                             ; 44e0f2
1476ee: mpy     y0,x0,b         y:(r0)+,x1                            ; 4dd8d8
1476ef: mac     y1,x1,b         x:(r1),x0                             ; 44e1fa
1476f0: move    a,x:(r3)+                                             ; 565b00
1476f1: move    b,x:(r2)+                                             ; 575a00
1476f2: move    #$c0,y0                                               ; 26c000
1476f3: move    #>$912614,y1                                          ; 47f400 912614
1476f5: move    #>$20,r1                                              ; 61f400 000020
1476f7: move    #>$50,r0                                              ; 60f400 000050
1476f9: move    x:(r1),x0                                             ; 44e100
1476fa: do      #<$10,>$147702                                        ; 061080 147701
1476fc: mpy     y0,x0,a         y:(r1)+,x1                            ; 4dd9d0
1476fd: mac     y1,x1,a         x:(r0),x0                             ; 44e0f2
1476fe: mpy     y0,x0,b         y:(r0)+,x1                            ; 4dd8d8
1476ff: mac     y1,x1,b         x:(r1),x0                             ; 44e1fa
147700: move    a,x:(r3)+                                             ; 565b00
147701: move    b,x:(r2)+                                             ; 575a00
147702: move    y:(r6+$4),x0                                          ; 0216b4
147703: mpyi    #>$fffc20,x0,a                                        ; 0141c0 fffc20
147705: add     #>$fffff0,a                                           ; 0140c0 fffff0
147707: move    x:(r6+$1d),b                                          ; 0276df
147708: move    y:(r6+$1d),b0                                         ; 0276f9
147709: move    b,l:?:>$ff                                            ; 497000 0000ff
14770b: move    a,x0                                                  ; 21c400
14770c: move    b,x1                                                  ; 21e500
14770d: maci    #>$28f5c,x0,b                                         ; 0141ca 028f5c
14770f: maci    #>$fd70a4,x1,b                                        ; 0141ea fd70a4
147711: move    b1,x:(r6+$1d)                                         ; 0276cd
147712: move    b0,y:(r6+$1d)                                         ; 0276e9
147713: move    x:(r6+$22),a                                          ; 028e9e
147714: move    y:(r6+$22),a0                                         ; 028eb8
147715: move    y:(r6+$5),x0                                          ; 0216f4
147716: move    a,x1                                                  ; 21c500
147717: maci    #>$fd70a4,x0,a                                        ; 0141c2 fd70a4
147719: maci    #>$fd70a4,x1,a                                        ; 0141e2 fd70a4
14771b: move    a1,x:(r6+$22)                                         ; 028e8c
14771c: move    a0,y:(r6+$22)                                         ; 028ea8
14771d: sub     #>$fffff0,b                                           ; 0140cc fffff0
14771f: asl     #$8,b,b                                               ; 0c1d91
147720: move    a,x1                                                  ; 21c500
147721: move    b,x0                                                  ; 21e400
147722: mpy     x1,x0,a                                               ; 2000a0
147723: move    y:(r6+$23),x0                                         ; 028ef4
147724: move    a,y:(r6+$23)                                          ; 028eee
147725: sub     x0,a            #>$80000,y1                           ; 47f444 080000
147727: tfr     x0,a            a,y0                                  ; 21c641
147728: move    #>$80,r3                                              ; 63f400 000080
14772a: move    #$f,m3                                                ; 050fa3
14772b: do      #<$10,>$14772e                                        ; 061080 14772d
14772d: mac     y1,y0,a         a,x:(r3)+                             ; 565bb2
14772e: move    #>$20,r1                                              ; 61f400 000020
147730: move    #>$1f,r2                                              ; 62f400 00001f
147732: move    x:(r6+$1d),b                                          ; 0276df
147733: move    y:(r6+$1d),b0                                         ; 0276f9
147734: move    l:?:>$ff,y                                            ; 43f000 0000ff
147736: sub     y,b                                                   ; 20003c
147737: add     #>$10,b                                               ; 0140c8 000010
147739: asr     #$4,b,b                                               ; 0c1c89
14773a: move    b1,y1                                                 ; 21a700
14773b: move    b0,y0                                                 ; 212600
14773c: move    l:(r2),a                                              ; 48e200
14773d: move    x:(r1)+,x1                                            ; 45d900
14773e: move    x:(r3)+,x0                                            ; 44db00
14773f: do      #<$6,>$14774a                                         ; 060680 147749
147741: move    l:?:>$ff,b                                            ; 49f000 0000ff
147743: do      #<$10,>$147749                                        ; 061080 147748
147745: mpy     x1,x0,a         a,l:(r2)+                             ; 485aa0
147746: asr     #$8,a,a                                               ; 0c1c10
147747: add     b,a             x:(r1)+,x1                            ; 45d910
147748: add     y,b             x:(r3)+,x0                            ; 44db38
147749: nop                                                           ; 000000
14774a: move    a,l:(r2)+                                             ; 485a00
14774b: move    #>$ffffff,m3                                          ; 05f423 ffffff
14774d: move    y:(r6+$10),r0                                         ; 0b76d0 000010
14774f: move    #>$7ff,m0                                             ; 05f420 0007ff
147751: move    m0,m4                                                 ; 0464a0
147752: move    r0,a                                                  ; 220e00
147753: add     #>$800,a                                              ; 0140c0 000800
147755: move    a,r1                                                  ; 21d100
147756: move    #>$20,r2                                              ; 62f400 000020
147758: move    #>$80,r3                                              ; 63f400 000080
14775a: do      #<$2,>$14776a                                         ; 060280 147769
14775c: move    x:(r2),n0                                             ; 70e200
14775d: lua     (r0)+n0,r4                                            ; 044814
14775e: do      #<$30,>$147769                                        ; 063080 147768
147760: move    y:(r2)+,y1                                            ; 4fda00
147761: move    y:(r4)+,x0                                            ; 4cdc00
147762: move    y:(r4)+,x1                                            ; 4ddc00
147763: mpysu   x1,y1,a                                               ; 01278f
147764: macsu   -x0,y1,a                                              ; 012694
147765: asr     a               x:(r2),n0                             ; 70e222
147766: add     x0,a                                                  ; 200040
147767: lua     (r0)+n0,r4                                            ; 044814
147768: move    a,x:(r3)+                                             ; 565b00
147769: move    r1,r0                                                 ; 223000
14776a: move    #>$ffffff,m4                                          ; 05f424 ffffff
14776c: move    #>$ffffff,m0                                          ; 05f420 ffffff
14776e: move    #>$3e,r4                                              ; 64f400 00003e
147770: move    #>$80,r0                                              ; 60f400 000080
147772: move    #$5f,m0                                               ; 055fa0
147773: move    #$10,n0                                               ; 381000
147774: move    #>$2aaaab,y1                                          ; 47f400 2aaaab
147776: move    y:(r6+$20),x0                                         ; 0286b4
147777: mpy     x0,y1,a                                               ; 2000c0
147778: move    x:(r0)+n0,x1                                          ; 45c800
147779: move    a,y1                                                  ; 21c700
14777a: do      #<$10,>$147783                                        ; 061080 147782
14777c: mpy     y1,x1,a         x:(r0)+n0,x1    a,y:(r4)+             ; b688f0
14777d: mac     y1,x1,a         x:(r0)+n0,x1                          ; 45c8f2
14777e: mac     y1,x1,a         x:(r0)+n0,x1                          ; 45c8f2
14777f: mpy     y1,x1,b         x:(r0)+n0,x1    b,y:(r4)+             ; b788f8
147780: mac     y1,x1,b         x:(r0)+n0,x1                          ; 45c8fa
147781: mac     y1,x1,b         (r0)+                                 ; 2058fa
147782: move    x:(r0)+n0,x1                                          ; 45c800
147783: move    a,y:(r4)+                                             ; 5e5c00
147784: move    b,y:(r4)+                                             ; 5f5c00
147785: move    #>$ffffff,m0                                          ; 05f420 ffffff
147787: move    #>$144ac7,r2                                          ; 62f400 144ac7
147789: move    y:(r6+$a),b                                           ; 022ebf
14778a: asr     #$10,b,b                                              ; 0c1ca1
14778b: move    #>$40,r5                                              ; 65f400 000040
14778d: move    #>$60,r4                                              ; 64f400 000060
14778f: move    b,n2                                                  ; 21fa00
147790: move    y:(r6+$1e),a                                          ; 027ebe
147791: move    y:(r6+$1f),b                                          ; 027eff
147792: move    #>$0,r0                                               ; 60f400 000000
147794: move    x:(r2+n2),x0                                          ; 44ea00
147795: move    y:(r5)+,y1                                            ; 4fdd00
147796: do      #<$10,>$14779c                                        ; 061080 14779b
147798: mac     x0,y1,a         a,x1            a,y:(r4)+             ; 165cc2
147799: mac     -x1,x0,a        y:(r5)+,y1                            ; 4fdda6
14779a: mac     x0,y1,b         b,x1            b,y:(r4)+             ; 1f5cca
14779b: mac     -x1,x0,b        y:(r5)+,y1                            ; 4fddae
14779c: move    a,y:(r6+$1e)                                          ; 027eae
14779d: move    b,y:(r6+$1f)                                          ; 027eef
14779e: move    #>$0,r4                                               ; 64f400 000000
1477a0: move    #>$60,r5                                              ; 65f400 000060
1477a2: move    #$2,n4                                                ; 3c0200
1477a3: move    n4,n5                                                 ; 239d00
1477a4: move    y:(r6+$10),a                                          ; 0246be
1477a5: move    a,r0                                                  ; 21d000
1477a6: add     #>$800,a                                              ; 0140c0 000800
1477a8: move    #>$7ff,m0                                             ; 05f420 0007ff
1477aa: move    m0,m1                                                 ; 0461a0
1477ab: move    a,r1                                                  ; 21d100
1477ac: move    y:(r6+$8),a                                           ; 0226be
1477ad: neg     a                                                     ; 200036
1477ae: sub     #>$fd71,a                                             ; 0140c4 00fd71
1477b0: move    #>$7fffff,b                                           ; 57f400 7fffff
1477b2: sub     a,b                                                   ; 20001c
1477b3: asr     b                                                     ; 20002a
1477b4: move    a,x1                                                  ; 21c500
1477b5: move    b,x0                                                  ; 21e400
1477b6: move    y:(r4)+n4,y0                                          ; 4ecc00
1477b7: do      #<$10,>$1477bc                                        ; 061080 1477bb
1477b9: mpy     y0,x0,a         y:(r5)+n5,y0                          ; 4ecdd0
1477ba: mac     x1,y0,a         y:(r4)+n4,y0                          ; 4ecce2
1477bb: move    a,x:(r0)+                                             ; 565800
1477bc: move    #>$1,r4                                               ; 64f400 000001
1477be: move    #>$61,r5                                              ; 65f400 000061
1477c0: move    r0,y:(r6+$10)                                         ; 0b7690 000010
1477c2: move    y:(r4)+n4,y0                                          ; 4ecc00
1477c3: do      #<$10,>$1477c8                                        ; 061080 1477c7
1477c5: mpy     y0,x0,a         y:(r5)+n5,y0                          ; 4ecdd0
1477c6: mac     x1,y0,a         y:(r4)+n4,y0                          ; 4ecce2
1477c7: move    a,x:(r1)+                                             ; 565900
1477c8: move    #>$ffffff,m0                                          ; 05f420 ffffff
1477ca: move    m0,m1                                                 ; 0461a0
1477cb: move    #>$0,r4                                               ; 64f400 000000
1477cd: move    #>$40,r5                                              ; 65f400 000040
1477cf: move    y:(r6+$7),x0                                          ; 021ef4
1477d0: move    #>$7fffff,a                                           ; 56f400 7fffff
1477d2: sub     x0,a                                                  ; 200044
1477d3: move    y:(r4)+,y0                                            ; 4edc00
1477d4: move    a,x1                                                  ; 21c500
1477d5: do      #<$10,>$1477dd                                        ; 061080 1477dc
1477d7: mpy     x1,y0,a         y:(r5)+,y0                            ; 4edde0
1477d8: mac     y0,x0,a         y:(r4)+,y0                            ; 4edcd2
1477d9: mpy     x1,y0,b         y:(r5)+,y0                            ; 4edde8
1477da: mac     y0,x0,b         y:(r4)+,y0                            ; 4edcda
1477db: move    a,y:(r7)+                                             ; 5e5f00
1477dc: move    b,y:(r7)+                                             ; 5f5f00
1477dd: rts                                                           ; 00000c
