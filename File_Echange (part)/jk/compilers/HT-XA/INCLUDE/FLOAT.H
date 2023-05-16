/*	Characteristics of floating types */

#define	DBL_RADIX	2		/* radix of exponent for a double */
#define	DBL_ROUNDS	0		/* doubles don't round when converted to int */
#define	FLT_RADIX	2		/* radix of float exponent */
#define	FLT_ROUNDS	0		/* float also truncates to int */

#if	defined(i8096) || defined(m6800) || defined(m6809) || defined(i8051) || \
	defined(h8300) || defined(h8300h)|| defined(m6805)
#define	FLT_MANT_DIG	24
#define	FLT_EPSILON	1.19209290e-07
#define	FLT_DIG		6
#define	FLT_MIN_EXP	-125
#define	FLT_MIN		1.17549435e-38
#define	FLT_MIN_10_EXP	-37
#define	FLT_MAX_EXP	128
#define FLT_MAX         3.40282347e+38
#define	FLT_MAX_10_EXP	38
#define	DBL_MANT_DIG	24
#define	DBL_EPSILON	1.19209290e-07
#define	DBL_DIG		6
#define	DBL_MIN_EXP	-125
#define	DBL_MIN		1.17549435e-38
#define	DBL_MIN_10_EXP	-37
#define	DBL_MAX_EXP	128
#define DBL_MAX         3.40282347e+38
#define	DBL_MAX_10_EXP	38
#endif

#if	defined(_MPC_)
	
/* MICROCHIP PIC */

#define	FLT_MANT_DIG	16
#define	FLT_EPSILON	3.05176e-05
#define	FLT_DIG		5
#define	FLT_MIN_EXP	-125
#define	FLT_MIN		1.17549435e-38
#define	FLT_MIN_10_EXP	-37
#define	FLT_MAX_EXP	128
#define FLT_MAX         3.40277e+38
#define	FLT_MAX_10_EXP	38
#if	sizeof(double) == 4
#define	DBL_MANT_DIG	24
#define	DBL_EPSILON	1.19209290e-07
#define	DBL_DIG		6
#define	DBL_MIN_EXP	-125
#define	DBL_MIN		1.17549435e-38
#define	DBL_MIN_10_EXP	-37
#define	DBL_MAX_EXP	128
#define DBL_MAX         3.40282347e+38
#define	DBL_MAX_10_EXP	38
#else
#define	DBL_MANT_DIG	FLT_MANT_DIG
#define	DBL_EPSILON	FLT_EPSILON
#define	DBL_DIG		FLT_DIG
#define	DBL_MIN_EXP	FLT_MIN_EXP
#define	DBL_MIN		FLT_MIN
#define	DBL_MIN_10_EXP	FLT_MIN_10_EXP
#define	DBL_MAX_EXP	FLT_MAX_EXP
#define DBL_MAX         FLT_MAX
#define	DBL_MAX_10_EXP	FLT_MAX_10_EXP
#endif
#endif

