/*
 *	Version : @(#)exp.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  exp.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for exp() routine                          *
**                                                                        *
**       The double coefficients are #1067 from Hart and Cheney. (18.08D) *
**       The single coefficients are #1063 from Hart and Cheney. (10.03D) *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

/*
	exp returns the exponential function of its
	floating-point argument.

*/

#include <errno.h>
#include <math.h>

#ifndef _SINGLE_FP
#define	P0	0.1513906799054338915894328e4
#define	P1	0.20202065651286927227886e2
#define	P2	0.23093347753750233624e-1
#define	Q0	0.4368211662727558498496814e4
#define	Q1	0.233184211427481623790295e3
#else
#define	P0	0.72152891511493e1
#define	P1	0.576900723731e-1
#define	Q0	0.208189237930062e2
#endif

#define	LOG2E	1.4426950408889634073599247
#define	SQRT2	1.4142135623730950488016887

#ifdef _SINGLE_FP
#define	MAXF	89.0
#else
#define	MAXF	710.0
#endif

double
exp( double arg )
{
	double	fract;
	double	xsq;
	int	ent;

	if( arg == 0.0 )
		return( 1.0 );
	if( arg < -MAXF )
		return( 0.0 );
	if( arg > MAXF )
	{
		errno = ERANGE;
		return( HUGE_VAL );
	}
	arg *= LOG2E;
	ent = floor( arg );
	fract = ( arg - ent ) - 0.5;
	xsq = fract * fract;

#ifndef _SINGLE_FP
	fract = ( ( P2 * xsq + P1 ) * xsq + P0 ) * fract;
	xsq = ( xsq + Q1 ) * xsq + Q0;
	return( ldexp( SQRT2 * ( xsq + fract ) / ( xsq - fract ), ent ) );
#else
	fract = ( P1 * xsq + P0 ) * fract;
	xsq = xsq + Q0;
	return( ldexp( SQRT2 * ( xsq + fract ) / ( xsq - fract ), ent ) );
#endif
}
