/*
 *      Version : @(#)float.h	1.35
 */

/**************************************************************************
**                                                                        *
**  FILE        :  float.h                                                *
**                                                                        *
**  DESCRIPTION :  FPRT (Floating Point Run-Time C expression support)    *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef	_FLOAT_H
#define	_FLOAT_H

/*
 * IEEE-754-1985 Format values:
*/
#define	FLT_RADIX	(2)
#define	FLT_ROUNDS	(1)	/* round to nearest */

#define	FLT_MANT_DIG	(24)
#define	FLT_DIG		(6)
#define	FLT_EPSILON	(1.1920929e-07F)
#define	FLT_MIN		(1.1754944e-38F)
#define	FLT_MIN_EXP	(-125)
#define	FLT_MIN_10_EXP	(-37)
#define	FLT_MAX		(3.4028233e+38F)
#define	FLT_MAX_EXP	(+128)
#define	FLT_MAX_10_EXP	(+38)

#ifndef _SINGLE_FP
#define	DBL_MANT_DIG	(53)
#define	DBL_DIG		(15)
#define	DBL_EPSILON	(2.2204460492503131E-16)
#define	DBL_MIN		(2.2250738585072014E-308)
#define	DBL_MIN_EXP	(-1021)
#define	DBL_MIN_10_EXP	(-307)
#define	DBL_MAX		(1.7976931348623157E+308)
#define	DBL_MAX_EXP	(+1024)
#define	DBL_MAX_10_EXP	(+308)
#else
#define	DBL_MANT_DIG	(FLT_MANT_DIG)
#define	DBL_DIG		(FLT_DIG)
#define	DBL_MIN_10_EXP	(FLT_MIN_10_EXP)
#define	DBL_MAX_10_EXP	(FLT_MAX_10_EXP)
#define	DBL_EPSILON	(FLT_EPSILON)
#define	DBL_MIN		(FLT_MIN)
#define	DBL_MAX		(FLT_MAX)
#define	DBL_MIN_EXP	(FLT_MIN_EXP)
#define	DBL_MAX_EXP	(FLT_MAX_EXP)
#endif

#define	LDBL_DIG	(DBL_DIG)
#define	LDBL_MANT_DIG	(DBL_MANT_DIG)
#define	LDBL_MIN_10_EXP	(DBL_MIN_10_EXP)
#define	LDBL_MAX_10_EXP	(DBL_MAX_10_EXP)
#define	LDBL_EPSILON	(DBL_EPSILON)
#define	LDBL_MIN	(DBL_MIN)
#define	LDBL_MAX	(DBL_MAX)
#define	LDBL_MIN_EXP	(DBL_MIN_EXP)
#define	LDBL_MAX_EXP	(DBL_MAX_EXP)

/*
 * IEEE-754 Exception handling interface definition
*/

#define	_TYPE_SIGNED_CHARACTER		1
#define	_TYPE_UNSIGNED_CHARACTER	2
#define	_TYPE_SIGNED_SHORT_INTEGER	3
#define	_TYPE_UNSIGNED_SHORT_INTEGER	4
#define	_TYPE_SIGNED_INTEGER		5
#define	_TYPE_UNSIGNED_INTEGER		6
#define	_TYPE_SIGNED_LONG_INTEGER	7
#define	_TYPE_UNSIGNED_LONG_INTEGER	8
#define	_TYPE_FLOAT			9
#define	_TYPE_DOUBLE			10

typedef unsigned char	_fp_fmt_t;

#define	EFINVOP		0x01	/* invalid operation, for instance 0.0/0.0, 0.0*Inf or on a NaN */
#define	EFDIVZ		0x02	/* division by zero, for instance 12.5/0.0 (not 0.0/0.0)    */
#define	EFOVFL		0x04	/* overflow, when the result of an operation is too large       */
#define	EFUNFL		0x08	/* underflow,when the result of an operation is too small       */
#define	EFINEXCT	0x10
#define	EFALL		0x1f	/* the combination of all the above enlisted exceptions */

typedef unsigned char	_fp_exception_type_t;

#define	_OP_ADDITION		1
#define	_OP_SUBTRACTION		2
#define	_OP_COMPARISON		3
#define	_OP_EQUALITY		4
#define	_OP_LESS_THAN		5
#define	_OP_LARGER_THAN		6
#define	_OP_MULTIPLICATION	7
#define	_OP_DIVISION		8
#define	_OP_CONVERSION		9

typedef unsigned char	_fp_operation_type_t;

typedef union _fp_value_union_t
{
	char		c;
	unsigned char	uc;
	short		s;
	unsigned short	us;
	int		i;
	unsigned int	ui;
	long		l;
	unsigned long	ul;
	float		f;
	double		d;
	struct a_s
	{
		long	l1;
		long	l2;
	}
	a_s;
}
_fp_value_union_t;

