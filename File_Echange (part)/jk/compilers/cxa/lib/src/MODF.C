/**************************************************************************
**                                                                        *
**  FILE        :  modf.c                                                 *
**                                                                        *
**  DESCRIPTION :  modf implementation for IEEE-754 single precision/     *
**                 double precision floating point format.                *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <float.h>
#include <math.h>

/*****************************************************************************
|*
|* FUNCTION:           modf
|*
|* AVAILABILITY:       EXTERNAL
|*
|* PARAMETERS:
|*
|*     Single or double precision floating point value to split
|*     and a pointer to the integer part to determine.
|*
|* RETURN VALUE:
|*
|*     The fraction of the passed variable.
|*
|* DESCRIPTION:
|*
|*     modf implementation for IEEE-754 single precision/double precision floating point format.
|*
|*     No need for error handling since it can not occur in this function.
|*     See ANSI-C X3J11 88-001, standard math library definition on modf (Paragraph 4.5.4.6).
|*
|*     Split a single precision floating point value into an integer and a
|*     fraction. The abs(fraction) will be less than one and of the same type
|*     as the passed variable has. The integer value will also be of the
|*     passed variable's type.
|*     Both parts will have the sign of the argument.
|*
|*     The two parts should together still be representing the passed variable's
|*     value, so passed_value = integer_value + fraction.
|*     They both have the same sign as the argument.
|*
|*     If the argument is zero, zero will be returned.
*/
#if _IEEE_754_FORMAT
#if _SINGLE_FP
float	modf( float f, float *intval )
{
	float2long_t	u;
	unsigned long	l;
	short		exp;

	/* this assumes that the float internally has the *same* endianess as the long */
	u.f	= f;
	l	= u.l;

	exp	= GET_FLOAT_EXPONENT( l );
	exp	-= FLOAT_BIAS;
	/* test if too small, to prevent too many shifts below */
	if ( exp < 0 )
	{
		/* integer is zero */
		*intval	= copysignf( 0.0, f );
		return f;
	}

	/*
	 * for IEEE-754 single precision, an (unbiased!) exponent >= FLOAT_FRACTION_SIZE tells us
	 * that the fraction == 0.0
	 * So check if the value EXCEEDS +-1 * 2 ^ 22
	*/
	if ( exp >= FLOAT_FRACTION_SIZE )
	{
		/* so the fraction is zero */
		*intval	= f;
		return copysignf( 0.0, f );
	}

	/* here the float is <-1*2^FLOAT_FRACTION_SIZE..+1*2^FLOAT_FRACTION_SIZE> */
	/* now clear the least significant bits, which would drop off at the integer */
	/* that are the last FLOAT_FRACTION_SIZE - exp bits */
	l	&= 0xffffffff << (FLOAT_FRACTION_SIZE - exp);

	u.l	= l;

	*intval	= u.f;
	return f - *intval;
}
#else
double	modf( double d, double *intval )
{
	double2longs_t	u;
	unsigned long	lo;
	unsigned long	hi;
	short		exp;
	int		shift_amount;

	/* this assumes that the double internally has the *same* endianess as the long */
	u.d	= d;
	hi	= u.s.hi;
	lo	= u.s.lo;

	exp	= GET_DOUBLE_EXPONENT( hi );
	exp	-= DOUBLE_BIAS;
	if ( exp < 0 )
	{
		/* the integer is zero */
		*intval	= copysign( 0.0, d );
		return d;
	}

	/*
	 * for IEEE-754 double precision, an (unbiased!) exponent >= DOUBLE_FRACTION_SIZE tells us
	 * that the fraction == 0.0
	*/
	if ( exp >= DOUBLE_FRACTION_SIZE )
	{
		/* so the fraction is zero */
		*intval	= d;
		return copysign( 0.0, d );
	}

	/* here the double is <-1*2^DOUBLE_FRACTION_SIZE..-1*2^0] or [+1*2^0..+1*2^DOUBLE_FRACTION_SIZE> */

	/*
	 * now it really gets though
	 * We must get a double with only those bits in the mantissa set
	 * which are part of the integer value. Then the fraction is simply found by subtracting
	 * the "integer" from the passed parameter.
	 * So we have to get the exponent's value, and mask the mantissa
	 * with (2 ^ (DOUBLE_FRACTION_SIZE - exp)) - 1
	 * This is too big for longs -> split
	 *
	 * Now clear the least significant bits, which would drop off at the integer
	 * being the last DOUBLE_FRACTION_SIZE - exp bits
	*/
	/* 0 <= exp < DOUBLE_FRACTION_SIZE */
	shift_amount	= DOUBLE_FRACTION_SIZE - exp;
	if ( shift_amount >= 32 )
	{
		/* all bits of the lo and some of the hi are to become zero */
		lo	= 0;
		/* need to clear bits in the hi-word as well */
		hi	&= 0xffffffff << (shift_amount - 32);
	}
	else
	{
		/* only bits of the lo need to be zeroed */
		lo	&= 0xffffffff << shift_amount;
	}

	u.s.hi	= hi;
	u.s.lo	= lo;

	*intval	= u.d;
	return d - *intval;
}
#endif
#else
#  error not implemented (yet)
#endif /* IEEE 754 format */
