/*
 *	Version : @(#)cosh.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  cosh.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for cosh() routine                         *
**                 Computes the 'hyperbolic cosinus' for the given angle  *
**                 (in radians)                                           *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

#define LOG_HALF -0.693147180559945309417232121 /* ln(0.5) */

double
cosh( double arg )
{
	if( arg < 0.0 )
		arg = -arg;

	if( arg > 37.0 )
	{
		return( exp( arg + LOG_HALF ) );
	}

	return( ( exp( arg + LOG_HALF ) + exp( -arg + LOG_HALF ) ) );
}
