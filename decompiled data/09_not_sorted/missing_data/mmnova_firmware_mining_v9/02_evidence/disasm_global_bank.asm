; GLOBAL: загрузка слота: длина 0x108, банк $7162A0 + слот*264 (lsl#8+lsl#3), RAM-копия $29D0F0; $71629C = текущий GLOBAL-слот.
; OS 0x40A74..0x40ABE (section_0_MAIN_OS.bin)

040a74: 222f0004       move.l $4(a7), d1
040a78: 70ff           moveq #$ff, d0
040a7a: b081           cmp.l d1, d0
040a7c: 6708           beq.b $40a86
040a7e: 2001           move.l d1, d0
040a80: e188           lsl.l #$8, d0
040a82: e789           lsl.l #$3, d1
040a84: 600c           bra.b $40a92
040a86: 20390071629c   move.l $71629c.l, d0
040a8c: 2200           move.l d0, d1
040a8e: e189           lsl.l #$8, d1
040a90: e788           lsl.l #$3, d0
040a92: d081           add.l d1, d0
040a94: 48780108       pea.l $108.w
040a98: 0680007162a0   addi.l #$7162a0, d0
040a9e: 2f00           move.l d0, -(a7)
040aa0: 48790029d0f0   pea.l $29d0f0.l
040aa6: 4eb90022b532   jsr $22b532.l
040aac: 4fef000c       lea.l $c(a7), a7
040ab0: 7001           moveq #$1, d0
040ab2: 23c00029c098   move.l d0, $29c098.l
040ab8: 4e75           rts 
040aba: 4feffff4       lea.l -$c(a7), a7
