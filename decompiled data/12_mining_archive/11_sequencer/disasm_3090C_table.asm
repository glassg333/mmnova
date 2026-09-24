; $3090C(k, track, step) — есть ли k-я блокировка на ячейке:
;   k=0: FF/$FE по наличию ноты notes[track][step] (зона +0x2A4).
;   k>=1: скан таблицы локов слота: 16-бит записи param<<9|track<<6|step,
;   база slot+0x17F8 ($7182DC), счётчик = байт slot+0x554 ($717038).
;   Возвращает индекс k-й (arg-1, 0-based) записи (track,step) или FF.
; $3087E (0x3087E) — проверка ячейки движком: наличие ноты (sne/neg) + скан таблицы.

; ---- f3090c (0x3090C..0x309C0, section_0 offset) ----
03090c: 4fefffec         lea.l -$14(a7), a7
030910: 48d7007c         movem.l d2-d6, (a7)
030914: 262f0018         move.l $18(a7), d3
030918: 2c2f001c         move.l $1c(a7), d6
03091c: 202f0020         move.l $20(a7), d0
030920: 2439002bda2e     move.l $2bda2e.l, d2
030926: 4a80             tst.l d0
030928: 6634             bne.b $3095e
03092a: ed8b             lsl.l #$6, d3
03092c: 2202             move.l d2, d1
03092e: e989             lsl.l #$4, d1
030930: 2002             move.l d2, d0
030932: ed88             lsl.l #$6, d0
030934: 9081             sub.l d1, d0
030936: 2200             move.l d0, d1
030938: e989             lsl.l #$4, d1
03093a: d081             add.l d1, d0
03093c: 9082             sub.l d2, d0
03093e: e788             lsl.l #$3, d0
030940: d083             add.l d3, d0
030942: d086             add.l d6, d0
030944: 41f900716d88     lea.l $716d88.l, a0
03094a: 10300800         move.b (a0, d0.l), d0
03094e: 49c0             extb.l d0
030950: 72ff             moveq #$ff, d1
030952: b280             cmp.l d0, d1
030954: 6764             beq.b $309ba
030956: 70fe             moveq #$fe, d0
030958: 6062             bra.b $309bc
03095a: 2002             move.l d2, d0
03095c: 605e             bra.b $309bc
03095e: 2a00             move.l d0, d5
030960: 5385             subq.l #$1, d5
030962: 2202             move.l d2, d1
030964: e989             lsl.l #$4, d1
030966: 2002             move.l d2, d0
030968: ed88             lsl.l #$6, d0
03096a: 9081             sub.l d1, d0
03096c: 2200             move.l d0, d1
03096e: e989             lsl.l #$4, d1
030970: d081             add.l d1, d0
030972: 9082             sub.l d2, d0
030974: e788             lsl.l #$3, d0
030976: 41f900717038     lea.l $717038.l, a0
03097c: 4284             clr.l d4
03097e: 18300800         move.b (a0, d0.l), d4
030982: 2040             movea.l d0, a0
030984: d1fc007182dc     adda.l #$7182dc, a0
03098a: 4282             clr.l d2
03098c: 93c9             suba.l a1, a1
03098e: 6026             bra.b $309b6
030990: 4281             clr.l d1
030992: 3210             move.w (a0), d1
030994: 703f             moveq #$3f, d0
030996: c081             and.l d1, d0
030998: bc80             cmp.l d0, d6
03099a: 6614             bne.b $309b0
03099c: 0281000001ff     andi.l #$1ff, d1
0309a2: ec81             asr.l #$6, d1
0309a4: b681             cmp.l d1, d3
0309a6: 6608             bne.b $309b0
0309a8: ba89             cmp.l a1, d5
0309aa: 67ae             beq.b $3095a
0309ac: 5289             addq.l #$1, a1
0309ae: 6002             bra.b $309b2
0309b0: 93c9             suba.l a1, a1
0309b2: 5282             addq.l #$1, d2
0309b4: 5488             addq.l #$2, a0
0309b6: b882             cmp.l d2, d4
0309b8: 6ed6             bgt.b $30990
0309ba: 70ff             moveq #$ff, d0
0309bc: 4cd7007c         movem.l (a7), d2-d6

; ---- f3087e (0x3087E..0x3090C, section_0 offset) ----
03087e: 4feffff0         lea.l -$10(a7), a7
030882: 48d7003c         movem.l d2-d5, (a7)
030886: 2a2f0014         move.l $14(a7), d5
03088a: 282f0018         move.l $18(a7), d4
03088e: 2439002bda2e     move.l $2bda2e.l, d2
030894: 2605             move.l d5, d3
030896: ed8b             lsl.l #$6, d3
030898: 2202             move.l d2, d1
03089a: e989             lsl.l #$4, d1
03089c: 2002             move.l d2, d0
03089e: ed88             lsl.l #$6, d0
0308a0: 9081             sub.l d1, d0
0308a2: 2200             move.l d0, d1
0308a4: e989             lsl.l #$4, d1
0308a6: d081             add.l d1, d0
0308a8: 9082             sub.l d2, d0
0308aa: e788             lsl.l #$3, d0
0308ac: d680             add.l d0, d3
0308ae: d684             add.l d4, d3
0308b0: 41f900716d88     lea.l $716d88.l, a0
0308b6: 12303800         move.b (a0, d3.l), d1
0308ba: 49c1             extb.l d1
0308bc: 74ff             moveq #$ff, d2
0308be: b481             cmp.l d1, d2
0308c0: 56c1             sne.b d1
0308c2: 49c1             extb.l d1
0308c4: 4481             neg.l d1
0308c6: 41f900717038     lea.l $717038.l, a0
0308cc: 4283             clr.l d3
0308ce: 16300800         move.b (a0, d0.l), d3
0308d2: 2040             movea.l d0, a0
0308d4: d1fc007182dc     adda.l #$7182dc, a0
0308da: 93c9             suba.l a1, a1
0308dc: 601e             bra.b $308fc
0308de: 4282             clr.l d2
0308e0: 3410             move.w (a0), d2
0308e2: 703f             moveq #$3f, d0
0308e4: c082             and.l d2, d0
0308e6: b880             cmp.l d0, d4
0308e8: 660e             bne.b $308f8
0308ea: 0282000001ff     andi.l #$1ff, d2
0308f0: ec82             asr.l #$6, d2
0308f2: ba82             cmp.l d2, d5
0308f4: 6602             bne.b $308f8
0308f6: 5281             addq.l #$1, d1
0308f8: 5289             addq.l #$1, a1
0308fa: 5488             addq.l #$2, a0
0308fc: b689             cmp.l a1, d3
0308fe: 6ede             bgt.b $308de
030900: 2001             move.l d1, d0
030902: 4cd7003c         movem.l (a7), d2-d5
030906: 4fef0010         lea.l $10(a7), a7
03090a: 4e75             rts 
