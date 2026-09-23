0x43800: b088             cmp.l a0, d0
0x43802: 6684             bne.b $43788
0x43804: 6098             bra.b $4379e
0x43806: 7006             moveq #$6, d0
0x43808: 9088             sub.l a0, d0
0x4380a: 2a03             move.l d3, d5
0x4380c: e1ad             lsl.l d0, d5
0x4380e: 8485             or.l d5, d2
0x43810: 707f             moveq #$7f, d0
0x43812: c081             and.l d1, d0
0x43814: 17808800         move.b d0, (a3, a0.l)
0x43818: 5288             addq.l #$1, a0
0x4381a: b888             cmp.l a0, d4
0x4381c: 66c2             bne.b $437e0
0x4381e: 18c2             move.b d2, (a4)+
0x43820: 204c             movea.l a4, a0
0x43822: 4280             clr.l d0
0x43824: 10f30800         move.b (a3, d0.l), (a0)+
0x43828: 5280             addq.l #$1, d0
0x4382a: b880             cmp.l d0, d4
0x4382c: 66f6             bne.b $43824
0x4382e: 6008             bra.b $43838
0x43830: 93c9             suba.l a1, a1
0x43832: 4a84             tst.l d4
0x43834: 6e8e             bgt.b $437c4
0x43836: 4214             clr.b (a4)
0x43838: 2009             move.l a1, d0
0x4383a: 5280             addq.l #$1, d0
0x4383c: d084             add.l d4, d0
0x4383e: 4cd71c3c         movem.l (a7), d2-d5/a2-a4
0x43842: 4fef0024         lea.l $24(a7), a7
0x43846: 4e75             rts 
0x43848: 4fefffe0         lea.l -$20(a7), a7
0x4384c: 48d70cfc         movem.l d2-d7/a2-a3, (a7)
0x43850: 266f0024         movea.l $24(a7), a3
0x43854: 282f002c         move.l $2c(a7), d4
0x43858: 67000092         beq.w $438ec
0x4385c: 4285             clr.l d5
0x4385e: 4286             clr.l d6
0x43860: 45f90029f8a8     lea.l $29f8a8.l, a2
0x43866: 1e335800         move.b (a3, d5.l), d7
0x4386a: 2245             movea.l d5, a1
0x4386c: 5289             addq.l #$1, a1
0x4386e: 41f35801         lea.l $1(a3, d5.l), a0
0x43872: 7401             moveq #$1, d2
0x43874: 4283             clr.l d3
0x43876: 1607             move.b d7, d3
0x43878: b889             cmp.l a1, d4
0x4387a: 633a             bls.b $438b6
0x4387c: 2202             move.l d2, d1
0x4387e: 5281             addq.l #$1, d1
0x43880: 1401             move.b d1, d2
0x43882: 4280             clr.l d0
0x43884: 1010             move.b (a0), d0
0x43886: b083             cmp.l d3, d0
0x43888: 6650             bne.b $438da
0x4388a: 5289             addq.l #$1, a1
0x4388c: 5288             addq.l #$1, a0
0x4388e: 4280             clr.l d0
0x43890: 1001             move.b d1, d0
0x43892: 727f             moveq #$7f, d1
0x43894: b280             cmp.l d0, d1
0x43896: 66e0             bne.b $43878
0x43898: 06850000007f     addi.l #$7f, d5
0x4389e: 7080             moveq #$80, d0
0x438a0: 8082             or.l d2, d0
0x438a2: 15806800         move.b d0, (a2, d6.l)
0x438a6: 5286             addq.l #$1, d6
0x438a8: 15876800         move.b d7, (a2, d6.l)
0x438ac: 5286             addq.l #$1, d6
0x438ae: b885             cmp.l d5, d4
0x438b0: 66b4             bne.b $43866
0x438b2: 2006             move.l d6, d0
0x438b4: 6038             bra.b $438ee
0x438b6: 2a09             move.l a1, d5
0x438b8: 4280             clr.l d0
0x438ba: 1002             move.b d2, d0
0x438bc: 7201             moveq #$1, d1
0x438be: b280             cmp.l d0, d1
0x438c0: 66dc             bne.b $4389e
0x438c2: 4a07             tst.b d7
0x438c4: 6ce2             bge.b $438a8
0x438c6: 7081             moveq #$81, d0
0x438c8: 15806800         move.b d0, (a2, d6.l)
0x438cc: 5286             addq.l #$1, d6
0x438ce: 15876800         move.b d7, (a2, d6.l)
0x438d2: 5286             addq.l #$1, d6
0x438d4: b885             cmp.l d5, d4
0x438d6: 668e             bne.b $43866
0x438d8: 60d8             bra.b $438b2
0x438da: 2401             move.l d1, d2
0x438dc: 5382             subq.l #$1, d2
0x438de: 4280             clr.l d0
0x438e0: 1002             move.b d2, d0
0x438e2: da80             add.l d0, d5
0x438e4: 7201             moveq #$1, d1
0x438e6: b280             cmp.l d0, d1
0x438e8: 66b4             bne.b $4389e
0x438ea: 60d6             bra.b $438c2
0x438ec: 4280             clr.l d0
0x438ee: 2f00             move.l d0, -(a7)
0x438f0: 2f2f002c         move.l $2c(a7), -(a7)
0x438f4: 48790029f8a8     pea.l $29f8a8.l
0x438fa: 4ebafe64         jsr $43760(pc)
0x438fe: 4fef000c         lea.l $c(a7), a7
0x43902: 4cd70cfc         movem.l (a7), d2-d7/a2-a3
0x43906: 4fef0020         lea.l $20(a7), a7
0x4390a: 4e75             rts 
0x4390c: 226f0008         movea.l $8(a7), a1
0x43910: 202f0004         move.l $4(a7), d0
0x43914: 5080             addq.l #$8, d0
0x43916: 13c000500001     move.b d0, $500001.l
0x4391c: 7280             moveq #$80, d1
0x4391e: 8081             or.l d1, d0
0x43920: 13c000500001     move.b d0, $500001.l
0x43926: 103900500001     move.b $500001.l, d0
0x4392c: 6df8             blt.b $43926
0x4392e: 207c00500004     movea.l #$500004, a0
0x43934: 7033             moveq #$33, d0
0x43936: 2099             move.l (a1)+, (a0)
0x43938: 2099             move.l (a1)+, (a0)
0x4393a: 2099             move.l (a1)+, (a0)
0x4393c: 2099             move.l (a1)+, (a0)
0x4393e: 5980             subq.l #$4, d0
0x43940: 6af4             bpl.b $43936
0x43942: 4e75             rts 
0x43944: 226f0008         movea.l $8(a7), a1
0x43948: 202f0004         move.l $4(a7), d0
0x4394c: 5080             addq.l #$8, d0
0x4394e: 13c000600001     move.b d0, $600001.l
0x43954: 7280             moveq #$80, d1
0x43956: 8081             or.l d1, d0
0x43958: 13c000600001     move.b d0, $600001.l
0x4395e: 103900600001     move.b $600001.l, d0
0x43964: 6df8             blt.b $4395e
0x43966: 207c00600004     movea.l #$600004, a0
0x4396c: 7033             moveq #$33, d0
0x4396e: 2099             move.l (a1)+, (a0)
0x43970: 2099             move.l (a1)+, (a0)
0x43972: 2099             move.l (a1)+, (a0)
0x43974: 2099             move.l (a1)+, (a0)
0x43976: 5980             subq.l #$4, d0
0x43978: 6af4             bpl.b $4396e
0x4397a: 4e75             rts 
0x4397c: 2f02             move.l d2, -(a7)
0x4397e: 40c1             move.w sr, d1
0x43980: 46fc2700         move.w #$2700, sr
0x43984: 203c00247dba     move.l #$247dba, d0
0x4398a: 23c001000070     move.l d0, $1000070.l
0x43990: 7493             moveq #$93, d2
0x43992: 13c200300017     move.b d2, $300017.l
0x43998: 207c00300036     movea.l #$300036, a0
0x4399e: 3010             move.w (a0), d0
0x439a0: 74ef             moveq #$ef, d2
0x439a2: c082             and.l d2, d0
0x439a4: 3080             move.w d0, (a0)
0x439a6: 7081             moveq #$81, d0
0x439a8: 13c000600000     move.b d0, $600000.l
0x439ae: 207c00500004     movea.l #$500004, a0
0x439b4: 2010             move.l (a0), d0
0x439b6: 2010             move.l (a0), d0
0x439b8: 2010             move.l (a0), d0
0x439ba: 227c00700000     movea.l #$700000, a1
0x439c0: 2011             move.l (a1), d0
0x439c2: d1fc00100000     adda.l #$100000, a0
0x439c8: 2010             move.l (a0), d0
0x439ca: 2010             move.l (a0), d0
0x439cc: 2010             move.l (a0), d0
0x439ce: 2011             move.l (a1), d0
0x439d0: 46c1             move.w d1, sr
0x439d2: 241f             move.l (a7)+, d2
0x439d4: 4e75             rts 
0x439d6: 2f0a             move.l a2, -(a7)
0x439d8: 2f02             move.l d2, -(a7)
0x439da: 202f000c         move.l $c(a7), d0
0x439de: 242f0010         move.l $10(a7), d2
0x439e2: 7201             moveq #$1, d1
0x439e4: 23c10029bfb8     move.l d1, $29bfb8.l
0x439ea: 123c000b         move.b #$b, d1
0x439ee: e3a8             lsl.l d1, d0
0x439f0: 2200             move.l d0, d1
0x439f2: 068100150000     addi.l #$150000, d1
0x439f8: 23c100500004     move.l d1, $500004.l
0x439fe: 700b             moveq #$b, d0
0x43a00: 13c000500001     move.b d0, $500001.l
0x43a06: 708b             moveq #$8b, d0
0x43a08: 13c000500001     move.b d0, $500001.l
0x43a0e: 103900500001     move.b $500001.l, d0
0x43a14: 6df8             blt.b $43a0e
0x43a16: 23c100600004     move.l d1, $600004.l
0x43a1c: 720b             moveq #$b, d1
0x43a1e: 13c100600001     move.b d1, $600001.l
0x43a24: 708b             moveq #$8b, d0
0x43a26: 13c000600001     move.b d0, $600001.l
0x43a2c: 103900600001     move.b $600001.l, d0
0x43a32: 6df8             blt.b $43a2c
0x43a34: 207c00600004     movea.l #$600004, a0
0x43a3a: 247c00500004     movea.l #$500004, a2
0x43a40: 203c000003ff     move.l #$3ff, d0
0x43a46: 2242             movea.l d2, a1
0x43a48: 5389             subq.l #$1, a1
0x43a4a: 2211             move.l (a1), d1
0x43a4c: 2081             move.l d1, (a0)
0x43a4e: 2481             move.l d1, (a2)
0x43a50: 5689             addq.l #$3, a1
0x43a52: 2211             move.l (a1), d1
0x43a54: 2081             move.l d1, (a0)
0x43a56: 2481             move.l d1, (a2)
0x43a58: 5689             addq.l #$3, a1
0x43a5a: 5380             subq.l #$1, d0
0x43a5c: 6aec             bpl.b $43a4a
0x43a5e: 42b90029bfb8     clr.l $29bfb8.l
0x43a64: 241f             move.l (a7)+, d2
0x43a66: 245f             movea.l (a7)+, a2
0x43a68: 4e75             rts 
0x43a6a: 2f0a             move.l a2, -(a7)
0x43a6c: 202f0008         move.l $8(a7), d0
0x43a70: 7201             moveq #$1, d1
0x43a72: 23c10029bfb8     move.l d1, $29bfb8.l
0x43a78: 123c000b         move.b #$b, d1
0x43a7c: e3a8             lsl.l d1, d0
0x43a7e: 2200             move.l d0, d1
0x43a80: 068100150000     addi.l #$150000, d1
0x43a86: 23c100500004     move.l d1, $500004.l
0x43a8c: 700b             moveq #$b, d0
0x43a8e: 13c000500001     move.b d0, $500001.l
0x43a94: 708b             moveq #$8b, d0
0x43a96: 13c000500001     move.b d0, $500001.l
0x43a9c: 103900500001     move.b $500001.l, d0
0x43aa2: 6df8             blt.b $43a9c
0x43aa4: 23c100600004     move.l d1, $600004.l
0x43aaa: 720b             moveq #$b, d1
0x43aac: 13c100600001     move.b d1, $600001.l
0x43ab2: 708b             moveq #$8b, d0
0x43ab4: 13c000600001     move.b d0, $600001.l
0x43aba: 103900600001     move.b $600001.l, d0
0x43ac0: 6df8             blt.b $43aba
0x43ac2: 207c00600004     movea.l #$600004, a0
0x43ac8: 247c00500004     movea.l #$500004, a2
0x43ace: 203c000003ff     move.l #$3ff, d0
0x43ad4: 227c00000000     movea.l #$0, a1
0x43ada: 5389             subq.l #$1, a1
0x43adc: 7200             moveq #$0, d1
0x43ade: 2081             move.l d1, (a0)
0x43ae0: 2481             move.l d1, (a2)
0x43ae2: 2081             move.l d1, (a0)
0x43ae4: 2481             move.l d1, (a2)
0x43ae6: 5380             subq.l #$1, d0
0x43ae8: 6af2             bpl.b $43adc
0x43aea: 42b90029bfb8     clr.l $29bfb8.l
0x43af0: 245f             movea.l (a7)+, a2
0x43af2: 4e75             rts 
0x43af4: 2f02             move.l d2, -(a7)
0x43af6: 202f0008         move.l $8(a7), d0
0x43afa: 242f000c         move.l $c(a7), d2
0x43afe: 720b             moveq #$b, d1
0x43b00: e3a8             lsl.l d1, d0
0x43b02: 068000150000     addi.l #$150000, d0
0x43b08: 23c000600004     move.l d0, $600004.l
0x43b0e: 700b             moveq #$b, d0
0x43b10: 13c000600001     move.b d0, $600001.l
0x43b16: 728b             moveq #$8b, d1
0x43b18: 13c100600001     move.b d1, $600001.l
0x43b1e: 103900600001     move.b $600001.l, d0
0x43b24: 6df8             blt.b $43b1e
0x43b26: 207c00600004     movea.l #$600004, a0
0x43b2c: 203c000003ff     move.l #$3ff, d0
0x43b32: 2242             movea.l d2, a1
0x43b34: 5389             subq.l #$1, a1
0x43b36: 2211             move.l (a1), d1
0x43b38: 2081             move.l d1, (a0)
0x43b3a: 5689             addq.l #$3, a1
0x43b3c: 2211             move.l (a1), d1
0x43b3e: 2081             move.l d1, (a0)
0x43b40: 5689             addq.l #$3, a1
0x43b42: 5380             subq.l #$1, d0
0x43b44: 6af0             bpl.b $43b36
0x43b46: 241f             move.l (a7)+, d2
0x43b48: 4e75             rts 
0x43b4a: 4feffff0         lea.l -$10(a7), a7
0x43b4e: 48d7003c         movem.l d2-d5, (a7)
0x43b52: 282f0014         move.l $14(a7), d4
0x43b56: 226f0018         movea.l $18(a7), a1
0x43b5a: 303900300036     move.w $300036.l, d0
0x43b60: 7210             moveq #$10, d1
0x43b62: 8081             or.l d1, d0
0x43b64: 33c000300036     move.w d0, $300036.l
0x43b6a: 700b             moveq #$b, d0
0x43b6c: e1ac             lsl.l d0, d4
0x43b6e: 4285             clr.l d5
0x43b70: 2604             move.l d4, d3
0x43b72: 068300150001     addi.l #$150001, d3
0x43b78: 7206             moveq #$6, d1
0x43b7a: 13c100600001     move.b d1, $600001.l
0x43b80: 7086             moveq #$86, d0
0x43b82: 13c000600001     move.b d0, $600001.l
0x43b88: 103900600001     move.b $600001.l, d0
0x43b8e: 6df8             blt.b $43b88
0x43b90: 2004             move.l d4, d0
0x43b92: 068000150000     addi.l #$150000, d0
0x43b98: 23c000600004     move.l d0, $600004.l
0x43b9e: 103900600002     move.b $600002.l, d0
0x43ba4: 08000000         btst.b #$0, d0
0x43ba8: 67f4             beq.b $43b9e
0x43baa: 243900600004     move.l $600004.l, d2
0x43bb0: 7206             moveq #$6, d1
0x43bb2: 13c100600001     move.b d1, $600001.l
0x43bb8: 7086             moveq #$86, d0
0x43bba: 13c000600001     move.b d0, $600001.l
0x43bc0: 103900600001     move.b $600001.l, d0
0x43bc6: 6df8             blt.b $43bc0
0x43bc8: 23c300600004     move.l d3, $600004.l
0x43bce: 103900600002     move.b $600002.l, d0
0x43bd4: 08000000         btst.b #$0, d0
0x43bd8: 67f4             beq.b $43bce
0x43bda: 223900600004     move.l $600004.l, d1
0x43be0: 2002             move.l d2, d0
0x43be2: e088             lsr.l #$8, d0
0x43be4: 2049             movea.l a1, a0
0x43be6: 30c0             move.w d0, (a0)+
0x43be8: 0282000000ff     andi.l #$ff, d2
0x43bee: e18a             lsl.l #$8, d2
0x43bf0: 2001             move.l d1, d0
0x43bf2: 4240             clr.w d0
0x43bf4: 4840             swap d0
0x43bf6: 8480             or.l d0, d2
0x43bf8: 3082             move.w d2, (a0)
0x43bfa: 33410004         move.w d1, $4(a1)
0x43bfe: 5285             addq.l #$1, d5
0x43c00: 5484             addq.l #$2, d4
0x43c02: 5483             addq.l #$2, d3
0x43c04: 0c85000003fe     cmpi.l #$3fe, d5
0x43c0a: 6716             beq.b $43c22
0x43c0c: 5c89             addq.l #$6, a1
0x43c0e: 7206             moveq #$6, d1
0x43c10: 13c100600001     move.b d1, $600001.l
0x43c16: 7086             moveq #$86, d0
0x43c18: 13c000600001     move.b d0, $600001.l
0x43c1e: 6000ff68         bra.w $43b88
0x43c22: 203900700000     move.l $700000.l, d0
0x43c28: 303900300036     move.w $300036.l, d0
0x43c2e: 72ef             moveq #$ef, d1
0x43c30: c081             and.l d1, d0
0x43c32: 33c000300036     move.w d0, $300036.l
0x43c38: 4cd7003c         movem.l (a7), d2-d5
0x43c3c: 4fef0010         lea.l $10(a7), a7
0x43c40: 4e75             rts 
0x43c42: 2f02             move.l d2, -(a7)
0x43c44: 222f0008         move.l $8(a7), d1
0x43c48: 40c2             move.w sr, d2
0x43c4a: 46fc2700         move.w #$2700, sr
0x43c4e: 203c000004fe     move.l #$4fe, d0
0x43c54: 23c000600004     move.l d0, $600004.l
0x43c5a: 7007             moveq #$7, d0
0x43c5c: 13c000600001     move.b d0, $600001.l
0x43c62: 7087             moveq #$87, d0
0x43c64: 13c000600001     move.b d0, $600001.l
0x43c6a: 103900600001     move.b $600001.l, d0
0x43c70: 6df8             blt.b $43c6a
0x43c72: 207c00600004     movea.l #$600004, a0
0x43c78: 2081             move.l d1, (a0)
0x43c7a: 203900600004     move.l $600004.l, d0
0x43c80: 203900600004     move.l $600004.l, d0
0x43c86: 203900600004     move.l $600004.l, d0
0x43c8c: 23c10029bfb4     move.l d1, $29bfb4.l
0x43c92: 46c2             move.w d2, sr
0x43c94: 241f             move.l (a7)+, d2
0x43c96: 4e75             rts 
0x43c98: 206f0004         movea.l $4(a7), a0
0x43c9c: 303cf0f0         move.w #$f0f0, d0
0x43ca0: 33c01000aaaa     move.w d0, $1000aaaa.l
0x43ca6: 303caaaa         move.w #$aaaa, d0
0x43caa: 33c01000aaaa     move.w d0, $1000aaaa.l
0x43cb0: 303c5555         move.w #$5555, d0
0x43cb4: 33c010005554     move.w d0, $10005554.l
0x43cba: 303c8080         move.w #$8080, d0
0x43cbe: 33c01000aaaa     move.w d0, $1000aaaa.l
0x43cc4: 303caaaa         move.w #$aaaa, d0
0x43cc8: 33c01000aaaa     move.w d0, $1000aaaa.l
0x43cce: 303c5555         move.w #$5555, d0
0x43cd2: 33c010005554     move.w d0, $10005554.l
0x43cd8: 30bc3030         move.w #$3030, (a0)
0x43cdc: 3010             move.w (a0), d0
0x43cde: 02800000ffff     andi.l #$ffff, d0
0x43ce4: 0c800000ffff     cmpi.l #$ffff, d0
0x43cea: 66f0             bne.b $43cdc
0x43cec: 4e75             rts 
0x43cee: 207c1000aaaa     movea.l #$1000aaaa, a0
0x43cf4: 30bcf0f0         move.w #$f0f0, (a0)
0x43cf8: 30bcaaaa         move.w #$aaaa, (a0)
0x43cfc: 227c10005554     movea.l #$10005554, a1
0x43d02: 32bc5555         move.w #$5555, (a1)
0x43d06: 30bc8080         move.w #$8080, (a0)
0x43d0a: 30bcaaaa         move.w #$aaaa, (a0)
0x43d0e: 32bc5555         move.w #$5555, (a1)
0x43d12: 206f0004         movea.l $4(a7), a0
0x43d16: 30bc3030         move.w #$3030, (a0)
0x43d1a: 4e75             rts 
0x43d1c: 206f0004         movea.l $4(a7), a0
0x43d20: 202f0008         move.l $8(a7), d0
0x43d24: 323caaaa         move.w #$aaaa, d1
0x43d28: 33c11000aaaa     move.w d1, $1000aaaa.l
0x43d2e: 323c5555         move.w #$5555, d1
0x43d32: 33c110005554     move.w d1, $10005554.l
0x43d38: 323ca0a0         move.w #$a0a0, d1
0x43d3c: 33c11000         move.w d1, $aaaaaaaa.l
