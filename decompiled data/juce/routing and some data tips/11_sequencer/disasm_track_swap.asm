; Подпрограмма обмена дорожками (track swap/copy)
; OS 0x033b04..0x033c02 (section_0_MAIN_OS.bin)
; Копирует между дорожками d2->d3: 8Б @+0x1B0+t*8, 8Б @+0x1E0+t*8, 64Б нот @+0x2A4+t*64, байты @+0x428/+0x42E/+0x434/+0x44C/+0x452/+0x458/+0x45E/+0x464/+0x46A (t-относительные), 16Б параметров @+0x470+t*16.

  033b04: 23440150     move.l d4, $150(a1)
  033b08: 23450154     move.l d5, $154(a1)
  033b0c: 4aaf002c     tst.l $2c(a7)
  033b10: 6722         beq.b $33b34
  033b12: 068000000274 addi.l #$274, d0
  033b18: 41f20800     lea.l (a2, d0.l), a0
  033b1c: 2001         move.l d1, d0
  033b1e: 068000000274 addi.l #$274, d0
  033b24: 43f30800     lea.l (a3, d0.l), a1
  033b28: 4280         clr.l d0
  033b2a: 12d8         move.b (a0)+, (a1)+
  033b2c: 5280         addq.l #$1, d0
  033b2e: 7a07         moveq #$7, d5
  033b30: ba80         cmp.l d0, d5
  033b32: 66f6         bne.b $33b2a
  033b34: 2003         move.l d3, d0
  033b36: e788         lsl.l #$3, d0
  033b38: 43f30800     lea.l (a3, d0.l), a1
  033b3c: 2002         move.l d2, d0
  033b3e: e788         lsl.l #$3, d0
  033b40: 41f20800     lea.l (a2, d0.l), a0
  033b44: 202801b0     move.l $1b0(a0), d0
  033b48: 222801b4     move.l $1b4(a0), d1
  033b4c: 234001b0     move.l d0, $1b0(a1)
  033b50: 234101b4     move.l d1, $1b4(a1)
  033b54: 282801e0     move.l $1e0(a0), d4
  033b58: 2a2801e4     move.l $1e4(a0), d5
  033b5c: 234401e0     move.l d4, $1e0(a1)
  033b60: 234501e4     move.l d5, $1e4(a1)
  033b64: 2002         move.l d2, d0
  033b66: ed88         lsl.l #$6, d0
  033b68: 0680000002a4 addi.l #$2a4, d0
  033b6e: 43f20800     lea.l (a2, d0.l), a1
  033b72: 2003         move.l d3, d0
  033b74: ed88         lsl.l #$6, d0
  033b76: 0680000002a4 addi.l #$2a4, d0
  033b7c: 41f30800     lea.l (a3, d0.l), a0
  033b80: 4280         clr.l d0
  033b82: 10d9         move.b (a1)+, (a0)+
  033b84: 5280         addq.l #$1, d0
  033b86: 7a40         moveq #$40, d5
  033b88: ba80         cmp.l d0, d5
  033b8a: 66f6         bne.b $33b82
  033b8c: 43f33800     lea.l (a3, d3.l), a1
  033b90: 41f22800     lea.l (a2, d2.l), a0
  033b94: 136804280428 move.b $428(a0), $428(a1)
  033b9a: 1368042e042e move.b $42e(a0), $42e(a1)
  033ba0: 136804340434 move.b $434(a0), $434(a1)
  033ba6: 1368044c044c move.b $44c(a0), $44c(a1)
  033bac: 136804520452 move.b $452(a0), $452(a1)
  033bb2: 136804580458 move.b $458(a0), $458(a1)
  033bb8: 1368045e045e move.b $45e(a0), $45e(a1)
  033bbe: 136804640464 move.b $464(a0), $464(a1)
  033bc4: 1368046a046a move.b $46a(a0), $46a(a1)
  033bca: e98a         lsl.l #$4, d2
  033bcc: 2002         move.l d2, d0
  033bce: 068000000470 addi.l #$470, d0
  033bd4: 41f20800     lea.l (a2, d0.l), a0
  033bd8: 2003         move.l d3, d0
  033bda: e988         lsl.l #$4, d0
  033bdc: 068000000470 addi.l #$470, d0
  033be2: 43f30800     lea.l (a3, d0.l), a1
  033be6: 4280         clr.l d0
  033be8: 12d8         move.b (a0)+, (a1)+
  033bea: 5280         addq.l #$1, d0
  033bec: 7210         moveq #$10, d1
  033bee: b280         cmp.l d0, d1
  033bf0: 66f6         bne.b $33be8
  033bf2: 2f03         move.l d3, -(a7)
  033bf4: 4ebaf300     jsr $32ef6(pc)
  033bf8: 588f         addq.l #$4, a7
  033bfa: 4cd70c3c     movem.l (a7), d2-d5/a2-a3
  033bfe: 4fef0018     lea.l $18(a7), a7
