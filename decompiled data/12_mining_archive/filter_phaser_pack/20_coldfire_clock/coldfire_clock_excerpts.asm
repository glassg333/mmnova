; ColdFire main OS (coldfire_main.bin, base $200000) - timing/clock module excerpts
; Iteration 27. Linear disasm source: exp26_cf_full.asm. Extracted ranges below.

; ==========================================================================
; RESET PROLOGUE: ROM $2676E0..$267F82 -> RAM $1000088..$1000929 (timing module copy)  ($200000-$200038)
; ==========================================================================
200000  2e7c00300000   movea.l #$300000, a7
200006  207c002676e0   movea.l #$2676e0, a0
20000c  227c01000088   movea.l #$1000088, a1
200012  203c0100092a   move.l #$100092a, d0
200018  9089           sub.l a1, d0
20001a  6706           beq.b $200022
20001c  12d8           move.b (a0)+, (a1)+
20001e  5380           subq.l #$1, d0
200020  6efa           bgt.b $20001c
200022  207c002676e0   movea.l #$2676e0, a0
200028  203c002c1be8   move.l #$2c1be8, d0
20002e  9088           sub.l a0, d0
200030  e480           asr.l #$2, d0
200032  4298           clr.l (a0)+
200034  5380           subq.l #$1, d0
200036  6efa           bgt.b $200032
200038  4ef90022b5a4   jmp $22b5a4.l

; ==========================================================================
; $2403D4 OS param->cell ROUTER (8 groups, jump table at $240400)  ($2403D4-$240494)
; ==========================================================================
2403d4  4feffff4       lea.l -$c(a7), a7
2403d8  48d7040c       movem.l d2-d3/a2, (a7)
2403dc  262f0010       move.l $10(a7), d3
2403e0  242f0014       move.l $14(a7), d2
2403e4  226f0018       movea.l $18(a7), a1
2403e8  203c010013e4   move.l #$10013e4, d0
2403ee  7208           moveq #$8, d1
2403f0  b282           cmp.l d2, d1
2403f2  65000098       bcs.w $24048c
2403f6  303b2a08       move.w $240400(pc, d2.l), d0
2403fa  48c0           ext.l d0
2403fc  4efb0802       jmp $240400(pc, d0.l)
240400  00220072       ori.b #$72, -(a2)
240404  00500050       ori.w #$50, (a0)
240408  00500038       ori.w #$38, (a0)
24040c  003800380012   ori.b #$38, $12.w
240412  e78b           lsl.l #$3, d3
240414  2003           move.l d3, d0
240416  d089           add.l a1, d0
240418  e588           lsl.l #$2, d0
24041a  068001001174   addi.l #$1001174, d0
240420  606a           bra.b $24048c
240422  2203           move.l d3, d1
240424  e589           lsl.l #$2, d1
240426  2003           move.l d3, d0
240428  e988           lsl.l #$4, d0
24042a  9081           sub.l d1, d0
24042c  d083           add.l d3, d0
24042e  e988           lsl.l #$4, d0
240430  068001000acc   addi.l #$1000acc, d0
240436  6054           bra.b $24048c
240438  2443           movea.l d3, a2
24043a  41f23a00       lea.l (a2, d3.l * 2), a0
24043e  2008           move.l a0, d0
240440  d082           add.l d2, d0
240442  e788           lsl.l #$3, d0
240444  d089           add.l a1, d0
240446  e588           lsl.l #$2, d0
240448  068001000e94   addi.l #$1000e94, d0
24044e  603c           bra.b $24048c
240450  e78a           lsl.l #$3, d2
240452  2203           move.l d3, d1
240454  e589           lsl.l #$2, d1
240456  2003           move.l d3, d0
240458  e988           lsl.l #$4, d0
24045a  9081           sub.l d1, d0
24045c  d083           add.l d3, d0
24045e  2442           movea.l d2, a2
240460  41f20c00       lea.l (a2, d0.l * 4), a0
240464  2008           move.l a0, d0
240466  d089           add.l a1, d0
240468  e588           lsl.l #$2, d0
24046a  068001000a14   addi.l #$1000a14, d0
240470  601a           bra.b $24048c
240472  2203           move.l d3, d1
240474  e589           lsl.l #$2, d1
240476  2003           move.l d3, d0
240478  e988           lsl.l #$4, d0
24047a  9081           sub.l d1, d0
24047c  d083           add.l d3, d0
24047e  43f10c00       lea.l (a1, d0.l * 4), a1
240482  2009           move.l a1, d0
240484  e588           lsl.l #$2, d0
240486  068001000b04   addi.l #$1000b04, d0
24048c  4cd7040c       movem.l (a7), d2-d3/a2
240490  4fef000c       lea.l $c(a7), a7
240494  4e75           rts 

