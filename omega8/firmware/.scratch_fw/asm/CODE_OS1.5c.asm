
/home/user/.scratch_fw/images/CODE_OS1.5c.decoded.bin:     file format binary


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
    8076:	86 2d       	ldaa	#0x2d
    8078:	b7 10 46    	staa	0x1046
    807b:	bd ec 32    	jsr	0xec32
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
    8137:	86 28       	ldaa	#0x28
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
    8180:	bd ab 71    	jsr	0xab71
    8183:	18 ce 79 00 	ldy	#0x7900
    8187:	ce 81 a5    	ldx	#0x81a5
    818a:	20 db       	bra	0x0x8167
    818c:	86 03       	ldaa	#0x3
    818e:	97 f9       	staa	*0xf9
    8190:	bd ab 71    	jsr	0xab71
    8193:	18 ce 79 00 	ldy	#0x7900
    8197:	ce 81 a5    	ldx	#0x81a5
    819a:	20 cb       	bra	0x0x8167
    819c:	7f 00 f9    	clr	0xf9
    819f:	bd ab 4b    	jsr	0xab4b
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
    81ec:	b7 7f fa    	staa	0x7ffa
    81ef:	b6 7f f9    	ldaa	0x7ff9
    81f2:	81 02       	cmpa	#0x2
    81f4:	23 01       	bls	0x0x81f7
    81f6:	4f          	clra
    81f7:	97 de       	staa	*0xde
    81f9:	b7 7f f9    	staa	0x7ff9
    81fc:	b6 1f 07    	ldaa	0x1f07
    81ff:	97 d5       	staa	*0xd5
    8201:	97 d6       	staa	*0xd6
    8203:	c6 fc       	ldab	#0xfc
    8205:	bd af d2    	jsr	0xafd2
    8208:	bd ab 5e    	jsr	0xab5e
    820b:	ce 51 00    	ldx	#0x5100
    820e:	c6 80       	ldab	#0x80
    8210:	4f          	clra
    8211:	a7 00       	staa	0x0,x
    8213:	08          	inx
    8214:	5a          	decb
    8215:	26 fa       	bne	0x0x8211
    8217:	cc 51 00    	ldd	#0x5100
    821a:	fd 51 80    	std	0x5180
    821d:	cc 51 10    	ldd	#0x5110
    8220:	fd 51 82    	std	0x5182
    8223:	cc 51 20    	ldd	#0x5120
    8226:	fd 51 84    	std	0x5184
    8229:	cc 51 30    	ldd	#0x5130
    822c:	fd 51 86    	std	0x5186
    822f:	cc 51 40    	ldd	#0x5140
    8232:	fd 51 88    	std	0x5188
    8235:	cc 51 50    	ldd	#0x5150
    8238:	fd 51 8a    	std	0x518a
    823b:	cc 51 60    	ldd	#0x5160
    823e:	fd 51 8c    	std	0x518c
    8241:	cc 51 70    	ldd	#0x5170
    8244:	fd 51 8e    	std	0x518e
    8247:	bd ab 4b    	jsr	0xab4b
    824a:	b6 7f f6    	ldaa	0x7ff6
    824d:	81 04       	cmpa	#0x4
    824f:	23 01       	bls	0x0x8252
    8251:	4f          	clra
    8252:	97 f9       	staa	*0xf9
    8254:	81 04       	cmpa	#0x4
    8256:	27 12       	beq	0x0x826a
    8258:	14 d1 80    	bset	*0xd1, #0x80
    825b:	b6 7f f7    	ldaa	0x7ff7
    825e:	84 7f       	anda	#0x7f
    8260:	97 fa       	staa	*0xfa
    8262:	bd ab 71    	jsr	0xab71
    8265:	bd a1 16    	jsr	0xa116
    8268:	20 11       	bra	0x0x827b
    826a:	b6 7f f7    	ldaa	0x7ff7
    826d:	b7 01 6a    	staa	0x16a
    8270:	bd ab 71    	jsr	0xab71
    8273:	bd a3 15    	jsr	0xa315
    8276:	b6 50 00    	ldaa	0x5000
    8279:	97 d1       	staa	*0xd1
    827b:	3f          	swi
    827c:	86 80       	ldaa	#0x80
    827e:	b7 10 23    	staa	0x1023
    8281:	b7 01 1c    	staa	0x11c
    8284:	fc 10 0e    	ldd	0x100e
    8287:	c3 4e 20    	addd	#0x4e20
    828a:	fd 10 16    	std	0x1016
    828d:	86 80       	ldaa	#0x80
    828f:	b7 10 22    	staa	0x1022
    8292:	0e          	cli
    8293:	bd 91 bc    	jsr	0x91bc
    8296:	4d          	tsta
    8297:	2b 02       	bmi	0x0x829b
    8299:	20 f8       	bra	0x0x8293
    829b:	16          	tab
    829c:	c4 f0       	andb	#0xf0
    829e:	c1 90       	cmpb	#0x90
    82a0:	27 2a       	beq	0x0x82cc
    82a2:	c1 80       	cmpb	#0x80
    82a4:	26 03       	bne	0x0x82a9
    82a6:	7e 8a 99    	jmp	0x8a99
    82a9:	c1 b0       	cmpb	#0xb0
    82ab:	26 03       	bne	0x0x82b0
    82ad:	7e 8d 54    	jmp	0x8d54
    82b0:	c1 e0       	cmpb	#0xe0
    82b2:	26 03       	bne	0x0x82b7
    82b4:	7e 8e 18    	jmp	0x8e18
    82b7:	c1 d0       	cmpb	#0xd0
    82b9:	26 03       	bne	0x0x82be
    82bb:	7e 8e 3d    	jmp	0x8e3d
    82be:	c1 c0       	cmpb	#0xc0
    82c0:	26 03       	bne	0x0x82c5
    82c2:	7e 8e 6c    	jmp	0x8e6c
    82c5:	c1 f0       	cmpb	#0xf0
    82c7:	26 ca       	bne	0x0x8293
    82c9:	7e 8e cf    	jmp	0x8ecf
    82cc:	97 df       	staa	*0xdf
    82ce:	7c 00 fc    	inc	0xfc
    82d1:	12 d1 80 0d 	brset	*0xd1, #0x80, 0x0x82e2
    82d5:	d6 d1       	ldab	*0xd1
    82d7:	27 09       	beq	0x0x82e2
    82d9:	ce 84 49    	ldx	#0x8449
    82dc:	58          	aslb
    82dd:	3a          	abx
    82de:	ee 00       	ldx	0x0,x
    82e0:	6e 00       	jmp	0x0,x
    82e2:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x82e9
    82e6:	7e 8b 97    	jmp	0x8b97
    82e9:	bd 91 bc    	jsr	0x91bc
    82ec:	13 f8 01 03 	brclr	*0xf8, #0x01, 0x0x82f3
    82f0:	7e 83 72    	jmp	0x8372
    82f3:	36          	psha
    82f4:	bd 91 bc    	jsr	0x91bc
    82f7:	4d          	tsta
    82f8:	26 04       	bne	0x0x82fe
    82fa:	32          	pula
    82fb:	7e 83 e8    	jmp	0x83e8
    82fe:	33          	pulb
    82ff:	f7 01 00    	stab	0x100
    8302:	18 8f       	xgdy
    8304:	bd 83 0d    	jsr	0x830d
    8307:	bd 83 2e    	jsr	0x832e
    830a:	7e 83 e8    	jmp	0x83e8
    830d:	d6 6a       	ldab	*0x6a
    830f:	c4 07       	andb	#0x7
    8311:	26 03       	bne	0x0x8316
    8313:	c6 01       	ldab	#0x1
    8315:	39          	rts
    8316:	c1 01       	cmpb	#0x1
    8318:	26 03       	bne	0x0x831d
    831a:	c6 03       	ldab	#0x3
    831c:	39          	rts
    831d:	c1 02       	cmpb	#0x2
    831f:	26 03       	bne	0x0x8324
    8321:	c6 0f       	ldab	#0xf
    8323:	39          	rts
    8324:	c1 03       	cmpb	#0x3
    8326:	26 03       	bne	0x0x832b
    8328:	c6 3f       	ldab	#0x3f
    832a:	39          	rts
    832b:	c6 ff       	ldab	#0xff
    832d:	39          	rts
    832e:	17          	tba
    832f:	da f8       	orab	*0xf8
    8331:	d7 f8       	stab	*0xf8
    8333:	43          	coma
    8334:	7d 00 d0    	tst	0xd0
    8337:	27 05       	beq	0x0x833e
    8339:	7f 00 d0    	clr	0xd0
    833c:	20 08       	bra	0x0x8346
    833e:	7d 10 29    	tst	0x1029
    8341:	2a fb       	bpl	0x0x833e
    8343:	f6 10 2a    	ldab	0x102a
    8346:	01          	nop
    8347:	01          	nop
    8348:	01          	nop
    8349:	01          	nop
    834a:	b7 10 42    	staa	0x1042
    834d:	01          	nop
    834e:	01          	nop
    834f:	01          	nop
    8350:	01          	nop
    8351:	01          	nop
    8352:	01          	nop
    8353:	01          	nop
    8354:	c6 81       	ldab	#0x81
    8356:	f7 10 2a    	stab	0x102a
    8359:	7d 10 29    	tst	0x1029
    835c:	2a fb       	bpl	0x0x8359
    835e:	f6 10 2a    	ldab	0x102a
    8361:	18 8f       	xgdy
    8363:	f7 10 2a    	stab	0x102a
    8366:	7d 10 29    	tst	0x1029
    8369:	2a fb       	bpl	0x0x8366
    836b:	f6 10 2a    	ldab	0x102a
    836e:	b7 10 2a    	staa	0x102a
    8371:	39          	rts
    8372:	7d 00 69    	tst	0x69
    8375:	26 27       	bne	0x0x839e
    8377:	36          	psha
    8378:	bd 91 bc    	jsr	0x91bc
    837b:	4d          	tsta
    837c:	26 04       	bne	0x0x8382
    837e:	32          	pula
    837f:	7e 8a d0    	jmp	0x8ad0
    8382:	7d 00 fd    	tst	0xfd
    8385:	27 06       	beq	0x0x838d
    8387:	7f 00 fd    	clr	0xfd
    838a:	7e 82 fe    	jmp	0x82fe
    838d:	fe 01 08    	ldx	0x108
    8390:	f6 01 00    	ldab	0x100
    8393:	e7 00       	stab	0x0,x
    8395:	bd 84 33    	jsr	0x8433
    8398:	ff 01 08    	stx	0x108
    839b:	7e 82 fe    	jmp	0x82fe
    839e:	36          	psha
    839f:	bd 91 bc    	jsr	0x91bc
    83a2:	4d          	tsta
    83a3:	26 04       	bne	0x0x83a9
    83a5:	32          	pula
    83a6:	7e 8b 38    	jmp	0x8b38
    83a9:	16          	tab
    83aa:	32          	pula
    83ab:	b1 01 00    	cmpa	0x100
    83ae:	2b 04       	bmi	0x0x83b4
    83b0:	8d 19       	bsr	0x0x83cb
    83b2:	20 34       	bra	0x0x83e8
    83b4:	7d 00 fd    	tst	0xfd
    83b7:	27 07       	beq	0x0x83c0
    83b9:	7f 00 fd    	clr	0xfd
    83bc:	36          	psha
    83bd:	7e 82 fe    	jmp	0x82fe
    83c0:	36          	psha
    83c1:	37          	pshb
    83c2:	b6 01 00    	ldaa	0x100
    83c5:	8d 04       	bsr	0x0x83cb
    83c7:	32          	pula
    83c8:	7e 82 fe    	jmp	0x82fe
    83cb:	fe 01 08    	ldx	0x108
    83ce:	c6 01       	ldab	#0x1
    83d0:	f7 01 0a    	stab	0x10a
    83d3:	6d 00       	tst	0x0,x
    83d5:	27 0b       	beq	0x0x83e2
    83d7:	bd 84 33    	jsr	0x8433
    83da:	78 01 0a    	asl	0x10a
    83dd:	24 f4       	bcc	0x0x83d3
    83df:	bd 84 33    	jsr	0x8433
    83e2:	a7 00       	staa	0x0,x
    83e4:	ff 01 08    	stx	0x108
    83e7:	39          	rts
    83e8:	7f 00 fc    	clr	0xfc
    83eb:	bd 91 bc    	jsr	0x91bc
    83ee:	4d          	tsta
    83ef:	2a 03       	bpl	0x0x83f4
    83f1:	7e 82 96    	jmp	0x8296
    83f4:	7c 00 fc    	inc	0xfc
    83f7:	c6 80       	ldab	#0x80
    83f9:	d1 df       	cmpb	*0xdf
    83fb:	27 0a       	beq	0x0x8407
    83fd:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8404
    8401:	7e 8b 9a    	jmp	0x8b9a
    8404:	7e 82 ec    	jmp	0x82ec
    8407:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x840e
    840b:	7e 8c 86    	jmp	0x8c86
    840e:	7e 8a b9    	jmp	0x8ab9
    8411:	fe 01 08    	ldx	0x108
    8414:	c6 02       	ldab	#0x2
    8416:	f7 01 0a    	stab	0x10a
    8419:	e6 00       	ldab	0x0,x
    841b:	27 06       	beq	0x0x8423
    841d:	11          	cba
    841e:	26 03       	bne	0x0x8423
    8420:	6f 00       	clr	0x0,x
    8422:	39          	rts
    8423:	8d 0e       	bsr	0x0x8433
    8425:	78 01 0a    	asl	0x10a
    8428:	24 ef       	bcc	0x0x8419
    842a:	39          	rts
    842b:	8f          	xgdx
    842c:	5a          	decb
    842d:	26 02       	bne	0x0x8431
    842f:	c6 07       	ldab	#0x7
    8431:	8f          	xgdx
    8432:	39          	rts
    8433:	8f          	xgdx
    8434:	5c          	incb
    8435:	c1 07       	cmpb	#0x7
    8437:	23 02       	bls	0x0x843b
    8439:	c6 01       	ldab	#0x1
    843b:	8f          	xgdx
    843c:	39          	rts
    843d:	e6 00       	ldab	0x0,x
    843f:	11          	cba
    8440:	27 04       	beq	0x0x8446
    8442:	8d ef       	bsr	0x0x8433
    8444:	20 f7       	bra	0x0x843d
    8446:	6f 00       	clr	0x0,x
    8448:	39          	rts
    8449:	84 57       	anda	#0x57
    844b:	84 5a       	anda	#0x5a
    844d:	86 33       	ldaa	#0x33
    844f:	86 36       	ldaa	#0x36
    8451:	86 39       	ldaa	#0x39
    8453:	86 3c       	ldaa	#0x3c
    8455:	87          	.byte	0x87
    8456:	c2 7e       	sbcb	#0x7e
    8458:	82 93       	sbca	#0x93
    845a:	81 80       	cmpa	#0x80
    845c:	26 03       	bne	0x0x8461
    845e:	7e 85 5b    	jmp	0x855b
    8461:	bd 91 bc    	jsr	0x91bc
    8464:	b1 50 21    	cmpa	0x5021
    8467:	24 60       	bcc	0x0x84c9
    8469:	13 f8 01 03 	brclr	*0xf8, #0x01, 0x0x8470
    846d:	7e 84 89    	jmp	0x8489
    8470:	36          	psha
    8471:	bd 91 bc    	jsr	0x91bc
    8474:	4d          	tsta
    8475:	26 04       	bne	0x0x847b
    8477:	32          	pula
    8478:	7e 84 ae    	jmp	0x84ae
    847b:	33          	pulb
    847c:	f7 01 00    	stab	0x100
    847f:	18 8f       	xgdy
    8481:	c6 01       	ldab	#0x1
    8483:	bd 83 2e    	jsr	0x832e
    8486:	7e 84 ae    	jmp	0x84ae
    8489:	36          	psha
    848a:	bd 91 bc    	jsr	0x91bc
    848d:	4d          	tsta
    848e:	26 04       	bne	0x0x8494
    8490:	32          	pula
    8491:	7e 85 6b    	jmp	0x856b
    8494:	7d 00 fd    	tst	0xfd
    8497:	27 05       	beq	0x0x849e
    8499:	7f 00 fd    	clr	0xfd
    849c:	20 dd       	bra	0x0x847b
    849e:	fe 01 08    	ldx	0x108
    84a1:	f6 01 00    	ldab	0x100
    84a4:	e7 00       	stab	0x0,x
    84a6:	bd 84 33    	jsr	0x8433
    84a9:	ff 01 08    	stx	0x108
    84ac:	20 cd       	bra	0x0x847b
    84ae:	7f 00 fc    	clr	0xfc
    84b1:	bd 91 bc    	jsr	0x91bc
    84b4:	4d          	tsta
    84b5:	2a 03       	bpl	0x0x84ba
    84b7:	7e 82 96    	jmp	0x8296
    84ba:	7c 00 fc    	inc	0xfc
    84bd:	c6 80       	ldab	#0x80
    84bf:	d1 df       	cmpb	*0xdf
    84c1:	27 03       	beq	0x0x84c6
    84c3:	7e 84 64    	jmp	0x8464
    84c6:	7e 85 5e    	jmp	0x855e
    84c9:	36          	psha
    84ca:	bd 91 bc    	jsr	0x91bc
    84cd:	4d          	tsta
    84ce:	26 04       	bne	0x0x84d4
    84d0:	32          	pula
    84d1:	7e 85 f4    	jmp	0x85f4
    84d4:	33          	pulb
    84d5:	36          	psha
    84d6:	37          	pshb
    84d7:	fe 51 80    	ldx	0x5180
    84da:	8f          	xgdx
    84db:	5c          	incb
    84dc:	d1 f0       	cmpb	*0xf0
    84de:	23 02       	bls	0x0x84e2
    84e0:	c6 01       	ldab	#0x1
    84e2:	8f          	xgdx
    84e3:	d6 f0       	ldab	*0xf0
    84e5:	cb 01       	addb	#0x1
    84e7:	6d 00       	tst	0x0,x
    84e9:	27 13       	beq	0x0x84fe
    84eb:	8f          	xgdx
    84ec:	5c          	incb
    84ed:	d1 f0       	cmpb	*0xf0
    84ef:	23 02       	bls	0x0x84f3
    84f1:	c6 01       	ldab	#0x1
    84f3:	8f          	xgdx
    84f4:	5a          	decb
    84f5:	c1 01       	cmpb	#0x1
    84f7:	24 ee       	bcc	0x0x84e7
    84f9:	32          	pula
    84fa:	33          	pulb
    84fb:	7e 84 ae    	jmp	0x84ae
    84fe:	ff 51 80    	stx	0x5180
    8501:	8f          	xgdx
    8502:	86 01       	ldaa	#0x1
    8504:	5d          	tstb
    8505:	27 04       	beq	0x0x850b
    8507:	5a          	decb
    8508:	48          	asla
    8509:	20 f9       	bra	0x0x8504
    850b:	16          	tab
    850c:	9a f8       	oraa	*0xf8
    850e:	97 f8       	staa	*0xf8
    8510:	53          	comb
    8511:	fe 51 80    	ldx	0x5180
    8514:	18 38       	puly
    8516:	8d 03       	bsr	0x0x851b
    8518:	7e 84 ae    	jmp	0x84ae
    851b:	7d 00 d0    	tst	0xd0
    851e:	27 05       	beq	0x0x8525
    8520:	7f 00 d0    	clr	0xd0
    8523:	20 08       	bra	0x0x852d
    8525:	7d 10 29    	tst	0x1029
    8528:	2a fb       	bpl	0x0x8525
    852a:	b6 10 2a    	ldaa	0x102a
    852d:	01          	nop
    852e:	01          	nop
    852f:	01          	nop
    8530:	01          	nop
    8531:	f7 10 42    	stab	0x1042
    8534:	01          	nop
    8535:	01          	nop
    8536:	01          	nop
    8537:	01          	nop
    8538:	01          	nop
    8539:	01          	nop
    853a:	01          	nop
    853b:	86 81       	ldaa	#0x81
    853d:	b7 10 2a    	staa	0x102a
    8540:	7d 10 29    	tst	0x1029
    8543:	2a fb       	bpl	0x0x8540
    8545:	b6 10 2a    	ldaa	0x102a
    8548:	18 8f       	xgdy
    854a:	a7 00       	staa	0x0,x
    854c:	b7 10 2a    	staa	0x102a
    854f:	7d 10 29    	tst	0x1029
    8552:	2a fb       	bpl	0x0x854f
    8554:	b6 10 2a    	ldaa	0x102a
    8557:	f7 10 2a    	stab	0x102a
    855a:	39          	rts
    855b:	bd 91 bc    	jsr	0x91bc
    855e:	b1 50 21    	cmpa	0x5021
    8561:	25 03       	bcs	0x0x8566
    8563:	7e 85 ef    	jmp	0x85ef
    8566:	36          	psha
    8567:	bd 91 bc    	jsr	0x91bc
    856a:	32          	pula
    856b:	12 f8 01 03 	brset	*0xf8, #0x01, 0x0x8572
    856f:	7e 85 d4    	jmp	0x85d4
    8572:	b1 01 00    	cmpa	0x100
    8575:	27 06       	beq	0x0x857d
    8577:	bd 84 11    	jsr	0x8411
    857a:	7e 85 d4    	jmp	0x85d4
    857d:	7f 01 00    	clr	0x100
    8580:	fe 01 08    	ldx	0x108
    8583:	c6 01       	ldab	#0x1
    8585:	f7 01 0a    	stab	0x10a
    8588:	a6 00       	ldaa	0x0,x
    858a:	26 34       	bne	0x0x85c0
    858c:	bd 84 2b    	jsr	0x842b
    858f:	78 01 0a    	asl	0x10a
    8592:	24 f4       	bcc	0x0x8588
    8594:	15 f8 01    	bclr	*0xf8, #0x01
    8597:	7d 00 d0    	tst	0xd0
    859a:	27 05       	beq	0x0x85a1
    859c:	7f 00 d0    	clr	0xd0
    859f:	20 08       	bra	0x0x85a9
    85a1:	7d 10 29    	tst	0x1029
    85a4:	2a fb       	bpl	0x0x85a1
    85a6:	f6 10 2a    	ldab	0x102a
    85a9:	c6 fe       	ldab	#0xfe
    85ab:	01          	nop
    85ac:	01          	nop
    85ad:	01          	nop
    85ae:	01          	nop
    85af:	f7 10 42    	stab	0x1042
    85b2:	01          	nop
    85b3:	01          	nop
    85b4:	01          	nop
    85b5:	01          	nop
    85b6:	01          	nop
    85b7:	01          	nop
    85b8:	01          	nop
    85b9:	86 80       	ldaa	#0x80
    85bb:	b7 10 2a    	staa	0x102a
    85be:	20 14       	bra	0x0x85d4
    85c0:	6f 00       	clr	0x0,x
    85c2:	16          	tab
    85c3:	f7 01 00    	stab	0x100
    85c6:	bd 84 2b    	jsr	0x842b
    85c9:	ff 01 08    	stx	0x108
    85cc:	4f          	clra
    85cd:	18 8f       	xgdy
    85cf:	c6 01       	ldab	#0x1
    85d1:	bd 83 2e    	jsr	0x832e
    85d4:	7f 00 fc    	clr	0xfc
    85d7:	bd 91 bc    	jsr	0x91bc
    85da:	4d          	tsta
    85db:	2a 03       	bpl	0x0x85e0
    85dd:	7e 82 96    	jmp	0x8296
    85e0:	7c 00 fc    	inc	0xfc
    85e3:	c6 80       	ldab	#0x80
    85e5:	d1 df       	cmpb	*0xdf
    85e7:	27 03       	beq	0x0x85ec
    85e9:	7e 84 64    	jmp	0x8464
    85ec:	7e 85 5e    	jmp	0x855e
    85ef:	36          	psha
    85f0:	bd 91 bc    	jsr	0x91bc
    85f3:	32          	pula
    85f4:	ce 51 01    	ldx	#0x5101
    85f7:	c6 02       	ldab	#0x2
    85f9:	a1 00       	cmpa	0x0,x
    85fb:	27 06       	beq	0x0x8603
    85fd:	08          	inx
    85fe:	58          	aslb
    85ff:	24 f8       	bcc	0x0x85f9
    8601:	20 d1       	bra	0x0x85d4
    8603:	6f 00       	clr	0x0,x
    8605:	17          	tba
    8606:	98 f8       	eora	*0xf8
    8608:	97 f8       	staa	*0xf8
    860a:	53          	comb
    860b:	7d 00 d0    	tst	0xd0
    860e:	27 05       	beq	0x0x8615
    8610:	7f 00 d0    	clr	0xd0
    8613:	20 08       	bra	0x0x861d
    8615:	7d 10 29    	tst	0x1029
    8618:	2a fb       	bpl	0x0x8615
    861a:	b6 10 2a    	ldaa	0x102a
    861d:	01          	nop
    861e:	01          	nop
    861f:	01          	nop
    8620:	01          	nop
    8621:	f7 10 42    	stab	0x1042
    8624:	01          	nop
    8625:	01          	nop
    8626:	01          	nop
    8627:	01          	nop
    8628:	01          	nop
    8629:	01          	nop
    862a:	01          	nop
    862b:	86 80       	ldaa	#0x80
    862d:	b7 10 2a    	staa	0x102a
    8630:	7e 85 d4    	jmp	0x85d4
    8633:	7e 82 93    	jmp	0x8293
    8636:	7e 82 93    	jmp	0x8293
    8639:	7e 82 93    	jmp	0x8293
    863c:	96 df       	ldaa	*0xdf
    863e:	81 80       	cmpa	#0x80
    8640:	26 03       	bne	0x0x8645
    8642:	7e 87 15    	jmp	0x8715
    8645:	96 f0       	ldaa	*0xf0
    8647:	4c          	inca
    8648:	44          	lsra
    8649:	b7 01 1f    	staa	0x11f
    864c:	bd 91 bc    	jsr	0x91bc
    864f:	36          	psha
    8650:	bd 91 bc    	jsr	0x91bc
    8653:	4d          	tsta
    8654:	26 04       	bne	0x0x865a
    8656:	32          	pula
    8657:	7e 87 1d    	jmp	0x871d
    865a:	33          	pulb
    865b:	36          	psha
    865c:	37          	pshb
    865d:	fe 01 08    	ldx	0x108
    8660:	8f          	xgdx
    8661:	5c          	incb
    8662:	f1 01 1f    	cmpb	0x11f
    8665:	25 01       	bcs	0x0x8668
    8667:	5f          	clrb
    8668:	8f          	xgdx
    8669:	f6 01 1f    	ldab	0x11f
    866c:	6d 00       	tst	0x0,x
    866e:	27 13       	beq	0x0x8683
    8670:	8f          	xgdx
    8671:	5c          	incb
    8672:	f1 01 1f    	cmpb	0x11f
    8675:	25 01       	bcs	0x0x8678
    8677:	5f          	clrb
    8678:	8f          	xgdx
    8679:	5a          	decb
    867a:	26 f0       	bne	0x0x866c
    867c:	f6 01 1f    	ldab	0x11f
    867f:	5c          	incb
    8680:	7e 86 fd    	jmp	0x86fd
    8683:	ff 01 08    	stx	0x108
    8686:	8f          	xgdx
    8687:	86 01       	ldaa	#0x1
    8689:	5d          	tstb
    868a:	27 04       	beq	0x0x8690
    868c:	5a          	decb
    868d:	48          	asla
    868e:	20 f9       	bra	0x0x8689
    8690:	36          	psha
    8691:	f6 01 1f    	ldab	0x11f
    8694:	48          	asla
    8695:	5a          	decb
    8696:	26 fc       	bne	0x0x8694
    8698:	33          	pulb
    8699:	1b          	aba
    869a:	16          	tab
    869b:	9a f8       	oraa	*0xf8
    869d:	97 f8       	staa	*0xf8
    869f:	53          	comb
    86a0:	fe 01 08    	ldx	0x108
    86a3:	7d 00 d0    	tst	0xd0
    86a6:	27 05       	beq	0x0x86ad
    86a8:	7f 00 d0    	clr	0xd0
    86ab:	20 08       	bra	0x0x86b5
    86ad:	7d 10 29    	tst	0x1029
    86b0:	2a fb       	bpl	0x0x86ad
    86b2:	b6 10 2a    	ldaa	0x102a
    86b5:	01          	nop
    86b6:	01          	nop
    86b7:	01          	nop
    86b8:	01          	nop
    86b9:	f7 10 42    	stab	0x1042
    86bc:	01          	nop
    86bd:	01          	nop
    86be:	01          	nop
    86bf:	01          	nop
    86c0:	01          	nop
    86c1:	01          	nop
    86c2:	01          	nop
    86c3:	86 81       	ldaa	#0x81
    86c5:	b7 10 2a    	staa	0x102a
    86c8:	7d 10 29    	tst	0x1029
    86cb:	2a fb       	bpl	0x0x86c8
    86cd:	b6 10 2a    	ldaa	0x102a
    86d0:	32          	pula
    86d1:	a7 00       	staa	0x0,x
    86d3:	b7 10 2a    	staa	0x102a
    86d6:	7d 10 29    	tst	0x1029
    86d9:	2a fb       	bpl	0x0x86d6
    86db:	b6 10 2a    	ldaa	0x102a
    86de:	32          	pula
    86df:	b7 10 2a    	staa	0x102a
    86e2:	7f 00 fc    	clr	0xfc
    86e5:	bd 91 bc    	jsr	0x91bc
    86e8:	4d          	tsta
    86e9:	2a 03       	bpl	0x0x86ee
    86eb:	7e 82 96    	jmp	0x8296
    86ee:	7c 00 fc    	inc	0xfc
    86f1:	c6 80       	ldab	#0x80
    86f3:	d1 df       	cmpb	*0xdf
    86f5:	27 03       	beq	0x0x86fa
    86f7:	7e 86 4f    	jmp	0x864f
    86fa:	7e 87 ba    	jmp	0x87ba
    86fd:	ce 01 00    	ldx	#0x100
    8700:	3a          	abx
    8701:	e6 00       	ldab	0x0,x
    8703:	27 0a       	beq	0x0x870f
    8705:	08          	inx
    8706:	8c 01 08    	cpx	#0x108
    8709:	25 f6       	bcs	0x0x8701
    870b:	32          	pula
    870c:	33          	pulb
    870d:	20 d3       	bra	0x0x86e2
    870f:	32          	pula
    8710:	a7 00       	staa	0x0,x
    8712:	33          	pulb
    8713:	20 cd       	bra	0x0x86e2
    8715:	bd 91 bc    	jsr	0x91bc
    8718:	36          	psha
    8719:	bd 91 bc    	jsr	0x91bc
    871c:	32          	pula
    871d:	ce 01 00    	ldx	#0x100
    8720:	c6 01       	ldab	#0x1
    8722:	a1 00       	cmpa	0x0,x
    8724:	27 07       	beq	0x0x872d
    8726:	08          	inx
    8727:	58          	aslb
    8728:	24 f8       	bcc	0x0x8722
    872a:	7e 87 a2    	jmp	0x87a2
    872d:	6f 00       	clr	0x0,x
    872f:	8f          	xgdx
    8730:	f1 01 1f    	cmpb	0x11f
    8733:	23 03       	bls	0x0x8738
    8735:	7e 87 a2    	jmp	0x87a2
    8738:	8f          	xgdx
    8739:	37          	pshb
    873a:	18 ce 01 00 	ldy	#0x100
    873e:	f6 01 1f    	ldab	0x11f
    8741:	18 3a       	aby
    8743:	18 a6 00    	ldaa	0x0,y
    8746:	26 0b       	bne	0x0x8753
    8748:	18 08       	iny
    874a:	18 8c 01 08 	cpy	#0x108
    874e:	25 f3       	bcs	0x0x8743
    8750:	32          	pula
    8751:	20 19       	bra	0x0x876c
    8753:	32          	pula
    8754:	36          	psha
    8755:	f6 01 1f    	ldab	0x11f
    8758:	48          	asla
    8759:	5a          	decb
    875a:	26 fc       	bne	0x0x8758
    875c:	33          	pulb
    875d:	1b          	aba
    875e:	16          	tab
    875f:	53          	comb
    8760:	4f          	clra
    8761:	36          	psha
    8762:	18 a6 00    	ldaa	0x0,y
    8765:	36          	psha
    8766:	18 6f 00    	clr	0x0,y
    8769:	7e 86 a3    	jmp	0x86a3
    876c:	36          	psha
    876d:	f6 01 1f    	ldab	0x11f
    8770:	48          	asla
    8771:	5a          	decb
    8772:	26 fc       	bne	0x0x8770
    8774:	33          	pulb
    8775:	1b          	aba
    8776:	16          	tab
    8777:	98 f8       	eora	*0xf8
    8779:	97 f8       	staa	*0xf8
    877b:	53          	comb
    877c:	7d 00 d0    	tst	0xd0
    877f:	27 05       	beq	0x0x8786
    8781:	7f 00 d0    	clr	0xd0
    8784:	20 08       	bra	0x0x878e
    8786:	7d 10 29    	tst	0x1029
    8789:	2a fb       	bpl	0x0x8786
    878b:	b6 10 2a    	ldaa	0x102a
    878e:	01          	nop
    878f:	01          	nop
    8790:	01          	nop
    8791:	01          	nop
    8792:	f7 10 42    	stab	0x1042
    8795:	01          	nop
    8796:	01          	nop
    8797:	01          	nop
    8798:	01          	nop
    8799:	01          	nop
    879a:	01          	nop
    879b:	01          	nop
    879c:	01          	nop
    879d:	86 80       	ldaa	#0x80
    879f:	b7 10 2a    	staa	0x102a
    87a2:	7f 00 fc    	clr	0xfc
    87a5:	bd 91 bc    	jsr	0x91bc
    87a8:	4d          	tsta
    87a9:	2a 03       	bpl	0x0x87ae
    87ab:	7e 82 96    	jmp	0x8296
    87ae:	7c 00 fc    	inc	0xfc
    87b1:	c6 80       	ldab	#0x80
    87b3:	d1 df       	cmpb	*0xdf
    87b5:	27 03       	beq	0x0x87ba
    87b7:	7e 86 4f    	jmp	0x864f
    87ba:	36          	psha
    87bb:	bd 91 bc    	jsr	0x91bc
    87be:	32          	pula
    87bf:	7e 87 1d    	jmp	0x871d
    87c2:	d6 df       	ldab	*0xdf
    87c4:	c4 0f       	andb	#0xf
    87c6:	d7 1e       	stab	*0x1e
    87c8:	ce 50 50    	ldx	#0x5050
    87cb:	3a          	abx
    87cc:	6d 08       	tst	0x8,x
    87ce:	27 03       	beq	0x0x87d3
    87d0:	7e 89 29    	jmp	0x8929
    87d3:	84 f0       	anda	#0xf0
    87d5:	81 80       	cmpa	#0x80
    87d7:	26 03       	bne	0x0x87dc
    87d9:	7e 88 63    	jmp	0x8863
    87dc:	bd 91 bc    	jsr	0x91bc
    87df:	ce 50 50    	ldx	#0x5050
    87e2:	d6 1e       	ldab	*0x1e
    87e4:	3a          	abx
    87e5:	e6 00       	ldab	0x0,x
    87e7:	26 06       	bne	0x0x87ef
    87e9:	bd 91 bc    	jsr	0x91bc
    87ec:	7e 88 51    	jmp	0x8851
    87ef:	d4 f8       	andb	*0xf8
    87f1:	26 24       	bne	0x0x8817
    87f3:	36          	psha
    87f4:	bd 91 bc    	jsr	0x91bc
    87f7:	4d          	tsta
    87f8:	26 04       	bne	0x0x87fe
    87fa:	32          	pula
    87fb:	7e 88 51    	jmp	0x8851
    87fe:	ce 00 00    	ldx	#0x0
    8801:	d6 1e       	ldab	*0x1e
    8803:	3a          	abx
    8804:	33          	pulb
    8805:	e7 00       	stab	0x0,x
    8807:	18 8f       	xgdy
    8809:	ce 50 50    	ldx	#0x5050
    880c:	d6 1e       	ldab	*0x1e
    880e:	3a          	abx
    880f:	e6 00       	ldab	0x0,x
    8811:	bd 83 2e    	jsr	0x832e
    8814:	7e 88 51    	jmp	0x8851
    8817:	36          	psha
    8818:	bd 91 bc    	jsr	0x91bc
    881b:	4d          	tsta
    881c:	26 04       	bne	0x0x8822
    881e:	32          	pula
    881f:	7e 88 6b    	jmp	0x886b
    8822:	36          	psha
    8823:	18 ce 00 00 	ldy	#0x0
    8827:	d6 1e       	ldab	*0x1e
    8829:	18 3a       	aby
    882b:	ce 51 80    	ldx	#0x5180
    882e:	58          	aslb
    882f:	3a          	abx
    8830:	3c          	pshx
    8831:	ee 00       	ldx	0x0,x
    8833:	18 e6 00    	ldab	0x0,y
    8836:	e7 00       	stab	0x0,x
    8838:	8f          	xgdx
    8839:	bd 8a 57    	jsr	0x8a57
    883c:	38          	pulx
    883d:	ed 00       	std	0x0,x
    883f:	32          	pula
    8840:	33          	pulb
    8841:	18 e7 00    	stab	0x0,y
    8844:	18 8f       	xgdy
    8846:	ce 50 50    	ldx	#0x5050
    8849:	d6 1e       	ldab	*0x1e
    884b:	3a          	abx
    884c:	e6 00       	ldab	0x0,x
    884e:	bd 83 2e    	jsr	0x832e
    8851:	7f 00 fc    	clr	0xfc
    8854:	bd 91 bc    	jsr	0x91bc
    8857:	4d          	tsta
    8858:	2a 03       	bpl	0x0x885d
    885a:	7e 82 96    	jmp	0x8296
    885d:	7c 00 fc    	inc	0xfc
    8860:	7e 88 17    	jmp	0x8817
    8863:	bd 91 bc    	jsr	0x91bc
    8866:	36          	psha
    8867:	bd 91 bc    	jsr	0x91bc
    886a:	32          	pula
    886b:	ce 50 50    	ldx	#0x5050
    886e:	d6 1e       	ldab	*0x1e
    8870:	3a          	abx
    8871:	6d 00       	tst	0x0,x
    8873:	26 03       	bne	0x0x8878
    8875:	7e 89 0c    	jmp	0x890c
    8878:	ce 00 00    	ldx	#0x0
    887b:	3a          	abx
    887c:	6d 00       	tst	0x0,x
    887e:	26 03       	bne	0x0x8883
    8880:	7e 89 0c    	jmp	0x890c
    8883:	a1 00       	cmpa	0x0,x
    8885:	27 06       	beq	0x0x888d
    8887:	bd 8a 77    	jsr	0x8a77
    888a:	7e 89 0c    	jmp	0x890c
    888d:	6f 00       	clr	0x0,x
    888f:	18 ce 51 80 	ldy	#0x5180
    8893:	d6 1e       	ldab	*0x1e
    8895:	58          	aslb
    8896:	18 3a       	aby
    8898:	18 ee 00    	ldy	0x0,y
    889b:	c6 01       	ldab	#0x1
    889d:	f7 01 0a    	stab	0x10a
    88a0:	18 a6 00    	ldaa	0x0,y
    88a3:	26 42       	bne	0x0x88e7
    88a5:	18 8f       	xgdy
    88a7:	bd 8a 67    	jsr	0x8a67
    88aa:	18 8f       	xgdy
    88ac:	78 01 0a    	asl	0x10a
    88af:	24 ef       	bcc	0x0x88a0
    88b1:	ce 50 50    	ldx	#0x5050
    88b4:	d6 1e       	ldab	*0x1e
    88b6:	3a          	abx
    88b7:	e6 00       	ldab	0x0,x
    88b9:	53          	comb
    88ba:	37          	pshb
    88bb:	d4 f8       	andb	*0xf8
    88bd:	d7 f8       	stab	*0xf8
    88bf:	7d 00 d0    	tst	0xd0
    88c2:	27 05       	beq	0x0x88c9
    88c4:	7f 00 d0    	clr	0xd0
    88c7:	20 08       	bra	0x0x88d1
    88c9:	7d 10 29    	tst	0x1029
    88cc:	2a fb       	bpl	0x0x88c9
    88ce:	f6 10 2a    	ldab	0x102a
    88d1:	33          	pulb
    88d2:	01          	nop
    88d3:	01          	nop
    88d4:	01          	nop
    88d5:	01          	nop
    88d6:	f7 10 42    	stab	0x1042
    88d9:	01          	nop
    88da:	01          	nop
    88db:	01          	nop
    88dc:	01          	nop
    88dd:	01          	nop
    88de:	01          	nop
    88df:	01          	nop
    88e0:	86 80       	ldaa	#0x80
    88e2:	b7 10 2a    	staa	0x102a
    88e5:	20 25       	bra	0x0x890c
    88e7:	18 6f 00    	clr	0x0,y
    88ea:	a7 00       	staa	0x0,x
    88ec:	18 8f       	xgdy
    88ee:	bd 8a 67    	jsr	0x8a67
    88f1:	18 8f       	xgdy
    88f3:	ce 51 80    	ldx	#0x5180
    88f6:	d6 1e       	ldab	*0x1e
    88f8:	58          	aslb
    88f9:	3a          	abx
    88fa:	1a ef 00    	sty	0x0,x
    88fd:	16          	tab
    88fe:	4f          	clra
    88ff:	18 8f       	xgdy
    8901:	ce 50 50    	ldx	#0x5050
    8904:	d6 1e       	ldab	*0x1e
    8906:	3a          	abx
    8907:	e6 00       	ldab	0x0,x
    8909:	bd 83 2e    	jsr	0x832e
    890c:	7f 00 fc    	clr	0xfc
    890f:	bd 91 bc    	jsr	0x91bc
    8912:	4d          	tsta
    8913:	2a 03       	bpl	0x0x8918
    8915:	7e 82 96    	jmp	0x8296
    8918:	7c 00 fc    	inc	0xfc
    891b:	d6 df       	ldab	*0xdf
    891d:	c4 f0       	andb	#0xf0
    891f:	c1 80       	cmpb	#0x80
    8921:	27 03       	beq	0x0x8926
    8923:	7e 87 df    	jmp	0x87df
    8926:	7e 88 66    	jmp	0x8866
    8929:	96 df       	ldaa	*0xdf
    892b:	84 f0       	anda	#0xf0
    892d:	81 80       	cmpa	#0x80
    892f:	26 06       	bne	0x0x8937
    8931:	bd 91 bc    	jsr	0x91bc
    8934:	7e 89 d0    	jmp	0x89d0
    8937:	bd 91 bc    	jsr	0x91bc
    893a:	36          	psha
    893b:	ce 50 50    	ldx	#0x5050
    893e:	d6 1e       	ldab	*0x1e
    8940:	3a          	abx
    8941:	6d 00       	tst	0x0,x
    8943:	26 07       	bne	0x0x894c
    8945:	32          	pula
    8946:	bd 91 bc    	jsr	0x91bc
    8949:	7e 89 be    	jmp	0x89be
    894c:	bd 91 bc    	jsr	0x91bc
    894f:	4d          	tsta
    8950:	26 03       	bne	0x0x8955
    8952:	7e 89 d4    	jmp	0x89d4
    8955:	33          	pulb
    8956:	36          	psha
    8957:	37          	pshb
    8958:	18 ce 51 00 	ldy	#0x5100
    895c:	d6 1e       	ldab	*0x1e
    895e:	86 10       	ldaa	#0x10
    8960:	3d          	mul
    8961:	18 3a       	aby
    8963:	ce 51 80    	ldx	#0x5180
    8966:	d6 1e       	ldab	*0x1e
    8968:	58          	aslb
    8969:	3a          	abx
    896a:	ec 00       	ldd	0x0,x
    896c:	5c          	incb
    896d:	18 e1 09    	cmpb	0x9,y
    8970:	23 03       	bls	0x0x8975
    8972:	18 e6 08    	ldab	0x8,y
    8975:	8f          	xgdx
    8976:	18 e6 0a    	ldab	0xa,y
    8979:	6d 00       	tst	0x0,x
    897b:	27 13       	beq	0x0x8990
    897d:	8f          	xgdx
    897e:	5c          	incb
    897f:	18 e1 09    	cmpb	0x9,y
    8982:	23 03       	bls	0x0x8987
    8984:	18 e6 08    	ldab	0x8,y
    8987:	8f          	xgdx
    8988:	5a          	decb
    8989:	26 ee       	bne	0x0x8979
    898b:	32          	pula
    898c:	33          	pulb
    898d:	7e 89 be    	jmp	0x89be
    8990:	d6 1e       	ldab	*0x1e
    8992:	58          	aslb
    8993:	18 ce 51 80 	ldy	#0x5180
    8997:	18 3a       	aby
    8999:	cd ef 00    	stx	0x0,y
    899c:	8f          	xgdx
    899d:	86 01       	ldaa	#0x1
    899f:	c4 07       	andb	#0x7
    89a1:	5d          	tstb
    89a2:	27 04       	beq	0x0x89a8
    89a4:	5a          	decb
    89a5:	48          	asla
    89a6:	20 f9       	bra	0x0x89a1
    89a8:	16          	tab
    89a9:	9a f8       	oraa	*0xf8
    89ab:	97 f8       	staa	*0xf8
    89ad:	53          	comb
    89ae:	37          	pshb
    89af:	d6 1e       	ldab	*0x1e
    89b1:	58          	aslb
    89b2:	ce 51 80    	ldx	#0x5180
    89b5:	3a          	abx
    89b6:	ee 00       	ldx	0x0,x
    89b8:	33          	pulb
    89b9:	18 38       	puly
    89bb:	bd 85 1b    	jsr	0x851b
    89be:	7f 00 fc    	clr	0xfc
    89c1:	bd 91 bc    	jsr	0x91bc
    89c4:	4d          	tsta
    89c5:	2a 03       	bpl	0x0x89ca
    89c7:	7e 82 96    	jmp	0x8296
    89ca:	7c 00 fc    	inc	0xfc
    89cd:	7e 89 3a    	jmp	0x893a
    89d0:	36          	psha
    89d1:	bd 91 bc    	jsr	0x91bc
    89d4:	ce 50 50    	ldx	#0x5050
    89d7:	d6 1e       	ldab	*0x1e
    89d9:	3a          	abx
    89da:	6d 00       	tst	0x0,x
    89dc:	26 04       	bne	0x0x89e2
    89de:	32          	pula
    89df:	7e 8a 3a    	jmp	0x8a3a
    89e2:	18 ce 51 00 	ldy	#0x5100
    89e6:	d6 1e       	ldab	*0x1e
    89e8:	86 10       	ldaa	#0x10
    89ea:	3d          	mul
    89eb:	18 3a       	aby
    89ed:	18 3c       	pshy
    89ef:	38          	pulx
    89f0:	18 e6 08    	ldab	0x8,y
    89f3:	c4 07       	andb	#0x7
    89f5:	3a          	abx
    89f6:	18 e6 0a    	ldab	0xa,y
    89f9:	32          	pula
    89fa:	a1 00       	cmpa	0x0,x
    89fc:	27 06       	beq	0x0x8a04
    89fe:	08          	inx
    89ff:	5a          	decb
    8a00:	26 f8       	bne	0x0x89fa
    8a02:	20 36       	bra	0x0x8a3a
    8a04:	6f 00       	clr	0x0,x
    8a06:	8f          	xgdx
    8a07:	c4 07       	andb	#0x7
    8a09:	4f          	clra
    8a0a:	0d          	sec
    8a0b:	49          	rola
    8a0c:	5a          	decb
    8a0d:	2a fc       	bpl	0x0x8a0b
    8a0f:	16          	tab
    8a10:	98 f8       	eora	*0xf8
    8a12:	97 f8       	staa	*0xf8
    8a14:	53          	comb
    8a15:	7d 00 d0    	tst	0xd0
    8a18:	27 05       	beq	0x0x8a1f
    8a1a:	7f 00 d0    	clr	0xd0
    8a1d:	20 08       	bra	0x0x8a27
    8a1f:	7d 10 29    	tst	0x1029
    8a22:	2a fb       	bpl	0x0x8a1f
    8a24:	b6 10 2a    	ldaa	0x102a
    8a27:	01          	nop
    8a28:	01          	nop
    8a29:	01          	nop
    8a2a:	01          	nop
    8a2b:	f7 10 42    	stab	0x1042
    8a2e:	01          	nop
    8a2f:	01          	nop
    8a30:	01          	nop
    8a31:	01          	nop
    8a32:	01          	nop
    8a33:	01          	nop
    8a34:	01          	nop
    8a35:	86 80       	ldaa	#0x80
    8a37:	b7 10 2a    	staa	0x102a
    8a3a:	7f 00 fc    	clr	0xfc
    8a3d:	bd 91 bc    	jsr	0x91bc
    8a40:	4d          	tsta
    8a41:	2a 03       	bpl	0x0x8a46
    8a43:	7e 82 96    	jmp	0x8296
    8a46:	7c 00 fc    	inc	0xfc
    8a49:	d6 df       	ldab	*0xdf
    8a4b:	c4 f0       	andb	#0xf0
    8a4d:	c1 80       	cmpb	#0x80
    8a4f:	27 03       	beq	0x0x8a54
    8a51:	7e 89 3a    	jmp	0x893a
    8a54:	7e 89 d0    	jmp	0x89d0
    8a57:	37          	pshb
    8a58:	c4 f0       	andb	#0xf0
    8a5a:	f7 01 1f    	stab	0x11f
    8a5d:	33          	pulb
    8a5e:	c4 07       	andb	#0x7
    8a60:	5c          	incb
    8a61:	c4 07       	andb	#0x7
    8a63:	fb 01 1f    	addb	0x11f
    8a66:	39          	rts
    8a67:	37          	pshb
    8a68:	c4 f0       	andb	#0xf0
    8a6a:	f7 01 1f    	stab	0x11f
    8a6d:	33          	pulb
    8a6e:	c4 07       	andb	#0x7
    8a70:	5a          	decb
    8a71:	c4 07       	andb	#0x7
    8a73:	fb 01 1f    	addb	0x11f
    8a76:	39          	rts
    8a77:	ce 51 80    	ldx	#0x5180
    8a7a:	d6 1e       	ldab	*0x1e
    8a7c:	58          	aslb
    8a7d:	3a          	abx
    8a7e:	ee 00       	ldx	0x0,x
    8a80:	c6 01       	ldab	#0x1
    8a82:	f7 01 0a    	stab	0x10a
    8a85:	e6 00       	ldab	0x0,x
    8a87:	27 06       	beq	0x0x8a8f
    8a89:	11          	cba
    8a8a:	26 03       	bne	0x0x8a8f
    8a8c:	6f 00       	clr	0x0,x
    8a8e:	39          	rts
    8a8f:	8f          	xgdx
    8a90:	8d c5       	bsr	0x0x8a57
    8a92:	8f          	xgdx
    8a93:	78 01 0a    	asl	0x10a
    8a96:	24 ed       	bcc	0x0x8a85
    8a98:	39          	rts
    8a99:	97 df       	staa	*0xdf
    8a9b:	7c 00 fc    	inc	0xfc
    8a9e:	12 d1 80 0d 	brset	*0xd1, #0x80, 0x0x8aaf
    8aa2:	d6 d1       	ldab	*0xd1
    8aa4:	27 09       	beq	0x0x8aaf
    8aa6:	ce 84 49    	ldx	#0x8449
    8aa9:	58          	aslb
    8aaa:	3a          	abx
    8aab:	ee 00       	ldx	0x0,x
    8aad:	6e 00       	jmp	0x0,x
    8aaf:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8ab6
    8ab3:	7e 8c 83    	jmp	0x8c83
    8ab6:	bd 91 bc    	jsr	0x91bc
    8ab9:	36          	psha
    8aba:	bd 91 bc    	jsr	0x91bc
    8abd:	32          	pula
    8abe:	7d 01 00    	tst	0x100
    8ac1:	26 06       	bne	0x0x8ac9
    8ac3:	bd 84 11    	jsr	0x8411
    8ac6:	7e 8b 6e    	jmp	0x8b6e
    8ac9:	7d 00 69    	tst	0x69
    8acc:	27 02       	beq	0x0x8ad0
    8ace:	20 68       	bra	0x0x8b38
    8ad0:	b1 01 00    	cmpa	0x100
    8ad3:	27 06       	beq	0x0x8adb
    8ad5:	bd 84 11    	jsr	0x8411
    8ad8:	7e 8b 6e    	jmp	0x8b6e
    8adb:	7f 01 00    	clr	0x100
    8ade:	fe 01 08    	ldx	0x108
    8ae1:	c6 01       	ldab	#0x1
    8ae3:	f7 01 0a    	stab	0x10a
    8ae6:	a6 00       	ldaa	0x0,x
    8ae8:	26 41       	bne	0x0x8b2b
    8aea:	bd 84 2b    	jsr	0x842b
    8aed:	78 01 0a    	asl	0x10a
    8af0:	24 f4       	bcc	0x0x8ae6
    8af2:	7d 00 f1    	tst	0xf1
    8af5:	27 08       	beq	0x0x8aff
    8af7:	bd 83 0d    	jsr	0x830d
    8afa:	d7 fd       	stab	*0xfd
    8afc:	7e 83 e8    	jmp	0x83e8
    8aff:	7f 00 f8    	clr	0xf8
    8b02:	7d 00 d0    	tst	0xd0
    8b05:	27 05       	beq	0x0x8b0c
    8b07:	7f 00 d0    	clr	0xd0
    8b0a:	20 08       	bra	0x0x8b14
    8b0c:	7d 10 29    	tst	0x1029
    8b0f:	2a fb       	bpl	0x0x8b0c
    8b11:	f6 10 2a    	ldab	0x102a
    8b14:	5f          	clrb
    8b15:	01          	nop
    8b16:	01          	nop
    8b17:	01          	nop
    8b18:	01          	nop
    8b19:	f7 10 42    	stab	0x1042
    8b1c:	01          	nop
    8b1d:	01          	nop
    8b1e:	01          	nop
    8b1f:	01          	nop
    8b20:	01          	nop
    8b21:	01          	nop
    8b22:	01          	nop
    8b23:	86 80       	ldaa	#0x80
    8b25:	b7 10 2a    	staa	0x102a
    8b28:	7e 83 e8    	jmp	0x83e8
    8b2b:	6f 00       	clr	0x0,x
    8b2d:	36          	psha
    8b2e:	bd 84 2b    	jsr	0x842b
    8b31:	ff 01 08    	stx	0x108
    8b34:	4f          	clra
    8b35:	7e 82 fe    	jmp	0x82fe
    8b38:	b1 01 00    	cmpa	0x100
    8b3b:	27 05       	beq	0x0x8b42
    8b3d:	bd 84 11    	jsr	0x8411
    8b40:	20 2c       	bra	0x0x8b6e
    8b42:	7f 01 00    	clr	0x100
    8b45:	fe 01 08    	ldx	0x108
    8b48:	c6 01       	ldab	#0x1
    8b4a:	f7 01 0a    	stab	0x10a
    8b4d:	86 80       	ldaa	#0x80
    8b4f:	e6 00       	ldab	0x0,x
    8b51:	27 04       	beq	0x0x8b57
    8b53:	11          	cba
    8b54:	25 01       	bcs	0x0x8b57
    8b56:	17          	tba
    8b57:	bd 84 33    	jsr	0x8433
    8b5a:	78 01 0a    	asl	0x10a
    8b5d:	24 f0       	bcc	0x0x8b4f
    8b5f:	81 80       	cmpa	#0x80
    8b61:	26 03       	bne	0x0x8b66
    8b63:	7e 8a f2    	jmp	0x8af2
    8b66:	36          	psha
    8b67:	bd 84 3d    	jsr	0x843d
    8b6a:	4f          	clra
    8b6b:	7e 82 fe    	jmp	0x82fe
    8b6e:	7f 00 fc    	clr	0xfc
    8b71:	bd 91 bc    	jsr	0x91bc
    8b74:	4d          	tsta
    8b75:	2a 03       	bpl	0x0x8b7a
    8b77:	7e 82 96    	jmp	0x8296
    8b7a:	7c 00 fc    	inc	0xfc
    8b7d:	c6 80       	ldab	#0x80
    8b7f:	d1 df       	cmpb	*0xdf
    8b81:	27 0a       	beq	0x0x8b8d
    8b83:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8b8a
    8b87:	7e 8b 9a    	jmp	0x8b9a
    8b8a:	7e 82 ec    	jmp	0x82ec
    8b8d:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8b94
    8b91:	7e 8c 86    	jmp	0x8c86
    8b94:	7e 8a b9    	jmp	0x8ab9
    8b97:	bd 91 bc    	jsr	0x91bc
    8b9a:	36          	psha
    8b9b:	bd 91 bc    	jsr	0x91bc
    8b9e:	4d          	tsta
    8b9f:	26 04       	bne	0x0x8ba5
    8ba1:	32          	pula
    8ba2:	7e 8c 8b    	jmp	0x8c8b
    8ba5:	7d 00 95    	tst	0x95
    8ba8:	27 31       	beq	0x0x8bdb
    8baa:	33          	pulb
    8bab:	36          	psha
    8bac:	37          	pshb
    8bad:	ce 01 00    	ldx	#0x100
    8bb0:	6d 00       	tst	0x0,x
    8bb2:	27 4d       	beq	0x0x8c01
    8bb4:	8f          	xgdx
    8bb5:	5c          	incb
    8bb6:	d1 f0       	cmpb	*0xf0
    8bb8:	22 03       	bhi	0x0x8bbd
    8bba:	8f          	xgdx
    8bbb:	20 f3       	bra	0x0x8bb0
    8bbd:	c1 07       	cmpb	#0x7
    8bbf:	24 0e       	bcc	0x0x8bcf
    8bc1:	ce 01 00    	ldx	#0x100
    8bc4:	3a          	abx
    8bc5:	e6 00       	ldab	0x0,x
    8bc7:	27 0b       	beq	0x0x8bd4
    8bc9:	08          	inx
    8bca:	8c 01 08    	cpx	#0x108
    8bcd:	25 f6       	bcs	0x0x8bc5
    8bcf:	32          	pula
    8bd0:	33          	pulb
    8bd1:	7e 8c 56    	jmp	0x8c56
    8bd4:	32          	pula
    8bd5:	a7 00       	staa	0x0,x
    8bd7:	33          	pulb
    8bd8:	7e 8c 56    	jmp	0x8c56
    8bdb:	33          	pulb
    8bdc:	36          	psha
    8bdd:	37          	pshb
    8bde:	fe 01 08    	ldx	0x108
    8be1:	8f          	xgdx
    8be2:	5c          	incb
    8be3:	d1 f0       	cmpb	*0xf0
    8be5:	23 01       	bls	0x0x8be8
    8be7:	5f          	clrb
    8be8:	8f          	xgdx
    8be9:	d6 f0       	ldab	*0xf0
    8beb:	cb 02       	addb	#0x2
    8bed:	6d 00       	tst	0x0,x
    8bef:	27 10       	beq	0x0x8c01
    8bf1:	8f          	xgdx
    8bf2:	5c          	incb
    8bf3:	d1 f0       	cmpb	*0xf0
    8bf5:	23 01       	bls	0x0x8bf8
    8bf7:	5f          	clrb
    8bf8:	8f          	xgdx
    8bf9:	5a          	decb
    8bfa:	26 f1       	bne	0x0x8bed
    8bfc:	d6 f0       	ldab	*0xf0
    8bfe:	5c          	incb
    8bff:	20 bc       	bra	0x0x8bbd
    8c01:	ff 01 08    	stx	0x108
    8c04:	8f          	xgdx
    8c05:	86 01       	ldaa	#0x1
    8c07:	5d          	tstb
    8c08:	27 04       	beq	0x0x8c0e
    8c0a:	5a          	decb
    8c0b:	48          	asla
    8c0c:	20 f9       	bra	0x0x8c07
    8c0e:	16          	tab
    8c0f:	9a f8       	oraa	*0xf8
    8c11:	97 f8       	staa	*0xf8
    8c13:	53          	comb
    8c14:	fe 01 08    	ldx	0x108
    8c17:	7d 00 d0    	tst	0xd0
    8c1a:	27 05       	beq	0x0x8c21
    8c1c:	7f 00 d0    	clr	0xd0
    8c1f:	20 08       	bra	0x0x8c29
    8c21:	7d 10 29    	tst	0x1029
    8c24:	2a fb       	bpl	0x0x8c21
    8c26:	b6 10 2a    	ldaa	0x102a
    8c29:	01          	nop
    8c2a:	01          	nop
    8c2b:	01          	nop
    8c2c:	01          	nop
    8c2d:	f7 10 42    	stab	0x1042
    8c30:	01          	nop
    8c31:	01          	nop
    8c32:	01          	nop
    8c33:	01          	nop
    8c34:	01          	nop
    8c35:	01          	nop
    8c36:	01          	nop
    8c37:	86 81       	ldaa	#0x81
    8c39:	b7 10 2a    	staa	0x102a
    8c3c:	7d 10 29    	tst	0x1029
    8c3f:	2a fb       	bpl	0x0x8c3c
    8c41:	b6 10 2a    	ldaa	0x102a
    8c44:	32          	pula
    8c45:	a7 00       	staa	0x0,x
    8c47:	b7 10 2a    	staa	0x102a
    8c4a:	7d 10 29    	tst	0x1029
    8c4d:	2a fb       	bpl	0x0x8c4a
    8c4f:	b6 10 2a    	ldaa	0x102a
    8c52:	32          	pula
    8c53:	b7 10 2a    	staa	0x102a
    8c56:	7f 00 fc    	clr	0xfc
    8c59:	bd 91 bc    	jsr	0x91bc
    8c5c:	4d          	tsta
    8c5d:	2a 03       	bpl	0x0x8c62
    8c5f:	7e 82 96    	jmp	0x8296
    8c62:	7c 00 fc    	inc	0xfc
    8c65:	c6 80       	ldab	#0x80
    8c67:	d1 df       	cmpb	*0xdf
    8c69:	27 0a       	beq	0x0x8c75
    8c6b:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8c72
    8c6f:	7e 8b 9a    	jmp	0x8b9a
    8c72:	7e 82 ec    	jmp	0x82ec
    8c75:	13 6a 20 03 	brclr	*0x6a, #0x20, 0x0x8c7c
    8c79:	7e 8a b9    	jmp	0x8ab9
    8c7c:	36          	psha
    8c7d:	bd 91 bc    	jsr	0x91bc
    8c80:	32          	pula
    8c81:	20 08       	bra	0x0x8c8b
    8c83:	bd 91 bc    	jsr	0x91bc
    8c86:	36          	psha
    8c87:	bd 91 bc    	jsr	0x91bc
    8c8a:	32          	pula
    8c8b:	ce 01 00    	ldx	#0x100
    8c8e:	c6 01       	ldab	#0x1
    8c90:	a1 00       	cmpa	0x0,x
    8c92:	27 07       	beq	0x0x8c9b
    8c94:	08          	inx
    8c95:	58          	aslb
    8c96:	24 f8       	bcc	0x0x8c90
    8c98:	7e 8d 26    	jmp	0x8d26
    8c9b:	7d 00 f1    	tst	0xf1
    8c9e:	27 22       	beq	0x0x8cc2
    8ca0:	3c          	pshx
    8ca1:	37          	pshb
    8ca2:	36          	psha
    8ca3:	8f          	xgdx
    8ca4:	86 01       	ldaa	#0x1
    8ca6:	5a          	decb
    8ca7:	2b 03       	bmi	0x0x8cac
    8ca9:	48          	asla
    8caa:	20 fa       	bra	0x0x8ca6
    8cac:	36          	psha
    8cad:	94 fd       	anda	*0xfd
    8caf:	27 06       	beq	0x0x8cb7
    8cb1:	32          	pula
    8cb2:	32          	pula
    8cb3:	33          	pulb
    8cb4:	38          	pulx
    8cb5:	20 dd       	bra	0x0x8c94
    8cb7:	32          	pula
    8cb8:	9a fd       	oraa	*0xfd
    8cba:	97 fd       	staa	*0xfd
    8cbc:	32          	pula
    8cbd:	33          	pulb
    8cbe:	38          	pulx
    8cbf:	7e 8d 26    	jmp	0x8d26
    8cc2:	6f 00       	clr	0x0,x
    8cc4:	96 f0       	ldaa	*0xf0
    8cc6:	81 07       	cmpa	#0x7
    8cc8:	27 31       	beq	0x0x8cfb
    8cca:	8f          	xgdx
    8ccb:	d1 f0       	cmpb	*0xf0
    8ccd:	23 03       	bls	0x0x8cd2
    8ccf:	7e 8d 26    	jmp	0x8d26
    8cd2:	8f          	xgdx
    8cd3:	37          	pshb
    8cd4:	18 ce 01 00 	ldy	#0x100
    8cd8:	d6 f0       	ldab	*0xf0
    8cda:	5c          	incb
    8cdb:	18 3a       	aby
    8cdd:	18 a6 00    	ldaa	0x0,y
    8ce0:	26 0b       	bne	0x0x8ced
    8ce2:	18 08       	iny
    8ce4:	18 8c 01 08 	cpy	#0x108
    8ce8:	25 f3       	bcs	0x0x8cdd
    8cea:	33          	pulb
    8ceb:	20 0e       	bra	0x0x8cfb
    8ced:	33          	pulb
    8cee:	53          	comb
    8cef:	4f          	clra
    8cf0:	36          	psha
    8cf1:	18 a6 00    	ldaa	0x0,y
    8cf4:	36          	psha
    8cf5:	18 6f 00    	clr	0x0,y
    8cf8:	7e 8c 17    	jmp	0x8c17
    8cfb:	17          	tba
    8cfc:	98 f8       	eora	*0xf8
    8cfe:	97 f8       	staa	*0xf8
    8d00:	53          	comb
    8d01:	7d 00 d0    	tst	0xd0
    8d04:	27 05       	beq	0x0x8d0b
    8d06:	7f 00 d0    	clr	0xd0
    8d09:	20 08       	bra	0x0x8d13
    8d0b:	7d 10 29    	tst	0x1029
    8d0e:	2a fb       	bpl	0x0x8d0b
    8d10:	b6 10 2a    	ldaa	0x102a
    8d13:	01          	nop
    8d14:	01          	nop
    8d15:	01          	nop
    8d16:	01          	nop
    8d17:	f7 10 42    	stab	0x1042
    8d1a:	01          	nop
    8d1b:	01          	nop
    8d1c:	01          	nop
    8d1d:	01          	nop
    8d1e:	01          	nop
    8d1f:	01          	nop
    8d20:	01          	nop
    8d21:	86 80       	ldaa	#0x80
    8d23:	b7 10 2a    	staa	0x102a
    8d26:	7f 00 fc    	clr	0xfc
    8d29:	bd 91 bc    	jsr	0x91bc
    8d2c:	4d          	tsta
    8d2d:	2a 03       	bpl	0x0x8d32
    8d2f:	7e 82 96    	jmp	0x8296
    8d32:	7c 00 fc    	inc	0xfc
    8d35:	c6 80       	ldab	#0x80
    8d37:	d1 df       	cmpb	*0xdf
    8d39:	27 0a       	beq	0x0x8d45
    8d3b:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x8d42
    8d3f:	7e 8b 9a    	jmp	0x8b9a
    8d42:	7e 82 ec    	jmp	0x82ec
    8d45:	13 6a 20 03 	brclr	*0x6a, #0x20, 0x0x8d4c
    8d49:	7e 8a b9    	jmp	0x8ab9
    8d4c:	36          	psha
    8d4d:	bd 91 bc    	jsr	0x91bc
    8d50:	32          	pula
    8d51:	7e 8c 8b    	jmp	0x8c8b
    8d54:	97 df       	staa	*0xdf
    8d56:	7c 00 fc    	inc	0xfc
    8d59:	bd 91 bc    	jsr	0x91bc
    8d5c:	81 00       	cmpa	#0x0
    8d5e:	26 09       	bne	0x0x8d69
    8d60:	7c 00 d8    	inc	0xd8
    8d63:	bd 91 bc    	jsr	0x91bc
    8d66:	7e 8d d8    	jmp	0x8dd8
    8d69:	81 20       	cmpa	#0x20
    8d6b:	26 25       	bne	0x0x8d92
    8d6d:	bd 91 bc    	jsr	0x91bc
    8d70:	7d 00 d8    	tst	0xd8
    8d73:	27 1a       	beq	0x0x8d8f
    8d75:	7f 00 d8    	clr	0xd8
    8d78:	d6 df       	ldab	*0xdf
    8d7a:	c4 0f       	andb	#0xf
    8d7c:	26 11       	bne	0x0x8d8f
    8d7e:	81 05       	cmpa	#0x5
    8d80:	25 02       	bcs	0x0x8d84
    8d82:	86 04       	ldaa	#0x4
    8d84:	97 f9       	staa	*0xf9
    8d86:	bd ab 4b    	jsr	0xab4b
    8d89:	b7 7f f6    	staa	0x7ff6
    8d8c:	bd ab 71    	jsr	0xab71
    8d8f:	7e 8d d8    	jmp	0x8dd8
    8d92:	7f 00 d8    	clr	0xd8
    8d95:	81 7b       	cmpa	#0x7b
    8d97:	26 09       	bne	0x0x8da2
    8d99:	bd 9c b5    	jsr	0x9cb5
    8d9c:	bd 91 bc    	jsr	0x91bc
    8d9f:	7e 8d d8    	jmp	0x8dd8
    8da2:	36          	psha
    8da3:	bd 91 bc    	jsr	0x91bc
    8da6:	33          	pulb
    8da7:	c1 01       	cmpb	#0x1
    8da9:	26 12       	bne	0x0x8dbd
    8dab:	13 8e 01 0c 	brclr	*0x8e, #0x01, 0x0x8dbb
    8daf:	36          	psha
    8db0:	37          	pshb
    8db1:	16          	tab
    8db2:	4f          	clra
    8db3:	8f          	xgdx
    8db4:	86 87       	ldaa	#0x87
    8db6:	bd 91 26    	jsr	0x9126
    8db9:	33          	pulb
    8dba:	32          	pula
    8dbb:	20 15       	bra	0x0x8dd2
    8dbd:	c1 02       	cmpb	#0x2
    8dbf:	26 29       	bne	0x0x8dea
    8dc1:	7d 00 8e    	tst	0x8e
    8dc4:	27 02       	beq	0x0x8dc8
    8dc6:	20 10       	bra	0x0x8dd8
    8dc8:	16          	tab
    8dc9:	4f          	clra
    8dca:	8f          	xgdx
    8dcb:	86 87       	ldaa	#0x87
    8dcd:	bd 91 26    	jsr	0x9126
    8dd0:	20 06       	bra	0x0x8dd8
    8dd2:	8f          	xgdx
    8dd3:	86 85       	ldaa	#0x85
    8dd5:	bd 91 26    	jsr	0x9126
    8dd8:	7f 00 fc    	clr	0xfc
    8ddb:	bd 91 bc    	jsr	0x91bc
    8dde:	4d          	tsta
    8ddf:	2a 03       	bpl	0x0x8de4
    8de1:	7e 82 96    	jmp	0x8296
    8de4:	7c 00 fc    	inc	0xfc
    8de7:	7e 8d 5c    	jmp	0x8d5c
    8dea:	c1 05       	cmpb	#0x5
    8dec:	27 e4       	beq	0x0x8dd2
    8dee:	c1 07       	cmpb	#0x7
    8df0:	26 08       	bne	0x0x8dfa
    8df2:	37          	pshb
    8df3:	d6 d6       	ldab	*0xd6
    8df5:	3d          	mul
    8df6:	05          	asld
    8df7:	33          	pulb
    8df8:	20 d8       	bra	0x0x8dd2
    8dfa:	c1 40       	cmpb	#0x40
    8dfc:	27 d4       	beq	0x0x8dd2
    8dfe:	c1 41       	cmpb	#0x41
    8e00:	27 d0       	beq	0x0x8dd2
    8e02:	c1 36       	cmpb	#0x36
    8e04:	25 0f       	bcs	0x0x8e15
    8e06:	c1 76       	cmpb	#0x76
    8e08:	22 0b       	bhi	0x0x8e15
    8e0a:	ce 92 10    	ldx	#0x9210
    8e0d:	c0 36       	subb	#0x36
    8e0f:	58          	aslb
    8e10:	3a          	abx
    8e11:	ee 00       	ldx	0x0,x
    8e13:	ad 00       	jsr	0x0,x
    8e15:	7e 8d d8    	jmp	0x8dd8
    8e18:	97 df       	staa	*0xdf
    8e1a:	7c 00 fc    	inc	0xfc
    8e1d:	bd 91 bc    	jsr	0x91bc
    8e20:	36          	psha
    8e21:	bd 91 bc    	jsr	0x91bc
    8e24:	16          	tab
    8e25:	32          	pula
    8e26:	8f          	xgdx
    8e27:	86 84       	ldaa	#0x84
    8e29:	bd 91 26    	jsr	0x9126
    8e2c:	7f 00 fc    	clr	0xfc
    8e2f:	bd 91 bc    	jsr	0x91bc
    8e32:	4d          	tsta
    8e33:	2a 03       	bpl	0x0x8e38
    8e35:	7e 82 96    	jmp	0x8296
    8e38:	7c 00 fc    	inc	0xfc
    8e3b:	20 e3       	bra	0x0x8e20
    8e3d:	97 df       	staa	*0xdf
    8e3f:	7c 00 fc    	inc	0xfc
    8e42:	bd 91 bc    	jsr	0x91bc
    8e45:	36          	psha
    8e46:	13 8e 02 08 	brclr	*0x8e, #0x02, 0x0x8e52
    8e4a:	16          	tab
    8e4b:	4f          	clra
    8e4c:	8f          	xgdx
    8e4d:	86 87       	ldaa	#0x87
    8e4f:	bd 91 26    	jsr	0x9126
    8e52:	33          	pulb
    8e53:	4f          	clra
    8e54:	8f          	xgdx
    8e55:	86 86       	ldaa	#0x86
    8e57:	bd 91 26    	jsr	0x9126
    8e5a:	7f 00 fc    	clr	0xfc
    8e5d:	bd 91 bc    	jsr	0x91bc
    8e60:	4d          	tsta
    8e61:	2a 03       	bpl	0x0x8e66
    8e63:	7e 82 96    	jmp	0x8296
    8e66:	7c 00 fc    	inc	0xfc
    8e69:	7e 8e 45    	jmp	0x8e45
    8e6c:	97 df       	staa	*0xdf
    8e6e:	7c 00 fc    	inc	0xfc
    8e71:	bd 91 bc    	jsr	0x91bc
    8e74:	7c 00 fc    	inc	0xfc
    8e77:	7d 00 fe    	tst	0xfe
    8e7a:	27 02       	beq	0x0x8e7e
    8e7c:	20 45       	bra	0x0x8ec3
    8e7e:	d6 f9       	ldab	*0xf9
    8e80:	c1 04       	cmpb	#0x4
    8e82:	26 0d       	bne	0x0x8e91
    8e84:	d6 df       	ldab	*0xdf
    8e86:	c4 0f       	andb	#0xf
    8e88:	26 39       	bne	0x0x8ec3
    8e8a:	d6 f9       	ldab	*0xf9
    8e8c:	b7 01 6a    	staa	0x16a
    8e8f:	20 02       	bra	0x0x8e93
    8e91:	97 fa       	staa	*0xfa
    8e93:	bd ab 4b    	jsr	0xab4b
    8e96:	b7 7f f7    	staa	0x7ff7
    8e99:	bd ab 71    	jsr	0xab71
    8e9c:	4f          	clra
    8e9d:	97 fb       	staa	*0xfb
    8e9f:	97 f2       	staa	*0xf2
    8ea1:	97 ff       	staa	*0xff
    8ea3:	b7 01 1b    	staa	0x11b
    8ea6:	bd a0 ff    	jsr	0xa0ff
    8ea9:	7c 01 7e    	inc	0x17e
    8eac:	c1 04       	cmpb	#0x4
    8eae:	26 0d       	bne	0x0x8ebd
    8eb0:	bd 9c b5    	jsr	0x9cb5
    8eb3:	bd a3 15    	jsr	0xa315
    8eb6:	b6 50 00    	ldaa	0x5000
    8eb9:	97 d1       	staa	*0xd1
    8ebb:	20 06       	bra	0x0x8ec3
    8ebd:	bd a1 16    	jsr	0xa116
    8ec0:	14 d1 80    	bset	*0xd1, #0x80
    8ec3:	7f 00 fc    	clr	0xfc
    8ec6:	bd 91 bc    	jsr	0x91bc
    8ec9:	4d          	tsta
    8eca:	2a a8       	bpl	0x0x8e74
    8ecc:	7e 82 96    	jmp	0x8296
    8ecf:	97 df       	staa	*0xdf
    8ed1:	7c 00 fc    	inc	0xfc
    8ed4:	bd 91 bc    	jsr	0x91bc
    8ed7:	81 00       	cmpa	#0x0
    8ed9:	26 3b       	bne	0x0x8f16
    8edb:	bd 91 bc    	jsr	0x91bc
    8ede:	81 00       	cmpa	#0x0
    8ee0:	26 34       	bne	0x0x8f16
    8ee2:	bd 91 bc    	jsr	0x91bc
    8ee5:	81 4d       	cmpa	#0x4d
    8ee7:	26 2d       	bne	0x0x8f16
    8ee9:	bd 91 bc    	jsr	0x91bc
    8eec:	81 08       	cmpa	#0x8
    8eee:	26 26       	bne	0x0x8f16
    8ef0:	bd 91 bc    	jsr	0x91bc
    8ef3:	4d          	tsta
    8ef4:	27 04       	beq	0x0x8efa
    8ef6:	81 55       	cmpa	#0x55
    8ef8:	26 1c       	bne	0x0x8f16
    8efa:	bd 91 bc    	jsr	0x91bc
    8efd:	4d          	tsta
    8efe:	27 07       	beq	0x0x8f07
    8f00:	81 2a       	cmpa	#0x2a
    8f02:	26 12       	bne	0x0x8f16
    8f04:	7e a7 32    	jmp	0xa732
    8f07:	bd 91 bc    	jsr	0x91bc
    8f0a:	81 7f       	cmpa	#0x7f
    8f0c:	27 05       	beq	0x0x8f13
    8f0e:	bd 91 bc    	jsr	0x91bc
    8f11:	20 0c       	bra	0x0x8f1f
    8f13:	7e 8f 95    	jmp	0x8f95
    8f16:	7f 00 fc    	clr	0xfc
    8f19:	7f 00 ff    	clr	0xff
    8f1c:	7e 82 93    	jmp	0x8293
    8f1f:	4d          	tsta
    8f20:	27 36       	beq	0x0x8f58
    8f22:	81 01       	cmpa	#0x1
    8f24:	27 1a       	beq	0x0x8f40
    8f26:	ce 00 00    	ldx	#0x0
    8f29:	3c          	pshx
    8f2a:	bd 91 bc    	jsr	0x91bc
    8f2d:	38          	pulx
    8f2e:	81 f7       	cmpa	#0xf7
    8f30:	27 05       	beq	0x0x8f37
    8f32:	a7 00       	staa	0x0,x
    8f34:	08          	inx
    8f35:	20 f2       	bra	0x0x8f29
    8f37:	bd a1 57    	jsr	0xa157
    8f3a:	7f 00 fc    	clr	0xfc
    8f3d:	7e 82 93    	jmp	0x8293
    8f40:	96 f9       	ldaa	*0xf9
    8f42:	81 04       	cmpa	#0x4
    8f44:	26 07       	bne	0x0x8f4d
    8f46:	b6 01 6a    	ldaa	0x16a
    8f49:	c6 50       	ldab	#0x50
    8f4b:	20 04       	bra	0x0x8f51
    8f4d:	96 fa       	ldaa	*0xfa
    8f4f:	c6 b0       	ldab	#0xb0
    8f51:	3d          	mul
    8f52:	c3 20 00    	addd	#0x2000
    8f55:	8f          	xgdx
    8f56:	20 03       	bra	0x0x8f5b
    8f58:	ce 20 00    	ldx	#0x2000
    8f5b:	3c          	pshx
    8f5c:	bd 91 bc    	jsr	0x91bc
    8f5f:	38          	pulx
    8f60:	81 f7       	cmpa	#0xf7
    8f62:	27 05       	beq	0x0x8f69
    8f64:	a7 00       	staa	0x0,x
    8f66:	08          	inx
    8f67:	20 f2       	bra	0x0x8f5b
    8f69:	4f          	clra
    8f6a:	97 fb       	staa	*0xfb
    8f6c:	97 f2       	staa	*0xf2
    8f6e:	97 ff       	staa	*0xff
    8f70:	b7 01 1b    	staa	0x11b
    8f73:	bd a0 ff    	jsr	0xa0ff
    8f76:	96 f9       	ldaa	*0xf9
    8f78:	81 04       	cmpa	#0x4
    8f7a:	26 0a       	bne	0x0x8f86
    8f7c:	bd a3 15    	jsr	0xa315
    8f7f:	b6 50 00    	ldaa	0x5000
    8f82:	97 d1       	staa	*0xd1
    8f84:	20 06       	bra	0x0x8f8c
    8f86:	bd a1 16    	jsr	0xa116
    8f89:	14 d1 80    	bset	*0xd1, #0x80
    8f8c:	7c 01 7e    	inc	0x17e
    8f8f:	7f 00 fc    	clr	0xfc
    8f92:	7e 82 93    	jmp	0x8293
    8f95:	bd 91 bc    	jsr	0x91bc
    8f98:	8d 1a       	bsr	0x0x8fb4
    8f9a:	7f 00 ff    	clr	0xff
    8f9d:	7f 01 1b    	clr	0x11b
    8fa0:	bd a0 ff    	jsr	0xa0ff
    8fa3:	96 f2       	ldaa	*0xf2
    8fa5:	84 20       	anda	#0x20
    8fa7:	97 f2       	staa	*0xf2
    8fa9:	86 80       	ldaa	#0x80
    8fab:	b7 01 1c    	staa	0x11c
    8fae:	7f 00 fc    	clr	0xfc
    8fb1:	7e 82 93    	jmp	0x8293
    8fb4:	36          	psha
    8fb5:	bd 91 16    	jsr	0x9116
    8fb8:	86 f0       	ldaa	#0xf0
    8fba:	b7 10 2f    	staa	0x102f
    8fbd:	86 00       	ldaa	#0x0
    8fbf:	bd 91 16    	jsr	0x9116
    8fc2:	b7 10 2f    	staa	0x102f
    8fc5:	bd 91 16    	jsr	0x9116
    8fc8:	b7 10 2f    	staa	0x102f
    8fcb:	86 4d       	ldaa	#0x4d
    8fcd:	bd 91 16    	jsr	0x9116
    8fd0:	b7 10 2f    	staa	0x102f
    8fd3:	86 08       	ldaa	#0x8
    8fd5:	bd 91 16    	jsr	0x9116
    8fd8:	b7 10 2f    	staa	0x102f
    8fdb:	bd 91 16    	jsr	0x9116
    8fde:	4f          	clra
    8fdf:	b7 10 2f    	staa	0x102f
    8fe2:	bd 91 16    	jsr	0x9116
    8fe5:	b7 10 2f    	staa	0x102f
    8fe8:	bd 91 16    	jsr	0x9116
    8feb:	b7 10 2f    	staa	0x102f
    8fee:	bd 91 16    	jsr	0x9116
    8ff1:	32          	pula
    8ff2:	b7 10 2f    	staa	0x102f
    8ff5:	27 6c       	beq	0x0x9063
    8ff7:	81 01       	cmpa	#0x1
    8ff9:	27 10       	beq	0x0x900b
    8ffb:	ce 00 20    	ldx	#0x20
    8ffe:	c6 b0       	ldab	#0xb0
    9000:	8d 4f       	bsr	0x0x9051
    9002:	bd 91 16    	jsr	0x9116
    9005:	86 f7       	ldaa	#0xf7
    9007:	b7 10 2f    	staa	0x102f
    900a:	39          	rts
    900b:	96 fa       	ldaa	*0xfa
    900d:	d6 f9       	ldab	*0xf9
    900f:	c1 04       	cmpb	#0x4
    9011:	26 03       	bne	0x0x9016
    9013:	b6 01 6a    	ldaa	0x16a
    9016:	f6 01 1b    	ldab	0x11b
    9019:	c1 05       	cmpb	#0x5
    901b:	26 07       	bne	0x0x9024
    901d:	ce 01 3c    	ldx	#0x13c
    9020:	bd ea 7a    	jsr	0xea7a
    9023:	4a          	deca
    9024:	d6 f9       	ldab	*0xf9
    9026:	c1 01       	cmpb	#0x1
    9028:	22 03       	bhi	0x0x902d
    902a:	7e 90 e7    	jmp	0x90e7
    902d:	c1 04       	cmpb	#0x4
    902f:	26 04       	bne	0x0x9035
    9031:	c6 50       	ldab	#0x50
    9033:	20 02       	bra	0x0x9037
    9035:	c6 b0       	ldab	#0xb0
    9037:	3d          	mul
    9038:	c3 20 00    	addd	#0x2000
    903b:	8f          	xgdx
    903c:	c6 b0       	ldab	#0xb0
    903e:	96 f9       	ldaa	*0xf9
    9040:	81 04       	cmpa	#0x4
    9042:	26 02       	bne	0x0x9046
    9044:	c6 50       	ldab	#0x50
    9046:	8d 09       	bsr	0x0x9051
    9048:	bd 91 16    	jsr	0x9116
    904b:	86 f7       	ldaa	#0xf7
    904d:	b7 10 2f    	staa	0x102f
    9050:	39          	rts
    9051:	a6 00       	ldaa	0x0,x
    9053:	84 7f       	anda	#0x7f
    9055:	bd 91 16    	jsr	0x9116
    9058:	bd 91 1e    	jsr	0x911e
    905b:	b7 10 2f    	staa	0x102f
    905e:	08          	inx
    905f:	5a          	decb
    9060:	26 ef       	bne	0x0x9051
    9062:	39          	rts
    9063:	86 ff       	ldaa	#0xff
    9065:	97 f8       	staa	*0xf8
    9067:	96 f9       	ldaa	*0xf9
    9069:	81 01       	cmpa	#0x1
    906b:	22 06       	bhi	0x0x9073
    906d:	7f 01 1f    	clr	0x11f
    9070:	7e 90 9d    	jmp	0x909d
    9073:	ce 20 00    	ldx	#0x2000
    9076:	18 ce 00 10 	ldy	#0x10
    907a:	c6 b0       	ldab	#0xb0
    907c:	96 f9       	ldaa	*0xf9
    907e:	81 04       	cmpa	#0x4
    9080:	26 02       	bne	0x0x9084
    9082:	c6 50       	ldab	#0x50
    9084:	8d cb       	bsr	0x0x9051
    9086:	18 09       	dey
    9088:	26 f0       	bne	0x0x907a
    908a:	96 f8       	ldaa	*0xf8
    908c:	44          	lsra
    908d:	27 04       	beq	0x0x9093
    908f:	97 f8       	staa	*0xf8
    9091:	20 e3       	bra	0x0x9076
    9093:	97 f8       	staa	*0xf8
    9095:	8d 7f       	bsr	0x0x9116
    9097:	86 f7       	ldaa	#0xf7
    9099:	b7 10 2f    	staa	0x102f
    909c:	39          	rts
    909d:	18 ce 00 10 	ldy	#0x10
    90a1:	18 3c       	pshy
    90a3:	5f          	clrb
    90a4:	f7 10 22    	stab	0x1022
    90a7:	f6 10 2d    	ldab	0x102d
    90aa:	37          	pshb
    90ab:	c4 7f       	andb	#0x7f
    90ad:	f7 10 2d    	stab	0x102d
    90b0:	96 f9       	ldaa	*0xf9
    90b2:	f6 01 1f    	ldab	0x11f
    90b5:	bd 79 00    	jsr	0x7900
    90b8:	32          	pula
    90b9:	b7 10 2d    	staa	0x102d
    90bc:	86 80       	ldaa	#0x80
    90be:	b7 10 22    	staa	0x1022
    90c1:	ce 00 20    	ldx	#0x20
    90c4:	c6 b0       	ldab	#0xb0
    90c6:	8d 89       	bsr	0x0x9051
    90c8:	7c 01 1f    	inc	0x11f
    90cb:	18 38       	puly
    90cd:	18 09       	dey
    90cf:	26 d0       	bne	0x0x90a1
    90d1:	96 f8       	ldaa	*0xf8
    90d3:	44          	lsra
    90d4:	27 04       	beq	0x0x90da
    90d6:	97 f8       	staa	*0xf8
    90d8:	20 c3       	bra	0x0x909d
    90da:	97 f8       	staa	*0xf8
    90dc:	bd 91 16    	jsr	0x9116
    90df:	86 f7       	ldaa	#0xf7
    90e1:	b7 10 2f    	staa	0x102f
    90e4:	7e a1 16    	jmp	0xa116
    90e7:	5f          	clrb
    90e8:	f7 10 22    	stab	0x1022
    90eb:	f6 10 2d    	ldab	0x102d
    90ee:	37          	pshb
    90ef:	c4 7f       	andb	#0x7f
    90f1:	f7 10 2d    	stab	0x102d
    90f4:	16          	tab
    90f5:	96 f9       	ldaa	*0xf9
    90f7:	bd 79 00    	jsr	0x7900
    90fa:	32          	pula
    90fb:	b7 10 2d    	staa	0x102d
    90fe:	86 80       	ldaa	#0x80
    9100:	b7 10 22    	staa	0x1022
    9103:	ce 00 20    	ldx	#0x20
    9106:	c6 b0       	ldab	#0xb0
    9108:	bd 90 51    	jsr	0x9051
    910b:	bd 91 16    	jsr	0x9116
    910e:	86 f7       	ldaa	#0xf7
    9110:	b7 10 2f    	staa	0x102f
    9113:	7e a1 16    	jmp	0xa116
    9116:	37          	pshb
    9117:	f6 10 2e    	ldab	0x102e
    911a:	2a fb       	bpl	0x0x9117
    911c:	33          	pulb
    911d:	39          	rts
    911e:	37          	pshb
    911f:	c6 4d       	ldab	#0x4d
    9121:	5a          	decb
    9122:	26 fd       	bne	0x0x9121
    9124:	33          	pulb
    9125:	39          	rts
    9126:	7d 00 d0    	tst	0xd0
    9129:	27 05       	beq	0x0x9130
    912b:	7f 00 d0    	clr	0xd0
    912e:	20 08       	bra	0x0x9138
    9130:	7d 10 29    	tst	0x1029
    9133:	2a fb       	bpl	0x0x9130
    9135:	f6 10 2a    	ldab	0x102a
    9138:	d6 d1       	ldab	*0xd1
    913a:	2b 2f       	bmi	0x0x916b
    913c:	27 2d       	beq	0x0x916b
    913e:	c1 05       	cmpb	#0x5
    9140:	27 1d       	beq	0x0x915f
    9142:	22 15       	bhi	0x0x9159
    9144:	f6 01 6b    	ldab	0x16b
    9147:	18 ce 50 50 	ldy	#0x5050
    914b:	18 3a       	aby
    914d:	18 e6 00    	ldab	0x0,y
    9150:	26 04       	bne	0x0x9156
    9152:	7c 00 d0    	inc	0xd0
    9155:	39          	rts
    9156:	53          	comb
    9157:	20 13       	bra	0x0x916c
    9159:	d6 df       	ldab	*0xdf
    915b:	c4 0f       	andb	#0xf
    915d:	20 e8       	bra	0x0x9147
    915f:	8f          	xgdx
    9160:	c1 07       	cmpb	#0x7
    9162:	26 06       	bne	0x0x916a
    9164:	8f          	xgdx
    9165:	f6 01 6b    	ldab	0x16b
    9168:	20 dd       	bra	0x0x9147
    916a:	8f          	xgdx
    916b:	5f          	clrb
    916c:	01          	nop
    916d:	01          	nop
    916e:	01          	nop
    916f:	01          	nop
    9170:	f7 10 42    	stab	0x1042
    9173:	01          	nop
    9174:	01          	nop
    9175:	01          	nop
    9176:	01          	nop
    9177:	01          	nop
    9178:	01          	nop
    9179:	01          	nop
    917a:	b7 10 2a    	staa	0x102a
    917d:	7d 10 29    	tst	0x1029
    9180:	2a fb       	bpl	0x0x917d
    9182:	b6 10 2a    	ldaa	0x102a
    9185:	8f          	xgdx
    9186:	f7 10 2a    	stab	0x102a
    9189:	37          	pshb
    918a:	7d 10 29    	tst	0x1029
    918d:	2a fb       	bpl	0x0x918a
    918f:	f6 10 2a    	ldab	0x102a
    9192:	b7 10 2a    	staa	0x102a
    9195:	33          	pulb
    9196:	12 d1 80 21 	brset	*0xd1, #0x80, 0x0x91bb
    919a:	c1 44       	cmpb	#0x44
    919c:	26 1d       	bne	0x0x91bb
    919e:	ce 50 03    	ldx	#0x5003
    91a1:	f6 01 6b    	ldab	0x16b
    91a4:	86 04       	ldaa	#0x4
    91a6:	3d          	mul
    91a7:	3a          	abx
    91a8:	a6 00       	ldaa	0x0,x
    91aa:	84 40       	anda	#0x40
    91ac:	26 0d       	bne	0x0x91bb
    91ae:	7d 10 29    	tst	0x1029
    91b1:	2a fb       	bpl	0x0x91ae
    91b3:	b6 10 2a    	ldaa	0x102a
    91b6:	86 8c       	ldaa	#0x8c
    91b8:	b7 10 2a    	staa	0x102a
    91bb:	39          	rts
    91bc:	0f          	sei
    91bd:	fe 01 c0    	ldx	0x1c0
    91c0:	bc 01 c2    	cpx	0x1c2
    91c3:	26 10       	bne	0x0x91d5
    91c5:	7d 00 fc    	tst	0xfc
    91c8:	27 03       	beq	0x0x91cd
    91ca:	0e          	cli
    91cb:	20 f0       	bra	0x0x91bd
    91cd:	38          	pulx
    91ce:	ff 01 6c    	stx	0x16c
    91d1:	0e          	cli
    91d2:	7e 95 68    	jmp	0x9568
    91d5:	0f          	sei
    91d6:	a6 00       	ldaa	0x0,x
    91d8:	8f          	xgdx
    91d9:	5c          	incb
    91da:	c4 bf       	andb	#0xbf
    91dc:	8f          	xgdx
    91dd:	ff 01 c0    	stx	0x1c0
    91e0:	0e          	cli
    91e1:	81 fa       	cmpa	#0xfa
    91e3:	27 05       	beq	0x0x91ea
    91e5:	81 f8       	cmpa	#0xf8
    91e7:	27 01       	beq	0x0x91ea
    91e9:	39          	rts
    91ea:	7d 00 d0    	tst	0xd0
    91ed:	27 05       	beq	0x0x91f4
    91ef:	7f 00 d0    	clr	0xd0
    91f2:	20 08       	bra	0x0x91fc
    91f4:	7d 10 29    	tst	0x1029
    91f7:	2a fb       	bpl	0x0x91f4
    91f9:	f6 10 2a    	ldab	0x102a
    91fc:	5f          	clrb
    91fd:	01          	nop
    91fe:	01          	nop
    91ff:	01          	nop
    9200:	01          	nop
    9201:	f7 10 42    	stab	0x1042
    9204:	01          	nop
    9205:	01          	nop
    9206:	01          	nop
    9207:	01          	nop
    9208:	01          	nop
    9209:	01          	nop
    920a:	01          	nop
    920b:	b7 10 2a    	staa	0x102a
    920e:	20 ac       	bra	0x0x91bc
    9210:	92 92       	sbca	*0x92
    9212:	92 99       	sbca	*0x99
    9214:	92 c2       	sbca	*0xc2
    9216:	92 eb       	sbca	*0xeb
    9218:	93 14       	subd	*0x14
    921a:	93 3d       	subd	*0x3d
    921c:	93 42       	subd	*0x42
    921e:	93 49       	subd	*0x49
    9220:	93 72       	subd	*0x72
    9222:	93 9b       	subd	*0x9b
    9224:	93 c4       	subd	*0xc4
    9226:	93 c4       	subd	*0xc4
    9228:	93 c4       	subd	*0xc4
    922a:	93 c4       	subd	*0xc4
    922c:	93 c4       	subd	*0xc4
    922e:	93 c4       	subd	*0xc4
    9230:	93 c5       	subd	*0xc5
    9232:	93 ca       	subd	*0xca
    9234:	93 cf       	subd	*0xcf
    9236:	93 d4       	subd	*0xd4
    9238:	93 d9       	subd	*0xd9
    923a:	93 de       	subd	*0xde
    923c:	93 ee       	subd	*0xee
    923e:	94 02       	anda	*0x2
    9240:	94 19       	anda	*0x19
    9242:	94 28       	anda	*0x28
    9244:	94 37       	anda	*0x37
    9246:	94 47       	anda	*0x47
    9248:	93 c4       	subd	*0xc4
    924a:	93 c4       	subd	*0xc4
    924c:	94 4e       	anda	*0x4e
    924e:	94 53       	anda	*0x53
    9250:	94 58       	anda	*0x58
    9252:	94 5d       	anda	*0x5d
    9254:	94 62       	anda	*0x62
    9256:	94 67       	anda	*0x67
    9258:	94 6c       	anda	*0x6c
    925a:	94 71       	anda	*0x71
    925c:	94 76       	anda	*0x76
    925e:	94 7b       	anda	*0x7b
    9260:	94 80       	anda	*0x80
    9262:	93 c4       	subd	*0xc4
    9264:	93 c4       	subd	*0xc4
    9266:	93 c4       	subd	*0xc4
    9268:	94 85       	anda	*0x85
    926a:	94 8a       	anda	*0x8a
    926c:	93 c4       	subd	*0xc4
    926e:	93 c4       	subd	*0xc4
    9270:	94 8f       	anda	*0x8f
    9272:	94 94       	anda	*0x94
    9274:	94 99       	anda	*0x99
    9276:	94 9e       	anda	*0x9e
    9278:	94 a3       	anda	*0xa3
    927a:	94 a8       	anda	*0xa8
    927c:	94 ad       	anda	*0xad
    927e:	94 b2       	anda	*0xb2
    9280:	94 b7       	anda	*0xb7
    9282:	94 bc       	anda	*0xbc
    9284:	94 c1       	anda	*0xc1
    9286:	94 c6       	anda	*0xc6
    9288:	94 cb       	anda	*0xcb
    928a:	94 d0       	anda	*0xd0
    928c:	94 d5       	anda	*0xd5
    928e:	94 f2       	anda	*0xf2
    9290:	94 f3       	anda	*0xf3
    9292:	84 3f       	anda	#0x3f
    9294:	c6 3e       	ldab	#0x3e
    9296:	7e 95 1c    	jmp	0x951c
    9299:	c6 44       	ldab	#0x44
    929b:	4d          	tsta
    929c:	27 12       	beq	0x0x92b0
    929e:	96 44       	ldaa	*0x44
    92a0:	8a 01       	oraa	#0x1
    92a2:	bd 95 4b    	jsr	0x954b
    92a5:	97 44       	staa	*0x44
    92a7:	14 f7 01    	bset	*0xf7, #0x01
    92aa:	8f          	xgdx
    92ab:	86 83       	ldaa	#0x83
    92ad:	7e 91 26    	jmp	0x9126
    92b0:	96 44       	ldaa	*0x44
    92b2:	84 fe       	anda	#0xfe
    92b4:	bd 95 4b    	jsr	0x954b
    92b7:	97 44       	staa	*0x44
    92b9:	15 f7 01    	bclr	*0xf7, #0x01
    92bc:	8f          	xgdx
    92bd:	86 83       	ldaa	#0x83
    92bf:	7e 91 26    	jmp	0x9126
    92c2:	c6 44       	ldab	#0x44
    92c4:	4d          	tsta
    92c5:	27 12       	beq	0x0x92d9
    92c7:	96 44       	ldaa	*0x44
    92c9:	8a 02       	oraa	#0x2
    92cb:	bd 95 4b    	jsr	0x954b
    92ce:	97 44       	staa	*0x44
    92d0:	14 f7 02    	bset	*0xf7, #0x02
    92d3:	8f          	xgdx
    92d4:	86 83       	ldaa	#0x83
    92d6:	7e 91 26    	jmp	0x9126
    92d9:	96 44       	ldaa	*0x44
    92db:	84 fd       	anda	#0xfd
    92dd:	bd 95 4b    	jsr	0x954b
    92e0:	97 44       	staa	*0x44
    92e2:	15 f7 02    	bclr	*0xf7, #0x02
    92e5:	8f          	xgdx
    92e6:	86 83       	ldaa	#0x83
    92e8:	7e 91 26    	jmp	0x9126
    92eb:	c6 44       	ldab	#0x44
    92ed:	4d          	tsta
    92ee:	27 12       	beq	0x0x9302
    92f0:	96 44       	ldaa	*0x44
    92f2:	8a 04       	oraa	#0x4
    92f4:	bd 95 4b    	jsr	0x954b
    92f7:	97 44       	staa	*0x44
    92f9:	14 f7 04    	bset	*0xf7, #0x04
    92fc:	8f          	xgdx
    92fd:	86 83       	ldaa	#0x83
    92ff:	7e 91 26    	jmp	0x9126
    9302:	96 44       	ldaa	*0x44
    9304:	84 fb       	anda	#0xfb
    9306:	bd 95 4b    	jsr	0x954b
    9309:	97 44       	staa	*0x44
    930b:	15 f7 04    	bclr	*0xf7, #0x04
    930e:	8f          	xgdx
    930f:	86 83       	ldaa	#0x83
    9311:	7e 91 26    	jmp	0x9126
    9314:	c6 44       	ldab	#0x44
    9316:	4d          	tsta
    9317:	27 12       	beq	0x0x932b
    9319:	96 44       	ldaa	*0x44
    931b:	8a 08       	oraa	#0x8
    931d:	bd 95 4b    	jsr	0x954b
    9320:	97 44       	staa	*0x44
    9322:	14 f7 08    	bset	*0xf7, #0x08
    9325:	8f          	xgdx
    9326:	86 83       	ldaa	#0x83
    9328:	7e 91 26    	jmp	0x9126
    932b:	96 44       	ldaa	*0x44
    932d:	84 f7       	anda	#0xf7
    932f:	bd 95 4b    	jsr	0x954b
    9332:	97 44       	staa	*0x44
    9334:	15 f7 08    	bclr	*0xf7, #0x08
    9337:	8f          	xgdx
    9338:	86 83       	ldaa	#0x83
    933a:	7e 91 26    	jmp	0x9126
    933d:	c6 42       	ldab	#0x42
    933f:	7e 95 1c    	jmp	0x951c
    9342:	84 3f       	anda	#0x3f
    9344:	c6 3f       	ldab	#0x3f
    9346:	7e 95 1c    	jmp	0x951c
    9349:	c6 44       	ldab	#0x44
    934b:	4d          	tsta
    934c:	27 12       	beq	0x0x9360
    934e:	96 44       	ldaa	*0x44
    9350:	8a 10       	oraa	#0x10
    9352:	bd 95 4b    	jsr	0x954b
    9355:	97 44       	staa	*0x44
    9357:	14 f7 10    	bset	*0xf7, #0x10
    935a:	8f          	xgdx
    935b:	86 83       	ldaa	#0x83
    935d:	7e 91 26    	jmp	0x9126
    9360:	96 44       	ldaa	*0x44
    9362:	84 ef       	anda	#0xef
    9364:	bd 95 4b    	jsr	0x954b
    9367:	97 44       	staa	*0x44
    9369:	15 f7 10    	bclr	*0xf7, #0x10
    936c:	8f          	xgdx
    936d:	86 83       	ldaa	#0x83
    936f:	7e 91 26    	jmp	0x9126
    9372:	c6 44       	ldab	#0x44
    9374:	4d          	tsta
    9375:	27 12       	beq	0x0x9389
    9377:	96 44       	ldaa	*0x44
    9379:	8a 20       	oraa	#0x20
    937b:	bd 95 4b    	jsr	0x954b
    937e:	97 44       	staa	*0x44
    9380:	14 f7 20    	bset	*0xf7, #0x20
    9383:	8f          	xgdx
    9384:	86 83       	ldaa	#0x83
    9386:	7e 91 26    	jmp	0x9126
    9389:	96 44       	ldaa	*0x44
    938b:	84 df       	anda	#0xdf
    938d:	bd 95 4b    	jsr	0x954b
    9390:	97 44       	staa	*0x44
    9392:	15 f7 20    	bclr	*0xf7, #0x20
    9395:	8f          	xgdx
    9396:	86 83       	ldaa	#0x83
    9398:	7e 91 26    	jmp	0x9126
    939b:	c6 44       	ldab	#0x44
    939d:	4d          	tsta
    939e:	27 12       	beq	0x0x93b2
    93a0:	96 44       	ldaa	*0x44
    93a2:	8a 40       	oraa	#0x40
    93a4:	bd 95 4b    	jsr	0x954b
    93a7:	97 44       	staa	*0x44
    93a9:	14 f7 40    	bset	*0xf7, #0x40
    93ac:	8f          	xgdx
    93ad:	86 83       	ldaa	#0x83
    93af:	7e 91 26    	jmp	0x9126
    93b2:	96 44       	ldaa	*0x44
    93b4:	84 bf       	anda	#0xbf
    93b6:	bd 95 4b    	jsr	0x954b
    93b9:	97 44       	staa	*0x44
    93bb:	15 f7 40    	bclr	*0xf7, #0x40
    93be:	8f          	xgdx
    93bf:	86 83       	ldaa	#0x83
    93c1:	7e 91 26    	jmp	0x9126
    93c4:	39          	rts
    93c5:	c6 43       	ldab	#0x43
    93c7:	7e 95 1c    	jmp	0x951c
    93ca:	c6 28       	ldab	#0x28
    93cc:	7e 95 1c    	jmp	0x951c
    93cf:	c6 2a       	ldab	#0x2a
    93d1:	7e 95 1c    	jmp	0x951c
    93d4:	c6 35       	ldab	#0x35
    93d6:	7e 95 1c    	jmp	0x951c
    93d9:	c6 37       	ldab	#0x37
    93db:	7e 95 1c    	jmp	0x951c
    93de:	c6 4c       	ldab	#0x4c
    93e0:	4d          	tsta
    93e1:	26 05       	bne	0x0x93e8
    93e3:	96 4c       	ldaa	*0x4c
    93e5:	7e af d2    	jmp	0xafd2
    93e8:	96 4c       	ldaa	*0x4c
    93ea:	44          	lsra
    93eb:	7e af d2    	jmp	0xafd2
    93ee:	c6 4f       	ldab	#0x4f
    93f0:	4d          	tsta
    93f1:	26 05       	bne	0x0x93f8
    93f3:	96 4f       	ldaa	*0x4f
    93f5:	7e af d2    	jmp	0xafd2
    93f8:	86 7f       	ldaa	#0x7f
    93fa:	90 4f       	suba	*0x4f
    93fc:	44          	lsra
    93fd:	9b 4f       	adda	*0x4f
    93ff:	7e af d2    	jmp	0xafd2
    9402:	c6 66       	ldab	#0x66
    9404:	4d          	tsta
    9405:	27 09       	beq	0x0x9410
    9407:	86 01       	ldaa	#0x1
    9409:	9a 66       	oraa	*0x66
    940b:	97 66       	staa	*0x66
    940d:	7e af d2    	jmp	0xafd2
    9410:	86 02       	ldaa	#0x2
    9412:	94 66       	anda	*0x66
    9414:	97 66       	staa	*0x66
    9416:	7e af d2    	jmp	0xafd2
    9419:	c6 4c       	ldab	#0x4c
    941b:	4d          	tsta
    941c:	27 05       	beq	0x0x9423
    941e:	96 4c       	ldaa	*0x4c
    9420:	7e af d2    	jmp	0xafd2
    9423:	86 64       	ldaa	#0x64
    9425:	7e af d2    	jmp	0xafd2
    9428:	c6 4d       	ldab	#0x4d
    942a:	4d          	tsta
    942b:	26 05       	bne	0x0x9432
    942d:	96 4d       	ldaa	*0x4d
    942f:	7e af d2    	jmp	0xafd2
    9432:	86 64       	ldaa	#0x64
    9434:	7e af d2    	jmp	0xafd2
    9437:	c6 52       	ldab	#0x52
    9439:	4d          	tsta
    943a:	26 05       	bne	0x0x9441
    943c:	96 52       	ldaa	*0x52
    943e:	7e af d2    	jmp	0xafd2
    9441:	96 52       	ldaa	*0x52
    9443:	44          	lsra
    9444:	7e af d2    	jmp	0xafd2
    9447:	97 50       	staa	*0x50
    9449:	c6 50       	ldab	#0x50
    944b:	7e af d2    	jmp	0xafd2
    944e:	c6 45       	ldab	#0x45
    9450:	7e 95 1c    	jmp	0x951c
    9453:	c6 46       	ldab	#0x46
    9455:	7e 95 1c    	jmp	0x951c
    9458:	c6 47       	ldab	#0x47
    945a:	7e 95 1c    	jmp	0x951c
    945d:	c6 49       	ldab	#0x49
    945f:	7e 95 1c    	jmp	0x951c
    9462:	c6 4c       	ldab	#0x4c
    9464:	7e 95 1c    	jmp	0x951c
    9467:	c6 4d       	ldab	#0x4d
    9469:	7e 95 1c    	jmp	0x951c
    946c:	c6 4e       	ldab	#0x4e
    946e:	7e 95 1c    	jmp	0x951c
    9471:	c6 6e       	ldab	#0x6e
    9473:	7e 95 1c    	jmp	0x951c
    9476:	c6 48       	ldab	#0x48
    9478:	7e 95 1c    	jmp	0x951c
    947b:	c6 6f       	ldab	#0x6f
    947d:	7e 95 1c    	jmp	0x951c
    9480:	c6 70       	ldab	#0x70
    9482:	7e 95 1c    	jmp	0x951c
    9485:	c6 48       	ldab	#0x48
    9487:	7e 95 1c    	jmp	0x951c
    948a:	c6 6f       	ldab	#0x6f
    948c:	7e 95 1c    	jmp	0x951c
    948f:	c6 4f       	ldab	#0x4f
    9491:	7e 95 1c    	jmp	0x951c
    9494:	c6 51       	ldab	#0x51
    9496:	7e 95 1c    	jmp	0x951c
    9499:	c6 52       	ldab	#0x52
    949b:	7e 95 1c    	jmp	0x951c
    949e:	c6 53       	ldab	#0x53
    94a0:	7e 95 1c    	jmp	0x951c
    94a3:	c6 55       	ldab	#0x55
    94a5:	7e 95 1c    	jmp	0x951c
    94a8:	c6 56       	ldab	#0x56
    94aa:	7e 95 1c    	jmp	0x951c
    94ad:	c6 57       	ldab	#0x57
    94af:	7e 95 1c    	jmp	0x951c
    94b2:	c6 58       	ldab	#0x58
    94b4:	7e 95 1c    	jmp	0x951c
    94b7:	c6 5a       	ldab	#0x5a
    94b9:	7e 95 1c    	jmp	0x951c
    94bc:	c6 63       	ldab	#0x63
    94be:	7e 95 1c    	jmp	0x951c
    94c1:	c6 5b       	ldab	#0x5b
    94c3:	7e 95 1c    	jmp	0x951c
    94c6:	c6 5c       	ldab	#0x5c
    94c8:	7e 95 1c    	jmp	0x951c
    94cb:	c6 5d       	ldab	#0x5d
    94cd:	7e 95 1c    	jmp	0x951c
    94d0:	c6 5f       	ldab	#0x5f
    94d2:	7e 95 1c    	jmp	0x951c
    94d5:	44          	lsra
    94d6:	44          	lsra
    94d7:	44          	lsra
    94d8:	91 71       	cmpa	*0x71
    94da:	26 01       	bne	0x0x94dd
    94dc:	39          	rts
    94dd:	97 71       	staa	*0x71
    94df:	ce ac 52    	ldx	#0xac52
    94e2:	d6 de       	ldab	*0xde
    94e4:	86 10       	ldaa	#0x10
    94e6:	3d          	mul
    94e7:	3a          	abx
    94e8:	d6 71       	ldab	*0x71
    94ea:	3a          	abx
    94eb:	a6 00       	ldaa	0x0,x
    94ed:	c6 4b       	ldab	#0x4b
    94ef:	7e af d2    	jmp	0xafd2
    94f2:	39          	rts
    94f3:	c6 41       	ldab	#0x41
    94f5:	4d          	tsta
    94f6:	27 12       	beq	0x0x950a
    94f8:	96 41       	ldaa	*0x41
    94fa:	8a 10       	oraa	#0x10
    94fc:	bd 95 4b    	jsr	0x954b
    94ff:	97 41       	staa	*0x41
    9501:	14 f7 80    	bset	*0xf7, #0x80
    9504:	8f          	xgdx
    9505:	86 83       	ldaa	#0x83
    9507:	7e 91 26    	jmp	0x9126
    950a:	96 41       	ldaa	*0x41
    950c:	84 ef       	anda	#0xef
    950e:	bd 95 4b    	jsr	0x954b
    9511:	97 41       	staa	*0x41
    9513:	15 f7 80    	bclr	*0xf7, #0x80
    9516:	8f          	xgdx
    9517:	86 83       	ldaa	#0x83
    9519:	7e 91 26    	jmp	0x9126
    951c:	7d 00 d1    	tst	0xd1
    951f:	2a 0e       	bpl	0x0x952f
    9521:	ce 00 00    	ldx	#0x0
    9524:	3a          	abx
    9525:	a7 00       	staa	0x0,x
    9527:	c0 20       	subb	#0x20
    9529:	8f          	xgdx
    952a:	86 83       	ldaa	#0x83
    952c:	7e 91 26    	jmp	0x9126
    952f:	36          	psha
    9530:	96 d1       	ldaa	*0xd1
    9532:	81 05       	cmpa	#0x5
    9534:	22 03       	bhi	0x0x9539
    9536:	32          	pula
    9537:	20 e8       	bra	0x0x9521
    9539:	96 df       	ldaa	*0xdf
    953b:	84 0f       	anda	#0xf
    953d:	b1 01 6b    	cmpa	0x16b
    9540:	32          	pula
    9541:	27 de       	beq	0x0x9521
    9543:	c0 20       	subb	#0x20
    9545:	8f          	xgdx
    9546:	86 83       	ldaa	#0x83
    9548:	7e 91 26    	jmp	0x9126
    954b:	c0 20       	subb	#0x20
    954d:	7d 00 d1    	tst	0xd1
    9550:	2a 01       	bpl	0x0x9553
    9552:	39          	rts
    9553:	36          	psha
    9554:	96 d1       	ldaa	*0xd1
    9556:	81 05       	cmpa	#0x5
    9558:	22 02       	bhi	0x0x955c
    955a:	32          	pula
    955b:	39          	rts
    955c:	96 df       	ldaa	*0xdf
    955e:	84 0f       	anda	#0xf
    9560:	b1 01 6b    	cmpa	0x16b
    9563:	32          	pula
    9564:	27 ec       	beq	0x0x9552
    9566:	38          	pulx
    9567:	39          	rts
    9568:	ce 10 23    	ldx	#0x1023
    956b:	1e 00 40 03 	brset	0x0,x, #0x40, 0x0x9572
    956f:	7e a0 6a    	jmp	0xa06a
    9572:	86 20       	ldaa	#0x20
    9574:	b7 10 44    	staa	0x1044
    9577:	01          	nop
    9578:	01          	nop
    9579:	b6 10 45    	ldaa	0x1045
    957c:	b1 01 0f    	cmpa	0x10f
    957f:	26 0b       	bne	0x0x958c
    9581:	7d 00 fe    	tst	0xfe
    9584:	27 03       	beq	0x0x9589
    9586:	7e a0 5c    	jmp	0xa05c
    9589:	7e 98 a4    	jmp	0x98a4
    958c:	16          	tab
    958d:	b8 01 0f    	eora	0x10f
    9590:	f7 01 0f    	stab	0x10f
    9593:	b4 01 0f    	anda	0x10f
    9596:	84 fc       	anda	#0xfc
    9598:	26 03       	bne	0x0x959d
    959a:	7e 97 9e    	jmp	0x979e
    959d:	7d 00 fe    	tst	0xfe
    95a0:	27 0d       	beq	0x0x95af
    95a2:	85 20       	bita	#0x20
    95a4:	26 03       	bne	0x0x95a9
    95a6:	7e a0 5c    	jmp	0xa05c
    95a9:	7f 00 fe    	clr	0xfe
    95ac:	7e a6 7a    	jmp	0xa67a
    95af:	48          	asla
    95b0:	24 2a       	bcc	0x0x95dc
    95b2:	96 ff       	ldaa	*0xff
    95b4:	81 01       	cmpa	#0x1
    95b6:	26 0d       	bne	0x0x95c5
    95b8:	b6 01 7b    	ldaa	0x17b
    95bb:	97 f9       	staa	*0xf9
    95bd:	bd ab 71    	jsr	0xab71
    95c0:	b6 01 7c    	ldaa	0x17c
    95c3:	97 fa       	staa	*0xfa
    95c5:	7f 00 ff    	clr	0xff
    95c8:	7f 01 1b    	clr	0x11b
    95cb:	bd a0 ff    	jsr	0xa0ff
    95ce:	96 f2       	ldaa	*0xf2
    95d0:	84 20       	anda	#0x20
    95d2:	97 f2       	staa	*0xf2
    95d4:	86 80       	ldaa	#0x80
    95d6:	b7 01 1c    	staa	0x11c
    95d9:	7e a0 5c    	jmp	0xa05c
    95dc:	48          	asla
    95dd:	25 03       	bcs	0x0x95e2
    95df:	7e 96 8c    	jmp	0x968c
    95e2:	d6 ff       	ldab	*0xff
    95e4:	27 4d       	beq	0x0x9633
    95e6:	c1 01       	cmpb	#0x1
    95e8:	27 71       	beq	0x0x965b
    95ea:	b6 01 1b    	ldaa	0x11b
    95ed:	81 05       	cmpa	#0x5
    95ef:	27 11       	beq	0x0x9602
    95f1:	81 25       	cmpa	#0x25
    95f3:	26 3e       	bne	0x0x9633
    95f5:	86 2b       	ldaa	#0x2b
    95f7:	b7 01 1b    	staa	0x11b
    95fa:	86 80       	ldaa	#0x80
    95fc:	b7 01 1c    	staa	0x11c
    95ff:	7e a0 5c    	jmp	0xa05c
    9602:	13 f2 40 2d 	brclr	*0xf2, #0x40, 0x0x9633
    9606:	b6 01 1e    	ldaa	0x11e
    9609:	81 1e       	cmpa	#0x1e
    960b:	27 03       	beq	0x0x9610
    960d:	7e a0 5c    	jmp	0xa05c
    9610:	4f          	clra
    9611:	f6 01 3c    	ldab	0x13c
    9614:	c1 41       	cmpb	#0x41
    9616:	27 01       	beq	0x0x9619
    9618:	4c          	inca
    9619:	bd 8f b4    	jsr	0x8fb4
    961c:	7f 00 ff    	clr	0xff
    961f:	7f 01 1b    	clr	0x11b
    9622:	bd a0 ff    	jsr	0xa0ff
    9625:	96 f2       	ldaa	*0xf2
    9627:	84 20       	anda	#0x20
    9629:	97 f2       	staa	*0xf2
    962b:	86 80       	ldaa	#0x80
    962d:	b7 01 1c    	staa	0x11c
    9630:	7e a0 5c    	jmp	0xa05c
    9633:	7d 01 70    	tst	0x170
    9636:	27 05       	beq	0x0x963d
    9638:	7f 00 ff    	clr	0xff
    963b:	20 38       	bra	0x0x9675
    963d:	c6 01       	ldab	#0x1
    963f:	d7 ff       	stab	*0xff
    9641:	d6 f9       	ldab	*0xf9
    9643:	f7 01 7b    	stab	0x17b
    9646:	d6 fa       	ldab	*0xfa
    9648:	f7 01 7c    	stab	0x17c
    964b:	c6 01       	ldab	#0x1
    964d:	f7 01 1b    	stab	0x11b
    9650:	14 f2 40    	bset	*0xf2, #0x40
    9653:	c6 80       	ldab	#0x80
    9655:	f7 01 1c    	stab	0x11c
    9658:	7e a0 5c    	jmp	0xa05c
    965b:	96 f9       	ldaa	*0xf9
    965d:	81 04       	cmpa	#0x4
    965f:	26 05       	bne	0x0x9666
    9661:	bd a6 31    	jsr	0xa631
    9664:	20 03       	bra	0x0x9669
    9666:	bd a6 43    	jsr	0xa643
    9669:	7f 00 ff    	clr	0xff
    966c:	7f 00 f2    	clr	0xf2
    966f:	7f 00 fb    	clr	0xfb
    9672:	bd a0 ff    	jsr	0xa0ff
    9675:	7f 01 1b    	clr	0x11b
    9678:	86 80       	ldaa	#0x80
    967a:	b7 01 1c    	staa	0x11c
    967d:	b6 01 7b    	ldaa	0x17b
    9680:	81 04       	cmpa	#0x4
    9682:	26 05       	bne	0x0x9689
    9684:	97 f9       	staa	*0xf9
    9686:	bd ab 71    	jsr	0xab71
    9689:	7e a0 5c    	jmp	0xa05c
    968c:	48          	asla
    968d:	25 03       	bcs	0x0x9692
    968f:	7e 96 a6    	jmp	0x96a6
    9692:	7d 00 fb    	tst	0xfb
    9695:	27 0c       	beq	0x0x96a3
    9697:	96 f9       	ldaa	*0xf9
    9699:	81 04       	cmpa	#0x4
    969b:	27 06       	beq	0x0x96a3
    969d:	7c 00 fe    	inc	0xfe
    96a0:	7e a6 60    	jmp	0xa660
    96a3:	7e a0 5c    	jmp	0xa05c
    96a6:	48          	asla
    96a7:	25 03       	bcs	0x0x96ac
    96a9:	7e 97 40    	jmp	0x9740
    96ac:	c6 29       	ldab	#0x29
    96ae:	f1 01 1b    	cmpb	0x11b
    96b1:	26 32       	bne	0x0x96e5
    96b3:	f6 01 6b    	ldab	0x16b
    96b6:	5c          	incb
    96b7:	c4 07       	andb	#0x7
    96b9:	f7 01 6b    	stab	0x16b
    96bc:	b6 50 00    	ldaa	0x5000
    96bf:	81 06       	cmpa	#0x6
    96c1:	27 18       	beq	0x0x96db
    96c3:	ce 50 50    	ldx	#0x5050
    96c6:	3a          	abx
    96c7:	a6 00       	ldaa	0x0,x
    96c9:	26 03       	bne	0x0x96ce
    96cb:	7f 01 6b    	clr	0x16b
    96ce:	86 29       	ldaa	#0x29
    96d0:	b7 01 1b    	staa	0x11b
    96d3:	86 80       	ldaa	#0x80
    96d5:	b7 01 1c    	staa	0x11c
    96d8:	7e a0 5c    	jmp	0xa05c
    96db:	7d 50 23    	tst	0x5023
    96de:	26 ee       	bne	0x0x96ce
    96e0:	7f 01 6b    	clr	0x16b
    96e3:	20 e9       	bra	0x0x96ce
    96e5:	7d 00 ff    	tst	0xff
    96e8:	26 27       	bne	0x0x9711
    96ea:	96 f9       	ldaa	*0xf9
    96ec:	4c          	inca
    96ed:	81 04       	cmpa	#0x4
    96ef:	25 01       	bcs	0x0x96f2
    96f1:	4f          	clra
    96f2:	97 f9       	staa	*0xf9
    96f4:	bd ab 4b    	jsr	0xab4b
    96f7:	b7 7f f6    	staa	0x7ff6
    96fa:	bd ab 71    	jsr	0xab71
    96fd:	86 80       	ldaa	#0x80
    96ff:	b7 01 1c    	staa	0x11c
    9702:	7f 00 fb    	clr	0xfb
    9705:	7f 00 f2    	clr	0xf2
    9708:	14 d1 80    	bset	*0xd1, #0x80
    970b:	bd a1 16    	jsr	0xa116
    970e:	7e a0 5c    	jmp	0xa05c
    9711:	13 ff 01 25 	brclr	*0xff, #0x01, 0x0x973a
    9715:	96 f9       	ldaa	*0xf9
    9717:	81 04       	cmpa	#0x4
    9719:	26 03       	bne	0x0x971e
    971b:	7e a0 5c    	jmp	0xa05c
    971e:	96 f9       	ldaa	*0xf9
    9720:	4c          	inca
    9721:	81 02       	cmpa	#0x2
    9723:	24 04       	bcc	0x0x9729
    9725:	86 03       	ldaa	#0x3
    9727:	20 06       	bra	0x0x972f
    9729:	81 04       	cmpa	#0x4
    972b:	25 02       	bcs	0x0x972f
    972d:	86 02       	ldaa	#0x2
    972f:	97 f9       	staa	*0xf9
    9731:	bd ab 71    	jsr	0xab71
    9734:	7c 01 7e    	inc	0x17e
    9737:	7e a0 5c    	jmp	0xa05c
    973a:	7c 01 7e    	inc	0x17e
    973d:	7e a0 5c    	jmp	0xa05c
    9740:	48          	asla
    9741:	24 11       	bcc	0x0x9754
    9743:	d6 ff       	ldab	*0xff
    9745:	c1 02       	cmpb	#0x2
    9747:	27 03       	beq	0x0x974c
    9749:	7e a0 5c    	jmp	0xa05c
    974c:	86 01       	ldaa	#0x1
    974e:	b7 01 7f    	staa	0x17f
    9751:	7e a0 5c    	jmp	0xa05c
    9754:	b6 01 0f    	ldaa	0x10f
    9757:	85 10       	bita	#0x10
    9759:	26 11       	bne	0x0x976c
    975b:	d6 ff       	ldab	*0xff
    975d:	c1 02       	cmpb	#0x2
    975f:	27 03       	beq	0x0x9764
    9761:	7e a0 5c    	jmp	0xa05c
    9764:	86 80       	ldaa	#0x80
    9766:	b7 01 7f    	staa	0x17f
    9769:	7e a0 5c    	jmp	0xa05c
    976c:	d6 ff       	ldab	*0xff
    976e:	c1 02       	cmpb	#0x2
    9770:	27 f2       	beq	0x0x9764
    9772:	86 04       	ldaa	#0x4
    9774:	97 f9       	staa	*0xf9
    9776:	bd ab 4b    	jsr	0xab4b
    9779:	b7 7f f6    	staa	0x7ff6
    977c:	bd ab 71    	jsr	0xab71
    977f:	bd 9c b5    	jsr	0x9cb5
    9782:	cc 51 00    	ldd	#0x5100
    9785:	fd 51 80    	std	0x5180
    9788:	bd a3 15    	jsr	0xa315
    978b:	b6 50 00    	ldaa	0x5000
    978e:	97 d1       	staa	*0xd1
    9790:	86 80       	ldaa	#0x80
    9792:	b7 01 1c    	staa	0x11c
    9795:	7f 00 fb    	clr	0xfb
    9798:	7f 00 f2    	clr	0xf2
    979b:	7e a0 5c    	jmp	0xa05c
    979e:	7d 00 fe    	tst	0xfe
    97a1:	27 03       	beq	0x0x97a6
    97a3:	7e a0 5c    	jmp	0xa05c
    97a6:	17          	tba
    97a7:	84 03       	anda	#0x3
    97a9:	81 03       	cmpa	#0x3
    97ab:	26 04       	bne	0x0x97b1
    97ad:	86 02       	ldaa	#0x2
    97af:	20 06       	bra	0x0x97b7
    97b1:	81 02       	cmpa	#0x2
    97b3:	26 02       	bne	0x0x97b7
    97b5:	86 03       	ldaa	#0x3
    97b7:	b1 01 18    	cmpa	0x118
    97ba:	26 06       	bne	0x0x97c2
    97bc:	f7 01 0f    	stab	0x10f
    97bf:	7e a0 5c    	jmp	0xa05c
    97c2:	f7 01 0f    	stab	0x10f
    97c5:	f6 01 18    	ldab	0x118
    97c8:	5c          	incb
    97c9:	c4 03       	andb	#0x3
    97cb:	11          	cba
    97cc:	27 12       	beq	0x0x97e0
    97ce:	f6 01 18    	ldab	0x118
    97d1:	5a          	decb
    97d2:	c4 03       	andb	#0x3
    97d4:	11          	cba
    97d5:	26 03       	bne	0x0x97da
    97d7:	7e 98 5b    	jmp	0x985b
    97da:	b7 01 18    	staa	0x118
    97dd:	7e a0 5c    	jmp	0xa05c
    97e0:	b7 01 18    	staa	0x118
    97e3:	7d 00 ff    	tst	0xff
    97e6:	26 27       	bne	0x0x980f
    97e8:	7f 00 fb    	clr	0xfb
    97eb:	7f 00 f2    	clr	0xf2
    97ee:	12 d1 80 11 	brset	*0xd1, #0x80, 0x0x9803
    97f2:	bd a0 d3    	jsr	0xa0d3
    97f5:	bd 9c b5    	jsr	0x9cb5
    97f8:	bd a3 15    	jsr	0xa315
    97fb:	b6 50 00    	ldaa	0x5000
    97fe:	97 d1       	staa	*0xd1
    9800:	7e a0 5c    	jmp	0xa05c
    9803:	bd a0 7e    	jsr	0xa07e
    9806:	bd a1 16    	jsr	0xa116
    9809:	14 d1 80    	bset	*0xd1, #0x80
    980c:	7e a0 5c    	jmp	0xa05c
    980f:	13 ff 01 12 	brclr	*0xff, #0x01, 0x0x9825
    9813:	96 f9       	ldaa	*0xf9
    9815:	81 04       	cmpa	#0x4
    9817:	26 06       	bne	0x0x981f
    9819:	bd a0 d3    	jsr	0xa0d3
    981c:	7e a0 5c    	jmp	0xa05c
    981f:	bd a0 7e    	jsr	0xa07e
    9822:	7e a0 5c    	jmp	0xa05c
    9825:	86 01       	ldaa	#0x1
    9827:	b7 01 1a    	staa	0x11a
    982a:	7d 00 fb    	tst	0xfb
    982d:	27 03       	beq	0x0x9832
    982f:	7e a0 5c    	jmp	0xa05c
    9832:	b6 01 1b    	ldaa	0x11b
    9835:	81 03       	cmpa	#0x3
    9837:	27 1f       	beq	0x0x9858
    9839:	81 04       	cmpa	#0x4
    983b:	27 1b       	beq	0x0x9858
    983d:	81 05       	cmpa	#0x5
    983f:	27 17       	beq	0x0x9858
    9841:	81 23       	cmpa	#0x23
    9843:	27 13       	beq	0x0x9858
    9845:	81 24       	cmpa	#0x24
    9847:	27 0f       	beq	0x0x9858
    9849:	81 25       	cmpa	#0x25
    984b:	27 0b       	beq	0x0x9858
    984d:	14 fb 01    	bset	*0xfb, #0x01
    9850:	14 f2 20    	bset	*0xf2, #0x20
    9853:	86 ff       	ldaa	#0xff
    9855:	b7 01 66    	staa	0x166
    9858:	7e a0 5c    	jmp	0xa05c
    985b:	b7 01 18    	staa	0x118
    985e:	7d 00 ff    	tst	0xff
    9861:	26 27       	bne	0x0x988a
    9863:	7f 00 fb    	clr	0xfb
    9866:	7f 00 f2    	clr	0xf2
    9869:	12 d1 80 11 	brset	*0xd1, #0x80, 0x0x987e
    986d:	bd a0 e9    	jsr	0xa0e9
    9870:	bd 9c b5    	jsr	0x9cb5
    9873:	bd a3 15    	jsr	0xa315
    9876:	b6 50 00    	ldaa	0x5000
    9879:	97 d1       	staa	*0xd1
    987b:	7e a0 5c    	jmp	0xa05c
    987e:	bd a0 a9    	jsr	0xa0a9
    9881:	bd a1 16    	jsr	0xa116
    9884:	14 d1 80    	bset	*0xd1, #0x80
    9887:	7e a0 5c    	jmp	0xa05c
    988a:	13 ff 01 12 	brclr	*0xff, #0x01, 0x0x98a0
    988e:	96 f9       	ldaa	*0xf9
    9890:	81 04       	cmpa	#0x4
    9892:	26 06       	bne	0x0x989a
    9894:	bd a0 e9    	jsr	0xa0e9
    9897:	7e a0 5c    	jmp	0xa05c
    989a:	bd a0 a9    	jsr	0xa0a9
    989d:	7e a0 5c    	jmp	0xa05c
    98a0:	86 80       	ldaa	#0x80
    98a2:	20 83       	bra	0x0x9827
    98a4:	86 10       	ldaa	#0x10
    98a6:	b7 10 44    	staa	0x1044
    98a9:	01          	nop
    98aa:	01          	nop
    98ab:	b6 10 45    	ldaa	0x1045
    98ae:	b1 01 10    	cmpa	0x110
    98b1:	26 03       	bne	0x0x98b6
    98b3:	7e 9a c9    	jmp	0x9ac9
    98b6:	16          	tab
    98b7:	b8 01 10    	eora	0x110
    98ba:	f7 01 10    	stab	0x110
    98bd:	b4 01 10    	anda	0x110
    98c0:	26 03       	bne	0x0x98c5
    98c2:	7e 9a c9    	jmp	0x9ac9
    98c5:	48          	asla
    98c6:	24 34       	bcc	0x0x98fc
    98c8:	86 16       	ldaa	#0x16
    98ca:	b1 01 1b    	cmpa	0x11b
    98cd:	26 10       	bne	0x0x98df
    98cf:	4c          	inca
    98d0:	b7 01 1b    	staa	0x11b
    98d3:	96 f2       	ldaa	*0xf2
    98d5:	84 20       	anda	#0x20
    98d7:	8a 02       	oraa	#0x2
    98d9:	97 f2       	staa	*0xf2
    98db:	86 02       	ldaa	#0x2
    98dd:	20 13       	bra	0x0x98f2
    98df:	b7 01 1b    	staa	0x11b
    98e2:	bd a0 ff    	jsr	0xa0ff
    98e5:	14 f3 80    	bset	*0xf3, #0x80
    98e8:	96 f2       	ldaa	*0xf2
    98ea:	84 20       	anda	#0x20
    98ec:	8a 01       	oraa	#0x1
    98ee:	97 f2       	staa	*0xf2
    98f0:	86 02       	ldaa	#0x2
    98f2:	97 ff       	staa	*0xff
    98f4:	86 80       	ldaa	#0x80
    98f6:	b7 01 1c    	staa	0x11c
    98f9:	7e a0 5c    	jmp	0xa05c
    98fc:	48          	asla
    98fd:	24 58       	bcc	0x0x9957
    98ff:	7d 01 10    	tst	0x110
    9902:	2a 11       	bpl	0x0x9915
    9904:	7d 00 99    	tst	0x99
    9907:	26 06       	bne	0x0x990f
    9909:	7c 00 99    	inc	0x99
    990c:	7e 95 c5    	jmp	0x95c5
    990f:	7f 00 99    	clr	0x99
    9912:	7e 95 c5    	jmp	0x95c5
    9915:	96 f2       	ldaa	*0xf2
    9917:	84 20       	anda	#0x20
    9919:	c6 13       	ldab	#0x13
    991b:	f1 01 1b    	cmpb	0x11b
    991e:	27 12       	beq	0x0x9932
    9920:	5c          	incb
    9921:	f1 01 1b    	cmpb	0x11b
    9924:	27 11       	beq	0x0x9937
    9926:	5c          	incb
    9927:	f1 01 1b    	cmpb	0x11b
    992a:	27 10       	beq	0x0x993c
    992c:	5a          	decb
    992d:	5a          	decb
    992e:	8a 01       	oraa	#0x1
    9930:	20 0e       	bra	0x0x9940
    9932:	5c          	incb
    9933:	8a 02       	oraa	#0x2
    9935:	20 09       	bra	0x0x9940
    9937:	5c          	incb
    9938:	8a 04       	oraa	#0x4
    993a:	20 04       	bra	0x0x9940
    993c:	c6 28       	ldab	#0x28
    993e:	8a 08       	oraa	#0x8
    9940:	f7 01 1b    	stab	0x11b
    9943:	97 f2       	staa	*0xf2
    9945:	bd a0 ff    	jsr	0xa0ff
    9948:	14 f3 40    	bset	*0xf3, #0x40
    994b:	86 02       	ldaa	#0x2
    994d:	97 ff       	staa	*0xff
    994f:	86 80       	ldaa	#0x80
    9951:	b7 01 1c    	staa	0x11c
    9954:	7e a0 5c    	jmp	0xa05c
    9957:	48          	asla
    9958:	24 33       	bcc	0x0x998d
    995a:	b6 10 45    	ldaa	0x1045
    995d:	85 40       	bita	#0x40
    995f:	27 03       	beq	0x0x9964
    9961:	7e a6 94    	jmp	0xa694
    9964:	96 f2       	ldaa	*0xf2
    9966:	84 20       	anda	#0x20
    9968:	c6 06       	ldab	#0x6
    996a:	f1 01 1b    	cmpb	0x11b
    996d:	27 04       	beq	0x0x9973
    996f:	8a 01       	oraa	#0x1
    9971:	20 03       	bra	0x0x9976
    9973:	5c          	incb
    9974:	8a 02       	oraa	#0x2
    9976:	f7 01 1b    	stab	0x11b
    9979:	97 f2       	staa	*0xf2
    997b:	bd a0 ff    	jsr	0xa0ff
    997e:	14 f3 20    	bset	*0xf3, #0x20
    9981:	86 02       	ldaa	#0x2
    9983:	97 ff       	staa	*0xff
    9985:	86 80       	ldaa	#0x80
    9987:	b7 01 1c    	staa	0x11c
    998a:	7e a0 5c    	jmp	0xa05c
    998d:	48          	asla
    998e:	25 03       	bcs	0x0x9993
    9990:	7e 9a 17    	jmp	0x9a17
    9993:	96 73       	ldaa	*0x73
    9995:	27 2b       	beq	0x0x99c2
    9997:	4a          	deca
    9998:	27 54       	beq	0x0x99ee
    999a:	96 52       	ldaa	*0x52
    999c:	c6 52       	ldab	#0x52
    999e:	13 f3 10 0f 	brclr	*0xf3, #0x10, 0x0x99b1
    99a2:	bd af d2    	jsr	0xafd2
    99a5:	15 f3 10    	bclr	*0xf3, #0x10
    99a8:	4f          	clra
    99a9:	c6 50       	ldab	#0x50
    99ab:	bd b0 5c    	jsr	0xb05c
    99ae:	7e a0 5c    	jmp	0xa05c
    99b1:	44          	lsra
    99b2:	bd af d2    	jsr	0xafd2
    99b5:	14 f3 10    	bset	*0xf3, #0x10
    99b8:	86 7f       	ldaa	#0x7f
    99ba:	c6 50       	ldab	#0x50
    99bc:	bd b0 5c    	jsr	0xb05c
    99bf:	7e a0 5c    	jmp	0xa05c
    99c2:	15 f3 08    	bclr	*0xf3, #0x08
    99c5:	c6 4c       	ldab	#0x4c
    99c7:	13 f3 10 11 	brclr	*0xf3, #0x10, 0x0x99dc
    99cb:	96 4c       	ldaa	*0x4c
    99cd:	bd af d2    	jsr	0xafd2
    99d0:	15 f3 10    	bclr	*0xf3, #0x10
    99d3:	4f          	clra
    99d4:	c6 4e       	ldab	#0x4e
    99d6:	bd b0 5c    	jsr	0xb05c
    99d9:	7e a0 5c    	jmp	0xa05c
    99dc:	86 64       	ldaa	#0x64
    99de:	bd af d2    	jsr	0xafd2
    99e1:	14 f3 10    	bset	*0xf3, #0x10
    99e4:	86 7f       	ldaa	#0x7f
    99e6:	c6 4e       	ldab	#0x4e
    99e8:	bd b0 5c    	jsr	0xb05c
    99eb:	7e a0 5c    	jmp	0xa05c
    99ee:	c6 4d       	ldab	#0x4d
    99f0:	13 f3 10 11 	brclr	*0xf3, #0x10, 0x0x9a05
    99f4:	96 4d       	ldaa	*0x4d
    99f6:	bd af d2    	jsr	0xafd2
    99f9:	15 f3 10    	bclr	*0xf3, #0x10
    99fc:	4f          	clra
    99fd:	c6 4f       	ldab	#0x4f
    99ff:	bd b0 5c    	jsr	0xb05c
    9a02:	7e a0 5c    	jmp	0xa05c
    9a05:	86 64       	ldaa	#0x64
    9a07:	bd af d2    	jsr	0xafd2
    9a0a:	14 f3 10    	bset	*0xf3, #0x10
    9a0d:	86 7f       	ldaa	#0x7f
    9a0f:	c6 4f       	ldab	#0x4f
    9a11:	bd b0 5c    	jsr	0xb05c
    9a14:	7e a0 5c    	jmp	0xa05c
    9a17:	48          	asla
    9a18:	25 03       	bcs	0x0x9a1d
    9a1a:	7e 9a 9d    	jmp	0x9a9d
    9a1d:	96 73       	ldaa	*0x73
    9a1f:	27 23       	beq	0x0x9a44
    9a21:	4a          	deca
    9a22:	27 4b       	beq	0x0x9a6f
    9a24:	96 66       	ldaa	*0x66
    9a26:	88 01       	eora	#0x1
    9a28:	97 66       	staa	*0x66
    9a2a:	c6 66       	ldab	#0x66
    9a2c:	bd af d2    	jsr	0xafd2
    9a2f:	86 08       	ldaa	#0x8
    9a31:	98 f3       	eora	*0xf3
    9a33:	97 f3       	staa	*0xf3
    9a35:	4f          	clra
    9a36:	c6 4d       	ldab	#0x4d
    9a38:	13 66 01 02 	brclr	*0x66, #0x01, 0x0x9a3e
    9a3c:	86 7f       	ldaa	#0x7f
    9a3e:	bd b0 5c    	jsr	0xb05c
    9a41:	7e a0 5c    	jmp	0xa05c
    9a44:	15 f3 10    	bclr	*0xf3, #0x10
    9a47:	96 4c       	ldaa	*0x4c
    9a49:	c6 4c       	ldab	#0x4c
    9a4b:	13 f3 08 0f 	brclr	*0xf3, #0x08, 0x0x9a5e
    9a4f:	bd af d2    	jsr	0xafd2
    9a52:	15 f3 08    	bclr	*0xf3, #0x08
    9a55:	4f          	clra
    9a56:	c6 4b       	ldab	#0x4b
    9a58:	bd b0 5c    	jsr	0xb05c
    9a5b:	7e a0 5c    	jmp	0xa05c
    9a5e:	44          	lsra
    9a5f:	bd af d2    	jsr	0xafd2
    9a62:	14 f3 08    	bset	*0xf3, #0x08
    9a65:	86 7f       	ldaa	#0x7f
    9a67:	c6 4b       	ldab	#0x4b
    9a69:	bd b0 5c    	jsr	0xb05c
    9a6c:	7e a0 5c    	jmp	0xa05c
    9a6f:	c6 4f       	ldab	#0x4f
    9a71:	13 f3 08 11 	brclr	*0xf3, #0x08, 0x0x9a86
    9a75:	96 4f       	ldaa	*0x4f
    9a77:	bd af d2    	jsr	0xafd2
    9a7a:	15 f3 08    	bclr	*0xf3, #0x08
    9a7d:	4f          	clra
    9a7e:	c6 4c       	ldab	#0x4c
    9a80:	bd b0 5c    	jsr	0xb05c
    9a83:	7e a0 5c    	jmp	0xa05c
    9a86:	86 7f       	ldaa	#0x7f
    9a88:	90 4f       	suba	*0x4f
    9a8a:	44          	lsra
    9a8b:	9b 4f       	adda	*0x4f
    9a8d:	bd af d2    	jsr	0xafd2
    9a90:	14 f3 08    	bset	*0xf3, #0x08
    9a93:	86 7f       	ldaa	#0x7f
    9a95:	c6 4c       	ldab	#0x4c
    9a97:	bd b0 5c    	jsr	0xb05c
    9a9a:	7e a0 5c    	jmp	0xa05c
    9a9d:	d6 73       	ldab	*0x73
    9a9f:	5c          	incb
    9aa0:	c1 02       	cmpb	#0x2
    9aa2:	23 01       	bls	0x0x9aa5
    9aa4:	5f          	clrb
    9aa5:	d7 73       	stab	*0x73
    9aa7:	86 e0       	ldaa	#0xe0
    9aa9:	94 f3       	anda	*0xf3
    9aab:	97 f3       	staa	*0xf3
    9aad:	86 01       	ldaa	#0x1
    9aaf:	7d 00 73    	tst	0x73
    9ab2:	27 0e       	beq	0x0x9ac2
    9ab4:	48          	asla
    9ab5:	5a          	decb
    9ab6:	27 0a       	beq	0x0x9ac2
    9ab8:	48          	asla
    9ab9:	d6 66       	ldab	*0x66
    9abb:	c4 01       	andb	#0x1
    9abd:	27 03       	beq	0x0x9ac2
    9abf:	14 f3 08    	bset	*0xf3, #0x08
    9ac2:	9a f3       	oraa	*0xf3
    9ac4:	97 f3       	staa	*0xf3
    9ac6:	7e a0 5c    	jmp	0xa05c
    9ac9:	86 08       	ldaa	#0x8
    9acb:	b7 10 44    	staa	0x1044
    9ace:	01          	nop
    9acf:	01          	nop
    9ad0:	b6 10 45    	ldaa	0x1045
    9ad3:	b1 01 11    	cmpa	0x111
    9ad6:	26 03       	bne	0x0x9adb
    9ad8:	7e 9b d4    	jmp	0x9bd4
    9adb:	16          	tab
    9adc:	b8 01 11    	eora	0x111
    9adf:	f7 01 11    	stab	0x111
    9ae2:	b4 01 11    	anda	0x111
    9ae5:	26 03       	bne	0x0x9aea
    9ae7:	7e 9b d4    	jmp	0x9bd4
    9aea:	44          	lsra
    9aeb:	24 50       	bcc	0x0x9b3d
    9aed:	12 f4 01 10 	brset	*0xf4, #0x01, 0x0x9b01
    9af1:	bd a0 ff    	jsr	0xa0ff
    9af4:	14 f4 01    	bset	*0xf4, #0x01
    9af7:	c6 19       	ldab	#0x19
    9af9:	96 f2       	ldaa	*0xf2
    9afb:	84 20       	anda	#0x20
    9afd:	8a 01       	oraa	#0x1
    9aff:	20 2b       	bra	0x0x9b2c
    9b01:	96 f2       	ldaa	*0xf2
    9b03:	84 20       	anda	#0x20
    9b05:	c6 19       	ldab	#0x19
    9b07:	f1 01 1b    	cmpb	0x11b
    9b0a:	27 12       	beq	0x0x9b1e
    9b0c:	5c          	incb
    9b0d:	f1 01 1b    	cmpb	0x11b
    9b10:	27 11       	beq	0x0x9b23
    9b12:	5c          	incb
    9b13:	f1 01 1b    	cmpb	0x11b
    9b16:	27 10       	beq	0x0x9b28
    9b18:	c6 19       	ldab	#0x19
    9b1a:	8a 01       	oraa	#0x1
    9b1c:	20 0e       	bra	0x0x9b2c
    9b1e:	5c          	incb
    9b1f:	8a 02       	oraa	#0x2
    9b21:	20 09       	bra	0x0x9b2c
    9b23:	5c          	incb
    9b24:	8a 04       	oraa	#0x4
    9b26:	20 04       	bra	0x0x9b2c
    9b28:	c6 19       	ldab	#0x19
    9b2a:	8a 01       	oraa	#0x1
    9b2c:	97 f2       	staa	*0xf2
    9b2e:	f7 01 1b    	stab	0x11b
    9b31:	86 02       	ldaa	#0x2
    9b33:	97 ff       	staa	*0xff
    9b35:	86 80       	ldaa	#0x80
    9b37:	b7 01 1c    	staa	0x11c
    9b3a:	7e a0 5c    	jmp	0xa05c
    9b3d:	44          	lsra
    9b3e:	24 14       	bcc	0x0x9b54
    9b40:	12 f4 02 bd 	brset	*0xf4, #0x02, 0x0x9b01
    9b44:	bd a0 ff    	jsr	0xa0ff
    9b47:	14 f4 02    	bset	*0xf4, #0x02
    9b4a:	c6 19       	ldab	#0x19
    9b4c:	96 f2       	ldaa	*0xf2
    9b4e:	84 20       	anda	#0x20
    9b50:	8a 01       	oraa	#0x1
    9b52:	20 d8       	bra	0x0x9b2c
    9b54:	44          	lsra
    9b55:	24 03       	bcc	0x0x9b5a
    9b57:	7e a0 5c    	jmp	0xa05c
    9b5a:	44          	lsra
    9b5b:	24 1f       	bcc	0x0x9b7c
    9b5d:	bd a0 ff    	jsr	0xa0ff
    9b60:	14 f4 08    	bset	*0xf4, #0x08
    9b63:	96 f2       	ldaa	*0xf2
    9b65:	84 20       	anda	#0x20
    9b67:	8a 01       	oraa	#0x1
    9b69:	97 f2       	staa	*0xf2
    9b6b:	c6 26       	ldab	#0x26
    9b6d:	f7 01 1b    	stab	0x11b
    9b70:	86 02       	ldaa	#0x2
    9b72:	97 ff       	staa	*0xff
    9b74:	86 80       	ldaa	#0x80
    9b76:	b7 01 1c    	staa	0x11c
    9b79:	7e a0 5c    	jmp	0xa05c
    9b7c:	13 f4 10 0d 	brclr	*0xf4, #0x10, 0x0x9b8d
    9b80:	c6 23       	ldab	#0x23
    9b82:	f1 01 1b    	cmpb	0x11b
    9b85:	27 25       	beq	0x0x9bac
    9b87:	5c          	incb
    9b88:	f1 01 1b    	cmpb	0x11b
    9b8b:	27 33       	beq	0x0x9bc0
    9b8d:	c6 23       	ldab	#0x23
    9b8f:	f7 01 1b    	stab	0x11b
    9b92:	bd a0 ff    	jsr	0xa0ff
    9b95:	14 f4 10    	bset	*0xf4, #0x10
    9b98:	96 f2       	ldaa	*0xf2
    9b9a:	84 20       	anda	#0x20
    9b9c:	8a 01       	oraa	#0x1
    9b9e:	97 f2       	staa	*0xf2
    9ba0:	86 02       	ldaa	#0x2
    9ba2:	97 ff       	staa	*0xff
    9ba4:	86 80       	ldaa	#0x80
    9ba6:	b7 01 1c    	staa	0x11c
    9ba9:	7e a0 5c    	jmp	0xa05c
    9bac:	5c          	incb
    9bad:	f7 01 1b    	stab	0x11b
    9bb0:	96 f2       	ldaa	*0xf2
    9bb2:	84 20       	anda	#0x20
    9bb4:	8a 02       	oraa	#0x2
    9bb6:	97 f2       	staa	*0xf2
    9bb8:	86 80       	ldaa	#0x80
    9bba:	b7 01 1c    	staa	0x11c
    9bbd:	7e a0 5c    	jmp	0xa05c
    9bc0:	5c          	incb
    9bc1:	f7 01 1b    	stab	0x11b
    9bc4:	96 f2       	ldaa	*0xf2
    9bc6:	84 20       	anda	#0x20
    9bc8:	8a 43       	oraa	#0x43
    9bca:	97 f2       	staa	*0xf2
    9bcc:	86 80       	ldaa	#0x80
    9bce:	b7 01 1c    	staa	0x11c
    9bd1:	7e a0 5c    	jmp	0xa05c
    9bd4:	86 04       	ldaa	#0x4
    9bd6:	b7 10 44    	staa	0x1044
    9bd9:	01          	nop
    9bda:	01          	nop
    9bdb:	b6 10 45    	ldaa	0x1045
    9bde:	b1 01 12    	cmpa	0x112
    9be1:	26 03       	bne	0x0x9be6
    9be3:	7e 9e 9d    	jmp	0x9e9d
    9be6:	16          	tab
    9be7:	b8 01 12    	eora	0x112
    9bea:	f7 01 12    	stab	0x112
    9bed:	b4 01 12    	anda	0x112
    9bf0:	26 03       	bne	0x0x9bf5
    9bf2:	7e 9e 9d    	jmp	0x9e9d
    9bf5:	48          	asla
    9bf6:	24 1e       	bcc	0x0x9c16
    9bf8:	96 4a       	ldaa	*0x4a
    9bfa:	4c          	inca
    9bfb:	84 03       	anda	#0x3
    9bfd:	97 4a       	staa	*0x4a
    9bff:	c6 4a       	ldab	#0x4a
    9c01:	bd af d2    	jsr	0xafd2
    9c04:	c6 1f       	ldab	#0x1f
    9c06:	d4 f4       	andb	*0xf4
    9c08:	d7 f4       	stab	*0xf4
    9c0a:	48          	asla
    9c0b:	48          	asla
    9c0c:	48          	asla
    9c0d:	48          	asla
    9c0e:	48          	asla
    9c0f:	9a f4       	oraa	*0xf4
    9c11:	97 f4       	staa	*0xf4
    9c13:	7e a0 4c    	jmp	0xa04c
    9c16:	48          	asla
    9c17:	24 6f       	bcc	0x0x9c88
    9c19:	86 02       	ldaa	#0x2
    9c1b:	b1 01 1b    	cmpa	0x11b
    9c1e:	26 15       	bne	0x0x9c35
    9c20:	96 f2       	ldaa	*0xf2
    9c22:	84 20       	anda	#0x20
    9c24:	8a 02       	oraa	#0x2
    9c26:	97 f2       	staa	*0xf2
    9c28:	86 03       	ldaa	#0x3
    9c2a:	b7 01 1b    	staa	0x11b
    9c2d:	86 80       	ldaa	#0x80
    9c2f:	b7 01 1c    	staa	0x11c
    9c32:	7e a0 5c    	jmp	0xa05c
    9c35:	86 03       	ldaa	#0x3
    9c37:	b1 01 1b    	cmpa	0x11b
    9c3a:	26 14       	bne	0x0x9c50
    9c3c:	4c          	inca
    9c3d:	b7 01 1b    	staa	0x11b
    9c40:	96 f2       	ldaa	*0xf2
    9c42:	84 20       	anda	#0x20
    9c44:	8a 04       	oraa	#0x4
    9c46:	97 f2       	staa	*0xf2
    9c48:	86 80       	ldaa	#0x80
    9c4a:	b7 01 1c    	staa	0x11c
    9c4d:	7e a0 5c    	jmp	0xa05c
    9c50:	86 04       	ldaa	#0x4
    9c52:	b1 01 1b    	cmpa	0x11b
    9c55:	26 14       	bne	0x0x9c6b
    9c57:	4c          	inca
    9c58:	b7 01 1b    	staa	0x11b
    9c5b:	96 f2       	ldaa	*0xf2
    9c5d:	84 20       	anda	#0x20
    9c5f:	8a 48       	oraa	#0x48
    9c61:	97 f2       	staa	*0xf2
    9c63:	86 80       	ldaa	#0x80
    9c65:	b7 01 1c    	staa	0x11c
    9c68:	7e a0 5c    	jmp	0xa05c
    9c6b:	86 02       	ldaa	#0x2
    9c6d:	b7 01 1b    	staa	0x11b
    9c70:	97 ff       	staa	*0xff
    9c72:	bd a0 ff    	jsr	0xa0ff
    9c75:	96 f2       	ldaa	*0xf2
    9c77:	84 20       	anda	#0x20
    9c79:	8a 01       	oraa	#0x1
    9c7b:	97 f2       	staa	*0xf2
    9c7d:	14 f5 40    	bset	*0xf5, #0x40
    9c80:	86 80       	ldaa	#0x80
    9c82:	b7 01 1c    	staa	0x11c
    9c85:	7e a0 5c    	jmp	0xa05c
    9c88:	48          	asla
    9c89:	24 76       	bcc	0x0x9d01
    9c8b:	86 04       	ldaa	#0x4
    9c8d:	91 f9       	cmpa	*0xf9
    9c8f:	26 08       	bne	0x0x9c99
    9c91:	15 f5 20    	bclr	*0xf5, #0x20
    9c94:	8d 1f       	bsr	0x0x9cb5
    9c96:	7e ab 2b    	jmp	0xab2b
    9c99:	86 20       	ldaa	#0x20
    9c9b:	98 6a       	eora	*0x6a
    9c9d:	97 6a       	staa	*0x6a
    9c9f:	13 6a 20 05 	brclr	*0x6a, #0x20, 0x0x9ca8
    9ca3:	14 f5 20    	bset	*0xf5, #0x20
    9ca6:	20 03       	bra	0x0x9cab
    9ca8:	15 f5 20    	bclr	*0xf5, #0x20
    9cab:	c6 6a       	ldab	#0x6a
    9cad:	bd af d2    	jsr	0xafd2
    9cb0:	8d 03       	bsr	0x0x9cb5
    9cb2:	7e ab 2b    	jmp	0xab2b
    9cb5:	7f 00 f8    	clr	0xf8
    9cb8:	ce 01 07    	ldx	#0x107
    9cbb:	ff 01 08    	stx	0x108
    9cbe:	c6 08       	ldab	#0x8
    9cc0:	ce 01 00    	ldx	#0x100
    9cc3:	6f 00       	clr	0x0,x
    9cc5:	08          	inx
    9cc6:	5a          	decb
    9cc7:	26 fa       	bne	0x0x9cc3
    9cc9:	96 f9       	ldaa	*0xf9
    9ccb:	81 04       	cmpa	#0x4
    9ccd:	26 0b       	bne	0x0x9cda
    9ccf:	ce 51 00    	ldx	#0x5100
    9cd2:	c6 08       	ldab	#0x8
    9cd4:	6f 00       	clr	0x0,x
    9cd6:	08          	inx
    9cd7:	5a          	decb
    9cd8:	26 fa       	bne	0x0x9cd4
    9cda:	7d 00 d0    	tst	0xd0
    9cdd:	27 05       	beq	0x0x9ce4
    9cdf:	7f 00 d0    	clr	0xd0
    9ce2:	20 08       	bra	0x0x9cec
    9ce4:	7d 10 29    	tst	0x1029
    9ce7:	2a fb       	bpl	0x0x9ce4
    9ce9:	f6 10 2a    	ldab	0x102a
    9cec:	5f          	clrb
    9ced:	01          	nop
    9cee:	01          	nop
    9cef:	01          	nop
    9cf0:	01          	nop
    9cf1:	f7 10 42    	stab	0x1042
    9cf4:	01          	nop
    9cf5:	01          	nop
    9cf6:	01          	nop
    9cf7:	01          	nop
    9cf8:	01          	nop
    9cf9:	01          	nop
    9cfa:	01          	nop
    9cfb:	86 80       	ldaa	#0x80
    9cfd:	b7 10 2a    	staa	0x102a
    9d00:	39          	rts
    9d01:	48          	asla
    9d02:	24 56       	bcc	0x0x9d5a
    9d04:	86 09       	ldaa	#0x9
    9d06:	b1 01 1b    	cmpa	0x11b
    9d09:	26 14       	bne	0x0x9d1f
    9d0b:	d6 f2       	ldab	*0xf2
    9d0d:	c4 20       	andb	#0x20
    9d0f:	ca 02       	orab	#0x2
    9d11:	d7 f2       	stab	*0xf2
    9d13:	4c          	inca
    9d14:	b7 01 1b    	staa	0x11b
    9d17:	86 80       	ldaa	#0x80
    9d19:	b7 01 1c    	staa	0x11c
    9d1c:	7e a0 5c    	jmp	0xa05c
    9d1f:	20 1a       	bra	0x0x9d3b
    9d21:	4c          	inca
    9d22:	b1 01 1b    	cmpa	0x11b
    9d25:	26 14       	bne	0x0x9d3b
    9d27:	4c          	inca
    9d28:	b7 01 1b    	staa	0x11b
    9d2b:	96 f2       	ldaa	*0xf2
    9d2d:	84 20       	anda	#0x20
    9d2f:	8a 04       	oraa	#0x4
    9d31:	97 f2       	staa	*0xf2
    9d33:	86 80       	ldaa	#0x80
    9d35:	b7 01 1c    	staa	0x11c
    9d38:	7e a0 5c    	jmp	0xa05c
    9d3b:	86 09       	ldaa	#0x9
    9d3d:	b7 01 1b    	staa	0x11b
    9d40:	86 02       	ldaa	#0x2
    9d42:	97 ff       	staa	*0xff
    9d44:	bd a0 ff    	jsr	0xa0ff
    9d47:	96 f2       	ldaa	*0xf2
    9d49:	84 20       	anda	#0x20
    9d4b:	8a 01       	oraa	#0x1
    9d4d:	97 f2       	staa	*0xf2
    9d4f:	14 f5 10    	bset	*0xf5, #0x10
    9d52:	86 80       	ldaa	#0x80
    9d54:	b7 01 1c    	staa	0x11c
    9d57:	7e a0 5c    	jmp	0xa05c
    9d5a:	48          	asla
    9d5b:	24 55       	bcc	0x0x9db2
    9d5d:	96 f2       	ldaa	*0xf2
    9d5f:	84 20       	anda	#0x20
    9d61:	c6 1d       	ldab	#0x1d
    9d63:	f1 01 1b    	cmpb	0x11b
    9d66:	27 1f       	beq	0x0x9d87
    9d68:	5c          	incb
    9d69:	f1 01 1b    	cmpb	0x11b
    9d6c:	27 1e       	beq	0x0x9d8c
    9d6e:	5c          	incb
    9d6f:	f1 01 1b    	cmpb	0x11b
    9d72:	27 1d       	beq	0x0x9d91
    9d74:	bd a0 ff    	jsr	0xa0ff
    9d77:	86 08       	ldaa	#0x8
    9d79:	9a f5       	oraa	*0xf5
    9d7b:	97 f5       	staa	*0xf5
    9d7d:	96 f2       	ldaa	*0xf2
    9d7f:	84 20       	anda	#0x20
    9d81:	8a 01       	oraa	#0x1
    9d83:	c6 1d       	ldab	#0x1d
    9d85:	20 1a       	bra	0x0x9da1
    9d87:	5c          	incb
    9d88:	8a 02       	oraa	#0x2
    9d8a:	20 15       	bra	0x0x9da1
    9d8c:	5c          	incb
    9d8d:	8a 04       	oraa	#0x4
    9d8f:	20 10       	bra	0x0x9da1
    9d91:	8f          	xgdx
    9d92:	12 d1 80 07 	brset	*0xd1, #0x80, 0x0x9d9d
    9d96:	8f          	xgdx
    9d97:	c6 1d       	ldab	#0x1d
    9d99:	8a 01       	oraa	#0x1
    9d9b:	20 04       	bra	0x0x9da1
    9d9d:	8f          	xgdx
    9d9e:	5c          	incb
    9d9f:	8a 08       	oraa	#0x8
    9da1:	97 f2       	staa	*0xf2
    9da3:	f7 01 1b    	stab	0x11b
    9da6:	86 02       	ldaa	#0x2
    9da8:	97 ff       	staa	*0xff
    9daa:	86 80       	ldaa	#0x80
    9dac:	b7 01 1c    	staa	0x11c
    9daf:	7e a0 5c    	jmp	0xa05c
    9db2:	48          	asla
    9db3:	24 54       	bcc	0x0x9e09
    9db5:	f6 01 1b    	ldab	0x11b
    9db8:	c1 22       	cmpb	#0x22
    9dba:	26 15       	bne	0x0x9dd1
    9dbc:	86 08       	ldaa	#0x8
    9dbe:	b7 01 1b    	staa	0x11b
    9dc1:	96 f2       	ldaa	*0xf2
    9dc3:	84 20       	anda	#0x20
    9dc5:	8a 01       	oraa	#0x1
    9dc7:	97 f2       	staa	*0xf2
    9dc9:	86 80       	ldaa	#0x80
    9dcb:	b7 01 1c    	staa	0x11c
    9dce:	7e a0 5c    	jmp	0xa05c
    9dd1:	c1 08       	cmpb	#0x8
    9dd3:	26 15       	bne	0x0x9dea
    9dd5:	86 22       	ldaa	#0x22
    9dd7:	b7 01 1b    	staa	0x11b
    9dda:	96 f2       	ldaa	*0xf2
    9ddc:	84 20       	anda	#0x20
    9dde:	8a 02       	oraa	#0x2
    9de0:	97 f2       	staa	*0xf2
    9de2:	86 80       	ldaa	#0x80
    9de4:	b7 01 1c    	staa	0x11c
    9de7:	7e a0 5c    	jmp	0xa05c
    9dea:	bd a0 ff    	jsr	0xa0ff
    9ded:	86 08       	ldaa	#0x8
    9def:	b7 01 1b    	staa	0x11b
    9df2:	14 f5 04    	bset	*0xf5, #0x04
    9df5:	86 02       	ldaa	#0x2
    9df7:	97 ff       	staa	*0xff
    9df9:	96 f2       	ldaa	*0xf2
    9dfb:	84 20       	anda	#0x20
    9dfd:	8a 01       	oraa	#0x1
    9dff:	97 f2       	staa	*0xf2
    9e01:	86 80       	ldaa	#0x80
    9e03:	b7 01 1c    	staa	0x11c
    9e06:	7e a0 5c    	jmp	0xa05c
    9e09:	48          	asla
    9e0a:	24 41       	bcc	0x0x9e4d
    9e0c:	d6 f9       	ldab	*0xf9
    9e0e:	c1 04       	cmpb	#0x4
    9e10:	26 38       	bne	0x0x9e4a
    9e12:	f6 01 1b    	ldab	0x11b
    9e15:	c1 29       	cmpb	#0x29
    9e17:	26 15       	bne	0x0x9e2e
    9e19:	86 2a       	ldaa	#0x2a
    9e1b:	b7 01 1b    	staa	0x11b
    9e1e:	96 f2       	ldaa	*0xf2
    9e20:	84 20       	anda	#0x20
    9e22:	8a 02       	oraa	#0x2
    9e24:	97 f2       	staa	*0xf2
    9e26:	86 80       	ldaa	#0x80
    9e28:	b7 01 1c    	staa	0x11c
    9e2b:	7e a0 5c    	jmp	0xa05c
    9e2e:	86 29       	ldaa	#0x29
    9e30:	b7 01 1b    	staa	0x11b
    9e33:	86 02       	ldaa	#0x2
    9e35:	97 ff       	staa	*0xff
    9e37:	bd a0 ff    	jsr	0xa0ff
    9e3a:	96 f2       	ldaa	*0xf2
    9e3c:	84 20       	anda	#0x20
    9e3e:	8a 01       	oraa	#0x1
    9e40:	97 f2       	staa	*0xf2
    9e42:	14 f5 02    	bset	*0xf5, #0x02
    9e45:	86 80       	ldaa	#0x80
    9e47:	b7 01 1c    	staa	0x11c
    9e4a:	7e a0 5c    	jmp	0xa05c
    9e4d:	d6 f9       	ldab	*0xf9
    9e4f:	c1 04       	cmpb	#0x4
    9e51:	27 03       	beq	0x0x9e56
    9e53:	7e a0 5c    	jmp	0xa05c
    9e56:	f6 01 1b    	ldab	0x11b
    9e59:	c1 0e       	cmpb	#0xe
    9e5b:	26 21       	bne	0x0x9e7e
    9e5d:	b6 50 00    	ldaa	0x5000
    9e60:	27 04       	beq	0x0x9e66
    9e62:	81 04       	cmpa	#0x4
    9e64:	23 03       	bls	0x0x9e69
    9e66:	7e a0 5c    	jmp	0xa05c
    9e69:	86 0f       	ldaa	#0xf
    9e6b:	b7 01 1b    	staa	0x11b
    9e6e:	96 f2       	ldaa	*0xf2
    9e70:	84 20       	anda	#0x20
    9e72:	8a 02       	oraa	#0x2
    9e74:	97 f2       	staa	*0xf2
    9e76:	86 80       	ldaa	#0x80
    9e78:	b7 01 1c    	staa	0x11c
    9e7b:	7e a0 5c    	jmp	0xa05c
    9e7e:	86 0e       	ldaa	#0xe
    9e80:	b7 01 1b    	staa	0x11b
    9e83:	86 02       	ldaa	#0x2
    9e85:	97 ff       	staa	*0xff
    9e87:	bd a0 ff    	jsr	0xa0ff
    9e8a:	96 f2       	ldaa	*0xf2
    9e8c:	84 20       	anda	#0x20
    9e8e:	8a 01       	oraa	#0x1
    9e90:	97 f2       	staa	*0xf2
    9e92:	14 f5 01    	bset	*0xf5, #0x01
    9e95:	86 80       	ldaa	#0x80
    9e97:	b7 01 1c    	staa	0x11c
    9e9a:	7e a0 5c    	jmp	0xa05c
    9e9d:	86 02       	ldaa	#0x2
    9e9f:	b7 10 44    	staa	0x1044
    9ea2:	01          	nop
    9ea3:	01          	nop
    9ea4:	b6 10 45    	ldaa	0x1045
    9ea7:	b1 01 13    	cmpa	0x113
    9eaa:	26 03       	bne	0x0x9eaf
    9eac:	7e 9f 19    	jmp	0x9f19
    9eaf:	16          	tab
    9eb0:	b8 01 13    	eora	0x113
    9eb3:	f7 01 13    	stab	0x113
    9eb6:	b4 01 13    	anda	0x113
    9eb9:	26 03       	bne	0x0x9ebe
    9ebb:	7e 9f 19    	jmp	0x9f19
    9ebe:	44          	lsra
    9ebf:	24 39       	bcc	0x0x9efa
    9ec1:	86 0c       	ldaa	#0xc
    9ec3:	b1 01 1b    	cmpa	0x11b
    9ec6:	27 1d       	beq	0x0x9ee5
    9ec8:	b7 01 1b    	staa	0x11b
    9ecb:	bd a0 ff    	jsr	0xa0ff
    9ece:	14 f6 05    	bset	*0xf6, #0x05
    9ed1:	86 02       	ldaa	#0x2
    9ed3:	97 ff       	staa	*0xff
    9ed5:	86 80       	ldaa	#0x80
    9ed7:	b7 01 1c    	staa	0x11c
    9eda:	96 f2       	ldaa	*0xf2
    9edc:	84 20       	anda	#0x20
    9ede:	8a 01       	oraa	#0x1
    9ee0:	97 f2       	staa	*0xf2
    9ee2:	7e a0 5c    	jmp	0xa05c
    9ee5:	96 f6       	ldaa	*0xf6
    9ee7:	84 fc       	anda	#0xfc
    9ee9:	48          	asla
    9eea:	24 02       	bcc	0x0x9eee
    9eec:	86 04       	ldaa	#0x4
    9eee:	8a 01       	oraa	#0x1
    9ef0:	97 f6       	staa	*0xf6
    9ef2:	86 80       	ldaa	#0x80
    9ef4:	b7 01 1c    	staa	0x11c
    9ef7:	7e a0 5c    	jmp	0xa05c
    9efa:	bd a0 ff    	jsr	0xa0ff
    9efd:	14 f6 02    	bset	*0xf6, #0x02
    9f00:	96 f2       	ldaa	*0xf2
    9f02:	84 20       	anda	#0x20
    9f04:	8a 01       	oraa	#0x1
    9f06:	97 f2       	staa	*0xf2
    9f08:	86 0d       	ldaa	#0xd
    9f0a:	b7 01 1b    	staa	0x11b
    9f0d:	86 02       	ldaa	#0x2
    9f0f:	97 ff       	staa	*0xff
    9f11:	86 80       	ldaa	#0x80
    9f13:	b7 01 1c    	staa	0x11c
    9f16:	7e a0 5c    	jmp	0xa05c
    9f19:	86 01       	ldaa	#0x1
    9f1b:	b7 10 44    	staa	0x1044
    9f1e:	01          	nop
    9f1f:	01          	nop
    9f20:	b6 10 45    	ldaa	0x1045
    9f23:	b1 01 14    	cmpa	0x114
    9f26:	26 03       	bne	0x0x9f2b
    9f28:	7e a0 5c    	jmp	0xa05c
    9f2b:	16          	tab
    9f2c:	b8 01 14    	eora	0x114
    9f2f:	f7 01 14    	stab	0x114
    9f32:	b4 01 14    	anda	0x114
    9f35:	26 03       	bne	0x0x9f3a
    9f37:	7e a0 5c    	jmp	0xa05c
    9f3a:	48          	asla
    9f3b:	24 20       	bcc	0x0x9f5d
    9f3d:	86 80       	ldaa	#0x80
    9f3f:	98 f7       	eora	*0xf7
    9f41:	97 f7       	staa	*0xf7
    9f43:	86 10       	ldaa	#0x10
    9f45:	98 41       	eora	*0x41
    9f47:	97 41       	staa	*0x41
    9f49:	c6 41       	ldab	#0x41
    9f4b:	bd af d2    	jsr	0xafd2
    9f4e:	4f          	clra
    9f4f:	c6 76       	ldab	#0x76
    9f51:	13 41 10 02 	brclr	*0x41, #0x10, 0x0x9f57
    9f55:	86 7f       	ldaa	#0x7f
    9f57:	bd b0 5c    	jsr	0xb05c
    9f5a:	7e a0 4c    	jmp	0xa04c
    9f5d:	48          	asla
    9f5e:	24 20       	bcc	0x0x9f80
    9f60:	86 40       	ldaa	#0x40
    9f62:	98 f7       	eora	*0xf7
    9f64:	97 f7       	staa	*0xf7
    9f66:	86 40       	ldaa	#0x40
    9f68:	98 44       	eora	*0x44
    9f6a:	97 44       	staa	*0x44
    9f6c:	c6 44       	ldab	#0x44
    9f6e:	bd af d2    	jsr	0xafd2
    9f71:	4f          	clra
    9f72:	c6 3f       	ldab	#0x3f
    9f74:	13 44 40 02 	brclr	*0x44, #0x40, 0x0x9f7a
    9f78:	86 7f       	ldaa	#0x7f
    9f7a:	bd b0 5c    	jsr	0xb05c
    9f7d:	7e a0 4c    	jmp	0xa04c
    9f80:	48          	asla
    9f81:	24 20       	bcc	0x0x9fa3
    9f83:	86 20       	ldaa	#0x20
    9f85:	98 f7       	eora	*0xf7
    9f87:	97 f7       	staa	*0xf7
    9f89:	86 20       	ldaa	#0x20
    9f8b:	98 44       	eora	*0x44
    9f8d:	97 44       	staa	*0x44
    9f8f:	c6 44       	ldab	#0x44
    9f91:	bd af d2    	jsr	0xafd2
    9f94:	4f          	clra
    9f95:	c6 3e       	ldab	#0x3e
    9f97:	13 44 20 02 	brclr	*0x44, #0x20, 0x0x9f9d
    9f9b:	86 7f       	ldaa	#0x7f
    9f9d:	bd b0 5c    	jsr	0xb05c
    9fa0:	7e a0 4c    	jmp	0xa04c
    9fa3:	48          	asla
    9fa4:	24 20       	bcc	0x0x9fc6
    9fa6:	86 10       	ldaa	#0x10
    9fa8:	98 f7       	eora	*0xf7
    9faa:	97 f7       	staa	*0xf7
    9fac:	86 10       	ldaa	#0x10
    9fae:	98 44       	eora	*0x44
    9fb0:	97 44       	staa	*0x44
    9fb2:	c6 44       	ldab	#0x44
    9fb4:	bd af d2    	jsr	0xafd2
    9fb7:	4f          	clra
    9fb8:	c6 3d       	ldab	#0x3d
    9fba:	13 44 10 02 	brclr	*0x44, #0x10, 0x0x9fc0
    9fbe:	86 7f       	ldaa	#0x7f
    9fc0:	bd b0 5c    	jsr	0xb05c
    9fc3:	7e a0 4c    	jmp	0xa04c
    9fc6:	48          	asla
    9fc7:	24 20       	bcc	0x0x9fe9
    9fc9:	86 08       	ldaa	#0x8
    9fcb:	98 f7       	eora	*0xf7
    9fcd:	97 f7       	staa	*0xf7
    9fcf:	86 08       	ldaa	#0x8
    9fd1:	98 44       	eora	*0x44
    9fd3:	97 44       	staa	*0x44
    9fd5:	c6 44       	ldab	#0x44
    9fd7:	bd af d2    	jsr	0xafd2
    9fda:	4f          	clra
    9fdb:	c6 3a       	ldab	#0x3a
    9fdd:	13 44 08 02 	brclr	*0x44, #0x08, 0x0x9fe3
    9fe1:	86 7f       	ldaa	#0x7f
    9fe3:	bd b0 5c    	jsr	0xb05c
    9fe6:	7e a0 4c    	jmp	0xa04c
    9fe9:	48          	asla
    9fea:	24 20       	bcc	0x0xa00c
    9fec:	86 04       	ldaa	#0x4
    9fee:	98 f7       	eora	*0xf7
    9ff0:	97 f7       	staa	*0xf7
    9ff2:	86 04       	ldaa	#0x4
    9ff4:	98 44       	eora	*0x44
    9ff6:	97 44       	staa	*0x44
    9ff8:	c6 44       	ldab	#0x44
    9ffa:	bd af d2    	jsr	0xafd2
    9ffd:	4f          	clra
    9ffe:	c6 39       	ldab	#0x39
    a000:	13 44 04 02 	brclr	*0x44, #0x04, 0x0xa006
    a004:	86 7f       	ldaa	#0x7f
    a006:	bd b0 5c    	jsr	0xb05c
    a009:	7e a0 4c    	jmp	0xa04c
    a00c:	48          	asla
    a00d:	24 20       	bcc	0x0xa02f
    a00f:	86 02       	ldaa	#0x2
    a011:	98 f7       	eora	*0xf7
    a013:	97 f7       	staa	*0xf7
    a015:	86 02       	ldaa	#0x2
    a017:	98 44       	eora	*0x44
    a019:	97 44       	staa	*0x44
    a01b:	c6 44       	ldab	#0x44
    a01d:	bd af d2    	jsr	0xafd2
    a020:	4f          	clra
    a021:	c6 38       	ldab	#0x38
    a023:	13 44 02 02 	brclr	*0x44, #0x02, 0x0xa029
    a027:	86 7f       	ldaa	#0x7f
    a029:	bd b0 5c    	jsr	0xb05c
    a02c:	7e a0 4c    	jmp	0xa04c
    a02f:	86 01       	ldaa	#0x1
    a031:	98 f7       	eora	*0xf7
    a033:	97 f7       	staa	*0xf7
    a035:	86 01       	ldaa	#0x1
    a037:	98 44       	eora	*0x44
    a039:	97 44       	staa	*0x44
    a03b:	c6 44       	ldab	#0x44
    a03d:	bd af d2    	jsr	0xafd2
    a040:	4f          	clra
    a041:	c6 37       	ldab	#0x37
    a043:	13 44 01 02 	brclr	*0x44, #0x01, 0x0xa049
    a047:	86 7f       	ldaa	#0x7f
    a049:	bd b0 5c    	jsr	0xb05c
    a04c:	7d 00 fb    	tst	0xfb
    a04f:	26 0b       	bne	0x0xa05c
    a051:	14 fb 01    	bset	*0xfb, #0x01
    a054:	14 f2 20    	bset	*0xf2, #0x20
    a057:	86 ff       	ldaa	#0xff
    a059:	b7 01 66    	staa	0x166
    a05c:	86 40       	ldaa	#0x40
    a05e:	b7 10 23    	staa	0x1023
    a061:	fc 10 0e    	ldd	0x100e
    a064:	c3 27 10    	addd	#0x2710
    a067:	fd 10 18    	std	0x1018
    a06a:	fe 01 c0    	ldx	0x1c0
    a06d:	bc 01 c2    	cpx	0x1c2
    a070:	26 03       	bne	0x0xa075
    a072:	7e aa 04    	jmp	0xaa04
    a075:	18 fe 01 6c 	ldy	0x16c
    a079:	18 3c       	pshy
    a07b:	7e 91 d5    	jmp	0x91d5
    a07e:	96 fa       	ldaa	*0xfa
    a080:	4c          	inca
    a081:	f6 01 11    	ldab	0x111
    a084:	c5 04       	bitb	#0x4
    a086:	27 02       	beq	0x0xa08a
    a088:	8b 09       	adda	#0x9
    a08a:	84 7f       	anda	#0x7f
    a08c:	81 51       	cmpa	#0x51
    a08e:	26 04       	bne	0x0xa094
    a090:	86 53       	ldaa	#0x53
    a092:	20 06       	bra	0x0xa09a
    a094:	81 52       	cmpa	#0x52
    a096:	26 02       	bne	0x0xa09a
    a098:	86 53       	ldaa	#0x53
    a09a:	97 fa       	staa	*0xfa
    a09c:	bd ab 4b    	jsr	0xab4b
    a09f:	b7 7f f7    	staa	0x7ff7
    a0a2:	bd ab 71    	jsr	0xab71
    a0a5:	7c 01 7e    	inc	0x17e
    a0a8:	39          	rts
    a0a9:	96 fa       	ldaa	*0xfa
    a0ab:	4a          	deca
    a0ac:	f6 01 11    	ldab	0x111
    a0af:	c5 04       	bitb	#0x4
    a0b1:	27 02       	beq	0x0xa0b5
    a0b3:	80 09       	suba	#0x9
    a0b5:	84 7f       	anda	#0x7f
    a0b7:	81 52       	cmpa	#0x52
    a0b9:	26 04       	bne	0x0xa0bf
    a0bb:	86 50       	ldaa	#0x50
    a0bd:	20 05       	bra	0x0xa0c4
    a0bf:	81 51       	cmpa	#0x51
    a0c1:	26 01       	bne	0x0xa0c4
    a0c3:	4a          	deca
    a0c4:	97 fa       	staa	*0xfa
    a0c6:	bd ab 4b    	jsr	0xab4b
    a0c9:	b7 7f f7    	staa	0x7ff7
    a0cc:	bd ab 71    	jsr	0xab71
    a0cf:	7c 01 7e    	inc	0x17e
    a0d2:	39          	rts
    a0d3:	b6 01 6a    	ldaa	0x16a
    a0d6:	4c          	inca
    a0d7:	84 7f       	anda	#0x7f
    a0d9:	b7 01 6a    	staa	0x16a
    a0dc:	bd ab 4b    	jsr	0xab4b
    a0df:	b7 7f f7    	staa	0x7ff7
    a0e2:	bd ab 71    	jsr	0xab71
    a0e5:	7c 01 7e    	inc	0x17e
    a0e8:	39          	rts
    a0e9:	b6 01 6a    	ldaa	0x16a
    a0ec:	4a          	deca
    a0ed:	84 7f       	anda	#0x7f
    a0ef:	b7 01 6a    	staa	0x16a
    a0f2:	bd ab 4b    	jsr	0xab4b
    a0f5:	b7 7f f7    	staa	0x7ff7
    a0f8:	bd ab 71    	jsr	0xab71
    a0fb:	7c 01 7e    	inc	0x17e
    a0fe:	39          	rts
    a0ff:	86 17       	ldaa	#0x17
    a101:	94 f3       	anda	*0xf3
    a103:	97 f3       	staa	*0xf3
    a105:	86 e4       	ldaa	#0xe4
    a107:	94 f4       	anda	*0xf4
    a109:	97 f4       	staa	*0xf4
    a10b:	86 20       	ldaa	#0x20
    a10d:	94 f5       	anda	*0xf5
    a10f:	97 f5       	staa	*0xf5
    a111:	86 00       	ldaa	#0x0
    a113:	97 f6       	staa	*0xf6
    a115:	39          	rts
    a116:	96 f9       	ldaa	*0xf9
    a118:	81 01       	cmpa	#0x1
    a11a:	22 1f       	bhi	0x0xa13b
    a11c:	4f          	clra
    a11d:	b7 10 22    	staa	0x1022
    a120:	b6 10 2d    	ldaa	0x102d
    a123:	36          	psha
    a124:	84 7f       	anda	#0x7f
    a126:	b7 10 2d    	staa	0x102d
    a129:	96 f9       	ldaa	*0xf9
    a12b:	d6 fa       	ldab	*0xfa
    a12d:	bd 79 00    	jsr	0x7900
    a130:	32          	pula
    a131:	b7 10 2d    	staa	0x102d
    a134:	86 80       	ldaa	#0x80
    a136:	b7 10 22    	staa	0x1022
    a139:	20 1c       	bra	0x0xa157
    a13b:	96 fa       	ldaa	*0xfa
    a13d:	c6 b0       	ldab	#0xb0
    a13f:	3d          	mul
    a140:	c3 20 00    	addd	#0x2000
    a143:	8f          	xgdx
    a144:	18 ce 00 20 	ldy	#0x20
    a148:	c6 b0       	ldab	#0xb0
    a14a:	a6 00       	ldaa	0x0,x
    a14c:	84 7f       	anda	#0x7f
    a14e:	18 a7 00    	staa	0x0,y
    a151:	08          	inx
    a152:	18 08       	iny
    a154:	5a          	decb
    a155:	26 f3       	bne	0x0xa14a
    a157:	96 41       	ldaa	*0x41
    a159:	84 10       	anda	#0x10
    a15b:	48          	asla
    a15c:	48          	asla
    a15d:	48          	asla
    a15e:	9a 44       	oraa	*0x44
    a160:	97 f7       	staa	*0xf7
    a162:	96 73       	ldaa	*0x73
    a164:	81 02       	cmpa	#0x2
    a166:	23 03       	bls	0x0xa16b
    a168:	4f          	clra
    a169:	97 73       	staa	*0x73
    a16b:	c6 01       	ldab	#0x1
    a16d:	4d          	tsta
    a16e:	27 05       	beq	0x0xa175
    a170:	58          	aslb
    a171:	4a          	deca
    a172:	27 01       	beq	0x0xa175
    a174:	58          	aslb
    a175:	d7 f3       	stab	*0xf3
    a177:	96 4a       	ldaa	*0x4a
    a179:	48          	asla
    a17a:	48          	asla
    a17b:	48          	asla
    a17c:	48          	asla
    a17d:	48          	asla
    a17e:	97 f4       	staa	*0xf4
    a180:	7d 00 d0    	tst	0xd0
    a183:	27 05       	beq	0x0xa18a
    a185:	7f 00 d0    	clr	0xd0
    a188:	20 08       	bra	0x0xa192
    a18a:	7d 10 29    	tst	0x1029
    a18d:	2a fb       	bpl	0x0xa18a
    a18f:	b6 10 2a    	ldaa	0x102a
    a192:	01          	nop
    a193:	01          	nop
    a194:	01          	nop
    a195:	01          	nop
    a196:	5f          	clrb
    a197:	f7 10 42    	stab	0x1042
    a19a:	7c 00 d0    	inc	0xd0
    a19d:	96 6a       	ldaa	*0x6a
    a19f:	84 20       	anda	#0x20
    a1a1:	d6 f5       	ldab	*0xf5
    a1a3:	c4 20       	andb	#0x20
    a1a5:	d7 f5       	stab	*0xf5
    a1a7:	36          	psha
    a1a8:	98 f5       	eora	*0xf5
    a1aa:	26 05       	bne	0x0xa1b1
    a1ac:	32          	pula
    a1ad:	97 f5       	staa	*0xf5
    a1af:	20 24       	bra	0x0xa1d5
    a1b1:	86 80       	ldaa	#0x80
    a1b3:	b7 10 2a    	staa	0x102a
    a1b6:	cc 01 00    	ldd	#0x100
    a1b9:	fd 01 08    	std	0x108
    a1bc:	4f          	clra
    a1bd:	97 d0       	staa	*0xd0
    a1bf:	97 f8       	staa	*0xf8
    a1c1:	fd 01 00    	std	0x100
    a1c4:	fd 01 02    	std	0x102
    a1c7:	fd 01 04    	std	0x104
    a1ca:	fd 01 06    	std	0x106
    a1cd:	32          	pula
    a1ce:	97 f5       	staa	*0xf5
    a1d0:	86 01       	ldaa	#0x1
    a1d2:	b7 01 09    	staa	0x109
    a1d5:	c6 88       	ldab	#0x88
    a1d7:	ce 00 20    	ldx	#0x20
    a1da:	7d 00 d0    	tst	0xd0
    a1dd:	27 05       	beq	0x0xa1e4
    a1df:	7f 00 d0    	clr	0xd0
    a1e2:	20 08       	bra	0x0xa1ec
    a1e4:	7d 10 29    	tst	0x1029
    a1e7:	2a fb       	bpl	0x0xa1e4
    a1e9:	b6 10 2a    	ldaa	0x102a
    a1ec:	86 82       	ldaa	#0x82
    a1ee:	b7 10 2a    	staa	0x102a
    a1f1:	7d 10 29    	tst	0x1029
    a1f4:	2a fb       	bpl	0x0xa1f1
    a1f6:	b6 10 2a    	ldaa	0x102a
    a1f9:	a6 00       	ldaa	0x0,x
    a1fb:	b7 10 2a    	staa	0x102a
    a1fe:	08          	inx
    a1ff:	5a          	decb
    a200:	26 ef       	bne	0x0xa1f1
    a202:	c6 fe       	ldab	#0xfe
    a204:	7d 10 29    	tst	0x1029
    a207:	2a fb       	bpl	0x0xa204
    a209:	b6 10 2a    	ldaa	0x102a
    a20c:	01          	nop
    a20d:	01          	nop
    a20e:	01          	nop
    a20f:	01          	nop
    a210:	f7 10 42    	stab	0x1042
    a213:	01          	nop
    a214:	01          	nop
    a215:	01          	nop
    a216:	01          	nop
    a217:	01          	nop
    a218:	01          	nop
    a219:	01          	nop
    a21a:	a6 00       	ldaa	0x0,x
    a21c:	b7 10 2a    	staa	0x102a
    a21f:	7d 10 29    	tst	0x1029
    a222:	2a fb       	bpl	0x0xa21f
    a224:	b6 10 2a    	ldaa	0x102a
    a227:	01          	nop
    a228:	01          	nop
    a229:	01          	nop
    a22a:	01          	nop
    a22b:	a6 08       	ldaa	0x8,x
    a22d:	b7 10 2a    	staa	0x102a
    a230:	7d 10 29    	tst	0x1029
    a233:	2a fb       	bpl	0x0xa230
    a235:	b6 10 2a    	ldaa	0x102a
    a238:	01          	nop
    a239:	01          	nop
    a23a:	a6 10       	ldaa	0x10,x
    a23c:	b7 10 2a    	staa	0x102a
    a23f:	08          	inx
    a240:	0d          	sec
    a241:	59          	rolb
    a242:	24 18       	bcc	0x0xa25c
    a244:	7d 10 29    	tst	0x1029
    a247:	2a fb       	bpl	0x0xa244
    a249:	b6 10 2a    	ldaa	0x102a
    a24c:	01          	nop
    a24d:	01          	nop
    a24e:	01          	nop
    a24f:	01          	nop
    a250:	f7 10 42    	stab	0x1042
    a253:	01          	nop
    a254:	01          	nop
    a255:	01          	nop
    a256:	01          	nop
    a257:	01          	nop
    a258:	01          	nop
    a259:	01          	nop
    a25a:	20 be       	bra	0x0xa21a
    a25c:	7d 10 29    	tst	0x1029
    a25f:	2a fb       	bpl	0x0xa25c
    a261:	b6 10 2a    	ldaa	0x102a
    a264:	4f          	clra
    a265:	01          	nop
    a266:	01          	nop
    a267:	01          	nop
    a268:	01          	nop
    a269:	b7 10 42    	staa	0x1042
    a26c:	01          	nop
    a26d:	01          	nop
    a26e:	01          	nop
    a26f:	01          	nop
    a270:	01          	nop
    a271:	01          	nop
    a272:	01          	nop
    a273:	b6 01 6e    	ldaa	0x16e
    a276:	b7 10 2a    	staa	0x102a
    a279:	7d 10 29    	tst	0x1029
    a27c:	2a fb       	bpl	0x0xa279
    a27e:	b6 10 2a    	ldaa	0x102a
    a281:	86 89       	ldaa	#0x89
    a283:	b7 10 2a    	staa	0x102a
    a286:	7d 10 29    	tst	0x1029
    a289:	2a fb       	bpl	0x0xa286
    a28b:	b6 10 2a    	ldaa	0x102a
    a28e:	86 83       	ldaa	#0x83
    a290:	b7 10 2a    	staa	0x102a
    a293:	7d 10 29    	tst	0x1029
    a296:	2a fb       	bpl	0x0xa293
    a298:	b6 10 2a    	ldaa	0x102a
    a29b:	86 8c       	ldaa	#0x8c
    a29d:	b7 10 2a    	staa	0x102a
    a2a0:	7d 10 29    	tst	0x1029
    a2a3:	2a fb       	bpl	0x0xa2a0
    a2a5:	b6 10 2a    	ldaa	0x102a
    a2a8:	96 d6       	ldaa	*0xd6
    a2aa:	b7 10 2a    	staa	0x102a
    a2ad:	7d 10 29    	tst	0x1029
    a2b0:	2a fb       	bpl	0x0xa2ad
    a2b2:	01          	nop
    a2b3:	01          	nop
    a2b4:	01          	nop
    a2b5:	01          	nop
    a2b6:	86 ff       	ldaa	#0xff
    a2b8:	b7 10 42    	staa	0x1042
    a2bb:	7c 00 d0    	inc	0xd0
    a2be:	bd ab 4b    	jsr	0xab4b
    a2c1:	c6 20       	ldab	#0x20
    a2c3:	4f          	clra
    a2c4:	ce 1f 20    	ldx	#0x1f20
    a2c7:	a7 00       	staa	0x0,x
    a2c9:	08          	inx
    a2ca:	5a          	decb
    a2cb:	26 fa       	bne	0x0xa2c7
    a2cd:	7e ab 71    	jmp	0xab71
    a2d0:	c6 b0       	ldab	#0xb0
    a2d2:	3d          	mul
    a2d3:	c3 20 00    	addd	#0x2000
    a2d6:	8f          	xgdx
    a2d7:	18 ce 00 20 	ldy	#0x20
    a2db:	c6 b0       	ldab	#0xb0
    a2dd:	a6 00       	ldaa	0x0,x
    a2df:	84 7f       	anda	#0x7f
    a2e1:	18 a7 00    	staa	0x0,y
    a2e4:	08          	inx
    a2e5:	18 08       	iny
    a2e7:	5a          	decb
    a2e8:	26 f3       	bne	0x0xa2dd
    a2ea:	39          	rts
    a2eb:	96 41       	ldaa	*0x41
    a2ed:	84 10       	anda	#0x10
    a2ef:	48          	asla
    a2f0:	48          	asla
    a2f1:	48          	asla
    a2f2:	9a 44       	oraa	*0x44
    a2f4:	97 f7       	staa	*0xf7
    a2f6:	96 73       	ldaa	*0x73
    a2f8:	81 02       	cmpa	#0x2
    a2fa:	23 03       	bls	0x0xa2ff
    a2fc:	4f          	clra
    a2fd:	97 73       	staa	*0x73
    a2ff:	c6 01       	ldab	#0x1
    a301:	4d          	tsta
    a302:	27 05       	beq	0x0xa309
    a304:	58          	aslb
    a305:	4a          	deca
    a306:	27 01       	beq	0x0xa309
    a308:	58          	aslb
    a309:	d7 f3       	stab	*0xf3
    a30b:	96 4a       	ldaa	*0x4a
    a30d:	48          	asla
    a30e:	48          	asla
    a30f:	48          	asla
    a310:	48          	asla
    a311:	48          	asla
    a312:	97 f4       	staa	*0xf4
    a314:	39          	rts
    a315:	b6 01 6a    	ldaa	0x16a
    a318:	c6 50       	ldab	#0x50
    a31a:	3d          	mul
    a31b:	c3 20 00    	addd	#0x2000
    a31e:	8f          	xgdx
    a31f:	18 ce 50 00 	ldy	#0x5000
    a323:	c6 50       	ldab	#0x50
    a325:	a6 00       	ldaa	0x0,x
    a327:	84 7f       	anda	#0x7f
    a329:	18 a7 00    	staa	0x0,y
    a32c:	08          	inx
    a32d:	18 08       	iny
    a32f:	5a          	decb
    a330:	26 f3       	bne	0x0xa325
    a332:	18 ce 50 50 	ldy	#0x5050
    a336:	ce dd 6b    	ldx	#0xdd6b
    a339:	7f 01 6b    	clr	0x16b
    a33c:	7f 01 7d    	clr	0x17d
    a33f:	f6 50 00    	ldab	0x5000
    a342:	c1 05       	cmpb	#0x5
    a344:	22 03       	bhi	0x0xa349
    a346:	7e a3 cb    	jmp	0xa3cb
    a349:	ce 50 01    	ldx	#0x5001
    a34c:	b6 01 6b    	ldaa	0x16b
    a34f:	c6 04       	ldab	#0x4
    a351:	3d          	mul
    a352:	3a          	abx
    a353:	e6 03       	ldab	0x3,x
    a355:	c4 01       	andb	#0x1
    a357:	37          	pshb
    a358:	e6 02       	ldab	0x2,x
    a35a:	c4 3f       	andb	#0x3f
    a35c:	37          	pshb
    a35d:	ce dd b3    	ldx	#0xddb3
    a360:	3a          	abx
    a361:	b6 01 7d    	ldaa	0x17d
    a364:	36          	psha
    a365:	fb 01 7d    	addb	0x17d
    a368:	f7 01 7d    	stab	0x17d
    a36b:	37          	pshb
    a36c:	16          	tab
    a36d:	a6 00       	ldaa	0x0,x
    a36f:	5a          	decb
    a370:	2b 03       	bmi	0x0xa375
    a372:	48          	asla
    a373:	20 fa       	bra	0x0xa36f
    a375:	18 a7 00    	staa	0x0,y
    a378:	ce 51 00    	ldx	#0x5100
    a37b:	f6 01 6b    	ldab	0x16b
    a37e:	86 10       	ldaa	#0x10
    a380:	3d          	mul
    a381:	3a          	abx
    a382:	8f          	xgdx
    a383:	37          	pshb
    a384:	8f          	xgdx
    a385:	33          	pulb
    a386:	e7 0b       	stab	0xb,x
    a388:	32          	pula
    a389:	4a          	deca
    a38a:	ab 0b       	adda	0xb,x
    a38c:	a7 09       	staa	0x9,x
    a38e:	32          	pula
    a38f:	ab 0b       	adda	0xb,x
    a391:	a7 08       	staa	0x8,x
    a393:	32          	pula
    a394:	a7 0a       	staa	0xa,x
    a396:	e6 08       	ldab	0x8,x
    a398:	c4 07       	andb	#0x7
    a39a:	3a          	abx
    a39b:	3c          	pshx
    a39c:	ce 51 80    	ldx	#0x5180
    a39f:	f6 01 6b    	ldab	0x16b
    a3a2:	58          	aslb
    a3a3:	3a          	abx
    a3a4:	32          	pula
    a3a5:	33          	pulb
    a3a6:	ed 00       	std	0x0,x
    a3a8:	32          	pula
    a3a9:	18 a7 08    	staa	0x8,y
    a3ac:	18 08       	iny
    a3ae:	7c 01 6b    	inc	0x16b
    a3b1:	d6 f0       	ldab	*0xf0
    a3b3:	5c          	incb
    a3b4:	f1 01 7d    	cmpb	0x17d
    a3b7:	22 90       	bhi	0x0xa349
    a3b9:	f6 01 6b    	ldab	0x16b
    a3bc:	c1 08       	cmpb	#0x8
    a3be:	27 35       	beq	0x0xa3f5
    a3c0:	ce 50 50    	ldx	#0x5050
    a3c3:	3a          	abx
    a3c4:	6f 00       	clr	0x0,x
    a3c6:	7c 01 6b    	inc	0x16b
    a3c9:	20 ee       	bra	0x0xa3b9
    a3cb:	c1 05       	cmpb	#0x5
    a3cd:	25 15       	bcs	0x0xa3e4
    a3cf:	ce dd bc    	ldx	#0xddbc
    a3d2:	5f          	clrb
    a3d3:	96 f0       	ldaa	*0xf0
    a3d5:	81 02       	cmpa	#0x2
    a3d7:	23 0b       	bls	0x0xa3e4
    a3d9:	5c          	incb
    a3da:	81 04       	cmpa	#0x4
    a3dc:	23 06       	bls	0x0xa3e4
    a3de:	5c          	incb
    a3df:	81 06       	cmpa	#0x6
    a3e1:	23 01       	bls	0x0xa3e4
    a3e3:	5c          	incb
    a3e4:	86 08       	ldaa	#0x8
    a3e6:	3d          	mul
    a3e7:	3a          	abx
    a3e8:	c6 08       	ldab	#0x8
    a3ea:	a6 00       	ldaa	0x0,x
    a3ec:	18 a7 00    	staa	0x0,y
    a3ef:	08          	inx
    a3f0:	18 08       	iny
    a3f2:	5a          	decb
    a3f3:	26 f5       	bne	0x0xa3ea
    a3f5:	86 07       	ldaa	#0x7
    a3f7:	b7 01 6b    	staa	0x16b
    a3fa:	ce 50 57    	ldx	#0x5057
    a3fd:	e6 00       	ldab	0x0,x
    a3ff:	26 06       	bne	0x0xa407
    a401:	7a 01 6b    	dec	0x16b
    a404:	09          	dex
    a405:	20 f6       	bra	0x0xa3fd
    a407:	7d 00 d0    	tst	0xd0
    a40a:	27 05       	beq	0x0xa411
    a40c:	7f 00 d0    	clr	0xd0
    a40f:	20 08       	bra	0x0xa419
    a411:	7d 10 29    	tst	0x1029
    a414:	2a fb       	bpl	0x0xa411
    a416:	b6 10 2a    	ldaa	0x102a
    a419:	53          	comb
    a41a:	01          	nop
    a41b:	01          	nop
    a41c:	01          	nop
    a41d:	01          	nop
    a41e:	f7 10 42    	stab	0x1042
    a421:	ce 50 01    	ldx	#0x5001
    a424:	f6 01 6b    	ldab	0x16b
    a427:	86 04       	ldaa	#0x4
    a429:	3d          	mul
    a42a:	3a          	abx
    a42b:	a6 01       	ldaa	0x1,x
    a42d:	36          	psha
    a42e:	86 82       	ldaa	#0x82
    a430:	b7 10 2a    	staa	0x102a
    a433:	a6 00       	ldaa	0x0,x
    a435:	81 01       	cmpa	#0x1
    a437:	22 20       	bhi	0x0xa459
    a439:	33          	pulb
    a43a:	4f          	clra
    a43b:	b7 10 22    	staa	0x1022
    a43e:	b6 10 2d    	ldaa	0x102d
    a441:	36          	psha
    a442:	84 7f       	anda	#0x7f
    a444:	b7 10 2d    	staa	0x102d
    a447:	a6 00       	ldaa	0x0,x
    a449:	3c          	pshx
    a44a:	bd 79 00    	jsr	0x7900
    a44d:	38          	pulx
    a44e:	32          	pula
    a44f:	b7 10 2d    	staa	0x102d
    a452:	86 80       	ldaa	#0x80
    a454:	b7 10 22    	staa	0x1022
    a457:	20 12       	bra	0x0xa46b
    a459:	97 f9       	staa	*0xf9
    a45b:	bd ab 71    	jsr	0xab71
    a45e:	32          	pula
    a45f:	3c          	pshx
    a460:	bd a2 d0    	jsr	0xa2d0
    a463:	38          	pulx
    a464:	c6 04       	ldab	#0x4
    a466:	d7 f9       	stab	*0xf9
    a468:	bd ab 71    	jsr	0xab71
    a46b:	18 ce 00 20 	ldy	#0x20
    a46f:	c6 88       	ldab	#0x88
    a471:	7d 10 29    	tst	0x1029
    a474:	2a fb       	bpl	0x0xa471
    a476:	b6 10 2a    	ldaa	0x102a
    a479:	18 a6 00    	ldaa	0x0,y
    a47c:	b7 10 2a    	staa	0x102a
    a47f:	18 08       	iny
    a481:	5a          	decb
    a482:	26 ed       	bne	0x0xa471
    a484:	7a 01 6b    	dec	0x16b
    a487:	2b 0a       	bmi	0x0xa493
    a489:	f6 01 6b    	ldab	0x16b
    a48c:	ce 50 50    	ldx	#0x5050
    a48f:	3a          	abx
    a490:	7e a3 fd    	jmp	0xa3fd
    a493:	86 04       	ldaa	#0x4
    a495:	97 f9       	staa	*0xf9
    a497:	bd ab 71    	jsr	0xab71
    a49a:	ce 50 28    	ldx	#0x5028
    a49d:	c6 fe       	ldab	#0xfe
    a49f:	7d 10 29    	tst	0x1029
    a4a2:	2a fb       	bpl	0x0xa49f
    a4a4:	b6 10 2a    	ldaa	0x102a
    a4a7:	01          	nop
    a4a8:	01          	nop
    a4a9:	01          	nop
    a4aa:	01          	nop
    a4ab:	f7 10 42    	stab	0x1042
    a4ae:	01          	nop
    a4af:	01          	nop
    a4b0:	01          	nop
    a4b1:	01          	nop
    a4b2:	01          	nop
    a4b3:	01          	nop
    a4b4:	01          	nop
    a4b5:	a6 00       	ldaa	0x0,x
    a4b7:	b7 10 2a    	staa	0x102a
    a4ba:	7d 10 29    	tst	0x1029
    a4bd:	2a fb       	bpl	0x0xa4ba
    a4bf:	b6 10 2a    	ldaa	0x102a
    a4c2:	01          	nop
    a4c3:	01          	nop
    a4c4:	01          	nop
    a4c5:	01          	nop
    a4c6:	a6 08       	ldaa	0x8,x
    a4c8:	b7 10 2a    	staa	0x102a
    a4cb:	7d 10 29    	tst	0x1029
    a4ce:	2a fb       	bpl	0x0xa4cb
    a4d0:	b6 10 2a    	ldaa	0x102a
    a4d3:	01          	nop
    a4d4:	01          	nop
    a4d5:	a6 10       	ldaa	0x10,x
    a4d7:	b7 10 2a    	staa	0x102a
    a4da:	08          	inx
    a4db:	0d          	sec
    a4dc:	59          	rolb
    a4dd:	24 18       	bcc	0x0xa4f7
    a4df:	7d 10 29    	tst	0x1029
    a4e2:	2a fb       	bpl	0x0xa4df
    a4e4:	b6 10 2a    	ldaa	0x102a
    a4e7:	01          	nop
    a4e8:	01          	nop
    a4e9:	01          	nop
    a4ea:	01          	nop
    a4eb:	f7 10 42    	stab	0x1042
    a4ee:	01          	nop
    a4ef:	01          	nop
    a4f0:	01          	nop
    a4f1:	01          	nop
    a4f2:	01          	nop
    a4f3:	01          	nop
    a4f4:	01          	nop
    a4f5:	20 be       	bra	0x0xa4b5
    a4f7:	7d 10 29    	tst	0x1029
    a4fa:	2a fb       	bpl	0x0xa4f7
    a4fc:	b6 10 2a    	ldaa	0x102a
    a4ff:	4f          	clra
    a500:	01          	nop
    a501:	01          	nop
    a502:	01          	nop
    a503:	01          	nop
    a504:	b7 10 42    	staa	0x1042
    a507:	01          	nop
    a508:	01          	nop
    a509:	01          	nop
    a50a:	01          	nop
    a50b:	01          	nop
    a50c:	01          	nop
    a50d:	01          	nop
    a50e:	b6 01 6e    	ldaa	0x16e
    a511:	b7 10 2a    	staa	0x102a
    a514:	7d 10 29    	tst	0x1029
    a517:	2a fb       	bpl	0x0xa514
    a519:	b6 10 2a    	ldaa	0x102a
    a51c:	86 89       	ldaa	#0x89
    a51e:	b7 10 2a    	staa	0x102a
    a521:	bd a2 eb    	jsr	0xa2eb
    a524:	bd ab 4b    	jsr	0xab4b
    a527:	c6 20       	ldab	#0x20
    a529:	4f          	clra
    a52a:	ce 1f 20    	ldx	#0x1f20
    a52d:	a7 00       	staa	0x0,x
    a52f:	08          	inx
    a530:	5a          	decb
    a531:	26 fa       	bne	0x0xa52d
    a533:	86 04       	ldaa	#0x4
    a535:	97 f9       	staa	*0xf9
    a537:	bd ab 71    	jsr	0xab71
    a53a:	86 07       	ldaa	#0x7
    a53c:	b7 01 6b    	staa	0x16b
    a53f:	ce 50 57    	ldx	#0x5057
    a542:	e6 00       	ldab	0x0,x
    a544:	26 06       	bne	0x0xa54c
    a546:	7a 01 6b    	dec	0x16b
    a549:	09          	dex
    a54a:	20 f6       	bra	0x0xa542
    a54c:	7d 00 d0    	tst	0xd0
    a54f:	27 05       	beq	0x0xa556
    a551:	7f 00 d0    	clr	0xd0
    a554:	20 08       	bra	0x0xa55e
    a556:	7d 10 29    	tst	0x1029
    a559:	2a fb       	bpl	0x0xa556
    a55b:	b6 10 2a    	ldaa	0x102a
    a55e:	53          	comb
    a55f:	01          	nop
    a560:	01          	nop
    a561:	01          	nop
    a562:	01          	nop
    a563:	f7 10 42    	stab	0x1042
    a566:	ce 50 03    	ldx	#0x5003
    a569:	f6 01 6b    	ldab	0x16b
    a56c:	86 04       	ldaa	#0x4
    a56e:	3d          	mul
    a56f:	3a          	abx
    a570:	a6 00       	ldaa	0x0,x
    a572:	84 40       	anda	#0x40
    a574:	26 0d       	bne	0x0xa583
    a576:	86 8c       	ldaa	#0x8c
    a578:	b7 10 2a    	staa	0x102a
    a57b:	7d 10 29    	tst	0x1029
    a57e:	2a fb       	bpl	0x0xa57b
    a580:	b6 10 2a    	ldaa	0x102a
    a583:	86 83       	ldaa	#0x83
    a585:	b7 10 2a    	staa	0x102a
    a588:	7d 10 29    	tst	0x1029
    a58b:	2a fb       	bpl	0x0xa588
    a58d:	b6 10 2a    	ldaa	0x102a
    a590:	86 8c       	ldaa	#0x8c
    a592:	b7 10 2a    	staa	0x102a
    a595:	7d 10 29    	tst	0x1029
    a598:	2a fb       	bpl	0x0xa595
    a59a:	b6 10 2a    	ldaa	0x102a
    a59d:	96 d6       	ldaa	*0xd6
    a59f:	f6 50 00    	ldab	0x5000
    a5a2:	c1 06       	cmpb	#0x6
    a5a4:	27 06       	beq	0x0xa5ac
    a5a6:	a6 01       	ldaa	0x1,x
    a5a8:	d6 d6       	ldab	*0xd6
    a5aa:	3d          	mul
    a5ab:	05          	asld
    a5ac:	b7 10 2a    	staa	0x102a
    a5af:	7a 01 6b    	dec	0x16b
    a5b2:	2b 0a       	bmi	0x0xa5be
    a5b4:	f6 01 6b    	ldab	0x16b
    a5b7:	ce 50 50    	ldx	#0x5050
    a5ba:	3a          	abx
    a5bb:	7e a5 42    	jmp	0xa542
    a5be:	7f 01 6b    	clr	0x16b
    a5c1:	39          	rts
    a5c2:	ce 50 50    	ldx	#0x5050
    a5c5:	f6 01 6b    	ldab	0x16b
    a5c8:	3a          	abx
    a5c9:	e6 00       	ldab	0x0,x
    a5cb:	7d 00 d0    	tst	0xd0
    a5ce:	27 05       	beq	0x0xa5d5
    a5d0:	7f 00 d0    	clr	0xd0
    a5d3:	20 08       	bra	0x0xa5dd
    a5d5:	b6 10 29    	ldaa	0x1029
    a5d8:	2a fb       	bpl	0x0xa5d5
    a5da:	b6 10 2a    	ldaa	0x102a
    a5dd:	01          	nop
    a5de:	01          	nop
    a5df:	01          	nop
    a5e0:	01          	nop
    a5e1:	53          	comb
    a5e2:	f7 10 42    	stab	0x1042
    a5e5:	01          	nop
    a5e6:	01          	nop
    a5e7:	01          	nop
    a5e8:	01          	nop
    a5e9:	01          	nop
    a5ea:	01          	nop
    a5eb:	ce 00 20    	ldx	#0x20
    a5ee:	86 82       	ldaa	#0x82
    a5f0:	b7 10 2a    	staa	0x102a
    a5f3:	c6 88       	ldab	#0x88
    a5f5:	7d 10 29    	tst	0x1029
    a5f8:	2a fb       	bpl	0x0xa5f5
    a5fa:	b6 10 2a    	ldaa	0x102a
    a5fd:	a6 00       	ldaa	0x0,x
    a5ff:	b7 10 2a    	staa	0x102a
    a602:	08          	inx
    a603:	5a          	decb
    a604:	26 ef       	bne	0x0xa5f5
    a606:	7d 10 29    	tst	0x1029
    a609:	2a fb       	bpl	0x0xa606
    a60b:	b6 10 2a    	ldaa	0x102a
    a60e:	86 89       	ldaa	#0x89
    a610:	b7 10 2a    	staa	0x102a
    a613:	ce 50 03    	ldx	#0x5003
    a616:	f6 01 6b    	ldab	0x16b
    a619:	86 04       	ldaa	#0x4
    a61b:	3d          	mul
    a61c:	3a          	abx
    a61d:	a6 00       	ldaa	0x0,x
    a61f:	84 40       	anda	#0x40
    a621:	26 0d       	bne	0x0xa630
    a623:	7d 10 29    	tst	0x1029
    a626:	2a fb       	bpl	0x0xa623
    a628:	b6 10 2a    	ldaa	0x102a
    a62b:	86 8c       	ldaa	#0x8c
    a62d:	b7 10 2a    	staa	0x102a
    a630:	39          	rts
    a631:	b6 01 6a    	ldaa	0x16a
    a634:	c6 50       	ldab	#0x50
    a636:	3d          	mul
    a637:	c3 20 00    	addd	#0x2000
    a63a:	8f          	xgdx
    a63b:	18 ce 50 00 	ldy	#0x5000
    a63f:	c6 50       	ldab	#0x50
    a641:	20 0f       	bra	0x0xa652
    a643:	96 fa       	ldaa	*0xfa
    a645:	c6 b0       	ldab	#0xb0
    a647:	3d          	mul
    a648:	c3 20 00    	addd	#0x2000
    a64b:	8f          	xgdx
    a64c:	18 ce 00 20 	ldy	#0x20
    a650:	c6 b0       	ldab	#0xb0
    a652:	18 a6 00    	ldaa	0x0,y
    a655:	84 7f       	anda	#0x7f
    a657:	a7 00       	staa	0x0,x
    a659:	08          	inx
    a65a:	18 08       	iny
    a65c:	5a          	decb
    a65d:	26 f3       	bne	0x0xa652
    a65f:	39          	rts
    a660:	ce 00 20    	ldx	#0x20
    a663:	18 ce 58 00 	ldy	#0x5800
    a667:	c6 b0       	ldab	#0xb0
    a669:	a6 00       	ldaa	0x0,x
    a66b:	18 a7 00    	staa	0x0,y
    a66e:	08          	inx
    a66f:	18 08       	iny
    a671:	5a          	decb
    a672:	26 f5       	bne	0x0xa669
    a674:	bd a1 16    	jsr	0xa116
    a677:	7e a0 5c    	jmp	0xa05c
    a67a:	ce 00 20    	ldx	#0x20
    a67d:	18 ce 58 00 	ldy	#0x5800
    a681:	c6 b0       	ldab	#0xb0
    a683:	18 a6 00    	ldaa	0x0,y
    a686:	a7 00       	staa	0x0,x
    a688:	08          	inx
    a689:	18 08       	iny
    a68b:	5a          	decb
    a68c:	26 f5       	bne	0x0xa683
    a68e:	bd a1 57    	jsr	0xa157
    a691:	7e a0 5c    	jmp	0xa05c
    a694:	bd 91 16    	jsr	0x9116
    a697:	86 f0       	ldaa	#0xf0
    a699:	b7 10 2f    	staa	0x102f
    a69c:	86 00       	ldaa	#0x0
    a69e:	bd 91 16    	jsr	0x9116
    a6a1:	b7 10 2f    	staa	0x102f
    a6a4:	bd 91 16    	jsr	0x9116
    a6a7:	b7 10 2f    	staa	0x102f
    a6aa:	86 4d       	ldaa	#0x4d
    a6ac:	bd 91 16    	jsr	0x9116
    a6af:	b7 10 2f    	staa	0x102f
    a6b2:	86 08       	ldaa	#0x8
    a6b4:	bd 91 16    	jsr	0x9116
    a6b7:	b7 10 2f    	staa	0x102f
    a6ba:	bd 91 16    	jsr	0x9116
    a6bd:	86 55       	ldaa	#0x55
    a6bf:	b7 10 2f    	staa	0x102f
    a6c2:	bd 91 16    	jsr	0x9116
    a6c5:	86 2a       	ldaa	#0x2a
    a6c7:	b7 10 2f    	staa	0x102f
    a6ca:	bd 91 16    	jsr	0x9116
    a6cd:	4f          	clra
    a6ce:	b7 10 2f    	staa	0x102f
    a6d1:	bd 91 16    	jsr	0x9116
    a6d4:	b7 10 2f    	staa	0x102f
    a6d7:	ce 80 00    	ldx	#0x8000
    a6da:	18 ce 01 90 	ldy	#0x190
    a6de:	c6 01       	ldab	#0x1
    a6e0:	d7 f8       	stab	*0xf8
    a6e2:	a6 00       	ldaa	0x0,x
    a6e4:	36          	psha
    a6e5:	84 0f       	anda	#0xf
    a6e7:	bd 91 16    	jsr	0x9116
    a6ea:	bd 91 1e    	jsr	0x911e
    a6ed:	b7 10 2f    	staa	0x102f
    a6f0:	32          	pula
    a6f1:	84 f0       	anda	#0xf0
    a6f3:	44          	lsra
    a6f4:	bd 91 16    	jsr	0x9116
    a6f7:	bd 91 1e    	jsr	0x911e
    a6fa:	b7 10 2f    	staa	0x102f
    a6fd:	18 09       	dey
    a6ff:	26 09       	bne	0x0xa70a
    a701:	18 ce 01 90 	ldy	#0x190
    a705:	58          	aslb
    a706:	24 02       	bcc	0x0xa70a
    a708:	c6 01       	ldab	#0x1
    a70a:	08          	inx
    a70b:	26 d3       	bne	0x0xa6e0
    a70d:	86 f7       	ldaa	#0xf7
    a70f:	bd 91 16    	jsr	0x9116
    a712:	bd 91 1e    	jsr	0x911e
    a715:	b7 10 2f    	staa	0x102f
    a718:	7f 00 f8    	clr	0xf8
    a71b:	7f 00 ff    	clr	0xff
    a71e:	7f 01 1b    	clr	0x11b
    a721:	bd a0 ff    	jsr	0xa0ff
    a724:	96 f2       	ldaa	*0xf2
    a726:	84 20       	anda	#0x20
    a728:	97 f2       	staa	*0xf2
    a72a:	86 80       	ldaa	#0x80
    a72c:	b7 01 1c    	staa	0x11c
    a72f:	7e a0 5c    	jmp	0xa05c
    a732:	bd a9 6c    	jsr	0xa96c
    a735:	bd a9 6c    	jsr	0xa96c
    a738:	b6 10 08    	ldaa	0x1008
    a73b:	84 df       	anda	#0xdf
    a73d:	b7 10 08    	staa	0x1008
    a740:	b6 10 00    	ldaa	0x1000
    a743:	8a 10       	oraa	#0x10
    a745:	b7 10 00    	staa	0x1000
    a748:	ce 40 00    	ldx	#0x4000
    a74b:	bd a9 6c    	jsr	0xa96c
    a74e:	16          	tab
    a74f:	bd a9 6c    	jsr	0xa96c
    a752:	48          	asla
    a753:	1b          	aba
    a754:	a7 00       	staa	0x0,x
    a756:	08          	inx
    a757:	8c 80 00    	cpx	#0x8000
    a75a:	25 ef       	bcs	0x0xa74b
    a75c:	b6 10 08    	ldaa	0x1008
    a75f:	85 20       	bita	#0x20
    a761:	26 0a       	bne	0x0xa76d
    a763:	8a 20       	oraa	#0x20
    a765:	b7 10 08    	staa	0x1008
    a768:	ce 40 00    	ldx	#0x4000
    a76b:	20 de       	bra	0x0xa74b
    a76d:	86 0c       	ldaa	#0xc
    a76f:	b7 10 2d    	staa	0x102d
    a772:	4f          	clra
    a773:	97 f2       	staa	*0xf2
    a775:	97 f3       	staa	*0xf3
    a777:	97 f4       	staa	*0xf4
    a779:	97 f5       	staa	*0xf5
    a77b:	97 f6       	staa	*0xf6
    a77d:	97 f7       	staa	*0xf7
    a77f:	97 f8       	staa	*0xf8
    a781:	bd a8 49    	jsr	0xa849
    a784:	86 40       	ldaa	#0x40
    a786:	97 f2       	staa	*0xf2
    a788:	86 20       	ldaa	#0x20
    a78a:	b7 10 44    	staa	0x1044
    a78d:	01          	nop
    a78e:	01          	nop
    a78f:	b6 10 45    	ldaa	0x1045
    a792:	b1 01 0f    	cmpa	0x10f
    a795:	27 f8       	beq	0x0xa78f
    a797:	16          	tab
    a798:	b8 01 0f    	eora	0x10f
    a79b:	f7 01 0f    	stab	0x10f
    a79e:	b4 01 0f    	anda	0x10f
    a7a1:	84 c0       	anda	#0xc0
    a7a3:	27 ea       	beq	0x0xa78f
    a7a5:	2a 03       	bpl	0x0xa7aa
    a7a7:	7e a8 2a    	jmp	0xa82a
    a7aa:	7f 00 f2    	clr	0xf2
    a7ad:	bd a8 c0    	jsr	0xa8c0
    a7b0:	b6 10 08    	ldaa	0x1008
    a7b3:	84 df       	anda	#0xdf
    a7b5:	b7 10 08    	staa	0x1008
    a7b8:	4f          	clra
    a7b9:	b7 10 22    	staa	0x1022
    a7bc:	18 ce 00 00 	ldy	#0x0
    a7c0:	ce a7 d4    	ldx	#0xa7d4
    a7c3:	a6 00       	ldaa	0x0,x
    a7c5:	18 a7 00    	staa	0x0,y
    a7c8:	18 08       	iny
    a7ca:	08          	inx
    a7cb:	8c a8 29    	cpx	#0xa829
    a7ce:	25 f3       	bcs	0x0xa7c3
    a7d0:	0f          	sei
    a7d1:	7e 00 00    	jmp	0x0
    a7d4:	ce 40 00    	ldx	#0x4000
    a7d7:	18 ce 80 00 	ldy	#0x8000
    a7db:	e6 00       	ldab	0x0,x
    a7dd:	86 aa       	ldaa	#0xaa
    a7df:	b7 d5 55    	staa	0xd555
    a7e2:	86 55       	ldaa	#0x55
    a7e4:	b7 aa aa    	staa	0xaaaa
    a7e7:	86 a0       	ldaa	#0xa0
    a7e9:	b7 d5 55    	staa	0xd555
    a7ec:	86 80       	ldaa	#0x80
    a7ee:	18 e7 00    	stab	0x0,y
    a7f1:	08          	inx
    a7f2:	18 08       	iny
    a7f4:	e6 00       	ldab	0x0,x
    a7f6:	4a          	deca
    a7f7:	26 f5       	bne	0x0xa7ee
    a7f9:	86 08       	ldaa	#0x8
    a7fb:	b7 10 23    	staa	0x1023
    a7fe:	fc 10 0e    	ldd	0x100e
    a801:	c3 5d c0    	addd	#0x5dc0
    a804:	fd 10 1e    	std	0x101e
    a807:	01          	nop
    a808:	01          	nop
    a809:	b6 10 23    	ldaa	0x1023
    a80c:	85 08       	bita	#0x8
    a80e:	27 f7       	beq	0x0xa807
    a810:	8c 80 00    	cpx	#0x8000
    a813:	25 c6       	bcs	0x0xa7db
    a815:	b6 10 08    	ldaa	0x1008
    a818:	85 20       	bita	#0x20
    a81a:	27 03       	beq	0x0xa81f
    a81c:	7e 80 00    	jmp	0x8000
    a81f:	8a 20       	oraa	#0x20
    a821:	b7 10 08    	staa	0x1008
    a824:	ce 40 00    	ldx	#0x4000
    a827:	20 b2       	bra	0x0xa7db
    a829:	01          	nop
    a82a:	7f 00 f2    	clr	0xf2
    a82d:	bd a9 16    	jsr	0xa916
    a830:	b6 10 45    	ldaa	0x1045
    a833:	b1 01 0f    	cmpa	0x10f
    a836:	27 f8       	beq	0x0xa830
    a838:	16          	tab
    a839:	b8 01 0f    	eora	0x10f
    a83c:	f7 01 0f    	stab	0x10f
    a83f:	b4 01 0f    	anda	0x10f
    a842:	84 80       	anda	#0x80
    a844:	27 ea       	beq	0x0xa830
    a846:	7e 80 00    	jmp	0x8000
    a849:	86 f7       	ldaa	#0xf7
    a84b:	b4 10 00    	anda	0x1000
    a84e:	b7 10 00    	staa	0x1000
    a851:	86 0c       	ldaa	#0xc
    a853:	b7 10 47    	staa	0x1047
    a856:	86 80       	ldaa	#0x80
    a858:	ba 10 00    	oraa	0x1000
    a85b:	b7 10 00    	staa	0x1000
    a85e:	01          	nop
    a85f:	88 80       	eora	#0x80
    a861:	b7 10 00    	staa	0x1000
    a864:	7f 01 1d    	clr	0x11d
    a867:	bd e9 39    	jsr	0xe939
    a86a:	ce 10 23    	ldx	#0x1023
    a86d:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa86d
    a871:	bd e9 6d    	jsr	0xe96d
    a874:	ce 01 20    	ldx	#0x120
    a877:	18 ce a8 a0 	ldy	#0xa8a0
    a87b:	c6 20       	ldab	#0x20
    a87d:	18 a6 00    	ldaa	0x0,y
    a880:	a7 00       	staa	0x0,x
    a882:	08          	inx
    a883:	18 08       	iny
    a885:	5a          	decb
    a886:	26 f5       	bne	0x0xa87d
    a888:	7f 01 1e    	clr	0x11e
    a88b:	86 20       	ldaa	#0x20
    a88d:	b7 01 1c    	staa	0x11c
    a890:	ce 10 23    	ldx	#0x1023
    a893:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa893
    a897:	bd e8 c1    	jsr	0xe8c1
    a89a:	7d 01 1c    	tst	0x11c
    a89d:	26 f1       	bne	0x0xa890
    a89f:	39          	rts
    a8a0:	2a 53       	bpl	0x0xa8f5
    a8a2:	41          	.byte	0x41
    a8a3:	56          	rorb
    a8a4:	45          	.byte	0x45
    a8a5:	2a 20       	bpl	0x0xa8c7
    a8a7:	54          	lsrb
    a8a8:	4f          	clra
    a8a9:	20 55       	bra	0x0xa900
    a8ab:	50          	negb
    a8ac:	44          	lsra
    a8ad:	41          	.byte	0x41
    a8ae:	54          	lsrb
    a8af:	45          	.byte	0x45
    a8b0:	4f          	clra
    a8b1:	53          	comb
    a8b2:	2c 2a       	bge	0x0xa8de
    a8b4:	45          	.byte	0x45
    a8b5:	58          	aslb
    a8b6:	49          	rola
    a8b7:	54          	lsrb
    a8b8:	2a 20       	bpl	0x0xa8da
    a8ba:	41          	.byte	0x41
    a8bb:	42          	.byte	0x42
    a8bc:	4f          	clra
    a8bd:	52          	.byte	0x52
    a8be:	54          	lsrb
    a8bf:	53          	comb
    a8c0:	ce 10 23    	ldx	#0x1023
    a8c3:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa8c3
    a8c7:	bd e9 6d    	jsr	0xe96d
    a8ca:	ce 01 20    	ldx	#0x120
    a8cd:	18 ce a8 f6 	ldy	#0xa8f6
    a8d1:	c6 20       	ldab	#0x20
    a8d3:	18 a6 00    	ldaa	0x0,y
    a8d6:	a7 00       	staa	0x0,x
    a8d8:	08          	inx
    a8d9:	18 08       	iny
    a8db:	5a          	decb
    a8dc:	26 f5       	bne	0x0xa8d3
    a8de:	7f 01 1e    	clr	0x11e
    a8e1:	86 20       	ldaa	#0x20
    a8e3:	b7 01 1c    	staa	0x11c
    a8e6:	ce 10 23    	ldx	#0x1023
    a8e9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa8e9
    a8ed:	bd e8 c1    	jsr	0xe8c1
    a8f0:	7d 01 1c    	tst	0x11c
    a8f3:	26 f1       	bne	0x0xa8e6
    a8f5:	39          	rts
    a8f6:	20 55       	bra	0x0xa94d
    a8f8:	50          	negb
    a8f9:	44          	lsra
    a8fa:	41          	.byte	0x41
    a8fb:	54          	lsrb
    a8fc:	49          	rola
    a8fd:	4e          	.byte	0x4e
    a8fe:	47          	asra
    a8ff:	20 4f       	bra	0x0xa950
    a901:	53          	comb
    a902:	20 2e       	bra	0x0xa932
    a904:	2e 2e       	bgt	0x0xa934
    a906:	20 54       	bra	0x0xa95c
    a908:	41          	.byte	0x41
    a909:	4b          	.byte	0x4b
    a90a:	45          	.byte	0x45
    a90b:	20 35       	bra	0x0xa942
    a90d:	20 2e       	bra	0x0xa93d
    a90f:	2e 2e       	bgt	0x0xa93f
    a911:	2e 2e       	bgt	0x0xa941
    a913:	2e 2e       	bgt	0x0xa943
    a915:	2e ce       	bgt	0x0xa8e5
    a917:	10          	sba
    a918:	23 1f       	bls	0x0xa939
    a91a:	00          	bgnd
    a91b:	10          	sba
    a91c:	fc bd e9    	ldd	0xbde9
    a91f:	6d ce       	tst	0xce,x
    a921:	01          	nop
    a922:	20 18       	bra	0x0xa93c
    a924:	ce a9 4c    	ldx	#0xa94c
    a927:	c6 20       	ldab	#0x20
    a929:	18 a6 00    	ldaa	0x0,y
    a92c:	a7 00       	staa	0x0,x
    a92e:	08          	inx
    a92f:	18 08       	iny
    a931:	5a          	decb
    a932:	26 f5       	bne	0x0xa929
    a934:	7f 01 1e    	clr	0x11e
    a937:	86 20       	ldaa	#0x20
    a939:	b7 01 1c    	staa	0x11c
    a93c:	ce 10 23    	ldx	#0x1023
    a93f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa93f
    a943:	bd e8 c1    	jsr	0xe8c1
    a946:	7d 01 1c    	tst	0x11c
    a949:	26 f1       	bne	0x0xa93c
    a94b:	39          	rts
    a94c:	4f          	clra
    a94d:	53          	comb
    a94e:	20 55       	bra	0x0xa9a5
    a950:	50          	negb
    a951:	44          	lsra
    a952:	41          	.byte	0x41
    a953:	54          	lsrb
    a954:	45          	.byte	0x45
    a955:	20 41       	bra	0x0xa998
    a957:	42          	.byte	0x42
    a958:	4f          	clra
    a959:	52          	.byte	0x52
    a95a:	54          	lsrb
    a95b:	44          	lsra
    a95c:	52          	.byte	0x52
    a95d:	45          	.byte	0x45
    a95e:	4c          	inca
    a95f:	4f          	clra
    a960:	41          	.byte	0x41
    a961:	44          	lsra
    a962:	20 53       	bra	0x0xa9b7
    a964:	45          	.byte	0x45
    a965:	51          	.byte	0x51
    a966:	26 4d       	bne	0x0xa9b5
    a968:	55          	.byte	0x55
    a969:	4c          	inca
    a96a:	54          	lsrb
    a96b:	49          	rola
    a96c:	3c          	pshx
    a96d:	fe 01 c0    	ldx	0x1c0
    a970:	bc 01 c2    	cpx	0x1c2
    a973:	27 f8       	beq	0x0xa96d
    a975:	a6 00       	ldaa	0x0,x
    a977:	8f          	xgdx
    a978:	5c          	incb
    a979:	c4 bf       	andb	#0xbf
    a97b:	8f          	xgdx
    a97c:	ff 01 c0    	stx	0x1c0
    a97f:	38          	pulx
    a980:	39          	rts
    a981:	0f          	sei
    a982:	4f          	clra
    a983:	b7 10 40    	staa	0x1040
    a986:	b6 01 0c    	ldaa	0x10c
    a989:	49          	rola
    a98a:	2a 02       	bpl	0x0xa98e
    a98c:	86 01       	ldaa	#0x1
    a98e:	b7 10 41    	staa	0x1041
    a991:	b7 01 0c    	staa	0x10c
    a994:	fc 01 0d    	ldd	0x10d
    a997:	5c          	incb
    a998:	c1 f8       	cmpb	#0xf8
    a99a:	23 02       	bls	0x0xa99e
    a99c:	c6 f2       	ldab	#0xf2
    a99e:	fd 01 0d    	std	0x10d
    a9a1:	8f          	xgdx
    a9a2:	a6 00       	ldaa	0x0,x
    a9a4:	8c 00 f2    	cpx	#0xf2
    a9a7:	26 15       	bne	0x0xa9be
    a9a9:	8d 41       	bsr	0x0xa9ec
    a9ab:	7d 00 fb    	tst	0xfb
    a9ae:	27 0e       	beq	0x0xa9be
    a9b0:	7d 00 fe    	tst	0xfe
    a9b3:	26 24       	bne	0x0xa9d9
    a9b5:	7d 01 67    	tst	0x167
    a9b8:	27 1f       	beq	0x0xa9d9
    a9ba:	84 df       	anda	#0xdf
    a9bc:	20 1b       	bra	0x0xa9d9
    a9be:	8c 00 f3    	cpx	#0xf3
    a9c1:	26 16       	bne	0x0xa9d9
    a9c3:	8d 27       	bsr	0x0xa9ec
    a9c5:	7d 00 99    	tst	0x99
    a9c8:	27 0f       	beq	0x0xa9d9
    a9ca:	7d 01 67    	tst	0x167
    a9cd:	27 0a       	beq	0x0xa9d9
    a9cf:	13 f3 40 04 	brclr	*0xf3, #0x40, 0x0xa9d7
    a9d3:	84 bf       	anda	#0xbf
    a9d5:	20 02       	bra	0x0xa9d9
    a9d7:	8a 40       	oraa	#0x40
    a9d9:	b7 10 40    	staa	0x1040
    a9dc:	86 80       	ldaa	#0x80
    a9de:	b7 10 23    	staa	0x1023
    a9e1:	fc 10 0e    	ldd	0x100e
    a9e4:	c3 13 88    	addd	#0x1388
    a9e7:	fd 10 16    	std	0x1016
    a9ea:	0e          	cli
    a9eb:	3b          	rti
    a9ec:	7a 01 66    	dec	0x166
    a9ef:	27 01       	beq	0x0xa9f2
    a9f1:	39          	rts
    a9f2:	7d 01 67    	tst	0x167
    a9f5:	26 09       	bne	0x0xaa00
    a9f7:	7c 01 67    	inc	0x167
    a9fa:	c6 19       	ldab	#0x19
    a9fc:	f7 01 66    	stab	0x166
    a9ff:	39          	rts
    aa00:	7f 01 67    	clr	0x167
    aa03:	39          	rts
    aa04:	ce 10 23    	ldx	#0x1023
    aa07:	1e 00 20 03 	brset	0x0,x, #0x20, 0x0xaa0e
    aa0b:	7e ab 37    	jmp	0xab37
    aa0e:	96 ff       	ldaa	*0xff
    aa10:	81 01       	cmpa	#0x1
    aa12:	26 03       	bne	0x0xaa17
    aa14:	7e ab 37    	jmp	0xab37
    aa17:	7d 00 fe    	tst	0xfe
    aa1a:	27 03       	beq	0x0xaa1f
    aa1c:	7e ab 37    	jmp	0xab37
    aa1f:	7d 01 17    	tst	0x117
    aa22:	27 0d       	beq	0x0xaa31
    aa24:	4f          	clra
    aa25:	b7 10 30    	staa	0x1030
    aa28:	b7 01 17    	staa	0x117
    aa2b:	ce 00 96    	ldx	#0x96
    aa2e:	7e ab 2b    	jmp	0xab2b
    aa31:	ce 00 00    	ldx	#0x0
    aa34:	f6 10 31    	ldab	0x1031
    aa37:	3a          	abx
    aa38:	f6 10 32    	ldab	0x1032
    aa3b:	3a          	abx
    aa3c:	f6 10 33    	ldab	0x1033
    aa3f:	3a          	abx
    aa40:	f6 10 34    	ldab	0x1034
    aa43:	3a          	abx
    aa44:	8f          	xgdx
    aa45:	04          	lsrd
    aa46:	04          	lsrd
    aa47:	54          	lsrb
    aa48:	ce 1f 00    	ldx	#0x1f00
    aa4b:	37          	pshb
    aa4c:	f6 01 16    	ldab	0x116
    aa4f:	3a          	abx
    aa50:	bd ab 4b    	jsr	0xab4b
    aa53:	33          	pulb
    aa54:	a6 00       	ldaa	0x0,x
    aa56:	10          	sba
    aa57:	26 40       	bne	0x0xaa99
    aa59:	bd ab 71    	jsr	0xab71
    aa5c:	86 01       	ldaa	#0x1
    aa5e:	b7 01 17    	staa	0x117
    aa61:	b6 01 16    	ldaa	0x116
    aa64:	4c          	inca
    aa65:	81 20       	cmpa	#0x20
    aa67:	25 01       	bcs	0x0xaa6a
    aa69:	4f          	clra
    aa6a:	b7 01 16    	staa	0x116
    aa6d:	81 07       	cmpa	#0x7
    aa6f:	22 07       	bhi	0x0xaa78
    aa71:	8a 70       	oraa	#0x70
    aa73:	b7 10 43    	staa	0x1043
    aa76:	20 1b       	bra	0x0xaa93
    aa78:	81 0f       	cmpa	#0xf
    aa7a:	22 07       	bhi	0x0xaa83
    aa7c:	8a 68       	oraa	#0x68
    aa7e:	b7 10 43    	staa	0x1043
    aa81:	20 10       	bra	0x0xaa93
    aa83:	81 17       	cmpa	#0x17
    aa85:	22 07       	bhi	0x0xaa8e
    aa87:	8a 58       	oraa	#0x58
    aa89:	b7 10 43    	staa	0x1043
    aa8c:	20 05       	bra	0x0xaa93
    aa8e:	8a 38       	oraa	#0x38
    aa90:	b7 10 43    	staa	0x1043
    aa93:	ce 03 e8    	ldx	#0x3e8
    aa96:	7e ab 2b    	jmp	0xab2b
    aa99:	2a 04       	bpl	0x0xaa9f
    aa9b:	40          	nega
    aa9c:	7e aa cf    	jmp	0xaacf
    aa9f:	81 02       	cmpa	#0x2
    aaa1:	23 10       	bls	0x0xaab3
    aaa3:	e7 00       	stab	0x0,x
    aaa5:	86 80       	ldaa	#0x80
    aaa7:	a7 20       	staa	0x20,x
    aaa9:	bd ab 71    	jsr	0xab71
    aaac:	17          	tba
    aaad:	f6 01 16    	ldab	0x116
    aab0:	7e aa fc    	jmp	0xaafc
    aab3:	6d 20       	tst	0x20,x
    aab5:	2b 03       	bmi	0x0xaaba
    aab7:	7e aa 59    	jmp	0xaa59
    aaba:	1f 20 01 05 	brclr	0x20,x, #0x01, 0x0xaac3
    aabe:	6f 20       	clr	0x20,x
    aac0:	7e aa 59    	jmp	0xaa59
    aac3:	e7 00       	stab	0x0,x
    aac5:	bd ab 71    	jsr	0xab71
    aac8:	17          	tba
    aac9:	f6 01 16    	ldab	0x116
    aacc:	7e aa fc    	jmp	0xaafc
    aacf:	81 02       	cmpa	#0x2
    aad1:	23 10       	bls	0x0xaae3
    aad3:	e7 00       	stab	0x0,x
    aad5:	86 81       	ldaa	#0x81
    aad7:	a7 20       	staa	0x20,x
    aad9:	bd ab 71    	jsr	0xab71
    aadc:	17          	tba
    aadd:	f6 01 16    	ldab	0x116
    aae0:	7e aa fc    	jmp	0xaafc
    aae3:	6d 20       	tst	0x20,x
    aae5:	2b 03       	bmi	0x0xaaea
    aae7:	7e aa 59    	jmp	0xaa59
    aaea:	1e 20 01 05 	brset	0x20,x, #0x01, 0x0xaaf3
    aaee:	6f 20       	clr	0x20,x
    aaf0:	7e aa 59    	jmp	0xaa59
    aaf3:	e7 00       	stab	0x0,x
    aaf5:	bd ab 71    	jsr	0xab71
    aaf8:	17          	tba
    aaf9:	f6 01 16    	ldab	0x116
    aafc:	f7 01 15    	stab	0x115
    aaff:	27 1e       	beq	0x0xab1f
    ab01:	c1 07       	cmpb	#0x7
    ab03:	27 1a       	beq	0x0xab1f
    ab05:	f6 01 70    	ldab	0x170
    ab08:	c1 02       	cmpb	#0x2
    ab0a:	26 03       	bne	0x0xab0f
    ab0c:	7e aa 93    	jmp	0xaa93
    ab0f:	7d 00 fb    	tst	0xfb
    ab12:	26 0b       	bne	0x0xab1f
    ab14:	14 fb 01    	bset	*0xfb, #0x01
    ab17:	14 f2 20    	bset	*0xf2, #0x20
    ab1a:	c6 1f       	ldab	#0x1f
    ab1c:	f7 01 66    	stab	0x166
    ab1f:	f6 01 15    	ldab	0x115
    ab22:	58          	aslb
    ab23:	ce ab ce    	ldx	#0xabce
    ab26:	3a          	abx
    ab27:	ee 00       	ldx	0x0,x
    ab29:	6e 00       	jmp	0x0,x
    ab2b:	86 20       	ldaa	#0x20
    ab2d:	b7 10 23    	staa	0x1023
    ab30:	8f          	xgdx
    ab31:	f3 10 0e    	addd	0x100e
    ab34:	fd 10 1a    	std	0x101a
    ab37:	fe 01 c0    	ldx	0x1c0
    ab3a:	bc 01 c2    	cpx	0x1c2
    ab3d:	26 03       	bne	0x0xab42
    ab3f:	7e b0 f0    	jmp	0xb0f0
    ab42:	18 fe 01 6c 	ldy	0x16c
    ab46:	18 3c       	pshy
    ab48:	7e 91 d5    	jmp	0x91d5
    ab4b:	36          	psha
    ab4c:	b6 10 00    	ldaa	0x1000
    ab4f:	84 ef       	anda	#0xef
    ab51:	b7 10 00    	staa	0x1000
    ab54:	b6 10 08    	ldaa	0x1008
    ab57:	84 1f       	anda	#0x1f
    ab59:	b7 10 08    	staa	0x1008
    ab5c:	32          	pula
    ab5d:	39          	rts
    ab5e:	36          	psha
    ab5f:	86 10       	ldaa	#0x10
    ab61:	ba 10 00    	oraa	0x1000
    ab64:	b7 10 00    	staa	0x1000
    ab67:	86 1f       	ldaa	#0x1f
    ab69:	b4 10 08    	anda	0x1008
    ab6c:	b7 10 08    	staa	0x1008
    ab6f:	32          	pula
    ab70:	39          	rts
    ab71:	36          	psha
    ab72:	96 f9       	ldaa	*0xf9
    ab74:	81 01       	cmpa	#0x1
    ab76:	22 02       	bhi	0x0xab7a
    ab78:	32          	pula
    ab79:	39          	rts
    ab7a:	80 02       	suba	#0x2
    ab7c:	26 12       	bne	0x0xab90
    ab7e:	b6 10 00    	ldaa	0x1000
    ab81:	84 ef       	anda	#0xef
    ab83:	b7 10 00    	staa	0x1000
    ab86:	b6 10 08    	ldaa	0x1008
    ab89:	84 1f       	anda	#0x1f
    ab8b:	b7 10 08    	staa	0x1008
    ab8e:	32          	pula
    ab8f:	39          	rts
    ab90:	81 01       	cmpa	#0x1
    ab92:	22 12       	bhi	0x0xaba6
    ab94:	b6 10 00    	ldaa	0x1000
    ab97:	84 ef       	anda	#0xef
    ab99:	b7 10 00    	staa	0x1000
    ab9c:	b6 10 08    	ldaa	0x1008
    ab9f:	8a 20       	oraa	#0x20
    aba1:	b7 10 08    	staa	0x1008
    aba4:	32          	pula
    aba5:	39          	rts
    aba6:	81 02       	cmpa	#0x2
    aba8:	22 12       	bhi	0x0xabbc
    abaa:	b6 10 00    	ldaa	0x1000
    abad:	8a 10       	oraa	#0x10
    abaf:	b7 10 00    	staa	0x1000
    abb2:	b6 10 08    	ldaa	0x1008
    abb5:	84 1f       	anda	#0x1f
    abb7:	b7 10 08    	staa	0x1008
    abba:	32          	pula
    abbb:	39          	rts
    abbc:	b6 10 00    	ldaa	0x1000
    abbf:	8a 10       	oraa	#0x10
    abc1:	b7 10 00    	staa	0x1000
    abc4:	b6 10 08    	ldaa	0x1008
    abc7:	8a 20       	oraa	#0x20
    abc9:	b7 10 08    	staa	0x1008
    abcc:	32          	pula
    abcd:	39          	rts
    abce:	ac 0e       	cpx	0xe,x
    abd0:	ac 82       	cpx	0x82,x
    abd2:	ac ba       	cpx	0xba,x
    abd4:	ac f2       	cpx	0xf2,x
    abd6:	ad 08       	jsr	0x8,x
    abd8:	ad 17       	jsr	0x17,x
    abda:	ad 26       	jsr	0x26,x
    abdc:	ad 35       	jsr	0x35,x
    abde:	ad 46       	jsr	0x46,x
    abe0:	ad 55       	jsr	0x55,x
    abe2:	ad 83       	jsr	0x83,x
    abe4:	ad b1       	jsr	0xb1,x
    abe6:	ad c0       	jsr	0xc0,x
    abe8:	ad cf       	jsr	0xcf,x
    abea:	ad de       	jsr	0xde,x
    abec:	ad ed       	jsr	0xed,x
    abee:	ad fc       	jsr	0xfc,x
    abf0:	ae 0b       	lds	0xb,x
    abf2:	ae 4c       	lds	0x4c,x
    abf4:	ae 79       	lds	0x79,x
    abf6:	ae ba       	lds	0xba,x
    abf8:	ae fb       	lds	0xfb,x
    abfa:	af 0a       	sts	0xa,x
    abfc:	af 4b       	sts	0x4b,x
    abfe:	af 5a       	sts	0x5a,x
    ac00:	af 69       	sts	0x69,x
    ac02:	af 78       	sts	0x78,x
    ac04:	af 87       	sts	0x87,x
    ac06:	af 96       	sts	0x96,x
    ac08:	af a5       	sts	0xa5,x
    ac0a:	af b4       	sts	0xb4,x
    ac0c:	af c3       	sts	0xc3,x
    ac0e:	36          	psha
    ac0f:	c6 74       	ldab	#0x74
    ac11:	96 73       	ldaa	*0x73
    ac13:	81 02       	cmpa	#0x2
    ac15:	25 02       	bcs	0x0xac19
    ac17:	c6 51       	ldab	#0x51
    ac19:	32          	pula
    ac1a:	36          	psha
    ac1b:	bd b0 5c    	jsr	0xb05c
    ac1e:	86 02       	ldaa	#0x2
    ac20:	91 73       	cmpa	*0x73
    ac22:	26 0b       	bne	0x0xac2f
    ac24:	32          	pula
    ac25:	97 50       	staa	*0x50
    ac27:	c6 50       	ldab	#0x50
    ac29:	bd af d2    	jsr	0xafd2
    ac2c:	7e aa 59    	jmp	0xaa59
    ac2f:	32          	pula
    ac30:	44          	lsra
    ac31:	44          	lsra
    ac32:	44          	lsra
    ac33:	91 71       	cmpa	*0x71
    ac35:	26 03       	bne	0x0xac3a
    ac37:	7e aa 59    	jmp	0xaa59
    ac3a:	97 71       	staa	*0x71
    ac3c:	ce ac 52    	ldx	#0xac52
    ac3f:	d6 de       	ldab	*0xde
    ac41:	86 10       	ldaa	#0x10
    ac43:	3d          	mul
    ac44:	3a          	abx
    ac45:	d6 71       	ldab	*0x71
    ac47:	3a          	abx
    ac48:	a6 00       	ldaa	0x0,x
    ac4a:	c6 4b       	ldab	#0x4b
    ac4c:	bd af d2    	jsr	0xafd2
    ac4f:	7e aa 59    	jmp	0xaa59
    ac52:	00          	bgnd
    ac53:	01          	nop
    ac54:	02          	idiv
    ac55:	03          	fdiv
    ac56:	04          	lsrd
    ac57:	00          	bgnd
    ac58:	02          	idiv
    ac59:	04          	lsrd
    ac5a:	01          	nop
    ac5b:	03          	fdiv
    ac5c:	00          	bgnd
    ac5d:	04          	lsrd
    ac5e:	02          	idiv
    ac5f:	00          	bgnd
    ac60:	01          	nop
    ac61:	04          	lsrd
    ac62:	00          	bgnd
    ac63:	01          	nop
    ac64:	02          	idiv
    ac65:	03          	fdiv
    ac66:	04          	lsrd
    ac67:	06          	tap
    ac68:	03          	fdiv
    ac69:	01          	nop
    ac6a:	04          	lsrd
    ac6b:	00          	bgnd
    ac6c:	02          	idiv
    ac6d:	06          	tap
    ac6e:	00          	bgnd
    ac6f:	03          	fdiv
    ac70:	01          	nop
    ac71:	06          	tap
    ac72:	00          	bgnd
    ac73:	02          	idiv
    ac74:	04          	lsrd
    ac75:	06          	tap
    ac76:	03          	fdiv
    ac77:	05          	asld
    ac78:	01          	nop
    ac79:	04          	lsrd
    ac7a:	00          	bgnd
    ac7b:	03          	fdiv
    ac7c:	01          	nop
    ac7d:	06          	tap
    ac7e:	02          	idiv
    ac7f:	05          	asld
    ac80:	00          	bgnd
    ac81:	06          	tap
    ac82:	16          	tab
    ac83:	ce ed f2    	ldx	#0xedf2
    ac86:	3a          	abx
    ac87:	a6 00       	ldaa	0x0,x
    ac89:	97 2a       	staa	*0x2a
    ac8b:	c6 2a       	ldab	#0x2a
    ac8d:	bd af d2    	jsr	0xafd2
    ac90:	b6 01 1b    	ldaa	0x11b
    ac93:	81 19       	cmpa	#0x19
    ac95:	26 19       	bne	0x0xacb0
    ac97:	13 f4 01 15 	brclr	*0xf4, #0x01, 0x0xacb0
    ac9b:	96 2a       	ldaa	*0x2a
    ac9d:	ce 01 31    	ldx	#0x131
    aca0:	bd ea 20    	jsr	0xea20
    aca3:	86 03       	ldaa	#0x3
    aca5:	b7 01 1c    	staa	0x11c
    aca8:	86 13       	ldaa	#0x13
    acaa:	b7 01 1e    	staa	0x11e
    acad:	bd e9 48    	jsr	0xe948
    acb0:	96 2a       	ldaa	*0x2a
    acb2:	c6 48       	ldab	#0x48
    acb4:	bd b0 5c    	jsr	0xb05c
    acb7:	7e aa 59    	jmp	0xaa59
    acba:	16          	tab
    acbb:	ce ed f2    	ldx	#0xedf2
    acbe:	3a          	abx
    acbf:	a6 00       	ldaa	0x0,x
    acc1:	97 37       	staa	*0x37
    acc3:	c6 37       	ldab	#0x37
    acc5:	bd af d2    	jsr	0xafd2
    acc8:	b6 01 1b    	ldaa	0x11b
    accb:	81 19       	cmpa	#0x19
    accd:	26 19       	bne	0x0xace8
    accf:	13 f4 02 15 	brclr	*0xf4, #0x02, 0x0xace8
    acd3:	96 37       	ldaa	*0x37
    acd5:	ce 01 31    	ldx	#0x131
    acd8:	bd ea 20    	jsr	0xea20
    acdb:	86 03       	ldaa	#0x3
    acdd:	b7 01 1c    	staa	0x11c
    ace0:	86 13       	ldaa	#0x13
    ace2:	b7 01 1e    	staa	0x11e
    ace5:	bd e9 48    	jsr	0xe948
    ace8:	96 37       	ldaa	*0x37
    acea:	c6 4a       	ldab	#0x4a
    acec:	bd b0 5c    	jsr	0xb05c
    acef:	7e aa 59    	jmp	0xaa59
    acf2:	16          	tab
    acf3:	ce ed f2    	ldx	#0xedf2
    acf6:	3a          	abx
    acf7:	a6 00       	ldaa	0x0,x
    acf9:	97 49       	staa	*0x49
    acfb:	c6 49       	ldab	#0x49
    acfd:	bd af d2    	jsr	0xafd2
    ad00:	c6 57       	ldab	#0x57
    ad02:	bd b0 5c    	jsr	0xb05c
    ad05:	7e aa 59    	jmp	0xaa59
    ad08:	97 35       	staa	*0x35
    ad0a:	c6 35       	ldab	#0x35
    ad0c:	bd af d2    	jsr	0xafd2
    ad0f:	c6 49       	ldab	#0x49
    ad11:	bd b0 5c    	jsr	0xb05c
    ad14:	7e aa 59    	jmp	0xaa59
    ad17:	97 20       	staa	*0x20
    ad19:	c6 20       	ldab	#0x20
    ad1b:	bd af d2    	jsr	0xafd2
    ad1e:	c6 05       	ldab	#0x5
    ad20:	bd b0 5c    	jsr	0xb05c
    ad23:	7e aa 59    	jmp	0xaa59
    ad26:	97 28       	staa	*0x28
    ad28:	c6 28       	ldab	#0x28
    ad2a:	bd af d2    	jsr	0xafd2
    ad2d:	c6 47       	ldab	#0x47
    ad2f:	bd b0 5c    	jsr	0xb05c
    ad32:	7e aa 59    	jmp	0xaa59
    ad35:	97 d5       	staa	*0xd5
    ad37:	97 d6       	staa	*0xd6
    ad39:	c6 fc       	ldab	#0xfc
    ad3b:	bd af d2    	jsr	0xafd2
    ad3e:	c6 07       	ldab	#0x7
    ad40:	bd b0 5c    	jsr	0xb05c
    ad43:	7e aa 59    	jmp	0xaa59
    ad46:	97 42       	staa	*0x42
    ad48:	c6 42       	ldab	#0x42
    ad4a:	bd af d2    	jsr	0xafd2
    ad4d:	c6 3b       	ldab	#0x3b
    ad4f:	bd b0 5c    	jsr	0xb05c
    ad52:	7e aa 59    	jmp	0xaa59
    ad55:	44          	lsra
    ad56:	97 3f       	staa	*0x3f
    ad58:	c6 3f       	ldab	#0x3f
    ad5a:	bd af d2    	jsr	0xafd2
    ad5d:	36          	psha
    ad5e:	b6 01 1b    	ldaa	0x11b
    ad61:	81 06       	cmpa	#0x6
    ad63:	26 15       	bne	0x0xad7a
    ad65:	96 3f       	ldaa	*0x3f
    ad67:	ce 01 3c    	ldx	#0x13c
    ad6a:	bd ea 20    	jsr	0xea20
    ad6d:	86 03       	ldaa	#0x3
    ad6f:	b7 01 1c    	staa	0x11c
    ad72:	86 1e       	ldaa	#0x1e
    ad74:	b7 01 1e    	staa	0x11e
    ad77:	bd e9 48    	jsr	0xe948
    ad7a:	32          	pula
    ad7b:	c6 3c       	ldab	#0x3c
    ad7d:	bd b0 5c    	jsr	0xb05c
    ad80:	7e aa 59    	jmp	0xaa59
    ad83:	44          	lsra
    ad84:	97 3e       	staa	*0x3e
    ad86:	c6 3e       	ldab	#0x3e
    ad88:	bd af d2    	jsr	0xafd2
    ad8b:	36          	psha
    ad8c:	b6 01 1b    	ldaa	0x11b
    ad8f:	81 06       	cmpa	#0x6
    ad91:	26 15       	bne	0x0xada8
    ad93:	96 3e       	ldaa	*0x3e
    ad95:	ce 01 37    	ldx	#0x137
    ad98:	bd ea 20    	jsr	0xea20
    ad9b:	86 03       	ldaa	#0x3
    ad9d:	b7 01 1c    	staa	0x11c
    ada0:	86 19       	ldaa	#0x19
    ada2:	b7 01 1e    	staa	0x11e
    ada5:	bd e9 48    	jsr	0xe948
    ada8:	32          	pula
    ada9:	c6 36       	ldab	#0x36
    adab:	bd b0 5c    	jsr	0xb05c
    adae:	7e aa 59    	jmp	0xaa59
    adb1:	97 43       	staa	*0x43
    adb3:	c6 43       	ldab	#0x43
    adb5:	bd af d2    	jsr	0xafd2
    adb8:	c6 46       	ldab	#0x46
    adba:	bd b0 5c    	jsr	0xb05c
    adbd:	7e aa 59    	jmp	0xaa59
    adc0:	97 45       	staa	*0x45
    adc2:	c6 45       	ldab	#0x45
    adc4:	bd af d2    	jsr	0xafd2
    adc7:	c6 54       	ldab	#0x54
    adc9:	bd b0 5c    	jsr	0xb05c
    adcc:	7e aa 59    	jmp	0xaa59
    adcf:	97 4c       	staa	*0x4c
    add1:	c6 4c       	ldab	#0x4c
    add3:	bd af d2    	jsr	0xafd2
    add6:	c6 58       	ldab	#0x58
    add8:	bd b0 5c    	jsr	0xb05c
    addb:	7e aa 59    	jmp	0xaa59
    adde:	97 46       	staa	*0x46
    ade0:	c6 46       	ldab	#0x46
    ade2:	bd af d2    	jsr	0xafd2
    ade5:	c6 55       	ldab	#0x55
    ade7:	bd b0 5c    	jsr	0xb05c
    adea:	7e aa 59    	jmp	0xaa59
    aded:	97 47       	staa	*0x47
    adef:	c6 47       	ldab	#0x47
    adf1:	bd af d2    	jsr	0xafd2
    adf4:	c6 56       	ldab	#0x56
    adf6:	bd b0 5c    	jsr	0xb05c
    adf9:	7e aa 59    	jmp	0xaa59
    adfc:	97 4f       	staa	*0x4f
    adfe:	c6 4f       	ldab	#0x4f
    ae00:	bd af d2    	jsr	0xafd2
    ae03:	c6 66       	ldab	#0x66
    ae05:	bd b0 5c    	jsr	0xb05c
    ae08:	7e aa 59    	jmp	0xaa59
    ae0b:	7d 00 99    	tst	0x99
    ae0e:	26 0f       	bne	0x0xae1f
    ae10:	97 5c       	staa	*0x5c
    ae12:	c6 5c       	ldab	#0x5c
    ae14:	bd af d2    	jsr	0xafd2
    ae17:	c6 71       	ldab	#0x71
    ae19:	bd b0 5c    	jsr	0xb05c
    ae1c:	7e aa 59    	jmp	0xaa59
    ae1f:	97 48       	staa	*0x48
    ae21:	c6 48       	ldab	#0x48
    ae23:	bd af d2    	jsr	0xafd2
    ae26:	36          	psha
    ae27:	b6 01 1b    	ldaa	0x11b
    ae2a:	81 17       	cmpa	#0x17
    ae2c:	26 15       	bne	0x0xae43
    ae2e:	96 48       	ldaa	*0x48
    ae30:	ce 01 34    	ldx	#0x134
    ae33:	bd ea 20    	jsr	0xea20
    ae36:	86 03       	ldaa	#0x3
    ae38:	b7 01 1c    	staa	0x11c
    ae3b:	86 16       	ldaa	#0x16
    ae3d:	b7 01 1e    	staa	0x11e
    ae40:	bd e9 48    	jsr	0xe948
    ae43:	32          	pula
    ae44:	c6 5c       	ldab	#0x5c
    ae46:	bd b0 5c    	jsr	0xb05c
    ae49:	7e aa 59    	jmp	0xaa59
    ae4c:	97 63       	staa	*0x63
    ae4e:	c6 63       	ldab	#0x63
    ae50:	bd af d2    	jsr	0xafd2
    ae53:	36          	psha
    ae54:	b6 01 1b    	ldaa	0x11b
    ae57:	81 13       	cmpa	#0x13
    ae59:	26 15       	bne	0x0xae70
    ae5b:	96 63       	ldaa	*0x63
    ae5d:	ce 01 31    	ldx	#0x131
    ae60:	bd ea 20    	jsr	0xea20
    ae63:	86 03       	ldaa	#0x3
    ae65:	b7 01 1c    	staa	0x11c
    ae68:	86 13       	ldaa	#0x13
    ae6a:	b7 01 1e    	staa	0x11e
    ae6d:	bd e9 48    	jsr	0xe948
    ae70:	32          	pula
    ae71:	c6 6f       	ldab	#0x6f
    ae73:	bd b0 5c    	jsr	0xb05c
    ae76:	7e aa 59    	jmp	0xaa59
    ae79:	7d 00 99    	tst	0x99
    ae7c:	26 0f       	bne	0x0xae8d
    ae7e:	97 5b       	staa	*0x5b
    ae80:	c6 5b       	ldab	#0x5b
    ae82:	bd af d2    	jsr	0xafd2
    ae85:	c6 70       	ldab	#0x70
    ae87:	bd b0 5c    	jsr	0xb05c
    ae8a:	7e aa 59    	jmp	0xaa59
    ae8d:	97 6e       	staa	*0x6e
    ae8f:	c6 6e       	ldab	#0x6e
    ae91:	bd af d2    	jsr	0xafd2
    ae94:	36          	psha
    ae95:	b6 01 1b    	ldaa	0x11b
    ae98:	81 17       	cmpa	#0x17
    ae9a:	26 15       	bne	0x0xaeb1
    ae9c:	96 6e       	ldaa	*0x6e
    ae9e:	ce 01 30    	ldx	#0x130
    aea1:	bd ea 20    	jsr	0xea20
    aea4:	86 03       	ldaa	#0x3
    aea6:	b7 01 1c    	staa	0x11c
    aea9:	86 12       	ldaa	#0x12
    aeab:	b7 01 1e    	staa	0x11e
    aeae:	bd e9 48    	jsr	0xe948
    aeb1:	32          	pula
    aeb2:	c6 5b       	ldab	#0x5b
    aeb4:	bd b0 5c    	jsr	0xb05c
    aeb7:	7e aa 59    	jmp	0xaa59
    aeba:	7d 00 99    	tst	0x99
    aebd:	26 0f       	bne	0x0xaece
    aebf:	97 5f       	staa	*0x5f
    aec1:	c6 5f       	ldab	#0x5f
    aec3:	bd af d2    	jsr	0xafd2
    aec6:	c6 73       	ldab	#0x73
    aec8:	bd b0 5c    	jsr	0xb05c
    aecb:	7e aa 59    	jmp	0xaa59
    aece:	97 70       	staa	*0x70
    aed0:	86 70       	ldaa	#0x70
    aed2:	bd af d2    	jsr	0xafd2
    aed5:	36          	psha
    aed6:	b6 01 1b    	ldaa	0x11b
    aed9:	81 17       	cmpa	#0x17
    aedb:	26 15       	bne	0x0xaef2
    aedd:	96 70       	ldaa	*0x70
    aedf:	ce 01 3c    	ldx	#0x13c
    aee2:	bd ea e7    	jsr	0xeae7
    aee5:	86 03       	ldaa	#0x3
    aee7:	b7 01 1c    	staa	0x11c
    aeea:	86 1f       	ldaa	#0x1f
    aeec:	b7 01 1e    	staa	0x11e
    aeef:	bd e9 48    	jsr	0xe948
    aef2:	32          	pula
    aef3:	c6 5e       	ldab	#0x5e
    aef5:	bd b0 5c    	jsr	0xb05c
    aef8:	7e aa 59    	jmp	0xaa59
    aefb:	97 4e       	staa	*0x4e
    aefd:	c6 4e       	ldab	#0x4e
    aeff:	bd af d2    	jsr	0xafd2
    af02:	c6 5a       	ldab	#0x5a
    af04:	bd b0 5c    	jsr	0xb05c
    af07:	7e aa 59    	jmp	0xaa59
    af0a:	7d 00 99    	tst	0x99
    af0d:	26 0f       	bne	0x0xaf1e
    af0f:	97 5d       	staa	*0x5d
    af11:	c6 5d       	ldab	#0x5d
    af13:	bd af d2    	jsr	0xafd2
    af16:	c6 72       	ldab	#0x72
    af18:	bd b0 5c    	jsr	0xb05c
    af1b:	7e aa 59    	jmp	0xaa59
    af1e:	97 6f       	staa	*0x6f
    af20:	c6 6f       	ldab	#0x6f
    af22:	bd af d2    	jsr	0xafd2
    af25:	36          	psha
    af26:	b6 01 1b    	ldaa	0x11b
    af29:	81 17       	cmpa	#0x17
    af2b:	26 15       	bne	0x0xaf42
    af2d:	96 6f       	ldaa	*0x6f
    af2f:	ce 01 38    	ldx	#0x138
    af32:	bd ea 20    	jsr	0xea20
    af35:	86 03       	ldaa	#0x3
    af37:	b7 01 1c    	staa	0x11c
    af3a:	86 1a       	ldaa	#0x1a
    af3c:	b7 01 1e    	staa	0x11e
    af3f:	bd e9 48    	jsr	0xe948
    af42:	32          	pula
    af43:	c6 5d       	ldab	#0x5d
    af45:	bd b0 5c    	jsr	0xb05c
    af48:	7e aa 59    	jmp	0xaa59
    af4b:	97 4d       	staa	*0x4d
    af4d:	c6 4d       	ldab	#0x4d
    af4f:	bd af d2    	jsr	0xafd2
    af52:	c6 59       	ldab	#0x59
    af54:	bd b0 5c    	jsr	0xb05c
    af57:	7e aa 59    	jmp	0xaa59
    af5a:	97 57       	staa	*0x57
    af5c:	c6 57       	ldab	#0x57
    af5e:	bd af d2    	jsr	0xafd2
    af61:	c6 6c       	ldab	#0x6c
    af63:	bd b0 5c    	jsr	0xb05c
    af66:	7e aa 59    	jmp	0xaa59
    af69:	97 58       	staa	*0x58
    af6b:	c6 58       	ldab	#0x58
    af6d:	bd af d2    	jsr	0xafd2
    af70:	c6 6d       	ldab	#0x6d
    af72:	bd b0 5c    	jsr	0xb05c
    af75:	7e aa 59    	jmp	0xaa59
    af78:	97 5a       	staa	*0x5a
    af7a:	c6 5a       	ldab	#0x5a
    af7c:	bd af d2    	jsr	0xafd2
    af7f:	c6 6e       	ldab	#0x6e
    af81:	bd b0 5c    	jsr	0xb05c
    af84:	7e aa 59    	jmp	0xaa59
    af87:	97 56       	staa	*0x56
    af89:	c6 56       	ldab	#0x56
    af8b:	bd af d2    	jsr	0xafd2
    af8e:	c6 6b       	ldab	#0x6b
    af90:	bd b0 5c    	jsr	0xb05c
    af93:	7e aa 59    	jmp	0xaa59
    af96:	97 55       	staa	*0x55
    af98:	c6 55       	ldab	#0x55
    af9a:	bd af d2    	jsr	0xafd2
    af9d:	c6 6a       	ldab	#0x6a
    af9f:	bd b0 5c    	jsr	0xb05c
    afa2:	7e aa 59    	jmp	0xaa59
    afa5:	97 51       	staa	*0x51
    afa7:	c6 51       	ldab	#0x51
    afa9:	bd af d2    	jsr	0xafd2
    afac:	c6 67       	ldab	#0x67
    afae:	bd b0 5c    	jsr	0xb05c
    afb1:	7e aa 59    	jmp	0xaa59
    afb4:	97 53       	staa	*0x53
    afb6:	c6 53       	ldab	#0x53
    afb8:	bd af d2    	jsr	0xafd2
    afbb:	c6 69       	ldab	#0x69
    afbd:	bd b0 5c    	jsr	0xb05c
    afc0:	7e aa 59    	jmp	0xaa59
    afc3:	97 52       	staa	*0x52
    afc5:	c6 52       	ldab	#0x52
    afc7:	bd af d2    	jsr	0xafd2
    afca:	c6 68       	ldab	#0x68
    afcc:	bd b0 5c    	jsr	0xb05c
    afcf:	7e aa 59    	jmp	0xaa59
    afd2:	37          	pshb
    afd3:	7d 00 d0    	tst	0xd0
    afd6:	27 05       	beq	0x0xafdd
    afd8:	7f 00 d0    	clr	0xd0
    afdb:	20 08       	bra	0x0xafe5
    afdd:	7d 10 29    	tst	0x1029
    afe0:	2a fb       	bpl	0x0xafdd
    afe2:	f6 10 2a    	ldab	0x102a
    afe5:	12 d1 80 18 	brset	*0xd1, #0x80, 0x0xb001
    afe9:	33          	pulb
    afea:	37          	pshb
    afeb:	c1 fc       	cmpb	#0xfc
    afed:	26 06       	bne	0x0xaff5
    afef:	33          	pulb
    aff0:	c6 ac       	ldab	#0xac
    aff2:	37          	pshb
    aff3:	20 0c       	bra	0x0xb001
    aff5:	f6 01 6b    	ldab	0x16b
    aff8:	ce 50 50    	ldx	#0x5050
    affb:	3a          	abx
    affc:	e6 00       	ldab	0x0,x
    affe:	53          	comb
    afff:	20 01       	bra	0x0xb002
    b001:	5f          	clrb
    b002:	01          	nop
    b003:	01          	nop
    b004:	01          	nop
    b005:	01          	nop
    b006:	f7 10 42    	stab	0x1042
    b009:	01          	nop
    b00a:	01          	nop
    b00b:	01          	nop
    b00c:	01          	nop
    b00d:	01          	nop
    b00e:	01          	nop
    b00f:	01          	nop
    b010:	c6 83       	ldab	#0x83
    b012:	f7 10 2a    	stab	0x102a
    b015:	7d 10 29    	tst	0x1029
    b018:	2a fb       	bpl	0x0xb015
    b01a:	f6 10 2a    	ldab	0x102a
    b01d:	33          	pulb
    b01e:	c1 fc       	cmpb	#0xfc
    b020:	26 02       	bne	0x0xb024
    b022:	c6 ac       	ldab	#0xac
    b024:	37          	pshb
    b025:	c0 20       	subb	#0x20
    b027:	f7 10 2a    	stab	0x102a
    b02a:	7d 10 29    	tst	0x1029
    b02d:	2a fb       	bpl	0x0xb02a
    b02f:	f6 10 2a    	ldab	0x102a
    b032:	b7 10 2a    	staa	0x102a
    b035:	33          	pulb
    b036:	12 d1 80 21 	brset	*0xd1, #0x80, 0x0xb05b
    b03a:	c1 44       	cmpb	#0x44
    b03c:	26 1d       	bne	0x0xb05b
    b03e:	ce 50 03    	ldx	#0x5003
    b041:	f6 01 6b    	ldab	0x16b
    b044:	86 04       	ldaa	#0x4
    b046:	3d          	mul
    b047:	3a          	abx
    b048:	a6 00       	ldaa	0x0,x
    b04a:	84 40       	anda	#0x40
    b04c:	26 0d       	bne	0x0xb05b
    b04e:	7d 10 29    	tst	0x1029
    b051:	2a fb       	bpl	0x0xb04e
    b053:	b6 10 2a    	ldaa	0x102a
    b056:	86 8c       	ldaa	#0x8c
    b058:	b7 10 2a    	staa	0x102a
    b05b:	39          	rts
    b05c:	0f          	sei
    b05d:	37          	pshb
    b05e:	f6 01 6f    	ldab	0x16f
    b061:	ca b0       	orab	#0xb0
    b063:	fe 01 62    	ldx	0x162
    b066:	e7 00       	stab	0x0,x
    b068:	8f          	xgdx
    b069:	5c          	incb
    b06a:	c1 60       	cmpb	#0x60
    b06c:	25 02       	bcs	0x0xb070
    b06e:	c6 40       	ldab	#0x40
    b070:	8f          	xgdx
    b071:	33          	pulb
    b072:	e7 00       	stab	0x0,x
    b074:	8f          	xgdx
    b075:	5c          	incb
    b076:	c1 60       	cmpb	#0x60
    b078:	25 02       	bcs	0x0xb07c
    b07a:	c6 40       	ldab	#0x40
    b07c:	8f          	xgdx
    b07d:	a7 00       	staa	0x0,x
    b07f:	8f          	xgdx
    b080:	5c          	incb
    b081:	c1 60       	cmpb	#0x60
    b083:	25 02       	bcs	0x0xb087
    b085:	c6 40       	ldab	#0x40
    b087:	8f          	xgdx
    b088:	ff 01 62    	stx	0x162
    b08b:	0e          	cli
    b08c:	7d 00 d7    	tst	0xd7
    b08f:	27 06       	beq	0x0xb097
    b091:	7f 00 d7    	clr	0xd7
    b094:	bd fe c9    	jsr	0xfec9
    b097:	39          	rts
    b098:	b0 fc b1    	suba	0xfcb1
    b09b:	3f          	swi
    b09c:	b1 67 b2    	cmpa	0x67b2
    b09f:	21 b3       	brn	0x0xb054
    b0a1:	93 b4       	subd	*0xb4
    b0a3:	bf b6 54    	sts	0xb654
    b0a6:	b5 56 b7    	bita	0x56b7
    b0a9:	6b          	.byte	0x6b
    b0aa:	b8 49 b9    	eora	0x49b9
    b0ad:	2b ba       	bmi	0x0xb069
    b0af:	03          	fdiv
    b0b0:	ba 06 bb    	oraa	0x6bb
    b0b3:	b7 bc 81    	staa	0xbc81
    b0b6:	bd 07 bd    	jsr	0x7bd
    b0b9:	70 bd 70    	neg	0xbd70
    b0bc:	bd 70 bd    	jsr	0x70bd
    b0bf:	90 bf       	suba	*0xbf
    b0c1:	1a c0       	.byte	0x1a, 0xc0
    b0c3:	01          	nop
    b0c4:	c0 e8       	subb	#0xe8
    b0c6:	c1 d1       	cmpb	#0xd1
    b0c8:	c2 f2       	sbcb	#0xf2
    b0ca:	c2 fb       	sbcb	#0xfb
    b0cc:	c4 d9       	andb	#0xd9
    b0ce:	c6 08       	ldab	#0x8
    b0d0:	c7          	.byte	0xc7
    b0d1:	6f c7       	clr	0xc7,x
    b0d3:	72          	.byte	0x72
    b0d4:	c8 ad       	eorb	#0xad
    b0d6:	c9 74       	adcb	#0x74
    b0d8:	ca 3b       	orab	#0x3b
    b0da:	cb 1c       	addb	#0x1c
    b0dc:	cb 28       	addb	#0x28
    b0de:	cc 1a cc    	ldd	#0x1acc
    b0e1:	a4 cd       	anda	0xcd,x
    b0e3:	32          	pula
    b0e4:	cd e4       	.byte	0xcd, 0xe4
    b0e6:	ce e7 ce    	ldx	#0xe7ce
    b0e9:	ea cf       	orab	0xcf,x
    b0eb:	ba d1 70    	oraa	0xd170
    b0ee:	d2 4c       	sbcb	*0x4c
    b0f0:	f6 01 1b    	ldab	0x11b
    b0f3:	58          	aslb
    b0f4:	ce b0 98    	ldx	#0xb098
    b0f7:	3a          	abx
    b0f8:	ee 00       	ldx	0x0,x
    b0fa:	6e 00       	jmp	0x0,x
    b0fc:	7d 01 1c    	tst	0x11c
    b0ff:	2a 03       	bpl	0x0xb104
    b101:	7e d3 cb    	jmp	0xd3cb
    b104:	7d 01 1c    	tst	0x11c
    b107:	27 03       	beq	0x0xb10c
    b109:	7e b1 32    	jmp	0xb132
    b10c:	7d 01 1e    	tst	0x11e
    b10f:	26 08       	bne	0x0xb119
    b111:	86 0c       	ldaa	#0xc
    b113:	b7 01 1e    	staa	0x11e
    b116:	bd e9 48    	jsr	0xe948
    b119:	7d 01 7e    	tst	0x17e
    b11c:	27 0c       	beq	0x0xb12a
    b11e:	7f 01 7e    	clr	0x17e
    b121:	7f 01 7f    	clr	0x17f
    b124:	7f 01 1a    	clr	0x11a
    b127:	7e d3 cb    	jmp	0xd3cb
    b12a:	7d 01 1c    	tst	0x11c
    b12d:	26 03       	bne	0x0xb132
    b12f:	7e d3 b7    	jmp	0xd3b7
    b132:	b6 10 23    	ldaa	0x1023
    b135:	85 10       	bita	#0x10
    b137:	27 03       	beq	0x0xb13c
    b139:	7e e8 bc    	jmp	0xe8bc
    b13c:	7e d3 b7    	jmp	0xd3b7
    b13f:	7d 01 1c    	tst	0x11c
    b142:	2a 03       	bpl	0x0xb147
    b144:	7e d4 95    	jmp	0xd495
    b147:	7d 01 1c    	tst	0x11c
    b14a:	27 03       	beq	0x0xb14f
    b14c:	7e b1 32    	jmp	0xb132
    b14f:	7d 01 1e    	tst	0x11e
    b152:	26 08       	bne	0x0xb15c
    b154:	86 0c       	ldaa	#0xc
    b156:	b7 01 1e    	staa	0x11e
    b159:	bd e9 48    	jsr	0xe948
    b15c:	7d 01 7e    	tst	0x17e
    b15f:	27 03       	beq	0x0xb164
    b161:	7e d4 95    	jmp	0xd495
    b164:	7e d3 b7    	jmp	0xd3b7
    b167:	7d 01 1c    	tst	0x11c
    b16a:	2a 03       	bpl	0x0xb16f
    b16c:	7e d5 ba    	jmp	0xd5ba
    b16f:	7d 01 1c    	tst	0x11c
    b172:	27 03       	beq	0x0xb177
    b174:	7e b1 32    	jmp	0xb132
    b177:	7d 01 1e    	tst	0x11e
    b17a:	26 08       	bne	0x0xb184
    b17c:	86 10       	ldaa	#0x10
    b17e:	b7 01 1e    	staa	0x11e
    b181:	bd e9 48    	jsr	0xe948
    b184:	7d 01 7f    	tst	0x17f
    b187:	27 29       	beq	0x0xb1b2
    b189:	2b 1d       	bmi	0x0xb1a8
    b18b:	b6 01 1e    	ldaa	0x11e
    b18e:	4c          	inca
    b18f:	81 1f       	cmpa	#0x1f
    b191:	23 02       	bls	0x0xb195
    b193:	20 06       	bra	0x0xb19b
    b195:	b7 01 1e    	staa	0x11e
    b198:	bd e9 48    	jsr	0xe948
    b19b:	4f          	clra
    b19c:	b7 01 7f    	staa	0x17f
    b19f:	b7 01 7e    	staa	0x17e
    b1a2:	b7 01 1a    	staa	0x11a
    b1a5:	7e d3 b7    	jmp	0xd3b7
    b1a8:	b6 01 1e    	ldaa	0x11e
    b1ab:	4a          	deca
    b1ac:	81 0f       	cmpa	#0xf
    b1ae:	22 e5       	bhi	0x0xb195
    b1b0:	20 e9       	bra	0x0xb19b
    b1b2:	7d 01 7e    	tst	0x17e
    b1b5:	27 0f       	beq	0x0xb1c6
    b1b7:	7f 01 7e    	clr	0x17e
    b1ba:	ce 01 20    	ldx	#0x120
    b1bd:	f6 01 1e    	ldab	0x11e
    b1c0:	3a          	abx
    b1c1:	86 20       	ldaa	#0x20
    b1c3:	7e b1 f2    	jmp	0xb1f2
    b1c6:	7d 01 1a    	tst	0x11a
    b1c9:	26 03       	bne	0x0xb1ce
    b1cb:	7e d3 b7    	jmp	0xd3b7
    b1ce:	ce 01 20    	ldx	#0x120
    b1d1:	f6 01 1e    	ldab	0x11e
    b1d4:	3a          	abx
    b1d5:	a6 00       	ldaa	0x0,x
    b1d7:	81 5f       	cmpa	#0x5f
    b1d9:	23 02       	bls	0x0xb1dd
    b1db:	86 20       	ldaa	#0x20
    b1dd:	7d 01 1a    	tst	0x11a
    b1e0:	2b 09       	bmi	0x0xb1eb
    b1e2:	4c          	inca
    b1e3:	81 5f       	cmpa	#0x5f
    b1e5:	23 0b       	bls	0x0xb1f2
    b1e7:	86 20       	ldaa	#0x20
    b1e9:	20 07       	bra	0x0xb1f2
    b1eb:	4a          	deca
    b1ec:	81 20       	cmpa	#0x20
    b1ee:	24 02       	bcc	0x0xb1f2
    b1f0:	86 5f       	ldaa	#0x5f
    b1f2:	7f 01 1a    	clr	0x11a
    b1f5:	a7 00       	staa	0x0,x
    b1f7:	36          	psha
    b1f8:	bd e9 0c    	jsr	0xe90c
    b1fb:	13 d1 80 0e 	brclr	*0xd1, #0x80, 0x0xb20d
    b1ff:	32          	pula
    b200:	8f          	xgdx
    b201:	83 00 70    	subd	#0x70
    b204:	8f          	xgdx
    b205:	a7 00       	staa	0x0,x
    b207:	bd e9 48    	jsr	0xe948
    b20a:	7e d3 b7    	jmp	0xd3b7
    b20d:	32          	pula
    b20e:	8f          	xgdx
    b20f:	83 01 30    	subd	#0x130
    b212:	c3 50 40    	addd	#0x5040
    b215:	8f          	xgdx
    b216:	a7 00       	staa	0x0,x
    b218:	bd e9 48    	jsr	0xe948
    b21b:	14 fb 80    	bset	*0xfb, #0x80
    b21e:	7e d3 b7    	jmp	0xd3b7
    b221:	7d 01 1c    	tst	0x11c
    b224:	2a 03       	bpl	0x0xb229
    b226:	7e d6 4e    	jmp	0xd64e
    b229:	7d 01 1e    	tst	0x11e
    b22c:	26 08       	bne	0x0xb236
    b22e:	7d 01 1c    	tst	0x11c
    b231:	27 1f       	beq	0x0xb252
    b233:	7e b1 32    	jmp	0xb132
    b236:	7d 01 1c    	tst	0x11c
    b239:	27 1f       	beq	0x0xb25a
    b23b:	cc 01 20    	ldd	#0x120
    b23e:	fb 01 1e    	addb	0x11e
    b241:	8f          	xgdx
    b242:	bd e9 0c    	jsr	0xe90c
    b245:	7a 01 1e    	dec	0x11e
    b248:	7a 01 1c    	dec	0x11c
    b24b:	26 0d       	bne	0x0xb25a
    b24d:	bd e9 8a    	jsr	0xe98a
    b250:	20 08       	bra	0x0xb25a
    b252:	86 13       	ldaa	#0x13
    b254:	b7 01 1e    	staa	0x11e
    b257:	bd e9 48    	jsr	0xe948
    b25a:	7d 01 7f    	tst	0x17f
    b25d:	27 10       	beq	0x0xb26f
    b25f:	bd eb 4c    	jsr	0xeb4c
    b262:	4f          	clra
    b263:	b7 01 7f    	staa	0x17f
    b266:	b7 01 7e    	staa	0x17e
    b269:	b7 01 1a    	staa	0x11a
    b26c:	7e d3 b7    	jmp	0xd3b7
    b26f:	7d 01 1a    	tst	0x11a
    b272:	26 03       	bne	0x0xb277
    b274:	7e d3 b7    	jmp	0xd3b7
    b277:	b6 01 1e    	ldaa	0x11e
    b27a:	81 13       	cmpa	#0x13
    b27c:	26 46       	bne	0x0xb2c4
    b27e:	b6 01 1a    	ldaa	0x11a
    b281:	2b 3b       	bmi	0x0xb2be
    b283:	f6 01 6f    	ldab	0x16f
    b286:	5c          	incb
    b287:	c4 0f       	andb	#0xf
    b289:	f7 01 6f    	stab	0x16f
    b28c:	b6 10 00    	ldaa	0x1000
    b28f:	36          	psha
    b290:	84 ef       	anda	#0xef
    b292:	b7 10 00    	staa	0x1000
    b295:	b6 10 08    	ldaa	0x1008
    b298:	36          	psha
    b299:	84 1f       	anda	#0x1f
    b29b:	b7 10 08    	staa	0x1008
    b29e:	f7 7f fe    	stab	0x7ffe
    b2a1:	32          	pula
    b2a2:	b7 10 08    	staa	0x1008
    b2a5:	32          	pula
    b2a6:	b7 10 00    	staa	0x1000
    b2a9:	ce d6 ac    	ldx	#0xd6ac
    b2ac:	18 ce 01 30 	ldy	#0x130
    b2b0:	bd ea d3    	jsr	0xead3
    b2b3:	7f 01 1a    	clr	0x11a
    b2b6:	86 04       	ldaa	#0x4
    b2b8:	b7 01 1c    	staa	0x11c
    b2bb:	7e d3 b7    	jmp	0xd3b7
    b2be:	f6 01 6f    	ldab	0x16f
    b2c1:	5a          	decb
    b2c2:	20 c3       	bra	0x0xb287
    b2c4:	81 19       	cmpa	#0x19
    b2c6:	26 34       	bne	0x0xb2fc
    b2c8:	b6 01 1a    	ldaa	0x11a
    b2cb:	2b 27       	bmi	0x0xb2f4
    b2cd:	d6 93       	ldab	*0x93
    b2cf:	5c          	incb
    b2d0:	c1 05       	cmpb	#0x5
    b2d2:	25 02       	bcs	0x0xb2d6
    b2d4:	c6 04       	ldab	#0x4
    b2d6:	d7 93       	stab	*0x93
    b2d8:	ce d6 f0    	ldx	#0xd6f0
    b2db:	18 ce 01 36 	ldy	#0x136
    b2df:	bd ea d3    	jsr	0xead3
    b2e2:	7f 01 1a    	clr	0x11a
    b2e5:	86 04       	ldaa	#0x4
    b2e7:	b7 01 1c    	staa	0x11c
    b2ea:	96 93       	ldaa	*0x93
    b2ec:	c6 93       	ldab	#0x93
    b2ee:	bd af d2    	jsr	0xafd2
    b2f1:	7e d3 b7    	jmp	0xd3b7
    b2f4:	d6 93       	ldab	*0x93
    b2f6:	5a          	decb
    b2f7:	2a dd       	bpl	0x0xb2d6
    b2f9:	5f          	clrb
    b2fa:	20 da       	bra	0x0xb2d6
    b2fc:	96 f9       	ldaa	*0xf9
    b2fe:	81 04       	cmpa	#0x4
    b300:	26 03       	bne	0x0xb305
    b302:	7e b3 6e    	jmp	0xb36e
    b305:	ce 00 20    	ldx	#0x20
    b308:	18 ce ee 72 	ldy	#0xee72
    b30c:	c6 b0       	ldab	#0xb0
    b30e:	18 a6 00    	ldaa	0x0,y
    b311:	a7 00       	staa	0x0,x
    b313:	08          	inx
    b314:	18 08       	iny
    b316:	5a          	decb
    b317:	26 f5       	bne	0x0xb30e
    b319:	7d 00 d0    	tst	0xd0
    b31c:	26 0b       	bne	0x0xb329
    b31e:	7d 10 29    	tst	0x1029
    b321:	2a fb       	bpl	0x0xb31e
    b323:	b6 10 2a    	ldaa	0x102a
    b326:	7c 00 d0    	inc	0xd0
    b329:	4f          	clra
    b32a:	01          	nop
    b32b:	01          	nop
    b32c:	01          	nop
    b32d:	01          	nop
    b32e:	b7 10 42    	staa	0x1042
    b331:	01          	nop
    b332:	01          	nop
    b333:	01          	nop
    b334:	01          	nop
    b335:	01          	nop
    b336:	bd a1 d5    	jsr	0xa1d5
    b339:	4f          	clra
    b33a:	97 f2       	staa	*0xf2
    b33c:	97 f6       	staa	*0xf6
    b33e:	97 ff       	staa	*0xff
    b340:	b7 01 1b    	staa	0x11b
    b343:	97 fb       	staa	*0xfb
    b345:	96 41       	ldaa	*0x41
    b347:	84 10       	anda	#0x10
    b349:	48          	asla
    b34a:	48          	asla
    b34b:	48          	asla
    b34c:	9a 44       	oraa	*0x44
    b34e:	97 f7       	staa	*0xf7
    b350:	96 73       	ldaa	*0x73
    b352:	97 f3       	staa	*0xf3
    b354:	96 4a       	ldaa	*0x4a
    b356:	48          	asla
    b357:	48          	asla
    b358:	48          	asla
    b359:	48          	asla
    b35a:	48          	asla
    b35b:	97 f4       	staa	*0xf4
    b35d:	96 6a       	ldaa	*0x6a
    b35f:	84 20       	anda	#0x20
    b361:	97 f5       	staa	*0xf5
    b363:	86 80       	ldaa	#0x80
    b365:	b7 01 1c    	staa	0x11c
    b368:	7f 01 1a    	clr	0x11a
    b36b:	7e d3 b7    	jmp	0xd3b7
    b36e:	ce 50 00    	ldx	#0x5000
    b371:	18 ce ef 22 	ldy	#0xef22
    b375:	c6 50       	ldab	#0x50
    b377:	18 a6 00    	ldaa	0x0,y
    b37a:	a7 00       	staa	0x0,x
    b37c:	08          	inx
    b37d:	18 08       	iny
    b37f:	5a          	decb
    b380:	26 f5       	bne	0x0xb377
    b382:	bd a3 32    	jsr	0xa332
    b385:	86 80       	ldaa	#0x80
    b387:	b7 01 1c    	staa	0x11c
    b38a:	7f 01 1a    	clr	0x11a
    b38d:	7f 01 1b    	clr	0x11b
    b390:	7e d3 b7    	jmp	0xd3b7
    b393:	7d 01 1c    	tst	0x11c
    b396:	2a 03       	bpl	0x0xb39b
    b398:	7e d7 0a    	jmp	0xd70a
    b39b:	7d 01 1e    	tst	0x11e
    b39e:	26 08       	bne	0x0xb3a8
    b3a0:	7d 01 1c    	tst	0x11c
    b3a3:	27 1f       	beq	0x0xb3c4
    b3a5:	7e b1 32    	jmp	0xb132
    b3a8:	7d 01 1c    	tst	0x11c
    b3ab:	27 24       	beq	0x0xb3d1
    b3ad:	cc 01 20    	ldd	#0x120
    b3b0:	fb 01 1e    	addb	0x11e
    b3b3:	8f          	xgdx
    b3b4:	bd e9 0c    	jsr	0xe90c
    b3b7:	7a 01 1e    	dec	0x11e
    b3ba:	7a 01 1c    	dec	0x11c
    b3bd:	26 12       	bne	0x0xb3d1
    b3bf:	bd e9 8a    	jsr	0xe98a
    b3c2:	20 0d       	bra	0x0xb3d1
    b3c4:	7d 01 1e    	tst	0x11e
    b3c7:	26 08       	bne	0x0xb3d1
    b3c9:	86 13       	ldaa	#0x13
    b3cb:	b7 01 1e    	staa	0x11e
    b3ce:	bd e9 48    	jsr	0xe948
    b3d1:	7d 01 7f    	tst	0x17f
    b3d4:	27 10       	beq	0x0xb3e6
    b3d6:	bd eb 4c    	jsr	0xeb4c
    b3d9:	4f          	clra
    b3da:	b7 01 7f    	staa	0x17f
    b3dd:	b7 01 7e    	staa	0x17e
    b3e0:	b7 01 1a    	staa	0x11a
    b3e3:	7e d3 b7    	jmp	0xd3b7
    b3e6:	7d 01 1a    	tst	0x11a
    b3e9:	26 03       	bne	0x0xb3ee
    b3eb:	7e d3 b7    	jmp	0xd3b7
    b3ee:	b6 01 1e    	ldaa	0x11e
    b3f1:	81 13       	cmpa	#0x13
    b3f3:	26 4d       	bne	0x0xb442
    b3f5:	b6 01 1a    	ldaa	0x11a
    b3f8:	2b 3e       	bmi	0x0xb438
    b3fa:	f6 01 70    	ldab	0x170
    b3fd:	5c          	incb
    b3fe:	c1 03       	cmpb	#0x3
    b400:	25 01       	bcs	0x0xb403
    b402:	5f          	clrb
    b403:	f7 01 70    	stab	0x170
    b406:	b6 10 00    	ldaa	0x1000
    b409:	36          	psha
    b40a:	84 ef       	anda	#0xef
    b40c:	b7 10 00    	staa	0x1000
    b40f:	b6 10 08    	ldaa	0x1008
    b412:	36          	psha
    b413:	84 1f       	anda	#0x1f
    b415:	b7 10 08    	staa	0x1008
    b418:	f7 7f ff    	stab	0x7fff
    b41b:	32          	pula
    b41c:	b7 10 08    	staa	0x1008
    b41f:	32          	pula
    b420:	b7 10 00    	staa	0x1000
    b423:	ce d7 66    	ldx	#0xd766
    b426:	18 ce 01 30 	ldy	#0x130
    b42a:	bd ea d3    	jsr	0xead3
    b42d:	7f 01 1a    	clr	0x11a
    b430:	86 04       	ldaa	#0x4
    b432:	b7 01 1c    	staa	0x11c
    b435:	7e d3 b7    	jmp	0xd3b7
    b438:	f6 01 70    	ldab	0x170
    b43b:	5a          	decb
    b43c:	2a c5       	bpl	0x0xb403
    b43e:	c6 02       	ldab	#0x2
    b440:	20 c1       	bra	0x0xb403
    b442:	81 19       	cmpa	#0x19
    b444:	26 2d       	bne	0x0xb473
    b446:	b6 01 1a    	ldaa	0x11a
    b449:	2b 1f       	bmi	0x0xb46a
    b44b:	d6 94       	ldab	*0x94
    b44d:	5c          	incb
    b44e:	c1 03       	cmpb	#0x3
    b450:	25 01       	bcs	0x0xb453
    b452:	5f          	clrb
    b453:	d7 94       	stab	*0x94
    b455:	ce d7 72    	ldx	#0xd772
    b458:	18 ce 01 36 	ldy	#0x136
    b45c:	bd ea d3    	jsr	0xead3
    b45f:	7f 01 1a    	clr	0x11a
    b462:	86 04       	ldaa	#0x4
    b464:	b7 01 1c    	staa	0x11c
    b467:	7e d3 b7    	jmp	0xd3b7
    b46a:	d6 94       	ldab	*0x94
    b46c:	5a          	decb
    b46d:	2a e4       	bpl	0x0xb453
    b46f:	c6 02       	ldab	#0x2
    b471:	20 e0       	bra	0x0xb453
    b473:	b6 01 1a    	ldaa	0x11a
    b476:	2b 3e       	bmi	0x0xb4b6
    b478:	b6 01 71    	ldaa	0x171
    b47b:	4c          	inca
    b47c:	81 32       	cmpa	#0x32
    b47e:	23 02       	bls	0x0xb482
    b480:	86 32       	ldaa	#0x32
    b482:	b7 01 71    	staa	0x171
    b485:	b7 10 46    	staa	0x1046
    b488:	f6 10 00    	ldab	0x1000
    b48b:	37          	pshb
    b48c:	c4 ef       	andb	#0xef
    b48e:	f7 10 00    	stab	0x1000
    b491:	f6 10 08    	ldab	0x1008
    b494:	37          	pshb
    b495:	c4 1f       	andb	#0x1f
    b497:	f7 10 08    	stab	0x1008
    b49a:	b7 7f fc    	staa	0x7ffc
    b49d:	33          	pulb
    b49e:	f7 10 08    	stab	0x1008
    b4a1:	33          	pulb
    b4a2:	f7 10 00    	stab	0x1000
    b4a5:	ce 01 3c    	ldx	#0x13c
    b4a8:	bd ea 20    	jsr	0xea20
    b4ab:	7f 01 1a    	clr	0x11a
    b4ae:	86 03       	ldaa	#0x3
    b4b0:	b7 01 1c    	staa	0x11c
    b4b3:	7e d3 b7    	jmp	0xd3b7
    b4b6:	b6 01 71    	ldaa	0x171
    b4b9:	4a          	deca
    b4ba:	2a c6       	bpl	0x0xb482
    b4bc:	4f          	clra
    b4bd:	20 c3       	bra	0x0xb482
    b4bf:	7d 01 1c    	tst	0x11c
    b4c2:	2a 03       	bpl	0x0xb4c7
    b4c4:	7e d7 7e    	jmp	0xd77e
    b4c7:	7d 01 1e    	tst	0x11e
    b4ca:	26 08       	bne	0x0xb4d4
    b4cc:	7d 01 1c    	tst	0x11c
    b4cf:	27 1f       	beq	0x0xb4f0
    b4d1:	7e b1 32    	jmp	0xb132
    b4d4:	7d 01 1c    	tst	0x11c
    b4d7:	27 24       	beq	0x0xb4fd
    b4d9:	cc 01 20    	ldd	#0x120
    b4dc:	fb 01 1e    	addb	0x11e
    b4df:	8f          	xgdx
    b4e0:	bd e9 0c    	jsr	0xe90c
    b4e3:	7a 01 1e    	dec	0x11e
    b4e6:	7a 01 1c    	dec	0x11c
    b4e9:	26 12       	bne	0x0xb4fd
    b4eb:	bd e9 8a    	jsr	0xe98a
    b4ee:	20 0d       	bra	0x0xb4fd
    b4f0:	7d 01 1e    	tst	0x11e
    b4f3:	26 08       	bne	0x0xb4fd
    b4f5:	86 1e       	ldaa	#0x1e
    b4f7:	b7 01 1e    	staa	0x11e
    b4fa:	bd e9 48    	jsr	0xe948
    b4fd:	7f 01 7e    	clr	0x17e
    b500:	7f 01 7f    	clr	0x17f
    b503:	7d 01 1a    	tst	0x11a
    b506:	26 03       	bne	0x0xb50b
    b508:	7e d3 b7    	jmp	0xd3b7
    b50b:	b6 01 3c    	ldaa	0x13c
    b50e:	81 41       	cmpa	#0x41
    b510:	26 04       	bne	0x0xb516
    b512:	86 81       	ldaa	#0x81
    b514:	20 06       	bra	0x0xb51c
    b516:	ce 01 3c    	ldx	#0x13c
    b519:	bd ea 7a    	jsr	0xea7a
    b51c:	7d 01 1a    	tst	0x11a
    b51f:	2b 09       	bmi	0x0xb52a
    b521:	4c          	inca
    b522:	81 81       	cmpa	#0x81
    b524:	23 09       	bls	0x0xb52f
    b526:	86 01       	ldaa	#0x1
    b528:	20 05       	bra	0x0xb52f
    b52a:	4a          	deca
    b52b:	26 02       	bne	0x0xb52f
    b52d:	86 81       	ldaa	#0x81
    b52f:	81 81       	cmpa	#0x81
    b531:	26 0f       	bne	0x0xb542
    b533:	86 41       	ldaa	#0x41
    b535:	b7 01 3c    	staa	0x13c
    b538:	86 4c       	ldaa	#0x4c
    b53a:	b7 01 3d    	staa	0x13d
    b53d:	b7 01 3e    	staa	0x13e
    b540:	20 06       	bra	0x0xb548
    b542:	ce 01 3c    	ldx	#0x13c
    b545:	bd ea 20    	jsr	0xea20
    b548:	7f 01 1a    	clr	0x11a
    b54b:	86 03       	ldaa	#0x3
    b54d:	b7 01 1c    	staa	0x11c
    b550:	14 f2 40    	bset	*0xf2, #0x40
    b553:	7e d3 b7    	jmp	0xd3b7
    b556:	7d 01 1c    	tst	0x11c
    b559:	2a 03       	bpl	0x0xb55e
    b55b:	7e d8 09    	jmp	0xd809
    b55e:	7d 01 1e    	tst	0x11e
    b561:	26 08       	bne	0x0xb56b
    b563:	7d 01 1c    	tst	0x11c
    b566:	27 1f       	beq	0x0xb587
    b568:	7e b1 32    	jmp	0xb132
    b56b:	7d 01 1c    	tst	0x11c
    b56e:	27 24       	beq	0x0xb594
    b570:	cc 01 20    	ldd	#0x120
    b573:	fb 01 1e    	addb	0x11e
    b576:	8f          	xgdx
    b577:	bd e9 0c    	jsr	0xe90c
    b57a:	7a 01 1e    	dec	0x11e
    b57d:	7a 01 1c    	dec	0x11c
    b580:	26 12       	bne	0x0xb594
    b582:	bd e9 8a    	jsr	0xe98a
    b585:	20 0d       	bra	0x0xb594
    b587:	7d 01 1e    	tst	0x11e
    b58a:	26 08       	bne	0x0xb594
    b58c:	86 13       	ldaa	#0x13
    b58e:	b7 01 1e    	staa	0x11e
    b591:	bd e9 48    	jsr	0xe948
    b594:	7d 01 7f    	tst	0x17f
    b597:	27 10       	beq	0x0xb5a9
    b599:	bd eb 4c    	jsr	0xeb4c
    b59c:	4f          	clra
    b59d:	b7 01 7f    	staa	0x17f
    b5a0:	b7 01 7e    	staa	0x17e
    b5a3:	b7 01 1a    	staa	0x11a
    b5a6:	7e d3 b7    	jmp	0xd3b7
    b5a9:	7d 01 1a    	tst	0x11a
    b5ac:	26 03       	bne	0x0xb5b1
    b5ae:	7e d3 b7    	jmp	0xd3b7
    b5b1:	b6 01 1e    	ldaa	0x11e
    b5b4:	81 13       	cmpa	#0x13
    b5b6:	26 30       	bne	0x0xb5e8
    b5b8:	b6 01 1a    	ldaa	0x11a
    b5bb:	2b 23       	bmi	0x0xb5e0
    b5bd:	96 40       	ldaa	*0x40
    b5bf:	4c          	inca
    b5c0:	81 7f       	cmpa	#0x7f
    b5c2:	25 02       	bcs	0x0xb5c6
    b5c4:	86 7f       	ldaa	#0x7f
    b5c6:	97 40       	staa	*0x40
    b5c8:	ce 01 31    	ldx	#0x131
    b5cb:	bd ea 5c    	jsr	0xea5c
    b5ce:	7f 01 1a    	clr	0x11a
    b5d1:	86 03       	ldaa	#0x3
    b5d3:	b7 01 1c    	staa	0x11c
    b5d6:	96 40       	ldaa	*0x40
    b5d8:	c6 40       	ldab	#0x40
    b5da:	bd af d2    	jsr	0xafd2
    b5dd:	7e d3 b7    	jmp	0xd3b7
    b5e0:	96 40       	ldaa	*0x40
    b5e2:	4a          	deca
    b5e3:	2a e1       	bpl	0x0xb5c6
    b5e5:	4f          	clra
    b5e6:	20 de       	bra	0x0xb5c6
    b5e8:	81 19       	cmpa	#0x19
    b5ea:	26 3d       	bne	0x0xb629
    b5ec:	b6 01 1a    	ldaa	0x11a
    b5ef:	2b 2e       	bmi	0x0xb61f
    b5f1:	d6 41       	ldab	*0x41
    b5f3:	c4 0f       	andb	#0xf
    b5f5:	5c          	incb
    b5f6:	c1 05       	cmpb	#0x5
    b5f8:	25 02       	bcs	0x0xb5fc
    b5fa:	c6 04       	ldab	#0x4
    b5fc:	96 41       	ldaa	*0x41
    b5fe:	84 10       	anda	#0x10
    b600:	1b          	aba
    b601:	97 41       	staa	*0x41
    b603:	ce d8 70    	ldx	#0xd870
    b606:	18 ce 01 36 	ldy	#0x136
    b60a:	bd ea d3    	jsr	0xead3
    b60d:	7f 01 1a    	clr	0x11a
    b610:	86 04       	ldaa	#0x4
    b612:	b7 01 1c    	staa	0x11c
    b615:	96 41       	ldaa	*0x41
    b617:	c6 41       	ldab	#0x41
    b619:	bd af d2    	jsr	0xafd2
    b61c:	7e d3 b7    	jmp	0xd3b7
    b61f:	d6 41       	ldab	*0x41
    b621:	c4 0f       	andb	#0xf
    b623:	5a          	decb
    b624:	2a d6       	bpl	0x0xb5fc
    b626:	5f          	clrb
    b627:	20 d3       	bra	0x0xb5fc
    b629:	b6 01 1a    	ldaa	0x11a
    b62c:	2b 1f       	bmi	0x0xb64d
    b62e:	96 67       	ldaa	*0x67
    b630:	4c          	inca
    b631:	84 7f       	anda	#0x7f
    b633:	97 67       	staa	*0x67
    b635:	ce 01 3c    	ldx	#0x13c
    b638:	bd ea 20    	jsr	0xea20
    b63b:	7f 01 1a    	clr	0x11a
    b63e:	86 03       	ldaa	#0x3
    b640:	b7 01 1c    	staa	0x11c
    b643:	96 67       	ldaa	*0x67
    b645:	c6 67       	ldab	#0x67
    b647:	bd af d2    	jsr	0xafd2
    b64a:	7e d3 b7    	jmp	0xd3b7
    b64d:	96 67       	ldaa	*0x67
    b64f:	4a          	deca
    b650:	84 7f       	anda	#0x7f
    b652:	20 df       	bra	0x0xb633
    b654:	7d 01 1c    	tst	0x11c
    b657:	2a 03       	bpl	0x0xb65c
    b659:	7e d8 84    	jmp	0xd884
    b65c:	7d 01 1e    	tst	0x11e
    b65f:	26 08       	bne	0x0xb669
    b661:	7d 01 1c    	tst	0x11c
    b664:	27 1f       	beq	0x0xb685
    b666:	7e b1 32    	jmp	0xb132
    b669:	7d 01 1c    	tst	0x11c
    b66c:	27 24       	beq	0x0xb692
    b66e:	cc 01 20    	ldd	#0x120
    b671:	fb 01 1e    	addb	0x11e
    b674:	8f          	xgdx
    b675:	bd e9 0c    	jsr	0xe90c
    b678:	7a 01 1e    	dec	0x11e
    b67b:	7a 01 1c    	dec	0x11c
    b67e:	26 12       	bne	0x0xb692
    b680:	bd e9 8a    	jsr	0xe98a
    b683:	20 0d       	bra	0x0xb692
    b685:	7d 01 1e    	tst	0x11e
    b688:	26 08       	bne	0x0xb692
    b68a:	86 13       	ldaa	#0x13
    b68c:	b7 01 1e    	staa	0x11e
    b68f:	bd e9 48    	jsr	0xe948
    b692:	7d 01 7f    	tst	0x17f
    b695:	27 10       	beq	0x0xb6a7
    b697:	bd eb 4c    	jsr	0xeb4c
    b69a:	4f          	clra
    b69b:	b7 01 7f    	staa	0x17f
    b69e:	b7 01 7e    	staa	0x17e
    b6a1:	b7 01 1a    	staa	0x11a
    b6a4:	7e d3 b7    	jmp	0xd3b7
    b6a7:	7d 01 1a    	tst	0x11a
    b6aa:	26 03       	bne	0x0xb6af
    b6ac:	7e d3 b7    	jmp	0xd3b7
    b6af:	b6 01 1e    	ldaa	0x11e
    b6b2:	81 13       	cmpa	#0x13
    b6b4:	26 51       	bne	0x0xb707
    b6b6:	b6 01 1a    	ldaa	0x11a
    b6b9:	2b 43       	bmi	0x0xb6fe
    b6bb:	b6 01 6e    	ldaa	0x16e
    b6be:	4c          	inca
    b6bf:	81 7f       	cmpa	#0x7f
    b6c1:	25 02       	bcs	0x0xb6c5
    b6c3:	86 7f       	ldaa	#0x7f
    b6c5:	b7 01 6e    	staa	0x16e
    b6c8:	f6 10 00    	ldab	0x1000
    b6cb:	37          	pshb
    b6cc:	c4 ef       	andb	#0xef
    b6ce:	f7 10 00    	stab	0x1000
    b6d1:	f6 10 08    	ldab	0x1008
    b6d4:	37          	pshb
    b6d5:	c4 1f       	andb	#0x1f
    b6d7:	f7 10 08    	stab	0x1008
    b6da:	b7 7f fd    	staa	0x7ffd
    b6dd:	33          	pulb
    b6de:	f7 10 08    	stab	0x1008
    b6e1:	33          	pulb
    b6e2:	f7 10 00    	stab	0x1000
    b6e5:	ce 01 31    	ldx	#0x131
    b6e8:	bd ea 5c    	jsr	0xea5c
    b6eb:	7f 01 1a    	clr	0x11a
    b6ee:	86 03       	ldaa	#0x3
    b6f0:	b7 01 1c    	staa	0x11c
    b6f3:	b6 01 6e    	ldaa	0x16e
    b6f6:	c6 ab       	ldab	#0xab
    b6f8:	bd af d2    	jsr	0xafd2
    b6fb:	7e d3 b7    	jmp	0xd3b7
    b6fe:	b6 01 6e    	ldaa	0x16e
    b701:	4a          	deca
    b702:	2a c1       	bpl	0x0xb6c5
    b704:	4f          	clra
    b705:	20 be       	bra	0x0xb6c5
    b707:	81 19       	cmpa	#0x19
    b709:	26 30       	bne	0x0xb73b
    b70b:	b6 01 1a    	ldaa	0x11a
    b70e:	2b 23       	bmi	0x0xb733
    b710:	96 3e       	ldaa	*0x3e
    b712:	4c          	inca
    b713:	81 40       	cmpa	#0x40
    b715:	25 02       	bcs	0x0xb719
    b717:	86 3f       	ldaa	#0x3f
    b719:	97 3e       	staa	*0x3e
    b71b:	ce 01 37    	ldx	#0x137
    b71e:	bd ea 20    	jsr	0xea20
    b721:	7f 01 1a    	clr	0x11a
    b724:	86 03       	ldaa	#0x3
    b726:	b7 01 1c    	staa	0x11c
    b729:	96 3e       	ldaa	*0x3e
    b72b:	c6 3e       	ldab	#0x3e
    b72d:	bd af d2    	jsr	0xafd2
    b730:	7e d3 b7    	jmp	0xd3b7
    b733:	96 3e       	ldaa	*0x3e
    b735:	4a          	deca
    b736:	2a e1       	bpl	0x0xb719
    b738:	4f          	clra
    b739:	20 de       	bra	0x0xb719
    b73b:	b6 01 1a    	ldaa	0x11a
    b73e:	2b 23       	bmi	0x0xb763
    b740:	96 3f       	ldaa	*0x3f
    b742:	4c          	inca
    b743:	81 40       	cmpa	#0x40
    b745:	25 02       	bcs	0x0xb749
    b747:	86 3f       	ldaa	#0x3f
    b749:	97 3f       	staa	*0x3f
    b74b:	ce 01 3c    	ldx	#0x13c
    b74e:	bd ea 20    	jsr	0xea20
    b751:	7f 01 1a    	clr	0x11a
    b754:	86 03       	ldaa	#0x3
    b756:	b7 01 1c    	staa	0x11c
    b759:	96 3f       	ldaa	*0x3f
    b75b:	c6 3f       	ldab	#0x3f
    b75d:	bd af d2    	jsr	0xafd2
    b760:	7e d3 b7    	jmp	0xd3b7
    b763:	96 3f       	ldaa	*0x3f
    b765:	4a          	deca
    b766:	2a e1       	bpl	0x0xb749
    b768:	4f          	clra
    b769:	20 de       	bra	0x0xb749
    b76b:	7d 01 1c    	tst	0x11c
    b76e:	2a 03       	bpl	0x0xb773
    b770:	7e d8 e6    	jmp	0xd8e6
    b773:	7d 01 1e    	tst	0x11e
    b776:	26 08       	bne	0x0xb780
    b778:	7d 01 1c    	tst	0x11c
    b77b:	27 1f       	beq	0x0xb79c
    b77d:	7e b1 32    	jmp	0xb132
    b780:	7d 01 1c    	tst	0x11c
    b783:	27 24       	beq	0x0xb7a9
    b785:	cc 01 20    	ldd	#0x120
    b788:	fb 01 1e    	addb	0x11e
    b78b:	8f          	xgdx
    b78c:	bd e9 0c    	jsr	0xe90c
    b78f:	7a 01 1e    	dec	0x11e
    b792:	7a 01 1c    	dec	0x11c
    b795:	26 12       	bne	0x0xb7a9
    b797:	bd e9 bd    	jsr	0xe9bd
    b79a:	20 0d       	bra	0x0xb7a9
    b79c:	7d 01 1e    	tst	0x11e
    b79f:	26 08       	bne	0x0xb7a9
    b7a1:	86 12       	ldaa	#0x12
    b7a3:	b7 01 1e    	staa	0x11e
    b7a6:	bd e9 48    	jsr	0xe948
    b7a9:	7d 01 7f    	tst	0x17f
    b7ac:	27 10       	beq	0x0xb7be
    b7ae:	bd eb be    	jsr	0xebbe
    b7b1:	4f          	clra
    b7b2:	b7 01 7f    	staa	0x17f
    b7b5:	b7 01 7e    	staa	0x17e
    b7b8:	b7 01 1a    	staa	0x11a
    b7bb:	7e d3 b7    	jmp	0xd3b7
    b7be:	7d 01 1a    	tst	0x11a
    b7c1:	26 03       	bne	0x0xb7c6
    b7c3:	7e d3 b7    	jmp	0xd3b7
    b7c6:	b6 01 1e    	ldaa	0x11e
    b7c9:	81 12       	cmpa	#0x12
    b7cb:	26 2a       	bne	0x0xb7f7
    b7cd:	d6 21       	ldab	*0x21
    b7cf:	c8 40       	eorb	#0x40
    b7d1:	d7 21       	stab	*0x21
    b7d3:	c4 40       	andb	#0x40
    b7d5:	54          	lsrb
    b7d6:	54          	lsrb
    b7d7:	54          	lsrb
    b7d8:	54          	lsrb
    b7d9:	54          	lsrb
    b7da:	54          	lsrb
    b7db:	ce d9 74    	ldx	#0xd974
    b7de:	18 ce 01 30 	ldy	#0x130
    b7e2:	bd ea bf    	jsr	0xeabf
    b7e5:	7f 01 1a    	clr	0x11a
    b7e8:	86 03       	ldaa	#0x3
    b7ea:	b7 01 1c    	staa	0x11c
    b7ed:	96 21       	ldaa	*0x21
    b7ef:	c6 21       	ldab	#0x21
    b7f1:	bd af d2    	jsr	0xafd2
    b7f4:	7e d3 b7    	jmp	0xd3b7
    b7f7:	81 16       	cmpa	#0x16
    b7f9:	26 02       	bne	0x0xb7fd
    b7fb:	20 e8       	bra	0x0xb7e5
    b7fd:	81 1a       	cmpa	#0x1a
    b7ff:	26 14       	bne	0x0xb815
    b801:	d6 21       	ldab	*0x21
    b803:	c8 01       	eorb	#0x1
    b805:	d7 21       	stab	*0x21
    b807:	c4 01       	andb	#0x1
    b809:	18 ce 01 38 	ldy	#0x138
    b80d:	ce d9 80    	ldx	#0xd980
    b810:	bd ea bf    	jsr	0xeabf
    b813:	20 d0       	bra	0x0xb7e5
    b815:	7d 01 1a    	tst	0x11a
    b818:	2b 27       	bmi	0x0xb841
    b81a:	d6 24       	ldab	*0x24
    b81c:	5c          	incb
    b81d:	c1 07       	cmpb	#0x7
    b81f:	25 02       	bcs	0x0xb823
    b821:	c6 06       	ldab	#0x6
    b823:	d7 24       	stab	*0x24
    b825:	ce d9 86    	ldx	#0xd986
    b828:	18 ce 01 3c 	ldy	#0x13c
    b82c:	bd ea bf    	jsr	0xeabf
    b82f:	7f 01 1a    	clr	0x11a
    b832:	86 03       	ldaa	#0x3
    b834:	b7 01 1c    	staa	0x11c
    b837:	96 24       	ldaa	*0x24
    b839:	c6 24       	ldab	#0x24
    b83b:	bd af d2    	jsr	0xafd2
    b83e:	7e d3 b7    	jmp	0xd3b7
    b841:	d6 24       	ldab	*0x24
    b843:	5a          	decb
    b844:	2a dd       	bpl	0x0xb823
    b846:	5f          	clrb
    b847:	20 da       	bra	0x0xb823
    b849:	7d 01 1c    	tst	0x11c
    b84c:	2a 03       	bpl	0x0xb851
    b84e:	7e d9 9b    	jmp	0xd99b
    b851:	7d 01 1e    	tst	0x11e
    b854:	26 08       	bne	0x0xb85e
    b856:	7d 01 1c    	tst	0x11c
    b859:	27 1f       	beq	0x0xb87a
    b85b:	7e b1 32    	jmp	0xb132
    b85e:	7d 01 1c    	tst	0x11c
    b861:	27 24       	beq	0x0xb887
    b863:	cc 01 20    	ldd	#0x120
    b866:	fb 01 1e    	addb	0x11e
    b869:	8f          	xgdx
    b86a:	bd e9 0c    	jsr	0xe90c
    b86d:	7a 01 1e    	dec	0x11e
    b870:	7a 01 1c    	dec	0x11c
    b873:	26 12       	bne	0x0xb887
    b875:	bd e9 8a    	jsr	0xe98a
    b878:	20 0d       	bra	0x0xb887
    b87a:	7d 01 1e    	tst	0x11e
    b87d:	26 08       	bne	0x0xb887
    b87f:	86 13       	ldaa	#0x13
    b881:	b7 01 1e    	staa	0x11e
    b884:	bd e9 48    	jsr	0xe948
    b887:	7d 01 7f    	tst	0x17f
    b88a:	27 10       	beq	0x0xb89c
    b88c:	bd eb 4c    	jsr	0xeb4c
    b88f:	4f          	clra
    b890:	b7 01 7f    	staa	0x17f
    b893:	b7 01 7e    	staa	0x17e
    b896:	b7 01 1a    	staa	0x11a
    b899:	7e d3 b7    	jmp	0xd3b7
    b89c:	7d 01 1a    	tst	0x11a
    b89f:	26 03       	bne	0x0xb8a4
    b8a1:	7e d3 b7    	jmp	0xd3b7
    b8a4:	b6 01 1e    	ldaa	0x11e
    b8a7:	81 13       	cmpa	#0x13
    b8a9:	26 3d       	bne	0x0xb8e8
    b8ab:	b6 01 1a    	ldaa	0x11a
    b8ae:	2b 2a       	bmi	0x0xb8da
    b8b0:	96 6a       	ldaa	*0x6a
    b8b2:	84 20       	anda	#0x20
    b8b4:	d6 6a       	ldab	*0x6a
    b8b6:	c4 0f       	andb	#0xf
    b8b8:	5c          	incb
    b8b9:	c1 05       	cmpb	#0x5
    b8bb:	25 02       	bcs	0x0xb8bf
    b8bd:	c6 04       	ldab	#0x4
    b8bf:	1b          	aba
    b8c0:	16          	tab
    b8c1:	d7 6a       	stab	*0x6a
    b8c3:	c4 0f       	andb	#0xf
    b8c5:	ce da 0a    	ldx	#0xda0a
    b8c8:	18 ce 01 30 	ldy	#0x130
    b8cc:	bd ea d3    	jsr	0xead3
    b8cf:	7f 01 1a    	clr	0x11a
    b8d2:	86 04       	ldaa	#0x4
    b8d4:	b7 01 1c    	staa	0x11c
    b8d7:	7e d3 b7    	jmp	0xd3b7
    b8da:	96 6a       	ldaa	*0x6a
    b8dc:	84 20       	anda	#0x20
    b8de:	d6 6a       	ldab	*0x6a
    b8e0:	c4 0f       	andb	#0xf
    b8e2:	5a          	decb
    b8e3:	2a da       	bpl	0x0xb8bf
    b8e5:	5f          	clrb
    b8e6:	20 d7       	bra	0x0xb8bf
    b8e8:	81 19       	cmpa	#0x19
    b8ea:	26 1c       	bne	0x0xb908
    b8ec:	d6 95       	ldab	*0x95
    b8ee:	5c          	incb
    b8ef:	c4 01       	andb	#0x1
    b8f1:	d7 95       	stab	*0x95
    b8f3:	ce da 1e    	ldx	#0xda1e
    b8f6:	18 ce 01 36 	ldy	#0x136
    b8fa:	bd ea d3    	jsr	0xead3
    b8fd:	7f 01 1a    	clr	0x11a
    b900:	86 04       	ldaa	#0x4
    b902:	b7 01 1c    	staa	0x11c
    b905:	7e d3 b7    	jmp	0xd3b7
    b908:	d6 69       	ldab	*0x69
    b90a:	5c          	incb
    b90b:	c4 01       	andb	#0x1
    b90d:	d7 69       	stab	*0x69
    b90f:	ce da 26    	ldx	#0xda26
    b912:	18 ce 01 3b 	ldy	#0x13b
    b916:	bd ea d3    	jsr	0xead3
    b919:	7f 01 1a    	clr	0x11a
    b91c:	86 04       	ldaa	#0x4
    b91e:	b7 01 1c    	staa	0x11c
    b921:	96 69       	ldaa	*0x69
    b923:	c6 69       	ldab	#0x69
    b925:	bd af d2    	jsr	0xafd2
    b928:	7e d3 b7    	jmp	0xd3b7
    b92b:	7d 01 1c    	tst	0x11c
    b92e:	2a 03       	bpl	0x0xb933
    b930:	7e da 2e    	jmp	0xda2e
    b933:	7d 01 1e    	tst	0x11e
    b936:	26 08       	bne	0x0xb940
    b938:	7d 01 1c    	tst	0x11c
    b93b:	27 1f       	beq	0x0xb95c
    b93d:	7e b1 32    	jmp	0xb132
    b940:	7d 01 1c    	tst	0x11c
    b943:	27 24       	beq	0x0xb969
    b945:	cc 01 20    	ldd	#0x120
    b948:	fb 01 1e    	addb	0x11e
    b94b:	8f          	xgdx
    b94c:	bd e9 0c    	jsr	0xe90c
    b94f:	7a 01 1e    	dec	0x11e
    b952:	7a 01 1c    	dec	0x11c
    b955:	26 12       	bne	0x0xb969
    b957:	bd e9 8a    	jsr	0xe98a
    b95a:	20 0d       	bra	0x0xb969
    b95c:	7d 01 1e    	tst	0x11e
    b95f:	26 08       	bne	0x0xb969
    b961:	86 13       	ldaa	#0x13
    b963:	b7 01 1e    	staa	0x11e
    b966:	bd e9 48    	jsr	0xe948
    b969:	7d 01 7f    	tst	0x17f
    b96c:	27 1e       	beq	0x0xb98c
    b96e:	2a 10       	bpl	0x0xb980
    b970:	bd eb 4c    	jsr	0xeb4c
    b973:	4f          	clra
    b974:	b7 01 7f    	staa	0x17f
    b977:	b7 01 7e    	staa	0x17e
    b97a:	b7 01 1a    	staa	0x11a
    b97d:	7e d3 b7    	jmp	0xd3b7
    b980:	b6 01 1e    	ldaa	0x11e
    b983:	81 19       	cmpa	#0x19
    b985:	27 ec       	beq	0x0xb973
    b987:	bd eb 4c    	jsr	0xeb4c
    b98a:	20 e7       	bra	0x0xb973
    b98c:	7d 01 1a    	tst	0x11a
    b98f:	26 03       	bne	0x0xb994
    b991:	7e d3 b7    	jmp	0xd3b7
    b994:	b6 01 1e    	ldaa	0x11e
    b997:	81 13       	cmpa	#0x13
    b999:	26 34       	bne	0x0xb9cf
    b99b:	b6 01 1a    	ldaa	0x11a
    b99e:	2b 27       	bmi	0x0xb9c7
    b9a0:	d6 6b       	ldab	*0x6b
    b9a2:	5c          	incb
    b9a3:	c1 03       	cmpb	#0x3
    b9a5:	25 02       	bcs	0x0xb9a9
    b9a7:	c6 02       	ldab	#0x2
    b9a9:	d7 6b       	stab	*0x6b
    b9ab:	ce da 80    	ldx	#0xda80
    b9ae:	18 ce 01 30 	ldy	#0x130
    b9b2:	bd ea d3    	jsr	0xead3
    b9b5:	7f 01 1a    	clr	0x11a
    b9b8:	86 04       	ldaa	#0x4
    b9ba:	b7 01 1c    	staa	0x11c
    b9bd:	96 6b       	ldaa	*0x6b
    b9bf:	c6 6b       	ldab	#0x6b
    b9c1:	bd af d2    	jsr	0xafd2
    b9c4:	7e d3 b7    	jmp	0xd3b7
    b9c7:	d6 6b       	ldab	*0x6b
    b9c9:	5a          	decb
    b9ca:	2a dd       	bpl	0x0xb9a9
    b9cc:	5f          	clrb
    b9cd:	20 da       	bra	0x0xb9a9
    b9cf:	b6 01 1a    	ldaa	0x11a
    b9d2:	2b 27       	bmi	0x0xb9fb
    b9d4:	d6 68       	ldab	*0x68
    b9d6:	5c          	incb
    b9d7:	c1 07       	cmpb	#0x7
    b9d9:	23 02       	bls	0x0xb9dd
    b9db:	c6 07       	ldab	#0x7
    b9dd:	d7 68       	stab	*0x68
    b9df:	ce da 8c    	ldx	#0xda8c
    b9e2:	18 ce 01 36 	ldy	#0x136
    b9e6:	bd ea d3    	jsr	0xead3
    b9e9:	7f 01 1a    	clr	0x11a
    b9ec:	86 04       	ldaa	#0x4
    b9ee:	b7 01 1c    	staa	0x11c
    b9f1:	96 68       	ldaa	*0x68
    b9f3:	c6 68       	ldab	#0x68
    b9f5:	bd af d2    	jsr	0xafd2
    b9f8:	7e d3 b7    	jmp	0xd3b7
    b9fb:	d6 68       	ldab	*0x68
    b9fd:	5a          	decb
    b9fe:	2a dd       	bpl	0x0xb9dd
    ba00:	5f          	clrb
    ba01:	20 da       	bra	0x0xb9dd
    ba03:	7e d3 b7    	jmp	0xd3b7
    ba06:	7d 01 1c    	tst	0x11c
    ba09:	2a 03       	bpl	0x0xba0e
    ba0b:	7e da ac    	jmp	0xdaac
    ba0e:	7d 01 1e    	tst	0x11e
    ba11:	26 08       	bne	0x0xba1b
    ba13:	7d 01 1c    	tst	0x11c
    ba16:	27 1f       	beq	0x0xba37
    ba18:	7e b1 32    	jmp	0xb132
    ba1b:	7d 01 1c    	tst	0x11c
    ba1e:	27 24       	beq	0x0xba44
    ba20:	cc 01 20    	ldd	#0x120
    ba23:	fb 01 1e    	addb	0x11e
    ba26:	8f          	xgdx
    ba27:	bd e9 0c    	jsr	0xe90c
    ba2a:	7a 01 1e    	dec	0x11e
    ba2d:	7a 01 1c    	dec	0x11c
    ba30:	26 12       	bne	0x0xba44
    ba32:	bd e9 8a    	jsr	0xe98a
    ba35:	20 0d       	bra	0x0xba44
    ba37:	7d 01 1e    	tst	0x11e
    ba3a:	26 08       	bne	0x0xba44
    ba3c:	86 09       	ldaa	#0x9
    ba3e:	b7 01 1e    	staa	0x11e
    ba41:	bd e9 48    	jsr	0xe948
    ba44:	7d 01 7f    	tst	0x17f
    ba47:	27 10       	beq	0x0xba59
    ba49:	bd eb 0b    	jsr	0xeb0b
    ba4c:	4f          	clra
    ba4d:	b7 01 7f    	staa	0x17f
    ba50:	b7 01 7e    	staa	0x17e
    ba53:	b7 01 1a    	staa	0x11a
    ba56:	7e d3 b7    	jmp	0xd3b7
    ba59:	7d 01 7e    	tst	0x17e
    ba5c:	27 0c       	beq	0x0xba6a
    ba5e:	bd ea f4    	jsr	0xeaf4
    ba61:	7f 01 7e    	clr	0x17e
    ba64:	7f 01 1a    	clr	0x11a
    ba67:	7e d3 b7    	jmp	0xd3b7
    ba6a:	7d 01 1a    	tst	0x11a
    ba6d:	26 03       	bne	0x0xba72
    ba6f:	7e d3 b7    	jmp	0xd3b7
    ba72:	b6 01 1e    	ldaa	0x11e
    ba75:	81 09       	cmpa	#0x9
    ba77:	26 29       	bne	0x0xbaa2
    ba79:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xba83
    ba7d:	7f 01 1a    	clr	0x11a
    ba80:	7e d3 b7    	jmp	0xd3b7
    ba83:	ce 00 76    	ldx	#0x76
    ba86:	bd bb 1b    	jsr	0xbb1b
    ba89:	bd bb 4f    	jsr	0xbb4f
    ba8c:	16          	tab
    ba8d:	ce de db    	ldx	#0xdedb
    ba90:	18 ce 01 26 	ldy	#0x126
    ba94:	bd ea d3    	jsr	0xead3
    ba97:	86 04       	ldaa	#0x4
    ba99:	b7 01 1c    	staa	0x11c
    ba9c:	7f 01 1a    	clr	0x11a
    ba9f:	7e d3 b7    	jmp	0xd3b7
    baa2:	81 0e       	cmpa	#0xe
    baa4:	26 29       	bne	0x0xbacf
    baa6:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xbab0
    baaa:	7f 01 1a    	clr	0x11a
    baad:	7e d3 b7    	jmp	0xd3b7
    bab0:	ce 00 77    	ldx	#0x77
    bab3:	bd bb 1b    	jsr	0xbb1b
    bab6:	bd bb 4f    	jsr	0xbb4f
    bab9:	16          	tab
    baba:	ce de db    	ldx	#0xdedb
    babd:	18 ce 01 2b 	ldy	#0x12b
    bac1:	bd ea d3    	jsr	0xead3
    bac4:	86 04       	ldaa	#0x4
    bac6:	b7 01 1c    	staa	0x11c
    bac9:	7f 01 1a    	clr	0x11a
    bacc:	7e d3 b7    	jmp	0xd3b7
    bacf:	81 19       	cmpa	#0x19
    bad1:	26 24       	bne	0x0xbaf7
    bad3:	ce 00 78    	ldx	#0x78
    bad6:	bd bb 1b    	jsr	0xbb1b
    bad9:	bd bb 9b    	jsr	0xbb9b
    badc:	ce 01 37    	ldx	#0x137
    badf:	7d 00 f6    	tst	0xf6
    bae2:	2a 05       	bpl	0x0xbae9
    bae4:	bd ea 5c    	jsr	0xea5c
    bae7:	20 03       	bra	0x0xbaec
    bae9:	bd ea 20    	jsr	0xea20
    baec:	86 03       	ldaa	#0x3
    baee:	b7 01 1c    	staa	0x11c
    baf1:	7f 01 1a    	clr	0x11a
    baf4:	7e d3 b7    	jmp	0xd3b7
    baf7:	ce 00 79    	ldx	#0x79
    bafa:	bd bb 1b    	jsr	0xbb1b
    bafd:	bd bb 9b    	jsr	0xbb9b
    bb00:	ce 01 3c    	ldx	#0x13c
    bb03:	7d 00 f6    	tst	0xf6
    bb06:	2a 05       	bpl	0x0xbb0d
    bb08:	bd ea 5c    	jsr	0xea5c
    bb0b:	20 03       	bra	0x0xbb10
    bb0d:	bd ea 20    	jsr	0xea20
    bb10:	86 03       	ldaa	#0x3
    bb12:	b7 01 1c    	staa	0x11c
    bb15:	7f 01 1a    	clr	0x11a
    bb18:	7e d3 b7    	jmp	0xd3b7
    bb1b:	96 f6       	ldaa	*0xf6
    bb1d:	44          	lsra
    bb1e:	44          	lsra
    bb1f:	44          	lsra
    bb20:	24 03       	bcc	0x0xbb25
    bb22:	a6 00       	ldaa	0x0,x
    bb24:	39          	rts
    bb25:	44          	lsra
    bb26:	24 06       	bcc	0x0xbb2e
    bb28:	c6 04       	ldab	#0x4
    bb2a:	3a          	abx
    bb2b:	a6 00       	ldaa	0x0,x
    bb2d:	39          	rts
    bb2e:	44          	lsra
    bb2f:	24 06       	bcc	0x0xbb37
    bb31:	c6 08       	ldab	#0x8
    bb33:	3a          	abx
    bb34:	a6 00       	ldaa	0x0,x
    bb36:	39          	rts
    bb37:	44          	lsra
    bb38:	24 06       	bcc	0x0xbb40
    bb3a:	c6 0c       	ldab	#0xc
    bb3c:	3a          	abx
    bb3d:	a6 00       	ldaa	0x0,x
    bb3f:	39          	rts
    bb40:	44          	lsra
    bb41:	24 06       	bcc	0x0xbb49
    bb43:	c6 10       	ldab	#0x10
    bb45:	3a          	abx
    bb46:	a6 00       	ldaa	0x0,x
    bb48:	39          	rts
    bb49:	c6 14       	ldab	#0x14
    bb4b:	3a          	abx
    bb4c:	a6 00       	ldaa	0x0,x
    bb4e:	39          	rts
    bb4f:	7d 01 1a    	tst	0x11a
    bb52:	2b 29       	bmi	0x0xbb7d
    bb54:	4c          	inca
    bb55:	7d 00 f6    	tst	0xf6
    bb58:	2a 14       	bpl	0x0xbb6e
    bb5a:	7d 00 8f    	tst	0x8f
    bb5d:	27 0f       	beq	0x0xbb6e
    bb5f:	81 03       	cmpa	#0x3
    bb61:	22 04       	bhi	0x0xbb67
    bb63:	86 04       	ldaa	#0x4
    bb65:	20 0d       	bra	0x0xbb74
    bb67:	81 09       	cmpa	#0x9
    bb69:	26 03       	bne	0x0xbb6e
    bb6b:	4c          	inca
    bb6c:	20 06       	bra	0x0xbb74
    bb6e:	81 19       	cmpa	#0x19
    bb70:	25 02       	bcs	0x0xbb74
    bb72:	86 18       	ldaa	#0x18
    bb74:	a7 00       	staa	0x0,x
    bb76:	8f          	xgdx
    bb77:	37          	pshb
    bb78:	8f          	xgdx
    bb79:	33          	pulb
    bb7a:	7e af d2    	jmp	0xafd2
    bb7d:	4a          	deca
    bb7e:	2a 03       	bpl	0x0xbb83
    bb80:	4f          	clra
    bb81:	20 f1       	bra	0x0xbb74
    bb83:	7d 00 f6    	tst	0xf6
    bb86:	2a ec       	bpl	0x0xbb74
    bb88:	7d 00 8f    	tst	0x8f
    bb8b:	27 e7       	beq	0x0xbb74
    bb8d:	81 09       	cmpa	#0x9
    bb8f:	26 03       	bne	0x0xbb94
    bb91:	4a          	deca
    bb92:	20 e0       	bra	0x0xbb74
    bb94:	81 03       	cmpa	#0x3
    bb96:	22 dc       	bhi	0x0xbb74
    bb98:	4f          	clra
    bb99:	20 d9       	bra	0x0xbb74
    bb9b:	7d 01 1a    	tst	0x11a
    bb9e:	2b 12       	bmi	0x0xbbb2
    bba0:	4c          	inca
    bba1:	84 7f       	anda	#0x7f
    bba3:	13 f6 10 02 	brclr	*0xf6, #0x10, 0x0xbba9
    bba7:	84 1f       	anda	#0x1f
    bba9:	a7 00       	staa	0x0,x
    bbab:	8f          	xgdx
    bbac:	37          	pshb
    bbad:	8f          	xgdx
    bbae:	33          	pulb
    bbaf:	7e af d2    	jmp	0xafd2
    bbb2:	4a          	deca
    bbb3:	84 7f       	anda	#0x7f
    bbb5:	20 ec       	bra	0x0xbba3
    bbb7:	7d 01 1c    	tst	0x11c
    bbba:	2a 03       	bpl	0x0xbbbf
    bbbc:	7e dc 49    	jmp	0xdc49
    bbbf:	7d 01 1e    	tst	0x11e
    bbc2:	26 08       	bne	0x0xbbcc
    bbc4:	7d 01 1c    	tst	0x11c
    bbc7:	27 1f       	beq	0x0xbbe8
    bbc9:	7e b1 32    	jmp	0xb132
    bbcc:	7d 01 1c    	tst	0x11c
    bbcf:	27 24       	beq	0x0xbbf5
    bbd1:	cc 01 20    	ldd	#0x120
    bbd4:	fb 01 1e    	addb	0x11e
    bbd7:	8f          	xgdx
    bbd8:	bd e9 0c    	jsr	0xe90c
    bbdb:	7a 01 1e    	dec	0x11e
    bbde:	7a 01 1c    	dec	0x11c
    bbe1:	26 12       	bne	0x0xbbf5
    bbe3:	bd e9 8a    	jsr	0xe98a
    bbe6:	20 0d       	bra	0x0xbbf5
    bbe8:	7d 01 1e    	tst	0x11e
    bbeb:	26 08       	bne	0x0xbbf5
    bbed:	86 19       	ldaa	#0x19
    bbef:	b7 01 1e    	staa	0x11e
    bbf2:	bd e9 48    	jsr	0xe948
    bbf5:	7d 01 7f    	tst	0x17f
    bbf8:	27 10       	beq	0x0xbc0a
    bbfa:	bd eb 0b    	jsr	0xeb0b
    bbfd:	4f          	clra
    bbfe:	b7 01 7f    	staa	0x17f
    bc01:	b7 01 7e    	staa	0x17e
    bc04:	b7 01 1a    	staa	0x11a
    bc07:	7e d3 b7    	jmp	0xd3b7
    bc0a:	7d 01 1a    	tst	0x11a
    bc0d:	26 03       	bne	0x0xbc12
    bc0f:	7e d3 b7    	jmp	0xd3b7
    bc12:	b6 01 1e    	ldaa	0x11e
    bc15:	81 19       	cmpa	#0x19
    bc17:	26 34       	bne	0x0xbc4d
    bc19:	18 ce 01 36 	ldy	#0x136
    bc1d:	ce dc aa    	ldx	#0xdcaa
    bc20:	7d 01 1a    	tst	0x11a
    bc23:	2b 20       	bmi	0x0xbc45
    bc25:	d6 8e       	ldab	*0x8e
    bc27:	5c          	incb
    bc28:	c1 03       	cmpb	#0x3
    bc2a:	25 02       	bcs	0x0xbc2e
    bc2c:	c6 02       	ldab	#0x2
    bc2e:	d7 8e       	stab	*0x8e
    bc30:	bd ea d3    	jsr	0xead3
    bc33:	7f 01 1a    	clr	0x11a
    bc36:	86 04       	ldaa	#0x4
    bc38:	b7 01 1c    	staa	0x11c
    bc3b:	96 8e       	ldaa	*0x8e
    bc3d:	c6 8e       	ldab	#0x8e
    bc3f:	bd af d2    	jsr	0xafd2
    bc42:	7e d3 b7    	jmp	0xd3b7
    bc45:	d6 8e       	ldab	*0x8e
    bc47:	5a          	decb
    bc48:	2a e4       	bpl	0x0xbc2e
    bc4a:	5f          	clrb
    bc4b:	20 e1       	bra	0x0xbc2e
    bc4d:	18 ce 01 3b 	ldy	#0x13b
    bc51:	ce dc b6    	ldx	#0xdcb6
    bc54:	7d 01 1a    	tst	0x11a
    bc57:	2b 20       	bmi	0x0xbc79
    bc59:	d6 8f       	ldab	*0x8f
    bc5b:	5c          	incb
    bc5c:	c1 02       	cmpb	#0x2
    bc5e:	25 02       	bcs	0x0xbc62
    bc60:	c6 01       	ldab	#0x1
    bc62:	d7 8f       	stab	*0x8f
    bc64:	bd ea d3    	jsr	0xead3
    bc67:	7f 01 1a    	clr	0x11a
    bc6a:	86 04       	ldaa	#0x4
    bc6c:	b7 01 1c    	staa	0x11c
    bc6f:	96 8f       	ldaa	*0x8f
    bc71:	c6 8f       	ldab	#0x8f
    bc73:	bd af d2    	jsr	0xafd2
    bc76:	7e d3 b7    	jmp	0xd3b7
    bc79:	d6 8f       	ldab	*0x8f
    bc7b:	5a          	decb
    bc7c:	2a e4       	bpl	0x0xbc62
    bc7e:	5f          	clrb
    bc7f:	20 e1       	bra	0x0xbc62
    bc81:	7d 01 1c    	tst	0x11c
    bc84:	2a 03       	bpl	0x0xbc89
    bc86:	7e dc be    	jmp	0xdcbe
    bc89:	7d 01 1e    	tst	0x11e
    bc8c:	26 08       	bne	0x0xbc96
    bc8e:	7d 01 1c    	tst	0x11c
    bc91:	27 1a       	beq	0x0xbcad
    bc93:	7e b1 32    	jmp	0xb132
    bc96:	7d 01 1c    	tst	0x11c
    bc99:	27 1a       	beq	0x0xbcb5
    bc9b:	cc 01 20    	ldd	#0x120
    bc9e:	fb 01 1e    	addb	0x11e
    bca1:	8f          	xgdx
    bca2:	bd e9 0c    	jsr	0xe90c
    bca5:	7a 01 1e    	dec	0x11e
    bca8:	7a 01 1c    	dec	0x11c
    bcab:	26 08       	bne	0x0xbcb5
    bcad:	86 11       	ldaa	#0x11
    bcaf:	b7 01 1e    	staa	0x11e
    bcb2:	bd e9 48    	jsr	0xe948
    bcb5:	4f          	clra
    bcb6:	b7 01 7f    	staa	0x17f
    bcb9:	b7 01 7e    	staa	0x17e
    bcbc:	7d 01 1a    	tst	0x11a
    bcbf:	26 03       	bne	0x0xbcc4
    bcc1:	7e d3 b7    	jmp	0xd3b7
    bcc4:	2b 2b       	bmi	0x0xbcf1
    bcc6:	b6 50 00    	ldaa	0x5000
    bcc9:	4c          	inca
    bcca:	81 02       	cmpa	#0x2
    bccc:	25 0a       	bcs	0x0xbcd8
    bcce:	81 05       	cmpa	#0x5
    bcd0:	22 04       	bhi	0x0xbcd6
    bcd2:	86 05       	ldaa	#0x5
    bcd4:	20 02       	bra	0x0xbcd8
    bcd6:	86 06       	ldaa	#0x6
    bcd8:	b7 50 00    	staa	0x5000
    bcdb:	97 d1       	staa	*0xd1
    bcdd:	bd 9c b5    	jsr	0x9cb5
    bce0:	bd a3 32    	jsr	0xa332
    bce3:	86 80       	ldaa	#0x80
    bce5:	b7 01 1c    	staa	0x11c
    bce8:	7f 01 1a    	clr	0x11a
    bceb:	14 fb 80    	bset	*0xfb, #0x80
    bcee:	7e d3 b7    	jmp	0xd3b7
    bcf1:	b6 50 00    	ldaa	0x5000
    bcf4:	81 06       	cmpa	#0x6
    bcf6:	26 04       	bne	0x0xbcfc
    bcf8:	86 05       	ldaa	#0x5
    bcfa:	20 dc       	bra	0x0xbcd8
    bcfc:	81 05       	cmpa	#0x5
    bcfe:	26 04       	bne	0x0xbd04
    bd00:	86 01       	ldaa	#0x1
    bd02:	20 d4       	bra	0x0xbcd8
    bd04:	4f          	clra
    bd05:	20 d1       	bra	0x0xbcd8
    bd07:	7d 01 1c    	tst	0x11c
    bd0a:	2a 03       	bpl	0x0xbd0f
    bd0c:	7e dd dc    	jmp	0xdddc
    bd0f:	7d 01 1e    	tst	0x11e
    bd12:	26 08       	bne	0x0xbd1c
    bd14:	7d 01 1c    	tst	0x11c
    bd17:	27 1a       	beq	0x0xbd33
    bd19:	7e b1 32    	jmp	0xb132
    bd1c:	7d 01 1c    	tst	0x11c
    bd1f:	27 1a       	beq	0x0xbd3b
    bd21:	cc 01 20    	ldd	#0x120
    bd24:	fb 01 1e    	addb	0x11e
    bd27:	8f          	xgdx
    bd28:	bd e9 0c    	jsr	0xe90c
    bd2b:	7a 01 1e    	dec	0x11e
    bd2e:	7a 01 1c    	dec	0x11c
    bd31:	26 08       	bne	0x0xbd3b
    bd33:	86 1e       	ldaa	#0x1e
    bd35:	b7 01 1e    	staa	0x11e
    bd38:	bd e9 48    	jsr	0xe948
    bd3b:	7f 01 7e    	clr	0x17e
    bd3e:	7f 01 7f    	clr	0x17f
    bd41:	7d 01 1a    	tst	0x11a
    bd44:	26 03       	bne	0x0xbd49
    bd46:	7e d3 b7    	jmp	0xd3b7
    bd49:	b6 50 21    	ldaa	0x5021
    bd4c:	7d 01 1a    	tst	0x11a
    bd4f:	2b 0e       	bmi	0x0xbd5f
    bd51:	4c          	inca
    bd52:	84 7f       	anda	#0x7f
    bd54:	b7 50 21    	staa	0x5021
    bd57:	ce 01 3c    	ldx	#0x13c
    bd5a:	bd ea 20    	jsr	0xea20
    bd5d:	20 03       	bra	0x0xbd62
    bd5f:	4a          	deca
    bd60:	20 f0       	bra	0x0xbd52
    bd62:	7f 01 1a    	clr	0x11a
    bd65:	86 03       	ldaa	#0x3
    bd67:	b7 01 1c    	staa	0x11c
    bd6a:	14 fb 80    	bset	*0xfb, #0x80
    bd6d:	7e d3 b7    	jmp	0xd3b7
    bd70:	7d 01 1c    	tst	0x11c
    bd73:	2a 03       	bpl	0x0xbd78
    bd75:	7e de 1f    	jmp	0xde1f
    bd78:	7d 01 1c    	tst	0x11c
    bd7b:	27 03       	beq	0x0xbd80
    bd7d:	7e b1 32    	jmp	0xb132
    bd80:	7d 01 1e    	tst	0x11e
    bd83:	26 08       	bne	0x0xbd8d
    bd85:	86 14       	ldaa	#0x14
    bd87:	b7 01 1e    	staa	0x11e
    bd8a:	bd e9 48    	jsr	0xe948
    bd8d:	7e d3 b7    	jmp	0xd3b7
    bd90:	7d 01 1c    	tst	0x11c
    bd93:	2a 03       	bpl	0x0xbd98
    bd95:	7e de 7a    	jmp	0xde7a
    bd98:	7d 01 1e    	tst	0x11e
    bd9b:	26 08       	bne	0x0xbda5
    bd9d:	7d 01 1c    	tst	0x11c
    bda0:	27 1f       	beq	0x0xbdc1
    bda2:	7e b1 32    	jmp	0xb132
    bda5:	7d 01 1c    	tst	0x11c
    bda8:	27 24       	beq	0x0xbdce
    bdaa:	cc 01 20    	ldd	#0x120
    bdad:	fb 01 1e    	addb	0x11e
    bdb0:	8f          	xgdx
    bdb1:	bd e9 0c    	jsr	0xe90c
    bdb4:	7a 01 1e    	dec	0x11e
    bdb7:	7a 01 1c    	dec	0x11c
    bdba:	26 12       	bne	0x0xbdce
    bdbc:	bd e9 8a    	jsr	0xe98a
    bdbf:	20 0d       	bra	0x0xbdce
    bdc1:	7d 01 1e    	tst	0x11e
    bdc4:	26 08       	bne	0x0xbdce
    bdc6:	86 03       	ldaa	#0x3
    bdc8:	b7 01 1e    	staa	0x11e
    bdcb:	bd e9 48    	jsr	0xe948
    bdce:	7d 01 7f    	tst	0x17f
    bdd1:	27 10       	beq	0x0xbde3
    bdd3:	bd eb 4c    	jsr	0xeb4c
    bdd6:	4f          	clra
    bdd7:	b7 01 7f    	staa	0x17f
    bdda:	b7 01 7e    	staa	0x17e
    bddd:	b7 01 1a    	staa	0x11a
    bde0:	7e d3 b7    	jmp	0xd3b7
    bde3:	7d 01 7e    	tst	0x17e
    bde6:	27 0c       	beq	0x0xbdf4
    bde8:	bd ea f4    	jsr	0xeaf4
    bdeb:	7f 01 7e    	clr	0x17e
    bdee:	7f 01 1a    	clr	0x11a
    bdf1:	7e d3 b7    	jmp	0xd3b7
    bdf4:	7d 01 1a    	tst	0x11a
    bdf7:	26 03       	bne	0x0xbdfc
    bdf9:	7e d3 b7    	jmp	0xd3b7
    bdfc:	ce de db    	ldx	#0xdedb
    bdff:	b6 01 1e    	ldaa	0x11e
    be02:	81 03       	cmpa	#0x3
    be04:	26 1a       	bne	0x0xbe20
    be06:	18 ce 01 20 	ldy	#0x120
    be0a:	d6 60       	ldab	*0x60
    be0c:	bd be ee    	jsr	0xbeee
    be0f:	d7 60       	stab	*0x60
    be11:	bd ea d3    	jsr	0xead3
    be14:	96 60       	ldaa	*0x60
    be16:	c6 60       	ldab	#0x60
    be18:	bd af d2    	jsr	0xafd2
    be1b:	86 04       	ldaa	#0x4
    be1d:	7e be e5    	jmp	0xbee5
    be20:	81 09       	cmpa	#0x9
    be22:	26 1a       	bne	0x0xbe3e
    be24:	18 ce 01 26 	ldy	#0x126
    be28:	d6 61       	ldab	*0x61
    be2a:	bd be ee    	jsr	0xbeee
    be2d:	d7 61       	stab	*0x61
    be2f:	bd ea d3    	jsr	0xead3
    be32:	96 61       	ldaa	*0x61
    be34:	c6 61       	ldab	#0x61
    be36:	bd af d2    	jsr	0xafd2
    be39:	86 04       	ldaa	#0x4
    be3b:	7e be e5    	jmp	0xbee5
    be3e:	81 0e       	cmpa	#0xe
    be40:	26 1a       	bne	0x0xbe5c
    be42:	18 ce 01 2b 	ldy	#0x12b
    be46:	d6 62       	ldab	*0x62
    be48:	bd be ee    	jsr	0xbeee
    be4b:	d7 62       	stab	*0x62
    be4d:	bd ea d3    	jsr	0xead3
    be50:	96 62       	ldaa	*0x62
    be52:	c6 62       	ldab	#0x62
    be54:	bd af d2    	jsr	0xafd2
    be57:	86 04       	ldaa	#0x4
    be59:	7e be e5    	jmp	0xbee5
    be5c:	81 13       	cmpa	#0x13
    be5e:	26 2b       	bne	0x0xbe8b
    be60:	b6 01 1a    	ldaa	0x11a
    be63:	2b 1f       	bmi	0x0xbe84
    be65:	96 63       	ldaa	*0x63
    be67:	4c          	inca
    be68:	84 7f       	anda	#0x7f
    be6a:	97 63       	staa	*0x63
    be6c:	ce 01 31    	ldx	#0x131
    be6f:	bd ea 20    	jsr	0xea20
    be72:	7f 01 1a    	clr	0x11a
    be75:	86 03       	ldaa	#0x3
    be77:	b7 01 1c    	staa	0x11c
    be7a:	96 63       	ldaa	*0x63
    be7c:	c6 63       	ldab	#0x63
    be7e:	bd af d2    	jsr	0xafd2
    be81:	7e d3 b7    	jmp	0xd3b7
    be84:	96 63       	ldaa	*0x63
    be86:	4a          	deca
    be87:	84 7f       	anda	#0x7f
    be89:	20 df       	bra	0x0xbe6a
    be8b:	81 19       	cmpa	#0x19
    be8d:	26 2b       	bne	0x0xbeba
    be8f:	b6 01 1a    	ldaa	0x11a
    be92:	2b 1f       	bmi	0x0xbeb3
    be94:	96 64       	ldaa	*0x64
    be96:	4c          	inca
    be97:	84 7f       	anda	#0x7f
    be99:	97 64       	staa	*0x64
    be9b:	ce 01 37    	ldx	#0x137
    be9e:	bd ea 20    	jsr	0xea20
    bea1:	7f 01 1a    	clr	0x11a
    bea4:	86 03       	ldaa	#0x3
    bea6:	b7 01 1c    	staa	0x11c
    bea9:	96 64       	ldaa	*0x64
    beab:	c6 64       	ldab	#0x64
    bead:	bd af d2    	jsr	0xafd2
    beb0:	7e d3 b7    	jmp	0xd3b7
    beb3:	96 64       	ldaa	*0x64
    beb5:	4a          	deca
    beb6:	84 7f       	anda	#0x7f
    beb8:	20 df       	bra	0x0xbe99
    beba:	7d 01 1a    	tst	0x11a
    bebd:	2b 1f       	bmi	0x0xbede
    bebf:	96 65       	ldaa	*0x65
    bec1:	4c          	inca
    bec2:	84 7f       	anda	#0x7f
    bec4:	97 65       	staa	*0x65
    bec6:	ce 01 3c    	ldx	#0x13c
    bec9:	bd ea 20    	jsr	0xea20
    becc:	7f 01 1a    	clr	0x11a
    becf:	86 03       	ldaa	#0x3
    bed1:	b7 01 1c    	staa	0x11c
    bed4:	96 65       	ldaa	*0x65
    bed6:	c6 65       	ldab	#0x65
    bed8:	bd af d2    	jsr	0xafd2
    bedb:	7e d3 b7    	jmp	0xd3b7
    bede:	96 65       	ldaa	*0x65
    bee0:	4a          	deca
    bee1:	84 7f       	anda	#0x7f
    bee3:	20 df       	bra	0x0xbec4
    bee5:	b7 01 1c    	staa	0x11c
    bee8:	7f 01 1a    	clr	0x11a
    beeb:	7e d3 b7    	jmp	0xd3b7
    beee:	7d 01 1a    	tst	0x11a
    bef1:	2b 15       	bmi	0x0xbf08
    bef3:	5c          	incb
    bef4:	c1 09       	cmpb	#0x9
    bef6:	26 02       	bne	0x0xbefa
    bef8:	5c          	incb
    bef9:	39          	rts
    befa:	c1 0d       	cmpb	#0xd
    befc:	26 03       	bne	0x0xbf01
    befe:	5c          	incb
    beff:	5c          	incb
    bf00:	39          	rts
    bf01:	c1 16       	cmpb	#0x16
    bf03:	25 02       	bcs	0x0xbf07
    bf05:	c6 15       	ldab	#0x15
    bf07:	39          	rts
    bf08:	5a          	decb
    bf09:	2a 02       	bpl	0x0xbf0d
    bf0b:	5f          	clrb
    bf0c:	39          	rts
    bf0d:	c1 0e       	cmpb	#0xe
    bf0f:	26 03       	bne	0x0xbf14
    bf11:	5a          	decb
    bf12:	5a          	decb
    bf13:	39          	rts
    bf14:	c1 09       	cmpb	#0x9
    bf16:	26 ef       	bne	0x0xbf07
    bf18:	5a          	decb
    bf19:	39          	rts
    bf1a:	7d 01 1c    	tst	0x11c
    bf1d:	2a 03       	bpl	0x0xbf22
    bf1f:	7e df 3f    	jmp	0xdf3f
    bf22:	7d 01 1e    	tst	0x11e
    bf25:	26 08       	bne	0x0xbf2f
    bf27:	7d 01 1c    	tst	0x11c
    bf2a:	27 1f       	beq	0x0xbf4b
    bf2c:	7e b1 32    	jmp	0xb132
    bf2f:	7d 01 1c    	tst	0x11c
    bf32:	27 24       	beq	0x0xbf58
    bf34:	cc 01 20    	ldd	#0x120
    bf37:	fb 01 1e    	addb	0x11e
    bf3a:	8f          	xgdx
    bf3b:	bd e9 0c    	jsr	0xe90c
    bf3e:	7a 01 1e    	dec	0x11e
    bf41:	7a 01 1c    	dec	0x11c
    bf44:	26 12       	bne	0x0xbf58
    bf46:	bd e9 8a    	jsr	0xe98a
    bf49:	20 0d       	bra	0x0xbf58
    bf4b:	7d 01 1e    	tst	0x11e
    bf4e:	26 08       	bne	0x0xbf58
    bf50:	86 13       	ldaa	#0x13
    bf52:	b7 01 1e    	staa	0x11e
    bf55:	bd e9 48    	jsr	0xe948
    bf58:	7d 01 7f    	tst	0x17f
    bf5b:	27 10       	beq	0x0xbf6d
    bf5d:	bd eb 4c    	jsr	0xeb4c
    bf60:	4f          	clra
    bf61:	b7 01 7f    	staa	0x17f
    bf64:	b7 01 7e    	staa	0x17e
    bf67:	b7 01 1a    	staa	0x11a
    bf6a:	7e d3 b7    	jmp	0xd3b7
    bf6d:	7d 01 1a    	tst	0x11a
    bf70:	26 03       	bne	0x0xbf75
    bf72:	7e d3 b7    	jmp	0xd3b7
    bf75:	b6 01 1e    	ldaa	0x11e
    bf78:	81 13       	cmpa	#0x13
    bf7a:	26 2b       	bne	0x0xbfa7
    bf7c:	b6 01 1a    	ldaa	0x11a
    bf7f:	2b 1f       	bmi	0x0xbfa0
    bf81:	96 54       	ldaa	*0x54
    bf83:	4c          	inca
    bf84:	84 7f       	anda	#0x7f
    bf86:	97 54       	staa	*0x54
    bf88:	ce 01 31    	ldx	#0x131
    bf8b:	bd ea 20    	jsr	0xea20
    bf8e:	7f 01 1a    	clr	0x11a
    bf91:	86 03       	ldaa	#0x3
    bf93:	b7 01 1c    	staa	0x11c
    bf96:	96 54       	ldaa	*0x54
    bf98:	c6 54       	ldab	#0x54
    bf9a:	bd af d2    	jsr	0xafd2
    bf9d:	7e d3 b7    	jmp	0xd3b7
    bfa0:	96 54       	ldaa	*0x54
    bfa2:	4a          	deca
    bfa3:	84 7f       	anda	#0x7f
    bfa5:	20 df       	bra	0x0xbf86
    bfa7:	81 19       	cmpa	#0x19
    bfa9:	26 2b       	bne	0x0xbfd6
    bfab:	b6 01 1a    	ldaa	0x11a
    bfae:	2b 1f       	bmi	0x0xbfcf
    bfb0:	96 59       	ldaa	*0x59
    bfb2:	4c          	inca
    bfb3:	84 7f       	anda	#0x7f
    bfb5:	97 59       	staa	*0x59
    bfb7:	ce 01 37    	ldx	#0x137
    bfba:	bd ea 20    	jsr	0xea20
    bfbd:	7f 01 1a    	clr	0x11a
    bfc0:	86 03       	ldaa	#0x3
    bfc2:	b7 01 1c    	staa	0x11c
    bfc5:	96 59       	ldaa	*0x59
    bfc7:	c6 59       	ldab	#0x59
    bfc9:	bd af d2    	jsr	0xafd2
    bfcc:	7e d3 b7    	jmp	0xd3b7
    bfcf:	96 59       	ldaa	*0x59
    bfd1:	4a          	deca
    bfd2:	84 7f       	anda	#0x7f
    bfd4:	20 df       	bra	0x0xbfb5
    bfd6:	b6 01 1a    	ldaa	0x11a
    bfd9:	2b 1f       	bmi	0x0xbffa
    bfdb:	96 5e       	ldaa	*0x5e
    bfdd:	4c          	inca
    bfde:	84 7f       	anda	#0x7f
    bfe0:	97 5e       	staa	*0x5e
    bfe2:	ce 01 3c    	ldx	#0x13c
    bfe5:	bd ea 20    	jsr	0xea20
    bfe8:	7f 01 1a    	clr	0x11a
    bfeb:	86 03       	ldaa	#0x3
    bfed:	b7 01 1c    	staa	0x11c
    bff0:	96 5e       	ldaa	*0x5e
    bff2:	c6 5e       	ldab	#0x5e
    bff4:	bd af d2    	jsr	0xafd2
    bff7:	7e d3 b7    	jmp	0xd3b7
    bffa:	96 5e       	ldaa	*0x5e
    bffc:	4a          	deca
    bffd:	84 7f       	anda	#0x7f
    bfff:	20 df       	bra	0x0xbfe0
    c001:	7d 01 1c    	tst	0x11c
    c004:	2a 03       	bpl	0x0xc009
    c006:	7e df 92    	jmp	0xdf92
    c009:	7d 01 1e    	tst	0x11e
    c00c:	26 08       	bne	0x0xc016
    c00e:	7d 01 1c    	tst	0x11c
    c011:	27 1f       	beq	0x0xc032
    c013:	7e b1 32    	jmp	0xb132
    c016:	7d 01 1c    	tst	0x11c
    c019:	27 24       	beq	0x0xc03f
    c01b:	cc 01 20    	ldd	#0x120
    c01e:	fb 01 1e    	addb	0x11e
    c021:	8f          	xgdx
    c022:	bd e9 0c    	jsr	0xe90c
    c025:	7a 01 1e    	dec	0x11e
    c028:	7a 01 1c    	dec	0x11c
    c02b:	26 12       	bne	0x0xc03f
    c02d:	bd e9 8a    	jsr	0xe98a
    c030:	20 0d       	bra	0x0xc03f
    c032:	7d 01 1e    	tst	0x11e
    c035:	26 08       	bne	0x0xc03f
    c037:	86 13       	ldaa	#0x13
    c039:	b7 01 1e    	staa	0x11e
    c03c:	bd e9 48    	jsr	0xe948
    c03f:	7d 01 7f    	tst	0x17f
    c042:	27 10       	beq	0x0xc054
    c044:	bd eb 4c    	jsr	0xeb4c
    c047:	4f          	clra
    c048:	b7 01 7f    	staa	0x17f
    c04b:	b7 01 7e    	staa	0x17e
    c04e:	b7 01 1a    	staa	0x11a
    c051:	7e d3 b7    	jmp	0xd3b7
    c054:	7d 01 1a    	tst	0x11a
    c057:	26 03       	bne	0x0xc05c
    c059:	7e d3 b7    	jmp	0xd3b7
    c05c:	b6 01 1e    	ldaa	0x11e
    c05f:	81 13       	cmpa	#0x13
    c061:	26 2b       	bne	0x0xc08e
    c063:	b6 01 1a    	ldaa	0x11a
    c066:	2b 1f       	bmi	0x0xc087
    c068:	96 90       	ldaa	*0x90
    c06a:	4c          	inca
    c06b:	84 7f       	anda	#0x7f
    c06d:	97 90       	staa	*0x90
    c06f:	ce 01 31    	ldx	#0x131
    c072:	bd ea 20    	jsr	0xea20
    c075:	7f 01 1a    	clr	0x11a
    c078:	86 03       	ldaa	#0x3
    c07a:	b7 01 1c    	staa	0x11c
    c07d:	96 90       	ldaa	*0x90
    c07f:	c6 90       	ldab	#0x90
    c081:	bd af d2    	jsr	0xafd2
    c084:	7e d3 b7    	jmp	0xd3b7
    c087:	96 90       	ldaa	*0x90
    c089:	4a          	deca
    c08a:	84 7f       	anda	#0x7f
    c08c:	20 df       	bra	0x0xc06d
    c08e:	81 19       	cmpa	#0x19
    c090:	26 2b       	bne	0x0xc0bd
    c092:	b6 01 1a    	ldaa	0x11a
    c095:	2b 1f       	bmi	0x0xc0b6
    c097:	96 91       	ldaa	*0x91
    c099:	4c          	inca
    c09a:	84 7f       	anda	#0x7f
    c09c:	97 91       	staa	*0x91
    c09e:	ce 01 37    	ldx	#0x137
    c0a1:	bd ea 20    	jsr	0xea20
    c0a4:	7f 01 1a    	clr	0x11a
    c0a7:	86 03       	ldaa	#0x3
    c0a9:	b7 01 1c    	staa	0x11c
    c0ac:	96 91       	ldaa	*0x91
    c0ae:	c6 91       	ldab	#0x91
    c0b0:	bd af d2    	jsr	0xafd2
    c0b3:	7e d3 b7    	jmp	0xd3b7
    c0b6:	96 91       	ldaa	*0x91
    c0b8:	4a          	deca
    c0b9:	84 7f       	anda	#0x7f
    c0bb:	20 df       	bra	0x0xc09c
    c0bd:	b6 01 1a    	ldaa	0x11a
    c0c0:	2b 1f       	bmi	0x0xc0e1
    c0c2:	96 92       	ldaa	*0x92
    c0c4:	4c          	inca
    c0c5:	84 7f       	anda	#0x7f
    c0c7:	97 92       	staa	*0x92
    c0c9:	ce 01 3c    	ldx	#0x13c
    c0cc:	bd ea 20    	jsr	0xea20
    c0cf:	7f 01 1a    	clr	0x11a
    c0d2:	86 03       	ldaa	#0x3
    c0d4:	b7 01 1c    	staa	0x11c
    c0d7:	96 92       	ldaa	*0x92
    c0d9:	c6 92       	ldab	#0x92
    c0db:	bd af d2    	jsr	0xafd2
    c0de:	7e d3 b7    	jmp	0xd3b7
    c0e1:	96 92       	ldaa	*0x92
    c0e3:	4a          	deca
    c0e4:	84 7f       	anda	#0x7f
    c0e6:	20 df       	bra	0x0xc0c7
    c0e8:	7d 01 1c    	tst	0x11c
    c0eb:	2a 03       	bpl	0x0xc0f0
    c0ed:	7e e0 2e    	jmp	0xe02e
    c0f0:	7d 01 1e    	tst	0x11e
    c0f3:	26 08       	bne	0x0xc0fd
    c0f5:	7d 01 1c    	tst	0x11c
    c0f8:	27 1f       	beq	0x0xc119
    c0fa:	7e b1 32    	jmp	0xb132
    c0fd:	7d 01 1c    	tst	0x11c
    c100:	27 24       	beq	0x0xc126
    c102:	cc 01 20    	ldd	#0x120
    c105:	fb 01 1e    	addb	0x11e
    c108:	8f          	xgdx
    c109:	bd e9 0c    	jsr	0xe90c
    c10c:	7a 01 1e    	dec	0x11e
    c10f:	7a 01 1c    	dec	0x11c
    c112:	26 12       	bne	0x0xc126
    c114:	bd e9 8a    	jsr	0xe98a
    c117:	20 0d       	bra	0x0xc126
    c119:	7d 01 1e    	tst	0x11e
    c11c:	26 08       	bne	0x0xc126
    c11e:	86 13       	ldaa	#0x13
    c120:	b7 01 1e    	staa	0x11e
    c123:	bd e9 48    	jsr	0xe948
    c126:	7d 01 7f    	tst	0x17f
    c129:	27 1e       	beq	0x0xc149
    c12b:	2a 10       	bpl	0x0xc13d
    c12d:	bd eb 4c    	jsr	0xeb4c
    c130:	4f          	clra
    c131:	b7 01 7f    	staa	0x17f
    c134:	b7 01 7e    	staa	0x17e
    c137:	b7 01 1a    	staa	0x11a
    c13a:	7e d3 b7    	jmp	0xd3b7
    c13d:	b6 01 1e    	ldaa	0x11e
    c140:	81 19       	cmpa	#0x19
    c142:	27 ec       	beq	0x0xc130
    c144:	bd eb 4c    	jsr	0xeb4c
    c147:	20 e7       	bra	0x0xc130
    c149:	7d 01 1a    	tst	0x11a
    c14c:	26 03       	bne	0x0xc151
    c14e:	7e d3 b7    	jmp	0xd3b7
    c151:	b6 01 1e    	ldaa	0x11e
    c154:	81 13       	cmpa	#0x13
    c156:	26 34       	bne	0x0xc18c
    c158:	b6 01 1a    	ldaa	0x11a
    c15b:	2b 27       	bmi	0x0xc184
    c15d:	d6 4b       	ldab	*0x4b
    c15f:	5c          	incb
    c160:	c1 06       	cmpb	#0x6
    c162:	23 02       	bls	0x0xc166
    c164:	c6 06       	ldab	#0x6
    c166:	d7 4b       	stab	*0x4b
    c168:	ce e0 91    	ldx	#0xe091
    c16b:	18 ce 01 30 	ldy	#0x130
    c16f:	bd ea d3    	jsr	0xead3
    c172:	7f 01 1a    	clr	0x11a
    c175:	86 04       	ldaa	#0x4
    c177:	b7 01 1c    	staa	0x11c
    c17a:	96 4b       	ldaa	*0x4b
    c17c:	c6 4b       	ldab	#0x4b
    c17e:	bd af d2    	jsr	0xafd2
    c181:	7e d3 b7    	jmp	0xd3b7
    c184:	d6 4b       	ldab	*0x4b
    c186:	5a          	decb
    c187:	2a dd       	bpl	0x0xc166
    c189:	5f          	clrb
    c18a:	20 da       	bra	0x0xc166
    c18c:	81 19       	cmpa	#0x19
    c18e:	26 34       	bne	0x0xc1c4
    c190:	b6 01 1a    	ldaa	0x11a
    c193:	2b 27       	bmi	0x0xc1bc
    c195:	d6 66       	ldab	*0x66
    c197:	5c          	incb
    c198:	c1 03       	cmpb	#0x3
    c19a:	23 02       	bls	0x0xc19e
    c19c:	c6 03       	ldab	#0x3
    c19e:	d7 66       	stab	*0x66
    c1a0:	ce e0 ad    	ldx	#0xe0ad
    c1a3:	18 ce 01 36 	ldy	#0x136
    c1a7:	bd ea d3    	jsr	0xead3
    c1aa:	7f 01 1a    	clr	0x11a
    c1ad:	86 04       	ldaa	#0x4
    c1af:	b7 01 1c    	staa	0x11c
    c1b2:	96 66       	ldaa	*0x66
    c1b4:	c6 66       	ldab	#0x66
    c1b6:	bd af d2    	jsr	0xafd2
    c1b9:	7e d3 b7    	jmp	0xd3b7
    c1bc:	d6 66       	ldab	*0x66
    c1be:	5a          	decb
    c1bf:	2a dd       	bpl	0x0xc19e
    c1c1:	5f          	clrb
    c1c2:	20 da       	bra	0x0xc19e
    c1c4:	4f          	clra
    c1c5:	b7 01 7e    	staa	0x17e
    c1c8:	b7 01 7f    	staa	0x17f
    c1cb:	b7 01 1a    	staa	0x11a
    c1ce:	7e d3 b7    	jmp	0xd3b7
    c1d1:	7d 01 1c    	tst	0x11c
    c1d4:	2a 03       	bpl	0x0xc1d9
    c1d6:	7e e0 bd    	jmp	0xe0bd
    c1d9:	7d 01 1e    	tst	0x11e
    c1dc:	26 08       	bne	0x0xc1e6
    c1de:	7d 01 1c    	tst	0x11c
    c1e1:	27 1f       	beq	0x0xc202
    c1e3:	7e b1 32    	jmp	0xb132
    c1e6:	7d 01 1c    	tst	0x11c
    c1e9:	27 2c       	beq	0x0xc217
    c1eb:	cc 01 20    	ldd	#0x120
    c1ee:	fb 01 1e    	addb	0x11e
    c1f1:	8f          	xgdx
    c1f2:	bd e9 0c    	jsr	0xe90c
    c1f5:	7a 01 1e    	dec	0x11e
    c1f8:	7a 01 1c    	dec	0x11c
    c1fb:	26 1a       	bne	0x0xc217
    c1fd:	bd e9 bd    	jsr	0xe9bd
    c200:	20 15       	bra	0x0xc217
    c202:	7d 01 1e    	tst	0x11e
    c205:	26 10       	bne	0x0xc217
    c207:	86 12       	ldaa	#0x12
    c209:	d6 4b       	ldab	*0x4b
    c20b:	c1 06       	cmpb	#0x6
    c20d:	26 02       	bne	0x0xc211
    c20f:	86 16       	ldaa	#0x16
    c211:	b7 01 1e    	staa	0x11e
    c214:	bd e9 48    	jsr	0xe948
    c217:	7d 01 7f    	tst	0x17f
    c21a:	27 10       	beq	0x0xc22c
    c21c:	bd eb be    	jsr	0xebbe
    c21f:	4f          	clra
    c220:	b7 01 7f    	staa	0x17f
    c223:	b7 01 7e    	staa	0x17e
    c226:	b7 01 1a    	staa	0x11a
    c229:	7e d3 b7    	jmp	0xd3b7
    c22c:	7d 01 1a    	tst	0x11a
    c22f:	26 03       	bne	0x0xc234
    c231:	7e d3 b7    	jmp	0xd3b7
    c234:	b6 01 1e    	ldaa	0x11e
    c237:	81 12       	cmpa	#0x12
    c239:	26 2b       	bne	0x0xc266
    c23b:	b6 01 1a    	ldaa	0x11a
    c23e:	2b 1f       	bmi	0x0xc25f
    c240:	96 6e       	ldaa	*0x6e
    c242:	4c          	inca
    c243:	84 7f       	anda	#0x7f
    c245:	97 6e       	staa	*0x6e
    c247:	ce 01 30    	ldx	#0x130
    c24a:	bd ea 20    	jsr	0xea20
    c24d:	7f 01 1a    	clr	0x11a
    c250:	86 03       	ldaa	#0x3
    c252:	b7 01 1c    	staa	0x11c
    c255:	96 6e       	ldaa	*0x6e
    c257:	c6 6e       	ldab	#0x6e
    c259:	bd af d2    	jsr	0xafd2
    c25c:	7e d3 b7    	jmp	0xd3b7
    c25f:	96 6e       	ldaa	*0x6e
    c261:	4a          	deca
    c262:	84 7f       	anda	#0x7f
    c264:	20 df       	bra	0x0xc245
    c266:	81 16       	cmpa	#0x16
    c268:	26 2b       	bne	0x0xc295
    c26a:	b6 01 1a    	ldaa	0x11a
    c26d:	2b 1f       	bmi	0x0xc28e
    c26f:	96 48       	ldaa	*0x48
    c271:	4c          	inca
    c272:	84 7f       	anda	#0x7f
    c274:	97 48       	staa	*0x48
    c276:	ce 01 34    	ldx	#0x134
    c279:	bd ea 20    	jsr	0xea20
    c27c:	7f 01 1a    	clr	0x11a
    c27f:	86 03       	ldaa	#0x3
    c281:	b7 01 1c    	staa	0x11c
    c284:	96 48       	ldaa	*0x48
    c286:	c6 48       	ldab	#0x48
    c288:	bd af d2    	jsr	0xafd2
    c28b:	7e d3 b7    	jmp	0xd3b7
    c28e:	96 48       	ldaa	*0x48
    c290:	4a          	deca
    c291:	84 7f       	anda	#0x7f
    c293:	20 df       	bra	0x0xc274
    c295:	81 1a       	cmpa	#0x1a
    c297:	26 2b       	bne	0x0xc2c4
    c299:	b6 01 1a    	ldaa	0x11a
    c29c:	2b 1f       	bmi	0x0xc2bd
    c29e:	96 6f       	ldaa	*0x6f
    c2a0:	4c          	inca
    c2a1:	84 7f       	anda	#0x7f
    c2a3:	97 6f       	staa	*0x6f
    c2a5:	ce 01 38    	ldx	#0x138
    c2a8:	bd ea 20    	jsr	0xea20
    c2ab:	7f 01 1a    	clr	0x11a
    c2ae:	86 03       	ldaa	#0x3
    c2b0:	b7 01 1c    	staa	0x11c
    c2b3:	96 6f       	ldaa	*0x6f
    c2b5:	c6 6f       	ldab	#0x6f
    c2b7:	bd af d2    	jsr	0xafd2
    c2ba:	7e d3 b7    	jmp	0xd3b7
    c2bd:	96 6f       	ldaa	*0x6f
    c2bf:	4a          	deca
    c2c0:	84 7f       	anda	#0x7f
    c2c2:	20 df       	bra	0x0xc2a3
    c2c4:	b6 01 1a    	ldaa	0x11a
    c2c7:	2b 21       	bmi	0x0xc2ea
    c2c9:	96 70       	ldaa	*0x70
    c2cb:	4c          	inca
    c2cc:	2a 02       	bpl	0x0xc2d0
    c2ce:	86 7f       	ldaa	#0x7f
    c2d0:	97 70       	staa	*0x70
    c2d2:	ce 01 3c    	ldx	#0x13c
    c2d5:	bd ea e7    	jsr	0xeae7
    c2d8:	7f 01 1a    	clr	0x11a
    c2db:	86 03       	ldaa	#0x3
    c2dd:	b7 01 1c    	staa	0x11c
    c2e0:	96 70       	ldaa	*0x70
    c2e2:	c6 70       	ldab	#0x70
    c2e4:	bd af d2    	jsr	0xafd2
    c2e7:	7e d3 b7    	jmp	0xd3b7
    c2ea:	96 70       	ldaa	*0x70
    c2ec:	4a          	deca
    c2ed:	2a e1       	bpl	0x0xc2d0
    c2ef:	4f          	clra
    c2f0:	20 de       	bra	0x0xc2d0
    c2f2:	7f 01 1a    	clr	0x11a
    c2f5:	7f 01 7e    	clr	0x17e
    c2f8:	7e d3 b7    	jmp	0xd3b7
    c2fb:	7d 01 1c    	tst	0x11c
    c2fe:	2a 03       	bpl	0x0xc303
    c300:	7e e1 3e    	jmp	0xe13e
    c303:	7d 01 1e    	tst	0x11e
    c306:	26 08       	bne	0x0xc310
    c308:	7d 01 1c    	tst	0x11c
    c30b:	27 1f       	beq	0x0xc32c
    c30d:	7e b1 32    	jmp	0xb132
    c310:	7d 01 1c    	tst	0x11c
    c313:	27 24       	beq	0x0xc339
    c315:	cc 01 20    	ldd	#0x120
    c318:	fb 01 1e    	addb	0x11e
    c31b:	8f          	xgdx
    c31c:	bd e9 0c    	jsr	0xe90c
    c31f:	7a 01 1e    	dec	0x11e
    c322:	7a 01 1c    	dec	0x11c
    c325:	26 12       	bne	0x0xc339
    c327:	bd e9 8a    	jsr	0xe98a
    c32a:	20 0d       	bra	0x0xc339
    c32c:	7d 01 1e    	tst	0x11e
    c32f:	26 08       	bne	0x0xc339
    c331:	86 03       	ldaa	#0x3
    c333:	b7 01 1e    	staa	0x11e
    c336:	bd e9 48    	jsr	0xe948
    c339:	7d 01 7f    	tst	0x17f
    c33c:	27 10       	beq	0x0xc34e
    c33e:	bd eb 4c    	jsr	0xeb4c
    c341:	4f          	clra
    c342:	b7 01 7f    	staa	0x17f
    c345:	b7 01 7e    	staa	0x17e
    c348:	b7 01 1a    	staa	0x11a
    c34b:	7e d3 b7    	jmp	0xd3b7
    c34e:	7d 01 7e    	tst	0x17e
    c351:	27 0c       	beq	0x0xc35f
    c353:	bd ea f4    	jsr	0xeaf4
    c356:	7f 01 7e    	clr	0x17e
    c359:	7f 01 1a    	clr	0x11a
    c35c:	7e d3 b7    	jmp	0xd3b7
    c35f:	7d 01 1a    	tst	0x11a
    c362:	26 03       	bne	0x0xc367
    c364:	7e d3 b7    	jmp	0xd3b7
    c367:	ce e1 e2    	ldx	#0xe1e2
    c36a:	b6 01 1e    	ldaa	0x11e
    c36d:	81 03       	cmpa	#0x3
    c36f:	26 34       	bne	0x0xc3a5
    c371:	18 ce 01 20 	ldy	#0x120
    c375:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc38f
    c379:	d6 2d       	ldab	*0x2d
    c37b:	bd c4 15    	jsr	0xc415
    c37e:	d7 2d       	stab	*0x2d
    c380:	bd ea d3    	jsr	0xead3
    c383:	96 2d       	ldaa	*0x2d
    c385:	c6 2d       	ldab	#0x2d
    c387:	bd af d2    	jsr	0xafd2
    c38a:	86 04       	ldaa	#0x4
    c38c:	7e c4 c2    	jmp	0xc4c2
    c38f:	d6 3a       	ldab	*0x3a
    c391:	bd c4 15    	jsr	0xc415
    c394:	d7 3a       	stab	*0x3a
    c396:	bd ea d3    	jsr	0xead3
    c399:	96 3a       	ldaa	*0x3a
    c39b:	c6 3a       	ldab	#0x3a
    c39d:	bd af d2    	jsr	0xafd2
    c3a0:	86 04       	ldaa	#0x4
    c3a2:	7e c4 c2    	jmp	0xc4c2
    c3a5:	81 09       	cmpa	#0x9
    c3a7:	26 34       	bne	0x0xc3dd
    c3a9:	18 ce 01 26 	ldy	#0x126
    c3ad:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc3c7
    c3b1:	d6 2e       	ldab	*0x2e
    c3b3:	bd c4 15    	jsr	0xc415
    c3b6:	d7 2e       	stab	*0x2e
    c3b8:	bd ea d3    	jsr	0xead3
    c3bb:	96 2e       	ldaa	*0x2e
    c3bd:	c6 2e       	ldab	#0x2e
    c3bf:	bd af d2    	jsr	0xafd2
    c3c2:	86 04       	ldaa	#0x4
    c3c4:	7e c4 c2    	jmp	0xc4c2
    c3c7:	d6 3b       	ldab	*0x3b
    c3c9:	bd c4 15    	jsr	0xc415
    c3cc:	d7 3b       	stab	*0x3b
    c3ce:	bd ea d3    	jsr	0xead3
    c3d1:	96 3b       	ldaa	*0x3b
    c3d3:	c6 3b       	ldab	#0x3b
    c3d5:	bd af d2    	jsr	0xafd2
    c3d8:	86 04       	ldaa	#0x4
    c3da:	7e c4 c2    	jmp	0xc4c2
    c3dd:	81 0e       	cmpa	#0xe
    c3df:	26 46       	bne	0x0xc427
    c3e1:	18 ce 01 2b 	ldy	#0x12b
    c3e5:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc3ff
    c3e9:	d6 2f       	ldab	*0x2f
    c3eb:	bd c4 15    	jsr	0xc415
    c3ee:	d7 2f       	stab	*0x2f
    c3f0:	bd ea d3    	jsr	0xead3
    c3f3:	96 2f       	ldaa	*0x2f
    c3f5:	c6 2f       	ldab	#0x2f
    c3f7:	bd af d2    	jsr	0xafd2
    c3fa:	86 04       	ldaa	#0x4
    c3fc:	7e c4 c2    	jmp	0xc4c2
    c3ff:	d6 3c       	ldab	*0x3c
    c401:	bd c4 15    	jsr	0xc415
    c404:	d7 3c       	stab	*0x3c
    c406:	bd ea d3    	jsr	0xead3
    c409:	96 3c       	ldaa	*0x3c
    c40b:	c6 3c       	ldab	#0x3c
    c40d:	bd af d2    	jsr	0xafd2
    c410:	86 04       	ldaa	#0x4
    c412:	7e c4 c2    	jmp	0xc4c2
    c415:	7d 01 1a    	tst	0x11a
    c418:	2b 08       	bmi	0x0xc422
    c41a:	5c          	incb
    c41b:	c1 11       	cmpb	#0x11
    c41d:	25 02       	bcs	0x0xc421
    c41f:	c6 10       	ldab	#0x10
    c421:	39          	rts
    c422:	5a          	decb
    c423:	2a fc       	bpl	0x0xc421
    c425:	5f          	clrb
    c426:	39          	rts
    c427:	81 13       	cmpa	#0x13
    c429:	26 33       	bne	0x0xc45e
    c42b:	ce 01 31    	ldx	#0x131
    c42e:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc448
    c432:	96 2a       	ldaa	*0x2a
    c434:	bd c4 cb    	jsr	0xc4cb
    c437:	97 2a       	staa	*0x2a
    c439:	bd ea 20    	jsr	0xea20
    c43c:	96 2a       	ldaa	*0x2a
    c43e:	c6 2a       	ldab	#0x2a
    c440:	bd af d2    	jsr	0xafd2
    c443:	86 03       	ldaa	#0x3
    c445:	7e c4 c2    	jmp	0xc4c2
    c448:	96 37       	ldaa	*0x37
    c44a:	bd c4 cb    	jsr	0xc4cb
    c44d:	97 37       	staa	*0x37
    c44f:	bd ea 20    	jsr	0xea20
    c452:	96 37       	ldaa	*0x37
    c454:	c6 37       	ldab	#0x37
    c456:	bd af d2    	jsr	0xafd2
    c459:	86 03       	ldaa	#0x3
    c45b:	7e c4 c2    	jmp	0xc4c2
    c45e:	81 19       	cmpa	#0x19
    c460:	26 31       	bne	0x0xc493
    c462:	ce 01 37    	ldx	#0x137
    c465:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc47e
    c469:	96 2b       	ldaa	*0x2b
    c46b:	bd c4 cb    	jsr	0xc4cb
    c46e:	97 2b       	staa	*0x2b
    c470:	bd ea 20    	jsr	0xea20
    c473:	96 2b       	ldaa	*0x2b
    c475:	c6 2b       	ldab	#0x2b
    c477:	bd af d2    	jsr	0xafd2
    c47a:	86 03       	ldaa	#0x3
    c47c:	20 44       	bra	0x0xc4c2
    c47e:	96 38       	ldaa	*0x38
    c480:	bd c4 cb    	jsr	0xc4cb
    c483:	97 38       	staa	*0x38
    c485:	bd ea 20    	jsr	0xea20
    c488:	96 38       	ldaa	*0x38
    c48a:	c6 38       	ldab	#0x38
    c48c:	bd af d2    	jsr	0xafd2
    c48f:	86 03       	ldaa	#0x3
    c491:	20 2f       	bra	0x0xc4c2
    c493:	ce 01 3c    	ldx	#0x13c
    c496:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc4af
    c49a:	96 2c       	ldaa	*0x2c
    c49c:	bd c4 cb    	jsr	0xc4cb
    c49f:	97 2c       	staa	*0x2c
    c4a1:	bd ea 20    	jsr	0xea20
    c4a4:	96 2c       	ldaa	*0x2c
    c4a6:	c6 2c       	ldab	#0x2c
    c4a8:	bd af d2    	jsr	0xafd2
    c4ab:	86 03       	ldaa	#0x3
    c4ad:	20 13       	bra	0x0xc4c2
    c4af:	96 39       	ldaa	*0x39
    c4b1:	bd c4 cb    	jsr	0xc4cb
    c4b4:	97 39       	staa	*0x39
    c4b6:	bd ea 20    	jsr	0xea20
    c4b9:	96 39       	ldaa	*0x39
    c4bb:	c6 39       	ldab	#0x39
    c4bd:	bd af d2    	jsr	0xafd2
    c4c0:	86 03       	ldaa	#0x3
    c4c2:	b7 01 1c    	staa	0x11c
    c4c5:	7f 01 1a    	clr	0x11a
    c4c8:	7e d3 b7    	jmp	0xd3b7
    c4cb:	7d 01 1a    	tst	0x11a
    c4ce:	2b 04       	bmi	0x0xc4d4
    c4d0:	4c          	inca
    c4d1:	84 7f       	anda	#0x7f
    c4d3:	39          	rts
    c4d4:	4a          	deca
    c4d5:	84 7f       	anda	#0x7f
    c4d7:	20 fa       	bra	0x0xc4d3
    c4d9:	7d 01 1c    	tst	0x11c
    c4dc:	2a 03       	bpl	0x0xc4e1
    c4de:	7e e2 26    	jmp	0xe226
    c4e1:	7d 01 1e    	tst	0x11e
    c4e4:	26 08       	bne	0x0xc4ee
    c4e6:	7d 01 1c    	tst	0x11c
    c4e9:	27 1f       	beq	0x0xc50a
    c4eb:	7e b1 32    	jmp	0xb132
    c4ee:	7d 01 1c    	tst	0x11c
    c4f1:	27 24       	beq	0x0xc517
    c4f3:	cc 01 20    	ldd	#0x120
    c4f6:	fb 01 1e    	addb	0x11e
    c4f9:	8f          	xgdx
    c4fa:	bd e9 0c    	jsr	0xe90c
    c4fd:	7a 01 1e    	dec	0x11e
    c500:	7a 01 1c    	dec	0x11c
    c503:	26 12       	bne	0x0xc517
    c505:	bd e9 8a    	jsr	0xe98a
    c508:	20 0d       	bra	0x0xc517
    c50a:	7d 01 1e    	tst	0x11e
    c50d:	26 08       	bne	0x0xc517
    c50f:	86 13       	ldaa	#0x13
    c511:	b7 01 1e    	staa	0x11e
    c514:	bd e9 48    	jsr	0xe948
    c517:	7d 01 7f    	tst	0x17f
    c51a:	27 10       	beq	0x0xc52c
    c51c:	bd eb 4c    	jsr	0xeb4c
    c51f:	4f          	clra
    c520:	b7 01 7f    	staa	0x17f
    c523:	b7 01 7e    	staa	0x17e
    c526:	b7 01 1a    	staa	0x11a
    c529:	7e d3 b7    	jmp	0xd3b7
    c52c:	7d 01 1a    	tst	0x11a
    c52f:	26 03       	bne	0x0xc534
    c531:	7e d3 b7    	jmp	0xd3b7
    c534:	b6 01 1e    	ldaa	0x11e
    c537:	81 13       	cmpa	#0x13
    c539:	26 53       	bne	0x0xc58e
    c53b:	ce e2 ab    	ldx	#0xe2ab
    c53e:	18 ce 01 30 	ldy	#0x130
    c542:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc573
    c546:	d6 27       	ldab	*0x27
    c548:	8d 17       	bsr	0x0xc561
    c54a:	d7 27       	stab	*0x27
    c54c:	bd ea d3    	jsr	0xead3
    c54f:	7f 01 1a    	clr	0x11a
    c552:	86 04       	ldaa	#0x4
    c554:	b7 01 1c    	staa	0x11c
    c557:	96 27       	ldaa	*0x27
    c559:	c6 27       	ldab	#0x27
    c55b:	bd af d2    	jsr	0xafd2
    c55e:	7e d3 b7    	jmp	0xd3b7
    c561:	7d 01 1a    	tst	0x11a
    c564:	2b 08       	bmi	0x0xc56e
    c566:	5c          	incb
    c567:	c1 06       	cmpb	#0x6
    c569:	25 02       	bcs	0x0xc56d
    c56b:	c6 05       	ldab	#0x5
    c56d:	39          	rts
    c56e:	5a          	decb
    c56f:	2a fc       	bpl	0x0xc56d
    c571:	5f          	clrb
    c572:	39          	rts
    c573:	d6 34       	ldab	*0x34
    c575:	8d ea       	bsr	0x0xc561
    c577:	d7 34       	stab	*0x34
    c579:	bd ea d3    	jsr	0xead3
    c57c:	7f 01 1a    	clr	0x11a
    c57f:	86 04       	ldaa	#0x4
    c581:	b7 01 1c    	staa	0x11c
    c584:	96 34       	ldaa	*0x34
    c586:	c6 34       	ldab	#0x34
    c588:	bd af d2    	jsr	0xafd2
    c58b:	7e d3 b7    	jmp	0xd3b7
    c58e:	81 19       	cmpa	#0x19
    c590:	26 23       	bne	0x0xc5b5
    c592:	ce e2 c3    	ldx	#0xe2c3
    c595:	18 ce 01 36 	ldy	#0x136
    c599:	d6 31       	ldab	*0x31
    c59b:	5c          	incb
    c59c:	c4 01       	andb	#0x1
    c59e:	d7 31       	stab	*0x31
    c5a0:	bd ea d3    	jsr	0xead3
    c5a3:	7f 01 1a    	clr	0x11a
    c5a6:	86 04       	ldaa	#0x4
    c5a8:	b7 01 1c    	staa	0x11c
    c5ab:	96 31       	ldaa	*0x31
    c5ad:	c6 31       	ldab	#0x31
    c5af:	bd af d2    	jsr	0xafd2
    c5b2:	7e d3 b7    	jmp	0xd3b7
    c5b5:	ce e2 cb    	ldx	#0xe2cb
    c5b8:	18 ce 01 3b 	ldy	#0x13b
    c5bc:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc5ed
    c5c0:	d6 25       	ldab	*0x25
    c5c2:	8d 17       	bsr	0x0xc5db
    c5c4:	d7 25       	stab	*0x25
    c5c6:	bd ea d3    	jsr	0xead3
    c5c9:	7f 01 1a    	clr	0x11a
    c5cc:	86 04       	ldaa	#0x4
    c5ce:	b7 01 1c    	staa	0x11c
    c5d1:	96 25       	ldaa	*0x25
    c5d3:	c6 25       	ldab	#0x25
    c5d5:	bd af d2    	jsr	0xafd2
    c5d8:	7e d3 b7    	jmp	0xd3b7
    c5db:	7d 01 1a    	tst	0x11a
    c5de:	2b 08       	bmi	0x0xc5e8
    c5e0:	5c          	incb
    c5e1:	c1 0d       	cmpb	#0xd
    c5e3:	25 02       	bcs	0x0xc5e7
    c5e5:	c6 0c       	ldab	#0xc
    c5e7:	39          	rts
    c5e8:	5a          	decb
    c5e9:	2a fc       	bpl	0x0xc5e7
    c5eb:	5f          	clrb
    c5ec:	39          	rts
    c5ed:	d6 32       	ldab	*0x32
    c5ef:	8d ea       	bsr	0x0xc5db
    c5f1:	d7 32       	stab	*0x32
    c5f3:	bd ea d3    	jsr	0xead3
    c5f6:	7f 01 1a    	clr	0x11a
    c5f9:	86 04       	ldaa	#0x4
    c5fb:	b7 01 1c    	staa	0x11c
    c5fe:	96 32       	ldaa	*0x32
    c600:	c6 32       	ldab	#0x32
    c602:	bd af d2    	jsr	0xafd2
    c605:	7e d3 b7    	jmp	0xd3b7
    c608:	7d 01 1c    	tst	0x11c
    c60b:	2a 03       	bpl	0x0xc610
    c60d:	7e e2 ff    	jmp	0xe2ff
    c610:	7d 01 1e    	tst	0x11e
    c613:	26 08       	bne	0x0xc61d
    c615:	7d 01 1c    	tst	0x11c
    c618:	27 1f       	beq	0x0xc639
    c61a:	7e b1 32    	jmp	0xb132
    c61d:	7d 01 1c    	tst	0x11c
    c620:	27 24       	beq	0x0xc646
    c622:	cc 01 20    	ldd	#0x120
    c625:	fb 01 1e    	addb	0x11e
    c628:	8f          	xgdx
    c629:	bd e9 0c    	jsr	0xe90c
    c62c:	7a 01 1e    	dec	0x11e
    c62f:	7a 01 1c    	dec	0x11c
    c632:	26 12       	bne	0x0xc646
    c634:	bd e9 8a    	jsr	0xe98a
    c637:	20 0d       	bra	0x0xc646
    c639:	7d 01 1e    	tst	0x11e
    c63c:	26 08       	bne	0x0xc646
    c63e:	86 19       	ldaa	#0x19
    c640:	b7 01 1e    	staa	0x11e
    c643:	bd e9 48    	jsr	0xe948
    c646:	7d 01 7f    	tst	0x17f
    c649:	27 1e       	beq	0x0xc669
    c64b:	2b 10       	bmi	0x0xc65d
    c64d:	bd eb 4c    	jsr	0xeb4c
    c650:	4f          	clra
    c651:	b7 01 7f    	staa	0x17f
    c654:	b7 01 7e    	staa	0x17e
    c657:	b7 01 1a    	staa	0x11a
    c65a:	7e d3 b7    	jmp	0xd3b7
    c65d:	b6 01 1e    	ldaa	0x11e
    c660:	81 19       	cmpa	#0x19
    c662:	27 ec       	beq	0x0xc650
    c664:	bd eb 4c    	jsr	0xeb4c
    c667:	20 e7       	bra	0x0xc650
    c669:	7d 01 1a    	tst	0x11a
    c66c:	26 03       	bne	0x0xc671
    c66e:	7e d3 b7    	jmp	0xd3b7
    c671:	b6 01 1e    	ldaa	0x11e
    c674:	81 13       	cmpa	#0x13
    c676:	26 5a       	bne	0x0xc6d2
    c678:	12 f4 02 2b 	brset	*0xf4, #0x02, 0x0xc6a7
    c67c:	7d 01 1a    	tst	0x11a
    c67f:	2b 1f       	bmi	0x0xc6a0
    c681:	96 29       	ldaa	*0x29
    c683:	4c          	inca
    c684:	84 7f       	anda	#0x7f
    c686:	97 29       	staa	*0x29
    c688:	ce 01 31    	ldx	#0x131
    c68b:	bd ea 20    	jsr	0xea20
    c68e:	7f 01 1a    	clr	0x11a
    c691:	86 03       	ldaa	#0x3
    c693:	b7 01 1c    	staa	0x11c
    c696:	96 29       	ldaa	*0x29
    c698:	c6 29       	ldab	#0x29
    c69a:	bd af d2    	jsr	0xafd2
    c69d:	7e d3 b7    	jmp	0xd3b7
    c6a0:	96 29       	ldaa	*0x29
    c6a2:	4a          	deca
    c6a3:	84 7f       	anda	#0x7f
    c6a5:	20 df       	bra	0x0xc686
    c6a7:	7d 01 1a    	tst	0x11a
    c6aa:	2b 1f       	bmi	0x0xc6cb
    c6ac:	96 36       	ldaa	*0x36
    c6ae:	4c          	inca
    c6af:	84 7f       	anda	#0x7f
    c6b1:	97 36       	staa	*0x36
    c6b3:	ce 01 31    	ldx	#0x131
    c6b6:	bd ea 20    	jsr	0xea20
    c6b9:	7f 01 1a    	clr	0x11a
    c6bc:	86 03       	ldaa	#0x3
    c6be:	b7 01 1c    	staa	0x11c
    c6c1:	96 36       	ldaa	*0x36
    c6c3:	c6 36       	ldab	#0x36
    c6c5:	bd af d2    	jsr	0xafd2
    c6c8:	7e d3 b7    	jmp	0xd3b7
    c6cb:	96 36       	ldaa	*0x36
    c6cd:	4a          	deca
    c6ce:	84 7f       	anda	#0x7f
    c6d0:	20 df       	bra	0x0xc6b1
    c6d2:	81 19       	cmpa	#0x19
    c6d4:	26 65       	bne	0x0xc73b
    c6d6:	ce e3 65    	ldx	#0xe365
    c6d9:	18 ce 01 37 	ldy	#0x137
    c6dd:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc70e
    c6e1:	7d 01 1a    	tst	0x11a
    c6e4:	2b 20       	bmi	0x0xc706
    c6e6:	d6 26       	ldab	*0x26
    c6e8:	5c          	incb
    c6e9:	c1 03       	cmpb	#0x3
    c6eb:	25 02       	bcs	0x0xc6ef
    c6ed:	c6 02       	ldab	#0x2
    c6ef:	d7 26       	stab	*0x26
    c6f1:	bd ea bf    	jsr	0xeabf
    c6f4:	7f 01 1a    	clr	0x11a
    c6f7:	86 03       	ldaa	#0x3
    c6f9:	b7 01 1c    	staa	0x11c
    c6fc:	96 26       	ldaa	*0x26
    c6fe:	c6 26       	ldab	#0x26
    c700:	bd af d2    	jsr	0xafd2
    c703:	7e d3 b7    	jmp	0xd3b7
    c706:	d6 26       	ldab	*0x26
    c708:	5a          	decb
    c709:	2a e4       	bpl	0x0xc6ef
    c70b:	5f          	clrb
    c70c:	20 e1       	bra	0x0xc6ef
    c70e:	7d 01 1a    	tst	0x11a
    c711:	2b 20       	bmi	0x0xc733
    c713:	d6 33       	ldab	*0x33
    c715:	5c          	incb
    c716:	c1 03       	cmpb	#0x3
    c718:	25 02       	bcs	0x0xc71c
    c71a:	c6 02       	ldab	#0x2
    c71c:	d7 33       	stab	*0x33
    c71e:	bd ea bf    	jsr	0xeabf
    c721:	7f 01 1a    	clr	0x11a
    c724:	86 03       	ldaa	#0x3
    c726:	b7 01 1c    	staa	0x11c
    c729:	96 33       	ldaa	*0x33
    c72b:	c6 33       	ldab	#0x33
    c72d:	bd af d2    	jsr	0xafd2
    c730:	7e d3 b7    	jmp	0xd3b7
    c733:	d6 33       	ldab	*0x33
    c735:	5a          	decb
    c736:	2a e4       	bpl	0x0xc71c
    c738:	5f          	clrb
    c739:	20 e1       	bra	0x0xc71c
    c73b:	ce e3 74    	ldx	#0xe374
    c73e:	18 ce 01 3c 	ldy	#0x13c
    c742:	7d 01 1a    	tst	0x11a
    c745:	2b 20       	bmi	0x0xc767
    c747:	d6 96       	ldab	*0x96
    c749:	5c          	incb
    c74a:	c1 04       	cmpb	#0x4
    c74c:	25 02       	bcs	0x0xc750
    c74e:	c6 03       	ldab	#0x3
    c750:	d7 96       	stab	*0x96
    c752:	bd ea bf    	jsr	0xeabf
    c755:	7f 01 1a    	clr	0x11a
    c758:	86 03       	ldaa	#0x3
    c75a:	b7 01 1c    	staa	0x11c
    c75d:	96 96       	ldaa	*0x96
    c75f:	c6 96       	ldab	#0x96
    c761:	bd af d2    	jsr	0xafd2
    c764:	7e d3 b7    	jmp	0xd3b7
    c767:	d6 96       	ldab	*0x96
    c769:	5a          	decb
    c76a:	2a e4       	bpl	0x0xc750
    c76c:	5f          	clrb
    c76d:	20 e1       	bra	0x0xc750
    c76f:	7e d3 b7    	jmp	0xd3b7
    c772:	7d 01 1c    	tst	0x11c
    c775:	2a 03       	bpl	0x0xc77a
    c777:	7e e3 80    	jmp	0xe380
    c77a:	7d 01 1e    	tst	0x11e
    c77d:	26 08       	bne	0x0xc787
    c77f:	7d 01 1c    	tst	0x11c
    c782:	27 1f       	beq	0x0xc7a3
    c784:	7e b1 32    	jmp	0xb132
    c787:	7d 01 1c    	tst	0x11c
    c78a:	27 24       	beq	0x0xc7b0
    c78c:	cc 01 20    	ldd	#0x120
    c78f:	fb 01 1e    	addb	0x11e
    c792:	8f          	xgdx
    c793:	bd e9 0c    	jsr	0xe90c
    c796:	7a 01 1e    	dec	0x11e
    c799:	7a 01 1c    	dec	0x11c
    c79c:	26 12       	bne	0x0xc7b0
    c79e:	bd e9 bd    	jsr	0xe9bd
    c7a1:	20 0d       	bra	0x0xc7b0
    c7a3:	7d 01 1e    	tst	0x11e
    c7a6:	26 08       	bne	0x0xc7b0
    c7a8:	86 02       	ldaa	#0x2
    c7aa:	b7 01 1e    	staa	0x11e
    c7ad:	bd e9 48    	jsr	0xe948
    c7b0:	7d 01 7f    	tst	0x17f
    c7b3:	27 10       	beq	0x0xc7c5
    c7b5:	bd eb be    	jsr	0xebbe
    c7b8:	4f          	clra
    c7b9:	b7 01 7f    	staa	0x17f
    c7bc:	b7 01 7e    	staa	0x17e
    c7bf:	b7 01 1a    	staa	0x11a
    c7c2:	7e d3 b7    	jmp	0xd3b7
    c7c5:	7d 01 7e    	tst	0x17e
    c7c8:	27 0c       	beq	0x0xc7d6
    c7ca:	bd ea f4    	jsr	0xeaf4
    c7cd:	7f 01 7e    	clr	0x17e
    c7d0:	7f 01 1a    	clr	0x11a
    c7d3:	7e d3 b7    	jmp	0xd3b7
    c7d6:	7d 01 1a    	tst	0x11a
    c7d9:	26 03       	bne	0x0xc7de
    c7db:	7e d3 b7    	jmp	0xd3b7
    c7de:	c6 02       	ldab	#0x2
    c7e0:	18 ce 00 b0 	ldy	#0xb0
    c7e4:	96 f9       	ldaa	*0xf9
    c7e6:	81 04       	cmpa	#0x4
    c7e8:	26 07       	bne	0x0xc7f1
    c7ea:	18 ce 50 30 	ldy	#0x5030
    c7ee:	14 fb 80    	bset	*0xfb, #0x80
    c7f1:	ce 01 20    	ldx	#0x120
    c7f4:	f1 01 1e    	cmpb	0x11e
    c7f7:	27 0a       	beq	0x0xc803
    c7f9:	18 08       	iny
    c7fb:	cb 04       	addb	#0x4
    c7fd:	08          	inx
    c7fe:	08          	inx
    c7ff:	08          	inx
    c800:	08          	inx
    c801:	20 f1       	bra	0x0xc7f4
    c803:	18 a6 00    	ldaa	0x0,y
    c806:	7d 01 1a    	tst	0x11a
    c809:	2b 23       	bmi	0x0xc82e
    c80b:	4c          	inca
    c80c:	f6 01 11    	ldab	0x111
    c80f:	c5 04       	bitb	#0x4
    c811:	27 02       	beq	0x0xc815
    c813:	8b 09       	adda	#0x9
    c815:	4d          	tsta
    c816:	2a 02       	bpl	0x0xc81a
    c818:	86 7f       	ldaa	#0x7f
    c81a:	18 a7 00    	staa	0x0,y
    c81d:	bd ea 5c    	jsr	0xea5c
    c820:	bd c8 3e    	jsr	0xc83e
    c823:	7f 01 1a    	clr	0x11a
    c826:	86 03       	ldaa	#0x3
    c828:	b7 01 1c    	staa	0x11c
    c82b:	7e d3 b7    	jmp	0xd3b7
    c82e:	4a          	deca
    c82f:	f6 01 11    	ldab	0x111
    c832:	c5 04       	bitb	#0x4
    c834:	27 02       	beq	0x0xc838
    c836:	80 09       	suba	#0x9
    c838:	4d          	tsta
    c839:	2a df       	bpl	0x0xc81a
    c83b:	4f          	clra
    c83c:	20 dc       	bra	0x0xc81a
    c83e:	18 a6 00    	ldaa	0x0,y
    c841:	36          	psha
    c842:	18 8f       	xgdy
    c844:	c1 3f       	cmpb	#0x3f
    c846:	22 02       	bhi	0x0xc84a
    c848:	cb 80       	addb	#0x80
    c84a:	37          	pshb
    c84b:	c4 0f       	andb	#0xf
    c84d:	c1 08       	cmpb	#0x8
    c84f:	25 02       	bcs	0x0xc853
    c851:	c0 08       	subb	#0x8
    c853:	5c          	incb
    c854:	86 01       	ldaa	#0x1
    c856:	5a          	decb
    c857:	27 03       	beq	0x0xc85c
    c859:	48          	asla
    c85a:	20 fa       	bra	0x0xc856
    c85c:	43          	coma
    c85d:	7d 00 d0    	tst	0xd0
    c860:	27 05       	beq	0x0xc867
    c862:	7f 00 d0    	clr	0xd0
    c865:	20 08       	bra	0x0xc86f
    c867:	7d 10 29    	tst	0x1029
    c86a:	2a fb       	bpl	0x0xc867
    c86c:	f6 10 2a    	ldab	0x102a
    c86f:	01          	nop
    c870:	01          	nop
    c871:	01          	nop
    c872:	01          	nop
    c873:	b7 10 42    	staa	0x1042
    c876:	01          	nop
    c877:	01          	nop
    c878:	01          	nop
    c879:	01          	nop
    c87a:	01          	nop
    c87b:	01          	nop
    c87c:	01          	nop
    c87d:	86 83       	ldaa	#0x83
    c87f:	b7 10 2a    	staa	0x102a
    c882:	32          	pula
    c883:	81 b8       	cmpa	#0xb8
    c885:	25 04       	bcs	0x0xc88b
    c887:	86 8a       	ldaa	#0x8a
    c889:	20 0a       	bra	0x0xc895
    c88b:	81 b0       	cmpa	#0xb0
    c88d:	25 04       	bcs	0x0xc893
    c88f:	86 89       	ldaa	#0x89
    c891:	20 02       	bra	0x0xc895
    c893:	86 88       	ldaa	#0x88
    c895:	7d 10 29    	tst	0x1029
    c898:	2a fb       	bpl	0x0xc895
    c89a:	f6 10 2a    	ldab	0x102a
    c89d:	b7 10 2a    	staa	0x102a
    c8a0:	32          	pula
    c8a1:	7d 10 29    	tst	0x1029
    c8a4:	2a fb       	bpl	0x0xc8a1
    c8a6:	f6 10 2a    	ldab	0x102a
    c8a9:	b7 10 2a    	staa	0x102a
    c8ac:	39          	rts
    c8ad:	7d 01 1c    	tst	0x11c
    c8b0:	2a 03       	bpl	0x0xc8b5
    c8b2:	7e e3 ca    	jmp	0xe3ca
    c8b5:	7d 01 1e    	tst	0x11e
    c8b8:	26 08       	bne	0x0xc8c2
    c8ba:	7d 01 1c    	tst	0x11c
    c8bd:	27 1f       	beq	0x0xc8de
    c8bf:	7e b1 32    	jmp	0xb132
    c8c2:	7d 01 1c    	tst	0x11c
    c8c5:	27 24       	beq	0x0xc8eb
    c8c7:	cc 01 20    	ldd	#0x120
    c8ca:	fb 01 1e    	addb	0x11e
    c8cd:	8f          	xgdx
    c8ce:	bd e9 0c    	jsr	0xe90c
    c8d1:	7a 01 1e    	dec	0x11e
    c8d4:	7a 01 1c    	dec	0x11c
    c8d7:	26 12       	bne	0x0xc8eb
    c8d9:	bd e9 bd    	jsr	0xe9bd
    c8dc:	20 0d       	bra	0x0xc8eb
    c8de:	7d 01 1e    	tst	0x11e
    c8e1:	26 08       	bne	0x0xc8eb
    c8e3:	86 02       	ldaa	#0x2
    c8e5:	b7 01 1e    	staa	0x11e
    c8e8:	bd e9 48    	jsr	0xe948
    c8eb:	7d 01 7f    	tst	0x17f
    c8ee:	27 10       	beq	0x0xc900
    c8f0:	bd eb be    	jsr	0xebbe
    c8f3:	4f          	clra
    c8f4:	b7 01 7f    	staa	0x17f
    c8f7:	b7 01 7e    	staa	0x17e
    c8fa:	b7 01 1a    	staa	0x11a
    c8fd:	7e d3 b7    	jmp	0xd3b7
    c900:	7d 01 7e    	tst	0x17e
    c903:	27 0c       	beq	0x0xc911
    c905:	bd ea f4    	jsr	0xeaf4
    c908:	7f 01 7e    	clr	0x17e
    c90b:	7f 01 1a    	clr	0x11a
    c90e:	7e d3 b7    	jmp	0xd3b7
    c911:	7d 01 1a    	tst	0x11a
    c914:	26 03       	bne	0x0xc919
    c916:	7e d3 b7    	jmp	0xd3b7
    c919:	c6 02       	ldab	#0x2
    c91b:	18 ce 00 a8 	ldy	#0xa8
    c91f:	96 f9       	ldaa	*0xf9
    c921:	81 04       	cmpa	#0x4
    c923:	26 07       	bne	0x0xc92c
    c925:	14 fb 80    	bset	*0xfb, #0x80
    c928:	18 ce 50 28 	ldy	#0x5028
    c92c:	ce 01 20    	ldx	#0x120
    c92f:	f1 01 1e    	cmpb	0x11e
    c932:	27 0a       	beq	0x0xc93e
    c934:	18 08       	iny
    c936:	cb 04       	addb	#0x4
    c938:	08          	inx
    c939:	08          	inx
    c93a:	08          	inx
    c93b:	08          	inx
    c93c:	20 f1       	bra	0x0xc92f
    c93e:	18 a6 00    	ldaa	0x0,y
    c941:	7d 01 1a    	tst	0x11a
    c944:	2b 20       	bmi	0x0xc966
    c946:	4c          	inca
    c947:	f6 01 11    	ldab	0x111
    c94a:	c5 04       	bitb	#0x4
    c94c:	27 02       	beq	0x0xc950
    c94e:	8b 09       	adda	#0x9
    c950:	84 7f       	anda	#0x7f
    c952:	18 a7 00    	staa	0x0,y
    c955:	bd ea 20    	jsr	0xea20
    c958:	bd c8 3e    	jsr	0xc83e
    c95b:	7f 01 1a    	clr	0x11a
    c95e:	86 03       	ldaa	#0x3
    c960:	b7 01 1c    	staa	0x11c
    c963:	7e d3 b7    	jmp	0xd3b7
    c966:	4a          	deca
    c967:	f6 01 11    	ldab	0x111
    c96a:	c5 04       	bitb	#0x4
    c96c:	27 02       	beq	0x0xc970
    c96e:	80 09       	suba	#0x9
    c970:	84 7f       	anda	#0x7f
    c972:	20 de       	bra	0x0xc952
    c974:	7d 01 1c    	tst	0x11c
    c977:	2a 03       	bpl	0x0xc97c
    c979:	7e e4 03    	jmp	0xe403
    c97c:	7d 01 1e    	tst	0x11e
    c97f:	26 08       	bne	0x0xc989
    c981:	7d 01 1c    	tst	0x11c
    c984:	27 1f       	beq	0x0xc9a5
    c986:	7e b1 32    	jmp	0xb132
    c989:	7d 01 1c    	tst	0x11c
    c98c:	27 24       	beq	0x0xc9b2
    c98e:	cc 01 20    	ldd	#0x120
    c991:	fb 01 1e    	addb	0x11e
    c994:	8f          	xgdx
    c995:	bd e9 0c    	jsr	0xe90c
    c998:	7a 01 1e    	dec	0x11e
    c99b:	7a 01 1c    	dec	0x11c
    c99e:	26 12       	bne	0x0xc9b2
    c9a0:	bd e9 bd    	jsr	0xe9bd
    c9a3:	20 0d       	bra	0x0xc9b2
    c9a5:	7d 01 1e    	tst	0x11e
    c9a8:	26 08       	bne	0x0xc9b2
    c9aa:	86 02       	ldaa	#0x2
    c9ac:	b7 01 1e    	staa	0x11e
    c9af:	bd e9 48    	jsr	0xe948
    c9b2:	7d 01 7f    	tst	0x17f
    c9b5:	27 10       	beq	0x0xc9c7
    c9b7:	bd eb be    	jsr	0xebbe
    c9ba:	4f          	clra
    c9bb:	b7 01 7f    	staa	0x17f
    c9be:	b7 01 7e    	staa	0x17e
    c9c1:	b7 01 1a    	staa	0x11a
    c9c4:	7e d3 b7    	jmp	0xd3b7
    c9c7:	7d 01 7e    	tst	0x17e
    c9ca:	27 0c       	beq	0x0xc9d8
    c9cc:	bd ea f4    	jsr	0xeaf4
    c9cf:	7f 01 7e    	clr	0x17e
    c9d2:	7f 01 1a    	clr	0x11a
    c9d5:	7e d3 b7    	jmp	0xd3b7
    c9d8:	7d 01 1a    	tst	0x11a
    c9db:	26 03       	bne	0x0xc9e0
    c9dd:	7e d3 b7    	jmp	0xd3b7
    c9e0:	c6 02       	ldab	#0x2
    c9e2:	18 ce 00 b8 	ldy	#0xb8
    c9e6:	96 f9       	ldaa	*0xf9
    c9e8:	81 04       	cmpa	#0x4
    c9ea:	26 07       	bne	0x0xc9f3
    c9ec:	14 fb 80    	bset	*0xfb, #0x80
    c9ef:	18 ce 50 38 	ldy	#0x5038
    c9f3:	ce 01 20    	ldx	#0x120
    c9f6:	f1 01 1e    	cmpb	0x11e
    c9f9:	27 0a       	beq	0x0xca05
    c9fb:	18 08       	iny
    c9fd:	cb 04       	addb	#0x4
    c9ff:	08          	inx
    ca00:	08          	inx
    ca01:	08          	inx
    ca02:	08          	inx
    ca03:	20 f1       	bra	0x0xc9f6
    ca05:	18 a6 00    	ldaa	0x0,y
    ca08:	7d 01 1a    	tst	0x11a
    ca0b:	2b 20       	bmi	0x0xca2d
    ca0d:	4c          	inca
    ca0e:	f6 01 11    	ldab	0x111
    ca11:	c5 04       	bitb	#0x4
    ca13:	27 02       	beq	0x0xca17
    ca15:	8b 09       	adda	#0x9
    ca17:	84 7f       	anda	#0x7f
    ca19:	18 a7 00    	staa	0x0,y
    ca1c:	bd ea 20    	jsr	0xea20
    ca1f:	bd c8 3e    	jsr	0xc83e
    ca22:	7f 01 1a    	clr	0x11a
    ca25:	86 03       	ldaa	#0x3
    ca27:	b7 01 1c    	staa	0x11c
    ca2a:	7e d3 b7    	jmp	0xd3b7
    ca2d:	4a          	deca
    ca2e:	f6 01 11    	ldab	0x111
    ca31:	c5 04       	bitb	#0x4
    ca33:	27 02       	beq	0x0xca37
    ca35:	80 09       	suba	#0x9
    ca37:	84 7f       	anda	#0x7f
    ca39:	20 de       	bra	0x0xca19
    ca3b:	7d 01 1c    	tst	0x11c
    ca3e:	2a 03       	bpl	0x0xca43
    ca40:	7e e4 3c    	jmp	0xe43c
    ca43:	7d 01 1e    	tst	0x11e
    ca46:	26 08       	bne	0x0xca50
    ca48:	7d 01 1c    	tst	0x11c
    ca4b:	27 1f       	beq	0x0xca6c
    ca4d:	7e b1 32    	jmp	0xb132
    ca50:	7d 01 1c    	tst	0x11c
    ca53:	27 27       	beq	0x0xca7c
    ca55:	cc 01 20    	ldd	#0x120
    ca58:	fb 01 1e    	addb	0x11e
    ca5b:	8f          	xgdx
    ca5c:	bd e9 0c    	jsr	0xe90c
    ca5f:	7a 01 1e    	dec	0x11e
    ca62:	7a 01 1c    	dec	0x11c
    ca65:	26 15       	bne	0x0xca7c
    ca67:	bd e9 8a    	jsr	0xe98a
    ca6a:	20 10       	bra	0x0xca7c
    ca6c:	7d 01 1e    	tst	0x11e
    ca6f:	26 0b       	bne	0x0xca7c
    ca71:	86 13       	ldaa	#0x13
    ca73:	b7 01 1e    	staa	0x11e
    ca76:	bd e9 48    	jsr	0xe948
    ca79:	7f 01 6b    	clr	0x16b
    ca7c:	7d 01 7f    	tst	0x17f
    ca7f:	27 10       	beq	0x0xca91
    ca81:	bd eb 4c    	jsr	0xeb4c
    ca84:	4f          	clra
    ca85:	b7 01 7f    	staa	0x17f
    ca88:	b7 01 7e    	staa	0x17e
    ca8b:	b7 01 1a    	staa	0x11a
    ca8e:	7e d3 b7    	jmp	0xd3b7
    ca91:	7d 01 1a    	tst	0x11a
    ca94:	26 03       	bne	0x0xca99
    ca96:	7e d3 b7    	jmp	0xd3b7
    ca99:	b6 01 1e    	ldaa	0x11e
    ca9c:	81 13       	cmpa	#0x13
    ca9e:	26 23       	bne	0x0xcac3
    caa0:	ce e2 ab    	ldx	#0xe2ab
    caa3:	18 ce 01 30 	ldy	#0x130
    caa7:	d6 a7       	ldab	*0xa7
    caa9:	bd c5 61    	jsr	0xc561
    caac:	d7 a7       	stab	*0xa7
    caae:	bd ea d3    	jsr	0xead3
    cab1:	7f 01 1a    	clr	0x11a
    cab4:	86 04       	ldaa	#0x4
    cab6:	b7 01 1c    	staa	0x11c
    cab9:	96 a7       	ldaa	*0xa7
    cabb:	c6 a7       	ldab	#0xa7
    cabd:	bd af d2    	jsr	0xafd2
    cac0:	7e d3 b7    	jmp	0xd3b7
    cac3:	81 19       	cmpa	#0x19
    cac5:	26 32       	bne	0x0xcaf9
    cac7:	ce e3 65    	ldx	#0xe365
    caca:	18 ce 01 37 	ldy	#0x137
    cace:	d6 a6       	ldab	*0xa6
    cad0:	7d 01 1a    	tst	0x11a
    cad3:	2b 1e       	bmi	0x0xcaf3
    cad5:	5c          	incb
    cad6:	c1 03       	cmpb	#0x3
    cad8:	25 02       	bcs	0x0xcadc
    cada:	c6 02       	ldab	#0x2
    cadc:	d7 a6       	stab	*0xa6
    cade:	bd ea bf    	jsr	0xeabf
    cae1:	7f 01 1a    	clr	0x11a
    cae4:	86 03       	ldaa	#0x3
    cae6:	b7 01 1c    	staa	0x11c
    cae9:	96 a6       	ldaa	*0xa6
    caeb:	c6 a6       	ldab	#0xa6
    caed:	bd af d2    	jsr	0xafd2
    caf0:	7e d3 b7    	jmp	0xd3b7
    caf3:	5a          	decb
    caf4:	2a e6       	bpl	0x0xcadc
    caf6:	5f          	clrb
    caf7:	20 e3       	bra	0x0xcadc
    caf9:	ce e2 cb    	ldx	#0xe2cb
    cafc:	18 ce 01 3b 	ldy	#0x13b
    cb00:	d6 a5       	ldab	*0xa5
    cb02:	bd c5 db    	jsr	0xc5db
    cb05:	d7 a5       	stab	*0xa5
    cb07:	bd ea d3    	jsr	0xead3
    cb0a:	7f 01 1a    	clr	0x11a
    cb0d:	86 04       	ldaa	#0x4
    cb0f:	b7 01 1c    	staa	0x11c
    cb12:	96 a5       	ldaa	*0xa5
    cb14:	c6 a5       	ldab	#0xa5
    cb16:	bd af d2    	jsr	0xafd2
    cb19:	7e d3 b7    	jmp	0xd3b7
    cb1c:	7f 01 7f    	clr	0x17f
    cb1f:	7f 01 7e    	clr	0x17e
    cb22:	7f 01 1a    	clr	0x11a
    cb25:	7e d3 b7    	jmp	0xd3b7
    cb28:	7d 01 1c    	tst	0x11c
    cb2b:	2a 03       	bpl	0x0xcb30
    cb2d:	7e e4 9a    	jmp	0xe49a
    cb30:	7d 01 1e    	tst	0x11e
    cb33:	26 08       	bne	0x0xcb3d
    cb35:	7d 01 1c    	tst	0x11c
    cb38:	27 1f       	beq	0x0xcb59
    cb3a:	7e b1 32    	jmp	0xb132
    cb3d:	7d 01 1c    	tst	0x11c
    cb40:	27 24       	beq	0x0xcb66
    cb42:	cc 01 20    	ldd	#0x120
    cb45:	fb 01 1e    	addb	0x11e
    cb48:	8f          	xgdx
    cb49:	bd e9 0c    	jsr	0xe90c
    cb4c:	7a 01 1e    	dec	0x11e
    cb4f:	7a 01 1c    	dec	0x11c
    cb52:	26 12       	bne	0x0xcb66
    cb54:	bd e9 bd    	jsr	0xe9bd
    cb57:	20 0d       	bra	0x0xcb66
    cb59:	7d 01 1e    	tst	0x11e
    cb5c:	26 08       	bne	0x0xcb66
    cb5e:	86 12       	ldaa	#0x12
    cb60:	b7 01 1e    	staa	0x11e
    cb63:	bd e9 48    	jsr	0xe948
    cb66:	7d 01 7f    	tst	0x17f
    cb69:	27 10       	beq	0x0xcb7b
    cb6b:	bd eb be    	jsr	0xebbe
    cb6e:	4f          	clra
    cb6f:	b7 01 7f    	staa	0x17f
    cb72:	b7 01 7e    	staa	0x17e
    cb75:	b7 01 1a    	staa	0x11a
    cb78:	7e d3 b7    	jmp	0xd3b7
    cb7b:	7d 01 1a    	tst	0x11a
    cb7e:	26 03       	bne	0x0xcb83
    cb80:	7e d3 b7    	jmp	0xd3b7
    cb83:	b6 01 1e    	ldaa	0x11e
    cb86:	81 12       	cmpa	#0x12
    cb88:	26 16       	bne	0x0xcba0
    cb8a:	d6 21       	ldab	*0x21
    cb8c:	c8 02       	eorb	#0x2
    cb8e:	d7 21       	stab	*0x21
    cb90:	c4 02       	andb	#0x2
    cb92:	54          	lsrb
    cb93:	18 ce 01 30 	ldy	#0x130
    cb97:	ce d9 74    	ldx	#0xd974
    cb9a:	bd ea bf    	jsr	0xeabf
    cb9d:	7e b7 e5    	jmp	0xb7e5
    cba0:	81 16       	cmpa	#0x16
    cba2:	26 17       	bne	0x0xcbbb
    cba4:	d6 21       	ldab	*0x21
    cba6:	c8 04       	eorb	#0x4
    cba8:	d7 21       	stab	*0x21
    cbaa:	c4 04       	andb	#0x4
    cbac:	54          	lsrb
    cbad:	54          	lsrb
    cbae:	18 ce 01 34 	ldy	#0x134
    cbb2:	ce d9 74    	ldx	#0xd974
    cbb5:	bd ea bf    	jsr	0xeabf
    cbb8:	7e b7 e5    	jmp	0xb7e5
    cbbb:	81 1a       	cmpa	#0x1a
    cbbd:	26 30       	bne	0x0xcbef
    cbbf:	b6 01 1a    	ldaa	0x11a
    cbc2:	2b 23       	bmi	0x0xcbe7
    cbc4:	96 22       	ldaa	*0x22
    cbc6:	4c          	inca
    cbc7:	81 7f       	cmpa	#0x7f
    cbc9:	25 02       	bcs	0x0xcbcd
    cbcb:	86 7f       	ldaa	#0x7f
    cbcd:	97 22       	staa	*0x22
    cbcf:	ce 01 38    	ldx	#0x138
    cbd2:	bd ea 5c    	jsr	0xea5c
    cbd5:	7f 01 1a    	clr	0x11a
    cbd8:	86 03       	ldaa	#0x3
    cbda:	b7 01 1c    	staa	0x11c
    cbdd:	96 22       	ldaa	*0x22
    cbdf:	c6 22       	ldab	#0x22
    cbe1:	bd af d2    	jsr	0xafd2
    cbe4:	7e d3 b7    	jmp	0xd3b7
    cbe7:	96 22       	ldaa	*0x22
    cbe9:	4a          	deca
    cbea:	2a e1       	bpl	0x0xcbcd
    cbec:	4f          	clra
    cbed:	20 de       	bra	0x0xcbcd
    cbef:	b6 01 1a    	ldaa	0x11a
    cbf2:	2b 1f       	bmi	0x0xcc13
    cbf4:	96 23       	ldaa	*0x23
    cbf6:	4c          	inca
    cbf7:	84 7f       	anda	#0x7f
    cbf9:	97 23       	staa	*0x23
    cbfb:	ce 01 3c    	ldx	#0x13c
    cbfe:	bd ea 20    	jsr	0xea20
    cc01:	7f 01 1a    	clr	0x11a
    cc04:	86 03       	ldaa	#0x3
    cc06:	b7 01 1c    	staa	0x11c
    cc09:	96 23       	ldaa	*0x23
    cc0b:	c6 23       	ldab	#0x23
    cc0d:	bd af d2    	jsr	0xafd2
    cc10:	7e d3 b7    	jmp	0xd3b7
    cc13:	96 23       	ldaa	*0x23
    cc15:	4a          	deca
    cc16:	84 7f       	anda	#0x7f
    cc18:	20 df       	bra	0x0xcbf9
    cc1a:	7d 01 1c    	tst	0x11c
    cc1d:	2a 03       	bpl	0x0xcc22
    cc1f:	7e e5 cc    	jmp	0xe5cc
    cc22:	7d 01 1e    	tst	0x11e
    cc25:	26 08       	bne	0x0xcc2f
    cc27:	7d 01 1c    	tst	0x11c
    cc2a:	27 1f       	beq	0x0xcc4b
    cc2c:	7e b1 32    	jmp	0xb132
    cc2f:	7d 01 1c    	tst	0x11c
    cc32:	27 24       	beq	0x0xcc58
    cc34:	cc 01 20    	ldd	#0x120
    cc37:	fb 01 1e    	addb	0x11e
    cc3a:	8f          	xgdx
    cc3b:	bd e9 0c    	jsr	0xe90c
    cc3e:	7a 01 1e    	dec	0x11e
    cc41:	7a 01 1c    	dec	0x11c
    cc44:	26 12       	bne	0x0xcc58
    cc46:	bd e9 8a    	jsr	0xe98a
    cc49:	20 0d       	bra	0x0xcc58
    cc4b:	7d 01 1e    	tst	0x11e
    cc4e:	26 08       	bne	0x0xcc58
    cc50:	86 13       	ldaa	#0x13
    cc52:	b7 01 1e    	staa	0x11e
    cc55:	bd e9 48    	jsr	0xe948
    cc58:	7d 01 7f    	tst	0x17f
    cc5b:	27 0d       	beq	0x0xcc6a
    cc5d:	4f          	clra
    cc5e:	b7 01 7f    	staa	0x17f
    cc61:	b7 01 7e    	staa	0x17e
    cc64:	b7 01 1a    	staa	0x11a
    cc67:	7e d3 b7    	jmp	0xd3b7
    cc6a:	7d 01 1a    	tst	0x11a
    cc6d:	26 03       	bne	0x0xcc72
    cc6f:	7e d3 b7    	jmp	0xd3b7
    cc72:	7d 01 1a    	tst	0x11a
    cc75:	2b 26       	bmi	0x0xcc9d
    cc77:	96 f0       	ldaa	*0xf0
    cc79:	84 07       	anda	#0x7
    cc7b:	4c          	inca
    cc7c:	84 07       	anda	#0x7
    cc7e:	97 f0       	staa	*0xf0
    cc80:	bd ab 4b    	jsr	0xab4b
    cc83:	96 f0       	ldaa	*0xf0
    cc85:	b7 7f fa    	staa	0x7ffa
    cc88:	4c          	inca
    cc89:	ce 01 31    	ldx	#0x131
    cc8c:	bd ea 20    	jsr	0xea20
    cc8f:	7f 01 1a    	clr	0x11a
    cc92:	86 03       	ldaa	#0x3
    cc94:	b7 01 1c    	staa	0x11c
    cc97:	bd ab 71    	jsr	0xab71
    cc9a:	7e d3 b7    	jmp	0xd3b7
    cc9d:	96 f0       	ldaa	*0xf0
    cc9f:	84 07       	anda	#0x7
    cca1:	4a          	deca
    cca2:	20 d8       	bra	0x0xcc7c
    cca4:	7d 01 1c    	tst	0x11c
    cca7:	2a 03       	bpl	0x0xccac
    cca9:	7e e6 1e    	jmp	0xe61e
    ccac:	7d 01 1e    	tst	0x11e
    ccaf:	26 08       	bne	0x0xccb9
    ccb1:	7d 01 1c    	tst	0x11c
    ccb4:	27 1f       	beq	0x0xccd5
    ccb6:	7e b1 32    	jmp	0xb132
    ccb9:	7d 01 1c    	tst	0x11c
    ccbc:	27 24       	beq	0x0xcce2
    ccbe:	cc 01 20    	ldd	#0x120
    ccc1:	fb 01 1e    	addb	0x11e
    ccc4:	8f          	xgdx
    ccc5:	bd e9 0c    	jsr	0xe90c
    ccc8:	7a 01 1e    	dec	0x11e
    cccb:	7a 01 1c    	dec	0x11c
    ccce:	26 12       	bne	0x0xcce2
    ccd0:	bd e9 8a    	jsr	0xe98a
    ccd3:	20 0d       	bra	0x0xcce2
    ccd5:	7d 01 1e    	tst	0x11e
    ccd8:	26 08       	bne	0x0xcce2
    ccda:	86 13       	ldaa	#0x13
    ccdc:	b7 01 1e    	staa	0x11e
    ccdf:	bd e9 48    	jsr	0xe948
    cce2:	7d 01 7f    	tst	0x17f
    cce5:	27 0d       	beq	0x0xccf4
    cce7:	4f          	clra
    cce8:	b7 01 7f    	staa	0x17f
    cceb:	b7 01 7e    	staa	0x17e
    ccee:	b7 01 1a    	staa	0x11a
    ccf1:	7e d3 b7    	jmp	0xd3b7
    ccf4:	7d 01 1a    	tst	0x11a
    ccf7:	26 03       	bne	0x0xccfc
    ccf9:	7e d3 b7    	jmp	0xd3b7
    ccfc:	7d 01 1a    	tst	0x11a
    ccff:	2b 29       	bmi	0x0xcd2a
    cd01:	96 de       	ldaa	*0xde
    cd03:	4c          	inca
    cd04:	81 02       	cmpa	#0x2
    cd06:	23 02       	bls	0x0xcd0a
    cd08:	86 02       	ldaa	#0x2
    cd0a:	97 de       	staa	*0xde
    cd0c:	bd ab 4b    	jsr	0xab4b
    cd0f:	96 de       	ldaa	*0xde
    cd11:	b7 7f f9    	staa	0x7ff9
    cd14:	4c          	inca
    cd15:	4c          	inca
    cd16:	ce 01 31    	ldx	#0x131
    cd19:	bd ea 20    	jsr	0xea20
    cd1c:	7f 01 1a    	clr	0x11a
    cd1f:	86 03       	ldaa	#0x3
    cd21:	b7 01 1c    	staa	0x11c
    cd24:	bd ab 71    	jsr	0xab71
    cd27:	7e d3 b7    	jmp	0xd3b7
    cd2a:	96 de       	ldaa	*0xde
    cd2c:	4a          	deca
    cd2d:	2a db       	bpl	0x0xcd0a
    cd2f:	4f          	clra
    cd30:	20 d8       	bra	0x0xcd0a
    cd32:	7d 01 1c    	tst	0x11c
    cd35:	2a 03       	bpl	0x0xcd3a
    cd37:	7e e8 18    	jmp	0xe818
    cd3a:	7d 01 1e    	tst	0x11e
    cd3d:	26 08       	bne	0x0xcd47
    cd3f:	7d 01 1c    	tst	0x11c
    cd42:	27 1a       	beq	0x0xcd5e
    cd44:	7e b1 32    	jmp	0xb132
    cd47:	7d 01 1c    	tst	0x11c
    cd4a:	27 1a       	beq	0x0xcd66
    cd4c:	cc 01 20    	ldd	#0x120
    cd4f:	fb 01 1e    	addb	0x11e
    cd52:	8f          	xgdx
    cd53:	bd e9 0c    	jsr	0xe90c
    cd56:	7a 01 1c    	dec	0x11c
    cd59:	bd e9 48    	jsr	0xe948
    cd5c:	20 08       	bra	0x0xcd66
    cd5e:	86 0f       	ldaa	#0xf
    cd60:	b7 01 1e    	staa	0x11e
    cd63:	bd e9 48    	jsr	0xe948
    cd66:	7d 01 7f    	tst	0x17f
    cd69:	27 0d       	beq	0x0xcd78
    cd6b:	4f          	clra
    cd6c:	b7 01 7f    	staa	0x17f
    cd6f:	b7 01 7e    	staa	0x17e
    cd72:	b7 01 1a    	staa	0x11a
    cd75:	7e d3 b7    	jmp	0xd3b7
    cd78:	7d 01 7e    	tst	0x17e
    cd7b:	27 29       	beq	0x0xcda6
    cd7d:	b6 01 1e    	ldaa	0x11e
    cd80:	81 0f       	cmpa	#0xf
    cd82:	26 11       	bne	0x0xcd95
    cd84:	86 1f       	ldaa	#0x1f
    cd86:	b7 01 1e    	staa	0x11e
    cd89:	bd e9 48    	jsr	0xe948
    cd8c:	7f 01 7e    	clr	0x17e
    cd8f:	7f 01 1a    	clr	0x11a
    cd92:	7e d3 b7    	jmp	0xd3b7
    cd95:	86 0f       	ldaa	#0xf
    cd97:	b7 01 1e    	staa	0x11e
    cd9a:	bd e9 48    	jsr	0xe948
    cd9d:	7f 01 7e    	clr	0x17e
    cda0:	7f 01 1a    	clr	0x11a
    cda3:	7e d3 b7    	jmp	0xd3b7
    cda6:	7d 01 1a    	tst	0x11a
    cda9:	26 03       	bne	0x0xcdae
    cdab:	7e d3 b7    	jmp	0xd3b7
    cdae:	b6 01 1e    	ldaa	0x11e
    cdb1:	81 0f       	cmpa	#0xf
    cdb3:	26 13       	bne	0x0xcdc8
    cdb5:	b6 01 2f    	ldaa	0x12f
    cdb8:	81 43       	cmpa	#0x43
    cdba:	26 06       	bne	0x0xcdc2
    cdbc:	4c          	inca
    cdbd:	b7 01 2f    	staa	0x12f
    cdc0:	20 17       	bra	0x0xcdd9
    cdc2:	4a          	deca
    cdc3:	b7 01 2f    	staa	0x12f
    cdc6:	20 11       	bra	0x0xcdd9
    cdc8:	b6 01 3f    	ldaa	0x13f
    cdcb:	81 41       	cmpa	#0x41
    cdcd:	26 06       	bne	0x0xcdd5
    cdcf:	4c          	inca
    cdd0:	b7 01 3f    	staa	0x13f
    cdd3:	20 04       	bra	0x0xcdd9
    cdd5:	4a          	deca
    cdd6:	b7 01 3f    	staa	0x13f
    cdd9:	7f 01 1a    	clr	0x11a
    cddc:	86 01       	ldaa	#0x1
    cdde:	b7 01 1c    	staa	0x11c
    cde1:	7e d3 b7    	jmp	0xd3b7
    cde4:	7d 01 1c    	tst	0x11c
    cde7:	2a 03       	bpl	0x0xcdec
    cde9:	7e e5 10    	jmp	0xe510
    cdec:	7d 01 1e    	tst	0x11e
    cdef:	26 08       	bne	0x0xcdf9
    cdf1:	7d 01 1c    	tst	0x11c
    cdf4:	27 1f       	beq	0x0xce15
    cdf6:	7e b1 32    	jmp	0xb132
    cdf9:	7d 01 1c    	tst	0x11c
    cdfc:	27 24       	beq	0x0xce22
    cdfe:	cc 01 20    	ldd	#0x120
    ce01:	fb 01 1e    	addb	0x11e
    ce04:	8f          	xgdx
    ce05:	bd e9 0c    	jsr	0xe90c
    ce08:	7a 01 1e    	dec	0x11e
    ce0b:	7a 01 1c    	dec	0x11c
    ce0e:	26 12       	bne	0x0xce22
    ce10:	bd e9 bd    	jsr	0xe9bd
    ce13:	20 0d       	bra	0x0xce22
    ce15:	7d 01 1e    	tst	0x11e
    ce18:	26 08       	bne	0x0xce22
    ce1a:	86 13       	ldaa	#0x13
    ce1c:	b7 01 1e    	staa	0x11e
    ce1f:	bd e9 48    	jsr	0xe948
    ce22:	7d 01 1a    	tst	0x11a
    ce25:	26 09       	bne	0x0xce30
    ce27:	7f 01 7f    	clr	0x17f
    ce2a:	7f 01 7e    	clr	0x17e
    ce2d:	7e d3 b7    	jmp	0xd3b7
    ce30:	7f 01 1a    	clr	0x11a
    ce33:	7f 01 7f    	clr	0x17f
    ce36:	7f 01 7e    	clr	0x17e
    ce39:	7d 00 d0    	tst	0xd0
    ce3c:	27 05       	beq	0x0xce43
    ce3e:	7f 00 d0    	clr	0xd0
    ce41:	20 08       	bra	0x0xce4b
    ce43:	7d 10 29    	tst	0x1029
    ce46:	2a fb       	bpl	0x0xce43
    ce48:	b6 10 2a    	ldaa	0x102a
    ce4b:	5f          	clrb
    ce4c:	01          	nop
    ce4d:	01          	nop
    ce4e:	01          	nop
    ce4f:	01          	nop
    ce50:	f7 10 42    	stab	0x1042
    ce53:	01          	nop
    ce54:	01          	nop
    ce55:	01          	nop
    ce56:	01          	nop
    ce57:	01          	nop
    ce58:	01          	nop
    ce59:	01          	nop
    ce5a:	86 fe       	ldaa	#0xfe
    ce5c:	b7 10 2a    	staa	0x102a
    ce5f:	bd e9 6d    	jsr	0xe96d
    ce62:	c6 20       	ldab	#0x20
    ce64:	ce 01 20    	ldx	#0x120
    ce67:	18 ce ce c7 	ldy	#0xcec7
    ce6b:	18 a6 00    	ldaa	0x0,y
    ce6e:	a7 00       	staa	0x0,x
    ce70:	08          	inx
    ce71:	18 08       	iny
    ce73:	5a          	decb
    ce74:	26 f5       	bne	0x0xce6b
    ce76:	7f 01 1e    	clr	0x11e
    ce79:	86 20       	ldaa	#0x20
    ce7b:	b7 01 1c    	staa	0x11c
    ce7e:	ce 10 23    	ldx	#0x1023
    ce81:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xce81
    ce85:	bd e8 c1    	jsr	0xe8c1
    ce88:	7d 01 1c    	tst	0x11c
    ce8b:	27 05       	beq	0x0xce92
    ce8d:	ce 10 23    	ldx	#0x1023
    ce90:	20 ef       	bra	0x0xce81
    ce92:	86 ff       	ldaa	#0xff
    ce94:	97 f8       	staa	*0xf8
    ce96:	c6 06       	ldab	#0x6
    ce98:	ce ff ff    	ldx	#0xffff
    ce9b:	01          	nop
    ce9c:	01          	nop
    ce9d:	01          	nop
    ce9e:	01          	nop
    ce9f:	09          	dex
    cea0:	26 f9       	bne	0x0xce9b
    cea2:	5a          	decb
    cea3:	26 f6       	bne	0x0xce9b
    cea5:	c6 06       	ldab	#0x6
    cea7:	44          	lsra
    cea8:	27 04       	beq	0x0xceae
    ceaa:	97 f8       	staa	*0xf8
    ceac:	20 ed       	bra	0x0xce9b
    ceae:	97 f8       	staa	*0xf8
    ceb0:	97 fb       	staa	*0xfb
    ceb2:	97 f2       	staa	*0xf2
    ceb4:	97 ff       	staa	*0xff
    ceb6:	b7 01 1b    	staa	0x11b
    ceb9:	bd a0 ff    	jsr	0xa0ff
    cebc:	bd a1 16    	jsr	0xa116
    cebf:	86 80       	ldaa	#0x80
    cec1:	b7 01 1c    	staa	0x11c
    cec4:	7e 82 93    	jmp	0x8293
    cec7:	20 54       	bra	0x0xcf1d
    cec9:	55          	.byte	0x55
    ceca:	4e          	.byte	0x4e
    cecb:	49          	rola
    cecc:	4e          	.byte	0x4e
    cecd:	47          	asra
    cece:	20 2e       	bra	0x0xcefe
    ced0:	2e 2e       	bgt	0x0xcf00
    ced2:	2e 2e       	bgt	0x0xcf02
    ced4:	20 20       	bra	0x0xcef6
    ced6:	20 20       	bra	0x0xcef8
    ced8:	20 20       	bra	0x0xcefa
    ceda:	20 20       	bra	0x0xcefc
    cedc:	20 20       	bra	0x0xcefe
    cede:	20 20       	bra	0x0xcf00
    cee0:	20 20       	bra	0x0xcf02
    cee2:	20 20       	bra	0x0xcf04
    cee4:	20 20       	bra	0x0xcf06
    cee6:	20 7e       	bra	0x0xcf66
    cee8:	d3 b7       	addd	*0xb7
    ceea:	7d 01 1c    	tst	0x11c
    ceed:	2a 03       	bpl	0x0xcef2
    ceef:	7e df e4    	jmp	0xdfe4
    cef2:	7d 01 1e    	tst	0x11e
    cef5:	26 08       	bne	0x0xceff
    cef7:	7d 01 1c    	tst	0x11c
    cefa:	27 1f       	beq	0x0xcf1b
    cefc:	7e b1 32    	jmp	0xb132
    ceff:	7d 01 1c    	tst	0x11c
    cf02:	27 24       	beq	0x0xcf28
    cf04:	cc 01 20    	ldd	#0x120
    cf07:	fb 01 1e    	addb	0x11e
    cf0a:	8f          	xgdx
    cf0b:	bd e9 0c    	jsr	0xe90c
    cf0e:	7a 01 1e    	dec	0x11e
    cf11:	7a 01 1c    	dec	0x11c
    cf14:	26 12       	bne	0x0xcf28
    cf16:	bd e9 8a    	jsr	0xe98a
    cf19:	20 0d       	bra	0x0xcf28
    cf1b:	7d 01 1e    	tst	0x11e
    cf1e:	26 08       	bne	0x0xcf28
    cf20:	86 13       	ldaa	#0x13
    cf22:	b7 01 1e    	staa	0x11e
    cf25:	bd e9 48    	jsr	0xe948
    cf28:	7d 01 7f    	tst	0x17f
    cf2b:	27 1e       	beq	0x0xcf4b
    cf2d:	2a 10       	bpl	0x0xcf3f
    cf2f:	bd eb 4c    	jsr	0xeb4c
    cf32:	4f          	clra
    cf33:	b7 01 7f    	staa	0x17f
    cf36:	b7 01 7e    	staa	0x17e
    cf39:	b7 01 1a    	staa	0x11a
    cf3c:	7e d3 b7    	jmp	0xd3b7
    cf3f:	b6 01 1e    	ldaa	0x11e
    cf42:	81 19       	cmpa	#0x19
    cf44:	27 ec       	beq	0x0xcf32
    cf46:	bd eb 4c    	jsr	0xeb4c
    cf49:	20 e7       	bra	0x0xcf32
    cf4b:	7d 01 1a    	tst	0x11a
    cf4e:	26 03       	bne	0x0xcf53
    cf50:	7e d3 b7    	jmp	0xd3b7
    cf53:	b6 01 1e    	ldaa	0x11e
    cf56:	81 13       	cmpa	#0x13
    cf58:	26 2b       	bne	0x0xcf85
    cf5a:	b6 01 1a    	ldaa	0x11a
    cf5d:	2b 1f       	bmi	0x0xcf7e
    cf5f:	96 50       	ldaa	*0x50
    cf61:	4c          	inca
    cf62:	84 7f       	anda	#0x7f
    cf64:	97 50       	staa	*0x50
    cf66:	ce 01 31    	ldx	#0x131
    cf69:	bd ea 20    	jsr	0xea20
    cf6c:	7f 01 1a    	clr	0x11a
    cf6f:	86 03       	ldaa	#0x3
    cf71:	b7 01 1c    	staa	0x11c
    cf74:	96 50       	ldaa	*0x50
    cf76:	c6 50       	ldab	#0x50
    cf78:	bd af d2    	jsr	0xafd2
    cf7b:	7e d3 b7    	jmp	0xd3b7
    cf7e:	96 50       	ldaa	*0x50
    cf80:	4a          	deca
    cf81:	84 7f       	anda	#0x7f
    cf83:	20 df       	bra	0x0xcf64
    cf85:	81 19       	cmpa	#0x19
    cf87:	26 2b       	bne	0x0xcfb4
    cf89:	b6 01 1a    	ldaa	0x11a
    cf8c:	2b 1f       	bmi	0x0xcfad
    cf8e:	96 98       	ldaa	*0x98
    cf90:	4c          	inca
    cf91:	84 7f       	anda	#0x7f
    cf93:	97 98       	staa	*0x98
    cf95:	ce 01 37    	ldx	#0x137
    cf98:	bd ea 20    	jsr	0xea20
    cf9b:	7f 01 1a    	clr	0x11a
    cf9e:	86 03       	ldaa	#0x3
    cfa0:	b7 01 1c    	staa	0x11c
    cfa3:	96 98       	ldaa	*0x98
    cfa5:	c6 98       	ldab	#0x98
    cfa7:	bd af d2    	jsr	0xafd2
    cfaa:	7e d3 b7    	jmp	0xd3b7
    cfad:	96 98       	ldaa	*0x98
    cfaf:	4a          	deca
    cfb0:	84 7f       	anda	#0x7f
    cfb2:	20 df       	bra	0x0xcf93
    cfb4:	7f 01 1a    	clr	0x11a
    cfb7:	7e d3 b7    	jmp	0xd3b7
    cfba:	7d 01 1c    	tst	0x11c
    cfbd:	2a 03       	bpl	0x0xcfc2
    cfbf:	7e e6 71    	jmp	0xe671
    cfc2:	7d 01 1e    	tst	0x11e
    cfc5:	26 08       	bne	0x0xcfcf
    cfc7:	7d 01 1c    	tst	0x11c
    cfca:	27 1f       	beq	0x0xcfeb
    cfcc:	7e b1 32    	jmp	0xb132
    cfcf:	7d 01 1c    	tst	0x11c
    cfd2:	27 1f       	beq	0x0xcff3
    cfd4:	cc 01 20    	ldd	#0x120
    cfd7:	fb 01 1e    	addb	0x11e
    cfda:	8f          	xgdx
    cfdb:	bd e9 0c    	jsr	0xe90c
    cfde:	7a 01 1e    	dec	0x11e
    cfe1:	7a 01 1c    	dec	0x11c
    cfe4:	26 0d       	bne	0x0xcff3
    cfe6:	bd e9 bd    	jsr	0xe9bd
    cfe9:	20 08       	bra	0x0xcff3
    cfeb:	86 16       	ldaa	#0x16
    cfed:	b7 01 1e    	staa	0x11e
    cff0:	bd e9 48    	jsr	0xe948
    cff3:	7f 01 7e    	clr	0x17e
    cff6:	7d 01 7f    	tst	0x17f
    cff9:	27 10       	beq	0x0xd00b
    cffb:	bd eb be    	jsr	0xebbe
    cffe:	4f          	clra
    cfff:	b7 01 7f    	staa	0x17f
    d002:	b7 01 7e    	staa	0x17e
    d005:	b7 01 1a    	staa	0x11a
    d008:	7e d3 b7    	jmp	0xd3b7
    d00b:	7d 01 1a    	tst	0x11a
    d00e:	26 03       	bne	0x0xd013
    d010:	7e d3 b7    	jmp	0xd3b7
    d013:	ce 50 01    	ldx	#0x5001
    d016:	b6 01 6b    	ldaa	0x16b
    d019:	c6 04       	ldab	#0x4
    d01b:	3d          	mul
    d01c:	3a          	abx
    d01d:	b6 01 1e    	ldaa	0x11e
    d020:	81 12       	cmpa	#0x12
    d022:	26 69       	bne	0x0xd08d
    d024:	a6 00       	ldaa	0x0,x
    d026:	7d 01 1a    	tst	0x11a
    d029:	2b 5c       	bmi	0x0xd087
    d02b:	4c          	inca
    d02c:	81 04       	cmpa	#0x4
    d02e:	25 02       	bcs	0x0xd032
    d030:	86 03       	ldaa	#0x3
    d032:	a7 00       	staa	0x0,x
    d034:	8b 41       	adda	#0x41
    d036:	b7 01 32    	staa	0x132
    d039:	7f 01 1a    	clr	0x11a
    d03c:	86 01       	ldaa	#0x1
    d03e:	b7 01 1c    	staa	0x11c
    d041:	a6 01       	ldaa	0x1,x
    d043:	e6 00       	ldab	0x0,x
    d045:	c1 01       	cmpb	#0x1
    d047:	23 18       	bls	0x0xd061
    d049:	d7 f9       	stab	*0xf9
    d04b:	bd ab 71    	jsr	0xab71
    d04e:	c6 04       	ldab	#0x4
    d050:	d7 f9       	stab	*0xf9
    d052:	bd a2 d0    	jsr	0xa2d0
    d055:	bd a2 eb    	jsr	0xa2eb
    d058:	bd ab 71    	jsr	0xab71
    d05b:	bd a5 c2    	jsr	0xa5c2
    d05e:	7e d1 62    	jmp	0xd162
    d061:	4f          	clra
    d062:	b7 10 22    	staa	0x1022
    d065:	b6 10 2d    	ldaa	0x102d
    d068:	36          	psha
    d069:	84 7f       	anda	#0x7f
    d06b:	b7 10 2d    	staa	0x102d
    d06e:	a6 00       	ldaa	0x0,x
    d070:	e6 01       	ldab	0x1,x
    d072:	bd 79 00    	jsr	0x7900
    d075:	32          	pula
    d076:	b7 10 2d    	staa	0x102d
    d079:	86 80       	ldaa	#0x80
    d07b:	b7 10 22    	staa	0x1022
    d07e:	bd a2 eb    	jsr	0xa2eb
    d081:	bd a5 c2    	jsr	0xa5c2
    d084:	7e d1 62    	jmp	0xd162
    d087:	4a          	deca
    d088:	2a a8       	bpl	0x0xd032
    d08a:	4f          	clra
    d08b:	20 a5       	bra	0x0xd032
    d08d:	81 16       	cmpa	#0x16
    d08f:	26 55       	bne	0x0xd0e6
    d091:	a6 01       	ldaa	0x1,x
    d093:	7d 01 1a    	tst	0x11a
    d096:	2b 3c       	bmi	0x0xd0d4
    d098:	4c          	inca
    d099:	84 7f       	anda	#0x7f
    d09b:	81 51       	cmpa	#0x51
    d09d:	26 04       	bne	0x0xd0a3
    d09f:	86 53       	ldaa	#0x53
    d0a1:	20 06       	bra	0x0xd0a9
    d0a3:	81 52       	cmpa	#0x52
    d0a5:	26 02       	bne	0x0xd0a9
    d0a7:	86 53       	ldaa	#0x53
    d0a9:	a7 01       	staa	0x1,x
    d0ab:	4c          	inca
    d0ac:	3c          	pshx
    d0ad:	ce 01 34    	ldx	#0x134
    d0b0:	bd ea 20    	jsr	0xea20
    d0b3:	38          	pulx
    d0b4:	a6 01       	ldaa	0x1,x
    d0b6:	e6 00       	ldab	0x0,x
    d0b8:	c1 01       	cmpb	#0x1
    d0ba:	23 a5       	bls	0x0xd061
    d0bc:	d7 f9       	stab	*0xf9
    d0be:	bd ab 71    	jsr	0xab71
    d0c1:	c6 04       	ldab	#0x4
    d0c3:	d7 f9       	stab	*0xf9
    d0c5:	bd a2 d0    	jsr	0xa2d0
    d0c8:	bd a2 eb    	jsr	0xa2eb
    d0cb:	bd ab 71    	jsr	0xab71
    d0ce:	bd a5 c2    	jsr	0xa5c2
    d0d1:	7e d1 62    	jmp	0xd162
    d0d4:	4a          	deca
    d0d5:	84 7f       	anda	#0x7f
    d0d7:	81 52       	cmpa	#0x52
    d0d9:	26 04       	bne	0x0xd0df
    d0db:	86 50       	ldaa	#0x50
    d0dd:	20 ca       	bra	0x0xd0a9
    d0df:	81 51       	cmpa	#0x51
    d0e1:	26 c6       	bne	0x0xd0a9
    d0e3:	4a          	deca
    d0e4:	20 c3       	bra	0x0xd0a9
    d0e6:	81 1a       	cmpa	#0x1a
    d0e8:	26 3e       	bne	0x0xd128
    d0ea:	a6 02       	ldaa	0x2,x
    d0ec:	84 40       	anda	#0x40
    d0ee:	b7 01 7d    	staa	0x17d
    d0f1:	a6 02       	ldaa	0x2,x
    d0f3:	84 3f       	anda	#0x3f
    d0f5:	f6 50 00    	ldab	0x5000
    d0f8:	c1 06       	cmpb	#0x6
    d0fa:	27 02       	beq	0x0xd0fe
    d0fc:	20 64       	bra	0x0xd162
    d0fe:	7d 01 1a    	tst	0x11a
    d101:	2b 19       	bmi	0x0xd11c
    d103:	4c          	inca
    d104:	7a 50 23    	dec	0x5023
    d107:	2a 04       	bpl	0x0xd10d
    d109:	4a          	deca
    d10a:	7c 50 23    	inc	0x5023
    d10d:	ba 01 7d    	oraa	0x17d
    d110:	a7 02       	staa	0x2,x
    d112:	84 3f       	anda	#0x3f
    d114:	ce 01 38    	ldx	#0x138
    d117:	bd ea 20    	jsr	0xea20
    d11a:	20 46       	bra	0x0xd162
    d11c:	7c 50 23    	inc	0x5023
    d11f:	4a          	deca
    d120:	2a eb       	bpl	0x0xd10d
    d122:	4c          	inca
    d123:	7a 50 23    	dec	0x5023
    d126:	20 e5       	bra	0x0xd10d
    d128:	f6 50 00    	ldab	0x5000
    d12b:	c1 06       	cmpb	#0x6
    d12d:	27 22       	beq	0x0xd151
    d12f:	a6 03       	ldaa	0x3,x
    d131:	7d 01 1a    	tst	0x11a
    d134:	2b 18       	bmi	0x0xd14e
    d136:	4c          	inca
    d137:	84 7f       	anda	#0x7f
    d139:	a7 03       	staa	0x3,x
    d13b:	36          	psha
    d13c:	ce 01 3c    	ldx	#0x13c
    d13f:	bd ea 20    	jsr	0xea20
    d142:	32          	pula
    d143:	d6 d6       	ldab	*0xd6
    d145:	3d          	mul
    d146:	05          	asld
    d147:	c6 ac       	ldab	#0xac
    d149:	bd af d2    	jsr	0xafd2
    d14c:	20 14       	bra	0x0xd162
    d14e:	4a          	deca
    d14f:	20 e6       	bra	0x0xd137
    d151:	e6 03       	ldab	0x3,x
    d153:	5c          	incb
    d154:	c4 01       	andb	#0x1
    d156:	e7 03       	stab	0x3,x
    d158:	ce e7 9b    	ldx	#0xe79b
    d15b:	18 ce 01 3c 	ldy	#0x13c
    d15f:	bd ea bf    	jsr	0xeabf
    d162:	7f 01 1a    	clr	0x11a
    d165:	86 03       	ldaa	#0x3
    d167:	b7 01 1c    	staa	0x11c
    d16a:	14 fb 80    	bset	*0xfb, #0x80
    d16d:	7e d3 b7    	jmp	0xd3b7
    d170:	7d 01 1c    	tst	0x11c
    d173:	2a 03       	bpl	0x0xd178
    d175:	7e e7 a1    	jmp	0xe7a1
    d178:	7d 01 1e    	tst	0x11e
    d17b:	26 08       	bne	0x0xd185
    d17d:	7d 01 1c    	tst	0x11c
    d180:	27 1f       	beq	0x0xd1a1
    d182:	7e b1 32    	jmp	0xb132
    d185:	7d 01 1c    	tst	0x11c
    d188:	27 1f       	beq	0x0xd1a9
    d18a:	cc 01 20    	ldd	#0x120
    d18d:	fb 01 1e    	addb	0x11e
    d190:	8f          	xgdx
    d191:	bd e9 0c    	jsr	0xe90c
    d194:	7a 01 1e    	dec	0x11e
    d197:	7a 01 1c    	dec	0x11c
    d19a:	26 0d       	bne	0x0xd1a9
    d19c:	bd e9 8a    	jsr	0xe98a
    d19f:	20 08       	bra	0x0xd1a9
    d1a1:	86 13       	ldaa	#0x13
    d1a3:	b7 01 1e    	staa	0x11e
    d1a6:	bd e9 48    	jsr	0xe948
    d1a9:	7d 01 7f    	tst	0x17f
    d1ac:	27 0d       	beq	0x0xd1bb
    d1ae:	4f          	clra
    d1af:	b7 01 7f    	staa	0x17f
    d1b2:	b7 01 7e    	staa	0x17e
    d1b5:	b7 01 1a    	staa	0x11a
    d1b8:	7e d3 b7    	jmp	0xd3b7
    d1bb:	7d 01 7e    	tst	0x17e
    d1be:	27 0c       	beq	0x0xd1cc
    d1c0:	7f 01 7f    	clr	0x17f
    d1c3:	7f 01 7e    	clr	0x17e
    d1c6:	7f 01 1a    	clr	0x11a
    d1c9:	7e d3 b7    	jmp	0xd3b7
    d1cc:	7d 01 1a    	tst	0x11a
    d1cf:	26 03       	bne	0x0xd1d4
    d1d1:	7e d3 b7    	jmp	0xd3b7
    d1d4:	b6 01 6b    	ldaa	0x16b
    d1d7:	c6 04       	ldab	#0x4
    d1d9:	3d          	mul
    d1da:	ce 50 03    	ldx	#0x5003
    d1dd:	3a          	abx
    d1de:	e6 00       	ldab	0x0,x
    d1e0:	c4 3f       	andb	#0x3f
    d1e2:	f7 01 7d    	stab	0x17d
    d1e5:	e6 00       	ldab	0x0,x
    d1e7:	c4 40       	andb	#0x40
    d1e9:	c8 40       	eorb	#0x40
    d1eb:	37          	pshb
    d1ec:	fa 01 7d    	orab	0x17d
    d1ef:	e7 00       	stab	0x0,x
    d1f1:	32          	pula
    d1f2:	4d          	tsta
    d1f3:	27 02       	beq	0x0xd1f7
    d1f5:	86 01       	ldaa	#0x1
    d1f7:	36          	psha
    d1f8:	4d          	tsta
    d1f9:	26 31       	bne	0x0xd22c
    d1fb:	7d 00 d0    	tst	0xd0
    d1fe:	27 05       	beq	0x0xd205
    d200:	7f 00 d0    	clr	0xd0
    d203:	20 08       	bra	0x0xd20d
    d205:	7d 10 29    	tst	0x1029
    d208:	2a fb       	bpl	0x0xd205
    d20a:	f6 10 2a    	ldab	0x102a
    d20d:	f6 01 6b    	ldab	0x16b
    d210:	ce 50 50    	ldx	#0x5050
    d213:	3a          	abx
    d214:	e6 00       	ldab	0x0,x
    d216:	53          	comb
    d217:	01          	nop
    d218:	01          	nop
    d219:	01          	nop
    d21a:	01          	nop
    d21b:	f7 10 42    	stab	0x1042
    d21e:	01          	nop
    d21f:	01          	nop
    d220:	01          	nop
    d221:	01          	nop
    d222:	01          	nop
    d223:	01          	nop
    d224:	01          	nop
    d225:	c6 8c       	ldab	#0x8c
    d227:	f7 10 2a    	stab	0x102a
    d22a:	20 07       	bra	0x0xd233
    d22c:	c6 44       	ldab	#0x44
    d22e:	96 44       	ldaa	*0x44
    d230:	bd af d2    	jsr	0xafd2
    d233:	33          	pulb
    d234:	18 ce 01 31 	ldy	#0x131
    d238:	ce e8 12    	ldx	#0xe812
    d23b:	bd ea bf    	jsr	0xeabf
    d23e:	7f 01 1a    	clr	0x11a
    d241:	86 03       	ldaa	#0x3
    d243:	b7 01 1c    	staa	0x11c
    d246:	14 fb 80    	bset	*0xfb, #0x80
    d249:	7e d3 b7    	jmp	0xd3b7
    d24c:	7d 01 1c    	tst	0x11c
    d24f:	2a 03       	bpl	0x0xd254
    d251:	7e e8 59    	jmp	0xe859
    d254:	7d 01 1e    	tst	0x11e
    d257:	26 08       	bne	0x0xd261
    d259:	7d 01 1c    	tst	0x11c
    d25c:	27 24       	beq	0x0xd282
    d25e:	7e b1 32    	jmp	0xb132
    d261:	7d 01 1c    	tst	0x11c
    d264:	27 24       	beq	0x0xd28a
    d266:	cc 01 20    	ldd	#0x120
    d269:	fb 01 1e    	addb	0x11e
    d26c:	8f          	xgdx
    d26d:	bd e9 0c    	jsr	0xe90c
    d270:	7a 01 1e    	dec	0x11e
    d273:	7a 01 1c    	dec	0x11c
    d276:	26 12       	bne	0x0xd28a
    d278:	86 1e       	ldaa	#0x1e
    d27a:	b7 01 1e    	staa	0x11e
    d27d:	bd e9 48    	jsr	0xe948
    d280:	20 08       	bra	0x0xd28a
    d282:	86 1e       	ldaa	#0x1e
    d284:	b7 01 1e    	staa	0x11e
    d287:	bd e9 48    	jsr	0xe948
    d28a:	7d 01 1a    	tst	0x11a
    d28d:	26 0a       	bne	0x0xd299
    d28f:	4f          	clra
    d290:	b7 01 7f    	staa	0x17f
    d293:	b7 01 7e    	staa	0x17e
    d296:	7e d3 b7    	jmp	0xd3b7
    d299:	86 0c       	ldaa	#0xc
    d29b:	b7 10 2d    	staa	0x102d
    d29e:	4f          	clra
    d29f:	97 f2       	staa	*0xf2
    d2a1:	97 f3       	staa	*0xf3
    d2a3:	97 f4       	staa	*0xf4
    d2a5:	97 f5       	staa	*0xf5
    d2a7:	97 f6       	staa	*0xf6
    d2a9:	97 f7       	staa	*0xf7
    d2ab:	97 f8       	staa	*0xf8
    d2ad:	bd d3 61    	jsr	0xd361
    d2b0:	4f          	clra
    d2b1:	b7 10 22    	staa	0x1022
    d2b4:	96 99       	ldaa	*0x99
    d2b6:	80 41       	suba	#0x41
    d2b8:	97 f9       	staa	*0xf9
    d2ba:	bd ab 71    	jsr	0xab71
    d2bd:	96 9a       	ldaa	*0x9a
    d2bf:	80 41       	suba	#0x41
    d2c1:	36          	psha
    d2c2:	ce f9 c1    	ldx	#0xf9c1
    d2c5:	18 ce 78 00 	ldy	#0x7800
    d2c9:	a6 00       	ldaa	0x0,x
    d2cb:	18 a7 00    	staa	0x0,y
    d2ce:	18 08       	iny
    d2d0:	08          	inx
    d2d1:	8c fb bf    	cpx	#0xfbbf
    d2d4:	23 f3       	bls	0x0xd2c9
    d2d6:	18 ce 00 00 	ldy	#0x0
    d2da:	ce d2 ee    	ldx	#0xd2ee
    d2dd:	a6 00       	ldaa	0x0,x
    d2df:	18 a7 00    	staa	0x0,y
    d2e2:	18 08       	iny
    d2e4:	08          	inx
    d2e5:	8c d3 60    	cpx	#0xd360
    d2e8:	25 f3       	bcs	0x0xd2dd
    d2ea:	0f          	sei
    d2eb:	7e 00 00    	jmp	0x0
    d2ee:	32          	pula
    d2ef:	4d          	tsta
    d2f0:	26 0c       	bne	0x0xd2fe
    d2f2:	b6 10 00    	ldaa	0x1000
    d2f5:	84 9f       	anda	#0x9f
    d2f7:	8a 20       	oraa	#0x20
    d2f9:	b7 10 00    	staa	0x1000
    d2fc:	20 0a       	bra	0x0xd308
    d2fe:	b6 10 00    	ldaa	0x1000
    d301:	84 9f       	anda	#0x9f
    d303:	8a 40       	oraa	#0x40
    d305:	b7 10 00    	staa	0x1000
    d308:	ce 20 00    	ldx	#0x2000
    d30b:	18 ce 80 00 	ldy	#0x8000
    d30f:	86 aa       	ldaa	#0xaa
    d311:	b7 d5 55    	staa	0xd555
    d314:	86 55       	ldaa	#0x55
    d316:	b7 aa aa    	staa	0xaaaa
    d319:	86 a0       	ldaa	#0xa0
    d31b:	b7 d5 55    	staa	0xd555
    d31e:	86 80       	ldaa	#0x80
    d320:	e6 00       	ldab	0x0,x
    d322:	18 e7 00    	stab	0x0,y
    d325:	08          	inx
    d326:	18 08       	iny
    d328:	4a          	deca
    d329:	26 f5       	bne	0x0xd320
    d32b:	86 08       	ldaa	#0x8
    d32d:	b7 10 23    	staa	0x1023
    d330:	fc 10 0e    	ldd	0x100e
    d333:	c3 5d c0    	addd	#0x5dc0
    d336:	fd 10 1e    	std	0x101e
    d339:	01          	nop
    d33a:	01          	nop
    d33b:	b6 10 23    	ldaa	0x1023
    d33e:	85 08       	bita	#0x8
    d340:	27 f7       	beq	0x0xd339
    d342:	8c 78 00    	cpx	#0x7800
    d345:	25 c8       	bcs	0x0xd30f
    d347:	22 06       	bhi	0x0xd34f
    d349:	18 ce fe 00 	ldy	#0xfe00
    d34d:	20 c0       	bra	0x0xd30f
    d34f:	18 8c 00 00 	cpy	#0x0
    d353:	26 ba       	bne	0x0xd30f
    d355:	b6 10 00    	ldaa	0x1000
    d358:	84 8f       	anda	#0x8f
    d35a:	b7 10 00    	staa	0x1000
    d35d:	7e 80 00    	jmp	0x8000
    d360:	01          	nop
    d361:	ce 10 23    	ldx	#0x1023
    d364:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd364
    d368:	bd e9 6d    	jsr	0xe96d
    d36b:	ce 01 20    	ldx	#0x120
    d36e:	18 ce d3 97 	ldy	#0xd397
    d372:	c6 20       	ldab	#0x20
    d374:	18 a6 00    	ldaa	0x0,y
    d377:	a7 00       	staa	0x0,x
    d379:	08          	inx
    d37a:	18 08       	iny
    d37c:	5a          	decb
    d37d:	26 f5       	bne	0x0xd374
    d37f:	7f 01 1e    	clr	0x11e
    d382:	86 20       	ldaa	#0x20
    d384:	b7 01 1c    	staa	0x11c
    d387:	ce 10 23    	ldx	#0x1023
    d38a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd38a
    d38e:	bd e8 c1    	jsr	0xe8c1
    d391:	7d 01 1c    	tst	0x11c
    d394:	26 f1       	bne	0x0xd387
    d396:	39          	rts
    d397:	20 57       	bra	0x0xd3f0
    d399:	52          	.byte	0x52
    d39a:	49          	rola
    d39b:	54          	lsrb
    d39c:	49          	rola
    d39d:	4e          	.byte	0x4e
    d39e:	47          	asra
    d39f:	20 42       	bra	0x0xd3e3
    d3a1:	41          	.byte	0x41
    d3a2:	4e          	.byte	0x4e
    d3a3:	4b          	.byte	0x4b
    d3a4:	20 2e       	bra	0x0xd3d4
    d3a6:	2e 20       	bgt	0x0xd3c8
    d3a8:	54          	lsrb
    d3a9:	41          	.byte	0x41
    d3aa:	4b          	.byte	0x4b
    d3ab:	45          	.byte	0x45
    d3ac:	20 35       	bra	0x0xd3e3
    d3ae:	20 2e       	bra	0x0xd3de
    d3b0:	2e 2e       	bgt	0x0xd3e0
    d3b2:	2e 2e       	bgt	0x0xd3e2
    d3b4:	2e 2e       	bgt	0x0xd3e4
    d3b6:	2e fe       	bgt	0x0xd3b6
    d3b8:	01          	nop
    d3b9:	c0 bc       	subb	#0xbc
    d3bb:	01          	nop
    d3bc:	c2 26       	sbcb	#0x26
    d3be:	03          	fdiv
    d3bf:	7e 95 68    	jmp	0x9568
    d3c2:	18 fe 01 6c 	ldy	0x16c
    d3c6:	18 3c       	pshy
    d3c8:	7e 91 d5    	jmp	0x91d5
    d3cb:	7f 00 ff    	clr	0xff
    d3ce:	7d 01 1d    	tst	0x11d
    d3d1:	27 2f       	beq	0x0xd402
    d3d3:	ce 10 23    	ldx	#0x1023
    d3d6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd3d6
    d3da:	86 f7       	ldaa	#0xf7
    d3dc:	b4 10 00    	anda	0x1000
    d3df:	b7 10 00    	staa	0x1000
    d3e2:	86 0c       	ldaa	#0xc
    d3e4:	b7 10 47    	staa	0x1047
    d3e7:	86 80       	ldaa	#0x80
    d3e9:	ba 10 00    	oraa	0x1000
    d3ec:	b7 10 00    	staa	0x1000
    d3ef:	01          	nop
    d3f0:	88 80       	eora	#0x80
    d3f2:	b7 10 00    	staa	0x1000
    d3f5:	7f 01 1d    	clr	0x11d
    d3f8:	bd e9 39    	jsr	0xe939
    d3fb:	ce 10 23    	ldx	#0x1023
    d3fe:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd3fe
    d402:	bd e9 6d    	jsr	0xe96d
    d405:	ce 01 20    	ldx	#0x120
    d408:	c6 10       	ldab	#0x10
    d40a:	96 f9       	ldaa	*0xf9
    d40c:	81 04       	cmpa	#0x4
    d40e:	25 06       	bcs	0x0xd416
    d410:	18 ce d4 85 	ldy	#0xd485
    d414:	20 07       	bra	0x0xd41d
    d416:	14 d1 80    	bset	*0xd1, #0x80
    d419:	18 ce d4 75 	ldy	#0xd475
    d41d:	18 a6 00    	ldaa	0x0,y
    d420:	a7 00       	staa	0x0,x
    d422:	08          	inx
    d423:	18 08       	iny
    d425:	5a          	decb
    d426:	26 f5       	bne	0x0xd41d
    d428:	96 f9       	ldaa	*0xf9
    d42a:	81 04       	cmpa	#0x4
    d42c:	27 30       	beq	0x0xd45e
    d42e:	8b 41       	adda	#0x41
    d430:	b7 01 2c    	staa	0x12c
    d433:	96 f9       	ldaa	*0xf9
    d435:	81 01       	cmpa	#0x1
    d437:	23 05       	bls	0x0xd43e
    d439:	86 41       	ldaa	#0x41
    d43b:	b7 01 28    	staa	0x128
    d43e:	96 fa       	ldaa	*0xfa
    d440:	4c          	inca
    d441:	ce 01 2d    	ldx	#0x12d
    d444:	bd ea 20    	jsr	0xea20
    d447:	08          	inx
    d448:	18 ce 00 c0 	ldy	#0xc0
    d44c:	c6 10       	ldab	#0x10
    d44e:	18 a6 00    	ldaa	0x0,y
    d451:	84 7f       	anda	#0x7f
    d453:	a7 00       	staa	0x0,x
    d455:	08          	inx
    d456:	18 08       	iny
    d458:	5a          	decb
    d459:	26 f3       	bne	0x0xd44e
    d45b:	7e e8 aa    	jmp	0xe8aa
    d45e:	b6 01 6a    	ldaa	0x16a
    d461:	4c          	inca
    d462:	ce 01 2d    	ldx	#0x12d
    d465:	bd ea 20    	jsr	0xea20
    d468:	08          	inx
    d469:	cc 50 00    	ldd	#0x5000
    d46c:	c3 00 40    	addd	#0x40
    d46f:	18 8f       	xgdy
    d471:	c6 10       	ldab	#0x10
    d473:	20 d9       	bra	0x0xd44e
    d475:	50          	negb
    d476:	41          	.byte	0x41
    d477:	54          	lsrb
    d478:	43          	coma
    d479:	48          	asla
    d47a:	20 20       	bra	0x0xd49c
    d47c:	52          	.byte	0x52
    d47d:	4f          	clra
    d47e:	4d          	tsta
    d47f:	20 20       	bra	0x0xd4a1
    d481:	20 20       	bra	0x0xd4a3
    d483:	20 20       	bra	0x0xd4a5
    d485:	4d          	tsta
    d486:	55          	.byte	0x55
    d487:	4c          	inca
    d488:	54          	lsrb
    d489:	49          	rola
    d48a:	20 20       	bra	0x0xd4ac
    d48c:	20 20       	bra	0x0xd4ae
    d48e:	20 20       	bra	0x0xd4b0
    d490:	20 20       	bra	0x0xd4b2
    d492:	20 20       	bra	0x0xd4b4
    d494:	20 7d       	bra	0x0xd513
    d496:	01          	nop
    d497:	7e 27 61    	jmp	0x2761
    d49a:	7f 01 7e    	clr	0x17e
    d49d:	96 f9       	ldaa	*0xf9
    d49f:	81 04       	cmpa	#0x4
    d4a1:	25 1e       	bcs	0x0xd4c1
    d4a3:	8b 41       	adda	#0x41
    d4a5:	b7 01 2c    	staa	0x12c
    d4a8:	b6 01 6a    	ldaa	0x16a
    d4ab:	4c          	inca
    d4ac:	ce 01 2d    	ldx	#0x12d
    d4af:	bd ea 20    	jsr	0xea20
    d4b2:	b6 01 6a    	ldaa	0x16a
    d4b5:	c6 50       	ldab	#0x50
    d4b7:	3d          	mul
    d4b8:	c3 20 00    	addd	#0x2000
    d4bb:	c3 00 40    	addd	#0x40
    d4be:	8f          	xgdx
    d4bf:	20 1a       	bra	0x0xd4db
    d4c1:	8b 41       	adda	#0x41
    d4c3:	b7 01 2c    	staa	0x12c
    d4c6:	96 fa       	ldaa	*0xfa
    d4c8:	4c          	inca
    d4c9:	ce 01 2d    	ldx	#0x12d
    d4cc:	bd ea 20    	jsr	0xea20
    d4cf:	96 fa       	ldaa	*0xfa
    d4d1:	c6 b0       	ldab	#0xb0
    d4d3:	3d          	mul
    d4d4:	c3 20 00    	addd	#0x2000
    d4d7:	c3 00 a0    	addd	#0xa0
    d4da:	8f          	xgdx
    d4db:	18 ce 01 30 	ldy	#0x130
    d4df:	c6 10       	ldab	#0x10
    d4e1:	a6 00       	ldaa	0x0,x
    d4e3:	84 7f       	anda	#0x7f
    d4e5:	18 a7 00    	staa	0x0,y
    d4e8:	08          	inx
    d4e9:	18 08       	iny
    d4eb:	5a          	decb
    d4ec:	26 f3       	bne	0x0xd4e1
    d4ee:	ce 10 23    	ldx	#0x1023
    d4f1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd4f1
    d4f5:	bd e9 6d    	jsr	0xe96d
    d4f8:	7e e8 aa    	jmp	0xe8aa
    d4fb:	7d 01 1d    	tst	0x11d
    d4fe:	26 0a       	bne	0x0xd50a
    d500:	bd ea 97    	jsr	0xea97
    d503:	ce 10 23    	ldx	#0x1023
    d506:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd506
    d50a:	bd e9 6d    	jsr	0xe96d
    d50d:	ce 01 20    	ldx	#0x120
    d510:	18 ce d5 9a 	ldy	#0xd59a
    d514:	c6 20       	ldab	#0x20
    d516:	18 a6 00    	ldaa	0x0,y
    d519:	a7 00       	staa	0x0,x
    d51b:	08          	inx
    d51c:	18 08       	iny
    d51e:	5a          	decb
    d51f:	26 f5       	bne	0x0xd516
    d521:	96 f9       	ldaa	*0xf9
    d523:	8b 41       	adda	#0x41
    d525:	b7 01 25    	staa	0x125
    d528:	96 f9       	ldaa	*0xf9
    d52a:	81 02       	cmpa	#0x2
    d52c:	24 07       	bcc	0x0xd535
    d52e:	86 02       	ldaa	#0x2
    d530:	97 f9       	staa	*0xf9
    d532:	bd ab 71    	jsr	0xab71
    d535:	8b 41       	adda	#0x41
    d537:	b7 01 2c    	staa	0x12c
    d53a:	96 fa       	ldaa	*0xfa
    d53c:	7d 00 d1    	tst	0xd1
    d53f:	2b 3a       	bmi	0x0xd57b
    d541:	b6 01 6a    	ldaa	0x16a
    d544:	7d 00 fb    	tst	0xfb
    d547:	27 32       	beq	0x0xd57b
    d549:	2b 30       	bmi	0x0xd57b
    d54b:	86 04       	ldaa	#0x4
    d54d:	97 f9       	staa	*0xf9
    d54f:	bd ab 71    	jsr	0xab71
    d552:	ce 50 01    	ldx	#0x5001
    d555:	b6 01 6b    	ldaa	0x16b
    d558:	c6 04       	ldab	#0x4
    d55a:	3d          	mul
    d55b:	3a          	abx
    d55c:	a6 00       	ldaa	0x0,x
    d55e:	8b 41       	adda	#0x41
    d560:	b7 01 25    	staa	0x125
    d563:	a6 01       	ldaa	0x1,x
    d565:	36          	psha
    d566:	a6 00       	ldaa	0x0,x
    d568:	81 02       	cmpa	#0x2
    d56a:	24 02       	bcc	0x0xd56e
    d56c:	86 02       	ldaa	#0x2
    d56e:	97 f9       	staa	*0xf9
    d570:	bd ab 71    	jsr	0xab71
    d573:	8b 41       	adda	#0x41
    d575:	b7 01 2c    	staa	0x12c
    d578:	32          	pula
    d579:	97 fa       	staa	*0xfa
    d57b:	4c          	inca
    d57c:	ce 01 26    	ldx	#0x126
    d57f:	bd ea 20    	jsr	0xea20
    d582:	a7 07       	staa	0x7,x
    d584:	09          	dex
    d585:	a6 00       	ldaa	0x0,x
    d587:	a7 07       	staa	0x7,x
    d589:	09          	dex
    d58a:	a6 00       	ldaa	0x0,x
    d58c:	a7 07       	staa	0x7,x
    d58e:	96 f9       	ldaa	*0xf9
    d590:	81 04       	cmpa	#0x4
    d592:	27 03       	beq	0x0xd597
    d594:	7e d4 cf    	jmp	0xd4cf
    d597:	7e d4 5e    	jmp	0xd45e
    d59a:	53          	comb
    d59b:	41          	.byte	0x41
    d59c:	56          	rorb
    d59d:	45          	.byte	0x45
    d59e:	20 20       	bra	0x0xd5c0
    d5a0:	20 20       	bra	0x0xd5c2
    d5a2:	20 20       	bra	0x0xd5c4
    d5a4:	3e          	wai
    d5a5:	20 20       	bra	0x0xd5c7
    d5a7:	20 20       	bra	0x0xd5c9
    d5a9:	20 20       	bra	0x0xd5cb
    d5ab:	20 20       	bra	0x0xd5cd
    d5ad:	20 20       	bra	0x0xd5cf
    d5af:	20 20       	bra	0x0xd5d1
    d5b1:	20 20       	bra	0x0xd5d3
    d5b3:	20 20       	bra	0x0xd5d5
    d5b5:	20 20       	bra	0x0xd5d7
    d5b7:	20 20       	bra	0x0xd5d9
    d5b9:	20 7d       	bra	0x0xd638
    d5bb:	01          	nop
    d5bc:	1d 26 0a    	bclr	0x26,x, #0x0a
    d5bf:	bd ea 97    	jsr	0xea97
    d5c2:	ce 10 23    	ldx	#0x1023
    d5c5:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd5c5
    d5c9:	bd e9 6d    	jsr	0xe96d
    d5cc:	ce 01 20    	ldx	#0x120
    d5cf:	18 ce d6 0e 	ldy	#0xd60e
    d5d3:	c6 20       	ldab	#0x20
    d5d5:	96 f9       	ldaa	*0xf9
    d5d7:	81 04       	cmpa	#0x4
    d5d9:	26 04       	bne	0x0xd5df
    d5db:	18 ce d6 2e 	ldy	#0xd62e
    d5df:	18 a6 00    	ldaa	0x0,y
    d5e2:	a7 00       	staa	0x0,x
    d5e4:	08          	inx
    d5e5:	18 08       	iny
    d5e7:	5a          	decb
    d5e8:	26 f5       	bne	0x0xd5df
    d5ea:	96 f9       	ldaa	*0xf9
    d5ec:	81 04       	cmpa	#0x4
    d5ee:	27 11       	beq	0x0xd601
    d5f0:	8b 41       	adda	#0x41
    d5f2:	b7 01 2c    	staa	0x12c
    d5f5:	96 fa       	ldaa	*0xfa
    d5f7:	4c          	inca
    d5f8:	ce 01 2d    	ldx	#0x12d
    d5fb:	bd ea 20    	jsr	0xea20
    d5fe:	7e d4 47    	jmp	0xd447
    d601:	b6 01 6a    	ldaa	0x16a
    d604:	4c          	inca
    d605:	ce 01 2d    	ldx	#0x12d
    d608:	bd ea 20    	jsr	0xea20
    d60b:	7e d4 5e    	jmp	0xd45e
    d60e:	4e          	.byte	0x4e
    d60f:	41          	.byte	0x41
    d610:	4d          	tsta
    d611:	45          	.byte	0x45
    d612:	20 50       	bra	0x0xd664
    d614:	41          	.byte	0x41
    d615:	54          	lsrb
    d616:	43          	coma
    d617:	48          	asla
    d618:	20 20       	bra	0x0xd63a
    d61a:	20 20       	bra	0x0xd63c
    d61c:	20 20       	bra	0x0xd63e
    d61e:	20 20       	bra	0x0xd640
    d620:	20 20       	bra	0x0xd642
    d622:	20 20       	bra	0x0xd644
    d624:	20 20       	bra	0x0xd646
    d626:	20 20       	bra	0x0xd648
    d628:	20 20       	bra	0x0xd64a
    d62a:	20 20       	bra	0x0xd64c
    d62c:	20 20       	bra	0x0xd64e
    d62e:	4e          	.byte	0x4e
    d62f:	41          	.byte	0x41
    d630:	4d          	tsta
    d631:	45          	.byte	0x45
    d632:	20 4d       	bra	0x0xd681
    d634:	55          	.byte	0x55
    d635:	4c          	inca
    d636:	54          	lsrb
    d637:	49          	rola
    d638:	20 20       	bra	0x0xd65a
    d63a:	20 20       	bra	0x0xd65c
    d63c:	20 20       	bra	0x0xd65e
    d63e:	20 20       	bra	0x0xd660
    d640:	20 20       	bra	0x0xd662
    d642:	20 20       	bra	0x0xd664
    d644:	20 20       	bra	0x0xd666
    d646:	20 20       	bra	0x0xd668
    d648:	20 20       	bra	0x0xd66a
    d64a:	20 20       	bra	0x0xd66c
    d64c:	20 20       	bra	0x0xd66e
    d64e:	bd e9 6d    	jsr	0xe96d
    d651:	ce 01 20    	ldx	#0x120
    d654:	18 ce d6 8c 	ldy	#0xd68c
    d658:	c6 20       	ldab	#0x20
    d65a:	18 a6 00    	ldaa	0x0,y
    d65d:	a7 00       	staa	0x0,x
    d65f:	08          	inx
    d660:	18 08       	iny
    d662:	5a          	decb
    d663:	26 f5       	bne	0x0xd65a
    d665:	ce d6 ac    	ldx	#0xd6ac
    d668:	f6 01 6f    	ldab	0x16f
    d66b:	18 ce 01 30 	ldy	#0x130
    d66f:	bd ea d3    	jsr	0xead3
    d672:	ce d6 f0    	ldx	#0xd6f0
    d675:	d6 93       	ldab	*0x93
    d677:	18 ce 01 36 	ldy	#0x136
    d67b:	bd ea d3    	jsr	0xead3
    d67e:	ce d7 04    	ldx	#0xd704
    d681:	5f          	clrb
    d682:	18 ce 01 3d 	ldy	#0x13d
    d686:	bd ea bf    	jsr	0xeabf
    d689:	7e e8 aa    	jmp	0xe8aa
    d68c:	43          	coma
    d68d:	48          	asla
    d68e:	41          	.byte	0x41
    d68f:	4e          	.byte	0x4e
    d690:	20 20       	bra	0x0xd6b2
    d692:	54          	lsrb
    d693:	55          	.byte	0x55
    d694:	4e          	.byte	0x4e
    d695:	45          	.byte	0x45
    d696:	20 20       	bra	0x0xd6b8
    d698:	49          	rola
    d699:	4e          	.byte	0x4e
    d69a:	49          	rola
    d69b:	54          	lsrb
    d69c:	20 20       	bra	0x0xd6be
    d69e:	20 20       	bra	0x0xd6c0
    d6a0:	20 20       	bra	0x0xd6c2
    d6a2:	20 20       	bra	0x0xd6c4
    d6a4:	20 20       	bra	0x0xd6c6
    d6a6:	20 20       	bra	0x0xd6c8
    d6a8:	20 20       	bra	0x0xd6ca
    d6aa:	20 20       	bra	0x0xd6cc
    d6ac:	20 20       	bra	0x0xd6ce
    d6ae:	20 31       	bra	0x0xd6e1
    d6b0:	20 20       	bra	0x0xd6d2
    d6b2:	20 32       	bra	0x0xd6e6
    d6b4:	20 20       	bra	0x0xd6d6
    d6b6:	20 33       	bra	0x0xd6eb
    d6b8:	20 20       	bra	0x0xd6da
    d6ba:	20 34       	bra	0x0xd6f0
    d6bc:	20 20       	bra	0x0xd6de
    d6be:	20 35       	bra	0x0xd6f5
    d6c0:	20 20       	bra	0x0xd6e2
    d6c2:	20 36       	bra	0x0xd6fa
    d6c4:	20 20       	bra	0x0xd6e6
    d6c6:	20 37       	bra	0x0xd6ff
    d6c8:	20 20       	bra	0x0xd6ea
    d6ca:	20 38       	bra	0x0xd704
    d6cc:	20 20       	bra	0x0xd6ee
    d6ce:	20 39       	bra	0x0xd709
    d6d0:	20 20       	bra	0x0xd6f2
    d6d2:	31          	ins
    d6d3:	30          	tsx
    d6d4:	20 20       	bra	0x0xd6f6
    d6d6:	31          	ins
    d6d7:	31          	ins
    d6d8:	20 20       	bra	0x0xd6fa
    d6da:	31          	ins
    d6db:	32          	pula
    d6dc:	20 20       	bra	0x0xd6fe
    d6de:	31          	ins
    d6df:	33          	pulb
    d6e0:	20 20       	bra	0x0xd702
    d6e2:	31          	ins
    d6e3:	34          	des
    d6e4:	20 20       	bra	0x0xd706
    d6e6:	31          	ins
    d6e7:	35          	txs
    d6e8:	20 20       	bra	0x0xd70a
    d6ea:	31          	ins
    d6eb:	36          	psha
    d6ec:	4f          	clra
    d6ed:	4d          	tsta
    d6ee:	4e          	.byte	0x4e
    d6ef:	49          	rola
    d6f0:	20 4f       	bra	0x0xd741
    d6f2:	46          	rora
    d6f3:	46          	rora
    d6f4:	20 39       	bra	0x0xd72f
    d6f6:	30          	tsx
    d6f7:	25 20       	bcs	0x0xd719
    d6f9:	39          	rts
    d6fa:	35          	txs
    d6fb:	25 20       	bcs	0x0xd71d
    d6fd:	39          	rts
    d6fe:	38          	pulx
    d6ff:	25 31       	bcs	0x0xd732
    d701:	30          	tsx
    d702:	30          	tsx
    d703:	25 20       	bcs	0x0xd725
    d705:	4e          	.byte	0x4e
    d706:	4f          	clra
    d707:	59          	rolb
    d708:	45          	.byte	0x45
    d709:	53          	comb
    d70a:	bd e9 6d    	jsr	0xe96d
    d70d:	ce 01 20    	ldx	#0x120
    d710:	18 ce d7 46 	ldy	#0xd746
    d714:	c6 20       	ldab	#0x20
    d716:	18 a6 00    	ldaa	0x0,y
    d719:	a7 00       	staa	0x0,x
    d71b:	08          	inx
    d71c:	18 08       	iny
    d71e:	5a          	decb
    d71f:	26 f5       	bne	0x0xd716
    d721:	ce d7 66    	ldx	#0xd766
    d724:	f6 01 70    	ldab	0x170
    d727:	18 ce 01 30 	ldy	#0x130
    d72b:	bd ea d3    	jsr	0xead3
    d72e:	ce d7 72    	ldx	#0xd772
    d731:	d6 94       	ldab	*0x94
    d733:	18 ce 01 36 	ldy	#0x136
    d737:	bd ea d3    	jsr	0xead3
    d73a:	b6 01 71    	ldaa	0x171
    d73d:	ce 01 3c    	ldx	#0x13c
    d740:	bd ea 20    	jsr	0xea20
    d743:	7e e8 aa    	jmp	0xe8aa
    d746:	4d          	tsta
    d747:	50          	negb
    d748:	52          	.byte	0x52
    d749:	4f          	clra
    d74a:	20 20       	bra	0x0xd76c
    d74c:	4b          	.byte	0x4b
    d74d:	4e          	.byte	0x4e
    d74e:	4f          	clra
    d74f:	42          	.byte	0x42
    d750:	20 20       	bra	0x0xd772
    d752:	4c          	inca
    d753:	43          	coma
    d754:	44          	lsra
    d755:	20 20       	bra	0x0xd777
    d757:	20 20       	bra	0x0xd779
    d759:	20 20       	bra	0x0xd77b
    d75b:	20 20       	bra	0x0xd77d
    d75d:	20 20       	bra	0x0xd77f
    d75f:	20 20       	bra	0x0xd781
    d761:	20 20       	bra	0x0xd783
    d763:	20 20       	bra	0x0xd785
    d765:	20 20       	bra	0x0xd787
    d767:	4f          	clra
    d768:	46          	rora
    d769:	46          	rora
    d76a:	4f          	clra
    d76b:	4e          	.byte	0x4e
    d76c:	20 31       	bra	0x0xd79f
    d76e:	4f          	clra
    d76f:	4e          	.byte	0x4e
    d770:	20 32       	bra	0x0xd7a4
    d772:	4a          	deca
    d773:	55          	.byte	0x55
    d774:	4d          	tsta
    d775:	50          	negb
    d776:	45          	.byte	0x45
    d777:	44          	lsra
    d778:	49          	rola
    d779:	54          	lsrb
    d77a:	4d          	tsta
    d77b:	54          	lsrb
    d77c:	43          	coma
    d77d:	48          	asla
    d77e:	bd e9 6d    	jsr	0xe96d
    d781:	96 f9       	ldaa	*0xf9
    d783:	81 04       	cmpa	#0x4
    d785:	27 0b       	beq	0x0xd792
    d787:	ce 01 20    	ldx	#0x120
    d78a:	18 ce d7 c9 	ldy	#0xd7c9
    d78e:	c6 20       	ldab	#0x20
    d790:	20 09       	bra	0x0xd79b
    d792:	ce 01 20    	ldx	#0x120
    d795:	18 ce d7 e9 	ldy	#0xd7e9
    d799:	c6 20       	ldab	#0x20
    d79b:	18 a6 00    	ldaa	0x0,y
    d79e:	a7 00       	staa	0x0,x
    d7a0:	08          	inx
    d7a1:	18 08       	iny
    d7a3:	5a          	decb
    d7a4:	26 f5       	bne	0x0xd79b
    d7a6:	96 f9       	ldaa	*0xf9
    d7a8:	81 04       	cmpa	#0x4
    d7aa:	26 0c       	bne	0x0xd7b8
    d7ac:	b6 01 6a    	ldaa	0x16a
    d7af:	4c          	inca
    d7b0:	ce 01 3c    	ldx	#0x13c
    d7b3:	bd ea 20    	jsr	0xea20
    d7b6:	20 0e       	bra	0x0xd7c6
    d7b8:	8b 41       	adda	#0x41
    d7ba:	b7 01 3b    	staa	0x13b
    d7bd:	96 fa       	ldaa	*0xfa
    d7bf:	4c          	inca
    d7c0:	ce 01 3c    	ldx	#0x13c
    d7c3:	bd ea 20    	jsr	0xea20
    d7c6:	7e e8 aa    	jmp	0xe8aa
    d7c9:	53          	comb
    d7ca:	59          	rolb
    d7cb:	53          	comb
    d7cc:	58          	aslb
    d7cd:	20 53       	bra	0x0xd822
    d7cf:	45          	.byte	0x45
    d7d0:	4e          	.byte	0x4e
    d7d1:	44          	lsra
    d7d2:	2c 50       	bge	0x0xd824
    d7d4:	49          	rola
    d7d5:	43          	coma
    d7d6:	4b          	.byte	0x4b
    d7d7:	23 2c       	bls	0x0xd805
    d7d9:	50          	negb
    d7da:	52          	.byte	0x52
    d7db:	45          	.byte	0x45
    d7dc:	53          	comb
    d7dd:	53          	comb
    d7de:	20 53       	bra	0x0xd833
    d7e0:	41          	.byte	0x41
    d7e1:	56          	rorb
    d7e2:	45          	.byte	0x45
    d7e3:	20 20       	bra	0x0xd805
    d7e5:	20 20       	bra	0x0xd807
    d7e7:	20 20       	bra	0x0xd809
    d7e9:	53          	comb
    d7ea:	45          	.byte	0x45
    d7eb:	4c          	inca
    d7ec:	45          	.byte	0x45
    d7ed:	43          	coma
    d7ee:	54          	lsrb
    d7ef:	20 4d       	bra	0x0xd83e
    d7f1:	55          	.byte	0x55
    d7f2:	4c          	inca
    d7f3:	54          	lsrb
    d7f4:	49          	rola
    d7f5:	2c 20       	bge	0x0xd817
    d7f7:	20 20       	bra	0x0xd819
    d7f9:	50          	negb
    d7fa:	52          	.byte	0x52
    d7fb:	45          	.byte	0x45
    d7fc:	53          	comb
    d7fd:	53          	comb
    d7fe:	20 53       	bra	0x0xd853
    d800:	41          	.byte	0x41
    d801:	56          	rorb
    d802:	45          	.byte	0x45
    d803:	20 20       	bra	0x0xd825
    d805:	20 20       	bra	0x0xd827
    d807:	20 20       	bra	0x0xd829
    d809:	7d 01 1d    	tst	0x11d
    d80c:	26 0a       	bne	0x0xd818
    d80e:	bd ea 97    	jsr	0xea97
    d811:	ce 10 23    	ldx	#0x1023
    d814:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd814
    d818:	bd e9 6d    	jsr	0xe96d
    d81b:	ce 01 20    	ldx	#0x120
    d81e:	18 ce d8 50 	ldy	#0xd850
    d822:	c6 20       	ldab	#0x20
    d824:	18 a6 00    	ldaa	0x0,y
    d827:	a7 00       	staa	0x0,x
    d829:	08          	inx
    d82a:	18 08       	iny
    d82c:	5a          	decb
    d82d:	26 f5       	bne	0x0xd824
    d82f:	ce 01 31    	ldx	#0x131
    d832:	96 40       	ldaa	*0x40
    d834:	bd ea 5c    	jsr	0xea5c
    d837:	ce d8 70    	ldx	#0xd870
    d83a:	d6 41       	ldab	*0x41
    d83c:	c4 0f       	andb	#0xf
    d83e:	18 ce 01 36 	ldy	#0x136
    d842:	bd ea d3    	jsr	0xead3
    d845:	ce 01 3c    	ldx	#0x13c
    d848:	96 67       	ldaa	*0x67
    d84a:	bd ea 20    	jsr	0xea20
    d84d:	7e e8 aa    	jmp	0xe8aa
    d850:	46          	rora
    d851:	49          	rola
    d852:	4e          	.byte	0x4e
    d853:	45          	.byte	0x45
    d854:	20 4d       	bra	0x0xd8a3
    d856:	4f          	clra
    d857:	44          	lsra
    d858:	45          	.byte	0x45
    d859:	32          	pula
    d85a:	20 45       	bra	0x0xd8a1
    d85c:	4e          	.byte	0x4e
    d85d:	56          	rorb
    d85e:	31          	ins
    d85f:	20 20       	bra	0x0xd881
    d861:	20 20       	bra	0x0xd883
    d863:	20 20       	bra	0x0xd885
    d865:	20 20       	bra	0x0xd887
    d867:	20 20       	bra	0x0xd889
    d869:	20 20       	bra	0x0xd88b
    d86b:	20 20       	bra	0x0xd88d
    d86d:	20 20       	bra	0x0xd88f
    d86f:	20 4e       	bra	0x0xd8bf
    d871:	4f          	clra
    d872:	52          	.byte	0x52
    d873:	4d          	tsta
    d874:	48          	asla
    d875:	41          	.byte	0x41
    d876:	4c          	inca
    d877:	46          	rora
    d878:	4e          	.byte	0x4e
    d879:	4f          	clra
    d87a:	43          	coma
    d87b:	56          	rorb
    d87c:	4c          	inca
    d87d:	4f          	clra
    d87e:	57          	asrb
    d87f:	31          	ins
    d880:	4c          	inca
    d881:	4f          	clra
    d882:	57          	asrb
    d883:	32          	pula
    d884:	7d 01 1d    	tst	0x11d
    d887:	26 0a       	bne	0x0xd893
    d889:	bd ea 97    	jsr	0xea97
    d88c:	ce 10 23    	ldx	#0x1023
    d88f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd88f
    d893:	bd e9 6d    	jsr	0xe96d
    d896:	ce 01 20    	ldx	#0x120
    d899:	18 ce d8 c6 	ldy	#0xd8c6
    d89d:	c6 20       	ldab	#0x20
    d89f:	18 a6 00    	ldaa	0x0,y
    d8a2:	a7 00       	staa	0x0,x
    d8a4:	08          	inx
    d8a5:	18 08       	iny
    d8a7:	5a          	decb
    d8a8:	26 f5       	bne	0x0xd89f
    d8aa:	ce 01 31    	ldx	#0x131
    d8ad:	b6 01 6e    	ldaa	0x16e
    d8b0:	bd ea 5c    	jsr	0xea5c
    d8b3:	ce 01 37    	ldx	#0x137
    d8b6:	96 3e       	ldaa	*0x3e
    d8b8:	bd ea 20    	jsr	0xea20
    d8bb:	08          	inx
    d8bc:	08          	inx
    d8bd:	08          	inx
    d8be:	96 3f       	ldaa	*0x3f
    d8c0:	bd ea 20    	jsr	0xea20
    d8c3:	7e e8 aa    	jmp	0xe8aa
    d8c6:	54          	lsrb
    d8c7:	55          	.byte	0x55
    d8c8:	4e          	.byte	0x4e
    d8c9:	45          	.byte	0x45
    d8ca:	20 20       	bra	0x0xd8ec
    d8cc:	4f          	clra
    d8cd:	53          	comb
    d8ce:	43          	coma
    d8cf:	31          	ins
    d8d0:	20 4f       	bra	0x0xd921
    d8d2:	53          	comb
    d8d3:	43          	coma
    d8d4:	32          	pula
    d8d5:	20 20       	bra	0x0xd8f7
    d8d7:	20 20       	bra	0x0xd8f9
    d8d9:	20 20       	bra	0x0xd8fb
    d8db:	20 20       	bra	0x0xd8fd
    d8dd:	20 20       	bra	0x0xd8ff
    d8df:	20 20       	bra	0x0xd901
    d8e1:	20 20       	bra	0x0xd903
    d8e3:	20 20       	bra	0x0xd905
    d8e5:	20 7d       	bra	0x0xd964
    d8e7:	01          	nop
    d8e8:	1d 26 0a    	bclr	0x26,x, #0x0a
    d8eb:	bd ea 97    	jsr	0xea97
    d8ee:	ce 10 23    	ldx	#0x1023
    d8f1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd8f1
    d8f5:	bd e9 6d    	jsr	0xe96d
    d8f8:	ce 01 20    	ldx	#0x120
    d8fb:	18 ce d9 54 	ldy	#0xd954
    d8ff:	c6 20       	ldab	#0x20
    d901:	18 a6 00    	ldaa	0x0,y
    d904:	a7 00       	staa	0x0,x
    d906:	08          	inx
    d907:	18 08       	iny
    d909:	5a          	decb
    d90a:	26 f5       	bne	0x0xd901
    d90c:	ce d9 74    	ldx	#0xd974
    d90f:	18 ce 01 30 	ldy	#0x130
    d913:	13 21 40 07 	brclr	*0x21, #0x40, 0x0xd91e
    d917:	c6 01       	ldab	#0x1
    d919:	bd ea bf    	jsr	0xeabf
    d91c:	20 04       	bra	0x0xd922
    d91e:	5f          	clrb
    d91f:	bd ea bf    	jsr	0xeabf
    d922:	18 ce 01 34 	ldy	#0x134
    d926:	ce d9 7a    	ldx	#0xd97a
    d929:	5f          	clrb
    d92a:	bd ea bf    	jsr	0xeabf
    d92d:	20 00       	bra	0x0xd92f
    d92f:	ce d9 80    	ldx	#0xd980
    d932:	18 ce 01 38 	ldy	#0x138
    d936:	12 21 01 06 	brset	*0x21, #0x01, 0x0xd940
    d93a:	5f          	clrb
    d93b:	bd ea bf    	jsr	0xeabf
    d93e:	20 05       	bra	0x0xd945
    d940:	c6 01       	ldab	#0x1
    d942:	bd ea bf    	jsr	0xeabf
    d945:	ce d9 86    	ldx	#0xd986
    d948:	d6 24       	ldab	*0x24
    d94a:	18 ce 01 3c 	ldy	#0x13c
    d94e:	bd ea bf    	jsr	0xeabf
    d951:	7e e8 aa    	jmp	0xe8aa
    d954:	20 4f       	bra	0x0xd9a5
    d956:	4e          	.byte	0x4e
    d957:	20 54       	bra	0x0xd9ad
    d959:	59          	rolb
    d95a:	50          	negb
    d95b:	20 4d       	bra	0x0xd9aa
    d95d:	44          	lsra
    d95e:	45          	.byte	0x45
    d95f:	20 44       	bra	0x0xd9a5
    d961:	45          	.byte	0x45
    d962:	53          	comb
    d963:	20 20       	bra	0x0xd985
    d965:	20 20       	bra	0x0xd987
    d967:	20 20       	bra	0x0xd989
    d969:	20 20       	bra	0x0xd98b
    d96b:	20 20       	bra	0x0xd98d
    d96d:	20 20       	bra	0x0xd98f
    d96f:	20 20       	bra	0x0xd991
    d971:	20 20       	bra	0x0xd993
    d973:	20 4f       	bra	0x0xd9c4
    d975:	46          	rora
    d976:	46          	rora
    d977:	20 4f       	bra	0x0xd9c8
    d979:	4e          	.byte	0x4e
    d97a:	45          	.byte	0x45
    d97b:	58          	aslb
    d97c:	50          	negb
    d97d:	4c          	inca
    d97e:	49          	rola
    d97f:	4e          	.byte	0x4e
    d980:	52          	.byte	0x52
    d981:	45          	.byte	0x45
    d982:	47          	asra
    d983:	4c          	inca
    d984:	45          	.byte	0x45
    d985:	47          	asra
    d986:	4f          	clra
    d987:	26 46       	bne	0x0xd9cf
    d989:	4f          	clra
    d98a:	53          	comb
    d98b:	31          	ins
    d98c:	4f          	clra
    d98d:	53          	comb
    d98e:	32          	pula
    d98f:	31          	ins
    d990:	26 32       	bne	0x0xd9c4
    d992:	31          	ins
    d993:	26 46       	bne	0x0xd9db
    d995:	32          	pula
    d996:	26 46       	bne	0x0xd9de
    d998:	46          	rora
    d999:	49          	rola
    d99a:	4c          	inca
    d99b:	7d 01 1d    	tst	0x11d
    d99e:	26 0a       	bne	0x0xd9aa
    d9a0:	bd ea 97    	jsr	0xea97
    d9a3:	ce 10 23    	ldx	#0x1023
    d9a6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd9a6
    d9aa:	bd e9 6d    	jsr	0xe96d
    d9ad:	ce 01 20    	ldx	#0x120
    d9b0:	18 ce d9 ea 	ldy	#0xd9ea
    d9b4:	c6 20       	ldab	#0x20
    d9b6:	18 a6 00    	ldaa	0x0,y
    d9b9:	a7 00       	staa	0x0,x
    d9bb:	08          	inx
    d9bc:	18 08       	iny
    d9be:	5a          	decb
    d9bf:	26 f5       	bne	0x0xd9b6
    d9c1:	ce da 0a    	ldx	#0xda0a
    d9c4:	d6 6a       	ldab	*0x6a
    d9c6:	c4 0f       	andb	#0xf
    d9c8:	18 ce 01 30 	ldy	#0x130
    d9cc:	bd ea d3    	jsr	0xead3
    d9cf:	ce da 1e    	ldx	#0xda1e
    d9d2:	d6 95       	ldab	*0x95
    d9d4:	18 ce 01 36 	ldy	#0x136
    d9d8:	bd ea d3    	jsr	0xead3
    d9db:	ce da 26    	ldx	#0xda26
    d9de:	d6 69       	ldab	*0x69
    d9e0:	18 ce 01 3b 	ldy	#0x13b
    d9e4:	bd ea d3    	jsr	0xead3
    d9e7:	7e e8 aa    	jmp	0xe8aa
    d9ea:	20 55       	bra	0x0xda41
    d9ec:	4e          	.byte	0x4e
    d9ed:	49          	rola
    d9ee:	20 56       	bra	0x0xda46
    d9f0:	4d          	tsta
    d9f1:	4f          	clra
    d9f2:	44          	lsra
    d9f3:	45          	.byte	0x45
    d9f4:	20 50       	bra	0x0xda46
    d9f6:	52          	.byte	0x52
    d9f7:	49          	rola
    d9f8:	4f          	clra
    d9f9:	52          	.byte	0x52
    d9fa:	20 20       	bra	0x0xda1c
    d9fc:	20 20       	bra	0x0xda1e
    d9fe:	20 20       	bra	0x0xda20
    da00:	20 20       	bra	0x0xda22
    da02:	20 20       	bra	0x0xda24
    da04:	20 20       	bra	0x0xda26
    da06:	20 20       	bra	0x0xda28
    da08:	20 20       	bra	0x0xda2a
    da0a:	20 4f       	bra	0x0xda5b
    da0c:	4e          	.byte	0x4e
    da0d:	45          	.byte	0x45
    da0e:	20 54       	bra	0x0xda64
    da10:	57          	asrb
    da11:	4f          	clra
    da12:	46          	rora
    da13:	4f          	clra
    da14:	55          	.byte	0x55
    da15:	52          	.byte	0x52
    da16:	20 53       	bra	0x0xda6b
    da18:	49          	rola
    da19:	58          	aslb
    da1a:	45          	.byte	0x45
    da1b:	47          	asra
    da1c:	48          	asla
    da1d:	54          	lsrb
    da1e:	43          	coma
    da1f:	59          	rolb
    da20:	43          	coma
    da21:	4c          	inca
    da22:	4e          	.byte	0x4e
    da23:	4f          	clra
    da24:	54          	lsrb
    da25:	45          	.byte	0x45
    da26:	4c          	inca
    da27:	41          	.byte	0x41
    da28:	53          	comb
    da29:	54          	lsrb
    da2a:	20 4c       	bra	0x0xda78
    da2c:	4f          	clra
    da2d:	57          	asrb
    da2e:	bd e9 6d    	jsr	0xe96d
    da31:	ce 01 20    	ldx	#0x120
    da34:	18 ce da 60 	ldy	#0xda60
    da38:	c6 20       	ldab	#0x20
    da3a:	18 a6 00    	ldaa	0x0,y
    da3d:	a7 00       	staa	0x0,x
    da3f:	08          	inx
    da40:	18 08       	iny
    da42:	5a          	decb
    da43:	26 f5       	bne	0x0xda3a
    da45:	ce da 80    	ldx	#0xda80
    da48:	d6 6b       	ldab	*0x6b
    da4a:	18 ce 01 30 	ldy	#0x130
    da4e:	bd ea d3    	jsr	0xead3
    da51:	ce da 8c    	ldx	#0xda8c
    da54:	18 ce 01 36 	ldy	#0x136
    da58:	d6 68       	ldab	*0x68
    da5a:	bd ea d3    	jsr	0xead3
    da5d:	7e e8 aa    	jmp	0xe8aa
    da60:	4f          	clra
    da61:	43          	coma
    da62:	54          	lsrb
    da63:	41          	.byte	0x41
    da64:	56          	rorb
    da65:	20 4d       	bra	0x0xdab4
    da67:	54          	lsrb
    da68:	52          	.byte	0x52
    da69:	47          	asra
    da6a:	20 20       	bra	0x0xda8c
    da6c:	20 20       	bra	0x0xda8e
    da6e:	20 20       	bra	0x0xda90
    da70:	20 20       	bra	0x0xda92
    da72:	20 20       	bra	0x0xda94
    da74:	20 20       	bra	0x0xda96
    da76:	20 20       	bra	0x0xda98
    da78:	20 20       	bra	0x0xda9a
    da7a:	20 20       	bra	0x0xda9c
    da7c:	20 20       	bra	0x0xda9e
    da7e:	20 20       	bra	0x0xdaa0
    da80:	20 4c       	bra	0x0xdace
    da82:	4f          	clra
    da83:	57          	asrb
    da84:	20 4d       	bra	0x0xdad3
    da86:	49          	rola
    da87:	44          	lsra
    da88:	48          	asla
    da89:	49          	rola
    da8a:	47          	asra
    da8b:	48          	asla
    da8c:	20 4f       	bra	0x0xdadd
    da8e:	46          	rora
    da8f:	46          	rora
    da90:	45          	.byte	0x45
    da91:	4e          	.byte	0x4e
    da92:	56          	rorb
    da93:	31          	ins
    da94:	45          	.byte	0x45
    da95:	4e          	.byte	0x4e
    da96:	56          	rorb
    da97:	32          	pula
    da98:	45          	.byte	0x45
    da99:	4e          	.byte	0x4e
    da9a:	56          	rorb
    da9b:	33          	pulb
    da9c:	45          	.byte	0x45
    da9d:	31          	ins
    da9e:	26 32       	bne	0x0xdad2
    daa0:	45          	.byte	0x45
    daa1:	31          	ins
    daa2:	26 33       	bne	0x0xdad7
    daa4:	45          	.byte	0x45
    daa5:	32          	pula
    daa6:	26 33       	bne	0x0xdadb
    daa8:	20 41       	bra	0x0xdaeb
    daaa:	4c          	inca
    daab:	4c          	inca
    daac:	7d 01 1d    	tst	0x11d
    daaf:	26 0a       	bne	0x0xdabb
    dab1:	bd ea 97    	jsr	0xea97
    dab4:	ce 10 23    	ldx	#0x1023
    dab7:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdab7
    dabb:	bd e9 6d    	jsr	0xe96d
    dabe:	ce 01 20    	ldx	#0x120
    dac1:	18 ce db e5 	ldy	#0xdbe5
    dac5:	c6 20       	ldab	#0x20
    dac7:	18 a6 00    	ldaa	0x0,y
    daca:	a7 00       	staa	0x0,x
    dacc:	08          	inx
    dacd:	18 08       	iny
    dacf:	5a          	decb
    dad0:	26 f5       	bne	0x0xdac7
    dad2:	96 f6       	ldaa	*0xf6
    dad4:	48          	asla
    dad5:	24 2b       	bcc	0x0xdb02
    dad7:	ce de db    	ldx	#0xdedb
    dada:	d6 8a       	ldab	*0x8a
    dadc:	18 ce 01 26 	ldy	#0x126
    dae0:	bd ea d3    	jsr	0xead3
    dae3:	ce de db    	ldx	#0xdedb
    dae6:	d6 8b       	ldab	*0x8b
    dae8:	18 08       	iny
    daea:	18 08       	iny
    daec:	bd ea d3    	jsr	0xead3
    daef:	ce 01 37    	ldx	#0x137
    daf2:	96 8c       	ldaa	*0x8c
    daf4:	bd ea 5c    	jsr	0xea5c
    daf7:	ce 01 3c    	ldx	#0x13c
    dafa:	96 8d       	ldaa	*0x8d
    dafc:	bd ea 5c    	jsr	0xea5c
    daff:	7e e8 aa    	jmp	0xe8aa
    db02:	48          	asla
    db03:	24 2b       	bcc	0x0xdb30
    db05:	ce de db    	ldx	#0xdedb
    db08:	d6 86       	ldab	*0x86
    db0a:	18 ce 01 26 	ldy	#0x126
    db0e:	bd ea d3    	jsr	0xead3
    db11:	ce de db    	ldx	#0xdedb
    db14:	d6 87       	ldab	*0x87
    db16:	18 08       	iny
    db18:	18 08       	iny
    db1a:	bd ea d3    	jsr	0xead3
    db1d:	ce 01 37    	ldx	#0x137
    db20:	96 88       	ldaa	*0x88
    db22:	bd ea 20    	jsr	0xea20
    db25:	08          	inx
    db26:	08          	inx
    db27:	08          	inx
    db28:	96 89       	ldaa	*0x89
    db2a:	bd ea 20    	jsr	0xea20
    db2d:	7e e8 aa    	jmp	0xe8aa
    db30:	48          	asla
    db31:	24 2b       	bcc	0x0xdb5e
    db33:	ce de db    	ldx	#0xdedb
    db36:	d6 82       	ldab	*0x82
    db38:	18 ce 01 26 	ldy	#0x126
    db3c:	bd ea d3    	jsr	0xead3
    db3f:	ce de db    	ldx	#0xdedb
    db42:	d6 83       	ldab	*0x83
    db44:	18 08       	iny
    db46:	18 08       	iny
    db48:	bd ea d3    	jsr	0xead3
    db4b:	ce 01 37    	ldx	#0x137
    db4e:	96 84       	ldaa	*0x84
    db50:	bd ea 20    	jsr	0xea20
    db53:	08          	inx
    db54:	08          	inx
    db55:	08          	inx
    db56:	96 85       	ldaa	*0x85
    db58:	bd ea 20    	jsr	0xea20
    db5b:	7e e8 aa    	jmp	0xe8aa
    db5e:	48          	asla
    db5f:	24 2b       	bcc	0x0xdb8c
    db61:	ce de db    	ldx	#0xdedb
    db64:	d6 7e       	ldab	*0x7e
    db66:	18 ce 01 26 	ldy	#0x126
    db6a:	bd ea d3    	jsr	0xead3
    db6d:	ce de db    	ldx	#0xdedb
    db70:	d6 7f       	ldab	*0x7f
    db72:	18 08       	iny
    db74:	18 08       	iny
    db76:	bd ea d3    	jsr	0xead3
    db79:	ce 01 37    	ldx	#0x137
    db7c:	96 80       	ldaa	*0x80
    db7e:	bd ea 20    	jsr	0xea20
    db81:	08          	inx
    db82:	08          	inx
    db83:	08          	inx
    db84:	96 81       	ldaa	*0x81
    db86:	bd ea 20    	jsr	0xea20
    db89:	7e e8 aa    	jmp	0xe8aa
    db8c:	48          	asla
    db8d:	24 2b       	bcc	0x0xdbba
    db8f:	ce de db    	ldx	#0xdedb
    db92:	d6 7a       	ldab	*0x7a
    db94:	18 ce 01 26 	ldy	#0x126
    db98:	bd ea d3    	jsr	0xead3
    db9b:	ce de db    	ldx	#0xdedb
    db9e:	d6 7b       	ldab	*0x7b
    dba0:	18 08       	iny
    dba2:	18 08       	iny
    dba4:	bd ea d3    	jsr	0xead3
    dba7:	ce 01 37    	ldx	#0x137
    dbaa:	96 7c       	ldaa	*0x7c
    dbac:	bd ea 20    	jsr	0xea20
    dbaf:	08          	inx
    dbb0:	08          	inx
    dbb1:	08          	inx
    dbb2:	96 7d       	ldaa	*0x7d
    dbb4:	bd ea 20    	jsr	0xea20
    dbb7:	7e e8 aa    	jmp	0xe8aa
    dbba:	ce de db    	ldx	#0xdedb
    dbbd:	d6 76       	ldab	*0x76
    dbbf:	18 ce 01 26 	ldy	#0x126
    dbc3:	bd ea d3    	jsr	0xead3
    dbc6:	ce de db    	ldx	#0xdedb
    dbc9:	d6 77       	ldab	*0x77
    dbcb:	18 08       	iny
    dbcd:	18 08       	iny
    dbcf:	bd ea d3    	jsr	0xead3
    dbd2:	ce 01 37    	ldx	#0x137
    dbd5:	96 78       	ldaa	*0x78
    dbd7:	bd ea 20    	jsr	0xea20
    dbda:	08          	inx
    dbdb:	08          	inx
    dbdc:	08          	inx
    dbdd:	96 79       	ldaa	*0x79
    dbdf:	bd ea 20    	jsr	0xea20
    dbe2:	7e e8 aa    	jmp	0xe8aa
    dbe5:	20 44       	bra	0x0xdc2b
    dbe7:	45          	.byte	0x45
    dbe8:	53          	comb
    dbe9:	54          	lsrb
    dbea:	20 20       	bra	0x0xdc0c
    dbec:	20 20       	bra	0x0xdc0e
    dbee:	20 20       	bra	0x0xdc10
    dbf0:	20 20       	bra	0x0xdc12
    dbf2:	20 20       	bra	0x0xdc14
    dbf4:	20 20       	bra	0x0xdc16
    dbf6:	41          	.byte	0x41
    dbf7:	4d          	tsta
    dbf8:	4e          	.byte	0x4e
    dbf9:	54          	lsrb
    dbfa:	20 20       	bra	0x0xdc1c
    dbfc:	20 20       	bra	0x0xdc1e
    dbfe:	20 20       	bra	0x0xdc20
    dc00:	20 20       	bra	0x0xdc22
    dc02:	20 20       	bra	0x0xdc24
    dc04:	20 41       	bra	0x0xdc47
    dc06:	54          	lsrb
    dc07:	54          	lsrb
    dc08:	31          	ins
    dc09:	44          	lsra
    dc0a:	45          	.byte	0x45
    dc0b:	43          	coma
    dc0c:	31          	ins
    dc0d:	53          	comb
    dc0e:	55          	.byte	0x55
    dc0f:	53          	comb
    dc10:	31          	ins
    dc11:	52          	.byte	0x52
    dc12:	45          	.byte	0x45
    dc13:	4c          	inca
    dc14:	31          	ins
    dc15:	41          	.byte	0x41
    dc16:	4d          	tsta
    dc17:	54          	lsrb
    dc18:	31          	ins
    dc19:	41          	.byte	0x41
    dc1a:	54          	lsrb
    dc1b:	54          	lsrb
    dc1c:	32          	pula
    dc1d:	44          	lsra
    dc1e:	45          	.byte	0x45
    dc1f:	43          	coma
    dc20:	32          	pula
    dc21:	53          	comb
    dc22:	55          	.byte	0x55
    dc23:	53          	comb
    dc24:	32          	pula
    dc25:	52          	.byte	0x52
    dc26:	45          	.byte	0x45
    dc27:	4c          	inca
    dc28:	32          	pula
    dc29:	41          	.byte	0x41
    dc2a:	4d          	tsta
    dc2b:	54          	lsrb
    dc2c:	32          	pula
    dc2d:	41          	.byte	0x41
    dc2e:	54          	lsrb
    dc2f:	54          	lsrb
    dc30:	33          	pulb
    dc31:	44          	lsra
    dc32:	45          	.byte	0x45
    dc33:	43          	coma
    dc34:	33          	pulb
    dc35:	53          	comb
    dc36:	55          	.byte	0x55
    dc37:	53          	comb
    dc38:	33          	pulb
    dc39:	52          	.byte	0x52
    dc3a:	45          	.byte	0x45
    dc3b:	4c          	inca
    dc3c:	33          	pulb
    dc3d:	41          	.byte	0x41
    dc3e:	4d          	tsta
    dc3f:	54          	lsrb
    dc40:	33          	pulb
    dc41:	44          	lsra
    dc42:	45          	.byte	0x45
    dc43:	4c          	inca
    dc44:	31          	ins
    dc45:	44          	lsra
    dc46:	45          	.byte	0x45
    dc47:	4c          	inca
    dc48:	33          	pulb
    dc49:	7d 01 1d    	tst	0x11d
    dc4c:	26 0a       	bne	0x0xdc58
    dc4e:	bd ea 97    	jsr	0xea97
    dc51:	ce 10 23    	ldx	#0x1023
    dc54:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdc54
    dc58:	bd e9 6d    	jsr	0xe96d
    dc5b:	ce 01 20    	ldx	#0x120
    dc5e:	18 ce dc 8a 	ldy	#0xdc8a
    dc62:	c6 20       	ldab	#0x20
    dc64:	18 a6 00    	ldaa	0x0,y
    dc67:	a7 00       	staa	0x0,x
    dc69:	08          	inx
    dc6a:	18 08       	iny
    dc6c:	5a          	decb
    dc6d:	26 f5       	bne	0x0xdc64
    dc6f:	ce dc aa    	ldx	#0xdcaa
    dc72:	d6 8e       	ldab	*0x8e
    dc74:	18 ce 01 36 	ldy	#0x136
    dc78:	bd ea d3    	jsr	0xead3
    dc7b:	ce dc b6    	ldx	#0xdcb6
    dc7e:	d6 8f       	ldab	*0x8f
    dc80:	18 08       	iny
    dc82:	18 08       	iny
    dc84:	bd ea d3    	jsr	0xead3
    dc87:	7e e8 aa    	jmp	0xe8aa
    dc8a:	43          	coma
    dc8b:	4e          	.byte	0x4e
    dc8c:	54          	lsrb
    dc8d:	52          	.byte	0x52
    dc8e:	4c          	inca
    dc8f:	20 43       	bra	0x0xdcd4
    dc91:	4f          	clra
    dc92:	4e          	.byte	0x4e
    dc93:	31          	ins
    dc94:	20 43       	bra	0x0xdcd9
    dc96:	4f          	clra
    dc97:	4e          	.byte	0x4e
    dc98:	32          	pula
    dc99:	20 41       	bra	0x0xdcdc
    dc9b:	53          	comb
    dc9c:	53          	comb
    dc9d:	47          	asra
    dc9e:	4e          	.byte	0x4e
    dc9f:	20 20       	bra	0x0xdcc1
    dca1:	20 20       	bra	0x0xdcc3
    dca3:	20 20       	bra	0x0xdcc5
    dca5:	20 20       	bra	0x0xdcc7
    dca7:	20 20       	bra	0x0xdcc9
    dca9:	20 42       	bra	0x0xdced
    dcab:	52          	.byte	0x52
    dcac:	54          	lsrb
    dcad:	48          	asla
    dcae:	4d          	tsta
    dcaf:	4f          	clra
    dcb0:	44          	lsra
    dcb1:	57          	asrb
    dcb2:	54          	lsrb
    dcb3:	55          	.byte	0x55
    dcb4:	43          	coma
    dcb5:	48          	asla
    dcb6:	20 44       	bra	0x0xdcfc
    dcb8:	59          	rolb
    dcb9:	4e          	.byte	0x4e
    dcba:	54          	lsrb
    dcbb:	52          	.byte	0x52
    dcbc:	41          	.byte	0x41
    dcbd:	4b          	.byte	0x4b
    dcbe:	7d 01 1d    	tst	0x11d
    dcc1:	26 0a       	bne	0x0xdccd
    dcc3:	bd ea 97    	jsr	0xea97
    dcc6:	ce 10 23    	ldx	#0x1023
    dcc9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdcc9
    dccd:	bd e9 6d    	jsr	0xe96d
    dcd0:	ce 01 20    	ldx	#0x120
    dcd3:	18 ce dd 0c 	ldy	#0xdd0c
    dcd7:	c6 20       	ldab	#0x20
    dcd9:	18 a6 00    	ldaa	0x0,y
    dcdc:	a7 00       	staa	0x0,x
    dcde:	08          	inx
    dcdf:	18 08       	iny
    dce1:	5a          	decb
    dce2:	26 f5       	bne	0x0xdcd9
    dce4:	f6 50 00    	ldab	0x5000
    dce7:	c1 0a       	cmpb	#0xa
    dce9:	25 04       	bcs	0x0xdcef
    dceb:	5f          	clrb
    dcec:	f7 50 00    	stab	0x5000
    dcef:	86 09       	ldaa	#0x9
    dcf1:	3d          	mul
    dcf2:	1b          	aba
    dcf3:	16          	tab
    dcf4:	ce dd 2c    	ldx	#0xdd2c
    dcf7:	3a          	abx
    dcf8:	18 ce 01 31 	ldy	#0x131
    dcfc:	c6 09       	ldab	#0x9
    dcfe:	a6 00       	ldaa	0x0,x
    dd00:	18 a7 00    	staa	0x0,y
    dd03:	08          	inx
    dd04:	18 08       	iny
    dd06:	5a          	decb
    dd07:	26 f5       	bne	0x0xdcfe
    dd09:	7e e8 aa    	jmp	0xe8aa
    dd0c:	20 4d       	bra	0x0xdd5b
    dd0e:	55          	.byte	0x55
    dd0f:	4c          	inca
    dd10:	54          	lsrb
    dd11:	49          	rola
    dd12:	20 54       	bra	0x0xdd68
    dd14:	59          	rolb
    dd15:	50          	negb
    dd16:	45          	.byte	0x45
    dd17:	3a          	abx
    dd18:	20 20       	bra	0x0xdd3a
    dd1a:	20 20       	bra	0x0xdd3c
    dd1c:	20 20       	bra	0x0xdd3e
    dd1e:	20 20       	bra	0x0xdd40
    dd20:	20 20       	bra	0x0xdd42
    dd22:	20 20       	bra	0x0xdd44
    dd24:	20 20       	bra	0x0xdd46
    dd26:	20 20       	bra	0x0xdd48
    dd28:	20 20       	bra	0x0xdd4a
    dd2a:	20 20       	bra	0x0xdd4c
    dd2c:	50          	negb
    dd2d:	52          	.byte	0x52
    dd2e:	45          	.byte	0x45
    dd2f:	50          	negb
    dd30:	41          	.byte	0x41
    dd31:	52          	.byte	0x52
    dd32:	45          	.byte	0x45
    dd33:	44          	lsra
    dd34:	20 53       	bra	0x0xdd89
    dd36:	50          	negb
    dd37:	4c          	inca
    dd38:	49          	rola
    dd39:	54          	lsrb
    dd3a:	20 31       	bra	0x0xdd6d
    dd3c:	2b 37       	bmi	0x0xdd75
    dd3e:	53          	comb
    dd3f:	50          	negb
    dd40:	4c          	inca
    dd41:	49          	rola
    dd42:	54          	lsrb
    dd43:	20 32       	bra	0x0xdd77
    dd45:	2b 36       	bmi	0x0xdd7d
    dd47:	53          	comb
    dd48:	50          	negb
    dd49:	4c          	inca
    dd4a:	49          	rola
    dd4b:	54          	lsrb
    dd4c:	20 33       	bra	0x0xdd81
    dd4e:	2b 35       	bmi	0x0xdd85
    dd50:	53          	comb
    dd51:	50          	negb
    dd52:	4c          	inca
    dd53:	49          	rola
    dd54:	54          	lsrb
    dd55:	20 34       	bra	0x0xdd8b
    dd57:	2b 34       	bmi	0x0xdd8d
    dd59:	4c          	inca
    dd5a:	41          	.byte	0x41
    dd5b:	59          	rolb
    dd5c:	45          	.byte	0x45
    dd5d:	52          	.byte	0x52
    dd5e:	20 34       	bra	0x0xdd94
    dd60:	2b 34       	bmi	0x0xdd96
    dd62:	4d          	tsta
    dd63:	55          	.byte	0x55
    dd64:	4c          	inca
    dd65:	54          	lsrb
    dd66:	49          	rola
    dd67:	43          	coma
    dd68:	48          	asla
    dd69:	41          	.byte	0x41
    dd6a:	4e          	.byte	0x4e
    dd6b:	01          	nop
    dd6c:	02          	idiv
    dd6d:	04          	lsrd
    dd6e:	08          	inx
    dd6f:	10          	sba
    dd70:	20 40       	bra	0x0xddb2
    dd72:	80 01       	suba	#0x1
    dd74:	fe 00 00    	ldx	0x0
    dd77:	00          	bgnd
    dd78:	00          	bgnd
    dd79:	00          	bgnd
    dd7a:	00          	bgnd
    dd7b:	03          	fdiv
    dd7c:	fc 00 00    	ldd	0x0
    dd7f:	00          	bgnd
    dd80:	00          	bgnd
    dd81:	00          	bgnd
    dd82:	00          	bgnd
    dd83:	07          	tpa
    dd84:	f8 00 00    	eorb	0x0
    dd87:	00          	bgnd
    dd88:	00          	bgnd
    dd89:	00          	bgnd
    dd8a:	00          	bgnd
    dd8b:	0f          	sei
    dd8c:	f0 00 00    	subb	0x0
    dd8f:	00          	bgnd
    dd90:	00          	bgnd
    dd91:	00          	bgnd
    dd92:	00          	bgnd
    dd93:	0f          	sei
    dd94:	f0 00 00    	subb	0x0
    dd97:	00          	bgnd
    dd98:	00          	bgnd
    dd99:	00          	bgnd
    dd9a:	00          	bgnd
    dd9b:	03          	fdiv
    dd9c:	fc 00 00    	ldd	0x0
    dd9f:	00          	bgnd
    dda0:	00          	bgnd
    dda1:	00          	bgnd
    dda2:	00          	bgnd
    dda3:	01          	nop
    dda4:	02          	idiv
    dda5:	fc 00 00    	ldd	0x0
    dda8:	00          	bgnd
    dda9:	00          	bgnd
    ddaa:	00          	bgnd
    ddab:	01          	nop
    ddac:	02          	idiv
    ddad:	04          	lsrd
    ddae:	08          	inx
    ddaf:	10          	sba
    ddb0:	20 40       	bra	0x0xddf2
    ddb2:	80 00       	suba	#0x0
    ddb4:	01          	nop
    ddb5:	03          	fdiv
    ddb6:	07          	tpa
    ddb7:	0f          	sei
    ddb8:	1f 3f 7f ff 	brclr	0x3f,x, #0x7f, 0x0xddbb
    ddbc:	01          	nop
    ddbd:	02          	idiv
    ddbe:	00          	bgnd
    ddbf:	00          	bgnd
    ddc0:	00          	bgnd
    ddc1:	00          	bgnd
    ddc2:	00          	bgnd
    ddc3:	00          	bgnd
    ddc4:	03          	fdiv
    ddc5:	0c          	clc
    ddc6:	00          	bgnd
    ddc7:	00          	bgnd
    ddc8:	00          	bgnd
    ddc9:	00          	bgnd
    ddca:	00          	bgnd
    ddcb:	00          	bgnd
    ddcc:	07          	tpa
    ddcd:	38          	pulx
    ddce:	00          	bgnd
    ddcf:	00          	bgnd
    ddd0:	00          	bgnd
    ddd1:	00          	bgnd
    ddd2:	00          	bgnd
    ddd3:	00          	bgnd
    ddd4:	0f          	sei
    ddd5:	f0 00 00    	subb	0x0
    ddd8:	00          	bgnd
    ddd9:	00          	bgnd
    ddda:	00          	bgnd
    dddb:	00          	bgnd
    dddc:	bd e9 6d    	jsr	0xe96d
    dddf:	ce 01 20    	ldx	#0x120
    dde2:	18 ce dd ff 	ldy	#0xddff
    dde6:	c6 20       	ldab	#0x20
    dde8:	18 a6 00    	ldaa	0x0,y
    ddeb:	a7 00       	staa	0x0,x
    dded:	08          	inx
    ddee:	18 08       	iny
    ddf0:	5a          	decb
    ddf1:	26 f5       	bne	0x0xdde8
    ddf3:	b6 50 21    	ldaa	0x5021
    ddf6:	ce 01 3c    	ldx	#0x13c
    ddf9:	bd ea 20    	jsr	0xea20
    ddfc:	7e e8 aa    	jmp	0xe8aa
    ddff:	53          	comb
    de00:	50          	negb
    de01:	4c          	inca
    de02:	49          	rola
    de03:	54          	lsrb
    de04:	20 50       	bra	0x0xde56
    de06:	4f          	clra
    de07:	49          	rola
    de08:	4e          	.byte	0x4e
    de09:	54          	lsrb
    de0a:	3a          	abx
    de0b:	20 20       	bra	0x0xde2d
    de0d:	20 20       	bra	0x0xde2f
    de0f:	4d          	tsta
    de10:	49          	rola
    de11:	44          	lsra
    de12:	49          	rola
    de13:	20 4e       	bra	0x0xde63
    de15:	4f          	clra
    de16:	54          	lsrb
    de17:	45          	.byte	0x45
    de18:	20 23       	bra	0x0xde3d
    de1a:	20 20       	bra	0x0xde3c
    de1c:	20 20       	bra	0x0xde3e
    de1e:	20 7d       	bra	0x0xde9d
    de20:	01          	nop
    de21:	1d 26 0a    	bclr	0x26,x, #0x0a
    de24:	bd ea 97    	jsr	0xea97
    de27:	ce 10 23    	ldx	#0x1023
    de2a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xde2a
    de2e:	bd e9 6d    	jsr	0xe96d
    de31:	ce 01 20    	ldx	#0x120
    de34:	18 ce de 5a 	ldy	#0xde5a
    de38:	c6 20       	ldab	#0x20
    de3a:	18 a6 00    	ldaa	0x0,y
    de3d:	a7 00       	staa	0x0,x
    de3f:	08          	inx
    de40:	18 08       	iny
    de42:	5a          	decb
    de43:	26 f5       	bne	0x0xde3a
    de45:	ce 01 34    	ldx	#0x134
    de48:	b6 01 68    	ldaa	0x168
    de4b:	bd ea 20    	jsr	0xea20
    de4e:	ce 01 3c    	ldx	#0x13c
    de51:	b6 01 69    	ldaa	0x169
    de54:	bd ea 20    	jsr	0xea20
    de57:	7e e8 aa    	jmp	0xe8aa
    de5a:	20 44       	bra	0x0xdea0
    de5c:	59          	rolb
    de5d:	4e          	.byte	0x4e
    de5e:	41          	.byte	0x41
    de5f:	4d          	tsta
    de60:	49          	rola
    de61:	43          	coma
    de62:	53          	comb
    de63:	20 52       	bra	0x0xdeb7
    de65:	41          	.byte	0x41
    de66:	4e          	.byte	0x4e
    de67:	47          	asra
    de68:	45          	.byte	0x45
    de69:	20 20       	bra	0x0xde8b
    de6b:	4f          	clra
    de6c:	4e          	.byte	0x4e
    de6d:	20 20       	bra	0x0xde8f
    de6f:	20 20       	bra	0x0xde91
    de71:	20 4f       	bra	0x0xdec2
    de73:	46          	rora
    de74:	46          	rora
    de75:	20 20       	bra	0x0xde97
    de77:	20 20       	bra	0x0xde99
    de79:	20 7d       	bra	0x0xdef8
    de7b:	01          	nop
    de7c:	1d 26 0a    	bclr	0x26,x, #0x0a
    de7f:	bd ea 97    	jsr	0xea97
    de82:	ce 10 23    	ldx	#0x1023
    de85:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xde85
    de89:	bd e9 6d    	jsr	0xe96d
    de8c:	86 20       	ldaa	#0x20
    de8e:	c6 20       	ldab	#0x20
    de90:	ce 01 20    	ldx	#0x120
    de93:	a7 00       	staa	0x0,x
    de95:	08          	inx
    de96:	5a          	decb
    de97:	26 fa       	bne	0x0xde93
    de99:	ce de db    	ldx	#0xdedb
    de9c:	d6 60       	ldab	*0x60
    de9e:	18 ce 01 20 	ldy	#0x120
    dea2:	bd ea d3    	jsr	0xead3
    dea5:	ce de db    	ldx	#0xdedb
    dea8:	d6 61       	ldab	*0x61
    deaa:	18 08       	iny
    deac:	18 08       	iny
    deae:	18 08       	iny
    deb0:	bd ea d3    	jsr	0xead3
    deb3:	ce de db    	ldx	#0xdedb
    deb6:	d6 62       	ldab	*0x62
    deb8:	18 08       	iny
    deba:	18 08       	iny
    debc:	bd ea d3    	jsr	0xead3
    debf:	ce 01 31    	ldx	#0x131
    dec2:	96 63       	ldaa	*0x63
    dec4:	bd ea 20    	jsr	0xea20
    dec7:	08          	inx
    dec8:	08          	inx
    dec9:	08          	inx
    deca:	08          	inx
    decb:	96 64       	ldaa	*0x64
    decd:	bd ea 20    	jsr	0xea20
    ded0:	08          	inx
    ded1:	08          	inx
    ded2:	08          	inx
    ded3:	96 65       	ldaa	*0x65
    ded5:	bd ea 20    	jsr	0xea20
    ded8:	7e e8 aa    	jmp	0xe8aa
    dedb:	20 4f       	bra	0x0xdf2c
    dedd:	46          	rora
    dede:	46          	rora
    dedf:	46          	rora
    dee0:	52          	.byte	0x52
    dee1:	45          	.byte	0x45
    dee2:	31          	ins
    dee3:	46          	rora
    dee4:	52          	.byte	0x52
    dee5:	45          	.byte	0x45
    dee6:	32          	pula
    dee7:	31          	ins
    dee8:	26 32       	bne	0x0xdf1c
    deea:	46          	rora
    deeb:	4c          	inca
    deec:	45          	.byte	0x45
    deed:	56          	rorb
    deee:	31          	ins
    deef:	4c          	inca
    def0:	45          	.byte	0x45
    def1:	56          	rorb
    def2:	32          	pula
    def3:	20 50       	bra	0x0xdf45
    def5:	57          	asrb
    def6:	31          	ins
    def7:	20 50       	bra	0x0xdf49
    def9:	57          	asrb
    defa:	32          	pula
    defb:	31          	ins
    defc:	26 32       	bne	0x0xdf30
    defe:	50          	negb
    deff:	46          	rora
    df00:	49          	rola
    df01:	4c          	inca
    df02:	54          	lsrb
    df03:	52          	.byte	0x52
    df04:	45          	.byte	0x45
    df05:	53          	comb
    df06:	4f          	clra
    df07:	4c          	inca
    df08:	45          	.byte	0x45
    df09:	56          	rorb
    df0a:	4e          	.byte	0x4e
    df0b:	58          	aslb
    df0c:	4d          	tsta
    df0d:	4f          	clra
    df0e:	44          	lsra
    df0f:	20 45       	bra	0x0xdf56
    df11:	41          	.byte	0x41
    df12:	31          	ins
    df13:	20 45       	bra	0x0xdf5a
    df15:	41          	.byte	0x41
    df16:	33          	pulb
    df17:	20 45       	bra	0x0xdf5e
    df19:	58          	aslb
    df1a:	54          	lsrb
    df1b:	4c          	inca
    df1c:	46          	rora
    df1d:	31          	ins
    df1e:	52          	.byte	0x52
    df1f:	4c          	inca
    df20:	46          	rora
    df21:	32          	pula
    df22:	52          	.byte	0x52
    df23:	4c          	inca
    df24:	46          	rora
    df25:	31          	ins
    df26:	44          	lsra
    df27:	4c          	inca
    df28:	46          	rora
    df29:	32          	pula
    df2a:	44          	lsra
    df2b:	31          	ins
    df2c:	26 32       	bne	0x0xdf60
    df2e:	44          	lsra
    df2f:	31          	ins
    df30:	26 32       	bne	0x0xdf64
    df32:	52          	.byte	0x52
    df33:	50          	negb
    df34:	41          	.byte	0x41
    df35:	4e          	.byte	0x4e
    df36:	52          	.byte	0x52
    df37:	50          	negb
    df38:	41          	.byte	0x41
    df39:	4e          	.byte	0x4e
    df3a:	44          	lsra
    df3b:	20 50       	bra	0x0xdf8d
    df3d:	41          	.byte	0x41
    df3e:	4e          	.byte	0x4e
    df3f:	bd e9 6d    	jsr	0xe96d
    df42:	ce 01 20    	ldx	#0x120
    df45:	18 ce df 72 	ldy	#0xdf72
    df49:	c6 20       	ldab	#0x20
    df4b:	18 a6 00    	ldaa	0x0,y
    df4e:	a7 00       	staa	0x0,x
    df50:	08          	inx
    df51:	18 08       	iny
    df53:	5a          	decb
    df54:	26 f5       	bne	0x0xdf4b
    df56:	ce 01 31    	ldx	#0x131
    df59:	96 54       	ldaa	*0x54
    df5b:	bd ea 20    	jsr	0xea20
    df5e:	08          	inx
    df5f:	08          	inx
    df60:	08          	inx
    df61:	08          	inx
    df62:	96 59       	ldaa	*0x59
    df64:	bd ea 20    	jsr	0xea20
    df67:	08          	inx
    df68:	08          	inx
    df69:	08          	inx
    df6a:	96 5e       	ldaa	*0x5e
    df6c:	bd ea 20    	jsr	0xea20
    df6f:	7e e8 aa    	jmp	0xe8aa
    df72:	20 44       	bra	0x0xdfb8
    df74:	4b          	.byte	0x4b
    df75:	32          	pula
    df76:	20 20       	bra	0x0xdf98
    df78:	20 44       	bra	0x0xdfbe
    df7a:	4b          	.byte	0x4b
    df7b:	32          	pula
    df7c:	20 20       	bra	0x0xdf9e
    df7e:	44          	lsra
    df7f:	4b          	.byte	0x4b
    df80:	32          	pula
    df81:	20 20       	bra	0x0xdfa3
    df83:	20 20       	bra	0x0xdfa5
    df85:	20 20       	bra	0x0xdfa7
    df87:	20 20       	bra	0x0xdfa9
    df89:	20 20       	bra	0x0xdfab
    df8b:	20 20       	bra	0x0xdfad
    df8d:	20 20       	bra	0x0xdfaf
    df8f:	20 20       	bra	0x0xdfb1
    df91:	20 bd       	bra	0x0xdf50
    df93:	e9 6d       	adcb	0x6d,x
    df95:	ce 01 20    	ldx	#0x120
    df98:	18 ce df c4 	ldy	#0xdfc4
    df9c:	c6 20       	ldab	#0x20
    df9e:	18 a6 00    	ldaa	0x0,y
    dfa1:	a7 00       	staa	0x0,x
    dfa3:	08          	inx
    dfa4:	18 08       	iny
    dfa6:	5a          	decb
    dfa7:	26 f5       	bne	0x0xdf9e
    dfa9:	ce 01 31    	ldx	#0x131
    dfac:	96 90       	ldaa	*0x90
    dfae:	bd ea 20    	jsr	0xea20
    dfb1:	ce 01 37    	ldx	#0x137
    dfb4:	96 91       	ldaa	*0x91
    dfb6:	bd ea 20    	jsr	0xea20
    dfb9:	ce 01 3c    	ldx	#0x13c
    dfbc:	96 92       	ldaa	*0x92
    dfbe:	bd ea 20    	jsr	0xea20
    dfc1:	7e e8 aa    	jmp	0xe8aa
    dfc4:	44          	lsra
    dfc5:	59          	rolb
    dfc6:	4e          	.byte	0x4e
    dfc7:	31          	ins
    dfc8:	20 20       	bra	0x0xdfea
    dfca:	44          	lsra
    dfcb:	59          	rolb
    dfcc:	4e          	.byte	0x4e
    dfcd:	32          	pula
    dfce:	20 44       	bra	0x0xe014
    dfd0:	59          	rolb
    dfd1:	4e          	.byte	0x4e
    dfd2:	33          	pulb
    dfd3:	20 20       	bra	0x0xdff5
    dfd5:	20 20       	bra	0x0xdff7
    dfd7:	20 20       	bra	0x0xdff9
    dfd9:	20 20       	bra	0x0xdffb
    dfdb:	20 20       	bra	0x0xdffd
    dfdd:	20 20       	bra	0x0xdfff
    dfdf:	20 20       	bra	0x0xe001
    dfe1:	20 20       	bra	0x0xe003
    dfe3:	20 bd       	bra	0x0xdfa2
    dfe5:	e9 6d       	adcb	0x6d,x
    dfe7:	ce 01 20    	ldx	#0x120
    dfea:	18 ce e0 0e 	ldy	#0xe00e
    dfee:	c6 20       	ldab	#0x20
    dff0:	18 a6 00    	ldaa	0x0,y
    dff3:	a7 00       	staa	0x0,x
    dff5:	08          	inx
    dff6:	18 08       	iny
    dff8:	5a          	decb
    dff9:	26 f5       	bne	0x0xdff0
    dffb:	ce 01 31    	ldx	#0x131
    dffe:	96 50       	ldaa	*0x50
    e000:	bd ea 20    	jsr	0xea20
    e003:	ce 01 37    	ldx	#0x137
    e006:	96 98       	ldaa	*0x98
    e008:	bd ea 20    	jsr	0xea20
    e00b:	7e e8 aa    	jmp	0xe8aa
    e00e:	44          	lsra
    e00f:	4c          	inca
    e010:	41          	.byte	0x41
    e011:	59          	rolb
    e012:	31          	ins
    e013:	20 44       	bra	0x0xe059
    e015:	4c          	inca
    e016:	41          	.byte	0x41
    e017:	59          	rolb
    e018:	33          	pulb
    e019:	20 20       	bra	0x0xe03b
    e01b:	20 20       	bra	0x0xe03d
    e01d:	20 20       	bra	0x0xe03f
    e01f:	20 20       	bra	0x0xe041
    e021:	20 20       	bra	0x0xe043
    e023:	20 20       	bra	0x0xe045
    e025:	20 20       	bra	0x0xe047
    e027:	20 20       	bra	0x0xe049
    e029:	20 20       	bra	0x0xe04b
    e02b:	20 20       	bra	0x0xe04d
    e02d:	20 7d       	bra	0x0xe0ac
    e02f:	01          	nop
    e030:	1d 26 0a    	bclr	0x26,x, #0x0a
    e033:	bd ea 97    	jsr	0xea97
    e036:	ce 10 23    	ldx	#0x1023
    e039:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe039
    e03d:	bd e9 6d    	jsr	0xe96d
    e040:	ce 01 20    	ldx	#0x120
    e043:	18 ce e0 71 	ldy	#0xe071
    e047:	c6 20       	ldab	#0x20
    e049:	18 a6 00    	ldaa	0x0,y
    e04c:	a7 00       	staa	0x0,x
    e04e:	08          	inx
    e04f:	18 08       	iny
    e051:	5a          	decb
    e052:	26 f5       	bne	0x0xe049
    e054:	ce e0 91    	ldx	#0xe091
    e057:	d6 4b       	ldab	*0x4b
    e059:	18 ce 01 30 	ldy	#0x130
    e05d:	bd ea d3    	jsr	0xead3
    e060:	ce e0 ad    	ldx	#0xe0ad
    e063:	d6 66       	ldab	*0x66
    e065:	18 08       	iny
    e067:	18 08       	iny
    e069:	18 08       	iny
    e06b:	bd ea d3    	jsr	0xead3
    e06e:	7e e8 aa    	jmp	0xe8aa
    e071:	54          	lsrb
    e072:	59          	rolb
    e073:	50          	negb
    e074:	45          	.byte	0x45
    e075:	20 20       	bra	0x0xe097
    e077:	49          	rola
    e078:	4e          	.byte	0x4e
    e079:	56          	rorb
    e07a:	54          	lsrb
    e07b:	20 20       	bra	0x0xe09d
    e07d:	20 20       	bra	0x0xe09f
    e07f:	20 20       	bra	0x0xe0a1
    e081:	20 20       	bra	0x0xe0a3
    e083:	20 20       	bra	0x0xe0a5
    e085:	20 20       	bra	0x0xe0a7
    e087:	20 20       	bra	0x0xe0a9
    e089:	20 20       	bra	0x0xe0ab
    e08b:	20 20       	bra	0x0xe0ad
    e08d:	20 20       	bra	0x0xe0af
    e08f:	20 20       	bra	0x0xe0b1
    e091:	4f          	clra
    e092:	42          	.byte	0x42
    e093:	4c          	inca
    e094:	50          	negb
    e095:	4f          	clra
    e096:	42          	.byte	0x42
    e097:	42          	.byte	0x42
    e098:	50          	negb
    e099:	4f          	clra
    e09a:	42          	.byte	0x42
    e09b:	48          	asla
    e09c:	50          	negb
    e09d:	4f          	clra
    e09e:	42          	.byte	0x42
    e09f:	42          	.byte	0x42
    e0a0:	52          	.byte	0x52
    e0a1:	4d          	tsta
    e0a2:	49          	rola
    e0a3:	4e          	.byte	0x4e
    e0a4:	49          	rola
    e0a5:	41          	.byte	0x41
    e0a6:	55          	.byte	0x55
    e0a7:	58          	aslb
    e0a8:	31          	ins
    e0a9:	43          	coma
    e0aa:	53          	comb
    e0ab:	38          	pulx
    e0ac:	30          	tsx
    e0ad:	20 4f       	bra	0x0xe0fe
    e0af:	46          	rora
    e0b0:	46          	rora
    e0b1:	45          	.byte	0x45
    e0b2:	4e          	.byte	0x4e
    e0b3:	56          	rorb
    e0b4:	31          	ins
    e0b5:	45          	.byte	0x45
    e0b6:	4e          	.byte	0x4e
    e0b7:	56          	rorb
    e0b8:	33          	pulb
    e0b9:	45          	.byte	0x45
    e0ba:	31          	ins
    e0bb:	26 33       	bne	0x0xe0f0
    e0bd:	bd e9 6d    	jsr	0xe96d
    e0c0:	ce 01 20    	ldx	#0x120
    e0c3:	18 ce e0 fe 	ldy	#0xe0fe
    e0c7:	c6 20       	ldab	#0x20
    e0c9:	86 06       	ldaa	#0x6
    e0cb:	91 4b       	cmpa	*0x4b
    e0cd:	26 04       	bne	0x0xe0d3
    e0cf:	18 ce e1 1e 	ldy	#0xe11e
    e0d3:	18 a6 00    	ldaa	0x0,y
    e0d6:	a7 00       	staa	0x0,x
    e0d8:	08          	inx
    e0d9:	18 08       	iny
    e0db:	5a          	decb
    e0dc:	26 f5       	bne	0x0xe0d3
    e0de:	ce 01 30    	ldx	#0x130
    e0e1:	96 6e       	ldaa	*0x6e
    e0e3:	bd ea 20    	jsr	0xea20
    e0e6:	08          	inx
    e0e7:	08          	inx
    e0e8:	96 48       	ldaa	*0x48
    e0ea:	bd ea 20    	jsr	0xea20
    e0ed:	08          	inx
    e0ee:	08          	inx
    e0ef:	96 6f       	ldaa	*0x6f
    e0f1:	bd ea 20    	jsr	0xea20
    e0f4:	08          	inx
    e0f5:	08          	inx
    e0f6:	96 70       	ldaa	*0x70
    e0f8:	bd ea e7    	jsr	0xeae7
    e0fb:	7e e8 aa    	jmp	0xe8aa
    e0fe:	20 49       	bra	0x0xe149
    e100:	4e          	.byte	0x4e
    e101:	20 4d       	bra	0x0xe150
    e103:	49          	rola
    e104:	58          	aslb
    e105:	20 54       	bra	0x0xe15b
    e107:	52          	.byte	0x52
    e108:	47          	asra
    e109:	20 57       	bra	0x0xe162
    e10b:	49          	rola
    e10c:	4e          	.byte	0x4e
    e10d:	20 20       	bra	0x0xe12f
    e10f:	20 20       	bra	0x0xe131
    e111:	20 20       	bra	0x0xe133
    e113:	20 20       	bra	0x0xe135
    e115:	20 20       	bra	0x0xe137
    e117:	20 20       	bra	0x0xe139
    e119:	20 20       	bra	0x0xe13b
    e11b:	20 20       	bra	0x0xe13d
    e11d:	20 20       	bra	0x0xe13f
    e11f:	49          	rola
    e120:	4e          	.byte	0x4e
    e121:	20 48       	bra	0x0xe16b
    e123:	50          	negb
    e124:	46          	rora
    e125:	20 48       	bra	0x0xe16f
    e127:	50          	negb
    e128:	52          	.byte	0x52
    e129:	20 57       	bra	0x0xe182
    e12b:	49          	rola
    e12c:	4e          	.byte	0x4e
    e12d:	20 20       	bra	0x0xe14f
    e12f:	20 20       	bra	0x0xe151
    e131:	20 20       	bra	0x0xe153
    e133:	20 20       	bra	0x0xe155
    e135:	20 20       	bra	0x0xe157
    e137:	20 20       	bra	0x0xe159
    e139:	20 20       	bra	0x0xe15b
    e13b:	20 20       	bra	0x0xe15d
    e13d:	20 7d       	bra	0x0xe1bc
    e13f:	01          	nop
    e140:	1d 26 0a    	bclr	0x26,x, #0x0a
    e143:	bd ea 97    	jsr	0xea97
    e146:	ce 10 23    	ldx	#0x1023
    e149:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe149
    e14d:	bd e9 6d    	jsr	0xe96d
    e150:	86 20       	ldaa	#0x20
    e152:	c6 20       	ldab	#0x20
    e154:	ce 01 20    	ldx	#0x120
    e157:	a7 00       	staa	0x0,x
    e159:	08          	inx
    e15a:	5a          	decb
    e15b:	26 fa       	bne	0x0xe157
    e15d:	ce e1 e2    	ldx	#0xe1e2
    e160:	12 f4 02 3f 	brset	*0xf4, #0x02, 0x0xe1a3
    e164:	d6 2d       	ldab	*0x2d
    e166:	18 ce 01 20 	ldy	#0x120
    e16a:	bd ea d3    	jsr	0xead3
    e16d:	ce e1 e2    	ldx	#0xe1e2
    e170:	d6 2e       	ldab	*0x2e
    e172:	18 08       	iny
    e174:	18 08       	iny
    e176:	18 08       	iny
    e178:	bd ea d3    	jsr	0xead3
    e17b:	ce e1 e2    	ldx	#0xe1e2
    e17e:	d6 2f       	ldab	*0x2f
    e180:	18 08       	iny
    e182:	18 08       	iny
    e184:	bd ea d3    	jsr	0xead3
    e187:	ce 01 31    	ldx	#0x131
    e18a:	96 2a       	ldaa	*0x2a
    e18c:	bd ea 20    	jsr	0xea20
    e18f:	08          	inx
    e190:	08          	inx
    e191:	08          	inx
    e192:	08          	inx
    e193:	96 2b       	ldaa	*0x2b
    e195:	bd ea 20    	jsr	0xea20
    e198:	08          	inx
    e199:	08          	inx
    e19a:	08          	inx
    e19b:	96 2c       	ldaa	*0x2c
    e19d:	bd ea 20    	jsr	0xea20
    e1a0:	7e e8 aa    	jmp	0xe8aa
    e1a3:	d6 3a       	ldab	*0x3a
    e1a5:	18 ce 01 20 	ldy	#0x120
    e1a9:	bd ea d3    	jsr	0xead3
    e1ac:	ce e1 e2    	ldx	#0xe1e2
    e1af:	d6 3b       	ldab	*0x3b
    e1b1:	18 08       	iny
    e1b3:	18 08       	iny
    e1b5:	18 08       	iny
    e1b7:	bd ea d3    	jsr	0xead3
    e1ba:	ce e1 e2    	ldx	#0xe1e2
    e1bd:	d6 3c       	ldab	*0x3c
    e1bf:	18 08       	iny
    e1c1:	18 08       	iny
    e1c3:	bd ea d3    	jsr	0xead3
    e1c6:	ce 01 31    	ldx	#0x131
    e1c9:	96 37       	ldaa	*0x37
    e1cb:	bd ea 20    	jsr	0xea20
    e1ce:	08          	inx
    e1cf:	08          	inx
    e1d0:	08          	inx
    e1d1:	08          	inx
    e1d2:	96 38       	ldaa	*0x38
    e1d4:	bd ea 20    	jsr	0xea20
    e1d7:	08          	inx
    e1d8:	08          	inx
    e1d9:	08          	inx
    e1da:	96 39       	ldaa	*0x39
    e1dc:	bd ea 20    	jsr	0xea20
    e1df:	7e e8 aa    	jmp	0xe8aa
    e1e2:	20 4f       	bra	0x0xe233
    e1e4:	46          	rora
    e1e5:	46          	rora
    e1e6:	46          	rora
    e1e7:	52          	.byte	0x52
    e1e8:	45          	.byte	0x45
    e1e9:	31          	ins
    e1ea:	46          	rora
    e1eb:	52          	.byte	0x52
    e1ec:	45          	.byte	0x45
    e1ed:	32          	pula
    e1ee:	31          	ins
    e1ef:	26 32       	bne	0x0xe223
    e1f1:	46          	rora
    e1f2:	4c          	inca
    e1f3:	45          	.byte	0x45
    e1f4:	56          	rorb
    e1f5:	31          	ins
    e1f6:	4c          	inca
    e1f7:	45          	.byte	0x45
    e1f8:	56          	rorb
    e1f9:	32          	pula
    e1fa:	20 50       	bra	0x0xe24c
    e1fc:	57          	asrb
    e1fd:	31          	ins
    e1fe:	20 50       	bra	0x0xe250
    e200:	57          	asrb
    e201:	32          	pula
    e202:	31          	ins
    e203:	26 32       	bne	0x0xe237
    e205:	50          	negb
    e206:	46          	rora
    e207:	49          	rola
    e208:	4c          	inca
    e209:	54          	lsrb
    e20a:	52          	.byte	0x52
    e20b:	45          	.byte	0x45
    e20c:	53          	comb
    e20d:	4f          	clra
    e20e:	4c          	inca
    e20f:	45          	.byte	0x45
    e210:	56          	rorb
    e211:	4e          	.byte	0x4e
    e212:	58          	aslb
    e213:	4d          	tsta
    e214:	4f          	clra
    e215:	44          	lsra
    e216:	20 45       	bra	0x0xe25d
    e218:	41          	.byte	0x41
    e219:	31          	ins
    e21a:	20 45       	bra	0x0xe261
    e21c:	41          	.byte	0x41
    e21d:	33          	pulb
    e21e:	20 45       	bra	0x0xe265
    e220:	58          	aslb
    e221:	54          	lsrb
    e222:	20 56       	bra	0x0xe27a
    e224:	4f          	clra
    e225:	4c          	inca
    e226:	bd e9 6d    	jsr	0xe96d
    e229:	ce 01 20    	ldx	#0x120
    e22c:	18 ce e2 8b 	ldy	#0xe28b
    e230:	c6 20       	ldab	#0x20
    e232:	18 a6 00    	ldaa	0x0,y
    e235:	a7 00       	staa	0x0,x
    e237:	08          	inx
    e238:	18 08       	iny
    e23a:	5a          	decb
    e23b:	26 f5       	bne	0x0xe232
    e23d:	ce e2 ab    	ldx	#0xe2ab
    e240:	18 ce 01 30 	ldy	#0x130
    e244:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe24f
    e248:	d6 27       	ldab	*0x27
    e24a:	bd ea d3    	jsr	0xead3
    e24d:	20 05       	bra	0x0xe254
    e24f:	d6 34       	ldab	*0x34
    e251:	bd ea d3    	jsr	0xead3
    e254:	ce e2 c3    	ldx	#0xe2c3
    e257:	18 08       	iny
    e259:	18 08       	iny
    e25b:	18 08       	iny
    e25d:	12 f4 02 09 	brset	*0xf4, #0x02, 0x0xe26a
    e261:	d6 31       	ldab	*0x31
    e263:	c4 01       	andb	#0x1
    e265:	bd ea d3    	jsr	0xead3
    e268:	20 06       	bra	0x0xe270
    e26a:	d6 31       	ldab	*0x31
    e26c:	54          	lsrb
    e26d:	bd ea d3    	jsr	0xead3
    e270:	ce e2 cb    	ldx	#0xe2cb
    e273:	18 08       	iny
    e275:	18 08       	iny
    e277:	12 f4 02 08 	brset	*0xf4, #0x02, 0x0xe283
    e27b:	d6 25       	ldab	*0x25
    e27d:	bd ea d3    	jsr	0xead3
    e280:	7e e8 aa    	jmp	0xe8aa
    e283:	d6 32       	ldab	*0x32
    e285:	bd ea d3    	jsr	0xead3
    e288:	7e e8 aa    	jmp	0xe8aa
    e28b:	57          	asrb
    e28c:	41          	.byte	0x41
    e28d:	56          	rorb
    e28e:	45          	.byte	0x45
    e28f:	20 20       	bra	0x0xe2b1
    e291:	4d          	tsta
    e292:	4f          	clra
    e293:	44          	lsra
    e294:	45          	.byte	0x45
    e295:	20 53       	bra	0x0xe2ea
    e297:	59          	rolb
    e298:	4e          	.byte	0x4e
    e299:	43          	coma
    e29a:	20 20       	bra	0x0xe2bc
    e29c:	20 20       	bra	0x0xe2be
    e29e:	20 20       	bra	0x0xe2c0
    e2a0:	20 20       	bra	0x0xe2c2
    e2a2:	20 20       	bra	0x0xe2c4
    e2a4:	20 20       	bra	0x0xe2c6
    e2a6:	20 20       	bra	0x0xe2c8
    e2a8:	20 20       	bra	0x0xe2ca
    e2aa:	20 20       	bra	0x0xe2cc
    e2ac:	54          	lsrb
    e2ad:	52          	.byte	0x52
    e2ae:	49          	rola
    e2af:	20 53       	bra	0x0xe304
    e2b1:	51          	.byte	0x51
    e2b2:	52          	.byte	0x52
    e2b3:	53          	comb
    e2b4:	57          	asrb
    e2b5:	55          	.byte	0x55
    e2b6:	50          	negb
    e2b7:	53          	comb
    e2b8:	57          	asrb
    e2b9:	44          	lsra
    e2ba:	4e          	.byte	0x4e
    e2bb:	52          	.byte	0x52
    e2bc:	41          	.byte	0x41
    e2bd:	4e          	.byte	0x4e
    e2be:	44          	lsra
    e2bf:	20 53       	bra	0x0xe314
    e2c1:	2f 48       	ble	0x0xe30b
    e2c3:	4d          	tsta
    e2c4:	4f          	clra
    e2c5:	4e          	.byte	0x4e
    e2c6:	4f          	clra
    e2c7:	50          	negb
    e2c8:	4f          	clra
    e2c9:	4c          	inca
    e2ca:	59          	rolb
    e2cb:	53          	comb
    e2cc:	45          	.byte	0x45
    e2cd:	4c          	inca
    e2ce:	46          	rora
    e2cf:	20 20       	bra	0x0xe2f1
    e2d1:	20 34       	bra	0x0xe307
    e2d3:	20 20       	bra	0x0xe2f5
    e2d5:	20 32       	bra	0x0xe309
    e2d7:	20 20       	bra	0x0xe2f9
    e2d9:	20 31       	bra	0x0xe30c
    e2db:	20 20       	bra	0x0xe2fd
    e2dd:	31          	ins
    e2de:	54          	lsrb
    e2df:	20 31       	bra	0x0xe312
    e2e1:	2f 32       	ble	0x0xe315
    e2e3:	31          	ins
    e2e4:	2f 32       	ble	0x0xe318
    e2e6:	54          	lsrb
    e2e7:	20 31       	bra	0x0xe31a
    e2e9:	2f 34       	ble	0x0xe31f
    e2eb:	31          	ins
    e2ec:	2f 34       	ble	0x0xe322
    e2ee:	54          	lsrb
    e2ef:	20 31       	bra	0x0xe322
    e2f1:	2f 38       	ble	0x0xe32b
    e2f3:	31          	ins
    e2f4:	2f 38       	ble	0x0xe32e
    e2f6:	54          	lsrb
    e2f7:	31          	ins
    e2f8:	2f 31       	ble	0x0xe32b
    e2fa:	36          	psha
    e2fb:	20 31       	bra	0x0xe32e
    e2fd:	36          	psha
    e2fe:	54          	lsrb
    e2ff:	bd e9 6d    	jsr	0xe96d
    e302:	ce 01 20    	ldx	#0x120
    e305:	18 ce e3 3c 	ldy	#0xe33c
    e309:	c6 20       	ldab	#0x20
    e30b:	18 a6 00    	ldaa	0x0,y
    e30e:	a7 00       	staa	0x0,x
    e310:	08          	inx
    e311:	18 08       	iny
    e313:	5a          	decb
    e314:	26 f5       	bne	0x0xe30b
    e316:	ce e3 65    	ldx	#0xe365
    e319:	18 ce 01 37 	ldy	#0x137
    e31d:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe328
    e321:	d6 26       	ldab	*0x26
    e323:	bd ea bf    	jsr	0xeabf
    e326:	20 05       	bra	0x0xe32d
    e328:	d6 33       	ldab	*0x33
    e32a:	bd ea bf    	jsr	0xeabf
    e32d:	d6 96       	ldab	*0x96
    e32f:	ce e3 74    	ldx	#0xe374
    e332:	18 ce 01 3c 	ldy	#0x13c
    e336:	bd ea bf    	jsr	0xeabf
    e339:	7e e8 aa    	jmp	0xe8aa
    e33c:	20 20       	bra	0x0xe35e
    e33e:	20 20       	bra	0x0xe360
    e340:	20 20       	bra	0x0xe362
    e342:	20 4b       	bra	0x0xe38f
    e344:	45          	.byte	0x45
    e345:	59          	rolb
    e346:	20 20       	bra	0x0xe368
    e348:	51          	.byte	0x51
    e349:	55          	.byte	0x55
    e34a:	41          	.byte	0x41
    e34b:	4e          	.byte	0x4e
    e34c:	20 20       	bra	0x0xe36e
    e34e:	20 20       	bra	0x0xe370
    e350:	20 20       	bra	0x0xe372
    e352:	20 20       	bra	0x0xe374
    e354:	20 20       	bra	0x0xe376
    e356:	20 20       	bra	0x0xe378
    e358:	20 20       	bra	0x0xe37a
    e35a:	20 20       	bra	0x0xe37c
    e35c:	4c          	inca
    e35d:	4f          	clra
    e35e:	57          	asrb
    e35f:	4d          	tsta
    e360:	45          	.byte	0x45
    e361:	44          	lsra
    e362:	20 48       	bra	0x0xe3ac
    e364:	49          	rola
    e365:	4f          	clra
    e366:	46          	rora
    e367:	46          	rora
    e368:	20 55       	bra	0x0xe3bf
    e36a:	50          	negb
    e36b:	20 44       	bra	0x0xe3b1
    e36d:	4e          	.byte	0x4e
    e36e:	55          	.byte	0x55
    e36f:	50          	negb
    e370:	31          	ins
    e371:	44          	lsra
    e372:	4e          	.byte	0x4e
    e373:	31          	ins
    e374:	4f          	clra
    e375:	46          	rora
    e376:	46          	rora
    e377:	4c          	inca
    e378:	46          	rora
    e379:	31          	ins
    e37a:	4c          	inca
    e37b:	46          	rora
    e37c:	32          	pula
    e37d:	31          	ins
    e37e:	26 32       	bne	0x0xe3b2
    e380:	7d 01 1d    	tst	0x11d
    e383:	26 0a       	bne	0x0xe38f
    e385:	bd ea 97    	jsr	0xea97
    e388:	ce 10 23    	ldx	#0x1023
    e38b:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe38b
    e38f:	bd e9 6d    	jsr	0xe96d
    e392:	86 20       	ldaa	#0x20
    e394:	c6 20       	ldab	#0x20
    e396:	ce 01 20    	ldx	#0x120
    e399:	a7 00       	staa	0x0,x
    e39b:	08          	inx
    e39c:	5a          	decb
    e39d:	26 fa       	bne	0x0xe399
    e39f:	ce 01 20    	ldx	#0x120
    e3a2:	18 ce 00 b0 	ldy	#0xb0
    e3a6:	96 f9       	ldaa	*0xf9
    e3a8:	81 04       	cmpa	#0x4
    e3aa:	26 04       	bne	0x0xe3b0
    e3ac:	18 ce 50 30 	ldy	#0x5030
    e3b0:	18 a6 00    	ldaa	0x0,y
    e3b3:	bd ea 5c    	jsr	0xea5c
    e3b6:	08          	inx
    e3b7:	08          	inx
    e3b8:	08          	inx
    e3b9:	08          	inx
    e3ba:	8c 01 2f    	cpx	#0x12f
    e3bd:	26 01       	bne	0x0xe3c0
    e3bf:	08          	inx
    e3c0:	18 08       	iny
    e3c2:	8c 01 3f    	cpx	#0x13f
    e3c5:	25 e9       	bcs	0x0xe3b0
    e3c7:	7e e8 aa    	jmp	0xe8aa
    e3ca:	bd e9 6d    	jsr	0xe96d
    e3cd:	86 20       	ldaa	#0x20
    e3cf:	c6 20       	ldab	#0x20
    e3d1:	ce 01 20    	ldx	#0x120
    e3d4:	a7 00       	staa	0x0,x
    e3d6:	08          	inx
    e3d7:	5a          	decb
    e3d8:	26 fa       	bne	0x0xe3d4
    e3da:	ce 01 20    	ldx	#0x120
    e3dd:	18 ce 00 a8 	ldy	#0xa8
    e3e1:	96 f9       	ldaa	*0xf9
    e3e3:	81 04       	cmpa	#0x4
    e3e5:	26 04       	bne	0x0xe3eb
    e3e7:	18 ce 50 28 	ldy	#0x5028
    e3eb:	18 a6 00    	ldaa	0x0,y
    e3ee:	bd ea 20    	jsr	0xea20
    e3f1:	08          	inx
    e3f2:	08          	inx
    e3f3:	8c 01 2f    	cpx	#0x12f
    e3f6:	26 01       	bne	0x0xe3f9
    e3f8:	08          	inx
    e3f9:	18 08       	iny
    e3fb:	8c 01 3f    	cpx	#0x13f
    e3fe:	25 eb       	bcs	0x0xe3eb
    e400:	7e e8 aa    	jmp	0xe8aa
    e403:	bd e9 6d    	jsr	0xe96d
    e406:	86 20       	ldaa	#0x20
    e408:	c6 20       	ldab	#0x20
    e40a:	ce 01 20    	ldx	#0x120
    e40d:	a7 00       	staa	0x0,x
    e40f:	08          	inx
    e410:	5a          	decb
    e411:	26 fa       	bne	0x0xe40d
    e413:	ce 01 20    	ldx	#0x120
    e416:	18 ce 00 b8 	ldy	#0xb8
    e41a:	96 f9       	ldaa	*0xf9
    e41c:	81 04       	cmpa	#0x4
    e41e:	26 04       	bne	0x0xe424
    e420:	18 ce 50 38 	ldy	#0x5038
    e424:	18 a6 00    	ldaa	0x0,y
    e427:	bd ea 20    	jsr	0xea20
    e42a:	08          	inx
    e42b:	08          	inx
    e42c:	8c 01 2f    	cpx	#0x12f
    e42f:	26 01       	bne	0x0xe432
    e431:	08          	inx
    e432:	18 08       	iny
    e434:	8c 01 3f    	cpx	#0x13f
    e437:	25 eb       	bcs	0x0xe424
    e439:	7e e8 aa    	jmp	0xe8aa
    e43c:	bd e9 6d    	jsr	0xe96d
    e43f:	ce 01 20    	ldx	#0x120
    e442:	18 ce e4 7a 	ldy	#0xe47a
    e446:	c6 20       	ldab	#0x20
    e448:	18 a6 00    	ldaa	0x0,y
    e44b:	a7 00       	staa	0x0,x
    e44d:	08          	inx
    e44e:	18 08       	iny
    e450:	5a          	decb
    e451:	26 f5       	bne	0x0xe448
    e453:	ce e2 ab    	ldx	#0xe2ab
    e456:	d6 a7       	ldab	*0xa7
    e458:	18 ce 01 30 	ldy	#0x130
    e45c:	bd ea d3    	jsr	0xead3
    e45f:	ce e3 65    	ldx	#0xe365
    e462:	d6 a6       	ldab	*0xa6
    e464:	18 ce 01 37 	ldy	#0x137
    e468:	bd ea bf    	jsr	0xeabf
    e46b:	ce e2 cb    	ldx	#0xe2cb
    e46e:	d6 a5       	ldab	*0xa5
    e470:	18 ce 01 3b 	ldy	#0x13b
    e474:	bd ea d3    	jsr	0xead3
    e477:	7e e8 aa    	jmp	0xe8aa
    e47a:	57          	asrb
    e47b:	41          	.byte	0x41
    e47c:	56          	rorb
    e47d:	45          	.byte	0x45
    e47e:	20 20       	bra	0x0xe4a0
    e480:	20 4b       	bra	0x0xe4cd
    e482:	45          	.byte	0x45
    e483:	59          	rolb
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
    e499:	20 bd       	bra	0x0xe458
    e49b:	e9 6d       	adcb	0x6d,x
    e49d:	ce 01 20    	ldx	#0x120
    e4a0:	18 ce e4 f0 	ldy	#0xe4f0
    e4a4:	c6 20       	ldab	#0x20
    e4a6:	18 a6 00    	ldaa	0x0,y
    e4a9:	a7 00       	staa	0x0,x
    e4ab:	08          	inx
    e4ac:	18 08       	iny
    e4ae:	5a          	decb
    e4af:	26 f5       	bne	0x0xe4a6
    e4b1:	ce d9 74    	ldx	#0xd974
    e4b4:	18 ce 01 30 	ldy	#0x130
    e4b8:	13 21 02 07 	brclr	*0x21, #0x02, 0x0xe4c3
    e4bc:	c6 01       	ldab	#0x1
    e4be:	bd ea bf    	jsr	0xeabf
    e4c1:	20 04       	bra	0x0xe4c7
    e4c3:	5f          	clrb
    e4c4:	bd ea bf    	jsr	0xeabf
    e4c7:	18 ce 01 34 	ldy	#0x134
    e4cb:	ce d9 74    	ldx	#0xd974
    e4ce:	12 21 04 06 	brset	*0x21, #0x04, 0x0xe4d8
    e4d2:	5f          	clrb
    e4d3:	bd ea bf    	jsr	0xeabf
    e4d6:	20 05       	bra	0x0xe4dd
    e4d8:	c6 01       	ldab	#0x1
    e4da:	bd ea bf    	jsr	0xeabf
    e4dd:	96 22       	ldaa	*0x22
    e4df:	ce 01 38    	ldx	#0x138
    e4e2:	bd ea 5c    	jsr	0xea5c
    e4e5:	96 23       	ldaa	*0x23
    e4e7:	ce 01 3c    	ldx	#0x13c
    e4ea:	bd ea 20    	jsr	0xea20
    e4ed:	7e e8 aa    	jmp	0xe8aa
    e4f0:	47          	asra
    e4f1:	4c          	inca
    e4f2:	53          	comb
    e4f3:	20 41       	bra	0x0xe536
    e4f5:	55          	.byte	0x55
    e4f6:	54          	lsrb
    e4f7:	20 49       	bra	0x0xe542
    e4f9:	4e          	.byte	0x4e
    e4fa:	54          	lsrb
    e4fb:	20 44       	bra	0x0xe541
    e4fd:	59          	rolb
    e4fe:	4e          	.byte	0x4e
    e4ff:	20 20       	bra	0x0xe521
    e501:	20 20       	bra	0x0xe523
    e503:	20 20       	bra	0x0xe525
    e505:	20 20       	bra	0x0xe527
    e507:	20 20       	bra	0x0xe529
    e509:	20 20       	bra	0x0xe52b
    e50b:	20 20       	bra	0x0xe52d
    e50d:	20 20       	bra	0x0xe52f
    e50f:	20 7d       	bra	0x0xe58e
    e511:	01          	nop
    e512:	1d 26 0a    	bclr	0x26,x, #0x0a
    e515:	bd ea 97    	jsr	0xea97
    e518:	ce 10 23    	ldx	#0x1023
    e51b:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe51b
    e51f:	bd e9 6d    	jsr	0xe96d
    e522:	ce 01 20    	ldx	#0x120
    e525:	18 ce e5 44 	ldy	#0xe544
    e529:	c6 20       	ldab	#0x20
    e52b:	18 a6 00    	ldaa	0x0,y
    e52e:	a7 00       	staa	0x0,x
    e530:	08          	inx
    e531:	18 08       	iny
    e533:	5a          	decb
    e534:	26 f5       	bne	0x0xe52b
    e536:	ce d7 04    	ldx	#0xd704
    e539:	5f          	clrb
    e53a:	18 ce 01 31 	ldy	#0x131
    e53e:	bd ea bf    	jsr	0xeabf
    e541:	7e e8 aa    	jmp	0xe8aa
    e544:	20 54       	bra	0x0xe59a
    e546:	55          	.byte	0x55
    e547:	4e          	.byte	0x4e
    e548:	45          	.byte	0x45
    e549:	3f          	swi
    e54a:	20 20       	bra	0x0xe56c
    e54c:	20 20       	bra	0x0xe56e
    e54e:	20 20       	bra	0x0xe570
    e550:	20 20       	bra	0x0xe572
    e552:	20 20       	bra	0x0xe574
    e554:	20 20       	bra	0x0xe576
    e556:	20 20       	bra	0x0xe578
    e558:	20 20       	bra	0x0xe57a
    e55a:	20 20       	bra	0x0xe57c
    e55c:	20 20       	bra	0x0xe57e
    e55e:	20 20       	bra	0x0xe580
    e560:	20 20       	bra	0x0xe582
    e562:	20 20       	bra	0x0xe584
    e564:	7d 01 1d    	tst	0x11d
    e567:	26 0a       	bne	0x0xe573
    e569:	bd ea 97    	jsr	0xea97
    e56c:	ce 10 23    	ldx	#0x1023
    e56f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe56f
    e573:	bd e9 6d    	jsr	0xe96d
    e576:	ce 01 20    	ldx	#0x120
    e579:	18 ce e5 9c 	ldy	#0xe59c
    e57d:	c6 20       	ldab	#0x20
    e57f:	18 a6 00    	ldaa	0x0,y
    e582:	a7 00       	staa	0x0,x
    e584:	08          	inx
    e585:	18 08       	iny
    e587:	5a          	decb
    e588:	26 f5       	bne	0x0xe57f
    e58a:	f6 10 28    	ldab	0x1028
    e58d:	c4 03       	andb	#0x3
    e58f:	ce e5 bc    	ldx	#0xe5bc
    e592:	18 ce 01 30 	ldy	#0x130
    e596:	bd ea d3    	jsr	0xead3
    e599:	7e e8 aa    	jmp	0xe8aa
    e59c:	53          	comb
    e59d:	45          	.byte	0x45
    e59e:	54          	lsrb
    e59f:	20 53       	bra	0x0xe5f4
    e5a1:	50          	negb
    e5a2:	49          	rola
    e5a3:	20 42       	bra	0x0xe5e7
    e5a5:	49          	rola
    e5a6:	54          	lsrb
    e5a7:	20 52       	bra	0x0xe5fb
    e5a9:	41          	.byte	0x41
    e5aa:	54          	lsrb
    e5ab:	45          	.byte	0x45
    e5ac:	20 20       	bra	0x0xe5ce
    e5ae:	20 20       	bra	0x0xe5d0
    e5b0:	20 4b       	bra	0x0xe5fd
    e5b2:	42          	.byte	0x42
    e5b3:	49          	rola
    e5b4:	54          	lsrb
    e5b5:	2f 53       	ble	0x0xe60a
    e5b7:	45          	.byte	0x45
    e5b8:	43          	coma
    e5b9:	20 20       	bra	0x0xe5db
    e5bb:	20 20       	bra	0x0xe5dd
    e5bd:	20 31       	bra	0x0xe5f0
    e5bf:	4b          	.byte	0x4b
    e5c0:	20 35       	bra	0x0xe5f7
    e5c2:	30          	tsx
    e5c3:	30          	tsx
    e5c4:	20 31       	bra	0x0xe5f7
    e5c6:	32          	pula
    e5c7:	35          	txs
    e5c8:	36          	psha
    e5c9:	32          	pula
    e5ca:	2e 35       	bgt	0x0xe601
    e5cc:	7d 01 1d    	tst	0x11d
    e5cf:	26 0a       	bne	0x0xe5db
    e5d1:	bd ea 97    	jsr	0xea97
    e5d4:	ce 10 23    	ldx	#0x1023
    e5d7:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe5d7
    e5db:	bd e9 6d    	jsr	0xe96d
    e5de:	ce 01 20    	ldx	#0x120
    e5e1:	18 ce e5 fe 	ldy	#0xe5fe
    e5e5:	c6 20       	ldab	#0x20
    e5e7:	18 a6 00    	ldaa	0x0,y
    e5ea:	a7 00       	staa	0x0,x
    e5ec:	08          	inx
    e5ed:	18 08       	iny
    e5ef:	5a          	decb
    e5f0:	26 f5       	bne	0x0xe5e7
    e5f2:	96 f0       	ldaa	*0xf0
    e5f4:	4c          	inca
    e5f5:	ce 01 31    	ldx	#0x131
    e5f8:	bd ea 20    	jsr	0xea20
    e5fb:	7e e8 aa    	jmp	0xe8aa
    e5fe:	53          	comb
    e5ff:	45          	.byte	0x45
    e600:	54          	lsrb
    e601:	20 23       	bra	0x0xe626
    e603:	20 4f       	bra	0x0xe654
    e605:	46          	rora
    e606:	20 56       	bra	0x0xe65e
    e608:	4f          	clra
    e609:	49          	rola
    e60a:	43          	coma
    e60b:	45          	.byte	0x45
    e60c:	53          	comb
    e60d:	20 20       	bra	0x0xe62f
    e60f:	20 20       	bra	0x0xe631
    e611:	20 20       	bra	0x0xe633
    e613:	20 20       	bra	0x0xe635
    e615:	20 20       	bra	0x0xe637
    e617:	20 20       	bra	0x0xe639
    e619:	20 20       	bra	0x0xe63b
    e61b:	20 20       	bra	0x0xe63d
    e61d:	20 7d       	bra	0x0xe69c
    e61f:	01          	nop
    e620:	1d 26 0a    	bclr	0x26,x, #0x0a
    e623:	bd ea 97    	jsr	0xea97
    e626:	ce 10 23    	ldx	#0x1023
    e629:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe629
    e62d:	bd e9 6d    	jsr	0xe96d
    e630:	ce 01 20    	ldx	#0x120
    e633:	18 ce e6 51 	ldy	#0xe651
    e637:	c6 20       	ldab	#0x20
    e639:	18 a6 00    	ldaa	0x0,y
    e63c:	a7 00       	staa	0x0,x
    e63e:	08          	inx
    e63f:	18 08       	iny
    e641:	5a          	decb
    e642:	26 f5       	bne	0x0xe639
    e644:	96 de       	ldaa	*0xde
    e646:	4c          	inca
    e647:	4c          	inca
    e648:	ce 01 31    	ldx	#0x131
    e64b:	bd ea 20    	jsr	0xea20
    e64e:	7e e8 aa    	jmp	0xe8aa
    e651:	53          	comb
    e652:	45          	.byte	0x45
    e653:	54          	lsrb
    e654:	20 23       	bra	0x0xe679
    e656:	20 4f       	bra	0x0xe6a7
    e658:	46          	rora
    e659:	20 46       	bra	0x0xe6a1
    e65b:	49          	rola
    e65c:	4c          	inca
    e65d:	54          	lsrb
    e65e:	45          	.byte	0x45
    e65f:	52          	.byte	0x52
    e660:	53          	comb
    e661:	20 20       	bra	0x0xe683
    e663:	20 20       	bra	0x0xe685
    e665:	20 20       	bra	0x0xe687
    e667:	20 20       	bra	0x0xe689
    e669:	20 20       	bra	0x0xe68b
    e66b:	20 20       	bra	0x0xe68d
    e66d:	20 20       	bra	0x0xe68f
    e66f:	20 20       	bra	0x0xe691
    e671:	7d 01 1d    	tst	0x11d
    e674:	26 0a       	bne	0x0xe680
    e676:	bd ea 97    	jsr	0xea97
    e679:	ce 10 23    	ldx	#0x1023
    e67c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe67c
    e680:	bd e9 6d    	jsr	0xe96d
    e683:	ce 01 20    	ldx	#0x120
    e686:	18 ce e7 5b 	ldy	#0xe75b
    e68a:	c6 20       	ldab	#0x20
    e68c:	b6 50 00    	ldaa	0x5000
    e68f:	81 06       	cmpa	#0x6
    e691:	25 04       	bcs	0x0xe697
    e693:	18 ce e7 7b 	ldy	#0xe77b
    e697:	18 a6 00    	ldaa	0x0,y
    e69a:	a7 00       	staa	0x0,x
    e69c:	08          	inx
    e69d:	18 08       	iny
    e69f:	5a          	decb
    e6a0:	26 f5       	bne	0x0xe697
    e6a2:	b6 01 6b    	ldaa	0x16b
    e6a5:	8b 31       	adda	#0x31
    e6a7:	b7 01 25    	staa	0x125
    e6aa:	ce 50 01    	ldx	#0x5001
    e6ad:	b6 01 6b    	ldaa	0x16b
    e6b0:	c6 04       	ldab	#0x4
    e6b2:	3d          	mul
    e6b3:	3a          	abx
    e6b4:	a6 00       	ldaa	0x0,x
    e6b6:	8b 41       	adda	#0x41
    e6b8:	b7 01 32    	staa	0x132
    e6bb:	a6 01       	ldaa	0x1,x
    e6bd:	3c          	pshx
    e6be:	4c          	inca
    e6bf:	ce 01 34    	ldx	#0x134
    e6c2:	bd ea 20    	jsr	0xea20
    e6c5:	b6 50 00    	ldaa	0x5000
    e6c8:	81 06       	cmpa	#0x6
    e6ca:	27 0c       	beq	0x0xe6d8
    e6cc:	38          	pulx
    e6cd:	a6 03       	ldaa	0x3,x
    e6cf:	3c          	pshx
    e6d0:	ce 01 3c    	ldx	#0x13c
    e6d3:	bd ea 20    	jsr	0xea20
    e6d6:	20 42       	bra	0x0xe71a
    e6d8:	38          	pulx
    e6d9:	7d 01 6b    	tst	0x16b
    e6dc:	26 06       	bne	0x0xe6e4
    e6de:	96 f0       	ldaa	*0xf0
    e6e0:	4c          	inca
    e6e1:	b7 50 23    	staa	0x5023
    e6e4:	e6 02       	ldab	0x2,x
    e6e6:	c4 40       	andb	#0x40
    e6e8:	f7 01 7d    	stab	0x17d
    e6eb:	e6 02       	ldab	0x2,x
    e6ed:	c4 3f       	andb	#0x3f
    e6ef:	b6 50 23    	ldaa	0x5023
    e6f2:	11          	cba
    e6f3:	24 03       	bcc	0x0xe6f8
    e6f5:	5a          	decb
    e6f6:	20 fa       	bra	0x0xe6f2
    e6f8:	10          	sba
    e6f9:	b7 50 23    	staa	0x5023
    e6fc:	37          	pshb
    e6fd:	fa 01 7d    	orab	0x17d
    e700:	e7 02       	stab	0x2,x
    e702:	32          	pula
    e703:	3c          	pshx
    e704:	ce 01 38    	ldx	#0x138
    e707:	bd ea 20    	jsr	0xea20
    e70a:	38          	pulx
    e70b:	e6 03       	ldab	0x3,x
    e70d:	c4 01       	andb	#0x1
    e70f:	3c          	pshx
    e710:	ce e7 9b    	ldx	#0xe79b
    e713:	18 ce 01 3c 	ldy	#0x13c
    e717:	bd ea bf    	jsr	0xeabf
    e71a:	38          	pulx
    e71b:	e6 00       	ldab	0x0,x
    e71d:	c1 01       	cmpb	#0x1
    e71f:	23 17       	bls	0x0xe738
    e721:	a6 01       	ldaa	0x1,x
    e723:	d7 f9       	stab	*0xf9
    e725:	bd ab 71    	jsr	0xab71
    e728:	c6 04       	ldab	#0x4
    e72a:	d7 f9       	stab	*0xf9
    e72c:	bd a2 d0    	jsr	0xa2d0
    e72f:	bd a2 eb    	jsr	0xa2eb
    e732:	bd ab 71    	jsr	0xab71
    e735:	7e e8 aa    	jmp	0xe8aa
    e738:	4f          	clra
    e739:	b7 10 22    	staa	0x1022
    e73c:	b6 10 2d    	ldaa	0x102d
    e73f:	36          	psha
    e740:	84 7f       	anda	#0x7f
    e742:	b7 10 2d    	staa	0x102d
    e745:	a6 00       	ldaa	0x0,x
    e747:	e6 01       	ldab	0x1,x
    e749:	bd 79 00    	jsr	0x7900
    e74c:	32          	pula
    e74d:	b7 10 2d    	staa	0x102d
    e750:	86 80       	ldaa	#0x80
    e752:	b7 10 22    	staa	0x1022
    e755:	bd a2 eb    	jsr	0xa2eb
    e758:	7e e8 aa    	jmp	0xe8aa
    e75b:	50          	negb
    e75c:	41          	.byte	0x41
    e75d:	54          	lsrb
    e75e:	43          	coma
    e75f:	48          	asla
    e760:	20 20       	bra	0x0xe782
    e762:	20 20       	bra	0x0xe784
    e764:	20 20       	bra	0x0xe786
    e766:	20 56       	bra	0x0xe7be
    e768:	4f          	clra
    e769:	4c          	inca
    e76a:	20 20       	bra	0x0xe78c
    e76c:	20 20       	bra	0x0xe78e
    e76e:	20 20       	bra	0x0xe790
    e770:	20 20       	bra	0x0xe792
    e772:	20 20       	bra	0x0xe794
    e774:	20 20       	bra	0x0xe796
    e776:	20 20       	bra	0x0xe798
    e778:	20 20       	bra	0x0xe79a
    e77a:	20 50       	bra	0x0xe7cc
    e77c:	41          	.byte	0x41
    e77d:	54          	lsrb
    e77e:	43          	coma
    e77f:	48          	asla
    e780:	20 20       	bra	0x0xe7a2
    e782:	23 56       	bls	0x0xe7da
    e784:	43          	coma
    e785:	53          	comb
    e786:	20 54       	bra	0x0xe7dc
    e788:	59          	rolb
    e789:	50          	negb
    e78a:	45          	.byte	0x45
    e78b:	20 20       	bra	0x0xe7ad
    e78d:	20 20       	bra	0x0xe7af
    e78f:	20 20       	bra	0x0xe7b1
    e791:	20 20       	bra	0x0xe7b3
    e793:	20 20       	bra	0x0xe7b5
    e795:	20 20       	bra	0x0xe7b7
    e797:	20 20       	bra	0x0xe7b9
    e799:	20 20       	bra	0x0xe7bb
    e79b:	4d          	tsta
    e79c:	4f          	clra
    e79d:	4e          	.byte	0x4e
    e79e:	50          	negb
    e79f:	4c          	inca
    e7a0:	59          	rolb
    e7a1:	7d 01 1d    	tst	0x11d
    e7a4:	26 0a       	bne	0x0xe7b0
    e7a6:	bd ea 97    	jsr	0xea97
    e7a9:	ce 10 23    	ldx	#0x1023
    e7ac:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe7ac
    e7b0:	bd e9 6d    	jsr	0xe96d
    e7b3:	ce 01 20    	ldx	#0x120
    e7b6:	18 ce e7 f2 	ldy	#0xe7f2
    e7ba:	c6 20       	ldab	#0x20
    e7bc:	18 a6 00    	ldaa	0x0,y
    e7bf:	a7 00       	staa	0x0,x
    e7c1:	08          	inx
    e7c2:	18 08       	iny
    e7c4:	5a          	decb
    e7c5:	26 f5       	bne	0x0xe7bc
    e7c7:	ce 50 03    	ldx	#0x5003
    e7ca:	b6 01 6b    	ldaa	0x16b
    e7cd:	c6 04       	ldab	#0x4
    e7cf:	3d          	mul
    e7d0:	3a          	abx
    e7d1:	7d 50 23    	tst	0x5023
    e7d4:	26 07       	bne	0x0xe7dd
    e7d6:	e6 00       	ldab	0x0,x
    e7d8:	c4 3f       	andb	#0x3f
    e7da:	f7 50 23    	stab	0x5023
    e7dd:	e6 00       	ldab	0x0,x
    e7df:	c4 40       	andb	#0x40
    e7e1:	27 02       	beq	0x0xe7e5
    e7e3:	c6 01       	ldab	#0x1
    e7e5:	18 ce 01 31 	ldy	#0x131
    e7e9:	ce e8 12    	ldx	#0xe812
    e7ec:	bd ea bf    	jsr	0xeabf
    e7ef:	7e e8 aa    	jmp	0xe8aa
    e7f2:	32          	pula
    e7f3:	4d          	tsta
    e7f4:	49          	rola
    e7f5:	58          	aslb
    e7f6:	20 20       	bra	0x0xe818
    e7f8:	20 20       	bra	0x0xe81a
    e7fa:	20 20       	bra	0x0xe81c
    e7fc:	20 20       	bra	0x0xe81e
    e7fe:	20 20       	bra	0x0xe820
    e800:	20 20       	bra	0x0xe822
    e802:	20 20       	bra	0x0xe824
    e804:	20 20       	bra	0x0xe826
    e806:	20 20       	bra	0x0xe828
    e808:	20 20       	bra	0x0xe82a
    e80a:	20 20       	bra	0x0xe82c
    e80c:	20 20       	bra	0x0xe82e
    e80e:	20 20       	bra	0x0xe830
    e810:	20 20       	bra	0x0xe832
    e812:	4f          	clra
    e813:	46          	rora
    e814:	46          	rora
    e815:	20 4f       	bra	0x0xe866
    e817:	4e          	.byte	0x4e
    e818:	ce 10 23    	ldx	#0x1023
    e81b:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe81b
    e81f:	bd e9 6d    	jsr	0xe96d
    e822:	ce 01 20    	ldx	#0x120
    e825:	18 ce e8 39 	ldy	#0xe839
    e829:	c6 20       	ldab	#0x20
    e82b:	18 a6 00    	ldaa	0x0,y
    e82e:	a7 00       	staa	0x0,x
    e830:	08          	inx
    e831:	18 08       	iny
    e833:	5a          	decb
    e834:	26 f5       	bne	0x0xe82b
    e836:	7e e8 aa    	jmp	0xe8aa
    e839:	55          	.byte	0x55
    e83a:	50          	negb
    e83b:	4c          	inca
    e83c:	4f          	clra
    e83d:	41          	.byte	0x41
    e83e:	44          	lsra
    e83f:	20 52       	bra	0x0xe893
    e841:	41          	.byte	0x41
    e842:	4d          	tsta
    e843:	20 42       	bra	0x0xe887
    e845:	4e          	.byte	0x4e
    e846:	4b          	.byte	0x4b
    e847:	20 43       	bra	0x0xe88c
    e849:	20 20       	bra	0x0xe86b
    e84b:	20 20       	bra	0x0xe86d
    e84d:	54          	lsrb
    e84e:	4f          	clra
    e84f:	20 52       	bra	0x0xe8a3
    e851:	4f          	clra
    e852:	4d          	tsta
    e853:	20 42       	bra	0x0xe897
    e855:	4e          	.byte	0x4e
    e856:	4b          	.byte	0x4b
    e857:	20 41       	bra	0x0xe89a
    e859:	b6 01 2f    	ldaa	0x12f
    e85c:	97 99       	staa	*0x99
    e85e:	b6 01 3f    	ldaa	0x13f
    e861:	97 9a       	staa	*0x9a
    e863:	ce 10 23    	ldx	#0x1023
    e866:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe866
    e86a:	bd e9 6d    	jsr	0xe96d
    e86d:	ce 01 20    	ldx	#0x120
    e870:	18 ce e8 8a 	ldy	#0xe88a
    e874:	c6 20       	ldab	#0x20
    e876:	18 a6 00    	ldaa	0x0,y
    e879:	a7 00       	staa	0x0,x
    e87b:	08          	inx
    e87c:	18 08       	iny
    e87e:	5a          	decb
    e87f:	26 f5       	bne	0x0xe876
    e881:	96 f2       	ldaa	*0xf2
    e883:	84 20       	anda	#0x20
    e885:	97 f2       	staa	*0xf2
    e887:	7e e8 aa    	jmp	0xe8aa
    e88a:	20 41       	bra	0x0xe8cd
    e88c:	52          	.byte	0x52
    e88d:	45          	.byte	0x45
    e88e:	20 59       	bra	0x0xe8e9
    e890:	4f          	clra
    e891:	55          	.byte	0x55
    e892:	20 53       	bra	0x0xe8e7
    e894:	55          	.byte	0x55
    e895:	52          	.byte	0x52
    e896:	45          	.byte	0x45
    e897:	20 3f       	bra	0x0xe8d8
    e899:	20 20       	bra	0x0xe8bb
    e89b:	20 20       	bra	0x0xe8bd
    e89d:	20 20       	bra	0x0xe8bf
    e89f:	20 20       	bra	0x0xe8c1
    e8a1:	20 20       	bra	0x0xe8c3
    e8a3:	20 20       	bra	0x0xe8c5
    e8a5:	20 20       	bra	0x0xe8c7
    e8a7:	4e          	.byte	0x4e
    e8a8:	4f          	clra
    e8a9:	20 7f       	bra	0x0xe92a
    e8ab:	01          	nop
    e8ac:	1e 86 20 b7 	brset	0x86,x, #0x20, 0x0xe867
    e8b0:	01          	nop
    e8b1:	1c ce 10    	bset	0xce,x, #0x10
    e8b4:	23 1f       	bls	0x0xe8d5
    e8b6:	00          	bgnd
    e8b7:	10          	sba
    e8b8:	fc 7e e8    	ldd	0x7ee8
    e8bb:	bc 8d 03    	cpx	0x8d03
    e8be:	7e d3 b7    	jmp	0xd3b7
    e8c1:	ce 01 20    	ldx	#0x120
    e8c4:	f6 01 1c    	ldab	0x11c
    e8c7:	5a          	decb
    e8c8:	3a          	abx
    e8c9:	a6 00       	ldaa	0x0,x
    e8cb:	b7 10 47    	staa	0x1047
    e8ce:	86 88       	ldaa	#0x88
    e8d0:	ba 10 00    	oraa	0x1000
    e8d3:	b7 10 00    	staa	0x1000
    e8d6:	88 80       	eora	#0x80
    e8d8:	b7 10 00    	staa	0x1000
    e8db:	88 08       	eora	#0x8
    e8dd:	b7 10 00    	staa	0x1000
    e8e0:	37          	pshb
    e8e1:	bd e9 39    	jsr	0xe939
    e8e4:	33          	pulb
    e8e5:	f7 01 1c    	stab	0x11c
    e8e8:	c1 10       	cmpb	#0x10
    e8ea:	27 01       	beq	0x0xe8ed
    e8ec:	39          	rts
    e8ed:	18 ce 10 23 	ldy	#0x1023
    e8f1:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xe8f1
    e8f5:	fb 
    e8f6:	86 8f       	ldaa	#0x8f
    e8f8:	b7 10 47    	staa	0x1047
    e8fb:	86 80       	ldaa	#0x80
    e8fd:	ba 10 00    	oraa	0x1000
    e900:	b7 10 00    	staa	0x1000
    e903:	88 80       	eora	#0x80
    e905:	b7 10 00    	staa	0x1000
    e908:	bd e9 39    	jsr	0xe939
    e90b:	39          	rts
    e90c:	f6 10 23    	ldab	0x1023
    e90f:	c5 10       	bitb	#0x10
    e911:	27 f9       	beq	0x0xe90c
    e913:	a6 00       	ldaa	0x0,x
    e915:	b7 10 47    	staa	0x1047
    e918:	86 88       	ldaa	#0x88
    e91a:	ba 10 00    	oraa	0x1000
    e91d:	b7 10 00    	staa	0x1000
    e920:	88 80       	eora	#0x80
    e922:	b7 10 00    	staa	0x1000
    e925:	88 08       	eora	#0x8
    e927:	b7 10 00    	staa	0x1000
    e92a:	86 10       	ldaa	#0x10
    e92c:	b7 10 23    	staa	0x1023
    e92f:	fc 10 0e    	ldd	0x100e
    e932:	c3 00 f0    	addd	#0xf0
    e935:	fd 10 1c    	std	0x101c
    e938:	39          	rts
    e939:	86 10       	ldaa	#0x10
    e93b:	b7 10 23    	staa	0x1023
    e93e:	fc 10 0e    	ldd	0x100e
    e941:	c3 00 f0    	addd	#0xf0
    e944:	fd 10 1c    	std	0x101c
    e947:	39          	rts
    e948:	f6 10 23    	ldab	0x1023
    e94b:	c5 10       	bitb	#0x10
    e94d:	27 f9       	beq	0x0xe948
    e94f:	b6 01 1e    	ldaa	0x11e
    e952:	81 0f       	cmpa	#0xf
    e954:	23 02       	bls	0x0xe958
    e956:	8b 30       	adda	#0x30
    e958:	8a 80       	oraa	#0x80
    e95a:	b7 10 47    	staa	0x1047
    e95d:	86 80       	ldaa	#0x80
    e95f:	ba 10 00    	oraa	0x1000
    e962:	b7 10 00    	staa	0x1000
    e965:	88 80       	eora	#0x80
    e967:	b7 10 00    	staa	0x1000
    e96a:	7e e9 39    	jmp	0xe939
    e96d:	ce 10 23    	ldx	#0x1023
    e970:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe970
    e974:	86 cf       	ldaa	#0xcf
    e976:	b7 10 47    	staa	0x1047
    e979:	86 80       	ldaa	#0x80
    e97b:	ba 10 00    	oraa	0x1000
    e97e:	b7 10 00    	staa	0x1000
    e981:	01          	nop
    e982:	88 80       	eora	#0x80
    e984:	b7 10 00    	staa	0x1000
    e987:	7e e9 39    	jmp	0xe939
    e98a:	b6 01 1e    	ldaa	0x11e
    e98d:	81 03       	cmpa	#0x3
    e98f:	22 04       	bhi	0x0xe995
    e991:	86 03       	ldaa	#0x3
    e993:	20 22       	bra	0x0xe9b7
    e995:	81 09       	cmpa	#0x9
    e997:	22 04       	bhi	0x0xe99d
    e999:	86 09       	ldaa	#0x9
    e99b:	20 1a       	bra	0x0xe9b7
    e99d:	81 0e       	cmpa	#0xe
    e99f:	22 04       	bhi	0x0xe9a5
    e9a1:	86 0e       	ldaa	#0xe
    e9a3:	20 12       	bra	0x0xe9b7
    e9a5:	81 13       	cmpa	#0x13
    e9a7:	22 04       	bhi	0x0xe9ad
    e9a9:	86 13       	ldaa	#0x13
    e9ab:	20 0a       	bra	0x0xe9b7
    e9ad:	81 19       	cmpa	#0x19
    e9af:	22 04       	bhi	0x0xe9b5
    e9b1:	86 19       	ldaa	#0x19
    e9b3:	20 02       	bra	0x0xe9b7
    e9b5:	86 1e       	ldaa	#0x1e
    e9b7:	b7 01 1e    	staa	0x11e
    e9ba:	7e e9 48    	jmp	0xe948
    e9bd:	86 02       	ldaa	#0x2
    e9bf:	b1 01 1e    	cmpa	0x11e
    e9c2:	23 06       	bls	0x0xe9ca
    e9c4:	b7 01 1e    	staa	0x11e
    e9c7:	7e e9 48    	jmp	0xe948
    e9ca:	86 06       	ldaa	#0x6
    e9cc:	b1 01 1e    	cmpa	0x11e
    e9cf:	23 06       	bls	0x0xe9d7
    e9d1:	b7 01 1e    	staa	0x11e
    e9d4:	7e e9 48    	jmp	0xe948
    e9d7:	86 0a       	ldaa	#0xa
    e9d9:	b1 01 1e    	cmpa	0x11e
    e9dc:	23 06       	bls	0x0xe9e4
    e9de:	b7 01 1e    	staa	0x11e
    e9e1:	7e e9 48    	jmp	0xe948
    e9e4:	86 0e       	ldaa	#0xe
    e9e6:	b1 01 1e    	cmpa	0x11e
    e9e9:	23 06       	bls	0x0xe9f1
    e9eb:	b7 01 1e    	staa	0x11e
    e9ee:	7e e9 48    	jmp	0xe948
    e9f1:	86 12       	ldaa	#0x12
    e9f3:	b1 01 1e    	cmpa	0x11e
    e9f6:	23 06       	bls	0x0xe9fe
    e9f8:	b7 01 1e    	staa	0x11e
    e9fb:	7e e9 48    	jmp	0xe948
    e9fe:	86 16       	ldaa	#0x16
    ea00:	b1 01 1e    	cmpa	0x11e
    ea03:	23 06       	bls	0x0xea0b
    ea05:	b7 01 1e    	staa	0x11e
    ea08:	7e e9 48    	jmp	0xe948
    ea0b:	86 1a       	ldaa	#0x1a
    ea0d:	b1 01 1e    	cmpa	0x11e
    ea10:	23 06       	bls	0x0xea18
    ea12:	b7 01 1e    	staa	0x11e
    ea15:	7e e9 48    	jmp	0xe948
    ea18:	86 1e       	ldaa	#0x1e
    ea1a:	b7 01 1e    	staa	0x11e
    ea1d:	7e e9 48    	jmp	0xe948
    ea20:	80 64       	suba	#0x64
    ea22:	24 09       	bcc	0x0xea2d
    ea24:	8b 64       	adda	#0x64
    ea26:	c6 20       	ldab	#0x20
    ea28:	e7 00       	stab	0x0,x
    ea2a:	08          	inx
    ea2b:	20 05       	bra	0x0xea32
    ea2d:	c6 31       	ldab	#0x31
    ea2f:	e7 00       	stab	0x0,x
    ea31:	08          	inx
    ea32:	80 0a       	suba	#0xa
    ea34:	24 14       	bcc	0x0xea4a
    ea36:	8b 0a       	adda	#0xa
    ea38:	c1 20       	cmpb	#0x20
    ea3a:	27 07       	beq	0x0xea43
    ea3c:	c6 30       	ldab	#0x30
    ea3e:	e7 00       	stab	0x0,x
    ea40:	08          	inx
    ea41:	20 14       	bra	0x0xea57
    ea43:	c6 20       	ldab	#0x20
    ea45:	e7 00       	stab	0x0,x
    ea47:	08          	inx
    ea48:	20 0d       	bra	0x0xea57
    ea4a:	5f          	clrb
    ea4b:	5c          	incb
    ea4c:	80 0a       	suba	#0xa
    ea4e:	24 fb       	bcc	0x0xea4b
    ea50:	8b 0a       	adda	#0xa
    ea52:	cb 30       	addb	#0x30
    ea54:	e7 00       	stab	0x0,x
    ea56:	08          	inx
    ea57:	8b 30       	adda	#0x30
    ea59:	a7 00       	staa	0x0,x
    ea5b:	39          	rts
    ea5c:	80 40       	suba	#0x40
    ea5e:	25 10       	bcs	0x0xea70
    ea60:	26 05       	bne	0x0xea67
    ea62:	8d bc       	bsr	0x0xea20
    ea64:	09          	dex
    ea65:	09          	dex
    ea66:	39          	rts
    ea67:	8d b7       	bsr	0x0xea20
    ea69:	09          	dex
    ea6a:	09          	dex
    ea6b:	86 2b       	ldaa	#0x2b
    ea6d:	a7 00       	staa	0x0,x
    ea6f:	39          	rts
    ea70:	40          	nega
    ea71:	8d ad       	bsr	0x0xea20
    ea73:	09          	dex
    ea74:	09          	dex
    ea75:	86 2d       	ldaa	#0x2d
    ea77:	a7 00       	staa	0x0,x
    ea79:	39          	rts
    ea7a:	a6 02       	ldaa	0x2,x
    ea7c:	80 30       	suba	#0x30
    ea7e:	e6 01       	ldab	0x1,x
    ea80:	c1 20       	cmpb	#0x20
    ea82:	26 01       	bne	0x0xea85
    ea84:	39          	rts
    ea85:	c0 30       	subb	#0x30
    ea87:	36          	psha
    ea88:	86 0a       	ldaa	#0xa
    ea8a:	3d          	mul
    ea8b:	32          	pula
    ea8c:	1b          	aba
    ea8d:	e6 00       	ldab	0x0,x
    ea8f:	c1 20       	cmpb	#0x20
    ea91:	26 01       	bne	0x0xea94
    ea93:	39          	rts
    ea94:	8b 64       	adda	#0x64
    ea96:	39          	rts
    ea97:	ce 10 23    	ldx	#0x1023
    ea9a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xea9a
    ea9e:	86 f7       	ldaa	#0xf7
    eaa0:	b4 10 00    	anda	0x1000
    eaa3:	b7 10 00    	staa	0x1000
    eaa6:	86 0e       	ldaa	#0xe
    eaa8:	b7 10 47    	staa	0x1047
    eaab:	86 80       	ldaa	#0x80
    eaad:	ba 10 00    	oraa	0x1000
    eab0:	b7 10 00    	staa	0x1000
    eab3:	01          	nop
    eab4:	88 80       	eora	#0x80
    eab6:	b7 10 00    	staa	0x1000
    eab9:	7c 01 1d    	inc	0x11d
    eabc:	7e e9 39    	jmp	0xe939
    eabf:	86 03       	ldaa	#0x3
    eac1:	3d          	mul
    eac2:	3a          	abx
    eac3:	c6 03       	ldab	#0x3
    eac5:	a6 00       	ldaa	0x0,x
    eac7:	18 a7 00    	staa	0x0,y
    eaca:	5a          	decb
    eacb:	27 05       	beq	0x0xead2
    eacd:	08          	inx
    eace:	18 08       	iny
    ead0:	20 f3       	bra	0x0xeac5
    ead2:	39          	rts
    ead3:	86 04       	ldaa	#0x4
    ead5:	3d          	mul
    ead6:	3a          	abx
    ead7:	c6 04       	ldab	#0x4
    ead9:	a6 00       	ldaa	0x0,x
    eadb:	18 a7 00    	staa	0x0,y
    eade:	5a          	decb
    eadf:	27 05       	beq	0x0xeae6
    eae1:	08          	inx
    eae2:	18 08       	iny
    eae4:	20 f3       	bra	0x0xead9
    eae6:	39          	rts
    eae7:	c6 c8       	ldab	#0xc8
    eae9:	3d          	mul
    eaea:	bd ea 20    	jsr	0xea20
    eaed:	86 25       	ldaa	#0x25
    eaef:	09          	dex
    eaf0:	09          	dex
    eaf1:	a7 00       	staa	0x0,x
    eaf3:	39          	rts
    eaf4:	b6 01 1e    	ldaa	0x11e
    eaf7:	81 0e       	cmpa	#0xe
    eaf9:	22 08       	bhi	0x0xeb03
    eafb:	8b 10       	adda	#0x10
    eafd:	b7 01 1e    	staa	0x11e
    eb00:	7e e9 48    	jmp	0xe948
    eb03:	80 10       	suba	#0x10
    eb05:	b7 01 1e    	staa	0x11e
    eb08:	7e e9 48    	jmp	0xe948
    eb0b:	b6 01 7f    	ldaa	0x17f
    eb0e:	2b 1e       	bmi	0x0xeb2e
    eb10:	b6 01 1e    	ldaa	0x11e
    eb13:	81 0e       	cmpa	#0xe
    eb15:	22 0a       	bhi	0x0xeb21
    eb17:	27 32       	beq	0x0xeb4b
    eb19:	86 0e       	ldaa	#0xe
    eb1b:	b7 01 1e    	staa	0x11e
    eb1e:	7e e9 48    	jmp	0xe948
    eb21:	81 1e       	cmpa	#0x1e
    eb23:	26 01       	bne	0x0xeb26
    eb25:	39          	rts
    eb26:	86 1e       	ldaa	#0x1e
    eb28:	b7 01 1e    	staa	0x11e
    eb2b:	7e e9 48    	jmp	0xe948
    eb2e:	b6 01 1e    	ldaa	0x11e
    eb31:	81 19       	cmpa	#0x19
    eb33:	25 0a       	bcs	0x0xeb3f
    eb35:	27 14       	beq	0x0xeb4b
    eb37:	86 19       	ldaa	#0x19
    eb39:	b7 01 1e    	staa	0x11e
    eb3c:	7e e9 48    	jmp	0xe948
    eb3f:	81 09       	cmpa	#0x9
    eb41:	27 08       	beq	0x0xeb4b
    eb43:	86 09       	ldaa	#0x9
    eb45:	b7 01 1e    	staa	0x11e
    eb48:	7e e9 48    	jmp	0xe948
    eb4b:	39          	rts
    eb4c:	b6 01 7f    	ldaa	0x17f
    eb4f:	2b 35       	bmi	0x0xeb86
    eb51:	b6 01 1e    	ldaa	0x11e
    eb54:	81 13       	cmpa	#0x13
    eb56:	25 16       	bcs	0x0xeb6e
    eb58:	22 08       	bhi	0x0xeb62
    eb5a:	86 19       	ldaa	#0x19
    eb5c:	b7 01 1e    	staa	0x11e
    eb5f:	7e e9 48    	jmp	0xe948
    eb62:	81 19       	cmpa	#0x19
    eb64:	22 57       	bhi	0x0xebbd
    eb66:	86 1e       	ldaa	#0x1e
    eb68:	b7 01 1e    	staa	0x11e
    eb6b:	7e e9 48    	jmp	0xe948
    eb6e:	81 03       	cmpa	#0x3
    eb70:	22 08       	bhi	0x0xeb7a
    eb72:	86 09       	ldaa	#0x9
    eb74:	b7 01 1e    	staa	0x11e
    eb77:	7e e9 48    	jmp	0xe948
    eb7a:	81 09       	cmpa	#0x9
    eb7c:	22 3f       	bhi	0x0xebbd
    eb7e:	86 0e       	ldaa	#0xe
    eb80:	b7 01 1e    	staa	0x11e
    eb83:	7e e9 48    	jmp	0xe948
    eb86:	b6 01 1e    	ldaa	0x11e
    eb89:	81 0f       	cmpa	#0xf
    eb8b:	25 18       	bcs	0x0xeba5
    eb8d:	81 1a       	cmpa	#0x1a
    eb8f:	25 08       	bcs	0x0xeb99
    eb91:	86 19       	ldaa	#0x19
    eb93:	b7 01 1e    	staa	0x11e
    eb96:	7e e9 48    	jmp	0xe948
    eb99:	81 19       	cmpa	#0x19
    eb9b:	26 20       	bne	0x0xebbd
    eb9d:	86 13       	ldaa	#0x13
    eb9f:	b7 01 1e    	staa	0x11e
    eba2:	7e e9 48    	jmp	0xe948
    eba5:	81 0e       	cmpa	#0xe
    eba7:	25 08       	bcs	0x0xebb1
    eba9:	86 09       	ldaa	#0x9
    ebab:	b7 01 1e    	staa	0x11e
    ebae:	7e e9 48    	jmp	0xe948
    ebb1:	81 09       	cmpa	#0x9
    ebb3:	26 08       	bne	0x0xebbd
    ebb5:	86 03       	ldaa	#0x3
    ebb7:	b7 01 1e    	staa	0x11e
    ebba:	7e e9 48    	jmp	0xe948
    ebbd:	39          	rts
    ebbe:	b6 01 7f    	ldaa	0x17f
    ebc1:	2b 3a       	bmi	0x0xebfd
    ebc3:	b6 01 1e    	ldaa	0x11e
    ebc6:	81 12       	cmpa	#0x12
    ebc8:	25 16       	bcs	0x0xebe0
    ebca:	22 04       	bhi	0x0xebd0
    ebcc:	86 16       	ldaa	#0x16
    ebce:	20 26       	bra	0x0xebf6
    ebd0:	81 16       	cmpa	#0x16
    ebd2:	22 04       	bhi	0x0xebd8
    ebd4:	86 1a       	ldaa	#0x1a
    ebd6:	20 1e       	bra	0x0xebf6
    ebd8:	81 1a       	cmpa	#0x1a
    ebda:	22 20       	bhi	0x0xebfc
    ebdc:	86 1e       	ldaa	#0x1e
    ebde:	20 16       	bra	0x0xebf6
    ebe0:	81 02       	cmpa	#0x2
    ebe2:	22 04       	bhi	0x0xebe8
    ebe4:	86 06       	ldaa	#0x6
    ebe6:	20 0e       	bra	0x0xebf6
    ebe8:	81 06       	cmpa	#0x6
    ebea:	22 04       	bhi	0x0xebf0
    ebec:	86 0a       	ldaa	#0xa
    ebee:	20 06       	bra	0x0xebf6
    ebf0:	81 0a       	cmpa	#0xa
    ebf2:	22 08       	bhi	0x0xebfc
    ebf4:	86 0e       	ldaa	#0xe
    ebf6:	b7 01 1e    	staa	0x11e
    ebf9:	7e e9 48    	jmp	0xe948
    ebfc:	39          	rts
    ebfd:	b6 01 1e    	ldaa	0x11e
    ec00:	81 0e       	cmpa	#0xe
    ec02:	22 16       	bhi	0x0xec1a
    ec04:	25 04       	bcs	0x0xec0a
    ec06:	86 0a       	ldaa	#0xa
    ec08:	20 ec       	bra	0x0xebf6
    ec0a:	81 0a       	cmpa	#0xa
    ec0c:	25 04       	bcs	0x0xec12
    ec0e:	86 06       	ldaa	#0x6
    ec10:	20 e4       	bra	0x0xebf6
    ec12:	81 06       	cmpa	#0x6
    ec14:	25 e6       	bcs	0x0xebfc
    ec16:	86 02       	ldaa	#0x2
    ec18:	20 dc       	bra	0x0xebf6
    ec1a:	81 1e       	cmpa	#0x1e
    ec1c:	25 04       	bcs	0x0xec22
    ec1e:	86 1a       	ldaa	#0x1a
    ec20:	20 d4       	bra	0x0xebf6
    ec22:	81 1a       	cmpa	#0x1a
    ec24:	25 04       	bcs	0x0xec2a
    ec26:	86 16       	ldaa	#0x16
    ec28:	20 cc       	bra	0x0xebf6
    ec2a:	81 16       	cmpa	#0x16
    ec2c:	25 ce       	bcs	0x0xebfc
    ec2e:	86 12       	ldaa	#0x12
    ec30:	20 c4       	bra	0x0xebf6
    ec32:	ce 10 23    	ldx	#0x1023
    ec35:	1f 00 10 f9 	brclr	0x0,x, #0x10, 0x0xec32
    ec39:	86 04       	ldaa	#0x4
    ec3b:	b7 01 1c    	staa	0x11c
    ec3e:	86 38       	ldaa	#0x38
    ec40:	b7 10 47    	staa	0x1047
    ec43:	86 80       	ldaa	#0x80
    ec45:	ba 10 00    	oraa	0x1000
    ec48:	b7 10 00    	staa	0x1000
    ec4b:	01          	nop
    ec4c:	88 80       	eora	#0x80
    ec4e:	b7 10 00    	staa	0x1000
    ec51:	96 10       	ldaa	*0x10
    ec53:	b7 10 23    	staa	0x1023
    ec56:	fc 10 0e    	ldd	0x100e
    ec59:	c3 20 08    	addd	#0x2008
    ec5c:	fd 10 1c    	std	0x101c
    ec5f:	ce 10 23    	ldx	#0x1023
    ec62:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec62
    ec66:	7a 01 1c    	dec	0x11c
    ec69:	26 d3       	bne	0x0xec3e
    ec6b:	86 08       	ldaa	#0x8
    ec6d:	b7 10 47    	staa	0x1047
    ec70:	86 80       	ldaa	#0x80
    ec72:	ba 10 00    	oraa	0x1000
    ec75:	b7 10 00    	staa	0x1000
    ec78:	01          	nop
    ec79:	88 80       	eora	#0x80
    ec7b:	b7 10 00    	staa	0x1000
    ec7e:	86 10       	ldaa	#0x10
    ec80:	b7 10 23    	staa	0x1023
    ec83:	fc 10 0e    	ldd	0x100e
    ec86:	c3 00 f0    	addd	#0xf0
    ec89:	fd 10 1c    	std	0x101c
    ec8c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec8c
    ec90:	86 01       	ldaa	#0x1
    ec92:	b7 10 47    	staa	0x1047
    ec95:	86 80       	ldaa	#0x80
    ec97:	ba 10 00    	oraa	0x1000
    ec9a:	b7 10 00    	staa	0x1000
    ec9d:	01          	nop
    ec9e:	88 80       	eora	#0x80
    eca0:	b7 10 00    	staa	0x1000
    eca3:	86 10       	ldaa	#0x10
    eca5:	b7 10 23    	staa	0x1023
    eca8:	fc 10 0e    	ldd	0x100e
    ecab:	c3 26 48    	addd	#0x2648
    ecae:	fd 10 1c    	std	0x101c
    ecb1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xecb1
    ecb5:	86 04       	ldaa	#0x4
    ecb7:	b7 10 47    	staa	0x1047
    ecba:	86 80       	ldaa	#0x80
    ecbc:	ba 10 00    	oraa	0x1000
    ecbf:	b7 10 00    	staa	0x1000
    ecc2:	01          	nop
    ecc3:	88 80       	eora	#0x80
    ecc5:	b7 10 00    	staa	0x1000
    ecc8:	86 10       	ldaa	#0x10
    ecca:	b7 10 23    	staa	0x1023
    eccd:	fc 10 0e    	ldd	0x100e
    ecd0:	c3 00 f0    	addd	#0xf0
    ecd3:	fd 10 1c    	std	0x101c
    ecd6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xecd6
    ecda:	86 0c       	ldaa	#0xc
    ecdc:	b7 10 47    	staa	0x1047
    ecdf:	b6 10 00    	ldaa	0x1000
    ece2:	8a 80       	oraa	#0x80
    ece4:	b7 10 00    	staa	0x1000
    ece7:	88 80       	eora	#0x80
    ece9:	b7 10 00    	staa	0x1000
    ecec:	7f 01 1d    	clr	0x11d
    ecef:	86 10       	ldaa	#0x10
    ecf1:	b7 10 23    	staa	0x1023
    ecf4:	fc 10 0e    	ldd	0x100e
    ecf7:	c3 00 f0    	addd	#0xf0
    ecfa:	fd 10 1c    	std	0x101c
    ecfd:	ce 01 20    	ldx	#0x120
    ed00:	18 ce ed d2 	ldy	#0xedd2
    ed04:	c6 20       	ldab	#0x20
    ed06:	18 a6 00    	ldaa	0x0,y
    ed09:	a7 00       	staa	0x0,x
    ed0b:	08          	inx
    ed0c:	18 08       	iny
    ed0e:	5a          	decb
    ed0f:	26 f5       	bne	0x0xed06
    ed11:	7f 01 1e    	clr	0x11e
    ed14:	86 20       	ldaa	#0x20
    ed16:	b7 01 1c    	staa	0x11c
    ed19:	ce 10 23    	ldx	#0x1023
    ed1c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xed1c
    ed20:	86 cf       	ldaa	#0xcf
    ed22:	b7 10 47    	staa	0x1047
    ed25:	86 80       	ldaa	#0x80
    ed27:	ba 10 00    	oraa	0x1000
    ed2a:	b7 10 00    	staa	0x1000
    ed2d:	01          	nop
    ed2e:	88 80       	eora	#0x80
    ed30:	b7 10 00    	staa	0x1000
    ed33:	86 10       	ldaa	#0x10
    ed35:	b7 10 23    	staa	0x1023
    ed38:	fc 10 0e    	ldd	0x100e
    ed3b:	c3 00 f0    	addd	#0xf0
    ed3e:	fd 10 1c    	std	0x101c
    ed41:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xed41
    ed45:	ce 01 20    	ldx	#0x120
    ed48:	f6 01 1c    	ldab	0x11c
    ed4b:	5a          	decb
    ed4c:	2a 22       	bpl	0x0xed70
    ed4e:	86 cf       	ldaa	#0xcf
    ed50:	b7 10 47    	staa	0x1047
    ed53:	86 80       	ldaa	#0x80
    ed55:	ba 10 00    	oraa	0x1000
    ed58:	b7 10 00    	staa	0x1000
    ed5b:	01          	nop
    ed5c:	88 80       	eora	#0x80
    ed5e:	b7 10 00    	staa	0x1000
    ed61:	86 10       	ldaa	#0x10
    ed63:	b7 10 23    	staa	0x1023
    ed66:	fc 10 0e    	ldd	0x100e
    ed69:	c3 00 f0    	addd	#0xf0
    ed6c:	fd 10 1c    	std	0x101c
    ed6f:	39          	rts
    ed70:	3a          	abx
    ed71:	a6 00       	ldaa	0x0,x
    ed73:	b7 10 47    	staa	0x1047
    ed76:	86 88       	ldaa	#0x88
    ed78:	ba 10 00    	oraa	0x1000
    ed7b:	b7 10 00    	staa	0x1000
    ed7e:	88 80       	eora	#0x80
    ed80:	b7 10 00    	staa	0x1000
    ed83:	88 08       	eora	#0x8
    ed85:	b7 10 00    	staa	0x1000
    ed88:	86 10       	ldaa	#0x10
    ed8a:	b7 10 23    	staa	0x1023
    ed8d:	37          	pshb
    ed8e:	fc 10 0e    	ldd	0x100e
    ed91:	c3 00 f0    	addd	#0xf0
    ed94:	fd 10 1c    	std	0x101c
    ed97:	33          	pulb
    ed98:	f7 01 1c    	stab	0x11c
    ed9b:	c1 10       	cmpb	#0x10
    ed9d:	27 02       	beq	0x0xeda1
    ed9f:	20 a4       	bra	0x0xed45
    eda1:	18 ce 10 23 	ldy	#0x1023
    eda5:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xeda5
    eda9:	fb 
    edaa:	86 8f       	ldaa	#0x8f
    edac:	b7 10 47    	staa	0x1047
    edaf:	86 80       	ldaa	#0x80
    edb1:	ba 10 00    	oraa	0x1000
    edb4:	b7 10 00    	staa	0x1000
    edb7:	88 80       	eora	#0x80
    edb9:	b7 10 00    	staa	0x1000
    edbc:	86 10       	ldaa	#0x10
    edbe:	b7 10 23    	staa	0x1023
    edc1:	fc 10 0e    	ldd	0x100e
    edc4:	c3 00 f0    	addd	#0xf0
    edc7:	fd 10 1c    	std	0x101c
    edca:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xedca
    edce:	fb 
    edcf:	7e ed 45    	jmp	0xed45
    edd2:	53          	comb
    edd3:	54          	lsrb
    edd4:	55          	.byte	0x55
    edd5:	44          	lsra
    edd6:	49          	rola
    edd7:	4f          	clra
    edd8:	20 20       	bra	0x0xedfa
    edda:	20 20       	bra	0x0xedfc
    eddc:	4f          	clra
    eddd:	4d          	tsta
    edde:	45          	.byte	0x45
    eddf:	47          	asra
    ede0:	41          	.byte	0x41
    ede1:	20 45       	bra	0x0xee28
    ede3:	4c          	inca
    ede4:	45          	.byte	0x45
    ede5:	43          	coma
    ede6:	54          	lsrb
    ede7:	52          	.byte	0x52
    ede8:	4f          	clra
    ede9:	4e          	.byte	0x4e
    edea:	49          	rola
    edeb:	43          	coma
    edec:	53          	comb
    eded:	20 31       	bra	0x0xee20
    edef:	2e 35       	bgt	0x0xee26
    edf1:	43          	coma
    edf2:	00          	bgnd
    edf3:	01          	nop
    edf4:	01          	nop
    edf5:	01          	nop
    edf6:	01          	nop
    edf7:	02          	idiv
    edf8:	02          	idiv
    edf9:	02          	idiv
    edfa:	02          	idiv
    edfb:	03          	fdiv
    edfc:	03          	fdiv
    edfd:	04          	lsrd
    edfe:	04          	lsrd
    edff:	05          	asld
    ee00:	05          	asld
    ee01:	05          	asld
    ee02:	06          	tap
    ee03:	06          	tap
    ee04:	06          	tap
    ee05:	07          	tpa
    ee06:	07          	tpa
    ee07:	07          	tpa
    ee08:	08          	inx
    ee09:	08          	inx
    ee0a:	08          	inx
    ee0b:	09          	dex
    ee0c:	09          	dex
    ee0d:	0a          	clv
    ee0e:	0a          	clv
    ee0f:	0b          	sev
    ee10:	0b          	sev
    ee11:	0c          	clc
    ee12:	0c          	clc
    ee13:	0d          	sec
    ee14:	0d          	sec
    ee15:	0e          	cli
    ee16:	0e          	cli
    ee17:	0f          	sei
    ee18:	10          	sba
    ee19:	11          	cba
    ee1a:	11          	cba
    ee1b:	12 12 13 13 	brset	*0x12, #0x13, 0x0xee32
    ee1f:	14 14 15    	bset	*0x14, #0x15
    ee22:	15 16 17    	bclr	*0x16, #0x17
    ee25:	18 18       	.byte	0x18, 0x18
    ee27:	19          	daa
    ee28:	19          	daa
    ee29:	1a 1b       	.byte	0x1a, 0x1b
    ee2b:	1c 1d 1e    	bset	0x1d,x, #0x1e
    ee2e:	1f 20 21 22 	brclr	0x20,x, #0x21, 0x0xee54
    ee32:	23 24       	bls	0x0xee58
    ee34:	25 26       	bcs	0x0xee5c
    ee36:	27 28       	beq	0x0xee60
    ee38:	29 2a       	bvs	0x0xee64
    ee3a:	2b 2c       	bmi	0x0xee68
    ee3c:	2d 2e       	blt	0x0xee6c
    ee3e:	30          	tsx
    ee3f:	31          	ins
    ee40:	32          	pula
    ee41:	33          	pulb
    ee42:	34          	des
    ee43:	35          	txs
    ee44:	36          	psha
    ee45:	37          	pshb
    ee46:	38          	pulx
    ee47:	39          	rts
    ee48:	3a          	abx
    ee49:	3b          	rti
    ee4a:	3c          	pshx
    ee4b:	3d          	mul
    ee4c:	3f          	swi
    ee4d:	41          	.byte	0x41
    ee4e:	42          	.byte	0x42
    ee4f:	43          	coma
    ee50:	44          	lsra
    ee51:	45          	.byte	0x45
    ee52:	47          	asra
    ee53:	49          	rola
    ee54:	4a          	deca
    ee55:	4c          	inca
    ee56:	4e          	.byte	0x4e
    ee57:	50          	negb
    ee58:	52          	.byte	0x52
    ee59:	54          	lsrb
    ee5a:	56          	rorb
    ee5b:	58          	aslb
    ee5c:	5a          	decb
    ee5d:	5c          	incb
    ee5e:	5e          	.byte	0x5e
    ee5f:	60 61       	neg	0x61,x
    ee61:	63 64       	com	0x64,x
    ee63:	66 68       	ror	0x68,x
    ee65:	6a 6c       	dec	0x6c,x
    ee67:	6e 70       	jmp	0x70,x
    ee69:	72          	.byte	0x72
    ee6a:	74 76 78    	lsr	0x7678
    ee6d:	7a 7c 7d    	dec	0x7c7d
    ee70:	7e 7f 00    	jmp	0x7f00
    ee73:	00          	bgnd
    ee74:	40          	nega
    ee75:	00          	bgnd
    ee76:	00          	bgnd
    ee77:	00          	bgnd
    ee78:	00          	bgnd
    ee79:	00          	bgnd
    ee7a:	5d          	tstb
    ee7b:	00          	bgnd
    ee7c:	00          	bgnd
    ee7d:	00          	bgnd
    ee7e:	00          	bgnd
    ee7f:	03          	fdiv
    ee80:	00          	bgnd
    ee81:	00          	bgnd
    ee82:	00          	bgnd
    ee83:	00          	bgnd
    ee84:	00          	bgnd
    ee85:	00          	bgnd
    ee86:	01          	nop
    ee87:	53          	comb
    ee88:	00          	bgnd
    ee89:	00          	bgnd
    ee8a:	00          	bgnd
    ee8b:	00          	bgnd
    ee8c:	10          	sba
    ee8d:	00          	bgnd
    ee8e:	00          	bgnd
    ee8f:	00          	bgnd
    ee90:	00          	bgnd
    ee91:	00          	bgnd
    ee92:	40          	nega
    ee93:	00          	bgnd
    ee94:	2c 3d       	bge	0x0xeed3
    ee96:	22 7f       	bhi	0x0xef17
    ee98:	7f 00 00    	clr	0x0
    ee9b:	00          	bgnd
    ee9c:	01          	nop
    ee9d:	00          	bgnd
    ee9e:	00          	bgnd
    ee9f:	00          	bgnd
    eea0:	00          	bgnd
    eea1:	7f 00 00    	clr	0x0
    eea4:	3d          	mul
    eea5:	57          	asrb
    eea6:	00          	bgnd
    eea7:	40          	nega
    eea8:	00          	bgnd
    eea9:	2d 3a       	blt	0x0xeee5
    eeab:	00          	bgnd
    eeac:	00          	bgnd
    eead:	00          	bgnd
    eeae:	5a          	decb
    eeaf:	40          	nega
    eeb0:	00          	bgnd
    eeb1:	00          	bgnd
    eeb2:	02          	idiv
    eeb3:	00          	bgnd
    eeb4:	00          	bgnd
    eeb5:	00          	bgnd
    eeb6:	00          	bgnd
    eeb7:	00          	bgnd
    eeb8:	00          	bgnd
    eeb9:	00          	bgnd
    eeba:	07          	tpa
    eebb:	00          	bgnd
    eebc:	21 01       	brn	0x0xeebf
    eebe:	40          	nega
    eebf:	40          	nega
    eec0:	28 4b       	bvc	0x0xef0d
    eec2:	20 0a       	bra	0x0xeece
    eec4:	00          	bgnd
    eec5:	01          	nop
    eec6:	02          	idiv
    eec7:	00          	bgnd
    eec8:	12 00 19 00 	brset	*0x0, #0x19, 0x0xeecc
    eecc:	10          	sba
    eecd:	00          	bgnd
    eece:	00          	bgnd
    eecf:	00          	bgnd
    eed0:	03          	fdiv
    eed1:	09          	dex
    eed2:	02          	idiv
    eed3:	00          	bgnd
    eed4:	11          	cba
	...
    eedd:	00          	bgnd
    eede:	40          	nega
    eedf:	40          	nega
    eee0:	00          	bgnd
    eee1:	00          	bgnd
    eee2:	02          	idiv
    eee3:	02          	idiv
    eee4:	02          	idiv
	...
    eef9:	00          	bgnd
    eefa:	07          	tpa
    eefb:	07          	tpa
    eefc:	07          	tpa
    eefd:	07          	tpa
    eefe:	07          	tpa
    eeff:	07          	tpa
    ef00:	07          	tpa
    ef01:	07          	tpa
    ef02:	40          	nega
    ef03:	40          	nega
    ef04:	40          	nega
    ef05:	40          	nega
    ef06:	40          	nega
    ef07:	40          	nega
    ef08:	40          	nega
    ef09:	40          	nega
	...
    ef12:	49          	rola
    ef13:	4e          	.byte	0x4e
    ef14:	49          	rola
    ef15:	54          	lsrb
    ef16:	49          	rola
    ef17:	41          	.byte	0x41
    ef18:	4c          	inca
    ef19:	20 20       	bra	0x0xef3b
    ef1b:	20 20       	bra	0x0xef3d
    ef1d:	20 20       	bra	0x0xef3f
    ef1f:	20 20       	bra	0x0xef41
    ef21:	20 01       	bra	0x0xef24
    ef23:	00          	bgnd
    ef24:	3b          	rti
    ef25:	44          	lsra
    ef26:	7f 00 01    	clr	0x1
    ef29:	44          	lsra
    ef2a:	7f 00 01    	clr	0x1
    ef2d:	44          	lsra
    ef2e:	7f 00 01    	clr	0x1
    ef31:	44          	lsra
    ef32:	7f 00 01    	clr	0x1
    ef35:	44          	lsra
    ef36:	7f 00 01    	clr	0x1
    ef39:	44          	lsra
    ef3a:	7f 00 01    	clr	0x1
    ef3d:	44          	lsra
    ef3e:	7f 00 01    	clr	0x1
    ef41:	44          	lsra
    ef42:	7f 3c 00    	clr	0x3c00
    ef45:	00          	bgnd
    ef46:	00          	bgnd
    ef47:	00          	bgnd
    ef48:	00          	bgnd
    ef49:	00          	bgnd
    ef4a:	07          	tpa
    ef4b:	07          	tpa
    ef4c:	07          	tpa
    ef4d:	07          	tpa
    ef4e:	07          	tpa
    ef4f:	07          	tpa
    ef50:	07          	tpa
    ef51:	07          	tpa
    ef52:	22 22       	bhi	0x0xef76
    ef54:	22 22       	bhi	0x0xef78
    ef56:	5e          	.byte	0x5e
    ef57:	5e          	.byte	0x5e
    ef58:	5e          	.byte	0x5e
    ef59:	5e          	.byte	0x5e
	...
    ef62:	49          	rola
    ef63:	4e          	.byte	0x4e
    ef64:	49          	rola
    ef65:	54          	lsrb
    ef66:	49          	rola
    ef67:	41          	.byte	0x41
    ef68:	4c          	inca
    ef69:	20 20       	bra	0x0xef8b
    ef6b:	20 20       	bra	0x0xef8d
    ef6d:	20 20       	bra	0x0xef8f
    ef6f:	20 20       	bra	0x0xef91
    ef71:	20 00       	bra	0x0xef73
    ef73:	02          	idiv
    ef74:	03          	fdiv
    ef75:	05          	asld
    ef76:	0f          	sei
    ef77:	0a          	clv
    ef78:	1e 14 3c 28 	brset	0x14,x, #0x3c, 0x0xefa4
    ef7c:	78 50 f0    	asl	0x50f0
    ef7f:	ff ff ff    	stx	0xffff
    ef82:	ff ff ff    	stx	0xffff
    ef85:	ff ff ff    	stx	0xffff
    ef88:	ff ff ff    	stx	0xffff
    ef8b:	ff ff ff    	stx	0xffff
    ef8e:	ff ff ff    	stx	0xffff
    ef91:	ff ff ff    	stx	0xffff
    ef94:	ff ff ff    	stx	0xffff
    ef97:	ff ff ff    	stx	0xffff
    ef9a:	ff ff ff    	stx	0xffff
    ef9d:	ff ff ff    	stx	0xffff
    efa0:	ff ff ff    	stx	0xffff
    efa3:	ff ff ff    	stx	0xffff
    efa6:	ff ff ff    	stx	0xffff
    efa9:	ff ff ff    	stx	0xffff
    efac:	ff ff ff    	stx	0xffff
    efaf:	ff ff ff    	stx	0xffff
    efb2:	ff ff ff    	stx	0xffff
    efb5:	ff ff ff    	stx	0xffff
    efb8:	ff ff ff    	stx	0xffff
    efbb:	ff ff ff    	stx	0xffff
    efbe:	ff ff ff    	stx	0xffff
    efc1:	ff ff ff    	stx	0xffff
    efc4:	ff ff ff    	stx	0xffff
    efc7:	ff ff ff    	stx	0xffff
    efca:	ff ff ff    	stx	0xffff
    efcd:	ff ff ff    	stx	0xffff
    efd0:	ff ff ff    	stx	0xffff
    efd3:	ff ff ff    	stx	0xffff
    efd6:	ff ff ff    	stx	0xffff
    efd9:	ff ff ff    	stx	0xffff
    efdc:	ff ff ff    	stx	0xffff
    efdf:	ff ff ff    	stx	0xffff
    efe2:	ff ff ff    	stx	0xffff
    efe5:	ff ff ff    	stx	0xffff
    efe8:	ff ff ff    	stx	0xffff
    efeb:	ff ff ff    	stx	0xffff
    efee:	ff ff ff    	stx	0xffff
    eff1:	ff ff ff    	stx	0xffff
    eff4:	ff ff ff    	stx	0xffff
    eff7:	ff ff ff    	stx	0xffff
    effa:	ff ff ff    	stx	0xffff
    effd:	ff ff ff    	stx	0xffff
    f000:	ff ff ff    	stx	0xffff
    f003:	ff ff ff    	stx	0xffff
    f006:	ff ff ff    	stx	0xffff
    f009:	ff ff ff    	stx	0xffff
    f00c:	ff ff ff    	stx	0xffff
    f00f:	ff ff ff    	stx	0xffff
    f012:	ff ff ff    	stx	0xffff
    f015:	ff ff ff    	stx	0xffff
    f018:	ff ff ff    	stx	0xffff
    f01b:	ff ff ff    	stx	0xffff
    f01e:	ff ff ff    	stx	0xffff
    f021:	ff ff ff    	stx	0xffff
    f024:	ff ff ff    	stx	0xffff
    f027:	ff ff ff    	stx	0xffff
    f02a:	ff ff ff    	stx	0xffff
    f02d:	ff ff ff    	stx	0xffff
    f030:	ff ff ff    	stx	0xffff
    f033:	ff ff ff    	stx	0xffff
    f036:	ff ff ff    	stx	0xffff
    f039:	ff ff ff    	stx	0xffff
    f03c:	ff ff ff    	stx	0xffff
    f03f:	ff ff ff    	stx	0xffff
    f042:	ff ff ff    	stx	0xffff
    f045:	ff ff ff    	stx	0xffff
    f048:	ff ff ff    	stx	0xffff
    f04b:	ff ff ff    	stx	0xffff
    f04e:	ff ff ff    	stx	0xffff
    f051:	ff ff ff    	stx	0xffff
    f054:	ff ff ff    	stx	0xffff
    f057:	ff ff ff    	stx	0xffff
    f05a:	ff ff ff    	stx	0xffff
    f05d:	ff ff ff    	stx	0xffff
    f060:	ff ff ff    	stx	0xffff
    f063:	ff ff ff    	stx	0xffff
    f066:	ff ff ff    	stx	0xffff
    f069:	ff ff ff    	stx	0xffff
    f06c:	ff ff ff    	stx	0xffff
    f06f:	ff ff ff    	stx	0xffff
    f072:	ff ff ff    	stx	0xffff
    f075:	ff ff ff    	stx	0xffff
    f078:	ff ff ff    	stx	0xffff
    f07b:	ff ff ff    	stx	0xffff
    f07e:	ff ff ff    	stx	0xffff
    f081:	ff ff ff    	stx	0xffff
    f084:	ff ff ff    	stx	0xffff
    f087:	ff ff ff    	stx	0xffff
    f08a:	ff ff ff    	stx	0xffff
    f08d:	ff ff ff    	stx	0xffff
    f090:	ff ff ff    	stx	0xffff
    f093:	ff ff ff    	stx	0xffff
    f096:	ff ff ff    	stx	0xffff
    f099:	ff ff ff    	stx	0xffff
    f09c:	ff ff ff    	stx	0xffff
    f09f:	ff ff ff    	stx	0xffff
    f0a2:	ff ff ff    	stx	0xffff
    f0a5:	ff ff ff    	stx	0xffff
    f0a8:	ff ff ff    	stx	0xffff
    f0ab:	ff ff ff    	stx	0xffff
    f0ae:	ff ff ff    	stx	0xffff
    f0b1:	ff ff ff    	stx	0xffff
    f0b4:	ff ff ff    	stx	0xffff
    f0b7:	ff ff ff    	stx	0xffff
    f0ba:	ff ff ff    	stx	0xffff
    f0bd:	ff ff ff    	stx	0xffff
    f0c0:	ff ff ff    	stx	0xffff
    f0c3:	ff ff ff    	stx	0xffff
    f0c6:	ff ff ff    	stx	0xffff
    f0c9:	ff ff ff    	stx	0xffff
    f0cc:	ff ff ff    	stx	0xffff
    f0cf:	ff ff ff    	stx	0xffff
    f0d2:	ff ff ff    	stx	0xffff
    f0d5:	ff ff ff    	stx	0xffff
    f0d8:	ff ff ff    	stx	0xffff
    f0db:	ff ff ff    	stx	0xffff
    f0de:	ff ff ff    	stx	0xffff
    f0e1:	ff ff ff    	stx	0xffff
    f0e4:	ff ff ff    	stx	0xffff
    f0e7:	ff ff ff    	stx	0xffff
    f0ea:	ff ff ff    	stx	0xffff
    f0ed:	ff ff ff    	stx	0xffff
    f0f0:	ff ff ff    	stx	0xffff
    f0f3:	ff ff ff    	stx	0xffff
    f0f6:	ff ff ff    	stx	0xffff
    f0f9:	ff ff ff    	stx	0xffff
    f0fc:	ff ff ff    	stx	0xffff
    f0ff:	ff ff ff    	stx	0xffff
    f102:	ff ff ff    	stx	0xffff
    f105:	ff ff ff    	stx	0xffff
    f108:	ff ff ff    	stx	0xffff
    f10b:	ff ff ff    	stx	0xffff
    f10e:	ff ff ff    	stx	0xffff
    f111:	ff ff ff    	stx	0xffff
    f114:	ff ff ff    	stx	0xffff
    f117:	ff ff ff    	stx	0xffff
    f11a:	ff ff ff    	stx	0xffff
    f11d:	ff ff ff    	stx	0xffff
    f120:	ff ff ff    	stx	0xffff
    f123:	ff ff ff    	stx	0xffff
    f126:	ff ff ff    	stx	0xffff
    f129:	ff ff ff    	stx	0xffff
    f12c:	ff ff ff    	stx	0xffff
    f12f:	ff ff ff    	stx	0xffff
    f132:	ff ff ff    	stx	0xffff
    f135:	ff ff ff    	stx	0xffff
    f138:	ff ff ff    	stx	0xffff
    f13b:	ff ff ff    	stx	0xffff
    f13e:	ff ff ff    	stx	0xffff
    f141:	ff ff ff    	stx	0xffff
    f144:	ff ff ff    	stx	0xffff
    f147:	ff ff ff    	stx	0xffff
    f14a:	ff ff ff    	stx	0xffff
    f14d:	ff ff ff    	stx	0xffff
    f150:	ff ff ff    	stx	0xffff
    f153:	ff ff ff    	stx	0xffff
    f156:	ff ff ff    	stx	0xffff
    f159:	ff ff ff    	stx	0xffff
    f15c:	ff ff ff    	stx	0xffff
    f15f:	ff ff ff    	stx	0xffff
    f162:	ff ff ff    	stx	0xffff
    f165:	ff ff ff    	stx	0xffff
    f168:	ff ff ff    	stx	0xffff
    f16b:	ff ff ff    	stx	0xffff
    f16e:	ff ff ff    	stx	0xffff
    f171:	ff ff ff    	stx	0xffff
    f174:	ff ff ff    	stx	0xffff
    f177:	ff ff ff    	stx	0xffff
    f17a:	ff ff ff    	stx	0xffff
    f17d:	ff ff ff    	stx	0xffff
    f180:	ff ff ff    	stx	0xffff
    f183:	ff ff ff    	stx	0xffff
    f186:	ff ff ff    	stx	0xffff
    f189:	ff ff ff    	stx	0xffff
    f18c:	ff ff ff    	stx	0xffff
    f18f:	ff ff ff    	stx	0xffff
    f192:	ff ff ff    	stx	0xffff
    f195:	ff ff ff    	stx	0xffff
    f198:	ff ff ff    	stx	0xffff
    f19b:	ff ff ff    	stx	0xffff
    f19e:	ff ff ff    	stx	0xffff
    f1a1:	ff ff ff    	stx	0xffff
    f1a4:	ff ff ff    	stx	0xffff
    f1a7:	ff ff ff    	stx	0xffff
    f1aa:	ff ff ff    	stx	0xffff
    f1ad:	ff ff ff    	stx	0xffff
    f1b0:	ff ff ff    	stx	0xffff
    f1b3:	ff ff ff    	stx	0xffff
    f1b6:	ff ff ff    	stx	0xffff
    f1b9:	ff ff ff    	stx	0xffff
    f1bc:	ff ff ff    	stx	0xffff
    f1bf:	ff ff ff    	stx	0xffff
    f1c2:	ff ff ff    	stx	0xffff
    f1c5:	ff ff ff    	stx	0xffff
    f1c8:	ff ff ff    	stx	0xffff
    f1cb:	ff ff ff    	stx	0xffff
    f1ce:	ff ff ff    	stx	0xffff
    f1d1:	ff ff ff    	stx	0xffff
    f1d4:	ff ff ff    	stx	0xffff
    f1d7:	ff ff ff    	stx	0xffff
    f1da:	ff ff ff    	stx	0xffff
    f1dd:	ff ff ff    	stx	0xffff
    f1e0:	ff ff ff    	stx	0xffff
    f1e3:	ff ff ff    	stx	0xffff
    f1e6:	ff ff ff    	stx	0xffff
    f1e9:	ff ff ff    	stx	0xffff
    f1ec:	ff ff ff    	stx	0xffff
    f1ef:	ff ff ff    	stx	0xffff
    f1f2:	ff ff ff    	stx	0xffff
    f1f5:	ff ff ff    	stx	0xffff
    f1f8:	ff ff ff    	stx	0xffff
    f1fb:	ff ff ff    	stx	0xffff
    f1fe:	ff ff ff    	stx	0xffff
    f201:	ff ff ff    	stx	0xffff
    f204:	ff ff ff    	stx	0xffff
    f207:	ff ff ff    	stx	0xffff
    f20a:	ff ff ff    	stx	0xffff
    f20d:	ff ff ff    	stx	0xffff
    f210:	ff ff ff    	stx	0xffff
    f213:	ff ff ff    	stx	0xffff
    f216:	ff ff ff    	stx	0xffff
    f219:	ff ff ff    	stx	0xffff
    f21c:	ff ff ff    	stx	0xffff
    f21f:	ff ff ff    	stx	0xffff
    f222:	ff ff ff    	stx	0xffff
    f225:	ff ff ff    	stx	0xffff
    f228:	ff ff ff    	stx	0xffff
    f22b:	ff ff ff    	stx	0xffff
    f22e:	ff ff ff    	stx	0xffff
    f231:	ff ff ff    	stx	0xffff
    f234:	ff ff ff    	stx	0xffff
    f237:	ff ff ff    	stx	0xffff
    f23a:	ff ff ff    	stx	0xffff
    f23d:	ff ff ff    	stx	0xffff
    f240:	ff ff ff    	stx	0xffff
    f243:	ff ff ff    	stx	0xffff
    f246:	ff ff ff    	stx	0xffff
    f249:	ff ff ff    	stx	0xffff
    f24c:	ff ff ff    	stx	0xffff
    f24f:	ff ff ff    	stx	0xffff
    f252:	ff ff ff    	stx	0xffff
    f255:	ff ff ff    	stx	0xffff
    f258:	ff ff ff    	stx	0xffff
    f25b:	ff ff ff    	stx	0xffff
    f25e:	ff ff ff    	stx	0xffff
    f261:	ff ff ff    	stx	0xffff
    f264:	ff ff ff    	stx	0xffff
    f267:	ff ff ff    	stx	0xffff
    f26a:	ff ff ff    	stx	0xffff
    f26d:	ff ff ff    	stx	0xffff
    f270:	ff ff ff    	stx	0xffff
    f273:	ff ff ff    	stx	0xffff
    f276:	ff ff ff    	stx	0xffff
    f279:	ff ff ff    	stx	0xffff
    f27c:	ff ff ff    	stx	0xffff
    f27f:	ff ff ff    	stx	0xffff
    f282:	ff ff ff    	stx	0xffff
    f285:	ff ff ff    	stx	0xffff
    f288:	ff ff ff    	stx	0xffff
    f28b:	ff ff ff    	stx	0xffff
    f28e:	ff ff ff    	stx	0xffff
    f291:	ff ff ff    	stx	0xffff
    f294:	ff ff ff    	stx	0xffff
    f297:	ff ff ff    	stx	0xffff
    f29a:	ff ff ff    	stx	0xffff
    f29d:	ff ff ff    	stx	0xffff
    f2a0:	ff ff ff    	stx	0xffff
    f2a3:	ff ff ff    	stx	0xffff
    f2a6:	ff ff ff    	stx	0xffff
    f2a9:	ff ff ff    	stx	0xffff
    f2ac:	ff ff ff    	stx	0xffff
    f2af:	ff ff ff    	stx	0xffff
    f2b2:	ff ff ff    	stx	0xffff
    f2b5:	ff ff ff    	stx	0xffff
    f2b8:	ff ff ff    	stx	0xffff
    f2bb:	ff ff ff    	stx	0xffff
    f2be:	ff ff ff    	stx	0xffff
    f2c1:	ff ff ff    	stx	0xffff
    f2c4:	ff ff ff    	stx	0xffff
    f2c7:	ff ff ff    	stx	0xffff
    f2ca:	ff ff ff    	stx	0xffff
    f2cd:	ff ff ff    	stx	0xffff
    f2d0:	ff ff ff    	stx	0xffff
    f2d3:	ff ff ff    	stx	0xffff
    f2d6:	ff ff ff    	stx	0xffff
    f2d9:	ff ff ff    	stx	0xffff
    f2dc:	ff ff ff    	stx	0xffff
    f2df:	ff ff ff    	stx	0xffff
    f2e2:	ff ff ff    	stx	0xffff
    f2e5:	ff ff ff    	stx	0xffff
    f2e8:	ff ff ff    	stx	0xffff
    f2eb:	ff ff ff    	stx	0xffff
    f2ee:	ff ff ff    	stx	0xffff
    f2f1:	ff ff ff    	stx	0xffff
    f2f4:	ff ff ff    	stx	0xffff
    f2f7:	ff ff ff    	stx	0xffff
    f2fa:	ff ff ff    	stx	0xffff
    f2fd:	ff ff ff    	stx	0xffff
    f300:	ff ff ff    	stx	0xffff
    f303:	ff ff ff    	stx	0xffff
    f306:	ff ff ff    	stx	0xffff
    f309:	ff ff ff    	stx	0xffff
    f30c:	ff ff ff    	stx	0xffff
    f30f:	ff ff ff    	stx	0xffff
    f312:	ff ff ff    	stx	0xffff
    f315:	ff ff ff    	stx	0xffff
    f318:	ff ff ff    	stx	0xffff
    f31b:	ff ff ff    	stx	0xffff
    f31e:	ff ff ff    	stx	0xffff
    f321:	ff ff ff    	stx	0xffff
    f324:	ff ff ff    	stx	0xffff
    f327:	ff ff ff    	stx	0xffff
    f32a:	ff ff ff    	stx	0xffff
    f32d:	ff ff ff    	stx	0xffff
    f330:	ff ff ff    	stx	0xffff
    f333:	ff ff ff    	stx	0xffff
    f336:	ff ff ff    	stx	0xffff
    f339:	ff ff ff    	stx	0xffff
    f33c:	ff ff ff    	stx	0xffff
    f33f:	ff ff ff    	stx	0xffff
    f342:	ff ff ff    	stx	0xffff
    f345:	ff ff ff    	stx	0xffff
    f348:	ff ff ff    	stx	0xffff
    f34b:	ff ff ff    	stx	0xffff
    f34e:	ff ff ff    	stx	0xffff
    f351:	ff ff ff    	stx	0xffff
    f354:	ff ff ff    	stx	0xffff
    f357:	ff ff ff    	stx	0xffff
    f35a:	ff ff ff    	stx	0xffff
    f35d:	ff ff ff    	stx	0xffff
    f360:	ff ff ff    	stx	0xffff
    f363:	ff ff ff    	stx	0xffff
    f366:	ff ff ff    	stx	0xffff
    f369:	ff ff ff    	stx	0xffff
    f36c:	ff ff ff    	stx	0xffff
    f36f:	ff ff ff    	stx	0xffff
    f372:	ff ff ff    	stx	0xffff
    f375:	ff ff ff    	stx	0xffff
    f378:	ff ff ff    	stx	0xffff
    f37b:	ff ff ff    	stx	0xffff
    f37e:	ff ff ff    	stx	0xffff
    f381:	ff ff ff    	stx	0xffff
    f384:	ff ff ff    	stx	0xffff
    f387:	ff ff ff    	stx	0xffff
    f38a:	ff ff ff    	stx	0xffff
    f38d:	ff ff ff    	stx	0xffff
    f390:	ff ff ff    	stx	0xffff
    f393:	ff ff ff    	stx	0xffff
    f396:	ff ff ff    	stx	0xffff
    f399:	ff ff ff    	stx	0xffff
    f39c:	ff ff ff    	stx	0xffff
    f39f:	ff ff ff    	stx	0xffff
    f3a2:	ff ff ff    	stx	0xffff
    f3a5:	ff ff ff    	stx	0xffff
    f3a8:	ff ff ff    	stx	0xffff
    f3ab:	ff ff ff    	stx	0xffff
    f3ae:	ff ff ff    	stx	0xffff
    f3b1:	ff ff ff    	stx	0xffff
    f3b4:	ff ff ff    	stx	0xffff
    f3b7:	ff ff ff    	stx	0xffff
    f3ba:	ff ff ff    	stx	0xffff
    f3bd:	ff ff ff    	stx	0xffff
    f3c0:	ff ff ff    	stx	0xffff
    f3c3:	ff ff ff    	stx	0xffff
    f3c6:	ff ff ff    	stx	0xffff
    f3c9:	ff ff ff    	stx	0xffff
    f3cc:	ff ff ff    	stx	0xffff
    f3cf:	ff ff ff    	stx	0xffff
    f3d2:	ff ff ff    	stx	0xffff
    f3d5:	ff ff ff    	stx	0xffff
    f3d8:	ff ff ff    	stx	0xffff
    f3db:	ff ff ff    	stx	0xffff
    f3de:	ff ff ff    	stx	0xffff
    f3e1:	ff ff ff    	stx	0xffff
    f3e4:	ff ff ff    	stx	0xffff
    f3e7:	ff ff ff    	stx	0xffff
    f3ea:	ff ff ff    	stx	0xffff
    f3ed:	ff ff ff    	stx	0xffff
    f3f0:	ff ff ff    	stx	0xffff
    f3f3:	ff ff ff    	stx	0xffff
    f3f6:	ff ff ff    	stx	0xffff
    f3f9:	ff ff ff    	stx	0xffff
    f3fc:	ff ff ff    	stx	0xffff
    f3ff:	ff ff ff    	stx	0xffff
    f402:	ff ff ff    	stx	0xffff
    f405:	ff ff ff    	stx	0xffff
    f408:	ff ff ff    	stx	0xffff
    f40b:	ff ff ff    	stx	0xffff
    f40e:	ff ff ff    	stx	0xffff
    f411:	ff ff ff    	stx	0xffff
    f414:	ff ff ff    	stx	0xffff
    f417:	ff ff ff    	stx	0xffff
    f41a:	ff ff ff    	stx	0xffff
    f41d:	ff ff ff    	stx	0xffff
    f420:	ff ff ff    	stx	0xffff
    f423:	ff ff ff    	stx	0xffff
    f426:	ff ff ff    	stx	0xffff
    f429:	ff ff ff    	stx	0xffff
    f42c:	ff ff ff    	stx	0xffff
    f42f:	ff ff ff    	stx	0xffff
    f432:	ff ff ff    	stx	0xffff
    f435:	ff ff ff    	stx	0xffff
    f438:	ff ff ff    	stx	0xffff
    f43b:	ff ff ff    	stx	0xffff
    f43e:	ff ff ff    	stx	0xffff
    f441:	ff ff ff    	stx	0xffff
    f444:	ff ff ff    	stx	0xffff
    f447:	ff ff ff    	stx	0xffff
    f44a:	ff ff ff    	stx	0xffff
    f44d:	ff ff ff    	stx	0xffff
    f450:	ff ff ff    	stx	0xffff
    f453:	ff ff ff    	stx	0xffff
    f456:	ff ff ff    	stx	0xffff
    f459:	ff ff ff    	stx	0xffff
    f45c:	ff ff ff    	stx	0xffff
    f45f:	ff ff ff    	stx	0xffff
    f462:	ff ff ff    	stx	0xffff
    f465:	ff ff ff    	stx	0xffff
    f468:	ff ff ff    	stx	0xffff
    f46b:	ff ff ff    	stx	0xffff
    f46e:	ff ff ff    	stx	0xffff
    f471:	ff ff ff    	stx	0xffff
    f474:	ff ff ff    	stx	0xffff
    f477:	ff ff ff    	stx	0xffff
    f47a:	ff ff ff    	stx	0xffff
    f47d:	ff ff ff    	stx	0xffff
    f480:	ff ff ff    	stx	0xffff
    f483:	ff ff ff    	stx	0xffff
    f486:	ff ff ff    	stx	0xffff
    f489:	ff ff ff    	stx	0xffff
    f48c:	ff ff ff    	stx	0xffff
    f48f:	ff ff ff    	stx	0xffff
    f492:	ff ff ff    	stx	0xffff
    f495:	ff ff ff    	stx	0xffff
    f498:	ff ff ff    	stx	0xffff
    f49b:	ff ff ff    	stx	0xffff
    f49e:	ff ff ff    	stx	0xffff
    f4a1:	ff ff ff    	stx	0xffff
    f4a4:	ff ff ff    	stx	0xffff
    f4a7:	ff ff ff    	stx	0xffff
    f4aa:	ff ff ff    	stx	0xffff
    f4ad:	ff ff ff    	stx	0xffff
    f4b0:	ff ff ff    	stx	0xffff
    f4b3:	ff ff ff    	stx	0xffff
    f4b6:	ff ff ff    	stx	0xffff
    f4b9:	ff ff ff    	stx	0xffff
    f4bc:	ff ff ff    	stx	0xffff
    f4bf:	ff ff ff    	stx	0xffff
    f4c2:	ff ff ff    	stx	0xffff
    f4c5:	ff ff ff    	stx	0xffff
    f4c8:	ff ff ff    	stx	0xffff
    f4cb:	ff ff ff    	stx	0xffff
    f4ce:	ff ff ff    	stx	0xffff
    f4d1:	ff ff ff    	stx	0xffff
    f4d4:	ff ff ff    	stx	0xffff
    f4d7:	ff ff ff    	stx	0xffff
    f4da:	ff ff ff    	stx	0xffff
    f4dd:	ff ff ff    	stx	0xffff
    f4e0:	ff ff ff    	stx	0xffff
    f4e3:	ff ff ff    	stx	0xffff
    f4e6:	ff ff ff    	stx	0xffff
    f4e9:	ff ff ff    	stx	0xffff
    f4ec:	ff ff ff    	stx	0xffff
    f4ef:	ff ff ff    	stx	0xffff
    f4f2:	ff ff ff    	stx	0xffff
    f4f5:	ff ff ff    	stx	0xffff
    f4f8:	ff ff ff    	stx	0xffff
    f4fb:	ff ff ff    	stx	0xffff
    f4fe:	ff ff ff    	stx	0xffff
    f501:	ff ff ff    	stx	0xffff
    f504:	ff ff ff    	stx	0xffff
    f507:	ff ff ff    	stx	0xffff
    f50a:	ff ff ff    	stx	0xffff
    f50d:	ff ff ff    	stx	0xffff
    f510:	ff ff ff    	stx	0xffff
    f513:	ff ff ff    	stx	0xffff
    f516:	ff ff ff    	stx	0xffff
    f519:	ff ff ff    	stx	0xffff
    f51c:	ff ff ff    	stx	0xffff
    f51f:	ff ff ff    	stx	0xffff
    f522:	ff ff ff    	stx	0xffff
    f525:	ff ff ff    	stx	0xffff
    f528:	ff ff ff    	stx	0xffff
    f52b:	ff ff ff    	stx	0xffff
    f52e:	ff ff ff    	stx	0xffff
    f531:	ff ff ff    	stx	0xffff
    f534:	ff ff ff    	stx	0xffff
    f537:	ff ff ff    	stx	0xffff
    f53a:	ff ff ff    	stx	0xffff
    f53d:	ff ff ff    	stx	0xffff
    f540:	ff ff ff    	stx	0xffff
    f543:	ff ff ff    	stx	0xffff
    f546:	ff ff ff    	stx	0xffff
    f549:	ff ff ff    	stx	0xffff
    f54c:	ff ff ff    	stx	0xffff
    f54f:	ff ff ff    	stx	0xffff
    f552:	ff ff ff    	stx	0xffff
    f555:	ff ff ff    	stx	0xffff
    f558:	ff ff ff    	stx	0xffff
    f55b:	ff ff ff    	stx	0xffff
    f55e:	ff ff ff    	stx	0xffff
    f561:	ff ff ff    	stx	0xffff
    f564:	ff ff ff    	stx	0xffff
    f567:	ff ff ff    	stx	0xffff
    f56a:	ff ff ff    	stx	0xffff
    f56d:	ff ff ff    	stx	0xffff
    f570:	ff ff ff    	stx	0xffff
    f573:	ff ff ff    	stx	0xffff
    f576:	ff ff ff    	stx	0xffff
    f579:	ff ff ff    	stx	0xffff
    f57c:	ff ff ff    	stx	0xffff
    f57f:	ff ff ff    	stx	0xffff
    f582:	ff ff ff    	stx	0xffff
    f585:	ff ff ff    	stx	0xffff
    f588:	ff ff ff    	stx	0xffff
    f58b:	ff ff ff    	stx	0xffff
    f58e:	ff ff ff    	stx	0xffff
    f591:	ff ff ff    	stx	0xffff
    f594:	ff ff ff    	stx	0xffff
    f597:	ff ff ff    	stx	0xffff
    f59a:	ff ff ff    	stx	0xffff
    f59d:	ff ff ff    	stx	0xffff
    f5a0:	ff ff ff    	stx	0xffff
    f5a3:	ff ff ff    	stx	0xffff
    f5a6:	ff ff ff    	stx	0xffff
    f5a9:	ff ff ff    	stx	0xffff
    f5ac:	ff ff ff    	stx	0xffff
    f5af:	ff ff ff    	stx	0xffff
    f5b2:	ff ff ff    	stx	0xffff
    f5b5:	ff ff ff    	stx	0xffff
    f5b8:	ff ff ff    	stx	0xffff
    f5bb:	ff ff ff    	stx	0xffff
    f5be:	ff ff ff    	stx	0xffff
    f5c1:	ff ff ff    	stx	0xffff
    f5c4:	ff ff ff    	stx	0xffff
    f5c7:	ff ff ff    	stx	0xffff
    f5ca:	ff ff ff    	stx	0xffff
    f5cd:	ff ff ff    	stx	0xffff
    f5d0:	ff ff ff    	stx	0xffff
    f5d3:	ff ff ff    	stx	0xffff
    f5d6:	ff ff ff    	stx	0xffff
    f5d9:	ff ff ff    	stx	0xffff
    f5dc:	ff ff ff    	stx	0xffff
    f5df:	ff ff ff    	stx	0xffff
    f5e2:	ff ff ff    	stx	0xffff
    f5e5:	ff ff ff    	stx	0xffff
    f5e8:	ff ff ff    	stx	0xffff
    f5eb:	ff ff ff    	stx	0xffff
    f5ee:	ff ff ff    	stx	0xffff
    f5f1:	ff ff ff    	stx	0xffff
    f5f4:	ff ff ff    	stx	0xffff
    f5f7:	ff ff ff    	stx	0xffff
    f5fa:	ff ff ff    	stx	0xffff
    f5fd:	ff ff ff    	stx	0xffff
    f600:	ff ff ff    	stx	0xffff
    f603:	ff ff ff    	stx	0xffff
    f606:	ff ff ff    	stx	0xffff
    f609:	ff ff ff    	stx	0xffff
    f60c:	ff ff ff    	stx	0xffff
    f60f:	ff ff ff    	stx	0xffff
    f612:	ff ff ff    	stx	0xffff
    f615:	ff ff ff    	stx	0xffff
    f618:	ff ff ff    	stx	0xffff
    f61b:	ff ff ff    	stx	0xffff
    f61e:	ff ff ff    	stx	0xffff
    f621:	ff ff ff    	stx	0xffff
    f624:	ff ff ff    	stx	0xffff
    f627:	ff ff ff    	stx	0xffff
    f62a:	ff ff ff    	stx	0xffff
    f62d:	ff ff ff    	stx	0xffff
    f630:	ff ff ff    	stx	0xffff
    f633:	ff ff ff    	stx	0xffff
    f636:	ff ff ff    	stx	0xffff
    f639:	ff ff ff    	stx	0xffff
    f63c:	ff ff ff    	stx	0xffff
    f63f:	ff ff ff    	stx	0xffff
    f642:	ff ff ff    	stx	0xffff
    f645:	ff ff ff    	stx	0xffff
    f648:	ff ff ff    	stx	0xffff
    f64b:	ff ff ff    	stx	0xffff
    f64e:	ff ff ff    	stx	0xffff
    f651:	ff ff ff    	stx	0xffff
    f654:	ff ff ff    	stx	0xffff
    f657:	ff ff ff    	stx	0xffff
    f65a:	ff ff ff    	stx	0xffff
    f65d:	ff ff ff    	stx	0xffff
    f660:	ff ff ff    	stx	0xffff
    f663:	ff ff ff    	stx	0xffff
    f666:	ff ff ff    	stx	0xffff
    f669:	ff ff ff    	stx	0xffff
    f66c:	ff ff ff    	stx	0xffff
    f66f:	ff ff ff    	stx	0xffff
    f672:	ff ff ff    	stx	0xffff
    f675:	ff ff ff    	stx	0xffff
    f678:	ff ff ff    	stx	0xffff
    f67b:	ff ff ff    	stx	0xffff
    f67e:	ff ff ff    	stx	0xffff
    f681:	ff ff ff    	stx	0xffff
    f684:	ff ff ff    	stx	0xffff
    f687:	ff ff ff    	stx	0xffff
    f68a:	ff ff ff    	stx	0xffff
    f68d:	ff ff ff    	stx	0xffff
    f690:	ff ff ff    	stx	0xffff
    f693:	ff ff ff    	stx	0xffff
    f696:	ff ff ff    	stx	0xffff
    f699:	ff ff ff    	stx	0xffff
    f69c:	ff ff ff    	stx	0xffff
    f69f:	ff ff ff    	stx	0xffff
    f6a2:	ff ff ff    	stx	0xffff
    f6a5:	ff ff ff    	stx	0xffff
    f6a8:	ff ff ff    	stx	0xffff
    f6ab:	ff ff ff    	stx	0xffff
    f6ae:	ff ff ff    	stx	0xffff
    f6b1:	ff ff ff    	stx	0xffff
    f6b4:	ff ff ff    	stx	0xffff
    f6b7:	ff ff ff    	stx	0xffff
    f6ba:	ff ff ff    	stx	0xffff
    f6bd:	ff ff ff    	stx	0xffff
    f6c0:	ff ff ff    	stx	0xffff
    f6c3:	ff ff ff    	stx	0xffff
    f6c6:	ff ff ff    	stx	0xffff
    f6c9:	ff ff ff    	stx	0xffff
    f6cc:	ff ff ff    	stx	0xffff
    f6cf:	ff ff ff    	stx	0xffff
    f6d2:	ff ff ff    	stx	0xffff
    f6d5:	ff ff ff    	stx	0xffff
    f6d8:	ff ff ff    	stx	0xffff
    f6db:	ff ff ff    	stx	0xffff
    f6de:	ff ff ff    	stx	0xffff
    f6e1:	ff ff ff    	stx	0xffff
    f6e4:	ff ff ff    	stx	0xffff
    f6e7:	ff ff ff    	stx	0xffff
    f6ea:	ff ff ff    	stx	0xffff
    f6ed:	ff ff ff    	stx	0xffff
    f6f0:	ff ff ff    	stx	0xffff
    f6f3:	ff ff ff    	stx	0xffff
    f6f6:	ff ff ff    	stx	0xffff
    f6f9:	ff ff ff    	stx	0xffff
    f6fc:	ff ff ff    	stx	0xffff
    f6ff:	ff ff ff    	stx	0xffff
    f702:	ff ff ff    	stx	0xffff
    f705:	ff ff ff    	stx	0xffff
    f708:	ff ff ff    	stx	0xffff
    f70b:	ff ff ff    	stx	0xffff
    f70e:	ff ff ff    	stx	0xffff
    f711:	ff ff ff    	stx	0xffff
    f714:	ff ff ff    	stx	0xffff
    f717:	ff ff ff    	stx	0xffff
    f71a:	ff ff ff    	stx	0xffff
    f71d:	ff ff ff    	stx	0xffff
    f720:	ff ff ff    	stx	0xffff
    f723:	ff ff ff    	stx	0xffff
    f726:	ff ff ff    	stx	0xffff
    f729:	ff ff ff    	stx	0xffff
    f72c:	ff ff ff    	stx	0xffff
    f72f:	ff ff ff    	stx	0xffff
    f732:	ff ff ff    	stx	0xffff
    f735:	ff ff ff    	stx	0xffff
    f738:	ff ff ff    	stx	0xffff
    f73b:	ff ff ff    	stx	0xffff
    f73e:	ff ff ff    	stx	0xffff
    f741:	ff ff ff    	stx	0xffff
    f744:	ff ff ff    	stx	0xffff
    f747:	ff ff ff    	stx	0xffff
    f74a:	ff ff ff    	stx	0xffff
    f74d:	ff ff ff    	stx	0xffff
    f750:	ff ff ff    	stx	0xffff
    f753:	ff ff ff    	stx	0xffff
    f756:	ff ff ff    	stx	0xffff
    f759:	ff ff ff    	stx	0xffff
    f75c:	ff ff ff    	stx	0xffff
    f75f:	ff ff ff    	stx	0xffff
    f762:	ff ff ff    	stx	0xffff
    f765:	ff ff ff    	stx	0xffff
    f768:	ff ff ff    	stx	0xffff
    f76b:	ff ff ff    	stx	0xffff
    f76e:	ff ff ff    	stx	0xffff
    f771:	ff ff ff    	stx	0xffff
    f774:	ff ff ff    	stx	0xffff
    f777:	ff ff ff    	stx	0xffff
    f77a:	ff ff ff    	stx	0xffff
    f77d:	ff ff ff    	stx	0xffff
    f780:	ff ff ff    	stx	0xffff
    f783:	ff ff ff    	stx	0xffff
    f786:	ff ff ff    	stx	0xffff
    f789:	ff ff ff    	stx	0xffff
    f78c:	ff ff ff    	stx	0xffff
    f78f:	ff ff ff    	stx	0xffff
    f792:	ff ff ff    	stx	0xffff
    f795:	ff ff ff    	stx	0xffff
    f798:	ff ff ff    	stx	0xffff
    f79b:	ff ff ff    	stx	0xffff
    f79e:	ff ff ff    	stx	0xffff
    f7a1:	ff ff ff    	stx	0xffff
    f7a4:	ff ff ff    	stx	0xffff
    f7a7:	ff ff ff    	stx	0xffff
    f7aa:	ff ff ff    	stx	0xffff
    f7ad:	ff ff ff    	stx	0xffff
    f7b0:	ff ff ff    	stx	0xffff
    f7b3:	ff ff ff    	stx	0xffff
    f7b6:	ff ff ff    	stx	0xffff
    f7b9:	ff ff ff    	stx	0xffff
    f7bc:	ff ff ff    	stx	0xffff
    f7bf:	ff ff ff    	stx	0xffff
    f7c2:	ff ff ff    	stx	0xffff
    f7c5:	ff ff ff    	stx	0xffff
    f7c8:	ff ff ff    	stx	0xffff
    f7cb:	ff ff ff    	stx	0xffff
    f7ce:	ff ff ff    	stx	0xffff
    f7d1:	ff ff ff    	stx	0xffff
    f7d4:	ff ff ff    	stx	0xffff
    f7d7:	ff ff ff    	stx	0xffff
    f7da:	ff ff ff    	stx	0xffff
    f7dd:	ff ff ff    	stx	0xffff
    f7e0:	ff ff ff    	stx	0xffff
    f7e3:	ff ff ff    	stx	0xffff
    f7e6:	ff ff ff    	stx	0xffff
    f7e9:	ff ff ff    	stx	0xffff
    f7ec:	ff ff ff    	stx	0xffff
    f7ef:	ff ff ff    	stx	0xffff
    f7f2:	ff ff ff    	stx	0xffff
    f7f5:	ff ff ff    	stx	0xffff
    f7f8:	ff ff ff    	stx	0xffff
    f7fb:	ff ff ff    	stx	0xffff
    f7fe:	ff ff ff    	stx	0xffff
    f801:	ff ff ff    	stx	0xffff
    f804:	ff ff ff    	stx	0xffff
    f807:	ff ff ff    	stx	0xffff
    f80a:	ff ff ff    	stx	0xffff
    f80d:	ff ff ff    	stx	0xffff
    f810:	ff ff ff    	stx	0xffff
    f813:	ff ff ff    	stx	0xffff
    f816:	ff ff ff    	stx	0xffff
    f819:	ff ff ff    	stx	0xffff
    f81c:	ff ff ff    	stx	0xffff
    f81f:	ff ff ff    	stx	0xffff
    f822:	ff ff ff    	stx	0xffff
    f825:	ff ff ff    	stx	0xffff
    f828:	ff ff ff    	stx	0xffff
    f82b:	ff ff ff    	stx	0xffff
    f82e:	ff ff ff    	stx	0xffff
    f831:	ff ff ff    	stx	0xffff
    f834:	ff ff ff    	stx	0xffff
    f837:	ff ff ff    	stx	0xffff
    f83a:	ff ff ff    	stx	0xffff
    f83d:	ff ff ff    	stx	0xffff
    f840:	ff ff ff    	stx	0xffff
    f843:	ff ff ff    	stx	0xffff
    f846:	ff ff ff    	stx	0xffff
    f849:	ff ff ff    	stx	0xffff
    f84c:	ff ff ff    	stx	0xffff
    f84f:	ff ff ff    	stx	0xffff
    f852:	ff ff ff    	stx	0xffff
    f855:	ff ff ff    	stx	0xffff
    f858:	ff ff ff    	stx	0xffff
    f85b:	ff ff ff    	stx	0xffff
    f85e:	ff ff ff    	stx	0xffff
    f861:	ff ff ff    	stx	0xffff
    f864:	ff ff ff    	stx	0xffff
    f867:	ff ff ff    	stx	0xffff
    f86a:	ff ff ff    	stx	0xffff
    f86d:	ff ff ff    	stx	0xffff
    f870:	ff ff ff    	stx	0xffff
    f873:	ff ff ff    	stx	0xffff
    f876:	ff ff ff    	stx	0xffff
    f879:	ff ff ff    	stx	0xffff
    f87c:	ff ff ff    	stx	0xffff
    f87f:	ff ff ff    	stx	0xffff
    f882:	ff ff ff    	stx	0xffff
    f885:	ff ff ff    	stx	0xffff
    f888:	ff ff ff    	stx	0xffff
    f88b:	ff ff ff    	stx	0xffff
    f88e:	ff ff ff    	stx	0xffff
    f891:	ff ff ff    	stx	0xffff
    f894:	ff ff ff    	stx	0xffff
    f897:	ff ff ff    	stx	0xffff
    f89a:	ff ff ff    	stx	0xffff
    f89d:	ff ff ff    	stx	0xffff
    f8a0:	ff ff ff    	stx	0xffff
    f8a3:	ff ff ff    	stx	0xffff
    f8a6:	ff ff ff    	stx	0xffff
    f8a9:	ff ff ff    	stx	0xffff
    f8ac:	ff ff ff    	stx	0xffff
    f8af:	ff ff ff    	stx	0xffff
    f8b2:	ff ff ff    	stx	0xffff
    f8b5:	ff ff ff    	stx	0xffff
    f8b8:	ff ff ff    	stx	0xffff
    f8bb:	ff ff ff    	stx	0xffff
    f8be:	ff ff ff    	stx	0xffff
    f8c1:	ff ff ff    	stx	0xffff
    f8c4:	ff ff ff    	stx	0xffff
    f8c7:	ff ff ff    	stx	0xffff
    f8ca:	ff ff ff    	stx	0xffff
    f8cd:	ff ff ff    	stx	0xffff
    f8d0:	ff ff ff    	stx	0xffff
    f8d3:	ff ff ff    	stx	0xffff
    f8d6:	ff ff ff    	stx	0xffff
    f8d9:	ff ff ff    	stx	0xffff
    f8dc:	ff ff ff    	stx	0xffff
    f8df:	ff ff ff    	stx	0xffff
    f8e2:	ff ff ff    	stx	0xffff
    f8e5:	ff ff ff    	stx	0xffff
    f8e8:	ff ff ff    	stx	0xffff
    f8eb:	ff ff ff    	stx	0xffff
    f8ee:	ff ff ff    	stx	0xffff
    f8f1:	ff ff ff    	stx	0xffff
    f8f4:	ff ff ff    	stx	0xffff
    f8f7:	ff ff ff    	stx	0xffff
    f8fa:	ff ff ff    	stx	0xffff
    f8fd:	ff ff ff    	stx	0xffff
    f900:	ff ff ff    	stx	0xffff
    f903:	ff ff ff    	stx	0xffff
    f906:	ff ff ff    	stx	0xffff
    f909:	ff ff ff    	stx	0xffff
    f90c:	ff ff ff    	stx	0xffff
    f90f:	ff ff ff    	stx	0xffff
    f912:	ff ff ff    	stx	0xffff
    f915:	ff ff ff    	stx	0xffff
    f918:	ff ff ff    	stx	0xffff
    f91b:	ff ff ff    	stx	0xffff
    f91e:	ff ff ff    	stx	0xffff
    f921:	ff ff ff    	stx	0xffff
    f924:	ff ff ff    	stx	0xffff
    f927:	ff ff ff    	stx	0xffff
    f92a:	ff ff ff    	stx	0xffff
    f92d:	ff ff ff    	stx	0xffff
    f930:	ff ff ff    	stx	0xffff
    f933:	ff ff ff    	stx	0xffff
    f936:	ff ff ff    	stx	0xffff
    f939:	ff ff ff    	stx	0xffff
    f93c:	ff ff ff    	stx	0xffff
    f93f:	ff ff ff    	stx	0xffff
    f942:	ff ff ff    	stx	0xffff
    f945:	ff ff ff    	stx	0xffff
    f948:	ff ff ff    	stx	0xffff
    f94b:	ff ff ff    	stx	0xffff
    f94e:	ff ff ff    	stx	0xffff
    f951:	ff ff ff    	stx	0xffff
    f954:	ff ff ff    	stx	0xffff
    f957:	ff ff ff    	stx	0xffff
    f95a:	ff ff ff    	stx	0xffff
    f95d:	ff ff ff    	stx	0xffff
    f960:	ff ff ff    	stx	0xffff
    f963:	ff ff ff    	stx	0xffff
    f966:	ff ff ff    	stx	0xffff
    f969:	ff ff ff    	stx	0xffff
    f96c:	ff ff ff    	stx	0xffff
    f96f:	ff ff ff    	stx	0xffff
    f972:	ff ff ff    	stx	0xffff
    f975:	ff ff ff    	stx	0xffff
    f978:	ff ff ff    	stx	0xffff
    f97b:	ff ff ff    	stx	0xffff
    f97e:	ff ff ff    	stx	0xffff
    f981:	ff ff ff    	stx	0xffff
    f984:	ff ff ff    	stx	0xffff
    f987:	ff ff ff    	stx	0xffff
    f98a:	ff ff ff    	stx	0xffff
    f98d:	ff ff ff    	stx	0xffff
    f990:	ff ff ff    	stx	0xffff
    f993:	ff ff ff    	stx	0xffff
    f996:	ff ff ff    	stx	0xffff
    f999:	ff ff ff    	stx	0xffff
    f99c:	ff ff ff    	stx	0xffff
    f99f:	ff ff ff    	stx	0xffff
    f9a2:	ff ff ff    	stx	0xffff
    f9a5:	ff ff ff    	stx	0xffff
    f9a8:	ff ff ff    	stx	0xffff
    f9ab:	ff ff ff    	stx	0xffff
    f9ae:	ff ff ff    	stx	0xffff
    f9b1:	ff ff ff    	stx	0xffff
    f9b4:	ff ff ff    	stx	0xffff
    f9b7:	ff ff ff    	stx	0xffff
    f9ba:	ff ff ff    	stx	0xffff
    f9bd:	ff ff ff    	stx	0xffff
    f9c0:	ff ff ff    	stx	0xffff
    f9c3:	ff ff ff    	stx	0xffff
    f9c6:	ff ff ff    	stx	0xffff
    f9c9:	ff ff ff    	stx	0xffff
    f9cc:	ff ff ff    	stx	0xffff
    f9cf:	ff ff ff    	stx	0xffff
    f9d2:	ff ff ff    	stx	0xffff
    f9d5:	ff ff ff    	stx	0xffff
    f9d8:	ff ff ff    	stx	0xffff
    f9db:	ff ff ff    	stx	0xffff
    f9de:	ff ff ff    	stx	0xffff
    f9e1:	f6 10 2e    	ldab	0x102e
    f9e4:	37          	pshb
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
    fdef:	ff 3b ff    	stx	0x3bff
    fdf2:	ff ff ff    	stx	0xffff
    fdf5:	ff ff ff    	stx	0xffff
    fdf8:	ff ff ff    	stx	0xffff
    fdfb:	ff ff ff    	stx	0xffff
    fdfe:	ff ff 7e    	stx	0xff7e
    fe01:	a9 81       	adca	0x81,x
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
    fe28:	7e fe c1    	jmp	0xfec1
    fe2b:	c5 02       	bitb	#0x2
    fe2d:	26 11       	bne	0x0xfe40
    fe2f:	c5 08       	bitb	#0x8
    fe31:	26 0d       	bne	0x0xfe40
    fe33:	b6 10 2f    	ldaa	0x102f
    fe36:	2b 0e       	bmi	0x0xfe46
    fe38:	7d 01 73    	tst	0x173
    fe3b:	26 31       	bne	0x0xfe6e
    fe3d:	7e fe c1    	jmp	0xfec1
    fe40:	b6 10 2f    	ldaa	0x102f
    fe43:	7e fe c1    	jmp	0xfec1
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
    fe7b:	7e fe c1    	jmp	0xfec1
    fe7e:	32          	pula
    fe7f:	7f 01 73    	clr	0x173
    fe82:	7e fe c1    	jmp	0xfec1
    fe85:	81 f8       	cmpa	#0xf8
    fe87:	26 0b       	bne	0x0xfe94
    fe89:	d6 df       	ldab	*0xdf
    fe8b:	c1 f0       	cmpb	#0xf0
    fe8d:	26 03       	bne	0x0xfe92
    fe8f:	7e fe c1    	jmp	0xfec1
    fe92:	20 da       	bra	0x0xfe6e
    fe94:	81 ff       	cmpa	#0xff
    fe96:	26 03       	bne	0x0xfe9b
    fe98:	7e 80 00    	jmp	0x8000
    fe9b:	81 fa       	cmpa	#0xfa
    fe9d:	26 02       	bne	0x0xfea1
    fe9f:	20 cd       	bra	0x0xfe6e
    fea1:	81 f7       	cmpa	#0xf7
    fea3:	26 0c       	bne	0x0xfeb1
    fea5:	c6 f0       	ldab	#0xf0
    fea7:	f1 01 73    	cmpb	0x173
    feaa:	26 15       	bne	0x0xfec1
    feac:	7f 01 73    	clr	0x173
    feaf:	20 bd       	bra	0x0xfe6e
    feb1:	81 f0       	cmpa	#0xf0
    feb3:	26 05       	bne	0x0xfeba
    feb5:	b7 01 73    	staa	0x173
    feb8:	20 b4       	bra	0x0xfe6e
    feba:	81 f4       	cmpa	#0xf4
    febc:	24 03       	bcc	0x0xfec1
    febe:	7f 01 73    	clr	0x173
    fec1:	33          	pulb
    fec2:	c5 80       	bitb	#0x80
    fec4:	27 02       	beq	0x0xfec8
    fec6:	8d 01       	bsr	0x0xfec9
    fec8:	3b          	rti
    fec9:	fe 01 60    	ldx	0x160
    fecc:	bc 01 62    	cpx	0x162
    fecf:	27 16       	beq	0x0xfee7
    fed1:	a6 00       	ldaa	0x0,x
    fed3:	b7 10 2f    	staa	0x102f
    fed6:	8f          	xgdx
    fed7:	5c          	incb
    fed8:	c1 60       	cmpb	#0x60
    feda:	25 02       	bcs	0x0xfede
    fedc:	c6 40       	ldab	#0x40
    fede:	fd 01 60    	std	0x160
    fee1:	86 ac       	ldaa	#0xac
    fee3:	b7 10 2d    	staa	0x102d
    fee6:	39          	rts
    fee7:	86 2c       	ldaa	#0x2c
    fee9:	b7 10 2d    	staa	0x102d
    feec:	7c 00 d7    	inc	0xd7
    feef:	39          	rts
    fef0:	ff ff ff    	stx	0xffff
    fef3:	ff ff ff    	stx	0xffff
    fef6:	ff ff ff    	stx	0xffff
    fef9:	ff ff ff    	stx	0xffff
    fefc:	ff ff ff    	stx	0xffff
    feff:	ff ff ff    	stx	0xffff
    ff02:	ff ff ff    	stx	0xffff
    ff05:	ff ff ff    	stx	0xffff
    ff08:	ff ff ff    	stx	0xffff
    ff0b:	ff ff ff    	stx	0xffff
    ff0e:	ff ff ff    	stx	0xffff
    ff11:	ff ff ff    	stx	0xffff
    ff14:	ff ff ff    	stx	0xffff
    ff17:	ff ff ff    	stx	0xffff
    ff1a:	ff ff ff    	stx	0xffff
    ff1d:	ff ff ff    	stx	0xffff
    ff20:	ff ff ff    	stx	0xffff
    ff23:	ff ff ff    	stx	0xffff
    ff26:	ff ff ff    	stx	0xffff
    ff29:	ff ff ff    	stx	0xffff
    ff2c:	ff ff ff    	stx	0xffff
    ff2f:	ff ff ff    	stx	0xffff
    ff32:	ff ff ff    	stx	0xffff
    ff35:	ff ff ff    	stx	0xffff
    ff38:	ff ff ff    	stx	0xffff
    ff3b:	ff ff ff    	stx	0xffff
    ff3e:	ff ff ff    	stx	0xffff
    ff41:	ff ff ff    	stx	0xffff
    ff44:	ff ff ff    	stx	0xffff
    ff47:	ff ff ff    	stx	0xffff
    ff4a:	ff ff ff    	stx	0xffff
    ff4d:	ff ff ff    	stx	0xffff
    ff50:	ff ff ff    	stx	0xffff
    ff53:	ff ff ff    	stx	0xffff
    ff56:	ff ff ff    	stx	0xffff
    ff59:	ff ff ff    	stx	0xffff
    ff5c:	ff ff ff    	stx	0xffff
    ff5f:	ff ff ff    	stx	0xffff
    ff62:	ff ff ff    	stx	0xffff
    ff65:	ff ff ff    	stx	0xffff
    ff68:	ff ff ff    	stx	0xffff
    ff6b:	ff ff ff    	stx	0xffff
    ff6e:	ff ff ff    	stx	0xffff
    ff71:	ff ff ff    	stx	0xffff
    ff74:	ff ff ff    	stx	0xffff
    ff77:	ff ff ff    	stx	0xffff
    ff7a:	ff ff ff    	stx	0xffff
    ff7d:	ff ff ff    	stx	0xffff
    ff80:	ff ff ff    	stx	0xffff
    ff83:	ff ff ff    	stx	0xffff
    ff86:	ff ff ff    	stx	0xffff
    ff89:	ff ff ff    	stx	0xffff
    ff8c:	ff ff ff    	stx	0xffff
    ff8f:	ff ff ff    	stx	0xffff
    ff92:	ff ff ff    	stx	0xffff
    ff95:	ff ff ff    	stx	0xffff
    ff98:	ff ff ff    	stx	0xffff
    ff9b:	ff ff ff    	stx	0xffff
    ff9e:	ff ff 7e    	stx	0xff7e
    ffa1:	80 00       	suba	#0x0
    ffa3:	ff ff ff    	stx	0xffff
    ffa6:	ff ff ff    	stx	0xffff
    ffa9:	ff ff ff    	stx	0xffff
    ffac:	ff ff ff    	stx	0xffff
    ffaf:	ff 4f 4d    	stx	0x4f4d
    ffb2:	38          	pulx
    ffb3:	20 43       	bra	0x0xfff8
    ffb5:	2e 31       	bgt	0x0xffe8
    ffb7:	2e 35       	bgt	0x0xffee
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
