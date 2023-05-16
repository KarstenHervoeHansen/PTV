/* Definitions for interrupt handling */



#ifndef	__INTRPT_H

#define	__INTRPT_H
#if	defined(i8086) && !defined(FLAT_MODEL)
typedef far interrupt void (*isr)(void);
#else
typedef interrupt void (*isr)(void);
#endif

#define	_imkstr_(x)	#x

/*
 *	ROM_VECTOR, RAM_VECTOR & CHANGE_VECTOR macros for 6805, 6809,
 *	68HC11/6801/6301 and 680x0
 *
 *	ROM_VECTOR: points a ROM based vector directly at an interrupt
 *		    function.
 *
 *	RAM_VECTOR: points a ROM based vector at a JMP instruction in
 *		    the "data" psect which jumps to the interrupt function.
 *		    If "data" is linked into RAM, this results in a "soft"
 *		    vector which can be modified.
 *
 *	CHANGE_VECTOR:  changes the a JMP instruction defined by RAM_VECTOR to
 *			point at a different interrupt function.
 *
 *	READ_RAM_VECTOR:	returns the address of the interrupt function
 *				to which a RAM vector effectively points.
 *
 *	Colin Weaver, August 1992
 */

/*
 *	68HC11,6801,6301,6805	ei() and di()
 */

#if	m6800 || m6805
#define	ei()	asm("	cli")
#define	di()	asm("	sei")
#endif	/* m6800 */

/*
 *	6805	ROM_VECTOR, RAM_VECTOR, CHANGE_VECTOR, READ_RAM_VECTOR
 */

#if	m6805
#define ROM_VECTOR(vec, fun) \
		asm("	ifge	(" _imkstr_(vec) "!.$F)-$E"); \
		asm("	fail	Can't modify reset vector"); \
		asm("	endc"); \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) "!.$F"); \
		asm("	fdb	_" _imkstr_(fun)); \
		asm("	psect	text")

#define	INIT_BYTE(addr, value) \
		asm("	psect	absdata,abs,ovrld"); \
		asm("	org	" _imkstr_(addr)); \
		asm("	fcb	" _imkstr_(value)); \
		asm("	psect	text")

struct _RAM_VEC {
	unsigned char	jmp;		/* jmp opcode */
	interrupt void	(*addr)(void);	/* address field */
};

#define	RAM_VECTOR(vec, fun) \
		asm("	ifge	(" _imkstr_(vec) "!.$F)-$E"); \
		asm("	fail	Can't modify reset vector"); \
		asm("	endc"); \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) "!.$F"); \
		asm("	fdb	999f"); \
		asm("	psect	data,class=CODE"); \
		asm("999:"); \
		asm("	jmp	_" _imkstr_(fun)); \
		asm("	psect	text")

extern unsigned char	_Lvectors[];
#define CHANGE_VECTOR(vec, fun)	(*(struct _RAM_VEC **)(_Lvectors+((vec)&0x0F)))->addr = fun
#define	READ_RAM_VECTOR(vec)	((*(struct _RAM_VEC **)(_Lvectors+((vec)&0x0F)))->addr)
#endif

/*
 *	68HC11, 6801, 6301, 6809 ROM_VECTOR, RAM_VECTOR, CHANGE_VECTOR, etc.
 */

#if	m6800 || m6809
#define ROM_VECTOR(vec, fun) \
		asm("	ifge	(" _imkstr_(vec) "!.$3F)-$3E"); \
		asm("	fail	Can't modify reset vector"); \
		asm("	endc"); \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) "!.$3F"); \
		asm("	fdb	_" _imkstr_(fun)); \
		asm("	psect	text")

struct _RAM_VEC {
	unsigned char	jmp;		/* jmp opcode */
	interrupt void	(*addr)(void);	/* address field */
};

#define	RAM_VECTOR(vec, fun) \
		asm("	ifge	(" _imkstr_(vec) "!.$3F)-$3E"); \
		asm("	fail	Can't modify reset vector"); \
		asm("	endc"); \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) "!.$3F"); \
		asm("	fdb	999f"); \
		asm("	psect	data,class=DATA"); \
		asm("999:"); \
		asm("	jmp	_" _imkstr_(fun)); \
		asm("	psect	text")

