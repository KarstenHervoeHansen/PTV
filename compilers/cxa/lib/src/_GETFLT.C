/*
 *	Version : @(#)_getflt.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _getflt.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for _getfloat() routine                    *
**                 This file is only linked when 'floating point' is used *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <float.h>
#include <ctype.h>

int	_getfloat( unsigned char, unsigned int, void *, FILE *, unsigned int * );

/* Return '0' if the conversion has success */
int
_getfloat( unsigned char type, unsigned int length, void * ap, 
	   FILE * fin, unsigned int *characters )
{
	register int		c;
	register unsigned char	sign = 0;
	double			value = 0.0;
	double			fract = 0.1;
	register int		exp;

	while( isspace( ((*characters)++, c = fgetc( fin )) ) )	/* read whitespace */
		;

	/* First we use 'exp' as an 'OK' indicator */
	/* Conversion is OK when the string of digits is not empty (not including the sign) */
	exp = 1;	/* When '0' afterwards, possible conversion can be done */

	if( c == '-' || c == '+' )			/* First check for sign */
	{
		if( c == '-' )
			sign = 1;
		(*characters)++;
		c = fgetc( fin );
	}

	while( isdigit( c ) )
	{
		value = value * 10.0 + (c - '0');
		(*characters)++;
		c = fgetc( fin );
		exp = 0;			/* Conversion OK */
	}

	if( c == '.' )
	{
		while( isdigit( ((*characters)++, c = fgetc( fin )) ) )
		{
			value = fract * (c - '0') + value;
			fract /= 10.0;
			exp = 0;			/* Conversion OK */
		}
	}

	if( exp )	/* Still no valid character found, no conversion can be done */
	{
		/* No success */
		return( 1 );
	}

	if( sign )			/* Make negative when neccessary */
		value = -value;

	sign = 0;

	if( (c == 'e') || (c == 'E') )	/* Check for 'exponent' */
	{
		(*characters)++;
		c = fgetc( fin );
		if( c == '-' || c == '+' )		/* Check for sign */
		{
			if( c == '-' )
				sign = 1;
			(*characters)++;
			c = fgetc( fin );
		}
	
		while( isdigit( c ) )			/* Read the exponent value */
		{
			exp = exp * 10 + (c - '0');
			(*characters)++;
			c = fgetc( fin );
		}

		/* Cut the exponent at 2*DBL_MAX_10_EXP */
		if( exp > 2*DBL_MAX_10_EXP )
			exp = 2*DBL_MAX_10_EXP;

		if( sign )	/* Negative exponent */
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
				value /= 1.0E10;	/* value /= 1.0E10; */
				exp -= 10;
			}
			while( exp >= 1 )
			{
				value /= 10.0;	/* value /= 1.0E1; */
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
				value *= 10.0;
				exp -= 1;
			}
		}
	}

	ungetc( c, fin );	/* Last read character was not used yet */

	if( type == 'l' || type == 'L' )	/* Double ? */
		*(double *)ap = value;
	else					/* Float */
		*(float *)ap = value;

	length; 		/* prevent warning 'parameter not used' */

	return( 0 );		/* Success */
}