#if	defined(_XA_)
#define	FLT_MANT_DIG	24
#define	FLT_EPSILON	1.19209290e-07
#define	FLT_DIG		6
#define	FLT_MIN_EXP	-125
#define	FLT_MIN		1.17549435e-38
#define	FLT_MIN_10_EXP	-37
#define	FLT_MAX_EXP	128
#define FLT_MAX         3.40282347e+38
#define	FLT_MAX_10_EXP	38
#define	FLT_MANT_DIG	24		/* 24 bits in mantissa */
#if	sizeof(double) == 4
#define	DBL_MANT_DIG	24
#define	DBL_EPSILON	1.19209290e-07
#define	DBL_DIG		6
#define	DBL_MIN_EXP	-125
#define	DBL_MIN		1.17549435e-38
#define	DBL_MIN_10_EXP	-37
#define	DBL_MAX_EXP	128
#define DBL_MAX         3.40282347e+38
#define	DBL_MAX_10_EXP	38
#define	DBL_MANT_DIG	24		/* 24 bits in mantissa */
#elif	__FASTDBL
#define	DBL_MANT_DIG	48
#define	DBL_EPSILON	2.147544363e-10
#define	DBL_DIG		14
#define	DBL_MIN_EXP	-16381
#define	DBL_MIN		2.225073858507202e-1640
#define	DBL_MIN_10_EXP	-1640
#define	DBL_MAX_EXP	16384
#define	DBL_MAX		1.797693134862315e+1640
#define	DBL_MAX_10_EXP	1641
#else
#define	DBL_MANT_DIG	53
#define	DBL_EPSILON	2.220446049250313e-16
#define	DBL_DIG		15
#define	DBL_MIN_EXP	-1021
#define	DBL_MIN		2.225073858507202e-308
#define	DBL_MIN_10_EXP	-307
#define	DBL_MAX_EXP	1024
#define	DBL_MAX		1.797693134862315e+308
#define	DBL_MAX_10_EXP	308
#endif
#endif


#if	defined(z80)
#define	FLT_MANT_DIG	24		/* 24 bits in mantissa */
#define	DBL_MANT_DIG	24		/* ditto for double */
#define	DBL_MANT_DIG	24		/* ditto long double */
#define	FLT_EPSILON	1.19209290e-07	/* smallest x, x+1.0 != 1.0 */
#define	DBL_EPSILON	1.19209290e-07	/* smallest x, x+1.0 != 1.0 */
#define	FLT_DIG		6		/* decimal significant digs */
#define	DBL_DIG		6
#define	FLT_MIN_EXP	-63		/* min binary exponent */
#define	DBL_MIN_EXP	-63
#define	FLT_MIN		1.0842021e-19	/* smallest floating number */
#define	DBL_MIN		1.0842021e-19
#define	FLT_MIN_10_EXP	-19
#define	DBL_MIN_10_EXP	-19
#define	FLT_MAX_EXP	63		/* max binary exponent */
#define	DBL_MAX_EXP	63
#define	FLT_MAX		9.223369e18	/* max floating number */
#define	DBL_MAX		9.223369e18
#define	FLT_MAX_10_EXP	18		/* max decimal exponent */
#define	DBL_MAX_10_EXP	18
#endif	z80

#if	i8086 || m68k

/*	The 8086 and 68000 use IEEE 32 and 64 bit floats */

#define	FLT_MANT_DIG	24
#define	FLT_EPSILON	1.192092896e-07
#define	FLT_DIG		6
#define	FLT_MIN_EXP	-125
#define	FLT_MIN		1.175494351e-38
#define	FLT_MIN_10_EXP	-37
#define	FLT_MAX_EXP	128
#define	FLT_MAX		3.402823466+38
#define	FLT_MAX_10_EXP	38
#define	DBL_MANT_DIG	53
#define	DBL_EPSILON	2.220446049250313e-16
#define	DBL_DIG		15
#define	DBL_MIN_EXP	-1021
#define	DBL_MIN		2.225073858507202e-308
#define	DBL_MIN_10_EXP	-307
#define	DBL_MAX_EXP	1024
#define	DBL_MAX		1.797693134862315e+308
#define	DBL_MAX_10_EXP	308
#endif	i8086 || m68k


/*	long double equates to double */


#define	LDBL_MANT_DIG	DBL_MANT_DIG
#define	LDBL_EPSILON	DBL_EPSILON
#define	LDBL_DIG	DBL_DIG
#define	LDBL_MIN_EXP	DBL_MIN_EXP
#define	LDBL_MIN	DBL_MIN
#define	LDBL_MIN_10_EXP	DBL_MIN_10_EXP
#define	LDBL_MAX_EXP	DBL_MAX_EXP
#define	LDBL_MAX	DBL_MAX
#define	LDBL_MAX_10_EXP	DBL_MAX_10_EXP
