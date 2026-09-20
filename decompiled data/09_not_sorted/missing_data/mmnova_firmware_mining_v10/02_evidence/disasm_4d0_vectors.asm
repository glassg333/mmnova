; Блок +0x4D0 (уточнение v8/v9): ПЯТЬ пер-дорожечных байтовых векторов шагом 6
; (+0x4D0/+0x4D6/+0x4DC/+0x4E2/+0x4E8 + дорожка) -> рабочая запись $29B816..0x81B
; (слот 4 = константа 7); затем 16-байтовый пер-дорожечный блок параметров машин
; +0x4EE + t*16 -> $29B81C..0x82B (v8 'блок 2' начинается с +0x4EE, не +0x4E2).

; ---- vectors (0x2D6A0..0x2D734, section_0 offset) ----
02d6a0: ed88             lsl.l #$6, d0
02d6a2: 2803             move.l d3, d4
02d6a4: e98c             lsl.l #$4, d4
02d6a6: 4aaf0014         tst.l $14(a7)
02d6aa: 67000088         beq.w $2d734
02d6ae: 9081             sub.l d1, d0
02d6b0: 2200             move.l d0, d1
02d6b2: e989             lsl.l #$4, d1
02d6b4: d081             add.l d1, d0
02d6b6: 9082             sub.l d2, d0
02d6b8: e788             lsl.l #$3, d0
02d6ba: 2200             move.l d0, d1
02d6bc: d283             add.l d3, d1
02d6be: 41f900716fb4     lea.l $716fb4.l, a0
02d6c4: 14301800         move.b (a0, d1.l), d2
02d6c8: 13c20029b816     move.b d2, $29b816.l
02d6ce: 41f900716fba     lea.l $716fba.l, a0
02d6d4: 14301800         move.b (a0, d1.l), d2
02d6d8: 13c20029b817     move.b d2, $29b817.l
02d6de: 41f900716fc0     lea.l $716fc0.l, a0
02d6e4: 14301800         move.b (a0, d1.l), d2
02d6e8: 13c20029b818     move.b d2, $29b818.l
02d6ee: 41f900716fc6     lea.l $716fc6.l, a0
02d6f4: 14301800         move.b (a0, d1.l), d2
02d6f8: 13c20029b819     move.b d2, $29b819.l
02d6fe: 7407             moveq #$7, d2
02d700: 13c20029b81a     move.b d2, $29b81a.l
02d706: 41f900716fcc     lea.l $716fcc.l, a0
02d70c: 12301800         move.b (a0, d1.l), d1
02d710: 13c10029b81b     move.b d1, $29b81b.l
02d716: d084             add.l d4, d0
02d718: 2240             movea.l d0, a1
02d71a: d3fc00716fd2     adda.l #$716fd2, a1
02d720: 41f90029b81c     lea.l $29b81c.l, a0
02d726: 10d9             move.b (a1)+, (a0)+
02d728: b1fc0029b82c     cmpa.l #$29b82c, a0
02d72e: 6700008e         beq.w $2d7be
02d732: 60f2             bra.b $2d726
