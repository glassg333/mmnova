; Очистка паттерна: FF-заполнение 62 строк локов (+0x557..+0x14D6, 0x2D2F2), очистка нот 6x64Б (+0x2A4, база $716D88), обнуление +0x556 (число строк, 0x2D2EA), слова +0x552 ($717036) и байта +0x554 ($717038).
; OS 0x2D2C0..0x2D390 (section_0_MAIN_OS.bin)

02d2c0: 66f4           bne.b $2d2b6
02d2c2: 5284           addq.l #$1, d4
02d2c4: 5288           addq.l #$1, a0
02d2c6: 7006           moveq #$6, d0
02d2c8: b084           cmp.l d4, d0
02d2ca: 6600ff46       bne.w $2d212
02d2ce: 2203           move.l d3, d1
02d2d0: e989           lsl.l #$4, d1
02d2d2: 2003           move.l d3, d0
02d2d4: ed88           lsl.l #$6, d0
02d2d6: 9081           sub.l d1, d0
02d2d8: 2200           move.l d0, d1
02d2da: e989           lsl.l #$4, d1
02d2dc: d081           add.l d1, d0
02d2de: 9083           sub.l d3, d0
02d2e0: e788           lsl.l #$3, d0
02d2e2: 41f90071703a   lea.l $71703a.l, a0
02d2e8: 4201           clr.b d1
02d2ea: 11810800       move.b d1, (a0, d0.l)
02d2ee: 4284           clr.l d4
02d2f0: 603e           bra.b $2d330
02d2f2: 10fcffff       move.b #$ff, (a0)+
02d2f6: 5280           addq.l #$1, d0
02d2f8: 7240           moveq #$40, d1
02d2fa: b280           cmp.l d0, d1
02d2fc: 66f4           bne.b $2d2f2
02d2fe: 5284           addq.l #$1, d4
02d300: 703e           moveq #$3e, d0
02d302: b084           cmp.l d4, d0
02d304: 662a           bne.b $2d330
02d306: 604e           bra.b $2d356
02d308: 2404           move.l d4, d2
02d30a: ed8a           lsl.l #$6, d2
02d30c: 2203           move.l d3, d1
02d30e: e989           lsl.l #$4, d1
02d310: 2003           move.l d3, d0
02d312: ed88           lsl.l #$6, d0
02d314: 9081           sub.l d1, d0
02d316: 2200           move.l d0, d1
02d318: e989           lsl.l #$4, d1
02d31a: d081           add.l d1, d0
02d31c: 9083           sub.l d3, d0
02d31e: e788           lsl.l #$3, d0
02d320: d480           add.l d0, d2
02d322: 2042           movea.l d2, a0
02d324: d1fc00716d88   adda.l #$716d88, a0
02d32a: 4280           clr.l d0
02d32c: 6000fe90       bra.w $2d1be
02d330: 2404           move.l d4, d2
02d332: ed8a           lsl.l #$6, d2
02d334: 2203           move.l d3, d1
02d336: e989           lsl.l #$4, d1
02d338: 2003           move.l d3, d0
02d33a: ed88           lsl.l #$6, d0
02d33c: 9081           sub.l d1, d0
02d33e: 2200           move.l d0, d1
02d340: e989           lsl.l #$4, d1
02d342: d081           add.l d1, d0
02d344: 9083           sub.l d3, d0
02d346: e788           lsl.l #$3, d0
02d348: d480           add.l d0, d2
02d34a: 2042           movea.l d2, a0
02d34c: d1fc0071703b   adda.l #$71703b, a0
02d352: 4280           clr.l d0
02d354: 609c           bra.b $2d2f2
02d356: 2203           move.l d3, d1
02d358: e989           lsl.l #$4, d1
02d35a: 2003           move.l d3, d0
02d35c: ed88           lsl.l #$6, d0
02d35e: 9081           sub.l d1, d0
02d360: 2200           move.l d0, d1
02d362: e989           lsl.l #$4, d1
02d364: d081           add.l d1, d0
02d366: 9083           sub.l d3, d0
02d368: e788           lsl.l #$3, d0
02d36a: 2040           movea.l d0, a0
02d36c: d1fc00717036   adda.l #$717036, a0
02d372: 4250           clr.w (a0)
02d374: 41f900717038   lea.l $717038.l, a0
02d37a: 4201           clr.b d1
02d37c: 11810800       move.b d1, (a0, d0.l)
02d380: 4cd7001c       movem.l (a7), d2-d4
02d384: 4fef000c       lea.l $c(a7), a7
02d388: 4e75           rts 
02d38a: 2f02           move.l d2, -(a7)
02d38c: 4282           clr.l d2
02d38e: 2f02           move.l d2, -(a7)