; ==========================================================================
; $242058 param-set handler: builds glide ptrs $1001508+t*$30, mirrors $70028A->$2C0262  ($242058-$242112)
; ==========================================================================
242058  4fefffc0       lea.l -$40(a7), a7
24205c  48d77cfc       movem.l d2-d7/a2-a6, (a7)
242060  2e2f0044       move.l $44(a7), d7
242064  2c2f0048       move.l $48(a7), d6
242068  2006           move.l d6, d0
24206a  e588           lsl.l #$2, d0
24206c  2f400038       move.l d0, $38(a7)
242070  2006           move.l d6, d0
242072  e988           lsl.l #$4, d0
242074  2f400034       move.l d0, $34(a7)
242078  90af0038       sub.l $38(a7), d0
24207c  2640           movea.l d0, a3
24207e  d7fc0070028a   adda.l #$70028a, a3
242084  2007           move.l d7, d0
242086  e588           lsl.l #$2, d0
242088  2f40003c       move.l d0, $3c(a7)
24208c  2007           move.l d7, d0
24208e  e988           lsl.l #$4, d0
242090  2c40           movea.l d0, a6
242092  90af003c       sub.l $3c(a7), d0
242096  2440           movea.l d0, a2
242098  d5fc002c0262   adda.l #$2c0262, a2
24209e  2007           move.l d7, d0
2420a0  ed88           lsl.l #$6, d0
2420a2  2f400030       move.l d0, $30(a7)
2420a6  908e           sub.l a6, d0
2420a8  2840           movea.l d0, a4
2420aa  d9fc01001508   adda.l #$1001508, a4
2420b0  4282           clr.l d2
2420b2  4bfae320       lea.l $2403d4(pc), a5
2420b6  1493           move.b (a3), (a2)
2420b8  156b00010001   move.b $1(a3), $1(a2)
2420be  122bff70       move.b -$90(a3), d1
2420c2  1541ff70       move.b d1, -$90(a2)
2420c6  102bffb8       move.b -$48(a3), d0
2420ca  1540ffb8       move.b d0, -$48(a2)
2420ce  49c0           extb.l d0
2420d0  2f00           move.l d0, -(a7)
2420d2  49c1           extb.l d1
2420d4  2f01           move.l d1, -(a7)
2420d6  2f07           move.l d7, -(a7)
2420d8  4e95           jsr (a5)
2420da  4fef000c       lea.l $c(a7), a7
2420de  2880           move.l d0, (a4)
2420e0  122bff71       move.b -$8f(a3), d1
2420e4  1541ff71       move.b d1, -$8f(a2)
2420e8  102bffb9       move.b -$47(a3), d0
2420ec  1540ffb9       move.b d0, -$47(a2)
2420f0  49c0           extb.l d0
2420f2  2f00           move.l d0, -(a7)
2420f4  49c1           extb.l d1
2420f6  2f01           move.l d1, -(a7)
2420f8  2f07           move.l d7, -(a7)
2420fa  4e95           jsr (a5)
2420fc  4fef000c       lea.l $c(a7), a7
242100  29400018       move.l d0, $18(a4)
242104  5282           addq.l #$1, d2
242106  548b           addq.l #$2, a3
242108  548a           addq.l #$2, a2
24210a  588c           addq.l #$4, a4
24210c  7006           moveq #$6, d0
24210e  b082           cmp.l d2, d0
242110  66a4           bne.b $2420b6
242112  2f06           move.l d6, -(a7)

