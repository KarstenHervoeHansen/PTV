/*	Standard types for HI-TECH Software code
	These types may need to be tuned for different
	machines/compilers. Notes with each one indicate assumptions
	that should be maintained for each type.
 */

/*
 *	$Header: /home/clyde/nz/RCS/hitech.h,v 1.8 94/04/27 13:39:06 clyde Exp $
 *
 *	$Log:	hitech.h,v $
 * Revision 1.8  94/04/27  13:39:06  clyde
 * Make FAST explicitly signed
 * 
 * Revision 1.7  93/05/13  00:10:56  clyde
 * Fix #endif's
 * 
 * Revision 1.6  92/06/25  16:15:43  clyde
 * Set LITTLE_ENDIAN for appropriate machines
 * 
 * Revision 1.5  92/06/01  15:05:44  clyde
 * Made setting of BIG_ENDIAN conditional
 * 
 * Revision 1.4  91/07/18  22:57:30  clyde
 * Added conditional setting of LARGE_DATA
 * 
 * Revision 1.3  91/07/08  15:37:16  clyde
 * Added BIG_ENDIAN
 * 
 * Revision 1.2  89/08/09  11:57:10  clyde
 * Added RCS header stuff
 * 
 */


/*
	Turn ANSI on if the compiler supports function prototypes and
	has the ANSI header files
		<stdlib.h>
		<string.h>
 */

#if	HI_TECH_C
#define	ANSI	1
#endif	/* HI_TECH_C */

/*	shorthand types */

#define	uchar	unsigned char
#define	ulong	unsigned long
#define	ushort	unsigned short

/* useful, tuneable types. Change only if:

	1) compiler does not support type, e.g. unsigned char.
	2) compiler generates bad code for a particular type.
	3) a larger type would generate faster code, e.g. byte counters
	   on the 65816 are inefficient code-wise.
 */

#define	BOOL	unsigned char		/* boolean variable. Any integral type
					   will do. */
#define	FAST	signed char		/* fast, small counter. Must permit
					   values -128 to 127 but may be larger. */
#define	UFAST	unsigned char		/* fast, small unsigned counter. Must
					   permit values 0-255 at least */
#define	BYTE	unsigned char		/* sizeof(BYTE) must == 1 */
#define	INT_16	short			/* signed, >= 16 bits */
#define	UINT_16	unsigned short		/* unsigned, >= 16 bits */
#define	INT_32	long			/* signed, >= 32 bits */
#define	UINT_32	unsigned long		/* unsigned, >= 32 bits */


/*	Register variable selectors; REG1 is for things that must go
	in registers at all costs, REG2 for things that should, REG3 for
	things that could go in registers if there are any left over.
	Ordering of declarations will of course come into it too.
 */

#if	z80		/* only has one register variable */
#define	REG1	register
#define	REG2	auto
#define	REG3	auto
#endif	/* z80 */

#if	i8086		/* only has two register variable */
#define	REG1	register
#define	REG2	register
#define	REG3	auto
#endif	/* i8086 */

#if	i8096 || m68k	/* lots of registers! */
#define	REG1	register
#define	REG2	register
#define	REG3	register
#endif	/* i8096 || m68k */

/*
 *	Little endian vs big endian
 *	Not complete by any means.
 */

#if     !defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)
#if interdata || sparc || sun || m68k || m68000
#define	BIG_ENDIAN	1
#endif

#if vax || i8086 || i386 || iAPX286
#undef	BIG_ENDIAN
#define	LITTLE_ENDIAN	1
#endif
#endif

/*
 *	Try and identify processors with large data spaces
 */

#if	sparc || m68k || m68000 || i386 || i80386 || FLAT_MODEL || vax
#define	LARGE_DATA	1
#endif
