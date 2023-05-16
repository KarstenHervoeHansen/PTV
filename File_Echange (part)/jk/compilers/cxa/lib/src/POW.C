/*
 *	Version : @(#)pow.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  pow.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for pow() routine                          *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <errno.h>
#include <math.h>

double
pow( double arg1, double arg2 )
{
	long	l;

	if( arg1 <= 0.0 )
	{
		if( arg1 == 0.0 )
		{
			if( arg2 <= 0.0 )
				errno = EDOM;
			return( 0.0 );
		}
		l = arg2;
		if( l != arg2 )
		{
			errno = EDOM;
			return( 0.0 );
		}
		arg1 = exp( arg2 * log( -arg1 ) );
		return( (l & 1) ? -arg1 : arg1 );
	}
	return( exp( arg2 * log( arg1 ) ) );
}
