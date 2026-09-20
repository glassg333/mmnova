; Плеер, ч.3: цикл по строкам локов: jsr $224800 = число активных строк (+0x556); маска строки = банк+0x274+строка*8 (btst битов), значение = банк+0x557+строка*64+шаг; выход 0xFF при сброшенном бите; запись в $29B516+d6 (d6+=8 на группу).
; OS 0x34F9A..0x35190 (section_0_MAIN_OS.bin)

034f9a: 4fefffd8       lea.l -$28(a7), a7
034f9e: 48d73cfc       movem.l d2-d7/a2-a5, (a7)
034fa2: 2a6f002c       movea.l $2c(a7), a5
034fa6: 99cc           suba.l a4, a4
034fa8: 4287           clr.l d7
034faa: 601a           bra.b $34fc6
034fac: 0102           btst.l d0, d2
034fae: 6702           beq.b $34fb2
034fb0: 5287           addq.l #$1, d7
034fb2: 5280           addq.l #$1, d0
034fb4: 7608           moveq #$8, d3
034fb6: b680           cmp.l d0, d3
034fb8: 66f2           bne.b $34fac
034fba: 5281           addq.l #$1, d1
034fbc: 5288           addq.l #$1, a0
034fbe: b681           cmp.l d1, d3
034fc0: 66000162       bne.w $35124
034fc4: 528c           addq.l #$1, a4
034fc6: 4eb900224800   jsr $224800.l
034fcc: b08c           cmp.l a4, d0
034fce: 6f00015e       ble.w $3512e
034fd2: 243900716ae0   move.l $716ae0.l, d2
034fd8: 2202           move.l d2, d1
034fda: e989           lsl.l #$4, d1
034fdc: 2002           move.l d2, d0
034fde: ed88           lsl.l #$6, d0
034fe0: 9081           sub.l d1, d0
034fe2: 2200           move.l d0, d1
034fe4: e989           lsl.l #$4, d1
034fe6: d081           add.l d1, d0
034fe8: 9082           sub.l d2, d0
034fea: d08c           add.l a4, d0
034fec: e788           lsl.l #$3, d0
034fee: 2040           movea.l d0, a0
034ff0: d1fc00716d58   adda.l #$716d58, a0
034ff6: 4281           clr.l d1
034ff8: 6000012a       bra.w $35124
034ffc: 263900716ae0   move.l $716ae0.l, d3
035002: 240c           move.l a4, d2
035004: e78a           lsl.l #$3, d2
035006: 2203           move.l d3, d1
035008: e989           lsl.l #$4, d1
03500a: 2003           move.l d3, d0
03500c: ed88           lsl.l #$6, d0
03500e: 9081           sub.l d1, d0
035010: 2200           move.l d0, d1
035012: e989           lsl.l #$4, d1
035014: d280           add.l d0, d1
035016: 9283           sub.l d3, d1
035018: e789           lsl.l #$3, d1
03501a: d481           add.l d1, d2
03501c: d485           add.l d5, d2
03501e: 41f900716d58   lea.l $716d58.l, a0
035024: 10302800       move.b (a0, d2.l), d0
035028: 49c0           extb.l d0
03502a: 0900           btst.l d4, d0
03502c: 674c           beq.b $3507a
03502e: 2007           move.l d7, d0
035030: ed88           lsl.l #$6, d0
035032: d081           add.l d1, d0
035034: d08d           add.l a5, d0
035036: 41f90071703a   lea.l $71703a.l, a0
03503c: 10300801       move.b $1(a0, d0.l), d0
035040: 49c0           extb.l d0
035042: 1480           move.b d0, (a2)
035044: 72ff           moveq #$ff, d1
035046: b280           cmp.l d0, d1
035048: 672c           beq.b $35076
03504a: 2f0d           move.l a5, -(a7)
03504c: 2f04           move.l d4, -(a7)
03504e: 2f05           move.l d5, -(a7)
035050: 2f0c           move.l a4, -(a7)
035052: 2f03           move.l d3, -(a7)
035054: 4ebabf28       jsr $30f7e(pc)
035058: 4fef0014       lea.l $14(a7), a7
03505c: 4a80           tst.l d0
03505e: 6702           beq.b $35062
035060: 5287           addq.l #$1, d7
035062: 4878ffff       pea.l $ffff.w
035066: 2f04           move.l d4, -(a7)
035068: 2f05           move.l d5, -(a7)
03506a: 4eb9002159cc   jsr $2159cc.l
035070: 4fef000c       lea.l $c(a7), a7
035074: 6008           bra.b $3507e
035076: 5287           addq.l #$1, d7
035078: 6004           bra.b $3507e
03507a: 16bcffff       move.b #$ff, (a3)
03507e: 5284           addq.l #$1, d4
035080: 528b           addq.l #$1, a3
035082: 528a           addq.l #$1, a2
035084: 7608           moveq #$8, d3
035086: b684           cmp.l d4, d3
035088: 6600ff72       bne.w $34ffc
03508c: 5285           addq.l #$1, d5
03508e: 5086           addq.l #$8, d6
035090: 7007           moveq #$7, d0
035092: b085           cmp.l d5, d0
035094: 6600009c       bne.w $35132
035098: 4284           clr.l d4
03509a: 2f04           move.l d4, -(a7)
03509c: 2f0d           move.l a5, -(a7)
03509e: 2f0c           move.l a4, -(a7)
0350a0: 4ebab86a       jsr $3090c(pc)
0350a4: 2040           movea.l d0, a0
0350a6: 4fef000c       lea.l $c(a7), a7
0350aa: 72ff           moveq #$ff, d1
0350ac: b280           cmp.l d0, d1
0350ae: 67000092       beq.w $35142
0350b2: 2639002bda2e   move.l $2bda2e.l, d3
0350b8: 70fe           moveq #$fe, d0
0350ba: b088           cmp.l a0, d0
0350bc: 662a           bne.b $350e8
0350be: 240c           move.l a4, d2
0350c0: ed8a           lsl.l #$6, d2
0350c2: 2203           move.l d3, d1
0350c4: e989           lsl.l #$4, d1
0350c6: 2003           move.l d3, d0
0350c8: ed88           lsl.l #$6, d0
0350ca: 9081           sub.l d1, d0
0350cc: 2200           move.l d0, d1
0350ce: e989           lsl.l #$4, d1
0350d0: d081           add.l d1, d0
0350d2: 9083           sub.l d3, d0
0350d4: e788           lsl.l #$3, d0
0350d6: d480           add.l d0, d2
0350d8: d48d           add.l a5, d2
0350da: 41f900716d88   lea.l $716d88.l, a0
0350e0: 10302800       move.b (a0, d2.l), d0
0350e4: 49c0           extb.l d0
0350e6: 602c           bra.b $35114
0350e8: 2203           move.l d3, d1
0350ea: e989           lsl.l #$4, d1
0350ec: 2003           move.l d3, d0
0350ee: ed88           lsl.l #$6, d0
0350f0: 9081           sub.l d1, d0
0350f2: 2200           move.l d0, d1
0350f4: e989           lsl.l #$4, d1
0350f6: d081           add.l d1, d0
0350f8: 9083           sub.l d3, d0
0350fa: 41f00c00       lea.l (a0, d0.l * 4), a0
0350fe: 2008           move.l a0, d0
035100: d080           add.l d0, d0
035102: 2040           movea.l d0, a0
035104: d1fc00716ae0   adda.l #$716ae0, a0
03510a: 4280           clr.l d0
03510c: 302817fc       move.w $17fc(a0), d0
035110: 7209           moveq #$9, d1
035112: e2a8           lsr.l d1, d0
035114: 41f90029b516   lea.l $29b516.l, a0
03511a: 11804840       move.b d0, $40(a0, d4.l)
03511e: 5284           addq.l #$1, d4
035120: 6000ff78       bra.w $3509a
035124: 1410           move.b (a0), d2
035126: 49c2           extb.l d2
035128: 4280           clr.l d0
03512a: 6000fe80       bra.w $34fac
03512e: 4285           clr.l d5
035130: 4286           clr.l d6
035132: 2446           movea.l d6, a2
035134: d5fc0029b516   adda.l #$29b516, a2
03513a: 264a           movea.l a2, a3
03513c: 4284           clr.l d4
03513e: 6000febc       bra.w $34ffc
035142: 41f90029b516   lea.l $29b516.l, a0
035148: 50c3           st.b d3
03514a: 11834840       move.b d3, $40(a0, d4.l)
03514e: 4878004f       pea.l $4f.w
035152: 2f0d           move.l a5, -(a7)
035154: 2f0c           move.l a4, -(a7)
035156: 4eba9852       jsr $2e9aa(pc)
03515a: 4fef000c       lea.l $c(a7), a7
03515e: 23c00029b5ba   move.l d0, $29b5ba.l
035164: 7001           moveq #$1, d0
035166: 23c00028b0c8   move.l d0, $28b0c8.l
03516c: 2f0d           move.l a5, -(a7)
03516e: 2f0c           move.l a4, -(a7)
035170: 2f3900716ae0   move.l $716ae0.l, -(a7)
035176: 4eb900215a40   jsr $215a40.l
03517c: 4fef000c       lea.l $c(a7), a7
035180: 4cd73cfc       movem.l (a7), d2-d7/a2-a5
035184: 4fef0028       lea.l $28(a7), a7
035188: 4e75           rts 
03518a: 4fefffd8       lea.l -$28(a7), a7
03518e: 48d7           movem.l d1/d3/d5/d7/a1/a3/a5/a7, (a7)
