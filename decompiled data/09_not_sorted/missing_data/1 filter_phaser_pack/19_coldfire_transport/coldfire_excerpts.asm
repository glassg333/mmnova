; ==== HI08 52-word senders: $24390C = DSP1 ($500001/$500004), $243944 = DSP2 ($600001/$600004)  ($ 24390C - $ 24397B) ====
24390c  226f0008       movea.l $8(a7), a1
243910  202f0004       move.l $4(a7), d0
243914  5080           addq.l #$8, d0
243916  13c000500001   move.b d0, $500001.l
24391c  7280           moveq #$80, d1
24391e  8081           or.l d1, d0
243920  13c000500001   move.b d0, $500001.l
243926  103900500001   move.b $500001.l, d0
24392c  6df8           blt.b $243926
24392e  207c00500004   movea.l #$500004, a0
243934  7033           moveq #$33, d0
243936  2099           move.l (a1)+, (a0)
243938  2099           move.l (a1)+, (a0)
24393a  2099           move.l (a1)+, (a0)
24393c  2099           move.l (a1)+, (a0)
24393e  5980           subq.l #$4, d0
243940  6af4           bpl.b $243936
243942  4e75           rts 
243944  226f0008       movea.l $8(a7), a1
243948  202f0004       move.l $4(a7), d0
24394c  5080           addq.l #$8, d0
24394e  13c000600001   move.b d0, $600001.l
243954  7280           moveq #$80, d1
243956  8081           or.l d1, d0
243958  13c000600001   move.b d0, $600001.l
24395e  103900600001   move.b $600001.l, d0
243964  6df8           blt.b $24395e
243966  207c00600004   movea.l #$600004, a0
24396c  7033           moveq #$33, d0
24396e  2099           move.l (a1)+, (a0)
243970  2099           move.l (a1)+, (a0)
243972  2099           move.l (a1)+, (a0)
243974  2099           move.l (a1)+, (a0)
243976  5980           subq.l #$4, d0
243978  6af4           bpl.b $24396e
24397a  4e75           rts 

; ==== Boot/bulk block writer $2439D6: cmd $150000|(arg<<11) + 1024 words to BOTH DSPs  ($ 2439D6 - $ 243A68) ====
2439d6  2f0a           move.l a2, -(a7)
2439d8  2f02           move.l d2, -(a7)
2439da  202f000c       move.l $c(a7), d0
2439de  242f0010       move.l $10(a7), d2
2439e2  7201           moveq #$1, d1
2439e4  23c10029bfb8   move.l d1, $29bfb8.l
2439ea  123c000b       move.b #$b, d1
2439ee  e3a8           lsl.l d1, d0
2439f0  2200           move.l d0, d1
2439f2  068100150000   addi.l #$150000, d1
2439f8  23c100500004   move.l d1, $500004.l
2439fe  700b           moveq #$b, d0
243a00  13c000500001   move.b d0, $500001.l
243a06  708b           moveq #$8b, d0
243a08  13c000500001   move.b d0, $500001.l
243a0e  103900500001   move.b $500001.l, d0
243a14  6df8           blt.b $243a0e
243a16  23c100600004   move.l d1, $600004.l
243a1c  720b           moveq #$b, d1
243a1e  13c100600001   move.b d1, $600001.l
243a24  708b           moveq #$8b, d0
243a26  13c000600001   move.b d0, $600001.l
243a2c  103900600001   move.b $600001.l, d0
243a32  6df8           blt.b $243a2c
243a34  207c00600004   movea.l #$600004, a0
243a3a  247c00500004   movea.l #$500004, a2
243a40  203c000003ff   move.l #$3ff, d0
243a46  2242           movea.l d2, a1
243a48  5389           subq.l #$1, a1
243a4a  2211           move.l (a1), d1
243a4c  2081           move.l d1, (a0)
243a4e  2481           move.l d1, (a2)
243a50  5689           addq.l #$3, a1
243a52  2211           move.l (a1), d1
243a54  2081           move.l d1, (a0)
243a56  2481           move.l d1, (a2)
243a58  5689           addq.l #$3, a1
243a5a  5380           subq.l #$1, d0
243a5c  6aec           bpl.b $243a4a
243a5e  42b90029bfb8   clr.l $29bfb8.l
243a64  241f           move.l (a7)+, d2
243a66  245f           movea.l (a7)+, a2
243a68  4e75           rts 

