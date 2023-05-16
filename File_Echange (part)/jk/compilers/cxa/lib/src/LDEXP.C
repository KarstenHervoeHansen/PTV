/*
 *	@(#)ldexp.c	1.9
 */

/**************************************************************************
**                                                                        *
**  FILE        :  ldexp.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for ldexp() routine                        *
**                 Returns x * (2 to the power n)                         *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<errno.h>
#include	<float.h>
#include	<math.h>

#ifdef	_SINGLE_FP
#  define 	BIAS	FLOAT_BIAS
#else
#  define 	BIAS	DOUBLE_BIAS
#endif

double
ldexp( double fr, int pwr )
{
#if _IEEE_754_FORMAT
#if _SINGLE_FP /* ------------------------------------------------------- */
	float2long_t	u;
	unsigned long	l;
	signed short	exp;

	/* this assumes that the float internally has the *same* endianess as the long */
	u.f	= fr;
	l	= u.l;

	exp	= GET_FLOAT_EXPONENT( l ) + pwr;
	/* under/overflow test */
	if ( pwr > 2 * BIAS || exp > MAX_FLOAT_BIASED_EXP )
	{
		errno	= ERANGE;
		return copysignf( HUGE_VAL, fr );
	}
	if ( exp < MIN_FLOAT_BIASED_EXP )
	{
		/* no ERANGE required, since that's implementation defined */
		return copysignf( 0.0, fr );
	}

	l	= PUT_FLOAT_EXPONENT( STRIP_FLOAT_EXPONENT( l ), exp );
	u.l	= l;

	return u.f;
#else
	double2longs_t	u;
	unsigned long	hi;
	signed short	exp;

	/* this assumes that the double internally has the *same* endianess as the long */
	u.d	= fr;
	hi	= u.s.hi;

	exp	= GET_DOUBLE_EXPONENT( hi ) + pwr;
	/* under/overflow test */
	if ( pwr > 2 * BIAS || exp > MAX_DOUBLE_BIASED_EXP )
	{
		errno	= ERANGE;
		return copysign( HUGE_VAL, fr );
	}
	if ( exp < MIN_DOUBLE_BIASED_EXP )
	{
		/* no ERANGE required, since that's implementation defined */
		return copysign( 0.0, fr );
	}

	hi	= PUT_DOUBLE_EXPONENT( STRIP_DOUBLE_EXPONENT( hi ), exp );
	u.s.hi	= hi;

	return u.d;
#endif /* ------------------------------------------------------- */
#else
	int		exp;
	register char	neg = 0;

	neg = 0;
	if ( fr < 0.0 )
	{
		fr = -fr;
		neg = 1;
	}
	fr = frexp( fr, &exp );

	if ( pwr > BIAS || pwr + exp > BIAS  )
	{
		errno = ERANGE;

		if ( neg )
			return( -HUGE_VAL );
		else
			return( HUGE_VAL );
	}

	exp += pwr;

	if ( exp < -(BIAS-2) )
		return( 0 );

	while ( exp > 30 )
	{
		fr *= 1.073741824e9;	/* fr *= 1L << 30; */
		exp -= 30;
	}
	while ( exp < -30 )
	{
		fr /= 1.073741824e9;	/* fr /= 1L << 30; */
		exp += 30;
	}
	if ( exp > 0 )
		fr *= 1L << exp;
	if ( exp < 0 )
		fr /= 1L << -exp;
	if ( neg )
		fr = -fr;
	return( fr );
#endif
}
