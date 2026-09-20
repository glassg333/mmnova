; GLOBAL: копия в $29D1F8 (вторая рабочая копия), state $29C098/$29C094/$29C0A0/$29D300.
; OS 0x40ABA..0x40B32 (section_0_MAIN_OS.bin)

040aba: 4feffff4       lea.l -$c(a7), a7
040abe: 48d7040c       movem.l d2-d3/a2, (a7)
040ac2: 262f0010       move.l $10(a7), d3
040ac6: 4ab90029c098   tst.l $29c098.l
040acc: 674e           beq.b $40b1c
040ace: 70ff           moveq #$ff, d0
040ad0: b083           cmp.l d3, d0
040ad2: 674c           beq.b $40b20
040ad4: 2403           move.l d3, d2
040ad6: e78a           lsl.l #$3, d2
040ad8: 2003           move.l d3, d0
040ada: e188           lsl.l #$8, d0
040adc: d480           add.l d0, d2
040ade: 0682007162a0   addi.l #$7162a0, d2
040ae4: 48780108       pea.l $108.w
040ae8: 2f02           move.l d2, -(a7)
040aea: 48790029d1f8   pea.l $29d1f8.l
040af0: 45f90022b532   lea.l $22b532.l, a2
040af6: 4e92           jsr (a2)
040af8: 23c30029d300   move.l d3, $29d300.l
040afe: 7001           moveq #$1, d0
040b00: 23c00029c094   move.l d0, $29c094.l
040b06: 48780108       pea.l $108.w
040b0a: 48790029d0f0   pea.l $29d0f0.l
040b10: 2f02           move.l d2, -(a7)
040b12: 4e92           jsr (a2)
040b14: 7001           moveq #$1, d0
040b16: 4fef0018       lea.l $18(a7), a7
040b1a: 600c           bra.b $40b28
040b1c: 4280           clr.l d0
040b1e: 6008           bra.b $40b28
040b20: 26390071629c   move.l $71629c.l, d3
040b26: 60ac           bra.b $40ad4
040b28: 4cd7040c       movem.l (a7), d2-d3/a2
040b2c: 4fef000c       lea.l $c(a7), a7
040b30: 4e75           rts 