typedef struct _fp_exception_info_t
{
	_fp_exception_type_t	exception;
	_fp_operation_type_t	operation;
	_fp_fmt_t		source_format;
	_fp_fmt_t		destination_format;
	_fp_value_union_t	operand1;
	_fp_value_union_t	operand2;
	_fp_value_union_t	result;
}
_fp_exception_info_t;

extern void		_fp_install_trap_handler( void (*) ( _fp_exception_info_t * ) );

extern int		_fp_get_exception_mask( void );
extern void		_fp_set_exception_mask( int );

extern int		_fp_get_exception_status( void );
extern void		_fp_set_exception_status( int );

/*
 * IEEE-754-1985 floating point format support macro's used in the MATH library
 *
 * 32 bit unsigned long size implementation
 * use assingment to union once, to prevent excessive load of long via a pointer to the float
*/
#define	_IEEE_754_FORMAT	1

/**************************************************************/

#define	FLOAT_FRACTION_SIZE		23	/* without the hidden bit */
#define	FLOAT_BIAS			127
#define	MIN_FLOAT_BIASED_EXP		1
#define	MAX_FLOAT_BIASED_EXP		0xfe
#define	FLOAT_BIASED_INF_EXP 		0xff
#define	FLOAT_BIASED_NaN_EXP 		0xff
#define	COPY_FLOAT_SIGN( sign, l )	( ((l) & 0x7fffffff) | ((unsigned long)(sign) & 0x80000000) )
#define	GET_FLOAT_SIGN( l )		( ( (l) & 0x80000000 ) ? 1 : 0)
#define	GET_FLOAT_MANTISSA( l )		( (l) & 0x7fffff )
#define	GET_FLOAT_EXPONENT( l )		( ( (l) >> FLOAT_FRACTION_SIZE) & 0xff )
#define	STRIP_FLOAT_EXPONENT( l )	( (l) & 0x807fffff )
#define	PUT_FLOAT_EXPONENT( l, exp )	( (l) | ((unsigned long)exp << FLOAT_FRACTION_SIZE) )
#define	FLOAT_IS_ZERO( l )		( ((l) & 0x7fffffff) == 0 ) /* test mantissa too for denormals */
typedef union
{
	unsigned long	l;
	float		f;
}
float2long_t;

/***********************************************************************/

#define	DOUBLE_FRACTION_SIZE		52	/* without the hidden bit */
#define	DOUBLE_BIAS			1023
#define	MIN_DOUBLE_BIASED_EXP 		1
#define	MAX_DOUBLE_BIASED_EXP		0x7fe
#define	DOUBLE_BIASED_INF_EXP 		0x7ff
#define	DOUBLE_BIASED_NaN_EXP 		0x7ff
#define	COPY_DOUBLE_SIGN( sign, hi )	( ((hi) & 0x7fffffff) | ((unsigned long)(sign) & 0x80000000) )
#define	GET_DOUBLE_SIGN( hi )		( ( (hi) & 0x80000000 ) ? 1 : 0)
#define	GET_DOUBLE_HI_MANTISSA( hi )	( (hi) & ( ( 1L << (DOUBLE_FRACTION_SIZE - 32) ) - 1 ) )
#define	GET_DOUBLE_LO_MANTISSA( lo )	(lo)
#define	GET_DOUBLE_EXPONENT( hi )	( ( (hi) >> (DOUBLE_FRACTION_SIZE - 32) ) & 0x7ff )
#define	STRIP_DOUBLE_EXPONENT( hi )	( (hi) & 0x800fffff )
#define	PUT_DOUBLE_EXPONENT( hi, exp )	( (hi) | ((unsigned long)exp << (DOUBLE_FRACTION_SIZE - 32) ) )
#define	DOUBLE_IS_ZERO( hi, lo )	( ((hi) & 0x7fffffff) == 0 && (lo) == 0 ) /* also for denormals */

typedef union
{
#if _BIG_ENDIAN
	struct
	{
		unsigned long	hi;
		unsigned long	lo;
	}
	s;
#else
	struct
	{
		unsigned long	lo;
		unsigned long	hi;
	}
	s;
#endif
	double	d;
}
double2longs_t;

/**********************************************************************/

/*
 * A subset of the IEEE-754-1985 Recommended Functions for both float and double
*/
float	copysignf( float, float );
int	isinff( float );
int	isfinitef( float );
int	isnanf( float );
float	scalbf( float, int );
double	copysign( double, double );
int	isinf( double );
int	isfinite( double );
int	isnan( double );
double	scalb( double, int );

#endif  /* _FLOAT_H */