; ==========================================================================
; PATCH/STATE LOAD: $700039 -> $1001880+t*$120 -> page words $00-$2F + tempo params $1000F34  ($2425A0-$242700)
; ==========================================================================
2425a2  12305801       move.b $1(a0, d5.l), d1
2425a6  49c1           extb.l d1
2425a8  2005           move.l d5, d0
2425aa  e588           lsl.l #$2, d0
2425ac  2040           movea.l d0, a0
2425ae  d1fc01000948   adda.l #$1000948, a0
2425b4  214115f8       move.l d1, $15f8(a0)
2425b8  2805           move.l d5, d4
2425ba  e98c           lsl.l #$4, d4
2425bc  2404           move.l d4, d2
2425be  9480           sub.l d0, d2
2425c0  2002           move.l d2, d0
2425c2  d085           add.l d5, d0
2425c4  e988           lsl.l #$4, d0
2425c6  2040           movea.l d0, a0
2425c8  d1fc01000ad0   adda.l #$1000ad0, a0
2425ce  4841           swap d1
2425d0  4241           clr.w d1
2425d2  2081           move.l d1, (a0)
2425d4  2205           move.l d5, d1
2425d6  eb89           lsl.l #$5, d1
2425d8  2005           move.l d5, d0
2425da  e188           lsl.l #$8, d0
2425dc  2e01           move.l d1, d7
2425de  de80           add.l d0, d7
2425e0  2247           movea.l d7, a1
2425e2  d3fc01001880   adda.l #$1001880, a1
2425e8  2005           move.l d5, d0
2425ea  e788           lsl.l #$3, d0
2425ec  2605           move.l d5, d3
2425ee  ed8b           lsl.l #$6, d3
2425f0  d083           add.l d3, d0
2425f2  2040           movea.l d0, a0
2425f4  d1fc00700039   adda.l #$700039, a0
2425fa  7448           moveq #$48, d2
2425fc  4281           clr.l d1
2425fe  4280           clr.l d0
242600  3018           move.w (a0)+, d0
242602  1200           move.b d0, d1
242604  e080           asr.l #$8, d0
242606  22c0           move.l d0, (a1)+
242608  22c1           move.l d1, (a1)+
24260a  5582           subq.l #$2, d2
24260c  66f2           bne.b $242600
24260e  223c010013e4   move.l #$10013e4, d1
242614  9684           sub.l d4, d3
242616  2043           movea.l d3, a0
242618  d1fc01001748   adda.l #$1001748, a0
24261e  7006           moveq #$6, d0
242620  20c1           move.l d1, (a0)+
242622  20c1           move.l d1, (a0)+
242624  5380           subq.l #$1, d0
242626  66f8           bne.b $242620
242628  2605           move.l d5, d3
24262a  e58b           lsl.l #$2, d3
24262c  2405           move.l d5, d2
24262e  e98a           lsl.l #$4, d2
242630  2202           move.l d2, d1
242632  9283           sub.l d3, d1
242634  2041           movea.l d1, a0
242636  d1fc01000948   adda.l #$1000948, a0
24263c  203c010013e4   move.l #$10013e4, d0
242642  21400a54       move.l d0, $a54(a0)
242646  21400a58       move.l d0, $a58(a0)
24264a  21400a5c       move.l d0, $a5c(a0)
24264e  2805           move.l d5, d4
242650  eb8c           lsl.l #$5, d4
242652  2c05           move.l d5, d6
242654  e18e           lsl.l #$8, d6
242656  2004           move.l d4, d0
242658  d086           add.l d6, d0
24265a  2240           movea.l d0, a1
24265c  d3fc01001880   adda.l #$1001880, a1
242662  d285           add.l d5, d1
242664  e989           lsl.l #$4, d1
242666  2041           movea.l d1, a0
242668  d1fc01000b04   adda.l #$1000b04, a0
24266e  4281           clr.l d1
242670  2019           move.l (a1)+, d0
242672  4840           swap d0
242674  4240           clr.w d0
242676  20c0           move.l d0, (a0)+
242678  5281           addq.l #$1, d1
24267a  7008           moveq #$8, d0
24267c  b081           cmp.l d1, d0
24267e  66f0           bne.b $242670
242680  2004           move.l d4, d0
242682  d086           add.l d6, d0
242684  2240           movea.l d0, a1
242686  d3fc01001980   adda.l #$1001980, a1
24268c  2002           move.l d2, d0
24268e  9083           sub.l d3, d0
242690  d085           add.l d5, d0
242692  e988           lsl.l #$4, d0
242694  2040           movea.l d0, a0
242696  d1fc01000ab4   adda.l #$1000ab4, a0
24269c  4281           clr.l d1
24269e  2019           move.l (a1)+, d0
2426a0  4840           swap d0
2426a2  4240           clr.w d0
2426a4  20c0           move.l d0, (a0)+
2426a6  5281           addq.l #$1, d1
2426a8  7005           moveq #$5, d0
2426aa  b081           cmp.l d1, d0
2426ac  66f0           bne.b $24269e
2426ae  9483           sub.l d3, d2
2426b0  2002           move.l d2, d0
2426b2  d085           add.l d5, d0
2426b4  e988           lsl.l #$4, d0
2426b6  2240           movea.l d0, a1
2426b8  d3fc01000a54   adda.l #$1000a54, a1
2426be  2047           movea.l d7, a0
2426c0  d1fc010018a2   adda.l #$10018a2, a0
2426c6  7018           moveq #$18, d0
2426c8  2200           move.l d0, d1
2426ca  22d8           move.l (a0)+, (a1)+
2426cc  22d8           move.l (a0)+, (a1)+
2426ce  22d8           move.l (a0)+, (a1)+
2426d0  22d8           move.l (a0)+, (a1)+
2426d2  5981           subq.l #$4, d1
2426d4  66f4           bne.b $2426ca
2426d6  2205           move.l d5, d1
2426d8  ef89           lsl.l #$7, d1
2426da  9284           sub.l d4, d1
2426dc  2241           movea.l d1, a1
2426de  d3fc01000f34   adda.l #$1000f34, a1
2426e4  2047           movea.l d7, a0
2426e6  d1fc01001902   adda.l #$1001902, a0
2426ec  22d8           move.l (a0)+, (a1)+
2426ee  22d8           move.l (a0)+, (a1)+
2426f0  22d8           move.l (a0)+, (a1)+
2426f2  22d8           move.l (a0)+, (a1)+
2426f4  5980           subq.l #$4, d0
2426f6  66f4           bne.b $2426ec
2426f8  2f05           move.l d5, -(a7)
2426fa  4ebafc14       jsr $242310(pc)
2426fe  2f05           move.l d5, -(a7)
242700  4ebae252       jsr $240954(pc)

; ==========================================================================
; DSP2->host interrupt: $29C040 = position from $600004  ($247DB8-$247E52)
; ==========================================================================
247db8  4e71           nop 
247dba  46fc2700       move.w #$2700, sr
247dbe  4fefff94       lea.l -$6c(a7), a7
247dc2  48d77fff       movem.l d0-d7/a0-a6, (a7)
247dc6  243900600004   move.l $600004.l, d2
247dcc  0c82000003ff   cmpi.l #$3ff, d2
247dd2  6f06           ble.b $247dda
247dd4  243900600004   move.l $600004.l, d2
247dda  203900700000   move.l $700000.l, d0
247de0  203900600004   move.l $600004.l, d0
247de6  23c00029c040   move.l d0, $29c040.l
247dec  203900600004   move.l $600004.l, d0
247df2  203900700000   move.l $700000.l, d0
247df8  20390029bfb8   move.l $29bfb8.l, d0
247dfe  6618           bne.b $247e18
247e00  103c0002       move.b #$2, d0
247e04  b082           cmp.l d2, d0
247e06  670003fc       beq.w $248204
247e0a  7001           moveq #$1, d0
247e0c  b082           cmp.l d2, d0
247e0e  6700055a       beq.w $24836a
247e12  4a82           tst.l d2
247e14  67000478       beq.w $24828e
247e18  4ab90026cfec   tst.l $26cfec.l
247e1e  66000912       bne.w $248732
247e22  7201           moveq #$1, d1
247e24  23c10029be24   move.l d1, $29be24.l
247e2a  46fc2300       move.w #$2300, sr
247e2e  4a82           tst.l d2
247e30  6720           beq.b $247e52
247e32  42b90029be24   clr.l $29be24.l
247e38  4ab90026d054   tst.l $26d054.l
247e3e  670008f2       beq.w $248732
247e42  42b90026d054   clr.l $26d054.l
247e48  4eb90023901e   jsr $23901e.l
247e4e  600008e2       bra.w $248732
247e52  4ab90029c048   tst.l $29c048.l

