;C:\XA\INCLUDE\stdlib.h: 92: extern int atexit(void (*)(void));
;C:\XA\INCLUDE\stdlib.h: 96: extern void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
;C:\XA\INCLUDE\stdlib.h: 97: extern void * bsearch(const void *, void *, size_t, size_t, int(*)(const void *, const void *));
	psect	rdata,class=DATA,space=0,align=0
	global	_testbbu
	align.w
_testbbu:
	dw	0
	dw	0
;	param _time assigned to r3 on entry
	psect	ltext,class=CODE,space=0,align=2
	global	_waitms
	signat	_waitms,4152
	psect	ltext
	file	"MAS.C"
	line	106
_waitms:
;MAS.C: 107: while (time-- > 0)
	line	107
	jmp	l6
l7:
;MAS.C: 108: { n= 750;
; _n allocated to r1
	line	108
	mov.w	r1,#02EEh
;MAS.C: 109: while (n>0) n--;
	line	109
	jmp	l9
l10:
	sub.w	r1,#01h
l9:
	mov.w	r0,r1
	bne	l10
	line	110
l6:
;MAS.C: 110: }
	line	107
	add.w	r3,#-1
	bcs	l7
;MAS.C: 111: }
	line	111
	ret
	align.w
	global	_waitus
	signat	_waitus,4152
	psect	ltext
;MAS.C: 114: void waitus(unsigned int time)
;MAS.C: 115: {
;	param _time assigned to r3 on entry
	line	115
_waitus:
;MAS.C: 116: while (time-- > 0) ;
	line	116
l13:
	add.w	r3,#-1
	bcs	l13
;MAS.C: 117: }
	line	117
	ret
	align.w
	global	_backchar
	signat	_backchar,4152
	global	_put_char0
	signat	_put_char0,4152
	global	_AnswerBuf
	psect	ltext
;MAS.C: 119: extern void put_char0(char n);
;MAS.C: 121: void backchar(unsigned char n)
;MAS.C: 122: { unsigned char i;
;	param _n assigned to r3l on entry
	line	122
_backchar:
	push	r4
	adds.w	r7,#-2
;_n stored from r3l
	line	123
	mov.b	[r7],r3l
;MAS.C: 123: put_char0 (' ');
	mov.b	r3l,#020h
	fcall	_put_char0
;MAS.C: 124: for (i=0;i<n;i++) put_char0(AnswerBuf[i]);
; _i allocated to r4l
	line	124
	movs.b	r4l,#00h
	cmp.b	[r7],#00h
	beq	l18
l17:
	mov.b	r0l,r4l
	movs.b	r0h,#00h
	mov.b	r3l,[r0+_AnswerBuf]
	fcall	_put_char0
	adds.b	r4l,#01h
	cmp.b	r4l,[r7]
	bcs	l17
l18:
;MAS.C: 125: put_char0 (13);
	line	125
	mov.b	r3l,#0Dh
	fcall	_put_char0
;MAS.C: 126: }
	line	126
	adds.w	r7,#02h
	pop	r4
	ret
	align.w
	global	_get_syncstat
	signat	_get_syncstat,24
	global	_SndRequest
	signat	_SndRequest,8249
	global	_AnswerCheck
	signat	_AnswerCheck,25
	global	_syncstatus
	psect	ltext
;MAS.C: 129: void get_syncstat(void)
;MAS.C: 130: {
	line	130
_get_syncstat:
;MAS.C: 131: if (SndRequest(0x15,"GS")==0)
	line	131
	mov.w	r2,#u19
	mov.b	r3l,#015h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l20
;MAS.C: 132: if (AnswerCheck()==0)
	line	132
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l20
;MAS.C: 133: { syncstatus= AnswerBuf[2] & 1;
	line	133
	mov.w	r0,#_AnswerBuf+02h
	mov.b	r0l,[r0]
	and.b	r0l,#01h
	mov.b	_syncstatus,r0l
;MAS.C: 134: backchar(3);
	line	134
	movs.b	r3l,#03h
	fjmp	_backchar
	line	135
;MAS.C: 135: }
;MAS.C: 136: }
	line	136
l20:
	ret
	align.w
	global	_gfase1
	signat	_gfase1,24
	global	_SndLong
	signat	_SndLong,12345
	global	_Number
	psect	ltext
;MAS.C: 139: void gfase1(void)
;MAS.C: 140: { SndLong(0x00,"HG",Number);
	line	140
_gfase1:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u29
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 141: }
	line	141
	ret
	align.w
	global	_gfase2
	signat	_gfase2,24
	psect	ltext
;MAS.C: 144: void gfase2(void)
;MAS.C: 145: { SndLong(0x00,"JG",Number);
	line	145
_gfase2:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u39
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 146: }
	line	146
	ret
	align.w
	global	_load1g7
	signat	_load1g7,24
	psect	ltext
;MAS.C: 149: void load1g7(void)
;MAS.C: 150: { SndLong(0x00,"HK",Number);
	line	150
_load1g7:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u49
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 151: }
	line	151
	ret
	align.w
	global	_load2g7
	signat	_load2g7,24
	psect	ltext
;MAS.C: 154: void load2g7(void)
;MAS.C: 155: { SndLong(0x00,"JK",Number);
	line	155
_load2g7:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u59
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 156: }
	line	156
	ret
	align.w
	global	_mfase1
	signat	_mfase1,24
	psect	ltext
;MAS.C: 159: void mfase1(void)
;MAS.C: 160: { SndLong(0x00,"HM",Number);
	line	160
_mfase1:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u69
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 161: }
	line	161
	ret
	align.w
	global	_mfase2
	signat	_mfase2,24
	psect	ltext
;MAS.C: 164: void mfase2(void)
;MAS.C: 165: { SndLong(0x00,"JM",Number);
	line	165
_mfase2:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u79
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 166: }
	line	166
	ret
	align.w
	global	_load1ms
	signat	_load1ms,24
	psect	ltext
;MAS.C: 169: void load1ms(void)
;MAS.C: 170: { SndLong(0x00,"HL",Number);
	line	170
_load1ms:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u89
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 171: }
	line	171
	ret
	align.w
	global	_load2ms
	signat	_load2ms,24
	psect	ltext
;MAS.C: 174: void load2ms(void)
;MAS.C: 175: { SndLong(0x00,"JL",Number);
	line	175
_load2ms:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u99
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 176: }
	line	176
	ret
	align.w
	global	_global1
	signat	_global1,24
	psect	ltext
;MAS.C: 179: void global1(void)
;MAS.C: 180: { SndLong(0x00,"HO",Number);
	line	180
_global1:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u109
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 181: }
	line	181
	ret
	align.w
	global	_global2
	signat	_global2,24
	psect	ltext
