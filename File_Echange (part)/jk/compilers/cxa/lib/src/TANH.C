/*
 *	Version : @(#)tanh.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  tanh.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for tanh() routine                         *
**                 Computes the 'hyperbolic tangent' for the given angle  *
**                 (in radians)                                           *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

double
tanh( double arg )
{
	double sign;

	sign = 1.0;
	if( arg < 0.0 )
	{
		arg = -arg;
		sign = -1.0;
	}

	if( arg > 37.0 )
		return( sign );

	return( sign * sinh( arg ) / cosh( arg ) );
}
