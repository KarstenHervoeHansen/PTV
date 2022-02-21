/*
 *	Version : @(#)_atan.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _atan.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for _atan() routine                        *
**                 Is used by 'atan()' and 'atan2()' routines             *
**                                                                        *
**      coefficients double are #5076 from Hart & Cheney. (17.55D)        *
**      coefficients single are #5071 from Hart & Cheney. ( 7.54D)        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

static double xatan( double arg );

#define	SQ2P1	2.414213562373095048802e0
#define	SQ2M1	0.414213562373095048802e0
#define	PIO2	1.570796326794896619231e0
#define	PIO4	0.785398163397448309615e0
#ifndef _SINGLE_FP
#define	P0	0.445413400592906803197511e2
#define	P1	0.77477687719204208616481e2
#define	P2	0.40969264832102256374186e2
#define	P3	0.666057901700926265753e1
#define	P4	0.1589740288482307048e0
#define	Q0	0.445413400592906804445995e2
#define	Q1	0.92324801072300974840693e2
#define	Q2	0.62835930511032376833267e2
#define	Q3	0.1550397755142198752523e2
#else
#define P0	0.1265998609915e2
#define	P1	0.63691887127e1
#define	Q0	0.1265998646243e2
#define Q1	0.105891113168e2
#endif

/*
	_atan reduces its argument (known to be positive)
	to the range [0,0.414...] and calls xatan.
*/
double
_atan( double arg )
{
	if( arg < SQ2M1 )
		return( xatan( arg ) );
	else if( arg > SQ2P1 )
		return( PIO2 - xatan( 1.0 / arg ) );
	else
		return( PIO4 + xatan( ( arg - 1.0 ) / ( arg + 1.0 ) ) );
}

/*
	xatan evaluates a series valid in the
	range [-0.414...,+0.414...].
*/
static double
xatan( double arg )
{
	double argsq;

	argsq = arg * arg;
#ifndef _SINGLE_FP
	return(
		(
			( ( ( ( P4 * argsq + P3 ) * argsq + P2 ) * argsq + P1 ) * argsq + P0 ) /
			( ( ( ( argsq + Q3 ) * argsq + Q2 ) * argsq + Q1 ) * argsq + Q0 )
		) * arg
	      );
#else
	return(
		( 	( P1 * argsq + P0 ) /
			( ( argsq + Q1 ) * argsq + Q0 ) 
		) * arg
	      );
#endif	      
}
