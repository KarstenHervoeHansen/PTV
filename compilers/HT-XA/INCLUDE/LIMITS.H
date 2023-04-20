/*	Characteristics of integral types */

#define	CHAR_BIT	8		/* bits per char */
#define	CHAR_MAX	127		/* max value of a char */
#define	CHAR_MIN	-128		/* min value */
#define	SCHAR_MAX	CHAR_MAX	/* chars are signed */
#define	SCHAR_MIN	CHAR_MIN
#define	UCHAR_MAX	255		/* for unsigned chars */
#define	SHRT_MAX	32767		/* max value of a short */
#define	SHRT_MIN	(int)-32768
#define	USHRT_MAX	65535		/* unsigned short */

/*	32 bit chips have 32 bit integers */

#if	defined(m68k) || defined(i8086) && defined(FLAT_MODEL)
#define	INT_MAX		2147483647	/* max value of int */
#define	INT_MIN		(int)-2147483648	/* min value */
#define	UINT_MAX	4294967295	/* unsigned int */
#else	/* not 32 bit chips */
#define	INT_MAX		32767		/* max for int */
#define	INT_MIN		(int)-32768	/* min for int */
#define	UINT_MAX	65535		/* unsigned int */
#endif	/* 32 bit chips */
#define	LONG_MAX	2147483647	/* max value of long */
#define	LONG_MIN	(long)-2147483648	/* min value */
#define	ULONG_MAX	4294967295	/* unsigned long */