; ==========================================================================
; FRAME LOOP: param decay (slot+$4), 8th-frame gate, phase advance (slot+$0C), $29BD6C=1  ($247FC6-$248090)
; ==========================================================================
247fc6  2202           move.l d2, d1
247fc8  eb89           lsl.l #$5, d1
247fca  2002           move.l d2, d0
247fcc  ef88           lsl.l #$7, d0
247fce  9081           sub.l d1, d0
247fd0  2440           movea.l d0, a2
247fd2  d5fc01000f4c   adda.l #$1000f4c, a2
247fd8  2202           move.l d2, d1
247fda  d281           add.l d1, d1
247fdc  2002           move.l d2, d0
247fde  e788           lsl.l #$3, d0
247fe0  9081           sub.l d1, d0
247fe2  2200           move.l d0, d1
247fe4  e989           lsl.l #$4, d1
247fe6  2240           movea.l d0, a1
247fe8  41f11846       lea.l $46(a1, d1.l), a0
247fec  d1fc002bfc16   adda.l #$2bfc16, a0
247ff2  93c9           suba.l a1, a1
247ff4  2012           move.l (a2), d0
247ff6  67000350       beq.w $248348
247ffa  22280004       move.l $4(a0), d1
247ffe  6d000352       blt.w $248352
248002  ec80           asr.l #$6, d0
248004  4c030800       muls.l d3, d0
248008  9280           sub.l d0, d1
24800a  21410004       move.l d1, $4(a0)
24800e  5289           addq.l #$1, a1
248010  45ea0020       lea.l $20(a2), a2
248014  41e80022       lea.l $22(a0), a0
248018  7003           moveq #$3, d0
24801a  b089           cmp.l a1, d0
24801c  66d6           bne.b $247ff4
24801e  5282           addq.l #$1, d2
248020  7206           moveq #$6, d1
248022  b282           cmp.l d2, d1
248024  66a0           bne.b $247fc6
248026  4eb90100039a   jsr $100039a.l
24802c  2004           move.l d4, d0
24802e  741d           moveq #$1d, d2
248030  e5a8           lsl.l d2, d0
248032  0c8020000000   cmpi.l #$20000000, d0
248038  660a           bne.b $248044
24803a  203900259200   move.l $259200.l, d0
248040  66000552       bne.w $248594
248044  7207           moveq #$7, d1
248046  c881           and.l d1, d4
248048  6600fde8       bne.w $247e32
24804c  2039002591fc   move.l $2591fc.l, d0
248052  41f9002bfc58   lea.l $2bfc58.l, a0
248058  66000420       bne.w $24847a
24805c  2028000c       move.l $c(a0), d0
248060  d190           add.l d0, (a0)
248062  2228002e       move.l $2e(a0), d1
248066  d3a80022       add.l d1, $22(a0)
24806a  24280050       move.l $50(a0), d2
24806e  d5a80044       add.l d2, $44(a0)
248072  41e80066       lea.l $66(a0), a0
248076  b1fc002bfebc   cmpa.l #$2bfebc, a0
24807c  66de           bne.b $24805c
24807e  7001           moveq #$1, d0
248080  23c00029bd6c   move.l d0, $29bd6c.l
248086  23c0002c15d8   move.l d0, $2c15d8.l
24808c  6000fda4       bra.w $247e32
248090  46fc2700       move.w #$2700, sr

; ==========================================================================
; CALLER 1 of tempo engine: delta = $29C040-$259204 >= 0  ($248470-$2484B8)
; ==========================================================================
248470  20bc00000081   move.l #$81, (a0)
248476  6000fc7e       bra.w $2480f6
24847a  42b9002591fc   clr.l $2591fc.l
248480  20390029c040   move.l $29c040.l, d0
248486  2200           move.l d0, d1
248488  92b900259204   sub.l $259204.l, d1
24848e  23c10029c044   move.l d1, $29c044.l
248494  23c000259204   move.l d0, $259204.l
24849a  4a81           tst.l d1
24849c  6d000144       blt.w $2485e2
2484a0  2f390029c044   move.l $29c044.l, -(a7)
2484a6  4eb901000462   jsr $1000462.l
2484ac  7401           moveq #$1, d2
2484ae  23c2002591fc   move.l d2, $2591fc.l
2484b4  588f           addq.l #$4, a7
2484b6  6000f97a       bra.w $247e32

; ==========================================================================
; CALLER 2 of tempo engine: delta+=$20 (negative branch)  ($2485E2-$248608)
; ==========================================================================
2485e2  068100000020   addi.l #$20, d1
2485e8  23c10029c044   move.l d1, $29c044.l
2485ee  2f390029c044   move.l $29c044.l, -(a7)
2485f4  4eb901000462   jsr $1000462.l
2485fa  7401           moveq #$1, d2
2485fc  23c2002591fc   move.l d2, $2591fc.l
248602  588f           addq.l #$4, a7
248604  6000f82c       bra.w $247e32
248608  4879002591f8   pea.l $2591f8.l

