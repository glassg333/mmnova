; Плеер/загрузчик локов, ч.1: копия слота в PAT3 ($29817E), подсчёт триггеров, копирование строк +0x557 по битам масок (+0x274 + a1*8), a1=группа 8Б; выход при a4<=a1. Внутри: источник = банк+0x557+d7*64, приёмник = PAT3+0x557+d6*64, d6/d7++ на установленном бите.
; OS 0x34DB6..0x34EA2 (section_0_MAIN_OS.bin)

034db6: 4fefffdc       lea.l -$24(a7), a7
034dba: 48d71cfc       movem.l d2-d7/a2-a4, (a7)
034dbe: 286f0028       movea.l $28(a7), a4
034dc2: 263900716ae0   move.l $716ae0.l, d3
034dc8: 48780001       pea.l $1.w
034dcc: 42a7           clr.l -(a7)
034dce: 48790029817e   pea.l $29817e.l
034dd4: 2f0c           move.l a4, -(a7)
034dd6: 2003           move.l d3, d0
034dd8: e988           lsl.l #$4, d0
034dda: 2203           move.l d3, d1
034ddc: ed89           lsl.l #$6, d1
034dde: 9280           sub.l d0, d1
034de0: 2001           move.l d1, d0
034de2: e988           lsl.l #$4, d0
034de4: d280           add.l d0, d1
034de6: 9283           sub.l d3, d1
034de8: e789           lsl.l #$3, d1
034dea: 068100716ae4   addi.l #$716ae4, d1
034df0: 2f01           move.l d1, -(a7)
034df2: 4ebaec90       jsr $33a84(pc)
034df6: 42b90028b11c   clr.l $28b11c.l
034dfc: 93c9           suba.l a1, a1
034dfe: 4287           clr.l d7
034e00: 4fef0014       lea.l $14(a7), a7
034e04: 601a           bra.b $34e20
034e06: 0102           btst.l d0, d2
034e08: 6702           beq.b $34e0c
034e0a: 5287           addq.l #$1, d7
034e0c: 5280           addq.l #$1, d0
034e0e: 7808           moveq #$8, d4
034e10: b880           cmp.l d0, d4
034e12: 66f2           bne.b $34e06
034e14: 5281           addq.l #$1, d1
034e16: 5288           addq.l #$1, a0
034e18: b881           cmp.l d1, d4
034e1a: 6600013c       bne.w $34f58
034e1e: 5289           addq.l #$1, a1
034e20: 2203           move.l d3, d1
034e22: e989           lsl.l #$4, d1
034e24: 2003           move.l d3, d0
034e26: ed88           lsl.l #$6, d0
034e28: b9c9           cmpa.l a1, a4
034e2a: 6f000136       ble.w $34f62
034e2e: 9081           sub.l d1, d0
034e30: 2200           move.l d0, d1
034e32: e989           lsl.l #$4, d1
034e34: d081           add.l d1, d0
034e36: 9083           sub.l d3, d0
034e38: d089           add.l a1, d0
034e3a: e788           lsl.l #$3, d0
034e3c: 2040           movea.l d0, a0
034e3e: d1fc00716d58   adda.l #$716d58, a0
034e44: 4281           clr.l d1
034e46: 60000110       bra.w $34f58
034e4a: 200b           move.l a3, d0
034e4c: 0900           btst.l d4, d0
034e4e: 673e           beq.b $34e8e
034e50: 2407           move.l d7, d2
034e52: ed8a           lsl.l #$6, d2
034e54: 2203           move.l d3, d1
034e56: e989           lsl.l #$4, d1
034e58: 2003           move.l d3, d0
034e5a: ed88           lsl.l #$6, d0
034e5c: 9081           sub.l d1, d0
034e5e: 2200           move.l d0, d1
034e60: e989           lsl.l #$4, d1
034e62: d081           add.l d1, d0
034e64: 9083           sub.l d3, d0
034e66: e788           lsl.l #$3, d0
034e68: d480           add.l d0, d2
034e6a: 2242           movea.l d2, a1
034e6c: d3fc0071703b   adda.l #$71703b, a1
034e72: 2006           move.l d6, d0
034e74: ed88           lsl.l #$6, d0
034e76: 2040           movea.l d0, a0
034e78: d1fc002986d5   adda.l #$2986d5, a0
034e7e: 4280           clr.l d0
034e80: 10d9           move.b (a1)+, (a0)+
034e82: 5280           addq.l #$1, d0
034e84: 7240           moveq #$40, d1
034e86: b280           cmp.l d0, d1
034e88: 66f6           bne.b $34e80
034e8a: 5286           addq.l #$1, d6
034e8c: 5287           addq.l #$1, d7
034e8e: 5284           addq.l #$1, d4
034e90: 7008           moveq #$8, d0
034e92: b084           cmp.l d4, d0
034e94: 66b4           bne.b $34e4a
034e96: 5285           addq.l #$1, d5
034e98: 528a           addq.l #$1, a2
034e9a: 7207           moveq #$7, d1
034e9c: b285           cmp.l d5, d1
034e9e: 660000dc       bne.w $34f7c
