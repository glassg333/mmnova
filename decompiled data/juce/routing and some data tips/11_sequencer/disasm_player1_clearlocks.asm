; Аналогичная распаковка для 'CLEAR/UNDO NOTE LOCKS' (строка $251FB8, вызов 0x39C2):
; 0x34F9A..0x35188; маски ячеек +0x274, значения slot+0x557+d7*64+a5, d7++ на ячейку.
; $30F7E (0x30F7E) = проверка 'ячейка пустеет' при снятии лока: после очистки значения
;   сканирует 64 Б строки; все FF -> jsr $30DA8 (удаление ячейки).
; $2159CC (0x159CC) = запись байта в плоский буфер $25799C + X*8 + Y (FF = нет лока).
; $215A40 (0x15A40) = перестановка бит-масок $26B538 по найденным строкам.

; ---- player1 (0x34F9A..0x35090, section_0 offset) ----
034f9a: 4fefffd8         lea.l -$28(a7), a7
034f9e: 48d73cfc         movem.l d2-d7/a2-a5, (a7)
034fa2: 2a6f002c         movea.l $2c(a7), a5
034fa6: 99cc             suba.l a4, a4
034fa8: 4287             clr.l d7
034faa: 601a             bra.b $34fc6
034fac: 0102             btst.l d0, d2
034fae: 6702             beq.b $34fb2
034fb0: 5287             addq.l #$1, d7
034fb2: 5280             addq.l #$1, d0
034fb4: 7608             moveq #$8, d3
034fb6: b680             cmp.l d0, d3
034fb8: 66f2             bne.b $34fac
034fba: 5281             addq.l #$1, d1
034fbc: 5288             addq.l #$1, a0
034fbe: b681             cmp.l d1, d3
034fc0: 66000162         bne.w $35124
034fc4: 528c             addq.l #$1, a4
034fc6: 4eb900224800     jsr $224800.l
034fcc: b08c             cmp.l a4, d0
034fce: 6f00015e         ble.w $3512e
034fd2: 243900716ae0     move.l $716ae0.l, d2
034fd8: 2202             move.l d2, d1
034fda: e989             lsl.l #$4, d1
034fdc: 2002             move.l d2, d0
034fde: ed88             lsl.l #$6, d0
034fe0: 9081             sub.l d1, d0
034fe2: 2200             move.l d0, d1
034fe4: e989             lsl.l #$4, d1
034fe6: d081             add.l d1, d0
034fe8: 9082             sub.l d2, d0
034fea: d08c             add.l a4, d0
034fec: e788             lsl.l #$3, d0
034fee: 2040             movea.l d0, a0
034ff0: d1fc00716d58     adda.l #$716d58, a0
034ff6: 4281             clr.l d1
034ff8: 6000012a         bra.w $35124
034ffc: 263900716ae0     move.l $716ae0.l, d3
035002: 240c             move.l a4, d2
035004: e78a             lsl.l #$3, d2
035006: 2203             move.l d3, d1
035008: e989             lsl.l #$4, d1
03500a: 2003             move.l d3, d0
03500c: ed88             lsl.l #$6, d0
03500e: 9081             sub.l d1, d0
035010: 2200             move.l d0, d1
035012: e989             lsl.l #$4, d1
035014: d280             add.l d0, d1
035016: 9283             sub.l d3, d1
035018: e789             lsl.l #$3, d1
03501a: d481             add.l d1, d2
03501c: d485             add.l d5, d2
03501e: 41f900716d58     lea.l $716d58.l, a0
035024: 10302800         move.b (a0, d2.l), d0
035028: 49c0             extb.l d0
03502a: 0900             btst.l d4, d0
03502c: 674c             beq.b $3507a
03502e: 2007             move.l d7, d0
035030: ed88             lsl.l #$6, d0
035032: d081             add.l d1, d0
035034: d08d             add.l a5, d0
035036: 41f90071703a     lea.l $71703a.l, a0
03503c: 10300801         move.b $1(a0, d0.l), d0
035040: 49c0             extb.l d0
035042: 1480             move.b d0, (a2)
035044: 72ff             moveq #$ff, d1
035046: b280             cmp.l d0, d1
035048: 672c             beq.b $35076
03504a: 2f0d             move.l a5, -(a7)
03504c: 2f04             move.l d4, -(a7)
03504e: 2f05             move.l d5, -(a7)
035050: 2f0c             move.l a4, -(a7)
035052: 2f03             move.l d3, -(a7)
035054: 4ebabf28         jsr $30f7e(pc)
035058: 4fef0014         lea.l $14(a7), a7
03505c: 4a80             tst.l d0
03505e: 6702             beq.b $35062
035060: 5287             addq.l #$1, d7
035062: 4878ffff         pea.l $ffff.w
035066: 2f04             move.l d4, -(a7)
035068: 2f05             move.l d5, -(a7)
03506a: 4eb9002159cc     jsr $2159cc.l
035070: 4fef000c         lea.l $c(a7), a7
035074: 6008             bra.b $3507e
035076: 5287             addq.l #$1, d7
035078: 6004             bra.b $3507e
03507a: 16bcffff         move.b #$ff, (a3)
03507e: 5284             addq.l #$1, d4
035080: 528b             addq.l #$1, a3
035082: 528a             addq.l #$1, a2
035084: 7608             moveq #$8, d3
035086: b684             cmp.l d4, d3
035088: 6600ff72         bne.w $34ffc
03508c: 5285             addq.l #$1, d5
03508e: 5086             addq.l #$8, d6

