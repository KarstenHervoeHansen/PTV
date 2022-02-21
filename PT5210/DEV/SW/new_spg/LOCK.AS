	psect	ltext,class=CODE,space=0,align=2
	global	_lock
	signat	_lock,24
	global	_freq_sel
	global	_genlocken
	global	_verr
	global	_d1_sel
	global	_burstfase
	signat	_burstfase,24
	global	_blackav
	global	_g_sel
	global	_syncav
	global	_slowlock
	global	_bulocken
	global	_buamav
	global	_buav
	global	_lockdet
	global	_errpre
	global	_outs
	global	_burstlock
	signat	_burstlock,24
	global	_hlock
	signat	_hlock,24
	global	_intern
	signat	_intern,24
	global	_level
	global	_outport
	global	_dolock
	global	_freqfase
	signat	_freqfase,24
	psect	ltext
	file	"LOCK.C"
	line	36
_lock:
	push.b	0403h
	movs.b	0403h,#00h
	push.b	0443h
	push.b	0442h
	and.b	042Ah,#-9
;LOCK.C: 39: if (!freq_sel)
	line	39
	jb	_freq_sel,l2
;LOCK.C: 40: { if (!genlocken)
	line	40
;LOCK.C: 41: goto SETINT;
	line	41
	jnb	_genlocken,l4
;LOCK.C: 42: if (verr>0)
	line	42
	cmp.b	_verr,#00h
;LOCK.C: 43: goto SETINT;
	line	43
	bne	l4
;LOCK.C: 44: if (d1_sel)
	line	44
;LOCK.C: 45: goto BULOD1;
	line	45
	jb	_d1_sel,l7
;LOCK.C: 46: burstfase();
	line	46
	fcall	_burstfase
;LOCK.C: 47: if (blackav<=24)
	line	47
	cmp.b	_blackav,#018h
;LOCK.C: 48: goto SETINT;
	line	48
	bl	l4
;LOCK.C: 49: if (g_sel) { if (!syncav && blackav<=36) goto SETINT; }
	line	49
	jnb	_g_sel,l9
	jb	_syncav,l11
	cmp.b	_blackav,#024h
L2:
	bg	l11
	line	100
l4:
;LOCK.C: 100: SETINT:
;LOCK.C: 105: outs= 0x04;
	line	105
	mov.w	r1,#_outs
	movs.b	[r1],#04h
L1:
;LOCK.C: 106: intern();
	line	106
	fcall	_intern
	line	108
l25:
;LOCK.C: 108: OUTLOCK:
;LOCK.C: 109: if (level) outport= outs | 0x20;
	line	109
	jnb	_level,l28
	mov.w	r1,#_outs
	mov.b	r0l,[r1]
	or.b	r0l,#020h
	mov.w	r1,#_outport
	mov.b	[r1],r0l
;LOCK.C: 110: else outport= outs & ~0x20;
	line	110
	jmp	L4
	line	53
l11:
;LOCK.C: 54: goto SETSLOW;
	line	54
	jb	_slowlock,l14
;LOCK.C: 55: if (!bulocken)
	line	55
;LOCK.C: 56: goto NOBU;
	line	56
	jnb	_bulocken,l16
;LOCK.C: 57: if (g_sel)
	line	57
	jnb	_g_sel,l17
;LOCK.C: 58: { if (buamav<=50) goto NOBU;
	line	58
	cmp.b	_buamav,#032h
	bl	l16
;LOCK.C: 59: if (!buav && buamav<=70) goto NOBU;
	line	59
	jb	_buav,l20
	cmp.b	_buamav,#046h
L3:
	bg	l20
	line	82
l16:
;LOCK.C: 82: NOBU:
;LOCK.C: 83: if (lockdet=1)
	line	83
	setb	_lockdet
	jnb	_lockdet,l26
;LOCK.C: 84: { errpre= 3;
	line	84
	movs.b	_errpre,#03h
;LOCK.C: 85: outs= 0x83;
	line	85
	mov.w	r1,#_outs
	mov.b	[r1],#-125
;LOCK.C: 86: }
;LOCK.C: 87: else
	line	87
	jmp	l27
;LOCK.C: 50: else if (!syncav && blackav<=34) goto SETINT;
	line	50
l9:
	jb	_syncav,l11
	cmp.b	_blackav,#022h
	jmp	L2
	line	64
l20:
;LOCK.C: 66: if (lockdet)
	line	66
	jnb	_lockdet,l23
;LOCK.C: 67: { errpre= 3;
	line	67
	movs.b	_errpre,#03h
;LOCK.C: 68: outs= 0x8b;
	line	68
	mov.w	r1,#_outs
	mov.b	[r1],#-117
;LOCK.C: 69: }
;LOCK.C: 70: else
	line	70
	jmp	l24
;LOCK.C: 60: }
;LOCK.C: 61: else
	line	61
l17:
;LOCK.C: 62: { if (buamav<=45) goto NOBU;
	line	62
	cmp.b	_buamav,#02Dh
	bl	l16
;LOCK.C: 63: if (!buav && buamav<=65) goto NOBU;
	line	63
	jb	_buav,l20
	cmp.b	_buamav,#041h
	jmp	L3
l23:
;LOCK.C: 71: { errpre= 1;
	line	71
	movs.b	_errpre,#01h
;LOCK.C: 72: outs= 0x09;
	line	72
	mov.w	r1,#_outs
	mov.b	[r1],#09h
	line	73
l24:
;LOCK.C: 73: }
;LOCK.C: 75: burstlock();
	line	75
	fcall	_burstlock
;LOCK.C: 76: goto OUTLOCK;
	line	76
	jmp	l25
	line	78
l7:
;LOCK.C: 78: BULOD1:
;LOCK.C: 79: outs= 0x8b;
	line	79
	mov.w	r1,#_outs
	mov.b	[r1],#-117
;LOCK.C: 80: goto OUTLOCK;
	line	80
	jmp	l25
l26:
;LOCK.C: 88: { errpre= 1;
	line	88
	movs.b	_errpre,#01h
;LOCK.C: 89: outs= 0x01;
	line	89
	mov.w	r1,#_outs
	movs.b	[r1],#01h
	line	90
l27:
;LOCK.C: 90: }
;LOCK.C: 92: hlock();
	line	92
	fcall	_hlock
;LOCK.C: 93: goto OUTLOCK;
	line	93
	jmp	l25
	line	95
l14:
;LOCK.C: 95: SETSLOW:
;LOCK.C: 96: outs= 0xa5;
	line	96
	mov.w	r1,#_outs
	mov.b	[r1],#-91
	jmp	L1
;LOCK.C: 97: intern();
	line	97
;LOCK.C: 98: goto OUTLOCK;
	line	98
l28:
	mov.w	r1,#_outs
	mov.b	r0l,[r1]
	and.b	r0l,#-33
	mov.w	r1,#_outport
	mov.b	[r1],r0l
	jmp	L4
;LOCK.C: 111: dolock= 0;
	line	111
;LOCK.C: 112: }
;LOCK.C: 114: else
	line	114
l2:
;LOCK.C: 115: { if (!genlocken)
	line	115
;LOCK.C: 116: goto SETINTF;
	line	116
	jnb	_genlocken,l32
;LOCK.C: 119: freqfase();
	line	119
	fcall	_freqfase
;LOCK.C: 120: if (P3_0)
	line	120
;LOCK.C: 121: goto SETINTF;
	line	121
	jb	0398h,l32
;LOCK.C: 124: outs= 0x8b;
	line	124
	mov.w	r1,#_outs
	mov.b	[r1],#-117
;LOCK.C: 125: burstlock();
	line	125
	fcall	_burstlock
;LOCK.C: 126: goto OUTLOCKF;
	line	126
	jmp	l34
	line	128
l32:
;LOCK.C: 128: SETINTF:
;LOCK.C: 129: outs= 0x04;
	line	129
	mov.w	r1,#_outs
	movs.b	[r1],#04h
;LOCK.C: 130: intern();
	line	130
	fcall	_intern
	line	132
l34:
;LOCK.C: 132: OUTLOCKF:
;LOCK.C: 133: outport= outs;
	line	133
	mov.w	r1,#_outs
	mov.b	r0l,[r1]
	mov.w	r1,#_outport
	mov.b	[r1],r0l
L4:
;LOCK.C: 134: dolock= 0;
	line	134
	clr	_dolock
	line	135
;LOCK.C: 135: }
;LOCK.C: 138: }
	line	138
	pop.b	0442h
	pop.b	0443h
	pop.b	0403h
	reti
	align.w
	global	_hpos
	global	_dual
	global	_black
	global	_buph
	global	_tableph
	global	_tablepyt
	global	_fasekor
	global	_fasepos
	psect	ltext
