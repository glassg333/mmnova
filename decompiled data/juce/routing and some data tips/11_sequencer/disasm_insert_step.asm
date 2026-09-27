; Вставка/сдвиг шага: строки +0x557 с шагом 64 (lsl #6), циклы 8x7
; OS 0x0164fe..0x0165ae (section_0_MAIN_OS.bin)
; Строки пер-шаговых значений: 56 строк (8x7), stride 64 от +0x557.

  0164fe: 1012         move.b (a2), d0
  016500: 49c0         extb.l d0
  016502: 0700         btst.l d3, d0
  016504: 677a         beq.b $16580
  016506: 7007         moveq #$7, d0
  016508: b089         cmp.l a1, d0
  01650a: 6672         bne.b $1657e
  01650c: 2004         move.l d4, d0
  01650e: ed88         lsl.l #$6, d0
  016510: 4aaf00a8     tst.l $a8(a7)
  016514: 6f2c         ble.b $16542
  016516: 206f0034     movea.l $34(a7), a0
  01651a: d1c0         adda.l d0, a0
  01651c: d1ce         adda.l a6, a0
  01651e: 14280556     move.b $556(a0), d2
  016522: 068000000556 addi.l #$556, d0
  016528: 266f0034     movea.l $34(a7), a3
  01652c: 41f30801     lea.l $1(a3, d0.l), a0
  016530: 4281         clr.l d1
  016532: 6008         bra.b $1653c
  016534: 1010         move.b (a0), d0
  016536: 10c2         move.b d2, (a0)+
  016538: 5281         addq.l #$1, d1
  01653a: 1400         move.b d0, d2
  01653c: bdc1         cmpa.l d1, a6
  01653e: 6ef4         bgt.b $16534
  016540: 603c         bra.b $1657e
  016542: 206f0034     movea.l $34(a7), a0
  016546: d1c0         adda.l d0, a0
  016548: 14280557     move.b $557(a0), d2
  01654c: 068000000557 addi.l #$557, d0
  016552: 266f0034     movea.l $34(a7), a3
  016556: 41f30801     lea.l $1(a3, d0.l), a0
  01655a: 4280         clr.l d0
  01655c: 6008         bra.b $16566
  01655e: 5280         addq.l #$1, d0
  016560: 1150ffff     move.b (a0), -$1(a0)
  016564: 5288         addq.l #$1, a0
  016566: 220e         move.l a6, d1
  016568: 5381         subq.l #$1, d1
  01656a: b280         cmp.l d0, d1
  01656c: 6ef0         bgt.b $1655e
  01656e: 2004         move.l d4, d0
  016570: ed88         lsl.l #$6, d0
  016572: d0af0034     add.l $34(a7), d0
  016576: 2040         movea.l d0, a0
  016578: d1c1         adda.l d1, a0
  01657a: 11420557     move.b d2, $557(a0)
  01657e: 5284         addq.l #$1, d4
  016580: 5283         addq.l #$1, d3
  016582: 7008         moveq #$8, d0
  016584: b083         cmp.l d3, d0
  016586: 6600ff76     bne.w $164fe
  01658a: 5289         addq.l #$1, a1
  01658c: 528a         addq.l #$1, a2
  01658e: b089         cmp.l a1, d0
  016590: 6620         bne.b $165b2
  016592: 6024         bra.b $165b8
  016594: 1410         move.b (a0), d2
  016596: 49c2         extb.l d2
  016598: 4280         clr.l d0
  01659a: 6000ff2a     bra.w $164c6
  01659e: 202f0030     move.l $30(a7), d0
  0165a2: e788         lsl.l #$3, d0
  0165a4: 068000000274 addi.l #$274, d0
  0165aa: 246f0034     movea.l $34(a7), a2
