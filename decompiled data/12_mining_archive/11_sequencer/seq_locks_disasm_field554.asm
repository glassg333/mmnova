; Поле +0x554 (один слот): word +0x552 = sext(+0x554); +0x554 <- +0x555. Аналог цикла 128 слотов (v8 0x3D5F2).
; OS 0x2C384..0x2C3C8 (section_0_MAIN_OS.bin)

02c384: 2f02           move.l d2, -(a7)
02c386: 242f0008       move.l $8(a7), d2
02c38a: 2202           move.l d2, d1
02c38c: e989           lsl.l #$4, d1
02c38e: 2002           move.l d2, d0
02c390: ed88           lsl.l #$6, d0
02c392: 9081           sub.l d1, d0
02c394: 2200           move.l d0, d1
02c396: e989           lsl.l #$4, d1
02c398: d081           add.l d1, d0
02c39a: 9082           sub.l d2, d0
02c39c: e788           lsl.l #$3, d0
02c39e: 41f900717039   lea.l $717039.l, a0
02c3a4: 12300800       move.b (a0, d0.l), d1
02c3a8: 49c1           extb.l d1
02c3aa: 2240           movea.l d0, a1
02c3ac: d3fc00717036   adda.l #$717036, a1
02c3b2: 41f900717038   lea.l $717038.l, a0
02c3b8: 14300800       move.b (a0, d0.l), d2
02c3bc: 4882           ext.w d2
02c3be: 3282           move.w d2, (a1)
02c3c0: 11810800       move.b d1, (a0, d0.l)
02c3c4: 241f           move.l (a7)+, d2
02c3c6: 4e75           rts 
