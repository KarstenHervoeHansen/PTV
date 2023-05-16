/*
 *	Version : @(#)strtod.c	1.5	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strtod.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for strtod() routine                       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>
#include <float.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

double
strtod( register const char *p, char ** ptr )
{
	register int		c;
	register unsigned char	sign = 0;
	double			value = 0.0;
	double			fract = 0.1;
	register int		exp;
	register const char *	sc;

	sc = p;	/* We need the value 'p' when anything goes wrong */

	while( isspace( c = *sc++ ) )		/* read whitespace */
		;

	/* First we use 'exp' as an 'OK' indicator */
	/* Conversion is OK when the string of digits is not empty (not including the sign) */
	exp = 1;	/* When '0' afterwards, conversion can be done */

	if( c == '-' || c == '+' )		/* First check for sign */
	{
		if( c == '-' )
			sign = 1;
		c = *sc++;
	}

	while( isdigit( c ) )
	{
		value = value * 10.0 + (c - '0');
		c = *sc++;
		exp = 0;			/* Conversion OK */
	}

	if( c == '.' )
	{	/* Just a '.' is not a valid conversion */
		while( isdigit( c = *sc++ ) )
		{
			value = fract * (c - '0') + value;
			fract *= 0.1;			/* fract /= 10.0; */
			exp = 0;			/* Conversion OK */
		}
	}

	if( exp )		/* Still no valid character found, no conversion can be done */
	{
		if( ptr != NULL )
			*ptr = (char *)p;	/* No conversion done */
		return( 0.0 );
	}

	if( sign )			/* Make negative when necessary */
		value = -value;

	sign = 0;

	if( (c == 'e') || (c == 'E') )	/* Check for 'exponent' */
	{
		c = *sc++;
		if( c == '-' || c == '+' )		/* Check for sign */
		{
			if( c == '-' )
				sign = 1;
			c = *sc++;
		}
	
		while( isdigit( c ) )			/* Read the exponent value */
		{
			exp = exp * 10 + (c - '0');
			c = *sc++;
			
			/* Cut the exponent at 2*DBL_MAX_10_EXP */
			if( exp > 2*DBL_MAX_10_EXP )
			{
			    exp = 2*DBL_MAX_10_EXP;
			    errno = ERANGE;
			    if ( sign )
				return( 0.0 );		/* underflow */
			    else if( value < 0.0 )	/* else overflow */
				return( - HUGE_VAL );
			    else
				return( HUGE_VAL );
			}
		}

		if( sign )				/* Negative exponent */
		{
#ifndef	_SINGLE_FP
			while( exp >= 100 )
			{
				value *= 1.0E-100;	/* value /= 1.0E100; */
				exp -= 100;
			}
#endif
			while( exp >= 10 )
			{
				value *= 1.0E-10;	/* value /= 1.0E10; */
				exp -= 10;
			}
			while( exp >= 1 )
			{
				value *= 1.0E-1;	/* value /= 1.0E1; */
				exp -= 1;
			}
		}
		else		/* Positive exponent */
		{
#ifndef	_SINGLE_FP
			while( exp >= 100 )
			{
				value *= 1.0E100;
				exp -= 100;
			}
#endif
			while( exp >= 10 )
			{
				value *= 1.0E10;
				exp -= 10;
			}
			while( exp >= 1 )
			{
				value *= 1.0E1;
				exp -= 1;
			}
		}
	}

	if( ptr != NULL )
		*ptr = (char*)--sc;	/* Last character was not used */

	return( value );
}