extern unsigned char	_Lvectors[];
#define CHANGE_VECTOR(vec, fun)	(*(struct _RAM_VEC **)(_Lvectors+((vec)&0x3F)))->addr = fun
#define	READ_RAM_VECTOR(vec)	((*(struct _RAM_VEC **)(_Lvectors+((vec)&0x3F)))->addr)
#endif

#if	m6809
/* ei() and di() are for the IRQ, efi() and edi() are for FIRQ */

#define	ei()	asm("	andcc	#$EF")
#define	di()	asm("	orcc	#$10")
#define	efi()	asm("	andcc	#$BF")
#define	dfi()	asm("	orcc	#$40")
#endif	/* m6809 */

/*
 *	68000 interrupt vector and interrupt handling
 */

#if	m68k

#define	di()	asm("or.w #$700,sr")
#define	ei()	asm("and.w #$F8FF,sr")

#define	ROM_VECTOR(vec, fun) \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	dc.l	_" _imkstr_(fun)); \
		asm("	psect	text")

struct _RAM_VEC {
	unsigned short	jmp;		/* jmp opcode */
	interrupt void	(*addr)(void);	/* address field */
};

#define	RAM_VECTOR(vec, fun) \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	dc.l	999f"); \
		asm("	psect	data,class=DATA"); \
		asm("999:"); \
		asm("	jmp	_" _imkstr_(fun)); \
		asm("	psect	text")

extern unsigned char	_Lvectors[];
#define CHANGE_VECTOR(vec, fun)	(*(struct _RAM_VEC **)(_Lvectors+(vec)))->addr = fun
#define	READ_RAM_VECTOR(vec)	((*(struct _RAM_VEC **)(_Lvectors+(vec)))->addr)

#endif	/* m68k */

#if	i8051

/*
 *	8051 interrupt vector and interrupt handling
 */

#define	ei()	asm("	setb	ea")
#define	di()	asm("	clr	ea")

#define	ROM_VECTOR(vec, fun) \
		asm("	global	_" _imkstr_(fun)); \
		asm("	psect	vectors,ovrld"); \
		asm("	org	" _imkstr_(vec)); \
		asm("	ljmp	_" _imkstr_(fun)); \
		asm("	psect	text")

#define	RAM_VECTOR(vec, fun) \
		asm("	global	_" _imkstr_(fun)); \
		asm("	psect	vectors,ovrld"); \
		asm("	org	" _imkstr_(vec)); \
		asm("	push	999f+1"); \
		asm("	push	999f"); \
		asm("	ret"); \
		asm("	psect	rdata,class=DATA"); \
		asm("999:"); \
		asm("	dw	_" _imkstr_(fun)); \
		asm("	psect	text");

extern near isr * code  _Lvectors[];

#define	CHANGE_VECTOR(vec, fun)	(*_Lvectors[(vec)+3] = fun)
#define	READ_RAM_VECTOR(vec)	(*_Lvectors[(vec)+3])

#endif	/* i8051 */

#ifdef	ROM_VECTOR
#define	set_vector(vec, fun)	ROM_VECTOR(vec, fun)
#endif

#if	i8096
#define	ei()	asm("	ei")
#define	di()	asm("	di")
#define	set_vector(vec, fun) \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) " and 0x7F"); \
		asm("	dcw	_" _imkstr_(fun)); \
		asm("	psect	text")

#endif	/* i8096 */

#if	h8300
#define	ei()	asm("	andc	#H'7F,ccr")
#define	di()	asm("	orc	#H'80,ccr")
#define	set_vector(vec, fun) \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) " and H'FF"); \
		asm("	dc.w	_" _imkstr_(fun)); \
		asm("	psect	text")

#endif	/* h8300 */

#if	h8300h
#define	ei()	asm("	andc	#H'7F,ccr")
#define	di()	asm("	orc	#H'80,ccr")
#define	set_vector(vec, fun) \
		asm("	psect	vectors,ovrld"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec) " and H'FF"); \
		asm("	dc.l	_" _imkstr_(fun)); \
		asm("	psect	text")

#endif	/* h8300 */
#if	i8086
#define	ei()	asm("	sti")
#define	di()	asm("	cli")
#endif	/* i8086 */

#if	z80
#define	ei()	asm("	ei")
#define	di()	asm("	di")
extern void	im(unsigned char);


/*
 *	Z80 interrupt mode 2 vector command
 */

