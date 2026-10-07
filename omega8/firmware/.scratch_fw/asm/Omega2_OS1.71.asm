
/home/user/.scratch_fw/images/Omega2_OS1.71.decoded.bin:     file format binary


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
    803c:	c3 ea 60    	addd	#0xea60
    803f:	fd 10 1c    	std	0x101c
    8042:	4f          	clra
    8043:	ce 01 ff    	ldx	#0x1ff
    8046:	a7 00       	staa	0x0,x
    8048:	09          	dex
    8049:	26 fb       	bne	0x0x8046
    804b:	97 00       	staa	*0x0
    804d:	7c 01 72    	inc	0x172
    8050:	ce 00 f2    	ldx	#0xf2
    8053:	ff 01 73    	stx	0x173
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
    8070:	b7 01 75    	staa	0x175
    8073:	b7 01 18    	staa	0x118
    8076:	86 09       	ldaa	#0x9
    8078:	b7 10 46    	staa	0x1046
    807b:	bd e0 ba    	jsr	0xe0ba
    807e:	cc 01 01    	ldd	#0x101
    8081:	fd 01 10    	std	0x110
    8084:	cc 01 09    	ldd	#0x109
    8087:	fd 01 13    	std	0x113
    808a:	cc 01 80    	ldd	#0x180
    808d:	fd 01 c0    	std	0x1c0
    8090:	fd 01 c2    	std	0x1c2
    8093:	cc 01 40    	ldd	#0x140
    8096:	fd 01 62    	std	0x162
    8099:	fd 01 60    	std	0x160
    809c:	7c 00 d7    	inc	0xd7
    809f:	c6 07       	ldab	#0x7
    80a1:	ce ff ff    	ldx	#0xffff
    80a4:	09          	dex
    80a5:	26 fd       	bne	0x0x80a4
    80a7:	5a          	decb
    80a8:	26 fa       	bne	0x0x80a4
    80aa:	86 70       	ldaa	#0x70
    80ac:	b7 10 43    	staa	0x1043
    80af:	7c 01 17    	inc	0x117
    80b2:	20 3e       	bra	0x0x80f2
    80b4:	ce 10 23    	ldx	#0x1023
    80b7:	1f 00 20 f9 	brclr	0x0,x, #0x20, 0x0x80b4
    80bb:	7d 01 17    	tst	0x117
    80be:	27 0a       	beq	0x0x80ca
    80c0:	4f          	clra
    80c1:	b7 01 17    	staa	0x117
    80c4:	b7 10 30    	staa	0x1030
    80c7:	7e 80 f2    	jmp	0x80f2
    80ca:	b6 10 34    	ldaa	0x1034
    80cd:	44          	lsra
    80ce:	f6 01 16    	ldab	0x116
    80d1:	ce 00 00    	ldx	#0x0
    80d4:	3a          	abx
    80d5:	a7 00       	staa	0x0,x
    80d7:	5c          	incb
    80d8:	c1 10       	cmpb	#0x10
    80da:	27 27       	beq	0x0x8103
    80dc:	f7 01 16    	stab	0x116
    80df:	7c 01 17    	inc	0x117
    80e2:	c1 07       	cmpb	#0x7
    80e4:	22 07       	bhi	0x0x80ed
    80e6:	ca 70       	orab	#0x70
    80e8:	f7 10 43    	stab	0x1043
    80eb:	20 05       	bra	0x0x80f2
    80ed:	ca 68       	orab	#0x68
    80ef:	f7 10 43    	stab	0x1043
    80f2:	86 20       	ldaa	#0x20
    80f4:	b7 10 23    	staa	0x1023
    80f7:	fc 10 0e    	ldd	0x100e
    80fa:	c3 0f a0    	addd	#0xfa0
    80fd:	fd 10 1a    	std	0x101a
    8100:	7e 80 b4    	jmp	0x80b4
    8103:	86 70       	ldaa	#0x70
    8105:	b7 10 43    	staa	0x1043
    8108:	86 01       	ldaa	#0x1
    810a:	b7 01 17    	staa	0x117
    810d:	7f 01 16    	clr	0x116
    8110:	86 3a       	ldaa	#0x3a
    8112:	b7 10 09    	staa	0x1009
    8115:	86 5e       	ldaa	#0x5e
    8117:	b7 10 28    	staa	0x1028
    811a:	b6 7f fd    	ldaa	0x7ffd
    811d:	b7 01 6e    	staa	0x16e
    8120:	b6 7f fc    	ldaa	0x7ffc
    8123:	81 32       	cmpa	#0x32
    8125:	23 02       	bls	0x0x8129
    8127:	86 0f       	ldaa	#0xf
    8129:	b7 01 71    	staa	0x171
    812c:	b7 10 46    	staa	0x1046
    812f:	b6 7f ff    	ldaa	0x7fff
    8132:	b7 01 70    	staa	0x170
    8135:	81 02       	cmpa	#0x2
    8137:	23 06       	bls	0x0x813f
    8139:	7f 01 70    	clr	0x170
    813c:	7f 7f ff    	clr	0x7fff
    813f:	b6 7f fe    	ldaa	0x7ffe
    8142:	b7 01 6f    	staa	0x16f
    8145:	81 0f       	cmpa	#0xf
    8147:	23 07       	bls	0x0x8150
    8149:	4f          	clra
    814a:	b7 01 6f    	staa	0x16f
    814d:	b7 7f fe    	staa	0x7ffe
    8150:	7c 00 d0    	inc	0xd0
    8153:	bd 9d 9c    	jsr	0x9d9c
    8156:	14 df 80    	bset	*0xdf, #0x80
    8159:	86 7f       	ldaa	#0x7f
    815b:	97 d5       	staa	*0xd5
    815d:	97 d6       	staa	*0xd6
    815f:	c6 ac       	ldab	#0xac
    8161:	bd a8 5a    	jsr	0xa85a
    8164:	86 15       	ldaa	#0x15
    8166:	97 f8       	staa	*0xf8
    8168:	3f          	swi
    8169:	86 80       	ldaa	#0x80
    816b:	b7 10 23    	staa	0x1023
    816e:	b7 01 1c    	staa	0x11c
    8171:	fc 10 0e    	ldd	0x100e
    8174:	c3 4e 20    	addd	#0x4e20
    8177:	fd 10 16    	std	0x1016
    817a:	86 80       	ldaa	#0x80
    817c:	b7 10 22    	staa	0x1022
    817f:	0e          	cli
    8180:	bd 8e bc    	jsr	0x8ebc
    8183:	4d          	tsta
    8184:	2b 02       	bmi	0x0x8188
    8186:	20 f8       	bra	0x0x8180
    8188:	16          	tab
    8189:	c4 f0       	andb	#0xf0
    818b:	c1 90       	cmpb	#0x90
    818d:	27 2a       	beq	0x0x81b9
    818f:	c1 80       	cmpb	#0x80
    8191:	26 03       	bne	0x0x8196
    8193:	7e 86 a9    	jmp	0x86a9
    8196:	c1 b0       	cmpb	#0xb0
    8198:	26 03       	bne	0x0x819d
    819a:	7e 8a 3d    	jmp	0x8a3d
    819d:	c1 e0       	cmpb	#0xe0
    819f:	26 03       	bne	0x0x81a4
    81a1:	7e 8b a3    	jmp	0x8ba3
    81a4:	c1 d0       	cmpb	#0xd0
    81a6:	26 03       	bne	0x0x81ab
    81a8:	7e 8b c7    	jmp	0x8bc7
    81ab:	c1 c0       	cmpb	#0xc0
    81ad:	26 03       	bne	0x0x81b2
    81af:	7e 8b f7    	jmp	0x8bf7
    81b2:	81 f0       	cmpa	#0xf0
    81b4:	26 ca       	bne	0x0x8180
    81b6:	7e 8c 4f    	jmp	0x8c4f
    81b9:	97 dc       	staa	*0xdc
    81bb:	7c 00 d1    	inc	0xd1
    81be:	96 df       	ldaa	*0xdf
    81c0:	2b 0c       	bmi	0x0x81ce
    81c2:	26 03       	bne	0x0x81c7
    81c4:	7e 83 22    	jmp	0x8322
    81c7:	81 02       	cmpa	#0x2
    81c9:	27 11       	beq	0x0x81dc
    81cb:	7e 84 93    	jmp	0x8493
    81ce:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x81d5
    81d2:	7e 89 5b    	jmp	0x895b
    81d5:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x81dc
    81d9:	7e 87 d0    	jmp	0x87d0
    81dc:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x81e3
    81e0:	7e 89 5b    	jmp	0x895b
    81e3:	bd 8e bc    	jsr	0x8ebc
    81e6:	13 f0 01 03 	brclr	*0xf0, #0x01, 0x0x81ed
    81ea:	7e 82 71    	jmp	0x8271
    81ed:	36          	psha
    81ee:	bd 8e bc    	jsr	0x8ebc
    81f1:	4d          	tsta
    81f2:	26 0f       	bne	0x0x8203
    81f4:	13 95 02 07 	brclr	*0x95, #0x02, 0x0x81ff
    81f8:	33          	pulb
    81f9:	bd 9f e9    	jsr	0x9fe9
    81fc:	7e 83 04    	jmp	0x8304
    81ff:	32          	pula
    8200:	7e 83 04    	jmp	0x8304
    8203:	d6 f9       	ldab	*0xf9
    8205:	c1 02       	cmpb	#0x2
    8207:	26 04       	bne	0x0x820d
    8209:	c6 03       	ldab	#0x3
    820b:	20 1c       	bra	0x0x8229
    820d:	13 95 02 07 	brclr	*0x95, #0x02, 0x0x8218
    8211:	33          	pulb
    8212:	37          	pshb
    8213:	36          	psha
    8214:	bd 9f e9    	jsr	0x9fe9
    8217:	32          	pula
    8218:	bd 82 1d    	jsr	0x821d
    821b:	20 0c       	bra	0x0x8229
    821d:	d6 6a       	ldab	*0x6a
    821f:	c4 07       	andb	#0x7
    8221:	26 03       	bne	0x0x8226
    8223:	c6 01       	ldab	#0x1
    8225:	39          	rts
    8226:	c6 03       	ldab	#0x3
    8228:	39          	rts
    8229:	d7 f0       	stab	*0xf0
    822b:	53          	comb
    822c:	36          	psha
    822d:	7d 00 d0    	tst	0xd0
    8230:	27 05       	beq	0x0x8237
    8232:	7f 00 d0    	clr	0xd0
    8235:	20 08       	bra	0x0x823f
    8237:	7d 10 29    	tst	0x1029
    823a:	2a fb       	bpl	0x0x8237
    823c:	b6 10 2a    	ldaa	0x102a
    823f:	f7 10 42    	stab	0x1042
    8242:	01          	nop
    8243:	01          	nop
    8244:	32          	pula
    8245:	c6 81       	ldab	#0x81
    8247:	f7 10 2a    	stab	0x102a
    824a:	7d 10 29    	tst	0x1029
    824d:	2a fb       	bpl	0x0x824a
    824f:	f6 10 2a    	ldab	0x102a
    8252:	16          	tab
    8253:	32          	pula
    8254:	b7 10 2a    	staa	0x102a
    8257:	12 dc 01 05 	brset	*0xdc, #0x01, 0x0x8260
    825b:	b7 01 00    	staa	0x100
    825e:	20 03       	bra	0x0x8263
    8260:	b7 01 08    	staa	0x108
    8263:	7d 10 29    	tst	0x1029
    8266:	2a fb       	bpl	0x0x8263
    8268:	b6 10 2a    	ldaa	0x102a
    826b:	f7 10 2a    	stab	0x102a
    826e:	7e 83 04    	jmp	0x8304
    8271:	7d 00 69    	tst	0x69
    8274:	26 30       	bne	0x0x82a6
    8276:	36          	psha
    8277:	bd 8e bc    	jsr	0x8ebc
    827a:	4d          	tsta
    827b:	26 0d       	bne	0x0x828a
    827d:	13 95 02 05 	brclr	*0x95, #0x02, 0x0x8286
    8281:	33          	pulb
    8282:	37          	pshb
    8283:	bd 9f e9    	jsr	0x9fe9
    8286:	32          	pula
    8287:	7e 86 f2    	jmp	0x86f2
    828a:	7d 00 fd    	tst	0xfd
    828d:	27 06       	beq	0x0x8295
    828f:	7f 00 fd    	clr	0xfd
    8292:	7e 82 03    	jmp	0x8203
    8295:	fe 01 10    	ldx	0x110
    8298:	f6 01 00    	ldab	0x100
    829b:	e7 00       	stab	0x0,x
    829d:	bd 86 86    	jsr	0x8686
    82a0:	ff 01 10    	stx	0x110
    82a3:	7e 82 03    	jmp	0x8203
    82a6:	36          	psha
    82a7:	bd 8e bc    	jsr	0x8ebc
    82aa:	4d          	tsta
    82ab:	26 0d       	bne	0x0x82ba
    82ad:	13 95 02 05 	brclr	*0x95, #0x02, 0x0x82b6
    82b1:	33          	pulb
    82b2:	37          	pshb
    82b3:	bd 9f e9    	jsr	0x9fe9
    82b6:	32          	pula
    82b7:	7e 87 51    	jmp	0x8751
    82ba:	13 95 02 07 	brclr	*0x95, #0x02, 0x0x82c5
    82be:	33          	pulb
    82bf:	37          	pshb
    82c0:	36          	psha
    82c1:	bd 9f e9    	jsr	0x9fe9
    82c4:	32          	pula
    82c5:	16          	tab
    82c6:	32          	pula
    82c7:	b1 01 00    	cmpa	0x100
    82ca:	2b 04       	bmi	0x0x82d0
    82cc:	8d 19       	bsr	0x0x82e7
    82ce:	20 34       	bra	0x0x8304
    82d0:	7d 00 fd    	tst	0xfd
    82d3:	27 07       	beq	0x0x82dc
    82d5:	7f 00 fd    	clr	0xfd
    82d8:	36          	psha
    82d9:	7e 82 18    	jmp	0x8218
    82dc:	36          	psha
    82dd:	37          	pshb
    82de:	b6 01 00    	ldaa	0x100
    82e1:	8d 04       	bsr	0x0x82e7
    82e3:	32          	pula
    82e4:	7e 82 18    	jmp	0x8218
    82e7:	fe 01 10    	ldx	0x110
    82ea:	c6 01       	ldab	#0x1
    82ec:	f7 01 12    	stab	0x112
    82ef:	6d 00       	tst	0x0,x
    82f1:	27 0b       	beq	0x0x82fe
    82f3:	bd 86 86    	jsr	0x8686
    82f6:	78 01 12    	asl	0x112
    82f9:	24 f4       	bcc	0x0x82ef
    82fb:	bd 86 86    	jsr	0x8686
    82fe:	a7 00       	staa	0x0,x
    8300:	ff 01 10    	stx	0x110
    8303:	39          	rts
    8304:	7f 00 d1    	clr	0xd1
    8307:	bd 86 0e    	jsr	0x860e
    830a:	bd 8e bc    	jsr	0x8ebc
    830d:	4d          	tsta
    830e:	2a 03       	bpl	0x0x8313
    8310:	7e 81 88    	jmp	0x8188
    8313:	7c 00 d1    	inc	0xd1
    8316:	c6 80       	ldab	#0x80
    8318:	d1 dc       	cmpb	*0xdc
    831a:	27 03       	beq	0x0x831f
    831c:	7e 81 e6    	jmp	0x81e6
    831f:	7e 86 d6    	jmp	0x86d6
    8322:	bd 8e bc    	jsr	0x8ebc
    8325:	12 dc 01 07 	brset	*0xdc, #0x01, 0x0x8330
    8329:	13 f0 01 0a 	brclr	*0xf0, #0x01, 0x0x8337
    832d:	7e 83 93    	jmp	0x8393
    8330:	13 f0 02 03 	brclr	*0xf0, #0x02, 0x0x8337
    8334:	7e 83 93    	jmp	0x8393
    8337:	36          	psha
    8338:	bd 8e bc    	jsr	0x8ebc
    833b:	4d          	tsta
    833c:	26 04       	bne	0x0x8342
    833e:	32          	pula
    833f:	7e 84 73    	jmp	0x8473
    8342:	33          	pulb
    8343:	36          	psha
    8344:	12 dc 01 07 	brset	*0xdc, #0x01, 0x0x834f
    8348:	86 01       	ldaa	#0x1
    834a:	f7 01 00    	stab	0x100
    834d:	20 05       	bra	0x0x8354
    834f:	86 02       	ldaa	#0x2
    8351:	f7 01 08    	stab	0x108
    8354:	36          	psha
    8355:	9a f0       	oraa	*0xf0
    8357:	97 f0       	staa	*0xf0
    8359:	32          	pula
    835a:	43          	coma
    835b:	37          	pshb
    835c:	7d 00 d0    	tst	0xd0
    835f:	27 05       	beq	0x0x8366
    8361:	7f 00 d0    	clr	0xd0
    8364:	20 08       	bra	0x0x836e
    8366:	7d 10 29    	tst	0x1029
    8369:	2a fb       	bpl	0x0x8366
    836b:	f6 10 2a    	ldab	0x102a
    836e:	b7 10 42    	staa	0x1042
    8371:	01          	nop
    8372:	01          	nop
    8373:	32          	pula
    8374:	c6 81       	ldab	#0x81
    8376:	f7 10 2a    	stab	0x102a
    8379:	7d 10 29    	tst	0x1029
    837c:	2a fb       	bpl	0x0x8379
    837e:	f6 10 2a    	ldab	0x102a
    8381:	33          	pulb
    8382:	b7 10 2a    	staa	0x102a
    8385:	7d 10 29    	tst	0x1029
    8388:	2a fb       	bpl	0x0x8385
    838a:	b6 10 2a    	ldaa	0x102a
    838d:	f7 10 2a    	stab	0x102a
    8390:	7e 84 73    	jmp	0x8473
    8393:	36          	psha
    8394:	bd 8e bc    	jsr	0x8ebc
    8397:	4d          	tsta
    8398:	27 03       	beq	0x0x839d
    839a:	7e 84 41    	jmp	0x8441
    839d:	32          	pula
    839e:	12 dc 01 12 	brset	*0xdc, #0x01, 0x0x83b4
    83a2:	12 f0 01 03 	brset	*0xf0, #0x01, 0x0x83a9
    83a6:	7e 84 73    	jmp	0x8473
    83a9:	b1 01 00    	cmpa	0x100
    83ac:	27 22       	beq	0x0x83d0
    83ae:	bd 86 3c    	jsr	0x863c
    83b1:	7e 84 73    	jmp	0x8473
    83b4:	12 f0 02 03 	brset	*0xf0, #0x02, 0x0x83bb
    83b8:	7e 84 73    	jmp	0x8473
    83bb:	b1 01 08    	cmpa	0x108
    83be:	27 06       	beq	0x0x83c6
    83c0:	bd 86 56    	jsr	0x8656
    83c3:	7e 84 73    	jmp	0x8473
    83c6:	fe 01 13    	ldx	0x113
    83c9:	c6 01       	ldab	#0x1
    83cb:	f7 01 12    	stab	0x112
    83ce:	20 08       	bra	0x0x83d8
    83d0:	fe 01 10    	ldx	0x110
    83d3:	c6 01       	ldab	#0x1
    83d5:	f7 01 12    	stab	0x112
    83d8:	a6 00       	ldaa	0x0,x
    83da:	26 3d       	bne	0x0x8419
    83dc:	bd 86 71    	jsr	0x8671
    83df:	78 01 12    	asl	0x112
    83e2:	24 f4       	bcc	0x0x83d8
    83e4:	12 dc 01 0a 	brset	*0xdc, #0x01, 0x0x83f2
    83e8:	15 f0 01    	bclr	*0xf0, #0x01
    83eb:	86 fe       	ldaa	#0xfe
    83ed:	7f 01 00    	clr	0x100
    83f0:	20 08       	bra	0x0x83fa
    83f2:	15 f0 02    	bclr	*0xf0, #0x02
    83f5:	86 fd       	ldaa	#0xfd
    83f7:	7f 01 08    	clr	0x108
    83fa:	7d 00 d0    	tst	0xd0
    83fd:	27 05       	beq	0x0x8404
    83ff:	7f 00 d0    	clr	0xd0
    8402:	20 08       	bra	0x0x840c
    8404:	7d 10 29    	tst	0x1029
    8407:	2a fb       	bpl	0x0x8404
    8409:	f6 10 2a    	ldab	0x102a
    840c:	b7 10 42    	staa	0x1042
    840f:	01          	nop
    8410:	01          	nop
    8411:	86 80       	ldaa	#0x80
    8413:	b7 10 2a    	staa	0x102a
    8416:	7e 84 73    	jmp	0x8473
    8419:	6f 00       	clr	0x0,x
    841b:	bd 86 71    	jsr	0x8671
    841e:	8f          	xgdx
    841f:	c1 07       	cmpb	#0x7
    8421:	22 0f       	bhi	0x0x8432
    8423:	8f          	xgdx
    8424:	ff 01 10    	stx	0x110
    8427:	b7 01 00    	staa	0x100
    842a:	16          	tab
    842b:	4f          	clra
    842c:	36          	psha
    842d:	86 01       	ldaa	#0x1
    842f:	7e 83 54    	jmp	0x8354
    8432:	8f          	xgdx
    8433:	ff 01 13    	stx	0x113
    8436:	b7 01 08    	staa	0x108
    8439:	16          	tab
    843a:	4f          	clra
    843b:	36          	psha
    843c:	86 02       	ldaa	#0x2
    843e:	7e 83 54    	jmp	0x8354
    8441:	33          	pulb
    8442:	36          	psha
    8443:	12 dc 01 16 	brset	*0xdc, #0x01, 0x0x845d
    8447:	fe 01 10    	ldx	0x110
    844a:	bd 86 86    	jsr	0x8686
    844d:	b6 01 00    	ldaa	0x100
    8450:	a7 00       	staa	0x0,x
    8452:	ff 01 10    	stx	0x110
    8455:	f7 01 00    	stab	0x100
    8458:	86 01       	ldaa	#0x1
    845a:	7e 83 54    	jmp	0x8354
    845d:	fe 01 13    	ldx	0x113
    8460:	bd 86 86    	jsr	0x8686
    8463:	b6 01 08    	ldaa	0x108
    8466:	a7 00       	staa	0x0,x
    8468:	ff 01 13    	stx	0x113
    846b:	f7 01 08    	stab	0x108
    846e:	86 02       	ldaa	#0x2
    8470:	7e 83 54    	jmp	0x8354
    8473:	7f 00 d1    	clr	0xd1
    8476:	bd 86 0e    	jsr	0x860e
    8479:	bd 8e bc    	jsr	0x8ebc
    847c:	4d          	tsta
    847d:	2a 03       	bpl	0x0x8482
    847f:	7e 81 88    	jmp	0x8188
    8482:	7c 00 d1    	inc	0xd1
    8485:	d6 dc       	ldab	*0xdc
    8487:	c4 f0       	andb	#0xf0
    8489:	c1 80       	cmpb	#0x80
    848b:	27 03       	beq	0x0x8490
    848d:	7e 83 25    	jmp	0x8325
    8490:	7e 87 a8    	jmp	0x87a8
    8493:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x849a
    8497:	7e 89 5b    	jmp	0x895b
    849a:	bd 8e bc    	jsr	0x8ebc
    849d:	b1 50 07    	cmpa	0x5007
    84a0:	24 07       	bcc	0x0x84a9
    84a2:	13 f0 01 0a 	brclr	*0xf0, #0x01, 0x0x84b0
    84a6:	7e 85 0d    	jmp	0x850d
    84a9:	13 f0 02 03 	brclr	*0xf0, #0x02, 0x0x84b0
    84ad:	7e 85 0d    	jmp	0x850d
    84b0:	36          	psha
    84b1:	bd 8e bc    	jsr	0x8ebc
    84b4:	4d          	tsta
    84b5:	26 04       	bne	0x0x84bb
    84b7:	32          	pula
    84b8:	7e 85 f0    	jmp	0x85f0
    84bb:	33          	pulb
    84bc:	36          	psha
    84bd:	f1 50 07    	cmpb	0x5007
    84c0:	24 07       	bcc	0x0x84c9
    84c2:	86 01       	ldaa	#0x1
    84c4:	f7 01 00    	stab	0x100
    84c7:	20 05       	bra	0x0x84ce
    84c9:	86 02       	ldaa	#0x2
    84cb:	f7 01 08    	stab	0x108
    84ce:	36          	psha
    84cf:	9a f0       	oraa	*0xf0
    84d1:	97 f0       	staa	*0xf0
    84d3:	32          	pula
    84d4:	43          	coma
    84d5:	37          	pshb
    84d6:	7d 00 d0    	tst	0xd0
    84d9:	27 05       	beq	0x0x84e0
    84db:	7f 00 d0    	clr	0xd0
    84de:	20 08       	bra	0x0x84e8
    84e0:	7d 10 29    	tst	0x1029
    84e3:	2a fb       	bpl	0x0x84e0
    84e5:	f6 10 2a    	ldab	0x102a
    84e8:	b7 10 42    	staa	0x1042
    84eb:	01          	nop
    84ec:	01          	nop
    84ed:	32          	pula
    84ee:	c6 81       	ldab	#0x81
    84f0:	f7 10 2a    	stab	0x102a
    84f3:	7d 10 29    	tst	0x1029
    84f6:	2a fb       	bpl	0x0x84f3
    84f8:	f6 10 2a    	ldab	0x102a
    84fb:	33          	pulb
    84fc:	b7 10 2a    	staa	0x102a
    84ff:	7d 10 29    	tst	0x1029
    8502:	2a fb       	bpl	0x0x84ff
    8504:	b6 10 2a    	ldaa	0x102a
    8507:	f7 10 2a    	stab	0x102a
    850a:	7e 85 f0    	jmp	0x85f0
    850d:	36          	psha
    850e:	bd 8e bc    	jsr	0x8ebc
    8511:	4d          	tsta
    8512:	27 03       	beq	0x0x8517
    8514:	7e 85 bd    	jmp	0x85bd
    8517:	32          	pula
    8518:	b1 50 07    	cmpa	0x5007
    851b:	24 12       	bcc	0x0x852f
    851d:	12 f0 01 03 	brset	*0xf0, #0x01, 0x0x8524
    8521:	7e 85 f0    	jmp	0x85f0
    8524:	b1 01 00    	cmpa	0x100
    8527:	27 22       	beq	0x0x854b
    8529:	bd 86 3c    	jsr	0x863c
    852c:	7e 85 f0    	jmp	0x85f0
    852f:	12 f0 02 03 	brset	*0xf0, #0x02, 0x0x8536
    8533:	7e 85 f0    	jmp	0x85f0
    8536:	b1 01 08    	cmpa	0x108
    8539:	27 06       	beq	0x0x8541
    853b:	bd 86 56    	jsr	0x8656
    853e:	7e 85 f0    	jmp	0x85f0
    8541:	fe 01 13    	ldx	0x113
    8544:	c6 01       	ldab	#0x1
    8546:	f7 01 12    	stab	0x112
    8549:	20 08       	bra	0x0x8553
    854b:	fe 01 10    	ldx	0x110
    854e:	c6 01       	ldab	#0x1
    8550:	f7 01 12    	stab	0x112
    8553:	a6 00       	ldaa	0x0,x
    8555:	26 3e       	bne	0x0x8595
    8557:	bd 86 71    	jsr	0x8671
    855a:	78 01 12    	asl	0x112
    855d:	24 f4       	bcc	0x0x8553
    855f:	8f          	xgdx
    8560:	c1 07       	cmpb	#0x7
    8562:	22 0a       	bhi	0x0x856e
    8564:	15 f0 01    	bclr	*0xf0, #0x01
    8567:	86 fe       	ldaa	#0xfe
    8569:	7f 01 00    	clr	0x100
    856c:	20 08       	bra	0x0x8576
    856e:	15 f0 02    	bclr	*0xf0, #0x02
    8571:	86 fd       	ldaa	#0xfd
    8573:	7f 01 08    	clr	0x108
    8576:	7d 00 d0    	tst	0xd0
    8579:	27 05       	beq	0x0x8580
    857b:	7f 00 d0    	clr	0xd0
    857e:	20 08       	bra	0x0x8588
    8580:	7d 10 29    	tst	0x1029
    8583:	2a fb       	bpl	0x0x8580
    8585:	f6 10 2a    	ldab	0x102a
    8588:	b7 10 42    	staa	0x1042
    858b:	01          	nop
    858c:	01          	nop
    858d:	86 80       	ldaa	#0x80
    858f:	b7 10 2a    	staa	0x102a
    8592:	7e 85 f0    	jmp	0x85f0
    8595:	6f 00       	clr	0x0,x
    8597:	bd 86 71    	jsr	0x8671
    859a:	8f          	xgdx
    859b:	c1 07       	cmpb	#0x7
    859d:	22 0f       	bhi	0x0x85ae
    859f:	8f          	xgdx
    85a0:	ff 01 10    	stx	0x110
    85a3:	b7 01 00    	staa	0x100
    85a6:	16          	tab
    85a7:	4f          	clra
    85a8:	36          	psha
    85a9:	86 01       	ldaa	#0x1
    85ab:	7e 84 ce    	jmp	0x84ce
    85ae:	8f          	xgdx
    85af:	ff 01 13    	stx	0x113
    85b2:	b7 01 08    	staa	0x108
    85b5:	16          	tab
    85b6:	4f          	clra
    85b7:	36          	psha
    85b8:	86 02       	ldaa	#0x2
    85ba:	7e 84 ce    	jmp	0x84ce
    85bd:	33          	pulb
    85be:	36          	psha
    85bf:	f1 50 07    	cmpb	0x5007
    85c2:	24 16       	bcc	0x0x85da
    85c4:	fe 01 10    	ldx	0x110
    85c7:	bd 86 86    	jsr	0x8686
    85ca:	b6 01 00    	ldaa	0x100
    85cd:	a7 00       	staa	0x0,x
    85cf:	ff 01 10    	stx	0x110
    85d2:	f7 01 00    	stab	0x100
    85d5:	86 01       	ldaa	#0x1
    85d7:	7e 84 ce    	jmp	0x84ce
    85da:	fe 01 13    	ldx	0x113
    85dd:	bd 86 86    	jsr	0x8686
    85e0:	b6 01 08    	ldaa	0x108
    85e3:	a7 00       	staa	0x0,x
    85e5:	ff 01 13    	stx	0x113
    85e8:	f7 01 08    	stab	0x108
    85eb:	86 02       	ldaa	#0x2
    85ed:	7e 84 ce    	jmp	0x84ce
    85f0:	7f 00 d1    	clr	0xd1
    85f3:	bd 86 0e    	jsr	0x860e
    85f6:	bd 8e bc    	jsr	0x8ebc
    85f9:	4d          	tsta
    85fa:	2a 03       	bpl	0x0x85ff
    85fc:	7e 81 88    	jmp	0x8188
    85ff:	7c 00 d1    	inc	0xd1
    8602:	c6 80       	ldab	#0x80
    8604:	d1 dc       	cmpb	*0xdc
    8606:	27 03       	beq	0x0x860b
    8608:	7e 84 9d    	jmp	0x849d
    860b:	7e 87 c1    	jmp	0x87c1
    860e:	20 13       	bra	0x0x8623
    8610:	86 0e       	ldaa	#0xe
    8612:	b1 01 1b    	cmpa	0x11b
    8615:	26 0c       	bne	0x0x8623
    8617:	39          	rts
    8618:	c6 01       	ldab	#0x1
    861a:	b6 01 6b    	ldaa	0x16b
    861d:	27 04       	beq	0x0x8623
    861f:	58          	aslb
    8620:	4a          	deca
    8621:	26 fa       	bne	0x0x861d
    8623:	d6 f0       	ldab	*0xf0
    8625:	54          	lsrb
    8626:	25 05       	bcs	0x0x862d
    8628:	15 f5 20    	bclr	*0xf5, #0x20
    862b:	20 03       	bra	0x0x8630
    862d:	14 f5 20    	bset	*0xf5, #0x20
    8630:	54          	lsrb
    8631:	25 05       	bcs	0x0x8638
    8633:	15 f5 80    	bclr	*0xf5, #0x80
    8636:	20 03       	bra	0x0x863b
    8638:	14 f5 80    	bset	*0xf5, #0x80
    863b:	39          	rts
    863c:	fe 01 10    	ldx	0x110
    863f:	c6 02       	ldab	#0x2
    8641:	f7 01 12    	stab	0x112
    8644:	e6 00       	ldab	0x0,x
    8646:	27 06       	beq	0x0x864e
    8648:	11          	cba
    8649:	26 03       	bne	0x0x864e
    864b:	6f 00       	clr	0x0,x
    864d:	39          	rts
    864e:	8d 36       	bsr	0x0x8686
    8650:	78 01 12    	asl	0x112
    8653:	24 ef       	bcc	0x0x8644
    8655:	39          	rts
    8656:	fe 01 13    	ldx	0x113
    8659:	c6 02       	ldab	#0x2
    865b:	f7 01 12    	stab	0x112
    865e:	e6 00       	ldab	0x0,x
    8660:	27 06       	beq	0x0x8668
    8662:	11          	cba
    8663:	26 03       	bne	0x0x8668
    8665:	6f 00       	clr	0x0,x
    8667:	39          	rts
    8668:	bd 86 86    	jsr	0x8686
    866b:	78 01 12    	asl	0x112
    866e:	24 ee       	bcc	0x0x865e
    8670:	39          	rts
    8671:	8f          	xgdx
    8672:	c1 07       	cmpb	#0x7
    8674:	22 07       	bhi	0x0x867d
    8676:	5a          	decb
    8677:	26 02       	bne	0x0x867b
    8679:	c6 07       	ldab	#0x7
    867b:	8f          	xgdx
    867c:	39          	rts
    867d:	5a          	decb
    867e:	c1 08       	cmpb	#0x8
    8680:	22 02       	bhi	0x0x8684
    8682:	c6 0f       	ldab	#0xf
    8684:	8f          	xgdx
    8685:	39          	rts
    8686:	8f          	xgdx
    8687:	c1 07       	cmpb	#0x7
    8689:	22 09       	bhi	0x0x8694
    868b:	5c          	incb
    868c:	c1 07       	cmpb	#0x7
    868e:	23 02       	bls	0x0x8692
    8690:	c6 01       	ldab	#0x1
    8692:	8f          	xgdx
    8693:	39          	rts
    8694:	5c          	incb
    8695:	c1 10       	cmpb	#0x10
    8697:	25 02       	bcs	0x0x869b
    8699:	c6 09       	ldab	#0x9
    869b:	8f          	xgdx
    869c:	39          	rts
    869d:	e6 00       	ldab	0x0,x
    869f:	11          	cba
    86a0:	27 04       	beq	0x0x86a6
    86a2:	8d e2       	bsr	0x0x8686
    86a4:	20 f7       	bra	0x0x869d
    86a6:	6f 00       	clr	0x0,x
    86a8:	39          	rts
    86a9:	97 dc       	staa	*0xdc
    86ab:	7c 00 d1    	inc	0xd1
    86ae:	96 df       	ldaa	*0xdf
    86b0:	2b 0c       	bmi	0x0x86be
    86b2:	26 03       	bne	0x0x86b7
    86b4:	7e 87 a5    	jmp	0x87a5
    86b7:	81 02       	cmpa	#0x2
    86b9:	27 11       	beq	0x0x86cc
    86bb:	7e 87 b7    	jmp	0x87b7
    86be:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x86c5
    86c2:	7e 89 df    	jmp	0x89df
    86c5:	12 6a 20 03 	brset	*0x6a, #0x20, 0x0x86cc
    86c9:	7e 88 9f    	jmp	0x889f
    86cc:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x86d3
    86d0:	7e 89 df    	jmp	0x89df
    86d3:	bd 8e bc    	jsr	0x8ebc
    86d6:	36          	psha
    86d7:	bd 8e bc    	jsr	0x8ebc
    86da:	13 95 02 05 	brclr	*0x95, #0x02, 0x0x86e3
    86de:	33          	pulb
    86df:	37          	pshb
    86e0:	bd 9f e9    	jsr	0x9fe9
    86e3:	32          	pula
    86e4:	12 f0 01 03 	brset	*0xf0, #0x01, 0x0x86eb
    86e8:	7e 87 87    	jmp	0x8787
    86eb:	7d 00 69    	tst	0x69
    86ee:	27 02       	beq	0x0x86f2
    86f0:	20 5f       	bra	0x0x8751
    86f2:	b1 01 00    	cmpa	0x100
    86f5:	27 06       	beq	0x0x86fd
    86f7:	bd 86 3c    	jsr	0x863c
    86fa:	7e 87 87    	jmp	0x8787
    86fd:	7f 01 00    	clr	0x100
    8700:	fe 01 10    	ldx	0x110
    8703:	c6 01       	ldab	#0x1
    8705:	f7 01 12    	stab	0x112
    8708:	a6 00       	ldaa	0x0,x
    870a:	26 38       	bne	0x0x8744
    870c:	bd 86 71    	jsr	0x8671
    870f:	78 01 12    	asl	0x112
    8712:	24 f4       	bcc	0x0x8708
    8714:	7d 00 f1    	tst	0xf1
    8717:	27 08       	beq	0x0x8721
    8719:	bd 82 1d    	jsr	0x821d
    871c:	d7 fd       	stab	*0xfd
    871e:	7e 83 04    	jmp	0x8304
    8721:	7f 00 f0    	clr	0xf0
    8724:	7d 00 d0    	tst	0xd0
    8727:	27 05       	beq	0x0x872e
    8729:	7f 00 d0    	clr	0xd0
    872c:	20 08       	bra	0x0x8736
    872e:	7d 10 29    	tst	0x1029
    8731:	2a fb       	bpl	0x0x872e
    8733:	f6 10 2a    	ldab	0x102a
    8736:	5f          	clrb
    8737:	f7 10 42    	stab	0x1042
    873a:	01          	nop
    873b:	01          	nop
    873c:	86 80       	ldaa	#0x80
    873e:	b7 10 2a    	staa	0x102a
    8741:	7e 83 04    	jmp	0x8304
    8744:	6f 00       	clr	0x0,x
    8746:	36          	psha
    8747:	bd 86 71    	jsr	0x8671
    874a:	ff 01 10    	stx	0x110
    874d:	4f          	clra
    874e:	7e 82 18    	jmp	0x8218
    8751:	b1 01 00    	cmpa	0x100
    8754:	27 05       	beq	0x0x875b
    8756:	bd 86 3c    	jsr	0x863c
    8759:	20 2c       	bra	0x0x8787
    875b:	7f 01 00    	clr	0x100
    875e:	fe 01 10    	ldx	0x110
    8761:	c6 01       	ldab	#0x1
    8763:	f7 01 12    	stab	0x112
    8766:	86 80       	ldaa	#0x80
    8768:	e6 00       	ldab	0x0,x
    876a:	27 04       	beq	0x0x8770
    876c:	11          	cba
    876d:	25 01       	bcs	0x0x8770
    876f:	17          	tba
    8770:	bd 86 86    	jsr	0x8686
    8773:	78 01 12    	asl	0x112
    8776:	24 f0       	bcc	0x0x8768
    8778:	81 80       	cmpa	#0x80
    877a:	26 03       	bne	0x0x877f
    877c:	7e 87 14    	jmp	0x8714
    877f:	36          	psha
    8780:	bd 86 9d    	jsr	0x869d
    8783:	4f          	clra
    8784:	7e 82 18    	jmp	0x8218
    8787:	7f 00 d1    	clr	0xd1
    878a:	bd 86 0e    	jsr	0x860e
    878d:	bd 8e bc    	jsr	0x8ebc
    8790:	4d          	tsta
    8791:	2a 03       	bpl	0x0x8796
    8793:	7e 81 88    	jmp	0x8188
    8796:	7c 00 d1    	inc	0xd1
    8799:	c6 80       	ldab	#0x80
    879b:	d1 dc       	cmpb	*0xdc
    879d:	27 03       	beq	0x0x87a2
    879f:	7e 81 e6    	jmp	0x81e6
    87a2:	7e 86 d6    	jmp	0x86d6
    87a5:	bd 8e bc    	jsr	0x8ebc
    87a8:	36          	psha
    87a9:	bd 8e bc    	jsr	0x8ebc
    87ac:	32          	pula
    87ad:	13 f0 03 03 	brclr	*0xf0, #0x03, 0x0x87b4
    87b1:	7e 83 9e    	jmp	0x839e
    87b4:	7e 84 73    	jmp	0x8473
    87b7:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x87be
    87bb:	7e 89 df    	jmp	0x89df
    87be:	bd 8e bc    	jsr	0x8ebc
    87c1:	36          	psha
    87c2:	bd 8e bc    	jsr	0x8ebc
    87c5:	32          	pula
    87c6:	13 f0 03 03 	brclr	*0xf0, #0x03, 0x0x87cd
    87ca:	7e 85 18    	jmp	0x8518
    87cd:	7e 85 f0    	jmp	0x85f0
    87d0:	bd 8e bc    	jsr	0x8ebc
    87d3:	36          	psha
    87d4:	bd 8e bc    	jsr	0x8ebc
    87d7:	4d          	tsta
    87d8:	26 04       	bne	0x0x87de
    87da:	32          	pula
    87db:	7e 88 a7    	jmp	0x88a7
    87de:	13 95 01 38 	brclr	*0x95, #0x01, 0x0x881a
    87e2:	33          	pulb
    87e3:	36          	psha
    87e4:	37          	pshb
    87e5:	ce 01 00    	ldx	#0x100
    87e8:	6d 00       	tst	0x0,x
    87ea:	27 53       	beq	0x0x883f
    87ec:	8f          	xgdx
    87ed:	5c          	incb
    87ee:	c1 01       	cmpb	#0x1
    87f0:	22 03       	bhi	0x0x87f5
    87f2:	8f          	xgdx
    87f3:	20 f3       	bra	0x0x87e8
    87f5:	8f          	xgdx
    87f6:	96 95       	ldaa	*0x95
    87f8:	81 02       	cmpa	#0x2
    87fa:	25 08       	bcs	0x0x8804
    87fc:	33          	pulb
    87fd:	32          	pula
    87fe:	bd 9f e9    	jsr	0x9fe9
    8801:	7e 88 8a    	jmp	0x888a
    8804:	6d 00       	tst	0x0,x
    8806:	27 0b       	beq	0x0x8813
    8808:	08          	inx
    8809:	8c 01 08    	cpx	#0x108
    880c:	25 f6       	bcs	0x0x8804
    880e:	32          	pula
    880f:	33          	pulb
    8810:	7e 88 8a    	jmp	0x888a
    8813:	32          	pula
    8814:	a7 00       	staa	0x0,x
    8816:	33          	pulb
    8817:	7e 88 8a    	jmp	0x888a
    881a:	33          	pulb
    881b:	36          	psha
    881c:	37          	pshb
    881d:	fe 01 10    	ldx	0x110
    8820:	8f          	xgdx
    8821:	5c          	incb
    8822:	c1 02       	cmpb	#0x2
    8824:	25 01       	bcs	0x0x8827
    8826:	5f          	clrb
    8827:	8f          	xgdx
    8828:	c6 02       	ldab	#0x2
    882a:	6d 00       	tst	0x0,x
    882c:	27 11       	beq	0x0x883f
    882e:	8f          	xgdx
    882f:	5c          	incb
    8830:	c1 02       	cmpb	#0x2
    8832:	25 01       	bcs	0x0x8835
    8834:	5f          	clrb
    8835:	8f          	xgdx
    8836:	5a          	decb
    8837:	26 f1       	bne	0x0x882a
    8839:	86 01       	ldaa	#0x1
    883b:	c6 02       	ldab	#0x2
    883d:	20 b6       	bra	0x0x87f5
    883f:	ff 01 10    	stx	0x110
    8842:	8f          	xgdx
    8843:	86 01       	ldaa	#0x1
    8845:	5d          	tstb
    8846:	27 04       	beq	0x0x884c
    8848:	5a          	decb
    8849:	48          	asla
    884a:	20 f9       	bra	0x0x8845
    884c:	16          	tab
    884d:	9a f0       	oraa	*0xf0
    884f:	97 f0       	staa	*0xf0
    8851:	53          	comb
    8852:	fe 01 10    	ldx	0x110
    8855:	7d 00 d0    	tst	0xd0
    8858:	27 05       	beq	0x0x885f
    885a:	7f 00 d0    	clr	0xd0
    885d:	20 08       	bra	0x0x8867
    885f:	7d 10 29    	tst	0x1029
    8862:	2a fb       	bpl	0x0x885f
    8864:	b6 10 2a    	ldaa	0x102a
    8867:	f7 10 42    	stab	0x1042
    886a:	01          	nop
    886b:	86 81       	ldaa	#0x81
    886d:	b7 10 2a    	staa	0x102a
    8870:	7d 10 29    	tst	0x1029
    8873:	2a fb       	bpl	0x0x8870
    8875:	b6 10 2a    	ldaa	0x102a
    8878:	32          	pula
    8879:	a7 00       	staa	0x0,x
    887b:	b7 10 2a    	staa	0x102a
    887e:	7d 10 29    	tst	0x1029
    8881:	2a fb       	bpl	0x0x887e
    8883:	b6 10 2a    	ldaa	0x102a
    8886:	32          	pula
    8887:	b7 10 2a    	staa	0x102a
    888a:	7f 00 d1    	clr	0xd1
    888d:	bd 86 0e    	jsr	0x860e
    8890:	bd 8e bc    	jsr	0x8ebc
    8893:	4d          	tsta
    8894:	2a 03       	bpl	0x0x8899
    8896:	7e 81 88    	jmp	0x8188
    8899:	7c 00 d1    	inc	0xd1
    889c:	7e 87 d3    	jmp	0x87d3
    889f:	bd 8e bc    	jsr	0x8ebc
    88a2:	36          	psha
    88a3:	bd 8e bc    	jsr	0x8ebc
    88a6:	32          	pula
    88a7:	ce 01 00    	ldx	#0x100
    88aa:	c6 01       	ldab	#0x1
    88ac:	a1 00       	cmpa	0x0,x
    88ae:	27 17       	beq	0x0x88c7
    88b0:	08          	inx
    88b1:	58          	aslb
    88b2:	25 10       	bcs	0x0x88c4
    88b4:	c1 04       	cmpb	#0x4
    88b6:	26 f4       	bne	0x0x88ac
    88b8:	13 95 02 f0 	brclr	*0x95, #0x02, 0x0x88ac
    88bc:	16          	tab
    88bd:	4f          	clra
    88be:	bd 9f e9    	jsr	0x9fe9
    88c1:	7e 89 38    	jmp	0x8938
    88c4:	7e 89 38    	jmp	0x8938
    88c7:	7d 00 f1    	tst	0xf1
    88ca:	27 22       	beq	0x0x88ee
    88cc:	3c          	pshx
    88cd:	37          	pshb
    88ce:	36          	psha
    88cf:	8f          	xgdx
    88d0:	86 01       	ldaa	#0x1
    88d2:	5a          	decb
    88d3:	2b 03       	bmi	0x0x88d8
    88d5:	48          	asla
    88d6:	20 fa       	bra	0x0x88d2
    88d8:	36          	psha
    88d9:	94 fd       	anda	*0xfd
    88db:	27 06       	beq	0x0x88e3
    88dd:	32          	pula
    88de:	32          	pula
    88df:	33          	pulb
    88e0:	38          	pulx
    88e1:	20 cd       	bra	0x0x88b0
    88e3:	32          	pula
    88e4:	9a fd       	oraa	*0xfd
    88e6:	97 fd       	staa	*0xfd
    88e8:	32          	pula
    88e9:	33          	pulb
    88ea:	38          	pulx
    88eb:	7e 89 38    	jmp	0x8938
    88ee:	6f 00       	clr	0x0,x
    88f0:	c1 02       	cmpb	#0x2
    88f2:	23 03       	bls	0x0x88f7
    88f4:	7e 89 38    	jmp	0x8938
    88f7:	18 ce 01 02 	ldy	#0x102
    88fb:	18 a6 00    	ldaa	0x0,y
    88fe:	26 0a       	bne	0x0x890a
    8900:	18 08       	iny
    8902:	18 8c 01 08 	cpy	#0x108
    8906:	25 f3       	bcs	0x0x88fb
    8908:	20 0d       	bra	0x0x8917
    890a:	53          	comb
    890b:	4f          	clra
    890c:	36          	psha
    890d:	18 a6 00    	ldaa	0x0,y
    8910:	36          	psha
    8911:	18 6f 00    	clr	0x0,y
    8914:	7e 88 55    	jmp	0x8855
    8917:	17          	tba
    8918:	98 f0       	eora	*0xf0
    891a:	97 f0       	staa	*0xf0
    891c:	53          	comb
    891d:	7d 00 d0    	tst	0xd0
    8920:	27 05       	beq	0x0x8927
    8922:	7f 00 d0    	clr	0xd0
    8925:	20 08       	bra	0x0x892f
    8927:	7d 10 29    	tst	0x1029
    892a:	2a fb       	bpl	0x0x8927
    892c:	b6 10 2a    	ldaa	0x102a
    892f:	f7 10 42    	stab	0x1042
    8932:	01          	nop
    8933:	86 80       	ldaa	#0x80
    8935:	b7 10 2a    	staa	0x102a
    8938:	7f 00 d1    	clr	0xd1
    893b:	bd 86 0e    	jsr	0x860e
    893e:	bd 8e bc    	jsr	0x8ebc
    8941:	4d          	tsta
    8942:	2a 03       	bpl	0x0x8947
    8944:	7e 81 88    	jmp	0x8188
    8947:	7c 00 d1    	inc	0xd1
    894a:	c6 80       	ldab	#0x80
    894c:	d1 dc       	cmpb	*0xdc
    894e:	27 03       	beq	0x0x8953
    8950:	7e 87 d3    	jmp	0x87d3
    8953:	36          	psha
    8954:	bd 8e bc    	jsr	0x8ebc
    8957:	32          	pula
    8958:	7e 88 a7    	jmp	0x88a7
    895b:	bd 8e bc    	jsr	0x8ebc
    895e:	36          	psha
    895f:	bd 8e bc    	jsr	0x8ebc
    8962:	7f 00 d1    	clr	0xd1
    8965:	4d          	tsta
    8966:	26 04       	bne	0x0x896c
    8968:	32          	pula
    8969:	7e 89 e7    	jmp	0x89e7
    896c:	32          	pula
    896d:	ce 01 00    	ldx	#0x100
    8970:	e6 00       	ldab	0x0,x
    8972:	27 20       	beq	0x0x8994
    8974:	11          	cba
    8975:	24 15       	bcc	0x0x898c
    8977:	a7 00       	staa	0x0,x
    8979:	08          	inx
    897a:	8c 01 08    	cpx	#0x108
    897d:	27 51       	beq	0x0x89d0
    897f:	17          	tba
    8980:	e6 00       	ldab	0x0,x
    8982:	26 f3       	bne	0x0x8977
    8984:	a7 00       	staa	0x0,x
    8986:	8f          	xgdx
    8987:	f7 01 7c    	stab	0x17c
    898a:	20 44       	bra	0x0x89d0
    898c:	08          	inx
    898d:	8c 01 08    	cpx	#0x108
    8990:	27 3e       	beq	0x0x89d0
    8992:	20 dc       	bra	0x0x8970
    8994:	a7 00       	staa	0x0,x
    8996:	8f          	xgdx
    8997:	f7 01 7c    	stab	0x17c
    899a:	26 34       	bne	0x0x89d0
    899c:	7d 01 15    	tst	0x115
    899f:	26 2f       	bne	0x0x89d0
    89a1:	86 01       	ldaa	#0x1
    89a3:	b7 01 19    	staa	0x119
    89a6:	86 80       	ldaa	#0x80
    89a8:	97 d2       	staa	*0xd2
    89aa:	7f 00 30    	clr	0x30
    89ad:	7f 00 d3    	clr	0xd3
    89b0:	96 73       	ldaa	*0x73
    89b2:	81 02       	cmpa	#0x2
    89b4:	26 04       	bne	0x0x89ba
    89b6:	96 74       	ldaa	*0x74
    89b8:	97 d3       	staa	*0xd3
    89ba:	86 08       	ldaa	#0x8
    89bc:	b7 10 23    	staa	0x1023
    89bf:	fc 10 0e    	ldd	0x100e
    89c2:	c3 00 c8    	addd	#0xc8
    89c5:	fd 10 1e    	std	0x101e
    89c8:	b6 10 22    	ldaa	0x1022
    89cb:	8a 08       	oraa	#0x8
    89cd:	b7 10 22    	staa	0x1022
    89d0:	bd 8e bc    	jsr	0x8ebc
    89d3:	4d          	tsta
    89d4:	2a 03       	bpl	0x0x89d9
    89d6:	7e 81 88    	jmp	0x8188
    89d9:	7c 00 d1    	inc	0xd1
    89dc:	7e 89 5e    	jmp	0x895e
    89df:	bd 8e bc    	jsr	0x8ebc
    89e2:	36          	psha
    89e3:	bd 8e bc    	jsr	0x8ebc
    89e6:	32          	pula
    89e7:	7f 00 d1    	clr	0xd1
    89ea:	13 95 02 07 	brclr	*0x95, #0x02, 0x0x89f5
    89ee:	36          	psha
    89ef:	16          	tab
    89f0:	4f          	clra
    89f1:	bd 9f e9    	jsr	0x9fe9
    89f4:	32          	pula
    89f5:	ce 01 00    	ldx	#0x100
    89f8:	a1 00       	cmpa	0x0,x
    89fa:	27 08       	beq	0x0x8a04
    89fc:	08          	inx
    89fd:	8c 01 08    	cpx	#0x108
    8a00:	27 23       	beq	0x0x8a25
    8a02:	20 f4       	bra	0x0x89f8
    8a04:	8c 01 07    	cpx	#0x107
    8a07:	25 09       	bcs	0x0x8a12
    8a09:	6f 00       	clr	0x0,x
    8a0b:	86 06       	ldaa	#0x6
    8a0d:	b7 01 7c    	staa	0x17c
    8a10:	20 13       	bra	0x0x8a25
    8a12:	a6 01       	ldaa	0x1,x
    8a14:	27 05       	beq	0x0x8a1b
    8a16:	a7 00       	staa	0x0,x
    8a18:	08          	inx
    8a19:	20 e9       	bra	0x0x8a04
    8a1b:	a7 00       	staa	0x0,x
    8a1d:	8f          	xgdx
    8a1e:	5a          	decb
    8a1f:	2a 01       	bpl	0x0x8a22
    8a21:	5f          	clrb
    8a22:	f7 01 7c    	stab	0x17c
    8a25:	bd 8e bc    	jsr	0x8ebc
    8a28:	4d          	tsta
    8a29:	2a 03       	bpl	0x0x8a2e
    8a2b:	7e 81 88    	jmp	0x8188
    8a2e:	7c 00 d1    	inc	0xd1
    8a31:	c6 80       	ldab	#0x80
    8a33:	d1 dc       	cmpb	*0xdc
    8a35:	27 03       	beq	0x0x8a3a
    8a37:	7e 89 5e    	jmp	0x895e
    8a3a:	7e 89 e2    	jmp	0x89e2
    8a3d:	97 dc       	staa	*0xdc
    8a3f:	7c 00 d1    	inc	0xd1
    8a42:	bd 8e bc    	jsr	0x8ebc
    8a45:	81 00       	cmpa	#0x0
    8a47:	26 14       	bne	0x0x8a5d
    8a49:	7c 00 d8    	inc	0xd8
    8a4c:	bd 8e bc    	jsr	0x8ebc
    8a4f:	12 95 02 03 	brset	*0x95, #0x02, 0x0x8a56
    8a53:	7e 8a cf    	jmp	0x8acf
    8a56:	5f          	clrb
    8a57:	bd 9f e9    	jsr	0x9fe9
    8a5a:	7e 8a cf    	jmp	0x8acf
    8a5d:	81 20       	cmpa	#0x20
    8a5f:	26 23       	bne	0x0x8a84
    8a61:	bd 8e bc    	jsr	0x8ebc
    8a64:	13 95 02 05 	brclr	*0x95, #0x02, 0x0x8a6d
    8a68:	c6 20       	ldab	#0x20
    8a6a:	bd 9f e9    	jsr	0x9fe9
    8a6d:	7d 00 d8    	tst	0xd8
    8a70:	27 0f       	beq	0x0x8a81
    8a72:	7f 00 d8    	clr	0xd8
    8a75:	84 01       	anda	#0x1
    8a77:	97 f9       	staa	*0xf9
    8a79:	86 80       	ldaa	#0x80
    8a7b:	b7 01 1c    	staa	0x11c
    8a7e:	bd a4 f9    	jsr	0xa4f9
    8a81:	7e 8a cf    	jmp	0x8acf
    8a84:	7f 00 d8    	clr	0xd8
    8a87:	81 7b       	cmpa	#0x7b
    8a89:	26 0e       	bne	0x0x8a99
    8a8b:	bd 9a 5c    	jsr	0x9a5c
    8a8e:	bd 8e bc    	jsr	0x8ebc
    8a91:	c6 7b       	ldab	#0x7b
    8a93:	bd 9f e9    	jsr	0x9fe9
    8a96:	7e 8a cf    	jmp	0x8acf
    8a99:	36          	psha
    8a9a:	bd 8e bc    	jsr	0x8ebc
    8a9d:	16          	tab
    8a9e:	32          	pula
    8a9f:	81 01       	cmpa	#0x1
    8aa1:	26 11       	bne	0x0x8ab4
    8aa3:	13 8e 01 0b 	brclr	*0x8e, #0x01, 0x0x8ab2
    8aa7:	36          	psha
    8aa8:	37          	pshb
    8aa9:	17          	tba
    8aaa:	8f          	xgdx
    8aab:	86 87       	ldaa	#0x87
    8aad:	bd 8e 67    	jsr	0x8e67
    8ab0:	33          	pulb
    8ab1:	32          	pula
    8ab2:	20 15       	bra	0x0x8ac9
    8ab4:	81 02       	cmpa	#0x2
    8ab6:	26 29       	bne	0x0x8ae1
    8ab8:	7d 00 8e    	tst	0x8e
    8abb:	27 02       	beq	0x0x8abf
    8abd:	20 10       	bra	0x0x8acf
    8abf:	17          	tba
    8ac0:	5f          	clrb
    8ac1:	8f          	xgdx
    8ac2:	86 87       	ldaa	#0x87
    8ac4:	bd 8e 67    	jsr	0x8e67
    8ac7:	20 06       	bra	0x0x8acf
    8ac9:	8f          	xgdx
    8aca:	86 85       	ldaa	#0x85
    8acc:	bd 8e 67    	jsr	0x8e67
    8acf:	7f 00 d1    	clr	0xd1
    8ad2:	bd 8e bc    	jsr	0x8ebc
    8ad5:	4d          	tsta
    8ad6:	2a 03       	bpl	0x0x8adb
    8ad8:	7e 81 88    	jmp	0x8188
    8adb:	7c 00 d1    	inc	0xd1
    8ade:	7e 8a 45    	jmp	0x8a45
    8ae1:	81 05       	cmpa	#0x5
    8ae3:	27 e4       	beq	0x0x8ac9
    8ae5:	81 07       	cmpa	#0x7
    8ae7:	27 e0       	beq	0x0x8ac9
    8ae9:	81 40       	cmpa	#0x40
    8aeb:	27 dc       	beq	0x0x8ac9
    8aed:	7e 8b 77    	jmp	0x8b77
    8af0:	27 03       	beq	0x0x8af5
    8af2:	7e 8b 77    	jmp	0x8b77
    8af5:	13 95 02 08 	brclr	*0x95, #0x02, 0x0x8b01
    8af9:	36          	psha
    8afa:	17          	tba
    8afb:	33          	pulb
    8afc:	36          	psha
    8afd:	bd 9f e9    	jsr	0x9fe9
    8b00:	33          	pulb
    8b01:	c1 40       	cmpb	#0x40
    8b03:	25 05       	bcs	0x0x8b0a
    8b05:	14 f1 01    	bset	*0xf1, #0x01
    8b08:	20 c5       	bra	0x0x8acf
    8b0a:	7d 00 f1    	tst	0xf1
    8b0d:	26 02       	bne	0x0x8b11
    8b0f:	20 be       	bra	0x0x8acf
    8b11:	7f 00 f1    	clr	0xf1
    8b14:	d6 fd       	ldab	*0xfd
    8b16:	26 03       	bne	0x0x8b1b
    8b18:	7e 8a cf    	jmp	0x8acf
    8b1b:	53          	comb
    8b1c:	7d 00 d0    	tst	0xd0
    8b1f:	27 05       	beq	0x0x8b26
    8b21:	7f 00 d0    	clr	0xd0
    8b24:	20 08       	bra	0x0x8b2e
    8b26:	7d 10 29    	tst	0x1029
    8b29:	2a fb       	bpl	0x0x8b26
    8b2b:	b6 10 2a    	ldaa	0x102a
    8b2e:	f7 10 42    	stab	0x1042
    8b31:	01          	nop
    8b32:	86 80       	ldaa	#0x80
    8b34:	b7 10 2a    	staa	0x102a
    8b37:	37          	pshb
    8b38:	d4 f0       	andb	*0xf0
    8b3a:	d7 f0       	stab	*0xf0
    8b3c:	bd 86 0e    	jsr	0x860e
    8b3f:	33          	pulb
    8b40:	ce 01 00    	ldx	#0x100
    8b43:	13 6a 20 05 	brclr	*0x6a, #0x20, 0x0x8b4c
    8b47:	7d 00 69    	tst	0x69
    8b4a:	26 11       	bne	0x0x8b5d
    8b4c:	54          	lsrb
    8b4d:	25 02       	bcs	0x0x8b51
    8b4f:	6f 00       	clr	0x0,x
    8b51:	08          	inx
    8b52:	8c 01 10    	cpx	#0x110
    8b55:	25 f5       	bcs	0x0x8b4c
    8b57:	7f 00 fd    	clr	0xfd
    8b5a:	7e 8a cf    	jmp	0x8acf
    8b5d:	13 fd 01 03 	brclr	*0xfd, #0x01, 0x0x8b64
    8b61:	7f 01 00    	clr	0x100
    8b64:	4f          	clra
    8b65:	5f          	clrb
    8b66:	fd 01 01    	std	0x101
    8b69:	b7 01 03    	staa	0x103
    8b6c:	b7 01 05    	staa	0x105
    8b6f:	b7 01 07    	staa	0x107
    8b72:	97 fd       	staa	*0xfd
    8b74:	7e 8a cf    	jmp	0x8acf
    8b77:	81 41       	cmpa	#0x41
    8b79:	26 03       	bne	0x0x8b7e
    8b7b:	7e 8a c9    	jmp	0x8ac9
    8b7e:	81 36       	cmpa	#0x36
    8b80:	25 1e       	bcs	0x0x8ba0
    8b82:	81 76       	cmpa	#0x76
    8b84:	22 1a       	bhi	0x0x8ba0
    8b86:	13 95 02 08 	brclr	*0x95, #0x02, 0x0x8b92
    8b8a:	36          	psha
    8b8b:	17          	tba
    8b8c:	33          	pulb
    8b8d:	36          	psha
    8b8e:	bd 9f e9    	jsr	0x9fe9
    8b91:	33          	pulb
    8b92:	ce 8f 07    	ldx	#0x8f07
    8b95:	37          	pshb
    8b96:	16          	tab
    8b97:	c0 36       	subb	#0x36
    8b99:	58          	aslb
    8b9a:	3a          	abx
    8b9b:	ee 00       	ldx	0x0,x
    8b9d:	32          	pula
    8b9e:	ad 00       	jsr	0x0,x
    8ba0:	7e 8a cf    	jmp	0x8acf
    8ba3:	97 dc       	staa	*0xdc
    8ba5:	7c 00 d1    	inc	0xd1
    8ba8:	bd 8e bc    	jsr	0x8ebc
    8bab:	36          	psha
    8bac:	bd 8e bc    	jsr	0x8ebc
    8baf:	33          	pulb
    8bb0:	8f          	xgdx
    8bb1:	86 84       	ldaa	#0x84
    8bb3:	bd 8e 67    	jsr	0x8e67
    8bb6:	7f 00 d1    	clr	0xd1
    8bb9:	bd 8e bc    	jsr	0x8ebc
    8bbc:	4d          	tsta
    8bbd:	2a 03       	bpl	0x0x8bc2
    8bbf:	7e 81 88    	jmp	0x8188
    8bc2:	7c 00 d1    	inc	0xd1
    8bc5:	20 e4       	bra	0x0x8bab
    8bc7:	97 dc       	staa	*0xdc
    8bc9:	7c 00 d1    	inc	0xd1
    8bcc:	bd 8e bc    	jsr	0x8ebc
    8bcf:	36          	psha
    8bd0:	13 8e 02 08 	brclr	*0x8e, #0x02, 0x0x8bdc
    8bd4:	c6 ff       	ldab	#0xff
    8bd6:	8f          	xgdx
    8bd7:	86 87       	ldaa	#0x87
    8bd9:	bd 8e 67    	jsr	0x8e67
    8bdc:	32          	pula
    8bdd:	c6 ff       	ldab	#0xff
    8bdf:	8f          	xgdx
    8be0:	86 86       	ldaa	#0x86
    8be2:	bd 8e 67    	jsr	0x8e67
    8be5:	7f 00 d1    	clr	0xd1
    8be8:	bd 8e bc    	jsr	0x8ebc
    8beb:	4d          	tsta
    8bec:	2a 03       	bpl	0x0x8bf1
    8bee:	7e 81 88    	jmp	0x8188
    8bf1:	7c 00 d1    	inc	0xd1
    8bf4:	7e 8b cf    	jmp	0x8bcf
    8bf7:	97 dc       	staa	*0xdc
    8bf9:	7c 00 d1    	inc	0xd1
    8bfc:	bd 8e bc    	jsr	0x8ebc
    8bff:	13 95 02 08 	brclr	*0x95, #0x02, 0x0x8c0b
    8c03:	36          	psha
    8c04:	16          	tab
    8c05:	86 ff       	ldaa	#0xff
    8c07:	bd 9f e9    	jsr	0x9fe9
    8c0a:	32          	pula
    8c0b:	7d 00 fe    	tst	0xfe
    8c0e:	27 02       	beq	0x0x8c12
    8c10:	20 31       	bra	0x0x8c43
    8c12:	d6 f9       	ldab	*0xf9
    8c14:	c1 02       	cmpb	#0x2
    8c16:	26 05       	bne	0x0x8c1d
    8c18:	b7 01 6a    	staa	0x16a
    8c1b:	20 02       	bra	0x0x8c1f
    8c1d:	97 fa       	staa	*0xfa
    8c1f:	4f          	clra
    8c20:	97 fb       	staa	*0xfb
    8c22:	97 f2       	staa	*0xf2
    8c24:	97 ff       	staa	*0xff
    8c26:	b7 01 1b    	staa	0x11b
    8c29:	bd 9d 8a    	jsr	0x9d8a
    8c2c:	7c 01 7e    	inc	0x17e
    8c2f:	c1 02       	cmpb	#0x2
    8c31:	26 0a       	bne	0x0x8c3d
    8c33:	bd 9f 35    	jsr	0x9f35
    8c36:	b6 50 00    	ldaa	0x5000
    8c39:	97 df       	staa	*0xdf
    8c3b:	20 06       	bra	0x0x8c43
    8c3d:	bd 9d 9c    	jsr	0x9d9c
    8c40:	14 df 80    	bset	*0xdf, #0x80
    8c43:	7f 00 d1    	clr	0xd1
    8c46:	bd 8e bc    	jsr	0x8ebc
    8c49:	4d          	tsta
    8c4a:	2a bf       	bpl	0x0x8c0b
    8c4c:	7e 81 88    	jmp	0x8188
    8c4f:	97 dc       	staa	*0xdc
    8c51:	7c 00 d1    	inc	0xd1
    8c54:	bd 8e bc    	jsr	0x8ebc
    8c57:	81 00       	cmpa	#0x0
    8c59:	26 3b       	bne	0x0x8c96
    8c5b:	bd 8e bc    	jsr	0x8ebc
    8c5e:	81 00       	cmpa	#0x0
    8c60:	26 34       	bne	0x0x8c96
    8c62:	bd 8e bc    	jsr	0x8ebc
    8c65:	81 4d       	cmpa	#0x4d
    8c67:	26 2d       	bne	0x0x8c96
    8c69:	bd 8e bc    	jsr	0x8ebc
    8c6c:	81 08       	cmpa	#0x8
    8c6e:	26 26       	bne	0x0x8c96
    8c70:	bd 8e bc    	jsr	0x8ebc
    8c73:	4d          	tsta
    8c74:	27 04       	beq	0x0x8c7a
    8c76:	81 55       	cmpa	#0x55
    8c78:	26 1c       	bne	0x0x8c96
    8c7a:	bd 8e bc    	jsr	0x8ebc
    8c7d:	4d          	tsta
    8c7e:	27 07       	beq	0x0x8c87
    8c80:	81 2a       	cmpa	#0x2a
    8c82:	26 12       	bne	0x0x8c96
    8c84:	7e a0 c9    	jmp	0xa0c9
    8c87:	bd 8e bc    	jsr	0x8ebc
    8c8a:	81 7f       	cmpa	#0x7f
    8c8c:	27 05       	beq	0x0x8c93
    8c8e:	bd 8e bc    	jsr	0x8ebc
    8c91:	20 0c       	bra	0x0x8c9f
    8c93:	7e 8d 15    	jmp	0x8d15
    8c96:	7f 00 d1    	clr	0xd1
    8c99:	7f 00 ff    	clr	0xff
    8c9c:	7e 81 80    	jmp	0x8180
    8c9f:	4d          	tsta
    8ca0:	27 36       	beq	0x0x8cd8
    8ca2:	81 01       	cmpa	#0x1
    8ca4:	27 1a       	beq	0x0x8cc0
    8ca6:	ce 00 00    	ldx	#0x0
    8ca9:	3c          	pshx
    8caa:	bd 8e bc    	jsr	0x8ebc
    8cad:	38          	pulx
    8cae:	81 f7       	cmpa	#0xf7
    8cb0:	27 05       	beq	0x0x8cb7
    8cb2:	a7 00       	staa	0x0,x
    8cb4:	08          	inx
    8cb5:	20 f2       	bra	0x0x8ca9
    8cb7:	bd 9d b8    	jsr	0x9db8
    8cba:	7f 00 d1    	clr	0xd1
    8cbd:	7e 81 80    	jmp	0x8180
    8cc0:	96 f9       	ldaa	*0xf9
    8cc2:	81 02       	cmpa	#0x2
    8cc4:	26 07       	bne	0x0x8ccd
    8cc6:	b6 01 6a    	ldaa	0x16a
    8cc9:	c6 40       	ldab	#0x40
    8ccb:	20 04       	bra	0x0x8cd1
    8ccd:	96 fa       	ldaa	*0xfa
    8ccf:	c6 b0       	ldab	#0xb0
    8cd1:	3d          	mul
    8cd2:	c3 20 00    	addd	#0x2000
    8cd5:	8f          	xgdx
    8cd6:	20 03       	bra	0x0x8cdb
    8cd8:	ce 20 00    	ldx	#0x2000
    8cdb:	3c          	pshx
    8cdc:	bd 8e bc    	jsr	0x8ebc
    8cdf:	38          	pulx
    8ce0:	81 f7       	cmpa	#0xf7
    8ce2:	27 05       	beq	0x0x8ce9
    8ce4:	a7 00       	staa	0x0,x
    8ce6:	08          	inx
    8ce7:	20 f2       	bra	0x0x8cdb
    8ce9:	4f          	clra
    8cea:	97 fb       	staa	*0xfb
    8cec:	97 f2       	staa	*0xf2
    8cee:	97 ff       	staa	*0xff
    8cf0:	b7 01 1b    	staa	0x11b
    8cf3:	bd 9d 8a    	jsr	0x9d8a
    8cf6:	96 f9       	ldaa	*0xf9
    8cf8:	81 02       	cmpa	#0x2
    8cfa:	26 0a       	bne	0x0x8d06
    8cfc:	bd 9f 35    	jsr	0x9f35
    8cff:	b6 50 00    	ldaa	0x5000
    8d02:	97 df       	staa	*0xdf
    8d04:	20 06       	bra	0x0x8d0c
    8d06:	bd 9d 9c    	jsr	0x9d9c
    8d09:	14 df 80    	bset	*0xdf, #0x80
    8d0c:	7c 01 7e    	inc	0x17e
    8d0f:	7f 00 d1    	clr	0xd1
    8d12:	7e 81 80    	jmp	0x8180
    8d15:	bd 8e bc    	jsr	0x8ebc
    8d18:	8d 1a       	bsr	0x0x8d34
    8d1a:	7f 00 ff    	clr	0xff
    8d1d:	7f 01 1b    	clr	0x11b
    8d20:	bd 9d 8a    	jsr	0x9d8a
    8d23:	96 f2       	ldaa	*0xf2
    8d25:	84 20       	anda	#0x20
    8d27:	97 f2       	staa	*0xf2
    8d29:	86 80       	ldaa	#0x80
    8d2b:	b7 01 1c    	staa	0x11c
    8d2e:	7f 00 d1    	clr	0xd1
    8d31:	7e 81 80    	jmp	0x8180
    8d34:	36          	psha
    8d35:	bd 8e 57    	jsr	0x8e57
    8d38:	86 f0       	ldaa	#0xf0
    8d3a:	b7 10 2f    	staa	0x102f
    8d3d:	86 00       	ldaa	#0x0
    8d3f:	bd 8e 57    	jsr	0x8e57
    8d42:	b7 10 2f    	staa	0x102f
    8d45:	bd 8e 57    	jsr	0x8e57
    8d48:	b7 10 2f    	staa	0x102f
    8d4b:	86 4d       	ldaa	#0x4d
    8d4d:	bd 8e 57    	jsr	0x8e57
    8d50:	b7 10 2f    	staa	0x102f
    8d53:	86 08       	ldaa	#0x8
    8d55:	bd 8e 57    	jsr	0x8e57
    8d58:	b7 10 2f    	staa	0x102f
    8d5b:	bd 8e 57    	jsr	0x8e57
    8d5e:	4f          	clra
    8d5f:	b7 10 2f    	staa	0x102f
    8d62:	bd 8e 57    	jsr	0x8e57
    8d65:	b7 10 2f    	staa	0x102f
    8d68:	bd 8e 57    	jsr	0x8e57
    8d6b:	b7 10 2f    	staa	0x102f
    8d6e:	bd 8e 57    	jsr	0x8e57
    8d71:	32          	pula
    8d72:	b7 10 2f    	staa	0x102f
    8d75:	27 65       	beq	0x0x8ddc
    8d77:	81 01       	cmpa	#0x1
    8d79:	27 10       	beq	0x0x8d8b
    8d7b:	ce 00 20    	ldx	#0x20
    8d7e:	c6 b0       	ldab	#0xb0
    8d80:	8d 48       	bsr	0x0x8dca
    8d82:	bd 8e 57    	jsr	0x8e57
    8d85:	86 f7       	ldaa	#0xf7
    8d87:	b7 10 2f    	staa	0x102f
    8d8a:	39          	rts
    8d8b:	96 fa       	ldaa	*0xfa
    8d8d:	d6 f9       	ldab	*0xf9
    8d8f:	c1 02       	cmpb	#0x2
    8d91:	26 03       	bne	0x0x8d96
    8d93:	b6 01 6a    	ldaa	0x16a
    8d96:	f6 01 1b    	ldab	0x11b
    8d99:	c1 05       	cmpb	#0x5
    8d9b:	26 07       	bne	0x0x8da4
    8d9d:	ce 01 3c    	ldx	#0x13c
    8da0:	bd df 02    	jsr	0xdf02
    8da3:	4a          	deca
    8da4:	d6 f9       	ldab	*0xf9
    8da6:	c1 02       	cmpb	#0x2
    8da8:	26 04       	bne	0x0x8dae
    8daa:	c6 40       	ldab	#0x40
    8dac:	20 02       	bra	0x0x8db0
    8dae:	c6 b0       	ldab	#0xb0
    8db0:	3d          	mul
    8db1:	c3 20 00    	addd	#0x2000
    8db4:	8f          	xgdx
    8db5:	c6 b0       	ldab	#0xb0
    8db7:	96 f9       	ldaa	*0xf9
    8db9:	81 02       	cmpa	#0x2
    8dbb:	26 02       	bne	0x0x8dbf
    8dbd:	c6 40       	ldab	#0x40
    8dbf:	8d 09       	bsr	0x0x8dca
    8dc1:	bd 8e 57    	jsr	0x8e57
    8dc4:	86 f7       	ldaa	#0xf7
    8dc6:	b7 10 2f    	staa	0x102f
    8dc9:	39          	rts
    8dca:	a6 00       	ldaa	0x0,x
    8dcc:	84 7f       	anda	#0x7f
    8dce:	bd 8e 57    	jsr	0x8e57
    8dd1:	bd 8e 5f    	jsr	0x8e5f
    8dd4:	b7 10 2f    	staa	0x102f
    8dd7:	08          	inx
    8dd8:	5a          	decb
    8dd9:	26 ef       	bne	0x0x8dca
    8ddb:	39          	rts
    8ddc:	86 ff       	ldaa	#0xff
    8dde:	97 f8       	staa	*0xf8
    8de0:	ce 20 00    	ldx	#0x2000
    8de3:	18 ce 00 80 	ldy	#0x80
    8de7:	c6 b0       	ldab	#0xb0
    8de9:	96 f9       	ldaa	*0xf9
    8deb:	81 02       	cmpa	#0x2
    8ded:	26 02       	bne	0x0x8df1
    8def:	c6 40       	ldab	#0x40
    8df1:	8d d7       	bsr	0x0x8dca
    8df3:	18 09       	dey
    8df5:	27 54       	beq	0x0x8e4b
    8df7:	18 8c 00 70 	cpy	#0x70
    8dfb:	26 06       	bne	0x0x8e03
    8dfd:	86 7f       	ldaa	#0x7f
    8dff:	97 f8       	staa	*0xf8
    8e01:	20 e4       	bra	0x0x8de7
    8e03:	18 8c 00 60 	cpy	#0x60
    8e07:	26 06       	bne	0x0x8e0f
    8e09:	86 3f       	ldaa	#0x3f
    8e0b:	97 f8       	staa	*0xf8
    8e0d:	20 d8       	bra	0x0x8de7
    8e0f:	18 8c 00 50 	cpy	#0x50
    8e13:	26 06       	bne	0x0x8e1b
    8e15:	86 1f       	ldaa	#0x1f
    8e17:	97 f8       	staa	*0xf8
    8e19:	20 cc       	bra	0x0x8de7
    8e1b:	18 8c 00 40 	cpy	#0x40
    8e1f:	26 06       	bne	0x0x8e27
    8e21:	86 0f       	ldaa	#0xf
    8e23:	97 f8       	staa	*0xf8
    8e25:	20 c0       	bra	0x0x8de7
    8e27:	18 8c 00 30 	cpy	#0x30
    8e2b:	26 06       	bne	0x0x8e33
    8e2d:	86 07       	ldaa	#0x7
    8e2f:	97 f8       	staa	*0xf8
    8e31:	20 b4       	bra	0x0x8de7
    8e33:	18 8c 00 20 	cpy	#0x20
    8e37:	26 06       	bne	0x0x8e3f
    8e39:	86 03       	ldaa	#0x3
    8e3b:	97 f8       	staa	*0xf8
    8e3d:	20 a8       	bra	0x0x8de7
    8e3f:	18 8c 00 10 	cpy	#0x10
    8e43:	26 a2       	bne	0x0x8de7
    8e45:	86 01       	ldaa	#0x1
    8e47:	97 f8       	staa	*0xf8
    8e49:	20 9c       	bra	0x0x8de7
    8e4b:	86 15       	ldaa	#0x15
    8e4d:	97 f8       	staa	*0xf8
    8e4f:	8d 06       	bsr	0x0x8e57
    8e51:	86 f7       	ldaa	#0xf7
    8e53:	b7 10 2f    	staa	0x102f
    8e56:	39          	rts
    8e57:	37          	pshb
    8e58:	f6 10 2e    	ldab	0x102e
    8e5b:	2a fb       	bpl	0x0x8e58
    8e5d:	33          	pulb
    8e5e:	39          	rts
    8e5f:	37          	pshb
    8e60:	c6 4d       	ldab	#0x4d
    8e62:	5a          	decb
    8e63:	26 fd       	bne	0x0x8e62
    8e65:	33          	pulb
    8e66:	39          	rts
    8e67:	7d 00 d0    	tst	0xd0
    8e6a:	27 05       	beq	0x0x8e71
    8e6c:	7f 00 d0    	clr	0xd0
    8e6f:	20 08       	bra	0x0x8e79
    8e71:	7d 10 29    	tst	0x1029
    8e74:	2a fb       	bpl	0x0x8e71
    8e76:	f6 10 2a    	ldab	0x102a
    8e79:	5f          	clrb
    8e7a:	7d 00 df    	tst	0xdf
    8e7d:	26 08       	bne	0x0x8e87
    8e7f:	d6 dc       	ldab	*0xdc
    8e81:	c4 01       	andb	#0x1
    8e83:	26 02       	bne	0x0x8e87
    8e85:	c6 02       	ldab	#0x2
    8e87:	f7 10 42    	stab	0x1042
    8e8a:	01          	nop
    8e8b:	01          	nop
    8e8c:	b7 10 2a    	staa	0x102a
    8e8f:	7d 10 29    	tst	0x1029
    8e92:	2a fb       	bpl	0x0x8e8f
    8e94:	b6 10 2a    	ldaa	0x102a
    8e97:	8f          	xgdx
    8e98:	b7 10 2a    	staa	0x102a
    8e9b:	36          	psha
    8e9c:	7d 10 29    	tst	0x1029
    8e9f:	2a fb       	bpl	0x0x8e9c
    8ea1:	b6 10 2a    	ldaa	0x102a
    8ea4:	f7 10 2a    	stab	0x102a
    8ea7:	12 95 02 02 	brset	*0x95, #0x02, 0x0x8ead
    8eab:	32          	pula
    8eac:	39          	rts
    8ead:	96 dc       	ldaa	*0xdc
    8eaf:	81 e0       	cmpa	#0xe0
    8eb1:	26 04       	bne	0x0x8eb7
    8eb3:	32          	pula
    8eb4:	7e 9f e9    	jmp	0x9fe9
    8eb7:	17          	tba
    8eb8:	33          	pulb
    8eb9:	7e 9f e9    	jmp	0x9fe9
    8ebc:	0f          	sei
    8ebd:	fe 01 c0    	ldx	0x1c0
    8ec0:	bc 01 c2    	cpx	0x1c2
    8ec3:	26 10       	bne	0x0x8ed5
    8ec5:	7d 00 d1    	tst	0xd1
    8ec8:	27 03       	beq	0x0x8ecd
    8eca:	0e          	cli
    8ecb:	20 f0       	bra	0x0x8ebd
    8ecd:	38          	pulx
    8ece:	ff 01 6c    	stx	0x16c
    8ed1:	0e          	cli
    8ed2:	7e 93 14    	jmp	0x9314
    8ed5:	0f          	sei
    8ed6:	a6 00       	ldaa	0x0,x
    8ed8:	8f          	xgdx
    8ed9:	5c          	incb
    8eda:	c4 bf       	andb	#0xbf
    8edc:	8f          	xgdx
    8edd:	ff 01 c0    	stx	0x1c0
    8ee0:	0e          	cli
    8ee1:	81 fa       	cmpa	#0xfa
    8ee3:	27 05       	beq	0x0x8eea
    8ee5:	81 f8       	cmpa	#0xf8
    8ee7:	27 01       	beq	0x0x8eea
    8ee9:	39          	rts
    8eea:	7d 00 d0    	tst	0xd0
    8eed:	27 05       	beq	0x0x8ef4
    8eef:	7f 00 d0    	clr	0xd0
    8ef2:	20 08       	bra	0x0x8efc
    8ef4:	7d 10 29    	tst	0x1029
    8ef7:	2a fb       	bpl	0x0x8ef4
    8ef9:	f6 10 2a    	ldab	0x102a
    8efc:	5f          	clrb
    8efd:	f7 10 42    	stab	0x1042
    8f00:	01          	nop
    8f01:	01          	nop
    8f02:	b7 10 2a    	staa	0x102a
    8f05:	20 b5       	bra	0x0x8ebc
    8f07:	8f          	xgdx
    8f08:	89 8f       	adca	#0x8f
    8f0a:	92 8f       	sbca	*0x8f
    8f0c:	af 8f       	sts	0x8f,x
    8f0e:	cc 8f e9    	ldd	#0x8fe9
    8f11:	90 06       	suba	*0x6
    8f13:	90 0d       	suba	*0xd
    8f15:	90 16       	suba	*0x16
    8f17:	90 33       	suba	*0x33
    8f19:	90 50       	suba	*0x50
    8f1b:	90 6d       	suba	*0x6d
    8f1d:	90 6d       	suba	*0x6d
    8f1f:	90 6d       	suba	*0x6d
    8f21:	90 6d       	suba	*0x6d
    8f23:	90 6d       	suba	*0x6d
    8f25:	90 6d       	suba	*0x6d
    8f27:	90 6e       	suba	*0x6e
    8f29:	90 75       	suba	*0x75
    8f2b:	90 7c       	suba	*0x7c
    8f2d:	90 83       	suba	*0x83
    8f2f:	90 8a       	suba	*0x8a
    8f31:	90 6d       	suba	*0x6d
    8f33:	90 6d       	suba	*0x6d
    8f35:	90 6d       	suba	*0x6d
    8f37:	90 6d       	suba	*0x6d
    8f39:	90 6d       	suba	*0x6d
    8f3b:	90 6d       	suba	*0x6d
    8f3d:	90 6d       	suba	*0x6d
    8f3f:	90 6d       	suba	*0x6d
    8f41:	90 6d       	suba	*0x6d
    8f43:	90 91       	suba	*0x91
    8f45:	90 98       	suba	*0x98
    8f47:	90 9f       	suba	*0x9f
    8f49:	90 a6       	suba	*0xa6
    8f4b:	90 ad       	suba	*0xad
    8f4d:	90 b4       	suba	*0xb4
    8f4f:	90 bb       	suba	*0xbb
    8f51:	90 6d       	suba	*0x6d
    8f53:	90 6d       	suba	*0x6d
    8f55:	90 6d       	suba	*0x6d
    8f57:	90 6d       	suba	*0x6d
    8f59:	90 6d       	suba	*0x6d
    8f5b:	90 6d       	suba	*0x6d
    8f5d:	90 6d       	suba	*0x6d
    8f5f:	90 6d       	suba	*0x6d
    8f61:	90 6d       	suba	*0x6d
    8f63:	90 6d       	suba	*0x6d
    8f65:	90 6d       	suba	*0x6d
    8f67:	90 c2       	suba	*0xc2
    8f69:	90 c9       	suba	*0xc9
    8f6b:	90 d0       	suba	*0xd0
    8f6d:	90 d7       	suba	*0xd7
    8f6f:	90 de       	suba	*0xde
    8f71:	90 e5       	suba	*0xe5
    8f73:	90 ec       	suba	*0xec
    8f75:	90 f3       	suba	*0xf3
    8f77:	90 fa       	suba	*0xfa
    8f79:	91 01       	cmpa	*0x1
    8f7b:	91 08       	cmpa	*0x8
    8f7d:	91 0f       	cmpa	*0xf
    8f7f:	91 16       	cmpa	*0x16
    8f81:	91 1d       	cmpa	*0x1d
    8f83:	91 24       	cmpa	*0x24
    8f85:	91 27       	cmpa	*0x27
    8f87:	91 39       	cmpa	*0x39
    8f89:	84 3f       	anda	#0x3f
    8f8b:	97 3e       	staa	*0x3e
    8f8d:	c6 3e       	ldab	#0x3e
    8f8f:	7e a8 5a    	jmp	0xa85a
    8f92:	c6 44       	ldab	#0x44
    8f94:	4d          	tsta
    8f95:	27 0c       	beq	0x0x8fa3
    8f97:	96 44       	ldaa	*0x44
    8f99:	8a 01       	oraa	#0x1
    8f9b:	97 44       	staa	*0x44
    8f9d:	14 f7 01    	bset	*0xf7, #0x01
    8fa0:	7e a8 5a    	jmp	0xa85a
    8fa3:	96 44       	ldaa	*0x44
    8fa5:	84 fe       	anda	#0xfe
    8fa7:	97 44       	staa	*0x44
    8fa9:	15 f7 01    	bclr	*0xf7, #0x01
    8fac:	7e a8 5a    	jmp	0xa85a
    8faf:	c6 44       	ldab	#0x44
    8fb1:	4d          	tsta
    8fb2:	27 0c       	beq	0x0x8fc0
    8fb4:	96 44       	ldaa	*0x44
    8fb6:	8a 02       	oraa	#0x2
    8fb8:	97 44       	staa	*0x44
    8fba:	14 f7 02    	bset	*0xf7, #0x02
    8fbd:	7e a8 5a    	jmp	0xa85a
    8fc0:	96 44       	ldaa	*0x44
    8fc2:	84 fd       	anda	#0xfd
    8fc4:	97 44       	staa	*0x44
    8fc6:	15 f7 02    	bclr	*0xf7, #0x02
    8fc9:	7e a8 5a    	jmp	0xa85a
    8fcc:	c6 44       	ldab	#0x44
    8fce:	4d          	tsta
    8fcf:	27 0c       	beq	0x0x8fdd
    8fd1:	96 44       	ldaa	*0x44
    8fd3:	8a 04       	oraa	#0x4
    8fd5:	97 44       	staa	*0x44
    8fd7:	14 f7 04    	bset	*0xf7, #0x04
    8fda:	7e a8 5a    	jmp	0xa85a
    8fdd:	96 44       	ldaa	*0x44
    8fdf:	84 fb       	anda	#0xfb
    8fe1:	97 44       	staa	*0x44
    8fe3:	15 f7 04    	bclr	*0xf7, #0x04
    8fe6:	7e a8 5a    	jmp	0xa85a
    8fe9:	c6 44       	ldab	#0x44
    8feb:	4d          	tsta
    8fec:	27 0c       	beq	0x0x8ffa
    8fee:	96 44       	ldaa	*0x44
    8ff0:	8a 08       	oraa	#0x8
    8ff2:	97 44       	staa	*0x44
    8ff4:	14 f7 08    	bset	*0xf7, #0x08
    8ff7:	7e a8 5a    	jmp	0xa85a
    8ffa:	96 44       	ldaa	*0x44
    8ffc:	84 f7       	anda	#0xf7
    8ffe:	97 44       	staa	*0x44
    9000:	15 f7 08    	bclr	*0xf7, #0x08
    9003:	7e a8 5a    	jmp	0xa85a
    9006:	97 42       	staa	*0x42
    9008:	c6 42       	ldab	#0x42
    900a:	7e a8 5a    	jmp	0xa85a
    900d:	84 3f       	anda	#0x3f
    900f:	97 3f       	staa	*0x3f
    9011:	c6 3f       	ldab	#0x3f
    9013:	7e a8 5a    	jmp	0xa85a
    9016:	c6 44       	ldab	#0x44
    9018:	4d          	tsta
    9019:	27 0c       	beq	0x0x9027
    901b:	96 44       	ldaa	*0x44
    901d:	8a 10       	oraa	#0x10
    901f:	97 44       	staa	*0x44
    9021:	14 f7 10    	bset	*0xf7, #0x10
    9024:	7e a8 5a    	jmp	0xa85a
    9027:	96 44       	ldaa	*0x44
    9029:	84 ef       	anda	#0xef
    902b:	97 44       	staa	*0x44
    902d:	15 f7 10    	bclr	*0xf7, #0x10
    9030:	7e a8 5a    	jmp	0xa85a
    9033:	c6 44       	ldab	#0x44
    9035:	4d          	tsta
    9036:	27 0c       	beq	0x0x9044
    9038:	96 44       	ldaa	*0x44
    903a:	8a 20       	oraa	#0x20
    903c:	97 44       	staa	*0x44
    903e:	14 f7 20    	bset	*0xf7, #0x20
    9041:	7e a8 5a    	jmp	0xa85a
    9044:	96 44       	ldaa	*0x44
    9046:	84 df       	anda	#0xdf
    9048:	97 44       	staa	*0x44
    904a:	15 f7 20    	bclr	*0xf7, #0x20
    904d:	7e a8 5a    	jmp	0xa85a
    9050:	c6 44       	ldab	#0x44
    9052:	4d          	tsta
    9053:	27 0c       	beq	0x0x9061
    9055:	96 44       	ldaa	*0x44
    9057:	8a 40       	oraa	#0x40
    9059:	97 44       	staa	*0x44
    905b:	14 f7 40    	bset	*0xf7, #0x40
    905e:	7e a8 5a    	jmp	0xa85a
    9061:	96 44       	ldaa	*0x44
    9063:	84 bf       	anda	#0xbf
    9065:	97 44       	staa	*0x44
    9067:	15 f7 40    	bclr	*0xf7, #0x40
    906a:	7e a8 5a    	jmp	0xa85a
    906d:	39          	rts
    906e:	97 43       	staa	*0x43
    9070:	c6 43       	ldab	#0x43
    9072:	7e a8 5a    	jmp	0xa85a
    9075:	97 28       	staa	*0x28
    9077:	c6 28       	ldab	#0x28
    9079:	7e a8 5a    	jmp	0xa85a
    907c:	97 2a       	staa	*0x2a
    907e:	c6 2a       	ldab	#0x2a
    9080:	7e a8 5a    	jmp	0xa85a
    9083:	97 35       	staa	*0x35
    9085:	c6 35       	ldab	#0x35
    9087:	7e a8 5a    	jmp	0xa85a
    908a:	97 37       	staa	*0x37
    908c:	c6 37       	ldab	#0x37
    908e:	7e a8 5a    	jmp	0xa85a
    9091:	97 45       	staa	*0x45
    9093:	c6 45       	ldab	#0x45
    9095:	7e a8 5a    	jmp	0xa85a
    9098:	97 46       	staa	*0x46
    909a:	c6 46       	ldab	#0x46
    909c:	7e a8 5a    	jmp	0xa85a
    909f:	97 47       	staa	*0x47
    90a1:	c6 47       	ldab	#0x47
    90a3:	7e a8 5a    	jmp	0xa85a
    90a6:	97 49       	staa	*0x49
    90a8:	c6 49       	ldab	#0x49
    90aa:	7e a8 5a    	jmp	0xa85a
    90ad:	97 4c       	staa	*0x4c
    90af:	c6 4c       	ldab	#0x4c
    90b1:	7e a8 5a    	jmp	0xa85a
    90b4:	97 4d       	staa	*0x4d
    90b6:	c6 4d       	ldab	#0x4d
    90b8:	7e a8 5a    	jmp	0xa85a
    90bb:	97 4e       	staa	*0x4e
    90bd:	c6 4e       	ldab	#0x4e
    90bf:	7e a8 5a    	jmp	0xa85a
    90c2:	97 4f       	staa	*0x4f
    90c4:	c6 4f       	ldab	#0x4f
    90c6:	7e a8 5a    	jmp	0xa85a
    90c9:	97 51       	staa	*0x51
    90cb:	c6 51       	ldab	#0x51
    90cd:	7e a8 5a    	jmp	0xa85a
    90d0:	97 52       	staa	*0x52
    90d2:	c6 52       	ldab	#0x52
    90d4:	7e a8 5a    	jmp	0xa85a
    90d7:	97 53       	staa	*0x53
    90d9:	c6 53       	ldab	#0x53
    90db:	7e a8 5a    	jmp	0xa85a
    90de:	97 55       	staa	*0x55
    90e0:	c6 55       	ldab	#0x55
    90e2:	7e a8 5a    	jmp	0xa85a
    90e5:	97 56       	staa	*0x56
    90e7:	c6 56       	ldab	#0x56
    90e9:	7e a8 5a    	jmp	0xa85a
    90ec:	97 57       	staa	*0x57
    90ee:	c6 57       	ldab	#0x57
    90f0:	7e a8 5a    	jmp	0xa85a
    90f3:	97 58       	staa	*0x58
    90f5:	c6 58       	ldab	#0x58
    90f7:	7e a8 5a    	jmp	0xa85a
    90fa:	97 5a       	staa	*0x5a
    90fc:	c6 5a       	ldab	#0x5a
    90fe:	7e a8 5a    	jmp	0xa85a
    9101:	97 63       	staa	*0x63
    9103:	c6 63       	ldab	#0x63
    9105:	7e a8 5a    	jmp	0xa85a
    9108:	97 5b       	staa	*0x5b
    910a:	c6 5b       	ldab	#0x5b
    910c:	7e a8 5a    	jmp	0xa85a
    910f:	97 5c       	staa	*0x5c
    9111:	c6 5c       	ldab	#0x5c
    9113:	7e a8 5a    	jmp	0xa85a
    9116:	97 5d       	staa	*0x5d
    9118:	c6 5d       	ldab	#0x5d
    911a:	7e a8 5a    	jmp	0xa85a
    911d:	97 5f       	staa	*0x5f
    911f:	c6 5f       	ldab	#0x5f
    9121:	7e a8 5a    	jmp	0xa85a
    9124:	97 db       	staa	*0xdb
    9126:	39          	rts
    9127:	4d          	tsta
    9128:	26 08       	bne	0x0x9132
    912a:	13 f3 10 03 	brclr	*0xf3, #0x10, 0x0x9131
    912e:	7e 96 6c    	jmp	0x966c
    9131:	39          	rts
    9132:	12 f3 10 fb 	brset	*0xf3, #0x10, 0x0x9131
    9136:	7e 97 09    	jmp	0x9709
    9139:	c6 41       	ldab	#0x41
    913b:	4d          	tsta
    913c:	27 0c       	beq	0x0x914a
    913e:	96 41       	ldaa	*0x41
    9140:	8a 10       	oraa	#0x10
    9142:	97 41       	staa	*0x41
    9144:	14 f7 80    	bset	*0xf7, #0x80
    9147:	7e a8 5a    	jmp	0xa85a
    914a:	96 41       	ldaa	*0x41
    914c:	84 7f       	anda	#0x7f
    914e:	97 41       	staa	*0x41
    9150:	15 f7 80    	bclr	*0xf7, #0x80
    9153:	7e a8 5a    	jmp	0xa85a
    9156:	86 08       	ldaa	#0x8
    9158:	b7 10 23    	staa	0x1023
    915b:	fc 10 0e    	ldd	0x100e
    915e:	c3 4e 20    	addd	#0x4e20
    9161:	fd 10 1e    	std	0x101e
    9164:	0e          	cli
    9165:	96 db       	ldaa	*0xdb
    9167:	4c          	inca
    9168:	8d 01       	bsr	0x0x916b
    916a:	3b          	rti
    916b:	7d 00 d2    	tst	0xd2
    916e:	2b 12       	bmi	0x0x9182
    9170:	bb 01 19    	adda	0x119
    9173:	25 04       	bcs	0x0x9179
    9175:	81 f0       	cmpa	#0xf0
    9177:	25 10       	bcs	0x0x9189
    9179:	14 d2 80    	bset	*0xd2, #0x80
    917c:	86 f0       	ldaa	#0xf0
    917e:	b7 01 19    	staa	0x119
    9181:	39          	rts
    9182:	16          	tab
    9183:	b6 01 19    	ldaa	0x119
    9186:	10          	sba
    9187:	23 04       	bls	0x0x918d
    9189:	b7 01 19    	staa	0x119
    918c:	39          	rts
    918d:	7f 00 d2    	clr	0xd2
    9190:	4f          	clra
    9191:	b7 01 19    	staa	0x119
    9194:	f6 10 0f    	ldab	0x100f
    9197:	c4 07       	andb	#0x7
    9199:	c1 01       	cmpb	#0x1
    919b:	23 02       	bls	0x0x919f
    919d:	c6 01       	ldab	#0x1
    919f:	f7 01 69    	stab	0x169
    91a2:	7d 00 f0    	tst	0xf0
    91a5:	27 36       	beq	0x0x91dd
    91a7:	7d 00 d0    	tst	0xd0
    91aa:	27 05       	beq	0x0x91b1
    91ac:	7f 00 d0    	clr	0xd0
    91af:	20 08       	bra	0x0x91b9
    91b1:	7d 10 29    	tst	0x1029
    91b4:	2a fb       	bpl	0x0x91b1
    91b6:	f6 10 2a    	ldab	0x102a
    91b9:	f6 01 7b    	ldab	0x17b
    91bc:	f7 10 42    	stab	0x1042
    91bf:	01          	nop
    91c0:	01          	nop
    91c1:	86 80       	ldaa	#0x80
    91c3:	b7 10 2a    	staa	0x102a
    91c6:	7f 00 f0    	clr	0xf0
    91c9:	0d          	sec
    91ca:	59          	rolb
    91cb:	f7 01 7b    	stab	0x17b
    91ce:	4f          	clra
    91cf:	4c          	inca
    91d0:	54          	lsrb
    91d1:	25 fc       	bcs	0x0x91cf
    91d3:	4a          	deca
    91d4:	81 01       	cmpa	#0x1
    91d6:	23 05       	bls	0x0x91dd
    91d8:	86 fe       	ldaa	#0xfe
    91da:	b7 01 7b    	staa	0x17b
    91dd:	96 73       	ldaa	*0x73
    91df:	81 01       	cmpa	#0x1
    91e1:	26 68       	bne	0x0x924b
    91e3:	ce 01 00    	ldx	#0x100
    91e6:	f6 01 68    	ldab	0x168
    91e9:	3a          	abx
    91ea:	a6 00       	ldaa	0x0,x
    91ec:	26 0b       	bne	0x0x91f9
    91ee:	7f 01 68    	clr	0x168
    91f1:	b6 01 00    	ldaa	0x100
    91f4:	26 03       	bne	0x0x91f9
    91f6:	7e 93 05    	jmp	0x9305
    91f9:	d6 d3       	ldab	*0xd3
    91fb:	27 0b       	beq	0x0x9208
    91fd:	8b 0c       	adda	#0xc
    91ff:	2a 04       	bpl	0x0x9205
    9201:	80 0c       	suba	#0xc
    9203:	20 03       	bra	0x0x9208
    9205:	5a          	decb
    9206:	26 f5       	bne	0x0x91fd
    9208:	7c 01 68    	inc	0x168
    920b:	f6 01 7c    	ldab	0x17c
    920e:	f1 01 68    	cmpb	0x168
    9211:	24 35       	bcc	0x0x9248
    9213:	7d 00 74    	tst	0x74
    9216:	27 14       	beq	0x0x922c
    9218:	7c 00 d3    	inc	0xd3
    921b:	d6 74       	ldab	*0x74
    921d:	d1 d3       	cmpb	*0xd3
    921f:	24 24       	bcc	0x0x9245
    9221:	d6 73       	ldab	*0x73
    9223:	c1 03       	cmpb	#0x3
    9225:	27 0b       	beq	0x0x9232
    9227:	7f 00 d3    	clr	0xd3
    922a:	20 19       	bra	0x0x9245
    922c:	d6 73       	ldab	*0x73
    922e:	c1 03       	cmpb	#0x3
    9230:	26 13       	bne	0x0x9245
    9232:	14 30 80    	bset	*0x30, #0x80
    9235:	7a 00 d3    	dec	0xd3
    9238:	2a 03       	bpl	0x0x923d
    923a:	7f 00 d3    	clr	0xd3
    923d:	7a 01 68    	dec	0x168
    9240:	7a 01 68    	dec	0x168
    9243:	2a 03       	bpl	0x0x9248
    9245:	7f 01 68    	clr	0x168
    9248:	7e 92 c5    	jmp	0x92c5
    924b:	81 02       	cmpa	#0x2
    924d:	26 69       	bne	0x0x92b8
    924f:	ce 01 00    	ldx	#0x100
    9252:	f6 01 68    	ldab	0x168
    9255:	3a          	abx
    9256:	a6 00       	ldaa	0x0,x
    9258:	26 11       	bne	0x0x926b
    925a:	ce 01 00    	ldx	#0x100
    925d:	f6 01 7c    	ldab	0x17c
    9260:	f7 01 68    	stab	0x168
    9263:	3a          	abx
    9264:	a6 00       	ldaa	0x0,x
    9266:	26 03       	bne	0x0x926b
    9268:	7e 93 05    	jmp	0x9305
    926b:	d6 d3       	ldab	*0xd3
    926d:	27 0b       	beq	0x0x927a
    926f:	8b 0c       	adda	#0xc
    9271:	2a 04       	bpl	0x0x9277
    9273:	80 0c       	suba	#0xc
    9275:	20 03       	bra	0x0x927a
    9277:	5a          	decb
    9278:	26 f5       	bne	0x0x926f
    927a:	7a 01 68    	dec	0x168
    927d:	2a 36       	bpl	0x0x92b5
    927f:	7d 00 74    	tst	0x74
    9282:	27 11       	beq	0x0x9295
    9284:	7a 00 d3    	dec	0xd3
    9287:	2a 26       	bpl	0x0x92af
    9289:	d6 73       	ldab	*0x73
    928b:	c1 03       	cmpb	#0x3
    928d:	27 0c       	beq	0x0x929b
    928f:	d6 74       	ldab	*0x74
    9291:	d7 d3       	stab	*0xd3
    9293:	20 1a       	bra	0x0x92af
    9295:	d6 73       	ldab	*0x73
    9297:	c1 03       	cmpb	#0x3
    9299:	26 14       	bne	0x0x92af
    929b:	7f 00 d3    	clr	0xd3
    929e:	7f 00 30    	clr	0x30
    92a1:	7c 01 68    	inc	0x168
    92a4:	7c 01 68    	inc	0x168
    92a7:	f6 01 7c    	ldab	0x17c
    92aa:	f1 01 68    	cmpb	0x168
    92ad:	24 06       	bcc	0x0x92b5
    92af:	f6 01 7c    	ldab	0x17c
    92b2:	f7 01 68    	stab	0x168
    92b5:	7e 92 c5    	jmp	0x92c5
    92b8:	81 03       	cmpa	#0x3
    92ba:	26 08       	bne	0x0x92c4
    92bc:	7d 00 30    	tst	0x30
    92bf:	2b 8e       	bmi	0x0x924f
    92c1:	7e 91 e3    	jmp	0x91e3
    92c4:	39          	rts
    92c5:	7d 00 d0    	tst	0xd0
    92c8:	27 05       	beq	0x0x92cf
    92ca:	7f 00 d0    	clr	0xd0
    92cd:	20 08       	bra	0x0x92d7
    92cf:	7d 10 29    	tst	0x1029
    92d2:	2a fb       	bpl	0x0x92cf
    92d4:	f6 10 2a    	ldab	0x102a
    92d7:	f6 01 7b    	ldab	0x17b
    92da:	f7 10 42    	stab	0x1042
    92dd:	53          	comb
    92de:	d7 f0       	stab	*0xf0
    92e0:	01          	nop
    92e1:	01          	nop
    92e2:	c6 81       	ldab	#0x81
    92e4:	f7 10 2a    	stab	0x102a
    92e7:	01          	nop
    92e8:	7d 10 29    	tst	0x1029
    92eb:	2a fa       	bpl	0x0x92e7
    92ed:	f6 10 2a    	ldab	0x102a
    92f0:	b7 10 2a    	staa	0x102a
    92f3:	01          	nop
    92f4:	7d 10 29    	tst	0x1029
    92f7:	2a fa       	bpl	0x0x92f3
    92f9:	b6 10 2a    	ldaa	0x102a
    92fc:	86 5a       	ldaa	#0x5a
    92fe:	b7 10 2a    	staa	0x102a
    9301:	bd 86 0e    	jsr	0x860e
    9304:	39          	rts
    9305:	7f 00 f0    	clr	0xf0
    9308:	b6 10 22    	ldaa	0x1022
    930b:	84 f7       	anda	#0xf7
    930d:	b7 10 22    	staa	0x1022
    9310:	bd 86 0e    	jsr	0x860e
    9313:	39          	rts
    9314:	ce 10 23    	ldx	#0x1023
    9317:	1e 00 40 03 	brset	0x0,x, #0x40, 0x0x931e
    931b:	7e 9d 19    	jmp	0x9d19
    931e:	86 20       	ldaa	#0x20
    9320:	b7 10 44    	staa	0x1044
    9323:	01          	nop
    9324:	01          	nop
    9325:	b6 10 45    	ldaa	0x1045
    9328:	b1 01 75    	cmpa	0x175
    932b:	26 0b       	bne	0x0x9338
    932d:	7d 00 fe    	tst	0xfe
    9330:	27 03       	beq	0x0x9335
    9332:	7e 9d 0b    	jmp	0x9d0b
    9335:	7e 95 7f    	jmp	0x957f
    9338:	16          	tab
    9339:	b8 01 75    	eora	0x175
    933c:	f7 01 75    	stab	0x175
    933f:	b4 01 75    	anda	0x175
    9342:	84 fc       	anda	#0xfc
    9344:	26 03       	bne	0x0x9349
    9346:	7e 94 86    	jmp	0x9486
    9349:	48          	asla
    934a:	24 22       	bcc	0x0x936e
    934c:	7d 00 fe    	tst	0xfe
    934f:	27 06       	beq	0x0x9357
    9351:	7f 00 fe    	clr	0xfe
    9354:	7e 9f cf    	jmp	0x9fcf
    9357:	7f 00 ff    	clr	0xff
    935a:	7f 01 1b    	clr	0x11b
    935d:	bd 9d 8a    	jsr	0x9d8a
    9360:	96 f2       	ldaa	*0xf2
    9362:	84 20       	anda	#0x20
    9364:	97 f2       	staa	*0xf2
    9366:	86 80       	ldaa	#0x80
    9368:	b7 01 1c    	staa	0x11c
    936b:	7e 9d 0b    	jmp	0x9d0b
    936e:	7d 00 fe    	tst	0xfe
    9371:	27 03       	beq	0x0x9376
    9373:	7e 9d 0b    	jmp	0x9d0b
    9376:	48          	asla
    9377:	25 03       	bcs	0x0x937c
    9379:	7e 94 04    	jmp	0x9404
    937c:	d6 ff       	ldab	*0xff
    937e:	27 3c       	beq	0x0x93bc
    9380:	c1 01       	cmpb	#0x1
    9382:	27 5b       	beq	0x0x93df
    9384:	86 05       	ldaa	#0x5
    9386:	b1 01 1b    	cmpa	0x11b
    9389:	26 31       	bne	0x0x93bc
    938b:	13 f2 40 2d 	brclr	*0xf2, #0x40, 0x0x93bc
    938f:	b6 01 1e    	ldaa	0x11e
    9392:	81 1e       	cmpa	#0x1e
    9394:	27 03       	beq	0x0x9399
    9396:	7e 9d 0b    	jmp	0x9d0b
    9399:	4f          	clra
    939a:	f6 01 3c    	ldab	0x13c
    939d:	c1 41       	cmpb	#0x41
    939f:	27 01       	beq	0x0x93a2
    93a1:	4c          	inca
    93a2:	bd 8d 34    	jsr	0x8d34
    93a5:	7f 00 ff    	clr	0xff
    93a8:	7f 01 1b    	clr	0x11b
    93ab:	bd 9d 8a    	jsr	0x9d8a
    93ae:	96 f2       	ldaa	*0xf2
    93b0:	84 20       	anda	#0x20
    93b2:	97 f2       	staa	*0xf2
    93b4:	86 80       	ldaa	#0x80
    93b6:	b7 01 1c    	staa	0x11c
    93b9:	7e 9d 0b    	jmp	0x9d0b
    93bc:	7d 01 75    	tst	0x175
    93bf:	2b 49       	bmi	0x0x940a
    93c1:	7d 01 70    	tst	0x170
    93c4:	27 05       	beq	0x0x93cb
    93c6:	7f 00 ff    	clr	0xff
    93c9:	20 2e       	bra	0x0x93f9
    93cb:	c6 01       	ldab	#0x1
    93cd:	d7 ff       	stab	*0xff
    93cf:	c6 01       	ldab	#0x1
    93d1:	f7 01 1b    	stab	0x11b
    93d4:	c6 40       	ldab	#0x40
    93d6:	d7 f2       	stab	*0xf2
    93d8:	58          	aslb
    93d9:	f7 01 1c    	stab	0x11c
    93dc:	7e 9d 0b    	jmp	0x9d0b
    93df:	d6 f9       	ldab	*0xf9
    93e1:	c1 02       	cmpb	#0x2
    93e3:	26 05       	bne	0x0x93ea
    93e5:	bd 9f 86    	jsr	0x9f86
    93e8:	20 03       	bra	0x0x93ed
    93ea:	bd 9f 98    	jsr	0x9f98
    93ed:	7f 00 ff    	clr	0xff
    93f0:	7f 00 f2    	clr	0xf2
    93f3:	7f 00 fb    	clr	0xfb
    93f6:	bd 9d 8a    	jsr	0x9d8a
    93f9:	7f 01 1b    	clr	0x11b
    93fc:	86 80       	ldaa	#0x80
    93fe:	b7 01 1c    	staa	0x11c
    9401:	7e 9d 0b    	jmp	0x9d0b
    9404:	48          	asla
    9405:	25 03       	bcs	0x0x940a
    9407:	7e 94 1e    	jmp	0x941e
    940a:	7d 00 fb    	tst	0xfb
    940d:	27 0c       	beq	0x0x941b
    940f:	96 f9       	ldaa	*0xf9
    9411:	81 02       	cmpa	#0x2
    9413:	24 06       	bcc	0x0x941b
    9415:	7c 00 fe    	inc	0xfe
    9418:	7e 9f b5    	jmp	0x9fb5
    941b:	7e 9d 0b    	jmp	0x9d0b
    941e:	48          	asla
    941f:	24 47       	bcc	0x0x9468
    9421:	7d 00 ff    	tst	0xff
    9424:	26 21       	bne	0x0x9447
    9426:	96 f9       	ldaa	*0xf9
    9428:	4c          	inca
    9429:	81 02       	cmpa	#0x2
    942b:	25 01       	bcs	0x0x942e
    942d:	4f          	clra
    942e:	97 f9       	staa	*0xf9
    9430:	86 80       	ldaa	#0x80
    9432:	b7 01 1c    	staa	0x11c
    9435:	bd a4 f9    	jsr	0xa4f9
    9438:	7f 00 fb    	clr	0xfb
    943b:	7f 00 f2    	clr	0xf2
    943e:	14 df 80    	bset	*0xdf, #0x80
    9441:	bd 9d 9c    	jsr	0x9d9c
    9444:	7e 9d 0b    	jmp	0x9d0b
    9447:	13 ff 01 17 	brclr	*0xff, #0x01, 0x0x9462
    944b:	96 f9       	ldaa	*0xf9
    944d:	81 02       	cmpa	#0x2
    944f:	26 03       	bne	0x0x9454
    9451:	7e 9d 0b    	jmp	0x9d0b
    9454:	4c          	inca
    9455:	84 01       	anda	#0x1
    9457:	97 f9       	staa	*0xf9
    9459:	bd a4 f9    	jsr	0xa4f9
    945c:	7c 01 7e    	inc	0x17e
    945f:	7e 9d 0b    	jmp	0x9d0b
    9462:	7c 01 7e    	inc	0x17e
    9465:	7e 9d 0b    	jmp	0x9d0b
    9468:	d6 ff       	ldab	*0xff
    946a:	c1 02       	cmpb	#0x2
    946c:	27 03       	beq	0x0x9471
    946e:	7e 9d 0b    	jmp	0x9d0b
    9471:	48          	asla
    9472:	25 02       	bcs	0x0x9476
    9474:	20 08       	bra	0x0x947e
    9476:	86 01       	ldaa	#0x1
    9478:	b7 01 7f    	staa	0x17f
    947b:	7e 9d 0b    	jmp	0x9d0b
    947e:	86 80       	ldaa	#0x80
    9480:	b7 01 7f    	staa	0x17f
    9483:	7e 9d 0b    	jmp	0x9d0b
    9486:	7d 00 fe    	tst	0xfe
    9489:	27 03       	beq	0x0x948e
    948b:	7e 9d 0b    	jmp	0x9d0b
    948e:	17          	tba
    948f:	84 03       	anda	#0x3
    9491:	81 03       	cmpa	#0x3
    9493:	26 04       	bne	0x0x9499
    9495:	86 02       	ldaa	#0x2
    9497:	20 06       	bra	0x0x949f
    9499:	81 02       	cmpa	#0x2
    949b:	26 02       	bne	0x0x949f
    949d:	86 03       	ldaa	#0x3
    949f:	b1 01 18    	cmpa	0x118
    94a2:	26 06       	bne	0x0x94aa
    94a4:	f7 01 75    	stab	0x175
    94a7:	7e 9d 0b    	jmp	0x9d0b
    94aa:	f7 01 75    	stab	0x175
    94ad:	f6 01 18    	ldab	0x118
    94b0:	5c          	incb
    94b1:	c4 03       	andb	#0x3
    94b3:	11          	cba
    94b4:	27 0f       	beq	0x0x94c5
    94b6:	f6 01 18    	ldab	0x118
    94b9:	5a          	decb
    94ba:	c4 03       	andb	#0x3
    94bc:	11          	cba
    94bd:	27 78       	beq	0x0x9537
    94bf:	b7 01 18    	staa	0x118
    94c2:	7e 9d 0b    	jmp	0x9d0b
    94c5:	b7 01 18    	staa	0x118
    94c8:	7d 00 ff    	tst	0xff
    94cb:	26 26       	bne	0x0x94f3
    94cd:	7f 00 fb    	clr	0xfb
    94d0:	7f 00 f2    	clr	0xf2
    94d3:	96 f9       	ldaa	*0xf9
    94d5:	81 02       	cmpa	#0x2
    94d7:	25 0e       	bcs	0x0x94e7
    94d9:	bd 9d 70    	jsr	0x9d70
    94dc:	bd 9f 35    	jsr	0x9f35
    94df:	b6 50 00    	ldaa	0x5000
    94e2:	97 df       	staa	*0xdf
    94e4:	7e 9d 0b    	jmp	0x9d0b
    94e7:	bd 9d 2d    	jsr	0x9d2d
    94ea:	bd 9d 9c    	jsr	0x9d9c
    94ed:	14 df 80    	bset	*0xdf, #0x80
    94f0:	7e 9d 0b    	jmp	0x9d0b
    94f3:	13 ff 01 12 	brclr	*0xff, #0x01, 0x0x9509
    94f7:	96 f9       	ldaa	*0xf9
    94f9:	81 02       	cmpa	#0x2
    94fb:	25 06       	bcs	0x0x9503
    94fd:	bd 9d 70    	jsr	0x9d70
    9500:	7e 9d 0b    	jmp	0x9d0b
    9503:	bd 9d 2d    	jsr	0x9d2d
    9506:	7e 9d 0b    	jmp	0x9d0b
    9509:	86 01       	ldaa	#0x1
    950b:	b7 01 1a    	staa	0x11a
    950e:	7d 00 fb    	tst	0xfb
    9511:	27 03       	beq	0x0x9516
    9513:	7e 9d 0b    	jmp	0x9d0b
    9516:	b6 01 1b    	ldaa	0x11b
    9519:	81 03       	cmpa	#0x3
    951b:	27 17       	beq	0x0x9534
    951d:	81 04       	cmpa	#0x4
    951f:	27 13       	beq	0x0x9534
    9521:	81 05       	cmpa	#0x5
    9523:	27 0f       	beq	0x0x9534
    9525:	81 23       	cmpa	#0x23
    9527:	27 0b       	beq	0x0x9534
    9529:	14 fb 01    	bset	*0xfb, #0x01
    952c:	14 f2 20    	bset	*0xf2, #0x20
    952f:	86 1f       	ldaa	#0x1f
    9531:	b7 01 66    	staa	0x166
    9534:	7e 9d 0b    	jmp	0x9d0b
    9537:	b7 01 18    	staa	0x118
    953a:	7d 00 ff    	tst	0xff
    953d:	26 26       	bne	0x0x9565
    953f:	7f 00 fb    	clr	0xfb
    9542:	7f 00 f2    	clr	0xf2
    9545:	96 f9       	ldaa	*0xf9
    9547:	81 02       	cmpa	#0x2
    9549:	25 0e       	bcs	0x0x9559
    954b:	bd 9d 7d    	jsr	0x9d7d
    954e:	bd 9f 35    	jsr	0x9f35
    9551:	b6 50 00    	ldaa	0x5000
    9554:	97 df       	staa	*0xdf
    9556:	7e 9d 0b    	jmp	0x9d0b
    9559:	bd 9d 4f    	jsr	0x9d4f
    955c:	bd 9d 9c    	jsr	0x9d9c
    955f:	14 df 80    	bset	*0xdf, #0x80
    9562:	7e 9d 0b    	jmp	0x9d0b
    9565:	13 ff 01 12 	brclr	*0xff, #0x01, 0x0x957b
    9569:	96 f9       	ldaa	*0xf9
    956b:	81 02       	cmpa	#0x2
    956d:	25 06       	bcs	0x0x9575
    956f:	bd 9d 7d    	jsr	0x9d7d
    9572:	7e 9d 0b    	jmp	0x9d0b
    9575:	bd 9d 4f    	jsr	0x9d4f
    9578:	7e 9d 0b    	jmp	0x9d0b
    957b:	86 80       	ldaa	#0x80
    957d:	20 8c       	bra	0x0x950b
    957f:	86 10       	ldaa	#0x10
    9581:	b7 10 44    	staa	0x1044
    9584:	01          	nop
    9585:	01          	nop
    9586:	b6 10 45    	ldaa	0x1045
    9589:	b1 01 76    	cmpa	0x176
    958c:	26 03       	bne	0x0x9591
    958e:	7e 97 f6    	jmp	0x97f6
    9591:	16          	tab
    9592:	b8 01 76    	eora	0x176
    9595:	f7 01 76    	stab	0x176
    9598:	b4 01 76    	anda	0x176
    959b:	26 03       	bne	0x0x95a0
    959d:	7e 97 f6    	jmp	0x97f6
    95a0:	48          	asla
    95a1:	24 34       	bcc	0x0x95d7
    95a3:	86 18       	ldaa	#0x18
    95a5:	b1 01 1b    	cmpa	0x11b
    95a8:	26 10       	bne	0x0x95ba
    95aa:	4c          	inca
    95ab:	b7 01 1b    	staa	0x11b
    95ae:	96 f2       	ldaa	*0xf2
    95b0:	84 20       	anda	#0x20
    95b2:	8a 02       	oraa	#0x2
    95b4:	97 f2       	staa	*0xf2
    95b6:	86 02       	ldaa	#0x2
    95b8:	20 13       	bra	0x0x95cd
    95ba:	b7 01 1b    	staa	0x11b
    95bd:	bd 9d 8a    	jsr	0x9d8a
    95c0:	14 f3 80    	bset	*0xf3, #0x80
    95c3:	96 f2       	ldaa	*0xf2
    95c5:	84 20       	anda	#0x20
    95c7:	8a 01       	oraa	#0x1
    95c9:	97 f2       	staa	*0xf2
    95cb:	86 02       	ldaa	#0x2
    95cd:	97 ff       	staa	*0xff
    95cf:	86 80       	ldaa	#0x80
    95d1:	b7 01 1c    	staa	0x11c
    95d4:	7e 9d 0b    	jmp	0x9d0b
    95d7:	48          	asla
    95d8:	24 41       	bcc	0x0x961b
    95da:	96 f2       	ldaa	*0xf2
    95dc:	84 20       	anda	#0x20
    95de:	c6 14       	ldab	#0x14
    95e0:	f1 01 1b    	cmpb	0x11b
    95e3:	26 05       	bne	0x0x95ea
    95e5:	5c          	incb
    95e6:	8a 02       	oraa	#0x2
    95e8:	20 1a       	bra	0x0x9604
    95ea:	5c          	incb
    95eb:	f1 01 1b    	cmpb	0x11b
    95ee:	26 05       	bne	0x0x95f5
    95f0:	5c          	incb
    95f1:	8a 04       	oraa	#0x4
    95f3:	20 0f       	bra	0x0x9604
    95f5:	5c          	incb
    95f6:	f1 01 1b    	cmpb	0x11b
    95f9:	26 05       	bne	0x0x9600
    95fb:	5c          	incb
    95fc:	8a 08       	oraa	#0x8
    95fe:	20 04       	bra	0x0x9604
    9600:	c6 14       	ldab	#0x14
    9602:	8a 01       	oraa	#0x1
    9604:	f7 01 1b    	stab	0x11b
    9607:	97 f2       	staa	*0xf2
    9609:	bd 9d 8a    	jsr	0x9d8a
    960c:	14 f3 40    	bset	*0xf3, #0x40
    960f:	86 02       	ldaa	#0x2
    9611:	97 ff       	staa	*0xff
    9613:	86 80       	ldaa	#0x80
    9615:	b7 01 1c    	staa	0x11c
    9618:	7e 9d 0b    	jmp	0x9d0b
    961b:	48          	asla
    961c:	24 36       	bcc	0x0x9654
    961e:	96 f2       	ldaa	*0xf2
    9620:	84 20       	anda	#0x20
    9622:	c6 06       	ldab	#0x6
    9624:	f1 01 1b    	cmpb	0x11b
    9627:	26 05       	bne	0x0x962e
    9629:	5c          	incb
    962a:	8a 02       	oraa	#0x2
    962c:	20 0f       	bra	0x0x963d
    962e:	5c          	incb
    962f:	f1 01 1b    	cmpb	0x11b
    9632:	26 05       	bne	0x0x9639
    9634:	5c          	incb
    9635:	8a 04       	oraa	#0x4
    9637:	20 04       	bra	0x0x963d
    9639:	c6 06       	ldab	#0x6
    963b:	8a 01       	oraa	#0x1
    963d:	f7 01 1b    	stab	0x11b
    9640:	97 f2       	staa	*0xf2
    9642:	bd 9d 8a    	jsr	0x9d8a
    9645:	14 f3 20    	bset	*0xf3, #0x20
    9648:	86 02       	ldaa	#0x2
    964a:	97 ff       	staa	*0xff
    964c:	86 80       	ldaa	#0x80
    964e:	b7 01 1c    	staa	0x11c
    9651:	7e 9d 0b    	jmp	0x9d0b
    9654:	48          	asla
    9655:	25 03       	bcs	0x0x965a
    9657:	7e 97 ac    	jmp	0x97ac
    965a:	12 f3 10 03 	brset	*0xf3, #0x10, 0x0x9661
    965e:	7e 96 fd    	jmp	0x96fd
    9661:	8d 09       	bsr	0x0x966c
    9663:	4f          	clra
    9664:	c6 75       	ldab	#0x75
    9666:	bd a8 a2    	jsr	0xa8a2
    9669:	7e 9d 0b    	jmp	0x9d0b
    966c:	15 f3 10    	bclr	*0xf3, #0x10
    966f:	b6 10 22    	ldaa	0x1022
    9672:	84 f7       	anda	#0xf7
    9674:	b7 10 22    	staa	0x1022
    9677:	7f 01 7c    	clr	0x17c
    967a:	7f 00 f0    	clr	0xf0
    967d:	7d 00 d0    	tst	0xd0
    9680:	26 08       	bne	0x0x968a
    9682:	7d 10 29    	tst	0x1029
    9685:	2a fb       	bpl	0x0x9682
    9687:	b6 10 2a    	ldaa	0x102a
    968a:	b6 01 7b    	ldaa	0x17b
    968d:	b7 10 42    	staa	0x1042
    9690:	86 80       	ldaa	#0x80
    9692:	b7 10 2a    	staa	0x102a
    9695:	ce 01 00    	ldx	#0x100
    9698:	ff 01 10    	stx	0x110
    969b:	c6 fe       	ldab	#0xfe
    969d:	6d 00       	tst	0x0,x
    969f:	27 3f       	beq	0x0x96e0
    96a1:	7d 10 29    	tst	0x1029
    96a4:	2a fb       	bpl	0x0x96a1
    96a6:	b6 10 2a    	ldaa	0x102a
    96a9:	86 81       	ldaa	#0x81
    96ab:	f7 10 42    	stab	0x1042
    96ae:	b7 10 2a    	staa	0x102a
    96b1:	7d 10 29    	tst	0x1029
    96b4:	2a fb       	bpl	0x0x96b1
    96b6:	b6 10 2a    	ldaa	0x102a
    96b9:	a6 00       	ldaa	0x0,x
    96bb:	b7 10 2a    	staa	0x102a
    96be:	7d 10 29    	tst	0x1029
    96c1:	2a fb       	bpl	0x0x96be
    96c3:	b6 10 2a    	ldaa	0x102a
    96c6:	86 5a       	ldaa	#0x5a
    96c8:	b7 10 2a    	staa	0x102a
    96cb:	ff 01 10    	stx	0x110
    96ce:	17          	tba
    96cf:	43          	coma
    96d0:	9a f0       	oraa	*0xf0
    96d2:	97 f0       	staa	*0xf0
    96d4:	12 f5 20 08 	brset	*0xf5, #0x20, 0x0x96e0
    96d8:	0d          	sec
    96d9:	59          	rolb
    96da:	08          	inx
    96db:	8c 01 01    	cpx	#0x101
    96de:	23 bd       	bls	0x0x969d
    96e0:	4f          	clra
    96e1:	5f          	clrb
    96e2:	fd 01 02    	std	0x102
    96e5:	fd 01 04    	std	0x104
    96e8:	fd 01 06    	std	0x106
    96eb:	86 01       	ldaa	#0x1
    96ed:	b7 01 11    	staa	0x111
    96f0:	bd 86 0e    	jsr	0x860e
    96f3:	96 71       	ldaa	*0x71
    96f5:	97 db       	staa	*0xdb
    96f7:	96 72       	ldaa	*0x72
    96f9:	b7 01 15    	staa	0x115
    96fc:	39          	rts
    96fd:	8d 0a       	bsr	0x0x9709
    96ff:	86 7f       	ldaa	#0x7f
    9701:	c6 75       	ldab	#0x75
    9703:	bd a8 a2    	jsr	0xa8a2
    9706:	7e 9d 0b    	jmp	0x9d0b
    9709:	14 f3 10    	bset	*0xf3, #0x10
    970c:	ce 01 00    	ldx	#0x100
    970f:	18 ce 01 01 	ldy	#0x101
    9713:	a6 00       	ldaa	0x0,x
    9715:	26 02       	bne	0x0x9719
    9717:	86 80       	ldaa	#0x80
    9719:	18 e6 00    	ldab	0x0,y
    971c:	26 0a       	bne	0x0x9728
    971e:	18 08       	iny
    9720:	18 8c 01 07 	cpy	#0x107
    9724:	23 f3       	bls	0x0x9719
    9726:	20 0f       	bra	0x0x9737
    9728:	11          	cba
    9729:	25 f3       	bcs	0x0x971e
    972b:	4d          	tsta
    972c:	2a 01       	bpl	0x0x972f
    972e:	4f          	clra
    972f:	18 a7 00    	staa	0x0,y
    9732:	e7 00       	stab	0x0,x
    9734:	17          	tba
    9735:	20 e7       	bra	0x0x971e
    9737:	4d          	tsta
    9738:	2a 11       	bpl	0x0x974b
    973a:	6f 00       	clr	0x0,x
    973c:	8c 01 00    	cpx	#0x100
    973f:	27 26       	beq	0x0x9767
    9741:	8f          	xgdx
    9742:	5a          	decb
    9743:	2a 01       	bpl	0x0x9746
    9745:	5f          	clrb
    9746:	f7 01 7c    	stab	0x17c
    9749:	20 1c       	bra	0x0x9767
    974b:	08          	inx
    974c:	8c 01 07    	cpx	#0x107
    974f:	25 0f       	bcs	0x0x9760
    9751:	86 06       	ldaa	#0x6
    9753:	b7 01 7c    	staa	0x17c
    9756:	7d 01 07    	tst	0x107
    9759:	27 0c       	beq	0x0x9767
    975b:	7c 01 7c    	inc	0x17c
    975e:	20 07       	bra	0x0x9767
    9760:	3c          	pshx
    9761:	18 38       	puly
    9763:	18 08       	iny
    9765:	20 ac       	bra	0x0x9713
    9767:	86 fe       	ldaa	#0xfe
    9769:	b7 01 7b    	staa	0x17b
    976c:	7f 01 68    	clr	0x168
    976f:	7d 01 00    	tst	0x100
    9772:	26 03       	bne	0x0x9777
    9774:	7e 97 ab    	jmp	0x97ab
    9777:	7d 01 15    	tst	0x115
    977a:	26 2f       	bne	0x0x97ab
    977c:	86 01       	ldaa	#0x1
    977e:	b7 01 19    	staa	0x119
    9781:	86 80       	ldaa	#0x80
    9783:	97 d2       	staa	*0xd2
    9785:	7f 00 30    	clr	0x30
    9788:	7f 00 d3    	clr	0xd3
    978b:	96 73       	ldaa	*0x73
    978d:	81 02       	cmpa	#0x2
    978f:	26 04       	bne	0x0x9795
    9791:	96 74       	ldaa	*0x74
    9793:	97 d3       	staa	*0xd3
    9795:	86 08       	ldaa	#0x8
    9797:	b7 10 23    	staa	0x1023
    979a:	fc 10 0e    	ldd	0x100e
    979d:	c3 00 c8    	addd	#0xc8
    97a0:	fd 10 1e    	std	0x101e
    97a3:	b6 10 22    	ldaa	0x1022
    97a6:	8a 08       	oraa	#0x8
    97a8:	b7 10 22    	staa	0x1022
    97ab:	39          	rts
    97ac:	48          	asla
    97ad:	24 1f       	bcc	0x0x97ce
    97af:	bd 9d 8a    	jsr	0x9d8a
    97b2:	14 f3 08    	bset	*0xf3, #0x08
    97b5:	96 f2       	ldaa	*0xf2
    97b7:	84 20       	anda	#0x20
    97b9:	8a 01       	oraa	#0x1
    97bb:	97 f2       	staa	*0xf2
    97bd:	c6 1a       	ldab	#0x1a
    97bf:	f7 01 1b    	stab	0x11b
    97c2:	86 02       	ldaa	#0x2
    97c4:	97 ff       	staa	*0xff
    97c6:	86 80       	ldaa	#0x80
    97c8:	b7 01 1c    	staa	0x11c
    97cb:	7e 9d 0b    	jmp	0x9d0b
    97ce:	7e 9d 0b    	jmp	0x9d0b
    97d1:	96 73       	ldaa	*0x73
    97d3:	4c          	inca
    97d4:	81 04       	cmpa	#0x4
    97d6:	23 02       	bls	0x0x97da
    97d8:	86 01       	ldaa	#0x1
    97da:	97 73       	staa	*0x73
    97dc:	d6 f3       	ldab	*0xf3
    97de:	c4 f8       	andb	#0xf8
    97e0:	1b          	aba
    97e1:	97 f3       	staa	*0xf3
    97e3:	7d 00 fb    	tst	0xfb
    97e6:	26 0b       	bne	0x0x97f3
    97e8:	14 fb 01    	bset	*0xfb, #0x01
    97eb:	14 f2 20    	bset	*0xf2, #0x20
    97ee:	86 1f       	ldaa	#0x1f
    97f0:	b7 01 66    	staa	0x166
    97f3:	7e 9d 0b    	jmp	0x9d0b
    97f6:	86 08       	ldaa	#0x8
    97f8:	b7 10 44    	staa	0x1044
    97fb:	01          	nop
    97fc:	01          	nop
    97fd:	b6 10 45    	ldaa	0x1045
    9800:	b1 01 77    	cmpa	0x177
    9803:	26 03       	bne	0x0x9808
    9805:	7e 98 f4    	jmp	0x98f4
    9808:	16          	tab
    9809:	b8 01 77    	eora	0x177
    980c:	f7 01 77    	stab	0x177
    980f:	b4 01 77    	anda	0x177
    9812:	26 03       	bne	0x0x9817
    9814:	7e 98 f4    	jmp	0x98f4
    9817:	44          	lsra
    9818:	24 50       	bcc	0x0x986a
    981a:	12 f4 01 10 	brset	*0xf4, #0x01, 0x0x982e
    981e:	bd 9d 8a    	jsr	0x9d8a
    9821:	14 f4 01    	bset	*0xf4, #0x01
    9824:	c6 1b       	ldab	#0x1b
    9826:	96 f2       	ldaa	*0xf2
    9828:	84 20       	anda	#0x20
    982a:	8a 01       	oraa	#0x1
    982c:	20 2b       	bra	0x0x9859
    982e:	96 f2       	ldaa	*0xf2
    9830:	84 20       	anda	#0x20
    9832:	c6 1b       	ldab	#0x1b
    9834:	f1 01 1b    	cmpb	0x11b
    9837:	27 12       	beq	0x0x984b
    9839:	5c          	incb
    983a:	f1 01 1b    	cmpb	0x11b
    983d:	27 11       	beq	0x0x9850
    983f:	5c          	incb
    9840:	f1 01 1b    	cmpb	0x11b
    9843:	27 10       	beq	0x0x9855
    9845:	c6 1b       	ldab	#0x1b
    9847:	8a 01       	oraa	#0x1
    9849:	20 0e       	bra	0x0x9859
    984b:	5c          	incb
    984c:	8a 02       	oraa	#0x2
    984e:	20 09       	bra	0x0x9859
    9850:	5c          	incb
    9851:	8a 04       	oraa	#0x4
    9853:	20 04       	bra	0x0x9859
    9855:	c6 1b       	ldab	#0x1b
    9857:	8a 01       	oraa	#0x1
    9859:	97 f2       	staa	*0xf2
    985b:	f7 01 1b    	stab	0x11b
    985e:	86 02       	ldaa	#0x2
    9860:	97 ff       	staa	*0xff
    9862:	86 80       	ldaa	#0x80
    9864:	b7 01 1c    	staa	0x11c
    9867:	7e 9d 0b    	jmp	0x9d0b
    986a:	44          	lsra
    986b:	24 14       	bcc	0x0x9881
    986d:	12 f4 02 bd 	brset	*0xf4, #0x02, 0x0x982e
    9871:	bd 9d 8a    	jsr	0x9d8a
    9874:	14 f4 02    	bset	*0xf4, #0x02
    9877:	c6 1b       	ldab	#0x1b
    9879:	96 f2       	ldaa	*0xf2
    987b:	84 20       	anda	#0x20
    987d:	8a 01       	oraa	#0x1
    987f:	20 d8       	bra	0x0x9859
    9881:	44          	lsra
    9882:	24 1e       	bcc	0x0x98a2
    9884:	7d 01 75    	tst	0x175
    9887:	2a 03       	bpl	0x0x988c
    9889:	7e a0 2a    	jmp	0xa02a
    988c:	96 f8       	ldaa	*0xf8
    988e:	84 fc       	anda	#0xfc
    9890:	12 f8 01 07 	brset	*0xf8, #0x01, 0x0x989b
    9894:	8a 01       	oraa	#0x1
    9896:	97 f8       	staa	*0xf8
    9898:	7e 9d 0b    	jmp	0x9d0b
    989b:	8a 02       	oraa	#0x2
    989d:	97 f8       	staa	*0xf8
    989f:	7e 9d 0b    	jmp	0x9d0b
    98a2:	44          	lsra
    98a3:	24 3a       	bcc	0x0x98df
    98a5:	20 25       	bra	0x0x98cc
    98a7:	7d 01 75    	tst	0x175
    98aa:	2a 20       	bpl	0x0x98cc
    98ac:	bd 9d 8a    	jsr	0x9d8a
    98af:	c6 23       	ldab	#0x23
    98b1:	f1 01 1b    	cmpb	0x11b
    98b4:	26 03       	bne	0x0x98b9
    98b6:	7e 9d 0b    	jmp	0x9d0b
    98b9:	f7 01 1b    	stab	0x11b
    98bc:	86 01       	ldaa	#0x1
    98be:	97 f2       	staa	*0xf2
    98c0:	86 02       	ldaa	#0x2
    98c2:	97 ff       	staa	*0xff
    98c4:	86 80       	ldaa	#0x80
    98c6:	b7 01 1c    	staa	0x11c
    98c9:	7e 9d 0b    	jmp	0x9d0b
    98cc:	d6 f8       	ldab	*0xf8
    98ce:	c4 0f       	andb	#0xf
    98d0:	96 f8       	ldaa	*0xf8
    98d2:	84 f0       	anda	#0xf0
    98d4:	48          	asla
    98d5:	24 02       	bcc	0x0x98d9
    98d7:	86 10       	ldaa	#0x10
    98d9:	1b          	aba
    98da:	97 f8       	staa	*0xf8
    98dc:	7e 9d 0b    	jmp	0x9d0b
    98df:	d6 f8       	ldab	*0xf8
    98e1:	c4 f3       	andb	#0xf3
    98e3:	96 f8       	ldaa	*0xf8
    98e5:	84 0c       	anda	#0xc
    98e7:	48          	asla
    98e8:	81 10       	cmpa	#0x10
    98ea:	26 02       	bne	0x0x98ee
    98ec:	86 04       	ldaa	#0x4
    98ee:	1b          	aba
    98ef:	97 f8       	staa	*0xf8
    98f1:	7e 9d 0b    	jmp	0x9d0b
    98f4:	86 04       	ldaa	#0x4
    98f6:	b7 10 44    	staa	0x1044
    98f9:	01          	nop
    98fa:	01          	nop
    98fb:	b6 10 45    	ldaa	0x1045
    98fe:	b1 01 78    	cmpa	0x178
    9901:	26 03       	bne	0x0x9906
    9903:	7e 9b 4c    	jmp	0x9b4c
    9906:	16          	tab
    9907:	b8 01 78    	eora	0x178
    990a:	f7 01 78    	stab	0x178
    990d:	b4 01 78    	anda	0x178
    9910:	26 03       	bne	0x0x9915
    9912:	7e 9b 4c    	jmp	0x9b4c
    9915:	48          	asla
    9916:	20 00       	bra	0x0x9918
    9918:	48          	asla
    9919:	24 6f       	bcc	0x0x998a
    991b:	86 02       	ldaa	#0x2
    991d:	b1 01 1b    	cmpa	0x11b
    9920:	26 15       	bne	0x0x9937
    9922:	96 f2       	ldaa	*0xf2
    9924:	84 20       	anda	#0x20
    9926:	8a 02       	oraa	#0x2
    9928:	97 f2       	staa	*0xf2
    992a:	86 03       	ldaa	#0x3
    992c:	b7 01 1b    	staa	0x11b
    992f:	86 80       	ldaa	#0x80
    9931:	b7 01 1c    	staa	0x11c
    9934:	7e 9d 0b    	jmp	0x9d0b
    9937:	86 03       	ldaa	#0x3
    9939:	b1 01 1b    	cmpa	0x11b
    993c:	26 14       	bne	0x0x9952
    993e:	4c          	inca
    993f:	b7 01 1b    	staa	0x11b
    9942:	96 f2       	ldaa	*0xf2
    9944:	84 20       	anda	#0x20
    9946:	8a 04       	oraa	#0x4
    9948:	97 f2       	staa	*0xf2
    994a:	86 80       	ldaa	#0x80
    994c:	b7 01 1c    	staa	0x11c
    994f:	7e 9d 0b    	jmp	0x9d0b
    9952:	86 04       	ldaa	#0x4
    9954:	b1 01 1b    	cmpa	0x11b
    9957:	26 14       	bne	0x0x996d
    9959:	4c          	inca
    995a:	b7 01 1b    	staa	0x11b
    995d:	96 f2       	ldaa	*0xf2
    995f:	84 20       	anda	#0x20
    9961:	8a 48       	oraa	#0x48
    9963:	97 f2       	staa	*0xf2
    9965:	86 80       	ldaa	#0x80
    9967:	b7 01 1c    	staa	0x11c
    996a:	7e 9d 0b    	jmp	0x9d0b
    996d:	86 02       	ldaa	#0x2
    996f:	b7 01 1b    	staa	0x11b
    9972:	97 ff       	staa	*0xff
    9974:	bd 9d 8a    	jsr	0x9d8a
    9977:	96 f2       	ldaa	*0xf2
    9979:	84 20       	anda	#0x20
    997b:	8a 01       	oraa	#0x1
    997d:	97 f2       	staa	*0xf2
    997f:	14 f5 40    	bset	*0xf5, #0x40
    9982:	86 80       	ldaa	#0x80
    9984:	b7 01 1c    	staa	0x11c
    9987:	7e 9d 0b    	jmp	0x9d0b
    998a:	48          	asla
    998b:	48          	asla
    998c:	25 03       	bcs	0x0x9991
    998e:	7e 9a 3d    	jmp	0x9a3d
    9991:	96 f9       	ldaa	*0xf9
    9993:	81 02       	cmpa	#0x2
    9995:	27 34       	beq	0x0x99cb
    9997:	b6 01 75    	ldaa	0x175
    999a:	85 10       	bita	#0x10
    999c:	26 03       	bne	0x0x99a1
    999e:	7e 9d 0b    	jmp	0x9d0b
    99a1:	86 02       	ldaa	#0x2
    99a3:	97 f9       	staa	*0xf9
    99a5:	96 fa       	ldaa	*0xfa
    99a7:	97 fc       	staa	*0xfc
    99a9:	bd 9a 5c    	jsr	0x9a5c
    99ac:	cc 01 09    	ldd	#0x109
    99af:	fd 01 13    	std	0x113
    99b2:	86 80       	ldaa	#0x80
    99b4:	b7 01 1c    	staa	0x11c
    99b7:	bd a4 f9    	jsr	0xa4f9
    99ba:	7f 00 fb    	clr	0xfb
    99bd:	7f 00 f2    	clr	0xf2
    99c0:	bd 9f 35    	jsr	0x9f35
    99c3:	b6 50 00    	ldaa	0x5000
    99c6:	97 df       	staa	*0xdf
    99c8:	7e 9d 0b    	jmp	0x9d0b
    99cb:	f6 01 1b    	ldab	0x11b
    99ce:	c1 0f       	cmpb	#0xf
    99d0:	26 14       	bne	0x0x99e6
    99d2:	5c          	incb
    99d3:	f7 01 1b    	stab	0x11b
    99d6:	96 f2       	ldaa	*0xf2
    99d8:	84 20       	anda	#0x20
    99da:	8a 02       	oraa	#0x2
    99dc:	97 f2       	staa	*0xf2
    99de:	86 80       	ldaa	#0x80
    99e0:	b7 01 1c    	staa	0x11c
    99e3:	7e 9d 0b    	jmp	0x9d0b
    99e6:	c1 10       	cmpb	#0x10
    99e8:	26 14       	bne	0x0x99fe
    99ea:	5c          	incb
    99eb:	f7 01 1b    	stab	0x11b
    99ee:	96 f2       	ldaa	*0xf2
    99f0:	84 20       	anda	#0x20
    99f2:	8a 04       	oraa	#0x4
    99f4:	97 f2       	staa	*0xf2
    99f6:	86 80       	ldaa	#0x80
    99f8:	b7 01 1c    	staa	0x11c
    99fb:	7e 9d 0b    	jmp	0x9d0b
    99fe:	c1 11       	cmpb	#0x11
    9a00:	26 07       	bne	0x0x9a09
    9a02:	b6 50 00    	ldaa	0x5000
    9a05:	81 01       	cmpa	#0x1
    9a07:	27 1f       	beq	0x0x9a28
    9a09:	bd 9d 8a    	jsr	0x9d8a
    9a0c:	86 0f       	ldaa	#0xf
    9a0e:	b7 01 1b    	staa	0x11b
    9a11:	14 f5 10    	bset	*0xf5, #0x10
    9a14:	86 02       	ldaa	#0x2
    9a16:	97 ff       	staa	*0xff
    9a18:	96 f2       	ldaa	*0xf2
    9a1a:	84 20       	anda	#0x20
    9a1c:	8a 01       	oraa	#0x1
    9a1e:	97 f2       	staa	*0xf2
    9a20:	86 80       	ldaa	#0x80
    9a22:	b7 01 1c    	staa	0x11c
    9a25:	7e 9d 0b    	jmp	0x9d0b
    9a28:	86 12       	ldaa	#0x12
    9a2a:	b7 01 1b    	staa	0x11b
    9a2d:	96 f2       	ldaa	*0xf2
    9a2f:	84 20       	anda	#0x20
    9a31:	8a 08       	oraa	#0x8
    9a33:	97 f2       	staa	*0xf2
    9a35:	86 80       	ldaa	#0x80
    9a37:	b7 01 1c    	staa	0x11c
    9a3a:	7e 9d 0b    	jmp	0x9d0b
    9a3d:	48          	asla
    9a3e:	24 4f       	bcc	0x0x9a8f
    9a40:	86 20       	ldaa	#0x20
    9a42:	98 6a       	eora	*0x6a
    9a44:	97 6a       	staa	*0x6a
    9a46:	13 6a 20 05 	brclr	*0x6a, #0x20, 0x0x9a4f
    9a4a:	14 f5 08    	bset	*0xf5, #0x08
    9a4d:	20 03       	bra	0x0x9a52
    9a4f:	15 f5 08    	bclr	*0xf5, #0x08
    9a52:	c6 6a       	ldab	#0x6a
    9a54:	bd a8 5a    	jsr	0xa85a
    9a57:	8d 03       	bsr	0x0x9a5c
    9a59:	7e a4 b7    	jmp	0xa4b7
    9a5c:	7f 00 f0    	clr	0xf0
    9a5f:	ce 01 01    	ldx	#0x101
    9a62:	ff 01 10    	stx	0x110
    9a65:	c6 10       	ldab	#0x10
    9a67:	09          	dex
    9a68:	6f 00       	clr	0x0,x
    9a6a:	08          	inx
    9a6b:	5a          	decb
    9a6c:	26 fa       	bne	0x0x9a68
    9a6e:	7d 00 d0    	tst	0xd0
    9a71:	27 05       	beq	0x0x9a78
    9a73:	7f 00 d0    	clr	0xd0
    9a76:	20 08       	bra	0x0x9a80
    9a78:	7d 10 29    	tst	0x1029
    9a7b:	2a fb       	bpl	0x0x9a78
    9a7d:	f6 10 2a    	ldab	0x102a
    9a80:	5f          	clrb
    9a81:	f7 10 42    	stab	0x1042
    9a84:	01          	nop
    9a85:	01          	nop
    9a86:	86 80       	ldaa	#0x80
    9a88:	b7 10 2a    	staa	0x102a
    9a8b:	15 f5 a0    	bclr	*0xf5, #0xa0
    9a8e:	39          	rts
    9a8f:	48          	asla
    9a90:	48          	asla
    9a91:	24 6e       	bcc	0x0x9b01
    9a93:	86 09       	ldaa	#0x9
    9a95:	b1 01 1b    	cmpa	0x11b
    9a98:	26 14       	bne	0x0x9aae
    9a9a:	d6 f2       	ldab	*0xf2
    9a9c:	c4 20       	andb	#0x20
    9a9e:	ca 02       	orab	#0x2
    9aa0:	d7 f2       	stab	*0xf2
    9aa2:	4c          	inca
    9aa3:	b7 01 1b    	staa	0x11b
    9aa6:	86 80       	ldaa	#0x80
    9aa8:	b7 01 1c    	staa	0x11c
    9aab:	7e 9d 0b    	jmp	0x9d0b
    9aae:	4c          	inca
    9aaf:	b1 01 1b    	cmpa	0x11b
    9ab2:	26 14       	bne	0x0x9ac8
    9ab4:	4c          	inca
    9ab5:	b7 01 1b    	staa	0x11b
    9ab8:	96 f2       	ldaa	*0xf2
    9aba:	84 20       	anda	#0x20
    9abc:	8a 04       	oraa	#0x4
    9abe:	97 f2       	staa	*0xf2
    9ac0:	86 80       	ldaa	#0x80
    9ac2:	b7 01 1c    	staa	0x11c
    9ac5:	7e 9d 0b    	jmp	0x9d0b
    9ac8:	4c          	inca
    9ac9:	b1 01 1b    	cmpa	0x11b
    9acc:	26 14       	bne	0x0x9ae2
    9ace:	4c          	inca
    9acf:	b7 01 1b    	staa	0x11b
    9ad2:	96 f2       	ldaa	*0xf2
    9ad4:	84 20       	anda	#0x20
    9ad6:	8a 08       	oraa	#0x8
    9ad8:	97 f2       	staa	*0xf2
    9ada:	86 80       	ldaa	#0x80
    9adc:	b7 01 1c    	staa	0x11c
    9adf:	7e 9d 0b    	jmp	0x9d0b
    9ae2:	86 09       	ldaa	#0x9
    9ae4:	b7 01 1b    	staa	0x11b
    9ae7:	86 02       	ldaa	#0x2
    9ae9:	97 ff       	staa	*0xff
    9aeb:	bd 9d 8a    	jsr	0x9d8a
    9aee:	96 f2       	ldaa	*0xf2
    9af0:	84 20       	anda	#0x20
    9af2:	8a 01       	oraa	#0x1
    9af4:	97 f2       	staa	*0xf2
    9af6:	14 f5 02    	bset	*0xf5, #0x02
    9af9:	86 80       	ldaa	#0x80
    9afb:	b7 01 1c    	staa	0x11c
    9afe:	7e 9d 0b    	jmp	0x9d0b
    9b01:	96 f2       	ldaa	*0xf2
    9b03:	84 20       	anda	#0x20
    9b05:	c6 1f       	ldab	#0x1f
    9b07:	f1 01 1b    	cmpb	0x11b
    9b0a:	26 05       	bne	0x0x9b11
    9b0c:	5c          	incb
    9b0d:	8a 02       	oraa	#0x2
    9b0f:	20 27       	bra	0x0x9b38
    9b11:	5c          	incb
    9b12:	f1 01 1b    	cmpb	0x11b
    9b15:	26 05       	bne	0x0x9b1c
    9b17:	5c          	incb
    9b18:	8a 04       	oraa	#0x4
    9b1a:	20 1c       	bra	0x0x9b38
    9b1c:	5c          	incb
    9b1d:	f1 01 1b    	cmpb	0x11b
    9b20:	26 05       	bne	0x0x9b27
    9b22:	5c          	incb
    9b23:	8a 08       	oraa	#0x8
    9b25:	20 11       	bra	0x0x9b38
    9b27:	bd 9d 8a    	jsr	0x9d8a
    9b2a:	86 01       	ldaa	#0x1
    9b2c:	9a f5       	oraa	*0xf5
    9b2e:	97 f5       	staa	*0xf5
    9b30:	96 f2       	ldaa	*0xf2
    9b32:	84 20       	anda	#0x20
    9b34:	8a 01       	oraa	#0x1
    9b36:	c6 1f       	ldab	#0x1f
    9b38:	97 f2       	staa	*0xf2
    9b3a:	f7 01 1b    	stab	0x11b
    9b3d:	86 02       	ldaa	#0x2
    9b3f:	97 ff       	staa	*0xff
    9b41:	86 80       	ldaa	#0x80
    9b43:	b7 01 1c    	staa	0x11c
    9b46:	7e 9d 0b    	jmp	0x9d0b
    9b49:	7e 9d 0b    	jmp	0x9d0b
    9b4c:	86 02       	ldaa	#0x2
    9b4e:	b7 10 44    	staa	0x1044
    9b51:	01          	nop
    9b52:	01          	nop
    9b53:	b6 10 45    	ldaa	0x1045
    9b56:	b1 01 79    	cmpa	0x179
    9b59:	26 03       	bne	0x0x9b5e
    9b5b:	7e 9b c8    	jmp	0x9bc8
    9b5e:	16          	tab
    9b5f:	b8 01 79    	eora	0x179
    9b62:	f7 01 79    	stab	0x179
    9b65:	b4 01 79    	anda	0x179
    9b68:	26 03       	bne	0x0x9b6d
    9b6a:	7e 9b c8    	jmp	0x9bc8
    9b6d:	44          	lsra
    9b6e:	24 39       	bcc	0x0x9ba9
    9b70:	86 0d       	ldaa	#0xd
    9b72:	b1 01 1b    	cmpa	0x11b
    9b75:	27 1d       	beq	0x0x9b94
    9b77:	b7 01 1b    	staa	0x11b
    9b7a:	bd 9d 8a    	jsr	0x9d8a
    9b7d:	14 f6 05    	bset	*0xf6, #0x05
    9b80:	86 02       	ldaa	#0x2
    9b82:	97 ff       	staa	*0xff
    9b84:	86 80       	ldaa	#0x80
    9b86:	b7 01 1c    	staa	0x11c
    9b89:	96 f2       	ldaa	*0xf2
    9b8b:	84 20       	anda	#0x20
    9b8d:	8a 01       	oraa	#0x1
    9b8f:	97 f2       	staa	*0xf2
    9b91:	7e 9d 0b    	jmp	0x9d0b
    9b94:	96 f6       	ldaa	*0xf6
    9b96:	84 fc       	anda	#0xfc
    9b98:	48          	asla
    9b99:	24 02       	bcc	0x0x9b9d
    9b9b:	86 04       	ldaa	#0x4
    9b9d:	8a 01       	oraa	#0x1
    9b9f:	97 f6       	staa	*0xf6
    9ba1:	86 80       	ldaa	#0x80
    9ba3:	b7 01 1c    	staa	0x11c
    9ba6:	7e 9d 0b    	jmp	0x9d0b
    9ba9:	bd 9d 8a    	jsr	0x9d8a
    9bac:	14 f6 02    	bset	*0xf6, #0x02
    9baf:	96 f2       	ldaa	*0xf2
    9bb1:	84 20       	anda	#0x20
    9bb3:	8a 01       	oraa	#0x1
    9bb5:	97 f2       	staa	*0xf2
    9bb7:	86 0e       	ldaa	#0xe
    9bb9:	b7 01 1b    	staa	0x11b
    9bbc:	86 02       	ldaa	#0x2
    9bbe:	97 ff       	staa	*0xff
    9bc0:	86 80       	ldaa	#0x80
    9bc2:	b7 01 1c    	staa	0x11c
    9bc5:	7e 9d 0b    	jmp	0x9d0b
    9bc8:	86 01       	ldaa	#0x1
    9bca:	b7 10 44    	staa	0x1044
    9bcd:	01          	nop
    9bce:	01          	nop
    9bcf:	b6 10 45    	ldaa	0x1045
    9bd2:	b1 01 7a    	cmpa	0x17a
    9bd5:	26 03       	bne	0x0x9bda
    9bd7:	7e 9d 0b    	jmp	0x9d0b
    9bda:	16          	tab
    9bdb:	b8 01 7a    	eora	0x17a
    9bde:	f7 01 7a    	stab	0x17a
    9be1:	b4 01 7a    	anda	0x17a
    9be4:	26 03       	bne	0x0x9be9
    9be6:	7e 9d 0b    	jmp	0x9d0b
    9be9:	48          	asla
    9bea:	24 20       	bcc	0x0x9c0c
    9bec:	86 80       	ldaa	#0x80
    9bee:	98 f7       	eora	*0xf7
    9bf0:	97 f7       	staa	*0xf7
    9bf2:	86 10       	ldaa	#0x10
    9bf4:	98 41       	eora	*0x41
    9bf6:	97 41       	staa	*0x41
    9bf8:	c6 41       	ldab	#0x41
    9bfa:	bd a8 5a    	jsr	0xa85a
    9bfd:	4f          	clra
    9bfe:	c6 76       	ldab	#0x76
    9c00:	13 41 10 02 	brclr	*0x41, #0x10, 0x0x9c06
    9c04:	86 7f       	ldaa	#0x7f
    9c06:	bd a8 a2    	jsr	0xa8a2
    9c09:	7e 9c fb    	jmp	0x9cfb
    9c0c:	48          	asla
    9c0d:	24 20       	bcc	0x0x9c2f
    9c0f:	86 40       	ldaa	#0x40
    9c11:	98 f7       	eora	*0xf7
    9c13:	97 f7       	staa	*0xf7
    9c15:	86 40       	ldaa	#0x40
    9c17:	98 44       	eora	*0x44
    9c19:	97 44       	staa	*0x44
    9c1b:	c6 44       	ldab	#0x44
    9c1d:	bd a8 5a    	jsr	0xa85a
    9c20:	4f          	clra
    9c21:	c6 3f       	ldab	#0x3f
    9c23:	13 44 40 02 	brclr	*0x44, #0x40, 0x0x9c29
    9c27:	86 7f       	ldaa	#0x7f
    9c29:	bd a8 a2    	jsr	0xa8a2
    9c2c:	7e 9c fb    	jmp	0x9cfb
    9c2f:	48          	asla
    9c30:	24 20       	bcc	0x0x9c52
    9c32:	86 20       	ldaa	#0x20
    9c34:	98 f7       	eora	*0xf7
    9c36:	97 f7       	staa	*0xf7
    9c38:	86 20       	ldaa	#0x20
    9c3a:	98 44       	eora	*0x44
    9c3c:	97 44       	staa	*0x44
    9c3e:	c6 44       	ldab	#0x44
    9c40:	bd a8 5a    	jsr	0xa85a
    9c43:	4f          	clra
    9c44:	c6 3e       	ldab	#0x3e
    9c46:	13 44 20 02 	brclr	*0x44, #0x20, 0x0x9c4c
    9c4a:	86 7f       	ldaa	#0x7f
    9c4c:	bd a8 a2    	jsr	0xa8a2
    9c4f:	7e 9c fb    	jmp	0x9cfb
    9c52:	48          	asla
    9c53:	24 20       	bcc	0x0x9c75
    9c55:	86 10       	ldaa	#0x10
    9c57:	98 f7       	eora	*0xf7
    9c59:	97 f7       	staa	*0xf7
    9c5b:	86 10       	ldaa	#0x10
    9c5d:	98 44       	eora	*0x44
    9c5f:	97 44       	staa	*0x44
    9c61:	c6 44       	ldab	#0x44
    9c63:	bd a8 5a    	jsr	0xa85a
    9c66:	4f          	clra
    9c67:	c6 3d       	ldab	#0x3d
    9c69:	13 44 10 02 	brclr	*0x44, #0x10, 0x0x9c6f
    9c6d:	86 7f       	ldaa	#0x7f
    9c6f:	bd a8 a2    	jsr	0xa8a2
    9c72:	7e 9c fb    	jmp	0x9cfb
    9c75:	48          	asla
    9c76:	24 20       	bcc	0x0x9c98
    9c78:	86 08       	ldaa	#0x8
    9c7a:	98 f7       	eora	*0xf7
    9c7c:	97 f7       	staa	*0xf7
    9c7e:	86 08       	ldaa	#0x8
    9c80:	98 44       	eora	*0x44
    9c82:	97 44       	staa	*0x44
    9c84:	c6 44       	ldab	#0x44
    9c86:	bd a8 5a    	jsr	0xa85a
    9c89:	4f          	clra
    9c8a:	c6 3a       	ldab	#0x3a
    9c8c:	13 44 08 02 	brclr	*0x44, #0x08, 0x0x9c92
    9c90:	86 7f       	ldaa	#0x7f
    9c92:	bd a8 a2    	jsr	0xa8a2
    9c95:	7e 9c fb    	jmp	0x9cfb
    9c98:	48          	asla
    9c99:	24 20       	bcc	0x0x9cbb
    9c9b:	86 04       	ldaa	#0x4
    9c9d:	98 f7       	eora	*0xf7
    9c9f:	97 f7       	staa	*0xf7
    9ca1:	86 04       	ldaa	#0x4
    9ca3:	98 44       	eora	*0x44
    9ca5:	97 44       	staa	*0x44
    9ca7:	c6 44       	ldab	#0x44
    9ca9:	bd a8 5a    	jsr	0xa85a
    9cac:	4f          	clra
    9cad:	c6 39       	ldab	#0x39
    9caf:	13 44 04 02 	brclr	*0x44, #0x04, 0x0x9cb5
    9cb3:	86 7f       	ldaa	#0x7f
    9cb5:	bd a8 a2    	jsr	0xa8a2
    9cb8:	7e 9c fb    	jmp	0x9cfb
    9cbb:	48          	asla
    9cbc:	24 20       	bcc	0x0x9cde
    9cbe:	86 02       	ldaa	#0x2
    9cc0:	98 f7       	eora	*0xf7
    9cc2:	97 f7       	staa	*0xf7
    9cc4:	86 02       	ldaa	#0x2
    9cc6:	98 44       	eora	*0x44
    9cc8:	97 44       	staa	*0x44
    9cca:	c6 44       	ldab	#0x44
    9ccc:	bd a8 5a    	jsr	0xa85a
    9ccf:	4f          	clra
    9cd0:	c6 38       	ldab	#0x38
    9cd2:	13 44 02 02 	brclr	*0x44, #0x02, 0x0x9cd8
    9cd6:	86 7f       	ldaa	#0x7f
    9cd8:	bd a8 a2    	jsr	0xa8a2
    9cdb:	7e 9c fb    	jmp	0x9cfb
    9cde:	86 01       	ldaa	#0x1
    9ce0:	98 f7       	eora	*0xf7
    9ce2:	97 f7       	staa	*0xf7
    9ce4:	86 01       	ldaa	#0x1
    9ce6:	98 44       	eora	*0x44
    9ce8:	97 44       	staa	*0x44
    9cea:	c6 44       	ldab	#0x44
    9cec:	bd a8 5a    	jsr	0xa85a
    9cef:	4f          	clra
    9cf0:	c6 37       	ldab	#0x37
    9cf2:	13 44 01 02 	brclr	*0x44, #0x01, 0x0x9cf8
    9cf6:	86 7f       	ldaa	#0x7f
    9cf8:	bd a8 a2    	jsr	0xa8a2
    9cfb:	7d 00 fb    	tst	0xfb
    9cfe:	26 0b       	bne	0x0x9d0b
    9d00:	14 fb 01    	bset	*0xfb, #0x01
    9d03:	14 f2 20    	bset	*0xf2, #0x20
    9d06:	86 1f       	ldaa	#0x1f
    9d08:	b7 01 66    	staa	0x166
    9d0b:	86 40       	ldaa	#0x40
    9d0d:	b7 10 23    	staa	0x1023
    9d10:	fc 10 0e    	ldd	0x100e
    9d13:	c3 4e 20    	addd	#0x4e20
    9d16:	fd 10 18    	std	0x1018
    9d19:	fe 01 c0    	ldx	0x1c0
    9d1c:	bc 01 c2    	cpx	0x1c2
    9d1f:	26 03       	bne	0x0x9d24
    9d21:	7e a3 77    	jmp	0xa377
    9d24:	18 fe 01 6c 	ldy	0x16c
    9d28:	18 3c       	pshy
    9d2a:	7e 8e d5    	jmp	0x8ed5
    9d2d:	96 fa       	ldaa	*0xfa
    9d2f:	4c          	inca
    9d30:	f6 01 77    	ldab	0x177
    9d33:	c5 04       	bitb	#0x4
    9d35:	27 02       	beq	0x0x9d39
    9d37:	8b 09       	adda	#0x9
    9d39:	84 7f       	anda	#0x7f
    9d3b:	81 51       	cmpa	#0x51
    9d3d:	26 04       	bne	0x0x9d43
    9d3f:	86 53       	ldaa	#0x53
    9d41:	20 06       	bra	0x0x9d49
    9d43:	81 52       	cmpa	#0x52
    9d45:	26 02       	bne	0x0x9d49
    9d47:	86 53       	ldaa	#0x53
    9d49:	97 fa       	staa	*0xfa
    9d4b:	7c 01 7e    	inc	0x17e
    9d4e:	39          	rts
    9d4f:	96 fa       	ldaa	*0xfa
    9d51:	4a          	deca
    9d52:	f6 01 77    	ldab	0x177
    9d55:	c5 04       	bitb	#0x4
    9d57:	27 02       	beq	0x0x9d5b
    9d59:	80 09       	suba	#0x9
    9d5b:	84 7f       	anda	#0x7f
    9d5d:	81 52       	cmpa	#0x52
    9d5f:	26 04       	bne	0x0x9d65
    9d61:	86 50       	ldaa	#0x50
    9d63:	20 05       	bra	0x0x9d6a
    9d65:	81 51       	cmpa	#0x51
    9d67:	26 01       	bne	0x0x9d6a
    9d69:	4a          	deca
    9d6a:	97 fa       	staa	*0xfa
    9d6c:	7c 01 7e    	inc	0x17e
    9d6f:	39          	rts
    9d70:	b6 01 6a    	ldaa	0x16a
    9d73:	4c          	inca
    9d74:	84 7f       	anda	#0x7f
    9d76:	b7 01 6a    	staa	0x16a
    9d79:	7c 01 7e    	inc	0x17e
    9d7c:	39          	rts
    9d7d:	b6 01 6a    	ldaa	0x16a
    9d80:	4a          	deca
    9d81:	84 7f       	anda	#0x7f
    9d83:	b7 01 6a    	staa	0x16a
    9d86:	7c 01 7e    	inc	0x17e
    9d89:	39          	rts
    9d8a:	86 10       	ldaa	#0x10
    9d8c:	94 f3       	anda	*0xf3
    9d8e:	97 f3       	staa	*0xf3
    9d90:	86 a8       	ldaa	#0xa8
    9d92:	94 f5       	anda	*0xf5
    9d94:	97 f5       	staa	*0xf5
    9d96:	4f          	clra
    9d97:	97 f4       	staa	*0xf4
    9d99:	97 f6       	staa	*0xf6
    9d9b:	39          	rts
    9d9c:	96 fa       	ldaa	*0xfa
    9d9e:	c6 b0       	ldab	#0xb0
    9da0:	3d          	mul
    9da1:	c3 20 00    	addd	#0x2000
    9da4:	8f          	xgdx
    9da5:	18 ce 00 20 	ldy	#0x20
    9da9:	c6 b0       	ldab	#0xb0
    9dab:	a6 00       	ldaa	0x0,x
    9dad:	84 7f       	anda	#0x7f
    9daf:	18 a7 00    	staa	0x0,y
    9db2:	08          	inx
    9db3:	18 08       	iny
    9db5:	5a          	decb
    9db6:	26 f3       	bne	0x0x9dab
    9db8:	96 41       	ldaa	*0x41
    9dba:	84 10       	anda	#0x10
    9dbc:	48          	asla
    9dbd:	48          	asla
    9dbe:	48          	asla
    9dbf:	9a 44       	oraa	*0x44
    9dc1:	97 f7       	staa	*0xf7
    9dc3:	7d 00 d0    	tst	0xd0
    9dc6:	27 05       	beq	0x0x9dcd
    9dc8:	7f 00 d0    	clr	0xd0
    9dcb:	20 08       	bra	0x0x9dd5
    9dcd:	7d 10 29    	tst	0x1029
    9dd0:	2a fb       	bpl	0x0x9dcd
    9dd2:	b6 10 2a    	ldaa	0x102a
    9dd5:	5f          	clrb
    9dd6:	96 f9       	ldaa	*0xf9
    9dd8:	81 02       	cmpa	#0x2
    9dda:	26 08       	bne	0x0x9de4
    9ddc:	c6 01       	ldab	#0x1
    9dde:	7d 01 6b    	tst	0x16b
    9de1:	26 01       	bne	0x0x9de4
    9de3:	5c          	incb
    9de4:	f7 10 42    	stab	0x1042
    9de7:	7c 00 d0    	inc	0xd0
    9dea:	13 f3 10 08 	brclr	*0xf3, #0x10, 0x0x9df6
    9dee:	96 6a       	ldaa	*0x6a
    9df0:	84 20       	anda	#0x20
    9df2:	44          	lsra
    9df3:	44          	lsra
    9df4:	97 f5       	staa	*0xf5
    9df6:	96 6a       	ldaa	*0x6a
    9df8:	84 20       	anda	#0x20
    9dfa:	44          	lsra
    9dfb:	44          	lsra
    9dfc:	d6 f5       	ldab	*0xf5
    9dfe:	c4 08       	andb	#0x8
    9e00:	10          	sba
    9e01:	26 0f       	bne	0x0x9e12
    9e03:	96 6a       	ldaa	*0x6a
    9e05:	84 20       	anda	#0x20
    9e07:	44          	lsra
    9e08:	44          	lsra
    9e09:	d6 f5       	ldab	*0xf5
    9e0b:	c4 a0       	andb	#0xa0
    9e0d:	1b          	aba
    9e0e:	97 f5       	staa	*0xf5
    9e10:	20 1b       	bra	0x0x9e2d
    9e12:	86 80       	ldaa	#0x80
    9e14:	b7 10 2a    	staa	0x102a
    9e17:	cc 01 01    	ldd	#0x101
    9e1a:	fd 01 10    	std	0x110
    9e1d:	4f          	clra
    9e1e:	97 d0       	staa	*0xd0
    9e20:	97 f0       	staa	*0xf0
    9e22:	b7 01 00    	staa	0x100
    9e25:	b7 01 01    	staa	0x101
    9e28:	15 f5 a0    	bclr	*0xf5, #0xa0
    9e2b:	20 d6       	bra	0x0x9e03
    9e2d:	c6 88       	ldab	#0x88
    9e2f:	ce 00 20    	ldx	#0x20
    9e32:	7d 00 d0    	tst	0xd0
    9e35:	27 05       	beq	0x0x9e3c
    9e37:	7f 00 d0    	clr	0xd0
    9e3a:	20 08       	bra	0x0x9e44
    9e3c:	7d 10 29    	tst	0x1029
    9e3f:	2a fb       	bpl	0x0x9e3c
    9e41:	b6 10 2a    	ldaa	0x102a
    9e44:	86 82       	ldaa	#0x82
    9e46:	b7 10 2a    	staa	0x102a
    9e49:	7d 10 29    	tst	0x1029
    9e4c:	2a fb       	bpl	0x0x9e49
    9e4e:	b6 10 2a    	ldaa	0x102a
    9e51:	a6 00       	ldaa	0x0,x
    9e53:	b7 10 2a    	staa	0x102a
    9e56:	08          	inx
    9e57:	5a          	decb
    9e58:	26 ef       	bne	0x0x9e49
    9e5a:	c6 fe       	ldab	#0xfe
    9e5c:	96 f9       	ldaa	*0xf9
    9e5e:	81 02       	cmpa	#0x2
    9e60:	26 08       	bne	0x0x9e6a
    9e62:	c6 01       	ldab	#0x1
    9e64:	7d 01 6b    	tst	0x16b
    9e67:	26 01       	bne	0x0x9e6a
    9e69:	5c          	incb
    9e6a:	7d 10 29    	tst	0x1029
    9e6d:	2a fb       	bpl	0x0x9e6a
    9e6f:	b6 10 2a    	ldaa	0x102a
    9e72:	f7 10 42    	stab	0x1042
    9e75:	01          	nop
    9e76:	01          	nop
    9e77:	a6 00       	ldaa	0x0,x
    9e79:	b7 10 2a    	staa	0x102a
    9e7c:	7d 10 29    	tst	0x1029
    9e7f:	2a fb       	bpl	0x0x9e7c
    9e81:	b6 10 2a    	ldaa	0x102a
    9e84:	01          	nop
    9e85:	01          	nop
    9e86:	01          	nop
    9e87:	01          	nop
    9e88:	a6 08       	ldaa	0x8,x
    9e8a:	b7 10 2a    	staa	0x102a
    9e8d:	7d 10 29    	tst	0x1029
    9e90:	2a fb       	bpl	0x0x9e8d
    9e92:	b6 10 2a    	ldaa	0x102a
    9e95:	01          	nop
    9e96:	01          	nop
    9e97:	a6 10       	ldaa	0x10,x
    9e99:	b7 10 2a    	staa	0x102a
    9e9c:	08          	inx
    9e9d:	0d          	sec
    9e9e:	59          	rolb
    9e9f:	c1 fb       	cmpb	#0xfb
    9ea1:	27 14       	beq	0x0x9eb7
    9ea3:	96 f9       	ldaa	*0xf9
    9ea5:	81 02       	cmpa	#0x2
    9ea7:	27 0e       	beq	0x0x9eb7
    9ea9:	7d 10 29    	tst	0x1029
    9eac:	2a fb       	bpl	0x0x9ea9
    9eae:	b6 10 2a    	ldaa	0x102a
    9eb1:	f7 10 42    	stab	0x1042
    9eb4:	01          	nop
    9eb5:	20 c0       	bra	0x0x9e77
    9eb7:	7d 10 29    	tst	0x1029
    9eba:	2a fb       	bpl	0x0x9eb7
    9ebc:	b6 10 2a    	ldaa	0x102a
    9ebf:	4f          	clra
    9ec0:	b7 10 42    	staa	0x1042
    9ec3:	01          	nop
    9ec4:	01          	nop
    9ec5:	b6 01 6e    	ldaa	0x16e
    9ec8:	b7 10 2a    	staa	0x102a
    9ecb:	7d 10 29    	tst	0x1029
    9ece:	2a fb       	bpl	0x0x9ecb
    9ed0:	86 89       	ldaa	#0x89
    9ed2:	b7 10 2a    	staa	0x102a
    9ed5:	7d 10 29    	tst	0x1029
    9ed8:	2a fb       	bpl	0x0x9ed5
    9eda:	b6 10 2a    	ldaa	0x102a
    9edd:	86 ff       	ldaa	#0xff
    9edf:	b7 10 42    	staa	0x1042
    9ee2:	7c 00 d0    	inc	0xd0
    9ee5:	bd a4 d7    	jsr	0xa4d7
    9ee8:	c6 20       	ldab	#0x20
    9eea:	4f          	clra
    9eeb:	ce 1f 00    	ldx	#0x1f00
    9eee:	a7 00       	staa	0x0,x
    9ef0:	08          	inx
    9ef1:	5a          	decb
    9ef2:	26 fa       	bne	0x0x9eee
    9ef4:	13 f3 10 0b 	brclr	*0xf3, #0x10, 0x0x9f03
    9ef8:	b6 10 22    	ldaa	0x1022
    9efb:	8a 08       	oraa	#0x8
    9efd:	b7 10 22    	staa	0x1022
    9f00:	7e a4 f9    	jmp	0xa4f9
    9f03:	96 71       	ldaa	*0x71
    9f05:	97 db       	staa	*0xdb
    9f07:	96 72       	ldaa	*0x72
    9f09:	b7 01 15    	staa	0x115
    9f0c:	7e a4 f9    	jmp	0xa4f9
    9f0f:	c6 b0       	ldab	#0xb0
    9f11:	3d          	mul
    9f12:	c3 20 00    	addd	#0x2000
    9f15:	8f          	xgdx
    9f16:	18 ce 00 20 	ldy	#0x20
    9f1a:	c6 b0       	ldab	#0xb0
    9f1c:	a6 00       	ldaa	0x0,x
    9f1e:	84 7f       	anda	#0x7f
    9f20:	18 a7 00    	staa	0x0,y
    9f23:	08          	inx
    9f24:	18 08       	iny
    9f26:	5a          	decb
    9f27:	26 f3       	bne	0x0x9f1c
    9f29:	96 41       	ldaa	*0x41
    9f2b:	84 10       	anda	#0x10
    9f2d:	48          	asla
    9f2e:	48          	asla
    9f2f:	48          	asla
    9f30:	9a 44       	oraa	*0x44
    9f32:	97 f7       	staa	*0xf7
    9f34:	39          	rts
    9f35:	b6 01 6a    	ldaa	0x16a
    9f38:	c6 40       	ldab	#0x40
    9f3a:	3d          	mul
    9f3b:	c3 20 00    	addd	#0x2000
    9f3e:	8f          	xgdx
    9f3f:	18 ce 50 00 	ldy	#0x5000
    9f43:	c6 40       	ldab	#0x40
    9f45:	a6 00       	ldaa	0x0,x
    9f47:	84 7f       	anda	#0x7f
    9f49:	18 a7 00    	staa	0x0,y
    9f4c:	08          	inx
    9f4d:	18 08       	iny
    9f4f:	5a          	decb
    9f50:	26 f3       	bne	0x0x9f45
    9f52:	86 01       	ldaa	#0x1
    9f54:	b7 01 6b    	staa	0x16b
    9f57:	b6 50 04    	ldaa	0x5004
    9f5a:	97 fa       	staa	*0xfa
    9f5c:	b6 50 09    	ldaa	0x5009
    9f5f:	97 f9       	staa	*0xf9
    9f61:	bd a4 f9    	jsr	0xa4f9
    9f64:	86 02       	ldaa	#0x2
    9f66:	97 f9       	staa	*0xf9
    9f68:	bd 9d 9c    	jsr	0x9d9c
    9f6b:	7f 01 6b    	clr	0x16b
    9f6e:	b6 50 01    	ldaa	0x5001
    9f71:	97 fa       	staa	*0xfa
    9f73:	b6 50 08    	ldaa	0x5008
    9f76:	97 f9       	staa	*0xf9
    9f78:	bd a4 f9    	jsr	0xa4f9
    9f7b:	86 02       	ldaa	#0x2
    9f7d:	97 f9       	staa	*0xf9
    9f7f:	bd 9d 9c    	jsr	0x9d9c
    9f82:	7f 01 6b    	clr	0x16b
    9f85:	39          	rts
    9f86:	b6 01 6a    	ldaa	0x16a
    9f89:	c6 40       	ldab	#0x40
    9f8b:	3d          	mul
    9f8c:	c3 20 00    	addd	#0x2000
    9f8f:	8f          	xgdx
    9f90:	18 ce 50 00 	ldy	#0x5000
    9f94:	c6 40       	ldab	#0x40
    9f96:	20 0f       	bra	0x0x9fa7
    9f98:	96 fa       	ldaa	*0xfa
    9f9a:	c6 b0       	ldab	#0xb0
    9f9c:	3d          	mul
    9f9d:	c3 20 00    	addd	#0x2000
    9fa0:	8f          	xgdx
    9fa1:	18 ce 00 20 	ldy	#0x20
    9fa5:	c6 b0       	ldab	#0xb0
    9fa7:	18 a6 00    	ldaa	0x0,y
    9faa:	84 7f       	anda	#0x7f
    9fac:	a7 00       	staa	0x0,x
    9fae:	08          	inx
    9faf:	18 08       	iny
    9fb1:	5a          	decb
    9fb2:	26 f3       	bne	0x0x9fa7
    9fb4:	39          	rts
    9fb5:	ce 00 20    	ldx	#0x20
    9fb8:	18 ce 58 00 	ldy	#0x5800
    9fbc:	c6 b0       	ldab	#0xb0
    9fbe:	a6 00       	ldaa	0x0,x
    9fc0:	18 a7 00    	staa	0x0,y
    9fc3:	08          	inx
    9fc4:	18 08       	iny
    9fc6:	5a          	decb
    9fc7:	26 f5       	bne	0x0x9fbe
    9fc9:	bd 9d 9c    	jsr	0x9d9c
    9fcc:	7e 9d 0b    	jmp	0x9d0b
    9fcf:	ce 00 20    	ldx	#0x20
    9fd2:	18 ce 58 00 	ldy	#0x5800
    9fd6:	c6 b0       	ldab	#0xb0
    9fd8:	18 a6 00    	ldaa	0x0,y
    9fdb:	a7 00       	staa	0x0,x
    9fdd:	08          	inx
    9fde:	18 08       	iny
    9fe0:	5a          	decb
    9fe1:	26 f5       	bne	0x0x9fd8
    9fe3:	bd 9d b8    	jsr	0x9db8
    9fe6:	7e 9d 0b    	jmp	0x9d0b
    9fe9:	0f          	sei
    9fea:	36          	psha
    9feb:	37          	pshb
    9fec:	f6 01 6f    	ldab	0x16f
    9fef:	da dc       	orab	*0xdc
    9ff1:	fe 01 62    	ldx	0x162
    9ff4:	e7 00       	stab	0x0,x
    9ff6:	8f          	xgdx
    9ff7:	5c          	incb
    9ff8:	c1 60       	cmpb	#0x60
    9ffa:	25 02       	bcs	0x0x9ffe
    9ffc:	c6 40       	ldab	#0x40
    9ffe:	8f          	xgdx
    9fff:	33          	pulb
    a000:	e7 00       	stab	0x0,x
    a002:	8f          	xgdx
    a003:	5c          	incb
    a004:	c1 60       	cmpb	#0x60
    a006:	25 02       	bcs	0x0xa00a
    a008:	c6 40       	ldab	#0x40
    a00a:	8f          	xgdx
    a00b:	32          	pula
    a00c:	4d          	tsta
    a00d:	2b 0b       	bmi	0x0xa01a
    a00f:	a7 00       	staa	0x0,x
    a011:	8f          	xgdx
    a012:	5c          	incb
    a013:	c1 60       	cmpb	#0x60
    a015:	25 02       	bcs	0x0xa019
    a017:	c6 40       	ldab	#0x40
    a019:	8f          	xgdx
    a01a:	ff 01 62    	stx	0x162
    a01d:	0e          	cli
    a01e:	7d 00 d7    	tst	0xd7
    a021:	27 06       	beq	0x0xa029
    a023:	7f 00 d7    	clr	0xd7
    a026:	7e fe ee    	jmp	0xfeee
    a029:	39          	rts
    a02a:	bd 8e 57    	jsr	0x8e57
    a02d:	86 f0       	ldaa	#0xf0
    a02f:	b7 10 2f    	staa	0x102f
    a032:	86 00       	ldaa	#0x0
    a034:	bd 8e 57    	jsr	0x8e57
    a037:	b7 10 2f    	staa	0x102f
    a03a:	bd 8e 57    	jsr	0x8e57
    a03d:	b7 10 2f    	staa	0x102f
    a040:	86 4d       	ldaa	#0x4d
    a042:	bd 8e 57    	jsr	0x8e57
    a045:	b7 10 2f    	staa	0x102f
    a048:	86 08       	ldaa	#0x8
    a04a:	bd 8e 57    	jsr	0x8e57
    a04d:	b7 10 2f    	staa	0x102f
    a050:	bd 8e 57    	jsr	0x8e57
    a053:	86 55       	ldaa	#0x55
    a055:	b7 10 2f    	staa	0x102f
    a058:	bd 8e 57    	jsr	0x8e57
    a05b:	86 2a       	ldaa	#0x2a
    a05d:	b7 10 2f    	staa	0x102f
    a060:	bd 8e 57    	jsr	0x8e57
    a063:	4f          	clra
    a064:	b7 10 2f    	staa	0x102f
    a067:	bd 8e 57    	jsr	0x8e57
    a06a:	b7 10 2f    	staa	0x102f
    a06d:	ce 80 00    	ldx	#0x8000
    a070:	18 ce 01 90 	ldy	#0x190
    a074:	c6 01       	ldab	#0x1
    a076:	d7 f8       	stab	*0xf8
    a078:	a6 00       	ldaa	0x0,x
    a07a:	36          	psha
    a07b:	84 0f       	anda	#0xf
    a07d:	bd 8e 57    	jsr	0x8e57
    a080:	bd 8e 5f    	jsr	0x8e5f
    a083:	b7 10 2f    	staa	0x102f
    a086:	32          	pula
    a087:	84 f0       	anda	#0xf0
    a089:	44          	lsra
    a08a:	bd 8e 57    	jsr	0x8e57
    a08d:	bd 8e 5f    	jsr	0x8e5f
    a090:	b7 10 2f    	staa	0x102f
    a093:	18 09       	dey
    a095:	26 09       	bne	0x0xa0a0
    a097:	18 ce 01 90 	ldy	#0x190
    a09b:	58          	aslb
    a09c:	24 02       	bcc	0x0xa0a0
    a09e:	c6 01       	ldab	#0x1
    a0a0:	08          	inx
    a0a1:	26 d3       	bne	0x0xa076
    a0a3:	86 f7       	ldaa	#0xf7
    a0a5:	bd 8e 57    	jsr	0x8e57
    a0a8:	bd 8e 5f    	jsr	0x8e5f
    a0ab:	b7 10 2f    	staa	0x102f
    a0ae:	86 15       	ldaa	#0x15
    a0b0:	97 f8       	staa	*0xf8
    a0b2:	7f 00 ff    	clr	0xff
    a0b5:	7f 01 1b    	clr	0x11b
    a0b8:	bd 9d 8a    	jsr	0x9d8a
    a0bb:	96 f2       	ldaa	*0xf2
    a0bd:	84 20       	anda	#0x20
    a0bf:	97 f2       	staa	*0xf2
    a0c1:	86 80       	ldaa	#0x80
    a0c3:	b7 01 1c    	staa	0x11c
    a0c6:	7e 9d 0b    	jmp	0x9d0b
    a0c9:	bd a3 03    	jsr	0xa303
    a0cc:	bd a3 03    	jsr	0xa303
    a0cf:	b6 10 08    	ldaa	0x1008
    a0d2:	84 df       	anda	#0xdf
    a0d4:	b7 10 08    	staa	0x1008
    a0d7:	b6 10 00    	ldaa	0x1000
    a0da:	8a 10       	oraa	#0x10
    a0dc:	b7 10 00    	staa	0x1000
    a0df:	ce 40 00    	ldx	#0x4000
    a0e2:	bd a3 03    	jsr	0xa303
    a0e5:	16          	tab
    a0e6:	bd a3 03    	jsr	0xa303
    a0e9:	48          	asla
    a0ea:	1b          	aba
    a0eb:	a7 00       	staa	0x0,x
    a0ed:	08          	inx
    a0ee:	8c 80 00    	cpx	#0x8000
    a0f1:	25 ef       	bcs	0x0xa0e2
    a0f3:	b6 10 08    	ldaa	0x1008
    a0f6:	85 20       	bita	#0x20
    a0f8:	26 0a       	bne	0x0xa104
    a0fa:	8a 20       	oraa	#0x20
    a0fc:	b7 10 08    	staa	0x1008
    a0ff:	ce 40 00    	ldx	#0x4000
    a102:	20 de       	bra	0x0xa0e2
    a104:	86 0c       	ldaa	#0xc
    a106:	b7 10 2d    	staa	0x102d
    a109:	4f          	clra
    a10a:	97 f2       	staa	*0xf2
    a10c:	97 f3       	staa	*0xf3
    a10e:	97 f4       	staa	*0xf4
    a110:	97 f5       	staa	*0xf5
    a112:	97 f6       	staa	*0xf6
    a114:	97 f7       	staa	*0xf7
    a116:	97 f8       	staa	*0xf8
    a118:	bd a1 e0    	jsr	0xa1e0
    a11b:	86 40       	ldaa	#0x40
    a11d:	97 f2       	staa	*0xf2
    a11f:	86 20       	ldaa	#0x20
    a121:	b7 10 44    	staa	0x1044
    a124:	01          	nop
    a125:	01          	nop
    a126:	b6 10 45    	ldaa	0x1045
    a129:	b1 01 75    	cmpa	0x175
    a12c:	27 f8       	beq	0x0xa126
    a12e:	16          	tab
    a12f:	b8 01 75    	eora	0x175
    a132:	f7 01 75    	stab	0x175
    a135:	b4 01 75    	anda	0x175
    a138:	84 c0       	anda	#0xc0
    a13a:	27 ea       	beq	0x0xa126
    a13c:	2a 03       	bpl	0x0xa141
    a13e:	7e a1 c1    	jmp	0xa1c1
    a141:	7f 00 f2    	clr	0xf2
    a144:	bd a2 57    	jsr	0xa257
    a147:	b6 10 08    	ldaa	0x1008
    a14a:	84 df       	anda	#0xdf
    a14c:	b7 10 08    	staa	0x1008
    a14f:	4f          	clra
    a150:	b7 10 22    	staa	0x1022
    a153:	18 ce 00 00 	ldy	#0x0
    a157:	ce a1 6b    	ldx	#0xa16b
    a15a:	a6 00       	ldaa	0x0,x
    a15c:	18 a7 00    	staa	0x0,y
    a15f:	18 08       	iny
    a161:	08          	inx
    a162:	8c a1 c0    	cpx	#0xa1c0
    a165:	25 f3       	bcs	0x0xa15a
    a167:	0f          	sei
    a168:	7e 00 00    	jmp	0x0
    a16b:	ce 40 00    	ldx	#0x4000
    a16e:	18 ce 80 00 	ldy	#0x8000
    a172:	e6 00       	ldab	0x0,x
    a174:	86 aa       	ldaa	#0xaa
    a176:	b7 d5 55    	staa	0xd555
    a179:	86 55       	ldaa	#0x55
    a17b:	b7 aa aa    	staa	0xaaaa
    a17e:	86 a0       	ldaa	#0xa0
    a180:	b7 d5 55    	staa	0xd555
    a183:	86 80       	ldaa	#0x80
    a185:	18 e7 00    	stab	0x0,y
    a188:	08          	inx
    a189:	18 08       	iny
    a18b:	e6 00       	ldab	0x0,x
    a18d:	4a          	deca
    a18e:	26 f5       	bne	0x0xa185
    a190:	86 08       	ldaa	#0x8
    a192:	b7 10 23    	staa	0x1023
    a195:	fc 10 0e    	ldd	0x100e
    a198:	c3 5d c0    	addd	#0x5dc0
    a19b:	fd 10 1e    	std	0x101e
    a19e:	01          	nop
    a19f:	01          	nop
    a1a0:	b6 10 23    	ldaa	0x1023
    a1a3:	85 08       	bita	#0x8
    a1a5:	27 f7       	beq	0x0xa19e
    a1a7:	8c 80 00    	cpx	#0x8000
    a1aa:	25 c6       	bcs	0x0xa172
    a1ac:	b6 10 08    	ldaa	0x1008
    a1af:	85 20       	bita	#0x20
    a1b1:	27 03       	beq	0x0xa1b6
    a1b3:	7e 80 00    	jmp	0x8000
    a1b6:	8a 20       	oraa	#0x20
    a1b8:	b7 10 08    	staa	0x1008
    a1bb:	ce 40 00    	ldx	#0x4000
    a1be:	20 b2       	bra	0x0xa172
    a1c0:	01          	nop
    a1c1:	7f 00 f2    	clr	0xf2
    a1c4:	bd a2 ad    	jsr	0xa2ad
    a1c7:	b6 10 45    	ldaa	0x1045
    a1ca:	b1 01 75    	cmpa	0x175
    a1cd:	27 f8       	beq	0x0xa1c7
    a1cf:	16          	tab
    a1d0:	b8 01 75    	eora	0x175
    a1d3:	f7 01 75    	stab	0x175
    a1d6:	b4 01 75    	anda	0x175
    a1d9:	84 80       	anda	#0x80
    a1db:	27 ea       	beq	0x0xa1c7
    a1dd:	7e 80 00    	jmp	0x8000
    a1e0:	86 f7       	ldaa	#0xf7
    a1e2:	b4 10 00    	anda	0x1000
    a1e5:	b7 10 00    	staa	0x1000
    a1e8:	86 0c       	ldaa	#0xc
    a1ea:	b7 10 47    	staa	0x1047
    a1ed:	86 80       	ldaa	#0x80
    a1ef:	ba 10 00    	oraa	0x1000
    a1f2:	b7 10 00    	staa	0x1000
    a1f5:	01          	nop
    a1f6:	88 80       	eora	#0x80
    a1f8:	b7 10 00    	staa	0x1000
    a1fb:	7f 01 1d    	clr	0x11d
    a1fe:	bd dd 7a    	jsr	0xdd7a
    a201:	ce 10 23    	ldx	#0x1023
    a204:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa204
    a208:	bd dd ae    	jsr	0xddae
    a20b:	ce 01 20    	ldx	#0x120
    a20e:	18 ce a2 37 	ldy	#0xa237
    a212:	c6 20       	ldab	#0x20
    a214:	18 a6 00    	ldaa	0x0,y
    a217:	a7 00       	staa	0x0,x
    a219:	08          	inx
    a21a:	18 08       	iny
    a21c:	5a          	decb
    a21d:	26 f5       	bne	0x0xa214
    a21f:	7f 01 1e    	clr	0x11e
    a222:	86 20       	ldaa	#0x20
    a224:	b7 01 1c    	staa	0x11c
    a227:	ce 10 23    	ldx	#0x1023
    a22a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa22a
    a22e:	bd dd 02    	jsr	0xdd02
    a231:	7d 01 1c    	tst	0x11c
    a234:	26 f1       	bne	0x0xa227
    a236:	39          	rts
    a237:	2a 53       	bpl	0x0xa28c
    a239:	41          	.byte	0x41
    a23a:	56          	rorb
    a23b:	45          	.byte	0x45
    a23c:	2a 20       	bpl	0x0xa25e
    a23e:	54          	lsrb
    a23f:	4f          	clra
    a240:	20 55       	bra	0x0xa297
    a242:	50          	negb
    a243:	44          	lsra
    a244:	41          	.byte	0x41
    a245:	54          	lsrb
    a246:	45          	.byte	0x45
    a247:	4f          	clra
    a248:	53          	comb
    a249:	2c 2a       	bge	0x0xa275
    a24b:	45          	.byte	0x45
    a24c:	58          	aslb
    a24d:	49          	rola
    a24e:	54          	lsrb
    a24f:	2a 20       	bpl	0x0xa271
    a251:	41          	.byte	0x41
    a252:	42          	.byte	0x42
    a253:	4f          	clra
    a254:	52          	.byte	0x52
    a255:	54          	lsrb
    a256:	53          	comb
    a257:	ce 10 23    	ldx	#0x1023
    a25a:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa25a
    a25e:	bd dd ae    	jsr	0xddae
    a261:	ce 01 20    	ldx	#0x120
    a264:	18 ce a2 8d 	ldy	#0xa28d
    a268:	c6 20       	ldab	#0x20
    a26a:	18 a6 00    	ldaa	0x0,y
    a26d:	a7 00       	staa	0x0,x
    a26f:	08          	inx
    a270:	18 08       	iny
    a272:	5a          	decb
    a273:	26 f5       	bne	0x0xa26a
    a275:	7f 01 1e    	clr	0x11e
    a278:	86 20       	ldaa	#0x20
    a27a:	b7 01 1c    	staa	0x11c
    a27d:	ce 10 23    	ldx	#0x1023
    a280:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa280
    a284:	bd dd 02    	jsr	0xdd02
    a287:	7d 01 1c    	tst	0x11c
    a28a:	26 f1       	bne	0x0xa27d
    a28c:	39          	rts
    a28d:	20 55       	bra	0x0xa2e4
    a28f:	50          	negb
    a290:	44          	lsra
    a291:	41          	.byte	0x41
    a292:	54          	lsrb
    a293:	49          	rola
    a294:	4e          	.byte	0x4e
    a295:	47          	asra
    a296:	20 4f       	bra	0x0xa2e7
    a298:	53          	comb
    a299:	20 2e       	bra	0x0xa2c9
    a29b:	2e 2e       	bgt	0x0xa2cb
    a29d:	20 54       	bra	0x0xa2f3
    a29f:	41          	.byte	0x41
    a2a0:	4b          	.byte	0x4b
    a2a1:	45          	.byte	0x45
    a2a2:	20 35       	bra	0x0xa2d9
    a2a4:	20 2e       	bra	0x0xa2d4
    a2a6:	2e 2e       	bgt	0x0xa2d6
    a2a8:	2e 2e       	bgt	0x0xa2d8
    a2aa:	2e 2e       	bgt	0x0xa2da
    a2ac:	2e ce       	bgt	0x0xa27c
    a2ae:	10          	sba
    a2af:	23 1f       	bls	0x0xa2d0
    a2b1:	00          	bgnd
    a2b2:	10          	sba
    a2b3:	fc bd dd    	ldd	0xbddd
    a2b6:	ae ce       	lds	0xce,x
    a2b8:	01          	nop
    a2b9:	20 18       	bra	0x0xa2d3
    a2bb:	ce a2 e3    	ldx	#0xa2e3
    a2be:	c6 20       	ldab	#0x20
    a2c0:	18 a6 00    	ldaa	0x0,y
    a2c3:	a7 00       	staa	0x0,x
    a2c5:	08          	inx
    a2c6:	18 08       	iny
    a2c8:	5a          	decb
    a2c9:	26 f5       	bne	0x0xa2c0
    a2cb:	7f 01 1e    	clr	0x11e
    a2ce:	86 20       	ldaa	#0x20
    a2d0:	b7 01 1c    	staa	0x11c
    a2d3:	ce 10 23    	ldx	#0x1023
    a2d6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xa2d6
    a2da:	bd dd 02    	jsr	0xdd02
    a2dd:	7d 01 1c    	tst	0x11c
    a2e0:	26 f1       	bne	0x0xa2d3
    a2e2:	39          	rts
    a2e3:	4f          	clra
    a2e4:	53          	comb
    a2e5:	20 55       	bra	0x0xa33c
    a2e7:	50          	negb
    a2e8:	44          	lsra
    a2e9:	41          	.byte	0x41
    a2ea:	54          	lsrb
    a2eb:	45          	.byte	0x45
    a2ec:	20 41       	bra	0x0xa32f
    a2ee:	42          	.byte	0x42
    a2ef:	4f          	clra
    a2f0:	52          	.byte	0x52
    a2f1:	54          	lsrb
    a2f2:	44          	lsra
    a2f3:	52          	.byte	0x52
    a2f4:	45          	.byte	0x45
    a2f5:	4c          	inca
    a2f6:	4f          	clra
    a2f7:	41          	.byte	0x41
    a2f8:	44          	lsra
    a2f9:	20 53       	bra	0x0xa34e
    a2fb:	45          	.byte	0x45
    a2fc:	51          	.byte	0x51
    a2fd:	26 4d       	bne	0x0xa34c
    a2ff:	55          	.byte	0x55
    a300:	4c          	inca
    a301:	54          	lsrb
    a302:	49          	rola
    a303:	3c          	pshx
    a304:	fe 01 c0    	ldx	0x1c0
    a307:	bc 01 c2    	cpx	0x1c2
    a30a:	27 f8       	beq	0x0xa304
    a30c:	a6 00       	ldaa	0x0,x
    a30e:	8f          	xgdx
    a30f:	5c          	incb
    a310:	c4 bf       	andb	#0xbf
    a312:	8f          	xgdx
    a313:	ff 01 c0    	stx	0x1c0
    a316:	38          	pulx
    a317:	39          	rts
    a318:	0f          	sei
    a319:	4f          	clra
    a31a:	b7 10 40    	staa	0x1040
    a31d:	b6 01 72    	ldaa	0x172
    a320:	49          	rola
    a321:	2a 02       	bpl	0x0xa325
    a323:	86 01       	ldaa	#0x1
    a325:	b7 10 41    	staa	0x1041
    a328:	b7 01 72    	staa	0x172
    a32b:	fc 01 73    	ldd	0x173
    a32e:	5c          	incb
    a32f:	c1 f8       	cmpb	#0xf8
    a331:	23 02       	bls	0x0xa335
    a333:	c6 f2       	ldab	#0xf2
    a335:	fd 01 73    	std	0x173
    a338:	8f          	xgdx
    a339:	a6 00       	ldaa	0x0,x
    a33b:	7d 00 fb    	tst	0xfb
    a33e:	27 24       	beq	0x0xa364
    a340:	8c 00 f2    	cpx	#0xf2
    a343:	26 1f       	bne	0x0xa364
    a345:	f6 01 66    	ldab	0x166
    a348:	5a          	decb
    a349:	c4 1f       	andb	#0x1f
    a34b:	f7 01 66    	stab	0x166
    a34e:	26 08       	bne	0x0xa358
    a350:	c6 01       	ldab	#0x1
    a352:	f8 01 67    	eorb	0x167
    a355:	f7 01 67    	stab	0x167
    a358:	7d 00 fe    	tst	0xfe
    a35b:	26 07       	bne	0x0xa364
    a35d:	7d 01 67    	tst	0x167
    a360:	26 02       	bne	0x0xa364
    a362:	84 df       	anda	#0xdf
    a364:	b7 10 40    	staa	0x1040
    a367:	86 80       	ldaa	#0x80
    a369:	b7 10 23    	staa	0x1023
    a36c:	fc 10 0e    	ldd	0x100e
    a36f:	c3 13 88    	addd	#0x1388
    a372:	fd 10 16    	std	0x1016
    a375:	0e          	cli
    a376:	3b          	rti
    a377:	ce 10 23    	ldx	#0x1023
    a37a:	1e 00 20 03 	brset	0x0,x, #0x20, 0x0xa381
    a37e:	7e a4 c3    	jmp	0xa4c3
    a381:	96 ff       	ldaa	*0xff
    a383:	81 01       	cmpa	#0x1
    a385:	26 03       	bne	0x0xa38a
    a387:	7e a4 c3    	jmp	0xa4c3
    a38a:	7d 00 fe    	tst	0xfe
    a38d:	27 03       	beq	0x0xa392
    a38f:	7e a4 c3    	jmp	0xa4c3
    a392:	7d 01 17    	tst	0x117
    a395:	27 0d       	beq	0x0xa3a4
    a397:	4f          	clra
    a398:	b7 10 30    	staa	0x1030
    a39b:	b7 01 17    	staa	0x117
    a39e:	ce 00 96    	ldx	#0x96
    a3a1:	7e a4 b7    	jmp	0xa4b7
    a3a4:	ce 00 00    	ldx	#0x0
    a3a7:	f6 10 31    	ldab	0x1031
    a3aa:	3a          	abx
    a3ab:	f6 10 32    	ldab	0x1032
    a3ae:	3a          	abx
    a3af:	f6 10 33    	ldab	0x1033
    a3b2:	3a          	abx
    a3b3:	f6 10 34    	ldab	0x1034
    a3b6:	3a          	abx
    a3b7:	8f          	xgdx
    a3b8:	04          	lsrd
    a3b9:	04          	lsrd
    a3ba:	54          	lsrb
    a3bb:	ce 00 00    	ldx	#0x0
    a3be:	37          	pshb
    a3bf:	f6 01 16    	ldab	0x116
    a3c2:	3a          	abx
    a3c3:	33          	pulb
    a3c4:	a6 00       	ldaa	0x0,x
    a3c6:	10          	sba
    a3c7:	26 27       	bne	0x0xa3f0
    a3c9:	86 01       	ldaa	#0x1
    a3cb:	b7 01 17    	staa	0x117
    a3ce:	b6 01 16    	ldaa	0x116
    a3d1:	4c          	inca
    a3d2:	81 10       	cmpa	#0x10
    a3d4:	25 01       	bcs	0x0xa3d7
    a3d6:	4f          	clra
    a3d7:	b7 01 16    	staa	0x116
    a3da:	81 07       	cmpa	#0x7
    a3dc:	22 07       	bhi	0x0xa3e5
    a3de:	8a 70       	oraa	#0x70
    a3e0:	b7 10 43    	staa	0x1043
    a3e3:	20 05       	bra	0x0xa3ea
    a3e5:	8a 68       	oraa	#0x68
    a3e7:	b7 10 43    	staa	0x1043
    a3ea:	ce 07 d0    	ldx	#0x7d0
    a3ed:	7e a4 b7    	jmp	0xa4b7
    a3f0:	2a 04       	bpl	0x0xa3f6
    a3f2:	40          	nega
    a3f3:	7e a4 40    	jmp	0xa440
    a3f6:	81 02       	cmpa	#0x2
    a3f8:	23 18       	bls	0x0xa412
    a3fa:	bd a4 d7    	jsr	0xa4d7
    a3fd:	e7 00       	stab	0x0,x
    a3ff:	37          	pshb
    a400:	ce 1f 00    	ldx	#0x1f00
    a403:	f6 01 16    	ldab	0x116
    a406:	3a          	abx
    a407:	86 80       	ldaa	#0x80
    a409:	a7 00       	staa	0x0,x
    a40b:	bd a4 f9    	jsr	0xa4f9
    a40e:	32          	pula
    a40f:	7e a4 8a    	jmp	0xa48a
    a412:	bd a4 d7    	jsr	0xa4d7
    a415:	37          	pshb
    a416:	18 ce 1f 00 	ldy	#0x1f00
    a41a:	f6 01 16    	ldab	0x116
    a41d:	18 3a       	aby
    a41f:	18 6d 00    	tst	0x0,y
    a422:	2b 07       	bmi	0x0xa42b
    a424:	bd a4 f9    	jsr	0xa4f9
    a427:	33          	pulb
    a428:	7e a3 c9    	jmp	0xa3c9
    a42b:	18 1f 00 01 	brclr	0x0,y, #0x01, 0x0xa437
    a42f:	07 
    a430:	18 6f 00    	clr	0x0,y
    a433:	33          	pulb
    a434:	7e a3 c9    	jmp	0xa3c9
    a437:	32          	pula
    a438:	a7 00       	staa	0x0,x
    a43a:	bd a4 f9    	jsr	0xa4f9
    a43d:	7e a4 8a    	jmp	0xa48a
    a440:	81 02       	cmpa	#0x2
    a442:	23 18       	bls	0x0xa45c
    a444:	bd a4 d7    	jsr	0xa4d7
    a447:	e7 00       	stab	0x0,x
    a449:	37          	pshb
    a44a:	ce 1f 00    	ldx	#0x1f00
    a44d:	f6 01 16    	ldab	0x116
    a450:	3a          	abx
    a451:	86 81       	ldaa	#0x81
    a453:	a7 00       	staa	0x0,x
    a455:	bd a4 f9    	jsr	0xa4f9
    a458:	32          	pula
    a459:	7e a4 8a    	jmp	0xa48a
    a45c:	bd a4 d7    	jsr	0xa4d7
    a45f:	37          	pshb
    a460:	18 ce 1f 00 	ldy	#0x1f00
    a464:	f6 01 16    	ldab	0x116
    a467:	18 3a       	aby
    a469:	18 6d 00    	tst	0x0,y
    a46c:	2b 07       	bmi	0x0xa475
    a46e:	bd a4 f9    	jsr	0xa4f9
    a471:	33          	pulb
    a472:	7e a3 c9    	jmp	0xa3c9
    a475:	18 1e 00 01 	brset	0x0,y, #0x01, 0x0xa484
    a479:	0a 
    a47a:	18 6f 00    	clr	0x0,y
    a47d:	33          	pulb
    a47e:	bd a4 f9    	jsr	0xa4f9
    a481:	7e a3 c9    	jmp	0xa3c9
    a484:	32          	pula
    a485:	a7 00       	staa	0x0,x
    a487:	bd a4 f9    	jsr	0xa4f9
    a48a:	f7 01 1f    	stab	0x11f
    a48d:	c1 07       	cmpb	#0x7
    a48f:	27 1a       	beq	0x0xa4ab
    a491:	f6 01 70    	ldab	0x170
    a494:	c1 02       	cmpb	#0x2
    a496:	26 03       	bne	0x0xa49b
    a498:	7e a3 ea    	jmp	0xa3ea
    a49b:	7d 00 fb    	tst	0xfb
    a49e:	26 0b       	bne	0x0xa4ab
    a4a0:	14 fb 01    	bset	*0xfb, #0x01
    a4a3:	14 f2 20    	bset	*0xf2, #0x20
    a4a6:	c6 1f       	ldab	#0x1f
    a4a8:	f7 01 66    	stab	0x166
    a4ab:	f6 01 1f    	ldab	0x11f
    a4ae:	58          	aslb
    a4af:	ce a5 4e    	ldx	#0xa54e
    a4b2:	3a          	abx
    a4b3:	ee 00       	ldx	0x0,x
    a4b5:	6e 00       	jmp	0x0,x
    a4b7:	86 20       	ldaa	#0x20
    a4b9:	b7 10 23    	staa	0x1023
    a4bc:	8f          	xgdx
    a4bd:	f3 10 0e    	addd	0x100e
    a4c0:	fd 10 1a    	std	0x101a
    a4c3:	fe 01 c0    	ldx	0x1c0
    a4c6:	bc 01 c2    	cpx	0x1c2
    a4c9:	26 03       	bne	0x0xa4ce
    a4cb:	7e a9 28    	jmp	0xa928
    a4ce:	18 fe 01 6c 	ldy	0x16c
    a4d2:	18 3c       	pshy
    a4d4:	7e 8e d5    	jmp	0x8ed5
    a4d7:	86 ef       	ldaa	#0xef
    a4d9:	b4 10 00    	anda	0x1000
    a4dc:	b7 10 00    	staa	0x1000
    a4df:	86 1f       	ldaa	#0x1f
    a4e1:	b4 10 08    	anda	0x1008
    a4e4:	b7 10 08    	staa	0x1008
    a4e7:	39          	rts
    a4e8:	86 10       	ldaa	#0x10
    a4ea:	ba 10 00    	oraa	0x1000
    a4ed:	b7 10 00    	staa	0x1000
    a4f0:	86 1f       	ldaa	#0x1f
    a4f2:	b4 10 08    	anda	0x1008
    a4f5:	b7 10 08    	staa	0x1008
    a4f8:	39          	rts
    a4f9:	36          	psha
    a4fa:	96 f9       	ldaa	*0xf9
    a4fc:	26 12       	bne	0x0xa510
    a4fe:	b6 10 00    	ldaa	0x1000
    a501:	84 ef       	anda	#0xef
    a503:	b7 10 00    	staa	0x1000
    a506:	b6 10 08    	ldaa	0x1008
    a509:	84 1f       	anda	#0x1f
    a50b:	b7 10 08    	staa	0x1008
    a50e:	32          	pula
    a50f:	39          	rts
    a510:	81 01       	cmpa	#0x1
    a512:	22 12       	bhi	0x0xa526
    a514:	b6 10 00    	ldaa	0x1000
    a517:	84 ef       	anda	#0xef
    a519:	b7 10 00    	staa	0x1000
    a51c:	b6 10 08    	ldaa	0x1008
    a51f:	8a 20       	oraa	#0x20
    a521:	b7 10 08    	staa	0x1008
    a524:	32          	pula
    a525:	39          	rts
    a526:	81 02       	cmpa	#0x2
    a528:	22 12       	bhi	0x0xa53c
    a52a:	b6 10 00    	ldaa	0x1000
    a52d:	8a 10       	oraa	#0x10
    a52f:	b7 10 00    	staa	0x1000
    a532:	b6 10 08    	ldaa	0x1008
    a535:	84 1f       	anda	#0x1f
    a537:	b7 10 08    	staa	0x1008
    a53a:	32          	pula
    a53b:	39          	rts
    a53c:	b6 10 00    	ldaa	0x1000
    a53f:	8a 10       	oraa	#0x10
    a541:	b7 10 00    	staa	0x1000
    a544:	b6 10 08    	ldaa	0x1008
    a547:	8a 20       	oraa	#0x20
    a549:	b7 10 08    	staa	0x1008
    a54c:	32          	pula
    a54d:	39          	rts
    a54e:	a5 6e       	bita	0x6e,x
    a550:	a5 d8       	bita	0xd8,x
    a552:	a5 fa       	bita	0xfa,x
    a554:	a6 64       	ldaa	0x64,x
    a556:	a6 86       	ldaa	0x86,x
    a558:	a6 a8       	ldaa	0xa8,x
    a55a:	a8 57       	eora	0x57,x
    a55c:	a8 57       	eora	0x57,x
    a55e:	a6 ca       	ldaa	0xca,x
    a560:	a7 12       	staa	0x12,x
    a562:	a7 61       	staa	0x61,x
    a564:	a7 ad       	staa	0xad,x
    a566:	a8 13       	eora	0x13,x
    a568:	a8 35       	eora	0x35,x
    a56a:	a8 57       	eora	0x57,x
    a56c:	a8 57       	eora	0x57,x
    a56e:	12 f8 08 2e 	brset	*0xf8, #0x08, 0x0xa5a0
    a572:	44          	lsra
    a573:	97 3f       	staa	*0x3f
    a575:	c6 3f       	ldab	#0x3f
    a577:	bd a8 5a    	jsr	0xa85a
    a57a:	36          	psha
    a57b:	b6 01 1b    	ldaa	0x11b
    a57e:	81 06       	cmpa	#0x6
    a580:	26 15       	bne	0x0xa597
    a582:	96 3f       	ldaa	*0x3f
    a584:	ce 01 3c    	ldx	#0x13c
    a587:	bd de a8    	jsr	0xdea8
    a58a:	86 03       	ldaa	#0x3
    a58c:	b7 01 1c    	staa	0x11c
    a58f:	86 1e       	ldaa	#0x1e
    a591:	b7 01 1e    	staa	0x11e
    a594:	bd dd 89    	jsr	0xdd89
    a597:	32          	pula
    a598:	c6 3c       	ldab	#0x3c
    a59a:	bd a8 a2    	jsr	0xa8a2
    a59d:	7e a3 c9    	jmp	0xa3c9
    a5a0:	16          	tab
    a5a1:	ce e2 7a    	ldx	#0xe27a
    a5a4:	3a          	abx
    a5a5:	a6 00       	ldaa	0x0,x
    a5a7:	97 37       	staa	*0x37
    a5a9:	c6 37       	ldab	#0x37
    a5ab:	bd a8 5a    	jsr	0xa85a
    a5ae:	b6 01 1b    	ldaa	0x11b
    a5b1:	81 1b       	cmpa	#0x1b
    a5b3:	26 19       	bne	0x0xa5ce
    a5b5:	13 f4 02 15 	brclr	*0xf4, #0x02, 0x0xa5ce
    a5b9:	96 37       	ldaa	*0x37
    a5bb:	ce 01 31    	ldx	#0x131
    a5be:	bd de a8    	jsr	0xdea8
    a5c1:	86 03       	ldaa	#0x3
    a5c3:	b7 01 1c    	staa	0x11c
    a5c6:	86 13       	ldaa	#0x13
    a5c8:	b7 01 1e    	staa	0x11e
    a5cb:	bd dd 89    	jsr	0xdd89
    a5ce:	96 37       	ldaa	*0x37
    a5d0:	c6 4a       	ldab	#0x4a
    a5d2:	bd a8 a2    	jsr	0xa8a2
    a5d5:	7e a3 c9    	jmp	0xa3c9
    a5d8:	12 f8 08 0f 	brset	*0xf8, #0x08, 0x0xa5eb
    a5dc:	97 42       	staa	*0x42
    a5de:	c6 42       	ldab	#0x42
    a5e0:	bd a8 5a    	jsr	0xa85a
    a5e3:	c6 3b       	ldab	#0x3b
    a5e5:	bd a8 a2    	jsr	0xa8a2
    a5e8:	7e a3 c9    	jmp	0xa3c9
    a5eb:	97 28       	staa	*0x28
    a5ed:	c6 28       	ldab	#0x28
    a5ef:	bd a8 5a    	jsr	0xa85a
    a5f2:	c6 47       	ldab	#0x47
    a5f4:	bd a8 a2    	jsr	0xa8a2
    a5f7:	7e a3 c9    	jmp	0xa3c9
    a5fa:	12 f8 08 2e 	brset	*0xf8, #0x08, 0x0xa62c
    a5fe:	44          	lsra
    a5ff:	97 3e       	staa	*0x3e
    a601:	c6 3e       	ldab	#0x3e
    a603:	bd a8 5a    	jsr	0xa85a
    a606:	36          	psha
    a607:	b6 01 1b    	ldaa	0x11b
    a60a:	81 06       	cmpa	#0x6
    a60c:	26 15       	bne	0x0xa623
    a60e:	96 3e       	ldaa	*0x3e
    a610:	ce 01 37    	ldx	#0x137
    a613:	bd de a8    	jsr	0xdea8
    a616:	86 03       	ldaa	#0x3
    a618:	b7 01 1c    	staa	0x11c
    a61b:	86 19       	ldaa	#0x19
    a61d:	b7 01 1e    	staa	0x11e
    a620:	bd dd 89    	jsr	0xdd89
    a623:	32          	pula
    a624:	c6 36       	ldab	#0x36
    a626:	bd a8 a2    	jsr	0xa8a2
    a629:	7e a3 c9    	jmp	0xa3c9
    a62c:	16          	tab
    a62d:	ce e2 7a    	ldx	#0xe27a
    a630:	3a          	abx
    a631:	a6 00       	ldaa	0x0,x
    a633:	97 2a       	staa	*0x2a
    a635:	c6 2a       	ldab	#0x2a
    a637:	bd a8 5a    	jsr	0xa85a
    a63a:	b6 01 1b    	ldaa	0x11b
    a63d:	81 1b       	cmpa	#0x1b
    a63f:	26 19       	bne	0x0xa65a
    a641:	13 f4 01 15 	brclr	*0xf4, #0x01, 0x0xa65a
    a645:	96 2a       	ldaa	*0x2a
    a647:	ce 01 31    	ldx	#0x131
    a64a:	bd de a8    	jsr	0xdea8
    a64d:	86 03       	ldaa	#0x3
    a64f:	b7 01 1c    	staa	0x11c
    a652:	86 13       	ldaa	#0x13
    a654:	b7 01 1e    	staa	0x11e
    a657:	bd dd 89    	jsr	0xdd89
    a65a:	96 2a       	ldaa	*0x2a
    a65c:	c6 48       	ldab	#0x48
    a65e:	bd a8 a2    	jsr	0xa8a2
    a661:	7e a3 c9    	jmp	0xa3c9
    a664:	12 f8 08 0f 	brset	*0xf8, #0x08, 0x0xa677
    a668:	97 43       	staa	*0x43
    a66a:	c6 43       	ldab	#0x43
    a66c:	bd a8 5a    	jsr	0xa85a
    a66f:	c6 46       	ldab	#0x46
    a671:	bd a8 a2    	jsr	0xa8a2
    a674:	7e a3 c9    	jmp	0xa3c9
    a677:	97 35       	staa	*0x35
    a679:	c6 35       	ldab	#0x35
    a67b:	bd a8 5a    	jsr	0xa85a
    a67e:	c6 49       	ldab	#0x49
    a680:	bd a8 a2    	jsr	0xa8a2
    a683:	7e a3 c9    	jmp	0xa3c9
    a686:	12 f8 02 0f 	brset	*0xf8, #0x02, 0x0xa699
    a68a:	97 4d       	staa	*0x4d
    a68c:	c6 4d       	ldab	#0x4d
    a68e:	bd a8 5a    	jsr	0xa85a
    a691:	c6 59       	ldab	#0x59
    a693:	bd a8 a2    	jsr	0xa8a2
    a696:	7e a3 c9    	jmp	0xa3c9
    a699:	97 46       	staa	*0x46
    a69b:	c6 46       	ldab	#0x46
    a69d:	bd a8 5a    	jsr	0xa85a
    a6a0:	c6 55       	ldab	#0x55
    a6a2:	bd a8 a2    	jsr	0xa8a2
    a6a5:	7e a3 c9    	jmp	0xa3c9
    a6a8:	12 f8 02 0f 	brset	*0xf8, #0x02, 0x0xa6bb
    a6ac:	97 4c       	staa	*0x4c
    a6ae:	c6 4c       	ldab	#0x4c
    a6b0:	bd a8 5a    	jsr	0xa85a
    a6b3:	c6 58       	ldab	#0x58
    a6b5:	bd a8 a2    	jsr	0xa8a2
    a6b8:	7e a3 c9    	jmp	0xa3c9
    a6bb:	97 45       	staa	*0x45
    a6bd:	c6 45       	ldab	#0x45
    a6bf:	bd a8 5a    	jsr	0xa85a
    a6c2:	c6 54       	ldab	#0x54
    a6c4:	bd a8 a2    	jsr	0xa8a2
    a6c7:	7e a3 c9    	jmp	0xa3c9
    a6ca:	12 f8 80 35 	brset	*0xf8, #0x80, 0x0xa703
    a6ce:	12 f8 40 22 	brset	*0xf8, #0x40, 0x0xa6f4
    a6d2:	12 f8 20 0f 	brset	*0xf8, #0x20, 0x0xa6e5
    a6d6:	97 52       	staa	*0x52
    a6d8:	c6 52       	ldab	#0x52
    a6da:	bd a8 5a    	jsr	0xa85a
    a6dd:	c6 68       	ldab	#0x68
    a6df:	bd a8 a2    	jsr	0xa8a2
    a6e2:	7e a3 c9    	jmp	0xa3c9
    a6e5:	97 57       	staa	*0x57
    a6e7:	c6 57       	ldab	#0x57
    a6e9:	bd a8 5a    	jsr	0xa85a
    a6ec:	c6 6c       	ldab	#0x6c
    a6ee:	bd a8 a2    	jsr	0xa8a2
    a6f1:	7e a3 c9    	jmp	0xa3c9
    a6f4:	97 5c       	staa	*0x5c
    a6f6:	c6 5c       	ldab	#0x5c
    a6f8:	bd a8 5a    	jsr	0xa85a
    a6fb:	c6 71       	ldab	#0x71
    a6fd:	bd a8 a2    	jsr	0xa8a2
    a700:	7e a3 c9    	jmp	0xa3c9
    a703:	97 20       	staa	*0x20
    a705:	c6 20       	ldab	#0x20
    a707:	bd a8 5a    	jsr	0xa85a
    a70a:	c6 05       	ldab	#0x5
    a70c:	bd a8 a2    	jsr	0xa8a2
    a70f:	7e a3 c9    	jmp	0xa3c9
    a712:	12 f8 80 35 	brset	*0xf8, #0x80, 0x0xa74b
    a716:	12 f8 40 22 	brset	*0xf8, #0x40, 0x0xa73c
    a71a:	12 f8 20 0f 	brset	*0xf8, #0x20, 0x0xa72d
    a71e:	97 53       	staa	*0x53
    a720:	c6 53       	ldab	#0x53
    a722:	bd a8 5a    	jsr	0xa85a
    a725:	c6 69       	ldab	#0x69
    a727:	bd a8 a2    	jsr	0xa8a2
    a72a:	7e a3 c9    	jmp	0xa3c9
    a72d:	97 58       	staa	*0x58
    a72f:	c6 58       	ldab	#0x58
    a731:	bd a8 5a    	jsr	0xa85a
    a734:	c6 6d       	ldab	#0x6d
    a736:	bd a8 a2    	jsr	0xa8a2
    a739:	7e a3 c9    	jmp	0xa3c9
    a73c:	97 5d       	staa	*0x5d
    a73e:	c6 5d       	ldab	#0x5d
    a740:	bd a8 5a    	jsr	0xa85a
    a743:	c6 72       	ldab	#0x72
    a745:	bd a8 a2    	jsr	0xa8a2
    a748:	7e a3 c9    	jmp	0xa3c9
    a74b:	16          	tab
    a74c:	ce e2 7a    	ldx	#0xe27a
    a74f:	3a          	abx
    a750:	a6 00       	ldaa	0x0,x
    a752:	97 49       	staa	*0x49
    a754:	c6 49       	ldab	#0x49
    a756:	bd a8 5a    	jsr	0xa85a
    a759:	c6 57       	ldab	#0x57
    a75b:	bd a8 a2    	jsr	0xa8a2
    a75e:	7e a3 c9    	jmp	0xa3c9
    a761:	12 f8 80 35 	brset	*0xf8, #0x80, 0x0xa79a
    a765:	12 f8 40 22 	brset	*0xf8, #0x40, 0x0xa78b
    a769:	12 f8 20 0f 	brset	*0xf8, #0x20, 0x0xa77c
    a76d:	97 55       	staa	*0x55
    a76f:	c6 55       	ldab	#0x55
    a771:	bd a8 5a    	jsr	0xa85a
    a774:	c6 6a       	ldab	#0x6a
    a776:	bd a8 a2    	jsr	0xa8a2
    a779:	7e a3 c9    	jmp	0xa3c9
    a77c:	97 5a       	staa	*0x5a
    a77e:	c6 5a       	ldab	#0x5a
    a780:	bd a8 5a    	jsr	0xa85a
    a783:	c6 6e       	ldab	#0x6e
    a785:	bd a8 a2    	jsr	0xa8a2
    a788:	7e a3 c9    	jmp	0xa3c9
    a78b:	97 5f       	staa	*0x5f
    a78d:	c6 5f       	ldab	#0x5f
    a78f:	bd a8 5a    	jsr	0xa85a
    a792:	c6 73       	ldab	#0x73
    a794:	bd a8 a2    	jsr	0xa8a2
    a797:	7e a3 c9    	jmp	0xa3c9
    a79a:	16          	tab
    a79b:	ce e2 7a    	ldx	#0xe27a
    a79e:	3a          	abx
    a79f:	a6 00       	ldaa	0x0,x
    a7a1:	97 71       	staa	*0x71
    a7a3:	97 db       	staa	*0xdb
    a7a5:	c6 74       	ldab	#0x74
    a7a7:	bd a8 a2    	jsr	0xa8a2
    a7aa:	7e a3 c9    	jmp	0xa3c9
    a7ad:	12 f8 80 35 	brset	*0xf8, #0x80, 0x0xa7e6
    a7b1:	12 f8 40 22 	brset	*0xf8, #0x40, 0x0xa7d7
    a7b5:	12 f8 20 0f 	brset	*0xf8, #0x20, 0x0xa7c8
    a7b9:	97 51       	staa	*0x51
    a7bb:	c6 51       	ldab	#0x51
    a7bd:	bd a8 5a    	jsr	0xa85a
    a7c0:	c6 67       	ldab	#0x67
    a7c2:	bd a8 a2    	jsr	0xa8a2
    a7c5:	7e a3 c9    	jmp	0xa3c9
    a7c8:	97 56       	staa	*0x56
    a7ca:	c6 56       	ldab	#0x56
    a7cc:	bd a8 5a    	jsr	0xa85a
    a7cf:	c6 6b       	ldab	#0x6b
    a7d1:	bd a8 a2    	jsr	0xa8a2
    a7d4:	7e a3 c9    	jmp	0xa3c9
    a7d7:	97 5b       	staa	*0x5b
    a7d9:	c6 5b       	ldab	#0x5b
    a7db:	bd a8 5a    	jsr	0xa85a
    a7de:	c6 70       	ldab	#0x70
    a7e0:	bd a8 a2    	jsr	0xa8a2
    a7e3:	7e a3 c9    	jmp	0xa3c9
    a7e6:	97 63       	staa	*0x63
    a7e8:	c6 63       	ldab	#0x63
    a7ea:	bd a8 5a    	jsr	0xa85a
    a7ed:	36          	psha
    a7ee:	b6 01 1b    	ldaa	0x11b
    a7f1:	81 14       	cmpa	#0x14
    a7f3:	26 15       	bne	0x0xa80a
    a7f5:	96 63       	ldaa	*0x63
    a7f7:	ce 01 31    	ldx	#0x131
    a7fa:	bd de a8    	jsr	0xdea8
    a7fd:	86 03       	ldaa	#0x3
    a7ff:	b7 01 1c    	staa	0x11c
    a802:	86 13       	ldaa	#0x13
    a804:	b7 01 1e    	staa	0x11e
    a807:	bd dd 89    	jsr	0xdd89
    a80a:	32          	pula
    a80b:	c6 6f       	ldab	#0x6f
    a80d:	bd a8 a2    	jsr	0xa8a2
    a810:	7e a3 c9    	jmp	0xa3c9
    a813:	12 f8 02 0f 	brset	*0xf8, #0x02, 0x0xa826
    a817:	97 4f       	staa	*0x4f
    a819:	c6 4f       	ldab	#0x4f
    a81b:	bd a8 5a    	jsr	0xa85a
    a81e:	c6 66       	ldab	#0x66
    a820:	bd a8 a2    	jsr	0xa8a2
    a823:	7e a3 c9    	jmp	0xa3c9
    a826:	97 d5       	staa	*0xd5
    a828:	c6 ac       	ldab	#0xac
    a82a:	bd a8 5a    	jsr	0xa85a
    a82d:	c6 07       	ldab	#0x7
    a82f:	bd a8 a2    	jsr	0xa8a2
    a832:	7e a3 c9    	jmp	0xa3c9
    a835:	12 f8 02 0f 	brset	*0xf8, #0x02, 0x0xa848
    a839:	97 4e       	staa	*0x4e
    a83b:	c6 4e       	ldab	#0x4e
    a83d:	bd a8 5a    	jsr	0xa85a
    a840:	c6 5a       	ldab	#0x5a
    a842:	bd a8 a2    	jsr	0xa8a2
    a845:	7e a3 c9    	jmp	0xa3c9
    a848:	97 47       	staa	*0x47
    a84a:	c6 47       	ldab	#0x47
    a84c:	bd a8 5a    	jsr	0xa85a
    a84f:	c6 56       	ldab	#0x56
    a851:	bd a8 a2    	jsr	0xa8a2
    a854:	7e a3 c9    	jmp	0xa3c9
    a857:	7e a3 c9    	jmp	0xa3c9
    a85a:	36          	psha
    a85b:	7d 00 d0    	tst	0xd0
    a85e:	27 05       	beq	0x0xa865
    a860:	7f 00 d0    	clr	0xd0
    a863:	20 08       	bra	0x0xa86d
    a865:	7d 10 29    	tst	0x1029
    a868:	2a fb       	bpl	0x0xa865
    a86a:	b6 10 2a    	ldaa	0x102a
    a86d:	37          	pshb
    a86e:	4f          	clra
    a86f:	d6 f9       	ldab	*0xf9
    a871:	c1 02       	cmpb	#0x2
    a873:	26 08       	bne	0x0xa87d
    a875:	86 02       	ldaa	#0x2
    a877:	7d 01 6b    	tst	0x16b
    a87a:	27 01       	beq	0x0xa87d
    a87c:	4a          	deca
    a87d:	b7 10 42    	staa	0x1042
    a880:	01          	nop
    a881:	01          	nop
    a882:	86 83       	ldaa	#0x83
    a884:	b7 10 2a    	staa	0x102a
    a887:	33          	pulb
    a888:	c0 20       	subb	#0x20
    a88a:	7d 10 29    	tst	0x1029
    a88d:	2a fb       	bpl	0x0xa88a
    a88f:	b6 10 2a    	ldaa	0x102a
    a892:	f7 10 2a    	stab	0x102a
    a895:	32          	pula
    a896:	7d 10 29    	tst	0x1029
    a899:	2a fb       	bpl	0x0xa896
    a89b:	f6 10 2a    	ldab	0x102a
    a89e:	b7 10 2a    	staa	0x102a
    a8a1:	39          	rts
    a8a2:	0f          	sei
    a8a3:	37          	pshb
    a8a4:	f6 01 6f    	ldab	0x16f
    a8a7:	ca b0       	orab	#0xb0
    a8a9:	fe 01 62    	ldx	0x162
    a8ac:	e7 00       	stab	0x0,x
    a8ae:	8f          	xgdx
    a8af:	5c          	incb
    a8b0:	c1 60       	cmpb	#0x60
    a8b2:	25 02       	bcs	0x0xa8b6
    a8b4:	c6 40       	ldab	#0x40
    a8b6:	8f          	xgdx
    a8b7:	33          	pulb
    a8b8:	e7 00       	stab	0x0,x
    a8ba:	8f          	xgdx
    a8bb:	5c          	incb
    a8bc:	c1 60       	cmpb	#0x60
    a8be:	25 02       	bcs	0x0xa8c2
    a8c0:	c6 40       	ldab	#0x40
    a8c2:	8f          	xgdx
    a8c3:	a7 00       	staa	0x0,x
    a8c5:	8f          	xgdx
    a8c6:	5c          	incb
    a8c7:	c1 60       	cmpb	#0x60
    a8c9:	25 02       	bcs	0x0xa8cd
    a8cb:	c6 40       	ldab	#0x40
    a8cd:	8f          	xgdx
    a8ce:	ff 01 62    	stx	0x162
    a8d1:	0e          	cli
    a8d2:	7d 00 d7    	tst	0xd7
    a8d5:	27 06       	beq	0x0xa8dd
    a8d7:	7f 00 d7    	clr	0xd7
    a8da:	7e fe ee    	jmp	0xfeee
    a8dd:	39          	rts
    a8de:	a9 34       	adca	0x34,x
    a8e0:	a9 77       	adca	0x77,x
    a8e2:	a9 9f       	adca	0x9f,x
    a8e4:	aa 57       	oraa	0x57,x
    a8e6:	ab 85       	adda	0x85,x
    a8e8:	ac b1       	cpx	0xb1,x
    a8ea:	ad 48       	jsr	0x48,x
    a8ec:	ae 5f       	lds	0x5f,x
    a8ee:	af 5d       	sts	0x5d,x
    a8f0:	af e7       	sts	0xe7,x
    a8f2:	b0 d8 b3    	suba	0xd8b3
    a8f5:	40          	nega
    a8f6:	b4 33 b5    	anda	0x33b5
    a8f9:	25 b6       	bcs	0x0xa8b1
    a8fb:	d6 b7       	ldab	*0xb7
    a8fd:	a0 b8       	suba	0xb8,x
    a8ff:	08          	inx
    a900:	b9 18 ba    	adca	0x18ba
    a903:	28 ba       	bvc	0x0xa8bf
    a905:	84 ba       	anda	#0xba
    a907:	87          	.byte	0x87
    a908:	bc 11 bc    	cpx	0x11bc
    a90b:	f8 bd df    	eorb	0xbddf
    a90e:	be af bf    	lds	0xafbf
    a911:	98 c0       	eora	*0xc0
    a913:	b1 c1 a9    	cmpa	0xc1a9
    a916:	c3 87 c4    	addd	#0x87c4
    a919:	b6 c6 1d    	ldaa	0xc61d
    a91c:	c6 3d       	ldab	#0x3d
    a91e:	c7          	.byte	0xc7
    a91f:	58          	aslb
    a920:	c8 19       	eorb	#0x19
    a922:	c8 da       	eorb	#0xda
    a924:	c9 ba       	adcb	#0xba
    a926:	ca 52       	orab	#0x52
    a928:	f6 01 1b    	ldab	0x11b
    a92b:	58          	aslb
    a92c:	ce a8 de    	ldx	#0xa8de
    a92f:	3a          	abx
    a930:	ee 00       	ldx	0x0,x
    a932:	6e 00       	jmp	0x0,x
    a934:	7d 01 1c    	tst	0x11c
    a937:	2a 03       	bpl	0x0xa93c
    a939:	7e ca 69    	jmp	0xca69
    a93c:	7d 01 1c    	tst	0x11c
    a93f:	27 03       	beq	0x0xa944
    a941:	7e a9 6a    	jmp	0xa96a
    a944:	7d 01 1e    	tst	0x11e
    a947:	26 08       	bne	0x0xa951
    a949:	86 0c       	ldaa	#0xc
    a94b:	b7 01 1e    	staa	0x11e
    a94e:	bd dd 89    	jsr	0xdd89
    a951:	7d 01 7e    	tst	0x17e
    a954:	27 0c       	beq	0x0xa962
    a956:	7f 01 7e    	clr	0x17e
    a959:	7f 01 7f    	clr	0x17f
    a95c:	7f 01 1a    	clr	0x11a
    a95f:	7e ca 69    	jmp	0xca69
    a962:	7d 01 1c    	tst	0x11c
    a965:	26 03       	bne	0x0xa96a
    a967:	7e ca 55    	jmp	0xca55
    a96a:	b6 10 23    	ldaa	0x1023
    a96d:	85 10       	bita	#0x10
    a96f:	27 03       	beq	0x0xa974
    a971:	7e dc fd    	jmp	0xdcfd
    a974:	7e ca 55    	jmp	0xca55
    a977:	7d 01 1c    	tst	0x11c
    a97a:	2a 03       	bpl	0x0xa97f
    a97c:	7e cb 2e    	jmp	0xcb2e
    a97f:	7d 01 1c    	tst	0x11c
    a982:	27 03       	beq	0x0xa987
    a984:	7e a9 6a    	jmp	0xa96a
    a987:	7d 01 1e    	tst	0x11e
    a98a:	26 08       	bne	0x0xa994
    a98c:	86 0c       	ldaa	#0xc
    a98e:	b7 01 1e    	staa	0x11e
    a991:	bd dd 89    	jsr	0xdd89
    a994:	7d 01 7e    	tst	0x17e
    a997:	27 03       	beq	0x0xa99c
    a999:	7e cb 2e    	jmp	0xcb2e
    a99c:	7e ca 55    	jmp	0xca55
    a99f:	7d 01 1c    	tst	0x11c
    a9a2:	2a 03       	bpl	0x0xa9a7
    a9a4:	7e cc 28    	jmp	0xcc28
    a9a7:	7d 01 1c    	tst	0x11c
    a9aa:	27 03       	beq	0x0xa9af
    a9ac:	7e a9 6a    	jmp	0xa96a
    a9af:	7d 01 1e    	tst	0x11e
    a9b2:	26 08       	bne	0x0xa9bc
    a9b4:	86 10       	ldaa	#0x10
    a9b6:	b7 01 1e    	staa	0x11e
    a9b9:	bd dd 89    	jsr	0xdd89
    a9bc:	7d 01 7f    	tst	0x17f
    a9bf:	27 29       	beq	0x0xa9ea
    a9c1:	2b 1d       	bmi	0x0xa9e0
    a9c3:	b6 01 1e    	ldaa	0x11e
    a9c6:	4c          	inca
    a9c7:	81 1f       	cmpa	#0x1f
    a9c9:	23 02       	bls	0x0xa9cd
    a9cb:	20 06       	bra	0x0xa9d3
    a9cd:	b7 01 1e    	staa	0x11e
    a9d0:	bd dd 89    	jsr	0xdd89
    a9d3:	4f          	clra
    a9d4:	b7 01 7f    	staa	0x17f
    a9d7:	b7 01 7e    	staa	0x17e
    a9da:	b7 01 1a    	staa	0x11a
    a9dd:	7e ca 55    	jmp	0xca55
    a9e0:	b6 01 1e    	ldaa	0x11e
    a9e3:	4a          	deca
    a9e4:	81 0f       	cmpa	#0xf
    a9e6:	22 e5       	bhi	0x0xa9cd
    a9e8:	20 e9       	bra	0x0xa9d3
    a9ea:	7d 01 7e    	tst	0x17e
    a9ed:	27 0f       	beq	0x0xa9fe
    a9ef:	7f 01 7e    	clr	0x17e
    a9f2:	ce 01 20    	ldx	#0x120
    a9f5:	f6 01 1e    	ldab	0x11e
    a9f8:	3a          	abx
    a9f9:	86 20       	ldaa	#0x20
    a9fb:	7e aa 2a    	jmp	0xaa2a
    a9fe:	7d 01 1a    	tst	0x11a
    aa01:	26 03       	bne	0x0xaa06
    aa03:	7e ca 55    	jmp	0xca55
    aa06:	ce 01 20    	ldx	#0x120
    aa09:	f6 01 1e    	ldab	0x11e
    aa0c:	3a          	abx
    aa0d:	a6 00       	ldaa	0x0,x
    aa0f:	81 5f       	cmpa	#0x5f
    aa11:	23 02       	bls	0x0xaa15
    aa13:	86 20       	ldaa	#0x20
    aa15:	7d 01 1a    	tst	0x11a
    aa18:	2b 09       	bmi	0x0xaa23
    aa1a:	4c          	inca
    aa1b:	81 5f       	cmpa	#0x5f
    aa1d:	23 0b       	bls	0x0xaa2a
    aa1f:	86 20       	ldaa	#0x20
    aa21:	20 07       	bra	0x0xaa2a
    aa23:	4a          	deca
    aa24:	81 20       	cmpa	#0x20
    aa26:	24 02       	bcc	0x0xaa2a
    aa28:	86 5f       	ldaa	#0x5f
    aa2a:	7f 01 1a    	clr	0x11a
    aa2d:	a7 00       	staa	0x0,x
    aa2f:	36          	psha
    aa30:	bd dd 4d    	jsr	0xdd4d
    aa33:	32          	pula
    aa34:	d6 f9       	ldab	*0xf9
    aa36:	c1 02       	cmpb	#0x2
    aa38:	27 0d       	beq	0x0xaa47
    aa3a:	8f          	xgdx
    aa3b:	83 00 70    	subd	#0x70
    aa3e:	8f          	xgdx
    aa3f:	a7 00       	staa	0x0,x
    aa41:	bd dd 89    	jsr	0xdd89
    aa44:	7e ca 55    	jmp	0xca55
    aa47:	8f          	xgdx
    aa48:	83 01 20    	subd	#0x120
    aa4b:	c3 50 20    	addd	#0x5020
    aa4e:	8f          	xgdx
    aa4f:	a7 00       	staa	0x0,x
    aa51:	bd dd 89    	jsr	0xdd89
    aa54:	7e ca 55    	jmp	0xca55
    aa57:	7d 01 1c    	tst	0x11c
    aa5a:	2a 03       	bpl	0x0xaa5f
    aa5c:	7e cc c5    	jmp	0xccc5
    aa5f:	7d 01 1e    	tst	0x11e
    aa62:	26 08       	bne	0x0xaa6c
    aa64:	7d 01 1c    	tst	0x11c
    aa67:	27 1f       	beq	0x0xaa88
    aa69:	7e a9 6a    	jmp	0xa96a
    aa6c:	7d 01 1c    	tst	0x11c
    aa6f:	27 1f       	beq	0x0xaa90
    aa71:	cc 01 20    	ldd	#0x120
    aa74:	fb 01 1e    	addb	0x11e
    aa77:	8f          	xgdx
    aa78:	bd dd 4d    	jsr	0xdd4d
    aa7b:	7a 01 1e    	dec	0x11e
    aa7e:	7a 01 1c    	dec	0x11c
    aa81:	26 0d       	bne	0x0xaa90
    aa83:	bd dd cb    	jsr	0xddcb
    aa86:	20 08       	bra	0x0xaa90
    aa88:	86 13       	ldaa	#0x13
    aa8a:	b7 01 1e    	staa	0x11e
    aa8d:	bd dd 89    	jsr	0xdd89
    aa90:	7d 01 7f    	tst	0x17f
    aa93:	27 10       	beq	0x0xaaa5
    aa95:	bd df d4    	jsr	0xdfd4
    aa98:	4f          	clra
    aa99:	b7 01 7f    	staa	0x17f
    aa9c:	b7 01 7e    	staa	0x17e
    aa9f:	b7 01 1a    	staa	0x11a
    aaa2:	7e ca 55    	jmp	0xca55
    aaa5:	7d 01 1a    	tst	0x11a
    aaa8:	26 03       	bne	0x0xaaad
    aaaa:	7e ca 55    	jmp	0xca55
    aaad:	b6 01 1e    	ldaa	0x11e
    aab0:	81 13       	cmpa	#0x13
    aab2:	26 46       	bne	0x0xaafa
    aab4:	b6 01 1a    	ldaa	0x11a
    aab7:	2b 3b       	bmi	0x0xaaf4
    aab9:	f6 01 6f    	ldab	0x16f
    aabc:	5c          	incb
    aabd:	c4 0f       	andb	#0xf
    aabf:	f7 01 6f    	stab	0x16f
    aac2:	b6 10 00    	ldaa	0x1000
    aac5:	36          	psha
    aac6:	84 ef       	anda	#0xef
    aac8:	b7 10 00    	staa	0x1000
    aacb:	b6 10 08    	ldaa	0x1008
    aace:	36          	psha
    aacf:	84 1f       	anda	#0x1f
    aad1:	b7 10 08    	staa	0x1008
    aad4:	f7 7f fe    	stab	0x7ffe
    aad7:	32          	pula
    aad8:	b7 10 08    	staa	0x1008
    aadb:	32          	pula
    aadc:	b7 10 00    	staa	0x1000
    aadf:	ce cd 23    	ldx	#0xcd23
    aae2:	18 ce 01 30 	ldy	#0x130
    aae6:	bd df 5b    	jsr	0xdf5b
    aae9:	7f 01 1a    	clr	0x11a
    aaec:	86 04       	ldaa	#0x4
    aaee:	b7 01 1c    	staa	0x11c
    aaf1:	7e ca 55    	jmp	0xca55
    aaf4:	f6 01 6f    	ldab	0x16f
    aaf7:	5a          	decb
    aaf8:	20 c3       	bra	0x0xaabd
    aafa:	81 19       	cmpa	#0x19
    aafc:	26 34       	bne	0x0xab32
    aafe:	b6 01 1a    	ldaa	0x11a
    ab01:	2b 27       	bmi	0x0xab2a
    ab03:	d6 93       	ldab	*0x93
    ab05:	5c          	incb
    ab06:	c1 05       	cmpb	#0x5
    ab08:	25 02       	bcs	0x0xab0c
    ab0a:	c6 04       	ldab	#0x4
    ab0c:	d7 93       	stab	*0x93
    ab0e:	ce cd 67    	ldx	#0xcd67
    ab11:	18 ce 01 36 	ldy	#0x136
    ab15:	bd df 5b    	jsr	0xdf5b
    ab18:	7f 01 1a    	clr	0x11a
    ab1b:	86 04       	ldaa	#0x4
    ab1d:	b7 01 1c    	staa	0x11c
    ab20:	96 93       	ldaa	*0x93
    ab22:	c6 93       	ldab	#0x93
    ab24:	bd a8 5a    	jsr	0xa85a
    ab27:	7e ca 55    	jmp	0xca55
    ab2a:	d6 93       	ldab	*0x93
    ab2c:	5a          	decb
    ab2d:	2a dd       	bpl	0x0xab0c
    ab2f:	5f          	clrb
    ab30:	20 da       	bra	0x0xab0c
    ab32:	ce 00 20    	ldx	#0x20
    ab35:	18 ce e2 fa 	ldy	#0xe2fa
    ab39:	c6 b0       	ldab	#0xb0
    ab3b:	18 a6 00    	ldaa	0x0,y
    ab3e:	a7 00       	staa	0x0,x
    ab40:	08          	inx
    ab41:	18 08       	iny
    ab43:	5a          	decb
    ab44:	26 f5       	bne	0x0xab3b
    ab46:	7d 00 d0    	tst	0xd0
    ab49:	26 0b       	bne	0x0xab56
    ab4b:	7d 10 29    	tst	0x1029
    ab4e:	2a fb       	bpl	0x0xab4b
    ab50:	b6 10 2a    	ldaa	0x102a
    ab53:	7c 00 d0    	inc	0xd0
    ab56:	4f          	clra
    ab57:	b7 10 42    	staa	0x1042
    ab5a:	bd 9e 2d    	jsr	0x9e2d
    ab5d:	4f          	clra
    ab5e:	97 f2       	staa	*0xf2
    ab60:	97 f6       	staa	*0xf6
    ab62:	97 ff       	staa	*0xff
    ab64:	b7 01 1b    	staa	0x11b
    ab67:	97 fb       	staa	*0xfb
    ab69:	96 41       	ldaa	*0x41
    ab6b:	84 10       	anda	#0x10
    ab6d:	48          	asla
    ab6e:	48          	asla
    ab6f:	48          	asla
    ab70:	9a 44       	oraa	*0x44
    ab72:	97 f7       	staa	*0xf7
    ab74:	96 6a       	ldaa	*0x6a
    ab76:	84 20       	anda	#0x20
    ab78:	97 f5       	staa	*0xf5
    ab7a:	86 80       	ldaa	#0x80
    ab7c:	b7 01 1c    	staa	0x11c
    ab7f:	7f 01 1a    	clr	0x11a
    ab82:	7e ca 55    	jmp	0xca55
    ab85:	7d 01 1c    	tst	0x11c
    ab88:	2a 03       	bpl	0x0xab8d
    ab8a:	7e cd 81    	jmp	0xcd81
    ab8d:	7d 01 1e    	tst	0x11e
    ab90:	26 08       	bne	0x0xab9a
    ab92:	7d 01 1c    	tst	0x11c
    ab95:	27 1f       	beq	0x0xabb6
    ab97:	7e a9 6a    	jmp	0xa96a
    ab9a:	7d 01 1c    	tst	0x11c
    ab9d:	27 24       	beq	0x0xabc3
    ab9f:	cc 01 20    	ldd	#0x120
    aba2:	fb 01 1e    	addb	0x11e
    aba5:	8f          	xgdx
    aba6:	bd dd 4d    	jsr	0xdd4d
    aba9:	7a 01 1e    	dec	0x11e
    abac:	7a 01 1c    	dec	0x11c
    abaf:	26 12       	bne	0x0xabc3
    abb1:	bd dd cb    	jsr	0xddcb
    abb4:	20 0d       	bra	0x0xabc3
    abb6:	7d 01 1e    	tst	0x11e
    abb9:	26 08       	bne	0x0xabc3
    abbb:	86 13       	ldaa	#0x13
    abbd:	b7 01 1e    	staa	0x11e
    abc0:	bd dd 89    	jsr	0xdd89
    abc3:	7d 01 7f    	tst	0x17f
    abc6:	27 10       	beq	0x0xabd8
    abc8:	bd df d4    	jsr	0xdfd4
    abcb:	4f          	clra
    abcc:	b7 01 7f    	staa	0x17f
    abcf:	b7 01 7e    	staa	0x17e
    abd2:	b7 01 1a    	staa	0x11a
    abd5:	7e ca 55    	jmp	0xca55
    abd8:	7d 01 1a    	tst	0x11a
    abdb:	26 03       	bne	0x0xabe0
    abdd:	7e ca 55    	jmp	0xca55
    abe0:	b6 01 1e    	ldaa	0x11e
    abe3:	81 13       	cmpa	#0x13
    abe5:	26 4d       	bne	0x0xac34
    abe7:	b6 01 1a    	ldaa	0x11a
    abea:	2b 3e       	bmi	0x0xac2a
    abec:	f6 01 70    	ldab	0x170
    abef:	5c          	incb
    abf0:	c1 03       	cmpb	#0x3
    abf2:	25 01       	bcs	0x0xabf5
    abf4:	5f          	clrb
    abf5:	f7 01 70    	stab	0x170
    abf8:	b6 10 00    	ldaa	0x1000
    abfb:	36          	psha
    abfc:	84 ef       	anda	#0xef
    abfe:	b7 10 00    	staa	0x1000
    ac01:	b6 10 08    	ldaa	0x1008
    ac04:	36          	psha
    ac05:	84 1f       	anda	#0x1f
    ac07:	b7 10 08    	staa	0x1008
    ac0a:	f7 7f ff    	stab	0x7fff
    ac0d:	32          	pula
    ac0e:	b7 10 08    	staa	0x1008
    ac11:	32          	pula
    ac12:	b7 10 00    	staa	0x1000
    ac15:	ce cd dd    	ldx	#0xcddd
    ac18:	18 ce 01 30 	ldy	#0x130
    ac1c:	bd df 5b    	jsr	0xdf5b
    ac1f:	7f 01 1a    	clr	0x11a
    ac22:	86 04       	ldaa	#0x4
    ac24:	b7 01 1c    	staa	0x11c
    ac27:	7e ca 55    	jmp	0xca55
    ac2a:	f6 01 70    	ldab	0x170
    ac2d:	5a          	decb
    ac2e:	2a c5       	bpl	0x0xabf5
    ac30:	c6 02       	ldab	#0x2
    ac32:	20 c1       	bra	0x0xabf5
    ac34:	81 19       	cmpa	#0x19
    ac36:	26 2d       	bne	0x0xac65
    ac38:	b6 01 1a    	ldaa	0x11a
    ac3b:	2b 1f       	bmi	0x0xac5c
    ac3d:	d6 94       	ldab	*0x94
    ac3f:	5c          	incb
    ac40:	c1 03       	cmpb	#0x3
    ac42:	25 01       	bcs	0x0xac45
    ac44:	5f          	clrb
    ac45:	d7 94       	stab	*0x94
    ac47:	ce cd e9    	ldx	#0xcde9
    ac4a:	18 ce 01 36 	ldy	#0x136
    ac4e:	bd df 5b    	jsr	0xdf5b
    ac51:	7f 01 1a    	clr	0x11a
    ac54:	86 04       	ldaa	#0x4
    ac56:	b7 01 1c    	staa	0x11c
    ac59:	7e ca 55    	jmp	0xca55
    ac5c:	d6 94       	ldab	*0x94
    ac5e:	5a          	decb
    ac5f:	2a e4       	bpl	0x0xac45
    ac61:	c6 02       	ldab	#0x2
    ac63:	20 e0       	bra	0x0xac45
    ac65:	b6 01 1a    	ldaa	0x11a
    ac68:	2b 3e       	bmi	0x0xaca8
    ac6a:	b6 01 71    	ldaa	0x171
    ac6d:	4c          	inca
    ac6e:	81 32       	cmpa	#0x32
    ac70:	23 02       	bls	0x0xac74
    ac72:	86 32       	ldaa	#0x32
    ac74:	b7 01 71    	staa	0x171
    ac77:	b7 10 46    	staa	0x1046
    ac7a:	f6 10 00    	ldab	0x1000
    ac7d:	37          	pshb
    ac7e:	c4 ef       	andb	#0xef
    ac80:	f7 10 00    	stab	0x1000
    ac83:	f6 10 08    	ldab	0x1008
    ac86:	37          	pshb
    ac87:	c4 1f       	andb	#0x1f
    ac89:	f7 10 08    	stab	0x1008
    ac8c:	b7 7f fc    	staa	0x7ffc
    ac8f:	33          	pulb
    ac90:	f7 10 08    	stab	0x1008
    ac93:	33          	pulb
    ac94:	f7 10 00    	stab	0x1000
    ac97:	ce 01 3c    	ldx	#0x13c
    ac9a:	bd de a8    	jsr	0xdea8
    ac9d:	7f 01 1a    	clr	0x11a
    aca0:	86 03       	ldaa	#0x3
    aca2:	b7 01 1c    	staa	0x11c
    aca5:	7e ca 55    	jmp	0xca55
    aca8:	b6 01 71    	ldaa	0x171
    acab:	4a          	deca
    acac:	2a c6       	bpl	0x0xac74
    acae:	4f          	clra
    acaf:	20 c3       	bra	0x0xac74
    acb1:	7d 01 1c    	tst	0x11c
    acb4:	2a 03       	bpl	0x0xacb9
    acb6:	7e cd f5    	jmp	0xcdf5
    acb9:	7d 01 1e    	tst	0x11e
    acbc:	26 08       	bne	0x0xacc6
    acbe:	7d 01 1c    	tst	0x11c
    acc1:	27 1f       	beq	0x0xace2
    acc3:	7e a9 6a    	jmp	0xa96a
    acc6:	7d 01 1c    	tst	0x11c
    acc9:	27 24       	beq	0x0xacef
    accb:	cc 01 20    	ldd	#0x120
    acce:	fb 01 1e    	addb	0x11e
    acd1:	8f          	xgdx
    acd2:	bd dd 4d    	jsr	0xdd4d
    acd5:	7a 01 1e    	dec	0x11e
    acd8:	7a 01 1c    	dec	0x11c
    acdb:	26 12       	bne	0x0xacef
    acdd:	bd dd cb    	jsr	0xddcb
    ace0:	20 0d       	bra	0x0xacef
    ace2:	7d 01 1e    	tst	0x11e
    ace5:	26 08       	bne	0x0xacef
    ace7:	86 1e       	ldaa	#0x1e
    ace9:	b7 01 1e    	staa	0x11e
    acec:	bd dd 89    	jsr	0xdd89
    acef:	7f 01 7e    	clr	0x17e
    acf2:	7f 01 7f    	clr	0x17f
    acf5:	7d 01 1a    	tst	0x11a
    acf8:	26 03       	bne	0x0xacfd
    acfa:	7e ca 55    	jmp	0xca55
    acfd:	b6 01 3c    	ldaa	0x13c
    ad00:	81 41       	cmpa	#0x41
    ad02:	26 04       	bne	0x0xad08
    ad04:	86 81       	ldaa	#0x81
    ad06:	20 06       	bra	0x0xad0e
    ad08:	ce 01 3c    	ldx	#0x13c
    ad0b:	bd df 02    	jsr	0xdf02
    ad0e:	7d 01 1a    	tst	0x11a
    ad11:	2b 09       	bmi	0x0xad1c
    ad13:	4c          	inca
    ad14:	81 81       	cmpa	#0x81
    ad16:	23 09       	bls	0x0xad21
    ad18:	86 01       	ldaa	#0x1
    ad1a:	20 05       	bra	0x0xad21
    ad1c:	4a          	deca
    ad1d:	26 02       	bne	0x0xad21
    ad1f:	86 81       	ldaa	#0x81
    ad21:	81 81       	cmpa	#0x81
    ad23:	26 0f       	bne	0x0xad34
    ad25:	86 41       	ldaa	#0x41
    ad27:	b7 01 3c    	staa	0x13c
    ad2a:	86 4c       	ldaa	#0x4c
    ad2c:	b7 01 3d    	staa	0x13d
    ad2f:	b7 01 3e    	staa	0x13e
    ad32:	20 06       	bra	0x0xad3a
    ad34:	ce 01 3c    	ldx	#0x13c
    ad37:	bd de a8    	jsr	0xdea8
    ad3a:	7f 01 1a    	clr	0x11a
    ad3d:	86 03       	ldaa	#0x3
    ad3f:	b7 01 1c    	staa	0x11c
    ad42:	14 f2 40    	bset	*0xf2, #0x40
    ad45:	7e ca 55    	jmp	0xca55
    ad48:	7d 01 1c    	tst	0x11c
    ad4b:	2a 03       	bpl	0x0xad50
    ad4d:	7e ce 8b    	jmp	0xce8b
    ad50:	7d 01 1e    	tst	0x11e
    ad53:	26 08       	bne	0x0xad5d
    ad55:	7d 01 1c    	tst	0x11c
    ad58:	27 1f       	beq	0x0xad79
    ad5a:	7e a9 6a    	jmp	0xa96a
    ad5d:	7d 01 1c    	tst	0x11c
    ad60:	27 24       	beq	0x0xad86
    ad62:	cc 01 20    	ldd	#0x120
    ad65:	fb 01 1e    	addb	0x11e
    ad68:	8f          	xgdx
    ad69:	bd dd 4d    	jsr	0xdd4d
    ad6c:	7a 01 1e    	dec	0x11e
    ad6f:	7a 01 1c    	dec	0x11c
    ad72:	26 12       	bne	0x0xad86
    ad74:	bd dd cb    	jsr	0xddcb
    ad77:	20 0d       	bra	0x0xad86
    ad79:	7d 01 1e    	tst	0x11e
    ad7c:	26 08       	bne	0x0xad86
    ad7e:	86 13       	ldaa	#0x13
    ad80:	b7 01 1e    	staa	0x11e
    ad83:	bd dd 89    	jsr	0xdd89
    ad86:	7d 01 7f    	tst	0x17f
    ad89:	27 10       	beq	0x0xad9b
    ad8b:	bd df d4    	jsr	0xdfd4
    ad8e:	4f          	clra
    ad8f:	b7 01 7f    	staa	0x17f
    ad92:	b7 01 7e    	staa	0x17e
    ad95:	b7 01 1a    	staa	0x11a
    ad98:	7e ca 55    	jmp	0xca55
    ad9b:	7d 01 1a    	tst	0x11a
    ad9e:	26 03       	bne	0x0xada3
    ada0:	7e ca 55    	jmp	0xca55
    ada3:	b6 01 1e    	ldaa	0x11e
    ada6:	81 13       	cmpa	#0x13
    ada8:	26 51       	bne	0x0xadfb
    adaa:	b6 01 1a    	ldaa	0x11a
    adad:	2b 43       	bmi	0x0xadf2
    adaf:	b6 01 6e    	ldaa	0x16e
    adb2:	4c          	inca
    adb3:	81 7f       	cmpa	#0x7f
    adb5:	25 02       	bcs	0x0xadb9
    adb7:	86 7f       	ldaa	#0x7f
    adb9:	b7 01 6e    	staa	0x16e
    adbc:	f6 10 00    	ldab	0x1000
    adbf:	37          	pshb
    adc0:	c4 ef       	andb	#0xef
    adc2:	f7 10 00    	stab	0x1000
    adc5:	f6 10 08    	ldab	0x1008
    adc8:	37          	pshb
    adc9:	c4 1f       	andb	#0x1f
    adcb:	f7 10 08    	stab	0x1008
    adce:	b7 7f fd    	staa	0x7ffd
    add1:	33          	pulb
    add2:	f7 10 08    	stab	0x1008
    add5:	33          	pulb
    add6:	f7 10 00    	stab	0x1000
    add9:	ce 01 31    	ldx	#0x131
    addc:	bd de e4    	jsr	0xdee4
    addf:	7f 01 1a    	clr	0x11a
    ade2:	86 03       	ldaa	#0x3
    ade4:	b7 01 1c    	staa	0x11c
    ade7:	b6 01 6e    	ldaa	0x16e
    adea:	c6 ab       	ldab	#0xab
    adec:	bd a8 5a    	jsr	0xa85a
    adef:	7e ca 55    	jmp	0xca55
    adf2:	b6 01 6e    	ldaa	0x16e
    adf5:	4a          	deca
    adf6:	2a c1       	bpl	0x0xadb9
    adf8:	4f          	clra
    adf9:	20 be       	bra	0x0xadb9
    adfb:	81 19       	cmpa	#0x19
    adfd:	26 30       	bne	0x0xae2f
    adff:	b6 01 1a    	ldaa	0x11a
    ae02:	2b 23       	bmi	0x0xae27
    ae04:	96 3e       	ldaa	*0x3e
    ae06:	4c          	inca
    ae07:	81 40       	cmpa	#0x40
    ae09:	25 02       	bcs	0x0xae0d
    ae0b:	86 3f       	ldaa	#0x3f
    ae0d:	97 3e       	staa	*0x3e
    ae0f:	ce 01 37    	ldx	#0x137
    ae12:	bd de a8    	jsr	0xdea8
    ae15:	7f 01 1a    	clr	0x11a
    ae18:	86 03       	ldaa	#0x3
    ae1a:	b7 01 1c    	staa	0x11c
    ae1d:	96 3e       	ldaa	*0x3e
    ae1f:	c6 3e       	ldab	#0x3e
    ae21:	bd a8 5a    	jsr	0xa85a
    ae24:	7e ca 55    	jmp	0xca55
    ae27:	96 3e       	ldaa	*0x3e
    ae29:	4a          	deca
    ae2a:	2a e1       	bpl	0x0xae0d
    ae2c:	4f          	clra
    ae2d:	20 de       	bra	0x0xae0d
    ae2f:	b6 01 1a    	ldaa	0x11a
    ae32:	2b 23       	bmi	0x0xae57
    ae34:	96 3f       	ldaa	*0x3f
    ae36:	4c          	inca
    ae37:	81 40       	cmpa	#0x40
    ae39:	25 02       	bcs	0x0xae3d
    ae3b:	86 3f       	ldaa	#0x3f
    ae3d:	97 3f       	staa	*0x3f
    ae3f:	ce 01 3c    	ldx	#0x13c
    ae42:	bd de a8    	jsr	0xdea8
    ae45:	7f 01 1a    	clr	0x11a
    ae48:	86 03       	ldaa	#0x3
    ae4a:	b7 01 1c    	staa	0x11c
    ae4d:	96 3f       	ldaa	*0x3f
    ae4f:	c6 3f       	ldab	#0x3f
    ae51:	bd a8 5a    	jsr	0xa85a
    ae54:	7e ca 55    	jmp	0xca55
    ae57:	96 3f       	ldaa	*0x3f
    ae59:	4a          	deca
    ae5a:	2a e1       	bpl	0x0xae3d
    ae5c:	4f          	clra
    ae5d:	20 de       	bra	0x0xae3d
    ae5f:	7d 01 1c    	tst	0x11c
    ae62:	2a 03       	bpl	0x0xae67
    ae64:	7e ce ed    	jmp	0xceed
    ae67:	7d 01 1e    	tst	0x11e
    ae6a:	26 08       	bne	0x0xae74
    ae6c:	7d 01 1c    	tst	0x11c
    ae6f:	27 1f       	beq	0x0xae90
    ae71:	7e a9 6a    	jmp	0xa96a
    ae74:	7d 01 1c    	tst	0x11c
    ae77:	27 24       	beq	0x0xae9d
    ae79:	cc 01 20    	ldd	#0x120
    ae7c:	fb 01 1e    	addb	0x11e
    ae7f:	8f          	xgdx
    ae80:	bd dd 4d    	jsr	0xdd4d
    ae83:	7a 01 1e    	dec	0x11e
    ae86:	7a 01 1c    	dec	0x11c
    ae89:	26 12       	bne	0x0xae9d
    ae8b:	bd dd cb    	jsr	0xddcb
    ae8e:	20 0d       	bra	0x0xae9d
    ae90:	7d 01 1e    	tst	0x11e
    ae93:	26 08       	bne	0x0xae9d
    ae95:	86 13       	ldaa	#0x13
    ae97:	b7 01 1e    	staa	0x11e
    ae9a:	bd dd 89    	jsr	0xdd89
    ae9d:	7d 01 7f    	tst	0x17f
    aea0:	27 10       	beq	0x0xaeb2
    aea2:	bd df d4    	jsr	0xdfd4
    aea5:	4f          	clra
    aea6:	b7 01 7f    	staa	0x17f
    aea9:	b7 01 7e    	staa	0x17e
    aeac:	b7 01 1a    	staa	0x11a
    aeaf:	7e ca 55    	jmp	0xca55
    aeb2:	7d 01 1a    	tst	0x11a
    aeb5:	26 03       	bne	0x0xaeba
    aeb7:	7e ca 55    	jmp	0xca55
    aeba:	b6 01 1e    	ldaa	0x11e
    aebd:	81 13       	cmpa	#0x13
    aebf:	26 30       	bne	0x0xaef1
    aec1:	b6 01 1a    	ldaa	0x11a
    aec4:	2b 23       	bmi	0x0xaee9
    aec6:	96 40       	ldaa	*0x40
    aec8:	4c          	inca
    aec9:	81 7f       	cmpa	#0x7f
    aecb:	25 02       	bcs	0x0xaecf
    aecd:	86 7f       	ldaa	#0x7f
    aecf:	97 40       	staa	*0x40
    aed1:	ce 01 31    	ldx	#0x131
    aed4:	bd de e4    	jsr	0xdee4
    aed7:	7f 01 1a    	clr	0x11a
    aeda:	86 03       	ldaa	#0x3
    aedc:	b7 01 1c    	staa	0x11c
    aedf:	96 40       	ldaa	*0x40
    aee1:	c6 40       	ldab	#0x40
    aee3:	bd a8 5a    	jsr	0xa85a
    aee6:	7e ca 55    	jmp	0xca55
    aee9:	96 40       	ldaa	*0x40
    aeeb:	4a          	deca
    aeec:	2a e1       	bpl	0x0xaecf
    aeee:	4f          	clra
    aeef:	20 de       	bra	0x0xaecf
    aef1:	81 19       	cmpa	#0x19
    aef3:	26 3d       	bne	0x0xaf32
    aef5:	b6 01 1a    	ldaa	0x11a
    aef8:	2b 2e       	bmi	0x0xaf28
    aefa:	d6 41       	ldab	*0x41
    aefc:	c4 0f       	andb	#0xf
    aefe:	5c          	incb
    aeff:	c1 05       	cmpb	#0x5
    af01:	25 02       	bcs	0x0xaf05
    af03:	c6 04       	ldab	#0x4
    af05:	96 41       	ldaa	*0x41
    af07:	84 10       	anda	#0x10
    af09:	1b          	aba
    af0a:	97 41       	staa	*0x41
    af0c:	ce cf 45    	ldx	#0xcf45
    af0f:	18 ce 01 36 	ldy	#0x136
    af13:	bd df 5b    	jsr	0xdf5b
    af16:	7f 01 1a    	clr	0x11a
    af19:	86 04       	ldaa	#0x4
    af1b:	b7 01 1c    	staa	0x11c
    af1e:	96 41       	ldaa	*0x41
    af20:	c6 41       	ldab	#0x41
    af22:	bd a8 5a    	jsr	0xa85a
    af25:	7e ca 55    	jmp	0xca55
    af28:	d6 41       	ldab	*0x41
    af2a:	c4 0f       	andb	#0xf
    af2c:	5a          	decb
    af2d:	2a d6       	bpl	0x0xaf05
    af2f:	5f          	clrb
    af30:	20 d3       	bra	0x0xaf05
    af32:	b6 01 1a    	ldaa	0x11a
    af35:	2b 1f       	bmi	0x0xaf56
    af37:	96 67       	ldaa	*0x67
    af39:	4c          	inca
    af3a:	84 7f       	anda	#0x7f
    af3c:	97 67       	staa	*0x67
    af3e:	ce 01 3c    	ldx	#0x13c
    af41:	bd de a8    	jsr	0xdea8
    af44:	7f 01 1a    	clr	0x11a
    af47:	86 03       	ldaa	#0x3
    af49:	b7 01 1c    	staa	0x11c
    af4c:	96 67       	ldaa	*0x67
    af4e:	c6 67       	ldab	#0x67
    af50:	bd a8 5a    	jsr	0xa85a
    af53:	7e ca 55    	jmp	0xca55
    af56:	96 67       	ldaa	*0x67
    af58:	4a          	deca
    af59:	84 7f       	anda	#0x7f
    af5b:	20 df       	bra	0x0xaf3c
    af5d:	7d 01 1c    	tst	0x11c
    af60:	2a 03       	bpl	0x0xaf65
    af62:	7e cf 59    	jmp	0xcf59
    af65:	7d 01 1e    	tst	0x11e
    af68:	26 08       	bne	0x0xaf72
    af6a:	7d 01 1c    	tst	0x11c
    af6d:	27 1f       	beq	0x0xaf8e
    af6f:	7e a9 6a    	jmp	0xa96a
    af72:	7d 01 1c    	tst	0x11c
    af75:	27 24       	beq	0x0xaf9b
    af77:	cc 01 20    	ldd	#0x120
    af7a:	fb 01 1e    	addb	0x11e
    af7d:	8f          	xgdx
    af7e:	bd dd 4d    	jsr	0xdd4d
    af81:	7a 01 1e    	dec	0x11e
    af84:	7a 01 1c    	dec	0x11c
    af87:	26 12       	bne	0x0xaf9b
    af89:	bd dd cb    	jsr	0xddcb
    af8c:	20 0d       	bra	0x0xaf9b
    af8e:	7d 01 1e    	tst	0x11e
    af91:	26 08       	bne	0x0xaf9b
    af93:	86 13       	ldaa	#0x13
    af95:	b7 01 1e    	staa	0x11e
    af98:	bd dd 89    	jsr	0xdd89
    af9b:	7d 01 7f    	tst	0x17f
    af9e:	27 0d       	beq	0x0xafad
    afa0:	4f          	clra
    afa1:	b7 01 7f    	staa	0x17f
    afa4:	b7 01 7e    	staa	0x17e
    afa7:	b7 01 1a    	staa	0x11a
    afaa:	7e ca 55    	jmp	0xca55
    afad:	7d 01 1a    	tst	0x11a
    afb0:	26 03       	bne	0x0xafb5
    afb2:	7e ca 55    	jmp	0xca55
    afb5:	96 4a       	ldaa	*0x4a
    afb7:	7d 01 1a    	tst	0x11a
    afba:	2b 25       	bmi	0x0xafe1
    afbc:	4c          	inca
    afbd:	81 04       	cmpa	#0x4
    afbf:	25 02       	bcs	0x0xafc3
    afc1:	86 03       	ldaa	#0x3
    afc3:	97 4a       	staa	*0x4a
    afc5:	c6 4a       	ldab	#0x4a
    afc7:	bd a8 5a    	jsr	0xa85a
    afca:	ce cf 9f    	ldx	#0xcf9f
    afcd:	d6 4a       	ldab	*0x4a
    afcf:	18 ce 01 30 	ldy	#0x130
    afd3:	bd df 5b    	jsr	0xdf5b
    afd6:	7f 01 1a    	clr	0x11a
    afd9:	86 04       	ldaa	#0x4
    afdb:	b7 01 1c    	staa	0x11c
    afde:	7e ca 55    	jmp	0xca55
    afe1:	4a          	deca
    afe2:	2a df       	bpl	0x0xafc3
    afe4:	4f          	clra
    afe5:	20 dc       	bra	0x0xafc3
    afe7:	7d 01 1c    	tst	0x11c
    afea:	2a 03       	bpl	0x0xafef
    afec:	7e cf af    	jmp	0xcfaf
    afef:	7d 01 1e    	tst	0x11e
    aff2:	26 08       	bne	0x0xaffc
    aff4:	7d 01 1c    	tst	0x11c
    aff7:	27 1f       	beq	0x0xb018
    aff9:	7e a9 6a    	jmp	0xa96a
    affc:	7d 01 1c    	tst	0x11c
    afff:	27 24       	beq	0x0xb025
    b001:	cc 01 20    	ldd	#0x120
    b004:	fb 01 1e    	addb	0x11e
    b007:	8f          	xgdx
    b008:	bd dd 4d    	jsr	0xdd4d
    b00b:	7a 01 1e    	dec	0x11e
    b00e:	7a 01 1c    	dec	0x11c
    b011:	26 12       	bne	0x0xb025
    b013:	bd dd cb    	jsr	0xddcb
    b016:	20 0d       	bra	0x0xb025
    b018:	7d 01 1e    	tst	0x11e
    b01b:	26 08       	bne	0x0xb025
    b01d:	86 13       	ldaa	#0x13
    b01f:	b7 01 1e    	staa	0x11e
    b022:	bd dd 89    	jsr	0xdd89
    b025:	7d 01 7f    	tst	0x17f
    b028:	27 10       	beq	0x0xb03a
    b02a:	bd df d4    	jsr	0xdfd4
    b02d:	4f          	clra
    b02e:	b7 01 7f    	staa	0x17f
    b031:	b7 01 7e    	staa	0x17e
    b034:	b7 01 1a    	staa	0x11a
    b037:	7e ca 55    	jmp	0xca55
    b03a:	7d 01 1a    	tst	0x11a
    b03d:	26 03       	bne	0x0xb042
    b03f:	7e ca 55    	jmp	0xca55
    b042:	b6 01 1e    	ldaa	0x11e
    b045:	81 13       	cmpa	#0x13
    b047:	26 3d       	bne	0x0xb086
    b049:	b6 01 1a    	ldaa	0x11a
    b04c:	2b 2a       	bmi	0x0xb078
    b04e:	96 6a       	ldaa	*0x6a
    b050:	84 20       	anda	#0x20
    b052:	d6 6a       	ldab	*0x6a
    b054:	c4 0f       	andb	#0xf
    b056:	5c          	incb
    b057:	c1 02       	cmpb	#0x2
    b059:	25 02       	bcs	0x0xb05d
    b05b:	c6 01       	ldab	#0x1
    b05d:	1b          	aba
    b05e:	16          	tab
    b05f:	d7 6a       	stab	*0x6a
    b061:	c4 0f       	andb	#0xf
    b063:	ce d0 1e    	ldx	#0xd01e
    b066:	18 ce 01 30 	ldy	#0x130
    b06a:	bd df 5b    	jsr	0xdf5b
    b06d:	7f 01 1a    	clr	0x11a
    b070:	86 04       	ldaa	#0x4
    b072:	b7 01 1c    	staa	0x11c
    b075:	7e ca 55    	jmp	0xca55
    b078:	96 6a       	ldaa	*0x6a
    b07a:	84 20       	anda	#0x20
    b07c:	d6 6a       	ldab	*0x6a
    b07e:	c4 0f       	andb	#0xf
    b080:	5a          	decb
    b081:	2a da       	bpl	0x0xb05d
    b083:	5f          	clrb
    b084:	20 d7       	bra	0x0xb05d
    b086:	81 19       	cmpa	#0x19
    b088:	26 2b       	bne	0x0xb0b5
    b08a:	7d 01 1a    	tst	0x11a
    b08d:	2b 20       	bmi	0x0xb0af
    b08f:	d6 95       	ldab	*0x95
    b091:	5c          	incb
    b092:	c1 03       	cmpb	#0x3
    b094:	25 02       	bcs	0x0xb098
    b096:	c6 02       	ldab	#0x2
    b098:	d7 95       	stab	*0x95
    b09a:	ce d0 32    	ldx	#0xd032
    b09d:	18 ce 01 36 	ldy	#0x136
    b0a1:	bd df 5b    	jsr	0xdf5b
    b0a4:	7f 01 1a    	clr	0x11a
    b0a7:	86 04       	ldaa	#0x4
    b0a9:	b7 01 1c    	staa	0x11c
    b0ac:	7e ca 55    	jmp	0xca55
    b0af:	d6 95       	ldab	*0x95
    b0b1:	5a          	decb
    b0b2:	2a e4       	bpl	0x0xb098
    b0b4:	5f          	clrb
    b0b5:	d6 69       	ldab	*0x69
    b0b7:	5c          	incb
    b0b8:	c4 01       	andb	#0x1
    b0ba:	d7 69       	stab	*0x69
    b0bc:	ce d0 3e    	ldx	#0xd03e
    b0bf:	18 ce 01 3b 	ldy	#0x13b
    b0c3:	bd df 5b    	jsr	0xdf5b
    b0c6:	7f 01 1a    	clr	0x11a
    b0c9:	86 04       	ldaa	#0x4
    b0cb:	b7 01 1c    	staa	0x11c
    b0ce:	96 69       	ldaa	*0x69
    b0d0:	c6 69       	ldab	#0x69
    b0d2:	bd a8 5a    	jsr	0xa85a
    b0d5:	7e ca 55    	jmp	0xca55
    b0d8:	7d 01 1c    	tst	0x11c
    b0db:	2a 03       	bpl	0x0xb0e0
    b0dd:	7e d0 46    	jmp	0xd046
    b0e0:	7d 01 1e    	tst	0x11e
    b0e3:	26 08       	bne	0x0xb0ed
    b0e5:	7d 01 1c    	tst	0x11c
    b0e8:	27 1f       	beq	0x0xb109
    b0ea:	7e a9 6a    	jmp	0xa96a
    b0ed:	7d 01 1c    	tst	0x11c
    b0f0:	27 24       	beq	0x0xb116
    b0f2:	cc 01 20    	ldd	#0x120
    b0f5:	fb 01 1e    	addb	0x11e
    b0f8:	8f          	xgdx
    b0f9:	bd dd 4d    	jsr	0xdd4d
    b0fc:	7a 01 1e    	dec	0x11e
    b0ff:	7a 01 1c    	dec	0x11c
    b102:	26 12       	bne	0x0xb116
    b104:	bd dd cb    	jsr	0xddcb
    b107:	20 0d       	bra	0x0xb116
    b109:	7d 01 1e    	tst	0x11e
    b10c:	26 08       	bne	0x0xb116
    b10e:	86 13       	ldaa	#0x13
    b110:	b7 01 1e    	staa	0x11e
    b113:	bd dd 89    	jsr	0xdd89
    b116:	7d 01 7f    	tst	0x17f
    b119:	27 10       	beq	0x0xb12b
    b11b:	bd df d4    	jsr	0xdfd4
    b11e:	4f          	clra
    b11f:	b7 01 7f    	staa	0x17f
    b122:	b7 01 7e    	staa	0x17e
    b125:	b7 01 1a    	staa	0x11a
    b128:	7e ca 55    	jmp	0xca55
    b12b:	7d 01 1a    	tst	0x11a
    b12e:	26 03       	bne	0x0xb133
    b130:	7e ca 55    	jmp	0xca55
    b133:	b6 01 1e    	ldaa	0x11e
    b136:	81 13       	cmpa	#0x13
    b138:	26 34       	bne	0x0xb16e
    b13a:	b6 01 1a    	ldaa	0x11a
    b13d:	2b 27       	bmi	0x0xb166
    b13f:	d6 6b       	ldab	*0x6b
    b141:	5c          	incb
    b142:	c1 03       	cmpb	#0x3
    b144:	25 02       	bcs	0x0xb148
    b146:	c6 02       	ldab	#0x2
    b148:	d7 6b       	stab	*0x6b
    b14a:	ce d0 98    	ldx	#0xd098
    b14d:	18 ce 01 30 	ldy	#0x130
    b151:	bd df 5b    	jsr	0xdf5b
    b154:	7f 01 1a    	clr	0x11a
    b157:	86 04       	ldaa	#0x4
    b159:	b7 01 1c    	staa	0x11c
    b15c:	96 6b       	ldaa	*0x6b
    b15e:	c6 6b       	ldab	#0x6b
    b160:	bd a8 5a    	jsr	0xa85a
    b163:	7e ca 55    	jmp	0xca55
    b166:	d6 6b       	ldab	*0x6b
    b168:	5a          	decb
    b169:	2a dd       	bpl	0x0xb148
    b16b:	5f          	clrb
    b16c:	20 da       	bra	0x0xb148
    b16e:	81 19       	cmpa	#0x19
    b170:	26 34       	bne	0x0xb1a6
    b172:	b6 01 1a    	ldaa	0x11a
    b175:	2b 27       	bmi	0x0xb19e
    b177:	d6 68       	ldab	*0x68
    b179:	5c          	incb
    b17a:	c1 07       	cmpb	#0x7
    b17c:	23 02       	bls	0x0xb180
    b17e:	c6 07       	ldab	#0x7
    b180:	d7 68       	stab	*0x68
    b182:	ce d0 a4    	ldx	#0xd0a4
    b185:	18 ce 01 36 	ldy	#0x136
    b189:	bd df 5b    	jsr	0xdf5b
    b18c:	7f 01 1a    	clr	0x11a
    b18f:	86 04       	ldaa	#0x4
    b191:	b7 01 1c    	staa	0x11c
    b194:	96 68       	ldaa	*0x68
    b196:	c6 68       	ldab	#0x68
    b198:	bd a8 5a    	jsr	0xa85a
    b19b:	7e ca 55    	jmp	0xca55
    b19e:	d6 68       	ldab	*0x68
    b1a0:	5a          	decb
    b1a1:	2a dd       	bpl	0x0xb180
    b1a3:	5f          	clrb
    b1a4:	20 da       	bra	0x0xb180
    b1a6:	7f 01 1a    	clr	0x11a
    b1a9:	7d 00 d0    	tst	0xd0
    b1ac:	27 05       	beq	0x0xb1b3
    b1ae:	7f 00 d0    	clr	0xd0
    b1b1:	20 08       	bra	0x0xb1bb
    b1b3:	7d 10 29    	tst	0x1029
    b1b6:	2a fb       	bpl	0x0xb1b3
    b1b8:	b6 10 2a    	ldaa	0x102a
    b1bb:	5f          	clrb
    b1bc:	f7 10 42    	stab	0x1042
    b1bf:	01          	nop
    b1c0:	86 fe       	ldaa	#0xfe
    b1c2:	b7 10 2a    	staa	0x102a
    b1c5:	bd dd ae    	jsr	0xddae
    b1c8:	c6 20       	ldab	#0x20
    b1ca:	ce 01 20    	ldx	#0x120
    b1cd:	18 ce b3 00 	ldy	#0xb300
    b1d1:	18 a6 00    	ldaa	0x0,y
    b1d4:	a7 00       	staa	0x0,x
    b1d6:	08          	inx
    b1d7:	18 08       	iny
    b1d9:	5a          	decb
    b1da:	26 f5       	bne	0x0xb1d1
    b1dc:	7f 01 1e    	clr	0x11e
    b1df:	86 20       	ldaa	#0x20
    b1e1:	b7 01 1c    	staa	0x11c
    b1e4:	ce 10 23    	ldx	#0x1023
    b1e7:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xb1e7
    b1eb:	bd dd 02    	jsr	0xdd02
    b1ee:	7d 01 1c    	tst	0x11c
    b1f1:	27 05       	beq	0x0xb1f8
    b1f3:	ce 10 23    	ldx	#0x1023
    b1f6:	20 ef       	bra	0x0xb1e7
    b1f8:	86 ff       	ldaa	#0xff
    b1fa:	97 f8       	staa	*0xf8
    b1fc:	c6 06       	ldab	#0x6
    b1fe:	ce ff ff    	ldx	#0xffff
    b201:	01          	nop
    b202:	01          	nop
    b203:	01          	nop
    b204:	01          	nop
    b205:	09          	dex
    b206:	26 f9       	bne	0x0xb201
    b208:	5a          	decb
    b209:	26 f6       	bne	0x0xb201
    b20b:	c6 06       	ldab	#0x6
    b20d:	44          	lsra
    b20e:	27 04       	beq	0x0xb214
    b210:	97 f8       	staa	*0xf8
    b212:	20 ed       	bra	0x0xb201
    b214:	97 fb       	staa	*0xfb
    b216:	97 f2       	staa	*0xf2
    b218:	97 ff       	staa	*0xff
    b21a:	b7 01 1b    	staa	0x11b
    b21d:	bd 9d 8a    	jsr	0x9d8a
    b220:	86 15       	ldaa	#0x15
    b222:	97 f8       	staa	*0xf8
    b224:	bd 9d 9c    	jsr	0x9d9c
    b227:	86 80       	ldaa	#0x80
    b229:	b7 01 1c    	staa	0x11c
    b22c:	7e 81 80    	jmp	0x8180
    b22f:	97 f8       	staa	*0xf8
    b231:	7d 10 29    	tst	0x1029
    b234:	2a f9       	bpl	0x0xb22f
    b236:	b6 10 2a    	ldaa	0x102a
    b239:	86 fe       	ldaa	#0xfe
    b23b:	b7 10 42    	staa	0x1042
    b23e:	86 ff       	ldaa	#0xff
    b240:	b7 10 2a    	staa	0x102a
    b243:	7d 10 29    	tst	0x1029
    b246:	2a fb       	bpl	0x0xb243
    b248:	b6 10 2a    	ldaa	0x102a
    b24b:	81 01       	cmpa	#0x1
    b24d:	26 07       	bne	0x0xb256
    b24f:	86 ff       	ldaa	#0xff
    b251:	b7 10 2a    	staa	0x102a
    b254:	20 ed       	bra	0x0xb243
    b256:	4d          	tsta
    b257:	2b 05       	bmi	0x0xb25e
    b259:	14 de 01    	bset	*0xde, #0x01
    b25c:	20 03       	bra	0x0xb261
    b25e:	15 de 01    	bclr	*0xde, #0x01
    b261:	86 fd       	ldaa	#0xfd
    b263:	b7 10 42    	staa	0x1042
    b266:	86 ff       	ldaa	#0xff
    b268:	b7 10 2a    	staa	0x102a
    b26b:	7d 10 29    	tst	0x1029
    b26e:	2a fb       	bpl	0x0xb26b
    b270:	b6 10 2a    	ldaa	0x102a
    b273:	81 01       	cmpa	#0x1
    b275:	26 07       	bne	0x0xb27e
    b277:	86 ff       	ldaa	#0xff
    b279:	b7 10 2a    	staa	0x102a
    b27c:	20 ed       	bra	0x0xb26b
    b27e:	4d          	tsta
    b27f:	2b 05       	bmi	0x0xb286
    b281:	14 de 02    	bset	*0xde, #0x02
    b284:	20 03       	bra	0x0xb289
    b286:	15 de 02    	bclr	*0xde, #0x02
    b289:	7c 00 d0    	inc	0xd0
    b28c:	bd dd ae    	jsr	0xddae
    b28f:	c6 20       	ldab	#0x20
    b291:	ce 01 20    	ldx	#0x120
    b294:	18 ce b3 20 	ldy	#0xb320
    b298:	18 a6 00    	ldaa	0x0,y
    b29b:	a7 00       	staa	0x0,x
    b29d:	08          	inx
    b29e:	18 08       	iny
    b2a0:	5a          	decb
    b2a1:	26 f5       	bne	0x0xb298
    b2a3:	ce 01 31    	ldx	#0x131
    b2a6:	18 ce 00 02 	ldy	#0x2
    b2aa:	d6 de       	ldab	*0xde
    b2ac:	86 47       	ldaa	#0x47
    b2ae:	54          	lsrb
    b2af:	25 02       	bcs	0x0xb2b3
    b2b1:	86 42       	ldaa	#0x42
    b2b3:	a7 00       	staa	0x0,x
    b2b5:	08          	inx
    b2b6:	08          	inx
    b2b7:	18 09       	dey
    b2b9:	26 f1       	bne	0x0xb2ac
    b2bb:	ce 10 23    	ldx	#0x1023
    b2be:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xb2be
    b2c2:	86 f7       	ldaa	#0xf7
    b2c4:	b4 10 00    	anda	0x1000
    b2c7:	b7 10 00    	staa	0x1000
    b2ca:	86 0c       	ldaa	#0xc
    b2cc:	b7 10 47    	staa	0x1047
    b2cf:	86 80       	ldaa	#0x80
    b2d1:	ba 10 00    	oraa	0x1000
    b2d4:	b7 10 00    	staa	0x1000
    b2d7:	01          	nop
    b2d8:	88 80       	eora	#0x80
    b2da:	b7 10 00    	staa	0x1000
    b2dd:	7f 01 1d    	clr	0x11d
    b2e0:	bd dd 7a    	jsr	0xdd7a
    b2e3:	7f 01 1e    	clr	0x11e
    b2e6:	86 20       	ldaa	#0x20
    b2e8:	b7 01 1c    	staa	0x11c
    b2eb:	ce 10 23    	ldx	#0x1023
    b2ee:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xb2ee
    b2f2:	bd dd 02    	jsr	0xdd02
    b2f5:	ce 10 23    	ldx	#0x1023
    b2f8:	7d 01 1c    	tst	0x11c
    b2fb:	26 f1       	bne	0x0xb2ee
    b2fd:	7e ca 55    	jmp	0xca55
    b300:	20 54       	bra	0x0xb356
    b302:	55          	.byte	0x55
    b303:	4e          	.byte	0x4e
    b304:	49          	rola
    b305:	4e          	.byte	0x4e
    b306:	47          	asra
    b307:	20 2e       	bra	0x0xb337
    b309:	2e 2e       	bgt	0x0xb339
    b30b:	2e 2e       	bgt	0x0xb33b
    b30d:	20 20       	bra	0x0xb32f
    b30f:	20 20       	bra	0x0xb331
    b311:	20 20       	bra	0x0xb333
    b313:	20 20       	bra	0x0xb335
    b315:	20 20       	bra	0x0xb337
    b317:	20 20       	bra	0x0xb339
    b319:	20 20       	bra	0x0xb33b
    b31b:	20 20       	bra	0x0xb33d
    b31d:	20 20       	bra	0x0xb33f
    b31f:	20 56       	bra	0x0xb377
    b321:	31          	ins
    b322:	20 32       	bra	0x0xb356
    b324:	20 33       	bra	0x0xb359
    b326:	20 34       	bra	0x0xb35c
    b328:	20 35       	bra	0x0xb35f
    b32a:	20 36       	bra	0x0xb362
    b32c:	20 37       	bra	0x0xb365
    b32e:	20 38       	bra	0x0xb368
    b330:	20 20       	bra	0x0xb352
    b332:	20 20       	bra	0x0xb354
    b334:	20 20       	bra	0x0xb356
    b336:	20 20       	bra	0x0xb358
    b338:	20 20       	bra	0x0xb35a
    b33a:	20 20       	bra	0x0xb35c
    b33c:	20 20       	bra	0x0xb35e
    b33e:	20 20       	bra	0x0xb360
    b340:	7d 01 1c    	tst	0x11c
    b343:	2a 03       	bpl	0x0xb348
    b345:	7e d0 c4    	jmp	0xd0c4
    b348:	7d 01 1e    	tst	0x11e
    b34b:	26 08       	bne	0x0xb355
    b34d:	7d 01 1c    	tst	0x11c
    b350:	27 1f       	beq	0x0xb371
    b352:	7e a9 6a    	jmp	0xa96a
    b355:	7d 01 1c    	tst	0x11c
    b358:	27 24       	beq	0x0xb37e
    b35a:	cc 01 20    	ldd	#0x120
    b35d:	fb 01 1e    	addb	0x11e
    b360:	8f          	xgdx
    b361:	bd dd 4d    	jsr	0xdd4d
    b364:	7a 01 1e    	dec	0x11e
    b367:	7a 01 1c    	dec	0x11c
    b36a:	26 12       	bne	0x0xb37e
    b36c:	bd dd fe    	jsr	0xddfe
    b36f:	20 0d       	bra	0x0xb37e
    b371:	7d 01 1e    	tst	0x11e
    b374:	26 08       	bne	0x0xb37e
    b376:	86 12       	ldaa	#0x12
    b378:	b7 01 1e    	staa	0x11e
    b37b:	bd dd 89    	jsr	0xdd89
    b37e:	7d 01 7f    	tst	0x17f
    b381:	27 10       	beq	0x0xb393
    b383:	bd e0 46    	jsr	0xe046
    b386:	4f          	clra
    b387:	b7 01 7f    	staa	0x17f
    b38a:	b7 01 7e    	staa	0x17e
    b38d:	b7 01 1a    	staa	0x11a
    b390:	7e ca 55    	jmp	0xca55
    b393:	7d 01 1a    	tst	0x11a
    b396:	26 03       	bne	0x0xb39b
    b398:	7e ca 55    	jmp	0xca55
    b39b:	b6 01 1e    	ldaa	0x11e
    b39e:	81 12       	cmpa	#0x12
    b3a0:	26 2a       	bne	0x0xb3cc
    b3a2:	d6 21       	ldab	*0x21
    b3a4:	c8 40       	eorb	#0x40
    b3a6:	d7 21       	stab	*0x21
    b3a8:	c4 40       	andb	#0x40
    b3aa:	54          	lsrb
    b3ab:	54          	lsrb
    b3ac:	54          	lsrb
    b3ad:	54          	lsrb
    b3ae:	54          	lsrb
    b3af:	54          	lsrb
    b3b0:	ce d1 5b    	ldx	#0xd15b
    b3b3:	18 ce 01 30 	ldy	#0x130
    b3b7:	bd df 47    	jsr	0xdf47
    b3ba:	7f 01 1a    	clr	0x11a
    b3bd:	86 03       	ldaa	#0x3
    b3bf:	b7 01 1c    	staa	0x11c
    b3c2:	96 21       	ldaa	*0x21
    b3c4:	c6 21       	ldab	#0x21
    b3c6:	bd a8 5a    	jsr	0xa85a
    b3c9:	7e ca 55    	jmp	0xca55
    b3cc:	81 16       	cmpa	#0x16
    b3ce:	26 17       	bne	0x0xb3e7
    b3d0:	d6 21       	ldab	*0x21
    b3d2:	c8 08       	eorb	#0x8
    b3d4:	d7 21       	stab	*0x21
    b3d6:	c4 08       	andb	#0x8
    b3d8:	54          	lsrb
    b3d9:	54          	lsrb
    b3da:	54          	lsrb
    b3db:	18 ce 01 34 	ldy	#0x134
    b3df:	ce d1 61    	ldx	#0xd161
    b3e2:	bd df 47    	jsr	0xdf47
    b3e5:	20 d3       	bra	0x0xb3ba
    b3e7:	81 1a       	cmpa	#0x1a
    b3e9:	26 14       	bne	0x0xb3ff
    b3eb:	d6 21       	ldab	*0x21
    b3ed:	c8 01       	eorb	#0x1
    b3ef:	d7 21       	stab	*0x21
    b3f1:	c4 01       	andb	#0x1
    b3f3:	18 ce 01 38 	ldy	#0x138
    b3f7:	ce d1 67    	ldx	#0xd167
    b3fa:	bd df 47    	jsr	0xdf47
    b3fd:	20 bb       	bra	0x0xb3ba
    b3ff:	7d 01 1a    	tst	0x11a
    b402:	2b 27       	bmi	0x0xb42b
    b404:	d6 24       	ldab	*0x24
    b406:	5c          	incb
    b407:	c1 07       	cmpb	#0x7
    b409:	25 02       	bcs	0x0xb40d
    b40b:	c6 06       	ldab	#0x6
    b40d:	d7 24       	stab	*0x24
    b40f:	ce d1 6d    	ldx	#0xd16d
    b412:	18 ce 01 3c 	ldy	#0x13c
    b416:	bd df 47    	jsr	0xdf47
    b419:	7f 01 1a    	clr	0x11a
    b41c:	86 03       	ldaa	#0x3
    b41e:	b7 01 1c    	staa	0x11c
    b421:	96 24       	ldaa	*0x24
    b423:	c6 24       	ldab	#0x24
    b425:	bd a8 5a    	jsr	0xa85a
    b428:	7e ca 55    	jmp	0xca55
    b42b:	d6 24       	ldab	*0x24
    b42d:	5a          	decb
    b42e:	2a dd       	bpl	0x0xb40d
    b430:	5f          	clrb
    b431:	20 da       	bra	0x0xb40d
    b433:	7d 01 1c    	tst	0x11c
    b436:	2a 03       	bpl	0x0xb43b
    b438:	7e d1 82    	jmp	0xd182
    b43b:	7d 01 1e    	tst	0x11e
    b43e:	26 08       	bne	0x0xb448
    b440:	7d 01 1c    	tst	0x11c
    b443:	27 1f       	beq	0x0xb464
    b445:	7e a9 6a    	jmp	0xa96a
    b448:	7d 01 1c    	tst	0x11c
    b44b:	27 24       	beq	0x0xb471
    b44d:	cc 01 20    	ldd	#0x120
    b450:	fb 01 1e    	addb	0x11e
    b453:	8f          	xgdx
    b454:	bd dd 4d    	jsr	0xdd4d
    b457:	7a 01 1e    	dec	0x11e
    b45a:	7a 01 1c    	dec	0x11c
    b45d:	26 12       	bne	0x0xb471
    b45f:	bd dd fe    	jsr	0xddfe
    b462:	20 0d       	bra	0x0xb471
    b464:	7d 01 1e    	tst	0x11e
    b467:	26 08       	bne	0x0xb471
    b469:	86 12       	ldaa	#0x12
    b46b:	b7 01 1e    	staa	0x11e
    b46e:	bd dd 89    	jsr	0xdd89
    b471:	7d 01 7f    	tst	0x17f
    b474:	27 10       	beq	0x0xb486
    b476:	bd e0 46    	jsr	0xe046
    b479:	4f          	clra
    b47a:	b7 01 7f    	staa	0x17f
    b47d:	b7 01 7e    	staa	0x17e
    b480:	b7 01 1a    	staa	0x11a
    b483:	7e ca 55    	jmp	0xca55
    b486:	7d 01 1a    	tst	0x11a
    b489:	26 03       	bne	0x0xb48e
    b48b:	7e ca 55    	jmp	0xca55
    b48e:	b6 01 1e    	ldaa	0x11e
    b491:	81 12       	cmpa	#0x12
    b493:	26 16       	bne	0x0xb4ab
    b495:	d6 21       	ldab	*0x21
    b497:	c8 02       	eorb	#0x2
    b499:	d7 21       	stab	*0x21
    b49b:	c4 02       	andb	#0x2
    b49d:	54          	lsrb
    b49e:	18 ce 01 30 	ldy	#0x130
    b4a2:	ce d1 5b    	ldx	#0xd15b
    b4a5:	bd df 47    	jsr	0xdf47
    b4a8:	7e b3 ba    	jmp	0xb3ba
    b4ab:	81 16       	cmpa	#0x16
    b4ad:	26 17       	bne	0x0xb4c6
    b4af:	d6 21       	ldab	*0x21
    b4b1:	c8 04       	eorb	#0x4
    b4b3:	d7 21       	stab	*0x21
    b4b5:	c4 04       	andb	#0x4
    b4b7:	54          	lsrb
    b4b8:	54          	lsrb
    b4b9:	18 ce 01 34 	ldy	#0x134
    b4bd:	ce d1 5b    	ldx	#0xd15b
    b4c0:	bd df 47    	jsr	0xdf47
    b4c3:	7e b3 ba    	jmp	0xb3ba
    b4c6:	81 1a       	cmpa	#0x1a
    b4c8:	26 30       	bne	0x0xb4fa
    b4ca:	b6 01 1a    	ldaa	0x11a
    b4cd:	2b 23       	bmi	0x0xb4f2
    b4cf:	96 22       	ldaa	*0x22
    b4d1:	4c          	inca
    b4d2:	81 7f       	cmpa	#0x7f
    b4d4:	25 02       	bcs	0x0xb4d8
    b4d6:	86 7f       	ldaa	#0x7f
    b4d8:	97 22       	staa	*0x22
    b4da:	ce 01 38    	ldx	#0x138
    b4dd:	bd de e4    	jsr	0xdee4
    b4e0:	7f 01 1a    	clr	0x11a
    b4e3:	86 03       	ldaa	#0x3
    b4e5:	b7 01 1c    	staa	0x11c
    b4e8:	96 22       	ldaa	*0x22
    b4ea:	c6 22       	ldab	#0x22
    b4ec:	bd a8 5a    	jsr	0xa85a
    b4ef:	7e ca 55    	jmp	0xca55
    b4f2:	96 22       	ldaa	*0x22
    b4f4:	4a          	deca
    b4f5:	2a e1       	bpl	0x0xb4d8
    b4f7:	4f          	clra
    b4f8:	20 de       	bra	0x0xb4d8
    b4fa:	b6 01 1a    	ldaa	0x11a
    b4fd:	2b 1f       	bmi	0x0xb51e
    b4ff:	96 23       	ldaa	*0x23
    b501:	4c          	inca
    b502:	84 7f       	anda	#0x7f
    b504:	97 23       	staa	*0x23
    b506:	ce 01 3c    	ldx	#0x13c
    b509:	bd de a8    	jsr	0xdea8
    b50c:	7f 01 1a    	clr	0x11a
    b50f:	86 03       	ldaa	#0x3
    b511:	b7 01 1c    	staa	0x11c
    b514:	96 23       	ldaa	*0x23
    b516:	c6 23       	ldab	#0x23
    b518:	bd a8 5a    	jsr	0xa85a
    b51b:	7e ca 55    	jmp	0xca55
    b51e:	96 23       	ldaa	*0x23
    b520:	4a          	deca
    b521:	84 7f       	anda	#0x7f
    b523:	20 df       	bra	0x0xb504
    b525:	7d 01 1c    	tst	0x11c
    b528:	2a 03       	bpl	0x0xb52d
    b52a:	7e d1 f8    	jmp	0xd1f8
    b52d:	7d 01 1e    	tst	0x11e
    b530:	26 08       	bne	0x0xb53a
    b532:	7d 01 1c    	tst	0x11c
    b535:	27 1f       	beq	0x0xb556
    b537:	7e a9 6a    	jmp	0xa96a
    b53a:	7d 01 1c    	tst	0x11c
    b53d:	27 24       	beq	0x0xb563
    b53f:	cc 01 20    	ldd	#0x120
    b542:	fb 01 1e    	addb	0x11e
    b545:	8f          	xgdx
    b546:	bd dd 4d    	jsr	0xdd4d
    b549:	7a 01 1e    	dec	0x11e
    b54c:	7a 01 1c    	dec	0x11c
    b54f:	26 12       	bne	0x0xb563
    b551:	bd dd cb    	jsr	0xddcb
    b554:	20 0d       	bra	0x0xb563
    b556:	7d 01 1e    	tst	0x11e
    b559:	26 08       	bne	0x0xb563
    b55b:	86 09       	ldaa	#0x9
    b55d:	b7 01 1e    	staa	0x11e
    b560:	bd dd 89    	jsr	0xdd89
    b563:	7d 01 7f    	tst	0x17f
    b566:	27 10       	beq	0x0xb578
    b568:	bd df 93    	jsr	0xdf93
    b56b:	4f          	clra
    b56c:	b7 01 7f    	staa	0x17f
    b56f:	b7 01 7e    	staa	0x17e
    b572:	b7 01 1a    	staa	0x11a
    b575:	7e ca 55    	jmp	0xca55
    b578:	7d 01 7e    	tst	0x17e
    b57b:	27 0c       	beq	0x0xb589
    b57d:	bd df 7c    	jsr	0xdf7c
    b580:	7f 01 7e    	clr	0x17e
    b583:	7f 01 1a    	clr	0x11a
    b586:	7e ca 55    	jmp	0xca55
    b589:	7d 01 1a    	tst	0x11a
    b58c:	26 03       	bne	0x0xb591
    b58e:	7e ca 55    	jmp	0xca55
    b591:	b6 01 1e    	ldaa	0x11e
    b594:	81 09       	cmpa	#0x9
    b596:	26 29       	bne	0x0xb5c1
    b598:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xb5a2
    b59c:	7f 01 1a    	clr	0x11a
    b59f:	7e ca 55    	jmp	0xca55
    b5a2:	ce 00 76    	ldx	#0x76
    b5a5:	bd b6 3a    	jsr	0xb63a
    b5a8:	bd b6 6e    	jsr	0xb66e
    b5ab:	16          	tab
    b5ac:	ce d6 20    	ldx	#0xd620
    b5af:	18 ce 01 26 	ldy	#0x126
    b5b3:	bd df 5b    	jsr	0xdf5b
    b5b6:	86 04       	ldaa	#0x4
    b5b8:	b7 01 1c    	staa	0x11c
    b5bb:	7f 01 1a    	clr	0x11a
    b5be:	7e ca 55    	jmp	0xca55
    b5c1:	81 0e       	cmpa	#0xe
    b5c3:	26 29       	bne	0x0xb5ee
    b5c5:	13 f6 10 06 	brclr	*0xf6, #0x10, 0x0xb5cf
    b5c9:	7f 01 1a    	clr	0x11a
    b5cc:	7e ca 55    	jmp	0xca55
    b5cf:	ce 00 77    	ldx	#0x77
    b5d2:	bd b6 3a    	jsr	0xb63a
    b5d5:	bd b6 6e    	jsr	0xb66e
    b5d8:	16          	tab
    b5d9:	ce d6 20    	ldx	#0xd620
    b5dc:	18 ce 01 2b 	ldy	#0x12b
    b5e0:	bd df 5b    	jsr	0xdf5b
    b5e3:	86 04       	ldaa	#0x4
    b5e5:	b7 01 1c    	staa	0x11c
    b5e8:	7f 01 1a    	clr	0x11a
    b5eb:	7e ca 55    	jmp	0xca55
    b5ee:	81 19       	cmpa	#0x19
    b5f0:	26 24       	bne	0x0xb616
    b5f2:	ce 00 78    	ldx	#0x78
    b5f5:	bd b6 3a    	jsr	0xb63a
    b5f8:	bd b6 ba    	jsr	0xb6ba
    b5fb:	ce 01 37    	ldx	#0x137
    b5fe:	7d 00 f6    	tst	0xf6
    b601:	2a 05       	bpl	0x0xb608
    b603:	bd de e4    	jsr	0xdee4
    b606:	20 03       	bra	0x0xb60b
    b608:	bd de a8    	jsr	0xdea8
    b60b:	86 03       	ldaa	#0x3
    b60d:	b7 01 1c    	staa	0x11c
    b610:	7f 01 1a    	clr	0x11a
    b613:	7e ca 55    	jmp	0xca55
    b616:	ce 00 79    	ldx	#0x79
    b619:	bd b6 3a    	jsr	0xb63a
    b61c:	bd b6 ba    	jsr	0xb6ba
    b61f:	ce 01 3c    	ldx	#0x13c
    b622:	7d 00 f6    	tst	0xf6
    b625:	2a 05       	bpl	0x0xb62c
    b627:	bd de e4    	jsr	0xdee4
    b62a:	20 03       	bra	0x0xb62f
    b62c:	bd de a8    	jsr	0xdea8
    b62f:	86 03       	ldaa	#0x3
    b631:	b7 01 1c    	staa	0x11c
    b634:	7f 01 1a    	clr	0x11a
    b637:	7e ca 55    	jmp	0xca55
    b63a:	96 f6       	ldaa	*0xf6
    b63c:	44          	lsra
    b63d:	44          	lsra
    b63e:	44          	lsra
    b63f:	24 03       	bcc	0x0xb644
    b641:	a6 00       	ldaa	0x0,x
    b643:	39          	rts
    b644:	44          	lsra
    b645:	24 06       	bcc	0x0xb64d
    b647:	c6 04       	ldab	#0x4
    b649:	3a          	abx
    b64a:	a6 00       	ldaa	0x0,x
    b64c:	39          	rts
    b64d:	44          	lsra
    b64e:	24 06       	bcc	0x0xb656
    b650:	c6 08       	ldab	#0x8
    b652:	3a          	abx
    b653:	a6 00       	ldaa	0x0,x
    b655:	39          	rts
    b656:	44          	lsra
    b657:	24 06       	bcc	0x0xb65f
    b659:	c6 0c       	ldab	#0xc
    b65b:	3a          	abx
    b65c:	a6 00       	ldaa	0x0,x
    b65e:	39          	rts
    b65f:	44          	lsra
    b660:	24 06       	bcc	0x0xb668
    b662:	c6 10       	ldab	#0x10
    b664:	3a          	abx
    b665:	a6 00       	ldaa	0x0,x
    b667:	39          	rts
    b668:	c6 14       	ldab	#0x14
    b66a:	3a          	abx
    b66b:	a6 00       	ldaa	0x0,x
    b66d:	39          	rts
    b66e:	7d 01 1a    	tst	0x11a
    b671:	2b 29       	bmi	0x0xb69c
    b673:	4c          	inca
    b674:	7d 00 f6    	tst	0xf6
    b677:	2a 14       	bpl	0x0xb68d
    b679:	7d 00 8f    	tst	0x8f
    b67c:	27 0f       	beq	0x0xb68d
    b67e:	81 03       	cmpa	#0x3
    b680:	22 04       	bhi	0x0xb686
    b682:	86 04       	ldaa	#0x4
    b684:	20 0d       	bra	0x0xb693
    b686:	81 09       	cmpa	#0x9
    b688:	26 03       	bne	0x0xb68d
    b68a:	4c          	inca
    b68b:	20 06       	bra	0x0xb693
    b68d:	81 19       	cmpa	#0x19
    b68f:	25 02       	bcs	0x0xb693
    b691:	86 18       	ldaa	#0x18
    b693:	a7 00       	staa	0x0,x
    b695:	8f          	xgdx
    b696:	37          	pshb
    b697:	8f          	xgdx
    b698:	33          	pulb
    b699:	7e a8 5a    	jmp	0xa85a
    b69c:	4a          	deca
    b69d:	2a 03       	bpl	0x0xb6a2
    b69f:	4f          	clra
    b6a0:	20 f1       	bra	0x0xb693
    b6a2:	7d 00 f6    	tst	0xf6
    b6a5:	2a ec       	bpl	0x0xb693
    b6a7:	7d 00 8f    	tst	0x8f
    b6aa:	27 e7       	beq	0x0xb693
    b6ac:	81 09       	cmpa	#0x9
    b6ae:	26 03       	bne	0x0xb6b3
    b6b0:	4a          	deca
    b6b1:	20 e0       	bra	0x0xb693
    b6b3:	81 03       	cmpa	#0x3
    b6b5:	22 dc       	bhi	0x0xb693
    b6b7:	4f          	clra
    b6b8:	20 d9       	bra	0x0xb693
    b6ba:	7d 01 1a    	tst	0x11a
    b6bd:	2b 12       	bmi	0x0xb6d1
    b6bf:	4c          	inca
    b6c0:	84 7f       	anda	#0x7f
    b6c2:	13 f6 10 02 	brclr	*0xf6, #0x10, 0x0xb6c8
    b6c6:	84 1f       	anda	#0x1f
    b6c8:	a7 00       	staa	0x0,x
    b6ca:	8f          	xgdx
    b6cb:	37          	pshb
    b6cc:	8f          	xgdx
    b6cd:	33          	pulb
    b6ce:	7e a8 5a    	jmp	0xa85a
    b6d1:	4a          	deca
    b6d2:	84 7f       	anda	#0x7f
    b6d4:	20 ec       	bra	0x0xb6c2
    b6d6:	7d 01 1c    	tst	0x11c
    b6d9:	2a 03       	bpl	0x0xb6de
    b6db:	7e d3 95    	jmp	0xd395
    b6de:	7d 01 1e    	tst	0x11e
    b6e1:	26 08       	bne	0x0xb6eb
    b6e3:	7d 01 1c    	tst	0x11c
    b6e6:	27 1f       	beq	0x0xb707
    b6e8:	7e a9 6a    	jmp	0xa96a
    b6eb:	7d 01 1c    	tst	0x11c
    b6ee:	27 24       	beq	0x0xb714
    b6f0:	cc 01 20    	ldd	#0x120
    b6f3:	fb 01 1e    	addb	0x11e
    b6f6:	8f          	xgdx
    b6f7:	bd dd 4d    	jsr	0xdd4d
    b6fa:	7a 01 1e    	dec	0x11e
    b6fd:	7a 01 1c    	dec	0x11c
    b700:	26 12       	bne	0x0xb714
    b702:	bd dd cb    	jsr	0xddcb
    b705:	20 0d       	bra	0x0xb714
    b707:	7d 01 1e    	tst	0x11e
    b70a:	26 08       	bne	0x0xb714
    b70c:	86 19       	ldaa	#0x19
    b70e:	b7 01 1e    	staa	0x11e
    b711:	bd dd 89    	jsr	0xdd89
    b714:	7d 01 7f    	tst	0x17f
    b717:	27 10       	beq	0x0xb729
    b719:	bd df 93    	jsr	0xdf93
    b71c:	4f          	clra
    b71d:	b7 01 7f    	staa	0x17f
    b720:	b7 01 7e    	staa	0x17e
    b723:	b7 01 1a    	staa	0x11a
    b726:	7e ca 55    	jmp	0xca55
    b729:	7d 01 1a    	tst	0x11a
    b72c:	26 03       	bne	0x0xb731
    b72e:	7e ca 55    	jmp	0xca55
    b731:	b6 01 1e    	ldaa	0x11e
    b734:	81 19       	cmpa	#0x19
    b736:	26 34       	bne	0x0xb76c
    b738:	18 ce 01 36 	ldy	#0x136
    b73c:	ce d3 f6    	ldx	#0xd3f6
    b73f:	7d 01 1a    	tst	0x11a
    b742:	2b 20       	bmi	0x0xb764
    b744:	d6 8e       	ldab	*0x8e
    b746:	5c          	incb
    b747:	c1 03       	cmpb	#0x3
    b749:	25 02       	bcs	0x0xb74d
    b74b:	c6 02       	ldab	#0x2
    b74d:	d7 8e       	stab	*0x8e
    b74f:	bd df 5b    	jsr	0xdf5b
    b752:	7f 01 1a    	clr	0x11a
    b755:	86 04       	ldaa	#0x4
    b757:	b7 01 1c    	staa	0x11c
    b75a:	96 8e       	ldaa	*0x8e
    b75c:	c6 8e       	ldab	#0x8e
    b75e:	bd a8 5a    	jsr	0xa85a
    b761:	7e ca 55    	jmp	0xca55
    b764:	d6 8e       	ldab	*0x8e
    b766:	5a          	decb
    b767:	2a e4       	bpl	0x0xb74d
    b769:	5f          	clrb
    b76a:	20 e1       	bra	0x0xb74d
    b76c:	18 ce 01 3b 	ldy	#0x13b
    b770:	ce d4 02    	ldx	#0xd402
    b773:	7d 01 1a    	tst	0x11a
    b776:	2b 20       	bmi	0x0xb798
    b778:	d6 8f       	ldab	*0x8f
    b77a:	5c          	incb
    b77b:	c1 02       	cmpb	#0x2
    b77d:	25 02       	bcs	0x0xb781
    b77f:	c6 01       	ldab	#0x1
    b781:	d7 8f       	stab	*0x8f
    b783:	bd df 5b    	jsr	0xdf5b
    b786:	7f 01 1a    	clr	0x11a
    b789:	86 04       	ldaa	#0x4
    b78b:	b7 01 1c    	staa	0x11c
    b78e:	96 8f       	ldaa	*0x8f
    b790:	c6 8f       	ldab	#0x8f
    b792:	bd a8 5a    	jsr	0xa85a
    b795:	7e ca 55    	jmp	0xca55
    b798:	d6 8f       	ldab	*0x8f
    b79a:	5a          	decb
    b79b:	2a e4       	bpl	0x0xb781
    b79d:	5f          	clrb
    b79e:	20 e1       	bra	0x0xb781
    b7a0:	7d 01 1c    	tst	0x11c
    b7a3:	2a 03       	bpl	0x0xb7a8
    b7a5:	7e d4 0a    	jmp	0xd40a
    b7a8:	7d 01 1e    	tst	0x11e
    b7ab:	26 08       	bne	0x0xb7b5
    b7ad:	7d 01 1c    	tst	0x11c
    b7b0:	27 1a       	beq	0x0xb7cc
    b7b2:	7e a9 6a    	jmp	0xa96a
    b7b5:	7d 01 1c    	tst	0x11c
    b7b8:	27 1a       	beq	0x0xb7d4
    b7ba:	cc 01 20    	ldd	#0x120
    b7bd:	fb 01 1e    	addb	0x11e
    b7c0:	8f          	xgdx
    b7c1:	bd dd 4d    	jsr	0xdd4d
    b7c4:	7a 01 1e    	dec	0x11e
    b7c7:	7a 01 1c    	dec	0x11c
    b7ca:	26 08       	bne	0x0xb7d4
    b7cc:	86 11       	ldaa	#0x11
    b7ce:	b7 01 1e    	staa	0x11e
    b7d1:	bd dd 89    	jsr	0xdd89
    b7d4:	4f          	clra
    b7d5:	b7 01 7f    	staa	0x17f
    b7d8:	b7 01 7e    	staa	0x17e
    b7db:	7d 01 1a    	tst	0x11a
    b7de:	26 03       	bne	0x0xb7e3
    b7e0:	7e ca 55    	jmp	0xca55
    b7e3:	2b 18       	bmi	0x0xb7fd
    b7e5:	b6 50 00    	ldaa	0x5000
    b7e8:	4c          	inca
    b7e9:	81 03       	cmpa	#0x3
    b7eb:	25 02       	bcs	0x0xb7ef
    b7ed:	86 02       	ldaa	#0x2
    b7ef:	b7 50 00    	staa	0x5000
    b7f2:	86 80       	ldaa	#0x80
    b7f4:	b7 01 1c    	staa	0x11c
    b7f7:	7f 01 1a    	clr	0x11a
    b7fa:	7e ca 55    	jmp	0xca55
    b7fd:	b6 50 00    	ldaa	0x5000
    b800:	84 03       	anda	#0x3
    b802:	4a          	deca
    b803:	2a ea       	bpl	0x0xb7ef
    b805:	4f          	clra
    b806:	20 e7       	bra	0x0xb7ef
    b808:	7d 01 1c    	tst	0x11c
    b80b:	2a 03       	bpl	0x0xb810
    b80d:	7e d4 90    	jmp	0xd490
    b810:	7d 01 1e    	tst	0x11e
    b813:	26 08       	bne	0x0xb81d
    b815:	7d 01 1c    	tst	0x11c
    b818:	27 1f       	beq	0x0xb839
    b81a:	7e a9 6a    	jmp	0xa96a
    b81d:	7d 01 1c    	tst	0x11c
    b820:	27 1f       	beq	0x0xb841
    b822:	cc 01 20    	ldd	#0x120
    b825:	fb 01 1e    	addb	0x11e
    b828:	8f          	xgdx
    b829:	bd dd 4d    	jsr	0xdd4d
    b82c:	7a 01 1e    	dec	0x11e
    b82f:	7a 01 1c    	dec	0x11c
    b832:	26 0d       	bne	0x0xb841
    b834:	bd dd fe    	jsr	0xddfe
    b837:	20 08       	bra	0x0xb841
    b839:	86 12       	ldaa	#0x12
    b83b:	b7 01 1e    	staa	0x11e
    b83e:	bd dd 89    	jsr	0xdd89
    b841:	7f 01 7e    	clr	0x17e
    b844:	7d 01 7f    	tst	0x17f
    b847:	27 10       	beq	0x0xb859
    b849:	bd e0 46    	jsr	0xe046
    b84c:	4f          	clra
    b84d:	b7 01 7f    	staa	0x17f
    b850:	b7 01 7e    	staa	0x17e
    b853:	b7 01 1a    	staa	0x11a
    b856:	7e ca 55    	jmp	0xca55
    b859:	7d 01 1a    	tst	0x11a
    b85c:	26 03       	bne	0x0xb861
    b85e:	7e ca 55    	jmp	0xca55
    b861:	b6 01 1e    	ldaa	0x11e
    b864:	81 12       	cmpa	#0x12
    b866:	26 2e       	bne	0x0xb896
    b868:	b6 50 08    	ldaa	0x5008
    b86b:	4c          	inca
    b86c:	84 01       	anda	#0x1
    b86e:	b7 50 08    	staa	0x5008
    b871:	8b 41       	adda	#0x41
    b873:	b7 01 32    	staa	0x132
    b876:	7f 01 1a    	clr	0x11a
    b879:	86 01       	ldaa	#0x1
    b87b:	b7 01 1c    	staa	0x11c
    b87e:	7f 01 6b    	clr	0x16b
    b881:	b6 50 08    	ldaa	0x5008
    b884:	97 f9       	staa	*0xf9
    b886:	bd a4 f9    	jsr	0xa4f9
    b889:	86 02       	ldaa	#0x2
    b88b:	97 f9       	staa	*0xf9
    b88d:	bd 9d 9c    	jsr	0x9d9c
    b890:	14 f5 10    	bset	*0xf5, #0x10
    b893:	7e ca 55    	jmp	0xca55
    b896:	81 16       	cmpa	#0x16
    b898:	26 33       	bne	0x0xb8cd
    b89a:	b6 50 01    	ldaa	0x5001
    b89d:	7d 01 1a    	tst	0x11a
    b8a0:	2b 26       	bmi	0x0xb8c8
    b8a2:	4c          	inca
    b8a3:	84 7f       	anda	#0x7f
    b8a5:	97 fa       	staa	*0xfa
    b8a7:	b7 50 01    	staa	0x5001
    b8aa:	4c          	inca
    b8ab:	ce 01 34    	ldx	#0x134
    b8ae:	bd de a8    	jsr	0xdea8
    b8b1:	7f 01 6b    	clr	0x16b
    b8b4:	b6 50 08    	ldaa	0x5008
    b8b7:	97 f9       	staa	*0xf9
    b8b9:	bd a4 f9    	jsr	0xa4f9
    b8bc:	86 02       	ldaa	#0x2
    b8be:	97 f9       	staa	*0xf9
    b8c0:	bd 9d 9c    	jsr	0x9d9c
    b8c3:	14 f5 10    	bset	*0xf5, #0x10
    b8c6:	20 45       	bra	0x0xb90d
    b8c8:	4a          	deca
    b8c9:	84 7f       	anda	#0x7f
    b8cb:	20 d8       	bra	0x0xb8a5
    b8cd:	81 1a       	cmpa	#0x1a
    b8cf:	26 23       	bne	0x0xb8f4
    b8d1:	b6 50 02    	ldaa	0x5002
    b8d4:	7d 01 1a    	tst	0x11a
    b8d7:	2b 12       	bmi	0x0xb8eb
    b8d9:	4c          	inca
    b8da:	81 45       	cmpa	#0x45
    b8dc:	25 02       	bcs	0x0xb8e0
    b8de:	86 44       	ldaa	#0x44
    b8e0:	b7 50 02    	staa	0x5002
    b8e3:	ce 01 38    	ldx	#0x138
    b8e6:	bd de e4    	jsr	0xdee4
    b8e9:	20 22       	bra	0x0xb90d
    b8eb:	4a          	deca
    b8ec:	81 3b       	cmpa	#0x3b
    b8ee:	22 f0       	bhi	0x0xb8e0
    b8f0:	86 3c       	ldaa	#0x3c
    b8f2:	20 ec       	bra	0x0xb8e0
    b8f4:	b6 50 03    	ldaa	0x5003
    b8f7:	7d 01 1a    	tst	0x11a
    b8fa:	2b 0e       	bmi	0x0xb90a
    b8fc:	4c          	inca
    b8fd:	84 7f       	anda	#0x7f
    b8ff:	b7 50 03    	staa	0x5003
    b902:	ce 01 3c    	ldx	#0x13c
    b905:	bd de a8    	jsr	0xdea8
    b908:	20 03       	bra	0x0xb90d
    b90a:	4a          	deca
    b90b:	20 f0       	bra	0x0xb8fd
    b90d:	7f 01 1a    	clr	0x11a
    b910:	86 03       	ldaa	#0x3
    b912:	b7 01 1c    	staa	0x11c
    b915:	7e ca 55    	jmp	0xca55
    b918:	7d 01 1c    	tst	0x11c
    b91b:	2a 03       	bpl	0x0xb920
    b91d:	7e d5 06    	jmp	0xd506
    b920:	7d 01 1e    	tst	0x11e
    b923:	26 08       	bne	0x0xb92d
    b925:	7d 01 1c    	tst	0x11c
    b928:	27 1f       	beq	0x0xb949
    b92a:	7e a9 6a    	jmp	0xa96a
    b92d:	7d 01 1c    	tst	0x11c
    b930:	27 1f       	beq	0x0xb951
    b932:	cc 01 20    	ldd	#0x120
    b935:	fb 01 1e    	addb	0x11e
    b938:	8f          	xgdx
    b939:	bd dd 4d    	jsr	0xdd4d
    b93c:	7a 01 1e    	dec	0x11e
    b93f:	7a 01 1c    	dec	0x11c
    b942:	26 0d       	bne	0x0xb951
    b944:	bd dd fe    	jsr	0xddfe
    b947:	20 08       	bra	0x0xb951
    b949:	86 12       	ldaa	#0x12
    b94b:	b7 01 1e    	staa	0x11e
    b94e:	bd dd 89    	jsr	0xdd89
    b951:	7f 01 7e    	clr	0x17e
    b954:	7d 01 7f    	tst	0x17f
    b957:	27 10       	beq	0x0xb969
    b959:	bd e0 46    	jsr	0xe046
    b95c:	4f          	clra
    b95d:	b7 01 7f    	staa	0x17f
    b960:	b7 01 7e    	staa	0x17e
    b963:	b7 01 1a    	staa	0x11a
    b966:	7e ca 55    	jmp	0xca55
    b969:	7d 01 1a    	tst	0x11a
    b96c:	26 03       	bne	0x0xb971
    b96e:	7e ca 55    	jmp	0xca55
    b971:	b6 01 1e    	ldaa	0x11e
    b974:	81 12       	cmpa	#0x12
    b976:	26 2e       	bne	0x0xb9a6
    b978:	b6 50 09    	ldaa	0x5009
    b97b:	4c          	inca
    b97c:	84 01       	anda	#0x1
    b97e:	b7 50 09    	staa	0x5009
    b981:	8b 41       	adda	#0x41
    b983:	b7 01 32    	staa	0x132
    b986:	7f 01 1a    	clr	0x11a
    b989:	86 01       	ldaa	#0x1
    b98b:	b7 01 1c    	staa	0x11c
    b98e:	7c 01 6b    	inc	0x16b
    b991:	b6 50 09    	ldaa	0x5009
    b994:	97 f9       	staa	*0xf9
    b996:	bd a4 f9    	jsr	0xa4f9
    b999:	86 02       	ldaa	#0x2
    b99b:	97 f9       	staa	*0xf9
    b99d:	bd 9d 9c    	jsr	0x9d9c
    b9a0:	14 f5 10    	bset	*0xf5, #0x10
    b9a3:	7e ca 55    	jmp	0xca55
    b9a6:	81 16       	cmpa	#0x16
    b9a8:	26 33       	bne	0x0xb9dd
    b9aa:	b6 50 04    	ldaa	0x5004
    b9ad:	7d 01 1a    	tst	0x11a
    b9b0:	2b 26       	bmi	0x0xb9d8
    b9b2:	4c          	inca
    b9b3:	84 7f       	anda	#0x7f
    b9b5:	b7 50 04    	staa	0x5004
    b9b8:	97 fa       	staa	*0xfa
    b9ba:	4c          	inca
    b9bb:	ce 01 34    	ldx	#0x134
    b9be:	bd de a8    	jsr	0xdea8
    b9c1:	7c 01 6b    	inc	0x16b
    b9c4:	b6 50 09    	ldaa	0x5009
    b9c7:	97 f9       	staa	*0xf9
    b9c9:	bd a4 f9    	jsr	0xa4f9
    b9cc:	86 02       	ldaa	#0x2
    b9ce:	97 f9       	staa	*0xf9
    b9d0:	bd 9d 9c    	jsr	0x9d9c
    b9d3:	14 f5 10    	bset	*0xf5, #0x10
    b9d6:	20 45       	bra	0x0xba1d
    b9d8:	4a          	deca
    b9d9:	84 7f       	anda	#0x7f
    b9db:	20 d8       	bra	0x0xb9b5
    b9dd:	81 1a       	cmpa	#0x1a
    b9df:	26 23       	bne	0x0xba04
    b9e1:	b6 50 05    	ldaa	0x5005
    b9e4:	7d 01 1a    	tst	0x11a
    b9e7:	2b 12       	bmi	0x0xb9fb
    b9e9:	4c          	inca
    b9ea:	81 45       	cmpa	#0x45
    b9ec:	25 02       	bcs	0x0xb9f0
    b9ee:	86 44       	ldaa	#0x44
    b9f0:	b7 50 05    	staa	0x5005
    b9f3:	ce 01 38    	ldx	#0x138
    b9f6:	bd de e4    	jsr	0xdee4
    b9f9:	20 22       	bra	0x0xba1d
    b9fb:	4a          	deca
    b9fc:	81 3b       	cmpa	#0x3b
    b9fe:	22 f0       	bhi	0x0xb9f0
    ba00:	86 3c       	ldaa	#0x3c
    ba02:	20 ec       	bra	0x0xb9f0
    ba04:	b6 50 06    	ldaa	0x5006
    ba07:	7d 01 1a    	tst	0x11a
    ba0a:	2b 0e       	bmi	0x0xba1a
    ba0c:	4c          	inca
    ba0d:	84 7f       	anda	#0x7f
    ba0f:	b7 50 06    	staa	0x5006
    ba12:	ce 01 3c    	ldx	#0x13c
    ba15:	bd de a8    	jsr	0xdea8
    ba18:	20 03       	bra	0x0xba1d
    ba1a:	4a          	deca
    ba1b:	20 f0       	bra	0x0xba0d
    ba1d:	7f 01 1a    	clr	0x11a
    ba20:	86 03       	ldaa	#0x3
    ba22:	b7 01 1c    	staa	0x11c
    ba25:	7e ca 55    	jmp	0xca55
    ba28:	7d 01 1c    	tst	0x11c
    ba2b:	2a 03       	bpl	0x0xba30
    ba2d:	7e d5 7c    	jmp	0xd57c
    ba30:	7d 01 1e    	tst	0x11e
    ba33:	26 08       	bne	0x0xba3d
    ba35:	7d 01 1c    	tst	0x11c
    ba38:	27 1a       	beq	0x0xba54
    ba3a:	7e a9 6a    	jmp	0xa96a
    ba3d:	7d 01 1c    	tst	0x11c
    ba40:	27 1a       	beq	0x0xba5c
    ba42:	cc 01 20    	ldd	#0x120
    ba45:	fb 01 1e    	addb	0x11e
    ba48:	8f          	xgdx
    ba49:	bd dd 4d    	jsr	0xdd4d
    ba4c:	7a 01 1e    	dec	0x11e
    ba4f:	7a 01 1c    	dec	0x11c
    ba52:	26 08       	bne	0x0xba5c
    ba54:	86 1e       	ldaa	#0x1e
    ba56:	b7 01 1e    	staa	0x11e
    ba59:	bd dd 89    	jsr	0xdd89
    ba5c:	7f 01 7e    	clr	0x17e
    ba5f:	7f 01 7f    	clr	0x17f
    ba62:	7d 01 1a    	tst	0x11a
    ba65:	26 03       	bne	0x0xba6a
    ba67:	7e ca 55    	jmp	0xca55
    ba6a:	b6 50 07    	ldaa	0x5007
    ba6d:	7d 01 1a    	tst	0x11a
    ba70:	2b 0f       	bmi	0x0xba81
    ba72:	4c          	inca
    ba73:	84 7f       	anda	#0x7f
    ba75:	b7 50 07    	staa	0x5007
    ba78:	ce 01 3c    	ldx	#0x13c
    ba7b:	bd de a8    	jsr	0xdea8
    ba7e:	7e ba 1d    	jmp	0xba1d
    ba81:	4a          	deca
    ba82:	20 ef       	bra	0x0xba73
    ba84:	7e ca 55    	jmp	0xca55
    ba87:	7d 01 1c    	tst	0x11c
    ba8a:	2a 03       	bpl	0x0xba8f
    ba8c:	7e d5 bf    	jmp	0xd5bf
    ba8f:	7d 01 1e    	tst	0x11e
    ba92:	26 08       	bne	0x0xba9c
    ba94:	7d 01 1c    	tst	0x11c
    ba97:	27 1f       	beq	0x0xbab8
    ba99:	7e a9 6a    	jmp	0xa96a
    ba9c:	7d 01 1c    	tst	0x11c
    ba9f:	27 24       	beq	0x0xbac5
    baa1:	cc 01 20    	ldd	#0x120
    baa4:	fb 01 1e    	addb	0x11e
    baa7:	8f          	xgdx
    baa8:	bd dd 4d    	jsr	0xdd4d
    baab:	7a 01 1e    	dec	0x11e
    baae:	7a 01 1c    	dec	0x11c
    bab1:	26 12       	bne	0x0xbac5
    bab3:	bd dd cb    	jsr	0xddcb
    bab6:	20 0d       	bra	0x0xbac5
    bab8:	7d 01 1e    	tst	0x11e
    babb:	26 08       	bne	0x0xbac5
    babd:	86 03       	ldaa	#0x3
    babf:	b7 01 1e    	staa	0x11e
    bac2:	bd dd 89    	jsr	0xdd89
    bac5:	7d 01 7f    	tst	0x17f
    bac8:	27 10       	beq	0x0xbada
    baca:	bd df d4    	jsr	0xdfd4
    bacd:	4f          	clra
    bace:	b7 01 7f    	staa	0x17f
    bad1:	b7 01 7e    	staa	0x17e
    bad4:	b7 01 1a    	staa	0x11a
    bad7:	7e ca 55    	jmp	0xca55
    bada:	7d 01 7e    	tst	0x17e
    badd:	27 0c       	beq	0x0xbaeb
    badf:	bd df 7c    	jsr	0xdf7c
    bae2:	7f 01 7e    	clr	0x17e
    bae5:	7f 01 1a    	clr	0x11a
    bae8:	7e ca 55    	jmp	0xca55
    baeb:	7d 01 1a    	tst	0x11a
    baee:	26 03       	bne	0x0xbaf3
    baf0:	7e ca 55    	jmp	0xca55
    baf3:	ce d6 20    	ldx	#0xd620
    baf6:	b6 01 1e    	ldaa	0x11e
    baf9:	81 03       	cmpa	#0x3
    bafb:	26 1a       	bne	0x0xbb17
    bafd:	18 ce 01 20 	ldy	#0x120
    bb01:	d6 60       	ldab	*0x60
    bb03:	bd bb e5    	jsr	0xbbe5
    bb06:	d7 60       	stab	*0x60
    bb08:	bd df 5b    	jsr	0xdf5b
    bb0b:	96 60       	ldaa	*0x60
    bb0d:	c6 60       	ldab	#0x60
    bb0f:	bd a8 5a    	jsr	0xa85a
    bb12:	86 04       	ldaa	#0x4
    bb14:	7e bb dc    	jmp	0xbbdc
    bb17:	81 09       	cmpa	#0x9
    bb19:	26 1a       	bne	0x0xbb35
    bb1b:	18 ce 01 26 	ldy	#0x126
    bb1f:	d6 61       	ldab	*0x61
    bb21:	bd bb e5    	jsr	0xbbe5
    bb24:	d7 61       	stab	*0x61
    bb26:	bd df 5b    	jsr	0xdf5b
    bb29:	96 61       	ldaa	*0x61
    bb2b:	c6 61       	ldab	#0x61
    bb2d:	bd a8 5a    	jsr	0xa85a
    bb30:	86 04       	ldaa	#0x4
    bb32:	7e bb dc    	jmp	0xbbdc
    bb35:	81 0e       	cmpa	#0xe
    bb37:	26 1a       	bne	0x0xbb53
    bb39:	18 ce 01 2b 	ldy	#0x12b
    bb3d:	d6 62       	ldab	*0x62
    bb3f:	bd bb e5    	jsr	0xbbe5
    bb42:	d7 62       	stab	*0x62
    bb44:	bd df 5b    	jsr	0xdf5b
    bb47:	96 62       	ldaa	*0x62
    bb49:	c6 62       	ldab	#0x62
    bb4b:	bd a8 5a    	jsr	0xa85a
    bb4e:	86 04       	ldaa	#0x4
    bb50:	7e bb dc    	jmp	0xbbdc
    bb53:	81 13       	cmpa	#0x13
    bb55:	26 2b       	bne	0x0xbb82
    bb57:	b6 01 1a    	ldaa	0x11a
    bb5a:	2b 1f       	bmi	0x0xbb7b
    bb5c:	96 63       	ldaa	*0x63
    bb5e:	4c          	inca
    bb5f:	84 7f       	anda	#0x7f
    bb61:	97 63       	staa	*0x63
    bb63:	ce 01 31    	ldx	#0x131
    bb66:	bd de a8    	jsr	0xdea8
    bb69:	7f 01 1a    	clr	0x11a
    bb6c:	86 03       	ldaa	#0x3
    bb6e:	b7 01 1c    	staa	0x11c
    bb71:	96 63       	ldaa	*0x63
    bb73:	c6 63       	ldab	#0x63
    bb75:	bd a8 5a    	jsr	0xa85a
    bb78:	7e ca 55    	jmp	0xca55
    bb7b:	96 63       	ldaa	*0x63
    bb7d:	4a          	deca
    bb7e:	84 7f       	anda	#0x7f
    bb80:	20 df       	bra	0x0xbb61
    bb82:	81 19       	cmpa	#0x19
    bb84:	26 2b       	bne	0x0xbbb1
    bb86:	b6 01 1a    	ldaa	0x11a
    bb89:	2b 1f       	bmi	0x0xbbaa
    bb8b:	96 64       	ldaa	*0x64
    bb8d:	4c          	inca
    bb8e:	84 7f       	anda	#0x7f
    bb90:	97 64       	staa	*0x64
    bb92:	ce 01 37    	ldx	#0x137
    bb95:	bd de a8    	jsr	0xdea8
    bb98:	7f 01 1a    	clr	0x11a
    bb9b:	86 03       	ldaa	#0x3
    bb9d:	b7 01 1c    	staa	0x11c
    bba0:	96 64       	ldaa	*0x64
    bba2:	c6 64       	ldab	#0x64
    bba4:	bd a8 5a    	jsr	0xa85a
    bba7:	7e ca 55    	jmp	0xca55
    bbaa:	96 64       	ldaa	*0x64
    bbac:	4a          	deca
    bbad:	84 7f       	anda	#0x7f
    bbaf:	20 df       	bra	0x0xbb90
    bbb1:	7d 01 1a    	tst	0x11a
    bbb4:	2b 1f       	bmi	0x0xbbd5
    bbb6:	96 65       	ldaa	*0x65
    bbb8:	4c          	inca
    bbb9:	84 7f       	anda	#0x7f
    bbbb:	97 65       	staa	*0x65
    bbbd:	ce 01 3c    	ldx	#0x13c
    bbc0:	bd de a8    	jsr	0xdea8
    bbc3:	7f 01 1a    	clr	0x11a
    bbc6:	86 03       	ldaa	#0x3
    bbc8:	b7 01 1c    	staa	0x11c
    bbcb:	96 65       	ldaa	*0x65
    bbcd:	c6 65       	ldab	#0x65
    bbcf:	bd a8 5a    	jsr	0xa85a
    bbd2:	7e ca 55    	jmp	0xca55
    bbd5:	96 65       	ldaa	*0x65
    bbd7:	4a          	deca
    bbd8:	84 7f       	anda	#0x7f
    bbda:	20 df       	bra	0x0xbbbb
    bbdc:	b7 01 1c    	staa	0x11c
    bbdf:	7f 01 1a    	clr	0x11a
    bbe2:	7e ca 55    	jmp	0xca55
    bbe5:	7d 01 1a    	tst	0x11a
    bbe8:	2b 15       	bmi	0x0xbbff
    bbea:	5c          	incb
    bbeb:	c1 09       	cmpb	#0x9
    bbed:	26 02       	bne	0x0xbbf1
    bbef:	5c          	incb
    bbf0:	39          	rts
    bbf1:	c1 0d       	cmpb	#0xd
    bbf3:	26 03       	bne	0x0xbbf8
    bbf5:	5c          	incb
    bbf6:	5c          	incb
    bbf7:	39          	rts
    bbf8:	c1 16       	cmpb	#0x16
    bbfa:	25 02       	bcs	0x0xbbfe
    bbfc:	c6 15       	ldab	#0x15
    bbfe:	39          	rts
    bbff:	5a          	decb
    bc00:	2a 02       	bpl	0x0xbc04
    bc02:	5f          	clrb
    bc03:	39          	rts
    bc04:	c1 0e       	cmpb	#0xe
    bc06:	26 03       	bne	0x0xbc0b
    bc08:	5a          	decb
    bc09:	5a          	decb
    bc0a:	39          	rts
    bc0b:	c1 09       	cmpb	#0x9
    bc0d:	26 ef       	bne	0x0xbbfe
    bc0f:	5a          	decb
    bc10:	39          	rts
    bc11:	7d 01 1c    	tst	0x11c
    bc14:	2a 03       	bpl	0x0xbc19
    bc16:	7e d6 84    	jmp	0xd684
    bc19:	7d 01 1e    	tst	0x11e
    bc1c:	26 08       	bne	0x0xbc26
    bc1e:	7d 01 1c    	tst	0x11c
    bc21:	27 1f       	beq	0x0xbc42
    bc23:	7e a9 6a    	jmp	0xa96a
    bc26:	7d 01 1c    	tst	0x11c
    bc29:	27 24       	beq	0x0xbc4f
    bc2b:	cc 01 20    	ldd	#0x120
    bc2e:	fb 01 1e    	addb	0x11e
    bc31:	8f          	xgdx
    bc32:	bd dd 4d    	jsr	0xdd4d
    bc35:	7a 01 1e    	dec	0x11e
    bc38:	7a 01 1c    	dec	0x11c
    bc3b:	26 12       	bne	0x0xbc4f
    bc3d:	bd dd cb    	jsr	0xddcb
    bc40:	20 0d       	bra	0x0xbc4f
    bc42:	7d 01 1e    	tst	0x11e
    bc45:	26 08       	bne	0x0xbc4f
    bc47:	86 13       	ldaa	#0x13
    bc49:	b7 01 1e    	staa	0x11e
    bc4c:	bd dd 89    	jsr	0xdd89
    bc4f:	7d 01 7f    	tst	0x17f
    bc52:	27 10       	beq	0x0xbc64
    bc54:	bd df d4    	jsr	0xdfd4
    bc57:	4f          	clra
    bc58:	b7 01 7f    	staa	0x17f
    bc5b:	b7 01 7e    	staa	0x17e
    bc5e:	b7 01 1a    	staa	0x11a
    bc61:	7e ca 55    	jmp	0xca55
    bc64:	7d 01 1a    	tst	0x11a
    bc67:	26 03       	bne	0x0xbc6c
    bc69:	7e ca 55    	jmp	0xca55
    bc6c:	b6 01 1e    	ldaa	0x11e
    bc6f:	81 13       	cmpa	#0x13
    bc71:	26 2b       	bne	0x0xbc9e
    bc73:	b6 01 1a    	ldaa	0x11a
    bc76:	2b 1f       	bmi	0x0xbc97
    bc78:	96 54       	ldaa	*0x54
    bc7a:	4c          	inca
    bc7b:	84 7f       	anda	#0x7f
    bc7d:	97 54       	staa	*0x54
    bc7f:	ce 01 31    	ldx	#0x131
    bc82:	bd de a8    	jsr	0xdea8
    bc85:	7f 01 1a    	clr	0x11a
    bc88:	86 03       	ldaa	#0x3
    bc8a:	b7 01 1c    	staa	0x11c
    bc8d:	96 54       	ldaa	*0x54
    bc8f:	c6 54       	ldab	#0x54
    bc91:	bd a8 5a    	jsr	0xa85a
    bc94:	7e ca 55    	jmp	0xca55
    bc97:	96 54       	ldaa	*0x54
    bc99:	4a          	deca
    bc9a:	84 7f       	anda	#0x7f
    bc9c:	20 df       	bra	0x0xbc7d
    bc9e:	81 19       	cmpa	#0x19
    bca0:	26 2b       	bne	0x0xbccd
    bca2:	b6 01 1a    	ldaa	0x11a
    bca5:	2b 1f       	bmi	0x0xbcc6
    bca7:	96 59       	ldaa	*0x59
    bca9:	4c          	inca
    bcaa:	84 7f       	anda	#0x7f
    bcac:	97 59       	staa	*0x59
    bcae:	ce 01 37    	ldx	#0x137
    bcb1:	bd de a8    	jsr	0xdea8
    bcb4:	7f 01 1a    	clr	0x11a
    bcb7:	86 03       	ldaa	#0x3
    bcb9:	b7 01 1c    	staa	0x11c
    bcbc:	96 59       	ldaa	*0x59
    bcbe:	c6 59       	ldab	#0x59
    bcc0:	bd a8 5a    	jsr	0xa85a
    bcc3:	7e ca 55    	jmp	0xca55
    bcc6:	96 59       	ldaa	*0x59
    bcc8:	4a          	deca
    bcc9:	84 7f       	anda	#0x7f
    bccb:	20 df       	bra	0x0xbcac
    bccd:	b6 01 1a    	ldaa	0x11a
    bcd0:	2b 1f       	bmi	0x0xbcf1
    bcd2:	96 5e       	ldaa	*0x5e
    bcd4:	4c          	inca
    bcd5:	84 7f       	anda	#0x7f
    bcd7:	97 5e       	staa	*0x5e
    bcd9:	ce 01 3c    	ldx	#0x13c
    bcdc:	bd de a8    	jsr	0xdea8
    bcdf:	7f 01 1a    	clr	0x11a
    bce2:	86 03       	ldaa	#0x3
    bce4:	b7 01 1c    	staa	0x11c
    bce7:	96 5e       	ldaa	*0x5e
    bce9:	c6 5e       	ldab	#0x5e
    bceb:	bd a8 5a    	jsr	0xa85a
    bcee:	7e ca 55    	jmp	0xca55
    bcf1:	96 5e       	ldaa	*0x5e
    bcf3:	4a          	deca
    bcf4:	84 7f       	anda	#0x7f
    bcf6:	20 df       	bra	0x0xbcd7
    bcf8:	7d 01 1c    	tst	0x11c
    bcfb:	2a 03       	bpl	0x0xbd00
    bcfd:	7e d6 d7    	jmp	0xd6d7
    bd00:	7d 01 1e    	tst	0x11e
    bd03:	26 08       	bne	0x0xbd0d
    bd05:	7d 01 1c    	tst	0x11c
    bd08:	27 1f       	beq	0x0xbd29
    bd0a:	7e a9 6a    	jmp	0xa96a
    bd0d:	7d 01 1c    	tst	0x11c
    bd10:	27 24       	beq	0x0xbd36
    bd12:	cc 01 20    	ldd	#0x120
    bd15:	fb 01 1e    	addb	0x11e
    bd18:	8f          	xgdx
    bd19:	bd dd 4d    	jsr	0xdd4d
    bd1c:	7a 01 1e    	dec	0x11e
    bd1f:	7a 01 1c    	dec	0x11c
    bd22:	26 12       	bne	0x0xbd36
    bd24:	bd dd cb    	jsr	0xddcb
    bd27:	20 0d       	bra	0x0xbd36
    bd29:	7d 01 1e    	tst	0x11e
    bd2c:	26 08       	bne	0x0xbd36
    bd2e:	86 13       	ldaa	#0x13
    bd30:	b7 01 1e    	staa	0x11e
    bd33:	bd dd 89    	jsr	0xdd89
    bd36:	7d 01 7f    	tst	0x17f
    bd39:	27 10       	beq	0x0xbd4b
    bd3b:	bd df d4    	jsr	0xdfd4
    bd3e:	4f          	clra
    bd3f:	b7 01 7f    	staa	0x17f
    bd42:	b7 01 7e    	staa	0x17e
    bd45:	b7 01 1a    	staa	0x11a
    bd48:	7e ca 55    	jmp	0xca55
    bd4b:	7d 01 1a    	tst	0x11a
    bd4e:	26 03       	bne	0x0xbd53
    bd50:	7e ca 55    	jmp	0xca55
    bd53:	b6 01 1e    	ldaa	0x11e
    bd56:	81 13       	cmpa	#0x13
    bd58:	26 2b       	bne	0x0xbd85
    bd5a:	b6 01 1a    	ldaa	0x11a
    bd5d:	2b 1f       	bmi	0x0xbd7e
    bd5f:	96 90       	ldaa	*0x90
    bd61:	4c          	inca
    bd62:	84 7f       	anda	#0x7f
    bd64:	97 90       	staa	*0x90
    bd66:	ce 01 31    	ldx	#0x131
    bd69:	bd de a8    	jsr	0xdea8
    bd6c:	7f 01 1a    	clr	0x11a
    bd6f:	86 03       	ldaa	#0x3
    bd71:	b7 01 1c    	staa	0x11c
    bd74:	96 90       	ldaa	*0x90
    bd76:	c6 90       	ldab	#0x90
    bd78:	bd a8 5a    	jsr	0xa85a
    bd7b:	7e ca 55    	jmp	0xca55
    bd7e:	96 90       	ldaa	*0x90
    bd80:	4a          	deca
    bd81:	84 7f       	anda	#0x7f
    bd83:	20 df       	bra	0x0xbd64
    bd85:	81 19       	cmpa	#0x19
    bd87:	26 2b       	bne	0x0xbdb4
    bd89:	b6 01 1a    	ldaa	0x11a
    bd8c:	2b 1f       	bmi	0x0xbdad
    bd8e:	96 91       	ldaa	*0x91
    bd90:	4c          	inca
    bd91:	84 7f       	anda	#0x7f
    bd93:	97 91       	staa	*0x91
    bd95:	ce 01 37    	ldx	#0x137
    bd98:	bd de a8    	jsr	0xdea8
    bd9b:	7f 01 1a    	clr	0x11a
    bd9e:	86 03       	ldaa	#0x3
    bda0:	b7 01 1c    	staa	0x11c
    bda3:	96 91       	ldaa	*0x91
    bda5:	c6 91       	ldab	#0x91
    bda7:	bd a8 5a    	jsr	0xa85a
    bdaa:	7e ca 55    	jmp	0xca55
    bdad:	96 91       	ldaa	*0x91
    bdaf:	4a          	deca
    bdb0:	84 7f       	anda	#0x7f
    bdb2:	20 df       	bra	0x0xbd93
    bdb4:	b6 01 1a    	ldaa	0x11a
    bdb7:	2b 1f       	bmi	0x0xbdd8
    bdb9:	96 92       	ldaa	*0x92
    bdbb:	4c          	inca
    bdbc:	84 7f       	anda	#0x7f
    bdbe:	97 92       	staa	*0x92
    bdc0:	ce 01 3c    	ldx	#0x13c
    bdc3:	bd de a8    	jsr	0xdea8
    bdc6:	7f 01 1a    	clr	0x11a
    bdc9:	86 03       	ldaa	#0x3
    bdcb:	b7 01 1c    	staa	0x11c
    bdce:	96 92       	ldaa	*0x92
    bdd0:	c6 92       	ldab	#0x92
    bdd2:	bd a8 5a    	jsr	0xa85a
    bdd5:	7e ca 55    	jmp	0xca55
    bdd8:	96 92       	ldaa	*0x92
    bdda:	4a          	deca
    bddb:	84 7f       	anda	#0x7f
    bddd:	20 df       	bra	0x0xbdbe
    bddf:	7d 01 1c    	tst	0x11c
    bde2:	2a 03       	bpl	0x0xbde7
    bde4:	7e d7 29    	jmp	0xd729
    bde7:	7d 01 1e    	tst	0x11e
    bdea:	26 08       	bne	0x0xbdf4
    bdec:	7d 01 1c    	tst	0x11c
    bdef:	27 1f       	beq	0x0xbe10
    bdf1:	7e a9 6a    	jmp	0xa96a
    bdf4:	7d 01 1c    	tst	0x11c
    bdf7:	27 24       	beq	0x0xbe1d
    bdf9:	cc 01 20    	ldd	#0x120
    bdfc:	fb 01 1e    	addb	0x11e
    bdff:	8f          	xgdx
    be00:	bd dd 4d    	jsr	0xdd4d
    be03:	7a 01 1e    	dec	0x11e
    be06:	7a 01 1c    	dec	0x11c
    be09:	26 12       	bne	0x0xbe1d
    be0b:	bd dd cb    	jsr	0xddcb
    be0e:	20 0d       	bra	0x0xbe1d
    be10:	7d 01 1e    	tst	0x11e
    be13:	26 08       	bne	0x0xbe1d
    be15:	86 13       	ldaa	#0x13
    be17:	b7 01 1e    	staa	0x11e
    be1a:	bd dd 89    	jsr	0xdd89
    be1d:	7d 01 7f    	tst	0x17f
    be20:	27 1e       	beq	0x0xbe40
    be22:	2a 10       	bpl	0x0xbe34
    be24:	bd df d4    	jsr	0xdfd4
    be27:	4f          	clra
    be28:	b7 01 7f    	staa	0x17f
    be2b:	b7 01 7e    	staa	0x17e
    be2e:	b7 01 1a    	staa	0x11a
    be31:	7e ca 55    	jmp	0xca55
    be34:	b6 01 1e    	ldaa	0x11e
    be37:	81 19       	cmpa	#0x19
    be39:	27 ec       	beq	0x0xbe27
    be3b:	bd df d4    	jsr	0xdfd4
    be3e:	20 e7       	bra	0x0xbe27
    be40:	7d 01 1a    	tst	0x11a
    be43:	26 03       	bne	0x0xbe48
    be45:	7e ca 55    	jmp	0xca55
    be48:	b6 01 1e    	ldaa	0x11e
    be4b:	81 13       	cmpa	#0x13
    be4d:	26 2b       	bne	0x0xbe7a
    be4f:	b6 01 1a    	ldaa	0x11a
    be52:	2b 1f       	bmi	0x0xbe73
    be54:	96 50       	ldaa	*0x50
    be56:	4c          	inca
    be57:	84 7f       	anda	#0x7f
    be59:	97 50       	staa	*0x50
    be5b:	ce 01 31    	ldx	#0x131
    be5e:	bd de a8    	jsr	0xdea8
    be61:	7f 01 1a    	clr	0x11a
    be64:	86 03       	ldaa	#0x3
    be66:	b7 01 1c    	staa	0x11c
    be69:	96 50       	ldaa	*0x50
    be6b:	c6 50       	ldab	#0x50
    be6d:	bd a8 5a    	jsr	0xa85a
    be70:	7e ca 55    	jmp	0xca55
    be73:	96 50       	ldaa	*0x50
    be75:	4a          	deca
    be76:	84 7f       	anda	#0x7f
    be78:	20 df       	bra	0x0xbe59
    be7a:	81 19       	cmpa	#0x19
    be7c:	26 2b       	bne	0x0xbea9
    be7e:	b6 01 1a    	ldaa	0x11a
    be81:	2b 1f       	bmi	0x0xbea2
    be83:	96 98       	ldaa	*0x98
    be85:	4c          	inca
    be86:	84 7f       	anda	#0x7f
    be88:	97 98       	staa	*0x98
    be8a:	ce 01 37    	ldx	#0x137
    be8d:	bd de a8    	jsr	0xdea8
    be90:	7f 01 1a    	clr	0x11a
    be93:	86 03       	ldaa	#0x3
    be95:	b7 01 1c    	staa	0x11c
    be98:	96 98       	ldaa	*0x98
    be9a:	c6 98       	ldab	#0x98
    be9c:	bd a8 5a    	jsr	0xa85a
    be9f:	7e ca 55    	jmp	0xca55
    bea2:	96 98       	ldaa	*0x98
    bea4:	4a          	deca
    bea5:	84 7f       	anda	#0x7f
    bea7:	20 df       	bra	0x0xbe88
    bea9:	7f 01 1a    	clr	0x11a
    beac:	7e ca 55    	jmp	0xca55
    beaf:	7d 01 1c    	tst	0x11c
    beb2:	2a 03       	bpl	0x0xbeb7
    beb4:	7e d7 73    	jmp	0xd773
    beb7:	7d 01 1e    	tst	0x11e
    beba:	26 08       	bne	0x0xbec4
    bebc:	7d 01 1c    	tst	0x11c
    bebf:	27 1f       	beq	0x0xbee0
    bec1:	7e a9 6a    	jmp	0xa96a
    bec4:	7d 01 1c    	tst	0x11c
    bec7:	27 24       	beq	0x0xbeed
    bec9:	cc 01 20    	ldd	#0x120
    becc:	fb 01 1e    	addb	0x11e
    becf:	8f          	xgdx
    bed0:	bd dd 4d    	jsr	0xdd4d
    bed3:	7a 01 1e    	dec	0x11e
    bed6:	7a 01 1c    	dec	0x11c
    bed9:	26 12       	bne	0x0xbeed
    bedb:	bd dd cb    	jsr	0xddcb
    bede:	20 0d       	bra	0x0xbeed
    bee0:	7d 01 1e    	tst	0x11e
    bee3:	26 08       	bne	0x0xbeed
    bee5:	86 13       	ldaa	#0x13
    bee7:	b7 01 1e    	staa	0x11e
    beea:	bd dd 89    	jsr	0xdd89
    beed:	7d 01 7f    	tst	0x17f
    bef0:	27 1e       	beq	0x0xbf10
    bef2:	2a 10       	bpl	0x0xbf04
    bef4:	bd df d4    	jsr	0xdfd4
    bef7:	4f          	clra
    bef8:	b7 01 7f    	staa	0x17f
    befb:	b7 01 7e    	staa	0x17e
    befe:	b7 01 1a    	staa	0x11a
    bf01:	7e ca 55    	jmp	0xca55
    bf04:	b6 01 1e    	ldaa	0x11e
    bf07:	81 19       	cmpa	#0x19
    bf09:	27 ec       	beq	0x0xbef7
    bf0b:	bd df d4    	jsr	0xdfd4
    bf0e:	20 e7       	bra	0x0xbef7
    bf10:	7d 01 1a    	tst	0x11a
    bf13:	26 03       	bne	0x0xbf18
    bf15:	7e ca 55    	jmp	0xca55
    bf18:	b6 01 1e    	ldaa	0x11e
    bf1b:	81 13       	cmpa	#0x13
    bf1d:	26 34       	bne	0x0xbf53
    bf1f:	b6 01 1a    	ldaa	0x11a
    bf22:	2b 27       	bmi	0x0xbf4b
    bf24:	d6 4b       	ldab	*0x4b
    bf26:	5c          	incb
    bf27:	c1 06       	cmpb	#0x6
    bf29:	23 02       	bls	0x0xbf2d
    bf2b:	c6 06       	ldab	#0x6
    bf2d:	d7 4b       	stab	*0x4b
    bf2f:	ce d7 d6    	ldx	#0xd7d6
    bf32:	18 ce 01 30 	ldy	#0x130
    bf36:	bd df 5b    	jsr	0xdf5b
    bf39:	7f 01 1a    	clr	0x11a
    bf3c:	86 04       	ldaa	#0x4
    bf3e:	b7 01 1c    	staa	0x11c
    bf41:	96 4b       	ldaa	*0x4b
    bf43:	c6 4b       	ldab	#0x4b
    bf45:	bd a8 5a    	jsr	0xa85a
    bf48:	7e ca 55    	jmp	0xca55
    bf4b:	d6 4b       	ldab	*0x4b
    bf4d:	5a          	decb
    bf4e:	2a dd       	bpl	0x0xbf2d
    bf50:	5f          	clrb
    bf51:	20 da       	bra	0x0xbf2d
    bf53:	81 19       	cmpa	#0x19
    bf55:	26 34       	bne	0x0xbf8b
    bf57:	b6 01 1a    	ldaa	0x11a
    bf5a:	2b 27       	bmi	0x0xbf83
    bf5c:	d6 66       	ldab	*0x66
    bf5e:	5c          	incb
    bf5f:	c1 03       	cmpb	#0x3
    bf61:	23 02       	bls	0x0xbf65
    bf63:	c6 03       	ldab	#0x3
    bf65:	d7 66       	stab	*0x66
    bf67:	ce d7 f2    	ldx	#0xd7f2
    bf6a:	18 ce 01 36 	ldy	#0x136
    bf6e:	bd df 5b    	jsr	0xdf5b
    bf71:	7f 01 1a    	clr	0x11a
    bf74:	86 04       	ldaa	#0x4
    bf76:	b7 01 1c    	staa	0x11c
    bf79:	96 66       	ldaa	*0x66
    bf7b:	c6 66       	ldab	#0x66
    bf7d:	bd a8 5a    	jsr	0xa85a
    bf80:	7e ca 55    	jmp	0xca55
    bf83:	d6 66       	ldab	*0x66
    bf85:	5a          	decb
    bf86:	2a dd       	bpl	0x0xbf65
    bf88:	5f          	clrb
    bf89:	20 da       	bra	0x0xbf65
    bf8b:	4f          	clra
    bf8c:	b7 01 7e    	staa	0x17e
    bf8f:	b7 01 7f    	staa	0x17f
    bf92:	b7 01 1a    	staa	0x11a
    bf95:	7e ca 55    	jmp	0xca55
    bf98:	7d 01 1c    	tst	0x11c
    bf9b:	2a 03       	bpl	0x0xbfa0
    bf9d:	7e d8 02    	jmp	0xd802
    bfa0:	7d 01 1e    	tst	0x11e
    bfa3:	26 08       	bne	0x0xbfad
    bfa5:	7d 01 1c    	tst	0x11c
    bfa8:	27 1f       	beq	0x0xbfc9
    bfaa:	7e a9 6a    	jmp	0xa96a
    bfad:	7d 01 1c    	tst	0x11c
    bfb0:	27 24       	beq	0x0xbfd6
    bfb2:	cc 01 20    	ldd	#0x120
    bfb5:	fb 01 1e    	addb	0x11e
    bfb8:	8f          	xgdx
    bfb9:	bd dd 4d    	jsr	0xdd4d
    bfbc:	7a 01 1e    	dec	0x11e
    bfbf:	7a 01 1c    	dec	0x11c
    bfc2:	26 12       	bne	0x0xbfd6
    bfc4:	bd dd fe    	jsr	0xddfe
    bfc7:	20 0d       	bra	0x0xbfd6
    bfc9:	7d 01 1e    	tst	0x11e
    bfcc:	26 08       	bne	0x0xbfd6
    bfce:	86 12       	ldaa	#0x12
    bfd0:	b7 01 1e    	staa	0x11e
    bfd3:	bd dd 89    	jsr	0xdd89
    bfd6:	7d 01 7f    	tst	0x17f
    bfd9:	27 10       	beq	0x0xbfeb
    bfdb:	bd e0 46    	jsr	0xe046
    bfde:	4f          	clra
    bfdf:	b7 01 7f    	staa	0x17f
    bfe2:	b7 01 7e    	staa	0x17e
    bfe5:	b7 01 1a    	staa	0x11a
    bfe8:	7e ca 55    	jmp	0xca55
    bfeb:	7d 01 1a    	tst	0x11a
    bfee:	26 03       	bne	0x0xbff3
    bff0:	7e ca 55    	jmp	0xca55
    bff3:	b6 01 1e    	ldaa	0x11e
    bff6:	81 12       	cmpa	#0x12
    bff8:	26 2b       	bne	0x0xc025
    bffa:	b6 01 1a    	ldaa	0x11a
    bffd:	2b 1f       	bmi	0x0xc01e
    bfff:	96 6e       	ldaa	*0x6e
    c001:	4c          	inca
    c002:	84 7f       	anda	#0x7f
    c004:	97 6e       	staa	*0x6e
    c006:	ce 01 30    	ldx	#0x130
    c009:	bd de a8    	jsr	0xdea8
    c00c:	7f 01 1a    	clr	0x11a
    c00f:	86 03       	ldaa	#0x3
    c011:	b7 01 1c    	staa	0x11c
    c014:	96 6e       	ldaa	*0x6e
    c016:	c6 6e       	ldab	#0x6e
    c018:	bd a8 5a    	jsr	0xa85a
    c01b:	7e ca 55    	jmp	0xca55
    c01e:	96 6e       	ldaa	*0x6e
    c020:	4a          	deca
    c021:	84 7f       	anda	#0x7f
    c023:	20 df       	bra	0x0xc004
    c025:	81 16       	cmpa	#0x16
    c027:	26 2b       	bne	0x0xc054
    c029:	b6 01 1a    	ldaa	0x11a
    c02c:	2b 1f       	bmi	0x0xc04d
    c02e:	96 48       	ldaa	*0x48
    c030:	4c          	inca
    c031:	84 7f       	anda	#0x7f
    c033:	97 48       	staa	*0x48
    c035:	ce 01 34    	ldx	#0x134
    c038:	bd de a8    	jsr	0xdea8
    c03b:	7f 01 1a    	clr	0x11a
    c03e:	86 03       	ldaa	#0x3
    c040:	b7 01 1c    	staa	0x11c
    c043:	96 48       	ldaa	*0x48
    c045:	c6 48       	ldab	#0x48
    c047:	bd a8 5a    	jsr	0xa85a
    c04a:	7e ca 55    	jmp	0xca55
    c04d:	96 48       	ldaa	*0x48
    c04f:	4a          	deca
    c050:	84 7f       	anda	#0x7f
    c052:	20 df       	bra	0x0xc033
    c054:	81 1a       	cmpa	#0x1a
    c056:	26 2b       	bne	0x0xc083
    c058:	b6 01 1a    	ldaa	0x11a
    c05b:	2b 1f       	bmi	0x0xc07c
    c05d:	96 6f       	ldaa	*0x6f
    c05f:	4c          	inca
    c060:	84 7f       	anda	#0x7f
    c062:	97 6f       	staa	*0x6f
    c064:	ce 01 38    	ldx	#0x138
    c067:	bd de a8    	jsr	0xdea8
    c06a:	7f 01 1a    	clr	0x11a
    c06d:	86 03       	ldaa	#0x3
    c06f:	b7 01 1c    	staa	0x11c
    c072:	96 6f       	ldaa	*0x6f
    c074:	c6 6f       	ldab	#0x6f
    c076:	bd a8 5a    	jsr	0xa85a
    c079:	7e ca 55    	jmp	0xca55
    c07c:	96 6f       	ldaa	*0x6f
    c07e:	4a          	deca
    c07f:	84 7f       	anda	#0x7f
    c081:	20 df       	bra	0x0xc062
    c083:	b6 01 1a    	ldaa	0x11a
    c086:	2b 21       	bmi	0x0xc0a9
    c088:	96 70       	ldaa	*0x70
    c08a:	4c          	inca
    c08b:	2a 02       	bpl	0x0xc08f
    c08d:	86 7f       	ldaa	#0x7f
    c08f:	97 70       	staa	*0x70
    c091:	ce 01 3c    	ldx	#0x13c
    c094:	bd df 6f    	jsr	0xdf6f
    c097:	7f 01 1a    	clr	0x11a
    c09a:	86 03       	ldaa	#0x3
    c09c:	b7 01 1c    	staa	0x11c
    c09f:	96 70       	ldaa	*0x70
    c0a1:	c6 70       	ldab	#0x70
    c0a3:	bd a8 5a    	jsr	0xa85a
    c0a6:	7e ca 55    	jmp	0xca55
    c0a9:	96 70       	ldaa	*0x70
    c0ab:	4a          	deca
    c0ac:	2a e1       	bpl	0x0xc08f
    c0ae:	4f          	clra
    c0af:	20 de       	bra	0x0xc08f
    c0b1:	7d 01 1c    	tst	0x11c
    c0b4:	2a 03       	bpl	0x0xc0b9
    c0b6:	7e d8 59    	jmp	0xd859
    c0b9:	7d 01 1e    	tst	0x11e
    c0bc:	26 08       	bne	0x0xc0c6
    c0be:	7d 01 1c    	tst	0x11c
    c0c1:	27 1f       	beq	0x0xc0e2
    c0c3:	7e a9 6a    	jmp	0xa96a
    c0c6:	7d 01 1c    	tst	0x11c
    c0c9:	27 24       	beq	0x0xc0ef
    c0cb:	cc 01 20    	ldd	#0x120
    c0ce:	fb 01 1e    	addb	0x11e
    c0d1:	8f          	xgdx
    c0d2:	bd dd 4d    	jsr	0xdd4d
    c0d5:	7a 01 1e    	dec	0x11e
    c0d8:	7a 01 1c    	dec	0x11c
    c0db:	26 12       	bne	0x0xc0ef
    c0dd:	bd dd cb    	jsr	0xddcb
    c0e0:	20 0d       	bra	0x0xc0ef
    c0e2:	7d 01 1e    	tst	0x11e
    c0e5:	26 08       	bne	0x0xc0ef
    c0e7:	86 13       	ldaa	#0x13
    c0e9:	b7 01 1e    	staa	0x11e
    c0ec:	bd dd 89    	jsr	0xdd89
    c0ef:	7d 01 7f    	tst	0x17f
    c0f2:	27 10       	beq	0x0xc104
    c0f4:	bd df d4    	jsr	0xdfd4
    c0f7:	4f          	clra
    c0f8:	b7 01 7f    	staa	0x17f
    c0fb:	b7 01 7e    	staa	0x17e
    c0fe:	b7 01 1a    	staa	0x11a
    c101:	7e ca 55    	jmp	0xca55
    c104:	7d 01 1a    	tst	0x11a
    c107:	26 03       	bne	0x0xc10c
    c109:	7e ca 55    	jmp	0xca55
    c10c:	b6 01 1e    	ldaa	0x11e
    c10f:	81 13       	cmpa	#0x13
    c111:	26 30       	bne	0x0xc143
    c113:	7d 01 1a    	tst	0x11a
    c116:	2b 23       	bmi	0x0xc13b
    c118:	d6 74       	ldab	*0x74
    c11a:	5c          	incb
    c11b:	c1 05       	cmpb	#0x5
    c11d:	25 02       	bcs	0x0xc121
    c11f:	c6 04       	ldab	#0x4
    c121:	d7 74       	stab	*0x74
    c123:	18 ce 01 30 	ldy	#0x130
    c127:	ce d8 c8    	ldx	#0xd8c8
    c12a:	bd df 5b    	jsr	0xdf5b
    c12d:	86 04       	ldaa	#0x4
    c12f:	b7 01 1c    	staa	0x11c
    c132:	7f 01 1a    	clr	0x11a
    c135:	7f 01 7e    	clr	0x17e
    c138:	7e ca 55    	jmp	0xca55
    c13b:	d6 74       	ldab	*0x74
    c13d:	5a          	decb
    c13e:	2a e1       	bpl	0x0xc121
    c140:	5f          	clrb
    c141:	20 de       	bra	0x0xc121
    c143:	81 19       	cmpa	#0x19
    c145:	26 31       	bne	0x0xc178
    c147:	d6 72       	ldab	*0x72
    c149:	7d 01 1a    	tst	0x11a
    c14c:	2b 24       	bmi	0x0xc172
    c14e:	5c          	incb
    c14f:	c1 0d       	cmpb	#0xd
    c151:	25 02       	bcs	0x0xc155
    c153:	c6 0c       	ldab	#0xc
    c155:	d7 72       	stab	*0x72
    c157:	f7 01 15    	stab	0x115
    c15a:	18 ce 01 36 	ldy	#0x136
    c15e:	ce da 7d    	ldx	#0xda7d
    c161:	bd df 5b    	jsr	0xdf5b
    c164:	86 04       	ldaa	#0x4
    c166:	b7 01 1c    	staa	0x11c
    c169:	7f 01 1a    	clr	0x11a
    c16c:	7f 01 7e    	clr	0x17e
    c16f:	7e ca 55    	jmp	0xca55
    c172:	5a          	decb
    c173:	2a e0       	bpl	0x0xc155
    c175:	5f          	clrb
    c176:	20 dd       	bra	0x0xc155
    c178:	d6 73       	ldab	*0x73
    c17a:	7d 01 1a    	tst	0x11a
    c17d:	2b 21       	bmi	0x0xc1a0
    c17f:	5c          	incb
    c180:	c1 04       	cmpb	#0x4
    c182:	25 02       	bcs	0x0xc186
    c184:	c6 03       	ldab	#0x3
    c186:	d7 73       	stab	*0x73
    c188:	18 ce 01 3b 	ldy	#0x13b
    c18c:	ce d8 dc    	ldx	#0xd8dc
    c18f:	bd df 5b    	jsr	0xdf5b
    c192:	86 04       	ldaa	#0x4
    c194:	b7 01 1c    	staa	0x11c
    c197:	7f 01 1a    	clr	0x11a
    c19a:	7f 01 7e    	clr	0x17e
    c19d:	7e ca 55    	jmp	0xca55
    c1a0:	5a          	decb
    c1a1:	c1 01       	cmpb	#0x1
    c1a3:	24 e1       	bcc	0x0xc186
    c1a5:	c6 01       	ldab	#0x1
    c1a7:	20 dd       	bra	0x0xc186
    c1a9:	7d 01 1c    	tst	0x11c
    c1ac:	2a 03       	bpl	0x0xc1b1
    c1ae:	7e d8 f0    	jmp	0xd8f0
    c1b1:	7d 01 1e    	tst	0x11e
    c1b4:	26 08       	bne	0x0xc1be
    c1b6:	7d 01 1c    	tst	0x11c
    c1b9:	27 1f       	beq	0x0xc1da
    c1bb:	7e a9 6a    	jmp	0xa96a
    c1be:	7d 01 1c    	tst	0x11c
    c1c1:	27 24       	beq	0x0xc1e7
    c1c3:	cc 01 20    	ldd	#0x120
    c1c6:	fb 01 1e    	addb	0x11e
    c1c9:	8f          	xgdx
    c1ca:	bd dd 4d    	jsr	0xdd4d
    c1cd:	7a 01 1e    	dec	0x11e
    c1d0:	7a 01 1c    	dec	0x11c
    c1d3:	26 12       	bne	0x0xc1e7
    c1d5:	bd dd cb    	jsr	0xddcb
    c1d8:	20 0d       	bra	0x0xc1e7
    c1da:	7d 01 1e    	tst	0x11e
    c1dd:	26 08       	bne	0x0xc1e7
    c1df:	86 03       	ldaa	#0x3
    c1e1:	b7 01 1e    	staa	0x11e
    c1e4:	bd dd 89    	jsr	0xdd89
    c1e7:	7d 01 7f    	tst	0x17f
    c1ea:	27 10       	beq	0x0xc1fc
    c1ec:	bd df d4    	jsr	0xdfd4
    c1ef:	4f          	clra
    c1f0:	b7 01 7f    	staa	0x17f
    c1f3:	b7 01 7e    	staa	0x17e
    c1f6:	b7 01 1a    	staa	0x11a
    c1f9:	7e ca 55    	jmp	0xca55
    c1fc:	7d 01 7e    	tst	0x17e
    c1ff:	27 0c       	beq	0x0xc20d
    c201:	bd df 7c    	jsr	0xdf7c
    c204:	7f 01 7e    	clr	0x17e
    c207:	7f 01 1a    	clr	0x11a
    c20a:	7e ca 55    	jmp	0xca55
    c20d:	7d 01 1a    	tst	0x11a
    c210:	26 03       	bne	0x0xc215
    c212:	7e ca 55    	jmp	0xca55
    c215:	ce d9 94    	ldx	#0xd994
    c218:	b6 01 1e    	ldaa	0x11e
    c21b:	81 03       	cmpa	#0x3
    c21d:	26 34       	bne	0x0xc253
    c21f:	18 ce 01 20 	ldy	#0x120
    c223:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc23d
    c227:	d6 2d       	ldab	*0x2d
    c229:	bd c2 c3    	jsr	0xc2c3
    c22c:	d7 2d       	stab	*0x2d
    c22e:	bd df 5b    	jsr	0xdf5b
    c231:	96 2d       	ldaa	*0x2d
    c233:	c6 2d       	ldab	#0x2d
    c235:	bd a8 5a    	jsr	0xa85a
    c238:	86 04       	ldaa	#0x4
    c23a:	7e c3 70    	jmp	0xc370
    c23d:	d6 3a       	ldab	*0x3a
    c23f:	bd c2 c3    	jsr	0xc2c3
    c242:	d7 3a       	stab	*0x3a
    c244:	bd df 5b    	jsr	0xdf5b
    c247:	96 3a       	ldaa	*0x3a
    c249:	c6 3a       	ldab	#0x3a
    c24b:	bd a8 5a    	jsr	0xa85a
    c24e:	86 04       	ldaa	#0x4
    c250:	7e c3 70    	jmp	0xc370
    c253:	81 09       	cmpa	#0x9
    c255:	26 34       	bne	0x0xc28b
    c257:	18 ce 01 26 	ldy	#0x126
    c25b:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc275
    c25f:	d6 2e       	ldab	*0x2e
    c261:	bd c2 c3    	jsr	0xc2c3
    c264:	d7 2e       	stab	*0x2e
    c266:	bd df 5b    	jsr	0xdf5b
    c269:	96 2e       	ldaa	*0x2e
    c26b:	c6 2e       	ldab	#0x2e
    c26d:	bd a8 5a    	jsr	0xa85a
    c270:	86 04       	ldaa	#0x4
    c272:	7e c3 70    	jmp	0xc370
    c275:	d6 3b       	ldab	*0x3b
    c277:	bd c2 c3    	jsr	0xc2c3
    c27a:	d7 3b       	stab	*0x3b
    c27c:	bd df 5b    	jsr	0xdf5b
    c27f:	96 3b       	ldaa	*0x3b
    c281:	c6 3b       	ldab	#0x3b
    c283:	bd a8 5a    	jsr	0xa85a
    c286:	86 04       	ldaa	#0x4
    c288:	7e c3 70    	jmp	0xc370
    c28b:	81 0e       	cmpa	#0xe
    c28d:	26 46       	bne	0x0xc2d5
    c28f:	18 ce 01 2b 	ldy	#0x12b
    c293:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc2ad
    c297:	d6 2f       	ldab	*0x2f
    c299:	bd c2 c3    	jsr	0xc2c3
    c29c:	d7 2f       	stab	*0x2f
    c29e:	bd df 5b    	jsr	0xdf5b
    c2a1:	96 2f       	ldaa	*0x2f
    c2a3:	c6 2f       	ldab	#0x2f
    c2a5:	bd a8 5a    	jsr	0xa85a
    c2a8:	86 04       	ldaa	#0x4
    c2aa:	7e c3 70    	jmp	0xc370
    c2ad:	d6 3c       	ldab	*0x3c
    c2af:	bd c2 c3    	jsr	0xc2c3
    c2b2:	d7 3c       	stab	*0x3c
    c2b4:	bd df 5b    	jsr	0xdf5b
    c2b7:	96 3c       	ldaa	*0x3c
    c2b9:	c6 3c       	ldab	#0x3c
    c2bb:	bd a8 5a    	jsr	0xa85a
    c2be:	86 04       	ldaa	#0x4
    c2c0:	7e c3 70    	jmp	0xc370
    c2c3:	7d 01 1a    	tst	0x11a
    c2c6:	2b 08       	bmi	0x0xc2d0
    c2c8:	5c          	incb
    c2c9:	c1 11       	cmpb	#0x11
    c2cb:	25 02       	bcs	0x0xc2cf
    c2cd:	c6 10       	ldab	#0x10
    c2cf:	39          	rts
    c2d0:	5a          	decb
    c2d1:	2a fc       	bpl	0x0xc2cf
    c2d3:	5f          	clrb
    c2d4:	39          	rts
    c2d5:	81 13       	cmpa	#0x13
    c2d7:	26 33       	bne	0x0xc30c
    c2d9:	ce 01 31    	ldx	#0x131
    c2dc:	12 f4 02 16 	brset	*0xf4, #0x02, 0x0xc2f6
    c2e0:	96 2a       	ldaa	*0x2a
    c2e2:	bd c3 79    	jsr	0xc379
    c2e5:	97 2a       	staa	*0x2a
    c2e7:	bd de a8    	jsr	0xdea8
    c2ea:	96 2a       	ldaa	*0x2a
    c2ec:	c6 2a       	ldab	#0x2a
    c2ee:	bd a8 5a    	jsr	0xa85a
    c2f1:	86 03       	ldaa	#0x3
    c2f3:	7e c3 70    	jmp	0xc370
    c2f6:	96 37       	ldaa	*0x37
    c2f8:	bd c3 79    	jsr	0xc379
    c2fb:	97 37       	staa	*0x37
    c2fd:	bd de a8    	jsr	0xdea8
    c300:	96 37       	ldaa	*0x37
    c302:	c6 37       	ldab	#0x37
    c304:	bd a8 5a    	jsr	0xa85a
    c307:	86 03       	ldaa	#0x3
    c309:	7e c3 70    	jmp	0xc370
    c30c:	81 19       	cmpa	#0x19
    c30e:	26 31       	bne	0x0xc341
    c310:	ce 01 37    	ldx	#0x137
    c313:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc32c
    c317:	96 2b       	ldaa	*0x2b
    c319:	bd c3 79    	jsr	0xc379
    c31c:	97 2b       	staa	*0x2b
    c31e:	bd de a8    	jsr	0xdea8
    c321:	96 2b       	ldaa	*0x2b
    c323:	c6 2b       	ldab	#0x2b
    c325:	bd a8 5a    	jsr	0xa85a
    c328:	86 03       	ldaa	#0x3
    c32a:	20 44       	bra	0x0xc370
    c32c:	96 38       	ldaa	*0x38
    c32e:	bd c3 79    	jsr	0xc379
    c331:	97 38       	staa	*0x38
    c333:	bd de a8    	jsr	0xdea8
    c336:	96 38       	ldaa	*0x38
    c338:	c6 38       	ldab	#0x38
    c33a:	bd a8 5a    	jsr	0xa85a
    c33d:	86 03       	ldaa	#0x3
    c33f:	20 2f       	bra	0x0xc370
    c341:	ce 01 3c    	ldx	#0x13c
    c344:	12 f4 02 15 	brset	*0xf4, #0x02, 0x0xc35d
    c348:	96 2c       	ldaa	*0x2c
    c34a:	bd c3 79    	jsr	0xc379
    c34d:	97 2c       	staa	*0x2c
    c34f:	bd de a8    	jsr	0xdea8
    c352:	96 2c       	ldaa	*0x2c
    c354:	c6 2c       	ldab	#0x2c
    c356:	bd a8 5a    	jsr	0xa85a
    c359:	86 03       	ldaa	#0x3
    c35b:	20 13       	bra	0x0xc370
    c35d:	96 39       	ldaa	*0x39
    c35f:	bd c3 79    	jsr	0xc379
    c362:	97 39       	staa	*0x39
    c364:	bd de a8    	jsr	0xdea8
    c367:	96 39       	ldaa	*0x39
    c369:	c6 39       	ldab	#0x39
    c36b:	bd a8 5a    	jsr	0xa85a
    c36e:	86 03       	ldaa	#0x3
    c370:	b7 01 1c    	staa	0x11c
    c373:	7f 01 1a    	clr	0x11a
    c376:	7e ca 55    	jmp	0xca55
    c379:	7d 01 1a    	tst	0x11a
    c37c:	2b 04       	bmi	0x0xc382
    c37e:	4c          	inca
    c37f:	84 7f       	anda	#0x7f
    c381:	39          	rts
    c382:	4a          	deca
    c383:	84 7f       	anda	#0x7f
    c385:	20 fa       	bra	0x0xc381
    c387:	7d 01 1c    	tst	0x11c
    c38a:	2a 03       	bpl	0x0xc38f
    c38c:	7e d9 d8    	jmp	0xd9d8
    c38f:	7d 01 1e    	tst	0x11e
    c392:	26 08       	bne	0x0xc39c
    c394:	7d 01 1c    	tst	0x11c
    c397:	27 1f       	beq	0x0xc3b8
    c399:	7e a9 6a    	jmp	0xa96a
    c39c:	7d 01 1c    	tst	0x11c
    c39f:	27 24       	beq	0x0xc3c5
    c3a1:	cc 01 20    	ldd	#0x120
    c3a4:	fb 01 1e    	addb	0x11e
    c3a7:	8f          	xgdx
    c3a8:	bd dd 4d    	jsr	0xdd4d
    c3ab:	7a 01 1e    	dec	0x11e
    c3ae:	7a 01 1c    	dec	0x11c
    c3b1:	26 12       	bne	0x0xc3c5
    c3b3:	bd dd cb    	jsr	0xddcb
    c3b6:	20 0d       	bra	0x0xc3c5
    c3b8:	7d 01 1e    	tst	0x11e
    c3bb:	26 08       	bne	0x0xc3c5
    c3bd:	86 13       	ldaa	#0x13
    c3bf:	b7 01 1e    	staa	0x11e
    c3c2:	bd dd 89    	jsr	0xdd89
    c3c5:	7d 01 7f    	tst	0x17f
    c3c8:	27 10       	beq	0x0xc3da
    c3ca:	bd df d4    	jsr	0xdfd4
    c3cd:	4f          	clra
    c3ce:	b7 01 7f    	staa	0x17f
    c3d1:	b7 01 7e    	staa	0x17e
    c3d4:	b7 01 1a    	staa	0x11a
    c3d7:	7e ca 55    	jmp	0xca55
    c3da:	7d 01 1a    	tst	0x11a
    c3dd:	26 03       	bne	0x0xc3e2
    c3df:	7e ca 55    	jmp	0xca55
    c3e2:	b6 01 1e    	ldaa	0x11e
    c3e5:	81 13       	cmpa	#0x13
    c3e7:	26 53       	bne	0x0xc43c
    c3e9:	ce da 5d    	ldx	#0xda5d
    c3ec:	18 ce 01 30 	ldy	#0x130
    c3f0:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc421
    c3f4:	d6 27       	ldab	*0x27
    c3f6:	8d 17       	bsr	0x0xc40f
    c3f8:	d7 27       	stab	*0x27
    c3fa:	bd df 5b    	jsr	0xdf5b
    c3fd:	7f 01 1a    	clr	0x11a
    c400:	86 04       	ldaa	#0x4
    c402:	b7 01 1c    	staa	0x11c
    c405:	96 27       	ldaa	*0x27
    c407:	c6 27       	ldab	#0x27
    c409:	bd a8 5a    	jsr	0xa85a
    c40c:	7e ca 55    	jmp	0xca55
    c40f:	7d 01 1a    	tst	0x11a
    c412:	2b 08       	bmi	0x0xc41c
    c414:	5c          	incb
    c415:	c1 06       	cmpb	#0x6
    c417:	25 02       	bcs	0x0xc41b
    c419:	c6 05       	ldab	#0x5
    c41b:	39          	rts
    c41c:	5a          	decb
    c41d:	2a fc       	bpl	0x0xc41b
    c41f:	5f          	clrb
    c420:	39          	rts
    c421:	d6 34       	ldab	*0x34
    c423:	8d ea       	bsr	0x0xc40f
    c425:	d7 34       	stab	*0x34
    c427:	bd df 5b    	jsr	0xdf5b
    c42a:	7f 01 1a    	clr	0x11a
    c42d:	86 04       	ldaa	#0x4
    c42f:	b7 01 1c    	staa	0x11c
    c432:	96 34       	ldaa	*0x34
    c434:	c6 34       	ldab	#0x34
    c436:	bd a8 5a    	jsr	0xa85a
    c439:	7e ca 55    	jmp	0xca55
    c43c:	81 19       	cmpa	#0x19
    c43e:	26 23       	bne	0x0xc463
    c440:	ce da 75    	ldx	#0xda75
    c443:	18 ce 01 36 	ldy	#0x136
    c447:	d6 31       	ldab	*0x31
    c449:	5c          	incb
    c44a:	c4 01       	andb	#0x1
    c44c:	d7 31       	stab	*0x31
    c44e:	bd df 5b    	jsr	0xdf5b
    c451:	7f 01 1a    	clr	0x11a
    c454:	86 04       	ldaa	#0x4
    c456:	b7 01 1c    	staa	0x11c
    c459:	96 31       	ldaa	*0x31
    c45b:	c6 31       	ldab	#0x31
    c45d:	bd a8 5a    	jsr	0xa85a
    c460:	7e ca 55    	jmp	0xca55
    c463:	ce da 7d    	ldx	#0xda7d
    c466:	18 ce 01 3b 	ldy	#0x13b
    c46a:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc49b
    c46e:	d6 25       	ldab	*0x25
    c470:	8d 17       	bsr	0x0xc489
    c472:	d7 25       	stab	*0x25
    c474:	bd df 5b    	jsr	0xdf5b
    c477:	7f 01 1a    	clr	0x11a
    c47a:	86 04       	ldaa	#0x4
    c47c:	b7 01 1c    	staa	0x11c
    c47f:	96 25       	ldaa	*0x25
    c481:	c6 25       	ldab	#0x25
    c483:	bd a8 5a    	jsr	0xa85a
    c486:	7e ca 55    	jmp	0xca55
    c489:	7d 01 1a    	tst	0x11a
    c48c:	2b 08       	bmi	0x0xc496
    c48e:	5c          	incb
    c48f:	c1 0f       	cmpb	#0xf
    c491:	25 02       	bcs	0x0xc495
    c493:	c6 0e       	ldab	#0xe
    c495:	39          	rts
    c496:	5a          	decb
    c497:	2a fc       	bpl	0x0xc495
    c499:	5f          	clrb
    c49a:	39          	rts
    c49b:	d6 32       	ldab	*0x32
    c49d:	8d ea       	bsr	0x0xc489
    c49f:	d7 32       	stab	*0x32
    c4a1:	bd df 5b    	jsr	0xdf5b
    c4a4:	7f 01 1a    	clr	0x11a
    c4a7:	86 04       	ldaa	#0x4
    c4a9:	b7 01 1c    	staa	0x11c
    c4ac:	96 32       	ldaa	*0x32
    c4ae:	c6 32       	ldab	#0x32
    c4b0:	bd a8 5a    	jsr	0xa85a
    c4b3:	7e ca 55    	jmp	0xca55
    c4b6:	7d 01 1c    	tst	0x11c
    c4b9:	2a 03       	bpl	0x0xc4be
    c4bb:	7e da b1    	jmp	0xdab1
    c4be:	7d 01 1e    	tst	0x11e
    c4c1:	26 08       	bne	0x0xc4cb
    c4c3:	7d 01 1c    	tst	0x11c
    c4c6:	27 1f       	beq	0x0xc4e7
    c4c8:	7e a9 6a    	jmp	0xa96a
    c4cb:	7d 01 1c    	tst	0x11c
    c4ce:	27 24       	beq	0x0xc4f4
    c4d0:	cc 01 20    	ldd	#0x120
    c4d3:	fb 01 1e    	addb	0x11e
    c4d6:	8f          	xgdx
    c4d7:	bd dd 4d    	jsr	0xdd4d
    c4da:	7a 01 1e    	dec	0x11e
    c4dd:	7a 01 1c    	dec	0x11c
    c4e0:	26 12       	bne	0x0xc4f4
    c4e2:	bd dd cb    	jsr	0xddcb
    c4e5:	20 0d       	bra	0x0xc4f4
    c4e7:	7d 01 1e    	tst	0x11e
    c4ea:	26 08       	bne	0x0xc4f4
    c4ec:	86 19       	ldaa	#0x19
    c4ee:	b7 01 1e    	staa	0x11e
    c4f1:	bd dd 89    	jsr	0xdd89
    c4f4:	7d 01 7f    	tst	0x17f
    c4f7:	27 1e       	beq	0x0xc517
    c4f9:	2b 10       	bmi	0x0xc50b
    c4fb:	bd df d4    	jsr	0xdfd4
    c4fe:	4f          	clra
    c4ff:	b7 01 7f    	staa	0x17f
    c502:	b7 01 7e    	staa	0x17e
    c505:	b7 01 1a    	staa	0x11a
    c508:	7e ca 55    	jmp	0xca55
    c50b:	b6 01 1e    	ldaa	0x11e
    c50e:	81 19       	cmpa	#0x19
    c510:	27 ec       	beq	0x0xc4fe
    c512:	bd df d4    	jsr	0xdfd4
    c515:	20 e7       	bra	0x0xc4fe
    c517:	7d 01 1a    	tst	0x11a
    c51a:	26 03       	bne	0x0xc51f
    c51c:	7e ca 55    	jmp	0xca55
    c51f:	b6 01 1e    	ldaa	0x11e
    c522:	81 13       	cmpa	#0x13
    c524:	26 5a       	bne	0x0xc580
    c526:	12 f4 02 2b 	brset	*0xf4, #0x02, 0x0xc555
    c52a:	7d 01 1a    	tst	0x11a
    c52d:	2b 1f       	bmi	0x0xc54e
    c52f:	96 29       	ldaa	*0x29
    c531:	4c          	inca
    c532:	84 7f       	anda	#0x7f
    c534:	97 29       	staa	*0x29
    c536:	ce 01 31    	ldx	#0x131
    c539:	bd de a8    	jsr	0xdea8
    c53c:	7f 01 1a    	clr	0x11a
    c53f:	86 03       	ldaa	#0x3
    c541:	b7 01 1c    	staa	0x11c
    c544:	96 29       	ldaa	*0x29
    c546:	c6 29       	ldab	#0x29
    c548:	bd a8 5a    	jsr	0xa85a
    c54b:	7e ca 55    	jmp	0xca55
    c54e:	96 29       	ldaa	*0x29
    c550:	4a          	deca
    c551:	84 7f       	anda	#0x7f
    c553:	20 df       	bra	0x0xc534
    c555:	7d 01 1a    	tst	0x11a
    c558:	2b 1f       	bmi	0x0xc579
    c55a:	96 36       	ldaa	*0x36
    c55c:	4c          	inca
    c55d:	84 7f       	anda	#0x7f
    c55f:	97 36       	staa	*0x36
    c561:	ce 01 31    	ldx	#0x131
    c564:	bd de a8    	jsr	0xdea8
    c567:	7f 01 1a    	clr	0x11a
    c56a:	86 03       	ldaa	#0x3
    c56c:	b7 01 1c    	staa	0x11c
    c56f:	96 36       	ldaa	*0x36
    c571:	c6 36       	ldab	#0x36
    c573:	bd a8 5a    	jsr	0xa85a
    c576:	7e ca 55    	jmp	0xca55
    c579:	96 36       	ldaa	*0x36
    c57b:	4a          	deca
    c57c:	84 7f       	anda	#0x7f
    c57e:	20 df       	bra	0x0xc55f
    c580:	81 19       	cmpa	#0x19
    c582:	26 65       	bne	0x0xc5e9
    c584:	ce db 17    	ldx	#0xdb17
    c587:	18 ce 01 37 	ldy	#0x137
    c58b:	12 f4 02 2d 	brset	*0xf4, #0x02, 0x0xc5bc
    c58f:	7d 01 1a    	tst	0x11a
    c592:	2b 20       	bmi	0x0xc5b4
    c594:	d6 26       	ldab	*0x26
    c596:	5c          	incb
    c597:	c1 03       	cmpb	#0x3
    c599:	25 02       	bcs	0x0xc59d
    c59b:	c6 02       	ldab	#0x2
    c59d:	d7 26       	stab	*0x26
    c59f:	bd df 47    	jsr	0xdf47
    c5a2:	7f 01 1a    	clr	0x11a
    c5a5:	86 03       	ldaa	#0x3
    c5a7:	b7 01 1c    	staa	0x11c
    c5aa:	96 26       	ldaa	*0x26
    c5ac:	c6 26       	ldab	#0x26
    c5ae:	bd a8 5a    	jsr	0xa85a
    c5b1:	7e ca 55    	jmp	0xca55
    c5b4:	d6 26       	ldab	*0x26
    c5b6:	5a          	decb
    c5b7:	2a e4       	bpl	0x0xc59d
    c5b9:	5f          	clrb
    c5ba:	20 e1       	bra	0x0xc59d
    c5bc:	7d 01 1a    	tst	0x11a
    c5bf:	2b 20       	bmi	0x0xc5e1
    c5c1:	d6 33       	ldab	*0x33
    c5c3:	5c          	incb
    c5c4:	c1 03       	cmpb	#0x3
    c5c6:	25 02       	bcs	0x0xc5ca
    c5c8:	c6 02       	ldab	#0x2
    c5ca:	d7 33       	stab	*0x33
    c5cc:	bd df 47    	jsr	0xdf47
    c5cf:	7f 01 1a    	clr	0x11a
    c5d2:	86 03       	ldaa	#0x3
    c5d4:	b7 01 1c    	staa	0x11c
    c5d7:	96 33       	ldaa	*0x33
    c5d9:	c6 33       	ldab	#0x33
    c5db:	bd a8 5a    	jsr	0xa85a
    c5de:	7e ca 55    	jmp	0xca55
    c5e1:	d6 33       	ldab	*0x33
    c5e3:	5a          	decb
    c5e4:	2a e4       	bpl	0x0xc5ca
    c5e6:	5f          	clrb
    c5e7:	20 e1       	bra	0x0xc5ca
    c5e9:	ce db 26    	ldx	#0xdb26
    c5ec:	18 ce 01 3c 	ldy	#0x13c
    c5f0:	7d 01 1a    	tst	0x11a
    c5f3:	2b 20       	bmi	0x0xc615
    c5f5:	d6 96       	ldab	*0x96
    c5f7:	5c          	incb
    c5f8:	c1 04       	cmpb	#0x4
    c5fa:	25 02       	bcs	0x0xc5fe
    c5fc:	c6 03       	ldab	#0x3
    c5fe:	d7 96       	stab	*0x96
    c600:	bd df 47    	jsr	0xdf47
    c603:	7f 01 1a    	clr	0x11a
    c606:	86 03       	ldaa	#0x3
    c608:	b7 01 1c    	staa	0x11c
    c60b:	96 96       	ldaa	*0x96
    c60d:	c6 96       	ldab	#0x96
    c60f:	bd a8 5a    	jsr	0xa85a
    c612:	7e ca 55    	jmp	0xca55
    c615:	d6 96       	ldab	*0x96
    c617:	5a          	decb
    c618:	2a e4       	bpl	0x0xc5fe
    c61a:	5f          	clrb
    c61b:	20 e1       	bra	0x0xc5fe
    c61d:	7d 01 1c    	tst	0x11c
    c620:	2a 03       	bpl	0x0xc625
    c622:	7e db 32    	jmp	0xdb32
    c625:	7d 01 1c    	tst	0x11c
    c628:	27 03       	beq	0x0xc62d
    c62a:	7e a9 6a    	jmp	0xa96a
    c62d:	7d 01 1e    	tst	0x11e
    c630:	26 08       	bne	0x0xc63a
    c632:	86 04       	ldaa	#0x4
    c634:	b7 01 1e    	staa	0x11e
    c637:	bd dd 89    	jsr	0xdd89
    c63a:	7e ca 55    	jmp	0xca55
    c63d:	7d 01 1c    	tst	0x11c
    c640:	2a 03       	bpl	0x0xc645
    c642:	7e db 35    	jmp	0xdb35
    c645:	7d 01 1e    	tst	0x11e
    c648:	26 08       	bne	0x0xc652
    c64a:	7d 01 1c    	tst	0x11c
    c64d:	27 1f       	beq	0x0xc66e
    c64f:	7e a9 6a    	jmp	0xa96a
    c652:	7d 01 1c    	tst	0x11c
    c655:	27 24       	beq	0x0xc67b
    c657:	cc 01 20    	ldd	#0x120
    c65a:	fb 01 1e    	addb	0x11e
    c65d:	8f          	xgdx
    c65e:	bd dd 4d    	jsr	0xdd4d
    c661:	7a 01 1e    	dec	0x11e
    c664:	7a 01 1c    	dec	0x11c
    c667:	26 12       	bne	0x0xc67b
    c669:	bd dd fe    	jsr	0xddfe
    c66c:	20 0d       	bra	0x0xc67b
    c66e:	7d 01 1e    	tst	0x11e
    c671:	26 08       	bne	0x0xc67b
    c673:	86 0e       	ldaa	#0xe
    c675:	b7 01 1e    	staa	0x11e
    c678:	bd dd 89    	jsr	0xdd89
    c67b:	7d 01 7f    	tst	0x17f
    c67e:	27 0d       	beq	0x0xc68d
    c680:	4f          	clra
    c681:	b7 01 7f    	staa	0x17f
    c684:	b7 01 7e    	staa	0x17e
    c687:	b7 01 1a    	staa	0x11a
    c68a:	7e ca 55    	jmp	0xca55
    c68d:	7d 01 7e    	tst	0x17e
    c690:	27 18       	beq	0x0xc6aa
    c692:	86 0e       	ldaa	#0xe
    c694:	b1 01 1e    	cmpa	0x11e
    c697:	26 02       	bne	0x0xc69b
    c699:	86 1e       	ldaa	#0x1e
    c69b:	b7 01 1e    	staa	0x11e
    c69e:	bd dd 89    	jsr	0xdd89
    c6a1:	7f 01 7e    	clr	0x17e
    c6a4:	7f 01 1a    	clr	0x11a
    c6a7:	7e ca 55    	jmp	0xca55
    c6aa:	7d 01 1a    	tst	0x11a
    c6ad:	26 03       	bne	0x0xc6b2
    c6af:	7e ca 55    	jmp	0xca55
    c6b2:	c6 0e       	ldab	#0xe
    c6b4:	f1 01 1e    	cmpb	0x11e
    c6b7:	26 1a       	bne	0x0xc6d3
    c6b9:	96 b0       	ldaa	*0xb0
    c6bb:	7d 01 1a    	tst	0x11a
    c6be:	2b 0e       	bmi	0x0xc6ce
    c6c0:	4c          	inca
    c6c1:	84 7f       	anda	#0x7f
    c6c3:	97 b0       	staa	*0xb0
    c6c5:	18 ce 00 b0 	ldy	#0xb0
    c6c9:	ce 01 2c    	ldx	#0x12c
    c6cc:	20 1f       	bra	0x0xc6ed
    c6ce:	4a          	deca
    c6cf:	84 7f       	anda	#0x7f
    c6d1:	20 f0       	bra	0x0xc6c3
    c6d3:	96 b1       	ldaa	*0xb1
    c6d5:	7d 01 1a    	tst	0x11a
    c6d8:	2b 0e       	bmi	0x0xc6e8
    c6da:	4c          	inca
    c6db:	84 7f       	anda	#0x7f
    c6dd:	97 b1       	staa	*0xb1
    c6df:	18 ce 00 b1 	ldy	#0xb1
    c6e3:	ce 01 3c    	ldx	#0x13c
    c6e6:	20 05       	bra	0x0xc6ed
    c6e8:	4a          	deca
    c6e9:	84 7f       	anda	#0x7f
    c6eb:	20 f0       	bra	0x0xc6dd
    c6ed:	bd de e4    	jsr	0xdee4
    c6f0:	bd c6 fe    	jsr	0xc6fe
    c6f3:	7f 01 1a    	clr	0x11a
    c6f6:	86 03       	ldaa	#0x3
    c6f8:	b7 01 1c    	staa	0x11c
    c6fb:	7e ca 55    	jmp	0xca55
    c6fe:	18 a6 00    	ldaa	0x0,y
    c701:	36          	psha
    c702:	18 8f       	xgdy
    c704:	37          	pshb
    c705:	c0 a8       	subb	#0xa8
    c707:	c4 07       	andb	#0x7
    c709:	5c          	incb
    c70a:	86 01       	ldaa	#0x1
    c70c:	5a          	decb
    c70d:	27 03       	beq	0x0xc712
    c70f:	48          	asla
    c710:	20 fa       	bra	0x0xc70c
    c712:	43          	coma
    c713:	7d 00 d0    	tst	0xd0
    c716:	27 05       	beq	0x0xc71d
    c718:	7f 00 d0    	clr	0xd0
    c71b:	20 08       	bra	0x0xc725
    c71d:	7d 10 29    	tst	0x1029
    c720:	2a fb       	bpl	0x0xc71d
    c722:	f6 10 2a    	ldab	0x102a
    c725:	b7 10 42    	staa	0x1042
    c728:	86 83       	ldaa	#0x83
    c72a:	b7 10 2a    	staa	0x102a
    c72d:	32          	pula
    c72e:	81 b8       	cmpa	#0xb8
    c730:	25 04       	bcs	0x0xc736
    c732:	86 8a       	ldaa	#0x8a
    c734:	20 0a       	bra	0x0xc740
    c736:	81 b0       	cmpa	#0xb0
    c738:	25 04       	bcs	0x0xc73e
    c73a:	86 89       	ldaa	#0x89
    c73c:	20 02       	bra	0x0xc740
    c73e:	86 88       	ldaa	#0x88
    c740:	7d 10 29    	tst	0x1029
    c743:	2a fb       	bpl	0x0xc740
    c745:	f6 10 2a    	ldab	0x102a
    c748:	b7 10 2a    	staa	0x102a
    c74b:	32          	pula
    c74c:	7d 10 29    	tst	0x1029
    c74f:	2a fb       	bpl	0x0xc74c
    c751:	f6 10 2a    	ldab	0x102a
    c754:	b7 10 2a    	staa	0x102a
    c757:	39          	rts
    c758:	7d 01 1c    	tst	0x11c
    c75b:	2a 03       	bpl	0x0xc760
    c75d:	7e db 8e    	jmp	0xdb8e
    c760:	7d 01 1e    	tst	0x11e
    c763:	26 08       	bne	0x0xc76d
    c765:	7d 01 1c    	tst	0x11c
    c768:	27 1f       	beq	0x0xc789
    c76a:	7e a9 6a    	jmp	0xa96a
    c76d:	7d 01 1c    	tst	0x11c
    c770:	27 24       	beq	0x0xc796
    c772:	cc 01 20    	ldd	#0x120
    c775:	fb 01 1e    	addb	0x11e
    c778:	8f          	xgdx
    c779:	bd dd 4d    	jsr	0xdd4d
    c77c:	7a 01 1e    	dec	0x11e
    c77f:	7a 01 1c    	dec	0x11c
    c782:	26 12       	bne	0x0xc796
    c784:	bd dd fe    	jsr	0xddfe
    c787:	20 0d       	bra	0x0xc796
    c789:	7d 01 1e    	tst	0x11e
    c78c:	26 08       	bne	0x0xc796
    c78e:	86 0e       	ldaa	#0xe
    c790:	b7 01 1e    	staa	0x11e
    c793:	bd dd 89    	jsr	0xdd89
    c796:	7d 01 7f    	tst	0x17f
    c799:	27 0d       	beq	0x0xc7a8
    c79b:	4f          	clra
    c79c:	b7 01 7f    	staa	0x17f
    c79f:	b7 01 7e    	staa	0x17e
    c7a2:	b7 01 1a    	staa	0x11a
    c7a5:	7e ca 55    	jmp	0xca55
    c7a8:	7d 01 7e    	tst	0x17e
    c7ab:	27 18       	beq	0x0xc7c5
    c7ad:	86 0e       	ldaa	#0xe
    c7af:	b1 01 1e    	cmpa	0x11e
    c7b2:	26 02       	bne	0x0xc7b6
    c7b4:	86 1e       	ldaa	#0x1e
    c7b6:	b7 01 1e    	staa	0x11e
    c7b9:	bd dd 89    	jsr	0xdd89
    c7bc:	7f 01 7e    	clr	0x17e
    c7bf:	7f 01 1a    	clr	0x11a
    c7c2:	7e ca 55    	jmp	0xca55
    c7c5:	7d 01 1a    	tst	0x11a
    c7c8:	26 03       	bne	0x0xc7cd
    c7ca:	7e ca 55    	jmp	0xca55
    c7cd:	c6 0e       	ldab	#0xe
    c7cf:	f1 01 1e    	cmpb	0x11e
    c7d2:	26 1a       	bne	0x0xc7ee
    c7d4:	96 a8       	ldaa	*0xa8
    c7d6:	7d 01 1a    	tst	0x11a
    c7d9:	2b 0e       	bmi	0x0xc7e9
    c7db:	4c          	inca
    c7dc:	84 7f       	anda	#0x7f
    c7de:	97 a8       	staa	*0xa8
    c7e0:	18 ce 00 a8 	ldy	#0xa8
    c7e4:	ce 01 2c    	ldx	#0x12c
    c7e7:	20 1f       	bra	0x0xc808
    c7e9:	4a          	deca
    c7ea:	84 7f       	anda	#0x7f
    c7ec:	20 f0       	bra	0x0xc7de
    c7ee:	96 a9       	ldaa	*0xa9
    c7f0:	7d 01 1a    	tst	0x11a
    c7f3:	2b 0e       	bmi	0x0xc803
    c7f5:	4c          	inca
    c7f6:	84 7f       	anda	#0x7f
    c7f8:	97 a9       	staa	*0xa9
    c7fa:	18 ce 00 a9 	ldy	#0xa9
    c7fe:	ce 01 3c    	ldx	#0x13c
    c801:	20 05       	bra	0x0xc808
    c803:	4a          	deca
    c804:	84 7f       	anda	#0x7f
    c806:	20 f0       	bra	0x0xc7f8
    c808:	bd de a8    	jsr	0xdea8
    c80b:	bd c6 fe    	jsr	0xc6fe
    c80e:	7f 01 1a    	clr	0x11a
    c811:	86 03       	ldaa	#0x3
    c813:	b7 01 1c    	staa	0x11c
    c816:	7e ca 55    	jmp	0xca55
    c819:	7d 01 1c    	tst	0x11c
    c81c:	2a 03       	bpl	0x0xc821
    c81e:	7e db d8    	jmp	0xdbd8
    c821:	7d 01 1e    	tst	0x11e
    c824:	26 08       	bne	0x0xc82e
    c826:	7d 01 1c    	tst	0x11c
    c829:	27 1f       	beq	0x0xc84a
    c82b:	7e a9 6a    	jmp	0xa96a
    c82e:	7d 01 1c    	tst	0x11c
    c831:	27 24       	beq	0x0xc857
    c833:	cc 01 20    	ldd	#0x120
    c836:	fb 01 1e    	addb	0x11e
    c839:	8f          	xgdx
    c83a:	bd dd 4d    	jsr	0xdd4d
    c83d:	7a 01 1e    	dec	0x11e
    c840:	7a 01 1c    	dec	0x11c
    c843:	26 12       	bne	0x0xc857
    c845:	bd dd fe    	jsr	0xddfe
    c848:	20 0d       	bra	0x0xc857
    c84a:	7d 01 1e    	tst	0x11e
    c84d:	26 08       	bne	0x0xc857
    c84f:	86 0e       	ldaa	#0xe
    c851:	b7 01 1e    	staa	0x11e
    c854:	bd dd 89    	jsr	0xdd89
    c857:	7d 01 7f    	tst	0x17f
    c85a:	27 0d       	beq	0x0xc869
    c85c:	4f          	clra
    c85d:	b7 01 7f    	staa	0x17f
    c860:	b7 01 7e    	staa	0x17e
    c863:	b7 01 1a    	staa	0x11a
    c866:	7e ca 55    	jmp	0xca55
    c869:	7d 01 7e    	tst	0x17e
    c86c:	27 18       	beq	0x0xc886
    c86e:	86 0e       	ldaa	#0xe
    c870:	b1 01 1e    	cmpa	0x11e
    c873:	26 02       	bne	0x0xc877
    c875:	86 1e       	ldaa	#0x1e
    c877:	b7 01 1e    	staa	0x11e
    c87a:	bd dd 89    	jsr	0xdd89
    c87d:	7f 01 7e    	clr	0x17e
    c880:	7f 01 1a    	clr	0x11a
    c883:	7e ca 55    	jmp	0xca55
    c886:	7d 01 1a    	tst	0x11a
    c889:	26 03       	bne	0x0xc88e
    c88b:	7e ca 55    	jmp	0xca55
    c88e:	c6 0e       	ldab	#0xe
    c890:	f1 01 1e    	cmpb	0x11e
    c893:	26 1a       	bne	0x0xc8af
    c895:	96 b8       	ldaa	*0xb8
    c897:	7d 01 1a    	tst	0x11a
    c89a:	2b 0e       	bmi	0x0xc8aa
    c89c:	4c          	inca
    c89d:	84 7f       	anda	#0x7f
    c89f:	97 b8       	staa	*0xb8
    c8a1:	18 ce 00 b8 	ldy	#0xb8
    c8a5:	ce 01 2c    	ldx	#0x12c
    c8a8:	20 1f       	bra	0x0xc8c9
    c8aa:	4a          	deca
    c8ab:	84 7f       	anda	#0x7f
    c8ad:	20 f0       	bra	0x0xc89f
    c8af:	96 b9       	ldaa	*0xb9
    c8b1:	7d 01 1a    	tst	0x11a
    c8b4:	2b 0e       	bmi	0x0xc8c4
    c8b6:	4c          	inca
    c8b7:	84 7f       	anda	#0x7f
    c8b9:	97 b9       	staa	*0xb9
    c8bb:	18 ce 00 b9 	ldy	#0xb9
    c8bf:	ce 01 3c    	ldx	#0x13c
    c8c2:	20 05       	bra	0x0xc8c9
    c8c4:	4a          	deca
    c8c5:	84 7f       	anda	#0x7f
    c8c7:	20 f0       	bra	0x0xc8b9
    c8c9:	bd de a8    	jsr	0xdea8
    c8cc:	bd c6 fe    	jsr	0xc6fe
    c8cf:	7f 01 1a    	clr	0x11a
    c8d2:	86 03       	ldaa	#0x3
    c8d4:	b7 01 1c    	staa	0x11c
    c8d7:	7e ca 55    	jmp	0xca55
    c8da:	7d 01 1c    	tst	0x11c
    c8dd:	2a 03       	bpl	0x0xc8e2
    c8df:	7e dc 22    	jmp	0xdc22
    c8e2:	7d 01 1e    	tst	0x11e
    c8e5:	26 08       	bne	0x0xc8ef
    c8e7:	7d 01 1c    	tst	0x11c
    c8ea:	27 1f       	beq	0x0xc90b
    c8ec:	7e a9 6a    	jmp	0xa96a
    c8ef:	7d 01 1c    	tst	0x11c
    c8f2:	27 24       	beq	0x0xc918
    c8f4:	cc 01 20    	ldd	#0x120
    c8f7:	fb 01 1e    	addb	0x11e
    c8fa:	8f          	xgdx
    c8fb:	bd dd 4d    	jsr	0xdd4d
    c8fe:	7a 01 1e    	dec	0x11e
    c901:	7a 01 1c    	dec	0x11c
    c904:	26 12       	bne	0x0xc918
    c906:	bd dd cb    	jsr	0xddcb
    c909:	20 0d       	bra	0x0xc918
    c90b:	7d 01 1e    	tst	0x11e
    c90e:	26 08       	bne	0x0xc918
    c910:	86 13       	ldaa	#0x13
    c912:	b7 01 1e    	staa	0x11e
    c915:	bd dd 89    	jsr	0xdd89
    c918:	7d 01 7f    	tst	0x17f
    c91b:	27 10       	beq	0x0xc92d
    c91d:	bd df d4    	jsr	0xdfd4
    c920:	4f          	clra
    c921:	b7 01 7f    	staa	0x17f
    c924:	b7 01 7e    	staa	0x17e
    c927:	b7 01 1a    	staa	0x11a
    c92a:	7e ca 55    	jmp	0xca55
    c92d:	7d 01 1a    	tst	0x11a
    c930:	26 03       	bne	0x0xc935
    c932:	7e ca 55    	jmp	0xca55
    c935:	b6 01 1e    	ldaa	0x11e
    c938:	81 13       	cmpa	#0x13
    c93a:	26 23       	bne	0x0xc95f
    c93c:	ce da 5d    	ldx	#0xda5d
    c93f:	18 ce 01 30 	ldy	#0x130
    c943:	d6 a7       	ldab	*0xa7
    c945:	bd c4 0f    	jsr	0xc40f
    c948:	d7 a7       	stab	*0xa7
    c94a:	bd df 5b    	jsr	0xdf5b
    c94d:	7f 01 1a    	clr	0x11a
    c950:	86 04       	ldaa	#0x4
    c952:	b7 01 1c    	staa	0x11c
    c955:	96 a7       	ldaa	*0xa7
    c957:	c6 a7       	ldab	#0xa7
    c959:	bd a8 5a    	jsr	0xa85a
    c95c:	7e ca 55    	jmp	0xca55
    c95f:	81 19       	cmpa	#0x19
    c961:	26 34       	bne	0x0xc997
    c963:	ce db 17    	ldx	#0xdb17
    c966:	18 ce 01 37 	ldy	#0x137
    c96a:	7d 01 1a    	tst	0x11a
    c96d:	2b 20       	bmi	0x0xc98f
    c96f:	d6 a6       	ldab	*0xa6
    c971:	5c          	incb
    c972:	c1 03       	cmpb	#0x3
    c974:	25 02       	bcs	0x0xc978
    c976:	c6 02       	ldab	#0x2
    c978:	d7 a6       	stab	*0xa6
    c97a:	bd df 47    	jsr	0xdf47
    c97d:	7f 01 1a    	clr	0x11a
    c980:	86 03       	ldaa	#0x3
    c982:	b7 01 1c    	staa	0x11c
    c985:	96 a6       	ldaa	*0xa6
    c987:	c6 a6       	ldab	#0xa6
    c989:	bd a8 5a    	jsr	0xa85a
    c98c:	7e ca 55    	jmp	0xca55
    c98f:	d6 a6       	ldab	*0xa6
    c991:	5a          	decb
    c992:	2a e4       	bpl	0x0xc978
    c994:	5f          	clrb
    c995:	20 e1       	bra	0x0xc978
    c997:	ce da 7d    	ldx	#0xda7d
    c99a:	18 ce 01 3b 	ldy	#0x13b
    c99e:	d6 a5       	ldab	*0xa5
    c9a0:	bd c4 89    	jsr	0xc489
    c9a3:	d7 a5       	stab	*0xa5
    c9a5:	bd df 5b    	jsr	0xdf5b
    c9a8:	7f 01 1a    	clr	0x11a
    c9ab:	86 04       	ldaa	#0x4
    c9ad:	b7 01 1c    	staa	0x11c
    c9b0:	96 a5       	ldaa	*0xa5
    c9b2:	c6 a5       	ldab	#0xa5
    c9b4:	bd a8 5a    	jsr	0xa85a
    c9b7:	7e ca 55    	jmp	0xca55
    c9ba:	7d 01 1c    	tst	0x11c
    c9bd:	2a 03       	bpl	0x0xc9c2
    c9bf:	7e dc 80    	jmp	0xdc80
    c9c2:	7d 01 1e    	tst	0x11e
    c9c5:	26 08       	bne	0x0xc9cf
    c9c7:	7d 01 1c    	tst	0x11c
    c9ca:	27 1f       	beq	0x0xc9eb
    c9cc:	7e a9 6a    	jmp	0xa96a
    c9cf:	7d 01 1c    	tst	0x11c
    c9d2:	27 24       	beq	0x0xc9f8
    c9d4:	cc 01 20    	ldd	#0x120
    c9d7:	fb 01 1e    	addb	0x11e
    c9da:	8f          	xgdx
    c9db:	bd dd 4d    	jsr	0xdd4d
    c9de:	7a 01 1e    	dec	0x11e
    c9e1:	7a 01 1c    	dec	0x11c
    c9e4:	26 12       	bne	0x0xc9f8
    c9e6:	bd dd cb    	jsr	0xddcb
    c9e9:	20 0d       	bra	0x0xc9f8
    c9eb:	7d 01 1e    	tst	0x11e
    c9ee:	26 08       	bne	0x0xc9f8
    c9f0:	86 13       	ldaa	#0x13
    c9f2:	b7 01 1e    	staa	0x11e
    c9f5:	bd dd 89    	jsr	0xdd89
    c9f8:	7d 01 7f    	tst	0x17f
    c9fb:	27 0d       	beq	0x0xca0a
    c9fd:	4f          	clra
    c9fe:	b7 01 7f    	staa	0x17f
    ca01:	b7 01 7e    	staa	0x17e
    ca04:	b7 01 1a    	staa	0x11a
    ca07:	7e ca 55    	jmp	0xca55
    ca0a:	7d 01 1a    	tst	0x11a
    ca0d:	26 03       	bne	0x0xca12
    ca0f:	7e ca 55    	jmp	0xca55
    ca12:	7d 01 1a    	tst	0x11a
    ca15:	2b 2d       	bmi	0x0xca44
    ca17:	b6 10 28    	ldaa	0x1028
    ca1a:	84 03       	anda	#0x3
    ca1c:	4a          	deca
    ca1d:	2a 01       	bpl	0x0xca20
    ca1f:	4f          	clra
    ca20:	b7 7f fb    	staa	0x7ffb
    ca23:	f6 10 28    	ldab	0x1028
    ca26:	c4 fc       	andb	#0xfc
    ca28:	1b          	aba
    ca29:	b7 10 28    	staa	0x1028
    ca2c:	ce dc d8    	ldx	#0xdcd8
    ca2f:	18 ce 01 30 	ldy	#0x130
    ca33:	16          	tab
    ca34:	c4 03       	andb	#0x3
    ca36:	bd df 5b    	jsr	0xdf5b
    ca39:	7f 01 1a    	clr	0x11a
    ca3c:	86 04       	ldaa	#0x4
    ca3e:	b7 01 1c    	staa	0x11c
    ca41:	7e ca 55    	jmp	0xca55
    ca44:	b6 10 28    	ldaa	0x1028
    ca47:	84 03       	anda	#0x3
    ca49:	4c          	inca
    ca4a:	81 04       	cmpa	#0x4
    ca4c:	25 d2       	bcs	0x0xca20
    ca4e:	86 03       	ldaa	#0x3
    ca50:	20 ce       	bra	0x0xca20
    ca52:	7e ca 55    	jmp	0xca55
    ca55:	fe 01 c0    	ldx	0x1c0
    ca58:	bc 01 c2    	cpx	0x1c2
    ca5b:	26 03       	bne	0x0xca60
    ca5d:	7e 93 14    	jmp	0x9314
    ca60:	18 fe 01 6c 	ldy	0x16c
    ca64:	18 3c       	pshy
    ca66:	7e 8e d5    	jmp	0x8ed5
    ca69:	7f 00 ff    	clr	0xff
    ca6c:	7d 01 1d    	tst	0x11d
    ca6f:	27 2f       	beq	0x0xcaa0
    ca71:	ce 10 23    	ldx	#0x1023
    ca74:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xca74
    ca78:	86 f7       	ldaa	#0xf7
    ca7a:	b4 10 00    	anda	0x1000
    ca7d:	b7 10 00    	staa	0x1000
    ca80:	86 0c       	ldaa	#0xc
    ca82:	b7 10 47    	staa	0x1047
    ca85:	86 80       	ldaa	#0x80
    ca87:	ba 10 00    	oraa	0x1000
    ca8a:	b7 10 00    	staa	0x1000
    ca8d:	01          	nop
    ca8e:	88 80       	eora	#0x80
    ca90:	b7 10 00    	staa	0x1000
    ca93:	7f 01 1d    	clr	0x11d
    ca96:	bd dd 7a    	jsr	0xdd7a
    ca99:	ce 10 23    	ldx	#0x1023
    ca9c:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xca9c
    caa0:	bd dd ae    	jsr	0xddae
    caa3:	ce 01 20    	ldx	#0x120
    caa6:	c6 10       	ldab	#0x10
    caa8:	96 f9       	ldaa	*0xf9
    caaa:	81 02       	cmpa	#0x2
    caac:	25 06       	bcs	0x0xcab4
    caae:	18 ce cb 1e 	ldy	#0xcb1e
    cab2:	20 04       	bra	0x0xcab8
    cab4:	18 ce cb 0e 	ldy	#0xcb0e
    cab8:	18 a6 00    	ldaa	0x0,y
    cabb:	a7 00       	staa	0x0,x
    cabd:	08          	inx
    cabe:	18 08       	iny
    cac0:	5a          	decb
    cac1:	26 f5       	bne	0x0xcab8
    cac3:	96 f9       	ldaa	*0xf9
    cac5:	27 0b       	beq	0x0xcad2
    cac7:	81 02       	cmpa	#0x2
    cac9:	27 2c       	beq	0x0xcaf7
    cacb:	86 42       	ldaa	#0x42
    cacd:	b7 01 2c    	staa	0x12c
    cad0:	20 05       	bra	0x0xcad7
    cad2:	86 41       	ldaa	#0x41
    cad4:	b7 01 2c    	staa	0x12c
    cad7:	96 fa       	ldaa	*0xfa
    cad9:	4c          	inca
    cada:	ce 01 2d    	ldx	#0x12d
    cadd:	bd de a8    	jsr	0xdea8
    cae0:	08          	inx
    cae1:	18 ce 00 c0 	ldy	#0xc0
    cae5:	c6 10       	ldab	#0x10
    cae7:	18 a6 00    	ldaa	0x0,y
    caea:	84 7f       	anda	#0x7f
    caec:	a7 00       	staa	0x0,x
    caee:	08          	inx
    caef:	18 08       	iny
    caf1:	5a          	decb
    caf2:	26 f3       	bne	0x0xcae7
    caf4:	7e dc eb    	jmp	0xdceb
    caf7:	b6 01 6a    	ldaa	0x16a
    cafa:	4c          	inca
    cafb:	ce 01 2d    	ldx	#0x12d
    cafe:	bd de a8    	jsr	0xdea8
    cb01:	08          	inx
    cb02:	cc 50 00    	ldd	#0x5000
    cb05:	c3 00 30    	addd	#0x30
    cb08:	18 8f       	xgdy
    cb0a:	c6 10       	ldab	#0x10
    cb0c:	20 d9       	bra	0x0xcae7
    cb0e:	50          	negb
    cb0f:	41          	.byte	0x41
    cb10:	54          	lsrb
    cb11:	43          	coma
    cb12:	48          	asla
    cb13:	20 20       	bra	0x0xcb35
    cb15:	20 20       	bra	0x0xcb37
    cb17:	20 20       	bra	0x0xcb39
    cb19:	20 20       	bra	0x0xcb3b
    cb1b:	20 20       	bra	0x0xcb3d
    cb1d:	20 4d       	bra	0x0xcb6c
    cb1f:	55          	.byte	0x55
    cb20:	4c          	inca
    cb21:	54          	lsrb
    cb22:	49          	rola
    cb23:	20 20       	bra	0x0xcb45
    cb25:	20 20       	bra	0x0xcb47
    cb27:	20 20       	bra	0x0xcb49
    cb29:	20 20       	bra	0x0xcb4b
    cb2b:	20 20       	bra	0x0xcb4d
    cb2d:	20 7d       	bra	0x0xcbac
    cb2f:	01          	nop
    cb30:	7e 27 67    	jmp	0x2767
    cb33:	7f 01 7e    	clr	0x17e
    cb36:	96 f9       	ldaa	*0xf9
    cb38:	27 26       	beq	0x0xcb60
    cb3a:	81 02       	cmpa	#0x2
    cb3c:	27 04       	beq	0x0xcb42
    cb3e:	86 42       	ldaa	#0x42
    cb40:	20 20       	bra	0x0xcb62
    cb42:	86 43       	ldaa	#0x43
    cb44:	b7 01 2c    	staa	0x12c
    cb47:	b6 01 6a    	ldaa	0x16a
    cb4a:	4c          	inca
    cb4b:	ce 01 2d    	ldx	#0x12d
    cb4e:	bd de a8    	jsr	0xdea8
    cb51:	b6 01 6a    	ldaa	0x16a
    cb54:	c6 40       	ldab	#0x40
    cb56:	3d          	mul
    cb57:	c3 20 00    	addd	#0x2000
    cb5a:	c3 00 30    	addd	#0x30
    cb5d:	8f          	xgdx
    cb5e:	20 1a       	bra	0x0xcb7a
    cb60:	86 41       	ldaa	#0x41
    cb62:	b7 01 2c    	staa	0x12c
    cb65:	96 fa       	ldaa	*0xfa
    cb67:	4c          	inca
    cb68:	ce 01 2d    	ldx	#0x12d
    cb6b:	bd de a8    	jsr	0xdea8
    cb6e:	96 fa       	ldaa	*0xfa
    cb70:	c6 b0       	ldab	#0xb0
    cb72:	3d          	mul
    cb73:	c3 20 00    	addd	#0x2000
    cb76:	c3 00 a0    	addd	#0xa0
    cb79:	8f          	xgdx
    cb7a:	18 ce 01 30 	ldy	#0x130
    cb7e:	c6 10       	ldab	#0x10
    cb80:	a6 00       	ldaa	0x0,x
    cb82:	84 7f       	anda	#0x7f
    cb84:	18 a7 00    	staa	0x0,y
    cb87:	08          	inx
    cb88:	18 08       	iny
    cb8a:	5a          	decb
    cb8b:	26 f3       	bne	0x0xcb80
    cb8d:	ce 10 23    	ldx	#0x1023
    cb90:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xcb90
    cb94:	bd dd ae    	jsr	0xddae
    cb97:	7e dc eb    	jmp	0xdceb
    cb9a:	7d 01 1d    	tst	0x11d
    cb9d:	26 0a       	bne	0x0xcba9
    cb9f:	bd df 1f    	jsr	0xdf1f
    cba2:	ce 10 23    	ldx	#0x1023
    cba5:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xcba5
    cba9:	bd dd ae    	jsr	0xddae
    cbac:	ce 01 20    	ldx	#0x120
    cbaf:	18 ce cc 08 	ldy	#0xcc08
    cbb3:	c6 20       	ldab	#0x20
    cbb5:	18 a6 00    	ldaa	0x0,y
    cbb8:	a7 00       	staa	0x0,x
    cbba:	08          	inx
    cbbb:	18 08       	iny
    cbbd:	5a          	decb
    cbbe:	26 f5       	bne	0x0xcbb5
    cbc0:	96 f9       	ldaa	*0xf9
    cbc2:	27 1b       	beq	0x0xcbdf
    cbc4:	81 02       	cmpa	#0x2
    cbc6:	26 0d       	bne	0x0xcbd5
    cbc8:	86 43       	ldaa	#0x43
    cbca:	b7 01 25    	staa	0x125
    cbcd:	b7 01 2c    	staa	0x12c
    cbd0:	b6 01 6a    	ldaa	0x16a
    cbd3:	20 14       	bra	0x0xcbe9
    cbd5:	86 42       	ldaa	#0x42
    cbd7:	b7 01 25    	staa	0x125
    cbda:	b7 01 2c    	staa	0x12c
    cbdd:	20 08       	bra	0x0xcbe7
    cbdf:	86 41       	ldaa	#0x41
    cbe1:	b7 01 25    	staa	0x125
    cbe4:	b7 01 2c    	staa	0x12c
    cbe7:	96 fa       	ldaa	*0xfa
    cbe9:	4c          	inca
    cbea:	ce 01 26    	ldx	#0x126
    cbed:	bd de a8    	jsr	0xdea8
    cbf0:	a7 07       	staa	0x7,x
    cbf2:	09          	dex
    cbf3:	a6 00       	ldaa	0x0,x
    cbf5:	a7 07       	staa	0x7,x
    cbf7:	09          	dex
    cbf8:	a6 00       	ldaa	0x0,x
    cbfa:	a7 07       	staa	0x7,x
    cbfc:	96 f9       	ldaa	*0xf9
    cbfe:	81 02       	cmpa	#0x2
    cc00:	27 03       	beq	0x0xcc05
    cc02:	7e ca d7    	jmp	0xcad7
    cc05:	7e ca f7    	jmp	0xcaf7
    cc08:	53          	comb
    cc09:	41          	.byte	0x41
    cc0a:	56          	rorb
    cc0b:	45          	.byte	0x45
    cc0c:	20 20       	bra	0x0xcc2e
    cc0e:	20 20       	bra	0x0xcc30
    cc10:	20 20       	bra	0x0xcc32
    cc12:	3e          	wai
    cc13:	20 20       	bra	0x0xcc35
    cc15:	20 20       	bra	0x0xcc37
    cc17:	20 20       	bra	0x0xcc39
    cc19:	20 20       	bra	0x0xcc3b
    cc1b:	20 20       	bra	0x0xcc3d
    cc1d:	20 20       	bra	0x0xcc3f
    cc1f:	20 20       	bra	0x0xcc41
    cc21:	20 20       	bra	0x0xcc43
    cc23:	20 20       	bra	0x0xcc45
    cc25:	20 20       	bra	0x0xcc47
    cc27:	20 7d       	bra	0x0xcca6
    cc29:	01          	nop
    cc2a:	1d 26 0a    	bclr	0x26,x, #0x0a
    cc2d:	bd df 1f    	jsr	0xdf1f
    cc30:	ce 10 23    	ldx	#0x1023
    cc33:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xcc33
    cc37:	bd dd ae    	jsr	0xddae
    cc3a:	ce 01 20    	ldx	#0x120
    cc3d:	18 ce cc 85 	ldy	#0xcc85
    cc41:	c6 20       	ldab	#0x20
    cc43:	96 f9       	ldaa	*0xf9
    cc45:	81 02       	cmpa	#0x2
    cc47:	26 04       	bne	0x0xcc4d
    cc49:	18 ce cc a5 	ldy	#0xcca5
    cc4d:	18 a6 00    	ldaa	0x0,y
    cc50:	a7 00       	staa	0x0,x
    cc52:	08          	inx
    cc53:	18 08       	iny
    cc55:	5a          	decb
    cc56:	26 f5       	bne	0x0xcc4d
    cc58:	96 f9       	ldaa	*0xf9
    cc5a:	27 0b       	beq	0x0xcc67
    cc5c:	81 02       	cmpa	#0x2
    cc5e:	27 18       	beq	0x0xcc78
    cc60:	86 42       	ldaa	#0x42
    cc62:	b7 01 2c    	staa	0x12c
    cc65:	20 05       	bra	0x0xcc6c
    cc67:	86 41       	ldaa	#0x41
    cc69:	b7 01 2c    	staa	0x12c
    cc6c:	96 fa       	ldaa	*0xfa
    cc6e:	4c          	inca
    cc6f:	ce 01 2d    	ldx	#0x12d
    cc72:	bd de a8    	jsr	0xdea8
    cc75:	7e ca e0    	jmp	0xcae0
    cc78:	b6 01 6a    	ldaa	0x16a
    cc7b:	4c          	inca
    cc7c:	ce 01 2d    	ldx	#0x12d
    cc7f:	bd de a8    	jsr	0xdea8
    cc82:	7e ca f7    	jmp	0xcaf7
    cc85:	4e          	.byte	0x4e
    cc86:	41          	.byte	0x41
    cc87:	4d          	tsta
    cc88:	45          	.byte	0x45
    cc89:	20 50       	bra	0x0xccdb
    cc8b:	41          	.byte	0x41
    cc8c:	54          	lsrb
    cc8d:	43          	coma
    cc8e:	48          	asla
    cc8f:	20 20       	bra	0x0xccb1
    cc91:	20 20       	bra	0x0xccb3
    cc93:	20 20       	bra	0x0xccb5
    cc95:	20 20       	bra	0x0xccb7
    cc97:	20 20       	bra	0x0xccb9
    cc99:	20 20       	bra	0x0xccbb
    cc9b:	20 20       	bra	0x0xccbd
    cc9d:	20 20       	bra	0x0xccbf
    cc9f:	20 20       	bra	0x0xccc1
    cca1:	20 20       	bra	0x0xccc3
    cca3:	20 20       	bra	0x0xccc5
    cca5:	4e          	.byte	0x4e
    cca6:	41          	.byte	0x41
    cca7:	4d          	tsta
    cca8:	45          	.byte	0x45
    cca9:	20 4d       	bra	0x0xccf8
    ccab:	55          	.byte	0x55
    ccac:	4c          	inca
    ccad:	54          	lsrb
    ccae:	49          	rola
    ccaf:	20 20       	bra	0x0xccd1
    ccb1:	20 20       	bra	0x0xccd3
    ccb3:	20 20       	bra	0x0xccd5
    ccb5:	20 20       	bra	0x0xccd7
    ccb7:	20 20       	bra	0x0xccd9
    ccb9:	20 20       	bra	0x0xccdb
    ccbb:	20 20       	bra	0x0xccdd
    ccbd:	20 20       	bra	0x0xccdf
    ccbf:	20 20       	bra	0x0xcce1
    ccc1:	20 20       	bra	0x0xcce3
    ccc3:	20 20       	bra	0x0xcce5
    ccc5:	bd dd ae    	jsr	0xddae
    ccc8:	ce 01 20    	ldx	#0x120
    cccb:	18 ce cd 03 	ldy	#0xcd03
    cccf:	c6 20       	ldab	#0x20
    ccd1:	18 a6 00    	ldaa	0x0,y
    ccd4:	a7 00       	staa	0x0,x
    ccd6:	08          	inx
    ccd7:	18 08       	iny
    ccd9:	5a          	decb
    ccda:	26 f5       	bne	0x0xccd1
    ccdc:	ce cd 23    	ldx	#0xcd23
    ccdf:	f6 01 6f    	ldab	0x16f
    cce2:	18 ce 01 30 	ldy	#0x130
    cce6:	bd df 5b    	jsr	0xdf5b
    cce9:	ce cd 67    	ldx	#0xcd67
    ccec:	d6 93       	ldab	*0x93
    ccee:	18 ce 01 36 	ldy	#0x136
    ccf2:	bd df 5b    	jsr	0xdf5b
    ccf5:	ce cd 7b    	ldx	#0xcd7b
    ccf8:	5f          	clrb
    ccf9:	18 ce 01 3d 	ldy	#0x13d
    ccfd:	bd df 47    	jsr	0xdf47
    cd00:	7e dc eb    	jmp	0xdceb
    cd03:	43          	coma
    cd04:	48          	asla
    cd05:	41          	.byte	0x41
    cd06:	4e          	.byte	0x4e
    cd07:	20 20       	bra	0x0xcd29
    cd09:	54          	lsrb
    cd0a:	55          	.byte	0x55
    cd0b:	4e          	.byte	0x4e
    cd0c:	45          	.byte	0x45
    cd0d:	20 20       	bra	0x0xcd2f
    cd0f:	49          	rola
    cd10:	4e          	.byte	0x4e
    cd11:	49          	rola
    cd12:	54          	lsrb
    cd13:	20 20       	bra	0x0xcd35
    cd15:	20 20       	bra	0x0xcd37
    cd17:	20 20       	bra	0x0xcd39
    cd19:	20 20       	bra	0x0xcd3b
    cd1b:	20 20       	bra	0x0xcd3d
    cd1d:	20 20       	bra	0x0xcd3f
    cd1f:	20 20       	bra	0x0xcd41
    cd21:	20 20       	bra	0x0xcd43
    cd23:	20 20       	bra	0x0xcd45
    cd25:	20 31       	bra	0x0xcd58
    cd27:	20 20       	bra	0x0xcd49
    cd29:	20 32       	bra	0x0xcd5d
    cd2b:	20 20       	bra	0x0xcd4d
    cd2d:	20 33       	bra	0x0xcd62
    cd2f:	20 20       	bra	0x0xcd51
    cd31:	20 34       	bra	0x0xcd67
    cd33:	20 20       	bra	0x0xcd55
    cd35:	20 35       	bra	0x0xcd6c
    cd37:	20 20       	bra	0x0xcd59
    cd39:	20 36       	bra	0x0xcd71
    cd3b:	20 20       	bra	0x0xcd5d
    cd3d:	20 37       	bra	0x0xcd76
    cd3f:	20 20       	bra	0x0xcd61
    cd41:	20 38       	bra	0x0xcd7b
    cd43:	20 20       	bra	0x0xcd65
    cd45:	20 39       	bra	0x0xcd80
    cd47:	20 20       	bra	0x0xcd69
    cd49:	31          	ins
    cd4a:	30          	tsx
    cd4b:	20 20       	bra	0x0xcd6d
    cd4d:	31          	ins
    cd4e:	31          	ins
    cd4f:	20 20       	bra	0x0xcd71
    cd51:	31          	ins
    cd52:	32          	pula
    cd53:	20 20       	bra	0x0xcd75
    cd55:	31          	ins
    cd56:	33          	pulb
    cd57:	20 20       	bra	0x0xcd79
    cd59:	31          	ins
    cd5a:	34          	des
    cd5b:	20 20       	bra	0x0xcd7d
    cd5d:	31          	ins
    cd5e:	35          	txs
    cd5f:	20 20       	bra	0x0xcd81
    cd61:	31          	ins
    cd62:	36          	psha
    cd63:	4f          	clra
    cd64:	4d          	tsta
    cd65:	4e          	.byte	0x4e
    cd66:	49          	rola
    cd67:	20 4f       	bra	0x0xcdb8
    cd69:	46          	rora
    cd6a:	46          	rora
    cd6b:	20 39       	bra	0x0xcda6
    cd6d:	30          	tsx
    cd6e:	25 20       	bcs	0x0xcd90
    cd70:	39          	rts
    cd71:	35          	txs
    cd72:	25 20       	bcs	0x0xcd94
    cd74:	39          	rts
    cd75:	38          	pulx
    cd76:	25 31       	bcs	0x0xcda9
    cd78:	30          	tsx
    cd79:	30          	tsx
    cd7a:	25 20       	bcs	0x0xcd9c
    cd7c:	4e          	.byte	0x4e
    cd7d:	4f          	clra
    cd7e:	59          	rolb
    cd7f:	45          	.byte	0x45
    cd80:	53          	comb
    cd81:	bd dd ae    	jsr	0xddae
    cd84:	ce 01 20    	ldx	#0x120
    cd87:	18 ce cd bd 	ldy	#0xcdbd
    cd8b:	c6 20       	ldab	#0x20
    cd8d:	18 a6 00    	ldaa	0x0,y
    cd90:	a7 00       	staa	0x0,x
    cd92:	08          	inx
    cd93:	18 08       	iny
    cd95:	5a          	decb
    cd96:	26 f5       	bne	0x0xcd8d
    cd98:	ce cd dd    	ldx	#0xcddd
    cd9b:	f6 01 70    	ldab	0x170
    cd9e:	18 ce 01 30 	ldy	#0x130
    cda2:	bd df 5b    	jsr	0xdf5b
    cda5:	ce cd e9    	ldx	#0xcde9
    cda8:	d6 94       	ldab	*0x94
    cdaa:	18 ce 01 36 	ldy	#0x136
    cdae:	bd df 5b    	jsr	0xdf5b
    cdb1:	b6 01 71    	ldaa	0x171
    cdb4:	ce 01 3c    	ldx	#0x13c
    cdb7:	bd de a8    	jsr	0xdea8
    cdba:	7e dc eb    	jmp	0xdceb
    cdbd:	4d          	tsta
    cdbe:	50          	negb
    cdbf:	52          	.byte	0x52
    cdc0:	4f          	clra
    cdc1:	20 20       	bra	0x0xcde3
    cdc3:	4b          	.byte	0x4b
    cdc4:	4e          	.byte	0x4e
    cdc5:	4f          	clra
    cdc6:	42          	.byte	0x42
    cdc7:	20 20       	bra	0x0xcde9
    cdc9:	4c          	inca
    cdca:	43          	coma
    cdcb:	44          	lsra
    cdcc:	20 20       	bra	0x0xcdee
    cdce:	20 20       	bra	0x0xcdf0
    cdd0:	20 20       	bra	0x0xcdf2
    cdd2:	20 20       	bra	0x0xcdf4
    cdd4:	20 20       	bra	0x0xcdf6
    cdd6:	20 20       	bra	0x0xcdf8
    cdd8:	20 20       	bra	0x0xcdfa
    cdda:	20 20       	bra	0x0xcdfc
    cddc:	20 20       	bra	0x0xcdfe
    cdde:	4f          	clra
    cddf:	46          	rora
    cde0:	46          	rora
    cde1:	4f          	clra
    cde2:	4e          	.byte	0x4e
    cde3:	20 31       	bra	0x0xce16
    cde5:	4f          	clra
    cde6:	4e          	.byte	0x4e
    cde7:	20 32       	bra	0x0xce1b
    cde9:	4a          	deca
    cdea:	55          	.byte	0x55
    cdeb:	4d          	tsta
    cdec:	50          	negb
    cded:	45          	.byte	0x45
    cdee:	44          	lsra
    cdef:	49          	rola
    cdf0:	54          	lsrb
    cdf1:	4d          	tsta
    cdf2:	54          	lsrb
    cdf3:	43          	coma
    cdf4:	48          	asla
    cdf5:	bd dd ae    	jsr	0xddae
    cdf8:	96 f9       	ldaa	*0xf9
    cdfa:	81 02       	cmpa	#0x2
    cdfc:	27 0b       	beq	0x0xce09
    cdfe:	ce 01 20    	ldx	#0x120
    ce01:	18 ce ce 4b 	ldy	#0xce4b
    ce05:	c6 20       	ldab	#0x20
    ce07:	20 09       	bra	0x0xce12
    ce09:	ce 01 20    	ldx	#0x120
    ce0c:	18 ce ce 6b 	ldy	#0xce6b
    ce10:	c6 20       	ldab	#0x20
    ce12:	18 a6 00    	ldaa	0x0,y
    ce15:	a7 00       	staa	0x0,x
    ce17:	08          	inx
    ce18:	18 08       	iny
    ce1a:	5a          	decb
    ce1b:	26 f5       	bne	0x0xce12
    ce1d:	96 f9       	ldaa	*0xf9
    ce1f:	81 02       	cmpa	#0x2
    ce21:	26 0c       	bne	0x0xce2f
    ce23:	b6 01 6a    	ldaa	0x16a
    ce26:	4c          	inca
    ce27:	ce 01 3c    	ldx	#0x13c
    ce2a:	bd de a8    	jsr	0xdea8
    ce2d:	20 19       	bra	0x0xce48
    ce2f:	96 f9       	ldaa	*0xf9
    ce31:	27 07       	beq	0x0xce3a
    ce33:	86 42       	ldaa	#0x42
    ce35:	b7 01 3b    	staa	0x13b
    ce38:	20 05       	bra	0x0xce3f
    ce3a:	86 41       	ldaa	#0x41
    ce3c:	b7 01 3b    	staa	0x13b
    ce3f:	96 fa       	ldaa	*0xfa
    ce41:	4c          	inca
    ce42:	ce 01 3c    	ldx	#0x13c
    ce45:	bd de a8    	jsr	0xdea8
    ce48:	7e dc eb    	jmp	0xdceb
    ce4b:	53          	comb
    ce4c:	59          	rolb
    ce4d:	53          	comb
    ce4e:	58          	aslb
    ce4f:	20 53       	bra	0x0xcea4
    ce51:	45          	.byte	0x45
    ce52:	4e          	.byte	0x4e
    ce53:	44          	lsra
    ce54:	2c 50       	bge	0x0xcea6
    ce56:	49          	rola
    ce57:	43          	coma
    ce58:	4b          	.byte	0x4b
    ce59:	23 2c       	bls	0x0xce87
    ce5b:	50          	negb
    ce5c:	52          	.byte	0x52
    ce5d:	45          	.byte	0x45
    ce5e:	53          	comb
    ce5f:	53          	comb
    ce60:	20 53       	bra	0x0xceb5
    ce62:	41          	.byte	0x41
    ce63:	56          	rorb
    ce64:	45          	.byte	0x45
    ce65:	20 20       	bra	0x0xce87
    ce67:	20 20       	bra	0x0xce89
    ce69:	20 20       	bra	0x0xce8b
    ce6b:	53          	comb
    ce6c:	45          	.byte	0x45
    ce6d:	4c          	inca
    ce6e:	45          	.byte	0x45
    ce6f:	43          	coma
    ce70:	54          	lsrb
    ce71:	20 4d       	bra	0x0xcec0
    ce73:	55          	.byte	0x55
    ce74:	4c          	inca
    ce75:	54          	lsrb
    ce76:	49          	rola
    ce77:	2c 20       	bge	0x0xce99
    ce79:	20 20       	bra	0x0xce9b
    ce7b:	50          	negb
    ce7c:	52          	.byte	0x52
    ce7d:	45          	.byte	0x45
    ce7e:	53          	comb
    ce7f:	53          	comb
    ce80:	20 53       	bra	0x0xced5
    ce82:	41          	.byte	0x41
    ce83:	56          	rorb
    ce84:	45          	.byte	0x45
    ce85:	20 20       	bra	0x0xcea7
    ce87:	20 20       	bra	0x0xcea9
    ce89:	20 20       	bra	0x0xceab
    ce8b:	7d 01 1d    	tst	0x11d
    ce8e:	26 0a       	bne	0x0xce9a
    ce90:	bd df 1f    	jsr	0xdf1f
    ce93:	ce 10 23    	ldx	#0x1023
    ce96:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xce96
    ce9a:	bd dd ae    	jsr	0xddae
    ce9d:	ce 01 20    	ldx	#0x120
    cea0:	18 ce ce cd 	ldy	#0xcecd
    cea4:	c6 20       	ldab	#0x20
    cea6:	18 a6 00    	ldaa	0x0,y
    cea9:	a7 00       	staa	0x0,x
    ceab:	08          	inx
    ceac:	18 08       	iny
    ceae:	5a          	decb
    ceaf:	26 f5       	bne	0x0xcea6
    ceb1:	ce 01 31    	ldx	#0x131
    ceb4:	b6 01 6e    	ldaa	0x16e
    ceb7:	bd de e4    	jsr	0xdee4
    ceba:	ce 01 37    	ldx	#0x137
    cebd:	96 3e       	ldaa	*0x3e
    cebf:	bd de a8    	jsr	0xdea8
    cec2:	08          	inx
    cec3:	08          	inx
    cec4:	08          	inx
    cec5:	96 3f       	ldaa	*0x3f
    cec7:	bd de a8    	jsr	0xdea8
    ceca:	7e dc eb    	jmp	0xdceb
    cecd:	54          	lsrb
    cece:	55          	.byte	0x55
    cecf:	4e          	.byte	0x4e
    ced0:	45          	.byte	0x45
    ced1:	20 20       	bra	0x0xcef3
    ced3:	4f          	clra
    ced4:	53          	comb
    ced5:	43          	coma
    ced6:	31          	ins
    ced7:	20 4f       	bra	0x0xcf28
    ced9:	53          	comb
    ceda:	43          	coma
    cedb:	32          	pula
    cedc:	20 20       	bra	0x0xcefe
    cede:	20 20       	bra	0x0xcf00
    cee0:	20 20       	bra	0x0xcf02
    cee2:	20 20       	bra	0x0xcf04
    cee4:	20 20       	bra	0x0xcf06
    cee6:	20 20       	bra	0x0xcf08
    cee8:	20 20       	bra	0x0xcf0a
    ceea:	20 20       	bra	0x0xcf0c
    ceec:	20 bd       	bra	0x0xceab
    ceee:	dd ae       	std	*0xae
    cef0:	ce 01 20    	ldx	#0x120
    cef3:	18 ce cf 25 	ldy	#0xcf25
    cef7:	c6 20       	ldab	#0x20
    cef9:	18 a6 00    	ldaa	0x0,y
    cefc:	a7 00       	staa	0x0,x
    cefe:	08          	inx
    ceff:	18 08       	iny
    cf01:	5a          	decb
    cf02:	26 f5       	bne	0x0xcef9
    cf04:	ce 01 31    	ldx	#0x131
    cf07:	96 40       	ldaa	*0x40
    cf09:	bd de e4    	jsr	0xdee4
    cf0c:	ce cf 45    	ldx	#0xcf45
    cf0f:	d6 41       	ldab	*0x41
    cf11:	c4 0f       	andb	#0xf
    cf13:	18 ce 01 36 	ldy	#0x136
    cf17:	bd df 5b    	jsr	0xdf5b
    cf1a:	ce 01 3c    	ldx	#0x13c
    cf1d:	96 67       	ldaa	*0x67
    cf1f:	bd de a8    	jsr	0xdea8
    cf22:	7e dc eb    	jmp	0xdceb
    cf25:	46          	rora
    cf26:	49          	rola
    cf27:	4e          	.byte	0x4e
    cf28:	45          	.byte	0x45
    cf29:	20 4d       	bra	0x0xcf78
    cf2b:	4f          	clra
    cf2c:	44          	lsra
    cf2d:	45          	.byte	0x45
    cf2e:	32          	pula
    cf2f:	20 45       	bra	0x0xcf76
    cf31:	4e          	.byte	0x4e
    cf32:	56          	rorb
    cf33:	31          	ins
    cf34:	20 20       	bra	0x0xcf56
    cf36:	20 20       	bra	0x0xcf58
    cf38:	20 20       	bra	0x0xcf5a
    cf3a:	20 20       	bra	0x0xcf5c
    cf3c:	20 20       	bra	0x0xcf5e
    cf3e:	20 20       	bra	0x0xcf60
    cf40:	20 20       	bra	0x0xcf62
    cf42:	20 20       	bra	0x0xcf64
    cf44:	20 4e       	bra	0x0xcf94
    cf46:	4f          	clra
    cf47:	52          	.byte	0x52
    cf48:	4d          	tsta
    cf49:	48          	asla
    cf4a:	41          	.byte	0x41
    cf4b:	4c          	inca
    cf4c:	46          	rora
    cf4d:	4e          	.byte	0x4e
    cf4e:	4f          	clra
    cf4f:	43          	coma
    cf50:	56          	rorb
    cf51:	4c          	inca
    cf52:	4f          	clra
    cf53:	57          	asrb
    cf54:	31          	ins
    cf55:	4c          	inca
    cf56:	4f          	clra
    cf57:	57          	asrb
    cf58:	32          	pula
    cf59:	bd dd ae    	jsr	0xddae
    cf5c:	ce 01 20    	ldx	#0x120
    cf5f:	18 ce cf 7f 	ldy	#0xcf7f
    cf63:	c6 20       	ldab	#0x20
    cf65:	18 a6 00    	ldaa	0x0,y
    cf68:	a7 00       	staa	0x0,x
    cf6a:	08          	inx
    cf6b:	18 08       	iny
    cf6d:	5a          	decb
    cf6e:	26 f5       	bne	0x0xcf65
    cf70:	ce cf 9f    	ldx	#0xcf9f
    cf73:	d6 4a       	ldab	*0x4a
    cf75:	18 ce 01 30 	ldy	#0x130
    cf79:	bd df 5b    	jsr	0xdf5b
    cf7c:	7e dc eb    	jmp	0xdceb
    cf7f:	58          	aslb
    cf80:	4d          	tsta
    cf81:	4f          	clra
    cf82:	44          	lsra
    cf83:	20 54       	bra	0x0xcfd9
    cf85:	41          	.byte	0x41
    cf86:	52          	.byte	0x52
    cf87:	47          	asra
    cf88:	45          	.byte	0x45
    cf89:	54          	lsrb
    cf8a:	20 20       	bra	0x0xcfac
    cf8c:	20 20       	bra	0x0xcfae
    cf8e:	20 20       	bra	0x0xcfb0
    cf90:	20 20       	bra	0x0xcfb2
    cf92:	20 20       	bra	0x0xcfb4
    cf94:	20 20       	bra	0x0xcfb6
    cf96:	20 20       	bra	0x0xcfb8
    cf98:	20 20       	bra	0x0xcfba
    cf9a:	20 20       	bra	0x0xcfbc
    cf9c:	20 20       	bra	0x0xcfbe
    cf9e:	20 20       	bra	0x0xcfc0
    cfa0:	4f          	clra
    cfa1:	46          	rora
    cfa2:	46          	rora
    cfa3:	4f          	clra
    cfa4:	53          	comb
    cfa5:	43          	coma
    cfa6:	31          	ins
    cfa7:	20 56       	bra	0x0xcfff
    cfa9:	43          	coma
    cfaa:	46          	rora
    cfab:	4f          	clra
    cfac:	31          	ins
    cfad:	26 46       	bne	0x0xcff5
    cfaf:	7d 01 1d    	tst	0x11d
    cfb2:	26 0a       	bne	0x0xcfbe
    cfb4:	bd df 1f    	jsr	0xdf1f
    cfb7:	ce 10 23    	ldx	#0x1023
    cfba:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xcfba
    cfbe:	bd dd ae    	jsr	0xddae
    cfc1:	ce 01 20    	ldx	#0x120
    cfc4:	18 ce cf fe 	ldy	#0xcffe
    cfc8:	c6 20       	ldab	#0x20
    cfca:	18 a6 00    	ldaa	0x0,y
    cfcd:	a7 00       	staa	0x0,x
    cfcf:	08          	inx
    cfd0:	18 08       	iny
    cfd2:	5a          	decb
    cfd3:	26 f5       	bne	0x0xcfca
    cfd5:	ce d0 1e    	ldx	#0xd01e
    cfd8:	d6 6a       	ldab	*0x6a
    cfda:	c4 0f       	andb	#0xf
    cfdc:	18 ce 01 30 	ldy	#0x130
    cfe0:	bd df 5b    	jsr	0xdf5b
    cfe3:	ce d0 32    	ldx	#0xd032
    cfe6:	d6 95       	ldab	*0x95
    cfe8:	18 ce 01 36 	ldy	#0x136
    cfec:	bd df 5b    	jsr	0xdf5b
    cfef:	ce d0 3e    	ldx	#0xd03e
    cff2:	d6 69       	ldab	*0x69
    cff4:	18 ce 01 3b 	ldy	#0x13b
    cff8:	bd df 5b    	jsr	0xdf5b
    cffb:	7e dc eb    	jmp	0xdceb
    cffe:	20 55       	bra	0x0xd055
    d000:	4e          	.byte	0x4e
    d001:	49          	rola
    d002:	20 56       	bra	0x0xd05a
    d004:	4d          	tsta
    d005:	4f          	clra
    d006:	44          	lsra
    d007:	45          	.byte	0x45
    d008:	20 50       	bra	0x0xd05a
    d00a:	52          	.byte	0x52
    d00b:	49          	rola
    d00c:	4f          	clra
    d00d:	52          	.byte	0x52
    d00e:	20 20       	bra	0x0xd030
    d010:	20 20       	bra	0x0xd032
    d012:	20 20       	bra	0x0xd034
    d014:	20 20       	bra	0x0xd036
    d016:	20 20       	bra	0x0xd038
    d018:	20 20       	bra	0x0xd03a
    d01a:	20 20       	bra	0x0xd03c
    d01c:	20 20       	bra	0x0xd03e
    d01e:	20 4f       	bra	0x0xd06f
    d020:	4e          	.byte	0x4e
    d021:	45          	.byte	0x45
    d022:	20 54       	bra	0x0xd078
    d024:	57          	asrb
    d025:	4f          	clra
    d026:	46          	rora
    d027:	4f          	clra
    d028:	55          	.byte	0x55
    d029:	52          	.byte	0x52
    d02a:	20 53       	bra	0x0xd07f
    d02c:	49          	rola
    d02d:	58          	aslb
    d02e:	45          	.byte	0x45
    d02f:	47          	asra
    d030:	48          	asla
    d031:	54          	lsrb
    d032:	43          	coma
    d033:	59          	rolb
    d034:	43          	coma
    d035:	4c          	inca
    d036:	4e          	.byte	0x4e
    d037:	4f          	clra
    d038:	54          	lsrb
    d039:	45          	.byte	0x45
    d03a:	4f          	clra
    d03b:	56          	rorb
    d03c:	45          	.byte	0x45
    d03d:	52          	.byte	0x52
    d03e:	4c          	inca
    d03f:	41          	.byte	0x41
    d040:	53          	comb
    d041:	54          	lsrb
    d042:	20 4c       	bra	0x0xd090
    d044:	4f          	clra
    d045:	57          	asrb
    d046:	bd dd ae    	jsr	0xddae
    d049:	ce 01 20    	ldx	#0x120
    d04c:	18 ce d0 78 	ldy	#0xd078
    d050:	c6 20       	ldab	#0x20
    d052:	18 a6 00    	ldaa	0x0,y
    d055:	a7 00       	staa	0x0,x
    d057:	08          	inx
    d058:	18 08       	iny
    d05a:	5a          	decb
    d05b:	26 f5       	bne	0x0xd052
    d05d:	ce d0 98    	ldx	#0xd098
    d060:	d6 6b       	ldab	*0x6b
    d062:	18 ce 01 30 	ldy	#0x130
    d066:	bd df 5b    	jsr	0xdf5b
    d069:	ce d0 a4    	ldx	#0xd0a4
    d06c:	18 ce 01 36 	ldy	#0x136
    d070:	d6 68       	ldab	*0x68
    d072:	bd df 5b    	jsr	0xdf5b
    d075:	7e dc eb    	jmp	0xdceb
    d078:	4f          	clra
    d079:	43          	coma
    d07a:	54          	lsrb
    d07b:	41          	.byte	0x41
    d07c:	56          	rorb
    d07d:	20 4d       	bra	0x0xd0cc
    d07f:	54          	lsrb
    d080:	52          	.byte	0x52
    d081:	47          	asra
    d082:	20 54       	bra	0x0xd0d8
    d084:	55          	.byte	0x55
    d085:	4e          	.byte	0x4e
    d086:	45          	.byte	0x45
    d087:	3f          	swi
    d088:	20 20       	bra	0x0xd0aa
    d08a:	20 20       	bra	0x0xd0ac
    d08c:	20 20       	bra	0x0xd0ae
    d08e:	20 20       	bra	0x0xd0b0
    d090:	20 20       	bra	0x0xd0b2
    d092:	20 20       	bra	0x0xd0b4
    d094:	20 4e       	bra	0x0xd0e4
    d096:	4f          	clra
    d097:	20 20       	bra	0x0xd0b9
    d099:	4c          	inca
    d09a:	4f          	clra
    d09b:	57          	asrb
    d09c:	20 4d       	bra	0x0xd0eb
    d09e:	49          	rola
    d09f:	44          	lsra
    d0a0:	48          	asla
    d0a1:	49          	rola
    d0a2:	47          	asra
    d0a3:	48          	asla
    d0a4:	20 4f       	bra	0x0xd0f5
    d0a6:	46          	rora
    d0a7:	46          	rora
    d0a8:	45          	.byte	0x45
    d0a9:	4e          	.byte	0x4e
    d0aa:	56          	rorb
    d0ab:	31          	ins
    d0ac:	45          	.byte	0x45
    d0ad:	4e          	.byte	0x4e
    d0ae:	56          	rorb
    d0af:	32          	pula
    d0b0:	45          	.byte	0x45
    d0b1:	4e          	.byte	0x4e
    d0b2:	56          	rorb
    d0b3:	33          	pulb
    d0b4:	45          	.byte	0x45
    d0b5:	31          	ins
    d0b6:	26 32       	bne	0x0xd0ea
    d0b8:	45          	.byte	0x45
    d0b9:	31          	ins
    d0ba:	26 33       	bne	0x0xd0ef
    d0bc:	45          	.byte	0x45
    d0bd:	32          	pula
    d0be:	26 33       	bne	0x0xd0f3
    d0c0:	20 41       	bra	0x0xd103
    d0c2:	4c          	inca
    d0c3:	4c          	inca
    d0c4:	7d 01 1d    	tst	0x11d
    d0c7:	26 0a       	bne	0x0xd0d3
    d0c9:	bd df 1f    	jsr	0xdf1f
    d0cc:	ce 10 23    	ldx	#0x1023
    d0cf:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd0cf
    d0d3:	bd dd ae    	jsr	0xddae
    d0d6:	ce 01 20    	ldx	#0x120
    d0d9:	18 ce d1 3b 	ldy	#0xd13b
    d0dd:	c6 20       	ldab	#0x20
    d0df:	18 a6 00    	ldaa	0x0,y
    d0e2:	a7 00       	staa	0x0,x
    d0e4:	08          	inx
    d0e5:	18 08       	iny
    d0e7:	5a          	decb
    d0e8:	26 f5       	bne	0x0xd0df
    d0ea:	ce d1 5b    	ldx	#0xd15b
    d0ed:	18 ce 01 30 	ldy	#0x130
    d0f1:	13 21 40 07 	brclr	*0x21, #0x40, 0x0xd0fc
    d0f5:	c6 01       	ldab	#0x1
    d0f7:	bd df 47    	jsr	0xdf47
    d0fa:	20 04       	bra	0x0xd100
    d0fc:	5f          	clrb
    d0fd:	bd df 47    	jsr	0xdf47
    d100:	18 ce 01 34 	ldy	#0x134
    d104:	ce d1 61    	ldx	#0xd161
    d107:	12 21 08 06 	brset	*0x21, #0x08, 0x0xd111
    d10b:	5f          	clrb
    d10c:	bd df 47    	jsr	0xdf47
    d10f:	20 05       	bra	0x0xd116
    d111:	c6 01       	ldab	#0x1
    d113:	bd df 47    	jsr	0xdf47
    d116:	ce d1 67    	ldx	#0xd167
    d119:	18 ce 01 38 	ldy	#0x138
    d11d:	12 21 01 06 	brset	*0x21, #0x01, 0x0xd127
    d121:	5f          	clrb
    d122:	bd df 47    	jsr	0xdf47
    d125:	20 05       	bra	0x0xd12c
    d127:	c6 01       	ldab	#0x1
    d129:	bd df 47    	jsr	0xdf47
    d12c:	ce d1 6d    	ldx	#0xd16d
    d12f:	d6 24       	ldab	*0x24
    d131:	18 ce 01 3c 	ldy	#0x13c
    d135:	bd df 47    	jsr	0xdf47
    d138:	7e dc eb    	jmp	0xdceb
    d13b:	20 4f       	bra	0x0xd18c
    d13d:	4e          	.byte	0x4e
    d13e:	20 54       	bra	0x0xd194
    d140:	59          	rolb
    d141:	50          	negb
    d142:	20 4d       	bra	0x0xd191
    d144:	44          	lsra
    d145:	45          	.byte	0x45
    d146:	20 44       	bra	0x0xd18c
    d148:	45          	.byte	0x45
    d149:	53          	comb
    d14a:	20 20       	bra	0x0xd16c
    d14c:	20 20       	bra	0x0xd16e
    d14e:	20 20       	bra	0x0xd170
    d150:	20 20       	bra	0x0xd172
    d152:	20 20       	bra	0x0xd174
    d154:	20 20       	bra	0x0xd176
    d156:	20 20       	bra	0x0xd178
    d158:	20 20       	bra	0x0xd17a
    d15a:	20 4f       	bra	0x0xd1ab
    d15c:	46          	rora
    d15d:	46          	rora
    d15e:	20 4f       	bra	0x0xd1af
    d160:	4e          	.byte	0x4e
    d161:	45          	.byte	0x45
    d162:	58          	aslb
    d163:	50          	negb
    d164:	4c          	inca
    d165:	49          	rola
    d166:	4e          	.byte	0x4e
    d167:	52          	.byte	0x52
    d168:	45          	.byte	0x45
    d169:	47          	asra
    d16a:	4c          	inca
    d16b:	45          	.byte	0x45
    d16c:	47          	asra
    d16d:	4f          	clra
    d16e:	26 46       	bne	0x0xd1b6
    d170:	4f          	clra
    d171:	53          	comb
    d172:	31          	ins
    d173:	4f          	clra
    d174:	53          	comb
    d175:	32          	pula
    d176:	31          	ins
    d177:	26 32       	bne	0x0xd1ab
    d179:	31          	ins
    d17a:	26 46       	bne	0x0xd1c2
    d17c:	32          	pula
    d17d:	26 46       	bne	0x0xd1c5
    d17f:	46          	rora
    d180:	49          	rola
    d181:	4c          	inca
    d182:	bd dd ae    	jsr	0xddae
    d185:	ce 01 20    	ldx	#0x120
    d188:	18 ce d1 d8 	ldy	#0xd1d8
    d18c:	c6 20       	ldab	#0x20
    d18e:	18 a6 00    	ldaa	0x0,y
    d191:	a7 00       	staa	0x0,x
    d193:	08          	inx
    d194:	18 08       	iny
    d196:	5a          	decb
    d197:	26 f5       	bne	0x0xd18e
    d199:	ce d1 5b    	ldx	#0xd15b
    d19c:	18 ce 01 30 	ldy	#0x130
    d1a0:	13 21 02 07 	brclr	*0x21, #0x02, 0x0xd1ab
    d1a4:	c6 01       	ldab	#0x1
    d1a6:	bd df 47    	jsr	0xdf47
    d1a9:	20 04       	bra	0x0xd1af
    d1ab:	5f          	clrb
    d1ac:	bd df 47    	jsr	0xdf47
    d1af:	18 ce 01 34 	ldy	#0x134
    d1b3:	ce d1 5b    	ldx	#0xd15b
    d1b6:	12 21 04 06 	brset	*0x21, #0x04, 0x0xd1c0
    d1ba:	5f          	clrb
    d1bb:	bd df 47    	jsr	0xdf47
    d1be:	20 05       	bra	0x0xd1c5
    d1c0:	c6 01       	ldab	#0x1
    d1c2:	bd df 47    	jsr	0xdf47
    d1c5:	96 22       	ldaa	*0x22
    d1c7:	ce 01 38    	ldx	#0x138
    d1ca:	bd de e4    	jsr	0xdee4
    d1cd:	96 23       	ldaa	*0x23
    d1cf:	ce 01 3c    	ldx	#0x13c
    d1d2:	bd de a8    	jsr	0xdea8
    d1d5:	7e dc eb    	jmp	0xdceb
    d1d8:	47          	asra
    d1d9:	4c          	inca
    d1da:	53          	comb
    d1db:	20 41       	bra	0x0xd21e
    d1dd:	55          	.byte	0x55
    d1de:	54          	lsrb
    d1df:	20 49       	bra	0x0xd22a
    d1e1:	4e          	.byte	0x4e
    d1e2:	54          	lsrb
    d1e3:	20 44       	bra	0x0xd229
    d1e5:	59          	rolb
    d1e6:	4e          	.byte	0x4e
    d1e7:	20 20       	bra	0x0xd209
    d1e9:	20 20       	bra	0x0xd20b
    d1eb:	20 20       	bra	0x0xd20d
    d1ed:	20 20       	bra	0x0xd20f
    d1ef:	20 20       	bra	0x0xd211
    d1f1:	20 20       	bra	0x0xd213
    d1f3:	20 20       	bra	0x0xd215
    d1f5:	20 20       	bra	0x0xd217
    d1f7:	20 7d       	bra	0x0xd276
    d1f9:	01          	nop
    d1fa:	1d 26 0a    	bclr	0x26,x, #0x0a
    d1fd:	bd df 1f    	jsr	0xdf1f
    d200:	ce 10 23    	ldx	#0x1023
    d203:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd203
    d207:	bd dd ae    	jsr	0xddae
    d20a:	ce 01 20    	ldx	#0x120
    d20d:	18 ce d3 31 	ldy	#0xd331
    d211:	c6 20       	ldab	#0x20
    d213:	18 a6 00    	ldaa	0x0,y
    d216:	a7 00       	staa	0x0,x
    d218:	08          	inx
    d219:	18 08       	iny
    d21b:	5a          	decb
    d21c:	26 f5       	bne	0x0xd213
    d21e:	96 f6       	ldaa	*0xf6
    d220:	48          	asla
    d221:	24 2b       	bcc	0x0xd24e
    d223:	ce d6 20    	ldx	#0xd620
    d226:	d6 8a       	ldab	*0x8a
    d228:	18 ce 01 26 	ldy	#0x126
    d22c:	bd df 5b    	jsr	0xdf5b
    d22f:	ce d6 20    	ldx	#0xd620
    d232:	d6 8b       	ldab	*0x8b
    d234:	18 08       	iny
    d236:	18 08       	iny
    d238:	bd df 5b    	jsr	0xdf5b
    d23b:	ce 01 37    	ldx	#0x137
    d23e:	96 8c       	ldaa	*0x8c
    d240:	bd de e4    	jsr	0xdee4
    d243:	ce 01 3c    	ldx	#0x13c
    d246:	96 8d       	ldaa	*0x8d
    d248:	bd de e4    	jsr	0xdee4
    d24b:	7e dc eb    	jmp	0xdceb
    d24e:	48          	asla
    d24f:	24 2b       	bcc	0x0xd27c
    d251:	ce d6 20    	ldx	#0xd620
    d254:	d6 86       	ldab	*0x86
    d256:	18 ce 01 26 	ldy	#0x126
    d25a:	bd df 5b    	jsr	0xdf5b
    d25d:	ce d6 20    	ldx	#0xd620
    d260:	d6 87       	ldab	*0x87
    d262:	18 08       	iny
    d264:	18 08       	iny
    d266:	bd df 5b    	jsr	0xdf5b
    d269:	ce 01 37    	ldx	#0x137
    d26c:	96 88       	ldaa	*0x88
    d26e:	bd de a8    	jsr	0xdea8
    d271:	08          	inx
    d272:	08          	inx
    d273:	08          	inx
    d274:	96 89       	ldaa	*0x89
    d276:	bd de a8    	jsr	0xdea8
    d279:	7e dc eb    	jmp	0xdceb
    d27c:	48          	asla
    d27d:	24 2b       	bcc	0x0xd2aa
    d27f:	ce d6 20    	ldx	#0xd620
    d282:	d6 82       	ldab	*0x82
    d284:	18 ce 01 26 	ldy	#0x126
    d288:	bd df 5b    	jsr	0xdf5b
    d28b:	ce d6 20    	ldx	#0xd620
    d28e:	d6 83       	ldab	*0x83
    d290:	18 08       	iny
    d292:	18 08       	iny
    d294:	bd df 5b    	jsr	0xdf5b
    d297:	ce 01 37    	ldx	#0x137
    d29a:	96 84       	ldaa	*0x84
    d29c:	bd de a8    	jsr	0xdea8
    d29f:	08          	inx
    d2a0:	08          	inx
    d2a1:	08          	inx
    d2a2:	96 85       	ldaa	*0x85
    d2a4:	bd de a8    	jsr	0xdea8
    d2a7:	7e dc eb    	jmp	0xdceb
    d2aa:	48          	asla
    d2ab:	24 2b       	bcc	0x0xd2d8
    d2ad:	ce d6 20    	ldx	#0xd620
    d2b0:	d6 7e       	ldab	*0x7e
    d2b2:	18 ce 01 26 	ldy	#0x126
    d2b6:	bd df 5b    	jsr	0xdf5b
    d2b9:	ce d6 20    	ldx	#0xd620
    d2bc:	d6 7f       	ldab	*0x7f
    d2be:	18 08       	iny
    d2c0:	18 08       	iny
    d2c2:	bd df 5b    	jsr	0xdf5b
    d2c5:	ce 01 37    	ldx	#0x137
    d2c8:	96 80       	ldaa	*0x80
    d2ca:	bd de a8    	jsr	0xdea8
    d2cd:	08          	inx
    d2ce:	08          	inx
    d2cf:	08          	inx
    d2d0:	96 81       	ldaa	*0x81
    d2d2:	bd de a8    	jsr	0xdea8
    d2d5:	7e dc eb    	jmp	0xdceb
    d2d8:	48          	asla
    d2d9:	24 2b       	bcc	0x0xd306
    d2db:	ce d6 20    	ldx	#0xd620
    d2de:	d6 7a       	ldab	*0x7a
    d2e0:	18 ce 01 26 	ldy	#0x126
    d2e4:	bd df 5b    	jsr	0xdf5b
    d2e7:	ce d6 20    	ldx	#0xd620
    d2ea:	d6 7b       	ldab	*0x7b
    d2ec:	18 08       	iny
    d2ee:	18 08       	iny
    d2f0:	bd df 5b    	jsr	0xdf5b
    d2f3:	ce 01 37    	ldx	#0x137
    d2f6:	96 7c       	ldaa	*0x7c
    d2f8:	bd de a8    	jsr	0xdea8
    d2fb:	08          	inx
    d2fc:	08          	inx
    d2fd:	08          	inx
    d2fe:	96 7d       	ldaa	*0x7d
    d300:	bd de a8    	jsr	0xdea8
    d303:	7e dc eb    	jmp	0xdceb
    d306:	ce d6 20    	ldx	#0xd620
    d309:	d6 76       	ldab	*0x76
    d30b:	18 ce 01 26 	ldy	#0x126
    d30f:	bd df 5b    	jsr	0xdf5b
    d312:	ce d6 20    	ldx	#0xd620
    d315:	d6 77       	ldab	*0x77
    d317:	18 08       	iny
    d319:	18 08       	iny
    d31b:	bd df 5b    	jsr	0xdf5b
    d31e:	ce 01 37    	ldx	#0x137
    d321:	96 78       	ldaa	*0x78
    d323:	bd de a8    	jsr	0xdea8
    d326:	08          	inx
    d327:	08          	inx
    d328:	08          	inx
    d329:	96 79       	ldaa	*0x79
    d32b:	bd de a8    	jsr	0xdea8
    d32e:	7e dc eb    	jmp	0xdceb
    d331:	20 44       	bra	0x0xd377
    d333:	45          	.byte	0x45
    d334:	53          	comb
    d335:	54          	lsrb
    d336:	20 20       	bra	0x0xd358
    d338:	20 20       	bra	0x0xd35a
    d33a:	20 20       	bra	0x0xd35c
    d33c:	20 20       	bra	0x0xd35e
    d33e:	20 20       	bra	0x0xd360
    d340:	20 20       	bra	0x0xd362
    d342:	41          	.byte	0x41
    d343:	4d          	tsta
    d344:	4e          	.byte	0x4e
    d345:	54          	lsrb
    d346:	20 20       	bra	0x0xd368
    d348:	20 20       	bra	0x0xd36a
    d34a:	20 20       	bra	0x0xd36c
    d34c:	20 20       	bra	0x0xd36e
    d34e:	20 20       	bra	0x0xd370
    d350:	20 41       	bra	0x0xd393
    d352:	54          	lsrb
    d353:	54          	lsrb
    d354:	31          	ins
    d355:	44          	lsra
    d356:	45          	.byte	0x45
    d357:	43          	coma
    d358:	31          	ins
    d359:	53          	comb
    d35a:	55          	.byte	0x55
    d35b:	53          	comb
    d35c:	31          	ins
    d35d:	52          	.byte	0x52
    d35e:	45          	.byte	0x45
    d35f:	4c          	inca
    d360:	31          	ins
    d361:	41          	.byte	0x41
    d362:	4d          	tsta
    d363:	54          	lsrb
    d364:	31          	ins
    d365:	41          	.byte	0x41
    d366:	54          	lsrb
    d367:	54          	lsrb
    d368:	32          	pula
    d369:	44          	lsra
    d36a:	45          	.byte	0x45
    d36b:	43          	coma
    d36c:	32          	pula
    d36d:	53          	comb
    d36e:	55          	.byte	0x55
    d36f:	53          	comb
    d370:	32          	pula
    d371:	52          	.byte	0x52
    d372:	45          	.byte	0x45
    d373:	4c          	inca
    d374:	32          	pula
    d375:	41          	.byte	0x41
    d376:	4d          	tsta
    d377:	54          	lsrb
    d378:	32          	pula
    d379:	41          	.byte	0x41
    d37a:	54          	lsrb
    d37b:	54          	lsrb
    d37c:	33          	pulb
    d37d:	44          	lsra
    d37e:	45          	.byte	0x45
    d37f:	43          	coma
    d380:	33          	pulb
    d381:	53          	comb
    d382:	55          	.byte	0x55
    d383:	53          	comb
    d384:	33          	pulb
    d385:	52          	.byte	0x52
    d386:	45          	.byte	0x45
    d387:	4c          	inca
    d388:	33          	pulb
    d389:	41          	.byte	0x41
    d38a:	4d          	tsta
    d38b:	54          	lsrb
    d38c:	33          	pulb
    d38d:	44          	lsra
    d38e:	45          	.byte	0x45
    d38f:	4c          	inca
    d390:	31          	ins
    d391:	44          	lsra
    d392:	45          	.byte	0x45
    d393:	4c          	inca
    d394:	33          	pulb
    d395:	7d 01 1d    	tst	0x11d
    d398:	26 0a       	bne	0x0xd3a4
    d39a:	bd df 1f    	jsr	0xdf1f
    d39d:	ce 10 23    	ldx	#0x1023
    d3a0:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd3a0
    d3a4:	bd dd ae    	jsr	0xddae
    d3a7:	ce 01 20    	ldx	#0x120
    d3aa:	18 ce d3 d6 	ldy	#0xd3d6
    d3ae:	c6 20       	ldab	#0x20
    d3b0:	18 a6 00    	ldaa	0x0,y
    d3b3:	a7 00       	staa	0x0,x
    d3b5:	08          	inx
    d3b6:	18 08       	iny
    d3b8:	5a          	decb
    d3b9:	26 f5       	bne	0x0xd3b0
    d3bb:	ce d3 f6    	ldx	#0xd3f6
    d3be:	d6 8e       	ldab	*0x8e
    d3c0:	18 ce 01 36 	ldy	#0x136
    d3c4:	bd df 5b    	jsr	0xdf5b
    d3c7:	ce d4 02    	ldx	#0xd402
    d3ca:	d6 8f       	ldab	*0x8f
    d3cc:	18 08       	iny
    d3ce:	18 08       	iny
    d3d0:	bd df 5b    	jsr	0xdf5b
    d3d3:	7e dc eb    	jmp	0xdceb
    d3d6:	43          	coma
    d3d7:	4e          	.byte	0x4e
    d3d8:	54          	lsrb
    d3d9:	52          	.byte	0x52
    d3da:	4c          	inca
    d3db:	20 43       	bra	0x0xd420
    d3dd:	4f          	clra
    d3de:	4e          	.byte	0x4e
    d3df:	31          	ins
    d3e0:	20 43       	bra	0x0xd425
    d3e2:	4f          	clra
    d3e3:	4e          	.byte	0x4e
    d3e4:	32          	pula
    d3e5:	20 41       	bra	0x0xd428
    d3e7:	53          	comb
    d3e8:	53          	comb
    d3e9:	47          	asra
    d3ea:	4e          	.byte	0x4e
    d3eb:	20 20       	bra	0x0xd40d
    d3ed:	20 20       	bra	0x0xd40f
    d3ef:	20 20       	bra	0x0xd411
    d3f1:	20 20       	bra	0x0xd413
    d3f3:	20 20       	bra	0x0xd415
    d3f5:	20 42       	bra	0x0xd439
    d3f7:	52          	.byte	0x52
    d3f8:	54          	lsrb
    d3f9:	48          	asla
    d3fa:	4d          	tsta
    d3fb:	4f          	clra
    d3fc:	44          	lsra
    d3fd:	57          	asrb
    d3fe:	54          	lsrb
    d3ff:	55          	.byte	0x55
    d400:	43          	coma
    d401:	48          	asla
    d402:	20 44       	bra	0x0xd448
    d404:	59          	rolb
    d405:	4e          	.byte	0x4e
    d406:	54          	lsrb
    d407:	52          	.byte	0x52
    d408:	41          	.byte	0x41
    d409:	4b          	.byte	0x4b
    d40a:	7d 01 1d    	tst	0x11d
    d40d:	26 0a       	bne	0x0xd419
    d40f:	bd df 1f    	jsr	0xdf1f
    d412:	ce 10 23    	ldx	#0x1023
    d415:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd415
    d419:	bd dd ae    	jsr	0xddae
    d41c:	ce 01 20    	ldx	#0x120
    d41f:	18 ce d4 55 	ldy	#0xd455
    d423:	c6 20       	ldab	#0x20
    d425:	18 a6 00    	ldaa	0x0,y
    d428:	a7 00       	staa	0x0,x
    d42a:	08          	inx
    d42b:	18 08       	iny
    d42d:	5a          	decb
    d42e:	26 f5       	bne	0x0xd425
    d430:	f6 50 00    	ldab	0x5000
    d433:	c1 03       	cmpb	#0x3
    d435:	25 01       	bcs	0x0xd438
    d437:	5f          	clrb
    d438:	86 09       	ldaa	#0x9
    d43a:	3d          	mul
    d43b:	1b          	aba
    d43c:	16          	tab
    d43d:	ce d4 75    	ldx	#0xd475
    d440:	3a          	abx
    d441:	18 ce 01 31 	ldy	#0x131
    d445:	c6 09       	ldab	#0x9
    d447:	a6 00       	ldaa	0x0,x
    d449:	18 a7 00    	staa	0x0,y
    d44c:	08          	inx
    d44d:	18 08       	iny
    d44f:	5a          	decb
    d450:	26 f5       	bne	0x0xd447
    d452:	7e dc eb    	jmp	0xdceb
    d455:	20 4d       	bra	0x0xd4a4
    d457:	55          	.byte	0x55
    d458:	4c          	inca
    d459:	54          	lsrb
    d45a:	49          	rola
    d45b:	20 54       	bra	0x0xd4b1
    d45d:	59          	rolb
    d45e:	50          	negb
    d45f:	45          	.byte	0x45
    d460:	3a          	abx
    d461:	20 20       	bra	0x0xd483
    d463:	20 20       	bra	0x0xd485
    d465:	20 20       	bra	0x0xd487
    d467:	20 20       	bra	0x0xd489
    d469:	20 20       	bra	0x0xd48b
    d46b:	20 20       	bra	0x0xd48d
    d46d:	20 20       	bra	0x0xd48f
    d46f:	20 20       	bra	0x0xd491
    d471:	20 20       	bra	0x0xd493
    d473:	20 20       	bra	0x0xd495
    d475:	4d          	tsta
    d476:	55          	.byte	0x55
    d477:	4c          	inca
    d478:	54          	lsrb
    d479:	49          	rola
    d47a:	20 31       	bra	0x0xd4ad
    d47c:	2b 31       	bmi	0x0xd4af
    d47e:	53          	comb
    d47f:	50          	negb
    d480:	4c          	inca
    d481:	49          	rola
    d482:	54          	lsrb
    d483:	20 31       	bra	0x0xd4b6
    d485:	2b 31       	bmi	0x0xd4b8
    d487:	4c          	inca
    d488:	41          	.byte	0x41
    d489:	59          	rolb
    d48a:	45          	.byte	0x45
    d48b:	52          	.byte	0x52
    d48c:	20 31       	bra	0x0xd4bf
    d48e:	2b 31       	bmi	0x0xd4c1
    d490:	bd dd ae    	jsr	0xddae
    d493:	ce 01 20    	ldx	#0x120
    d496:	18 ce d4 e6 	ldy	#0xd4e6
    d49a:	c6 20       	ldab	#0x20
    d49c:	18 a6 00    	ldaa	0x0,y
    d49f:	a7 00       	staa	0x0,x
    d4a1:	08          	inx
    d4a2:	18 08       	iny
    d4a4:	5a          	decb
    d4a5:	26 f5       	bne	0x0xd49c
    d4a7:	b6 50 08    	ldaa	0x5008
    d4aa:	8b 41       	adda	#0x41
    d4ac:	b7 01 32    	staa	0x132
    d4af:	b6 50 01    	ldaa	0x5001
    d4b2:	4c          	inca
    d4b3:	ce 01 34    	ldx	#0x134
    d4b6:	bd de a8    	jsr	0xdea8
    d4b9:	ce 01 38    	ldx	#0x138
    d4bc:	b6 50 02    	ldaa	0x5002
    d4bf:	bd de e4    	jsr	0xdee4
    d4c2:	ce 01 3c    	ldx	#0x13c
    d4c5:	b6 50 03    	ldaa	0x5003
    d4c8:	bd de a8    	jsr	0xdea8
    d4cb:	b6 50 01    	ldaa	0x5001
    d4ce:	f6 50 08    	ldab	0x5008
    d4d1:	d7 f9       	stab	*0xf9
    d4d3:	bd a4 f9    	jsr	0xa4f9
    d4d6:	c6 02       	ldab	#0x2
    d4d8:	d7 f9       	stab	*0xf9
    d4da:	bd 9f 0f    	jsr	0x9f0f
    d4dd:	bd a4 f9    	jsr	0xa4f9
    d4e0:	7f 01 6b    	clr	0x16b
    d4e3:	7e dc eb    	jmp	0xdceb
    d4e6:	50          	negb
    d4e7:	41          	.byte	0x41
    d4e8:	54          	lsrb
    d4e9:	43          	coma
    d4ea:	48          	asla
    d4eb:	31          	ins
    d4ec:	20 4f       	bra	0x0xd53d
    d4ee:	43          	coma
    d4ef:	54          	lsrb
    d4f0:	31          	ins
    d4f1:	20 56       	bra	0x0xd549
    d4f3:	4f          	clra
    d4f4:	4c          	inca
    d4f5:	31          	ins
    d4f6:	20 20       	bra	0x0xd518
    d4f8:	20 20       	bra	0x0xd51a
    d4fa:	20 20       	bra	0x0xd51c
    d4fc:	20 20       	bra	0x0xd51e
    d4fe:	20 20       	bra	0x0xd520
    d500:	20 20       	bra	0x0xd522
    d502:	20 20       	bra	0x0xd524
    d504:	20 20       	bra	0x0xd526
    d506:	bd dd ae    	jsr	0xddae
    d509:	ce 01 20    	ldx	#0x120
    d50c:	18 ce d5 5c 	ldy	#0xd55c
    d510:	c6 20       	ldab	#0x20
    d512:	18 a6 00    	ldaa	0x0,y
    d515:	a7 00       	staa	0x0,x
    d517:	08          	inx
    d518:	18 08       	iny
    d51a:	5a          	decb
    d51b:	26 f5       	bne	0x0xd512
    d51d:	b6 50 09    	ldaa	0x5009
    d520:	8b 41       	adda	#0x41
    d522:	b7 01 32    	staa	0x132
    d525:	b6 50 04    	ldaa	0x5004
    d528:	4c          	inca
    d529:	ce 01 34    	ldx	#0x134
    d52c:	bd de a8    	jsr	0xdea8
    d52f:	ce 01 38    	ldx	#0x138
    d532:	b6 50 05    	ldaa	0x5005
    d535:	bd de e4    	jsr	0xdee4
    d538:	ce 01 3c    	ldx	#0x13c
    d53b:	b6 50 06    	ldaa	0x5006
    d53e:	bd de a8    	jsr	0xdea8
    d541:	b6 50 04    	ldaa	0x5004
    d544:	f6 50 09    	ldab	0x5009
    d547:	d7 f9       	stab	*0xf9
    d549:	bd a4 f9    	jsr	0xa4f9
    d54c:	c6 02       	ldab	#0x2
    d54e:	d7 f9       	stab	*0xf9
    d550:	bd 9f 0f    	jsr	0x9f0f
    d553:	bd a4 f9    	jsr	0xa4f9
    d556:	7c 01 6b    	inc	0x16b
    d559:	7e dc eb    	jmp	0xdceb
    d55c:	50          	negb
    d55d:	41          	.byte	0x41
    d55e:	54          	lsrb
    d55f:	43          	coma
    d560:	48          	asla
    d561:	32          	pula
    d562:	20 4f       	bra	0x0xd5b3
    d564:	43          	coma
    d565:	54          	lsrb
    d566:	32          	pula
    d567:	20 56       	bra	0x0xd5bf
    d569:	4f          	clra
    d56a:	4c          	inca
    d56b:	32          	pula
    d56c:	20 20       	bra	0x0xd58e
    d56e:	20 20       	bra	0x0xd590
    d570:	20 20       	bra	0x0xd592
    d572:	20 20       	bra	0x0xd594
    d574:	20 20       	bra	0x0xd596
    d576:	20 20       	bra	0x0xd598
    d578:	20 20       	bra	0x0xd59a
    d57a:	20 20       	bra	0x0xd59c
    d57c:	bd dd ae    	jsr	0xddae
    d57f:	ce 01 20    	ldx	#0x120
    d582:	18 ce d5 9f 	ldy	#0xd59f
    d586:	c6 20       	ldab	#0x20
    d588:	18 a6 00    	ldaa	0x0,y
    d58b:	a7 00       	staa	0x0,x
    d58d:	08          	inx
    d58e:	18 08       	iny
    d590:	5a          	decb
    d591:	26 f5       	bne	0x0xd588
    d593:	b6 50 07    	ldaa	0x5007
    d596:	ce 01 3c    	ldx	#0x13c
    d599:	bd de a8    	jsr	0xdea8
    d59c:	7e dc eb    	jmp	0xdceb
    d59f:	53          	comb
    d5a0:	50          	negb
    d5a1:	4c          	inca
    d5a2:	49          	rola
    d5a3:	54          	lsrb
    d5a4:	20 50       	bra	0x0xd5f6
    d5a6:	4f          	clra
    d5a7:	49          	rola
    d5a8:	4e          	.byte	0x4e
    d5a9:	54          	lsrb
    d5aa:	20 20       	bra	0x0xd5cc
    d5ac:	20 20       	bra	0x0xd5ce
    d5ae:	20 4d       	bra	0x0xd5fd
    d5b0:	49          	rola
    d5b1:	44          	lsra
    d5b2:	49          	rola
    d5b3:	20 4e       	bra	0x0xd603
    d5b5:	4f          	clra
    d5b6:	54          	lsrb
    d5b7:	45          	.byte	0x45
    d5b8:	20 23       	bra	0x0xd5dd
    d5ba:	20 20       	bra	0x0xd5dc
    d5bc:	20 20       	bra	0x0xd5de
    d5be:	20 7d       	bra	0x0xd63d
    d5c0:	01          	nop
    d5c1:	1d 26 0a    	bclr	0x26,x, #0x0a
    d5c4:	bd df 1f    	jsr	0xdf1f
    d5c7:	ce 10 23    	ldx	#0x1023
    d5ca:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd5ca
    d5ce:	bd dd ae    	jsr	0xddae
    d5d1:	86 20       	ldaa	#0x20
    d5d3:	c6 20       	ldab	#0x20
    d5d5:	ce 01 20    	ldx	#0x120
    d5d8:	a7 00       	staa	0x0,x
    d5da:	08          	inx
    d5db:	5a          	decb
    d5dc:	26 fa       	bne	0x0xd5d8
    d5de:	ce d6 20    	ldx	#0xd620
    d5e1:	d6 60       	ldab	*0x60
    d5e3:	18 ce 01 20 	ldy	#0x120
    d5e7:	bd df 5b    	jsr	0xdf5b
    d5ea:	ce d6 20    	ldx	#0xd620
    d5ed:	d6 61       	ldab	*0x61
    d5ef:	18 08       	iny
    d5f1:	18 08       	iny
    d5f3:	18 08       	iny
    d5f5:	bd df 5b    	jsr	0xdf5b
    d5f8:	ce d6 20    	ldx	#0xd620
    d5fb:	d6 62       	ldab	*0x62
    d5fd:	18 08       	iny
    d5ff:	18 08       	iny
    d601:	bd df 5b    	jsr	0xdf5b
    d604:	ce 01 31    	ldx	#0x131
    d607:	96 63       	ldaa	*0x63
    d609:	bd de a8    	jsr	0xdea8
    d60c:	08          	inx
    d60d:	08          	inx
    d60e:	08          	inx
    d60f:	08          	inx
    d610:	96 64       	ldaa	*0x64
    d612:	bd de a8    	jsr	0xdea8
    d615:	08          	inx
    d616:	08          	inx
    d617:	08          	inx
    d618:	96 65       	ldaa	*0x65
    d61a:	bd de a8    	jsr	0xdea8
    d61d:	7e dc eb    	jmp	0xdceb
    d620:	20 4f       	bra	0x0xd671
    d622:	46          	rora
    d623:	46          	rora
    d624:	46          	rora
    d625:	52          	.byte	0x52
    d626:	45          	.byte	0x45
    d627:	31          	ins
    d628:	46          	rora
    d629:	52          	.byte	0x52
    d62a:	45          	.byte	0x45
    d62b:	32          	pula
    d62c:	31          	ins
    d62d:	26 32       	bne	0x0xd661
    d62f:	46          	rora
    d630:	4c          	inca
    d631:	45          	.byte	0x45
    d632:	56          	rorb
    d633:	31          	ins
    d634:	4c          	inca
    d635:	45          	.byte	0x45
    d636:	56          	rorb
    d637:	32          	pula
    d638:	20 50       	bra	0x0xd68a
    d63a:	57          	asrb
    d63b:	31          	ins
    d63c:	20 50       	bra	0x0xd68e
    d63e:	57          	asrb
    d63f:	32          	pula
    d640:	31          	ins
    d641:	26 32       	bne	0x0xd675
    d643:	50          	negb
    d644:	46          	rora
    d645:	49          	rola
    d646:	4c          	inca
    d647:	54          	lsrb
    d648:	52          	.byte	0x52
    d649:	45          	.byte	0x45
    d64a:	53          	comb
    d64b:	4f          	clra
    d64c:	4c          	inca
    d64d:	45          	.byte	0x45
    d64e:	56          	rorb
    d64f:	4e          	.byte	0x4e
    d650:	58          	aslb
    d651:	4d          	tsta
    d652:	4f          	clra
    d653:	44          	lsra
    d654:	20 45       	bra	0x0xd69b
    d656:	41          	.byte	0x41
    d657:	31          	ins
    d658:	20 45       	bra	0x0xd69f
    d65a:	41          	.byte	0x41
    d65b:	33          	pulb
    d65c:	20 45       	bra	0x0xd6a3
    d65e:	58          	aslb
    d65f:	54          	lsrb
    d660:	4c          	inca
    d661:	46          	rora
    d662:	31          	ins
    d663:	52          	.byte	0x52
    d664:	4c          	inca
    d665:	46          	rora
    d666:	32          	pula
    d667:	52          	.byte	0x52
    d668:	4c          	inca
    d669:	46          	rora
    d66a:	31          	ins
    d66b:	44          	lsra
    d66c:	4c          	inca
    d66d:	46          	rora
    d66e:	32          	pula
    d66f:	44          	lsra
    d670:	31          	ins
    d671:	26 32       	bne	0x0xd6a5
    d673:	44          	lsra
    d674:	31          	ins
    d675:	26 32       	bne	0x0xd6a9
    d677:	52          	.byte	0x52
    d678:	50          	negb
    d679:	41          	.byte	0x41
    d67a:	4e          	.byte	0x4e
    d67b:	52          	.byte	0x52
    d67c:	50          	negb
    d67d:	41          	.byte	0x41
    d67e:	4e          	.byte	0x4e
    d67f:	44          	lsra
    d680:	20 50       	bra	0x0xd6d2
    d682:	41          	.byte	0x41
    d683:	4e          	.byte	0x4e
    d684:	bd dd ae    	jsr	0xddae
    d687:	ce 01 20    	ldx	#0x120
    d68a:	18 ce d6 b7 	ldy	#0xd6b7
    d68e:	c6 20       	ldab	#0x20
    d690:	18 a6 00    	ldaa	0x0,y
    d693:	a7 00       	staa	0x0,x
    d695:	08          	inx
    d696:	18 08       	iny
    d698:	5a          	decb
    d699:	26 f5       	bne	0x0xd690
    d69b:	ce 01 31    	ldx	#0x131
    d69e:	96 54       	ldaa	*0x54
    d6a0:	bd de a8    	jsr	0xdea8
    d6a3:	08          	inx
    d6a4:	08          	inx
    d6a5:	08          	inx
    d6a6:	08          	inx
    d6a7:	96 59       	ldaa	*0x59
    d6a9:	bd de a8    	jsr	0xdea8
    d6ac:	08          	inx
    d6ad:	08          	inx
    d6ae:	08          	inx
    d6af:	96 5e       	ldaa	*0x5e
    d6b1:	bd de a8    	jsr	0xdea8
    d6b4:	7e dc eb    	jmp	0xdceb
    d6b7:	20 44       	bra	0x0xd6fd
    d6b9:	4b          	.byte	0x4b
    d6ba:	32          	pula
    d6bb:	20 20       	bra	0x0xd6dd
    d6bd:	20 44       	bra	0x0xd703
    d6bf:	4b          	.byte	0x4b
    d6c0:	32          	pula
    d6c1:	20 20       	bra	0x0xd6e3
    d6c3:	44          	lsra
    d6c4:	4b          	.byte	0x4b
    d6c5:	32          	pula
    d6c6:	20 20       	bra	0x0xd6e8
    d6c8:	20 20       	bra	0x0xd6ea
    d6ca:	20 20       	bra	0x0xd6ec
    d6cc:	20 20       	bra	0x0xd6ee
    d6ce:	20 20       	bra	0x0xd6f0
    d6d0:	20 20       	bra	0x0xd6f2
    d6d2:	20 20       	bra	0x0xd6f4
    d6d4:	20 20       	bra	0x0xd6f6
    d6d6:	20 bd       	bra	0x0xd695
    d6d8:	dd ae       	std	*0xae
    d6da:	ce 01 20    	ldx	#0x120
    d6dd:	18 ce d7 09 	ldy	#0xd709
    d6e1:	c6 20       	ldab	#0x20
    d6e3:	18 a6 00    	ldaa	0x0,y
    d6e6:	a7 00       	staa	0x0,x
    d6e8:	08          	inx
    d6e9:	18 08       	iny
    d6eb:	5a          	decb
    d6ec:	26 f5       	bne	0x0xd6e3
    d6ee:	ce 01 31    	ldx	#0x131
    d6f1:	96 90       	ldaa	*0x90
    d6f3:	bd de a8    	jsr	0xdea8
    d6f6:	ce 01 37    	ldx	#0x137
    d6f9:	96 91       	ldaa	*0x91
    d6fb:	bd de a8    	jsr	0xdea8
    d6fe:	ce 01 3c    	ldx	#0x13c
    d701:	96 92       	ldaa	*0x92
    d703:	bd de a8    	jsr	0xdea8
    d706:	7e dc eb    	jmp	0xdceb
    d709:	44          	lsra
    d70a:	59          	rolb
    d70b:	4e          	.byte	0x4e
    d70c:	31          	ins
    d70d:	20 20       	bra	0x0xd72f
    d70f:	44          	lsra
    d710:	59          	rolb
    d711:	4e          	.byte	0x4e
    d712:	32          	pula
    d713:	20 44       	bra	0x0xd759
    d715:	59          	rolb
    d716:	4e          	.byte	0x4e
    d717:	33          	pulb
    d718:	20 20       	bra	0x0xd73a
    d71a:	20 20       	bra	0x0xd73c
    d71c:	20 20       	bra	0x0xd73e
    d71e:	20 20       	bra	0x0xd740
    d720:	20 20       	bra	0x0xd742
    d722:	20 20       	bra	0x0xd744
    d724:	20 20       	bra	0x0xd746
    d726:	20 20       	bra	0x0xd748
    d728:	20 bd       	bra	0x0xd6e7
    d72a:	dd ae       	std	*0xae
    d72c:	ce 01 20    	ldx	#0x120
    d72f:	18 ce d7 53 	ldy	#0xd753
    d733:	c6 20       	ldab	#0x20
    d735:	18 a6 00    	ldaa	0x0,y
    d738:	a7 00       	staa	0x0,x
    d73a:	08          	inx
    d73b:	18 08       	iny
    d73d:	5a          	decb
    d73e:	26 f5       	bne	0x0xd735
    d740:	ce 01 31    	ldx	#0x131
    d743:	96 50       	ldaa	*0x50
    d745:	bd de a8    	jsr	0xdea8
    d748:	ce 01 37    	ldx	#0x137
    d74b:	96 98       	ldaa	*0x98
    d74d:	bd de a8    	jsr	0xdea8
    d750:	7e dc eb    	jmp	0xdceb
    d753:	44          	lsra
    d754:	4c          	inca
    d755:	41          	.byte	0x41
    d756:	59          	rolb
    d757:	31          	ins
    d758:	20 44       	bra	0x0xd79e
    d75a:	4c          	inca
    d75b:	41          	.byte	0x41
    d75c:	59          	rolb
    d75d:	33          	pulb
    d75e:	20 20       	bra	0x0xd780
    d760:	20 20       	bra	0x0xd782
    d762:	20 20       	bra	0x0xd784
    d764:	20 20       	bra	0x0xd786
    d766:	20 20       	bra	0x0xd788
    d768:	20 20       	bra	0x0xd78a
    d76a:	20 20       	bra	0x0xd78c
    d76c:	20 20       	bra	0x0xd78e
    d76e:	20 20       	bra	0x0xd790
    d770:	20 20       	bra	0x0xd792
    d772:	20 7d       	bra	0x0xd7f1
    d774:	01          	nop
    d775:	1d 26 0a    	bclr	0x26,x, #0x0a
    d778:	bd df 1f    	jsr	0xdf1f
    d77b:	ce 10 23    	ldx	#0x1023
    d77e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd77e
    d782:	bd dd ae    	jsr	0xddae
    d785:	ce 01 20    	ldx	#0x120
    d788:	18 ce d7 b6 	ldy	#0xd7b6
    d78c:	c6 20       	ldab	#0x20
    d78e:	18 a6 00    	ldaa	0x0,y
    d791:	a7 00       	staa	0x0,x
    d793:	08          	inx
    d794:	18 08       	iny
    d796:	5a          	decb
    d797:	26 f5       	bne	0x0xd78e
    d799:	ce d7 d6    	ldx	#0xd7d6
    d79c:	d6 4b       	ldab	*0x4b
    d79e:	18 ce 01 30 	ldy	#0x130
    d7a2:	bd df 5b    	jsr	0xdf5b
    d7a5:	ce d7 f2    	ldx	#0xd7f2
    d7a8:	d6 66       	ldab	*0x66
    d7aa:	18 08       	iny
    d7ac:	18 08       	iny
    d7ae:	18 08       	iny
    d7b0:	bd df 5b    	jsr	0xdf5b
    d7b3:	7e dc eb    	jmp	0xdceb
    d7b6:	54          	lsrb
    d7b7:	59          	rolb
    d7b8:	50          	negb
    d7b9:	45          	.byte	0x45
    d7ba:	20 20       	bra	0x0xd7dc
    d7bc:	49          	rola
    d7bd:	4e          	.byte	0x4e
    d7be:	56          	rorb
    d7bf:	54          	lsrb
    d7c0:	20 20       	bra	0x0xd7e2
    d7c2:	20 20       	bra	0x0xd7e4
    d7c4:	20 20       	bra	0x0xd7e6
    d7c6:	20 20       	bra	0x0xd7e8
    d7c8:	20 20       	bra	0x0xd7ea
    d7ca:	20 20       	bra	0x0xd7ec
    d7cc:	20 20       	bra	0x0xd7ee
    d7ce:	20 20       	bra	0x0xd7f0
    d7d0:	20 20       	bra	0x0xd7f2
    d7d2:	20 20       	bra	0x0xd7f4
    d7d4:	20 20       	bra	0x0xd7f6
    d7d6:	4f          	clra
    d7d7:	42          	.byte	0x42
    d7d8:	4c          	inca
    d7d9:	50          	negb
    d7da:	4f          	clra
    d7db:	42          	.byte	0x42
    d7dc:	42          	.byte	0x42
    d7dd:	50          	negb
    d7de:	4f          	clra
    d7df:	42          	.byte	0x42
    d7e0:	48          	asla
    d7e1:	50          	negb
    d7e2:	4f          	clra
    d7e3:	42          	.byte	0x42
    d7e4:	42          	.byte	0x42
    d7e5:	52          	.byte	0x52
    d7e6:	4d          	tsta
    d7e7:	49          	rola
    d7e8:	4e          	.byte	0x4e
    d7e9:	49          	rola
    d7ea:	41          	.byte	0x41
    d7eb:	55          	.byte	0x55
    d7ec:	58          	aslb
    d7ed:	31          	ins
    d7ee:	41          	.byte	0x41
    d7ef:	55          	.byte	0x55
    d7f0:	58          	aslb
    d7f1:	32          	pula
    d7f2:	20 4f       	bra	0x0xd843
    d7f4:	46          	rora
    d7f5:	46          	rora
    d7f6:	45          	.byte	0x45
    d7f7:	4e          	.byte	0x4e
    d7f8:	56          	rorb
    d7f9:	31          	ins
    d7fa:	45          	.byte	0x45
    d7fb:	4e          	.byte	0x4e
    d7fc:	56          	rorb
    d7fd:	33          	pulb
    d7fe:	45          	.byte	0x45
    d7ff:	31          	ins
    d800:	26 33       	bne	0x0xd835
    d802:	bd dd ae    	jsr	0xddae
    d805:	ce 01 20    	ldx	#0x120
    d808:	18 ce d8 39 	ldy	#0xd839
    d80c:	c6 20       	ldab	#0x20
    d80e:	18 a6 00    	ldaa	0x0,y
    d811:	a7 00       	staa	0x0,x
    d813:	08          	inx
    d814:	18 08       	iny
    d816:	5a          	decb
    d817:	26 f5       	bne	0x0xd80e
    d819:	ce 01 30    	ldx	#0x130
    d81c:	96 6e       	ldaa	*0x6e
    d81e:	bd de a8    	jsr	0xdea8
    d821:	08          	inx
    d822:	08          	inx
    d823:	96 48       	ldaa	*0x48
    d825:	bd de a8    	jsr	0xdea8
    d828:	08          	inx
    d829:	08          	inx
    d82a:	96 6f       	ldaa	*0x6f
    d82c:	bd de a8    	jsr	0xdea8
    d82f:	08          	inx
    d830:	08          	inx
    d831:	96 70       	ldaa	*0x70
    d833:	bd df 6f    	jsr	0xdf6f
    d836:	7e dc eb    	jmp	0xdceb
    d839:	20 49       	bra	0x0xd884
    d83b:	4e          	.byte	0x4e
    d83c:	20 4d       	bra	0x0xd88b
    d83e:	49          	rola
    d83f:	58          	aslb
    d840:	20 54       	bra	0x0xd896
    d842:	52          	.byte	0x52
    d843:	47          	asra
    d844:	20 57       	bra	0x0xd89d
    d846:	49          	rola
    d847:	4e          	.byte	0x4e
    d848:	20 20       	bra	0x0xd86a
    d84a:	20 20       	bra	0x0xd86c
    d84c:	20 20       	bra	0x0xd86e
    d84e:	20 20       	bra	0x0xd870
    d850:	20 20       	bra	0x0xd872
    d852:	20 20       	bra	0x0xd874
    d854:	20 20       	bra	0x0xd876
    d856:	20 20       	bra	0x0xd878
    d858:	20 7d       	bra	0x0xd8d7
    d85a:	01          	nop
    d85b:	1d 26 0a    	bclr	0x26,x, #0x0a
    d85e:	bd df 1f    	jsr	0xdf1f
    d861:	ce 10 23    	ldx	#0x1023
    d864:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd864
    d868:	bd dd ae    	jsr	0xddae
    d86b:	ce 01 20    	ldx	#0x120
    d86e:	18 ce d8 a8 	ldy	#0xd8a8
    d872:	c6 20       	ldab	#0x20
    d874:	18 a6 00    	ldaa	0x0,y
    d877:	a7 00       	staa	0x0,x
    d879:	08          	inx
    d87a:	18 08       	iny
    d87c:	5a          	decb
    d87d:	26 f5       	bne	0x0xd874
    d87f:	ce d8 c8    	ldx	#0xd8c8
    d882:	d6 74       	ldab	*0x74
    d884:	18 ce 01 30 	ldy	#0x130
    d888:	bd df 5b    	jsr	0xdf5b
    d88b:	ce da 7d    	ldx	#0xda7d
    d88e:	d6 72       	ldab	*0x72
    d890:	18 08       	iny
    d892:	18 08       	iny
    d894:	18 08       	iny
    d896:	bd df 5b    	jsr	0xdf5b
    d899:	d6 73       	ldab	*0x73
    d89b:	18 ce 01 3b 	ldy	#0x13b
    d89f:	ce d8 dc    	ldx	#0xd8dc
    d8a2:	bd df 5b    	jsr	0xdf5b
    d8a5:	7e dc eb    	jmp	0xdceb
    d8a8:	52          	.byte	0x52
    d8a9:	4e          	.byte	0x4e
    d8aa:	47          	asra
    d8ab:	45          	.byte	0x45
    d8ac:	20 20       	bra	0x0xd8ce
    d8ae:	53          	comb
    d8af:	59          	rolb
    d8b0:	4e          	.byte	0x4e
    d8b1:	43          	coma
    d8b2:	20 4d       	bra	0x0xd901
    d8b4:	4f          	clra
    d8b5:	44          	lsra
    d8b6:	45          	.byte	0x45
    d8b7:	20 20       	bra	0x0xd8d9
    d8b9:	20 20       	bra	0x0xd8db
    d8bb:	20 20       	bra	0x0xd8dd
    d8bd:	20 20       	bra	0x0xd8df
    d8bf:	20 20       	bra	0x0xd8e1
    d8c1:	20 20       	bra	0x0xd8e3
    d8c3:	20 20       	bra	0x0xd8e5
    d8c5:	20 20       	bra	0x0xd8e7
    d8c7:	20 20       	bra	0x0xd8e9
    d8c9:	4f          	clra
    d8ca:	46          	rora
    d8cb:	46          	rora
    d8cc:	31          	ins
    d8cd:	4f          	clra
    d8ce:	43          	coma
    d8cf:	54          	lsrb
    d8d0:	32          	pula
    d8d1:	4f          	clra
    d8d2:	43          	coma
    d8d3:	54          	lsrb
    d8d4:	33          	pulb
    d8d5:	4f          	clra
    d8d6:	43          	coma
    d8d7:	54          	lsrb
    d8d8:	34          	des
    d8d9:	4f          	clra
    d8da:	43          	coma
    d8db:	54          	lsrb
    d8dc:	20 20       	bra	0x0xd8fe
    d8de:	20 20       	bra	0x0xd900
    d8e0:	20 20       	bra	0x0xd902
    d8e2:	55          	.byte	0x55
    d8e3:	50          	negb
    d8e4:	44          	lsra
    d8e5:	4f          	clra
    d8e6:	57          	asrb
    d8e7:	4e          	.byte	0x4e
    d8e8:	55          	.byte	0x55
    d8e9:	50          	negb
    d8ea:	44          	lsra
    d8eb:	4e          	.byte	0x4e
    d8ec:	52          	.byte	0x52
    d8ed:	41          	.byte	0x41
    d8ee:	4e          	.byte	0x4e
    d8ef:	44          	lsra
    d8f0:	7d 01 1d    	tst	0x11d
    d8f3:	26 0a       	bne	0x0xd8ff
    d8f5:	bd df 1f    	jsr	0xdf1f
    d8f8:	ce 10 23    	ldx	#0x1023
    d8fb:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xd8fb
    d8ff:	bd dd ae    	jsr	0xddae
    d902:	86 20       	ldaa	#0x20
    d904:	c6 20       	ldab	#0x20
    d906:	ce 01 20    	ldx	#0x120
    d909:	a7 00       	staa	0x0,x
    d90b:	08          	inx
    d90c:	5a          	decb
    d90d:	26 fa       	bne	0x0xd909
    d90f:	ce d9 94    	ldx	#0xd994
    d912:	12 f4 02 3f 	brset	*0xf4, #0x02, 0x0xd955
    d916:	d6 2d       	ldab	*0x2d
    d918:	18 ce 01 20 	ldy	#0x120
    d91c:	bd df 5b    	jsr	0xdf5b
    d91f:	ce d9 94    	ldx	#0xd994
    d922:	d6 2e       	ldab	*0x2e
    d924:	18 08       	iny
    d926:	18 08       	iny
    d928:	18 08       	iny
    d92a:	bd df 5b    	jsr	0xdf5b
    d92d:	ce d9 94    	ldx	#0xd994
    d930:	d6 2f       	ldab	*0x2f
    d932:	18 08       	iny
    d934:	18 08       	iny
    d936:	bd df 5b    	jsr	0xdf5b
    d939:	ce 01 31    	ldx	#0x131
    d93c:	96 2a       	ldaa	*0x2a
    d93e:	bd de a8    	jsr	0xdea8
    d941:	08          	inx
    d942:	08          	inx
    d943:	08          	inx
    d944:	08          	inx
    d945:	96 2b       	ldaa	*0x2b
    d947:	bd de a8    	jsr	0xdea8
    d94a:	08          	inx
    d94b:	08          	inx
    d94c:	08          	inx
    d94d:	96 2c       	ldaa	*0x2c
    d94f:	bd de a8    	jsr	0xdea8
    d952:	7e dc eb    	jmp	0xdceb
    d955:	d6 3a       	ldab	*0x3a
    d957:	18 ce 01 20 	ldy	#0x120
    d95b:	bd df 5b    	jsr	0xdf5b
    d95e:	ce d9 94    	ldx	#0xd994
    d961:	d6 3b       	ldab	*0x3b
    d963:	18 08       	iny
    d965:	18 08       	iny
    d967:	18 08       	iny
    d969:	bd df 5b    	jsr	0xdf5b
    d96c:	ce d9 94    	ldx	#0xd994
    d96f:	d6 3c       	ldab	*0x3c
    d971:	18 08       	iny
    d973:	18 08       	iny
    d975:	bd df 5b    	jsr	0xdf5b
    d978:	ce 01 31    	ldx	#0x131
    d97b:	96 37       	ldaa	*0x37
    d97d:	bd de a8    	jsr	0xdea8
    d980:	08          	inx
    d981:	08          	inx
    d982:	08          	inx
    d983:	08          	inx
    d984:	96 38       	ldaa	*0x38
    d986:	bd de a8    	jsr	0xdea8
    d989:	08          	inx
    d98a:	08          	inx
    d98b:	08          	inx
    d98c:	96 39       	ldaa	*0x39
    d98e:	bd de a8    	jsr	0xdea8
    d991:	7e dc eb    	jmp	0xdceb
    d994:	20 4f       	bra	0x0xd9e5
    d996:	46          	rora
    d997:	46          	rora
    d998:	46          	rora
    d999:	52          	.byte	0x52
    d99a:	45          	.byte	0x45
    d99b:	31          	ins
    d99c:	46          	rora
    d99d:	52          	.byte	0x52
    d99e:	45          	.byte	0x45
    d99f:	32          	pula
    d9a0:	31          	ins
    d9a1:	26 32       	bne	0x0xd9d5
    d9a3:	46          	rora
    d9a4:	4c          	inca
    d9a5:	45          	.byte	0x45
    d9a6:	56          	rorb
    d9a7:	31          	ins
    d9a8:	4c          	inca
    d9a9:	45          	.byte	0x45
    d9aa:	56          	rorb
    d9ab:	32          	pula
    d9ac:	20 50       	bra	0x0xd9fe
    d9ae:	57          	asrb
    d9af:	31          	ins
    d9b0:	20 50       	bra	0x0xda02
    d9b2:	57          	asrb
    d9b3:	32          	pula
    d9b4:	31          	ins
    d9b5:	26 32       	bne	0x0xd9e9
    d9b7:	50          	negb
    d9b8:	46          	rora
    d9b9:	49          	rola
    d9ba:	4c          	inca
    d9bb:	54          	lsrb
    d9bc:	52          	.byte	0x52
    d9bd:	45          	.byte	0x45
    d9be:	53          	comb
    d9bf:	4f          	clra
    d9c0:	4c          	inca
    d9c1:	45          	.byte	0x45
    d9c2:	56          	rorb
    d9c3:	4e          	.byte	0x4e
    d9c4:	58          	aslb
    d9c5:	4d          	tsta
    d9c6:	4f          	clra
    d9c7:	44          	lsra
    d9c8:	20 45       	bra	0x0xda0f
    d9ca:	41          	.byte	0x41
    d9cb:	31          	ins
    d9cc:	20 45       	bra	0x0xda13
    d9ce:	41          	.byte	0x41
    d9cf:	33          	pulb
    d9d0:	20 45       	bra	0x0xda17
    d9d2:	58          	aslb
    d9d3:	54          	lsrb
    d9d4:	20 56       	bra	0x0xda2c
    d9d6:	4f          	clra
    d9d7:	4c          	inca
    d9d8:	bd dd ae    	jsr	0xddae
    d9db:	ce 01 20    	ldx	#0x120
    d9de:	18 ce da 3d 	ldy	#0xda3d
    d9e2:	c6 20       	ldab	#0x20
    d9e4:	18 a6 00    	ldaa	0x0,y
    d9e7:	a7 00       	staa	0x0,x
    d9e9:	08          	inx
    d9ea:	18 08       	iny
    d9ec:	5a          	decb
    d9ed:	26 f5       	bne	0x0xd9e4
    d9ef:	ce da 5d    	ldx	#0xda5d
    d9f2:	18 ce 01 30 	ldy	#0x130
    d9f6:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xda01
    d9fa:	d6 27       	ldab	*0x27
    d9fc:	bd df 5b    	jsr	0xdf5b
    d9ff:	20 05       	bra	0x0xda06
    da01:	d6 34       	ldab	*0x34
    da03:	bd df 5b    	jsr	0xdf5b
    da06:	ce da 75    	ldx	#0xda75
    da09:	18 08       	iny
    da0b:	18 08       	iny
    da0d:	18 08       	iny
    da0f:	12 f4 02 09 	brset	*0xf4, #0x02, 0x0xda1c
    da13:	d6 31       	ldab	*0x31
    da15:	c4 01       	andb	#0x1
    da17:	bd df 5b    	jsr	0xdf5b
    da1a:	20 06       	bra	0x0xda22
    da1c:	d6 31       	ldab	*0x31
    da1e:	54          	lsrb
    da1f:	bd df 5b    	jsr	0xdf5b
    da22:	ce da 7d    	ldx	#0xda7d
    da25:	18 08       	iny
    da27:	18 08       	iny
    da29:	12 f4 02 08 	brset	*0xf4, #0x02, 0x0xda35
    da2d:	d6 25       	ldab	*0x25
    da2f:	bd df 5b    	jsr	0xdf5b
    da32:	7e dc eb    	jmp	0xdceb
    da35:	d6 32       	ldab	*0x32
    da37:	bd df 5b    	jsr	0xdf5b
    da3a:	7e dc eb    	jmp	0xdceb
    da3d:	57          	asrb
    da3e:	41          	.byte	0x41
    da3f:	56          	rorb
    da40:	45          	.byte	0x45
    da41:	20 20       	bra	0x0xda63
    da43:	4d          	tsta
    da44:	4f          	clra
    da45:	44          	lsra
    da46:	45          	.byte	0x45
    da47:	20 53       	bra	0x0xda9c
    da49:	59          	rolb
    da4a:	4e          	.byte	0x4e
    da4b:	43          	coma
    da4c:	20 20       	bra	0x0xda6e
    da4e:	20 20       	bra	0x0xda70
    da50:	20 20       	bra	0x0xda72
    da52:	20 20       	bra	0x0xda74
    da54:	20 20       	bra	0x0xda76
    da56:	20 20       	bra	0x0xda78
    da58:	20 20       	bra	0x0xda7a
    da5a:	20 20       	bra	0x0xda7c
    da5c:	20 20       	bra	0x0xda7e
    da5e:	54          	lsrb
    da5f:	52          	.byte	0x52
    da60:	49          	rola
    da61:	20 53       	bra	0x0xdab6
    da63:	51          	.byte	0x51
    da64:	52          	.byte	0x52
    da65:	53          	comb
    da66:	57          	asrb
    da67:	55          	.byte	0x55
    da68:	50          	negb
    da69:	53          	comb
    da6a:	57          	asrb
    da6b:	44          	lsra
    da6c:	4e          	.byte	0x4e
    da6d:	52          	.byte	0x52
    da6e:	41          	.byte	0x41
    da6f:	4e          	.byte	0x4e
    da70:	44          	lsra
    da71:	20 53       	bra	0x0xdac6
    da73:	2f 48       	ble	0x0xdabd
    da75:	4d          	tsta
    da76:	4f          	clra
    da77:	4e          	.byte	0x4e
    da78:	4f          	clra
    da79:	50          	negb
    da7a:	4f          	clra
    da7b:	4c          	inca
    da7c:	59          	rolb
    da7d:	53          	comb
    da7e:	45          	.byte	0x45
    da7f:	4c          	inca
    da80:	46          	rora
    da81:	20 20       	bra	0x0xdaa3
    da83:	20 34       	bra	0x0xdab9
    da85:	20 20       	bra	0x0xdaa7
    da87:	20 32       	bra	0x0xdabb
    da89:	20 20       	bra	0x0xdaab
    da8b:	20 31       	bra	0x0xdabe
    da8d:	20 20       	bra	0x0xdaaf
    da8f:	31          	ins
    da90:	54          	lsrb
    da91:	20 31       	bra	0x0xdac4
    da93:	2f 32       	ble	0x0xdac7
    da95:	31          	ins
    da96:	2f 32       	ble	0x0xdaca
    da98:	54          	lsrb
    da99:	20 31       	bra	0x0xdacc
    da9b:	2f 34       	ble	0x0xdad1
    da9d:	31          	ins
    da9e:	2f 34       	ble	0x0xdad4
    daa0:	54          	lsrb
    daa1:	20 31       	bra	0x0xdad4
    daa3:	2f 38       	ble	0x0xdadd
    daa5:	31          	ins
    daa6:	2f 38       	ble	0x0xdae0
    daa8:	54          	lsrb
    daa9:	31          	ins
    daaa:	2f 31       	ble	0x0xdadd
    daac:	36          	psha
    daad:	20 31       	bra	0x0xdae0
    daaf:	36          	psha
    dab0:	54          	lsrb
    dab1:	bd dd ae    	jsr	0xddae
    dab4:	ce 01 20    	ldx	#0x120
    dab7:	18 ce da ee 	ldy	#0xdaee
    dabb:	c6 20       	ldab	#0x20
    dabd:	18 a6 00    	ldaa	0x0,y
    dac0:	a7 00       	staa	0x0,x
    dac2:	08          	inx
    dac3:	18 08       	iny
    dac5:	5a          	decb
    dac6:	26 f5       	bne	0x0xdabd
    dac8:	ce db 17    	ldx	#0xdb17
    dacb:	18 ce 01 37 	ldy	#0x137
    dacf:	12 f4 02 07 	brset	*0xf4, #0x02, 0x0xdada
    dad3:	d6 26       	ldab	*0x26
    dad5:	bd df 47    	jsr	0xdf47
    dad8:	20 05       	bra	0x0xdadf
    dada:	d6 33       	ldab	*0x33
    dadc:	bd df 47    	jsr	0xdf47
    dadf:	d6 96       	ldab	*0x96
    dae1:	ce db 26    	ldx	#0xdb26
    dae4:	18 ce 01 3c 	ldy	#0x13c
    dae8:	bd df 47    	jsr	0xdf47
    daeb:	7e dc eb    	jmp	0xdceb
    daee:	20 20       	bra	0x0xdb10
    daf0:	20 20       	bra	0x0xdb12
    daf2:	20 20       	bra	0x0xdb14
    daf4:	20 4b       	bra	0x0xdb41
    daf6:	45          	.byte	0x45
    daf7:	59          	rolb
    daf8:	20 20       	bra	0x0xdb1a
    dafa:	51          	.byte	0x51
    dafb:	55          	.byte	0x55
    dafc:	41          	.byte	0x41
    dafd:	4e          	.byte	0x4e
    dafe:	20 20       	bra	0x0xdb20
    db00:	20 20       	bra	0x0xdb22
    db02:	20 20       	bra	0x0xdb24
    db04:	20 20       	bra	0x0xdb26
    db06:	20 20       	bra	0x0xdb28
    db08:	20 20       	bra	0x0xdb2a
    db0a:	20 20       	bra	0x0xdb2c
    db0c:	20 20       	bra	0x0xdb2e
    db0e:	4c          	inca
    db0f:	4f          	clra
    db10:	57          	asrb
    db11:	4d          	tsta
    db12:	45          	.byte	0x45
    db13:	44          	lsra
    db14:	20 48       	bra	0x0xdb5e
    db16:	49          	rola
    db17:	4f          	clra
    db18:	46          	rora
    db19:	46          	rora
    db1a:	20 55       	bra	0x0xdb71
    db1c:	50          	negb
    db1d:	20 44       	bra	0x0xdb63
    db1f:	4e          	.byte	0x4e
    db20:	55          	.byte	0x55
    db21:	50          	negb
    db22:	31          	ins
    db23:	44          	lsra
    db24:	4e          	.byte	0x4e
    db25:	31          	ins
    db26:	4f          	clra
    db27:	46          	rora
    db28:	46          	rora
    db29:	4c          	inca
    db2a:	46          	rora
    db2b:	31          	ins
    db2c:	4c          	inca
    db2d:	46          	rora
    db2e:	32          	pula
    db2f:	31          	ins
    db30:	26 32       	bne	0x0xdb64
    db32:	7e ca 55    	jmp	0xca55
    db35:	7d 01 1d    	tst	0x11d
    db38:	26 0a       	bne	0x0xdb44
    db3a:	bd df 1f    	jsr	0xdf1f
    db3d:	ce 10 23    	ldx	#0x1023
    db40:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdb40
    db44:	bd dd ae    	jsr	0xddae
    db47:	ce 01 20    	ldx	#0x120
    db4a:	18 ce db 6e 	ldy	#0xdb6e
    db4e:	c6 20       	ldab	#0x20
    db50:	18 a6 00    	ldaa	0x0,y
    db53:	a7 00       	staa	0x0,x
    db55:	08          	inx
    db56:	18 08       	iny
    db58:	5a          	decb
    db59:	26 f5       	bne	0x0xdb50
    db5b:	ce 01 2c    	ldx	#0x12c
    db5e:	96 b0       	ldaa	*0xb0
    db60:	bd de e4    	jsr	0xdee4
    db63:	ce 01 3c    	ldx	#0x13c
    db66:	96 b1       	ldaa	*0xb1
    db68:	bd de e4    	jsr	0xdee4
    db6b:	7e dc eb    	jmp	0xdceb
    db6e:	56          	rorb
    db6f:	4f          	clra
    db70:	49          	rola
    db71:	43          	coma
    db72:	45          	.byte	0x45
    db73:	20 31       	bra	0x0xdba6
    db75:	20 50       	bra	0x0xdbc7
    db77:	41          	.byte	0x41
    db78:	4e          	.byte	0x4e
    db79:	20 20       	bra	0x0xdb9b
    db7b:	20 20       	bra	0x0xdb9d
    db7d:	20 56       	bra	0x0xdbd5
    db7f:	4f          	clra
    db80:	49          	rola
    db81:	43          	coma
    db82:	45          	.byte	0x45
    db83:	20 32       	bra	0x0xdbb7
    db85:	20 50       	bra	0x0xdbd7
    db87:	41          	.byte	0x41
    db88:	4e          	.byte	0x4e
    db89:	20 20       	bra	0x0xdbab
    db8b:	20 20       	bra	0x0xdbad
    db8d:	20 bd       	bra	0x0xdb4c
    db8f:	dd ae       	std	*0xae
    db91:	ce 01 20    	ldx	#0x120
    db94:	18 ce db b8 	ldy	#0xdbb8
    db98:	c6 20       	ldab	#0x20
    db9a:	18 a6 00    	ldaa	0x0,y
    db9d:	a7 00       	staa	0x0,x
    db9f:	08          	inx
    dba0:	18 08       	iny
    dba2:	5a          	decb
    dba3:	26 f5       	bne	0x0xdb9a
    dba5:	ce 01 2c    	ldx	#0x12c
    dba8:	96 a8       	ldaa	*0xa8
    dbaa:	bd de a8    	jsr	0xdea8
    dbad:	ce 01 3c    	ldx	#0x13c
    dbb0:	96 a9       	ldaa	*0xa9
    dbb2:	bd de a8    	jsr	0xdea8
    dbb5:	7e dc eb    	jmp	0xdceb
    dbb8:	50          	negb
    dbb9:	41          	.byte	0x41
    dbba:	4e          	.byte	0x4e
    dbbb:	31          	ins
    dbbc:	20 4c       	bra	0x0xdc0a
    dbbe:	46          	rora
    dbbf:	4f          	clra
    dbc0:	20 52       	bra	0x0xdc14
    dbc2:	54          	lsrb
    dbc3:	20 20       	bra	0x0xdbe5
    dbc5:	20 20       	bra	0x0xdbe7
    dbc7:	20 50       	bra	0x0xdc19
    dbc9:	41          	.byte	0x41
    dbca:	4e          	.byte	0x4e
    dbcb:	32          	pula
    dbcc:	20 4c       	bra	0x0xdc1a
    dbce:	46          	rora
    dbcf:	4f          	clra
    dbd0:	20 52       	bra	0x0xdc24
    dbd2:	54          	lsrb
    dbd3:	20 20       	bra	0x0xdbf5
    dbd5:	20 20       	bra	0x0xdbf7
    dbd7:	20 bd       	bra	0x0xdb96
    dbd9:	dd ae       	std	*0xae
    dbdb:	ce 01 20    	ldx	#0x120
    dbde:	18 ce dc 02 	ldy	#0xdc02
    dbe2:	c6 20       	ldab	#0x20
    dbe4:	18 a6 00    	ldaa	0x0,y
    dbe7:	a7 00       	staa	0x0,x
    dbe9:	08          	inx
    dbea:	18 08       	iny
    dbec:	5a          	decb
    dbed:	26 f5       	bne	0x0xdbe4
    dbef:	ce 01 2c    	ldx	#0x12c
    dbf2:	96 b8       	ldaa	*0xb8
    dbf4:	bd de a8    	jsr	0xdea8
    dbf7:	ce 01 3c    	ldx	#0x13c
    dbfa:	96 b9       	ldaa	*0xb9
    dbfc:	bd de a8    	jsr	0xdea8
    dbff:	7e dc eb    	jmp	0xdceb
    dc02:	50          	negb
    dc03:	41          	.byte	0x41
    dc04:	4e          	.byte	0x4e
    dc05:	31          	ins
    dc06:	20 4c       	bra	0x0xdc54
    dc08:	46          	rora
    dc09:	4f          	clra
    dc0a:	20 44       	bra	0x0xdc50
    dc0c:	50          	negb
    dc0d:	20 20       	bra	0x0xdc2f
    dc0f:	20 20       	bra	0x0xdc31
    dc11:	20 50       	bra	0x0xdc63
    dc13:	41          	.byte	0x41
    dc14:	4e          	.byte	0x4e
    dc15:	32          	pula
    dc16:	20 4c       	bra	0x0xdc64
    dc18:	46          	rora
    dc19:	4f          	clra
    dc1a:	20 44       	bra	0x0xdc60
    dc1c:	50          	negb
    dc1d:	20 20       	bra	0x0xdc3f
    dc1f:	20 20       	bra	0x0xdc41
    dc21:	20 bd       	bra	0x0xdbe0
    dc23:	dd ae       	std	*0xae
    dc25:	ce 01 20    	ldx	#0x120
    dc28:	18 ce dc 60 	ldy	#0xdc60
    dc2c:	c6 20       	ldab	#0x20
    dc2e:	18 a6 00    	ldaa	0x0,y
    dc31:	a7 00       	staa	0x0,x
    dc33:	08          	inx
    dc34:	18 08       	iny
    dc36:	5a          	decb
    dc37:	26 f5       	bne	0x0xdc2e
    dc39:	ce da 5d    	ldx	#0xda5d
    dc3c:	d6 a7       	ldab	*0xa7
    dc3e:	18 ce 01 30 	ldy	#0x130
    dc42:	bd df 5b    	jsr	0xdf5b
    dc45:	ce db 17    	ldx	#0xdb17
    dc48:	d6 a6       	ldab	*0xa6
    dc4a:	18 ce 01 37 	ldy	#0x137
    dc4e:	bd df 47    	jsr	0xdf47
    dc51:	ce da 7d    	ldx	#0xda7d
    dc54:	d6 a5       	ldab	*0xa5
    dc56:	18 ce 01 3b 	ldy	#0x13b
    dc5a:	bd df 5b    	jsr	0xdf5b
    dc5d:	7e dc eb    	jmp	0xdceb
    dc60:	57          	asrb
    dc61:	41          	.byte	0x41
    dc62:	56          	rorb
    dc63:	45          	.byte	0x45
    dc64:	20 20       	bra	0x0xdc86
    dc66:	20 4b       	bra	0x0xdcb3
    dc68:	45          	.byte	0x45
    dc69:	59          	rolb
    dc6a:	20 53       	bra	0x0xdcbf
    dc6c:	59          	rolb
    dc6d:	4e          	.byte	0x4e
    dc6e:	43          	coma
    dc6f:	20 20       	bra	0x0xdc91
    dc71:	20 20       	bra	0x0xdc93
    dc73:	20 20       	bra	0x0xdc95
    dc75:	20 20       	bra	0x0xdc97
    dc77:	20 20       	bra	0x0xdc99
    dc79:	20 20       	bra	0x0xdc9b
    dc7b:	20 20       	bra	0x0xdc9d
    dc7d:	20 20       	bra	0x0xdc9f
    dc7f:	20 7d       	bra	0x0xdcfe
    dc81:	01          	nop
    dc82:	1d 26 0a    	bclr	0x26,x, #0x0a
    dc85:	bd df 1f    	jsr	0xdf1f
    dc88:	ce 10 23    	ldx	#0x1023
    dc8b:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdc8b
    dc8f:	bd dd ae    	jsr	0xddae
    dc92:	ce 01 20    	ldx	#0x120
    dc95:	18 ce dc b8 	ldy	#0xdcb8
    dc99:	c6 20       	ldab	#0x20
    dc9b:	18 a6 00    	ldaa	0x0,y
    dc9e:	a7 00       	staa	0x0,x
    dca0:	08          	inx
    dca1:	18 08       	iny
    dca3:	5a          	decb
    dca4:	26 f5       	bne	0x0xdc9b
    dca6:	f6 10 28    	ldab	0x1028
    dca9:	c4 03       	andb	#0x3
    dcab:	ce dc d8    	ldx	#0xdcd8
    dcae:	18 ce 01 30 	ldy	#0x130
    dcb2:	bd df 5b    	jsr	0xdf5b
    dcb5:	7e dc eb    	jmp	0xdceb
    dcb8:	53          	comb
    dcb9:	45          	.byte	0x45
    dcba:	54          	lsrb
    dcbb:	20 53       	bra	0x0xdd10
    dcbd:	50          	negb
    dcbe:	49          	rola
    dcbf:	20 42       	bra	0x0xdd03
    dcc1:	49          	rola
    dcc2:	54          	lsrb
    dcc3:	20 52       	bra	0x0xdd17
    dcc5:	41          	.byte	0x41
    dcc6:	54          	lsrb
    dcc7:	45          	.byte	0x45
    dcc8:	20 20       	bra	0x0xdcea
    dcca:	20 20       	bra	0x0xdcec
    dccc:	20 4b       	bra	0x0xdd19
    dcce:	42          	.byte	0x42
    dccf:	49          	rola
    dcd0:	54          	lsrb
    dcd1:	2f 53       	ble	0x0xdd26
    dcd3:	45          	.byte	0x45
    dcd4:	43          	coma
    dcd5:	20 20       	bra	0x0xdcf7
    dcd7:	20 20       	bra	0x0xdcf9
    dcd9:	20 31       	bra	0x0xdd0c
    dcdb:	4b          	.byte	0x4b
    dcdc:	20 35       	bra	0x0xdd13
    dcde:	30          	tsx
    dcdf:	30          	tsx
    dce0:	20 31       	bra	0x0xdd13
    dce2:	32          	pula
    dce3:	35          	txs
    dce4:	36          	psha
    dce5:	32          	pula
    dce6:	2e 35       	bgt	0x0xdd1d
    dce8:	7e dc eb    	jmp	0xdceb
    dceb:	7f 01 1e    	clr	0x11e
    dcee:	86 20       	ldaa	#0x20
    dcf0:	b7 01 1c    	staa	0x11c
    dcf3:	ce 10 23    	ldx	#0x1023
    dcf6:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdcf6
    dcfa:	7e dc fd    	jmp	0xdcfd
    dcfd:	8d 03       	bsr	0x0xdd02
    dcff:	7e ca 55    	jmp	0xca55
    dd02:	ce 01 20    	ldx	#0x120
    dd05:	f6 01 1c    	ldab	0x11c
    dd08:	5a          	decb
    dd09:	3a          	abx
    dd0a:	a6 00       	ldaa	0x0,x
    dd0c:	b7 10 47    	staa	0x1047
    dd0f:	86 88       	ldaa	#0x88
    dd11:	ba 10 00    	oraa	0x1000
    dd14:	b7 10 00    	staa	0x1000
    dd17:	88 80       	eora	#0x80
    dd19:	b7 10 00    	staa	0x1000
    dd1c:	88 08       	eora	#0x8
    dd1e:	b7 10 00    	staa	0x1000
    dd21:	37          	pshb
    dd22:	bd dd 7a    	jsr	0xdd7a
    dd25:	33          	pulb
    dd26:	f7 01 1c    	stab	0x11c
    dd29:	c1 10       	cmpb	#0x10
    dd2b:	27 01       	beq	0x0xdd2e
    dd2d:	39          	rts
    dd2e:	18 ce 10 23 	ldy	#0x1023
    dd32:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xdd32
    dd36:	fb 
    dd37:	86 8f       	ldaa	#0x8f
    dd39:	b7 10 47    	staa	0x1047
    dd3c:	86 80       	ldaa	#0x80
    dd3e:	ba 10 00    	oraa	0x1000
    dd41:	b7 10 00    	staa	0x1000
    dd44:	88 80       	eora	#0x80
    dd46:	b7 10 00    	staa	0x1000
    dd49:	bd dd 7a    	jsr	0xdd7a
    dd4c:	39          	rts
    dd4d:	f6 10 23    	ldab	0x1023
    dd50:	c5 10       	bitb	#0x10
    dd52:	27 f9       	beq	0x0xdd4d
    dd54:	a6 00       	ldaa	0x0,x
    dd56:	b7 10 47    	staa	0x1047
    dd59:	86 88       	ldaa	#0x88
    dd5b:	ba 10 00    	oraa	0x1000
    dd5e:	b7 10 00    	staa	0x1000
    dd61:	88 80       	eora	#0x80
    dd63:	b7 10 00    	staa	0x1000
    dd66:	88 08       	eora	#0x8
    dd68:	b7 10 00    	staa	0x1000
    dd6b:	86 10       	ldaa	#0x10
    dd6d:	b7 10 23    	staa	0x1023
    dd70:	fc 10 0e    	ldd	0x100e
    dd73:	c3 00 f0    	addd	#0xf0
    dd76:	fd 10 1c    	std	0x101c
    dd79:	39          	rts
    dd7a:	86 10       	ldaa	#0x10
    dd7c:	b7 10 23    	staa	0x1023
    dd7f:	fc 10 0e    	ldd	0x100e
    dd82:	c3 00 f0    	addd	#0xf0
    dd85:	fd 10 1c    	std	0x101c
    dd88:	39          	rts
    dd89:	f6 10 23    	ldab	0x1023
    dd8c:	c5 10       	bitb	#0x10
    dd8e:	27 f9       	beq	0x0xdd89
    dd90:	b6 01 1e    	ldaa	0x11e
    dd93:	81 0f       	cmpa	#0xf
    dd95:	23 02       	bls	0x0xdd99
    dd97:	8b 30       	adda	#0x30
    dd99:	8a 80       	oraa	#0x80
    dd9b:	b7 10 47    	staa	0x1047
    dd9e:	86 80       	ldaa	#0x80
    dda0:	ba 10 00    	oraa	0x1000
    dda3:	b7 10 00    	staa	0x1000
    dda6:	88 80       	eora	#0x80
    dda8:	b7 10 00    	staa	0x1000
    ddab:	7e dd 7a    	jmp	0xdd7a
    ddae:	ce 10 23    	ldx	#0x1023
    ddb1:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xddb1
    ddb5:	86 cf       	ldaa	#0xcf
    ddb7:	b7 10 47    	staa	0x1047
    ddba:	86 80       	ldaa	#0x80
    ddbc:	ba 10 00    	oraa	0x1000
    ddbf:	b7 10 00    	staa	0x1000
    ddc2:	01          	nop
    ddc3:	88 80       	eora	#0x80
    ddc5:	b7 10 00    	staa	0x1000
    ddc8:	7e dd 7a    	jmp	0xdd7a
    ddcb:	b6 01 1e    	ldaa	0x11e
    ddce:	81 03       	cmpa	#0x3
    ddd0:	22 04       	bhi	0x0xddd6
    ddd2:	86 03       	ldaa	#0x3
    ddd4:	20 22       	bra	0x0xddf8
    ddd6:	81 09       	cmpa	#0x9
    ddd8:	22 04       	bhi	0x0xddde
    ddda:	86 09       	ldaa	#0x9
    dddc:	20 1a       	bra	0x0xddf8
    ddde:	81 0e       	cmpa	#0xe
    dde0:	22 04       	bhi	0x0xdde6
    dde2:	86 0e       	ldaa	#0xe
    dde4:	20 12       	bra	0x0xddf8
    dde6:	81 13       	cmpa	#0x13
    dde8:	22 04       	bhi	0x0xddee
    ddea:	86 13       	ldaa	#0x13
    ddec:	20 0a       	bra	0x0xddf8
    ddee:	81 19       	cmpa	#0x19
    ddf0:	22 04       	bhi	0x0xddf6
    ddf2:	86 19       	ldaa	#0x19
    ddf4:	20 02       	bra	0x0xddf8
    ddf6:	86 1e       	ldaa	#0x1e
    ddf8:	b7 01 1e    	staa	0x11e
    ddfb:	7e dd 89    	jmp	0xdd89
    ddfe:	86 02       	ldaa	#0x2
    de00:	b1 01 1e    	cmpa	0x11e
    de03:	23 06       	bls	0x0xde0b
    de05:	b7 01 1e    	staa	0x11e
    de08:	7e dd 89    	jmp	0xdd89
    de0b:	86 06       	ldaa	#0x6
    de0d:	b1 01 1e    	cmpa	0x11e
    de10:	23 06       	bls	0x0xde18
    de12:	b7 01 1e    	staa	0x11e
    de15:	7e dd 89    	jmp	0xdd89
    de18:	86 0a       	ldaa	#0xa
    de1a:	b1 01 1e    	cmpa	0x11e
    de1d:	23 06       	bls	0x0xde25
    de1f:	b7 01 1e    	staa	0x11e
    de22:	7e dd 89    	jmp	0xdd89
    de25:	86 0e       	ldaa	#0xe
    de27:	b1 01 1e    	cmpa	0x11e
    de2a:	23 06       	bls	0x0xde32
    de2c:	b7 01 1e    	staa	0x11e
    de2f:	7e dd 89    	jmp	0xdd89
    de32:	86 12       	ldaa	#0x12
    de34:	b1 01 1e    	cmpa	0x11e
    de37:	23 06       	bls	0x0xde3f
    de39:	b7 01 1e    	staa	0x11e
    de3c:	7e dd 89    	jmp	0xdd89
    de3f:	86 16       	ldaa	#0x16
    de41:	b1 01 1e    	cmpa	0x11e
    de44:	23 06       	bls	0x0xde4c
    de46:	b7 01 1e    	staa	0x11e
    de49:	7e dd 89    	jmp	0xdd89
    de4c:	86 1a       	ldaa	#0x1a
    de4e:	b1 01 1e    	cmpa	0x11e
    de51:	23 06       	bls	0x0xde59
    de53:	b7 01 1e    	staa	0x11e
    de56:	7e dd 89    	jmp	0xdd89
    de59:	86 1e       	ldaa	#0x1e
    de5b:	b7 01 1e    	staa	0x11e
    de5e:	7e dd 89    	jmp	0xdd89
    de61:	7d 01 1e    	tst	0x11e
    de64:	2a 06       	bpl	0x0xde6c
    de66:	7f 01 1e    	clr	0x11e
    de69:	7e dd 89    	jmp	0xdd89
    de6c:	86 03       	ldaa	#0x3
    de6e:	b1 01 1e    	cmpa	0x11e
    de71:	23 06       	bls	0x0xde79
    de73:	b7 01 1e    	staa	0x11e
    de76:	7e dd 89    	jmp	0xdd89
    de79:	86 05       	ldaa	#0x5
    de7b:	b1 01 1e    	cmpa	0x11e
    de7e:	23 06       	bls	0x0xde86
    de80:	b7 01 1e    	staa	0x11e
    de83:	7e dd 89    	jmp	0xdd89
    de86:	86 09       	ldaa	#0x9
    de88:	b1 01 1e    	cmpa	0x11e
    de8b:	23 06       	bls	0x0xde93
    de8d:	b7 01 1e    	staa	0x11e
    de90:	7e dd 89    	jmp	0xdd89
    de93:	86 0c       	ldaa	#0xc
    de95:	b1 01 1e    	cmpa	0x11e
    de98:	23 06       	bls	0x0xdea0
    de9a:	b7 01 1e    	staa	0x11e
    de9d:	7e dd 89    	jmp	0xdd89
    dea0:	86 0f       	ldaa	#0xf
    dea2:	b7 01 1e    	staa	0x11e
    dea5:	7e dd 89    	jmp	0xdd89
    dea8:	80 64       	suba	#0x64
    deaa:	24 09       	bcc	0x0xdeb5
    deac:	8b 64       	adda	#0x64
    deae:	c6 20       	ldab	#0x20
    deb0:	e7 00       	stab	0x0,x
    deb2:	08          	inx
    deb3:	20 05       	bra	0x0xdeba
    deb5:	c6 31       	ldab	#0x31
    deb7:	e7 00       	stab	0x0,x
    deb9:	08          	inx
    deba:	80 0a       	suba	#0xa
    debc:	24 14       	bcc	0x0xded2
    debe:	8b 0a       	adda	#0xa
    dec0:	c1 20       	cmpb	#0x20
    dec2:	27 07       	beq	0x0xdecb
    dec4:	c6 30       	ldab	#0x30
    dec6:	e7 00       	stab	0x0,x
    dec8:	08          	inx
    dec9:	20 14       	bra	0x0xdedf
    decb:	c6 20       	ldab	#0x20
    decd:	e7 00       	stab	0x0,x
    decf:	08          	inx
    ded0:	20 0d       	bra	0x0xdedf
    ded2:	5f          	clrb
    ded3:	5c          	incb
    ded4:	80 0a       	suba	#0xa
    ded6:	24 fb       	bcc	0x0xded3
    ded8:	8b 0a       	adda	#0xa
    deda:	cb 30       	addb	#0x30
    dedc:	e7 00       	stab	0x0,x
    dede:	08          	inx
    dedf:	8b 30       	adda	#0x30
    dee1:	a7 00       	staa	0x0,x
    dee3:	39          	rts
    dee4:	80 40       	suba	#0x40
    dee6:	25 10       	bcs	0x0xdef8
    dee8:	26 05       	bne	0x0xdeef
    deea:	8d bc       	bsr	0x0xdea8
    deec:	09          	dex
    deed:	09          	dex
    deee:	39          	rts
    deef:	8d b7       	bsr	0x0xdea8
    def1:	09          	dex
    def2:	09          	dex
    def3:	86 2b       	ldaa	#0x2b
    def5:	a7 00       	staa	0x0,x
    def7:	39          	rts
    def8:	40          	nega
    def9:	8d ad       	bsr	0x0xdea8
    defb:	09          	dex
    defc:	09          	dex
    defd:	86 2d       	ldaa	#0x2d
    deff:	a7 00       	staa	0x0,x
    df01:	39          	rts
    df02:	a6 02       	ldaa	0x2,x
    df04:	80 30       	suba	#0x30
    df06:	e6 01       	ldab	0x1,x
    df08:	c1 20       	cmpb	#0x20
    df0a:	26 01       	bne	0x0xdf0d
    df0c:	39          	rts
    df0d:	c0 30       	subb	#0x30
    df0f:	36          	psha
    df10:	86 0a       	ldaa	#0xa
    df12:	3d          	mul
    df13:	32          	pula
    df14:	1b          	aba
    df15:	e6 00       	ldab	0x0,x
    df17:	c1 20       	cmpb	#0x20
    df19:	26 01       	bne	0x0xdf1c
    df1b:	39          	rts
    df1c:	8b 64       	adda	#0x64
    df1e:	39          	rts
    df1f:	ce 10 23    	ldx	#0x1023
    df22:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xdf22
    df26:	86 f7       	ldaa	#0xf7
    df28:	b4 10 00    	anda	0x1000
    df2b:	b7 10 00    	staa	0x1000
    df2e:	86 0e       	ldaa	#0xe
    df30:	b7 10 47    	staa	0x1047
    df33:	86 80       	ldaa	#0x80
    df35:	ba 10 00    	oraa	0x1000
    df38:	b7 10 00    	staa	0x1000
    df3b:	01          	nop
    df3c:	88 80       	eora	#0x80
    df3e:	b7 10 00    	staa	0x1000
    df41:	7c 01 1d    	inc	0x11d
    df44:	7e dd 7a    	jmp	0xdd7a
    df47:	86 03       	ldaa	#0x3
    df49:	3d          	mul
    df4a:	3a          	abx
    df4b:	c6 03       	ldab	#0x3
    df4d:	a6 00       	ldaa	0x0,x
    df4f:	18 a7 00    	staa	0x0,y
    df52:	5a          	decb
    df53:	27 05       	beq	0x0xdf5a
    df55:	08          	inx
    df56:	18 08       	iny
    df58:	20 f3       	bra	0x0xdf4d
    df5a:	39          	rts
    df5b:	86 04       	ldaa	#0x4
    df5d:	3d          	mul
    df5e:	3a          	abx
    df5f:	c6 04       	ldab	#0x4
    df61:	a6 00       	ldaa	0x0,x
    df63:	18 a7 00    	staa	0x0,y
    df66:	5a          	decb
    df67:	27 05       	beq	0x0xdf6e
    df69:	08          	inx
    df6a:	18 08       	iny
    df6c:	20 f3       	bra	0x0xdf61
    df6e:	39          	rts
    df6f:	c6 c8       	ldab	#0xc8
    df71:	3d          	mul
    df72:	bd de a8    	jsr	0xdea8
    df75:	86 25       	ldaa	#0x25
    df77:	09          	dex
    df78:	09          	dex
    df79:	a7 00       	staa	0x0,x
    df7b:	39          	rts
    df7c:	b6 01 1e    	ldaa	0x11e
    df7f:	81 0e       	cmpa	#0xe
    df81:	22 08       	bhi	0x0xdf8b
    df83:	8b 10       	adda	#0x10
    df85:	b7 01 1e    	staa	0x11e
    df88:	7e dd 89    	jmp	0xdd89
    df8b:	80 10       	suba	#0x10
    df8d:	b7 01 1e    	staa	0x11e
    df90:	7e dd 89    	jmp	0xdd89
    df93:	b6 01 7f    	ldaa	0x17f
    df96:	2b 1e       	bmi	0x0xdfb6
    df98:	b6 01 1e    	ldaa	0x11e
    df9b:	81 0e       	cmpa	#0xe
    df9d:	22 0a       	bhi	0x0xdfa9
    df9f:	27 32       	beq	0x0xdfd3
    dfa1:	86 0e       	ldaa	#0xe
    dfa3:	b7 01 1e    	staa	0x11e
    dfa6:	7e dd 89    	jmp	0xdd89
    dfa9:	81 1e       	cmpa	#0x1e
    dfab:	26 01       	bne	0x0xdfae
    dfad:	39          	rts
    dfae:	86 1e       	ldaa	#0x1e
    dfb0:	b7 01 1e    	staa	0x11e
    dfb3:	7e dd 89    	jmp	0xdd89
    dfb6:	b6 01 1e    	ldaa	0x11e
    dfb9:	81 19       	cmpa	#0x19
    dfbb:	25 0a       	bcs	0x0xdfc7
    dfbd:	27 14       	beq	0x0xdfd3
    dfbf:	86 19       	ldaa	#0x19
    dfc1:	b7 01 1e    	staa	0x11e
    dfc4:	7e dd 89    	jmp	0xdd89
    dfc7:	81 09       	cmpa	#0x9
    dfc9:	27 08       	beq	0x0xdfd3
    dfcb:	86 09       	ldaa	#0x9
    dfcd:	b7 01 1e    	staa	0x11e
    dfd0:	7e dd 89    	jmp	0xdd89
    dfd3:	39          	rts
    dfd4:	b6 01 7f    	ldaa	0x17f
    dfd7:	2b 35       	bmi	0x0xe00e
    dfd9:	b6 01 1e    	ldaa	0x11e
    dfdc:	81 13       	cmpa	#0x13
    dfde:	25 16       	bcs	0x0xdff6
    dfe0:	22 08       	bhi	0x0xdfea
    dfe2:	86 19       	ldaa	#0x19
    dfe4:	b7 01 1e    	staa	0x11e
    dfe7:	7e dd 89    	jmp	0xdd89
    dfea:	81 19       	cmpa	#0x19
    dfec:	22 57       	bhi	0x0xe045
    dfee:	86 1e       	ldaa	#0x1e
    dff0:	b7 01 1e    	staa	0x11e
    dff3:	7e dd 89    	jmp	0xdd89
    dff6:	81 03       	cmpa	#0x3
    dff8:	22 08       	bhi	0x0xe002
    dffa:	86 09       	ldaa	#0x9
    dffc:	b7 01 1e    	staa	0x11e
    dfff:	7e dd 89    	jmp	0xdd89
    e002:	81 09       	cmpa	#0x9
    e004:	22 3f       	bhi	0x0xe045
    e006:	86 0e       	ldaa	#0xe
    e008:	b7 01 1e    	staa	0x11e
    e00b:	7e dd 89    	jmp	0xdd89
    e00e:	b6 01 1e    	ldaa	0x11e
    e011:	81 0f       	cmpa	#0xf
    e013:	25 18       	bcs	0x0xe02d
    e015:	81 1a       	cmpa	#0x1a
    e017:	25 08       	bcs	0x0xe021
    e019:	86 19       	ldaa	#0x19
    e01b:	b7 01 1e    	staa	0x11e
    e01e:	7e dd 89    	jmp	0xdd89
    e021:	81 19       	cmpa	#0x19
    e023:	26 20       	bne	0x0xe045
    e025:	86 13       	ldaa	#0x13
    e027:	b7 01 1e    	staa	0x11e
    e02a:	7e dd 89    	jmp	0xdd89
    e02d:	81 0e       	cmpa	#0xe
    e02f:	25 08       	bcs	0x0xe039
    e031:	86 09       	ldaa	#0x9
    e033:	b7 01 1e    	staa	0x11e
    e036:	7e dd 89    	jmp	0xdd89
    e039:	81 09       	cmpa	#0x9
    e03b:	26 08       	bne	0x0xe045
    e03d:	86 03       	ldaa	#0x3
    e03f:	b7 01 1e    	staa	0x11e
    e042:	7e dd 89    	jmp	0xdd89
    e045:	39          	rts
    e046:	b6 01 7f    	ldaa	0x17f
    e049:	2b 3a       	bmi	0x0xe085
    e04b:	b6 01 1e    	ldaa	0x11e
    e04e:	81 12       	cmpa	#0x12
    e050:	25 16       	bcs	0x0xe068
    e052:	22 04       	bhi	0x0xe058
    e054:	86 16       	ldaa	#0x16
    e056:	20 26       	bra	0x0xe07e
    e058:	81 16       	cmpa	#0x16
    e05a:	22 04       	bhi	0x0xe060
    e05c:	86 1a       	ldaa	#0x1a
    e05e:	20 1e       	bra	0x0xe07e
    e060:	81 1a       	cmpa	#0x1a
    e062:	22 20       	bhi	0x0xe084
    e064:	86 1e       	ldaa	#0x1e
    e066:	20 16       	bra	0x0xe07e
    e068:	81 02       	cmpa	#0x2
    e06a:	22 04       	bhi	0x0xe070
    e06c:	86 06       	ldaa	#0x6
    e06e:	20 0e       	bra	0x0xe07e
    e070:	81 06       	cmpa	#0x6
    e072:	22 04       	bhi	0x0xe078
    e074:	86 0a       	ldaa	#0xa
    e076:	20 06       	bra	0x0xe07e
    e078:	81 0a       	cmpa	#0xa
    e07a:	22 08       	bhi	0x0xe084
    e07c:	86 0e       	ldaa	#0xe
    e07e:	b7 01 1e    	staa	0x11e
    e081:	7e dd 89    	jmp	0xdd89
    e084:	39          	rts
    e085:	b6 01 1e    	ldaa	0x11e
    e088:	81 0e       	cmpa	#0xe
    e08a:	22 16       	bhi	0x0xe0a2
    e08c:	25 04       	bcs	0x0xe092
    e08e:	86 0a       	ldaa	#0xa
    e090:	20 ec       	bra	0x0xe07e
    e092:	81 0a       	cmpa	#0xa
    e094:	25 04       	bcs	0x0xe09a
    e096:	86 06       	ldaa	#0x6
    e098:	20 e4       	bra	0x0xe07e
    e09a:	81 06       	cmpa	#0x6
    e09c:	25 e6       	bcs	0x0xe084
    e09e:	86 02       	ldaa	#0x2
    e0a0:	20 dc       	bra	0x0xe07e
    e0a2:	81 1e       	cmpa	#0x1e
    e0a4:	25 04       	bcs	0x0xe0aa
    e0a6:	86 1a       	ldaa	#0x1a
    e0a8:	20 d4       	bra	0x0xe07e
    e0aa:	81 1a       	cmpa	#0x1a
    e0ac:	25 04       	bcs	0x0xe0b2
    e0ae:	86 16       	ldaa	#0x16
    e0b0:	20 cc       	bra	0x0xe07e
    e0b2:	81 16       	cmpa	#0x16
    e0b4:	25 ce       	bcs	0x0xe084
    e0b6:	86 12       	ldaa	#0x12
    e0b8:	20 c4       	bra	0x0xe07e
    e0ba:	ce 10 23    	ldx	#0x1023
    e0bd:	1f 00 10 f9 	brclr	0x0,x, #0x10, 0x0xe0ba
    e0c1:	86 04       	ldaa	#0x4
    e0c3:	b7 01 1c    	staa	0x11c
    e0c6:	86 38       	ldaa	#0x38
    e0c8:	b7 10 47    	staa	0x1047
    e0cb:	86 80       	ldaa	#0x80
    e0cd:	ba 10 00    	oraa	0x1000
    e0d0:	b7 10 00    	staa	0x1000
    e0d3:	01          	nop
    e0d4:	88 80       	eora	#0x80
    e0d6:	b7 10 00    	staa	0x1000
    e0d9:	96 10       	ldaa	*0x10
    e0db:	b7 10 23    	staa	0x1023
    e0de:	fc 10 0e    	ldd	0x100e
    e0e1:	c3 20 08    	addd	#0x2008
    e0e4:	fd 10 1c    	std	0x101c
    e0e7:	ce 10 23    	ldx	#0x1023
    e0ea:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe0ea
    e0ee:	7a 01 1c    	dec	0x11c
    e0f1:	26 d3       	bne	0x0xe0c6
    e0f3:	86 08       	ldaa	#0x8
    e0f5:	b7 10 47    	staa	0x1047
    e0f8:	86 80       	ldaa	#0x80
    e0fa:	ba 10 00    	oraa	0x1000
    e0fd:	b7 10 00    	staa	0x1000
    e100:	01          	nop
    e101:	88 80       	eora	#0x80
    e103:	b7 10 00    	staa	0x1000
    e106:	86 10       	ldaa	#0x10
    e108:	b7 10 23    	staa	0x1023
    e10b:	fc 10 0e    	ldd	0x100e
    e10e:	c3 00 f0    	addd	#0xf0
    e111:	fd 10 1c    	std	0x101c
    e114:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe114
    e118:	86 01       	ldaa	#0x1
    e11a:	b7 10 47    	staa	0x1047
    e11d:	86 80       	ldaa	#0x80
    e11f:	ba 10 00    	oraa	0x1000
    e122:	b7 10 00    	staa	0x1000
    e125:	01          	nop
    e126:	88 80       	eora	#0x80
    e128:	b7 10 00    	staa	0x1000
    e12b:	86 10       	ldaa	#0x10
    e12d:	b7 10 23    	staa	0x1023
    e130:	fc 10 0e    	ldd	0x100e
    e133:	c3 26 48    	addd	#0x2648
    e136:	fd 10 1c    	std	0x101c
    e139:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe139
    e13d:	86 04       	ldaa	#0x4
    e13f:	b7 10 47    	staa	0x1047
    e142:	86 80       	ldaa	#0x80
    e144:	ba 10 00    	oraa	0x1000
    e147:	b7 10 00    	staa	0x1000
    e14a:	01          	nop
    e14b:	88 80       	eora	#0x80
    e14d:	b7 10 00    	staa	0x1000
    e150:	86 10       	ldaa	#0x10
    e152:	b7 10 23    	staa	0x1023
    e155:	fc 10 0e    	ldd	0x100e
    e158:	c3 00 f0    	addd	#0xf0
    e15b:	fd 10 1c    	std	0x101c
    e15e:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe15e
    e162:	86 0c       	ldaa	#0xc
    e164:	b7 10 47    	staa	0x1047
    e167:	b6 10 00    	ldaa	0x1000
    e16a:	8a 80       	oraa	#0x80
    e16c:	b7 10 00    	staa	0x1000
    e16f:	88 80       	eora	#0x80
    e171:	b7 10 00    	staa	0x1000
    e174:	7f 01 1d    	clr	0x11d
    e177:	86 10       	ldaa	#0x10
    e179:	b7 10 23    	staa	0x1023
    e17c:	fc 10 0e    	ldd	0x100e
    e17f:	c3 00 f0    	addd	#0xf0
    e182:	fd 10 1c    	std	0x101c
    e185:	ce 01 20    	ldx	#0x120
    e188:	18 ce e2 5a 	ldy	#0xe25a
    e18c:	c6 20       	ldab	#0x20
    e18e:	18 a6 00    	ldaa	0x0,y
    e191:	a7 00       	staa	0x0,x
    e193:	08          	inx
    e194:	18 08       	iny
    e196:	5a          	decb
    e197:	26 f5       	bne	0x0xe18e
    e199:	7f 01 1e    	clr	0x11e
    e19c:	86 20       	ldaa	#0x20
    e19e:	b7 01 1c    	staa	0x11c
    e1a1:	ce 10 23    	ldx	#0x1023
    e1a4:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe1a4
    e1a8:	86 cf       	ldaa	#0xcf
    e1aa:	b7 10 47    	staa	0x1047
    e1ad:	86 80       	ldaa	#0x80
    e1af:	ba 10 00    	oraa	0x1000
    e1b2:	b7 10 00    	staa	0x1000
    e1b5:	01          	nop
    e1b6:	88 80       	eora	#0x80
    e1b8:	b7 10 00    	staa	0x1000
    e1bb:	86 10       	ldaa	#0x10
    e1bd:	b7 10 23    	staa	0x1023
    e1c0:	fc 10 0e    	ldd	0x100e
    e1c3:	c3 00 f0    	addd	#0xf0
    e1c6:	fd 10 1c    	std	0x101c
    e1c9:	1f 00 10 fc 	brclr	0x0,x, #0x10, 0x0xe1c9
    e1cd:	ce 01 20    	ldx	#0x120
    e1d0:	f6 01 1c    	ldab	0x11c
    e1d3:	5a          	decb
    e1d4:	2a 22       	bpl	0x0xe1f8
    e1d6:	86 cf       	ldaa	#0xcf
    e1d8:	b7 10 47    	staa	0x1047
    e1db:	86 80       	ldaa	#0x80
    e1dd:	ba 10 00    	oraa	0x1000
    e1e0:	b7 10 00    	staa	0x1000
    e1e3:	01          	nop
    e1e4:	88 80       	eora	#0x80
    e1e6:	b7 10 00    	staa	0x1000
    e1e9:	86 10       	ldaa	#0x10
    e1eb:	b7 10 23    	staa	0x1023
    e1ee:	fc 10 0e    	ldd	0x100e
    e1f1:	c3 00 f0    	addd	#0xf0
    e1f4:	fd 10 1c    	std	0x101c
    e1f7:	39          	rts
    e1f8:	3a          	abx
    e1f9:	a6 00       	ldaa	0x0,x
    e1fb:	b7 10 47    	staa	0x1047
    e1fe:	86 88       	ldaa	#0x88
    e200:	ba 10 00    	oraa	0x1000
    e203:	b7 10 00    	staa	0x1000
    e206:	88 80       	eora	#0x80
    e208:	b7 10 00    	staa	0x1000
    e20b:	88 08       	eora	#0x8
    e20d:	b7 10 00    	staa	0x1000
    e210:	86 10       	ldaa	#0x10
    e212:	b7 10 23    	staa	0x1023
    e215:	37          	pshb
    e216:	fc 10 0e    	ldd	0x100e
    e219:	c3 00 f0    	addd	#0xf0
    e21c:	fd 10 1c    	std	0x101c
    e21f:	33          	pulb
    e220:	f7 01 1c    	stab	0x11c
    e223:	c1 10       	cmpb	#0x10
    e225:	27 02       	beq	0x0xe229
    e227:	20 a4       	bra	0x0xe1cd
    e229:	18 ce 10 23 	ldy	#0x1023
    e22d:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xe22d
    e231:	fb 
    e232:	86 8f       	ldaa	#0x8f
    e234:	b7 10 47    	staa	0x1047
    e237:	86 80       	ldaa	#0x80
    e239:	ba 10 00    	oraa	0x1000
    e23c:	b7 10 00    	staa	0x1000
    e23f:	88 80       	eora	#0x80
    e241:	b7 10 00    	staa	0x1000
    e244:	86 10       	ldaa	#0x10
    e246:	b7 10 23    	staa	0x1023
    e249:	fc 10 0e    	ldd	0x100e
    e24c:	c3 00 f0    	addd	#0xf0
    e24f:	fd 10 1c    	std	0x101c
    e252:	18 1f 00 10 	brclr	0x0,y, #0x10, 0x0xe252
    e256:	fb 
    e257:	7e e1 cd    	jmp	0xe1cd
    e25a:	53          	comb
    e25b:	54          	lsrb
    e25c:	55          	.byte	0x55
    e25d:	44          	lsra
    e25e:	49          	rola
    e25f:	4f          	clra
    e260:	20 20       	bra	0x0xe282
    e262:	20 4f       	bra	0x0xe2b3
    e264:	4d          	tsta
    e265:	45          	.byte	0x45
    e266:	47          	asra
    e267:	41          	.byte	0x41
    e268:	20 32       	bra	0x0xe29c
    e26a:	45          	.byte	0x45
    e26b:	4c          	inca
    e26c:	45          	.byte	0x45
    e26d:	43          	coma
    e26e:	54          	lsrb
    e26f:	52          	.byte	0x52
    e270:	4f          	clra
    e271:	4e          	.byte	0x4e
    e272:	49          	rola
    e273:	43          	coma
    e274:	53          	comb
    e275:	20 31       	bra	0x0xe2a8
    e277:	2e 37       	bgt	0x0xe2b0
    e279:	31          	ins
    e27a:	00          	bgnd
    e27b:	01          	nop
    e27c:	01          	nop
    e27d:	01          	nop
    e27e:	01          	nop
    e27f:	02          	idiv
    e280:	02          	idiv
    e281:	02          	idiv
    e282:	02          	idiv
    e283:	03          	fdiv
    e284:	03          	fdiv
    e285:	04          	lsrd
    e286:	04          	lsrd
    e287:	05          	asld
    e288:	05          	asld
    e289:	05          	asld
    e28a:	06          	tap
    e28b:	06          	tap
    e28c:	06          	tap
    e28d:	07          	tpa
    e28e:	07          	tpa
    e28f:	07          	tpa
    e290:	08          	inx
    e291:	08          	inx
    e292:	08          	inx
    e293:	09          	dex
    e294:	09          	dex
    e295:	0a          	clv
    e296:	0a          	clv
    e297:	0b          	sev
    e298:	0b          	sev
    e299:	0c          	clc
    e29a:	0c          	clc
    e29b:	0d          	sec
    e29c:	0d          	sec
    e29d:	0e          	cli
    e29e:	0e          	cli
    e29f:	0f          	sei
    e2a0:	10          	sba
    e2a1:	11          	cba
    e2a2:	11          	cba
    e2a3:	12 12 13 13 	brset	*0x12, #0x13, 0x0xe2ba
    e2a7:	14 14 15    	bset	*0x14, #0x15
    e2aa:	15 16 17    	bclr	*0x16, #0x17
    e2ad:	18 18       	.byte	0x18, 0x18
    e2af:	19          	daa
    e2b0:	19          	daa
    e2b1:	1a 1b       	.byte	0x1a, 0x1b
    e2b3:	1c 1d 1e    	bset	0x1d,x, #0x1e
    e2b6:	1f 20 21 22 	brclr	0x20,x, #0x21, 0x0xe2dc
    e2ba:	23 24       	bls	0x0xe2e0
    e2bc:	25 26       	bcs	0x0xe2e4
    e2be:	27 28       	beq	0x0xe2e8
    e2c0:	29 2a       	bvs	0x0xe2ec
    e2c2:	2b 2c       	bmi	0x0xe2f0
    e2c4:	2d 2e       	blt	0x0xe2f4
    e2c6:	30          	tsx
    e2c7:	31          	ins
    e2c8:	32          	pula
    e2c9:	33          	pulb
    e2ca:	34          	des
    e2cb:	35          	txs
    e2cc:	36          	psha
    e2cd:	37          	pshb
    e2ce:	38          	pulx
    e2cf:	39          	rts
    e2d0:	3a          	abx
    e2d1:	3b          	rti
    e2d2:	3c          	pshx
    e2d3:	3d          	mul
    e2d4:	3f          	swi
    e2d5:	41          	.byte	0x41
    e2d6:	42          	.byte	0x42
    e2d7:	43          	coma
    e2d8:	44          	lsra
    e2d9:	45          	.byte	0x45
    e2da:	47          	asra
    e2db:	49          	rola
    e2dc:	4a          	deca
    e2dd:	4c          	inca
    e2de:	4e          	.byte	0x4e
    e2df:	50          	negb
    e2e0:	52          	.byte	0x52
    e2e1:	54          	lsrb
    e2e2:	56          	rorb
    e2e3:	58          	aslb
    e2e4:	5a          	decb
    e2e5:	5c          	incb
    e2e6:	5e          	.byte	0x5e
    e2e7:	60 61       	neg	0x61,x
    e2e9:	63 64       	com	0x64,x
    e2eb:	66 68       	ror	0x68,x
    e2ed:	6a 6c       	dec	0x6c,x
    e2ef:	6e 70       	jmp	0x70,x
    e2f1:	72          	.byte	0x72
    e2f2:	74 76 78    	lsr	0x7678
    e2f5:	7a 7c 7d    	dec	0x7c7d
    e2f8:	7e 7f 00    	jmp	0x7f00
    e2fb:	00          	bgnd
    e2fc:	40          	nega
    e2fd:	00          	bgnd
    e2fe:	00          	bgnd
    e2ff:	00          	bgnd
    e300:	00          	bgnd
    e301:	00          	bgnd
    e302:	5d          	tstb
    e303:	00          	bgnd
    e304:	00          	bgnd
    e305:	00          	bgnd
    e306:	00          	bgnd
    e307:	03          	fdiv
    e308:	00          	bgnd
    e309:	00          	bgnd
    e30a:	00          	bgnd
    e30b:	00          	bgnd
    e30c:	00          	bgnd
    e30d:	00          	bgnd
    e30e:	01          	nop
    e30f:	53          	comb
    e310:	00          	bgnd
    e311:	00          	bgnd
    e312:	00          	bgnd
    e313:	00          	bgnd
    e314:	10          	sba
    e315:	00          	bgnd
    e316:	00          	bgnd
    e317:	00          	bgnd
    e318:	00          	bgnd
    e319:	00          	bgnd
    e31a:	40          	nega
    e31b:	00          	bgnd
    e31c:	2c 3d       	bge	0x0xe35b
    e31e:	22 7f       	bhi	0x0xe39f
    e320:	7f 00 00    	clr	0x0
    e323:	00          	bgnd
    e324:	01          	nop
    e325:	00          	bgnd
    e326:	00          	bgnd
    e327:	00          	bgnd
    e328:	00          	bgnd
    e329:	7f 00 00    	clr	0x0
    e32c:	3d          	mul
    e32d:	57          	asrb
    e32e:	00          	bgnd
    e32f:	40          	nega
    e330:	00          	bgnd
    e331:	2d 3a       	blt	0x0xe36d
    e333:	00          	bgnd
    e334:	00          	bgnd
    e335:	00          	bgnd
    e336:	5a          	decb
    e337:	40          	nega
    e338:	00          	bgnd
    e339:	00          	bgnd
    e33a:	02          	idiv
    e33b:	00          	bgnd
    e33c:	00          	bgnd
    e33d:	00          	bgnd
    e33e:	00          	bgnd
    e33f:	00          	bgnd
    e340:	00          	bgnd
    e341:	00          	bgnd
    e342:	07          	tpa
    e343:	00          	bgnd
    e344:	21 01       	brn	0x0xe347
    e346:	40          	nega
    e347:	40          	nega
    e348:	28 4b       	bvc	0x0xe395
    e34a:	20 0a       	bra	0x0xe356
    e34c:	00          	bgnd
    e34d:	01          	nop
    e34e:	02          	idiv
    e34f:	00          	bgnd
    e350:	12 00 19 00 	brset	*0x0, #0x19, 0x0xe354
    e354:	10          	sba
    e355:	00          	bgnd
    e356:	00          	bgnd
    e357:	00          	bgnd
    e358:	03          	fdiv
    e359:	09          	dex
    e35a:	02          	idiv
    e35b:	00          	bgnd
    e35c:	11          	cba
	...
    e365:	00          	bgnd
    e366:	40          	nega
    e367:	40          	nega
    e368:	00          	bgnd
    e369:	00          	bgnd
    e36a:	02          	idiv
    e36b:	02          	idiv
    e36c:	02          	idiv
	...
    e381:	00          	bgnd
    e382:	07          	tpa
    e383:	07          	tpa
    e384:	07          	tpa
    e385:	07          	tpa
    e386:	07          	tpa
    e387:	07          	tpa
    e388:	07          	tpa
    e389:	07          	tpa
    e38a:	40          	nega
    e38b:	40          	nega
    e38c:	40          	nega
    e38d:	40          	nega
    e38e:	40          	nega
    e38f:	40          	nega
    e390:	40          	nega
    e391:	40          	nega
	...
    e39a:	49          	rola
    e39b:	4e          	.byte	0x4e
    e39c:	49          	rola
    e39d:	54          	lsrb
    e39e:	49          	rola
    e39f:	41          	.byte	0x41
    e3a0:	4c          	inca
    e3a1:	20 20       	bra	0x0xe3c3
    e3a3:	20 20       	bra	0x0xe3c5
    e3a5:	20 20       	bra	0x0xe3c7
    e3a7:	20 20       	bra	0x0xe3c9
    e3a9:	20 00       	bra	0x0xe3ab
    e3ab:	02          	idiv
    e3ac:	03          	fdiv
    e3ad:	05          	asld
    e3ae:	0f          	sei
    e3af:	0a          	clv
    e3b0:	1e 14 3c 28 	brset	0x14,x, #0x3c, 0x0xe3dc
    e3b4:	78 50 f0    	asl	0x50f0
    e3b7:	ff ff ff    	stx	0xffff
    e3ba:	ff ff ff    	stx	0xffff
    e3bd:	ff ff ff    	stx	0xffff
    e3c0:	ff ff ff    	stx	0xffff
    e3c3:	ff ff ff    	stx	0xffff
    e3c6:	ff ff ff    	stx	0xffff
    e3c9:	ff ff ff    	stx	0xffff
    e3cc:	ff ff ff    	stx	0xffff
    e3cf:	ff ff ff    	stx	0xffff
    e3d2:	ff ff ff    	stx	0xffff
    e3d5:	ff ff ff    	stx	0xffff
    e3d8:	ff ff ff    	stx	0xffff
    e3db:	ff ff ff    	stx	0xffff
    e3de:	ff ff ff    	stx	0xffff
    e3e1:	ff ff ff    	stx	0xffff
    e3e4:	ff ff ff    	stx	0xffff
    e3e7:	ff ff ff    	stx	0xffff
    e3ea:	ff ff ff    	stx	0xffff
    e3ed:	ff ff ff    	stx	0xffff
    e3f0:	ff ff ff    	stx	0xffff
    e3f3:	ff ff ff    	stx	0xffff
    e3f6:	ff ff ff    	stx	0xffff
    e3f9:	ff ff ff    	stx	0xffff
    e3fc:	ff ff ff    	stx	0xffff
    e3ff:	ff ff ff    	stx	0xffff
    e402:	ff ff ff    	stx	0xffff
    e405:	ff ff ff    	stx	0xffff
    e408:	ff ff ff    	stx	0xffff
    e40b:	ff ff ff    	stx	0xffff
    e40e:	ff ff ff    	stx	0xffff
    e411:	ff ff ff    	stx	0xffff
    e414:	ff ff ff    	stx	0xffff
    e417:	ff ff ff    	stx	0xffff
    e41a:	ff ff ff    	stx	0xffff
    e41d:	ff ff ff    	stx	0xffff
    e420:	ff ff ff    	stx	0xffff
    e423:	ff ff ff    	stx	0xffff
    e426:	ff ff ff    	stx	0xffff
    e429:	ff ff ff    	stx	0xffff
    e42c:	ff ff ff    	stx	0xffff
    e42f:	ff ff ff    	stx	0xffff
    e432:	ff ff ff    	stx	0xffff
    e435:	ff ff ff    	stx	0xffff
    e438:	ff ff ff    	stx	0xffff
    e43b:	ff ff ff    	stx	0xffff
    e43e:	ff ff ff    	stx	0xffff
    e441:	ff ff ff    	stx	0xffff
    e444:	ff ff ff    	stx	0xffff
    e447:	ff ff ff    	stx	0xffff
    e44a:	ff ff ff    	stx	0xffff
    e44d:	ff ff ff    	stx	0xffff
    e450:	ff ff ff    	stx	0xffff
    e453:	ff ff ff    	stx	0xffff
    e456:	ff ff ff    	stx	0xffff
    e459:	ff ff ff    	stx	0xffff
    e45c:	ff ff ff    	stx	0xffff
    e45f:	ff ff ff    	stx	0xffff
    e462:	ff ff ff    	stx	0xffff
    e465:	ff ff ff    	stx	0xffff
    e468:	ff ff ff    	stx	0xffff
    e46b:	ff ff ff    	stx	0xffff
    e46e:	ff ff ff    	stx	0xffff
    e471:	ff ff ff    	stx	0xffff
    e474:	ff ff ff    	stx	0xffff
    e477:	ff ff ff    	stx	0xffff
    e47a:	ff ff ff    	stx	0xffff
    e47d:	ff ff ff    	stx	0xffff
    e480:	ff ff ff    	stx	0xffff
    e483:	ff ff ff    	stx	0xffff
    e486:	ff ff ff    	stx	0xffff
    e489:	ff ff ff    	stx	0xffff
    e48c:	ff ff ff    	stx	0xffff
    e48f:	ff ff ff    	stx	0xffff
    e492:	ff ff ff    	stx	0xffff
    e495:	ff ff ff    	stx	0xffff
    e498:	ff ff ff    	stx	0xffff
    e49b:	ff ff ff    	stx	0xffff
    e49e:	ff ff ff    	stx	0xffff
    e4a1:	ff ff ff    	stx	0xffff
    e4a4:	ff ff ff    	stx	0xffff
    e4a7:	ff ff ff    	stx	0xffff
    e4aa:	ff ff ff    	stx	0xffff
    e4ad:	ff ff ff    	stx	0xffff
    e4b0:	ff ff ff    	stx	0xffff
    e4b3:	ff ff ff    	stx	0xffff
    e4b6:	ff ff ff    	stx	0xffff
    e4b9:	ff ff ff    	stx	0xffff
    e4bc:	ff ff ff    	stx	0xffff
    e4bf:	ff ff ff    	stx	0xffff
    e4c2:	ff ff ff    	stx	0xffff
    e4c5:	ff ff ff    	stx	0xffff
    e4c8:	ff ff ff    	stx	0xffff
    e4cb:	ff ff ff    	stx	0xffff
    e4ce:	ff ff ff    	stx	0xffff
    e4d1:	ff ff ff    	stx	0xffff
    e4d4:	ff ff ff    	stx	0xffff
    e4d7:	ff ff ff    	stx	0xffff
    e4da:	ff ff ff    	stx	0xffff
    e4dd:	ff ff ff    	stx	0xffff
    e4e0:	ff ff ff    	stx	0xffff
    e4e3:	ff ff ff    	stx	0xffff
    e4e6:	ff ff ff    	stx	0xffff
    e4e9:	ff ff ff    	stx	0xffff
    e4ec:	ff ff ff    	stx	0xffff
    e4ef:	ff ff ff    	stx	0xffff
    e4f2:	ff ff ff    	stx	0xffff
    e4f5:	ff ff ff    	stx	0xffff
    e4f8:	ff ff ff    	stx	0xffff
    e4fb:	ff ff ff    	stx	0xffff
    e4fe:	ff ff ff    	stx	0xffff
    e501:	ff ff ff    	stx	0xffff
    e504:	ff ff ff    	stx	0xffff
    e507:	ff ff ff    	stx	0xffff
    e50a:	ff ff ff    	stx	0xffff
    e50d:	ff ff ff    	stx	0xffff
    e510:	ff ff ff    	stx	0xffff
    e513:	ff ff ff    	stx	0xffff
    e516:	ff ff ff    	stx	0xffff
    e519:	ff ff ff    	stx	0xffff
    e51c:	ff ff ff    	stx	0xffff
    e51f:	ff ff ff    	stx	0xffff
    e522:	ff ff ff    	stx	0xffff
    e525:	ff ff ff    	stx	0xffff
    e528:	ff ff ff    	stx	0xffff
    e52b:	ff ff ff    	stx	0xffff
    e52e:	ff ff ff    	stx	0xffff
    e531:	ff ff ff    	stx	0xffff
    e534:	ff ff ff    	stx	0xffff
    e537:	ff ff ff    	stx	0xffff
    e53a:	ff ff ff    	stx	0xffff
    e53d:	ff ff ff    	stx	0xffff
    e540:	ff ff ff    	stx	0xffff
    e543:	ff ff ff    	stx	0xffff
    e546:	ff ff ff    	stx	0xffff
    e549:	ff ff ff    	stx	0xffff
    e54c:	ff ff ff    	stx	0xffff
    e54f:	ff ff ff    	stx	0xffff
    e552:	ff ff ff    	stx	0xffff
    e555:	ff ff ff    	stx	0xffff
    e558:	ff ff ff    	stx	0xffff
    e55b:	ff ff ff    	stx	0xffff
    e55e:	ff ff ff    	stx	0xffff
    e561:	ff ff ff    	stx	0xffff
    e564:	ff ff ff    	stx	0xffff
    e567:	ff ff ff    	stx	0xffff
    e56a:	ff ff ff    	stx	0xffff
    e56d:	ff ff ff    	stx	0xffff
    e570:	ff ff ff    	stx	0xffff
    e573:	ff ff ff    	stx	0xffff
    e576:	ff ff ff    	stx	0xffff
    e579:	ff ff ff    	stx	0xffff
    e57c:	ff ff ff    	stx	0xffff
    e57f:	ff ff ff    	stx	0xffff
    e582:	ff ff ff    	stx	0xffff
    e585:	ff ff ff    	stx	0xffff
    e588:	ff ff ff    	stx	0xffff
    e58b:	ff ff ff    	stx	0xffff
    e58e:	ff ff ff    	stx	0xffff
    e591:	ff ff ff    	stx	0xffff
    e594:	ff ff ff    	stx	0xffff
    e597:	ff ff ff    	stx	0xffff
    e59a:	ff ff ff    	stx	0xffff
    e59d:	ff ff ff    	stx	0xffff
    e5a0:	ff ff ff    	stx	0xffff
    e5a3:	ff ff ff    	stx	0xffff
    e5a6:	ff ff ff    	stx	0xffff
    e5a9:	ff ff ff    	stx	0xffff
    e5ac:	ff ff ff    	stx	0xffff
    e5af:	ff ff ff    	stx	0xffff
    e5b2:	ff ff ff    	stx	0xffff
    e5b5:	ff ff ff    	stx	0xffff
    e5b8:	ff ff ff    	stx	0xffff
    e5bb:	ff ff ff    	stx	0xffff
    e5be:	ff ff ff    	stx	0xffff
    e5c1:	ff ff ff    	stx	0xffff
    e5c4:	ff ff ff    	stx	0xffff
    e5c7:	ff ff ff    	stx	0xffff
    e5ca:	ff ff ff    	stx	0xffff
    e5cd:	ff ff ff    	stx	0xffff
    e5d0:	ff ff ff    	stx	0xffff
    e5d3:	ff ff ff    	stx	0xffff
    e5d6:	ff ff ff    	stx	0xffff
    e5d9:	ff ff ff    	stx	0xffff
    e5dc:	ff ff ff    	stx	0xffff
    e5df:	ff ff ff    	stx	0xffff
    e5e2:	ff ff ff    	stx	0xffff
    e5e5:	ff ff ff    	stx	0xffff
    e5e8:	ff ff ff    	stx	0xffff
    e5eb:	ff ff ff    	stx	0xffff
    e5ee:	ff ff ff    	stx	0xffff
    e5f1:	ff ff ff    	stx	0xffff
    e5f4:	ff ff ff    	stx	0xffff
    e5f7:	ff ff ff    	stx	0xffff
    e5fa:	ff ff ff    	stx	0xffff
    e5fd:	ff ff ff    	stx	0xffff
    e600:	ff ff ff    	stx	0xffff
    e603:	ff ff ff    	stx	0xffff
    e606:	ff ff ff    	stx	0xffff
    e609:	ff ff ff    	stx	0xffff
    e60c:	ff ff ff    	stx	0xffff
    e60f:	ff ff ff    	stx	0xffff
    e612:	ff ff ff    	stx	0xffff
    e615:	ff ff ff    	stx	0xffff
    e618:	ff ff ff    	stx	0xffff
    e61b:	ff ff ff    	stx	0xffff
    e61e:	ff ff ff    	stx	0xffff
    e621:	ff ff ff    	stx	0xffff
    e624:	ff ff ff    	stx	0xffff
    e627:	ff ff ff    	stx	0xffff
    e62a:	ff ff ff    	stx	0xffff
    e62d:	ff ff ff    	stx	0xffff
    e630:	ff ff ff    	stx	0xffff
    e633:	ff ff ff    	stx	0xffff
    e636:	ff ff ff    	stx	0xffff
    e639:	ff ff ff    	stx	0xffff
    e63c:	ff ff ff    	stx	0xffff
    e63f:	ff ff ff    	stx	0xffff
    e642:	ff ff ff    	stx	0xffff
    e645:	ff ff ff    	stx	0xffff
    e648:	ff ff ff    	stx	0xffff
    e64b:	ff ff ff    	stx	0xffff
    e64e:	ff ff ff    	stx	0xffff
    e651:	ff ff ff    	stx	0xffff
    e654:	ff ff ff    	stx	0xffff
    e657:	ff ff ff    	stx	0xffff
    e65a:	ff ff ff    	stx	0xffff
    e65d:	ff ff ff    	stx	0xffff
    e660:	ff ff ff    	stx	0xffff
    e663:	ff ff ff    	stx	0xffff
    e666:	ff ff ff    	stx	0xffff
    e669:	ff ff ff    	stx	0xffff
    e66c:	ff ff ff    	stx	0xffff
    e66f:	ff ff ff    	stx	0xffff
    e672:	ff ff ff    	stx	0xffff
    e675:	ff ff ff    	stx	0xffff
    e678:	ff ff ff    	stx	0xffff
    e67b:	ff ff ff    	stx	0xffff
    e67e:	ff ff ff    	stx	0xffff
    e681:	ff ff ff    	stx	0xffff
    e684:	ff ff ff    	stx	0xffff
    e687:	ff ff ff    	stx	0xffff
    e68a:	ff ff ff    	stx	0xffff
    e68d:	ff ff ff    	stx	0xffff
    e690:	ff ff ff    	stx	0xffff
    e693:	ff ff ff    	stx	0xffff
    e696:	ff ff ff    	stx	0xffff
    e699:	ff ff ff    	stx	0xffff
    e69c:	ff ff ff    	stx	0xffff
    e69f:	ff ff ff    	stx	0xffff
    e6a2:	ff ff ff    	stx	0xffff
    e6a5:	ff ff ff    	stx	0xffff
    e6a8:	ff ff ff    	stx	0xffff
    e6ab:	ff ff ff    	stx	0xffff
    e6ae:	ff ff ff    	stx	0xffff
    e6b1:	ff ff ff    	stx	0xffff
    e6b4:	ff ff ff    	stx	0xffff
    e6b7:	ff ff ff    	stx	0xffff
    e6ba:	ff ff ff    	stx	0xffff
    e6bd:	ff ff ff    	stx	0xffff
    e6c0:	ff ff ff    	stx	0xffff
    e6c3:	ff ff ff    	stx	0xffff
    e6c6:	ff ff ff    	stx	0xffff
    e6c9:	ff ff ff    	stx	0xffff
    e6cc:	ff ff ff    	stx	0xffff
    e6cf:	ff ff ff    	stx	0xffff
    e6d2:	ff ff ff    	stx	0xffff
    e6d5:	ff ff ff    	stx	0xffff
    e6d8:	ff ff ff    	stx	0xffff
    e6db:	ff ff ff    	stx	0xffff
    e6de:	ff ff ff    	stx	0xffff
    e6e1:	ff ff ff    	stx	0xffff
    e6e4:	ff ff ff    	stx	0xffff
    e6e7:	ff ff ff    	stx	0xffff
    e6ea:	ff ff ff    	stx	0xffff
    e6ed:	ff ff ff    	stx	0xffff
    e6f0:	ff ff ff    	stx	0xffff
    e6f3:	ff ff ff    	stx	0xffff
    e6f6:	ff ff ff    	stx	0xffff
    e6f9:	ff ff ff    	stx	0xffff
    e6fc:	ff ff ff    	stx	0xffff
    e6ff:	ff ff ff    	stx	0xffff
    e702:	ff ff ff    	stx	0xffff
    e705:	ff ff ff    	stx	0xffff
    e708:	ff ff ff    	stx	0xffff
    e70b:	ff ff ff    	stx	0xffff
    e70e:	ff ff ff    	stx	0xffff
    e711:	ff ff ff    	stx	0xffff
    e714:	ff ff ff    	stx	0xffff
    e717:	ff ff ff    	stx	0xffff
    e71a:	ff ff ff    	stx	0xffff
    e71d:	ff ff ff    	stx	0xffff
    e720:	ff ff ff    	stx	0xffff
    e723:	ff ff ff    	stx	0xffff
    e726:	ff ff ff    	stx	0xffff
    e729:	ff ff ff    	stx	0xffff
    e72c:	ff ff ff    	stx	0xffff
    e72f:	ff ff ff    	stx	0xffff
    e732:	ff ff ff    	stx	0xffff
    e735:	ff ff ff    	stx	0xffff
    e738:	ff ff ff    	stx	0xffff
    e73b:	ff ff ff    	stx	0xffff
    e73e:	ff ff ff    	stx	0xffff
    e741:	ff ff ff    	stx	0xffff
    e744:	ff ff ff    	stx	0xffff
    e747:	ff ff ff    	stx	0xffff
    e74a:	ff ff ff    	stx	0xffff
    e74d:	ff ff ff    	stx	0xffff
    e750:	ff ff ff    	stx	0xffff
    e753:	ff ff ff    	stx	0xffff
    e756:	ff ff ff    	stx	0xffff
    e759:	ff ff ff    	stx	0xffff
    e75c:	ff ff ff    	stx	0xffff
    e75f:	ff ff ff    	stx	0xffff
    e762:	ff ff ff    	stx	0xffff
    e765:	ff ff ff    	stx	0xffff
    e768:	ff ff ff    	stx	0xffff
    e76b:	ff ff ff    	stx	0xffff
    e76e:	ff ff ff    	stx	0xffff
    e771:	ff ff ff    	stx	0xffff
    e774:	ff ff ff    	stx	0xffff
    e777:	ff ff ff    	stx	0xffff
    e77a:	ff ff ff    	stx	0xffff
    e77d:	ff ff ff    	stx	0xffff
    e780:	ff ff ff    	stx	0xffff
    e783:	ff ff ff    	stx	0xffff
    e786:	ff ff ff    	stx	0xffff
    e789:	ff ff ff    	stx	0xffff
    e78c:	ff ff ff    	stx	0xffff
    e78f:	ff ff ff    	stx	0xffff
    e792:	ff ff ff    	stx	0xffff
    e795:	ff ff ff    	stx	0xffff
    e798:	ff ff ff    	stx	0xffff
    e79b:	ff ff ff    	stx	0xffff
    e79e:	ff ff ff    	stx	0xffff
    e7a1:	ff ff ff    	stx	0xffff
    e7a4:	ff ff ff    	stx	0xffff
    e7a7:	ff ff ff    	stx	0xffff
    e7aa:	ff ff ff    	stx	0xffff
    e7ad:	ff ff ff    	stx	0xffff
    e7b0:	ff ff ff    	stx	0xffff
    e7b3:	ff ff ff    	stx	0xffff
    e7b6:	ff ff ff    	stx	0xffff
    e7b9:	ff ff ff    	stx	0xffff
    e7bc:	ff ff ff    	stx	0xffff
    e7bf:	ff ff ff    	stx	0xffff
    e7c2:	ff ff ff    	stx	0xffff
    e7c5:	ff ff ff    	stx	0xffff
    e7c8:	ff ff ff    	stx	0xffff
    e7cb:	ff ff ff    	stx	0xffff
    e7ce:	ff ff ff    	stx	0xffff
    e7d1:	ff ff ff    	stx	0xffff
    e7d4:	ff ff ff    	stx	0xffff
    e7d7:	ff ff ff    	stx	0xffff
    e7da:	ff ff ff    	stx	0xffff
    e7dd:	ff ff ff    	stx	0xffff
    e7e0:	ff ff ff    	stx	0xffff
    e7e3:	ff ff ff    	stx	0xffff
    e7e6:	ff ff ff    	stx	0xffff
    e7e9:	ff ff ff    	stx	0xffff
    e7ec:	ff ff ff    	stx	0xffff
    e7ef:	ff ff ff    	stx	0xffff
    e7f2:	ff ff ff    	stx	0xffff
    e7f5:	ff ff ff    	stx	0xffff
    e7f8:	ff ff ff    	stx	0xffff
    e7fb:	ff ff ff    	stx	0xffff
    e7fe:	ff ff ff    	stx	0xffff
    e801:	ff ff ff    	stx	0xffff
    e804:	ff ff ff    	stx	0xffff
    e807:	ff ff ff    	stx	0xffff
    e80a:	ff ff ff    	stx	0xffff
    e80d:	ff ff ff    	stx	0xffff
    e810:	ff ff ff    	stx	0xffff
    e813:	ff ff ff    	stx	0xffff
    e816:	ff ff ff    	stx	0xffff
    e819:	ff ff ff    	stx	0xffff
    e81c:	ff ff ff    	stx	0xffff
    e81f:	ff ff ff    	stx	0xffff
    e822:	ff ff ff    	stx	0xffff
    e825:	ff ff ff    	stx	0xffff
    e828:	ff ff ff    	stx	0xffff
    e82b:	ff ff ff    	stx	0xffff
    e82e:	ff ff ff    	stx	0xffff
    e831:	ff ff ff    	stx	0xffff
    e834:	ff ff ff    	stx	0xffff
    e837:	ff ff ff    	stx	0xffff
    e83a:	ff ff ff    	stx	0xffff
    e83d:	ff ff ff    	stx	0xffff
    e840:	ff ff ff    	stx	0xffff
    e843:	ff ff ff    	stx	0xffff
    e846:	ff ff ff    	stx	0xffff
    e849:	ff ff ff    	stx	0xffff
    e84c:	ff ff ff    	stx	0xffff
    e84f:	ff ff ff    	stx	0xffff
    e852:	ff ff ff    	stx	0xffff
    e855:	ff ff ff    	stx	0xffff
    e858:	ff ff ff    	stx	0xffff
    e85b:	ff ff ff    	stx	0xffff
    e85e:	ff ff ff    	stx	0xffff
    e861:	ff ff ff    	stx	0xffff
    e864:	ff ff ff    	stx	0xffff
    e867:	ff ff ff    	stx	0xffff
    e86a:	ff ff ff    	stx	0xffff
    e86d:	ff ff ff    	stx	0xffff
    e870:	ff ff ff    	stx	0xffff
    e873:	ff ff ff    	stx	0xffff
    e876:	ff ff ff    	stx	0xffff
    e879:	ff ff ff    	stx	0xffff
    e87c:	ff ff ff    	stx	0xffff
    e87f:	ff ff ff    	stx	0xffff
    e882:	ff ff ff    	stx	0xffff
    e885:	ff ff ff    	stx	0xffff
    e888:	ff ff ff    	stx	0xffff
    e88b:	ff ff ff    	stx	0xffff
    e88e:	ff ff ff    	stx	0xffff
    e891:	ff ff ff    	stx	0xffff
    e894:	ff ff ff    	stx	0xffff
    e897:	ff ff ff    	stx	0xffff
    e89a:	ff ff ff    	stx	0xffff
    e89d:	ff ff ff    	stx	0xffff
    e8a0:	ff ff ff    	stx	0xffff
    e8a3:	ff ff ff    	stx	0xffff
    e8a6:	ff ff ff    	stx	0xffff
    e8a9:	ff ff ff    	stx	0xffff
    e8ac:	ff ff ff    	stx	0xffff
    e8af:	ff ff ff    	stx	0xffff
    e8b2:	ff ff ff    	stx	0xffff
    e8b5:	ff ff ff    	stx	0xffff
    e8b8:	ff ff ff    	stx	0xffff
    e8bb:	ff ff ff    	stx	0xffff
    e8be:	ff ff ff    	stx	0xffff
    e8c1:	ff ff ff    	stx	0xffff
    e8c4:	ff ff ff    	stx	0xffff
    e8c7:	ff ff ff    	stx	0xffff
    e8ca:	ff ff ff    	stx	0xffff
    e8cd:	ff ff ff    	stx	0xffff
    e8d0:	ff ff ff    	stx	0xffff
    e8d3:	ff ff ff    	stx	0xffff
    e8d6:	ff ff ff    	stx	0xffff
    e8d9:	ff ff ff    	stx	0xffff
    e8dc:	ff ff ff    	stx	0xffff
    e8df:	ff ff ff    	stx	0xffff
    e8e2:	ff ff ff    	stx	0xffff
    e8e5:	ff ff ff    	stx	0xffff
    e8e8:	ff ff ff    	stx	0xffff
    e8eb:	ff ff ff    	stx	0xffff
    e8ee:	ff ff ff    	stx	0xffff
    e8f1:	ff ff ff    	stx	0xffff
    e8f4:	ff ff ff    	stx	0xffff
    e8f7:	ff ff ff    	stx	0xffff
    e8fa:	ff ff ff    	stx	0xffff
    e8fd:	ff ff ff    	stx	0xffff
    e900:	ff ff ff    	stx	0xffff
    e903:	ff ff ff    	stx	0xffff
    e906:	ff ff ff    	stx	0xffff
    e909:	ff ff ff    	stx	0xffff
    e90c:	ff ff ff    	stx	0xffff
    e90f:	ff ff ff    	stx	0xffff
    e912:	ff ff ff    	stx	0xffff
    e915:	ff ff ff    	stx	0xffff
    e918:	ff ff ff    	stx	0xffff
    e91b:	ff ff ff    	stx	0xffff
    e91e:	ff ff ff    	stx	0xffff
    e921:	ff ff ff    	stx	0xffff
    e924:	ff ff ff    	stx	0xffff
    e927:	ff ff ff    	stx	0xffff
    e92a:	ff ff ff    	stx	0xffff
    e92d:	ff ff ff    	stx	0xffff
    e930:	ff ff ff    	stx	0xffff
    e933:	ff ff ff    	stx	0xffff
    e936:	ff ff ff    	stx	0xffff
    e939:	ff ff ff    	stx	0xffff
    e93c:	ff ff ff    	stx	0xffff
    e93f:	ff ff ff    	stx	0xffff
    e942:	ff ff ff    	stx	0xffff
    e945:	ff ff ff    	stx	0xffff
    e948:	ff ff ff    	stx	0xffff
    e94b:	ff ff ff    	stx	0xffff
    e94e:	ff ff ff    	stx	0xffff
    e951:	ff ff ff    	stx	0xffff
    e954:	ff ff ff    	stx	0xffff
    e957:	ff ff ff    	stx	0xffff
    e95a:	ff ff ff    	stx	0xffff
    e95d:	ff ff ff    	stx	0xffff
    e960:	ff ff ff    	stx	0xffff
    e963:	ff ff ff    	stx	0xffff
    e966:	ff ff ff    	stx	0xffff
    e969:	ff ff ff    	stx	0xffff
    e96c:	ff ff ff    	stx	0xffff
    e96f:	ff ff ff    	stx	0xffff
    e972:	ff ff ff    	stx	0xffff
    e975:	ff ff ff    	stx	0xffff
    e978:	ff ff ff    	stx	0xffff
    e97b:	ff ff ff    	stx	0xffff
    e97e:	ff ff ff    	stx	0xffff
    e981:	ff ff ff    	stx	0xffff
    e984:	ff ff ff    	stx	0xffff
    e987:	ff ff ff    	stx	0xffff
    e98a:	ff ff ff    	stx	0xffff
    e98d:	ff ff ff    	stx	0xffff
    e990:	ff ff ff    	stx	0xffff
    e993:	ff ff ff    	stx	0xffff
    e996:	ff ff ff    	stx	0xffff
    e999:	ff ff ff    	stx	0xffff
    e99c:	ff ff ff    	stx	0xffff
    e99f:	ff ff ff    	stx	0xffff
    e9a2:	ff ff ff    	stx	0xffff
    e9a5:	ff ff ff    	stx	0xffff
    e9a8:	ff ff ff    	stx	0xffff
    e9ab:	ff ff ff    	stx	0xffff
    e9ae:	ff ff ff    	stx	0xffff
    e9b1:	ff ff ff    	stx	0xffff
    e9b4:	ff ff ff    	stx	0xffff
    e9b7:	ff ff ff    	stx	0xffff
    e9ba:	ff ff ff    	stx	0xffff
    e9bd:	ff ff ff    	stx	0xffff
    e9c0:	ff ff ff    	stx	0xffff
    e9c3:	ff ff ff    	stx	0xffff
    e9c6:	ff ff ff    	stx	0xffff
    e9c9:	ff ff ff    	stx	0xffff
    e9cc:	ff ff ff    	stx	0xffff
    e9cf:	ff ff ff    	stx	0xffff
    e9d2:	ff ff ff    	stx	0xffff
    e9d5:	ff ff ff    	stx	0xffff
    e9d8:	ff ff ff    	stx	0xffff
    e9db:	ff ff ff    	stx	0xffff
    e9de:	ff ff ff    	stx	0xffff
    e9e1:	ff ff ff    	stx	0xffff
    e9e4:	ff ff ff    	stx	0xffff
    e9e7:	ff ff ff    	stx	0xffff
    e9ea:	ff ff ff    	stx	0xffff
    e9ed:	ff ff ff    	stx	0xffff
    e9f0:	ff ff ff    	stx	0xffff
    e9f3:	ff ff ff    	stx	0xffff
    e9f6:	ff ff ff    	stx	0xffff
    e9f9:	ff ff ff    	stx	0xffff
    e9fc:	ff ff ff    	stx	0xffff
    e9ff:	ff ff ff    	stx	0xffff
    ea02:	ff ff ff    	stx	0xffff
    ea05:	ff ff ff    	stx	0xffff
    ea08:	ff ff ff    	stx	0xffff
    ea0b:	ff ff ff    	stx	0xffff
    ea0e:	ff ff ff    	stx	0xffff
    ea11:	ff ff ff    	stx	0xffff
    ea14:	ff ff ff    	stx	0xffff
    ea17:	ff ff ff    	stx	0xffff
    ea1a:	ff ff ff    	stx	0xffff
    ea1d:	ff ff ff    	stx	0xffff
    ea20:	ff ff ff    	stx	0xffff
    ea23:	ff ff ff    	stx	0xffff
    ea26:	ff ff ff    	stx	0xffff
    ea29:	ff ff ff    	stx	0xffff
    ea2c:	ff ff ff    	stx	0xffff
    ea2f:	ff ff ff    	stx	0xffff
    ea32:	ff ff ff    	stx	0xffff
    ea35:	ff ff ff    	stx	0xffff
    ea38:	ff ff ff    	stx	0xffff
    ea3b:	ff ff ff    	stx	0xffff
    ea3e:	ff ff ff    	stx	0xffff
    ea41:	ff ff ff    	stx	0xffff
    ea44:	ff ff ff    	stx	0xffff
    ea47:	ff ff ff    	stx	0xffff
    ea4a:	ff ff ff    	stx	0xffff
    ea4d:	ff ff ff    	stx	0xffff
    ea50:	ff ff ff    	stx	0xffff
    ea53:	ff ff ff    	stx	0xffff
    ea56:	ff ff ff    	stx	0xffff
    ea59:	ff ff ff    	stx	0xffff
    ea5c:	ff ff ff    	stx	0xffff
    ea5f:	ff ff ff    	stx	0xffff
    ea62:	ff ff ff    	stx	0xffff
    ea65:	ff ff ff    	stx	0xffff
    ea68:	ff ff ff    	stx	0xffff
    ea6b:	ff ff ff    	stx	0xffff
    ea6e:	ff ff ff    	stx	0xffff
    ea71:	ff ff ff    	stx	0xffff
    ea74:	ff ff ff    	stx	0xffff
    ea77:	ff ff ff    	stx	0xffff
    ea7a:	ff ff ff    	stx	0xffff
    ea7d:	ff ff ff    	stx	0xffff
    ea80:	ff ff ff    	stx	0xffff
    ea83:	ff ff ff    	stx	0xffff
    ea86:	ff ff ff    	stx	0xffff
    ea89:	ff ff ff    	stx	0xffff
    ea8c:	ff ff ff    	stx	0xffff
    ea8f:	ff ff ff    	stx	0xffff
    ea92:	ff ff ff    	stx	0xffff
    ea95:	ff ff ff    	stx	0xffff
    ea98:	ff ff ff    	stx	0xffff
    ea9b:	ff ff ff    	stx	0xffff
    ea9e:	ff ff ff    	stx	0xffff
    eaa1:	ff ff ff    	stx	0xffff
    eaa4:	ff ff ff    	stx	0xffff
    eaa7:	ff ff ff    	stx	0xffff
    eaaa:	ff ff ff    	stx	0xffff
    eaad:	ff ff ff    	stx	0xffff
    eab0:	ff ff ff    	stx	0xffff
    eab3:	ff ff ff    	stx	0xffff
    eab6:	ff ff ff    	stx	0xffff
    eab9:	ff ff ff    	stx	0xffff
    eabc:	ff ff ff    	stx	0xffff
    eabf:	ff ff ff    	stx	0xffff
    eac2:	ff ff ff    	stx	0xffff
    eac5:	ff ff ff    	stx	0xffff
    eac8:	ff ff ff    	stx	0xffff
    eacb:	ff ff ff    	stx	0xffff
    eace:	ff ff ff    	stx	0xffff
    ead1:	ff ff ff    	stx	0xffff
    ead4:	ff ff ff    	stx	0xffff
    ead7:	ff ff ff    	stx	0xffff
    eada:	ff ff ff    	stx	0xffff
    eadd:	ff ff ff    	stx	0xffff
    eae0:	ff ff ff    	stx	0xffff
    eae3:	ff ff ff    	stx	0xffff
    eae6:	ff ff ff    	stx	0xffff
    eae9:	ff ff ff    	stx	0xffff
    eaec:	ff ff ff    	stx	0xffff
    eaef:	ff ff ff    	stx	0xffff
    eaf2:	ff ff ff    	stx	0xffff
    eaf5:	ff ff ff    	stx	0xffff
    eaf8:	ff ff ff    	stx	0xffff
    eafb:	ff ff ff    	stx	0xffff
    eafe:	ff ff ff    	stx	0xffff
    eb01:	ff ff ff    	stx	0xffff
    eb04:	ff ff ff    	stx	0xffff
    eb07:	ff ff ff    	stx	0xffff
    eb0a:	ff ff ff    	stx	0xffff
    eb0d:	ff ff ff    	stx	0xffff
    eb10:	ff ff ff    	stx	0xffff
    eb13:	ff ff ff    	stx	0xffff
    eb16:	ff ff ff    	stx	0xffff
    eb19:	ff ff ff    	stx	0xffff
    eb1c:	ff ff ff    	stx	0xffff
    eb1f:	ff ff ff    	stx	0xffff
    eb22:	ff ff ff    	stx	0xffff
    eb25:	ff ff ff    	stx	0xffff
    eb28:	ff ff ff    	stx	0xffff
    eb2b:	ff ff ff    	stx	0xffff
    eb2e:	ff ff ff    	stx	0xffff
    eb31:	ff ff ff    	stx	0xffff
    eb34:	ff ff ff    	stx	0xffff
    eb37:	ff ff ff    	stx	0xffff
    eb3a:	ff ff ff    	stx	0xffff
    eb3d:	ff ff ff    	stx	0xffff
    eb40:	ff ff ff    	stx	0xffff
    eb43:	ff ff ff    	stx	0xffff
    eb46:	ff ff ff    	stx	0xffff
    eb49:	ff ff ff    	stx	0xffff
    eb4c:	ff ff ff    	stx	0xffff
    eb4f:	ff ff ff    	stx	0xffff
    eb52:	ff ff ff    	stx	0xffff
    eb55:	ff ff ff    	stx	0xffff
    eb58:	ff ff ff    	stx	0xffff
    eb5b:	ff ff ff    	stx	0xffff
    eb5e:	ff ff ff    	stx	0xffff
    eb61:	ff ff ff    	stx	0xffff
    eb64:	ff ff ff    	stx	0xffff
    eb67:	ff ff ff    	stx	0xffff
    eb6a:	ff ff ff    	stx	0xffff
    eb6d:	ff ff ff    	stx	0xffff
    eb70:	ff ff ff    	stx	0xffff
    eb73:	ff ff ff    	stx	0xffff
    eb76:	ff ff ff    	stx	0xffff
    eb79:	ff ff ff    	stx	0xffff
    eb7c:	ff ff ff    	stx	0xffff
    eb7f:	ff ff ff    	stx	0xffff
    eb82:	ff ff ff    	stx	0xffff
    eb85:	ff ff ff    	stx	0xffff
    eb88:	ff ff ff    	stx	0xffff
    eb8b:	ff ff ff    	stx	0xffff
    eb8e:	ff ff ff    	stx	0xffff
    eb91:	ff ff ff    	stx	0xffff
    eb94:	ff ff ff    	stx	0xffff
    eb97:	ff ff ff    	stx	0xffff
    eb9a:	ff ff ff    	stx	0xffff
    eb9d:	ff ff ff    	stx	0xffff
    eba0:	ff ff ff    	stx	0xffff
    eba3:	ff ff ff    	stx	0xffff
    eba6:	ff ff ff    	stx	0xffff
    eba9:	ff ff ff    	stx	0xffff
    ebac:	ff ff ff    	stx	0xffff
    ebaf:	ff ff ff    	stx	0xffff
    ebb2:	ff ff ff    	stx	0xffff
    ebb5:	ff ff ff    	stx	0xffff
    ebb8:	ff ff ff    	stx	0xffff
    ebbb:	ff ff ff    	stx	0xffff
    ebbe:	ff ff ff    	stx	0xffff
    ebc1:	ff ff ff    	stx	0xffff
    ebc4:	ff ff ff    	stx	0xffff
    ebc7:	ff ff ff    	stx	0xffff
    ebca:	ff ff ff    	stx	0xffff
    ebcd:	ff ff ff    	stx	0xffff
    ebd0:	ff ff ff    	stx	0xffff
    ebd3:	ff ff ff    	stx	0xffff
    ebd6:	ff ff ff    	stx	0xffff
    ebd9:	ff ff ff    	stx	0xffff
    ebdc:	ff ff ff    	stx	0xffff
    ebdf:	ff ff ff    	stx	0xffff
    ebe2:	ff ff ff    	stx	0xffff
    ebe5:	ff ff ff    	stx	0xffff
    ebe8:	ff ff ff    	stx	0xffff
    ebeb:	ff ff ff    	stx	0xffff
    ebee:	ff ff ff    	stx	0xffff
    ebf1:	ff ff ff    	stx	0xffff
    ebf4:	ff ff ff    	stx	0xffff
    ebf7:	ff ff ff    	stx	0xffff
    ebfa:	ff ff ff    	stx	0xffff
    ebfd:	ff ff ff    	stx	0xffff
    ec00:	ff ff ff    	stx	0xffff
    ec03:	ff ff ff    	stx	0xffff
    ec06:	ff ff ff    	stx	0xffff
    ec09:	ff ff ff    	stx	0xffff
    ec0c:	ff ff ff    	stx	0xffff
    ec0f:	ff ff ff    	stx	0xffff
    ec12:	ff ff ff    	stx	0xffff
    ec15:	ff ff ff    	stx	0xffff
    ec18:	ff ff ff    	stx	0xffff
    ec1b:	ff ff ff    	stx	0xffff
    ec1e:	ff ff ff    	stx	0xffff
    ec21:	ff ff ff    	stx	0xffff
    ec24:	ff ff ff    	stx	0xffff
    ec27:	ff ff ff    	stx	0xffff
    ec2a:	ff ff ff    	stx	0xffff
    ec2d:	ff ff ff    	stx	0xffff
    ec30:	ff ff ff    	stx	0xffff
    ec33:	ff ff ff    	stx	0xffff
    ec36:	ff ff ff    	stx	0xffff
    ec39:	ff ff ff    	stx	0xffff
    ec3c:	ff ff ff    	stx	0xffff
    ec3f:	ff ff ff    	stx	0xffff
    ec42:	ff ff ff    	stx	0xffff
    ec45:	ff ff ff    	stx	0xffff
    ec48:	ff ff ff    	stx	0xffff
    ec4b:	ff ff ff    	stx	0xffff
    ec4e:	ff ff ff    	stx	0xffff
    ec51:	ff ff ff    	stx	0xffff
    ec54:	ff ff ff    	stx	0xffff
    ec57:	ff ff ff    	stx	0xffff
    ec5a:	ff ff ff    	stx	0xffff
    ec5d:	ff ff ff    	stx	0xffff
    ec60:	ff ff ff    	stx	0xffff
    ec63:	ff ff ff    	stx	0xffff
    ec66:	ff ff ff    	stx	0xffff
    ec69:	ff ff ff    	stx	0xffff
    ec6c:	ff ff ff    	stx	0xffff
    ec6f:	ff ff ff    	stx	0xffff
    ec72:	ff ff ff    	stx	0xffff
    ec75:	ff ff ff    	stx	0xffff
    ec78:	ff ff ff    	stx	0xffff
    ec7b:	ff ff ff    	stx	0xffff
    ec7e:	ff ff ff    	stx	0xffff
    ec81:	ff ff ff    	stx	0xffff
    ec84:	ff ff ff    	stx	0xffff
    ec87:	ff ff ff    	stx	0xffff
    ec8a:	ff ff ff    	stx	0xffff
    ec8d:	ff ff ff    	stx	0xffff
    ec90:	ff ff ff    	stx	0xffff
    ec93:	ff ff ff    	stx	0xffff
    ec96:	ff ff ff    	stx	0xffff
    ec99:	ff ff ff    	stx	0xffff
    ec9c:	ff ff ff    	stx	0xffff
    ec9f:	ff ff ff    	stx	0xffff
    eca2:	ff ff ff    	stx	0xffff
    eca5:	ff ff ff    	stx	0xffff
    eca8:	ff ff ff    	stx	0xffff
    ecab:	ff ff ff    	stx	0xffff
    ecae:	ff ff ff    	stx	0xffff
    ecb1:	ff ff ff    	stx	0xffff
    ecb4:	ff ff ff    	stx	0xffff
    ecb7:	ff ff ff    	stx	0xffff
    ecba:	ff ff ff    	stx	0xffff
    ecbd:	ff ff ff    	stx	0xffff
    ecc0:	ff ff ff    	stx	0xffff
    ecc3:	ff ff ff    	stx	0xffff
    ecc6:	ff ff ff    	stx	0xffff
    ecc9:	ff ff ff    	stx	0xffff
    eccc:	ff ff ff    	stx	0xffff
    eccf:	ff ff ff    	stx	0xffff
    ecd2:	ff ff ff    	stx	0xffff
    ecd5:	ff ff ff    	stx	0xffff
    ecd8:	ff ff ff    	stx	0xffff
    ecdb:	ff ff ff    	stx	0xffff
    ecde:	ff ff ff    	stx	0xffff
    ece1:	ff ff ff    	stx	0xffff
    ece4:	ff ff ff    	stx	0xffff
    ece7:	ff ff ff    	stx	0xffff
    ecea:	ff ff ff    	stx	0xffff
    eced:	ff ff ff    	stx	0xffff
    ecf0:	ff ff ff    	stx	0xffff
    ecf3:	ff ff ff    	stx	0xffff
    ecf6:	ff ff ff    	stx	0xffff
    ecf9:	ff ff ff    	stx	0xffff
    ecfc:	ff ff ff    	stx	0xffff
    ecff:	ff ff ff    	stx	0xffff
    ed02:	ff ff ff    	stx	0xffff
    ed05:	ff ff ff    	stx	0xffff
    ed08:	ff ff ff    	stx	0xffff
    ed0b:	ff ff ff    	stx	0xffff
    ed0e:	ff ff ff    	stx	0xffff
    ed11:	ff ff ff    	stx	0xffff
    ed14:	ff ff ff    	stx	0xffff
    ed17:	ff ff ff    	stx	0xffff
    ed1a:	ff ff ff    	stx	0xffff
    ed1d:	ff ff ff    	stx	0xffff
    ed20:	ff ff ff    	stx	0xffff
    ed23:	ff ff ff    	stx	0xffff
    ed26:	ff ff ff    	stx	0xffff
    ed29:	ff ff ff    	stx	0xffff
    ed2c:	ff ff ff    	stx	0xffff
    ed2f:	ff ff ff    	stx	0xffff
    ed32:	ff ff ff    	stx	0xffff
    ed35:	ff ff ff    	stx	0xffff
    ed38:	ff ff ff    	stx	0xffff
    ed3b:	ff ff ff    	stx	0xffff
    ed3e:	ff ff ff    	stx	0xffff
    ed41:	ff ff ff    	stx	0xffff
    ed44:	ff ff ff    	stx	0xffff
    ed47:	ff ff ff    	stx	0xffff
    ed4a:	ff ff ff    	stx	0xffff
    ed4d:	ff ff ff    	stx	0xffff
    ed50:	ff ff ff    	stx	0xffff
    ed53:	ff ff ff    	stx	0xffff
    ed56:	ff ff ff    	stx	0xffff
    ed59:	ff ff ff    	stx	0xffff
    ed5c:	ff ff ff    	stx	0xffff
    ed5f:	ff ff ff    	stx	0xffff
    ed62:	ff ff ff    	stx	0xffff
    ed65:	ff ff ff    	stx	0xffff
    ed68:	ff ff ff    	stx	0xffff
    ed6b:	ff ff ff    	stx	0xffff
    ed6e:	ff ff ff    	stx	0xffff
    ed71:	ff ff ff    	stx	0xffff
    ed74:	ff ff ff    	stx	0xffff
    ed77:	ff ff ff    	stx	0xffff
    ed7a:	ff ff ff    	stx	0xffff
    ed7d:	ff ff ff    	stx	0xffff
    ed80:	ff ff ff    	stx	0xffff
    ed83:	ff ff ff    	stx	0xffff
    ed86:	ff ff ff    	stx	0xffff
    ed89:	ff ff ff    	stx	0xffff
    ed8c:	ff ff ff    	stx	0xffff
    ed8f:	ff ff ff    	stx	0xffff
    ed92:	ff ff ff    	stx	0xffff
    ed95:	ff ff ff    	stx	0xffff
    ed98:	ff ff ff    	stx	0xffff
    ed9b:	ff ff ff    	stx	0xffff
    ed9e:	ff ff ff    	stx	0xffff
    eda1:	ff ff ff    	stx	0xffff
    eda4:	ff ff ff    	stx	0xffff
    eda7:	ff ff ff    	stx	0xffff
    edaa:	ff ff ff    	stx	0xffff
    edad:	ff ff ff    	stx	0xffff
    edb0:	ff ff ff    	stx	0xffff
    edb3:	ff ff ff    	stx	0xffff
    edb6:	ff ff ff    	stx	0xffff
    edb9:	ff ff ff    	stx	0xffff
    edbc:	ff ff ff    	stx	0xffff
    edbf:	ff ff ff    	stx	0xffff
    edc2:	ff ff ff    	stx	0xffff
    edc5:	ff ff ff    	stx	0xffff
    edc8:	ff ff ff    	stx	0xffff
    edcb:	ff ff ff    	stx	0xffff
    edce:	ff ff ff    	stx	0xffff
    edd1:	ff ff ff    	stx	0xffff
    edd4:	ff ff ff    	stx	0xffff
    edd7:	ff ff ff    	stx	0xffff
    edda:	ff ff ff    	stx	0xffff
    eddd:	ff ff ff    	stx	0xffff
    ede0:	ff ff ff    	stx	0xffff
    ede3:	ff ff ff    	stx	0xffff
    ede6:	ff ff ff    	stx	0xffff
    ede9:	ff ff ff    	stx	0xffff
    edec:	ff ff ff    	stx	0xffff
    edef:	ff ff ff    	stx	0xffff
    edf2:	ff ff ff    	stx	0xffff
    edf5:	ff ff ff    	stx	0xffff
    edf8:	ff ff ff    	stx	0xffff
    edfb:	ff ff ff    	stx	0xffff
    edfe:	ff ff ff    	stx	0xffff
    ee01:	ff ff ff    	stx	0xffff
    ee04:	ff ff ff    	stx	0xffff
    ee07:	ff ff ff    	stx	0xffff
    ee0a:	ff ff ff    	stx	0xffff
    ee0d:	ff ff ff    	stx	0xffff
    ee10:	ff ff ff    	stx	0xffff
    ee13:	ff ff ff    	stx	0xffff
    ee16:	ff ff ff    	stx	0xffff
    ee19:	ff ff ff    	stx	0xffff
    ee1c:	ff ff ff    	stx	0xffff
    ee1f:	ff ff ff    	stx	0xffff
    ee22:	ff ff ff    	stx	0xffff
    ee25:	ff ff ff    	stx	0xffff
    ee28:	ff ff ff    	stx	0xffff
    ee2b:	ff ff ff    	stx	0xffff
    ee2e:	ff ff ff    	stx	0xffff
    ee31:	ff ff ff    	stx	0xffff
    ee34:	ff ff ff    	stx	0xffff
    ee37:	ff ff ff    	stx	0xffff
    ee3a:	ff ff ff    	stx	0xffff
    ee3d:	ff ff ff    	stx	0xffff
    ee40:	ff ff ff    	stx	0xffff
    ee43:	ff ff ff    	stx	0xffff
    ee46:	ff ff ff    	stx	0xffff
    ee49:	ff ff ff    	stx	0xffff
    ee4c:	ff ff ff    	stx	0xffff
    ee4f:	ff ff ff    	stx	0xffff
    ee52:	ff ff ff    	stx	0xffff
    ee55:	ff ff ff    	stx	0xffff
    ee58:	ff ff ff    	stx	0xffff
    ee5b:	ff ff ff    	stx	0xffff
    ee5e:	ff ff ff    	stx	0xffff
    ee61:	ff ff ff    	stx	0xffff
    ee64:	ff ff ff    	stx	0xffff
    ee67:	ff ff ff    	stx	0xffff
    ee6a:	ff ff ff    	stx	0xffff
    ee6d:	ff ff ff    	stx	0xffff
    ee70:	ff ff ff    	stx	0xffff
    ee73:	ff ff ff    	stx	0xffff
    ee76:	ff ff ff    	stx	0xffff
    ee79:	ff ff ff    	stx	0xffff
    ee7c:	ff ff ff    	stx	0xffff
    ee7f:	ff ff ff    	stx	0xffff
    ee82:	ff ff ff    	stx	0xffff
    ee85:	ff ff ff    	stx	0xffff
    ee88:	ff ff ff    	stx	0xffff
    ee8b:	ff ff ff    	stx	0xffff
    ee8e:	ff ff ff    	stx	0xffff
    ee91:	ff ff ff    	stx	0xffff
    ee94:	ff ff ff    	stx	0xffff
    ee97:	ff ff ff    	stx	0xffff
    ee9a:	ff ff ff    	stx	0xffff
    ee9d:	ff ff ff    	stx	0xffff
    eea0:	ff ff ff    	stx	0xffff
    eea3:	ff ff ff    	stx	0xffff
    eea6:	ff ff ff    	stx	0xffff
    eea9:	ff ff ff    	stx	0xffff
    eeac:	ff ff ff    	stx	0xffff
    eeaf:	ff ff ff    	stx	0xffff
    eeb2:	ff ff ff    	stx	0xffff
    eeb5:	ff ff ff    	stx	0xffff
    eeb8:	ff ff ff    	stx	0xffff
    eebb:	ff ff ff    	stx	0xffff
    eebe:	ff ff ff    	stx	0xffff
    eec1:	ff ff ff    	stx	0xffff
    eec4:	ff ff ff    	stx	0xffff
    eec7:	ff ff ff    	stx	0xffff
    eeca:	ff ff ff    	stx	0xffff
    eecd:	ff ff ff    	stx	0xffff
    eed0:	ff ff ff    	stx	0xffff
    eed3:	ff ff ff    	stx	0xffff
    eed6:	ff ff ff    	stx	0xffff
    eed9:	ff ff ff    	stx	0xffff
    eedc:	ff ff ff    	stx	0xffff
    eedf:	ff ff ff    	stx	0xffff
    eee2:	ff ff ff    	stx	0xffff
    eee5:	ff ff ff    	stx	0xffff
    eee8:	ff ff ff    	stx	0xffff
    eeeb:	ff ff ff    	stx	0xffff
    eeee:	ff ff ff    	stx	0xffff
    eef1:	ff ff ff    	stx	0xffff
    eef4:	ff ff ff    	stx	0xffff
    eef7:	ff ff ff    	stx	0xffff
    eefa:	ff ff ff    	stx	0xffff
    eefd:	ff ff ff    	stx	0xffff
    ef00:	ff ff ff    	stx	0xffff
    ef03:	ff ff ff    	stx	0xffff
    ef06:	ff ff ff    	stx	0xffff
    ef09:	ff ff ff    	stx	0xffff
    ef0c:	ff ff ff    	stx	0xffff
    ef0f:	ff ff ff    	stx	0xffff
    ef12:	ff ff ff    	stx	0xffff
    ef15:	ff ff ff    	stx	0xffff
    ef18:	ff ff ff    	stx	0xffff
    ef1b:	ff ff ff    	stx	0xffff
    ef1e:	ff ff ff    	stx	0xffff
    ef21:	ff ff ff    	stx	0xffff
    ef24:	ff ff ff    	stx	0xffff
    ef27:	ff ff ff    	stx	0xffff
    ef2a:	ff ff ff    	stx	0xffff
    ef2d:	ff ff ff    	stx	0xffff
    ef30:	ff ff ff    	stx	0xffff
    ef33:	ff ff ff    	stx	0xffff
    ef36:	ff ff ff    	stx	0xffff
    ef39:	ff ff ff    	stx	0xffff
    ef3c:	ff ff ff    	stx	0xffff
    ef3f:	ff ff ff    	stx	0xffff
    ef42:	ff ff ff    	stx	0xffff
    ef45:	ff ff ff    	stx	0xffff
    ef48:	ff ff ff    	stx	0xffff
    ef4b:	ff ff ff    	stx	0xffff
    ef4e:	ff ff ff    	stx	0xffff
    ef51:	ff ff ff    	stx	0xffff
    ef54:	ff ff ff    	stx	0xffff
    ef57:	ff ff ff    	stx	0xffff
    ef5a:	ff ff ff    	stx	0xffff
    ef5d:	ff ff ff    	stx	0xffff
    ef60:	ff ff ff    	stx	0xffff
    ef63:	ff ff ff    	stx	0xffff
    ef66:	ff ff ff    	stx	0xffff
    ef69:	ff ff ff    	stx	0xffff
    ef6c:	ff ff ff    	stx	0xffff
    ef6f:	ff ff ff    	stx	0xffff
    ef72:	ff ff ff    	stx	0xffff
    ef75:	ff ff ff    	stx	0xffff
    ef78:	ff ff ff    	stx	0xffff
    ef7b:	ff ff ff    	stx	0xffff
    ef7e:	ff ff ff    	stx	0xffff
    ef81:	ff ff ff    	stx	0xffff
    ef84:	ff ff ff    	stx	0xffff
    ef87:	ff ff ff    	stx	0xffff
    ef8a:	ff ff ff    	stx	0xffff
    ef8d:	ff ff ff    	stx	0xffff
    ef90:	ff ff ff    	stx	0xffff
    ef93:	ff ff ff    	stx	0xffff
    ef96:	ff ff ff    	stx	0xffff
    ef99:	ff ff ff    	stx	0xffff
    ef9c:	ff ff ff    	stx	0xffff
    ef9f:	ff ff ff    	stx	0xffff
    efa2:	ff ff ff    	stx	0xffff
    efa5:	ff ff ff    	stx	0xffff
    efa8:	ff ff ff    	stx	0xffff
    efab:	ff ff ff    	stx	0xffff
    efae:	ff ff ff    	stx	0xffff
    efb1:	ff ff ff    	stx	0xffff
    efb4:	ff ff ff    	stx	0xffff
    efb7:	ff ff ff    	stx	0xffff
    efba:	ff ff ff    	stx	0xffff
    efbd:	ff ff ff    	stx	0xffff
    efc0:	ff ff ff    	stx	0xffff
    efc3:	ff ff ff    	stx	0xffff
    efc6:	ff ff ff    	stx	0xffff
    efc9:	ff ff ff    	stx	0xffff
    efcc:	ff ff ff    	stx	0xffff
    efcf:	ff ff ff    	stx	0xffff
    efd2:	ff ff ff    	stx	0xffff
    efd5:	ff ff ff    	stx	0xffff
    efd8:	ff ff ff    	stx	0xffff
    efdb:	ff ff ff    	stx	0xffff
    efde:	ff ff ff    	stx	0xffff
    efe1:	ff ff ff    	stx	0xffff
    efe4:	ff ff ff    	stx	0xffff
    efe7:	ff ff ff    	stx	0xffff
    efea:	ff ff ff    	stx	0xffff
    efed:	ff ff ff    	stx	0xffff
    eff0:	ff ff ff    	stx	0xffff
    eff3:	ff ff ff    	stx	0xffff
    eff6:	ff ff ff    	stx	0xffff
    eff9:	ff ff ff    	stx	0xffff
    effc:	ff ff ff    	stx	0xffff
    efff:	ff ff ff    	stx	0xffff
    f002:	ff ff ff    	stx	0xffff
    f005:	ff ff ff    	stx	0xffff
    f008:	ff ff ff    	stx	0xffff
    f00b:	ff ff ff    	stx	0xffff
    f00e:	ff ff ff    	stx	0xffff
    f011:	ff ff ff    	stx	0xffff
    f014:	ff ff ff    	stx	0xffff
    f017:	ff ff ff    	stx	0xffff
    f01a:	ff ff ff    	stx	0xffff
    f01d:	ff ff ff    	stx	0xffff
    f020:	ff ff ff    	stx	0xffff
    f023:	ff ff ff    	stx	0xffff
    f026:	ff ff ff    	stx	0xffff
    f029:	ff ff ff    	stx	0xffff
    f02c:	ff ff ff    	stx	0xffff
    f02f:	ff ff ff    	stx	0xffff
    f032:	ff ff ff    	stx	0xffff
    f035:	ff ff ff    	stx	0xffff
    f038:	ff ff ff    	stx	0xffff
    f03b:	ff ff ff    	stx	0xffff
    f03e:	ff ff ff    	stx	0xffff
    f041:	ff ff ff    	stx	0xffff
    f044:	ff ff ff    	stx	0xffff
    f047:	ff ff ff    	stx	0xffff
    f04a:	ff ff ff    	stx	0xffff
    f04d:	ff ff ff    	stx	0xffff
    f050:	ff ff ff    	stx	0xffff
    f053:	ff ff ff    	stx	0xffff
    f056:	ff ff ff    	stx	0xffff
    f059:	ff ff ff    	stx	0xffff
    f05c:	ff ff ff    	stx	0xffff
    f05f:	ff ff ff    	stx	0xffff
    f062:	ff ff ff    	stx	0xffff
    f065:	ff ff ff    	stx	0xffff
    f068:	ff ff ff    	stx	0xffff
    f06b:	ff ff ff    	stx	0xffff
    f06e:	ff ff ff    	stx	0xffff
    f071:	ff ff ff    	stx	0xffff
    f074:	ff ff ff    	stx	0xffff
    f077:	ff ff ff    	stx	0xffff
    f07a:	ff ff ff    	stx	0xffff
    f07d:	ff ff ff    	stx	0xffff
    f080:	ff ff ff    	stx	0xffff
    f083:	ff ff ff    	stx	0xffff
    f086:	ff ff ff    	stx	0xffff
    f089:	ff ff ff    	stx	0xffff
    f08c:	ff ff ff    	stx	0xffff
    f08f:	ff ff ff    	stx	0xffff
    f092:	ff ff ff    	stx	0xffff
    f095:	ff ff ff    	stx	0xffff
    f098:	ff ff ff    	stx	0xffff
    f09b:	ff ff ff    	stx	0xffff
    f09e:	ff ff ff    	stx	0xffff
    f0a1:	ff ff ff    	stx	0xffff
    f0a4:	ff ff ff    	stx	0xffff
    f0a7:	ff ff ff    	stx	0xffff
    f0aa:	ff ff ff    	stx	0xffff
    f0ad:	ff ff ff    	stx	0xffff
    f0b0:	ff ff ff    	stx	0xffff
    f0b3:	ff ff ff    	stx	0xffff
    f0b6:	ff ff ff    	stx	0xffff
    f0b9:	ff ff ff    	stx	0xffff
    f0bc:	ff ff ff    	stx	0xffff
    f0bf:	ff ff ff    	stx	0xffff
    f0c2:	ff ff ff    	stx	0xffff
    f0c5:	ff ff ff    	stx	0xffff
    f0c8:	ff ff ff    	stx	0xffff
    f0cb:	ff ff ff    	stx	0xffff
    f0ce:	ff ff ff    	stx	0xffff
    f0d1:	ff ff ff    	stx	0xffff
    f0d4:	ff ff ff    	stx	0xffff
    f0d7:	ff ff ff    	stx	0xffff
    f0da:	ff ff ff    	stx	0xffff
    f0dd:	ff ff ff    	stx	0xffff
    f0e0:	ff ff ff    	stx	0xffff
    f0e3:	ff ff ff    	stx	0xffff
    f0e6:	ff ff ff    	stx	0xffff
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
    f9e0:	ff ff ff    	stx	0xffff
    f9e3:	ff ff ff    	stx	0xffff
    f9e6:	ff ff ff    	stx	0xffff
    f9e9:	ff ff ff    	stx	0xffff
    f9ec:	ff ff ff    	stx	0xffff
    f9ef:	ff ff ff    	stx	0xffff
    f9f2:	ff ff ff    	stx	0xffff
    f9f5:	ff ff ff    	stx	0xffff
    f9f8:	ff ff ff    	stx	0xffff
    f9fb:	ff ff ff    	stx	0xffff
    f9fe:	ff ff ff    	stx	0xffff
    fa01:	ff ff ff    	stx	0xffff
    fa04:	ff ff ff    	stx	0xffff
    fa07:	ff ff ff    	stx	0xffff
    fa0a:	ff ff ff    	stx	0xffff
    fa0d:	ff ff ff    	stx	0xffff
    fa10:	ff ff ff    	stx	0xffff
    fa13:	ff ff ff    	stx	0xffff
    fa16:	ff ff ff    	stx	0xffff
    fa19:	ff ff ff    	stx	0xffff
    fa1c:	ff ff ff    	stx	0xffff
    fa1f:	ff ff ff    	stx	0xffff
    fa22:	ff ff ff    	stx	0xffff
    fa25:	ff ff ff    	stx	0xffff
    fa28:	ff ff ff    	stx	0xffff
    fa2b:	ff ff ff    	stx	0xffff
    fa2e:	ff ff ff    	stx	0xffff
    fa31:	ff ff ff    	stx	0xffff
    fa34:	ff ff ff    	stx	0xffff
    fa37:	ff ff ff    	stx	0xffff
    fa3a:	ff ff ff    	stx	0xffff
    fa3d:	ff ff ff    	stx	0xffff
    fa40:	ff ff ff    	stx	0xffff
    fa43:	ff ff ff    	stx	0xffff
    fa46:	ff ff ff    	stx	0xffff
    fa49:	ff ff ff    	stx	0xffff
    fa4c:	ff ff ff    	stx	0xffff
    fa4f:	ff ff ff    	stx	0xffff
    fa52:	ff ff ff    	stx	0xffff
    fa55:	ff ff ff    	stx	0xffff
    fa58:	ff ff ff    	stx	0xffff
    fa5b:	ff ff ff    	stx	0xffff
    fa5e:	ff ff ff    	stx	0xffff
    fa61:	ff ff ff    	stx	0xffff
    fa64:	ff ff ff    	stx	0xffff
    fa67:	ff ff ff    	stx	0xffff
    fa6a:	ff ff ff    	stx	0xffff
    fa6d:	ff ff ff    	stx	0xffff
    fa70:	ff ff ff    	stx	0xffff
    fa73:	ff ff ff    	stx	0xffff
    fa76:	ff ff ff    	stx	0xffff
    fa79:	ff ff ff    	stx	0xffff
    fa7c:	ff ff ff    	stx	0xffff
    fa7f:	ff ff ff    	stx	0xffff
    fa82:	ff ff ff    	stx	0xffff
    fa85:	ff ff ff    	stx	0xffff
    fa88:	ff ff ff    	stx	0xffff
    fa8b:	ff ff ff    	stx	0xffff
    fa8e:	ff ff ff    	stx	0xffff
    fa91:	ff ff ff    	stx	0xffff
    fa94:	ff ff ff    	stx	0xffff
    fa97:	ff ff ff    	stx	0xffff
    fa9a:	ff ff ff    	stx	0xffff
    fa9d:	ff ff ff    	stx	0xffff
    faa0:	ff ff ff    	stx	0xffff
    faa3:	ff ff ff    	stx	0xffff
    faa6:	ff ff ff    	stx	0xffff
    faa9:	ff ff ff    	stx	0xffff
    faac:	ff ff ff    	stx	0xffff
    faaf:	ff ff ff    	stx	0xffff
    fab2:	ff ff ff    	stx	0xffff
    fab5:	ff ff ff    	stx	0xffff
    fab8:	ff ff ff    	stx	0xffff
    fabb:	ff ff ff    	stx	0xffff
    fabe:	ff ff ff    	stx	0xffff
    fac1:	ff ff ff    	stx	0xffff
    fac4:	ff ff ff    	stx	0xffff
    fac7:	ff ff ff    	stx	0xffff
    faca:	ff ff ff    	stx	0xffff
    facd:	ff ff ff    	stx	0xffff
    fad0:	ff ff ff    	stx	0xffff
    fad3:	ff ff ff    	stx	0xffff
    fad6:	ff ff ff    	stx	0xffff
    fad9:	ff ff ff    	stx	0xffff
    fadc:	ff ff ff    	stx	0xffff
    fadf:	ff ff ff    	stx	0xffff
    fae2:	ff ff ff    	stx	0xffff
    fae5:	ff ff ff    	stx	0xffff
    fae8:	ff ff ff    	stx	0xffff
    faeb:	ff ff ff    	stx	0xffff
    faee:	ff ff ff    	stx	0xffff
    faf1:	ff ff ff    	stx	0xffff
    faf4:	ff ff ff    	stx	0xffff
    faf7:	ff ff ff    	stx	0xffff
    fafa:	ff ff ff    	stx	0xffff
    fafd:	ff ff ff    	stx	0xffff
    fb00:	ff ff ff    	stx	0xffff
    fb03:	ff ff ff    	stx	0xffff
    fb06:	ff ff ff    	stx	0xffff
    fb09:	ff ff ff    	stx	0xffff
    fb0c:	ff ff ff    	stx	0xffff
    fb0f:	ff ff ff    	stx	0xffff
    fb12:	ff ff ff    	stx	0xffff
    fb15:	ff ff ff    	stx	0xffff
    fb18:	ff ff ff    	stx	0xffff
    fb1b:	ff ff ff    	stx	0xffff
    fb1e:	ff ff ff    	stx	0xffff
    fb21:	ff ff ff    	stx	0xffff
    fb24:	ff ff ff    	stx	0xffff
    fb27:	ff ff ff    	stx	0xffff
    fb2a:	ff ff ff    	stx	0xffff
    fb2d:	ff ff ff    	stx	0xffff
    fb30:	ff ff ff    	stx	0xffff
    fb33:	ff ff ff    	stx	0xffff
    fb36:	ff ff ff    	stx	0xffff
    fb39:	ff ff ff    	stx	0xffff
    fb3c:	ff ff ff    	stx	0xffff
    fb3f:	ff ff ff    	stx	0xffff
    fb42:	ff ff ff    	stx	0xffff
    fb45:	ff ff ff    	stx	0xffff
    fb48:	ff ff ff    	stx	0xffff
    fb4b:	ff ff ff    	stx	0xffff
    fb4e:	ff ff ff    	stx	0xffff
    fb51:	ff ff ff    	stx	0xffff
    fb54:	ff ff ff    	stx	0xffff
    fb57:	ff ff ff    	stx	0xffff
    fb5a:	ff ff ff    	stx	0xffff
    fb5d:	ff ff ff    	stx	0xffff
    fb60:	ff ff ff    	stx	0xffff
    fb63:	ff ff ff    	stx	0xffff
    fb66:	ff ff ff    	stx	0xffff
    fb69:	ff ff ff    	stx	0xffff
    fb6c:	ff ff ff    	stx	0xffff
    fb6f:	ff ff ff    	stx	0xffff
    fb72:	ff ff ff    	stx	0xffff
    fb75:	ff ff ff    	stx	0xffff
    fb78:	ff ff ff    	stx	0xffff
    fb7b:	ff ff ff    	stx	0xffff
    fb7e:	ff ff ff    	stx	0xffff
    fb81:	ff ff ff    	stx	0xffff
    fb84:	ff ff ff    	stx	0xffff
    fb87:	ff ff ff    	stx	0xffff
    fb8a:	ff ff ff    	stx	0xffff
    fb8d:	ff ff ff    	stx	0xffff
    fb90:	ff ff ff    	stx	0xffff
    fb93:	ff ff ff    	stx	0xffff
    fb96:	ff ff ff    	stx	0xffff
    fb99:	ff ff ff    	stx	0xffff
    fb9c:	ff ff ff    	stx	0xffff
    fb9f:	ff ff ff    	stx	0xffff
    fba2:	ff ff ff    	stx	0xffff
    fba5:	ff ff ff    	stx	0xffff
    fba8:	ff ff ff    	stx	0xffff
    fbab:	ff ff ff    	stx	0xffff
    fbae:	ff ff ff    	stx	0xffff
    fbb1:	ff ff ff    	stx	0xffff
    fbb4:	ff ff ff    	stx	0xffff
    fbb7:	ff ff ff    	stx	0xffff
    fbba:	ff ff ff    	stx	0xffff
    fbbd:	ff ff ff    	stx	0xffff
    fbc0:	ff ff ff    	stx	0xffff
    fbc3:	ff ff ff    	stx	0xffff
    fbc6:	ff ff ff    	stx	0xffff
    fbc9:	ff ff ff    	stx	0xffff
    fbcc:	ff ff ff    	stx	0xffff
    fbcf:	ff ff ff    	stx	0xffff
    fbd2:	ff ff ff    	stx	0xffff
    fbd5:	ff ff ff    	stx	0xffff
    fbd8:	ff ff ff    	stx	0xffff
    fbdb:	ff ff ff    	stx	0xffff
    fbde:	ff ff ff    	stx	0xffff
    fbe1:	ff ff ff    	stx	0xffff
    fbe4:	ff ff ff    	stx	0xffff
    fbe7:	ff ff ff    	stx	0xffff
    fbea:	ff ff ff    	stx	0xffff
    fbed:	ff ff ff    	stx	0xffff
    fbf0:	ff ff ff    	stx	0xffff
    fbf3:	ff ff ff    	stx	0xffff
    fbf6:	ff ff ff    	stx	0xffff
    fbf9:	ff ff ff    	stx	0xffff
    fbfc:	ff ff ff    	stx	0xffff
    fbff:	ff ff ff    	stx	0xffff
    fc02:	ff ff ff    	stx	0xffff
    fc05:	ff ff ff    	stx	0xffff
    fc08:	ff ff ff    	stx	0xffff
    fc0b:	ff ff ff    	stx	0xffff
    fc0e:	ff ff ff    	stx	0xffff
    fc11:	ff ff ff    	stx	0xffff
    fc14:	ff ff ff    	stx	0xffff
    fc17:	ff ff ff    	stx	0xffff
    fc1a:	ff ff ff    	stx	0xffff
    fc1d:	ff ff ff    	stx	0xffff
    fc20:	ff ff ff    	stx	0xffff
    fc23:	ff ff ff    	stx	0xffff
    fc26:	ff ff ff    	stx	0xffff
    fc29:	ff ff ff    	stx	0xffff
    fc2c:	ff ff ff    	stx	0xffff
    fc2f:	ff ff ff    	stx	0xffff
    fc32:	ff ff ff    	stx	0xffff
    fc35:	ff ff ff    	stx	0xffff
    fc38:	ff ff ff    	stx	0xffff
    fc3b:	ff ff ff    	stx	0xffff
    fc3e:	ff ff ff    	stx	0xffff
    fc41:	ff ff ff    	stx	0xffff
    fc44:	ff ff ff    	stx	0xffff
    fc47:	ff ff ff    	stx	0xffff
    fc4a:	ff ff ff    	stx	0xffff
    fc4d:	ff ff ff    	stx	0xffff
    fc50:	ff ff ff    	stx	0xffff
    fc53:	ff ff ff    	stx	0xffff
    fc56:	ff ff ff    	stx	0xffff
    fc59:	ff ff ff    	stx	0xffff
    fc5c:	ff ff ff    	stx	0xffff
    fc5f:	ff ff ff    	stx	0xffff
    fc62:	ff ff ff    	stx	0xffff
    fc65:	ff ff ff    	stx	0xffff
    fc68:	ff ff ff    	stx	0xffff
    fc6b:	ff ff ff    	stx	0xffff
    fc6e:	ff ff ff    	stx	0xffff
    fc71:	ff ff ff    	stx	0xffff
    fc74:	ff ff ff    	stx	0xffff
    fc77:	ff ff ff    	stx	0xffff
    fc7a:	ff ff ff    	stx	0xffff
    fc7d:	ff ff ff    	stx	0xffff
    fc80:	ff ff ff    	stx	0xffff
    fc83:	ff ff ff    	stx	0xffff
    fc86:	ff ff ff    	stx	0xffff
    fc89:	ff ff ff    	stx	0xffff
    fc8c:	ff ff ff    	stx	0xffff
    fc8f:	ff ff ff    	stx	0xffff
    fc92:	ff ff ff    	stx	0xffff
    fc95:	ff ff ff    	stx	0xffff
    fc98:	ff ff ff    	stx	0xffff
    fc9b:	ff ff ff    	stx	0xffff
    fc9e:	ff ff ff    	stx	0xffff
    fca1:	ff ff ff    	stx	0xffff
    fca4:	ff ff ff    	stx	0xffff
    fca7:	ff ff ff    	stx	0xffff
    fcaa:	ff ff ff    	stx	0xffff
    fcad:	ff ff ff    	stx	0xffff
    fcb0:	ff ff ff    	stx	0xffff
    fcb3:	ff ff ff    	stx	0xffff
    fcb6:	ff ff ff    	stx	0xffff
    fcb9:	ff ff ff    	stx	0xffff
    fcbc:	ff ff ff    	stx	0xffff
    fcbf:	ff ff ff    	stx	0xffff
    fcc2:	ff ff ff    	stx	0xffff
    fcc5:	ff ff ff    	stx	0xffff
    fcc8:	ff ff ff    	stx	0xffff
    fccb:	ff ff ff    	stx	0xffff
    fcce:	ff ff ff    	stx	0xffff
    fcd1:	ff ff ff    	stx	0xffff
    fcd4:	ff ff ff    	stx	0xffff
    fcd7:	ff ff ff    	stx	0xffff
    fcda:	ff ff ff    	stx	0xffff
    fcdd:	ff ff ff    	stx	0xffff
    fce0:	ff ff ff    	stx	0xffff
    fce3:	ff ff ff    	stx	0xffff
    fce6:	ff ff ff    	stx	0xffff
    fce9:	ff ff ff    	stx	0xffff
    fcec:	ff ff ff    	stx	0xffff
    fcef:	ff ff ff    	stx	0xffff
    fcf2:	ff ff ff    	stx	0xffff
    fcf5:	ff ff ff    	stx	0xffff
    fcf8:	ff ff ff    	stx	0xffff
    fcfb:	ff ff ff    	stx	0xffff
    fcfe:	ff ff ff    	stx	0xffff
    fd01:	ff ff ff    	stx	0xffff
    fd04:	ff ff ff    	stx	0xffff
    fd07:	ff ff ff    	stx	0xffff
    fd0a:	ff ff ff    	stx	0xffff
    fd0d:	ff ff ff    	stx	0xffff
    fd10:	ff ff ff    	stx	0xffff
    fd13:	ff ff ff    	stx	0xffff
    fd16:	ff ff ff    	stx	0xffff
    fd19:	ff ff ff    	stx	0xffff
    fd1c:	ff ff ff    	stx	0xffff
    fd1f:	ff ff ff    	stx	0xffff
    fd22:	ff ff ff    	stx	0xffff
    fd25:	ff ff ff    	stx	0xffff
    fd28:	ff ff ff    	stx	0xffff
    fd2b:	ff ff ff    	stx	0xffff
    fd2e:	ff ff ff    	stx	0xffff
    fd31:	ff ff ff    	stx	0xffff
    fd34:	ff ff ff    	stx	0xffff
    fd37:	ff ff ff    	stx	0xffff
    fd3a:	ff ff ff    	stx	0xffff
    fd3d:	ff ff ff    	stx	0xffff
    fd40:	ff ff ff    	stx	0xffff
    fd43:	ff ff ff    	stx	0xffff
    fd46:	ff ff ff    	stx	0xffff
    fd49:	ff ff ff    	stx	0xffff
    fd4c:	ff ff ff    	stx	0xffff
    fd4f:	ff ff ff    	stx	0xffff
    fd52:	ff ff ff    	stx	0xffff
    fd55:	ff ff ff    	stx	0xffff
    fd58:	ff ff ff    	stx	0xffff
    fd5b:	ff ff ff    	stx	0xffff
    fd5e:	ff ff ff    	stx	0xffff
    fd61:	ff ff ff    	stx	0xffff
    fd64:	ff ff ff    	stx	0xffff
    fd67:	ff ff ff    	stx	0xffff
    fd6a:	ff ff ff    	stx	0xffff
    fd6d:	ff ff ff    	stx	0xffff
    fd70:	ff ff ff    	stx	0xffff
    fd73:	ff ff ff    	stx	0xffff
    fd76:	ff ff ff    	stx	0xffff
    fd79:	ff ff ff    	stx	0xffff
    fd7c:	ff ff ff    	stx	0xffff
    fd7f:	ff ff ff    	stx	0xffff
    fd82:	ff ff ff    	stx	0xffff
    fd85:	ff ff ff    	stx	0xffff
    fd88:	ff ff ff    	stx	0xffff
    fd8b:	ff ff ff    	stx	0xffff
    fd8e:	ff ff ff    	stx	0xffff
    fd91:	ff ff ff    	stx	0xffff
    fd94:	ff ff ff    	stx	0xffff
    fd97:	ff ff ff    	stx	0xffff
    fd9a:	ff ff ff    	stx	0xffff
    fd9d:	ff ff ff    	stx	0xffff
    fda0:	ff ff ff    	stx	0xffff
    fda3:	ff ff ff    	stx	0xffff
    fda6:	ff ff ff    	stx	0xffff
    fda9:	ff ff ff    	stx	0xffff
    fdac:	ff ff ff    	stx	0xffff
    fdaf:	ff ff ff    	stx	0xffff
    fdb2:	ff ff ff    	stx	0xffff
    fdb5:	ff ff ff    	stx	0xffff
    fdb8:	ff ff ff    	stx	0xffff
    fdbb:	ff ff ff    	stx	0xffff
    fdbe:	ff ff ff    	stx	0xffff
    fdc1:	ff ff ff    	stx	0xffff
    fdc4:	ff ff ff    	stx	0xffff
    fdc7:	ff ff ff    	stx	0xffff
    fdca:	ff ff ff    	stx	0xffff
    fdcd:	ff ff ff    	stx	0xffff
    fdd0:	ff ff ff    	stx	0xffff
    fdd3:	ff ff ff    	stx	0xffff
    fdd6:	ff ff ff    	stx	0xffff
    fdd9:	ff ff ff    	stx	0xffff
    fddc:	ff ff ff    	stx	0xffff
    fddf:	ff ff ff    	stx	0xffff
    fde2:	ff ff ff    	stx	0xffff
    fde5:	ff ff ff    	stx	0xffff
    fde8:	ff ff ff    	stx	0xffff
    fdeb:	ff ff ff    	stx	0xffff
    fdee:	ff ff 7e    	stx	0xff7e
    fdf1:	91 56       	cmpa	*0x56
    fdf3:	ff ff ff    	stx	0xffff
    fdf6:	ff ff ff    	stx	0xffff
    fdf9:	ff ff ff    	stx	0xffff
    fdfc:	ff ff ff    	stx	0xffff
    fdff:	ff 7e a3    	stx	0x7ea3
    fe02:	18 ff ff ff 	sty	0xffff
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
    fe28:	7e fe e6    	jmp	0xfee6
    fe2b:	c5 02       	bitb	#0x2
    fe2d:	26 11       	bne	0x0xfe40
    fe2f:	c5 08       	bitb	#0x8
    fe31:	26 0d       	bne	0x0xfe40
    fe33:	b6 10 2f    	ldaa	0x102f
    fe36:	2b 0e       	bmi	0x0xfe46
    fe38:	7d 00 dd    	tst	0xdd
    fe3b:	26 29       	bne	0x0xfe66
    fe3d:	7e fe e6    	jmp	0xfee6
    fe40:	b6 10 2f    	ldaa	0x102f
    fe43:	7e fe e6    	jmp	0xfee6
    fe46:	81 ef       	cmpa	#0xef
    fe48:	22 32       	bhi	0x0xfe7c
    fe4a:	16          	tab
    fe4b:	c4 0f       	andb	#0xf
    fe4d:	f0 01 6f    	subb	0x16f
    fe50:	27 10       	beq	0x0xfe62
    fe52:	7d 00 df    	tst	0xdf
    fe55:	26 1f       	bne	0x0xfe76
    fe57:	c1 01       	cmpb	#0x1
    fe59:	26 1b       	bne	0x0xfe76
    fe5b:	84 f0       	anda	#0xf0
    fe5d:	4c          	inca
    fe5e:	97 dd       	staa	*0xdd
    fe60:	20 04       	bra	0x0xfe66
    fe62:	84 f0       	anda	#0xf0
    fe64:	97 dd       	staa	*0xdd
    fe66:	fe 01 c2    	ldx	0x1c2
    fe69:	a7 00       	staa	0x0,x
    fe6b:	8f          	xgdx
    fe6c:	5c          	incb
    fe6d:	c4 bf       	andb	#0xbf
    fe6f:	8f          	xgdx
    fe70:	ff 01 c2    	stx	0x1c2
    fe73:	7e fe e6    	jmp	0xfee6
    fe76:	7f 00 dd    	clr	0xdd
    fe79:	7e fe e6    	jmp	0xfee6
    fe7c:	81 f8       	cmpa	#0xf8
    fe7e:	26 22       	bne	0x0xfea2
    fe80:	d6 dc       	ldab	*0xdc
    fe82:	c1 f0       	cmpb	#0xf0
    fe84:	26 03       	bne	0x0xfe89
    fe86:	7e fe e6    	jmp	0xfee6
    fe89:	7d 01 15    	tst	0x115
    fe8c:	27 12       	beq	0x0xfea0
    fe8e:	13 f3 10 0e 	brclr	*0xf3, #0x10, 0x0xfea0
    fe92:	f6 01 15    	ldab	0x115
    fe95:	ce e3 aa    	ldx	#0xe3aa
    fe98:	3a          	abx
    fe99:	a6 00       	ldaa	0x0,x
    fe9b:	bd 91 6b    	jsr	0x916b
    fe9e:	86 f8       	ldaa	#0xf8
    fea0:	20 c4       	bra	0x0xfe66
    fea2:	81 ff       	cmpa	#0xff
    fea4:	26 03       	bne	0x0xfea9
    fea6:	7e 80 00    	jmp	0x8000
    fea9:	81 fa       	cmpa	#0xfa
    feab:	26 1b       	bne	0x0xfec8
    fead:	c6 01       	ldab	#0x1
    feaf:	f7 01 19    	stab	0x119
    feb2:	c6 80       	ldab	#0x80
    feb4:	d7 d2       	stab	*0xd2
    feb6:	7f 00 30    	clr	0x30
    feb9:	7f 00 d3    	clr	0xd3
    febc:	d6 73       	ldab	*0x73
    febe:	c1 02       	cmpb	#0x2
    fec0:	26 04       	bne	0x0xfec6
    fec2:	d6 74       	ldab	*0x74
    fec4:	d7 d3       	stab	*0xd3
    fec6:	20 9e       	bra	0x0xfe66
    fec8:	81 f7       	cmpa	#0xf7
    feca:	26 0b       	bne	0x0xfed7
    fecc:	c6 f0       	ldab	#0xf0
    fece:	d1 dd       	cmpb	*0xdd
    fed0:	26 14       	bne	0x0xfee6
    fed2:	7f 00 dd    	clr	0xdd
    fed5:	20 8f       	bra	0x0xfe66
    fed7:	81 f0       	cmpa	#0xf0
    fed9:	26 04       	bne	0x0xfedf
    fedb:	97 dd       	staa	*0xdd
    fedd:	20 87       	bra	0x0xfe66
    fedf:	81 f4       	cmpa	#0xf4
    fee1:	24 03       	bcc	0x0xfee6
    fee3:	7f 00 dd    	clr	0xdd
    fee6:	33          	pulb
    fee7:	c5 80       	bitb	#0x80
    fee9:	27 02       	beq	0x0xfeed
    feeb:	8d 01       	bsr	0x0xfeee
    feed:	3b          	rti
    feee:	fe 01 60    	ldx	0x160
    fef1:	bc 01 62    	cpx	0x162
    fef4:	27 16       	beq	0x0xff0c
    fef6:	a6 00       	ldaa	0x0,x
    fef8:	b7 10 2f    	staa	0x102f
    fefb:	8f          	xgdx
    fefc:	5c          	incb
    fefd:	c1 60       	cmpb	#0x60
    feff:	25 02       	bcs	0x0xff03
    ff01:	c6 40       	ldab	#0x40
    ff03:	fd 01 60    	std	0x160
    ff06:	86 ac       	ldaa	#0xac
    ff08:	b7 10 2d    	staa	0x102d
    ff0b:	39          	rts
    ff0c:	86 2c       	ldaa	#0x2c
    ff0e:	b7 10 2d    	staa	0x102d
    ff11:	7c 00 d7    	inc	0xd7
    ff14:	39          	rts
    ff15:	ff ff ff    	stx	0xffff
    ff18:	ff ff ff    	stx	0xffff
    ff1b:	ff ff ff    	stx	0xffff
    ff1e:	ff ff ff    	stx	0xffff
    ff21:	ff ff ff    	stx	0xffff
    ff24:	ff ff ff    	stx	0xffff
    ff27:	ff ff ff    	stx	0xffff
    ff2a:	ff ff ff    	stx	0xffff
    ff2d:	ff ff ff    	stx	0xffff
    ff30:	ff ff ff    	stx	0xffff
    ff33:	ff ff ff    	stx	0xffff
    ff36:	ff ff ff    	stx	0xffff
    ff39:	ff ff ff    	stx	0xffff
    ff3c:	ff ff ff    	stx	0xffff
    ff3f:	ff ff ff    	stx	0xffff
    ff42:	ff ff ff    	stx	0xffff
    ff45:	ff ff ff    	stx	0xffff
    ff48:	ff ff ff    	stx	0xffff
    ff4b:	ff ff ff    	stx	0xffff
    ff4e:	ff ff ff    	stx	0xffff
    ff51:	ff ff ff    	stx	0xffff
    ff54:	ff ff ff    	stx	0xffff
    ff57:	ff ff ff    	stx	0xffff
    ff5a:	ff ff ff    	stx	0xffff
    ff5d:	ff ff ff    	stx	0xffff
    ff60:	ff ff ff    	stx	0xffff
    ff63:	ff ff ff    	stx	0xffff
    ff66:	ff ff ff    	stx	0xffff
    ff69:	ff ff ff    	stx	0xffff
    ff6c:	ff ff ff    	stx	0xffff
    ff6f:	ff ff ff    	stx	0xffff
    ff72:	ff ff ff    	stx	0xffff
    ff75:	ff ff ff    	stx	0xffff
    ff78:	ff ff ff    	stx	0xffff
    ff7b:	ff ff ff    	stx	0xffff
    ff7e:	ff ff ff    	stx	0xffff
    ff81:	ff ff ff    	stx	0xffff
    ff84:	ff ff ff    	stx	0xffff
    ff87:	ff ff ff    	stx	0xffff
    ff8a:	ff ff ff    	stx	0xffff
    ff8d:	ff ff ff    	stx	0xffff
    ff90:	ff ff ff    	stx	0xffff
    ff93:	ff ff ff    	stx	0xffff
    ff96:	ff ff ff    	stx	0xffff
    ff99:	ff ff ff    	stx	0xffff
    ff9c:	ff ff ff    	stx	0xffff
    ff9f:	ff 01 01    	stx	0x101
    ffa2:	20 fc       	bra	0x0xffa0
    ffa4:	ff ff ff    	stx	0xffff
    ffa7:	ff ff ff    	stx	0xffff
    ffaa:	ff ff ff    	stx	0xffff
    ffad:	ff ff ff    	stx	0xffff
    ffb0:	4f          	clra
    ffb1:	32          	pula
    ffb2:	43          	coma
    ffb3:	50          	negb
    ffb4:	55          	.byte	0x55
    ffb5:	20 56       	bra	0x0x1000d
    ffb7:	2e 31       	bgt	0x0xffea
    ffb9:	2e 37       	bgt	0x0xfff2
    ffbb:	31          	ins
    ffbc:	20 28       	bra	0x0xffe6
    ffbe:	43          	coma
    ffbf:	29 32       	bvs	0x0xfff3
    ffc1:	30          	tsx
    ffc2:	30          	tsx
    ffc3:	30          	tsx
    ffc4:	20 53       	bra	0x0x10019
    ffc6:	54          	lsrb
    ffc7:	55          	.byte	0x55
    ffc8:	44          	lsra
    ffc9:	49          	rola
    ffca:	4f          	clra
    ffcb:	20 45       	bra	0x0x10012
    ffcd:	4c          	inca
    ffce:	45          	.byte	0x45
    ffcf:	43          	coma
    ffd0:	54          	lsrb
    ffd1:	52          	.byte	0x52
    ffd2:	4f          	clra
    ffd3:	4e          	.byte	0x4e
    ffd4:	49          	rola
    ffd5:	43          	coma
    ffd6:	fe 20 ff    	ldx	0x20ff
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