;MAS.C: 184: void global2(void)
;MAS.C: 185: { SndLong(0x00,"JO",Number);
	line	185
_global2:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u119
	movs.b	r3l,#00h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 186: }
	line	186
	ret
	align.w
	global	_scload1
	signat	_scload1,24
	global	_SndInt
	signat	_SndInt,12345
	psect	ltext
;MAS.C: 189: void scload1(void)
;MAS.C: 190: { SndInt(0x00,"HH",Number);
	line	190
_scload1:
	mov.w	r1,_Number
	mov.w	r2,#u129
	movs.b	r3l,#00h
	fjmp	_SndInt
	align.w
	global	_scload2
	signat	_scload2,24
	psect	ltext
;MAS.C: 194: void scload2(void)
;MAS.C: 195: { SndInt(0x00,"JH",Number);
	line	195
_scload2:
	mov.w	r1,_Number
	mov.w	r2,#u139
	movs.b	r3l,#00h
	fjmp	_SndInt
	align.w
	global	_sch1
	signat	_sch1,24
	psect	ltext
;MAS.C: 198: void sch1(void)
;MAS.C: 199: { SndLong(0x00,"HS",(unsigned int)Number);
	line	199
_sch1:
	mov.w	r0,_Number
	movs.w	r1,#00h
	push	r0,r1
	mov.w	r2,#u149
	mov.b	r3l,r1l
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 200: }
	line	200
	ret
	align.w
	global	_sch2
	signat	_sch2,24
	psect	ltext
;MAS.C: 203: void sch2(void)
;MAS.C: 204: { SndLong(0x00,"JS",(unsigned int)Number);
	line	204
_sch2:
	mov.w	r0,_Number
	movs.w	r1,#00h
	push	r0,r1
	mov.w	r2,#u159
	mov.b	r3l,r1l
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 205: }
	line	205
	ret
	align.w
	global	_phases1
	signat	_phases1,24
	global	_req
	global	_SndSpecial
	signat	_SndSpecial,12345
	psect	ltext
;MAS.C: 208: void phases1(void)
;MAS.C: 209: { if (req)
	line	209
_phases1:
	jnb	_req,l38
;MAS.C: 210: { req= 0;
	line	210
	clr	_req
;MAS.C: 211: if (SndRequest(0x00,"HX")==0)
	line	211
	mov.w	r2,#u169
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l37
;MAS.C: 212: if (AnswerCheck()==0)
	line	212
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l37
;MAS.C: 213: { waitms(100);
	line	213
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 214: backchar(16);
	line	214
	mov.b	r3l,#010h
	fjmp	_backchar
	line	215
;MAS.C: 215: }
;MAS.C: 216: }
	line	216
;MAS.C: 217: else SndSpecial(0x00, "HX",'$');
	line	217
l38:
	mov.b	r1l,#024h
	mov.w	r2,#u179
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 218: }
	line	218
l37:
	ret
	align.w
	global	_phases2
	signat	_phases2,24
	psect	ltext
;MAS.C: 221: void phases2(void)
;MAS.C: 222: { if (req)
	line	222
_phases2:
	jnb	_req,l43
;MAS.C: 223: { req= 0;
	line	223
	clr	_req
;MAS.C: 224: if (SndRequest(0x00,"JX")==0)
	line	224
	mov.w	r2,#u189
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l42
;MAS.C: 225: if (AnswerCheck()==0)
	line	225
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l42
;MAS.C: 226: { waitms(100);
	line	226
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 227: backchar(16);
	line	227
	mov.b	r3l,#010h
	fjmp	_backchar
	line	228
;MAS.C: 228: }
;MAS.C: 229: }
	line	229
;MAS.C: 230: else SndSpecial(0x00,"JX",'$');
	line	230
l43:
	mov.b	r1l,#024h
	mov.w	r2,#u199
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 231: }
	line	231
l42:
	ret
	align.w
	global	_phases1m
	signat	_phases1m,24
	psect	ltext
;MAS.C: 234: void phases1m(void)
;MAS.C: 235: { if (req)
	line	235
_phases1m:
	jnb	_req,l48
;MAS.C: 236: { req= 0;
	line	236
	clr	_req
;MAS.C: 237: if (SndRequest(0x00,"HY")==0)
	line	237
	mov.w	r2,#u209
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l47
;MAS.C: 238: if (AnswerCheck()==0)
	line	238
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l47
;MAS.C: 239: { waitms(100);
	line	239
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 240: backchar(16);
	line	240
	mov.b	r3l,#010h
	fjmp	_backchar
	line	241
;MAS.C: 241: }
;MAS.C: 242: }
	line	242
;MAS.C: 243: else SndSpecial(0x00,"HY",'$');
	line	243
l48:
	mov.b	r1l,#024h
	mov.w	r2,#u219
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 244: }
	line	244
l47:
	ret
	align.w
	global	_phases2m
	signat	_phases2m,24
	psect	ltext
;MAS.C: 247: void phases2m(void)
;MAS.C: 248: { if (req)
	line	248
_phases2m:
	jnb	_req,l53
;MAS.C: 249: { req= 0;
	line	249
	clr	_req
;MAS.C: 250: if (SndRequest(0x00,"JY")==0)
	line	250
	mov.w	r2,#u229
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l52
;MAS.C: 251: if (AnswerCheck()==0)
	line	251
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l52
;MAS.C: 252: { waitms(100);
	line	252
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 253: backchar(16);
	line	253
	mov.b	r3l,#010h
	fjmp	_backchar
	line	254
;MAS.C: 254: }
;MAS.C: 255: }
	line	255
;MAS.C: 256: else SndSpecial(0x00,"JY",'$');
	line	256
l53:
	mov.b	r1l,#024h
	mov.w	r2,#u239
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 257: }
	line	257
l52:
	ret
	align.w
	global	_daclev1
	signat	_daclev1,24
	global	_save
	global	_SndChar
	signat	_SndChar,12345
	psect	ltext
;MAS.C: 260: void daclev1(void)
;MAS.C: 261: { if (req)
	line	261
_daclev1:
	jnb	_req,l58
;MAS.C: 262: { req= 0;
	line	262
	clr	_req
;MAS.C: 263: if (SndRequest(0x00,"HD")==0)
	line	263
	mov.w	r2,#u249
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l57
;MAS.C: 264: if (AnswerCheck()==0)
	line	264
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l57
;MAS.C: 265: { waitms(20);
	line	265
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 266: backchar(3);
	line	266
	movs.b	r3l,#03h
	fjmp	_backchar
	line	267
;MAS.C: 267: }
;MAS.C: 268: }
	line	268
;MAS.C: 269: else if (save)
	line	269
l58:
	jnb	_save,l62
;MAS.C: 270: { save= 0;
	line	270
	clr	_save
;MAS.C: 271: SndSpecial(0x00, "HD",'$');
	line	271
	mov.b	r1l,#024h
	mov.w	r2,#u259
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 272: }
;MAS.C: 273: else SndChar(0x00,"HD",(unsigned char)Number);
	line	273
l62:
	mov.b	r1l,_Number
	mov.w	r2,#u269
	movs.b	r3l,#00h
	fjmp	_SndChar
;MAS.C: 274: }
	line	274
l57:
	ret
	align.w
	global	_dacoffset1
	signat	_dacoffset1,24
	psect	ltext
;MAS.C: 277: void dacoffset1(void)
;MAS.C: 278: { if (req)
	line	278
_dacoffset1:
	jnb	_req,l65
;MAS.C: 279: { req= 0;
	line	279
	clr	_req
;MAS.C: 280: if (SndRequest(0x00,"HC")==0)
	line	280
	mov.w	r2,#u279
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l64
;MAS.C: 281: if (AnswerCheck()==0)
	line	281
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l64
;MAS.C: 282: { waitms(20);
	line	282
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 283: backchar(3);
	line	283
	movs.b	r3l,#03h
	fjmp	_backchar
	line	284
;MAS.C: 284: }
;MAS.C: 285: }
	line	285
;MAS.C: 286: else if (save)
	line	286
l65:
	jnb	_save,l69
;MAS.C: 287: { save= 0;
	line	287
	clr	_save
;MAS.C: 288: SndSpecial(0x00,"HC",'$');
	line	288
	mov.b	r1l,#024h
	mov.w	r2,#u289
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 289: }
;MAS.C: 290: else SndChar(0x00,"HC",(unsigned char)Number);
	line	290
l69:
	mov.b	r1l,_Number
	mov.w	r2,#u299
	movs.b	r3l,#00h
	fjmp	_SndChar
;MAS.C: 291: }
	line	291
l64:
	ret
	align.w
	global	_daclev2
	signat	_daclev2,24
	psect	ltext
;MAS.C: 294: void daclev2(void)
;MAS.C: 295: { if (req)
	line	295
_daclev2:
	jnb	_req,l72
;MAS.C: 296: { req= 0;
	line	296
	clr	_req
;MAS.C: 297: if (SndRequest(0x00,"JD")==0)
	line	297
	mov.w	r2,#u309
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l71
;MAS.C: 298: if (AnswerCheck()==0)
	line	298
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l71
;MAS.C: 299: { waitms(20);
	line	299
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 300: backchar(3);
	line	300
	movs.b	r3l,#03h
	fjmp	_backchar
	line	301
;MAS.C: 301: }
;MAS.C: 302: }
	line	302
;MAS.C: 303: else if (save)
	line	303
l72:
	jnb	_save,l76
;MAS.C: 304: { save= 0;
	line	304
	clr	_save
;MAS.C: 305: SndSpecial(0x00,"JD",'$');
	line	305
	mov.b	r1l,#024h
	mov.w	r2,#u319
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 306: }
;MAS.C: 307: else SndChar(0x00,"JD",(unsigned char)Number);
	line	307
l76:
	mov.b	r1l,_Number
	mov.w	r2,#u329
	movs.b	r3l,#00h
	fjmp	_SndChar
;MAS.C: 308: }
	line	308
l71:
	ret
	align.w
	global	_dacoffset2
	signat	_dacoffset2,24
	psect	ltext
;MAS.C: 311: void dacoffset2(void)
;MAS.C: 312: { if (req)
	line	312
_dacoffset2:
	jnb	_req,l79
;MAS.C: 313: { req= 0;
	line	313
	clr	_req
;MAS.C: 314: if (SndRequest(0x00,"JC")==0)
	line	314
	mov.w	r2,#u339
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l78
;MAS.C: 315: if (AnswerCheck()==0)
	line	315
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l78
;MAS.C: 316: { waitms(20);
	line	316
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 317: backchar(3);
	line	317
	movs.b	r3l,#03h
	fjmp	_backchar
	line	318
;MAS.C: 318: }
;MAS.C: 319: }
	line	319
;MAS.C: 320: else if (save)
	line	320
l79:
	jnb	_save,l83
;MAS.C: 321: { save= 0;
	line	321
	clr	_save
;MAS.C: 322: SndSpecial(0x00,"JC",'$');
	line	322
	mov.b	r1l,#024h
	mov.w	r2,#u349
	movs.b	r3l,#00h
	fjmp	_SndSpecial
;MAS.C: 323: }
;MAS.C: 324: else SndChar(0x00,"JC",(unsigned char)Number);
	line	324
l83:
	mov.b	r1l,_Number
	mov.w	r2,#u359
	movs.b	r3l,#00h
	fjmp	_SndChar
;MAS.C: 325: }
	line	325
l78:
	ret
	align.w
	global	_reset1
	signat	_reset1,24
	global	_SndVoid
	signat	_SndVoid,8249
	psect	ltext
;MAS.C: 328: void reset1(void)
;MAS.C: 329: { SndVoid(0x00,"HZ");
	line	329
_reset1:
	mov.w	r2,#u369
	movs.b	r3l,#00h
	fjmp	_SndVoid
	align.w
	global	_reset2
	signat	_reset2,24
	psect	ltext
;MAS.C: 333: void reset2(void)
;MAS.C: 334: { SndVoid(0x00,"JZ");
	line	334
_reset2:
	mov.w	r2,#u379
	movs.b	r3l,#00h
	fjmp	_SndVoid
	align.w
	global	_hphsetg
	signat	_hphsetg,24
	psect	ltext
;MAS.C: 338: void hphsetg(void)
;MAS.C: 339: { SndLong(0x15,"GZ",Number);
	line	339
_hphsetg:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u389
	mov.b	r3l,#015h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 340: }
	line	340
	ret
	align.w
	global	_hphsetm
	signat	_hphsetm,24
	psect	ltext
;MAS.C: 343: void hphsetm(void)
;MAS.C: 344: { SndLong(0x15,"GY",Number);
	line	344
_hphsetm:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u399
	mov.b	r3l,#015h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 345: }
	line	345
	ret
	align.w
	global	_gensetg
	signat	_gensetg,24
	psect	ltext
;MAS.C: 348: void gensetg(void)
;MAS.C: 349: {
	line	349
_gensetg:
;MAS.C: 350: if (req) {
	line	350
	jnb	_req,l90
;MAS.C: 351: if (SndRequest(0x15,"GF")==0)
	line	351
	mov.w	r2,#u409
	mov.b	r3l,#015h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l91
;MAS.C: 352: req=0;
	line	352
	clr	_req
l91:
;MAS.C: 353: if (AnswerCheck()==0)
	line	353
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l89
;MAS.C: 354: { waitms(20);
	line	354
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 355: backchar(5);
	line	355
	movs.b	r3l,#05h
	fjmp	_backchar
	line	356
;MAS.C: 356: }
;MAS.C: 357: }
;MAS.C: 358: else
	line	358
l90:
;MAS.C: 359: SndLong(0x15,"GF",Number);
	line	359
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u419
	mov.b	r3l,#015h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 360: }
	line	360
l89:
	ret
	align.w
	global	_gensetm
	signat	_gensetm,24
	psect	ltext
;MAS.C: 363: void gensetm(void)
;MAS.C: 364: {
	line	364
_gensetm:
;MAS.C: 365: if (req) {
	line	365
	jnb	_req,l95
;MAS.C: 366: if (SndRequest(0x15,"GG")==0)
	line	366
	mov.w	r2,#u429
	mov.b	r3l,#015h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l96
;MAS.C: 367: req=0;
	line	367
	clr	_req
l96:
;MAS.C: 368: if (AnswerCheck()==0)
	line	368
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l94
;MAS.C: 369: { waitms(20);
	line	369
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 370: backchar(5);
	line	370
	movs.b	r3l,#05h
	fjmp	_backchar
	line	371
;MAS.C: 371: }
;MAS.C: 372: }
;MAS.C: 373: else
	line	373
l95:
;MAS.C: 374: SndLong(0x15,"GG",Number);
	line	374
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u439
	mov.b	r3l,#015h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 375: }
	line	375
l94:
	ret
	align.w
	global	_gensetd6
	signat	_gensetd6,24
	psect	ltext
;MAS.C: 378: void gensetd6(void)
;MAS.C: 379: { SndLong(0x15,"GD",Number);
	line	379
_gensetd6:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u449
	mov.b	r3l,#015h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 380: }
	line	380
	ret
	align.w
	global	_gensetd5
	signat	_gensetd5,24
	psect	ltext
;MAS.C: 383: void gensetd5(void)
;MAS.C: 384: { SndLong(0x15,"GE",Number);
	line	384
_gensetd5:
	push.w	_Number+02h
	push.w	_Number
	mov.w	r2,#u459
	mov.b	r3l,#015h
	fcall	_SndLong
	adds.w	r7,#04h
;MAS.C: 385: }
	line	385
	ret
	align.w
	global	_hphcalcg
	signat	_hphcalcg,24
	psect	ltext
;MAS.C: 388: void hphcalcg(void)
;MAS.C: 389: {
	line	389
_hphcalcg:
;MAS.C: 390: if (req) {
	line	390
	jnb	_req,l102
;MAS.C: 391: if (SndRequest(0x15,"GA")==0)
	line	391
	mov.w	r2,#u469
	mov.b	r3l,#015h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l103
;MAS.C: 392: req=0;
	line	392
	clr	_req
l103:
;MAS.C: 393: if (AnswerCheck()==0)
	line	393
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l101
;MAS.C: 394: { waitms(20);
	line	394
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 395: backchar(10);
	line	395
	mov.b	r3l,#0Ah
	fjmp	_backchar
	line	396
;MAS.C: 396: }
;MAS.C: 397: }
;MAS.C: 398: else
	line	398
l102:
;MAS.C: 399: SndVoid(0x15,"GA");
	line	399
	mov.w	r2,#u479
	mov.b	r3l,#015h
	fjmp	_SndVoid
;MAS.C: 400: }
	line	400
l101:
	ret
	align.w
	global	_hphcalcm
	signat	_hphcalcm,24
	psect	ltext
;MAS.C: 403: void hphcalcm(void)
;MAS.C: 404: {
	line	404
_hphcalcm:
;MAS.C: 405: if (req) {
	line	405
	jnb	_req,l107
;MAS.C: 406: if (SndRequest(0x15,"GU")==0)
	line	406
	mov.w	r2,#u489
	mov.b	r3l,#015h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l108
;MAS.C: 407: req=0;
	line	407
	clr	_req
l108:
;MAS.C: 408: if (AnswerCheck()==0)
	line	408
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l106
;MAS.C: 409: { waitms(20);
	line	409
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 410: backchar(10);
	line	410
	mov.b	r3l,#0Ah
	fjmp	_backchar
	line	411
;MAS.C: 411: }
;MAS.C: 412: }
;MAS.C: 413: else
	line	413
l107:
;MAS.C: 414: SndVoid(0x15,"GU");
	line	414
	mov.w	r2,#u499
	mov.b	r3l,#015h
	fjmp	_SndVoid
;MAS.C: 415: }
	line	415
l106:
	ret
	align.w
	global	_hphupg
	signat	_hphupg,24
	psect	ltext
;MAS.C: 418: void hphupg(void)
;MAS.C: 419: { SndInt(0x15,"GI",100);
	line	419
_hphupg:
	mov.w	r1,#064h
	mov.w	r2,#u509
	mov.b	r3l,#015h
	fjmp	_SndInt
	align.w
	global	_hphupm
	signat	_hphupm,24
	psect	ltext
;MAS.C: 423: void hphupm(void)
;MAS.C: 424: { SndInt(0x15,"GJ",100);
	line	424
_hphupm:
	mov.w	r1,#064h
	mov.w	r2,#u519
	mov.b	r3l,#015h
	fjmp	_SndInt
	align.w
	psect	code,class=CODE,space=0,align=0
	global	_modetab
_modetab:
;MAS.C: 428: { 0xa1,0xaf,0xad,0xa5,0xa5,0xa4,0xb1,0xa1,0xa1,0xb0,0xa0,0xa0
	db	161
	db	175
	db	173
	db	165
	db	165
	db	164
	db	177
	db	161
	db	161
	db	176
	db	160
;MAS.C: 429: };
	db	160
;MAS.C: 431: void genmode(void)
;MAS.C: 432: { SndChar(0x15,"GM",(unsigned char)Number);
	global	_genmode
	signat	_genmode,24
	global	_ud5buf
	global	_ud5
	psect	ltext
;MAS.C: 427: code unsigned char modetab[]=
	line	432
_genmode:
	mov.b	r1l,_Number
	mov.w	r2,#u529
	mov.b	r3l,#015h
	fcall	_SndChar
;MAS.C: 433: ud5buf= modetab[(unsigned char)Number];
	line	433
	mov.b	r2l,_Number
	movs.b	r2h,#00h
	mov.w	r0,#_modetab&(0+65535)
	mov.w	r1,#_modetab shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	mov.b	_ud5buf,r1l
;MAS.C: 434: ud5= ud5buf;
	line	434
	mov.b	r0l,r1l
	mov.w	r1,#0A001h
	mov.b	[r1],r0l
;MAS.C: 435: }
	line	435
	ret
	align.w
	global	_wrtext1
	signat	_wrtext1,24
	global	_instrptr
	global	_instring
	global	_SndAsc_n
	signat	_SndAsc_n,16441
	psect	ltext
;MAS.C: 438: char instring[11];
;MAS.C: 439: char *instrptr;
;MAS.C: 440: unsigned char instr_index;
;MAS.C: 443: void wrtext1(void)
;MAS.C: 444: { instrptr= instring;
	line	444
_wrtext1:
	mov.w	r1,#_instrptr
	mov.w	[r1],#_instring
;MAS.C: 445: SndAsc_n(0x00, "HW",(unsigned char)Number,instrptr);
	line	445
	mov.w	r1,[r1]
	push	r1
	mov.b	r1l,_Number
	mov.w	r2,#u539
	movs.b	r3l,#00h
	fcall	_SndAsc_n
	adds.w	r7,#02h
;MAS.C: 446: }
	line	446
	ret
	align.w
	global	_rdtext1
	signat	_rdtext1,24
	global	_SndRequest_n
	signat	_SndRequest_n,12345
	psect	ltext
;MAS.C: 449: void rdtext1(void)
;MAS.C: 450: {
	line	450
_rdtext1:
;MAS.C: 451: if (SndRequest_n(0x00,"HR",(unsigned char)Number)==0)
	line	451
	mov.b	r1l,_Number
	mov.w	r2,#u549
	movs.b	r3l,#00h
	fcall	_SndRequest_n
	mov.b	r0l,r0l
	bne	l115
;MAS.C: 452: if (AnswerCheck()==0)
	line	452
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l115
;MAS.C: 453: { waitms(100);
	line	453
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 454: backchar(12);
	line	454
	mov.b	r3l,#0Ch
	fjmp	_backchar
	line	455
;MAS.C: 455: }
;MAS.C: 456: }
	line	456
l115:
	ret
	align.w
	global	_wrtext2
	signat	_wrtext2,24
	psect	ltext
;MAS.C: 459: void wrtext2(void)
;MAS.C: 460: { instrptr= instring;
	line	460
_wrtext2:
	mov.w	r1,#_instrptr
	mov.w	[r1],#_instring
;MAS.C: 461: SndAsc_n(0x00,"JW",(unsigned char)Number,instrptr);
	line	461
	mov.w	r1,[r1]
	push	r1
	mov.b	r1l,_Number
	mov.w	r2,#u559
	movs.b	r3l,#00h
	fcall	_SndAsc_n
	adds.w	r7,#02h
;MAS.C: 462: }
	line	462
	ret
	align.w
	global	_rdtext2
	signat	_rdtext2,24
	psect	ltext
;MAS.C: 465: void rdtext2(void)
;MAS.C: 466: {
	line	466
_rdtext2:
;MAS.C: 467: if (SndRequest_n(0x00,"JR",(unsigned char)Number)==0)
	line	467
	mov.b	r1l,_Number
	mov.w	r2,#u569
	movs.b	r3l,#00h
	fcall	_SndRequest_n
	mov.b	r0l,r0l
	bne	l119
;MAS.C: 468: if (AnswerCheck()==0)
	line	468
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l119
;MAS.C: 469: { waitms(100);
	line	469
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 470: backchar(12);
	line	470
	mov.b	r3l,#0Ch
	fjmp	_backchar
	line	471
;MAS.C: 471: }
;MAS.C: 472: }
	line	472
l119:
	ret
	align.w
	global	_genwrtxt
	signat	_genwrtxt,24
	global	_IIC2_WrStr
	signat	_IIC2_WrStr,12344
	psect	ltext
;MAS.C: 475: void genwrtxt(void)
;MAS.C: 476: { instrptr= instring;
	line	476
_genwrtxt:
	mov.w	r1,#_instrptr
	mov.w	[r1],#_instring
;MAS.C: 477: IIC2_WrStr(0xa8, 12, instrptr);
	line	477
	mov.w	r1,[r1]
	mov.b	r2l,#0Ch
	mov.b	r3l,#-88
	fjmp	_IIC2_WrStr
	align.w
	global	_genrdtxt
	signat	_genrdtxt,24
	global	_IIC2_RdStr
	signat	_IIC2_RdStr,12344
	psect	ltext
;MAS.C: 481: void genrdtxt(void)
;MAS.C: 482: { IIC2_RdStr(0xa8, 12, AnswerBuf+1);
	line	482
_genrdtxt:
	mov.w	r1,#_AnswerBuf+01h
	mov.b	r2l,#0Ch
	mov.b	r3l,#-88
	fcall	_IIC2_RdStr
;MAS.C: 483: AnswerBuf[0]= '"';
	line	483
	mov.b	_AnswerBuf,#022h
;MAS.C: 484: AnswerBuf[9]= '"';
	line	484
	mov.b	_AnswerBuf+09h,#022h
;MAS.C: 485: backchar(12);
	line	485
	mov.b	r3l,#0Ch
	fjmp	_backchar
	align.w
	global	_genstoreg
	signat	_genstoreg,24
	global	_IIC2_WrLongInt
	signat	_IIC2_WrLongInt,16440
	psect	ltext
;MAS.C: 488: void genstoreg(void)
;MAS.C: 489: { IIC2_WrLongInt(0xa8, 0, Number, 12345);
	line	489
_genstoreg:
	mov.w	r0,#03039h
	push	r0
	push.w	_Number+02h
	push.w	_Number
	movs.b	r2l,#00h
	mov.b	r3l,#-88
	fcall	_IIC2_WrLongInt
	adds.w	r7,#06h
;MAS.C: 490: }
	line	490
	ret
	align.w
	global	_genstorem
	signat	_genstorem,24
	psect	ltext
;MAS.C: 492: void genstorem(void)
;MAS.C: 493: { IIC2_WrLongInt(0xa8, 6, Number, 23456);
	line	493
_genstorem:
	mov.w	r0,#05BA0h
	push	r0
	push.w	_Number+02h
	push.w	_Number
	movs.b	r2l,#06h
	mov.b	r3l,#-88
	fcall	_IIC2_WrLongInt
	adds.w	r7,#06h
;MAS.C: 494: }
	line	494
	ret
	align.w
	global	_genrdg
	signat	_genrdg,24
	global	_IIC2_RdLong
	signat	_IIC2_RdLong,8252
	global	_IIC2_RdInt
	signat	_IIC2_RdInt,8250
	psect	ltext
;MAS.C: 496: void genrdg(void)
;MAS.C: 497: { unsigned long a;
	line	497
_genrdg:
	adds.w	r7,#-6
;MAS.C: 498: unsigned int b;
;MAS.C: 500: a= IIC2_RdLong(0xa8, 0);
	line	500
	movs.b	r2l,#00h
	mov.b	r3l,#-88
	fcall	_IIC2_RdLong
	mov.w	[r7+02h],r0
	mov.w	[r7+04h],r1
;MAS.C: 501: b= IIC2_RdInt(0xa8, 4);
	line	501
	movs.b	r2l,#04h
	mov.b	r3l,#-88
	fcall	_IIC2_RdInt
	mov.w	[r7],r0
;MAS.C: 502: }
	line	502
	adds.w	r7,#06h
	ret
	align.w
	global	_genrdm
	signat	_genrdm,24
	psect	ltext
;MAS.C: 504: void genrdm(void)
;MAS.C: 505: { unsigned long a;
	line	505
_genrdm:
	adds.w	r7,#-6
;MAS.C: 506: unsigned int b;
;MAS.C: 508: a= IIC2_RdLong(0xa8, 6);
	line	508
	movs.b	r2l,#06h
	mov.b	r3l,#-88
	fcall	_IIC2_RdLong
	mov.w	[r7+02h],r0
	mov.w	[r7+04h],r1
;MAS.C: 509: b= IIC2_RdInt(0xa8, 10);
	line	509
	mov.b	r2l,#0Ah
	mov.b	r3l,#-88
	fcall	_IIC2_RdInt
	mov.w	[r7],r0
;MAS.C: 510: }
	line	510
	adds.w	r7,#06h
	ret
	align.w
	global	_status1
	signat	_status1,24
	psect	ltext
;MAS.C: 513: void status1(void)
;MAS.C: 514: {
	line	514
_status1:
;MAS.C: 515: if (SndRequest(0x00,"HS")==0)
	line	515
	mov.w	r2,#u579
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l128
;MAS.C: 516: if (AnswerCheck()==0)
	line	516
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l128
;MAS.C: 517: { waitms(20);
	line	517
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 518: backchar(3);
	line	518
	movs.b	r3l,#03h
	fjmp	_backchar
	line	519
;MAS.C: 519: }
;MAS.C: 520: }
	line	520
l128:
	ret
	align.w
	global	_status2
	signat	_status2,24
	psect	ltext
;MAS.C: 523: void status2(void)
;MAS.C: 524: {
	line	524
_status2:
;MAS.C: 525: if (SndRequest(0x00,"JS")==0)
	line	525
	mov.w	r2,#u589
	movs.b	r3l,#00h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l131
;MAS.C: 526: if (AnswerCheck()==0)
	line	526
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l131
;MAS.C: 527: { waitms(20);
	line	527
	mov.w	r3,#014h
	fcall	_waitms
;MAS.C: 528: backchar(3);
	line	528
	movs.b	r3l,#03h
	fjmp	_backchar
	line	529
;MAS.C: 529: }
;MAS.C: 530: }
	line	530
l131:
	ret
	align.w
	global	_optstat
	signat	_optstat,24
	global	_options
	psect	ltext
;MAS.C: 533: void optstat(void)
;MAS.C: 534: { unsigned char i;
	line	534
_optstat:
;MAS.C: 535: unsigned int n;
;MAS.C: 536: if (req)
	line	536
	jnb	_req,l134
;MAS.C: 537: { req= 0;
	line	537
	clr	_req
;MAS.C: 538: n= 1;
; _n allocated to r2
	line	538
	movs.w	r2,#01h
;MAS.C: 539: for (i=0;i<15;i++)
; _i allocated to r1l
	line	539
	movs.b	r1l,#00h
	line	540
l136:
;MAS.C: 540: { if (options & n ) AnswerBuf[i]= '1'; else AnswerBuf[i]= '0';
	mov.w	r0,_options
	and.w	r0,r2
	beq	l139
	mov.b	r0l,r1l
	movs.b	r0h,#00h
	mov.b	[r0+_AnswerBuf],#031h
	jmp	l140
l139:
	mov.b	r0l,r1l
	movs.b	r0h,#00h
	mov.b	[r0+_AnswerBuf],#030h
l140:
;MAS.C: 541: n <<= 1;
	line	541
	asl.w	r2,#01h
;MAS.C: 542: }
	line	539
	adds.b	r1l,#01h
	cmp.b	r1l,#0Fh
	bcs	l136
;MAS.C: 543: backchar(15);
	line	543
	mov.b	r3l,#0Fh
	fjmp	_backchar
	line	544
;MAS.C: 544: }
;MAS.C: 545: }
	line	545
l134:
	ret
	align.w
	global	_dummy
	signat	_dummy,24
	psect	ltext
;MAS.C: 548: void dummy(void)
;MAS.C: 549: { SndVoid(0x15,"DU");
	line	549
_dummy:
	mov.w	r2,#u599
	mov.b	r3l,#015h
	fjmp	_SndVoid
	align.w
	global	_NotFound
	signat	_NotFound,24
	psect	ltext
;MAS.C: 552: void NotFound(void)
;MAS.C: 553: { ;
	line	553
_NotFound:
;MAS.C: 554: }
	line	554
	ret
	align.w
	global	_warntemp
	signat	_warntemp,24
	psect	ltext
;MAS.C: 557: struct kommando {
;MAS.C: 558: char Aa;
;MAS.C: 559: char Bb;
;MAS.C: 560: void (*funktion)(void);
;MAS.C: 561: };
;MAS.C: 563: void warntemp(void) {
	line	563
_warntemp:
;MAS.C: 564: };
	line	564
	ret
	align.w
	global	_kommandotabel
	psect	code
	align.w
_kommandotabel:
;MAS.C: 571: 'G','M',genmode,
	db	71
	db	77
	dw	(_genmode) & 0ffffh
	dw	seg(_genmode)
;MAS.C: 572: 'G','S',get_syncstat,
	db	71
	db	83
	dw	(_get_syncstat) & 0ffffh
	dw	seg(_get_syncstat)
;MAS.C: 573: 'G','Z',hphsetg,
	db	71
	db	90
	dw	(_hphsetg) & 0ffffh
	dw	seg(_hphsetg)
;MAS.C: 574: 'G','Y',hphsetm,
	db	71
	db	89
	dw	(_hphsetm) & 0ffffh
	dw	seg(_hphsetm)
;MAS.C: 575: 'G','A',hphcalcg,
	db	71
	db	65
	dw	(_hphcalcg) & 0ffffh
	dw	seg(_hphcalcg)
;MAS.C: 576: 'G','I',hphupg,
	db	71
	db	73
	dw	(_hphupg) & 0ffffh
	dw	seg(_hphupg)
;MAS.C: 577: 'G','J',hphupm,
	db	71
	db	74
	dw	(_hphupm) & 0ffffh
	dw	seg(_hphupm)
;MAS.C: 578: 'G','F',gensetg,
	db	71
	db	70
	dw	(_gensetg) & 0ffffh
	dw	seg(_gensetg)
;MAS.C: 579: 'G','G',gensetm,
	db	71
	db	71
	dw	(_gensetm) & 0ffffh
	dw	seg(_gensetm)
;MAS.C: 580: 'G','D',gensetd6,
	db	71
	db	68
	dw	(_gensetd6) & 0ffffh
	dw	seg(_gensetd6)
;MAS.C: 581: 'G','E',gensetd5,
	db	71
	db	69
	dw	(_gensetd5) & 0ffffh
	dw	seg(_gensetd5)
;MAS.C: 582: 'G','W',genwrtxt,
	db	71
	db	87
	dw	(_genwrtxt) & 0ffffh
	dw	seg(_genwrtxt)
;MAS.C: 583: 'G','R',genrdtxt,
	db	71
	db	82
	dw	(_genrdtxt) & 0ffffh
	dw	seg(_genrdtxt)
;MAS.C: 584: 'G','V',genstoreg,
	db	71
	db	86
	dw	(_genstoreg) & 0ffffh
	dw	seg(_genstoreg)
;MAS.C: 585: 'G','X',genstorem,
	db	71
	db	88
	dw	(_genstorem) & 0ffffh
	dw	seg(_genstorem)
;MAS.C: 586: 'G','U',hphcalcm,
	db	71
	db	85
	dw	(_hphcalcm) & 0ffffh
	dw	seg(_hphcalcm)
;MAS.C: 587: 'G','K',genrdg,
	db	71
	db	75
	dw	(_genrdg) & 0ffffh
	dw	seg(_genrdg)
;MAS.C: 588: 'G','L',genrdm,
	db	71
	db	76
	dw	(_genrdm) & 0ffffh
	dw	seg(_genrdm)
;MAS.C: 590: 'H','G',gfase1,
	db	72
	db	71
	dw	(_gfase1) & 0ffffh
	dw	seg(_gfase1)
;MAS.C: 591: 'H','K',load1g7,
	db	72
	db	75
	dw	(_load1g7) & 0ffffh
	dw	seg(_load1g7)
;MAS.C: 592: 'H','M',mfase1,
	db	72
	db	77
	dw	(_mfase1) & 0ffffh
	dw	seg(_mfase1)
;MAS.C: 593: 'H','L',load1ms,
	db	72
	db	76
	dw	(_load1ms) & 0ffffh
	dw	seg(_load1ms)
;MAS.C: 594: 'H','O',global1,
	db	72
	db	79
	dw	(_global1) & 0ffffh
	dw	seg(_global1)
;MAS.C: 595: 'H','H',scload1,
	db	72
	db	72
	dw	(_scload1) & 0ffffh
	dw	seg(_scload1)
;MAS.C: 596: 'H','X',phases1,
	db	72
	db	88
	dw	(_phases1) & 0ffffh
	dw	seg(_phases1)
;MAS.C: 597: 'H','Y',phases1m,
	db	72
	db	89
	dw	(_phases1m) & 0ffffh
	dw	seg(_phases1m)
;MAS.C: 598: 'H','D',daclev1,
	db	72
	db	68
	dw	(_daclev1) & 0ffffh
	dw	seg(_daclev1)
;MAS.C: 599: 'H','C',dacoffset1,
	db	72
	db	67
	dw	(_dacoffset1) & 0ffffh
	dw	seg(_dacoffset1)
;MAS.C: 600: 'H','Z',reset1,
	db	72
	db	90
	dw	(_reset1) & 0ffffh
	dw	seg(_reset1)
;MAS.C: 601: 'H','W',wrtext1,
	db	72
	db	87
	dw	(_wrtext1) & 0ffffh
	dw	seg(_wrtext1)
;MAS.C: 602: 'H','R',rdtext1,
	db	72
	db	82
	dw	(_rdtext1) & 0ffffh
	dw	seg(_rdtext1)
;MAS.C: 603: 'H','S',status1,
	db	72
	db	83
	dw	(_status1) & 0ffffh
	dw	seg(_status1)
;MAS.C: 605: 'J','G',gfase2,
	db	74
	db	71
	dw	(_gfase2) & 0ffffh
	dw	seg(_gfase2)
;MAS.C: 606: 'J','K',load2g7,
	db	74
	db	75
	dw	(_load2g7) & 0ffffh
	dw	seg(_load2g7)
;MAS.C: 607: 'J','M',mfase2,
	db	74
	db	77
	dw	(_mfase2) & 0ffffh
	dw	seg(_mfase2)
;MAS.C: 608: 'J','L',load2ms,
	db	74
	db	76
	dw	(_load2ms) & 0ffffh
	dw	seg(_load2ms)
;MAS.C: 609: 'J','O',global2,
	db	74
	db	79
	dw	(_global2) & 0ffffh
	dw	seg(_global2)
;MAS.C: 610: 'J','H',scload2,
	db	74
	db	72
	dw	(_scload2) & 0ffffh
	dw	seg(_scload2)
;MAS.C: 611: 'J','X',phases2,
	db	74
	db	88
	dw	(_phases2) & 0ffffh
	dw	seg(_phases2)
;MAS.C: 612: 'J','Y',phases2m,
	db	74
	db	89
	dw	(_phases2m) & 0ffffh
	dw	seg(_phases2m)
;MAS.C: 613: 'J','D',daclev2,
	db	74
	db	68
	dw	(_daclev2) & 0ffffh
	dw	seg(_daclev2)
;MAS.C: 614: 'J','C',dacoffset2,
	db	74
	db	67
	dw	(_dacoffset2) & 0ffffh
	dw	seg(_dacoffset2)
;MAS.C: 615: 'J','Z',reset2,
	db	74
	db	90
	dw	(_reset2) & 0ffffh
	dw	seg(_reset2)
;MAS.C: 616: 'J','W',wrtext2,
	db	74
	db	87
	dw	(_wrtext2) & 0ffffh
	dw	seg(_wrtext2)
;MAS.C: 617: 'J','R',rdtext2,
	db	74
	db	82
	dw	(_rdtext2) & 0ffffh
	dw	seg(_rdtext2)
;MAS.C: 618: 'J','S',status2,
	db	74
	db	83
	dw	(_status2) & 0ffffh
	dw	seg(_status2)
;MAS.C: 619: 'O','S',optstat,
	db	79
	db	83
	dw	(_optstat) & 0ffffh
	dw	seg(_optstat)
;MAS.C: 622: 'S','C',sch1,
	db	83
	db	67
	dw	(_sch1) & 0ffffh
	dw	seg(_sch1)
;MAS.C: 623: 'S','D',sch2,
	db	83
	db	68
	dw	(_sch2) & 0ffffh
	dw	seg(_sch2)
;MAS.C: 624: 'D','U',dummy,
	db	68
	db	85
	dw	(_dummy) & 0ffffh
	dw	seg(_dummy)
;MAS.C: 625: 'x','x',NotFound
	db	120
	db	120
;MAS.C: 626: };
	dw	(_NotFound) & 0ffffh
	dw	seg(_NotFound)
;MAS.C: 631: CommandExecute(char A,char B) {
;	param _A assigned to r3l on entry
;	param _B assigned to r2l on entry
	global	_CommandExecute
	signat	_CommandExecute,8250
	global	_header1
	global	indir_lr1
	psect	ltext
;MAS.C: 569: code const struct kommando kommandotabel[] = {
	line	631
_CommandExecute:
	push	r6
	adds.w	r7,#-2
;_B stored from r2l
	line	633
	mov.b	[r7],r2l
;_A stored from r3l
	mov.b	[r7+01h],r3l
;MAS.C: 632: unsigned char x;
;MAS.C: 633: for (x = 0; x < (sizeof(kommandotabel)/sizeof(struct kommando)); x++)
; _x allocated to r6l
	movs.b	r6l,#00h
	line	634
l146:
;MAS.C: 634: if (A==kommandotabel[x].Aa)
	mov.b	r0l,r6l
	movs.b	r0h,#00h
	mov.w	r2,r0
	mulu.w	r2,#06h
	mov.w	r0,#_kommandotabel&(0+65535)
	mov.w	r1,#_kommandotabel shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	cmp.b	r1l,[r7+01h]
	bne	l149
;MAS.C: 635: if (B==kommandotabel[x].Bb)
	line	635
	mov.b	r0l,r6l
	movs.b	r0h,#00h
	mov.w	r2,r0
	mulu.w	r2,#06h
	adds.w	r2,#01h
	mov.w	r0,#_kommandotabel&(0+65535)
	mov.w	r1,#_kommandotabel shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	cmp.b	r1l,[r7]
	bne	l149
;MAS.C: 636: {
;MAS.C: 637: header1 = A;
	line	637
	mov.b	r0l,[r7+01h]
	mov.b	_header1,r0l
;MAS.C: 638: kommandotabel[x].funktion();
	line	638
	mov.b	r0l,r6l
	movs.b	r0h,#00h
	mov.w	r2,r0
	mulu.w	r2,#06h
	adds.w	r2,#02h
	mov.w	r0,#_kommandotabel&(0+65535)
	mov.w	r1,#_kommandotabel shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.w	r2,[r0+]
	movc.w	r3,[r0+]
	clr	0218h
	fcall	indir_lr1
;MAS.C: 639: break;
	line	639
	jmp	l145
	line	641
l149:
	line	633
	adds.b	r6l,#01h
	cmp.b	r6l,#033h
	bcs	l146
	line	641
l145:
	adds.w	r7,#02h
	pop	r6
	ret
	align.w
	global	_CharHandler
	signat	_CharHandler,4152
	global	__ctype_
	global	_toupper
	signat	_toupper,4153
	global	_instr_index
	global	_quit
	global	_Number1
	global	llmul
	global	llmul
	psect	ltext
;MAS.C: 646: void CharHandler(char c)
;MAS.C: 647: { static unsigned char MesState;
;	param _c assigned to r3l on entry
	line	647
_CharHandler:
	push	r4
; _c loaded to r4l
	line	649
	mov.b	r4l,r3l
;MAS.C: 648: static char First,Second;
;MAS.C: 649: switch ( MesState)
	line	690
	mov.w	r1,#F900
	mov.b	r0l,[r1]
	or.b	r0l,r0l
	beq	l154
	sub.b	r0l,#01h
	beq	l156
	sub.b	r0l,#01h
	beq	l158
	sub.b	r0l,#01h
	beq	l171
	sub.b	r0l,#01h
	beq	l183
	line	689
;MAS.C: 689: default: MesState = 0; break;
	movs.b	[r1],#00h
	jmp	l151
	line	650
l154:
;MAS.C: 650: { case 0: if(((_ctype_+1)[(unsigned char)(c)]&(0x01|0x02)) || (c=='?'))
	mov.b	r2l,r4l
	movs.b	r2h,#00h
	adds.w	r2,#01h
	mov.w	r0,#__ctype_&(0+65535)
	mov.w	r1,#__ctype_ shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	and.b	r1l,#03h
	bne	u20
	cmp.b	r4l,#03Fh
	bne	l151
u20:
;MAS.C: 651: { First = toupper(c); MesState = 1;} break;
	line	651
	mov.b	r3l,r4l
	sext.b	r3h
	fcall	_toupper
	mov.w	r1,#F901
	mov.b	[r1],r0l
	mov.w	r1,#F900
	movs.b	[r1],#01h
	jmp	l151
	line	652
l156:
;MAS.C: 652: case 1: if(((_ctype_+1)[(unsigned char)(c)]&(0x01|0x02)) || (c=='?'))
	mov.b	r2l,r4l
	movs.b	r2h,#00h
	adds.w	r2,#01h
	mov.w	r0,#__ctype_&(0+65535)
	mov.w	r1,#__ctype_ shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	and.b	r1l,#03h
	bne	u50
	cmp.b	r4l,#03Fh
	bne	l151
u50:
;MAS.C: 653: { Second =toupper(c); MesState = 2; Number = 0;} break;
	line	653
	mov.b	r3l,r4l
	sext.b	r3h
	fcall	_toupper
	mov.w	r1,#F902
	mov.b	[r1],r0l
	mov.w	r1,#F900
	movs.b	[r1],#02h
	movs.w	_Number,#00h
	movs.w	_Number+02h,#00h
	jmp	l151
	line	654
l158:
;MAS.C: 654: case 2: if(((_ctype_+1)[(unsigned char)(c)]&0x04)) Number = 10 * Number + (c-'0');
	mov.b	r2l,r4l
	movs.b	r2h,#00h
	adds.w	r2,#01h
	mov.w	r0,#__ctype_&(0+65535)
	mov.w	r1,#__ctype_ shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	and.b	r1l,#04h
	beq	l159
	mov.w	r2,#0Ah
	movs.w	r3,#00h
	mov.w	r0,_Number
	mov.w	r1,_Number+02h
	fcall	llmul
	push	r0,r1
	mov.b	r0l,r4l
	sext.b	r0h
	sub.w	r0,#030h
	mov.w	r2,r0
	sext.w	r3
	pop	r0,r1
	add.w	r2,r0
	addc.w	r3,r1
	mov.w	_Number,r2
	mov.w	_Number+02h,r3
l159:
;MAS.C: 655: if (c==',') MesState= 3;
	line	655
	cmp.b	r4l,#02Ch
	bne	l160
	mov.w	r1,#F900
	movs.b	[r1],#03h
;MAS.C: 656: else if (c==34)
	line	656
	jmp	l151
l160:
	cmp.b	r4l,#022h
	bne	l162
;MAS.C: 657: { MesState= 4;
	line	657
	mov.w	r1,#F900
	movs.b	[r1],#04h
;MAS.C: 658: instr_index= 0;
	line	658
	mov.w	r1,#_instr_index
	movs.b	[r1],#00h
;MAS.C: 659: }
;MAS.C: 660: else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '%')
	line	660
	jmp	l151
l162:
	cmp.b	r4l,#0Ah
	beq	u160
	cmp.b	r4l,#03Bh
	beq	u160
	cmp.b	r4l,#0Dh
	beq	u160
	cmp.b	r4l,#03Fh
	beq	u160
	cmp.b	r4l,#021h
	beq	u160
	cmp.b	r4l,#025h
	bne	l151
u160:
;MAS.C: 661: { MesState = 0;
	line	661
	mov.w	r1,#F900
	movs.b	[r1],#00h
;MAS.C: 662: if (c == '?') req = 1; else req = 0;
	line	662
	cmp.b	r4l,#03Fh
	bne	l165
	setb	_req
	jmp	l166
l165:
	clr	_req
l166:
;MAS.C: 663: if (c == '!') quit = 1; else quit = 0;
	line	663
	cmp.b	r4l,#021h
	bne	l167
	setb	_quit
	jmp	l168
l167:
	clr	_quit
l168:
;MAS.C: 664: if (c == '%') save = 1; else save = 0;
	line	664
	cmp.b	r4l,#025h
	bne	l169
	setb	_save
	jmp	l170
l169:
	clr	_save
l170:
;MAS.C: 665: CommandExecute(First,Second);
	line	665
	mov.w	r1,#F902
	mov.b	r2l,[r1]
	mov.w	r1,#F901
	mov.b	r3l,[r1]
	jmp	L2
	line	666
;MAS.C: 666: } break;
	line	667
l171:
;MAS.C: 667: case 3: if(((_ctype_+1)[(unsigned char)(c)]&0x04)) Number1 = 10 * Number1 + (c-'0');
	mov.b	r2l,r4l
	movs.b	r2h,#00h
	adds.w	r2,#01h
	mov.w	r0,#__ctype_&(0+65535)
	mov.w	r1,#__ctype_ shr (0+16)
	add.w	r0,r2
	mov.b	0443h,r1l
	setb	0218h
	movc.b	r1l,[r0+]
	clr	0218h
	and.b	r1l,#04h
	beq	l172
	mov.w	r2,#0Ah
	movs.w	r3,#00h
	mov.w	r0,_Number1
	mov.w	r1,_Number1+02h
	fcall	llmul
	push	r0,r1
	mov.b	r0l,r4l
	sext.b	r0h
	sub.w	r0,#030h
	mov.w	r2,r0
	sext.w	r3
	pop	r0,r1
	add.w	r2,r0
	addc.w	r3,r1
	mov.w	_Number1,r2
	mov.w	_Number1+02h,r3
;MAS.C: 668: else if (c==34)
	line	668
	jmp	l151
l172:
	cmp.b	r4l,#022h
	bne	l174
;MAS.C: 669: { MesState= 4;
	line	669
	mov.w	r1,#F900
	movs.b	[r1],#04h
;MAS.C: 670: instr_index= 0;
	line	670
	mov.w	r1,#_instr_index
	movs.b	[r1],#00h
;MAS.C: 671: }
;MAS.C: 672: else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '%')
	line	672
	jmp	l151
l174:
	cmp.b	r4l,#0Ah
	beq	u270
	cmp.b	r4l,#03Bh
	beq	u270
	cmp.b	r4l,#0Dh
	beq	u270
	cmp.b	r4l,#03Fh
	beq	u270
	cmp.b	r4l,#021h
	beq	u270
	cmp.b	r4l,#025h
	bne	l151
u270:
;MAS.C: 673: { MesState = 0;
	line	673
	mov.w	r1,#F900
	movs.b	[r1],#00h
;MAS.C: 674: if (c == '?') req = 1; else req = 0;
	line	674
	cmp.b	r4l,#03Fh
	bne	l177
	setb	_req
	jmp	l178
l177:
	clr	_req
l178:
;MAS.C: 675: if (c == '!') quit = 1; else quit = 0;
	line	675
	cmp.b	r4l,#021h
	bne	l179
	setb	_quit
	jmp	l180
l179:
	clr	_quit
l180:
;MAS.C: 676: if (c == '%') save = 1; else save = 0;
	line	676
	cmp.b	r4l,#025h
	bne	l181
	setb	_save
	jmp	l182
l181:
	clr	_save
l182:
;MAS.C: 677: CommandExecute(First,Second);
	line	677
	mov.w	r1,#F902
	mov.b	r2l,[r1]
	mov.w	r1,#F901
	mov.b	r3l,[r1]
	jmp	L2
	line	678
;MAS.C: 678: } break;
	line	679
l183:
;MAS.C: 679: case 4: if (c==34) instring[instr_index]= 0;
	cmp.b	r4l,#022h
	bne	l184
	mov.w	r1,#_instr_index
	mov.b	r1l,[r1]
	movs.b	r1h,#00h
	mov.b	[r1+_instring],r1h
;MAS.C: 680: else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '%')
	line	680
	jmp	l151
l184:
	cmp.b	r4l,#0Ah
	beq	u380
	cmp.b	r4l,#03Bh
	beq	u380
	cmp.b	r4l,#0Dh
	beq	u380
	cmp.b	r4l,#03Fh
	beq	u380
	cmp.b	r4l,#021h
	beq	u380
	cmp.b	r4l,#025h
	bne	u391
u380:
;MAS.C: 681: { MesState = 0;
	line	681
	mov.w	r1,#F900
	movs.b	[r1],#00h
;MAS.C: 682: if (c == '?') req = 1; else req = 0;
	line	682
	cmp.b	r4l,#03Fh
	bne	l187
	setb	_req
	jmp	l188
u391:
	mov.w	r1,#_instr_index
	mov.b	r1l,[r1]
	movs.b	r1h,#00h
	mov.b	[r1+_instring],r4l
	mov.w	r1,#_instr_index
	adds.b	[r1],#01h
;MAS.C: 688: break;
	line	688
	jmp	l151
l187:
	clr	_req
l188:
;MAS.C: 683: if (c == '!') quit = 1; else quit = 0;
	line	683
	cmp.b	r4l,#021h
	bne	l189
	setb	_quit
	jmp	l190
l189:
	clr	_quit
l190:
;MAS.C: 684: if (c == '%') save = 1; else save = 0;
	line	684
	cmp.b	r4l,#025h
	bne	l191
	setb	_save
	jmp	l192
l191:
	clr	_save
l192:
;MAS.C: 685: CommandExecute(First,Second);
	line	685
	mov.w	r1,#F902
	mov.b	r2l,[r1]
	mov.w	r1,#F901
	mov.b	r3l,[r1]
L2:
	fcall	_CommandExecute
;MAS.C: 686: }
;MAS.C: 687: else instring[instr_index++]= c;
	line	687
;MAS.C: 691: }
	line	691
l151:
	pop	r4
	ret
	align.w
	global	_Timer_int
	signat	_Timer_int,24
	global	_Timer_flag
	psect	ltext
;MAS.C: 700: asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); asm("	global	_" "Timer_int"); asm("	org	" "0x84"); asm("	dw	""0x8000+0x0900+0x1000"", 999f"); asm("	psect	text"); asm("999:	fjmp	_" "Timer_int");
# 700 "MAS.C"
	psect	vectors,ovrld,align=2,class=CODE,space=0 ;#
# 700 "MAS.C"
	global	_Timer_int ;#
# 700 "MAS.C"
	org	0x84 ;#
# 700 "MAS.C"
	dw	0x8000+0x0900+0x1000, 999f ;#
# 700 "MAS.C"
	psect	text ;#
# 700 "MAS.C"
999:	fjmp	_Timer_int ;#
;MAS.C: 701: asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); asm("	global	_" "Serial0_int"); asm("	org	" "0xA0"); asm("	dw	""0x8F00"", 999f"); asm("	psect	text"); asm("999:	fjmp	_" "Serial0_int");
	psect	vectors,ovrld,align=2,class=CODE,space=0 ;#
# 701 "MAS.C"
	global	_Serial0_int ;#
# 701 "MAS.C"
	org	0xA0 ;#
# 701 "MAS.C"
	dw	0x8F00, 999f ;#
# 701 "MAS.C"
	psect	text ;#
# 701 "MAS.C"
999:	fjmp	_Serial0_int ;#
;MAS.C: 702: asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); asm("	global	_" "Serial1_int"); asm("	org	" "0xA8"); asm("	dw	""0x8F00"", 999f"); asm("	psect	text"); asm("999:	fjmp	_" "Serial1_int");
	psect	vectors,ovrld,align=2,class=CODE,space=0 ;#
# 702 "MAS.C"
	global	_Serial1_int ;#
# 702 "MAS.C"
	org	0xA8 ;#
# 702 "MAS.C"
	dw	0x8F00, 999f ;#
# 702 "MAS.C"
	psect	text ;#
# 702 "MAS.C"
999:	fjmp	_Serial1_int ;#
	psect	ltext
;MAS.C: 708: interrupt void Timer_int(void)
;MAS.C: 709: { static near unsigned char ticker;
	line	709
_Timer_int:
;MAS.C: 710: TH0 = -29491200/256/4/3/50;
	line	710
	mov.b	0451h,#040h
;MAS.C: 711: if (++ticker>2)
	line	711
	adds.b	F910,#01h
	cmp.b	F910,#02h
	bl	l195
;MAS.C: 712: { ticker= 0;
	line	712
	movs.b	F910,#00h
;MAS.C: 713: Timer_flag= 1;
	line	713
	setb	_Timer_flag
	line	714
;MAS.C: 714: }
;MAS.C: 715: }
	line	715
l195:
	reti
	align.w
	global	_checkopt
	signat	_checkopt,24
	psect	ltext
;MAS.C: 718: void checkopt(void)
;MAS.C: 719: {
	line	719
_checkopt:
;MAS.C: 721: }
	line	721
	ret
	align.w
	global	_opstart
	signat	_opstart,24
	psect	ltext
;MAS.C: 724: void opstart(void)
;MAS.C: 725: {
	line	725
_opstart:
;MAS.C: 727: }
	line	727
	ret
	align.w
	global	_main
	signat	_main,24
	global	_syssetup
	signat	_syssetup,24
	global	_Serial0_init
	signat	_Serial0_init,24
	global	_Serial1_init
	signat	_Serial1_init,24
	global	_ud1buf
	global	_ud1
	global	_oldtemp
	global	_tempwarn
	global	_slicer
	global	_teller
	global	_reread
	global	_ud4buf
	global	_ud4
	global	_IIC1_Write
	signat	_IIC1_Write,8248
	global	_IIC2_Write
	signat	_IIC2_Write,8248
	global	_grense
	global	_fejl
	global	_cc
	global	_get_char0
	signat	_get_char0,25
	global	_temp
	global	_Rd_iic
	signat	_Rd_iic,4153
	global	_tempcnt
	global	_shotdown
	global	_lev1det
	global	_lev2det
	global	lcmp
	psect	ltext
;MAS.C: 732: void main(void)
;MAS.C: 733: { unsigned char i;
	line	733
_main:
	push	r4
; _testtel allocated to r4l
;MAS.C: 734: unsigned char testtel;
;MAS.C: 739: BCR= 0x02;
	line	739
	movs.b	046Ah,#02h
;MAS.C: 740: syssetup();
	line	740
	fcall	_syssetup
;MAS.C: 743: S0CON= 0x50;
	line	743
	mov.b	0420h,#050h
;MAS.C: 744: S1CON= 0xF2;
	line	744
	mov.b	0424h,#-14
;MAS.C: 747: S1ADDR= 0x10;
	line	747
	mov.b	0465h,#010h
;MAS.C: 748: S1ADEN= 0xCC;
	line	748
	mov.b	0466h,#-52
;MAS.C: 752: TMOD= 0x21;
	line	752
	mov.b	045Ch,#021h
;MAS.C: 753: RTL1= -48;
	line	753
	mov.b	0456h,#-48
;MAS.C: 754: TH0= -255;
	line	754
	movs.b	0451h,#01h
;MAS.C: 755: T2MOD= 0x31;
	line	755
	mov.b	0419h,#031h
;MAS.C: 756: T2CAPH= -1;
	line	756
	movs.b	045Bh,#-1
;MAS.C: 757: T2CAPL= -6;
	line	757
	movs.b	045Ah,#-6
;MAS.C: 759: TH2= -1;
	line	759
	movs.b	0459h,#-1
;MAS.C: 760: TL2= -6;
	line	760
	movs.b	0458h,#-6
;MAS.C: 761: TR2= 1;
	line	761
	setb	02C2h
;MAS.C: 764: IT0 = 1;
	line	764
	setb	0280h
;MAS.C: 765: IT1 = 1;
	line	765
	setb	0282h
;MAS.C: 766: EX0= 0;
	line	766
	clr	0330h
;MAS.C: 767: ET0= 1;
	line	767
	setb	0331h
;MAS.C: 768: ET1= 0;
	line	768
	clr	0333h
;MAS.C: 772: IPA0= 0x90;
	line	772
	mov.b	04A0h,#-112
;MAS.C: 773: IPA4= 0xff;
	line	773
	movs.b	04A4h,#-1
;MAS.C: 774: IPA5= 0xff;
	line	774
	movs.b	04A5h,#-1
;MAS.C: 776: Serial0_init();
	line	776
	fcall	_Serial0_init
;MAS.C: 777: Serial1_init();
	line	777
	fcall	_Serial1_init
;MAS.C: 779: ud1buf |= 15;
	line	779
	or.b	_ud1buf,#0Fh
;MAS.C: 780: ud1= ud1buf;
	line	780
	mov.b	r0l,_ud1buf
	mov.w	r1,#02001h
	mov.b	[r1],r0l
;MAS.C: 782: ERI0 =1;
	line	782
	setb	0338h
;MAS.C: 785: TR0 = 1;
	line	785
	setb	0284h
;MAS.C: 786: TR1= 1;
	line	786
	setb	0286h
;MAS.C: 788: EA = 1 ;
	line	788
	setb	0337h
;MAS.C: 791: { ud1buf &= ~16; ud1= ud1buf;};
	line	791
	and.b	_ud1buf,#-17
	mov.b	r0l,_ud1buf
	mov.w	r1,#02001h
	mov.b	[r1],r0l
;MAS.C: 794: oldtemp = 255;
	line	794
	mov.w	_oldtemp,#0FFh
;MAS.C: 795: tempwarn = 0;
	line	795
	movs.b	_tempwarn,#00h
;MAS.C: 796: slicer=0;
	line	796
	movs.b	_slicer,#00h
;MAS.C: 797: Number=0;
	line	797
	movs.w	_Number,#00h
	movs.w	_Number+02h,#00h
;MAS.C: 798: teller=0;
	line	798
	movs.w	_teller,#00h
;MAS.C: 799: options=0;
	line	799
	movs.w	_options,#00h
;MAS.C: 800: reread=0;
	line	800
	movs.b	_reread,#00h
;MAS.C: 811: { ud4buf ^= 16; ud4= ud4buf;};
	line	811
	xor.b	_ud4buf,#010h
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 813: { ud4buf &= ~0x20; ud4= ud4buf;};
	line	813
	and.b	_ud4buf,#-33
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 814: { ud4buf &= ~0x40; ud4= ud4buf;};
	line	814
	and.b	_ud4buf,#-65
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 815: { ud4buf &= ~0x80; ud4= ud4buf;};
	line	815
	and.b	_ud4buf,#07Fh
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 816: waitms(100);
	line	816
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 818: { ud4buf |= 0x20; ud4= ud4buf;};
	line	818
	or.b	_ud4buf,#020h
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 819: waitms(100);
	line	819
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 821: SndChar(0xf0,"HA",0x11);
	line	821
	mov.b	r1l,#011h
	mov.w	r2,#u609
	mov.b	r3l,#-16
	fcall	_SndChar
;MAS.C: 822: waitms(10);
	line	822
	mov.w	r3,#0Ah
	fcall	_waitms
;MAS.C: 823: i= S1BUF;
; _i allocated to r0h
	line	823
	mov.b	r0h,0464h
;MAS.C: 824: if (i==0x11) options |= 0x0100;
	line	824
	cmp.b	r0h,#011h
	bne	l200
	or.w	_options,#0100h
;MAS.C: 825: else if (i==0x11+64) options |= 0x0800;
	line	825
	jmp	l201
l200:
	cmp.b	r0h,#051h
	bne	l201
	or.w	_options,#0800h
;MAS.C: 827: { ud4buf |= 0x40; ud4= ud4buf;};
	line	827
l201:
	or.b	_ud4buf,#040h
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 828: waitms(100);
	line	828
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 829: SndChar(0xf0,"SW",0x12);
	line	829
	mov.b	r1l,#012h
	mov.w	r2,#u619
	mov.b	r3l,#-16
	fcall	_SndChar
;MAS.C: 830: SndChar(0xf0,"HA",0x12);
	line	830
	mov.b	r1l,#012h
	mov.w	r2,#u629
	mov.b	r3l,#-16
	fcall	_SndChar
;MAS.C: 831: waitms(10);
	line	831
	mov.w	r3,#0Ah
	fcall	_waitms
;MAS.C: 832: i= S1BUF;
	line	832
	mov.b	r0h,0464h
;MAS.C: 833: if (i==0x12) options |= 0x0200;
	line	833
	cmp.b	r0h,#012h
	bne	l203
	or.w	_options,#0200h
;MAS.C: 834: else if (i==0x12+64) options |= 0x1000;
	line	834
	jmp	l204
l203:
	cmp.b	r0h,#052h
	bne	l204
	or.w	_options,#01000h
;MAS.C: 836: { ud4buf |= 0x80; ud4= ud4buf;};
	line	836
l204:
	or.b	_ud4buf,#-128
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 837: waitms(100);
	line	837
	mov.w	r3,#064h
	fcall	_waitms
;MAS.C: 838: SndChar(0xf0,"SW",0x13);
	line	838
	mov.b	r1l,#013h
	mov.w	r2,#u639
	mov.b	r3l,#-16
	fcall	_SndChar
;MAS.C: 839: SndChar(0xf0,"HA",0x13);
	line	839
	mov.b	r1l,#013h
	mov.w	r2,#u649
	mov.b	r3l,#-16
	fcall	_SndChar
;MAS.C: 840: waitms(10);
	line	840
	mov.w	r3,#0Ah
	fcall	_waitms
;MAS.C: 841: i= S1BUF;
	line	841
	mov.b	r0h,0464h
;MAS.C: 842: if (i==0x13) options |= 0x0400;
	line	842
	cmp.b	r0h,#013h
	bne	l206
	or.w	_options,#0400h
;MAS.C: 843: else if (i==0x13+64) options |= 0x2000;
	line	843
	jmp	l207
l206:
	cmp.b	r0h,#053h
	bne	l207
	or.w	_options,#02000h
;MAS.C: 844: if (SndChar(0x16,"AD",0)==0)
	line	844
l207:
	movs.b	r1l,#00h
	mov.w	r2,#u659
	mov.b	r3l,#016h
	fcall	_SndChar
	mov.b	r0l,r0l
	bne	l209
;MAS.C: 845: { options |= 1;
	line	845
	or.w	_options,#01h
	line	846
l209:
;MAS.C: 846: }
;MAS.C: 847: if (SndChar(0x17,"SD",0)==0)
	line	847
	movs.b	r1l,#00h
	mov.w	r2,#u669
	mov.b	r3l,#017h
	fcall	_SndChar
	mov.b	r0l,r0l
	bne	l210
;MAS.C: 848: { options |= 2;
	line	848
	or.w	_options,#02h
	line	849
l210:
;MAS.C: 849: }
;MAS.C: 850: if (IIC1_Write(0xa2,0xff))
	line	850
	movs.b	r2l,#-1
	mov.b	r3l,#-94
	fcall	_IIC1_Write
	bcc	l211
;MAS.C: 851: { options |= 8;
	line	851
	or.w	_options,#08h
	line	852
l211:
;MAS.C: 852: }
;MAS.C: 853: if (IIC2_Write(0x40,0xff))
	line	853
	movs.b	r2l,#-1
	mov.b	r3l,#040h
	fcall	_IIC2_Write
	bcc	l212
;MAS.C: 854: { options |= 16;
	line	854
	or.w	_options,#010h
	line	855
l212:
;MAS.C: 855: }
;MAS.C: 856: if (IIC1_Write(0x48,0xff))
	line	856
	movs.b	r2l,#-1
	mov.b	r3l,#048h
	fcall	_IIC1_Write
	bcc	l213
;MAS.C: 857: { options |= 32;
	line	857
	or.w	_options,#020h
	line	858
l213:
;MAS.C: 858: }
;MAS.C: 859: if (IIC1_Write(0x4e,0xff))
	line	859
	movs.b	r2l,#-1
	mov.b	r3l,#04Eh
	fcall	_IIC1_Write
	bcc	l214
;MAS.C: 860: { options |= 64;
	line	860
	or.w	_options,#040h
	line	861
l214:
;MAS.C: 861: }
;MAS.C: 863: if (SndRequest(0x15,"GS")==0)
	line	863
	mov.w	r2,#u679
	mov.b	r3l,#015h
	fcall	_SndRequest
	mov.b	r0l,r0l
	bne	l215
;MAS.C: 864: if (AnswerCheck()==0)
	line	864
	fcall	_AnswerCheck
	mov.b	r0l,r0l
	bne	l215
;MAS.C: 865: { if (((unsigned char)AnswerBuf[0]*100 + (unsigned char)AnswerBuf[1]*10 + (unsigned char)AnswerBuf[2]) & 64)
	line	865
	mov.b	r0l,_AnswerBuf
	movs.b	r0h,#00h
	mulu.w	r0,#0Ah
	mov.w	r1,#_AnswerBuf+01h
	mov.b	r1l,[r1]
	movs.b	r1h,#00h
	add.w	r1,r0
	mov.w	r2,r1
	mulu.w	r2,#0Ah
	mov.w	r0,#_AnswerBuf+02h
	mov.b	r0l,[r0]
	movs.b	r0h,#00h
	add.w	r0,r2
	and.w	r0,#040h
	beq	l215
;MAS.C: 866: options |= 0x4000;
	line	866
	or.w	_options,#04000h
;MAS.C: 867: }
	line	867
;MAS.C: 870: grense = 104-2;
	line	870
l215:
	mov.w	_grense,#066h
;MAS.C: 871: { ud4buf ^= 16; ud4= ud4buf;};
	line	871
	xor.b	_ud4buf,#010h
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
;MAS.C: 872: fejl = 0;
	line	872
	movs.w	_fejl,#00h
;MAS.C: 875: if (fejl & 8)
	line	875
	mov.w	r0,_fejl
	and.w	r0,#08h
	line	878
;MAS.C: 876: {
;MAS.C: 878: }
;MAS.C: 880: checkopt();
	line	880
	fcall	_checkopt
;MAS.C: 881: opstart();
	line	881
	fcall	_opstart
;MAS.C: 884: ud5buf= 0xa1;
	line	884
	mov.b	_ud5buf,#-95
;MAS.C: 885: ud5= ud5buf;
	line	885
	mov.b	r0l,_ud5buf
	mov.w	r1,#0A001h
	mov.b	[r1],r0l
;MAS.C: 890: while(1)
	line	890
;MAS.C: 891: { while(Timer_flag == 0)
	line	891
	jmp	l222
;MAS.C: 892: { while ((cc=get_char0()) != 0)
	line	892
l226:
;MAS.C: 893: CharHandler(cc);
	line	893
	mov.b	r3l,_cc
	fcall	_CharHandler
l225:
	line	892
	fcall	_get_char0
	mov.b	_cc,r0l
	bne	l226
	line	894
l222:
;MAS.C: 894: }
	line	891
	jnb	_Timer_flag,l225
;MAS.C: 899: Timer_flag = 0;
	line	899
	clr	_Timer_flag
;MAS.C: 901: slicer++; if (slicer > 10) slicer = 1;
	line	901
	adds.b	_slicer,#01h
	cmp.b	_slicer,#0Ah
	bl	l228
	movs.b	_slicer,#01h
l228:
;MAS.C: 904: {
;MAS.C: 905: if (Number==8) Number= 0; else Number= 8;
	line	905
	mov.w	r2,#08h
	movs.w	r3,#00h
	mov.w	r0,_Number
	mov.w	r1,_Number+02h
	fcall	lcmp
	bne	l229
	movs.w	_Number,#00h
	jmp	L4
l229:
	mov.w	_Number,#08h
L4:
	movs.w	_Number+02h,#00h
;MAS.C: 906: genmode();
	line	906
	fcall	_genmode
;MAS.C: 908: }
;MAS.C: 910: if (slicer == 1)
	line	910
	cmp.b	_slicer,#01h
	bne	l231
;MAS.C: 912: { temp = Rd_iic(0x90);
	line	912
	mov.b	r3l,#-112
	fcall	_Rd_iic
	movs.b	r0h,#00h
	mov.w	_temp,r0
;MAS.C: 913: if (temp==0) temp= 255;
	line	913
	mov.w	r0,r0
	bne	l232
	mov.w	_temp,#0FFh
l232:
;MAS.C: 914: if (temp==oldtemp) tempcnt=25;
	line	914
	mov.w	r0,_oldtemp
	cmp.w	_temp,r0
	bne	l233
	mov.b	_tempcnt,#019h
;MAS.C: 915: else if (--tempcnt==0 || oldtemp>temp+1)
	line	915
	jmp	l231
l233:
	sub.b	_tempcnt,#01h
	beq	u410
	mov.w	r0,_temp
	adds.w	r0,#01h
	cmp.w	_oldtemp,r0
	bl	l231
u410:
;MAS.C: 916: { tempcnt = 25;
	line	916
	mov.b	_tempcnt,#019h
;MAS.C: 917: oldtemp = temp;
	line	917
	mov.w	_oldtemp,_temp
;MAS.C: 919: if (temp> 59+2)
	line	919
	cmp.w	_temp,#03Dh
	ble	l236
;MAS.C: 920: { if (tempwarn!=0)
	line	920
	cmp.b	_tempwarn,#00h
	beq	l238
;MAS.C: 921: { tempwarn=0; warntemp(); }
	line	921
	movs.b	_tempwarn,#00h
	jmp	L6
;MAS.C: 922: }
;MAS.C: 923: else if (temp< 52)
	line	923
l236:
	cmp.w	_temp,#034h
	bge	l239
;MAS.C: 924: { shotdown= 1;
	line	924
	setb	_shotdown
;MAS.C: 925: if (tempwarn!=2)
	line	925
	cmp.b	_tempwarn,#02h
	beq	l238
;MAS.C: 926: { tempwarn=2; warntemp(); }
	line	926
	movs.b	_tempwarn,#02h
	jmp	L6
;MAS.C: 927: }
;MAS.C: 928: else if (temp< 59)
	line	928
l239:
	cmp.w	_temp,#03Bh
	bge	l238
;MAS.C: 929: { if (tempwarn!=1)
	line	929
	cmp.b	_tempwarn,#01h
	beq	l238
;MAS.C: 930: { tempwarn=1; warntemp(); }
	line	930
	movs.b	_tempwarn,#01h
L6:
	fcall	_warntemp
;MAS.C: 931: }
	line	931
;MAS.C: 932: if (temp> 52+10)
	line	932
l238:
	cmp.w	_temp,#03Eh
	ble	l244
;MAS.C: 933: if (shotdown)
	line	933
	jnb	_shotdown,l244
;MAS.C: 934: { shotdown=0;
	line	934
	clr	_shotdown
	line	937
;MAS.C: 937: }
;MAS.C: 939: if (temp>grense)
	line	939
l244:
	mov.w	r0,_grense
	cmp.w	_temp,r0
	bl	l246
;MAS.C: 940: { temp=0;
	line	940
	movs.w	_temp,#00h
;MAS.C: 941: if (ud1buf & 16) { ud1buf &= ~16; ud1= ud1buf;};
	line	941
	mov.b	r0l,_ud1buf
	and.b	r0l,#010h
	beq	l247
	and.b	_ud1buf,#-17
	mov.b	r0l,_ud1buf
	mov.w	r1,#02001h
	mov.b	[r1],r0l
l247:
;MAS.C: 942: grense = 104-2;
	line	942
	mov.w	_grense,#066h
;MAS.C: 943: }
;MAS.C: 944: else
	line	944
	jmp	l231
l246:
;MAS.C: 945: { grense = 104;
	line	945
	mov.w	_grense,#068h
;MAS.C: 946: if (!(ud1buf & 16)) { ud1buf |= 16; ud1= ud1buf;};
	line	946
	mov.b	r0l,_ud1buf
	and.b	r0l,#010h
	bne	l231
	or.b	_ud1buf,#010h
	mov.b	r0l,_ud1buf
	mov.w	r1,#02001h
	mov.b	[r1],r0l
	line	947
;MAS.C: 947: }
	line	948
;MAS.C: 948: }
;MAS.C: 949: }
	line	949
l231:
;MAS.C: 951: if (slicer == 2)
	line	951
	cmp.b	_slicer,#02h
	bne	l250
;MAS.C: 953: {
;MAS.C: 954: lev1det= Rd_iic(0x44);
	line	954
	mov.b	r3l,#044h
	fcall	_Rd_iic
	movs.b	r0h,#00h
	mov.w	_lev1det,r0
;MAS.C: 955: lev2det= Rd_iic(0x46);
	line	955
	mov.b	r3l,#046h
	fcall	_Rd_iic
	movs.b	r0h,#00h
	mov.w	_lev2det,r0
	line	956
l250:
;MAS.C: 956: }
;MAS.C: 958: if (slicer == 3)
	line	958
	cmp.b	_slicer,#03h
	bne	l251
;MAS.C: 960: { { ud4buf ^= 16; ud4= ud4buf;};
	line	960
	xor.b	_ud4buf,#010h
	mov.b	r0l,_ud4buf
	mov.w	r1,#08001h
	mov.b	[r1],r0l
	line	961
l251:
;MAS.C: 961: }
;MAS.C: 963: if (slicer == 4)
	line	963
	cmp.b	_slicer,#04h
	line	966
;MAS.C: 965: {
;MAS.C: 966: }
;MAS.C: 968: if (slicer == 5)
	line	968
	cmp.b	_slicer,#05h
	line	971
;MAS.C: 970: {
;MAS.C: 971: }
;MAS.C: 973: if (slicer == 6)
	line	973
	cmp.b	_slicer,#06h
	line	976
;MAS.C: 975: {
;MAS.C: 976: }
;MAS.C: 978: if (slicer == 7)
	line	978
	cmp.b	_slicer,#07h
	line	981
;MAS.C: 980: {
;MAS.C: 981: }
;MAS.C: 983: if (slicer == 8)
	line	983
	cmp.b	_slicer,#08h
	line	987
;MAS.C: 985: {
;MAS.C: 987: }
;MAS.C: 989: if (slicer == 9)
	line	989
	cmp.b	_slicer,#09h
	bne	l222
;MAS.C: 991: {
;MAS.C: 992: testtel++;
	line	992
	adds.b	r4l,#01h
;MAS.C: 995: if ((testtel++ & 15)== 15) ;
	line	995
	mov.b	r0l,r4l
	adds.b	r4l,#01h
	and.b	r0l,#0Fh
	cmp.b	r0l,#0Fh
;MAS.C: 1006: }
	line	1006
;MAS.C: 1008: }
	line	1008
	line	890
	jmp	l222
	align.w
	psect	strings,class=DATA,space=1,align=0
u379:
	db	74,90,0
u369:
	db	72,90,0
u389:
	db	71,90,0
u229:
u239:
	db	74,89,0
u219:
u209:
	db	72,89,0
u399:
	db	71,89,0
u199:
u189:
	db	74,88,0
u169:
u179:
	db	72,88,0
u639:
u619:
	db	83,87,0
u559:
	db	74,87,0
u539:
	db	72,87,0
u499:
u489:
	db	71,85,0
u599:
	db	68,85,0
u159:
u589:
	db	74,83,0
u149:
u579:
	db	72,83,0
u19:
u679:
	db	71,83,0
u569:
	db	74,82,0
u549:
	db	72,82,0
u119:
	db	74,79,0
u109:
	db	72,79,0
u79:
	db	74,77,0
u69:
	db	72,77,0
u529:
	db	71,77,0
u99:
	db	74,76,0
u89:
	db	72,76,0
u59:
	db	74,75,0
u49:
	db	72,75,0
u519:
	db	71,74,0
u509:
	db	71,73,0
u139:
	db	74,72,0
u129:
	db	72,72,0
u39:
	db	74,71,0
u29:
	db	72,71,0
u429:
u439:
	db	71,71,0
u409:
u419:
	db	71,70,0
u459:
	db	71,69,0
u669:
	db	83,68,0
u329:
u319:
u309:
	db	74,68,0
u269:
u249:
u259:
	db	72,68,0
u449:
	db	71,68,0
u659:
	db	65,68,0
u349:
u359:
u339:
	db	74,67,0
u299:
u289:
u279:
	db	72,67,0
u609:
u649:
u629:
	db	72,65,0
u469:
u479:
	db	71,65,0
	psect	rbss,class=DATA,space=1,align=0
	global	_Number
	align.w
_Number:
	ds	4
	global	_Number1
	align.w
_Number1:
	ds	4
	global	_fejl
	align.w
_fejl:
	ds	2
	global	_grense
	align.w
_grense:
	ds	2
	psect	bss,class=DATA,space=1,align=0
	global	_instrptr
	align.w
_instrptr:
	ds	2
	global	_lev1det
	psect	rbss
	align.w
_lev1det:
	ds	2
	global	_lev2det
	align.w
_lev2det:
	ds	2
	global	_oldtemp
	align.w
_oldtemp:
	ds	2
	global	_options
	align.w
_options:
	ds	2
	global	_teller
	align.w
_teller:
	ds	2
	global	_temp
	align.w
_temp:
	ds	2
	psect	bss
F900:
	ds	1
F901:
	ds	1
F902:
	ds	1
	psect	rbss
F910:
	ds	1
	psect	rbit,bit,class=BITSEG,space=1,align=0
	global	_Acknowledge
_Acknowledge:
	ds	1
	global	_Timer_flag
_Timer_flag:
	ds	1
	global	_bufbusy
_bufbusy:
	ds	1
	global	_c
	psect	rbss
_c:
	ds	1
	global	_cc
_cc:
	ds	1
	global	_genbusy
	psect	rbit
_genbusy:
	ds	1
	global	_header1
	psect	rbss
_header1:
	ds	1
	global	_ind1
_ind1	equ	8193
	global	_ind2
_ind2	equ	16385
	global	_instr_index
	psect	bss
_instr_index:
	ds	1
	global	_instring
_instring:
	ds	11
	global	_myaddress
	psect	rbss
_myaddress:
	ds	1
	global	_pgadata
_pgadata	equ	131072
	global	_quit
	psect	rbit
_quit:
	ds	1
	global	_req
_req:
	ds	1
	global	_req_ready
_req_ready:
	ds	1
	global	_reread
	psect	rbss
_reread:
	ds	1
	global	_save
	psect	rbit
_save:
	ds	1
	global	_shotdown
_shotdown:
	ds	1
	global	_slicer
	psect	rbss
_slicer:
	ds	1
	global	_speed
_speed:
	ds	1
	global	_syncstatus
_syncstatus:
	ds	1
	global	_tempcnt
_tempcnt:
	ds	1
	global	_tempwarn
_tempwarn:
	ds	1
	global	_ud1
_ud1	equ	8193
	global	_ud1buf
_ud1buf:
	ds	1
	global	_ud2
_ud2	equ	16385
	global	_ud3
_ud3	equ	24577
	global	_ud4
_ud4	equ	32769
	global	_ud4buf
_ud4buf:
	ds	1
	global	_ud5
_ud5	equ	40961
	global	_ud5buf
_ud5buf:
	ds	1
	end