;LOCK.C: 144: void burstfase(void)
;LOCK.C: 145: { unsigned char s1,s2,s3,s4;
	line	145
_burstfase:
	push	r4,r5,r6
	adds.w	r7,#-2
;LOCK.C: 146: unsigned char y,x;
;LOCK.C: 147: unsigned int i;
;LOCK.C: 148: unsigned char z;
;LOCK.C: 149: static bit yneg;
;LOCK.C: 151: if (g_sel)
	line	151
	jnb	_g_sel,l36
;LOCK.C: 152: { i= hpos + 91;
; _i allocated to r4
	line	152
	mov.w	r4,_hpos
	add.w	r4,#05Bh
;LOCK.C: 153: if (i>864) i -= 864;
	line	153
	cmp.w	r4,#0360h
	bl	l38
	sub.w	r4,#0360h
;LOCK.C: 154: }
;LOCK.C: 155: else
	line	155
	jmp	l38
l36:
;LOCK.C: 156: { i= hpos + 88;
	line	156
	mov.w	r4,_hpos
	add.w	r4,#058h
;LOCK.C: 157: if (i>858) i -= 858;
	line	157
	cmp.w	r4,#035Ah
	bl	l38
	sub.w	r4,#035Ah
;LOCK.C: 158: }
	line	158
l38:
;LOCK.C: 160: if (i>255) i -= 4;
	line	160
	cmp.w	r4,#0FFh
	bl	l40
	sub.w	r4,#04h
l40:
;LOCK.C: 163: s1= dual[i];
; _s1 allocated to r5h
	line	163
	mov.w	r0,r4
	add.w	r0,_dual
	mov.b	r5h,[r0]
;LOCK.C: 164: s2= dual[i+1];
; _s2 allocated to r6l
	line	164
	mov.w	r0,r4
	add.w	r0,_dual
	mov.b	r6l,[r0+01h]
;LOCK.C: 165: s3= dual[i+2];
; _s3 allocated to r6h
	line	165
	mov.w	r0,r4
	add.w	r0,_dual
	mov.b	r6h,[r0+02h]
;LOCK.C: 166: s4= dual[i+3];
; _s4 allocated to r5l
	line	166
	mov.w	r0,r4
	add.w	r0,_dual
	mov.b	r5l,[r0+03h]
;LOCK.C: 167: if (g_sel) black= (s1+s2+s3) /3;
	line	167
	jnb	_g_sel,l41
	mov.b	r0l,r6l
	movs.b	r0h,#00h
	mov.b	r2l,r5h
	mov.b	r2h,r0h
	mov.b	r1l,r6h
	mov.b	r1h,r0h
	add.w	r1,r2
	add.w	r1,r0
	mov.w	r0,r1
	sext.w	r1
	div.d	r0,#03h
	jmp	L5
;LOCK.C: 168: else black= (s1+s2+s3+s4) /4;
	line	168
l41:
	mov.b	r0l,r5l
	movs.b	r0h,#00h
	mov.b	r1l,r6h
	mov.b	r1h,r0h
	mov.b	r3l,r6l
	mov.b	r3h,r0h
	mov.b	r2l,r5h
	mov.b	r2h,r0h
	add.w	r2,r3
	add.w	r2,r1
	add.w	r2,r0
	mov.w	r0,r2
	sext.w	r1
	div.d	r0,#04h
L5:
	mov.b	_black,r0l
;LOCK.C: 170: if (black > blackav)
	line	170
	mov.b	r0l,_blackav
	cmp.b	_black,r0l
	bl	l43
;LOCK.C: 171: { if (blackav!=255)
	line	171
	cmp.b	_blackav,#-1
	beq	l45
;LOCK.C: 172: blackav += 1;
	line	172
	adds.b	_blackav,#01h
;LOCK.C: 173: }
;LOCK.C: 174: else if (blackav!=0)
	line	174
	jmp	l45
l43:
	cmp.b	_blackav,#00h
	beq	l45
;LOCK.C: 175: blackav -= 1;
	line	175
	sub.b	_blackav,#01h
;LOCK.C: 178: if (s4>s2)
	line	178
l45:
;_s4 stored from r5l
	mov.b	[r7],r5l
	cmp.b	r5l,r6l
	bl	l47
;LOCK.C: 179: { y= s4-s2;
; _y allocated to r5l
	line	179
	mov.b	r5l,[r7]
	sub.b	r5l,r6l
;LOCK.C: 180: yneg= 1;
	line	180
	setb	F300
;LOCK.C: 181: }
;LOCK.C: 182: else
	line	182
	jmp	l48
l47:
;LOCK.C: 183: { y= s2-s4;
	line	183
	mov.b	r5l,r6l
	sub.b	r5l,[r7]
;LOCK.C: 184: yneg= 0;
	line	184
	clr	F300
	line	185
l48:
;LOCK.C: 185: }
;LOCK.C: 187: x= s1 - s3;
; _x allocated to r6l
	line	187
	mov.b	r6l,r5h
	sub.b	r6l,r6h
;LOCK.C: 189: if (s3>s1)
	line	189
	cmp.b	r6h,r5h
	bl	l49
;LOCK.C: 191: { x= s3-s1;
	line	191
	mov.b	r6l,r6h
	sub.b	r6l,r5h
;LOCK.C: 192: if (!yneg)
	line	192
	jb	F300,l50
;LOCK.C: 193: { buph= tableph[256 * x + y] + 0x0100;
	line	193
	mov.b	r1h,r6l
	movs.b	r1l,#00h
	mov.b	r0l,r5l
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	movs.b	r0h,#00h
	add.w	r0,#0100h
	mov.w	_buph,r0
;LOCK.C: 194: z= tablepyt[256 * (x/2 + 128) + y/2];
; _z allocated to r5h
	line	194
	mov.b	r0l,r6l
	lsr.b	r0l,#01h
	mov.b	r1h,r0l
	movs.b	r1l,#00h
	mov.b	r0l,r5l
	lsr.b	r0l,#01h
	mov.b	r0h,r1l
	add.w	r0,r1
	add.w	r0,#-32768
	mov.w	r1,#_tablepyt
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	movc.b	r5h,[r2+]
;LOCK.C: 195: }
;LOCK.C: 196: else
	line	196
	jmp	l52
l50:
;LOCK.C: 197: { buph= tableph[256 * y + x] + 0xfe00;
	line	197
	mov.b	r1h,r5l
	movs.b	r1l,#00h
	mov.b	r0l,r6l
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	movs.b	r0h,#00h
	add.w	r0,#-512
	mov.w	_buph,r0
;LOCK.C: 198: z= tablepyt[256 * (x/2 + 128) + y/2 + 128];
	line	198
	mov.b	r0l,r6l
	lsr.b	r0l,#01h
	mov.b	r1h,r0l
	movs.b	r1l,#00h
	mov.b	r0l,r5l
	lsr.b	r0l,#01h
	mov.b	r0h,r1l
	add.w	r0,r1
	add.w	r0,#-32640
	mov.w	r1,#_tablepyt
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	movc.b	r5h,[r2+]
	line	199
;LOCK.C: 199: }
;LOCK.C: 201: }
;LOCK.C: 202: else if (x==0)
	line	202
	jmp	l52
l49:
	or.b	r6l,r6l
	bne	l53
;LOCK.C: 203: { if (!yneg)
	line	203
	jb	F300,l54
;LOCK.C: 204: { buph= 0x4000>>6;
	line	204
	mov.w	_buph,#0100h
;LOCK.C: 205: z= tablepyt[y/2];
	line	205
	mov.b	r0l,r5l
	lsr.b	r0l,#01h
	movs.b	r0h,#00h
	mov.w	r1,#_tablepyt
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r5h,[r2+]
;LOCK.C: 206: }
;LOCK.C: 207: else
	line	207
	jmp	l52
l54:
;LOCK.C: 208: { buph= 0xc000>>6;
	line	208
	mov.w	_buph,#0300h
;LOCK.C: 209: z= tablepyt[y/2 + 128];
	line	209
	mov.b	r0l,r5l
	lsr.b	r0l,#01h
	movs.b	r0h,#00h
	add.w	r0,#080h
	mov.w	r1,#_tablepyt
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r5h,[r2+]
	line	210
;LOCK.C: 210: }
;LOCK.C: 211: }
;LOCK.C: 212: else if (yneg)
	line	212
	jmp	l52
l53:
	jnb	F300,l57
;LOCK.C: 213: { buph= tableph[256 * x + y] + 0xff00;
	line	213
	mov.b	r1h,r6l
	movs.b	r1l,#00h
	mov.b	r0l,r5l
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	movs.b	r0h,#00h
	add.w	r0,#-256
	mov.w	_buph,r0
;LOCK.C: 214: z= tablepyt[128 * x + y/2 + 128];
	line	214
	mov.b	r1l,r6l
	movs.b	r1h,#00h
	asl.w	r1,#07h
	mov.b	r0l,r5l
	lsr.b	r0l,#01h
	movs.b	r0h,#00h
	add.w	r0,r1
	add.w	r0,#080h
	mov.w	r1,#_tablepyt
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	movc.b	r5h,[r2+]
;LOCK.C: 215: }
;LOCK.C: 216: else
	line	216
	jmp	l52
l57:
;LOCK.C: 217: { buph= tableph[256 * y + x];
	line	217
	mov.b	r1h,r5l
	movs.b	r1l,#00h
	mov.b	r0l,r6l
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	movs.b	r0h,#00h
	mov.w	_buph,r0
;LOCK.C: 218: z= tablepyt[128 * x + y/2];
	line	218
	mov.b	r1l,r6l
	movs.b	r1h,#00h
	asl.w	r1,#07h
	mov.b	r0l,r5l
	lsr.b	r0l,#01h
	movs.b	r0h,#00h
	add.w	r0,r1
	mov.w	r1,#_tablepyt
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	movc.b	r5h,[r2+]
	line	219
;LOCK.C: 219: }
l52:
;LOCK.C: 239: buph= fasekor[buph & 0x3ff];
	line	239
	mov.w	r0,_buph
	and.w	r0,#03FFh
	asl.w	r0,#01h
	mov.w	r1,#_fasekor
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.w	r0,[r2+]
	clr	0218h
	mov.w	_buph,r0
;LOCK.C: 253: buph= buph + fasepos[i];
	line	253
	mov.w	r0,r4
	asl.w	r0,#01h
	mov.w	r1,#_fasepos
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.w	r0,[r2+]
	add.w	r0,_buph
	mov.w	_buph,r0
;LOCK.C: 269: if (z>buamav)
	line	269
	cmp.b	r5h,_buamav
	bl	l59
;LOCK.C: 270: { if (buamav!=255)
	line	270
	cmp.b	_buamav,#-1
	beq	l35
;LOCK.C: 271: buamav += 1;
	line	271
	adds.b	_buamav,#01h
;LOCK.C: 272: }
;LOCK.C: 273: else if (buamav!=0)
	line	273
	jmp	l35
l59:
	cmp.b	_buamav,#00h
	beq	l35
;LOCK.C: 274: buamav -= 1;
	line	274
	sub.b	_buamav,#01h
;LOCK.C: 276: }
	line	276
