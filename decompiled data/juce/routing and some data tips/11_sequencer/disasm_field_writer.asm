; Запись байтов в векторы машин (+0x43A/+0x440/+0x446)+track
; OS 0x02f3dc..0x02f45c (section_0_MAIN_OS.bin)
; База слота*6520 + a1(дорожка); ограничения: <=3 (+0x440), <=11 (+0x446) = ID машины.

  02f3dc: 4feffff0     lea.l -$10(a7), a7
  02f3e0: 48d7003c     movem.l d2-d5, (a7)
  02f3e4: 226f0014     movea.l $14(a7), a1
  02f3e8: 262f001c     move.l $1c(a7), d3
  02f3ec: 282f0020     move.l $20(a7), d4
  02f3f0: 1a04         move.b d4, d5
  02f3f2: 2439002bda2e move.l $2bda2e.l, d2
  02f3f8: 2202         move.l d2, d1
  02f3fa: e989         lsl.l #$4, d1
  02f3fc: 2002         move.l d2, d0
  02f3fe: ed88         lsl.l #$6, d0
  02f400: 9081         sub.l d1, d0
  02f402: 2200         move.l d0, d1
  02f404: e989         lsl.l #$4, d1
  02f406: d081         add.l d1, d0
  02f408: 9082         sub.l d2, d0
  02f40a: e788         lsl.l #$3, d0
  02f40c: 2200         move.l d0, d1
  02f40e: d289         add.l a1, d1
  02f410: 41f900716f1e lea.l $716f1e.l, a0
  02f416: 102f001b     move.b $1b(a7), d0
  02f41a: 11801800     move.b d0, (a0, d1.l)
  02f41e: 4280         clr.l d0
  02f420: 1003         move.b d3, d0
  02f422: 7403         moveq #$3, d2
  02f424: b480         cmp.l d0, d2
  02f426: 650a         bcs.b $2f432
  02f428: 41f900716f24 lea.l $716f24.l, a0
  02f42e: 11831800     move.b d3, (a0, d1.l)
  02f432: 4280         clr.l d0
  02f434: 1005         move.b d5, d0
  02f436: 740b         moveq #$b, d2
  02f438: b480         cmp.l d0, d2
  02f43a: 650a         bcs.b $2f446
  02f43c: 41f900716f2a lea.l $716f2a.l, a0
  02f442: 11841800     move.b d4, (a0, d1.l)
  02f446: 4ab9002bff12 tst.l $2bff12.l
  02f44c: 6714         beq.b $2f462
  02f44e: 4282         clr.l d2
  02f450: 2f02         move.l d2, -(a7)
  02f452: 4ebaff5a     jsr $2f3ae(pc)
  02f456: 5282         addq.l #$1, d2
  02f458: 588f         addq.l #$4, a7
  02f45a: 7006         moveq #$6, d0
