; $35338(marker, track, step) — УДАЛЕНИЕ ноты/блокировок (undo):
;   marker $FE: notes[track][step] = FF, затем $2E9AA(track,step,$80).
;   count(+0x554)-- и СДВИГ ВЛЕВО записей таблицы с индекса найденной
;   (move.w (a0),-$2(a0) от slot+0x17FA+2*index до count-1); повтор $3090C(1,..).

; ---- delete (0x35338..0x35470, section_0 offset) ----
035338: 4fefffe8         lea.l -$18(a7), a7
03533c: 48d7047c         movem.l d2-d6/a2, (a7)
035340: 226f001c         movea.l $1c(a7), a1
035344: 2c2f0020         move.l $20(a7), d6
035348: 2a2f0024         move.l $24(a7), d5
03534c: 2439002bda2e     move.l $2bda2e.l, d2
035352: 70fe             moveq #$fe, d0
035354: b089             cmp.l a1, d0
035356: 66000118         bne.w $35470
03535a: 45f900716d88     lea.l $716d88.l, a2
035360: 2606             move.l d6, d3
035362: ed8b             lsl.l #$6, d3
035364: 2002             move.l d2, d0
035366: e988             lsl.l #$4, d0
035368: 2202             move.l d2, d1
03536a: ed89             lsl.l #$6, d1
03536c: 9280             sub.l d0, d1
03536e: 2001             move.l d1, d0
035370: e988             lsl.l #$4, d0
035372: d280             add.l d0, d1
035374: 9282             sub.l d2, d1
035376: e789             lsl.l #$3, d1
035378: d283             add.l d3, d1
03537a: d285             add.l d5, d1
03537c: 50c4             st.b d4
03537e: 15841800         move.b d4, (a2, d1.l)
035382: 48780080         pea.l $80.w
035386: 2f05             move.l d5, -(a7)
035388: 2f06             move.l d6, -(a7)
03538a: 4eba961e         jsr $2e9aa(pc)
03538e: 4fef000c         lea.l $c(a7), a7
035392: 4a80             tst.l d0
035394: 6700014c         beq.w $354e2
035398: 48780001         pea.l $1.w
03539c: 2f05             move.l d5, -(a7)
03539e: 2f06             move.l d6, -(a7)
0353a0: 4ebab56a         jsr $3090c(pc)
0353a4: 2240             movea.l d0, a1
0353a6: 2439002bda2e     move.l $2bda2e.l, d2
0353ac: 4fef000c         lea.l $c(a7), a7
0353b0: 2202             move.l d2, d1
0353b2: e989             lsl.l #$4, d1
0353b4: 2002             move.l d2, d0
0353b6: ed88             lsl.l #$6, d0
0353b8: 78fe             moveq #$fe, d4
0353ba: b889             cmp.l a1, d4
0353bc: 6618             bne.b $353d6
0353be: 9081             sub.l d1, d0
0353c0: 2200             move.l d0, d1
0353c2: e989             lsl.l #$4, d1
0353c4: d081             add.l d1, d0
0353c6: 9082             sub.l d2, d0
0353c8: e788             lsl.l #$3, d0
0353ca: d083             add.l d3, d0
0353cc: d085             add.l d5, d0
0353ce: 18320800         move.b (a2, d0.l), d4
0353d2: 49c4             extb.l d4
0353d4: 6024             bra.b $353fa
0353d6: 9081             sub.l d1, d0
0353d8: 2200             move.l d0, d1
0353da: e989             lsl.l #$4, d1
0353dc: d081             add.l d1, d0
0353de: 9082             sub.l d2, d0
0353e0: 41f10c00         lea.l (a1, d0.l * 4), a0
0353e4: 2008             move.l a0, d0
0353e6: d080             add.l d0, d0
0353e8: 2040             movea.l d0, a0
0353ea: d1fc00716ae0     adda.l #$716ae0, a0
0353f0: 4284             clr.l d4
0353f2: 382817fc         move.w $17fc(a0), d4
0353f6: 7009             moveq #$9, d0
0353f8: e0ac             lsr.l d0, d4
0353fa: 2606             move.l d6, d3
0353fc: ed8b             lsl.l #$6, d3
0353fe: 2202             move.l d2, d1
035400: e989             lsl.l #$4, d1
035402: 2002             move.l d2, d0
035404: ed88             lsl.l #$6, d0
035406: 9081             sub.l d1, d0
035408: 2200             move.l d0, d1
03540a: e989             lsl.l #$4, d1
03540c: d081             add.l d1, d0
03540e: 9082             sub.l d2, d0
035410: 2400             move.l d0, d2
035412: e78a             lsl.l #$3, d2
035414: d682             add.l d2, d3
035416: d685             add.l d5, d3
035418: 41f900716d88     lea.l $716d88.l, a0
03541e: 11843800         move.b d4, (a0, d3.l)
035422: 41f900717038     lea.l $717038.l, a0
035428: 12302800         move.b (a0, d2.l), d1
03542c: 5381             subq.l #$1, d1
03542e: 11812800         move.b d1, (a0, d2.l)
035432: 0281000000ff     andi.l #$ff, d1
035438: 41f10c00         lea.l (a1, d0.l * 4), a0
03543c: 2008             move.l a0, d0
03543e: d080             add.l d0, d0
035440: 2040             movea.l d0, a0
035442: d1fc007182de     adda.l #$7182de, a0
035448: 2009             move.l a1, d0
03544a: 6008             bra.b $35454
03544c: 5280             addq.l #$1, d0
03544e: 3150fffe         move.w (a0), -$2(a0)
035452: 5488             addq.l #$2, a0
035454: b280             cmp.l d0, d1
035456: 6ef4             bgt.b $3544c
035458: 48780001         pea.l $1.w
03545c: 2f05             move.l d5, -(a7)
03545e: 2f06             move.l d6, -(a7)
035460: 4ebab4aa         jsr $3090c(pc)
035464: 4fef000c         lea.l $c(a7), a7
035468: 72ff             moveq #$ff, d1
03546a: b280             cmp.l d0, d1
03546c: 6674             bne.b $354e2
03546e: 6062             bra.b $354d2

; ---- shift (0x35438..0x35470, section_0 offset) ----
035438: 41f10c00         lea.l (a1, d0.l * 4), a0
03543c: 2008             move.l a0, d0
03543e: d080             add.l d0, d0
035440: 2040             movea.l d0, a0
035442: d1fc007182de     adda.l #$7182de, a0
035448: 2009             move.l a1, d0
03544a: 6008             bra.b $35454
03544c: 5280             addq.l #$1, d0
03544e: 3150fffe         move.w (a0), -$2(a0)
035452: 5488             addq.l #$2, a0
035454: b280             cmp.l d0, d1
035456: 6ef4             bgt.b $3544c
035458: 48780001         pea.l $1.w
03545c: 2f05             move.l d5, -(a7)
03545e: 2f06             move.l d6, -(a7)
035460: 4ebab4aa         jsr $3090c(pc)
035464: 4fef000c         lea.l $c(a7), a7
035468: 72ff             moveq #$ff, d1
03546a: b280             cmp.l d0, d1
03546c: 6674             bne.b $354e2
03546e: 6062             bra.b $354d2