l35:
	adds.w	r7,#02h
	pop	r4,r5,r6
	ret
	align.w
	psect	ltext
;LOCK.C: 281: void freqfase(void)
;LOCK.C: 282: { unsigned char s1,s2,s3,s4;
	line	282
_freqfase:
	push	r4
;LOCK.C: 283: unsigned char y,x;
;LOCK.C: 284: unsigned int i;
;LOCK.C: 286: static bit yneg;
;LOCK.C: 288: i= 0;
; _i allocated to r0
	line	288
;LOCK.C: 290: s1= dual[i];
; _s1 allocated to r1l
	line	290
	mov.w	r0,_dual
	mov.b	r1l,[r0]
;LOCK.C: 291: s2= dual[i+1];
; _s2 allocated to r2l
	line	291
	mov.b	r2l,[r0+01h]
;LOCK.C: 292: s3= dual[i+2];
; _s3 allocated to r2h
	line	292
	mov.b	r2h,[r0+02h]
;LOCK.C: 293: s4= dual[i+3];
; _s4 allocated to r1h
	line	293
	mov.b	r1h,[r0+03h]
;LOCK.C: 296: if (s4>s2)
	line	296
	cmp.b	r1h,r2l
	bl	l64
;LOCK.C: 297: { y= s4-s2;
; _y allocated to r4l
	line	297
	mov.b	r4l,r1h
	sub.b	r4l,r2l
;LOCK.C: 298: yneg= 1;
	line	298
	setb	F309
;LOCK.C: 299: }
;LOCK.C: 300: else
	line	300
	jmp	l65
l64:
;LOCK.C: 301: { y= s2-s4;
	line	301
	mov.b	r4l,r2l
	sub.b	r4l,r1h
;LOCK.C: 302: yneg= 0;
	line	302
	clr	F309
	line	303
l65:
;LOCK.C: 303: }
;LOCK.C: 305: x= s1 - s3;
; _x allocated to r4h
	line	305
	mov.b	r4h,r1l
	sub.b	r4h,r2h
;LOCK.C: 307: if (s3>s1)
	line	307
	cmp.b	r2h,r1l
	bl	l66
;LOCK.C: 309: { x= s3-s1;
	line	309
	mov.b	r4h,r2h
	sub.b	r4h,r1l
;LOCK.C: 310: if (!yneg)
	line	310
	jb	F309,l67
;LOCK.C: 311: { buph= tableph[256 * x + y] + 0x0100;
	line	311
	mov.b	r1h,r4h
	movs.b	r1l,#00h
	mov.b	r0l,r4l
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	clr	021Ah
	movs.b	r0h,#00h
	add.w	r0,#0100h
	jmp	L8
;LOCK.C: 312: }
;LOCK.C: 313: else
	line	313
l67:
;LOCK.C: 314: { buph= tableph[256 * y + x] + 0xfe00;
	line	314
	mov.b	r1h,r4l
	movs.b	r1l,#00h
	mov.b	r0l,r4h
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	clr	021Ah
	movs.b	r0h,#00h
	add.w	r0,#-512
	jmp	L8
	line	315
;LOCK.C: 315: }
;LOCK.C: 316: }
;LOCK.C: 317: else if (x==0)
	line	317
l66:
	or.b	r4h,r4h
	bne	l70
;LOCK.C: 318: { if (!yneg)
	line	318
	jb	F309,l71
;LOCK.C: 319: { buph= 0x4000>>6;
	line	319
	mov.w	_buph,#0100h
;LOCK.C: 320: }
;LOCK.C: 321: else
	line	321
	jmp	l69
l71:
;LOCK.C: 322: { buph= 0xc000>>6;
	line	322
	mov.w	_buph,#0300h
	line	323
;LOCK.C: 323: }
;LOCK.C: 324: }
;LOCK.C: 325: else if (yneg)
	line	325
	jmp	l69
l70:
	jnb	F309,l74
;LOCK.C: 326: { buph= tableph[256 * x + y] + 0xff00;
	line	326
	mov.b	r1h,r4h
	movs.b	r1l,#00h
	mov.b	r0l,r4l
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	clr	021Ah
	movs.b	r0h,#00h
	add.w	r0,#-256
	jmp	L8
;LOCK.C: 327: }
;LOCK.C: 328: else
	line	328
l74:
;LOCK.C: 329: { buph= tableph[256 * y + x];
	line	329
	mov.b	r1h,r4l
	movs.b	r1l,#00h
	mov.b	r0l,r4h
	mov.b	r0h,r1l
	add.w	r0,r1
	mov.w	r1,#_tableph
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.b	r0l,[r2+]
	clr	021Ah
	movs.b	r0h,#00h
L8:
	mov.w	_buph,r0
	line	330
;LOCK.C: 330: }
l69:
;LOCK.C: 333: buph= fasekor[buph & 0x3ff];
	line	333
	mov.w	r0,_buph
	and.w	r0,#03FFh
	asl.w	r0,#01h
	mov.w	r1,#_fasekor
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	add.w	r2,r0
	mov.b	0443h,r3l
	setb	021Ah
	movc.w	r0,[r2+]
	clr	0218h
	mov.w	_buph,r0
;LOCK.C: 336: }
	line	336
	pop	r4
	ret
	align.w
	global	_eph
	global	_subcref
	global	_oldph
	global	_minus
	global	_phdet
	global	_freqh
	global	_freql
	global	_freqload
	global	_hfasec
	signat	_hfasec,24
	psect	ltext
