; РАСПАКОВКА ЯЧЕЕК-ЛОКОВ ('COPY NOTE'/'COPY MIDI NOTE', строка меню $251D85, вызов 0x2E28).
; Фаза 1 (0x351B6): d7 = сумма popcount(масок ячеек дорожек 0..T-1), маска дорожки d6
;   = байты $716D58 + slot*0x1978 + d6*8 (+0x274), T = $224800 = текущая дорожка.
; Фаза 2 (0x352CE): по дорожке T: байт маски -> a4; бит d4 установлен ->
;   out[$29B46E + шаг] = slot+0x557 + d7*64 + a5 (строка=ячейка, колонка=параметр a5);
;   d7++ НА КАЖДУЮ УСТАНОВЛЕННУЮ ЯЧЕЙКУ (ординал ячейки track-major); нет бита -> FF.
; Хвост (0x35238): $3090C(d4,a5,d6): $FE -> нота из +0x2A4, иначе машина.
; arg a5 = индекс параметра (колонка строки) — приходит из $2579F0 (геттер 0x1A23E).

; ---- phase1_head (0x3518A..0x35200, section_0 offset) ----
03518a: 4fefffd8         lea.l -$28(a7), a7
03518e: 48d73cfc         movem.l d2-d7/a2-a5, (a7)
035192: 2a6f002c         movea.l $2c(a7), a5
035196: 4286             clr.l d6
035198: 4287             clr.l d7
03519a: 601a             bra.b $351b6
03519c: 0102             btst.l d0, d2
03519e: 6702             beq.b $351a2
0351a0: 5287             addq.l #$1, d7
0351a2: 5280             addq.l #$1, d0
0351a4: 7608             moveq #$8, d3
0351a6: b680             cmp.l d0, d3
0351a8: 66f2             bne.b $3519c
0351aa: 5281             addq.l #$1, d1
0351ac: 5288             addq.l #$1, a0
0351ae: b681             cmp.l d1, d3
0351b0: 66000112         bne.w $352c4
0351b4: 5286             addq.l #$1, d6
0351b6: 4eb900224800     jsr $224800.l
0351bc: 263900716ae0     move.l $716ae0.l, d3
0351c2: b086             cmp.l d6, d0
0351c4: 6f000108         ble.w $352ce
0351c8: 2003             move.l d3, d0
0351ca: e988             lsl.l #$4, d0
0351cc: 2203             move.l d3, d1
0351ce: ed89             lsl.l #$6, d1
0351d0: 9280             sub.l d0, d1
0351d2: 2001             move.l d1, d0
0351d4: e988             lsl.l #$4, d0
0351d6: d280             add.l d0, d1
0351d8: 9283             sub.l d3, d1
0351da: d286             add.l d6, d1
0351dc: e789             lsl.l #$3, d1
0351de: 2041             movea.l d1, a0
0351e0: d1fc00716d58     adda.l #$716d58, a0
0351e6: 4281             clr.l d1
0351e8: 600000da         bra.w $352c4
0351ec: 200c             move.l a4, d0
0351ee: 0900             btst.l d4, d0
0351f0: 672a             beq.b $3521c
0351f2: 2407             move.l d7, d2
0351f4: ed8a             lsl.l #$6, d2
0351f6: 2203             move.l d3, d1
0351f8: e989             lsl.l #$4, d1
0351fa: 2003             move.l d3, d0
0351fc: ed88             lsl.l #$6, d0
0351fe: 9081             sub.l d1, d0

; ---- bitloop (0x351EC..0x35240, section_0 offset) ----
0351ec: 200c             move.l a4, d0
0351ee: 0900             btst.l d4, d0
0351f0: 672a             beq.b $3521c
0351f2: 2407             move.l d7, d2
0351f4: ed8a             lsl.l #$6, d2
0351f6: 2203             move.l d3, d1
0351f8: e989             lsl.l #$4, d1
0351fa: 2003             move.l d3, d0
0351fc: ed88             lsl.l #$6, d0
0351fe: 9081             sub.l d1, d0
035200: 2200             move.l d0, d1
035202: e989             lsl.l #$4, d1
035204: d081             add.l d1, d0
035206: 9083             sub.l d3, d0
035208: e788             lsl.l #$3, d0
03520a: d480             add.l d0, d2
03520c: d48d             add.l a5, d2
03520e: 41f90071703a     lea.l $71703a.l, a0
035214: 14b02801         move.b $1(a0, d2.l), (a2)
035218: 5287             addq.l #$1, d7
03521a: 6004             bra.b $35220
03521c: 12bcffff         move.b #$ff, (a1)
035220: 5284             addq.l #$1, d4
035222: 528a             addq.l #$1, a2
035224: 5289             addq.l #$1, a1
035226: 7208             moveq #$8, d1
035228: b284             cmp.l d4, d1
03522a: 66c0             bne.b $351ec
03522c: 528b             addq.l #$1, a3
03522e: 5085             addq.l #$8, d5
035230: 7038             moveq #$38, d0
035232: b085             cmp.l d5, d0
035234: 660000b8         bne.w $352ee
035238: 4284             clr.l d4
03523a: 2f04             move.l d4, -(a7)
03523c: 2f0d             move.l a5, -(a7)
03523e: 2f06             move.l d6, -(a7)