#define ROM_VECTOR(vec, fun) \
		asm("	psect	im2vecs,class=CODE,ovrld,reloc=256"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	defw	_" _imkstr_(fun)); \
		asm("	psect	text")

struct _RAM_VEC {
        unsigned char   jmp;            /* jmp opcode */
        unsigned int    addr;           /* address field */
};

#define RAM_VECTOR(vec, fun) \
                asm("   psect   im2vecs,class=CODE,ovrld,size=256,reloc=256"); \
                asm("   org     " _imkstr_(vec)); \
                asm("   defw    999f"); \
                asm("   psect   data,class=DATA"); \
                asm("999:"); \
                asm("   jp      _" _imkstr_(fun)); \
                asm("   psect   text")

extern unsigned char    _Lim2vecs[];
#define CHANGE_VECTOR(vec, fun) (*(struct _RAM_VEC **)(_Lim2vecs+((vec)&0xFF)))->addr = ((unsigned)(fun))
#define READ_RAM_VECTOR(vec)    ((void interrupt (*)(void))((*(struct _RAM_VEC **)(_Lim2vecs+((vec)&0xFF)))->addr))
#endif	z80

#if	_XA_

#define	_iexpand_(a)	a
#define	ei()	asm("	setb	337h")
#define	di()	asm("	clr	337h")

struct _RAM_VEC {
	unsigned short	jmp;		/* jmp opcode */
	interrupt void	(*addr)(void);	/* address field */
};

#if	LARGE_MODEL | HUGE_MODEL
#define	ROM_VECTOR(vec, fun, psw) \
		asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	dw	"_imkstr_(psw)", 999f"); \
		asm("	psect	text"); \
		asm("999:	fjmp	_" _imkstr_(fun));

#define	RAM_VECTOR(vec, fun, psw) \
		asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	dw	"_imkstr_(psw)", 999f"); \
		asm("	psect	text"); \
		asm("999:	push.w	__Vc" _imkstr_(vec) "__"); \
		asm("	push.w	__Vc" _imkstr_(vec) "__+2"); \
		asm("	ret"); \
		asm("	psect	rdata,size=1024,class=DATA,space=0"); \
		asm("	global	__Vc" _imkstr_(vec) "__"); \
		asm("__Vc" _imkstr_(vec) "__:"); \
		asm("	dw	_" _imkstr_(fun) " and 0ffffh"); \
		asm("	dw	_" _imkstr_(fun) " shr 16"); \
		extern isr _Vc ## _iexpand_(vec) ## __

#else	/* LARGE_MODEL */

#define	ROM_VECTOR(vec, fun, psw) \
		asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	dw	"_imkstr_(psw)", _" _imkstr_(fun));

#define	RAM_VECTOR(vec, fun, psw) \
		asm("	psect	vectors,ovrld,align=2,class=CODE,space=0"); \
		asm("	global	_" _imkstr_(fun)); \
		asm("	org	" _imkstr_(vec)); \
		asm("	dw	"_imkstr_(psw)", 999f"); \
		asm("	psect	text"); \
		asm("999:	push.w	__Vc" _imkstr_(vec) "__"); \
		asm("	ret"); \
		asm("	psect	rdata,size=1024,class=DATA,space=0"); \
		asm("	global	__Vc" _imkstr_(vec) "__"); \
		asm("__Vc" _imkstr_(vec) "__:"); \
		asm("	dw	_" _imkstr_(fun)); \
		extern isr _Vc ## _iexpand_(vec) ## __
#endif


#define	USE_RAM_VECTOR(vec)	extern isr _Vc ## _iexpand_(vec) ## __
#define CHANGE_VECTOR(vec, fun) (_Vc ## _iexpand_(vec) ## __ = (fun))
#define	READ_RAM_VECTOR(vec)	(_Vc ## _iexpand_(vec) ## __)

#endif	/* _XA_ */

/*	The type of a pointer to an interrupt vector */

#if	defined(i8086) && !defined(FLAT_MODEL)

extern isr set_vector(volatile far isr *, isr);

#if	_HOSTED
extern isr setiva(int, isr);
extern isr getiva(int);
#endif	/* _HOSTED */

#else	/* i8086 */

#ifndef	set_vector
extern isr set_vector(volatile isr *, isr);
#endif

#endif	/* i8086 */

#endif	/* __INTRPT_H */