;LOCK.C: 340: void burstlock(void)
;LOCK.C: 341: { unsigned char a;
	line	341
_burstlock:
;LOCK.C: 342: unsigned int i;
;LOCK.C: 343: eph= subcref - buph;
	line	343
	mov.w	r0,_subcref
	sub.w	r0,_buph
	mov.w	_eph,r0
;LOCK.C: 354: i= oldph;
; _i allocated to r3
	line	354
	mov.w	r3,_oldph
;LOCK.C: 355: oldph= eph;
	line	355
	mov.w	_oldph,r0
;LOCK.C: 356: minus= 0;
	line	356
	clr	_minus
;LOCK.C: 360: if (eph > i)
	line	360
	cmp.w	_eph,r3
	bl	l77
;LOCK.C: 361: { if ((eph - i) >= 0x8000)
	line	361
	mov.w	r0,_eph
	sub.w	r0,r3
	jmp	L9
;LOCK.C: 362: minus= 1;
	line	362
;LOCK.C: 363: }
;LOCK.C: 364: else if (i-eph >= 0x8000)
	line	364
l77:
	mov.w	r0,r3
	sub.w	r0,_eph
L9:
	cmp.w	r0,#-32768
	bcs	l79
;LOCK.C: 365: minus= 1;
	line	365
	setb	_minus
;LOCK.C: 368: eph= eph/2 + i/2 + (0x8000 * minus);
	line	368
l79:
	mov.w	r0,_eph
	lsr.w	r0,#01h
	mov	c,_minus
	movs.w	r2,#00h
	rlc.w	r2,#01h
	asl.w	r2,#0Fh
	mov.w	r1,r3
	lsr.w	r1,#01h
	add.w	r1,r2
	add.w	r1,r0
	mov.w	_eph,r1
;LOCK.C: 398: if ((phdet & 0xe000)==0 || (phdet & 0xe000)== 0xe000)
	line	398
	mov.w	r0,_phdet
	and.w	r0,#-8192
	beq	u140
	mov.w	r0,_phdet
	and.w	r0,#-8192
	cmp.w	r0,#-8192
	bne	u151
u140:
;LOCK.C: 399: { phdet= eph>>1;
	line	399
	mov.w	r0,_eph
	lsr.w	r0,#01h
	mov.w	_phdet,r0
;LOCK.C: 400: if (phdet & 0x4000) phdet |= 0x8000;
	line	400
	and.w	r0,#04000h
	beq	l82
	or.w	_phdet,#-32768
;LOCK.C: 401: else phdet &= ~0x8000;
	line	401
	jmp	l84
u151:
;LOCK.C: 404: {
;LOCK.C: 405: if ((phdet & 0x8000)==0)
	line	405
	mov.w	r0,_phdet
	and.w	r0,#-32768
	bne	l85
;LOCK.C: 406: phdet= eph>>1;
	line	406
	mov.w	r0,_eph
	lsr.w	r0,#01h
	jmp	L10
;LOCK.C: 407: else phdet= (eph>>1) | 0x8000;
	line	407
l82:
	and.w	_phdet,#07FFFh
;LOCK.C: 402: }
;LOCK.C: 403: else
	line	403
	jmp	l84
l85:
	mov.w	r0,_eph
	lsr.w	r0,#01h
	or.w	r0,#-32768
L10:
	mov.w	_phdet,r0
	line	422
l84:
;LOCK.C: 422: }
;LOCK.C: 426: freqh= ((unsigned char)(phdet>>12)) ^8;
	line	426
	mov.w	r0,_phdet
	lsr.w	r0,#0Ch
	xor.b	r0l,#08h
	mov.w	r1,#_freqh
	mov.b	[r1],r0l
;LOCK.C: 427: a= (unsigned char)(phdet>>4);
; _a allocated to r2l
	line	427
	mov.w	r0,_phdet
	lsr.w	r0,#04h
	mov.b	r2l,r0l
;LOCK.C: 428: freql= a;
	line	428
	mov.w	r1,#_freql
	mov.b	[r1],r2l
;LOCK.C: 429: freqload= a;
	line	429
	mov.w	r1,#_freqload
	mov.b	[r1],r2l
;LOCK.C: 432: if (!freq_sel)
	line	432
	jb	_freq_sel,l76
;LOCK.C: 433: hfasec();
	line	433
	fjmp	_hfasec
;LOCK.C: 436: }
	line	436
