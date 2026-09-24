; Плеер, ч.2: таблица 64x256Б в $290E8E; jsr $3090C(шаг) -> инфа шага; запись байтов по (a3)+; str-цикл d4 0..0x40 c шагом $100(a2) (0x34F4A); $2BDA2E = текущий слот; $716AE0 = номер слота.
; OS 0x34EA2..0x34F9A (section_0_MAIN_OS.bin)

034ea2: 4284           clr.l d4
034ea4: 45f900290e8e   lea.l $290e8e.l, a2
034eaa: 42a7           clr.l -(a7)
034eac: 2f04           move.l d4, -(a7)
034eae: 2f0c           move.l a4, -(a7)
034eb0: 4ebaba5a       jsr $3090c(pc)
034eb4: 2040           movea.l d0, a0
034eb6: 2004           move.l d4, d0
034eb8: e188           lsl.l #$8, d0
034eba: 2640           movea.l d0, a3
034ebc: d7fc00290e8e   adda.l #$290e8e, a3
034ec2: 4285           clr.l d5
034ec4: 6072           bra.b $34f38
034ec6: 2639002bda2e   move.l $2bda2e.l, d3
034ecc: 70fe           moveq #$fe, d0
034ece: b088           cmp.l a0, d0
034ed0: 662a           bne.b $34efc
034ed2: 240c           move.l a4, d2
034ed4: ed8a           lsl.l #$6, d2
034ed6: 2203           move.l d3, d1
034ed8: e989           lsl.l #$4, d1
034eda: 2003           move.l d3, d0
034edc: ed88           lsl.l #$6, d0
034ede: 9081           sub.l d1, d0
034ee0: 2200           move.l d0, d1
034ee2: e989           lsl.l #$4, d1
034ee4: d081           add.l d1, d0
034ee6: 9083           sub.l d3, d0
034ee8: e788           lsl.l #$3, d0
034eea: d480           add.l d0, d2
034eec: d484           add.l d4, d2
034eee: 41f900716d88   lea.l $716d88.l, a0
034ef4: 10302800       move.b (a0, d2.l), d0
034ef8: 49c0           extb.l d0
034efa: 602c           bra.b $34f28
034efc: 2203           move.l d3, d1
034efe: e989           lsl.l #$4, d1
034f00: 2003           move.l d3, d0
034f02: ed88           lsl.l #$6, d0
034f04: 9081           sub.l d1, d0
034f06: 2200           move.l d0, d1
034f08: e989           lsl.l #$4, d1
034f0a: d081           add.l d1, d0
034f0c: 9083           sub.l d3, d0
034f0e: 41f00c00       lea.l (a0, d0.l * 4), a0
034f12: 2008           move.l a0, d0
034f14: d080           add.l d0, d0
034f16: 2040           movea.l d0, a0
034f18: d1fc00716ae0   adda.l #$716ae0, a0
034f1e: 4280           clr.l d0
034f20: 302817fc       move.w $17fc(a0), d0
034f24: 7209           moveq #$9, d1
034f26: e2a8           lsr.l d1, d0
034f28: 16c0           move.b d0, (a3)+
034f2a: 5285           addq.l #$1, d5
034f2c: 2f05           move.l d5, -(a7)
034f2e: 2f04           move.l d4, -(a7)
034f30: 2f0c           move.l a4, -(a7)
034f32: 4ebab9d8       jsr $3090c(pc)
034f36: 2040           movea.l d0, a0
034f38: 4fef000c       lea.l $c(a7), a7
034f3c: 70ff           moveq #$ff, d0
034f3e: b088           cmp.l a0, d0
034f40: 6684           bne.b $34ec6
034f42: 50c1           st.b d1
034f44: 15815800       move.b d1, (a2, d5.l)
034f48: 5284           addq.l #$1, d4
034f4a: 45ea0100       lea.l $100(a2), a2
034f4e: 7040           moveq #$40, d0
034f50: b084           cmp.l d4, d0
034f52: 6600ff56       bne.w $34eaa
034f56: 6030           bra.b $34f88
034f58: 1410           move.b (a0), d2
034f5a: 49c2           extb.l d2
034f5c: 4280           clr.l d0
034f5e: 6000fea6       bra.w $34e06
034f62: 9081           sub.l d1, d0
034f64: 2200           move.l d0, d1
034f66: e989           lsl.l #$4, d1
034f68: d081           add.l d1, d0
034f6a: 9083           sub.l d3, d0
034f6c: d08c           add.l a4, d0
034f6e: e788           lsl.l #$3, d0
034f70: 2440           movea.l d0, a2
034f72: d5fc00716d58   adda.l #$716d58, a2
034f78: 4286           clr.l d6
034f7a: 4285           clr.l d5
034f7c: 1812           move.b (a2), d4
034f7e: 49c4           extb.l d4
034f80: 2644           movea.l d4, a3
034f82: 4284           clr.l d4
034f84: 6000fec4       bra.w $34e4a
034f88: 7201           moveq #$1, d1
034f8a: 23c10028b120   move.l d1, $28b120.l
034f90: 4cd71cfc       movem.l (a7), d2-d7/a2-a4
034f94: 4fef0024       lea.l $24(a7), a7
034f98: 4e75           rts 