; ==========================================================================
; HELPERS: $267A12 12-glide states/track, $267A6E filler, $267A92 filler  ($267A12-$267ABA)
; ==========================================================================
267a12  222f0004       move.l $4(a7), d1
267a16  2001           move.l d1, d0
267a18  ed88           lsl.l #$6, d0
267a1a  e989           lsl.l #$4, d1
267a1c  9081           sub.l d1, d0
267a1e  2240           movea.l d0, a1
267a20  d3fc01001508   adda.l #$1001508, a1
267a26  4281           clr.l d1
267a28  2051           movea.l (a1), a0
267a2a  23500120       move.l (a0), $120(a1)
267a2e  23480240       move.l a0, $240(a1)
267a32  2029fee0       move.l -$120(a1), d0
267a36  6f20           ble.b $267a58
267a38  d190           add.l d0, (a0)
267a3a  203c007effff   move.l #$7effff, d0
267a40  b090           cmp.l (a0), d0
267a42  6c1e           bge.b $267a62
267a44  2080           move.l d0, (a0)
267a46  601a           bra.b $267a62
267a48  2051           movea.l (a1), a0
267a4a  23500120       move.l (a0), $120(a1)
267a4e  23480240       move.l a0, $240(a1)
267a52  2029fee0       move.l -$120(a1), d0
267a56  6ee0           bgt.b $267a38
267a58  4a80           tst.l d0
267a5a  6706           beq.b $267a62
267a5c  d190           add.l d0, (a0)
267a5e  6a02           bpl.b $267a62
267a60  4290           clr.l (a0)
267a62  5281           addq.l #$1, d1
267a64  5889           addq.l #$4, a1
267a66  700c           moveq #$c, d0
267a68  b081           cmp.l d1, d0
267a6a  66dc           bne.b $267a48
267a6c  4e75           rts 
267a6e  302f0006       move.w $6(a7), d0
267a72  c0fc000c       mulu.w #$c, d0
267a76  207c010013a8   movea.l #$10013a8, a0
267a7c  d1c0           adda.l d0, a0
267a7e  2260           movea.l -(a0), a1
267a80  22a8ffb8       move.l -$48(a0), (a1)
267a84  2260           movea.l -(a0), a1
267a86  22a8ffb8       move.l -$48(a0), (a1)
267a8a  2260           movea.l -(a0), a1
267a8c  22a8ffb8       move.l -$48(a0), (a1)
267a90  4e75           rts 
267a92  302f0006       move.w $6(a7), d0
267a96  c0fc0030       mulu.w #$30, d0
267a9a  220a           move.l a2, d1
267a9c  2440           movea.l d0, a2
267a9e  d5fc01001658   adda.l #$1001658, a2
267aa4  43ea0120       lea.l $120(a2), a1
267aa8  7006           moveq #$6, d0
267aaa  2061           movea.l -(a1), a0
267aac  20a2           move.l -(a2), (a0)
267aae  2061           movea.l -(a1), a0
267ab0  20a2           move.l -(a2), (a0)
267ab2  5380           subq.l #$1, d0
267ab4  66f4           bne.b $267aaa
267ab6  2441           movea.l d1, a2
267ab8  4e75           rts 
267aba  4fefffbc       lea.l -$44(a7), a7