l76:
	ret
	align.w
	global	_hfasef
	signat	_hfasef,24
	global	_phdiff
	global	_href
	global	_hinput
	global	_outf
	psect	ltext
;LOCK.C: 438: void hlock(void)
;LOCK.C: 439: { unsigned char a;
	line	439
_hlock:
;LOCK.C: 440: hfasef();
	line	440
	fcall	_hfasef
;LOCK.C: 441: phdiff= href - hinput;
	line	441
	mov.w	r2,_href
	mov.w	r3,_href+02h
	sub.w	r2,_hinput
	subb.w	r3,_hinput+02h
	mov.w	r1,#_phdiff
	mov.w	[r1],r2
	mov.w	[r1+02h],r3
;LOCK.C: 442: if (phdiff & 0x80000000)
	line	442
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#00h
	and.w	r3,#08000h
	or.w	r2,r3
	beq	l89
;LOCK.C: 443: {
;LOCK.C: 444: phdiff= -phdiff;
	line	444
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	cpl.w	r2
	cpl.w	r3
	add.w	r2,#01h
	addc.w	r3,#00h
	mov.w	[r1],r2
	mov.w	[r1+02h],r3
;LOCK.C: 446: if (!(phdiff & 0xFFFE0000))
	line	446
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#00h
	and.w	r3,#0FFFEh
	or.w	r2,r3
	bne	l90
;LOCK.C: 447: {
;LOCK.C: 449: if (phdiff & 0xFFFFFC00)
	line	449
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#0FC00h
	and.w	r3,#0FFFFh
	or.w	r2,r3
	bne	L15
;LOCK.C: 450: outf= 0x401;
	line	450
;LOCK.C: 451: else outf= -phdiff;
	line	451
	mov.w	r1,[r1]
	jmp	L11
;LOCK.C: 452: }
;LOCK.C: 453: else
	line	453
l90:
;LOCK.C: 454: { if (g_sel) phdiff= (unsigned long)864 * 256 - phdiff;
	line	454
	jnb	_g_sel,l94
	mov.w	r1,#_phdiff
	mov.w	r2,#06000h
	movs.w	r3,#03h
	sub.w	r2,[r1]
	subb.w	r3,[r1+02h]
	mov.w	[r1],r2
	mov.w	[r1+02h],r3
;LOCK.C: 455: else phdiff= (unsigned long)858 * 256 - phdiff;
	line	455
	jmp	l95
l94:
	mov.w	r1,#_phdiff
	mov.w	r2,#05A00h
	movs.w	r3,#03h
	sub.w	r2,[r1]
	subb.w	r3,[r1+02h]
	mov.w	[r1],r2
	mov.w	[r1+02h],r3
l95:
;LOCK.C: 457: if (phdiff & 0xFFFFFC00)
	line	457
	mov.w	r1,#_phdiff
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#0FC00h
	and.w	r3,#0FFFFh
	or.w	r2,r3
	bne	L12
;LOCK.C: 458: outf= 0x3ff;
	line	458
;LOCK.C: 459: else outf= phdiff;
	line	459
	mov.w	r1,[r1]
	jmp	L14
	line	460
;LOCK.C: 460: }
;LOCK.C: 461: }
;LOCK.C: 462: else
	line	462
l89:
;LOCK.C: 464: { if (!(phdiff & 0xFFFE0000))
	line	464
	mov.w	r1,#_phdiff
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#00h
	and.w	r3,#0FFFEh
	or.w	r2,r3
	bne	l99
;LOCK.C: 465: {
;LOCK.C: 466: if (phdiff & 0xFFFFFC00)
	line	466
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#0FC00h
	and.w	r3,#0FFFFh
	or.w	r2,r3
	beq	l100
L12:
;LOCK.C: 468: outf= 0x3ff;
	line	468
	mov.w	_outf,#03FFh
;LOCK.C: 469: else outf= phdiff;
	line	469
	jmp	l98
l100:
	mov.w	r1,#_phdiff
	mov.w	r1,[r1]
	jmp	L14
;LOCK.C: 470: }
;LOCK.C: 471: else
	line	471
l99:
;LOCK.C: 472: { if (g_sel) phdiff= (unsigned long)864 * 256 - phdiff;
	line	472
	jnb	_g_sel,l103
	mov.w	r1,#_phdiff
	mov.w	r2,#06000h
	movs.w	r3,#03h
	sub.w	r2,[r1]
	subb.w	r3,[r1+02h]
	mov.w	[r1],r2
	mov.w	[r1+02h],r3
;LOCK.C: 473: else phdiff= (unsigned long)858 * 256 - phdiff;
	line	473
	jmp	l104
l103:
	mov.w	r1,#_phdiff
	mov.w	r2,#05A00h
	movs.w	r3,#03h
	sub.w	r2,[r1]
	subb.w	r3,[r1+02h]
	mov.w	[r1],r2
	mov.w	[r1+02h],r3
l104:
;LOCK.C: 476: if (phdiff & 0xFFFFFC00)
	line	476
	mov.w	r1,#_phdiff
	mov.w	r2,[r1]
	mov.w	r3,[r1+02h]
	and.w	r2,#0FC00h
	and.w	r3,#0FFFFh
	or.w	r2,r3
	beq	l105
L15:
;LOCK.C: 477: outf= 0x401;
	line	477
	mov.w	_outf,#0401h
;LOCK.C: 478: else outf= -phdiff;
	line	478
	jmp	l98
l105:
	mov.w	r1,#_phdiff
	mov.w	r1,[r1]
L11:
	neg.w	r1
L14:
	mov.w	_outf,r1
	line	479
;LOCK.C: 479: }
	line	480
l98:
;LOCK.C: 480: }
;LOCK.C: 482: outf <<= 1;
	line	482
	mov.w	r0,_outf
	asl.w	r0,#01h
	mov.w	_outf,r0
;LOCK.C: 485: freqh= ((unsigned char)(outf/256)) ^8;
	line	485
	mov.b	r0l,_outf+01h
	xor.b	r0l,#08h
	mov.w	r1,#_freqh
	mov.b	[r1],r0l
;LOCK.C: 486: a= (unsigned char)(outf & 0x00ff);
; _a allocated to r0h
	line	486
	mov.b	r0h,_outf
;LOCK.C: 487: freql= a;
	line	487
	mov.w	r1,#_freql
	mov.b	[r1],r0h
;LOCK.C: 488: freqload= a;
	line	488
	mov.w	r1,#_freqload
	mov.b	[r1],r0h
;LOCK.C: 489: hfasec();
	line	489
	fjmp	_hfasec
	align.w
	psect	ltext
;LOCK.C: 493: void intern(void)
;LOCK.C: 494: { unsigned char a;
	line	494
_intern:
;LOCK.C: 495: freqh= (unsigned char)(0x800>>8);
	line	495
	mov.w	r1,#_freqh
	mov.b	[r1],#08h
;LOCK.C: 496: a= (unsigned char)(0x800 & 0x00ff);
; _a allocated to r0l
	line	496
	movs.b	r0l,#00h
;LOCK.C: 497: freql= a;
	line	497
	mov.w	r1,#_freql
	mov.b	[r1],r0l
;LOCK.C: 498: freqload= a;
	line	498
	mov.w	r1,#_freqload
	movs.b	[r1],#00h
;LOCK.C: 499: }
	line	499
	ret
	align.w
	global	_linadd
	psect	ltext
;LOCK.C: 502: void hfasec(void)
;LOCK.C: 503: { unsigned char s1, s2;
	line	503
_hfasec:
;LOCK.C: 504: s1= (unsigned char)(href>>11);
; _s1 allocated to r2l
	line	504
	mov.w	r0,_href
	asr.d	r0,#0Bh
	mov.b	r2l,r0l
;LOCK.C: 505: s2= (unsigned char)(hpos>>3);
; _s2 allocated to r1l
	line	505
	mov.w	r0,_hpos
	lsr.w	r0,#03h
	mov.b	r1l,r0l
;LOCK.C: 507: if (g_sel)
	line	507
	jnb	_g_sel,l109
;LOCK.C: 508: { if (s1>s2)
	line	508
	cmp.b	r2l,r1l
	bl	l110
;LOCK.C: 509: { s1 = s1-s2;
	line	509
	sub.b	r2l,r1l
;LOCK.C: 510: if (s1>54)
	line	510
	cmp.b	r2l,#036h
	bl	l111
;LOCK.C: 511: {
;LOCK.C: 512: s1= 108-s1;
	line	512
	mov.b	r0l,#06Ch
	sub.b	r0l,r2l
	mov.b	r2l,r0l
;LOCK.C: 513: if (s1>4) linadd= 1;
	line	513
	cmp.b	r2l,#04h
	bl	l108
	movs.b	_linadd,#01h
;LOCK.C: 514: }
;LOCK.C: 515: else if (s1>4) linadd= 2;
	line	515
	ret
l111:
	cmp.b	r2l,#04h
	bl	l108
	movs.b	_linadd,#02h
;LOCK.C: 516: }
	line	516
;LOCK.C: 517: else
	line	517
	ret
l110:
;LOCK.C: 518: { s2= s2-s1;
	line	518
	sub.b	r1l,r2l
;LOCK.C: 519: if (s2>54)
	line	519
	cmp.b	r1l,#036h
	bl	l116
;LOCK.C: 520: {
;LOCK.C: 521: s2= 108-s2;
	line	521
	mov.b	r0l,#06Ch
	sub.b	r0l,r1l
	mov.b	r1l,r0l
;LOCK.C: 522: if (s2>4) linadd= 2;
	line	522
	cmp.b	r1l,#04h
	bl	l108
	movs.b	_linadd,#02h
;LOCK.C: 523: }
;LOCK.C: 524: else if (s2>4) linadd= 1;
	line	524
	ret
l116:
	cmp.b	r1l,#04h
	bl	l108
	movs.b	_linadd,#01h
;LOCK.C: 525: }
	line	525
;LOCK.C: 526: }
;LOCK.C: 527: else
	line	527
	ret
l109:
;LOCK.C: 528: { if (s1>s2)
	line	528
	cmp.b	r2l,r1l
	bl	l121
;LOCK.C: 529: { s1 = s1-s2;
	line	529
	sub.b	r2l,r1l
;LOCK.C: 530: if (s1>54)
	line	530
	cmp.b	r2l,#036h
	bl	l122
;LOCK.C: 531: {
;LOCK.C: 532: s1= 108-s1;
	line	532
	mov.b	r0l,#06Ch
	sub.b	r0l,r2l
	mov.b	r2l,r0l
;LOCK.C: 533: if (s1>4) linadd= 1;
	line	533
	cmp.b	r2l,#04h
	bl	l108
	movs.b	_linadd,#01h
;LOCK.C: 534: }
;LOCK.C: 535: else if (s1>4) linadd= 2;
	line	535
	ret
l122:
	cmp.b	r2l,#04h
	bl	l108
	movs.b	_linadd,#02h
;LOCK.C: 536: }
	line	536
;LOCK.C: 537: else
	line	537
	ret
l121:
;LOCK.C: 538: { s2= s2-s1;
	line	538
	sub.b	r1l,r2l
;LOCK.C: 539: if (s2>54)
	line	539
	cmp.b	r1l,#036h
	bl	l127
;LOCK.C: 540: {
;LOCK.C: 541: s2= 108-s2;
	line	541
	mov.b	r0l,#06Ch
	sub.b	r0l,r1l
	mov.b	r1l,r0l
;LOCK.C: 542: if (s2>4) linadd= 2;
	line	542
	cmp.b	r1l,#04h
	bl	l108
	movs.b	_linadd,#02h
;LOCK.C: 543: }
;LOCK.C: 544: else if (s2>4) linadd= 1;
	line	544
	ret
l127:
	cmp.b	r1l,#04h
	bl	l108
	movs.b	_linadd,#01h
;LOCK.C: 545: }
	line	545
	line	546
;LOCK.C: 546: }
;LOCK.C: 547: }
	line	547
l108:
	ret
	align.w
	psect	rbit,bit,class=BITSEG,space=1,align=0
F300:
	ds	1
F309:
	ds	1
	end
