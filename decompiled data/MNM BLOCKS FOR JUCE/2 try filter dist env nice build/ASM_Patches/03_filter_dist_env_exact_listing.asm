; ФИЛЬТР + ДИСТОРШН + ОГИБАЮЩАЯ  (Monomachine SFX-60 OS 1.32B)

; 1) ФИЛЬТР — 2-полюсный резонатор, ЛИТЕРАЛ из 01_GND-SIN_full.txt
144d29: move  #>$1e454e,x1      ; C1 = 0.23649215
144d2b: move  #>$5b75b8,y0      ; C2 = 0.71452600
144d2d: move  y:(r6+$1c),y1     ; s1
144d2e: move  y:(r6+$1d),b      ; s2
144d33: do    #<$10,>$144d39    ; 16-сэмпловый блок
144d35: mac   y0,x0,a   x:(r1)+,x0  y1,y:(r4)+   ; a += C2*x
144d36: mac  -y1,y0,a   x:(r0)+,b   b,y1         ; a -= s1*C2
144d37: mac   x1,x0,b   x:(r1)+,x0  y1,y:(r4)+   ; b += C1*x
144d38: mac  -y1,x1,b   x:(r0)+,a   a,y1         ; b -= s1*C1
; Глобальный Base/Width SVF ядра: P:$05D3-$05FA, коэфф. $F528BD(-0.08476) / $4A4DF0(+0.58044)

; 2) ДИСТОРШН — SR.SM насыщение, ЛИТЕРАЛ из chorus_disasm.txt (машины дисторшна нет)
1476a5: bset  #$14,sr           ; ВКЛ. SR.SM -> hard clip +-1.0 (Q23)
1476a6: do    #<$10,>$1476aa
1476a8: mac  -x1,x0,b   b,x:(r0)+  b,y0
1476a9: mac   y0,x0,a   a,x1       a,y:(r4)+
1476aa: bclr  #$14,sr           ; ВЫКЛ.
; Цепочка (README_architecture_routing): Machine -> Distortion & SRR -> Filter
; SRR/bitcrush = FX-EXT, дробный просмотр (mpysu+dmac), питч-табл. $101AFB

; 3) ОГИБАЮЩАЯ AMP — func_0004A8 (P:$04A8-$04F5), KERNEL_VOICE_CHAIN.md §4.1
; state 1 ATTACK : y:$4FF += TBL[ y:$141800 + (V-$10 ATK)>>16 ]
; state 4 DECAY  : с (V-$0E DEC)^2 и TBL[ $141880 + idx ], сравнение
; state 5 SUSTAIN: y:$4FF = (V-$0E)^2
; state 2 RELEASE: y:$4FF -= TBL[ $141880 + (V-$0D REL)>>16 ]   (P:$4E5-$4EE)
; default        : y:$4FF = $7FFFFF
; trigger (P:$4FF): V-$08==1 -> сброс LFO
; LFO (P:$0506)  : state1 множитель $791FD0 = 0.9462824 за шаг
; 1.6.0: прогиб кривой Decay расширен до 150% (evaluateDecayCurve150 / shapeCurve150)
