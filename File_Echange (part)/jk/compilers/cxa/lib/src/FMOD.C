/*
 *	Version : @(#)fmod.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fmod.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for fmod() routine                         *
**                 Returns the floating point remainder of the division   *
**		   of x by y, zero if y is zero or if x/y would overflow. *
**		   0 <= | remainder |<| y | and sign remainder is sign x. *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <math.h>

double
fmod( double x, double y )
{
	double n;

	if( y == 0.0 )
		return( 0.0 );

	if( (y < 0.0 ? -y : y) < (x < 0.0 ? -x : x) )
		return( y * modf( x / y, &n) );
	else
		if( (y < 0.0 ? -y : y) > (x < 0.0 ? -x : x) )
			return( x );
		else
			return( 0.0 );
}
