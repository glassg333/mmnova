; Редактор локов: скан таблицы слота (0x2C400): база $716AE0+slot*0x1978.
; Первое слово +0x17FC ($7182DC) = запись 0; шаг = w&0x3F, дорожка = (w asr 6)&7,
; параметр = w>>9 (биты 9..15); поиск (track,step) по счётчику из регистров.

; ---- scan (0x2C400..0x2C500, section_0 offset) ----
02c400: 00042f16         ori.b #$16, d4
02c404: 4eb90024f218     jsr $24f218.l
02c40a: 4fef000c         lea.l $c(a7), a7
02c40e: 7401             moveq #$1, d2
02c410: c282             and.l d2, d1
02c412: 67000160         beq.w $2c574
02c416: 2204             move.l d4, d1
02c418: e989             lsl.l #$4, d1
02c41a: 2004             move.l d4, d0
02c41c: ed88             lsl.l #$6, d0
02c41e: 9081             sub.l d1, d0
02c420: 2200             move.l d0, d1
02c422: e989             lsl.l #$4, d1
02c424: d081             add.l d1, d0
02c426: 9084             sub.l d4, d0
02c428: 41f30c00         lea.l (a3, d0.l * 4), a0
02c42c: 2008             move.l a0, d0
02c42e: d080             add.l d0, d0
02c430: 2040             movea.l d0, a0
02c432: d1fc00716ae0     adda.l #$716ae0, a0
02c438: 4281             clr.l d1
02c43a: 322817fc         move.w $17fc(a0), d1
02c43e: 143c003f         move.b #$3f, d2
02c442: c481             and.l d1, d2
02c444: ec81             asr.l #$6, d1
02c446: 7607             moveq #$7, d3
02c448: c283             and.l d3, d1
02c44a: 2040             movea.l d0, a0
02c44c: d1fc007182de     adda.l #$7182de, a0
02c452: 6012             bra.b $2c466
02c454: 528b             addq.l #$1, a3
02c456: 4280             clr.l d0
02c458: 3018             move.w (a0)+, d0
02c45a: 743f             moveq #$3f, d2
02c45c: c480             and.l d0, d2
02c45e: 2200             move.l d0, d1
02c460: ec81             asr.l #$6, d1
02c462: 7007             moveq #$7, d0
02c464: c280             and.l d0, d1
02c466: b486             cmp.l d6, d2
02c468: 6604             bne.b $2c46e
02c46a: ba81             cmp.l d1, d5
02c46c: 6708             beq.b $2c476
02c46e: be8b             cmp.l a3, d7
02c470: 6ee2             bgt.b $2c454
02c472: 6000011c         bra.w $2c590
02c476: be8b             cmp.l a3, d7
02c478: 6f000116         ble.w $2c590
02c47c: 43eb0001         lea.l $1(a3), a1
02c480: 2204             move.l d4, d1
02c482: e989             lsl.l #$4, d1
02c484: 2004             move.l d4, d0
02c486: ed88             lsl.l #$6, d0
02c488: 9081             sub.l d1, d0
02c48a: 2200             move.l d0, d1
02c48c: e989             lsl.l #$4, d1
02c48e: d081             add.l d1, d0
02c490: 9084             sub.l d4, d0
02c492: e588             lsl.l #$2, d0
02c494: 2200             move.l d0, d1
02c496: d289             add.l a1, d1
02c498: d281             add.l d1, d1
02c49a: 2041             movea.l d1, a0
02c49c: d1fc00716ae0     adda.l #$716ae0, a0
02c4a2: 4281             clr.l d1
02c4a4: 322817fc         move.w $17fc(a0), d1
02c4a8: 743f             moveq #$3f, d2
02c4aa: c481             and.l d1, d2
02c4ac: ec81             asr.l #$6, d1
02c4ae: 7607             moveq #$7, d3
02c4b0: c283             and.l d3, d1
02c4b2: d08b             add.l a3, d0
02c4b4: d080             add.l d0, d0
02c4b6: 2040             movea.l d0, a0
02c4b8: d1fc007182e0     adda.l #$7182e0, a0
02c4be: 6012             bra.b $2c4d2
02c4c0: 5289             addq.l #$1, a1
02c4c2: 4280             clr.l d0
02c4c4: 3018             move.w (a0)+, d0
02c4c6: 743f             moveq #$3f, d2
02c4c8: c480             and.l d0, d2
02c4ca: 2200             move.l d0, d1
02c4cc: ec81             asr.l #$6, d1
02c4ce: 7007             moveq #$7, d0
02c4d0: c280             and.l d0, d1
02c4d2: b486             cmp.l d6, d2
02c4d4: 6608             bne.b $2c4de
02c4d6: ba81             cmp.l d1, d5
02c4d8: 6604             bne.b $2c4de
02c4da: be89             cmp.l a1, d7
02c4dc: 6ee2             bgt.b $2c4c0
02c4de: be89             cmp.l a1, d7
02c4e0: 6d0000ae         blt.w $2c590
02c4e4: 45e9ffff         lea.l -$1(a1), a2
02c4e8: 1614             move.b (a4), d3
02c4ea: 49c3             extb.l d3
02c4ec: 2004             move.l d4, d0
02c4ee: e988             lsl.l #$4, d0
02c4f0: 2204             move.l d4, d1
02c4f2: ed89             lsl.l #$6, d1
02c4f4: 9280             sub.l d0, d1
02c4f6: 2001             move.l d1, d0
02c4f8: e988             lsl.l #$4, d0
02c4fa: d280             add.l d0, d1
02c4fc: 9284             sub.l d4, d1
02c4fe: e589             lsl.l #$2, d1
