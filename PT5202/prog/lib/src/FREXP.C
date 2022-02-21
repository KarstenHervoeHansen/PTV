/**************************************************************************
**                                                                        *
**  FILE        :  frexp.c                                                *
**                                                                        *
**  DESCRIPTION :  frexp implementation for IEEE-754 single precision/    *
**                 double precision floating point format.                *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <float.h>
#include <math.h>

/*****************************************************************************
|*
|* FUNCTION:           frexp
|*
|* AVAILABILITY:       EXTERNAL
|*
|* PARAMETERS:
|*
|*     Single precision floating point value (to split into its mantissa and
|*     signed unbiased exponent) and a pointer to the exponent value.
|*
|* RETURN VALUE:
|*
|*     The fraction of the passed variable.
|*
|* DESCRIPTION:
|*
|*     frexp implementation for IEEE-754 single precision/double precision floating point format.
|*     See ANSI-C X3J11 88-001, standard math library definition on frexp (Paragraph 4.5.4.2).
|*
|*     To set ERANGE in errno on underflow is implementation defined, so lets leave it out.
|*
|*     Split a single precision floating point value into a mantissa and a
|*     signed unbiased exponent. The mantissa will be returned as a variable of the passed
|*     variable's type and be in the range of [1/2, 1) (so with an exponent of -1).
|*     The exponent will be unbiased and a signed integer.
|*
|*     The two parts together should still be representing the passed variable's
|*     value, so passed_value = fraction * 2^exponent.
|*     Also, the resulting mantissa has the sign of the passed value.
|*
|*     If the passed value is zero, zero will be returned for both results.
*/
#if _IEEE_754_FORMAT
#if _SINGLE_FP
float	frexp( float f, int *exponent )
{
	float2long_t	u;
	unsigned long	l;
	int		exp;
	unsigned long	mantissa;

	/* this assumes that the float internally has the *same* endianess as the long */
	u.f	= f;
	l	= u.l;
	exp	= GET_FLOAT_EXPONENT( l );

	/* test the mantissa == 0 in case denormal values are passed */
	mantissa	= GET_FLOAT_MANTISSA( l );
	if ( exp == 0 && mantissa == 0 )
	{
		*exponent	= 0;
		return u.f;		
	}

	*exponent	= exp - FLOAT_BIAS + 1;	/* the unbiased exponent of the returned value */

	/*
	 * Now convert the passed variable from 1.0 <= |mantissa| < 2.0 to 0.5 <= |mantissa| < 1.0.
	 * The mantissa stays the same but its new exponent is loaded with sp_bias-1. (1.0 * 2^-1 = 0.5)
	*/
	exp	= FLOAT_BIAS - 1;
	l	= PUT_FLOAT_EXPONENT( STRIP_FLOAT_EXPONENT( l ), exp );
	u.l	= l;

	return u.f;
}
#else
double	frexp( double d, int *exponent )
{
	double2longs_t	u;
	unsigned long	lo;
	unsigned long	hi;
	int		exp;
	unsigned long	mantissa_hi;

	/* this assumes that the double internally has the *same* endianess as the long */
	u.d	= d;
	hi	= u.s.hi;
	lo	= u.s.lo;

	exp	= GET_DOUBLE_EXPONENT( hi );

	/* test the mantissa == 0 in case denormal values are passed -> denormals are not tested... */
	mantissa_hi	= GET_DOUBLE_HI_MANTISSA( hi );
	if ( exp == 0 && mantissa_hi == 0 && GET_DOUBLE_LO_MANTISSA( lo ) == 0 )
	{
		/* got zero, return that */
		*exponent	= 0;
		return u.d;		
	}

	*exponent	= exp - DOUBLE_BIAS + 1;	/* the unbiased exponent of the returned value */

	/*
	 * Now convert the passed variable from 1.0 <= |mantissa| < 2.0 to 0.5 <= |mantissa| < 1.0.
	 * The mantissa stays the same but its new exponent is loaded with dp_bias-1. (1.0 * 2^-1 = 0.5)
	*/
	exp	= DOUBLE_BIAS - 1;
	hi	= PUT_DOUBLE_EXPONENT( STRIP_DOUBLE_EXPONENT( hi ), exp );
	u.s.hi	= hi;

	return u.d;
}
#endif
#else
#  error not implemented (yet)
#endif /* IEEE 754 format */
