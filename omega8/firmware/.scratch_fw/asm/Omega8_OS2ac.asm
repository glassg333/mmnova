
/home/user/.scratch_fw/images/Omega8_OS2ac.decoded.bin:     file format binary


Disassembly of section .data:

00008000 <.data>:
    8000:	8e 01 ff    	lds	#0x1ff
    8003:	86 1e       	ldaa	#0x1e
    8005:	b7 10 08    	staa	0x1008
    8008:	4f          	clra
    8009:	b7 10 00    	staa	0x1000
    800c:	86 88       	ldaa	#0x88
    800e:	b7 10 26    	staa	0x1026
    8011:	86 20       	ldaa	#0x20
    8013:	b7 10 2b    	staa	0x102b
    8016:	86 2c       	ldaa	#0x2c
    8018:	b7 10 2d    	staa	0x102d
    801b:	86 93       	ldaa	#0x93
    801d:	b7 10 39    	staa	0x1039
    8020:	86 ff       	ldaa	#0xff
    8022:	b7 10 42    	staa	0x1042
    8025:	86 78       	ldaa	#0x78
    8027:	b7 10 43    	staa	0x1043
    802a:	4f          	clra
    802b:	b7 10 40    	staa	0x1040
    802e:	b7 10 41    	staa	0x1041
    8031:	b7 10 44    	staa	0x1044
    8034:	86 10       	ldaa	#0x10
    8036:	b7 10 23    	staa	0x1023
    8039:	fc 10 0e    	ldd	0x100e
    803c:	c3 9c 40    	addd	#0x9c40
    803f:	fd 10 1c    	std	0x101c
    8042:	4f          	clra
    8043:	ce 01 ff    	ldx	#0x1ff
    8046:	a7 00       	staa	0x0,x
    8048:	09          	dex
    8049:	26 fb       	bne	0x0x8046
    804b:	97 00       	staa	*0x0
    804d:	7c 01 0c    	inc	0x10c
    8050:	ce 00 f2    	ldx	#0xf2
    8053:	ff 01 0d    	stx	0x10d
    8056:	86 20       	ldaa	#0x20
    8058:	b7 10 44    	staa	0x1044
    805b:	01          	nop
    805c:	01          	nop
    805d:	b6 10 45    	ldaa	0x1045
    8060:	84 03       	anda	#0x3
    8062:	81 03       	cmpa	#0x3
    8064:	26 04       	bne	0x0x806a
    8066:	86 02       	ldaa	#0x2
    8068:	20 06       	bra	0x0x8070
    806a:	81 02       	cmpa	#0x2
    806c:	26 02       	bne	0x0x8070
    806e:	86 03       	ldaa	#0x3
    8070:	b7 01 0f    	staa	0x10f
    8073:	b7 01 18    	staa	0x118
    8076:	86 05       	ldaa	#0x5
    8078:	b7 10 46    	staa	0x1046
    807b:	bd ed ce    	jsr	0xedce
    807e:	cc 01 01    	ldd	#0x101
    8081:	fd 01 08    	std	0x108
    8084:	cc 01 80    	ldd	#0x180
    8087:	fd 01 c0    	std	0x1c0
    808a:	fd 01 c2    	std	0x1c2
    808d:	cc 01 40    	ldd	#0x140
    8090:	fd 01 62    	std	0x162
    8093:	fd 01 60    	std	0x160
    8096:	7c 00 d7    	inc	0xd7
    8099:	c6 09       	ldab	#0x9
    809b:	ce ff ff    	ldx	#0xffff
    809e:	09          	dex
    809f:	26 fd       	bne	0x0x809e
    80a1:	5a          	decb
    80a2:	26 fa       	bne	0x0x809e
    80a4:	86 70       	ldaa	#0x70
    80a6:	b7 10 43    	staa	0x1043
    80a9:	7c 01 17    	inc	0x117
    80ac:	20 54       	bra	0x0x8102
    80ae:	ce 10 23    	ldx	#0x1023
    80b1:	1f 00 20 f9 	brclr	0x0,x, #0x20, 0x0x80ae
    80b5:	7d 01 17    	tst	0x117
    80b8:	27 0a       	beq	0x0x80c4
    80ba:	4f          	clra
    80bb:	b7 01 17    	staa	0x117
    80be:	b7 10 30    	staa	0x1030
    80c1:	7e 81 02    	jmp	0x8102
    80c4:	b6 10 34    	ldaa	0x1034
    80c7:	44          	lsra
    80c8:	f6 01 16    	ldab	0x116
    80cb:	ce 1f 00    	ldx	#0x1f00
    80ce:	3a          	abx
    80cf:	a7 00       	staa	0x0,x
    80d1:	5c          	incb
    80d2:	c1 20       	cmpb	#0x20
    80d4:	27 3d       	beq	0x0x8113
    80d6:	f7 01 16    	stab	0x116
    80d9:	7c 01 17    	inc	0x117
    80dc:	c1 07       	cmpb	#0x7
    80de:	22 07       	bhi	0x0x80e7
    80e0:	ca 70       	orab	#0x70
    80e2:	f7 10 43    	stab	0x1043
    80e5:	20 1b       	bra	0x0x8102
    80e7:	c1 0f       	cmpb	#0xf
    80e9:	22 07       	bhi	0x0x80f2
    80eb:	ca 68       	orab	#0x68
    80ed:	f7 10 43    	stab	0x1043
    80f0:	20 10       	bra	0x0x8102
    80f2:	c1 17       	cmpb	#0x17
    80f4:	22 07       	bhi	0x0x80fd
    80f6:	ca 58       	orab	#0x58
    80f8:	f7 10 43    	stab	0x1043
    80fb:	20 05       	bra	0x0x8102
    80fd:	ca 38       	orab	#0x38
    80ff:	f7 10 43    	stab	0x1043
    8102:	86 20       	ldaa	#0x20
    8104:	b7 10 23    	staa	0x1023
    8107:	fc 10 0e    	ldd	0x100e
    810a:	c3 0f a0    	addd	#0xfa0
    810d:	fd 10 1a    	std	0x101a
    8110:	7e 80 ae    	jmp	0x80ae
    8113:	86 70       	ldaa	#0x70
    8115:	b7 10 43    	staa	0x1043
    8118:	86 01       	ldaa	#0x1
    811a:	b7 01 17    	staa	0x117
    811d:	7f 01 16    	clr	0x116
    8120:	86 3a       	ldaa	#0x3a
    8122:	b7 10 09    	staa	0x1009
    8125:	86 5e       	ldaa	#0x5e
    8127:	b7 10 28    	staa	0x1028
    812a:	b6 7f fd    	ldaa	0x7ffd
    812d:	b7 01 6e    	staa	0x16e
    8130:	b6 7f fc    	ldaa	0x7ffc
    8133:	81 32       	cmpa	#0x32
    8135:	23 02       	bls	0x0x8139
    8137:	86 0f       	ldaa	#0xf
    8139:	b7 01 71    	staa	0x171
    813c:	b7 10 46    	staa	0x1046
    813f:	b6 7f ff    	ldaa	0x7fff
    8142:	b7 01 70    	staa	0x170
    8145:	81 02       	cmpa	#0x2
    8147:	23 06       	bls	0x0x814f
    8149:	7f 01 70    	clr	0x170
    814c:	7f 7f ff    	clr	0x7fff
    814f:	b6 7f fe    	ldaa	0x7ffe
    8152:	b7 01 6f    	staa	0x16f
    8155:	81 0f       	cmpa	#0xf
    8157:	23 07       	bls	0x0x8160
    8159:	4f          	clra
    815a:	b7 01 6f    	staa	0x16f
    815d:	b7 7f fe    	staa	0x7ffe
    8160:	18 ce 79 00 	ldy	#0x7900
    8164:	ce 81 a5    	ldx	#0x81a5
    8167:	a6 00       	ldaa	0x0,x
    8169:	18 a7 00    	staa	0x0,y
    816c:	18 08       	iny
    816e:	08          	inx
    816f:	8c 81 e1    	cpx	#0x81e1
    8172:	25 f3       	bcs	0x0x8167
    8174:	96 f9       	ldaa	*0xf9
    8176:	27 14       	beq	0x0x818c
    8178:	86 04       	ldaa	#0x4
    817a:	91 f9       	cmpa	*0xf9
    817c:	27 1e       	beq	0x0x819c
    817e:	97 f9       	staa	*0xf9
    8180:	bd ad c4    	jsr	0xadc4
    8183:	18 ce 79 00 	ldy	#0x7900
    8187:	ce 81 a5    	ldx	#0x81a5
    818a:	20 db       	bra	0x0x8167
    818c:	86 03       	ldaa	#0x3
    818e:	97 f9       	staa	*0xf9
    8190:	bd ad c4    	jsr	0xadc4
    8193:	18 ce 79 00 	ldy	#0x7900
    8197:	ce 81 a5    	ldx	#0x81a5
    819a:	20 cb       	bra	0x0x8167
    819c:	7f 00 f9    	clr	0xf9
    819f:	bd ad 9e    	jsr	0xad9e
    81a2:	7e 81 e2    	jmp	0x81e2
    81a5:	4d          	tsta
    81a6:	26 0c       	bne	0x0x81b4
    81a8:	b6 10 00    	ldaa	0x1000
    81ab:	84 9f       	anda	#0x9f
    81ad:	8a 20       	oraa	#0x20
    81af:	b7 10 00    	staa	0x1000
    81b2:	20 0a       	bra	0x0x81be
    81b4:	b6 10 00    	ldaa	0x1000
    81b7:	84 9f       	anda	#0x9f
    81b9:	8a 40       	oraa	#0x40
    81bb:	b7 10 00    	staa	0x1000
    81be:	86 b0       	ldaa	#0xb0
    81c0:	3d          	mul
    81c1:	c3 80 00    	addd	#0x8000
    81c4:	8f          	xgdx
    81c5:	18 ce 00 20 	ldy	#0x20
    81c9:	c6 b0       	ldab	#0xb0
    81cb:	a6 00       	ldaa	0x0,x
    81cd:	84 7f       	anda	#0x7f
    81cf:	18 a7 00    	staa	0x0,y
    81d2:	08          	inx
    81d3:	18 08       	iny
    81d5:	5a          	decb
    81d6:	26 f3       	bne	0x0x81cb
    81d8:	b6 10 00    	ldaa	0x1000
    81db:	84 9f       	anda	#0x9f
    81dd:	b7 10 00    	staa	0x1000
    81e0:	39          	rts
    81e1:	01          	nop
    81e2:	7c 00 d0    	inc	0xd0
    81e5:	b6 7f fa    	ldaa	0x7ffa
    81e8:	84 07       	anda	#0x7
    81ea:	97 f0       	staa	*0xf0
    81ec:	b6 1f 07    	ldaa	0x1f07
    81ef:	97 d5       	staa	*0xd5
    81f1:	97 d6       	staa	*0xd6
    81f3:	c6 fc       	ldab	#0xfc
    81f5:	bd b0 fc    	jsr	0xb0fc
    81f8:	bd ad b1    	jsr	0xadb1
    81fb:	ce 51 00    	ldx	#0x5100
    81fe:	c6 80       	ldab	#0x80
    8200:	4f          	clra
    8201:	a7 00       	staa	0x0,x
    8203:	08          	inx
    8204:	5a          	decb
    8205:	26 fa       	bne	0x0x8201
    8207:	cc 51 00    	ldd	#0x5100
    820a:	fd 51 80    	std	0x5180
    820d:	cc 51 10    	ldd	#0x5110
    8210:	fd 51 82    	std	0x5182
    8213:	cc 51 20    	ldd	#0x5120
    8216:	fd 51 84    	std	0x5184
    8219:	cc 51 30    	ldd	#0x5130
    821c:	fd 51 86    	std	0x5186
    821f:	cc 51 40    	ldd	#0x5140
    8222:	fd 51 88    	std	0x5188
    8225:	cc 51 50    	ldd	#0x5150
    8228:	fd 51 8a    	std	0x518a
    822b:	cc 51 60    	ldd	#0x5160
    822e:	fd 51 8c    	std	0x518c
    8231:	cc 51 70    	ldd	#0x5170
    8234:	fd 51 8e    	std	0x518e
    8237:	bd ad 9e    	jsr	0xad9e
    823a:	b6 7f f6    	ldaa	0x7ff6
    823d:	81 04       	cmpa	#0x4
    823f:	23 01       	bls	0x0x8242
    8241:	4f          	clra
    8242:	97 f9       	staa	*0xf9
    8244:	81 04       	cmpa	#0x4
    8246:	27 12       	beq	0x0x825a
    8248:	14 d1 80    	bset	*0xd1, #0x80
    824b:	b6 7f f7    	ldaa	0x7ff7
    824e:	84 7f       	anda	#0x7f
    8250:	97 fa       	staa	*0xfa
    8252:	bd ad c4    	jsr	0xadc4
    8255:	bd a3 70    	jsr	0xa370
    8258:	20 11       	bra	0x0x826b
    825a:	b6 7f f7    	ldaa	0x7ff7
    825d:	b7 01 6a    	staa	0x16a
    8260:	bd ad c4    	jsr	0xadc4
    8263:	bd a5 8e    	jsr	0xa58e
    8266:	b6 50 00    	ldaa	0x5000
    8269:	97 d1       	staa	*0xd1
    826b:	3f          	swi
    826c:	86 80       	ldaa	#0x80
    826e:	b7 10 23    	staa	0x1023
    8271:	b7 01 1c    	staa	0x11c
    8274:	fc 10 0e    	ldd	0x100e
    8277:	c3 4e 20    	addd	#0x4e20
    827a:	fd 10 16    	std	0x1016
    827d:	86 80       	ldaa	#0x80
    827f:	b7 10 22    	staa	0x1022
    8282:	0e          	cli
    8283:	bd 92 a5    	jsr	0x92a5
    8286:	4d          	tsta
    8287:	2b 02       	bmi	0x0x828b
    8289:	20 f8       	bra	0x0x8283
    828b:	16          	tab
    828c:	c4 f0       	andb	#0xf0
    828e:	c1 90       	cmpb	#0x90
    8290:	27 2a       	beq	0x0x82bc
    8292:	c1 80       	cmpb	#0x80
    8294:	26 03       	bne	0x0x8299
    8296:	7e 8a 90    	jmp	0x8a90
    8299:	c1 b0       	cmpb	#0xb0
    829b:	26 03       	bne	0x0x82a0
    829d:	7e 8e 29    	jmp	0x8e29
    82a0:	c1 e0       	cmpb	#0xe0
    82a2:	26 03       	bne	0x0x82a7
    82a4:	7e 8e ed    	jmp	0x8eed
    82a7:	c1 d0       	cmpb	#0xd0
    82a9:	26 03       	bne	0x0x82ae
    82ab:	7e 8f 12    	jmp	0x8f12
    82ae:	c1 c0       	cmpb	#0xc0
    82b0:	26 03       	bne	0x0x82b5
    82b2:	7e 8f 41    	jmp	0x8f41
    82b5:	c1 f0       	cmpb	#0xf0
    82b7:	26 ca       	bne	0x0x8283
    82b9:	7e 8f a4    	jmp	0x8fa4
    82bc:	97 df       	staa	*0xdf
    82be:	7c 00 fc    	inc	0xfc
    82c1:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x82c8
    82c5:	7e 8d 52    	jmp	0x8d52
    82c8:	12 d1 80 0d 	brset	*0xd1, #0x80, 0x0x82d9
    82cc:	d6 d1       	ldab	*0xd1
    82ce:	27 09       	beq	0x0x82d9
    82d0:	ce 84 40    	ldx	#0x8440
    82d3:	58          	aslb
    82d4:	3a          	abx
    82d5:	ee 00       	ldx	0x0,x
    82d7:	6e 00       	jmp	0x0,x
    82d9:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x82e0
    82dd:	7e 8b 95    	jmp	0x8b95
    82e0:	bd 92 a5    	jsr	0x92a5
    82e3:	13 f8 01 03 	brclr	*0xf8, #0x01, 0x0x82ea
    82e7:	7e 83 69    	jmp	0x8369
    82ea:	36          	psha
    82eb:	bd 92 a5    	jsr	0x92a5
    82ee:	4d          	tsta
    82ef:	26 04       	bne	0x0x82f5
    82f1:	32          	pula
    82f2:	7e 83 df    	jmp	0x83df
    82f5:	33          	pulb
    82f6:	f7 01 00    	stab	0x100
    82f9:	18 8f       	xgdy
    82fb:	bd 83 04    	jsr	0x8304
    82fe:	bd 83 25    	jsr	0x8325
    8301:	7e 83 df    	jmp	0x83df
    8304:	d6 6a       	ldab	*0x6a
    8306:	c4 07       	andb	#0x7
    8308:	26 03       	bne	0x0x830d
    830a:	c6 01       	ldab	#0x1
    830c:	39          	rts
    830d:	c1 01       	cmpb	#0x1
    830f:	26 03       	bne	0x0x8314
    8311:	c6 03       	ldab	#0x3
    8313:	39          	rts
    8314:	c1 02       	cmpb	#0x2
    8316:	26 03       	bne	0x0x831b
    8318:	c6 0f       	ldab	#0xf
    831a:	39          	rts
    831b:	c1 03       	cmpb	#0x3
    831d:	26 03       	bne	0x0x8322
    831f:	c6 3f       	ldab	#0x3f
    8321:	39          	rts
    8322:	c6 ff       	ldab	#0xff
    8324:	39          	rts
    8325:	17          	tba
    8326:	da f8       	orab	*0xf8
    8328:	d7 f8       	stab	*0xf8
    832a:	43          	coma
    832b:	7d 00 d0    	tst	0xd0
    832e:	27 05       	beq	0x0x8335
    8330:	7f 00 d0    	clr	0xd0
    8333:	20 08       	bra	0x0x833d
    8335:	7d 10 29    	tst	0x1029
    8338:	2a fb       	bpl	0x0x8335
    833a:	f6 10 2a    	ldab	0x102a
    833d:	01          	nop
    833e:	01          	nop
    833f:	01          	nop
    8340:	01          	nop
    8341:	b7 10 42    	staa	0x1042
    8344:	01          	nop
    8345:	01          	nop
    8346:	01          	nop
    8347:	01          	nop
    8348:	01          	nop
    8349:	01          	nop
    834a:	01          	nop
    834b:	c6 81       	ldab	#0x81
    834d:	f7 10 2a    	stab	0x102a
    8350:	7d 10 29    	tst	0x1029
    8353:	2a fb       	bpl	0x0x8350
    8355:	f6 10 2a    	ldab	0x102a
    8358:	18 8f       	xgdy
    835a:	f7 10 2a    	stab	0x102a
    835d:	7d 10 29    	tst	0x1029
    8360:	2a fb       	bpl	0x0x835d
    8362:	f6 10 2a    	ldab	0x102a
    8365:	b7 10 2a    	staa	0x102a
    8368:	39          	rts
    8369:	7d 00 69    	tst	0x69
    836c:	26 27       	bne	0x0x8395
    836e:	36          	psha
    836f:	bd 92 a5    	jsr	0x92a5
    8372:	4d          	tsta
    8373:	26 04       	bne	0x0x8379
    8375:	32          	pula
    8376:	7e 8a ce    	jmp	0x8ace
    8379:	7d 00 fd    	tst	0xfd
    837c:	27 06       	beq	0x0x8384
    837e:	7f 00 fd    	clr	0xfd
    8381:	7e 82 f5    	jmp	0x82f5
    8384:	fe 01 08    	ldx	0x108
    8387:	f6 01 00    	ldab	0x100
    838a:	e7 00       	stab	0x0,x
    838c:	bd 84 2a    	jsr	0x842a
    838f:	ff 01 08    	stx	0x108
    8392:	7e 82 f5    	jmp	0x82f5
    8395:	36          	psha
    8396:	bd 92 a5    	jsr	0x92a5
    8399:	4d          	tsta
    839a:	26 04       	bne	0x0x83a0
    839c:	32          	pula
    839d:	7e 8b 36    	jmp	0x8b36
    83a0:	16          	tab
    83a1:	32          	pula
    83a2:	b1 01 00    	cmpa	0x100
    83a5:	2b 04       	bmi	0x0x83ab
    83a7:	8d 19       	bsr	0x0x83c2
    83a9:	20 34       	bra	0x0x83df
    83ab:	7d 00 fd    	tst	0xfd
    83ae:	27 07       	beq	0x0x83b7
    83b0:	7f 00 fd    	clr	0xfd
    83b3:	36          	psha
    83b4:	7e 82 f5    	jmp	0x82f5
    83b7:	36          	psha
    83b8:	37          	pshb
    83b9:	b6 01 00    	ldaa	0x100
    83bc:	8d 04       	bsr	0x0x83c2
    83be:	32          	pula
    83bf:	7e 82 f5    	jmp	0x82f5
    83c2:	fe 01 08    	ldx	0x108
    83c5:	c6 01       	ldab	#0x1
    83c7:	f7 01 0a    	stab	0x10a
    83ca:	6d 00       	tst	0x0,x
    83cc:	27 0b       	beq	0x0x83d9
    83ce:	bd 84 2a    	jsr	0x842a
    83d1:	78 01 0a    	asl	0x10a
    83d4:	24 f4       	bcc	0x0x83ca
    83d6:	bd 84 2a    	jsr	0x842a
    83d9:	a7 00       	staa	0x0,x
    83db:	ff 01 08    	stx	0x108
    83de:	39          	rts
    83df:	7f 00 fc    	clr	0xfc
    83e2:	bd 92 a5    	jsr	0x92a5
    83e5:	4d          	tsta
    83e6:	2a 03       	bpl	0x0x83eb
    83e8:	7e 82 86    	jmp	0x8286
    83eb:	7c 00 fc    	inc	0xfc
    83ee:	c6 80       	ldab	#0x80
    83f0:	d1 df       	cmpb	*0xdf
    83f2:	27 0a       	beq	0x0x83fe
    83f4:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x83fb
    83f8:	7e 8b 98    	jmp	0x8b98
    83fb:	7e 82 e3    	jmp	0x82e3
    83fe:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8405
    8402:	7e 8c 84    	jmp	0x8c84
    8405:	7e 8a b7    	jmp	0x8ab7
    8408:	fe 01 08    	ldx	0x108
    840b:	c6 02       	ldab	#0x2
    840d:	f7 01 0a    	stab	0x10a
    8410:	e6 00       	ldab	0x0,x
    8412:	27 06       	beq	0x0x841a
    8414:	11          	cba
    8415:	26 03       	bne	0x0x841a
    8417:	6f 00       	clr	0x0,x
    8419:	39          	rts
    841a:	8d 0e       	bsr	0x0x842a
    841c:	78 01 0a    	asl	0x10a
    841f:	24 ef       	bcc	0x0x8410
    8421:	39          	rts
    8422:	8f          	xgdx
    8423:	5a          	decb
    8424:	26 02       	bne	0x0x8428
    8426:	c6 07       	ldab	#0x7
    8428:	8f          	xgdx
    8429:	39          	rts
    842a:	8f          	xgdx
    842b:	5c          	incb
    842c:	c1 07       	cmpb	#0x7
    842e:	23 02       	bls	0x0x8432
    8430:	c6 01       	ldab	#0x1
    8432:	8f          	xgdx
    8433:	39          	rts
    8434:	e6 00       	ldab	0x0,x
    8436:	11          	cba
    8437:	27 04       	beq	0x0x843d
    8439:	8d ef       	bsr	0x0x842a
    843b:	20 f7       	bra	0x0x8434
    843d:	6f 00       	clr	0x0,x
    843f:	39          	rts
    8440:	84 4e       	anda	#0x4e
    8442:	84 51       	anda	#0x51
    8444:	86 2a       	ldaa	#0x2a
    8446:	86 2d       	ldaa	#0x2d
    8448:	86 30       	ldaa	#0x30
    844a:	86 33       	ldaa	#0x33
    844c:	87          	.byte	0x87
    844d:	b9 7e 82    	adca	0x7e82
    8450:	83 81 80    	subd	#0x8180
    8453:	26 03       	bne	0x0x8458
    8455:	7e 85 52    	jmp	0x8552
    8458:	bd 92 a5    	jsr	0x92a5
    845b:	b1 50 21    	cmpa	0x5021
    845e:	24 60       	bcc	0x0x84c0
    8460:	13 f8 01 03 	brclr	*0xf8, #0x01, 0x0x8467
    8464:	7e 84 80    	jmp	0x8480
    8467:	36          	psha
    8468:	bd 92 a5    	jsr	0x92a5
    846b:	4d          	tsta
    846c:	26 04       	bne	0x0x8472
    846e:	32          	pula
    846f:	7e 84 a5    	jmp	0x84a5
    8472:	33          	pulb
    8473:	f7 01 00    	stab	0x100
    8476:	18 8f       	xgdy
    8478:	c6 01       	ldab	#0x1
    847a:	bd 83 25    	jsr	0x8325
    847d:	7e 84 a5    	jmp	0x84a5
    8480:	36          	psha
    8481:	bd 92 a5    	jsr	0x92a5
    8484:	4d          	tsta
    8485:	26 04       	bne	0x0x848b
    8487:	32          	pula
    8488:	7e 85 62    	jmp	0x8562
    848b:	7d 00 fd    	tst	0xfd
    848e:	27 05       	beq	0x0x8495
    8490:	7f 00 fd    	clr	0xfd
    8493:	20 dd       	bra	0x0x8472
    8495:	fe 01 08    	ldx	0x108
    8498:	f6 01 00    	ldab	0x100
    849b:	e7 00       	stab	0x0,x
    849d:	bd 84 2a    	jsr	0x842a
    84a0:	ff 01 08    	stx	0x108
    84a3:	20 cd       	bra	0x0x8472
    84a5:	7f 00 fc    	clr	0xfc
    84a8:	bd 92 a5    	jsr	0x92a5
    84ab:	4d          	tsta
    84ac:	2a 03       	bpl	0x0x84b1
    84ae:	7e 82 86    	jmp	0x8286
    84b1:	7c 00 fc    	inc	0xfc
    84b4:	c6 80       	ldab	#0x80
    84b6:	d1 df       	cmpb	*0xdf
    84b8:	27 03       	beq	0x0x84bd
    84ba:	7e 84 5b    	jmp	0x845b
    84bd:	7e 85 55    	jmp	0x8555
    84c0:	36          	psha
    84c1:	bd 92 a5    	jsr	0x92a5
    84c4:	4d          	tsta
    84c5:	26 04       	bne	0x0x84cb
    84c7:	32          	pula
    84c8:	7e 85 eb    	jmp	0x85eb
    84cb:	33          	pulb
    84cc:	36          	psha
    84cd:	37          	pshb
    84ce:	fe 51 80    	ldx	0x5180
    84d1:	8f          	xgdx
    84d2:	5c          	incb
    84d3:	d1 f0       	cmpb	*0xf0
    84d5:	23 02       	bls	0x0x84d9
    84d7:	c6 01       	ldab	#0x1
    84d9:	8f          	xgdx
    84da:	d6 f0       	ldab	*0xf0
    84dc:	cb 01       	addb	#0x1
    84de:	6d 00       	tst	0x0,x
    84e0:	27 13       	beq	0x0x84f5
    84e2:	8f          	xgdx
    84e3:	5c          	incb
    84e4:	d1 f0       	cmpb	*0xf0
    84e6:	23 02       	bls	0x0x84ea
    84e8:	c6 01       	ldab	#0x1
    84ea:	8f          	xgdx
    84eb:	5a          	decb
    84ec:	c1 01       	cmpb	#0x1
    84ee:	24 ee       	bcc	0x0x84de
    84f0:	32          	pula
    84f1:	33          	pulb
    84f2:	7e 84 a5    	jmp	0x84a5
    84f5:	ff 51 80    	stx	0x5180
    84f8:	8f          	xgdx
    84f9:	86 01       	ldaa	#0x1
    84fb:	5d          	tstb
    84fc:	27 04       	beq	0x0x8502
    84fe:	5a          	decb
    84ff:	48          	asla
    8500:	20 f9       	bra	0x0x84fb
    8502:	16          	tab
    8503:	9a f8       	oraa	*0xf8
    8505:	97 f8       	staa	*0xf8
    8507:	53          	comb
    8508:	fe 51 80    	ldx	0x5180
    850b:	18 38       	puly
    850d:	8d 03       	bsr	0x0x8512
    850f:	7e 84 a5    	jmp	0x84a5
    8512:	7d 00 d0    	tst	0xd0
    8515:	27 05       	beq	0x0x851c
    8517:	7f 00 d0    	clr	0xd0
    851a:	20 08       	bra	0x0x8524
    851c:	7d 10 29    	tst	0x1029
    851f:	2a fb       	bpl	0x0x851c
    8521:	b6 10 2a    	ldaa	0x102a
    8524:	01          	nop
    8525:	01          	nop
    8526:	01          	nop
    8527:	01          	nop
    8528:	f7 10 42    	stab	0x1042
    852b:	01          	nop
    852c:	01          	nop
    852d:	01          	nop
    852e:	01          	nop
    852f:	01          	nop
    8530:	01          	nop
    8531:	01          	nop
    8532:	86 81       	ldaa	#0x81
    8534:	b7 10 2a    	staa	0x102a
    8537:	7d 10 29    	tst	0x1029
    853a:	2a fb       	bpl	0x0x8537
    853c:	b6 10 2a    	ldaa	0x102a
    853f:	18 8f       	xgdy
    8541:	a7 00       	staa	0x0,x
    8543:	b7 10 2a    	staa	0x102a
    8546:	7d 10 29    	tst	0x1029
    8549:	2a fb       	bpl	0x0x8546
    854b:	b6 10 2a    	ldaa	0x102a
    854e:	f7 10 2a    	stab	0x102a
    8551:	39          	rts
    8552:	bd 92 a5    	jsr	0x92a5
    8555:	b1 50 21    	cmpa	0x5021
    8558:	25 03       	bcs	0x0x855d
    855a:	7e 85 e6    	jmp	0x85e6
    855d:	36          	psha
    855e:	bd 92 a5    	jsr	0x92a5
    8561:	32          	pula
    8562:	12 f8 01 03 	brset	*0xf8, #0x01, 0x0x8569
    8566:	7e 85 cb    	jmp	0x85cb
    8569:	b1 01 00    	cmpa	0x100
    856c:	27 06       	beq	0x0x8574
    856e:	bd 84 08    	jsr	0x8408
    8571:	7e 85 cb    	jmp	0x85cb
    8574:	7f 01 00    	clr	0x100
    8577:	fe 01 08    	ldx	0x108
    857a:	c6 01       	ldab	#0x1
    857c:	f7 01 0a    	stab	0x10a
    857f:	a6 00       	ldaa	0x0,x
    8581:	26 34       	bne	0x0x85b7
    8583:	bd 84 22    	jsr	0x8422
    8586:	78 01 0a    	asl	0x10a
    8589:	24 f4       	bcc	0x0x857f
    858b:	15 f8 01    	bclr	*0xf8, #0x01
    858e:	7d 00 d0    	tst	0xd0
    8591:	27 05       	beq	0x0x8598
    8593:	7f 00 d0    	clr	0xd0
    8596:	20 08       	bra	0x0x85a0
    8598:	7d 10 29    	tst	0x1029
    859b:	2a fb       	bpl	0x0x8598
    859d:	f6 10 2a    	ldab	0x102a
    85a0:	c6 fe       	ldab	#0xfe
    85a2:	01          	nop
    85a3:	01          	nop
    85a4:	01          	nop
    85a5:	01          	nop
    85a6:	f7 10 42    	stab	0x1042
    85a9:	01          	nop
    85aa:	01          	nop
    85ab:	01          	nop
    85ac:	01          	nop
    85ad:	01          	nop
    85ae:	01          	nop
    85af:	01          	nop
    85b0:	86 80       	ldaa	#0x80
    85b2:	b7 10 2a    	staa	0x102a
    85b5:	20 14       	bra	0x0x85cb
    85b7:	6f 00       	clr	0x0,x
    85b9:	16          	tab
    85ba:	f7 01 00    	stab	0x100
    85bd:	bd 84 22    	jsr	0x8422
    85c0:	ff 01 08    	stx	0x108
    85c3:	4f          	clra
    85c4:	18 8f       	xgdy
    85c6:	c6 01       	ldab	#0x1
    85c8:	bd 83 25    	jsr	0x8325
    85cb:	7f 00 fc    	clr	0xfc
    85ce:	bd 92 a5    	jsr	0x92a5
    85d1:	4d          	tsta
    85d2:	2a 03       	bpl	0x0x85d7
    85d4:	7e 82 86    	jmp	0x8286
    85d7:	7c 00 fc    	inc	0xfc
    85da:	c6 80       	ldab	#0x80
    85dc:	d1 df       	cmpb	*0xdf
    85de:	27 03       	beq	0x0x85e3
    85e0:	7e 84 5b    	jmp	0x845b
    85e3:	7e 85 55    	jmp	0x8555
    85e6:	36          	psha
    85e7:	bd 92 a5    	jsr	0x92a5
    85ea:	32          	pula
    85eb:	ce 51 01    	ldx	#0x5101
    85ee:	c6 02       	ldab	#0x2
    85f0:	a1 00       	cmpa	0x0,x
    85f2:	27 06       	beq	0x0x85fa
    85f4:	08          	inx
    85f5:	58          	aslb
    85f6:	24 f8       	bcc	0x0x85f0
    85f8:	20 d1       	bra	0x0x85cb
    85fa:	6f 00       	clr	0x0,x
    85fc:	17          	tba
    85fd:	98 f8       	eora	*0xf8
    85ff:	97 f8       	staa	*0xf8
    8601:	53          	comb
    8602:	7d 00 d0    	tst	0xd0
    8605:	27 05       	beq	0x0x860c
    8607:	7f 00 d0    	clr	0xd0
    860a:	20 08       	bra	0x0x8614
    860c:	7d 10 29    	tst	0x1029
    860f:	2a fb       	bpl	0x0x860c
    8611:	b6 10 2a    	ldaa	0x102a
    8614:	01          	nop
    8615:	01          	nop
    8616:	01          	nop
    8617:	01          	nop
    8618:	f7 10 42    	stab	0x1042
    861b:	01          	nop
    861c:	01          	nop
    861d:	01          	nop
    861e:	01          	nop
    861f:	01          	nop
    8620:	01          	nop
    8621:	01          	nop
    8622:	86 80       	ldaa	#0x80
    8624:	b7 10 2a    	staa	0x102a
    8627:	7e 85 cb    	jmp	0x85cb
    862a:	7e 82 83    	jmp	0x8283
    862d:	7e 82 83    	jmp	0x8283
    8630:	7e 82 83    	jmp	0x8283
    8633:	96 df       	ldaa	*0xdf
    8635:	81 80       	cmpa	#0x80
    8637:	26 03       	bne	0x0x863c
    8639:	7e 87 0c    	jmp	0x870c
    863c:	96 f0       	ldaa	*0xf0
    863e:	4c          	inca
    863f:	44          	lsra
    8640:	b7 01 1f    	staa	0x11f
    8643:	bd 92 a5    	jsr	0x92a5
    8646:	36          	psha
    8647:	bd 92 a5    	jsr	0x92a5
    864a:	4d          	tsta
    864b:	26 04       	bne	0x0x8651
    864d:	32          	pula
    864e:	7e 87 14    	jmp	0x8714
    8651:	33          	pulb
    8652:	36          	psha
    8653:	37          	pshb
    8654:	fe 01 08    	ldx	0x108
    8657:	8f          	xgdx
    8658:	5c          	incb
    8659:	f1 01 1f    	cmpb	0x11f
    865c:	25 01       	bcs	0x0x865f
    865e:	5f          	clrb
    865f:	8f          	xgdx
    8660:	f6 01 1f    	ldab	0x11f
    8663:	6d 00       	tst	0x0,x
    8665:	27 13       	beq	0x0x867a
    8667:	8f          	xgdx
    8668:	5c          	incb
    8669:	f1 01 1f    	cmpb	0x11f
    866c:	25 01       	bcs	0x0x866f
    866e:	5f          	clrb
    866f:	8f          	xgdx
    8670:	5a          	decb
    8671:	26 f0       	bne	0x0x8663
    8673:	f6 01 1f    	ldab	0x11f
    8676:	5c          	incb
    8677:	7e 86 f4    	jmp	0x86f4
    867a:	ff 01 08    	stx	0x108
    867d:	8f          	xgdx
    867e:	86 01       	ldaa	#0x1
    8680:	5d          	tstb
    8681:	27 04       	beq	0x0x8687
    8683:	5a          	decb
    8684:	48          	asla
    8685:	20 f9       	bra	0x0x8680
    8687:	36          	psha
    8688:	f6 01 1f    	ldab	0x11f
    868b:	48          	asla
    868c:	5a          	decb
    868d:	26 fc       	bne	0x0x868b
    868f:	33          	pulb
    8690:	1b          	aba
    8691:	16          	tab
    8692:	9a f8       	oraa	*0xf8
    8694:	97 f8       	staa	*0xf8
    8696:	53          	comb
    8697:	fe 01 08    	ldx	0x108
    869a:	7d 00 d0    	tst	0xd0
    869d:	27 05       	beq	0x0x86a4
    869f:	7f 00 d0    	clr	0xd0
    86a2:	20 08       	bra	0x0x86ac
    86a4:	7d 10 29    	tst	0x1029
    86a7:	2a fb       	bpl	0x0x86a4
    86a9:	b6 10 2a    	ldaa	0x102a
    86ac:	01          	nop
    86ad:	01          	nop
    86ae:	01          	nop
    86af:	01          	nop
    86b0:	f7 10 42    	stab	0x1042
    86b3:	01          	nop
    86b4:	01          	nop
    86b5:	01          	nop
    86b6:	01          	nop
    86b7:	01          	nop
    86b8:	01          	nop
    86b9:	01          	nop
    86ba:	86 81       	ldaa	#0x81
    86bc:	b7 10 2a    	staa	0x102a
    86bf:	7d 10 29    	tst	0x1029
    86c2:	2a fb       	bpl	0x0x86bf
    86c4:	b6 10 2a    	ldaa	0x102a
    86c7:	32          	pula
    86c8:	a7 00       	staa	0x0,x
    86ca:	b7 10 2a    	staa	0x102a
    86cd:	7d 10 29    	tst	0x1029
    86d0:	2a fb       	bpl	0x0x86cd
    86d2:	b6 10 2a    	ldaa	0x102a
    86d5:	32          	pula
    86d6:	b7 10 2a    	staa	0x102a
    86d9:	7f 00 fc    	clr	0xfc
    86dc:	bd 92 a5    	jsr	0x92a5
    86df:	4d          	tsta
    86e0:	2a 03       	bpl	0x0x86e5
    86e2:	7e 82 86    	jmp	0x8286
    86e5:	7c 00 fc    	inc	0xfc
    86e8:	c6 80       	ldab	#0x80
    86ea:	d1 df       	cmpb	*0xdf
    86ec:	27 03       	beq	0x0x86f1
    86ee:	7e 86 46    	jmp	0x8646
    86f1:	7e 87 b1    	jmp	0x87b1
    86f4:	ce 01 00    	ldx	#0x100
    86f7:	3a          	abx
    86f8:	e6 00       	ldab	0x0,x
    86fa:	27 0a       	beq	0x0x8706
    86fc:	08          	inx
    86fd:	8c 01 08    	cpx	#0x108
    8700:	25 f6       	bcs	0x0x86f8
    8702:	32          	pula
    8703:	33          	pulb
    8704:	20 d3       	bra	0x0x86d9
    8706:	32          	pula
    8707:	a7 00       	staa	0x0,x
    8709:	33          	pulb
    870a:	20 cd       	bra	0x0x86d9
    870c:	bd 92 a5    	jsr	0x92a5
    870f:	36          	psha
    8710:	bd 92 a5    	jsr	0x92a5
    8713:	32          	pula
    8714:	ce 01 00    	ldx	#0x100
    8717:	c6 01       	ldab	#0x1
    8719:	a1 00       	cmpa	0x0,x
    871b:	27 07       	beq	0x0x8724
    871d:	08          	inx
    871e:	58          	aslb
    871f:	24 f8       	bcc	0x0x8719
    8721:	7e 87 99    	jmp	0x8799
    8724:	6f 00       	clr	0x0,x
    8726:	8f          	xgdx
    8727:	f1 01 1f    	cmpb	0x11f
    872a:	23 03       	bls	0x0x872f
    872c:	7e 87 99    	jmp	0x8799
    872f:	8f          	xgdx
    8730:	37          	pshb
    8731:	18 ce 01 00 	ldy	#0x100
    8735:	f6 01 1f    	ldab	0x11f
    8738:	18 3a       	aby
    873a:	18 a6 00    	ldaa	0x0,y
    873d:	26 0b       	bne	0x0x874a
    873f:	18 08       	iny
    8741:	18 8c 01 08 	cpy	#0x108
    8745:	25 f3       	bcs	0x0x873a
    8747:	32          	pula
    8748:	20 19       	bra	0x0x8763
    874a:	32          	pula
    874b:	36          	psha
    874c:	f6 01 1f    	ldab	0x11f
    874f:	48          	asla
    8750:	5a          	decb
    8751:	26 fc       	bne	0x0x874f
    8753:	33          	pulb
    8754:	1b          	aba
    8755:	16          	tab
    8756:	53          	comb
    8757:	4f          	clra
    8758:	36          	psha
    8759:	18 a6 00    	ldaa	0x0,y
    875c:	36          	psha
    875d:	18 6f 00    	clr	0x0,y
    8760:	7e 86 9a    	jmp	0x869a
    8763:	36          	psha
    8764:	f6 01 1f    	ldab	0x11f
    8767:	48          	asla
    8768:	5a          	decb
    8769:	26 fc       	bne	0x0x8767
    876b:	33          	pulb
    876c:	1b          	aba
    876d:	16          	tab
    876e:	98 f8       	eora	*0xf8
    8770:	97 f8       	staa	*0xf8
    8772:	53          	comb
    8773:	7d 00 d0    	tst	0xd0
    8776:	27 05       	beq	0x0x877d
    8778:	7f 00 d0    	clr	0xd0
    877b:	20 08       	bra	0x0x8785
    877d:	7d 10 29    	tst	0x1029
    8780:	2a fb       	bpl	0x0x877d
    8782:	b6 10 2a    	ldaa	0x102a
    8785:	01          	nop
    8786:	01          	nop
    8787:	01          	nop
    8788:	01          	nop
    8789:	f7 10 42    	stab	0x1042
    878c:	01          	nop
    878d:	01          	nop
    878e:	01          	nop
    878f:	01          	nop
    8790:	01          	nop
    8791:	01          	nop
    8792:	01          	nop
    8793:	01          	nop
    8794:	86 80       	ldaa	#0x80
    8796:	b7 10 2a    	staa	0x102a
    8799:	7f 00 fc    	clr	0xfc
    879c:	bd 92 a5    	jsr	0x92a5
    879f:	4d          	tsta
    87a0:	2a 03       	bpl	0x0x87a5
    87a2:	7e 82 86    	jmp	0x8286
    87a5:	7c 00 fc    	inc	0xfc
    87a8:	c6 80       	ldab	#0x80
    87aa:	d1 df       	cmpb	*0xdf
    87ac:	27 03       	beq	0x0x87b1
    87ae:	7e 86 46    	jmp	0x8646
    87b1:	36          	psha
    87b2:	bd 92 a5    	jsr	0x92a5
    87b5:	32          	pula
    87b6:	7e 87 14    	jmp	0x8714
    87b9:	d6 df       	ldab	*0xdf
    87bb:	c4 0f       	andb	#0xf
    87bd:	d7 1e       	stab	*0x1e
    87bf:	ce 50 50    	ldx	#0x5050
    87c2:	3a          	abx
    87c3:	6d 08       	tst	0x8,x
    87c5:	27 03       	beq	0x0x87ca
    87c7:	7e 89 20    	jmp	0x8920
    87ca:	84 f0       	anda	#0xf0
    87cc:	81 80       	cmpa	#0x80
    87ce:	26 03       	bne	0x0x87d3
    87d0:	7e 88 5a    	jmp	0x885a
    87d3:	bd 92 a5    	jsr	0x92a5
    87d6:	ce 50 50    	ldx	#0x5050
    87d9:	d6 1e       	ldab	*0x1e
    87db:	3a          	abx
    87dc:	e6 00       	ldab	0x0,x
    87de:	26 06       	bne	0x0x87e6
    87e0:	bd 92 a5    	jsr	0x92a5
    87e3:	7e 88 48    	jmp	0x8848
    87e6:	d4 f8       	andb	*0xf8
    87e8:	26 24       	bne	0x0x880e
    87ea:	36          	psha
    87eb:	bd 92 a5    	jsr	0x92a5
    87ee:	4d          	tsta
    87ef:	26 04       	bne	0x0x87f5
    87f1:	32          	pula
    87f2:	7e 88 48    	jmp	0x8848
    87f5:	ce 00 00    	ldx	#0x0
    87f8:	d6 1e       	ldab	*0x1e
    87fa:	3a          	abx
    87fb:	33          	pulb
    87fc:	e7 00       	stab	0x0,x
    87fe:	18 8f       	xgdy
    8800:	ce 50 50    	ldx	#0x5050
    8803:	d6 1e       	ldab	*0x1e
    8805:	3a          	abx
    8806:	e6 00       	ldab	0x0,x
    8808:	bd 83 25    	jsr	0x8325
    880b:	7e 88 48    	jmp	0x8848
    880e:	36          	psha
    880f:	bd 92 a5    	jsr	0x92a5
    8812:	4d          	tsta
    8813:	26 04       	bne	0x0x8819
    8815:	32          	pula
    8816:	7e 88 62    	jmp	0x8862
    8819:	36          	psha
    881a:	18 ce 00 00 	ldy	#0x0
    881e:	d6 1e       	ldab	*0x1e
    8820:	18 3a       	aby
    8822:	ce 51 80    	ldx	#0x5180
    8825:	58          	aslb
    8826:	3a          	abx
    8827:	3c          	pshx
    8828:	ee 00       	ldx	0x0,x
    882a:	18 e6 00    	ldab	0x0,y
    882d:	e7 00       	stab	0x0,x
    882f:	8f          	xgdx
    8830:	bd 8a 4e    	jsr	0x8a4e
    8833:	38          	pulx
    8834:	ed 00       	std	0x0,x
    8836:	32          	pula
    8837:	33          	pulb
    8838:	18 e7 00    	stab	0x0,y
    883b:	18 8f       	xgdy
    883d:	ce 50 50    	ldx	#0x5050
    8840:	d6 1e       	ldab	*0x1e
    8842:	3a          	abx
    8843:	e6 00       	ldab	0x0,x
    8845:	bd 83 25    	jsr	0x8325
    8848:	7f 00 fc    	clr	0xfc
    884b:	bd 92 a5    	jsr	0x92a5
    884e:	4d          	tsta
    884f:	2a 03       	bpl	0x0x8854
    8851:	7e 82 86    	jmp	0x8286
    8854:	7c 00 fc    	inc	0xfc
    8857:	7e 88 0e    	jmp	0x880e
    885a:	bd 92 a5    	jsr	0x92a5
    885d:	36          	psha
    885e:	bd 92 a5    	jsr	0x92a5
    8861:	32          	pula
    8862:	ce 50 50    	ldx	#0x5050
    8865:	d6 1e       	ldab	*0x1e
    8867:	3a          	abx
    8868:	6d 00       	tst	0x0,x
    886a:	26 03       	bne	0x0x886f
    886c:	7e 89 03    	jmp	0x8903
    886f:	ce 00 00    	ldx	#0x0
    8872:	3a          	abx
    8873:	6d 00       	tst	0x0,x
    8875:	26 03       	bne	0x0x887a
    8877:	7e 89 03    	jmp	0x8903
    887a:	a1 00       	cmpa	0x0,x
    887c:	27 06       	beq	0x0x8884
    887e:	bd 8a 6e    	jsr	0x8a6e
    8881:	7e 89 03    	jmp	0x8903
    8884:	6f 00       	clr	0x0,x
    8886:	18 ce 51 80 	ldy	#0x5180
    888a:	d6 1e       	ldab	*0x1e
    888c:	58          	aslb
    888d:	18 3a       	aby
    888f:	18 ee 00    	ldy	0x0,y
    8892:	c6 01       	ldab	#0x1
    8894:	f7 01 0a    	stab	0x10a
    8897:	18 a6 00    	ldaa	0x0,y
    889a:	26 42       	bne	0x0x88de
    889c:	18 8f       	xgdy
    889e:	bd 8a 5e    	jsr	0x8a5e
    88a1:	18 8f       	xgdy
    88a3:	78 01 0a    	asl	0x10a
    88a6:	24 ef       	bcc	0x0x8897
    88a8:	ce 50 50    	ldx	#0x5050
    88ab:	d6 1e       	ldab	*0x1e
    88ad:	3a          	abx
    88ae:	e6 00       	ldab	0x0,x
    88b0:	53          	comb
    88b1:	37          	pshb
    88b2:	d4 f8       	andb	*0xf8
    88b4:	d7 f8       	stab	*0xf8
    88b6:	7d 00 d0    	tst	0xd0
    88b9:	27 05       	beq	0x0x88c0
    88bb:	7f 00 d0    	clr	0xd0
    88be:	20 08       	bra	0x0x88c8
    88c0:	7d 10 29    	tst	0x1029
    88c3:	2a fb       	bpl	0x0x88c0
    88c5:	f6 10 2a    	ldab	0x102a
    88c8:	33          	pulb
    88c9:	01          	nop
    88ca:	01          	nop
    88cb:	01          	nop
    88cc:	01          	nop
    88cd:	f7 10 42    	stab	0x1042
    88d0:	01          	nop
    88d1:	01          	nop
    88d2:	01          	nop
    88d3:	01          	nop
    88d4:	01          	nop
    88d5:	01          	nop
    88d6:	01          	nop
    88d7:	86 80       	ldaa	#0x80
    88d9:	b7 10 2a    	staa	0x102a
    88dc:	20 25       	bra	0x0x8903
    88de:	18 6f 00    	clr	0x0,y
    88e1:	a7 00       	staa	0x0,x
    88e3:	18 8f       	xgdy
    88e5:	bd 8a 5e    	jsr	0x8a5e
    88e8:	18 8f       	xgdy
    88ea:	ce 51 80    	ldx	#0x5180
    88ed:	d6 1e       	ldab	*0x1e
    88ef:	58          	aslb
    88f0:	3a          	abx
    88f1:	1a ef 00    	sty	0x0,x
    88f4:	16          	tab
    88f5:	4f          	clra
    88f6:	18 8f       	xgdy
    88f8:	ce 50 50    	ldx	#0x5050
    88fb:	d6 1e       	ldab	*0x1e
    88fd:	3a          	abx
    88fe:	e6 00       	ldab	0x0,x
    8900:	bd 83 25    	jsr	0x8325
    8903:	7f 00 fc    	clr	0xfc
    8906:	bd 92 a5    	jsr	0x92a5
    8909:	4d          	tsta
    890a:	2a 03       	bpl	0x0x890f
    890c:	7e 82 86    	jmp	0x8286
    890f:	7c 00 fc    	inc	0xfc
    8912:	d6 df       	ldab	*0xdf
    8914:	c4 f0       	andb	#0xf0
    8916:	c1 80       	cmpb	#0x80
    8918:	27 03       	beq	0x0x891d
    891a:	7e 87 d6    	jmp	0x87d6
    891d:	7e 88 5d    	jmp	0x885d
    8920:	96 df       	ldaa	*0xdf
    8922:	84 f0       	anda	#0xf0
    8924:	81 80       	cmpa	#0x80
    8926:	26 06       	bne	0x0x892e
    8928:	bd 92 a5    	jsr	0x92a5
    892b:	7e 89 c7    	jmp	0x89c7
    892e:	bd 92 a5    	jsr	0x92a5
    8931:	36          	psha
    8932:	ce 50 50    	ldx	#0x5050
    8935:	d6 1e       	ldab	*0x1e
    8937:	3a          	abx
    8938:	6d 00       	tst	0x0,x
    893a:	26 07       	bne	0x0x8943
    893c:	32          	pula
    893d:	bd 92 a5    	jsr	0x92a5
    8940:	7e 89 b5    	jmp	0x89b5
    8943:	bd 92 a5    	jsr	0x92a5
    8946:	4d          	tsta
    8947:	26 03       	bne	0x0x894c
    8949:	7e 89 cb    	jmp	0x89cb
    894c:	33          	pulb
    894d:	36          	psha
    894e:	37          	pshb
    894f:	18 ce 51 00 	ldy	#0x5100
    8953:	d6 1e       	ldab	*0x1e
    8955:	86 10       	ldaa	#0x10
    8957:	3d          	mul
    8958:	18 3a       	aby
    895a:	ce 51 80    	ldx	#0x5180
    895d:	d6 1e       	ldab	*0x1e
    895f:	58          	aslb
    8960:	3a          	abx
    8961:	ec 00       	ldd	0x0,x
    8963:	5c          	incb
    8964:	18 e1 09    	cmpb	0x9,y
    8967:	23 03       	bls	0x0x896c
    8969:	18 e6 08    	ldab	0x8,y
    896c:	8f          	xgdx
    896d:	18 e6 0a    	ldab	0xa,y
    8970:	6d 00       	tst	0x0,x
    8972:	27 13       	beq	0x0x8987
    8974:	8f          	xgdx
    8975:	5c          	incb
    8976:	18 e1 09    	cmpb	0x9,y
    8979:	23 03       	bls	0x0x897e
    897b:	18 e6 08    	ldab	0x8,y
    897e:	8f          	xgdx
    897f:	5a          	decb
    8980:	26 ee       	bne	0x0x8970
    8982:	32          	pula
    8983:	33          	pulb
    8984:	7e 89 b5    	jmp	0x89b5
    8987:	d6 1e       	ldab	*0x1e
    8989:	58          	aslb
    898a:	18 ce 51 80 	ldy	#0x5180
    898e:	18 3a       	aby
    8990:	cd ef 00    	stx	0x0,y
    8993:	8f          	xgdx
    8994:	86 01       	ldaa	#0x1
    8996:	c4 07       	andb	#0x7
    8998:	5d          	tstb
    8999:	27 04       	beq	0x0x899f
    899b:	5a          	decb
    899c:	48          	asla
    899d:	20 f9       	bra	0x0x8998
    899f:	16          	tab
    89a0:	9a f8       	oraa	*0xf8
    89a2:	97 f8       	staa	*0xf8
    89a4:	53          	comb
    89a5:	37          	pshb
    89a6:	d6 1e       	ldab	*0x1e
    89a8:	58          	aslb
    89a9:	ce 51 80    	ldx	#0x5180
    89ac:	3a          	abx
    89ad:	ee 00       	ldx	0x0,x
    89af:	33          	pulb
    89b0:	18 38       	puly
    89b2:	bd 85 12    	jsr	0x8512
    89b5:	7f 00 fc    	clr	0xfc
    89b8:	bd 92 a5    	jsr	0x92a5
    89bb:	4d          	tsta
    89bc:	2a 03       	bpl	0x0x89c1
    89be:	7e 82 86    	jmp	0x8286
    89c1:	7c 00 fc    	inc	0xfc
    89c4:	7e 89 31    	jmp	0x8931
    89c7:	36          	psha
    89c8:	bd 92 a5    	jsr	0x92a5
    89cb:	ce 50 50    	ldx	#0x5050
    89ce:	d6 1e       	ldab	*0x1e
    89d0:	3a          	abx
    89d1:	6d 00       	tst	0x0,x
    89d3:	26 04       	bne	0x0x89d9
    89d5:	32          	pula
    89d6:	7e 8a 31    	jmp	0x8a31
    89d9:	18 ce 51 00 	ldy	#0x5100
    89dd:	d6 1e       	ldab	*0x1e
    89df:	86 10       	ldaa	#0x10
    89e1:	3d          	mul
    89e2:	18 3a       	aby
    89e4:	18 3c       	pshy
    89e6:	38          	pulx
    89e7:	18 e6 08    	ldab	0x8,y
    89ea:	c4 07       	andb	#0x7
    89ec:	3a          	abx
    89ed:	18 e6 0a    	ldab	0xa,y
    89f0:	32          	pula
    89f1:	a1 00       	cmpa	0x0,x
    89f3:	27 06       	beq	0x0x89fb
    89f5:	08          	inx
    89f6:	5a          	decb
    89f7:	26 f8       	bne	0x0x89f1
    89f9:	20 36       	bra	0x0x8a31
    89fb:	6f 00       	clr	0x0,x
    89fd:	8f          	xgdx
    89fe:	c4 07       	andb	#0x7
    8a00:	4f          	clra
    8a01:	0d          	sec
    8a02:	49          	rola
    8a03:	5a          	decb
    8a04:	2a fc       	bpl	0x0x8a02
    8a06:	16          	tab
    8a07:	98 f8       	eora	*0xf8
    8a09:	97 f8       	staa	*0xf8
    8a0b:	53          	comb
    8a0c:	7d 00 d0    	tst	0xd0
    8a0f:	27 05       	beq	0x0x8a16
    8a11:	7f 00 d0    	clr	0xd0
    8a14:	20 08       	bra	0x0x8a1e
    8a16:	7d 10 29    	tst	0x1029
    8a19:	2a fb       	bpl	0x0x8a16
    8a1b:	b6 10 2a    	ldaa	0x102a
    8a1e:	01          	nop
    8a1f:	01          	nop
    8a20:	01          	nop
    8a21:	01          	nop
    8a22:	f7 10 42    	stab	0x1042
    8a25:	01          	nop
    8a26:	01          	nop
    8a27:	01          	nop
    8a28:	01          	nop
    8a29:	01          	nop
    8a2a:	01          	nop
    8a2b:	01          	nop
    8a2c:	86 80       	ldaa	#0x80
    8a2e:	b7 10 2a    	staa	0x102a
    8a31:	7f 00 fc    	clr	0xfc
    8a34:	bd 92 a5    	jsr	0x92a5
    8a37:	4d          	tsta
    8a38:	2a 03       	bpl	0x0x8a3d
    8a3a:	7e 82 86    	jmp	0x8286
    8a3d:	7c 00 fc    	inc	0xfc
    8a40:	d6 df       	ldab	*0xdf
    8a42:	c4 f0       	andb	#0xf0
    8a44:	c1 80       	cmpb	#0x80
    8a46:	27 03       	beq	0x0x8a4b
    8a48:	7e 89 31    	jmp	0x8931
    8a4b:	7e 89 c7    	jmp	0x89c7
    8a4e:	37          	pshb
    8a4f:	c4 f0       	andb	#0xf0
    8a51:	f7 01 1f    	stab	0x11f
    8a54:	33          	pulb
    8a55:	c4 07       	andb	#0x7
    8a57:	5c          	incb
    8a58:	c4 07       	andb	#0x7
    8a5a:	fb 01 1f    	addb	0x11f
    8a5d:	39          	rts
    8a5e:	37          	pshb
    8a5f:	c4 f0       	andb	#0xf0
    8a61:	f7 01 1f    	stab	0x11f
    8a64:	33          	pulb
    8a65:	c4 07       	andb	#0x7
    8a67:	5a          	decb
    8a68:	c4 07       	andb	#0x7
    8a6a:	fb 01 1f    	addb	0x11f
    8a6d:	39          	rts
    8a6e:	ce 51 80    	ldx	#0x5180
    8a71:	d6 1e       	ldab	*0x1e
    8a73:	58          	aslb
    8a74:	3a          	abx
    8a75:	ee 00       	ldx	0x0,x
    8a77:	c6 01       	ldab	#0x1
    8a79:	f7 01 0a    	stab	0x10a
    8a7c:	e6 00       	ldab	0x0,x
    8a7e:	27 06       	beq	0x0x8a86
    8a80:	11          	cba
    8a81:	26 03       	bne	0x0x8a86
    8a83:	6f 00       	clr	0x0,x
    8a85:	39          	rts
    8a86:	8f          	xgdx
    8a87:	8d c5       	bsr	0x0x8a4e
    8a89:	8f          	xgdx
    8a8a:	78 01 0a    	asl	0x10a
    8a8d:	24 ed       	bcc	0x0x8a7c
    8a8f:	39          	rts
    8a90:	97 df       	staa	*0xdf
    8a92:	7c 00 fc    	inc	0xfc
    8a95:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x8a9c
    8a99:	7e 8d d6    	jmp	0x8dd6
    8a9c:	12 d1 80 0d 	brset	*0xd1, #0x80, 0x0x8aad
    8aa0:	d6 d1       	ldab	*0xd1
    8aa2:	27 09       	beq	0x0x8aad
    8aa4:	ce 84 40    	ldx	#0x8440
    8aa7:	58          	aslb
    8aa8:	3a          	abx
    8aa9:	ee 00       	ldx	0x0,x
    8aab:	6e 00       	jmp	0x0,x
    8aad:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8ab4
    8ab1:	7e 8c 81    	jmp	0x8c81
    8ab4:	bd 92 a5    	jsr	0x92a5
    8ab7:	36          	psha
    8ab8:	bd 92 a5    	jsr	0x92a5
    8abb:	32          	pula
    8abc:	7d 01 00    	tst	0x100
    8abf:	26 06       	bne	0x0x8ac7
    8ac1:	bd 84 08    	jsr	0x8408
    8ac4:	7e 8b 6c    	jmp	0x8b6c
    8ac7:	7d 00 69    	tst	0x69
    8aca:	27 02       	beq	0x0x8ace
    8acc:	20 68       	bra	0x0x8b36
    8ace:	b1 01 00    	cmpa	0x100
    8ad1:	27 06       	beq	0x0x8ad9
    8ad3:	bd 84 08    	jsr	0x8408
    8ad6:	7e 8b 6c    	jmp	0x8b6c
    8ad9:	7f 01 00    	clr	0x100
    8adc:	fe 01 08    	ldx	0x108
    8adf:	c6 01       	ldab	#0x1
    8ae1:	f7 01 0a    	stab	0x10a
    8ae4:	a6 00       	ldaa	0x0,x
    8ae6:	26 41       	bne	0x0x8b29
    8ae8:	bd 84 22    	jsr	0x8422
    8aeb:	78 01 0a    	asl	0x10a
    8aee:	24 f4       	bcc	0x0x8ae4
    8af0:	7d 00 f1    	tst	0xf1
    8af3:	27 08       	beq	0x0x8afd
    8af5:	bd 83 04    	jsr	0x8304
    8af8:	d7 fd       	stab	*0xfd
    8afa:	7e 83 df    	jmp	0x83df
    8afd:	7f 00 f8    	clr	0xf8
    8b00:	7d 00 d0    	tst	0xd0
    8b03:	27 05       	beq	0x0x8b0a
    8b05:	7f 00 d0    	clr	0xd0
    8b08:	20 08       	bra	0x0x8b12
    8b0a:	7d 10 29    	tst	0x1029
    8b0d:	2a fb       	bpl	0x0x8b0a
    8b0f:	f6 10 2a    	ldab	0x102a
    8b12:	5f          	clrb
    8b13:	01          	nop
    8b14:	01          	nop
    8b15:	01          	nop
    8b16:	01          	nop
    8b17:	f7 10 42    	stab	0x1042
    8b1a:	01          	nop
    8b1b:	01          	nop
    8b1c:	01          	nop
    8b1d:	01          	nop
    8b1e:	01          	nop
    8b1f:	01          	nop
    8b20:	01          	nop
    8b21:	86 80       	ldaa	#0x80
    8b23:	b7 10 2a    	staa	0x102a
    8b26:	7e 83 df    	jmp	0x83df
    8b29:	6f 00       	clr	0x0,x
    8b2b:	36          	psha
    8b2c:	bd 84 22    	jsr	0x8422
    8b2f:	ff 01 08    	stx	0x108
    8b32:	4f          	clra
    8b33:	7e 82 f5    	jmp	0x82f5
    8b36:	b1 01 00    	cmpa	0x100
    8b39:	27 05       	beq	0x0x8b40
    8b3b:	bd 84 08    	jsr	0x8408
    8b3e:	20 2c       	bra	0x0x8b6c
    8b40:	7f 01 00    	clr	0x100
    8b43:	fe 01 08    	ldx	0x108
    8b46:	c6 01       	ldab	#0x1
    8b48:	f7 01 0a    	stab	0x10a
    8b4b:	86 80       	ldaa	#0x80
    8b4d:	e6 00       	ldab	0x0,x
    8b4f:	27 04       	beq	0x0x8b55
    8b51:	11          	cba
    8b52:	25 01       	bcs	0x0x8b55
    8b54:	17          	tba
    8b55:	bd 84 2a    	jsr	0x842a
    8b58:	78 01 0a    	asl	0x10a
    8b5b:	24 f0       	bcc	0x0x8b4d
    8b5d:	81 80       	cmpa	#0x80
    8b5f:	26 03       	bne	0x0x8b64
    8b61:	7e 8a f0    	jmp	0x8af0
    8b64:	36          	psha
    8b65:	bd 84 34    	jsr	0x8434
    8b68:	4f          	clra
    8b69:	7e 82 f5    	jmp	0x82f5
    8b6c:	7f 00 fc    	clr	0xfc
    8b6f:	bd 92 a5    	jsr	0x92a5
    8b72:	4d          	tsta
    8b73:	2a 03       	bpl	0x0x8b78
    8b75:	7e 82 86    	jmp	0x8286
    8b78:	7c 00 fc    	inc	0xfc
    8b7b:	c6 80       	ldab	#0x80
    8b7d:	d1 df       	cmpb	*0xdf
    8b7f:	27 0a       	beq	0x0x8b8b
    8b81:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8b88
    8b85:	7e 8b 98    	jmp	0x8b98
    8b88:	7e 82 e3    	jmp	0x82e3
    8b8b:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8b92
    8b8f:	7e 8c 84    	jmp	0x8c84
    8b92:	7e 8a b7    	jmp	0x8ab7
    8b95:	bd 92 a5    	jsr	0x92a5
    8b98:	36          	psha
    8b99:	bd 92 a5    	jsr	0x92a5
    8b9c:	4d          	tsta
    8b9d:	26 04       	bne	0x0x8ba3
    8b9f:	32          	pula
    8ba0:	7e 8c 89    	jmp	0x8c89
    8ba3:	7d 00 95    	tst	0x95
    8ba6:	27 31       	beq	0x0x8bd9
    8ba8:	33          	pulb
    8ba9:	36          	psha
    8baa:	37          	pshb
    8bab:	ce 01 00    	ldx	#0x100
    8bae:	6d 00       	tst	0x0,x
    8bb0:	27 4d       	beq	0x0x8bff
    8bb2:	8f          	xgdx
    8bb3:	5c          	incb
    8bb4:	d1 f0       	cmpb	*0xf0
    8bb6:	22 03       	bhi	0x0x8bbb
    8bb8:	8f          	xgdx
    8bb9:	20 f3       	bra	0x0x8bae
    8bbb:	c1 07       	cmpb	#0x7
    8bbd:	24 0e       	bcc	0x0x8bcd
    8bbf:	ce 01 00    	ldx	#0x100
    8bc2:	3a          	abx
    8bc3:	e6 00       	ldab	0x0,x
    8bc5:	27 0b       	beq	0x0x8bd2
    8bc7:	08          	inx
    8bc8:	8c 01 08    	cpx	#0x108
    8bcb:	25 f6       	bcs	0x0x8bc3
    8bcd:	32          	pula
    8bce:	33          	pulb
    8bcf:	7e 8c 54    	jmp	0x8c54
    8bd2:	32          	pula
    8bd3:	a7 00       	staa	0x0,x
    8bd5:	33          	pulb
    8bd6:	7e 8c 54    	jmp	0x8c54
    8bd9:	33          	pulb
    8bda:	36          	psha
    8bdb:	37          	pshb
    8bdc:	fe 01 08    	ldx	0x108
    8bdf:	8f          	xgdx
    8be0:	5c          	incb
    8be1:	d1 f0       	cmpb	*0xf0
    8be3:	23 01       	bls	0x0x8be6
    8be5:	5f          	clrb
    8be6:	8f          	xgdx
    8be7:	d6 f0       	ldab	*0xf0
    8be9:	cb 02       	addb	#0x2
    8beb:	6d 00       	tst	0x0,x
    8bed:	27 10       	beq	0x0x8bff
    8bef:	8f          	xgdx
    8bf0:	5c          	incb
    8bf1:	d1 f0       	cmpb	*0xf0
    8bf3:	23 01       	bls	0x0x8bf6
    8bf5:	5f          	clrb
    8bf6:	8f          	xgdx
    8bf7:	5a          	decb
    8bf8:	26 f1       	bne	0x0x8beb
    8bfa:	d6 f0       	ldab	*0xf0
    8bfc:	5c          	incb
    8bfd:	20 bc       	bra	0x0x8bbb
    8bff:	ff 01 08    	stx	0x108
    8c02:	8f          	xgdx
    8c03:	86 01       	ldaa	#0x1
    8c05:	5d          	tstb
    8c06:	27 04       	beq	0x0x8c0c
    8c08:	5a          	decb
    8c09:	48          	asla
    8c0a:	20 f9       	bra	0x0x8c05
    8c0c:	16          	tab
    8c0d:	9a f8       	oraa	*0xf8
    8c0f:	97 f8       	staa	*0xf8
    8c11:	53          	comb
    8c12:	fe 01 08    	ldx	0x108
    8c15:	7d 00 d0    	tst	0xd0
    8c18:	27 05       	beq	0x0x8c1f
    8c1a:	7f 00 d0    	clr	0xd0
    8c1d:	20 08       	bra	0x0x8c27
    8c1f:	7d 10 29    	tst	0x1029
    8c22:	2a fb       	bpl	0x0x8c1f
    8c24:	b6 10 2a    	ldaa	0x102a
    8c27:	01          	nop
    8c28:	01          	nop
    8c29:	01          	nop
    8c2a:	01          	nop
    8c2b:	f7 10 42    	stab	0x1042
    8c2e:	01          	nop
    8c2f:	01          	nop
    8c30:	01          	nop
    8c31:	01          	nop
    8c32:	01          	nop
    8c33:	01          	nop
    8c34:	01          	nop
    8c35:	86 81       	ldaa	#0x81
    8c37:	b7 10 2a    	staa	0x102a
    8c3a:	7d 10 29    	tst	0x1029
    8c3d:	2a fb       	bpl	0x0x8c3a
    8c3f:	b6 10 2a    	ldaa	0x102a
    8c42:	32          	pula
    8c43:	a7 00       	staa	0x0,x
    8c45:	b7 10 2a    	staa	0x102a
    8c48:	7d 10 29    	tst	0x1029
    8c4b:	2a fb       	bpl	0x0x8c48
    8c4d:	b6 10 2a    	ldaa	0x102a
    8c50:	32          	pula
    8c51:	b7 10 2a    	staa	0x102a
    8c54:	7f 00 fc    	clr	0xfc
    8c57:	bd 92 a5    	jsr	0x92a5
    8c5a:	4d          	tsta
    8c5b:	2a 03       	bpl	0x0x8c60
    8c5d:	7e 82 86    	jmp	0x8286
    8c60:	7c 00 fc    	inc	0xfc
    8c63:	c6 80       	ldab	#0x80
    8c65:	d1 df       	cmpb	*0xdf
    8c67:	27 0a       	beq	0x0x8c73
    8c69:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8c70
    8c6d:	7e 8b 98    	jmp	0x8b98
    8c70:	7e 82 e3    	jmp	0x82e3
    8c73:	13 6a 20 03 	brclr	*0x6a, #0x20, 0x0x8c7a
    8c77:	7e 8a b7    	jmp	0x8ab7
    8c7a:	36          	psha
    8c7b:	bd 92 a5    	jsr	0x92a5
    8c7e:	32          	pula
    8c7f:	20 08       	bra	0x0x8c89
    8c81:	bd 92 a5    	jsr	0x92a5
    8c84:	36          	psha
    8c85:	bd 92 a5    	jsr	0x92a5
    8c88:	32          	pula
    8c89:	ce 01 00    	ldx	#0x100
    8c8c:	c6 01       	ldab	#0x1
    8c8e:	a1 00       	cmpa	0x0,x
    8c90:	27 07       	beq	0x0x8c99
    8c92:	08          	inx
    8c93:	58          	aslb
    8c94:	24 f8       	bcc	0x0x8c8e
    8c96:	7e 8d 24    	jmp	0x8d24
    8c99:	7d 00 f1    	tst	0xf1
    8c9c:	27 22       	beq	0x0x8cc0
    8c9e:	3c          	pshx
    8c9f:	37          	pshb
    8ca0:	36          	psha
    8ca1:	8f          	xgdx
    8ca2:	86 01       	ldaa	#0x1
    8ca4:	5a          	decb
    8ca5:	2b 03       	bmi	0x0x8caa
    8ca7:	48          	asla
    8ca8:	20 fa       	bra	0x0x8ca4
    8caa:	36          	psha
    8cab:	94 fd       	anda	*0xfd
    8cad:	27 06       	beq	0x0x8cb5
    8caf:	32          	pula
    8cb0:	32          	pula
    8cb1:	33          	pulb
    8cb2:	38          	pulx
    8cb3:	20 dd       	bra	0x0x8c92
    8cb5:	32          	pula
    8cb6:	9a fd       	oraa	*0xfd
    8cb8:	97 fd       	staa	*0xfd
    8cba:	32          	pula
    8cbb:	33          	pulb
    8cbc:	38          	pulx
    8cbd:	7e 8d 24    	jmp	0x8d24
    8cc0:	6f 00       	clr	0x0,x
    8cc2:	96 f0       	ldaa	*0xf0
    8cc4:	81 07       	cmpa	#0x7
    8cc6:	27 31       	beq	0x0x8cf9
    8cc8:	8f          	xgdx
    8cc9:	d1 f0       	cmpb	*0xf0
    8ccb:	23 03       	bls	0x0x8cd0
    8ccd:	7e 8d 24    	jmp	0x8d24
    8cd0:	8f          	xgdx
    8cd1:	37          	pshb
    8cd2:	18 ce 01 00 	ldy	#0x100
    8cd6:	d6 f0       	ldab	*0xf0
    8cd8:	5c          	incb
    8cd9:	18 3a       	aby
    8cdb:	18 a6 00    	ldaa	0x0,y
    8cde:	26 0b       	bne	0x0x8ceb
    8ce0:	18 08       	iny
    8ce2:	18 8c 01 08 	cpy	#0x108
    8ce6:	25 f3       	bcs	0x0x8cdb
    8ce8:	33          	pulb
    8ce9:	20 0e       	bra	0x0x8cf9
    8ceb:	33          	pulb
    8cec:	53          	comb
    8ced:	4f          	clra
    8cee:	36          	psha
    8cef:	18 a6 00    	ldaa	0x0,y
    8cf2:	36          	psha
    8cf3:	18 6f 00    	clr	0x0,y
    8cf6:	7e 8c 15    	jmp	0x8c15
    8cf9:	17          	tba
    8cfa:	98 f8       	eora	*0xf8
    8cfc:	97 f8       	staa	*0xf8
    8cfe:	53          	comb
    8cff:	7d 00 d0    	tst	0xd0
    8d02:	27 05       	beq	0x0x8d09
    8d04:	7f 00 d0    	clr	0xd0
    8d07:	20 08       	bra	0x0x8d11
    8d09:	7d 10 29    	tst	0x1029
    8d0c:	2a fb       	bpl	0x0x8d09
    8d0e:	b6 10 2a    	ldaa	0x102a
    8d11:	01          	nop
    8d12:	01          	nop
    8d13:	01          	nop
    8d14:	01          	nop
    8d15:	f7 10 42    	stab	0x1042
    8d18:	01          	nop
    8d19:	01          	nop
    8d1a:	01          	nop
    8d1b:	01          	nop
    8d1c:	01          	nop
    8d1d:	01          	nop
    8d1e:	01          	nop
    8d1f:	86 80       	ldaa	#0x80
    8d21:	b7 10 2a    	staa	0x102a
    8d24:	7f 00 fc    	clr	0xfc
    8d27:	bd 92 a5    	jsr	0x92a5
    8d2a:	4d          	tsta
    8d2b:	2a 03       	bpl	0x0x8d30
    8d2d:	7e 82 86    	jmp	0x8286
    8d30:	7c 00 fc    	inc	0xfc
    8d33:	c6 80       	ldab	#0x80
    8d35:	d1 df       	cmpb	*0xdf
    8d37:	27 0a       	beq	0x0x8d43
    8d39:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8d40
    8d3d:	7e 8b 98    	jmp	0x8b98
    8d40:	7e 82 e3    	jmp	0x82e3
    8d43:	13 6a 20 03 	brclr	*0x6a, #0x20, 0x0x8d4a
    8d47:	7e 8a b7    	jmp	0x8ab7
    8d4a:	36          	psha
    8d4b:	bd 92 a5    	jsr	0x92a5
    8d4e:	32          	pula
    8d4f:	7e 8c 89    	jmp	0x8c89
    8d52:	bd 92 a5    	jsr	0x92a5
    8d55:	36          	psha
    8d56:	bd 92 a5    	jsr	0x92a5
    8d59:	7f 00 fc    	clr	0xfc
    8d5c:	4d          	tsta
    8d5d:	26 04       	bne	0x0x8d63
    8d5f:	32          	pula
    8d60:	7e 8d de    	jmp	0x8dde
    8d63:	32          	pula
    8d64:	ce 01 00    	ldx	#0x100
    8d67:	e6 00       	ldab	0x0,x
    8d69:	27 20       	beq	0x0x8d8b
    8d6b:	11          	cba
    8d6c:	24 15       	bcc	0x0x8d83
    8d6e:	a7 00       	staa	0x0,x
    8d70:	08          	inx
    8d71:	8c 01 08    	cpx	#0x108
    8d74:	27 51       	beq	0x0x8dc7
    8d76:	17          	tba
    8d77:	e6 00       	ldab	0x0,x
    8d79:	26 f3       	bne	0x0x8d6e
    8d7b:	a7 00       	staa	0x0,x
    8d7d:	8f          	xgdx
    8d7e:	f7 01 76    	stab	0x176
    8d81:	20 44       	bra	0x0x8dc7
    8d83:	08          	inx
    8d84:	8c 01 08    	cpx	#0x108
    8d87:	27 3e       	beq	0x0x8dc7
    8d89:	20 dc       	bra	0x0x8d67
    8d8b:	a7 00       	staa	0x0,x
    8d8d:	8f          	xgdx
    8d8e:	f7 01 76    	stab	0x176
    8d91:	26 34       	bne	0x0x8dc7
    8d93:	7d 00 dc    	tst	0xdc
    8d96:	26 2f       	bne	0x0x8dc7
    8d98:	86 01       	ldaa	#0x1
    8d9a:	b7 01 72    	staa	0x172
    8d9d:	86 80       	ldaa	#0x80
    8d9f:	97 d2       	staa	*0xd2
    8da1:	7f 00 30    	clr	0x30
    8da4:	7f 00 d3    	clr	0xd3
    8da7:	96 73       	ldaa	*0x73
    8da9:	81 02       	cmpa	#0x2
    8dab:	26 04       	bne	0x0x8db1
    8dad:	96 74       	ldaa	*0x74
    8daf:	97 d3       	staa	*0xd3
    8db1:	86 08       	ldaa	#0x8
    8db3:	b7 10 23    	staa	0x1023
    8db6:	fc 10 0e    	ldd	0x100e
    8db9:	c3 00 c8    	addd	#0xc8
    8dbc:	fd 10 1e    	std	0x101e
    8dbf:	b6 10 22    	ldaa	0x1022
    8dc2:	8a 08       	oraa	#0x8
    8dc4:	b7 10 22    	staa	0x1022
    8dc7:	bd 92 a5    	jsr	0x92a5
    8dca:	4d          	tsta
    8dcb:	2a 03       	bpl	0x0x8dd0
    8dcd:	7e 82 86    	jmp	0x8286
    8dd0:	7c 00 fc    	inc	0xfc
    8dd3:	7e 8d 55    	jmp	0x8d55
    8dd6:	bd 92 a5    	jsr	0x92a5
    8dd9:	36          	psha
    8dda:	bd 92 a5    	jsr	0x92a5
    8ddd:	32          	pula
    8dde:	7f 00 fc    	clr	0xfc
    8de1:	ce 01 00    	ldx	#0x100
    8de4:	a1 00       	cmpa	0x0,x
    8de6:	27 08       	beq	0x0x8df0
    8de8:	08          	inx
    8de9:	8c 01 08    	cpx	#0x108
    8dec:	27 23       	beq	0x0x8e11
    8dee:	20 f4       	bra	0x0x8de4
    8df0:	8c 01 07    	cpx	#0x107
    8df3:	25 09       	bcs	0x0x8dfe
    8df5:	6f 00       	clr	0x0,x
    8df7:	86 06       	ldaa	#0x6
    8df9:	b7 01 76    	staa	0x176
    8dfc:	20 13       	bra	0x0x8e11
    8dfe:	a6 01       	ldaa	0x1,x
    8e00:	27 05       	beq	0x0x8e07
    8e02:	a7 00       	staa	0x0,x
    8e04:	08          	inx
    8e05:	20 e9       	bra	0x0x8df0
    8e07:	a7 00       	staa	0x0,x
    8e09:	8f          	xgdx
    8e0a:	5a          	decb
    8e0b:	2a 01       	bpl	0x0x8e0e
    8e0d:	5f          	clrb
    8e0e:	f7 01 76    	stab	0x176
    8e11:	bd 92 a5    	jsr	0x92a5
    8e14:	4d          	tsta
    8e15:	2a 03       	bpl	0x0x8e1a
    8e17:	7e 82 86    	jmp	0x8286
    8e1a:	7c 00 fc    	inc	0xfc
    8e1d:	c6 80       	ldab	#0x80
    8e1f:	d1 df       	cmpb	*0xdf
    8e21:	27 03       	beq	0x0x8e26
    8e23:	7e 8d 55    	jmp	0x8d55
    8e26:	7e 8d d9    	jmp	0x8dd9
    8e29:	97 df       	staa	*0xdf
    8e2b:	7c 00 fc    	inc	0xfc
    8e2e:	bd 92 a5    	jsr	0x92a5
    8e31:	81 00       	cmpa	#0x0
    8e33:	26 09       	bne	0x0x8e3e
    8e35:	7c 00 d8    	inc	0xd8
    8e38:	bd 92 a5    	jsr	0x92a5
    8e3b:	7e 8e ad    	jmp	0x8ead
    8e3e:	81 20       	cmpa	#0x20
    8e40:	26 25       	bne	0x0x8e67
    8e42:	bd 92 a5    	jsr	0x92a5
    8e45:	7d 00 d8    	tst	0xd8
    8e48:	27 1a       	beq	0x0x8e64
    8e4a:	7f 00 d8    	clr	0xd8
    8e4d:	d6 df       	ldab	*0xdf
    8e4f:	c4 0f       	andb	#0xf
    8e51:	26 11       	bne	0x0x8e64
    8e53:	81 05       	cmpa	#0x5
    8e55:	25 02       	bcs	0x0x8e59
    8e57:	86 04       	ldaa	#0x4
    8e59:	97 f9       	staa	*0xf9
    8e5b:	bd ad 9e    	jsr	0xad9e
    8e5e:	b7 7f f6    	staa	0x7ff6
    8e61:	bd ad c4    	jsr	0xadc4
    8e64:	7e 8e ad    	jmp	0x8ead
    8e67:	7f 00 d8    	clr	0xd8
    8e6a:	81 7b       	cmpa	#0x7b
    8e6c:	26 09       	bne	0x0x8e77
    8e6e:	bd 9f 0f    	jsr	0x9f0f
    8e71:	bd 92 a5    	jsr	0x92a5
    8e74:	7e 8e ad    	jmp	0x8ead
    8e77:	36          	psha
    8e78:	bd 92 a5    	jsr	0x92a5
    8e7b:	33          	pulb
    8e7c:	c1 01       	cmpb	#0x1
    8e7e:	26 12       	bne	0x0x8e92
    8e80:	13 8e 01 0c 	brclr	*0x8e, #0x01, 0x0x8e90
    8e84:	36          	psha
    8e85:	37          	pshb
    8e86:	16          	tab
    8e87:	4f          	clra
    8e88:	8f          	xgdx
    8e89:	86 87       	ldaa	#0x87
    8e8b:	bd 91 fb    	jsr	0x91fb
    8e8e:	33          	pulb
    8e8f:	32          	pula
    8e90:	20 15       	bra	0x0x8ea7
    8e92:	c1 02       	cmpb	#0x2
    8e94:	26 29       	bne	0x0x8ebf
    8e96:	7d 00 8e    	tst	0x8e
    8e99:	27 02       	beq	0x0x8e9d
    8e9b:	20 10       	bra	0x0x8ead
    8e9d:	16          	tab
    8e9e:	4f          	clra
    8e9f:	8f          	xgdx
    8ea0:	86 87       	ldaa	#0x87
    8ea2:	bd 91 fb    	jsr	0x91fb
    8ea5:	20 06       	bra	0x0x8ead
    8ea7:	8f          	xgdx
    8ea8:	86 85       	ldaa	#0x85
    8eaa:	bd 91 fb    	jsr	0x91fb
    8ead:	7f 00 fc    	clr	0xfc
    8eb0:	bd 92 a5    	jsr	0x92a5
    8eb3:	4d          	tsta
    8eb4:	2a 03       	bpl	0x0x8eb9
    8eb6:	7e 82 86    	jmp	0x8286
    8eb9:	7c 00 fc    	inc	0xfc
    8ebc:	7e 8e 31    	jmp	0x8e31
    8ebf:	c1 05       	cmpb	#0x5
    8ec1:	27 e4       	beq	0x0x8ea7
    8ec3:	c1 07       	cmpb	#0x7
    8ec5:	26 08       	bne	0x0x8ecf
    8ec7:	37          	pshb
    8ec8:	d6 d6       	ldab	*0xd6
    8eca:	3d          	mul
    8ecb:	05          	asld
    8ecc:	33          	pulb
    8ecd:	20 d8       	bra	0x0x8ea7
    8ecf:	c1 40       	cmpb	#0x40
    8ed1:	27 d4       	beq	0x0x8ea7
    8ed3:	c1 41       	cmpb	#0x41
    8ed5:	27 d0       	beq	0x0x8ea7
    8ed7:	c1 36       	cmpb	#0x36
    8ed9:	25 0f       	bcs	0x0x8eea
    8edb:	c1 76       	cmpb	#0x76
    8edd:	22 0b       	bhi	0x0x8eea
    8edf:	ce 92 f9    	ldx	#0x92f9
    8ee2:	c0 36       	subb	#0x36
    8ee4:	58          	aslb
    8ee5:	3a          	abx
    8ee6:	ee 00       	ldx	0x0,x
    8ee8:	ad 00       	jsr	0x0,x
    8eea:	7e 8e ad    	jmp	0x8ead
    8eed:	97 df       	staa	*0xdf
    8eef:	7c 00 fc    	inc	0xfc
    8ef2:	bd 92 a5    	jsr	0x92a5
    8ef5:	36          	psha
    8ef6:	bd 92 a5    	jsr	0x92a5
    8ef9:	16          	tab
    8efa:	32          	pula
    8efb:	8f          	xgdx
    8efc:	86 84       	ldaa	#0x84
    8efe:	bd 91 fb    	jsr	0x91fb
    8f01:	7f 00 fc    	clr	0xfc
    8f04:	bd 92 a5    	jsr	0x92a5
    8f07:	4d          	tsta
    8f08:	2a 03       	bpl	0x0x8f0d
    8f0a:	7e 82 86    	jmp	0x8286
    8f0d:	7c 00 fc    	inc	0xfc
    8f10:	20 e3       	bra	0x0x8ef5
    8f12:	97 df       	staa	*0xdf
    8f14:	7c 00 fc    	inc	0xfc
    8f17:	bd 92 a5    	jsr	0x92a5
    8f1a:	36          	psha
    8f1b:	13 8e 02 08 	brclr	*0x8e, #0x02, 0x0x8f27
    8f1f:	16          	tab
    8f20:	4f          	clra
    8f21:	8f          	xgdx
    8f22:	86 87       	ldaa	#0x87
    8f24:	bd 91 fb    	jsr	0x91fb
    8f27:	33          	pulb
    8f28:	4f          	clra
    8f29:	8f          	xgdx
    8f2a:	86 86       	ldaa	#0x86
    8f2c:	bd 91 fb    	jsr	0x91fb
    8f2f:	7f 00 fc    	clr	0xfc
    8f32:	bd 92 a5    	jsr	0x92a5
    8f35:	4d          	tsta
    8f36:	2a 03       	bpl	0x0x8f3b
    8f38:	7e 82 86    	jmp	0x8286
    8f3b:	7c 00 fc    	inc	0xfc
    8f3e:	7e 8f 1a    	jmp	0x8f1a
    8f41:	97 df       	staa	*0xdf
    8f43:	7c 00 fc    	inc	0xfc
    8f46:	bd 92 a5    	jsr	0x92a5
    8f49:	7c 00 fc    	inc	0xfc
    8f4c:	7d 00 fe    	tst	0xfe
    8f4f:	27 02       	beq	0x0x8f53
    8f51:	20 45       	bra	0x0x8f98
    8f53:	d6 f9       	ldab	*0xf9
    8f55:	c1 04       	cmpb	#0x4
    8f57:	26 0d       	bne	0x0x8f66
    8f59:	d6 df       	ldab	*0xdf
    8f5b:	c4 0f       	andb	#0xf
    8f5d:	26 39       	bne	0x0x8f98
    8f5f:	d6 f9       	ldab	*0xf9
    8f61:	b7 01 6a    	staa	0x16a
    8f64:	20 02       	bra	0x0x8f68
    8f66:	97 fa       	staa	*0xfa
    8f68:	bd ad 9e    	jsr	0xad9e
    8f6b:	b7 7f f7    	staa	0x7ff7
    8f6e:	bd ad c4    	jsr	0xadc4
    8f71:	4f          	clra
    8f72:	97 fb       	staa	*0xfb
    8f74:	97 f2       	staa	*0xf2
    8f76:	97 ff       	staa	*0xff
    8f78:	b7 01 1b    	staa	0x11b
    8f7b:	bd a3 59    	jsr	0xa359
    8f7e:	7c 01 7e    	inc	0x17e
    8f81:	c1 04       	cmpb	#0x4
    8f83:	26 0d       	bne	0x0x8f92
    8f85:	bd 9f 0f    	jsr	0x9f0f
    8f88:	bd a5 8e    	jsr	0xa58e
    8f8b:	b6 50 00    	ldaa	0x5000
    8f8e:	97 d1       	staa	*0xd1
    8f90:	20 06       	bra	0x0x8f98
    8f92:	bd a3 70    	jsr	0xa370
    8f95:	14 d1 80    	bset	*0xd1, #0x80
    8f98:	7f 00 fc    	clr	0xfc
    8f9b:	bd 92 a5    	jsr	0x92a5
    8f9e:	4d          	tsta
    8f9f:	2a a8       	bpl	0x0x8f49
    8fa1:	7e 82 86    	jmp	0x8286
    8fa4:	97 df       	staa	*0xdf
    8fa6:	7c 00 fc    	inc	0xfc
    8fa9:	bd 92 a5    	jsr	0x92a5
    8fac:	81 00       	cmpa	#0x0
    8fae:	26 3b       	bne	0x0x8feb
    8fb0:	bd 92 a5    	jsr	0x92a5
    8fb3:	81 00       	cmpa	#0x0
    8fb5:	26 34       	bne	0x0x8feb
    8fb7:	bd 92 a5    	jsr	0x92a5
    8fba:	81 4d       	cmpa	#0x4d
    8fbc:	26 2d       	bne	0x0x8feb
    8fbe:	bd 92 a5    	jsr	0x92a5
    8fc1:	81 08       	cmpa	#0x8
    8fc3:	26 26       	bne	0x0x8feb
    8fc5:	bd 92 a5    	jsr	0x92a5
    8fc8:	4d          	tsta
    8fc9:	27 04       	beq	0x0x8fcf
    8fcb:	81 55       	cmpa	#0x55
    8fcd:	26 1c       	bne	0x0x8feb
    8fcf:	bd 92 a5    	jsr	0x92a5
    8fd2:	4d          	tsta
    8fd3:	27 07       	beq	0x0x8fdc
    8fd5:	81 2a       	cmpa	#0x2a
    8fd7:	26 12       	bne	0x0x8feb
    8fd9:	7e a9 ab    	jmp	0xa9ab
    8fdc:	bd 92 a5    	jsr	0x92a5
    8fdf:	81 7f       	cmpa	#0x7f
    8fe1:	27 05       	beq	0x0x8fe8
    8fe3:	bd 92 a5    	jsr	0x92a5
    8fe6:	20 0c       	bra	0x0x8ff4
    8fe8:	7e 90 6a    	jmp	0x906a
    8feb:	7f 00 fc    	clr	0xfc
    8fee:	7f 00 ff    	clr	0xff
    8ff1:	7e 82 83    	jmp	0x8283
    8ff4:	4d          	tsta
    8ff5:	27 36       	beq	0x0x902d
    8ff7:	81 01       	cmpa	#0x1
    8ff9:	27 1a       	beq	0x0x9015
    8ffb:	ce 00 00    	ldx	#0x0
    8ffe:	3c          	pshx
    8fff:	bd 92 a5    	jsr	0x92a5
    9002:	38          	pulx
    9003:	81 f7       	cmpa	#0xf7
    9005:	27 05       	beq	0x0x900c
    9007:	a7 00       	staa	0x0,x
    9009:	08          	inx
    900a:	20 f2       	bra	0x0x8ffe
    900c:	bd a3 bd    	jsr	0xa3bd
    900f:	7f 00 fc    	clr	0xfc
    9012:	7e 82 83    	jmp	0x8283
    9015:	96 f9       	ldaa	*0xf9
    9017:	81 04       	cmpa	#0x4
    9019:	26 07       	bne	0x0x9022
    901b:	b6 01 6a    	ldaa	0x16a
    901e:	c6 50       	ldab	#0x50
    9020:	20 04       	bra	0x0x9026
    9022:	96 fa       	ldaa	*0xfa
    9024:	c6 b0       	ldab	#0xb0
    9026:	3d          	mul
    9027:	c3 20 00    	addd	#0x2000
    902a:	8f          	xgdx
    902b:	20 03       	bra	0x0x9030
    902d:	ce 20 00    	ldx	#0x2000
    9030:	3c          	pshx
    9031:	bd 92 a5    	jsr	0x92a5
    9034:	38          	pulx
    9035:	81 f7       	cmpa	#0xf7
    9037:	27 05       	beq	0x0x903e
    9039:	a7 00       	staa	0x0,x
    903b:	08          	inx
    903c:	20 f2       	bra	0x0x9030
    903e:	4f          	clra
    903f:	97 fb       	staa	*0xfb
    9041:	97 f2       	staa	*0xf2
    9043:	97 ff       	staa	*0xff
    9045:	b7 01 1b    	staa	0x11b
    9048:	bd a3 59    	jsr	0xa359
    904b:	96 f9       	ldaa	*0xf9
    904d:	81 04       	cmpa	#0x4
    904f:	26 0a       	bne	0x0x905b
    9051:	bd a5 8e    	jsr	0xa58e
    9054:	b6 50 00    	ldaa	0x5000
    9057:	97 d1       	staa	*0xd1
    9059:	20 06       	bra	0x0x9061
    905b:	bd a3 70    	jsr	0xa370
    905e:	14 d1 80    	bset	*0xd1, #0x80
    9061:	7c 01 7e    	inc	0x17e
    9064:	7f 00 fc    	clr	0xfc
    9067:	7e 82 83    	jmp	0x8283
    906a:	bd 92 a5    	jsr	0x92a5
    906d:	8d 1a       	bsr	0x0x9089
    906f:	7f 00 ff    	clr	0xff
    9072:	7f 01 1b    	clr	0x11b
    9075:	bd a3 59    	jsr	0xa359
    9078:	96 f2       	ldaa	*0xf2
    907a:	84 20       	anda	#0x20
    907c:	97 f2       	staa	*0xf2
    907e:	86 80       	ldaa	#0x80
    9080:	b7 01 1c    	staa	0x11c
    9083:	7f 00 fc    	clr	0xfc
    9086:	7e 82 83    	jmp	0x8283
    9089:	36          	psha
    908a:	bd 91 eb    	jsr	0x91eb
    908d:	86 f0       	ldaa	#0xf0
    908f:	b7 10 2f    	staa	0x102f
    9092:	86 00       	ldaa	#0x0
    9094:	bd 91 eb    	jsr	0x91eb
    9097:	b7 10 2f    	staa	0x102f
    909a:	bd 91 eb    	jsr	0x91eb
    909d:	b7 10 2f    	staa	0x102f
    90a0:	86 4d       	ldaa	#0x4d
    90a2:	bd 91 eb    	jsr	0x91eb
    90a5:	b7 10 2f    	staa	0x102f
    90a8:	86 08       	ldaa	#0x8
    90aa:	bd 91 eb    	jsr	0x91eb
    90ad:	b7 10 2f    	staa	0x102f
    90b0:	bd 91 eb    	jsr	0x91eb
    90b3:	4f          	clra
    90b4:	b7 10 2f    	staa	0x102f
    90b7:	bd 91 eb    	jsr	0x91eb
    90ba:	b7 10 2f    	staa	0x102f
    90bd:	bd 91 eb    	jsr	0x91eb
    90c0:	b7 10 2f    	staa	0x102f
    90c3:	bd 91 eb    	jsr	0x91eb
    90c6:	32          	pula
    90c7:	b7 10 2f    	staa	0x102f
    90ca:	27 6c       	beq	0x0x9138
    90cc:	81 01       	cmpa	#0x1
    90ce:	27 10       	beq	0x0x90e0
    90d0:	ce 00 20    	ldx	#0x20
    90d3:	c6 b0       	ldab	#0xb0
    90d5:	8d 4f       	bsr	0x0x9126
    90d7:	bd 91 eb    	jsr	0x91eb
    90da:	86 f7       	ldaa	#0xf7
    90dc:	b7 10 2f    	staa	0x102f
    90df:	39          	rts
    90e0:	96 fa       	ldaa	*0xfa
    90e2:	d6 f9       	ldab	*0xf9
    90e4:	c1 04       	cmpb	#0x4
    90e6:	26 03       	bne	0x0x90eb
    90e8:	b6 01 6a    	ldaa	0x16a
    90eb:	f6 01 1b    	ldab	0x11b
    90ee:	c1 05       	cmpb	#0x5
    90f0:	26 07       	bne	0x0x90f9
    90f2:	ce 01 3c    	ldx	#0x13c
    90f5:	bd ec 16    	jsr	0xec16
    90f8:	4a          	deca
    90f9:	d6 f9       	ldab	*0xf9
    90fb:	c1 01       	cmpb	#0x1
    90fd:	22 03       	bhi	0x0x9102
    90ff:	7e 91 bc    	jmp	0x91bc
    9102:	c1 04       	cmpb	#0x4
    9104:	26 04       	bne	0x0x910a
    9106:	c6 50       	ldab	#0x50
    9108:	20 02       	bra	0x0x910c
    910a:	c6 b0       	ldab	#0xb0
    910c:	3d          	mul
    910d:	c3 20 00    	addd	#0x2000
    9110:	8f          	xgdx
    9111:	c6 b0       	ldab	#0xb0
    9113:	96 f9       	ldaa	*0xf9
    9115:	81 04       	cmpa	#0x4
    9117:	26 02       	bne	0x0x911b
    9119:	c6 50       	ldab	#0x50
    911b:	8d 09       	bsr	0x0x9126
    911d:	bd 91 eb    	jsr	0x91eb
    9120:	86 f7       	ldaa	#0xf7
    9122:	b7 10 2f    	staa	0x102f
    9125:	39          	rts
    9126:	a6 00       	ldaa	0x0,x
    9128:	84 7f       	anda	#0x7f
    912a:	bd 91 eb    	jsr	0x91eb
    912d:	bd 91 f3    	jsr	0x91f3
    9130:	b7 10 2f    	staa	0x102f
    9133:	08          	inx
    9134:	5a          	decb
    9135:	26 ef       	bne	0x0x9126
    9137:	39          	rts
    9138:	86 ff       	ldaa	#0xff
    913a:	97 f8       	staa	*0xf8
    913c:	96 f9       	ldaa	*0xf9
    913e:	81 01       	cmpa	#0x1
    9140:	22 06       	bhi	0x0x9148
    9142:	7f 01 1f    	clr	0x11f
    9145:	7e 91 72    	jmp	0x9172
    9148:	ce 20 00    	ldx	#0x2000
    914b:	18 ce 00 10 	ldy	#0x10
    914f:	c6 b0       	ldab	#0xb0
    9151:	96 f9       	ldaa	*0xf9
    9153:	81 04       	cmpa	#0x4
    9155:	26 02       	bne	0x0x9159
    9157:	c6 50       	ldab	#0x50
    9159:	8d cb       	bsr	0x0x9126
    915b:	18 09       	dey
    915d:	26 f0       	bne	0x0x914f
    915f:	96 f8       	ldaa	*0xf8
    9161:	44          	lsra
    9162:	27 04       	beq	0x0x9168
    9164:	97 f8       	staa	*0xf8
    9166:	20 e3       	bra	0x0x914b
    9168:	97 f8       	staa	*0xf8
    916a:	8d 7f       	bsr	0x0x91eb
    916c:	86 f7       	ldaa	#0xf7
    916e:	b7 10 2f    	staa	0x102f
    9171:	39          	rts
    9172:	18 ce 00 10 	ldy	#0x10
    9176:	18 3c       	pshy
    9178:	5f          	clrb
    9179:	f7 10 22    	stab	0x1022
    917c:	f6 10 2d    	ldab	0x102d
    917f:	37          	pshb
    9180:	c4 7f       	andb	#0x7f
    9182:	f7 10 2d    	stab	0x102d
    9185:	96 f9       	ldaa	*0xf9
    9187:	f6 01 1f    	ldab	0x11f
    918a:	bd 79 00    	jsr	0x7900
    918d:	32          	pula
    918e:	b7 10 2d    	staa	0x102d
    9191:	86 80       	ldaa	#0x80
    9193:	b7 10 22    	staa	0x1022
    9196:	ce 00 20    	ldx	#0x20
    9199:	c6 b0       	ldab	#0xb0
    919b:	8d 89       	bsr	0x0x9126
    919d:	7c 01 1f    	inc	0x11f
    91a0:	18 38       	puly
    91a2:	18 09       	dey
    91a4:	26 d0       	bne	0x0x9176
    91a6:	96 f8       	ldaa	*0xf8
    91a8:	44          	lsra
    91a9:	27 04       	beq	0x0x91af
    91ab:	97 f8       	staa	*0xf8
    91ad:	20 c3       	bra	0x0x9172
    91af:	97 f8       	staa	*0xf8
    91b1:	bd 91 eb    	jsr	0x91eb
    91b4:	86 f7       	ldaa	#0xf7
    91b6:	b7 10 2f    	staa	0x102f
    91b9:	7e a3 70    	jmp	0xa370
    91bc:	5f          	clrb
    91bd:	f7 10 22    	stab	0x1022
    91c0:	f6 10 2d    	ldab	0x102d
    91c3:	37          	pshb
    91c4:	c4 7f       	andb	#0x7f
    91c6:	f7 10 2d    	stab	0x102d
    91c9:	16          	tab
    91ca:	96 f9       	ldaa	*0xf9
    91cc:	bd 79 00    	jsr	0x7900
    91cf:	32          	pula
    91d0:	b7 10 2d    	staa	0x102d
    91d3:	86 80       	ldaa	#0x80
    91d5:	b7 10 22    	staa	0x1022
    91d8:	ce 00 20    	ldx	#0x20
    91db:	c6 b0       	ldab	#0xb0
    91dd:	bd 91 26    	jsr	0x9126
    91e0:	bd 91 eb    	jsr	0x91eb
    91e3:	86 f7       	ldaa	#0xf7
    91e5:	b7 10 2f    	staa	0x102f
    91e8:	7e a3 70    	jmp	0xa370
    91eb:	37          	pshb
    91ec:	f6 10 2e    	ldab	0x102e
    91ef:	2a fb       	bpl	0x0x91ec
    91f1:	33          	pulb
    91f2:	39          	rts
    91f3:	37          	pshb
    91f4:	c6 4d       	ldab	#0x4d
    91f6:	5a          	decb
    91f7:	26 fd       	bne	0x0x91f6
    91f9:	33          	pulb
    91fa:	39          	rts
    91fb:	f6 10 22    	ldab	0x1022
    91fe:	c4 f7       	andb	#0xf7
    9200:	f7 10 22    	stab	0x1022
    9203:	7d 00 d0    	tst	0xd0
    9206:	27 05       	beq	0x0x920d
    9208:	7f 00 d0    	clr	0xd0
    920b:	20 08       	bra	0x0x9215
    920d:	7d 10 29    	tst	0x1029
    9210:	2a fb       	bpl	0x0x920d
    9212:	f6 10 2a    	ldab	0x102a
    9215:	d6 d1       	ldab	*0xd1
    9217:	2b 2f       	bmi	0x0x9248
    9219:	27 2d       	beq	0x0x9248
    921b:	c1 05       	cmpb	#0x5
    921d:	27 1d       	beq	0x0x923c
    921f:	22 15       	bhi	0x0x9236
    9221:	f6 01 6b    	ldab	0x16b
    9224:	18 ce 50 50 	ldy	#0x5050
    9228:	18 3a       	aby
    922a:	18 e6 00    	ldab	0x0,y
    922d:	26 04       	bne	0x0x9233
    922f:	7c 00 d0    	inc	0xd0
    9232:	39          	rts
    9233:	53          	comb
    9234:	20 13       	bra	0x0x9249
    9236:	d6 df       	ldab	*0xdf
    9238:	c4 0f       	andb	#0xf
    923a:	20 e8       	bra	0x0x9224
    923c:	8f          	xgdx
    923d:	c1 07       	cmpb	#0x7
    923f:	26 06       	bne	0x0x9247
    9241:	8f          	xgdx
    9242:	f6 01 6b    	ldab	0x16b
    9245:	20 dd       	bra	0x0x9224
    9247:	8f          	xgdx
    9248:	5f          	clrb
    9249:	01          	nop
    924a:	01          	nop
    924b:	01          	nop
    924c:	01          	nop
    924d:	f7 10 42    	stab	0x1042
    9250:	01          	nop
    9251:	01          	nop
    9252:	01          	nop
    9253:	01          	nop
    9254:	01          	nop
    9255:	01          	nop
    9256:	01          	nop
    9257:	b7 10 2a    	staa	0x102a
    925a:	7d 10 29    	tst	0x1029
    925d:	2a fb       	bpl	0x0x925a
    925f:	b6 10 2a    	ldaa	0x102a
    9262:	8f          	xgdx
    9263:	f7 10 2a    	stab	0x102a
    9266:	37          	pshb
    9267:	7d 10 29    	tst	0x1029
    926a:	2a fb       	bpl	0x0x9267
    926c:	f6 10 2a    	ldab	0x102a
    926f:	b7 10 2a    	staa	0x102a
    9272:	33          	pulb
    9273:	12 d1 80 21 	brset	*0xd1, #0x80, 0x0x9298
    9277:	c1 44       	cmpb	#0x44
    9279:	26 1d       	bne	0x0x9298
    927b:	ce 50 03    	ldx	#0x5003
    927e:	f6 01 6b    	ldab	0x16b
    9281:	86 04       	ldaa	#0x4
    9283:	3d          	mul
    9284:	3a          	abx
    9285:	a6 00       	ldaa	0x0,x
    9287:	84 40       	anda	#0x40
    9289:	26 0d       	bne	0x0x9298
    928b:	7d 10 29    	tst	0x1029
    928e:	2a fb       	bpl	0x0x928b
    9290:	b6 10 2a    	ldaa	0x102a
    9293:	86 8c       	ldaa	#0x8c
    9295:	b7 10 2a    	staa	0x102a
    9298:	13 f3 10 08 	brclr	*0xf3, #0x10, 0x0x92a4
    929c:	f6 10 22    	ldab	0x1022
    929f:	ca 08       	orab	#0x8
    92a1:	f7 10 22    	stab	0x1022
    92a4:	39          	rts
    92a5:	0f          	sei
    92a6:	fe 01 c0    	ldx	0x1c0
    92a9:	bc 01 c2    	cpx	0x1c2
    92ac:	26 10       	bne	0x0x92be
    92ae:	7d 00 fc    	tst	0xfc
    92b1:	27 03       	beq	0x0x92b6
    92b3:	0e          	cli
    92b4:	20 f0       	bra	0x0x92a6
    92b6:	38          	pulx
    92b7:	ff 01 6c    	stx	0x16c
    92ba:	0e          	cli
    92bb:	7e 97 90    	jmp	0x9790
    92be:	0f          	sei
    92bf:	a6 00       	ldaa	0x0,x
    92c1:	8f          	xgdx
    92c2:	5c          	incb
    92c3:	c4 bf       	andb	#0xbf
    92c5:	8f          	xgdx
    92c6:	ff 01 c0    	stx	0x1c0
    92c9:	0e          	cli
    92ca:	81 fa       	cmpa	#0xfa
    92cc:	27 05       	beq	0x0x92d3
    92ce:	81 f8       	cmpa	#0xf8
    92d0:	27 01       	beq	0x0x92d3
    92d2:	39          	rts
    92d3:	7d 00 d0    	tst	0xd0
    92d6:	27 05       	beq	0x0x92dd
    92d8:	7f 00 d0    	clr	0xd0
    92db:	20 08       	bra	0x0x92e5
    92dd:	7d 10 29    	tst	0x1029
    92e0:	2a fb       	bpl	0x0x92dd
    92e2:	f6 10 2a    	ldab	0x102a
    92e5:	5f          	clrb
    92e6:	01          	nop
    92e7:	01          	nop
    92e8:	01          	nop
    92e9:	01          	nop
    92ea:	f7 10 42    	stab	0x1042
    92ed:	01          	nop
    92ee:	01          	nop
    92ef:	01          	nop
    92f0:	01          	nop
    92f1:	01          	nop
    92f2:	01          	nop
    92f3:	01          	nop
    92f4:	b7 10 2a    	staa	0x102a
    92f7:	20 ac       	bra	0x0x92a5
    92f9:	93 7b       	subd	*0x7b
    92fb:	93 82       	subd	*0x82
    92fd:	93 ab       	subd	*0xab
    92ff:	93 d4       	subd	*0xd4
    9301:	93 fd       	subd	*0xfd
    9303:	94 26       	anda	*0x26
    9305:	94 2b       	anda	*0x2b
    9307:	94 32       	anda	*0x32
    9309:	94 5b       	anda	*0x5b
    930b:	94 84       	anda	*0x84
    930d:	94 ad       	anda	*0xad
    930f:	94 ad       	anda	*0xad
    9311:	94 ad       	anda	*0xad
    9313:	94 ad       	anda	*0xad
    9315:	94 ad       	anda	*0xad
    9317:	94 ad       	anda	*0xad
    9319:	94 ae       	anda	*0xae
    931b:	94 b3       	anda	*0xb3
    931d:	94 b8       	anda	*0xb8
    931f:	94 bd       	anda	*0xbd
    9321:	94 c2       	anda	*0xc2
    9323:	94 ad       	anda	*0xad
    9325:	94 ad       	anda	*0xad
    9327:	94 ad       	anda	*0xad
    9329:	94 ad       	anda	*0xad
    932b:	94 ad       	anda	*0xad
    932d:	94 ad       	anda	*0xad
    932f:	94 ad       	anda	*0xad
    9331:	94 ad       	anda	*0xad
    9333:	94 ad       	anda	*0xad
    9335:	94 c7       	anda	*0xc7
    9337:	94 cc       	anda	*0xcc
    9339:	94 d1       	anda	*0xd1
    933b:	94 d6       	anda	*0xd6
    933d:	94 db       	anda	*0xdb
    933f:	94 e0       	anda	*0xe0
    9341:	94 e5       	anda	*0xe5
    9343:	94 ad       	anda	*0xad
    9345:	94 ad       	anda	*0xad
    9347:	94 ad       	anda	*0xad
    9349:	94 ad       	anda	*0xad
    934b:	94 ad       	anda	*0xad
    934d:	94 ad       	anda	*0xad
    934f:	94 ad       	anda	*0xad
    9351:	94 ea       	anda	*0xea
    9353:	94 ef       	anda	*0xef
    9355:	94 ad       	anda	*0xad
    9357:	94 ad       	anda	*0xad
    9359:	94 f4       	anda	*0xf4
    935b:	94 f9       	anda	*0xf9
    935d:	94 fe       	anda	*0xfe
    935f:	95 03       	bita	*0x3
    9361:	95 08       	bita	*0x8
    9363:	95 0d       	bita	*0xd
    9365:	95 12       	bita	*0x12
    9367:	95 17       	bita	*0x17
    9369:	95 1c       	bita	*0x1c
    936b:	95 21       	bita	*0x21
    936d:	95 26       	bita	*0x26
    936f:	95 2b       	bita	*0x2b
    9371:	95 30       	bita	*0x30
    9373:	95 35       	bita	*0x35
    9375:	95 3a       	bita	*0x3a
    9377:	95 3d       	bita	*0x3d
    9379:	95 4f       	bita	*0x4f
    937b:	84 3f       	anda	#0x3f
    937d:	c6 3e       	ldab	#0x3e
    937f:	7e 95 78    	jmp	0x9578
    9382:	c6 44       	ldab	#0x44
    9384:	4d          	tsta
    9385:	27 12       	beq	0x0x9399
    9387:	96 44       	ldaa	*0x44
    9389:	8a 01       	oraa	#0x1
    938b:	bd 95 a7    	jsr	0x95a7
    938e:	97 44       	staa	*0x44
    9390:	14 f7 01    	bset	*0xf7, #0x01
    9393:	8f          	xgdx
    9394:	86 83       	ldaa	#0x83
    9396:	7e 91 fb    	jmp	0x91fb
    9399:	96 44       	ldaa	*0x44
    939b:	84 fe       	anda	#0xfe
    939d:	bd 95 a7    	jsr	0x95a7
    93a0:	97 44       	staa	*0x44
    93a2:	15 f7 01    	bclr	*0xf7, #0x01
    93a5:	8f          	xgdx
    93a6:	86 83       	ldaa	#0x83
    93a8:	7e 91 fb    	jmp	0x91fb
    93ab:	c6 44       	ldab	#0x44
    93ad:	4d          	tsta
    93ae:	27 12       	beq	0x0x93c2
    93b0:	96 44       	ldaa	*0x44
    93b2:	8a 02       	oraa	#0x2
    93b4:	bd 95 a7    	jsr	0x95a7
    93b7:	97 44       	staa	*0x44
    93b9:	14 f7 02    	bset	*0xf7, #0x02
    93bc:	8f          	xgdx
    93bd:	86 83       	ldaa	#0x83
    93bf:	7e 91 fb    	jmp	0x91fb
    93c2:	96 44       	ldaa	*0x44
    93c4:	84 fd       	anda	#0xfd
    93c6:	bd 95 a7    	jsr	0x95a7
    93c9:	97 44       	staa	*0x44
    93cb:	15 f7 02    	bclr	*0xf7, #0x02
    93ce:	8f          	xgdx
    93cf:	86 83       	ldaa	#0x83
    93d1:	7e 91 fb    	jmp	0x91fb
    93d4:	c6 44       	ldab	#0x44
    93d6:	4d          	tsta
    93d7:	27 12       	beq	0x0x93eb
    93d9:	96 44       	ldaa	*0x44
    93db:	8a 04       	oraa	#0x4
    93dd:	bd 95 a7    	jsr	0x95a7
    93e0:	97 44       	staa	*0x44
    93e2:	14 f7 04    	bset	*0xf7, #0x04
    93e5:	8f          	xgdx
    93e6:	86 83       	ldaa	#0x83
    93e8:	7e 91 fb    	jmp	0x91fb
    93eb:	96 44       	ldaa	*0x44
    93ed:	84 fb       	anda	#0xfb
    93ef:	bd 95 a7    	jsr	0x95a7
    93f2:	97 44       	staa	*0x44
    93f4:	15 f7 04    	bclr	*0xf7, #0x04
    93f7:	8f          	xgdx
    93f8:	86 83       	ldaa	#0x83
    93fa:	7e 91 fb    	jmp	0x91fb
    93fd:	c6 44       	ldab	#0x44
    93ff:	4d          	tsta
    9400:	27 12       	beq	0x0x9414
    9402:	96 44       	ldaa	*0x44
    9404:	8a 08       	oraa	#0x8
    9406:	bd 95 a7    	jsr	0x95a7
    9409:	97 44       	staa	*0x44
    940b:	14 f7 08    	bset	*0xf7, #0x08
    940e:	8f          	xgdx
    940f:	86 83       	ldaa	#0x83
    9411:	7e 91 fb    	jmp	0x91fb
    9414:	96 44       	ldaa	*0x44
    9416:	84 f7       	anda	#0xf7
    9418:	bd 95 a7    	jsr	0x95a7
    941b:	97 44       	staa	*0x44
    941d:	15 f7 08    	bclr	*0xf7, #0x08
    9420:	8f          	xgdx
    9421:	86 83       	ldaa	#0x83
    9423:	7e 91 fb    	jmp	0x91fb
    9426:	c6 42       	ldab	#0x42
    9428:	7e 95 78    	jmp	0x9578
    942b:	84 3f       	anda	#0x3f
    942d:	c6 3f       	ldab	#0x3f
    942f:	7e 95 78    	jmp	0x9578
    9432:	c6 44       	ldab	#0x44
    9434:	4d          	tsta
    9435:	27 12       	beq	0x0x9449
    9437:	96 44       	ldaa	*0x44
    9439:	8a 10       	oraa	#0x10
    943b:	bd 95 a7    	jsr	0x95a7
    943e:	97 44       	staa	*0x44
    9440:	14 f7 10    	bset	*0xf7, #0x10
    9443:	8f          	xgdx
    9444:	86 83       	ldaa	#0x83
    9446:	7e 91 fb    	jmp	0x91fb
    9449:	96 44       	ldaa	*0x44
    944b:	84 ef       	anda	#0xef
    944d:	bd 95 a7    	jsr	0x95a7
    9450:	97 44       	staa	*0x44
    9452:	15 f7 10    	bclr	*0xf7, #0x10
    9455:	8f          	xgdx
    9456:	86 83       	ldaa	#0x83
    9458:	7e 91 fb    	jmp	0x91fb
    945b:	c6 44       	ldab	#0x44
    945d:	4d          	tsta
    945e:	27 12       	beq	0x0x9472
    9460:	96 44       	ldaa	*0x44
    9462:	8a 20       	oraa	#0x20
    9464:	bd 95 a7    	jsr	0x95a7
    9467:	97 44       	staa	*0x44
    9469:	14 f7 20    	bset	*0xf7, #0x20
    946c:	8f          	xgdx
    946d:	86 83       	ldaa	#0x83
    946f:	7e 91 fb    	jmp	0x91fb
    9472:	96 44       	ldaa	*0x44
    9474:	84 df       	anda	#0xdf
    9476:	bd 95 a7    	jsr	0x95a7
    9479:	97 44       	staa	*0x44
    947b:	15 f7 20    	bclr	*0xf7, #0x20
    947e:	8f          	xgdx
    947f:	86 83       	ldaa	#0x83
    9481:	7e 91 fb    	jmp	0x91fb
    9484:	c6 44       	ldab	#0x44
    9486:	4d          	tsta
    9487:	27 12       	beq	0x0x949b
    9489:	96 44       	ldaa	*0x44
    948b:	8a 40       	oraa	#0x40
    948d:	bd 95 a7    	jsr	0x95a7
    9490:	97 44       	staa	*0x44
    9492:	14 f7 40    	bset	*0xf7, #0x40
    9495:	8f          	xgdx
    9496:	86 83       	ldaa	#0x83
    9498:	7e 91 fb    	jmp	0x91fb
    949b:	96 44       	ldaa	*0x44
    949d:	84 bf       	anda	#0xbf
    949f:	bd 95 a7    	jsr	0x95a7
    94a2:	97 44       	staa	*0x44
    94a4:	15 f7 40    	bclr	*0xf7, #0x40
    94a7:	8f          	xgdx
    94a8:	86 83       	ldaa	#0x83
    94aa:	7e 91 fb    	jmp	0x91fb
    94ad:	39          	rts
    94ae:	c6 43       	ldab	#0x43
    94b0:	7e 95 78    	jmp	0x9578
    94b3:	c6 28       	ldab	#0x28
    94b5:	7e 95 78    	jmp	0x9578
    94b8:	c6 2a       	ldab	#0x2a
    94ba:	7e 95 78    	jmp	0x9578
    94bd:	c6 35       	ldab	#0x35
    94bf:	7e 95 78    	jmp	0x9578
    94c2:	c6 37       	ldab	#0x37
    94c4:	7e 95 78    	jmp	0x9578
    94c7:	c6 45       	ldab	#0x45
    94c9:	7e 95 78    	jmp	0x9578
    94cc:	c6 46       	ldab	#0x46
    94ce:	7e 95 78    	jmp	0x9578
    94d1:	c6 47       	ldab	#0x47
    94d3:	7e 95 78    	jmp	0x9578
    94d6:	c6 49       	ldab	#0x49
    94d8:	7e 95 78    	jmp	0x9578
    94db:	c6 4c       	ldab	#0x4c
    94dd:	7e 95 78    	jmp	0x9578
    94e0:	c6 4d       	ldab	#0x4d
    94e2:	7e 95 78    	jmp	0x9578
    94e5:	c6 4e       	ldab	#0x4e
    94e7:	7e 95 78    	jmp	0x9578
    94ea:	c6 48       	ldab	#0x48
    94ec:	7e 95 78    	jmp	0x9578
    94ef:	c6 6f       	ldab	#0x6f
    94f1:	7e 95 78    	jmp	0x9578
    94f4:	c6 4f       	ldab	#0x4f
    94f6:	7e 95 78    	jmp	0x9578
    94f9:	c6 51       	ldab	#0x51
    94fb:	7e 95 78    	jmp	0x9578
    94fe:	c6 52       	ldab	#0x52
    9500:	7e 95 78    	jmp	0x9578
    9503:	c6 53       	ldab	#0x53
    9505:	7e 95 78    	jmp	0x9578
    9508:	c6 55       	ldab	#0x55
    950a:	7e 95 78    	jmp	0x9578
    950d:	c6 56       	ldab	#0x56
    950f:	7e 95 78    	jmp	0x9578
    9512:	c6 57       	ldab	#0x57
    9514:	7e 95 78    	jmp	0x9578
    9517:	c6 58       	ldab	#0x58
    9519:	7e 95 78    	jmp	0x9578
    951c:	c6 5a       	ldab	#0x5a
    951e:	7e 95 78    	jmp	0x9578
    9521:	c6 63       	ldab	#0x63
    9523:	7e 95 78    	jmp	0x9578
    9526:	c6 5b       	ldab	#0x5b
    9528:	7e 95 78    	jmp	0x9578
    952b:	c6 5c       	ldab	#0x5c
    952d:	7e 95 78    	jmp	0x9578
    9530:	c6 5d       	ldab	#0x5d
    9532:	7e 95 78    	jmp	0x9578
    9535:	c6 5f       	ldab	#0x5f
    9537:	7e 95 78    	jmp	0x9578
    953a:	97 db       	staa	*0xdb
    953c:	39          	rts
    953d:	4d          	tsta
    953e:	26 08       	bne	0x0x9548
    9540:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x9547
    9544:	7e 9b b7    	jmp	0x9bb7
    9547:	39          	rts
    9548:	12 f3 10 fb 	brset	*0xf3, #0x10, 0x0x9547
    954c:	7e 9c 59    	jmp	0x9c59
    954f:	c6 41       	ldab	#0x41
    9551:	4d          	tsta
    9552:	27 12       	beq	0x0x9566
    9554:	96 41       	ldaa	*0x41
    9556:	8a 10       	oraa	#0x10
    9558:	bd 95 a7    	jsr	0x95a7
    955b:	97 41       	staa	*0x41
    955d:	14 f7 80    	bset	*0xf7, #0x80
    9560:	8f          	xgdx
    9561:	86 83       	ldaa	#0x83
    9563:	7e 91 fb    	jmp	0x91fb
    9566:	96 41       	ldaa	*0x41
    9568:	84 ef       	anda	#0xef
    956a:	bd 95 a7    	jsr	0x95a7
    956d:	97 41       	staa	*0x41
    956f:	15 f7 80    	bclr	*0xf7, #0x80
    9572:	8f          	xgdx
    9573:	86 83       	ldaa	#0x83
    9575:	7e 91 fb    	jmp	0x91fb
    9578:	7d 00 d1    	tst	0xd1
    957b:	2a 0e       	bpl	0x0x958b
    957d:	ce 00 00    	ldx	#0x0
    9580:	3a          	abx
    9581:	a7 00       	staa	0x0,x
    9583:	c0 20       	subb	#0x20
    9585:	8f          	xgdx
    9586:	86 83       	ldaa	#0x83
    9588:	7e 91 fb    	jmp	0x91fb
    958b:	36          	psha
    958c:	96 d1       	ldaa	*0xd1
    958e:	81 05       	cmpa	#0x5
    9590:	22 03       	bhi	0x0x9595
    9592:	32          	pula
    9593:	20 e8       	bra	0x0x957d
    9595:	96 df       	ldaa	*0xdf
    9597:	84 0f       	anda	#0xf
    9599:	b1 01 6b    	cmpa	0x16b
    959c:	32          	pula
    959d:	27 de       	beq	0x0x957d
    959f:	c0 20       	subb	#0x20
    95a1:	8f          	xgdx
    95a2:	86 83       	ldaa	#0x83
    95a4:	7e 91 fb    	jmp	0x91fb
    95a7:	c0 20       	subb	#0x20
    95a9:	7d 00 d1    	tst	0xd1
    95ac:	2a 01       	bpl	0x0x95af
    95ae:	39          	rts
    95af:	36          	psha
    95b0:	96 d1       	ldaa	*0xd1
    95b2:	81 05       	cmpa	#0x5
    95b4:	22 02       	bhi	0x0x95b8
    95b6:	32          	pula
    95b7:	39          	rts
    95b8:	96 df       	ldaa	*0xdf
    95ba:	84 0f       	anda	#0xf
    95bc:	b1 01 6b    	cmpa	0x16b
    95bf:	32          	pula
    95c0:	27 ec       	beq	0x0x95ae
    95c2:	38          	pulx
    95c3:	39          	rts
    95c4:	86 08       	ldaa	#0x8
    95c6:	b7 10 23    	staa	0x1023
    95c9:	fc 10 0e    	ldd	0x100e
    95cc:	c3 4e 20    	addd	#0x4e20
    95cf:	fd 10 1e    	std	0x101e
    95d2:	0e          	cli
    95d3:	96 db       	ldaa	*0xdb
    95d5:	4c          	inca
    95d6:	8d 01       	bsr	0x0x95d9
    95d8:	3b          	rti
    95d9:	7d 00 d2    	tst	0xd2
    95dc:	2b 12       	bmi	0x0x95f0
    95de:	bb 01 72    	adda	0x172
    95e1:	25 04       	bcs	0x0x95e7
    95e3:	81 f0       	cmpa	#0xf0
    95e5:	25 10       	bcs	0x0x95f7
    95e7:	14 d2 80    	bset	*0xd2, #0x80
    95ea:	86 f0       	ldaa	#0xf0
    95ec:	b7 01 72    	staa	0x172
    95ef:	39          	rts
    95f0:	16          	tab
    95f1:	b6 01 72    	ldaa	0x172
    95f4:	10          	sba
    95f5:	23 04       	bls	0x0x95fb
    95f7:	b7 01 72    	staa	0x172
    95fa:	39          	rts
    95fb:	7f 00 d2    	clr	0xd2
    95fe:	4f          	clra
    95ff:	b7 01 72    	staa	0x172
    9602:	f6 10 0f    	ldab	0x100f
    9605:	c4 07       	andb	#0x7
    9607:	d1 f0       	cmpb	*0xf0
    9609:	23 02       	bls	0x0x960d
    960b:	d6 f0       	ldab	*0xf0
    960d:	f7 01 74    	stab	0x174
    9610:	7d 00 f8    	tst	0xf8
    9613:	27 3f       	beq	0x0x9654
    9615:	7d 00 d0    	tst	0xd0
    9618:	27 05       	beq	0x0x961f
    961a:	7f 00 d0    	clr	0xd0
    961d:	20 08       	bra	0x0x9627
    961f:	7d 10 29    	tst	0x1029
    9622:	2a fb       	bpl	0x0x961f
    9624:	f6 10 2a    	ldab	0x102a
    9627:	f6 01 75    	ldab	0x175
    962a:	01          	nop
    962b:	01          	nop
    962c:	01          	nop
    962d:	01          	nop
    962e:	f7 10 42    	stab	0x1042
    9631:	01          	nop
    9632:	01          	nop
    9633:	01          	nop
    9634:	01          	nop
    9635:	01          	nop
    9636:	01          	nop
    9637:	01          	nop
    9638:	86 80       	ldaa	#0x80
    963a:	b7 10 2a    	staa	0x102a
    963d:	7f 00 f8    	clr	0xf8
    9640:	0d          	sec
    9641:	59          	rolb
    9642:	f7 01 75    	stab	0x175
    9645:	4f          	clra
    9646:	4c          	inca
    9647:	54          	lsrb
    9648:	25 fc       	bcs	0x0x9646
    964a:	4a          	deca
    964b:	91 f0       	cmpa	*0xf0
    964d:	23 05       	bls	0x0x9654
    964f:	86 fe       	ldaa	#0xfe
    9651:	b7 01 75    	staa	0x175
    9654:	96 73       	ldaa	*0x73
    9656:	81 01       	cmpa	#0x1
    9658:	26 68       	bne	0x0x96c2
    965a:	ce 01 00    	ldx	#0x100
    965d:	f6 01 0b    	ldab	0x10b
    9660:	3a          	abx
    9661:	a6 00       	ldaa	0x0,x
    9663:	26 0b       	bne	0x0x9670
    9665:	7f 01 0b    	clr	0x10b
    9668:	b6 01 00    	ldaa	0x100
    966b:	26 03       	bne	0x0x9670
    966d:	7e 97 84    	jmp	0x9784
    9670:	d6 d3       	ldab	*0xd3
    9672:	27 0b       	beq	0x0x967f
    9674:	8b 0c       	adda	#0xc
    9676:	2a 04       	bpl	0x0x967c
    9678:	80 0c       	suba	#0xc
    967a:	20 03       	bra	0x0x967f
    967c:	5a          	decb
    967d:	26 f5       	bne	0x0x9674
    967f:	7c 01 0b    	inc	0x10b
    9682:	f6 01 76    	ldab	0x176
    9685:	f1 01 0b    	cmpb	0x10b
    9688:	24 35       	bcc	0x0x96bf
    968a:	7d 00 74    	tst	0x74
    968d:	27 14       	beq	0x0x96a3
    968f:	7c 00 d3    	inc	0xd3
    9692:	d6 74       	ldab	*0x74
    9694:	d1 d3       	cmpb	*0xd3
    9696:	24 24       	bcc	0x0x96bc
    9698:	d6 73       	ldab	*0x73
    969a:	c1 03       	cmpb	#0x3
    969c:	27 0b       	beq	0x0x96a9
    969e:	7f 00 d3    	clr	0xd3
    96a1:	20 19       	bra	0x0x96bc
    96a3:	d6 73       	ldab	*0x73
    96a5:	c1 03       	cmpb	#0x3
    96a7:	26 13       	bne	0x0x96bc
    96a9:	14 30 80    	bset	*0x30, #0x80
    96ac:	7a 00 d3    	dec	0xd3
    96af:	2a 03       	bpl	0x0x96b4
    96b1:	7f 00 d3    	clr	0xd3
    96b4:	7a 01 0b    	dec	0x10b
    96b7:	7a 01 0b    	dec	0x10b
    96ba:	2a 03       	bpl	0x0x96bf
    96bc:	7f 01 0b    	clr	0x10b
    96bf:	7e 97 3c    	jmp	0x973c
    96c2:	81 02       	cmpa	#0x2
    96c4:	26 69       	bne	0x0x972f
    96c6:	ce 01 00    	ldx	#0x100
    96c9:	f6 01 0b    	ldab	0x10b
    96cc:	3a          	abx
    96cd:	a6 00       	ldaa	0x0,x
    96cf:	26 11       	bne	0x0x96e2
    96d1:	ce 01 00    	ldx	#0x100
    96d4:	f6 01 76    	ldab	0x176
    96d7:	f7 01 0b    	stab	0x10b
    96da:	3a          	abx
    96db:	a6 00       	ldaa	0x0,x
    96dd:	26 03       	bne	0x0x96e2
    96df:	7e 97 84    	jmp	0x9784
    96e2:	d6 d3       	ldab	*0xd3
    96e4:	27 0b       	beq	0x0x96f1
    96e6:	8b 0c       	adda	#0xc
    96e8:	2a 04       	bpl	0x0x96ee
    96ea:	80 0c       	suba	#0xc
    96ec:	20 03       	bra	0x0x96f1
    96ee:	5a          	decb
    96ef:	26 f5       	bne	0x0x96e6
    96f1:	7a 01 0b    	dec	0x10b
    96f4:	2a 36       	bpl	0x0x972c
    96f6:	7d 00 74    	tst	0x74
    96f9:	27 11       	beq	0x0x970c
    96fb:	7a 00 d3    	dec	0xd3
    96fe:	2a 26       	bpl	0x0x9726
    9700:	d6 73       	ldab	*0x73
    9702:	c1 03       	cmpb	#0x3
    9704:	27 0c       	beq	0x0x9712
    9706:	d6 74       	ldab	*0x74
    9708:	d7 d3       	stab	*0xd3
    970a:	20 1a       	bra	0x0x9726
    970c:	d6 73       	ldab	*0x73
    970e:	c1 03       	cmpb	#0x3
    9710:	26 14       	bne	0x0x9726
    9712:	7f 00 d3    	clr	0xd3
    9715:	7f 00 30    	clr	0x30
    9718:	7c 01 0b    	inc	0x10b
    971b:	7c 01 0b    	inc	0x10b
    971e:	f6 01 76    	ldab	0x176
    9721:	f1 01 0b    	cmpb	0x10b
    9724:	24 06       	bcc	0x0x972c
    9726:	f6 01 76    	ldab	0x176
    9729:	f7 01 0b    	stab	0x10b
    972c:	7e 97 3c    	jmp	0x973c
    972f:	81 03       	cmpa	#0x3
    9731:	26 08       	bne	0x0x973b
    9733:	7d 00 30    	tst	0x30
    9736:	2b 8e       	bmi	0x0x96c6
    9738:	7e 96 5a    	jmp	0x965a
    973b:	39          	rts
    973c:	7d 00 d0    	tst	0xd0
    973f:	27 05       	beq	0x0x9746
    9741:	7f 00 d0    	clr	0xd0
    9744:	20 08       	bra	0x0x974e
    9746:	7d 10 29    	tst	0x1029
    9749:	2a fb       	bpl	0x0x9746
    974b:	f6 10 2a    	ldab	0x102a
    974e:	f6 01 75    	ldab	0x175
    9751:	01          	nop
    9752:	01          	nop
    9753:	01          	nop
    9754:	01          	nop
    9755:	f7 10 42    	stab	0x1042
    9758:	01          	nop
    9759:	01          	nop
    975a:	01          	nop
    975b:	01          	nop
    975c:	01          	nop
    975d:	01          	nop
    975e:	01          	nop
    975f:	53          	comb
    9760:	d7 f8       	stab	*0xf8
    9762:	01          	nop
    9763:	01          	nop
    9764:	c6 81       	ldab	#0x81
    9766:	f7 10 2a    	stab	0x102a
    9769:	01          	nop
    976a:	7d 10 29    	tst	0x1029
    976d:	2a fa       	bpl	0x0x9769
    976f:	f6 10 2a    	ldab	0x102a
    9772:	b7 10 2a    	staa	0x102a
    9775:	01          	nop
    9776:	7d 10 29    	tst	0x1029
    9779:	2a fa       	bpl	0x0x9775
    977b:	b6 10 2a    	ldaa	0x102a
    977e:	86 5a       	ldaa	#0x5a
    9780:	b7 10 2a    	staa	0x102a
    9783:	39          	rts
    9784:	7f 00 f8    	clr	0xf8
    9787:	b6 10 22    	ldaa	0x1022
    978a:	84 f7       	anda	#0xf7
    978c:	b7 10 22    	staa	0x1022
    978f:	39          	rts
    9790:	ce 10 23    	ldx	#0x1023
    9793:	1e 00 40 03 	brset	0x0,x, #0x40, 0x0x979a
    9797:	7e a2 c4    	jmp	0xa2c4
    979a:	86 20       	ldaa	#0x20
    979c:	b7 10 44    	staa	0x1044
    979f:	01          	nop
    97a0:	01          	nop
    97a1:	b6 10 45    	ldaa	0x1045
    97a4:	b1 01 0f    	cmpa	0x10f
    97a7:	26 0b       	bne	0x0x97b4
    97a9:	7d 00 fe    	tst	0xfe
    97ac:	27 03       	beq	0x0x97b1
    97ae:	7e a2 b6    	jmp	0xa2b6
    97b1:	7e 9a cc    	jmp	0x9acc
    97b4:	16          	tab
    97b5:	b8 01 0f    	eora	0x10f
    97b8:	f7 01 0f    	stab	0x10f
    97bb:	b4 01 0f    	anda	0x10f
    97be:	84 fc       	anda	#0xfc
    97c0:	26 03       	bne	0x0x97c5
    97c2:	7e 99 c6    	jmp	0x99c6
    97c5:	7d 00 fe    	tst	0xfe
    97c8:	27 0d       	beq	0x0x97d7
    97ca:	85 20       	bita	#0x20
    97cc:	26 03       	bne	0x0x97d1
    97ce:	7e a2 b6    	jmp	0xa2b6
    97d1:	7f 00 fe    	clr	0xfe
    97d4:	7e a8 f3    	jmp	0xa8f3
    97d7:	48          	asla
    97d8:	24 2a       	bcc	0x0x9804
    97da:	96 ff       	ldaa	*0xff
    97dc:	81 01       	cmpa	#0x1
    97de:	26 0d       	bne	0x0x97ed
    97e0:	b6 01 7b    	ldaa	0x17b
    97e3:	97 f9       	staa	*0xf9
    97e5:	bd ad c4    	jsr	0xadc4
    97e8:	b6 01 7c    	ldaa	0x17c
    97eb:	97 fa       	staa	*0xfa
    97ed:	7f 00 ff    	clr	0xff
    97f0:	7f 01 1b    	clr	0x11b
    97f3:	bd a3 59    	jsr	0xa359
    97f6:	96 f2       	ldaa	*0xf2
    97f8:	84 20       	anda	#0x20
    97fa:	97 f2       	staa	*0xf2
    97fc:	86 80       	ldaa	#0x80
    97fe:	b7 01 1c    	staa	0x11c
    9801:	7e a2 b6    	jmp	0xa2b6
    9804:	48          	asla
    9805:	25 03       	bcs	0x0x980a
    9807:	7e 98 b4    	jmp	0x98b4
    980a:	d6 ff       	ldab	*0xff
    980c:	27 4d       	beq	0x0x985b
    980e:	c1 01       	cmpb	#0x1
    9810:	27 71       	beq	0x0x9883
    9812:	b6 01 1b    	ldaa	0x11b
    9815:	81 05       	cmpa	#0x5
    9817:	27 11       	beq	0x0x982a
    9819:	81 25       	cmpa	#0x25
    981b:	26 3e       	bne	0x0x985b
    981d:	86 2b       	ldaa	#0x2b
    981f:	b7 01 1b    	staa	0x11b
    9822:	86 80       	ldaa	#0x80
    9824:	b7 01 1c    	staa	0x11c
    9827:	7e a2 b6    	jmp	0xa2b6
    982a:	13 f2 40 2d 	brclr	*0xf2, #0x40, 0x0x985b
    982e:	b6 01 1e    	ldaa	0x11e
    9831:	81 1e       	cmpa	#0x1e
    9833:	27 03       	beq	0x0x9838
    9835:	7e a2 b6    	jmp	0xa2b6
    9838:	4f          	clra
    9839:	f6 01 3c    	ldab	0x13c
    983c:	c1 41       	cmpb	#0x41
    983e:	27 01       	beq	0x0x9841
    9840:	4c          	inca
    9841:	bd 90 89    	jsr	0x9089
    9844:	7f 00 ff    	clr	0xff
    9847:	7f 01 1b    	clr	0x11b
    984a:	bd a3 59    	jsr	0xa359
    984d:	96 f2       	ldaa	*0xf2
    984f:	84 20       	anda	#0x20
    9851:	97 f2       	staa	*0xf2
    9853:	86 80       	ldaa	#0x80
    9855:	b7 01 1c    	staa	0x11c
    9858:	7e a2 b6    	jmp	0xa2b6
    985b:	7d 01 70    	tst	0x170
    985e:	27 05       	beq	0x0x9865
    9860:	7f 00 ff    	clr	0xff
    9863:	20 38       	bra	0x0x989d
    9865:	c6 01       	ldab	#0x1
    9867:	d7 ff       	stab	*0xff
    9869:	d6 f9       	ldab	*0xf9
    986b:	f7 01 7b    	stab	0x17b
    986e:	d6 fa       	ldab	*0xfa
    9870:	f7 01 7c    	stab	0x17c
    9873:	c6 01       	ldab	#0x1
    9875:	f7 01 1b    	stab	0x11b
    9878:	14 f2 40    	bset	*0xf2, #0x40
    987b:	c6 80       	ldab	#0x80
    987d:	f7 01 1c    	stab	0x11c
    9880:	7e a2 b6    	jmp	0xa2b6
    9883:	96 f9       	ldaa	*0xf9
    9885:	81 04       	cmpa	#0x4
    9887:	26 05       	bne	0x0x988e
    9889:	bd a8 aa    	jsr	0xa8aa
    988c:	20 03       	bra	0x0x9891
    988e:	bd a8 bc    	jsr	0xa8bc
    9891:	7f 00 ff    	clr	0xff
    9894:	7f 00 f2    	clr	0xf2
    9897:	7f 00 fb    	clr	0xfb
    989a:	bd a3 59    	jsr	0xa359
    989d:	7f 01 1b    	clr	0x11b
    98a0:	86 80       	ldaa	#0x80
    98a2:	b7 01 1c    	staa	0x11c
    98a5:	b6 01 7b    	ldaa	0x17b
    98a8:	81 04       	cmpa	#0x4
    98aa:	26 05       	bne	0x0x98b1
    98ac:	97 f9       	staa	*0xf9
    98ae:	bd ad c4    	jsr	0xadc4
    98b1:	7e a2 b6    	jmp	0xa2b6
    98b4:	48          	asla
    98b5:	25 03       	bcs	0x0x98ba
    98b7:	7e 98 ce    	jmp	0x98ce
    98ba:	7d 00 fb    	tst	0xfb
    98bd:	27 0c       	beq	0x0x98cb
    98bf:	96 f9       	ldaa	*0xf9
    98c1:	81 04       	cmpa	#0x4
    98c3:	27 06       	beq	0x0x98cb
    98c5:	7c 00 fe    	inc	0xfe
    98c8:	7e a8 d9    	jmp	0xa8d9
    98cb:	7e a2 b6    	jmp	0xa2b6
    98ce:	48          	asla
    98cf:	25 03       	bcs	0x0x98d4
    98d1:	7e 99 68    	jmp	0x9968
    98d4:	c6 29       	ldab	#0x29
    98d6:	f1 01 1b    	cmpb	0x11b
    98d9:	26 32       	bne	0x0x990d
    98db:	f6 01 6b    	ldab	0x16b
    98de:	5c          	incb
    98df:	c4 07       	andb	#0x7
    98e1:	f7 01 6b    	stab	0x16b
    98e4:	b6 50 00    	ldaa	0x5000
    98e7:	81 06       	cmpa	#0x6
    98e9:	27 18       	beq	0x0x9903
    98eb:	ce 50 50    	ldx	#0x5050
    98ee:	3a          	abx
    98ef:	a6 00       	ldaa	0x0,x
    98f1:	26 03       	bne	0x0x98f6
    98f3:	7f 01 6b    	clr	0x16b
    98f6:	86 29       	ldaa	#0x29
    98f8:	b7 01 1b    	staa	0x11b
    98fb:	86 80       	ldaa	#0x80
    98fd:	b7 01 1c    	staa	0x11c
    9900:	7e a2 b6    	jmp	0xa2b6
    9903:	7d 50 23    	tst	0x5023
    9906:	26 ee       	bne	0x0x98f6
    9908:	7f 01 6b    	clr	0x16b
    990b:	20 e9       	bra	0x0x98f6
    990d:	7d 00 ff    	tst	0xff
    9910:	26 27       	bne	0x0x9939
    9912:	96 f9       	ldaa	*0xf9
    9914:	4c          	inca
    9915:	81 04       	cmpa	#0x4
    9917:	25 01       	bcs	0x0x991a
    9919:	4f          	clra
    991a:	97 f9       	staa	*0xf9
    991c:	bd ad 9e    	jsr	0xad9e
    991f:	b7 7f f6    	staa	0x7ff6
    9922:	bd ad c4    	jsr	0xadc4
    9925:	86 80       	ldaa	#0x80
    9927:	b7 01 1c    	staa	0x11c
    992a:	7f 00 fb    	clr	0xfb
    992d:	7f 00 f2    	clr	0xf2
    9930:	14 d1 80    	bset	*0xd1, #0x80
    9933:	bd a3 70    	jsr	0xa370
    9936:	7e a2 b6    	jmp	0xa2b6
    9939:	13 ff 01 25 	brclr	*0xff, #0x01, 0x0x9962
    993d:	96 f9       	ldaa	*0xf9
    993f:	81 04       	cmpa	#0x4
    9941:	26 03       	bne	0x0x9946
    9943:	7e a2 b6    	jmp	0xa2b6
    9946:	96 f9       	ldaa	*0xf9
    9948:	4c          	inca
    9949:	81 02       	cmpa	#0x2
    994b:	24 04       	bcc	0x0x9951
    994d:	86 03       	ldaa	#0x3
    994f:	20 06       	bra	0x0x9957
    9951:	81 04       	cmpa	#0x4
    9953:	25 02       	bcs	0x0x9957
    9955:	86 02       	ldaa	#0x2
    9957:	97 f9       	staa	*0xf9
    9959:	bd ad c4    	jsr	0xadc4
    995c:	7c 01 7e    	inc	0x17e
    995f:	7e a2 b6    	jmp	0xa2b6
    9962:	7c 01 7e    	inc	0x17e
    9965:	7e a2 b6    	jmp	0xa2b6
    9968:	48          	asla
    9969:	24 11       	bcc	0x0x997c
    996b:	d6 ff       	ldab	*0xff
    996d:	c1 02       	cmpb	#0x2
    996f:	27 03       	beq	0x0x9974
    9971:	7e a2 b6    	jmp	0xa2b6
    9974:	86 01       	ldaa	#0x1
    9976:	b7 01 7f    	staa	0x17f
    9979:	7e a2 b6    	jmp	0xa2b6
    997c:	b6 01 0f    	ldaa	0x10f
    997f:	85 10       	bita	#0x10
    9981:	26 11       	bne	0x0x9994
    9983:	d6 ff       	ldab	*0xff
    9985:	c1 02       	cmpb	#0x2
    9987:	27 03       	beq	0x0x998c
    9989:	7e a2 b6    	jmp	0xa2b6
    998c:	86 80       	ldaa	#0x80
    998e:	b7 01 7f    	staa	0x17f
    9991:	7e a2 b6    	jmp	0xa2b6
    9994:	d6 ff       	ldab	*0xff
    9996:	c1 02       	cmpb	#0x2
    9998:	27 f2       	beq	0x0x998c
    999a:	86 04       	ldaa	#0x4
    999c:	97 f9       	staa	*0xf9
    999e:	bd ad 9e    	jsr	0xad9e
    99a1:	b7 7f f6    	staa	0x7ff6
    99a4:	bd ad c4    	jsr	0xadc4
    99a7:	bd 9f 0f    	jsr	0x9f0f
    99aa:	cc 51 00    	ldd	#0x5100
    99ad:	fd 51 80    	std	0x5180
    99b0:	bd a5 8e    	jsr	0xa58e
    99b3:	b6 50 00    	ldaa	0x5000
    99b6:	97 d1       	staa	*0xd1
    99b8:	86 80       	ldaa	#0x80
    99ba:	b7 01 1c    	staa	0x11c
    99bd:	7f 00 fb    	clr	0xfb
    99c0:	7f 00 f2    	clr	0xf2
    99c3:	7e a2 b6    	jmp	0xa2b6
    99c6:	7d 00 fe    	tst	0xfe
    99c9:	27 03       	beq	0x0x99ce
    99cb:	7e a2 b6    	jmp	0xa2b6
    99ce:	17          	tba
    99cf:	84 03       	anda	#0x3
    99d1:	81 03       	cmpa	#0x3
    99d3:	26 04       	bne	0x0x99d9
    99d5:	86 02       	ldaa	#0x2
    99d7:	20 06       	bra	0x0x99df
    99d9:	81 02       	cmpa	#0x2
    99db:	26 02       	bne	0x0x99df
    99dd:	86 03       	ldaa	#0x3
    99df:	b1 01 18    	cmpa	0x118
    99e2:	26 06       	bne	0x0x99ea
    99e4:	f7 01 0f    	stab	0x10f
    99e7:	7e a2 b6    	jmp	0xa2b6
    99ea:	f7 01 0f    	stab	0x10f
    99ed:	f6 01 18    	ldab	0x118
    99f0:	5c          	incb
    99f1:	c4 03       	andb	#0x3
    99f3:	11          	cba
    99f4:	27 12       	beq	0x0x9a08
    99f6:	f6 01 18    	ldab	0x118
    99f9:	5a          	decb
    99fa:	c4 03       	andb	#0x3
    99fc:	11          	cba
    99fd:	26 03       	bne	0x0x9a02
    99ff:	7e 9a 83    	jmp	0x9a83
    9a02:	b7 01 18    	staa	0x118
    9a05:	7e a2 b6    	jmp	0xa2b6
    9a08:	b7 01 18    	staa	0x118
    9a0b:	7d 00 ff    	tst	0xff
    9a0e:	26 27       	bne	0x0x9a37
    9a10:	7f 00 fb    	clr	0xfb
    9a13:	7f 00 f2    	clr	0xf2
    9a16:	12 d1 80 11 	brset	*0xd1, #0x80, 0x0x9a2b
    9a1a:	bd a3 2d    	jsr	0xa32d
    9a1d:	bd 9f 0f    	jsr	0x9f0f
    9a20:	bd a5 8e    	jsr	0xa58e
    9a23:	b6 50 00    	ldaa	0x5000
    9a26:	97 d1       	staa	*0xd1
    9a28:	7e a2 b6    	jmp	0xa2b6
    9a2b:	bd a2 d8    	jsr	0xa2d8
    9a2e:	bd a3 70    	jsr	0xa370
    9a31:	14 d1 80    	bset	*0xd1, #0x80
    9a34:	7e a2 b6    	jmp	0xa2b6
    9a37:	13 ff 01 12 	brclr	*0xff, #0x01, 0x0x9a4d
    9a3b:	96 f9       	ldaa	*0xf9
    9a3d:	81 04       	cmpa	#0x4
    9a3f:	26 06       	bne	0x0x9a47
    9a41:	bd a3 2d    	jsr	0xa32d
    9a44:	7e a2 b6    	jmp	0xa2b6
    9a47:	bd a2 d8    	jsr	0xa2d8
    9a4a:	7e a2 b6    	jmp	0xa2b6
    9a4d:	86 01       	ldaa	#0x1
    9a4f:	b7 01 1a    	staa	0x11a
    9a52:	7d 00 fb    	tst	0xfb
    9a55:	27 03       	beq	0x0x9a5a
    9a57:	7e a2 b6    	jmp	0xa2b6
    9a5a:	b6 01 1b    	ldaa	0x11b
    9a5d:	81 03       	cmpa	#0x3
    9a5f:	27 1f       	beq	0x0x9a80
    9a61:	81 04       	cmpa	#0x4
    9a63:	27 1b       	beq	0x0x9a80
    9a65:	81 05       	cmpa	#0x5
    9a67:	27 17       	beq	0x0x9a80
    9a69:	81 23       	cmpa	#0x23
    9a6b:	27 13       	beq	0x0x9a80
    9a6d:	81 24       	cmpa	#0x24
    9a6f:	27 0f       	beq	0x0x9a80
    9a71:	81 25       	cmpa	#0x25
    9a73:	27 0b       	beq	0x0x9a80
    9a75:	14 fb 01    	bset	*0xfb, #0x01
    9a78:	14 f2 20    	bset	*0xf2, #0x20
    9a7b:	86 1f       	ldaa	#0x1f
    9a7d:	b7 01 66    	staa	0x166
    9a80:	7e a2 b6    	jmp	0xa2b6
    9a83:	b7 01 18    	staa	0x118
    9a86:	7d 00 ff    	tst	0xff
    9a89:	26 27       	bne	0x0x9ab2
    9a8b:	7f 00 fb    	clr	0xfb
    9a8e:	7f 00 f2    	clr	0xf2
    9a91:	12 d1 80 11 	brset	*0xd1, #0x80, 0x0x9aa6
    9a95:	bd a3 43    	jsr	0xa343
    9a98:	bd 9f 0f    	jsr	0x9f0f
    9a9b:	bd a5 8e    	jsr	0xa58e
    9a9e:	b6 50 00    	ldaa	0x5000
    9aa1:	97 d1       	staa	*0xd1
    9aa3:	7e a2 b6    	jmp	0xa2b6
    9aa6:	bd a3 03    	jsr	0xa303
    9aa9:	bd a3 70    	jsr	0xa370
    9aac:	14 d1 80    	bset	*0xd1, #0x80
    9aaf:	7e a2 b6    	jmp	0xa2b6
    9ab2:	13 ff 01 12 	brclr	*0xff, #0x01, 0x0x9ac8
    9ab6:	96 f9       	ldaa	*0xf9
    9ab8:	81 04       	cmpa	#0x4
    9aba:	26 06       	bne	0x0x9ac2
    9abc:	bd a3 43    	jsr	0xa343
    9abf:	7e a2 b6    	jmp	0xa2b6
    9ac2:	bd a3 03    	jsr	0xa303
    9ac5:	7e a2 b6    	jmp	0xa2b6
    9ac8:	86 80       	ldaa	#0x80
    9aca:	20 83       	bra	0x0x9a4f
    9acc:	86 10       	ldaa	#0x10
    9ace:	b7 10 44    	staa	0x1044
    9ad1:	01          	nop
    9ad2:	01          	nop
    9ad3:	b6 10 45    	ldaa	0x1045
    9ad6:	b1 01 10    	cmpa	0x110
    9ad9:	26 03       	bne	0x0x9ade
    9adb:	7e 9d 43    	jmp	0x9d43
    9ade:	16          	tab
    9adf:	b8 01 10    	eora	0x110
    9ae2:	f7 01 10    	stab	0x110
    9ae5:	b4 01 10    	anda	0x110
    9ae8:	26 03       	bne	0x0x9aed
    9aea:	7e 9d 43    	jmp	0x9d43
    9aed:	48          	asla
    9aee:	24 34       	bcc	0x0x9b24
    9af0:	86 16       	ldaa	#0x16
    9af2:	b1 01 1b    	cmpa	0x11b
    9af5:	26 10       	bne	0x0x9b07
    9af7:	4c          	inca
    9af8:	b7 01 1b    	staa	0x11b
    9afb:	96 f2       	ldaa	*0xf2
    9afd:	84 20       	anda	#0x20
    9aff:	8a 02       	oraa	#0x2
    9b01:	97 f2       	staa	*0xf2
    9b03:	86 02       	ldaa	#0x2
    9b05:	20 13       	bra	0x0x9b1a
    9b07:	b7 01 1b    	staa	0x11b
    9b0a:	bd a3 59    	jsr	0xa359
    9b0d:	14 f3 80    	bset	*0xf3, #0x80
    9b10:	96 f2       	ldaa	*0xf2
    9b12:	84 20       	anda	#0x20
    9b14:	8a 01       	oraa	#0x1
    9b16:	97 f2       	staa	*0xf2
    9b18:	86 02       	ldaa	#0x2
    9b1a:	97 ff       	staa	*0xff
    9b1c:	86 80       	ldaa	#0x80
    9b1e:	b7 01 1c    	staa	0x11c
    9b21:	7e a2 b6    	jmp	0xa2b6
    9b24:	48          	asla
    9b25:	24 42       	bcc	0x0x9b69
    9b27:	96 f2       	ldaa	*0xf2
    9b29:	84 20       	anda	#0x20
    9b2b:	c6 13       	ldab	#0x13
    9b2d:	f1 01 1b    	cmpb	0x11b
    9b30:	27 12       	beq	0x0x9b44
    9b32:	5c          	incb
    9b33:	f1 01 1b    	cmpb	0x11b
    9b36:	27 11       	beq	0x0x9b49
    9b38:	5c          	incb
    9b39:	f1 01 1b    	cmpb	0x11b
    9b3c:	27 10       	beq	0x0x9b4e
    9b3e:	5a          	decb
    9b3f:	5a          	decb
    9b40:	8a 01       	oraa	#0x1
    9b42:	20 0e       	bra	0x0x9b52
    9b44:	5c          	incb
    9b45:	8a 02       	oraa	#0x2
    9b47:	20 09       	bra	0x0x9b52
    9b49:	5c          	incb
    9b4a:	8a 04       	oraa	#0x4
    9b4c:	20 04       	bra	0x0x9b52
    9b4e:	c6 28       	ldab	#0x28
    9b50:	8a 08       	oraa	#0x8
    9b52:	f7 01 1b    	stab	0x11b
    9b55:	97 f2       	staa	*0xf2
    9b57:	bd a3 59    	jsr	0xa359
    9b5a:	14 f3 40    	bset	*0xf3, #0x40
    9b5d:	86 02       	ldaa	#0x2
    9b5f:	97 ff       	staa	*0xff
    9b61:	86 80       	ldaa	#0x80
    9b63:	b7 01 1c    	staa	0x11c
    9b66:	7e a2 b6    	jmp	0xa2b6
    9b69:	48          	asla
    9b6a:	24 33       	bcc	0x0x9b9f
    9b6c:	b6 10 45    	ldaa	0x1045
    9b6f:	85 40       	bita	#0x40
    9b71:	27 03       	beq	0x0x9b76
    9b73:	7e a9 0d    	jmp	0xa90d
    9b76:	96 f2       	ldaa	*0xf2
    9b78:	84 20       	anda	#0x20
    9b7a:	c6 06       	ldab	#0x6
    9b7c:	f1 01 1b    	cmpb	0x11b
    9b7f:	27 04       	beq	0x0x9b85
    9b81:	8a 01       	oraa	#0x1
    9b83:	20 03       	bra	0x0x9b88
    9b85:	5c          	incb
    9b86:	8a 02       	oraa	#0x2
    9b88:	f7 01 1b    	stab	0x11b
    9b8b:	97 f2       	staa	*0xf2
    9b8d:	bd a3 59    	jsr	0xa359
    9b90:	14 f3 20    	bset	*0xf3, #0x20
    9b93:	86 02       	ldaa	#0x2
    9b95:	97 ff       	staa	*0xff
    9b97:	86 80       	ldaa	#0x80
    9b99:	b7 01 1c    	staa	0x11c
    9b9c:	7e a2 b6    	jmp	0xa2b6
    9b9f:	48          	asla
    9ba0:	25 03       	bcs	0x0x9ba5
    9ba2:	7e 9c fc    	jmp	0x9cfc
    9ba5:	12 f3 10 03 	brset	*0xf3, #0x10, 0x0x9bac
    9ba9:	7e 9c 4d    	jmp	0x9c4d
    9bac:	8d 09       	bsr	0x0x9bb7
    9bae:	4f          	clra
    9baf:	c6 75       	ldab	#0x75
    9bb1:	bd b1 9a    	jsr	0xb19a
    9bb4:	7e a2 b6    	jmp	0xa2b6
    9bb7:	15 f3 10    	bclr	*0xf3, #0x10
    9bba:	b6 10 22    	ldaa	0x1022
    9bbd:	84 f7       	anda	#0xf7
    9bbf:	b7 10 22    	staa	0x1022
    9bc2:	7f 01 76    	clr	0x176
    9bc5:	7f 00 f8    	clr	0xf8
    9bc8:	7d 00 d0    	tst	0xd0
    9bcb:	26 08       	bne	0x0x9bd5
    9bcd:	7d 10 29    	tst	0x1029
    9bd0:	2a fb       	bpl	0x0x9bcd
    9bd2:	b6 10 2a    	ldaa	0x102a
    9bd5:	b6 01 75    	ldaa	0x175
    9bd8:	01          	nop
    9bd9:	01          	nop
    9bda:	01          	nop
    9bdb:	01          	nop
    9bdc:	b7 10 42    	staa	0x1042
    9bdf:	01          	nop
    9be0:	01          	nop
    9be1:	01          	nop
    9be2:	01          	nop
    9be3:	01          	nop
    9be4:	01          	nop
    9be5:	01          	nop
    9be6:	86 80       	ldaa	#0x80
    9be8:	b7 10 2a    	staa	0x102a
    9beb:	ce 01 00    	ldx	#0x100
    9bee:	ff 01 08    	stx	0x108
    9bf1:	c6 fe       	ldab	#0xfe
    9bf3:	6d 00       	tst	0x0,x
    9bf5:	27 48       	beq	0x0x9c3f
    9bf7:	7d 10 29    	tst	0x1029
    9bfa:	2a fb       	bpl	0x0x9bf7
    9bfc:	b6 10 2a    	ldaa	0x102a
    9bff:	86 81       	ldaa	#0x81
    9c01:	01          	nop
    9c02:	01          	nop
    9c03:	f7 10 42    	stab	0x1042
    9c06:	01          	nop
    9c07:	01          	nop
    9c08:	01          	nop
    9c09:	01          	nop
    9c0a:	01          	nop
    9c0b:	01          	nop
    9c0c:	01          	nop
    9c0d:	b7 10 2a    	staa	0x102a
    9c10:	7d 10 29    	tst	0x1029
    9c13:	2a fb       	bpl	0x0x9c10
    9c15:	b6 10 2a    	ldaa	0x102a
    9c18:	a6 00       	ldaa	0x0,x
    9c1a:	b7 10 2a    	staa	0x102a
    9c1d:	7d 10 29    	tst	0x1029
    9c20:	2a fb       	bpl	0x0x9c1d
    9c22:	b6 10 2a    	ldaa	0x102a
    9c25:	86 5a       	ldaa	#0x5a
    9c27:	b7 10 2a    	staa	0x102a
    9c2a:	ff 01 08    	stx	0x108
    9c2d:	17          	tba
    9c2e:	43          	coma
    9c2f:	9a f8       	oraa	*0xf8
    9c31:	97 f8       	staa	*0xf8
    9c33:	12 f5 20 08 	brset	*0xf5, #0x20, 0x0x9c3f
    9c37:	0d          	sec
    9c38:	59          	rolb
    9c39:	08          	inx
    9c3a:	8c 01 f0    	cpx	#0x1f0
    9c3d:	23 b4       	bls	0x0x9bf3
    9c3f:	86 01       	ldaa	#0x1
    9c41:	b7 01 09    	staa	0x109
    9c44:	96 71       	ldaa	*0x71
    9c46:	97 db       	staa	*0xdb
    9c48:	96 72       	ldaa	*0x72
    9c4a:	97 dc       	staa	*0xdc
    9c4c:	39          	rts
    9c4d:	8d 0a       	bsr	0x0x9c59
    9c4f:	86 7f       	ldaa	#0x7f
    9c51:	c6 75       	ldab	#0x75
    9c53:	bd b1 9a    	jsr	0xb19a
    9c56:	7e a2 b6    	jmp	0xa2b6
    9c59:	14 f3 10    	bset	*0xf3, #0x10
    9c5c:	ce 01 00    	ldx	#0x100
    9c5f:	18 ce 01 01 	ldy	#0x101
    9c63:	a6 00       	ldaa	0x0,x
    9c65:	26 02       	bne	0x0x9c69
    9c67:	86 80       	ldaa	#0x80
    9c69:	18 e6 00    	ldab	0x0,y
    9c6c:	26 0a       	bne	0x0x9c78
    9c6e:	18 08       	iny
    9c70:	18 8c 01 07 	cpy	#0x107
    9c74:	23 f3       	bls	0x0x9c69
    9c76:	20 0f       	bra	0x0x9c87
    9c78:	11          	cba
    9c79:	25 f3       	bcs	0x0x9c6e
    9c7b:	4d          	tsta
    9c7c:	2a 01       	bpl	0x0x9c7f
    9c7e:	4f          	clra
    9c7f:	18 a7 00    	staa	0x0,y
    9c82:	e7 00       	stab	0x0,x
    9c84:	17          	tba
    9c85:	20 e7       	bra	0x0x9c6e
    9c87:	4d          	tsta
    9c88:	2a 11       	bpl	0x0x9c9b
    9c8a:	6f 00       	clr	0x0,x
    9c8c:	8c 01 00    	cpx	#0x100
    9c8f:	27 26       	beq	0x0x9cb7
    9c91:	8f          	xgdx
    9c92:	5a          	decb
    9c93:	2a 01       	bpl	0x0x9c96
    9c95:	5f          	clrb
    9c96:	f7 01 76    	stab	0x176
    9c99:	20 1c       	bra	0x0x9cb7
    9c9b:	08          	inx
    9c9c:	8c 01 07    	cpx	#0x107
    9c9f:	25 0f       	bcs	0x0x9cb0
    9ca1:	86 06       	ldaa	#0x6
    9ca3:	b7 01 76    	staa	0x176
    9ca6:	7d 01 07    	tst	0x107
    9ca9:	27 0c       	beq	0x0x9cb7
    9cab:	7c 01 76    	inc	0x176
    9cae:	20 07       	bra	0x0x9cb7
    9cb0:	3c          	pshx
    9cb1:	18 38       	puly
    9cb3:	18 08       	iny
    9cb5:	20 ac       	bra	0x0x9c63
    9cb7:	86 fe       	ldaa	#0xfe
    9cb9:	b7 01 75    	staa	0x175
    9cbc:	7f 01 0b    	clr	0x10b
    9cbf:	7d 01 00    	tst	0x100
    9cc2:	26 03       	bne	0x0x9cc7
    9cc4:	7e 9c fb    	jmp	0x9cfb
    9cc7:	7d 00 dc    	tst	0xdc
    9cca:	26 2f       	bne	0x0x9cfb
    9ccc:	86 01       	ldaa	#0x1
    9cce:	b7 01 72    	staa	0x172
    9cd1:	86 80       	ldaa	#0x80
    9cd3:	97 d2       	staa	*0xd2
    9cd5:	7f 00 30    	clr	0x30
    9cd8:	7f 00 d3    	clr	0xd3
    9cdb:	96 73       	ldaa	*0x73
    9cdd:	81 02       	cmpa	#0x2
    9cdf:	26 04       	bne	0x0x9ce5
    9ce1:	96 74       	ldaa	*0x74
    9ce3:	97 d3       	staa	*0xd3
    9ce5:	86 08       	ldaa	#0x8
    9ce7:	b7 10 23    	staa	0x1023
    9cea:	fc 10 0e    	ldd	0x100e
    9ced:	c3 00 c8    	addd	#0xc8
    9cf0:	fd 10 1e    	std	0x101e
    9cf3:	b6 10 22    	ldaa	0x1022
    9cf6:	8a 08       	oraa	#0x8
    9cf8:	b7 10 22    	staa	0x1022
    9cfb:	39          	rts
    9cfc:	48          	asla
    9cfd:	24 1f       	bcc	0x0x9d1e
    9cff:	bd a3 59    	jsr	0xa359
    9d02:	14 f3 08    	bset	*0xf3, #0x08
    9d05:	96 f2       	ldaa	*0xf2
    9d07:	84 20       	anda	#0x20
    9d09:	8a 01       	oraa	#0x1
    9d0b:	97 f2       	staa	*0xf2
    9d0d:	c6 18       	ldab	#0x18
    9d0f:	f7 01 1b    	stab	0x11b
    9d12:	86 02       	ldaa	#0x2
    9d14:	97 ff       	staa	*0xff
    9d16:	86 80       	ldaa	#0x80
    9d18:	b7 01 1c    	staa	0x11c
    9d1b:	7e a2 b6    	jmp	0xa2b6
    9d1e:	96 73       	ldaa	*0x73
    9d20:	4c          	inca
    9d21:	81 03       	cmpa	#0x3
    9d23:	23 02       	bls	0x0x9d27
    9d25:	86 01       	ldaa	#0x1
    9d27:	97 73       	staa	*0x73
    9d29:	d6 f3       	ldab	*0xf3
    9d2b:	c4 f8       	andb	#0xf8
    9d2d:	1b          	aba
    9d2e:	97 f3       	staa	*0xf3
    9d30:	7d 00 fb    	tst	0xfb
    9d33:	26 0b       	bne	0x0x9d40
    9d35:	14 fb 01    	bset	*0xfb, #0x01
    9d38:	14 f2 20    	bset	*0xf2, #0x20
    9d3b:	86 1f       	ldaa	#0x1f
    9d3d:	b7 01 66    	staa	0x166
    9d40:	7e a2 b6    	jmp	0xa2b6
    9d43:	86 08       	ldaa	#0x8
    9d45:	b7 10 44    	staa	0x1044
    9d48:	01          	nop
    9d49:	01          	nop
    9d4a:	b6 10 45    	ldaa	0x1045
    9d4d:	b1 01 11    	cmpa	0x111
    9d50:	26 03       	bne	0x0x9d55
    9d52:	7e 9e 2e    	jmp	0x9e2e
    9d55:	16          	tab
    9d56:	b8 01 11    	eora	0x111
    9d59:	f7 01 11    	stab	0x111
    9d5c:	b4 01 11    	anda	0x111
    9d5f:	26 03       	bne	0x0x9d64
    9d61:	7e 9e 2e    	jmp	0x9e2e
    9d64:	44          	lsra
    9d65:	24 50       	bcc	0x0x9db7
    9d67:	12 f4 01 10 	brset	*0xf4, #0x01, 0x0x9d7b
    9d6b:	bd a3 59    	jsr	0xa359
    9d6e:	14 f4 01    	bset	*0xf4, #0x01
    9d71:	c6 19       	ldab	#0x19
    9d73:	96 f2       	ldaa	*0xf2
    9d75:	84 20       	anda	#0x20
    9d77:	8a 01       	oraa	#0x1
    9d79:	20 2b       	bra	0x0x9da6
    9d7b:	96 f2       	ldaa	*0xf2
    9d7d:	84 20       	anda	#0x20
    9d7f:	c6 19       	ldab	#0x19
    9d81:	f1 01 1b    	cmpb	0x11b
    9d84:	27 12       	beq	0x0x9d98
    9d86:	5c          	incb
    9d87:	f1 01 1b    	cmpb	0x11b
    9d8a:	27 11       	beq	0x0x9d9d
    9d8c:	5c          	incb
    9d8d:	f1 01 1b    	cmpb	0x11b
    9d90:	27 10       	beq	0x0x9da2
    9d92:	c6 19       	ldab	#0x19
    9d94:	8a 01       	oraa	#0x1
    9d96:	20 0e       	bra	0x0x9da6
    9d98:	5c          	incb
    9d99:	8a 02       	oraa	#0x2
    9d9b:	20 09       	bra	0x0x9da6
    9d9d:	5c          	incb
    9d9e:	8a 04       	oraa	#0x4
    9da0:	20 04       	bra	0x0x9da6
    9da2:	c6 19       	ldab	#0x19
    9da4:	8a 01       	oraa	#0x1
    9da6:	97 f2       	staa	*0xf2
    9da8:	f7 01 1b    	stab	0x11b
    9dab:	86 02       	ldaa	#0x2
    9dad:	97 ff       	staa	*0xff
    9daf:	86 80       	ldaa	#0x80
    9db1:	b7 01 1c    	staa	0x11c
    9db4:	7e a2 b6    	jmp	0xa2b6
    9db7:	44          	lsra
    9db8:	24 14       	bcc	0x0x9dce
    9dba:	12 f4 02 bd 	brset	*0xf4, #0x02, 0x0x9d7b
    9dbe:	bd a3 59    	jsr	0xa359
    9dc1:	14 f4 02    	bset	*0xf4, #0x02
    9dc4:	c6 19       	ldab	#0x19
    9dc6:	96 f2       	ldaa	*0xf2
    9dc8:	84 20       	anda	#0x20
    9dca:	8a 01       	oraa	#0x1
    9dcc:	20 d8       	bra	0x0x9da6
    9dce:	44          	lsra
    9dcf:	24 03       	bcc	0x0x9dd4
    9dd1:	7e a2 b6    	jmp	0xa2b6
    9dd4:	44          	lsra
    9dd5:	24 1f       	bcc	0x0x9df6
    9dd7:	bd a3 59    	jsr	0xa359
    9dda:	14 f4 08    	bset	*0xf4, #0x08
    9ddd:	96 f2       	ldaa	*0xf2
    9ddf:	84 20       	anda	#0x20
    9de1:	8a 01       	oraa	#0x1
    9de3:	97 f2       	staa	*0xf2
    9de5:	c6 26       	ldab	#0x26
    9de7:	f7 01 1b    	stab	0x11b
    9dea:	86 02       	ldaa	#0x2
    9dec:	97 ff       	staa	*0xff
    9dee:	86 80       	ldaa	#0x80
    9df0:	b7 01 1c    	staa	0x11c
    9df3:	7e a2 b6    	jmp	0xa2b6
    9df6:	c6 24       	ldab	#0x24
    9df8:	f1 01 1b    	cmpb	0x11b
    9dfb:	27 1d       	beq	0x0x9e1a
    9dfd:	f7 01 1b    	stab	0x11b
    9e00:	bd a3 59    	jsr	0xa359
    9e03:	14 f4 10    	bset	*0xf4, #0x10
    9e06:	96 f2       	ldaa	*0xf2
    9e08:	84 20       	anda	#0x20
    9e0a:	8a 01       	oraa	#0x1
    9e0c:	97 f2       	staa	*0xf2
    9e0e:	86 02       	ldaa	#0x2
    9e10:	97 ff       	staa	*0xff
    9e12:	86 80       	ldaa	#0x80
    9e14:	b7 01 1c    	staa	0x11c
    9e17:	7e a2 b6    	jmp	0xa2b6
    9e1a:	5c          	incb
    9e1b:	f7 01 1b    	stab	0x11b
    9e1e:	96 f2       	ldaa	*0xf2
    9e20:	84 20       	anda	#0x20
    9e22:	8a 42       	oraa	#0x42
    9e24:	97 f2       	staa	*0xf2
    9e26:	86 80       	ldaa	#0x80
    9e28:	b7 01 1c    	staa	0x11c
    9e2b:	7e a2 b6    	jmp	0xa2b6
    9e2e:	86 04       	ldaa	#0x4
    9e30:	b7 10 44    	staa	0x1044
    9e33:	01          	nop
    9e34:	01          	nop
    9e35:	b6 10 45    	ldaa	0x1045
    9e38:	b1 01 12    	cmpa	0x112
    9e3b:	26 03       	bne	0x0x9e40
    9e3d:	7e a0 f7    	jmp	0xa0f7
    9e40:	16          	tab
    9e41:	b8 01 12    	eora	0x112
    9e44:	f7 01 12    	stab	0x112
    9e47:	b4 01 12    	anda	0x112
    9e4a:	26 03       	bne	0x0x9e4f
    9e4c:	7e a0 f7    	jmp	0xa0f7
    9e4f:	48          	asla
    9e50:	24 1e       	bcc	0x0x9e70
    9e52:	96 4a       	ldaa	*0x4a
    9e54:	4c          	inca
    9e55:	84 03       	anda	#0x3
    9e57:	97 4a       	staa	*0x4a
    9e59:	c6 4a       	ldab	#0x4a
    9e5b:	bd b0 fc    	jsr	0xb0fc
    9e5e:	c6 1f       	ldab	#0x1f
    9e60:	d4 f4       	andb	*0xf4
    9e62:	d7 f4       	stab	*0xf4
    9e64:	48          	asla
    9e65:	48          	asla
    9e66:	48          	asla
    9e67:	48          	asla
    9e68:	48          	asla
    9e69:	9a f4       	oraa	*0xf4
    9e6b:	97 f4       	staa	*0xf4
    9e6d:	7e a2 a6    	jmp	0xa2a6
    9e70:	48          	asla
    9e71:	24 6f       	bcc	0x0x9ee2
    9e73:	86 02       	ldaa	#0x2
    9e75:	b1 01 1b    	cmpa	0x11b
    9e78:	26 15       	bne	0x0x9e8f
    9e7a:	96 f2       	ldaa	*0xf2
    9e7c:	84 20       	anda	#0x20
    9e7e:	8a 02       	oraa	#0x2
    9e80:	97 f2       	staa	*0xf2
    9e82:	86 03       	ldaa	#0x3
    9e84:	b7 01 1b    	staa	0x11b
    9e87:	86 80       	ldaa	#0x80
    9e89:	b7 01 1c    	staa	0x11c
    9e8c:	7e a2 b6    	jmp	0xa2b6
    9e8f:	86 03       	ldaa	#0x3
    9e91:	b1 01 1b    	cmpa	0x11b
    9e94:	26 14       	bne	0x0x9eaa
    9e96:	4c          	inca
    9e97:	b7 01 1b    	staa	0x11b
    9e9a:	96 f2       	ldaa	*0xf2
    9e9c:	84 20       	anda	#0x20
    9e9e:	8a 04       	oraa	#0x4
    9ea0:	97 f2       	staa	*0xf2
    9ea2:	86 80       	ldaa	#0x80
    9ea4:	b7 01 1c    	staa	0x11c
    9ea7:	7e a2 b6    	jmp	0xa2b6
    9eaa:	86 04       	ldaa	#0x4
    9eac:	b1 01 1b    	cmpa	0x11b
    9eaf:	26 14       	bne	0x0x9ec5
    9eb1:	4c          	inca
    9eb2:	b7 01 1b    	staa	0x11b
    9eb5:	96 f2       	ldaa	*0xf2
    9eb7:	84 20       	anda	#0x20
    9eb9:	8a 48       	oraa	#0x48
    9ebb:	97 f2       	staa	*0xf2
    9ebd:	86 80       	ldaa	#0x80
    9ebf:	b7 01 1c    	staa	0x11c
    9ec2:	7e a2 b6    	jmp	0xa2b6
    9ec5:	86 02       	ldaa	#0x2
    9ec7:	b7 01 1b    	staa	0x11b
    9eca:	97 ff       	staa	*0xff
    9ecc:	bd a3 59    	jsr	0xa359
    9ecf:	96 f2       	ldaa	*0xf2
    9ed1:	84 20       	anda	#0x20
    9ed3:	8a 01       	oraa	#0x1
    9ed5:	97 f2       	staa	*0xf2
    9ed7:	14 f5 40    	bset	*0xf5, #0x40
    9eda:	86 80       	ldaa	#0x80
    9edc:	b7 01 1c    	staa	0x11c
    9edf:	7e a2 b6    	jmp	0xa2b6
    9ee2:	48          	asla
    9ee3:	24 76       	bcc	0x0x9f5b
    9ee5:	86 04       	ldaa	#0x4
    9ee7:	91 f9       	cmpa	*0xf9
    9ee9:	26 08       	bne	0x0x9ef3
    9eeb:	15 f5 20    	bclr	*0xf5, #0x20
    9eee:	8d 1f       	bsr	0x0x9f0f
    9ef0:	7e ad 7e    	jmp	0xad7e
    9ef3:	86 20       	ldaa	#0x20
    9ef5:	98 6a       	eora	*0x6a
    9ef7:	97 6a       	staa	*0x6a
    9ef9:	13 6a 20 05 	brclr	*0x6a, #0x20, 0x0x9f02
    9efd:	14 f5 20    	bset	*0xf5, #0x20
    9f00:	20 03       	bra	0x0x9f05
    9f02:	15 f5 20    	bclr	*0xf5, #0x20
    9f05:	c6 6a       	ldab	#0x6a
    9f07:	bd b0 fc    	jsr	0xb0fc
    9f0a:	8d 03       	bsr	0x0x9f0f
    9f0c:	7e ad 7e    	jmp	0xad7e
    9f0f:	7f 00 f8    	clr	0xf8
    9f12:	ce 01 07    	ldx	#0x107
    9f15:	ff 01 08    	stx	0x108
    9f18:	c6 08       	ldab	#0x8
    9f1a:	ce 01 00    	ldx	#0x100
    9f1d:	6f 00       	clr	0x0,x
    9f1f:	08          	inx
    9f20:	5a          	decb
    9f21:	26 fa       	bne	0x0x9f1d
    9f23:	96 f9       	ldaa	*0xf9
    9f25:	81 04       	cmpa	#0x4
    9f27:	26 0b       	bne	0x0x9f34
    9f29:	ce 51 00    	ldx	#0x5100
    9f2c:	c6 08       	ldab	#0x8
    9f2e:	6f 00       	clr	0x0,x
    9f30:	08          	inx
    9f31:	5a          	decb
    9f32:	26 fa       	bne	0x0x9f2e
    9f34:	7d 00 d0    	tst	0xd0
    9f37:	27 05       	beq	0x0x9f3e
    9f39:	7f 00 d0    	clr	0xd0
    9f3c:	20 08       	bra	0x0x9f46
    9f3e:	7d 10 29    	tst	0x1029
    9f41:	2a fb       	bpl	0x0x9f3e
    9f43:	f6 10 2a    	ldab	0x102a
    9f46:	5f          	clrb
    9f47:	01          	nop
    9f48:	01          	nop
    9f49:	01          	nop
    9f4a:	01          	nop
    9f4b:	f7 10 42    	stab	0x1042
    9f4e:	01          	nop
    9f4f:	01          	nop
    9f50:	01          	nop
    9f51:	01          	nop
    9f52:	01          	nop
    9f53:	01          	nop
    9f54:	01          	nop
    9f55:	86 80       	ldaa	#0x80
    9f57:	b7 10 2a    	staa	0x102a
    9f5a:	39          	rts
    9f5b:	48          	asla
    9f5c:	24 56       	bcc	0x0x9fb4
    9f5e:	86 09       	ldaa	#0x9
    9f60:	b1 01 1b    	cmpa	0x11b
    9f63:	26 14       	bne	0x0x9f79
    9f65:	d6 f2       	ldab	*0xf2
    9f67:	c4 20       	andb	#0x20
    9f69:	ca 02       	orab	#0x2
    9f6b:	d7 f2       	stab	*0xf2
    9f6d:	4c          	inca
    9f6e:	b7 01 1b    	staa	0x11b
    9f71:	86 80       	ldaa	#0x80
    9f73:	b7 01 1c    	staa	0x11c
    9f76:	7e a2 b6    	jmp	0xa2b6
    9f79:	20 1a       	bra	0x0x9f95
    9f7b:	4c          	inca
    9f7c:	b1 01 1b    	cmpa	0x11b
    9f7f:	26 14       	bne	0x0x9f95
    9f81:	4c          	inca
    9f82:	b7 01 1b    	staa	0x11b
    9f85:	96 f2       	ldaa	*0xf2
    9f87:	84 20       	anda	#0x20
    9f89:	8a 04       	oraa	#0x4
    9f8b:	97 f2       	staa	*0xf2
    9f8d:	86 80       	ldaa	#0x80
    9f8f:	b7 01 1c    	staa	0x11c
    9f92:	7e a2 b6    	jmp	0xa2b6
    9f95:	86 09       	ldaa	#0x9
    9f97:	b7 01 1b    	staa	0x11b
    9f9a:	86 02       	ldaa	#0x2
    9f9c:	97 ff       	staa	*0xff
    9f9e:	bd a3 59    	jsr	0xa359
    9fa1:	96 f2       	ldaa	*0xf2
    9fa3:	84 20       	anda	#0x20
    9fa5:	8a 01       	oraa	#0x1
    9fa7:	97 f2       	staa	*0xf2
    9fa9:	14 f5 10    	bset	*0xf5, #0x10
    9fac:	86 80       	ldaa	#0x80
    9fae:	b7 01 1c    	staa	0x11c
    9fb1:	7e a2 b6    	jmp	0xa2b6
    9fb4:	48          	asla
    9fb5:	24 55       	bcc	0x0xa00c
    9fb7:	96 f2       	ldaa	*0xf2
    9fb9:	84 20       	anda	#0x20
    9fbb:	c6 1d       	ldab	#0x1d
    9fbd:	f1 01 1b    	cmpb	0x11b
    9fc0:	27 1f       	beq	0x0x9fe1
    9fc2:	5c          	incb
    9fc3:	f1 01 1b    	cmpb	0x11b
    9fc6:	27 1e       	beq	0x0x9fe6
    9fc8:	5c          	incb
    9fc9:	f1 01 1b    	cmpb	0x11b
    9fcc:	27 1d       	beq	0x0x9feb
    9fce:	bd a3 59    	jsr	0xa359
    9fd1:	86 08       	ldaa	#0x8
    9fd3:	9a f5       	oraa	*0xf5
    9fd5:	97 f5       	staa	*0xf5
    9fd7:	96 f2       	ldaa	*0xf2
    9fd9:	84 20       	anda	#0x20
    9fdb:	8a 01       	oraa	#0x1
    9fdd:	c6 1d       	ldab	#0x1d
    9fdf:	20 1a       	bra	0x0x9ffb
    9fe1:	5c          	incb
    9fe2:	8a 02       	oraa	#0x2
    9fe4:	20 15       	bra	0x0x9ffb
    9fe6:	5c          	incb
    9fe7:	8a 04       	oraa	#0x4
    9fe9:	20 10       	bra	0x0x9ffb
    9feb:	8f          	xgdx
    9fec:	12 d1 80 07 	brset	*0xd1, #0x80, 0x0x9ff7
    9ff0:	8f          	xgdx
    9ff1:	c6 1d       	ldab	#0x1d
    9ff3:	8a 01       	oraa	#0x1
    9ff5:	20 04       	bra	0x0x9ffb
    9ff7:	8f          	xgdx
    9ff8:	5c          	incb
    9ff9:	8a 08       	oraa	#0x8
    9ffb:	97 f2       	staa	*0xf2
    9ffd:	f7 01 1b    	stab	0x11b
    a000:	86 02       	ldaa	#0x2
    a002:	97 ff       	staa	*0xff
    a004:	86 80       	ldaa	#0x80
    a006:	b7 01 1c    	staa	0x11c
    a009:	7e a2 b6    	jmp	0xa2b6
    a00c:	48          	asla
    a00d:	24 54       	bcc	0x0xa063
    a00f:	f6 01 1b    	ldab	0x11b
    a012:	c1 22       	cmpb	#0x22
    a014:	26 15       	bne	0x0xa02b
    a016:	86 08       	ldaa	#0x8
    a018:	b7 01 1b    	staa	0x11b
    a01b:	96 f2       	ldaa	*0xf2
    a01d:	84 20       	anda	#0x20
    a01f:	8a 01       	oraa	#0x1
    a021:	97 f2       	staa	*0xf2
    a023:	86 80       	ldaa	#0x80
    a025:	b7 01 1c    	staa	0x11c
    a028:	7e a2 b6    	jmp	0xa2b6
    a02b:	c1 08       	cmpb	#0x8
    a02d:	26 15       	bne	0x0xa044
    a02f:	86 22       	ldaa	#0x22
    a031:	b7 01 1b    	staa	0x11b
    a034:	96 f2       	ldaa	*0xf2
    a036:	84 20       	anda	#0x20
    a038:	8a 02       	oraa	#0x2
    a03a:	97 f2       	staa	*0xf2
    a03c:	86 80       	ldaa	#0x80
    a03e:	b7 01 1c    	staa	0x11c
    a041:	7e a2 b6    	jmp	0xa2b6
    a044:	bd a3 59    	jsr	0xa359
    a047:	86 08       	ldaa	#0x8
    a049:	b7 01 1b    	staa	0x11b
    a04c:	14 f5 04    	bset	*0xf5, #0x04
    a04f:	86 02       	ldaa	#0x2
    a051:	97 ff       	staa	*0xff
    a053:	96 f2       	ldaa	*0xf2
    a055:	84 20       	anda	#0x20
    a057:	8a 01       	oraa	#0x1
    a059:	97 f2       	staa	*0xf2
    a05b:	86 80       	ldaa	#0x80
    a05d:	b7 01 1c    	staa	0x11c
    a060:	7e a2 b6    	jmp	0xa2b6
    a063:	48          	asla
    a064:	24 41       	bcc	0x0xa0a7
    a066:	d6 f9       	ldab	*0xf9
    a068:	c1 04       	cmpb	#0x4
    a06a:	26 38       	bne	0x0xa0a4
    a06c:	f6 01 1b    	ldab	0x11b
    a06f:	c1 29       	cmpb	#0x29
    a071:	26 15       	bne	0x0xa088
    a073:	86 2a       	ldaa	#0x2a
    a075:	b7 01 1b    	staa	0x11b
    a078:	96 f2       	ldaa	*0xf2
    a07a:	84 20       	anda	#0x20
    a07c:	8a 02       	oraa	#0x2
    a07e:	97 f2       	staa	*0xf2
    a080:	86 80       	ldaa	#0x80
    a082:	b7 01 1c    	staa	0x11c
    a085:	7e a2 b6    	jmp	0xa2b6
    a088:	86 29       	ldaa	#0x29
    a08a:	b7 01 1b    	staa	0x11b
    a08d:	86 02       	ldaa	#0x2
    a08f:	97 ff       	staa	*0xff
    a091:	bd a3 59    	jsr	0xa359
    a094:	96 f2       	ldaa	*0xf2
    a096:	84 20       	anda	#0x20
    a098:	8a 01       	oraa	#0x1
    a09a:	97 f2       	staa	*0xf2
    a09c:	14 f5 02    	bset	*0xf5, #0x02
    a09f:	86 80       	ldaa	#0x80
    a0a1:	b7 01 1c    	staa	0x11c
    a0a4:	7e a2 b6    	jmp	0xa2b6
    a0a7:	d6 f9       	ldab	*0xf9
    a0a9:	c1 04       	cmpb	#0x4
    a0ab:	27 03       	beq	0x0xa0b0
    a0ad:	7e a2 b6    	jmp	0xa2b6
    a0b0:	f6 01 1b    	ldab	0x11b
    a0b3:	c1 0e       	cmpb	#0xe
    a0b5:	26 21       	bne	0x0xa0d8
    a0b7:	b6 50 00    	ldaa	0x5000
    a0ba:	27 04       	beq	0x0xa0c0
    a0bc:	81 04       	cmpa	#0x4
    a0be:	23 03       	bls	0x0xa0c3
    a0c0:	7e a2 b6    	jmp	0xa2b6
    a0c3:	86 0f       	ldaa	#0xf
    a0c5:	b7 01 1b    	staa	0x11b
    a0c8:	96 f2       	ldaa	*0xf2
    a0ca:	84 20       	anda	#0x20
    a0cc:	8a 02       	oraa	#0x2
    a0ce:	97 f2       	staa	*0xf2
    a0d0:	86 80       	ldaa	#0x80
    a0d2:	b7 01 1c    	staa	0x11c
    a0d5:	7e a2 b6    	jmp	0xa2b6
    a0d8:	86 0e       	ldaa	#0xe
    a0da:	b7 01 1b    	staa	0x11b
    a0dd:	86 02       	ldaa	#0x2
    a0df:	97 ff       	staa	*0xff
    a0e1:	bd a3 59    	jsr	0xa359
    a0e4:	96 f2       	ldaa	*0xf2
    a0e6:	84 20       	anda	#0x20
    a0e8:	8a 01       	oraa	#0x1
    a0ea:	97 f2       	staa	*0xf2
    a0ec:	14 f5 01    	bset	*0xf5, #0x01
    a0ef:	86 80       	ldaa	#0x80
    a0f1:	b7 01 1c    	staa	0x11c
    a0f4:	7e a2 b6    	jmp	0xa2b6
    a0f7:	86 02       	ldaa	#0x2
    a0f9:	b7 10 44    	staa	0x1044
    a0fc:	01          	nop
    a0fd:	01          	nop
    a0fe:	b6 10 45    	ldaa	0x1045
    a101:	b1 01 13    	cmpa	0x113
    a104:	26 03       	bne	0x0xa109
    a106:	7e a1 73    	jmp	0xa173
    a109:	16          	tab
    a10a:	b8 01 13    	eora	0x113
    a10d:	f7 01 13    	stab	0x113
    a110:	b4 01 13    	anda	0x113
    a113:	26 03       	bne	0x0xa118
    a115:	7e a1 73    	jmp	0xa173
    a118:	44          	lsra
    a119:	24 39       	bcc	0x0xa154
    a11b:	86 0c       	ldaa	#0xc
    a11d:	b1 01 1b    	cmpa	0x11b
    a120:	27 1d       	beq	0x0xa13f
    a122:	b7 01 1b    	staa	0x11b
    a125:	bd a3 59    	jsr	0xa359
    a128:	14 f6 05    	bset	*0xf6, #0x05
    a12b:	86 02       	ldaa	#0x2
    a12d:	97 ff       	staa	*0xff
    a12f:	86 80       	ldaa	#0x80
    a131:	b7 01 1c    	staa	0x11c
    a134:	96 f2       	ldaa	*0xf2
    a136:	84 20       	anda	#0x20
    a138:	8a 01       	oraa	#0x1
    a13a:	97 f2       	staa	*0xf2
    a13c:	7e a2 b6    	jmp	0xa2b6
    a13f:	96 f6       	ldaa	*0xf6
    a141:	84 fc       	anda	#0xfc
    a143:	48          	asla
    a144:	24 02       	bcc	0x0xa148
    a146:	86 04       	ldaa	#0x4
    a148:	8a 01       	oraa	#0x1
    a14a:	97 f6       	staa	*0xf6
    a14c:	86 80       	ldaa	#0x80
    a14e:	b7 01 1c    	staa	0x11c
    a151:	7e a2 b6    	jmp	0xa2b6
    a154:	bd a3 59    	jsr	0xa359
    a157:	14 f6 02    	bset	*0xf6, #0x02
    a15a:	96 f2       	ldaa	*0xf2
    a15c:	84 20       	anda	#0x20
    a15e:	8a 01       	oraa	#0x1
    a160:	97 f2       	staa	*0xf2
    a162:	86 0d       	ldaa	#0xd
    a164:	b7 01 1b    	staa	0x11b
    a167:	86 02       	ldaa	#0x2
    a169:	97 ff       	staa	*0xff
    a16b:	86 80       	ldaa	#0x80
    a16d:	b7 01 1c    	staa	0x11c
    a170:	7e a2 b6    	jmp	0xa2b6
    a173:	86 01       	ldaa	#0x1
    a175:	b7 10 44    	staa	0x1044
    a178:	01          	nop
    a179:	01          	nop
    a17a:	b6 10 45    	ldaa	0x1045
    a17d:	b1 01 14    	cmpa	0x114
    a180:	26 03       	bne	0x0xa185
    a182:	7e a2 b6    	jmp	0xa2b6
    a185:	16          	tab
    a186:	b8 01 14    	eora	0x114
    a189:	f7 01 14    	stab	0x114
    a18c:	b4 01 14    	anda	0x114
    a18f:	26 03       	bne	0x0xa194
    a191:	7e a2 b6    	jmp	0xa2b6
    a194:	48          	asla
    a195:	24 20       	bcc	0x0xa1b7
    a197:	86 80       	ldaa	#0x80
    a199:	98 f7       	eora	*0xf7
    a19b:	97 f7       	staa	*0xf7
    a19d:	86 10       	ldaa	#0x10
    a19f:	98 41       	eora	*0x41
    a1a1:	97 41       	staa	*0x41
    a1a3:	c6 41       	ldab	#0x41
    a1a5:	bd b0 fc    	jsr	0xb0fc
    a1a8:	4f          	clra
    a1a9:	c6 76       	ldab	#0x76
    a1ab:	13 41 10 02 	brclr	*0x41, #0x10, 0x0xa1b1
    a1af:	86 7f       	ldaa	#0x7f
    a1b1:	bd b1 9a    	jsr	0xb19a
    a1b4:	7e a2 a6    	jmp	0xa2a6
    a1b7:	48          	asla
    a1b8:	24 20       	bcc	0x0xa1da
    a1ba:	86 40       	ldaa	#0x40
    a1bc:	98 f7       	eora	*0xf7
    a1be:	97 f7       	staa	*0xf7
    a1c0:	86 40       	ldaa	#0x40
    a1c2:	98 44       	eora	*0x44
    a1c4:	97 44       	staa	*0x44
    a1c6:	c6 44       	ldab	#0x44
    a1c8:	bd b0 fc    	jsr	0xb0fc
    a1cb:	4f          	clra
    a1cc:	c6 3f       	ldab	#0x3f
    a1ce:	13 44 40 02 	brclr	*0x44, #0x40, 0x0xa1d4
    a1d2:	86 7f       	ldaa	#0x7f
    a1d4:	bd b1 9a    	jsr	0xb19a
    a1d7:	7e a2 a6    	jmp	0xa2a6
    a1da:	48          	asla
    a1db:	24 20       	bcc	0x0xa1fd
    a1dd:	86 20       	ldaa	#0x20
    a1df:	98 f7       	eora	*0xf7
    a1e1:	97 f7       	staa	*0xf7
    a1e3:	86 20       	ldaa	#0x20
    a1e5:	98 44       	eora	*0x44
    a1e7:	97 44       	staa	*0x44
    a1e9:	c6 44       	ldab	#0x44
    a1eb:	bd b0 fc    	jsr	0xb0fc
    a1ee:	4f          	clra
    a1ef:	c6 3e       	ldab	#0x3e
    a1f1:	13 44 20 02 	brclr	*0x44, #0x20, 0x0xa1f7
    a1f5:	86 7f       	ldaa	#0x7f
    a1f7:	bd b1 9a    	jsr	0xb19a
    a1fa:	7e a2 a6    	jmp	0xa2a6
    a1fd:	48          	asla
    a1fe:	24 20       	bcc	0x0xa220
    a200:	86 10       	ldaa	#0x10
    a202:	98 f7       	eora	*0xf7
    a204:	97 f7       	staa	*0xf7
    a206:	86 10       	ldaa	#0x10
    a208:	98 44       	eora	*0x44
    a20a:	97 44       	staa	*0x44
    a20c:	c6 44       	ldab	#0x44
    a20e:	bd b0 fc    	jsr	0xb0fc
    a211:	4f          	clra
    a212:	c6 3d       	ldab	#0x3d
    a214:	13 44 10 02 	brclr	*0x44, #0x10, 0x0xa21a
    a218:	86 7f       	ldaa	#0x7f
    a21a:	bd b1 9a    	jsr	0xb19a
    a21d:	7e a2 a6    	jmp	0xa2a6
    a220:	48          	asla
    a221:	24 20       	bcc	0x0xa243
    a223:	86 08       	ldaa	#0x8
    a225:	98 f7       	eora	*0xf7
    a227:	97 f7       	staa	*0xf7
    a229:	86 08       	ldaa	#0x8
    a22b:	98 44       	eora	*0x44
    a22d:	97 44       	staa	*0x44
    a22f:	c6 44       	ldab	#0x44
    a231:	bd b0 fc    	jsr	0xb0fc
    a234:	4f          	clra
    a235:	c6 3a       	ldab	#0x3a
    a237:	13 44 08 02 	brclr	*0x44, #0x08, 0x0xa23d
    a23b:	86 7f       	ldaa	#0x7f
    a23d:	bd b1 9a    	jsr	0xb19a
    a240:	7e a2 a6    	jmp	0xa2a6
    a243:	48          	asla
    a244:	24 20       	bcc	0x0xa266
    a246:	86 04       	ldaa	#0x4
    a248:	98 f7       	eora	*0xf7
    a24a:	97 f7       	staa	*0xf7
    a24c:	86 04       	ldaa	#0x4
    a24e:	98 44       	eora	*0x44
    a250:	97 44       	staa	*0x44
    a252:	c6 44       	ldab	#0x44
    a254:	bd b0 fc    	jsr	0xb0fc
    a257:	4f          	clra
    a258:	c6 39       	ldab	#0x39
    a25a:	13 44 04 02 	brclr	*0x44, #0x04, 0x0xa260
    a25e:	86 7f       	ldaa	#0x7f
    a260:	bd b1 9a    	jsr	0xb19a
    a263:	7e a2 a6    	jmp	0xa2a6
    a266:	48          	asla
    a267:	24 20       	bcc	0x0xa289
    a269:	86 02       	ldaa	#0x2
    a26b:	98 f7       	eora	*0xf7
    a26d:	97 f7       	staa	*0xf7
    a26f:	86 02       	ldaa	#0x2
    a271:	98 44       	eora	*0x44
    a273:	97 44       	staa	*0x44
    a275:	c6 44       	ldab	#0x44
    a277:	bd b0 fc    	jsr	0xb0fc
    a27a:	4f          	clra
    a27b:	c6 38       	ldab	#0x38
    a27d:	13 44 02 02 	brclr	*0x44, #0x02, 0x0xa283
    a281:	86 7f       	ldaa	#0x7f
    a283:	bd b1 9a    	jsr	0xb19a
    a286:	7e a2 a6    	jmp	0xa2a6
    a289:	86 01       	ldaa	#0x1
    a28b:	98 f7       	eora	*0xf7
    a28d:	97 f7       	staa	*0xf7
    a28f:	86 01       	ldaa	#0x1
    a291:	98 44       	eora	*0x44
    a293:	97 44       	staa	*0x44
    a295:	c6 44       	ldab	#0x44
    a297:	bd b0 fc    	jsr	0xb0fc
    a29a:	4f          	clra
    a29b:	c6 37       	ldab	#0x37
    a29d:	13 44 01 02 	brclr	*0x44, #0x01, 0x0xa2a3
    a2a1:	86 7f       	ldaa	#0x7f
    a2a3:	bd b1 9a    	jsr	0xb19a
    a2a6:	7d 00 fb    	tst	0xfb
    a2a9:	26 0b       	bne	0x0xa2b6
    a2ab:	14 fb 01    	bset	*0xfb, #0x01
    a2ae:	14 f2 20    	bset	*0xf2, #0x20
    a2b1:	86 1f       	ldaa	#0x1f
    a2b3:	b7 01 66    	staa	0x166
    a2b6:	86 40       	ldaa	#0x40
    a2b8:	b7 10 23    	staa	0x1023
    a2bb:	fc 10 0e    	ldd	0x100e
    a2be:	c3 27 10    	addd	#0x2710
    a2c1:	fd 10 18    	std	0x1018
    a2c4:	fe 01 c0    	ldx	0x1c0
    a2c7:	bc 01 c2    	cpx	0x1c2
    a2ca:	26 03       	bne	0x0xa2cf
    a2cc:	7e ac 59    	jmp	0xac59
    a2cf:	18 fe 01 6c 	ldy	0x16c
    a2d3:	18 3c       	pshy
    a2d5:	7e 92 be    	jmp	0x92be
    a2d8:	96 fa       	ldaa	*0xfa
    a2da:	4c          	inca
    a2db:	f6 01 11    	ldab	0x111
    a2de:	c5 04       	bitb	#0x4
    a2e0:	27 02       	beq	0x0xa2e4
    a2e2:	8b 09       	adda	#0x9
    a2e4:	84 7f       	anda	#0x7f
    a2e6:	81 51       	cmpa	#0x51
    a2e8:	26 04       	bne	0x0xa2ee
    a2ea:	86 53       	ldaa	#0x53
    a2ec:	20 06       	bra	0x0xa2f4
    a2ee:	81 52       	cmpa	#0x52
    a2f0:	26 02       	bne	0x0xa2f4
    a2f2:	86 53       	ldaa	#0x53
    a2f4:	97 fa       	staa	*0xfa
    a2f6:	bd ad 9e    	jsr	0xad9e
    a2f9:	b7 7f f7    	staa	0x7ff7
    a2fc:	bd ad c4    	jsr	0xadc4
    a2ff:	7c 01 7e    	inc	0x17e
    a302:	39          	rts
    a303:	96 fa       	ldaa	*0xfa
    a305:	4a          	deca
    a306:	f6 01 11    	ldab	0x111
    a309:	c5 04       	bitb	#0x4
    a30b:	27 02       	beq	0x0xa30f
    a30d:	80 09       	suba	#0x9
    a30f:	84 7f       	anda	#0x7f
    a311:	81 52       	cmpa	#0x52
    a313:	26 04       	bne	0x0xa319
    a315:	86 50       	ldaa	#0x50
    a317:	20 05       	bra	0x0xa31e
    a319:	81 51       	cmpa	#0x51
    a31b:	26 01       	bne	0x0xa31e
    a31d:	4a          	deca
    a31e:	97 fa       	staa	*0xfa
    a320:	bd ad 9e    	jsr	0xad9e
    a323:	b7 7f f7    	staa	0x7ff7
    a326:	bd ad c4    	jsr	0xadc4
    a329:	7c 01 7e    	inc	0x17e
    a32c:	39          	rts
    a32d:	b6 01 6a    	ldaa	0x16a
    a330:	4c          	inca
    a331:	84 7f       	anda	#0x7f
    a333:	b7 01 6a    	staa	0x16a
    a336:	bd ad 9e    	jsr	0xad9e
    a339:	b7 7f f7    	staa	0x7ff7
    a33c:	bd ad c4    	jsr	0xadc4
    a33f:	7c 01 7e    	inc	0x17e
    a342:	39          	rts
    a343:	b6 01 6a    	ldaa	0x16a
    a346:	4a          	deca
    a347:	84 7f       	anda	#0x7f
    a349:	b7 01 6a    	staa	0x16a
    a34c:	bd ad 9e    	jsr	0xad9e
    a34f:	b7 7f f7    	staa	0x7ff7
    a352:	bd ad c4    	jsr	0xadc4
    a355:	7c 01 7e    	inc	0x17e
    a358:	39          	rts
    a359:	86 17       	ldaa	#0x17
    a35b:	94 f3       	anda	*0xf3
    a35d:	97 f3       	staa	*0xf3
    a35f:	86 e4       	ldaa	#0xe4
    a361:	94 f4       	anda	*0xf4
    a363:	97 f4       	staa	*0xf4
    a365:	86 20       	ldaa	#0x20
    a367:	94 f5       	anda	*0xf5
    a369:	97 f5       	staa	*0xf5
    a36b:	86 00       	ldaa	#0x0
    a36d:	97 f6       	staa	*0xf6
    a36f:	39          	rts
    a370:	13 f3 10 08 	brclr	*0xf3, #0x10, 0x0xa37c
    a374:	b6 10 22    	ldaa	0x1022
    a377:	84 f7       	anda	#0xf7
    a379:	b7 10 22    	staa	0x1022
    a37c:	96 f9       	ldaa	*0xf9
    a37e:	81 01       	cmpa	#0x1
    a380:	22 1f       	bhi	0x0xa3a1
    a382:	4f          	clra
    a383:	b7 10 22    	staa	0x1022
    a386:	b6 10 2d    	ldaa	0x102d
    a389:	36          	psha
    a38a:	84 7f       	anda	#0x7f
    a38c:	b7 10 2d    	staa	0x102d
    a38f:	96 f9       	ldaa	*0xf9
    a391:	d6 fa       	ldab	*0xfa
    a393:	bd 79 00    	jsr	0x7900
    a396:	32          	pula
    a397:	b7 10 2d    	staa	0x102d
    a39a:	86 80       	ldaa	#0x80
    a39c:	b7 10 22    	staa	0x1022
    a39f:	20 1c       	bra	0x0xa3bd
    a3a1:	96 fa       	ldaa	*0xfa
    a3a3:	c6 b0       	ldab	#0xb0
    a3a5:	3d          	mul
    a3a6:	c3 20 00    	addd	#0x2000
    a3a9:	8f          	xgdx
    a3aa:	18 ce 00 20 	ldy	#0x20
    a3ae:	c6 b0       	ldab	#0xb0
    a3b0:	a6 00       	ldaa	0x0,x
    a3b2:	84 7f       	anda	#0x7f
    a3b4:	18 a7 00    	staa	0x0,y
    a3b7:	08          	inx
    a3b8:	18 08       	iny
    a3ba:	5a          	decb
    a3bb:	26 f3       	bne	0x0xa3b0
    a3bd:	96 73       	ldaa	*0x73
    a3bf:	84 07       	anda	#0x7
    a3c1:	97 73       	staa	*0x73
    a3c3:	96 41       	ldaa	*0x41
    a3c5:	84 10       	anda	#0x10
    a3c7:	48          	asla
    a3c8:	48          	asla
    a3c9:	48          	asla
    a3ca:	9a 44       	oraa	*0x44
    a3cc:	97 f7       	staa	*0xf7
    a3ce:	96 f3       	ldaa	*0xf3
    a3d0:	84 10       	anda	#0x10
    a3d2:	9a 73       	oraa	*0x73
    a3d4:	97 f3       	staa	*0xf3
    a3d6:	96 4a       	ldaa	*0x4a
    a3d8:	48          	asla
    a3d9:	48          	asla
    a3da:	48          	asla
    a3db:	48          	asla
    a3dc:	48          	asla
    a3dd:	97 f4       	staa	*0xf4
    a3df:	7d 00 d0    	tst	0xd0
    a3e2:	27 05       	beq	0x0xa3e9
    a3e4:	7f 00 d0    	clr	0xd0
    a3e7:	20 08       	bra	0x0xa3f1
    a3e9:	7d 10 29    	tst	0x1029
    a3ec:	2a fb       	bpl	0x0xa3e9
    a3ee:	b6 10 2a    	ldaa	0x102a
    a3f1:	01          	nop
    a3f2:	01          	nop
    a3f3:	01          	nop
    a3f4:	01          	nop
    a3f5:	5f          	clrb
    a3f6:	f7 10 42    	stab	0x1042
    a3f9:	7c 00 d0    	inc	0xd0
    a3fc:	13 f3 10 06 	brclr	*0xf3, #0x10, 0x0xa406
    a400:	96 6a       	ldaa	*0x6a
    a402:	84 20       	anda	#0x20
    a404:	97 f5       	staa	*0xf5
    a406:	96 6a       	ldaa	*0x6a
    a408:	84 20       	anda	#0x20
    a40a:	d6 f5       	ldab	*0xf5
    a40c:	c4 20       	andb	#0x20
    a40e:	d7 f5       	stab	*0xf5
    a410:	36          	psha
    a411:	98 f5       	eora	*0xf5
    a413:	26 05       	bne	0x0xa41a
    a415:	32          	pula
    a416:	97 f5       	staa	*0xf5
    a418:	20 24       	bra	0x0xa43e
    a41a:	86 80       	ldaa	#0x80
    a41c:	b7 10 2a    	staa	0x102a
    a41f:	cc 01 00    	ldd	#0x100
    a422:	fd 01 08    	std	0x108
    a425:	4f          	clra
    a426:	97 d0       	staa	*0xd0
    a428:	97 f8       	staa	*0xf8
    a42a:	fd 01 00    	std	0x100
    a42d:	fd 01 02    	std	0x102
    a430:	fd 01 04    	std	0x104
    a433:	fd 01 06    	std	0x106
    a436:	32          	pula
    a437:	97 f5       	staa	*0xf5
    a439:	86 01       	ldaa	#0x1
    a43b:	b7 01 09    	staa	0x109
    a43e:	c6 88       	ldab	#0x88
    a440:	ce 00 20    	ldx	#0x20
    a443:	7d 00 d0    	tst	0xd0
    a446:	27 05       	beq	0x0xa44d
    a448:	7f 00 d0    	clr	0xd0
    a44b:	20 08       	bra	0x0xa455
    a44d:	7d 10 29    	tst	0x1029
    a450:	2a fb       	bpl	0x0xa44d
    a452:	b6 10 2a    	ldaa	0x102a
    a455:	86 82       	ldaa	#0x82
    a457:	b7 10 2a    	staa	0x102a
    a45a:	7d 10 29    	tst	0x1029
    a45d:	2a fb       	bpl	0x0xa45a
    a45f:	b6 10 2a    	ldaa	0x102a
    a462:	a6 00       	ldaa	0x0,x
    a464:	b7 10 2a    	staa	0x102a
    a467:	08          	inx
    a468:	5a          	decb
    a469:	26 ef       	bne	0x0xa45a
    a46b:	c6 fe       	ldab	#0xfe
    a46d:	7d 10 29    	tst	0x1029
    a470:	2a fb       	bpl	0x0xa46d
    a472:	b6 10 2a    	ldaa	0x102a
    a475:	01          	nop
    a476:	01          	nop
    a477:	01          	nop
    a478:	01          	nop
    a479:	f7 10 42    	stab	0x1042
    a47c:	01          	nop
    a47d:	01          	nop
    a47e:	01          	nop
    a47f:	01          	nop
    a480:	01          	nop
    a481:	01          	nop
    a482:	01          	nop
    a483:	a6 00       	ldaa	0x0,x
    a485:	b7 10 2a    	staa	0x102a
    a488:	7d 10 29    	tst	0x1029
    a48b:	2a fb       	bpl	0x0xa488
    a48d:	b6 10 2a    	ldaa	0x102a
    a490:	01          	nop
    a491:	01          	nop
    a492:	01          	nop
    a493:	01          	nop
    a494:	a6 08       	ldaa	0x8,x
    a496:	b7 10 2a    	staa	0x102a
    a499:	7d 10 29    	tst	0x1029
    a49c:	2a fb       	bpl	0x0xa499
    a49e:	b6 10 2a    	ldaa	0x102a
    a4a1:	01          	nop
    a4a2:	01          	nop
    a4a3:	a6 10       	ldaa	0x10,x
    a4a5:	b7 10 2a    	staa	0x102a
    a4a8:	08          	inx
    a4a9:	0d          	sec
    a4aa:	59          	rolb
    a4ab:	24 18       	bcc	0x0xa4c5
    a4ad:	7d 10 29    	tst	0x1029
    a4b0:	2a fb       	bpl	0x0xa4ad
    a4b2:	b6 10 2a    	ldaa	0x102a
    a4b5:	01          	nop
    a4b6:	01          	nop
    a4b7:	01          	nop
    a4b8:	01          	nop
    a4b9:	f7 10 42    	stab	0x1042
    a4bc:	01          	nop
    a4bd:	01          	nop
    a4be:	01          	nop
    a4bf:	01          	nop
    a4c0:	01          	nop
    a4c1:	01          	nop
    a4c2:	01          	nop
    a4c3:	20 be       	bra	0x0xa483
    a4c5:	7d 10 29    	tst	0x1029
    a4c8:	2a fb       	bpl	0x0xa4c5
    a4ca:	b6 10 2a    	ldaa	0x102a
    a4cd:	4f          	clra
    a4ce:	01          	nop
    a4cf:	01          	nop
    a4d0:	01          	nop
    a4d1:	01          	nop
    a4d2:	b7 10 42    	staa	0x1042
    a4d5:	01          	nop
    a4d6:	01          	nop
    a4d7:	01          	nop
    a4d8:	01          	nop
    a4d9:	01          	nop
    a4da:	01          	nop
    a4db:	01          	nop
    a4dc:	b6 01 6e    	ldaa	0x16e
    a4df:	b7 10 2a    	staa	0x102a
    a4e2:	7d 10 29    	tst	0x1029
    a4e5:	2a fb       	bpl	0x0xa4e2
    a4e7:	b6 10 2a    	ldaa	0x102a
    a4ea:	86 89       	ldaa	#0x89
    a4ec:	b7 10 2a    	staa	0x102a
    a4ef:	7d 10 29    	tst	0x1029
    a4f2:	2a fb       	bpl	0x0xa4ef
    a4f4:	b6 10 2a    	ldaa	0x102a
    a4f7:	86 83       	ldaa	#0x83
    a4f9:	b7 10 2a    	staa	0x102a
    a4fc:	7d 10 29    	tst	0x1029
    a4ff:	2a fb       	bpl	0x0xa4fc
    a501:	b6 10 2a    	ldaa	0x102a
    a504:	86 8c       	ldaa	#0x8c
    a506:	b7 10 2a    	staa	0x102a
    a509:	7d 10 29    	tst	0x1029
    a50c:	2a fb       	bpl	0x0xa509
    a50e:	b6 10 2a    	ldaa	0x102a
    a511:	96 d6       	ldaa	*0xd6
    a513:	b7 10 2a    	staa	0x102a
    a516:	7d 10 29    	tst	0x1029
    a519:	2a fb       	bpl	0x0xa516
    a51b:	01          	nop
    a51c:	01          	nop
    a51d:	01          	nop
    a51e:	01          	nop
    a51f:	86 ff       	ldaa	#0xff
    a521:	b7 10 42    	staa	0x1042
    a524:	7c 00 d0    	inc	0xd0
    a527:	bd ad 9e    	jsr	0xad9e
    a52a:	c6 20       	ldab	#0x20
    a52c:	4f          	clra
    a52d:	ce 1f 20    	ldx	#0x1f20
    a530:	a7 00       	staa	0x0,x
    a532:	08          	inx
    a533:	5a          	decb
    a534:	26 fa       	bne	0x0xa530
    a536:	13 f3 10 0b 	brclr	*0xf3, #0x10, 0x0xa545
    a53a:	b6 10 22    	ldaa	0x1022
    a53d:	8a 08       	oraa	#0x8
    a53f:	b7 10 22    	staa	0x1022
    a542:	7e ad c4    	jmp	0xadc4
    a545:	96 71       	ldaa	*0x71
    a547:	97 db       	staa	*0xdb
    a549:	96 72       	ldaa	*0x72
    a54b:	97 dc       	staa	*0xdc
    a54d:	7e ad c4    	jmp	0xadc4
    a550:	c6 b0       	ldab	#0xb0
    a552:	3d          	mul
    a553:	c3 20 00    	addd	#0x2000
    a556:	8f          	xgdx
    a557:	18 ce 00 20 	ldy	#0x20
    a55b:	c6 b0       	ldab	#0xb0
    a55d:	a6 00       	ldaa	0x0,x
    a55f:	84 7f       	anda	#0x7f
    a561:	18 a7 00    	staa	0x0,y
    a564:	08          	inx
    a565:	18 08       	iny
    a567:	5a          	decb
    a568:	26 f3       	bne	0x0xa55d
    a56a:	39          	rts
    a56b:	96 73       	ldaa	*0x73
    a56d:	84 07       	anda	#0x7
    a56f:	97 73       	staa	*0x73
    a571:	96 41       	ldaa	*0x41
    a573:	84 10       	anda	#0x10
    a575:	48          	asla
    a576:	48          	asla
    a577:	48          	asla
    a578:	9a 44       	oraa	*0x44
    a57a:	97 f7       	staa	*0xf7
    a57c:	96 f3       	ldaa	*0xf3
    a57e:	84 10       	anda	#0x10
    a580:	9a 73       	oraa	*0x73
    a582:	97 f3       	staa	*0xf3
    a584:	96 4a       	ldaa	*0x4a
    a586:	48          	asla
    a587:	48          	asla
    a588:	48          	asla
    a589:	48          	asla
    a58a:	48          	asla
    a58b:	97 f4       	staa	*0xf4
    a58d:	39          	rts
    a58e:	b6 01 6a    	ldaa	0x16a
    a591:	c6 50       	ldab	#0x50
    a593:	3d          	mul
    a594:	c3 20 00    	addd	#0x2000
    a597:	8f          	xgdx
    a598:	18 ce 50 00 	ldy	#0x5000
    a59c:	c6 50       	ldab	#0x50
    a59e:	a6 00       	ldaa	0x0,x
    a5a0:	84 7f       	anda	#0x7f
    a5a2:	18 a7 00    	staa	0x0,y
    a5a5:	08          	inx
    a5a6:	18 08       	iny
    a5a8:	5a          	decb
    a5a9:	26 f3       	bne	0x0xa59e
    a5ab:	18 ce 50 50 	ldy	#0x5050
    a5af:	ce de e3    	ldx	#0xdee3
    a5b2:	7f 01 6b    	clr	0x16b
    a5b5:	7f 01 7d    	clr	0x17d
    a5b8:	f6 50 00    	ldab	0x5000
    a5bb:	c1 05       	cmpb	#0x5
    a5bd:	22 03       	bhi	0x0xa5c2
    a5bf:	7e a6 44    	jmp	0xa644
    a5c2:	ce 50 01    	ldx	#0x5001
    a5c5:	b6 01 6b    	ldaa	0x16b
    a5c8:	c6 04       	ldab	#0x4
    a5ca:	3d          	mul
    a5cb:	3a          	abx
    a5cc:	e6 03       	ldab	0x3,x
    a5ce:	c4 01       	andb	#0x1
    a5d0:	37          	pshb
    a5d1:	e6 02       	ldab	0x2,x
    a5d3:	c4 3f       	andb	#0x3f
    a5d5:	37          	pshb
    a5d6:	ce df 2b    	ldx	#0xdf2b
    a5d9:	3a          	abx
    a5da:	b6 01 7d    	ldaa	0x17d
    a5dd:	36          	psha
    a5de:	fb 01 7d    	addb	0x17d
    a5e1:	f7 01 7d    	stab	0x17d
    a5e4:	37          	pshb
    a5e5:	16          	tab
    a5e6:	a6 00       	ldaa	0x0,x
    a5e8:	5a          	decb
    a5e9:	2b 03       	bmi	0x0xa5ee
    a5eb:	48          	asla
    a5ec:	20 fa       	bra	0x0xa5e8
    a5ee:	18 a7 00    	staa	0x0,y
    a5f1:	ce 51 00    	ldx	#0x5100
    a5f4:	f6 01 6b    	ldab	0x16b
    a5f7:	86 10       	ldaa	#0x10
    a5f9:	3d          	mul
    a5fa:	3a          	abx
    a5fb:	8f          	xgdx
    a5fc:	37          	pshb
    a5fd:	8f          	xgdx
    a5fe:	33          	pulb
    a5ff:	e7 0b       	stab	0xb,x
    a601:	32          	pula
    a602:	4a          	deca
    a603:	ab 0b       	adda	0xb,x
    a605:	a7 09       	staa	0x9,x
    a607:	32          	pula
    a608:	ab 0b       	adda	0xb,x
    a60a:	a7 08       	staa	0x8,x
    a60c:	32          	pula
    a60d:	a7 0a       	staa	0xa,x
    a60f:	e6 08       	ldab	0x8,x
    a611:	c4 07       	andb	#0x7
    a613:	3a          	abx
    a614:	3c          	pshx
    a615:	ce 51 80    	ldx	#0x5180
    a618:	f6 01 6b    	ldab	0x16b
    a61b:	58          	aslb
    a61c:	3a          	abx
    a61d:	32          	pula
    a61e:	33          	pulb
    a61f:	ed 00       	std	0x0,x
    a621:	32          	pula
    a622:	18 a7 08    	staa	0x8,y
    a625:	18 08       	iny
    a627:	7c 01 6b    	inc	0x16b
    a62a:	d6 f0       	ldab	*0xf0
    a62c:	5c          	incb
    a62d:	f1 01 7d    	cmpb	0x17d
    a630:	22 90       	bhi	0x0xa5c2
    a632:	f6 01 6b    	ldab	0x16b
    a635:	c1 08       	cmpb	#0x8
    a637:	27 35       	beq	0x0xa66e
    a639:	ce 50 50    	ldx	#0x5050
    a63c:	3a          	abx
    a63d:	6f 00       	clr	0x0,x
    a63f:	7c 01 6b    	inc	0x16b
    a642:	20 ee       	bra	0x0xa632
    a644:	c1 05       	cmpb	#0x5
    a646:	25 15       	bcs	0x0xa65d
    a648:	ce df 34    	ldx	#0xdf34
    a64b:	5f          	clrb
    a64c:	96 f0       	ldaa	*0xf0
    a64e:	81 02       	cmpa	#0x2
    a650:	23 0b       	bls	0x0xa65d
    a652:	5c          	incb
    a653:	81 04       	cmpa	#0x4
    a655:	23 06       	bls	0x0xa65d
    a657:	5c          	incb
    a658:	81 06       	cmpa	#0x6
    a65a:	23 01       	bls	0x0xa65d
    a65c:	5c          	incb
    a65d:	86 08       	ldaa	#0x8
    a65f:	3d          	mul
    a660:	3a          	abx
    a661:	c6 08       	ldab	#0x8
    a663:	a6 00       	ldaa	0x0,x
    a665:	18 a7 00    	staa	0x0,y
    a668:	08          	inx
    a669:	18 08       	iny
    a66b:	5a          	decb
    a66c:	26 f5       	bne	0x0xa663
    a66e:	86 07       	ldaa	#0x7
    a670:	b7 01 6b    	staa	0x16b
    a673:	ce 50 57    	ldx	#0x5057
    a676:	e6 00       	ldab	0x0,x
    a678:	26 06       	bne	0x0xa680
    a67a:	7a 01 6b    	dec	0x16b
    a67d:	09          	dex
    a67e:	20 f6       	bra	0x0xa676
    a680:	7d 00 d0    	tst	0xd0
    a683:	27 05       	beq	0x0xa68a
    a685:	7f 00 d0    	clr	0xd0
    a688:	20 08       	bra	0x0xa692
    a68a:	7d 10 29    	tst	0x1029
    a68d:	2a fb       	bpl	0x0xa68a
    a68f:	b6 10 2a    	ldaa	0x102a
    a692:	53          	comb
    a693:	01          	nop
    a694:	01          	nop
    a695:	01          	nop
    a696:	01          	nop
    a697:	f7 10 42    	stab	0x1042
    a69a:	ce 50 01    	ldx	#0x5001
    a69d:	f6 01 6b    	ldab	0x16b
    a6a0:	86 04       	ldaa	#0x4
    a6a2:	3d          	mul
    a6a3:	3a          	abx
    a6a4:	a6 01       	ldaa	0x1,x
    a6a6:	36          	psha
    a6a7:	86 82       	ldaa	#0x82
    a6a9:	b7 10 2a    	staa	0x102a
    a6ac:	a6 00       	ldaa	0x0,x
    a6ae:	81 01       	cmpa	#0x1
    a6b0:	22 20       	bhi	0x0xa6d2
    a6b2:	33          	pulb
    a6b3:	4f          	clra
    a6b4:	b7 10 22    	staa	0x1022
    a6b7:	b6 10 2d    	ldaa	0x102d
    a6ba:	36          	psha
    a6bb:	84 7f       	anda	#0x7f
    a6bd:	b7 10 2d    	staa	0x102d
    a6c0:	a6 00       	ldaa	0x0,x
    a6c2:	3c          	pshx
    a6c3:	bd 79 00    	jsr	0x7900
    a6c6:	38          	pulx
    a6c7:	32          	pula
    a6c8:	b7 10 2d    	staa	0x102d
    a6cb:	86 80       	ldaa	#0x80
    a6cd:	b7 10 22    	staa	0x1022
    a6d0:	20 12       	bra	0x0xa6e4
    a6d2:	97 f9       	staa	*0xf9
    a6d4:	bd ad c4    	jsr	0xadc4
    a6d7:	32          	pula
    a6d8:	3c          	pshx
    a6d9:	bd a5 50    	jsr	0xa550
    a6dc:	38          	pulx
    a6dd:	c6 04       	ldab	#0x4
    a6df:	d7 f9       	stab	*0xf9
    a6e1:	bd ad c4    	jsr	0xadc4
    a6e4:	18 ce 00 20 	ldy	#0x20
    a6e8:	c6 88       	ldab	#0x88
    a6ea:	7d 10 29    	tst	0x1029
    a6ed:	2a fb       	bpl	0x0xa6ea
    a6ef:	b6 10 2a    	ldaa	0x102a
    a6f2:	18 a6 00    	ldaa	0x0,y
    a6f5:	b7 10 2a    	staa	0x102a
    a6f8:	18 08       	iny
    a6fa:	5a          	decb
    a6fb:	26 ed       	bne	0x0xa6ea
    a6fd:	7a 01 6b    	dec	0x16b
    a700:	2b 0a       	bmi	0x0xa70c
    a702:	f6 01 6b    	ldab	0x16b
    a705:	ce 50 50    	ldx	#0x5050
    a708:	3a          	abx
    a709:	7e a6 76    	jmp	0xa676
    a70c:	86 04       	ldaa	#0x4
    a70e:	97 f9       	staa	*0xf9
    a710:	bd ad c4    	jsr	0xadc4
    a713:	ce 50 28    	ldx	#0x5028
    a716:	c6 fe       	ldab	#0xfe
    a718:	7d 10 29    	tst	0x1029
    a71b:	2a fb       	bpl	0x0xa718
    a71d:	b6 10 2a    	ldaa	0x102a
    a720:	01          	nop
    a721:	01          	nop
    a722:	01          	nop
    a723:	01          	nop
    a724:	f7 10 42    	stab	0x1042
    a727:	01          	nop
    a728:	01          	nop
    a729:	01          	nop
    a72a:	01          	nop
    a72b:	01          	nop
    a72c:	01          	nop
    a72d:	01          	nop
    a72e:	a6 00       	ldaa	0x0,x
    a730:	b7 10 2a    	staa	0x102a
    a733:	7d 10 29    	tst	0x1029
    a736:	2a fb       	bpl	0x0xa733
    a738:	b6 10 2a    	ldaa	0x102a
    a73b:	01          	nop
    a73c:	01          	nop
    a73d:	01          	nop
    a73e:	01          	nop
    a73f:	a6 08       	ldaa	0x8,x
    a741:	b7 10 2a    	staa	0x102a
    a744:	7d 10 29    	tst	0x1029
    a747:	2a fb       	bpl	0x0xa744
    a749:	b6 10 2a    	ldaa	0x102a
    a74c:	01          	nop
    a74d:	01          	nop
    a74e:	a6 10       	ldaa	0x10,x
    a750:	b7 10 2a    	staa	0x102a
    a753:	08          	inx
    a754:	0d          	sec
    a755:	59          	rolb
    a756:	24 18       	bcc	0x0xa770
    a758:	7d 10 29    	tst	0x1029
    a75b:	2a fb       	bpl	0x0xa758
    a75d:	b6 10 2a    	ldaa	0x102a
    a760:	01          	nop
    a761:	01          	nop
    a762:	01          	nop
    a763:	01          	nop
    a764:	f7 10 42    	stab	0x1042
    a767:	01          	nop
    a768:	01          	nop
    a769:	01          	nop
    a76a:	01          	nop
    a76b:	01          	nop
    a76c:	01          	nop
    a76d:	01          	nop
    a76e:	20 be       	bra	0x0xa72e
    a770:	7d 10 29    	tst	0x1029
    a773:	2a fb       	bpl	0x0xa770
    a775:	b6 10 2a    	ldaa	0x102a
    a778:	4f          	clra
    a779:	01          	nop
    a77a:	01          	nop
    a77b:	01          	nop
    a77c:	01          	nop
    a77d:	b7 10 42    	staa	0x1042
    a780:	01          	nop
    a781:	01          	nop
    a782:	01          	nop
    a783:	01          	nop
    a784:	01          	nop
    a785:	01          	nop
    a786:	01          	nop
    a787:	b6 01 6e    	ldaa	0x16e
    a78a:	b7 10 2a    	staa	0x102a
    a78d:	7d 10 29    	tst	0x1029
    a790:	2a fb       	bpl	0x0xa78d
    a792:	b6 10 2a    	ldaa	0x102a
    a795:	86 89       	ldaa	#0x89
    a797:	b7 10 2a    	staa	0x102a
    a79a:	bd a5 6b    	jsr	0xa56b
    a79d:	bd ad 9e    	jsr	0xad9e
    a7a0:	c6 20       	ldab	#0x20
    a7a2:	4f          	clra
    a7a3:	ce 1f 20    	ldx	#0x1f20
    a7a6:	a7 00       	staa	0x0,x
    a7a8:	08          	inx
    a7a9:	5a          	decb
    a7aa:	26 fa       	bne	0x0xa7a6
    a7ac:	86 04       	ldaa	#0x4
    a7ae:	97 f9       	staa	*0xf9
    a7b0:	bd ad c4    	jsr	0xadc4
    a7b3:	86 07       	ldaa	#0x7
    a7b5:	b7 01 6b    	staa	0x16b
    a7b8:	ce 50 57    	ldx	#0x5057
    a7bb:	e6 00       	ldab	0x0,x
    a7bd:	26 06       	bne	0x0xa7c5
    a7bf:	7a 01 6b    	dec	0x16b
    a7c2:	09          	dex
    a7c3:	20 f6       	bra	0x0xa7bb
    a7c5:	7d 00 d0    	tst	0xd0
    a7c8:	27 05       	beq	0x0xa7cf
    a7ca:	7f 00 d0    	clr	0xd0
    a7cd:	20 08       	bra	0x0xa7d7
    a7cf:	7d 10 29    	tst	0x1029
    a7d2:	2a fb       	bpl	0x0xa7cf
    a7d4:	b6 10 2a    	ldaa	0x102a
    a7d7:	53          	comb
    a7d8:	01          	nop
    a7d9:	01          	nop
    a7da:	01          	nop
    a7db:	01          	nop
    a7dc:	f7 10 42    	stab	0x1042
    a7df:	ce 50 03    	ldx	#0x5003
    a7e2:	f6 01 6b    	ldab	0x16b
    a7e5:	86 04       	ldaa	#0x4
    a7e7:	3d          	mul
    a7e8:	3a          	abx
    a7e9:	a6 00       	ldaa	0x0,x
    a7eb:	84 40       	anda	#0x40
    a7ed:	26 0d       	bne	0x0xa7fc
    a7ef:	86 8c       	ldaa	#0x8c
    a7f1:	b7 10 2a    	staa	0x102a
    a7f4:	7d 10 29    	tst	0x1029
    a7f7:	2a fb       	bpl	0x0xa7f4
    a7f9:	b6 10 2a    	ldaa	0x102a
    a7fc:	86 83       	ldaa	#0x83
    a7fe:	b7 10 2a    	staa	0x102a
    a801:	7d 10 29    	tst	0x1029
    a804:	2a fb       	bpl	0x0xa801
    a806:	b6 10 2a    	ldaa	0x102a
    a809:	86 8c       	ldaa	#0x8c
    a80b:	b7 10 2a    	staa	0x102a
    a80e:	7d 10 29    	tst	0x1029
    a811:	2a fb       	bpl	0x0xa80e
    a813:	b6 10 2a    	ldaa	0x102a
    a816:	96 d6       	ldaa	*0xd6
    a818:	f6 50 00    	ldab	0x5000
    a81b:	c1 06       	cmpb	#0x6
    a81d:	27 06       	beq	0x0xa825
    a81f:	a6 01       	ldaa	0x1,x
    a821:	d6 d6       	ldab	*0xd6
    a823:	3d          	mul
    a824:	05          	asld
    a825:	b7 10 2a    	staa	0x102a
    a828:	7a 01 6b    	dec	0x16b
    a82b:	2b 0a       	bmi	0x0xa837
    a82d:	f6 01 6b    	ldab	0x16b
    a830:	ce 50 50    	ldx	#0x5050
    a833:	3a          	abx
    a834:	7e a7 bb    	jmp	0xa7bb
    a837:	7f 01 6b    	clr	0x16b
    a83a:	39          	rts
    a83b:	ce 50 50    	ldx	#0x5050
    a83e:	f6 01 6b    	ldab	0x16b
    a841:	3a          	abx
    a842:	e6 00       	ldab	0x0,x
    a844:	7d 00 d0    	tst	0xd0
    a847:	27 05       	beq	0x0xa84e
    a849:	7f 00 d0    	clr	0xd0
    a84c:	20 08       	bra	0x0xa856
    a84e:	b6 10 29    	ldaa	0x1029
    a851:	2a fb       	bpl	0x0xa84e
    a853:	b6 10 2a    	ldaa	0x102a
    a856:	01          	nop
    a857:	01          	nop
    a858:	01          	nop
    a859:	01          	nop
    a85a:	53          	comb
    a85b:	f7 10 42    	stab	0x1042
    a85e:	01          	nop
    a85f:	01          	nop
    a860:	01          	nop
    a861:	01          	nop
    a862:	01          	nop
    a863:	01          	nop
    a864:	ce 00 20    	ldx	#0x20
    a867:	86 82       	ldaa	#0x82
    a869:	b7 10 2a    	staa	0x102a
    a86c:	c6 88       	ldab	#0x88
    a86e:	7d 10 29    	tst	0x1029
    a871:	2a fb       	bpl	0x0xa86e
    a873:	b6 10 2a    	ldaa	0x102a
    a876:	a6 00       	ldaa	0x0,x
    a878:	b7 10 2a    	staa	0x102a
    a87b:	08          	inx
    a87c:	5a          	decb
    a87d:	26 ef       	bne	0x0xa86e
    a87f:	7d 10 29    	tst	0x1029
    a882:	2a fb       	bpl	0x0xa87f
    a884:	b6 10 2a    	ldaa	0x102a
    a887:	86 89       	ldaa	#0x89
    a889:	b7 10 2a    	staa	0x102a
    a88c:	ce 50 03    	ldx	#0x5003
    a88f:	f6 01 6b    	ldab	0x16b
    a892:	86 04       	ldaa	#0x4
    a894:	3d          	mul
    a895:	3a          	abx
    a896:	a6 00       	ldaa	0x0,x
    a898:	84 40       	anda	#0x40
    a89a:	26 0d       	bne	0x0xa8a9
    a89c:	7d 10 29    	tst	0x1029
    a89f:	2a fb       	bpl	0x0xa89c
    a8a1:	b6 10 2a    	ldaa	0x102a
    a8a4:	86 8c       	ldaa	#0x8c
    a8a6:	b7 10 2a    	staa	0x102a
    a8a9:	39          	rts
    a8aa:	b6 01 6a    	ldaa	0x16a
    a8ad:	c6 50       	ldab	#0x50
    a8af:	3d          	mul
    a8b0:	c3 20 00    	addd	#0x2000
    a8b3:	8f          	xgdx
    a8b4:	18 ce 50 00 	ldy	#0x5000
    a8b8:	c6 50       	ldab	#0x50
    a8ba:	20 0f       	bra	0x0xa8cb
    a8bc:	96 fa       	ldaa	*0xfa
    a8be:	c6 b0       	ldab	#0xb0
    a8c0:	3d          	mul
    a8c1:	c3 20 00    	addd	#0x2000
    a8c4:	8f          	xgdx
    a8c5:	18 ce 00 20 	ldy	#0x20
    a8c9:	c6 b0       	ldab	#0xb0
    a8cb:	18 a6 00    	ldaa	0x0,y
    a8ce:	84 7f       	anda	#0x7f
    a8d0:	a7 00       	staa	0x0,x
    a8d2:	08          	inx
    a8d3:	18 08       	iny
    a8d5:	5a          	decb
    a8d6:	26 f3       	bne	0x0xa8cb
    a8d8:	39          	rts
    a8d9:	ce 00 20    	ldx	#0x20
    a8dc:	18 ce 58 00 	ldy	#0x5800
    a8e0:	c6 b0       	ldab	#0xb0
    a8e2:	a6 00       	ldaa	0x0,x
    a8e4:	18 a7 00    	staa	0x0,y
    a8e7:	08          	inx
    a8e8:	18 08       	iny
    a8ea:	5a          	decb
    a8eb:	26 f5       	bne	0x0xa8e2
    a8ed:	bd a3 70    	jsr	0xa370
    a8f0:	7e a2 b6    	jmp	0xa2b6
    a8f3:	ce 00 20    	ldx	#0x20
    a8f6:	18 ce 58 00 	ldy	#0x5800
    a8fa:	c6 b0       	ldab	#0xb0
    a8fc:	18 a6 00    	ldaa	0x0,y
    a8ff:	a7 00       	staa	0x0,x
    a901:	08          	inx
    a902:	18 08       	iny
    a904:	5a          	decb
    a905:	26 f5       	bne	0x0xa8fc
    a907:	bd a3 bd    	jsr	0xa3bd
    a90a:	7e a2 b6    	jmp	0xa2b6
    a90d:	bd 91 eb    	jsr	0x91eb
    a910:	86 f0       	ldaa	#0xf0
    a912:	b7 10 2f    	staa	0x102f
    a915:	86 00       	ldaa	#0x0
    a917:	bd 91 eb    	jsr	0x91eb
    a91a:	b7 10 2f    	staa	0x102f
    a91d:	bd 91 eb    	jsr	0x91eb
    a920:	b7 10 2f    	staa	0x102f
    a923:	86 4d       	ldaa	#0x4d
    a925:	bd 91 eb    	jsr	0x91eb
    a928:	b7 10 2f    	staa	0x102f
    a92b:	86 08       	ldaa	#0x8
    a92d:	bd 91 eb    	jsr	0x91eb
    a930:	b7 10 2f    	staa	0x102f
    a933:	bd 91 eb    	jsr	0x91eb
    a936:	86 55       	ldaa	#0x55
    a938:	b7 10 2f    	staa	0x102f
    a93b:	bd 91 eb    	jsr	0x91eb
    a93e:	86 2a       	ldaa	#0x2a
    a940:	b7 10 2f    	staa	0x102f
    a943:	bd 91 eb    	jsr	0x91eb
    a946:	4f          	clra
    a947:	b7 10 2f    	staa	0x102f
    a94a:	bd 91 eb    	jsr	0x91eb
    a94d:	b7 10 2f    	staa	0x102f
    a950:	ce 80 00    	ldx	#0x8000
    a953:	18 ce 01 90 	ldy	#0x190
    a957:	c6 01       	ldab	#0x1
    a959:	d7 f8       	stab	*0xf8
    a95b:	a6 00       	ldaa	0x0,x
    a95d:	36          	psha
    a95e:	84 0f       	anda	#0xf
    a960:	bd 91 eb    	jsr	0x91eb
    a963:	bd 91 f3    	jsr	0x91f3
    a966:	b7 10 2f    	staa	0x102f
    a969:	32          	pula
    a96a:	84 f0       	anda	#0xf0
    a96c:	44          	lsra
    a96d:	bd 91 eb    	jsr	0x91eb
    a970:	bd 91 f3    	jsr	0x91f3
    a973:	b7 10 2f    	staa	0x102f
    a976:	18 09       	dey
    a978:	26 09       	bne	0x0xa983
    a97a:	18 ce 01 90 	ldy	#0x190
    a97e:	58          	aslb
    a97f:	24 02       	bcc	0x0xa983
    a981:	c6 01       	ldab	#0x1
    a983:	08          	inx
    a984:	26 d3       	bne	0x0xa959
    a986:	86 f7       	ldaa	#0xf7
    a988:	bd 91 eb    	jsr	0x91eb
    a98b:	bd 91 f3    	jsr	0x91f3
    a98e:	b7 10 2f    	staa	0x102f
    a991:	7f 00 f8    	clr	0xf8
    a994:	7f 00 ff    	clr	0xff
    a997:	7f 01 1b    	clr	0x11b
    a99a:	bd a3 59    	jsr	0xa359
    a99d:	96 f2       	ldaa	*0xf2
    a99f:	84 20       	anda	#0x20
    a9a1:	97 f2       	staa	*0xf2
    a9a3:	86 80       	ldaa	#0x80
    a9a5:	b7 01 1c    	staa	0x11c
    a9a8:	7e a2 b6    	jmp	0xa2b6
    a9ab:	bd ab e5    	jsr	0xabe5
    a9ae:	bd ab e5    	jsr	0xabe5
    a9b1:	b6 10 08    	ldaa	0x1008
    a9b4:	84 df       	anda	#0xdf
    a9b6:	b7 10 08    	staa	0x1008
    a9b9:	b6 10 00    	ldaa	0x1000
    a9bc:	8a 10       	oraa	#0x10
    a9be:	b7 10 00    	staa	0x1000
    a9c1:	ce 40 00    	ldx	#0x4000
    a9c4:	bd ab e5    	jsr	0xabe5
    a9c7:	16          	tab
    a9c8:	bd ab e5    	jsr	0xabe5
    a9cb:	48          	asla
    a9cc:	1b          	aba
    a9cd:	a7 00       	staa	0x0,x
    a9cf:	08          	inx
    a9d0:	8c 80 00    	cpx	#0x8000
    a9d3:	25 ef       	bcs	0x0xa9c4
    a9d5:	b6 10 08    	ldaa	0x1008
    a9d8:	85 20       	bita	#0x20
    a9da:	26 0a       	bne	0x0xa9e6
    a9dc:	8a 20       	oraa	#0x20
    a9de:	b7 10 08    	staa	0x1008
    a9e1:	ce 40 00    	ldx	#0x4000
    a9e4:	20 de       	bra	0x0xa9c4
    a9e6:	86 0c       	ldaa	#0xc
    a9e8:	b7 10 2d    	staa	0x102d
    a9eb:	4f          	clra
    a9ec:	97 f2       	staa	*0xf2
    a9ee:	97 f3       	staa	*0xf3
    a9f0:	97 f4       	staa	*0xf4
    a9f2:	97 f5       	staa	*0xf5
    a9f4:	97 f6       	staa	*0xf6
    a9f6:	97 f7       	staa	*0xf7
    a9f8:	97 f8       	staa	*0xf8
    a9fa:	bd aa c2    	jsr	0xaac2
    a9fd:	86 40       	ldaa	#0x40
    a9ff:	97 f2       	staa	*0xf2
    aa01:	86 20       	ldaa	#0x20
    aa03:	b7 10 44    	staa	0x1044
    aa06:	01          	nop
    aa07:	01          	nop
    aa08:	b6 10 45    	ldaa	0x1045
    aa0b:	b1 01 0f    	cmpa	0x10f
    aa0e:	27 f8       	beq	0x0xaa08
    aa10:	16          	tab
    aa11:	b8 01 0f    	eora	0x10f
    aa14:	f7 01 0f    	stab	0x10f
    aa17:	b4 01 0f    	anda	0x10f
    aa1a:	84 c0       	anda	#0xc0
    aa1c:	27 ea       	beq	0x0xaa08
    aa1e:	2a 03       	bpl	0x0xaa23
    aa20:	7e aa a3    	jmp	0xaaa3
    aa23:	7f 00 f2    	clr	0xf2
    aa26:	bd ab 39    	jsr	0xab39
    aa29:	b6 10 08    	ldaa	0x1008
    aa2c:	84 df       	anda	#0xdf
    aa2e:	b7 10 08    	staa	0x1008
    aa31:	4f          	clra
    aa32:	b7 10 22    	staa	0x1022
    aa35:	18 ce 00 00 	ldy	#0x0
    aa39:	ce aa 4d    	ldx	#0xaa4d
    aa3c:	a6 00       	ldaa	0x0,x
    aa3e:	18 a7 00    	staa	0x0,y
    aa41:	18 08       	iny
    aa43:	08          	inx
    aa44:	8c aa a2    	cpx	#0xaaa2
    aa47:	25 f3       	bcs	0x0xaa3c
    aa49:	0f          	sei
    aa4a:	7e 00 00    	jmp	0x0
    aa4d:	ce 40 00    	ldx	#0x4000
    aa50:	18 ce 80 00 	ldy	#0x8000
    aa54:	e6 00       	ldab	0x0,x
    aa56:	86 aa       	ldaa	#0xaa
    aa58:	b7 d5 55    	staa	0xd555
    aa5b:	86 55       	ldaa	#0x55
    aa5d:	b7 aa aa    	staa	0xaaaa
    aa60:	86 a0       	ldaa	#0xa0
    aa62:	b7 d5 55    	staa	0xd555
    aa65:	86 80       	ldaa	#0x80
    aa67:	18 e7 00    	stab	0x0,y
    aa6a:	08          	inx
    aa6b:	18 08       	iny
    aa6d:	e6 00       	ldab	0x0,x
    aa6f:	4a          	deca
    aa70:	26 f5       	bne	0x0xaa67
    aa72:	86 08       	ldaa	#0x8
    aa74:	b7 10 23    	staa	0x1023
    aa77:	fc 10 0e    	ldd	0x100e
    aa7a:	c3 5d c0    	addd	#0x5dc0
    aa7d:	fd 10 1e    	std	0x101e
    aa80:	01          	nop
    aa81:	01          	nop
    aa82:	b6 10 23    	ldaa	0x1023
    aa85:	85 08       	bita	#0x8
    aa87:	27 f7       	beq	0x0xaa80
    aa89:	8c 80 00    	cpx	#0x8000
    aa8c:	25 c6       	bcs	0x0xaa54
    aa8e:	b6 10 08    	ldaa	0x1008
    aa91:	85 20       	bita	#0x20
    aa93:	27 03       	beq	0x0xaa98
    aa95:	7e 80 00    	jmp	0x8000
    aa98:	8a 20       	oraa	#0x20
    aa9a:	b7 10 08    	staa	0x1008
    aa9d:	ce 40 00    	ldx	#0x4000
    aaa0:	20 b2       	bra	0x0xaa54
    aaa2:	01          	nop
    aaa3:	7f 00 f2    	clr	0xf2
    aaa6:	bd ab 8f    	jsr	0xab8f
    aaa9:	b6 10 45    	ldaa	0x1045
    aaac:	b1 01 0f    	cmpa	0x10f
    aaaf:	27 f8       	beq	0x0xaaa9
    aab1:	16          	tab
    aab2:	b8 01 0f    	eora	0x10f
    aab5:	f7 01 0f    	stab	0x10f
    aab8:	b4 01 0f    	anda	0x10f
    aabb:	84 80       	anda	#0x80
    aabd:	27 ea       	beq	0x0xaaa9
    aabf:	7e 80 00    	jmp	0x8000
    aac2:	86 f7       	ldaa	#0xf7
    aac4:	b4 10 00    	anda	0x1000
    aac7:	b7 10 00    	staa	0x1000
    aaca:	86 0c       	ldaa	#0xc
    aacc:	b7 10 47    	staa	0x1047
    aacf:	86 80       	ldaa	#0x80
    aad1:	ba 10 00    	oraa	0x1000
    aad4:	b7 10 00    	staa	0x1000
    aad7:	01          	nop
    aad8:	88 80       	eora	#0x80
    aada:	b7 10 00    	staa	0x1000
    aadd:	7f 01 1d    	clr	0x11d
    aae0:	bd ea d5    	jsr	0xead5
    aae3:	ce 10 23    	ldx	#0x1023
    aae6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xaae6
    aaea:	bd eb 09    	jsr	0xeb09
    aaed:	ce 01 20    	ldx	#0x120
    aaf0:	18 ce ab 19 	ldy	#0xab19
    aaf4:	c6 20       	ldab	#0x20
    aaf6:	18 a6 00    	ldaa	0x0,y
    aaf9:	a7 00       	staa	0x0,x
    aafb:	08          	inx
    aafc:	18 08       	iny
    aafe:	5a          	decb
    aaff:	26 f5       	bne	0x0xaaf6
    ab01:	7f 01 1e    	clr	0x11e
    ab04:	86 20       	ldaa	#0x20
    ab06:	b7 01 1c    	staa	0x11c
    ab09:	ce 10 23    	ldx	#0x1023
    ab0c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xab0c
    ab10:	bd ea 5d    	jsr	0xea5d
    ab13:	7d 01 1c    	tst	0x11c
    ab16:	26 f1       	bne	0x0xab09
    ab18:	39          	rts
    ab19:	2a 53       	bpl	0x0xab6e
    ab1b:	41          	.byte	0x41
    ab1c:	56          	rorb
    ab1d:	45          	.byte	0x45
    ab1e:	2a 20       	bpl	0x0xab40
    ab20:	54          	lsrb
    ab21:	4f          	clra
    ab22:	20 55       	bra	0x0xab79
    ab24:	50          	negb
    ab25:	44          	lsra
    ab26:	41          	.byte	0x41
    ab27:	54          	lsrb
    ab28:	45          	.byte	0x45
    ab29:	4f          	clra
    ab2a:	53          	comb
    ab2b:	2c 2a       	bge	0x0xab57
    ab2d:	45          	.byte	0x45
    ab2e:	58          	aslb
    ab2f:	49          	rola
    ab30:	54          	lsrb
    ab31:	2a 20       	bpl	0x0xab53
    ab33:	41          	.byte	0x41
    ab34:	42          	.byte	0x42
    ab35:	4f          	clra
    ab36:	52          	.byte	0x52
    ab37:	54          	lsrb
    ab38:	53          	comb
    ab39:	ce 10 23    	ldx	#0x1023
    ab3c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xab3c
    ab40:	bd eb 09    	jsr	0xeb09
    ab43:	ce 01 20    	ldx	#0x120
    ab46:	18 ce ab 6f 	ldy	#0xab6f
    ab4a:	c6 20       	ldab	#0x20
    ab4c:	18 a6 00    	ldaa	0x0,y
    ab4f:	a7 00       	staa	0x0,x
    ab51:	08          	inx
    ab52:	18 08       	iny
    ab54:	5a          	decb
    ab55:	26 f5       	bne	0x0xab4c
    ab57:	7f 01 1e    	clr	0x11e
    ab5a:	86 20       	ldaa	#0x20
    ab5c:	b7 01 1c    	staa	0x11c
    ab5f:	ce 10 23    	ldx	#0x1023
    ab62:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xab62
    ab66:	bd ea 5d    	jsr	0xea5d
    ab69:	7d 01 1c    	tst	0x11c
    ab6c:	26 f1       	bne	0x0xab5f
    ab6e:	39          	rts
    ab6f:	20 55       	bra	0x0xabc6
    ab71:	50          	negb
    ab72:	44          	lsra
    ab73:	41          	.byte	0x41
    ab74:	54          	lsrb
    ab75:	49          	rola
    ab76:	4e          	.byte	0x4e
    ab77:	47          	asra
    ab78:	20 4f       	bra	0x0xabc9
    ab7a:	53          	comb
    ab7b:	20 2e       	bra	0x0xabab
    ab7d:	2e 2e       	bgt	0x0xabad
    ab7f:	20 54       	bra	0x0xabd5
    ab81:	41          	.byte	0x41
    ab82:	4b          	.byte	0x4b
    ab83:	45          	.byte	0x45
    ab84:	20 35       	bra	0x0xabbb
    ab86:	20 2e       	bra	0x0xabb6
    ab88:	2e 2e       	bgt	0x0xabb8
    ab8a:	2e 2e       	bgt	0x0xabba
    ab8c:	2e 2e       	bgt	0x0xabbc
    ab8e:	2e ce       	bgt	0x0xab5e
    ab90:	10          	sba
    ab91:	23 1f       	bls	0x0xabb2
    ab93:	00          	bgnd
    ab94:	10          	sba
    ab95:	fc bd eb    	ldd	0xbdeb
    ab98:	09          	dex
    ab99:	ce 01 20    	ldx	#0x120
    ab9c:	18 ce ab c5 	ldy	#0xabc5
    aba0:	c6 20       	ldab	#0x20
    aba2:	18 a6 00    	ldaa	0x0,y
    aba5:	a7 00       	staa	0x0,x
    aba7:	08          	inx
    aba8:	18 08       	iny
    abaa:	5a          	decb
    abab:	26 f5       	bne	0x0xaba2
    abad:	7f 01 1e    	clr	0x11e
    abb0:	86 20       	ldaa	#0x20
    abb2:	b7 01 1c    	staa	0x11c
    abb5:	ce 10 23    	ldx	#0x1023
    abb8:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xabb8
    abbc:	bd ea 5d    	jsr	0xea5d
    abbf:	7d 01 1c    	tst	0x11c
    abc2:	26 f1       	bne	0x0xabb5
    abc4:	39          	rts
    abc5:	4f          	clra
    abc6:	53          	comb
    abc7:	20 55       	bra	0x0xac1e
    abc9:	50          	negb
    abca:	44          	lsra
    abcb:	41          	.byte	0x41
    abcc:	54          	lsrb
    abcd:	45          	.byte	0x45
    abce:	20 41       	bra	0x0xac11
    abd0:	42          	.byte	0x42
    abd1:	4f          	clra
    abd2:	52          	.byte	0x52
    abd3:	54          	lsrb
    abd4:	44          	lsra
    abd5:	52          	.byte	0x52
    abd6:	45          	.byte	0x45
    abd7:	4c          	inca
    abd8:	4f          	clra
    abd9:	41          	.byte	0x41
    abda:	44          	lsra
    abdb:	20 53       	bra	0x0xac30
    abdd:	45          	.byte	0x45
    abde:	51          	.byte	0x51
    abdf:	26 4d       	bne	0x0xac2e
    abe1:	55          	.byte	0x55
    abe2:	4c          	inca
    abe3:	54          	lsrb
    abe4:	49          	rola
    abe5:	3c          	pshx
    abe6:	fe 01 c0    	ldx	0x1c0
    abe9:	bc 01 c2    	cpx	0x1c2
    abec:	27 f8       	beq	0x0xabe6
    abee:	a6 00       	ldaa	0x0,x
    abf0:	8f          	xgdx
    abf1:	5c          	incb
    abf2:	c4 bf       	andb	#0xbf
    abf4:	8f          	xgdx
    abf5:	ff 01 c0    	stx	0x1c0
    abf8:	38          	pulx
    abf9:	39          	rts
    abfa:	0f          	sei
    abfb:	4f          	clra
    abfc:	b7 10 40    	staa	0x1040
    abff:	b6 01 0c    	ldaa	0x10c
    ac02:	49          	rola
    ac03:	2a 02       	bpl	0x0xac07
    ac05:	86 01       	ldaa	#0x1
    ac07:	b7 10 41    	staa	0x1041
    ac0a:	b7 01 0c    	staa	0x10c
    ac0d:	fc 01 0d    	ldd	0x10d
    ac10:	5c          	incb
    ac11:	c1 f8       	cmpb	#0xf8
    ac13:	23 02       	bls	0x0xac17
    ac15:	c6 f2       	ldab	#0xf2
    ac17:	fd 01 0d    	std	0x10d
    ac1a:	8f          	xgdx
    ac1b:	a6 00       	ldaa	0x0,x
    ac1d:	7d 00 fb    	tst	0xfb
    ac20:	27 24       	beq	0x0xac46
    ac22:	8c 00 f2    	cpx	#0xf2
    ac25:	26 1f       	bne	0x0xac46
    ac27:	f6 01 66    	ldab	0x166
    ac2a:	5a          	decb
    ac2b:	c4 1f       	andb	#0x1f
    ac2d:	f7 01 66    	stab	0x166
    ac30:	26 08       	bne	0x0xac3a
    ac32:	c6 01       	ldab	#0x1
    ac34:	f8 01 67    	eorb	0x167
    ac37:	f7 01 67    	stab	0x167
    ac3a:	7d 00 fe    	tst	0xfe
    ac3d:	26 07       	bne	0x0xac46
    ac3f:	7d 01 67    	tst	0x167
    ac42:	26 02       	bne	0x0xac46
    ac44:	84 df       	anda	#0xdf
    ac46:	b7 10 40    	staa	0x1040
    ac49:	86 80       	ldaa	#0x80
    ac4b:	b7 10 23    	staa	0x1023
    ac4e:	fc 10 0e    	ldd	0x100e
    ac51:	c3 13 88    	addd	#0x1388
    ac54:	fd 10 16    	std	0x1016
    ac57:	0e          	cli
    ac58:	3b          	rti
    ac59:	ce 10 23    	ldx	#0x1023
    ac5c:	1e 00 20 03 	brset	0x0,x, #0x20, 0x0xac63
    ac60:	7e ad 8a    	jmp	0xad8a
    ac63:	96 ff       	ldaa	*0xff
    ac65:	81 01       	cmpa	#0x1
    ac67:	26 03       	bne	0x0xac6c
    ac69:	7e ad 8a    	jmp	0xad8a
    ac6c:	7d 00 fe    	tst	0xfe
    ac6f:	27 03       	beq	0x0xac74
    ac71:	7e ad 8a    	jmp	0xad8a
    ac74:	7d 01 17    	tst	0x117
    ac77:	27 0d       	beq	0x0xac86
    ac79:	4f          	clra
    ac7a:	b7 10 30    	staa	0x1030
    ac7d:	b7 01 17    	staa	0x117
    ac80:	ce 00 96    	ldx	#0x96
    ac83:	7e ad 7e    	jmp	0xad7e
    ac86:	ce 00 00    	ldx	#0x0
    ac89:	f6 10 31    	ldab	0x1031
    ac8c:	3a          	abx
    ac8d:	f6 10 32    	ldab	0x1032
    ac90:	3a          	abx
    ac91:	f6 10 33    	ldab	0x1033
    ac94:	3a          	abx
    ac95:	f6 10 34    	ldab	0x1034
    ac98:	3a          	abx
    ac99:	8f          	xgdx
    ac9a:	04          	lsrd
    ac9b:	04          	lsrd
    ac9c:	54          	lsrb
    ac9d:	ce 1f 00    	ldx	#0x1f00
    aca0:	37          	pshb
    aca1:	f6 01 16    	ldab	0x116
    aca4:	3a          	abx
    aca5:	bd ad 9e    	jsr	0xad9e
    aca8:	33          	pulb
    aca9:	a6 00       	ldaa	0x0,x
    acab:	10          	sba
    acac:	26 40       	bne	0x0xacee
    acae:	bd ad c4    	jsr	0xadc4
    acb1:	86 01       	ldaa	#0x1
    acb3:	b7 01 17    	staa	0x117
    acb6:	b6 01 16    	ldaa	0x116
    acb9:	4c          	inca
    acba:	81 20       	cmpa	#0x20
    acbc:	25 01       	bcs	0x0xacbf
    acbe:	4f          	clra
    acbf:	b7 01 16    	staa	0x116
    acc2:	81 07       	cmpa	#0x7
    acc4:	22 07       	bhi	0x0xaccd
    acc6:	8a 70       	oraa	#0x70
    acc8:	b7 10 43    	staa	0x1043
    accb:	20 1b       	bra	0x0xace8
    accd:	81 0f       	cmpa	#0xf
    accf:	22 07       	bhi	0x0xacd8
    acd1:	8a 68       	oraa	#0x68
    acd3:	b7 10 43    	staa	0x1043
    acd6:	20 10       	bra	0x0xace8
    acd8:	81 17       	cmpa	#0x17
    acda:	22 07       	bhi	0x0xace3
    acdc:	8a 58       	oraa	#0x58
    acde:	b7 10 43    	staa	0x1043
    ace1:	20 05       	bra	0x0xace8
    ace3:	8a 38       	oraa	#0x38
    ace5:	b7 10 43    	staa	0x1043
    ace8:	ce 03 e8    	ldx	#0x3e8
    aceb:	7e ad 7e    	jmp	0xad7e
    acee:	2a 04       	bpl	0x0xacf4
    acf0:	40          	nega
    acf1:	7e ad 24    	jmp	0xad24
    acf4:	81 02       	cmpa	#0x2
    acf6:	23 10       	bls	0x0xad08
    acf8:	e7 00       	stab	0x0,x
    acfa:	86 80       	ldaa	#0x80
    acfc:	a7 20       	staa	0x20,x
    acfe:	bd ad c4    	jsr	0xadc4
    ad01:	17          	tba
    ad02:	f6 01 16    	ldab	0x116
    ad05:	7e ad 51    	jmp	0xad51
    ad08:	6d 20       	tst	0x20,x
    ad0a:	2b 03       	bmi	0x0xad0f
    ad0c:	7e ac ae    	jmp	0xacae
    ad0f:	1f 20 01 05 	brclr	0x20,x, #0x01, 0x0xad18
    ad13:	6f 20       	clr	0x20,x
    ad15:	7e ac ae    	jmp	0xacae
    ad18:	e7 00       	stab	0x0,x
    ad1a:	bd ad c4    	jsr	0xadc4
    ad1d:	17          	tba
    ad1e:	f6 01 16    	ldab	0x116
    ad21:	7e ad 51    	jmp	0xad51
    ad24:	81 02       	cmpa	#0x2
    ad26:	23 10       	bls	0x0xad38
    ad28:	e7 00       	stab	0x0,x
    ad2a:	86 81       	ldaa	#0x81
    ad2c:	a7 20       	staa	0x20,x
    ad2e:	bd ad c4    	jsr	0xadc4
    ad31:	17          	tba
    ad32:	f6 01 16    	ldab	0x116
    ad35:	7e ad 51    	jmp	0xad51
    ad38:	6d 20       	tst	0x20,x
    ad3a:	2b 03       	bmi	0x0xad3f
    ad3c:	7e ac ae    	jmp	0xacae
    ad3f:	1e 20 01 05 	brset	0x20,x, #0x01, 0x0xad48
    ad43:	6f 20       	clr	0x20,x
    ad45:	7e ac ae    	jmp	0xacae
    ad48:	e7 00       	stab	0x0,x
    ad4a:	bd ad c4    	jsr	0xadc4
    ad4d:	17          	tba
    ad4e:	f6 01 16    	ldab	0x116
    ad51:	f7 01 15    	stab	0x115
    ad54:	c1 07       	cmpb	#0x7
    ad56:	27 1a       	beq	0x0xad72
    ad58:	f6 01 70    	ldab	0x170
    ad5b:	c1 02       	cmpb	#0x2
    ad5d:	26 03       	bne	0x0xad62
    ad5f:	7e ac e8    	jmp	0xace8
    ad62:	7d 00 fb    	tst	0xfb
    ad65:	26 0b       	bne	0x0xad72
    ad67:	14 fb 01    	bset	*0xfb, #0x01
    ad6a:	14 f2 20    	bset	*0xf2, #0x20
    ad6d:	c6 1f       	ldab	#0x1f
    ad6f:	f7 01 66    	stab	0x166
    ad72:	f6 01 15    	ldab	0x115
    ad75:	58          	aslb
    ad76:	ce ae 21    	ldx	#0xae21
    ad79:	3a          	abx
    ad7a:	ee 00       	ldx	0x0,x
    ad7c:	6e 00       	jmp	0x0,x
    ad7e:	86 20       	ldaa	#0x20
    ad80:	b7 10 23    	staa	0x1023
    ad83:	8f          	xgdx
    ad84:	f3 10 0e    	addd	0x100e
    ad87:	fd 10 1a    	std	0x101a
    ad8a:	fe 01 c0    	ldx	0x1c0
    ad8d:	bc 01 c2    	cpx	0x1c2
    ad90:	26 03       	bne	0x0xad95
    ad92:	7e b2 2e    	jmp	0xb22e
    ad95:	18 fe 01 6c 	ldy	0x16c
    ad99:	18 3c       	pshy
    ad9b:	7e 92 be    	jmp	0x92be
    ad9e:	36          	psha
    ad9f:	b6 10 00    	ldaa	0x1000
    ada2:	84 ef       	anda	#0xef
    ada4:	b7 10 00    	staa	0x1000
    ada7:	b6 10 08    	ldaa	0x1008
    adaa:	84 1f       	anda	#0x1f
    adac:	b7 10 08    	staa	0x1008
    adaf:	32          	pula
    adb0:	39          	rts
    adb1:	36          	psha
    adb2:	86 10       	ldaa	#0x10
    adb4:	ba 10 00    	oraa	0x1000
    adb7:	b7 10 00    	staa	0x1000
    adba:	86 1f       	ldaa	#0x1f
    adbc:	b4 10 08    	anda	0x1008
    adbf:	b7 10 08    	staa	0x1008
    adc2:	32          	pula
    adc3:	39          	rts
    adc4:	36          	psha
    adc5:	96 f9       	ldaa	*0xf9
    adc7:	81 01       	cmpa	#0x1
    adc9:	22 02       	bhi	0x0xadcd
    adcb:	32          	pula
    adcc:	39          	rts
    adcd:	80 02       	suba	#0x2
    adcf:	26 12       	bne	0x0xade3
    add1:	b6 10 00    	ldaa	0x1000
    add4:	84 ef       	anda	#0xef
    add6:	b7 10 00    	staa	0x1000
    add9:	b6 10 08    	ldaa	0x1008
    addc:	84 1f       	anda	#0x1f
    adde:	b7 10 08    	staa	0x1008
    ade1:	32          	pula
    ade2:	39          	rts
    ade3:	81 01       	cmpa	#0x1
    ade5:	22 12       	bhi	0x0xadf9
    ade7:	b6 10 00    	ldaa	0x1000
    adea:	84 ef       	anda	#0xef
    adec:	b7 10 00    	staa	0x1000
    adef:	b6 10 08    	ldaa	0x1008
    adf2:	8a 20       	oraa	#0x20
    adf4:	b7 10 08    	staa	0x1008
    adf7:	32          	pula
    adf8:	39          	rts
    adf9:	81 02       	cmpa	#0x2
    adfb:	22 12       	bhi	0x0xae0f
    adfd:	b6 10 00    	ldaa	0x1000
    ae00:	8a 10       	oraa	#0x10
    ae02:	b7 10 00    	staa	0x1000
    ae05:	b6 10 08    	ldaa	0x1008
    ae08:	84 1f       	anda	#0x1f
    ae0a:	b7 10 08    	staa	0x1008
    ae0d:	32          	pula
    ae0e:	39          	rts
    ae0f:	b6 10 00    	ldaa	0x1000
    ae12:	8a 10       	oraa	#0x10
    ae14:	b7 10 00    	staa	0x1000
    ae17:	b6 10 08    	ldaa	0x1008
    ae1a:	8a 20       	oraa	#0x20
    ae1c:	b7 10 08    	staa	0x1008
    ae1f:	32          	pula
    ae20:	39          	rts
    ae21:	ae 61       	lds	0x61,x
    ae23:	ae 74       	lds	0x74,x
    ae25:	ae ac       	lds	0xac,x
    ae27:	ae e4       	lds	0xe4,x
    ae29:	ae fa       	lds	0xfa,x
    ae2b:	af 09       	sts	0x9,x
    ae2d:	af 18       	sts	0x18,x
    ae2f:	af 27       	sts	0x27,x
    ae31:	af 38       	sts	0x38,x
    ae33:	af 47       	sts	0x47,x
    ae35:	af 75       	sts	0x75,x
    ae37:	af a3       	sts	0xa3,x
    ae39:	af b2       	sts	0xb2,x
    ae3b:	af c1       	sts	0xc1,x
    ae3d:	af d0       	sts	0xd0,x
    ae3f:	af df       	sts	0xdf,x
    ae41:	af ee       	sts	0xee,x
    ae43:	af fd       	sts	0xfd,x
    ae45:	b0 0c b0    	suba	0xcb0
    ae48:	39          	rts
    ae49:	b0 48 b0    	suba	0x48b0
    ae4c:	57          	asrb
    ae4d:	b0 66 b0    	suba	0x66b0
    ae50:	75          	.byte	0x75
    ae51:	b0 84 b0    	suba	0x84b0
    ae54:	93 b0       	subd	*0xb0
    ae56:	a2 b0       	sbca	0xb0,x
    ae58:	b1 b0 c0    	cmpa	0xb0c0
    ae5b:	b0 cf b0    	suba	0xcfb0
    ae5e:	de b0       	ldx	*0xb0
    ae60:	ed 16       	std	0x16,x
    ae62:	ce ef 8e    	ldx	#0xef8e
    ae65:	3a          	abx
    ae66:	a6 00       	ldaa	0x0,x
    ae68:	97 71       	staa	*0x71
    ae6a:	97 db       	staa	*0xdb
    ae6c:	c6 74       	ldab	#0x74
    ae6e:	bd b1 9a    	jsr	0xb19a
    ae71:	7e ac ae    	jmp	0xacae
    ae74:	16          	tab
    ae75:	ce ef 8e    	ldx	#0xef8e
    ae78:	3a          	abx
    ae79:	a6 00       	ldaa	0x0,x
    ae7b:	97 2a       	staa	*0x2a
    ae7d:	c6 2a       	ldab	#0x2a
    ae7f:	bd b0 fc    	jsr	0xb0fc
    ae82:	b6 01 1b    	ldaa	0x11b
    ae85:	81 19       	cmpa	#0x19
    ae87:	26 19       	bne	0x0xaea2
    ae89:	13 f4 01 15 	brclr	*0xf4, #0x01, 0x0xaea2
    ae8d:	96 2a       	ldaa	*0x2a
    ae8f:	ce 01 31    	ldx	#0x131
    ae92:	bd eb bc    	jsr	0xebbc
    ae95:	86 03       	ldaa	#0x3
    ae97:	b7 01 1c    	staa	0x11c
    ae9a:	86 13       	ldaa	#0x13
    ae9c:	b7 01 1e    	staa	0x11e
    ae9f:	bd ea e4    	jsr	0xeae4
    aea2:	96 2a       	ldaa	*0x2a
    aea4:	c6 48       	ldab	#0x48
    aea6:	bd b1 9a    	jsr	0xb19a
    aea9:	7e ac ae    	jmp	0xacae
    aeac:	16          	tab
    aead:	ce ef 8e    	ldx	#0xef8e
    aeb0:	3a          	abx
    aeb1:	a6 00       	ldaa	0x0,x
    aeb3:	97 37       	staa	*0x37
    aeb5:	c6 37       	ldab	#0x37
    aeb7:	bd b0 fc    	jsr	0xb0fc
    aeba:	b6 01 1b    	ldaa	0x11b
    aebd:	81 19       	cmpa	#0x19
    aebf:	26 19       	bne	0x0xaeda
    aec1:	13 f4 02 15 	brclr	*0xf4, #0x02, 0x0xaeda
    aec5:	96 37       	ldaa	*0x37
    aec7:	ce 01 31    	ldx	#0x131
    aeca:	bd eb bc    	jsr	0xebbc
    aecd:	86 03       	ldaa	#0x3
    aecf:	b7 01 1c    	staa	0x11c
    aed2:	86 13       	ldaa	#0x13
    aed4:	b7 01 1e    	staa	0x11e
    aed7:	bd ea e4    	jsr	0xeae4
    aeda:	96 37       	ldaa	*0x37
    aedc:	c6 4a       	ldab	#0x4a
    aede:	bd b1 9a    	jsr	0xb19a
    aee1:	7e ac ae    	jmp	0xacae
    aee4:	16          	tab
    aee5:	ce ef 8e    	ldx	#0xef8e
    aee8:	3a          	abx
    aee9:	a6 00       	ldaa	0x0,x
    aeeb:	97 49       	staa	*0x49
    aeed:	c6 49       	ldab	#0x49
    aeef:	bd b0 fc    	jsr	0xb0fc
    aef2:	c6 57       	ldab	#0x57
    aef4:	bd b1 9a    	jsr	0xb19a
    aef7:	7e ac ae    	jmp	0xacae
    aefa:	97 35       	staa	*0x35
    aefc:	c6 35       	ldab	#0x35
    aefe:	bd b0 fc    	jsr	0xb0fc
    af01:	c6 49       	ldab	#0x49
    af03:	bd b1 9a    	jsr	0xb19a
    af06:	7e ac ae    	jmp	0xacae
    af09:	97 20       	staa	*0x20
    af0b:	c6 20       	ldab	#0x20
    af0d:	bd b0 fc    	jsr	0xb0fc
    af10:	c6 05       	ldab	#0x5
    af12:	bd b1 9a    	jsr	0xb19a
    af15:	7e ac ae    	jmp	0xacae
    af18:	97 28       	staa	*0x28
    af1a:	c6 28       	ldab	#0x28
    af1c:	bd b0 fc    	jsr	0xb0fc
    af1f:	c6 47       	ldab	#0x47
    af21:	bd b1 9a    	jsr	0xb19a
    af24:	7e ac ae    	jmp	0xacae
    af27:	97 d5       	staa	*0xd5
    af29:	97 d6       	staa	*0xd6
    af2b:	c6 fc       	ldab	#0xfc
    af2d:	bd b0 fc    	jsr	0xb0fc
    af30:	c6 07       	ldab	#0x7
    af32:	bd b1 9a    	jsr	0xb19a
    af35:	7e ac ae    	jmp	0xacae
    af38:	97 42       	staa	*0x42
    af3a:	c6 42       	ldab	#0x42
    af3c:	bd b0 fc    	jsr	0xb0fc
    af3f:	c6 3b       	ldab	#0x3b
    af41:	bd b1 9a    	jsr	0xb19a
    af44:	7e ac ae    	jmp	0xacae
    af47:	44          	lsra
    af48:	97 3f       	staa	*0x3f
    af4a:	c6 3f       	ldab	#0x3f
    af4c:	bd b0 fc    	jsr	0xb0fc
    af4f:	36          	psha
    af50:	b6 01 1b    	ldaa	0x11b
    af53:	81 06       	cmpa	#0x6
    af55:	26 15       	bne	0x0xaf6c
    af57:	96 3f       	ldaa	*0x3f
    af59:	ce 01 3c    	ldx	#0x13c
    af5c:	bd eb bc    	jsr	0xebbc
    af5f:	86 03       	ldaa	#0x3
    af61:	b7 01 1c    	staa	0x11c
    af64:	86 1e       	ldaa	#0x1e
    af66:	b7 01 1e    	staa	0x11e
    af69:	bd ea e4    	jsr	0xeae4
    af6c:	32          	pula
    af6d:	c6 3c       	ldab	#0x3c
    af6f:	bd b1 9a    	jsr	0xb19a
    af72:	7e ac ae    	jmp	0xacae
    af75:	44          	lsra
    af76:	97 3e       	staa	*0x3e
    af78:	c6 3e       	ldab	#0x3e
    af7a:	bd b0 fc    	jsr	0xb0fc
    af7d:	36          	psha
    af7e:	b6 01 1b    	ldaa	0x11b
    af81:	81 06       	cmpa	#0x6
    af83:	26 15       	bne	0x0xaf9a
    af85:	96 3e       	ldaa	*0x3e
    af87:	ce 01 37    	ldx	#0x137
    af8a:	bd eb bc    	jsr	0xebbc
    af8d:	86 03       	ldaa	#0x3
    af8f:	b7 01 1c    	staa	0x11c
    af92:	86 19       	ldaa	#0x19
    af94:	b7 01 1e    	staa	0x11e
    af97:	bd ea e4    	jsr	0xeae4
    af9a:	32          	pula
    af9b:	c6 36       	ldab	#0x36
    af9d:	bd b1 9a    	jsr	0xb19a
    afa0:	7e ac ae    	jmp	0xacae
    afa3:	97 43       	staa	*0x43
    afa5:	c6 43       	ldab	#0x43
    afa7:	bd b0 fc    	jsr	0xb0fc
    afaa:	c6 46       	ldab	#0x46
    afac:	bd b1 9a    	jsr	0xb19a
    afaf:	7e ac ae    	jmp	0xacae
    afb2:	97 45       	staa	*0x45
    afb4:	c6 45       	ldab	#0x45
    afb6:	bd b0 fc    	jsr	0xb0fc
    afb9:	c6 54       	ldab	#0x54
    afbb:	bd b1 9a    	jsr	0xb19a
    afbe:	7e ac ae    	jmp	0xacae
    afc1:	97 4c       	staa	*0x4c
    afc3:	c6 4c       	ldab	#0x4c
    afc5:	bd b0 fc    	jsr	0xb0fc
    afc8:	c6 58       	ldab	#0x58
    afca:	bd b1 9a    	jsr	0xb19a
    afcd:	7e ac ae    	jmp	0xacae
    afd0:	97 46       	staa	*0x46
    afd2:	c6 46       	ldab	#0x46
    afd4:	bd b0 fc    	jsr	0xb0fc
    afd7:	c6 55       	ldab	#0x55
    afd9:	bd b1 9a    	jsr	0xb19a
    afdc:	7e ac ae    	jmp	0xacae
    afdf:	97 47       	staa	*0x47
    afe1:	c6 47       	ldab	#0x47
    afe3:	bd b0 fc    	jsr	0xb0fc
    afe6:	c6 56       	ldab	#0x56
    afe8:	bd b1 9a    	jsr	0xb19a
    afeb:	7e ac ae    	jmp	0xacae
    afee:	97 4f       	staa	*0x4f
    aff0:	c6 4f       	ldab	#0x4f
    aff2:	bd b0 fc    	jsr	0xb0fc
    aff5:	c6 66       	ldab	#0x66
    aff7:	bd b1 9a    	jsr	0xb19a
    affa:	7e ac ae    	jmp	0xacae
    affd:	97 5c       	staa	*0x5c
    afff:	c6 5c       	ldab	#0x5c
    b001:	bd b0 fc    	jsr	0xb0fc
    b004:	c6 71       	ldab	#0x71
    b006:	bd b1 9a    	jsr	0xb19a
    b009:	7e ac ae    	jmp	0xacae
    b00c:	97 63       	staa	*0x63
    b00e:	c6 63       	ldab	#0x63
    b010:	bd b0 fc    	jsr	0xb0fc
    b013:	36          	psha
    b014:	b6 01 1b    	ldaa	0x11b
    b017:	81 13       	cmpa	#0x13
    b019:	26 15       	bne	0x0xb030
    b01b:	96 63       	ldaa	*0x63
    b01d:	ce 01 31    	ldx	#0x131
    b020:	bd eb bc    	jsr	0xebbc
    b023:	86 03       	ldaa	#0x3
    b025:	b7 01 1c    	staa	0x11c
    b028:	86 13       	ldaa	#0x13
    b02a:	b7 01 1e    	staa	0x11e
    b02d:	bd ea e4    	jsr	0xeae4
    b030:	32          	pula
    b031:	c6 6f       	ldab	#0x6f
    b033:	bd b1 9a    	jsr	0xb19a
    b036:	7e ac ae    	jmp	0xacae
    b039:	97 5b       	staa	*0x5b
    b03b:	c6 5b       	ldab	#0x5b
    b03d:	bd b0 fc    	jsr	0xb0fc
    b040:	c6 70       	ldab	#0x70
    b042:	bd b1 9a    	jsr	0xb19a
    b045:	7e ac ae    	jmp	0xacae
    b048:	97 5f       	staa	*0x5f
    b04a:	c6 5f       	ldab	#0x5f
    b04c:	bd b0 fc    	jsr	0xb0fc
    b04f:	c6 73       	ldab	#0x73
    b051:	bd b1 9a    	jsr	0xb19a
    b054:	7e ac ae    	jmp	0xacae
    b057:	97 4e       	staa	*0x4e
    b059:	c6 4e       	ldab	#0x4e
    b05b:	bd b0 fc    	jsr	0xb0fc
    b05e:	c6 5a       	ldab	#0x5a
    b060:	bd b1 9a    	jsr	0xb19a
    b063:	7e ac ae    	jmp	0xacae
    b066:	97 5d       	staa	*0x5d
    b068:	c6 5d       	ldab	#0x5d
    b06a:	bd b0 fc    	jsr	0xb0fc
    b06d:	c6 72       	ldab	#0x72
    b06f:	bd b1 9a    	jsr	0xb19a
    b072:	7e ac ae    	jmp	0xacae
    b075:	97 4d       	staa	*0x4d
    b077:	c6 4d       	ldab	#0x4d
    b079:	bd b0 fc    	jsr	0xb0fc
    b07c:	c6 59       	ldab	#0x59
    b07e:	bd b1 9a    	jsr	0xb19a
    b081:	7e ac ae    	jmp	0xacae
    b084:	97 57       	staa	*0x57
    b086:	c6 57       	ldab	#0x57
    b088:	bd b0 fc    	jsr	0xb0fc
    b08b:	c6 6c       	ldab	#0x6c
    b08d:	bd b1 9a    	jsr	0xb19a
    b090:	7e ac ae    	jmp	0xacae
    b093:	97 58       	staa	*0x58
    b095:	c6 58       	ldab	#0x58
    b097:	bd b0 fc    	jsr	0xb0fc
    b09a:	c6 6d       	ldab	#0x6d
    b09c:	bd b1 9a    	jsr	0xb19a
    b09f:	7e ac ae    	jmp	0xacae
    b0a2:	97 5a       	staa	*0x5a
    b0a4:	c6 5a       	ldab	#0x5a
    b0a6:	bd b0 fc    	jsr	0xb0fc
    b0a9:	c6 6e       	ldab	#0x6e
    b0ab:	bd b1 9a    	jsr	0xb19a
    b0ae:	7e ac ae    	jmp	0xacae
    b0b1:	97 56       	staa	*0x56
    b0b3:	c6 56       	ldab	#0x56
    b0b5:	bd b0 fc    	jsr	0xb0fc
    b0b8:	c6 6b       	ldab	#0x6b
    b0ba:	bd b1 9a    	jsr	0xb19a
    b0bd:	7e ac ae    	jmp	0xacae
    b0c0:	97 55       	staa	*0x55
    b0c2:	c6 55       	ldab	#0x55
    b0c4:	bd b0 fc    	jsr	0xb0fc
    b0c7:	c6 6a       	ldab	#0x6a
    b0c9:	bd b1 9a    	jsr	0xb19a
    b0cc:	7e ac ae    	jmp	0xacae
    b0cf:	97 51       	staa	*0x51
    b0d1:	c6 51       	ldab	#0x51
    b0d3:	bd b0 fc    	jsr	0xb0fc
    b0d6:	c6 67       	ldab	#0x67
    b0d8:	bd b1 9a    	jsr	0xb19a
    b0db:	7e ac ae    	jmp	0xacae
    b0de:	97 53       	staa	*0x53
    b0e0:	c6 53       	ldab	#0x53
    b0e2:	bd b0 fc    	jsr	0xb0fc
    b0e5:	c6 69       	ldab	#0x69
    b0e7:	bd b1 9a    	jsr	0xb19a
    b0ea:	7e ac ae    	jmp	0xacae
    b0ed:	97 52       	staa	*0x52
    b0ef:	c6 52       	ldab	#0x52
    b0f1:	bd b0 fc    	jsr	0xb0fc
    b0f4:	c6 68       	ldab	#0x68
    b0f6:	bd b1 9a    	jsr	0xb19a
    b0f9:	7e ac ae    	jmp	0xacae
    b0fc:	37          	pshb
    b0fd:	f6 10 22    	ldab	0x1022
    b100:	c4 f7       	andb	#0xf7
    b102:	f7 10 22    	stab	0x1022
    b105:	7d 00 d0    	tst	0xd0
    b108:	27 05       	beq	0x0xb10f
    b10a:	7f 00 d0    	clr	0xd0
    b10d:	20 08       	bra	0x0xb117
    b10f:	7d 10 29    	tst	0x1029
    b112:	2a fb       	bpl	0x0xb10f
    b114:	f6 10 2a    	ldab	0x102a
    b117:	12 d1 80 18 	brset	*0xd1, #0x80, 0x0xb133
    b11b:	33          	pulb
    b11c:	37          	pshb
    b11d:	c1 fc       	cmpb	#0xfc
    b11f:	26 06       	bne	0x0xb127
    b121:	33          	pulb
    b122:	c6 ac       	ldab	#0xac
    b124:	37          	pshb
    b125:	20 0c       	bra	0x0xb133
    b127:	f6 01 6b    	ldab	0x16b
    b12a:	ce 50 50    	ldx	#0x5050
    b12d:	3a          	abx
    b12e:	e6 00       	ldab	0x0,x
    b130:	53          	comb
    b131:	20 01       	bra	0x0xb134
    b133:	5f          	clrb
    b134:	01          	nop
    b135:	01          	nop
    b136:	01          	nop
    b137:	01          	nop
    b138:	f7 10 42    	stab	0x1042
    b13b:	01          	nop
    b13c:	01          	nop
    b13d:	01          	nop
    b13e:	01          	nop
    b13f:	01          	nop
    b140:	01          	nop
    b141:	01          	nop
    b142:	c6 83       	ldab	#0x83
    b144:	f7 10 2a    	stab	0x102a
    b147:	7d 10 29    	tst	0x1029
    b14a:	2a fb       	bpl	0x0xb147
    b14c:	f6 10 2a    	ldab	0x102a
    b14f:	33          	pulb
    b150:	c1 fc       	cmpb	#0xfc
    b152:	26 02       	bne	0x0xb156
    b154:	c6 ac       	ldab	#0xac
    b156:	37          	pshb
    b157:	c0 20       	subb	#0x20
    b159:	f7 10 2a    	stab	0x102a
    b15c:	7d 10 29    	tst	0x1029
    b15f:	2a fb       	bpl	0x0xb15c
    b161:	f6 10 2a    	ldab	0x102a
    b164:	b7 10 2a    	staa	0x102a
    b167:	33          	pulb
    b168:	12 d1 80 21 	brset	*0xd1, #0x80, 0x0xb18d
    b16c:	c1 44       	cmpb	#0x44
    b16e:	26 1d       	bne	0x0xb18d
    b170:	ce 50 03    	ldx	#0x5003
    b173:	f6 01 6b    	ldab	0x16b
    b176:	86 04       	ldaa	#0x4
    b178:	3d          	mul
    b179:	3a          	abx
    b17a:	a6 00       	ldaa	0x0,x
    b17c:	84 40       	anda	#0x40
    b17e:	26 0d       	bne	0x0xb18d
    b180:	7d 10 29    	tst	0x1029
    b183:	2a fb       	bpl	0x0xb180
    b185:	b6 10 2a    	ldaa	0x102a
    b188:	86 8c       	ldaa	#0x8c
    b18a:	b7 10 2a    	staa	0x102a
    b18d:	13 f3 10 08 	brclr	*0xf3, #0x10, 0x0xb199
    b191:	f6 10 22    	ldab	0x1022
    b194:	ca 08       	orab	#0x8
    b196:	f7 10 22    	stab	0x1022
    b199:	39          	rts
    b19a:	0f          	sei
    b19b:	37          	pshb
    b19c:	f6 01 6f    	ldab	0x16f
    b19f:	ca b0       	orab	#0xb0
    b1a1:	fe 01 62    	ldx	0x162
    b1a4:	e7 00       	stab	0x0,x
    b1a6:	8f          	xgdx
    b1a7:	5c          	incb
    b1a8:	c1 60       	cmpb	#0x60
    b1aa:	25 02       	bcs	0x0xb1ae
    b1ac:	c6 40       	ldab	#0x40
    b1ae:	8f          	xgdx
    b1af:	33          	pulb
    b1b0:	e7 00       	stab	0x0,x
    b1b2:	8f          	xgdx
    b1b3:	5c          	incb
    b1b4:	c1 60       	cmpb	#0x60
    b1b6:	25 02       	bcs	0x0xb1ba
    b1b8:	c6 40       	ldab	#0x40
    b1ba:	8f          	xgdx
    b1bb:	a7 00       	staa	0x0,x
    b1bd:	8f          	xgdx
    b1be:	5c          	incb
    b1bf:	c1 60       	cmpb	#0x60
    b1c1:	25 02       	bcs	0x0xb1c5
    b1c3:	c6 40       	ldab	#0x40
    b1c5:	8f          	xgdx
    b1c6:	ff 01 62    	stx	0x162
    b1c9:	0e          	cli
    b1ca:	7d 00 d7    	tst	0xd7
    b1cd:	27 06       	beq	0x0xb1d5
    b1cf:	7f 00 d7    	clr	0xd7
    b1d2:	bd fe f8    	jsr	0xfef8
    b1d5:	39          	rts
    b1d6:	b2 3a b2    	sbca	0x3ab2
    b1d9:	7d b2 a5    	tst	0xb2a5
    b1dc:	b3 5f b4    	subd	0x5fb4
    b1df:	d1 b5       	cmpb	*0xb5
    b1e1:	fd b7 92    	std	0xb792
    b1e4:	b6 94 b8    	ldaa	0x94b8
    b1e7:	a9 b9       	adca	0xb9,x
    b1e9:	87          	.byte	0x87
    b1ea:	ba 69 bb    	oraa	0x69bb
    b1ed:	41          	.byte	0x41
    b1ee:	bb 44 bc    	adda	0x44bc
    b1f1:	f5 bd bf    	bitb	0xbdbf
    b1f4:	be 45 be    	lds	0x45be
    b1f7:	ae be       	lds	0xbe,x
    b1f9:	ae be       	lds	0xbe,x
    b1fb:	ae be       	lds	0xbe,x
    b1fd:	ce c0 58    	ldx	#0xc058
    b200:	c1 3f       	cmpb	#0x3f
    b202:	c2 26       	sbcb	#0x26
    b204:	c3 0f c4    	addd	#0xfc4
    b207:	30          	tsx
    b208:	c5 00       	bitb	#0x0
    b20a:	c6 de       	ldab	#0xde
    b20c:	c8 0d       	eorb	#0xd
    b20e:	c9 74       	adcb	#0x74
    b210:	c9 77       	adcb	#0x77
    b212:	ca b2       	orab	#0xb2
    b214:	cb 79       	addb	#0x79
    b216:	cc 40 cd    	ldd	#0x40cd
    b219:	21 cd       	brn	0x0xb1e8
    b21b:	2d ce       	blt	0x0xb1eb
    b21d:	1f ce 20 ce 	brclr	0xce,x, #0x20, 0x0xb1ef
    b221:	aa cf       	oraa	0xcf,x
    b223:	5c          	incb
    b224:	d0 5f       	subb	*0x5f
    b226:	d0 62       	subb	*0x62
    b228:	d1 32       	cmpb	*0x32
    b22a:	d2 e8       	sbcb	*0xe8
    b22c:	d3 c4       	addd	*0xc4
    b22e:	f6 01 1b    	ldab	0x11b
    b231:	58          	aslb
    b232:	ce b1 d6    	ldx	#0xb1d6
    b235:	3a          	abx
    b236:	ee 00       	ldx	0x0,x
    b238:	6e 00       	jmp	0x0,x
    b23a:	7d 01 1c    	tst	0x11c
    b23d:	2a 03       	bpl	0x0xb242
    b23f:	7e d5 43    	jmp	0xd543
    b242:	7d 01 1c    	tst	0x11c
    b245:	27 03       	beq	0x0xb24a
    b247:	7e b2 70    	jmp	0xb270
    b24a:	7d 01 1e    	tst	0x11e
    b24d:	26 08       	bne	0x0xb257
    b24f:	86 0c       	ldaa	#0xc
    b251:	b7 01 1e    	staa	0x11e
    b254:	bd ea e4    	jsr	0xeae4
    b257:	7d 01 7e    	tst	0x17e
    b25a:	27 0c       	beq	0x0xb268
    b25c:	7f 01 7e    	clr	0x17e
    b25f:	7f 01 7f    	clr	0x17f
    b262:	7f 01 1a    	clr	0x11a
    b265:	7e d5 43    	jmp	0xd543
    b268:	7d 01 1c    	tst	0x11c
    b26b:	26 03       	bne	0x0xb270
    b26d:	7e d5 2f    	jmp	0xd52f
    b270:	b6 10 23    	ldaa	0x1023
    b273:	85 10       	bita	#0x10
    b275:	27 03       	beq	0x0xb27a
    b277:	7e ea 58    	jmp	0xea58
    b27a:	7e d5 2f    	jmp	0xd52f
    b27d:	7d 01 1c    	tst	0x11c
    b280:	2a 03       	bpl	0x0xb285
    b282:	7e d6 0d    	jmp	0xd60d
    b285:	7d 01 1c    	tst	0x11c
    b288:	27 03       	beq	0x0xb28d
    b28a:	7e b2 70    	jmp	0xb270
    b28d:	7d 01 1e    	tst	0x11e
    b290:	26 08       	bne	0x0xb29a
    b292:	86 0c       	ldaa	#0xc
    b294:	b7 01 1e    	staa	0x11e
    b297:	bd ea e4    	jsr	0xeae4
    b29a:	7d 01 7e    	tst	0x17e
    b29d:	27 03       	beq	0x0xb2a2
    b29f:	7e d6 0d    	jmp	0xd60d
    b2a2:	7e d5 2f    	jmp	0xd52f
    b2a5:	7d 01 1c    	tst	0x11c
    b2a8:	2a 03       	bpl	0x0xb2ad
    b2aa:	7e d7 32    	jmp	0xd732
    b2ad:	7d 01 1c    	tst	0x11c
    b2b0:	27 03       	beq	0x0xb2b5
    b2b2:	7e b2 70    	jmp	0xb270
    b2b5:	7d 01 1e    	tst	0x11e
    b2b8:	26 08       	bne	0x0xb2c2
    b2ba:	86 10       	ldaa	#0x10
    b2bc:	b7 01 1e    	staa	0x11e
    b2bf:	bd ea e4    	jsr	0xeae4
    b2c2:	7d 01 7f    	tst	0x17f
    b2c5:	27 29       	beq	0x0xb2f0
    b2c7:	2b 1d       	bmi	0x0xb2e6
    b2c9:	b6 01 1e    	ldaa	0x11e
    b2cc:	4c          	inca
    b2cd:	81 1f       	cmpa	#0x1f
    b2cf:	23 02       	bls	0x0xb2d3
    b2d1:	20 06       	bra	0x0xb2d9
    b2d3:	b7 01 1e    	staa	0x11e
    b2d6:	bd ea e4    	jsr	0xeae4
    b2d9:	4f          	clra
    b2da:	b7 01 7f    	staa	0x17f
    b2dd:	b7 01 7e    	staa	0x17e
    b2e0:	b7 01 1a    	staa	0x11a
    b2e3:	7e d5 2f    	jmp	0xd52f
    b2e6:	b6 01 1e    	ldaa	0x11e
    b2e9:	4a          	deca
    b2ea:	81 0f       	cmpa	#0xf
    b2ec:	22 e5       	bhi	0x0xb2d3
    b2ee:	20 e9       	bra	0x0xb2d9
    b2f0:	7d 01 7e    	tst	0x17e
    b2f3:	27 0f       	beq	0x0xb304
    b2f5:	7f 01 7e    	clr	0x17e
    b2f8:	ce 01 20    	ldx	#0x120
    b2fb:	f6 01 1e    	ldab	0x11e
    b2fe:	3a          	abx
    b2ff:	86 20       	ldaa	#0x20
    b301:	7e b3 30    	jmp	0xb330
    b304:	7d 01 1a    	tst	0x11a
    b307:	26 03       	bne	0x0xb30c
    b309:	7e d5 2f    	jmp	0xd52f
    b30c:	ce 01 20    	ldx	#0x120
    b30f:	f6 01 1e    	ldab	0x11e
    b312:	3a          	abx
    b313:	a6 00       	ldaa	0x0,x
    b315:	81 5f       	cmpa	#0x5f
    b317:	23 02       	bls	0x0xb31b
    b319:	86 20       	ldaa	#0x20
    b31b:	7d 01 1a    	tst	0x11a
    b31e:	2b 09       	bmi	0x0xb329
    b320:	4c          	inca
    b321:	81 5f       	cmpa	#0x5f
    b323:	23 0b       	bls	0x0xb330
    b325:	86 20       	ldaa	#0x20
    b327:	20 07       	bra	0x0xb330
    b329:	4a          	deca
    b32a:	81 20       	cmpa	#0x20
    b32c:	24 02       	bcc	0x0xb330
    b32e:	86 5f       	ldaa	#0x5f
    b330:	7f 01 1a    	clr	0x11a
    b333:	a7 00       	staa	0x0,x
    b335:	36          	psha
    b336:	bd ea a8    	jsr	0xeaa8
    b339:	13 d1 80 0e 	brclr	*0xd1, #0x80, 0x0xb34b
    b33d:	32          	pula
    b33e:	8f          	xgdx
    b33f:	83 00 70    	subd	#0x70
    b342:	8f          	xgdx
    b343:	a7 00       	staa	0x0,x
    b345:	bd ea e4    	jsr	0xeae4
    b348:	7e d5 2f    	jmp	0xd52f
    b34b:	32          	pula
    b34c:	8f          	xgdx
    b34d:	83 01 30    	subd	#0x130
    b350:	c3 50 40    	addd	#0x5040
    b353:	8f          	xgdx
    b354:	a7 00       	staa	0x0,x
    b356:	bd ea e4    	jsr	0xeae4
    b359:	14 fb 80    	bset	*0xfb, #0x80
    b35c:	7e d5 2f    	jmp	0xd52f
    b35f:	7d 01 1c    	tst	0x11c
    b362:	2a 03       	bpl	0x0xb367
    b364:	7e d7 c6    	jmp	0xd7c6
    b367:	7d 01 1e    	tst	0x11e
    b36a:	26 08       	bne	0x0xb374
    b36c:	7d 01 1c    	tst	0x11c
    b36f:	27 1f       	beq	0x0xb390
    b371:	7e b2 70    	jmp	0xb270
    b374:	7d 01 1c    	tst	0x11c
    b377:	27 1f       	beq	0x0xb398
    b379:	cc 01 20    	ldd	#0x120
    b37c:	fb 01 1e    	addb	0x11e
    b37f:	8f          	xgdx
    b380:	bd ea a8    	jsr	0xeaa8
    b383:	7a 01 1e    	dec	0x11e
    b386:	7a 01 1c    	dec	0x11c
    b389:	26 0d       	bne	0x0xb398
    b38b:	bd eb 26    	jsr	0xeb26
    b38e:	20 08       	bra	0x0xb398
    b390:	86 13       	ldaa	#0x13
    b392:	b7 01 1e    	staa	0x11e
    b395:	bd ea e4    	jsr	0xeae4
    b398:	7d 01 7f    	tst	0x17f
    b39b:	27 10       	beq	0x0xb3ad
    b39d:	bd ec e8    	jsr	0xece8
    b3a0:	4f          	clra
    b3a1:	b7 01 7f    	staa	0x17f
    b3a4:	b7 01 7e    	staa	0x17e
    b3a7:	b7 01 1a    	staa	0x11a
    b3aa:	7e d5 2f    	jmp	0xd52f
    b3ad:	7d 01 1a    	tst	0x11a
    b3b0:	26 03       	bne	0x0xb3b5
    b3b2:	7e d5 2f    	jmp	0xd52f
    b3b5:	b6 01 1e    	ldaa	0x11e
    b3b8:	81 13       	cmpa	#0x13
    b3ba:	26 46       	bne	0x0xb402
    b3bc:	b6 01 1a    	ldaa	0x11a
    b3bf:	2b 3b       	bmi	0x0xb3fc
    b3c1:	f6 01 6f    	ldab	0x16f
    b3c4:	5c          	incb
    b3c5:	c4 0f       	andb	#0xf
    b3c7:	f7 01 6f    	stab	0x16f
    b3ca:	b6 10 00    	ldaa	0x1000
    b3cd:	36          	psha
    b3ce:	84 ef       	anda	#0xef
    b3d0:	b7 10 00    	staa	0x1000
    b3d3:	b6 10 08    	ldaa	0x1008
    b3d6:	36          	psha
    b3d7:	84 1f       	anda	#0x1f
    b3d9:	b7 10 08    	staa	0x1008
    b3dc:	f7 7f fe    	stab	0x7ffe
    b3df:	32          	pula
    b3e0:	b7 10 08    	staa	0x1008
    b3e3:	32          	pula
    b3e4:	b7 10 00    	staa	0x1000
    b3e7:	ce d8 24    	ldx	#0xd824
    b3ea:	18 ce 01 30 	ldy	#0x130
    b3ee:	bd ec 6f    	jsr	0xec6f
    b3f1:	7f 01 1a    	clr	0x11a
    b3f4:	86 04       	ldaa	#0x4
    b3f6:	b7 01 1c    	staa	0x11c
    b3f9:	7e d5 2f    	jmp	0xd52f
    b3fc:	f6 01 6f    	ldab	0x16f
    b3ff:	5a          	decb
    b400:	20 c3       	bra	0x0xb3c5
    b402:	81 19       	cmpa	#0x19
    b404:	26 34       	bne	0x0xb43a
    b406:	b6 01 1a    	ldaa	0x11a
    b409:	2b 27       	bmi	0x0xb432
    b40b:	d6 93       	ldab	*0x93
    b40d:	5c          	incb
    b40e:	c1 05       	cmpb	#0x5
    b410:	25 02       	bcs	0x0xb414
    b412:	c6 04       	ldab	#0x4
    b414:	d7 93       	stab	*0x93
    b416:	ce d8 68    	ldx	#0xd868
    b419:	18 ce 01 36 	ldy	#0x136
    b41d:	bd ec 6f    	jsr	0xec6f
    b420:	7f 01 1a    	clr	0x11a
    b423:	86 04       	ldaa	#0x4
    b425:	b7 01 1c    	staa	0x11c
    b428:	96 93       	ldaa	*0x93
    b42a:	c6 93       	ldab	#0x93
    b42c:	bd b0 fc    	jsr	0xb0fc
    b42f:	7e d5 2f    	jmp	0xd52f
    b432:	d6 93       	ldab	*0x93
    b434:	5a          	decb
    b435:	2a dd       	bpl	0x0xb414
    b437:	5f          	clrb
    b438:	20 da       	bra	0x0xb414
    b43a:	96 f9       	ldaa	*0xf9
    b43c:	81 04       	cmpa	#0x4
    b43e:	26 03       	bne	0x0xb443
    b440:	7e b4 ac    	jmp	0xb4ac
    b443:	ce 00 20    	ldx	#0x20
    b446:	18 ce f0 0e 	ldy	#0xf00e
    b44a:	c6 b0       	ldab	#0xb0
    b44c:	18 a6 00    	ldaa	0x0,y
    b44f:	a7 00       	staa	0x0,x
    b451:	08          	inx
    b452:	18 08       	iny
    b454:	5a          	decb
    b455:	26 f5       	bne	0x0xb44c
    b457:	7d 00 d0    	tst	0xd0
    b45a:	26 0b       	bne	0x0xb467
    b45c:	7d 10 29    	tst	0x1029
    b45f:	2a fb       	bpl	0x0xb45c
    b461:	b6 10 2a    	ldaa	0x102a
    b464:	7c 00 d0    	inc	0xd0
    b467:	4f          	clra
    b468:	01          	nop
    b469:	01          	nop
    b46a:	01          	nop
    b46b:	01          	nop
    b46c:	b7 10 42    	staa	0x1042
    b46f:	01          	nop
    b470:	01          	nop
    b471:	01          	nop
    b472:	01          	nop
    b473:	01          	nop
    b474:	bd a4 3e    	jsr	0xa43e
    b477:	4f          	clra
    b478:	97 f2       	staa	*0xf2
    b47a:	97 f6       	staa	*0xf6
    b47c:	97 ff       	staa	*0xff
    b47e:	b7 01 1b    	staa	0x11b
    b481:	97 fb       	staa	*0xfb
    b483:	96 41       	ldaa	*0x41
    b485:	84 10       	anda	#0x10
    b487:	48          	asla
    b488:	48          	asla
    b489:	48          	asla
    b48a:	9a 44       	oraa	*0x44
    b48c:	97 f7       	staa	*0xf7
    b48e:	96 73       	ldaa	*0x73
    b490:	97 f3       	staa	*0xf3
    b492:	96 4a       	ldaa	*0x4a
    b494:	48          	asla
    b495:	48          	asla
    b496:	48          	asla
    b497:	48          	asla
    b498:	48          	asla
    b499:	97 f4       	staa	*0xf4
    b49b:	96 6a       	ldaa	*0x6a
    b49d:	84 20       	anda	#0x20
    b49f:	97 f5       	staa	*0xf5
    b4a1:	86 80       	ldaa	#0x80
    b4a3:	b7 01 1c    	staa	0x11c
    b4a6:	7f 01 1a    	clr	0x11a
    b4a9:	7e d5 2f    	jmp	0xd52f
    b4ac:	ce 50 00    	ldx	#0x5000
    b4af:	18 ce f0 be 	ldy	#0xf0be
    b4b3:	c6 50       	ldab	#0x50
    b4b5:	18 a6 00    	ldaa	0x0,y
    b4b8:	a7 00       	staa	0x0,x
    b4ba:	08          	inx
    b4bb:	18 08       	iny
    b4bd:	5a          	decb
    b4be:	26 f5       	bne	0x0xb4b5
    b4c0:	bd a5 ab    	jsr	0xa5ab
    b4c3:	86 80       	ldaa	#0x80
    b4c5:	b7 01 1c    	staa	0x11c
    b4c8:	7f 01 1a    	clr	0x11a
    b4cb:	7f 01 1b    	clr	0x11b
    b4ce:	7e d5 2f    	jmp	0xd52f
    b4d1:	7d 01 1c    	tst	0x11c
    b4d4:	2a 03       	bpl	0x0xb4d9
    b4d6:	7e d8 82    	jmp	0xd882
    b4d9:	7d 01 1e    	tst	0x11e
    b4dc:	26 08       	bne	0x0xb4e6
    b4de:	7d 01 1c    	tst	0x11c
    b4e1:	27 1f       	beq	0x0xb502
    b4e3:	7e b2 70    	jmp	0xb270
    b4e6:	7d 01 1c    	tst	0x11c
    b4e9:	27 24       	beq	0x0xb50f
    b4eb:	cc 01 20    	ldd	#0x120
    b4ee:	fb 01 1e    	addb	0x11e
    b4f1:	8f          	xgdx
    b4f2:	bd ea a8    	jsr	0xeaa8
    b4f5:	7a 01 1e    	dec	0x11e
    b4f8:	7a 01 1c    	dec	0x11c
    b4fb:	26 12       	bne	0x0xb50f
    b4fd:	bd eb 26    	jsr	0xeb26
    b500:	20 0d       	bra	0x0xb50f
    b502:	7d 01 1e    	tst	0x11e
    b505:	26 08       	bne	0x0xb50f
    b507:	86 13       	ldaa	#0x13
    b509:	b7 01 1e    	staa	0x11e
    b50c:	bd ea e4    	jsr	0xeae4
    b50f:	7d 01 7f    	tst	0x17f
    b512:	27 10       	beq	0x0xb524
    b514:	bd ec e8    	jsr	0xece8
    b517:	4f          	clra
    b518:	b7 01 7f    	staa	0x17f
    b51b:	b7 01 7e    	staa	0x17e
    b51e:	b7 01 1a    	staa	0x11a
    b521:	7e d5 2f    	jmp	0xd52f
    b524:	7d 01 1a    	tst	0x11a
    b527:	26 03       	bne	0x0xb52c
    b529:	7e d5 2f    	jmp	0xd52f
    b52c:	b6 01 1e    	ldaa	0x11e
    b52f:	81 13       	cmpa	#0x13
    b531:	26 4d       	bne	0x0xb580
    b533:	b6 01 1a    	ldaa	0x11a
    b536:	2b 3e       	bmi	0x0xb576
    b538:	f6 01 70    	ldab	0x170
    b53b:	5c          	incb
    b53c:	c1 03       	cmpb	#0x3
    b53e:	25 01       	bcs	0x0xb541
    b540:	5f          	clrb
    b541:	f7 01 70    	stab	0x170
    b544:	b6 10 00    	ldaa	0x1000
    b547:	36          	psha
    b548:	84 ef       	anda	#0xef
    b54a:	b7 10 00    	staa	0x1000
    b54d:	b6 10 08    	ldaa	0x1008
    b550:	36          	psha
    b551:	84 1f       	anda	#0x1f
    b553:	b7 10 08    	staa	0x1008
    b556:	f7 7f ff    	stab	0x7fff
    b559:	32          	pula
    b55a:	b7 10 08    	staa	0x1008
    b55d:	32          	pula
    b55e:	b7 10 00    	staa	0x1000
    b561:	ce d8 de    	ldx	#0xd8de
    b564:	18 ce 01 30 	ldy	#0x130
    b568:	bd ec 6f    	jsr	0xec6f
    b56b:	7f 01 1a    	clr	0x11a
    b56e:	86 04       	ldaa	#0x4
    b570:	b7 01 1c    	staa	0x11c
    b573:	7e d5 2f    	jmp	0xd52f
    b576:	f6 01 70    	ldab	0x170
    b579:	5a          	decb
    b57a:	2a c5       	bpl	0x0xb541
    b57c:	c6 02       	ldab	#0x2
    b57e:	20 c1       	bra	0x0xb541
    b580:	81 19       	cmpa	#0x19
    b582:	26 2d       	bne	0x0xb5b1
    b584:	b6 01 1a    	ldaa	0x11a
    b587:	2b 1f       	bmi	0x0xb5a8
    b589:	d6 94       	ldab	*0x94
    b58b:	5c          	incb
    b58c:	c1 03       	cmpb	#0x3
    b58e:	25 01       	bcs	0x0xb591
    b590:	5f          	clrb
    b591:	d7 94       	stab	*0x94
    b593:	ce d8 ea    	ldx	#0xd8ea
    b596:	18 ce 01 36 	ldy	#0x136
    b59a:	bd ec 6f    	jsr	0xec6f
    b59d:	7f 01 1a    	clr	0x11a
    b5a0:	86 04       	ldaa	#0x4
    b5a2:	b7 01 1c    	staa	0x11c
    b5a5:	7e d5 2f    	jmp	0xd52f
    b5a8:	d6 94       	ldab	*0x94
    b5aa:	5a          	decb
    b5ab:	2a e4       	bpl	0x0xb591
    b5ad:	c6 02       	ldab	#0x2
    b5af:	20 e0       	bra	0x0xb591
    b5b1:	b6 01 1a    	ldaa	0x11a
    b5b4:	2b 3e       	bmi	0x0xb5f4
    b5b6:	b6 01 71    	ldaa	0x171
    b5b9:	4c          	inca
    b5ba:	81 32       	cmpa	#0x32
    b5bc:	23 02       	bls	0x0xb5c0
    b5be:	86 32       	ldaa	#0x32
    b5c0:	b7 01 71    	staa	0x171
    b5c3:	b7 10 46    	staa	0x1046
    b5c6:	f6 10 00    	ldab	0x1000
    b5c9:	37          	pshb
    b5ca:	c4 ef       	andb	#0xef
    b5cc:	f7 10 00    	stab	0x1000
    b5cf:	f6 10 08    	ldab	0x1008
    b5d2:	37          	pshb
    b5d3:	c4 1f       	andb	#0x1f
    b5d5:	f7 10 08    	stab	0x1008
    b5d8:	b7 7f fc    	staa	0x7ffc
    b5db:	33          	pulb
    b5dc:	f7 10 08    	stab	0x1008
    b5df:	33          	pulb
    b5e0:	f7 10 00    	stab	0x1000
    b5e3:	ce 01 3c    	ldx	#0x13c
    b5e6:	bd eb bc    	jsr	0xebbc
    b5e9:	7f 01 1a    	clr	0x11a
    b5ec:	86 03       	ldaa	#0x3
    b5ee:	b7 01 1c    	staa	0x11c
    b5f1:	7e d5 2f    	jmp	0xd52f
    b5f4:	b6 01 71    	ldaa	0x171
    b5f7:	4a          	deca
    b5f8:	2a c6       	bpl	0x0xb5c0
    b5fa:	4f          	clra
    b5fb:	20 c3       	bra	0x0xb5c0
    b5fd:	7d 01 1c    	tst	0x11c
    b600:	2a 03       	bpl	0x0xb605
    b602:	7e d8 f6    	jmp	0xd8f6
    b605:	7d 01 1e    	tst	0x11e
    b608:	26 08       	bne	0x0xb612
    b60a:	7d 01 1c    	tst	0x11c
    b60d:	27 1f       	beq	0x0xb62e
    b60f:	7e b2 70    	jmp	0xb270
    b612:	7d 01 1c    	tst	0x11c
    b615:	27 24       	beq	0x0xb63b
    b617:	cc 01 20    	ldd	#0x120
    b61a:	fb 01 1e    	addb	0x11e
    b61d:	8f          	xgdx
    b61e:	bd ea a8    	jsr	0xeaa8
    b621:	7a 01 1e    	dec	0x11e
    b624:	7a 01 1c    	dec	0x11c
    b627:	26 12       	bne	0x0xb63b
    b629:	bd eb 26    	jsr	0xeb26
    b62c:	20 0d       	bra	0x0xb63b
    b62e:	7d 01 1e    	tst	0x11e
    b631:	26 08       	bne	0x0xb63b
    b633:	86 1e       	ldaa	#0x1e
    b635:	b7 01 1e    	staa	0x11e
    b638:	bd ea e4    	jsr	0xeae4
    b63b:	7f 01 7e    	clr	0x17e
    b63e:	7f 01 7f    	clr	0x17f
    b641:	7d 01 1a    	tst	0x11a
    b644:	26 03       	bne	0x0xb649
    b646:	7e d5 2f    	jmp	0xd52f
    b649:	b6 01 3c    	ldaa	0x13c
    b64c:	81 41       	cmpa	#0x41
    b64e:	26 04       	bne	0x0xb654
    b650:	86 81       	ldaa	#0x81
    b652:	20 06       	bra	0x0xb65a
    b654:	ce 01 3c    	ldx	#0x13c
    b657:	bd ec 16    	jsr	0xec16
    b65a:	7d 01 1a    	tst	0x11a
    b65d:	2b 09       	bmi	0x0xb668
    b65f:	4c          	inca
    b660:	81 81       	cmpa	#0x81
    b662:	23 09       	bls	0x0xb66d
    b664:	86 01       	ldaa	#0x1
    b666:	20 05       	bra	0x0xb66d
    b668:	4a          	deca
    b669:	26 02       	bne	0x0xb66d
    b66b:	86 81       	ldaa	#0x81
    b66d:	81 81       	cmpa	#0x81
    b66f:	26 0f       	bne	0x0xb680
    b671:	86 41       	ldaa	#0x41
    b673:	b7 01 3c    	staa	0x13c
    b676:	86 4c       	ldaa	#0x4c
    b678:	b7 01 3d    	staa	0x13d
    b67b:	b7 01 3e    	staa	0x13e
    b67e:	20 06       	bra	0x0xb686
    b680:	ce 01 3c    	ldx	#0x13c
    b683:	bd eb bc    	jsr	0xebbc
    b686:	7f 01 1a    	clr	0x11a
    b689:	86 03       	ldaa	#0x3
    b68b:	b7 01 1c    	staa	0x11c
    b68e:	14 f2 40    	bset	*0xf2, #0x40
    b691:	7e d5 2f    	jmp	0xd52f
    b694:	7d 01 1c    	tst	0x11c
    b697:	2a 03       	bpl	0x0xb69c
    b699:	7e d9 81    	jmp	0xd981
    b69c:	7d 01 1e    	tst	0x11e
    b69f:	26 08       	bne	0x0xb6a9
    b6a1:	7d 01 1c    	tst	0x11c
    b6a4:	27 1f       	beq	0x0xb6c5
    b6a6:	7e b2 70    	jmp	0xb270
    b6a9:	7d 01 1c    	tst	0x11c
    b6ac:	27 24       	beq	0x0xb6d2
    b6ae:	cc 01 20    	ldd	#0x120
    b6b1:	fb 01 1e    	addb	0x11e
    b6b4:	8f          	xgdx
    b6b5:	bd ea a8    	jsr	0xeaa8
    b6b8:	7a 01 1e    	dec	0x11e
    b6bb:	7a 01 1c    	dec	0x11c
    b6be:	26 12       	bne	0x0xb6d2
    b6c0:	bd eb 26    	jsr	0xeb26
    b6c3:	20 0d       	bra	0x0xb6d2
    b6c5:	7d 01 1e    	tst	0x11e
    b6c8:	26 08       	bne	0x0xb6d2
    b6ca:	86 13       	ldaa	#0x13
    b6cc:	b7 01 1e    	staa	0x11e
    b6cf:	bd ea e4    	jsr	0xeae4
    b6d2:	7d 01 7f    	tst	0x17f
    b6d5:	27 10       	beq	0x0xb6e7
    b6d7:	bd ec e8    	jsr	0xece8
    b6da:	4f          	clra
    b6db:	b7 01 7f    	staa	0x17f
    b6de:	b7 01 7e    	staa	0x17e
    b6e1:	b7 01 1a    	staa	0x11a
    b6e4:	7e d5 2f    	jmp	0xd52f
    b6e7:	7d 01 1a    	tst	0x11a
    b6ea:	26 03       	bne	0x0xb6ef
    b6ec:	7e d5 2f    	jmp	0xd52f
    b6ef:	b6 01 1e    	ldaa	0x11e
    b6f2:	81 13       	cmpa	#0x13
    b6f4:	26 30       	bne	0x0xb726
    b6f6:	b6 01 1a    	ldaa	0x11a
    b6f9:	2b 23       	bmi	0x0xb71e
    b6fb:	96 40       	ldaa	*0x40
    b6fd:	4c          	inca
    b6fe:	81 7f       	cmpa	#0x7f
    b700:	25 02       	bcs	0x0xb704
    b702:	86 7f       	ldaa	#0x7f
    b704:	97 40       	staa	*0x40
    b706:	ce 01 31    	ldx	#0x131
    b709:	bd eb f8    	jsr	0xebf8
    b70c:	7f 01 1a    	clr	0x11a
    b70f:	86 03       	ldaa	#0x3
    b711:	b7 01 1c    	staa	0x11c
    b714:	96 40       	ldaa	*0x40
    b716:	c6 40       	ldab	#0x40
    b718:	bd b0 fc    	jsr	0xb0fc
    b71b:	7e d5 2f    	jmp	0xd52f
    b71e:	96 40       	ldaa	*0x40
    b720:	4a          	deca
    b721:	2a e1       	bpl	0x0xb704
    b723:	4f          	clra
    b724:	20 de       	bra	0x0xb704
    b726:	81 19       	cmpa	#0x19
    b728:	26 3d       	bne	0x0xb767
    b72a:	b6 01 1a    	ldaa	0x11a
    b72d:	2b 2e       	bmi	0x0xb75d
    b72f:	d6 41       	ldab	*0x41
    b731:	c4 0f       	andb	#0xf
    b733:	5c          	incb
    b734:	c1 05       	cmpb	#0x5
    b736:	25 02       	bcs	0x0xb73a
    b738:	c6 04       	ldab	#0x4
    b73a:	96 41       	ldaa	*0x41
    b73c:	84 10       	anda	#0x10
    b73e:	1b          	aba
    b73f:	97 41       	staa	*0x41
    b741:	ce d9 e8    	ldx	#0xd9e8
    b744:	18 ce 01 36 	ldy	#0x136
    b748:	bd ec 6f    	jsr	0xec6f
    b74b:	7f 01 1a    	clr	0x11a
    b74e:	86 04       	ldaa	#0x4
    b750:	b7 01 1c    	staa	0x11c
    b753:	96 41       	ldaa	*0x41
    b755:	c6 41       	ldab	#0x41
    b757:	bd b0 fc    	jsr	0xb0fc
    b75a:	7e d5 2f    	jmp	0xd52f
    b75d:	d6 41       	ldab	*0x41
    b75f:	c4 0f       	andb	#0xf
    b761:	5a          	decb
    b762:	2a d6       	bpl	0x0xb73a
    b764:	5f          	clrb
    b765:	20 d3       	bra	0x0xb73a
    b767:	b6 01 1a    	ldaa	0x11a
    b76a:	2b 1f       	bmi	0x0xb78b
    b76c:	96 67       	ldaa	*0x67
    b76e:	4c          	inca
    b76f:	84 7f       	anda	#0x7f
    b771:	97 67       	staa	*0x67
    b773:	ce 01 3c    	ldx	#0x13c
    b776:	bd eb bc    	jsr	0xebbc
    b779:	7f 01 1a    	clr	0x11a
    b77c:	86 03       	ldaa	#0x3
    b77e:	b7 01 1c    	staa	0x11c
    b781:	96 67       	ldaa	*0x67
    b783:	c6 67       	ldab	#0x67
    b785:	bd b0 fc    	jsr	0xb0fc
    b788:	7e d5 2f    	jmp	0xd52f
    b78b:	96 67       	ldaa	*0x67
    b78d:	4a          	deca
    b78e:	84 7f       	anda	#0x7f
    b790:	20 df       	bra	0x0xb771
    b792:	7d 01 1c    	tst	0x11c
    b795:	2a 03       	bpl	0x0xb79a
    b797:	7e d9 fc    	jmp	0xd9fc
    b79a:	7d 01 1e    	tst	0x11e
    b79d:	26 08       	bne	0x0xb7a7
    b79f:	7d 01 1c    	tst	0x11c
    b7a2:	27 1f       	beq	0x0xb7c3
    b7a4:	7e b2 70    	jmp	0xb270
    b7a7:	7d 01 1c    	tst	0x11c
    b7aa:	27 24       	beq	0x0xb7d0
    b7ac:	cc 01 20    	ldd	#0x120
    b7af:	fb 01 1e    	addb	0x11e
    b7b2:	8f          	xgdx
    b7b3:	bd ea a8    	jsr	0xeaa8
    b7b6:	7a 01 1e    	dec	0x11e
    b7b9:	7a 01 1c    	dec	0x11c
    b7bc:	26 12       	bne	0x0xb7d0
    b7be:	bd eb 26    	jsr	0xeb26
    b7c1:	20 0d       	bra	0x0xb7d0
    b7c3:	7d 01 1e    	tst	0x11e
    b7c6:	26 08       	bne	0x0xb7d0
    b7c8:	86 13       	ldaa	#0x13
    b7ca:	b7 01 1e    	staa	0x11e
    b7cd:	bd ea e4    	jsr	0xeae4
    b7d0:	7d 01 7f    	tst	0x17f
    b7d3:	27 10       	beq	0x0xb7e5
    b7d5:	bd ec e8    	jsr	0xece8
    b7d8:	4f          	clra
    b7d9:	b7 01 7f    	staa	0x17f
    b7dc:	b7 01 7e    	staa	0x17e
    b7df:	b7 01 1a    	staa	0x11a
    b7e2:	7e d5 2f    	jmp	0xd52f
    b7e5:	7d 01 1a    	tst	0x11a
    b7e8:	26 03       	bne	0x0xb7ed
    b7ea:	7e d5 2f    	jmp	0xd52f
    b7ed:	b6 01 1e    	ldaa	0x11e
    b7f0:	81 13       	cmpa	#0x13
    b7f2:	26 51       	bne	0x0xb845
    b7f4:	b6 01 1a    	ldaa	0x11a
    b7f7:	2b 43       	bmi	0x0xb83c
    b7f9:	b6 01 6e    	ldaa	0x16e
    b7fc:	4c          	inca
    b7fd:	81 7f       	cmpa	#0x7f
    b7ff:	25 02       	bcs	0x0xb803
    b801:	86 7f       	ldaa	#0x7f
    b803:	b7 01 6e    	staa	0x16e
    b806:	f6 10 00    	ldab	0x1000
    b809:	37          	pshb
    b80a:	c4 ef       	andb	#0xef
    b80c:	f7 10 00    	stab	0x1000
    b80f:	f6 10 08    	ldab	0x1008
    b812:	37          	pshb
    b813:	c4 1f       	andb	#0x1f
    b815:	f7 10 08    	stab	0x1008
    b818:	b7 7f fd    	staa	0x7ffd
    b81b:	33          	pulb
    b81c:	f7 10 08    	stab	0x1008
    b81f:	33          	pulb
    b820:	f7 10 00    	stab	0x1000
    b823:	ce 01 31    	ldx	#0x131
    b826:	bd eb f8    	jsr	0xebf8
    b829:	7f 01 1a    	clr	0x11a
    b82c:	86 03       	ldaa	#0x3
    b82e:	b7 01 1c    	staa	0x11c
    b831:	b6 01 6e    	ldaa	0x16e
    b834:	c6 ab       	ldab	#0xab
    b836:	bd b0 fc    	jsr	0xb0fc
    b839:	7e d5 2f    	jmp	0xd52f
    b83c:	b6 01 6e    	ldaa	0x16e
    b83f:	4a          	deca
    b840:	2a c1       	bpl	0x0xb803
    b842:	4f          	clra
    b843:	20 be       	bra	0x0xb803
    b845:	81 19       	cmpa	#0x19
    b847:	26 30       	bne	0x0xb879
    b849:	b6 01 1a    	ldaa	0x11a
    b84c:	2b 23       	bmi	0x0xb871
    b84e:	96 3e       	ldaa	*0x3e
    b850:	4c          	inca
    b851:	81 40       	cmpa	#0x40
    b853:	25 02       	bcs	0x0xb857
    b855:	86 3f       	ldaa	#0x3f
    b857:	97 3e       	staa	*0x3e
    b859:	ce 01 37    	ldx	#0x137
    b85c:	bd eb bc    	jsr	0xebbc
    b85f:	7f 01 1a    	clr	0x11a
    b862:	86 03       	ldaa	#0x3
    b864:	b7 01 1c    	staa	0x11c
    b867:	96 3e       	ldaa	*0x3e
    b869:	c6 3e       	ldab	#0x3e
    b86b:	bd b0 fc    	jsr	0xb0fc
    b86e:	7e d5 2f    	jmp	0xd52f
    b871:	96 3e       	ldaa	*0x3e
    b873:	4a          	deca
    b874:	2a e1       	bpl	0x0xb857
    b876:	4f          	clra
    b877:	20 de       	bra	0x0xb857
    b879:	b6 01 1a    	ldaa	0x11a
    b87c:	2b 23       	bmi	0x0xb8a1
    b87e:	96 3f       	ldaa	*0x3f
    b880:	4c          	inca
    b881:	81 40       	cmpa	#0x40
    b883:	25 02       	bcs	0x0xb887
    b885:	86 3f       	ldaa	#0x3f
    b887:	97 3f       	staa	*0x3f
    b889:	ce 01 3c    	ldx	#0x13c
    b88c:	bd eb bc    	jsr	0xebbc
    b88f:	7f 01 1a    	clr	0x11a
    b892:	86 03       	ldaa	#0x3
    b894:	b7 01 1c    	staa	0x11c
    b897:	96 3f       	ldaa	*0x3f
    b899:	c6 3f       	ldab	#0x3f
    b89b:	bd b0 fc    	jsr	0xb0fc
    b89e:	7e d5 2f    	jmp	0xd52f
    b8a1:	96 3f       	ldaa	*0x3f
    b8a3:	4a          	deca
    b8a4:	2a e1       	bpl	0x0xb887
    b8a6:	4f          	clra
    b8a7:	20 de       	bra	0x0xb887
    b8a9:	7d 01 1c    	tst	0x11c
    b8ac:	2a 03       	bpl	0x0xb8b1
    b8ae:	7e da 5e    	jmp	0xda5e
    b8b1:	7d 01 1e    	tst	0x11e
    b8b4:	26 08       	bne	0x0xb8be
    b8b6:	7d 01 1c    	tst	0x11c
    b8b9:	27 1f       	beq	0x0xb8da
    b8bb:	7e b2 70    	jmp	0xb270
    b8be:	7d 01 1c    	tst	0x11c
    b8c1:	27 24       	beq	0x0xb8e7
    b8c3:	cc 01 20    	ldd	#0x120
    b8c6:	fb 01 1e    	addb	0x11e
    b8c9:	8f          	xgdx
    b8ca:	bd ea a8    	jsr	0xeaa8
    b8cd:	7a 01 1e    	dec	0x11e
    b8d0:	7a 01 1c    	dec	0x11c
    b8d3:	26 12       	bne	0x0xb8e7
    b8d5:	bd eb 59    	jsr	0xeb59
    b8d8:	20 0d       	bra	0x0xb8e7
    b8da:	7d 01 1e    	tst	0x11e
    b8dd:	26 08       	bne	0x0xb8e7
    b8df:	86 12       	ldaa	#0x12
    b8e1:	b7 01 1e    	staa	0x11e
    b8e4:	bd ea e4    	jsr	0xeae4
    b8e7:	7d 01 7f    	tst	0x17f
    b8ea:	27 10       	beq	0x0xb8fc
    b8ec:	bd ed 5a    	jsr	0xed5a
    b8ef:	4f          	clra
    b8f0:	b7 01 7f    	staa	0x17f
    b8f3:	b7 01 7e    	staa	0x17e
    b8f6:	b7 01 1a    	staa	0x11a
    b8f9:	7e d5 2f    	jmp	0xd52f
    b8fc:	7d 01 1a    	tst	0x11a
    b8ff:	26 03       	bne	0x0xb904
    b901:	7e d5 2f    	jmp	0xd52f
    b904:	b6 01 1e    	ldaa	0x11e
    b907:	81 12       	cmpa	#0x12
    b909:	26 2a       	bne	0x0xb935
    b90b:	d6 21       	ldab	*0x21
    b90d:	c8 40       	eorb	#0x40
    b90f:	d7 21       	stab	*0x21
    b911:	c4 40       	andb	#0x40
    b913:	54          	lsrb
    b914:	54          	lsrb
    b915:	54          	lsrb
    b916:	54          	lsrb
    b917:	54          	lsrb
    b918:	54          	lsrb
    b919:	ce da ec    	ldx	#0xdaec
    b91c:	18 ce 01 30 	ldy	#0x130
    b920:	bd ec 5b    	jsr	0xec5b
    b923:	7f 01 1a    	clr	0x11a
    b926:	86 03       	ldaa	#0x3
    b928:	b7 01 1c    	staa	0x11c
    b92b:	96 21       	ldaa	*0x21
    b92d:	c6 21       	ldab	#0x21
    b92f:	bd b0 fc    	jsr	0xb0fc
    b932:	7e d5 2f    	jmp	0xd52f
    b935:	81 16       	cmpa	#0x16
    b937:	26 02       	bne	0x0xb93b
    b939:	20 e8       	bra	0x0xb923
    b93b:	81 1a       	cmpa	#0x1a
    b93d:	26 14       	bne	0x0xb953
    b93f:	d6 21       	ldab	*0x21
    b941:	c8 01       	eorb	#0x1
    b943:	d7 21       	stab	*0x21
    b945:	c4 01       	andb	#0x1
    b947:	18 ce 01 38 	ldy	#0x138
    b94b:	ce da f8    	ldx	#0xdaf8
    b94e:	bd ec 5b    	jsr	0xec5b
    b951:	20 d0       	bra	0x0xb923
    b953:	7d 01 1a    	tst	0x11a
    b956:	2b 27       	bmi	0x0xb97f
    b958:	d6 24       	ldab	*0x24
    b95a:	5c          	incb
    b95b:	c1 07       	cmpb	#0x7
    b95d:	25 02       	bcs	0x0xb961
    b95f:	c6 06       	ldab	#0x6
    b961:	d7 24       	stab	*0x24
    b963:	ce da fe    	ldx	#0xdafe
    b966:	18 ce 01 3c 	ldy	#0x13c
    b96a:	bd ec 5b    	jsr	0xec5b
    b96d:	7f 01 1a    	clr	0x11a
    b970:	86 03       	ldaa	#0x3
    b972:	b7 01 1c    	staa	0x11c
    b975:	96 24       	ldaa	*0x24
    b977:	c6 24       	ldab	#0x24
    b979:	bd b0 fc    	jsr	0xb0fc
    b97c:	7e d5 2f    	jmp	0xd52f
    b97f:	d6 24       	ldab	*0x24
    b981:	5a          	decb
    b982:	2a dd       	bpl	0x0xb961
    b984:	5f          	clrb
    b985:	20 da       	bra	0x0xb961
    b987:	7d 01 1c    	tst	0x11c
    b98a:	2a 03       	bpl	0x0xb98f
    b98c:	7e db 13    	jmp	0xdb13
    b98f:	7d 01 1e    	tst	0x11e
    b992:	26 08       	bne	0x0xb99c
    b994:	7d 01 1c    	tst	0x11c
    b997:	27 1f       	beq	0x0xb9b8
    b999:	7e b2 70    	jmp	0xb270
    b99c:	7d 01 1c    	tst	0x11c
    b99f:	27 24       	beq	0x0xb9c5
    b9a1:	cc 01 20    	ldd	#0x120
    b9a4:	fb 01 1e    	addb	0x11e
    b9a7:	8f          	xgdx
    b9a8:	bd ea a8    	jsr	0xeaa8
    b9ab:	7a 01 1e    	dec	0x11e
    b9ae:	7a 01 1c    	dec	0x11c
    b9b1:	26 12       	bne	0x0xb9c5
    b9b3:	bd eb 26    	jsr	0xeb26
    b9b6:	20 0d       	bra	0x0xb9c5
    b9b8:	7d 01 1e    	tst	0x11e
    b9bb:	26 08       	bne	0x0xb9c5
    b9bd:	86 13       	ldaa	#0x13
    b9bf:	b7 01 1e    	staa	0x11e
    b9c2:	bd ea e4    	jsr	0xeae4
    b9c5:	7d 01 7f    	tst	0x17f
    b9c8:	27 10       	beq	0x0xb9da
    b9ca:	bd ec e8    	jsr	0xece8
    b9cd:	4f          	clra
    b9ce:	b7 01 7f    	staa	0x17f
    b9d1:	b7 01 7e    	staa	0x17e
    b9d4:	b7 01 1a    	staa	0x11a
    b9d7:	7e d5 2f    	jmp	0xd52f
    b9da:	7d 01 1a    	tst	0x11a
    b9dd:	26 03       	bne	0x0xb9e2
    b9df:	7e d5 2f    	jmp	0xd52f
    b9e2:	b6 01 1e    	ldaa	0x11e
    b9e5:	81 13       	cmpa	#0x13
    b9e7:	26 3d       	bne	0x0xba26
    b9e9:	b6 01 1a    	ldaa	0x11a
    b9ec:	2b 2a       	bmi	0x0xba18
    b9ee:	96 6a       	ldaa	*0x6a
    b9f0:	84 20       	anda	#0x20
    b9f2:	d6 6a       	ldab	*0x6a
    b9f4:	c4 0f       	andb	#0xf
    b9f6:	5c          	incb
    b9f7:	c1 05       	cmpb	#0x5
    b9f9:	25 02       	bcs	0x0xb9fd
    b9fb:	c6 04       	ldab	#0x4
    b9fd:	1b          	aba
    b9fe:	16          	tab
    b9ff:	d7 6a       	stab	*0x6a
    ba01:	c4 0f       	andb	#0xf
    ba03:	ce db 82    	ldx	#0xdb82
    ba06:	18 ce 01 30 	ldy	#0x130
    ba0a:	bd ec 6f    	jsr	0xec6f
    ba0d:	7f 01 1a    	clr	0x11a
    ba10:	86 04       	ldaa	#0x4
    ba12:	b7 01 1c    	staa	0x11c
    ba15:	7e d5 2f    	jmp	0xd52f
    ba18:	96 6a       	ldaa	*0x6a
    ba1a:	84 20       	anda	#0x20
    ba1c:	d6 6a       	ldab	*0x6a
    ba1e:	c4 0f       	andb	#0xf
    ba20:	5a          	decb
    ba21:	2a da       	bpl	0x0xb9fd
    ba23:	5f          	clrb
    ba24:	20 d7       	bra	0x0xb9fd
    ba26:	81 19       	cmpa	#0x19
    ba28:	26 1c       	bne	0x0xba46
    ba2a:	d6 95       	ldab	*0x95
    ba2c:	5c          	incb
    ba2d:	c4 01       	andb	#0x1
    ba2f:	d7 95       	stab	*0x95
    ba31:	ce db 96    	ldx	#0xdb96
    ba34:	18 ce 01 36 	ldy	#0x136
    ba38:	bd ec 6f    	jsr	0xec6f
    ba3b:	7f 01 1a    	clr	0x11a
    ba3e:	86 04       	ldaa	#0x4
    ba40:	b7 01 1c    	staa	0x11c
    ba43:	7e d5 2f    	jmp	0xd52f
    ba46:	d6 69       	ldab	*0x69
    ba48:	5c          	incb
    ba49:	c4 01       	andb	#0x1
    ba4b:	d7 69       	stab	*0x69
    ba4d:	ce db 9e    	ldx	#0xdb9e
    ba50:	18 ce 01 3b 	ldy	#0x13b
    ba54:	bd ec 6f    	jsr	0xec6f
    ba57:	7f 01 1a    	clr	0x11a
    ba5a:	86 04       	ldaa	#0x4
    ba5c:	b7 01 1c    	staa	0x11c
    ba5f:	96 69       	ldaa	*0x69
    ba61:	c6 69       	ldab	#0x69
    ba63:	bd b0 fc    	jsr	0xb0fc
    ba66:	7e d5 2f    	jmp	0xd52f
    ba69:	7d 01 1c    	tst	0x11c
    ba6c:	2a 03       	bpl	0x0xba71
    ba6e:	7e db a6    	jmp	0xdba6
    ba71:	7d 01 1e    	tst	0x11e
    ba74:	26 08       	bne	0x0xba7e
    ba76:	7d 01 1c    	tst	0x11c
    ba79:	27 1f       	beq	0x0xba9a
    ba7b:	7e b2 70    	jmp	0xb270
    ba7e:	7d 01 1c    	tst	0x11c
    ba81:	27 24       	beq	0x0xbaa7
    ba83:	cc 01 20    	ldd	#0x120
    ba86:	fb 01 1e    	addb	0x11e
    ba89:	8f          	xgdx
    ba8a:	bd ea a8    	jsr	0xeaa8
    ba8d:	7a 01 1e    	dec	0x11e
    ba90:	7a 01 1c    	dec	0x11c
    ba93:	26 12       	bne	0x0xbaa7
    ba95:	bd eb 26    	jsr	0xeb26
    ba98:	20 0d       	bra	0x0xbaa7
    ba9a:	7d 01 1e    	tst	0x11e
    ba9d:	26 08       	bne	0x0xbaa7
    ba9f:	86 13       	ldaa	#0x13
    baa1:	b7 01 1e    	staa	0x11e
    baa4:	bd ea e4    	jsr	0xeae4
    baa7:	7d 01 7f    	tst	0x17f
    baaa:	27 1e       	beq	0x0xbaca
    baac:	2a 10       	bpl	0x0xbabe
    baae:	bd ec e8    	jsr	0xece8
    bab1:	4f          	clra
    bab2:	b7 01 7f    	staa	0x17f
    bab5:	b7 01 7e    	staa	0x17e
    bab8:	b7 01 1a    	staa	0x11a
    babb:	7e d5 2f    	jmp	0xd52f
    babe:	b6 01 1e    	ldaa	0x11e
    bac1:	81 19       	cmpa	#0x19
    bac3:	27 ec       	beq	0x0xbab1
    bac5:	bd ec e8    	jsr	0xece8
    bac8:	20 e7       	bra	0x0xbab1
    baca:	7d 01 1a    	tst	0x11a
    bacd:	26 03       	bne	0x0xbad2
    bacf:	7e d5 2f    	jmp	0xd52f
    bad2:	b6 01 1e    	ldaa	0x11e
    bad5:	81 13       	cmpa	#0x13
    bad7:	26 34       	bne	0x0xbb0d
    bad9:	b6 01 1a    	ldaa	0x11a
    badc:	2b 27       	bmi	0x0xbb05
    bade:	d6 6b       	ldab	*0x6b
    bae0:	5c          	incb
    bae1:	c1 03       	cmpb	#0x3
    bae3:	25 02       	bcs	0x0xbae7
    bae5:	c6 02       	ldab	#0x2
    bae7:	d7 6b       	stab	*0x6b
    bae9:	ce db f8    	ldx	#0xdbf8
    baec:	18 ce 01 30 	ldy	#0x130
    baf0:	bd ec 6f    	jsr	0xec6f
    baf3:	7f 01 1a    	clr	0x11a
    baf6:	86 04       	ldaa	#0x4
    baf8:	b7 01 1c    	staa	0x11c
    bafb:	96 6b       	ldaa	*0x6b
    bafd:	c6 6b       	ldab	#0x6b
    baff:	bd b0 fc    	jsr	0xb0fc
    bb02:	7e d5 2f    	jmp	0xd52f
    bb05:	d6 6b       	ldab	*0x6b
    bb07:	5a          	decb
    bb08:	2a dd       	bpl	0x0xbae7
    bb0a:	5f          	clrb
    bb0b:	20 da       	bra	0x0xbae7
    bb0d:	b6 01 1a    	ldaa	0x11a
    bb10:	2b 27       	bmi	0x0xbb39
    bb12:	d6 68       	ldab	*0x68
    bb14:	5c          	incb
    bb15:	c1 07       	cmpb	#0x7
    bb17:	23 02       	bls	0x0xbb1b
    bb19:	c6 07       	ldab	#0x7
    bb1b:	d7 68       	stab	*0x68
    bb1d:	ce dc 04    	ldx	#0xdc04
    bb20:	18 ce 01 36 	ldy	#0x136
    bb24:	bd ec 6f    	jsr	0xec6f
    bb27:	7f 01 1a    	clr	0x11a
    bb2a:	86 04       	ldaa	#0x4
    bb2c:	b7 01 1c    	staa	0x11c
    bb2f:	96 68       	ldaa	*0x68
    bb31:	c6 68       	ldab	#0x68
    bb33:	bd b0 fc    	jsr	0xb0fc
    bb36:	7e d5 2f    	jmp	0xd52f
    bb39:	d6 68       	ldab	*0x68
    bb3b:	5a          	decb
    bb3c:	2a dd       	bpl	0x0xbb1b
    bb3e:	5f          	clrb
    bb3f:	20 da       	bra	0x0xbb1b
    bb41:	7e d5 2f    	jmp	0xd52f
    bb44:	7d 01 1c    	tst	0x11c
    bb47:	2a 03       	bpl	0x0xbb4c
    bb49:	7e dc 24    	jmp	0xdc24
    bb4c:	7d 01 1e    	tst	0x11e
    bb4f:	26 08       	bne	0x0xbb59
    bb51:	7d 01 1c    	tst	0x11c
    bb54:	27 1f       	beq	0x0xbb75
    bb56:	7e b2 70    	jmp	0xb270
    bb59:	7d 01 1c    	tst	0x11c
    bb5c:	27 24       	beq	0x0xbb82
    bb5e:	cc 01 20    	ldd	#0x120
    bb61:	fb 01 1e    	addb	0x11e
    bb64:	8f          	xgdx
    bb65:	bd ea a8    	jsr	0xeaa8
    bb68:	7a 01 1e    	dec	0x11e
    bb6b:	7a 01 1c    	dec	0x11c
    bb6e:	26 12       	bne	0x0xbb82
    bb70:	bd eb 26    	jsr	0xeb26
    bb73:	20 0d       	bra	0x0xbb82
    bb75:	7d 01 1e    	tst	0x11e
    bb78:	26 08       	bne	0x0xbb82
    bb7a:	86 09       	ldaa	#0x9
    bb7c:	b7 01 1e    	staa	0x11e
    bb7f:	bd ea e4    	jsr	0xeae4
    bb82:	7d 01 7f    	tst	0x17f
    bb85:	27 10       	beq	0x0xbb97
    bb87:	bd ec a7    	jsr	0xeca7
    bb8a:	4f          	clra
    bb8b:	b7 01 7f    	staa	0x17f
    bb8e:	b7 01 7e    	staa	0x17e
    bb91:	b7 01 1a    	staa	0x11a
    bb94:	7e d5 2f    	jmp	0xd52f
    bb97:	7d 01 7e    	tst	0x17e
    bb9a:	27 0c       	beq	0x0xbba8
    bb9c:	bd ec 90    	jsr	0xec90
    bb9f:	7f 01 7e    	clr	0x17e
    bba2:	7f 01 1a    	clr	0x11a
    bba5:	7e d5 2f    	jmp	0xd52f
    bba8:	7d 01 1a    	tst	0x11a
    bbab:	26 03       	bne	0x0xbbb0
    bbad:	7e d5 2f    	jmp	0xd52f
    bbb0:	b6 01 1e    	ldaa	0x11e
    bbb3:	81 09       	cmpa	#0x9
    bbb5:	26 29       	bne	0x0xbbe0
    bbb7:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xbbc1
    bbbb:	7f 01 1a    	clr	0x11a
    bbbe:	7e d5 2f    	jmp	0xd52f
    bbc1:	ce 00 76    	ldx	#0x76
    bbc4:	bd bc 59    	jsr	0xbc59
    bbc7:	bd bc 8d    	jsr	0xbc8d
    bbca:	16          	tab
    bbcb:	ce e0 53    	ldx	#0xe053
    bbce:	18 ce 01 26 	ldy	#0x126
    bbd2:	bd ec 6f    	jsr	0xec6f
    bbd5:	86 04       	ldaa	#0x4
    bbd7:	b7 01 1c    	staa	0x11c
    bbda:	7f 01 1a    	clr	0x11a
    bbdd:	7e d5 2f    	jmp	0xd52f
    bbe0:	81 0e       	cmpa	#0xe
    bbe2:	26 29       	bne	0x0xbc0d
    bbe4:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xbbee
    bbe8:	7f 01 1a    	clr	0x11a
    bbeb:	7e d5 2f    	jmp	0xd52f
    bbee:	ce 00 77    	ldx	#0x77
    bbf1:	bd bc 59    	jsr	0xbc59
    bbf4:	bd bc 8d    	jsr	0xbc8d
    bbf7:	16          	tab
    bbf8:	ce e0 53    	ldx	#0xe053
    bbfb:	18 ce 01 2b 	ldy	#0x12b
    bbff:	bd ec 6f    	jsr	0xec6f
    bc02:	86 04       	ldaa	#0x4
    bc04:	b7 01 1c    	staa	0x11c
    bc07:	7f 01 1a    	clr	0x11a
    bc0a:	7e d5 2f    	jmp	0xd52f
    bc0d:	81 19       	cmpa	#0x19
    bc0f:	26 24       	bne	0x0xbc35
    bc11:	ce 00 78    	ldx	#0x78
    bc14:	bd bc 59    	jsr	0xbc59
    bc17:	bd bc d9    	jsr	0xbcd9
    bc1a:	ce 01 37    	ldx	#0x137
    bc1d:	7d 00 f6    	tst	0xf6
    bc20:	2a 05       	bpl	0x0xbc27
    bc22:	bd eb f8    	jsr	0xebf8
    bc25:	20 03       	bra	0x0xbc2a
    bc27:	bd eb bc    	jsr	0xebbc
    bc2a:	86 03       	ldaa	#0x3
    bc2c:	b7 01 1c    	staa	0x11c
    bc2f:	7f 01 1a    	clr	0x11a
    bc32:	7e d5 2f    	jmp	0xd52f
    bc35:	ce 00 79    	ldx	#0x79
    bc38:	bd bc 59    	jsr	0xbc59
    bc3b:	bd bc d9    	jsr	0xbcd9
    bc3e:	ce 01 3c    	ldx	#0x13c
    bc41:	7d 00 f6    	tst	0xf6
    bc44:	2a 05       	bpl	0x0xbc4b
    bc46:	bd eb f8    	jsr	0xebf8
    bc49:	20 03       	bra	0x0xbc4e
    bc4b:	bd eb bc    	jsr	0xebbc
    bc4e:	86 03       	ldaa	#0x3
    bc50:	b7 01 1c    	staa	0x11c
    bc53:	7f 01 1a    	clr	0x11a
    bc56:	7e d5 2f    	jmp	0xd52f
    bc59:	96 f6       	ldaa	*0xf6
    bc5b:	44          	lsra
    bc5c:	44          	lsra
    bc5d:	44          	lsra
    bc5e:	24 03       	bcc	0x0xbc63
    bc60:	a6 00       	ldaa	0x0,x
    bc62:	39          	rts
    bc63:	44          	lsra
    bc64:	24 06       	bcc	0x0xbc6c
    bc66:	c6 04       	ldab	#0x4
    bc68:	3a          	abx
    bc69:	a6 00       	ldaa	0x0,x
    bc6b:	39          	rts
    bc6c:	44          	lsra
    bc6d:	24 06       	bcc	0x0xbc75
    bc6f:	c6 08       	ldab	#0x8
    bc71:	3a          	abx
    bc72:	a6 00       	ldaa	0x0,x
    bc74:	39          	rts
    bc75:	44          	lsra
    bc76:	24 06       	bcc	0x0xbc7e
    bc78:	c6 0c       	ldab	#0xc
    bc7a:	3a          	abx
    bc7b:	a6 00       	ldaa	0x0,x
    bc7d:	39          	rts
    bc7e:	44          	lsra
    bc7f:	24 06       	bcc	0x0xbc87
    bc81:	c6 10       	ldab	#0x10
    bc83:	3a          	abx
    bc84:	a6 00       	ldaa	0x0,x
    bc86:	39          	rts
    bc87:	c6 14       	ldab	#0x14
    bc89:	3a          	abx
    bc8a:	a6 00       	ldaa	0x0,x
    bc8c:	39          	rts
    bc8d:	7d 01 1a    	tst	0x11a
    bc90:	2b 29       	bmi	0x0xbcbb
    bc92:	4c          	inca
    bc93:	7d 00 f6    	tst	0xf6
    bc96:	2a 14       	bpl	0x0xbcac
    bc98:	7d 00 8f    	tst	0x8f
    bc9b:	27 0f       	beq	0x0xbcac
    bc9d:	81 03       	cmpa	#0x3
    bc9f:	22 04       	bhi	0x0xbca5
    bca1:	86 04       	ldaa	#0x4
    bca3:	20 0d       	bra	0x0xbcb2
    bca5:	81 09       	cmpa	#0x9
    bca7:	26 03       	bne	0x0xbcac
    bca9:	4c          	inca
    bcaa:	20 06       	bra	0x0xbcb2
    bcac:	81 19       	cmpa	#0x19
    bcae:	25 02       	bcs	0x0xbcb2
    bcb0:	86 18       	ldaa	#0x18
    bcb2:	a7 00       	staa	0x0,x
    bcb4:	8f          	xgdx
    bcb5:	37          	pshb
    bcb6:	8f          	xgdx
    bcb7:	33          	pulb
    bcb8:	7e b0 fc    	jmp	0xb0fc
    bcbb:	4a          	deca
    bcbc:	2a 03       	bpl	0x0xbcc1
    bcbe:	4f          	clra
    bcbf:	20 f1       	bra	0x0xbcb2
    bcc1:	7d 00 f6    	tst	0xf6
    bcc4:	2a ec       	bpl	0x0xbcb2
    bcc6:	7d 00 8f    	tst	0x8f
    bcc9:	27 e7       	beq	0x0xbcb2
    bccb:	81 09       	cmpa	#0x9
    bccd:	26 03       	bne	0x0xbcd2
    bccf:	4a          	deca
    bcd0:	20 e0       	bra	0x0xbcb2
    bcd2:	81 03       	cmpa	#0x3
    bcd4:	22 dc       	bhi	0x0xbcb2
    bcd6:	4f          	clra
    bcd7:	20 d9       	bra	0x0xbcb2
    bcd9:	7d 01 1a    	tst	0x11a
    bcdc:	2b 12       	bmi	0x0xbcf0
    bcde:	4c          	inca
    bcdf:	84 7f       	anda	#0x7f
    bce1:	13 f6 10 02 	brclr	*0xf6, #0x10, 0x0xbce7
    bce5:	84 1f       	anda	#0x1f
    bce7:	a7 00       	staa	0x0,x
    bce9:	8f          	xgdx
    bcea:	37          	pshb
    bceb:	8f          	xgdx
    bcec:	33          	pulb
    bced:	7e b0 fc    	jmp	0xb0fc
    bcf0:	4a          	deca
    bcf1:	84 7f       	anda	#0x7f
    bcf3:	20 ec       	bra	0x0xbce1
    bcf5:	7d 01 1c    	tst	0x11c
    bcf8:	2a 03       	bpl	0x0xbcfd
    bcfa:	7e dd c1    	jmp	0xddc1
    bcfd:	7d 01 1e    	tst	0x11e
    bd00:	26 08       	bne	0x0xbd0a
    bd02:	7d 01 1c    	tst	0x11c
    bd05:	27 1f       	beq	0x0xbd26
    bd07:	7e b2 70    	jmp	0xb270
    bd0a:	7d 01 1c    	tst	0x11c
    bd0d:	27 24       	beq	0x0xbd33
    bd0f:	cc 01 20    	ldd	#0x120
    bd12:	fb 01 1e    	addb	0x11e
    bd15:	8f          	xgdx
    bd16:	bd ea a8    	jsr	0xeaa8
    bd19:	7a 01 1e    	dec	0x11e
    bd1c:	7a 01 1c    	dec	0x11c
    bd1f:	26 12       	bne	0x0xbd33
    bd21:	bd eb 26    	jsr	0xeb26
    bd24:	20 0d       	bra	0x0xbd33
    bd26:	7d 01 1e    	tst	0x11e
    bd29:	26 08       	bne	0x0xbd33
    bd2b:	86 19       	ldaa	#0x19
    bd2d:	b7 01 1e    	staa	0x11e
    bd30:	bd ea e4    	jsr	0xeae4
    bd33:	7d 01 7f    	tst	0x17f
    bd36:	27 10       	beq	0x0xbd48
    bd38:	bd ec a7    	jsr	0xeca7
    bd3b:	4f          	clra
    bd3c:	b7 01 7f    	staa	0x17f
    bd3f:	b7 01 7e    	staa	0x17e
    bd42:	b7 01 1a    	staa	0x11a
    bd45:	7e d5 2f    	jmp	0xd52f
    bd48:	7d 01 1a    	tst	0x11a
    bd4b:	26 03       	bne	0x0xbd50
    bd4d:	7e d5 2f    	jmp	0xd52f
    bd50:	b6 01 1e    	ldaa	0x11e
    bd53:	81 19       	cmpa	#0x19
    bd55:	26 34       	bne	0x0xbd8b
    bd57:	18 ce 01 36 	ldy	#0x136
    bd5b:	ce de 22    	ldx	#0xde22
    bd5e:	7d 01 1a    	tst	0x11a
    bd61:	2b 20       	bmi	0x0xbd83
    bd63:	d6 8e       	ldab	*0x8e
    bd65:	5c          	incb
    bd66:	c1 03       	cmpb	#0x3
    bd68:	25 02       	bcs	0x0xbd6c
    bd6a:	c6 02       	ldab	#0x2
    bd6c:	d7 8e       	stab	*0x8e
    bd6e:	bd ec 6f    	jsr	0xec6f
    bd71:	7f 01 1a    	clr	0x11a
    bd74:	86 04       	ldaa	#0x4
    bd76:	b7 01 1c    	staa	0x11c
    bd79:	96 8e       	ldaa	*0x8e
    bd7b:	c6 8e       	ldab	#0x8e
    bd7d:	bd b0 fc    	jsr	0xb0fc
    bd80:	7e d5 2f    	jmp	0xd52f
    bd83:	d6 8e       	ldab	*0x8e
    bd85:	5a          	decb
    bd86:	2a e4       	bpl	0x0xbd6c
    bd88:	5f          	clrb
    bd89:	20 e1       	bra	0x0xbd6c
    bd8b:	18 ce 01 3b 	ldy	#0x13b
    bd8f:	ce de 2e    	ldx	#0xde2e
    bd92:	7d 01 1a    	tst	0x11a
    bd95:	2b 20       	bmi	0x0xbdb7
    bd97:	d6 8f       	ldab	*0x8f
    bd99:	5c          	incb
    bd9a:	c1 02       	cmpb	#0x2
    bd9c:	25 02       	bcs	0x0xbda0
    bd9e:	c6 01       	ldab	#0x1
    bda0:	d7 8f       	stab	*0x8f
    bda2:	bd ec 6f    	jsr	0xec6f
    bda5:	7f 01 1a    	clr	0x11a
    bda8:	86 04       	ldaa	#0x4
    bdaa:	b7 01 1c    	staa	0x11c
    bdad:	96 8f       	ldaa	*0x8f
    bdaf:	c6 8f       	ldab	#0x8f
    bdb1:	bd b0 fc    	jsr	0xb0fc
    bdb4:	7e d5 2f    	jmp	0xd52f
    bdb7:	d6 8f       	ldab	*0x8f
    bdb9:	5a          	decb
    bdba:	2a e4       	bpl	0x0xbda0
    bdbc:	5f          	clrb
    bdbd:	20 e1       	bra	0x0xbda0
    bdbf:	7d 01 1c    	tst	0x11c
    bdc2:	2a 03       	bpl	0x0xbdc7
    bdc4:	7e de 36    	jmp	0xde36
    bdc7:	7d 01 1e    	tst	0x11e
    bdca:	26 08       	bne	0x0xbdd4
    bdcc:	7d 01 1c    	tst	0x11c
    bdcf:	27 1a       	beq	0x0xbdeb
    bdd1:	7e b2 70    	jmp	0xb270
    bdd4:	7d 01 1c    	tst	0x11c
    bdd7:	27 1a       	beq	0x0xbdf3
    bdd9:	cc 01 20    	ldd	#0x120
    bddc:	fb 01 1e    	addb	0x11e
    bddf:	8f          	xgdx
    bde0:	bd ea a8    	jsr	0xeaa8
    bde3:	7a 01 1e    	dec	0x11e
    bde6:	7a 01 1c    	dec	0x11c
    bde9:	26 08       	bne	0x0xbdf3
    bdeb:	86 11       	ldaa	#0x11
    bded:	b7 01 1e    	staa	0x11e
    bdf0:	bd ea e4    	jsr	0xeae4
    bdf3:	4f          	clra
    bdf4:	b7 01 7f    	staa	0x17f
    bdf7:	b7 01 7e    	staa	0x17e
    bdfa:	7d 01 1a    	tst	0x11a
    bdfd:	26 03       	bne	0x0xbe02
    bdff:	7e d5 2f    	jmp	0xd52f
    be02:	2b 2b       	bmi	0x0xbe2f
    be04:	b6 50 00    	ldaa	0x5000
    be07:	4c          	inca
    be08:	81 02       	cmpa	#0x2
    be0a:	25 0a       	bcs	0x0xbe16
    be0c:	81 05       	cmpa	#0x5
    be0e:	22 04       	bhi	0x0xbe14
    be10:	86 05       	ldaa	#0x5
    be12:	20 02       	bra	0x0xbe16
    be14:	86 06       	ldaa	#0x6
    be16:	b7 50 00    	staa	0x5000
    be19:	97 d1       	staa	*0xd1
    be1b:	bd 9f 0f    	jsr	0x9f0f
    be1e:	bd a5 ab    	jsr	0xa5ab
    be21:	86 80       	ldaa	#0x80
    be23:	b7 01 1c    	staa	0x11c
    be26:	7f 01 1a    	clr	0x11a
    be29:	14 fb 80    	bset	*0xfb, #0x80
    be2c:	7e d5 2f    	jmp	0xd52f
    be2f:	b6 50 00    	ldaa	0x5000
    be32:	81 06       	cmpa	#0x6
    be34:	26 04       	bne	0x0xbe3a
    be36:	86 05       	ldaa	#0x5
    be38:	20 dc       	bra	0x0xbe16
    be3a:	81 05       	cmpa	#0x5
    be3c:	26 04       	bne	0x0xbe42
    be3e:	86 01       	ldaa	#0x1
    be40:	20 d4       	bra	0x0xbe16
    be42:	4f          	clra
    be43:	20 d1       	bra	0x0xbe16
    be45:	7d 01 1c    	tst	0x11c
    be48:	2a 03       	bpl	0x0xbe4d
    be4a:	7e df 54    	jmp	0xdf54
    be4d:	7d 01 1e    	tst	0x11e
    be50:	26 08       	bne	0x0xbe5a
    be52:	7d 01 1c    	tst	0x11c
    be55:	27 1a       	beq	0x0xbe71
    be57:	7e b2 70    	jmp	0xb270
    be5a:	7d 01 1c    	tst	0x11c
    be5d:	27 1a       	beq	0x0xbe79
    be5f:	cc 01 20    	ldd	#0x120
    be62:	fb 01 1e    	addb	0x11e
    be65:	8f          	xgdx
    be66:	bd ea a8    	jsr	0xeaa8
    be69:	7a 01 1e    	dec	0x11e
    be6c:	7a 01 1c    	dec	0x11c
    be6f:	26 08       	bne	0x0xbe79
    be71:	86 1e       	ldaa	#0x1e
    be73:	b7 01 1e    	staa	0x11e
    be76:	bd ea e4    	jsr	0xeae4
    be79:	7f 01 7e    	clr	0x17e
    be7c:	7f 01 7f    	clr	0x17f
    be7f:	7d 01 1a    	tst	0x11a
    be82:	26 03       	bne	0x0xbe87
    be84:	7e d5 2f    	jmp	0xd52f
    be87:	b6 50 21    	ldaa	0x5021
    be8a:	7d 01 1a    	tst	0x11a
    be8d:	2b 0e       	bmi	0x0xbe9d
    be8f:	4c          	inca
    be90:	84 7f       	anda	#0x7f
    be92:	b7 50 21    	staa	0x5021
    be95:	ce 01 3c    	ldx	#0x13c
    be98:	bd eb bc    	jsr	0xebbc
    be9b:	20 03       	bra	0x0xbea0
    be9d:	4a          	deca
    be9e:	20 f0       	bra	0x0xbe90
    bea0:	7f 01 1a    	clr	0x11a
    bea3:	86 03       	ldaa	#0x3
    bea5:	b7 01 1c    	staa	0x11c
    bea8:	14 fb 80    	bset	*0xfb, #0x80
    beab:	7e d5 2f    	jmp	0xd52f
    beae:	7d 01 1c    	tst	0x11c
    beb1:	2a 03       	bpl	0x0xbeb6
    beb3:	7e df 97    	jmp	0xdf97
    beb6:	7d 01 1c    	tst	0x11c
    beb9:	27 03       	beq	0x0xbebe
    bebb:	7e b2 70    	jmp	0xb270
    bebe:	7d 01 1e    	tst	0x11e
    bec1:	26 08       	bne	0x0xbecb
    bec3:	86 14       	ldaa	#0x14
    bec5:	b7 01 1e    	staa	0x11e
    bec8:	bd ea e4    	jsr	0xeae4
    becb:	7e d5 2f    	jmp	0xd52f
    bece:	7d 01 1c    	tst	0x11c
    bed1:	2a 03       	bpl	0x0xbed6
    bed3:	7e df f2    	jmp	0xdff2
    bed6:	7d 01 1e    	tst	0x11e
    bed9:	26 08       	bne	0x0xbee3
    bedb:	7d 01 1c    	tst	0x11c
    bede:	27 1f       	beq	0x0xbeff
    bee0:	7e b2 70    	jmp	0xb270
    bee3:	7d 01 1c    	tst	0x11c
    bee6:	27 24       	beq	0x0xbf0c
    bee8:	cc 01 20    	ldd	#0x120
    beeb:	fb 01 1e    	addb	0x11e
    beee:	8f          	xgdx
    beef:	bd ea a8    	jsr	0xeaa8
    bef2:	7a 01 1e    	dec	0x11e
    bef5:	7a 01 1c    	dec	0x11c
    bef8:	26 12       	bne	0x0xbf0c
    befa:	bd eb 26    	jsr	0xeb26
    befd:	20 0d       	bra	0x0xbf0c
    beff:	7d 01 1e    	tst	0x11e
    bf02:	26 08       	bne	0x0xbf0c
    bf04:	86 03       	ldaa	#0x3
    bf06:	b7 01 1e    	staa	0x11e
    bf09:	bd ea e4    	jsr	0xeae4
    bf0c:	7d 01 7f    	tst	0x17f
    bf0f:	27 10       	beq	0x0xbf21
    bf11:	bd ec e8    	jsr	0xece8
    bf14:	4f          	clra
    bf15:	b7 01 7f    	staa	0x17f
    bf18:	b7 01 7e    	staa	0x17e
    bf1b:	b7 01 1a    	staa	0x11a
    bf1e:	7e d5 2f    	jmp	0xd52f
    bf21:	7d 01 7e    	tst	0x17e
    bf24:	27 0c       	beq	0x0xbf32
    bf26:	bd ec 90    	jsr	0xec90
    bf29:	7f 01 7e    	clr	0x17e
    bf2c:	7f 01 1a    	clr	0x11a
    bf2f:	7e d5 2f    	jmp	0xd52f
    bf32:	7d 01 1a    	tst	0x11a
    bf35:	26 03       	bne	0x0xbf3a
    bf37:	7e d5 2f    	jmp	0xd52f
    bf3a:	ce e0 53    	ldx	#0xe053
    bf3d:	b6 01 1e    	ldaa	0x11e
    bf40:	81 03       	cmpa	#0x3
    bf42:	26 1a       	bne	0x0xbf5e
    bf44:	18 ce 01 20 	ldy	#0x120
    bf48:	d6 60       	ldab	*0x60
    bf4a:	bd c0 2c    	jsr	0xc02c
    bf4d:	d7 60       	stab	*0x60
    bf4f:	bd ec 6f    	jsr	0xec6f
    bf52:	96 60       	ldaa	*0x60
    bf54:	c6 60       	ldab	#0x60
    bf56:	bd b0 fc    	jsr	0xb0fc
    bf59:	86 04       	ldaa	#0x4
    bf5b:	7e c0 23    	jmp	0xc023
    bf5e:	81 09       	cmpa	#0x9
    bf60:	26 1a       	bne	0x0xbf7c
    bf62:	18 ce 01 26 	ldy	#0x126
    bf66:	d6 61       	ldab	*0x61
    bf68:	bd c0 2c    	jsr	0xc02c
    bf6b:	d7 61       	stab	*0x61
    bf6d:	bd ec 6f    	jsr	0xec6f
    bf70:	96 61       	ldaa	*0x61
    bf72:	c6 61       	ldab	#0x61
    bf74:	bd b0 fc    	jsr	0xb0fc
    bf77:	86 04       	ldaa	#0x4
    bf79:	7e c0 23    	jmp	0xc023
    bf7c:	81 0e       	cmpa	#0xe
    bf7e:	26 1a       	bne	0x0xbf9a
    bf80:	18 ce 01 2b 	ldy	#0x12b
    bf84:	d6 62       	ldab	*0x62
    bf86:	bd c0 2c    	jsr	0xc02c
    bf89:	d7 62       	stab	*0x62
    bf8b:	bd ec 6f    	jsr	0xec6f
    bf8e:	96 62       	ldaa	*0x62
    bf90:	c6 62       	ldab	#0x62
    bf92:	bd b0 fc    	jsr	0xb0fc
    bf95:	86 04       	ldaa	#0x4
    bf97:	7e c0 23    	jmp	0xc023
    bf9a:	81 13       	cmpa	#0x13
    bf9c:	26 2b       	bne	0x0xbfc9
    bf9e:	b6 01 1a    	ldaa	0x11a
    bfa1:	2b 1f       	bmi	0x0xbfc2
    bfa3:	96 63       	ldaa	*0x63
    bfa5:	4c          	inca
    bfa6:	84 7f       	anda	#0x7f
    bfa8:	97 63       	staa	*0x63
    bfaa:	ce 01 31    	ldx	#0x131
    bfad:	bd eb bc    	jsr	0xebbc
    bfb0:	7f 01 1a    	clr	0x11a
    bfb3:	86 03       	ldaa	#0x3
    bfb5:	b7 01 1c    	staa	0x11c
    bfb8:	96 63       	ldaa	*0x63
    bfba:	c6 63       	ldab	#0x63
    bfbc:	bd b0 fc    	jsr	0xb0fc
    bfbf:	7e d5 2f    	jmp	0xd52f
    bfc2:	96 63       	ldaa	*0x63
    bfc4:	4a          	deca
    bfc5:	84 7f       	anda	#0x7f
    bfc7:	20 df       	bra	0x0xbfa8
    bfc9:	81 19       	cmpa	#0x19
    bfcb:	26 2b       	bne	0x0xbff8
    bfcd:	b6 01 1a    	ldaa	0x11a
    bfd0:	2b 1f       	bmi	0x0xbff1
    bfd2:	96 64       	ldaa	*0x64
    bfd4:	4c          	inca
    bfd5:	84 7f       	anda	#0x7f
    bfd7:	97 64       	staa	*0x64
    bfd9:	ce 01 37    	ldx	#0x137
    bfdc:	bd eb bc    	jsr	0xebbc
    bfdf:	7f 01 1a    	clr	0x11a
    bfe2:	86 03       	ldaa	#0x3
    bfe4:	b7 01 1c    	staa	0x11c
    bfe7:	96 64       	ldaa	*0x64
    bfe9:	c6 64       	ldab	#0x64
    bfeb:	bd b0 fc    	jsr	0xb0fc
    bfee:	7e d5 2f    	jmp	0xd52f
    bff1:	96 64       	ldaa	*0x64
    bff3:	4a          	deca
    bff4:	84 7f       	anda	#0x7f
    bff6:	20 df       	bra	0x0xbfd7
    bff8:	7d 01 1a    	tst	0x11a
    bffb:	2b 1f       	bmi	0x0xc01c
    bffd:	96 65       	ldaa	*0x65
    bfff:	4c          	inca
    c000:	84 7f       	anda	#0x7f
    c002:	97 65       	staa	*0x65
    c004:	ce 01 3c    	ldx	#0x13c
    c007:	bd eb bc    	jsr	0xebbc
    c00a:	7f 01 1a    	clr	0x11a
    c00d:	86 03       	ldaa	#0x3
    c00f:	b7 01 1c    	staa	0x11c
    c012:	96 65       	ldaa	*0x65
    c014:	c6 65       	ldab	#0x65
    c016:	bd b0 fc    	jsr	0xb0fc
    c019:	7e d5 2f    	jmp	0xd52f
    c01c:	96 65       	ldaa	*0x65
    c01e:	4a          	deca
    c01f:	84 7f       	anda	#0x7f
    c021:	20 df       	bra	0x0xc002
    c023:	b7 01 1c    	staa	0x11c
    c026:	7f 01 1a    	clr	0x11a
    c029:	7e d5 2f    	jmp	0xd52f
    c02c:	7d 01 1a    	tst	0x11a
    c02f:	2b 15       	bmi	0x0xc046
    c031:	5c          	incb
    c032:	c1 09       	cmpb	#0x9
    c034:	26 02       	bne	0x0xc038
    c036:	5c          	incb
    c037:	39          	rts
    c038:	c1 0d       	cmpb	#0xd
    c03a:	26 03       	bne	0x0xc03f
    c03c:	5c          	incb
    c03d:	5c          	incb
    c03e:	39          	rts
    c03f:	c1 16       	cmpb	#0x16
    c041:	25 02       	bcs	0x0xc045
    c043:	c6 15       	ldab	#0x15
    c045:	39          	rts
    c046:	5a          	decb
    c047:	2a 02       	bpl	0x0xc04b
    c049:	5f          	clrb
    c04a:	39          	rts
    c04b:	c1 0e       	cmpb	#0xe
    c04d:	26 03       	bne	0x0xc052
    c04f:	5a          	decb
    c050:	5a          	decb
    c051:	39          	rts
    c052:	c1 09       	cmpb	#0x9
    c054:	26 ef       	bne	0x0xc045
    c056:	5a          	decb
    c057:	39          	rts
    c058:	7d 01 1c    	tst	0x11c
    c05b:	2a 03       	bpl	0x0xc060
    c05d:	7e e0 b7    	jmp	0xe0b7
    c060:	7d 01 1e    	tst	0x11e
    c063:	26 08       	bne	0x0xc06d
    c065:	7d 01 1c    	tst	0x11c
    c068:	27 1f       	beq	0x0xc089
    c06a:	7e b2 70    	jmp	0xb270
    c06d:	7d 01 1c    	tst	0x11c
    c070:	27 24       	beq	0x0xc096
    c072:	cc 01 20    	ldd	#0x120
    c075:	fb 01 1e    	addb	0x11e
    c078:	8f          	xgdx
    c079:	bd ea a8    	jsr	0xeaa8
    c07c:	7a 01 1e    	dec	0x11e
    c07f:	7a 01 1c    	dec	0x11c
    c082:	26 12       	bne	0x0xc096
    c084:	bd eb 26    	jsr	0xeb26
    c087:	20 0d       	bra	0x0xc096
    c089:	7d 01 1e    	tst	0x11e
    c08c:	26 08       	bne	0x0xc096
    c08e:	86 13       	ldaa	#0x13
    c090:	b7 01 1e    	staa	0x11e
    c093:	bd ea e4    	jsr	0xeae4
    c096:	7d 01 7f    	tst	0x17f
    c099:	27 10       	beq	0x0xc0ab
    c09b:	bd ec e8    	jsr	0xece8
    c09e:	4f          	clra
    c09f:	b7 01 7f    	staa	0x17f
    c0a2:	b7 01 7e    	staa	0x17e
    c0a5:	b7 01 1a    	staa	0x11a
    c0a8:	7e d5 2f    	jmp	0xd52f
    c0ab:	7d 01 1a    	tst	0x11a
    c0ae:	26 03       	bne	0x0xc0b3
    c0b0:	7e d5 2f    	jmp	0xd52f
    c0b3:	b6 01 1e    	ldaa	0x11e
    c0b6:	81 13       	cmpa	#0x13
    c0b8:	26 2b       	bne	0x0xc0e5
    c0ba:	b6 01 1a    	ldaa	0x11a
    c0bd:	2b 1f       	bmi	0x0xc0de
    c0bf:	96 54       	ldaa	*0x54
    c0c1:	4c          	inca
    c0c2:	84 7f       	anda	#0x7f
    c0c4:	97 54       	staa	*0x54
    c0c6:	ce 01 31    	ldx	#0x131
    c0c9:	bd eb bc    	jsr	0xebbc
    c0cc:	7f 01 1a    	clr	0x11a
    c0cf:	86 03       	ldaa	#0x3
    c0d1:	b7 01 1c    	staa	0x11c
    c0d4:	96 54       	ldaa	*0x54
    c0d6:	c6 54       	ldab	#0x54
    c0d8:	bd b0 fc    	jsr	0xb0fc
    c0db:	7e d5 2f    	jmp	0xd52f
    c0de:	96 54       	ldaa	*0x54
    c0e0:	4a          	deca
    c0e1:	84 7f       	anda	#0x7f
    c0e3:	20 df       	bra	0x0xc0c4
    c0e5:	81 19       	cmpa	#0x19
    c0e7:	26 2b       	bne	0x0xc114
    c0e9:	b6 01 1a    	ldaa	0x11a
    c0ec:	2b 1f       	bmi	0x0xc10d
    c0ee:	96 59       	ldaa	*0x59
    c0f0:	4c          	inca
    c0f1:	84 7f       	anda	#0x7f
    c0f3:	97 59       	staa	*0x59
    c0f5:	ce 01 37    	ldx	#0x137
    c0f8:	bd eb bc    	jsr	0xebbc
    c0fb:	7f 01 1a    	clr	0x11a
    c0fe:	86 03       	ldaa	#0x3
    c100:	b7 01 1c    	staa	0x11c
    c103:	96 59       	ldaa	*0x59
    c105:	c6 59       	ldab	#0x59
    c107:	bd b0 fc    	jsr	0xb0fc
    c10a:	7e d5 2f    	jmp	0xd52f
    c10d:	96 59       	ldaa	*0x59
    c10f:	4a          	deca
    c110:	84 7f       	anda	#0x7f
    c112:	20 df       	bra	0x0xc0f3
    c114:	b6 01 1a    	ldaa	0x11a
    c117:	2b 1f       	bmi	0x0xc138
    c119:	96 5e       	ldaa	*0x5e
    c11b:	4c          	inca
    c11c:	84 7f       	anda	#0x7f
    c11e:	97 5e       	staa	*0x5e
    c120:	ce 01 3c    	ldx	#0x13c
    c123:	bd eb bc    	jsr	0xebbc
    c126:	7f 01 1a    	clr	0x11a
    c129:	86 03       	ldaa	#0x3
    c12b:	b7 01 1c    	staa	0x11c
    c12e:	96 5e       	ldaa	*0x5e
    c130:	c6 5e       	ldab	#0x5e
    c132:	bd b0 fc    	jsr	0xb0fc
    c135:	7e d5 2f    	jmp	0xd52f
    c138:	96 5e       	ldaa	*0x5e
    c13a:	4a          	deca
    c13b:	84 7f       	anda	#0x7f
    c13d:	20 df       	bra	0x0xc11e
    c13f:	7d 01 1c    	tst	0x11c
    c142:	2a 03       	bpl	0x0xc147
    c144:	7e e1 0a    	jmp	0xe10a
    c147:	7d 01 1e    	tst	0x11e
    c14a:	26 08       	bne	0x0xc154
    c14c:	7d 01 1c    	tst	0x11c
    c14f:	27 1f       	beq	0x0xc170
    c151:	7e b2 70    	jmp	0xb270
    c154:	7d 01 1c    	tst	0x11c
    c157:	27 24       	beq	0x0xc17d
    c159:	cc 01 20    	ldd	#0x120
    c15c:	fb 01 1e    	addb	0x11e
    c15f:	8f          	xgdx
    c160:	bd ea a8    	jsr	0xeaa8
    c163:	7a 01 1e    	dec	0x11e
    c166:	7a 01 1c    	dec	0x11c
    c169:	26 12       	bne	0x0xc17d
    c16b:	bd eb 26    	jsr	0xeb26
    c16e:	20 0d       	bra	0x0xc17d
    c170:	7d 01 1e    	tst	0x11e
    c173:	26 08       	bne	0x0xc17d
    c175:	86 13       	ldaa	#0x13
    c177:	b7 01 1e    	staa	0x11e
    c17a:	bd ea e4    	jsr	0xeae4
    c17d:	7d 01 7f    	tst	0x17f
    c180:	27 10       	beq	0x0xc192
    c182:	bd ec e8    	jsr	0xece8
    c185:	4f          	clra
    c186:	b7 01 7f    	staa	0x17f
    c189:	b7 01 7e    	staa	0x17e
    c18c:	b7 01 1a    	staa	0x11a
    c18f:	7e d5 2f    	jmp	0xd52f
    c192:	7d 01 1a    	tst	0x11a
    c195:	26 03       	bne	0x0xc19a
    c197:	7e d5 2f    	jmp	0xd52f
    c19a:	b6 01 1e    	ldaa	0x11e
    c19d:	81 13       	cmpa	#0x13
    c19f:	26 2b       	bne	0x0xc1cc
    c1a1:	b6 01 1a    	ldaa	0x11a
    c1a4:	2b 1f       	bmi	0x0xc1c5
    c1a6:	96 90       	ldaa	*0x90
    c1a8:	4c          	inca
    c1a9:	84 7f       	anda	#0x7f
    c1ab:	97 90       	staa	*0x90
    c1ad:	ce 01 31    	ldx	#0x131
    c1b0:	bd eb bc    	jsr	0xebbc
    c1b3:	7f 01 1a    	clr	0x11a
    c1b6:	86 03       	ldaa	#0x3
    c1b8:	b7 01 1c    	staa	0x11c
    c1bb:	96 90       	ldaa	*0x90
    c1bd:	c6 90       	ldab	#0x90
    c1bf:	bd b0 fc    	jsr	0xb0fc
    c1c2:	7e d5 2f    	jmp	0xd52f
    c1c5:	96 90       	ldaa	*0x90
    c1c7:	4a          	deca
    c1c8:	84 7f       	anda	#0x7f
    c1ca:	20 df       	bra	0x0xc1ab
    c1cc:	81 19       	cmpa	#0x19
    c1ce:	26 2b       	bne	0x0xc1fb
    c1d0:	b6 01 1a    	ldaa	0x11a
    c1d3:	2b 1f       	bmi	0x0xc1f4
    c1d5:	96 91       	ldaa	*0x91
    c1d7:	4c          	inca
    c1d8:	84 7f       	anda	#0x7f
    c1da:	97 91       	staa	*0x91
    c1dc:	ce 01 37    	ldx	#0x137
    c1df:	bd eb bc    	jsr	0xebbc
    c1e2:	7f 01 1a    	clr	0x11a
    c1e5:	86 03       	ldaa	#0x3
    c1e7:	b7 01 1c    	staa	0x11c
    c1ea:	96 91       	ldaa	*0x91
    c1ec:	c6 91       	ldab	#0x91
    c1ee:	bd b0 fc    	jsr	0xb0fc
    c1f1:	7e d5 2f    	jmp	0xd52f
    c1f4:	96 91       	ldaa	*0x91
    c1f6:	4a          	deca
    c1f7:	84 7f       	anda	#0x7f
    c1f9:	20 df       	bra	0x0xc1da
    c1fb:	b6 01 1a    	ldaa	0x11a
    c1fe:	2b 1f       	bmi	0x0xc21f
    c200:	96 92       	ldaa	*0x92
    c202:	4c          	inca
    c203:	84 7f       	anda	#0x7f
    c205:	97 92       	staa	*0x92
    c207:	ce 01 3c    	ldx	#0x13c
    c20a:	bd eb bc    	jsr	0xebbc
    c20d:	7f 01 1a    	clr	0x11a
    c210:	86 03       	ldaa	#0x3
    c212:	b7 01 1c    	staa	0x11c
    c215:	96 92       	ldaa	*0x92
    c217:	c6 92       	ldab	#0x92
    c219:	bd b0 fc    	jsr	0xb0fc
    c21c:	7e d5 2f    	jmp	0xd52f
    c21f:	96 92       	ldaa	*0x92
    c221:	4a          	deca
    c222:	84 7f       	anda	#0x7f
    c224:	20 df       	bra	0x0xc205
    c226:	7d 01 1c    	tst	0x11c
    c229:	2a 03       	bpl	0x0xc22e
    c22b:	7e e1 a6    	jmp	0xe1a6
    c22e:	7d 01 1e    	tst	0x11e
    c231:	26 08       	bne	0x0xc23b
    c233:	7d 01 1c    	tst	0x11c
    c236:	27 1f       	beq	0x0xc257
    c238:	7e b2 70    	jmp	0xb270
    c23b:	7d 01 1c    	tst	0x11c
    c23e:	27 24       	beq	0x0xc264
    c240:	cc 01 20    	ldd	#0x120
    c243:	fb 01 1e    	addb	0x11e
    c246:	8f          	xgdx
    c247:	bd ea a8    	jsr	0xeaa8
    c24a:	7a 01 1e    	dec	0x11e
    c24d:	7a 01 1c    	dec	0x11c
    c250:	26 12       	bne	0x0xc264
    c252:	bd eb 26    	jsr	0xeb26
    c255:	20 0d       	bra	0x0xc264
    c257:	7d 01 1e    	tst	0x11e
    c25a:	26 08       	bne	0x0xc264
    c25c:	86 13       	ldaa	#0x13
    c25e:	b7 01 1e    	staa	0x11e
    c261:	bd ea e4    	jsr	0xeae4
    c264:	7d 01 7f    	tst	0x17f
    c267:	27 1e       	beq	0x0xc287
    c269:	2a 10       	bpl	0x0xc27b
    c26b:	bd ec e8    	jsr	0xece8
    c26e:	4f          	clra
    c26f:	b7 01 7f    	staa	0x17f
    c272:	b7 01 7e    	staa	0x17e
    c275:	b7 01 1a    	staa	0x11a
    c278:	7e d5 2f    	jmp	0xd52f
    c27b:	b6 01 1e    	ldaa	0x11e
    c27e:	81 19       	cmpa	#0x19
    c280:	27 ec       	beq	0x0xc26e
    c282:	bd ec e8    	jsr	0xece8
    c285:	20 e7       	bra	0x0xc26e
    c287:	7d 01 1a    	tst	0x11a
    c28a:	26 03       	bne	0x0xc28f
    c28c:	7e d5 2f    	jmp	0xd52f
    c28f:	b6 01 1e    	ldaa	0x11e
    c292:	81 13       	cmpa	#0x13
    c294:	26 34       	bne	0x0xc2ca
    c296:	b6 01 1a    	ldaa	0x11a
    c299:	2b 27       	bmi	0x0xc2c2
    c29b:	d6 4b       	ldab	*0x4b
    c29d:	5c          	incb
    c29e:	c1 06       	cmpb	#0x6
    c2a0:	23 02       	bls	0x0xc2a4
    c2a2:	c6 06       	ldab	#0x6
    c2a4:	d7 4b       	stab	*0x4b
    c2a6:	ce e2 09    	ldx	#0xe209
    c2a9:	18 ce 01 30 	ldy	#0x130
    c2ad:	bd ec 6f    	jsr	0xec6f
    c2b0:	7f 01 1a    	clr	0x11a
    c2b3:	86 04       	ldaa	#0x4
    c2b5:	b7 01 1c    	staa	0x11c
    c2b8:	96 4b       	ldaa	*0x4b
    c2ba:	c6 4b       	ldab	#0x4b
    c2bc:	bd b0 fc    	jsr	0xb0fc
    c2bf:	7e d5 2f    	jmp	0xd52f
    c2c2:	d6 4b       	ldab	*0x4b
    c2c4:	5a          	decb
    c2c5:	2a dd       	bpl	0x0xc2a4
    c2c7:	5f          	clrb
    c2c8:	20 da       	bra	0x0xc2a4
    c2ca:	81 19       	cmpa	#0x19
    c2cc:	26 34       	bne	0x0xc302
    c2ce:	b6 01 1a    	ldaa	0x11a
    c2d1:	2b 27       	bmi	0x0xc2fa
    c2d3:	d6 66       	ldab	*0x66
    c2d5:	5c          	incb
    c2d6:	c1 03       	cmpb	#0x3
    c2d8:	23 02       	bls	0x0xc2dc
    c2da:	c6 03       	ldab	#0x3
    c2dc:	d7 66       	stab	*0x66
    c2de:	ce e2 25    	ldx	#0xe225
    c2e1:	18 ce 01 36 	ldy	#0x136
    c2e5:	bd ec 6f    	jsr	0xec6f
    c2e8:	7f 01 1a    	clr	0x11a
    c2eb:	86 04       	ldaa	#0x4
    c2ed:	b7 01 1c    	staa	0x11c
    c2f0:	96 66       	ldaa	*0x66
    c2f2:	c6 66       	ldab	#0x66
    c2f4:	bd b0 fc    	jsr	0xb0fc
    c2f7:	7e d5 2f    	jmp	0xd52f
    c2fa:	d6 66       	ldab	*0x66
    c2fc:	5a          	decb
    c2fd:	2a dd       	bpl	0x0xc2dc
    c2ff:	5f          	clrb
    c300:	20 da       	bra	0x0xc2dc
    c302:	4f          	clra
    c303:	b7 01 7e    	staa	0x17e
    c306:	b7 01 7f    	staa	0x17f
    c309:	b7 01 1a    	staa	0x11a
    c30c:	7e d5 2f    	jmp	0xd52f
    c30f:	7d 01 1c    	tst	0x11c
    c312:	2a 03       	bpl	0x0xc317
    c314:	7e e2 35    	jmp	0xe235
    c317:	7d 01 1e    	tst	0x11e
    c31a:	26 08       	bne	0x0xc324
    c31c:	7d 01 1c    	tst	0x11c
    c31f:	27 1f       	beq	0x0xc340
    c321:	7e b2 70    	jmp	0xb270
    c324:	7d 01 1c    	tst	0x11c
    c327:	27 2c       	beq	0x0xc355
    c329:	cc 01 20    	ldd	#0x120
    c32c:	fb 01 1e    	addb	0x11e
    c32f:	8f          	xgdx
    c330:	bd ea a8    	jsr	0xeaa8
    c333:	7a 01 1e    	dec	0x11e
    c336:	7a 01 1c    	dec	0x11c
    c339:	26 1a       	bne	0x0xc355
    c33b:	bd eb 59    	jsr	0xeb59
    c33e:	20 15       	bra	0x0xc355
    c340:	7d 01 1e    	tst	0x11e
    c343:	26 10       	bne	0x0xc355
    c345:	86 12       	ldaa	#0x12
    c347:	d6 4b       	ldab	*0x4b
    c349:	c1 06       	cmpb	#0x6
    c34b:	26 02       	bne	0x0xc34f
    c34d:	86 16       	ldaa	#0x16
    c34f:	b7 01 1e    	staa	0x11e
    c352:	bd ea e4    	jsr	0xeae4
    c355:	7d 01 7f    	tst	0x17f
    c358:	27 10       	beq	0x0xc36a
    c35a:	bd ed 5a    	jsr	0xed5a
    c35d:	4f          	clra
    c35e:	b7 01 7f    	staa	0x17f
    c361:	b7 01 7e    	staa	0x17e
    c364:	b7 01 1a    	staa	0x11a
    c367:	7e d5 2f    	jmp	0xd52f
    c36a:	7d 01 1a    	tst	0x11a
    c36d:	26 03       	bne	0x0xc372
    c36f:	7e d5 2f    	jmp	0xd52f
    c372:	b6 01 1e    	ldaa	0x11e
    c375:	81 12       	cmpa	#0x12
    c377:	26 2b       	bne	0x0xc3a4
    c379:	b6 01 1a    	ldaa	0x11a
    c37c:	2b 1f       	bmi	0x0xc39d
    c37e:	96 6e       	ldaa	*0x6e
    c380:	4c          	inca
    c381:	84 7f       	anda	#0x7f
    c383:	97 6e       	staa	*0x6e
    c385:	ce 01 30    	ldx	#0x130
    c388:	bd eb bc    	jsr	0xebbc
    c38b:	7f 01 1a    	clr	0x11a
    c38e:	86 03       	ldaa	#0x3
    c390:	b7 01 1c    	staa	0x11c
    c393:	96 6e       	ldaa	*0x6e
    c395:	c6 6e       	ldab	#0x6e
    c397:	bd b0 fc    	jsr	0xb0fc
    c39a:	7e d5 2f    	jmp	0xd52f
    c39d:	96 6e       	ldaa	*0x6e
    c39f:	4a          	deca
    c3a0:	84 7f       	anda	#0x7f
    c3a2:	20 df       	bra	0x0xc383
    c3a4:	81 16       	cmpa	#0x16
    c3a6:	26 2b       	bne	0x0xc3d3
    c3a8:	b6 01 1a    	ldaa	0x11a
    c3ab:	2b 1f       	bmi	0x0xc3cc
    c3ad:	96 48       	ldaa	*0x48
    c3af:	4c          	inca
    c3b0:	84 7f       	anda	#0x7f
    c3b2:	97 48       	staa	*0x48
    c3b4:	ce 01 34    	ldx	#0x134
    c3b7:	bd eb bc    	jsr	0xebbc
    c3ba:	7f 01 1a    	clr	0x11a
    c3bd:	86 03       	ldaa	#0x3
    c3bf:	b7 01 1c    	staa	0x11c
    c3c2:	96 48       	ldaa	*0x48
    c3c4:	c6 48       	ldab	#0x48
    c3c6:	bd b0 fc    	jsr	0xb0fc
    c3c9:	7e d5 2f    	jmp	0xd52f
    c3cc:	96 48       	ldaa	*0x48
    c3ce:	4a          	deca
    c3cf:	84 7f       	anda	#0x7f
    c3d1:	20 df       	bra	0x0xc3b2
    c3d3:	81 1a       	cmpa	#0x1a
    c3d5:	26 2b       	bne	0x0xc402
    c3d7:	b6 01 1a    	ldaa	0x11a
    c3da:	2b 1f       	bmi	0x0xc3fb
    c3dc:	96 6f       	ldaa	*0x6f
    c3de:	4c          	inca
    c3df:	84 7f       	anda	#0x7f
    c3e1:	97 6f       	staa	*0x6f
    c3e3:	ce 01 38    	ldx	#0x138
    c3e6:	bd eb bc    	jsr	0xebbc
    c3e9:	7f 01 1a    	clr	0x11a
    c3ec:	86 03       	ldaa	#0x3
    c3ee:	b7 01 1c    	staa	0x11c
    c3f1:	96 6f       	ldaa	*0x6f
    c3f3:	c6 6f       	ldab	#0x6f
    c3f5:	bd b0 fc    	jsr	0xb0fc
    c3f8:	7e d5 2f    	jmp	0xd52f
    c3fb:	96 6f       	ldaa	*0x6f
    c3fd:	4a          	deca
    c3fe:	84 7f       	anda	#0x7f
    c400:	20 df       	bra	0x0xc3e1
    c402:	b6 01 1a    	ldaa	0x11a
    c405:	2b 21       	bmi	0x0xc428
    c407:	96 70       	ldaa	*0x70
    c409:	4c          	inca
    c40a:	2a 02       	bpl	0x0xc40e
    c40c:	86 7f       	ldaa	#0x7f
    c40e:	97 70       	staa	*0x70
    c410:	ce 01 3c    	ldx	#0x13c
    c413:	bd ec 83    	jsr	0xec83
    c416:	7f 01 1a    	clr	0x11a
    c419:	86 03       	ldaa	#0x3
    c41b:	b7 01 1c    	staa	0x11c
    c41e:	96 70       	ldaa	*0x70
    c420:	c6 70       	ldab	#0x70
    c422:	bd b0 fc    	jsr	0xb0fc
    c425:	7e d5 2f    	jmp	0xd52f
    c428:	96 70       	ldaa	*0x70
    c42a:	4a          	deca
    c42b:	2a e1       	bpl	0x0xc40e
    c42d:	4f          	clra
    c42e:	20 de       	bra	0x0xc40e
    c430:	7d 01 1c    	tst	0x11c
    c433:	2a 03       	bpl	0x0xc438
    c435:	7e e2 b6    	jmp	0xe2b6
    c438:	7d 01 1e    	tst	0x11e
    c43b:	26 08       	bne	0x0xc445
    c43d:	7d 01 1c    	tst	0x11c
    c440:	27 1f       	beq	0x0xc461
    c442:	7e b2 70    	jmp	0xb270
    c445:	7d 01 1c    	tst	0x11c
    c448:	27 24       	beq	0x0xc46e
    c44a:	cc 01 20    	ldd	#0x120
    c44d:	fb 01 1e    	addb	0x11e
    c450:	8f          	xgdx
    c451:	bd ea a8    	jsr	0xeaa8
    c454:	7a 01 1e    	dec	0x11e
    c457:	7a 01 1c    	dec	0x11c
    c45a:	26 12       	bne	0x0xc46e
    c45c:	bd eb 26    	jsr	0xeb26
    c45f:	20 0d       	bra	0x0xc46e
    c461:	7d 01 1e    	tst	0x11e
    c464:	26 08       	bne	0x0xc46e
    c466:	86 13       	ldaa	#0x13
    c468:	b7 01 1e    	staa	0x11e
    c46b:	bd ea e4    	jsr	0xeae4
    c46e:	7d 01 7f    	tst	0x17f
    c471:	27 1e       	beq	0x0xc491
    c473:	2b 17       	bmi	0x0xc48c
    c475:	b6 01 1e    	ldaa	0x11e
    c478:	81 19       	cmpa	#0x19
    c47a:	27 03       	beq	0x0xc47f
    c47c:	bd ec e8    	jsr	0xece8
    c47f:	4f          	clra
    c480:	b7 01 7f    	staa	0x17f
    c483:	b7 01 7e    	staa	0x17e
    c486:	b7 01 1a    	staa	0x11a
    c489:	7e d5 2f    	jmp	0xd52f
    c48c:	bd ec e8    	jsr	0xece8
    c48f:	20 ee       	bra	0x0xc47f
    c491:	7d 01 1a    	tst	0x11a
    c494:	26 03       	bne	0x0xc499
    c496:	7e d5 2f    	jmp	0xd52f
    c499:	b6 01 1e    	ldaa	0x11e
    c49c:	81 13       	cmpa	#0x13
    c49e:	26 30       	bne	0x0xc4d0
    c4a0:	7d 01 1a    	tst	0x11a
    c4a3:	2b 23       	bmi	0x0xc4c8
    c4a5:	d6 74       	ldab	*0x74
    c4a7:	5c          	incb
    c4a8:	c1 05       	cmpb	#0x5
    c4aa:	25 02       	bcs	0x0xc4ae
    c4ac:	c6 04       	ldab	#0x4
    c4ae:	d7 74       	stab	*0x74
    c4b0:	18 ce 01 30 	ldy	#0x130
    c4b4:	ce e3 19    	ldx	#0xe319
    c4b7:	bd ec 6f    	jsr	0xec6f
    c4ba:	86 04       	ldaa	#0x4
    c4bc:	b7 01 1c    	staa	0x11c
    c4bf:	7f 01 1a    	clr	0x11a
    c4c2:	7f 01 7e    	clr	0x17e
    c4c5:	7e d5 2f    	jmp	0xd52f
    c4c8:	d6 74       	ldab	*0x74
    c4ca:	5a          	decb
    c4cb:	2a e1       	bpl	0x0xc4ae
    c4cd:	5f          	clrb
    c4ce:	20 de       	bra	0x0xc4ae
    c4d0:	d6 72       	ldab	*0x72
    c4d2:	7d 01 1a    	tst	0x11a
    c4d5:	2b 23       	bmi	0x0xc4fa
    c4d7:	5c          	incb
    c4d8:	c1 0d       	cmpb	#0xd
    c4da:	25 02       	bcs	0x0xc4de
    c4dc:	c6 0c       	ldab	#0xc
    c4de:	d7 72       	stab	*0x72
    c4e0:	d7 dc       	stab	*0xdc
    c4e2:	18 ce 01 36 	ldy	#0x136
    c4e6:	ce e4 ba    	ldx	#0xe4ba
    c4e9:	bd ec 6f    	jsr	0xec6f
    c4ec:	86 04       	ldaa	#0x4
    c4ee:	b7 01 1c    	staa	0x11c
    c4f1:	7f 01 1a    	clr	0x11a
    c4f4:	7f 01 7e    	clr	0x17e
    c4f7:	7e d5 2f    	jmp	0xd52f
    c4fa:	5a          	decb
    c4fb:	2a e1       	bpl	0x0xc4de
    c4fd:	5f          	clrb
    c4fe:	20 de       	bra	0x0xc4de
    c500:	7d 01 1c    	tst	0x11c
    c503:	2a 03       	bpl	0x0xc508
    c505:	7e e3 2d    	jmp	0xe32d
    c508:	7d 01 1e    	tst	0x11e
    c50b:	26 08       	bne	0x0xc515
    c50d:	7d 01 1c    	tst	0x11c
    c510:	27 1f       	beq	0x0xc531
    c512:	7e b2 70    	jmp	0xb270
    c515:	7d 01 1c    	tst	0x11c
    c518:	27 24       	beq	0x0xc53e
    c51a:	cc 01 20    	ldd	#0x120
    c51d:	fb 01 1e    	addb	0x11e
    c520:	8f          	xgdx
    c521:	bd ea a8    	jsr	0xeaa8
    c524:	7a 01 1e    	dec	0x11e
    c527:	7a 01 1c    	dec	0x11c
    c52a:	26 12       	bne	0x0xc53e
    c52c:	bd eb 26    	jsr	0xeb26
    c52f:	20 0d       	bra	0x0xc53e
    c531:	7d 01 1e    	tst	0x11e
    c534:	26 08       	bne	0x0xc53e
    c536:	86 03       	ldaa	#0x3
    c538:	b7 01 1e    	staa	0x11e
    c53b:	bd ea e4    	jsr	0xeae4
    c53e:	7d 01 7f    	tst	0x17f
    c541:	27 10       	beq	0x0xc553
    c543:	bd ec e8    	jsr	0xece8
    c546:	4f          	clra
    c547:	b7 01 7f    	staa	0x17f
    c54a:	b7 01 7e    	staa	0x17e
    c54d:	b7 01 1a    	staa	0x11a
    c550:	7e d5 2f    	jmp	0xd52f
    c553:	7d 01 7e    	tst	0x17e
    c556:	27 0c       	beq	0x0xc564
    c558:	bd ec 90    	jsr	0xec90
    c55b:	7f 01 7e    	clr	0x17e
    c55e:	7f 01 1a    	clr	0x11a
    c561:	7e d5 2f    	jmp	0xd52f
    c564:	7d 01 1a    	tst	0x11a
    c567:	26 03       	bne	0x0xc56c
    c569:	7e d5 2f    	jmp	0xd52f
    c56c:	ce e3 d1    	ldx	#0xe3d1
    c56f:	b6 01 1e    	ldaa	0x11e
    c572:	81 03       	cmpa	#0x3
    c574:	26 34       	bne	0x0xc5aa
    c576:	18 ce 01 20 	ldy	#0x120
    c57a:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc594
    c57e:	d6 2d       	ldab	*0x2d
    c580:	bd c6 1a    	jsr	0xc61a
    c583:	d7 2d       	stab	*0x2d
    c585:	bd ec 6f    	jsr	0xec6f
    c588:	96 2d       	ldaa	*0x2d
    c58a:	c6 2d       	ldab	#0x2d
    c58c:	bd b0 fc    	jsr	0xb0fc
    c58f:	86 04       	ldaa	#0x4
    c591:	7e c6 c7    	jmp	0xc6c7
    c594:	d6 3a       	ldab	*0x3a
    c596:	bd c6 1a    	jsr	0xc61a
    c599:	d7 3a       	stab	*0x3a
    c59b:	bd ec 6f    	jsr	0xec6f
    c59e:	96 3a       	ldaa	*0x3a
    c5a0:	c6 3a       	ldab	#0x3a
    c5a2:	bd b0 fc    	jsr	0xb0fc
    c5a5:	86 04       	ldaa	#0x4
    c5a7:	7e c6 c7    	jmp	0xc6c7
    c5aa:	81 09       	cmpa	#0x9
    c5ac:	26 34       	bne	0x0xc5e2
    c5ae:	18 ce 01 26 	ldy	#0x126
    c5b2:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc5cc
    c5b6:	d6 2e       	ldab	*0x2e
    c5b8:	bd c6 1a    	jsr	0xc61a
    c5bb:	d7 2e       	stab	*0x2e
    c5bd:	bd ec 6f    	jsr	0xec6f
    c5c0:	96 2e       	ldaa	*0x2e
    c5c2:	c6 2e       	ldab	#0x2e
    c5c4:	bd b0 fc    	jsr	0xb0fc
    c5c7:	86 04       	ldaa	#0x4
    c5c9:	7e c6 c7    	jmp	0xc6c7
    c5cc:	d6 3b       	ldab	*0x3b
    c5ce:	bd c6 1a    	jsr	0xc61a
    c5d1:	d7 3b       	stab	*0x3b
    c5d3:	bd ec 6f    	jsr	0xec6f
    c5d6:	96 3b       	ldaa	*0x3b
    c5d8:	c6 3b       	ldab	#0x3b
    c5da:	bd b0 fc    	jsr	0xb0fc
    c5dd:	86 04       	ldaa	#0x4
    c5df:	7e c6 c7    	jmp	0xc6c7
    c5e2:	81 0e       	cmpa	#0xe
    c5e4:	26 46       	bne	0x0xc62c
    c5e6:	18 ce 01 2b 	ldy	#0x12b
    c5ea:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc604
    c5ee:	d6 2f       	ldab	*0x2f
    c5f0:	bd c6 1a    	jsr	0xc61a
    c5f3:	d7 2f       	stab	*0x2f
    c5f5:	bd ec 6f    	jsr	0xec6f
    c5f8:	96 2f       	ldaa	*0x2f
    c5fa:	c6 2f       	ldab	#0x2f
    c5fc:	bd b0 fc    	jsr	0xb0fc
    c5ff:	86 04       	ldaa	#0x4
    c601:	7e c6 c7    	jmp	0xc6c7
    c604:	d6 3c       	ldab	*0x3c
    c606:	bd c6 1a    	jsr	0xc61a
    c609:	d7 3c       	stab	*0x3c
    c60b:	bd ec 6f    	jsr	0xec6f
    c60e:	96 3c       	ldaa	*0x3c
    c610:	c6 3c       	ldab	#0x3c
    c612:	bd b0 fc    	jsr	0xb0fc
    c615:	86 04       	ldaa	#0x4
    c617:	7e c6 c7    	jmp	0xc6c7
    c61a:	7d 01 1a    	tst	0x11a
    c61d:	2b 08       	bmi	0x0xc627
    c61f:	5c          	incb
    c620:	c1 11       	cmpb	#0x11
    c622:	25 02       	bcs	0x0xc626
    c624:	c6 10       	ldab	#0x10
    c626:	39          	rts
    c627:	5a          	decb
    c628:	2a fc       	bpl	0x0xc626
    c62a:	5f          	clrb
    c62b:	39          	rts
    c62c:	81 13       	cmpa	#0x13
    c62e:	26 33       	bne	0x0xc663
    c630:	ce 01 31    	ldx	#0x131
    c633:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc64d
    c637:	96 2a       	ldaa	*0x2a
    c639:	bd c6 d0    	jsr	0xc6d0
    c63c:	97 2a       	staa	*0x2a
    c63e:	bd eb bc    	jsr	0xebbc
    c641:	96 2a       	ldaa	*0x2a
    c643:	c6 2a       	ldab	#0x2a
    c645:	bd b0 fc    	jsr	0xb0fc
    c648:	86 03       	ldaa	#0x3
    c64a:	7e c6 c7    	jmp	0xc6c7
    c64d:	96 37       	ldaa	*0x37
    c64f:	bd c6 d0    	jsr	0xc6d0
    c652:	97 37       	staa	*0x37
    c654:	bd eb bc    	jsr	0xebbc
    c657:	96 37       	ldaa	*0x37
    c659:	c6 37       	ldab	#0x37
    c65b:	bd b0 fc    	jsr	0xb0fc
    c65e:	86 03       	ldaa	#0x3
    c660:	7e c6 c7    	jmp	0xc6c7
    c663:	81 19       	cmpa	#0x19
    c665:	26 31       	bne	0x0xc698
    c667:	ce 01 37    	ldx	#0x137
    c66a:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc683
    c66e:	96 2b       	ldaa	*0x2b
    c670:	bd c6 d0    	jsr	0xc6d0
    c673:	97 2b       	staa	*0x2b
    c675:	bd eb bc    	jsr	0xebbc
    c678:	96 2b       	ldaa	*0x2b
    c67a:	c6 2b       	ldab	#0x2b
    c67c:	bd b0 fc    	jsr	0xb0fc
    c67f:	86 03       	ldaa	#0x3
    c681:	20 44       	bra	0x0xc6c7
    c683:	96 38       	ldaa	*0x38
    c685:	bd c6 d0    	jsr	0xc6d0
    c688:	97 38       	staa	*0x38
    c68a:	bd eb bc    	jsr	0xebbc
    c68d:	96 38       	ldaa	*0x38
    c68f:	c6 38       	ldab	#0x38
    c691:	bd b0 fc    	jsr	0xb0fc
    c694:	86 03       	ldaa	#0x3
    c696:	20 2f       	bra	0x0xc6c7
    c698:	ce 01 3c    	ldx	#0x13c
    c69b:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc6b4
    c69f:	96 2c       	ldaa	*0x2c
    c6a1:	bd c6 d0    	jsr	0xc6d0
    c6a4:	97 2c       	staa	*0x2c
    c6a6:	bd eb bc    	jsr	0xebbc
    c6a9:	96 2c       	ldaa	*0x2c
    c6ab:	c6 2c       	ldab	#0x2c
    c6ad:	bd b0 fc    	jsr	0xb0fc
    c6b0:	86 03       	ldaa	#0x3
    c6b2:	20 13       	bra	0x0xc6c7
    c6b4:	96 39       	ldaa	*0x39
    c6b6:	bd c6 d0    	jsr	0xc6d0
    c6b9:	97 39       	staa	*0x39
    c6bb:	bd eb bc    	jsr	0xebbc
    c6be:	96 39       	ldaa	*0x39
    c6c0:	c6 39       	ldab	#0x39
    c6c2:	bd b0 fc    	jsr	0xb0fc
    c6c5:	86 03       	ldaa	#0x3
    c6c7:	b7 01 1c    	staa	0x11c
    c6ca:	7f 01 1a    	clr	0x11a
    c6cd:	7e d5 2f    	jmp	0xd52f
    c6d0:	7d 01 1a    	tst	0x11a
    c6d3:	2b 04       	bmi	0x0xc6d9
    c6d5:	4c          	inca
    c6d6:	84 7f       	anda	#0x7f
    c6d8:	39          	rts
    c6d9:	4a          	deca
    c6da:	84 7f       	anda	#0x7f
    c6dc:	20 fa       	bra	0x0xc6d8
    c6de:	7d 01 1c    	tst	0x11c
    c6e1:	2a 03       	bpl	0x0xc6e6
    c6e3:	7e e4 15    	jmp	0xe415
    c6e6:	7d 01 1e    	tst	0x11e
    c6e9:	26 08       	bne	0x0xc6f3
    c6eb:	7d 01 1c    	tst	0x11c
    c6ee:	27 1f       	beq	0x0xc70f
    c6f0:	7e b2 70    	jmp	0xb270
    c6f3:	7d 01 1c    	tst	0x11c
    c6f6:	27 24       	beq	0x0xc71c
    c6f8:	cc 01 20    	ldd	#0x120
    c6fb:	fb 01 1e    	addb	0x11e
    c6fe:	8f          	xgdx
    c6ff:	bd ea a8    	jsr	0xeaa8
    c702:	7a 01 1e    	dec	0x11e
    c705:	7a 01 1c    	dec	0x11c
    c708:	26 12       	bne	0x0xc71c
    c70a:	bd eb 26    	jsr	0xeb26
    c70d:	20 0d       	bra	0x0xc71c
    c70f:	7d 01 1e    	tst	0x11e
    c712:	26 08       	bne	0x0xc71c
    c714:	86 13       	ldaa	#0x13
    c716:	b7 01 1e    	staa	0x11e
    c719:	bd ea e4    	jsr	0xeae4
    c71c:	7d 01 7f    	tst	0x17f
    c71f:	27 10       	beq	0x0xc731
    c721:	bd ec e8    	jsr	0xece8
    c724:	4f          	clra
    c725:	b7 01 7f    	staa	0x17f
    c728:	b7 01 7e    	staa	0x17e
    c72b:	b7 01 1a    	staa	0x11a
    c72e:	7e d5 2f    	jmp	0xd52f
    c731:	7d 01 1a    	tst	0x11a
    c734:	26 03       	bne	0x0xc739
    c736:	7e d5 2f    	jmp	0xd52f
    c739:	b6 01 1e    	ldaa	0x11e
    c73c:	81 13       	cmpa	#0x13
    c73e:	26 53       	bne	0x0xc793
    c740:	ce e4 9a    	ldx	#0xe49a
    c743:	18 ce 01 30 	ldy	#0x130
    c747:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc778
    c74b:	d6 27       	ldab	*0x27
    c74d:	8d 17       	bsr	0x0xc766
    c74f:	d7 27       	stab	*0x27
    c751:	bd ec 6f    	jsr	0xec6f
    c754:	7f 01 1a    	clr	0x11a
    c757:	86 04       	ldaa	#0x4
    c759:	b7 01 1c    	staa	0x11c
    c75c:	96 27       	ldaa	*0x27
    c75e:	c6 27       	ldab	#0x27
    c760:	bd b0 fc    	jsr	0xb0fc
    c763:	7e d5 2f    	jmp	0xd52f
    c766:	7d 01 1a    	tst	0x11a
    c769:	2b 08       	bmi	0x0xc773
    c76b:	5c          	incb
    c76c:	c1 06       	cmpb	#0x6
    c76e:	25 02       	bcs	0x0xc772
    c770:	c6 05       	ldab	#0x5
    c772:	39          	rts
    c773:	5a          	decb
    c774:	2a fc       	bpl	0x0xc772
    c776:	5f          	clrb
    c777:	39          	rts
    c778:	d6 34       	ldab	*0x34
    c77a:	8d ea       	bsr	0x0xc766
    c77c:	d7 34       	stab	*0x34
    c77e:	bd ec 6f    	jsr	0xec6f
    c781:	7f 01 1a    	clr	0x11a
    c784:	86 04       	ldaa	#0x4
    c786:	b7 01 1c    	staa	0x11c
    c789:	96 34       	ldaa	*0x34
    c78b:	c6 34       	ldab	#0x34
    c78d:	bd b0 fc    	jsr	0xb0fc
    c790:	7e d5 2f    	jmp	0xd52f
    c793:	81 19       	cmpa	#0x19
    c795:	26 23       	bne	0x0xc7ba
    c797:	ce e4 b2    	ldx	#0xe4b2
    c79a:	18 ce 01 36 	ldy	#0x136
    c79e:	d6 31       	ldab	*0x31
    c7a0:	5c          	incb
    c7a1:	c4 01       	andb	#0x1
    c7a3:	d7 31       	stab	*0x31
    c7a5:	bd ec 6f    	jsr	0xec6f
    c7a8:	7f 01 1a    	clr	0x11a
    c7ab:	86 04       	ldaa	#0x4
    c7ad:	b7 01 1c    	staa	0x11c
    c7b0:	96 31       	ldaa	*0x31
    c7b2:	c6 31       	ldab	#0x31
    c7b4:	bd b0 fc    	jsr	0xb0fc
    c7b7:	7e d5 2f    	jmp	0xd52f
    c7ba:	ce e4 ba    	ldx	#0xe4ba
    c7bd:	18 ce 01 3b 	ldy	#0x13b
    c7c1:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc7f2
    c7c5:	d6 25       	ldab	*0x25
    c7c7:	8d 17       	bsr	0x0xc7e0
    c7c9:	d7 25       	stab	*0x25
    c7cb:	bd ec 6f    	jsr	0xec6f
    c7ce:	7f 01 1a    	clr	0x11a
    c7d1:	86 04       	ldaa	#0x4
    c7d3:	b7 01 1c    	staa	0x11c
    c7d6:	96 25       	ldaa	*0x25
    c7d8:	c6 25       	ldab	#0x25
    c7da:	bd b0 fc    	jsr	0xb0fc
    c7dd:	7e d5 2f    	jmp	0xd52f
    c7e0:	7d 01 1a    	tst	0x11a
    c7e3:	2b 08       	bmi	0x0xc7ed
    c7e5:	5c          	incb
    c7e6:	c1 0d       	cmpb	#0xd
    c7e8:	25 02       	bcs	0x0xc7ec
    c7ea:	c6 0c       	ldab	#0xc
    c7ec:	39          	rts
    c7ed:	5a          	decb
    c7ee:	2a fc       	bpl	0x0xc7ec
    c7f0:	5f          	clrb
    c7f1:	39          	rts
    c7f2:	d6 32       	ldab	*0x32
    c7f4:	8d ea       	bsr	0x0xc7e0
    c7f6:	d7 32       	stab	*0x32
    c7f8:	bd ec 6f    	jsr	0xec6f
    c7fb:	7f 01 1a    	clr	0x11a
    c7fe:	86 04       	ldaa	#0x4
    c800:	b7 01 1c    	staa	0x11c
    c803:	96 32       	ldaa	*0x32
    c805:	c6 32       	ldab	#0x32
    c807:	bd b0 fc    	jsr	0xb0fc
    c80a:	7e d5 2f    	jmp	0xd52f
    c80d:	7d 01 1c    	tst	0x11c
    c810:	2a 03       	bpl	0x0xc815
    c812:	7e e4 ee    	jmp	0xe4ee
    c815:	7d 01 1e    	tst	0x11e
    c818:	26 08       	bne	0x0xc822
    c81a:	7d 01 1c    	tst	0x11c
    c81d:	27 1f       	beq	0x0xc83e
    c81f:	7e b2 70    	jmp	0xb270
    c822:	7d 01 1c    	tst	0x11c
    c825:	27 24       	beq	0x0xc84b
    c827:	cc 01 20    	ldd	#0x120
    c82a:	fb 01 1e    	addb	0x11e
    c82d:	8f          	xgdx
    c82e:	bd ea a8    	jsr	0xeaa8
    c831:	7a 01 1e    	dec	0x11e
    c834:	7a 01 1c    	dec	0x11c
    c837:	26 12       	bne	0x0xc84b
    c839:	bd eb 26    	jsr	0xeb26
    c83c:	20 0d       	bra	0x0xc84b
    c83e:	7d 01 1e    	tst	0x11e
    c841:	26 08       	bne	0x0xc84b
    c843:	86 19       	ldaa	#0x19
    c845:	b7 01 1e    	staa	0x11e
    c848:	bd ea e4    	jsr	0xeae4
    c84b:	7d 01 7f    	tst	0x17f
    c84e:	27 1e       	beq	0x0xc86e
    c850:	2b 10       	bmi	0x0xc862
    c852:	bd ec e8    	jsr	0xece8
    c855:	4f          	clra
    c856:	b7 01 7f    	staa	0x17f
    c859:	b7 01 7e    	staa	0x17e
    c85c:	b7 01 1a    	staa	0x11a
    c85f:	7e d5 2f    	jmp	0xd52f
    c862:	b6 01 1e    	ldaa	0x11e
    c865:	81 19       	cmpa	#0x19
    c867:	27 ec       	beq	0x0xc855
    c869:	bd ec e8    	jsr	0xece8
    c86c:	20 e7       	bra	0x0xc855
    c86e:	7d 01 1a    	tst	0x11a
    c871:	26 03       	bne	0x0xc876
    c873:	7e d5 2f    	jmp	0xd52f
    c876:	b6 01 1e    	ldaa	0x11e
    c879:	81 13       	cmpa	#0x13
    c87b:	26 5a       	bne	0x0xc8d7
    c87d:	12 f4 02 2b 	brset	*0xf4, #0x02, 0x0xc8ac
    c881:	7d 01 1a    	tst	0x11a
    c884:	2b 1f       	bmi	0x0xc8a5
    c886:	96 29       	ldaa	*0x29
    c888:	4c          	inca
    c889:	84 7f       	anda	#0x7f
    c88b:	97 29       	staa	*0x29
    c88d:	ce 01 31    	ldx	#0x131
    c890:	bd eb bc    	jsr	0xebbc
    c893:	7f 01 1a    	clr	0x11a
    c896:	86 03       	ldaa	#0x3
    c898:	b7 01 1c    	staa	0x11c
    c89b:	96 29       	ldaa	*0x29
    c89d:	c6 29       	ldab	#0x29
    c89f:	bd b0 fc    	jsr	0xb0fc
    c8a2:	7e d5 2f    	jmp	0xd52f
    c8a5:	96 29       	ldaa	*0x29
    c8a7:	4a          	deca
    c8a8:	84 7f       	anda	#0x7f
    c8aa:	20 df       	bra	0x0xc88b
    c8ac:	7d 01 1a    	tst	0x11a
    c8af:	2b 1f       	bmi	0x0xc8d0
    c8b1:	96 36       	ldaa	*0x36
    c8b3:	4c          	inca
    c8b4:	84 7f       	anda	#0x7f
    c8b6:	97 36       	staa	*0x36
    c8b8:	ce 01 31    	ldx	#0x131
    c8bb:	bd eb bc    	jsr	0xebbc
    c8be:	7f 01 1a    	clr	0x11a
    c8c1:	86 03       	ldaa	#0x3
    c8c3:	b7 01 1c    	staa	0x11c
    c8c6:	96 36       	ldaa	*0x36
    c8c8:	c6 36       	ldab	#0x36
    c8ca:	bd b0 fc    	jsr	0xb0fc
    c8cd:	7e d5 2f    	jmp	0xd52f
    c8d0:	96 36       	ldaa	*0x36
    c8d2:	4a          	deca
    c8d3:	84 7f       	anda	#0x7f
    c8d5:	20 df       	bra	0x0xc8b6
    c8d7:	81 19       	cmpa	#0x19
    c8d9:	26 65       	bne	0x0xc940
    c8db:	ce e5 54    	ldx	#0xe554
    c8de:	18 ce 01 37 	ldy	#0x137
    c8e2:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc913
    c8e6:	7d 01 1a    	tst	0x11a
    c8e9:	2b 20       	bmi	0x0xc90b
    c8eb:	d6 26       	ldab	*0x26
    c8ed:	5c          	incb
    c8ee:	c1 03       	cmpb	#0x3
    c8f0:	25 02       	bcs	0x0xc8f4
    c8f2:	c6 02       	ldab	#0x2
    c8f4:	d7 26       	stab	*0x26
    c8f6:	bd ec 5b    	jsr	0xec5b
    c8f9:	7f 01 1a    	clr	0x11a
    c8fc:	86 03       	ldaa	#0x3
    c8fe:	b7 01 1c    	staa	0x11c
    c901:	96 26       	ldaa	*0x26
    c903:	c6 26       	ldab	#0x26
    c905:	bd b0 fc    	jsr	0xb0fc
    c908:	7e d5 2f    	jmp	0xd52f
    c90b:	d6 26       	ldab	*0x26
    c90d:	5a          	decb
    c90e:	2a e4       	bpl	0x0xc8f4
    c910:	5f          	clrb
    c911:	20 e1       	bra	0x0xc8f4
    c913:	7d 01 1a    	tst	0x11a
    c916:	2b 20       	bmi	0x0xc938
    c918:	d6 33       	ldab	*0x33
    c91a:	5c          	incb
    c91b:	c1 03       	cmpb	#0x3
    c91d:	25 02       	bcs	0x0xc921
    c91f:	c6 02       	ldab	#0x2
    c921:	d7 33       	stab	*0x33
    c923:	bd ec 5b    	jsr	0xec5b
    c926:	7f 01 1a    	clr	0x11a
    c929:	86 03       	ldaa	#0x3
    c92b:	b7 01 1c    	staa	0x11c
    c92e:	96 33       	ldaa	*0x33
    c930:	c6 33       	ldab	#0x33
    c932:	bd b0 fc    	jsr	0xb0fc
    c935:	7e d5 2f    	jmp	0xd52f
    c938:	d6 33       	ldab	*0x33
    c93a:	5a          	decb
    c93b:	2a e4       	bpl	0x0xc921
    c93d:	5f          	clrb
    c93e:	20 e1       	bra	0x0xc921
    c940:	ce e5 63    	ldx	#0xe563
    c943:	18 ce 01 3c 	ldy	#0x13c
    c947:	7d 01 1a    	tst	0x11a
    c94a:	2b 20       	bmi	0x0xc96c
    c94c:	d6 96       	ldab	*0x96
    c94e:	5c          	incb
    c94f:	c1 04       	cmpb	#0x4
    c951:	25 02       	bcs	0x0xc955
    c953:	c6 03       	ldab	#0x3
    c955:	d7 96       	stab	*0x96
    c957:	bd ec 5b    	jsr	0xec5b
    c95a:	7f 01 1a    	clr	0x11a
    c95d:	86 03       	ldaa	#0x3
    c95f:	b7 01 1c    	staa	0x11c
    c962:	96 96       	ldaa	*0x96
    c964:	c6 96       	ldab	#0x96
    c966:	bd b0 fc    	jsr	0xb0fc
    c969:	7e d5 2f    	jmp	0xd52f
    c96c:	d6 96       	ldab	*0x96
    c96e:	5a          	decb
    c96f:	2a e4       	bpl	0x0xc955
    c971:	5f          	clrb
    c972:	20 e1       	bra	0x0xc955
    c974:	7e d5 2f    	jmp	0xd52f
    c977:	7d 01 1c    	tst	0x11c
    c97a:	2a 03       	bpl	0x0xc97f
    c97c:	7e e5 6f    	jmp	0xe56f
    c97f:	7d 01 1e    	tst	0x11e
    c982:	26 08       	bne	0x0xc98c
    c984:	7d 01 1c    	tst	0x11c
    c987:	27 1f       	beq	0x0xc9a8
    c989:	7e b2 70    	jmp	0xb270
    c98c:	7d 01 1c    	tst	0x11c
    c98f:	27 24       	beq	0x0xc9b5
    c991:	cc 01 20    	ldd	#0x120
    c994:	fb 01 1e    	addb	0x11e
    c997:	8f          	xgdx
    c998:	bd ea a8    	jsr	0xeaa8
    c99b:	7a 01 1e    	dec	0x11e
    c99e:	7a 01 1c    	dec	0x11c
    c9a1:	26 12       	bne	0x0xc9b5
    c9a3:	bd eb 59    	jsr	0xeb59
    c9a6:	20 0d       	bra	0x0xc9b5
    c9a8:	7d 01 1e    	tst	0x11e
    c9ab:	26 08       	bne	0x0xc9b5
    c9ad:	86 02       	ldaa	#0x2
    c9af:	b7 01 1e    	staa	0x11e
    c9b2:	bd ea e4    	jsr	0xeae4
    c9b5:	7d 01 7f    	tst	0x17f
    c9b8:	27 10       	beq	0x0xc9ca
    c9ba:	bd ed 5a    	jsr	0xed5a
    c9bd:	4f          	clra
    c9be:	b7 01 7f    	staa	0x17f
    c9c1:	b7 01 7e    	staa	0x17e
    c9c4:	b7 01 1a    	staa	0x11a
    c9c7:	7e d5 2f    	jmp	0xd52f
    c9ca:	7d 01 7e    	tst	0x17e
    c9cd:	27 0c       	beq	0x0xc9db
    c9cf:	bd ec 90    	jsr	0xec90
    c9d2:	7f 01 7e    	clr	0x17e
    c9d5:	7f 01 1a    	clr	0x11a
    c9d8:	7e d5 2f    	jmp	0xd52f
    c9db:	7d 01 1a    	tst	0x11a
    c9de:	26 03       	bne	0x0xc9e3
    c9e0:	7e d5 2f    	jmp	0xd52f
    c9e3:	c6 02       	ldab	#0x2
    c9e5:	18 ce 00 b0 	ldy	#0xb0
    c9e9:	96 f9       	ldaa	*0xf9
    c9eb:	81 04       	cmpa	#0x4
    c9ed:	26 07       	bne	0x0xc9f6
    c9ef:	18 ce 50 30 	ldy	#0x5030
    c9f3:	14 fb 80    	bset	*0xfb, #0x80
    c9f6:	ce 01 20    	ldx	#0x120
    c9f9:	f1 01 1e    	cmpb	0x11e
    c9fc:	27 0a       	beq	0x0xca08
    c9fe:	18 08       	iny
    ca00:	cb 04       	addb	#0x4
    ca02:	08          	inx
    ca03:	08          	inx
    ca04:	08          	inx
    ca05:	08          	inx
    ca06:	20 f1       	bra	0x0xc9f9
    ca08:	18 a6 00    	ldaa	0x0,y
    ca0b:	7d 01 1a    	tst	0x11a
    ca0e:	2b 23       	bmi	0x0xca33
    ca10:	4c          	inca
    ca11:	f6 01 11    	ldab	0x111
    ca14:	c5 04       	bitb	#0x4
    ca16:	27 02       	beq	0x0xca1a
    ca18:	8b 09       	adda	#0x9
    ca1a:	4d          	tsta
    ca1b:	2a 02       	bpl	0x0xca1f
    ca1d:	86 7f       	ldaa	#0x7f
    ca1f:	18 a7 00    	staa	0x0,y
    ca22:	bd eb f8    	jsr	0xebf8
    ca25:	bd ca 43    	jsr	0xca43
    ca28:	7f 01 1a    	clr	0x11a
    ca2b:	86 03       	ldaa	#0x3
    ca2d:	b7 01 1c    	staa	0x11c
    ca30:	7e d5 2f    	jmp	0xd52f
    ca33:	4a          	deca
    ca34:	f6 01 11    	ldab	0x111
    ca37:	c5 04       	bitb	#0x4
    ca39:	27 02       	beq	0x0xca3d
    ca3b:	80 09       	suba	#0x9
    ca3d:	4d          	tsta
    ca3e:	2a df       	bpl	0x0xca1f
    ca40:	4f          	clra
    ca41:	20 dc       	bra	0x0xca1f
    ca43:	18 a6 00    	ldaa	0x0,y
    ca46:	36          	psha
    ca47:	18 8f       	xgdy
    ca49:	c1 3f       	cmpb	#0x3f
    ca4b:	22 02       	bhi	0x0xca4f
    ca4d:	cb 80       	addb	#0x80
    ca4f:	37          	pshb
    ca50:	c4 0f       	andb	#0xf
    ca52:	c1 08       	cmpb	#0x8
    ca54:	25 02       	bcs	0x0xca58
    ca56:	c0 08       	subb	#0x8
    ca58:	5c          	incb
    ca59:	86 01       	ldaa	#0x1
    ca5b:	5a          	decb
    ca5c:	27 03       	beq	0x0xca61
    ca5e:	48          	asla
    ca5f:	20 fa       	bra	0x0xca5b
    ca61:	43          	coma
    ca62:	7d 00 d0    	tst	0xd0
    ca65:	27 05       	beq	0x0xca6c
    ca67:	7f 00 d0    	clr	0xd0
    ca6a:	20 08       	bra	0x0xca74
    ca6c:	7d 10 29    	tst	0x1029
    ca6f:	2a fb       	bpl	0x0xca6c
    ca71:	f6 10 2a    	ldab	0x102a
    ca74:	01          	nop
    ca75:	01          	nop
    ca76:	01          	nop
    ca77:	01          	nop
    ca78:	b7 10 42    	staa	0x1042
    ca7b:	01          	nop
    ca7c:	01          	nop
    ca7d:	01          	nop
    ca7e:	01          	nop
    ca7f:	01          	nop
    ca80:	01          	nop
    ca81:	01          	nop
    ca82:	86 83       	ldaa	#0x83
    ca84:	b7 10 2a    	staa	0x102a
    ca87:	32          	pula
    ca88:	81 b8       	cmpa	#0xb8
    ca8a:	25 04       	bcs	0x0xca90
    ca8c:	86 8a       	ldaa	#0x8a
    ca8e:	20 0a       	bra	0x0xca9a
    ca90:	81 b0       	cmpa	#0xb0
    ca92:	25 04       	bcs	0x0xca98
    ca94:	86 89       	ldaa	#0x89
    ca96:	20 02       	bra	0x0xca9a
    ca98:	86 88       	ldaa	#0x88
    ca9a:	7d 10 29    	tst	0x1029
    ca9d:	2a fb       	bpl	0x0xca9a
    ca9f:	f6 10 2a    	ldab	0x102a
    caa2:	b7 10 2a    	staa	0x102a
    caa5:	32          	pula
    caa6:	7d 10 29    	tst	0x1029
    caa9:	2a fb       	bpl	0x0xcaa6
    caab:	f6 10 2a    	ldab	0x102a
    caae:	b7 10 2a    	staa	0x102a
    cab1:	39          	rts
    cab2:	7d 01 1c    	tst	0x11c
    cab5:	2a 03       	bpl	0x0xcaba
    cab7:	7e e5 b9    	jmp	0xe5b9
    caba:	7d 01 1e    	tst	0x11e
    cabd:	26 08       	bne	0x0xcac7
    cabf:	7d 01 1c    	tst	0x11c
    cac2:	27 1f       	beq	0x0xcae3
    cac4:	7e b2 70    	jmp	0xb270
    cac7:	7d 01 1c    	tst	0x11c
    caca:	27 24       	beq	0x0xcaf0
    cacc:	cc 01 20    	ldd	#0x120
    cacf:	fb 01 1e    	addb	0x11e
    cad2:	8f          	xgdx
    cad3:	bd ea a8    	jsr	0xeaa8
    cad6:	7a 01 1e    	dec	0x11e
    cad9:	7a 01 1c    	dec	0x11c
    cadc:	26 12       	bne	0x0xcaf0
    cade:	bd eb 59    	jsr	0xeb59
    cae1:	20 0d       	bra	0x0xcaf0
    cae3:	7d 01 1e    	tst	0x11e
    cae6:	26 08       	bne	0x0xcaf0
    cae8:	86 02       	ldaa	#0x2
    caea:	b7 01 1e    	staa	0x11e
    caed:	bd ea e4    	jsr	0xeae4
    caf0:	7d 01 7f    	tst	0x17f
    caf3:	27 10       	beq	0x0xcb05
    caf5:	bd ed 5a    	jsr	0xed5a
    caf8:	4f          	clra
    caf9:	b7 01 7f    	staa	0x17f
    cafc:	b7 01 7e    	staa	0x17e
    caff:	b7 01 1a    	staa	0x11a
    cb02:	7e d5 2f    	jmp	0xd52f
    cb05:	7d 01 7e    	tst	0x17e
    cb08:	27 0c       	beq	0x0xcb16
    cb0a:	bd ec 90    	jsr	0xec90
    cb0d:	7f 01 7e    	clr	0x17e
    cb10:	7f 01 1a    	clr	0x11a
    cb13:	7e d5 2f    	jmp	0xd52f
    cb16:	7d 01 1a    	tst	0x11a
    cb19:	26 03       	bne	0x0xcb1e
    cb1b:	7e d5 2f    	jmp	0xd52f
    cb1e:	c6 02       	ldab	#0x2
    cb20:	18 ce 00 a8 	ldy	#0xa8
    cb24:	96 f9       	ldaa	*0xf9
    cb26:	81 04       	cmpa	#0x4
    cb28:	26 07       	bne	0x0xcb31
    cb2a:	14 fb 80    	bset	*0xfb, #0x80
    cb2d:	18 ce 50 28 	ldy	#0x5028
    cb31:	ce 01 20    	ldx	#0x120
    cb34:	f1 01 1e    	cmpb	0x11e
    cb37:	27 0a       	beq	0x0xcb43
    cb39:	18 08       	iny
    cb3b:	cb 04       	addb	#0x4
    cb3d:	08          	inx
    cb3e:	08          	inx
    cb3f:	08          	inx
    cb40:	08          	inx
    cb41:	20 f1       	bra	0x0xcb34
    cb43:	18 a6 00    	ldaa	0x0,y
    cb46:	7d 01 1a    	tst	0x11a
    cb49:	2b 20       	bmi	0x0xcb6b
    cb4b:	4c          	inca
    cb4c:	f6 01 11    	ldab	0x111
    cb4f:	c5 04       	bitb	#0x4
    cb51:	27 02       	beq	0x0xcb55
    cb53:	8b 09       	adda	#0x9
    cb55:	84 7f       	anda	#0x7f
    cb57:	18 a7 00    	staa	0x0,y
    cb5a:	bd eb bc    	jsr	0xebbc
    cb5d:	bd ca 43    	jsr	0xca43
    cb60:	7f 01 1a    	clr	0x11a
    cb63:	86 03       	ldaa	#0x3
    cb65:	b7 01 1c    	staa	0x11c
    cb68:	7e d5 2f    	jmp	0xd52f
    cb6b:	4a          	deca
    cb6c:	f6 01 11    	ldab	0x111
    cb6f:	c5 04       	bitb	#0x4
    cb71:	27 02       	beq	0x0xcb75
    cb73:	80 09       	suba	#0x9
    cb75:	84 7f       	anda	#0x7f
    cb77:	20 de       	bra	0x0xcb57
    cb79:	7d 01 1c    	tst	0x11c
    cb7c:	2a 03       	bpl	0x0xcb81
    cb7e:	7e e5 f2    	jmp	0xe5f2
    cb81:	7d 01 1e    	tst	0x11e
    cb84:	26 08       	bne	0x0xcb8e
    cb86:	7d 01 1c    	tst	0x11c
    cb89:	27 1f       	beq	0x0xcbaa
    cb8b:	7e b2 70    	jmp	0xb270
    cb8e:	7d 01 1c    	tst	0x11c
    cb91:	27 24       	beq	0x0xcbb7
    cb93:	cc 01 20    	ldd	#0x120
    cb96:	fb 01 1e    	addb	0x11e
    cb99:	8f          	xgdx
    cb9a:	bd ea a8    	jsr	0xeaa8
    cb9d:	7a 01 1e    	dec	0x11e
    cba0:	7a 01 1c    	dec	0x11c
    cba3:	26 12       	bne	0x0xcbb7
    cba5:	bd eb 59    	jsr	0xeb59
    cba8:	20 0d       	bra	0x0xcbb7
    cbaa:	7d 01 1e    	tst	0x11e
    cbad:	26 08       	bne	0x0xcbb7
    cbaf:	86 02       	ldaa	#0x2
    cbb1:	b7 01 1e    	staa	0x11e
    cbb4:	bd ea e4    	jsr	0xeae4
    cbb7:	7d 01 7f    	tst	0x17f
    cbba:	27 10       	beq	0x0xcbcc
    cbbc:	bd ed 5a    	jsr	0xed5a
    cbbf:	4f          	clra
    cbc0:	b7 01 7f    	staa	0x17f
    cbc3:	b7 01 7e    	staa	0x17e
    cbc6:	b7 01 1a    	staa	0x11a
    cbc9:	7e d5 2f    	jmp	0xd52f
    cbcc:	7d 01 7e    	tst	0x17e
    cbcf:	27 0c       	beq	0x0xcbdd
    cbd1:	bd ec 90    	jsr	0xec90
    cbd4:	7f 01 7e    	clr	0x17e
    cbd7:	7f 01 1a    	clr	0x11a
    cbda:	7e d5 2f    	jmp	0xd52f
    cbdd:	7d 01 1a    	tst	0x11a
    cbe0:	26 03       	bne	0x0xcbe5
    cbe2:	7e d5 2f    	jmp	0xd52f
    cbe5:	c6 02       	ldab	#0x2
    cbe7:	18 ce 00 b8 	ldy	#0xb8
    cbeb:	96 f9       	ldaa	*0xf9
    cbed:	81 04       	cmpa	#0x4
    cbef:	26 07       	bne	0x0xcbf8
    cbf1:	14 fb 80    	bset	*0xfb, #0x80
    cbf4:	18 ce 50 38 	ldy	#0x5038
    cbf8:	ce 01 20    	ldx	#0x120
    cbfb:	f1 01 1e    	cmpb	0x11e
    cbfe:	27 0a       	beq	0x0xcc0a
    cc00:	18 08       	iny
    cc02:	cb 04       	addb	#0x4
    cc04:	08          	inx
    cc05:	08          	inx
    cc06:	08          	inx
    cc07:	08          	inx
    cc08:	20 f1       	bra	0x0xcbfb
    cc0a:	18 a6 00    	ldaa	0x0,y
    cc0d:	7d 01 1a    	tst	0x11a
    cc10:	2b 20       	bmi	0x0xcc32
    cc12:	4c          	inca
    cc13:	f6 01 11    	ldab	0x111
    cc16:	c5 04       	bitb	#0x4
    cc18:	27 02       	beq	0x0xcc1c
    cc1a:	8b 09       	adda	#0x9
    cc1c:	84 7f       	anda	#0x7f
    cc1e:	18 a7 00    	staa	0x0,y
    cc21:	bd eb bc    	jsr	0xebbc
    cc24:	bd ca 43    	jsr	0xca43
    cc27:	7f 01 1a    	clr	0x11a
    cc2a:	86 03       	ldaa	#0x3
    cc2c:	b7 01 1c    	staa	0x11c
    cc2f:	7e d5 2f    	jmp	0xd52f
    cc32:	4a          	deca
    cc33:	f6 01 11    	ldab	0x111
    cc36:	c5 04       	bitb	#0x4
    cc38:	27 02       	beq	0x0xcc3c
    cc3a:	80 09       	suba	#0x9
    cc3c:	84 7f       	anda	#0x7f
    cc3e:	20 de       	bra	0x0xcc1e
    cc40:	7d 01 1c    	tst	0x11c
    cc43:	2a 03       	bpl	0x0xcc48
    cc45:	7e e6 2b    	jmp	0xe62b
    cc48:	7d 01 1e    	tst	0x11e
    cc4b:	26 08       	bne	0x0xcc55
    cc4d:	7d 01 1c    	tst	0x11c
    cc50:	27 1f       	beq	0x0xcc71
    cc52:	7e b2 70    	jmp	0xb270
    cc55:	7d 01 1c    	tst	0x11c
    cc58:	27 27       	beq	0x0xcc81
    cc5a:	cc 01 20    	ldd	#0x120
    cc5d:	fb 01 1e    	addb	0x11e
    cc60:	8f          	xgdx
    cc61:	bd ea a8    	jsr	0xeaa8
    cc64:	7a 01 1e    	dec	0x11e
    cc67:	7a 01 1c    	dec	0x11c
    cc6a:	26 15       	bne	0x0xcc81
    cc6c:	bd eb 26    	jsr	0xeb26
    cc6f:	20 10       	bra	0x0xcc81
    cc71:	7d 01 1e    	tst	0x11e
    cc74:	26 0b       	bne	0x0xcc81
    cc76:	86 13       	ldaa	#0x13
    cc78:	b7 01 1e    	staa	0x11e
    cc7b:	bd ea e4    	jsr	0xeae4
    cc7e:	7f 01 6b    	clr	0x16b
    cc81:	7d 01 7f    	tst	0x17f
    cc84:	27 10       	beq	0x0xcc96
    cc86:	bd ec e8    	jsr	0xece8
    cc89:	4f          	clra
    cc8a:	b7 01 7f    	staa	0x17f
    cc8d:	b7 01 7e    	staa	0x17e
    cc90:	b7 01 1a    	staa	0x11a
    cc93:	7e d5 2f    	jmp	0xd52f
    cc96:	7d 01 1a    	tst	0x11a
    cc99:	26 03       	bne	0x0xcc9e
    cc9b:	7e d5 2f    	jmp	0xd52f
    cc9e:	b6 01 1e    	ldaa	0x11e
    cca1:	81 13       	cmpa	#0x13
    cca3:	26 23       	bne	0x0xccc8
    cca5:	ce e4 9a    	ldx	#0xe49a
    cca8:	18 ce 01 30 	ldy	#0x130
    ccac:	d6 a7       	ldab	*0xa7
    ccae:	bd c7 66    	jsr	0xc766
    ccb1:	d7 a7       	stab	*0xa7
    ccb3:	bd ec 6f    	jsr	0xec6f
    ccb6:	7f 01 1a    	clr	0x11a
    ccb9:	86 04       	ldaa	#0x4
    ccbb:	b7 01 1c    	staa	0x11c
    ccbe:	96 a7       	ldaa	*0xa7
    ccc0:	c6 a7       	ldab	#0xa7
    ccc2:	bd b0 fc    	jsr	0xb0fc
    ccc5:	7e d5 2f    	jmp	0xd52f
    ccc8:	81 19       	cmpa	#0x19
    ccca:	26 32       	bne	0x0xccfe
    cccc:	ce e5 54    	ldx	#0xe554
    cccf:	18 ce 01 37 	ldy	#0x137
    ccd3:	d6 a6       	ldab	*0xa6
    ccd5:	7d 01 1a    	tst	0x11a
    ccd8:	2b 1e       	bmi	0x0xccf8
    ccda:	5c          	incb
    ccdb:	c1 03       	cmpb	#0x3
    ccdd:	25 02       	bcs	0x0xcce1
    ccdf:	c6 02       	ldab	#0x2
    cce1:	d7 a6       	stab	*0xa6
    cce3:	bd ec 5b    	jsr	0xec5b
    cce6:	7f 01 1a    	clr	0x11a
    cce9:	86 03       	ldaa	#0x3
    cceb:	b7 01 1c    	staa	0x11c
    ccee:	96 a6       	ldaa	*0xa6
    ccf0:	c6 a6       	ldab	#0xa6
    ccf2:	bd b0 fc    	jsr	0xb0fc
    ccf5:	7e d5 2f    	jmp	0xd52f
    ccf8:	5a          	decb
    ccf9:	2a e6       	bpl	0x0xcce1
    ccfb:	5f          	clrb
    ccfc:	20 e3       	bra	0x0xcce1
    ccfe:	ce e4 ba    	ldx	#0xe4ba
    cd01:	18 ce 01 3b 	ldy	#0x13b
    cd05:	d6 a5       	ldab	*0xa5
    cd07:	bd c7 e0    	jsr	0xc7e0
    cd0a:	d7 a5       	stab	*0xa5
    cd0c:	bd ec 6f    	jsr	0xec6f
    cd0f:	7f 01 1a    	clr	0x11a
    cd12:	86 04       	ldaa	#0x4
    cd14:	b7 01 1c    	staa	0x11c
    cd17:	96 a5       	ldaa	*0xa5
    cd19:	c6 a5       	ldab	#0xa5
    cd1b:	bd b0 fc    	jsr	0xb0fc
    cd1e:	7e d5 2f    	jmp	0xd52f
    cd21:	7f 01 7f    	clr	0x17f
    cd24:	7f 01 7e    	clr	0x17e
    cd27:	7f 01 1a    	clr	0x11a
    cd2a:	7e d5 2f    	jmp	0xd52f
    cd2d:	7d 01 1c    	tst	0x11c
    cd30:	2a 03       	bpl	0x0xcd35
    cd32:	7e e6 89    	jmp	0xe689
    cd35:	7d 01 1e    	tst	0x11e
    cd38:	26 08       	bne	0x0xcd42
    cd3a:	7d 01 1c    	tst	0x11c
    cd3d:	27 1f       	beq	0x0xcd5e
    cd3f:	7e b2 70    	jmp	0xb270
    cd42:	7d 01 1c    	tst	0x11c
    cd45:	27 24       	beq	0x0xcd6b
    cd47:	cc 01 20    	ldd	#0x120
    cd4a:	fb 01 1e    	addb	0x11e
    cd4d:	8f          	xgdx
    cd4e:	bd ea a8    	jsr	0xeaa8
    cd51:	7a 01 1e    	dec	0x11e
    cd54:	7a 01 1c    	dec	0x11c
    cd57:	26 12       	bne	0x0xcd6b
    cd59:	bd eb 59    	jsr	0xeb59
    cd5c:	20 0d       	bra	0x0xcd6b
    cd5e:	7d 01 1e    	tst	0x11e
    cd61:	26 08       	bne	0x0xcd6b
    cd63:	86 12       	ldaa	#0x12
    cd65:	b7 01 1e    	staa	0x11e
    cd68:	bd ea e4    	jsr	0xeae4
    cd6b:	7d 01 7f    	tst	0x17f
    cd6e:	27 10       	beq	0x0xcd80
    cd70:	bd ed 5a    	jsr	0xed5a
    cd73:	4f          	clra
    cd74:	b7 01 7f    	staa	0x17f
    cd77:	b7 01 7e    	staa	0x17e
    cd7a:	b7 01 1a    	staa	0x11a
    cd7d:	7e d5 2f    	jmp	0xd52f
    cd80:	7d 01 1a    	tst	0x11a
    cd83:	26 03       	bne	0x0xcd88
    cd85:	7e d5 2f    	jmp	0xd52f
    cd88:	b6 01 1e    	ldaa	0x11e
    cd8b:	81 12       	cmpa	#0x12
    cd8d:	26 16       	bne	0x0xcda5
    cd8f:	d6 21       	ldab	*0x21
    cd91:	c8 02       	eorb	#0x2
    cd93:	d7 21       	stab	*0x21
    cd95:	c4 02       	andb	#0x2
    cd97:	54          	lsrb
    cd98:	18 ce 01 30 	ldy	#0x130
    cd9c:	ce da ec    	ldx	#0xdaec
    cd9f:	bd ec 5b    	jsr	0xec5b
    cda2:	7e b9 23    	jmp	0xb923
    cda5:	81 16       	cmpa	#0x16
    cda7:	26 17       	bne	0x0xcdc0
    cda9:	d6 21       	ldab	*0x21
    cdab:	c8 04       	eorb	#0x4
    cdad:	d7 21       	stab	*0x21
    cdaf:	c4 04       	andb	#0x4
    cdb1:	54          	lsrb
    cdb2:	54          	lsrb
    cdb3:	18 ce 01 34 	ldy	#0x134
    cdb7:	ce da ec    	ldx	#0xdaec
    cdba:	bd ec 5b    	jsr	0xec5b
    cdbd:	7e b9 23    	jmp	0xb923
    cdc0:	81 1a       	cmpa	#0x1a
    cdc2:	26 30       	bne	0x0xcdf4
    cdc4:	b6 01 1a    	ldaa	0x11a
    cdc7:	2b 23       	bmi	0x0xcdec
    cdc9:	96 22       	ldaa	*0x22
    cdcb:	4c          	inca
    cdcc:	81 7f       	cmpa	#0x7f
    cdce:	25 02       	bcs	0x0xcdd2
    cdd0:	86 7f       	ldaa	#0x7f
    cdd2:	97 22       	staa	*0x22
    cdd4:	ce 01 38    	ldx	#0x138
    cdd7:	bd eb f8    	jsr	0xebf8
    cdda:	7f 01 1a    	clr	0x11a
    cddd:	86 03       	ldaa	#0x3
    cddf:	b7 01 1c    	staa	0x11c
    cde2:	96 22       	ldaa	*0x22
    cde4:	c6 22       	ldab	#0x22
    cde6:	bd b0 fc    	jsr	0xb0fc
    cde9:	7e d5 2f    	jmp	0xd52f
    cdec:	96 22       	ldaa	*0x22
    cdee:	4a          	deca
    cdef:	2a e1       	bpl	0x0xcdd2
    cdf1:	4f          	clra
    cdf2:	20 de       	bra	0x0xcdd2
    cdf4:	b6 01 1a    	ldaa	0x11a
    cdf7:	2b 1f       	bmi	0x0xce18
    cdf9:	96 23       	ldaa	*0x23
    cdfb:	4c          	inca
    cdfc:	84 7f       	anda	#0x7f
    cdfe:	97 23       	staa	*0x23
    ce00:	ce 01 3c    	ldx	#0x13c
    ce03:	bd eb bc    	jsr	0xebbc
    ce06:	7f 01 1a    	clr	0x11a
    ce09:	86 03       	ldaa	#0x3
    ce0b:	b7 01 1c    	staa	0x11c
    ce0e:	96 23       	ldaa	*0x23
    ce10:	c6 23       	ldab	#0x23
    ce12:	bd b0 fc    	jsr	0xb0fc
    ce15:	7e d5 2f    	jmp	0xd52f
    ce18:	96 23       	ldaa	*0x23
    ce1a:	4a          	deca
    ce1b:	84 7f       	anda	#0x7f
    ce1d:	20 df       	bra	0x0xcdfe
    ce1f:	39          	rts
    ce20:	7d 01 1c    	tst	0x11c
    ce23:	2a 03       	bpl	0x0xce28
    ce25:	7e e7 bb    	jmp	0xe7bb
    ce28:	7d 01 1e    	tst	0x11e
    ce2b:	26 08       	bne	0x0xce35
    ce2d:	7d 01 1c    	tst	0x11c
    ce30:	27 1f       	beq	0x0xce51
    ce32:	7e b2 70    	jmp	0xb270
    ce35:	7d 01 1c    	tst	0x11c
    ce38:	27 24       	beq	0x0xce5e
    ce3a:	cc 01 20    	ldd	#0x120
    ce3d:	fb 01 1e    	addb	0x11e
    ce40:	8f          	xgdx
    ce41:	bd ea a8    	jsr	0xeaa8
    ce44:	7a 01 1e    	dec	0x11e
    ce47:	7a 01 1c    	dec	0x11c
    ce4a:	26 12       	bne	0x0xce5e
    ce4c:	bd eb 26    	jsr	0xeb26
    ce4f:	20 0d       	bra	0x0xce5e
    ce51:	7d 01 1e    	tst	0x11e
    ce54:	26 08       	bne	0x0xce5e
    ce56:	86 13       	ldaa	#0x13
    ce58:	b7 01 1e    	staa	0x11e
    ce5b:	bd ea e4    	jsr	0xeae4
    ce5e:	7d 01 7f    	tst	0x17f
    ce61:	27 0d       	beq	0x0xce70
    ce63:	4f          	clra
    ce64:	b7 01 7f    	staa	0x17f
    ce67:	b7 01 7e    	staa	0x17e
    ce6a:	b7 01 1a    	staa	0x11a
    ce6d:	7e d5 2f    	jmp	0xd52f
    ce70:	7d 01 1a    	tst	0x11a
    ce73:	26 03       	bne	0x0xce78
    ce75:	7e d5 2f    	jmp	0xd52f
    ce78:	7d 01 1a    	tst	0x11a
    ce7b:	2b 26       	bmi	0x0xcea3
    ce7d:	96 f0       	ldaa	*0xf0
    ce7f:	84 07       	anda	#0x7
    ce81:	4c          	inca
    ce82:	84 07       	anda	#0x7
    ce84:	97 f0       	staa	*0xf0
    ce86:	bd ad 9e    	jsr	0xad9e
    ce89:	96 f0       	ldaa	*0xf0
    ce8b:	b7 7f fa    	staa	0x7ffa
    ce8e:	4c          	inca
    ce8f:	ce 01 31    	ldx	#0x131
    ce92:	bd eb bc    	jsr	0xebbc
    ce95:	7f 01 1a    	clr	0x11a
    ce98:	86 03       	ldaa	#0x3
    ce9a:	b7 01 1c    	staa	0x11c
    ce9d:	bd ad c4    	jsr	0xadc4
    cea0:	7e d5 2f    	jmp	0xd52f
    cea3:	96 f0       	ldaa	*0xf0
    cea5:	84 07       	anda	#0x7
    cea7:	4a          	deca
    cea8:	20 d8       	bra	0x0xce82
    ceaa:	7d 01 1c    	tst	0x11c
    cead:	2a 03       	bpl	0x0xceb2
    ceaf:	7e e9 b4    	jmp	0xe9b4
    ceb2:	7d 01 1e    	tst	0x11e
    ceb5:	26 08       	bne	0x0xcebf
    ceb7:	7d 01 1c    	tst	0x11c
    ceba:	27 1a       	beq	0x0xced6
    cebc:	7e b2 70    	jmp	0xb270
    cebf:	7d 01 1c    	tst	0x11c
    cec2:	27 1a       	beq	0x0xcede
    cec4:	cc 01 20    	ldd	#0x120
    cec7:	fb 01 1e    	addb	0x11e
    ceca:	8f          	xgdx
    cecb:	bd ea a8    	jsr	0xeaa8
    cece:	7a 01 1c    	dec	0x11c
    ced1:	bd ea e4    	jsr	0xeae4
    ced4:	20 08       	bra	0x0xcede
    ced6:	86 0f       	ldaa	#0xf
    ced8:	b7 01 1e    	staa	0x11e
    cedb:	bd ea e4    	jsr	0xeae4
    cede:	7d 01 7f    	tst	0x17f
    cee1:	27 0d       	beq	0x0xcef0
    cee3:	4f          	clra
    cee4:	b7 01 7f    	staa	0x17f
    cee7:	b7 01 7e    	staa	0x17e
    ceea:	b7 01 1a    	staa	0x11a
    ceed:	7e d5 2f    	jmp	0xd52f
    cef0:	7d 01 7e    	tst	0x17e
    cef3:	27 29       	beq	0x0xcf1e
    cef5:	b6 01 1e    	ldaa	0x11e
    cef8:	81 0f       	cmpa	#0xf
    cefa:	26 11       	bne	0x0xcf0d
    cefc:	86 1f       	ldaa	#0x1f
    cefe:	b7 01 1e    	staa	0x11e
    cf01:	bd ea e4    	jsr	0xeae4
    cf04:	7f 01 7e    	clr	0x17e
    cf07:	7f 01 1a    	clr	0x11a
    cf0a:	7e d5 2f    	jmp	0xd52f
    cf0d:	86 0f       	ldaa	#0xf
    cf0f:	b7 01 1e    	staa	0x11e
    cf12:	bd ea e4    	jsr	0xeae4
    cf15:	7f 01 7e    	clr	0x17e
    cf18:	7f 01 1a    	clr	0x11a
    cf1b:	7e d5 2f    	jmp	0xd52f
    cf1e:	7d 01 1a    	tst	0x11a
    cf21:	26 03       	bne	0x0xcf26
    cf23:	7e d5 2f    	jmp	0xd52f
    cf26:	b6 01 1e    	ldaa	0x11e
    cf29:	81 0f       	cmpa	#0xf
    cf2b:	26 13       	bne	0x0xcf40
    cf2d:	b6 01 2f    	ldaa	0x12f
    cf30:	81 43       	cmpa	#0x43
    cf32:	26 06       	bne	0x0xcf3a
    cf34:	4c          	inca
    cf35:	b7 01 2f    	staa	0x12f
    cf38:	20 17       	bra	0x0xcf51
    cf3a:	4a          	deca
    cf3b:	b7 01 2f    	staa	0x12f
    cf3e:	20 11       	bra	0x0xcf51
    cf40:	b6 01 3f    	ldaa	0x13f
    cf43:	81 41       	cmpa	#0x41
    cf45:	26 06       	bne	0x0xcf4d
    cf47:	4c          	inca
    cf48:	b7 01 3f    	staa	0x13f
    cf4b:	20 04       	bra	0x0xcf51
    cf4d:	4a          	deca
    cf4e:	b7 01 3f    	staa	0x13f
    cf51:	7f 01 1a    	clr	0x11a
    cf54:	86 01       	ldaa	#0x1
    cf56:	b7 01 1c    	staa	0x11c
    cf59:	7e d5 2f    	jmp	0xd52f
    cf5c:	7d 01 1c    	tst	0x11c
    cf5f:	2a 03       	bpl	0x0xcf64
    cf61:	7e e6 ff    	jmp	0xe6ff
    cf64:	7d 01 1e    	tst	0x11e
    cf67:	26 08       	bne	0x0xcf71
    cf69:	7d 01 1c    	tst	0x11c
    cf6c:	27 1f       	beq	0x0xcf8d
    cf6e:	7e b2 70    	jmp	0xb270
    cf71:	7d 01 1c    	tst	0x11c
    cf74:	27 24       	beq	0x0xcf9a
    cf76:	cc 01 20    	ldd	#0x120
    cf79:	fb 01 1e    	addb	0x11e
    cf7c:	8f          	xgdx
    cf7d:	bd ea a8    	jsr	0xeaa8
    cf80:	7a 01 1e    	dec	0x11e
    cf83:	7a 01 1c    	dec	0x11c
    cf86:	26 12       	bne	0x0xcf9a
    cf88:	bd eb 59    	jsr	0xeb59
    cf8b:	20 0d       	bra	0x0xcf9a
    cf8d:	7d 01 1e    	tst	0x11e
    cf90:	26 08       	bne	0x0xcf9a
    cf92:	86 13       	ldaa	#0x13
    cf94:	b7 01 1e    	staa	0x11e
    cf97:	bd ea e4    	jsr	0xeae4
    cf9a:	7d 01 1a    	tst	0x11a
    cf9d:	26 09       	bne	0x0xcfa8
    cf9f:	7f 01 7f    	clr	0x17f
    cfa2:	7f 01 7e    	clr	0x17e
    cfa5:	7e d5 2f    	jmp	0xd52f
    cfa8:	7f 01 1a    	clr	0x11a
    cfab:	7f 01 7f    	clr	0x17f
    cfae:	7f 01 7e    	clr	0x17e
    cfb1:	7d 00 d0    	tst	0xd0
    cfb4:	27 05       	beq	0x0xcfbb
    cfb6:	7f 00 d0    	clr	0xd0
    cfb9:	20 08       	bra	0x0xcfc3
    cfbb:	7d 10 29    	tst	0x1029
    cfbe:	2a fb       	bpl	0x0xcfbb
    cfc0:	b6 10 2a    	ldaa	0x102a
    cfc3:	5f          	clrb
    cfc4:	01          	nop
    cfc5:	01          	nop
    cfc6:	01          	nop
    cfc7:	01          	nop
    cfc8:	f7 10 42    	stab	0x1042
    cfcb:	01          	nop
    cfcc:	01          	nop
    cfcd:	01          	nop
    cfce:	01          	nop
    cfcf:	01          	nop
    cfd0:	01          	nop
    cfd1:	01          	nop
    cfd2:	86 fe       	ldaa	#0xfe
    cfd4:	b7 10 2a    	staa	0x102a
    cfd7:	bd eb 09    	jsr	0xeb09
    cfda:	c6 20       	ldab	#0x20
    cfdc:	ce 01 20    	ldx	#0x120
    cfdf:	18 ce d0 3f 	ldy	#0xd03f
    cfe3:	18 a6 00    	ldaa	0x0,y
    cfe6:	a7 00       	staa	0x0,x
    cfe8:	08          	inx
    cfe9:	18 08       	iny
    cfeb:	5a          	decb
    cfec:	26 f5       	bne	0x0xcfe3
    cfee:	7f 01 1e    	clr	0x11e
    cff1:	86 20       	ldaa	#0x20
    cff3:	b7 01 1c    	staa	0x11c
    cff6:	ce 10 23    	ldx	#0x1023
    cff9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xcff9
    cffd:	bd ea 5d    	jsr	0xea5d
    d000:	7d 01 1c    	tst	0x11c
    d003:	27 05       	beq	0x0xd00a
    d005:	ce 10 23    	ldx	#0x1023
    d008:	20 ef       	bra	0x0xcff9
    d00a:	86 ff       	ldaa	#0xff
    d00c:	97 f8       	staa	*0xf8
    d00e:	c6 06       	ldab	#0x6
    d010:	ce ff ff    	ldx	#0xffff
    d013:	01          	nop
    d014:	01          	nop
    d015:	01          	nop
    d016:	01          	nop
    d017:	09          	dex
    d018:	26 f9       	bne	0x0xd013
    d01a:	5a          	decb
    d01b:	26 f6       	bne	0x0xd013
    d01d:	c6 06       	ldab	#0x6
    d01f:	44          	lsra
    d020:	27 04       	beq	0x0xd026
    d022:	97 f8       	staa	*0xf8
    d024:	20 ed       	bra	0x0xd013
    d026:	97 f8       	staa	*0xf8
    d028:	97 fb       	staa	*0xfb
    d02a:	97 f2       	staa	*0xf2
    d02c:	97 ff       	staa	*0xff
    d02e:	b7 01 1b    	staa	0x11b
    d031:	bd a3 59    	jsr	0xa359
    d034:	bd a3 70    	jsr	0xa370
    d037:	86 80       	ldaa	#0x80
    d039:	b7 01 1c    	staa	0x11c
    d03c:	7e 82 83    	jmp	0x8283
    d03f:	20 54       	bra	0x0xd095
    d041:	55          	.byte	0x55
    d042:	4e          	.byte	0x4e
    d043:	49          	rola
    d044:	4e          	.byte	0x4e
    d045:	47          	asra
    d046:	20 2e       	bra	0x0xd076
    d048:	2e 2e       	bgt	0x0xd078
    d04a:	2e 2e       	bgt	0x0xd07a
    d04c:	20 20       	bra	0x0xd06e
    d04e:	20 20       	bra	0x0xd070
    d050:	20 20       	bra	0x0xd072
    d052:	20 20       	bra	0x0xd074
    d054:	20 20       	bra	0x0xd076
    d056:	20 20       	bra	0x0xd078
    d058:	20 20       	bra	0x0xd07a
    d05a:	20 20       	bra	0x0xd07c
    d05c:	20 20       	bra	0x0xd07e
    d05e:	20 7e       	bra	0x0xd0de
    d060:	d5 2f       	bitb	*0x2f
    d062:	7d 01 1c    	tst	0x11c
    d065:	2a 03       	bpl	0x0xd06a
    d067:	7e e1 5c    	jmp	0xe15c
    d06a:	7d 01 1e    	tst	0x11e
    d06d:	26 08       	bne	0x0xd077
    d06f:	7d 01 1c    	tst	0x11c
    d072:	27 1f       	beq	0x0xd093
    d074:	7e b2 70    	jmp	0xb270
    d077:	7d 01 1c    	tst	0x11c
    d07a:	27 24       	beq	0x0xd0a0
    d07c:	cc 01 20    	ldd	#0x120
    d07f:	fb 01 1e    	addb	0x11e
    d082:	8f          	xgdx
    d083:	bd ea a8    	jsr	0xeaa8
    d086:	7a 01 1e    	dec	0x11e
    d089:	7a 01 1c    	dec	0x11c
    d08c:	26 12       	bne	0x0xd0a0
    d08e:	bd eb 26    	jsr	0xeb26
    d091:	20 0d       	bra	0x0xd0a0
    d093:	7d 01 1e    	tst	0x11e
    d096:	26 08       	bne	0x0xd0a0
    d098:	86 13       	ldaa	#0x13
    d09a:	b7 01 1e    	staa	0x11e
    d09d:	bd ea e4    	jsr	0xeae4
    d0a0:	7d 01 7f    	tst	0x17f
    d0a3:	27 1e       	beq	0x0xd0c3
    d0a5:	2a 10       	bpl	0x0xd0b7
    d0a7:	bd ec e8    	jsr	0xece8
    d0aa:	4f          	clra
    d0ab:	b7 01 7f    	staa	0x17f
    d0ae:	b7 01 7e    	staa	0x17e
    d0b1:	b7 01 1a    	staa	0x11a
    d0b4:	7e d5 2f    	jmp	0xd52f
    d0b7:	b6 01 1e    	ldaa	0x11e
    d0ba:	81 19       	cmpa	#0x19
    d0bc:	27 ec       	beq	0x0xd0aa
    d0be:	bd ec e8    	jsr	0xece8
    d0c1:	20 e7       	bra	0x0xd0aa
    d0c3:	7d 01 1a    	tst	0x11a
    d0c6:	26 03       	bne	0x0xd0cb
    d0c8:	7e d5 2f    	jmp	0xd52f
    d0cb:	b6 01 1e    	ldaa	0x11e
    d0ce:	81 13       	cmpa	#0x13
    d0d0:	26 2b       	bne	0x0xd0fd
    d0d2:	b6 01 1a    	ldaa	0x11a
    d0d5:	2b 1f       	bmi	0x0xd0f6
    d0d7:	96 50       	ldaa	*0x50
    d0d9:	4c          	inca
    d0da:	84 7f       	anda	#0x7f
    d0dc:	97 50       	staa	*0x50
    d0de:	ce 01 31    	ldx	#0x131
    d0e1:	bd eb bc    	jsr	0xebbc
    d0e4:	7f 01 1a    	clr	0x11a
    d0e7:	86 03       	ldaa	#0x3
    d0e9:	b7 01 1c    	staa	0x11c
    d0ec:	96 50       	ldaa	*0x50
    d0ee:	c6 50       	ldab	#0x50
    d0f0:	bd b0 fc    	jsr	0xb0fc
    d0f3:	7e d5 2f    	jmp	0xd52f
    d0f6:	96 50       	ldaa	*0x50
    d0f8:	4a          	deca
    d0f9:	84 7f       	anda	#0x7f
    d0fb:	20 df       	bra	0x0xd0dc
    d0fd:	81 19       	cmpa	#0x19
    d0ff:	26 2b       	bne	0x0xd12c
    d101:	b6 01 1a    	ldaa	0x11a
    d104:	2b 1f       	bmi	0x0xd125
    d106:	96 98       	ldaa	*0x98
    d108:	4c          	inca
    d109:	84 7f       	anda	#0x7f
    d10b:	97 98       	staa	*0x98
    d10d:	ce 01 37    	ldx	#0x137
    d110:	bd eb bc    	jsr	0xebbc
    d113:	7f 01 1a    	clr	0x11a
    d116:	86 03       	ldaa	#0x3
    d118:	b7 01 1c    	staa	0x11c
    d11b:	96 98       	ldaa	*0x98
    d11d:	c6 98       	ldab	#0x98
    d11f:	bd b0 fc    	jsr	0xb0fc
    d122:	7e d5 2f    	jmp	0xd52f
    d125:	96 98       	ldaa	*0x98
    d127:	4a          	deca
    d128:	84 7f       	anda	#0x7f
    d12a:	20 df       	bra	0x0xd10b
    d12c:	7f 01 1a    	clr	0x11a
    d12f:	7e d5 2f    	jmp	0xd52f
    d132:	7d 01 1c    	tst	0x11c
    d135:	2a 03       	bpl	0x0xd13a
    d137:	7e e8 0d    	jmp	0xe80d
    d13a:	7d 01 1e    	tst	0x11e
    d13d:	26 08       	bne	0x0xd147
    d13f:	7d 01 1c    	tst	0x11c
    d142:	27 1f       	beq	0x0xd163
    d144:	7e b2 70    	jmp	0xb270
    d147:	7d 01 1c    	tst	0x11c
    d14a:	27 1f       	beq	0x0xd16b
    d14c:	cc 01 20    	ldd	#0x120
    d14f:	fb 01 1e    	addb	0x11e
    d152:	8f          	xgdx
    d153:	bd ea a8    	jsr	0xeaa8
    d156:	7a 01 1e    	dec	0x11e
    d159:	7a 01 1c    	dec	0x11c
    d15c:	26 0d       	bne	0x0xd16b
    d15e:	bd eb 59    	jsr	0xeb59
    d161:	20 08       	bra	0x0xd16b
    d163:	86 16       	ldaa	#0x16
    d165:	b7 01 1e    	staa	0x11e
    d168:	bd ea e4    	jsr	0xeae4
    d16b:	7f 01 7e    	clr	0x17e
    d16e:	7d 01 7f    	tst	0x17f
    d171:	27 10       	beq	0x0xd183
    d173:	bd ed 5a    	jsr	0xed5a
    d176:	4f          	clra
    d177:	b7 01 7f    	staa	0x17f
    d17a:	b7 01 7e    	staa	0x17e
    d17d:	b7 01 1a    	staa	0x11a
    d180:	7e d5 2f    	jmp	0xd52f
    d183:	7d 01 1a    	tst	0x11a
    d186:	26 03       	bne	0x0xd18b
    d188:	7e d5 2f    	jmp	0xd52f
    d18b:	ce 50 01    	ldx	#0x5001
    d18e:	b6 01 6b    	ldaa	0x16b
    d191:	c6 04       	ldab	#0x4
    d193:	3d          	mul
    d194:	3a          	abx
    d195:	b6 01 1e    	ldaa	0x11e
    d198:	81 12       	cmpa	#0x12
    d19a:	26 69       	bne	0x0xd205
    d19c:	a6 00       	ldaa	0x0,x
    d19e:	7d 01 1a    	tst	0x11a
    d1a1:	2b 5c       	bmi	0x0xd1ff
    d1a3:	4c          	inca
    d1a4:	81 04       	cmpa	#0x4
    d1a6:	25 02       	bcs	0x0xd1aa
    d1a8:	86 03       	ldaa	#0x3
    d1aa:	a7 00       	staa	0x0,x
    d1ac:	8b 41       	adda	#0x41
    d1ae:	b7 01 32    	staa	0x132
    d1b1:	7f 01 1a    	clr	0x11a
    d1b4:	86 01       	ldaa	#0x1
    d1b6:	b7 01 1c    	staa	0x11c
    d1b9:	a6 01       	ldaa	0x1,x
    d1bb:	e6 00       	ldab	0x0,x
    d1bd:	c1 01       	cmpb	#0x1
    d1bf:	23 18       	bls	0x0xd1d9
    d1c1:	d7 f9       	stab	*0xf9
    d1c3:	bd ad c4    	jsr	0xadc4
    d1c6:	c6 04       	ldab	#0x4
    d1c8:	d7 f9       	stab	*0xf9
    d1ca:	bd a5 50    	jsr	0xa550
    d1cd:	bd a5 6b    	jsr	0xa56b
    d1d0:	bd ad c4    	jsr	0xadc4
    d1d3:	bd a8 3b    	jsr	0xa83b
    d1d6:	7e d2 da    	jmp	0xd2da
    d1d9:	4f          	clra
    d1da:	b7 10 22    	staa	0x1022
    d1dd:	b6 10 2d    	ldaa	0x102d
    d1e0:	36          	psha
    d1e1:	84 7f       	anda	#0x7f
    d1e3:	b7 10 2d    	staa	0x102d
    d1e6:	a6 00       	ldaa	0x0,x
    d1e8:	e6 01       	ldab	0x1,x
    d1ea:	bd 79 00    	jsr	0x7900
    d1ed:	32          	pula
    d1ee:	b7 10 2d    	staa	0x102d
    d1f1:	86 80       	ldaa	#0x80
    d1f3:	b7 10 22    	staa	0x1022
    d1f6:	bd a5 6b    	jsr	0xa56b
    d1f9:	bd a8 3b    	jsr	0xa83b
    d1fc:	7e d2 da    	jmp	0xd2da
    d1ff:	4a          	deca
    d200:	2a a8       	bpl	0x0xd1aa
    d202:	4f          	clra
    d203:	20 a5       	bra	0x0xd1aa
    d205:	81 16       	cmpa	#0x16
    d207:	26 55       	bne	0x0xd25e
    d209:	a6 01       	ldaa	0x1,x
    d20b:	7d 01 1a    	tst	0x11a
    d20e:	2b 3c       	bmi	0x0xd24c
    d210:	4c          	inca
    d211:	84 7f       	anda	#0x7f
    d213:	81 51       	cmpa	#0x51
    d215:	26 04       	bne	0x0xd21b
    d217:	86 53       	ldaa	#0x53
    d219:	20 06       	bra	0x0xd221
    d21b:	81 52       	cmpa	#0x52
    d21d:	26 02       	bne	0x0xd221
    d21f:	86 53       	ldaa	#0x53
    d221:	a7 01       	staa	0x1,x
    d223:	4c          	inca
    d224:	3c          	pshx
    d225:	ce 01 34    	ldx	#0x134
    d228:	bd eb bc    	jsr	0xebbc
    d22b:	38          	pulx
    d22c:	a6 01       	ldaa	0x1,x
    d22e:	e6 00       	ldab	0x0,x
    d230:	c1 01       	cmpb	#0x1
    d232:	23 a5       	bls	0x0xd1d9
    d234:	d7 f9       	stab	*0xf9
    d236:	bd ad c4    	jsr	0xadc4
    d239:	c6 04       	ldab	#0x4
    d23b:	d7 f9       	stab	*0xf9
    d23d:	bd a5 50    	jsr	0xa550
    d240:	bd a5 6b    	jsr	0xa56b
    d243:	bd ad c4    	jsr	0xadc4
    d246:	bd a8 3b    	jsr	0xa83b
    d249:	7e d2 da    	jmp	0xd2da
    d24c:	4a          	deca
    d24d:	84 7f       	anda	#0x7f
    d24f:	81 52       	cmpa	#0x52
    d251:	26 04       	bne	0x0xd257
    d253:	86 50       	ldaa	#0x50
    d255:	20 ca       	bra	0x0xd221
    d257:	81 51       	cmpa	#0x51
    d259:	26 c6       	bne	0x0xd221
    d25b:	4a          	deca
    d25c:	20 c3       	bra	0x0xd221
    d25e:	81 1a       	cmpa	#0x1a
    d260:	26 3e       	bne	0x0xd2a0
    d262:	a6 02       	ldaa	0x2,x
    d264:	84 40       	anda	#0x40
    d266:	b7 01 7d    	staa	0x17d
    d269:	a6 02       	ldaa	0x2,x
    d26b:	84 3f       	anda	#0x3f
    d26d:	f6 50 00    	ldab	0x5000
    d270:	c1 06       	cmpb	#0x6
    d272:	27 02       	beq	0x0xd276
    d274:	20 64       	bra	0x0xd2da
    d276:	7d 01 1a    	tst	0x11a
    d279:	2b 19       	bmi	0x0xd294
    d27b:	4c          	inca
    d27c:	7a 50 23    	dec	0x5023
    d27f:	2a 04       	bpl	0x0xd285
    d281:	4a          	deca
    d282:	7c 50 23    	inc	0x5023
    d285:	ba 01 7d    	oraa	0x17d
    d288:	a7 02       	staa	0x2,x
    d28a:	84 3f       	anda	#0x3f
    d28c:	ce 01 38    	ldx	#0x138
    d28f:	bd eb bc    	jsr	0xebbc
    d292:	20 46       	bra	0x0xd2da
    d294:	7c 50 23    	inc	0x5023
    d297:	4a          	deca
    d298:	2a eb       	bpl	0x0xd285
    d29a:	4c          	inca
    d29b:	7a 50 23    	dec	0x5023
    d29e:	20 e5       	bra	0x0xd285
    d2a0:	f6 50 00    	ldab	0x5000
    d2a3:	c1 06       	cmpb	#0x6
    d2a5:	27 22       	beq	0x0xd2c9
    d2a7:	a6 03       	ldaa	0x3,x
    d2a9:	7d 01 1a    	tst	0x11a
    d2ac:	2b 18       	bmi	0x0xd2c6
    d2ae:	4c          	inca
    d2af:	84 7f       	anda	#0x7f
    d2b1:	a7 03       	staa	0x3,x
    d2b3:	36          	psha
    d2b4:	ce 01 3c    	ldx	#0x13c
    d2b7:	bd eb bc    	jsr	0xebbc
    d2ba:	32          	pula
    d2bb:	d6 d6       	ldab	*0xd6
    d2bd:	3d          	mul
    d2be:	05          	asld
    d2bf:	c6 ac       	ldab	#0xac
    d2c1:	bd b0 fc    	jsr	0xb0fc
    d2c4:	20 14       	bra	0x0xd2da
    d2c6:	4a          	deca
    d2c7:	20 e6       	bra	0x0xd2af
    d2c9:	e6 03       	ldab	0x3,x
    d2cb:	5c          	incb
    d2cc:	c4 01       	andb	#0x1
    d2ce:	e7 03       	stab	0x3,x
    d2d0:	ce e9 37    	ldx	#0xe937
    d2d3:	18 ce 01 3c 	ldy	#0x13c
    d2d7:	bd ec 5b    	jsr	0xec5b
    d2da:	7f 01 1a    	clr	0x11a
    d2dd:	86 03       	ldaa	#0x3
    d2df:	b7 01 1c    	staa	0x11c
    d2e2:	14 fb 80    	bset	*0xfb, #0x80
    d2e5:	7e d5 2f    	jmp	0xd52f
    d2e8:	7d 01 1c    	tst	0x11c
    d2eb:	2a 03       	bpl	0x0xd2f0
    d2ed:	7e e9 3d    	jmp	0xe93d
    d2f0:	7d 01 1e    	tst	0x11e
    d2f3:	26 08       	bne	0x0xd2fd
    d2f5:	7d 01 1c    	tst	0x11c
    d2f8:	27 1f       	beq	0x0xd319
    d2fa:	7e b2 70    	jmp	0xb270
    d2fd:	7d 01 1c    	tst	0x11c
    d300:	27 1f       	beq	0x0xd321
    d302:	cc 01 20    	ldd	#0x120
    d305:	fb 01 1e    	addb	0x11e
    d308:	8f          	xgdx
    d309:	bd ea a8    	jsr	0xeaa8
    d30c:	7a 01 1e    	dec	0x11e
    d30f:	7a 01 1c    	dec	0x11c
    d312:	26 0d       	bne	0x0xd321
    d314:	bd eb 26    	jsr	0xeb26
    d317:	20 08       	bra	0x0xd321
    d319:	86 13       	ldaa	#0x13
    d31b:	b7 01 1e    	staa	0x11e
    d31e:	bd ea e4    	jsr	0xeae4
    d321:	7d 01 7f    	tst	0x17f
    d324:	27 0d       	beq	0x0xd333
    d326:	4f          	clra
    d327:	b7 01 7f    	staa	0x17f
    d32a:	b7 01 7e    	staa	0x17e
    d32d:	b7 01 1a    	staa	0x11a
    d330:	7e d5 2f    	jmp	0xd52f
    d333:	7d 01 7e    	tst	0x17e
    d336:	27 0c       	beq	0x0xd344
    d338:	7f 01 7f    	clr	0x17f
    d33b:	7f 01 7e    	clr	0x17e
    d33e:	7f 01 1a    	clr	0x11a
    d341:	7e d5 2f    	jmp	0xd52f
    d344:	7d 01 1a    	tst	0x11a
    d347:	26 03       	bne	0x0xd34c
    d349:	7e d5 2f    	jmp	0xd52f
    d34c:	b6 01 6b    	ldaa	0x16b
    d34f:	c6 04       	ldab	#0x4
    d351:	3d          	mul
    d352:	ce 50 03    	ldx	#0x5003
    d355:	3a          	abx
    d356:	e6 00       	ldab	0x0,x
    d358:	c4 3f       	andb	#0x3f
    d35a:	f7 01 7d    	stab	0x17d
    d35d:	e6 00       	ldab	0x0,x
    d35f:	c4 40       	andb	#0x40
    d361:	c8 40       	eorb	#0x40
    d363:	37          	pshb
    d364:	fa 01 7d    	orab	0x17d
    d367:	e7 00       	stab	0x0,x
    d369:	32          	pula
    d36a:	4d          	tsta
    d36b:	27 02       	beq	0x0xd36f
    d36d:	86 01       	ldaa	#0x1
    d36f:	36          	psha
    d370:	4d          	tsta
    d371:	26 31       	bne	0x0xd3a4
    d373:	7d 00 d0    	tst	0xd0
    d376:	27 05       	beq	0x0xd37d
    d378:	7f 00 d0    	clr	0xd0
    d37b:	20 08       	bra	0x0xd385
    d37d:	7d 10 29    	tst	0x1029
    d380:	2a fb       	bpl	0x0xd37d
    d382:	f6 10 2a    	ldab	0x102a
    d385:	f6 01 6b    	ldab	0x16b
    d388:	ce 50 50    	ldx	#0x5050
    d38b:	3a          	abx
    d38c:	e6 00       	ldab	0x0,x
    d38e:	53          	comb
    d38f:	01          	nop
    d390:	01          	nop
    d391:	01          	nop
    d392:	01          	nop
    d393:	f7 10 42    	stab	0x1042
    d396:	01          	nop
    d397:	01          	nop
    d398:	01          	nop
    d399:	01          	nop
    d39a:	01          	nop
    d39b:	01          	nop
    d39c:	01          	nop
    d39d:	c6 8c       	ldab	#0x8c
    d39f:	f7 10 2a    	stab	0x102a
    d3a2:	20 07       	bra	0x0xd3ab
    d3a4:	c6 44       	ldab	#0x44
    d3a6:	96 44       	ldaa	*0x44
    d3a8:	bd b0 fc    	jsr	0xb0fc
    d3ab:	33          	pulb
    d3ac:	18 ce 01 31 	ldy	#0x131
    d3b0:	ce e9 ae    	ldx	#0xe9ae
    d3b3:	bd ec 5b    	jsr	0xec5b
    d3b6:	7f 01 1a    	clr	0x11a
    d3b9:	86 03       	ldaa	#0x3
    d3bb:	b7 01 1c    	staa	0x11c
    d3be:	14 fb 80    	bset	*0xfb, #0x80
    d3c1:	7e d5 2f    	jmp	0xd52f
    d3c4:	7d 01 1c    	tst	0x11c
    d3c7:	2a 03       	bpl	0x0xd3cc
    d3c9:	7e e9 f5    	jmp	0xe9f5
    d3cc:	7d 01 1e    	tst	0x11e
    d3cf:	26 08       	bne	0x0xd3d9
    d3d1:	7d 01 1c    	tst	0x11c
    d3d4:	27 24       	beq	0x0xd3fa
    d3d6:	7e b2 70    	jmp	0xb270
    d3d9:	7d 01 1c    	tst	0x11c
    d3dc:	27 24       	beq	0x0xd402
    d3de:	cc 01 20    	ldd	#0x120
    d3e1:	fb 01 1e    	addb	0x11e
    d3e4:	8f          	xgdx
    d3e5:	bd ea a8    	jsr	0xeaa8
    d3e8:	7a 01 1e    	dec	0x11e
    d3eb:	7a 01 1c    	dec	0x11c
    d3ee:	26 12       	bne	0x0xd402
    d3f0:	86 1e       	ldaa	#0x1e
    d3f2:	b7 01 1e    	staa	0x11e
    d3f5:	bd ea e4    	jsr	0xeae4
    d3f8:	20 08       	bra	0x0xd402
    d3fa:	86 1e       	ldaa	#0x1e
    d3fc:	b7 01 1e    	staa	0x11e
    d3ff:	bd ea e4    	jsr	0xeae4
    d402:	7d 01 1a    	tst	0x11a
    d405:	26 0a       	bne	0x0xd411
    d407:	4f          	clra
    d408:	b7 01 7f    	staa	0x17f
    d40b:	b7 01 7e    	staa	0x17e
    d40e:	7e d5 2f    	jmp	0xd52f
    d411:	86 0c       	ldaa	#0xc
    d413:	b7 10 2d    	staa	0x102d
    d416:	4f          	clra
    d417:	97 f2       	staa	*0xf2
    d419:	97 f3       	staa	*0xf3
    d41b:	97 f4       	staa	*0xf4
    d41d:	97 f5       	staa	*0xf5
    d41f:	97 f6       	staa	*0xf6
    d421:	97 f7       	staa	*0xf7
    d423:	97 f8       	staa	*0xf8
    d425:	bd d4 d9    	jsr	0xd4d9
    d428:	4f          	clra
    d429:	b7 10 22    	staa	0x1022
    d42c:	96 99       	ldaa	*0x99
    d42e:	80 41       	suba	#0x41
    d430:	97 f9       	staa	*0xf9
    d432:	bd ad c4    	jsr	0xadc4
    d435:	96 9a       	ldaa	*0x9a
    d437:	80 41       	suba	#0x41
    d439:	36          	psha
    d43a:	ce f9 c1    	ldx	#0xf9c1
    d43d:	18 ce 78 00 	ldy	#0x7800
    d441:	a6 00       	ldaa	0x0,x
    d443:	18 a7 00    	staa	0x0,y
    d446:	18 08       	iny
    d448:	08          	inx
    d449:	8c fb bf    	cpx	#0xfbbf
    d44c:	23 f3       	bls	0x0xd441
    d44e:	18 ce 00 00 	ldy	#0x0
    d452:	ce d4 66    	ldx	#0xd466
    d455:	a6 00       	ldaa	0x0,x
    d457:	18 a7 00    	staa	0x0,y
    d45a:	18 08       	iny
    d45c:	08          	inx
    d45d:	8c d4 d8    	cpx	#0xd4d8
    d460:	25 f3       	bcs	0x0xd455
    d462:	0f          	sei
    d463:	7e 00 00    	jmp	0x0
    d466:	32          	pula
    d467:	4d          	tsta
    d468:	26 0c       	bne	0x0xd476
    d46a:	b6 10 00    	ldaa	0x1000
    d46d:	84 9f       	anda	#0x9f
    d46f:	8a 20       	oraa	#0x20
    d471:	b7 10 00    	staa	0x1000
    d474:	20 0a       	bra	0x0xd480
    d476:	b6 10 00    	ldaa	0x1000
    d479:	84 9f       	anda	#0x9f
    d47b:	8a 40       	oraa	#0x40
    d47d:	b7 10 00    	staa	0x1000
    d480:	ce 20 00    	ldx	#0x2000
    d483:	18 ce 80 00 	ldy	#0x8000
    d487:	86 aa       	ldaa	#0xaa
    d489:	b7 d5 55    	staa	0xd555
    d48c:	86 55       	ldaa	#0x55
    d48e:	b7 aa aa    	staa	0xaaaa
    d491:	86 a0       	ldaa	#0xa0
    d493:	b7 d5 55    	staa	0xd555
    d496:	86 80       	ldaa	#0x80
    d498:	e6 00       	ldab	0x0,x
    d49a:	18 e7 00    	stab	0x0,y
    d49d:	08          	inx
    d49e:	18 08       	iny
    d4a0:	4a          	deca
    d4a1:	26 f5       	bne	0x0xd498
    d4a3:	86 08       	ldaa	#0x8
    d4a5:	b7 10 23    	staa	0x1023
    d4a8:	fc 10 0e    	ldd	0x100e
    d4ab:	c3 5d c0    	addd	#0x5dc0
    d4ae:	fd 10 1e    	std	0x101e
    d4b1:	01          	nop
    d4b2:	01          	nop
    d4b3:	b6 10 23    	ldaa	0x1023
    d4b6:	85 08       	bita	#0x8
    d4b8:	27 f7       	beq	0x0xd4b1
    d4ba:	8c 78 00    	cpx	#0x7800
    d4bd:	25 c8       	bcs	0x0xd487
    d4bf:	22 06       	bhi	0x0xd4c7
    d4c1:	18 ce fe 00 	ldy	#0xfe00
    d4c5:	20 c0       	bra	0x0xd487
    d4c7:	18 8c 00 00 	cpy	#0x0
    d4cb:	26 ba       	bne	0x0xd487
    d4cd:	b6 10 00    	ldaa	0x1000
    d4d0:	84 8f       	anda	#0x8f
    d4d2:	b7 10 00    	staa	0x1000
    d4d5:	7e 80 00    	jmp	0x8000
    d4d8:	01          	nop
    d4d9:	ce 10 23    	ldx	#0x1023
    d4dc:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd4dc
    d4e0:	bd eb 09    	jsr	0xeb09
    d4e3:	ce 01 20    	ldx	#0x120
    d4e6:	18 ce d5 0f 	ldy	#0xd50f
    d4ea:	c6 20       	ldab	#0x20
    d4ec:	18 a6 00    	ldaa	0x0,y
    d4ef:	a7 00       	staa	0x0,x
    d4f1:	08          	inx
    d4f2:	18 08       	iny
    d4f4:	5a          	decb
    d4f5:	26 f5       	bne	0x0xd4ec
    d4f7:	7f 01 1e    	clr	0x11e
    d4fa:	86 20       	ldaa	#0x20
    d4fc:	b7 01 1c    	staa	0x11c
    d4ff:	ce 10 23    	ldx	#0x1023
    d502:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd502
    d506:	bd ea 5d    	jsr	0xea5d
    d509:	7d 01 1c    	tst	0x11c
    d50c:	26 f1       	bne	0x0xd4ff
    d50e:	39          	rts
    d50f:	20 57       	bra	0x0xd568
    d511:	52          	.byte	0x52
    d512:	49          	rola
    d513:	54          	lsrb
    d514:	49          	rola
    d515:	4e          	.byte	0x4e
    d516:	47          	asra
    d517:	20 42       	bra	0x0xd55b
    d519:	41          	.byte	0x41
    d51a:	4e          	.byte	0x4e
    d51b:	4b          	.byte	0x4b
    d51c:	20 2e       	bra	0x0xd54c
    d51e:	2e 20       	bgt	0x0xd540
    d520:	54          	lsrb
    d521:	41          	.byte	0x41
    d522:	4b          	.byte	0x4b
    d523:	45          	.byte	0x45
    d524:	20 35       	bra	0x0xd55b
    d526:	20 2e       	bra	0x0xd556
    d528:	2e 2e       	bgt	0x0xd558
    d52a:	2e 2e       	bgt	0x0xd55a
    d52c:	2e 2e       	bgt	0x0xd55c
    d52e:	2e fe       	bgt	0x0xd52e
    d530:	01          	nop
    d531:	c0 bc       	subb	#0xbc
    d533:	01          	nop
    d534:	c2 26       	sbcb	#0x26
    d536:	03          	fdiv
    d537:	7e 97 90    	jmp	0x9790
    d53a:	18 fe 01 6c 	ldy	0x16c
    d53e:	18 3c       	pshy
    d540:	7e 92 be    	jmp	0x92be
    d543:	7f 00 ff    	clr	0xff
    d546:	7d 01 1d    	tst	0x11d
    d549:	27 2f       	beq	0x0xd57a
    d54b:	ce 10 23    	ldx	#0x1023
    d54e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd54e
    d552:	86 f7       	ldaa	#0xf7
    d554:	b4 10 00    	anda	0x1000
    d557:	b7 10 00    	staa	0x1000
    d55a:	86 0c       	ldaa	#0xc
    d55c:	b7 10 47    	staa	0x1047
    d55f:	86 80       	ldaa	#0x80
    d561:	ba 10 00    	oraa	0x1000
    d564:	b7 10 00    	staa	0x1000
    d567:	01          	nop
    d568:	88 80       	eora	#0x80
    d56a:	b7 10 00    	staa	0x1000
    d56d:	7f 01 1d    	clr	0x11d
    d570:	bd ea d5    	jsr	0xead5
    d573:	ce 10 23    	ldx	#0x1023
    d576:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd576
    d57a:	bd eb 09    	jsr	0xeb09
    d57d:	ce 01 20    	ldx	#0x120
    d580:	c6 10       	ldab	#0x10
    d582:	96 f9       	ldaa	*0xf9
    d584:	81 04       	cmpa	#0x4
    d586:	25 06       	bcs	0x0xd58e
    d588:	18 ce d5 fd 	ldy	#0xd5fd
    d58c:	20 07       	bra	0x0xd595
    d58e:	14 d1 80    	bset	*0xd1, #0x80
    d591:	18 ce d5 ed 	ldy	#0xd5ed
    d595:	18 a6 00    	ldaa	0x0,y
    d598:	a7 00       	staa	0x0,x
    d59a:	08          	inx
    d59b:	18 08       	iny
    d59d:	5a          	decb
    d59e:	26 f5       	bne	0x0xd595
    d5a0:	96 f9       	ldaa	*0xf9
    d5a2:	81 04       	cmpa	#0x4
    d5a4:	27 30       	beq	0x0xd5d6
    d5a6:	8b 41       	adda	#0x41
    d5a8:	b7 01 2c    	staa	0x12c
    d5ab:	96 f9       	ldaa	*0xf9
    d5ad:	81 01       	cmpa	#0x1
    d5af:	23 05       	bls	0x0xd5b6
    d5b1:	86 41       	ldaa	#0x41
    d5b3:	b7 01 28    	staa	0x128
    d5b6:	96 fa       	ldaa	*0xfa
    d5b8:	4c          	inca
    d5b9:	ce 01 2d    	ldx	#0x12d
    d5bc:	bd eb bc    	jsr	0xebbc
    d5bf:	08          	inx
    d5c0:	18 ce 00 c0 	ldy	#0xc0
    d5c4:	c6 10       	ldab	#0x10
    d5c6:	18 a6 00    	ldaa	0x0,y
    d5c9:	84 7f       	anda	#0x7f
    d5cb:	a7 00       	staa	0x0,x
    d5cd:	08          	inx
    d5ce:	18 08       	iny
    d5d0:	5a          	decb
    d5d1:	26 f3       	bne	0x0xd5c6
    d5d3:	7e ea 46    	jmp	0xea46
    d5d6:	b6 01 6a    	ldaa	0x16a
    d5d9:	4c          	inca
    d5da:	ce 01 2d    	ldx	#0x12d
    d5dd:	bd eb bc    	jsr	0xebbc
    d5e0:	08          	inx
    d5e1:	cc 50 00    	ldd	#0x5000
    d5e4:	c3 00 40    	addd	#0x40
    d5e7:	18 8f       	xgdy
    d5e9:	c6 10       	ldab	#0x10
    d5eb:	20 d9       	bra	0x0xd5c6
    d5ed:	50          	negb
    d5ee:	41          	.byte	0x41
    d5ef:	54          	lsrb
    d5f0:	43          	coma
    d5f1:	48          	asla
    d5f2:	20 20       	bra	0x0xd614
    d5f4:	52          	.byte	0x52
    d5f5:	4f          	clra
    d5f6:	4d          	tsta
    d5f7:	20 20       	bra	0x0xd619
    d5f9:	20 20       	bra	0x0xd61b
    d5fb:	20 20       	bra	0x0xd61d
    d5fd:	4d          	tsta
    d5fe:	55          	.byte	0x55
    d5ff:	4c          	inca
    d600:	54          	lsrb
    d601:	49          	rola
    d602:	20 20       	bra	0x0xd624
    d604:	20 20       	bra	0x0xd626
    d606:	20 20       	bra	0x0xd628
    d608:	20 20       	bra	0x0xd62a
    d60a:	20 20       	bra	0x0xd62c
    d60c:	20 7d       	bra	0x0xd68b
    d60e:	01          	nop
    d60f:	7e 27 61    	jmp	0x2761
    d612:	7f 01 7e    	clr	0x17e
    d615:	96 f9       	ldaa	*0xf9
    d617:	81 04       	cmpa	#0x4
    d619:	25 1e       	bcs	0x0xd639
    d61b:	8b 41       	adda	#0x41
    d61d:	b7 01 2c    	staa	0x12c
    d620:	b6 01 6a    	ldaa	0x16a
    d623:	4c          	inca
    d624:	ce 01 2d    	ldx	#0x12d
    d627:	bd eb bc    	jsr	0xebbc
    d62a:	b6 01 6a    	ldaa	0x16a
    d62d:	c6 50       	ldab	#0x50
    d62f:	3d          	mul
    d630:	c3 20 00    	addd	#0x2000
    d633:	c3 00 40    	addd	#0x40
    d636:	8f          	xgdx
    d637:	20 1a       	bra	0x0xd653
    d639:	8b 41       	adda	#0x41
    d63b:	b7 01 2c    	staa	0x12c
    d63e:	96 fa       	ldaa	*0xfa
    d640:	4c          	inca
    d641:	ce 01 2d    	ldx	#0x12d
    d644:	bd eb bc    	jsr	0xebbc
    d647:	96 fa       	ldaa	*0xfa
    d649:	c6 b0       	ldab	#0xb0
    d64b:	3d          	mul
    d64c:	c3 20 00    	addd	#0x2000
    d64f:	c3 00 a0    	addd	#0xa0
    d652:	8f          	xgdx
    d653:	18 ce 01 30 	ldy	#0x130
    d657:	c6 10       	ldab	#0x10
    d659:	a6 00       	ldaa	0x0,x
    d65b:	84 7f       	anda	#0x7f
    d65d:	18 a7 00    	staa	0x0,y
    d660:	08          	inx
    d661:	18 08       	iny
    d663:	5a          	decb
    d664:	26 f3       	bne	0x0xd659
    d666:	ce 10 23    	ldx	#0x1023
    d669:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd669
    d66d:	bd eb 09    	jsr	0xeb09
    d670:	7e ea 46    	jmp	0xea46
    d673:	7d 01 1d    	tst	0x11d
    d676:	26 0a       	bne	0x0xd682
    d678:	bd ec 33    	jsr	0xec33
    d67b:	ce 10 23    	ldx	#0x1023
    d67e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd67e
    d682:	bd eb 09    	jsr	0xeb09
    d685:	ce 01 20    	ldx	#0x120
    d688:	18 ce d7 12 	ldy	#0xd712
    d68c:	c6 20       	ldab	#0x20
    d68e:	18 a6 00    	ldaa	0x0,y
    d691:	a7 00       	staa	0x0,x
    d693:	08          	inx
    d694:	18 08       	iny
    d696:	5a          	decb
    d697:	26 f5       	bne	0x0xd68e
    d699:	96 f9       	ldaa	*0xf9
    d69b:	8b 41       	adda	#0x41
    d69d:	b7 01 25    	staa	0x125
    d6a0:	96 f9       	ldaa	*0xf9
    d6a2:	81 02       	cmpa	#0x2
    d6a4:	24 07       	bcc	0x0xd6ad
    d6a6:	86 02       	ldaa	#0x2
    d6a8:	97 f9       	staa	*0xf9
    d6aa:	bd ad c4    	jsr	0xadc4
    d6ad:	8b 41       	adda	#0x41
    d6af:	b7 01 2c    	staa	0x12c
    d6b2:	96 fa       	ldaa	*0xfa
    d6b4:	7d 00 d1    	tst	0xd1
    d6b7:	2b 3a       	bmi	0x0xd6f3
    d6b9:	b6 01 6a    	ldaa	0x16a
    d6bc:	7d 00 fb    	tst	0xfb
    d6bf:	27 32       	beq	0x0xd6f3
    d6c1:	2b 30       	bmi	0x0xd6f3
    d6c3:	86 04       	ldaa	#0x4
    d6c5:	97 f9       	staa	*0xf9
    d6c7:	bd ad c4    	jsr	0xadc4
    d6ca:	ce 50 01    	ldx	#0x5001
    d6cd:	b6 01 6b    	ldaa	0x16b
    d6d0:	c6 04       	ldab	#0x4
    d6d2:	3d          	mul
    d6d3:	3a          	abx
    d6d4:	a6 00       	ldaa	0x0,x
    d6d6:	8b 41       	adda	#0x41
    d6d8:	b7 01 25    	staa	0x125
    d6db:	a6 01       	ldaa	0x1,x
    d6dd:	36          	psha
    d6de:	a6 00       	ldaa	0x0,x
    d6e0:	81 02       	cmpa	#0x2
    d6e2:	24 02       	bcc	0x0xd6e6
    d6e4:	86 02       	ldaa	#0x2
    d6e6:	97 f9       	staa	*0xf9
    d6e8:	bd ad c4    	jsr	0xadc4
    d6eb:	8b 41       	adda	#0x41
    d6ed:	b7 01 2c    	staa	0x12c
    d6f0:	32          	pula
    d6f1:	97 fa       	staa	*0xfa
    d6f3:	4c          	inca
    d6f4:	ce 01 26    	ldx	#0x126
    d6f7:	bd eb bc    	jsr	0xebbc
    d6fa:	a7 07       	staa	0x7,x
    d6fc:	09          	dex
    d6fd:	a6 00       	ldaa	0x0,x
    d6ff:	a7 07       	staa	0x7,x
    d701:	09          	dex
    d702:	a6 00       	ldaa	0x0,x
    d704:	a7 07       	staa	0x7,x
    d706:	96 f9       	ldaa	*0xf9
    d708:	81 04       	cmpa	#0x4
    d70a:	27 03       	beq	0x0xd70f
    d70c:	7e d6 47    	jmp	0xd647
    d70f:	7e d5 d6    	jmp	0xd5d6
    d712:	53          	comb
    d713:	41          	.byte	0x41
    d714:	56          	rorb
    d715:	45          	.byte	0x45
    d716:	20 20       	bra	0x0xd738
    d718:	20 20       	bra	0x0xd73a
    d71a:	20 20       	bra	0x0xd73c
    d71c:	3e          	wai
    d71d:	20 20       	bra	0x0xd73f
    d71f:	20 20       	bra	0x0xd741
    d721:	20 20       	bra	0x0xd743
    d723:	20 20       	bra	0x0xd745
    d725:	20 20       	bra	0x0xd747
    d727:	20 20       	bra	0x0xd749
    d729:	20 20       	bra	0x0xd74b
    d72b:	20 20       	bra	0x0xd74d
    d72d:	20 20       	bra	0x0xd74f
    d72f:	20 20       	bra	0x0xd751
    d731:	20 7d       	bra	0x0xd7b0
    d733:	01          	nop
    d734:	1d 26 0a    	bclr	0x26,x, #0x0a
    d737:	bd ec 33    	jsr	0xec33
    d73a:	ce 10 23    	ldx	#0x1023
    d73d:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd73d
    d741:	bd eb 09    	jsr	0xeb09
    d744:	ce 01 20    	ldx	#0x120
    d747:	18 ce d7 86 	ldy	#0xd786
    d74b:	c6 20       	ldab	#0x20
    d74d:	96 f9       	ldaa	*0xf9
    d74f:	81 04       	cmpa	#0x4
    d751:	26 04       	bne	0x0xd757
    d753:	18 ce d7 a6 	ldy	#0xd7a6
    d757:	18 a6 00    	ldaa	0x0,y
    d75a:	a7 00       	staa	0x0,x
    d75c:	08          	inx
    d75d:	18 08       	iny
    d75f:	5a          	decb
    d760:	26 f5       	bne	0x0xd757
    d762:	96 f9       	ldaa	*0xf9
    d764:	81 04       	cmpa	#0x4
    d766:	27 11       	beq	0x0xd779
    d768:	8b 41       	adda	#0x41
    d76a:	b7 01 2c    	staa	0x12c
    d76d:	96 fa       	ldaa	*0xfa
    d76f:	4c          	inca
    d770:	ce 01 2d    	ldx	#0x12d
    d773:	bd eb bc    	jsr	0xebbc
    d776:	7e d5 bf    	jmp	0xd5bf
    d779:	b6 01 6a    	ldaa	0x16a
    d77c:	4c          	inca
    d77d:	ce 01 2d    	ldx	#0x12d
    d780:	bd eb bc    	jsr	0xebbc
    d783:	7e d5 d6    	jmp	0xd5d6
    d786:	4e          	.byte	0x4e
    d787:	41          	.byte	0x41
    d788:	4d          	tsta
    d789:	45          	.byte	0x45
    d78a:	20 50       	bra	0x0xd7dc
    d78c:	41          	.byte	0x41
    d78d:	54          	lsrb
    d78e:	43          	coma
    d78f:	48          	asla
    d790:	20 20       	bra	0x0xd7b2
    d792:	20 20       	bra	0x0xd7b4
    d794:	20 20       	bra	0x0xd7b6
    d796:	20 20       	bra	0x0xd7b8
    d798:	20 20       	bra	0x0xd7ba
    d79a:	20 20       	bra	0x0xd7bc
    d79c:	20 20       	bra	0x0xd7be
    d79e:	20 20       	bra	0x0xd7c0
    d7a0:	20 20       	bra	0x0xd7c2
    d7a2:	20 20       	bra	0x0xd7c4
    d7a4:	20 20       	bra	0x0xd7c6
    d7a6:	4e          	.byte	0x4e
    d7a7:	41          	.byte	0x41
    d7a8:	4d          	tsta
    d7a9:	45          	.byte	0x45
    d7aa:	20 4d       	bra	0x0xd7f9
    d7ac:	55          	.byte	0x55
    d7ad:	4c          	inca
    d7ae:	54          	lsrb
    d7af:	49          	rola
    d7b0:	20 20       	bra	0x0xd7d2
    d7b2:	20 20       	bra	0x0xd7d4
    d7b4:	20 20       	bra	0x0xd7d6
    d7b6:	20 20       	bra	0x0xd7d8
    d7b8:	20 20       	bra	0x0xd7da
    d7ba:	20 20       	bra	0x0xd7dc
    d7bc:	20 20       	bra	0x0xd7de
    d7be:	20 20       	bra	0x0xd7e0
    d7c0:	20 20       	bra	0x0xd7e2
    d7c2:	20 20       	bra	0x0xd7e4
    d7c4:	20 20       	bra	0x0xd7e6
    d7c6:	bd eb 09    	jsr	0xeb09
    d7c9:	ce 01 20    	ldx	#0x120
    d7cc:	18 ce d8 04 	ldy	#0xd804
    d7d0:	c6 20       	ldab	#0x20
    d7d2:	18 a6 00    	ldaa	0x0,y
    d7d5:	a7 00       	staa	0x0,x
    d7d7:	08          	inx
    d7d8:	18 08       	iny
    d7da:	5a          	decb
    d7db:	26 f5       	bne	0x0xd7d2
    d7dd:	ce d8 24    	ldx	#0xd824
    d7e0:	f6 01 6f    	ldab	0x16f
    d7e3:	18 ce 01 30 	ldy	#0x130
    d7e7:	bd ec 6f    	jsr	0xec6f
    d7ea:	ce d8 68    	ldx	#0xd868
    d7ed:	d6 93       	ldab	*0x93
    d7ef:	18 ce 01 36 	ldy	#0x136
    d7f3:	bd ec 6f    	jsr	0xec6f
    d7f6:	ce d8 7c    	ldx	#0xd87c
    d7f9:	5f          	clrb
    d7fa:	18 ce 01 3d 	ldy	#0x13d
    d7fe:	bd ec 5b    	jsr	0xec5b
    d801:	7e ea 46    	jmp	0xea46
    d804:	43          	coma
    d805:	48          	asla
    d806:	41          	.byte	0x41
    d807:	4e          	.byte	0x4e
    d808:	20 20       	bra	0x0xd82a
    d80a:	54          	lsrb
    d80b:	55          	.byte	0x55
    d80c:	4e          	.byte	0x4e
    d80d:	45          	.byte	0x45
    d80e:	20 20       	bra	0x0xd830
    d810:	49          	rola
    d811:	4e          	.byte	0x4e
    d812:	49          	rola
    d813:	54          	lsrb
    d814:	20 20       	bra	0x0xd836
    d816:	20 20       	bra	0x0xd838
    d818:	20 20       	bra	0x0xd83a
    d81a:	20 20       	bra	0x0xd83c
    d81c:	20 20       	bra	0x0xd83e
    d81e:	20 20       	bra	0x0xd840
    d820:	20 20       	bra	0x0xd842
    d822:	20 20       	bra	0x0xd844
    d824:	20 20       	bra	0x0xd846
    d826:	20 31       	bra	0x0xd859
    d828:	20 20       	bra	0x0xd84a
    d82a:	20 32       	bra	0x0xd85e
    d82c:	20 20       	bra	0x0xd84e
    d82e:	20 33       	bra	0x0xd863
    d830:	20 20       	bra	0x0xd852
    d832:	20 34       	bra	0x0xd868
    d834:	20 20       	bra	0x0xd856
    d836:	20 35       	bra	0x0xd86d
    d838:	20 20       	bra	0x0xd85a
    d83a:	20 36       	bra	0x0xd872
    d83c:	20 20       	bra	0x0xd85e
    d83e:	20 37       	bra	0x0xd877
    d840:	20 20       	bra	0x0xd862
    d842:	20 38       	bra	0x0xd87c
    d844:	20 20       	bra	0x0xd866
    d846:	20 39       	bra	0x0xd881
    d848:	20 20       	bra	0x0xd86a
    d84a:	31          	ins
    d84b:	30          	tsx
    d84c:	20 20       	bra	0x0xd86e
    d84e:	31          	ins
    d84f:	31          	ins
    d850:	20 20       	bra	0x0xd872
    d852:	31          	ins
    d853:	32          	pula
    d854:	20 20       	bra	0x0xd876
    d856:	31          	ins
    d857:	33          	pulb
    d858:	20 20       	bra	0x0xd87a
    d85a:	31          	ins
    d85b:	34          	des
    d85c:	20 20       	bra	0x0xd87e
    d85e:	31          	ins
    d85f:	35          	txs
    d860:	20 20       	bra	0x0xd882
    d862:	31          	ins
    d863:	36          	psha
    d864:	4f          	clra
    d865:	4d          	tsta
    d866:	4e          	.byte	0x4e
    d867:	49          	rola
    d868:	20 4f       	bra	0x0xd8b9
    d86a:	46          	rora
    d86b:	46          	rora
    d86c:	20 39       	bra	0x0xd8a7
    d86e:	30          	tsx
    d86f:	25 20       	bcs	0x0xd891
    d871:	39          	rts
    d872:	35          	txs
    d873:	25 20       	bcs	0x0xd895
    d875:	39          	rts
    d876:	38          	pulx
    d877:	25 31       	bcs	0x0xd8aa
    d879:	30          	tsx
    d87a:	30          	tsx
    d87b:	25 20       	bcs	0x0xd89d
    d87d:	4e          	.byte	0x4e
    d87e:	4f          	clra
    d87f:	59          	rolb
    d880:	45          	.byte	0x45
    d881:	53          	comb
    d882:	bd eb 09    	jsr	0xeb09
    d885:	ce 01 20    	ldx	#0x120
    d888:	18 ce d8 be 	ldy	#0xd8be
    d88c:	c6 20       	ldab	#0x20
    d88e:	18 a6 00    	ldaa	0x0,y
    d891:	a7 00       	staa	0x0,x
    d893:	08          	inx
    d894:	18 08       	iny
    d896:	5a          	decb
    d897:	26 f5       	bne	0x0xd88e
    d899:	ce d8 de    	ldx	#0xd8de
    d89c:	f6 01 70    	ldab	0x170
    d89f:	18 ce 01 30 	ldy	#0x130
    d8a3:	bd ec 6f    	jsr	0xec6f
    d8a6:	ce d8 ea    	ldx	#0xd8ea
    d8a9:	d6 94       	ldab	*0x94
    d8ab:	18 ce 01 36 	ldy	#0x136
    d8af:	bd ec 6f    	jsr	0xec6f
    d8b2:	b6 01 71    	ldaa	0x171
    d8b5:	ce 01 3c    	ldx	#0x13c
    d8b8:	bd eb bc    	jsr	0xebbc
    d8bb:	7e ea 46    	jmp	0xea46
    d8be:	4d          	tsta
    d8bf:	50          	negb
    d8c0:	52          	.byte	0x52
    d8c1:	4f          	clra
    d8c2:	20 20       	bra	0x0xd8e4
    d8c4:	4b          	.byte	0x4b
    d8c5:	4e          	.byte	0x4e
    d8c6:	4f          	clra
    d8c7:	42          	.byte	0x42
    d8c8:	20 20       	bra	0x0xd8ea
    d8ca:	4c          	inca
    d8cb:	43          	coma
    d8cc:	44          	lsra
    d8cd:	20 20       	bra	0x0xd8ef
    d8cf:	20 20       	bra	0x0xd8f1
    d8d1:	20 20       	bra	0x0xd8f3
    d8d3:	20 20       	bra	0x0xd8f5
    d8d5:	20 20       	bra	0x0xd8f7
    d8d7:	20 20       	bra	0x0xd8f9
    d8d9:	20 20       	bra	0x0xd8fb
    d8db:	20 20       	bra	0x0xd8fd
    d8dd:	20 20       	bra	0x0xd8ff
    d8df:	4f          	clra
    d8e0:	46          	rora
    d8e1:	46          	rora
    d8e2:	4f          	clra
    d8e3:	4e          	.byte	0x4e
    d8e4:	20 31       	bra	0x0xd917
    d8e6:	4f          	clra
    d8e7:	4e          	.byte	0x4e
    d8e8:	20 32       	bra	0x0xd91c
    d8ea:	4a          	deca
    d8eb:	55          	.byte	0x55
    d8ec:	4d          	tsta
    d8ed:	50          	negb
    d8ee:	45          	.byte	0x45
    d8ef:	44          	lsra
    d8f0:	49          	rola
    d8f1:	54          	lsrb
    d8f2:	4d          	tsta
    d8f3:	54          	lsrb
    d8f4:	43          	coma
    d8f5:	48          	asla
    d8f6:	bd eb 09    	jsr	0xeb09
    d8f9:	96 f9       	ldaa	*0xf9
    d8fb:	81 04       	cmpa	#0x4
    d8fd:	27 0b       	beq	0x0xd90a
    d8ff:	ce 01 20    	ldx	#0x120
    d902:	18 ce d9 41 	ldy	#0xd941
    d906:	c6 20       	ldab	#0x20
    d908:	20 09       	bra	0x0xd913
    d90a:	ce 01 20    	ldx	#0x120
    d90d:	18 ce d9 61 	ldy	#0xd961
    d911:	c6 20       	ldab	#0x20
    d913:	18 a6 00    	ldaa	0x0,y
    d916:	a7 00       	staa	0x0,x
    d918:	08          	inx
    d919:	18 08       	iny
    d91b:	5a          	decb
    d91c:	26 f5       	bne	0x0xd913
    d91e:	96 f9       	ldaa	*0xf9
    d920:	81 04       	cmpa	#0x4
    d922:	26 0c       	bne	0x0xd930
    d924:	b6 01 6a    	ldaa	0x16a
    d927:	4c          	inca
    d928:	ce 01 3c    	ldx	#0x13c
    d92b:	bd eb bc    	jsr	0xebbc
    d92e:	20 0e       	bra	0x0xd93e
    d930:	8b 41       	adda	#0x41
    d932:	b7 01 3b    	staa	0x13b
    d935:	96 fa       	ldaa	*0xfa
    d937:	4c          	inca
    d938:	ce 01 3c    	ldx	#0x13c
    d93b:	bd eb bc    	jsr	0xebbc
    d93e:	7e ea 46    	jmp	0xea46
    d941:	53          	comb
    d942:	59          	rolb
    d943:	53          	comb
    d944:	58          	aslb
    d945:	20 53       	bra	0x0xd99a
    d947:	45          	.byte	0x45
    d948:	4e          	.byte	0x4e
    d949:	44          	lsra
    d94a:	2c 50       	bge	0x0xd99c
    d94c:	49          	rola
    d94d:	43          	coma
    d94e:	4b          	.byte	0x4b
    d94f:	23 2c       	bls	0x0xd97d
    d951:	50          	negb
    d952:	52          	.byte	0x52
    d953:	45          	.byte	0x45
    d954:	53          	comb
    d955:	53          	comb
    d956:	20 53       	bra	0x0xd9ab
    d958:	41          	.byte	0x41
    d959:	56          	rorb
    d95a:	45          	.byte	0x45
    d95b:	20 20       	bra	0x0xd97d
    d95d:	20 20       	bra	0x0xd97f
    d95f:	20 20       	bra	0x0xd981
    d961:	53          	comb
    d962:	45          	.byte	0x45
    d963:	4c          	inca
    d964:	45          	.byte	0x45
    d965:	43          	coma
    d966:	54          	lsrb
    d967:	20 4d       	bra	0x0xd9b6
    d969:	55          	.byte	0x55
    d96a:	4c          	inca
    d96b:	54          	lsrb
    d96c:	49          	rola
    d96d:	2c 20       	bge	0x0xd98f
    d96f:	20 20       	bra	0x0xd991
    d971:	50          	negb
    d972:	52          	.byte	0x52
    d973:	45          	.byte	0x45
    d974:	53          	comb
    d975:	53          	comb
    d976:	20 53       	bra	0x0xd9cb
    d978:	41          	.byte	0x41
    d979:	56          	rorb
    d97a:	45          	.byte	0x45
    d97b:	20 20       	bra	0x0xd99d
    d97d:	20 20       	bra	0x0xd99f
    d97f:	20 20       	bra	0x0xd9a1
    d981:	7d 01 1d    	tst	0x11d
    d984:	26 0a       	bne	0x0xd990
    d986:	bd ec 33    	jsr	0xec33
    d989:	ce 10 23    	ldx	#0x1023
    d98c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd98c
    d990:	bd eb 09    	jsr	0xeb09
    d993:	ce 01 20    	ldx	#0x120
    d996:	18 ce d9 c8 	ldy	#0xd9c8
    d99a:	c6 20       	ldab	#0x20
    d99c:	18 a6 00    	ldaa	0x0,y
    d99f:	a7 00       	staa	0x0,x
    d9a1:	08          	inx
    d9a2:	18 08       	iny
    d9a4:	5a          	decb
    d9a5:	26 f5       	bne	0x0xd99c
    d9a7:	ce 01 31    	ldx	#0x131
    d9aa:	96 40       	ldaa	*0x40
    d9ac:	bd eb f8    	jsr	0xebf8
    d9af:	ce d9 e8    	ldx	#0xd9e8
    d9b2:	d6 41       	ldab	*0x41
    d9b4:	c4 0f       	andb	#0xf
    d9b6:	18 ce 01 36 	ldy	#0x136
    d9ba:	bd ec 6f    	jsr	0xec6f
    d9bd:	ce 01 3c    	ldx	#0x13c
    d9c0:	96 67       	ldaa	*0x67
    d9c2:	bd eb bc    	jsr	0xebbc
    d9c5:	7e ea 46    	jmp	0xea46
    d9c8:	46          	rora
    d9c9:	49          	rola
    d9ca:	4e          	.byte	0x4e
    d9cb:	45          	.byte	0x45
    d9cc:	20 4d       	bra	0x0xda1b
    d9ce:	4f          	clra
    d9cf:	44          	lsra
    d9d0:	45          	.byte	0x45
    d9d1:	32          	pula
    d9d2:	20 45       	bra	0x0xda19
    d9d4:	4e          	.byte	0x4e
    d9d5:	56          	rorb
    d9d6:	31          	ins
    d9d7:	20 20       	bra	0x0xd9f9
    d9d9:	20 20       	bra	0x0xd9fb
    d9db:	20 20       	bra	0x0xd9fd
    d9dd:	20 20       	bra	0x0xd9ff
    d9df:	20 20       	bra	0x0xda01
    d9e1:	20 20       	bra	0x0xda03
    d9e3:	20 20       	bra	0x0xda05
    d9e5:	20 20       	bra	0x0xda07
    d9e7:	20 4e       	bra	0x0xda37
    d9e9:	4f          	clra
    d9ea:	52          	.byte	0x52
    d9eb:	4d          	tsta
    d9ec:	48          	asla
    d9ed:	41          	.byte	0x41
    d9ee:	4c          	inca
    d9ef:	46          	rora
    d9f0:	4e          	.byte	0x4e
    d9f1:	4f          	clra
    d9f2:	43          	coma
    d9f3:	56          	rorb
    d9f4:	4c          	inca
    d9f5:	4f          	clra
    d9f6:	57          	asrb
    d9f7:	31          	ins
    d9f8:	4c          	inca
    d9f9:	4f          	clra
    d9fa:	57          	asrb
    d9fb:	32          	pula
    d9fc:	7d 01 1d    	tst	0x11d
    d9ff:	26 0a       	bne	0x0xda0b
    da01:	bd ec 33    	jsr	0xec33
    da04:	ce 10 23    	ldx	#0x1023
    da07:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xda07
    da0b:	bd eb 09    	jsr	0xeb09
    da0e:	ce 01 20    	ldx	#0x120
    da11:	18 ce da 3e 	ldy	#0xda3e
    da15:	c6 20       	ldab	#0x20
    da17:	18 a6 00    	ldaa	0x0,y
    da1a:	a7 00       	staa	0x0,x
    da1c:	08          	inx
    da1d:	18 08       	iny
    da1f:	5a          	decb
    da20:	26 f5       	bne	0x0xda17
    da22:	ce 01 31    	ldx	#0x131
    da25:	b6 01 6e    	ldaa	0x16e
    da28:	bd eb f8    	jsr	0xebf8
    da2b:	ce 01 37    	ldx	#0x137
    da2e:	96 3e       	ldaa	*0x3e
    da30:	bd eb bc    	jsr	0xebbc
    da33:	08          	inx
    da34:	08          	inx
    da35:	08          	inx
    da36:	96 3f       	ldaa	*0x3f
    da38:	bd eb bc    	jsr	0xebbc
    da3b:	7e ea 46    	jmp	0xea46
    da3e:	54          	lsrb
    da3f:	55          	.byte	0x55
    da40:	4e          	.byte	0x4e
    da41:	45          	.byte	0x45
    da42:	20 20       	bra	0x0xda64
    da44:	4f          	clra
    da45:	53          	comb
    da46:	43          	coma
    da47:	31          	ins
    da48:	20 4f       	bra	0x0xda99
    da4a:	53          	comb
    da4b:	43          	coma
    da4c:	32          	pula
    da4d:	20 20       	bra	0x0xda6f
    da4f:	20 20       	bra	0x0xda71
    da51:	20 20       	bra	0x0xda73
    da53:	20 20       	bra	0x0xda75
    da55:	20 20       	bra	0x0xda77
    da57:	20 20       	bra	0x0xda79
    da59:	20 20       	bra	0x0xda7b
    da5b:	20 20       	bra	0x0xda7d
    da5d:	20 7d       	bra	0x0xdadc
    da5f:	01          	nop
    da60:	1d 26 0a    	bclr	0x26,x, #0x0a
    da63:	bd ec 33    	jsr	0xec33
    da66:	ce 10 23    	ldx	#0x1023
    da69:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xda69
    da6d:	bd eb 09    	jsr	0xeb09
    da70:	ce 01 20    	ldx	#0x120
    da73:	18 ce da cc 	ldy	#0xdacc
    da77:	c6 20       	ldab	#0x20
    da79:	18 a6 00    	ldaa	0x0,y
    da7c:	a7 00       	staa	0x0,x
    da7e:	08          	inx
    da7f:	18 08       	iny
    da81:	5a          	decb
    da82:	26 f5       	bne	0x0xda79
    da84:	ce da ec    	ldx	#0xdaec
    da87:	18 ce 01 30 	ldy	#0x130
    da8b:	13 21 40 07 	brclr	*0x21, #0x40, 0x0xda96
    da8f:	c6 01       	ldab	#0x1
    da91:	bd ec 5b    	jsr	0xec5b
    da94:	20 04       	bra	0x0xda9a
    da96:	5f          	clrb
    da97:	bd ec 5b    	jsr	0xec5b
    da9a:	18 ce 01 34 	ldy	#0x134
    da9e:	ce da f2    	ldx	#0xdaf2
    daa1:	5f          	clrb
    daa2:	bd ec 5b    	jsr	0xec5b
    daa5:	20 00       	bra	0x0xdaa7
    daa7:	ce da f8    	ldx	#0xdaf8
    daaa:	18 ce 01 38 	ldy	#0x138
    daae:	12 21 01 06 	brset	*0x21, #0x01, 0x0xdab8
    dab2:	5f          	clrb
    dab3:	bd ec 5b    	jsr	0xec5b
    dab6:	20 05       	bra	0x0xdabd
    dab8:	c6 01       	ldab	#0x1
    daba:	bd ec 5b    	jsr	0xec5b
    dabd:	ce da fe    	ldx	#0xdafe
    dac0:	d6 24       	ldab	*0x24
    dac2:	18 ce 01 3c 	ldy	#0x13c
    dac6:	bd ec 5b    	jsr	0xec5b
    dac9:	7e ea 46    	jmp	0xea46
    dacc:	20 4f       	bra	0x0xdb1d
    dace:	4e          	.byte	0x4e
    dacf:	20 54       	bra	0x0xdb25
    dad1:	59          	rolb
    dad2:	50          	negb
    dad3:	20 4d       	bra	0x0xdb22
    dad5:	44          	lsra
    dad6:	45          	.byte	0x45
    dad7:	20 44       	bra	0x0xdb1d
    dad9:	45          	.byte	0x45
    dada:	53          	comb
    dadb:	20 20       	bra	0x0xdafd
    dadd:	20 20       	bra	0x0xdaff
    dadf:	20 20       	bra	0x0xdb01
    dae1:	20 20       	bra	0x0xdb03
    dae3:	20 20       	bra	0x0xdb05
    dae5:	20 20       	bra	0x0xdb07
    dae7:	20 20       	bra	0x0xdb09
    dae9:	20 20       	bra	0x0xdb0b
    daeb:	20 4f       	bra	0x0xdb3c
    daed:	46          	rora
    daee:	46          	rora
    daef:	20 4f       	bra	0x0xdb40
    daf1:	4e          	.byte	0x4e
    daf2:	45          	.byte	0x45
    daf3:	58          	aslb
    daf4:	50          	negb
    daf5:	4c          	inca
    daf6:	49          	rola
    daf7:	4e          	.byte	0x4e
    daf8:	52          	.byte	0x52
    daf9:	45          	.byte	0x45
    dafa:	47          	asra
    dafb:	4c          	inca
    dafc:	45          	.byte	0x45
    dafd:	47          	asra
    dafe:	4f          	clra
    daff:	26 46       	bne	0x0xdb47
    db01:	4f          	clra
    db02:	53          	comb
    db03:	31          	ins
    db04:	4f          	clra
    db05:	53          	comb
    db06:	32          	pula
    db07:	31          	ins
    db08:	26 32       	bne	0x0xdb3c
    db0a:	31          	ins
    db0b:	26 46       	bne	0x0xdb53
    db0d:	32          	pula
    db0e:	26 46       	bne	0x0xdb56
    db10:	46          	rora
    db11:	49          	rola
    db12:	4c          	inca
    db13:	7d 01 1d    	tst	0x11d
    db16:	26 0a       	bne	0x0xdb22
    db18:	bd ec 33    	jsr	0xec33
    db1b:	ce 10 23    	ldx	#0x1023
    db1e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdb1e
    db22:	bd eb 09    	jsr	0xeb09
    db25:	ce 01 20    	ldx	#0x120
    db28:	18 ce db 62 	ldy	#0xdb62
    db2c:	c6 20       	ldab	#0x20
    db2e:	18 a6 00    	ldaa	0x0,y
    db31:	a7 00       	staa	0x0,x
    db33:	08          	inx
    db34:	18 08       	iny
    db36:	5a          	decb
    db37:	26 f5       	bne	0x0xdb2e
    db39:	ce db 82    	ldx	#0xdb82
    db3c:	d6 6a       	ldab	*0x6a
    db3e:	c4 0f       	andb	#0xf
    db40:	18 ce 01 30 	ldy	#0x130
    db44:	bd ec 6f    	jsr	0xec6f
    db47:	ce db 96    	ldx	#0xdb96
    db4a:	d6 95       	ldab	*0x95
    db4c:	18 ce 01 36 	ldy	#0x136
    db50:	bd ec 6f    	jsr	0xec6f
    db53:	ce db 9e    	ldx	#0xdb9e
    db56:	d6 69       	ldab	*0x69
    db58:	18 ce 01 3b 	ldy	#0x13b
    db5c:	bd ec 6f    	jsr	0xec6f
    db5f:	7e ea 46    	jmp	0xea46
    db62:	20 55       	bra	0x0xdbb9
    db64:	4e          	.byte	0x4e
    db65:	49          	rola
    db66:	20 56       	bra	0x0xdbbe
    db68:	4d          	tsta
    db69:	4f          	clra
    db6a:	44          	lsra
    db6b:	45          	.byte	0x45
    db6c:	20 50       	bra	0x0xdbbe
    db6e:	52          	.byte	0x52
    db6f:	49          	rola
    db70:	4f          	clra
    db71:	52          	.byte	0x52
    db72:	20 20       	bra	0x0xdb94
    db74:	20 20       	bra	0x0xdb96
    db76:	20 20       	bra	0x0xdb98
    db78:	20 20       	bra	0x0xdb9a
    db7a:	20 20       	bra	0x0xdb9c
    db7c:	20 20       	bra	0x0xdb9e
    db7e:	20 20       	bra	0x0xdba0
    db80:	20 20       	bra	0x0xdba2
    db82:	20 4f       	bra	0x0xdbd3
    db84:	4e          	.byte	0x4e
    db85:	45          	.byte	0x45
    db86:	20 54       	bra	0x0xdbdc
    db88:	57          	asrb
    db89:	4f          	clra
    db8a:	46          	rora
    db8b:	4f          	clra
    db8c:	55          	.byte	0x55
    db8d:	52          	.byte	0x52
    db8e:	20 53       	bra	0x0xdbe3
    db90:	49          	rola
    db91:	58          	aslb
    db92:	45          	.byte	0x45
    db93:	47          	asra
    db94:	48          	asla
    db95:	54          	lsrb
    db96:	43          	coma
    db97:	59          	rolb
    db98:	43          	coma
    db99:	4c          	inca
    db9a:	4e          	.byte	0x4e
    db9b:	4f          	clra
    db9c:	54          	lsrb
    db9d:	45          	.byte	0x45
    db9e:	4c          	inca
    db9f:	41          	.byte	0x41
    dba0:	53          	comb
    dba1:	54          	lsrb
    dba2:	20 4c       	bra	0x0xdbf0
    dba4:	4f          	clra
    dba5:	57          	asrb
    dba6:	bd eb 09    	jsr	0xeb09
    dba9:	ce 01 20    	ldx	#0x120
    dbac:	18 ce db d8 	ldy	#0xdbd8
    dbb0:	c6 20       	ldab	#0x20
    dbb2:	18 a6 00    	ldaa	0x0,y
    dbb5:	a7 00       	staa	0x0,x
    dbb7:	08          	inx
    dbb8:	18 08       	iny
    dbba:	5a          	decb
    dbbb:	26 f5       	bne	0x0xdbb2
    dbbd:	ce db f8    	ldx	#0xdbf8
    dbc0:	d6 6b       	ldab	*0x6b
    dbc2:	18 ce 01 30 	ldy	#0x130
    dbc6:	bd ec 6f    	jsr	0xec6f
    dbc9:	ce dc 04    	ldx	#0xdc04
    dbcc:	18 ce 01 36 	ldy	#0x136
    dbd0:	d6 68       	ldab	*0x68
    dbd2:	bd ec 6f    	jsr	0xec6f
    dbd5:	7e ea 46    	jmp	0xea46
    dbd8:	4f          	clra
    dbd9:	43          	coma
    dbda:	54          	lsrb
    dbdb:	41          	.byte	0x41
    dbdc:	56          	rorb
    dbdd:	20 4d       	bra	0x0xdc2c
    dbdf:	54          	lsrb
    dbe0:	52          	.byte	0x52
    dbe1:	47          	asra
    dbe2:	20 20       	bra	0x0xdc04
    dbe4:	20 20       	bra	0x0xdc06
    dbe6:	20 20       	bra	0x0xdc08
    dbe8:	20 20       	bra	0x0xdc0a
    dbea:	20 20       	bra	0x0xdc0c
    dbec:	20 20       	bra	0x0xdc0e
    dbee:	20 20       	bra	0x0xdc10
    dbf0:	20 20       	bra	0x0xdc12
    dbf2:	20 20       	bra	0x0xdc14
    dbf4:	20 20       	bra	0x0xdc16
    dbf6:	20 20       	bra	0x0xdc18
    dbf8:	20 4c       	bra	0x0xdc46
    dbfa:	4f          	clra
    dbfb:	57          	asrb
    dbfc:	20 4d       	bra	0x0xdc4b
    dbfe:	49          	rola
    dbff:	44          	lsra
    dc00:	48          	asla
    dc01:	49          	rola
    dc02:	47          	asra
    dc03:	48          	asla
    dc04:	20 4f       	bra	0x0xdc55
    dc06:	46          	rora
    dc07:	46          	rora
    dc08:	45          	.byte	0x45
    dc09:	4e          	.byte	0x4e
    dc0a:	56          	rorb
    dc0b:	31          	ins
    dc0c:	45          	.byte	0x45
    dc0d:	4e          	.byte	0x4e
    dc0e:	56          	rorb
    dc0f:	32          	pula
    dc10:	45          	.byte	0x45
    dc11:	4e          	.byte	0x4e
    dc12:	56          	rorb
    dc13:	33          	pulb
    dc14:	45          	.byte	0x45
    dc15:	31          	ins
    dc16:	26 32       	bne	0x0xdc4a
    dc18:	45          	.byte	0x45
    dc19:	31          	ins
    dc1a:	26 33       	bne	0x0xdc4f
    dc1c:	45          	.byte	0x45
    dc1d:	32          	pula
    dc1e:	26 33       	bne	0x0xdc53
    dc20:	20 41       	bra	0x0xdc63
    dc22:	4c          	inca
    dc23:	4c          	inca
    dc24:	7d 01 1d    	tst	0x11d
    dc27:	26 0a       	bne	0x0xdc33
    dc29:	bd ec 33    	jsr	0xec33
    dc2c:	ce 10 23    	ldx	#0x1023
    dc2f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdc2f
    dc33:	bd eb 09    	jsr	0xeb09
    dc36:	ce 01 20    	ldx	#0x120
    dc39:	18 ce dd 5d 	ldy	#0xdd5d
    dc3d:	c6 20       	ldab	#0x20
    dc3f:	18 a6 00    	ldaa	0x0,y
    dc42:	a7 00       	staa	0x0,x
    dc44:	08          	inx
    dc45:	18 08       	iny
    dc47:	5a          	decb
    dc48:	26 f5       	bne	0x0xdc3f
    dc4a:	96 f6       	ldaa	*0xf6
    dc4c:	48          	asla
    dc4d:	24 2b       	bcc	0x0xdc7a
    dc4f:	ce e0 53    	ldx	#0xe053
    dc52:	d6 8a       	ldab	*0x8a
    dc54:	18 ce 01 26 	ldy	#0x126
    dc58:	bd ec 6f    	jsr	0xec6f
    dc5b:	ce e0 53    	ldx	#0xe053
    dc5e:	d6 8b       	ldab	*0x8b
    dc60:	18 08       	iny
    dc62:	18 08       	iny
    dc64:	bd ec 6f    	jsr	0xec6f
    dc67:	ce 01 37    	ldx	#0x137
    dc6a:	96 8c       	ldaa	*0x8c
    dc6c:	bd eb f8    	jsr	0xebf8
    dc6f:	ce 01 3c    	ldx	#0x13c
    dc72:	96 8d       	ldaa	*0x8d
    dc74:	bd eb f8    	jsr	0xebf8
    dc77:	7e ea 46    	jmp	0xea46
    dc7a:	48          	asla
    dc7b:	24 2b       	bcc	0x0xdca8
    dc7d:	ce e0 53    	ldx	#0xe053
    dc80:	d6 86       	ldab	*0x86
    dc82:	18 ce 01 26 	ldy	#0x126
    dc86:	bd ec 6f    	jsr	0xec6f
    dc89:	ce e0 53    	ldx	#0xe053
    dc8c:	d6 87       	ldab	*0x87
    dc8e:	18 08       	iny
    dc90:	18 08       	iny
    dc92:	bd ec 6f    	jsr	0xec6f
    dc95:	ce 01 37    	ldx	#0x137
    dc98:	96 88       	ldaa	*0x88
    dc9a:	bd eb bc    	jsr	0xebbc
    dc9d:	08          	inx
    dc9e:	08          	inx
    dc9f:	08          	inx
    dca0:	96 89       	ldaa	*0x89
    dca2:	bd eb bc    	jsr	0xebbc
    dca5:	7e ea 46    	jmp	0xea46
    dca8:	48          	asla
    dca9:	24 2b       	bcc	0x0xdcd6
    dcab:	ce e0 53    	ldx	#0xe053
    dcae:	d6 82       	ldab	*0x82
    dcb0:	18 ce 01 26 	ldy	#0x126
    dcb4:	bd ec 6f    	jsr	0xec6f
    dcb7:	ce e0 53    	ldx	#0xe053
    dcba:	d6 83       	ldab	*0x83
    dcbc:	18 08       	iny
    dcbe:	18 08       	iny
    dcc0:	bd ec 6f    	jsr	0xec6f
    dcc3:	ce 01 37    	ldx	#0x137
    dcc6:	96 84       	ldaa	*0x84
    dcc8:	bd eb bc    	jsr	0xebbc
    dccb:	08          	inx
    dccc:	08          	inx
    dccd:	08          	inx
    dcce:	96 85       	ldaa	*0x85
    dcd0:	bd eb bc    	jsr	0xebbc
    dcd3:	7e ea 46    	jmp	0xea46
    dcd6:	48          	asla
    dcd7:	24 2b       	bcc	0x0xdd04
    dcd9:	ce e0 53    	ldx	#0xe053
    dcdc:	d6 7e       	ldab	*0x7e
    dcde:	18 ce 01 26 	ldy	#0x126
    dce2:	bd ec 6f    	jsr	0xec6f
    dce5:	ce e0 53    	ldx	#0xe053
    dce8:	d6 7f       	ldab	*0x7f
    dcea:	18 08       	iny
    dcec:	18 08       	iny
    dcee:	bd ec 6f    	jsr	0xec6f
    dcf1:	ce 01 37    	ldx	#0x137
    dcf4:	96 80       	ldaa	*0x80
    dcf6:	bd eb bc    	jsr	0xebbc
    dcf9:	08          	inx
    dcfa:	08          	inx
    dcfb:	08          	inx
    dcfc:	96 81       	ldaa	*0x81
    dcfe:	bd eb bc    	jsr	0xebbc
    dd01:	7e ea 46    	jmp	0xea46
    dd04:	48          	asla
    dd05:	24 2b       	bcc	0x0xdd32
    dd07:	ce e0 53    	ldx	#0xe053
    dd0a:	d6 7a       	ldab	*0x7a
    dd0c:	18 ce 01 26 	ldy	#0x126
    dd10:	bd ec 6f    	jsr	0xec6f
    dd13:	ce e0 53    	ldx	#0xe053
    dd16:	d6 7b       	ldab	*0x7b
    dd18:	18 08       	iny
    dd1a:	18 08       	iny
    dd1c:	bd ec 6f    	jsr	0xec6f
    dd1f:	ce 01 37    	ldx	#0x137
    dd22:	96 7c       	ldaa	*0x7c
    dd24:	bd eb bc    	jsr	0xebbc
    dd27:	08          	inx
    dd28:	08          	inx
    dd29:	08          	inx
    dd2a:	96 7d       	ldaa	*0x7d
    dd2c:	bd eb bc    	jsr	0xebbc
    dd2f:	7e ea 46    	jmp	0xea46
    dd32:	ce e0 53    	ldx	#0xe053
    dd35:	d6 76       	ldab	*0x76
    dd37:	18 ce 01 26 	ldy	#0x126
    dd3b:	bd ec 6f    	jsr	0xec6f
    dd3e:	ce e0 53    	ldx	#0xe053
    dd41:	d6 77       	ldab	*0x77
    dd43:	18 08       	iny
    dd45:	18 08       	iny
    dd47:	bd ec 6f    	jsr	0xec6f
    dd4a:	ce 01 37    	ldx	#0x137
    dd4d:	96 78       	ldaa	*0x78
    dd4f:	bd eb bc    	jsr	0xebbc
    dd52:	08          	inx
    dd53:	08          	inx
    dd54:	08          	inx
    dd55:	96 79       	ldaa	*0x79
    dd57:	bd eb bc    	jsr	0xebbc
    dd5a:	7e ea 46    	jmp	0xea46
    dd5d:	20 44       	bra	0x0xdda3
    dd5f:	45          	.byte	0x45
    dd60:	53          	comb
    dd61:	54          	lsrb
    dd62:	20 20       	bra	0x0xdd84
    dd64:	20 20       	bra	0x0xdd86
    dd66:	20 20       	bra	0x0xdd88
    dd68:	20 20       	bra	0x0xdd8a
    dd6a:	20 20       	bra	0x0xdd8c
    dd6c:	20 20       	bra	0x0xdd8e
    dd6e:	41          	.byte	0x41
    dd6f:	4d          	tsta
    dd70:	4e          	.byte	0x4e
    dd71:	54          	lsrb
    dd72:	20 20       	bra	0x0xdd94
    dd74:	20 20       	bra	0x0xdd96
    dd76:	20 20       	bra	0x0xdd98
    dd78:	20 20       	bra	0x0xdd9a
    dd7a:	20 20       	bra	0x0xdd9c
    dd7c:	20 41       	bra	0x0xddbf
    dd7e:	54          	lsrb
    dd7f:	54          	lsrb
    dd80:	31          	ins
    dd81:	44          	lsra
    dd82:	45          	.byte	0x45
    dd83:	43          	coma
    dd84:	31          	ins
    dd85:	53          	comb
    dd86:	55          	.byte	0x55
    dd87:	53          	comb
    dd88:	31          	ins
    dd89:	52          	.byte	0x52
    dd8a:	45          	.byte	0x45
    dd8b:	4c          	inca
    dd8c:	31          	ins
    dd8d:	41          	.byte	0x41
    dd8e:	4d          	tsta
    dd8f:	54          	lsrb
    dd90:	31          	ins
    dd91:	41          	.byte	0x41
    dd92:	54          	lsrb
    dd93:	54          	lsrb
    dd94:	32          	pula
    dd95:	44          	lsra
    dd96:	45          	.byte	0x45
    dd97:	43          	coma
    dd98:	32          	pula
    dd99:	53          	comb
    dd9a:	55          	.byte	0x55
    dd9b:	53          	comb
    dd9c:	32          	pula
    dd9d:	52          	.byte	0x52
    dd9e:	45          	.byte	0x45
    dd9f:	4c          	inca
    dda0:	32          	pula
    dda1:	41          	.byte	0x41
    dda2:	4d          	tsta
    dda3:	54          	lsrb
    dda4:	32          	pula
    dda5:	41          	.byte	0x41
    dda6:	54          	lsrb
    dda7:	54          	lsrb
    dda8:	33          	pulb
    dda9:	44          	lsra
    ddaa:	45          	.byte	0x45
    ddab:	43          	coma
    ddac:	33          	pulb
    ddad:	53          	comb
    ddae:	55          	.byte	0x55
    ddaf:	53          	comb
    ddb0:	33          	pulb
    ddb1:	52          	.byte	0x52
    ddb2:	45          	.byte	0x45
    ddb3:	4c          	inca
    ddb4:	33          	pulb
    ddb5:	41          	.byte	0x41
    ddb6:	4d          	tsta
    ddb7:	54          	lsrb
    ddb8:	33          	pulb
    ddb9:	44          	lsra
    ddba:	45          	.byte	0x45
    ddbb:	4c          	inca
    ddbc:	31          	ins
    ddbd:	44          	lsra
    ddbe:	45          	.byte	0x45
    ddbf:	4c          	inca
    ddc0:	33          	pulb
    ddc1:	7d 01 1d    	tst	0x11d
    ddc4:	26 0a       	bne	0x0xddd0
    ddc6:	bd ec 33    	jsr	0xec33
    ddc9:	ce 10 23    	ldx	#0x1023
    ddcc:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xddcc
    ddd0:	bd eb 09    	jsr	0xeb09
    ddd3:	ce 01 20    	ldx	#0x120
    ddd6:	18 ce de 02 	ldy	#0xde02
    ddda:	c6 20       	ldab	#0x20
    dddc:	18 a6 00    	ldaa	0x0,y
    dddf:	a7 00       	staa	0x0,x
    dde1:	08          	inx
    dde2:	18 08       	iny
    dde4:	5a          	decb
    dde5:	26 f5       	bne	0x0xdddc
    dde7:	ce de 22    	ldx	#0xde22
    ddea:	d6 8e       	ldab	*0x8e
    ddec:	18 ce 01 36 	ldy	#0x136
    ddf0:	bd ec 6f    	jsr	0xec6f
    ddf3:	ce de 2e    	ldx	#0xde2e
    ddf6:	d6 8f       	ldab	*0x8f
    ddf8:	18 08       	iny
    ddfa:	18 08       	iny
    ddfc:	bd ec 6f    	jsr	0xec6f
    ddff:	7e ea 46    	jmp	0xea46
    de02:	43          	coma
    de03:	4e          	.byte	0x4e
    de04:	54          	lsrb
    de05:	52          	.byte	0x52
    de06:	4c          	inca
    de07:	20 43       	bra	0x0xde4c
    de09:	4f          	clra
    de0a:	4e          	.byte	0x4e
    de0b:	31          	ins
    de0c:	20 43       	bra	0x0xde51
    de0e:	4f          	clra
    de0f:	4e          	.byte	0x4e
    de10:	32          	pula
    de11:	20 41       	bra	0x0xde54
    de13:	53          	comb
    de14:	53          	comb
    de15:	47          	asra
    de16:	4e          	.byte	0x4e
    de17:	20 20       	bra	0x0xde39
    de19:	20 20       	bra	0x0xde3b
    de1b:	20 20       	bra	0x0xde3d
    de1d:	20 20       	bra	0x0xde3f
    de1f:	20 20       	bra	0x0xde41
    de21:	20 42       	bra	0x0xde65
    de23:	52          	.byte	0x52
    de24:	54          	lsrb
    de25:	48          	asla
    de26:	4d          	tsta
    de27:	4f          	clra
    de28:	44          	lsra
    de29:	57          	asrb
    de2a:	54          	lsrb
    de2b:	55          	.byte	0x55
    de2c:	43          	coma
    de2d:	48          	asla
    de2e:	20 44       	bra	0x0xde74
    de30:	59          	rolb
    de31:	4e          	.byte	0x4e
    de32:	54          	lsrb
    de33:	52          	.byte	0x52
    de34:	41          	.byte	0x41
    de35:	4b          	.byte	0x4b
    de36:	7d 01 1d    	tst	0x11d
    de39:	26 0a       	bne	0x0xde45
    de3b:	bd ec 33    	jsr	0xec33
    de3e:	ce 10 23    	ldx	#0x1023
    de41:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xde41
    de45:	bd eb 09    	jsr	0xeb09
    de48:	ce 01 20    	ldx	#0x120
    de4b:	18 ce de 84 	ldy	#0xde84
    de4f:	c6 20       	ldab	#0x20
    de51:	18 a6 00    	ldaa	0x0,y
    de54:	a7 00       	staa	0x0,x
    de56:	08          	inx
    de57:	18 08       	iny
    de59:	5a          	decb
    de5a:	26 f5       	bne	0x0xde51
    de5c:	f6 50 00    	ldab	0x5000
    de5f:	c1 0a       	cmpb	#0xa
    de61:	25 04       	bcs	0x0xde67
    de63:	5f          	clrb
    de64:	f7 50 00    	stab	0x5000
    de67:	86 09       	ldaa	#0x9
    de69:	3d          	mul
    de6a:	1b          	aba
    de6b:	16          	tab
    de6c:	ce de a4    	ldx	#0xdea4
    de6f:	3a          	abx
    de70:	18 ce 01 31 	ldy	#0x131
    de74:	c6 09       	ldab	#0x9
    de76:	a6 00       	ldaa	0x0,x
    de78:	18 a7 00    	staa	0x0,y
    de7b:	08          	inx
    de7c:	18 08       	iny
    de7e:	5a          	decb
    de7f:	26 f5       	bne	0x0xde76
    de81:	7e ea 46    	jmp	0xea46
    de84:	20 4d       	bra	0x0xded3
    de86:	55          	.byte	0x55
    de87:	4c          	inca
    de88:	54          	lsrb
    de89:	49          	rola
    de8a:	20 54       	bra	0x0xdee0
    de8c:	59          	rolb
    de8d:	50          	negb
    de8e:	45          	.byte	0x45
    de8f:	3a          	abx
    de90:	20 20       	bra	0x0xdeb2
    de92:	20 20       	bra	0x0xdeb4
    de94:	20 20       	bra	0x0xdeb6
    de96:	20 20       	bra	0x0xdeb8
    de98:	20 20       	bra	0x0xdeba
    de9a:	20 20       	bra	0x0xdebc
    de9c:	20 20       	bra	0x0xdebe
    de9e:	20 20       	bra	0x0xdec0
    dea0:	20 20       	bra	0x0xdec2
    dea2:	20 20       	bra	0x0xdec4
    dea4:	50          	negb
    dea5:	52          	.byte	0x52
    dea6:	45          	.byte	0x45
    dea7:	50          	negb
    dea8:	41          	.byte	0x41
    dea9:	52          	.byte	0x52
    deaa:	45          	.byte	0x45
    deab:	44          	lsra
    deac:	20 53       	bra	0x0xdf01
    deae:	50          	negb
    deaf:	4c          	inca
    deb0:	49          	rola
    deb1:	54          	lsrb
    deb2:	20 31       	bra	0x0xdee5
    deb4:	2b 37       	bmi	0x0xdeed
    deb6:	53          	comb
    deb7:	50          	negb
    deb8:	4c          	inca
    deb9:	49          	rola
    deba:	54          	lsrb
    debb:	20 32       	bra	0x0xdeef
    debd:	2b 36       	bmi	0x0xdef5
    debf:	53          	comb
    dec0:	50          	negb
    dec1:	4c          	inca
    dec2:	49          	rola
    dec3:	54          	lsrb
    dec4:	20 33       	bra	0x0xdef9
    dec6:	2b 35       	bmi	0x0xdefd
    dec8:	53          	comb
    dec9:	50          	negb
    deca:	4c          	inca
    decb:	49          	rola
    decc:	54          	lsrb
    decd:	20 34       	bra	0x0xdf03
    decf:	2b 34       	bmi	0x0xdf05
    ded1:	4c          	inca
    ded2:	41          	.byte	0x41
    ded3:	59          	rolb
    ded4:	45          	.byte	0x45
    ded5:	52          	.byte	0x52
    ded6:	20 34       	bra	0x0xdf0c
    ded8:	2b 34       	bmi	0x0xdf0e
    deda:	4d          	tsta
    dedb:	55          	.byte	0x55
    dedc:	4c          	inca
    dedd:	54          	lsrb
    dede:	49          	rola
    dedf:	43          	coma
    dee0:	48          	asla
    dee1:	41          	.byte	0x41
    dee2:	4e          	.byte	0x4e
    dee3:	01          	nop
    dee4:	02          	idiv
    dee5:	04          	lsrd
    dee6:	08          	inx
    dee7:	10          	sba
    dee8:	20 40       	bra	0x0xdf2a
    deea:	80 01       	suba	#0x1
    deec:	fe 00 00    	ldx	0x0
    deef:	00          	bgnd
    def0:	00          	bgnd
    def1:	00          	bgnd
    def2:	00          	bgnd
    def3:	03          	fdiv
    def4:	fc 00 00    	ldd	0x0
    def7:	00          	bgnd
    def8:	00          	bgnd
    def9:	00          	bgnd
    defa:	00          	bgnd
    defb:	07          	tpa
    defc:	f8 00 00    	eorb	0x0
    deff:	00          	bgnd
    df00:	00          	bgnd
    df01:	00          	bgnd
    df02:	00          	bgnd
    df03:	0f          	sei
    df04:	f0 00 00    	subb	0x0
    df07:	00          	bgnd
    df08:	00          	bgnd
    df09:	00          	bgnd
    df0a:	00          	bgnd
    df0b:	0f          	sei
    df0c:	f0 00 00    	subb	0x0
    df0f:	00          	bgnd
    df10:	00          	bgnd
    df11:	00          	bgnd
    df12:	00          	bgnd
    df13:	03          	fdiv
    df14:	fc 00 00    	ldd	0x0
    df17:	00          	bgnd
    df18:	00          	bgnd
    df19:	00          	bgnd
    df1a:	00          	bgnd
    df1b:	01          	nop
    df1c:	02          	idiv
    df1d:	fc 00 00    	ldd	0x0
    df20:	00          	bgnd
    df21:	00          	bgnd
    df22:	00          	bgnd
    df23:	01          	nop
    df24:	02          	idiv
    df25:	04          	lsrd
    df26:	08          	inx
    df27:	10          	sba
    df28:	20 40       	bra	0x0xdf6a
    df2a:	80 00       	suba	#0x0
    df2c:	01          	nop
    df2d:	03          	fdiv
    df2e:	07          	tpa
    df2f:	0f          	sei
    df30:	1f 3f 7f ff 	brclr	0x3f,x, #0x7f, 0x0xdf33
    df34:	01          	nop
    df35:	02          	idiv
    df36:	00          	bgnd
    df37:	00          	bgnd
    df38:	00          	bgnd
    df39:	00          	bgnd
    df3a:	00          	bgnd
    df3b:	00          	bgnd
    df3c:	03          	fdiv
    df3d:	0c          	clc
    df3e:	00          	bgnd
    df3f:	00          	bgnd
    df40:	00          	bgnd
    df41:	00          	bgnd
    df42:	00          	bgnd
    df43:	00          	bgnd
    df44:	07          	tpa
    df45:	38          	pulx
    df46:	00          	bgnd
    df47:	00          	bgnd
    df48:	00          	bgnd
    df49:	00          	bgnd
    df4a:	00          	bgnd
    df4b:	00          	bgnd
    df4c:	0f          	sei
    df4d:	f0 00 00    	subb	0x0
    df50:	00          	bgnd
    df51:	00          	bgnd
    df52:	00          	bgnd
    df53:	00          	bgnd
    df54:	bd eb 09    	jsr	0xeb09
    df57:	ce 01 20    	ldx	#0x120
    df5a:	18 ce df 77 	ldy	#0xdf77
    df5e:	c6 20       	ldab	#0x20
    df60:	18 a6 00    	ldaa	0x0,y
    df63:	a7 00       	staa	0x0,x
    df65:	08          	inx
    df66:	18 08       	iny
    df68:	5a          	decb
    df69:	26 f5       	bne	0x0xdf60
    df6b:	b6 50 21    	ldaa	0x5021
    df6e:	ce 01 3c    	ldx	#0x13c
    df71:	bd eb bc    	jsr	0xebbc
    df74:	7e ea 46    	jmp	0xea46
    df77:	53          	comb
    df78:	50          	negb
    df79:	4c          	inca
    df7a:	49          	rola
    df7b:	54          	lsrb
    df7c:	20 50       	bra	0x0xdfce
    df7e:	4f          	clra
    df7f:	49          	rola
    df80:	4e          	.byte	0x4e
    df81:	54          	lsrb
    df82:	3a          	abx
    df83:	20 20       	bra	0x0xdfa5
    df85:	20 20       	bra	0x0xdfa7
    df87:	4d          	tsta
    df88:	49          	rola
    df89:	44          	lsra
    df8a:	49          	rola
    df8b:	20 4e       	bra	0x0xdfdb
    df8d:	4f          	clra
    df8e:	54          	lsrb
    df8f:	45          	.byte	0x45
    df90:	20 23       	bra	0x0xdfb5
    df92:	20 20       	bra	0x0xdfb4
    df94:	20 20       	bra	0x0xdfb6
    df96:	20 7d       	bra	0x0xe015
    df98:	01          	nop
    df99:	1d 26 0a    	bclr	0x26,x, #0x0a
    df9c:	bd ec 33    	jsr	0xec33
    df9f:	ce 10 23    	ldx	#0x1023
    dfa2:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdfa2
    dfa6:	bd eb 09    	jsr	0xeb09
    dfa9:	ce 01 20    	ldx	#0x120
    dfac:	18 ce df d2 	ldy	#0xdfd2
    dfb0:	c6 20       	ldab	#0x20
    dfb2:	18 a6 00    	ldaa	0x0,y
    dfb5:	a7 00       	staa	0x0,x
    dfb7:	08          	inx
    dfb8:	18 08       	iny
    dfba:	5a          	decb
    dfbb:	26 f5       	bne	0x0xdfb2
    dfbd:	ce 01 34    	ldx	#0x134
    dfc0:	b6 01 68    	ldaa	0x168
    dfc3:	bd eb bc    	jsr	0xebbc
    dfc6:	ce 01 3c    	ldx	#0x13c
    dfc9:	b6 01 69    	ldaa	0x169
    dfcc:	bd eb bc    	jsr	0xebbc
    dfcf:	7e ea 46    	jmp	0xea46
    dfd2:	20 44       	bra	0x0xe018
    dfd4:	59          	rolb
    dfd5:	4e          	.byte	0x4e
    dfd6:	41          	.byte	0x41
    dfd7:	4d          	tsta
    dfd8:	49          	rola
    dfd9:	43          	coma
    dfda:	53          	comb
    dfdb:	20 52       	bra	0x0xe02f
    dfdd:	41          	.byte	0x41
    dfde:	4e          	.byte	0x4e
    dfdf:	47          	asra
    dfe0:	45          	.byte	0x45
    dfe1:	20 20       	bra	0x0xe003
    dfe3:	4f          	clra
    dfe4:	4e          	.byte	0x4e
    dfe5:	20 20       	bra	0x0xe007
    dfe7:	20 20       	bra	0x0xe009
    dfe9:	20 4f       	bra	0x0xe03a
    dfeb:	46          	rora
    dfec:	46          	rora
    dfed:	20 20       	bra	0x0xe00f
    dfef:	20 20       	bra	0x0xe011
    dff1:	20 7d       	bra	0x0xe070
    dff3:	01          	nop
    dff4:	1d 26 0a    	bclr	0x26,x, #0x0a
    dff7:	bd ec 33    	jsr	0xec33
    dffa:	ce 10 23    	ldx	#0x1023
    dffd:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdffd
    e001:	bd eb 09    	jsr	0xeb09
    e004:	86 20       	ldaa	#0x20
    e006:	c6 20       	ldab	#0x20
    e008:	ce 01 20    	ldx	#0x120
    e00b:	a7 00       	staa	0x0,x
    e00d:	08          	inx
    e00e:	5a          	decb
    e00f:	26 fa       	bne	0x0xe00b
    e011:	ce e0 53    	ldx	#0xe053
    e014:	d6 60       	ldab	*0x60
    e016:	18 ce 01 20 	ldy	#0x120
    e01a:	bd ec 6f    	jsr	0xec6f
    e01d:	ce e0 53    	ldx	#0xe053
    e020:	d6 61       	ldab	*0x61
    e022:	18 08       	iny
    e024:	18 08       	iny
    e026:	18 08       	iny
    e028:	bd ec 6f    	jsr	0xec6f
    e02b:	ce e0 53    	ldx	#0xe053
    e02e:	d6 62       	ldab	*0x62
    e030:	18 08       	iny
    e032:	18 08       	iny
    e034:	bd ec 6f    	jsr	0xec6f
    e037:	ce 01 31    	ldx	#0x131
    e03a:	96 63       	ldaa	*0x63
    e03c:	bd eb bc    	jsr	0xebbc
    e03f:	08          	inx
    e040:	08          	inx
    e041:	08          	inx
    e042:	08          	inx
    e043:	96 64       	ldaa	*0x64
    e045:	bd eb bc    	jsr	0xebbc
    e048:	08          	inx
    e049:	08          	inx
    e04a:	08          	inx
    e04b:	96 65       	ldaa	*0x65
    e04d:	bd eb bc    	jsr	0xebbc
    e050:	7e ea 46    	jmp	0xea46
    e053:	20 4f       	bra	0x0xe0a4
    e055:	46          	rora
    e056:	46          	rora
    e057:	46          	rora
    e058:	52          	.byte	0x52
    e059:	45          	.byte	0x45
    e05a:	31          	ins
    e05b:	46          	rora
    e05c:	52          	.byte	0x52
    e05d:	45          	.byte	0x45
    e05e:	32          	pula
    e05f:	31          	ins
    e060:	26 32       	bne	0x0xe094
    e062:	46          	rora
    e063:	4c          	inca
    e064:	45          	.byte	0x45
    e065:	56          	rorb
    e066:	31          	ins
    e067:	4c          	inca
    e068:	45          	.byte	0x45
    e069:	56          	rorb
    e06a:	32          	pula
    e06b:	20 50       	bra	0x0xe0bd
    e06d:	57          	asrb
    e06e:	31          	ins
    e06f:	20 50       	bra	0x0xe0c1
    e071:	57          	asrb
    e072:	32          	pula
    e073:	31          	ins
    e074:	26 32       	bne	0x0xe0a8
    e076:	50          	negb
    e077:	46          	rora
    e078:	49          	rola
    e079:	4c          	inca
    e07a:	54          	lsrb
    e07b:	52          	.byte	0x52
    e07c:	45          	.byte	0x45
    e07d:	53          	comb
    e07e:	4f          	clra
    e07f:	4c          	inca
    e080:	45          	.byte	0x45
    e081:	56          	rorb
    e082:	4e          	.byte	0x4e
    e083:	58          	aslb
    e084:	4d          	tsta
    e085:	4f          	clra
    e086:	44          	lsra
    e087:	20 45       	bra	0x0xe0ce
    e089:	41          	.byte	0x41
    e08a:	31          	ins
    e08b:	20 45       	bra	0x0xe0d2
    e08d:	41          	.byte	0x41
    e08e:	33          	pulb
    e08f:	20 45       	bra	0x0xe0d6
    e091:	58          	aslb
    e092:	54          	lsrb
    e093:	4c          	inca
    e094:	46          	rora
    e095:	31          	ins
    e096:	52          	.byte	0x52
    e097:	4c          	inca
    e098:	46          	rora
    e099:	32          	pula
    e09a:	52          	.byte	0x52
    e09b:	4c          	inca
    e09c:	46          	rora
    e09d:	31          	ins
    e09e:	44          	lsra
    e09f:	4c          	inca
    e0a0:	46          	rora
    e0a1:	32          	pula
    e0a2:	44          	lsra
    e0a3:	31          	ins
    e0a4:	26 32       	bne	0x0xe0d8
    e0a6:	44          	lsra
    e0a7:	31          	ins
    e0a8:	26 32       	bne	0x0xe0dc
    e0aa:	52          	.byte	0x52
    e0ab:	50          	negb
    e0ac:	41          	.byte	0x41
    e0ad:	4e          	.byte	0x4e
    e0ae:	52          	.byte	0x52
    e0af:	50          	negb
    e0b0:	41          	.byte	0x41
    e0b1:	4e          	.byte	0x4e
    e0b2:	44          	lsra
    e0b3:	20 50       	bra	0x0xe105
    e0b5:	41          	.byte	0x41
    e0b6:	4e          	.byte	0x4e
    e0b7:	bd eb 09    	jsr	0xeb09
    e0ba:	ce 01 20    	ldx	#0x120
    e0bd:	18 ce e0 ea 	ldy	#0xe0ea
    e0c1:	c6 20       	ldab	#0x20
    e0c3:	18 a6 00    	ldaa	0x0,y
    e0c6:	a7 00       	staa	0x0,x
    e0c8:	08          	inx
    e0c9:	18 08       	iny
    e0cb:	5a          	decb
    e0cc:	26 f5       	bne	0x0xe0c3
    e0ce:	ce 01 31    	ldx	#0x131
    e0d1:	96 54       	ldaa	*0x54
    e0d3:	bd eb bc    	jsr	0xebbc
    e0d6:	08          	inx
    e0d7:	08          	inx
    e0d8:	08          	inx
    e0d9:	08          	inx
    e0da:	96 59       	ldaa	*0x59
    e0dc:	bd eb bc    	jsr	0xebbc
    e0df:	08          	inx
    e0e0:	08          	inx
    e0e1:	08          	inx
    e0e2:	96 5e       	ldaa	*0x5e
    e0e4:	bd eb bc    	jsr	0xebbc
    e0e7:	7e ea 46    	jmp	0xea46
    e0ea:	20 44       	bra	0x0xe130
    e0ec:	4b          	.byte	0x4b
    e0ed:	32          	pula
    e0ee:	20 20       	bra	0x0xe110
    e0f0:	20 44       	bra	0x0xe136
    e0f2:	4b          	.byte	0x4b
    e0f3:	32          	pula
    e0f4:	20 20       	bra	0x0xe116
    e0f6:	44          	lsra
    e0f7:	4b          	.byte	0x4b
    e0f8:	32          	pula
    e0f9:	20 20       	bra	0x0xe11b
    e0fb:	20 20       	bra	0x0xe11d
    e0fd:	20 20       	bra	0x0xe11f
    e0ff:	20 20       	bra	0x0xe121
    e101:	20 20       	bra	0x0xe123
    e103:	20 20       	bra	0x0xe125
    e105:	20 20       	bra	0x0xe127
    e107:	20 20       	bra	0x0xe129
    e109:	20 bd       	bra	0x0xe0c8
    e10b:	eb 09       	addb	0x9,x
    e10d:	ce 01 20    	ldx	#0x120
    e110:	18 ce e1 3c 	ldy	#0xe13c
    e114:	c6 20       	ldab	#0x20
    e116:	18 a6 00    	ldaa	0x0,y
    e119:	a7 00       	staa	0x0,x
    e11b:	08          	inx
    e11c:	18 08       	iny
    e11e:	5a          	decb
    e11f:	26 f5       	bne	0x0xe116
    e121:	ce 01 31    	ldx	#0x131
    e124:	96 90       	ldaa	*0x90
    e126:	bd eb bc    	jsr	0xebbc
    e129:	ce 01 37    	ldx	#0x137
    e12c:	96 91       	ldaa	*0x91
    e12e:	bd eb bc    	jsr	0xebbc
    e131:	ce 01 3c    	ldx	#0x13c
    e134:	96 92       	ldaa	*0x92
    e136:	bd eb bc    	jsr	0xebbc
    e139:	7e ea 46    	jmp	0xea46
    e13c:	44          	lsra
    e13d:	59          	rolb
    e13e:	4e          	.byte	0x4e
    e13f:	31          	ins
    e140:	20 20       	bra	0x0xe162
    e142:	44          	lsra
    e143:	59          	rolb
    e144:	4e          	.byte	0x4e
    e145:	32          	pula
    e146:	20 44       	bra	0x0xe18c
    e148:	59          	rolb
    e149:	4e          	.byte	0x4e
    e14a:	33          	pulb
    e14b:	20 20       	bra	0x0xe16d
    e14d:	20 20       	bra	0x0xe16f
    e14f:	20 20       	bra	0x0xe171
    e151:	20 20       	bra	0x0xe173
    e153:	20 20       	bra	0x0xe175
    e155:	20 20       	bra	0x0xe177
    e157:	20 20       	bra	0x0xe179
    e159:	20 20       	bra	0x0xe17b
    e15b:	20 bd       	bra	0x0xe11a
    e15d:	eb 09       	addb	0x9,x
    e15f:	ce 01 20    	ldx	#0x120
    e162:	18 ce e1 86 	ldy	#0xe186
    e166:	c6 20       	ldab	#0x20
    e168:	18 a6 00    	ldaa	0x0,y
    e16b:	a7 00       	staa	0x0,x
    e16d:	08          	inx
    e16e:	18 08       	iny
    e170:	5a          	decb
    e171:	26 f5       	bne	0x0xe168
    e173:	ce 01 31    	ldx	#0x131
    e176:	96 50       	ldaa	*0x50
    e178:	bd eb bc    	jsr	0xebbc
    e17b:	ce 01 37    	ldx	#0x137
    e17e:	96 98       	ldaa	*0x98
    e180:	bd eb bc    	jsr	0xebbc
    e183:	7e ea 46    	jmp	0xea46
    e186:	44          	lsra
    e187:	4c          	inca
    e188:	41          	.byte	0x41
    e189:	59          	rolb
    e18a:	31          	ins
    e18b:	20 44       	bra	0x0xe1d1
    e18d:	4c          	inca
    e18e:	41          	.byte	0x41
    e18f:	59          	rolb
    e190:	33          	pulb
    e191:	20 20       	bra	0x0xe1b3
    e193:	20 20       	bra	0x0xe1b5
    e195:	20 20       	bra	0x0xe1b7
    e197:	20 20       	bra	0x0xe1b9
    e199:	20 20       	bra	0x0xe1bb
    e19b:	20 20       	bra	0x0xe1bd
    e19d:	20 20       	bra	0x0xe1bf
    e19f:	20 20       	bra	0x0xe1c1
    e1a1:	20 20       	bra	0x0xe1c3
    e1a3:	20 20       	bra	0x0xe1c5
    e1a5:	20 7d       	bra	0x0xe224
    e1a7:	01          	nop
    e1a8:	1d 26 0a    	bclr	0x26,x, #0x0a
    e1ab:	bd ec 33    	jsr	0xec33
    e1ae:	ce 10 23    	ldx	#0x1023
    e1b1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe1b1
    e1b5:	bd eb 09    	jsr	0xeb09
    e1b8:	ce 01 20    	ldx	#0x120
    e1bb:	18 ce e1 e9 	ldy	#0xe1e9
    e1bf:	c6 20       	ldab	#0x20
    e1c1:	18 a6 00    	ldaa	0x0,y
    e1c4:	a7 00       	staa	0x0,x
    e1c6:	08          	inx
    e1c7:	18 08       	iny
    e1c9:	5a          	decb
    e1ca:	26 f5       	bne	0x0xe1c1
    e1cc:	ce e2 09    	ldx	#0xe209
    e1cf:	d6 4b       	ldab	*0x4b
    e1d1:	18 ce 01 30 	ldy	#0x130
    e1d5:	bd ec 6f    	jsr	0xec6f
    e1d8:	ce e2 25    	ldx	#0xe225
    e1db:	d6 66       	ldab	*0x66
    e1dd:	18 08       	iny
    e1df:	18 08       	iny
    e1e1:	18 08       	iny
    e1e3:	bd ec 6f    	jsr	0xec6f
    e1e6:	7e ea 46    	jmp	0xea46
    e1e9:	54          	lsrb
    e1ea:	59          	rolb
    e1eb:	50          	negb
    e1ec:	45          	.byte	0x45
    e1ed:	20 20       	bra	0x0xe20f
    e1ef:	49          	rola
    e1f0:	4e          	.byte	0x4e
    e1f1:	56          	rorb
    e1f2:	54          	lsrb
    e1f3:	20 20       	bra	0x0xe215
    e1f5:	20 20       	bra	0x0xe217
    e1f7:	20 20       	bra	0x0xe219
    e1f9:	20 20       	bra	0x0xe21b
    e1fb:	20 20       	bra	0x0xe21d
    e1fd:	20 20       	bra	0x0xe21f
    e1ff:	20 20       	bra	0x0xe221
    e201:	20 20       	bra	0x0xe223
    e203:	20 20       	bra	0x0xe225
    e205:	20 20       	bra	0x0xe227
    e207:	20 20       	bra	0x0xe229
    e209:	4f          	clra
    e20a:	42          	.byte	0x42
    e20b:	4c          	inca
    e20c:	50          	negb
    e20d:	4f          	clra
    e20e:	42          	.byte	0x42
    e20f:	42          	.byte	0x42
    e210:	50          	negb
    e211:	4f          	clra
    e212:	42          	.byte	0x42
    e213:	48          	asla
    e214:	50          	negb
    e215:	4f          	clra
    e216:	42          	.byte	0x42
    e217:	42          	.byte	0x42
    e218:	52          	.byte	0x52
    e219:	4d          	tsta
    e21a:	49          	rola
    e21b:	4e          	.byte	0x4e
    e21c:	49          	rola
    e21d:	41          	.byte	0x41
    e21e:	55          	.byte	0x55
    e21f:	58          	aslb
    e220:	31          	ins
    e221:	43          	coma
    e222:	53          	comb
    e223:	38          	pulx
    e224:	30          	tsx
    e225:	20 4f       	bra	0x0xe276
    e227:	46          	rora
    e228:	46          	rora
    e229:	45          	.byte	0x45
    e22a:	4e          	.byte	0x4e
    e22b:	56          	rorb
    e22c:	31          	ins
    e22d:	45          	.byte	0x45
    e22e:	4e          	.byte	0x4e
    e22f:	56          	rorb
    e230:	33          	pulb
    e231:	45          	.byte	0x45
    e232:	31          	ins
    e233:	26 33       	bne	0x0xe268
    e235:	bd eb 09    	jsr	0xeb09
    e238:	ce 01 20    	ldx	#0x120
    e23b:	18 ce e2 76 	ldy	#0xe276
    e23f:	c6 20       	ldab	#0x20
    e241:	86 06       	ldaa	#0x6
    e243:	91 4b       	cmpa	*0x4b
    e245:	26 04       	bne	0x0xe24b
    e247:	18 ce e2 96 	ldy	#0xe296
    e24b:	18 a6 00    	ldaa	0x0,y
    e24e:	a7 00       	staa	0x0,x
    e250:	08          	inx
    e251:	18 08       	iny
    e253:	5a          	decb
    e254:	26 f5       	bne	0x0xe24b
    e256:	ce 01 30    	ldx	#0x130
    e259:	96 6e       	ldaa	*0x6e
    e25b:	bd eb bc    	jsr	0xebbc
    e25e:	08          	inx
    e25f:	08          	inx
    e260:	96 48       	ldaa	*0x48
    e262:	bd eb bc    	jsr	0xebbc
    e265:	08          	inx
    e266:	08          	inx
    e267:	96 6f       	ldaa	*0x6f
    e269:	bd eb bc    	jsr	0xebbc
    e26c:	08          	inx
    e26d:	08          	inx
    e26e:	96 70       	ldaa	*0x70
    e270:	bd ec 83    	jsr	0xec83
    e273:	7e ea 46    	jmp	0xea46
    e276:	20 49       	bra	0x0xe2c1
    e278:	4e          	.byte	0x4e
    e279:	20 4d       	bra	0x0xe2c8
    e27b:	49          	rola
    e27c:	58          	aslb
    e27d:	20 54       	bra	0x0xe2d3
    e27f:	52          	.byte	0x52
    e280:	47          	asra
    e281:	20 57       	bra	0x0xe2da
    e283:	49          	rola
    e284:	4e          	.byte	0x4e
    e285:	20 20       	bra	0x0xe2a7
    e287:	20 20       	bra	0x0xe2a9
    e289:	20 20       	bra	0x0xe2ab
    e28b:	20 20       	bra	0x0xe2ad
    e28d:	20 20       	bra	0x0xe2af
    e28f:	20 20       	bra	0x0xe2b1
    e291:	20 20       	bra	0x0xe2b3
    e293:	20 20       	bra	0x0xe2b5
    e295:	20 20       	bra	0x0xe2b7
    e297:	49          	rola
    e298:	4e          	.byte	0x4e
    e299:	20 48       	bra	0x0xe2e3
    e29b:	50          	negb
    e29c:	46          	rora
    e29d:	20 48       	bra	0x0xe2e7
    e29f:	50          	negb
    e2a0:	52          	.byte	0x52
    e2a1:	20 57       	bra	0x0xe2fa
    e2a3:	49          	rola
    e2a4:	4e          	.byte	0x4e
    e2a5:	20 20       	bra	0x0xe2c7
    e2a7:	20 20       	bra	0x0xe2c9
    e2a9:	20 20       	bra	0x0xe2cb
    e2ab:	20 20       	bra	0x0xe2cd
    e2ad:	20 20       	bra	0x0xe2cf
    e2af:	20 20       	bra	0x0xe2d1
    e2b1:	20 20       	bra	0x0xe2d3
    e2b3:	20 20       	bra	0x0xe2d5
    e2b5:	20 7d       	bra	0x0xe334
    e2b7:	01          	nop
    e2b8:	1d 26 0a    	bclr	0x26,x, #0x0a
    e2bb:	bd ec 33    	jsr	0xec33
    e2be:	ce 10 23    	ldx	#0x1023
    e2c1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe2c1
    e2c5:	bd eb 09    	jsr	0xeb09
    e2c8:	ce 01 20    	ldx	#0x120
    e2cb:	18 ce e2 f9 	ldy	#0xe2f9
    e2cf:	c6 20       	ldab	#0x20
    e2d1:	18 a6 00    	ldaa	0x0,y
    e2d4:	a7 00       	staa	0x0,x
    e2d6:	08          	inx
    e2d7:	18 08       	iny
    e2d9:	5a          	decb
    e2da:	26 f5       	bne	0x0xe2d1
    e2dc:	ce e3 19    	ldx	#0xe319
    e2df:	d6 74       	ldab	*0x74
    e2e1:	18 ce 01 30 	ldy	#0x130
    e2e5:	bd ec 6f    	jsr	0xec6f
    e2e8:	ce e4 ba    	ldx	#0xe4ba
    e2eb:	d6 72       	ldab	*0x72
    e2ed:	18 08       	iny
    e2ef:	18 08       	iny
    e2f1:	18 08       	iny
    e2f3:	bd ec 6f    	jsr	0xec6f
    e2f6:	7e ea 46    	jmp	0xea46
    e2f9:	52          	.byte	0x52
    e2fa:	4e          	.byte	0x4e
    e2fb:	47          	asra
    e2fc:	45          	.byte	0x45
    e2fd:	20 20       	bra	0x0xe31f
    e2ff:	53          	comb
    e300:	59          	rolb
    e301:	4e          	.byte	0x4e
    e302:	43          	coma
    e303:	20 20       	bra	0x0xe325
    e305:	20 20       	bra	0x0xe327
    e307:	20 20       	bra	0x0xe329
    e309:	20 20       	bra	0x0xe32b
    e30b:	20 20       	bra	0x0xe32d
    e30d:	20 20       	bra	0x0xe32f
    e30f:	20 20       	bra	0x0xe331
    e311:	20 20       	bra	0x0xe333
    e313:	20 20       	bra	0x0xe335
    e315:	20 20       	bra	0x0xe337
    e317:	20 20       	bra	0x0xe339
    e319:	20 4f       	bra	0x0xe36a
    e31b:	46          	rora
    e31c:	46          	rora
    e31d:	31          	ins
    e31e:	4f          	clra
    e31f:	43          	coma
    e320:	54          	lsrb
    e321:	32          	pula
    e322:	4f          	clra
    e323:	43          	coma
    e324:	54          	lsrb
    e325:	33          	pulb
    e326:	4f          	clra
    e327:	43          	coma
    e328:	54          	lsrb
    e329:	34          	des
    e32a:	4f          	clra
    e32b:	43          	coma
    e32c:	54          	lsrb
    e32d:	7d 01 1d    	tst	0x11d
    e330:	26 0a       	bne	0x0xe33c
    e332:	bd ec 33    	jsr	0xec33
    e335:	ce 10 23    	ldx	#0x1023
    e338:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe338
    e33c:	bd eb 09    	jsr	0xeb09
    e33f:	86 20       	ldaa	#0x20
    e341:	c6 20       	ldab	#0x20
    e343:	ce 01 20    	ldx	#0x120
    e346:	a7 00       	staa	0x0,x
    e348:	08          	inx
    e349:	5a          	decb
    e34a:	26 fa       	bne	0x0xe346
    e34c:	ce e3 d1    	ldx	#0xe3d1
    e34f:	12 f4 02 3f 	brset	*0xf4, #0x02, 0x0xe392
    e353:	d6 2d       	ldab	*0x2d
    e355:	18 ce 01 20 	ldy	#0x120
    e359:	bd ec 6f    	jsr	0xec6f
    e35c:	ce e3 d1    	ldx	#0xe3d1
    e35f:	d6 2e       	ldab	*0x2e
    e361:	18 08       	iny
    e363:	18 08       	iny
    e365:	18 08       	iny
    e367:	bd ec 6f    	jsr	0xec6f
    e36a:	ce e3 d1    	ldx	#0xe3d1
    e36d:	d6 2f       	ldab	*0x2f
    e36f:	18 08       	iny
    e371:	18 08       	iny
    e373:	bd ec 6f    	jsr	0xec6f
    e376:	ce 01 31    	ldx	#0x131
    e379:	96 2a       	ldaa	*0x2a
    e37b:	bd eb bc    	jsr	0xebbc
    e37e:	08          	inx
    e37f:	08          	inx
    e380:	08          	inx
    e381:	08          	inx
    e382:	96 2b       	ldaa	*0x2b
    e384:	bd eb bc    	jsr	0xebbc
    e387:	08          	inx
    e388:	08          	inx
    e389:	08          	inx
    e38a:	96 2c       	ldaa	*0x2c
    e38c:	bd eb bc    	jsr	0xebbc
    e38f:	7e ea 46    	jmp	0xea46
    e392:	d6 3a       	ldab	*0x3a
    e394:	18 ce 01 20 	ldy	#0x120
    e398:	bd ec 6f    	jsr	0xec6f
    e39b:	ce e3 d1    	ldx	#0xe3d1
    e39e:	d6 3b       	ldab	*0x3b
    e3a0:	18 08       	iny
    e3a2:	18 08       	iny
    e3a4:	18 08       	iny
    e3a6:	bd ec 6f    	jsr	0xec6f
    e3a9:	ce e3 d1    	ldx	#0xe3d1
    e3ac:	d6 3c       	ldab	*0x3c
    e3ae:	18 08       	iny
    e3b0:	18 08       	iny
    e3b2:	bd ec 6f    	jsr	0xec6f
    e3b5:	ce 01 31    	ldx	#0x131
    e3b8:	96 37       	ldaa	*0x37
    e3ba:	bd eb bc    	jsr	0xebbc
    e3bd:	08          	inx
    e3be:	08          	inx
    e3bf:	08          	inx
    e3c0:	08          	inx
    e3c1:	96 38       	ldaa	*0x38
    e3c3:	bd eb bc    	jsr	0xebbc
    e3c6:	08          	inx
    e3c7:	08          	inx
    e3c8:	08          	inx
    e3c9:	96 39       	ldaa	*0x39
    e3cb:	bd eb bc    	jsr	0xebbc
    e3ce:	7e ea 46    	jmp	0xea46
    e3d1:	20 4f       	bra	0x0xe422
    e3d3:	46          	rora
    e3d4:	46          	rora
    e3d5:	46          	rora
    e3d6:	52          	.byte	0x52
    e3d7:	45          	.byte	0x45
    e3d8:	31          	ins
    e3d9:	46          	rora
    e3da:	52          	.byte	0x52
    e3db:	45          	.byte	0x45
    e3dc:	32          	pula
    e3dd:	31          	ins
    e3de:	26 32       	bne	0x0xe412
    e3e0:	46          	rora
    e3e1:	4c          	inca
    e3e2:	45          	.byte	0x45
    e3e3:	56          	rorb
    e3e4:	31          	ins
    e3e5:	4c          	inca
    e3e6:	45          	.byte	0x45
    e3e7:	56          	rorb
    e3e8:	32          	pula
    e3e9:	20 50       	bra	0x0xe43b
    e3eb:	57          	asrb
    e3ec:	31          	ins
    e3ed:	20 50       	bra	0x0xe43f
    e3ef:	57          	asrb
    e3f0:	32          	pula
    e3f1:	31          	ins
    e3f2:	26 32       	bne	0x0xe426
    e3f4:	50          	negb
    e3f5:	46          	rora
    e3f6:	49          	rola
    e3f7:	4c          	inca
    e3f8:	54          	lsrb
    e3f9:	52          	.byte	0x52
    e3fa:	45          	.byte	0x45
    e3fb:	53          	comb
    e3fc:	4f          	clra
    e3fd:	4c          	inca
    e3fe:	45          	.byte	0x45
    e3ff:	56          	rorb
    e400:	4e          	.byte	0x4e
    e401:	58          	aslb
    e402:	4d          	tsta
    e403:	4f          	clra
    e404:	44          	lsra
    e405:	20 45       	bra	0x0xe44c
    e407:	41          	.byte	0x41
    e408:	31          	ins
    e409:	20 45       	bra	0x0xe450
    e40b:	41          	.byte	0x41
    e40c:	33          	pulb
    e40d:	20 45       	bra	0x0xe454
    e40f:	58          	aslb
    e410:	54          	lsrb
    e411:	20 56       	bra	0x0xe469
    e413:	4f          	clra
    e414:	4c          	inca
    e415:	bd eb 09    	jsr	0xeb09
    e418:	ce 01 20    	ldx	#0x120
    e41b:	18 ce e4 7a 	ldy	#0xe47a
    e41f:	c6 20       	ldab	#0x20
    e421:	18 a6 00    	ldaa	0x0,y
    e424:	a7 00       	staa	0x0,x
    e426:	08          	inx
    e427:	18 08       	iny
    e429:	5a          	decb
    e42a:	26 f5       	bne	0x0xe421
    e42c:	ce e4 9a    	ldx	#0xe49a
    e42f:	18 ce 01 30 	ldy	#0x130
    e433:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe43e
    e437:	d6 27       	ldab	*0x27
    e439:	bd ec 6f    	jsr	0xec6f
    e43c:	20 05       	bra	0x0xe443
    e43e:	d6 34       	ldab	*0x34
    e440:	bd ec 6f    	jsr	0xec6f
    e443:	ce e4 b2    	ldx	#0xe4b2
    e446:	18 08       	iny
    e448:	18 08       	iny
    e44a:	18 08       	iny
    e44c:	12 f4 02 09 	brset	*0xf4, #0x02, 0x0xe459
    e450:	d6 31       	ldab	*0x31
    e452:	c4 01       	andb	#0x1
    e454:	bd ec 6f    	jsr	0xec6f
    e457:	20 06       	bra	0x0xe45f
    e459:	d6 31       	ldab	*0x31
    e45b:	54          	lsrb
    e45c:	bd ec 6f    	jsr	0xec6f
    e45f:	ce e4 ba    	ldx	#0xe4ba
    e462:	18 08       	iny
    e464:	18 08       	iny
    e466:	12 f4 02 08 	brset	*0xf4, #0x02, 0x0xe472
    e46a:	d6 25       	ldab	*0x25
    e46c:	bd ec 6f    	jsr	0xec6f
    e46f:	7e ea 46    	jmp	0xea46
    e472:	d6 32       	ldab	*0x32
    e474:	bd ec 6f    	jsr	0xec6f
    e477:	7e ea 46    	jmp	0xea46
    e47a:	57          	asrb
    e47b:	41          	.byte	0x41
    e47c:	56          	rorb
    e47d:	45          	.byte	0x45
    e47e:	20 20       	bra	0x0xe4a0
    e480:	4d          	tsta
    e481:	4f          	clra
    e482:	44          	lsra
    e483:	45          	.byte	0x45
    e484:	20 53       	bra	0x0xe4d9
    e486:	59          	rolb
    e487:	4e          	.byte	0x4e
    e488:	43          	coma
    e489:	20 20       	bra	0x0xe4ab
    e48b:	20 20       	bra	0x0xe4ad
    e48d:	20 20       	bra	0x0xe4af
    e48f:	20 20       	bra	0x0xe4b1
    e491:	20 20       	bra	0x0xe4b3
    e493:	20 20       	bra	0x0xe4b5
    e495:	20 20       	bra	0x0xe4b7
    e497:	20 20       	bra	0x0xe4b9
    e499:	20 20       	bra	0x0xe4bb
    e49b:	54          	lsrb
    e49c:	52          	.byte	0x52
    e49d:	49          	rola
    e49e:	20 53       	bra	0x0xe4f3
    e4a0:	51          	.byte	0x51
    e4a1:	52          	.byte	0x52
    e4a2:	53          	comb
    e4a3:	57          	asrb
    e4a4:	55          	.byte	0x55
    e4a5:	50          	negb
    e4a6:	53          	comb
    e4a7:	57          	asrb
    e4a8:	44          	lsra
    e4a9:	4e          	.byte	0x4e
    e4aa:	52          	.byte	0x52
    e4ab:	41          	.byte	0x41
    e4ac:	4e          	.byte	0x4e
    e4ad:	44          	lsra
    e4ae:	20 53       	bra	0x0xe503
    e4b0:	2f 48       	ble	0x0xe4fa
    e4b2:	4d          	tsta
    e4b3:	4f          	clra
    e4b4:	4e          	.byte	0x4e
    e4b5:	4f          	clra
    e4b6:	50          	negb
    e4b7:	4f          	clra
    e4b8:	4c          	inca
    e4b9:	59          	rolb
    e4ba:	53          	comb
    e4bb:	45          	.byte	0x45
    e4bc:	4c          	inca
    e4bd:	46          	rora
    e4be:	20 20       	bra	0x0xe4e0
    e4c0:	20 34       	bra	0x0xe4f6
    e4c2:	20 20       	bra	0x0xe4e4
    e4c4:	20 32       	bra	0x0xe4f8
    e4c6:	20 20       	bra	0x0xe4e8
    e4c8:	20 31       	bra	0x0xe4fb
    e4ca:	20 20       	bra	0x0xe4ec
    e4cc:	31          	ins
    e4cd:	54          	lsrb
    e4ce:	20 31       	bra	0x0xe501
    e4d0:	2f 32       	ble	0x0xe504
    e4d2:	31          	ins
    e4d3:	2f 32       	ble	0x0xe507
    e4d5:	54          	lsrb
    e4d6:	20 31       	bra	0x0xe509
    e4d8:	2f 34       	ble	0x0xe50e
    e4da:	31          	ins
    e4db:	2f 34       	ble	0x0xe511
    e4dd:	54          	lsrb
    e4de:	20 31       	bra	0x0xe511
    e4e0:	2f 38       	ble	0x0xe51a
    e4e2:	31          	ins
    e4e3:	2f 38       	ble	0x0xe51d
    e4e5:	54          	lsrb
    e4e6:	31          	ins
    e4e7:	2f 31       	ble	0x0xe51a
    e4e9:	36          	psha
    e4ea:	20 31       	bra	0x0xe51d
    e4ec:	36          	psha
    e4ed:	54          	lsrb
    e4ee:	bd eb 09    	jsr	0xeb09
    e4f1:	ce 01 20    	ldx	#0x120
    e4f4:	18 ce e5 2b 	ldy	#0xe52b
    e4f8:	c6 20       	ldab	#0x20
    e4fa:	18 a6 00    	ldaa	0x0,y
    e4fd:	a7 00       	staa	0x0,x
    e4ff:	08          	inx
    e500:	18 08       	iny
    e502:	5a          	decb
    e503:	26 f5       	bne	0x0xe4fa
    e505:	ce e5 54    	ldx	#0xe554
    e508:	18 ce 01 37 	ldy	#0x137
    e50c:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe517
    e510:	d6 26       	ldab	*0x26
    e512:	bd ec 5b    	jsr	0xec5b
    e515:	20 05       	bra	0x0xe51c
    e517:	d6 33       	ldab	*0x33
    e519:	bd ec 5b    	jsr	0xec5b
    e51c:	d6 96       	ldab	*0x96
    e51e:	ce e5 63    	ldx	#0xe563
    e521:	18 ce 01 3c 	ldy	#0x13c
    e525:	bd ec 5b    	jsr	0xec5b
    e528:	7e ea 46    	jmp	0xea46
    e52b:	20 20       	bra	0x0xe54d
    e52d:	20 20       	bra	0x0xe54f
    e52f:	20 20       	bra	0x0xe551
    e531:	20 4b       	bra	0x0xe57e
    e533:	45          	.byte	0x45
    e534:	59          	rolb
    e535:	20 20       	bra	0x0xe557
    e537:	51          	.byte	0x51
    e538:	55          	.byte	0x55
    e539:	41          	.byte	0x41
    e53a:	4e          	.byte	0x4e
    e53b:	20 20       	bra	0x0xe55d
    e53d:	20 20       	bra	0x0xe55f
    e53f:	20 20       	bra	0x0xe561
    e541:	20 20       	bra	0x0xe563
    e543:	20 20       	bra	0x0xe565
    e545:	20 20       	bra	0x0xe567
    e547:	20 20       	bra	0x0xe569
    e549:	20 20       	bra	0x0xe56b
    e54b:	4c          	inca
    e54c:	4f          	clra
    e54d:	57          	asrb
    e54e:	4d          	tsta
    e54f:	45          	.byte	0x45
    e550:	44          	lsra
    e551:	20 48       	bra	0x0xe59b
    e553:	49          	rola
    e554:	4f          	clra
    e555:	46          	rora
    e556:	46          	rora
    e557:	20 55       	bra	0x0xe5ae
    e559:	50          	negb
    e55a:	20 44       	bra	0x0xe5a0
    e55c:	4e          	.byte	0x4e
    e55d:	55          	.byte	0x55
    e55e:	50          	negb
    e55f:	31          	ins
    e560:	44          	lsra
    e561:	4e          	.byte	0x4e
    e562:	31          	ins
    e563:	4f          	clra
    e564:	46          	rora
    e565:	46          	rora
    e566:	4c          	inca
    e567:	46          	rora
    e568:	31          	ins
    e569:	4c          	inca
    e56a:	46          	rora
    e56b:	32          	pula
    e56c:	31          	ins
    e56d:	26 32       	bne	0x0xe5a1
    e56f:	7d 01 1d    	tst	0x11d
    e572:	26 0a       	bne	0x0xe57e
    e574:	bd ec 33    	jsr	0xec33
    e577:	ce 10 23    	ldx	#0x1023
    e57a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe57a
    e57e:	bd eb 09    	jsr	0xeb09
    e581:	86 20       	ldaa	#0x20
    e583:	c6 20       	ldab	#0x20
    e585:	ce 01 20    	ldx	#0x120
    e588:	a7 00       	staa	0x0,x
    e58a:	08          	inx
    e58b:	5a          	decb
    e58c:	26 fa       	bne	0x0xe588
    e58e:	ce 01 20    	ldx	#0x120
    e591:	18 ce 00 b0 	ldy	#0xb0
    e595:	96 f9       	ldaa	*0xf9
    e597:	81 04       	cmpa	#0x4
    e599:	26 04       	bne	0x0xe59f
    e59b:	18 ce 50 30 	ldy	#0x5030
    e59f:	18 a6 00    	ldaa	0x0,y
    e5a2:	bd eb f8    	jsr	0xebf8
    e5a5:	08          	inx
    e5a6:	08          	inx
    e5a7:	08          	inx
    e5a8:	08          	inx
    e5a9:	8c 01 2f    	cpx	#0x12f
    e5ac:	26 01       	bne	0x0xe5af
    e5ae:	08          	inx
    e5af:	18 08       	iny
    e5b1:	8c 01 3f    	cpx	#0x13f
    e5b4:	25 e9       	bcs	0x0xe59f
    e5b6:	7e ea 46    	jmp	0xea46
    e5b9:	bd eb 09    	jsr	0xeb09
    e5bc:	86 20       	ldaa	#0x20
    e5be:	c6 20       	ldab	#0x20
    e5c0:	ce 01 20    	ldx	#0x120
    e5c3:	a7 00       	staa	0x0,x
    e5c5:	08          	inx
    e5c6:	5a          	decb
    e5c7:	26 fa       	bne	0x0xe5c3
    e5c9:	ce 01 20    	ldx	#0x120
    e5cc:	18 ce 00 a8 	ldy	#0xa8
    e5d0:	96 f9       	ldaa	*0xf9
    e5d2:	81 04       	cmpa	#0x4
    e5d4:	26 04       	bne	0x0xe5da
    e5d6:	18 ce 50 28 	ldy	#0x5028
    e5da:	18 a6 00    	ldaa	0x0,y
    e5dd:	bd eb bc    	jsr	0xebbc
    e5e0:	08          	inx
    e5e1:	08          	inx
    e5e2:	8c 01 2f    	cpx	#0x12f
    e5e5:	26 01       	bne	0x0xe5e8
    e5e7:	08          	inx
    e5e8:	18 08       	iny
    e5ea:	8c 01 3f    	cpx	#0x13f
    e5ed:	25 eb       	bcs	0x0xe5da
    e5ef:	7e ea 46    	jmp	0xea46
    e5f2:	bd eb 09    	jsr	0xeb09
    e5f5:	86 20       	ldaa	#0x20
    e5f7:	c6 20       	ldab	#0x20
    e5f9:	ce 01 20    	ldx	#0x120
    e5fc:	a7 00       	staa	0x0,x
    e5fe:	08          	inx
    e5ff:	5a          	decb
    e600:	26 fa       	bne	0x0xe5fc
    e602:	ce 01 20    	ldx	#0x120
    e605:	18 ce 00 b8 	ldy	#0xb8
    e609:	96 f9       	ldaa	*0xf9
    e60b:	81 04       	cmpa	#0x4
    e60d:	26 04       	bne	0x0xe613
    e60f:	18 ce 50 38 	ldy	#0x5038
    e613:	18 a6 00    	ldaa	0x0,y
    e616:	bd eb bc    	jsr	0xebbc
    e619:	08          	inx
    e61a:	08          	inx
    e61b:	8c 01 2f    	cpx	#0x12f
    e61e:	26 01       	bne	0x0xe621
    e620:	08          	inx
    e621:	18 08       	iny
    e623:	8c 01 3f    	cpx	#0x13f
    e626:	25 eb       	bcs	0x0xe613
    e628:	7e ea 46    	jmp	0xea46
    e62b:	bd eb 09    	jsr	0xeb09
    e62e:	ce 01 20    	ldx	#0x120
    e631:	18 ce e6 69 	ldy	#0xe669
    e635:	c6 20       	ldab	#0x20
    e637:	18 a6 00    	ldaa	0x0,y
    e63a:	a7 00       	staa	0x0,x
    e63c:	08          	inx
    e63d:	18 08       	iny
    e63f:	5a          	decb
    e640:	26 f5       	bne	0x0xe637
    e642:	ce e4 9a    	ldx	#0xe49a
    e645:	d6 a7       	ldab	*0xa7
    e647:	18 ce 01 30 	ldy	#0x130
    e64b:	bd ec 6f    	jsr	0xec6f
    e64e:	ce e5 54    	ldx	#0xe554
    e651:	d6 a6       	ldab	*0xa6
    e653:	18 ce 01 37 	ldy	#0x137
    e657:	bd ec 5b    	jsr	0xec5b
    e65a:	ce e4 ba    	ldx	#0xe4ba
    e65d:	d6 a5       	ldab	*0xa5
    e65f:	18 ce 01 3b 	ldy	#0x13b
    e663:	bd ec 6f    	jsr	0xec6f
    e666:	7e ea 46    	jmp	0xea46
    e669:	57          	asrb
    e66a:	41          	.byte	0x41
    e66b:	56          	rorb
    e66c:	45          	.byte	0x45
    e66d:	20 20       	bra	0x0xe68f
    e66f:	20 4b       	bra	0x0xe6bc
    e671:	45          	.byte	0x45
    e672:	59          	rolb
    e673:	20 53       	bra	0x0xe6c8
    e675:	59          	rolb
    e676:	4e          	.byte	0x4e
    e677:	43          	coma
    e678:	20 20       	bra	0x0xe69a
    e67a:	20 20       	bra	0x0xe69c
    e67c:	20 20       	bra	0x0xe69e
    e67e:	20 20       	bra	0x0xe6a0
    e680:	20 20       	bra	0x0xe6a2
    e682:	20 20       	bra	0x0xe6a4
    e684:	20 20       	bra	0x0xe6a6
    e686:	20 20       	bra	0x0xe6a8
    e688:	20 bd       	bra	0x0xe647
    e68a:	eb 09       	addb	0x9,x
    e68c:	ce 01 20    	ldx	#0x120
    e68f:	18 ce e6 df 	ldy	#0xe6df
    e693:	c6 20       	ldab	#0x20
    e695:	18 a6 00    	ldaa	0x0,y
    e698:	a7 00       	staa	0x0,x
    e69a:	08          	inx
    e69b:	18 08       	iny
    e69d:	5a          	decb
    e69e:	26 f5       	bne	0x0xe695
    e6a0:	ce da ec    	ldx	#0xdaec
    e6a3:	18 ce 01 30 	ldy	#0x130
    e6a7:	13 21 02 07 	brclr	*0x21, #0x02, 0x0xe6b2
    e6ab:	c6 01       	ldab	#0x1
    e6ad:	bd ec 5b    	jsr	0xec5b
    e6b0:	20 04       	bra	0x0xe6b6
    e6b2:	5f          	clrb
    e6b3:	bd ec 5b    	jsr	0xec5b
    e6b6:	18 ce 01 34 	ldy	#0x134
    e6ba:	ce da ec    	ldx	#0xdaec
    e6bd:	12 21 04 06 	brset	*0x21, #0x04, 0x0xe6c7
    e6c1:	5f          	clrb
    e6c2:	bd ec 5b    	jsr	0xec5b
    e6c5:	20 05       	bra	0x0xe6cc
    e6c7:	c6 01       	ldab	#0x1
    e6c9:	bd ec 5b    	jsr	0xec5b
    e6cc:	96 22       	ldaa	*0x22
    e6ce:	ce 01 38    	ldx	#0x138
    e6d1:	bd eb f8    	jsr	0xebf8
    e6d4:	96 23       	ldaa	*0x23
    e6d6:	ce 01 3c    	ldx	#0x13c
    e6d9:	bd eb bc    	jsr	0xebbc
    e6dc:	7e ea 46    	jmp	0xea46
    e6df:	47          	asra
    e6e0:	4c          	inca
    e6e1:	53          	comb
    e6e2:	20 41       	bra	0x0xe725
    e6e4:	55          	.byte	0x55
    e6e5:	54          	lsrb
    e6e6:	20 49       	bra	0x0xe731
    e6e8:	4e          	.byte	0x4e
    e6e9:	54          	lsrb
    e6ea:	20 44       	bra	0x0xe730
    e6ec:	59          	rolb
    e6ed:	4e          	.byte	0x4e
    e6ee:	20 20       	bra	0x0xe710
    e6f0:	20 20       	bra	0x0xe712
    e6f2:	20 20       	bra	0x0xe714
    e6f4:	20 20       	bra	0x0xe716
    e6f6:	20 20       	bra	0x0xe718
    e6f8:	20 20       	bra	0x0xe71a
    e6fa:	20 20       	bra	0x0xe71c
    e6fc:	20 20       	bra	0x0xe71e
    e6fe:	20 7d       	bra	0x0xe77d
    e700:	01          	nop
    e701:	1d 26 0a    	bclr	0x26,x, #0x0a
    e704:	bd ec 33    	jsr	0xec33
    e707:	ce 10 23    	ldx	#0x1023
    e70a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe70a
    e70e:	bd eb 09    	jsr	0xeb09
    e711:	ce 01 20    	ldx	#0x120
    e714:	18 ce e7 33 	ldy	#0xe733
    e718:	c6 20       	ldab	#0x20
    e71a:	18 a6 00    	ldaa	0x0,y
    e71d:	a7 00       	staa	0x0,x
    e71f:	08          	inx
    e720:	18 08       	iny
    e722:	5a          	decb
    e723:	26 f5       	bne	0x0xe71a
    e725:	ce d8 7c    	ldx	#0xd87c
    e728:	5f          	clrb
    e729:	18 ce 01 31 	ldy	#0x131
    e72d:	bd ec 5b    	jsr	0xec5b
    e730:	7e ea 46    	jmp	0xea46
    e733:	20 54       	bra	0x0xe789
    e735:	55          	.byte	0x55
    e736:	4e          	.byte	0x4e
    e737:	45          	.byte	0x45
    e738:	3f          	swi
    e739:	20 20       	bra	0x0xe75b
    e73b:	20 20       	bra	0x0xe75d
    e73d:	20 20       	bra	0x0xe75f
    e73f:	20 20       	bra	0x0xe761
    e741:	20 20       	bra	0x0xe763
    e743:	20 20       	bra	0x0xe765
    e745:	20 20       	bra	0x0xe767
    e747:	20 20       	bra	0x0xe769
    e749:	20 20       	bra	0x0xe76b
    e74b:	20 20       	bra	0x0xe76d
    e74d:	20 20       	bra	0x0xe76f
    e74f:	20 20       	bra	0x0xe771
    e751:	20 20       	bra	0x0xe773
    e753:	7d 01 1d    	tst	0x11d
    e756:	26 0a       	bne	0x0xe762
    e758:	bd ec 33    	jsr	0xec33
    e75b:	ce 10 23    	ldx	#0x1023
    e75e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe75e
    e762:	bd eb 09    	jsr	0xeb09
    e765:	ce 01 20    	ldx	#0x120
    e768:	18 ce e7 8b 	ldy	#0xe78b
    e76c:	c6 20       	ldab	#0x20
    e76e:	18 a6 00    	ldaa	0x0,y
    e771:	a7 00       	staa	0x0,x
    e773:	08          	inx
    e774:	18 08       	iny
    e776:	5a          	decb
    e777:	26 f5       	bne	0x0xe76e
    e779:	f6 10 28    	ldab	0x1028
    e77c:	c4 03       	andb	#0x3
    e77e:	ce e7 ab    	ldx	#0xe7ab
    e781:	18 ce 01 30 	ldy	#0x130
    e785:	bd ec 6f    	jsr	0xec6f
    e788:	7e ea 46    	jmp	0xea46
    e78b:	53          	comb
    e78c:	45          	.byte	0x45
    e78d:	54          	lsrb
    e78e:	20 53       	bra	0x0xe7e3
    e790:	50          	negb
    e791:	49          	rola
    e792:	20 42       	bra	0x0xe7d6
    e794:	49          	rola
    e795:	54          	lsrb
    e796:	20 52       	bra	0x0xe7ea
    e798:	41          	.byte	0x41
    e799:	54          	lsrb
    e79a:	45          	.byte	0x45
    e79b:	20 20       	bra	0x0xe7bd
    e79d:	20 20       	bra	0x0xe7bf
    e79f:	20 4b       	bra	0x0xe7ec
    e7a1:	42          	.byte	0x42
    e7a2:	49          	rola
    e7a3:	54          	lsrb
    e7a4:	2f 53       	ble	0x0xe7f9
    e7a6:	45          	.byte	0x45
    e7a7:	43          	coma
    e7a8:	20 20       	bra	0x0xe7ca
    e7aa:	20 20       	bra	0x0xe7cc
    e7ac:	20 31       	bra	0x0xe7df
    e7ae:	4b          	.byte	0x4b
    e7af:	20 35       	bra	0x0xe7e6
    e7b1:	30          	tsx
    e7b2:	30          	tsx
    e7b3:	20 31       	bra	0x0xe7e6
    e7b5:	32          	pula
    e7b6:	35          	txs
    e7b7:	36          	psha
    e7b8:	32          	pula
    e7b9:	2e 35       	bgt	0x0xe7f0
    e7bb:	7d 01 1d    	tst	0x11d
    e7be:	26 0a       	bne	0x0xe7ca
    e7c0:	bd ec 33    	jsr	0xec33
    e7c3:	ce 10 23    	ldx	#0x1023
    e7c6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe7c6
    e7ca:	bd eb 09    	jsr	0xeb09
    e7cd:	ce 01 20    	ldx	#0x120
    e7d0:	18 ce e7 ed 	ldy	#0xe7ed
    e7d4:	c6 20       	ldab	#0x20
    e7d6:	18 a6 00    	ldaa	0x0,y
    e7d9:	a7 00       	staa	0x0,x
    e7db:	08          	inx
    e7dc:	18 08       	iny
    e7de:	5a          	decb
    e7df:	26 f5       	bne	0x0xe7d6
    e7e1:	96 f0       	ldaa	*0xf0
    e7e3:	4c          	inca
    e7e4:	ce 01 31    	ldx	#0x131
    e7e7:	bd eb bc    	jsr	0xebbc
    e7ea:	7e ea 46    	jmp	0xea46
    e7ed:	53          	comb
    e7ee:	45          	.byte	0x45
    e7ef:	54          	lsrb
    e7f0:	20 23       	bra	0x0xe815
    e7f2:	20 4f       	bra	0x0xe843
    e7f4:	46          	rora
    e7f5:	20 56       	bra	0x0xe84d
    e7f7:	4f          	clra
    e7f8:	49          	rola
    e7f9:	43          	coma
    e7fa:	45          	.byte	0x45
    e7fb:	53          	comb
    e7fc:	20 20       	bra	0x0xe81e
    e7fe:	20 20       	bra	0x0xe820
    e800:	20 20       	bra	0x0xe822
    e802:	20 20       	bra	0x0xe824
    e804:	20 20       	bra	0x0xe826
    e806:	20 20       	bra	0x0xe828
    e808:	20 20       	bra	0x0xe82a
    e80a:	20 20       	bra	0x0xe82c
    e80c:	20 7d       	bra	0x0xe88b
    e80e:	01          	nop
    e80f:	1d 26 0a    	bclr	0x26,x, #0x0a
    e812:	bd ec 33    	jsr	0xec33
    e815:	ce 10 23    	ldx	#0x1023
    e818:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe818
    e81c:	bd eb 09    	jsr	0xeb09
    e81f:	ce 01 20    	ldx	#0x120
    e822:	18 ce e8 f7 	ldy	#0xe8f7
    e826:	c6 20       	ldab	#0x20
    e828:	b6 50 00    	ldaa	0x5000
    e82b:	81 06       	cmpa	#0x6
    e82d:	25 04       	bcs	0x0xe833
    e82f:	18 ce e9 17 	ldy	#0xe917
    e833:	18 a6 00    	ldaa	0x0,y
    e836:	a7 00       	staa	0x0,x
    e838:	08          	inx
    e839:	18 08       	iny
    e83b:	5a          	decb
    e83c:	26 f5       	bne	0x0xe833
    e83e:	b6 01 6b    	ldaa	0x16b
    e841:	8b 31       	adda	#0x31
    e843:	b7 01 25    	staa	0x125
    e846:	ce 50 01    	ldx	#0x5001
    e849:	b6 01 6b    	ldaa	0x16b
    e84c:	c6 04       	ldab	#0x4
    e84e:	3d          	mul
    e84f:	3a          	abx
    e850:	a6 00       	ldaa	0x0,x
    e852:	8b 41       	adda	#0x41
    e854:	b7 01 32    	staa	0x132
    e857:	a6 01       	ldaa	0x1,x
    e859:	3c          	pshx
    e85a:	4c          	inca
    e85b:	ce 01 34    	ldx	#0x134
    e85e:	bd eb bc    	jsr	0xebbc
    e861:	b6 50 00    	ldaa	0x5000
    e864:	81 06       	cmpa	#0x6
    e866:	27 0c       	beq	0x0xe874
    e868:	38          	pulx
    e869:	a6 03       	ldaa	0x3,x
    e86b:	3c          	pshx
    e86c:	ce 01 3c    	ldx	#0x13c
    e86f:	bd eb bc    	jsr	0xebbc
    e872:	20 42       	bra	0x0xe8b6
    e874:	38          	pulx
    e875:	7d 01 6b    	tst	0x16b
    e878:	26 06       	bne	0x0xe880
    e87a:	96 f0       	ldaa	*0xf0
    e87c:	4c          	inca
    e87d:	b7 50 23    	staa	0x5023
    e880:	e6 02       	ldab	0x2,x
    e882:	c4 40       	andb	#0x40
    e884:	f7 01 7d    	stab	0x17d
    e887:	e6 02       	ldab	0x2,x
    e889:	c4 3f       	andb	#0x3f
    e88b:	b6 50 23    	ldaa	0x5023
    e88e:	11          	cba
    e88f:	24 03       	bcc	0x0xe894
    e891:	5a          	decb
    e892:	20 fa       	bra	0x0xe88e
    e894:	10          	sba
    e895:	b7 50 23    	staa	0x5023
    e898:	37          	pshb
    e899:	fa 01 7d    	orab	0x17d
    e89c:	e7 02       	stab	0x2,x
    e89e:	32          	pula
    e89f:	3c          	pshx
    e8a0:	ce 01 38    	ldx	#0x138
    e8a3:	bd eb bc    	jsr	0xebbc
    e8a6:	38          	pulx
    e8a7:	e6 03       	ldab	0x3,x
    e8a9:	c4 01       	andb	#0x1
    e8ab:	3c          	pshx
    e8ac:	ce e9 37    	ldx	#0xe937
    e8af:	18 ce 01 3c 	ldy	#0x13c
    e8b3:	bd ec 5b    	jsr	0xec5b
    e8b6:	38          	pulx
    e8b7:	e6 00       	ldab	0x0,x
    e8b9:	c1 01       	cmpb	#0x1
    e8bb:	23 17       	bls	0x0xe8d4
    e8bd:	a6 01       	ldaa	0x1,x
    e8bf:	d7 f9       	stab	*0xf9
    e8c1:	bd ad c4    	jsr	0xadc4
    e8c4:	c6 04       	ldab	#0x4
    e8c6:	d7 f9       	stab	*0xf9
    e8c8:	bd a5 50    	jsr	0xa550
    e8cb:	bd a5 6b    	jsr	0xa56b
    e8ce:	bd ad c4    	jsr	0xadc4
    e8d1:	7e ea 46    	jmp	0xea46
    e8d4:	4f          	clra
    e8d5:	b7 10 22    	staa	0x1022
    e8d8:	b6 10 2d    	ldaa	0x102d
    e8db:	36          	psha
    e8dc:	84 7f       	anda	#0x7f
    e8de:	b7 10 2d    	staa	0x102d
    e8e1:	a6 00       	ldaa	0x0,x
    e8e3:	e6 01       	ldab	0x1,x
    e8e5:	bd 79 00    	jsr	0x7900
    e8e8:	32          	pula
    e8e9:	b7 10 2d    	staa	0x102d
    e8ec:	86 80       	ldaa	#0x80
    e8ee:	b7 10 22    	staa	0x1022
    e8f1:	bd a5 6b    	jsr	0xa56b
    e8f4:	7e ea 46    	jmp	0xea46
    e8f7:	50          	negb
    e8f8:	41          	.byte	0x41
    e8f9:	54          	lsrb
    e8fa:	43          	coma
    e8fb:	48          	asla
    e8fc:	20 20       	bra	0x0xe91e
    e8fe:	20 20       	bra	0x0xe920
    e900:	20 20       	bra	0x0xe922
    e902:	20 56       	bra	0x0xe95a
    e904:	4f          	clra
    e905:	4c          	inca
    e906:	20 20       	bra	0x0xe928
    e908:	20 20       	bra	0x0xe92a
    e90a:	20 20       	bra	0x0xe92c
    e90c:	20 20       	bra	0x0xe92e
    e90e:	20 20       	bra	0x0xe930
    e910:	20 20       	bra	0x0xe932
    e912:	20 20       	bra	0x0xe934
    e914:	20 20       	bra	0x0xe936
    e916:	20 50       	bra	0x0xe968
    e918:	41          	.byte	0x41
    e919:	54          	lsrb
    e91a:	43          	coma
    e91b:	48          	asla
    e91c:	20 20       	bra	0x0xe93e
    e91e:	23 56       	bls	0x0xe976
    e920:	43          	coma
    e921:	53          	comb
    e922:	20 54       	bra	0x0xe978
    e924:	59          	rolb
    e925:	50          	negb
    e926:	45          	.byte	0x45
    e927:	20 20       	bra	0x0xe949
    e929:	20 20       	bra	0x0xe94b
    e92b:	20 20       	bra	0x0xe94d
    e92d:	20 20       	bra	0x0xe94f
    e92f:	20 20       	bra	0x0xe951
    e931:	20 20       	bra	0x0xe953
    e933:	20 20       	bra	0x0xe955
    e935:	20 20       	bra	0x0xe957
    e937:	4d          	tsta
    e938:	4f          	clra
    e939:	4e          	.byte	0x4e
    e93a:	50          	negb
    e93b:	4c          	inca
    e93c:	59          	rolb
    e93d:	7d 01 1d    	tst	0x11d
    e940:	26 0a       	bne	0x0xe94c
    e942:	bd ec 33    	jsr	0xec33
    e945:	ce 10 23    	ldx	#0x1023
    e948:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe948
    e94c:	bd eb 09    	jsr	0xeb09
    e94f:	ce 01 20    	ldx	#0x120
    e952:	18 ce e9 8e 	ldy	#0xe98e
    e956:	c6 20       	ldab	#0x20
    e958:	18 a6 00    	ldaa	0x0,y
    e95b:	a7 00       	staa	0x0,x
    e95d:	08          	inx
    e95e:	18 08       	iny
    e960:	5a          	decb
    e961:	26 f5       	bne	0x0xe958
    e963:	ce 50 03    	ldx	#0x5003
    e966:	b6 01 6b    	ldaa	0x16b
    e969:	c6 04       	ldab	#0x4
    e96b:	3d          	mul
    e96c:	3a          	abx
    e96d:	7d 50 23    	tst	0x5023
    e970:	26 07       	bne	0x0xe979
    e972:	e6 00       	ldab	0x0,x
    e974:	c4 3f       	andb	#0x3f
    e976:	f7 50 23    	stab	0x5023
    e979:	e6 00       	ldab	0x0,x
    e97b:	c4 40       	andb	#0x40
    e97d:	27 02       	beq	0x0xe981
    e97f:	c6 01       	ldab	#0x1
    e981:	18 ce 01 31 	ldy	#0x131
    e985:	ce e9 ae    	ldx	#0xe9ae
    e988:	bd ec 5b    	jsr	0xec5b
    e98b:	7e ea 46    	jmp	0xea46
    e98e:	32          	pula
    e98f:	4d          	tsta
    e990:	49          	rola
    e991:	58          	aslb
    e992:	20 20       	bra	0x0xe9b4
    e994:	20 20       	bra	0x0xe9b6
    e996:	20 20       	bra	0x0xe9b8
    e998:	20 20       	bra	0x0xe9ba
    e99a:	20 20       	bra	0x0xe9bc
    e99c:	20 20       	bra	0x0xe9be
    e99e:	20 20       	bra	0x0xe9c0
    e9a0:	20 20       	bra	0x0xe9c2
    e9a2:	20 20       	bra	0x0xe9c4
    e9a4:	20 20       	bra	0x0xe9c6
    e9a6:	20 20       	bra	0x0xe9c8
    e9a8:	20 20       	bra	0x0xe9ca
    e9aa:	20 20       	bra	0x0xe9cc
    e9ac:	20 20       	bra	0x0xe9ce
    e9ae:	4f          	clra
    e9af:	46          	rora
    e9b0:	46          	rora
    e9b1:	20 4f       	bra	0x0xea02
    e9b3:	4e          	.byte	0x4e
    e9b4:	ce 10 23    	ldx	#0x1023
    e9b7:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe9b7
    e9bb:	bd eb 09    	jsr	0xeb09
    e9be:	ce 01 20    	ldx	#0x120
    e9c1:	18 ce e9 d5 	ldy	#0xe9d5
    e9c5:	c6 20       	ldab	#0x20
    e9c7:	18 a6 00    	ldaa	0x0,y
    e9ca:	a7 00       	staa	0x0,x
    e9cc:	08          	inx
    e9cd:	18 08       	iny
    e9cf:	5a          	decb
    e9d0:	26 f5       	bne	0x0xe9c7
    e9d2:	7e ea 46    	jmp	0xea46
    e9d5:	55          	.byte	0x55
    e9d6:	50          	negb
    e9d7:	4c          	inca
    e9d8:	4f          	clra
    e9d9:	41          	.byte	0x41
    e9da:	44          	lsra
    e9db:	20 52       	bra	0x0xea2f
    e9dd:	41          	.byte	0x41
    e9de:	4d          	tsta
    e9df:	20 42       	bra	0x0xea23
    e9e1:	4e          	.byte	0x4e
    e9e2:	4b          	.byte	0x4b
    e9e3:	20 43       	bra	0x0xea28
    e9e5:	20 20       	bra	0x0xea07
    e9e7:	20 20       	bra	0x0xea09
    e9e9:	54          	lsrb
    e9ea:	4f          	clra
    e9eb:	20 52       	bra	0x0xea3f
    e9ed:	4f          	clra
    e9ee:	4d          	tsta
    e9ef:	20 42       	bra	0x0xea33
    e9f1:	4e          	.byte	0x4e
    e9f2:	4b          	.byte	0x4b
    e9f3:	20 41       	bra	0x0xea36
    e9f5:	b6 01 2f    	ldaa	0x12f
    e9f8:	97 99       	staa	*0x99
    e9fa:	b6 01 3f    	ldaa	0x13f
    e9fd:	97 9a       	staa	*0x9a
    e9ff:	ce 10 23    	ldx	#0x1023
    ea02:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xea02
    ea06:	bd eb 09    	jsr	0xeb09
    ea09:	ce 01 20    	ldx	#0x120
    ea0c:	18 ce ea 26 	ldy	#0xea26
    ea10:	c6 20       	ldab	#0x20
    ea12:	18 a6 00    	ldaa	0x0,y
    ea15:	a7 00       	staa	0x0,x
    ea17:	08          	inx
    ea18:	18 08       	iny
    ea1a:	5a          	decb
    ea1b:	26 f5       	bne	0x0xea12
    ea1d:	96 f2       	ldaa	*0xf2
    ea1f:	84 20       	anda	#0x20
    ea21:	97 f2       	staa	*0xf2
    ea23:	7e ea 46    	jmp	0xea46
    ea26:	20 41       	bra	0x0xea69
    ea28:	52          	.byte	0x52
    ea29:	45          	.byte	0x45
    ea2a:	20 59       	bra	0x0xea85
    ea2c:	4f          	clra
    ea2d:	55          	.byte	0x55
    ea2e:	20 53       	bra	0x0xea83
    ea30:	55          	.byte	0x55
    ea31:	52          	.byte	0x52
    ea32:	45          	.byte	0x45
    ea33:	20 3f       	bra	0x0xea74
    ea35:	20 20       	bra	0x0xea57
    ea37:	20 20       	bra	0x0xea59
    ea39:	20 20       	bra	0x0xea5b
    ea3b:	20 20       	bra	0x0xea5d
    ea3d:	20 20       	bra	0x0xea5f
    ea3f:	20 20       	bra	0x0xea61
    ea41:	20 20       	bra	0x0xea63
    ea43:	4e          	.byte	0x4e
    ea44:	4f          	clra
    ea45:	20 7f       	bra	0x0xeac6
    ea47:	01          	nop
    ea48:	1e 86 20 b7 	brset	0x86,x, #0x20, 0x0xea03
    ea4c:	01          	nop
    ea4d:	1c ce 10    	bset	0xce,x, #0x10
    ea50:	23 1f       	bls	0x0xea71
    ea52:	00          	bgnd
    ea53:	10          	sba
    ea54:	fc 7e ea    	ldd	0x7eea
    ea57:	58          	aslb
    ea58:	8d 03       	bsr	0x0xea5d
    ea5a:	7e d5 2f    	jmp	0xd52f
    ea5d:	ce 01 20    	ldx	#0x120
    ea60:	f6 01 1c    	ldab	0x11c
    ea63:	5a          	decb
    ea64:	3a          	abx
    ea65:	a6 00       	ldaa	0x0,x
    ea67:	b7 10 47    	staa	0x1047
    ea6a:	86 88       	ldaa	#0x88
    ea6c:	ba 10 00    	oraa	0x1000
    ea6f:	b7 10 00    	staa	0x1000
    ea72:	88 80       	eora	#0x80
    ea74:	b7 10 00    	staa	0x1000
    ea77:	88 08       	eora	#0x8
    ea79:	b7 10 00    	staa	0x1000
    ea7c:	37          	pshb
    ea7d:	bd ea d5    	jsr	0xead5
    ea80:	33          	pulb
    ea81:	f7 01 1c    	stab	0x11c
    ea84:	c1 10       	cmpb	#0x10
    ea86:	27 01       	beq	0x0xea89
    ea88:	39          	rts
    ea89:	18 ce 10 23 	ldy	#0x1023
    ea8d:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xea8d
    ea91:	fb 
    ea92:	86 8f       	ldaa	#0x8f
    ea94:	b7 10 47    	staa	0x1047
    ea97:	86 80       	ldaa	#0x80
    ea99:	ba 10 00    	oraa	0x1000
    ea9c:	b7 10 00    	staa	0x1000
    ea9f:	88 80       	eora	#0x80
    eaa1:	b7 10 00    	staa	0x1000
    eaa4:	bd ea d5    	jsr	0xead5
    eaa7:	39          	rts
    eaa8:	f6 10 23    	ldab	0x1023
    eaab:	c5 10       	bitb	#0x10
    eaad:	27 f9       	beq	0x0xeaa8
    eaaf:	a6 00       	ldaa	0x0,x
    eab1:	b7 10 47    	staa	0x1047
    eab4:	86 88       	ldaa	#0x88
    eab6:	ba 10 00    	oraa	0x1000
    eab9:	b7 10 00    	staa	0x1000
    eabc:	88 80       	eora	#0x80
    eabe:	b7 10 00    	staa	0x1000
    eac1:	88 08       	eora	#0x8
    eac3:	b7 10 00    	staa	0x1000
    eac6:	86 10       	ldaa	#0x10
    eac8:	b7 10 23    	staa	0x1023
    eacb:	fc 10 0e    	ldd	0x100e
    eace:	c3 00 f0    	addd	#0xf0
    ead1:	fd 10 1c    	std	0x101c
    ead4:	39          	rts
    ead5:	86 10       	ldaa	#0x10
    ead7:	b7 10 23    	staa	0x1023
    eada:	fc 10 0e    	ldd	0x100e
    eadd:	c3 00 f0    	addd	#0xf0
    eae0:	fd 10 1c    	std	0x101c
    eae3:	39          	rts
    eae4:	f6 10 23    	ldab	0x1023
    eae7:	c5 10       	bitb	#0x10
    eae9:	27 f9       	beq	0x0xeae4
    eaeb:	b6 01 1e    	ldaa	0x11e
    eaee:	81 0f       	cmpa	#0xf
    eaf0:	23 02       	bls	0x0xeaf4
    eaf2:	8b 30       	adda	#0x30
    eaf4:	8a 80       	oraa	#0x80
    eaf6:	b7 10 47    	staa	0x1047
    eaf9:	86 80       	ldaa	#0x80
    eafb:	ba 10 00    	oraa	0x1000
    eafe:	b7 10 00    	staa	0x1000
    eb01:	88 80       	eora	#0x80
    eb03:	b7 10 00    	staa	0x1000
    eb06:	7e ea d5    	jmp	0xead5
    eb09:	ce 10 23    	ldx	#0x1023
    eb0c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xeb0c
    eb10:	86 cf       	ldaa	#0xcf
    eb12:	b7 10 47    	staa	0x1047
    eb15:	86 80       	ldaa	#0x80
    eb17:	ba 10 00    	oraa	0x1000
    eb1a:	b7 10 00    	staa	0x1000
    eb1d:	01          	nop
    eb1e:	88 80       	eora	#0x80
    eb20:	b7 10 00    	staa	0x1000
    eb23:	7e ea d5    	jmp	0xead5
    eb26:	b6 01 1e    	ldaa	0x11e
    eb29:	81 03       	cmpa	#0x3
    eb2b:	22 04       	bhi	0x0xeb31
    eb2d:	86 03       	ldaa	#0x3
    eb2f:	20 22       	bra	0x0xeb53
    eb31:	81 09       	cmpa	#0x9
    eb33:	22 04       	bhi	0x0xeb39
    eb35:	86 09       	ldaa	#0x9
    eb37:	20 1a       	bra	0x0xeb53
    eb39:	81 0e       	cmpa	#0xe
    eb3b:	22 04       	bhi	0x0xeb41
    eb3d:	86 0e       	ldaa	#0xe
    eb3f:	20 12       	bra	0x0xeb53
    eb41:	81 13       	cmpa	#0x13
    eb43:	22 04       	bhi	0x0xeb49
    eb45:	86 13       	ldaa	#0x13
    eb47:	20 0a       	bra	0x0xeb53
    eb49:	81 19       	cmpa	#0x19
    eb4b:	22 04       	bhi	0x0xeb51
    eb4d:	86 19       	ldaa	#0x19
    eb4f:	20 02       	bra	0x0xeb53
    eb51:	86 1e       	ldaa	#0x1e
    eb53:	b7 01 1e    	staa	0x11e
    eb56:	7e ea e4    	jmp	0xeae4
    eb59:	86 02       	ldaa	#0x2
    eb5b:	b1 01 1e    	cmpa	0x11e
    eb5e:	23 06       	bls	0x0xeb66
    eb60:	b7 01 1e    	staa	0x11e
    eb63:	7e ea e4    	jmp	0xeae4
    eb66:	86 06       	ldaa	#0x6
    eb68:	b1 01 1e    	cmpa	0x11e
    eb6b:	23 06       	bls	0x0xeb73
    eb6d:	b7 01 1e    	staa	0x11e
    eb70:	7e ea e4    	jmp	0xeae4
    eb73:	86 0a       	ldaa	#0xa
    eb75:	b1 01 1e    	cmpa	0x11e
    eb78:	23 06       	bls	0x0xeb80
    eb7a:	b7 01 1e    	staa	0x11e
    eb7d:	7e ea e4    	jmp	0xeae4
    eb80:	86 0e       	ldaa	#0xe
    eb82:	b1 01 1e    	cmpa	0x11e
    eb85:	23 06       	bls	0x0xeb8d
    eb87:	b7 01 1e    	staa	0x11e
    eb8a:	7e ea e4    	jmp	0xeae4
    eb8d:	86 12       	ldaa	#0x12
    eb8f:	b1 01 1e    	cmpa	0x11e
    eb92:	23 06       	bls	0x0xeb9a
    eb94:	b7 01 1e    	staa	0x11e
    eb97:	7e ea e4    	jmp	0xeae4
    eb9a:	86 16       	ldaa	#0x16
    eb9c:	b1 01 1e    	cmpa	0x11e
    eb9f:	23 06       	bls	0x0xeba7
    eba1:	b7 01 1e    	staa	0x11e
    eba4:	7e ea e4    	jmp	0xeae4
    eba7:	86 1a       	ldaa	#0x1a
    eba9:	b1 01 1e    	cmpa	0x11e
    ebac:	23 06       	bls	0x0xebb4
    ebae:	b7 01 1e    	staa	0x11e
    ebb1:	7e ea e4    	jmp	0xeae4
    ebb4:	86 1e       	ldaa	#0x1e
    ebb6:	b7 01 1e    	staa	0x11e
    ebb9:	7e ea e4    	jmp	0xeae4
    ebbc:	80 64       	suba	#0x64
    ebbe:	24 09       	bcc	0x0xebc9
    ebc0:	8b 64       	adda	#0x64
    ebc2:	c6 20       	ldab	#0x20
    ebc4:	e7 00       	stab	0x0,x
    ebc6:	08          	inx
    ebc7:	20 05       	bra	0x0xebce
    ebc9:	c6 31       	ldab	#0x31
    ebcb:	e7 00       	stab	0x0,x
    ebcd:	08          	inx
    ebce:	80 0a       	suba	#0xa
    ebd0:	24 14       	bcc	0x0xebe6
    ebd2:	8b 0a       	adda	#0xa
    ebd4:	c1 20       	cmpb	#0x20
    ebd6:	27 07       	beq	0x0xebdf
    ebd8:	c6 30       	ldab	#0x30
    ebda:	e7 00       	stab	0x0,x
    ebdc:	08          	inx
    ebdd:	20 14       	bra	0x0xebf3
    ebdf:	c6 20       	ldab	#0x20
    ebe1:	e7 00       	stab	0x0,x
    ebe3:	08          	inx
    ebe4:	20 0d       	bra	0x0xebf3
    ebe6:	5f          	clrb
    ebe7:	5c          	incb
    ebe8:	80 0a       	suba	#0xa
    ebea:	24 fb       	bcc	0x0xebe7
    ebec:	8b 0a       	adda	#0xa
    ebee:	cb 30       	addb	#0x30
    ebf0:	e7 00       	stab	0x0,x
    ebf2:	08          	inx
    ebf3:	8b 30       	adda	#0x30
    ebf5:	a7 00       	staa	0x0,x
    ebf7:	39          	rts
    ebf8:	80 40       	suba	#0x40
    ebfa:	25 10       	bcs	0x0xec0c
    ebfc:	26 05       	bne	0x0xec03
    ebfe:	8d bc       	bsr	0x0xebbc
    ec00:	09          	dex
    ec01:	09          	dex
    ec02:	39          	rts
    ec03:	8d b7       	bsr	0x0xebbc
    ec05:	09          	dex
    ec06:	09          	dex
    ec07:	86 2b       	ldaa	#0x2b
    ec09:	a7 00       	staa	0x0,x
    ec0b:	39          	rts
    ec0c:	40          	nega
    ec0d:	8d ad       	bsr	0x0xebbc
    ec0f:	09          	dex
    ec10:	09          	dex
    ec11:	86 2d       	ldaa	#0x2d
    ec13:	a7 00       	staa	0x0,x
    ec15:	39          	rts
    ec16:	a6 02       	ldaa	0x2,x
    ec18:	80 30       	suba	#0x30
    ec1a:	e6 01       	ldab	0x1,x
    ec1c:	c1 20       	cmpb	#0x20
    ec1e:	26 01       	bne	0x0xec21
    ec20:	39          	rts
    ec21:	c0 30       	subb	#0x30
    ec23:	36          	psha
    ec24:	86 0a       	ldaa	#0xa
    ec26:	3d          	mul
    ec27:	32          	pula
    ec28:	1b          	aba
    ec29:	e6 00       	ldab	0x0,x
    ec2b:	c1 20       	cmpb	#0x20
    ec2d:	26 01       	bne	0x0xec30
    ec2f:	39          	rts
    ec30:	8b 64       	adda	#0x64
    ec32:	39          	rts
    ec33:	ce 10 23    	ldx	#0x1023
    ec36:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec36
    ec3a:	86 f7       	ldaa	#0xf7
    ec3c:	b4 10 00    	anda	0x1000
    ec3f:	b7 10 00    	staa	0x1000
    ec42:	86 0e       	ldaa	#0xe
    ec44:	b7 10 47    	staa	0x1047
    ec47:	86 80       	ldaa	#0x80
    ec49:	ba 10 00    	oraa	0x1000
    ec4c:	b7 10 00    	staa	0x1000
    ec4f:	01          	nop
    ec50:	88 80       	eora	#0x80
    ec52:	b7 10 00    	staa	0x1000
    ec55:	7c 01 1d    	inc	0x11d
    ec58:	7e ea d5    	jmp	0xead5
    ec5b:	86 03       	ldaa	#0x3
    ec5d:	3d          	mul
    ec5e:	3a          	abx
    ec5f:	c6 03       	ldab	#0x3
    ec61:	a6 00       	ldaa	0x0,x
    ec63:	18 a7 00    	staa	0x0,y
    ec66:	5a          	decb
    ec67:	27 05       	beq	0x0xec6e
    ec69:	08          	inx
    ec6a:	18 08       	iny
    ec6c:	20 f3       	bra	0x0xec61
    ec6e:	39          	rts
    ec6f:	86 04       	ldaa	#0x4
    ec71:	3d          	mul
    ec72:	3a          	abx
    ec73:	c6 04       	ldab	#0x4
    ec75:	a6 00       	ldaa	0x0,x
    ec77:	18 a7 00    	staa	0x0,y
    ec7a:	5a          	decb
    ec7b:	27 05       	beq	0x0xec82
    ec7d:	08          	inx
    ec7e:	18 08       	iny
    ec80:	20 f3       	bra	0x0xec75
    ec82:	39          	rts
    ec83:	c6 c8       	ldab	#0xc8
    ec85:	3d          	mul
    ec86:	bd eb bc    	jsr	0xebbc
    ec89:	86 25       	ldaa	#0x25
    ec8b:	09          	dex
    ec8c:	09          	dex
    ec8d:	a7 00       	staa	0x0,x
    ec8f:	39          	rts
    ec90:	b6 01 1e    	ldaa	0x11e
    ec93:	81 0e       	cmpa	#0xe
    ec95:	22 08       	bhi	0x0xec9f
    ec97:	8b 10       	adda	#0x10
    ec99:	b7 01 1e    	staa	0x11e
    ec9c:	7e ea e4    	jmp	0xeae4
    ec9f:	80 10       	suba	#0x10
    eca1:	b7 01 1e    	staa	0x11e
    eca4:	7e ea e4    	jmp	0xeae4
    eca7:	b6 01 7f    	ldaa	0x17f
    ecaa:	2b 1e       	bmi	0x0xecca
    ecac:	b6 01 1e    	ldaa	0x11e
    ecaf:	81 0e       	cmpa	#0xe
    ecb1:	22 0a       	bhi	0x0xecbd
    ecb3:	27 32       	beq	0x0xece7
    ecb5:	86 0e       	ldaa	#0xe
    ecb7:	b7 01 1e    	staa	0x11e
    ecba:	7e ea e4    	jmp	0xeae4
    ecbd:	81 1e       	cmpa	#0x1e
    ecbf:	26 01       	bne	0x0xecc2
    ecc1:	39          	rts
    ecc2:	86 1e       	ldaa	#0x1e
    ecc4:	b7 01 1e    	staa	0x11e
    ecc7:	7e ea e4    	jmp	0xeae4
    ecca:	b6 01 1e    	ldaa	0x11e
    eccd:	81 19       	cmpa	#0x19
    eccf:	25 0a       	bcs	0x0xecdb
    ecd1:	27 14       	beq	0x0xece7
    ecd3:	86 19       	ldaa	#0x19
    ecd5:	b7 01 1e    	staa	0x11e
    ecd8:	7e ea e4    	jmp	0xeae4
    ecdb:	81 09       	cmpa	#0x9
    ecdd:	27 08       	beq	0x0xece7
    ecdf:	86 09       	ldaa	#0x9
    ece1:	b7 01 1e    	staa	0x11e
    ece4:	7e ea e4    	jmp	0xeae4
    ece7:	39          	rts
    ece8:	b6 01 7f    	ldaa	0x17f
    eceb:	2b 35       	bmi	0x0xed22
    eced:	b6 01 1e    	ldaa	0x11e
    ecf0:	81 13       	cmpa	#0x13
    ecf2:	25 16       	bcs	0x0xed0a
    ecf4:	22 08       	bhi	0x0xecfe
    ecf6:	86 19       	ldaa	#0x19
    ecf8:	b7 01 1e    	staa	0x11e
    ecfb:	7e ea e4    	jmp	0xeae4
    ecfe:	81 19       	cmpa	#0x19
    ed00:	22 57       	bhi	0x0xed59
    ed02:	86 1e       	ldaa	#0x1e
    ed04:	b7 01 1e    	staa	0x11e
    ed07:	7e ea e4    	jmp	0xeae4
    ed0a:	81 03       	cmpa	#0x3
    ed0c:	22 08       	bhi	0x0xed16
    ed0e:	86 09       	ldaa	#0x9
    ed10:	b7 01 1e    	staa	0x11e
    ed13:	7e ea e4    	jmp	0xeae4
    ed16:	81 09       	cmpa	#0x9
    ed18:	22 3f       	bhi	0x0xed59
    ed1a:	86 0e       	ldaa	#0xe
    ed1c:	b7 01 1e    	staa	0x11e
    ed1f:	7e ea e4    	jmp	0xeae4
    ed22:	b6 01 1e    	ldaa	0x11e
    ed25:	81 0f       	cmpa	#0xf
    ed27:	25 18       	bcs	0x0xed41
    ed29:	81 1a       	cmpa	#0x1a
    ed2b:	25 08       	bcs	0x0xed35
    ed2d:	86 19       	ldaa	#0x19
    ed2f:	b7 01 1e    	staa	0x11e
    ed32:	7e ea e4    	jmp	0xeae4
    ed35:	81 19       	cmpa	#0x19
    ed37:	26 20       	bne	0x0xed59
    ed39:	86 13       	ldaa	#0x13
    ed3b:	b7 01 1e    	staa	0x11e
    ed3e:	7e ea e4    	jmp	0xeae4
    ed41:	81 0e       	cmpa	#0xe
    ed43:	25 08       	bcs	0x0xed4d
    ed45:	86 09       	ldaa	#0x9
    ed47:	b7 01 1e    	staa	0x11e
    ed4a:	7e ea e4    	jmp	0xeae4
    ed4d:	81 09       	cmpa	#0x9
    ed4f:	26 08       	bne	0x0xed59
    ed51:	86 03       	ldaa	#0x3
    ed53:	b7 01 1e    	staa	0x11e
    ed56:	7e ea e4    	jmp	0xeae4
    ed59:	39          	rts
    ed5a:	b6 01 7f    	ldaa	0x17f
    ed5d:	2b 3a       	bmi	0x0xed99
    ed5f:	b6 01 1e    	ldaa	0x11e
    ed62:	81 12       	cmpa	#0x12
    ed64:	25 16       	bcs	0x0xed7c
    ed66:	22 04       	bhi	0x0xed6c
    ed68:	86 16       	ldaa	#0x16
    ed6a:	20 26       	bra	0x0xed92
    ed6c:	81 16       	cmpa	#0x16
    ed6e:	22 04       	bhi	0x0xed74
    ed70:	86 1a       	ldaa	#0x1a
    ed72:	20 1e       	bra	0x0xed92
    ed74:	81 1a       	cmpa	#0x1a
    ed76:	22 20       	bhi	0x0xed98
    ed78:	86 1e       	ldaa	#0x1e
    ed7a:	20 16       	bra	0x0xed92
    ed7c:	81 02       	cmpa	#0x2
    ed7e:	22 04       	bhi	0x0xed84
    ed80:	86 06       	ldaa	#0x6
    ed82:	20 0e       	bra	0x0xed92
    ed84:	81 06       	cmpa	#0x6
    ed86:	22 04       	bhi	0x0xed8c
    ed88:	86 0a       	ldaa	#0xa
    ed8a:	20 06       	bra	0x0xed92
    ed8c:	81 0a       	cmpa	#0xa
    ed8e:	22 08       	bhi	0x0xed98
    ed90:	86 0e       	ldaa	#0xe
    ed92:	b7 01 1e    	staa	0x11e
    ed95:	7e ea e4    	jmp	0xeae4
    ed98:	39          	rts
    ed99:	b6 01 1e    	ldaa	0x11e
    ed9c:	81 0e       	cmpa	#0xe
    ed9e:	22 16       	bhi	0x0xedb6
    eda0:	25 04       	bcs	0x0xeda6
    eda2:	86 0a       	ldaa	#0xa
    eda4:	20 ec       	bra	0x0xed92
    eda6:	81 0a       	cmpa	#0xa
    eda8:	25 04       	bcs	0x0xedae
    edaa:	86 06       	ldaa	#0x6
    edac:	20 e4       	bra	0x0xed92
    edae:	81 06       	cmpa	#0x6
    edb0:	25 e6       	bcs	0x0xed98
    edb2:	86 02       	ldaa	#0x2
    edb4:	20 dc       	bra	0x0xed92
    edb6:	81 1e       	cmpa	#0x1e
    edb8:	25 04       	bcs	0x0xedbe
    edba:	86 1a       	ldaa	#0x1a
    edbc:	20 d4       	bra	0x0xed92
    edbe:	81 1a       	cmpa	#0x1a
    edc0:	25 04       	bcs	0x0xedc6
    edc2:	86 16       	ldaa	#0x16
    edc4:	20 cc       	bra	0x0xed92
    edc6:	81 16       	cmpa	#0x16
    edc8:	25 ce       	bcs	0x0xed98
    edca:	86 12       	ldaa	#0x12
    edcc:	20 c4       	bra	0x0xed92
    edce:	ce 10 23    	ldx	#0x1023
    edd1:	1f 00 10 f9 	brclr	0x0,x, #0x10, 0x0xedce
    edd5:	86 04       	ldaa	#0x4
    edd7:	b7 01 1c    	staa	0x11c
    edda:	86 38       	ldaa	#0x38
    eddc:	b7 10 47    	staa	0x1047
    eddf:	86 80       	ldaa	#0x80
    ede1:	ba 10 00    	oraa	0x1000
    ede4:	b7 10 00    	staa	0x1000
    ede7:	01          	nop
    ede8:	88 80       	eora	#0x80
    edea:	b7 10 00    	staa	0x1000
    eded:	96 10       	ldaa	*0x10
    edef:	b7 10 23    	staa	0x1023
    edf2:	fc 10 0e    	ldd	0x100e
    edf5:	c3 20 08    	addd	#0x2008
    edf8:	fd 10 1c    	std	0x101c
    edfb:	ce 10 23    	ldx	#0x1023
    edfe:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xedfe
    ee02:	7a 01 1c    	dec	0x11c
    ee05:	26 d3       	bne	0x0xedda
    ee07:	86 08       	ldaa	#0x8
    ee09:	b7 10 47    	staa	0x1047
    ee0c:	86 80       	ldaa	#0x80
    ee0e:	ba 10 00    	oraa	0x1000
    ee11:	b7 10 00    	staa	0x1000
    ee14:	01          	nop
    ee15:	88 80       	eora	#0x80
    ee17:	b7 10 00    	staa	0x1000
    ee1a:	86 10       	ldaa	#0x10
    ee1c:	b7 10 23    	staa	0x1023
    ee1f:	fc 10 0e    	ldd	0x100e
    ee22:	c3 00 f0    	addd	#0xf0
    ee25:	fd 10 1c    	std	0x101c
    ee28:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xee28
    ee2c:	86 01       	ldaa	#0x1
    ee2e:	b7 10 47    	staa	0x1047
    ee31:	86 80       	ldaa	#0x80
    ee33:	ba 10 00    	oraa	0x1000
    ee36:	b7 10 00    	staa	0x1000
    ee39:	01          	nop
    ee3a:	88 80       	eora	#0x80
    ee3c:	b7 10 00    	staa	0x1000
    ee3f:	86 10       	ldaa	#0x10
    ee41:	b7 10 23    	staa	0x1023
    ee44:	fc 10 0e    	ldd	0x100e
    ee47:	c3 26 48    	addd	#0x2648
    ee4a:	fd 10 1c    	std	0x101c
    ee4d:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xee4d
    ee51:	86 04       	ldaa	#0x4
    ee53:	b7 10 47    	staa	0x1047
    ee56:	86 80       	ldaa	#0x80
    ee58:	ba 10 00    	oraa	0x1000
    ee5b:	b7 10 00    	staa	0x1000
    ee5e:	01          	nop
    ee5f:	88 80       	eora	#0x80
    ee61:	b7 10 00    	staa	0x1000
    ee64:	86 10       	ldaa	#0x10
    ee66:	b7 10 23    	staa	0x1023
    ee69:	fc 10 0e    	ldd	0x100e
    ee6c:	c3 00 f0    	addd	#0xf0
    ee6f:	fd 10 1c    	std	0x101c
    ee72:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xee72
    ee76:	86 0c       	ldaa	#0xc
    ee78:	b7 10 47    	staa	0x1047
    ee7b:	b6 10 00    	ldaa	0x1000
    ee7e:	8a 80       	oraa	#0x80
    ee80:	b7 10 00    	staa	0x1000
    ee83:	88 80       	eora	#0x80
    ee85:	b7 10 00    	staa	0x1000
    ee88:	7f 01 1d    	clr	0x11d
    ee8b:	86 10       	ldaa	#0x10
    ee8d:	b7 10 23    	staa	0x1023
    ee90:	fc 10 0e    	ldd	0x100e
    ee93:	c3 00 f0    	addd	#0xf0
    ee96:	fd 10 1c    	std	0x101c
    ee99:	ce 01 20    	ldx	#0x120
    ee9c:	18 ce ef 6e 	ldy	#0xef6e
    eea0:	c6 20       	ldab	#0x20
    eea2:	18 a6 00    	ldaa	0x0,y
    eea5:	a7 00       	staa	0x0,x
    eea7:	08          	inx
    eea8:	18 08       	iny
    eeaa:	5a          	decb
    eeab:	26 f5       	bne	0x0xeea2
    eead:	7f 01 1e    	clr	0x11e
    eeb0:	86 20       	ldaa	#0x20
    eeb2:	b7 01 1c    	staa	0x11c
    eeb5:	ce 10 23    	ldx	#0x1023
    eeb8:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xeeb8
    eebc:	86 cf       	ldaa	#0xcf
    eebe:	b7 10 47    	staa	0x1047
    eec1:	86 80       	ldaa	#0x80
    eec3:	ba 10 00    	oraa	0x1000
    eec6:	b7 10 00    	staa	0x1000
    eec9:	01          	nop
    eeca:	88 80       	eora	#0x80
    eecc:	b7 10 00    	staa	0x1000
    eecf:	86 10       	ldaa	#0x10
    eed1:	b7 10 23    	staa	0x1023
    eed4:	fc 10 0e    	ldd	0x100e
    eed7:	c3 00 f0    	addd	#0xf0
    eeda:	fd 10 1c    	std	0x101c
    eedd:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xeedd
    eee1:	ce 01 20    	ldx	#0x120
    eee4:	f6 01 1c    	ldab	0x11c
    eee7:	5a          	decb
    eee8:	2a 22       	bpl	0x0xef0c
    eeea:	86 cf       	ldaa	#0xcf
    eeec:	b7 10 47    	staa	0x1047
    eeef:	86 80       	ldaa	#0x80
    eef1:	ba 10 00    	oraa	0x1000
    eef4:	b7 10 00    	staa	0x1000
    eef7:	01          	nop
    eef8:	88 80       	eora	#0x80
    eefa:	b7 10 00    	staa	0x1000
    eefd:	86 10       	ldaa	#0x10
    eeff:	b7 10 23    	staa	0x1023
    ef02:	fc 10 0e    	ldd	0x100e
    ef05:	c3 00 f0    	addd	#0xf0
    ef08:	fd 10 1c    	std	0x101c
    ef0b:	39          	rts
    ef0c:	3a          	abx
    ef0d:	a6 00       	ldaa	0x0,x
    ef0f:	b7 10 47    	staa	0x1047
    ef12:	86 88       	ldaa	#0x88
    ef14:	ba 10 00    	oraa	0x1000
    ef17:	b7 10 00    	staa	0x1000
    ef1a:	88 80       	eora	#0x80
    ef1c:	b7 10 00    	staa	0x1000
    ef1f:	88 08       	eora	#0x8
    ef21:	b7 10 00    	staa	0x1000
    ef24:	86 10       	ldaa	#0x10
    ef26:	b7 10 23    	staa	0x1023
    ef29:	37          	pshb
    ef2a:	fc 10 0e    	ldd	0x100e
    ef2d:	c3 00 f0    	addd	#0xf0
    ef30:	fd 10 1c    	std	0x101c
    ef33:	33          	pulb
    ef34:	f7 01 1c    	stab	0x11c
    ef37:	c1 10       	cmpb	#0x10
    ef39:	27 02       	beq	0x0xef3d
    ef3b:	20 a4       	bra	0x0xeee1
    ef3d:	18 ce 10 23 	ldy	#0x1023
    ef41:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xef41
    ef45:	fb 
    ef46:	86 8f       	ldaa	#0x8f
    ef48:	b7 10 47    	staa	0x1047
    ef4b:	86 80       	ldaa	#0x80
    ef4d:	ba 10 00    	oraa	0x1000
    ef50:	b7 10 00    	staa	0x1000
    ef53:	88 80       	eora	#0x80
    ef55:	b7 10 00    	staa	0x1000
    ef58:	86 10       	ldaa	#0x10
    ef5a:	b7 10 23    	staa	0x1023
    ef5d:	fc 10 0e    	ldd	0x100e
    ef60:	c3 00 f0    	addd	#0xf0
    ef63:	fd 10 1c    	std	0x101c
    ef66:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xef66
    ef6a:	fb 
    ef6b:	7e ee e1    	jmp	0xeee1
    ef6e:	53          	comb
    ef6f:	54          	lsrb
    ef70:	55          	.byte	0x55
    ef71:	44          	lsra
    ef72:	49          	rola
    ef73:	4f          	clra
    ef74:	20 20       	bra	0x0xef96
    ef76:	20 20       	bra	0x0xef98
    ef78:	4f          	clra
    ef79:	4d          	tsta
    ef7a:	45          	.byte	0x45
    ef7b:	47          	asra
    ef7c:	41          	.byte	0x41
    ef7d:	20 45       	bra	0x0xefc4
    ef7f:	4c          	inca
    ef80:	45          	.byte	0x45
    ef81:	43          	coma
    ef82:	54          	lsrb
    ef83:	52          	.byte	0x52
    ef84:	4f          	clra
    ef85:	4e          	.byte	0x4e
    ef86:	49          	rola
    ef87:	43          	coma
    ef88:	53          	comb
    ef89:	20 32       	bra	0x0xefbd
    ef8b:	2e 41       	bgt	0x0xefce
    ef8d:	43          	coma
    ef8e:	00          	bgnd
    ef8f:	01          	nop
    ef90:	01          	nop
    ef91:	01          	nop
    ef92:	01          	nop
    ef93:	02          	idiv
    ef94:	02          	idiv
    ef95:	02          	idiv
    ef96:	02          	idiv
    ef97:	03          	fdiv
    ef98:	03          	fdiv
    ef99:	04          	lsrd
    ef9a:	04          	lsrd
    ef9b:	05          	asld
    ef9c:	05          	asld
    ef9d:	05          	asld
    ef9e:	06          	tap
    ef9f:	06          	tap
    efa0:	06          	tap
    efa1:	07          	tpa
    efa2:	07          	tpa
    efa3:	07          	tpa
    efa4:	08          	inx
    efa5:	08          	inx
    efa6:	08          	inx
    efa7:	09          	dex
    efa8:	09          	dex
    efa9:	0a          	clv
    efaa:	0a          	clv
    efab:	0b          	sev
    efac:	0b          	sev
    efad:	0c          	clc
    efae:	0c          	clc
    efaf:	0d          	sec
    efb0:	0d          	sec
    efb1:	0e          	cli
    efb2:	0e          	cli
    efb3:	0f          	sei
    efb4:	10          	sba
    efb5:	11          	cba
    efb6:	11          	cba
    efb7:	12 12 13 13 	brset	*0x12, #0x13, 0x0xefce
    efbb:	14 14 15    	bset	*0x14, #0x15
    efbe:	15 16 17    	bclr	*0x16, #0x17
    efc1:	18 18       	.byte	0x18, 0x18
    efc3:	19          	daa
    efc4:	19          	daa
    efc5:	1a 1b       	.byte	0x1a, 0x1b
    efc7:	1c 1d 1e    	bset	0x1d,x, #0x1e
    efca:	1f 20 21 22 	brclr	0x20,x, #0x21, 0x0xeff0
    efce:	23 24       	bls	0x0xeff4
    efd0:	25 26       	bcs	0x0xeff8
    efd2:	27 28       	beq	0x0xeffc
    efd4:	29 2a       	bvs	0x0xf000
    efd6:	2b 2c       	bmi	0x0xf004
    efd8:	2d 2e       	blt	0x0xf008
    efda:	30          	tsx
    efdb:	31          	ins
    efdc:	32          	pula
    efdd:	33          	pulb
    efde:	34          	des
    efdf:	35          	txs
    efe0:	36          	psha
    efe1:	37          	pshb
    efe2:	38          	pulx
    efe3:	39          	rts
    efe4:	3a          	abx
    efe5:	3b          	rti
    efe6:	3c          	pshx
    efe7:	3d          	mul
    efe8:	3f          	swi
    efe9:	41          	.byte	0x41
    efea:	42          	.byte	0x42
    efeb:	43          	coma
    efec:	44          	lsra
    efed:	45          	.byte	0x45
    efee:	47          	asra
    efef:	49          	rola
    eff0:	4a          	deca
    eff1:	4c          	inca
    eff2:	4e          	.byte	0x4e
    eff3:	50          	negb
    eff4:	52          	.byte	0x52
    eff5:	54          	lsrb
    eff6:	56          	rorb
    eff7:	58          	aslb
    eff8:	5a          	decb
    eff9:	5c          	incb
    effa:	5e          	.byte	0x5e
    effb:	60 61       	neg	0x61,x
    effd:	63 64       	com	0x64,x
    efff:	66 68       	ror	0x68,x
    f001:	6a 6c       	dec	0x6c,x
    f003:	6e 70       	jmp	0x70,x
    f005:	72          	.byte	0x72
    f006:	74 76 78    	lsr	0x7678
    f009:	7a 7c 7d    	dec	0x7c7d
    f00c:	7e 7f 00    	jmp	0x7f00
    f00f:	00          	bgnd
    f010:	40          	nega
    f011:	00          	bgnd
    f012:	00          	bgnd
    f013:	00          	bgnd
    f014:	00          	bgnd
    f015:	00          	bgnd
    f016:	5d          	tstb
    f017:	00          	bgnd
    f018:	00          	bgnd
    f019:	00          	bgnd
    f01a:	00          	bgnd
    f01b:	03          	fdiv
    f01c:	00          	bgnd
    f01d:	00          	bgnd
    f01e:	00          	bgnd
    f01f:	00          	bgnd
    f020:	00          	bgnd
    f021:	00          	bgnd
    f022:	01          	nop
    f023:	53          	comb
    f024:	00          	bgnd
    f025:	00          	bgnd
    f026:	00          	bgnd
    f027:	00          	bgnd
    f028:	10          	sba
    f029:	00          	bgnd
    f02a:	00          	bgnd
    f02b:	00          	bgnd
    f02c:	00          	bgnd
    f02d:	00          	bgnd
    f02e:	40          	nega
    f02f:	00          	bgnd
    f030:	2c 3d       	bge	0x0xf06f
    f032:	22 7f       	bhi	0x0xf0b3
    f034:	7f 00 00    	clr	0x0
    f037:	00          	bgnd
    f038:	01          	nop
    f039:	00          	bgnd
    f03a:	00          	bgnd
    f03b:	00          	bgnd
    f03c:	00          	bgnd
    f03d:	7f 00 00    	clr	0x0
    f040:	3d          	mul
    f041:	57          	asrb
    f042:	00          	bgnd
    f043:	40          	nega
    f044:	00          	bgnd
    f045:	2d 3a       	blt	0x0xf081
    f047:	00          	bgnd
    f048:	00          	bgnd
    f049:	00          	bgnd
    f04a:	5a          	decb
    f04b:	40          	nega
    f04c:	00          	bgnd
    f04d:	00          	bgnd
    f04e:	02          	idiv
    f04f:	00          	bgnd
    f050:	00          	bgnd
    f051:	00          	bgnd
    f052:	00          	bgnd
    f053:	00          	bgnd
    f054:	00          	bgnd
    f055:	00          	bgnd
    f056:	07          	tpa
    f057:	00          	bgnd
    f058:	21 01       	brn	0x0xf05b
    f05a:	40          	nega
    f05b:	40          	nega
    f05c:	28 4b       	bvc	0x0xf0a9
    f05e:	20 0a       	bra	0x0xf06a
    f060:	00          	bgnd
    f061:	01          	nop
    f062:	02          	idiv
    f063:	00          	bgnd
    f064:	12 00 19 00 	brset	*0x0, #0x19, 0x0xf068
    f068:	10          	sba
    f069:	00          	bgnd
    f06a:	00          	bgnd
    f06b:	00          	bgnd
    f06c:	03          	fdiv
    f06d:	09          	dex
    f06e:	02          	idiv
    f06f:	00          	bgnd
    f070:	11          	cba
	...
    f079:	00          	bgnd
    f07a:	40          	nega
    f07b:	40          	nega
    f07c:	00          	bgnd
    f07d:	00          	bgnd
    f07e:	02          	idiv
    f07f:	02          	idiv
    f080:	02          	idiv
	...
    f095:	00          	bgnd
    f096:	07          	tpa
    f097:	07          	tpa
    f098:	07          	tpa
    f099:	07          	tpa
    f09a:	07          	tpa
    f09b:	07          	tpa
    f09c:	07          	tpa
    f09d:	07          	tpa
    f09e:	40          	nega
    f09f:	40          	nega
    f0a0:	40          	nega
    f0a1:	40          	nega
    f0a2:	40          	nega
    f0a3:	40          	nega
    f0a4:	40          	nega
    f0a5:	40          	nega
	...
    f0ae:	49          	rola
    f0af:	4e          	.byte	0x4e
    f0b0:	49          	rola
    f0b1:	54          	lsrb
    f0b2:	49          	rola
    f0b3:	41          	.byte	0x41
    f0b4:	4c          	inca
    f0b5:	20 20       	bra	0x0xf0d7
    f0b7:	20 20       	bra	0x0xf0d9
    f0b9:	20 20       	bra	0x0xf0db
    f0bb:	20 20       	bra	0x0xf0dd
    f0bd:	20 01       	bra	0x0xf0c0
    f0bf:	00          	bgnd
    f0c0:	3b          	rti
    f0c1:	44          	lsra
    f0c2:	7f 00 01    	clr	0x1
    f0c5:	44          	lsra
    f0c6:	7f 00 01    	clr	0x1
    f0c9:	44          	lsra
    f0ca:	7f 00 01    	clr	0x1
    f0cd:	44          	lsra
    f0ce:	7f 00 01    	clr	0x1
    f0d1:	44          	lsra
    f0d2:	7f 00 01    	clr	0x1
    f0d5:	44          	lsra
    f0d6:	7f 00 01    	clr	0x1
    f0d9:	44          	lsra
    f0da:	7f 00 01    	clr	0x1
    f0dd:	44          	lsra
    f0de:	7f 3c 00    	clr	0x3c00
    f0e1:	00          	bgnd
    f0e2:	00          	bgnd
    f0e3:	00          	bgnd
    f0e4:	00          	bgnd
    f0e5:	00          	bgnd
    f0e6:	07          	tpa
    f0e7:	07          	tpa
    f0e8:	07          	tpa
    f0e9:	07          	tpa
    f0ea:	07          	tpa
    f0eb:	07          	tpa
    f0ec:	07          	tpa
    f0ed:	07          	tpa
    f0ee:	22 22       	bhi	0x0xf112
    f0f0:	22 22       	bhi	0x0xf114
    f0f2:	5e          	.byte	0x5e
    f0f3:	5e          	.byte	0x5e
    f0f4:	5e          	.byte	0x5e
    f0f5:	5e          	.byte	0x5e
	...
    f0fe:	49          	rola
    f0ff:	4e          	.byte	0x4e
    f100:	49          	rola
    f101:	54          	lsrb
    f102:	49          	rola
    f103:	41          	.byte	0x41
    f104:	4c          	inca
    f105:	20 20       	bra	0x0xf127
    f107:	20 20       	bra	0x0xf129
    f109:	20 20       	bra	0x0xf12b
    f10b:	20 20       	bra	0x0xf12d
    f10d:	20 00       	bra	0x0xf10f
    f10f:	02          	idiv
    f110:	03          	fdiv
    f111:	05          	asld
    f112:	0f          	sei
    f113:	0a          	clv
    f114:	1e 14 3c 28 	brset	0x14,x, #0x3c, 0x0xf140
    f118:	78 50 f0    	asl	0x50f0
    f11b:	ff ff ff    	stx	0xffff
    f11e:	ff ff ff    	stx	0xffff
    f121:	ff ff ff    	stx	0xffff
    f124:	ff ff ff    	stx	0xffff
    f127:	ff ff ff    	stx	0xffff
    f12a:	ff ff ff    	stx	0xffff
    f12d:	ff ff ff    	stx	0xffff
    f130:	ff ff ff    	stx	0xffff
    f133:	ff ff ff    	stx	0xffff
    f136:	ff ff ff    	stx	0xffff
    f139:	ff ff ff    	stx	0xffff
    f13c:	ff ff ff    	stx	0xffff
    f13f:	ff ff ff    	stx	0xffff
    f142:	ff ff ff    	stx	0xffff
    f145:	ff ff ff    	stx	0xffff
    f148:	ff ff ff    	stx	0xffff
    f14b:	ff ff ff    	stx	0xffff
    f14e:	ff ff ff    	stx	0xffff
    f151:	ff ff ff    	stx	0xffff
    f154:	ff ff ff    	stx	0xffff
    f157:	ff ff ff    	stx	0xffff
    f15a:	ff ff ff    	stx	0xffff
    f15d:	ff ff ff    	stx	0xffff
    f160:	ff ff ff    	stx	0xffff
    f163:	ff ff ff    	stx	0xffff
    f166:	ff ff ff    	stx	0xffff
    f169:	ff ff ff    	stx	0xffff
    f16c:	ff ff ff    	stx	0xffff
    f16f:	ff ff ff    	stx	0xffff
    f172:	ff ff ff    	stx	0xffff
    f175:	ff ff ff    	stx	0xffff
    f178:	ff ff ff    	stx	0xffff
    f17b:	ff ff ff    	stx	0xffff
    f17e:	ff ff ff    	stx	0xffff
    f181:	ff ff ff    	stx	0xffff
    f184:	ff ff ff    	stx	0xffff
    f187:	ff ff ff    	stx	0xffff
    f18a:	ff ff ff    	stx	0xffff
    f18d:	ff ff ff    	stx	0xffff
    f190:	ff ff ff    	stx	0xffff
    f193:	ff ff ff    	stx	0xffff
    f196:	ff ff ff    	stx	0xffff
    f199:	ff ff ff    	stx	0xffff
    f19c:	ff ff ff    	stx	0xffff
    f19f:	ff ff ff    	stx	0xffff
    f1a2:	ff ff ff    	stx	0xffff
    f1a5:	ff ff ff    	stx	0xffff
    f1a8:	ff ff ff    	stx	0xffff
    f1ab:	ff ff ff    	stx	0xffff
    f1ae:	ff ff ff    	stx	0xffff
    f1b1:	ff ff ff    	stx	0xffff
    f1b4:	ff ff ff    	stx	0xffff
    f1b7:	ff ff ff    	stx	0xffff
    f1ba:	ff ff ff    	stx	0xffff
    f1bd:	ff ff ff    	stx	0xffff
    f1c0:	ff ff ff    	stx	0xffff
    f1c3:	ff ff ff    	stx	0xffff
    f1c6:	ff ff ff    	stx	0xffff
    f1c9:	ff ff ff    	stx	0xffff
    f1cc:	ff ff ff    	stx	0xffff
    f1cf:	ff ff ff    	stx	0xffff
    f1d2:	ff ff ff    	stx	0xffff
    f1d5:	ff ff ff    	stx	0xffff
    f1d8:	ff ff ff    	stx	0xffff
    f1db:	ff ff ff    	stx	0xffff
    f1de:	ff ff ff    	stx	0xffff
    f1e1:	ff ff ff    	stx	0xffff
    f1e4:	ff ff ff    	stx	0xffff
    f1e7:	ff ff ff    	stx	0xffff
    f1ea:	ff ff ff    	stx	0xffff
    f1ed:	ff ff ff    	stx	0xffff
    f1f0:	ff ff ff    	stx	0xffff
    f1f3:	ff ff ff    	stx	0xffff
    f1f6:	ff ff ff    	stx	0xffff
    f1f9:	ff ff ff    	stx	0xffff
    f1fc:	ff ff ff    	stx	0xffff
    f1ff:	ff ff ff    	stx	0xffff
    f202:	ff ff ff    	stx	0xffff
    f205:	ff ff ff    	stx	0xffff
    f208:	ff ff ff    	stx	0xffff
    f20b:	ff ff ff    	stx	0xffff
    f20e:	ff ff ff    	stx	0xffff
    f211:	ff ff ff    	stx	0xffff
    f214:	ff ff ff    	stx	0xffff
    f217:	ff ff ff    	stx	0xffff
    f21a:	ff ff ff    	stx	0xffff
    f21d:	ff ff ff    	stx	0xffff
    f220:	ff ff ff    	stx	0xffff
    f223:	ff ff ff    	stx	0xffff
    f226:	ff ff ff    	stx	0xffff
    f229:	ff ff ff    	stx	0xffff
    f22c:	ff ff ff    	stx	0xffff
    f22f:	ff ff ff    	stx	0xffff
    f232:	ff ff ff    	stx	0xffff
    f235:	ff ff ff    	stx	0xffff
    f238:	ff ff ff    	stx	0xffff
    f23b:	ff ff ff    	stx	0xffff
    f23e:	ff ff ff    	stx	0xffff
    f241:	ff ff ff    	stx	0xffff
    f244:	ff ff ff    	stx	0xffff
    f247:	ff ff ff    	stx	0xffff
    f24a:	ff ff ff    	stx	0xffff
    f24d:	ff ff ff    	stx	0xffff
    f250:	ff ff ff    	stx	0xffff
    f253:	ff ff ff    	stx	0xffff
    f256:	ff ff ff    	stx	0xffff
    f259:	ff ff ff    	stx	0xffff
    f25c:	ff ff ff    	stx	0xffff
    f25f:	ff ff ff    	stx	0xffff
    f262:	ff ff ff    	stx	0xffff
    f265:	ff ff ff    	stx	0xffff
    f268:	ff ff ff    	stx	0xffff
    f26b:	ff ff ff    	stx	0xffff
    f26e:	ff ff ff    	stx	0xffff
    f271:	ff ff ff    	stx	0xffff
    f274:	ff ff ff    	stx	0xffff
    f277:	ff ff ff    	stx	0xffff
    f27a:	ff ff ff    	stx	0xffff
    f27d:	ff ff ff    	stx	0xffff
    f280:	ff ff ff    	stx	0xffff
    f283:	ff ff ff    	stx	0xffff
    f286:	ff ff ff    	stx	0xffff
    f289:	ff ff ff    	stx	0xffff
    f28c:	ff ff ff    	stx	0xffff
    f28f:	ff ff ff    	stx	0xffff
    f292:	ff ff ff    	stx	0xffff
    f295:	ff ff ff    	stx	0xffff
    f298:	ff ff ff    	stx	0xffff
    f29b:	ff ff ff    	stx	0xffff
    f29e:	ff ff ff    	stx	0xffff
    f2a1:	ff ff ff    	stx	0xffff
    f2a4:	ff ff ff    	stx	0xffff
    f2a7:	ff ff ff    	stx	0xffff
    f2aa:	ff ff ff    	stx	0xffff
    f2ad:	ff ff ff    	stx	0xffff
    f2b0:	ff ff ff    	stx	0xffff
    f2b3:	ff ff ff    	stx	0xffff
    f2b6:	ff ff ff    	stx	0xffff
    f2b9:	ff ff ff    	stx	0xffff
    f2bc:	ff ff ff    	stx	0xffff
    f2bf:	ff ff ff    	stx	0xffff
    f2c2:	ff ff ff    	stx	0xffff
    f2c5:	ff ff ff    	stx	0xffff
    f2c8:	ff ff ff    	stx	0xffff
    f2cb:	ff ff ff    	stx	0xffff
    f2ce:	ff ff ff    	stx	0xffff
    f2d1:	ff ff ff    	stx	0xffff
    f2d4:	ff ff ff    	stx	0xffff
    f2d7:	ff ff ff    	stx	0xffff
    f2da:	ff ff ff    	stx	0xffff
    f2dd:	ff ff ff    	stx	0xffff
    f2e0:	ff ff ff    	stx	0xffff
    f2e3:	ff ff ff    	stx	0xffff
    f2e6:	ff ff ff    	stx	0xffff
    f2e9:	ff ff ff    	stx	0xffff
    f2ec:	ff ff ff    	stx	0xffff
    f2ef:	ff ff ff    	stx	0xffff
    f2f2:	ff ff ff    	stx	0xffff
    f2f5:	ff ff ff    	stx	0xffff
    f2f8:	ff ff ff    	stx	0xffff
    f2fb:	ff ff ff    	stx	0xffff
    f2fe:	ff ff ff    	stx	0xffff
    f301:	ff ff ff    	stx	0xffff
    f304:	ff ff ff    	stx	0xffff
    f307:	ff ff ff    	stx	0xffff
    f30a:	ff ff ff    	stx	0xffff
    f30d:	ff ff ff    	stx	0xffff
    f310:	ff ff ff    	stx	0xffff
    f313:	ff ff ff    	stx	0xffff
    f316:	ff ff ff    	stx	0xffff
    f319:	ff ff ff    	stx	0xffff
    f31c:	ff ff ff    	stx	0xffff
    f31f:	ff ff ff    	stx	0xffff
    f322:	ff ff ff    	stx	0xffff
    f325:	ff ff ff    	stx	0xffff
    f328:	ff ff ff    	stx	0xffff
    f32b:	ff ff ff    	stx	0xffff
    f32e:	ff ff ff    	stx	0xffff
    f331:	ff ff ff    	stx	0xffff
    f334:	ff ff ff    	stx	0xffff
    f337:	ff ff ff    	stx	0xffff
    f33a:	ff ff ff    	stx	0xffff
    f33d:	ff ff ff    	stx	0xffff
    f340:	ff ff ff    	stx	0xffff
    f343:	ff ff ff    	stx	0xffff
    f346:	ff ff ff    	stx	0xffff
    f349:	ff ff ff    	stx	0xffff
    f34c:	ff ff ff    	stx	0xffff
    f34f:	ff ff ff    	stx	0xffff
    f352:	ff ff ff    	stx	0xffff
    f355:	ff ff ff    	stx	0xffff
    f358:	ff ff ff    	stx	0xffff
    f35b:	ff ff ff    	stx	0xffff
    f35e:	ff ff ff    	stx	0xffff
    f361:	ff ff ff    	stx	0xffff
    f364:	ff ff ff    	stx	0xffff
    f367:	ff ff ff    	stx	0xffff
    f36a:	ff ff ff    	stx	0xffff
    f36d:	ff ff ff    	stx	0xffff
    f370:	ff ff ff    	stx	0xffff
    f373:	ff ff ff    	stx	0xffff
    f376:	ff ff ff    	stx	0xffff
    f379:	ff ff ff    	stx	0xffff
    f37c:	ff ff ff    	stx	0xffff
    f37f:	ff ff ff    	stx	0xffff
    f382:	ff ff ff    	stx	0xffff
    f385:	ff ff ff    	stx	0xffff
    f388:	ff ff ff    	stx	0xffff
    f38b:	ff ff ff    	stx	0xffff
    f38e:	ff ff ff    	stx	0xffff
    f391:	ff ff ff    	stx	0xffff
    f394:	ff ff ff    	stx	0xffff
    f397:	ff ff ff    	stx	0xffff
    f39a:	ff ff ff    	stx	0xffff
    f39d:	ff ff ff    	stx	0xffff
    f3a0:	ff ff ff    	stx	0xffff
    f3a3:	ff ff ff    	stx	0xffff
    f3a6:	ff ff ff    	stx	0xffff
    f3a9:	ff ff ff    	stx	0xffff
    f3ac:	ff ff ff    	stx	0xffff
    f3af:	ff ff ff    	stx	0xffff
    f3b2:	ff ff ff    	stx	0xffff
    f3b5:	ff ff ff    	stx	0xffff
    f3b8:	ff ff ff    	stx	0xffff
    f3bb:	ff ff ff    	stx	0xffff
    f3be:	ff ff ff    	stx	0xffff
    f3c1:	ff ff ff    	stx	0xffff
    f3c4:	ff ff ff    	stx	0xffff
    f3c7:	ff ff ff    	stx	0xffff
    f3ca:	ff ff ff    	stx	0xffff
    f3cd:	ff ff ff    	stx	0xffff
    f3d0:	ff ff ff    	stx	0xffff
    f3d3:	ff ff ff    	stx	0xffff
    f3d6:	ff ff ff    	stx	0xffff
    f3d9:	ff ff ff    	stx	0xffff
    f3dc:	ff ff ff    	stx	0xffff
    f3df:	ff ff ff    	stx	0xffff
    f3e2:	ff ff ff    	stx	0xffff
    f3e5:	ff ff ff    	stx	0xffff
    f3e8:	ff ff ff    	stx	0xffff
    f3eb:	ff ff ff    	stx	0xffff
    f3ee:	ff ff ff    	stx	0xffff
    f3f1:	ff ff ff    	stx	0xffff
    f3f4:	ff ff ff    	stx	0xffff
    f3f7:	ff ff ff    	stx	0xffff
    f3fa:	ff ff ff    	stx	0xffff
    f3fd:	ff ff ff    	stx	0xffff
    f400:	ff ff ff    	stx	0xffff
    f403:	ff ff ff    	stx	0xffff
    f406:	ff ff ff    	stx	0xffff
    f409:	ff ff ff    	stx	0xffff
    f40c:	ff ff ff    	stx	0xffff
    f40f:	ff ff ff    	stx	0xffff
    f412:	ff ff ff    	stx	0xffff
    f415:	ff ff ff    	stx	0xffff
    f418:	ff ff ff    	stx	0xffff
    f41b:	ff ff ff    	stx	0xffff
    f41e:	ff ff ff    	stx	0xffff
    f421:	ff ff ff    	stx	0xffff
    f424:	ff ff ff    	stx	0xffff
    f427:	ff ff ff    	stx	0xffff
    f42a:	ff ff ff    	stx	0xffff
    f42d:	ff ff ff    	stx	0xffff
    f430:	ff ff ff    	stx	0xffff
    f433:	ff ff ff    	stx	0xffff
    f436:	ff ff ff    	stx	0xffff
    f439:	ff ff ff    	stx	0xffff
    f43c:	ff ff ff    	stx	0xffff
    f43f:	ff ff ff    	stx	0xffff
    f442:	ff ff ff    	stx	0xffff
    f445:	ff ff ff    	stx	0xffff
    f448:	ff ff ff    	stx	0xffff
    f44b:	ff ff ff    	stx	0xffff
    f44e:	ff ff ff    	stx	0xffff
    f451:	ff ff ff    	stx	0xffff
    f454:	ff ff ff    	stx	0xffff
    f457:	ff ff ff    	stx	0xffff
    f45a:	ff ff ff    	stx	0xffff
    f45d:	ff ff ff    	stx	0xffff
    f460:	ff ff ff    	stx	0xffff
    f463:	ff ff ff    	stx	0xffff
    f466:	ff ff ff    	stx	0xffff
    f469:	ff ff ff    	stx	0xffff
    f46c:	ff ff ff    	stx	0xffff
    f46f:	ff ff ff    	stx	0xffff
    f472:	ff ff ff    	stx	0xffff
    f475:	ff ff ff    	stx	0xffff
    f478:	ff ff ff    	stx	0xffff
    f47b:	ff ff ff    	stx	0xffff
    f47e:	ff ff ff    	stx	0xffff
    f481:	ff ff ff    	stx	0xffff
    f484:	ff ff ff    	stx	0xffff
    f487:	ff ff ff    	stx	0xffff
    f48a:	ff ff ff    	stx	0xffff
    f48d:	ff ff ff    	stx	0xffff
    f490:	ff ff ff    	stx	0xffff
    f493:	ff ff ff    	stx	0xffff
    f496:	ff ff ff    	stx	0xffff
    f499:	ff ff ff    	stx	0xffff
    f49c:	ff ff ff    	stx	0xffff
    f49f:	ff ff ff    	stx	0xffff
    f4a2:	ff ff ff    	stx	0xffff
    f4a5:	ff ff ff    	stx	0xffff
    f4a8:	ff ff ff    	stx	0xffff
    f4ab:	ff ff ff    	stx	0xffff
    f4ae:	ff ff ff    	stx	0xffff
    f4b1:	ff ff ff    	stx	0xffff
    f4b4:	ff ff ff    	stx	0xffff
    f4b7:	ff ff ff    	stx	0xffff
    f4ba:	ff ff ff    	stx	0xffff
    f4bd:	ff ff ff    	stx	0xffff
    f4c0:	ff ff ff    	stx	0xffff
    f4c3:	ff ff ff    	stx	0xffff
    f4c6:	ff ff ff    	stx	0xffff
    f4c9:	ff ff ff    	stx	0xffff
    f4cc:	ff ff ff    	stx	0xffff
    f4cf:	ff ff ff    	stx	0xffff
    f4d2:	ff ff ff    	stx	0xffff
    f4d5:	ff ff ff    	stx	0xffff
    f4d8:	ff ff ff    	stx	0xffff
    f4db:	ff ff ff    	stx	0xffff
    f4de:	ff ff ff    	stx	0xffff
    f4e1:	ff ff ff    	stx	0xffff
    f4e4:	ff ff ff    	stx	0xffff
    f4e7:	ff ff ff    	stx	0xffff
    f4ea:	ff ff ff    	stx	0xffff
    f4ed:	ff ff ff    	stx	0xffff
    f4f0:	ff ff ff    	stx	0xffff
    f4f3:	ff ff ff    	stx	0xffff
    f4f6:	ff ff ff    	stx	0xffff
    f4f9:	ff ff ff    	stx	0xffff
    f4fc:	ff ff ff    	stx	0xffff
    f4ff:	ff ff ff    	stx	0xffff
    f502:	ff ff ff    	stx	0xffff
    f505:	ff ff ff    	stx	0xffff
    f508:	ff ff ff    	stx	0xffff
    f50b:	ff ff ff    	stx	0xffff
    f50e:	ff ff ff    	stx	0xffff
    f511:	ff ff ff    	stx	0xffff
    f514:	ff ff ff    	stx	0xffff
    f517:	ff ff ff    	stx	0xffff
    f51a:	ff ff ff    	stx	0xffff
    f51d:	ff ff ff    	stx	0xffff
    f520:	ff ff ff    	stx	0xffff
    f523:	ff ff ff    	stx	0xffff
    f526:	ff ff ff    	stx	0xffff
    f529:	ff ff ff    	stx	0xffff
    f52c:	ff ff ff    	stx	0xffff
    f52f:	ff ff ff    	stx	0xffff
    f532:	ff ff ff    	stx	0xffff
    f535:	ff ff ff    	stx	0xffff
    f538:	ff ff ff    	stx	0xffff
    f53b:	ff ff ff    	stx	0xffff
    f53e:	ff ff ff    	stx	0xffff
    f541:	ff ff ff    	stx	0xffff
    f544:	ff ff ff    	stx	0xffff
    f547:	ff ff ff    	stx	0xffff
    f54a:	ff ff ff    	stx	0xffff
    f54d:	ff ff ff    	stx	0xffff
    f550:	ff ff ff    	stx	0xffff
    f553:	ff ff ff    	stx	0xffff
    f556:	ff ff ff    	stx	0xffff
    f559:	ff ff ff    	stx	0xffff
    f55c:	ff ff ff    	stx	0xffff
    f55f:	ff ff ff    	stx	0xffff
    f562:	ff ff ff    	stx	0xffff
    f565:	ff ff ff    	stx	0xffff
    f568:	ff ff ff    	stx	0xffff
    f56b:	ff ff ff    	stx	0xffff
    f56e:	ff ff ff    	stx	0xffff
    f571:	ff ff ff    	stx	0xffff
    f574:	ff ff ff    	stx	0xffff
    f577:	ff ff ff    	stx	0xffff
    f57a:	ff ff ff    	stx	0xffff
    f57d:	ff ff ff    	stx	0xffff
    f580:	ff ff ff    	stx	0xffff
    f583:	ff ff ff    	stx	0xffff
    f586:	ff ff ff    	stx	0xffff
    f589:	ff ff ff    	stx	0xffff
    f58c:	ff ff ff    	stx	0xffff
    f58f:	ff ff ff    	stx	0xffff
    f592:	ff ff ff    	stx	0xffff
    f595:	ff ff ff    	stx	0xffff
    f598:	ff ff ff    	stx	0xffff
    f59b:	ff ff ff    	stx	0xffff
    f59e:	ff ff ff    	stx	0xffff
    f5a1:	ff ff ff    	stx	0xffff
    f5a4:	ff ff ff    	stx	0xffff
    f5a7:	ff ff ff    	stx	0xffff
    f5aa:	ff ff ff    	stx	0xffff
    f5ad:	ff ff ff    	stx	0xffff
    f5b0:	ff ff ff    	stx	0xffff
    f5b3:	ff ff ff    	stx	0xffff
    f5b6:	ff ff ff    	stx	0xffff
    f5b9:	ff ff ff    	stx	0xffff
    f5bc:	ff ff ff    	stx	0xffff
    f5bf:	ff ff ff    	stx	0xffff
    f5c2:	ff ff ff    	stx	0xffff
    f5c5:	ff ff ff    	stx	0xffff
    f5c8:	ff ff ff    	stx	0xffff
    f5cb:	ff ff ff    	stx	0xffff
    f5ce:	ff ff ff    	stx	0xffff
    f5d1:	ff ff ff    	stx	0xffff
    f5d4:	ff ff ff    	stx	0xffff
    f5d7:	ff ff ff    	stx	0xffff
    f5da:	ff ff ff    	stx	0xffff
    f5dd:	ff ff ff    	stx	0xffff
    f5e0:	ff ff ff    	stx	0xffff
    f5e3:	ff ff ff    	stx	0xffff
    f5e6:	ff ff ff    	stx	0xffff
    f5e9:	ff ff ff    	stx	0xffff
    f5ec:	ff ff ff    	stx	0xffff
    f5ef:	ff ff ff    	stx	0xffff
    f5f2:	ff ff ff    	stx	0xffff
    f5f5:	ff ff ff    	stx	0xffff
    f5f8:	ff ff ff    	stx	0xffff
    f5fb:	ff ff ff    	stx	0xffff
    f5fe:	ff ff ff    	stx	0xffff
    f601:	ff ff ff    	stx	0xffff
    f604:	ff ff ff    	stx	0xffff
    f607:	ff ff ff    	stx	0xffff
    f60a:	ff ff ff    	stx	0xffff
    f60d:	ff ff ff    	stx	0xffff
    f610:	ff ff ff    	stx	0xffff
    f613:	ff ff ff    	stx	0xffff
    f616:	ff ff ff    	stx	0xffff
    f619:	ff ff ff    	stx	0xffff
    f61c:	ff ff ff    	stx	0xffff
    f61f:	ff ff ff    	stx	0xffff
    f622:	ff ff ff    	stx	0xffff
    f625:	ff ff ff    	stx	0xffff
    f628:	ff ff ff    	stx	0xffff
    f62b:	ff ff ff    	stx	0xffff
    f62e:	ff ff ff    	stx	0xffff
    f631:	ff ff ff    	stx	0xffff
    f634:	ff ff ff    	stx	0xffff
    f637:	ff ff ff    	stx	0xffff
    f63a:	ff ff ff    	stx	0xffff
    f63d:	ff ff ff    	stx	0xffff
    f640:	ff ff ff    	stx	0xffff
    f643:	ff ff ff    	stx	0xffff
    f646:	ff ff ff    	stx	0xffff
    f649:	ff ff ff    	stx	0xffff
    f64c:	ff ff ff    	stx	0xffff
    f64f:	ff ff ff    	stx	0xffff
    f652:	ff ff ff    	stx	0xffff
    f655:	ff ff ff    	stx	0xffff
    f658:	ff ff ff    	stx	0xffff
    f65b:	ff ff ff    	stx	0xffff
    f65e:	ff ff ff    	stx	0xffff
    f661:	ff ff ff    	stx	0xffff
    f664:	ff ff ff    	stx	0xffff
    f667:	ff ff ff    	stx	0xffff
    f66a:	ff ff ff    	stx	0xffff
    f66d:	ff ff ff    	stx	0xffff
    f670:	ff ff ff    	stx	0xffff
    f673:	ff ff ff    	stx	0xffff
    f676:	ff ff ff    	stx	0xffff
    f679:	ff ff ff    	stx	0xffff
    f67c:	ff ff ff    	stx	0xffff
    f67f:	ff ff ff    	stx	0xffff
    f682:	ff ff ff    	stx	0xffff
    f685:	ff ff ff    	stx	0xffff
    f688:	ff ff ff    	stx	0xffff
    f68b:	ff ff ff    	stx	0xffff
    f68e:	ff ff ff    	stx	0xffff
    f691:	ff ff ff    	stx	0xffff
    f694:	ff ff ff    	stx	0xffff
    f697:	ff ff ff    	stx	0xffff
    f69a:	ff ff ff    	stx	0xffff
    f69d:	ff ff ff    	stx	0xffff
    f6a0:	ff ff ff    	stx	0xffff
    f6a3:	ff ff ff    	stx	0xffff
    f6a6:	ff ff ff    	stx	0xffff
    f6a9:	ff ff ff    	stx	0xffff
    f6ac:	ff ff ff    	stx	0xffff
    f6af:	ff ff ff    	stx	0xffff
    f6b2:	ff ff ff    	stx	0xffff
    f6b5:	ff ff ff    	stx	0xffff
    f6b8:	ff ff ff    	stx	0xffff
    f6bb:	ff ff ff    	stx	0xffff
    f6be:	ff ff ff    	stx	0xffff
    f6c1:	ff ff ff    	stx	0xffff
    f6c4:	ff ff ff    	stx	0xffff
    f6c7:	ff ff ff    	stx	0xffff
    f6ca:	ff ff ff    	stx	0xffff
    f6cd:	ff ff ff    	stx	0xffff
    f6d0:	ff ff ff    	stx	0xffff
    f6d3:	ff ff ff    	stx	0xffff
    f6d6:	ff ff ff    	stx	0xffff
    f6d9:	ff ff ff    	stx	0xffff
    f6dc:	ff ff ff    	stx	0xffff
    f6df:	ff ff ff    	stx	0xffff
    f6e2:	ff ff ff    	stx	0xffff
    f6e5:	ff ff ff    	stx	0xffff
    f6e8:	ff ff ff    	stx	0xffff
    f6eb:	ff ff ff    	stx	0xffff
    f6ee:	ff ff ff    	stx	0xffff
    f6f1:	ff ff ff    	stx	0xffff
    f6f4:	ff ff ff    	stx	0xffff
    f6f7:	ff ff ff    	stx	0xffff
    f6fa:	ff ff ff    	stx	0xffff
    f6fd:	ff ff ff    	stx	0xffff
    f700:	ff ff ff    	stx	0xffff
    f703:	ff ff ff    	stx	0xffff
    f706:	ff ff ff    	stx	0xffff
    f709:	ff ff ff    	stx	0xffff
    f70c:	ff ff ff    	stx	0xffff
    f70f:	ff ff ff    	stx	0xffff
    f712:	ff ff ff    	stx	0xffff
    f715:	ff ff ff    	stx	0xffff
    f718:	ff ff ff    	stx	0xffff
    f71b:	ff ff ff    	stx	0xffff
    f71e:	ff ff ff    	stx	0xffff
    f721:	ff ff ff    	stx	0xffff
    f724:	ff ff ff    	stx	0xffff
    f727:	ff ff ff    	stx	0xffff
    f72a:	ff ff ff    	stx	0xffff
    f72d:	ff ff ff    	stx	0xffff
    f730:	ff ff ff    	stx	0xffff
    f733:	ff ff ff    	stx	0xffff
    f736:	ff ff ff    	stx	0xffff
    f739:	ff ff ff    	stx	0xffff
    f73c:	ff ff ff    	stx	0xffff
    f73f:	ff ff ff    	stx	0xffff
    f742:	ff ff ff    	stx	0xffff
    f745:	ff ff ff    	stx	0xffff
    f748:	ff ff ff    	stx	0xffff
    f74b:	ff ff ff    	stx	0xffff
    f74e:	ff ff ff    	stx	0xffff
    f751:	ff ff ff    	stx	0xffff
    f754:	ff ff ff    	stx	0xffff
    f757:	ff ff ff    	stx	0xffff
    f75a:	ff ff ff    	stx	0xffff
    f75d:	ff ff ff    	stx	0xffff
    f760:	ff ff ff    	stx	0xffff
    f763:	ff ff ff    	stx	0xffff
    f766:	ff ff ff    	stx	0xffff
    f769:	ff ff ff    	stx	0xffff
    f76c:	ff ff ff    	stx	0xffff
    f76f:	ff ff ff    	stx	0xffff
    f772:	ff ff ff    	stx	0xffff
    f775:	ff ff ff    	stx	0xffff
    f778:	ff ff ff    	stx	0xffff
    f77b:	ff ff ff    	stx	0xffff
    f77e:	ff ff ff    	stx	0xffff
    f781:	ff ff ff    	stx	0xffff
    f784:	ff ff ff    	stx	0xffff
    f787:	ff ff ff    	stx	0xffff
    f78a:	ff ff ff    	stx	0xffff
    f78d:	ff ff ff    	stx	0xffff
    f790:	ff ff ff    	stx	0xffff
    f793:	ff ff ff    	stx	0xffff
    f796:	ff ff ff    	stx	0xffff
    f799:	ff ff ff    	stx	0xffff
    f79c:	ff ff ff    	stx	0xffff
    f79f:	ff ff ff    	stx	0xffff
    f7a2:	ff ff ff    	stx	0xffff
    f7a5:	ff ff ff    	stx	0xffff
    f7a8:	ff ff ff    	stx	0xffff
    f7ab:	ff ff ff    	stx	0xffff
    f7ae:	ff ff ff    	stx	0xffff
    f7b1:	ff ff ff    	stx	0xffff
    f7b4:	ff ff ff    	stx	0xffff
    f7b7:	ff ff ff    	stx	0xffff
    f7ba:	ff ff ff    	stx	0xffff
    f7bd:	ff ff ff    	stx	0xffff
    f7c0:	ff ff ff    	stx	0xffff
    f7c3:	ff ff ff    	stx	0xffff
    f7c6:	ff ff ff    	stx	0xffff
    f7c9:	ff ff ff    	stx	0xffff
    f7cc:	ff ff ff    	stx	0xffff
    f7cf:	ff ff ff    	stx	0xffff
    f7d2:	ff ff ff    	stx	0xffff
    f7d5:	ff ff ff    	stx	0xffff
    f7d8:	ff ff ff    	stx	0xffff
    f7db:	ff ff ff    	stx	0xffff
    f7de:	ff ff ff    	stx	0xffff
    f7e1:	ff ff ff    	stx	0xffff
    f7e4:	ff ff ff    	stx	0xffff
    f7e7:	ff ff ff    	stx	0xffff
    f7ea:	ff ff ff    	stx	0xffff
    f7ed:	ff ff ff    	stx	0xffff
    f7f0:	ff ff ff    	stx	0xffff
    f7f3:	ff ff ff    	stx	0xffff
    f7f6:	ff ff ff    	stx	0xffff
    f7f9:	ff ff ff    	stx	0xffff
    f7fc:	ff ff ff    	stx	0xffff
    f7ff:	ff ff ff    	stx	0xffff
    f802:	ff ff ff    	stx	0xffff
    f805:	ff ff ff    	stx	0xffff
    f808:	ff ff ff    	stx	0xffff
    f80b:	ff ff ff    	stx	0xffff
    f80e:	ff ff ff    	stx	0xffff
    f811:	ff ff ff    	stx	0xffff
    f814:	ff ff ff    	stx	0xffff
    f817:	ff ff ff    	stx	0xffff
    f81a:	ff ff ff    	stx	0xffff
    f81d:	ff ff ff    	stx	0xffff
    f820:	ff ff ff    	stx	0xffff
    f823:	ff ff ff    	stx	0xffff
    f826:	ff ff ff    	stx	0xffff
    f829:	ff ff ff    	stx	0xffff
    f82c:	ff ff ff    	stx	0xffff
    f82f:	ff ff ff    	stx	0xffff
    f832:	ff ff ff    	stx	0xffff
    f835:	ff ff ff    	stx	0xffff
    f838:	ff ff ff    	stx	0xffff
    f83b:	ff ff ff    	stx	0xffff
    f83e:	ff ff ff    	stx	0xffff
    f841:	ff ff ff    	stx	0xffff
    f844:	ff ff ff    	stx	0xffff
    f847:	ff ff ff    	stx	0xffff
    f84a:	ff ff ff    	stx	0xffff
    f84d:	ff ff ff    	stx	0xffff
    f850:	ff ff ff    	stx	0xffff
    f853:	ff ff ff    	stx	0xffff
    f856:	ff ff ff    	stx	0xffff
    f859:	ff ff ff    	stx	0xffff
    f85c:	ff ff ff    	stx	0xffff
    f85f:	ff ff ff    	stx	0xffff
    f862:	ff ff ff    	stx	0xffff
    f865:	ff ff ff    	stx	0xffff
    f868:	ff ff ff    	stx	0xffff
    f86b:	ff ff ff    	stx	0xffff
    f86e:	ff ff ff    	stx	0xffff
    f871:	ff ff ff    	stx	0xffff
    f874:	ff ff ff    	stx	0xffff
    f877:	ff ff ff    	stx	0xffff
    f87a:	ff ff ff    	stx	0xffff
    f87d:	ff ff ff    	stx	0xffff
    f880:	ff ff ff    	stx	0xffff
    f883:	ff ff ff    	stx	0xffff
    f886:	ff ff ff    	stx	0xffff
    f889:	ff ff ff    	stx	0xffff
    f88c:	ff ff ff    	stx	0xffff
    f88f:	ff ff ff    	stx	0xffff
    f892:	ff ff ff    	stx	0xffff
    f895:	ff ff ff    	stx	0xffff
    f898:	ff ff ff    	stx	0xffff
    f89b:	ff ff ff    	stx	0xffff
    f89e:	ff ff ff    	stx	0xffff
    f8a1:	ff ff ff    	stx	0xffff
    f8a4:	ff ff ff    	stx	0xffff
    f8a7:	ff ff ff    	stx	0xffff
    f8aa:	ff ff ff    	stx	0xffff
    f8ad:	ff ff ff    	stx	0xffff
    f8b0:	ff ff ff    	stx	0xffff
    f8b3:	ff ff ff    	stx	0xffff
    f8b6:	ff ff ff    	stx	0xffff
    f8b9:	ff ff ff    	stx	0xffff
    f8bc:	ff ff ff    	stx	0xffff
    f8bf:	ff ff ff    	stx	0xffff
    f8c2:	ff ff ff    	stx	0xffff
    f8c5:	ff ff ff    	stx	0xffff
    f8c8:	ff ff ff    	stx	0xffff
    f8cb:	ff ff ff    	stx	0xffff
    f8ce:	ff ff ff    	stx	0xffff
    f8d1:	ff ff ff    	stx	0xffff
    f8d4:	ff ff ff    	stx	0xffff
    f8d7:	ff ff ff    	stx	0xffff
    f8da:	ff ff ff    	stx	0xffff
    f8dd:	ff ff ff    	stx	0xffff
    f8e0:	ff ff ff    	stx	0xffff
    f8e3:	ff ff ff    	stx	0xffff
    f8e6:	ff ff ff    	stx	0xffff
    f8e9:	ff ff ff    	stx	0xffff
    f8ec:	ff ff ff    	stx	0xffff
    f8ef:	ff ff ff    	stx	0xffff
    f8f2:	ff ff ff    	stx	0xffff
    f8f5:	ff ff ff    	stx	0xffff
    f8f8:	ff ff ff    	stx	0xffff
    f8fb:	ff ff ff    	stx	0xffff
    f8fe:	ff ff ff    	stx	0xffff
    f901:	ff ff ff    	stx	0xffff
    f904:	ff ff ff    	stx	0xffff
    f907:	ff ff ff    	stx	0xffff
    f90a:	ff ff ff    	stx	0xffff
    f90d:	ff ff ff    	stx	0xffff
    f910:	ff ff ff    	stx	0xffff
    f913:	ff ff ff    	stx	0xffff
    f916:	ff ff ff    	stx	0xffff
    f919:	ff ff ff    	stx	0xffff
    f91c:	ff ff ff    	stx	0xffff
    f91f:	ff ff ff    	stx	0xffff
    f922:	ff ff ff    	stx	0xffff
    f925:	ff ff ff    	stx	0xffff
    f928:	ff ff ff    	stx	0xffff
    f92b:	ff ff ff    	stx	0xffff
    f92e:	ff ff ff    	stx	0xffff
    f931:	ff ff ff    	stx	0xffff
    f934:	ff ff ff    	stx	0xffff
    f937:	ff ff ff    	stx	0xffff
    f93a:	ff ff ff    	stx	0xffff
    f93d:	ff ff ff    	stx	0xffff
    f940:	ff ff ff    	stx	0xffff
    f943:	ff ff ff    	stx	0xffff
    f946:	ff ff ff    	stx	0xffff
    f949:	ff ff ff    	stx	0xffff
    f94c:	ff ff ff    	stx	0xffff
    f94f:	ff ff ff    	stx	0xffff
    f952:	ff ff ff    	stx	0xffff
    f955:	ff ff ff    	stx	0xffff
    f958:	ff ff ff    	stx	0xffff
    f95b:	ff ff ff    	stx	0xffff
    f95e:	ff ff ff    	stx	0xffff
    f961:	ff ff ff    	stx	0xffff
    f964:	ff ff ff    	stx	0xffff
    f967:	ff ff ff    	stx	0xffff
    f96a:	ff ff ff    	stx	0xffff
    f96d:	ff ff ff    	stx	0xffff
    f970:	ff ff ff    	stx	0xffff
    f973:	ff ff ff    	stx	0xffff
    f976:	ff ff ff    	stx	0xffff
    f979:	ff ff ff    	stx	0xffff
    f97c:	ff ff ff    	stx	0xffff
    f97f:	ff ff ff    	stx	0xffff
    f982:	ff ff ff    	stx	0xffff
    f985:	ff ff ff    	stx	0xffff
    f988:	ff ff ff    	stx	0xffff
    f98b:	ff ff ff    	stx	0xffff
    f98e:	ff ff ff    	stx	0xffff
    f991:	ff ff ff    	stx	0xffff
    f994:	ff ff ff    	stx	0xffff
    f997:	ff ff ff    	stx	0xffff
    f99a:	ff ff ff    	stx	0xffff
    f99d:	ff ff ff    	stx	0xffff
    f9a0:	ff ff ff    	stx	0xffff
    f9a3:	ff ff ff    	stx	0xffff
    f9a6:	ff ff ff    	stx	0xffff
    f9a9:	ff ff ff    	stx	0xffff
    f9ac:	ff ff ff    	stx	0xffff
    f9af:	ff ff ff    	stx	0xffff
    f9b2:	ff ff ff    	stx	0xffff
    f9b5:	ff ff ff    	stx	0xffff
    f9b8:	ff ff ff    	stx	0xffff
    f9bb:	ff ff ff    	stx	0xffff
    f9be:	ff ff ff    	stx	0xffff
    f9c1:	ff ff ff    	stx	0xffff
    f9c4:	ff ff ff    	stx	0xffff
    f9c7:	ff ff ff    	stx	0xffff
    f9ca:	ff ff ff    	stx	0xffff
    f9cd:	ff ff ff    	stx	0xffff
    f9d0:	ff ff ff    	stx	0xffff
    f9d3:	ff ff ff    	stx	0xffff
    f9d6:	ff ff ff    	stx	0xffff
    f9d9:	ff ff ff    	stx	0xffff
    f9dc:	ff ff ff    	stx	0xffff
    f9df:	ff ff f6    	stx	0xfff6
    f9e2:	10          	sba
    f9e3:	2e 37       	bgt	0x0xfa1c
    f9e5:	c5 20       	bitb	#0x20
    f9e7:	26 03       	bne	0x0xf9ec
    f9e9:	7e fe d0    	jmp	0xfed0
    f9ec:	c5 02       	bitb	#0x2
    f9ee:	26 11       	bne	0x0xfa01
    f9f0:	c5 08       	bitb	#0x8
    f9f2:	26 0d       	bne	0x0xfa01
    f9f4:	b6 10 2f    	ldaa	0x102f
    f9f7:	2b 0e       	bmi	0x0xfa07
    f9f9:	7d 01 73    	tst	0x173
    f9fc:	26 31       	bne	0x0xfa2f
    f9fe:	7e fe d0    	jmp	0xfed0
    fa01:	b6 10 2f    	ldaa	0x102f
    fa04:	7e fe d0    	jmp	0xfed0
    fa07:	81 ef       	cmpa	#0xef
    fa09:	22 3b       	bhi	0x0xfa46
    fa0b:	16          	tab
    fa0c:	36          	psha
    fa0d:	c4 0f       	andb	#0xf
    fa0f:	f0 01 6f    	subb	0x16f
    fa12:	27 15       	beq	0x0xfa29
    fa14:	25 29       	bcs	0x0xfa3f
    fa16:	96 d1       	ldaa	*0xd1
    fa18:	81 06       	cmpa	#0x6
    fa1a:	26 23       	bne	0x0xfa3f
    fa1c:	c1 07       	cmpb	#0x7
    fa1e:	22 1f       	bhi	0x0xfa3f
    fa20:	32          	pula
    fa21:	b0 01 6f    	suba	0x16f
    fa24:	b7 01 73    	staa	0x173
    fa27:	20 06       	bra	0x0xfa2f
    fa29:	32          	pula
    fa2a:	84 f0       	anda	#0xf0
    fa2c:	b7 01 73    	staa	0x173
    fa2f:	fe 01 c2    	ldx	0x1c2
    fa32:	a7 00       	staa	0x0,x
    fa34:	8f          	xgdx
    fa35:	5c          	incb
    fa36:	c4 bf       	andb	#0xbf
    fa38:	8f          	xgdx
    fa39:	ff 01 c2    	stx	0x1c2
    fa3c:	7e fe d0    	jmp	0xfed0
    fa3f:	32          	pula
    fa40:	7f 01 73    	clr	0x173
    fa43:	7e fe d0    	jmp	0xfed0
    fa46:	81 f8       	cmpa	#0xf8
    fa48:	7e fe d0    	jmp	0xfed0
    fa4b:	26 18       	bne	0x0xfa65
    fa4d:	f6 01 7d    	ldab	0x17d
    fa50:	c1 f0       	cmpb	#0xf0
    fa52:	26 03       	bne	0x0xfa57
    fa54:	7e fe d0    	jmp	0xfed0
    fa57:	7d 00 dc    	tst	0xdc
    fa5a:	27 07       	beq	0x0xfa63
    fa5c:	d6 dc       	ldab	*0xdc
    fa5e:	3a          	abx
    fa5f:	a6 00       	ldaa	0x0,x
    fa61:	86 f8       	ldaa	#0xf8
    fa63:	20 ca       	bra	0x0xfa2f
    fa65:	81 ff       	cmpa	#0xff
    fa67:	20 00       	bra	0x0xfa69
    fa69:	81 fa       	cmpa	#0xfa
    fa6b:	20 04       	bra	0x0xfa71
    fa6d:	26 02       	bne	0x0xfa71
    fa6f:	20 be       	bra	0x0xfa2f
    fa71:	81 f7       	cmpa	#0xf7
    fa73:	26 0c       	bne	0x0xfa81
    fa75:	c6 f0       	ldab	#0xf0
    fa77:	f1 01 73    	cmpb	0x173
    fa7a:	26 15       	bne	0x0xfa91
    fa7c:	7f 01 73    	clr	0x173
    fa7f:	20 ae       	bra	0x0xfa2f
    fa81:	81 f0       	cmpa	#0xf0
    fa83:	26 05       	bne	0x0xfa8a
    fa85:	b7 01 73    	staa	0x173
    fa88:	20 a5       	bra	0x0xfa2f
    fa8a:	81 f4       	cmpa	#0xf4
    fa8c:	24 03       	bcc	0x0xfa91
    fa8e:	7f 01 73    	clr	0x173
    fa91:	33          	pulb
    fa92:	c5 80       	bitb	#0x80
    fa94:	27 02       	beq	0x0xfa98
    fa96:	8d 01       	bsr	0x0xfa99
    fa98:	3b          	rti
    fa99:	fe 01 60    	ldx	0x160
    fa9c:	bc 01 62    	cpx	0x162
    fa9f:	27 16       	beq	0x0xfab7
    faa1:	a6 00       	ldaa	0x0,x
    faa3:	b7 10 2f    	staa	0x102f
    faa6:	8f          	xgdx
    faa7:	5c          	incb
    faa8:	c1 60       	cmpb	#0x60
    faaa:	25 02       	bcs	0x0xfaae
    faac:	c6 40       	ldab	#0x40
    faae:	fd 01 60    	std	0x160
    fab1:	86 ac       	ldaa	#0xac
    fab3:	b7 10 2d    	staa	0x102d
    fab6:	39          	rts
    fab7:	86 2c       	ldaa	#0x2c
    fab9:	b7 10 2d    	staa	0x102d
    fabc:	7c 00 d7    	inc	0xd7
    fabf:	39          	rts
    fac0:	ff ff ff    	stx	0xffff
    fac3:	ff ff ff    	stx	0xffff
    fac6:	ff ff ff    	stx	0xffff
    fac9:	ff ff ff    	stx	0xffff
    facc:	ff ff ff    	stx	0xffff
    facf:	ff ff ff    	stx	0xffff
    fad2:	ff ff ff    	stx	0xffff
    fad5:	ff ff ff    	stx	0xffff
    fad8:	ff ff ff    	stx	0xffff
    fadb:	ff ff ff    	stx	0xffff
    fade:	ff ff ff    	stx	0xffff
    fae1:	ff ff ff    	stx	0xffff
    fae4:	ff ff ff    	stx	0xffff
    fae7:	ff ff ff    	stx	0xffff
    faea:	ff ff ff    	stx	0xffff
    faed:	ff ff ff    	stx	0xffff
    faf0:	ff ff ff    	stx	0xffff
    faf3:	ff ff ff    	stx	0xffff
    faf6:	ff ff ff    	stx	0xffff
    faf9:	ff ff ff    	stx	0xffff
    fafc:	ff ff ff    	stx	0xffff
    faff:	ff ff ff    	stx	0xffff
    fb02:	ff ff ff    	stx	0xffff
    fb05:	ff ff ff    	stx	0xffff
    fb08:	ff ff ff    	stx	0xffff
    fb0b:	ff ff ff    	stx	0xffff
    fb0e:	ff ff ff    	stx	0xffff
    fb11:	ff ff ff    	stx	0xffff
    fb14:	ff ff ff    	stx	0xffff
    fb17:	ff ff ff    	stx	0xffff
    fb1a:	ff ff ff    	stx	0xffff
    fb1d:	ff ff ff    	stx	0xffff
    fb20:	ff ff ff    	stx	0xffff
    fb23:	ff ff ff    	stx	0xffff
    fb26:	ff ff ff    	stx	0xffff
    fb29:	ff ff ff    	stx	0xffff
    fb2c:	ff ff ff    	stx	0xffff
    fb2f:	ff ff ff    	stx	0xffff
    fb32:	ff ff ff    	stx	0xffff
    fb35:	ff ff ff    	stx	0xffff
    fb38:	ff ff ff    	stx	0xffff
    fb3b:	ff ff ff    	stx	0xffff
    fb3e:	ff ff ff    	stx	0xffff
    fb41:	ff ff ff    	stx	0xffff
    fb44:	ff ff ff    	stx	0xffff
    fb47:	ff ff ff    	stx	0xffff
    fb4a:	ff ff ff    	stx	0xffff
    fb4d:	ff ff ff    	stx	0xffff
    fb50:	ff ff ff    	stx	0xffff
    fb53:	ff ff ff    	stx	0xffff
    fb56:	ff ff ff    	stx	0xffff
    fb59:	ff ff ff    	stx	0xffff
    fb5c:	ff ff ff    	stx	0xffff
    fb5f:	ff ff ff    	stx	0xffff
    fb62:	ff ff ff    	stx	0xffff
    fb65:	ff ff ff    	stx	0xffff
    fb68:	ff ff ff    	stx	0xffff
    fb6b:	ff ff ff    	stx	0xffff
    fb6e:	ff ff ff    	stx	0xffff
    fb71:	ff ff ff    	stx	0xffff
    fb74:	ff ff ff    	stx	0xffff
    fb77:	ff ff ff    	stx	0xffff
    fb7a:	ff ff ff    	stx	0xffff
    fb7d:	ff ff ff    	stx	0xffff
    fb80:	ff ff ff    	stx	0xffff
    fb83:	ff ff ff    	stx	0xffff
    fb86:	ff ff ff    	stx	0xffff
    fb89:	ff ff ff    	stx	0xffff
    fb8c:	ff ff ff    	stx	0xffff
    fb8f:	ff ff ff    	stx	0xffff
    fb92:	ff ff ff    	stx	0xffff
    fb95:	ff ff fe    	stx	0xfffe
    fb98:	20 ff       	bra	0x0xfb99
    fb9a:	ff ff ff    	stx	0xffff
    fb9d:	ff ff ff    	stx	0xffff
    fba0:	ff fd f0    	stx	0xfdf0
    fba3:	ff ff ff    	stx	0xffff
    fba6:	ff ff ff    	stx	0xffff
    fba9:	fe 00 ff    	ldx	0xff
    fbac:	ff ff ff    	stx	0xffff
    fbaf:	ff ff ff    	stx	0xffff
    fbb2:	ff ff ff    	stx	0xffff
    fbb5:	ff ff fe    	stx	0xfffe
    fbb8:	00          	bgnd
    fbb9:	80 00       	suba	#0x0
    fbbb:	80 00       	suba	#0x0
    fbbd:	80 00       	suba	#0x0
    fbbf:	80 00       	suba	#0x0
    fbc1:	ff ff ff    	stx	0xffff
    fbc4:	ff ff ff    	stx	0xffff
    fbc7:	ff ff ff    	stx	0xffff
    fbca:	ff ff ff    	stx	0xffff
    fbcd:	ff ff ff    	stx	0xffff
    fbd0:	ff ff ff    	stx	0xffff
    fbd3:	ff ff ff    	stx	0xffff
    fbd6:	ff ff ff    	stx	0xffff
    fbd9:	ff ff ff    	stx	0xffff
    fbdc:	ff ff ff    	stx	0xffff
    fbdf:	ff ff ff    	stx	0xffff
    fbe2:	ff ff ff    	stx	0xffff
    fbe5:	ff ff ff    	stx	0xffff
    fbe8:	ff ff ff    	stx	0xffff
    fbeb:	ff ff ff    	stx	0xffff
    fbee:	ff ff ff    	stx	0xffff
    fbf1:	ff ff ff    	stx	0xffff
    fbf4:	ff ff ff    	stx	0xffff
    fbf7:	ff ff ff    	stx	0xffff
    fbfa:	ff ff ff    	stx	0xffff
    fbfd:	ff ff ff    	stx	0xffff
    fc00:	ff ff ff    	stx	0xffff
    fc03:	ff ff ff    	stx	0xffff
    fc06:	ff ff ff    	stx	0xffff
    fc09:	ff ff ff    	stx	0xffff
    fc0c:	ff ff ff    	stx	0xffff
    fc0f:	ff ff ff    	stx	0xffff
    fc12:	ff ff ff    	stx	0xffff
    fc15:	ff ff ff    	stx	0xffff
    fc18:	ff ff ff    	stx	0xffff
    fc1b:	ff ff ff    	stx	0xffff
    fc1e:	ff ff ff    	stx	0xffff
    fc21:	ff ff ff    	stx	0xffff
    fc24:	ff ff ff    	stx	0xffff
    fc27:	ff ff ff    	stx	0xffff
    fc2a:	ff ff ff    	stx	0xffff
    fc2d:	ff ff ff    	stx	0xffff
    fc30:	ff ff ff    	stx	0xffff
    fc33:	ff ff ff    	stx	0xffff
    fc36:	ff ff ff    	stx	0xffff
    fc39:	ff ff ff    	stx	0xffff
    fc3c:	ff ff ff    	stx	0xffff
    fc3f:	ff ff ff    	stx	0xffff
    fc42:	ff ff ff    	stx	0xffff
    fc45:	ff ff ff    	stx	0xffff
    fc48:	ff ff ff    	stx	0xffff
    fc4b:	ff ff ff    	stx	0xffff
    fc4e:	ff ff ff    	stx	0xffff
    fc51:	ff ff ff    	stx	0xffff
    fc54:	ff ff ff    	stx	0xffff
    fc57:	ff ff ff    	stx	0xffff
    fc5a:	ff ff ff    	stx	0xffff
    fc5d:	ff ff ff    	stx	0xffff
    fc60:	ff ff ff    	stx	0xffff
    fc63:	ff ff ff    	stx	0xffff
    fc66:	ff ff ff    	stx	0xffff
    fc69:	ff ff ff    	stx	0xffff
    fc6c:	ff ff ff    	stx	0xffff
    fc6f:	ff ff ff    	stx	0xffff
    fc72:	ff ff ff    	stx	0xffff
    fc75:	ff ff ff    	stx	0xffff
    fc78:	ff ff ff    	stx	0xffff
    fc7b:	ff ff ff    	stx	0xffff
    fc7e:	ff ff ff    	stx	0xffff
    fc81:	ff ff ff    	stx	0xffff
    fc84:	ff ff ff    	stx	0xffff
    fc87:	ff ff ff    	stx	0xffff
    fc8a:	ff ff ff    	stx	0xffff
    fc8d:	ff ff ff    	stx	0xffff
    fc90:	ff ff ff    	stx	0xffff
    fc93:	ff ff ff    	stx	0xffff
    fc96:	ff ff ff    	stx	0xffff
    fc99:	ff ff ff    	stx	0xffff
    fc9c:	ff ff ff    	stx	0xffff
    fc9f:	ff ff ff    	stx	0xffff
    fca2:	ff ff ff    	stx	0xffff
    fca5:	ff ff ff    	stx	0xffff
    fca8:	ff ff ff    	stx	0xffff
    fcab:	ff ff ff    	stx	0xffff
    fcae:	ff ff ff    	stx	0xffff
    fcb1:	ff ff ff    	stx	0xffff
    fcb4:	ff ff ff    	stx	0xffff
    fcb7:	ff ff ff    	stx	0xffff
    fcba:	ff ff ff    	stx	0xffff
    fcbd:	ff ff ff    	stx	0xffff
    fcc0:	ff ff ff    	stx	0xffff
    fcc3:	ff ff ff    	stx	0xffff
    fcc6:	ff ff ff    	stx	0xffff
    fcc9:	ff ff ff    	stx	0xffff
    fccc:	ff ff ff    	stx	0xffff
    fccf:	ff ff ff    	stx	0xffff
    fcd2:	ff ff ff    	stx	0xffff
    fcd5:	ff ff ff    	stx	0xffff
    fcd8:	ff ff ff    	stx	0xffff
    fcdb:	ff ff ff    	stx	0xffff
    fcde:	ff ff ff    	stx	0xffff
    fce1:	ff ff ff    	stx	0xffff
    fce4:	ff ff ff    	stx	0xffff
    fce7:	ff ff ff    	stx	0xffff
    fcea:	ff ff ff    	stx	0xffff
    fced:	ff ff ff    	stx	0xffff
    fcf0:	ff ff ff    	stx	0xffff
    fcf3:	ff ff ff    	stx	0xffff
    fcf6:	ff ff ff    	stx	0xffff
    fcf9:	ff ff ff    	stx	0xffff
    fcfc:	ff ff ff    	stx	0xffff
    fcff:	ff ff ff    	stx	0xffff
    fd02:	ff ff ff    	stx	0xffff
    fd05:	ff ff ff    	stx	0xffff
    fd08:	ff ff ff    	stx	0xffff
    fd0b:	ff ff ff    	stx	0xffff
    fd0e:	ff ff ff    	stx	0xffff
    fd11:	ff ff ff    	stx	0xffff
    fd14:	ff ff ff    	stx	0xffff
    fd17:	ff ff ff    	stx	0xffff
    fd1a:	ff ff ff    	stx	0xffff
    fd1d:	ff ff ff    	stx	0xffff
    fd20:	ff ff ff    	stx	0xffff
    fd23:	ff ff ff    	stx	0xffff
    fd26:	ff ff ff    	stx	0xffff
    fd29:	ff ff ff    	stx	0xffff
    fd2c:	ff ff ff    	stx	0xffff
    fd2f:	ff ff ff    	stx	0xffff
    fd32:	ff ff ff    	stx	0xffff
    fd35:	ff ff ff    	stx	0xffff
    fd38:	ff ff ff    	stx	0xffff
    fd3b:	ff ff ff    	stx	0xffff
    fd3e:	ff ff ff    	stx	0xffff
    fd41:	ff ff ff    	stx	0xffff
    fd44:	ff ff ff    	stx	0xffff
    fd47:	ff ff ff    	stx	0xffff
    fd4a:	ff ff ff    	stx	0xffff
    fd4d:	ff ff ff    	stx	0xffff
    fd50:	ff ff ff    	stx	0xffff
    fd53:	ff ff ff    	stx	0xffff
    fd56:	ff ff ff    	stx	0xffff
    fd59:	ff ff ff    	stx	0xffff
    fd5c:	ff ff ff    	stx	0xffff
    fd5f:	ff ff ff    	stx	0xffff
    fd62:	ff ff ff    	stx	0xffff
    fd65:	ff ff ff    	stx	0xffff
    fd68:	ff ff ff    	stx	0xffff
    fd6b:	ff ff ff    	stx	0xffff
    fd6e:	ff ff ff    	stx	0xffff
    fd71:	ff ff ff    	stx	0xffff
    fd74:	ff ff ff    	stx	0xffff
    fd77:	ff ff ff    	stx	0xffff
    fd7a:	ff ff ff    	stx	0xffff
    fd7d:	ff ff ff    	stx	0xffff
    fd80:	ff ff ff    	stx	0xffff
    fd83:	ff ff ff    	stx	0xffff
    fd86:	ff ff ff    	stx	0xffff
    fd89:	ff ff ff    	stx	0xffff
    fd8c:	ff ff ff    	stx	0xffff
    fd8f:	ff ff ff    	stx	0xffff
    fd92:	ff ff ff    	stx	0xffff
    fd95:	ff ff ff    	stx	0xffff
    fd98:	ff ff ff    	stx	0xffff
    fd9b:	ff ff ff    	stx	0xffff
    fd9e:	ff ff ff    	stx	0xffff
    fda1:	ff ff ff    	stx	0xffff
    fda4:	ff ff ff    	stx	0xffff
    fda7:	ff ff ff    	stx	0xffff
    fdaa:	ff ff ff    	stx	0xffff
    fdad:	ff ff ff    	stx	0xffff
    fdb0:	ff ff ff    	stx	0xffff
    fdb3:	ff ff ff    	stx	0xffff
    fdb6:	ff ff ff    	stx	0xffff
    fdb9:	ff ff ff    	stx	0xffff
    fdbc:	ff ff ff    	stx	0xffff
    fdbf:	ff ff ff    	stx	0xffff
    fdc2:	ff ff ff    	stx	0xffff
    fdc5:	ff ff ff    	stx	0xffff
    fdc8:	ff ff ff    	stx	0xffff
    fdcb:	ff ff ff    	stx	0xffff
    fdce:	ff ff ff    	stx	0xffff
    fdd1:	ff ff ff    	stx	0xffff
    fdd4:	ff ff ff    	stx	0xffff
    fdd7:	ff ff ff    	stx	0xffff
    fdda:	ff ff ff    	stx	0xffff
    fddd:	ff ff ff    	stx	0xffff
    fde0:	ff ff ff    	stx	0xffff
    fde3:	ff ff ff    	stx	0xffff
    fde6:	ff ff ff    	stx	0xffff
    fde9:	ff ff ff    	stx	0xffff
    fdec:	ff ff ff    	stx	0xffff
    fdef:	ff 7e 95    	stx	0x7e95
    fdf2:	c4 ff       	andb	#0xff
    fdf4:	ff ff ff    	stx	0xffff
    fdf7:	ff ff ff    	stx	0xffff
    fdfa:	ff ff ff    	stx	0xffff
    fdfd:	ff ff ff    	stx	0xffff
    fe00:	7e ab fa    	jmp	0xabfa
    fe03:	ff ff ff    	stx	0xffff
    fe06:	ff ff ff    	stx	0xffff
    fe09:	ff ff ff    	stx	0xffff
    fe0c:	ff ff ff    	stx	0xffff
    fe0f:	ff ff ff    	stx	0xffff
    fe12:	ff ff ff    	stx	0xffff
    fe15:	ff ff ff    	stx	0xffff
    fe18:	ff ff ff    	stx	0xffff
    fe1b:	ff ff ff    	stx	0xffff
    fe1e:	ff ff f6    	stx	0xfff6
    fe21:	10          	sba
    fe22:	2e 37       	bgt	0x0xfe5b
    fe24:	c5 20       	bitb	#0x20
    fe26:	26 03       	bne	0x0xfe2b
    fe28:	7e fe f0    	jmp	0xfef0
    fe2b:	c5 02       	bitb	#0x2
    fe2d:	26 11       	bne	0x0xfe40
    fe2f:	c5 08       	bitb	#0x8
    fe31:	26 0d       	bne	0x0xfe40
    fe33:	b6 10 2f    	ldaa	0x102f
    fe36:	2b 0e       	bmi	0x0xfe46
    fe38:	7d 01 73    	tst	0x173
    fe3b:	26 31       	bne	0x0xfe6e
    fe3d:	7e fe f0    	jmp	0xfef0
    fe40:	b6 10 2f    	ldaa	0x102f
    fe43:	7e fe f0    	jmp	0xfef0
    fe46:	81 ef       	cmpa	#0xef
    fe48:	22 3b       	bhi	0x0xfe85
    fe4a:	16          	tab
    fe4b:	36          	psha
    fe4c:	c4 0f       	andb	#0xf
    fe4e:	f0 01 6f    	subb	0x16f
    fe51:	27 15       	beq	0x0xfe68
    fe53:	25 29       	bcs	0x0xfe7e
    fe55:	96 d1       	ldaa	*0xd1
    fe57:	81 06       	cmpa	#0x6
    fe59:	26 23       	bne	0x0xfe7e
    fe5b:	c1 07       	cmpb	#0x7
    fe5d:	22 1f       	bhi	0x0xfe7e
    fe5f:	32          	pula
    fe60:	b0 01 6f    	suba	0x16f
    fe63:	b7 01 73    	staa	0x173
    fe66:	20 06       	bra	0x0xfe6e
    fe68:	32          	pula
    fe69:	84 f0       	anda	#0xf0
    fe6b:	b7 01 73    	staa	0x173
    fe6e:	fe 01 c2    	ldx	0x1c2
    fe71:	a7 00       	staa	0x0,x
    fe73:	8f          	xgdx
    fe74:	5c          	incb
    fe75:	c4 bf       	andb	#0xbf
    fe77:	8f          	xgdx
    fe78:	ff 01 c2    	stx	0x1c2
    fe7b:	7e fe f0    	jmp	0xfef0
    fe7e:	32          	pula
    fe7f:	7f 01 73    	clr	0x173
    fe82:	7e fe f0    	jmp	0xfef0
    fe85:	81 f8       	cmpa	#0xf8
    fe87:	26 21       	bne	0x0xfeaa
    fe89:	d6 df       	ldab	*0xdf
    fe8b:	c1 f0       	cmpb	#0xf0
    fe8d:	26 03       	bne	0x0xfe92
    fe8f:	7e fe f0    	jmp	0xfef0
    fe92:	7d 00 dc    	tst	0xdc
    fe95:	27 11       	beq	0x0xfea8
    fe97:	13 f3 10 0d 	brclr	*0xf3, #0x10, 0x0xfea8
    fe9b:	d6 dc       	ldab	*0xdc
    fe9d:	ce f1 0e    	ldx	#0xf10e
    fea0:	3a          	abx
    fea1:	a6 00       	ldaa	0x0,x
    fea3:	bd 95 d9    	jsr	0x95d9
    fea6:	86 f8       	ldaa	#0xf8
    fea8:	20 c4       	bra	0x0xfe6e
    feaa:	81 ff       	cmpa	#0xff
    feac:	26 03       	bne	0x0xfeb1
    feae:	7e 80 00    	jmp	0x8000
    feb1:	81 fa       	cmpa	#0xfa
    feb3:	26 1b       	bne	0x0xfed0
    feb5:	c6 01       	ldab	#0x1
    feb7:	f7 01 72    	stab	0x172
    feba:	c6 80       	ldab	#0x80
    febc:	d7 d2       	stab	*0xd2
    febe:	7f 00 30    	clr	0x30
    fec1:	7f 00 d3    	clr	0xd3
    fec4:	d6 73       	ldab	*0x73
    fec6:	c1 02       	cmpb	#0x2
    fec8:	26 04       	bne	0x0xfece
    feca:	d6 74       	ldab	*0x74
    fecc:	d7 d3       	stab	*0xd3
    fece:	20 9e       	bra	0x0xfe6e
    fed0:	81 f7       	cmpa	#0xf7
    fed2:	26 0c       	bne	0x0xfee0
    fed4:	c6 f0       	ldab	#0xf0
    fed6:	f1 01 73    	cmpb	0x173
    fed9:	26 15       	bne	0x0xfef0
    fedb:	7f 01 73    	clr	0x173
    fede:	20 8e       	bra	0x0xfe6e
    fee0:	81 f0       	cmpa	#0xf0
    fee2:	26 05       	bne	0x0xfee9
    fee4:	b7 01 73    	staa	0x173
    fee7:	20 85       	bra	0x0xfe6e
    fee9:	81 f4       	cmpa	#0xf4
    feeb:	24 03       	bcc	0x0xfef0
    feed:	7f 01 73    	clr	0x173
    fef0:	33          	pulb
    fef1:	c5 80       	bitb	#0x80
    fef3:	27 02       	beq	0x0xfef7
    fef5:	8d 01       	bsr	0x0xfef8
    fef7:	3b          	rti
    fef8:	fe 01 60    	ldx	0x160
    fefb:	bc 01 62    	cpx	0x162
    fefe:	27 16       	beq	0x0xff16
    ff00:	a6 00       	ldaa	0x0,x
    ff02:	b7 10 2f    	staa	0x102f
    ff05:	8f          	xgdx
    ff06:	5c          	incb
    ff07:	c1 60       	cmpb	#0x60
    ff09:	25 02       	bcs	0x0xff0d
    ff0b:	c6 40       	ldab	#0x40
    ff0d:	fd 01 60    	std	0x160
    ff10:	86 ac       	ldaa	#0xac
    ff12:	b7 10 2d    	staa	0x102d
    ff15:	39          	rts
    ff16:	86 2c       	ldaa	#0x2c
    ff18:	b7 10 2d    	staa	0x102d
    ff1b:	7c 00 d7    	inc	0xd7
    ff1e:	39          	rts
    ff1f:	ff ff ff    	stx	0xffff
    ff22:	ff ff ff    	stx	0xffff
    ff25:	ff ff ff    	stx	0xffff
    ff28:	ff ff ff    	stx	0xffff
    ff2b:	ff ff ff    	stx	0xffff
    ff2e:	ff ff ff    	stx	0xffff
    ff31:	ff ff ff    	stx	0xffff
    ff34:	ff ff ff    	stx	0xffff
    ff37:	ff ff ff    	stx	0xffff
    ff3a:	ff ff ff    	stx	0xffff
    ff3d:	ff ff ff    	stx	0xffff
    ff40:	ff ff ff    	stx	0xffff
    ff43:	ff ff ff    	stx	0xffff
    ff46:	ff ff ff    	stx	0xffff
    ff49:	ff ff ff    	stx	0xffff
    ff4c:	ff ff ff    	stx	0xffff
    ff4f:	ff ff ff    	stx	0xffff
    ff52:	ff ff ff    	stx	0xffff
    ff55:	ff ff ff    	stx	0xffff
    ff58:	ff ff ff    	stx	0xffff
    ff5b:	ff ff ff    	stx	0xffff
    ff5e:	ff ff ff    	stx	0xffff
    ff61:	ff ff ff    	stx	0xffff
    ff64:	ff ff ff    	stx	0xffff
    ff67:	ff ff ff    	stx	0xffff
    ff6a:	ff ff ff    	stx	0xffff
    ff6d:	ff ff ff    	stx	0xffff
    ff70:	ff ff ff    	stx	0xffff
    ff73:	ff ff ff    	stx	0xffff
    ff76:	ff ff ff    	stx	0xffff
    ff79:	ff ff ff    	stx	0xffff
    ff7c:	ff ff ff    	stx	0xffff
    ff7f:	ff ff ff    	stx	0xffff
    ff82:	ff ff ff    	stx	0xffff
    ff85:	ff ff ff    	stx	0xffff
    ff88:	ff ff ff    	stx	0xffff
    ff8b:	ff ff ff    	stx	0xffff
    ff8e:	ff ff ff    	stx	0xffff
    ff91:	ff ff ff    	stx	0xffff
    ff94:	ff ff ff    	stx	0xffff
    ff97:	ff ff ff    	stx	0xffff
    ff9a:	ff ff ff    	stx	0xffff
    ff9d:	ff ff ff    	stx	0xffff
    ffa0:	7e 80 00    	jmp	0x8000
    ffa3:	ff ff ff    	stx	0xffff
    ffa6:	ff ff ff    	stx	0xffff
    ffa9:	ff ff ff    	stx	0xffff
    ffac:	ff ff ff    	stx	0xffff
    ffaf:	ff 4f 4d    	stx	0x4f4d
    ffb2:	38          	pulx
    ffb3:	20 56       	bra	0x0x1000b
    ffb5:	2e 32       	bgt	0x0xffe9
    ffb7:	2e 41       	bgt	0x0xfffa
    ffb9:	43          	coma
    ffba:	20 28       	bra	0x0xffe4
    ffbc:	43          	coma
    ffbd:	29 32       	bvs	0x0xfff1
    ffbf:	30          	tsx
    ffc0:	30          	tsx
    ffc1:	38          	pulx
    ffc2:	20 53       	bra	0x0x10017
    ffc4:	54          	lsrb
    ffc5:	55          	.byte	0x55
    ffc6:	44          	lsra
    ffc7:	49          	rola
    ffc8:	4f          	clra
    ffc9:	20 45       	bra	0x0x10010
    ffcb:	4c          	inca
    ffcc:	45          	.byte	0x45
    ffcd:	43          	coma
    ffce:	54          	lsrb
    ffcf:	52          	.byte	0x52
    ffd0:	4f          	clra
    ffd1:	4e          	.byte	0x4e
    ffd2:	49          	rola
    ffd3:	43          	coma
    ffd4:	53          	comb
    ffd5:	ff fe 20    	stx	0xfe20
    ffd8:	ff ff ff    	stx	0xffff
    ffdb:	ff ff ff    	stx	0xffff
    ffde:	ff ff fd    	stx	0xfffd
    ffe1:	f0 ff ff    	subb	0xffff
    ffe4:	ff ff ff    	stx	0xffff
    ffe7:	ff fe 00    	stx	0xfe00
    ffea:	ff ff ff    	stx	0xffff
    ffed:	ff ff ff    	stx	0xffff
    fff0:	ff ff ff    	stx	0xffff
    fff3:	ff ff ff    	stx	0xffff
    fff6:	fe 00 80    	ldx	0x80
    fff9:	00          	bgnd
    fffa:	80 00       	suba	#0x0
    fffc:	80 00       	suba	#0x0
    fffe:	80 00       	suba	#0x0
