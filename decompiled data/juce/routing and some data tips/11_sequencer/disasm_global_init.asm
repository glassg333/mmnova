; GLOBAL: инициализация нового слота: поля +0..+0xC, таблица 12Б от +0x3C (32 итерации: 0x77/1/1/1/0x10/0/0 на фазах 0/1/0x20/0x40/0x60/0x80/0xA0), +0xFD=0, +0x104=0x1130, +0x12=9, +0x18..+0x1B=1/2/7/A.
; OS 0x40B32..0x40CA8 (section_0_MAIN_OS.bin)

040b32: 4fefffec       lea.l -$14(a7), a7
040b36: 48d7043c       movem.l d2-d5/a2, (a7)
040b3a: 262f0018       move.l $18(a7), d3
040b3e: 2a03           move.l d3, d5
040b40: e78d           lsl.l #$3, d5
040b42: 2803           move.l d3, d4
040b44: e18c           lsl.l #$8, d4
040b46: 2405           move.l d5, d2
040b48: d484           add.l d4, d2
040b4a: 2442           movea.l d2, a2
040b4c: d5fc007162a0   adda.l #$7162a0, a2
040b52: 48780108       pea.l $108.w
040b56: 2f0a           move.l a2, -(a7)
040b58: 48790029d1f8   pea.l $29d1f8.l
040b5e: 4eb90022b532   jsr $22b532.l
040b64: 23c30029d300   move.l d3, $29d300.l
040b6a: 7001           moveq #$1, d0
040b6c: 23c00029c094   move.l d0, $29c094.l
040b72: 14bc0008       move.b #$8, (a2)
040b76: 41f9007162a0   lea.l $7162a0.l, a0
040b7c: 4201           clr.b d1
040b7e: 11812801       move.b d1, $1(a0, d2.l)
040b82: 41f9007162a2   lea.l $7162a2.l, a0
040b88: 7006           moveq #$6, d0
040b8a: 11802800       move.b d0, (a0, d2.l)
040b8e: 11802801       move.b d0, $1(a0, d2.l)
040b92: 41f9007162a4   lea.l $7162a4.l, a0
040b98: 7207           moveq #$7, d1
040b9a: 11812800       move.b d1, (a0, d2.l)
040b9e: 4200           clr.b d0
040ba0: 11802801       move.b d0, $1(a0, d2.l)
040ba4: 41f9007162a6   lea.l $7162a6.l, a0
040baa: 11802800       move.b d0, (a0, d2.l)
040bae: 11802801       move.b d0, $1(a0, d2.l)
040bb2: 41f9007162a8   lea.l $7162a8.l, a0
040bb8: 11802800       move.b d0, (a0, d2.l)
040bbc: 7202           moveq #$2, d1
040bbe: 11812801       move.b d1, $1(a0, d2.l)
040bc2: 41f9007162aa   lea.l $7162aa.l, a0
040bc8: 7001           moveq #$1, d0
040bca: 11802800       move.b d0, (a0, d2.l)
040bce: 11802801       move.b d0, $1(a0, d2.l)
040bd2: 43f9007162ac   lea.l $7162ac.l, a1
040bd8: 4201           clr.b d1
040bda: 13812800       move.b d1, (a1, d2.l)
040bde: 45f90071639c   lea.l $71639c.l, a2
040be4: 15802801       move.b d0, $1(a2, d2.l)
040be8: 2042           movea.l d2, a0
040bea: d1fc007162dc   adda.l #$7162dc, a0
040bf0: 4281           clr.l d1
040bf2: 4fef000c       lea.l $c(a7), a7
040bf6: 746c           moveq #$6c, d2
040bf8: b481           cmp.l d1, d2
040bfa: 6c0000ac       bge.w $40ca8
040bfe: 10bc0077       move.b #$77, (a0)
040c02: 50c0           st.b d0
040c04: 11400020       move.b d0, $20(a0)
040c08: 11400040       move.b d0, $40(a0)
040c0c: 7410           moveq #$10, d2
040c0e: 11420060       move.b d2, $60(a0)
040c12: 4200           clr.b d0
040c14: 11400080       move.b d0, $80(a0)
040c18: 114000a0       move.b d0, $a0(a0)
040c1c: 06810000000c   addi.l #$c, d1
040c22: 5288           addq.l #$1, a0
040c24: 0c8100000180   cmpi.l #$180, d1
040c2a: 66ca           bne.b $40bf6
040c2c: 2005           move.l d5, d0
040c2e: d084           add.l d4, d0
040c30: 4201           clr.b d1
040c32: 13810801       move.b d1, $1(a1, d0.l)
040c36: 41f9007162ae   lea.l $7162ae.l, a0
040c3c: 11810800       move.b d1, (a0, d0.l)
040c40: 11810801       move.b d1, $1(a0, d0.l)
040c44: 41f9007162b0   lea.l $7162b0.l, a0
040c4a: 11810800       move.b d1, (a0, d0.l)
040c4e: 11810801       move.b d1, $1(a0, d0.l)
040c52: 2040           movea.l d0, a0
040c54: d1fc007163a4   adda.l #$7163a4, a0
040c5a: 20bc00001130   move.l #$1130, (a0)
040c60: 7401           moveq #$1, d2
040c62: 15820800       move.b d2, (a2, d0.l)
040c66: 2240           movea.l d0, a1
040c68: d3fc007162b2   adda.l #$7162b2, a1
040c6e: 2040           movea.l d0, a0
040c70: d1fc007162b8   adda.l #$7162b8, a0
040c76: 7209           moveq #$9, d1
040c78: 1281           move.b d1, (a1)
040c7a: 10bc0001       move.b #$1, (a0)
040c7e: 7002           moveq #$2, d0
040c80: 11400001       move.b d0, $1(a0)
040c84: 7407           moveq #$7, d2
040c86: 11420002       move.b d2, $2(a0)
040c8a: 700a           moveq #$a, d0
040c8c: 11400003       move.b d0, $3(a0)
040c90: 7401           moveq #$1, d2
040c92: 1342001e       move.b d2, $1e(a1)
040c96: 5289           addq.l #$1, a1
040c98: 5281           addq.l #$1, d1
040c9a: 5888           addq.l #$4, a0
040c9c: 4280           clr.l d0
040c9e: 1001           move.b d1, d0
040ca0: 740f           moveq #$f, d2
040ca2: b480           cmp.l d0, d2
040ca4: 66d2           bne.b $40c78
040ca6: 603a           bra.b $40ce2