; ==== DSP word-read service via CVR $06/$86 + host-command state machine (DSP2 buffers)  ($ 24828E - $ 248300) ====
24828e  7206           moveq #$6, d1
248290  13c100500001   move.b d1, $500001.l
248296  7086           moveq #$86, d0
248298  13c000500001   move.b d0, $500001.l
24829e  103900500001   move.b $500001.l, d0
2482a4  6df8           blt.b $24829e
2482a6  203900259114   move.l $259114.l, d0
2482ac  5280           addq.l #$1, d0
2482ae  23c000500004   move.l d0, $500004.l
2482b4  203900700000   move.l $700000.l, d0
2482ba  103900500002   move.b $500002.l, d0
2482c0  08000000       btst.b #$0, d0
2482c4  67f4           beq.b $2482ba
2482c6  203900500004   move.l $500004.l, d0
2482cc  23c001000af0   move.l d0, $1000af0.l
2482d2  203900700000   move.l $700000.l, d0
2482d8  20390029c008   move.l $29c008.l, d0
2482de  660003e6       bne.w $2486c6
2482e2  20390029c004   move.l $29c004.l, d0
2482e8  6700fb2e       beq.w $247e18
2482ec  487901000bf4   pea.l $1000bf4.l
2482f2  48780002       pea.l $2.w
2482f6  4ebab64c       jsr $243944(pc)
2482fa  42b901000c94   clr.l $1000c94.l
248300  42b901000c74   clr.l $1000c74.l

; ==== DSP2 sender: buffer $1000A54 -> CVR $08|$80 (Y:$0500)  ($ 24837E - $ 2483B0) ====
24837e  487901000a54   pea.l $1000a54.l
248384  42a7           clr.l -(a7)
248386  4ebab5bc       jsr $243944(pc)
24838a  42b901000af4   clr.l $1000af4.l
248390  42b901000ad4   clr.l $1000ad4.l
248396  42b901000ad8   clr.l $1000ad8.l
24839c  42b90029bffc   clr.l $29bffc.l
2483a2  508f           addq.l #$8, a7
2483a4  6000fa72       bra.w $247e18
2483a8  41f90026dd34   lea.l $26dd34.l, a0
2483ae  4ab06800       tst.l (a0, d6.l)

; ==== DSP2 sender: buffer $1000B24 -> CVR $09|$80 (Y:$0600)  ($ 248262 - $ 248276) ====
248262  487901000b24   pea.l $1000b24.l
248268  48780001       pea.l $1.w
24826c  4ebab6d6       jsr $243944(pc)
248270  42b901000bc4   clr.l $1000bc4.l
248276  42b901000ba4   clr.l $1000ba4.l

; ==== DSP2 sender: buffer $1000BF4 -> CVR $0A|$80 (Y:$0700)  ($ 2482EC - $ 248300) ====
2482ec  487901000bf4   pea.l $1000bf4.l
2482f2  48780002       pea.l $2.w
2482f6  4ebab64c       jsr $243944(pc)
2482fa  42b901000c94   clr.l $1000c94.l
248300  42b901000c74   clr.l $1000c74.l

; ==== DSP1 sender: buffer $1000CC4 -> CVR $08|$80 (Y:$0500)  ($ 2486C6 - $ 2486EC) ====
2486c6  487901000cc4   pea.l $1000cc4.l
2486cc  42a7           clr.l -(a7)
2486ce  4ebab23c       jsr $24390c(pc)
2486d2  42b901000d64   clr.l $1000d64.l
2486d8  42b901000d44   clr.l $1000d44.l
2486de  42b901000d48   clr.l $1000d48.l
2486e4  42b90029c008   clr.l $29c008.l
2486ea  508f           addq.l #$8, a7
2486ec  6044           bra.b $248732

; ==== DSP1 sender: buffer $1000D94 -> CVR $09|$80 (Y:$0600)  ($ 24855E - $ 248584) ====
24855e  487901000d94   pea.l $1000d94.l
248564  48780001       pea.l $1.w
248568  4ebab3a2       jsr $24390c(pc)
24856c  42b901000e34   clr.l $1000e34.l
248572  42b901000e14   clr.l $1000e14.l
248578  42b901000e18   clr.l $1000e18.l
24857e  42b90029c00c   clr.l $29c00c.l
248584  508f           addq.l #$8, a7

; ==== DSP1 sender: buffer $1000E64 -> CVR $0A|$80 (Y:$0700)  ($ 2484EC - $ 248512) ====
2484ec  487901000e64   pea.l $1000e64.l
2484f2  48780002       pea.l $2.w
2484f6  4ebab414       jsr $24390c(pc)
2484fa  42b901000f04   clr.l $1000f04.l
248500  42b901000ee4   clr.l $1000ee4.l
248506  42b901000ee8   clr.l $1000ee8.l
24850c  42b90029c010   clr.l $29c010.l
248512  508f           addq.l #$8, a7

; ==== PER-FRAME 6-track update loop $267DEE (0.75/0.25 pointer glide, staging fill calls)  ($ 267DEE - $ 267F80) ====
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