; ==========================================================================
; TEMPO ENGINE $267ABA (full): integrators, $F23FA modulus, 2^23 phase wrap, tick dispatch  ($267ABA-$267D78)
; ==========================================================================
267aba  4fefffbc       lea.l -$44(a7), a7
267abe  48d77cfc       movem.l d2-d7/a2-a6, (a7)
267ac2  2039002bc2a4   move.l $2bc2a4.l, d0
267ac8  2f400040       move.l d0, $40(a7)
267acc  9dce           suba.l a6, a6
267ace  43f9010009a8   lea.l $10009a8.l, a1
267ad4  42af003c       clr.l $3c(a7)
267ad8  42af0038       clr.l $38(a7)
267adc  7230           moveq #$30, d1
267ade  2f410034       move.l d1, $34(a7)
267ae2  203c000005ec   move.l #$5ec, d0
267ae8  2f400030       move.l d0, $30(a7)
267aec  4a91           tst.l (a1)
267aee  66000246       bne.w $267d36
267af2  4287           clr.l d7
267af4  286f0030       movea.l $30(a7), a4
267af8  d9fc01000948   adda.l #$1000948, a4
267afe  246f0034       movea.l $34(a7), a2
267b02  d5fc002bfc16   adda.l #$2bfc16, a2
267b08  2a6f0038       movea.l $38(a7), a5
267b0c  dbfc0029c04c   adda.l #$29c04c, a5
267b12  4286           clr.l d6
267b14  266f003c       movea.l $3c(a7), a3
267b18  d7fc002bfc58   adda.l #$2bfc58, a3
267b1e  306c0008       movea.w $8(a4), a0
267b22  41f08c00       lea.l (a0, a0.l * 4), a0
267b26  2008           move.l a0, d0
267b28  6d0000f8       blt.w $267c22
267b2c  2a00           move.l d0, d5
267b2e  ee85           asr.l #$7, d5
267b30  7202           moveq #$2, d1
267b32  b285           cmp.l d5, d1
267b34  670000fe       beq.w $267c34
267b38  2047           movea.l d7, a0
267b3a  242c0014       move.l $14(a4), d2
267b3e  700a           moveq #$a, d0
267b40  e0a2           asr.l d0, d2
267b42  222f0040       move.l $40(a7), d1
267b46  4c012800       muls.l d1, d2
267b4a  202f0048       move.l $48(a7), d0
267b4e  4c002800       muls.l d0, d2
267b52  2615           move.l (a5), d3
267b54  20390029bd6c   move.l $29bd6c.l, d0
267b5a  5280           addq.l #$1, d0
267b5c  4c030800       muls.l d3, d0
267b60  2202           move.l d2, d1
267b62  0681000791fd   addi.l #$791fd, d1
267b68  2800           move.l d0, d4
267b6a  d881           add.l d1, d4
267b6c  223c000f23fa   move.l #$f23fa, d1
267b72  4c414804       divs.l d1, d4
267b76  d483           add.l d3, d2
267b78  2204           move.l d4, d1
267b7a  e589           lsl.l #$2, d1
267b7c  2004           move.l d4, d0
267b7e  ef88           lsl.l #$7, d0
267b80  9081           sub.l d1, d0
267b82  d084           add.l d4, d0
267b84  2200           move.l d0, d1
267b86  ed89           lsl.l #$6, d1
267b88  9280           sub.l d0, d1
267b8a  2001           move.l d1, d0
267b8c  ed88           lsl.l #$6, d0
267b8e  9081           sub.l d1, d0
267b90  d080           add.l d0, d0
267b92  9480           sub.l d0, d2
267b94  2a82           move.l d2, (a5)
267b96  302c0010       move.w $10(a4), d0
267b9a  48c0           ext.l d0
267b9c  2200           move.l d0, d1
267b9e  e789           lsl.l #$3, d1
267ba0  9280           sub.l d0, d1
267ba2  6b0000fe       bmi.w $267ca2
267ba6  ee81           asr.l #$7, d1
267ba8  2001           move.l d1, d0
267baa  5c80           addq.l #$6, d0
267bac  e1ac           lsl.l d0, d4
267bae  2004           move.l d4, d0
267bb0  23c40029d0ec   move.l d4, $29d0ec.l
267bb6  2544001e       move.l d4, $1e(a2)
267bba  4a88           tst.l a0
267bbc  67000104       beq.w $267cc2
267bc0  4a85           tst.l d5
267bc2  6f0000fe       ble.w $267cc2
267bc6  4293           clr.l (a3)
267bc8  2f00           move.l d0, -(a7)
267bca  2f08           move.l a0, -(a7)
267bcc  2f06           move.l d6, -(a7)
267bce  2f0e           move.l a6, -(a7)
267bd0  2f49003c       move.l a1, $3c(a7)
267bd4  4eb90024010c   jsr $24010c.l
267bda  4fef0010       lea.l $10(a7), a7
267bde  226f002c       movea.l $2c(a7), a1
267be2  4a87           tst.l d7
267be4  6716           beq.b $267bfc
267be6  256a00020006   move.l $2(a2), $6(a2)
267bec  203c791d0000   move.l #$791d0000, d0
267bf2  2540001a       move.l d0, $1a(a2)
267bf6  7201           moveq #$1, d1
267bf8  25410016       move.l d1, $16(a2)
267bfc  49ec0020       lea.l $20(a4), a4
267c00  45ea0022       lea.l $22(a2), a2
267c04  588d           addq.l #$4, a5
267c06  7002           moveq #$2, d0
267c08  b086           cmp.l d6, d0
267c0a  67000100       beq.w $267d0c
267c0e  5286           addq.l #$1, d6
267c10  47eb0022       lea.l $22(a3), a3
267c14  306c0008       movea.w $8(a4), a0
267c18  41f08c00       lea.l (a0, a0.l * 4), a0
267c1c  2008           move.l a0, d0
267c1e  6c00ff0c       bge.w $267b2c
267c22  06800000007f   addi.l #$7f, d0
267c28  2a00           move.l d0, d5
267c2a  ee85           asr.l #$7, d5
267c2c  7202           moveq #$2, d1
267c2e  b285           cmp.l d5, d1
267c30  6600ff06       bne.w $267b38
267c34  91c8           suba.l a0, a0
267c36  242c0014       move.l $14(a4), d2
267c3a  700a           moveq #$a, d0
267c3c  e0a2           asr.l d0, d2
267c3e  222f0040       move.l $40(a7), d1
267c42  4c012800       muls.l d1, d2
267c46  202f0048       move.l $48(a7), d0
267c4a  4c002800       muls.l d0, d2
267c4e  2615           move.l (a5), d3
267c50  20390029bd6c   move.l $29bd6c.l, d0
267c56  5280           addq.l #$1, d0
267c58  4c030800       muls.l d3, d0
267c5c  2202           move.l d2, d1
267c5e  0681000791fd   addi.l #$791fd, d1
267c64  2800           move.l d0, d4
267c66  d881           add.l d1, d4
267c68  223c000f23fa   move.l #$f23fa, d1
267c6e  4c414804       divs.l d1, d4
267c72  d483           add.l d3, d2
267c74  2204           move.l d4, d1
267c76  e589           lsl.l #$2, d1
267c78  2004           move.l d4, d0
267c7a  ef88           lsl.l #$7, d0
267c7c  9081           sub.l d1, d0
267c7e  d084           add.l d4, d0
267c80  2200           move.l d0, d1
267c82  ed89           lsl.l #$6, d1
267c84  9280           sub.l d0, d1
267c86  2001           move.l d1, d0
267c88  ed88           lsl.l #$6, d0
267c8a  9081           sub.l d1, d0
267c8c  d080           add.l d0, d0
267c8e  9480           sub.l d0, d2
267c90  2a82           move.l d2, (a5)
267c92  302c0010       move.w $10(a4), d0
267c96  48c0           ext.l d0
267c98  2200           move.l d0, d1
267c9a  e789           lsl.l #$3, d1
267c9c  9280           sub.l d0, d1
267c9e  6a00ff06       bpl.w $267ba6
267ca2  06810000007f   addi.l #$7f, d1
267ca8  ee81           asr.l #$7, d1
267caa  2001           move.l d1, d0
267cac  5c80           addq.l #$6, d0
267cae  e1ac           lsl.l d0, d4
267cb0  2004           move.l d4, d0
267cb2  23c40029d0ec   move.l d4, $29d0ec.l
267cb8  2544001e       move.l d4, $1e(a2)
267cbc  4a88           tst.l a0
267cbe  6600ff00       bne.w $267bc0
267cc2  2400           move.l d0, d2
267cc4  d493           add.l (a3), d2
267cc6  2682           move.l d2, (a3)
267cc8  7204           moveq #$4, d1
267cca  b285           cmp.l d5, d1
267ccc  671e           beq.b $267cec
267cce  0c82007fffff   cmpi.l #$7fffff, d2
267cd4  6f30           ble.b $267d06
267cd6  7203           moveq #$3, d1
267cd8  b285           cmp.l d5, d1
267cda  6724           beq.b $267d00
267cdc  0682ff800000   addi.l #$ff800000, d2
267ce2  2682           move.l d2, (a3)
267ce4  307c0001       movea.w #$1, a0
267ce8  6000fede       bra.w $267bc8
267cec  0c8200400000   cmpi.l #$400000, d2
267cf2  6fda           ble.b $267cce
267cf4  26bc00400000   move.l #$400000, (a3)
267cfa  91c8           suba.l a0, a0
267cfc  6000feca       bra.w $267bc8
267d00  26bc00800000   move.l #$800000, (a3)
267d06  91c8           suba.l a0, a0
267d08  6000febe       bra.w $267bc8
267d0c  528e           addq.l #$1, a6
267d0e  7260           moveq #$60, d1
267d10  d3af0030       add.l d1, $30(a7)
267d14  7066           moveq #$66, d0
267d16  d1af0034       add.l d0, $34(a7)
267d1a  123c000c       move.b #$c, d1
267d1e  d3af0038       add.l d1, $38(a7)
267d22  d1af003c       add.l d0, $3c(a7)
267d26  43e90020       lea.l $20(a1), a1
267d2a  103c0006       move.b #$6, d0
267d2e  b08e           cmp.l a6, d0
267d30  6600fdba       bne.w $267aec
267d34  6032           bra.b $267d68
267d36  4291           clr.l (a1)
267d38  7e01           moveq #$1, d7
267d3a  286f0030       movea.l $30(a7), a4
267d3e  d9fc01000948   adda.l #$1000948, a4
267d44  246f0034       movea.l $34(a7), a2
267d48  d5fc002bfc16   adda.l #$2bfc16, a2
267d4e  2a6f0038       movea.l $38(a7), a5
267d52  dbfc0029c04c   adda.l #$29c04c, a5
267d58  4286           clr.l d6
267d5a  266f003c       movea.l $3c(a7), a3
267d5e  d7fc002bfc58   adda.l #$2bfc58, a3
267d64  6000fdb8       bra.w $267b1e
267d68  42b90029bd6c   clr.l $29bd6c.l
267d6e  4cd77cfc       movem.l (a7), d2-d7/a2-a6
267d72  4fef0044       lea.l $44(a7), a7
267d76  4e75           rts 
267d78  4fefffec       lea.l -$14(a7), a7

