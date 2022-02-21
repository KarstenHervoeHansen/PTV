/*
 *	Version : @(#)sinh.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  sinh.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for sinh() routine                         *
**                 Computes the 'hyperbolic sinus' for the given angle    *
**                 (in radians)                                           *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>
/*
	sinh(arg) returns the hyperbolic sine of its floating-
	point argument.

	The exponential function is called for arguments
	greater in magnitude than 0.5.

	A series is used for arguments smaller in magnitude than 0.5.
	The double coefficients are #2027 from Hart & Cheney. (16.45D)
	The single coefficients are #2021 from Hart & Cheney. ( 6.98D)

	cosh(arg) is computed from the exponential function for
	all arguments.
*/

#ifndef _SINGLE_FP
#define	P0   0.6282813250833546179346e4
#define	P1   0.9178748106756360272984e3
#define	P2   0.3181332190110303793958e2
#define	P3   0.336083852466226007246e0
#define	Q0   0.6282813250833546241924e4
#define	Q1  -0.1292607311299731203424e3
#else
#define	P0  -0.2019614849422e2
#define	P1  -0.236617830421e1
#define	Q0  -0.2019615061071e2
#endif
#define LOG_HALF -0.693147180559945309417232121 /* ln(0.5) */

double
sinh( double arg )
{
	double temp, argsq;
	register char sign = 0;

	sign = 1;
	if( arg < 0.0 )
	{
		arg = -arg;
		sign = -1;
	}

	if( arg > 37.0 )
	{
		temp = exp( arg + LOG_HALF );
			
		if ( sign )
			return( temp );
		else
			return( -temp );
	}

	if( arg > 0.5 )
	{
		return( sign * ( exp( arg + LOG_HALF ) - exp( -arg + LOG_HALF ) ) );
	}

	argsq = arg*arg;
#ifndef _SINGLE_FP
	temp = ( ( ( P3 * argsq + P2 ) * argsq + P1 ) * argsq + P0 ) * arg;
	temp /= ( ( argsq + Q1 ) * argsq + Q0 );
#else
	temp = ( P1 * argsq + P0 ) * arg;
	temp /= ( argsq + Q0 );
#endif
	return( sign * temp );
}
