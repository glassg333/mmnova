
/home/user/.scratch_fw/images/CODE_OS1.5.decoded.bin:     file format binary


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
    807b:	bd ec 00    	jsr	0xec00
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
    9020:	bd ea 48    	jsr	0xea48
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
    a336:	ce dd 63    	ldx	#0xdd63
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
    a35d:	ce dd ab    	ldx	#0xddab
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
    a3cf:	ce dd b4    	ldx	#0xddb4
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
    a867:	bd e9 07    	jsr	0xe907
    a86a:	ce 10 23    	ldx	#0x1023
    a86d:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa86d
    a871:	bd e9 3b    	jsr	0xe93b
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
    a897:	bd e8 8f    	jsr	0xe88f
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
    a8c7:	bd e9 3b    	jsr	0xe93b
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
    a8ed:	bd e8 8f    	jsr	0xe88f
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
    a91f:	3b          	rti
    a920:	ce 01 20    	ldx	#0x120
    a923:	18 ce a9 4c 	ldy	#0xa94c
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
    a943:	bd e8 8f    	jsr	0xe88f
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
    ac67:	05          	asld
    ac68:	03          	fdiv
    ac69:	01          	nop
    ac6a:	04          	lsrd
    ac6b:	00          	bgnd
    ac6c:	02          	idiv
    ac6d:	05          	asld
    ac6e:	00          	bgnd
    ac6f:	03          	fdiv
    ac70:	01          	nop
    ac71:	04          	lsrd
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
    ac83:	ce ed c0    	ldx	#0xedc0
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
    aca0:	bd e9 ee    	jsr	0xe9ee
    aca3:	86 03       	ldaa	#0x3
    aca5:	b7 01 1c    	staa	0x11c
    aca8:	86 13       	ldaa	#0x13
    acaa:	b7 01 1e    	staa	0x11e
    acad:	bd e9 16    	jsr	0xe916
    acb0:	96 2a       	ldaa	*0x2a
    acb2:	c6 48       	ldab	#0x48
    acb4:	bd b0 5c    	jsr	0xb05c
    acb7:	7e aa 59    	jmp	0xaa59
    acba:	16          	tab
    acbb:	ce ed c0    	ldx	#0xedc0
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
    acd8:	bd e9 ee    	jsr	0xe9ee
    acdb:	86 03       	ldaa	#0x3
    acdd:	b7 01 1c    	staa	0x11c
    ace0:	86 13       	ldaa	#0x13
    ace2:	b7 01 1e    	staa	0x11e
    ace5:	bd e9 16    	jsr	0xe916
    ace8:	96 37       	ldaa	*0x37
    acea:	c6 4a       	ldab	#0x4a
    acec:	bd b0 5c    	jsr	0xb05c
    acef:	7e aa 59    	jmp	0xaa59
    acf2:	16          	tab
    acf3:	ce ed c0    	ldx	#0xedc0
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
    ad6a:	bd e9 ee    	jsr	0xe9ee
    ad6d:	86 03       	ldaa	#0x3
    ad6f:	b7 01 1c    	staa	0x11c
    ad72:	86 1e       	ldaa	#0x1e
    ad74:	b7 01 1e    	staa	0x11e
    ad77:	bd e9 16    	jsr	0xe916
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
    ad98:	bd e9 ee    	jsr	0xe9ee
    ad9b:	86 03       	ldaa	#0x3
    ad9d:	b7 01 1c    	staa	0x11c
    ada0:	86 19       	ldaa	#0x19
    ada2:	b7 01 1e    	staa	0x11e
    ada5:	bd e9 16    	jsr	0xe916
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
    ae33:	bd e9 ee    	jsr	0xe9ee
    ae36:	86 03       	ldaa	#0x3
    ae38:	b7 01 1c    	staa	0x11c
    ae3b:	86 16       	ldaa	#0x16
    ae3d:	b7 01 1e    	staa	0x11e
    ae40:	bd e9 16    	jsr	0xe916
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
    ae60:	bd e9 ee    	jsr	0xe9ee
    ae63:	86 03       	ldaa	#0x3
    ae65:	b7 01 1c    	staa	0x11c
    ae68:	86 13       	ldaa	#0x13
    ae6a:	b7 01 1e    	staa	0x11e
    ae6d:	bd e9 16    	jsr	0xe916
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
    aea1:	bd e9 ee    	jsr	0xe9ee
    aea4:	86 03       	ldaa	#0x3
    aea6:	b7 01 1c    	staa	0x11c
    aea9:	86 12       	ldaa	#0x12
    aeab:	b7 01 1e    	staa	0x11e
    aeae:	bd e9 16    	jsr	0xe916
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
    aee2:	bd ea b5    	jsr	0xeab5
    aee5:	86 03       	ldaa	#0x3
    aee7:	b7 01 1c    	staa	0x11c
    aeea:	86 1f       	ldaa	#0x1f
    aeec:	b7 01 1e    	staa	0x11e
    aeef:	bd e9 16    	jsr	0xe916
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
    af32:	bd e9 ee    	jsr	0xe9ee
    af35:	86 03       	ldaa	#0x3
    af37:	b7 01 1c    	staa	0x11c
    af3a:	86 1a       	ldaa	#0x1a
    af3c:	b7 01 1e    	staa	0x11e
    af3f:	bd e9 16    	jsr	0xe916
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
    b0c8:	c2 ea       	sbcb	#0xea
    b0ca:	c2 f3       	sbcb	#0xf3
    b0cc:	c4 d1       	andb	#0xd1
    b0ce:	c6 00       	ldab	#0x0
    b0d0:	c7          	.byte	0xc7
    b0d1:	67 c7       	asr	0xc7,x
    b0d3:	6a c8       	dec	0xc8,x
    b0d5:	a5 c9       	bita	0xc9,x
    b0d7:	6c ca       	inc	0xca,x
    b0d9:	33          	pulb
    b0da:	cb 14       	addb	#0x14
    b0dc:	cb 20       	addb	#0x20
    b0de:	cc 12 cc    	ldd	#0x12cc
    b0e1:	9c cd       	cpx	*0xcd
    b0e3:	2a cd       	bpl	0x0xb0b2
    b0e5:	dc ce       	ldd	*0xce
    b0e7:	df ce       	stx	*0xce
    b0e9:	e2 cf       	sbcb	0xcf,x
    b0eb:	b2 d1 68    	sbca	0xd168
    b0ee:	d2 44       	sbcb	*0x44
    b0f0:	f6 01 1b    	ldab	0x11b
    b0f3:	58          	aslb
    b0f4:	ce b0 98    	ldx	#0xb098
    b0f7:	3a          	abx
    b0f8:	ee 00       	ldx	0x0,x
    b0fa:	6e 00       	jmp	0x0,x
    b0fc:	7d 01 1c    	tst	0x11c
    b0ff:	2a 03       	bpl	0x0xb104
    b101:	7e d3 c3    	jmp	0xd3c3
    b104:	7d 01 1c    	tst	0x11c
    b107:	27 03       	beq	0x0xb10c
    b109:	7e b1 32    	jmp	0xb132
    b10c:	7d 01 1e    	tst	0x11e
    b10f:	26 08       	bne	0x0xb119
    b111:	86 0c       	ldaa	#0xc
    b113:	b7 01 1e    	staa	0x11e
    b116:	bd e9 16    	jsr	0xe916
    b119:	7d 01 7e    	tst	0x17e
    b11c:	27 0c       	beq	0x0xb12a
    b11e:	7f 01 7e    	clr	0x17e
    b121:	7f 01 7f    	clr	0x17f
    b124:	7f 01 1a    	clr	0x11a
    b127:	7e d3 c3    	jmp	0xd3c3
    b12a:	7d 01 1c    	tst	0x11c
    b12d:	26 03       	bne	0x0xb132
    b12f:	7e d3 af    	jmp	0xd3af
    b132:	b6 10 23    	ldaa	0x1023
    b135:	85 10       	bita	#0x10
    b137:	27 03       	beq	0x0xb13c
    b139:	7e e8 8a    	jmp	0xe88a
    b13c:	7e d3 af    	jmp	0xd3af
    b13f:	7d 01 1c    	tst	0x11c
    b142:	2a 03       	bpl	0x0xb147
    b144:	7e d4 8d    	jmp	0xd48d
    b147:	7d 01 1c    	tst	0x11c
    b14a:	27 03       	beq	0x0xb14f
    b14c:	7e b1 32    	jmp	0xb132
    b14f:	7d 01 1e    	tst	0x11e
    b152:	26 08       	bne	0x0xb15c
    b154:	86 0c       	ldaa	#0xc
    b156:	b7 01 1e    	staa	0x11e
    b159:	bd e9 16    	jsr	0xe916
    b15c:	7d 01 7e    	tst	0x17e
    b15f:	27 03       	beq	0x0xb164
    b161:	7e d4 8d    	jmp	0xd48d
    b164:	7e d3 af    	jmp	0xd3af
    b167:	7d 01 1c    	tst	0x11c
    b16a:	2a 03       	bpl	0x0xb16f
    b16c:	7e d5 b2    	jmp	0xd5b2
    b16f:	7d 01 1c    	tst	0x11c
    b172:	27 03       	beq	0x0xb177
    b174:	7e b1 32    	jmp	0xb132
    b177:	7d 01 1e    	tst	0x11e
    b17a:	26 08       	bne	0x0xb184
    b17c:	86 10       	ldaa	#0x10
    b17e:	b7 01 1e    	staa	0x11e
    b181:	bd e9 16    	jsr	0xe916
    b184:	7d 01 7f    	tst	0x17f
    b187:	27 29       	beq	0x0xb1b2
    b189:	2b 1d       	bmi	0x0xb1a8
    b18b:	b6 01 1e    	ldaa	0x11e
    b18e:	4c          	inca
    b18f:	81 1f       	cmpa	#0x1f
    b191:	23 02       	bls	0x0xb195
    b193:	20 06       	bra	0x0xb19b
    b195:	b7 01 1e    	staa	0x11e
    b198:	bd e9 16    	jsr	0xe916
    b19b:	4f          	clra
    b19c:	b7 01 7f    	staa	0x17f
    b19f:	b7 01 7e    	staa	0x17e
    b1a2:	b7 01 1a    	staa	0x11a
    b1a5:	7e d3 af    	jmp	0xd3af
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
    b1cb:	7e d3 af    	jmp	0xd3af
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
    b1f8:	bd e8 da    	jsr	0xe8da
    b1fb:	13 d1 80 0e 	brclr	*0xd1, #0x80, 0x0xb20d
    b1ff:	32          	pula
    b200:	8f          	xgdx
    b201:	83 00 70    	subd	#0x70
    b204:	8f          	xgdx
    b205:	a7 00       	staa	0x0,x
    b207:	bd e9 16    	jsr	0xe916
    b20a:	7e d3 af    	jmp	0xd3af
    b20d:	32          	pula
    b20e:	8f          	xgdx
    b20f:	83 01 30    	subd	#0x130
    b212:	c3 50 40    	addd	#0x5040
    b215:	8f          	xgdx
    b216:	a7 00       	staa	0x0,x
    b218:	bd e9 16    	jsr	0xe916
    b21b:	14 fb 80    	bset	*0xfb, #0x80
    b21e:	7e d3 af    	jmp	0xd3af
    b221:	7d 01 1c    	tst	0x11c
    b224:	2a 03       	bpl	0x0xb229
    b226:	7e d6 46    	jmp	0xd646
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
    b242:	bd e8 da    	jsr	0xe8da
    b245:	7a 01 1e    	dec	0x11e
    b248:	7a 01 1c    	dec	0x11c
    b24b:	26 0d       	bne	0x0xb25a
    b24d:	bd e9 58    	jsr	0xe958
    b250:	20 08       	bra	0x0xb25a
    b252:	86 13       	ldaa	#0x13
    b254:	b7 01 1e    	staa	0x11e
    b257:	bd e9 16    	jsr	0xe916
    b25a:	7d 01 7f    	tst	0x17f
    b25d:	27 10       	beq	0x0xb26f
    b25f:	bd eb 1a    	jsr	0xeb1a
    b262:	4f          	clra
    b263:	b7 01 7f    	staa	0x17f
    b266:	b7 01 7e    	staa	0x17e
    b269:	b7 01 1a    	staa	0x11a
    b26c:	7e d3 af    	jmp	0xd3af
    b26f:	7d 01 1a    	tst	0x11a
    b272:	26 03       	bne	0x0xb277
    b274:	7e d3 af    	jmp	0xd3af
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
    b2a9:	ce d6 a4    	ldx	#0xd6a4
    b2ac:	18 ce 01 30 	ldy	#0x130
    b2b0:	bd ea a1    	jsr	0xeaa1
    b2b3:	7f 01 1a    	clr	0x11a
    b2b6:	86 04       	ldaa	#0x4
    b2b8:	b7 01 1c    	staa	0x11c
    b2bb:	7e d3 af    	jmp	0xd3af
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
    b2d8:	ce d6 e8    	ldx	#0xd6e8
    b2db:	18 ce 01 36 	ldy	#0x136
    b2df:	bd ea a1    	jsr	0xeaa1
    b2e2:	7f 01 1a    	clr	0x11a
    b2e5:	86 04       	ldaa	#0x4
    b2e7:	b7 01 1c    	staa	0x11c
    b2ea:	96 93       	ldaa	*0x93
    b2ec:	c6 93       	ldab	#0x93
    b2ee:	bd af d2    	jsr	0xafd2
    b2f1:	7e d3 af    	jmp	0xd3af
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
    b308:	18 ce ee 40 	ldy	#0xee40
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
    b36b:	7e d3 af    	jmp	0xd3af
    b36e:	ce 50 00    	ldx	#0x5000
    b371:	18 ce ee f0 	ldy	#0xeef0
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
    b390:	7e d3 af    	jmp	0xd3af
    b393:	7d 01 1c    	tst	0x11c
    b396:	2a 03       	bpl	0x0xb39b
    b398:	7e d7 02    	jmp	0xd702
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
    b3b4:	bd e8 da    	jsr	0xe8da
    b3b7:	7a 01 1e    	dec	0x11e
    b3ba:	7a 01 1c    	dec	0x11c
    b3bd:	26 12       	bne	0x0xb3d1
    b3bf:	bd e9 58    	jsr	0xe958
    b3c2:	20 0d       	bra	0x0xb3d1
    b3c4:	7d 01 1e    	tst	0x11e
    b3c7:	26 08       	bne	0x0xb3d1
    b3c9:	86 13       	ldaa	#0x13
    b3cb:	b7 01 1e    	staa	0x11e
    b3ce:	bd e9 16    	jsr	0xe916
    b3d1:	7d 01 7f    	tst	0x17f
    b3d4:	27 10       	beq	0x0xb3e6
    b3d6:	bd eb 1a    	jsr	0xeb1a
    b3d9:	4f          	clra
    b3da:	b7 01 7f    	staa	0x17f
    b3dd:	b7 01 7e    	staa	0x17e
    b3e0:	b7 01 1a    	staa	0x11a
    b3e3:	7e d3 af    	jmp	0xd3af
    b3e6:	7d 01 1a    	tst	0x11a
    b3e9:	26 03       	bne	0x0xb3ee
    b3eb:	7e d3 af    	jmp	0xd3af
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
    b423:	ce d7 5e    	ldx	#0xd75e
    b426:	18 ce 01 30 	ldy	#0x130
    b42a:	bd ea a1    	jsr	0xeaa1
    b42d:	7f 01 1a    	clr	0x11a
    b430:	86 04       	ldaa	#0x4
    b432:	b7 01 1c    	staa	0x11c
    b435:	7e d3 af    	jmp	0xd3af
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
    b455:	ce d7 6a    	ldx	#0xd76a
    b458:	18 ce 01 36 	ldy	#0x136
    b45c:	bd ea a1    	jsr	0xeaa1
    b45f:	7f 01 1a    	clr	0x11a
    b462:	86 04       	ldaa	#0x4
    b464:	b7 01 1c    	staa	0x11c
    b467:	7e d3 af    	jmp	0xd3af
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
    b4a8:	bd e9 ee    	jsr	0xe9ee
    b4ab:	7f 01 1a    	clr	0x11a
    b4ae:	86 03       	ldaa	#0x3
    b4b0:	b7 01 1c    	staa	0x11c
    b4b3:	7e d3 af    	jmp	0xd3af
    b4b6:	b6 01 71    	ldaa	0x171
    b4b9:	4a          	deca
    b4ba:	2a c6       	bpl	0x0xb482
    b4bc:	4f          	clra
    b4bd:	20 c3       	bra	0x0xb482
    b4bf:	7d 01 1c    	tst	0x11c
    b4c2:	2a 03       	bpl	0x0xb4c7
    b4c4:	7e d7 76    	jmp	0xd776
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
    b4e0:	bd e8 da    	jsr	0xe8da
    b4e3:	7a 01 1e    	dec	0x11e
    b4e6:	7a 01 1c    	dec	0x11c
    b4e9:	26 12       	bne	0x0xb4fd
    b4eb:	bd e9 58    	jsr	0xe958
    b4ee:	20 0d       	bra	0x0xb4fd
    b4f0:	7d 01 1e    	tst	0x11e
    b4f3:	26 08       	bne	0x0xb4fd
    b4f5:	86 1e       	ldaa	#0x1e
    b4f7:	b7 01 1e    	staa	0x11e
    b4fa:	bd e9 16    	jsr	0xe916
    b4fd:	7f 01 7e    	clr	0x17e
    b500:	7f 01 7f    	clr	0x17f
    b503:	7d 01 1a    	tst	0x11a
    b506:	26 03       	bne	0x0xb50b
    b508:	7e d3 af    	jmp	0xd3af
    b50b:	b6 01 3c    	ldaa	0x13c
    b50e:	81 41       	cmpa	#0x41
    b510:	26 04       	bne	0x0xb516
    b512:	86 81       	ldaa	#0x81
    b514:	20 06       	bra	0x0xb51c
    b516:	ce 01 3c    	ldx	#0x13c
    b519:	bd ea 48    	jsr	0xea48
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
    b545:	bd e9 ee    	jsr	0xe9ee
    b548:	7f 01 1a    	clr	0x11a
    b54b:	86 03       	ldaa	#0x3
    b54d:	b7 01 1c    	staa	0x11c
    b550:	14 f2 40    	bset	*0xf2, #0x40
    b553:	7e d3 af    	jmp	0xd3af
    b556:	7d 01 1c    	tst	0x11c
    b559:	2a 03       	bpl	0x0xb55e
    b55b:	7e d8 01    	jmp	0xd801
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
    b577:	bd e8 da    	jsr	0xe8da
    b57a:	7a 01 1e    	dec	0x11e
    b57d:	7a 01 1c    	dec	0x11c
    b580:	26 12       	bne	0x0xb594
    b582:	bd e9 58    	jsr	0xe958
    b585:	20 0d       	bra	0x0xb594
    b587:	7d 01 1e    	tst	0x11e
    b58a:	26 08       	bne	0x0xb594
    b58c:	86 13       	ldaa	#0x13
    b58e:	b7 01 1e    	staa	0x11e
    b591:	bd e9 16    	jsr	0xe916
    b594:	7d 01 7f    	tst	0x17f
    b597:	27 10       	beq	0x0xb5a9
    b599:	bd eb 1a    	jsr	0xeb1a
    b59c:	4f          	clra
    b59d:	b7 01 7f    	staa	0x17f
    b5a0:	b7 01 7e    	staa	0x17e
    b5a3:	b7 01 1a    	staa	0x11a
    b5a6:	7e d3 af    	jmp	0xd3af
    b5a9:	7d 01 1a    	tst	0x11a
    b5ac:	26 03       	bne	0x0xb5b1
    b5ae:	7e d3 af    	jmp	0xd3af
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
    b5cb:	bd ea 2a    	jsr	0xea2a
    b5ce:	7f 01 1a    	clr	0x11a
    b5d1:	86 03       	ldaa	#0x3
    b5d3:	b7 01 1c    	staa	0x11c
    b5d6:	96 40       	ldaa	*0x40
    b5d8:	c6 40       	ldab	#0x40
    b5da:	bd af d2    	jsr	0xafd2
    b5dd:	7e d3 af    	jmp	0xd3af
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
    b603:	ce d8 68    	ldx	#0xd868
    b606:	18 ce 01 36 	ldy	#0x136
    b60a:	bd ea a1    	jsr	0xeaa1
    b60d:	7f 01 1a    	clr	0x11a
    b610:	86 04       	ldaa	#0x4
    b612:	b7 01 1c    	staa	0x11c
    b615:	96 41       	ldaa	*0x41
    b617:	c6 41       	ldab	#0x41
    b619:	bd af d2    	jsr	0xafd2
    b61c:	7e d3 af    	jmp	0xd3af
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
    b638:	bd e9 ee    	jsr	0xe9ee
    b63b:	7f 01 1a    	clr	0x11a
    b63e:	86 03       	ldaa	#0x3
    b640:	b7 01 1c    	staa	0x11c
    b643:	96 67       	ldaa	*0x67
    b645:	c6 67       	ldab	#0x67
    b647:	bd af d2    	jsr	0xafd2
    b64a:	7e d3 af    	jmp	0xd3af
    b64d:	96 67       	ldaa	*0x67
    b64f:	4a          	deca
    b650:	84 7f       	anda	#0x7f
    b652:	20 df       	bra	0x0xb633
    b654:	7d 01 1c    	tst	0x11c
    b657:	2a 03       	bpl	0x0xb65c
    b659:	7e d8 7c    	jmp	0xd87c
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
    b675:	bd e8 da    	jsr	0xe8da
    b678:	7a 01 1e    	dec	0x11e
    b67b:	7a 01 1c    	dec	0x11c
    b67e:	26 12       	bne	0x0xb692
    b680:	bd e9 58    	jsr	0xe958
    b683:	20 0d       	bra	0x0xb692
    b685:	7d 01 1e    	tst	0x11e
    b688:	26 08       	bne	0x0xb692
    b68a:	86 13       	ldaa	#0x13
    b68c:	b7 01 1e    	staa	0x11e
    b68f:	bd e9 16    	jsr	0xe916
    b692:	7d 01 7f    	tst	0x17f
    b695:	27 10       	beq	0x0xb6a7
    b697:	bd eb 1a    	jsr	0xeb1a
    b69a:	4f          	clra
    b69b:	b7 01 7f    	staa	0x17f
    b69e:	b7 01 7e    	staa	0x17e
    b6a1:	b7 01 1a    	staa	0x11a
    b6a4:	7e d3 af    	jmp	0xd3af
    b6a7:	7d 01 1a    	tst	0x11a
    b6aa:	26 03       	bne	0x0xb6af
    b6ac:	7e d3 af    	jmp	0xd3af
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
    b6e8:	bd ea 2a    	jsr	0xea2a
    b6eb:	7f 01 1a    	clr	0x11a
    b6ee:	86 03       	ldaa	#0x3
    b6f0:	b7 01 1c    	staa	0x11c
    b6f3:	b6 01 6e    	ldaa	0x16e
    b6f6:	c6 ab       	ldab	#0xab
    b6f8:	bd af d2    	jsr	0xafd2
    b6fb:	7e d3 af    	jmp	0xd3af
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
    b71e:	bd e9 ee    	jsr	0xe9ee
    b721:	7f 01 1a    	clr	0x11a
    b724:	86 03       	ldaa	#0x3
    b726:	b7 01 1c    	staa	0x11c
    b729:	96 3e       	ldaa	*0x3e
    b72b:	c6 3e       	ldab	#0x3e
    b72d:	bd af d2    	jsr	0xafd2
    b730:	7e d3 af    	jmp	0xd3af
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
    b74e:	bd e9 ee    	jsr	0xe9ee
    b751:	7f 01 1a    	clr	0x11a
    b754:	86 03       	ldaa	#0x3
    b756:	b7 01 1c    	staa	0x11c
    b759:	96 3f       	ldaa	*0x3f
    b75b:	c6 3f       	ldab	#0x3f
    b75d:	bd af d2    	jsr	0xafd2
    b760:	7e d3 af    	jmp	0xd3af
    b763:	96 3f       	ldaa	*0x3f
    b765:	4a          	deca
    b766:	2a e1       	bpl	0x0xb749
    b768:	4f          	clra
    b769:	20 de       	bra	0x0xb749
    b76b:	7d 01 1c    	tst	0x11c
    b76e:	2a 03       	bpl	0x0xb773
    b770:	7e d8 de    	jmp	0xd8de
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
    b78c:	bd e8 da    	jsr	0xe8da
    b78f:	7a 01 1e    	dec	0x11e
    b792:	7a 01 1c    	dec	0x11c
    b795:	26 12       	bne	0x0xb7a9
    b797:	bd e9 8b    	jsr	0xe98b
    b79a:	20 0d       	bra	0x0xb7a9
    b79c:	7d 01 1e    	tst	0x11e
    b79f:	26 08       	bne	0x0xb7a9
    b7a1:	86 12       	ldaa	#0x12
    b7a3:	b7 01 1e    	staa	0x11e
    b7a6:	bd e9 16    	jsr	0xe916
    b7a9:	7d 01 7f    	tst	0x17f
    b7ac:	27 10       	beq	0x0xb7be
    b7ae:	bd eb 8c    	jsr	0xeb8c
    b7b1:	4f          	clra
    b7b2:	b7 01 7f    	staa	0x17f
    b7b5:	b7 01 7e    	staa	0x17e
    b7b8:	b7 01 1a    	staa	0x11a
    b7bb:	7e d3 af    	jmp	0xd3af
    b7be:	7d 01 1a    	tst	0x11a
    b7c1:	26 03       	bne	0x0xb7c6
    b7c3:	7e d3 af    	jmp	0xd3af
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
    b7db:	ce d9 6c    	ldx	#0xd96c
    b7de:	18 ce 01 30 	ldy	#0x130
    b7e2:	bd ea 8d    	jsr	0xea8d
    b7e5:	7f 01 1a    	clr	0x11a
    b7e8:	86 03       	ldaa	#0x3
    b7ea:	b7 01 1c    	staa	0x11c
    b7ed:	96 21       	ldaa	*0x21
    b7ef:	c6 21       	ldab	#0x21
    b7f1:	bd af d2    	jsr	0xafd2
    b7f4:	7e d3 af    	jmp	0xd3af
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
    b80d:	ce d9 78    	ldx	#0xd978
    b810:	bd ea 8d    	jsr	0xea8d
    b813:	20 d0       	bra	0x0xb7e5
    b815:	7d 01 1a    	tst	0x11a
    b818:	2b 27       	bmi	0x0xb841
    b81a:	d6 24       	ldab	*0x24
    b81c:	5c          	incb
    b81d:	c1 07       	cmpb	#0x7
    b81f:	25 02       	bcs	0x0xb823
    b821:	c6 06       	ldab	#0x6
    b823:	d7 24       	stab	*0x24
    b825:	ce d9 7e    	ldx	#0xd97e
    b828:	18 ce 01 3c 	ldy	#0x13c
    b82c:	bd ea 8d    	jsr	0xea8d
    b82f:	7f 01 1a    	clr	0x11a
    b832:	86 03       	ldaa	#0x3
    b834:	b7 01 1c    	staa	0x11c
    b837:	96 24       	ldaa	*0x24
    b839:	c6 24       	ldab	#0x24
    b83b:	bd af d2    	jsr	0xafd2
    b83e:	7e d3 af    	jmp	0xd3af
    b841:	d6 24       	ldab	*0x24
    b843:	5a          	decb
    b844:	2a dd       	bpl	0x0xb823
    b846:	5f          	clrb
    b847:	20 da       	bra	0x0xb823
    b849:	7d 01 1c    	tst	0x11c
    b84c:	2a 03       	bpl	0x0xb851
    b84e:	7e d9 93    	jmp	0xd993
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
    b86a:	bd e8 da    	jsr	0xe8da
    b86d:	7a 01 1e    	dec	0x11e
    b870:	7a 01 1c    	dec	0x11c
    b873:	26 12       	bne	0x0xb887
    b875:	bd e9 58    	jsr	0xe958
    b878:	20 0d       	bra	0x0xb887
    b87a:	7d 01 1e    	tst	0x11e
    b87d:	26 08       	bne	0x0xb887
    b87f:	86 13       	ldaa	#0x13
    b881:	b7 01 1e    	staa	0x11e
    b884:	bd e9 16    	jsr	0xe916
    b887:	7d 01 7f    	tst	0x17f
    b88a:	27 10       	beq	0x0xb89c
    b88c:	bd eb 1a    	jsr	0xeb1a
    b88f:	4f          	clra
    b890:	b7 01 7f    	staa	0x17f
    b893:	b7 01 7e    	staa	0x17e
    b896:	b7 01 1a    	staa	0x11a
    b899:	7e d3 af    	jmp	0xd3af
    b89c:	7d 01 1a    	tst	0x11a
    b89f:	26 03       	bne	0x0xb8a4
    b8a1:	7e d3 af    	jmp	0xd3af
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
    b8c5:	ce da 02    	ldx	#0xda02
    b8c8:	18 ce 01 30 	ldy	#0x130
    b8cc:	bd ea a1    	jsr	0xeaa1
    b8cf:	7f 01 1a    	clr	0x11a
    b8d2:	86 04       	ldaa	#0x4
    b8d4:	b7 01 1c    	staa	0x11c
    b8d7:	7e d3 af    	jmp	0xd3af
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
    b8f3:	ce da 16    	ldx	#0xda16
    b8f6:	18 ce 01 36 	ldy	#0x136
    b8fa:	bd ea a1    	jsr	0xeaa1
    b8fd:	7f 01 1a    	clr	0x11a
    b900:	86 04       	ldaa	#0x4
    b902:	b7 01 1c    	staa	0x11c
    b905:	7e d3 af    	jmp	0xd3af
    b908:	d6 69       	ldab	*0x69
    b90a:	5c          	incb
    b90b:	c4 01       	andb	#0x1
    b90d:	d7 69       	stab	*0x69
    b90f:	ce da 1e    	ldx	#0xda1e
    b912:	18 ce 01 3b 	ldy	#0x13b
    b916:	bd ea a1    	jsr	0xeaa1
    b919:	7f 01 1a    	clr	0x11a
    b91c:	86 04       	ldaa	#0x4
    b91e:	b7 01 1c    	staa	0x11c
    b921:	96 69       	ldaa	*0x69
    b923:	c6 69       	ldab	#0x69
    b925:	bd af d2    	jsr	0xafd2
    b928:	7e d3 af    	jmp	0xd3af
    b92b:	7d 01 1c    	tst	0x11c
    b92e:	2a 03       	bpl	0x0xb933
    b930:	7e da 26    	jmp	0xda26
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
    b94c:	bd e8 da    	jsr	0xe8da
    b94f:	7a 01 1e    	dec	0x11e
    b952:	7a 01 1c    	dec	0x11c
    b955:	26 12       	bne	0x0xb969
    b957:	bd e9 58    	jsr	0xe958
    b95a:	20 0d       	bra	0x0xb969
    b95c:	7d 01 1e    	tst	0x11e
    b95f:	26 08       	bne	0x0xb969
    b961:	86 13       	ldaa	#0x13
    b963:	b7 01 1e    	staa	0x11e
    b966:	bd e9 16    	jsr	0xe916
    b969:	7d 01 7f    	tst	0x17f
    b96c:	27 1e       	beq	0x0xb98c
    b96e:	2a 10       	bpl	0x0xb980
    b970:	bd eb 1a    	jsr	0xeb1a
    b973:	4f          	clra
    b974:	b7 01 7f    	staa	0x17f
    b977:	b7 01 7e    	staa	0x17e
    b97a:	b7 01 1a    	staa	0x11a
    b97d:	7e d3 af    	jmp	0xd3af
    b980:	b6 01 1e    	ldaa	0x11e
    b983:	81 19       	cmpa	#0x19
    b985:	27 ec       	beq	0x0xb973
    b987:	bd eb 1a    	jsr	0xeb1a
    b98a:	20 e7       	bra	0x0xb973
    b98c:	7d 01 1a    	tst	0x11a
    b98f:	26 03       	bne	0x0xb994
    b991:	7e d3 af    	jmp	0xd3af
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
    b9ab:	ce da 78    	ldx	#0xda78
    b9ae:	18 ce 01 30 	ldy	#0x130
    b9b2:	bd ea a1    	jsr	0xeaa1
    b9b5:	7f 01 1a    	clr	0x11a
    b9b8:	86 04       	ldaa	#0x4
    b9ba:	b7 01 1c    	staa	0x11c
    b9bd:	96 6b       	ldaa	*0x6b
    b9bf:	c6 6b       	ldab	#0x6b
    b9c1:	bd af d2    	jsr	0xafd2
    b9c4:	7e d3 af    	jmp	0xd3af
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
    b9df:	ce da 84    	ldx	#0xda84
    b9e2:	18 ce 01 36 	ldy	#0x136
    b9e6:	bd ea a1    	jsr	0xeaa1
    b9e9:	7f 01 1a    	clr	0x11a
    b9ec:	86 04       	ldaa	#0x4
    b9ee:	b7 01 1c    	staa	0x11c
    b9f1:	96 68       	ldaa	*0x68
    b9f3:	c6 68       	ldab	#0x68
    b9f5:	bd af d2    	jsr	0xafd2
    b9f8:	7e d3 af    	jmp	0xd3af
    b9fb:	d6 68       	ldab	*0x68
    b9fd:	5a          	decb
    b9fe:	2a dd       	bpl	0x0xb9dd
    ba00:	5f          	clrb
    ba01:	20 da       	bra	0x0xb9dd
    ba03:	7e d3 af    	jmp	0xd3af
    ba06:	7d 01 1c    	tst	0x11c
    ba09:	2a 03       	bpl	0x0xba0e
    ba0b:	7e da a4    	jmp	0xdaa4
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
    ba27:	bd e8 da    	jsr	0xe8da
    ba2a:	7a 01 1e    	dec	0x11e
    ba2d:	7a 01 1c    	dec	0x11c
    ba30:	26 12       	bne	0x0xba44
    ba32:	bd e9 58    	jsr	0xe958
    ba35:	20 0d       	bra	0x0xba44
    ba37:	7d 01 1e    	tst	0x11e
    ba3a:	26 08       	bne	0x0xba44
    ba3c:	86 09       	ldaa	#0x9
    ba3e:	b7 01 1e    	staa	0x11e
    ba41:	bd e9 16    	jsr	0xe916
    ba44:	7d 01 7f    	tst	0x17f
    ba47:	27 10       	beq	0x0xba59
    ba49:	bd ea d9    	jsr	0xead9
    ba4c:	4f          	clra
    ba4d:	b7 01 7f    	staa	0x17f
    ba50:	b7 01 7e    	staa	0x17e
    ba53:	b7 01 1a    	staa	0x11a
    ba56:	7e d3 af    	jmp	0xd3af
    ba59:	7d 01 7e    	tst	0x17e
    ba5c:	27 0c       	beq	0x0xba6a
    ba5e:	bd ea c2    	jsr	0xeac2
    ba61:	7f 01 7e    	clr	0x17e
    ba64:	7f 01 1a    	clr	0x11a
    ba67:	7e d3 af    	jmp	0xd3af
    ba6a:	7d 01 1a    	tst	0x11a
    ba6d:	26 03       	bne	0x0xba72
    ba6f:	7e d3 af    	jmp	0xd3af
    ba72:	b6 01 1e    	ldaa	0x11e
    ba75:	81 09       	cmpa	#0x9
    ba77:	26 29       	bne	0x0xbaa2
    ba79:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xba83
    ba7d:	7f 01 1a    	clr	0x11a
    ba80:	7e d3 af    	jmp	0xd3af
    ba83:	ce 00 76    	ldx	#0x76
    ba86:	bd bb 1b    	jsr	0xbb1b
    ba89:	bd bb 4f    	jsr	0xbb4f
    ba8c:	16          	tab
    ba8d:	ce de d3    	ldx	#0xded3
    ba90:	18 ce 01 26 	ldy	#0x126
    ba94:	bd ea a1    	jsr	0xeaa1
    ba97:	86 04       	ldaa	#0x4
    ba99:	b7 01 1c    	staa	0x11c
    ba9c:	7f 01 1a    	clr	0x11a
    ba9f:	7e d3 af    	jmp	0xd3af
    baa2:	81 0e       	cmpa	#0xe
    baa4:	26 29       	bne	0x0xbacf
    baa6:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xbab0
    baaa:	7f 01 1a    	clr	0x11a
    baad:	7e d3 af    	jmp	0xd3af
    bab0:	ce 00 77    	ldx	#0x77
    bab3:	bd bb 1b    	jsr	0xbb1b
    bab6:	bd bb 4f    	jsr	0xbb4f
    bab9:	16          	tab
    baba:	ce de d3    	ldx	#0xded3
    babd:	18 ce 01 2b 	ldy	#0x12b
    bac1:	bd ea a1    	jsr	0xeaa1
    bac4:	86 04       	ldaa	#0x4
    bac6:	b7 01 1c    	staa	0x11c
    bac9:	7f 01 1a    	clr	0x11a
    bacc:	7e d3 af    	jmp	0xd3af
    bacf:	81 19       	cmpa	#0x19
    bad1:	26 24       	bne	0x0xbaf7
    bad3:	ce 00 78    	ldx	#0x78
    bad6:	bd bb 1b    	jsr	0xbb1b
    bad9:	bd bb 9b    	jsr	0xbb9b
    badc:	ce 01 37    	ldx	#0x137
    badf:	7d 00 f6    	tst	0xf6
    bae2:	2a 05       	bpl	0x0xbae9
    bae4:	bd ea 2a    	jsr	0xea2a
    bae7:	20 03       	bra	0x0xbaec
    bae9:	bd e9 ee    	jsr	0xe9ee
    baec:	86 03       	ldaa	#0x3
    baee:	b7 01 1c    	staa	0x11c
    baf1:	7f 01 1a    	clr	0x11a
    baf4:	7e d3 af    	jmp	0xd3af
    baf7:	ce 00 79    	ldx	#0x79
    bafa:	bd bb 1b    	jsr	0xbb1b
    bafd:	bd bb 9b    	jsr	0xbb9b
    bb00:	ce 01 3c    	ldx	#0x13c
    bb03:	7d 00 f6    	tst	0xf6
    bb06:	2a 05       	bpl	0x0xbb0d
    bb08:	bd ea 2a    	jsr	0xea2a
    bb0b:	20 03       	bra	0x0xbb10
    bb0d:	bd e9 ee    	jsr	0xe9ee
    bb10:	86 03       	ldaa	#0x3
    bb12:	b7 01 1c    	staa	0x11c
    bb15:	7f 01 1a    	clr	0x11a
    bb18:	7e d3 af    	jmp	0xd3af
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
    bbbc:	7e dc 41    	jmp	0xdc41
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
    bbd8:	bd e8 da    	jsr	0xe8da
    bbdb:	7a 01 1e    	dec	0x11e
    bbde:	7a 01 1c    	dec	0x11c
    bbe1:	26 12       	bne	0x0xbbf5
    bbe3:	bd e9 58    	jsr	0xe958
    bbe6:	20 0d       	bra	0x0xbbf5
    bbe8:	7d 01 1e    	tst	0x11e
    bbeb:	26 08       	bne	0x0xbbf5
    bbed:	86 19       	ldaa	#0x19
    bbef:	b7 01 1e    	staa	0x11e
    bbf2:	bd e9 16    	jsr	0xe916
    bbf5:	7d 01 7f    	tst	0x17f
    bbf8:	27 10       	beq	0x0xbc0a
    bbfa:	bd ea d9    	jsr	0xead9
    bbfd:	4f          	clra
    bbfe:	b7 01 7f    	staa	0x17f
    bc01:	b7 01 7e    	staa	0x17e
    bc04:	b7 01 1a    	staa	0x11a
    bc07:	7e d3 af    	jmp	0xd3af
    bc0a:	7d 01 1a    	tst	0x11a
    bc0d:	26 03       	bne	0x0xbc12
    bc0f:	7e d3 af    	jmp	0xd3af
    bc12:	b6 01 1e    	ldaa	0x11e
    bc15:	81 19       	cmpa	#0x19
    bc17:	26 34       	bne	0x0xbc4d
    bc19:	18 ce 01 36 	ldy	#0x136
    bc1d:	ce dc a2    	ldx	#0xdca2
    bc20:	7d 01 1a    	tst	0x11a
    bc23:	2b 20       	bmi	0x0xbc45
    bc25:	d6 8e       	ldab	*0x8e
    bc27:	5c          	incb
    bc28:	c1 03       	cmpb	#0x3
    bc2a:	25 02       	bcs	0x0xbc2e
    bc2c:	c6 02       	ldab	#0x2
    bc2e:	d7 8e       	stab	*0x8e
    bc30:	bd ea a1    	jsr	0xeaa1
    bc33:	7f 01 1a    	clr	0x11a
    bc36:	86 04       	ldaa	#0x4
    bc38:	b7 01 1c    	staa	0x11c
    bc3b:	96 8e       	ldaa	*0x8e
    bc3d:	c6 8e       	ldab	#0x8e
    bc3f:	bd af d2    	jsr	0xafd2
    bc42:	7e d3 af    	jmp	0xd3af
    bc45:	d6 8e       	ldab	*0x8e
    bc47:	5a          	decb
    bc48:	2a e4       	bpl	0x0xbc2e
    bc4a:	5f          	clrb
    bc4b:	20 e1       	bra	0x0xbc2e
    bc4d:	18 ce 01 3b 	ldy	#0x13b
    bc51:	ce dc ae    	ldx	#0xdcae
    bc54:	7d 01 1a    	tst	0x11a
    bc57:	2b 20       	bmi	0x0xbc79
    bc59:	d6 8f       	ldab	*0x8f
    bc5b:	5c          	incb
    bc5c:	c1 02       	cmpb	#0x2
    bc5e:	25 02       	bcs	0x0xbc62
    bc60:	c6 01       	ldab	#0x1
    bc62:	d7 8f       	stab	*0x8f
    bc64:	bd ea a1    	jsr	0xeaa1
    bc67:	7f 01 1a    	clr	0x11a
    bc6a:	86 04       	ldaa	#0x4
    bc6c:	b7 01 1c    	staa	0x11c
    bc6f:	96 8f       	ldaa	*0x8f
    bc71:	c6 8f       	ldab	#0x8f
    bc73:	bd af d2    	jsr	0xafd2
    bc76:	7e d3 af    	jmp	0xd3af
    bc79:	d6 8f       	ldab	*0x8f
    bc7b:	5a          	decb
    bc7c:	2a e4       	bpl	0x0xbc62
    bc7e:	5f          	clrb
    bc7f:	20 e1       	bra	0x0xbc62
    bc81:	7d 01 1c    	tst	0x11c
    bc84:	2a 03       	bpl	0x0xbc89
    bc86:	7e dc b6    	jmp	0xdcb6
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
    bca2:	bd e8 da    	jsr	0xe8da
    bca5:	7a 01 1e    	dec	0x11e
    bca8:	7a 01 1c    	dec	0x11c
    bcab:	26 08       	bne	0x0xbcb5
    bcad:	86 11       	ldaa	#0x11
    bcaf:	b7 01 1e    	staa	0x11e
    bcb2:	bd e9 16    	jsr	0xe916
    bcb5:	4f          	clra
    bcb6:	b7 01 7f    	staa	0x17f
    bcb9:	b7 01 7e    	staa	0x17e
    bcbc:	7d 01 1a    	tst	0x11a
    bcbf:	26 03       	bne	0x0xbcc4
    bcc1:	7e d3 af    	jmp	0xd3af
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
    bcee:	7e d3 af    	jmp	0xd3af
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
    bd0c:	7e dd d4    	jmp	0xddd4
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
    bd28:	bd e8 da    	jsr	0xe8da
    bd2b:	7a 01 1e    	dec	0x11e
    bd2e:	7a 01 1c    	dec	0x11c
    bd31:	26 08       	bne	0x0xbd3b
    bd33:	86 1e       	ldaa	#0x1e
    bd35:	b7 01 1e    	staa	0x11e
    bd38:	bd e9 16    	jsr	0xe916
    bd3b:	7f 01 7e    	clr	0x17e
    bd3e:	7f 01 7f    	clr	0x17f
    bd41:	7d 01 1a    	tst	0x11a
    bd44:	26 03       	bne	0x0xbd49
    bd46:	7e d3 af    	jmp	0xd3af
    bd49:	b6 50 21    	ldaa	0x5021
    bd4c:	7d 01 1a    	tst	0x11a
    bd4f:	2b 0e       	bmi	0x0xbd5f
    bd51:	4c          	inca
    bd52:	84 7f       	anda	#0x7f
    bd54:	b7 50 21    	staa	0x5021
    bd57:	ce 01 3c    	ldx	#0x13c
    bd5a:	bd e9 ee    	jsr	0xe9ee
    bd5d:	20 03       	bra	0x0xbd62
    bd5f:	4a          	deca
    bd60:	20 f0       	bra	0x0xbd52
    bd62:	7f 01 1a    	clr	0x11a
    bd65:	86 03       	ldaa	#0x3
    bd67:	b7 01 1c    	staa	0x11c
    bd6a:	14 fb 80    	bset	*0xfb, #0x80
    bd6d:	7e d3 af    	jmp	0xd3af
    bd70:	7d 01 1c    	tst	0x11c
    bd73:	2a 03       	bpl	0x0xbd78
    bd75:	7e de 17    	jmp	0xde17
    bd78:	7d 01 1c    	tst	0x11c
    bd7b:	27 03       	beq	0x0xbd80
    bd7d:	7e b1 32    	jmp	0xb132
    bd80:	7d 01 1e    	tst	0x11e
    bd83:	26 08       	bne	0x0xbd8d
    bd85:	86 14       	ldaa	#0x14
    bd87:	b7 01 1e    	staa	0x11e
    bd8a:	bd e9 16    	jsr	0xe916
    bd8d:	7e d3 af    	jmp	0xd3af
    bd90:	7d 01 1c    	tst	0x11c
    bd93:	2a 03       	bpl	0x0xbd98
    bd95:	7e de 72    	jmp	0xde72
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
    bdb1:	bd e8 da    	jsr	0xe8da
    bdb4:	7a 01 1e    	dec	0x11e
    bdb7:	7a 01 1c    	dec	0x11c
    bdba:	26 12       	bne	0x0xbdce
    bdbc:	bd e9 58    	jsr	0xe958
    bdbf:	20 0d       	bra	0x0xbdce
    bdc1:	7d 01 1e    	tst	0x11e
    bdc4:	26 08       	bne	0x0xbdce
    bdc6:	86 03       	ldaa	#0x3
    bdc8:	b7 01 1e    	staa	0x11e
    bdcb:	bd e9 16    	jsr	0xe916
    bdce:	7d 01 7f    	tst	0x17f
    bdd1:	27 10       	beq	0x0xbde3
    bdd3:	bd eb 1a    	jsr	0xeb1a
    bdd6:	4f          	clra
    bdd7:	b7 01 7f    	staa	0x17f
    bdda:	b7 01 7e    	staa	0x17e
    bddd:	b7 01 1a    	staa	0x11a
    bde0:	7e d3 af    	jmp	0xd3af
    bde3:	7d 01 7e    	tst	0x17e
    bde6:	27 0c       	beq	0x0xbdf4
    bde8:	bd ea c2    	jsr	0xeac2
    bdeb:	7f 01 7e    	clr	0x17e
    bdee:	7f 01 1a    	clr	0x11a
    bdf1:	7e d3 af    	jmp	0xd3af
    bdf4:	7d 01 1a    	tst	0x11a
    bdf7:	26 03       	bne	0x0xbdfc
    bdf9:	7e d3 af    	jmp	0xd3af
    bdfc:	ce de d3    	ldx	#0xded3
    bdff:	b6 01 1e    	ldaa	0x11e
    be02:	81 03       	cmpa	#0x3
    be04:	26 1a       	bne	0x0xbe20
    be06:	18 ce 01 20 	ldy	#0x120
    be0a:	d6 60       	ldab	*0x60
    be0c:	bd be ee    	jsr	0xbeee
    be0f:	d7 60       	stab	*0x60
    be11:	bd ea a1    	jsr	0xeaa1
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
    be2f:	bd ea a1    	jsr	0xeaa1
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
    be4d:	bd ea a1    	jsr	0xeaa1
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
    be6f:	bd e9 ee    	jsr	0xe9ee
    be72:	7f 01 1a    	clr	0x11a
    be75:	86 03       	ldaa	#0x3
    be77:	b7 01 1c    	staa	0x11c
    be7a:	96 63       	ldaa	*0x63
    be7c:	c6 63       	ldab	#0x63
    be7e:	bd af d2    	jsr	0xafd2
    be81:	7e d3 af    	jmp	0xd3af
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
    be9e:	bd e9 ee    	jsr	0xe9ee
    bea1:	7f 01 1a    	clr	0x11a
    bea4:	86 03       	ldaa	#0x3
    bea6:	b7 01 1c    	staa	0x11c
    bea9:	96 64       	ldaa	*0x64
    beab:	c6 64       	ldab	#0x64
    bead:	bd af d2    	jsr	0xafd2
    beb0:	7e d3 af    	jmp	0xd3af
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
    bec9:	bd e9 ee    	jsr	0xe9ee
    becc:	7f 01 1a    	clr	0x11a
    becf:	86 03       	ldaa	#0x3
    bed1:	b7 01 1c    	staa	0x11c
    bed4:	96 65       	ldaa	*0x65
    bed6:	c6 65       	ldab	#0x65
    bed8:	bd af d2    	jsr	0xafd2
    bedb:	7e d3 af    	jmp	0xd3af
    bede:	96 65       	ldaa	*0x65
    bee0:	4a          	deca
    bee1:	84 7f       	anda	#0x7f
    bee3:	20 df       	bra	0x0xbec4
    bee5:	b7 01 1c    	staa	0x11c
    bee8:	7f 01 1a    	clr	0x11a
    beeb:	7e d3 af    	jmp	0xd3af
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
    bf1f:	7e df 37    	jmp	0xdf37
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
    bf3b:	bd e8 da    	jsr	0xe8da
    bf3e:	7a 01 1e    	dec	0x11e
    bf41:	7a 01 1c    	dec	0x11c
    bf44:	26 12       	bne	0x0xbf58
    bf46:	bd e9 58    	jsr	0xe958
    bf49:	20 0d       	bra	0x0xbf58
    bf4b:	7d 01 1e    	tst	0x11e
    bf4e:	26 08       	bne	0x0xbf58
    bf50:	86 13       	ldaa	#0x13
    bf52:	b7 01 1e    	staa	0x11e
    bf55:	bd e9 16    	jsr	0xe916
    bf58:	7d 01 7f    	tst	0x17f
    bf5b:	27 10       	beq	0x0xbf6d
    bf5d:	bd eb 1a    	jsr	0xeb1a
    bf60:	4f          	clra
    bf61:	b7 01 7f    	staa	0x17f
    bf64:	b7 01 7e    	staa	0x17e
    bf67:	b7 01 1a    	staa	0x11a
    bf6a:	7e d3 af    	jmp	0xd3af
    bf6d:	7d 01 1a    	tst	0x11a
    bf70:	26 03       	bne	0x0xbf75
    bf72:	7e d3 af    	jmp	0xd3af
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
    bf8b:	bd e9 ee    	jsr	0xe9ee
    bf8e:	7f 01 1a    	clr	0x11a
    bf91:	86 03       	ldaa	#0x3
    bf93:	b7 01 1c    	staa	0x11c
    bf96:	96 54       	ldaa	*0x54
    bf98:	c6 54       	ldab	#0x54
    bf9a:	bd af d2    	jsr	0xafd2
    bf9d:	7e d3 af    	jmp	0xd3af
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
    bfba:	bd e9 ee    	jsr	0xe9ee
    bfbd:	7f 01 1a    	clr	0x11a
    bfc0:	86 03       	ldaa	#0x3
    bfc2:	b7 01 1c    	staa	0x11c
    bfc5:	96 59       	ldaa	*0x59
    bfc7:	c6 59       	ldab	#0x59
    bfc9:	bd af d2    	jsr	0xafd2
    bfcc:	7e d3 af    	jmp	0xd3af
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
    bfe5:	bd e9 ee    	jsr	0xe9ee
    bfe8:	7f 01 1a    	clr	0x11a
    bfeb:	86 03       	ldaa	#0x3
    bfed:	b7 01 1c    	staa	0x11c
    bff0:	96 5e       	ldaa	*0x5e
    bff2:	c6 5e       	ldab	#0x5e
    bff4:	bd af d2    	jsr	0xafd2
    bff7:	7e d3 af    	jmp	0xd3af
    bffa:	96 5e       	ldaa	*0x5e
    bffc:	4a          	deca
    bffd:	84 7f       	anda	#0x7f
    bfff:	20 df       	bra	0x0xbfe0
    c001:	7d 01 1c    	tst	0x11c
    c004:	2a 03       	bpl	0x0xc009
    c006:	7e df 8a    	jmp	0xdf8a
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
    c022:	bd e8 da    	jsr	0xe8da
    c025:	7a 01 1e    	dec	0x11e
    c028:	7a 01 1c    	dec	0x11c
    c02b:	26 12       	bne	0x0xc03f
    c02d:	bd e9 58    	jsr	0xe958
    c030:	20 0d       	bra	0x0xc03f
    c032:	7d 01 1e    	tst	0x11e
    c035:	26 08       	bne	0x0xc03f
    c037:	86 13       	ldaa	#0x13
    c039:	b7 01 1e    	staa	0x11e
    c03c:	bd e9 16    	jsr	0xe916
    c03f:	7d 01 7f    	tst	0x17f
    c042:	27 10       	beq	0x0xc054
    c044:	bd eb 1a    	jsr	0xeb1a
    c047:	4f          	clra
    c048:	b7 01 7f    	staa	0x17f
    c04b:	b7 01 7e    	staa	0x17e
    c04e:	b7 01 1a    	staa	0x11a
    c051:	7e d3 af    	jmp	0xd3af
    c054:	7d 01 1a    	tst	0x11a
    c057:	26 03       	bne	0x0xc05c
    c059:	7e d3 af    	jmp	0xd3af
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
    c072:	bd e9 ee    	jsr	0xe9ee
    c075:	7f 01 1a    	clr	0x11a
    c078:	86 03       	ldaa	#0x3
    c07a:	b7 01 1c    	staa	0x11c
    c07d:	96 90       	ldaa	*0x90
    c07f:	c6 90       	ldab	#0x90
    c081:	bd af d2    	jsr	0xafd2
    c084:	7e d3 af    	jmp	0xd3af
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
    c0a1:	bd e9 ee    	jsr	0xe9ee
    c0a4:	7f 01 1a    	clr	0x11a
    c0a7:	86 03       	ldaa	#0x3
    c0a9:	b7 01 1c    	staa	0x11c
    c0ac:	96 91       	ldaa	*0x91
    c0ae:	c6 91       	ldab	#0x91
    c0b0:	bd af d2    	jsr	0xafd2
    c0b3:	7e d3 af    	jmp	0xd3af
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
    c0cc:	bd e9 ee    	jsr	0xe9ee
    c0cf:	7f 01 1a    	clr	0x11a
    c0d2:	86 03       	ldaa	#0x3
    c0d4:	b7 01 1c    	staa	0x11c
    c0d7:	96 92       	ldaa	*0x92
    c0d9:	c6 92       	ldab	#0x92
    c0db:	bd af d2    	jsr	0xafd2
    c0de:	7e d3 af    	jmp	0xd3af
    c0e1:	96 92       	ldaa	*0x92
    c0e3:	4a          	deca
    c0e4:	84 7f       	anda	#0x7f
    c0e6:	20 df       	bra	0x0xc0c7
    c0e8:	7d 01 1c    	tst	0x11c
    c0eb:	2a 03       	bpl	0x0xc0f0
    c0ed:	7e e0 26    	jmp	0xe026
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
    c109:	bd e8 da    	jsr	0xe8da
    c10c:	7a 01 1e    	dec	0x11e
    c10f:	7a 01 1c    	dec	0x11c
    c112:	26 12       	bne	0x0xc126
    c114:	bd e9 58    	jsr	0xe958
    c117:	20 0d       	bra	0x0xc126
    c119:	7d 01 1e    	tst	0x11e
    c11c:	26 08       	bne	0x0xc126
    c11e:	86 13       	ldaa	#0x13
    c120:	b7 01 1e    	staa	0x11e
    c123:	bd e9 16    	jsr	0xe916
    c126:	7d 01 7f    	tst	0x17f
    c129:	27 1e       	beq	0x0xc149
    c12b:	2a 10       	bpl	0x0xc13d
    c12d:	bd eb 1a    	jsr	0xeb1a
    c130:	4f          	clra
    c131:	b7 01 7f    	staa	0x17f
    c134:	b7 01 7e    	staa	0x17e
    c137:	b7 01 1a    	staa	0x11a
    c13a:	7e d3 af    	jmp	0xd3af
    c13d:	b6 01 1e    	ldaa	0x11e
    c140:	81 19       	cmpa	#0x19
    c142:	27 ec       	beq	0x0xc130
    c144:	bd eb 1a    	jsr	0xeb1a
    c147:	20 e7       	bra	0x0xc130
    c149:	7d 01 1a    	tst	0x11a
    c14c:	26 03       	bne	0x0xc151
    c14e:	7e d3 af    	jmp	0xd3af
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
    c168:	ce e0 89    	ldx	#0xe089
    c16b:	18 ce 01 30 	ldy	#0x130
    c16f:	bd ea a1    	jsr	0xeaa1
    c172:	7f 01 1a    	clr	0x11a
    c175:	86 04       	ldaa	#0x4
    c177:	b7 01 1c    	staa	0x11c
    c17a:	96 4b       	ldaa	*0x4b
    c17c:	c6 4b       	ldab	#0x4b
    c17e:	bd af d2    	jsr	0xafd2
    c181:	7e d3 af    	jmp	0xd3af
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
    c1a0:	ce e0 a5    	ldx	#0xe0a5
    c1a3:	18 ce 01 36 	ldy	#0x136
    c1a7:	bd ea a1    	jsr	0xeaa1
    c1aa:	7f 01 1a    	clr	0x11a
    c1ad:	86 04       	ldaa	#0x4
    c1af:	b7 01 1c    	staa	0x11c
    c1b2:	96 66       	ldaa	*0x66
    c1b4:	c6 66       	ldab	#0x66
    c1b6:	bd af d2    	jsr	0xafd2
    c1b9:	7e d3 af    	jmp	0xd3af
    c1bc:	d6 66       	ldab	*0x66
    c1be:	5a          	decb
    c1bf:	2a dd       	bpl	0x0xc19e
    c1c1:	5f          	clrb
    c1c2:	20 da       	bra	0x0xc19e
    c1c4:	4f          	clra
    c1c5:	b7 01 7e    	staa	0x17e
    c1c8:	b7 01 7f    	staa	0x17f
    c1cb:	b7 01 1a    	staa	0x11a
    c1ce:	7e d3 af    	jmp	0xd3af
    c1d1:	7d 01 1c    	tst	0x11c
    c1d4:	2a 03       	bpl	0x0xc1d9
    c1d6:	7e e0 b5    	jmp	0xe0b5
    c1d9:	7d 01 1e    	tst	0x11e
    c1dc:	26 08       	bne	0x0xc1e6
    c1de:	7d 01 1c    	tst	0x11c
    c1e1:	27 1f       	beq	0x0xc202
    c1e3:	7e b1 32    	jmp	0xb132
    c1e6:	7d 01 1c    	tst	0x11c
    c1e9:	27 24       	beq	0x0xc20f
    c1eb:	cc 01 20    	ldd	#0x120
    c1ee:	fb 01 1e    	addb	0x11e
    c1f1:	8f          	xgdx
    c1f2:	bd e8 da    	jsr	0xe8da
    c1f5:	7a 01 1e    	dec	0x11e
    c1f8:	7a 01 1c    	dec	0x11c
    c1fb:	26 12       	bne	0x0xc20f
    c1fd:	bd e9 8b    	jsr	0xe98b
    c200:	20 0d       	bra	0x0xc20f
    c202:	7d 01 1e    	tst	0x11e
    c205:	26 08       	bne	0x0xc20f
    c207:	86 12       	ldaa	#0x12
    c209:	b7 01 1e    	staa	0x11e
    c20c:	bd e9 16    	jsr	0xe916
    c20f:	7d 01 7f    	tst	0x17f
    c212:	27 10       	beq	0x0xc224
    c214:	bd eb 8c    	jsr	0xeb8c
    c217:	4f          	clra
    c218:	b7 01 7f    	staa	0x17f
    c21b:	b7 01 7e    	staa	0x17e
    c21e:	b7 01 1a    	staa	0x11a
    c221:	7e d3 af    	jmp	0xd3af
    c224:	7d 01 1a    	tst	0x11a
    c227:	26 03       	bne	0x0xc22c
    c229:	7e d3 af    	jmp	0xd3af
    c22c:	b6 01 1e    	ldaa	0x11e
    c22f:	81 12       	cmpa	#0x12
    c231:	26 2b       	bne	0x0xc25e
    c233:	b6 01 1a    	ldaa	0x11a
    c236:	2b 1f       	bmi	0x0xc257
    c238:	96 6e       	ldaa	*0x6e
    c23a:	4c          	inca
    c23b:	84 7f       	anda	#0x7f
    c23d:	97 6e       	staa	*0x6e
    c23f:	ce 01 30    	ldx	#0x130
    c242:	bd e9 ee    	jsr	0xe9ee
    c245:	7f 01 1a    	clr	0x11a
    c248:	86 03       	ldaa	#0x3
    c24a:	b7 01 1c    	staa	0x11c
    c24d:	96 6e       	ldaa	*0x6e
    c24f:	c6 6e       	ldab	#0x6e
    c251:	bd af d2    	jsr	0xafd2
    c254:	7e d3 af    	jmp	0xd3af
    c257:	96 6e       	ldaa	*0x6e
    c259:	4a          	deca
    c25a:	84 7f       	anda	#0x7f
    c25c:	20 df       	bra	0x0xc23d
    c25e:	81 16       	cmpa	#0x16
    c260:	26 2b       	bne	0x0xc28d
    c262:	b6 01 1a    	ldaa	0x11a
    c265:	2b 1f       	bmi	0x0xc286
    c267:	96 48       	ldaa	*0x48
    c269:	4c          	inca
    c26a:	84 7f       	anda	#0x7f
    c26c:	97 48       	staa	*0x48
    c26e:	ce 01 34    	ldx	#0x134
    c271:	bd e9 ee    	jsr	0xe9ee
    c274:	7f 01 1a    	clr	0x11a
    c277:	86 03       	ldaa	#0x3
    c279:	b7 01 1c    	staa	0x11c
    c27c:	96 48       	ldaa	*0x48
    c27e:	c6 48       	ldab	#0x48
    c280:	bd af d2    	jsr	0xafd2
    c283:	7e d3 af    	jmp	0xd3af
    c286:	96 48       	ldaa	*0x48
    c288:	4a          	deca
    c289:	84 7f       	anda	#0x7f
    c28b:	20 df       	bra	0x0xc26c
    c28d:	81 1a       	cmpa	#0x1a
    c28f:	26 2b       	bne	0x0xc2bc
    c291:	b6 01 1a    	ldaa	0x11a
    c294:	2b 1f       	bmi	0x0xc2b5
    c296:	96 6f       	ldaa	*0x6f
    c298:	4c          	inca
    c299:	84 7f       	anda	#0x7f
    c29b:	97 6f       	staa	*0x6f
    c29d:	ce 01 38    	ldx	#0x138
    c2a0:	bd e9 ee    	jsr	0xe9ee
    c2a3:	7f 01 1a    	clr	0x11a
    c2a6:	86 03       	ldaa	#0x3
    c2a8:	b7 01 1c    	staa	0x11c
    c2ab:	96 6f       	ldaa	*0x6f
    c2ad:	c6 6f       	ldab	#0x6f
    c2af:	bd af d2    	jsr	0xafd2
    c2b2:	7e d3 af    	jmp	0xd3af
    c2b5:	96 6f       	ldaa	*0x6f
    c2b7:	4a          	deca
    c2b8:	84 7f       	anda	#0x7f
    c2ba:	20 df       	bra	0x0xc29b
    c2bc:	b6 01 1a    	ldaa	0x11a
    c2bf:	2b 21       	bmi	0x0xc2e2
    c2c1:	96 70       	ldaa	*0x70
    c2c3:	4c          	inca
    c2c4:	2a 02       	bpl	0x0xc2c8
    c2c6:	86 7f       	ldaa	#0x7f
    c2c8:	97 70       	staa	*0x70
    c2ca:	ce 01 3c    	ldx	#0x13c
    c2cd:	bd ea b5    	jsr	0xeab5
    c2d0:	7f 01 1a    	clr	0x11a
    c2d3:	86 03       	ldaa	#0x3
    c2d5:	b7 01 1c    	staa	0x11c
    c2d8:	96 70       	ldaa	*0x70
    c2da:	c6 70       	ldab	#0x70
    c2dc:	bd af d2    	jsr	0xafd2
    c2df:	7e d3 af    	jmp	0xd3af
    c2e2:	96 70       	ldaa	*0x70
    c2e4:	4a          	deca
    c2e5:	2a e1       	bpl	0x0xc2c8
    c2e7:	4f          	clra
    c2e8:	20 de       	bra	0x0xc2c8
    c2ea:	7f 01 1a    	clr	0x11a
    c2ed:	7f 01 7e    	clr	0x17e
    c2f0:	7e d3 af    	jmp	0xd3af
    c2f3:	7d 01 1c    	tst	0x11c
    c2f6:	2a 03       	bpl	0x0xc2fb
    c2f8:	7e e1 0c    	jmp	0xe10c
    c2fb:	7d 01 1e    	tst	0x11e
    c2fe:	26 08       	bne	0x0xc308
    c300:	7d 01 1c    	tst	0x11c
    c303:	27 1f       	beq	0x0xc324
    c305:	7e b1 32    	jmp	0xb132
    c308:	7d 01 1c    	tst	0x11c
    c30b:	27 24       	beq	0x0xc331
    c30d:	cc 01 20    	ldd	#0x120
    c310:	fb 01 1e    	addb	0x11e
    c313:	8f          	xgdx
    c314:	bd e8 da    	jsr	0xe8da
    c317:	7a 01 1e    	dec	0x11e
    c31a:	7a 01 1c    	dec	0x11c
    c31d:	26 12       	bne	0x0xc331
    c31f:	bd e9 58    	jsr	0xe958
    c322:	20 0d       	bra	0x0xc331
    c324:	7d 01 1e    	tst	0x11e
    c327:	26 08       	bne	0x0xc331
    c329:	86 03       	ldaa	#0x3
    c32b:	b7 01 1e    	staa	0x11e
    c32e:	bd e9 16    	jsr	0xe916
    c331:	7d 01 7f    	tst	0x17f
    c334:	27 10       	beq	0x0xc346
    c336:	bd eb 1a    	jsr	0xeb1a
    c339:	4f          	clra
    c33a:	b7 01 7f    	staa	0x17f
    c33d:	b7 01 7e    	staa	0x17e
    c340:	b7 01 1a    	staa	0x11a
    c343:	7e d3 af    	jmp	0xd3af
    c346:	7d 01 7e    	tst	0x17e
    c349:	27 0c       	beq	0x0xc357
    c34b:	bd ea c2    	jsr	0xeac2
    c34e:	7f 01 7e    	clr	0x17e
    c351:	7f 01 1a    	clr	0x11a
    c354:	7e d3 af    	jmp	0xd3af
    c357:	7d 01 1a    	tst	0x11a
    c35a:	26 03       	bne	0x0xc35f
    c35c:	7e d3 af    	jmp	0xd3af
    c35f:	ce e1 b0    	ldx	#0xe1b0
    c362:	b6 01 1e    	ldaa	0x11e
    c365:	81 03       	cmpa	#0x3
    c367:	26 34       	bne	0x0xc39d
    c369:	18 ce 01 20 	ldy	#0x120
    c36d:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc387
    c371:	d6 2d       	ldab	*0x2d
    c373:	bd c4 0d    	jsr	0xc40d
    c376:	d7 2d       	stab	*0x2d
    c378:	bd ea a1    	jsr	0xeaa1
    c37b:	96 2d       	ldaa	*0x2d
    c37d:	c6 2d       	ldab	#0x2d
    c37f:	bd af d2    	jsr	0xafd2
    c382:	86 04       	ldaa	#0x4
    c384:	7e c4 ba    	jmp	0xc4ba
    c387:	d6 3a       	ldab	*0x3a
    c389:	bd c4 0d    	jsr	0xc40d
    c38c:	d7 3a       	stab	*0x3a
    c38e:	bd ea a1    	jsr	0xeaa1
    c391:	96 3a       	ldaa	*0x3a
    c393:	c6 3a       	ldab	#0x3a
    c395:	bd af d2    	jsr	0xafd2
    c398:	86 04       	ldaa	#0x4
    c39a:	7e c4 ba    	jmp	0xc4ba
    c39d:	81 09       	cmpa	#0x9
    c39f:	26 34       	bne	0x0xc3d5
    c3a1:	18 ce 01 26 	ldy	#0x126
    c3a5:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc3bf
    c3a9:	d6 2e       	ldab	*0x2e
    c3ab:	bd c4 0d    	jsr	0xc40d
    c3ae:	d7 2e       	stab	*0x2e
    c3b0:	bd ea a1    	jsr	0xeaa1
    c3b3:	96 2e       	ldaa	*0x2e
    c3b5:	c6 2e       	ldab	#0x2e
    c3b7:	bd af d2    	jsr	0xafd2
    c3ba:	86 04       	ldaa	#0x4
    c3bc:	7e c4 ba    	jmp	0xc4ba
    c3bf:	d6 3b       	ldab	*0x3b
    c3c1:	bd c4 0d    	jsr	0xc40d
    c3c4:	d7 3b       	stab	*0x3b
    c3c6:	bd ea a1    	jsr	0xeaa1
    c3c9:	96 3b       	ldaa	*0x3b
    c3cb:	c6 3b       	ldab	#0x3b
    c3cd:	bd af d2    	jsr	0xafd2
    c3d0:	86 04       	ldaa	#0x4
    c3d2:	7e c4 ba    	jmp	0xc4ba
    c3d5:	81 0e       	cmpa	#0xe
    c3d7:	26 46       	bne	0x0xc41f
    c3d9:	18 ce 01 2b 	ldy	#0x12b
    c3dd:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc3f7
    c3e1:	d6 2f       	ldab	*0x2f
    c3e3:	bd c4 0d    	jsr	0xc40d
    c3e6:	d7 2f       	stab	*0x2f
    c3e8:	bd ea a1    	jsr	0xeaa1
    c3eb:	96 2f       	ldaa	*0x2f
    c3ed:	c6 2f       	ldab	#0x2f
    c3ef:	bd af d2    	jsr	0xafd2
    c3f2:	86 04       	ldaa	#0x4
    c3f4:	7e c4 ba    	jmp	0xc4ba
    c3f7:	d6 3c       	ldab	*0x3c
    c3f9:	bd c4 0d    	jsr	0xc40d
    c3fc:	d7 3c       	stab	*0x3c
    c3fe:	bd ea a1    	jsr	0xeaa1
    c401:	96 3c       	ldaa	*0x3c
    c403:	c6 3c       	ldab	#0x3c
    c405:	bd af d2    	jsr	0xafd2
    c408:	86 04       	ldaa	#0x4
    c40a:	7e c4 ba    	jmp	0xc4ba
    c40d:	7d 01 1a    	tst	0x11a
    c410:	2b 08       	bmi	0x0xc41a
    c412:	5c          	incb
    c413:	c1 11       	cmpb	#0x11
    c415:	25 02       	bcs	0x0xc419
    c417:	c6 10       	ldab	#0x10
    c419:	39          	rts
    c41a:	5a          	decb
    c41b:	2a fc       	bpl	0x0xc419
    c41d:	5f          	clrb
    c41e:	39          	rts
    c41f:	81 13       	cmpa	#0x13
    c421:	26 33       	bne	0x0xc456
    c423:	ce 01 31    	ldx	#0x131
    c426:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc440
    c42a:	96 2a       	ldaa	*0x2a
    c42c:	bd c4 c3    	jsr	0xc4c3
    c42f:	97 2a       	staa	*0x2a
    c431:	bd e9 ee    	jsr	0xe9ee
    c434:	96 2a       	ldaa	*0x2a
    c436:	c6 2a       	ldab	#0x2a
    c438:	bd af d2    	jsr	0xafd2
    c43b:	86 03       	ldaa	#0x3
    c43d:	7e c4 ba    	jmp	0xc4ba
    c440:	96 37       	ldaa	*0x37
    c442:	bd c4 c3    	jsr	0xc4c3
    c445:	97 37       	staa	*0x37
    c447:	bd e9 ee    	jsr	0xe9ee
    c44a:	96 37       	ldaa	*0x37
    c44c:	c6 37       	ldab	#0x37
    c44e:	bd af d2    	jsr	0xafd2
    c451:	86 03       	ldaa	#0x3
    c453:	7e c4 ba    	jmp	0xc4ba
    c456:	81 19       	cmpa	#0x19
    c458:	26 31       	bne	0x0xc48b
    c45a:	ce 01 37    	ldx	#0x137
    c45d:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc476
    c461:	96 2b       	ldaa	*0x2b
    c463:	bd c4 c3    	jsr	0xc4c3
    c466:	97 2b       	staa	*0x2b
    c468:	bd e9 ee    	jsr	0xe9ee
    c46b:	96 2b       	ldaa	*0x2b
    c46d:	c6 2b       	ldab	#0x2b
    c46f:	bd af d2    	jsr	0xafd2
    c472:	86 03       	ldaa	#0x3
    c474:	20 44       	bra	0x0xc4ba
    c476:	96 38       	ldaa	*0x38
    c478:	bd c4 c3    	jsr	0xc4c3
    c47b:	97 38       	staa	*0x38
    c47d:	bd e9 ee    	jsr	0xe9ee
    c480:	96 38       	ldaa	*0x38
    c482:	c6 38       	ldab	#0x38
    c484:	bd af d2    	jsr	0xafd2
    c487:	86 03       	ldaa	#0x3
    c489:	20 2f       	bra	0x0xc4ba
    c48b:	ce 01 3c    	ldx	#0x13c
    c48e:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc4a7
    c492:	96 2c       	ldaa	*0x2c
    c494:	bd c4 c3    	jsr	0xc4c3
    c497:	97 2c       	staa	*0x2c
    c499:	bd e9 ee    	jsr	0xe9ee
    c49c:	96 2c       	ldaa	*0x2c
    c49e:	c6 2c       	ldab	#0x2c
    c4a0:	bd af d2    	jsr	0xafd2
    c4a3:	86 03       	ldaa	#0x3
    c4a5:	20 13       	bra	0x0xc4ba
    c4a7:	96 39       	ldaa	*0x39
    c4a9:	bd c4 c3    	jsr	0xc4c3
    c4ac:	97 39       	staa	*0x39
    c4ae:	bd e9 ee    	jsr	0xe9ee
    c4b1:	96 39       	ldaa	*0x39
    c4b3:	c6 39       	ldab	#0x39
    c4b5:	bd af d2    	jsr	0xafd2
    c4b8:	86 03       	ldaa	#0x3
    c4ba:	b7 01 1c    	staa	0x11c
    c4bd:	7f 01 1a    	clr	0x11a
    c4c0:	7e d3 af    	jmp	0xd3af
    c4c3:	7d 01 1a    	tst	0x11a
    c4c6:	2b 04       	bmi	0x0xc4cc
    c4c8:	4c          	inca
    c4c9:	84 7f       	anda	#0x7f
    c4cb:	39          	rts
    c4cc:	4a          	deca
    c4cd:	84 7f       	anda	#0x7f
    c4cf:	20 fa       	bra	0x0xc4cb
    c4d1:	7d 01 1c    	tst	0x11c
    c4d4:	2a 03       	bpl	0x0xc4d9
    c4d6:	7e e1 f4    	jmp	0xe1f4
    c4d9:	7d 01 1e    	tst	0x11e
    c4dc:	26 08       	bne	0x0xc4e6
    c4de:	7d 01 1c    	tst	0x11c
    c4e1:	27 1f       	beq	0x0xc502
    c4e3:	7e b1 32    	jmp	0xb132
    c4e6:	7d 01 1c    	tst	0x11c
    c4e9:	27 24       	beq	0x0xc50f
    c4eb:	cc 01 20    	ldd	#0x120
    c4ee:	fb 01 1e    	addb	0x11e
    c4f1:	8f          	xgdx
    c4f2:	bd e8 da    	jsr	0xe8da
    c4f5:	7a 01 1e    	dec	0x11e
    c4f8:	7a 01 1c    	dec	0x11c
    c4fb:	26 12       	bne	0x0xc50f
    c4fd:	bd e9 58    	jsr	0xe958
    c500:	20 0d       	bra	0x0xc50f
    c502:	7d 01 1e    	tst	0x11e
    c505:	26 08       	bne	0x0xc50f
    c507:	86 13       	ldaa	#0x13
    c509:	b7 01 1e    	staa	0x11e
    c50c:	bd e9 16    	jsr	0xe916
    c50f:	7d 01 7f    	tst	0x17f
    c512:	27 10       	beq	0x0xc524
    c514:	bd eb 1a    	jsr	0xeb1a
    c517:	4f          	clra
    c518:	b7 01 7f    	staa	0x17f
    c51b:	b7 01 7e    	staa	0x17e
    c51e:	b7 01 1a    	staa	0x11a
    c521:	7e d3 af    	jmp	0xd3af
    c524:	7d 01 1a    	tst	0x11a
    c527:	26 03       	bne	0x0xc52c
    c529:	7e d3 af    	jmp	0xd3af
    c52c:	b6 01 1e    	ldaa	0x11e
    c52f:	81 13       	cmpa	#0x13
    c531:	26 53       	bne	0x0xc586
    c533:	ce e2 79    	ldx	#0xe279
    c536:	18 ce 01 30 	ldy	#0x130
    c53a:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc56b
    c53e:	d6 27       	ldab	*0x27
    c540:	8d 17       	bsr	0x0xc559
    c542:	d7 27       	stab	*0x27
    c544:	bd ea a1    	jsr	0xeaa1
    c547:	7f 01 1a    	clr	0x11a
    c54a:	86 04       	ldaa	#0x4
    c54c:	b7 01 1c    	staa	0x11c
    c54f:	96 27       	ldaa	*0x27
    c551:	c6 27       	ldab	#0x27
    c553:	bd af d2    	jsr	0xafd2
    c556:	7e d3 af    	jmp	0xd3af
    c559:	7d 01 1a    	tst	0x11a
    c55c:	2b 08       	bmi	0x0xc566
    c55e:	5c          	incb
    c55f:	c1 06       	cmpb	#0x6
    c561:	25 02       	bcs	0x0xc565
    c563:	c6 05       	ldab	#0x5
    c565:	39          	rts
    c566:	5a          	decb
    c567:	2a fc       	bpl	0x0xc565
    c569:	5f          	clrb
    c56a:	39          	rts
    c56b:	d6 34       	ldab	*0x34
    c56d:	8d ea       	bsr	0x0xc559
    c56f:	d7 34       	stab	*0x34
    c571:	bd ea a1    	jsr	0xeaa1
    c574:	7f 01 1a    	clr	0x11a
    c577:	86 04       	ldaa	#0x4
    c579:	b7 01 1c    	staa	0x11c
    c57c:	96 34       	ldaa	*0x34
    c57e:	c6 34       	ldab	#0x34
    c580:	bd af d2    	jsr	0xafd2
    c583:	7e d3 af    	jmp	0xd3af
    c586:	81 19       	cmpa	#0x19
    c588:	26 23       	bne	0x0xc5ad
    c58a:	ce e2 91    	ldx	#0xe291
    c58d:	18 ce 01 36 	ldy	#0x136
    c591:	d6 31       	ldab	*0x31
    c593:	5c          	incb
    c594:	c4 01       	andb	#0x1
    c596:	d7 31       	stab	*0x31
    c598:	bd ea a1    	jsr	0xeaa1
    c59b:	7f 01 1a    	clr	0x11a
    c59e:	86 04       	ldaa	#0x4
    c5a0:	b7 01 1c    	staa	0x11c
    c5a3:	96 31       	ldaa	*0x31
    c5a5:	c6 31       	ldab	#0x31
    c5a7:	bd af d2    	jsr	0xafd2
    c5aa:	7e d3 af    	jmp	0xd3af
    c5ad:	ce e2 99    	ldx	#0xe299
    c5b0:	18 ce 01 3b 	ldy	#0x13b
    c5b4:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc5e5
    c5b8:	d6 25       	ldab	*0x25
    c5ba:	8d 17       	bsr	0x0xc5d3
    c5bc:	d7 25       	stab	*0x25
    c5be:	bd ea a1    	jsr	0xeaa1
    c5c1:	7f 01 1a    	clr	0x11a
    c5c4:	86 04       	ldaa	#0x4
    c5c6:	b7 01 1c    	staa	0x11c
    c5c9:	96 25       	ldaa	*0x25
    c5cb:	c6 25       	ldab	#0x25
    c5cd:	bd af d2    	jsr	0xafd2
    c5d0:	7e d3 af    	jmp	0xd3af
    c5d3:	7d 01 1a    	tst	0x11a
    c5d6:	2b 08       	bmi	0x0xc5e0
    c5d8:	5c          	incb
    c5d9:	c1 0d       	cmpb	#0xd
    c5db:	25 02       	bcs	0x0xc5df
    c5dd:	c6 0c       	ldab	#0xc
    c5df:	39          	rts
    c5e0:	5a          	decb
    c5e1:	2a fc       	bpl	0x0xc5df
    c5e3:	5f          	clrb
    c5e4:	39          	rts
    c5e5:	d6 32       	ldab	*0x32
    c5e7:	8d ea       	bsr	0x0xc5d3
    c5e9:	d7 32       	stab	*0x32
    c5eb:	bd ea a1    	jsr	0xeaa1
    c5ee:	7f 01 1a    	clr	0x11a
    c5f1:	86 04       	ldaa	#0x4
    c5f3:	b7 01 1c    	staa	0x11c
    c5f6:	96 32       	ldaa	*0x32
    c5f8:	c6 32       	ldab	#0x32
    c5fa:	bd af d2    	jsr	0xafd2
    c5fd:	7e d3 af    	jmp	0xd3af
    c600:	7d 01 1c    	tst	0x11c
    c603:	2a 03       	bpl	0x0xc608
    c605:	7e e2 cd    	jmp	0xe2cd
    c608:	7d 01 1e    	tst	0x11e
    c60b:	26 08       	bne	0x0xc615
    c60d:	7d 01 1c    	tst	0x11c
    c610:	27 1f       	beq	0x0xc631
    c612:	7e b1 32    	jmp	0xb132
    c615:	7d 01 1c    	tst	0x11c
    c618:	27 24       	beq	0x0xc63e
    c61a:	cc 01 20    	ldd	#0x120
    c61d:	fb 01 1e    	addb	0x11e
    c620:	8f          	xgdx
    c621:	bd e8 da    	jsr	0xe8da
    c624:	7a 01 1e    	dec	0x11e
    c627:	7a 01 1c    	dec	0x11c
    c62a:	26 12       	bne	0x0xc63e
    c62c:	bd e9 58    	jsr	0xe958
    c62f:	20 0d       	bra	0x0xc63e
    c631:	7d 01 1e    	tst	0x11e
    c634:	26 08       	bne	0x0xc63e
    c636:	86 19       	ldaa	#0x19
    c638:	b7 01 1e    	staa	0x11e
    c63b:	bd e9 16    	jsr	0xe916
    c63e:	7d 01 7f    	tst	0x17f
    c641:	27 1e       	beq	0x0xc661
    c643:	2b 10       	bmi	0x0xc655
    c645:	bd eb 1a    	jsr	0xeb1a
    c648:	4f          	clra
    c649:	b7 01 7f    	staa	0x17f
    c64c:	b7 01 7e    	staa	0x17e
    c64f:	b7 01 1a    	staa	0x11a
    c652:	7e d3 af    	jmp	0xd3af
    c655:	b6 01 1e    	ldaa	0x11e
    c658:	81 19       	cmpa	#0x19
    c65a:	27 ec       	beq	0x0xc648
    c65c:	bd eb 1a    	jsr	0xeb1a
    c65f:	20 e7       	bra	0x0xc648
    c661:	7d 01 1a    	tst	0x11a
    c664:	26 03       	bne	0x0xc669
    c666:	7e d3 af    	jmp	0xd3af
    c669:	b6 01 1e    	ldaa	0x11e
    c66c:	81 13       	cmpa	#0x13
    c66e:	26 5a       	bne	0x0xc6ca
    c670:	12 f4 02 2b 	brset	*0xf4, #0x02, 0x0xc69f
    c674:	7d 01 1a    	tst	0x11a
    c677:	2b 1f       	bmi	0x0xc698
    c679:	96 29       	ldaa	*0x29
    c67b:	4c          	inca
    c67c:	84 7f       	anda	#0x7f
    c67e:	97 29       	staa	*0x29
    c680:	ce 01 31    	ldx	#0x131
    c683:	bd e9 ee    	jsr	0xe9ee
    c686:	7f 01 1a    	clr	0x11a
    c689:	86 03       	ldaa	#0x3
    c68b:	b7 01 1c    	staa	0x11c
    c68e:	96 29       	ldaa	*0x29
    c690:	c6 29       	ldab	#0x29
    c692:	bd af d2    	jsr	0xafd2
    c695:	7e d3 af    	jmp	0xd3af
    c698:	96 29       	ldaa	*0x29
    c69a:	4a          	deca
    c69b:	84 7f       	anda	#0x7f
    c69d:	20 df       	bra	0x0xc67e
    c69f:	7d 01 1a    	tst	0x11a
    c6a2:	2b 1f       	bmi	0x0xc6c3
    c6a4:	96 36       	ldaa	*0x36
    c6a6:	4c          	inca
    c6a7:	84 7f       	anda	#0x7f
    c6a9:	97 36       	staa	*0x36
    c6ab:	ce 01 31    	ldx	#0x131
    c6ae:	bd e9 ee    	jsr	0xe9ee
    c6b1:	7f 01 1a    	clr	0x11a
    c6b4:	86 03       	ldaa	#0x3
    c6b6:	b7 01 1c    	staa	0x11c
    c6b9:	96 36       	ldaa	*0x36
    c6bb:	c6 36       	ldab	#0x36
    c6bd:	bd af d2    	jsr	0xafd2
    c6c0:	7e d3 af    	jmp	0xd3af
    c6c3:	96 36       	ldaa	*0x36
    c6c5:	4a          	deca
    c6c6:	84 7f       	anda	#0x7f
    c6c8:	20 df       	bra	0x0xc6a9
    c6ca:	81 19       	cmpa	#0x19
    c6cc:	26 65       	bne	0x0xc733
    c6ce:	ce e3 33    	ldx	#0xe333
    c6d1:	18 ce 01 37 	ldy	#0x137
    c6d5:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc706
    c6d9:	7d 01 1a    	tst	0x11a
    c6dc:	2b 20       	bmi	0x0xc6fe
    c6de:	d6 26       	ldab	*0x26
    c6e0:	5c          	incb
    c6e1:	c1 03       	cmpb	#0x3
    c6e3:	25 02       	bcs	0x0xc6e7
    c6e5:	c6 02       	ldab	#0x2
    c6e7:	d7 26       	stab	*0x26
    c6e9:	bd ea 8d    	jsr	0xea8d
    c6ec:	7f 01 1a    	clr	0x11a
    c6ef:	86 03       	ldaa	#0x3
    c6f1:	b7 01 1c    	staa	0x11c
    c6f4:	96 26       	ldaa	*0x26
    c6f6:	c6 26       	ldab	#0x26
    c6f8:	bd af d2    	jsr	0xafd2
    c6fb:	7e d3 af    	jmp	0xd3af
    c6fe:	d6 26       	ldab	*0x26
    c700:	5a          	decb
    c701:	2a e4       	bpl	0x0xc6e7
    c703:	5f          	clrb
    c704:	20 e1       	bra	0x0xc6e7
    c706:	7d 01 1a    	tst	0x11a
    c709:	2b 20       	bmi	0x0xc72b
    c70b:	d6 33       	ldab	*0x33
    c70d:	5c          	incb
    c70e:	c1 03       	cmpb	#0x3
    c710:	25 02       	bcs	0x0xc714
    c712:	c6 02       	ldab	#0x2
    c714:	d7 33       	stab	*0x33
    c716:	bd ea 8d    	jsr	0xea8d
    c719:	7f 01 1a    	clr	0x11a
    c71c:	86 03       	ldaa	#0x3
    c71e:	b7 01 1c    	staa	0x11c
    c721:	96 33       	ldaa	*0x33
    c723:	c6 33       	ldab	#0x33
    c725:	bd af d2    	jsr	0xafd2
    c728:	7e d3 af    	jmp	0xd3af
    c72b:	d6 33       	ldab	*0x33
    c72d:	5a          	decb
    c72e:	2a e4       	bpl	0x0xc714
    c730:	5f          	clrb
    c731:	20 e1       	bra	0x0xc714
    c733:	ce e3 42    	ldx	#0xe342
    c736:	18 ce 01 3c 	ldy	#0x13c
    c73a:	7d 01 1a    	tst	0x11a
    c73d:	2b 20       	bmi	0x0xc75f
    c73f:	d6 96       	ldab	*0x96
    c741:	5c          	incb
    c742:	c1 04       	cmpb	#0x4
    c744:	25 02       	bcs	0x0xc748
    c746:	c6 03       	ldab	#0x3
    c748:	d7 96       	stab	*0x96
    c74a:	bd ea 8d    	jsr	0xea8d
    c74d:	7f 01 1a    	clr	0x11a
    c750:	86 03       	ldaa	#0x3
    c752:	b7 01 1c    	staa	0x11c
    c755:	96 96       	ldaa	*0x96
    c757:	c6 96       	ldab	#0x96
    c759:	bd af d2    	jsr	0xafd2
    c75c:	7e d3 af    	jmp	0xd3af
    c75f:	d6 96       	ldab	*0x96
    c761:	5a          	decb
    c762:	2a e4       	bpl	0x0xc748
    c764:	5f          	clrb
    c765:	20 e1       	bra	0x0xc748
    c767:	7e d3 af    	jmp	0xd3af
    c76a:	7d 01 1c    	tst	0x11c
    c76d:	2a 03       	bpl	0x0xc772
    c76f:	7e e3 4e    	jmp	0xe34e
    c772:	7d 01 1e    	tst	0x11e
    c775:	26 08       	bne	0x0xc77f
    c777:	7d 01 1c    	tst	0x11c
    c77a:	27 1f       	beq	0x0xc79b
    c77c:	7e b1 32    	jmp	0xb132
    c77f:	7d 01 1c    	tst	0x11c
    c782:	27 24       	beq	0x0xc7a8
    c784:	cc 01 20    	ldd	#0x120
    c787:	fb 01 1e    	addb	0x11e
    c78a:	8f          	xgdx
    c78b:	bd e8 da    	jsr	0xe8da
    c78e:	7a 01 1e    	dec	0x11e
    c791:	7a 01 1c    	dec	0x11c
    c794:	26 12       	bne	0x0xc7a8
    c796:	bd e9 8b    	jsr	0xe98b
    c799:	20 0d       	bra	0x0xc7a8
    c79b:	7d 01 1e    	tst	0x11e
    c79e:	26 08       	bne	0x0xc7a8
    c7a0:	86 02       	ldaa	#0x2
    c7a2:	b7 01 1e    	staa	0x11e
    c7a5:	bd e9 16    	jsr	0xe916
    c7a8:	7d 01 7f    	tst	0x17f
    c7ab:	27 10       	beq	0x0xc7bd
    c7ad:	bd eb 8c    	jsr	0xeb8c
    c7b0:	4f          	clra
    c7b1:	b7 01 7f    	staa	0x17f
    c7b4:	b7 01 7e    	staa	0x17e
    c7b7:	b7 01 1a    	staa	0x11a
    c7ba:	7e d3 af    	jmp	0xd3af
    c7bd:	7d 01 7e    	tst	0x17e
    c7c0:	27 0c       	beq	0x0xc7ce
    c7c2:	bd ea c2    	jsr	0xeac2
    c7c5:	7f 01 7e    	clr	0x17e
    c7c8:	7f 01 1a    	clr	0x11a
    c7cb:	7e d3 af    	jmp	0xd3af
    c7ce:	7d 01 1a    	tst	0x11a
    c7d1:	26 03       	bne	0x0xc7d6
    c7d3:	7e d3 af    	jmp	0xd3af
    c7d6:	c6 02       	ldab	#0x2
    c7d8:	18 ce 00 b0 	ldy	#0xb0
    c7dc:	96 f9       	ldaa	*0xf9
    c7de:	81 04       	cmpa	#0x4
    c7e0:	26 07       	bne	0x0xc7e9
    c7e2:	18 ce 50 30 	ldy	#0x5030
    c7e6:	14 fb 80    	bset	*0xfb, #0x80
    c7e9:	ce 01 20    	ldx	#0x120
    c7ec:	f1 01 1e    	cmpb	0x11e
    c7ef:	27 0a       	beq	0x0xc7fb
    c7f1:	18 08       	iny
    c7f3:	cb 04       	addb	#0x4
    c7f5:	08          	inx
    c7f6:	08          	inx
    c7f7:	08          	inx
    c7f8:	08          	inx
    c7f9:	20 f1       	bra	0x0xc7ec
    c7fb:	18 a6 00    	ldaa	0x0,y
    c7fe:	7d 01 1a    	tst	0x11a
    c801:	2b 23       	bmi	0x0xc826
    c803:	4c          	inca
    c804:	f6 01 11    	ldab	0x111
    c807:	c5 04       	bitb	#0x4
    c809:	27 02       	beq	0x0xc80d
    c80b:	8b 09       	adda	#0x9
    c80d:	4d          	tsta
    c80e:	2a 02       	bpl	0x0xc812
    c810:	86 7f       	ldaa	#0x7f
    c812:	18 a7 00    	staa	0x0,y
    c815:	bd ea 2a    	jsr	0xea2a
    c818:	bd c8 36    	jsr	0xc836
    c81b:	7f 01 1a    	clr	0x11a
    c81e:	86 03       	ldaa	#0x3
    c820:	b7 01 1c    	staa	0x11c
    c823:	7e d3 af    	jmp	0xd3af
    c826:	4a          	deca
    c827:	f6 01 11    	ldab	0x111
    c82a:	c5 04       	bitb	#0x4
    c82c:	27 02       	beq	0x0xc830
    c82e:	80 09       	suba	#0x9
    c830:	4d          	tsta
    c831:	2a df       	bpl	0x0xc812
    c833:	4f          	clra
    c834:	20 dc       	bra	0x0xc812
    c836:	18 a6 00    	ldaa	0x0,y
    c839:	36          	psha
    c83a:	18 8f       	xgdy
    c83c:	c1 3f       	cmpb	#0x3f
    c83e:	22 02       	bhi	0x0xc842
    c840:	cb 80       	addb	#0x80
    c842:	37          	pshb
    c843:	c4 0f       	andb	#0xf
    c845:	c1 08       	cmpb	#0x8
    c847:	25 02       	bcs	0x0xc84b
    c849:	c0 08       	subb	#0x8
    c84b:	5c          	incb
    c84c:	86 01       	ldaa	#0x1
    c84e:	5a          	decb
    c84f:	27 03       	beq	0x0xc854
    c851:	48          	asla
    c852:	20 fa       	bra	0x0xc84e
    c854:	43          	coma
    c855:	7d 00 d0    	tst	0xd0
    c858:	27 05       	beq	0x0xc85f
    c85a:	7f 00 d0    	clr	0xd0
    c85d:	20 08       	bra	0x0xc867
    c85f:	7d 10 29    	tst	0x1029
    c862:	2a fb       	bpl	0x0xc85f
    c864:	f6 10 2a    	ldab	0x102a
    c867:	01          	nop
    c868:	01          	nop
    c869:	01          	nop
    c86a:	01          	nop
    c86b:	b7 10 42    	staa	0x1042
    c86e:	01          	nop
    c86f:	01          	nop
    c870:	01          	nop
    c871:	01          	nop
    c872:	01          	nop
    c873:	01          	nop
    c874:	01          	nop
    c875:	86 83       	ldaa	#0x83
    c877:	b7 10 2a    	staa	0x102a
    c87a:	32          	pula
    c87b:	81 b8       	cmpa	#0xb8
    c87d:	25 04       	bcs	0x0xc883
    c87f:	86 8a       	ldaa	#0x8a
    c881:	20 0a       	bra	0x0xc88d
    c883:	81 b0       	cmpa	#0xb0
    c885:	25 04       	bcs	0x0xc88b
    c887:	86 89       	ldaa	#0x89
    c889:	20 02       	bra	0x0xc88d
    c88b:	86 88       	ldaa	#0x88
    c88d:	7d 10 29    	tst	0x1029
    c890:	2a fb       	bpl	0x0xc88d
    c892:	f6 10 2a    	ldab	0x102a
    c895:	b7 10 2a    	staa	0x102a
    c898:	32          	pula
    c899:	7d 10 29    	tst	0x1029
    c89c:	2a fb       	bpl	0x0xc899
    c89e:	f6 10 2a    	ldab	0x102a
    c8a1:	b7 10 2a    	staa	0x102a
    c8a4:	39          	rts
    c8a5:	7d 01 1c    	tst	0x11c
    c8a8:	2a 03       	bpl	0x0xc8ad
    c8aa:	7e e3 98    	jmp	0xe398
    c8ad:	7d 01 1e    	tst	0x11e
    c8b0:	26 08       	bne	0x0xc8ba
    c8b2:	7d 01 1c    	tst	0x11c
    c8b5:	27 1f       	beq	0x0xc8d6
    c8b7:	7e b1 32    	jmp	0xb132
    c8ba:	7d 01 1c    	tst	0x11c
    c8bd:	27 24       	beq	0x0xc8e3
    c8bf:	cc 01 20    	ldd	#0x120
    c8c2:	fb 01 1e    	addb	0x11e
    c8c5:	8f          	xgdx
    c8c6:	bd e8 da    	jsr	0xe8da
    c8c9:	7a 01 1e    	dec	0x11e
    c8cc:	7a 01 1c    	dec	0x11c
    c8cf:	26 12       	bne	0x0xc8e3
    c8d1:	bd e9 8b    	jsr	0xe98b
    c8d4:	20 0d       	bra	0x0xc8e3
    c8d6:	7d 01 1e    	tst	0x11e
    c8d9:	26 08       	bne	0x0xc8e3
    c8db:	86 02       	ldaa	#0x2
    c8dd:	b7 01 1e    	staa	0x11e
    c8e0:	bd e9 16    	jsr	0xe916
    c8e3:	7d 01 7f    	tst	0x17f
    c8e6:	27 10       	beq	0x0xc8f8
    c8e8:	bd eb 8c    	jsr	0xeb8c
    c8eb:	4f          	clra
    c8ec:	b7 01 7f    	staa	0x17f
    c8ef:	b7 01 7e    	staa	0x17e
    c8f2:	b7 01 1a    	staa	0x11a
    c8f5:	7e d3 af    	jmp	0xd3af
    c8f8:	7d 01 7e    	tst	0x17e
    c8fb:	27 0c       	beq	0x0xc909
    c8fd:	bd ea c2    	jsr	0xeac2
    c900:	7f 01 7e    	clr	0x17e
    c903:	7f 01 1a    	clr	0x11a
    c906:	7e d3 af    	jmp	0xd3af
    c909:	7d 01 1a    	tst	0x11a
    c90c:	26 03       	bne	0x0xc911
    c90e:	7e d3 af    	jmp	0xd3af
    c911:	c6 02       	ldab	#0x2
    c913:	18 ce 00 a8 	ldy	#0xa8
    c917:	96 f9       	ldaa	*0xf9
    c919:	81 04       	cmpa	#0x4
    c91b:	26 07       	bne	0x0xc924
    c91d:	14 fb 80    	bset	*0xfb, #0x80
    c920:	18 ce 50 28 	ldy	#0x5028
    c924:	ce 01 20    	ldx	#0x120
    c927:	f1 01 1e    	cmpb	0x11e
    c92a:	27 0a       	beq	0x0xc936
    c92c:	18 08       	iny
    c92e:	cb 04       	addb	#0x4
    c930:	08          	inx
    c931:	08          	inx
    c932:	08          	inx
    c933:	08          	inx
    c934:	20 f1       	bra	0x0xc927
    c936:	18 a6 00    	ldaa	0x0,y
    c939:	7d 01 1a    	tst	0x11a
    c93c:	2b 20       	bmi	0x0xc95e
    c93e:	4c          	inca
    c93f:	f6 01 11    	ldab	0x111
    c942:	c5 04       	bitb	#0x4
    c944:	27 02       	beq	0x0xc948
    c946:	8b 09       	adda	#0x9
    c948:	84 7f       	anda	#0x7f
    c94a:	18 a7 00    	staa	0x0,y
    c94d:	bd e9 ee    	jsr	0xe9ee
    c950:	bd c8 36    	jsr	0xc836
    c953:	7f 01 1a    	clr	0x11a
    c956:	86 03       	ldaa	#0x3
    c958:	b7 01 1c    	staa	0x11c
    c95b:	7e d3 af    	jmp	0xd3af
    c95e:	4a          	deca
    c95f:	f6 01 11    	ldab	0x111
    c962:	c5 04       	bitb	#0x4
    c964:	27 02       	beq	0x0xc968
    c966:	80 09       	suba	#0x9
    c968:	84 7f       	anda	#0x7f
    c96a:	20 de       	bra	0x0xc94a
    c96c:	7d 01 1c    	tst	0x11c
    c96f:	2a 03       	bpl	0x0xc974
    c971:	7e e3 d1    	jmp	0xe3d1
    c974:	7d 01 1e    	tst	0x11e
    c977:	26 08       	bne	0x0xc981
    c979:	7d 01 1c    	tst	0x11c
    c97c:	27 1f       	beq	0x0xc99d
    c97e:	7e b1 32    	jmp	0xb132
    c981:	7d 01 1c    	tst	0x11c
    c984:	27 24       	beq	0x0xc9aa
    c986:	cc 01 20    	ldd	#0x120
    c989:	fb 01 1e    	addb	0x11e
    c98c:	8f          	xgdx
    c98d:	bd e8 da    	jsr	0xe8da
    c990:	7a 01 1e    	dec	0x11e
    c993:	7a 01 1c    	dec	0x11c
    c996:	26 12       	bne	0x0xc9aa
    c998:	bd e9 8b    	jsr	0xe98b
    c99b:	20 0d       	bra	0x0xc9aa
    c99d:	7d 01 1e    	tst	0x11e
    c9a0:	26 08       	bne	0x0xc9aa
    c9a2:	86 02       	ldaa	#0x2
    c9a4:	b7 01 1e    	staa	0x11e
    c9a7:	bd e9 16    	jsr	0xe916
    c9aa:	7d 01 7f    	tst	0x17f
    c9ad:	27 10       	beq	0x0xc9bf
    c9af:	bd eb 8c    	jsr	0xeb8c
    c9b2:	4f          	clra
    c9b3:	b7 01 7f    	staa	0x17f
    c9b6:	b7 01 7e    	staa	0x17e
    c9b9:	b7 01 1a    	staa	0x11a
    c9bc:	7e d3 af    	jmp	0xd3af
    c9bf:	7d 01 7e    	tst	0x17e
    c9c2:	27 0c       	beq	0x0xc9d0
    c9c4:	bd ea c2    	jsr	0xeac2
    c9c7:	7f 01 7e    	clr	0x17e
    c9ca:	7f 01 1a    	clr	0x11a
    c9cd:	7e d3 af    	jmp	0xd3af
    c9d0:	7d 01 1a    	tst	0x11a
    c9d3:	26 03       	bne	0x0xc9d8
    c9d5:	7e d3 af    	jmp	0xd3af
    c9d8:	c6 02       	ldab	#0x2
    c9da:	18 ce 00 b8 	ldy	#0xb8
    c9de:	96 f9       	ldaa	*0xf9
    c9e0:	81 04       	cmpa	#0x4
    c9e2:	26 07       	bne	0x0xc9eb
    c9e4:	14 fb 80    	bset	*0xfb, #0x80
    c9e7:	18 ce 50 38 	ldy	#0x5038
    c9eb:	ce 01 20    	ldx	#0x120
    c9ee:	f1 01 1e    	cmpb	0x11e
    c9f1:	27 0a       	beq	0x0xc9fd
    c9f3:	18 08       	iny
    c9f5:	cb 04       	addb	#0x4
    c9f7:	08          	inx
    c9f8:	08          	inx
    c9f9:	08          	inx
    c9fa:	08          	inx
    c9fb:	20 f1       	bra	0x0xc9ee
    c9fd:	18 a6 00    	ldaa	0x0,y
    ca00:	7d 01 1a    	tst	0x11a
    ca03:	2b 20       	bmi	0x0xca25
    ca05:	4c          	inca
    ca06:	f6 01 11    	ldab	0x111
    ca09:	c5 04       	bitb	#0x4
    ca0b:	27 02       	beq	0x0xca0f
    ca0d:	8b 09       	adda	#0x9
    ca0f:	84 7f       	anda	#0x7f
    ca11:	18 a7 00    	staa	0x0,y
    ca14:	bd e9 ee    	jsr	0xe9ee
    ca17:	bd c8 36    	jsr	0xc836
    ca1a:	7f 01 1a    	clr	0x11a
    ca1d:	86 03       	ldaa	#0x3
    ca1f:	b7 01 1c    	staa	0x11c
    ca22:	7e d3 af    	jmp	0xd3af
    ca25:	4a          	deca
    ca26:	f6 01 11    	ldab	0x111
    ca29:	c5 04       	bitb	#0x4
    ca2b:	27 02       	beq	0x0xca2f
    ca2d:	80 09       	suba	#0x9
    ca2f:	84 7f       	anda	#0x7f
    ca31:	20 de       	bra	0x0xca11
    ca33:	7d 01 1c    	tst	0x11c
    ca36:	2a 03       	bpl	0x0xca3b
    ca38:	7e e4 0a    	jmp	0xe40a
    ca3b:	7d 01 1e    	tst	0x11e
    ca3e:	26 08       	bne	0x0xca48
    ca40:	7d 01 1c    	tst	0x11c
    ca43:	27 1f       	beq	0x0xca64
    ca45:	7e b1 32    	jmp	0xb132
    ca48:	7d 01 1c    	tst	0x11c
    ca4b:	27 27       	beq	0x0xca74
    ca4d:	cc 01 20    	ldd	#0x120
    ca50:	fb 01 1e    	addb	0x11e
    ca53:	8f          	xgdx
    ca54:	bd e8 da    	jsr	0xe8da
    ca57:	7a 01 1e    	dec	0x11e
    ca5a:	7a 01 1c    	dec	0x11c
    ca5d:	26 15       	bne	0x0xca74
    ca5f:	bd e9 58    	jsr	0xe958
    ca62:	20 10       	bra	0x0xca74
    ca64:	7d 01 1e    	tst	0x11e
    ca67:	26 0b       	bne	0x0xca74
    ca69:	86 13       	ldaa	#0x13
    ca6b:	b7 01 1e    	staa	0x11e
    ca6e:	bd e9 16    	jsr	0xe916
    ca71:	7f 01 6b    	clr	0x16b
    ca74:	7d 01 7f    	tst	0x17f
    ca77:	27 10       	beq	0x0xca89
    ca79:	bd eb 1a    	jsr	0xeb1a
    ca7c:	4f          	clra
    ca7d:	b7 01 7f    	staa	0x17f
    ca80:	b7 01 7e    	staa	0x17e
    ca83:	b7 01 1a    	staa	0x11a
    ca86:	7e d3 af    	jmp	0xd3af
    ca89:	7d 01 1a    	tst	0x11a
    ca8c:	26 03       	bne	0x0xca91
    ca8e:	7e d3 af    	jmp	0xd3af
    ca91:	b6 01 1e    	ldaa	0x11e
    ca94:	81 13       	cmpa	#0x13
    ca96:	26 23       	bne	0x0xcabb
    ca98:	ce e2 79    	ldx	#0xe279
    ca9b:	18 ce 01 30 	ldy	#0x130
    ca9f:	d6 a7       	ldab	*0xa7
    caa1:	bd c5 59    	jsr	0xc559
    caa4:	d7 a7       	stab	*0xa7
    caa6:	bd ea a1    	jsr	0xeaa1
    caa9:	7f 01 1a    	clr	0x11a
    caac:	86 04       	ldaa	#0x4
    caae:	b7 01 1c    	staa	0x11c
    cab1:	96 a7       	ldaa	*0xa7
    cab3:	c6 a7       	ldab	#0xa7
    cab5:	bd af d2    	jsr	0xafd2
    cab8:	7e d3 af    	jmp	0xd3af
    cabb:	81 19       	cmpa	#0x19
    cabd:	26 32       	bne	0x0xcaf1
    cabf:	ce e3 33    	ldx	#0xe333
    cac2:	18 ce 01 37 	ldy	#0x137
    cac6:	d6 a6       	ldab	*0xa6
    cac8:	7d 01 1a    	tst	0x11a
    cacb:	2b 1e       	bmi	0x0xcaeb
    cacd:	5c          	incb
    cace:	c1 03       	cmpb	#0x3
    cad0:	25 02       	bcs	0x0xcad4
    cad2:	c6 02       	ldab	#0x2
    cad4:	d7 a6       	stab	*0xa6
    cad6:	bd ea 8d    	jsr	0xea8d
    cad9:	7f 01 1a    	clr	0x11a
    cadc:	86 03       	ldaa	#0x3
    cade:	b7 01 1c    	staa	0x11c
    cae1:	96 a6       	ldaa	*0xa6
    cae3:	c6 a6       	ldab	#0xa6
    cae5:	bd af d2    	jsr	0xafd2
    cae8:	7e d3 af    	jmp	0xd3af
    caeb:	5a          	decb
    caec:	2a e6       	bpl	0x0xcad4
    caee:	5f          	clrb
    caef:	20 e3       	bra	0x0xcad4
    caf1:	ce e2 99    	ldx	#0xe299
    caf4:	18 ce 01 3b 	ldy	#0x13b
    caf8:	d6 a5       	ldab	*0xa5
    cafa:	bd c5 d3    	jsr	0xc5d3
    cafd:	d7 a5       	stab	*0xa5
    caff:	bd ea a1    	jsr	0xeaa1
    cb02:	7f 01 1a    	clr	0x11a
    cb05:	86 04       	ldaa	#0x4
    cb07:	b7 01 1c    	staa	0x11c
    cb0a:	96 a5       	ldaa	*0xa5
    cb0c:	c6 a5       	ldab	#0xa5
    cb0e:	bd af d2    	jsr	0xafd2
    cb11:	7e d3 af    	jmp	0xd3af
    cb14:	7f 01 7f    	clr	0x17f
    cb17:	7f 01 7e    	clr	0x17e
    cb1a:	7f 01 1a    	clr	0x11a
    cb1d:	7e d3 af    	jmp	0xd3af
    cb20:	7d 01 1c    	tst	0x11c
    cb23:	2a 03       	bpl	0x0xcb28
    cb25:	7e e4 68    	jmp	0xe468
    cb28:	7d 01 1e    	tst	0x11e
    cb2b:	26 08       	bne	0x0xcb35
    cb2d:	7d 01 1c    	tst	0x11c
    cb30:	27 1f       	beq	0x0xcb51
    cb32:	7e b1 32    	jmp	0xb132
    cb35:	7d 01 1c    	tst	0x11c
    cb38:	27 24       	beq	0x0xcb5e
    cb3a:	cc 01 20    	ldd	#0x120
    cb3d:	fb 01 1e    	addb	0x11e
    cb40:	8f          	xgdx
    cb41:	bd e8 da    	jsr	0xe8da
    cb44:	7a 01 1e    	dec	0x11e
    cb47:	7a 01 1c    	dec	0x11c
    cb4a:	26 12       	bne	0x0xcb5e
    cb4c:	bd e9 8b    	jsr	0xe98b
    cb4f:	20 0d       	bra	0x0xcb5e
    cb51:	7d 01 1e    	tst	0x11e
    cb54:	26 08       	bne	0x0xcb5e
    cb56:	86 12       	ldaa	#0x12
    cb58:	b7 01 1e    	staa	0x11e
    cb5b:	bd e9 16    	jsr	0xe916
    cb5e:	7d 01 7f    	tst	0x17f
    cb61:	27 10       	beq	0x0xcb73
    cb63:	bd eb 8c    	jsr	0xeb8c
    cb66:	4f          	clra
    cb67:	b7 01 7f    	staa	0x17f
    cb6a:	b7 01 7e    	staa	0x17e
    cb6d:	b7 01 1a    	staa	0x11a
    cb70:	7e d3 af    	jmp	0xd3af
    cb73:	7d 01 1a    	tst	0x11a
    cb76:	26 03       	bne	0x0xcb7b
    cb78:	7e d3 af    	jmp	0xd3af
    cb7b:	b6 01 1e    	ldaa	0x11e
    cb7e:	81 12       	cmpa	#0x12
    cb80:	26 16       	bne	0x0xcb98
    cb82:	d6 21       	ldab	*0x21
    cb84:	c8 02       	eorb	#0x2
    cb86:	d7 21       	stab	*0x21
    cb88:	c4 02       	andb	#0x2
    cb8a:	54          	lsrb
    cb8b:	18 ce 01 30 	ldy	#0x130
    cb8f:	ce d9 6c    	ldx	#0xd96c
    cb92:	bd ea 8d    	jsr	0xea8d
    cb95:	7e b7 e5    	jmp	0xb7e5
    cb98:	81 16       	cmpa	#0x16
    cb9a:	26 17       	bne	0x0xcbb3
    cb9c:	d6 21       	ldab	*0x21
    cb9e:	c8 04       	eorb	#0x4
    cba0:	d7 21       	stab	*0x21
    cba2:	c4 04       	andb	#0x4
    cba4:	54          	lsrb
    cba5:	54          	lsrb
    cba6:	18 ce 01 34 	ldy	#0x134
    cbaa:	ce d9 6c    	ldx	#0xd96c
    cbad:	bd ea 8d    	jsr	0xea8d
    cbb0:	7e b7 e5    	jmp	0xb7e5
    cbb3:	81 1a       	cmpa	#0x1a
    cbb5:	26 30       	bne	0x0xcbe7
    cbb7:	b6 01 1a    	ldaa	0x11a
    cbba:	2b 23       	bmi	0x0xcbdf
    cbbc:	96 22       	ldaa	*0x22
    cbbe:	4c          	inca
    cbbf:	81 7f       	cmpa	#0x7f
    cbc1:	25 02       	bcs	0x0xcbc5
    cbc3:	86 7f       	ldaa	#0x7f
    cbc5:	97 22       	staa	*0x22
    cbc7:	ce 01 38    	ldx	#0x138
    cbca:	bd ea 2a    	jsr	0xea2a
    cbcd:	7f 01 1a    	clr	0x11a
    cbd0:	86 03       	ldaa	#0x3
    cbd2:	b7 01 1c    	staa	0x11c
    cbd5:	96 22       	ldaa	*0x22
    cbd7:	c6 22       	ldab	#0x22
    cbd9:	bd af d2    	jsr	0xafd2
    cbdc:	7e d3 af    	jmp	0xd3af
    cbdf:	96 22       	ldaa	*0x22
    cbe1:	4a          	deca
    cbe2:	2a e1       	bpl	0x0xcbc5
    cbe4:	4f          	clra
    cbe5:	20 de       	bra	0x0xcbc5
    cbe7:	b6 01 1a    	ldaa	0x11a
    cbea:	2b 1f       	bmi	0x0xcc0b
    cbec:	96 23       	ldaa	*0x23
    cbee:	4c          	inca
    cbef:	84 7f       	anda	#0x7f
    cbf1:	97 23       	staa	*0x23
    cbf3:	ce 01 3c    	ldx	#0x13c
    cbf6:	bd e9 ee    	jsr	0xe9ee
    cbf9:	7f 01 1a    	clr	0x11a
    cbfc:	86 03       	ldaa	#0x3
    cbfe:	b7 01 1c    	staa	0x11c
    cc01:	96 23       	ldaa	*0x23
    cc03:	c6 23       	ldab	#0x23
    cc05:	bd af d2    	jsr	0xafd2
    cc08:	7e d3 af    	jmp	0xd3af
    cc0b:	96 23       	ldaa	*0x23
    cc0d:	4a          	deca
    cc0e:	84 7f       	anda	#0x7f
    cc10:	20 df       	bra	0x0xcbf1
    cc12:	7d 01 1c    	tst	0x11c
    cc15:	2a 03       	bpl	0x0xcc1a
    cc17:	7e e5 9a    	jmp	0xe59a
    cc1a:	7d 01 1e    	tst	0x11e
    cc1d:	26 08       	bne	0x0xcc27
    cc1f:	7d 01 1c    	tst	0x11c
    cc22:	27 1f       	beq	0x0xcc43
    cc24:	7e b1 32    	jmp	0xb132
    cc27:	7d 01 1c    	tst	0x11c
    cc2a:	27 24       	beq	0x0xcc50
    cc2c:	cc 01 20    	ldd	#0x120
    cc2f:	fb 01 1e    	addb	0x11e
    cc32:	8f          	xgdx
    cc33:	bd e8 da    	jsr	0xe8da
    cc36:	7a 01 1e    	dec	0x11e
    cc39:	7a 01 1c    	dec	0x11c
    cc3c:	26 12       	bne	0x0xcc50
    cc3e:	bd e9 58    	jsr	0xe958
    cc41:	20 0d       	bra	0x0xcc50
    cc43:	7d 01 1e    	tst	0x11e
    cc46:	26 08       	bne	0x0xcc50
    cc48:	86 13       	ldaa	#0x13
    cc4a:	b7 01 1e    	staa	0x11e
    cc4d:	bd e9 16    	jsr	0xe916
    cc50:	7d 01 7f    	tst	0x17f
    cc53:	27 0d       	beq	0x0xcc62
    cc55:	4f          	clra
    cc56:	b7 01 7f    	staa	0x17f
    cc59:	b7 01 7e    	staa	0x17e
    cc5c:	b7 01 1a    	staa	0x11a
    cc5f:	7e d3 af    	jmp	0xd3af
    cc62:	7d 01 1a    	tst	0x11a
    cc65:	26 03       	bne	0x0xcc6a
    cc67:	7e d3 af    	jmp	0xd3af
    cc6a:	7d 01 1a    	tst	0x11a
    cc6d:	2b 26       	bmi	0x0xcc95
    cc6f:	96 f0       	ldaa	*0xf0
    cc71:	84 07       	anda	#0x7
    cc73:	4c          	inca
    cc74:	84 07       	anda	#0x7
    cc76:	97 f0       	staa	*0xf0
    cc78:	bd ab 4b    	jsr	0xab4b
    cc7b:	96 f0       	ldaa	*0xf0
    cc7d:	b7 7f fa    	staa	0x7ffa
    cc80:	4c          	inca
    cc81:	ce 01 31    	ldx	#0x131
    cc84:	bd e9 ee    	jsr	0xe9ee
    cc87:	7f 01 1a    	clr	0x11a
    cc8a:	86 03       	ldaa	#0x3
    cc8c:	b7 01 1c    	staa	0x11c
    cc8f:	bd ab 71    	jsr	0xab71
    cc92:	7e d3 af    	jmp	0xd3af
    cc95:	96 f0       	ldaa	*0xf0
    cc97:	84 07       	anda	#0x7
    cc99:	4a          	deca
    cc9a:	20 d8       	bra	0x0xcc74
    cc9c:	7d 01 1c    	tst	0x11c
    cc9f:	2a 03       	bpl	0x0xcca4
    cca1:	7e e5 ec    	jmp	0xe5ec
    cca4:	7d 01 1e    	tst	0x11e
    cca7:	26 08       	bne	0x0xccb1
    cca9:	7d 01 1c    	tst	0x11c
    ccac:	27 1f       	beq	0x0xcccd
    ccae:	7e b1 32    	jmp	0xb132
    ccb1:	7d 01 1c    	tst	0x11c
    ccb4:	27 24       	beq	0x0xccda
    ccb6:	cc 01 20    	ldd	#0x120
    ccb9:	fb 01 1e    	addb	0x11e
    ccbc:	8f          	xgdx
    ccbd:	bd e8 da    	jsr	0xe8da
    ccc0:	7a 01 1e    	dec	0x11e
    ccc3:	7a 01 1c    	dec	0x11c
    ccc6:	26 12       	bne	0x0xccda
    ccc8:	bd e9 58    	jsr	0xe958
    cccb:	20 0d       	bra	0x0xccda
    cccd:	7d 01 1e    	tst	0x11e
    ccd0:	26 08       	bne	0x0xccda
    ccd2:	86 13       	ldaa	#0x13
    ccd4:	b7 01 1e    	staa	0x11e
    ccd7:	bd e9 16    	jsr	0xe916
    ccda:	7d 01 7f    	tst	0x17f
    ccdd:	27 0d       	beq	0x0xccec
    ccdf:	4f          	clra
    cce0:	b7 01 7f    	staa	0x17f
    cce3:	b7 01 7e    	staa	0x17e
    cce6:	b7 01 1a    	staa	0x11a
    cce9:	7e d3 af    	jmp	0xd3af
    ccec:	7d 01 1a    	tst	0x11a
    ccef:	26 03       	bne	0x0xccf4
    ccf1:	7e d3 af    	jmp	0xd3af
    ccf4:	7d 01 1a    	tst	0x11a
    ccf7:	2b 29       	bmi	0x0xcd22
    ccf9:	96 de       	ldaa	*0xde
    ccfb:	4c          	inca
    ccfc:	81 02       	cmpa	#0x2
    ccfe:	23 02       	bls	0x0xcd02
    cd00:	86 02       	ldaa	#0x2
    cd02:	97 de       	staa	*0xde
    cd04:	bd ab 4b    	jsr	0xab4b
    cd07:	96 de       	ldaa	*0xde
    cd09:	b7 7f f9    	staa	0x7ff9
    cd0c:	4c          	inca
    cd0d:	4c          	inca
    cd0e:	ce 01 31    	ldx	#0x131
    cd11:	bd e9 ee    	jsr	0xe9ee
    cd14:	7f 01 1a    	clr	0x11a
    cd17:	86 03       	ldaa	#0x3
    cd19:	b7 01 1c    	staa	0x11c
    cd1c:	bd ab 71    	jsr	0xab71
    cd1f:	7e d3 af    	jmp	0xd3af
    cd22:	96 de       	ldaa	*0xde
    cd24:	4a          	deca
    cd25:	2a db       	bpl	0x0xcd02
    cd27:	4f          	clra
    cd28:	20 d8       	bra	0x0xcd02
    cd2a:	7d 01 1c    	tst	0x11c
    cd2d:	2a 03       	bpl	0x0xcd32
    cd2f:	7e e7 e6    	jmp	0xe7e6
    cd32:	7d 01 1e    	tst	0x11e
    cd35:	26 08       	bne	0x0xcd3f
    cd37:	7d 01 1c    	tst	0x11c
    cd3a:	27 1a       	beq	0x0xcd56
    cd3c:	7e b1 32    	jmp	0xb132
    cd3f:	7d 01 1c    	tst	0x11c
    cd42:	27 1a       	beq	0x0xcd5e
    cd44:	cc 01 20    	ldd	#0x120
    cd47:	fb 01 1e    	addb	0x11e
    cd4a:	8f          	xgdx
    cd4b:	bd e8 da    	jsr	0xe8da
    cd4e:	7a 01 1c    	dec	0x11c
    cd51:	bd e9 16    	jsr	0xe916
    cd54:	20 08       	bra	0x0xcd5e
    cd56:	86 0f       	ldaa	#0xf
    cd58:	b7 01 1e    	staa	0x11e
    cd5b:	bd e9 16    	jsr	0xe916
    cd5e:	7d 01 7f    	tst	0x17f
    cd61:	27 0d       	beq	0x0xcd70
    cd63:	4f          	clra
    cd64:	b7 01 7f    	staa	0x17f
    cd67:	b7 01 7e    	staa	0x17e
    cd6a:	b7 01 1a    	staa	0x11a
    cd6d:	7e d3 af    	jmp	0xd3af
    cd70:	7d 01 7e    	tst	0x17e
    cd73:	27 29       	beq	0x0xcd9e
    cd75:	b6 01 1e    	ldaa	0x11e
    cd78:	81 0f       	cmpa	#0xf
    cd7a:	26 11       	bne	0x0xcd8d
    cd7c:	86 1f       	ldaa	#0x1f
    cd7e:	b7 01 1e    	staa	0x11e
    cd81:	bd e9 16    	jsr	0xe916
    cd84:	7f 01 7e    	clr	0x17e
    cd87:	7f 01 1a    	clr	0x11a
    cd8a:	7e d3 af    	jmp	0xd3af
    cd8d:	86 0f       	ldaa	#0xf
    cd8f:	b7 01 1e    	staa	0x11e
    cd92:	bd e9 16    	jsr	0xe916
    cd95:	7f 01 7e    	clr	0x17e
    cd98:	7f 01 1a    	clr	0x11a
    cd9b:	7e d3 af    	jmp	0xd3af
    cd9e:	7d 01 1a    	tst	0x11a
    cda1:	26 03       	bne	0x0xcda6
    cda3:	7e d3 af    	jmp	0xd3af
    cda6:	b6 01 1e    	ldaa	0x11e
    cda9:	81 0f       	cmpa	#0xf
    cdab:	26 13       	bne	0x0xcdc0
    cdad:	b6 01 2f    	ldaa	0x12f
    cdb0:	81 43       	cmpa	#0x43
    cdb2:	26 06       	bne	0x0xcdba
    cdb4:	4c          	inca
    cdb5:	b7 01 2f    	staa	0x12f
    cdb8:	20 17       	bra	0x0xcdd1
    cdba:	4a          	deca
    cdbb:	b7 01 2f    	staa	0x12f
    cdbe:	20 11       	bra	0x0xcdd1
    cdc0:	b6 01 3f    	ldaa	0x13f
    cdc3:	81 41       	cmpa	#0x41
    cdc5:	26 06       	bne	0x0xcdcd
    cdc7:	4c          	inca
    cdc8:	b7 01 3f    	staa	0x13f
    cdcb:	20 04       	bra	0x0xcdd1
    cdcd:	4a          	deca
    cdce:	b7 01 3f    	staa	0x13f
    cdd1:	7f 01 1a    	clr	0x11a
    cdd4:	86 01       	ldaa	#0x1
    cdd6:	b7 01 1c    	staa	0x11c
    cdd9:	7e d3 af    	jmp	0xd3af
    cddc:	7d 01 1c    	tst	0x11c
    cddf:	2a 03       	bpl	0x0xcde4
    cde1:	7e e4 de    	jmp	0xe4de
    cde4:	7d 01 1e    	tst	0x11e
    cde7:	26 08       	bne	0x0xcdf1
    cde9:	7d 01 1c    	tst	0x11c
    cdec:	27 1f       	beq	0x0xce0d
    cdee:	7e b1 32    	jmp	0xb132
    cdf1:	7d 01 1c    	tst	0x11c
    cdf4:	27 24       	beq	0x0xce1a
    cdf6:	cc 01 20    	ldd	#0x120
    cdf9:	fb 01 1e    	addb	0x11e
    cdfc:	8f          	xgdx
    cdfd:	bd e8 da    	jsr	0xe8da
    ce00:	7a 01 1e    	dec	0x11e
    ce03:	7a 01 1c    	dec	0x11c
    ce06:	26 12       	bne	0x0xce1a
    ce08:	bd e9 8b    	jsr	0xe98b
    ce0b:	20 0d       	bra	0x0xce1a
    ce0d:	7d 01 1e    	tst	0x11e
    ce10:	26 08       	bne	0x0xce1a
    ce12:	86 13       	ldaa	#0x13
    ce14:	b7 01 1e    	staa	0x11e
    ce17:	bd e9 16    	jsr	0xe916
    ce1a:	7d 01 1a    	tst	0x11a
    ce1d:	26 09       	bne	0x0xce28
    ce1f:	7f 01 7f    	clr	0x17f
    ce22:	7f 01 7e    	clr	0x17e
    ce25:	7e d3 af    	jmp	0xd3af
    ce28:	7f 01 1a    	clr	0x11a
    ce2b:	7f 01 7f    	clr	0x17f
    ce2e:	7f 01 7e    	clr	0x17e
    ce31:	7d 00 d0    	tst	0xd0
    ce34:	27 05       	beq	0x0xce3b
    ce36:	7f 00 d0    	clr	0xd0
    ce39:	20 08       	bra	0x0xce43
    ce3b:	7d 10 29    	tst	0x1029
    ce3e:	2a fb       	bpl	0x0xce3b
    ce40:	b6 10 2a    	ldaa	0x102a
    ce43:	5f          	clrb
    ce44:	01          	nop
    ce45:	01          	nop
    ce46:	01          	nop
    ce47:	01          	nop
    ce48:	f7 10 42    	stab	0x1042
    ce4b:	01          	nop
    ce4c:	01          	nop
    ce4d:	01          	nop
    ce4e:	01          	nop
    ce4f:	01          	nop
    ce50:	01          	nop
    ce51:	01          	nop
    ce52:	86 fe       	ldaa	#0xfe
    ce54:	b7 10 2a    	staa	0x102a
    ce57:	bd e9 3b    	jsr	0xe93b
    ce5a:	c6 20       	ldab	#0x20
    ce5c:	ce 01 20    	ldx	#0x120
    ce5f:	18 ce ce bf 	ldy	#0xcebf
    ce63:	18 a6 00    	ldaa	0x0,y
    ce66:	a7 00       	staa	0x0,x
    ce68:	08          	inx
    ce69:	18 08       	iny
    ce6b:	5a          	decb
    ce6c:	26 f5       	bne	0x0xce63
    ce6e:	7f 01 1e    	clr	0x11e
    ce71:	86 20       	ldaa	#0x20
    ce73:	b7 01 1c    	staa	0x11c
    ce76:	ce 10 23    	ldx	#0x1023
    ce79:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xce79
    ce7d:	bd e8 8f    	jsr	0xe88f
    ce80:	7d 01 1c    	tst	0x11c
    ce83:	27 05       	beq	0x0xce8a
    ce85:	ce 10 23    	ldx	#0x1023
    ce88:	20 ef       	bra	0x0xce79
    ce8a:	86 ff       	ldaa	#0xff
    ce8c:	97 f8       	staa	*0xf8
    ce8e:	c6 06       	ldab	#0x6
    ce90:	ce ff ff    	ldx	#0xffff
    ce93:	01          	nop
    ce94:	01          	nop
    ce95:	01          	nop
    ce96:	01          	nop
    ce97:	09          	dex
    ce98:	26 f9       	bne	0x0xce93
    ce9a:	5a          	decb
    ce9b:	26 f6       	bne	0x0xce93
    ce9d:	c6 06       	ldab	#0x6
    ce9f:	44          	lsra
    cea0:	27 04       	beq	0x0xcea6
    cea2:	97 f8       	staa	*0xf8
    cea4:	20 ed       	bra	0x0xce93
    cea6:	97 f8       	staa	*0xf8
    cea8:	97 fb       	staa	*0xfb
    ceaa:	97 f2       	staa	*0xf2
    ceac:	97 ff       	staa	*0xff
    ceae:	b7 01 1b    	staa	0x11b
    ceb1:	bd a0 ff    	jsr	0xa0ff
    ceb4:	bd a1 16    	jsr	0xa116
    ceb7:	86 80       	ldaa	#0x80
    ceb9:	b7 01 1c    	staa	0x11c
    cebc:	7e 82 93    	jmp	0x8293
    cebf:	20 54       	bra	0x0xcf15
    cec1:	55          	.byte	0x55
    cec2:	4e          	.byte	0x4e
    cec3:	49          	rola
    cec4:	4e          	.byte	0x4e
    cec5:	47          	asra
    cec6:	20 2e       	bra	0x0xcef6
    cec8:	2e 2e       	bgt	0x0xcef8
    ceca:	2e 2e       	bgt	0x0xcefa
    cecc:	20 20       	bra	0x0xceee
    cece:	20 20       	bra	0x0xcef0
    ced0:	20 20       	bra	0x0xcef2
    ced2:	20 20       	bra	0x0xcef4
    ced4:	20 20       	bra	0x0xcef6
    ced6:	20 20       	bra	0x0xcef8
    ced8:	20 20       	bra	0x0xcefa
    ceda:	20 20       	bra	0x0xcefc
    cedc:	20 20       	bra	0x0xcefe
    cede:	20 7e       	bra	0x0xcf5e
    cee0:	d3 af       	addd	*0xaf
    cee2:	7d 01 1c    	tst	0x11c
    cee5:	2a 03       	bpl	0x0xceea
    cee7:	7e df dc    	jmp	0xdfdc
    ceea:	7d 01 1e    	tst	0x11e
    ceed:	26 08       	bne	0x0xcef7
    ceef:	7d 01 1c    	tst	0x11c
    cef2:	27 1f       	beq	0x0xcf13
    cef4:	7e b1 32    	jmp	0xb132
    cef7:	7d 01 1c    	tst	0x11c
    cefa:	27 24       	beq	0x0xcf20
    cefc:	cc 01 20    	ldd	#0x120
    ceff:	fb 01 1e    	addb	0x11e
    cf02:	8f          	xgdx
    cf03:	bd e8 da    	jsr	0xe8da
    cf06:	7a 01 1e    	dec	0x11e
    cf09:	7a 01 1c    	dec	0x11c
    cf0c:	26 12       	bne	0x0xcf20
    cf0e:	bd e9 58    	jsr	0xe958
    cf11:	20 0d       	bra	0x0xcf20
    cf13:	7d 01 1e    	tst	0x11e
    cf16:	26 08       	bne	0x0xcf20
    cf18:	86 13       	ldaa	#0x13
    cf1a:	b7 01 1e    	staa	0x11e
    cf1d:	bd e9 16    	jsr	0xe916
    cf20:	7d 01 7f    	tst	0x17f
    cf23:	27 1e       	beq	0x0xcf43
    cf25:	2a 10       	bpl	0x0xcf37
    cf27:	bd eb 1a    	jsr	0xeb1a
    cf2a:	4f          	clra
    cf2b:	b7 01 7f    	staa	0x17f
    cf2e:	b7 01 7e    	staa	0x17e
    cf31:	b7 01 1a    	staa	0x11a
    cf34:	7e d3 af    	jmp	0xd3af
    cf37:	b6 01 1e    	ldaa	0x11e
    cf3a:	81 19       	cmpa	#0x19
    cf3c:	27 ec       	beq	0x0xcf2a
    cf3e:	bd eb 1a    	jsr	0xeb1a
    cf41:	20 e7       	bra	0x0xcf2a
    cf43:	7d 01 1a    	tst	0x11a
    cf46:	26 03       	bne	0x0xcf4b
    cf48:	7e d3 af    	jmp	0xd3af
    cf4b:	b6 01 1e    	ldaa	0x11e
    cf4e:	81 13       	cmpa	#0x13
    cf50:	26 2b       	bne	0x0xcf7d
    cf52:	b6 01 1a    	ldaa	0x11a
    cf55:	2b 1f       	bmi	0x0xcf76
    cf57:	96 50       	ldaa	*0x50
    cf59:	4c          	inca
    cf5a:	84 7f       	anda	#0x7f
    cf5c:	97 50       	staa	*0x50
    cf5e:	ce 01 31    	ldx	#0x131
    cf61:	bd e9 ee    	jsr	0xe9ee
    cf64:	7f 01 1a    	clr	0x11a
    cf67:	86 03       	ldaa	#0x3
    cf69:	b7 01 1c    	staa	0x11c
    cf6c:	96 50       	ldaa	*0x50
    cf6e:	c6 50       	ldab	#0x50
    cf70:	bd af d2    	jsr	0xafd2
    cf73:	7e d3 af    	jmp	0xd3af
    cf76:	96 50       	ldaa	*0x50
    cf78:	4a          	deca
    cf79:	84 7f       	anda	#0x7f
    cf7b:	20 df       	bra	0x0xcf5c
    cf7d:	81 19       	cmpa	#0x19
    cf7f:	26 2b       	bne	0x0xcfac
    cf81:	b6 01 1a    	ldaa	0x11a
    cf84:	2b 1f       	bmi	0x0xcfa5
    cf86:	96 98       	ldaa	*0x98
    cf88:	4c          	inca
    cf89:	84 7f       	anda	#0x7f
    cf8b:	97 98       	staa	*0x98
    cf8d:	ce 01 37    	ldx	#0x137
    cf90:	bd e9 ee    	jsr	0xe9ee
    cf93:	7f 01 1a    	clr	0x11a
    cf96:	86 03       	ldaa	#0x3
    cf98:	b7 01 1c    	staa	0x11c
    cf9b:	96 98       	ldaa	*0x98
    cf9d:	c6 98       	ldab	#0x98
    cf9f:	bd af d2    	jsr	0xafd2
    cfa2:	7e d3 af    	jmp	0xd3af
    cfa5:	96 98       	ldaa	*0x98
    cfa7:	4a          	deca
    cfa8:	84 7f       	anda	#0x7f
    cfaa:	20 df       	bra	0x0xcf8b
    cfac:	7f 01 1a    	clr	0x11a
    cfaf:	7e d3 af    	jmp	0xd3af
    cfb2:	7d 01 1c    	tst	0x11c
    cfb5:	2a 03       	bpl	0x0xcfba
    cfb7:	7e e6 3f    	jmp	0xe63f
    cfba:	7d 01 1e    	tst	0x11e
    cfbd:	26 08       	bne	0x0xcfc7
    cfbf:	7d 01 1c    	tst	0x11c
    cfc2:	27 1f       	beq	0x0xcfe3
    cfc4:	7e b1 32    	jmp	0xb132
    cfc7:	7d 01 1c    	tst	0x11c
    cfca:	27 1f       	beq	0x0xcfeb
    cfcc:	cc 01 20    	ldd	#0x120
    cfcf:	fb 01 1e    	addb	0x11e
    cfd2:	8f          	xgdx
    cfd3:	bd e8 da    	jsr	0xe8da
    cfd6:	7a 01 1e    	dec	0x11e
    cfd9:	7a 01 1c    	dec	0x11c
    cfdc:	26 0d       	bne	0x0xcfeb
    cfde:	bd e9 8b    	jsr	0xe98b
    cfe1:	20 08       	bra	0x0xcfeb
    cfe3:	86 16       	ldaa	#0x16
    cfe5:	b7 01 1e    	staa	0x11e
    cfe8:	bd e9 16    	jsr	0xe916
    cfeb:	7f 01 7e    	clr	0x17e
    cfee:	7d 01 7f    	tst	0x17f
    cff1:	27 10       	beq	0x0xd003
    cff3:	bd eb 8c    	jsr	0xeb8c
    cff6:	4f          	clra
    cff7:	b7 01 7f    	staa	0x17f
    cffa:	b7 01 7e    	staa	0x17e
    cffd:	b7 01 1a    	staa	0x11a
    d000:	7e d3 af    	jmp	0xd3af
    d003:	7d 01 1a    	tst	0x11a
    d006:	26 03       	bne	0x0xd00b
    d008:	7e d3 af    	jmp	0xd3af
    d00b:	ce 50 01    	ldx	#0x5001
    d00e:	b6 01 6b    	ldaa	0x16b
    d011:	c6 04       	ldab	#0x4
    d013:	3d          	mul
    d014:	3a          	abx
    d015:	b6 01 1e    	ldaa	0x11e
    d018:	81 12       	cmpa	#0x12
    d01a:	26 69       	bne	0x0xd085
    d01c:	a6 00       	ldaa	0x0,x
    d01e:	7d 01 1a    	tst	0x11a
    d021:	2b 5c       	bmi	0x0xd07f
    d023:	4c          	inca
    d024:	81 04       	cmpa	#0x4
    d026:	25 02       	bcs	0x0xd02a
    d028:	86 03       	ldaa	#0x3
    d02a:	a7 00       	staa	0x0,x
    d02c:	8b 41       	adda	#0x41
    d02e:	b7 01 32    	staa	0x132
    d031:	7f 01 1a    	clr	0x11a
    d034:	86 01       	ldaa	#0x1
    d036:	b7 01 1c    	staa	0x11c
    d039:	a6 01       	ldaa	0x1,x
    d03b:	e6 00       	ldab	0x0,x
    d03d:	c1 01       	cmpb	#0x1
    d03f:	23 18       	bls	0x0xd059
    d041:	d7 f9       	stab	*0xf9
    d043:	bd ab 71    	jsr	0xab71
    d046:	c6 04       	ldab	#0x4
    d048:	d7 f9       	stab	*0xf9
    d04a:	bd a2 d0    	jsr	0xa2d0
    d04d:	bd a2 eb    	jsr	0xa2eb
    d050:	bd ab 71    	jsr	0xab71
    d053:	bd a5 c2    	jsr	0xa5c2
    d056:	7e d1 5a    	jmp	0xd15a
    d059:	4f          	clra
    d05a:	b7 10 22    	staa	0x1022
    d05d:	b6 10 2d    	ldaa	0x102d
    d060:	36          	psha
    d061:	84 7f       	anda	#0x7f
    d063:	b7 10 2d    	staa	0x102d
    d066:	a6 00       	ldaa	0x0,x
    d068:	e6 01       	ldab	0x1,x
    d06a:	bd 79 00    	jsr	0x7900
    d06d:	32          	pula
    d06e:	b7 10 2d    	staa	0x102d
    d071:	86 80       	ldaa	#0x80
    d073:	b7 10 22    	staa	0x1022
    d076:	bd a2 eb    	jsr	0xa2eb
    d079:	bd a5 c2    	jsr	0xa5c2
    d07c:	7e d1 5a    	jmp	0xd15a
    d07f:	4a          	deca
    d080:	2a a8       	bpl	0x0xd02a
    d082:	4f          	clra
    d083:	20 a5       	bra	0x0xd02a
    d085:	81 16       	cmpa	#0x16
    d087:	26 55       	bne	0x0xd0de
    d089:	a6 01       	ldaa	0x1,x
    d08b:	7d 01 1a    	tst	0x11a
    d08e:	2b 3c       	bmi	0x0xd0cc
    d090:	4c          	inca
    d091:	84 7f       	anda	#0x7f
    d093:	81 51       	cmpa	#0x51
    d095:	26 04       	bne	0x0xd09b
    d097:	86 53       	ldaa	#0x53
    d099:	20 06       	bra	0x0xd0a1
    d09b:	81 52       	cmpa	#0x52
    d09d:	26 02       	bne	0x0xd0a1
    d09f:	86 53       	ldaa	#0x53
    d0a1:	a7 01       	staa	0x1,x
    d0a3:	4c          	inca
    d0a4:	3c          	pshx
    d0a5:	ce 01 34    	ldx	#0x134
    d0a8:	bd e9 ee    	jsr	0xe9ee
    d0ab:	38          	pulx
    d0ac:	a6 01       	ldaa	0x1,x
    d0ae:	e6 00       	ldab	0x0,x
    d0b0:	c1 01       	cmpb	#0x1
    d0b2:	23 a5       	bls	0x0xd059
    d0b4:	d7 f9       	stab	*0xf9
    d0b6:	bd ab 71    	jsr	0xab71
    d0b9:	c6 04       	ldab	#0x4
    d0bb:	d7 f9       	stab	*0xf9
    d0bd:	bd a2 d0    	jsr	0xa2d0
    d0c0:	bd a2 eb    	jsr	0xa2eb
    d0c3:	bd ab 71    	jsr	0xab71
    d0c6:	bd a5 c2    	jsr	0xa5c2
    d0c9:	7e d1 5a    	jmp	0xd15a
    d0cc:	4a          	deca
    d0cd:	84 7f       	anda	#0x7f
    d0cf:	81 52       	cmpa	#0x52
    d0d1:	26 04       	bne	0x0xd0d7
    d0d3:	86 50       	ldaa	#0x50
    d0d5:	20 ca       	bra	0x0xd0a1
    d0d7:	81 51       	cmpa	#0x51
    d0d9:	26 c6       	bne	0x0xd0a1
    d0db:	4a          	deca
    d0dc:	20 c3       	bra	0x0xd0a1
    d0de:	81 1a       	cmpa	#0x1a
    d0e0:	26 3e       	bne	0x0xd120
    d0e2:	a6 02       	ldaa	0x2,x
    d0e4:	84 40       	anda	#0x40
    d0e6:	b7 01 7d    	staa	0x17d
    d0e9:	a6 02       	ldaa	0x2,x
    d0eb:	84 3f       	anda	#0x3f
    d0ed:	f6 50 00    	ldab	0x5000
    d0f0:	c1 06       	cmpb	#0x6
    d0f2:	27 02       	beq	0x0xd0f6
    d0f4:	20 64       	bra	0x0xd15a
    d0f6:	7d 01 1a    	tst	0x11a
    d0f9:	2b 19       	bmi	0x0xd114
    d0fb:	4c          	inca
    d0fc:	7a 50 23    	dec	0x5023
    d0ff:	2a 04       	bpl	0x0xd105
    d101:	4a          	deca
    d102:	7c 50 23    	inc	0x5023
    d105:	ba 01 7d    	oraa	0x17d
    d108:	a7 02       	staa	0x2,x
    d10a:	84 3f       	anda	#0x3f
    d10c:	ce 01 38    	ldx	#0x138
    d10f:	bd e9 ee    	jsr	0xe9ee
    d112:	20 46       	bra	0x0xd15a
    d114:	7c 50 23    	inc	0x5023
    d117:	4a          	deca
    d118:	2a eb       	bpl	0x0xd105
    d11a:	4c          	inca
    d11b:	7a 50 23    	dec	0x5023
    d11e:	20 e5       	bra	0x0xd105
    d120:	f6 50 00    	ldab	0x5000
    d123:	c1 06       	cmpb	#0x6
    d125:	27 22       	beq	0x0xd149
    d127:	a6 03       	ldaa	0x3,x
    d129:	7d 01 1a    	tst	0x11a
    d12c:	2b 18       	bmi	0x0xd146
    d12e:	4c          	inca
    d12f:	84 7f       	anda	#0x7f
    d131:	a7 03       	staa	0x3,x
    d133:	36          	psha
    d134:	ce 01 3c    	ldx	#0x13c
    d137:	bd e9 ee    	jsr	0xe9ee
    d13a:	32          	pula
    d13b:	d6 d6       	ldab	*0xd6
    d13d:	3d          	mul
    d13e:	05          	asld
    d13f:	c6 ac       	ldab	#0xac
    d141:	bd af d2    	jsr	0xafd2
    d144:	20 14       	bra	0x0xd15a
    d146:	4a          	deca
    d147:	20 e6       	bra	0x0xd12f
    d149:	e6 03       	ldab	0x3,x
    d14b:	5c          	incb
    d14c:	c4 01       	andb	#0x1
    d14e:	e7 03       	stab	0x3,x
    d150:	ce e7 69    	ldx	#0xe769
    d153:	18 ce 01 3c 	ldy	#0x13c
    d157:	bd ea 8d    	jsr	0xea8d
    d15a:	7f 01 1a    	clr	0x11a
    d15d:	86 03       	ldaa	#0x3
    d15f:	b7 01 1c    	staa	0x11c
    d162:	14 fb 80    	bset	*0xfb, #0x80
    d165:	7e d3 af    	jmp	0xd3af
    d168:	7d 01 1c    	tst	0x11c
    d16b:	2a 03       	bpl	0x0xd170
    d16d:	7e e7 6f    	jmp	0xe76f
    d170:	7d 01 1e    	tst	0x11e
    d173:	26 08       	bne	0x0xd17d
    d175:	7d 01 1c    	tst	0x11c
    d178:	27 1f       	beq	0x0xd199
    d17a:	7e b1 32    	jmp	0xb132
    d17d:	7d 01 1c    	tst	0x11c
    d180:	27 1f       	beq	0x0xd1a1
    d182:	cc 01 20    	ldd	#0x120
    d185:	fb 01 1e    	addb	0x11e
    d188:	8f          	xgdx
    d189:	bd e8 da    	jsr	0xe8da
    d18c:	7a 01 1e    	dec	0x11e
    d18f:	7a 01 1c    	dec	0x11c
    d192:	26 0d       	bne	0x0xd1a1
    d194:	bd e9 58    	jsr	0xe958
    d197:	20 08       	bra	0x0xd1a1
    d199:	86 13       	ldaa	#0x13
    d19b:	b7 01 1e    	staa	0x11e
    d19e:	bd e9 16    	jsr	0xe916
    d1a1:	7d 01 7f    	tst	0x17f
    d1a4:	27 0d       	beq	0x0xd1b3
    d1a6:	4f          	clra
    d1a7:	b7 01 7f    	staa	0x17f
    d1aa:	b7 01 7e    	staa	0x17e
    d1ad:	b7 01 1a    	staa	0x11a
    d1b0:	7e d3 af    	jmp	0xd3af
    d1b3:	7d 01 7e    	tst	0x17e
    d1b6:	27 0c       	beq	0x0xd1c4
    d1b8:	7f 01 7f    	clr	0x17f
    d1bb:	7f 01 7e    	clr	0x17e
    d1be:	7f 01 1a    	clr	0x11a
    d1c1:	7e d3 af    	jmp	0xd3af
    d1c4:	7d 01 1a    	tst	0x11a
    d1c7:	26 03       	bne	0x0xd1cc
    d1c9:	7e d3 af    	jmp	0xd3af
    d1cc:	b6 01 6b    	ldaa	0x16b
    d1cf:	c6 04       	ldab	#0x4
    d1d1:	3d          	mul
    d1d2:	ce 50 03    	ldx	#0x5003
    d1d5:	3a          	abx
    d1d6:	e6 00       	ldab	0x0,x
    d1d8:	c4 3f       	andb	#0x3f
    d1da:	f7 01 7d    	stab	0x17d
    d1dd:	e6 00       	ldab	0x0,x
    d1df:	c4 40       	andb	#0x40
    d1e1:	c8 40       	eorb	#0x40
    d1e3:	37          	pshb
    d1e4:	fa 01 7d    	orab	0x17d
    d1e7:	e7 00       	stab	0x0,x
    d1e9:	32          	pula
    d1ea:	4d          	tsta
    d1eb:	27 02       	beq	0x0xd1ef
    d1ed:	86 01       	ldaa	#0x1
    d1ef:	36          	psha
    d1f0:	4d          	tsta
    d1f1:	26 31       	bne	0x0xd224
    d1f3:	7d 00 d0    	tst	0xd0
    d1f6:	27 05       	beq	0x0xd1fd
    d1f8:	7f 00 d0    	clr	0xd0
    d1fb:	20 08       	bra	0x0xd205
    d1fd:	7d 10 29    	tst	0x1029
    d200:	2a fb       	bpl	0x0xd1fd
    d202:	f6 10 2a    	ldab	0x102a
    d205:	f6 01 6b    	ldab	0x16b
    d208:	ce 50 50    	ldx	#0x5050
    d20b:	3a          	abx
    d20c:	e6 00       	ldab	0x0,x
    d20e:	53          	comb
    d20f:	01          	nop
    d210:	01          	nop
    d211:	01          	nop
    d212:	01          	nop
    d213:	f7 10 42    	stab	0x1042
    d216:	01          	nop
    d217:	01          	nop
    d218:	01          	nop
    d219:	01          	nop
    d21a:	01          	nop
    d21b:	01          	nop
    d21c:	01          	nop
    d21d:	c6 8c       	ldab	#0x8c
    d21f:	f7 10 2a    	stab	0x102a
    d222:	20 07       	bra	0x0xd22b
    d224:	c6 44       	ldab	#0x44
    d226:	96 44       	ldaa	*0x44
    d228:	bd af d2    	jsr	0xafd2
    d22b:	33          	pulb
    d22c:	18 ce 01 31 	ldy	#0x131
    d230:	ce e7 e0    	ldx	#0xe7e0
    d233:	bd ea 8d    	jsr	0xea8d
    d236:	7f 01 1a    	clr	0x11a
    d239:	86 03       	ldaa	#0x3
    d23b:	b7 01 1c    	staa	0x11c
    d23e:	14 fb 80    	bset	*0xfb, #0x80
    d241:	7e d3 af    	jmp	0xd3af
    d244:	7d 01 1c    	tst	0x11c
    d247:	2a 03       	bpl	0x0xd24c
    d249:	7e e8 27    	jmp	0xe827
    d24c:	7d 01 1e    	tst	0x11e
    d24f:	26 08       	bne	0x0xd259
    d251:	7d 01 1c    	tst	0x11c
    d254:	27 24       	beq	0x0xd27a
    d256:	7e b1 32    	jmp	0xb132
    d259:	7d 01 1c    	tst	0x11c
    d25c:	27 24       	beq	0x0xd282
    d25e:	cc 01 20    	ldd	#0x120
    d261:	fb 01 1e    	addb	0x11e
    d264:	8f          	xgdx
    d265:	bd e8 da    	jsr	0xe8da
    d268:	7a 01 1e    	dec	0x11e
    d26b:	7a 01 1c    	dec	0x11c
    d26e:	26 12       	bne	0x0xd282
    d270:	86 1e       	ldaa	#0x1e
    d272:	b7 01 1e    	staa	0x11e
    d275:	bd e9 16    	jsr	0xe916
    d278:	20 08       	bra	0x0xd282
    d27a:	86 1e       	ldaa	#0x1e
    d27c:	b7 01 1e    	staa	0x11e
    d27f:	bd e9 16    	jsr	0xe916
    d282:	7d 01 1a    	tst	0x11a
    d285:	26 0a       	bne	0x0xd291
    d287:	4f          	clra
    d288:	b7 01 7f    	staa	0x17f
    d28b:	b7 01 7e    	staa	0x17e
    d28e:	7e d3 af    	jmp	0xd3af
    d291:	86 0c       	ldaa	#0xc
    d293:	b7 10 2d    	staa	0x102d
    d296:	4f          	clra
    d297:	97 f2       	staa	*0xf2
    d299:	97 f3       	staa	*0xf3
    d29b:	97 f4       	staa	*0xf4
    d29d:	97 f5       	staa	*0xf5
    d29f:	97 f6       	staa	*0xf6
    d2a1:	97 f7       	staa	*0xf7
    d2a3:	97 f8       	staa	*0xf8
    d2a5:	bd d3 59    	jsr	0xd359
    d2a8:	4f          	clra
    d2a9:	b7 10 22    	staa	0x1022
    d2ac:	96 99       	ldaa	*0x99
    d2ae:	80 41       	suba	#0x41
    d2b0:	97 f9       	staa	*0xf9
    d2b2:	bd ab 71    	jsr	0xab71
    d2b5:	96 9a       	ldaa	*0x9a
    d2b7:	80 41       	suba	#0x41
    d2b9:	36          	psha
    d2ba:	ce f9 c1    	ldx	#0xf9c1
    d2bd:	18 ce 78 00 	ldy	#0x7800
    d2c1:	a6 00       	ldaa	0x0,x
    d2c3:	18 a7 00    	staa	0x0,y
    d2c6:	18 08       	iny
    d2c8:	08          	inx
    d2c9:	8c fb bf    	cpx	#0xfbbf
    d2cc:	23 f3       	bls	0x0xd2c1
    d2ce:	18 ce 00 00 	ldy	#0x0
    d2d2:	ce d2 e6    	ldx	#0xd2e6
    d2d5:	a6 00       	ldaa	0x0,x
    d2d7:	18 a7 00    	staa	0x0,y
    d2da:	18 08       	iny
    d2dc:	08          	inx
    d2dd:	8c d3 58    	cpx	#0xd358
    d2e0:	25 f3       	bcs	0x0xd2d5
    d2e2:	0f          	sei
    d2e3:	7e 00 00    	jmp	0x0
    d2e6:	32          	pula
    d2e7:	4d          	tsta
    d2e8:	26 0c       	bne	0x0xd2f6
    d2ea:	b6 10 00    	ldaa	0x1000
    d2ed:	84 9f       	anda	#0x9f
    d2ef:	8a 20       	oraa	#0x20
    d2f1:	b7 10 00    	staa	0x1000
    d2f4:	20 0a       	bra	0x0xd300
    d2f6:	b6 10 00    	ldaa	0x1000
    d2f9:	84 9f       	anda	#0x9f
    d2fb:	8a 40       	oraa	#0x40
    d2fd:	b7 10 00    	staa	0x1000
    d300:	ce 20 00    	ldx	#0x2000
    d303:	18 ce 80 00 	ldy	#0x8000
    d307:	86 aa       	ldaa	#0xaa
    d309:	b7 d5 55    	staa	0xd555
    d30c:	86 55       	ldaa	#0x55
    d30e:	b7 aa aa    	staa	0xaaaa
    d311:	86 a0       	ldaa	#0xa0
    d313:	b7 d5 55    	staa	0xd555
    d316:	86 80       	ldaa	#0x80
    d318:	e6 00       	ldab	0x0,x
    d31a:	18 e7 00    	stab	0x0,y
    d31d:	08          	inx
    d31e:	18 08       	iny
    d320:	4a          	deca
    d321:	26 f5       	bne	0x0xd318
    d323:	86 08       	ldaa	#0x8
    d325:	b7 10 23    	staa	0x1023
    d328:	fc 10 0e    	ldd	0x100e
    d32b:	c3 5d c0    	addd	#0x5dc0
    d32e:	fd 10 1e    	std	0x101e
    d331:	01          	nop
    d332:	01          	nop
    d333:	b6 10 23    	ldaa	0x1023
    d336:	85 08       	bita	#0x8
    d338:	27 f7       	beq	0x0xd331
    d33a:	8c 78 00    	cpx	#0x7800
    d33d:	25 c8       	bcs	0x0xd307
    d33f:	22 06       	bhi	0x0xd347
    d341:	18 ce fe 00 	ldy	#0xfe00
    d345:	20 c0       	bra	0x0xd307
    d347:	18 8c 00 00 	cpy	#0x0
    d34b:	26 ba       	bne	0x0xd307
    d34d:	b6 10 00    	ldaa	0x1000
    d350:	84 8f       	anda	#0x8f
    d352:	b7 10 00    	staa	0x1000
    d355:	7e 80 00    	jmp	0x8000
    d358:	01          	nop
    d359:	ce 10 23    	ldx	#0x1023
    d35c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd35c
    d360:	bd e9 3b    	jsr	0xe93b
    d363:	ce 01 20    	ldx	#0x120
    d366:	18 ce d3 8f 	ldy	#0xd38f
    d36a:	c6 20       	ldab	#0x20
    d36c:	18 a6 00    	ldaa	0x0,y
    d36f:	a7 00       	staa	0x0,x
    d371:	08          	inx
    d372:	18 08       	iny
    d374:	5a          	decb
    d375:	26 f5       	bne	0x0xd36c
    d377:	7f 01 1e    	clr	0x11e
    d37a:	86 20       	ldaa	#0x20
    d37c:	b7 01 1c    	staa	0x11c
    d37f:	ce 10 23    	ldx	#0x1023
    d382:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd382
    d386:	bd e8 8f    	jsr	0xe88f
    d389:	7d 01 1c    	tst	0x11c
    d38c:	26 f1       	bne	0x0xd37f
    d38e:	39          	rts
    d38f:	20 57       	bra	0x0xd3e8
    d391:	52          	.byte	0x52
    d392:	49          	rola
    d393:	54          	lsrb
    d394:	49          	rola
    d395:	4e          	.byte	0x4e
    d396:	47          	asra
    d397:	20 42       	bra	0x0xd3db
    d399:	41          	.byte	0x41
    d39a:	4e          	.byte	0x4e
    d39b:	4b          	.byte	0x4b
    d39c:	20 2e       	bra	0x0xd3cc
    d39e:	2e 20       	bgt	0x0xd3c0
    d3a0:	54          	lsrb
    d3a1:	41          	.byte	0x41
    d3a2:	4b          	.byte	0x4b
    d3a3:	45          	.byte	0x45
    d3a4:	20 35       	bra	0x0xd3db
    d3a6:	20 2e       	bra	0x0xd3d6
    d3a8:	2e 2e       	bgt	0x0xd3d8
    d3aa:	2e 2e       	bgt	0x0xd3da
    d3ac:	2e 2e       	bgt	0x0xd3dc
    d3ae:	2e fe       	bgt	0x0xd3ae
    d3b0:	01          	nop
    d3b1:	c0 bc       	subb	#0xbc
    d3b3:	01          	nop
    d3b4:	c2 26       	sbcb	#0x26
    d3b6:	03          	fdiv
    d3b7:	7e 95 68    	jmp	0x9568
    d3ba:	18 fe 01 6c 	ldy	0x16c
    d3be:	18 3c       	pshy
    d3c0:	7e 91 d5    	jmp	0x91d5
    d3c3:	7f 00 ff    	clr	0xff
    d3c6:	7d 01 1d    	tst	0x11d
    d3c9:	27 2f       	beq	0x0xd3fa
    d3cb:	ce 10 23    	ldx	#0x1023
    d3ce:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd3ce
    d3d2:	86 f7       	ldaa	#0xf7
    d3d4:	b4 10 00    	anda	0x1000
    d3d7:	b7 10 00    	staa	0x1000
    d3da:	86 0c       	ldaa	#0xc
    d3dc:	b7 10 47    	staa	0x1047
    d3df:	86 80       	ldaa	#0x80
    d3e1:	ba 10 00    	oraa	0x1000
    d3e4:	b7 10 00    	staa	0x1000
    d3e7:	01          	nop
    d3e8:	88 80       	eora	#0x80
    d3ea:	b7 10 00    	staa	0x1000
    d3ed:	7f 01 1d    	clr	0x11d
    d3f0:	bd e9 07    	jsr	0xe907
    d3f3:	ce 10 23    	ldx	#0x1023
    d3f6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd3f6
    d3fa:	bd e9 3b    	jsr	0xe93b
    d3fd:	ce 01 20    	ldx	#0x120
    d400:	c6 10       	ldab	#0x10
    d402:	96 f9       	ldaa	*0xf9
    d404:	81 04       	cmpa	#0x4
    d406:	25 06       	bcs	0x0xd40e
    d408:	18 ce d4 7d 	ldy	#0xd47d
    d40c:	20 07       	bra	0x0xd415
    d40e:	14 d1 80    	bset	*0xd1, #0x80
    d411:	18 ce d4 6d 	ldy	#0xd46d
    d415:	18 a6 00    	ldaa	0x0,y
    d418:	a7 00       	staa	0x0,x
    d41a:	08          	inx
    d41b:	18 08       	iny
    d41d:	5a          	decb
    d41e:	26 f5       	bne	0x0xd415
    d420:	96 f9       	ldaa	*0xf9
    d422:	81 04       	cmpa	#0x4
    d424:	27 30       	beq	0x0xd456
    d426:	8b 41       	adda	#0x41
    d428:	b7 01 2c    	staa	0x12c
    d42b:	96 f9       	ldaa	*0xf9
    d42d:	81 01       	cmpa	#0x1
    d42f:	23 05       	bls	0x0xd436
    d431:	86 41       	ldaa	#0x41
    d433:	b7 01 28    	staa	0x128
    d436:	96 fa       	ldaa	*0xfa
    d438:	4c          	inca
    d439:	ce 01 2d    	ldx	#0x12d
    d43c:	bd e9 ee    	jsr	0xe9ee
    d43f:	08          	inx
    d440:	18 ce 00 c0 	ldy	#0xc0
    d444:	c6 10       	ldab	#0x10
    d446:	18 a6 00    	ldaa	0x0,y
    d449:	84 7f       	anda	#0x7f
    d44b:	a7 00       	staa	0x0,x
    d44d:	08          	inx
    d44e:	18 08       	iny
    d450:	5a          	decb
    d451:	26 f3       	bne	0x0xd446
    d453:	7e e8 78    	jmp	0xe878
    d456:	b6 01 6a    	ldaa	0x16a
    d459:	4c          	inca
    d45a:	ce 01 2d    	ldx	#0x12d
    d45d:	bd e9 ee    	jsr	0xe9ee
    d460:	08          	inx
    d461:	cc 50 00    	ldd	#0x5000
    d464:	c3 00 40    	addd	#0x40
    d467:	18 8f       	xgdy
    d469:	c6 10       	ldab	#0x10
    d46b:	20 d9       	bra	0x0xd446
    d46d:	50          	negb
    d46e:	41          	.byte	0x41
    d46f:	54          	lsrb
    d470:	43          	coma
    d471:	48          	asla
    d472:	20 20       	bra	0x0xd494
    d474:	52          	.byte	0x52
    d475:	4f          	clra
    d476:	4d          	tsta
    d477:	20 20       	bra	0x0xd499
    d479:	20 20       	bra	0x0xd49b
    d47b:	20 20       	bra	0x0xd49d
    d47d:	4d          	tsta
    d47e:	55          	.byte	0x55
    d47f:	4c          	inca
    d480:	54          	lsrb
    d481:	49          	rola
    d482:	20 20       	bra	0x0xd4a4
    d484:	20 20       	bra	0x0xd4a6
    d486:	20 20       	bra	0x0xd4a8
    d488:	20 20       	bra	0x0xd4aa
    d48a:	20 20       	bra	0x0xd4ac
    d48c:	20 7d       	bra	0x0xd50b
    d48e:	01          	nop
    d48f:	7e 27 61    	jmp	0x2761
    d492:	7f 01 7e    	clr	0x17e
    d495:	96 f9       	ldaa	*0xf9
    d497:	81 04       	cmpa	#0x4
    d499:	25 1e       	bcs	0x0xd4b9
    d49b:	8b 41       	adda	#0x41
    d49d:	b7 01 2c    	staa	0x12c
    d4a0:	b6 01 6a    	ldaa	0x16a
    d4a3:	4c          	inca
    d4a4:	ce 01 2d    	ldx	#0x12d
    d4a7:	bd e9 ee    	jsr	0xe9ee
    d4aa:	b6 01 6a    	ldaa	0x16a
    d4ad:	c6 50       	ldab	#0x50
    d4af:	3d          	mul
    d4b0:	c3 20 00    	addd	#0x2000
    d4b3:	c3 00 40    	addd	#0x40
    d4b6:	8f          	xgdx
    d4b7:	20 1a       	bra	0x0xd4d3
    d4b9:	8b 41       	adda	#0x41
    d4bb:	b7 01 2c    	staa	0x12c
    d4be:	96 fa       	ldaa	*0xfa
    d4c0:	4c          	inca
    d4c1:	ce 01 2d    	ldx	#0x12d
    d4c4:	bd e9 ee    	jsr	0xe9ee
    d4c7:	96 fa       	ldaa	*0xfa
    d4c9:	c6 b0       	ldab	#0xb0
    d4cb:	3d          	mul
    d4cc:	c3 20 00    	addd	#0x2000
    d4cf:	c3 00 a0    	addd	#0xa0
    d4d2:	8f          	xgdx
    d4d3:	18 ce 01 30 	ldy	#0x130
    d4d7:	c6 10       	ldab	#0x10
    d4d9:	a6 00       	ldaa	0x0,x
    d4db:	84 7f       	anda	#0x7f
    d4dd:	18 a7 00    	staa	0x0,y
    d4e0:	08          	inx
    d4e1:	18 08       	iny
    d4e3:	5a          	decb
    d4e4:	26 f3       	bne	0x0xd4d9
    d4e6:	ce 10 23    	ldx	#0x1023
    d4e9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd4e9
    d4ed:	bd e9 3b    	jsr	0xe93b
    d4f0:	7e e8 78    	jmp	0xe878
    d4f3:	7d 01 1d    	tst	0x11d
    d4f6:	26 0a       	bne	0x0xd502
    d4f8:	bd ea 65    	jsr	0xea65
    d4fb:	ce 10 23    	ldx	#0x1023
    d4fe:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd4fe
    d502:	bd e9 3b    	jsr	0xe93b
    d505:	ce 01 20    	ldx	#0x120
    d508:	18 ce d5 92 	ldy	#0xd592
    d50c:	c6 20       	ldab	#0x20
    d50e:	18 a6 00    	ldaa	0x0,y
    d511:	a7 00       	staa	0x0,x
    d513:	08          	inx
    d514:	18 08       	iny
    d516:	5a          	decb
    d517:	26 f5       	bne	0x0xd50e
    d519:	96 f9       	ldaa	*0xf9
    d51b:	8b 41       	adda	#0x41
    d51d:	b7 01 25    	staa	0x125
    d520:	96 f9       	ldaa	*0xf9
    d522:	81 02       	cmpa	#0x2
    d524:	24 07       	bcc	0x0xd52d
    d526:	86 02       	ldaa	#0x2
    d528:	97 f9       	staa	*0xf9
    d52a:	bd ab 71    	jsr	0xab71
    d52d:	8b 41       	adda	#0x41
    d52f:	b7 01 2c    	staa	0x12c
    d532:	96 fa       	ldaa	*0xfa
    d534:	7d 00 d1    	tst	0xd1
    d537:	2b 3a       	bmi	0x0xd573
    d539:	b6 01 6a    	ldaa	0x16a
    d53c:	7d 00 fb    	tst	0xfb
    d53f:	27 32       	beq	0x0xd573
    d541:	2b 30       	bmi	0x0xd573
    d543:	86 04       	ldaa	#0x4
    d545:	97 f9       	staa	*0xf9
    d547:	bd ab 71    	jsr	0xab71
    d54a:	ce 50 01    	ldx	#0x5001
    d54d:	b6 01 6b    	ldaa	0x16b
    d550:	c6 04       	ldab	#0x4
    d552:	3d          	mul
    d553:	3a          	abx
    d554:	a6 00       	ldaa	0x0,x
    d556:	8b 41       	adda	#0x41
    d558:	b7 01 25    	staa	0x125
    d55b:	a6 01       	ldaa	0x1,x
    d55d:	36          	psha
    d55e:	a6 00       	ldaa	0x0,x
    d560:	81 02       	cmpa	#0x2
    d562:	24 02       	bcc	0x0xd566
    d564:	86 02       	ldaa	#0x2
    d566:	97 f9       	staa	*0xf9
    d568:	bd ab 71    	jsr	0xab71
    d56b:	8b 41       	adda	#0x41
    d56d:	b7 01 2c    	staa	0x12c
    d570:	32          	pula
    d571:	97 fa       	staa	*0xfa
    d573:	4c          	inca
    d574:	ce 01 26    	ldx	#0x126
    d577:	bd e9 ee    	jsr	0xe9ee
    d57a:	a7 07       	staa	0x7,x
    d57c:	09          	dex
    d57d:	a6 00       	ldaa	0x0,x
    d57f:	a7 07       	staa	0x7,x
    d581:	09          	dex
    d582:	a6 00       	ldaa	0x0,x
    d584:	a7 07       	staa	0x7,x
    d586:	96 f9       	ldaa	*0xf9
    d588:	81 04       	cmpa	#0x4
    d58a:	27 03       	beq	0x0xd58f
    d58c:	7e d4 c7    	jmp	0xd4c7
    d58f:	7e d4 56    	jmp	0xd456
    d592:	53          	comb
    d593:	41          	.byte	0x41
    d594:	56          	rorb
    d595:	45          	.byte	0x45
    d596:	20 20       	bra	0x0xd5b8
    d598:	20 20       	bra	0x0xd5ba
    d59a:	20 20       	bra	0x0xd5bc
    d59c:	3e          	wai
    d59d:	20 20       	bra	0x0xd5bf
    d59f:	20 20       	bra	0x0xd5c1
    d5a1:	20 20       	bra	0x0xd5c3
    d5a3:	20 20       	bra	0x0xd5c5
    d5a5:	20 20       	bra	0x0xd5c7
    d5a7:	20 20       	bra	0x0xd5c9
    d5a9:	20 20       	bra	0x0xd5cb
    d5ab:	20 20       	bra	0x0xd5cd
    d5ad:	20 20       	bra	0x0xd5cf
    d5af:	20 20       	bra	0x0xd5d1
    d5b1:	20 7d       	bra	0x0xd630
    d5b3:	01          	nop
    d5b4:	1d 26 0a    	bclr	0x26,x, #0x0a
    d5b7:	bd ea 65    	jsr	0xea65
    d5ba:	ce 10 23    	ldx	#0x1023
    d5bd:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd5bd
    d5c1:	bd e9 3b    	jsr	0xe93b
    d5c4:	ce 01 20    	ldx	#0x120
    d5c7:	18 ce d6 06 	ldy	#0xd606
    d5cb:	c6 20       	ldab	#0x20
    d5cd:	96 f9       	ldaa	*0xf9
    d5cf:	81 04       	cmpa	#0x4
    d5d1:	26 04       	bne	0x0xd5d7
    d5d3:	18 ce d6 26 	ldy	#0xd626
    d5d7:	18 a6 00    	ldaa	0x0,y
    d5da:	a7 00       	staa	0x0,x
    d5dc:	08          	inx
    d5dd:	18 08       	iny
    d5df:	5a          	decb
    d5e0:	26 f5       	bne	0x0xd5d7
    d5e2:	96 f9       	ldaa	*0xf9
    d5e4:	81 04       	cmpa	#0x4
    d5e6:	27 11       	beq	0x0xd5f9
    d5e8:	8b 41       	adda	#0x41
    d5ea:	b7 01 2c    	staa	0x12c
    d5ed:	96 fa       	ldaa	*0xfa
    d5ef:	4c          	inca
    d5f0:	ce 01 2d    	ldx	#0x12d
    d5f3:	bd e9 ee    	jsr	0xe9ee
    d5f6:	7e d4 3f    	jmp	0xd43f
    d5f9:	b6 01 6a    	ldaa	0x16a
    d5fc:	4c          	inca
    d5fd:	ce 01 2d    	ldx	#0x12d
    d600:	bd e9 ee    	jsr	0xe9ee
    d603:	7e d4 56    	jmp	0xd456
    d606:	4e          	.byte	0x4e
    d607:	41          	.byte	0x41
    d608:	4d          	tsta
    d609:	45          	.byte	0x45
    d60a:	20 50       	bra	0x0xd65c
    d60c:	41          	.byte	0x41
    d60d:	54          	lsrb
    d60e:	43          	coma
    d60f:	48          	asla
    d610:	20 20       	bra	0x0xd632
    d612:	20 20       	bra	0x0xd634
    d614:	20 20       	bra	0x0xd636
    d616:	20 20       	bra	0x0xd638
    d618:	20 20       	bra	0x0xd63a
    d61a:	20 20       	bra	0x0xd63c
    d61c:	20 20       	bra	0x0xd63e
    d61e:	20 20       	bra	0x0xd640
    d620:	20 20       	bra	0x0xd642
    d622:	20 20       	bra	0x0xd644
    d624:	20 20       	bra	0x0xd646
    d626:	4e          	.byte	0x4e
    d627:	41          	.byte	0x41
    d628:	4d          	tsta
    d629:	45          	.byte	0x45
    d62a:	20 4d       	bra	0x0xd679
    d62c:	55          	.byte	0x55
    d62d:	4c          	inca
    d62e:	54          	lsrb
    d62f:	49          	rola
    d630:	20 20       	bra	0x0xd652
    d632:	20 20       	bra	0x0xd654
    d634:	20 20       	bra	0x0xd656
    d636:	20 20       	bra	0x0xd658
    d638:	20 20       	bra	0x0xd65a
    d63a:	20 20       	bra	0x0xd65c
    d63c:	20 20       	bra	0x0xd65e
    d63e:	20 20       	bra	0x0xd660
    d640:	20 20       	bra	0x0xd662
    d642:	20 20       	bra	0x0xd664
    d644:	20 20       	bra	0x0xd666
    d646:	bd e9 3b    	jsr	0xe93b
    d649:	ce 01 20    	ldx	#0x120
    d64c:	18 ce d6 84 	ldy	#0xd684
    d650:	c6 20       	ldab	#0x20
    d652:	18 a6 00    	ldaa	0x0,y
    d655:	a7 00       	staa	0x0,x
    d657:	08          	inx
    d658:	18 08       	iny
    d65a:	5a          	decb
    d65b:	26 f5       	bne	0x0xd652
    d65d:	ce d6 a4    	ldx	#0xd6a4
    d660:	f6 01 6f    	ldab	0x16f
    d663:	18 ce 01 30 	ldy	#0x130
    d667:	bd ea a1    	jsr	0xeaa1
    d66a:	ce d6 e8    	ldx	#0xd6e8
    d66d:	d6 93       	ldab	*0x93
    d66f:	18 ce 01 36 	ldy	#0x136
    d673:	bd ea a1    	jsr	0xeaa1
    d676:	ce d6 fc    	ldx	#0xd6fc
    d679:	5f          	clrb
    d67a:	18 ce 01 3d 	ldy	#0x13d
    d67e:	bd ea 8d    	jsr	0xea8d
    d681:	7e e8 78    	jmp	0xe878
    d684:	43          	coma
    d685:	48          	asla
    d686:	41          	.byte	0x41
    d687:	4e          	.byte	0x4e
    d688:	20 20       	bra	0x0xd6aa
    d68a:	54          	lsrb
    d68b:	55          	.byte	0x55
    d68c:	4e          	.byte	0x4e
    d68d:	45          	.byte	0x45
    d68e:	20 20       	bra	0x0xd6b0
    d690:	49          	rola
    d691:	4e          	.byte	0x4e
    d692:	49          	rola
    d693:	54          	lsrb
    d694:	20 20       	bra	0x0xd6b6
    d696:	20 20       	bra	0x0xd6b8
    d698:	20 20       	bra	0x0xd6ba
    d69a:	20 20       	bra	0x0xd6bc
    d69c:	20 20       	bra	0x0xd6be
    d69e:	20 20       	bra	0x0xd6c0
    d6a0:	20 20       	bra	0x0xd6c2
    d6a2:	20 20       	bra	0x0xd6c4
    d6a4:	20 20       	bra	0x0xd6c6
    d6a6:	20 31       	bra	0x0xd6d9
    d6a8:	20 20       	bra	0x0xd6ca
    d6aa:	20 32       	bra	0x0xd6de
    d6ac:	20 20       	bra	0x0xd6ce
    d6ae:	20 33       	bra	0x0xd6e3
    d6b0:	20 20       	bra	0x0xd6d2
    d6b2:	20 34       	bra	0x0xd6e8
    d6b4:	20 20       	bra	0x0xd6d6
    d6b6:	20 35       	bra	0x0xd6ed
    d6b8:	20 20       	bra	0x0xd6da
    d6ba:	20 36       	bra	0x0xd6f2
    d6bc:	20 20       	bra	0x0xd6de
    d6be:	20 37       	bra	0x0xd6f7
    d6c0:	20 20       	bra	0x0xd6e2
    d6c2:	20 38       	bra	0x0xd6fc
    d6c4:	20 20       	bra	0x0xd6e6
    d6c6:	20 39       	bra	0x0xd701
    d6c8:	20 20       	bra	0x0xd6ea
    d6ca:	31          	ins
    d6cb:	30          	tsx
    d6cc:	20 20       	bra	0x0xd6ee
    d6ce:	31          	ins
    d6cf:	31          	ins
    d6d0:	20 20       	bra	0x0xd6f2
    d6d2:	31          	ins
    d6d3:	32          	pula
    d6d4:	20 20       	bra	0x0xd6f6
    d6d6:	31          	ins
    d6d7:	33          	pulb
    d6d8:	20 20       	bra	0x0xd6fa
    d6da:	31          	ins
    d6db:	34          	des
    d6dc:	20 20       	bra	0x0xd6fe
    d6de:	31          	ins
    d6df:	35          	txs
    d6e0:	20 20       	bra	0x0xd702
    d6e2:	31          	ins
    d6e3:	36          	psha
    d6e4:	4f          	clra
    d6e5:	4d          	tsta
    d6e6:	4e          	.byte	0x4e
    d6e7:	49          	rola
    d6e8:	20 4f       	bra	0x0xd739
    d6ea:	46          	rora
    d6eb:	46          	rora
    d6ec:	20 39       	bra	0x0xd727
    d6ee:	30          	tsx
    d6ef:	25 20       	bcs	0x0xd711
    d6f1:	39          	rts
    d6f2:	35          	txs
    d6f3:	25 20       	bcs	0x0xd715
    d6f5:	39          	rts
    d6f6:	38          	pulx
    d6f7:	25 31       	bcs	0x0xd72a
    d6f9:	30          	tsx
    d6fa:	30          	tsx
    d6fb:	25 20       	bcs	0x0xd71d
    d6fd:	4e          	.byte	0x4e
    d6fe:	4f          	clra
    d6ff:	59          	rolb
    d700:	45          	.byte	0x45
    d701:	53          	comb
    d702:	bd e9 3b    	jsr	0xe93b
    d705:	ce 01 20    	ldx	#0x120
    d708:	18 ce d7 3e 	ldy	#0xd73e
    d70c:	c6 20       	ldab	#0x20
    d70e:	18 a6 00    	ldaa	0x0,y
    d711:	a7 00       	staa	0x0,x
    d713:	08          	inx
    d714:	18 08       	iny
    d716:	5a          	decb
    d717:	26 f5       	bne	0x0xd70e
    d719:	ce d7 5e    	ldx	#0xd75e
    d71c:	f6 01 70    	ldab	0x170
    d71f:	18 ce 01 30 	ldy	#0x130
    d723:	bd ea a1    	jsr	0xeaa1
    d726:	ce d7 6a    	ldx	#0xd76a
    d729:	d6 94       	ldab	*0x94
    d72b:	18 ce 01 36 	ldy	#0x136
    d72f:	bd ea a1    	jsr	0xeaa1
    d732:	b6 01 71    	ldaa	0x171
    d735:	ce 01 3c    	ldx	#0x13c
    d738:	bd e9 ee    	jsr	0xe9ee
    d73b:	7e e8 78    	jmp	0xe878
    d73e:	4d          	tsta
    d73f:	50          	negb
    d740:	52          	.byte	0x52
    d741:	4f          	clra
    d742:	20 20       	bra	0x0xd764
    d744:	4b          	.byte	0x4b
    d745:	4e          	.byte	0x4e
    d746:	4f          	clra
    d747:	42          	.byte	0x42
    d748:	20 20       	bra	0x0xd76a
    d74a:	4c          	inca
    d74b:	43          	coma
    d74c:	44          	lsra
    d74d:	20 20       	bra	0x0xd76f
    d74f:	20 20       	bra	0x0xd771
    d751:	20 20       	bra	0x0xd773
    d753:	20 20       	bra	0x0xd775
    d755:	20 20       	bra	0x0xd777
    d757:	20 20       	bra	0x0xd779
    d759:	20 20       	bra	0x0xd77b
    d75b:	20 20       	bra	0x0xd77d
    d75d:	20 20       	bra	0x0xd77f
    d75f:	4f          	clra
    d760:	46          	rora
    d761:	46          	rora
    d762:	4f          	clra
    d763:	4e          	.byte	0x4e
    d764:	20 31       	bra	0x0xd797
    d766:	4f          	clra
    d767:	4e          	.byte	0x4e
    d768:	20 32       	bra	0x0xd79c
    d76a:	4a          	deca
    d76b:	55          	.byte	0x55
    d76c:	4d          	tsta
    d76d:	50          	negb
    d76e:	45          	.byte	0x45
    d76f:	44          	lsra
    d770:	49          	rola
    d771:	54          	lsrb
    d772:	4d          	tsta
    d773:	54          	lsrb
    d774:	43          	coma
    d775:	48          	asla
    d776:	bd e9 3b    	jsr	0xe93b
    d779:	96 f9       	ldaa	*0xf9
    d77b:	81 04       	cmpa	#0x4
    d77d:	27 0b       	beq	0x0xd78a
    d77f:	ce 01 20    	ldx	#0x120
    d782:	18 ce d7 c1 	ldy	#0xd7c1
    d786:	c6 20       	ldab	#0x20
    d788:	20 09       	bra	0x0xd793
    d78a:	ce 01 20    	ldx	#0x120
    d78d:	18 ce d7 e1 	ldy	#0xd7e1
    d791:	c6 20       	ldab	#0x20
    d793:	18 a6 00    	ldaa	0x0,y
    d796:	a7 00       	staa	0x0,x
    d798:	08          	inx
    d799:	18 08       	iny
    d79b:	5a          	decb
    d79c:	26 f5       	bne	0x0xd793
    d79e:	96 f9       	ldaa	*0xf9
    d7a0:	81 04       	cmpa	#0x4
    d7a2:	26 0c       	bne	0x0xd7b0
    d7a4:	b6 01 6a    	ldaa	0x16a
    d7a7:	4c          	inca
    d7a8:	ce 01 3c    	ldx	#0x13c
    d7ab:	bd e9 ee    	jsr	0xe9ee
    d7ae:	20 0e       	bra	0x0xd7be
    d7b0:	8b 41       	adda	#0x41
    d7b2:	b7 01 3b    	staa	0x13b
    d7b5:	96 fa       	ldaa	*0xfa
    d7b7:	4c          	inca
    d7b8:	ce 01 3c    	ldx	#0x13c
    d7bb:	bd e9 ee    	jsr	0xe9ee
    d7be:	7e e8 78    	jmp	0xe878
    d7c1:	53          	comb
    d7c2:	59          	rolb
    d7c3:	53          	comb
    d7c4:	58          	aslb
    d7c5:	20 53       	bra	0x0xd81a
    d7c7:	45          	.byte	0x45
    d7c8:	4e          	.byte	0x4e
    d7c9:	44          	lsra
    d7ca:	2c 50       	bge	0x0xd81c
    d7cc:	49          	rola
    d7cd:	43          	coma
    d7ce:	4b          	.byte	0x4b
    d7cf:	23 2c       	bls	0x0xd7fd
    d7d1:	50          	negb
    d7d2:	52          	.byte	0x52
    d7d3:	45          	.byte	0x45
    d7d4:	53          	comb
    d7d5:	53          	comb
    d7d6:	20 53       	bra	0x0xd82b
    d7d8:	41          	.byte	0x41
    d7d9:	56          	rorb
    d7da:	45          	.byte	0x45
    d7db:	20 20       	bra	0x0xd7fd
    d7dd:	20 20       	bra	0x0xd7ff
    d7df:	20 20       	bra	0x0xd801
    d7e1:	53          	comb
    d7e2:	45          	.byte	0x45
    d7e3:	4c          	inca
    d7e4:	45          	.byte	0x45
    d7e5:	43          	coma
    d7e6:	54          	lsrb
    d7e7:	20 4d       	bra	0x0xd836
    d7e9:	55          	.byte	0x55
    d7ea:	4c          	inca
    d7eb:	54          	lsrb
    d7ec:	49          	rola
    d7ed:	2c 20       	bge	0x0xd80f
    d7ef:	20 20       	bra	0x0xd811
    d7f1:	50          	negb
    d7f2:	52          	.byte	0x52
    d7f3:	45          	.byte	0x45
    d7f4:	53          	comb
    d7f5:	53          	comb
    d7f6:	20 53       	bra	0x0xd84b
    d7f8:	41          	.byte	0x41
    d7f9:	56          	rorb
    d7fa:	45          	.byte	0x45
    d7fb:	20 20       	bra	0x0xd81d
    d7fd:	20 20       	bra	0x0xd81f
    d7ff:	20 20       	bra	0x0xd821
    d801:	7d 01 1d    	tst	0x11d
    d804:	26 0a       	bne	0x0xd810
    d806:	bd ea 65    	jsr	0xea65
    d809:	ce 10 23    	ldx	#0x1023
    d80c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd80c
    d810:	bd e9 3b    	jsr	0xe93b
    d813:	ce 01 20    	ldx	#0x120
    d816:	18 ce d8 48 	ldy	#0xd848
    d81a:	c6 20       	ldab	#0x20
    d81c:	18 a6 00    	ldaa	0x0,y
    d81f:	a7 00       	staa	0x0,x
    d821:	08          	inx
    d822:	18 08       	iny
    d824:	5a          	decb
    d825:	26 f5       	bne	0x0xd81c
    d827:	ce 01 31    	ldx	#0x131
    d82a:	96 40       	ldaa	*0x40
    d82c:	bd ea 2a    	jsr	0xea2a
    d82f:	ce d8 68    	ldx	#0xd868
    d832:	d6 41       	ldab	*0x41
    d834:	c4 0f       	andb	#0xf
    d836:	18 ce 01 36 	ldy	#0x136
    d83a:	bd ea a1    	jsr	0xeaa1
    d83d:	ce 01 3c    	ldx	#0x13c
    d840:	96 67       	ldaa	*0x67
    d842:	bd e9 ee    	jsr	0xe9ee
    d845:	7e e8 78    	jmp	0xe878
    d848:	46          	rora
    d849:	49          	rola
    d84a:	4e          	.byte	0x4e
    d84b:	45          	.byte	0x45
    d84c:	20 4d       	bra	0x0xd89b
    d84e:	4f          	clra
    d84f:	44          	lsra
    d850:	45          	.byte	0x45
    d851:	32          	pula
    d852:	20 45       	bra	0x0xd899
    d854:	4e          	.byte	0x4e
    d855:	56          	rorb
    d856:	31          	ins
    d857:	20 20       	bra	0x0xd879
    d859:	20 20       	bra	0x0xd87b
    d85b:	20 20       	bra	0x0xd87d
    d85d:	20 20       	bra	0x0xd87f
    d85f:	20 20       	bra	0x0xd881
    d861:	20 20       	bra	0x0xd883
    d863:	20 20       	bra	0x0xd885
    d865:	20 20       	bra	0x0xd887
    d867:	20 4e       	bra	0x0xd8b7
    d869:	4f          	clra
    d86a:	52          	.byte	0x52
    d86b:	4d          	tsta
    d86c:	48          	asla
    d86d:	41          	.byte	0x41
    d86e:	4c          	inca
    d86f:	46          	rora
    d870:	4e          	.byte	0x4e
    d871:	4f          	clra
    d872:	43          	coma
    d873:	56          	rorb
    d874:	4c          	inca
    d875:	4f          	clra
    d876:	57          	asrb
    d877:	31          	ins
    d878:	4c          	inca
    d879:	4f          	clra
    d87a:	57          	asrb
    d87b:	32          	pula
    d87c:	7d 01 1d    	tst	0x11d
    d87f:	26 0a       	bne	0x0xd88b
    d881:	bd ea 65    	jsr	0xea65
    d884:	ce 10 23    	ldx	#0x1023
    d887:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd887
    d88b:	bd e9 3b    	jsr	0xe93b
    d88e:	ce 01 20    	ldx	#0x120
    d891:	18 ce d8 be 	ldy	#0xd8be
    d895:	c6 20       	ldab	#0x20
    d897:	18 a6 00    	ldaa	0x0,y
    d89a:	a7 00       	staa	0x0,x
    d89c:	08          	inx
    d89d:	18 08       	iny
    d89f:	5a          	decb
    d8a0:	26 f5       	bne	0x0xd897
    d8a2:	ce 01 31    	ldx	#0x131
    d8a5:	b6 01 6e    	ldaa	0x16e
    d8a8:	bd ea 2a    	jsr	0xea2a
    d8ab:	ce 01 37    	ldx	#0x137
    d8ae:	96 3e       	ldaa	*0x3e
    d8b0:	bd e9 ee    	jsr	0xe9ee
    d8b3:	08          	inx
    d8b4:	08          	inx
    d8b5:	08          	inx
    d8b6:	96 3f       	ldaa	*0x3f
    d8b8:	bd e9 ee    	jsr	0xe9ee
    d8bb:	7e e8 78    	jmp	0xe878
    d8be:	54          	lsrb
    d8bf:	55          	.byte	0x55
    d8c0:	4e          	.byte	0x4e
    d8c1:	45          	.byte	0x45
    d8c2:	20 20       	bra	0x0xd8e4
    d8c4:	4f          	clra
    d8c5:	53          	comb
    d8c6:	43          	coma
    d8c7:	31          	ins
    d8c8:	20 4f       	bra	0x0xd919
    d8ca:	53          	comb
    d8cb:	43          	coma
    d8cc:	32          	pula
    d8cd:	20 20       	bra	0x0xd8ef
    d8cf:	20 20       	bra	0x0xd8f1
    d8d1:	20 20       	bra	0x0xd8f3
    d8d3:	20 20       	bra	0x0xd8f5
    d8d5:	20 20       	bra	0x0xd8f7
    d8d7:	20 20       	bra	0x0xd8f9
    d8d9:	20 20       	bra	0x0xd8fb
    d8db:	20 20       	bra	0x0xd8fd
    d8dd:	20 7d       	bra	0x0xd95c
    d8df:	01          	nop
    d8e0:	1d 26 0a    	bclr	0x26,x, #0x0a
    d8e3:	bd ea 65    	jsr	0xea65
    d8e6:	ce 10 23    	ldx	#0x1023
    d8e9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd8e9
    d8ed:	bd e9 3b    	jsr	0xe93b
    d8f0:	ce 01 20    	ldx	#0x120
    d8f3:	18 ce d9 4c 	ldy	#0xd94c
    d8f7:	c6 20       	ldab	#0x20
    d8f9:	18 a6 00    	ldaa	0x0,y
    d8fc:	a7 00       	staa	0x0,x
    d8fe:	08          	inx
    d8ff:	18 08       	iny
    d901:	5a          	decb
    d902:	26 f5       	bne	0x0xd8f9
    d904:	ce d9 6c    	ldx	#0xd96c
    d907:	18 ce 01 30 	ldy	#0x130
    d90b:	13 21 40 07 	brclr	*0x21, #0x40, 0x0xd916
    d90f:	c6 01       	ldab	#0x1
    d911:	bd ea 8d    	jsr	0xea8d
    d914:	20 04       	bra	0x0xd91a
    d916:	5f          	clrb
    d917:	bd ea 8d    	jsr	0xea8d
    d91a:	18 ce 01 34 	ldy	#0x134
    d91e:	ce d9 72    	ldx	#0xd972
    d921:	5f          	clrb
    d922:	bd ea 8d    	jsr	0xea8d
    d925:	20 00       	bra	0x0xd927
    d927:	ce d9 78    	ldx	#0xd978
    d92a:	18 ce 01 38 	ldy	#0x138
    d92e:	12 21 01 06 	brset	*0x21, #0x01, 0x0xd938
    d932:	5f          	clrb
    d933:	bd ea 8d    	jsr	0xea8d
    d936:	20 05       	bra	0x0xd93d
    d938:	c6 01       	ldab	#0x1
    d93a:	bd ea 8d    	jsr	0xea8d
    d93d:	ce d9 7e    	ldx	#0xd97e
    d940:	d6 24       	ldab	*0x24
    d942:	18 ce 01 3c 	ldy	#0x13c
    d946:	bd ea 8d    	jsr	0xea8d
    d949:	7e e8 78    	jmp	0xe878
    d94c:	20 4f       	bra	0x0xd99d
    d94e:	4e          	.byte	0x4e
    d94f:	20 54       	bra	0x0xd9a5
    d951:	59          	rolb
    d952:	50          	negb
    d953:	20 4d       	bra	0x0xd9a2
    d955:	44          	lsra
    d956:	45          	.byte	0x45
    d957:	20 44       	bra	0x0xd99d
    d959:	45          	.byte	0x45
    d95a:	53          	comb
    d95b:	20 20       	bra	0x0xd97d
    d95d:	20 20       	bra	0x0xd97f
    d95f:	20 20       	bra	0x0xd981
    d961:	20 20       	bra	0x0xd983
    d963:	20 20       	bra	0x0xd985
    d965:	20 20       	bra	0x0xd987
    d967:	20 20       	bra	0x0xd989
    d969:	20 20       	bra	0x0xd98b
    d96b:	20 4f       	bra	0x0xd9bc
    d96d:	46          	rora
    d96e:	46          	rora
    d96f:	20 4f       	bra	0x0xd9c0
    d971:	4e          	.byte	0x4e
    d972:	45          	.byte	0x45
    d973:	58          	aslb
    d974:	50          	negb
    d975:	4c          	inca
    d976:	49          	rola
    d977:	4e          	.byte	0x4e
    d978:	52          	.byte	0x52
    d979:	45          	.byte	0x45
    d97a:	47          	asra
    d97b:	4c          	inca
    d97c:	45          	.byte	0x45
    d97d:	47          	asra
    d97e:	4f          	clra
    d97f:	26 46       	bne	0x0xd9c7
    d981:	4f          	clra
    d982:	53          	comb
    d983:	31          	ins
    d984:	4f          	clra
    d985:	53          	comb
    d986:	32          	pula
    d987:	31          	ins
    d988:	26 32       	bne	0x0xd9bc
    d98a:	31          	ins
    d98b:	26 46       	bne	0x0xd9d3
    d98d:	32          	pula
    d98e:	26 46       	bne	0x0xd9d6
    d990:	46          	rora
    d991:	49          	rola
    d992:	4c          	inca
    d993:	7d 01 1d    	tst	0x11d
    d996:	26 0a       	bne	0x0xd9a2
    d998:	bd ea 65    	jsr	0xea65
    d99b:	ce 10 23    	ldx	#0x1023
    d99e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd99e
    d9a2:	bd e9 3b    	jsr	0xe93b
    d9a5:	ce 01 20    	ldx	#0x120
    d9a8:	18 ce d9 e2 	ldy	#0xd9e2
    d9ac:	c6 20       	ldab	#0x20
    d9ae:	18 a6 00    	ldaa	0x0,y
    d9b1:	a7 00       	staa	0x0,x
    d9b3:	08          	inx
    d9b4:	18 08       	iny
    d9b6:	5a          	decb
    d9b7:	26 f5       	bne	0x0xd9ae
    d9b9:	ce da 02    	ldx	#0xda02
    d9bc:	d6 6a       	ldab	*0x6a
    d9be:	c4 0f       	andb	#0xf
    d9c0:	18 ce 01 30 	ldy	#0x130
    d9c4:	bd ea a1    	jsr	0xeaa1
    d9c7:	ce da 16    	ldx	#0xda16
    d9ca:	d6 95       	ldab	*0x95
    d9cc:	18 ce 01 36 	ldy	#0x136
    d9d0:	bd ea a1    	jsr	0xeaa1
    d9d3:	ce da 1e    	ldx	#0xda1e
    d9d6:	d6 69       	ldab	*0x69
    d9d8:	18 ce 01 3b 	ldy	#0x13b
    d9dc:	bd ea a1    	jsr	0xeaa1
    d9df:	7e e8 78    	jmp	0xe878
    d9e2:	20 55       	bra	0x0xda39
    d9e4:	4e          	.byte	0x4e
    d9e5:	49          	rola
    d9e6:	20 56       	bra	0x0xda3e
    d9e8:	4d          	tsta
    d9e9:	4f          	clra
    d9ea:	44          	lsra
    d9eb:	45          	.byte	0x45
    d9ec:	20 50       	bra	0x0xda3e
    d9ee:	52          	.byte	0x52
    d9ef:	49          	rola
    d9f0:	4f          	clra
    d9f1:	52          	.byte	0x52
    d9f2:	20 20       	bra	0x0xda14
    d9f4:	20 20       	bra	0x0xda16
    d9f6:	20 20       	bra	0x0xda18
    d9f8:	20 20       	bra	0x0xda1a
    d9fa:	20 20       	bra	0x0xda1c
    d9fc:	20 20       	bra	0x0xda1e
    d9fe:	20 20       	bra	0x0xda20
    da00:	20 20       	bra	0x0xda22
    da02:	20 4f       	bra	0x0xda53
    da04:	4e          	.byte	0x4e
    da05:	45          	.byte	0x45
    da06:	20 54       	bra	0x0xda5c
    da08:	57          	asrb
    da09:	4f          	clra
    da0a:	46          	rora
    da0b:	4f          	clra
    da0c:	55          	.byte	0x55
    da0d:	52          	.byte	0x52
    da0e:	20 53       	bra	0x0xda63
    da10:	49          	rola
    da11:	58          	aslb
    da12:	45          	.byte	0x45
    da13:	47          	asra
    da14:	48          	asla
    da15:	54          	lsrb
    da16:	43          	coma
    da17:	59          	rolb
    da18:	43          	coma
    da19:	4c          	inca
    da1a:	4e          	.byte	0x4e
    da1b:	4f          	clra
    da1c:	54          	lsrb
    da1d:	45          	.byte	0x45
    da1e:	4c          	inca
    da1f:	41          	.byte	0x41
    da20:	53          	comb
    da21:	54          	lsrb
    da22:	20 4c       	bra	0x0xda70
    da24:	4f          	clra
    da25:	57          	asrb
    da26:	bd e9 3b    	jsr	0xe93b
    da29:	ce 01 20    	ldx	#0x120
    da2c:	18 ce da 58 	ldy	#0xda58
    da30:	c6 20       	ldab	#0x20
    da32:	18 a6 00    	ldaa	0x0,y
    da35:	a7 00       	staa	0x0,x
    da37:	08          	inx
    da38:	18 08       	iny
    da3a:	5a          	decb
    da3b:	26 f5       	bne	0x0xda32
    da3d:	ce da 78    	ldx	#0xda78
    da40:	d6 6b       	ldab	*0x6b
    da42:	18 ce 01 30 	ldy	#0x130
    da46:	bd ea a1    	jsr	0xeaa1
    da49:	ce da 84    	ldx	#0xda84
    da4c:	18 ce 01 36 	ldy	#0x136
    da50:	d6 68       	ldab	*0x68
    da52:	bd ea a1    	jsr	0xeaa1
    da55:	7e e8 78    	jmp	0xe878
    da58:	4f          	clra
    da59:	43          	coma
    da5a:	54          	lsrb
    da5b:	41          	.byte	0x41
    da5c:	56          	rorb
    da5d:	20 4d       	bra	0x0xdaac
    da5f:	54          	lsrb
    da60:	52          	.byte	0x52
    da61:	47          	asra
    da62:	20 20       	bra	0x0xda84
    da64:	20 20       	bra	0x0xda86
    da66:	20 20       	bra	0x0xda88
    da68:	20 20       	bra	0x0xda8a
    da6a:	20 20       	bra	0x0xda8c
    da6c:	20 20       	bra	0x0xda8e
    da6e:	20 20       	bra	0x0xda90
    da70:	20 20       	bra	0x0xda92
    da72:	20 20       	bra	0x0xda94
    da74:	20 20       	bra	0x0xda96
    da76:	20 20       	bra	0x0xda98
    da78:	20 4c       	bra	0x0xdac6
    da7a:	4f          	clra
    da7b:	57          	asrb
    da7c:	20 4d       	bra	0x0xdacb
    da7e:	49          	rola
    da7f:	44          	lsra
    da80:	48          	asla
    da81:	49          	rola
    da82:	47          	asra
    da83:	48          	asla
    da84:	20 4f       	bra	0x0xdad5
    da86:	46          	rora
    da87:	46          	rora
    da88:	45          	.byte	0x45
    da89:	4e          	.byte	0x4e
    da8a:	56          	rorb
    da8b:	31          	ins
    da8c:	45          	.byte	0x45
    da8d:	4e          	.byte	0x4e
    da8e:	56          	rorb
    da8f:	32          	pula
    da90:	45          	.byte	0x45
    da91:	4e          	.byte	0x4e
    da92:	56          	rorb
    da93:	33          	pulb
    da94:	45          	.byte	0x45
    da95:	31          	ins
    da96:	26 32       	bne	0x0xdaca
    da98:	45          	.byte	0x45
    da99:	31          	ins
    da9a:	26 33       	bne	0x0xdacf
    da9c:	45          	.byte	0x45
    da9d:	32          	pula
    da9e:	26 33       	bne	0x0xdad3
    daa0:	20 41       	bra	0x0xdae3
    daa2:	4c          	inca
    daa3:	4c          	inca
    daa4:	7d 01 1d    	tst	0x11d
    daa7:	26 0a       	bne	0x0xdab3
    daa9:	bd ea 65    	jsr	0xea65
    daac:	ce 10 23    	ldx	#0x1023
    daaf:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdaaf
    dab3:	bd e9 3b    	jsr	0xe93b
    dab6:	ce 01 20    	ldx	#0x120
    dab9:	18 ce db dd 	ldy	#0xdbdd
    dabd:	c6 20       	ldab	#0x20
    dabf:	18 a6 00    	ldaa	0x0,y
    dac2:	a7 00       	staa	0x0,x
    dac4:	08          	inx
    dac5:	18 08       	iny
    dac7:	5a          	decb
    dac8:	26 f5       	bne	0x0xdabf
    daca:	96 f6       	ldaa	*0xf6
    dacc:	48          	asla
    dacd:	24 2b       	bcc	0x0xdafa
    dacf:	ce de d3    	ldx	#0xded3
    dad2:	d6 8a       	ldab	*0x8a
    dad4:	18 ce 01 26 	ldy	#0x126
    dad8:	bd ea a1    	jsr	0xeaa1
    dadb:	ce de d3    	ldx	#0xded3
    dade:	d6 8b       	ldab	*0x8b
    dae0:	18 08       	iny
    dae2:	18 08       	iny
    dae4:	bd ea a1    	jsr	0xeaa1
    dae7:	ce 01 37    	ldx	#0x137
    daea:	96 8c       	ldaa	*0x8c
    daec:	bd ea 2a    	jsr	0xea2a
    daef:	ce 01 3c    	ldx	#0x13c
    daf2:	96 8d       	ldaa	*0x8d
    daf4:	bd ea 2a    	jsr	0xea2a
    daf7:	7e e8 78    	jmp	0xe878
    dafa:	48          	asla
    dafb:	24 2b       	bcc	0x0xdb28
    dafd:	ce de d3    	ldx	#0xded3
    db00:	d6 86       	ldab	*0x86
    db02:	18 ce 01 26 	ldy	#0x126
    db06:	bd ea a1    	jsr	0xeaa1
    db09:	ce de d3    	ldx	#0xded3
    db0c:	d6 87       	ldab	*0x87
    db0e:	18 08       	iny
    db10:	18 08       	iny
    db12:	bd ea a1    	jsr	0xeaa1
    db15:	ce 01 37    	ldx	#0x137
    db18:	96 88       	ldaa	*0x88
    db1a:	bd e9 ee    	jsr	0xe9ee
    db1d:	08          	inx
    db1e:	08          	inx
    db1f:	08          	inx
    db20:	96 89       	ldaa	*0x89
    db22:	bd e9 ee    	jsr	0xe9ee
    db25:	7e e8 78    	jmp	0xe878
    db28:	48          	asla
    db29:	24 2b       	bcc	0x0xdb56
    db2b:	ce de d3    	ldx	#0xded3
    db2e:	d6 82       	ldab	*0x82
    db30:	18 ce 01 26 	ldy	#0x126
    db34:	bd ea a1    	jsr	0xeaa1
    db37:	ce de d3    	ldx	#0xded3
    db3a:	d6 83       	ldab	*0x83
    db3c:	18 08       	iny
    db3e:	18 08       	iny
    db40:	bd ea a1    	jsr	0xeaa1
    db43:	ce 01 37    	ldx	#0x137
    db46:	96 84       	ldaa	*0x84
    db48:	bd e9 ee    	jsr	0xe9ee
    db4b:	08          	inx
    db4c:	08          	inx
    db4d:	08          	inx
    db4e:	96 85       	ldaa	*0x85
    db50:	bd e9 ee    	jsr	0xe9ee
    db53:	7e e8 78    	jmp	0xe878
    db56:	48          	asla
    db57:	24 2b       	bcc	0x0xdb84
    db59:	ce de d3    	ldx	#0xded3
    db5c:	d6 7e       	ldab	*0x7e
    db5e:	18 ce 01 26 	ldy	#0x126
    db62:	bd ea a1    	jsr	0xeaa1
    db65:	ce de d3    	ldx	#0xded3
    db68:	d6 7f       	ldab	*0x7f
    db6a:	18 08       	iny
    db6c:	18 08       	iny
    db6e:	bd ea a1    	jsr	0xeaa1
    db71:	ce 01 37    	ldx	#0x137
    db74:	96 80       	ldaa	*0x80
    db76:	bd e9 ee    	jsr	0xe9ee
    db79:	08          	inx
    db7a:	08          	inx
    db7b:	08          	inx
    db7c:	96 81       	ldaa	*0x81
    db7e:	bd e9 ee    	jsr	0xe9ee
    db81:	7e e8 78    	jmp	0xe878
    db84:	48          	asla
    db85:	24 2b       	bcc	0x0xdbb2
    db87:	ce de d3    	ldx	#0xded3
    db8a:	d6 7a       	ldab	*0x7a
    db8c:	18 ce 01 26 	ldy	#0x126
    db90:	bd ea a1    	jsr	0xeaa1
    db93:	ce de d3    	ldx	#0xded3
    db96:	d6 7b       	ldab	*0x7b
    db98:	18 08       	iny
    db9a:	18 08       	iny
    db9c:	bd ea a1    	jsr	0xeaa1
    db9f:	ce 01 37    	ldx	#0x137
    dba2:	96 7c       	ldaa	*0x7c
    dba4:	bd e9 ee    	jsr	0xe9ee
    dba7:	08          	inx
    dba8:	08          	inx
    dba9:	08          	inx
    dbaa:	96 7d       	ldaa	*0x7d
    dbac:	bd e9 ee    	jsr	0xe9ee
    dbaf:	7e e8 78    	jmp	0xe878
    dbb2:	ce de d3    	ldx	#0xded3
    dbb5:	d6 76       	ldab	*0x76
    dbb7:	18 ce 01 26 	ldy	#0x126
    dbbb:	bd ea a1    	jsr	0xeaa1
    dbbe:	ce de d3    	ldx	#0xded3
    dbc1:	d6 77       	ldab	*0x77
    dbc3:	18 08       	iny
    dbc5:	18 08       	iny
    dbc7:	bd ea a1    	jsr	0xeaa1
    dbca:	ce 01 37    	ldx	#0x137
    dbcd:	96 78       	ldaa	*0x78
    dbcf:	bd e9 ee    	jsr	0xe9ee
    dbd2:	08          	inx
    dbd3:	08          	inx
    dbd4:	08          	inx
    dbd5:	96 79       	ldaa	*0x79
    dbd7:	bd e9 ee    	jsr	0xe9ee
    dbda:	7e e8 78    	jmp	0xe878
    dbdd:	20 44       	bra	0x0xdc23
    dbdf:	45          	.byte	0x45
    dbe0:	53          	comb
    dbe1:	54          	lsrb
    dbe2:	20 20       	bra	0x0xdc04
    dbe4:	20 20       	bra	0x0xdc06
    dbe6:	20 20       	bra	0x0xdc08
    dbe8:	20 20       	bra	0x0xdc0a
    dbea:	20 20       	bra	0x0xdc0c
    dbec:	20 20       	bra	0x0xdc0e
    dbee:	41          	.byte	0x41
    dbef:	4d          	tsta
    dbf0:	4e          	.byte	0x4e
    dbf1:	54          	lsrb
    dbf2:	20 20       	bra	0x0xdc14
    dbf4:	20 20       	bra	0x0xdc16
    dbf6:	20 20       	bra	0x0xdc18
    dbf8:	20 20       	bra	0x0xdc1a
    dbfa:	20 20       	bra	0x0xdc1c
    dbfc:	20 41       	bra	0x0xdc3f
    dbfe:	54          	lsrb
    dbff:	54          	lsrb
    dc00:	31          	ins
    dc01:	44          	lsra
    dc02:	45          	.byte	0x45
    dc03:	43          	coma
    dc04:	31          	ins
    dc05:	53          	comb
    dc06:	55          	.byte	0x55
    dc07:	53          	comb
    dc08:	31          	ins
    dc09:	52          	.byte	0x52
    dc0a:	45          	.byte	0x45
    dc0b:	4c          	inca
    dc0c:	31          	ins
    dc0d:	41          	.byte	0x41
    dc0e:	4d          	tsta
    dc0f:	54          	lsrb
    dc10:	31          	ins
    dc11:	41          	.byte	0x41
    dc12:	54          	lsrb
    dc13:	54          	lsrb
    dc14:	32          	pula
    dc15:	44          	lsra
    dc16:	45          	.byte	0x45
    dc17:	43          	coma
    dc18:	32          	pula
    dc19:	53          	comb
    dc1a:	55          	.byte	0x55
    dc1b:	53          	comb
    dc1c:	32          	pula
    dc1d:	52          	.byte	0x52
    dc1e:	45          	.byte	0x45
    dc1f:	4c          	inca
    dc20:	32          	pula
    dc21:	41          	.byte	0x41
    dc22:	4d          	tsta
    dc23:	54          	lsrb
    dc24:	32          	pula
    dc25:	41          	.byte	0x41
    dc26:	54          	lsrb
    dc27:	54          	lsrb
    dc28:	33          	pulb
    dc29:	44          	lsra
    dc2a:	45          	.byte	0x45
    dc2b:	43          	coma
    dc2c:	33          	pulb
    dc2d:	53          	comb
    dc2e:	55          	.byte	0x55
    dc2f:	53          	comb
    dc30:	33          	pulb
    dc31:	52          	.byte	0x52
    dc32:	45          	.byte	0x45
    dc33:	4c          	inca
    dc34:	33          	pulb
    dc35:	41          	.byte	0x41
    dc36:	4d          	tsta
    dc37:	54          	lsrb
    dc38:	33          	pulb
    dc39:	44          	lsra
    dc3a:	45          	.byte	0x45
    dc3b:	4c          	inca
    dc3c:	31          	ins
    dc3d:	44          	lsra
    dc3e:	45          	.byte	0x45
    dc3f:	4c          	inca
    dc40:	33          	pulb
    dc41:	7d 01 1d    	tst	0x11d
    dc44:	26 0a       	bne	0x0xdc50
    dc46:	bd ea 65    	jsr	0xea65
    dc49:	ce 10 23    	ldx	#0x1023
    dc4c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdc4c
    dc50:	bd e9 3b    	jsr	0xe93b
    dc53:	ce 01 20    	ldx	#0x120
    dc56:	18 ce dc 82 	ldy	#0xdc82
    dc5a:	c6 20       	ldab	#0x20
    dc5c:	18 a6 00    	ldaa	0x0,y
    dc5f:	a7 00       	staa	0x0,x
    dc61:	08          	inx
    dc62:	18 08       	iny
    dc64:	5a          	decb
    dc65:	26 f5       	bne	0x0xdc5c
    dc67:	ce dc a2    	ldx	#0xdca2
    dc6a:	d6 8e       	ldab	*0x8e
    dc6c:	18 ce 01 36 	ldy	#0x136
    dc70:	bd ea a1    	jsr	0xeaa1
    dc73:	ce dc ae    	ldx	#0xdcae
    dc76:	d6 8f       	ldab	*0x8f
    dc78:	18 08       	iny
    dc7a:	18 08       	iny
    dc7c:	bd ea a1    	jsr	0xeaa1
    dc7f:	7e e8 78    	jmp	0xe878
    dc82:	43          	coma
    dc83:	4e          	.byte	0x4e
    dc84:	54          	lsrb
    dc85:	52          	.byte	0x52
    dc86:	4c          	inca
    dc87:	20 43       	bra	0x0xdccc
    dc89:	4f          	clra
    dc8a:	4e          	.byte	0x4e
    dc8b:	31          	ins
    dc8c:	20 43       	bra	0x0xdcd1
    dc8e:	4f          	clra
    dc8f:	4e          	.byte	0x4e
    dc90:	32          	pula
    dc91:	20 41       	bra	0x0xdcd4
    dc93:	53          	comb
    dc94:	53          	comb
    dc95:	47          	asra
    dc96:	4e          	.byte	0x4e
    dc97:	20 20       	bra	0x0xdcb9
    dc99:	20 20       	bra	0x0xdcbb
    dc9b:	20 20       	bra	0x0xdcbd
    dc9d:	20 20       	bra	0x0xdcbf
    dc9f:	20 20       	bra	0x0xdcc1
    dca1:	20 42       	bra	0x0xdce5
    dca3:	52          	.byte	0x52
    dca4:	54          	lsrb
    dca5:	48          	asla
    dca6:	4d          	tsta
    dca7:	4f          	clra
    dca8:	44          	lsra
    dca9:	57          	asrb
    dcaa:	54          	lsrb
    dcab:	55          	.byte	0x55
    dcac:	43          	coma
    dcad:	48          	asla
    dcae:	20 44       	bra	0x0xdcf4
    dcb0:	59          	rolb
    dcb1:	4e          	.byte	0x4e
    dcb2:	54          	lsrb
    dcb3:	52          	.byte	0x52
    dcb4:	41          	.byte	0x41
    dcb5:	4b          	.byte	0x4b
    dcb6:	7d 01 1d    	tst	0x11d
    dcb9:	26 0a       	bne	0x0xdcc5
    dcbb:	bd ea 65    	jsr	0xea65
    dcbe:	ce 10 23    	ldx	#0x1023
    dcc1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdcc1
    dcc5:	bd e9 3b    	jsr	0xe93b
    dcc8:	ce 01 20    	ldx	#0x120
    dccb:	18 ce dd 04 	ldy	#0xdd04
    dccf:	c6 20       	ldab	#0x20
    dcd1:	18 a6 00    	ldaa	0x0,y
    dcd4:	a7 00       	staa	0x0,x
    dcd6:	08          	inx
    dcd7:	18 08       	iny
    dcd9:	5a          	decb
    dcda:	26 f5       	bne	0x0xdcd1
    dcdc:	f6 50 00    	ldab	0x5000
    dcdf:	c1 0a       	cmpb	#0xa
    dce1:	25 04       	bcs	0x0xdce7
    dce3:	5f          	clrb
    dce4:	f7 50 00    	stab	0x5000
    dce7:	86 09       	ldaa	#0x9
    dce9:	3d          	mul
    dcea:	1b          	aba
    dceb:	16          	tab
    dcec:	ce dd 24    	ldx	#0xdd24
    dcef:	3a          	abx
    dcf0:	18 ce 01 31 	ldy	#0x131
    dcf4:	c6 09       	ldab	#0x9
    dcf6:	a6 00       	ldaa	0x0,x
    dcf8:	18 a7 00    	staa	0x0,y
    dcfb:	08          	inx
    dcfc:	18 08       	iny
    dcfe:	5a          	decb
    dcff:	26 f5       	bne	0x0xdcf6
    dd01:	7e e8 78    	jmp	0xe878
    dd04:	20 4d       	bra	0x0xdd53
    dd06:	55          	.byte	0x55
    dd07:	4c          	inca
    dd08:	54          	lsrb
    dd09:	49          	rola
    dd0a:	20 54       	bra	0x0xdd60
    dd0c:	59          	rolb
    dd0d:	50          	negb
    dd0e:	45          	.byte	0x45
    dd0f:	3a          	abx
    dd10:	20 20       	bra	0x0xdd32
    dd12:	20 20       	bra	0x0xdd34
    dd14:	20 20       	bra	0x0xdd36
    dd16:	20 20       	bra	0x0xdd38
    dd18:	20 20       	bra	0x0xdd3a
    dd1a:	20 20       	bra	0x0xdd3c
    dd1c:	20 20       	bra	0x0xdd3e
    dd1e:	20 20       	bra	0x0xdd40
    dd20:	20 20       	bra	0x0xdd42
    dd22:	20 20       	bra	0x0xdd44
    dd24:	50          	negb
    dd25:	52          	.byte	0x52
    dd26:	45          	.byte	0x45
    dd27:	50          	negb
    dd28:	41          	.byte	0x41
    dd29:	52          	.byte	0x52
    dd2a:	45          	.byte	0x45
    dd2b:	44          	lsra
    dd2c:	20 53       	bra	0x0xdd81
    dd2e:	50          	negb
    dd2f:	4c          	inca
    dd30:	49          	rola
    dd31:	54          	lsrb
    dd32:	20 31       	bra	0x0xdd65
    dd34:	2b 37       	bmi	0x0xdd6d
    dd36:	53          	comb
    dd37:	50          	negb
    dd38:	4c          	inca
    dd39:	49          	rola
    dd3a:	54          	lsrb
    dd3b:	20 32       	bra	0x0xdd6f
    dd3d:	2b 36       	bmi	0x0xdd75
    dd3f:	53          	comb
    dd40:	50          	negb
    dd41:	4c          	inca
    dd42:	49          	rola
    dd43:	54          	lsrb
    dd44:	20 33       	bra	0x0xdd79
    dd46:	2b 35       	bmi	0x0xdd7d
    dd48:	53          	comb
    dd49:	50          	negb
    dd4a:	4c          	inca
    dd4b:	49          	rola
    dd4c:	54          	lsrb
    dd4d:	20 34       	bra	0x0xdd83
    dd4f:	2b 34       	bmi	0x0xdd85
    dd51:	4c          	inca
    dd52:	41          	.byte	0x41
    dd53:	59          	rolb
    dd54:	45          	.byte	0x45
    dd55:	52          	.byte	0x52
    dd56:	20 34       	bra	0x0xdd8c
    dd58:	2b 34       	bmi	0x0xdd8e
    dd5a:	4d          	tsta
    dd5b:	55          	.byte	0x55
    dd5c:	4c          	inca
    dd5d:	54          	lsrb
    dd5e:	49          	rola
    dd5f:	43          	coma
    dd60:	48          	asla
    dd61:	41          	.byte	0x41
    dd62:	4e          	.byte	0x4e
    dd63:	01          	nop
    dd64:	02          	idiv
    dd65:	04          	lsrd
    dd66:	08          	inx
    dd67:	10          	sba
    dd68:	20 40       	bra	0x0xddaa
    dd6a:	80 01       	suba	#0x1
    dd6c:	fe 00 00    	ldx	0x0
    dd6f:	00          	bgnd
    dd70:	00          	bgnd
    dd71:	00          	bgnd
    dd72:	00          	bgnd
    dd73:	03          	fdiv
    dd74:	fc 00 00    	ldd	0x0
    dd77:	00          	bgnd
    dd78:	00          	bgnd
    dd79:	00          	bgnd
    dd7a:	00          	bgnd
    dd7b:	07          	tpa
    dd7c:	f8 00 00    	eorb	0x0
    dd7f:	00          	bgnd
    dd80:	00          	bgnd
    dd81:	00          	bgnd
    dd82:	00          	bgnd
    dd83:	0f          	sei
    dd84:	f0 00 00    	subb	0x0
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
    dd93:	03          	fdiv
    dd94:	fc 00 00    	ldd	0x0
    dd97:	00          	bgnd
    dd98:	00          	bgnd
    dd99:	00          	bgnd
    dd9a:	00          	bgnd
    dd9b:	01          	nop
    dd9c:	02          	idiv
    dd9d:	fc 00 00    	ldd	0x0
    dda0:	00          	bgnd
    dda1:	00          	bgnd
    dda2:	00          	bgnd
    dda3:	01          	nop
    dda4:	02          	idiv
    dda5:	04          	lsrd
    dda6:	08          	inx
    dda7:	10          	sba
    dda8:	20 40       	bra	0x0xddea
    ddaa:	80 00       	suba	#0x0
    ddac:	01          	nop
    ddad:	03          	fdiv
    ddae:	07          	tpa
    ddaf:	0f          	sei
    ddb0:	1f 3f 7f ff 	brclr	0x3f,x, #0x7f, 0x0xddb3
    ddb4:	01          	nop
    ddb5:	02          	idiv
    ddb6:	00          	bgnd
    ddb7:	00          	bgnd
    ddb8:	00          	bgnd
    ddb9:	00          	bgnd
    ddba:	00          	bgnd
    ddbb:	00          	bgnd
    ddbc:	03          	fdiv
    ddbd:	0c          	clc
    ddbe:	00          	bgnd
    ddbf:	00          	bgnd
    ddc0:	00          	bgnd
    ddc1:	00          	bgnd
    ddc2:	00          	bgnd
    ddc3:	00          	bgnd
    ddc4:	07          	tpa
    ddc5:	38          	pulx
    ddc6:	00          	bgnd
    ddc7:	00          	bgnd
    ddc8:	00          	bgnd
    ddc9:	00          	bgnd
    ddca:	00          	bgnd
    ddcb:	00          	bgnd
    ddcc:	0f          	sei
    ddcd:	f0 00 00    	subb	0x0
    ddd0:	00          	bgnd
    ddd1:	00          	bgnd
    ddd2:	00          	bgnd
    ddd3:	00          	bgnd
    ddd4:	bd e9 3b    	jsr	0xe93b
    ddd7:	ce 01 20    	ldx	#0x120
    ddda:	18 ce dd f7 	ldy	#0xddf7
    ddde:	c6 20       	ldab	#0x20
    dde0:	18 a6 00    	ldaa	0x0,y
    dde3:	a7 00       	staa	0x0,x
    dde5:	08          	inx
    dde6:	18 08       	iny
    dde8:	5a          	decb
    dde9:	26 f5       	bne	0x0xdde0
    ddeb:	b6 50 21    	ldaa	0x5021
    ddee:	ce 01 3c    	ldx	#0x13c
    ddf1:	bd e9 ee    	jsr	0xe9ee
    ddf4:	7e e8 78    	jmp	0xe878
    ddf7:	53          	comb
    ddf8:	50          	negb
    ddf9:	4c          	inca
    ddfa:	49          	rola
    ddfb:	54          	lsrb
    ddfc:	20 50       	bra	0x0xde4e
    ddfe:	4f          	clra
    ddff:	49          	rola
    de00:	4e          	.byte	0x4e
    de01:	54          	lsrb
    de02:	3a          	abx
    de03:	20 20       	bra	0x0xde25
    de05:	20 20       	bra	0x0xde27
    de07:	4d          	tsta
    de08:	49          	rola
    de09:	44          	lsra
    de0a:	49          	rola
    de0b:	20 4e       	bra	0x0xde5b
    de0d:	4f          	clra
    de0e:	54          	lsrb
    de0f:	45          	.byte	0x45
    de10:	20 23       	bra	0x0xde35
    de12:	20 20       	bra	0x0xde34
    de14:	20 20       	bra	0x0xde36
    de16:	20 7d       	bra	0x0xde95
    de18:	01          	nop
    de19:	1d 26 0a    	bclr	0x26,x, #0x0a
    de1c:	bd ea 65    	jsr	0xea65
    de1f:	ce 10 23    	ldx	#0x1023
    de22:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xde22
    de26:	bd e9 3b    	jsr	0xe93b
    de29:	ce 01 20    	ldx	#0x120
    de2c:	18 ce de 52 	ldy	#0xde52
    de30:	c6 20       	ldab	#0x20
    de32:	18 a6 00    	ldaa	0x0,y
    de35:	a7 00       	staa	0x0,x
    de37:	08          	inx
    de38:	18 08       	iny
    de3a:	5a          	decb
    de3b:	26 f5       	bne	0x0xde32
    de3d:	ce 01 34    	ldx	#0x134
    de40:	b6 01 68    	ldaa	0x168
    de43:	bd e9 ee    	jsr	0xe9ee
    de46:	ce 01 3c    	ldx	#0x13c
    de49:	b6 01 69    	ldaa	0x169
    de4c:	bd e9 ee    	jsr	0xe9ee
    de4f:	7e e8 78    	jmp	0xe878
    de52:	20 44       	bra	0x0xde98
    de54:	59          	rolb
    de55:	4e          	.byte	0x4e
    de56:	41          	.byte	0x41
    de57:	4d          	tsta
    de58:	49          	rola
    de59:	43          	coma
    de5a:	53          	comb
    de5b:	20 52       	bra	0x0xdeaf
    de5d:	41          	.byte	0x41
    de5e:	4e          	.byte	0x4e
    de5f:	47          	asra
    de60:	45          	.byte	0x45
    de61:	20 20       	bra	0x0xde83
    de63:	4f          	clra
    de64:	4e          	.byte	0x4e
    de65:	20 20       	bra	0x0xde87
    de67:	20 20       	bra	0x0xde89
    de69:	20 4f       	bra	0x0xdeba
    de6b:	46          	rora
    de6c:	46          	rora
    de6d:	20 20       	bra	0x0xde8f
    de6f:	20 20       	bra	0x0xde91
    de71:	20 7d       	bra	0x0xdef0
    de73:	01          	nop
    de74:	1d 26 0a    	bclr	0x26,x, #0x0a
    de77:	bd ea 65    	jsr	0xea65
    de7a:	ce 10 23    	ldx	#0x1023
    de7d:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xde7d
    de81:	bd e9 3b    	jsr	0xe93b
    de84:	86 20       	ldaa	#0x20
    de86:	c6 20       	ldab	#0x20
    de88:	ce 01 20    	ldx	#0x120
    de8b:	a7 00       	staa	0x0,x
    de8d:	08          	inx
    de8e:	5a          	decb
    de8f:	26 fa       	bne	0x0xde8b
    de91:	ce de d3    	ldx	#0xded3
    de94:	d6 60       	ldab	*0x60
    de96:	18 ce 01 20 	ldy	#0x120
    de9a:	bd ea a1    	jsr	0xeaa1
    de9d:	ce de d3    	ldx	#0xded3
    dea0:	d6 61       	ldab	*0x61
    dea2:	18 08       	iny
    dea4:	18 08       	iny
    dea6:	18 08       	iny
    dea8:	bd ea a1    	jsr	0xeaa1
    deab:	ce de d3    	ldx	#0xded3
    deae:	d6 62       	ldab	*0x62
    deb0:	18 08       	iny
    deb2:	18 08       	iny
    deb4:	bd ea a1    	jsr	0xeaa1
    deb7:	ce 01 31    	ldx	#0x131
    deba:	96 63       	ldaa	*0x63
    debc:	bd e9 ee    	jsr	0xe9ee
    debf:	08          	inx
    dec0:	08          	inx
    dec1:	08          	inx
    dec2:	08          	inx
    dec3:	96 64       	ldaa	*0x64
    dec5:	bd e9 ee    	jsr	0xe9ee
    dec8:	08          	inx
    dec9:	08          	inx
    deca:	08          	inx
    decb:	96 65       	ldaa	*0x65
    decd:	bd e9 ee    	jsr	0xe9ee
    ded0:	7e e8 78    	jmp	0xe878
    ded3:	20 4f       	bra	0x0xdf24
    ded5:	46          	rora
    ded6:	46          	rora
    ded7:	46          	rora
    ded8:	52          	.byte	0x52
    ded9:	45          	.byte	0x45
    deda:	31          	ins
    dedb:	46          	rora
    dedc:	52          	.byte	0x52
    dedd:	45          	.byte	0x45
    dede:	32          	pula
    dedf:	31          	ins
    dee0:	26 32       	bne	0x0xdf14
    dee2:	46          	rora
    dee3:	4c          	inca
    dee4:	45          	.byte	0x45
    dee5:	56          	rorb
    dee6:	31          	ins
    dee7:	4c          	inca
    dee8:	45          	.byte	0x45
    dee9:	56          	rorb
    deea:	32          	pula
    deeb:	20 50       	bra	0x0xdf3d
    deed:	57          	asrb
    deee:	31          	ins
    deef:	20 50       	bra	0x0xdf41
    def1:	57          	asrb
    def2:	32          	pula
    def3:	31          	ins
    def4:	26 32       	bne	0x0xdf28
    def6:	50          	negb
    def7:	46          	rora
    def8:	49          	rola
    def9:	4c          	inca
    defa:	54          	lsrb
    defb:	52          	.byte	0x52
    defc:	45          	.byte	0x45
    defd:	53          	comb
    defe:	4f          	clra
    deff:	4c          	inca
    df00:	45          	.byte	0x45
    df01:	56          	rorb
    df02:	4e          	.byte	0x4e
    df03:	58          	aslb
    df04:	4d          	tsta
    df05:	4f          	clra
    df06:	44          	lsra
    df07:	20 45       	bra	0x0xdf4e
    df09:	41          	.byte	0x41
    df0a:	31          	ins
    df0b:	20 45       	bra	0x0xdf52
    df0d:	41          	.byte	0x41
    df0e:	33          	pulb
    df0f:	20 45       	bra	0x0xdf56
    df11:	58          	aslb
    df12:	54          	lsrb
    df13:	4c          	inca
    df14:	46          	rora
    df15:	31          	ins
    df16:	52          	.byte	0x52
    df17:	4c          	inca
    df18:	46          	rora
    df19:	32          	pula
    df1a:	52          	.byte	0x52
    df1b:	4c          	inca
    df1c:	46          	rora
    df1d:	31          	ins
    df1e:	44          	lsra
    df1f:	4c          	inca
    df20:	46          	rora
    df21:	32          	pula
    df22:	44          	lsra
    df23:	31          	ins
    df24:	26 32       	bne	0x0xdf58
    df26:	44          	lsra
    df27:	31          	ins
    df28:	26 32       	bne	0x0xdf5c
    df2a:	52          	.byte	0x52
    df2b:	50          	negb
    df2c:	41          	.byte	0x41
    df2d:	4e          	.byte	0x4e
    df2e:	52          	.byte	0x52
    df2f:	50          	negb
    df30:	41          	.byte	0x41
    df31:	4e          	.byte	0x4e
    df32:	44          	lsra
    df33:	20 50       	bra	0x0xdf85
    df35:	41          	.byte	0x41
    df36:	4e          	.byte	0x4e
    df37:	bd e9 3b    	jsr	0xe93b
    df3a:	ce 01 20    	ldx	#0x120
    df3d:	18 ce df 6a 	ldy	#0xdf6a
    df41:	c6 20       	ldab	#0x20
    df43:	18 a6 00    	ldaa	0x0,y
    df46:	a7 00       	staa	0x0,x
    df48:	08          	inx
    df49:	18 08       	iny
    df4b:	5a          	decb
    df4c:	26 f5       	bne	0x0xdf43
    df4e:	ce 01 31    	ldx	#0x131
    df51:	96 54       	ldaa	*0x54
    df53:	bd e9 ee    	jsr	0xe9ee
    df56:	08          	inx
    df57:	08          	inx
    df58:	08          	inx
    df59:	08          	inx
    df5a:	96 59       	ldaa	*0x59
    df5c:	bd e9 ee    	jsr	0xe9ee
    df5f:	08          	inx
    df60:	08          	inx
    df61:	08          	inx
    df62:	96 5e       	ldaa	*0x5e
    df64:	bd e9 ee    	jsr	0xe9ee
    df67:	7e e8 78    	jmp	0xe878
    df6a:	20 44       	bra	0x0xdfb0
    df6c:	4b          	.byte	0x4b
    df6d:	32          	pula
    df6e:	20 20       	bra	0x0xdf90
    df70:	20 44       	bra	0x0xdfb6
    df72:	4b          	.byte	0x4b
    df73:	32          	pula
    df74:	20 20       	bra	0x0xdf96
    df76:	44          	lsra
    df77:	4b          	.byte	0x4b
    df78:	32          	pula
    df79:	20 20       	bra	0x0xdf9b
    df7b:	20 20       	bra	0x0xdf9d
    df7d:	20 20       	bra	0x0xdf9f
    df7f:	20 20       	bra	0x0xdfa1
    df81:	20 20       	bra	0x0xdfa3
    df83:	20 20       	bra	0x0xdfa5
    df85:	20 20       	bra	0x0xdfa7
    df87:	20 20       	bra	0x0xdfa9
    df89:	20 bd       	bra	0x0xdf48
    df8b:	e9 3b       	adcb	0x3b,x
    df8d:	ce 01 20    	ldx	#0x120
    df90:	18 ce df bc 	ldy	#0xdfbc
    df94:	c6 20       	ldab	#0x20
    df96:	18 a6 00    	ldaa	0x0,y
    df99:	a7 00       	staa	0x0,x
    df9b:	08          	inx
    df9c:	18 08       	iny
    df9e:	5a          	decb
    df9f:	26 f5       	bne	0x0xdf96
    dfa1:	ce 01 31    	ldx	#0x131
    dfa4:	96 90       	ldaa	*0x90
    dfa6:	bd e9 ee    	jsr	0xe9ee
    dfa9:	ce 01 37    	ldx	#0x137
    dfac:	96 91       	ldaa	*0x91
    dfae:	bd e9 ee    	jsr	0xe9ee
    dfb1:	ce 01 3c    	ldx	#0x13c
    dfb4:	96 92       	ldaa	*0x92
    dfb6:	bd e9 ee    	jsr	0xe9ee
    dfb9:	7e e8 78    	jmp	0xe878
    dfbc:	44          	lsra
    dfbd:	59          	rolb
    dfbe:	4e          	.byte	0x4e
    dfbf:	31          	ins
    dfc0:	20 20       	bra	0x0xdfe2
    dfc2:	44          	lsra
    dfc3:	59          	rolb
    dfc4:	4e          	.byte	0x4e
    dfc5:	32          	pula
    dfc6:	20 44       	bra	0x0xe00c
    dfc8:	59          	rolb
    dfc9:	4e          	.byte	0x4e
    dfca:	33          	pulb
    dfcb:	20 20       	bra	0x0xdfed
    dfcd:	20 20       	bra	0x0xdfef
    dfcf:	20 20       	bra	0x0xdff1
    dfd1:	20 20       	bra	0x0xdff3
    dfd3:	20 20       	bra	0x0xdff5
    dfd5:	20 20       	bra	0x0xdff7
    dfd7:	20 20       	bra	0x0xdff9
    dfd9:	20 20       	bra	0x0xdffb
    dfdb:	20 bd       	bra	0x0xdf9a
    dfdd:	e9 3b       	adcb	0x3b,x
    dfdf:	ce 01 20    	ldx	#0x120
    dfe2:	18 ce e0 06 	ldy	#0xe006
    dfe6:	c6 20       	ldab	#0x20
    dfe8:	18 a6 00    	ldaa	0x0,y
    dfeb:	a7 00       	staa	0x0,x
    dfed:	08          	inx
    dfee:	18 08       	iny
    dff0:	5a          	decb
    dff1:	26 f5       	bne	0x0xdfe8
    dff3:	ce 01 31    	ldx	#0x131
    dff6:	96 50       	ldaa	*0x50
    dff8:	bd e9 ee    	jsr	0xe9ee
    dffb:	ce 01 37    	ldx	#0x137
    dffe:	96 98       	ldaa	*0x98
    e000:	bd e9 ee    	jsr	0xe9ee
    e003:	7e e8 78    	jmp	0xe878
    e006:	44          	lsra
    e007:	4c          	inca
    e008:	41          	.byte	0x41
    e009:	59          	rolb
    e00a:	31          	ins
    e00b:	20 44       	bra	0x0xe051
    e00d:	4c          	inca
    e00e:	41          	.byte	0x41
    e00f:	59          	rolb
    e010:	33          	pulb
    e011:	20 20       	bra	0x0xe033
    e013:	20 20       	bra	0x0xe035
    e015:	20 20       	bra	0x0xe037
    e017:	20 20       	bra	0x0xe039
    e019:	20 20       	bra	0x0xe03b
    e01b:	20 20       	bra	0x0xe03d
    e01d:	20 20       	bra	0x0xe03f
    e01f:	20 20       	bra	0x0xe041
    e021:	20 20       	bra	0x0xe043
    e023:	20 20       	bra	0x0xe045
    e025:	20 7d       	bra	0x0xe0a4
    e027:	01          	nop
    e028:	1d 26 0a    	bclr	0x26,x, #0x0a
    e02b:	bd ea 65    	jsr	0xea65
    e02e:	ce 10 23    	ldx	#0x1023
    e031:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe031
    e035:	bd e9 3b    	jsr	0xe93b
    e038:	ce 01 20    	ldx	#0x120
    e03b:	18 ce e0 69 	ldy	#0xe069
    e03f:	c6 20       	ldab	#0x20
    e041:	18 a6 00    	ldaa	0x0,y
    e044:	a7 00       	staa	0x0,x
    e046:	08          	inx
    e047:	18 08       	iny
    e049:	5a          	decb
    e04a:	26 f5       	bne	0x0xe041
    e04c:	ce e0 89    	ldx	#0xe089
    e04f:	d6 4b       	ldab	*0x4b
    e051:	18 ce 01 30 	ldy	#0x130
    e055:	bd ea a1    	jsr	0xeaa1
    e058:	ce e0 a5    	ldx	#0xe0a5
    e05b:	d6 66       	ldab	*0x66
    e05d:	18 08       	iny
    e05f:	18 08       	iny
    e061:	18 08       	iny
    e063:	bd ea a1    	jsr	0xeaa1
    e066:	7e e8 78    	jmp	0xe878
    e069:	54          	lsrb
    e06a:	59          	rolb
    e06b:	50          	negb
    e06c:	45          	.byte	0x45
    e06d:	20 20       	bra	0x0xe08f
    e06f:	49          	rola
    e070:	4e          	.byte	0x4e
    e071:	56          	rorb
    e072:	54          	lsrb
    e073:	20 20       	bra	0x0xe095
    e075:	20 20       	bra	0x0xe097
    e077:	20 20       	bra	0x0xe099
    e079:	20 20       	bra	0x0xe09b
    e07b:	20 20       	bra	0x0xe09d
    e07d:	20 20       	bra	0x0xe09f
    e07f:	20 20       	bra	0x0xe0a1
    e081:	20 20       	bra	0x0xe0a3
    e083:	20 20       	bra	0x0xe0a5
    e085:	20 20       	bra	0x0xe0a7
    e087:	20 20       	bra	0x0xe0a9
    e089:	4f          	clra
    e08a:	42          	.byte	0x42
    e08b:	4c          	inca
    e08c:	50          	negb
    e08d:	4f          	clra
    e08e:	42          	.byte	0x42
    e08f:	42          	.byte	0x42
    e090:	50          	negb
    e091:	4f          	clra
    e092:	42          	.byte	0x42
    e093:	48          	asla
    e094:	50          	negb
    e095:	4f          	clra
    e096:	42          	.byte	0x42
    e097:	42          	.byte	0x42
    e098:	52          	.byte	0x52
    e099:	4d          	tsta
    e09a:	49          	rola
    e09b:	4e          	.byte	0x4e
    e09c:	49          	rola
    e09d:	41          	.byte	0x41
    e09e:	55          	.byte	0x55
    e09f:	58          	aslb
    e0a0:	31          	ins
    e0a1:	41          	.byte	0x41
    e0a2:	55          	.byte	0x55
    e0a3:	58          	aslb
    e0a4:	32          	pula
    e0a5:	20 4f       	bra	0x0xe0f6
    e0a7:	46          	rora
    e0a8:	46          	rora
    e0a9:	45          	.byte	0x45
    e0aa:	4e          	.byte	0x4e
    e0ab:	56          	rorb
    e0ac:	31          	ins
    e0ad:	45          	.byte	0x45
    e0ae:	4e          	.byte	0x4e
    e0af:	56          	rorb
    e0b0:	33          	pulb
    e0b1:	45          	.byte	0x45
    e0b2:	31          	ins
    e0b3:	26 33       	bne	0x0xe0e8
    e0b5:	bd e9 3b    	jsr	0xe93b
    e0b8:	ce 01 20    	ldx	#0x120
    e0bb:	18 ce e0 ec 	ldy	#0xe0ec
    e0bf:	c6 20       	ldab	#0x20
    e0c1:	18 a6 00    	ldaa	0x0,y
    e0c4:	a7 00       	staa	0x0,x
    e0c6:	08          	inx
    e0c7:	18 08       	iny
    e0c9:	5a          	decb
    e0ca:	26 f5       	bne	0x0xe0c1
    e0cc:	ce 01 30    	ldx	#0x130
    e0cf:	96 6e       	ldaa	*0x6e
    e0d1:	bd e9 ee    	jsr	0xe9ee
    e0d4:	08          	inx
    e0d5:	08          	inx
    e0d6:	96 48       	ldaa	*0x48
    e0d8:	bd e9 ee    	jsr	0xe9ee
    e0db:	08          	inx
    e0dc:	08          	inx
    e0dd:	96 6f       	ldaa	*0x6f
    e0df:	bd e9 ee    	jsr	0xe9ee
    e0e2:	08          	inx
    e0e3:	08          	inx
    e0e4:	96 70       	ldaa	*0x70
    e0e6:	bd ea b5    	jsr	0xeab5
    e0e9:	7e e8 78    	jmp	0xe878
    e0ec:	20 49       	bra	0x0xe137
    e0ee:	4e          	.byte	0x4e
    e0ef:	20 4d       	bra	0x0xe13e
    e0f1:	49          	rola
    e0f2:	58          	aslb
    e0f3:	20 54       	bra	0x0xe149
    e0f5:	52          	.byte	0x52
    e0f6:	47          	asra
    e0f7:	20 57       	bra	0x0xe150
    e0f9:	49          	rola
    e0fa:	4e          	.byte	0x4e
    e0fb:	20 20       	bra	0x0xe11d
    e0fd:	20 20       	bra	0x0xe11f
    e0ff:	20 20       	bra	0x0xe121
    e101:	20 20       	bra	0x0xe123
    e103:	20 20       	bra	0x0xe125
    e105:	20 20       	bra	0x0xe127
    e107:	20 20       	bra	0x0xe129
    e109:	20 20       	bra	0x0xe12b
    e10b:	20 7d       	bra	0x0xe18a
    e10d:	01          	nop
    e10e:	1d 26 0a    	bclr	0x26,x, #0x0a
    e111:	bd ea 65    	jsr	0xea65
    e114:	ce 10 23    	ldx	#0x1023
    e117:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe117
    e11b:	bd e9 3b    	jsr	0xe93b
    e11e:	86 20       	ldaa	#0x20
    e120:	c6 20       	ldab	#0x20
    e122:	ce 01 20    	ldx	#0x120
    e125:	a7 00       	staa	0x0,x
    e127:	08          	inx
    e128:	5a          	decb
    e129:	26 fa       	bne	0x0xe125
    e12b:	ce e1 b0    	ldx	#0xe1b0
    e12e:	12 f4 02 3f 	brset	*0xf4, #0x02, 0x0xe171
    e132:	d6 2d       	ldab	*0x2d
    e134:	18 ce 01 20 	ldy	#0x120
    e138:	bd ea a1    	jsr	0xeaa1
    e13b:	ce e1 b0    	ldx	#0xe1b0
    e13e:	d6 2e       	ldab	*0x2e
    e140:	18 08       	iny
    e142:	18 08       	iny
    e144:	18 08       	iny
    e146:	bd ea a1    	jsr	0xeaa1
    e149:	ce e1 b0    	ldx	#0xe1b0
    e14c:	d6 2f       	ldab	*0x2f
    e14e:	18 08       	iny
    e150:	18 08       	iny
    e152:	bd ea a1    	jsr	0xeaa1
    e155:	ce 01 31    	ldx	#0x131
    e158:	96 2a       	ldaa	*0x2a
    e15a:	bd e9 ee    	jsr	0xe9ee
    e15d:	08          	inx
    e15e:	08          	inx
    e15f:	08          	inx
    e160:	08          	inx
    e161:	96 2b       	ldaa	*0x2b
    e163:	bd e9 ee    	jsr	0xe9ee
    e166:	08          	inx
    e167:	08          	inx
    e168:	08          	inx
    e169:	96 2c       	ldaa	*0x2c
    e16b:	bd e9 ee    	jsr	0xe9ee
    e16e:	7e e8 78    	jmp	0xe878
    e171:	d6 3a       	ldab	*0x3a
    e173:	18 ce 01 20 	ldy	#0x120
    e177:	bd ea a1    	jsr	0xeaa1
    e17a:	ce e1 b0    	ldx	#0xe1b0
    e17d:	d6 3b       	ldab	*0x3b
    e17f:	18 08       	iny
    e181:	18 08       	iny
    e183:	18 08       	iny
    e185:	bd ea a1    	jsr	0xeaa1
    e188:	ce e1 b0    	ldx	#0xe1b0
    e18b:	d6 3c       	ldab	*0x3c
    e18d:	18 08       	iny
    e18f:	18 08       	iny
    e191:	bd ea a1    	jsr	0xeaa1
    e194:	ce 01 31    	ldx	#0x131
    e197:	96 37       	ldaa	*0x37
    e199:	bd e9 ee    	jsr	0xe9ee
    e19c:	08          	inx
    e19d:	08          	inx
    e19e:	08          	inx
    e19f:	08          	inx
    e1a0:	96 38       	ldaa	*0x38
    e1a2:	bd e9 ee    	jsr	0xe9ee
    e1a5:	08          	inx
    e1a6:	08          	inx
    e1a7:	08          	inx
    e1a8:	96 39       	ldaa	*0x39
    e1aa:	bd e9 ee    	jsr	0xe9ee
    e1ad:	7e e8 78    	jmp	0xe878
    e1b0:	20 4f       	bra	0x0xe201
    e1b2:	46          	rora
    e1b3:	46          	rora
    e1b4:	46          	rora
    e1b5:	52          	.byte	0x52
    e1b6:	45          	.byte	0x45
    e1b7:	31          	ins
    e1b8:	46          	rora
    e1b9:	52          	.byte	0x52
    e1ba:	45          	.byte	0x45
    e1bb:	32          	pula
    e1bc:	31          	ins
    e1bd:	26 32       	bne	0x0xe1f1
    e1bf:	46          	rora
    e1c0:	4c          	inca
    e1c1:	45          	.byte	0x45
    e1c2:	56          	rorb
    e1c3:	31          	ins
    e1c4:	4c          	inca
    e1c5:	45          	.byte	0x45
    e1c6:	56          	rorb
    e1c7:	32          	pula
    e1c8:	20 50       	bra	0x0xe21a
    e1ca:	57          	asrb
    e1cb:	31          	ins
    e1cc:	20 50       	bra	0x0xe21e
    e1ce:	57          	asrb
    e1cf:	32          	pula
    e1d0:	31          	ins
    e1d1:	26 32       	bne	0x0xe205
    e1d3:	50          	negb
    e1d4:	46          	rora
    e1d5:	49          	rola
    e1d6:	4c          	inca
    e1d7:	54          	lsrb
    e1d8:	52          	.byte	0x52
    e1d9:	45          	.byte	0x45
    e1da:	53          	comb
    e1db:	4f          	clra
    e1dc:	4c          	inca
    e1dd:	45          	.byte	0x45
    e1de:	56          	rorb
    e1df:	4e          	.byte	0x4e
    e1e0:	58          	aslb
    e1e1:	4d          	tsta
    e1e2:	4f          	clra
    e1e3:	44          	lsra
    e1e4:	20 45       	bra	0x0xe22b
    e1e6:	41          	.byte	0x41
    e1e7:	31          	ins
    e1e8:	20 45       	bra	0x0xe22f
    e1ea:	41          	.byte	0x41
    e1eb:	33          	pulb
    e1ec:	20 45       	bra	0x0xe233
    e1ee:	58          	aslb
    e1ef:	54          	lsrb
    e1f0:	20 56       	bra	0x0xe248
    e1f2:	4f          	clra
    e1f3:	4c          	inca
    e1f4:	bd e9 3b    	jsr	0xe93b
    e1f7:	ce 01 20    	ldx	#0x120
    e1fa:	18 ce e2 59 	ldy	#0xe259
    e1fe:	c6 20       	ldab	#0x20
    e200:	18 a6 00    	ldaa	0x0,y
    e203:	a7 00       	staa	0x0,x
    e205:	08          	inx
    e206:	18 08       	iny
    e208:	5a          	decb
    e209:	26 f5       	bne	0x0xe200
    e20b:	ce e2 79    	ldx	#0xe279
    e20e:	18 ce 01 30 	ldy	#0x130
    e212:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe21d
    e216:	d6 27       	ldab	*0x27
    e218:	bd ea a1    	jsr	0xeaa1
    e21b:	20 05       	bra	0x0xe222
    e21d:	d6 34       	ldab	*0x34
    e21f:	bd ea a1    	jsr	0xeaa1
    e222:	ce e2 91    	ldx	#0xe291
    e225:	18 08       	iny
    e227:	18 08       	iny
    e229:	18 08       	iny
    e22b:	12 f4 02 09 	brset	*0xf4, #0x02, 0x0xe238
    e22f:	d6 31       	ldab	*0x31
    e231:	c4 01       	andb	#0x1
    e233:	bd ea a1    	jsr	0xeaa1
    e236:	20 06       	bra	0x0xe23e
    e238:	d6 31       	ldab	*0x31
    e23a:	54          	lsrb
    e23b:	bd ea a1    	jsr	0xeaa1
    e23e:	ce e2 99    	ldx	#0xe299
    e241:	18 08       	iny
    e243:	18 08       	iny
    e245:	12 f4 02 08 	brset	*0xf4, #0x02, 0x0xe251
    e249:	d6 25       	ldab	*0x25
    e24b:	bd ea a1    	jsr	0xeaa1
    e24e:	7e e8 78    	jmp	0xe878
    e251:	d6 32       	ldab	*0x32
    e253:	bd ea a1    	jsr	0xeaa1
    e256:	7e e8 78    	jmp	0xe878
    e259:	57          	asrb
    e25a:	41          	.byte	0x41
    e25b:	56          	rorb
    e25c:	45          	.byte	0x45
    e25d:	20 20       	bra	0x0xe27f
    e25f:	4d          	tsta
    e260:	4f          	clra
    e261:	44          	lsra
    e262:	45          	.byte	0x45
    e263:	20 53       	bra	0x0xe2b8
    e265:	59          	rolb
    e266:	4e          	.byte	0x4e
    e267:	43          	coma
    e268:	20 20       	bra	0x0xe28a
    e26a:	20 20       	bra	0x0xe28c
    e26c:	20 20       	bra	0x0xe28e
    e26e:	20 20       	bra	0x0xe290
    e270:	20 20       	bra	0x0xe292
    e272:	20 20       	bra	0x0xe294
    e274:	20 20       	bra	0x0xe296
    e276:	20 20       	bra	0x0xe298
    e278:	20 20       	bra	0x0xe29a
    e27a:	54          	lsrb
    e27b:	52          	.byte	0x52
    e27c:	49          	rola
    e27d:	20 53       	bra	0x0xe2d2
    e27f:	51          	.byte	0x51
    e280:	52          	.byte	0x52
    e281:	53          	comb
    e282:	57          	asrb
    e283:	55          	.byte	0x55
    e284:	50          	negb
    e285:	53          	comb
    e286:	57          	asrb
    e287:	44          	lsra
    e288:	4e          	.byte	0x4e
    e289:	52          	.byte	0x52
    e28a:	41          	.byte	0x41
    e28b:	4e          	.byte	0x4e
    e28c:	44          	lsra
    e28d:	20 53       	bra	0x0xe2e2
    e28f:	2f 48       	ble	0x0xe2d9
    e291:	4d          	tsta
    e292:	4f          	clra
    e293:	4e          	.byte	0x4e
    e294:	4f          	clra
    e295:	50          	negb
    e296:	4f          	clra
    e297:	4c          	inca
    e298:	59          	rolb
    e299:	53          	comb
    e29a:	45          	.byte	0x45
    e29b:	4c          	inca
    e29c:	46          	rora
    e29d:	20 20       	bra	0x0xe2bf
    e29f:	20 34       	bra	0x0xe2d5
    e2a1:	20 20       	bra	0x0xe2c3
    e2a3:	20 32       	bra	0x0xe2d7
    e2a5:	20 20       	bra	0x0xe2c7
    e2a7:	20 31       	bra	0x0xe2da
    e2a9:	20 20       	bra	0x0xe2cb
    e2ab:	31          	ins
    e2ac:	54          	lsrb
    e2ad:	20 31       	bra	0x0xe2e0
    e2af:	2f 32       	ble	0x0xe2e3
    e2b1:	31          	ins
    e2b2:	2f 32       	ble	0x0xe2e6
    e2b4:	54          	lsrb
    e2b5:	20 31       	bra	0x0xe2e8
    e2b7:	2f 34       	ble	0x0xe2ed
    e2b9:	31          	ins
    e2ba:	2f 34       	ble	0x0xe2f0
    e2bc:	54          	lsrb
    e2bd:	20 31       	bra	0x0xe2f0
    e2bf:	2f 38       	ble	0x0xe2f9
    e2c1:	31          	ins
    e2c2:	2f 38       	ble	0x0xe2fc
    e2c4:	54          	lsrb
    e2c5:	31          	ins
    e2c6:	2f 31       	ble	0x0xe2f9
    e2c8:	36          	psha
    e2c9:	20 31       	bra	0x0xe2fc
    e2cb:	36          	psha
    e2cc:	54          	lsrb
    e2cd:	bd e9 3b    	jsr	0xe93b
    e2d0:	ce 01 20    	ldx	#0x120
    e2d3:	18 ce e3 0a 	ldy	#0xe30a
    e2d7:	c6 20       	ldab	#0x20
    e2d9:	18 a6 00    	ldaa	0x0,y
    e2dc:	a7 00       	staa	0x0,x
    e2de:	08          	inx
    e2df:	18 08       	iny
    e2e1:	5a          	decb
    e2e2:	26 f5       	bne	0x0xe2d9
    e2e4:	ce e3 33    	ldx	#0xe333
    e2e7:	18 ce 01 37 	ldy	#0x137
    e2eb:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xe2f6
    e2ef:	d6 26       	ldab	*0x26
    e2f1:	bd ea 8d    	jsr	0xea8d
    e2f4:	20 05       	bra	0x0xe2fb
    e2f6:	d6 33       	ldab	*0x33
    e2f8:	bd ea 8d    	jsr	0xea8d
    e2fb:	d6 96       	ldab	*0x96
    e2fd:	ce e3 42    	ldx	#0xe342
    e300:	18 ce 01 3c 	ldy	#0x13c
    e304:	bd ea 8d    	jsr	0xea8d
    e307:	7e e8 78    	jmp	0xe878
    e30a:	20 20       	bra	0x0xe32c
    e30c:	20 20       	bra	0x0xe32e
    e30e:	20 20       	bra	0x0xe330
    e310:	20 4b       	bra	0x0xe35d
    e312:	45          	.byte	0x45
    e313:	59          	rolb
    e314:	20 20       	bra	0x0xe336
    e316:	51          	.byte	0x51
    e317:	55          	.byte	0x55
    e318:	41          	.byte	0x41
    e319:	4e          	.byte	0x4e
    e31a:	20 20       	bra	0x0xe33c
    e31c:	20 20       	bra	0x0xe33e
    e31e:	20 20       	bra	0x0xe340
    e320:	20 20       	bra	0x0xe342
    e322:	20 20       	bra	0x0xe344
    e324:	20 20       	bra	0x0xe346
    e326:	20 20       	bra	0x0xe348
    e328:	20 20       	bra	0x0xe34a
    e32a:	4c          	inca
    e32b:	4f          	clra
    e32c:	57          	asrb
    e32d:	4d          	tsta
    e32e:	45          	.byte	0x45
    e32f:	44          	lsra
    e330:	20 48       	bra	0x0xe37a
    e332:	49          	rola
    e333:	4f          	clra
    e334:	46          	rora
    e335:	46          	rora
    e336:	20 55       	bra	0x0xe38d
    e338:	50          	negb
    e339:	20 44       	bra	0x0xe37f
    e33b:	4e          	.byte	0x4e
    e33c:	55          	.byte	0x55
    e33d:	50          	negb
    e33e:	31          	ins
    e33f:	44          	lsra
    e340:	4e          	.byte	0x4e
    e341:	31          	ins
    e342:	4f          	clra
    e343:	46          	rora
    e344:	46          	rora
    e345:	4c          	inca
    e346:	46          	rora
    e347:	31          	ins
    e348:	4c          	inca
    e349:	46          	rora
    e34a:	32          	pula
    e34b:	31          	ins
    e34c:	26 32       	bne	0x0xe380
    e34e:	7d 01 1d    	tst	0x11d
    e351:	26 0a       	bne	0x0xe35d
    e353:	bd ea 65    	jsr	0xea65
    e356:	ce 10 23    	ldx	#0x1023
    e359:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe359
    e35d:	bd e9 3b    	jsr	0xe93b
    e360:	86 20       	ldaa	#0x20
    e362:	c6 20       	ldab	#0x20
    e364:	ce 01 20    	ldx	#0x120
    e367:	a7 00       	staa	0x0,x
    e369:	08          	inx
    e36a:	5a          	decb
    e36b:	26 fa       	bne	0x0xe367
    e36d:	ce 01 20    	ldx	#0x120
    e370:	18 ce 00 b0 	ldy	#0xb0
    e374:	96 f9       	ldaa	*0xf9
    e376:	81 04       	cmpa	#0x4
    e378:	26 04       	bne	0x0xe37e
    e37a:	18 ce 50 30 	ldy	#0x5030
    e37e:	18 a6 00    	ldaa	0x0,y
    e381:	bd ea 2a    	jsr	0xea2a
    e384:	08          	inx
    e385:	08          	inx
    e386:	08          	inx
    e387:	08          	inx
    e388:	8c 01 2f    	cpx	#0x12f
    e38b:	26 01       	bne	0x0xe38e
    e38d:	08          	inx
    e38e:	18 08       	iny
    e390:	8c 01 3f    	cpx	#0x13f
    e393:	25 e9       	bcs	0x0xe37e
    e395:	7e e8 78    	jmp	0xe878
    e398:	bd e9 3b    	jsr	0xe93b
    e39b:	86 20       	ldaa	#0x20
    e39d:	c6 20       	ldab	#0x20
    e39f:	ce 01 20    	ldx	#0x120
    e3a2:	a7 00       	staa	0x0,x
    e3a4:	08          	inx
    e3a5:	5a          	decb
    e3a6:	26 fa       	bne	0x0xe3a2
    e3a8:	ce 01 20    	ldx	#0x120
    e3ab:	18 ce 00 a8 	ldy	#0xa8
    e3af:	96 f9       	ldaa	*0xf9
    e3b1:	81 04       	cmpa	#0x4
    e3b3:	26 04       	bne	0x0xe3b9
    e3b5:	18 ce 50 28 	ldy	#0x5028
    e3b9:	18 a6 00    	ldaa	0x0,y
    e3bc:	bd e9 ee    	jsr	0xe9ee
    e3bf:	08          	inx
    e3c0:	08          	inx
    e3c1:	8c 01 2f    	cpx	#0x12f
    e3c4:	26 01       	bne	0x0xe3c7
    e3c6:	08          	inx
    e3c7:	18 08       	iny
    e3c9:	8c 01 3f    	cpx	#0x13f
    e3cc:	25 eb       	bcs	0x0xe3b9
    e3ce:	7e e8 78    	jmp	0xe878
    e3d1:	bd e9 3b    	jsr	0xe93b
    e3d4:	86 20       	ldaa	#0x20
    e3d6:	c6 20       	ldab	#0x20
    e3d8:	ce 01 20    	ldx	#0x120
    e3db:	a7 00       	staa	0x0,x
    e3dd:	08          	inx
    e3de:	5a          	decb
    e3df:	26 fa       	bne	0x0xe3db
    e3e1:	ce 01 20    	ldx	#0x120
    e3e4:	18 ce 00 b8 	ldy	#0xb8
    e3e8:	96 f9       	ldaa	*0xf9
    e3ea:	81 04       	cmpa	#0x4
    e3ec:	26 04       	bne	0x0xe3f2
    e3ee:	18 ce 50 38 	ldy	#0x5038
    e3f2:	18 a6 00    	ldaa	0x0,y
    e3f5:	bd e9 ee    	jsr	0xe9ee
    e3f8:	08          	inx
    e3f9:	08          	inx
    e3fa:	8c 01 2f    	cpx	#0x12f
    e3fd:	26 01       	bne	0x0xe400
    e3ff:	08          	inx
    e400:	18 08       	iny
    e402:	8c 01 3f    	cpx	#0x13f
    e405:	25 eb       	bcs	0x0xe3f2
    e407:	7e e8 78    	jmp	0xe878
    e40a:	bd e9 3b    	jsr	0xe93b
    e40d:	ce 01 20    	ldx	#0x120
    e410:	18 ce e4 48 	ldy	#0xe448
    e414:	c6 20       	ldab	#0x20
    e416:	18 a6 00    	ldaa	0x0,y
    e419:	a7 00       	staa	0x0,x
    e41b:	08          	inx
    e41c:	18 08       	iny
    e41e:	5a          	decb
    e41f:	26 f5       	bne	0x0xe416
    e421:	ce e2 79    	ldx	#0xe279
    e424:	d6 a7       	ldab	*0xa7
    e426:	18 ce 01 30 	ldy	#0x130
    e42a:	bd ea a1    	jsr	0xeaa1
    e42d:	ce e3 33    	ldx	#0xe333
    e430:	d6 a6       	ldab	*0xa6
    e432:	18 ce 01 37 	ldy	#0x137
    e436:	bd ea 8d    	jsr	0xea8d
    e439:	ce e2 99    	ldx	#0xe299
    e43c:	d6 a5       	ldab	*0xa5
    e43e:	18 ce 01 3b 	ldy	#0x13b
    e442:	bd ea a1    	jsr	0xeaa1
    e445:	7e e8 78    	jmp	0xe878
    e448:	57          	asrb
    e449:	41          	.byte	0x41
    e44a:	56          	rorb
    e44b:	45          	.byte	0x45
    e44c:	20 20       	bra	0x0xe46e
    e44e:	20 4b       	bra	0x0xe49b
    e450:	45          	.byte	0x45
    e451:	59          	rolb
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
    e467:	20 bd       	bra	0x0xe426
    e469:	e9 3b       	adcb	0x3b,x
    e46b:	ce 01 20    	ldx	#0x120
    e46e:	18 ce e4 be 	ldy	#0xe4be
    e472:	c6 20       	ldab	#0x20
    e474:	18 a6 00    	ldaa	0x0,y
    e477:	a7 00       	staa	0x0,x
    e479:	08          	inx
    e47a:	18 08       	iny
    e47c:	5a          	decb
    e47d:	26 f5       	bne	0x0xe474
    e47f:	ce d9 6c    	ldx	#0xd96c
    e482:	18 ce 01 30 	ldy	#0x130
    e486:	13 21 02 07 	brclr	*0x21, #0x02, 0x0xe491
    e48a:	c6 01       	ldab	#0x1
    e48c:	bd ea 8d    	jsr	0xea8d
    e48f:	20 04       	bra	0x0xe495
    e491:	5f          	clrb
    e492:	bd ea 8d    	jsr	0xea8d
    e495:	18 ce 01 34 	ldy	#0x134
    e499:	ce d9 6c    	ldx	#0xd96c
    e49c:	12 21 04 06 	brset	*0x21, #0x04, 0x0xe4a6
    e4a0:	5f          	clrb
    e4a1:	bd ea 8d    	jsr	0xea8d
    e4a4:	20 05       	bra	0x0xe4ab
    e4a6:	c6 01       	ldab	#0x1
    e4a8:	bd ea 8d    	jsr	0xea8d
    e4ab:	96 22       	ldaa	*0x22
    e4ad:	ce 01 38    	ldx	#0x138
    e4b0:	bd ea 2a    	jsr	0xea2a
    e4b3:	96 23       	ldaa	*0x23
    e4b5:	ce 01 3c    	ldx	#0x13c
    e4b8:	bd e9 ee    	jsr	0xe9ee
    e4bb:	7e e8 78    	jmp	0xe878
    e4be:	47          	asra
    e4bf:	4c          	inca
    e4c0:	53          	comb
    e4c1:	20 41       	bra	0x0xe504
    e4c3:	55          	.byte	0x55
    e4c4:	54          	lsrb
    e4c5:	20 49       	bra	0x0xe510
    e4c7:	4e          	.byte	0x4e
    e4c8:	54          	lsrb
    e4c9:	20 44       	bra	0x0xe50f
    e4cb:	59          	rolb
    e4cc:	4e          	.byte	0x4e
    e4cd:	20 20       	bra	0x0xe4ef
    e4cf:	20 20       	bra	0x0xe4f1
    e4d1:	20 20       	bra	0x0xe4f3
    e4d3:	20 20       	bra	0x0xe4f5
    e4d5:	20 20       	bra	0x0xe4f7
    e4d7:	20 20       	bra	0x0xe4f9
    e4d9:	20 20       	bra	0x0xe4fb
    e4db:	20 20       	bra	0x0xe4fd
    e4dd:	20 7d       	bra	0x0xe55c
    e4df:	01          	nop
    e4e0:	1d 26 0a    	bclr	0x26,x, #0x0a
    e4e3:	bd ea 65    	jsr	0xea65
    e4e6:	ce 10 23    	ldx	#0x1023
    e4e9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe4e9
    e4ed:	bd e9 3b    	jsr	0xe93b
    e4f0:	ce 01 20    	ldx	#0x120
    e4f3:	18 ce e5 12 	ldy	#0xe512
    e4f7:	c6 20       	ldab	#0x20
    e4f9:	18 a6 00    	ldaa	0x0,y
    e4fc:	a7 00       	staa	0x0,x
    e4fe:	08          	inx
    e4ff:	18 08       	iny
    e501:	5a          	decb
    e502:	26 f5       	bne	0x0xe4f9
    e504:	ce d6 fc    	ldx	#0xd6fc
    e507:	5f          	clrb
    e508:	18 ce 01 31 	ldy	#0x131
    e50c:	bd ea 8d    	jsr	0xea8d
    e50f:	7e e8 78    	jmp	0xe878
    e512:	20 54       	bra	0x0xe568
    e514:	55          	.byte	0x55
    e515:	4e          	.byte	0x4e
    e516:	45          	.byte	0x45
    e517:	3f          	swi
    e518:	20 20       	bra	0x0xe53a
    e51a:	20 20       	bra	0x0xe53c
    e51c:	20 20       	bra	0x0xe53e
    e51e:	20 20       	bra	0x0xe540
    e520:	20 20       	bra	0x0xe542
    e522:	20 20       	bra	0x0xe544
    e524:	20 20       	bra	0x0xe546
    e526:	20 20       	bra	0x0xe548
    e528:	20 20       	bra	0x0xe54a
    e52a:	20 20       	bra	0x0xe54c
    e52c:	20 20       	bra	0x0xe54e
    e52e:	20 20       	bra	0x0xe550
    e530:	20 20       	bra	0x0xe552
    e532:	7d 01 1d    	tst	0x11d
    e535:	26 0a       	bne	0x0xe541
    e537:	bd ea 65    	jsr	0xea65
    e53a:	ce 10 23    	ldx	#0x1023
    e53d:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe53d
    e541:	bd e9 3b    	jsr	0xe93b
    e544:	ce 01 20    	ldx	#0x120
    e547:	18 ce e5 6a 	ldy	#0xe56a
    e54b:	c6 20       	ldab	#0x20
    e54d:	18 a6 00    	ldaa	0x0,y
    e550:	a7 00       	staa	0x0,x
    e552:	08          	inx
    e553:	18 08       	iny
    e555:	5a          	decb
    e556:	26 f5       	bne	0x0xe54d
    e558:	f6 10 28    	ldab	0x1028
    e55b:	c4 03       	andb	#0x3
    e55d:	ce e5 8a    	ldx	#0xe58a
    e560:	18 ce 01 30 	ldy	#0x130
    e564:	bd ea a1    	jsr	0xeaa1
    e567:	7e e8 78    	jmp	0xe878
    e56a:	53          	comb
    e56b:	45          	.byte	0x45
    e56c:	54          	lsrb
    e56d:	20 53       	bra	0x0xe5c2
    e56f:	50          	negb
    e570:	49          	rola
    e571:	20 42       	bra	0x0xe5b5
    e573:	49          	rola
    e574:	54          	lsrb
    e575:	20 52       	bra	0x0xe5c9
    e577:	41          	.byte	0x41
    e578:	54          	lsrb
    e579:	45          	.byte	0x45
    e57a:	20 20       	bra	0x0xe59c
    e57c:	20 20       	bra	0x0xe59e
    e57e:	20 4b       	bra	0x0xe5cb
    e580:	42          	.byte	0x42
    e581:	49          	rola
    e582:	54          	lsrb
    e583:	2f 53       	ble	0x0xe5d8
    e585:	45          	.byte	0x45
    e586:	43          	coma
    e587:	20 20       	bra	0x0xe5a9
    e589:	20 20       	bra	0x0xe5ab
    e58b:	20 31       	bra	0x0xe5be
    e58d:	4b          	.byte	0x4b
    e58e:	20 35       	bra	0x0xe5c5
    e590:	30          	tsx
    e591:	30          	tsx
    e592:	20 31       	bra	0x0xe5c5
    e594:	32          	pula
    e595:	35          	txs
    e596:	36          	psha
    e597:	32          	pula
    e598:	2e 35       	bgt	0x0xe5cf
    e59a:	7d 01 1d    	tst	0x11d
    e59d:	26 0a       	bne	0x0xe5a9
    e59f:	bd ea 65    	jsr	0xea65
    e5a2:	ce 10 23    	ldx	#0x1023
    e5a5:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe5a5
    e5a9:	bd e9 3b    	jsr	0xe93b
    e5ac:	ce 01 20    	ldx	#0x120
    e5af:	18 ce e5 cc 	ldy	#0xe5cc
    e5b3:	c6 20       	ldab	#0x20
    e5b5:	18 a6 00    	ldaa	0x0,y
    e5b8:	a7 00       	staa	0x0,x
    e5ba:	08          	inx
    e5bb:	18 08       	iny
    e5bd:	5a          	decb
    e5be:	26 f5       	bne	0x0xe5b5
    e5c0:	96 f0       	ldaa	*0xf0
    e5c2:	4c          	inca
    e5c3:	ce 01 31    	ldx	#0x131
    e5c6:	bd e9 ee    	jsr	0xe9ee
    e5c9:	7e e8 78    	jmp	0xe878
    e5cc:	53          	comb
    e5cd:	45          	.byte	0x45
    e5ce:	54          	lsrb
    e5cf:	20 23       	bra	0x0xe5f4
    e5d1:	20 4f       	bra	0x0xe622
    e5d3:	46          	rora
    e5d4:	20 56       	bra	0x0xe62c
    e5d6:	4f          	clra
    e5d7:	49          	rola
    e5d8:	43          	coma
    e5d9:	45          	.byte	0x45
    e5da:	53          	comb
    e5db:	20 20       	bra	0x0xe5fd
    e5dd:	20 20       	bra	0x0xe5ff
    e5df:	20 20       	bra	0x0xe601
    e5e1:	20 20       	bra	0x0xe603
    e5e3:	20 20       	bra	0x0xe605
    e5e5:	20 20       	bra	0x0xe607
    e5e7:	20 20       	bra	0x0xe609
    e5e9:	20 20       	bra	0x0xe60b
    e5eb:	20 7d       	bra	0x0xe66a
    e5ed:	01          	nop
    e5ee:	1d 26 0a    	bclr	0x26,x, #0x0a
    e5f1:	bd ea 65    	jsr	0xea65
    e5f4:	ce 10 23    	ldx	#0x1023
    e5f7:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe5f7
    e5fb:	bd e9 3b    	jsr	0xe93b
    e5fe:	ce 01 20    	ldx	#0x120
    e601:	18 ce e6 1f 	ldy	#0xe61f
    e605:	c6 20       	ldab	#0x20
    e607:	18 a6 00    	ldaa	0x0,y
    e60a:	a7 00       	staa	0x0,x
    e60c:	08          	inx
    e60d:	18 08       	iny
    e60f:	5a          	decb
    e610:	26 f5       	bne	0x0xe607
    e612:	96 de       	ldaa	*0xde
    e614:	4c          	inca
    e615:	4c          	inca
    e616:	ce 01 31    	ldx	#0x131
    e619:	bd e9 ee    	jsr	0xe9ee
    e61c:	7e e8 78    	jmp	0xe878
    e61f:	53          	comb
    e620:	45          	.byte	0x45
    e621:	54          	lsrb
    e622:	20 23       	bra	0x0xe647
    e624:	20 4f       	bra	0x0xe675
    e626:	46          	rora
    e627:	20 46       	bra	0x0xe66f
    e629:	49          	rola
    e62a:	4c          	inca
    e62b:	54          	lsrb
    e62c:	45          	.byte	0x45
    e62d:	52          	.byte	0x52
    e62e:	53          	comb
    e62f:	20 20       	bra	0x0xe651
    e631:	20 20       	bra	0x0xe653
    e633:	20 20       	bra	0x0xe655
    e635:	20 20       	bra	0x0xe657
    e637:	20 20       	bra	0x0xe659
    e639:	20 20       	bra	0x0xe65b
    e63b:	20 20       	bra	0x0xe65d
    e63d:	20 20       	bra	0x0xe65f
    e63f:	7d 01 1d    	tst	0x11d
    e642:	26 0a       	bne	0x0xe64e
    e644:	bd ea 65    	jsr	0xea65
    e647:	ce 10 23    	ldx	#0x1023
    e64a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe64a
    e64e:	bd e9 3b    	jsr	0xe93b
    e651:	ce 01 20    	ldx	#0x120
    e654:	18 ce e7 29 	ldy	#0xe729
    e658:	c6 20       	ldab	#0x20
    e65a:	b6 50 00    	ldaa	0x5000
    e65d:	81 06       	cmpa	#0x6
    e65f:	25 04       	bcs	0x0xe665
    e661:	18 ce e7 49 	ldy	#0xe749
    e665:	18 a6 00    	ldaa	0x0,y
    e668:	a7 00       	staa	0x0,x
    e66a:	08          	inx
    e66b:	18 08       	iny
    e66d:	5a          	decb
    e66e:	26 f5       	bne	0x0xe665
    e670:	b6 01 6b    	ldaa	0x16b
    e673:	8b 31       	adda	#0x31
    e675:	b7 01 25    	staa	0x125
    e678:	ce 50 01    	ldx	#0x5001
    e67b:	b6 01 6b    	ldaa	0x16b
    e67e:	c6 04       	ldab	#0x4
    e680:	3d          	mul
    e681:	3a          	abx
    e682:	a6 00       	ldaa	0x0,x
    e684:	8b 41       	adda	#0x41
    e686:	b7 01 32    	staa	0x132
    e689:	a6 01       	ldaa	0x1,x
    e68b:	3c          	pshx
    e68c:	4c          	inca
    e68d:	ce 01 34    	ldx	#0x134
    e690:	bd e9 ee    	jsr	0xe9ee
    e693:	b6 50 00    	ldaa	0x5000
    e696:	81 06       	cmpa	#0x6
    e698:	27 0c       	beq	0x0xe6a6
    e69a:	38          	pulx
    e69b:	a6 03       	ldaa	0x3,x
    e69d:	3c          	pshx
    e69e:	ce 01 3c    	ldx	#0x13c
    e6a1:	bd e9 ee    	jsr	0xe9ee
    e6a4:	20 42       	bra	0x0xe6e8
    e6a6:	38          	pulx
    e6a7:	7d 01 6b    	tst	0x16b
    e6aa:	26 06       	bne	0x0xe6b2
    e6ac:	96 f0       	ldaa	*0xf0
    e6ae:	4c          	inca
    e6af:	b7 50 23    	staa	0x5023
    e6b2:	e6 02       	ldab	0x2,x
    e6b4:	c4 40       	andb	#0x40
    e6b6:	f7 01 7d    	stab	0x17d
    e6b9:	e6 02       	ldab	0x2,x
    e6bb:	c4 3f       	andb	#0x3f
    e6bd:	b6 50 23    	ldaa	0x5023
    e6c0:	11          	cba
    e6c1:	24 03       	bcc	0x0xe6c6
    e6c3:	5a          	decb
    e6c4:	20 fa       	bra	0x0xe6c0
    e6c6:	10          	sba
    e6c7:	b7 50 23    	staa	0x5023
    e6ca:	37          	pshb
    e6cb:	fa 01 7d    	orab	0x17d
    e6ce:	e7 02       	stab	0x2,x
    e6d0:	32          	pula
    e6d1:	3c          	pshx
    e6d2:	ce 01 38    	ldx	#0x138
    e6d5:	bd e9 ee    	jsr	0xe9ee
    e6d8:	38          	pulx
    e6d9:	e6 03       	ldab	0x3,x
    e6db:	c4 01       	andb	#0x1
    e6dd:	3c          	pshx
    e6de:	ce e7 69    	ldx	#0xe769
    e6e1:	18 ce 01 3c 	ldy	#0x13c
    e6e5:	bd ea 8d    	jsr	0xea8d
    e6e8:	38          	pulx
    e6e9:	e6 00       	ldab	0x0,x
    e6eb:	c1 01       	cmpb	#0x1
    e6ed:	23 17       	bls	0x0xe706
    e6ef:	a6 01       	ldaa	0x1,x
    e6f1:	d7 f9       	stab	*0xf9
    e6f3:	bd ab 71    	jsr	0xab71
    e6f6:	c6 04       	ldab	#0x4
    e6f8:	d7 f9       	stab	*0xf9
    e6fa:	bd a2 d0    	jsr	0xa2d0
    e6fd:	bd a2 eb    	jsr	0xa2eb
    e700:	bd ab 71    	jsr	0xab71
    e703:	7e e8 78    	jmp	0xe878
    e706:	4f          	clra
    e707:	b7 10 22    	staa	0x1022
    e70a:	b6 10 2d    	ldaa	0x102d
    e70d:	36          	psha
    e70e:	84 7f       	anda	#0x7f
    e710:	b7 10 2d    	staa	0x102d
    e713:	a6 00       	ldaa	0x0,x
    e715:	e6 01       	ldab	0x1,x
    e717:	bd 79 00    	jsr	0x7900
    e71a:	32          	pula
    e71b:	b7 10 2d    	staa	0x102d
    e71e:	86 80       	ldaa	#0x80
    e720:	b7 10 22    	staa	0x1022
    e723:	bd a2 eb    	jsr	0xa2eb
    e726:	7e e8 78    	jmp	0xe878
    e729:	50          	negb
    e72a:	41          	.byte	0x41
    e72b:	54          	lsrb
    e72c:	43          	coma
    e72d:	48          	asla
    e72e:	20 20       	bra	0x0xe750
    e730:	20 20       	bra	0x0xe752
    e732:	20 20       	bra	0x0xe754
    e734:	20 56       	bra	0x0xe78c
    e736:	4f          	clra
    e737:	4c          	inca
    e738:	20 20       	bra	0x0xe75a
    e73a:	20 20       	bra	0x0xe75c
    e73c:	20 20       	bra	0x0xe75e
    e73e:	20 20       	bra	0x0xe760
    e740:	20 20       	bra	0x0xe762
    e742:	20 20       	bra	0x0xe764
    e744:	20 20       	bra	0x0xe766
    e746:	20 20       	bra	0x0xe768
    e748:	20 50       	bra	0x0xe79a
    e74a:	41          	.byte	0x41
    e74b:	54          	lsrb
    e74c:	43          	coma
    e74d:	48          	asla
    e74e:	20 20       	bra	0x0xe770
    e750:	23 56       	bls	0x0xe7a8
    e752:	43          	coma
    e753:	53          	comb
    e754:	20 54       	bra	0x0xe7aa
    e756:	59          	rolb
    e757:	50          	negb
    e758:	45          	.byte	0x45
    e759:	20 20       	bra	0x0xe77b
    e75b:	20 20       	bra	0x0xe77d
    e75d:	20 20       	bra	0x0xe77f
    e75f:	20 20       	bra	0x0xe781
    e761:	20 20       	bra	0x0xe783
    e763:	20 20       	bra	0x0xe785
    e765:	20 20       	bra	0x0xe787
    e767:	20 20       	bra	0x0xe789
    e769:	4d          	tsta
    e76a:	4f          	clra
    e76b:	4e          	.byte	0x4e
    e76c:	50          	negb
    e76d:	4c          	inca
    e76e:	59          	rolb
    e76f:	7d 01 1d    	tst	0x11d
    e772:	26 0a       	bne	0x0xe77e
    e774:	bd ea 65    	jsr	0xea65
    e777:	ce 10 23    	ldx	#0x1023
    e77a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe77a
    e77e:	bd e9 3b    	jsr	0xe93b
    e781:	ce 01 20    	ldx	#0x120
    e784:	18 ce e7 c0 	ldy	#0xe7c0
    e788:	c6 20       	ldab	#0x20
    e78a:	18 a6 00    	ldaa	0x0,y
    e78d:	a7 00       	staa	0x0,x
    e78f:	08          	inx
    e790:	18 08       	iny
    e792:	5a          	decb
    e793:	26 f5       	bne	0x0xe78a
    e795:	ce 50 03    	ldx	#0x5003
    e798:	b6 01 6b    	ldaa	0x16b
    e79b:	c6 04       	ldab	#0x4
    e79d:	3d          	mul
    e79e:	3a          	abx
    e79f:	7d 50 23    	tst	0x5023
    e7a2:	26 07       	bne	0x0xe7ab
    e7a4:	e6 00       	ldab	0x0,x
    e7a6:	c4 3f       	andb	#0x3f
    e7a8:	f7 50 23    	stab	0x5023
    e7ab:	e6 00       	ldab	0x0,x
    e7ad:	c4 40       	andb	#0x40
    e7af:	27 02       	beq	0x0xe7b3
    e7b1:	c6 01       	ldab	#0x1
    e7b3:	18 ce 01 31 	ldy	#0x131
    e7b7:	ce e7 e0    	ldx	#0xe7e0
    e7ba:	bd ea 8d    	jsr	0xea8d
    e7bd:	7e e8 78    	jmp	0xe878
    e7c0:	32          	pula
    e7c1:	4d          	tsta
    e7c2:	49          	rola
    e7c3:	58          	aslb
    e7c4:	20 20       	bra	0x0xe7e6
    e7c6:	20 20       	bra	0x0xe7e8
    e7c8:	20 20       	bra	0x0xe7ea
    e7ca:	20 20       	bra	0x0xe7ec
    e7cc:	20 20       	bra	0x0xe7ee
    e7ce:	20 20       	bra	0x0xe7f0
    e7d0:	20 20       	bra	0x0xe7f2
    e7d2:	20 20       	bra	0x0xe7f4
    e7d4:	20 20       	bra	0x0xe7f6
    e7d6:	20 20       	bra	0x0xe7f8
    e7d8:	20 20       	bra	0x0xe7fa
    e7da:	20 20       	bra	0x0xe7fc
    e7dc:	20 20       	bra	0x0xe7fe
    e7de:	20 20       	bra	0x0xe800
    e7e0:	4f          	clra
    e7e1:	46          	rora
    e7e2:	46          	rora
    e7e3:	20 4f       	bra	0x0xe834
    e7e5:	4e          	.byte	0x4e
    e7e6:	ce 10 23    	ldx	#0x1023
    e7e9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe7e9
    e7ed:	bd e9 3b    	jsr	0xe93b
    e7f0:	ce 01 20    	ldx	#0x120
    e7f3:	18 ce e8 07 	ldy	#0xe807
    e7f7:	c6 20       	ldab	#0x20
    e7f9:	18 a6 00    	ldaa	0x0,y
    e7fc:	a7 00       	staa	0x0,x
    e7fe:	08          	inx
    e7ff:	18 08       	iny
    e801:	5a          	decb
    e802:	26 f5       	bne	0x0xe7f9
    e804:	7e e8 78    	jmp	0xe878
    e807:	55          	.byte	0x55
    e808:	50          	negb
    e809:	4c          	inca
    e80a:	4f          	clra
    e80b:	41          	.byte	0x41
    e80c:	44          	lsra
    e80d:	20 52       	bra	0x0xe861
    e80f:	41          	.byte	0x41
    e810:	4d          	tsta
    e811:	20 42       	bra	0x0xe855
    e813:	4e          	.byte	0x4e
    e814:	4b          	.byte	0x4b
    e815:	20 43       	bra	0x0xe85a
    e817:	20 20       	bra	0x0xe839
    e819:	20 20       	bra	0x0xe83b
    e81b:	54          	lsrb
    e81c:	4f          	clra
    e81d:	20 52       	bra	0x0xe871
    e81f:	4f          	clra
    e820:	4d          	tsta
    e821:	20 42       	bra	0x0xe865
    e823:	4e          	.byte	0x4e
    e824:	4b          	.byte	0x4b
    e825:	20 41       	bra	0x0xe868
    e827:	b6 01 2f    	ldaa	0x12f
    e82a:	97 99       	staa	*0x99
    e82c:	b6 01 3f    	ldaa	0x13f
    e82f:	97 9a       	staa	*0x9a
    e831:	ce 10 23    	ldx	#0x1023
    e834:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe834
    e838:	bd e9 3b    	jsr	0xe93b
    e83b:	ce 01 20    	ldx	#0x120
    e83e:	18 ce e8 58 	ldy	#0xe858
    e842:	c6 20       	ldab	#0x20
    e844:	18 a6 00    	ldaa	0x0,y
    e847:	a7 00       	staa	0x0,x
    e849:	08          	inx
    e84a:	18 08       	iny
    e84c:	5a          	decb
    e84d:	26 f5       	bne	0x0xe844
    e84f:	96 f2       	ldaa	*0xf2
    e851:	84 20       	anda	#0x20
    e853:	97 f2       	staa	*0xf2
    e855:	7e e8 78    	jmp	0xe878
    e858:	20 41       	bra	0x0xe89b
    e85a:	52          	.byte	0x52
    e85b:	45          	.byte	0x45
    e85c:	20 59       	bra	0x0xe8b7
    e85e:	4f          	clra
    e85f:	55          	.byte	0x55
    e860:	20 53       	bra	0x0xe8b5
    e862:	55          	.byte	0x55
    e863:	52          	.byte	0x52
    e864:	45          	.byte	0x45
    e865:	20 3f       	bra	0x0xe8a6
    e867:	20 20       	bra	0x0xe889
    e869:	20 20       	bra	0x0xe88b
    e86b:	20 20       	bra	0x0xe88d
    e86d:	20 20       	bra	0x0xe88f
    e86f:	20 20       	bra	0x0xe891
    e871:	20 20       	bra	0x0xe893
    e873:	20 20       	bra	0x0xe895
    e875:	4e          	.byte	0x4e
    e876:	4f          	clra
    e877:	20 7f       	bra	0x0xe8f8
    e879:	01          	nop
    e87a:	1e 86 20 b7 	brset	0x86,x, #0x20, 0x0xe835
    e87e:	01          	nop
    e87f:	1c ce 10    	bset	0xce,x, #0x10
    e882:	23 1f       	bls	0x0xe8a3
    e884:	00          	bgnd
    e885:	10          	sba
    e886:	fc 7e e8    	ldd	0x7ee8
    e889:	8a 8d       	oraa	#0x8d
    e88b:	03          	fdiv
    e88c:	7e d3 af    	jmp	0xd3af
    e88f:	ce 01 20    	ldx	#0x120
    e892:	f6 01 1c    	ldab	0x11c
    e895:	5a          	decb
    e896:	3a          	abx
    e897:	a6 00       	ldaa	0x0,x
    e899:	b7 10 47    	staa	0x1047
    e89c:	86 88       	ldaa	#0x88
    e89e:	ba 10 00    	oraa	0x1000
    e8a1:	b7 10 00    	staa	0x1000
    e8a4:	88 80       	eora	#0x80
    e8a6:	b7 10 00    	staa	0x1000
    e8a9:	88 08       	eora	#0x8
    e8ab:	b7 10 00    	staa	0x1000
    e8ae:	37          	pshb
    e8af:	bd e9 07    	jsr	0xe907
    e8b2:	33          	pulb
    e8b3:	f7 01 1c    	stab	0x11c
    e8b6:	c1 10       	cmpb	#0x10
    e8b8:	27 01       	beq	0x0xe8bb
    e8ba:	39          	rts
    e8bb:	18 ce 10 23 	ldy	#0x1023
    e8bf:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xe8bf
    e8c3:	fb 
    e8c4:	86 8f       	ldaa	#0x8f
    e8c6:	b7 10 47    	staa	0x1047
    e8c9:	86 80       	ldaa	#0x80
    e8cb:	ba 10 00    	oraa	0x1000
    e8ce:	b7 10 00    	staa	0x1000
    e8d1:	88 80       	eora	#0x80
    e8d3:	b7 10 00    	staa	0x1000
    e8d6:	bd e9 07    	jsr	0xe907
    e8d9:	39          	rts
    e8da:	f6 10 23    	ldab	0x1023
    e8dd:	c5 10       	bitb	#0x10
    e8df:	27 f9       	beq	0x0xe8da
    e8e1:	a6 00       	ldaa	0x0,x
    e8e3:	b7 10 47    	staa	0x1047
    e8e6:	86 88       	ldaa	#0x88
    e8e8:	ba 10 00    	oraa	0x1000
    e8eb:	b7 10 00    	staa	0x1000
    e8ee:	88 80       	eora	#0x80
    e8f0:	b7 10 00    	staa	0x1000
    e8f3:	88 08       	eora	#0x8
    e8f5:	b7 10 00    	staa	0x1000
    e8f8:	86 10       	ldaa	#0x10
    e8fa:	b7 10 23    	staa	0x1023
    e8fd:	fc 10 0e    	ldd	0x100e
    e900:	c3 00 f0    	addd	#0xf0
    e903:	fd 10 1c    	std	0x101c
    e906:	39          	rts
    e907:	86 10       	ldaa	#0x10
    e909:	b7 10 23    	staa	0x1023
    e90c:	fc 10 0e    	ldd	0x100e
    e90f:	c3 00 f0    	addd	#0xf0
    e912:	fd 10 1c    	std	0x101c
    e915:	39          	rts
    e916:	f6 10 23    	ldab	0x1023
    e919:	c5 10       	bitb	#0x10
    e91b:	27 f9       	beq	0x0xe916
    e91d:	b6 01 1e    	ldaa	0x11e
    e920:	81 0f       	cmpa	#0xf
    e922:	23 02       	bls	0x0xe926
    e924:	8b 30       	adda	#0x30
    e926:	8a 80       	oraa	#0x80
    e928:	b7 10 47    	staa	0x1047
    e92b:	86 80       	ldaa	#0x80
    e92d:	ba 10 00    	oraa	0x1000
    e930:	b7 10 00    	staa	0x1000
    e933:	88 80       	eora	#0x80
    e935:	b7 10 00    	staa	0x1000
    e938:	7e e9 07    	jmp	0xe907
    e93b:	ce 10 23    	ldx	#0x1023
    e93e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe93e
    e942:	86 cf       	ldaa	#0xcf
    e944:	b7 10 47    	staa	0x1047
    e947:	86 80       	ldaa	#0x80
    e949:	ba 10 00    	oraa	0x1000
    e94c:	b7 10 00    	staa	0x1000
    e94f:	01          	nop
    e950:	88 80       	eora	#0x80
    e952:	b7 10 00    	staa	0x1000
    e955:	7e e9 07    	jmp	0xe907
    e958:	b6 01 1e    	ldaa	0x11e
    e95b:	81 03       	cmpa	#0x3
    e95d:	22 04       	bhi	0x0xe963
    e95f:	86 03       	ldaa	#0x3
    e961:	20 22       	bra	0x0xe985
    e963:	81 09       	cmpa	#0x9
    e965:	22 04       	bhi	0x0xe96b
    e967:	86 09       	ldaa	#0x9
    e969:	20 1a       	bra	0x0xe985
    e96b:	81 0e       	cmpa	#0xe
    e96d:	22 04       	bhi	0x0xe973
    e96f:	86 0e       	ldaa	#0xe
    e971:	20 12       	bra	0x0xe985
    e973:	81 13       	cmpa	#0x13
    e975:	22 04       	bhi	0x0xe97b
    e977:	86 13       	ldaa	#0x13
    e979:	20 0a       	bra	0x0xe985
    e97b:	81 19       	cmpa	#0x19
    e97d:	22 04       	bhi	0x0xe983
    e97f:	86 19       	ldaa	#0x19
    e981:	20 02       	bra	0x0xe985
    e983:	86 1e       	ldaa	#0x1e
    e985:	b7 01 1e    	staa	0x11e
    e988:	7e e9 16    	jmp	0xe916
    e98b:	86 02       	ldaa	#0x2
    e98d:	b1 01 1e    	cmpa	0x11e
    e990:	23 06       	bls	0x0xe998
    e992:	b7 01 1e    	staa	0x11e
    e995:	7e e9 16    	jmp	0xe916
    e998:	86 06       	ldaa	#0x6
    e99a:	b1 01 1e    	cmpa	0x11e
    e99d:	23 06       	bls	0x0xe9a5
    e99f:	b7 01 1e    	staa	0x11e
    e9a2:	7e e9 16    	jmp	0xe916
    e9a5:	86 0a       	ldaa	#0xa
    e9a7:	b1 01 1e    	cmpa	0x11e
    e9aa:	23 06       	bls	0x0xe9b2
    e9ac:	b7 01 1e    	staa	0x11e
    e9af:	7e e9 16    	jmp	0xe916
    e9b2:	86 0e       	ldaa	#0xe
    e9b4:	b1 01 1e    	cmpa	0x11e
    e9b7:	23 06       	bls	0x0xe9bf
    e9b9:	b7 01 1e    	staa	0x11e
    e9bc:	7e e9 16    	jmp	0xe916
    e9bf:	86 12       	ldaa	#0x12
    e9c1:	b1 01 1e    	cmpa	0x11e
    e9c4:	23 06       	bls	0x0xe9cc
    e9c6:	b7 01 1e    	staa	0x11e
    e9c9:	7e e9 16    	jmp	0xe916
    e9cc:	86 16       	ldaa	#0x16
    e9ce:	b1 01 1e    	cmpa	0x11e
    e9d1:	23 06       	bls	0x0xe9d9
    e9d3:	b7 01 1e    	staa	0x11e
    e9d6:	7e e9 16    	jmp	0xe916
    e9d9:	86 1a       	ldaa	#0x1a
    e9db:	b1 01 1e    	cmpa	0x11e
    e9de:	23 06       	bls	0x0xe9e6
    e9e0:	b7 01 1e    	staa	0x11e
    e9e3:	7e e9 16    	jmp	0xe916
    e9e6:	86 1e       	ldaa	#0x1e
    e9e8:	b7 01 1e    	staa	0x11e
    e9eb:	7e e9 16    	jmp	0xe916
    e9ee:	80 64       	suba	#0x64
    e9f0:	24 09       	bcc	0x0xe9fb
    e9f2:	8b 64       	adda	#0x64
    e9f4:	c6 20       	ldab	#0x20
    e9f6:	e7 00       	stab	0x0,x
    e9f8:	08          	inx
    e9f9:	20 05       	bra	0x0xea00
    e9fb:	c6 31       	ldab	#0x31
    e9fd:	e7 00       	stab	0x0,x
    e9ff:	08          	inx
    ea00:	80 0a       	suba	#0xa
    ea02:	24 14       	bcc	0x0xea18
    ea04:	8b 0a       	adda	#0xa
    ea06:	c1 20       	cmpb	#0x20
    ea08:	27 07       	beq	0x0xea11
    ea0a:	c6 30       	ldab	#0x30
    ea0c:	e7 00       	stab	0x0,x
    ea0e:	08          	inx
    ea0f:	20 14       	bra	0x0xea25
    ea11:	c6 20       	ldab	#0x20
    ea13:	e7 00       	stab	0x0,x
    ea15:	08          	inx
    ea16:	20 0d       	bra	0x0xea25
    ea18:	5f          	clrb
    ea19:	5c          	incb
    ea1a:	80 0a       	suba	#0xa
    ea1c:	24 fb       	bcc	0x0xea19
    ea1e:	8b 0a       	adda	#0xa
    ea20:	cb 30       	addb	#0x30
    ea22:	e7 00       	stab	0x0,x
    ea24:	08          	inx
    ea25:	8b 30       	adda	#0x30
    ea27:	a7 00       	staa	0x0,x
    ea29:	39          	rts
    ea2a:	80 40       	suba	#0x40
    ea2c:	25 10       	bcs	0x0xea3e
    ea2e:	26 05       	bne	0x0xea35
    ea30:	8d bc       	bsr	0x0xe9ee
    ea32:	09          	dex
    ea33:	09          	dex
    ea34:	39          	rts
    ea35:	8d b7       	bsr	0x0xe9ee
    ea37:	09          	dex
    ea38:	09          	dex
    ea39:	86 2b       	ldaa	#0x2b
    ea3b:	a7 00       	staa	0x0,x
    ea3d:	39          	rts
    ea3e:	40          	nega
    ea3f:	8d ad       	bsr	0x0xe9ee
    ea41:	09          	dex
    ea42:	09          	dex
    ea43:	86 2d       	ldaa	#0x2d
    ea45:	a7 00       	staa	0x0,x
    ea47:	39          	rts
    ea48:	a6 02       	ldaa	0x2,x
    ea4a:	80 30       	suba	#0x30
    ea4c:	e6 01       	ldab	0x1,x
    ea4e:	c1 20       	cmpb	#0x20
    ea50:	26 01       	bne	0x0xea53
    ea52:	39          	rts
    ea53:	c0 30       	subb	#0x30
    ea55:	36          	psha
    ea56:	86 0a       	ldaa	#0xa
    ea58:	3d          	mul
    ea59:	32          	pula
    ea5a:	1b          	aba
    ea5b:	e6 00       	ldab	0x0,x
    ea5d:	c1 20       	cmpb	#0x20
    ea5f:	26 01       	bne	0x0xea62
    ea61:	39          	rts
    ea62:	8b 64       	adda	#0x64
    ea64:	39          	rts
    ea65:	ce 10 23    	ldx	#0x1023
    ea68:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xea68
    ea6c:	86 f7       	ldaa	#0xf7
    ea6e:	b4 10 00    	anda	0x1000
    ea71:	b7 10 00    	staa	0x1000
    ea74:	86 0e       	ldaa	#0xe
    ea76:	b7 10 47    	staa	0x1047
    ea79:	86 80       	ldaa	#0x80
    ea7b:	ba 10 00    	oraa	0x1000
    ea7e:	b7 10 00    	staa	0x1000
    ea81:	01          	nop
    ea82:	88 80       	eora	#0x80
    ea84:	b7 10 00    	staa	0x1000
    ea87:	7c 01 1d    	inc	0x11d
    ea8a:	7e e9 07    	jmp	0xe907
    ea8d:	86 03       	ldaa	#0x3
    ea8f:	3d          	mul
    ea90:	3a          	abx
    ea91:	c6 03       	ldab	#0x3
    ea93:	a6 00       	ldaa	0x0,x
    ea95:	18 a7 00    	staa	0x0,y
    ea98:	5a          	decb
    ea99:	27 05       	beq	0x0xeaa0
    ea9b:	08          	inx
    ea9c:	18 08       	iny
    ea9e:	20 f3       	bra	0x0xea93
    eaa0:	39          	rts
    eaa1:	86 04       	ldaa	#0x4
    eaa3:	3d          	mul
    eaa4:	3a          	abx
    eaa5:	c6 04       	ldab	#0x4
    eaa7:	a6 00       	ldaa	0x0,x
    eaa9:	18 a7 00    	staa	0x0,y
    eaac:	5a          	decb
    eaad:	27 05       	beq	0x0xeab4
    eaaf:	08          	inx
    eab0:	18 08       	iny
    eab2:	20 f3       	bra	0x0xeaa7
    eab4:	39          	rts
    eab5:	c6 c8       	ldab	#0xc8
    eab7:	3d          	mul
    eab8:	bd e9 ee    	jsr	0xe9ee
    eabb:	86 25       	ldaa	#0x25
    eabd:	09          	dex
    eabe:	09          	dex
    eabf:	a7 00       	staa	0x0,x
    eac1:	39          	rts
    eac2:	b6 01 1e    	ldaa	0x11e
    eac5:	81 0e       	cmpa	#0xe
    eac7:	22 08       	bhi	0x0xead1
    eac9:	8b 10       	adda	#0x10
    eacb:	b7 01 1e    	staa	0x11e
    eace:	7e e9 16    	jmp	0xe916
    ead1:	80 10       	suba	#0x10
    ead3:	b7 01 1e    	staa	0x11e
    ead6:	7e e9 16    	jmp	0xe916
    ead9:	b6 01 7f    	ldaa	0x17f
    eadc:	2b 1e       	bmi	0x0xeafc
    eade:	b6 01 1e    	ldaa	0x11e
    eae1:	81 0e       	cmpa	#0xe
    eae3:	22 0a       	bhi	0x0xeaef
    eae5:	27 32       	beq	0x0xeb19
    eae7:	86 0e       	ldaa	#0xe
    eae9:	b7 01 1e    	staa	0x11e
    eaec:	7e e9 16    	jmp	0xe916
    eaef:	81 1e       	cmpa	#0x1e
    eaf1:	26 01       	bne	0x0xeaf4
    eaf3:	39          	rts
    eaf4:	86 1e       	ldaa	#0x1e
    eaf6:	b7 01 1e    	staa	0x11e
    eaf9:	7e e9 16    	jmp	0xe916
    eafc:	b6 01 1e    	ldaa	0x11e
    eaff:	81 19       	cmpa	#0x19
    eb01:	25 0a       	bcs	0x0xeb0d
    eb03:	27 14       	beq	0x0xeb19
    eb05:	86 19       	ldaa	#0x19
    eb07:	b7 01 1e    	staa	0x11e
    eb0a:	7e e9 16    	jmp	0xe916
    eb0d:	81 09       	cmpa	#0x9
    eb0f:	27 08       	beq	0x0xeb19
    eb11:	86 09       	ldaa	#0x9
    eb13:	b7 01 1e    	staa	0x11e
    eb16:	7e e9 16    	jmp	0xe916
    eb19:	39          	rts
    eb1a:	b6 01 7f    	ldaa	0x17f
    eb1d:	2b 35       	bmi	0x0xeb54
    eb1f:	b6 01 1e    	ldaa	0x11e
    eb22:	81 13       	cmpa	#0x13
    eb24:	25 16       	bcs	0x0xeb3c
    eb26:	22 08       	bhi	0x0xeb30
    eb28:	86 19       	ldaa	#0x19
    eb2a:	b7 01 1e    	staa	0x11e
    eb2d:	7e e9 16    	jmp	0xe916
    eb30:	81 19       	cmpa	#0x19
    eb32:	22 57       	bhi	0x0xeb8b
    eb34:	86 1e       	ldaa	#0x1e
    eb36:	b7 01 1e    	staa	0x11e
    eb39:	7e e9 16    	jmp	0xe916
    eb3c:	81 03       	cmpa	#0x3
    eb3e:	22 08       	bhi	0x0xeb48
    eb40:	86 09       	ldaa	#0x9
    eb42:	b7 01 1e    	staa	0x11e
    eb45:	7e e9 16    	jmp	0xe916
    eb48:	81 09       	cmpa	#0x9
    eb4a:	22 3f       	bhi	0x0xeb8b
    eb4c:	86 0e       	ldaa	#0xe
    eb4e:	b7 01 1e    	staa	0x11e
    eb51:	7e e9 16    	jmp	0xe916
    eb54:	b6 01 1e    	ldaa	0x11e
    eb57:	81 0f       	cmpa	#0xf
    eb59:	25 18       	bcs	0x0xeb73
    eb5b:	81 1a       	cmpa	#0x1a
    eb5d:	25 08       	bcs	0x0xeb67
    eb5f:	86 19       	ldaa	#0x19
    eb61:	b7 01 1e    	staa	0x11e
    eb64:	7e e9 16    	jmp	0xe916
    eb67:	81 19       	cmpa	#0x19
    eb69:	26 20       	bne	0x0xeb8b
    eb6b:	86 13       	ldaa	#0x13
    eb6d:	b7 01 1e    	staa	0x11e
    eb70:	7e e9 16    	jmp	0xe916
    eb73:	81 0e       	cmpa	#0xe
    eb75:	25 08       	bcs	0x0xeb7f
    eb77:	86 09       	ldaa	#0x9
    eb79:	b7 01 1e    	staa	0x11e
    eb7c:	7e e9 16    	jmp	0xe916
    eb7f:	81 09       	cmpa	#0x9
    eb81:	26 08       	bne	0x0xeb8b
    eb83:	86 03       	ldaa	#0x3
    eb85:	b7 01 1e    	staa	0x11e
    eb88:	7e e9 16    	jmp	0xe916
    eb8b:	39          	rts
    eb8c:	b6 01 7f    	ldaa	0x17f
    eb8f:	2b 3a       	bmi	0x0xebcb
    eb91:	b6 01 1e    	ldaa	0x11e
    eb94:	81 12       	cmpa	#0x12
    eb96:	25 16       	bcs	0x0xebae
    eb98:	22 04       	bhi	0x0xeb9e
    eb9a:	86 16       	ldaa	#0x16
    eb9c:	20 26       	bra	0x0xebc4
    eb9e:	81 16       	cmpa	#0x16
    eba0:	22 04       	bhi	0x0xeba6
    eba2:	86 1a       	ldaa	#0x1a
    eba4:	20 1e       	bra	0x0xebc4
    eba6:	81 1a       	cmpa	#0x1a
    eba8:	22 20       	bhi	0x0xebca
    ebaa:	86 1e       	ldaa	#0x1e
    ebac:	20 16       	bra	0x0xebc4
    ebae:	81 02       	cmpa	#0x2
    ebb0:	22 04       	bhi	0x0xebb6
    ebb2:	86 06       	ldaa	#0x6
    ebb4:	20 0e       	bra	0x0xebc4
    ebb6:	81 06       	cmpa	#0x6
    ebb8:	22 04       	bhi	0x0xebbe
    ebba:	86 0a       	ldaa	#0xa
    ebbc:	20 06       	bra	0x0xebc4
    ebbe:	81 0a       	cmpa	#0xa
    ebc0:	22 08       	bhi	0x0xebca
    ebc2:	86 0e       	ldaa	#0xe
    ebc4:	b7 01 1e    	staa	0x11e
    ebc7:	7e e9 16    	jmp	0xe916
    ebca:	39          	rts
    ebcb:	b6 01 1e    	ldaa	0x11e
    ebce:	81 0e       	cmpa	#0xe
    ebd0:	22 16       	bhi	0x0xebe8
    ebd2:	25 04       	bcs	0x0xebd8
    ebd4:	86 0a       	ldaa	#0xa
    ebd6:	20 ec       	bra	0x0xebc4
    ebd8:	81 0a       	cmpa	#0xa
    ebda:	25 04       	bcs	0x0xebe0
    ebdc:	86 06       	ldaa	#0x6
    ebde:	20 e4       	bra	0x0xebc4
    ebe0:	81 06       	cmpa	#0x6
    ebe2:	25 e6       	bcs	0x0xebca
    ebe4:	86 02       	ldaa	#0x2
    ebe6:	20 dc       	bra	0x0xebc4
    ebe8:	81 1e       	cmpa	#0x1e
    ebea:	25 04       	bcs	0x0xebf0
    ebec:	86 1a       	ldaa	#0x1a
    ebee:	20 d4       	bra	0x0xebc4
    ebf0:	81 1a       	cmpa	#0x1a
    ebf2:	25 04       	bcs	0x0xebf8
    ebf4:	86 16       	ldaa	#0x16
    ebf6:	20 cc       	bra	0x0xebc4
    ebf8:	81 16       	cmpa	#0x16
    ebfa:	25 ce       	bcs	0x0xebca
    ebfc:	86 12       	ldaa	#0x12
    ebfe:	20 c4       	bra	0x0xebc4
    ec00:	ce 10 23    	ldx	#0x1023
    ec03:	1f 00 10 f9 	brclr	0x0,x, #0x10, 0x0xec00
    ec07:	86 04       	ldaa	#0x4
    ec09:	b7 01 1c    	staa	0x11c
    ec0c:	86 38       	ldaa	#0x38
    ec0e:	b7 10 47    	staa	0x1047
    ec11:	86 80       	ldaa	#0x80
    ec13:	ba 10 00    	oraa	0x1000
    ec16:	b7 10 00    	staa	0x1000
    ec19:	01          	nop
    ec1a:	88 80       	eora	#0x80
    ec1c:	b7 10 00    	staa	0x1000
    ec1f:	96 10       	ldaa	*0x10
    ec21:	b7 10 23    	staa	0x1023
    ec24:	fc 10 0e    	ldd	0x100e
    ec27:	c3 20 08    	addd	#0x2008
    ec2a:	fd 10 1c    	std	0x101c
    ec2d:	ce 10 23    	ldx	#0x1023
    ec30:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec30
    ec34:	7a 01 1c    	dec	0x11c
    ec37:	26 d3       	bne	0x0xec0c
    ec39:	86 08       	ldaa	#0x8
    ec3b:	b7 10 47    	staa	0x1047
    ec3e:	86 80       	ldaa	#0x80
    ec40:	ba 10 00    	oraa	0x1000
    ec43:	b7 10 00    	staa	0x1000
    ec46:	01          	nop
    ec47:	88 80       	eora	#0x80
    ec49:	b7 10 00    	staa	0x1000
    ec4c:	86 10       	ldaa	#0x10
    ec4e:	b7 10 23    	staa	0x1023
    ec51:	fc 10 0e    	ldd	0x100e
    ec54:	c3 00 f0    	addd	#0xf0
    ec57:	fd 10 1c    	std	0x101c
    ec5a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec5a
    ec5e:	86 01       	ldaa	#0x1
    ec60:	b7 10 47    	staa	0x1047
    ec63:	86 80       	ldaa	#0x80
    ec65:	ba 10 00    	oraa	0x1000
    ec68:	b7 10 00    	staa	0x1000
    ec6b:	01          	nop
    ec6c:	88 80       	eora	#0x80
    ec6e:	b7 10 00    	staa	0x1000
    ec71:	86 10       	ldaa	#0x10
    ec73:	b7 10 23    	staa	0x1023
    ec76:	fc 10 0e    	ldd	0x100e
    ec79:	c3 26 48    	addd	#0x2648
    ec7c:	fd 10 1c    	std	0x101c
    ec7f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xec7f
    ec83:	86 04       	ldaa	#0x4
    ec85:	b7 10 47    	staa	0x1047
    ec88:	86 80       	ldaa	#0x80
    ec8a:	ba 10 00    	oraa	0x1000
    ec8d:	b7 10 00    	staa	0x1000
    ec90:	01          	nop
    ec91:	88 80       	eora	#0x80
    ec93:	b7 10 00    	staa	0x1000
    ec96:	86 10       	ldaa	#0x10
    ec98:	b7 10 23    	staa	0x1023
    ec9b:	fc 10 0e    	ldd	0x100e
    ec9e:	c3 00 f0    	addd	#0xf0
    eca1:	fd 10 1c    	std	0x101c
    eca4:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xeca4
    eca8:	86 0c       	ldaa	#0xc
    ecaa:	b7 10 47    	staa	0x1047
    ecad:	b6 10 00    	ldaa	0x1000
    ecb0:	8a 80       	oraa	#0x80
    ecb2:	b7 10 00    	staa	0x1000
    ecb5:	88 80       	eora	#0x80
    ecb7:	b7 10 00    	staa	0x1000
    ecba:	7f 01 1d    	clr	0x11d
    ecbd:	86 10       	ldaa	#0x10
    ecbf:	b7 10 23    	staa	0x1023
    ecc2:	fc 10 0e    	ldd	0x100e
    ecc5:	c3 00 f0    	addd	#0xf0
    ecc8:	fd 10 1c    	std	0x101c
    eccb:	ce 01 20    	ldx	#0x120
    ecce:	18 ce ed a0 	ldy	#0xeda0
    ecd2:	c6 20       	ldab	#0x20
    ecd4:	18 a6 00    	ldaa	0x0,y
    ecd7:	a7 00       	staa	0x0,x
    ecd9:	08          	inx
    ecda:	18 08       	iny
    ecdc:	5a          	decb
    ecdd:	26 f5       	bne	0x0xecd4
    ecdf:	7f 01 1e    	clr	0x11e
    ece2:	86 20       	ldaa	#0x20
    ece4:	b7 01 1c    	staa	0x11c
    ece7:	ce 10 23    	ldx	#0x1023
    ecea:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xecea
    ecee:	86 cf       	ldaa	#0xcf
    ecf0:	b7 10 47    	staa	0x1047
    ecf3:	86 80       	ldaa	#0x80
    ecf5:	ba 10 00    	oraa	0x1000
    ecf8:	b7 10 00    	staa	0x1000
    ecfb:	01          	nop
    ecfc:	88 80       	eora	#0x80
    ecfe:	b7 10 00    	staa	0x1000
    ed01:	86 10       	ldaa	#0x10
    ed03:	b7 10 23    	staa	0x1023
    ed06:	fc 10 0e    	ldd	0x100e
    ed09:	c3 00 f0    	addd	#0xf0
    ed0c:	fd 10 1c    	std	0x101c
    ed0f:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xed0f
    ed13:	ce 01 20    	ldx	#0x120
    ed16:	f6 01 1c    	ldab	0x11c
    ed19:	5a          	decb
    ed1a:	2a 22       	bpl	0x0xed3e
    ed1c:	86 cf       	ldaa	#0xcf
    ed1e:	b7 10 47    	staa	0x1047
    ed21:	86 80       	ldaa	#0x80
    ed23:	ba 10 00    	oraa	0x1000
    ed26:	b7 10 00    	staa	0x1000
    ed29:	01          	nop
    ed2a:	88 80       	eora	#0x80
    ed2c:	b7 10 00    	staa	0x1000
    ed2f:	86 10       	ldaa	#0x10
    ed31:	b7 10 23    	staa	0x1023
    ed34:	fc 10 0e    	ldd	0x100e
    ed37:	c3 00 f0    	addd	#0xf0
    ed3a:	fd 10 1c    	std	0x101c
    ed3d:	39          	rts
    ed3e:	3a          	abx
    ed3f:	a6 00       	ldaa	0x0,x
    ed41:	b7 10 47    	staa	0x1047
    ed44:	86 88       	ldaa	#0x88
    ed46:	ba 10 00    	oraa	0x1000
    ed49:	b7 10 00    	staa	0x1000
    ed4c:	88 80       	eora	#0x80
    ed4e:	b7 10 00    	staa	0x1000
    ed51:	88 08       	eora	#0x8
    ed53:	b7 10 00    	staa	0x1000
    ed56:	86 10       	ldaa	#0x10
    ed58:	b7 10 23    	staa	0x1023
    ed5b:	37          	pshb
    ed5c:	fc 10 0e    	ldd	0x100e
    ed5f:	c3 00 f0    	addd	#0xf0
    ed62:	fd 10 1c    	std	0x101c
    ed65:	33          	pulb
    ed66:	f7 01 1c    	stab	0x11c
    ed69:	c1 10       	cmpb	#0x10
    ed6b:	27 02       	beq	0x0xed6f
    ed6d:	20 a4       	bra	0x0xed13
    ed6f:	18 ce 10 23 	ldy	#0x1023
    ed73:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xed73
    ed77:	fb 
    ed78:	86 8f       	ldaa	#0x8f
    ed7a:	b7 10 47    	staa	0x1047
    ed7d:	86 80       	ldaa	#0x80
    ed7f:	ba 10 00    	oraa	0x1000
    ed82:	b7 10 00    	staa	0x1000
    ed85:	88 80       	eora	#0x80
    ed87:	b7 10 00    	staa	0x1000
    ed8a:	86 10       	ldaa	#0x10
    ed8c:	b7 10 23    	staa	0x1023
    ed8f:	fc 10 0e    	ldd	0x100e
    ed92:	c3 00 f0    	addd	#0xf0
    ed95:	fd 10 1c    	std	0x101c
    ed98:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xed98
    ed9c:	fb 
    ed9d:	7e ed 13    	jmp	0xed13
    eda0:	53          	comb
    eda1:	54          	lsrb
    eda2:	55          	.byte	0x55
    eda3:	44          	lsra
    eda4:	49          	rola
    eda5:	4f          	clra
    eda6:	20 20       	bra	0x0xedc8
    eda8:	20 20       	bra	0x0xedca
    edaa:	4f          	clra
    edab:	4d          	tsta
    edac:	45          	.byte	0x45
    edad:	47          	asra
    edae:	41          	.byte	0x41
    edaf:	20 45       	bra	0x0xedf6
    edb1:	4c          	inca
    edb2:	45          	.byte	0x45
    edb3:	43          	coma
    edb4:	54          	lsrb
    edb5:	52          	.byte	0x52
    edb6:	4f          	clra
    edb7:	4e          	.byte	0x4e
    edb8:	49          	rola
    edb9:	43          	coma
    edba:	53          	comb
    edbb:	20 43       	bra	0x0xee00
    edbd:	31          	ins
    edbe:	2e 35       	bgt	0x0xedf5
    edc0:	00          	bgnd
    edc1:	01          	nop
    edc2:	01          	nop
    edc3:	01          	nop
    edc4:	01          	nop
    edc5:	02          	idiv
    edc6:	02          	idiv
    edc7:	02          	idiv
    edc8:	02          	idiv
    edc9:	03          	fdiv
    edca:	03          	fdiv
    edcb:	04          	lsrd
    edcc:	04          	lsrd
    edcd:	05          	asld
    edce:	05          	asld
    edcf:	05          	asld
    edd0:	06          	tap
    edd1:	06          	tap
    edd2:	06          	tap
    edd3:	07          	tpa
    edd4:	07          	tpa
    edd5:	07          	tpa
    edd6:	08          	inx
    edd7:	08          	inx
    edd8:	08          	inx
    edd9:	09          	dex
    edda:	09          	dex
    eddb:	0a          	clv
    eddc:	0a          	clv
    eddd:	0b          	sev
    edde:	0b          	sev
    eddf:	0c          	clc
    ede0:	0c          	clc
    ede1:	0d          	sec
    ede2:	0d          	sec
    ede3:	0e          	cli
    ede4:	0e          	cli
    ede5:	0f          	sei
    ede6:	10          	sba
    ede7:	11          	cba
    ede8:	11          	cba
    ede9:	12 12 13 13 	brset	*0x12, #0x13, 0x0xee00
    eded:	14 14 15    	bset	*0x14, #0x15
    edf0:	15 16 17    	bclr	*0x16, #0x17
    edf3:	18 18       	.byte	0x18, 0x18
    edf5:	19          	daa
    edf6:	19          	daa
    edf7:	1a 1b       	.byte	0x1a, 0x1b
    edf9:	1c 1d 1e    	bset	0x1d,x, #0x1e
    edfc:	1f 20 21 22 	brclr	0x20,x, #0x21, 0x0xee22
    ee00:	23 24       	bls	0x0xee26
    ee02:	25 26       	bcs	0x0xee2a
    ee04:	27 28       	beq	0x0xee2e
    ee06:	29 2a       	bvs	0x0xee32
    ee08:	2b 2c       	bmi	0x0xee36
    ee0a:	2d 2e       	blt	0x0xee3a
    ee0c:	30          	tsx
    ee0d:	31          	ins
    ee0e:	32          	pula
    ee0f:	33          	pulb
    ee10:	34          	des
    ee11:	35          	txs
    ee12:	36          	psha
    ee13:	37          	pshb
    ee14:	38          	pulx
    ee15:	39          	rts
    ee16:	3a          	abx
    ee17:	3b          	rti
    ee18:	3c          	pshx
    ee19:	3d          	mul
    ee1a:	3f          	swi
    ee1b:	41          	.byte	0x41
    ee1c:	42          	.byte	0x42
    ee1d:	43          	coma
    ee1e:	44          	lsra
    ee1f:	45          	.byte	0x45
    ee20:	47          	asra
    ee21:	49          	rola
    ee22:	4a          	deca
    ee23:	4c          	inca
    ee24:	4e          	.byte	0x4e
    ee25:	50          	negb
    ee26:	52          	.byte	0x52
    ee27:	54          	lsrb
    ee28:	56          	rorb
    ee29:	58          	aslb
    ee2a:	5a          	decb
    ee2b:	5c          	incb
    ee2c:	5e          	.byte	0x5e
    ee2d:	60 61       	neg	0x61,x
    ee2f:	63 64       	com	0x64,x
    ee31:	66 68       	ror	0x68,x
    ee33:	6a 6c       	dec	0x6c,x
    ee35:	6e 70       	jmp	0x70,x
    ee37:	72          	.byte	0x72
    ee38:	74 76 78    	lsr	0x7678
    ee3b:	7a 7c 7d    	dec	0x7c7d
    ee3e:	7e 7f 00    	jmp	0x7f00
    ee41:	00          	bgnd
    ee42:	40          	nega
    ee43:	00          	bgnd
    ee44:	00          	bgnd
    ee45:	00          	bgnd
    ee46:	00          	bgnd
    ee47:	00          	bgnd
    ee48:	5d          	tstb
    ee49:	00          	bgnd
    ee4a:	00          	bgnd
    ee4b:	00          	bgnd
    ee4c:	00          	bgnd
    ee4d:	03          	fdiv
    ee4e:	00          	bgnd
    ee4f:	00          	bgnd
    ee50:	00          	bgnd
    ee51:	00          	bgnd
    ee52:	00          	bgnd
    ee53:	00          	bgnd
    ee54:	01          	nop
    ee55:	53          	comb
    ee56:	00          	bgnd
    ee57:	00          	bgnd
    ee58:	00          	bgnd
    ee59:	00          	bgnd
    ee5a:	10          	sba
    ee5b:	00          	bgnd
    ee5c:	00          	bgnd
    ee5d:	00          	bgnd
    ee5e:	00          	bgnd
    ee5f:	00          	bgnd
    ee60:	40          	nega
    ee61:	00          	bgnd
    ee62:	2c 3d       	bge	0x0xeea1
    ee64:	22 7f       	bhi	0x0xeee5
    ee66:	7f 00 00    	clr	0x0
    ee69:	00          	bgnd
    ee6a:	01          	nop
    ee6b:	00          	bgnd
    ee6c:	00          	bgnd
    ee6d:	00          	bgnd
    ee6e:	00          	bgnd
    ee6f:	7f 00 00    	clr	0x0
    ee72:	3d          	mul
    ee73:	57          	asrb
    ee74:	00          	bgnd
    ee75:	40          	nega
    ee76:	00          	bgnd
    ee77:	2d 3a       	blt	0x0xeeb3
    ee79:	00          	bgnd
    ee7a:	00          	bgnd
    ee7b:	00          	bgnd
    ee7c:	5a          	decb
    ee7d:	40          	nega
    ee7e:	00          	bgnd
    ee7f:	00          	bgnd
    ee80:	02          	idiv
    ee81:	00          	bgnd
    ee82:	00          	bgnd
    ee83:	00          	bgnd
    ee84:	00          	bgnd
    ee85:	00          	bgnd
    ee86:	00          	bgnd
    ee87:	00          	bgnd
    ee88:	07          	tpa
    ee89:	00          	bgnd
    ee8a:	21 01       	brn	0x0xee8d
    ee8c:	40          	nega
    ee8d:	40          	nega
    ee8e:	28 4b       	bvc	0x0xeedb
    ee90:	20 0a       	bra	0x0xee9c
    ee92:	00          	bgnd
    ee93:	01          	nop
    ee94:	02          	idiv
    ee95:	00          	bgnd
    ee96:	12 00 19 00 	brset	*0x0, #0x19, 0x0xee9a
    ee9a:	10          	sba
    ee9b:	00          	bgnd
    ee9c:	00          	bgnd
    ee9d:	00          	bgnd
    ee9e:	03          	fdiv
    ee9f:	09          	dex
    eea0:	02          	idiv
    eea1:	00          	bgnd
    eea2:	11          	cba
	...
    eeab:	00          	bgnd
    eeac:	40          	nega
    eead:	40          	nega
    eeae:	00          	bgnd
    eeaf:	00          	bgnd
    eeb0:	02          	idiv
    eeb1:	02          	idiv
    eeb2:	02          	idiv
	...
    eec7:	00          	bgnd
    eec8:	07          	tpa
    eec9:	07          	tpa
    eeca:	07          	tpa
    eecb:	07          	tpa
    eecc:	07          	tpa
    eecd:	07          	tpa
    eece:	07          	tpa
    eecf:	07          	tpa
    eed0:	40          	nega
    eed1:	40          	nega
    eed2:	40          	nega
    eed3:	40          	nega
    eed4:	40          	nega
    eed5:	40          	nega
    eed6:	40          	nega
    eed7:	40          	nega
	...
    eee0:	49          	rola
    eee1:	4e          	.byte	0x4e
    eee2:	49          	rola
    eee3:	54          	lsrb
    eee4:	49          	rola
    eee5:	41          	.byte	0x41
    eee6:	4c          	inca
    eee7:	20 20       	bra	0x0xef09
    eee9:	20 20       	bra	0x0xef0b
    eeeb:	20 20       	bra	0x0xef0d
    eeed:	20 20       	bra	0x0xef0f
    eeef:	20 01       	bra	0x0xeef2
    eef1:	00          	bgnd
    eef2:	3b          	rti
    eef3:	44          	lsra
    eef4:	7f 00 01    	clr	0x1
    eef7:	44          	lsra
    eef8:	7f 00 01    	clr	0x1
    eefb:	44          	lsra
    eefc:	7f 00 01    	clr	0x1
    eeff:	44          	lsra
    ef00:	7f 00 01    	clr	0x1
    ef03:	44          	lsra
    ef04:	7f 00 01    	clr	0x1
    ef07:	44          	lsra
    ef08:	7f 00 01    	clr	0x1
    ef0b:	44          	lsra
    ef0c:	7f 00 01    	clr	0x1
    ef0f:	44          	lsra
    ef10:	7f 3c 00    	clr	0x3c00
    ef13:	00          	bgnd
    ef14:	00          	bgnd
    ef15:	00          	bgnd
    ef16:	00          	bgnd
    ef17:	00          	bgnd
    ef18:	07          	tpa
    ef19:	07          	tpa
    ef1a:	07          	tpa
    ef1b:	07          	tpa
    ef1c:	07          	tpa
    ef1d:	07          	tpa
    ef1e:	07          	tpa
    ef1f:	07          	tpa
    ef20:	22 22       	bhi	0x0xef44
    ef22:	22 22       	bhi	0x0xef46
    ef24:	5e          	.byte	0x5e
    ef25:	5e          	.byte	0x5e
    ef26:	5e          	.byte	0x5e
    ef27:	5e          	.byte	0x5e
	...
    ef30:	49          	rola
    ef31:	4e          	.byte	0x4e
    ef32:	49          	rola
    ef33:	54          	lsrb
    ef34:	49          	rola
    ef35:	41          	.byte	0x41
    ef36:	4c          	inca
    ef37:	20 20       	bra	0x0xef59
    ef39:	20 20       	bra	0x0xef5b
    ef3b:	20 20       	bra	0x0xef5d
    ef3d:	20 20       	bra	0x0xef5f
    ef3f:	20 00       	bra	0x0xef41
    ef41:	02          	idiv
    ef42:	03          	fdiv
    ef43:	05          	asld
    ef44:	0f          	sei
    ef45:	0a          	clv
    ef46:	1e 14 3c 28 	brset	0x14,x, #0x3c, 0x0xef72
    ef4a:	78 50 f0    	asl	0x50f0
    ef4d:	ff ff ff    	stx	0xffff
    ef50:	ff ff ff    	stx	0xffff
    ef53:	ff ff ff    	stx	0xffff
    ef56:	ff ff ff    	stx	0xffff
    ef59:	ff ff ff    	stx	0xffff
    ef5c:	ff ff ff    	stx	0xffff
    ef5f:	ff ff ff    	stx	0xffff
    ef62:	ff ff ff    	stx	0xffff
    ef65:	ff ff ff    	stx	0xffff
    ef68:	ff ff ff    	stx	0xffff
    ef6b:	ff ff ff    	stx	0xffff
    ef6e:	ff ff ff    	stx	0xffff
    ef71:	ff ff ff    	stx	0xffff
    ef74:	ff ff ff    	stx	0xffff
    ef77:	ff ff ff    	stx	0xffff
    ef7a:	ff ff ff    	stx	0xffff
    ef7d:	ff ff ff    	stx	0xffff
    ef80:	ff ff ff    	stx	0xffff
    ef83:	ff ff ff    	stx	0xffff
    ef86:	ff ff ff    	stx	0xffff
    ef89:	ff ff ff    	stx	0xffff
    ef8c:	ff ff ff    	stx	0xffff
    ef8f:	ff ff ff    	stx	0xffff
    ef92:	ff ff ff    	stx	0xffff
    ef95:	ff ff ff    	stx	0xffff
    ef98:	ff ff ff    	stx	0xffff
    ef9b:	ff ff ff    	stx	0xffff
    ef9e:	ff ff ff    	stx	0xffff
    efa1:	ff ff ff    	stx	0xffff
    efa4:	ff ff ff    	stx	0xffff
    efa7:	ff ff ff    	stx	0xffff
    efaa:	ff ff ff    	stx	0xffff
    efad:	ff ff ff    	stx	0xffff
    efb0:	ff ff ff    	stx	0xffff
    efb3:	ff ff ff    	stx	0xffff
    efb6:	ff ff ff    	stx	0xffff
    efb9:	ff ff ff    	stx	0xffff
    efbc:	ff ff ff    	stx	0xffff
    efbf:	ff ff ff    	stx	0xffff
    efc2:	ff ff ff    	stx	0xffff
    efc5:	ff ff ff    	stx	0xffff
    efc8:	ff ff ff    	stx	0xffff
    efcb:	ff ff ff    	stx	0xffff
    efce:	ff ff ff    	stx	0xffff
    efd1:	ff ff ff    	stx	0xffff
    efd4:	ff ff ff    	stx	0xffff
    efd7:	ff ff ff    	stx	0xffff
    efda:	ff ff ff    	stx	0xffff
    efdd:	ff ff ff    	stx	0xffff
    efe0:	ff ff ff    	stx	0xffff
    efe3:	ff ff ff    	stx	0xffff
    efe6:	ff ff ff    	stx	0xffff
    efe9:	ff ff ff    	stx	0xffff
    efec:	ff ff ff    	stx	0xffff
    efef:	ff ff ff    	stx	0xffff
    eff2:	ff ff ff    	stx	0xffff
    eff5:	ff ff ff    	stx	0xffff
    eff8:	ff ff ff    	stx	0xffff
    effb:	ff ff ff    	stx	0xffff
    effe:	ff ff ff    	stx	0xffff
    f001:	ff ff ff    	stx	0xffff
    f004:	ff ff ff    	stx	0xffff
    f007:	ff ff ff    	stx	0xffff
    f00a:	ff ff ff    	stx	0xffff
    f00d:	ff ff ff    	stx	0xffff
    f010:	ff ff ff    	stx	0xffff
    f013:	ff ff ff    	stx	0xffff
    f016:	ff ff ff    	stx	0xffff
    f019:	ff ff ff    	stx	0xffff
    f01c:	ff ff ff    	stx	0xffff
    f01f:	ff ff ff    	stx	0xffff
    f022:	ff ff ff    	stx	0xffff
    f025:	ff ff ff    	stx	0xffff
    f028:	ff ff ff    	stx	0xffff
    f02b:	ff ff ff    	stx	0xffff
    f02e:	ff ff ff    	stx	0xffff
    f031:	ff ff ff    	stx	0xffff
    f034:	ff ff ff    	stx	0xffff
    f037:	ff ff ff    	stx	0xffff
    f03a:	ff ff ff    	stx	0xffff
    f03d:	ff ff ff    	stx	0xffff
    f040:	ff ff ff    	stx	0xffff
    f043:	ff ff ff    	stx	0xffff
    f046:	ff ff ff    	stx	0xffff
    f049:	ff ff ff    	stx	0xffff
    f04c:	ff ff ff    	stx	0xffff
    f04f:	ff ff ff    	stx	0xffff
    f052:	ff ff ff    	stx	0xffff
    f055:	ff ff ff    	stx	0xffff
    f058:	ff ff ff    	stx	0xffff
    f05b:	ff ff ff    	stx	0xffff
    f05e:	ff ff ff    	stx	0xffff
    f061:	ff ff ff    	stx	0xffff
    f064:	ff ff ff    	stx	0xffff
    f067:	ff ff ff    	stx	0xffff
    f06a:	ff ff ff    	stx	0xffff
    f06d:	ff ff ff    	stx	0xffff
    f070:	ff ff ff    	stx	0xffff
    f073:	ff ff ff    	stx	0xffff
    f076:	ff ff ff    	stx	0xffff
    f079:	ff ff ff    	stx	0xffff
    f07c:	ff ff ff    	stx	0xffff
    f07f:	ff ff ff    	stx	0xffff
    f082:	ff ff ff    	stx	0xffff
    f085:	ff ff ff    	stx	0xffff
    f088:	ff ff ff    	stx	0xffff
    f08b:	ff ff ff    	stx	0xffff
    f08e:	ff ff ff    	stx	0xffff
    f091:	ff ff ff    	stx	0xffff
    f094:	ff ff ff    	stx	0xffff
    f097:	ff ff ff    	stx	0xffff
    f09a:	ff ff ff    	stx	0xffff
    f09d:	ff ff ff    	stx	0xffff
    f0a0:	ff ff ff    	stx	0xffff
    f0a3:	ff ff ff    	stx	0xffff
    f0a6:	ff ff ff    	stx	0xffff
    f0a9:	ff ff ff    	stx	0xffff
    f0ac:	ff ff ff    	stx	0xffff
    f0af:	ff ff ff    	stx	0xffff
    f0b2:	ff ff ff    	stx	0xffff
    f0b5:	ff ff ff    	stx	0xffff
    f0b8:	ff ff ff    	stx	0xffff
    f0bb:	ff ff ff    	stx	0xffff
    f0be:	ff ff ff    	stx	0xffff
    f0c1:	ff ff ff    	stx	0xffff
    f0c4:	ff ff ff    	stx	0xffff
    f0c7:	ff ff ff    	stx	0xffff
    f0ca:	ff ff ff    	stx	0xffff
    f0cd:	ff ff ff    	stx	0xffff
    f0d0:	ff ff ff    	stx	0xffff
    f0d3:	ff ff ff    	stx	0xffff
    f0d6:	ff ff ff    	stx	0xffff
    f0d9:	ff ff ff    	stx	0xffff
    f0dc:	ff ff ff    	stx	0xffff
    f0df:	ff ff ff    	stx	0xffff
    f0e2:	ff ff ff    	stx	0xffff
    f0e5:	ff ff ff    	stx	0xffff
    f0e8:	ff ff ff    	stx	0xffff
    f0eb:	ff ff ff    	stx	0xffff
    f0ee:	ff ff ff    	stx	0xffff
    f0f1:	ff ff ff    	stx	0xffff
    f0f4:	ff ff ff    	stx	0xffff
    f0f7:	ff ff ff    	stx	0xffff
    f0fa:	ff ff ff    	stx	0xffff
    f0fd:	ff ff ff    	stx	0xffff
    f100:	ff ff ff    	stx	0xffff
    f103:	ff ff ff    	stx	0xffff
    f106:	ff ff ff    	stx	0xffff
    f109:	ff ff ff    	stx	0xffff
    f10c:	ff ff ff    	stx	0xffff
    f10f:	ff ff ff    	stx	0xffff
    f112:	ff ff ff    	stx	0xffff
    f115:	ff ff ff    	stx	0xffff
    f118:	ff ff ff    	stx	0xffff
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