; ==========================================================================
; PAGE ASSEMBLER $267DEE: glide 0.25/0.75, fade-in, per-track rebuild  ($267DEE-$267F82)
; ==========================================================================
267dee  4fefffbc       lea.l -$44(a7), a7
267df2  48d77cfc       movem.l d2-d7/a2-a6, (a7)
267df6  203c01000948   move.l #$1000948, d0
267dfc  2f400040       move.l d0, $40(a7)
267e00  4283           clr.l d3
267e02  4286           clr.l d6
267e04  4df901000f34   lea.l $1000f34.l, a6
267e0a  2a3c01001f40   move.l #$1001f40, d5
267e10  47f901000ad0   lea.l $1000ad0.l, a3
267e16  4bf901001962   lea.l $1001962.l, a5
267e1c  41f90029c014   lea.l $29c014.l, a0
267e22  2f480038       move.l a0, $38(a7)
267e26  43f90029bffc   lea.l $29bffc.l, a1
267e2c  2f490034       move.l a1, $34(a7)
267e30  41f9002bfc2e   lea.l $2bfc2e.l, a0
267e36  2f480030       move.l a0, $30(a7)
267e3a  49f901001174   lea.l $1001174.l, a4
267e40  2809           move.l a1, d4
267e42  43f90029c014   lea.l $29c014.l, a1
267e48  2f49002c       move.l a1, $2c(a7)
267e4c  2e3c010009a8   move.l #$10009a8, d7
267e52  7001           moveq #$1, d0
267e54  206f002c       movea.l $2c(a7), a0
267e58  5888           addq.l #$4, a0
267e5a  2f48002c       move.l a0, $2c(a7)
267e5e  5988           subq.l #$4, a0
267e60  20c0           move.l d0, (a0)+
267e62  2244           movea.l d4, a1
267e64  2f51003c       move.l (a1), $3c(a7)
267e68  4291           clr.l (a1)
267e6a  2f03           move.l d3, -(a7)
267e6c  4ebafc00       jsr $267a6e(pc)
267e70  2f03           move.l d3, -(a7)
267e72  4ebafc1e       jsr $267a92(pc)
267e76  224d           movea.l a5, a1
267e78  204c           movea.l a4, a0
267e7a  7008           moveq #$8, d0
267e7c  20d9           move.l (a1)+, (a0)+
267e7e  20d9           move.l (a1)+, (a0)+
267e80  5580           subq.l #$2, d0
267e82  66f8           bne.b $267e7c
267e84  2446           movea.l d6, a2
267e86  d5fc01001882   adda.l #$1001882, a2
267e8c  226f0048       movea.l $48(a7), a1
267e90  43e901bc       lea.l $1bc(a1), a1
267e94  7402           moveq #$2, d2
267e96  7208           moveq #$8, d1
267e98  201a           move.l (a2)+, d0
267e9a  2051           movea.l (a1), a0
267e9c  41f08a00       lea.l (a0, a0.l * 2), a0
267ea0  d088           add.l a0, d0
267ea2  e480           asr.l #$2, d0
267ea4  22c0           move.l d0, (a1)+
267ea6  5381           subq.l #$1, d1
267ea8  66ee           bne.b $267e98
267eaa  43e9ff30       lea.l -$d0(a1), a1
267eae  7218           moveq #$18, d1
267eb0  5382           subq.l #$1, d2
267eb2  66e4           bne.b $267e98
267eb4  2046           movea.l d6, a0
267eb6  d1fc01001902   adda.l #$1001902, a0
267ebc  7018           moveq #$18, d0
267ebe  224e           movea.l a6, a1
267ec0  22d8           move.l (a0)+, (a1)+
267ec2  22d8           move.l (a0)+, (a1)+
267ec4  22d8           move.l (a0)+, (a1)+
267ec6  22d8           move.l (a0)+, (a1)+
267ec8  5980           subq.l #$4, d0
267eca  66f4           bne.b $267ec0
267ecc  206bfffc       movea.l -$4(a3), a0
267ed0  41f08a00       lea.l (a0, a0.l * 2), a0
267ed4  2008           move.l a0, d0
267ed6  226f0038       movea.l $38(a7), a1
267eda  5889           addq.l #$4, a1
267edc  2f490038       move.l a1, $38(a7)
267ee0  5989           subq.l #$4, a1
267ee2  d099           add.l (a1)+, d0
267ee4  e480           asr.l #$2, d0
267ee6  2740fffc       move.l d0, -$4(a3)
267eea  2f03           move.l d3, -(a7)
267eec  4ebafb24       jsr $267a12(pc)
267ef0  2047           movea.l d7, a0
267ef2  2f10           move.l (a0), -(a7)
267ef4  068700000020   addi.l #$20, d7
267efa  2f03           move.l d3, -(a7)
267efc  4eb90023fe46   jsr $23fe46.l
267f02  4fef0014       lea.l $14(a7), a7
267f06  2245           movea.l d5, a1
267f08  4aa9f928       tst.l -$6d8(a1)
267f0c  6f08           ble.b $267f16
267f0e  42a9f928       clr.l -$6d8(a1)
267f12  42abffa0       clr.l -$60(a3)
267f16  2045           movea.l d5, a0
267f18  5885           addq.l #$4, d5
267f1a  2018           move.l (a0)+, d0
267f1c  4840           swap d0
267f1e  4240           clr.w d0
267f20  2053           movea.l (a3), a0
267f22  41f08a00       lea.l (a0, a0.l * 2), a0
267f26  d088           add.l a0, d0
267f28  e480           asr.l #$2, d0
267f2a  2680           move.l d0, (a3)
267f2c  202f003c       move.l $3c(a7), d0
267f30  226f0034       movea.l $34(a7), a1
267f34  5889           addq.l #$4, a1
267f36  2f490034       move.l a1, $34(a7)
267f3a  5989           subq.l #$4, a1
267f3c  22c0           move.l d0, (a1)+
267f3e  206f0038       movea.l $38(a7), a0
267f42  5888           addq.l #$4, a0
267f44  2f480038       move.l a0, $38(a7)
267f48  5988           subq.l #$4, a0
267f4a  4298           clr.l (a0)+
267f4c  5283           addq.l #$1, d3
267f4e  5884           addq.l #$4, d4
267f50  49ec0020       lea.l $20(a4), a4
267f54  068600000120   addi.l #$120, d6
267f5a  4bed0120       lea.l $120(a5), a5
267f5e  203c000000d0   move.l #$d0, d0
267f64  d1af0040       add.l d0, $40(a7)
267f68  d7c0           adda.l d0, a3
267f6a  4dee0060       lea.l $60(a6), a6
267f6e  103c0006       move.b #$6, d0
267f72  b083           cmp.l d3, d0
267f74  6600fedc       bne.w $267e52
267f78  4cd77cfc       movem.l (a7), d2-d7/a2-a6
267f7c  4fef0044       lea.l $44(a7), a7
267f80  4e75           rts 
