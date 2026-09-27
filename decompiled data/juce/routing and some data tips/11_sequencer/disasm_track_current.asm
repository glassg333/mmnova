; ТЕКУЩАЯ ДОРОЖКА (не 'число строк', как считали в v9!): $224800 = геттер.
; 0x24800: tst.l $26b570; beq; move.l $26bb94/$26bb98,d0; rts
; 0x24A84 (сеттер): если новое значение < 0, то += 6; divs.l #6 — МОДУЛЬ 6 ДОРОЖЕК.
; $26BB94/$26BB98 = селектор текущей дорожки (2 режима по флагу $26B570).

; ---- getter_24800 (0x24800..0x24820, section_0 offset) ----
024800: 4ab90026b570     tst.l $26b570.l
024806: 6708             beq.b $24810
024808: 20390026bb94     move.l $26bb94.l, d0
02480e: 4e75             rts 
024810: 20390026bb98     move.l $26bb98.l, d0
024816: 4e75             rts 
024818: 4e71             nop 
02481a: 20390026bb98     move.l $26bb98.l, d0

; ---- setter_24a84 (0x24A84..0x24B50, section_0 offset) ----
024a84: 2f03             move.l d3, -(a7)
024a86: 2f02             move.l d2, -(a7)
024a88: 202f000c         move.l $c(a7), d0
024a8c: 20790026b570     movea.l $26b570.l, a0
024a92: 4a88             tst.l a0
024a94: 6708             beq.b $24a9e
024a96: 22390026bb94     move.l $26bb94.l, d1
024a9c: 6006             bra.b $24aa4
024a9e: 22390026bb98     move.l $26bb98.l, d1
024aa4: b280             cmp.l d0, d1
024aa6: 670c             beq.b $24ab4
024aa8: 42b90026b468     clr.l $26b468.l
024aae: 42b90026b46c     clr.l $26b46c.l
024ab4: 4a80             tst.l d0
024ab6: 6c02             bge.b $24aba
024ab8: 5c80             addq.l #$6, d0
024aba: 4a88             tst.l a0
024abc: 6728             beq.b $24ae6
024abe: 2200             move.l d0, d1
024ac0: 7406             moveq #$6, d2
024ac2: 4c421803         divs.l d2, d1
024ac6: 23c30026bb94     move.l d3, $26bb94.l
024acc: 2f03             move.l d3, -(a7)
024ace: 4eb900241d72     jsr $241d72.l
024ad4: 23c00026bd18     move.l d0, $26bd18.l
024ada: 588f             addq.l #$4, a7
024adc: 4ab90026b570     tst.l $26b570.l
024ae2: 6622             bne.b $24b06
024ae4: 6028             bra.b $24b0e
024ae6: 2200             move.l d0, d1
024ae8: 7406             moveq #$6, d2
024aea: 4c421803         divs.l d2, d1
024aee: 23c30026bb98     move.l d3, $26bb98.l
024af4: 23c300700004     move.l d3, $700004.l
024afa: 4eb900243740     jsr $243740.l
024b00: 4a80             tst.l d0
024b02: 6646             bne.b $24b4a
024b04: 6018             bra.b $24b1e
024b06: 20390026bb94     move.l $26bb94.l, d0
024b0c: 6006             bra.b $24b14
024b0e: 20390026bb98     move.l $26bb98.l, d0
024b14: eb88             lsl.l #$5, d0
024b16: 068000000020     addi.l #$20, d0
024b1c: 6026             bra.b $24b44
024b1e: 4eb900241722     jsr $241722.l
024b24: 4ab90026b570     tst.l $26b570.l
024b2a: 6708             beq.b $24b34
024b2c: 22390026bb94     move.l $26bb94.l, d1
024b32: 6006             bra.b $24b3a
024b34: 22390026bb98     move.l $26bb98.l, d1
024b3a: d081             add.l d1, d0
024b3c: 23c00026bd18     move.l d0, $26bd18.l
024b42: 7008             moveq #$8, d0
024b44: 23c000257c98     move.l d0, $257c98.l
024b4a: 241f             move.l (a7)+, d2
024b4c: 261f             move.l (a7)+, d3
024b4e: 4e75             rts 