; ---- f30f7e (0x30F7E..0x31040, section_0 offset) ----
030f7e: 4fefffe4         lea.l -$1c(a7), a7
030f82: 48d704fc         movem.l d2-d7/a2, (a7)
030f86: 262f0020         move.l $20(a7), d3
030f8a: 2c2f0024         move.l $24(a7), d6
030f8e: 2a2f0028         move.l $28(a7), d5
030f92: 282f002c         move.l $2c(a7), d4
030f96: 2203             move.l d3, d1
030f98: e989             lsl.l #$4, d1
030f9a: 2003             move.l d3, d0
030f9c: ed88             lsl.l #$6, d0
030f9e: 9081             sub.l d1, d0
030fa0: 2200             move.l d0, d1
030fa2: e989             lsl.l #$4, d1
030fa4: d081             add.l d1, d0
030fa6: 9083             sub.l d3, d0
030fa8: e788             lsl.l #$3, d0
030faa: 2440             movea.l d0, a2
030fac: d5fc00716ae4     adda.l #$716ae4, a2
030fb2: 2006             move.l d6, d0
030fb4: e788             lsl.l #$3, d0
030fb6: d08a             add.l a2, d0
030fb8: 2040             movea.l d0, a0
030fba: d1c5             adda.l d5, a0
030fbc: 10280274         move.b $274(a0), d0
030fc0: 49c0             extb.l d0
030fc2: 0900             btst.l d4, d0
030fc4: 6754             beq.b $3101a
030fc6: 2f04             move.l d4, -(a7)
030fc8: 2f05             move.l d5, -(a7)
030fca: 2f06             move.l d6, -(a7)
030fcc: 2f03             move.l d3, -(a7)
030fce: 4ebafcf8         jsr $30cc8(pc)
030fd2: 2400             move.l d0, d2
030fd4: 2200             move.l d0, d1
030fd6: ed89             lsl.l #$6, d1
030fd8: 2001             move.l d1, d0
030fda: d08a             add.l a2, d0
030fdc: d0af0040         add.l $40(a7), d0
030fe0: 2040             movea.l d0, a0
030fe2: 41e80556         lea.l $556(a0), a0
030fe6: 4fef0010         lea.l $10(a7), a7
030fea: 4a280001         tst.b $1(a0)
030fee: 6d2a             blt.b $3101a
030ff0: 50c0             st.b d0
030ff2: 11400001         move.b d0, $1(a0)
030ff6: 2001             move.l d1, d0
030ff8: 068000000556     addi.l #$556, d0
030ffe: 41f20801         lea.l $1(a2, d0.l), a0
031002: 4281             clr.l d1
031004: 1010             move.b (a0), d0
031006: 49c0             extb.l d0
031008: 7eff             moveq #$ff, d7
03100a: be80             cmp.l d0, d7
03100c: 6d10             blt.b $3101e
03100e: 5281             addq.l #$1, d1
031010: 5288             addq.l #$1, a0
031012: 7040             moveq #$40, d0
031014: b081             cmp.l d1, d0
031016: 670a             beq.b $31022
031018: 60ea             bra.b $31004
03101a: 4280             clr.l d0
03101c: 6018             bra.b $31036
03101e: 7001             moveq #$1, d0
031020: 6014             bra.b $31036
031022: 2f02             move.l d2, -(a7)
031024: 2f04             move.l d4, -(a7)
031026: 2f05             move.l d5, -(a7)
031028: 2f06             move.l d6, -(a7)
03102a: 2f03             move.l d3, -(a7)
03102c: 4ebafd7a         jsr $30da8(pc)
031030: 4280             clr.l d0
031032: 4fef0014         lea.l $14(a7), a7
031036: 4cd704fc         movem.l (a7), d2-d7/a2
03103a: 4fef001c         lea.l $1c(a7), a7
03103e: 4e75             rts 

; ---- helper_2159cc (0x159CC..0x159E8, section_0 offset) ----
0159cc: 202f0004         move.l $4(a7), d0
0159d0: e788             lsl.l #$3, d0
0159d2: 2240             movea.l d0, a1
0159d4: d3fc0025799c     adda.l #$25799c, a1
0159da: 206f0008         movea.l $8(a7), a0
0159de: 102f000f         move.b $f(a7), d0
0159e2: 11809800         move.b d0, (a0, a1.l)
0159e6: 4e75             rts 