; ---- phase2_setup (0x352CE..0x35300, section_0 offset) ----
0352ce: 2003             move.l d3, d0
0352d0: e988             lsl.l #$4, d0
0352d2: 2203             move.l d3, d1
0352d4: ed89             lsl.l #$6, d1
0352d6: 9280             sub.l d0, d1
0352d8: 2001             move.l d1, d0
0352da: e988             lsl.l #$4, d0
0352dc: d280             add.l d0, d1
0352de: 9283             sub.l d3, d1
0352e0: d286             add.l d6, d1
0352e2: e789             lsl.l #$3, d1
0352e4: 2641             movea.l d1, a3
0352e6: d7fc00716d58     adda.l #$716d58, a3
0352ec: 4285             clr.l d5
0352ee: 1213             move.b (a3), d1
0352f0: 49c1             extb.l d1
0352f2: 2841             movea.l d1, a4
0352f4: 2245             movea.l d5, a1
0352f6: d3fc0029b46e     adda.l #$29b46e, a1
0352fc: 2449             movea.l a1, a2
0352fe: 4284             clr.l d4

; ---- note_tail (0x35238..0x352C4, section_0 offset) ----
035238: 4284             clr.l d4
03523a: 2f04             move.l d4, -(a7)
03523c: 2f0d             move.l a5, -(a7)
03523e: 2f06             move.l d6, -(a7)
035240: 4ebab6ca         jsr $3090c(pc)
035244: 2040             movea.l d0, a0
035246: 4fef000c         lea.l $c(a7), a7
03524a: 72ff             moveq #$ff, d1
03524c: b280             cmp.l d0, d1
03524e: 670000b4         beq.w $35304
035252: 2639002bda2e     move.l $2bda2e.l, d3
035258: 70fe             moveq #$fe, d0
03525a: b088             cmp.l a0, d0
03525c: 662a             bne.b $35288
03525e: 2406             move.l d6, d2
035260: ed8a             lsl.l #$6, d2
035262: 2203             move.l d3, d1
035264: e989             lsl.l #$4, d1
035266: 2003             move.l d3, d0
035268: ed88             lsl.l #$6, d0
03526a: 9081             sub.l d1, d0
03526c: 2200             move.l d0, d1
03526e: e989             lsl.l #$4, d1
035270: d081             add.l d1, d0
035272: 9083             sub.l d3, d0
035274: e788             lsl.l #$3, d0
035276: d480             add.l d0, d2
035278: d48d             add.l a5, d2
03527a: 41f900716d88     lea.l $716d88.l, a0
035280: 10302800         move.b (a0, d2.l), d0
035284: 49c0             extb.l d0
035286: 602c             bra.b $352b4
035288: 2203             move.l d3, d1
03528a: e989             lsl.l #$4, d1
03528c: 2003             move.l d3, d0
03528e: ed88             lsl.l #$6, d0
035290: 9081             sub.l d1, d0
035292: 2200             move.l d0, d1
035294: e989             lsl.l #$4, d1
035296: d081             add.l d1, d0
035298: 9083             sub.l d3, d0
03529a: 41f00c00         lea.l (a0, d0.l * 4), a0
03529e: 2008             move.l a0, d0
0352a0: d080             add.l d0, d0
0352a2: 2040             movea.l d0, a0
0352a4: d1fc00716ae0     adda.l #$716ae0, a0
0352aa: 4280             clr.l d0
0352ac: 302817fc         move.w $17fc(a0), d0
0352b0: 7209             moveq #$9, d1
0352b2: e2a8             lsr.l d1, d0
0352b4: 41f90029b46e     lea.l $29b46e.l, a0
0352ba: 11804840         move.b d0, $40(a0, d4.l)
0352be: 5284             addq.l #$1, d4
0352c0: 6000ff78         bra.w $3523a
