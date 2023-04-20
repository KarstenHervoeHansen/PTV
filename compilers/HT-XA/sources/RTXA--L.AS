;
;	Runtime startoff module for HI-TECH C
;	Philips XA, Large model
;
;	Clyde Smith-Stubbs, September 1995
;
;	Copyright (C) 1995, HI-TECH Software
;	All Rights Reserved
;
;	This software remains the property of HI-TECH SOFTWARE and is
;	supplied under licence only. The use of this software is
;	permitted under the terms of that licence only. Copying of
;	this software except for the purpose of making backup or
;	working copies for the use of the licensee on a single
;	processor is prohibited.
;
	psect	vectors,ovrld,class=CODE,align=2,space=0
	psect	text,class=CODE,align=2,space=0
	psect	code,class=CODE,space=0
	psect	farbss,local,class=FARBSS,space=1,reloc=65536,size=65535,space=1
	psect	fardata,local,class=FARDATA,space=1,reloc=65536,size=65535,space=1
	psect	rbss,size=1024,class=DATA,space=1,reloc=2
	psect	rdata,size=1024,class=DATA,space=0,reloc=2
	psect	rbit,bit,size=512,class=BITSEG,space=1
	psect	bss,class=DATA,space=1,reloc=2
	psect	data,class=DATA,space=0,reloc=2
	psect	const,class=DATA,space=0,reloc=2
	psect	strings,class=DATA,space=0,reloc=2
	psect	nvram,class=DATA,space=1,reloc=2
	psect	farnvram,class=FARNVRAM,space=1,reloc=2
	psect	stack,space=1,reloc=2
	psect	heap,space=1,reloc=2
;
	global	start,_main,_exit		;entry point, main() and exit()
	global	__Lrbss,__Hrbss			;rbss psect bounds
	global	__Lrbit,__Hrbit			;rbit psect bounds
	global	__Lrdata,__Hrdata,__Brdata	;rdata psect bounds, load addr
	global	__Lbss,__Hbss			;bss psect bounds
	global	__Ldata,__Hstrings,__Bdata	;data/const/strings bounds, load
	global	__Hstack			;stack address
	global	powerup,start
;
	psect	vectors
	dw	8F00h,powerup		;point RESET vector at powerup code, set system mode
;
;	Copyright message, and initialization code
;
	psect	text
	db	"C Library Copyright (C) 1995 HI-TECH Software",0dh,0ah
	db	"Brisbane, QLD. Australia",0dh,0ah,0,26
	db	"$Id$"
;
start:
	and.b	440h,#0FEh		;reset page zero bit
	mov	sp,#__Hstack-2		;initialize SP
	add	sp,#2
;
;	Clear the RBIT psect
;
	mov	r0,#((__Hrbit-__Lrbit+7)/8) & 127
	mov	r1,#__Lrbit/8
	call	clear
;
;	Clear the RBSS psect
;
	mov	r0,#__Hrbss-__Lrbss
	mov	r1,#__Lrbss
	call	clear
;
;	Copy the RDATA psect
;
	mov	r0,#__Hrdata-__Lrdata
	mov	r1,#__Brdata
	mov	r2,#__Lrdata
	call	copy
;
;	Clear the BSS psect
;
	mov	r0,#__Hbss-__Lbss
	mov	r1,#__Lbss
	call	clear
;
;	Copy the DATA,CONST and STRINGS psects
;
	mov	r0,#__Hstrings-__Ldata
	mov	r1,#__Bdata
	mov	r2,#__Ldata
	call	copy
;
;	Now call the main() function
;
	fcall	_main
_exit:
	br	start
;
;	clear:	zero "r0" bytes of memory at address "r1",
;		does nothing if "r0" is zero.
;
clear:
	cmp	r0,#0
	beq	2f
1:
	movs.b	[r1+],#0	;clear 1 byte
	djnz	r0,1b
2:
	ret
;
;	copy:	copy "r0" bytes from address "r1" to address "r2",
;		"r1" is treated as a code pointer. does nothing
;		if "r0" is zero.
;
copy:
	cmp	r0,#0
	beq	2f
1:
	movc	r3l,[r1+]	;get source byte
	mov	[r2+],r3l	;store to destination
	djnz	r0,1b
2:
	ret
;
	end	start
;
;	End of file: "$Id$"
;
