
/home/user/.scratch_fw/images/Omega8_OS2a.decoded.bin:     file format binary


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
    807b:	bd ed 9c    	jsr	0xed9c
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
    90f5:	bd eb e4    	jsr	0xebe4
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
    a5af:	ce de db    	ldx	#0xdedb
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
    a5d6:	ce df 23    	ldx	#0xdf23
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
    a648:	ce df 2c    	ldx	#0xdf2c
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
    aae0:	bd ea a3    	jsr	0xeaa3
    aae3:	ce 10 23    	ldx	#0x1023
    aae6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xaae6
    aaea:	bd ea d7    	jsr	0xead7
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
    ab10:	bd ea 2b    	jsr	0xea2b
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
    ab40:	bd ea d7    	jsr	0xead7
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
    ab66:	bd ea 2b    	jsr	0xea2b
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
    ab95:	fc bd ea    	ldd	0xbdea
    ab98:	d7 ce       	stab	*0xce
    ab9a:	01          	nop
    ab9b:	20 18       	bra	0x0xabb5
    ab9d:	ce ab c5    	ldx	#0xabc5
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
    abbc:	bd ea 2b    	jsr	0xea2b
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
    ae62:	ce ef 5c    	ldx	#0xef5c
    ae65:	3a          	abx
    ae66:	a6 00       	ldaa	0x0,x
    ae68:	97 71       	staa	*0x71
    ae6a:	97 db       	staa	*0xdb
    ae6c:	c6 74       	ldab	#0x74
    ae6e:	bd b1 9a    	jsr	0xb19a
    ae71:	7e ac ae    	jmp	0xacae
    ae74:	16          	tab
    ae75:	ce ef 5c    	ldx	#0xef5c
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
    ae92:	bd eb 8a    	jsr	0xeb8a
    ae95:	86 03       	ldaa	#0x3
    ae97:	b7 01 1c    	staa	0x11c
    ae9a:	86 13       	ldaa	#0x13
    ae9c:	b7 01 1e    	staa	0x11e
    ae9f:	bd ea b2    	jsr	0xeab2
    aea2:	96 2a       	ldaa	*0x2a
    aea4:	c6 48       	ldab	#0x48
    aea6:	bd b1 9a    	jsr	0xb19a
    aea9:	7e ac ae    	jmp	0xacae
    aeac:	16          	tab
    aead:	ce ef 5c    	ldx	#0xef5c
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
    aeca:	bd eb 8a    	jsr	0xeb8a
    aecd:	86 03       	ldaa	#0x3
    aecf:	b7 01 1c    	staa	0x11c
    aed2:	86 13       	ldaa	#0x13
    aed4:	b7 01 1e    	staa	0x11e
    aed7:	bd ea b2    	jsr	0xeab2
    aeda:	96 37       	ldaa	*0x37
    aedc:	c6 4a       	ldab	#0x4a
    aede:	bd b1 9a    	jsr	0xb19a
    aee1:	7e ac ae    	jmp	0xacae
    aee4:	16          	tab
    aee5:	ce ef 5c    	ldx	#0xef5c
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
    af5c:	bd eb 8a    	jsr	0xeb8a
    af5f:	86 03       	ldaa	#0x3
    af61:	b7 01 1c    	staa	0x11c
    af64:	86 1e       	ldaa	#0x1e
    af66:	b7 01 1e    	staa	0x11e
    af69:	bd ea b2    	jsr	0xeab2
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
    af8a:	bd eb 8a    	jsr	0xeb8a
    af8d:	86 03       	ldaa	#0x3
    af8f:	b7 01 1c    	staa	0x11c
    af92:	86 19       	ldaa	#0x19
    af94:	b7 01 1e    	staa	0x11e
    af97:	bd ea b2    	jsr	0xeab2
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
    b020:	bd eb 8a    	jsr	0xeb8a
    b023:	86 03       	ldaa	#0x3
    b025:	b7 01 1c    	staa	0x11c
    b028:	86 13       	ldaa	#0x13
    b02a:	b7 01 1e    	staa	0x11e
    b02d:	bd ea b2    	jsr	0xeab2
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
    b207:	28 c4       	bvc	0x0xb1cd
    b209:	f8 c6 d6    	eorb	0xc6d6
    b20c:	c8 05       	eorb	#0x5
    b20e:	c9 6c       	adcb	#0x6c
    b210:	c9 6f       	adcb	#0x6f
    b212:	ca aa       	orab	#0xaa
    b214:	cb 71       	addb	#0x71
    b216:	cc 38 cd    	ldd	#0x38cd
    b219:	19          	daa
    b21a:	cd 25       	.byte	0xcd, 0x25
    b21c:	ce 17 ce    	ldx	#0x17ce
    b21f:	18 ce a2 cf 	ldy	#0xa2cf
    b223:	54          	lsrb
    b224:	d0 57       	subb	*0x57
    b226:	d0 5a       	subb	*0x5a
    b228:	d1 2a       	cmpb	*0x2a
    b22a:	d2 e0       	sbcb	*0xe0
    b22c:	d3 bc       	addd	*0xbc
    b22e:	f6 01 1b    	ldab	0x11b
    b231:	58          	aslb
    b232:	ce b1 d6    	ldx	#0xb1d6
    b235:	3a          	abx
    b236:	ee 00       	ldx	0x0,x
    b238:	6e 00       	jmp	0x0,x
    b23a:	7d 01 1c    	tst	0x11c
    b23d:	2a 03       	bpl	0x0xb242
    b23f:	7e d5 3b    	jmp	0xd53b
    b242:	7d 01 1c    	tst	0x11c
    b245:	27 03       	beq	0x0xb24a
    b247:	7e b2 70    	jmp	0xb270
    b24a:	7d 01 1e    	tst	0x11e
    b24d:	26 08       	bne	0x0xb257
    b24f:	86 0c       	ldaa	#0xc
    b251:	b7 01 1e    	staa	0x11e
    b254:	bd ea b2    	jsr	0xeab2
    b257:	7d 01 7e    	tst	0x17e
    b25a:	27 0c       	beq	0x0xb268
    b25c:	7f 01 7e    	clr	0x17e
    b25f:	7f 01 7f    	clr	0x17f
    b262:	7f 01 1a    	clr	0x11a
    b265:	7e d5 3b    	jmp	0xd53b
    b268:	7d 01 1c    	tst	0x11c
    b26b:	26 03       	bne	0x0xb270
    b26d:	7e d5 27    	jmp	0xd527
    b270:	b6 10 23    	ldaa	0x1023
    b273:	85 10       	bita	#0x10
    b275:	27 03       	beq	0x0xb27a
    b277:	7e ea 26    	jmp	0xea26
    b27a:	7e d5 27    	jmp	0xd527
    b27d:	7d 01 1c    	tst	0x11c
    b280:	2a 03       	bpl	0x0xb285
    b282:	7e d6 05    	jmp	0xd605
    b285:	7d 01 1c    	tst	0x11c
    b288:	27 03       	beq	0x0xb28d
    b28a:	7e b2 70    	jmp	0xb270
    b28d:	7d 01 1e    	tst	0x11e
    b290:	26 08       	bne	0x0xb29a
    b292:	86 0c       	ldaa	#0xc
    b294:	b7 01 1e    	staa	0x11e
    b297:	bd ea b2    	jsr	0xeab2
    b29a:	7d 01 7e    	tst	0x17e
    b29d:	27 03       	beq	0x0xb2a2
    b29f:	7e d6 05    	jmp	0xd605
    b2a2:	7e d5 27    	jmp	0xd527
    b2a5:	7d 01 1c    	tst	0x11c
    b2a8:	2a 03       	bpl	0x0xb2ad
    b2aa:	7e d7 2a    	jmp	0xd72a
    b2ad:	7d 01 1c    	tst	0x11c
    b2b0:	27 03       	beq	0x0xb2b5
    b2b2:	7e b2 70    	jmp	0xb270
    b2b5:	7d 01 1e    	tst	0x11e
    b2b8:	26 08       	bne	0x0xb2c2
    b2ba:	86 10       	ldaa	#0x10
    b2bc:	b7 01 1e    	staa	0x11e
    b2bf:	bd ea b2    	jsr	0xeab2
    b2c2:	7d 01 7f    	tst	0x17f
    b2c5:	27 29       	beq	0x0xb2f0
    b2c7:	2b 1d       	bmi	0x0xb2e6
    b2c9:	b6 01 1e    	ldaa	0x11e
    b2cc:	4c          	inca
    b2cd:	81 1f       	cmpa	#0x1f
    b2cf:	23 02       	bls	0x0xb2d3
    b2d1:	20 06       	bra	0x0xb2d9
    b2d3:	b7 01 1e    	staa	0x11e
    b2d6:	bd ea b2    	jsr	0xeab2
    b2d9:	4f          	clra
    b2da:	b7 01 7f    	staa	0x17f
    b2dd:	b7 01 7e    	staa	0x17e
    b2e0:	b7 01 1a    	staa	0x11a
    b2e3:	7e d5 27    	jmp	0xd527
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
    b309:	7e d5 27    	jmp	0xd527
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
    b336:	bd ea 76    	jsr	0xea76
    b339:	13 d1 80 0e 	brclr	*0xd1, #0x80, 0x0xb34b
    b33d:	32          	pula
    b33e:	8f          	xgdx
    b33f:	83 00 70    	subd	#0x70
    b342:	8f          	xgdx
    b343:	a7 00       	staa	0x0,x
    b345:	bd ea b2    	jsr	0xeab2
    b348:	7e d5 27    	jmp	0xd527
    b34b:	32          	pula
    b34c:	8f          	xgdx
    b34d:	83 01 30    	subd	#0x130
    b350:	c3 50 40    	addd	#0x5040
    b353:	8f          	xgdx
    b354:	a7 00       	staa	0x0,x
    b356:	bd ea b2    	jsr	0xeab2
    b359:	14 fb 80    	bset	*0xfb, #0x80
    b35c:	7e d5 27    	jmp	0xd527
    b35f:	7d 01 1c    	tst	0x11c
    b362:	2a 03       	bpl	0x0xb367
    b364:	7e d7 be    	jmp	0xd7be
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
    b380:	bd ea 76    	jsr	0xea76
    b383:	7a 01 1e    	dec	0x11e
    b386:	7a 01 1c    	dec	0x11c
    b389:	26 0d       	bne	0x0xb398
    b38b:	bd ea f4    	jsr	0xeaf4
    b38e:	20 08       	bra	0x0xb398
    b390:	86 13       	ldaa	#0x13
    b392:	b7 01 1e    	staa	0x11e
    b395:	bd ea b2    	jsr	0xeab2
    b398:	7d 01 7f    	tst	0x17f
    b39b:	27 10       	beq	0x0xb3ad
    b39d:	bd ec b6    	jsr	0xecb6
    b3a0:	4f          	clra
    b3a1:	b7 01 7f    	staa	0x17f
    b3a4:	b7 01 7e    	staa	0x17e
    b3a7:	b7 01 1a    	staa	0x11a
    b3aa:	7e d5 27    	jmp	0xd527
    b3ad:	7d 01 1a    	tst	0x11a
    b3b0:	26 03       	bne	0x0xb3b5
    b3b2:	7e d5 27    	jmp	0xd527
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
    b3e7:	ce d8 1c    	ldx	#0xd81c
    b3ea:	18 ce 01 30 	ldy	#0x130
    b3ee:	bd ec 3d    	jsr	0xec3d
    b3f1:	7f 01 1a    	clr	0x11a
    b3f4:	86 04       	ldaa	#0x4
    b3f6:	b7 01 1c    	staa	0x11c
    b3f9:	7e d5 27    	jmp	0xd527
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
    b416:	ce d8 60    	ldx	#0xd860
    b419:	18 ce 01 36 	ldy	#0x136
    b41d:	bd ec 3d    	jsr	0xec3d
    b420:	7f 01 1a    	clr	0x11a
    b423:	86 04       	ldaa	#0x4
    b425:	b7 01 1c    	staa	0x11c
    b428:	96 93       	ldaa	*0x93
    b42a:	c6 93       	ldab	#0x93
    b42c:	bd b0 fc    	jsr	0xb0fc
    b42f:	7e d5 27    	jmp	0xd527
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
    b446:	18 ce ef dc 	ldy	#0xefdc
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
    b4a9:	7e d5 27    	jmp	0xd527
    b4ac:	ce 50 00    	ldx	#0x5000
    b4af:	18 ce f0 8c 	ldy	#0xf08c
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
    b4ce:	7e d5 27    	jmp	0xd527
    b4d1:	7d 01 1c    	tst	0x11c
    b4d4:	2a 03       	bpl	0x0xb4d9
    b4d6:	7e d8 7a    	jmp	0xd87a
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
    b4f2:	bd ea 76    	jsr	0xea76
    b4f5:	7a 01 1e    	dec	0x11e
    b4f8:	7a 01 1c    	dec	0x11c
    b4fb:	26 12       	bne	0x0xb50f
    b4fd:	bd ea f4    	jsr	0xeaf4
    b500:	20 0d       	bra	0x0xb50f
    b502:	7d 01 1e    	tst	0x11e
    b505:	26 08       	bne	0x0xb50f
    b507:	86 13       	ldaa	#0x13
    b509:	b7 01 1e    	staa	0x11e
    b50c:	bd ea b2    	jsr	0xeab2
    b50f:	7d 01 7f    	tst	0x17f
    b512:	27 10       	beq	0x0xb524
    b514:	bd ec b6    	jsr	0xecb6
    b517:	4f          	clra
    b518:	b7 01 7f    	staa	0x17f
    b51b:	b7 01 7e    	staa	0x17e
    b51e:	b7 01 1a    	staa	0x11a
    b521:	7e d5 27    	jmp	0xd527
    b524:	7d 01 1a    	tst	0x11a
    b527:	26 03       	bne	0x0xb52c
    b529:	7e d5 27    	jmp	0xd527
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
    b561:	ce d8 d6    	ldx	#0xd8d6
    b564:	18 ce 01 30 	ldy	#0x130
    b568:	bd ec 3d    	jsr	0xec3d
    b56b:	7f 01 1a    	clr	0x11a
    b56e:	86 04       	ldaa	#0x4
    b570:	b7 01 1c    	staa	0x11c
    b573:	7e d5 27    	jmp	0xd527
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
    b593:	ce d8 e2    	ldx	#0xd8e2
    b596:	18 ce 01 36 	ldy	#0x136
    b59a:	bd ec 3d    	jsr	0xec3d
    b59d:	7f 01 1a    	clr	0x11a
    b5a0:	86 04       	ldaa	#0x4
    b5a2:	b7 01 1c    	staa	0x11c
    b5a5:	7e d5 27    	jmp	0xd527
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
    b5e6:	bd eb 8a    	jsr	0xeb8a
    b5e9:	7f 01 1a    	clr	0x11a
    b5ec:	86 03       	ldaa	#0x3
    b5ee:	b7 01 1c    	staa	0x11c
    b5f1:	7e d5 27    	jmp	0xd527
    b5f4:	b6 01 71    	ldaa	0x171
    b5f7:	4a          	deca
    b5f8:	2a c6       	bpl	0x0xb5c0
    b5fa:	4f          	clra
    b5fb:	20 c3       	bra	0x0xb5c0
    b5fd:	7d 01 1c    	tst	0x11c
    b600:	2a 03       	bpl	0x0xb605
    b602:	7e d8 ee    	jmp	0xd8ee
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
    b61e:	bd ea 76    	jsr	0xea76
    b621:	7a 01 1e    	dec	0x11e
    b624:	7a 01 1c    	dec	0x11c
    b627:	26 12       	bne	0x0xb63b
    b629:	bd ea f4    	jsr	0xeaf4
    b62c:	20 0d       	bra	0x0xb63b
    b62e:	7d 01 1e    	tst	0x11e
    b631:	26 08       	bne	0x0xb63b
    b633:	86 1e       	ldaa	#0x1e
    b635:	b7 01 1e    	staa	0x11e
    b638:	bd ea b2    	jsr	0xeab2
    b63b:	7f 01 7e    	clr	0x17e
    b63e:	7f 01 7f    	clr	0x17f
    b641:	7d 01 1a    	tst	0x11a
    b644:	26 03       	bne	0x0xb649
    b646:	7e d5 27    	jmp	0xd527
    b649:	b6 01 3c    	ldaa	0x13c
    b64c:	81 41       	cmpa	#0x41
    b64e:	26 04       	bne	0x0xb654
    b650:	86 81       	ldaa	#0x81
    b652:	20 06       	bra	0x0xb65a
    b654:	ce 01 3c    	ldx	#0x13c
    b657:	bd eb e4    	jsr	0xebe4
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
    b683:	bd eb 8a    	jsr	0xeb8a
    b686:	7f 01 1a    	clr	0x11a
    b689:	86 03       	ldaa	#0x3
    b68b:	b7 01 1c    	staa	0x11c
    b68e:	14 f2 40    	bset	*0xf2, #0x40
    b691:	7e d5 27    	jmp	0xd527
    b694:	7d 01 1c    	tst	0x11c
    b697:	2a 03       	bpl	0x0xb69c
    b699:	7e d9 79    	jmp	0xd979
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
    b6b5:	bd ea 76    	jsr	0xea76
    b6b8:	7a 01 1e    	dec	0x11e
    b6bb:	7a 01 1c    	dec	0x11c
    b6be:	26 12       	bne	0x0xb6d2
    b6c0:	bd ea f4    	jsr	0xeaf4
    b6c3:	20 0d       	bra	0x0xb6d2
    b6c5:	7d 01 1e    	tst	0x11e
    b6c8:	26 08       	bne	0x0xb6d2
    b6ca:	86 13       	ldaa	#0x13
    b6cc:	b7 01 1e    	staa	0x11e
    b6cf:	bd ea b2    	jsr	0xeab2
    b6d2:	7d 01 7f    	tst	0x17f
    b6d5:	27 10       	beq	0x0xb6e7
    b6d7:	bd ec b6    	jsr	0xecb6
    b6da:	4f          	clra
    b6db:	b7 01 7f    	staa	0x17f
    b6de:	b7 01 7e    	staa	0x17e
    b6e1:	b7 01 1a    	staa	0x11a
    b6e4:	7e d5 27    	jmp	0xd527
    b6e7:	7d 01 1a    	tst	0x11a
    b6ea:	26 03       	bne	0x0xb6ef
    b6ec:	7e d5 27    	jmp	0xd527
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
    b709:	bd eb c6    	jsr	0xebc6
    b70c:	7f 01 1a    	clr	0x11a
    b70f:	86 03       	ldaa	#0x3
    b711:	b7 01 1c    	staa	0x11c
    b714:	96 40       	ldaa	*0x40
    b716:	c6 40       	ldab	#0x40
    b718:	bd b0 fc    	jsr	0xb0fc
    b71b:	7e d5 27    	jmp	0xd527
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
    b741:	ce d9 e0    	ldx	#0xd9e0
    b744:	18 ce 01 36 	ldy	#0x136
    b748:	bd ec 3d    	jsr	0xec3d
    b74b:	7f 01 1a    	clr	0x11a
    b74e:	86 04       	ldaa	#0x4
    b750:	b7 01 1c    	staa	0x11c
    b753:	96 41       	ldaa	*0x41
    b755:	c6 41       	ldab	#0x41
    b757:	bd b0 fc    	jsr	0xb0fc
    b75a:	7e d5 27    	jmp	0xd527
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
    b776:	bd eb 8a    	jsr	0xeb8a
    b779:	7f 01 1a    	clr	0x11a
    b77c:	86 03       	ldaa	#0x3
    b77e:	b7 01 1c    	staa	0x11c
    b781:	96 67       	ldaa	*0x67
    b783:	c6 67       	ldab	#0x67
    b785:	bd b0 fc    	jsr	0xb0fc
    b788:	7e d5 27    	jmp	0xd527
    b78b:	96 67       	ldaa	*0x67
    b78d:	4a          	deca
    b78e:	84 7f       	anda	#0x7f
    b790:	20 df       	bra	0x0xb771
    b792:	7d 01 1c    	tst	0x11c
    b795:	2a 03       	bpl	0x0xb79a
    b797:	7e d9 f4    	jmp	0xd9f4
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
    b7b3:	bd ea 76    	jsr	0xea76
    b7b6:	7a 01 1e    	dec	0x11e
    b7b9:	7a 01 1c    	dec	0x11c
    b7bc:	26 12       	bne	0x0xb7d0
    b7be:	bd ea f4    	jsr	0xeaf4
    b7c1:	20 0d       	bra	0x0xb7d0
    b7c3:	7d 01 1e    	tst	0x11e
    b7c6:	26 08       	bne	0x0xb7d0
    b7c8:	86 13       	ldaa	#0x13
    b7ca:	b7 01 1e    	staa	0x11e
    b7cd:	bd ea b2    	jsr	0xeab2
    b7d0:	7d 01 7f    	tst	0x17f
    b7d3:	27 10       	beq	0x0xb7e5
    b7d5:	bd ec b6    	jsr	0xecb6
    b7d8:	4f          	clra
    b7d9:	b7 01 7f    	staa	0x17f
    b7dc:	b7 01 7e    	staa	0x17e
    b7df:	b7 01 1a    	staa	0x11a
    b7e2:	7e d5 27    	jmp	0xd527
    b7e5:	7d 01 1a    	tst	0x11a
    b7e8:	26 03       	bne	0x0xb7ed
    b7ea:	7e d5 27    	jmp	0xd527
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
    b826:	bd eb c6    	jsr	0xebc6
    b829:	7f 01 1a    	clr	0x11a
    b82c:	86 03       	ldaa	#0x3
    b82e:	b7 01 1c    	staa	0x11c
    b831:	b6 01 6e    	ldaa	0x16e
    b834:	c6 ab       	ldab	#0xab
    b836:	bd b0 fc    	jsr	0xb0fc
    b839:	7e d5 27    	jmp	0xd527
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
    b85c:	bd eb 8a    	jsr	0xeb8a
    b85f:	7f 01 1a    	clr	0x11a
    b862:	86 03       	ldaa	#0x3
    b864:	b7 01 1c    	staa	0x11c
    b867:	96 3e       	ldaa	*0x3e
    b869:	c6 3e       	ldab	#0x3e
    b86b:	bd b0 fc    	jsr	0xb0fc
    b86e:	7e d5 27    	jmp	0xd527
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
    b88c:	bd eb 8a    	jsr	0xeb8a
    b88f:	7f 01 1a    	clr	0x11a
    b892:	86 03       	ldaa	#0x3
    b894:	b7 01 1c    	staa	0x11c
    b897:	96 3f       	ldaa	*0x3f
    b899:	c6 3f       	ldab	#0x3f
    b89b:	bd b0 fc    	jsr	0xb0fc
    b89e:	7e d5 27    	jmp	0xd527
    b8a1:	96 3f       	ldaa	*0x3f
    b8a3:	4a          	deca
    b8a4:	2a e1       	bpl	0x0xb887
    b8a6:	4f          	clra
    b8a7:	20 de       	bra	0x0xb887
    b8a9:	7d 01 1c    	tst	0x11c
    b8ac:	2a 03       	bpl	0x0xb8b1
    b8ae:	7e da 56    	jmp	0xda56
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
    b8ca:	bd ea 76    	jsr	0xea76
    b8cd:	7a 01 1e    	dec	0x11e
    b8d0:	7a 01 1c    	dec	0x11c
    b8d3:	26 12       	bne	0x0xb8e7
    b8d5:	bd eb 27    	jsr	0xeb27
    b8d8:	20 0d       	bra	0x0xb8e7
    b8da:	7d 01 1e    	tst	0x11e
    b8dd:	26 08       	bne	0x0xb8e7
    b8df:	86 12       	ldaa	#0x12
    b8e1:	b7 01 1e    	staa	0x11e
    b8e4:	bd ea b2    	jsr	0xeab2
    b8e7:	7d 01 7f    	tst	0x17f
    b8ea:	27 10       	beq	0x0xb8fc
    b8ec:	bd ed 28    	jsr	0xed28
    b8ef:	4f          	clra
    b8f0:	b7 01 7f    	staa	0x17f
    b8f3:	b7 01 7e    	staa	0x17e
    b8f6:	b7 01 1a    	staa	0x11a
    b8f9:	7e d5 27    	jmp	0xd527
    b8fc:	7d 01 1a    	tst	0x11a
    b8ff:	26 03       	bne	0x0xb904
    b901:	7e d5 27    	jmp	0xd527
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
    b919:	ce da e4    	ldx	#0xdae4
    b91c:	18 ce 01 30 	ldy	#0x130
    b920:	bd ec 29    	jsr	0xec29
    b923:	7f 01 1a    	clr	0x11a
    b926:	86 03       	ldaa	#0x3
    b928:	b7 01 1c    	staa	0x11c
    b92b:	96 21       	ldaa	*0x21
    b92d:	c6 21       	ldab	#0x21
    b92f:	bd b0 fc    	jsr	0xb0fc
    b932:	7e d5 27    	jmp	0xd527
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
    b94b:	ce da f0    	ldx	#0xdaf0
    b94e:	bd ec 29    	jsr	0xec29
    b951:	20 d0       	bra	0x0xb923
    b953:	7d 01 1a    	tst	0x11a
    b956:	2b 27       	bmi	0x0xb97f
    b958:	d6 24       	ldab	*0x24
    b95a:	5c          	incb
    b95b:	c1 07       	cmpb	#0x7
    b95d:	25 02       	bcs	0x0xb961
    b95f:	c6 06       	ldab	#0x6
    b961:	d7 24       	stab	*0x24
    b963:	ce da f6    	ldx	#0xdaf6
    b966:	18 ce 01 3c 	ldy	#0x13c
    b96a:	bd ec 29    	jsr	0xec29
    b96d:	7f 01 1a    	clr	0x11a
    b970:	86 03       	ldaa	#0x3
    b972:	b7 01 1c    	staa	0x11c
    b975:	96 24       	ldaa	*0x24
    b977:	c6 24       	ldab	#0x24
    b979:	bd b0 fc    	jsr	0xb0fc
    b97c:	7e d5 27    	jmp	0xd527
    b97f:	d6 24       	ldab	*0x24
    b981:	5a          	decb
    b982:	2a dd       	bpl	0x0xb961
    b984:	5f          	clrb
    b985:	20 da       	bra	0x0xb961
    b987:	7d 01 1c    	tst	0x11c
    b98a:	2a 03       	bpl	0x0xb98f
    b98c:	7e db 0b    	jmp	0xdb0b
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
    b9a8:	bd ea 76    	jsr	0xea76
    b9ab:	7a 01 1e    	dec	0x11e
    b9ae:	7a 01 1c    	dec	0x11c
    b9b1:	26 12       	bne	0x0xb9c5
    b9b3:	bd ea f4    	jsr	0xeaf4
    b9b6:	20 0d       	bra	0x0xb9c5
    b9b8:	7d 01 1e    	tst	0x11e
    b9bb:	26 08       	bne	0x0xb9c5
    b9bd:	86 13       	ldaa	#0x13
    b9bf:	b7 01 1e    	staa	0x11e
    b9c2:	bd ea b2    	jsr	0xeab2
    b9c5:	7d 01 7f    	tst	0x17f
    b9c8:	27 10       	beq	0x0xb9da
    b9ca:	bd ec b6    	jsr	0xecb6
    b9cd:	4f          	clra
    b9ce:	b7 01 7f    	staa	0x17f
    b9d1:	b7 01 7e    	staa	0x17e
    b9d4:	b7 01 1a    	staa	0x11a
    b9d7:	7e d5 27    	jmp	0xd527
    b9da:	7d 01 1a    	tst	0x11a
    b9dd:	26 03       	bne	0x0xb9e2
    b9df:	7e d5 27    	jmp	0xd527
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
    ba03:	ce db 7a    	ldx	#0xdb7a
    ba06:	18 ce 01 30 	ldy	#0x130
    ba0a:	bd ec 3d    	jsr	0xec3d
    ba0d:	7f 01 1a    	clr	0x11a
    ba10:	86 04       	ldaa	#0x4
    ba12:	b7 01 1c    	staa	0x11c
    ba15:	7e d5 27    	jmp	0xd527
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
    ba31:	ce db 8e    	ldx	#0xdb8e
    ba34:	18 ce 01 36 	ldy	#0x136
    ba38:	bd ec 3d    	jsr	0xec3d
    ba3b:	7f 01 1a    	clr	0x11a
    ba3e:	86 04       	ldaa	#0x4
    ba40:	b7 01 1c    	staa	0x11c
    ba43:	7e d5 27    	jmp	0xd527
    ba46:	d6 69       	ldab	*0x69
    ba48:	5c          	incb
    ba49:	c4 01       	andb	#0x1
    ba4b:	d7 69       	stab	*0x69
    ba4d:	ce db 96    	ldx	#0xdb96
    ba50:	18 ce 01 3b 	ldy	#0x13b
    ba54:	bd ec 3d    	jsr	0xec3d
    ba57:	7f 01 1a    	clr	0x11a
    ba5a:	86 04       	ldaa	#0x4
    ba5c:	b7 01 1c    	staa	0x11c
    ba5f:	96 69       	ldaa	*0x69
    ba61:	c6 69       	ldab	#0x69
    ba63:	bd b0 fc    	jsr	0xb0fc
    ba66:	7e d5 27    	jmp	0xd527
    ba69:	7d 01 1c    	tst	0x11c
    ba6c:	2a 03       	bpl	0x0xba71
    ba6e:	7e db 9e    	jmp	0xdb9e
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
    ba8a:	bd ea 76    	jsr	0xea76
    ba8d:	7a 01 1e    	dec	0x11e
    ba90:	7a 01 1c    	dec	0x11c
    ba93:	26 12       	bne	0x0xbaa7
    ba95:	bd ea f4    	jsr	0xeaf4
    ba98:	20 0d       	bra	0x0xbaa7
    ba9a:	7d 01 1e    	tst	0x11e
    ba9d:	26 08       	bne	0x0xbaa7
    ba9f:	86 13       	ldaa	#0x13
    baa1:	b7 01 1e    	staa	0x11e
    baa4:	bd ea b2    	jsr	0xeab2
    baa7:	7d 01 7f    	tst	0x17f
    baaa:	27 1e       	beq	0x0xbaca
    baac:	2a 10       	bpl	0x0xbabe
    baae:	bd ec b6    	jsr	0xecb6
    bab1:	4f          	clra
    bab2:	b7 01 7f    	staa	0x17f
    bab5:	b7 01 7e    	staa	0x17e
    bab8:	b7 01 1a    	staa	0x11a
    babb:	7e d5 27    	jmp	0xd527
    babe:	b6 01 1e    	ldaa	0x11e
    bac1:	81 19       	cmpa	#0x19
    bac3:	27 ec       	beq	0x0xbab1
    bac5:	bd ec b6    	jsr	0xecb6
    bac8:	20 e7       	bra	0x0xbab1
    baca:	7d 01 1a    	tst	0x11a
    bacd:	26 03       	bne	0x0xbad2
    bacf:	7e d5 27    	jmp	0xd527
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
    bae9:	ce db f0    	ldx	#0xdbf0
    baec:	18 ce 01 30 	ldy	#0x130
    baf0:	bd ec 3d    	jsr	0xec3d
    baf3:	7f 01 1a    	clr	0x11a
    baf6:	86 04       	ldaa	#0x4
    baf8:	b7 01 1c    	staa	0x11c
    bafb:	96 6b       	ldaa	*0x6b
    bafd:	c6 6b       	ldab	#0x6b
    baff:	bd b0 fc    	jsr	0xb0fc
    bb02:	7e d5 27    	jmp	0xd527
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
    bb1d:	ce db fc    	ldx	#0xdbfc
    bb20:	18 ce 01 36 	ldy	#0x136
    bb24:	bd ec 3d    	jsr	0xec3d
    bb27:	7f 01 1a    	clr	0x11a
    bb2a:	86 04       	ldaa	#0x4
    bb2c:	b7 01 1c    	staa	0x11c
    bb2f:	96 68       	ldaa	*0x68
    bb31:	c6 68       	ldab	#0x68
    bb33:	bd b0 fc    	jsr	0xb0fc
    bb36:	7e d5 27    	jmp	0xd527
    bb39:	d6 68       	ldab	*0x68
    bb3b:	5a          	decb
    bb3c:	2a dd       	bpl	0x0xbb1b
    bb3e:	5f          	clrb
    bb3f:	20 da       	bra	0x0xbb1b
    bb41:	7e d5 27    	jmp	0xd527
    bb44:	7d 01 1c    	tst	0x11c
    bb47:	2a 03       	bpl	0x0xbb4c
    bb49:	7e dc 1c    	jmp	0xdc1c
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
    bb65:	bd ea 76    	jsr	0xea76
    bb68:	7a 01 1e    	dec	0x11e
    bb6b:	7a 01 1c    	dec	0x11c
    bb6e:	26 12       	bne	0x0xbb82
    bb70:	bd ea f4    	jsr	0xeaf4
    bb73:	20 0d       	bra	0x0xbb82
    bb75:	7d 01 1e    	tst	0x11e
    bb78:	26 08       	bne	0x0xbb82
    bb7a:	86 09       	ldaa	#0x9
    bb7c:	b7 01 1e    	staa	0x11e
    bb7f:	bd ea b2    	jsr	0xeab2
    bb82:	7d 01 7f    	tst	0x17f
    bb85:	27 10       	beq	0x0xbb97
    bb87:	bd ec 75    	jsr	0xec75
    bb8a:	4f          	clra
    bb8b:	b7 01 7f    	staa	0x17f
    bb8e:	b7 01 7e    	staa	0x17e
    bb91:	b7 01 1a    	staa	0x11a
    bb94:	7e d5 27    	jmp	0xd527
    bb97:	7d 01 7e    	tst	0x17e
    bb9a:	27 0c       	beq	0x0xbba8
    bb9c:	bd ec 5e    	jsr	0xec5e
    bb9f:	7f 01 7e    	clr	0x17e
    bba2:	7f 01 1a    	clr	0x11a
    bba5:	7e d5 27    	jmp	0xd527
    bba8:	7d 01 1a    	tst	0x11a
    bbab:	26 03       	bne	0x0xbbb0
    bbad:	7e d5 27    	jmp	0xd527
    bbb0:	b6 01 1e    	ldaa	0x11e
    bbb3:	81 09       	cmpa	#0x9
    bbb5:	26 29       	bne	0x0xbbe0
    bbb7:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xbbc1
    bbbb:	7f 01 1a    	clr	0x11a
    bbbe:	7e d5 27    	jmp	0xd527
    bbc1:	ce 00 76    	ldx	#0x76
    bbc4:	bd bc 59    	jsr	0xbc59
    bbc7:	bd bc 8d    	jsr	0xbc8d
    bbca:	16          	tab
    bbcb:	ce e0 4b    	ldx	#0xe04b
    bbce:	18 ce 01 26 	ldy	#0x126
    bbd2:	bd ec 3d    	jsr	0xec3d
    bbd5:	86 04       	ldaa	#0x4
    bbd7:	b7 01 1c    	staa	0x11c
    bbda:	7f 01 1a    	clr	0x11a
    bbdd:	7e d5 27    	jmp	0xd527
    bbe0:	81 0e       	cmpa	#0xe
    bbe2:	26 29       	bne	0x0xbc0d
    bbe4:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xbbee
    bbe8:	7f 01 1a    	clr	0x11a
    bbeb:	7e d5 27    	jmp	0xd527
    bbee:	ce 00 77    	ldx	#0x77
    bbf1:	bd bc 59    	jsr	0xbc59
    bbf4:	bd bc 8d    	jsr	0xbc8d
    bbf7:	16          	tab
    bbf8:	ce e0 4b    	ldx	#0xe04b
    bbfb:	18 ce 01 2b 	ldy	#0x12b
    bbff:	bd ec 3d    	jsr	0xec3d
    bc02:	86 04       	ldaa	#0x4
    bc04:	b7 01 1c    	staa	0x11c
    bc07:	7f 01 1a    	clr	0x11a
    bc0a:	7e d5 27    	jmp	0xd527
    bc0d:	81 19       	cmpa	#0x19
    bc0f:	26 24       	bne	0x0xbc35
    bc11:	ce 00 78    	ldx	#0x78
    bc14:	bd bc 59    	jsr	0xbc59
    bc17:	bd bc d9    	jsr	0xbcd9
    bc1a:	ce 01 37    	ldx	#0x137
    bc1d:	7d 00 f6    	tst	0xf6
    bc20:	2a 05       	bpl	0x0xbc27
    bc22:	bd eb c6    	jsr	0xebc6
    bc25:	20 03       	bra	0x0xbc2a
    bc27:	bd eb 8a    	jsr	0xeb8a
    bc2a:	86 03       	ldaa	#0x3
    bc2c:	b7 01 1c    	staa	0x11c
    bc2f:	7f 01 1a    	clr	0x11a
    bc32:	7e d5 27    	jmp	0xd527
    bc35:	ce 00 79    	ldx	#0x79
    bc38:	bd bc 59    	jsr	0xbc59
    bc3b:	bd bc d9    	jsr	0xbcd9
    bc3e:	ce 01 3c    	ldx	#0x13c
    bc41:	7d 00 f6    	tst	0xf6
    bc44:	2a 05       	bpl	0x0xbc4b
    bc46:	bd eb c6    	jsr	0xebc6
    bc49:	20 03       	bra	0x0xbc4e
    bc4b:	bd eb 8a    	jsr	0xeb8a
    bc4e:	86 03       	ldaa	#0x3
    bc50:	b7 01 1c    	staa	0x11c
    bc53:	7f 01 1a    	clr	0x11a
    bc56:	7e d5 27    	jmp	0xd527
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
    bcfa:	7e dd b9    	jmp	0xddb9
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
    bd16:	bd ea 76    	jsr	0xea76
    bd19:	7a 01 1e    	dec	0x11e
    bd1c:	7a 01 1c    	dec	0x11c
    bd1f:	26 12       	bne	0x0xbd33
    bd21:	bd ea f4    	jsr	0xeaf4
    bd24:	20 0d       	bra	0x0xbd33
    bd26:	7d 01 1e    	tst	0x11e
    bd29:	26 08       	bne	0x0xbd33
    bd2b:	86 19       	ldaa	#0x19
    bd2d:	b7 01 1e    	staa	0x11e
    bd30:	bd ea b2    	jsr	0xeab2
    bd33:	7d 01 7f    	tst	0x17f
    bd36:	27 10       	beq	0x0xbd48
    bd38:	bd ec 75    	jsr	0xec75
    bd3b:	4f          	clra
    bd3c:	b7 01 7f    	staa	0x17f
    bd3f:	b7 01 7e    	staa	0x17e
    bd42:	b7 01 1a    	staa	0x11a
    bd45:	7e d5 27    	jmp	0xd527
    bd48:	7d 01 1a    	tst	0x11a
    bd4b:	26 03       	bne	0x0xbd50
    bd4d:	7e d5 27    	jmp	0xd527
    bd50:	b6 01 1e    	ldaa	0x11e
    bd53:	81 19       	cmpa	#0x19
    bd55:	26 34       	bne	0x0xbd8b
    bd57:	18 ce 01 36 	ldy	#0x136
    bd5b:	ce de 1a    	ldx	#0xde1a
    bd5e:	7d 01 1a    	tst	0x11a
    bd61:	2b 20       	bmi	0x0xbd83
    bd63:	d6 8e       	ldab	*0x8e
    bd65:	5c          	incb
    bd66:	c1 03       	cmpb	#0x3
    bd68:	25 02       	bcs	0x0xbd6c
    bd6a:	c6 02       	ldab	#0x2
    bd6c:	d7 8e       	stab	*0x8e
    bd6e:	bd ec 3d    	jsr	0xec3d
    bd71:	7f 01 1a    	clr	0x11a
    bd74:	86 04       	ldaa	#0x4
    bd76:	b7 01 1c    	staa	0x11c
    bd79:	96 8e       	ldaa	*0x8e
    bd7b:	c6 8e       	ldab	#0x8e
    bd7d:	bd b0 fc    	jsr	0xb0fc
    bd80:	7e d5 27    	jmp	0xd527
    bd83:	d6 8e       	ldab	*0x8e
    bd85:	5a          	decb
    bd86:	2a e4       	bpl	0x0xbd6c
    bd88:	5f          	clrb
    bd89:	20 e1       	bra	0x0xbd6c
    bd8b:	18 ce 01 3b 	ldy	#0x13b
    bd8f:	ce de 26    	ldx	#0xde26
    bd92:	7d 01 1a    	tst	0x11a
    bd95:	2b 20       	bmi	0x0xbdb7
    bd97:	d6 8f       	ldab	*0x8f
    bd99:	5c          	incb
    bd9a:	c1 02       	cmpb	#0x2
    bd9c:	25 02       	bcs	0x0xbda0
    bd9e:	c6 01       	ldab	#0x1
    bda0:	d7 8f       	stab	*0x8f
    bda2:	bd ec 3d    	jsr	0xec3d
    bda5:	7f 01 1a    	clr	0x11a
    bda8:	86 04       	ldaa	#0x4
    bdaa:	b7 01 1c    	staa	0x11c
    bdad:	96 8f       	ldaa	*0x8f
    bdaf:	c6 8f       	ldab	#0x8f
    bdb1:	bd b0 fc    	jsr	0xb0fc
    bdb4:	7e d5 27    	jmp	0xd527
    bdb7:	d6 8f       	ldab	*0x8f
    bdb9:	5a          	decb
    bdba:	2a e4       	bpl	0x0xbda0
    bdbc:	5f          	clrb
    bdbd:	20 e1       	bra	0x0xbda0
    bdbf:	7d 01 1c    	tst	0x11c
    bdc2:	2a 03       	bpl	0x0xbdc7
    bdc4:	7e de 2e    	jmp	0xde2e
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
    bde0:	bd ea 76    	jsr	0xea76
    bde3:	7a 01 1e    	dec	0x11e
    bde6:	7a 01 1c    	dec	0x11c
    bde9:	26 08       	bne	0x0xbdf3
    bdeb:	86 11       	ldaa	#0x11
    bded:	b7 01 1e    	staa	0x11e
    bdf0:	bd ea b2    	jsr	0xeab2
    bdf3:	4f          	clra
    bdf4:	b7 01 7f    	staa	0x17f
    bdf7:	b7 01 7e    	staa	0x17e
    bdfa:	7d 01 1a    	tst	0x11a
    bdfd:	26 03       	bne	0x0xbe02
    bdff:	7e d5 27    	jmp	0xd527
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
    be2c:	7e d5 27    	jmp	0xd527
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
    be4a:	7e df 4c    	jmp	0xdf4c
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
    be66:	bd ea 76    	jsr	0xea76
    be69:	7a 01 1e    	dec	0x11e
    be6c:	7a 01 1c    	dec	0x11c
    be6f:	26 08       	bne	0x0xbe79
    be71:	86 1e       	ldaa	#0x1e
    be73:	b7 01 1e    	staa	0x11e
    be76:	bd ea b2    	jsr	0xeab2
    be79:	7f 01 7e    	clr	0x17e
    be7c:	7f 01 7f    	clr	0x17f
    be7f:	7d 01 1a    	tst	0x11a
    be82:	26 03       	bne	0x0xbe87
    be84:	7e d5 27    	jmp	0xd527
    be87:	b6 50 21    	ldaa	0x5021
    be8a:	7d 01 1a    	tst	0x11a
    be8d:	2b 0e       	bmi	0x0xbe9d
    be8f:	4c          	inca
    be90:	84 7f       	anda	#0x7f
    be92:	b7 50 21    	staa	0x5021
    be95:	ce 01 3c    	ldx	#0x13c
    be98:	bd eb 8a    	jsr	0xeb8a
    be9b:	20 03       	bra	0x0xbea0
    be9d:	4a          	deca
    be9e:	20 f0       	bra	0x0xbe90
    bea0:	7f 01 1a    	clr	0x11a
    bea3:	86 03       	ldaa	#0x3
    bea5:	b7 01 1c    	staa	0x11c
    bea8:	14 fb 80    	bset	*0xfb, #0x80
    beab:	7e d5 27    	jmp	0xd527
    beae:	7d 01 1c    	tst	0x11c
    beb1:	2a 03       	bpl	0x0xbeb6
    beb3:	7e df 8f    	jmp	0xdf8f
    beb6:	7d 01 1c    	tst	0x11c
    beb9:	27 03       	beq	0x0xbebe
    bebb:	7e b2 70    	jmp	0xb270
    bebe:	7d 01 1e    	tst	0x11e
    bec1:	26 08       	bne	0x0xbecb
    bec3:	86 14       	ldaa	#0x14
    bec5:	b7 01 1e    	staa	0x11e
    bec8:	bd ea b2    	jsr	0xeab2
    becb:	7e d5 27    	jmp	0xd527
    bece:	7d 01 1c    	tst	0x11c
    bed1:	2a 03       	bpl	0x0xbed6
    bed3:	7e df ea    	jmp	0xdfea
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
    beef:	bd ea 76    	jsr	0xea76
    bef2:	7a 01 1e    	dec	0x11e
    bef5:	7a 01 1c    	dec	0x11c
    bef8:	26 12       	bne	0x0xbf0c
    befa:	bd ea f4    	jsr	0xeaf4
    befd:	20 0d       	bra	0x0xbf0c
    beff:	7d 01 1e    	tst	0x11e
    bf02:	26 08       	bne	0x0xbf0c
    bf04:	86 03       	ldaa	#0x3
    bf06:	b7 01 1e    	staa	0x11e
    bf09:	bd ea b2    	jsr	0xeab2
    bf0c:	7d 01 7f    	tst	0x17f
    bf0f:	27 10       	beq	0x0xbf21
    bf11:	bd ec b6    	jsr	0xecb6
    bf14:	4f          	clra
    bf15:	b7 01 7f    	staa	0x17f
    bf18:	b7 01 7e    	staa	0x17e
    bf1b:	b7 01 1a    	staa	0x11a
    bf1e:	7e d5 27    	jmp	0xd527
    bf21:	7d 01 7e    	tst	0x17e
    bf24:	27 0c       	beq	0x0xbf32
    bf26:	bd ec 5e    	jsr	0xec5e
    bf29:	7f 01 7e    	clr	0x17e
    bf2c:	7f 01 1a    	clr	0x11a
    bf2f:	7e d5 27    	jmp	0xd527
    bf32:	7d 01 1a    	tst	0x11a
    bf35:	26 03       	bne	0x0xbf3a
    bf37:	7e d5 27    	jmp	0xd527
    bf3a:	ce e0 4b    	ldx	#0xe04b
    bf3d:	b6 01 1e    	ldaa	0x11e
    bf40:	81 03       	cmpa	#0x3
    bf42:	26 1a       	bne	0x0xbf5e
    bf44:	18 ce 01 20 	ldy	#0x120
    bf48:	d6 60       	ldab	*0x60
    bf4a:	bd c0 2c    	jsr	0xc02c
    bf4d:	d7 60       	stab	*0x60
    bf4f:	bd ec 3d    	jsr	0xec3d
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
    bf6d:	bd ec 3d    	jsr	0xec3d
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
    bf8b:	bd ec 3d    	jsr	0xec3d
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
    bfad:	bd eb 8a    	jsr	0xeb8a
    bfb0:	7f 01 1a    	clr	0x11a
    bfb3:	86 03       	ldaa	#0x3
    bfb5:	b7 01 1c    	staa	0x11c
    bfb8:	96 63       	ldaa	*0x63
    bfba:	c6 63       	ldab	#0x63
    bfbc:	bd b0 fc    	jsr	0xb0fc
    bfbf:	7e d5 27    	jmp	0xd527
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
    bfdc:	bd eb 8a    	jsr	0xeb8a
    bfdf:	7f 01 1a    	clr	0x11a
    bfe2:	86 03       	ldaa	#0x3
    bfe4:	b7 01 1c    	staa	0x11c
    bfe7:	96 64       	ldaa	*0x64
    bfe9:	c6 64       	ldab	#0x64
    bfeb:	bd b0 fc    	jsr	0xb0fc
    bfee:	7e d5 27    	jmp	0xd527
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
    c007:	bd eb 8a    	jsr	0xeb8a
    c00a:	7f 01 1a    	clr	0x11a
    c00d:	86 03       	ldaa	#0x3
    c00f:	b7 01 1c    	staa	0x11c
    c012:	96 65       	ldaa	*0x65
    c014:	c6 65       	ldab	#0x65
    c016:	bd b0 fc    	jsr	0xb0fc
    c019:	7e d5 27    	jmp	0xd527
    c01c:	96 65       	ldaa	*0x65
    c01e:	4a          	deca
    c01f:	84 7f       	anda	#0x7f
    c021:	20 df       	bra	0x0xc002
    c023:	b7 01 1c    	staa	0x11c
    c026:	7f 01 1a    	clr	0x11a
    c029:	7e d5 27    	jmp	0xd527
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
    c05d:	7e e0 af    	jmp	0xe0af
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
    c079:	bd ea 76    	jsr	0xea76
    c07c:	7a 01 1e    	dec	0x11e
    c07f:	7a 01 1c    	dec	0x11c
    c082:	26 12       	bne	0x0xc096
    c084:	bd ea f4    	jsr	0xeaf4
    c087:	20 0d       	bra	0x0xc096
    c089:	7d 01 1e    	tst	0x11e
    c08c:	26 08       	bne	0x0xc096
    c08e:	86 13       	ldaa	#0x13
    c090:	b7 01 1e    	staa	0x11e
    c093:	bd ea b2    	jsr	0xeab2
    c096:	7d 01 7f    	tst	0x17f
    c099:	27 10       	beq	0x0xc0ab
    c09b:	bd ec b6    	jsr	0xecb6
    c09e:	4f          	clra
    c09f:	b7 01 7f    	staa	0x17f
    c0a2:	b7 01 7e    	staa	0x17e
    c0a5:	b7 01 1a    	staa	0x11a
    c0a8:	7e d5 27    	jmp	0xd527
    c0ab:	7d 01 1a    	tst	0x11a
    c0ae:	26 03       	bne	0x0xc0b3
    c0b0:	7e d5 27    	jmp	0xd527
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
    c0c9:	bd eb 8a    	jsr	0xeb8a
    c0cc:	7f 01 1a    	clr	0x11a
    c0cf:	86 03       	ldaa	#0x3
    c0d1:	b7 01 1c    	staa	0x11c
    c0d4:	96 54       	ldaa	*0x54
    c0d6:	c6 54       	ldab	#0x54
    c0d8:	bd b0 fc    	jsr	0xb0fc
    c0db:	7e d5 27    	jmp	0xd527
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
    c0f8:	bd eb 8a    	jsr	0xeb8a
    c0fb:	7f 01 1a    	clr	0x11a
    c0fe:	86 03       	ldaa	#0x3
    c100:	b7 01 1c    	staa	0x11c
    c103:	96 59       	ldaa	*0x59
    c105:	c6 59       	ldab	#0x59
    c107:	bd b0 fc    	jsr	0xb0fc
    c10a:	7e d5 27    	jmp	0xd527
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
    c123:	bd eb 8a    	jsr	0xeb8a
    c126:	7f 01 1a    	clr	0x11a
    c129:	86 03       	ldaa	#0x3
    c12b:	b7 01 1c    	staa	0x11c
    c12e:	96 5e       	ldaa	*0x5e
    c130:	c6 5e       	ldab	#0x5e
    c132:	bd b0 fc    	jsr	0xb0fc
    c135:	7e d5 27    	jmp	0xd527
    c138:	96 5e       	ldaa	*0x5e
    c13a:	4a          	deca
    c13b:	84 7f       	anda	#0x7f
    c13d:	20 df       	bra	0x0xc11e
    c13f:	7d 01 1c    	tst	0x11c
    c142:	2a 03       	bpl	0x0xc147
    c144:	7e e1 02    	jmp	0xe102
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
    c160:	bd ea 76    	jsr	0xea76
    c163:	7a 01 1e    	dec	0x11e
    c166:	7a 01 1c    	dec	0x11c
    c169:	26 12       	bne	0x0xc17d
    c16b:	bd ea f4    	jsr	0xeaf4
    c16e:	20 0d       	bra	0x0xc17d
    c170:	7d 01 1e    	tst	0x11e
    c173:	26 08       	bne	0x0xc17d
    c175:	86 13       	ldaa	#0x13
    c177:	b7 01 1e    	staa	0x11e
    c17a:	bd ea b2    	jsr	0xeab2
    c17d:	7d 01 7f    	tst	0x17f
    c180:	27 10       	beq	0x0xc192
    c182:	bd ec b6    	jsr	0xecb6
    c185:	4f          	clra
    c186:	b7 01 7f    	staa	0x17f
    c189:	b7 01 7e    	staa	0x17e
    c18c:	b7 01 1a    	staa	0x11a
    c18f:	7e d5 27    	jmp	0xd527
    c192:	7d 01 1a    	tst	0x11a
    c195:	26 03       	bne	0x0xc19a
    c197:	7e d5 27    	jmp	0xd527
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
    c1b0:	bd eb 8a    	jsr	0xeb8a
    c1b3:	7f 01 1a    	clr	0x11a
    c1b6:	86 03       	ldaa	#0x3
    c1b8:	b7 01 1c    	staa	0x11c
    c1bb:	96 90       	ldaa	*0x90
    c1bd:	c6 90       	ldab	#0x90
    c1bf:	bd b0 fc    	jsr	0xb0fc
    c1c2:	7e d5 27    	jmp	0xd527
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
    c1df:	bd eb 8a    	jsr	0xeb8a
    c1e2:	7f 01 1a    	clr	0x11a
    c1e5:	86 03       	ldaa	#0x3
    c1e7:	b7 01 1c    	staa	0x11c
    c1ea:	96 91       	ldaa	*0x91
    c1ec:	c6 91       	ldab	#0x91
    c1ee:	bd b0 fc    	jsr	0xb0fc
    c1f1:	7e d5 27    	jmp	0xd527
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
    c20a:	bd eb 8a    	jsr	0xeb8a
    c20d:	7f 01 1a    	clr	0x11a
    c210:	86 03       	ldaa	#0x3
    c212:	b7 01 1c    	staa	0x11c
    c215:	96 92       	ldaa	*0x92
    c217:	c6 92       	ldab	#0x92
    c219:	bd b0 fc    	jsr	0xb0fc
    c21c:	7e d5 27    	jmp	0xd527
    c21f:	96 92       	ldaa	*0x92
    c221:	4a          	deca
    c222:	84 7f       	anda	#0x7f
    c224:	20 df       	bra	0x0xc205
    c226:	7d 01 1c    	tst	0x11c
    c229:	2a 03       	bpl	0x0xc22e
    c22b:	7e e1 9e    	jmp	0xe19e
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
    c247:	bd ea 76    	jsr	0xea76
    c24a:	7a 01 1e    	dec	0x11e
    c24d:	7a 01 1c    	dec	0x11c
    c250:	26 12       	bne	0x0xc264
    c252:	bd ea f4    	jsr	0xeaf4
    c255:	20 0d       	bra	0x0xc264
    c257:	7d 01 1e    	tst	0x11e
    c25a:	26 08       	bne	0x0xc264
    c25c:	86 13       	ldaa	#0x13
    c25e:	b7 01 1e    	staa	0x11e
    c261:	bd ea b2    	jsr	0xeab2
    c264:	7d 01 7f    	tst	0x17f
    c267:	27 1e       	beq	0x0xc287
    c269:	2a 10       	bpl	0x0xc27b
    c26b:	bd ec b6    	jsr	0xecb6
    c26e:	4f          	clra
    c26f:	b7 01 7f    	staa	0x17f
    c272:	b7 01 7e    	staa	0x17e
    c275:	b7 01 1a    	staa	0x11a
    c278:	7e d5 27    	jmp	0xd527
    c27b:	b6 01 1e    	ldaa	0x11e
    c27e:	81 19       	cmpa	#0x19
    c280:	27 ec       	beq	0x0xc26e
    c282:	bd ec b6    	jsr	0xecb6
    c285:	20 e7       	bra	0x0xc26e
    c287:	7d 01 1a    	tst	0x11a
    c28a:	26 03       	bne	0x0xc28f
    c28c:	7e d5 27    	jmp	0xd527
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
    c2a6:	ce e2 01    	ldx	#0xe201
    c2a9:	18 ce 01 30 	ldy	#0x130
    c2ad:	bd ec 3d    	jsr	0xec3d
    c2b0:	7f 01 1a    	clr	0x11a
    c2b3:	86 04       	ldaa	#0x4
    c2b5:	b7 01 1c    	staa	0x11c
    c2b8:	96 4b       	ldaa	*0x4b
    c2ba:	c6 4b       	ldab	#0x4b
    c2bc:	bd b0 fc    	jsr	0xb0fc
    c2bf:	7e d5 27    	jmp	0xd527
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
    c2de:	ce e2 1d    	ldx	#0xe21d
    c2e1:	18 ce 01 36 	ldy	#0x136
    c2e5:	bd ec 3d    	jsr	0xec3d
    c2e8:	7f 01 1a    	clr	0x11a
    c2eb:	86 04       	ldaa	#0x4
    c2ed:	b7 01 1c    	staa	0x11c
    c2f0:	96 66       	ldaa	*0x66
    c2f2:	c6 66       	ldab	#0x66
    c2f4:	bd b0 fc    	jsr	0xb0fc
    c2f7:	7e d5 27    	jmp	0xd527
    c2fa:	d6 66       	ldab	*0x66
    c2fc:	5a          	decb
    c2fd:	2a dd       	bpl	0x0xc2dc
    c2ff:	5f          	clrb
    c300:	20 da       	bra	0x0xc2dc
    c302:	4f          	clra
    c303:	b7 01 7e    	staa	0x17e
    c306:	b7 01 7f    	staa	0x17f
    c309:	b7 01 1a    	staa	0x11a
    c30c:	7e d5 27    	jmp	0xd527
    c30f:	7d 01 1c    	tst	0x11c
    c312:	2a 03       	bpl	0x0xc317
    c314:	7e e2 2d    	jmp	0xe22d
    c317:	7d 01 1e    	tst	0x11e
    c31a:	26 08       	bne	0x0xc324
    c31c:	7d 01 1c    	tst	0x11c
    c31f:	27 1f       	beq	0x0xc340
    c321:	7e b2 70    	jmp	0xb270
    c324:	7d 01 1c    	tst	0x11c
    c327:	27 24       	beq	0x0xc34d
    c329:	cc 01 20    	ldd	#0x120
    c32c:	fb 01 1e    	addb	0x11e
    c32f:	8f          	xgdx
    c330:	bd ea 76    	jsr	0xea76
    c333:	7a 01 1e    	dec	0x11e
    c336:	7a 01 1c    	dec	0x11c
    c339:	26 12       	bne	0x0xc34d
    c33b:	bd eb 27    	jsr	0xeb27
    c33e:	20 0d       	bra	0x0xc34d
    c340:	7d 01 1e    	tst	0x11e
    c343:	26 08       	bne	0x0xc34d
    c345:	86 12       	ldaa	#0x12
    c347:	b7 01 1e    	staa	0x11e
    c34a:	bd ea b2    	jsr	0xeab2
    c34d:	7d 01 7f    	tst	0x17f
    c350:	27 10       	beq	0x0xc362
    c352:	bd ed 28    	jsr	0xed28
    c355:	4f          	clra
    c356:	b7 01 7f    	staa	0x17f
    c359:	b7 01 7e    	staa	0x17e
    c35c:	b7 01 1a    	staa	0x11a
    c35f:	7e d5 27    	jmp	0xd527
    c362:	7d 01 1a    	tst	0x11a
    c365:	26 03       	bne	0x0xc36a
    c367:	7e d5 27    	jmp	0xd527
    c36a:	b6 01 1e    	ldaa	0x11e
    c36d:	81 12       	cmpa	#0x12
    c36f:	26 2b       	bne	0x0xc39c
    c371:	b6 01 1a    	ldaa	0x11a
    c374:	2b 1f       	bmi	0x0xc395
    c376:	96 6e       	ldaa	*0x6e
    c378:	4c          	inca
    c379:	84 7f       	anda	#0x7f
    c37b:	97 6e       	staa	*0x6e
    c37d:	ce 01 30    	ldx	#0x130
    c380:	bd eb 8a    	jsr	0xeb8a
    c383:	7f 01 1a    	clr	0x11a
    c386:	86 03       	ldaa	#0x3
    c388:	b7 01 1c    	staa	0x11c
    c38b:	96 6e       	ldaa	*0x6e
    c38d:	c6 6e       	ldab	#0x6e
    c38f:	bd b0 fc    	jsr	0xb0fc
    c392:	7e d5 27    	jmp	0xd527
    c395:	96 6e       	ldaa	*0x6e
    c397:	4a          	deca
    c398:	84 7f       	anda	#0x7f
    c39a:	20 df       	bra	0x0xc37b
    c39c:	81 16       	cmpa	#0x16
    c39e:	26 2b       	bne	0x0xc3cb
    c3a0:	b6 01 1a    	ldaa	0x11a
    c3a3:	2b 1f       	bmi	0x0xc3c4
    c3a5:	96 48       	ldaa	*0x48
    c3a7:	4c          	inca
    c3a8:	84 7f       	anda	#0x7f
    c3aa:	97 48       	staa	*0x48
    c3ac:	ce 01 34    	ldx	#0x134
    c3af:	bd eb 8a    	jsr	0xeb8a
    c3b2:	7f 01 1a    	clr	0x11a
    c3b5:	86 03       	ldaa	#0x3
    c3b7:	b7 01 1c    	staa	0x11c
    c3ba:	96 48       	ldaa	*0x48
    c3bc:	c6 48       	ldab	#0x48
    c3be:	bd b0 fc    	jsr	0xb0fc
    c3c1:	7e d5 27    	jmp	0xd527
    c3c4:	96 48       	ldaa	*0x48
    c3c6:	4a          	deca
    c3c7:	84 7f       	anda	#0x7f
    c3c9:	20 df       	bra	0x0xc3aa
    c3cb:	81 1a       	cmpa	#0x1a
    c3cd:	26 2b       	bne	0x0xc3fa
    c3cf:	b6 01 1a    	ldaa	0x11a
    c3d2:	2b 1f       	bmi	0x0xc3f3
    c3d4:	96 6f       	ldaa	*0x6f
    c3d6:	4c          	inca
    c3d7:	84 7f       	anda	#0x7f
    c3d9:	97 6f       	staa	*0x6f
    c3db:	ce 01 38    	ldx	#0x138
    c3de:	bd eb 8a    	jsr	0xeb8a
    c3e1:	7f 01 1a    	clr	0x11a
    c3e4:	86 03       	ldaa	#0x3
    c3e6:	b7 01 1c    	staa	0x11c
    c3e9:	96 6f       	ldaa	*0x6f
    c3eb:	c6 6f       	ldab	#0x6f
    c3ed:	bd b0 fc    	jsr	0xb0fc
    c3f0:	7e d5 27    	jmp	0xd527
    c3f3:	96 6f       	ldaa	*0x6f
    c3f5:	4a          	deca
    c3f6:	84 7f       	anda	#0x7f
    c3f8:	20 df       	bra	0x0xc3d9
    c3fa:	b6 01 1a    	ldaa	0x11a
    c3fd:	2b 21       	bmi	0x0xc420
    c3ff:	96 70       	ldaa	*0x70
    c401:	4c          	inca
    c402:	2a 02       	bpl	0x0xc406
    c404:	86 7f       	ldaa	#0x7f
    c406:	97 70       	staa	*0x70
    c408:	ce 01 3c    	ldx	#0x13c
    c40b:	bd ec 51    	jsr	0xec51
    c40e:	7f 01 1a    	clr	0x11a
    c411:	86 03       	ldaa	#0x3
    c413:	b7 01 1c    	staa	0x11c
    c416:	96 70       	ldaa	*0x70
    c418:	c6 70       	ldab	#0x70
    c41a:	bd b0 fc    	jsr	0xb0fc
    c41d:	7e d5 27    	jmp	0xd527
    c420:	96 70       	ldaa	*0x70
    c422:	4a          	deca
    c423:	2a e1       	bpl	0x0xc406
    c425:	4f          	clra
    c426:	20 de       	bra	0x0xc406
    c428:	7d 01 1c    	tst	0x11c
    c42b:	2a 03       	bpl	0x0xc430
    c42d:	7e e2 84    	jmp	0xe284
    c430:	7d 01 1e    	tst	0x11e
    c433:	26 08       	bne	0x0xc43d
    c435:	7d 01 1c    	tst	0x11c
    c438:	27 1f       	beq	0x0xc459
    c43a:	7e b2 70    	jmp	0xb270
    c43d:	7d 01 1c    	tst	0x11c
    c440:	27 24       	beq	0x0xc466
    c442:	cc 01 20    	ldd	#0x120
    c445:	fb 01 1e    	addb	0x11e
    c448:	8f          	xgdx
    c449:	bd ea 76    	jsr	0xea76
    c44c:	7a 01 1e    	dec	0x11e
    c44f:	7a 01 1c    	dec	0x11c
    c452:	26 12       	bne	0x0xc466
    c454:	bd ea f4    	jsr	0xeaf4
    c457:	20 0d       	bra	0x0xc466
    c459:	7d 01 1e    	tst	0x11e
    c45c:	26 08       	bne	0x0xc466
    c45e:	86 13       	ldaa	#0x13
    c460:	b7 01 1e    	staa	0x11e
    c463:	bd ea b2    	jsr	0xeab2
    c466:	7d 01 7f    	tst	0x17f
    c469:	27 1e       	beq	0x0xc489
    c46b:	2b 17       	bmi	0x0xc484
    c46d:	b6 01 1e    	ldaa	0x11e
    c470:	81 19       	cmpa	#0x19
    c472:	27 03       	beq	0x0xc477
    c474:	bd ec b6    	jsr	0xecb6
    c477:	4f          	clra
    c478:	b7 01 7f    	staa	0x17f
    c47b:	b7 01 7e    	staa	0x17e
    c47e:	b7 01 1a    	staa	0x11a
    c481:	7e d5 27    	jmp	0xd527
    c484:	bd ec b6    	jsr	0xecb6
    c487:	20 ee       	bra	0x0xc477
    c489:	7d 01 1a    	tst	0x11a
    c48c:	26 03       	bne	0x0xc491
    c48e:	7e d5 27    	jmp	0xd527
    c491:	b6 01 1e    	ldaa	0x11e
    c494:	81 13       	cmpa	#0x13
    c496:	26 30       	bne	0x0xc4c8
    c498:	7d 01 1a    	tst	0x11a
    c49b:	2b 23       	bmi	0x0xc4c0
    c49d:	d6 74       	ldab	*0x74
    c49f:	5c          	incb
    c4a0:	c1 05       	cmpb	#0x5
    c4a2:	25 02       	bcs	0x0xc4a6
    c4a4:	c6 04       	ldab	#0x4
    c4a6:	d7 74       	stab	*0x74
    c4a8:	18 ce 01 30 	ldy	#0x130
    c4ac:	ce e2 e7    	ldx	#0xe2e7
    c4af:	bd ec 3d    	jsr	0xec3d
    c4b2:	86 04       	ldaa	#0x4
    c4b4:	b7 01 1c    	staa	0x11c
    c4b7:	7f 01 1a    	clr	0x11a
    c4ba:	7f 01 7e    	clr	0x17e
    c4bd:	7e d5 27    	jmp	0xd527
    c4c0:	d6 74       	ldab	*0x74
    c4c2:	5a          	decb
    c4c3:	2a e1       	bpl	0x0xc4a6
    c4c5:	5f          	clrb
    c4c6:	20 de       	bra	0x0xc4a6
    c4c8:	d6 72       	ldab	*0x72
    c4ca:	7d 01 1a    	tst	0x11a
    c4cd:	2b 23       	bmi	0x0xc4f2
    c4cf:	5c          	incb
    c4d0:	c1 0d       	cmpb	#0xd
    c4d2:	25 02       	bcs	0x0xc4d6
    c4d4:	c6 0c       	ldab	#0xc
    c4d6:	d7 72       	stab	*0x72
    c4d8:	d7 dc       	stab	*0xdc
    c4da:	18 ce 01 36 	ldy	#0x136
    c4de:	ce e4 88    	ldx	#0xe488
    c4e1:	bd ec 3d    	jsr	0xec3d
    c4e4:	86 04       	ldaa	#0x4
    c4e6:	b7 01 1c    	staa	0x11c
    c4e9:	7f 01 1a    	clr	0x11a
    c4ec:	7f 01 7e    	clr	0x17e
    c4ef:	7e d5 27    	jmp	0xd527
    c4f2:	5a          	decb
    c4f3:	2a e1       	bpl	0x0xc4d6
    c4f5:	5f          	clrb
    c4f6:	20 de       	bra	0x0xc4d6
    c4f8:	7d 01 1c    	tst	0x11c
    c4fb:	2a 03       	bpl	0x0xc500
    c4fd:	7e e2 fb    	jmp	0xe2fb
    c500:	7d 01 1e    	tst	0x11e
    c503:	26 08       	bne	0x0xc50d
    c505:	7d 01 1c    	tst	0x11c
    c508:	27 1f       	beq	0x0xc529
    c50a:	7e b2 70    	jmp	0xb270
    c50d:	7d 01 1c    	tst	0x11c
    c510:	27 24       	beq	0x0xc536
    c512:	cc 01 20    	ldd	#0x120
    c515:	fb 01 1e    	addb	0x11e
    c518:	8f          	xgdx
    c519:	bd ea 76    	jsr	0xea76
    c51c:	7a 01 1e    	dec	0x11e
    c51f:	7a 01 1c    	dec	0x11c
    c522:	26 12       	bne	0x0xc536
    c524:	bd ea f4    	jsr	0xeaf4
    c527:	20 0d       	bra	0x0xc536
    c529:	7d 01 1e    	tst	0x11e
    c52c:	26 08       	bne	0x0xc536
    c52e:	86 03       	ldaa	#0x3
    c530:	b7 01 1e    	staa	0x11e
    c533:	bd ea b2    	jsr	0xeab2
    c536:	7d 01 7f    	tst	0x17f
    c539:	27 10       	beq	0x0xc54b
    c53b:	bd ec b6    	jsr	0xecb6
    c53e:	4f          	clra
    c53f:	b7 01 7f    	staa	0x17f
    c542:	b7 01 7e    	staa	0x17e
    c545:	b7 01 1a    	staa	0x11a
    c548:	7e d5 27    	jmp	0xd527
    c54b:	7d 01 7e    	tst	0x17e
    c54e:	27 0c       	beq	0x0xc55c
    c550:	bd ec 5e    	jsr	0xec5e
    c553:	7f 01 7e    	clr	0x17e
    c556:	7f 01 1a    	clr	0x11a
    c559:	7e d5 27    	jmp	0xd527
    c55c:	7d 01 1a    	tst	0x11a
    c55f:	26 03       	bne	0x0xc564
    c561:	7e d5 27    	jmp	0xd527
    c564:	ce e3 9f    	ldx	#0xe39f
    c567:	b6 01 1e    	ldaa	0x11e
    c56a:	81 03       	cmpa	#0x3
    c56c:	26 34       	bne	0x0xc5a2
    c56e:	18 ce 01 20 	ldy	#0x120
    c572:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc58c
    c576:	d6 2d       	ldab	*0x2d
    c578:	bd c6 12    	jsr	0xc612
    c57b:	d7 2d       	stab	*0x2d
    c57d:	bd ec 3d    	jsr	0xec3d
    c580:	96 2d       	ldaa	*0x2d
    c582:	c6 2d       	ldab	#0x2d
    c584:	bd b0 fc    	jsr	0xb0fc
    c587:	86 04       	ldaa	#0x4
    c589:	7e c6 bf    	jmp	0xc6bf
    c58c:	d6 3a       	ldab	*0x3a
    c58e:	bd c6 12    	jsr	0xc612
    c591:	d7 3a       	stab	*0x3a
    c593:	bd ec 3d    	jsr	0xec3d
    c596:	96 3a       	ldaa	*0x3a
    c598:	c6 3a       	ldab	#0x3a
    c59a:	bd b0 fc    	jsr	0xb0fc
    c59d:	86 04       	ldaa	#0x4
    c59f:	7e c6 bf    	jmp	0xc6bf
    c5a2:	81 09       	cmpa	#0x9
    c5a4:	26 34       	bne	0x0xc5da
    c5a6:	18 ce 01 26 	ldy	#0x126
    c5aa:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc5c4
    c5ae:	d6 2e       	ldab	*0x2e
    c5b0:	bd c6 12    	jsr	0xc612
    c5b3:	d7 2e       	stab	*0x2e
    c5b5:	bd ec 3d    	jsr	0xec3d
    c5b8:	96 2e       	ldaa	*0x2e
    c5ba:	c6 2e       	ldab	#0x2e
    c5bc:	bd b0 fc    	jsr	0xb0fc
    c5bf:	86 04       	ldaa	#0x4
    c5c1:	7e c6 bf    	jmp	0xc6bf
    c5c4:	d6 3b       	ldab	*0x3b
    c5c6:	bd c6 12    	jsr	0xc612
    c5c9:	d7 3b       	stab	*0x3b
    c5cb:	bd ec 3d    	jsr	0xec3d
    c5ce:	96 3b       	ldaa	*0x3b
    c5d0:	c6 3b       	ldab	#0x3b
    c5d2:	bd b0 fc    	jsr	0xb0fc
    c5d5:	86 04       	ldaa	#0x4
    c5d7:	7e c6 bf    	jmp	0xc6bf
    c5da:	81 0e       	cmpa	#0xe
    c5dc:	26 46       	bne	0x0xc624
    c5de:	18 ce 01 2b 	ldy	#0x12b
    c5e2:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc5fc
    c5e6:	d6 2f       	ldab	*0x2f
    c5e8:	bd c6 12    	jsr	0xc612
    c5eb:	d7 2f       	stab	*0x2f
    c5ed:	bd ec 3d    	jsr	0xec3d
    c5f0:	96 2f       	ldaa	*0x2f
    c5f2:	c6 2f       	ldab	#0x2f
    c5f4:	bd b0 fc    	jsr	0xb0fc
    c5f7:	86 04       	ldaa	#0x4
    c5f9:	7e c6 bf    	jmp	0xc6bf
    c5fc:	d6 3c       	ldab	*0x3c
    c5fe:	bd c6 12    	jsr	0xc612
    c601:	d7 3c       	stab	*0x3c
    c603:	bd ec 3d    	jsr	0xec3d
    c606:	96 3c       	ldaa	*0x3c
    c608:	c6 3c       	ldab	#0x3c
    c60a:	bd b0 fc    	jsr	0xb0fc
    c60d:	86 04       	ldaa	#0x4
    c60f:	7e c6 bf    	jmp	0xc6bf
    c612:	7d 01 1a    	tst	0x11a
    c615:	2b 08       	bmi	0x0xc61f
    c617:	5c          	incb
    c618:	c1 11       	cmpb	#0x11
    c61a:	25 02       	bcs	0x0xc61e
    c61c:	c6 10       	ldab	#0x10
    c61e:	39          	rts
    c61f:	5a          	decb
    c620:	2a fc       	bpl	0x0xc61e
    c622:	5f          	clrb
    c623:	39          	rts
    c624:	81 13       	cmpa	#0x13
    c626:	26 33       	bne	0x0xc65b
    c628:	ce 01 31    	ldx	#0x131
    c62b:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc645
    c62f:	96 2a       	ldaa	*0x2a
    c631:	bd c6 c8    	jsr	0xc6c8
    c634:	97 2a       	staa	*0x2a
    c636:	bd eb 8a    	jsr	0xeb8a
    c639:	96 2a       	ldaa	*0x2a
    c63b:	c6 2a       	ldab	#0x2a
    c63d:	bd b0 fc    	jsr	0xb0fc
    c640:	86 03       	ldaa	#0x3
    c642:	7e c6 bf    	jmp	0xc6bf
    c645:	96 37       	ldaa	*0x37
    c647:	bd c6 c8    	jsr	0xc6c8
    c64a:	97 37       	staa	*0x37
    c64c:	bd eb 8a    	jsr	0xeb8a
    c64f:	96 37       	ldaa	*0x37
    c651:	c6 37       	ldab	#0x37
    c653:	bd b0 fc    	jsr	0xb0fc
    c656:	86 03       	ldaa	#0x3
    c658:	7e c6 bf    	jmp	0xc6bf
    c65b:	81 19       	cmpa	#0x19
    c65d:	26 31       	bne	0x0xc690
    c65f:	ce 01 37    	ldx	#0x137
    c662:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc67b
    c666:	96 2b       	ldaa	*0x2b
    c668:	bd c6 c8    	jsr	0xc6c8
    c66b:	97 2b       	staa	*0x2b
    c66d:	bd eb 8a    	jsr	0xeb8a
    c670:	96 2b       	ldaa	*0x2b
    c672:	c6 2b       	ldab	#0x2b
    c674:	bd b0 fc    	jsr	0xb0fc
    c677:	86 03       	ldaa	#0x3
    c679:	20 44       	bra	0x0xc6bf
    c67b:	96 38       	ldaa	*0x38
    c67d:	bd c6 c8    	jsr	0xc6c8
    c680:	97 38       	staa	*0x38
    c682:	bd eb 8a    	jsr	0xeb8a
    c685:	96 38       	ldaa	*0x38
    c687:	c6 38       	ldab	#0x38
    c689:	bd b0 fc    	jsr	0xb0fc
    c68c:	86 03       	ldaa	#0x3
    c68e:	20 2f       	bra	0x0xc6bf
    c690:	ce 01 3c    	ldx	#0x13c
    c693:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc6ac
    c697:	96 2c       	ldaa	*0x2c
    c699:	bd c6 c8    	jsr	0xc6c8
    c69c:	97 2c       	staa	*0x2c
    c69e:	bd eb 8a    	jsr	0xeb8a
    c6a1:	96 2c       	ldaa	*0x2c
    c6a3:	c6 2c       	ldab	#0x2c
    c6a5:	bd b0 fc    	jsr	0xb0fc
    c6a8:	86 03       	ldaa	#0x3
    c6aa:	20 13       	bra	0x0xc6bf
    c6ac:	96 39       	ldaa	*0x39
    c6ae:	bd c6 c8    	jsr	0xc6c8
    c6b1:	97 39       	staa	*0x39
    c6b3:	bd eb 8a    	jsr	0xeb8a
    c6b6:	96 39       	ldaa	*0x39
    c6b8:	c6 39       	ldab	#0x39
    c6ba:	bd b0 fc    	jsr	0xb0fc
    c6bd:	86 03       	ldaa	#0x3
    c6bf:	b7 01 1c    	staa	0x11c
    c6c2:	7f 01 1a    	clr	0x11a
    c6c5:	7e d5 27    	jmp	0xd527
    c6c8:	7d 01 1a    	tst	0x11a
    c6cb:	2b 04       	bmi	0x0xc6d1
    c6cd:	4c          	inca
    c6ce:	84 7f       	anda	#0x7f
    c6d0:	39          	rts
    c6d1:	4a          	deca
    c6d2:	84 7f       	anda	#0x7f
    c6d4:	20 fa       	bra	0x0xc6d0
    c6d6:	7d 01 1c    	tst	0x11c
    c6d9:	2a 03       	bpl	0x0xc6de
    c6db:	7e e3 e3    	jmp	0xe3e3
    c6de:	7d 01 1e    	tst	0x11e
    c6e1:	26 08       	bne	0x0xc6eb
    c6e3:	7d 01 1c    	tst	0x11c
    c6e6:	27 1f       	beq	0x0xc707
    c6e8:	7e b2 70    	jmp	0xb270
    c6eb:	7d 01 1c    	tst	0x11c
    c6ee:	27 24       	beq	0x0xc714
    c6f0:	cc 01 20    	ldd	#0x120
    c6f3:	fb 01 1e    	addb	0x11e
    c6f6:	8f          	xgdx
    c6f7:	bd ea 76    	jsr	0xea76
    c6fa:	7a 01 1e    	dec	0x11e
    c6fd:	7a 01 1c    	dec	0x11c
    c700:	26 12       	bne	0x0xc714
    c702:	bd ea f4    	jsr	0xeaf4
    c705:	20 0d       	bra	0x0xc714
    c707:	7d 01 1e    	tst	0x11e
    c70a:	26 08       	bne	0x0xc714
    c70c:	86 13       	ldaa	#0x13
    c70e:	b7 01 1e    	staa	0x11e
    c711:	bd ea b2    	jsr	0xeab2
    c714:	7d 01 7f    	tst	0x17f
    c717:	27 10       	beq	0x0xc729
    c719:	bd ec b6    	jsr	0xecb6
    c71c:	4f          	clra
    c71d:	b7 01 7f    	staa	0x17f
    c720:	b7 01 7e    	staa	0x17e
    c723:	b7 01 1a    	staa	0x11a
    c726:	7e d5 27    	jmp	0xd527
    c729:	7d 01 1a    	tst	0x11a
    c72c:	26 03       	bne	0x0xc731
    c72e:	7e d5 27    	jmp	0xd527
    c731:	b6 01 1e    	ldaa	0x11e
    c734:	81 13       	cmpa	#0x13
    c736:	26 53       	bne	0x0xc78b
    c738:	ce e4 68    	ldx	#0xe468
    c73b:	18 ce 01 30 	ldy	#0x130
    c73f:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc770
    c743:	d6 27       	ldab	*0x27
    c745:	8d 17       	bsr	0x0xc75e
    c747:	d7 27       	stab	*0x27
    c749:	bd ec 3d    	jsr	0xec3d
    c74c:	7f 01 1a    	clr	0x11a
    c74f:	86 04       	ldaa	#0x4
    c751:	b7 01 1c    	staa	0x11c
    c754:	96 27       	ldaa	*0x27
    c756:	c6 27       	ldab	#0x27
    c758:	bd b0 fc    	jsr	0xb0fc
    c75b:	7e d5 27    	jmp	0xd527
    c75e:	7d 01 1a    	tst	0x11a
    c761:	2b 08       	bmi	0x0xc76b
    c763:	5c          	incb
    c764:	c1 06       	cmpb	#0x6
    c766:	25 02       	bcs	0x0xc76a
    c768:	c6 05       	ldab	#0x5
    c76a:	39          	rts
    c76b:	5a          	decb
    c76c:	2a fc       	bpl	0x0xc76a
    c76e:	5f          	clrb
    c76f:	39          	rts
    c770:	d6 34       	ldab	*0x34
    c772:	8d ea       	bsr	0x0xc75e
    c774:	d7 34       	stab	*0x34
    c776:	bd ec 3d    	jsr	0xec3d
    c779:	7f 01 1a    	clr	0x11a
    c77c:	86 04       	ldaa	#0x4
    c77e:	b7 01 1c    	staa	0x11c
    c781:	96 34       	ldaa	*0x34
    c783:	c6 34       	ldab	#0x34
    c785:	bd b0 fc    	jsr	0xb0fc
    c788:	7e d5 27    	jmp	0xd527
    c78b:	81 19       	cmpa	#0x19
    c78d:	26 23       	bne	0x0xc7b2
    c78f:	ce e4 80    	ldx	#0xe480
    c792:	18 ce 01 36 	ldy	#0x136
    c796:	d6 31       	ldab	*0x31
    c798:	5c          	incb
    c799:	c4 01       	andb	#0x1
    c79b:	d7 31       	stab	*0x31
    c79d:	bd ec 3d    	jsr	0xec3d
    c7a0:	7f 01 1a    	clr	0x11a
    c7a3:	86 04       	ldaa	#0x4
    c7a5:	b7 01 1c    	staa	0x11c
    c7a8:	96 31       	ldaa	*0x31
    c7aa:	c6 31       	ldab	#0x31
    c7ac:	bd b0 fc    	jsr	0xb0fc
    c7af:	7e d5 27    	jmp	0xd527
    c7b2:	ce e4 88    	ldx	#0xe488
    c7b5:	18 ce 01 3b 	ldy	#0x13b
    c7b9:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc7ea
    c7bd:	d6 25       	ldab	*0x25
    c7bf:	8d 17       	bsr	0x0xc7d8
    c7c1:	d7 25       	stab	*0x25
    c7c3:	bd ec 3d    	jsr	0xec3d
    c7c6:	7f 01 1a    	clr	0x11a
    c7c9:	86 04       	ldaa	#0x4
    c7cb:	b7 01 1c    	staa	0x11c
    c7ce:	96 25       	ldaa	*0x25
    c7d0:	c6 25       	ldab	#0x25
    c7d2:	bd b0 fc    	jsr	0xb0fc
    c7d5:	7e d5 27    	jmp	0xd527
    c7d8:	7d 01 1a    	tst	0x11a
    c7db:	2b 08       	bmi	0x0xc7e5
    c7dd:	5c          	incb
    c7de:	c1 0d       	cmpb	#0xd
    c7e0:	25 02       	bcs	0x0xc7e4
    c7e2:	c6 0c       	ldab	#0xc
    c7e4:	39          	rts
    c7e5:	5a          	decb
    c7e6:	2a fc       	bpl	0x0xc7e4
    c7e8:	5f          	clrb
    c7e9:	39          	rts
    c7ea:	d6 32       	ldab	*0x32
    c7ec:	8d ea       	bsr	0x0xc7d8
    c7ee:	d7 32       	stab	*0x32
    c7f0:	bd ec 3d    	jsr	0xec3d
    c7f3:	7f 01 1a    	clr	0x11a
    c7f6:	86 04       	ldaa	#0x4
    c7f8:	b7 01 1c    	staa	0x11c
    c7fb:	96 32       	ldaa	*0x32
    c7fd:	c6 32       	ldab	#0x32
    c7ff:	bd b0 fc    	jsr	0xb0fc
    c802:	7e d5 27    	jmp	0xd527
    c805:	7d 01 1c    	tst	0x11c
    c808:	2a 03       	bpl	0x0xc80d
    c80a:	7e e4 bc    	jmp	0xe4bc
    c80d:	7d 01 1e    	tst	0x11e
    c810:	26 08       	bne	0x0xc81a
    c812:	7d 01 1c    	tst	0x11c
    c815:	27 1f       	beq	0x0xc836
    c817:	7e b2 70    	jmp	0xb270
    c81a:	7d 01 1c    	tst	0x11c
    c81d:	27 24       	beq	0x0xc843
    c81f:	cc 01 20    	ldd	#0x120
    c822:	fb 01 1e    	addb	0x11e
    c825:	8f          	xgdx
    c826:	bd ea 76    	jsr	0xea76
    c829:	7a 01 1e    	dec	0x11e
    c82c:	7a 01 1c    	dec	0x11c
    c82f:	26 12       	bne	0x0xc843
    c831:	bd ea f4    	jsr	0xeaf4
    c834:	20 0d       	bra	0x0xc843
    c836:	7d 01 1e    	tst	0x11e
    c839:	26 08       	bne	0x0xc843
    c83b:	86 19       	ldaa	#0x19
    c83d:	b7 01 1e    	staa	0x11e
    c840:	bd ea b2    	jsr	0xeab2
    c843:	7d 01 7f    	tst	0x17f
    c846:	27 1e       	beq	0x0xc866
    c848:	2b 10       	bmi	0x0xc85a
    c84a:	bd ec b6    	jsr	0xecb6
    c84d:	4f          	clra
    c84e:	b7 01 7f    	staa	0x17f
    c851:	b7 01 7e    	staa	0x17e
    c854:	b7 01 1a    	staa	0x11a
    c857:	7e d5 27    	jmp	0xd527
    c85a:	b6 01 1e    	ldaa	0x11e
    c85d:	81 19       	cmpa	#0x19
    c85f:	27 ec       	beq	0x0xc84d
    c861:	bd ec b6    	jsr	0xecb6
    c864:	20 e7       	bra	0x0xc84d
    c866:	7d 01 1a    	tst	0x11a
    c869:	26 03       	bne	0x0xc86e
    c86b:	7e d5 27    	jmp	0xd527
    c86e:	b6 01 1e    	ldaa	0x11e
    c871:	81 13       	cmpa	#0x13
    c873:	26 5a       	bne	0x0xc8cf
    c875:	12 f4 02 2b 	brset	*0xf4, #0x02, 0x0xc8a4
    c879:	7d 01 1a    	tst	0x11a
    c87c:	2b 1f       	bmi	0x0xc89d
    c87e:	96 29       	ldaa	*0x29
    c880:	4c          	inca
    c881:	84 7f       	anda	#0x7f
    c883:	97 29       	staa	*0x29
    c885:	ce 01 31    	ldx	#0x131
    c888:	bd eb 8a    	jsr	0xeb8a
    c88b:	7f 01 1a    	clr	0x11a
    c88e:	86 03       	ldaa	#0x3
    c890:	b7 01 1c    	staa	0x11c
    c893:	96 29       	ldaa	*0x29
    c895:	c6 29       	ldab	#0x29
    c897:	bd b0 fc    	jsr	0xb0fc
    c89a:	7e d5 27    	jmp	0xd527
    c89d:	96 29       	ldaa	*0x29
    c89f:	4a          	deca
    c8a0:	84 7f       	anda	#0x7f
    c8a2:	20 df       	bra	0x0xc883
    c8a4:	7d 01 1a    	tst	0x11a
    c8a7:	2b 1f       	bmi	0x0xc8c8
    c8a9:	96 36       	ldaa	*0x36
    c8ab:	4c          	inca
    c8ac:	84 7f       	anda	#0x7f
    c8ae:	97 36       	staa	*0x36
    c8b0:	ce 01 31    	ldx	#0x131
    c8b3:	bd eb 8a    	jsr	0xeb8a
    c8b6:	7f 01 1a    	clr	0x11a
    c8b9:	86 03       	ldaa	#0x3
    c8bb:	b7 01 1c    	staa	0x11c
    c8be:	96 36       	ldaa	*0x36
    c8c0:	c6 36       	ldab	#0x36
    c8c2:	bd b0 fc    	jsr	0xb0fc
    c8c5:	7e d5 27    	jmp	0xd527
    c8c8:	96 36       	ldaa	*0x36
    c8ca:	4a          	deca
    c8cb:	84 7f       	anda	#0x7f
    c8cd:	20 df       	bra	0x0xc8ae
    c8cf:	81 19       	cmpa	#0x19
    c8d1:	26 65       	bne	0x0xc938
    c8d3:	ce e5 22    	ldx	#0xe522
    c8d6:	18 ce 01 37 	ldy	#0x137
    c8da:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc90b
    c8de:	7d 01 1a    	tst	0x11a
    c8e1:	2b 20       	bmi	0x0xc903
    c8e3:	d6 26       	ldab	*0x26
    c8e5:	5c          	incb
    c8e6:	c1 03       	cmpb	#0x3
    c8e8:	25 02       	bcs	0x0xc8ec
    c8ea:	c6 02       	ldab	#0x2
    c8ec:	d7 26       	stab	*0x26
    c8ee:	bd ec 29    	jsr	0xec29
    c8f1:	7f 01 1a    	clr	0x11a
    c8f4:	86 03       	ldaa	#0x3
    c8f6:	b7 01 1c    	staa	0x11c
    c8f9:	96 26       	ldaa	*0x26
    c8fb:	c6 26       	ldab	#0x26
    c8fd:	bd b0 fc    	jsr	0xb0fc
    c900:	7e d5 27    	jmp	0xd527
    c903:	d6 26       	ldab	*0x26
    c905:	5a          	decb
    c906:	2a e4       	bpl	0x0xc8ec
    c908:	5f          	clrb
    c909:	20 e1       	bra	0x0xc8ec
    c90b:	7d 01 1a    	tst	0x11a
    c90e:	2b 20       	bmi	0x0xc930
    c910:	d6 33       	ldab	*0x33
    c912:	5c          	incb
    c913:	c1 03       	cmpb	#0x3
    c915:	25 02       	bcs	0x0xc919
    c917:	c6 02       	ldab	#0x2
    c919:	d7 33       	stab	*0x33
    c91b:	bd ec 29    	jsr	0xec29
    c91e:	7f 01 1a    	clr	0x11a
    c921:	86 03       	ldaa	#0x3
    c923:	b7 01 1c    	staa	0x11c
    c926:	96 33       	ldaa	*0x33
    c928:	c6 33       	ldab	#0x33
    c92a:	bd b0 fc    	jsr	0xb0fc
    c92d:	7e d5 27    	jmp	0xd527
    c930:	d6 33       	ldab	*0x33
    c932:	5a          	decb
    c933:	2a e4       	bpl	0x0xc919
    c935:	5f          	clrb
    c936:	20 e1       	bra	0x0xc919
    c938:	ce e5 31    	ldx	#0xe531
    c93b:	18 ce 01 3c 	ldy	#0x13c
    c93f:	7d 01 1a    	tst	0x11a
    c942:	2b 20       	bmi	0x0xc964
    c944:	d6 96       	ldab	*0x96
    c946:	5c          	incb
    c947:	c1 04       	cmpb	#0x4
    c949:	25 02       	bcs	0x0xc94d
    c94b:	c6 03       	ldab	#0x3
    c94d:	d7 96       	stab	*0x96
    c94f:	bd ec 29    	jsr	0xec29
    c952:	7f 01 1a    	clr	0x11a
    c955:	86 03       	ldaa	#0x3
    c957:	b7 01 1c    	staa	0x11c
    c95a:	96 96       	ldaa	*0x96
    c95c:	c6 96       	ldab	#0x96
    c95e:	bd b0 fc    	jsr	0xb0fc
    c961:	7e d5 27    	jmp	0xd527
    c964:	d6 96       	ldab	*0x96
    c966:	5a          	decb
    c967:	2a e4       	bpl	0x0xc94d
    c969:	5f          	clrb
    c96a:	20 e1       	bra	0x0xc94d
    c96c:	7e d5 27    	jmp	0xd527
    c96f:	7d 01 1c    	tst	0x11c
    c972:	2a 03       	bpl	0x0xc977
    c974:	7e e5 3d    	jmp	0xe53d
    c977:	7d 01 1e    	tst	0x11e
    c97a:	26 08       	bne	0x0xc984
    c97c:	7d 01 1c    	tst	0x11c
    c97f:	27 1f       	beq	0x0xc9a0
    c981:	7e b2 70    	jmp	0xb270
    c984:	7d 01 1c    	tst	0x11c
    c987:	27 24       	beq	0x0xc9ad
    c989:	cc 01 20    	ldd	#0x120
    c98c:	fb 01 1e    	addb	0x11e
    c98f:	8f          	xgdx
    c990:	bd ea 76    	jsr	0xea76
    c993:	7a 01 1e    	dec	0x11e
    c996:	7a 01 1c    	dec	0x11c
    c999:	26 12       	bne	0x0xc9ad
    c99b:	bd eb 27    	jsr	0xeb27
    c99e:	20 0d       	bra	0x0xc9ad
    c9a0:	7d 01 1e    	tst	0x11e
    c9a3:	26 08       	bne	0x0xc9ad
    c9a5:	86 02       	ldaa	#0x2
    c9a7:	b7 01 1e    	staa	0x11e
    c9aa:	bd ea b2    	jsr	0xeab2
    c9ad:	7d 01 7f    	tst	0x17f
    c9b0:	27 10       	beq	0x0xc9c2
    c9b2:	bd ed 28    	jsr	0xed28
    c9b5:	4f          	clra
    c9b6:	b7 01 7f    	staa	0x17f
    c9b9:	b7 01 7e    	staa	0x17e
    c9bc:	b7 01 1a    	staa	0x11a
    c9bf:	7e d5 27    	jmp	0xd527
    c9c2:	7d 01 7e    	tst	0x17e
    c9c5:	27 0c       	beq	0x0xc9d3
    c9c7:	bd ec 5e    	jsr	0xec5e
    c9ca:	7f 01 7e    	clr	0x17e
    c9cd:	7f 01 1a    	clr	0x11a
    c9d0:	7e d5 27    	jmp	0xd527
    c9d3:	7d 01 1a    	tst	0x11a
    c9d6:	26 03       	bne	0x0xc9db
    c9d8:	7e d5 27    	jmp	0xd527
    c9db:	c6 02       	ldab	#0x2
    c9dd:	18 ce 00 b0 	ldy	#0xb0
    c9e1:	96 f9       	ldaa	*0xf9
    c9e3:	81 04       	cmpa	#0x4
    c9e5:	26 07       	bne	0x0xc9ee
    c9e7:	18 ce 50 30 	ldy	#0x5030
    c9eb:	14 fb 80    	bset	*0xfb, #0x80
    c9ee:	ce 01 20    	ldx	#0x120
    c9f1:	f1 01 1e    	cmpb	0x11e
    c9f4:	27 0a       	beq	0x0xca00
    c9f6:	18 08       	iny
    c9f8:	cb 04       	addb	#0x4
    c9fa:	08          	inx
    c9fb:	08          	inx
    c9fc:	08          	inx
    c9fd:	08          	inx
    c9fe:	20 f1       	bra	0x0xc9f1
    ca00:	18 a6 00    	ldaa	0x0,y
    ca03:	7d 01 1a    	tst	0x11a
    ca06:	2b 23       	bmi	0x0xca2b
    ca08:	4c          	inca
    ca09:	f6 01 11    	ldab	0x111
    ca0c:	c5 04       	bitb	#0x4
    ca0e:	27 02       	beq	0x0xca12
    ca10:	8b 09       	adda	#0x9
    ca12:	4d          	tsta
    ca13:	2a 02       	bpl	0x0xca17
    ca15:	86 7f       	ldaa	#0x7f
    ca17:	18 a7 00    	staa	0x0,y
    ca1a:	bd eb c6    	jsr	0xebc6
    ca1d:	bd ca 3b    	jsr	0xca3b
    ca20:	7f 01 1a    	clr	0x11a
    ca23:	86 03       	ldaa	#0x3
    ca25:	b7 01 1c    	staa	0x11c
    ca28:	7e d5 27    	jmp	0xd527
    ca2b:	4a          	deca
    ca2c:	f6 01 11    	ldab	0x111
    ca2f:	c5 04       	bitb	#0x4
    ca31:	27 02       	beq	0x0xca35
    ca33:	80 09       	suba	#0x9
    ca35:	4d          	tsta
    ca36:	2a df       	bpl	0x0xca17
    ca38:	4f          	clra
    ca39:	20 dc       	bra	0x0xca17
    ca3b:	18 a6 00    	ldaa	0x0,y
    ca3e:	36          	psha
    ca3f:	18 8f       	xgdy
    ca41:	c1 3f       	cmpb	#0x3f
    ca43:	22 02       	bhi	0x0xca47
    ca45:	cb 80       	addb	#0x80
    ca47:	37          	pshb
    ca48:	c4 0f       	andb	#0xf
    ca4a:	c1 08       	cmpb	#0x8
    ca4c:	25 02       	bcs	0x0xca50
    ca4e:	c0 08       	subb	#0x8
    ca50:	5c          	incb
    ca51:	86 01       	ldaa	#0x1
    ca53:	5a          	decb
    ca54:	27 03       	beq	0x0xca59
    ca56:	48          	asla
    ca57:	20 fa       	bra	0x0xca53
    ca59:	43          	coma
    ca5a:	7d 00 d0    	tst	0xd0
    ca5d:	27 05       	beq	0x0xca64
    ca5f:	7f 00 d0    	clr	0xd0
    ca62:	20 08       	bra	0x0xca6c
    ca64:	7d 10 29    	tst	0x1029
    ca67:	2a fb       	bpl	0x0xca64
    ca69:	f6 10 2a    	ldab	0x102a
    ca6c:	01          	nop
    ca6d:	01          	nop
    ca6e:	01          	nop
    ca6f:	01          	nop
    ca70:	b7 10 42    	staa	0x1042
    ca73:	01          	nop
    ca74:	01          	nop
    ca75:	01          	nop
    ca76:	01          	nop
    ca77:	01          	nop
    ca78:	01          	nop
    ca79:	01          	nop
    ca7a:	86 83       	ldaa	#0x83
    ca7c:	b7 10 2a    	staa	0x102a
    ca7f:	32          	pula
    ca80:	81 b8       	cmpa	#0xb8
    ca82:	25 04       	bcs	0x0xca88
    ca84:	86 8a       	ldaa	#0x8a
    ca86:	20 0a       	bra	0x0xca92
    ca88:	81 b0       	cmpa	#0xb0
    ca8a:	25 04       	bcs	0x0xca90
    ca8c:	86 89       	ldaa	#0x89
    ca8e:	20 02       	bra	0x0xca92
    ca90:	86 88       	ldaa	#0x88
    ca92:	7d 10 29    	tst	0x1029
    ca95:	2a fb       	bpl	0x0xca92
    ca97:	f6 10 2a    	ldab	0x102a
    ca9a:	b7 10 2a    	staa	0x102a
    ca9d:	32          	pula
    ca9e:	7d 10 29    	tst	0x1029
    caa1:	2a fb       	bpl	0x0xca9e
    caa3:	f6 10 2a    	ldab	0x102a
    caa6:	b7 10 2a    	staa	0x102a
    caa9:	39          	rts
    caaa:	7d 01 1c    	tst	0x11c
    caad:	2a 03       	bpl	0x0xcab2
    caaf:	7e e5 87    	jmp	0xe587
    cab2:	7d 01 1e    	tst	0x11e
    cab5:	26 08       	bne	0x0xcabf
    cab7:	7d 01 1c    	tst	0x11c
    caba:	27 1f       	beq	0x0xcadb
    cabc:	7e b2 70    	jmp	0xb270
    cabf:	7d 01 1c    	tst	0x11c
    cac2:	27 24       	beq	0x0xcae8
    cac4:	cc 01 20    	ldd	#0x120
    cac7:	fb 01 1e    	addb	0x11e
    caca:	8f          	xgdx
    cacb:	bd ea 76    	jsr	0xea76
    cace:	7a 01 1e    	dec	0x11e
    cad1:	7a 01 1c    	dec	0x11c
    cad4:	26 12       	bne	0x0xcae8
    cad6:	bd eb 27    	jsr	0xeb27
    cad9:	20 0d       	bra	0x0xcae8
    cadb:	7d 01 1e    	tst	0x11e
    cade:	26 08       	bne	0x0xcae8
    cae0:	86 02       	ldaa	#0x2
    cae2:	b7 01 1e    	staa	0x11e
    cae5:	bd ea b2    	jsr	0xeab2
    cae8:	7d 01 7f    	tst	0x17f
    caeb:	27 10       	beq	0x0xcafd
    caed:	bd ed 28    	jsr	0xed28
    caf0:	4f          	clra
    caf1:	b7 01 7f    	staa	0x17f
    caf4:	b7 01 7e    	staa	0x17e
    caf7:	b7 01 1a    	staa	0x11a
    cafa:	7e d5 27    	jmp	0xd527
    cafd:	7d 01 7e    	tst	0x17e
    cb00:	27 0c       	beq	0x0xcb0e
    cb02:	bd ec 5e    	jsr	0xec5e
    cb05:	7f 01 7e    	clr	0x17e
    cb08:	7f 01 1a    	clr	0x11a
    cb0b:	7e d5 27    	jmp	0xd527
    cb0e:	7d 01 1a    	tst	0x11a
    cb11:	26 03       	bne	0x0xcb16
    cb13:	7e d5 27    	jmp	0xd527
    cb16:	c6 02       	ldab	#0x2
    cb18:	18 ce 00 a8 	ldy	#0xa8
    cb1c:	96 f9       	ldaa	*0xf9
    cb1e:	81 04       	cmpa	#0x4
    cb20:	26 07       	bne	0x0xcb29
    cb22:	14 fb 80    	bset	*0xfb, #0x80
    cb25:	18 ce 50 28 	ldy	#0x5028
    cb29:	ce 01 20    	ldx	#0x120
    cb2c:	f1 01 1e    	cmpb	0x11e
    cb2f:	27 0a       	beq	0x0xcb3b
    cb31:	18 08       	iny
    cb33:	cb 04       	addb	#0x4
    cb35:	08          	inx
    cb36:	08          	inx
    cb37:	08          	inx
    cb38:	08          	inx
    cb39:	20 f1       	bra	0x0xcb2c
    cb3b:	18 a6 00    	ldaa	0x0,y
    cb3e:	7d 01 1a    	tst	0x11a
    cb41:	2b 20       	bmi	0x0xcb63
    cb43:	4c          	inca
    cb44:	f6 01 11    	ldab	0x111
    cb47:	c5 04       	bitb	#0x4
    cb49:	27 02       	beq	0x0xcb4d
    cb4b:	8b 09       	adda	#0x9
    cb4d:	84 7f       	anda	#0x7f
    cb4f:	18 a7 00    	staa	0x0,y
    cb52:	bd eb 8a    	jsr	0xeb8a
    cb55:	bd ca 3b    	jsr	0xca3b
    cb58:	7f 01 1a    	clr	0x11a
    cb5b:	86 03       	ldaa	#0x3
    cb5d:	b7 01 1c    	staa	0x11c
    cb60:	7e d5 27    	jmp	0xd527
    cb63:	4a          	deca
    cb64:	f6 01 11    	ldab	0x111
    cb67:	c5 04       	bitb	#0x4
    cb69:	27 02       	beq	0x0xcb6d
    cb6b:	80 09       	suba	#0x9
    cb6d:	84 7f       	anda	#0x7f
    cb6f:	20 de       	bra	0x0xcb4f
    cb71:	7d 01 1c    	tst	0x11c
    cb74:	2a 03       	bpl	0x0xcb79
    cb76:	7e e5 c0    	jmp	0xe5c0
    cb79:	7d 01 1e    	tst	0x11e
    cb7c:	26 08       	bne	0x0xcb86
    cb7e:	7d 01 1c    	tst	0x11c
    cb81:	27 1f       	beq	0x0xcba2
    cb83:	7e b2 70    	jmp	0xb270
    cb86:	7d 01 1c    	tst	0x11c
    cb89:	27 24       	beq	0x0xcbaf
    cb8b:	cc 01 20    	ldd	#0x120
    cb8e:	fb 01 1e    	addb	0x11e
    cb91:	8f          	xgdx
    cb92:	bd ea 76    	jsr	0xea76
    cb95:	7a 01 1e    	dec	0x11e
    cb98:	7a 01 1c    	dec	0x11c
    cb9b:	26 12       	bne	0x0xcbaf
    cb9d:	bd eb 27    	jsr	0xeb27
    cba0:	20 0d       	bra	0x0xcbaf
    cba2:	7d 01 1e    	tst	0x11e
    cba5:	26 08       	bne	0x0xcbaf
    cba7:	86 02       	ldaa	#0x2
    cba9:	b7 01 1e    	staa	0x11e
    cbac:	bd ea b2    	jsr	0xeab2
    cbaf:	7d 01 7f    	tst	0x17f
    cbb2:	27 10       	beq	0x0xcbc4
    cbb4:	bd ed 28    	jsr	0xed28
    cbb7:	4f          	clra
    cbb8:	b7 01 7f    	staa	0x17f
    cbbb:	b7 01 7e    	staa	0x17e
    cbbe:	b7 01 1a    	staa	0x11a
    cbc1:	7e d5 27    	jmp	0xd527
    cbc4:	7d 01 7e    	tst	0x17e
    cbc7:	27 0c       	beq	0x0xcbd5
    cbc9:	bd ec 5e    	jsr	0xec5e
    cbcc:	7f 01 7e    	clr	0x17e
    cbcf:	7f 01 1a    	clr	0x11a
    cbd2:	7e d5 27    	jmp	0xd527
    cbd5:	7d 01 1a    	tst	0x11a
    cbd8:	26 03       	bne	0x0xcbdd
    cbda:	7e d5 27    	jmp	0xd527
    cbdd:	c6 02       	ldab	#0x2
    cbdf:	18 ce 00 b8 	ldy	#0xb8
    cbe3:	96 f9       	ldaa	*0xf9
    cbe5:	81 04       	cmpa	#0x4
    cbe7:	26 07       	bne	0x0xcbf0
    cbe9:	14 fb 80    	bset	*0xfb, #0x80
    cbec:	18 ce 50 38 	ldy	#0x5038
    cbf0:	ce 01 20    	ldx	#0x120
    cbf3:	f1 01 1e    	cmpb	0x11e
    cbf6:	27 0a       	beq	0x0xcc02
    cbf8:	18 08       	iny
    cbfa:	cb 04       	addb	#0x4
    cbfc:	08          	inx
    cbfd:	08          	inx
    cbfe:	08          	inx
    cbff:	08          	inx
    cc00:	20 f1       	bra	0x0xcbf3
    cc02:	18 a6 00    	ldaa	0x0,y
    cc05:	7d 01 1a    	tst	0x11a
    cc08:	2b 20       	bmi	0x0xcc2a
    cc0a:	4c          	inca
    cc0b:	f6 01 11    	ldab	0x111
    cc0e:	c5 04       	bitb	#0x4
    cc10:	27 02       	beq	0x0xcc14
    cc12:	8b 09       	adda	#0x9
    cc14:	84 7f       	anda	#0x7f
    cc16:	18 a7 00    	staa	0x0,y
    cc19:	bd eb 8a    	jsr	0xeb8a
    cc1c:	bd ca 3b    	jsr	0xca3b
    cc1f:	7f 01 1a    	clr	0x11a
    cc22:	86 03       	ldaa	#0x3
    cc24:	b7 01 1c    	staa	0x11c
    cc27:	7e d5 27    	jmp	0xd527
    cc2a:	4a          	deca
    cc2b:	f6 01 11    	ldab	0x111
    cc2e:	c5 04       	bitb	#0x4
    cc30:	27 02       	beq	0x0xcc34
    cc32:	80 09       	suba	#0x9
    cc34:	84 7f       	anda	#0x7f
    cc36:	20 de       	bra	0x0xcc16
    cc38:	7d 01 1c    	tst	0x11c
    cc3b:	2a 03       	bpl	0x0xcc40
    cc3d:	7e e5 f9    	jmp	0xe5f9
    cc40:	7d 01 1e    	tst	0x11e
    cc43:	26 08       	bne	0x0xcc4d
    cc45:	7d 01 1c    	tst	0x11c
    cc48:	27 1f       	beq	0x0xcc69
    cc4a:	7e b2 70    	jmp	0xb270
    cc4d:	7d 01 1c    	tst	0x11c
    cc50:	27 27       	beq	0x0xcc79
    cc52:	cc 01 20    	ldd	#0x120
    cc55:	fb 01 1e    	addb	0x11e
    cc58:	8f          	xgdx
    cc59:	bd ea 76    	jsr	0xea76
    cc5c:	7a 01 1e    	dec	0x11e
    cc5f:	7a 01 1c    	dec	0x11c
    cc62:	26 15       	bne	0x0xcc79
    cc64:	bd ea f4    	jsr	0xeaf4
    cc67:	20 10       	bra	0x0xcc79
    cc69:	7d 01 1e    	tst	0x11e
    cc6c:	26 0b       	bne	0x0xcc79
    cc6e:	86 13       	ldaa	#0x13
    cc70:	b7 01 1e    	staa	0x11e
    cc73:	bd ea b2    	jsr	0xeab2
    cc76:	7f 01 6b    	clr	0x16b
    cc79:	7d 01 7f    	tst	0x17f
    cc7c:	27 10       	beq	0x0xcc8e
    cc7e:	bd ec b6    	jsr	0xecb6
    cc81:	4f          	clra
    cc82:	b7 01 7f    	staa	0x17f
    cc85:	b7 01 7e    	staa	0x17e
    cc88:	b7 01 1a    	staa	0x11a
    cc8b:	7e d5 27    	jmp	0xd527
    cc8e:	7d 01 1a    	tst	0x11a
    cc91:	26 03       	bne	0x0xcc96
    cc93:	7e d5 27    	jmp	0xd527
    cc96:	b6 01 1e    	ldaa	0x11e
    cc99:	81 13       	cmpa	#0x13
    cc9b:	26 23       	bne	0x0xccc0
    cc9d:	ce e4 68    	ldx	#0xe468
    cca0:	18 ce 01 30 	ldy	#0x130
    cca4:	d6 a7       	ldab	*0xa7
    cca6:	bd c7 5e    	jsr	0xc75e
    cca9:	d7 a7       	stab	*0xa7
    ccab:	bd ec 3d    	jsr	0xec3d
    ccae:	7f 01 1a    	clr	0x11a
    ccb1:	86 04       	ldaa	#0x4
    ccb3:	b7 01 1c    	staa	0x11c
    ccb6:	96 a7       	ldaa	*0xa7
    ccb8:	c6 a7       	ldab	#0xa7
    ccba:	bd b0 fc    	jsr	0xb0fc
    ccbd:	7e d5 27    	jmp	0xd527
    ccc0:	81 19       	cmpa	#0x19
    ccc2:	26 32       	bne	0x0xccf6
    ccc4:	ce e5 22    	ldx	#0xe522
    ccc7:	18 ce 01 37 	ldy	#0x137
    cccb:	d6 a6       	ldab	*0xa6
    cccd:	7d 01 1a    	tst	0x11a
    ccd0:	2b 1e       	bmi	0x0xccf0
    ccd2:	5c          	incb
    ccd3:	c1 03       	cmpb	#0x3
    ccd5:	25 02       	bcs	0x0xccd9
    ccd7:	c6 02       	ldab	#0x2
    ccd9:	d7 a6       	stab	*0xa6
    ccdb:	bd ec 29    	jsr	0xec29
    ccde:	7f 01 1a    	clr	0x11a
    cce1:	86 03       	ldaa	#0x3
    cce3:	b7 01 1c    	staa	0x11c
    cce6:	96 a6       	ldaa	*0xa6
    cce8:	c6 a6       	ldab	#0xa6
    ccea:	bd b0 fc    	jsr	0xb0fc
    cced:	7e d5 27    	jmp	0xd527
    ccf0:	5a          	decb
    ccf1:	2a e6       	bpl	0x0xccd9
    ccf3:	5f          	clrb
    ccf4:	20 e3       	bra	0x0xccd9
    ccf6:	ce e4 88    	ldx	#0xe488
    ccf9:	18 ce 01 3b 	ldy	#0x13b
    ccfd:	d6 a5       	ldab	*0xa5
    ccff:	bd c7 d8    	jsr	0xc7d8
    cd02:	d7 a5       	stab	*0xa5
    cd04:	bd ec 3d    	jsr	0xec3d
    cd07:	7f 01 1a    	clr	0x11a
    cd0a:	86 04       	ldaa	#0x4
    cd0c:	b7 01 1c    	staa	0x11c
    cd0f:	96 a5       	ldaa	*0xa5
    cd11:	c6 a5       	ldab	#0xa5
    cd13:	bd b0 fc    	jsr	0xb0fc
    cd16:	7e d5 27    	jmp	0xd527
    cd19:	7f 01 7f    	clr	0x17f
    cd1c:	7f 01 7e    	clr	0x17e
    cd1f:	7f 01 1a    	clr	0x11a
    cd22:	7e d5 27    	jmp	0xd527
    cd25:	7d 01 1c    	tst	0x11c
    cd28:	2a 03       	bpl	0x0xcd2d
    cd2a:	7e e6 57    	jmp	0xe657
    cd2d:	7d 01 1e    	tst	0x11e
    cd30:	26 08       	bne	0x0xcd3a
    cd32:	7d 01 1c    	tst	0x11c
    cd35:	27 1f       	beq	0x0xcd56
    cd37:	7e b2 70    	jmp	0xb270
    cd3a:	7d 01 1c    	tst	0x11c
    cd3d:	27 24       	beq	0x0xcd63
    cd3f:	cc 01 20    	ldd	#0x120
    cd42:	fb 01 1e    	addb	0x11e
    cd45:	8f          	xgdx
    cd46:	bd ea 76    	jsr	0xea76
    cd49:	7a 01 1e    	dec	0x11e
    cd4c:	7a 01 1c    	dec	0x11c
    cd4f:	26 12       	bne	0x0xcd63
    cd51:	bd eb 27    	jsr	0xeb27
    cd54:	20 0d       	bra	0x0xcd63
    cd56:	7d 01 1e    	tst	0x11e
    cd59:	26 08       	bne	0x0xcd63
    cd5b:	86 12       	ldaa	#0x12
    cd5d:	b7 01 1e    	staa	0x11e
    cd60:	bd ea b2    	jsr	0xeab2
    cd63:	7d 01 7f    	tst	0x17f
    cd66:	27 10       	beq	0x0xcd78
    cd68:	bd ed 28    	jsr	0xed28
    cd6b:	4f          	clra
    cd6c:	b7 01 7f    	staa	0x17f
    cd6f:	b7 01 7e    	staa	0x17e
    cd72:	b7 01 1a    	staa	0x11a
    cd75:	7e d5 27    	jmp	0xd527
    cd78:	7d 01 1a    	tst	0x11a
    cd7b:	26 03       	bne	0x0xcd80
    cd7d:	7e d5 27    	jmp	0xd527
    cd80:	b6 01 1e    	ldaa	0x11e
    cd83:	81 12       	cmpa	#0x12
    cd85:	26 16       	bne	0x0xcd9d
    cd87:	d6 21       	ldab	*0x21
    cd89:	c8 02       	eorb	#0x2
    cd8b:	d7 21       	stab	*0x21
    cd8d:	c4 02       	andb	#0x2
    cd8f:	54          	lsrb
    cd90:	18 ce 01 30 	ldy	#0x130
    cd94:	ce da e4    	ldx	#0xdae4
    cd97:	bd ec 29    	jsr	0xec29
    cd9a:	7e b9 23    	jmp	0xb923
    cd9d:	81 16       	cmpa	#0x16
    cd9f:	26 17       	bne	0x0xcdb8
    cda1:	d6 21       	ldab	*0x21
    cda3:	c8 04       	eorb	#0x4
    cda5:	d7 21       	stab	*0x21
    cda7:	c4 04       	andb	#0x4
    cda9:	54          	lsrb
    cdaa:	54          	lsrb
    cdab:	18 ce 01 34 	ldy	#0x134
    cdaf:	ce da e4    	ldx	#0xdae4
    cdb2:	bd ec 29    	jsr	0xec29
    cdb5:	7e b9 23    	jmp	0xb923
    cdb8:	81 1a       	cmpa	#0x1a
    cdba:	26 30       	bne	0x0xcdec
    cdbc:	b6 01 1a    	ldaa	0x11a
    cdbf:	2b 23       	bmi	0x0xcde4
    cdc1:	96 22       	ldaa	*0x22
    cdc3:	4c          	inca
    cdc4:	81 7f       	cmpa	#0x7f
    cdc6:	25 02       	bcs	0x0xcdca
    cdc8:	86 7f       	ldaa	#0x7f
    cdca:	97 22       	staa	*0x22
    cdcc:	ce 01 38    	ldx	#0x138
    cdcf:	bd eb c6    	jsr	0xebc6
    cdd2:	7f 01 1a    	clr	0x11a
    cdd5:	86 03       	ldaa	#0x3
    cdd7:	b7 01 1c    	staa	0x11c
    cdda:	96 22       	ldaa	*0x22
    cddc:	c6 22       	ldab	#0x22
    cdde:	bd b0 fc    	jsr	0xb0fc
    cde1:	7e d5 27    	jmp	0xd527
    cde4:	96 22       	ldaa	*0x22
    cde6:	4a          	deca
    cde7:	2a e1       	bpl	0x0xcdca
    cde9:	4f          	clra
    cdea:	20 de       	bra	0x0xcdca
    cdec:	b6 01 1a    	ldaa	0x11a
    cdef:	2b 1f       	bmi	0x0xce10
    cdf1:	96 23       	ldaa	*0x23
    cdf3:	4c          	inca
    cdf4:	84 7f       	anda	#0x7f
    cdf6:	97 23       	staa	*0x23
    cdf8:	ce 01 3c    	ldx	#0x13c
    cdfb:	bd eb 8a    	jsr	0xeb8a
    cdfe:	7f 01 1a    	clr	0x11a
    ce01:	86 03       	ldaa	#0x3
    ce03:	b7 01 1c    	staa	0x11c
    ce06:	96 23       	ldaa	*0x23
    ce08:	c6 23       	ldab	#0x23
    ce0a:	bd b0 fc    	jsr	0xb0fc
    ce0d:	7e d5 27    	jmp	0xd527
    ce10:	96 23       	ldaa	*0x23
    ce12:	4a          	deca
    ce13:	84 7f       	anda	#0x7f
    ce15:	20 df       	bra	0x0xcdf6
    ce17:	39          	rts
    ce18:	7d 01 1c    	tst	0x11c
    ce1b:	2a 03       	bpl	0x0xce20
    ce1d:	7e e7 89    	jmp	0xe789
    ce20:	7d 01 1e    	tst	0x11e
    ce23:	26 08       	bne	0x0xce2d
    ce25:	7d 01 1c    	tst	0x11c
    ce28:	27 1f       	beq	0x0xce49
    ce2a:	7e b2 70    	jmp	0xb270
    ce2d:	7d 01 1c    	tst	0x11c
    ce30:	27 24       	beq	0x0xce56
    ce32:	cc 01 20    	ldd	#0x120
    ce35:	fb 01 1e    	addb	0x11e
    ce38:	8f          	xgdx
    ce39:	bd ea 76    	jsr	0xea76
    ce3c:	7a 01 1e    	dec	0x11e
    ce3f:	7a 01 1c    	dec	0x11c
    ce42:	26 12       	bne	0x0xce56
    ce44:	bd ea f4    	jsr	0xeaf4
    ce47:	20 0d       	bra	0x0xce56
    ce49:	7d 01 1e    	tst	0x11e
    ce4c:	26 08       	bne	0x0xce56
    ce4e:	86 13       	ldaa	#0x13
    ce50:	b7 01 1e    	staa	0x11e
    ce53:	bd ea b2    	jsr	0xeab2
    ce56:	7d 01 7f    	tst	0x17f
    ce59:	27 0d       	beq	0x0xce68
    ce5b:	4f          	clra
    ce5c:	b7 01 7f    	staa	0x17f
    ce5f:	b7 01 7e    	staa	0x17e
    ce62:	b7 01 1a    	staa	0x11a
    ce65:	7e d5 27    	jmp	0xd527
    ce68:	7d 01 1a    	tst	0x11a
    ce6b:	26 03       	bne	0x0xce70
    ce6d:	7e d5 27    	jmp	0xd527
    ce70:	7d 01 1a    	tst	0x11a
    ce73:	2b 26       	bmi	0x0xce9b
    ce75:	96 f0       	ldaa	*0xf0
    ce77:	84 07       	anda	#0x7
    ce79:	4c          	inca
    ce7a:	84 07       	anda	#0x7
    ce7c:	97 f0       	staa	*0xf0
    ce7e:	bd ad 9e    	jsr	0xad9e
    ce81:	96 f0       	ldaa	*0xf0
    ce83:	b7 7f fa    	staa	0x7ffa
    ce86:	4c          	inca
    ce87:	ce 01 31    	ldx	#0x131
    ce8a:	bd eb 8a    	jsr	0xeb8a
    ce8d:	7f 01 1a    	clr	0x11a
    ce90:	86 03       	ldaa	#0x3
    ce92:	b7 01 1c    	staa	0x11c
    ce95:	bd ad c4    	jsr	0xadc4
    ce98:	7e d5 27    	jmp	0xd527
    ce9b:	96 f0       	ldaa	*0xf0
    ce9d:	84 07       	anda	#0x7
    ce9f:	4a          	deca
    cea0:	20 d8       	bra	0x0xce7a
    cea2:	7d 01 1c    	tst	0x11c
    cea5:	2a 03       	bpl	0x0xceaa
    cea7:	7e e9 82    	jmp	0xe982
    ceaa:	7d 01 1e    	tst	0x11e
    cead:	26 08       	bne	0x0xceb7
    ceaf:	7d 01 1c    	tst	0x11c
    ceb2:	27 1a       	beq	0x0xcece
    ceb4:	7e b2 70    	jmp	0xb270
    ceb7:	7d 01 1c    	tst	0x11c
    ceba:	27 1a       	beq	0x0xced6
    cebc:	cc 01 20    	ldd	#0x120
    cebf:	fb 01 1e    	addb	0x11e
    cec2:	8f          	xgdx
    cec3:	bd ea 76    	jsr	0xea76
    cec6:	7a 01 1c    	dec	0x11c
    cec9:	bd ea b2    	jsr	0xeab2
    cecc:	20 08       	bra	0x0xced6
    cece:	86 0f       	ldaa	#0xf
    ced0:	b7 01 1e    	staa	0x11e
    ced3:	bd ea b2    	jsr	0xeab2
    ced6:	7d 01 7f    	tst	0x17f
    ced9:	27 0d       	beq	0x0xcee8
    cedb:	4f          	clra
    cedc:	b7 01 7f    	staa	0x17f
    cedf:	b7 01 7e    	staa	0x17e
    cee2:	b7 01 1a    	staa	0x11a
    cee5:	7e d5 27    	jmp	0xd527
    cee8:	7d 01 7e    	tst	0x17e
    ceeb:	27 29       	beq	0x0xcf16
    ceed:	b6 01 1e    	ldaa	0x11e
    cef0:	81 0f       	cmpa	#0xf
    cef2:	26 11       	bne	0x0xcf05
    cef4:	86 1f       	ldaa	#0x1f
    cef6:	b7 01 1e    	staa	0x11e
    cef9:	bd ea b2    	jsr	0xeab2
    cefc:	7f 01 7e    	clr	0x17e
    ceff:	7f 01 1a    	clr	0x11a
    cf02:	7e d5 27    	jmp	0xd527
    cf05:	86 0f       	ldaa	#0xf
    cf07:	b7 01 1e    	staa	0x11e
    cf0a:	bd ea b2    	jsr	0xeab2
    cf0d:	7f 01 7e    	clr	0x17e
    cf10:	7f 01 1a    	clr	0x11a
    cf13:	7e d5 27    	jmp	0xd527
    cf16:	7d 01 1a    	tst	0x11a
    cf19:	26 03       	bne	0x0xcf1e
    cf1b:	7e d5 27    	jmp	0xd527
    cf1e:	b6 01 1e    	ldaa	0x11e
    cf21:	81 0f       	cmpa	#0xf
    cf23:	26 13       	bne	0x0xcf38
    cf25:	b6 01 2f    	ldaa	0x12f
    cf28:	81 43       	cmpa	#0x43
    cf2a:	26 06       	bne	0x0xcf32
    cf2c:	4c          	inca
    cf2d:	b7 01 2f    	staa	0x12f
    cf30:	20 17       	bra	0x0xcf49
    cf32:	4a          	deca
    cf33:	b7 01 2f    	staa	0x12f
    cf36:	20 11       	bra	0x0xcf49
    cf38:	b6 01 3f    	ldaa	0x13f
    cf3b:	81 41       	cmpa	#0x41
    cf3d:	26 06       	bne	0x0xcf45
    cf3f:	4c          	inca
    cf40:	b7 01 3f    	staa	0x13f
    cf43:	20 04       	bra	0x0xcf49
    cf45:	4a          	deca
    cf46:	b7 01 3f    	staa	0x13f
    cf49:	7f 01 1a    	clr	0x11a
    cf4c:	86 01       	ldaa	#0x1
    cf4e:	b7 01 1c    	staa	0x11c
    cf51:	7e d5 27    	jmp	0xd527
    cf54:	7d 01 1c    	tst	0x11c
    cf57:	2a 03       	bpl	0x0xcf5c
    cf59:	7e e6 cd    	jmp	0xe6cd
    cf5c:	7d 01 1e    	tst	0x11e
    cf5f:	26 08       	bne	0x0xcf69
    cf61:	7d 01 1c    	tst	0x11c
    cf64:	27 1f       	beq	0x0xcf85
    cf66:	7e b2 70    	jmp	0xb270
    cf69:	7d 01 1c    	tst	0x11c
    cf6c:	27 24       	beq	0x0xcf92
    cf6e:	cc 01 20    	ldd	#0x120
    cf71:	fb 01 1e    	addb	0x11e
    cf74:	8f          	xgdx
    cf75:	bd ea 76    	jsr	0xea76
    cf78:	7a 01 1e    	dec	0x11e
    cf7b:	7a 01 1c    	dec	0x11c
    cf7e:	26 12       	bne	0x0xcf92
    cf80:	bd eb 27    	jsr	0xeb27
    cf83:	20 0d       	bra	0x0xcf92
    cf85:	7d 01 1e    	tst	0x11e
    cf88:	26 08       	bne	0x0xcf92
    cf8a:	86 13       	ldaa	#0x13
    cf8c:	b7 01 1e    	staa	0x11e
    cf8f:	bd ea b2    	jsr	0xeab2
    cf92:	7d 01 1a    	tst	0x11a
    cf95:	26 09       	bne	0x0xcfa0
    cf97:	7f 01 7f    	clr	0x17f
    cf9a:	7f 01 7e    	clr	0x17e
    cf9d:	7e d5 27    	jmp	0xd527
    cfa0:	7f 01 1a    	clr	0x11a
    cfa3:	7f 01 7f    	clr	0x17f
    cfa6:	7f 01 7e    	clr	0x17e
    cfa9:	7d 00 d0    	tst	0xd0
    cfac:	27 05       	beq	0x0xcfb3
    cfae:	7f 00 d0    	clr	0xd0
    cfb1:	20 08       	bra	0x0xcfbb
    cfb3:	7d 10 29    	tst	0x1029
    cfb6:	2a fb       	bpl	0x0xcfb3
    cfb8:	b6 10 2a    	ldaa	0x102a
    cfbb:	5f          	clrb
    cfbc:	01          	nop
    cfbd:	01          	nop
    cfbe:	01          	nop
    cfbf:	01          	nop
    cfc0:	f7 10 42    	stab	0x1042
    cfc3:	01          	nop
    cfc4:	01          	nop
    cfc5:	01          	nop
    cfc6:	01          	nop
    cfc7:	01          	nop
    cfc8:	01          	nop
    cfc9:	01          	nop
    cfca:	86 fe       	ldaa	#0xfe
    cfcc:	b7 10 2a    	staa	0x102a
    cfcf:	bd ea d7    	jsr	0xead7
    cfd2:	c6 20       	ldab	#0x20
    cfd4:	ce 01 20    	ldx	#0x120
    cfd7:	18 ce d0 37 	ldy	#0xd037
    cfdb:	18 a6 00    	ldaa	0x0,y
    cfde:	a7 00       	staa	0x0,x
    cfe0:	08          	inx
    cfe1:	18 08       	iny
    cfe3:	5a          	decb
    cfe4:	26 f5       	bne	0x0xcfdb
    cfe6:	7f 01 1e    	clr	0x11e
    cfe9:	86 20       	ldaa	#0x20
    cfeb:	b7 01 1c    	staa	0x11c
    cfee:	ce 10 23    	ldx	#0x1023
    cff1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xcff1
    cff5:	bd ea 2b    	jsr	0xea2b
    cff8:	7d 01 1c    	tst	0x11c
    cffb:	27 05       	beq	0x0xd002
    cffd:	ce 10 23    	ldx	#0x1023
    d000:	20 ef       	bra	0x0xcff1
    d002:	86 ff       	ldaa	#0xff
    d004:	97 f8       	staa	*0xf8
    d006:	c6 06       	ldab	#0x6
    d008:	ce ff ff    	ldx	#0xffff
    d00b:	01          	nop
    d00c:	01          	nop
    d00d:	01          	nop
    d00e:	01          	nop
    d00f:	09          	dex
    d010:	26 f9       	bne	0x0xd00b
    d012:	5a          	decb
    d013:	26 f6       	bne	0x0xd00b
    d015:	c6 06       	ldab	#0x6
    d017:	44          	lsra
    d018:	27 04       	beq	0x0xd01e
    d01a:	97 f8       	staa	*0xf8
    d01c:	20 ed       	bra	0x0xd00b
    d01e:	97 f8       	staa	*0xf8
    d020:	97 fb       	staa	*0xfb
    d022:	97 f2       	staa	*0xf2
    d024:	97 ff       	staa	*0xff
    d026:	b7 01 1b    	staa	0x11b
    d029:	bd a3 59    	jsr	0xa359
    d02c:	bd a3 70    	jsr	0xa370
    d02f:	86 80       	ldaa	#0x80
    d031:	b7 01 1c    	staa	0x11c
    d034:	7e 82 83    	jmp	0x8283
    d037:	20 54       	bra	0x0xd08d
    d039:	55          	.byte	0x55
    d03a:	4e          	.byte	0x4e
    d03b:	49          	rola
    d03c:	4e          	.byte	0x4e
    d03d:	47          	asra
    d03e:	20 2e       	bra	0x0xd06e
    d040:	2e 2e       	bgt	0x0xd070
    d042:	2e 2e       	bgt	0x0xd072
    d044:	20 20       	bra	0x0xd066
    d046:	20 20       	bra	0x0xd068
    d048:	20 20       	bra	0x0xd06a
    d04a:	20 20       	bra	0x0xd06c
    d04c:	20 20       	bra	0x0xd06e
    d04e:	20 20       	bra	0x0xd070
    d050:	20 20       	bra	0x0xd072
    d052:	20 20       	bra	0x0xd074
    d054:	20 20       	bra	0x0xd076
    d056:	20 7e       	bra	0x0xd0d6
    d058:	d5 27       	bitb	*0x27
    d05a:	7d 01 1c    	tst	0x11c
    d05d:	2a 03       	bpl	0x0xd062
    d05f:	7e e1 54    	jmp	0xe154
    d062:	7d 01 1e    	tst	0x11e
    d065:	26 08       	bne	0x0xd06f
    d067:	7d 01 1c    	tst	0x11c
    d06a:	27 1f       	beq	0x0xd08b
    d06c:	7e b2 70    	jmp	0xb270
    d06f:	7d 01 1c    	tst	0x11c
    d072:	27 24       	beq	0x0xd098
    d074:	cc 01 20    	ldd	#0x120
    d077:	fb 01 1e    	addb	0x11e
    d07a:	8f          	xgdx
    d07b:	bd ea 76    	jsr	0xea76
    d07e:	7a 01 1e    	dec	0x11e
    d081:	7a 01 1c    	dec	0x11c
    d084:	26 12       	bne	0x0xd098
    d086:	bd ea f4    	jsr	0xeaf4
    d089:	20 0d       	bra	0x0xd098
    d08b:	7d 01 1e    	tst	0x11e
    d08e:	26 08       	bne	0x0xd098
    d090:	86 13       	ldaa	#0x13
    d092:	b7 01 1e    	staa	0x11e
    d095:	bd ea b2    	jsr	0xeab2
    d098:	7d 01 7f    	tst	0x17f
    d09b:	27 1e       	beq	0x0xd0bb
    d09d:	2a 10       	bpl	0x0xd0af
    d09f:	bd ec b6    	jsr	0xecb6
    d0a2:	4f          	clra
    d0a3:	b7 01 7f    	staa	0x17f
    d0a6:	b7 01 7e    	staa	0x17e
    d0a9:	b7 01 1a    	staa	0x11a
    d0ac:	7e d5 27    	jmp	0xd527
    d0af:	b6 01 1e    	ldaa	0x11e
    d0b2:	81 19       	cmpa	#0x19
    d0b4:	27 ec       	beq	0x0xd0a2
    d0b6:	bd ec b6    	jsr	0xecb6
    d0b9:	20 e7       	bra	0x0xd0a2
    d0bb:	7d 01 1a    	tst	0x11a
    d0be:	26 03       	bne	0x0xd0c3
    d0c0:	7e d5 27    	jmp	0xd527
    d0c3:	b6 01 1e    	ldaa	0x11e
    d0c6:	81 13       	cmpa	#0x13
    d0c8:	26 2b       	bne	0x0xd0f5
    d0ca:	b6 01 1a    	ldaa	0x11a
    d0cd:	2b 1f       	bmi	0x0xd0ee
    d0cf:	96 50       	ldaa	*0x50
    d0d1:	4c          	inca
    d0d2:	84 7f       	anda	#0x7f
    d0d4:	97 50       	staa	*0x50
    d0d6:	ce 01 31    	ldx	#0x131
    d0d9:	bd eb 8a    	jsr	0xeb8a
    d0dc:	7f 01 1a    	clr	0x11a
    d0df:	86 03       	ldaa	#0x3
    d0e1:	b7 01 1c    	staa	0x11c
    d0e4:	96 50       	ldaa	*0x50
    d0e6:	c6 50       	ldab	#0x50
    d0e8:	bd b0 fc    	jsr	0xb0fc
    d0eb:	7e d5 27    	jmp	0xd527
    d0ee:	96 50       	ldaa	*0x50
    d0f0:	4a          	deca
    d0f1:	84 7f       	anda	#0x7f
    d0f3:	20 df       	bra	0x0xd0d4
    d0f5:	81 19       	cmpa	#0x19
    d0f7:	26 2b       	bne	0x0xd124
    d0f9:	b6 01 1a    	ldaa	0x11a
    d0fc:	2b 1f       	bmi	0x0xd11d
    d0fe:	96 98       	ldaa	*0x98
    d100:	4c          	inca
    d101:	84 7f       	anda	#0x7f
    d103:	97 98       	staa	*0x98
    d105:	ce 01 37    	ldx	#0x137
    d108:	bd eb 8a    	jsr	0xeb8a
    d10b:	7f 01 1a    	clr	0x11a
    d10e:	86 03       	ldaa	#0x3
    d110:	b7 01 1c    	staa	0x11c
    d113:	96 98       	ldaa	*0x98
    d115:	c6 98       	ldab	#0x98
    d117:	bd b0 fc    	jsr	0xb0fc
    d11a:	7e d5 27    	jmp	0xd527
    d11d:	96 98       	ldaa	*0x98
    d11f:	4a          	deca
    d120:	84 7f       	anda	#0x7f
    d122:	20 df       	bra	0x0xd103
    d124:	7f 01 1a    	clr	0x11a
    d127:	7e d5 27    	jmp	0xd527
    d12a:	7d 01 1c    	tst	0x11c
    d12d:	2a 03       	bpl	0x0xd132
    d12f:	7e e7 db    	jmp	0xe7db
    d132:	7d 01 1e    	tst	0x11e
    d135:	26 08       	bne	0x0xd13f
    d137:	7d 01 1c    	tst	0x11c
    d13a:	27 1f       	beq	0x0xd15b
    d13c:	7e b2 70    	jmp	0xb270
    d13f:	7d 01 1c    	tst	0x11c
    d142:	27 1f       	beq	0x0xd163
    d144:	cc 01 20    	ldd	#0x120
    d147:	fb 01 1e    	addb	0x11e
    d14a:	8f          	xgdx
    d14b:	bd ea 76    	jsr	0xea76
    d14e:	7a 01 1e    	dec	0x11e
    d151:	7a 01 1c    	dec	0x11c
    d154:	26 0d       	bne	0x0xd163
    d156:	bd eb 27    	jsr	0xeb27
    d159:	20 08       	bra	0x0xd163
    d15b:	86 16       	ldaa	#0x16
    d15d:	b7 01 1e    	staa	0x11e
    d160:	bd ea b2    	jsr	0xeab2
    d163:	7f 01 7e    	clr	0x17e
    d166:	7d 01 7f    	tst	0x17f
    d169:	27 10       	beq	0x0xd17b
    d16b:	bd ed 28    	jsr	0xed28
    d16e:	4f          	clra
    d16f:	b7 01 7f    	staa	0x17f
    d172:	b7 01 7e    	staa	0x17e
    d175:	b7 01 1a    	staa	0x11a
    d178:	7e d5 27    	jmp	0xd527
    d17b:	7d 01 1a    	tst	0x11a
    d17e:	26 03       	bne	0x0xd183
    d180:	7e d5 27    	jmp	0xd527
    d183:	ce 50 01    	ldx	#0x5001
    d186:	b6 01 6b    	ldaa	0x16b
    d189:	c6 04       	ldab	#0x4
    d18b:	3d          	mul
    d18c:	3a          	abx
    d18d:	b6 01 1e    	ldaa	0x11e
    d190:	81 12       	cmpa	#0x12
    d192:	26 69       	bne	0x0xd1fd
    d194:	a6 00       	ldaa	0x0,x
    d196:	7d 01 1a    	tst	0x11a
    d199:	2b 5c       	bmi	0x0xd1f7
    d19b:	4c          	inca
    d19c:	81 04       	cmpa	#0x4
    d19e:	25 02       	bcs	0x0xd1a2
    d1a0:	86 03       	ldaa	#0x3
    d1a2:	a7 00       	staa	0x0,x
    d1a4:	8b 41       	adda	#0x41
    d1a6:	b7 01 32    	staa	0x132
    d1a9:	7f 01 1a    	clr	0x11a
    d1ac:	86 01       	ldaa	#0x1
    d1ae:	b7 01 1c    	staa	0x11c
    d1b1:	a6 01       	ldaa	0x1,x
    d1b3:	e6 00       	ldab	0x0,x
    d1b5:	c1 01       	cmpb	#0x1
    d1b7:	23 18       	bls	0x0xd1d1
    d1b9:	d7 f9       	stab	*0xf9
    d1bb:	bd ad c4    	jsr	0xadc4
    d1be:	c6 04       	ldab	#0x4
    d1c0:	d7 f9       	stab	*0xf9
    d1c2:	bd a5 50    	jsr	0xa550
    d1c5:	bd a5 6b    	jsr	0xa56b
    d1c8:	bd ad c4    	jsr	0xadc4
    d1cb:	bd a8 3b    	jsr	0xa83b
    d1ce:	7e d2 d2    	jmp	0xd2d2
    d1d1:	4f          	clra
    d1d2:	b7 10 22    	staa	0x1022
    d1d5:	b6 10 2d    	ldaa	0x102d
    d1d8:	36          	psha
    d1d9:	84 7f       	anda	#0x7f
    d1db:	b7 10 2d    	staa	0x102d
    d1de:	a6 00       	ldaa	0x0,x
    d1e0:	e6 01       	ldab	0x1,x
    d1e2:	bd 79 00    	jsr	0x7900
    d1e5:	32          	pula
    d1e6:	b7 10 2d    	staa	0x102d
    d1e9:	86 80       	ldaa	#0x80
    d1eb:	b7 10 22    	staa	0x1022
    d1ee:	bd a5 6b    	jsr	0xa56b
    d1f1:	bd a8 3b    	jsr	0xa83b
    d1f4:	7e d2 d2    	jmp	0xd2d2
    d1f7:	4a          	deca
    d1f8:	2a a8       	bpl	0x0xd1a2
    d1fa:	4f          	clra
    d1fb:	20 a5       	bra	0x0xd1a2
    d1fd:	81 16       	cmpa	#0x16
    d1ff:	26 55       	bne	0x0xd256
    d201:	a6 01       	ldaa	0x1,x
    d203:	7d 01 1a    	tst	0x11a
    d206:	2b 3c       	bmi	0x0xd244
    d208:	4c          	inca
    d209:	84 7f       	anda	#0x7f
    d20b:	81 51       	cmpa	#0x51
    d20d:	26 04       	bne	0x0xd213
    d20f:	86 53       	ldaa	#0x53
    d211:	20 06       	bra	0x0xd219
    d213:	81 52       	cmpa	#0x52
    d215:	26 02       	bne	0x0xd219
    d217:	86 53       	ldaa	#0x53
    d219:	a7 01       	staa	0x1,x
    d21b:	4c          	inca
    d21c:	3c          	pshx
    d21d:	ce 01 34    	ldx	#0x134
    d220:	bd eb 8a    	jsr	0xeb8a
    d223:	38          	pulx
    d224:	a6 01       	ldaa	0x1,x
    d226:	e6 00       	ldab	0x0,x
    d228:	c1 01       	cmpb	#0x1
    d22a:	23 a5       	bls	0x0xd1d1
    d22c:	d7 f9       	stab	*0xf9
    d22e:	bd ad c4    	jsr	0xadc4
    d231:	c6 04       	ldab	#0x4
    d233:	d7 f9       	stab	*0xf9
    d235:	bd a5 50    	jsr	0xa550
    d238:	bd a5 6b    	jsr	0xa56b
    d23b:	bd ad c4    	jsr	0xadc4
    d23e:	bd a8 3b    	jsr	0xa83b
    d241:	7e d2 d2    	jmp	0xd2d2
    d244:	4a          	deca
    d245:	84 7f       	anda	#0x7f
    d247:	81 52       	cmpa	#0x52
    d249:	26 04       	bne	0x0xd24f
    d24b:	86 50       	ldaa	#0x50
    d24d:	20 ca       	bra	0x0xd219
    d24f:	81 51       	cmpa	#0x51
    d251:	26 c6       	bne	0x0xd219
    d253:	4a          	deca
    d254:	20 c3       	bra	0x0xd219
    d256:	81 1a       	cmpa	#0x1a
    d258:	26 3e       	bne	0x0xd298
    d25a:	a6 02       	ldaa	0x2,x
    d25c:	84 40       	anda	#0x40
    d25e:	b7 01 7d    	staa	0x17d
    d261:	a6 02       	ldaa	0x2,x
    d263:	84 3f       	anda	#0x3f
    d265:	f6 50 00    	ldab	0x5000
    d268:	c1 06       	cmpb	#0x6
    d26a:	27 02       	beq	0x0xd26e
    d26c:	20 64       	bra	0x0xd2d2
    d26e:	7d 01 1a    	tst	0x11a
    d271:	2b 19       	bmi	0x0xd28c
    d273:	4c          	inca
    d274:	7a 50 23    	dec	0x5023
    d277:	2a 04       	bpl	0x0xd27d
    d279:	4a          	deca
    d27a:	7c 50 23    	inc	0x5023
    d27d:	ba 01 7d    	oraa	0x17d
    d280:	a7 02       	staa	0x2,x
    d282:	84 3f       	anda	#0x3f
    d284:	ce 01 38    	ldx	#0x138
    d287:	bd eb 8a    	jsr	0xeb8a
    d28a:	20 46       	bra	0x0xd2d2
    d28c:	7c 50 23    	inc	0x5023
    d28f:	4a          	deca
    d290:	2a eb       	bpl	0x0xd27d
    d292:	4c          	inca
    d293:	7a 50 23    	dec	0x5023
    d296:	20 e5       	bra	0x0xd27d
    d298:	f6 50 00    	ldab	0x5000
    d29b:	c1 06       	cmpb	#0x6
    d29d:	27 22       	beq	0x0xd2c1
    d29f:	a6 03       	ldaa	0x3,x
    d2a1:	7d 01 1a    	tst	0x11a
    d2a4:	2b 18       	bmi	0x0xd2be
    d2a6:	4c          	inca
    d2a7:	84 7f       	anda	#0x7f
    d2a9:	a7 03       	staa	0x3,x
    d2ab:	36          	psha
    d2ac:	ce 01 3c    	ldx	#0x13c
    d2af:	bd eb 8a    	jsr	0xeb8a
    d2b2:	32          	pula
    d2b3:	d6 d6       	ldab	*0xd6
    d2b5:	3d          	mul
    d2b6:	05          	asld
    d2b7:	c6 ac       	ldab	#0xac
    d2b9:	bd b0 fc    	jsr	0xb0fc
    d2bc:	20 14       	bra	0x0xd2d2
    d2be:	4a          	deca
    d2bf:	20 e6       	bra	0x0xd2a7
    d2c1:	e6 03       	ldab	0x3,x
    d2c3:	5c          	incb
    d2c4:	c4 01       	andb	#0x1
    d2c6:	e7 03       	stab	0x3,x
    d2c8:	ce e9 05    	ldx	#0xe905
    d2cb:	18 ce 01 3c 	ldy	#0x13c
    d2cf:	bd ec 29    	jsr	0xec29
    d2d2:	7f 01 1a    	clr	0x11a
    d2d5:	86 03       	ldaa	#0x3
    d2d7:	b7 01 1c    	staa	0x11c
    d2da:	14 fb 80    	bset	*0xfb, #0x80
    d2dd:	7e d5 27    	jmp	0xd527
    d2e0:	7d 01 1c    	tst	0x11c
    d2e3:	2a 03       	bpl	0x0xd2e8
    d2e5:	7e e9 0b    	jmp	0xe90b
    d2e8:	7d 01 1e    	tst	0x11e
    d2eb:	26 08       	bne	0x0xd2f5
    d2ed:	7d 01 1c    	tst	0x11c
    d2f0:	27 1f       	beq	0x0xd311
    d2f2:	7e b2 70    	jmp	0xb270
    d2f5:	7d 01 1c    	tst	0x11c
    d2f8:	27 1f       	beq	0x0xd319
    d2fa:	cc 01 20    	ldd	#0x120
    d2fd:	fb 01 1e    	addb	0x11e
    d300:	8f          	xgdx
    d301:	bd ea 76    	jsr	0xea76
    d304:	7a 01 1e    	dec	0x11e
    d307:	7a 01 1c    	dec	0x11c
    d30a:	26 0d       	bne	0x0xd319
    d30c:	bd ea f4    	jsr	0xeaf4
    d30f:	20 08       	bra	0x0xd319
    d311:	86 13       	ldaa	#0x13
    d313:	b7 01 1e    	staa	0x11e
    d316:	bd ea b2    	jsr	0xeab2
    d319:	7d 01 7f    	tst	0x17f
    d31c:	27 0d       	beq	0x0xd32b
    d31e:	4f          	clra
    d31f:	b7 01 7f    	staa	0x17f
    d322:	b7 01 7e    	staa	0x17e
    d325:	b7 01 1a    	staa	0x11a
    d328:	7e d5 27    	jmp	0xd527
    d32b:	7d 01 7e    	tst	0x17e
    d32e:	27 0c       	beq	0x0xd33c
    d330:	7f 01 7f    	clr	0x17f
    d333:	7f 01 7e    	clr	0x17e
    d336:	7f 01 1a    	clr	0x11a
    d339:	7e d5 27    	jmp	0xd527
    d33c:	7d 01 1a    	tst	0x11a
    d33f:	26 03       	bne	0x0xd344
    d341:	7e d5 27    	jmp	0xd527
    d344:	b6 01 6b    	ldaa	0x16b
    d347:	c6 04       	ldab	#0x4
    d349:	3d          	mul
    d34a:	ce 50 03    	ldx	#0x5003
    d34d:	3a          	abx
    d34e:	e6 00       	ldab	0x0,x
    d350:	c4 3f       	andb	#0x3f
    d352:	f7 01 7d    	stab	0x17d
    d355:	e6 00       	ldab	0x0,x
    d357:	c4 40       	andb	#0x40
    d359:	c8 40       	eorb	#0x40
    d35b:	37          	pshb
    d35c:	fa 01 7d    	orab	0x17d
    d35f:	e7 00       	stab	0x0,x
    d361:	32          	pula
    d362:	4d          	tsta
    d363:	27 02       	beq	0x0xd367
    d365:	86 01       	ldaa	#0x1
    d367:	36          	psha
    d368:	4d          	tsta
    d369:	26 31       	bne	0x0xd39c
    d36b:	7d 00 d0    	tst	0xd0
    d36e:	27 05       	beq	0x0xd375
    d370:	7f 00 d0    	clr	0xd0
    d373:	20 08       	bra	0x0xd37d
    d375:	7d 10 29    	tst	0x1029
    d378:	2a fb       	bpl	0x0xd375
    d37a:	f6 10 2a    	ldab	0x102a
    d37d:	f6 01 6b    	ldab	0x16b
    d380:	ce 50 50    	ldx	#0x5050
    d383:	3a          	abx
    d384:	e6 00       	ldab	0x0,x
    d386:	53          	comb
    d387:	01          	nop
    d388:	01          	nop
    d389:	01          	nop
    d38a:	01          	nop
    d38b:	f7 10 42    	stab	0x1042
    d38e:	01          	nop
    d38f:	01          	nop
    d390:	01          	nop
    d391:	01          	nop
    d392:	01          	nop
    d393:	01          	nop
    d394:	01          	nop
    d395:	c6 8c       	ldab	#0x8c
    d397:	f7 10 2a    	stab	0x102a
    d39a:	20 07       	bra	0x0xd3a3
    d39c:	c6 44       	ldab	#0x44
    d39e:	96 44       	ldaa	*0x44
    d3a0:	bd b0 fc    	jsr	0xb0fc
    d3a3:	33          	pulb
    d3a4:	18 ce 01 31 	ldy	#0x131
    d3a8:	ce e9 7c    	ldx	#0xe97c
    d3ab:	bd ec 29    	jsr	0xec29
    d3ae:	7f 01 1a    	clr	0x11a
    d3b1:	86 03       	ldaa	#0x3
    d3b3:	b7 01 1c    	staa	0x11c
    d3b6:	14 fb 80    	bset	*0xfb, #0x80
    d3b9:	7e d5 27    	jmp	0xd527
    d3bc:	7d 01 1c    	tst	0x11c
    d3bf:	2a 03       	bpl	0x0xd3c4
    d3c1:	7e e9 c3    	jmp	0xe9c3
    d3c4:	7d 01 1e    	tst	0x11e
    d3c7:	26 08       	bne	0x0xd3d1
    d3c9:	7d 01 1c    	tst	0x11c
    d3cc:	27 24       	beq	0x0xd3f2
    d3ce:	7e b2 70    	jmp	0xb270
    d3d1:	7d 01 1c    	tst	0x11c
    d3d4:	27 24       	beq	0x0xd3fa
    d3d6:	cc 01 20    	ldd	#0x120
    d3d9:	fb 01 1e    	addb	0x11e
    d3dc:	8f          	xgdx
    d3dd:	bd ea 76    	jsr	0xea76
    d3e0:	7a 01 1e    	dec	0x11e
    d3e3:	7a 01 1c    	dec	0x11c
    d3e6:	26 12       	bne	0x0xd3fa
    d3e8:	86 1e       	ldaa	#0x1e
    d3ea:	b7 01 1e    	staa	0x11e
    d3ed:	bd ea b2    	jsr	0xeab2
    d3f0:	20 08       	bra	0x0xd3fa
    d3f2:	86 1e       	ldaa	#0x1e
    d3f4:	b7 01 1e    	staa	0x11e
    d3f7:	bd ea b2    	jsr	0xeab2
    d3fa:	7d 01 1a    	tst	0x11a
    d3fd:	26 0a       	bne	0x0xd409
    d3ff:	4f          	clra
    d400:	b7 01 7f    	staa	0x17f
    d403:	b7 01 7e    	staa	0x17e
    d406:	7e d5 27    	jmp	0xd527
    d409:	86 0c       	ldaa	#0xc
    d40b:	b7 10 2d    	staa	0x102d
    d40e:	4f          	clra
    d40f:	97 f2       	staa	*0xf2
    d411:	97 f3       	staa	*0xf3
    d413:	97 f4       	staa	*0xf4
    d415:	97 f5       	staa	*0xf5
    d417:	97 f6       	staa	*0xf6
    d419:	97 f7       	staa	*0xf7
    d41b:	97 f8       	staa	*0xf8
    d41d:	bd d4 d1    	jsr	0xd4d1
    d420:	4f          	clra
    d421:	b7 10 22    	staa	0x1022
    d424:	96 99       	ldaa	*0x99
    d426:	80 41       	suba	#0x41
    d428:	97 f9       	staa	*0xf9
    d42a:	bd ad c4    	jsr	0xadc4
    d42d:	96 9a       	ldaa	*0x9a
    d42f:	80 41       	suba	#0x41
    d431:	36          	psha
    d432:	ce f9 c1    	ldx	#0xf9c1
    d435:	18 ce 78 00 	ldy	#0x7800
    d439:	a6 00       	ldaa	0x0,x
    d43b:	18 a7 00    	staa	0x0,y
    d43e:	18 08       	iny
    d440:	08          	inx
    d441:	8c fb bf    	cpx	#0xfbbf
    d444:	23 f3       	bls	0x0xd439
    d446:	18 ce 00 00 	ldy	#0x0
    d44a:	ce d4 5e    	ldx	#0xd45e
    d44d:	a6 00       	ldaa	0x0,x
    d44f:	18 a7 00    	staa	0x0,y
    d452:	18 08       	iny
    d454:	08          	inx
    d455:	8c d4 d0    	cpx	#0xd4d0
    d458:	25 f3       	bcs	0x0xd44d
    d45a:	0f          	sei
    d45b:	7e 00 00    	jmp	0x0
    d45e:	32          	pula
    d45f:	4d          	tsta
    d460:	26 0c       	bne	0x0xd46e
    d462:	b6 10 00    	ldaa	0x1000
    d465:	84 9f       	anda	#0x9f
    d467:	8a 20       	oraa	#0x20
    d469:	b7 10 00    	staa	0x1000
    d46c:	20 0a       	bra	0x0xd478
    d46e:	b6 10 00    	ldaa	0x1000
    d471:	84 9f       	anda	#0x9f
    d473:	8a 40       	oraa	#0x40
    d475:	b7 10 00    	staa	0x1000
    d478:	ce 20 00    	ldx	#0x2000
    d47b:	18 ce 80 00 	ldy	#0x8000
    d47f:	86 aa       	ldaa	#0xaa
    d481:	b7 d5 55    	staa	0xd555
    d484:	86 55       	ldaa	#0x55
    d486:	b7 aa aa    	staa	0xaaaa
    d489:	86 a0       	ldaa	#0xa0
    d48b:	b7 d5 55    	staa	0xd555
    d48e:	86 80       	ldaa	#0x80
    d490:	e6 00       	ldab	0x0,x
    d492:	18 e7 00    	stab	0x0,y
    d495:	08          	inx
    d496:	18 08       	iny
    d498:	4a          	deca
    d499:	26 f5       	bne	0x0xd490
    d49b:	86 08       	ldaa	#0x8
    d49d:	b7 10 23    	staa	0x1023
    d4a0:	fc 10 0e    	ldd	0x100e
    d4a3:	c3 5d c0    	addd	#0x5dc0
    d4a6:	fd 10 1e    	std	0x101e
    d4a9:	01          	nop
    d4aa:	01          	nop
    d4ab:	b6 10 23    	ldaa	0x1023
    d4ae:	85 08       	bita	#0x8
    d4b0:	27 f7       	beq	0x0xd4a9
    d4b2:	8c 78 00    	cpx	#0x7800
    d4b5:	25 c8       	bcs	0x0xd47f
    d4b7:	22 06       	bhi	0x0xd4bf
    d4b9:	18 ce fe 00 	ldy	#0xfe00
    d4bd:	20 c0       	bra	0x0xd47f
    d4bf:	18 8c 00 00 	cpy	#0x0
    d4c3:	26 ba       	bne	0x0xd47f
    d4c5:	b6 10 00    	ldaa	0x1000
    d4c8:	84 8f       	anda	#0x8f
    d4ca:	b7 10 00    	staa	0x1000
    d4cd:	7e 80 00    	jmp	0x8000
    d4d0:	01          	nop
    d4d1:	ce 10 23    	ldx	#0x1023
    d4d4:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd4d4
    d4d8:	bd ea d7    	jsr	0xead7
    d4db:	ce 01 20    	ldx	#0x120
    d4de:	18 ce d5 07 	ldy	#0xd507
    d4e2:	c6 20       	ldab	#0x20
    d4e4:	18 a6 00    	ldaa	0x0,y
    d4e7:	a7 00       	staa	0x0,x
    d4e9:	08          	inx
    d4ea:	18 08       	iny
    d4ec:	5a          	decb
    d4ed:	26 f5       	bne	0x0xd4e4
    d4ef:	7f 01 1e    	clr	0x11e
    d4f2:	86 20       	ldaa	#0x20
    d4f4:	b7 01 1c    	staa	0x11c
    d4f7:	ce 10 23    	ldx	#0x1023
    d4fa:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd4fa
    d4fe:	bd ea 2b    	jsr	0xea2b
    d501:	7d 01 1c    	tst	0x11c
    d504:	26 f1       	bne	0x0xd4f7
    d506:	39          	rts
    d507:	20 57       	bra	0x0xd560
    d509:	52          	.byte	0x52
    d50a:	49          	rola
    d50b:	54          	lsrb
    d50c:	49          	rola
    d50d:	4e          	.byte	0x4e
    d50e:	47          	asra
    d50f:	20 42       	bra	0x0xd553
    d511:	41          	.byte	0x41
    d512:	4e          	.byte	0x4e
    d513:	4b          	.byte	0x4b
    d514:	20 2e       	bra	0x0xd544
    d516:	2e 20       	bgt	0x0xd538
    d518:	54          	lsrb
    d519:	41          	.byte	0x41
    d51a:	4b          	.byte	0x4b
    d51b:	45          	.byte	0x45
    d51c:	20 35       	bra	0x0xd553
    d51e:	20 2e       	bra	0x0xd54e
    d520:	2e 2e       	bgt	0x0xd550
    d522:	2e 2e       	bgt	0x0xd552
    d524:	2e 2e       	bgt	0x0xd554
    d526:	2e fe       	bgt	0x0xd526
    d528:	01          	nop
    d529:	c0 bc       	subb	#0xbc
    d52b:	01          	nop
    d52c:	c2 26       	sbcb	#0x26
    d52e:	03          	fdiv
    d52f:	7e 97 90    	jmp	0x9790
    d532:	18 fe 01 6c 	ldy	0x16c
    d536:	18 3c       	pshy
    d538:	7e 92 be    	jmp	0x92be
    d53b:	7f 00 ff    	clr	0xff
    d53e:	7d 01 1d    	tst	0x11d
    d541:	27 2f       	beq	0x0xd572
    d543:	ce 10 23    	ldx	#0x1023
    d546:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd546
    d54a:	86 f7       	ldaa	#0xf7
    d54c:	b4 10 00    	anda	0x1000
    d54f:	b7 10 00    	staa	0x1000
    d552:	86 0c       	ldaa	#0xc
    d554:	b7 10 47    	staa	0x1047
    d557:	86 80       	ldaa	#0x80
    d559:	ba 10 00    	oraa	0x1000
    d55c:	b7 10 00    	staa	0x1000
    d55f:	01          	nop
    d560:	88 80       	eora	#0x80
    d562:	b7 10 00    	staa	0x1000
    d565:	7f 01 1d    	clr	0x11d
    d568:	bd ea a3    	jsr	0xeaa3
    d56b:	ce 10 23    	ldx	#0x1023
    d56e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd56e
    d572:	bd ea d7    	jsr	0xead7
    d575:	ce 01 20    	ldx	#0x120
    d578:	c6 10       	ldab	#0x10
    d57a:	96 f9       	ldaa	*0xf9
    d57c:	81 04       	cmpa	#0x4
    d57e:	25 06       	bcs	0x0xd586
    d580:	18 ce d5 f5 	ldy	#0xd5f5
    d584:	20 07       	bra	0x0xd58d
    d586:	14 d1 80    	bset	*0xd1, #0x80
    d589:	18 ce d5 e5 	ldy	#0xd5e5
    d58d:	18 a6 00    	ldaa	0x0,y
    d590:	a7 00       	staa	0x0,x
    d592:	08          	inx
    d593:	18 08       	iny
    d595:	5a          	decb
    d596:	26 f5       	bne	0x0xd58d
    d598:	96 f9       	ldaa	*0xf9
    d59a:	81 04       	cmpa	#0x4
    d59c:	27 30       	beq	0x0xd5ce
    d59e:	8b 41       	adda	#0x41
    d5a0:	b7 01 2c    	staa	0x12c
    d5a3:	96 f9       	ldaa	*0xf9
    d5a5:	81 01       	cmpa	#0x1
    d5a7:	23 05       	bls	0x0xd5ae
    d5a9:	86 41       	ldaa	#0x41
    d5ab:	b7 01 28    	staa	0x128
    d5ae:	96 fa       	ldaa	*0xfa
    d5b0:	4c          	inca
    d5b1:	ce 01 2d    	ldx	#0x12d
    d5b4:	bd eb 8a    	jsr	0xeb8a
    d5b7:	08          	inx
    d5b8:	18 ce 00 c0 	ldy	#0xc0
    d5bc:	c6 10       	ldab	#0x10
    d5be:	18 a6 00    	ldaa	0x0,y
    d5c1:	84 7f       	anda	#0x7f
    d5c3:	a7 00       	staa	0x0,x
    d5c5:	08          	inx
    d5c6:	18 08       	iny
    d5c8:	5a          	decb
    d5c9:	26 f3       	bne	0x0xd5be
    d5cb:	7e ea 14    	jmp	0xea14
    d5ce:	b6 01 6a    	ldaa	0x16a
    d5d1:	4c          	inca
    d5d2:	ce 01 2d    	ldx	#0x12d
    d5d5:	bd eb 8a    	jsr	0xeb8a
    d5d8:	08          	inx
    d5d9:	cc 50 00    	ldd	#0x5000
    d5dc:	c3 00 40    	addd	#0x40
    d5df:	18 8f       	xgdy
    d5e1:	c6 10       	ldab	#0x10
    d5e3:	20 d9       	bra	0x0xd5be
    d5e5:	50          	negb
    d5e6:	41          	.byte	0x41
    d5e7:	54          	lsrb
    d5e8:	43          	coma
    d5e9:	48          	asla
    d5ea:	20 20       	bra	0x0xd60c
    d5ec:	52          	.byte	0x52
    d5ed:	4f          	clra
    d5ee:	4d          	tsta
    d5ef:	20 20       	bra	0x0xd611
    d5f1:	20 20       	bra	0x0xd613
    d5f3:	20 20       	bra	0x0xd615
    d5f5:	4d          	tsta
    d5f6:	55          	.byte	0x55
    d5f7:	4c          	inca
    d5f8:	54          	lsrb
    d5f9:	49          	rola
    d5fa:	20 20       	bra	0x0xd61c
    d5fc:	20 20       	bra	0x0xd61e
    d5fe:	20 20       	bra	0x0xd620
    d600:	20 20       	bra	0x0xd622
    d602:	20 20       	bra	0x0xd624
    d604:	20 7d       	bra	0x0xd683
    d606:	01          	nop
    d607:	7e 27 61    	jmp	0x2761
    d60a:	7f 01 7e    	clr	0x17e
    d60d:	96 f9       	ldaa	*0xf9
    d60f:	81 04       	cmpa	#0x4
    d611:	25 1e       	bcs	0x0xd631
    d613:	8b 41       	adda	#0x41
    d615:	b7 01 2c    	staa	0x12c
    d618:	b6 01 6a    	ldaa	0x16a
    d61b:	4c          	inca
    d61c:	ce 01 2d    	ldx	#0x12d
    d61f:	bd eb 8a    	jsr	0xeb8a
    d622:	b6 01 6a    	ldaa	0x16a
    d625:	c6 50       	ldab	#0x50
    d627:	3d          	mul
    d628:	c3 20 00    	addd	#0x2000
    d62b:	c3 00 40    	addd	#0x40
    d62e:	8f          	xgdx
    d62f:	20 1a       	bra	0x0xd64b
    d631:	8b 41       	adda	#0x41
    d633:	b7 01 2c    	staa	0x12c
    d636:	96 fa       	ldaa	*0xfa
    d638:	4c          	inca
    d639:	ce 01 2d    	ldx	#0x12d
    d63c:	bd eb 8a    	jsr	0xeb8a
    d63f:	96 fa       	ldaa	*0xfa
    d641:	c6 b0       	ldab	#0xb0
    d643:	3d          	mul
    d644:	c3 20 00    	addd	#0x2000
    d647:	c3 00 a0    	addd	#0xa0
    d64a:	8f          	xgdx
    d64b:	18 ce 01 30 	ldy	#0x130
    d64f:	c6 10       	ldab	#0x10
    d651:	a6 00       	ldaa	0x0,x
    d653:	84 7f       	anda	#0x7f
    d655:	18 a7 00    	staa	0x0,y
    d658:	08          	inx
    d659:	18 08       	iny
    d65b:	5a          	decb
    d65c:	26 f3       	bne	0x0xd651
    d65e:	ce 10 23    	ldx	#0x1023
    d661:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd661
    d665:	bd ea d7    	jsr	0xead7
    d668:	7e ea 14    	jmp	0xea14
    d66b:	7d 01 1d    	tst	0x11d
    d66e:	26 0a       	bne	0x0xd67a
    d670:	bd ec 01    	jsr	0xec01
    d673:	ce 10 23    	ldx	#0x1023
    d676:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd676
    d67a:	bd ea d7    	jsr	0xead7
    d67d:	ce 01 20    	ldx	#0x120
    d680:	18 ce d7 0a 	ldy	#0xd70a
    d684:	c6 20       	ldab	#0x20
    d686:	18 a6 00    	ldaa	0x0,y
    d689:	a7 00       	staa	0x0,x
    d68b:	08          	inx
    d68c:	18 08       	iny
    d68e:	5a          	decb
    d68f:	26 f5       	bne	0x0xd686
    d691:	96 f9       	ldaa	*0xf9
    d693:	8b 41       	adda	#0x41
    d695:	b7 01 25    	staa	0x125
    d698:	96 f9       	ldaa	*0xf9
    d69a:	81 02       	cmpa	#0x2
    d69c:	24 07       	bcc	0x0xd6a5
    d69e:	86 02       	ldaa	#0x2
    d6a0:	97 f9       	staa	*0xf9
    d6a2:	bd ad c4    	jsr	0xadc4
    d6a5:	8b 41       	adda	#0x41
    d6a7:	b7 01 2c    	staa	0x12c
    d6aa:	96 fa       	ldaa	*0xfa
    d6ac:	7d 00 d1    	tst	0xd1
    d6af:	2b 3a       	bmi	0x0xd6eb
    d6b1:	b6 01 6a    	ldaa	0x16a
    d6b4:	7d 00 fb    	tst	0xfb
    d6b7:	27 32       	beq	0x0xd6eb
    d6b9:	2b 30       	bmi	0x0xd6eb
    d6bb:	86 04       	ldaa	#0x4
    d6bd:	97 f9       	staa	*0xf9
    d6bf:	bd ad c4    	jsr	0xadc4
    d6c2:	ce 50 01    	ldx	#0x5001
    d6c5:	b6 01 6b    	ldaa	0x16b
    d6c8:	c6 04       	ldab	#0x4
    d6ca:	3d          	mul
    d6cb:	3a          	abx
    d6cc:	a6 00       	ldaa	0x0,x
    d6ce:	8b 41       	adda	#0x41
    d6d0:	b7 01 25    	staa	0x125
    d6d3:	a6 01       	ldaa	0x1,x
    d6d5:	36          	psha
    d6d6:	a6 00       	ldaa	0x0,x
    d6d8:	81 02       	cmpa	#0x2
    d6da:	24 02       	bcc	0x0xd6de
    d6dc:	86 02       	ldaa	#0x2
    d6de:	97 f9       	staa	*0xf9
    d6e0:	bd ad c4    	jsr	0xadc4
    d6e3:	8b 41       	adda	#0x41
    d6e5:	b7 01 2c    	staa	0x12c
    d6e8:	32          	pula
    d6e9:	97 fa       	staa	*0xfa
    d6eb:	4c          	inca
    d6ec:	ce 01 26    	ldx	#0x126
    d6ef:	bd eb 8a    	jsr	0xeb8a
    d6f2:	a7 07       	staa	0x7,x
    d6f4:	09          	dex
    d6f5:	a6 00       	ldaa	0x0,x
    d6f7:	a7 07       	staa	0x7,x
    d6f9:	09          	dex
    d6fa:	a6 00       	ldaa	0x0,x
    d6fc:	a7 07       	staa	0x7,x
    d6fe:	96 f9       	ldaa	*0xf9
    d700:	81 04       	cmpa	#0x4
    d702:	27 03       	beq	0x0xd707
    d704:	7e d6 3f    	jmp	0xd63f
    d707:	7e d5 ce    	jmp	0xd5ce
    d70a:	53          	comb
    d70b:	41          	.byte	0x41
    d70c:	56          	rorb
    d70d:	45          	.byte	0x45
    d70e:	20 20       	bra	0x0xd730
    d710:	20 20       	bra	0x0xd732
    d712:	20 20       	bra	0x0xd734
    d714:	3e          	wai
    d715:	20 20       	bra	0x0xd737
    d717:	20 20       	bra	0x0xd739
    d719:	20 20       	bra	0x0xd73b
    d71b:	20 20       	bra	0x0xd73d
    d71d:	20 20       	bra	0x0xd73f
    d71f:	20 20       	bra	0x0xd741
    d721:	20 20       	bra	0x0xd743
    d723:	20 20       	bra	0x0xd745
    d725:	20 20       	bra	0x0xd747
    d727:	20 20       	bra	0x0xd749
    d729:	20 7d       	bra	0x0xd7a8
    d72b:	01          	nop
    d72c:	1d 26 0a    	bclr	0x26,x, #0x0a
    d72f:	bd ec 01    	jsr	0xec01
    d732:	ce 10 23    	ldx	#0x1023
    d735:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd735
    d739:	bd ea d7    	jsr	0xead7
    d73c:	ce 01 20    	ldx	#0x120
    d73f:	18 ce d7 7e 	ldy	#0xd77e
    d743:	c6 20       	ldab	#0x20
    d745:	96 f9       	ldaa	*0xf9
    d747:	81 04       	cmpa	#0x4
    d749:	26 04       	bne	0x0xd74f
    d74b:	18 ce d7 9e 	ldy	#0xd79e
    d74f:	18 a6 00    	ldaa	0x0,y
    d752:	a7 00       	staa	0x0,x
    d754:	08          	inx
    d755:	18 08       	iny
    d757:	5a          	decb
    d758:	26 f5       	bne	0x0xd74f
    d75a:	96 f9       	ldaa	*0xf9
    d75c:	81 04       	cmpa	#0x4
    d75e:	27 11       	beq	0x0xd771
    d760:	8b 41       	adda	#0x41
    d762:	b7 01 2c    	staa	0x12c
    d765:	96 fa       	ldaa	*0xfa
    d767:	4c          	inca
    d768:	ce 01 2d    	ldx	#0x12d
    d76b:	bd eb 8a    	jsr	0xeb8a
    d76e:	7e d5 b7    	jmp	0xd5b7
    d771:	b6 01 6a    	ldaa	0x16a
    d774:	4c          	inca
    d775:	ce 01 2d    	ldx	#0x12d
    d778:	bd eb 8a    	jsr	0xeb8a
    d77b:	7e d5 ce    	jmp	0xd5ce
    d77e:	4e          	.byte	0x4e
    d77f:	41          	.byte	0x41
    d780:	4d          	tsta
    d781:	45          	.byte	0x45
    d782:	20 50       	bra	0x0xd7d4
    d784:	41          	.byte	0x41
    d785:	54          	lsrb
    d786:	43          	coma
    d787:	48          	asla
    d788:	20 20       	bra	0x0xd7aa
    d78a:	20 20       	bra	0x0xd7ac
    d78c:	20 20       	bra	0x0xd7ae
    d78e:	20 20       	bra	0x0xd7b0
    d790:	20 20       	bra	0x0xd7b2
    d792:	20 20       	bra	0x0xd7b4
    d794:	20 20       	bra	0x0xd7b6
    d796:	20 20       	bra	0x0xd7b8
    d798:	20 20       	bra	0x0xd7ba
    d79a:	20 20       	bra	0x0xd7bc
    d79c:	20 20       	bra	0x0xd7be
    d79e:	4e          	.byte	0x4e
    d79f:	41          	.byte	0x41
    d7a0:	4d          	tsta
    d7a1:	45          	.byte	0x45
    d7a2:	20 4d       	bra	0x0xd7f1
    d7a4:	55          	.byte	0x55
    d7a5:	4c          	inca
    d7a6:	54          	lsrb
    d7a7:	49          	rola
    d7a8:	20 20       	bra	0x0xd7ca
    d7aa:	20 20       	bra	0x0xd7cc
    d7ac:	20 20       	bra	0x0xd7ce
    d7ae:	20 20       	bra	0x0xd7d0
    d7b0:	20 20       	bra	0x0xd7d2
    d7b2:	20 20       	bra	0x0xd7d4
    d7b4:	20 20       	bra	0x0xd7d6
    d7b6:	20 20       	bra	0x0xd7d8
    d7b8:	20 20       	bra	0x0xd7da
    d7ba:	20 20       	bra	0x0xd7dc
    d7bc:	20 20       	bra	0x0xd7de
    d7be:	bd ea d7    	jsr	0xead7
    d7c1:	ce 01 20    	ldx	#0x120
    d7c4:	18 ce d7 fc 	ldy	#0xd7fc
    d7c8:	c6 20       	ldab	#0x20
    d7ca:	18 a6 00    	ldaa	0x0,y
    d7cd:	a7 00       	staa	0x0,x
    d7cf:	08          	inx
    d7d0:	18 08       	iny
    d7d2:	5a          	decb
    d7d3:	26 f5       	bne	0x0xd7ca
    d7d5:	ce d8 1c    	ldx	#0xd81c
    d7d8:	f6 01 6f    	ldab	0x16f
    d7db:	18 ce 01 30 	ldy	#0x130
    d7df:	bd ec 3d    	jsr	0xec3d
    d7e2:	ce d8 60    	ldx	#0xd860
    d7e5:	d6 93       	ldab	*0x93
    d7e7:	18 ce 01 36 	ldy	#0x136
    d7eb:	bd ec 3d    	jsr	0xec3d
    d7ee:	ce d8 74    	ldx	#0xd874
    d7f1:	5f          	clrb
    d7f2:	18 ce 01 3d 	ldy	#0x13d
    d7f6:	bd ec 29    	jsr	0xec29
    d7f9:	7e ea 14    	jmp	0xea14
    d7fc:	43          	coma
    d7fd:	48          	asla
    d7fe:	41          	.byte	0x41
    d7ff:	4e          	.byte	0x4e
    d800:	20 20       	bra	0x0xd822
    d802:	54          	lsrb
    d803:	55          	.byte	0x55
    d804:	4e          	.byte	0x4e
    d805:	45          	.byte	0x45
    d806:	20 20       	bra	0x0xd828
    d808:	49          	rola
    d809:	4e          	.byte	0x4e
    d80a:	49          	rola
    d80b:	54          	lsrb
    d80c:	20 20       	bra	0x0xd82e
    d80e:	20 20       	bra	0x0xd830
    d810:	20 20       	bra	0x0xd832
    d812:	20 20       	bra	0x0xd834
    d814:	20 20       	bra	0x0xd836
    d816:	20 20       	bra	0x0xd838
    d818:	20 20       	bra	0x0xd83a
    d81a:	20 20       	bra	0x0xd83c
    d81c:	20 20       	bra	0x0xd83e
    d81e:	20 31       	bra	0x0xd851
    d820:	20 20       	bra	0x0xd842
    d822:	20 32       	bra	0x0xd856
    d824:	20 20       	bra	0x0xd846
    d826:	20 33       	bra	0x0xd85b
    d828:	20 20       	bra	0x0xd84a
    d82a:	20 34       	bra	0x0xd860
    d82c:	20 20       	bra	0x0xd84e
    d82e:	20 35       	bra	0x0xd865
    d830:	20 20       	bra	0x0xd852
    d832:	20 36       	bra	0x0xd86a
    d834:	20 20       	bra	0x0xd856
    d836:	20 37       	bra	0x0xd86f
    d838:	20 20       	bra	0x0xd85a
    d83a:	20 38       	bra	0x0xd874
    d83c:	20 20       	bra	0x0xd85e
    d83e:	20 39       	bra	0x0xd879
    d840:	20 20       	bra	0x0xd862
    d842:	31          	ins
    d843:	30          	tsx
    d844:	20 20       	bra	0x0xd866
    d846:	31          	ins
    d847:	31          	ins
    d848:	20 20       	bra	0x0xd86a
    d84a:	31          	ins
    d84b:	32          	pula
    d84c:	20 20       	bra	0x0xd86e
    d84e:	31          	ins
    d84f:	33          	pulb
    d850:	20 20       	bra	0x0xd872
    d852:	31          	ins
    d853:	34          	des
    d854:	20 20       	bra	0x0xd876
    d856:	31          	ins
    d857:	35          	txs
    d858:	20 20       	bra	0x0xd87a
    d85a:	31          	ins
    d85b:	36          	psha
    d85c:	4f          	clra
    d85d:	4d          	tsta
    d85e:	4e          	.byte	0x4e
    d85f:	49          	rola
    d860:	20 4f       	bra	0x0xd8b1
    d862:	46          	rora
    d863:	46          	rora
    d864:	20 39       	bra	0x0xd89f
    d866:	30          	tsx
    d867:	25 20       	bcs	0x0xd889
    d869:	39          	rts
    d86a:	35          	txs
    d86b:	25 20       	bcs	0x0xd88d
    d86d:	39          	rts
    d86e:	38          	pulx
    d86f:	25 31       	bcs	0x0xd8a2
    d871:	30          	tsx
    d872:	30          	tsx
    d873:	25 20       	bcs	0x0xd895
    d875:	4e          	.byte	0x4e
    d876:	4f          	clra
    d877:	59          	rolb
    d878:	45          	.byte	0x45
    d879:	53          	comb
    d87a:	bd ea d7    	jsr	0xead7
    d87d:	ce 01 20    	ldx	#0x120
    d880:	18 ce d8 b6 	ldy	#0xd8b6
    d884:	c6 20       	ldab	#0x20
    d886:	18 a6 00    	ldaa	0x0,y
    d889:	a7 00       	staa	0x0,x
    d88b:	08          	inx
    d88c:	18 08       	iny
    d88e:	5a          	decb
    d88f:	26 f5       	bne	0x0xd886
    d891:	ce d8 d6    	ldx	#0xd8d6
    d894:	f6 01 70    	ldab	0x170
    d897:	18 ce 01 30 	ldy	#0x130
    d89b:	bd ec 3d    	jsr	0xec3d
    d89e:	ce d8 e2    	ldx	#0xd8e2
    d8a1:	d6 94       	ldab	*0x94
    d8a3:	18 ce 01 36 	ldy	#0x136
    d8a7:	bd ec 3d    	jsr	0xec3d
    d8aa:	b6 01 71    	ldaa	0x171
    d8ad:	ce 01 3c    	ldx	#0x13c
    d8b0:	bd eb 8a    	jsr	0xeb8a
    d8b3:	7e ea 14    	jmp	0xea14
    d8b6:	4d          	tsta
    d8b7:	50          	negb
    d8b8:	52          	.byte	0x52
    d8b9:	4f          	clra
    d8ba:	20 20       	bra	0x0xd8dc
    d8bc:	4b          	.byte	0x4b
    d8bd:	4e          	.byte	0x4e
    d8be:	4f          	clra
    d8bf:	42          	.byte	0x42
    d8c0:	20 20       	bra	0x0xd8e2
    d8c2:	4c          	inca
    d8c3:	43          	coma
    d8c4:	44          	lsra
    d8c5:	20 20       	bra	0x0xd8e7
    d8c7:	20 20       	bra	0x0xd8e9
    d8c9:	20 20       	bra	0x0xd8eb
    d8cb:	20 20       	bra	0x0xd8ed
    d8cd:	20 20       	bra	0x0xd8ef
    d8cf:	20 20       	bra	0x0xd8f1
    d8d1:	20 20       	bra	0x0xd8f3
    d8d3:	20 20       	bra	0x0xd8f5
    d8d5:	20 20       	bra	0x0xd8f7
    d8d7:	4f          	clra
    d8d8:	46          	rora
    d8d9:	46          	rora
    d8da:	4f          	clra
    d8db:	4e          	.byte	0x4e
    d8dc:	20 31       	bra	0x0xd90f
    d8de:	4f          	clra
    d8df:	4e          	.byte	0x4e
    d8e0:	20 32       	bra	0x0xd914
    d8e2:	4a          	deca
    d8e3:	55          	.byte	0x55
    d8e4:	4d          	tsta
    d8e5:	50          	negb
    d8e6:	45          	.byte	0x45
    d8e7:	44          	lsra
    d8e8:	49          	rola
    d8e9:	54          	lsrb
    d8ea:	4d          	tsta
    d8eb:	54          	lsrb
    d8ec:	43          	coma
    d8ed:	48          	asla
    d8ee:	bd ea d7    	jsr	0xead7
    d8f1:	96 f9       	ldaa	*0xf9
    d8f3:	81 04       	cmpa	#0x4
    d8f5:	27 0b       	beq	0x0xd902
    d8f7:	ce 01 20    	ldx	#0x120
    d8fa:	18 ce d9 39 	ldy	#0xd939
    d8fe:	c6 20       	ldab	#0x20
    d900:	20 09       	bra	0x0xd90b
    d902:	ce 01 20    	ldx	#0x120
    d905:	18 ce d9 59 	ldy	#0xd959
    d909:	c6 20       	ldab	#0x20
    d90b:	18 a6 00    	ldaa	0x0,y
    d90e:	a7 00       	staa	0x0,x
    d910:	08          	inx
    d911:	18 08       	iny
    d913:	5a          	decb
    d914:	26 f5       	bne	0x0xd90b
    d916:	96 f9       	ldaa	*0xf9
    d918:	81 04       	cmpa	#0x4
    d91a:	26 0c       	bne	0x0xd928
    d91c:	b6 01 6a    	ldaa	0x16a
    d91f:	4c          	inca
    d920:	ce 01 3c    	ldx	#0x13c
    d923:	bd eb 8a    	jsr	0xeb8a
    d926:	20 0e       	bra	0x0xd936
    d928:	8b 41       	adda	#0x41
    d92a:	b7 01 3b    	staa	0x13b
    d92d:	96 fa       	ldaa	*0xfa
    d92f:	4c          	inca
    d930:	ce 01 3c    	ldx	#0x13c
    d933:	bd eb 8a    	jsr	0xeb8a
    d936:	7e ea 14    	jmp	0xea14
    d939:	53          	comb
    d93a:	59          	rolb
    d93b:	53          	comb
    d93c:	58          	aslb
    d93d:	20 53       	bra	0x0xd992
    d93f:	45          	.byte	0x45
    d940:	4e          	.byte	0x4e
    d941:	44          	lsra
    d942:	2c 50       	bge	0x0xd994
    d944:	49          	rola
    d945:	43          	coma
    d946:	4b          	.byte	0x4b
    d947:	23 2c       	bls	0x0xd975
    d949:	50          	negb
    d94a:	52          	.byte	0x52
    d94b:	45          	.byte	0x45
    d94c:	53          	comb
    d94d:	53          	comb
    d94e:	20 53       	bra	0x0xd9a3
    d950:	41          	.byte	0x41
    d951:	56          	rorb
    d952:	45          	.byte	0x45
    d953:	20 20       	bra	0x0xd975
    d955:	20 20       	bra	0x0xd977
    d957:	20 20       	bra	0x0xd979
    d959:	53          	comb
    d95a:	45          	.byte	0x45
    d95b:	4c          	inca
    d95c:	45          	.byte	0x45
    d95d:	43          	coma
    d95e:	54          	lsrb
    d95f:	20 4d       	bra	0x0xd9ae
    d961:	55          	.byte	0x55
    d962:	4c          	inca
    d963:	54          	lsrb
    d964:	49          	rola
    d965:	2c 20       	bge	0x0xd987
    d967:	20 20       	bra	0x0xd989
    d969:	50          	negb
    d96a:	52          	.byte	0x52
    d96b:	45          	.byte	0x45
    d96c:	53          	comb
    d96d:	53          	comb
    d96e:	20 53       	bra	0x0xd9c3
    d970:	41          	.byte	0x41
    d971:	56          	rorb
    d972:	45          	.byte	0x45
    d973:	20 20       	bra	0x0xd995
    d975:	20 20       	bra	0x0xd997
    d977:	20 20       	bra	0x0xd999
    d979:	7d 01 1d    	tst	0x11d
    d97c:	26 0a       	bne	0x0xd988
    d97e:	bd ec 01    	jsr	0xec01
    d981:	ce 10 23    	ldx	#0x1023
    d984:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd984
    d988:	bd ea d7    	jsr	0xead7
    d98b:	ce 01 20    	ldx	#0x120
    d98e:	18 ce d9 c0 	ldy	#0xd9c0
    d992:	c6 20       	ldab	#0x20
    d994:	18 a6 00    	ldaa	0x0,y
    d997:	a7 00       	staa	0x0,x
    d999:	08          	inx
    d99a:	18 08       	iny
    d99c:	5a          	decb
    d99d:	26 f5       	bne	0x0xd994
    d99f:	ce 01 31    	ldx	#0x131
    d9a2:	96 40       	ldaa	*0x40
    d9a4:	bd eb c6    	jsr	0xebc6
    d9a7:	ce d9 e0    	ldx	#0xd9e0
    d9aa:	d6 41       	ldab	*0x41
    d9ac:	c4 0f       	andb	#0xf
    d9ae:	18 ce 01 36 	ldy	#0x136
    d9b2:	bd ec 3d    	jsr	0xec3d
    d9b5:	ce 01 3c    	ldx	#0x13c
    d9b8:	96 67       	ldaa	*0x67
    d9ba:	bd eb 8a    	jsr	0xeb8a
    d9bd:	7e ea 14    	jmp	0xea14
    d9c0:	46          	rora
    d9c1:	49          	rola
    d9c2:	4e          	.byte	0x4e
    d9c3:	45          	.byte	0x45
    d9c4:	20 4d       	bra	0x0xda13
    d9c6:	4f          	clra
    d9c7:	44          	lsra
    d9c8:	45          	.byte	0x45
    d9c9:	32          	pula
    d9ca:	20 45       	bra	0x0xda11
    d9cc:	4e          	.byte	0x4e
    d9cd:	56          	rorb
    d9ce:	31          	ins
    d9cf:	20 20       	bra	0x0xd9f1
    d9d1:	20 20       	bra	0x0xd9f3
    d9d3:	20 20       	bra	0x0xd9f5
    d9d5:	20 20       	bra	0x0xd9f7
    d9d7:	20 20       	bra	0x0xd9f9
    d9d9:	20 20       	bra	0x0xd9fb
    d9db:	20 20       	bra	0x0xd9fd
    d9dd:	20 20       	bra	0x0xd9ff
    d9df:	20 4e       	bra	0x0xda2f
    d9e1:	4f          	clra
    d9e2:	52          	.byte	0x52
    d9e3:	4d          	tsta
    d9e4:	48          	asla
    d9e5:	41          	.byte	0x41
    d9e6:	4c          	inca
    d9e7:	46          	rora
    d9e8:	4e          	.byte	0x4e
    d9e9:	4f          	clra
    d9ea:	43          	coma
    d9eb:	56          	rorb
    d9ec:	4c          	inca
    d9ed:	4f          	clra
    d9ee:	57          	asrb
    d9ef:	31          	ins
    d9f0:	4c          	inca
    d9f1:	4f          	clra
    d9f2:	57          	asrb
    d9f3:	32          	pula
    d9f4:	7d 01 1d    	tst	0x11d
    d9f7:	26 0a       	bne	0x0xda03
    d9f9:	bd ec 01    	jsr	0xec01
    d9fc:	ce 10 23    	ldx	#0x1023
    d9ff:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd9ff
    da03:	bd ea d7    	jsr	0xead7
    da06:	ce 01 20    	ldx	#0x120
    da09:	18 ce da 36 	ldy	#0xda36
    da0d:	c6 20       	ldab	#0x20
    da0f:	18 a6 00    	ldaa	0x0,y
    da12:	a7 00       	staa	0x0,x
    da14:	08          	inx
    da15:	18 08       	iny
    da17:	5a          	decb
    da18:	26 f5       	bne	0x0xda0f
    da1a:	ce 01 31    	ldx	#0x131
    da1d:	b6 01 6e    	ldaa	0x16e
    da20:	bd eb c6    	jsr	0xebc6
    da23:	ce 01 37    	ldx	#0x137
    da26:	96 3e       	ldaa	*0x3e
    da28:	bd eb 8a    	jsr	0xeb8a
    da2b:	08          	inx
    da2c:	08          	inx
    da2d:	08          	inx
    da2e:	96 3f       	ldaa	*0x3f
    da30:	bd eb 8a    	jsr	0xeb8a
    da33:	7e ea 14    	jmp	0xea14
    da36:	54          	lsrb
    da37:	55          	.byte	0x55
    da38:	4e          	.byte	0x4e
    da39:	45          	.byte	0x45
    da3a:	20 20       	bra	0x0xda5c
    da3c:	4f          	clra
    da3d:	53          	comb
    da3e:	43          	coma
    da3f:	31          	ins
    da40:	20 4f       	bra	0x0xda91
    da42:	53          	comb
    da43:	43          	coma
    da44:	32          	pula
    da45:	20 20       	bra	0x0xda67
    da47:	20 20       	bra	0x0xda69
    da49:	20 20       	bra	0x0xda6b
    da4b:	20 20       	bra	0x0xda6d
    da4d:	20 20       	bra	0x0xda6f
    da4f:	20 20       	bra	0x0xda71
    da51:	20 20       	bra	0x0xda73
    da53:	20 20       	bra	0x0xda75
    da55:	20 7d       	bra	0x0xdad4
    da57:	01          	nop
    da58:	1d 26 0a    	bclr	0x26,x, #0x0a
    da5b:	bd ec 01    	jsr	0xec01
    da5e:	ce 10 23    	ldx	#0x1023
    da61:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xda61
    da65:	bd ea d7    	jsr	0xead7
    da68:	ce 01 20    	ldx	#0x120
    da6b:	18 ce da c4 	ldy	#0xdac4
    da6f:	c6 20       	ldab	#0x20
    da71:	18 a6 00    	ldaa	0x0,y
    da74:	a7 00       	staa	0x0,x
    da76:	08          	inx
    da77:	18 08       	iny
    da79:	5a          	decb
    da7a:	26 f5       	bne	0x0xda71
    da7c:	ce da e4    	ldx	#0xdae4
    da7f:	18 ce 01 30 	ldy	#0x130
    da83:	13 21 40 07 	brclr	*0x21, #0x40, 0x0xda8e
    da87:	c6 01       	ldab	#0x1
    da89:	bd ec 29    	jsr	0xec29
    da8c:	20 04       	bra	0x0xda92
    da8e:	5f          	clrb
    da8f:	bd ec 29    	jsr	0xec29
    da92:	18 ce 01 34 	ldy	#0x134
    da96:	ce da ea    	ldx	#0xdaea
    da99:	5f          	clrb
    da9a:	bd ec 29    	jsr	0xec29
    da9d:	20 00       	bra	0x0xda9f
    da9f:	ce da f0    	ldx	#0xdaf0
    daa2:	18 ce 01 38 	ldy	#0x138
    daa6:	12 21 01 06 	brset	*0x21, #0x01, 0x0xdab0
    daaa:	5f          	clrb
    daab:	bd ec 29    	jsr	0xec29
    daae:	20 05       	bra	0x0xdab5
    dab0:	c6 01       	ldab	#0x1
    dab2:	bd ec 29    	jsr	0xec29
    dab5:	ce da f6    	ldx	#0xdaf6
    dab8:	d6 24       	ldab	*0x24
    daba:	18 ce 01 3c 	ldy	#0x13c
    dabe:	bd ec 29    	jsr	0xec29
    dac1:	7e ea 14    	jmp	0xea14
    dac4:	20 4f       	bra	0x0xdb15
    dac6:	4e          	.byte	0x4e
    dac7:	20 54       	bra	0x0xdb1d
    dac9:	59          	rolb
    daca:	50          	negb
    dacb:	20 4d       	bra	0x0xdb1a
    dacd:	44          	lsra
    dace:	45          	.byte	0x45
    dacf:	20 44       	bra	0x0xdb15
    dad1:	45          	.byte	0x45
    dad2:	53          	comb
    dad3:	20 20       	bra	0x0xdaf5
    dad5:	20 20       	bra	0x0xdaf7
    dad7:	20 20       	bra	0x0xdaf9
    dad9:	20 20       	bra	0x0xdafb
    dadb:	20 20       	bra	0x0xdafd
    dadd:	20 20       	bra	0x0xdaff
    dadf:	20 20       	bra	0x0xdb01
    dae1:	20 20       	bra	0x0xdb03
    dae3:	20 4f       	bra	0x0xdb34
    dae5:	46          	rora
    dae6:	46          	rora
    dae7:	20 4f       	bra	0x0xdb38
    dae9:	4e          	.byte	0x4e
    daea:	45          	.byte	0x45
    daeb:	58          	aslb
    daec:	50          	negb
    daed:	4c          	inca
    daee:	49          	rola
    daef:	4e          	.byte	0x4e
    daf0:	52          	.byte	0x52
    daf1:	45          	.byte	0x45
    daf2:	47          	asra
    daf3:	4c          	inca
    daf4:	45          	.byte	0x45
    daf5:	47          	asra
    daf6:	4f          	clra
    daf7:	26 46       	bne	0x0xdb3f
    daf9:	4f          	clra
    dafa:	53          	comb
    dafb:	31          	ins
    dafc:	4f          	clra
    dafd:	53          	comb
    dafe:	32          	pula
    daff:	31          	ins
    db00:	26 32       	bne	0x0xdb34
    db02:	31          	ins
    db03:	26 46       	bne	0x0xdb4b
    db05:	32          	pula
    db06:	26 46       	bne	0x0xdb4e
    db08:	46          	rora
    db09:	49          	rola
    db0a:	4c          	inca
    db0b:	7d 01 1d    	tst	0x11d
    db0e:	26 0a       	bne	0x0xdb1a
    db10:	bd ec 01    	jsr	0xec01
    db13:	ce 10 23    	ldx	#0x1023
    db16:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdb16
    db1a:	bd ea d7    	jsr	0xead7
    db1d:	ce 01 20    	ldx	#0x120
    db20:	18 ce db 5a 	ldy	#0xdb5a
    db24:	c6 20       	ldab	#0x20
    db26:	18 a6 00    	ldaa	0x0,y
    db29:	a7 00       	staa	0x0,x
    db2b:	08          	inx
    db2c:	18 08       	iny
    db2e:	5a          	decb
    db2f:	26 f5       	bne	0x0xdb26
    db31:	ce db 7a    	ldx	#0xdb7a
    db34:	d6 6a       	ldab	*0x6a
    db36:	c4 0f       	andb	#0xf
    db38:	18 ce 01 30 	ldy	#0x130
    db3c:	bd ec 3d    	jsr	0xec3d
    db3f:	ce db 8e    	ldx	#0xdb8e
    db42:	d6 95       	ldab	*0x95
    db44:	18 ce 01 36 	ldy	#0x136
    db48:	bd ec 3d    	jsr	0xec3d
    db4b:	ce db 96    	ldx	#0xdb96
    db4e:	d6 69       	ldab	*0x69
    db50:	18 ce 01 3b 	ldy	#0x13b
    db54:	bd ec 3d    	jsr	0xec3d
    db57:	7e ea 14    	jmp	0xea14
    db5a:	20 55       	bra	0x0xdbb1
    db5c:	4e          	.byte	0x4e
    db5d:	49          	rola
    db5e:	20 56       	bra	0x0xdbb6
    db60:	4d          	tsta
    db61:	4f          	clra
    db62:	44          	lsra
    db63:	45          	.byte	0x45
    db64:	20 50       	bra	0x0xdbb6
    db66:	52          	.byte	0x52
    db67:	49          	rola
    db68:	4f          	clra
    db69:	52          	.byte	0x52
    db6a:	20 20       	bra	0x0xdb8c
    db6c:	20 20       	bra	0x0xdb8e
    db6e:	20 20       	bra	0x0xdb90
    db70:	20 20       	bra	0x0xdb92
    db72:	20 20       	bra	0x0xdb94
    db74:	20 20       	bra	0x0xdb96
    db76:	20 20       	bra	0x0xdb98
    db78:	20 20       	bra	0x0xdb9a
    db7a:	20 4f       	bra	0x0xdbcb
    db7c:	4e          	.byte	0x4e
    db7d:	45          	.byte	0x45
    db7e:	20 54       	bra	0x0xdbd4
    db80:	57          	asrb
    db81:	4f          	clra
    db82:	46          	rora
    db83:	4f          	clra
    db84:	55          	.byte	0x55
    db85:	52          	.byte	0x52
    db86:	20 53       	bra	0x0xdbdb
    db88:	49          	rola
    db89:	58          	aslb
    db8a:	45          	.byte	0x45
    db8b:	47          	asra
    db8c:	48          	asla
    db8d:	54          	lsrb
    db8e:	43          	coma
    db8f:	59          	rolb
    db90:	43          	coma
    db91:	4c          	inca
    db92:	4e          	.byte	0x4e
    db93:	4f          	clra
    db94:	54          	lsrb
    db95:	45          	.byte	0x45
    db96:	4c          	inca
    db97:	41          	.byte	0x41
    db98:	53          	comb
    db99:	54          	lsrb
    db9a:	20 4c       	bra	0x0xdbe8
    db9c:	4f          	clra
    db9d:	57          	asrb
    db9e:	bd ea d7    	jsr	0xead7
    dba1:	ce 01 20    	ldx	#0x120
    dba4:	18 ce db d0 	ldy	#0xdbd0
    dba8:	c6 20       	ldab	#0x20
    dbaa:	18 a6 00    	ldaa	0x0,y
    dbad:	a7 00       	staa	0x0,x
    dbaf:	08          	inx
    dbb0:	18 08       	iny
    dbb2:	5a          	decb
    dbb3:	26 f5       	bne	0x0xdbaa
    dbb5:	ce db f0    	ldx	#0xdbf0
    dbb8:	d6 6b       	ldab	*0x6b
    dbba:	18 ce 01 30 	ldy	#0x130
    dbbe:	bd ec 3d    	jsr	0xec3d
    dbc1:	ce db fc    	ldx	#0xdbfc
    dbc4:	18 ce 01 36 	ldy	#0x136
    dbc8:	d6 68       	ldab	*0x68
    dbca:	bd ec 3d    	jsr	0xec3d
    dbcd:	7e ea 14    	jmp	0xea14
    dbd0:	4f          	clra
    dbd1:	43          	coma
    dbd2:	54          	lsrb
    dbd3:	41          	.byte	0x41
    dbd4:	56          	rorb
    dbd5:	20 4d       	bra	0x0xdc24
    dbd7:	54          	lsrb
    dbd8:	52          	.byte	0x52
    dbd9:	47          	asra
    dbda:	20 20       	bra	0x0xdbfc
    dbdc:	20 20       	bra	0x0xdbfe
    dbde:	20 20       	bra	0x0xdc00
    dbe0:	20 20       	bra	0x0xdc02
    dbe2:	20 20       	bra	0x0xdc04
    dbe4:	20 20       	bra	0x0xdc06
    dbe6:	20 20       	bra	0x0xdc08
    dbe8:	20 20       	bra	0x0xdc0a
    dbea:	20 20       	bra	0x0xdc0c
    dbec:	20 20       	bra	0x0xdc0e
    dbee:	20 20       	bra	0x0xdc10
    dbf0:	20 4c       	bra	0x0xdc3e
    dbf2:	4f          	clra
    dbf3:	57          	asrb
    dbf4:	20 4d       	bra	0x0xdc43
    dbf6:	49          	rola
    dbf7:	44          	lsra
    dbf8:	48          	asla
    dbf9:	49          	rola
    dbfa:	47          	asra
    dbfb:	48          	asla
    dbfc:	20 4f       	bra	0x0xdc4d
    dbfe:	46          	rora
    dbff:	46          	rora
    dc00:	45          	.byte	0x45
    dc01:	4e          	.byte	0x4e
    dc02:	56          	rorb
    dc03:	31          	ins
    dc04:	45          	.byte	0x45
    dc05:	4e          	.byte	0x4e
    dc06:	56          	rorb
    dc07:	32          	pula
    dc08:	45          	.byte	0x45
    dc09:	4e          	.byte	0x4e
    dc0a:	56          	rorb
    dc0b:	33          	pulb
    dc0c:	45          	.byte	0x45
    dc0d:	31          	ins
    dc0e:	26 32       	bne	0x0xdc42
    dc10:	45          	.byte	0x45
    dc11:	31          	ins
    dc12:	26 33       	bne	0x0xdc47
    dc14:	45          	.byte	0x45
    dc15:	32          	pula
    dc16:	26 33       	bne	0x0xdc4b
    dc18:	20 41       	bra	0x0xdc5b
    dc1a:	4c          	inca
    dc1b:	4c          	inca
    dc1c:	7d 01 1d    	tst	0x11d
    dc1f:	26 0a       	bne	0x0xdc2b
    dc21:	bd ec 01    	jsr	0xec01
    dc24:	ce 10 23    	ldx	#0x1023
    dc27:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdc27
    dc2b:	bd ea d7    	jsr	0xead7
    dc2e:	ce 01 20    	ldx	#0x120
    dc31:	18 ce dd 55 	ldy	#0xdd55
    dc35:	c6 20       	ldab	#0x20
    dc37:	18 a6 00    	ldaa	0x0,y
    dc3a:	a7 00       	staa	0x0,x
    dc3c:	08          	inx
    dc3d:	18 08       	iny
    dc3f:	5a          	decb
    dc40:	26 f5       	bne	0x0xdc37
    dc42:	96 f6       	ldaa	*0xf6
    dc44:	48          	asla
    dc45:	24 2b       	bcc	0x0xdc72
    dc47:	ce e0 4b    	ldx	#0xe04b
    dc4a:	d6 8a       	ldab	*0x8a
    dc4c:	18 ce 01 26 	ldy	#0x126
    dc50:	bd ec 3d    	jsr	0xec3d
    dc53:	ce e0 4b    	ldx	#0xe04b
    dc56:	d6 8b       	ldab	*0x8b
    dc58:	18 08       	iny
    dc5a:	18 08       	iny
    dc5c:	bd ec 3d    	jsr	0xec3d
    dc5f:	ce 01 37    	ldx	#0x137
    dc62:	96 8c       	ldaa	*0x8c
    dc64:	bd eb c6    	jsr	0xebc6
    dc67:	ce 01 3c    	ldx	#0x13c
    dc6a:	96 8d       	ldaa	*0x8d
    dc6c:	bd eb c6    	jsr	0xebc6
    dc6f:	7e ea 14    	jmp	0xea14
    dc72:	48          	asla
    dc73:	24 2b       	bcc	0x0xdca0
    dc75:	ce e0 4b    	ldx	#0xe04b
    dc78:	d6 86       	ldab	*0x86
    dc7a:	18 ce 01 26 	ldy	#0x126
    dc7e:	bd ec 3d    	jsr	0xec3d
    dc81:	ce e0 4b    	ldx	#0xe04b
    dc84:	d6 87       	ldab	*0x87
    dc86:	18 08       	iny
    dc88:	18 08       	iny
    dc8a:	bd ec 3d    	jsr	0xec3d
    dc8d:	ce 01 37    	ldx	#0x137
    dc90:	96 88       	ldaa	*0x88
    dc92:	bd eb 8a    	jsr	0xeb8a
    dc95:	08          	inx
    dc96:	08          	inx
    dc97:	08          	inx
    dc98:	96 89       	ldaa	*0x89
    dc9a:	bd eb 8a    	jsr	0xeb8a
    dc9d:	7e ea 14    	jmp	0xea14
    dca0:	48          	asla
    dca1:	24 2b       	bcc	0x0xdcce
    dca3:	ce e0 4b    	ldx	#0xe04b
    dca6:	d6 82       	ldab	*0x82
    dca8:	18 ce 01 26 	ldy	#0x126
    dcac:	bd ec 3d    	jsr	0xec3d
    dcaf:	ce e0 4b    	ldx	#0xe04b
    dcb2:	d6 83       	ldab	*0x83
    dcb4:	18 08       	iny
    dcb6:	18 08       	iny
    dcb8:	bd ec 3d    	jsr	0xec3d
    dcbb:	ce 01 37    	ldx	#0x137
    dcbe:	96 84       	ldaa	*0x84
    dcc0:	bd eb 8a    	jsr	0xeb8a
    dcc3:	08          	inx
    dcc4:	08          	inx
    dcc5:	08          	inx
    dcc6:	96 85       	ldaa	*0x85
    dcc8:	bd eb 8a    	jsr	0xeb8a
    dccb:	7e ea 14    	jmp	0xea14
    dcce:	48          	asla
    dccf:	24 2b       	bcc	0x0xdcfc
    dcd1:	ce e0 4b    	ldx	#0xe04b
    dcd4:	d6 7e       	ldab	*0x7e
    dcd6:	18 ce 01 26 	ldy	#0x126
    dcda:	bd ec 3d    	jsr	0xec3d
    dcdd:	ce e0 4b    	ldx	#0xe04b
    dce0:	d6 7f       	ldab	*0x7f
    dce2:	18 08       	iny
    dce4:	18 08       	iny
    dce6:	bd ec 3d    	jsr	0xec3d
    dce9:	ce 01 37    	ldx	#0x137
    dcec:	96 80       	ldaa	*0x80
    dcee:	bd eb 8a    	jsr	0xeb8a
    dcf1:	08          	inx
    dcf2:	08          	inx
    dcf3:	08          	inx
    dcf4:	96 81       	ldaa	*0x81
    dcf6:	bd eb 8a    	jsr	0xeb8a
    dcf9:	7e ea 14    	jmp	0xea14
    dcfc:	48          	asla
    dcfd:	24 2b       	bcc	0x0xdd2a
    dcff:	ce e0 4b    	ldx	#0xe04b
    dd02:	d6 7a       	ldab	*0x7a
    dd04:	18 ce 01 26 	ldy	#0x126
    dd08:	bd ec 3d    	jsr	0xec3d
    dd0b:	ce e0 4b    	ldx	#0xe04b
    dd0e:	d6 7b       	ldab	*0x7b
    dd10:	18 08       	iny
    dd12:	18 08       	iny
    dd14:	bd ec 3d    	jsr	0xec3d
    dd17:	ce 01 37    	ldx	#0x137
    dd1a:	96 7c       	ldaa	*0x7c
    dd1c:	bd eb 8a    	jsr	0xeb8a
    dd1f:	08          	inx
    dd20:	08          	inx
    dd21:	08          	inx
    dd22:	96 7d       	ldaa	*0x7d
    dd24:	bd eb 8a    	jsr	0xeb8a
    dd27:	7e ea 14    	jmp	0xea14
    dd2a:	ce e0 4b    	ldx	#0xe04b
    dd2d:	d6 76       	ldab	*0x76
    dd2f:	18 ce 01 26 	ldy	#0x126
    dd33:	bd ec 3d    	jsr	0xec3d
    dd36:	ce e0 4b    	ldx	#0xe04b
    dd39:	d6 77       	ldab	*0x77
    dd3b:	18 08       	iny
    dd3d:	18 08       	iny
    dd3f:	bd ec 3d    	jsr	0xec3d
    dd42:	ce 01 37    	ldx	#0x137
    dd45:	96 78       	ldaa	*0x78
    dd47:	bd eb 8a    	jsr	0xeb8a
    dd4a:	08          	inx
    dd4b:	08          	inx
    dd4c:	08          	inx
    dd4d:	96 79       	ldaa	*0x79
    dd4f:	bd eb 8a    	jsr	0xeb8a
    dd52:	7e ea 14    	jmp	0xea14
    dd55:	20 44       	bra	0x0xdd9b
    dd57:	45          	.byte	0x45
    dd58:	53          	comb
    dd59:	54          	lsrb
    dd5a:	20 20       	bra	0x0xdd7c
    dd5c:	20 20       	bra	0x0xdd7e
    dd5e:	20 20       	bra	0x0xdd80
    dd60:	20 20       	bra	0x0xdd82
    dd62:	20 20       	bra	0x0xdd84
    dd64:	20 20       	bra	0x0xdd86
    dd66:	41          	.byte	0x41
    dd67:	4d          	tsta
    dd68:	4e          	.byte	0x4e
    dd69:	54          	lsrb
    dd6a:	20 20       	bra	0x0xdd8c
    dd6c:	20 20       	bra	0x0xdd8e
    dd6e:	20 20       	bra	0x0xdd90
    dd70:	20 20       	bra	0x0xdd92
    dd72:	20 20       	bra	0x0xdd94
    dd74:	20 41       	bra	0x0xddb7
    dd76:	54          	lsrb
    dd77:	54          	lsrb
    dd78:	31          	ins
    dd79:	44          	lsra
    dd7a:	45          	.byte	0x45
    dd7b:	43          	coma
    dd7c:	31          	ins
    dd7d:	53          	comb
    dd7e:	55          	.byte	0x55
    dd7f:	53          	comb
    dd80:	31          	ins
    dd81:	52          	.byte	0x52
    dd82:	45          	.byte	0x45
    dd83:	4c          	inca
    dd84:	31          	ins
    dd85:	41          	.byte	0x41
    dd86:	4d          	tsta
    dd87:	54          	lsrb
    dd88:	31          	ins
    dd89:	41          	.byte	0x41
    dd8a:	54          	lsrb
    dd8b:	54          	lsrb
    dd8c:	32          	pula
    dd8d:	44          	lsra
    dd8e:	45          	.byte	0x45
    dd8f:	43          	coma
    dd90:	32          	pula
    dd91:	53          	comb
    dd92:	55          	.byte	0x55
    dd93:	53          	comb
    dd94:	32          	pula
    dd95:	52          	.byte	0x52
    dd96:	45          	.byte	0x45
    dd97:	4c          	inca
    dd98:	32          	pula
    dd99:	41          	.byte	0x41
    dd9a:	4d          	tsta
    dd9b:	54          	lsrb
    dd9c:	32          	pula
    dd9d:	41          	.byte	0x41
    dd9e:	54          	lsrb
    dd9f:	54          	lsrb
    dda0:	33          	pulb
    dda1:	44          	lsra
    dda2:	45          	.byte	0x45
    dda3:	43          	coma
    dda4:	33          	pulb
    dda5:	53          	comb
    dda6:	55          	.byte	0x55
    dda7:	53          	comb
    dda8:	33          	pulb
    dda9:	52          	.byte	0x52
    ddaa:	45          	.byte	0x45
    ddab:	4c          	inca
    ddac:	33          	pulb
    ddad:	41          	.byte	0x41
    ddae:	4d          	tsta
    ddaf:	54          	lsrb
    ddb0:	33          	pulb
    ddb1:	44          	lsra
    ddb2:	45          	.byte	0x45
    ddb3:	4c          	inca
    ddb4:	31          	ins
    ddb5:	44          	lsra
    ddb6:	45          	.byte	0x45
    ddb7:	4c          	inca
    ddb8:	33          	pulb
    ddb9:	7d 01 1d    	tst	0x11d
    ddbc:	26 0a       	bne	0x0xddc8
    ddbe:	bd ec 01    	jsr	0xec01
    ddc1:	ce 10 23    	ldx	#0x1023
    ddc4:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xddc4
    ddc8:	bd ea d7    	jsr	0xead7
    ddcb:	ce 01 20    	ldx	#0x120
    ddce:	18 ce dd fa 	ldy	#0xddfa
    ddd2:	c6 20       	ldab	#0x20
    ddd4:	18 a6 00    	ldaa	0x0,y
    ddd7:	a7 00       	staa	0x0,x
    ddd9:	08          	inx
    ddda:	18 08       	iny
    dddc:	5a          	decb
    dddd:	26 f5       	bne	0x0xddd4
    dddf:	ce de 1a    	ldx	#0xde1a
    dde2:	d6 8e       	ldab	*0x8e
    dde4:	18 ce 01 36 	ldy	#0x136
    dde8:	bd ec 3d    	jsr	0xec3d
    ddeb:	ce de 26    	ldx	#0xde26
    ddee:	d6 8f       	ldab	*0x8f
    ddf0:	18 08       	iny
    ddf2:	18 08       	iny
    ddf4:	bd ec 3d    	jsr	0xec3d
    ddf7:	7e ea 14    	jmp	0xea14
    ddfa:	43          	coma
    ddfb:	4e          	.byte	0x4e
    ddfc:	54          	lsrb
    ddfd:	52          	.byte	0x52
    ddfe:	4c          	inca
    ddff:	20 43       	bra	0x0xde44
    de01:	4f          	clra
    de02:	4e          	.byte	0x4e
    de03:	31          	ins
    de04:	20 43       	bra	0x0xde49
    de06:	4f          	clra
    de07:	4e          	.byte	0x4e
    de08:	32          	pula
    de09:	20 41       	bra	0x0xde4c
    de0b:	53          	comb
    de0c:	53          	comb
    de0d:	47          	asra
    de0e:	4e          	.byte	0x4e
    de0f:	20 20       	bra	0x0xde31
    de11:	20 20       	bra	0x0xde33
    de13:	20 20       	bra	0x0xde35
    de15:	20 20       	bra	0x0xde37
    de17:	20 20       	bra	0x0xde39
    de19:	20 42       	bra	0x0xde5d
    de1b:	52          	.byte	0x52
    de1c:	54          	lsrb
    de1d:	48          	asla
    de1e:	4d          	tsta
    de1f:	4f          	clra
    de20:	44          	lsra
    de21:	57          	asrb
    de22:	54          	lsrb
    de23:	55          	.byte	0x55
    de24:	43          	coma
    de25:	48          	asla
    de26:	20 44       	bra	0x0xde6c
    de28:	59          	rolb
    de29:	4e          	.byte	0x4e
    de2a:	54          	lsrb
    de2b:	52          	.byte	0x52
    de2c:	41          	.byte	0x41
    de2d:	4b          	.byte	0x4b
    de2e:	7d 01 1d    	tst	0x11d
    de31:	26 0a       	bne	0x0xde3d
    de33:	bd ec 01    	jsr	0xec01
    de36:	ce 10 23    	ldx	#0x1023
    de39:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xde39
    de3d:	bd ea d7    	jsr	0xead7
    de40:	ce 01 20    	ldx	#0x120
    de43:	18 ce de 7c 	ldy	#0xde7c
    de47:	c6 20       	ldab	#0x20
    de49:	18 a6 00    	ldaa	0x0,y
    de4c:	a7 00       	staa	0x0,x
    de4e:	08          	inx
    de4f:	18 08       	iny
    de51:	5a          	decb
    de52:	26 f5       	bne	0x0xde49
    de54:	f6 50 00    	ldab	0x5000
    de57:	c1 0a       	cmpb	#0xa
    de59:	25 04       	bcs	0x0xde5f
    de5b:	5f          	clrb
    de5c:	f7 50 00    	stab	0x5000
    de5f:	86 09       	ldaa	#0x9
    de61:	3d          	mul
    de62:	1b          	aba
    de63:	16          	tab
    de64:	ce de 9c    	ldx	#0xde9c
    de67:	3a          	abx
    de68:	18 ce 01 31 	ldy	#0x131
    de6c:	c6 09       	ldab	#0x9
    de6e:	a6 00       	ldaa	0x0,x
    de70:	18 a7 00    	staa	0x0,y
    de73:	08          	inx
    de74:	18 08       	iny
    de76:	5a          	decb
    de77:	26 f5       	bne	0x0xde6e
    de79:	7e ea 14    	jmp	0xea14
    de7c:	20 4d       	bra	0x0xdecb
    de7e:	55          	.byte	0x55
    de7f:	4c          	inca
    de80:	54          	lsrb
    de81:	49          	rola
    de82:	20 54       	bra	0x0xded8
    de84:	59          	rolb
    de85:	50          	negb
    de86:	45          	.byte	0x45
    de87:	3a          	abx
    de88:	20 20       	bra	0x0xdeaa
    de8a:	20 20       	bra	0x0xdeac
    de8c:	20 20       	bra	0x0xdeae
    de8e:	20 20       	bra	0x0xdeb0
    de90:	20 20       	bra	0x0xdeb2
    de92:	20 20       	bra	0x0xdeb4
    de94:	20 20       	bra	0x0xdeb6
    de96:	20 20       	bra	0x0xdeb8
    de98:	20 20       	bra	0x0xdeba
    de9a:	20 20       	bra	0x0xdebc
    de9c:	50          	negb
    de9d:	52          	.byte	0x52
    de9e:	45          	.byte	0x45
    de9f:	50          	negb
    dea0:	41          	.byte	0x41
    dea1:	52          	.byte	0x52
    dea2:	45          	.byte	0x45
    dea3:	44          	lsra
    dea4:	20 53       	bra	0x0xdef9
    dea6:	50          	negb
    dea7:	4c          	inca
    dea8:	49          	rola
    dea9:	54          	lsrb
    deaa:	20 31       	bra	0x0xdedd
    deac:	2b 37       	bmi	0x0xdee5
    deae:	53          	comb
    deaf:	50          	negb
    deb0:	4c          	inca
    deb1:	49          	rola
    deb2:	54          	lsrb
    deb3:	20 32       	bra	0x0xdee7
    deb5:	2b 36       	bmi	0x0xdeed
    deb7:	53          	comb
    deb8:	50          	negb
    deb9:	4c          	inca
    deba:	49          	rola
    debb:	54          	lsrb
    debc:	20 33       	bra	0x0xdef1
    debe:	2b 35       	bmi	0x0xdef5
    dec0:	53          	comb
    dec1:	50          	negb
    dec2:	4c          	inca
    dec3:	49          	rola
    dec4:	54          	lsrb
    dec5:	20 34       	bra	0x0xdefb
    dec7:	2b 34       	bmi	0x0xdefd
    dec9:	4c          	inca
    deca:	41          	.byte	0x41
    decb:	59          	rolb
    decc:	45          	.byte	0x45
    decd:	52          	.byte	0x52
    dece:	20 34       	bra	0x0xdf04
    ded0:	2b 34       	bmi	0x0xdf06
    ded2:	4d          	tsta
    ded3:	55          	.byte	0x55
    ded4:	4c          	inca
    ded5:	54          	lsrb
    ded6:	49          	rola
    ded7:	43          	coma
    ded8:	48          	asla
    ded9:	41          	.byte	0x41
    deda:	4e          	.byte	0x4e
    dedb:	01          	nop
    dedc:	02          	idiv
    dedd:	04          	lsrd
    dede:	08          	inx
    dedf:	10          	sba
    dee0:	20 40       	bra	0x0xdf22
    dee2:	80 01       	suba	#0x1
    dee4:	fe 00 00    	ldx	0x0
    dee7:	00          	bgnd
    dee8:	00          	bgnd
    dee9:	00          	bgnd
    deea:	00          	bgnd
    deeb:	03          	fdiv
    deec:	fc 00 00    	ldd	0x0
    deef:	00          	bgnd
    def0:	00          	bgnd
    def1:	00          	bgnd
    def2:	00          	bgnd
    def3:	07          	tpa
    def4:	f8 00 00    	eorb	0x0
    def7:	00          	bgnd
    def8:	00          	bgnd
    def9:	00          	bgnd
    defa:	00          	bgnd
    defb:	0f          	sei
    defc:	f0 00 00    	subb	0x0
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
    df0b:	03          	fdiv
    df0c:	fc 00 00    	ldd	0x0
    df0f:	00          	bgnd
    df10:	00          	bgnd
    df11:	00          	bgnd
    df12:	00          	bgnd
    df13:	01          	nop
    df14:	02          	idiv
    df15:	fc 00 00    	ldd	0x0
    df18:	00          	bgnd
    df19:	00          	bgnd
    df1a:	00          	bgnd
    df1b:	01          	nop
    df1c:	02          	idiv
    df1d:	04          	lsrd
    df1e:	08          	inx
    df1f:	10          	sba
    df20:	20 40       	bra	0x0xdf62
    df22:	80 00       	suba	#0x0
    df24:	01          	nop
    df25:	03          	fdiv
    df26:	07          	tpa
    df27:	0f          	sei
    df28:	1f 3f 7f ff 	brclr	0x3f,x, #0x7f, 0x0xdf2b
    df2c:	01          	nop
    df2d:	02          	idiv
    df2e:	00          	bgnd
    df2f:	00          	bgnd
    df30:	00          	bgnd
    df31:	00          	bgnd
    df32:	00          	bgnd
    df33:	00          	bgnd
    df34:	03          	fdiv
    df35:	0c          	clc
    df36:	00          	bgnd
    df37:	00          	bgnd
    df38:	00          	bgnd
    df39:	00          	bgnd
    df3a:	00          	bgnd
    df3b:	00          	bgnd
    df3c:	07          	tpa
    df3d:	38          	pulx
    df3e:	00          	bgnd
    df3f:	00          	bgnd
    df40:	00          	bgnd
    df41:	00          	bgnd
    df42:	00          	bgnd
    df43:	00          	bgnd
    df44:	0f          	sei
    df45:	f0 00 00    	subb	0x0
    df48:	00          	bgnd
    df49:	00          	bgnd
    df4a:	00          	bgnd
    df4b:	00          	bgnd
    df4c:	bd ea d7    	jsr	0xead7
    df4f:	ce 01 20    	ldx	#0x120
    df52:	18 ce df 6f 	ldy	#0xdf6f
    df56:	c6 20       	ldab	#0x20
    df58:	18 a6 00    	ldaa	0x0,y
    df5b:	a7 00       	staa	0x0,x
    df5d:	08          	inx
    df5e:	18 08       	iny
    df60:	5a          	decb
    df61:	26 f5       	bne	0x0xdf58
    df63:	b6 50 21    	ldaa	0x5021
    df66:	ce 01 3c    	ldx	#0x13c
    df69:	bd eb 8a    	jsr	0xeb8a
    df6c:	7e ea 14    	jmp	0xea14
    df6f:	53          	comb
    df70:	50          	negb
    df71:	4c          	inca
    df72:	49          	rola
    df73:	54          	lsrb
    df74:	20 50       	bra	0x0xdfc6
    df76:	4f          	clra
    df77:	49          	rola
    df78:	4e          	.byte	0x4e
    df79:	54          	lsrb
    df7a:	3a          	abx
    df7b:	20 20       	bra	0x0xdf9d
    df7d:	20 20       	bra	0x0xdf9f
    df7f:	4d          	tsta
    df80:	49          	rola
    df81:	44          	lsra
    df82:	49          	rola
    df83:	20 4e       	bra	0x0xdfd3
    df85:	4f          	clra
    df86:	54          	lsrb
    df87:	45          	.byte	0x45
    df88:	20 23       	bra	0x0xdfad
    df8a:	20 20       	bra	0x0xdfac
    df8c:	20 20       	bra	0x0xdfae
    df8e:	20 7d       	bra	0x0xe00d
    df90:	01          	nop
    df91:	1d 26 0a    	bclr	0x26,x, #0x0a
    df94:	bd ec 01    	jsr	0xec01
    df97:	ce 10 23    	ldx	#0x1023
    df9a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdf9a
    df9e:	bd ea d7    	jsr	0xead7
    dfa1:	ce 01 20    	ldx	#0x120
    dfa4:	18 ce df ca 	ldy	#0xdfca
    dfa8:	c6 20       	ldab	#0x20
    dfaa:	18 a6 00    	ldaa	0x0,y
    dfad:	a7 00       	staa	0x0,x
    dfaf:	08          	inx
    dfb0:	18 08       	iny
    dfb2:	5a          	decb
    dfb3:	26 f5       	bne	0x0xdfaa
    dfb5:	ce 01 34    	ldx	#0x134
    dfb8:	b6 01 68    	ldaa	0x168
    dfbb:	bd eb 8a    	jsr	0xeb8a
    dfbe:	ce 01 3c    	ldx	#0x13c
    dfc1:	b6 01 69    	ldaa	0x169
    dfc4:	bd eb 8a    	jsr	0xeb8a
    dfc7:	7e ea 14    	jmp	0xea14
    dfca:	20 44       	bra	0x0xe010
    dfcc:	59          	rolb
    dfcd:	4e          	.byte	0x4e
    dfce:	41          	.byte	0x41
    dfcf:	4d          	tsta
    dfd0:	49          	rola
    dfd1:	43          	coma
    dfd2:	53          	comb
    dfd3:	20 52       	bra	0x0xe027
    dfd5:	41          	.byte	0x41
    dfd6:	4e          	.byte	0x4e
    dfd7:	47          	asra
    dfd8:	45          	.byte	0x45
    dfd9:	20 20       	bra	0x0xdffb
    dfdb:	4f          	clra
    dfdc:	4e          	.byte	0x4e
    dfdd:	20 20       	bra	0x0xdfff
    dfdf:	20 20       	bra	0x0xe001
    dfe1:	20 4f       	bra	0x0xe032
    dfe3:	46          	rora
    dfe4:	46          	rora
    dfe5:	20 20       	bra	0x0xe007
    dfe7:	20 20       	bra	0x0xe009
    dfe9:	20 7d       	bra	0x0xe068
    dfeb:	01          	nop
    dfec:	1d 26 0a    	bclr	0x26,x, #0x0a
    dfef:	bd ec 01    	jsr	0xec01
    dff2:	ce 10 23    	ldx	#0x1023
    dff5:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdff5
    dff9:	bd ea d7    	jsr	0xead7
    dffc:	86 20       	ldaa	#0x20
    dffe:	c6 20       	ldab	#0x20
    e000:	ce 01 20    	ldx	#0x120
    e003:	a7 00       	staa	0x0,x
    e005:	08          	inx
    e006:	5a          	decb
    e007:	26 fa       	bne	0x0xe003
    e009:	ce e0 4b    	ldx	#0xe04b
    e00c:	d6 60       	ldab	*0x60
    e00e:	18 ce 01 20 	ldy	#0x120
    e012:	bd ec 3d    	jsr	0xec3d
    e015:	ce e0 4b    	ldx	#0xe04b
    e018:	d6 61       	ldab	*0x61
    e01a:	18 08       	iny
    e01c:	18 08       	iny
    e01e:	18 08       	iny
    e020:	bd ec 3d    	jsr	0xec3d
    e023:	ce e0 4b    	ldx	#0xe04b
    e026:	d6 62       	ldab	*0x62
    e028:	18 08       	iny
    e02a:	18 08       	iny
    e02c:	bd ec 3d    	jsr	0xec3d
    e02f:	ce 01 31    	ldx	#0x131
    e032:	96 63       	ldaa	*0x63
    e034:	bd eb 8a    	jsr	0xeb8a
    e037:	08          	inx
    e038:	08          	inx
    e039:	08          	inx
    e03a:	08          	inx
    e03b:	96 64       	ldaa	*0x64
    e03d:	bd eb 8a    	jsr	0xeb8a
    e040:	08          	inx
    e041:	08          	inx
    e042:	08          	inx
    e043:	96 65       	ldaa	*0x65
    e045:	bd eb 8a    	jsr	0xeb8a
    e048:	7e ea 14    	jmp	0xea14
    e04b:	20 4f       	bra	0x0xe09c
    e04d:	46          	rora
    e04e:	46          	rora
    e04f:	46          	rora
    e050:	52          	.byte	0x52
    e051:	45          	.byte	0x45
    e052:	31          	ins
    e053:	46          	rora
    e054:	52          	.byte	0x52
    e055:	45          	.byte	0x45
    e056:	32          	pula
    e057:	31          	ins
    e058:	26 32       	bne	0x0xe08c
    e05a:	46          	rora
    e05b:	4c          	inca
    e05c:	45          	.byte	0x45
    e05d:	56          	rorb
    e05e:	31          	ins
    e05f:	4c          	inca
    e060:	45          	.byte	0x45
    e061:	56          	rorb
    e062:	32          	pula
    e063:	20 50       	bra	0x0xe0b5
    e065:	57          	asrb
    e066:	31          	ins
    e067:	20 50       	bra	0x0xe0b9
    e069:	57          	asrb
    e06a:	32          	pula
    e06b:	31          	ins
    e06c:	26 32       	bne	0x0xe0a0
    e06e:	50          	negb
    e06f:	46          	rora
    e070:	49          	rola
    e071:	4c          	inca
    e072:	54          	lsrb
    e073:	52          	.byte	0x52
    e074:	45          	.byte	0x45
    e075:	53          	comb
    e076:	4f          	clra
    e077:	4c          	inca
    e078:	45          	.byte	0x45
    e079:	56          	rorb
    e07a:	4e          	.byte	0x4e
    e07b:	58          	aslb
    e07c:	4d          	tsta
    e07d:	4f          	clra
    e07e:	44          	lsra
    e07f:	20 45       	bra	0x0xe0c6
    e081:	41          	.byte	0x41
    e082:	31          	ins
    e083:	20 45       	bra	0x0xe0ca
    e085:	41          	.byte	0x41
    e086:	33          	pulb
    e087:	20 45       	bra	0x0xe0ce
    e089:	58          	aslb
    e08a:	54          	lsrb
    e08b:	4c          	inca
    e08c:	46          	rora
    e08d:	31          	ins
    e08e:	52          	.byte	0x52
    e08f:	4c          	inca
    e090:	46          	rora
    e091:	32          	pula
    e092:	52          	.byte	0x52
    e093:	4c          	inca
    e094:	46          	rora
    e095:	31          	ins
    e096:	44          	lsra
    e097:	4c          	inca
    e098:	46          	rora
    e099:	32          	pula
    e09a:	44          	lsra
    e09b:	31          	ins
    e09c:	26 32       	bne	0x0xe0d0
    e09e:	44          	lsra
    e09f:	31          	ins
    e0a0:	26 32       	bne	0x0xe0d4
    e0a2:	52          	.byte	0x52
    e0a3:	50          	negb
    e0a4:	41          	.byte	0x41
    e0a5:	4e          	.byte	0x4e
    e0a6:	52          	.byte	0x52
    e0a7:	50          	negb
    e0a8:	41          	.byte	0x41
    e0a9:	4e          	.byte	0x4e
    e0aa:	44          	lsra
    e0ab:	20 50       	bra	0x0xe0fd
    e0ad:	41          	.byte	0x41
    e0ae:	4e          	.byte	0x4e
    e0af:	bd ea d7    	jsr	0xead7
    e0b2:	ce 01 20    	ldx	#0x120
    e0b5:	18 ce e0 e2 	ldy	#0xe0e2
    e0b9:	c6 20       	ldab	#0x20
    e0bb:	18 a6 00    	ldaa	0x0,y
    e0be:	a7 00       	staa	0x0,x
    e0c0:	08          	inx
    e0c1:	18 08       	iny
    e0c3:	5a          	decb
    e0c4:	26 f5       	bne	0x0xe0bb
    e0c6:	ce 01 31    	ldx	#0x131
    e0c9:	96 54       	ldaa	*0x54
    e0cb:	bd eb 8a    	jsr	0xeb8a
    e0ce:	08          	inx
    e0cf:	08          	inx
    e0d0:	08          	inx
    e0d1:	08          	inx
    e0d2:	96 59       	ldaa	*0x59
    e0d4:	bd eb 8a    	jsr	0xeb8a
    e0d7:	08          	inx
    e0d8:	08          	inx
    e0d9:	08          	inx
    e0da:	96 5e       	ldaa	*0x5e
    e0dc:	bd eb 8a    	jsr	0xeb8a
    e0df:	7e ea 14    	jmp	0xea14
    e0e2:	20 44       	bra	0x0xe128
    e0e4:	4b          	.byte	0x4b
    e0e5:	32          	pula
    e0e6:	20 20       	bra	0x0xe108
    e0e8:	20 44       	bra	0x0xe12e
    e0ea:	4b          	.byte	0x4b
    e0eb:	32          	pula
    e0ec:	20 20       	bra	0x0xe10e
    e0ee:	44          	lsra
    e0ef:	4b          	.byte	0x4b
    e0f0:	32          	pula
    e0f1:	20 20       	bra	0x0xe113
    e0f3:	20 20       	bra	0x0xe115
    e0f5:	20 20       	bra	0x0xe117
    e0f7:	20 20       	bra	0x0xe119
    e0f9:	20 20       	bra	0x0xe11b
    e0fb:	20 20       	bra	0x0xe11d
    e0fd:	20 20       	bra	0x0xe11f
    e0ff:	20 20       	bra	0x0xe121
    e101:	20 bd       	bra	0x0xe0c0
    e103:	ea d7       	orab	0xd7,x
    e105:	ce 01 20    	ldx	#0x120
    e108:	18 ce e1 34 	ldy	#0xe134
    e10c:	c6 20       	ldab	#0x20
    e10e:	18 a6 00    	ldaa	0x0,y
    e111:	a7 00       	staa	0x0,x
    e113:	08          	inx
    e114:	18 08       	iny
    e116:	5a          	decb
    e117:	26 f5       	bne	0x0xe10e
    e119:	ce 01 31    	ldx	#0x131
    e11c:	96 90       	ldaa	*0x90
    e11e:	bd eb 8a    	jsr	0xeb8a
    e121:	ce 01 37    	ldx	#0x137
    e124:	96 91       	ldaa	*0x91
    e126:	bd eb 8a    	jsr	0xeb8a
    e129:	ce 01 3c    	ldx	#0x13c
    e12c:	96 92       	ldaa	*0x92
    e12e:	bd eb 8a    	jsr	0xeb8a
    e131:	7e ea 14    	jmp	0xea14
    e134:	44          	lsra
    e135:	59          	rolb
    e136:	4e          	.byte	0x4e
    e137:	31          	ins
    e138:	20 20       	bra	0x0xe15a
    e13a:	44          	lsra
    e13b:	59          	rolb
    e13c:	4e          	.byte	0x4e
    e13d:	32          	pula
    e13e:	20 44       	bra	0x0xe184
    e140:	59          	rolb
    e141:	4e          	.byte	0x4e
    e142:	33          	pulb
    e143:	20 20       	bra	0x0xe165
    e145:	20 20       	bra	0x0xe167
    e147:	20 20       	bra	0x0xe169
    e149:	20 20       	bra	0x0xe16b
    e14b:	20 20       	bra	0x0xe16d
    e14d:	20 20       	bra	0x0xe16f
    e14f:	20 20       	bra	0x0xe171
    e151:	20 20       	bra	0x0xe173
    e153:	20 bd       	bra	0x0xe112
    e155:	ea d7       	orab	0xd7,x
    e157:	ce 01 20    	ldx	#0x120
    e15a:	18 ce e1 7e 	ldy	#0xe17e
    e15e:	c6 20       	ldab	#0x20
    e160:	18 a6 00    	ldaa	0x0,y
    e163:	a7 00       	staa	0x0,x
    e165:	08          	inx
    e166:	18 08       	iny
    e168:	5a          	decb
    e169:	26 f5       	bne	0x0xe160
    e16b:	ce 01 31    	ldx	#0x131
    e16e:	96 50       	ldaa	*0x50
    e170:	bd eb 8a    	jsr	0xeb8a
    e173:	ce 01 37    	ldx	#0x137
    e176:	96 98       	ldaa	*0x98
    e178:	bd eb 8a    	jsr	0xeb8a
    e17b:	7e ea 14    	jmp	0xea14
    e17e:	44          	lsra
    e17f:	4c          	inca
    e180:	41          	.byte	0x41
    e181:	59          	rolb
    e182:	31          	ins
    e183:	20 44       	bra	0x0xe1c9
    e185:	4c          	inca
    e186:	41          	.byte	0x41
    e187:	59          	rolb
    e188:	33          	pulb
    e189:	20 20       	bra	0x0xe1ab
    e18b:	20 20       	bra	0x0xe1ad
    e18d:	20 20       	bra	0x0xe1af
    e18f:	20 20       	bra	0x0xe1b1
    e191:	20 20       	bra	0x0xe1b3
    e193:	20 20       	bra	0x0xe1b5
    e195:	20 20       	bra	0x0xe1b7
    e197:	20 20       	bra	0x0xe1b9
    e199:	20 20       	bra	0x0xe1bb
    e19b:	20 20       	bra	0x0xe1bd
    e19d:	20 7d       	bra	0x0xe21c
    e19f:	01          	nop
    e1a0:	1d 26 0a    	bclr	0x26,x, #0x0a
    e1a3:	bd ec 01    	jsr	0xec01
    e1a6:	ce 10 23    	ldx	#0x1023
    e1a9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe1a9
    e1ad:	bd ea d7    	jsr	0xead7
    e1b0:	ce 01 20    	ldx	#0x120
    e1b3:	18 ce e1 e1 	ldy	#0xe1e1
    e1b7:	c6 20       	ldab	#0x20
    e1b9:	18 a6 00    	ldaa	0x0,y
    e1bc:	a7 00       	staa	0x0,x
    e1be:	08          	inx
    e1bf:	18 08       	iny
    e1c1:	5a          	decb
    e1c2:	26 f5       	bne	0x0xe1b9
    e1c4:	ce e2 01    	ldx	#0xe201
    e1c7:	d6 4b       	ldab	*0x4b
    e1c9:	18 ce 01 30 	ldy	#0x130
    e1cd:	bd ec 3d    	jsr	0xec3d
    e1d0:	ce e2 1d    	ldx	#0xe21d
    e1d3:	d6 66       	ldab	*0x66
    e1d5:	18 08       	iny
    e1d7:	18 08       	iny
    e1d9:	18 08       	iny
    e1db:	bd ec 3d    	jsr	0xec3d
    e1de:	7e ea 14    	jmp	0xea14
    e1e1:	54          	lsrb
    e1e2:	59          	rolb
    e1e3:	50          	negb
    e1e4:	45          	.byte	0x45
    e1e5:	20 20       	bra	0x0xe207
    e1e7:	49          	rola
    e1e8:	4e          	.byte	0x4e
    e1e9:	56          	rorb
    e1ea:	54          	lsrb
    e1eb:	20 20       	bra	0x0xe20d
    e1ed:	20 20       	bra	0x0xe20f
    e1ef:	20 20       	bra	0x0xe211
    e1f1:	20 20       	bra	0x0xe213
    e1f3:	20 20       	bra	0x0xe215
    e1f5:	20 20       	bra	0x0xe217
    e1f7:	20 20       	bra	0x0xe219
    e1f9:	20 20       	bra	0x0xe21b
    e1fb:	20 20       	bra	0x0xe21d
    e1fd:	20 20       	bra	0x0xe21f
    e1ff:	20 20       	bra	0x0xe221
    e201:	4f          	clra
    e202:	42          	.byte	0x42
    e203:	4c          	inca
    e204:	50          	negb
    e205:	4f          	clra
    e206:	42          	.byte	0x42
    e207:	42          	.byte	0x42
    e208:	50          	negb
    e209:	4f          	clra
    e20a:	42          	.byte	0x42
    e20b:	48          	asla
    e20c:	50          	negb
    e20d:	4f          	clra
    e20e:	42          	.byte	0x42
    e20f:	42          	.byte	0x42
    e210:	52          	.byte	0x52
    e211:	4d          	tsta
    e212:	49          	rola
    e213:	4e          	.byte	0x4e
    e214:	49          	rola
    e215:	20 33       	bra	0x0xe24a
    e217:	30          	tsx
    e218:	33          	pulb
    e219:	20 41       	bra	0x0xe25c
    e21b:	52          	.byte	0x52
    e21c:	50          	negb
    e21d:	20 4f       	bra	0x0xe26e
    e21f:	46          	rora
    e220:	46          	rora
    e221:	45          	.byte	0x45
    e222:	4e          	.byte	0x4e
    e223:	56          	rorb
    e224:	31          	ins
    e225:	45          	.byte	0x45
    e226:	4e          	.byte	0x4e
    e227:	56          	rorb
    e228:	33          	pulb
    e229:	45          	.byte	0x45
    e22a:	31          	ins
    e22b:	26 33       	bne	0x0xe260
    e22d:	bd ea d7    	jsr	0xead7
    e230:	ce 01 20    	ldx	#0x120
    e233:	18 ce e2 64 	ldy	#0xe264
    e237:	c6 20       	ldab	#0x20
    e239:	18 a6 00    	ldaa	0x0,y
    e23c:	a7 00       	staa	0x0,x
    e23e:	08          	inx
    e23f:	18 08       	iny
    e241:	5a          	decb
    e242:	26 f5       	bne	0x0xe239
    e244:	ce 01 30    	ldx	#0x130
    e247:	96 6e       	ldaa	*0x6e
    e249:	bd eb 8a    	jsr	0xeb8a
    e24c:	08          	inx
    e24d:	08          	inx
    e24e:	96 48       	ldaa	*0x48
    e250:	bd eb 8a    	jsr	0xeb8a
    e253:	08          	inx
    e254:	08          	inx
    e255:	96 6f       	ldaa	*0x6f
    e257:	bd eb 8a    	jsr	0xeb8a
    e25a:	08          	inx
    e25b:	08          	inx
    e25c:	96 70       	ldaa	*0x70
    e25e:	bd ec 51    	jsr	0xec51
    e261:	7e ea 14    	jmp	0xea14
    e264:	20 49       	bra	0x0xe2af
    e266:	4e          	.byte	0x4e
    e267:	20 4d       	bra	0x0xe2b6
    e269:	49          	rola
    e26a:	58          	aslb
    e26b:	20 54       	bra	0x0xe2c1
    e26d:	52          	.byte	0x52
    e26e:	47          	asra
    e26f:	20 57       	bra	0x0xe2c8
    e271:	49          	rola
    e272:	4e          	.byte	0x4e
    e273:	20 20       	bra	0x0xe295
    e275:	20 20       	bra	0x0xe297
    e277:	20 20       	bra	0x0xe299
    e279:	20 20       	bra	0x0xe29b
    e27b:	20 20       	bra	0x0xe29d
    e27d:	20 20       	bra	0x0xe29f
    e27f:	20 20       	bra	0x0xe2a1
    e281:	20 20       	bra	0x0xe2a3
    e283:	20 7d       	bra	0x0xe302
    e285:	01          	nop
    e286:	1d 26 0a    	bclr	0x26,x, #0x0a
    e289:	bd ec 01    	jsr	0xec01
    e28c:	ce 10 23    	ldx	#0x1023
    e28f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe28f
    e293:	bd ea d7    	jsr	0xead7
    e296:	ce 01 20    	ldx	#0x120
    e299:	18 ce e2 c7 	ldy	#0xe2c7
    e29d:	c6 20       	ldab	#0x20
    e29f:	18 a6 00    	ldaa	0x0,y
    e2a2:	a7 00       	staa	0x0,x
    e2a4:	08          	inx
    e2a5:	18 08       	iny
    e2a7:	5a          	decb
    e2a8:	26 f5       	bne	0x0xe29f
    e2aa:	ce e2 e7    	ldx	#0xe2e7
    e2ad:	d6 74       	ldab	*0x74
    e2af:	18 ce 01 30 	ldy	#0x130
    e2b3:	bd ec 3d    	jsr	0xec3d
    e2b6:	ce e4 88    	ldx	#0xe488
    e2b9:	d6 72       	ldab	*0x72
    e2bb:	18 08       	iny
    e2bd:	18 08       	iny
    e2bf:	18 08       	iny
    e2c1:	bd ec 3d    	jsr	0xec3d
    e2c4:	7e ea 14    	jmp	0xea14
    e2c7:	52          	.byte	0x52
    e2c8:	4e          	.byte	0x4e
    e2c9:	47          	asra
    e2ca:	45          	.byte	0x45
    e2cb:	20 20       	bra	0x0xe2ed
    e2cd:	53          	comb
    e2ce:	59          	rolb
    e2cf:	4e          	.byte	0x4e
    e2d0:	43          	coma
    e2d1:	20 20       	bra	0x0xe2f3
    e2d3:	20 20       	bra	0x0xe2f5
    e2d5:	20 20       	bra	0x0xe2f7
    e2d7:	20 20       	bra	0x0xe2f9
    e2d9:	20 20       	bra	0x0xe2fb
    e2db:	20 20       	bra	0x0xe2fd
    e2dd:	20 20       	bra	0x0xe2ff
    e2df:	20 20       	bra	0x0xe301
    e2e1:	20 20       	bra	0x0xe303
    e2e3:	20 20       	bra	0x0xe305
    e2e5:	20 20       	bra	0x0xe307
    e2e7:	20 4f       	bra	0x0xe338
    e2e9:	46          	rora
    e2ea:	46          	rora
    e2eb:	31          	ins
    e2ec:	4f          	clra
    e2ed:	43          	coma
    e2ee:	54          	lsrb
    e2ef:	32          	pula
    e2f0:	4f          	clra
    e2f1:	43          	coma
    e2f2:	54          	lsrb
    e2f3:	33          	pulb
    e2f4:	4f          	clra
    e2f5:	43          	coma
    e2f6:	54          	lsrb
    e2f7:	34          	des
    e2f8:	4f          	clra
    e2f9:	43          	coma
    e2fa:	54          	lsrb
    e2fb:	7d 01 1d    	tst	0x11d
    e2fe:	26 0a       	bne	0x0xe30a
    e300:	bd ec 01    	jsr	0xec01
    e303:	ce 10 23    	ldx	#0x1023
    e306:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe306
    e30a:	bd ea d7    	jsr	0xead7
    e30d:	86 20       	ldaa	#0x20
    e30f:	c6 20       	ldab	#0x20
    e311:	ce 01 20    	ldx	#0x120
    e314:	a7 00       	staa	0x0,x
    e316:	08          	inx
    e317:	5a          	decb
    e318:	26 fa       	bne	0x0xe314
    e31a:	ce e3 9f    	ldx	#0xe39f
    e31d:	12 f4 02 3f 	brset	*0xf4, #0x02, 0x0xe360
    e321:	d6 2d       	ldab	*0x2d
    e323:	18 ce 01 20 	ldy	#0x120
    e327:	bd ec 3d    	jsr	0xec3d
    e32a:	ce e3 9f    	ldx	#0xe39f
    e32d:	d6 2e       	ldab	*0x2e
    e32f:	18 08       	iny
    e331:	18 08       	iny
    e333:	18 08       	iny
    e335:	bd ec 3d    	jsr	0xec3d
    e338:	ce e3 9f    	ldx	#0xe39f
    e33b:	d6 2f       	ldab	*0x2f
    e33d:	18 08       	iny
    e33f:	18 08       	iny
    e341:	bd ec 3d    	jsr	0xec3d
    e344:	ce 01 31    	ldx	#0x131
    e347:	96 2a       	ldaa	*0x2a
    e349:	bd eb 8a    	jsr	0xeb8a
    e34c:	08          	inx
    e34d:	08          	inx
    e34e:	08          	inx
    e34f:	08          	inx
    e350:	96 2b       	ldaa	*0x2b
    e352:	bd eb 8a    	jsr	0xeb8a
    e355:	08          	inx
    e356:	08          	inx
    e357:	08          	inx
    e358:	96 2c       	ldaa	*0x2c
    e35a:	bd eb 8a    	jsr	0xeb8a
    e35d:	7e ea 14    	jmp	0xea14
    e360:	d6 3a       	ldab	*0x3a
    e362:	18 ce 01 20 	ldy	#0x120
    e366:	bd ec 3d    	jsr	0xec3d
    e369:	ce e3 9f    	ldx	#0xe39f
    e36c:	d6 3b       	ldab	*0x3b
    e36e:	18 08       	iny
    e370:	18 08       	iny
    e372:	18 08       	iny
    e374:	bd ec 3d    	jsr	0xec3d
    e377:	ce e3 9f    	ldx	#0xe39f
    e37a:	d6 3c       	ldab	*0x3c
    e37c:	18 08       	iny
    e37e:	18 08       	iny
    e380:	bd ec 3d    	jsr	0xec3d
    e383:	ce 01 31    	ldx	#0x131
    e386:	96 37       	ldaa	*0x37
    e388:	bd eb 8a    	jsr	0xeb8a
    e38b:	08          	inx
    e38c:	08          	inx
    e38d:	08          	inx
    e38e:	08          	inx
    e38f:	96 38       	ldaa	*0x38
    e391:	bd eb 8a    	jsr	0xeb8a
    e394:	08          	inx
    e395:	08          	inx
    e396:	08          	inx
    e397:	96 39       	ldaa	*0x39
    e399:	bd eb 8a    	jsr	0xeb8a
    e39c:	7e ea 14    	jmp	0xea14
    e39f:	20 4f       	bra	0x0xe3f0
    e3a1:	46          	rora
    e3a2:	46          	rora
    e3a3:	46          	rora
    e3a4:	52          	.byte	0x52
    e3a5:	45          	.byte	0x45
    e3a6:	31          	ins
    e3a7:	46          	rora
    e3a8:	52          	.byte	0x52
    e3a9:	45          	.byte	0x45
    e3aa:	32          	pula
    e3ab:	31          	ins
    e3ac:	26 32       	bne	0x0xe3e0
    e3ae:	46          	rora
    e3af:	4c          	inca
    e3b0:	45          	.byte	0x45
    e3b1:	56          	rorb
    e3b2:	31          	ins
    e3b3:	4c          	inca
    e3b4:	45          	.byte	0x45
    e3b5:	56          	rorb
    e3b6:	32          	pula
    e3b7:	20 50       	bra	0x0xe409
    e3b9:	57          	asrb
    e3ba:	31          	ins
    e3bb:	20 50       	bra	0x0xe40d
    e3bd:	57          	asrb
    e3be:	32          	pula
    e3bf:	31          	ins
    e3c0:	26 32       	bne	0x0xe3f4
    e3c2:	50          	negb
    e3c3:	46          	rora
    e3c4:	49          	rola
    e3c5:	4c          	inca
    e3c6:	54          	lsrb
    e3c7:	52          	.byte	0x52
    e3c8:	45          	.byte	0x45
    e3c9:	53          	comb
    e3ca:	4f          	clra
    e3cb:	4c          	inca
    e3cc:	45          	.byte	0x45
    e3cd:	56          	rorb
    e3ce:	4e          	.byte	0x4e
    e3cf:	58          	aslb
    e3d0:	4d          	tsta
    e3d1:	4f          	clra
    e3d2:	44          	lsra
    e3d3:	20 45       	bra	0x0xe41a
    e3d5:	41          	.byte	0x41
    e3d6:	31          	ins
    e3d7:	20 45       	bra	0x0xe41e
    e3d9:	41          	.byte	0x41
    e3da:	33          	pulb
    e3db:	20 45       	bra	0x0xe422
    e3dd:	58          	aslb
    e3de:	54          	lsrb
    e3df:	20 56       	bra	0x0xe437
    e3e1:	4f          	clra
    e3e2:	4c          	inca
    e3e3:	bd ea d7    	jsr	0xead7
    e3e6:	ce 01 20    	ldx	#0x120
    e3e9:	18 ce e4 48 	ldy	#0xe448
    e3ed:	c6 20       	ldab	#0x20
    e3ef:	18 a6 00    	ldaa	0x0,y
    e3f2:	a7 00       	staa	0x0,x
    e3f4:	08          	inx
    e3f5:	18 08       	iny
    e3f7:	5a          	decb
    e3f8:	26 f5       	bne	0x0xe3ef
    e3fa:	ce e4 68    	ldx	#0xe468
    e3fd:	18 ce 01 30 	ldy	#0x130
    e401:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe40c
    e405:	d6 27       	ldab	*0x27
    e407:	bd ec 3d    	jsr	0xec3d
    e40a:	20 05       	bra	0x0xe411
    e40c:	d6 34       	ldab	*0x34
    e40e:	bd ec 3d    	jsr	0xec3d
    e411:	ce e4 80    	ldx	#0xe480
    e414:	18 08       	iny
    e416:	18 08       	iny
    e418:	18 08       	iny
    e41a:	12 f4 02 09 	brset	*0xf4, #0x02, 0x0xe427
    e41e:	d6 31       	ldab	*0x31
    e420:	c4 01       	andb	#0x1
    e422:	bd ec 3d    	jsr	0xec3d
    e425:	20 06       	bra	0x0xe42d
    e427:	d6 31       	ldab	*0x31
    e429:	54          	lsrb
    e42a:	bd ec 3d    	jsr	0xec3d
    e42d:	ce e4 88    	ldx	#0xe488
    e430:	18 08       	iny
    e432:	18 08       	iny
    e434:	12 f4 02 08 	brset	*0xf4, #0x02, 0x0xe440
    e438:	d6 25       	ldab	*0x25
    e43a:	bd ec 3d    	jsr	0xec3d
    e43d:	7e ea 14    	jmp	0xea14
    e440:	d6 32       	ldab	*0x32
    e442:	bd ec 3d    	jsr	0xec3d
    e445:	7e ea 14    	jmp	0xea14
    e448:	57          	asrb
    e449:	41          	.byte	0x41
    e44a:	56          	rorb
    e44b:	45          	.byte	0x45
    e44c:	20 20       	bra	0x0xe46e
    e44e:	4d          	tsta
    e44f:	4f          	clra
    e450:	44          	lsra
    e451:	45          	.byte	0x45
    e452:	20 53       	bra	0x0xe4a7
    e454:	59          	rolb
    e455:	4e          	.byte	0x4e
    e456:	43          	coma
    e457:	20 20       	bra	0x0xe479
    e459:	20 20       	bra	0x0xe47b
    e45b:	20 20       	bra	0x0xe47d
    e45d:	20 20       	bra	0x0xe47f
    e45f:	20 20       	bra	0x0xe481
    e461:	20 20       	bra	0x0xe483
    e463:	20 20       	bra	0x0xe485
    e465:	20 20       	bra	0x0xe487
    e467:	20 20       	bra	0x0xe489
    e469:	54          	lsrb
    e46a:	52          	.byte	0x52
    e46b:	49          	rola
    e46c:	20 53       	bra	0x0xe4c1
    e46e:	51          	.byte	0x51
    e46f:	52          	.byte	0x52
    e470:	53          	comb
    e471:	57          	asrb
    e472:	55          	.byte	0x55
    e473:	50          	negb
    e474:	53          	comb
    e475:	57          	asrb
    e476:	44          	lsra
    e477:	4e          	.byte	0x4e
    e478:	52          	.byte	0x52
    e479:	41          	.byte	0x41
    e47a:	4e          	.byte	0x4e
    e47b:	44          	lsra
    e47c:	20 53       	bra	0x0xe4d1
    e47e:	2f 48       	ble	0x0xe4c8
    e480:	4d          	tsta
    e481:	4f          	clra
    e482:	4e          	.byte	0x4e
    e483:	4f          	clra
    e484:	50          	negb
    e485:	4f          	clra
    e486:	4c          	inca
    e487:	59          	rolb
    e488:	53          	comb
    e489:	45          	.byte	0x45
    e48a:	4c          	inca
    e48b:	46          	rora
    e48c:	20 20       	bra	0x0xe4ae
    e48e:	20 34       	bra	0x0xe4c4
    e490:	20 20       	bra	0x0xe4b2
    e492:	20 32       	bra	0x0xe4c6
    e494:	20 20       	bra	0x0xe4b6
    e496:	20 31       	bra	0x0xe4c9
    e498:	20 20       	bra	0x0xe4ba
    e49a:	31          	ins
    e49b:	54          	lsrb
    e49c:	20 31       	bra	0x0xe4cf
    e49e:	2f 32       	ble	0x0xe4d2
    e4a0:	31          	ins
    e4a1:	2f 32       	ble	0x0xe4d5
    e4a3:	54          	lsrb
    e4a4:	20 31       	bra	0x0xe4d7
    e4a6:	2f 34       	ble	0x0xe4dc
    e4a8:	31          	ins
    e4a9:	2f 34       	ble	0x0xe4df
    e4ab:	54          	lsrb
    e4ac:	20 31       	bra	0x0xe4df
    e4ae:	2f 38       	ble	0x0xe4e8
    e4b0:	31          	ins
    e4b1:	2f 38       	ble	0x0xe4eb
    e4b3:	54          	lsrb
    e4b4:	31          	ins
    e4b5:	2f 31       	ble	0x0xe4e8
    e4b7:	36          	psha
    e4b8:	20 31       	bra	0x0xe4eb
    e4ba:	36          	psha
    e4bb:	54          	lsrb
    e4bc:	bd ea d7    	jsr	0xead7
    e4bf:	ce 01 20    	ldx	#0x120
    e4c2:	18 ce e4 f9 	ldy	#0xe4f9
    e4c6:	c6 20       	ldab	#0x20
    e4c8:	18 a6 00    	ldaa	0x0,y
    e4cb:	a7 00       	staa	0x0,x
    e4cd:	08          	inx
    e4ce:	18 08       	iny
    e4d0:	5a          	decb
    e4d1:	26 f5       	bne	0x0xe4c8
    e4d3:	ce e5 22    	ldx	#0xe522
    e4d6:	18 ce 01 37 	ldy	#0x137
    e4da:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe4e5
    e4de:	d6 26       	ldab	*0x26
    e4e0:	bd ec 29    	jsr	0xec29
    e4e3:	20 05       	bra	0x0xe4ea
    e4e5:	d6 33       	ldab	*0x33
    e4e7:	bd ec 29    	jsr	0xec29
    e4ea:	d6 96       	ldab	*0x96
    e4ec:	ce e5 31    	ldx	#0xe531
    e4ef:	18 ce 01 3c 	ldy	#0x13c
    e4f3:	bd ec 29    	jsr	0xec29
    e4f6:	7e ea 14    	jmp	0xea14
    e4f9:	20 20       	bra	0x0xe51b
    e4fb:	20 20       	bra	0x0xe51d
    e4fd:	20 20       	bra	0x0xe51f
    e4ff:	20 4b       	bra	0x0xe54c
    e501:	45          	.byte	0x45
    e502:	59          	rolb
    e503:	20 20       	bra	0x0xe525
    e505:	51          	.byte	0x51
    e506:	55          	.byte	0x55
    e507:	41          	.byte	0x41
    e508:	4e          	.byte	0x4e
    e509:	20 20       	bra	0x0xe52b
    e50b:	20 20       	bra	0x0xe52d
    e50d:	20 20       	bra	0x0xe52f
    e50f:	20 20       	bra	0x0xe531
    e511:	20 20       	bra	0x0xe533
    e513:	20 20       	bra	0x0xe535
    e515:	20 20       	bra	0x0xe537
    e517:	20 20       	bra	0x0xe539
    e519:	4c          	inca
    e51a:	4f          	clra
    e51b:	57          	asrb
    e51c:	4d          	tsta
    e51d:	45          	.byte	0x45
    e51e:	44          	lsra
    e51f:	20 48       	bra	0x0xe569
    e521:	49          	rola
    e522:	4f          	clra
    e523:	46          	rora
    e524:	46          	rora
    e525:	20 55       	bra	0x0xe57c
    e527:	50          	negb
    e528:	20 44       	bra	0x0xe56e
    e52a:	4e          	.byte	0x4e
    e52b:	55          	.byte	0x55
    e52c:	50          	negb
    e52d:	31          	ins
    e52e:	44          	lsra
    e52f:	4e          	.byte	0x4e
    e530:	31          	ins
    e531:	4f          	clra
    e532:	46          	rora
    e533:	46          	rora
    e534:	4c          	inca
    e535:	46          	rora
    e536:	31          	ins
    e537:	4c          	inca
    e538:	46          	rora
    e539:	32          	pula
    e53a:	31          	ins
    e53b:	26 32       	bne	0x0xe56f
    e53d:	7d 01 1d    	tst	0x11d
    e540:	26 0a       	bne	0x0xe54c
    e542:	bd ec 01    	jsr	0xec01
    e545:	ce 10 23    	ldx	#0x1023
    e548:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe548
    e54c:	bd ea d7    	jsr	0xead7
    e54f:	86 20       	ldaa	#0x20
    e551:	c6 20       	ldab	#0x20
    e553:	ce 01 20    	ldx	#0x120
    e556:	a7 00       	staa	0x0,x
    e558:	08          	inx
    e559:	5a          	decb
    e55a:	26 fa       	bne	0x0xe556
    e55c:	ce 01 20    	ldx	#0x120
    e55f:	18 ce 00 b0 	ldy	#0xb0
    e563:	96 f9       	ldaa	*0xf9
    e565:	81 04       	cmpa	#0x4
    e567:	26 04       	bne	0x0xe56d
    e569:	18 ce 50 30 	ldy	#0x5030
    e56d:	18 a6 00    	ldaa	0x0,y
    e570:	bd eb c6    	jsr	0xebc6
    e573:	08          	inx
    e574:	08          	inx
    e575:	08          	inx
    e576:	08          	inx
    e577:	8c 01 2f    	cpx	#0x12f
    e57a:	26 01       	bne	0x0xe57d
    e57c:	08          	inx
    e57d:	18 08       	iny
    e57f:	8c 01 3f    	cpx	#0x13f
    e582:	25 e9       	bcs	0x0xe56d
    e584:	7e ea 14    	jmp	0xea14
    e587:	bd ea d7    	jsr	0xead7
    e58a:	86 20       	ldaa	#0x20
    e58c:	c6 20       	ldab	#0x20
    e58e:	ce 01 20    	ldx	#0x120
    e591:	a7 00       	staa	0x0,x
    e593:	08          	inx
    e594:	5a          	decb
    e595:	26 fa       	bne	0x0xe591
    e597:	ce 01 20    	ldx	#0x120
    e59a:	18 ce 00 a8 	ldy	#0xa8
    e59e:	96 f9       	ldaa	*0xf9
    e5a0:	81 04       	cmpa	#0x4
    e5a2:	26 04       	bne	0x0xe5a8
    e5a4:	18 ce 50 28 	ldy	#0x5028
    e5a8:	18 a6 00    	ldaa	0x0,y
    e5ab:	bd eb 8a    	jsr	0xeb8a
    e5ae:	08          	inx
    e5af:	08          	inx
    e5b0:	8c 01 2f    	cpx	#0x12f
    e5b3:	26 01       	bne	0x0xe5b6
    e5b5:	08          	inx
    e5b6:	18 08       	iny
    e5b8:	8c 01 3f    	cpx	#0x13f
    e5bb:	25 eb       	bcs	0x0xe5a8
    e5bd:	7e ea 14    	jmp	0xea14
    e5c0:	bd ea d7    	jsr	0xead7
    e5c3:	86 20       	ldaa	#0x20
    e5c5:	c6 20       	ldab	#0x20
    e5c7:	ce 01 20    	ldx	#0x120
    e5ca:	a7 00       	staa	0x0,x
    e5cc:	08          	inx
    e5cd:	5a          	decb
    e5ce:	26 fa       	bne	0x0xe5ca
    e5d0:	ce 01 20    	ldx	#0x120
    e5d3:	18 ce 00 b8 	ldy	#0xb8
    e5d7:	96 f9       	ldaa	*0xf9
    e5d9:	81 04       	cmpa	#0x4
    e5db:	26 04       	bne	0x0xe5e1
    e5dd:	18 ce 50 38 	ldy	#0x5038
    e5e1:	18 a6 00    	ldaa	0x0,y
    e5e4:	bd eb 8a    	jsr	0xeb8a
    e5e7:	08          	inx
    e5e8:	08          	inx
    e5e9:	8c 01 2f    	cpx	#0x12f
    e5ec:	26 01       	bne	0x0xe5ef
    e5ee:	08          	inx
    e5ef:	18 08       	iny
    e5f1:	8c 01 3f    	cpx	#0x13f
    e5f4:	25 eb       	bcs	0x0xe5e1
    e5f6:	7e ea 14    	jmp	0xea14
    e5f9:	bd ea d7    	jsr	0xead7
    e5fc:	ce 01 20    	ldx	#0x120
    e5ff:	18 ce e6 37 	ldy	#0xe637
    e603:	c6 20       	ldab	#0x20
    e605:	18 a6 00    	ldaa	0x0,y
    e608:	a7 00       	staa	0x0,x
    e60a:	08          	inx
    e60b:	18 08       	iny
    e60d:	5a          	decb
    e60e:	26 f5       	bne	0x0xe605
    e610:	ce e4 68    	ldx	#0xe468
    e613:	d6 a7       	ldab	*0xa7
    e615:	18 ce 01 30 	ldy	#0x130
    e619:	bd ec 3d    	jsr	0xec3d
    e61c:	ce e5 22    	ldx	#0xe522
    e61f:	d6 a6       	ldab	*0xa6
    e621:	18 ce 01 37 	ldy	#0x137
    e625:	bd ec 29    	jsr	0xec29
    e628:	ce e4 88    	ldx	#0xe488
    e62b:	d6 a5       	ldab	*0xa5
    e62d:	18 ce 01 3b 	ldy	#0x13b
    e631:	bd ec 3d    	jsr	0xec3d
    e634:	7e ea 14    	jmp	0xea14
    e637:	57          	asrb
    e638:	41          	.byte	0x41
    e639:	56          	rorb
    e63a:	45          	.byte	0x45
    e63b:	20 20       	bra	0x0xe65d
    e63d:	20 4b       	bra	0x0xe68a
    e63f:	45          	.byte	0x45
    e640:	59          	rolb
    e641:	20 53       	bra	0x0xe696
    e643:	59          	rolb
    e644:	4e          	.byte	0x4e
    e645:	43          	coma
    e646:	20 20       	bra	0x0xe668
    e648:	20 20       	bra	0x0xe66a
    e64a:	20 20       	bra	0x0xe66c
    e64c:	20 20       	bra	0x0xe66e
    e64e:	20 20       	bra	0x0xe670
    e650:	20 20       	bra	0x0xe672
    e652:	20 20       	bra	0x0xe674
    e654:	20 20       	bra	0x0xe676
    e656:	20 bd       	bra	0x0xe615
    e658:	ea d7       	orab	0xd7,x
    e65a:	ce 01 20    	ldx	#0x120
    e65d:	18 ce e6 ad 	ldy	#0xe6ad
    e661:	c6 20       	ldab	#0x20
    e663:	18 a6 00    	ldaa	0x0,y
    e666:	a7 00       	staa	0x0,x
    e668:	08          	inx
    e669:	18 08       	iny
    e66b:	5a          	decb
    e66c:	26 f5       	bne	0x0xe663
    e66e:	ce da e4    	ldx	#0xdae4
    e671:	18 ce 01 30 	ldy	#0x130
    e675:	13 21 02 07 	brclr	*0x21, #0x02, 0x0xe680
    e679:	c6 01       	ldab	#0x1
    e67b:	bd ec 29    	jsr	0xec29
    e67e:	20 04       	bra	0x0xe684
    e680:	5f          	clrb
    e681:	bd ec 29    	jsr	0xec29
    e684:	18 ce 01 34 	ldy	#0x134
    e688:	ce da e4    	ldx	#0xdae4
    e68b:	12 21 04 06 	brset	*0x21, #0x04, 0x0xe695
    e68f:	5f          	clrb
    e690:	bd ec 29    	jsr	0xec29
    e693:	20 05       	bra	0x0xe69a
    e695:	c6 01       	ldab	#0x1
    e697:	bd ec 29    	jsr	0xec29
    e69a:	96 22       	ldaa	*0x22
    e69c:	ce 01 38    	ldx	#0x138
    e69f:	bd eb c6    	jsr	0xebc6
    e6a2:	96 23       	ldaa	*0x23
    e6a4:	ce 01 3c    	ldx	#0x13c
    e6a7:	bd eb 8a    	jsr	0xeb8a
    e6aa:	7e ea 14    	jmp	0xea14
    e6ad:	47          	asra
    e6ae:	4c          	inca
    e6af:	53          	comb
    e6b0:	20 41       	bra	0x0xe6f3
    e6b2:	55          	.byte	0x55
    e6b3:	54          	lsrb
    e6b4:	20 49       	bra	0x0xe6ff
    e6b6:	4e          	.byte	0x4e
    e6b7:	54          	lsrb
    e6b8:	20 44       	bra	0x0xe6fe
    e6ba:	59          	rolb
    e6bb:	4e          	.byte	0x4e
    e6bc:	20 20       	bra	0x0xe6de
    e6be:	20 20       	bra	0x0xe6e0
    e6c0:	20 20       	bra	0x0xe6e2
    e6c2:	20 20       	bra	0x0xe6e4
    e6c4:	20 20       	bra	0x0xe6e6
    e6c6:	20 20       	bra	0x0xe6e8
    e6c8:	20 20       	bra	0x0xe6ea
    e6ca:	20 20       	bra	0x0xe6ec
    e6cc:	20 7d       	bra	0x0xe74b
    e6ce:	01          	nop
    e6cf:	1d 26 0a    	bclr	0x26,x, #0x0a
    e6d2:	bd ec 01    	jsr	0xec01
    e6d5:	ce 10 23    	ldx	#0x1023
    e6d8:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe6d8
    e6dc:	bd ea d7    	jsr	0xead7
    e6df:	ce 01 20    	ldx	#0x120
    e6e2:	18 ce e7 01 	ldy	#0xe701
    e6e6:	c6 20       	ldab	#0x20
    e6e8:	18 a6 00    	ldaa	0x0,y
    e6eb:	a7 00       	staa	0x0,x
    e6ed:	08          	inx
    e6ee:	18 08       	iny
    e6f0:	5a          	decb
    e6f1:	26 f5       	bne	0x0xe6e8
    e6f3:	ce d8 74    	ldx	#0xd874
    e6f6:	5f          	clrb
    e6f7:	18 ce 01 31 	ldy	#0x131
    e6fb:	bd ec 29    	jsr	0xec29
    e6fe:	7e ea 14    	jmp	0xea14
    e701:	20 54       	bra	0x0xe757
    e703:	55          	.byte	0x55
    e704:	4e          	.byte	0x4e
    e705:	45          	.byte	0x45
    e706:	3f          	swi
    e707:	20 20       	bra	0x0xe729
    e709:	20 20       	bra	0x0xe72b
    e70b:	20 20       	bra	0x0xe72d
    e70d:	20 20       	bra	0x0xe72f
    e70f:	20 20       	bra	0x0xe731
    e711:	20 20       	bra	0x0xe733
    e713:	20 20       	bra	0x0xe735
    e715:	20 20       	bra	0x0xe737
    e717:	20 20       	bra	0x0xe739
    e719:	20 20       	bra	0x0xe73b
    e71b:	20 20       	bra	0x0xe73d
    e71d:	20 20       	bra	0x0xe73f
    e71f:	20 20       	bra	0x0xe741
    e721:	7d 01 1d    	tst	0x11d
    e724:	26 0a       	bne	0x0xe730
    e726:	bd ec 01    	jsr	0xec01
    e729:	ce 10 23    	ldx	#0x1023
    e72c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe72c
    e730:	bd ea d7    	jsr	0xead7
    e733:	ce 01 20    	ldx	#0x120
    e736:	18 ce e7 59 	ldy	#0xe759
    e73a:	c6 20       	ldab	#0x20
    e73c:	18 a6 00    	ldaa	0x0,y
    e73f:	a7 00       	staa	0x0,x
    e741:	08          	inx
    e742:	18 08       	iny
    e744:	5a          	decb
    e745:	26 f5       	bne	0x0xe73c
    e747:	f6 10 28    	ldab	0x1028
    e74a:	c4 03       	andb	#0x3
    e74c:	ce e7 79    	ldx	#0xe779
    e74f:	18 ce 01 30 	ldy	#0x130
    e753:	bd ec 3d    	jsr	0xec3d
    e756:	7e ea 14    	jmp	0xea14
    e759:	53          	comb
    e75a:	45          	.byte	0x45
    e75b:	54          	lsrb
    e75c:	20 53       	bra	0x0xe7b1
    e75e:	50          	negb
    e75f:	49          	rola
    e760:	20 42       	bra	0x0xe7a4
    e762:	49          	rola
    e763:	54          	lsrb
    e764:	20 52       	bra	0x0xe7b8
    e766:	41          	.byte	0x41
    e767:	54          	lsrb
    e768:	45          	.byte	0x45
    e769:	20 20       	bra	0x0xe78b
    e76b:	20 20       	bra	0x0xe78d
    e76d:	20 4b       	bra	0x0xe7ba
    e76f:	42          	.byte	0x42
    e770:	49          	rola
    e771:	54          	lsrb
    e772:	2f 53       	ble	0x0xe7c7
    e774:	45          	.byte	0x45
    e775:	43          	coma
    e776:	20 20       	bra	0x0xe798
    e778:	20 20       	bra	0x0xe79a
    e77a:	20 31       	bra	0x0xe7ad
    e77c:	4b          	.byte	0x4b
    e77d:	20 35       	bra	0x0xe7b4
    e77f:	30          	tsx
    e780:	30          	tsx
    e781:	20 31       	bra	0x0xe7b4
    e783:	32          	pula
    e784:	35          	txs
    e785:	36          	psha
    e786:	32          	pula
    e787:	2e 35       	bgt	0x0xe7be
    e789:	7d 01 1d    	tst	0x11d
    e78c:	26 0a       	bne	0x0xe798
    e78e:	bd ec 01    	jsr	0xec01
    e791:	ce 10 23    	ldx	#0x1023
    e794:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe794
    e798:	bd ea d7    	jsr	0xead7
    e79b:	ce 01 20    	ldx	#0x120
    e79e:	18 ce e7 bb 	ldy	#0xe7bb
    e7a2:	c6 20       	ldab	#0x20
    e7a4:	18 a6 00    	ldaa	0x0,y
    e7a7:	a7 00       	staa	0x0,x
    e7a9:	08          	inx
    e7aa:	18 08       	iny
    e7ac:	5a          	decb
    e7ad:	26 f5       	bne	0x0xe7a4
    e7af:	96 f0       	ldaa	*0xf0
    e7b1:	4c          	inca
    e7b2:	ce 01 31    	ldx	#0x131
    e7b5:	bd eb 8a    	jsr	0xeb8a
    e7b8:	7e ea 14    	jmp	0xea14
    e7bb:	53          	comb
    e7bc:	45          	.byte	0x45
    e7bd:	54          	lsrb
    e7be:	20 23       	bra	0x0xe7e3
    e7c0:	20 4f       	bra	0x0xe811
    e7c2:	46          	rora
    e7c3:	20 56       	bra	0x0xe81b
    e7c5:	4f          	clra
    e7c6:	49          	rola
    e7c7:	43          	coma
    e7c8:	45          	.byte	0x45
    e7c9:	53          	comb
    e7ca:	20 20       	bra	0x0xe7ec
    e7cc:	20 20       	bra	0x0xe7ee
    e7ce:	20 20       	bra	0x0xe7f0
    e7d0:	20 20       	bra	0x0xe7f2
    e7d2:	20 20       	bra	0x0xe7f4
    e7d4:	20 20       	bra	0x0xe7f6
    e7d6:	20 20       	bra	0x0xe7f8
    e7d8:	20 20       	bra	0x0xe7fa
    e7da:	20 7d       	bra	0x0xe859
    e7dc:	01          	nop
    e7dd:	1d 26 0a    	bclr	0x26,x, #0x0a
    e7e0:	bd ec 01    	jsr	0xec01
    e7e3:	ce 10 23    	ldx	#0x1023
    e7e6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe7e6
    e7ea:	bd ea d7    	jsr	0xead7
    e7ed:	ce 01 20    	ldx	#0x120
    e7f0:	18 ce e8 c5 	ldy	#0xe8c5
    e7f4:	c6 20       	ldab	#0x20
    e7f6:	b6 50 00    	ldaa	0x5000
    e7f9:	81 06       	cmpa	#0x6
    e7fb:	25 04       	bcs	0x0xe801
    e7fd:	18 ce e8 e5 	ldy	#0xe8e5
    e801:	18 a6 00    	ldaa	0x0,y
    e804:	a7 00       	staa	0x0,x
    e806:	08          	inx
    e807:	18 08       	iny
    e809:	5a          	decb
    e80a:	26 f5       	bne	0x0xe801
    e80c:	b6 01 6b    	ldaa	0x16b
    e80f:	8b 31       	adda	#0x31
    e811:	b7 01 25    	staa	0x125
    e814:	ce 50 01    	ldx	#0x5001
    e817:	b6 01 6b    	ldaa	0x16b
    e81a:	c6 04       	ldab	#0x4
    e81c:	3d          	mul
    e81d:	3a          	abx
    e81e:	a6 00       	ldaa	0x0,x
    e820:	8b 41       	adda	#0x41
    e822:	b7 01 32    	staa	0x132
    e825:	a6 01       	ldaa	0x1,x
    e827:	3c          	pshx
    e828:	4c          	inca
    e829:	ce 01 34    	ldx	#0x134
    e82c:	bd eb 8a    	jsr	0xeb8a
    e82f:	b6 50 00    	ldaa	0x5000
    e832:	81 06       	cmpa	#0x6
    e834:	27 0c       	beq	0x0xe842
    e836:	38          	pulx
    e837:	a6 03       	ldaa	0x3,x
    e839:	3c          	pshx
    e83a:	ce 01 3c    	ldx	#0x13c
    e83d:	bd eb 8a    	jsr	0xeb8a
    e840:	20 42       	bra	0x0xe884
    e842:	38          	pulx
    e843:	7d 01 6b    	tst	0x16b
    e846:	26 06       	bne	0x0xe84e
    e848:	96 f0       	ldaa	*0xf0
    e84a:	4c          	inca
    e84b:	b7 50 23    	staa	0x5023
    e84e:	e6 02       	ldab	0x2,x
    e850:	c4 40       	andb	#0x40
    e852:	f7 01 7d    	stab	0x17d
    e855:	e6 02       	ldab	0x2,x
    e857:	c4 3f       	andb	#0x3f
    e859:	b6 50 23    	ldaa	0x5023
    e85c:	11          	cba
    e85d:	24 03       	bcc	0x0xe862
    e85f:	5a          	decb
    e860:	20 fa       	bra	0x0xe85c
    e862:	10          	sba
    e863:	b7 50 23    	staa	0x5023
    e866:	37          	pshb
    e867:	fa 01 7d    	orab	0x17d
    e86a:	e7 02       	stab	0x2,x
    e86c:	32          	pula
    e86d:	3c          	pshx
    e86e:	ce 01 38    	ldx	#0x138
    e871:	bd eb 8a    	jsr	0xeb8a
    e874:	38          	pulx
    e875:	e6 03       	ldab	0x3,x
    e877:	c4 01       	andb	#0x1
    e879:	3c          	pshx
    e87a:	ce e9 05    	ldx	#0xe905
    e87d:	18 ce 01 3c 	ldy	#0x13c
    e881:	bd ec 29    	jsr	0xec29
    e884:	38          	pulx
    e885:	e6 00       	ldab	0x0,x
    e887:	c1 01       	cmpb	#0x1
    e889:	23 17       	bls	0x0xe8a2
    e88b:	a6 01       	ldaa	0x1,x
    e88d:	d7 f9       	stab	*0xf9
    e88f:	bd ad c4    	jsr	0xadc4
    e892:	c6 04       	ldab	#0x4
    e894:	d7 f9       	stab	*0xf9
    e896:	bd a5 50    	jsr	0xa550
    e899:	bd a5 6b    	jsr	0xa56b
    e89c:	bd ad c4    	jsr	0xadc4
    e89f:	7e ea 14    	jmp	0xea14
    e8a2:	4f          	clra
    e8a3:	b7 10 22    	staa	0x1022
    e8a6:	b6 10 2d    	ldaa	0x102d
    e8a9:	36          	psha
    e8aa:	84 7f       	anda	#0x7f
    e8ac:	b7 10 2d    	staa	0x102d
    e8af:	a6 00       	ldaa	0x0,x
    e8b1:	e6 01       	ldab	0x1,x
    e8b3:	bd 79 00    	jsr	0x7900
    e8b6:	32          	pula
    e8b7:	b7 10 2d    	staa	0x102d
    e8ba:	86 80       	ldaa	#0x80
    e8bc:	b7 10 22    	staa	0x1022
    e8bf:	bd a5 6b    	jsr	0xa56b
    e8c2:	7e ea 14    	jmp	0xea14
    e8c5:	50          	negb
    e8c6:	41          	.byte	0x41
    e8c7:	54          	lsrb
    e8c8:	43          	coma
    e8c9:	48          	asla
    e8ca:	20 20       	bra	0x0xe8ec
    e8cc:	20 20       	bra	0x0xe8ee
    e8ce:	20 20       	bra	0x0xe8f0
    e8d0:	20 56       	bra	0x0xe928
    e8d2:	4f          	clra
    e8d3:	4c          	inca
    e8d4:	20 20       	bra	0x0xe8f6
    e8d6:	20 20       	bra	0x0xe8f8
    e8d8:	20 20       	bra	0x0xe8fa
    e8da:	20 20       	bra	0x0xe8fc
    e8dc:	20 20       	bra	0x0xe8fe
    e8de:	20 20       	bra	0x0xe900
    e8e0:	20 20       	bra	0x0xe902
    e8e2:	20 20       	bra	0x0xe904
    e8e4:	20 50       	bra	0x0xe936
    e8e6:	41          	.byte	0x41
    e8e7:	54          	lsrb
    e8e8:	43          	coma
    e8e9:	48          	asla
    e8ea:	20 20       	bra	0x0xe90c
    e8ec:	23 56       	bls	0x0xe944
    e8ee:	43          	coma
    e8ef:	53          	comb
    e8f0:	20 54       	bra	0x0xe946
    e8f2:	59          	rolb
    e8f3:	50          	negb
    e8f4:	45          	.byte	0x45
    e8f5:	20 20       	bra	0x0xe917
    e8f7:	20 20       	bra	0x0xe919
    e8f9:	20 20       	bra	0x0xe91b
    e8fb:	20 20       	bra	0x0xe91d
    e8fd:	20 20       	bra	0x0xe91f
    e8ff:	20 20       	bra	0x0xe921
    e901:	20 20       	bra	0x0xe923
    e903:	20 20       	bra	0x0xe925
    e905:	4d          	tsta
    e906:	4f          	clra
    e907:	4e          	.byte	0x4e
    e908:	50          	negb
    e909:	4c          	inca
    e90a:	59          	rolb
    e90b:	7d 01 1d    	tst	0x11d
    e90e:	26 0a       	bne	0x0xe91a
    e910:	bd ec 01    	jsr	0xec01
    e913:	ce 10 23    	ldx	#0x1023
    e916:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe916
    e91a:	bd ea d7    	jsr	0xead7
    e91d:	ce 01 20    	ldx	#0x120
    e920:	18 ce e9 5c 	ldy	#0xe95c
    e924:	c6 20       	ldab	#0x20
    e926:	18 a6 00    	ldaa	0x0,y
    e929:	a7 00       	staa	0x0,x
    e92b:	08          	inx
    e92c:	18 08       	iny
    e92e:	5a          	decb
    e92f:	26 f5       	bne	0x0xe926
    e931:	ce 50 03    	ldx	#0x5003
    e934:	b6 01 6b    	ldaa	0x16b
    e937:	c6 04       	ldab	#0x4
    e939:	3d          	mul
    e93a:	3a          	abx
    e93b:	7d 50 23    	tst	0x5023
    e93e:	26 07       	bne	0x0xe947
    e940:	e6 00       	ldab	0x0,x
    e942:	c4 3f       	andb	#0x3f
    e944:	f7 50 23    	stab	0x5023
    e947:	e6 00       	ldab	0x0,x
    e949:	c4 40       	andb	#0x40
    e94b:	27 02       	beq	0x0xe94f
    e94d:	c6 01       	ldab	#0x1
    e94f:	18 ce 01 31 	ldy	#0x131
    e953:	ce e9 7c    	ldx	#0xe97c
    e956:	bd ec 29    	jsr	0xec29
    e959:	7e ea 14    	jmp	0xea14
    e95c:	32          	pula
    e95d:	4d          	tsta
    e95e:	49          	rola
    e95f:	58          	aslb
    e960:	20 20       	bra	0x0xe982
    e962:	20 20       	bra	0x0xe984
    e964:	20 20       	bra	0x0xe986
    e966:	20 20       	bra	0x0xe988
    e968:	20 20       	bra	0x0xe98a
    e96a:	20 20       	bra	0x0xe98c
    e96c:	20 20       	bra	0x0xe98e
    e96e:	20 20       	bra	0x0xe990
    e970:	20 20       	bra	0x0xe992
    e972:	20 20       	bra	0x0xe994
    e974:	20 20       	bra	0x0xe996
    e976:	20 20       	bra	0x0xe998
    e978:	20 20       	bra	0x0xe99a
    e97a:	20 20       	bra	0x0xe99c
    e97c:	4f          	clra
    e97d:	46          	rora
    e97e:	46          	rora
    e97f:	20 4f       	bra	0x0xe9d0
    e981:	4e          	.byte	0x4e
    e982:	ce 10 23    	ldx	#0x1023
    e985:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe985
    e989:	bd ea d7    	jsr	0xead7
    e98c:	ce 01 20    	ldx	#0x120
    e98f:	18 ce e9 a3 	ldy	#0xe9a3
    e993:	c6 20       	ldab	#0x20
    e995:	18 a6 00    	ldaa	0x0,y
    e998:	a7 00       	staa	0x0,x
    e99a:	08          	inx
    e99b:	18 08       	iny
    e99d:	5a          	decb
    e99e:	26 f5       	bne	0x0xe995
    e9a0:	7e ea 14    	jmp	0xea14
    e9a3:	55          	.byte	0x55
    e9a4:	50          	negb
    e9a5:	4c          	inca
    e9a6:	4f          	clra
    e9a7:	41          	.byte	0x41
    e9a8:	44          	lsra
    e9a9:	20 52       	bra	0x0xe9fd
    e9ab:	41          	.byte	0x41
    e9ac:	4d          	tsta
    e9ad:	20 42       	bra	0x0xe9f1
    e9af:	4e          	.byte	0x4e
    e9b0:	4b          	.byte	0x4b
    e9b1:	20 43       	bra	0x0xe9f6
    e9b3:	20 20       	bra	0x0xe9d5
    e9b5:	20 20       	bra	0x0xe9d7
    e9b7:	54          	lsrb
    e9b8:	4f          	clra
    e9b9:	20 52       	bra	0x0xea0d
    e9bb:	4f          	clra
    e9bc:	4d          	tsta
    e9bd:	20 42       	bra	0x0xea01
    e9bf:	4e          	.byte	0x4e
    e9c0:	4b          	.byte	0x4b
    e9c1:	20 41       	bra	0x0xea04
    e9c3:	b6 01 2f    	ldaa	0x12f
    e9c6:	97 99       	staa	*0x99
    e9c8:	b6 01 3f    	ldaa	0x13f
    e9cb:	97 9a       	staa	*0x9a
    e9cd:	ce 10 23    	ldx	#0x1023
    e9d0:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe9d0
    e9d4:	bd ea d7    	jsr	0xead7
    e9d7:	ce 01 20    	ldx	#0x120
    e9da:	18 ce e9 f4 	ldy	#0xe9f4
    e9de:	c6 20       	ldab	#0x20
    e9e0:	18 a6 00    	ldaa	0x0,y
    e9e3:	a7 00       	staa	0x0,x
    e9e5:	08          	inx
    e9e6:	18 08       	iny
    e9e8:	5a          	decb
    e9e9:	26 f5       	bne	0x0xe9e0
    e9eb:	96 f2       	ldaa	*0xf2
    e9ed:	84 20       	anda	#0x20
    e9ef:	97 f2       	staa	*0xf2
    e9f1:	7e ea 14    	jmp	0xea14
    e9f4:	20 41       	bra	0x0xea37
    e9f6:	52          	.byte	0x52
    e9f7:	45          	.byte	0x45
    e9f8:	20 59       	bra	0x0xea53
    e9fa:	4f          	clra
    e9fb:	55          	.byte	0x55
    e9fc:	20 53       	bra	0x0xea51
    e9fe:	55          	.byte	0x55
    e9ff:	52          	.byte	0x52
    ea00:	45          	.byte	0x45
    ea01:	20 3f       	bra	0x0xea42
    ea03:	20 20       	bra	0x0xea25
    ea05:	20 20       	bra	0x0xea27
    ea07:	20 20       	bra	0x0xea29
    ea09:	20 20       	bra	0x0xea2b
    ea0b:	20 20       	bra	0x0xea2d
    ea0d:	20 20       	bra	0x0xea2f
    ea0f:	20 20       	bra	0x0xea31
    ea11:	4e          	.byte	0x4e
    ea12:	4f          	clra
    ea13:	20 7f       	bra	0x0xea94
    ea15:	01          	nop
    ea16:	1e 86 20 b7 	brset	0x86,x, #0x20, 0x0xe9d1
    ea1a:	01          	nop
    ea1b:	1c ce 10    	bset	0xce,x, #0x10
    ea1e:	23 1f       	bls	0x0xea3f
    ea20:	00          	bgnd
    ea21:	10          	sba
    ea22:	fc 7e ea    	ldd	0x7eea
    ea25:	26 8d       	bne	0x0xe9b4
    ea27:	03          	fdiv
    ea28:	7e d5 27    	jmp	0xd527
    ea2b:	ce 01 20    	ldx	#0x120
    ea2e:	f6 01 1c    	ldab	0x11c
    ea31:	5a          	decb
    ea32:	3a          	abx
    ea33:	a6 00       	ldaa	0x0,x
    ea35:	b7 10 47    	staa	0x1047
    ea38:	86 88       	ldaa	#0x88
    ea3a:	ba 10 00    	oraa	0x1000
    ea3d:	b7 10 00    	staa	0x1000
    ea40:	88 80       	eora	#0x80
    ea42:	b7 10 00    	staa	0x1000
    ea45:	88 08       	eora	#0x8
    ea47:	b7 10 00    	staa	0x1000
    ea4a:	37          	pshb
    ea4b:	bd ea a3    	jsr	0xeaa3
    ea4e:	33          	pulb
    ea4f:	f7 01 1c    	stab	0x11c
    ea52:	c1 10       	cmpb	#0x10
    ea54:	27 01       	beq	0x0xea57
    ea56:	39          	rts
    ea57:	18 ce 10 23 	ldy	#0x1023
    ea5b:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xea5b
    ea5f:	fb 
    ea60:	86 8f       	ldaa	#0x8f
    ea62:	b7 10 47    	staa	0x1047
    ea65:	86 80       	ldaa	#0x80
    ea67:	ba 10 00    	oraa	0x1000
    ea6a:	b7 10 00    	staa	0x1000
    ea6d:	88 80       	eora	#0x80
    ea6f:	b7 10 00    	staa	0x1000
    ea72:	bd ea a3    	jsr	0xeaa3
    ea75:	39          	rts
    ea76:	f6 10 23    	ldab	0x1023
    ea79:	c5 10       	bitb	#0x10
    ea7b:	27 f9       	beq	0x0xea76
    ea7d:	a6 00       	ldaa	0x0,x
    ea7f:	b7 10 47    	staa	0x1047
    ea82:	86 88       	ldaa	#0x88
    ea84:	ba 10 00    	oraa	0x1000
    ea87:	b7 10 00    	staa	0x1000
    ea8a:	88 80       	eora	#0x80
    ea8c:	b7 10 00    	staa	0x1000
    ea8f:	88 08       	eora	#0x8
    ea91:	b7 10 00    	staa	0x1000
    ea94:	86 10       	ldaa	#0x10
    ea96:	b7 10 23    	staa	0x1023
    ea99:	fc 10 0e    	ldd	0x100e
    ea9c:	c3 00 f0    	addd	#0xf0
    ea9f:	fd 10 1c    	std	0x101c
    eaa2:	39          	rts
    eaa3:	86 10       	ldaa	#0x10
    eaa5:	b7 10 23    	staa	0x1023
    eaa8:	fc 10 0e    	ldd	0x100e
    eaab:	c3 00 f0    	addd	#0xf0
    eaae:	fd 10 1c    	std	0x101c
    eab1:	39          	rts
    eab2:	f6 10 23    	ldab	0x1023
    eab5:	c5 10       	bitb	#0x10
    eab7:	27 f9       	beq	0x0xeab2
    eab9:	b6 01 1e    	ldaa	0x11e
    eabc:	81 0f       	cmpa	#0xf
    eabe:	23 02       	bls	0x0xeac2
    eac0:	8b 30       	adda	#0x30
    eac2:	8a 80       	oraa	#0x80
    eac4:	b7 10 47    	staa	0x1047
    eac7:	86 80       	ldaa	#0x80
    eac9:	ba 10 00    	oraa	0x1000
    eacc:	b7 10 00    	staa	0x1000
    eacf:	88 80       	eora	#0x80
    ead1:	b7 10 00    	staa	0x1000
    ead4:	7e ea a3    	jmp	0xeaa3
    ead7:	ce 10 23    	ldx	#0x1023
    eada:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xeada
    eade:	86 cf       	ldaa	#0xcf
    eae0:	b7 10 47    	staa	0x1047
    eae3:	86 80       	ldaa	#0x80
    eae5:	ba 10 00    	oraa	0x1000
    eae8:	b7 10 00    	staa	0x1000
    eaeb:	01          	nop
    eaec:	88 80       	eora	#0x80
    eaee:	b7 10 00    	staa	0x1000
    eaf1:	7e ea a3    	jmp	0xeaa3
    eaf4:	b6 01 1e    	ldaa	0x11e
    eaf7:	81 03       	cmpa	#0x3
    eaf9:	22 04       	bhi	0x0xeaff
    eafb:	86 03       	ldaa	#0x3
    eafd:	20 22       	bra	0x0xeb21
    eaff:	81 09       	cmpa	#0x9
    eb01:	22 04       	bhi	0x0xeb07
    eb03:	86 09       	ldaa	#0x9
    eb05:	20 1a       	bra	0x0xeb21
    eb07:	81 0e       	cmpa	#0xe
    eb09:	22 04       	bhi	0x0xeb0f
    eb0b:	86 0e       	ldaa	#0xe
    eb0d:	20 12       	bra	0x0xeb21
    eb0f:	81 13       	cmpa	#0x13
    eb11:	22 04       	bhi	0x0xeb17
    eb13:	86 13       	ldaa	#0x13
    eb15:	20 0a       	bra	0x0xeb21
    eb17:	81 19       	cmpa	#0x19
    eb19:	22 04       	bhi	0x0xeb1f
    eb1b:	86 19       	ldaa	#0x19
    eb1d:	20 02       	bra	0x0xeb21
    eb1f:	86 1e       	ldaa	#0x1e
    eb21:	b7 01 1e    	staa	0x11e
    eb24:	7e ea b2    	jmp	0xeab2
    eb27:	86 02       	ldaa	#0x2
    eb29:	b1 01 1e    	cmpa	0x11e
    eb2c:	23 06       	bls	0x0xeb34
    eb2e:	b7 01 1e    	staa	0x11e
    eb31:	7e ea b2    	jmp	0xeab2
    eb34:	86 06       	ldaa	#0x6
    eb36:	b1 01 1e    	cmpa	0x11e
    eb39:	23 06       	bls	0x0xeb41
    eb3b:	b7 01 1e    	staa	0x11e
    eb3e:	7e ea b2    	jmp	0xeab2
    eb41:	86 0a       	ldaa	#0xa
    eb43:	b1 01 1e    	cmpa	0x11e
    eb46:	23 06       	bls	0x0xeb4e
    eb48:	b7 01 1e    	staa	0x11e
    eb4b:	7e ea b2    	jmp	0xeab2
    eb4e:	86 0e       	ldaa	#0xe
    eb50:	b1 01 1e    	cmpa	0x11e
    eb53:	23 06       	bls	0x0xeb5b
    eb55:	b7 01 1e    	staa	0x11e
    eb58:	7e ea b2    	jmp	0xeab2
    eb5b:	86 12       	ldaa	#0x12
    eb5d:	b1 01 1e    	cmpa	0x11e
    eb60:	23 06       	bls	0x0xeb68
    eb62:	b7 01 1e    	staa	0x11e
    eb65:	7e ea b2    	jmp	0xeab2
    eb68:	86 16       	ldaa	#0x16
    eb6a:	b1 01 1e    	cmpa	0x11e
    eb6d:	23 06       	bls	0x0xeb75
    eb6f:	b7 01 1e    	staa	0x11e
    eb72:	7e ea b2    	jmp	0xeab2
    eb75:	86 1a       	ldaa	#0x1a
    eb77:	b1 01 1e    	cmpa	0x11e
    eb7a:	23 06       	bls	0x0xeb82
    eb7c:	b7 01 1e    	staa	0x11e
    eb7f:	7e ea b2    	jmp	0xeab2
    eb82:	86 1e       	ldaa	#0x1e
    eb84:	b7 01 1e    	staa	0x11e
    eb87:	7e ea b2    	jmp	0xeab2
    eb8a:	80 64       	suba	#0x64
    eb8c:	24 09       	bcc	0x0xeb97
    eb8e:	8b 64       	adda	#0x64
    eb90:	c6 20       	ldab	#0x20
    eb92:	e7 00       	stab	0x0,x
    eb94:	08          	inx
    eb95:	20 05       	bra	0x0xeb9c
    eb97:	c6 31       	ldab	#0x31
    eb99:	e7 00       	stab	0x0,x
    eb9b:	08          	inx
    eb9c:	80 0a       	suba	#0xa
    eb9e:	24 14       	bcc	0x0xebb4
    eba0:	8b 0a       	adda	#0xa
    eba2:	c1 20       	cmpb	#0x20
    eba4:	27 07       	beq	0x0xebad
    eba6:	c6 30       	ldab	#0x30
    eba8:	e7 00       	stab	0x0,x
    ebaa:	08          	inx
    ebab:	20 14       	bra	0x0xebc1
    ebad:	c6 20       	ldab	#0x20
    ebaf:	e7 00       	stab	0x0,x
    ebb1:	08          	inx
    ebb2:	20 0d       	bra	0x0xebc1
    ebb4:	5f          	clrb
    ebb5:	5c          	incb
    ebb6:	80 0a       	suba	#0xa
    ebb8:	24 fb       	bcc	0x0xebb5
    ebba:	8b 0a       	adda	#0xa
    ebbc:	cb 30       	addb	#0x30
    ebbe:	e7 00       	stab	0x0,x
    ebc0:	08          	inx
    ebc1:	8b 30       	adda	#0x30
    ebc3:	a7 00       	staa	0x0,x
    ebc5:	39          	rts
    ebc6:	80 40       	suba	#0x40
    ebc8:	25 10       	bcs	0x0xebda
    ebca:	26 05       	bne	0x0xebd1
    ebcc:	8d bc       	bsr	0x0xeb8a
    ebce:	09          	dex
    ebcf:	09          	dex
    ebd0:	39          	rts
    ebd1:	8d b7       	bsr	0x0xeb8a
    ebd3:	09          	dex
    ebd4:	09          	dex
    ebd5:	86 2b       	ldaa	#0x2b
    ebd7:	a7 00       	staa	0x0,x
    ebd9:	39          	rts
    ebda:	40          	nega
    ebdb:	8d ad       	bsr	0x0xeb8a
    ebdd:	09          	dex
    ebde:	09          	dex
    ebdf:	86 2d       	ldaa	#0x2d
    ebe1:	a7 00       	staa	0x0,x
    ebe3:	39          	rts
    ebe4:	a6 02       	ldaa	0x2,x
    ebe6:	80 30       	suba	#0x30
    ebe8:	e6 01       	ldab	0x1,x
    ebea:	c1 20       	cmpb	#0x20
    ebec:	26 01       	bne	0x0xebef
    ebee:	39          	rts
    ebef:	c0 30       	subb	#0x30
    ebf1:	36          	psha
    ebf2:	86 0a       	ldaa	#0xa
    ebf4:	3d          	mul
    ebf5:	32          	pula
    ebf6:	1b          	aba
    ebf7:	e6 00       	ldab	0x0,x
    ebf9:	c1 20       	cmpb	#0x20
    ebfb:	26 01       	bne	0x0xebfe
    ebfd:	39          	rts
    ebfe:	8b 64       	adda	#0x64
    ec00:	39          	rts
    ec01:	ce 10 23    	ldx	#0x1023
    ec04:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec04
    ec08:	86 f7       	ldaa	#0xf7
    ec0a:	b4 10 00    	anda	0x1000
    ec0d:	b7 10 00    	staa	0x1000
    ec10:	86 0e       	ldaa	#0xe
    ec12:	b7 10 47    	staa	0x1047
    ec15:	86 80       	ldaa	#0x80
    ec17:	ba 10 00    	oraa	0x1000
    ec1a:	b7 10 00    	staa	0x1000
    ec1d:	01          	nop
    ec1e:	88 80       	eora	#0x80
    ec20:	b7 10 00    	staa	0x1000
    ec23:	7c 01 1d    	inc	0x11d
    ec26:	7e ea a3    	jmp	0xeaa3
    ec29:	86 03       	ldaa	#0x3
    ec2b:	3d          	mul
    ec2c:	3a          	abx
    ec2d:	c6 03       	ldab	#0x3
    ec2f:	a6 00       	ldaa	0x0,x
    ec31:	18 a7 00    	staa	0x0,y
    ec34:	5a          	decb
    ec35:	27 05       	beq	0x0xec3c
    ec37:	08          	inx
    ec38:	18 08       	iny
    ec3a:	20 f3       	bra	0x0xec2f
    ec3c:	39          	rts
    ec3d:	86 04       	ldaa	#0x4
    ec3f:	3d          	mul
    ec40:	3a          	abx
    ec41:	c6 04       	ldab	#0x4
    ec43:	a6 00       	ldaa	0x0,x
    ec45:	18 a7 00    	staa	0x0,y
    ec48:	5a          	decb
    ec49:	27 05       	beq	0x0xec50
    ec4b:	08          	inx
    ec4c:	18 08       	iny
    ec4e:	20 f3       	bra	0x0xec43
    ec50:	39          	rts
    ec51:	c6 c8       	ldab	#0xc8
    ec53:	3d          	mul
    ec54:	bd eb 8a    	jsr	0xeb8a
    ec57:	86 25       	ldaa	#0x25
    ec59:	09          	dex
    ec5a:	09          	dex
    ec5b:	a7 00       	staa	0x0,x
    ec5d:	39          	rts
    ec5e:	b6 01 1e    	ldaa	0x11e
    ec61:	81 0e       	cmpa	#0xe
    ec63:	22 08       	bhi	0x0xec6d
    ec65:	8b 10       	adda	#0x10
    ec67:	b7 01 1e    	staa	0x11e
    ec6a:	7e ea b2    	jmp	0xeab2
    ec6d:	80 10       	suba	#0x10
    ec6f:	b7 01 1e    	staa	0x11e
    ec72:	7e ea b2    	jmp	0xeab2
    ec75:	b6 01 7f    	ldaa	0x17f
    ec78:	2b 1e       	bmi	0x0xec98
    ec7a:	b6 01 1e    	ldaa	0x11e
    ec7d:	81 0e       	cmpa	#0xe
    ec7f:	22 0a       	bhi	0x0xec8b
    ec81:	27 32       	beq	0x0xecb5
    ec83:	86 0e       	ldaa	#0xe
    ec85:	b7 01 1e    	staa	0x11e
    ec88:	7e ea b2    	jmp	0xeab2
    ec8b:	81 1e       	cmpa	#0x1e
    ec8d:	26 01       	bne	0x0xec90
    ec8f:	39          	rts
    ec90:	86 1e       	ldaa	#0x1e
    ec92:	b7 01 1e    	staa	0x11e
    ec95:	7e ea b2    	jmp	0xeab2
    ec98:	b6 01 1e    	ldaa	0x11e
    ec9b:	81 19       	cmpa	#0x19
    ec9d:	25 0a       	bcs	0x0xeca9
    ec9f:	27 14       	beq	0x0xecb5
    eca1:	86 19       	ldaa	#0x19
    eca3:	b7 01 1e    	staa	0x11e
    eca6:	7e ea b2    	jmp	0xeab2
    eca9:	81 09       	cmpa	#0x9
    ecab:	27 08       	beq	0x0xecb5
    ecad:	86 09       	ldaa	#0x9
    ecaf:	b7 01 1e    	staa	0x11e
    ecb2:	7e ea b2    	jmp	0xeab2
    ecb5:	39          	rts
    ecb6:	b6 01 7f    	ldaa	0x17f
    ecb9:	2b 35       	bmi	0x0xecf0
    ecbb:	b6 01 1e    	ldaa	0x11e
    ecbe:	81 13       	cmpa	#0x13
    ecc0:	25 16       	bcs	0x0xecd8
    ecc2:	22 08       	bhi	0x0xeccc
    ecc4:	86 19       	ldaa	#0x19
    ecc6:	b7 01 1e    	staa	0x11e
    ecc9:	7e ea b2    	jmp	0xeab2
    eccc:	81 19       	cmpa	#0x19
    ecce:	22 57       	bhi	0x0xed27
    ecd0:	86 1e       	ldaa	#0x1e
    ecd2:	b7 01 1e    	staa	0x11e
    ecd5:	7e ea b2    	jmp	0xeab2
    ecd8:	81 03       	cmpa	#0x3
    ecda:	22 08       	bhi	0x0xece4
    ecdc:	86 09       	ldaa	#0x9
    ecde:	b7 01 1e    	staa	0x11e
    ece1:	7e ea b2    	jmp	0xeab2
    ece4:	81 09       	cmpa	#0x9
    ece6:	22 3f       	bhi	0x0xed27
    ece8:	86 0e       	ldaa	#0xe
    ecea:	b7 01 1e    	staa	0x11e
    eced:	7e ea b2    	jmp	0xeab2
    ecf0:	b6 01 1e    	ldaa	0x11e
    ecf3:	81 0f       	cmpa	#0xf
    ecf5:	25 18       	bcs	0x0xed0f
    ecf7:	81 1a       	cmpa	#0x1a
    ecf9:	25 08       	bcs	0x0xed03
    ecfb:	86 19       	ldaa	#0x19
    ecfd:	b7 01 1e    	staa	0x11e
    ed00:	7e ea b2    	jmp	0xeab2
    ed03:	81 19       	cmpa	#0x19
    ed05:	26 20       	bne	0x0xed27
    ed07:	86 13       	ldaa	#0x13
    ed09:	b7 01 1e    	staa	0x11e
    ed0c:	7e ea b2    	jmp	0xeab2
    ed0f:	81 0e       	cmpa	#0xe
    ed11:	25 08       	bcs	0x0xed1b
    ed13:	86 09       	ldaa	#0x9
    ed15:	b7 01 1e    	staa	0x11e
    ed18:	7e ea b2    	jmp	0xeab2
    ed1b:	81 09       	cmpa	#0x9
    ed1d:	26 08       	bne	0x0xed27
    ed1f:	86 03       	ldaa	#0x3
    ed21:	b7 01 1e    	staa	0x11e
    ed24:	7e ea b2    	jmp	0xeab2
    ed27:	39          	rts
    ed28:	b6 01 7f    	ldaa	0x17f
    ed2b:	2b 3a       	bmi	0x0xed67
    ed2d:	b6 01 1e    	ldaa	0x11e
    ed30:	81 12       	cmpa	#0x12
    ed32:	25 16       	bcs	0x0xed4a
    ed34:	22 04       	bhi	0x0xed3a
    ed36:	86 16       	ldaa	#0x16
    ed38:	20 26       	bra	0x0xed60
    ed3a:	81 16       	cmpa	#0x16
    ed3c:	22 04       	bhi	0x0xed42
    ed3e:	86 1a       	ldaa	#0x1a
    ed40:	20 1e       	bra	0x0xed60
    ed42:	81 1a       	cmpa	#0x1a
    ed44:	22 20       	bhi	0x0xed66
    ed46:	86 1e       	ldaa	#0x1e
    ed48:	20 16       	bra	0x0xed60
    ed4a:	81 02       	cmpa	#0x2
    ed4c:	22 04       	bhi	0x0xed52
    ed4e:	86 06       	ldaa	#0x6
    ed50:	20 0e       	bra	0x0xed60
    ed52:	81 06       	cmpa	#0x6
    ed54:	22 04       	bhi	0x0xed5a
    ed56:	86 0a       	ldaa	#0xa
    ed58:	20 06       	bra	0x0xed60
    ed5a:	81 0a       	cmpa	#0xa
    ed5c:	22 08       	bhi	0x0xed66
    ed5e:	86 0e       	ldaa	#0xe
    ed60:	b7 01 1e    	staa	0x11e
    ed63:	7e ea b2    	jmp	0xeab2
    ed66:	39          	rts
    ed67:	b6 01 1e    	ldaa	0x11e
    ed6a:	81 0e       	cmpa	#0xe
    ed6c:	22 16       	bhi	0x0xed84
    ed6e:	25 04       	bcs	0x0xed74
    ed70:	86 0a       	ldaa	#0xa
    ed72:	20 ec       	bra	0x0xed60
    ed74:	81 0a       	cmpa	#0xa
    ed76:	25 04       	bcs	0x0xed7c
    ed78:	86 06       	ldaa	#0x6
    ed7a:	20 e4       	bra	0x0xed60
    ed7c:	81 06       	cmpa	#0x6
    ed7e:	25 e6       	bcs	0x0xed66
    ed80:	86 02       	ldaa	#0x2
    ed82:	20 dc       	bra	0x0xed60
    ed84:	81 1e       	cmpa	#0x1e
    ed86:	25 04       	bcs	0x0xed8c
    ed88:	86 1a       	ldaa	#0x1a
    ed8a:	20 d4       	bra	0x0xed60
    ed8c:	81 1a       	cmpa	#0x1a
    ed8e:	25 04       	bcs	0x0xed94
    ed90:	86 16       	ldaa	#0x16
    ed92:	20 cc       	bra	0x0xed60
    ed94:	81 16       	cmpa	#0x16
    ed96:	25 ce       	bcs	0x0xed66
    ed98:	86 12       	ldaa	#0x12
    ed9a:	20 c4       	bra	0x0xed60
    ed9c:	ce 10 23    	ldx	#0x1023
    ed9f:	1f 00 10 f9 	brclr	0x0,x, #0x10, 0x0xed9c
    eda3:	86 04       	ldaa	#0x4
    eda5:	b7 01 1c    	staa	0x11c
    eda8:	86 38       	ldaa	#0x38
    edaa:	b7 10 47    	staa	0x1047
    edad:	86 80       	ldaa	#0x80
    edaf:	ba 10 00    	oraa	0x1000
    edb2:	b7 10 00    	staa	0x1000
    edb5:	01          	nop
    edb6:	88 80       	eora	#0x80
    edb8:	b7 10 00    	staa	0x1000
    edbb:	96 10       	ldaa	*0x10
    edbd:	b7 10 23    	staa	0x1023
    edc0:	fc 10 0e    	ldd	0x100e
    edc3:	c3 20 08    	addd	#0x2008
    edc6:	fd 10 1c    	std	0x101c
    edc9:	ce 10 23    	ldx	#0x1023
    edcc:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xedcc
    edd0:	7a 01 1c    	dec	0x11c
    edd3:	26 d3       	bne	0x0xeda8
    edd5:	86 08       	ldaa	#0x8
    edd7:	b7 10 47    	staa	0x1047
    edda:	86 80       	ldaa	#0x80
    eddc:	ba 10 00    	oraa	0x1000
    eddf:	b7 10 00    	staa	0x1000
    ede2:	01          	nop
    ede3:	88 80       	eora	#0x80
    ede5:	b7 10 00    	staa	0x1000
    ede8:	86 10       	ldaa	#0x10
    edea:	b7 10 23    	staa	0x1023
    eded:	fc 10 0e    	ldd	0x100e
    edf0:	c3 00 f0    	addd	#0xf0
    edf3:	fd 10 1c    	std	0x101c
    edf6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xedf6
    edfa:	86 01       	ldaa	#0x1
    edfc:	b7 10 47    	staa	0x1047
    edff:	86 80       	ldaa	#0x80
    ee01:	ba 10 00    	oraa	0x1000
    ee04:	b7 10 00    	staa	0x1000
    ee07:	01          	nop
    ee08:	88 80       	eora	#0x80
    ee0a:	b7 10 00    	staa	0x1000
    ee0d:	86 10       	ldaa	#0x10
    ee0f:	b7 10 23    	staa	0x1023
    ee12:	fc 10 0e    	ldd	0x100e
    ee15:	c3 26 48    	addd	#0x2648
    ee18:	fd 10 1c    	std	0x101c
    ee1b:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xee1b
    ee1f:	86 04       	ldaa	#0x4
    ee21:	b7 10 47    	staa	0x1047
    ee24:	86 80       	ldaa	#0x80
    ee26:	ba 10 00    	oraa	0x1000
    ee29:	b7 10 00    	staa	0x1000
    ee2c:	01          	nop
    ee2d:	88 80       	eora	#0x80
    ee2f:	b7 10 00    	staa	0x1000
    ee32:	86 10       	ldaa	#0x10
    ee34:	b7 10 23    	staa	0x1023
    ee37:	fc 10 0e    	ldd	0x100e
    ee3a:	c3 00 f0    	addd	#0xf0
    ee3d:	fd 10 1c    	std	0x101c
    ee40:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xee40
    ee44:	86 0c       	ldaa	#0xc
    ee46:	b7 10 47    	staa	0x1047
    ee49:	b6 10 00    	ldaa	0x1000
    ee4c:	8a 80       	oraa	#0x80
    ee4e:	b7 10 00    	staa	0x1000
    ee51:	88 80       	eora	#0x80
    ee53:	b7 10 00    	staa	0x1000
    ee56:	7f 01 1d    	clr	0x11d
    ee59:	86 10       	ldaa	#0x10
    ee5b:	b7 10 23    	staa	0x1023
    ee5e:	fc 10 0e    	ldd	0x100e
    ee61:	c3 00 f0    	addd	#0xf0
    ee64:	fd 10 1c    	std	0x101c
    ee67:	ce 01 20    	ldx	#0x120
    ee6a:	18 ce ef 3c 	ldy	#0xef3c
    ee6e:	c6 20       	ldab	#0x20
    ee70:	18 a6 00    	ldaa	0x0,y
    ee73:	a7 00       	staa	0x0,x
    ee75:	08          	inx
    ee76:	18 08       	iny
    ee78:	5a          	decb
    ee79:	26 f5       	bne	0x0xee70
    ee7b:	7f 01 1e    	clr	0x11e
    ee7e:	86 20       	ldaa	#0x20
    ee80:	b7 01 1c    	staa	0x11c
    ee83:	ce 10 23    	ldx	#0x1023
    ee86:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xee86
    ee8a:	86 cf       	ldaa	#0xcf
    ee8c:	b7 10 47    	staa	0x1047
    ee8f:	86 80       	ldaa	#0x80
    ee91:	ba 10 00    	oraa	0x1000
    ee94:	b7 10 00    	staa	0x1000
    ee97:	01          	nop
    ee98:	88 80       	eora	#0x80
    ee9a:	b7 10 00    	staa	0x1000
    ee9d:	86 10       	ldaa	#0x10
    ee9f:	b7 10 23    	staa	0x1023
    eea2:	fc 10 0e    	ldd	0x100e
    eea5:	c3 00 f0    	addd	#0xf0
    eea8:	fd 10 1c    	std	0x101c
    eeab:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xeeab
    eeaf:	ce 01 20    	ldx	#0x120
    eeb2:	f6 01 1c    	ldab	0x11c
    eeb5:	5a          	decb
    eeb6:	2a 22       	bpl	0x0xeeda
    eeb8:	86 cf       	ldaa	#0xcf
    eeba:	b7 10 47    	staa	0x1047
    eebd:	86 80       	ldaa	#0x80
    eebf:	ba 10 00    	oraa	0x1000
    eec2:	b7 10 00    	staa	0x1000
    eec5:	01          	nop
    eec6:	88 80       	eora	#0x80
    eec8:	b7 10 00    	staa	0x1000
    eecb:	86 10       	ldaa	#0x10
    eecd:	b7 10 23    	staa	0x1023
    eed0:	fc 10 0e    	ldd	0x100e
    eed3:	c3 00 f0    	addd	#0xf0
    eed6:	fd 10 1c    	std	0x101c
    eed9:	39          	rts
    eeda:	3a          	abx
    eedb:	a6 00       	ldaa	0x0,x
    eedd:	b7 10 47    	staa	0x1047
    eee0:	86 88       	ldaa	#0x88
    eee2:	ba 10 00    	oraa	0x1000
    eee5:	b7 10 00    	staa	0x1000
    eee8:	88 80       	eora	#0x80
    eeea:	b7 10 00    	staa	0x1000
    eeed:	88 08       	eora	#0x8
    eeef:	b7 10 00    	staa	0x1000
    eef2:	86 10       	ldaa	#0x10
    eef4:	b7 10 23    	staa	0x1023
    eef7:	37          	pshb
    eef8:	fc 10 0e    	ldd	0x100e
    eefb:	c3 00 f0    	addd	#0xf0
    eefe:	fd 10 1c    	std	0x101c
    ef01:	33          	pulb
    ef02:	f7 01 1c    	stab	0x11c
    ef05:	c1 10       	cmpb	#0x10
    ef07:	27 02       	beq	0x0xef0b
    ef09:	20 a4       	bra	0x0xeeaf
    ef0b:	18 ce 10 23 	ldy	#0x1023
    ef0f:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xef0f
    ef13:	fb 
    ef14:	86 8f       	ldaa	#0x8f
    ef16:	b7 10 47    	staa	0x1047
    ef19:	86 80       	ldaa	#0x80
    ef1b:	ba 10 00    	oraa	0x1000
    ef1e:	b7 10 00    	staa	0x1000
    ef21:	88 80       	eora	#0x80
    ef23:	b7 10 00    	staa	0x1000
    ef26:	86 10       	ldaa	#0x10
    ef28:	b7 10 23    	staa	0x1023
    ef2b:	fc 10 0e    	ldd	0x100e
    ef2e:	c3 00 f0    	addd	#0xf0
    ef31:	fd 10 1c    	std	0x101c
    ef34:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xef34
    ef38:	fb 
    ef39:	7e ee af    	jmp	0xeeaf
    ef3c:	53          	comb
    ef3d:	54          	lsrb
    ef3e:	55          	.byte	0x55
    ef3f:	44          	lsra
    ef40:	49          	rola
    ef41:	4f          	clra
    ef42:	20 20       	bra	0x0xef64
    ef44:	20 20       	bra	0x0xef66
    ef46:	4f          	clra
    ef47:	4d          	tsta
    ef48:	45          	.byte	0x45
    ef49:	47          	asra
    ef4a:	41          	.byte	0x41
    ef4b:	20 45       	bra	0x0xef92
    ef4d:	4c          	inca
    ef4e:	45          	.byte	0x45
    ef4f:	43          	coma
    ef50:	54          	lsrb
    ef51:	52          	.byte	0x52
    ef52:	4f          	clra
    ef53:	4e          	.byte	0x4e
    ef54:	49          	rola
    ef55:	43          	coma
    ef56:	53          	comb
    ef57:	20 56       	bra	0x0xefaf
    ef59:	32          	pula
    ef5a:	2e 41       	bgt	0x0xef9d
    ef5c:	00          	bgnd
    ef5d:	01          	nop
    ef5e:	01          	nop
    ef5f:	01          	nop
    ef60:	01          	nop
    ef61:	02          	idiv
    ef62:	02          	idiv
    ef63:	02          	idiv
    ef64:	02          	idiv
    ef65:	03          	fdiv
    ef66:	03          	fdiv
    ef67:	04          	lsrd
    ef68:	04          	lsrd
    ef69:	05          	asld
    ef6a:	05          	asld
    ef6b:	05          	asld
    ef6c:	06          	tap
    ef6d:	06          	tap
    ef6e:	06          	tap
    ef6f:	07          	tpa
    ef70:	07          	tpa
    ef71:	07          	tpa
    ef72:	08          	inx
    ef73:	08          	inx
    ef74:	08          	inx
    ef75:	09          	dex
    ef76:	09          	dex
    ef77:	0a          	clv
    ef78:	0a          	clv
    ef79:	0b          	sev
    ef7a:	0b          	sev
    ef7b:	0c          	clc
    ef7c:	0c          	clc
    ef7d:	0d          	sec
    ef7e:	0d          	sec
    ef7f:	0e          	cli
    ef80:	0e          	cli
    ef81:	0f          	sei
    ef82:	10          	sba
    ef83:	11          	cba
    ef84:	11          	cba
    ef85:	12 12 13 13 	brset	*0x12, #0x13, 0x0xef9c
    ef89:	14 14 15    	bset	*0x14, #0x15
    ef8c:	15 16 17    	bclr	*0x16, #0x17
    ef8f:	18 18       	.byte	0x18, 0x18
    ef91:	19          	daa
    ef92:	19          	daa
    ef93:	1a 1b       	.byte	0x1a, 0x1b
    ef95:	1c 1d 1e    	bset	0x1d,x, #0x1e
    ef98:	1f 20 21 22 	brclr	0x20,x, #0x21, 0x0xefbe
    ef9c:	23 24       	bls	0x0xefc2
    ef9e:	25 26       	bcs	0x0xefc6
    efa0:	27 28       	beq	0x0xefca
    efa2:	29 2a       	bvs	0x0xefce
    efa4:	2b 2c       	bmi	0x0xefd2
    efa6:	2d 2e       	blt	0x0xefd6
    efa8:	30          	tsx
    efa9:	31          	ins
    efaa:	32          	pula
    efab:	33          	pulb
    efac:	34          	des
    efad:	35          	txs
    efae:	36          	psha
    efaf:	37          	pshb
    efb0:	38          	pulx
    efb1:	39          	rts
    efb2:	3a          	abx
    efb3:	3b          	rti
    efb4:	3c          	pshx
    efb5:	3d          	mul
    efb6:	3f          	swi
    efb7:	41          	.byte	0x41
    efb8:	42          	.byte	0x42
    efb9:	43          	coma
    efba:	44          	lsra
    efbb:	45          	.byte	0x45
    efbc:	47          	asra
    efbd:	49          	rola
    efbe:	4a          	deca
    efbf:	4c          	inca
    efc0:	4e          	.byte	0x4e
    efc1:	50          	negb
    efc2:	52          	.byte	0x52
    efc3:	54          	lsrb
    efc4:	56          	rorb
    efc5:	58          	aslb
    efc6:	5a          	decb
    efc7:	5c          	incb
    efc8:	5e          	.byte	0x5e
    efc9:	60 61       	neg	0x61,x
    efcb:	63 64       	com	0x64,x
    efcd:	66 68       	ror	0x68,x
    efcf:	6a 6c       	dec	0x6c,x
    efd1:	6e 70       	jmp	0x70,x
    efd3:	72          	.byte	0x72
    efd4:	74 76 78    	lsr	0x7678
    efd7:	7a 7c 7d    	dec	0x7c7d
    efda:	7e 7f 00    	jmp	0x7f00
    efdd:	00          	bgnd
    efde:	40          	nega
    efdf:	00          	bgnd
    efe0:	00          	bgnd
    efe1:	00          	bgnd
    efe2:	00          	bgnd
    efe3:	00          	bgnd
    efe4:	5d          	tstb
    efe5:	00          	bgnd
    efe6:	00          	bgnd
    efe7:	00          	bgnd
    efe8:	00          	bgnd
    efe9:	03          	fdiv
    efea:	00          	bgnd
    efeb:	00          	bgnd
    efec:	00          	bgnd
    efed:	00          	bgnd
    efee:	00          	bgnd
    efef:	00          	bgnd
    eff0:	01          	nop
    eff1:	53          	comb
    eff2:	00          	bgnd
    eff3:	00          	bgnd
    eff4:	00          	bgnd
    eff5:	00          	bgnd
    eff6:	10          	sba
    eff7:	00          	bgnd
    eff8:	00          	bgnd
    eff9:	00          	bgnd
    effa:	00          	bgnd
    effb:	00          	bgnd
    effc:	40          	nega
    effd:	00          	bgnd
    effe:	2c 3d       	bge	0x0xf03d
    f000:	22 7f       	bhi	0x0xf081
    f002:	7f 00 00    	clr	0x0
    f005:	00          	bgnd
    f006:	01          	nop
    f007:	00          	bgnd
    f008:	00          	bgnd
    f009:	00          	bgnd
    f00a:	00          	bgnd
    f00b:	7f 00 00    	clr	0x0
    f00e:	3d          	mul
    f00f:	57          	asrb
    f010:	00          	bgnd
    f011:	40          	nega
    f012:	00          	bgnd
    f013:	2d 3a       	blt	0x0xf04f
    f015:	00          	bgnd
    f016:	00          	bgnd
    f017:	00          	bgnd
    f018:	5a          	decb
    f019:	40          	nega
    f01a:	00          	bgnd
    f01b:	00          	bgnd
    f01c:	02          	idiv
    f01d:	00          	bgnd
    f01e:	00          	bgnd
    f01f:	00          	bgnd
    f020:	00          	bgnd
    f021:	00          	bgnd
    f022:	00          	bgnd
    f023:	00          	bgnd
    f024:	07          	tpa
    f025:	00          	bgnd
    f026:	21 01       	brn	0x0xf029
    f028:	40          	nega
    f029:	40          	nega
    f02a:	28 4b       	bvc	0x0xf077
    f02c:	20 0a       	bra	0x0xf038
    f02e:	00          	bgnd
    f02f:	01          	nop
    f030:	02          	idiv
    f031:	00          	bgnd
    f032:	12 00 19 00 	brset	*0x0, #0x19, 0x0xf036
    f036:	10          	sba
    f037:	00          	bgnd
    f038:	00          	bgnd
    f039:	00          	bgnd
    f03a:	03          	fdiv
    f03b:	09          	dex
    f03c:	02          	idiv
    f03d:	00          	bgnd
    f03e:	11          	cba
	...
    f047:	00          	bgnd
    f048:	40          	nega
    f049:	40          	nega
    f04a:	00          	bgnd
    f04b:	00          	bgnd
    f04c:	02          	idiv
    f04d:	02          	idiv
    f04e:	02          	idiv
	...
    f063:	00          	bgnd
    f064:	07          	tpa
    f065:	07          	tpa
    f066:	07          	tpa
    f067:	07          	tpa
    f068:	07          	tpa
    f069:	07          	tpa
    f06a:	07          	tpa
    f06b:	07          	tpa
    f06c:	40          	nega
    f06d:	40          	nega
    f06e:	40          	nega
    f06f:	40          	nega
    f070:	40          	nega
    f071:	40          	nega
    f072:	40          	nega
    f073:	40          	nega
	...
    f07c:	49          	rola
    f07d:	4e          	.byte	0x4e
    f07e:	49          	rola
    f07f:	54          	lsrb
    f080:	49          	rola
    f081:	41          	.byte	0x41
    f082:	4c          	inca
    f083:	20 20       	bra	0x0xf0a5
    f085:	20 20       	bra	0x0xf0a7
    f087:	20 20       	bra	0x0xf0a9
    f089:	20 20       	bra	0x0xf0ab
    f08b:	20 01       	bra	0x0xf08e
    f08d:	00          	bgnd
    f08e:	3b          	rti
    f08f:	44          	lsra
    f090:	7f 00 01    	clr	0x1
    f093:	44          	lsra
    f094:	7f 00 01    	clr	0x1
    f097:	44          	lsra
    f098:	7f 00 01    	clr	0x1
    f09b:	44          	lsra
    f09c:	7f 00 01    	clr	0x1
    f09f:	44          	lsra
    f0a0:	7f 00 01    	clr	0x1
    f0a3:	44          	lsra
    f0a4:	7f 00 01    	clr	0x1
    f0a7:	44          	lsra
    f0a8:	7f 00 01    	clr	0x1
    f0ab:	44          	lsra
    f0ac:	7f 3c 00    	clr	0x3c00
    f0af:	00          	bgnd
    f0b0:	00          	bgnd
    f0b1:	00          	bgnd
    f0b2:	00          	bgnd
    f0b3:	00          	bgnd
    f0b4:	07          	tpa
    f0b5:	07          	tpa
    f0b6:	07          	tpa
    f0b7:	07          	tpa
    f0b8:	07          	tpa
    f0b9:	07          	tpa
    f0ba:	07          	tpa
    f0bb:	07          	tpa
    f0bc:	22 22       	bhi	0x0xf0e0
    f0be:	22 22       	bhi	0x0xf0e2
    f0c0:	5e          	.byte	0x5e
    f0c1:	5e          	.byte	0x5e
    f0c2:	5e          	.byte	0x5e
    f0c3:	5e          	.byte	0x5e
	...
    f0cc:	49          	rola
    f0cd:	4e          	.byte	0x4e
    f0ce:	49          	rola
    f0cf:	54          	lsrb
    f0d0:	49          	rola
    f0d1:	41          	.byte	0x41
    f0d2:	4c          	inca
    f0d3:	20 20       	bra	0x0xf0f5
    f0d5:	20 20       	bra	0x0xf0f7
    f0d7:	20 20       	bra	0x0xf0f9
    f0d9:	20 20       	bra	0x0xf0fb
    f0db:	20 00       	bra	0x0xf0dd
    f0dd:	02          	idiv
    f0de:	03          	fdiv
    f0df:	05          	asld
    f0e0:	0f          	sei
    f0e1:	0a          	clv
    f0e2:	1e 14 3c 28 	brset	0x14,x, #0x3c, 0x0xf10e
    f0e6:	78 50 f0    	asl	0x50f0
    f0e9:	ff ff ff    	stx	0xffff
    f0ec:	ff ff ff    	stx	0xffff
    f0ef:	ff ff ff    	stx	0xffff
    f0f2:	ff ff ff    	stx	0xffff
    f0f5:	ff ff ff    	stx	0xffff
    f0f8:	ff ff ff    	stx	0xffff
    f0fb:	ff ff ff    	stx	0xffff
    f0fe:	ff ff ff    	stx	0xffff
    f101:	ff ff ff    	stx	0xffff
    f104:	ff ff ff    	stx	0xffff
    f107:	ff ff ff    	stx	0xffff
    f10a:	ff ff ff    	stx	0xffff
    f10d:	ff ff ff    	stx	0xffff
    f110:	ff ff ff    	stx	0xffff
    f113:	ff ff ff    	stx	0xffff
    f116:	ff ff ff    	stx	0xffff
    f119:	ff ff ff    	stx	0xffff
    f11c:	ff ff ff    	stx	0xffff
    f11f:	ff ff ff    	stx	0xffff
    f122:	ff ff ff    	stx	0xffff
    f125:	ff ff ff    	stx	0xffff
    f128:	ff ff ff    	stx	0xffff
    f12b:	ff ff ff    	stx	0xffff
    f12e:	ff ff ff    	stx	0xffff
    f131:	ff ff ff    	stx	0xffff
    f134:	ff ff ff    	stx	0xffff
    f137:	ff ff ff    	stx	0xffff
    f13a:	ff ff ff    	stx	0xffff
    f13d:	ff ff ff    	stx	0xffff
    f140:	ff ff ff    	stx	0xffff
    f143:	ff ff ff    	stx	0xffff
    f146:	ff ff ff    	stx	0xffff
    f149:	ff ff ff    	stx	0xffff
    f14c:	ff ff ff    	stx	0xffff
    f14f:	ff ff ff    	stx	0xffff
    f152:	ff ff ff    	stx	0xffff
    f155:	ff ff ff    	stx	0xffff
    f158:	ff ff ff    	stx	0xffff
    f15b:	ff ff ff    	stx	0xffff
    f15e:	ff ff ff    	stx	0xffff
    f161:	ff ff ff    	stx	0xffff
    f164:	ff ff ff    	stx	0xffff
    f167:	ff ff ff    	stx	0xffff
    f16a:	ff ff ff    	stx	0xffff
    f16d:	ff ff ff    	stx	0xffff
    f170:	ff ff ff    	stx	0xffff
    f173:	ff ff ff    	stx	0xffff
    f176:	ff ff ff    	stx	0xffff
    f179:	ff ff ff    	stx	0xffff
    f17c:	ff ff ff    	stx	0xffff
    f17f:	ff ff ff    	stx	0xffff
    f182:	ff ff ff    	stx	0xffff
    f185:	ff ff ff    	stx	0xffff
    f188:	ff ff ff    	stx	0xffff
    f18b:	ff ff ff    	stx	0xffff
    f18e:	ff ff ff    	stx	0xffff
    f191:	ff ff ff    	stx	0xffff
    f194:	ff ff ff    	stx	0xffff
    f197:	ff ff ff    	stx	0xffff
    f19a:	ff ff ff    	stx	0xffff
    f19d:	ff ff ff    	stx	0xffff
    f1a0:	ff ff ff    	stx	0xffff
    f1a3:	ff ff ff    	stx	0xffff
    f1a6:	ff ff ff    	stx	0xffff
    f1a9:	ff ff ff    	stx	0xffff
    f1ac:	ff ff ff    	stx	0xffff
    f1af:	ff ff ff    	stx	0xffff
    f1b2:	ff ff ff    	stx	0xffff
    f1b5:	ff ff ff    	stx	0xffff
    f1b8:	ff ff ff    	stx	0xffff
    f1bb:	ff ff ff    	stx	0xffff
    f1be:	ff ff ff    	stx	0xffff
    f1c1:	ff ff ff    	stx	0xffff
    f1c4:	ff ff ff    	stx	0xffff
    f1c7:	ff ff ff    	stx	0xffff
    f1ca:	ff ff ff    	stx	0xffff
    f1cd:	ff ff ff    	stx	0xffff
    f1d0:	ff ff ff    	stx	0xffff
    f1d3:	ff ff ff    	stx	0xffff
    f1d6:	ff ff ff    	stx	0xffff
    f1d9:	ff ff ff    	stx	0xffff
    f1dc:	ff ff ff    	stx	0xffff
    f1df:	ff ff ff    	stx	0xffff
    f1e2:	ff ff ff    	stx	0xffff
    f1e5:	ff ff ff    	stx	0xffff
    f1e8:	ff ff ff    	stx	0xffff
    f1eb:	ff ff ff    	stx	0xffff
    f1ee:	ff ff ff    	stx	0xffff
    f1f1:	ff ff ff    	stx	0xffff
    f1f4:	ff ff ff    	stx	0xffff
    f1f7:	ff ff ff    	stx	0xffff
    f1fa:	ff ff ff    	stx	0xffff
    f1fd:	ff ff ff    	stx	0xffff
    f200:	ff ff ff    	stx	0xffff
    f203:	ff ff ff    	stx	0xffff
    f206:	ff ff ff    	stx	0xffff
    f209:	ff ff ff    	stx	0xffff
    f20c:	ff ff ff    	stx	0xffff
    f20f:	ff ff ff    	stx	0xffff
    f212:	ff ff ff    	stx	0xffff
    f215:	ff ff ff    	stx	0xffff
    f218:	ff ff ff    	stx	0xffff
    f21b:	ff ff ff    	stx	0xffff
    f21e:	ff ff ff    	stx	0xffff
    f221:	ff ff ff    	stx	0xffff
    f224:	ff ff ff    	stx	0xffff
    f227:	ff ff ff    	stx	0xffff
    f22a:	ff ff ff    	stx	0xffff
    f22d:	ff ff ff    	stx	0xffff
    f230:	ff ff ff    	stx	0xffff
    f233:	ff ff ff    	stx	0xffff
    f236:	ff ff ff    	stx	0xffff
    f239:	ff ff ff    	stx	0xffff
    f23c:	ff ff ff    	stx	0xffff
    f23f:	ff ff ff    	stx	0xffff
    f242:	ff ff ff    	stx	0xffff
    f245:	ff ff ff    	stx	0xffff
    f248:	ff ff ff    	stx	0xffff
    f24b:	ff ff ff    	stx	0xffff
    f24e:	ff ff ff    	stx	0xffff
    f251:	ff ff ff    	stx	0xffff
    f254:	ff ff ff    	stx	0xffff
    f257:	ff ff ff    	stx	0xffff
    f25a:	ff ff ff    	stx	0xffff
    f25d:	ff ff ff    	stx	0xffff
    f260:	ff ff ff    	stx	0xffff
    f263:	ff ff ff    	stx	0xffff
    f266:	ff ff ff    	stx	0xffff
    f269:	ff ff ff    	stx	0xffff
    f26c:	ff ff ff    	stx	0xffff
    f26f:	ff ff ff    	stx	0xffff
    f272:	ff ff ff    	stx	0xffff
    f275:	ff ff ff    	stx	0xffff
    f278:	ff ff ff    	stx	0xffff
    f27b:	ff ff ff    	stx	0xffff
    f27e:	ff ff ff    	stx	0xffff
    f281:	ff ff ff    	stx	0xffff
    f284:	ff ff ff    	stx	0xffff
    f287:	ff ff ff    	stx	0xffff
    f28a:	ff ff ff    	stx	0xffff
    f28d:	ff ff ff    	stx	0xffff
    f290:	ff ff ff    	stx	0xffff
    f293:	ff ff ff    	stx	0xffff
    f296:	ff ff ff    	stx	0xffff
    f299:	ff ff ff    	stx	0xffff
    f29c:	ff ff ff    	stx	0xffff
    f29f:	ff ff ff    	stx	0xffff
    f2a2:	ff ff ff    	stx	0xffff
    f2a5:	ff ff ff    	stx	0xffff
    f2a8:	ff ff ff    	stx	0xffff
    f2ab:	ff ff ff    	stx	0xffff
    f2ae:	ff ff ff    	stx	0xffff
    f2b1:	ff ff ff    	stx	0xffff
    f2b4:	ff ff ff    	stx	0xffff
    f2b7:	ff ff ff    	stx	0xffff
    f2ba:	ff ff ff    	stx	0xffff
    f2bd:	ff ff ff    	stx	0xffff
    f2c0:	ff ff ff    	stx	0xffff
    f2c3:	ff ff ff    	stx	0xffff
    f2c6:	ff ff ff    	stx	0xffff
    f2c9:	ff ff ff    	stx	0xffff
    f2cc:	ff ff ff    	stx	0xffff
    f2cf:	ff ff ff    	stx	0xffff
    f2d2:	ff ff ff    	stx	0xffff
    f2d5:	ff ff ff    	stx	0xffff
    f2d8:	ff ff ff    	stx	0xffff
    f2db:	ff ff ff    	stx	0xffff
    f2de:	ff ff ff    	stx	0xffff
    f2e1:	ff ff ff    	stx	0xffff
    f2e4:	ff ff ff    	stx	0xffff
    f2e7:	ff ff ff    	stx	0xffff
    f2ea:	ff ff ff    	stx	0xffff
    f2ed:	ff ff ff    	stx	0xffff
    f2f0:	ff ff ff    	stx	0xffff
    f2f3:	ff ff ff    	stx	0xffff
    f2f6:	ff ff ff    	stx	0xffff
    f2f9:	ff ff ff    	stx	0xffff
    f2fc:	ff ff ff    	stx	0xffff
    f2ff:	ff ff ff    	stx	0xffff
    f302:	ff ff ff    	stx	0xffff
    f305:	ff ff ff    	stx	0xffff
    f308:	ff ff ff    	stx	0xffff
    f30b:	ff ff ff    	stx	0xffff
    f30e:	ff ff ff    	stx	0xffff
    f311:	ff ff ff    	stx	0xffff
    f314:	ff ff ff    	stx	0xffff
    f317:	ff ff ff    	stx	0xffff
    f31a:	ff ff ff    	stx	0xffff
    f31d:	ff ff ff    	stx	0xffff
    f320:	ff ff ff    	stx	0xffff
    f323:	ff ff ff    	stx	0xffff
    f326:	ff ff ff    	stx	0xffff
    f329:	ff ff ff    	stx	0xffff
    f32c:	ff ff ff    	stx	0xffff
    f32f:	ff ff ff    	stx	0xffff
    f332:	ff ff ff    	stx	0xffff
    f335:	ff ff ff    	stx	0xffff
    f338:	ff ff ff    	stx	0xffff
    f33b:	ff ff ff    	stx	0xffff
    f33e:	ff ff ff    	stx	0xffff
    f341:	ff ff ff    	stx	0xffff
    f344:	ff ff ff    	stx	0xffff
    f347:	ff ff ff    	stx	0xffff
    f34a:	ff ff ff    	stx	0xffff
    f34d:	ff ff ff    	stx	0xffff
    f350:	ff ff ff    	stx	0xffff
    f353:	ff ff ff    	stx	0xffff
    f356:	ff ff ff    	stx	0xffff
    f359:	ff ff ff    	stx	0xffff
    f35c:	ff ff ff    	stx	0xffff
    f35f:	ff ff ff    	stx	0xffff
    f362:	ff ff ff    	stx	0xffff
    f365:	ff ff ff    	stx	0xffff
    f368:	ff ff ff    	stx	0xffff
    f36b:	ff ff ff    	stx	0xffff
    f36e:	ff ff ff    	stx	0xffff
    f371:	ff ff ff    	stx	0xffff
    f374:	ff ff ff    	stx	0xffff
    f377:	ff ff ff    	stx	0xffff
    f37a:	ff ff ff    	stx	0xffff
    f37d:	ff ff ff    	stx	0xffff
    f380:	ff ff ff    	stx	0xffff
    f383:	ff ff ff    	stx	0xffff
    f386:	ff ff ff    	stx	0xffff
    f389:	ff ff ff    	stx	0xffff
    f38c:	ff ff ff    	stx	0xffff
    f38f:	ff ff ff    	stx	0xffff
    f392:	ff ff ff    	stx	0xffff
    f395:	ff ff ff    	stx	0xffff
    f398:	ff ff ff    	stx	0xffff
    f39b:	ff ff ff    	stx	0xffff
    f39e:	ff ff ff    	stx	0xffff
    f3a1:	ff ff ff    	stx	0xffff
    f3a4:	ff ff ff    	stx	0xffff
    f3a7:	ff ff ff    	stx	0xffff
    f3aa:	ff ff ff    	stx	0xffff
    f3ad:	ff ff ff    	stx	0xffff
    f3b0:	ff ff ff    	stx	0xffff
    f3b3:	ff ff ff    	stx	0xffff
    f3b6:	ff ff ff    	stx	0xffff
    f3b9:	ff ff ff    	stx	0xffff
    f3bc:	ff ff ff    	stx	0xffff
    f3bf:	ff ff ff    	stx	0xffff
    f3c2:	ff ff ff    	stx	0xffff
    f3c5:	ff ff ff    	stx	0xffff
    f3c8:	ff ff ff    	stx	0xffff
    f3cb:	ff ff ff    	stx	0xffff
    f3ce:	ff ff ff    	stx	0xffff
    f3d1:	ff ff ff    	stx	0xffff
    f3d4:	ff ff ff    	stx	0xffff
    f3d7:	ff ff ff    	stx	0xffff
    f3da:	ff ff ff    	stx	0xffff
    f3dd:	ff ff ff    	stx	0xffff
    f3e0:	ff ff ff    	stx	0xffff
    f3e3:	ff ff ff    	stx	0xffff
    f3e6:	ff ff ff    	stx	0xffff
    f3e9:	ff ff ff    	stx	0xffff
    f3ec:	ff ff ff    	stx	0xffff
    f3ef:	ff ff ff    	stx	0xffff
    f3f2:	ff ff ff    	stx	0xffff
    f3f5:	ff ff ff    	stx	0xffff
    f3f8:	ff ff ff    	stx	0xffff
    f3fb:	ff ff ff    	stx	0xffff
    f3fe:	ff ff ff    	stx	0xffff
    f401:	ff ff ff    	stx	0xffff
    f404:	ff ff ff    	stx	0xffff
    f407:	ff ff ff    	stx	0xffff
    f40a:	ff ff ff    	stx	0xffff
    f40d:	ff ff ff    	stx	0xffff
    f410:	ff ff ff    	stx	0xffff
    f413:	ff ff ff    	stx	0xffff
    f416:	ff ff ff    	stx	0xffff
    f419:	ff ff ff    	stx	0xffff
    f41c:	ff ff ff    	stx	0xffff
    f41f:	ff ff ff    	stx	0xffff
    f422:	ff ff ff    	stx	0xffff
    f425:	ff ff ff    	stx	0xffff
    f428:	ff ff ff    	stx	0xffff
    f42b:	ff ff ff    	stx	0xffff
    f42e:	ff ff ff    	stx	0xffff
    f431:	ff ff ff    	stx	0xffff
    f434:	ff ff ff    	stx	0xffff
    f437:	ff ff ff    	stx	0xffff
    f43a:	ff ff ff    	stx	0xffff
    f43d:	ff ff ff    	stx	0xffff
    f440:	ff ff ff    	stx	0xffff
    f443:	ff ff ff    	stx	0xffff
    f446:	ff ff ff    	stx	0xffff
    f449:	ff ff ff    	stx	0xffff
    f44c:	ff ff ff    	stx	0xffff
    f44f:	ff ff ff    	stx	0xffff
    f452:	ff ff ff    	stx	0xffff
    f455:	ff ff ff    	stx	0xffff
    f458:	ff ff ff    	stx	0xffff
    f45b:	ff ff ff    	stx	0xffff
    f45e:	ff ff ff    	stx	0xffff
    f461:	ff ff ff    	stx	0xffff
    f464:	ff ff ff    	stx	0xffff
    f467:	ff ff ff    	stx	0xffff
    f46a:	ff ff ff    	stx	0xffff
    f46d:	ff ff ff    	stx	0xffff
    f470:	ff ff ff    	stx	0xffff
    f473:	ff ff ff    	stx	0xffff
    f476:	ff ff ff    	stx	0xffff
    f479:	ff ff ff    	stx	0xffff
    f47c:	ff ff ff    	stx	0xffff
    f47f:	ff ff ff    	stx	0xffff
    f482:	ff ff ff    	stx	0xffff
    f485:	ff ff ff    	stx	0xffff
    f488:	ff ff ff    	stx	0xffff
    f48b:	ff ff ff    	stx	0xffff
    f48e:	ff ff ff    	stx	0xffff
    f491:	ff ff ff    	stx	0xffff
    f494:	ff ff ff    	stx	0xffff
    f497:	ff ff ff    	stx	0xffff
    f49a:	ff ff ff    	stx	0xffff
    f49d:	ff ff ff    	stx	0xffff
    f4a0:	ff ff ff    	stx	0xffff
    f4a3:	ff ff ff    	stx	0xffff
    f4a6:	ff ff ff    	stx	0xffff
    f4a9:	ff ff ff    	stx	0xffff
    f4ac:	ff ff ff    	stx	0xffff
    f4af:	ff ff ff    	stx	0xffff
    f4b2:	ff ff ff    	stx	0xffff
    f4b5:	ff ff ff    	stx	0xffff
    f4b8:	ff ff ff    	stx	0xffff
    f4bb:	ff ff ff    	stx	0xffff
    f4be:	ff ff ff    	stx	0xffff
    f4c1:	ff ff ff    	stx	0xffff
    f4c4:	ff ff ff    	stx	0xffff
    f4c7:	ff ff ff    	stx	0xffff
    f4ca:	ff ff ff    	stx	0xffff
    f4cd:	ff ff ff    	stx	0xffff
    f4d0:	ff ff ff    	stx	0xffff
    f4d3:	ff ff ff    	stx	0xffff
    f4d6:	ff ff ff    	stx	0xffff
    f4d9:	ff ff ff    	stx	0xffff
    f4dc:	ff ff ff    	stx	0xffff
    f4df:	ff ff ff    	stx	0xffff
    f4e2:	ff ff ff    	stx	0xffff
    f4e5:	ff ff ff    	stx	0xffff
    f4e8:	ff ff ff    	stx	0xffff
    f4eb:	ff ff ff    	stx	0xffff
    f4ee:	ff ff ff    	stx	0xffff
    f4f1:	ff ff ff    	stx	0xffff
    f4f4:	ff ff ff    	stx	0xffff
    f4f7:	ff ff ff    	stx	0xffff
    f4fa:	ff ff ff    	stx	0xffff
    f4fd:	ff ff ff    	stx	0xffff
    f500:	ff ff ff    	stx	0xffff
    f503:	ff ff ff    	stx	0xffff
    f506:	ff ff ff    	stx	0xffff
    f509:	ff ff ff    	stx	0xffff
    f50c:	ff ff ff    	stx	0xffff
    f50f:	ff ff ff    	stx	0xffff
    f512:	ff ff ff    	stx	0xffff
    f515:	ff ff ff    	stx	0xffff
    f518:	ff ff ff    	stx	0xffff
    f51b:	ff ff ff    	stx	0xffff
    f51e:	ff ff ff    	stx	0xffff
    f521:	ff ff ff    	stx	0xffff
    f524:	ff ff ff    	stx	0xffff
    f527:	ff ff ff    	stx	0xffff
    f52a:	ff ff ff    	stx	0xffff
    f52d:	ff ff ff    	stx	0xffff
    f530:	ff ff ff    	stx	0xffff
    f533:	ff ff ff    	stx	0xffff
    f536:	ff ff ff    	stx	0xffff
    f539:	ff ff ff    	stx	0xffff
    f53c:	ff ff ff    	stx	0xffff
    f53f:	ff ff ff    	stx	0xffff
    f542:	ff ff ff    	stx	0xffff
    f545:	ff ff ff    	stx	0xffff
    f548:	ff ff ff    	stx	0xffff
    f54b:	ff ff ff    	stx	0xffff
    f54e:	ff ff ff    	stx	0xffff
    f551:	ff ff ff    	stx	0xffff
    f554:	ff ff ff    	stx	0xffff
    f557:	ff ff ff    	stx	0xffff
    f55a:	ff ff ff    	stx	0xffff
    f55d:	ff ff ff    	stx	0xffff
    f560:	ff ff ff    	stx	0xffff
    f563:	ff ff ff    	stx	0xffff
    f566:	ff ff ff    	stx	0xffff
    f569:	ff ff ff    	stx	0xffff
    f56c:	ff ff ff    	stx	0xffff
    f56f:	ff ff ff    	stx	0xffff
    f572:	ff ff ff    	stx	0xffff
    f575:	ff ff ff    	stx	0xffff
    f578:	ff ff ff    	stx	0xffff
    f57b:	ff ff ff    	stx	0xffff
    f57e:	ff ff ff    	stx	0xffff
    f581:	ff ff ff    	stx	0xffff
    f584:	ff ff ff    	stx	0xffff
    f587:	ff ff ff    	stx	0xffff
    f58a:	ff ff ff    	stx	0xffff
    f58d:	ff ff ff    	stx	0xffff
    f590:	ff ff ff    	stx	0xffff
    f593:	ff ff ff    	stx	0xffff
    f596:	ff ff ff    	stx	0xffff
    f599:	ff ff ff    	stx	0xffff
    f59c:	ff ff ff    	stx	0xffff
    f59f:	ff ff ff    	stx	0xffff
    f5a2:	ff ff ff    	stx	0xffff
    f5a5:	ff ff ff    	stx	0xffff
    f5a8:	ff ff ff    	stx	0xffff
    f5ab:	ff ff ff    	stx	0xffff
    f5ae:	ff ff ff    	stx	0xffff
    f5b1:	ff ff ff    	stx	0xffff
    f5b4:	ff ff ff    	stx	0xffff
    f5b7:	ff ff ff    	stx	0xffff
    f5ba:	ff ff ff    	stx	0xffff
    f5bd:	ff ff ff    	stx	0xffff
    f5c0:	ff ff ff    	stx	0xffff
    f5c3:	ff ff ff    	stx	0xffff
    f5c6:	ff ff ff    	stx	0xffff
    f5c9:	ff ff ff    	stx	0xffff
    f5cc:	ff ff ff    	stx	0xffff
    f5cf:	ff ff ff    	stx	0xffff
    f5d2:	ff ff ff    	stx	0xffff
    f5d5:	ff ff ff    	stx	0xffff
    f5d8:	ff ff ff    	stx	0xffff
    f5db:	ff ff ff    	stx	0xffff
    f5de:	ff ff ff    	stx	0xffff
    f5e1:	ff ff ff    	stx	0xffff
    f5e4:	ff ff ff    	stx	0xffff
    f5e7:	ff ff ff    	stx	0xffff
    f5ea:	ff ff ff    	stx	0xffff
    f5ed:	ff ff ff    	stx	0xffff
    f5f0:	ff ff ff    	stx	0xffff
    f5f3:	ff ff ff    	stx	0xffff
    f5f6:	ff ff ff    	stx	0xffff
    f5f9:	ff ff ff    	stx	0xffff
    f5fc:	ff ff ff    	stx	0xffff
    f5ff:	ff ff ff    	stx	0xffff
    f602:	ff ff ff    	stx	0xffff
    f605:	ff ff ff    	stx	0xffff
    f608:	ff ff ff    	stx	0xffff
    f60b:	ff ff ff    	stx	0xffff
    f60e:	ff ff ff    	stx	0xffff
    f611:	ff ff ff    	stx	0xffff
    f614:	ff ff ff    	stx	0xffff
    f617:	ff ff ff    	stx	0xffff
    f61a:	ff ff ff    	stx	0xffff
    f61d:	ff ff ff    	stx	0xffff
    f620:	ff ff ff    	stx	0xffff
    f623:	ff ff ff    	stx	0xffff
    f626:	ff ff ff    	stx	0xffff
    f629:	ff ff ff    	stx	0xffff
    f62c:	ff ff ff    	stx	0xffff
    f62f:	ff ff ff    	stx	0xffff
    f632:	ff ff ff    	stx	0xffff
    f635:	ff ff ff    	stx	0xffff
    f638:	ff ff ff    	stx	0xffff
    f63b:	ff ff ff    	stx	0xffff
    f63e:	ff ff ff    	stx	0xffff
    f641:	ff ff ff    	stx	0xffff
    f644:	ff ff ff    	stx	0xffff
    f647:	ff ff ff    	stx	0xffff
    f64a:	ff ff ff    	stx	0xffff
    f64d:	ff ff ff    	stx	0xffff
    f650:	ff ff ff    	stx	0xffff
    f653:	ff ff ff    	stx	0xffff
    f656:	ff ff ff    	stx	0xffff
    f659:	ff ff ff    	stx	0xffff
    f65c:	ff ff ff    	stx	0xffff
    f65f:	ff ff ff    	stx	0xffff
    f662:	ff ff ff    	stx	0xffff
    f665:	ff ff ff    	stx	0xffff
    f668:	ff ff ff    	stx	0xffff
    f66b:	ff ff ff    	stx	0xffff
    f66e:	ff ff ff    	stx	0xffff
    f671:	ff ff ff    	stx	0xffff
    f674:	ff ff ff    	stx	0xffff
    f677:	ff ff ff    	stx	0xffff
    f67a:	ff ff ff    	stx	0xffff
    f67d:	ff ff ff    	stx	0xffff
    f680:	ff ff ff    	stx	0xffff
    f683:	ff ff ff    	stx	0xffff
    f686:	ff ff ff    	stx	0xffff
    f689:	ff ff ff    	stx	0xffff
    f68c:	ff ff ff    	stx	0xffff
    f68f:	ff ff ff    	stx	0xffff
    f692:	ff ff ff    	stx	0xffff
    f695:	ff ff ff    	stx	0xffff
    f698:	ff ff ff    	stx	0xffff
    f69b:	ff ff ff    	stx	0xffff
    f69e:	ff ff ff    	stx	0xffff
    f6a1:	ff ff ff    	stx	0xffff
    f6a4:	ff ff ff    	stx	0xffff
    f6a7:	ff ff ff    	stx	0xffff
    f6aa:	ff ff ff    	stx	0xffff
    f6ad:	ff ff ff    	stx	0xffff
    f6b0:	ff ff ff    	stx	0xffff
    f6b3:	ff ff ff    	stx	0xffff
    f6b6:	ff ff ff    	stx	0xffff
    f6b9:	ff ff ff    	stx	0xffff
    f6bc:	ff ff ff    	stx	0xffff
    f6bf:	ff ff ff    	stx	0xffff
    f6c2:	ff ff ff    	stx	0xffff
    f6c5:	ff ff ff    	stx	0xffff
    f6c8:	ff ff ff    	stx	0xffff
    f6cb:	ff ff ff    	stx	0xffff
    f6ce:	ff ff ff    	stx	0xffff
    f6d1:	ff ff ff    	stx	0xffff
    f6d4:	ff ff ff    	stx	0xffff
    f6d7:	ff ff ff    	stx	0xffff
    f6da:	ff ff ff    	stx	0xffff
    f6dd:	ff ff ff    	stx	0xffff
    f6e0:	ff ff ff    	stx	0xffff
    f6e3:	ff ff ff    	stx	0xffff
    f6e6:	ff ff ff    	stx	0xffff
    f6e9:	ff ff ff    	stx	0xffff
    f6ec:	ff ff ff    	stx	0xffff
    f6ef:	ff ff ff    	stx	0xffff
    f6f2:	ff ff ff    	stx	0xffff
    f6f5:	ff ff ff    	stx	0xffff
    f6f8:	ff ff ff    	stx	0xffff
    f6fb:	ff ff ff    	stx	0xffff
    f6fe:	ff ff ff    	stx	0xffff
    f701:	ff ff ff    	stx	0xffff
    f704:	ff ff ff    	stx	0xffff
    f707:	ff ff ff    	stx	0xffff
    f70a:	ff ff ff    	stx	0xffff
    f70d:	ff ff ff    	stx	0xffff
    f710:	ff ff ff    	stx	0xffff
    f713:	ff ff ff    	stx	0xffff
    f716:	ff ff ff    	stx	0xffff
    f719:	ff ff ff    	stx	0xffff
    f71c:	ff ff ff    	stx	0xffff
    f71f:	ff ff ff    	stx	0xffff
    f722:	ff ff ff    	stx	0xffff
    f725:	ff ff ff    	stx	0xffff
    f728:	ff ff ff    	stx	0xffff
    f72b:	ff ff ff    	stx	0xffff
    f72e:	ff ff ff    	stx	0xffff
    f731:	ff ff ff    	stx	0xffff
    f734:	ff ff ff    	stx	0xffff
    f737:	ff ff ff    	stx	0xffff
    f73a:	ff ff ff    	stx	0xffff
    f73d:	ff ff ff    	stx	0xffff
    f740:	ff ff ff    	stx	0xffff
    f743:	ff ff ff    	stx	0xffff
    f746:	ff ff ff    	stx	0xffff
    f749:	ff ff ff    	stx	0xffff
    f74c:	ff ff ff    	stx	0xffff
    f74f:	ff ff ff    	stx	0xffff
    f752:	ff ff ff    	stx	0xffff
    f755:	ff ff ff    	stx	0xffff
    f758:	ff ff ff    	stx	0xffff
    f75b:	ff ff ff    	stx	0xffff
    f75e:	ff ff ff    	stx	0xffff
    f761:	ff ff ff    	stx	0xffff
    f764:	ff ff ff    	stx	0xffff
    f767:	ff ff ff    	stx	0xffff
    f76a:	ff ff ff    	stx	0xffff
    f76d:	ff ff ff    	stx	0xffff
    f770:	ff ff ff    	stx	0xffff
    f773:	ff ff ff    	stx	0xffff
    f776:	ff ff ff    	stx	0xffff
    f779:	ff ff ff    	stx	0xffff
    f77c:	ff ff ff    	stx	0xffff
    f77f:	ff ff ff    	stx	0xffff
    f782:	ff ff ff    	stx	0xffff
    f785:	ff ff ff    	stx	0xffff
    f788:	ff ff ff    	stx	0xffff
    f78b:	ff ff ff    	stx	0xffff
    f78e:	ff ff ff    	stx	0xffff
    f791:	ff ff ff    	stx	0xffff
    f794:	ff ff ff    	stx	0xffff
    f797:	ff ff ff    	stx	0xffff
    f79a:	ff ff ff    	stx	0xffff
    f79d:	ff ff ff    	stx	0xffff
    f7a0:	ff ff ff    	stx	0xffff
    f7a3:	ff ff ff    	stx	0xffff
    f7a6:	ff ff ff    	stx	0xffff
    f7a9:	ff ff ff    	stx	0xffff
    f7ac:	ff ff ff    	stx	0xffff
    f7af:	ff ff ff    	stx	0xffff
    f7b2:	ff ff ff    	stx	0xffff
    f7b5:	ff ff ff    	stx	0xffff
    f7b8:	ff ff ff    	stx	0xffff
    f7bb:	ff ff ff    	stx	0xffff
    f7be:	ff ff ff    	stx	0xffff
    f7c1:	ff ff ff    	stx	0xffff
    f7c4:	ff ff ff    	stx	0xffff
    f7c7:	ff ff ff    	stx	0xffff
    f7ca:	ff ff ff    	stx	0xffff
    f7cd:	ff ff ff    	stx	0xffff
    f7d0:	ff ff ff    	stx	0xffff
    f7d3:	ff ff ff    	stx	0xffff
    f7d6:	ff ff ff    	stx	0xffff
    f7d9:	ff ff ff    	stx	0xffff
    f7dc:	ff ff ff    	stx	0xffff
    f7df:	ff ff ff    	stx	0xffff
    f7e2:	ff ff ff    	stx	0xffff
    f7e5:	ff ff ff    	stx	0xffff
    f7e8:	ff ff ff    	stx	0xffff
    f7eb:	ff ff ff    	stx	0xffff
    f7ee:	ff ff ff    	stx	0xffff
    f7f1:	ff ff ff    	stx	0xffff
    f7f4:	ff ff ff    	stx	0xffff
    f7f7:	ff ff ff    	stx	0xffff
    f7fa:	ff ff ff    	stx	0xffff
    f7fd:	ff ff ff    	stx	0xffff
    f800:	ff ff ff    	stx	0xffff
    f803:	ff ff ff    	stx	0xffff
    f806:	ff ff ff    	stx	0xffff
    f809:	ff ff ff    	stx	0xffff
    f80c:	ff ff ff    	stx	0xffff
    f80f:	ff ff ff    	stx	0xffff
    f812:	ff ff ff    	stx	0xffff
    f815:	ff ff ff    	stx	0xffff
    f818:	ff ff ff    	stx	0xffff
    f81b:	ff ff ff    	stx	0xffff
    f81e:	ff ff ff    	stx	0xffff
    f821:	ff ff ff    	stx	0xffff
    f824:	ff ff ff    	stx	0xffff
    f827:	ff ff ff    	stx	0xffff
    f82a:	ff ff ff    	stx	0xffff
    f82d:	ff ff ff    	stx	0xffff
    f830:	ff ff ff    	stx	0xffff
    f833:	ff ff ff    	stx	0xffff
    f836:	ff ff ff    	stx	0xffff
    f839:	ff ff ff    	stx	0xffff
    f83c:	ff ff ff    	stx	0xffff
    f83f:	ff ff ff    	stx	0xffff
    f842:	ff ff ff    	stx	0xffff
    f845:	ff ff ff    	stx	0xffff
    f848:	ff ff ff    	stx	0xffff
    f84b:	ff ff ff    	stx	0xffff
    f84e:	ff ff ff    	stx	0xffff
    f851:	ff ff ff    	stx	0xffff
    f854:	ff ff ff    	stx	0xffff
    f857:	ff ff ff    	stx	0xffff
    f85a:	ff ff ff    	stx	0xffff
    f85d:	ff ff ff    	stx	0xffff
    f860:	ff ff ff    	stx	0xffff
    f863:	ff ff ff    	stx	0xffff
    f866:	ff ff ff    	stx	0xffff
    f869:	ff ff ff    	stx	0xffff
    f86c:	ff ff ff    	stx	0xffff
    f86f:	ff ff ff    	stx	0xffff
    f872:	ff ff ff    	stx	0xffff
    f875:	ff ff ff    	stx	0xffff
    f878:	ff ff ff    	stx	0xffff
    f87b:	ff ff ff    	stx	0xffff
    f87e:	ff ff ff    	stx	0xffff
    f881:	ff ff ff    	stx	0xffff
    f884:	ff ff ff    	stx	0xffff
    f887:	ff ff ff    	stx	0xffff
    f88a:	ff ff ff    	stx	0xffff
    f88d:	ff ff ff    	stx	0xffff
    f890:	ff ff ff    	stx	0xffff
    f893:	ff ff ff    	stx	0xffff
    f896:	ff ff ff    	stx	0xffff
    f899:	ff ff ff    	stx	0xffff
    f89c:	ff ff ff    	stx	0xffff
    f89f:	ff ff ff    	stx	0xffff
    f8a2:	ff ff ff    	stx	0xffff
    f8a5:	ff ff ff    	stx	0xffff
    f8a8:	ff ff ff    	stx	0xffff
    f8ab:	ff ff ff    	stx	0xffff
    f8ae:	ff ff ff    	stx	0xffff
    f8b1:	ff ff ff    	stx	0xffff
    f8b4:	ff ff ff    	stx	0xffff
    f8b7:	ff ff ff    	stx	0xffff
    f8ba:	ff ff ff    	stx	0xffff
    f8bd:	ff ff ff    	stx	0xffff
    f8c0:	ff ff ff    	stx	0xffff
    f8c3:	ff ff ff    	stx	0xffff
    f8c6:	ff ff ff    	stx	0xffff
    f8c9:	ff ff ff    	stx	0xffff
    f8cc:	ff ff ff    	stx	0xffff
    f8cf:	ff ff ff    	stx	0xffff
    f8d2:	ff ff ff    	stx	0xffff
    f8d5:	ff ff ff    	stx	0xffff
    f8d8:	ff ff ff    	stx	0xffff
    f8db:	ff ff ff    	stx	0xffff
    f8de:	ff ff ff    	stx	0xffff
    f8e1:	ff ff ff    	stx	0xffff
    f8e4:	ff ff ff    	stx	0xffff
    f8e7:	ff ff ff    	stx	0xffff
    f8ea:	ff ff ff    	stx	0xffff
    f8ed:	ff ff ff    	stx	0xffff
    f8f0:	ff ff ff    	stx	0xffff
    f8f3:	ff ff ff    	stx	0xffff
    f8f6:	ff ff ff    	stx	0xffff
    f8f9:	ff ff ff    	stx	0xffff
    f8fc:	ff ff ff    	stx	0xffff
    f8ff:	ff ff ff    	stx	0xffff
    f902:	ff ff ff    	stx	0xffff
    f905:	ff ff ff    	stx	0xffff
    f908:	ff ff ff    	stx	0xffff
    f90b:	ff ff ff    	stx	0xffff
    f90e:	ff ff ff    	stx	0xffff
    f911:	ff ff ff    	stx	0xffff
    f914:	ff ff ff    	stx	0xffff
    f917:	ff ff ff    	stx	0xffff
    f91a:	ff ff ff    	stx	0xffff
    f91d:	ff ff ff    	stx	0xffff
    f920:	ff ff ff    	stx	0xffff
    f923:	ff ff ff    	stx	0xffff
    f926:	ff ff ff    	stx	0xffff
    f929:	ff ff ff    	stx	0xffff
    f92c:	ff ff ff    	stx	0xffff
    f92f:	ff ff ff    	stx	0xffff
    f932:	ff ff ff    	stx	0xffff
    f935:	ff ff ff    	stx	0xffff
    f938:	ff ff ff    	stx	0xffff
    f93b:	ff ff ff    	stx	0xffff
    f93e:	ff ff ff    	stx	0xffff
    f941:	ff ff ff    	stx	0xffff
    f944:	ff ff ff    	stx	0xffff
    f947:	ff ff ff    	stx	0xffff
    f94a:	ff ff ff    	stx	0xffff
    f94d:	ff ff ff    	stx	0xffff
    f950:	ff ff ff    	stx	0xffff
    f953:	ff ff ff    	stx	0xffff
    f956:	ff ff ff    	stx	0xffff
    f959:	ff ff ff    	stx	0xffff
    f95c:	ff ff ff    	stx	0xffff
    f95f:	ff ff ff    	stx	0xffff
    f962:	ff ff ff    	stx	0xffff
    f965:	ff ff ff    	stx	0xffff
    f968:	ff ff ff    	stx	0xffff
    f96b:	ff ff ff    	stx	0xffff
    f96e:	ff ff ff    	stx	0xffff
    f971:	ff ff ff    	stx	0xffff
    f974:	ff ff ff    	stx	0xffff
    f977:	ff ff ff    	stx	0xffff
    f97a:	ff ff ff    	stx	0xffff
    f97d:	ff ff ff    	stx	0xffff
    f980:	ff ff ff    	stx	0xffff
    f983:	ff ff ff    	stx	0xffff
    f986:	ff ff ff    	stx	0xffff
    f989:	ff ff ff    	stx	0xffff
    f98c:	ff ff ff    	stx	0xffff
    f98f:	ff ff ff    	stx	0xffff
    f992:	ff ff ff    	stx	0xffff
    f995:	ff ff ff    	stx	0xffff
    f998:	ff ff ff    	stx	0xffff
    f99b:	ff ff ff    	stx	0xffff
    f99e:	ff ff ff    	stx	0xffff
    f9a1:	ff ff ff    	stx	0xffff
    f9a4:	ff ff ff    	stx	0xffff
    f9a7:	ff ff ff    	stx	0xffff
    f9aa:	ff ff ff    	stx	0xffff
    f9ad:	ff ff ff    	stx	0xffff
    f9b0:	ff ff ff    	stx	0xffff
    f9b3:	ff ff ff    	stx	0xffff
    f9b6:	ff ff ff    	stx	0xffff
    f9b9:	ff ff ff    	stx	0xffff
    f9bc:	ff ff ff    	stx	0xffff
    f9bf:	ff ff ff    	stx	0xffff
    f9c2:	ff ff ff    	stx	0xffff
    f9c5:	ff ff ff    	stx	0xffff
    f9c8:	ff ff ff    	stx	0xffff
    f9cb:	ff ff ff    	stx	0xffff
    f9ce:	ff ff ff    	stx	0xffff
    f9d1:	ff ff ff    	stx	0xffff
    f9d4:	ff ff ff    	stx	0xffff
    f9d7:	ff ff ff    	stx	0xffff
    f9da:	ff ff ff    	stx	0xffff
    f9dd:	ff ff ff    	stx	0xffff
    f9e0:	ff f6 10    	stx	0xf610
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
    fe9d:	ce f0 dc    	ldx	#0xf0dc
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
    ffb9:	20 28       	bra	0x0xffe3
    ffbb:	43          	coma
    ffbc:	29 32       	bvs	0x0xfff0
    ffbe:	30          	tsx
    ffbf:	30          	tsx
    ffc0:	38          	pulx
    ffc1:	20 53       	bra	0x0x10016
    ffc3:	54          	lsrb
    ffc4:	55          	.byte	0x55
    ffc5:	44          	lsra
    ffc6:	49          	rola
    ffc7:	4f          	clra
    ffc8:	20 45       	bra	0x0x1000f
    ffca:	4c          	inca
    ffcb:	45          	.byte	0x45
    ffcc:	43          	coma
    ffcd:	54          	lsrb
    ffce:	52          	.byte	0x52
    ffcf:	4f          	clra
    ffd0:	4e          	.byte	0x4e
    ffd1:	49          	rola
    ffd2:	43          	coma
    ffd3:	53          	comb
    ffd4:	ff ff fe    	stx	0xfffe
    ffd7:	20 ff       	bra	0x0xffd8
    ffd9:	ff ff ff    	stx	0xffff
    ffdc:	ff ff ff    	stx	0xffff
    ffdf:	ff fd f0    	stx	0xfdf0
    ffe2:	ff ff ff    	stx	0xffff
    ffe5:	ff ff ff    	stx	0xffff
    ffe8:	fe 00 ff    	ldx	0xff
    ffeb:	ff ff ff    	stx	0xffff
    ffee:	ff ff ff    	stx	0xffff
    fff1:	ff ff ff    	stx	0xffff
    fff4:	ff ff fe    	stx	0xfffe
    fff7:	00          	bgnd
    fff8:	80 00       	suba	#0x0
    fffa:	80 00       	suba	#0x0
    fffc:	80 00       	suba	#0x0
    fffe:	80 00       	suba	#0x0
