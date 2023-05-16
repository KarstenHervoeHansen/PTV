/*
 *	Version : @(#)sqrt.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  sqrt.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for sqrt() routine                         *
**                 Computes the 'square' root for the given value         *
**                                                                        *
**                 Uses the 'Newton' method                               *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>
#include <errno.h>
#include <stdlib.h>

double
sqrt( double arg )
{
	double	temp;
	int	exp;

	if( arg <= 0.0 )
	{
		if( arg < 0.0 )
			errno = EDOM;
		return( 0.0 );
	}
	temp = frexp( arg, &exp );
	if( exp & 1 )
	{
		temp *= 2.0;
		exp--;
	}
	temp = 0.5 * ( 1.0 + temp );

	while( exp > 60 )
	{
		temp *= 1.073741824e9; /* ( 1L << 30 ); */
		exp -= 60;
	}
	while( exp < -60 )
	{
		temp /= 1.073741824e9; /* ( 1L << 30 ); */
		exp += 60;
	}
	if( exp >= 0 )
		temp *= 1L << ( exp / 2 );
	else
		temp /= 1L << ( -exp / 2 );

	/* Just use 'exp' as a loop counter now */
	for( exp=0; exp<=4; exp++ )
		temp = 0.5 * ( temp + arg / temp );

	return( temp );
}
